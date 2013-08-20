#include <whereswally/patterns/pattern_tools.h>
#include <map>
#include <opencv2/highgui/highgui.hpp>
#include <omp.h>
#include <ctime>

using namespace std;
using namespace cv;

//**
//**  CLASS INITIALISATION
//**

wwp::region::region() {
  largest_x = 0;
  largest_y = 0;
  smallest_x = INT_MAX;
  smallest_y = INT_MAX;
  size = 0;
  av_x = 0.0; 
  av_y = 0.0;
}


//**
//**  EQUIVALENCE RELATIONSHIPS
//**

bool wwp::operator==(wwp::region lhs, wwp::region rhs) {
  bool same = true;
  if(lhs.size != rhs.size) same = false; 
  if(fabs(lhs.av_x -rhs.av_x) > 0.1 ) same = false;
  if(fabs(lhs.av_y -rhs.av_y) > 0.1 ) same = false;
  if(lhs.smallest_x != rhs.smallest_x) same = false;
  if(lhs.smallest_y != rhs.smallest_y) same = false;
  if(lhs.largest_x != rhs.largest_x) same = false;
  if(lhs.largest_y != rhs.largest_y) same = false;
  return same;
}

bool wwp::operator<(wwp::homography lhs, wwp::homography rhs) {
  if(lhs.a > rhs.a) {
    return true;
  } else if(lhs.a < rhs.b) {
    return false;
  }
  if(lhs.b > rhs.b) {
    return true;
  } else if(lhs.b < rhs.b) {
    return false;
  }
  if(lhs.c > rhs.c) {
    return true;
  } else if(lhs.c < rhs.c) {
    return false;
  }
  if(lhs.d > rhs.d) {
    return true;
  } else if(lhs.d < rhs.d) {
    return false;
  }
  return false;
}

//**
//**  COLOUR ANALYSIS TOOLS
//**

//-- finds regions using the quick Connected Component Labelling algorithm
vector<wwp::region> wwp::fast_find_regions(Mat input) {
  vector< vector<int> > region_array(input.rows);
    
  for(int i=0; i<input.rows; i++) {
    region_array[i].resize(input.cols);
    for(int j=0; j<input.cols; j++) {
      region_array[i][j] = -1;
    }
  }

  int region_count = 0;
  vector<int> region_equivalence;
  //-- loop that calculates roughly correct regions
  #pragma omp parallel for default(none) shared(input, region_array, region_equivalence, region_count)
  for(int i=0; i<input.rows; i++) {
    for(int j=0; j<input.cols; j++) {
      //-- if current pixel is zero-valued, then move onto next pixel
      if(input.at<uchar>(i,j) == 0 ) {
        region_array[i][j] = -1;
        continue;
      }
      
      //-- get values of neighbours to the north and west, unless pixel is on the north or west border
      int n_neighbour, w_neighbour;
      if(i==0) {
        n_neighbour = 0;
      } else {
        n_neighbour = region_array[i-1][j];
      }

      if(j==0) { 
        w_neighbour = 0;
      } else {
        w_neighbour = region_array[i][j-1];
      }

      //-- if there are different regions north and west of the current pixel, define an equivalence between them
      if(n_neighbour >= 0 && w_neighbour >= 0 && n_neighbour != w_neighbour) {
        int reg, equiv;
        if(n_neighbour > w_neighbour) {
          reg = n_neighbour;
          equiv = w_neighbour;
        } else {
          reg = w_neighbour;
          equiv = n_neighbour;
        }
        region_equivalence[reg] = equiv;
      }
  
      //-- if the north neighbour's region is non-zero, current pixel joins that region
      //-- if north is zero but west's region is not, current pixel joins west region
      //-- otherwise, define a new region
      if(n_neighbour >= 0) {
        region_array[i][j] = n_neighbour;
      } else if (w_neighbour >= 0) {
        region_array[i][j] = w_neighbour;
      } else {
          region_equivalence.push_back(region_count);
          region_array[i][j] = region_count;
          region_count++;
      }
    }
  }

  //-- remove redundant definitions, e.g. if reg(3) == 2 && reg(2) == 1, then reg(1) is reassigned to 1
  for(size_t i=0; i<region_equivalence.size(); i++) {
    int equiv_new=region_equivalence[i], equiv_old = -1;
    //-- while current region is not equivalent to itself
    while(equiv_new != equiv_old) {
      equiv_old = equiv_new;
      equiv_new = region_equivalence[equiv_new];
    }
      region_equivalence[i] = equiv_new;
  }

  //-- add each region to a list, and determine some basic information about it
  vector<wwp::region> region_list;
  for(int i=0; i<input.rows; i++) {
    for(int j=0; j<input.cols; j++) {
      if(region_array[i][j] == -1) {continue;}
      size_t val = region_equivalence[region_array[i][j]]; //-- adds this pixel to the region that it's current region is equivalent to
      if(region_list.size() < val+1) {
        region_list.resize(val+1);
      }
      if(region_list[val].smallest_x > j) region_list[val].smallest_x = j;
      if(region_list[val].smallest_y > i) region_list[val].smallest_y = i;
      if(region_list[val].largest_x < j) region_list[val].largest_x = j;
      if(region_list[val].largest_y < i) region_list[val].largest_y = i;
      region_list[val].size+=1;
      region_list[val].av_x+=j;
      region_list[val].av_y+=i;
    }
  }

  //-- normalize average positions and add to the list of regions discovered
  vector<wwp::region> found_regions;
  for(vector<wwp::region>::iterator it=region_list.begin(); it!=region_list.end(); ++it) {
    it->av_x /= it->size;
    it->av_y /= it->size;
    found_regions.push_back(*it);
  }
  return found_regions;
}

double wwp::estimate_black_line_thickness(Mat image, int limit, int tolerance) {
  int num_threads = omp_get_max_threads();
 
  //-- get mask that describes where black lines are in sharpened image
  Mat black_image = get_greyscale_in_image(image, 0, limit, tolerance);
  black_image = (black_image > 0);

  //-- find the minimum distance between each pixel and the nearest empty pixel, and calculate the average
  Size image_size(image.cols, image.rows);
  Mat distance_map(image_size, CV_8U, Scalar(0));
  distanceTransform(black_image, distance_map, CV_DIST_L2,5); //CV_DIST_L1 indicates distances are calculated with manhattan distance

  //-- calculate the range of zero-distance pixels
  double min_distance=DBL_MAX, max_distance=0;
  minMaxIdx(distance_map, &min_distance, &max_distance, NULL, NULL, black_image);
  black_image.release();
  int range = max_distance-min_distance;

	int count=0;
  double best_width=0;
  #pragma omp parallel for default(none) firstprivate(range,max_distance, num_threads) shared(image, distance_map) reduction(+: best_width) reduction(+: count)
	for(int i=0; i<num_threads; i++) {
    Mat subdist = distance_map.rowRange(i*(image.rows-1)/num_threads, (i+1)*(image.rows-1)/num_threads);
		int minimum_difference_in_predictions = 1;
		for(int j=0; j<range; j++) {
			double average_prediction, deviation_prediction;
			double standard_deviation, sum_deviation, average_distance_to_zero;
			int count_zero;

			//-- create a mask showing the location of all pixels that are not max_distance away from a zero pixel
			Mat dist_tmp = (subdist < max_distance-j) & ( subdist > 0 );
			distanceTransform(dist_tmp, dist_tmp, CV_DIST_C,3);
		 
      //-- calculate the average distance to zero
			average_distance_to_zero = (double)(sum(dist_tmp)[0])/countNonZero(dist_tmp);
  
      //-- count the  number of zero pixels in the subimage
			count_zero = dist_tmp.cols*dist_tmp.rows - countNonZero(dist_tmp);

      //-- calculate the standard deviation from average
			Mat deviation;
			pow(dist_tmp-average_distance_to_zero,2, deviation);
			sum_deviation = sum(deviation)[0];                            // sum_deviation includes the zero pixels, which we don't want to count
      deviation.release();
			sum_deviation -= count_zero*pow(average_distance_to_zero,2);  // so we remove a (0-average_distance_to_zero)^2 for each zero value
			standard_deviation = sqrt(sum_deviation/countNonZero(dist_tmp));

      //-- using the average and standard deviation, predict the width of the image lines
			average_prediction = (average_distance_to_zero-0.5)/0.25;
			deviation_prediction = (standard_deviation)*(4.0*sqrt(3.0));

      //-- find the predictions that are closest together
			if( fabs(average_prediction-deviation_prediction) < minimum_difference_in_predictions) {
				minimum_difference_in_predictions = fabs(average_prediction-deviation_prediction);
				best_width = (average_prediction+deviation_prediction)/2;
				count = 1;
			}
		}
    subdist.release();
	}
  //-- release uneeded reference to distance_map
  distance_map.release();

  //-- return the average prediction per processor
  return best_width/count;
}

Mat wwp::get_greyscale_in_image(Mat image, int low_in, int high_in, int tolerance) {
  int high,low;
  Mat grey;
  //-- user can define low_in and high_in in any order, find the lowest and highest of the two
  high = (high_in>low_in) ? high_in : low_in;
  low  = (high_in>low_in) ? low_in : high_in;

	int num_threads = omp_get_max_threads();
	Mat out(image.size(), CV_8U);
  #pragma omp parallel for default(none) firstprivate(high,low,num_threads, tolerance) shared(image,out)
	for(int i=0; i<num_threads; i++) {
    //-- split the image into the BGR decomposition (reverse of RGB)
		Mat bgr[3];
		split(image.rowRange(i*(image.rows-1)/num_threads,(i+1)*(image.rows-1)/num_threads), bgr);

		//-- check if bgr values are within tolerance of the requested values
		Mat blue = (bgr[0] >= low-tolerance) & (bgr[0] <= high+tolerance);
		Mat green = (bgr[1] >= low-tolerance) & (bgr[1] <= high+tolerance);
		Mat red = (bgr[2] >= low-tolerance) & (bgr[2] <= high+tolerance);

		//-- merges all positions that satisfy boundaries
		Mat color_magnitude = blue & green & blue;

		//-- finds colours which are within tolerance of each other -> nearly greys
		Mat rg = ( (bgr[1]-tolerance) <= bgr[2] ) & ( bgr[2] <= bgr[1]+tolerance );
		Mat rb = ( (bgr[2]-tolerance) <= bgr[0] ) & ( bgr[0] <= bgr[2]+tolerance );
		Mat gb = ( (bgr[0]-tolerance) <= bgr[1] ) & ( bgr[1] <= bgr[0]+tolerance );
		Mat similar_colours = rg & rb & gb;
    
    //-- write the local array to the 'out' matrix
		Mat subout = out.rowRange(i*(out.rows-1)/num_threads,(i+1)*(out.rows-1)/num_threads);
		Mat result = color_magnitude & similar_colours;
		result.copyTo(subout);
	}
  return out;
}

//-- returns mask based on colours chosen
//-- user can define a range of allowable colours, and also an acceptable ratio
//-- acceptable ratios mean that 'red-ish' colours can be found, for example
Mat wwp::get_colour_in_image(Mat image, string colour_one, std::string colour_two, float redgreen, float greenred, float redblue, float bluered, float greenblue, float bluegreen) {
  int num_threads = omp_get_num_threads();
  Mat out(image.size(),CV_8U);
  //-- these hold the ranges for allowed colours
  int r[2],g[2],b[2];

  //-- parse the rgb strings (e.g. '#FFB3AD') into the distinct colour sections
  string r_hex[2] = {colour_one.substr(1,2),colour_two.substr(1,2)};
  string g_hex[2] = {colour_one.substr(3,2),colour_two.substr(3,2)};
  string b_hex[2] = {colour_one.substr(5,2),colour_two.substr(5,2)};

  //-- needed to convert hex strings to ints
  stringstream to_hex;
  to_hex.flags(ios_base::hex);

  //-- convert hex strings of colour_one to integer values
  to_hex << r_hex[0];
  to_hex >> r[0];
  to_hex.clear();
  to_hex << g_hex[0];
  to_hex >> g[0];
  to_hex.clear();
  to_hex << b_hex[0];
  to_hex >> b[0];
  to_hex.clear();

  //-- repeat for second string
  to_hex << r_hex[1];
  to_hex >> r[1];
  to_hex.clear();
  to_hex << g_hex[1];
  to_hex >> g[1];
  to_hex.clear();
  to_hex << b_hex[1];
  to_hex >> b[1];
  to_hex.clear();

  #pragma omp parallel for default(none) shared(image, out, cout,r, g, b) firstprivate(num_threads, redgreen, greenred, redblue, bluered, bluegreen, greenblue)
  for(int i=0; i<num_threads; i++) {
    Mat subimage = image.rowRange(i*(image.rows-1)/num_threads, (i+1)*(image.rows-1)/num_threads);
    Mat bgr[3];
    split(subimage, bgr);

    //-- find colours fulfill the colour range
    Mat red = ( bgr[2] >= r[0] ) & ( bgr[2] <= r[1] );
    Mat green = ( bgr[1] >= g[0] ) & ( bgr[1] <= g[1] );
    Mat blue = ( bgr[0] >= b[0] ) & ( bgr[0] <= b[1] );
    Mat saturation = red & green & blue;

    //-- find colours that fulfull the ratio range
    Mat rg = bgr[2] >= redgreen*bgr[1];
    Mat gr = bgr[1] >= greenred*bgr[2];
    Mat rb = bgr[2] >= redblue*bgr[0];
    Mat br = bgr[0] >= bluered*bgr[2];
    Mat gb = bgr[1] >= greenblue*bgr[0];
    Mat bg = bgr[0] >= bluegreen*bgr[1];

    //-- find the overlaps and write to global array
    Mat ratios = rg & gr & rb & br & gb & bg;
    Mat result = ratios & saturation;
    Mat subout = out.rowRange(i*(out.rows-1)/num_threads, (i+1)*(out.rows-1)/num_threads);
    result.copyTo(subout);
  }
  return out;
}
