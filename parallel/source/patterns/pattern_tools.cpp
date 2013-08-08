#include <whereswally/patterns/pattern_tools.h>
#include <map>

using namespace std;
using namespace cv;

wwp::region::region() {
  largest_x = 0;
  largest_y = 0;
  smallest_x = INT_MAX;
  smallest_y = INT_MAX;
}

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

void wwp::print_output(string message, int thread_num, int num_threads, string pattern_name) {
  stringstream output_stream;
//  cout << "\033[" << num_threads << "F" << flush;
  output_stream << "\033[s\033[" << num_threads-thread_num << "F\033[KThread(" << thread_num << ")[" << pattern_name << "]: " << message;
  output_stream << "\n\033[u";
//  cout << output_stream.str() << flush;
}

vector<wwp::region> wwp::fast_find_regions(Mat input) {
  int x = 0;
  int regions[input.rows][input.cols];
  cout << "okay" << endl;
  cout << regions << endl;
  x = 20;

  // initialise regions to 0
  for(int i=0; i<input.rows; i++) {
    for(int j=0; j<input.cols; j++) {
      regions[i][j] = 0;
    }
  }

  // calculate rough regions, and region equivalences
  int n_neighbour, w_neighbour;
  map<int, int> region_equivalence;
  int region_count = 0;
  for(int i=0; i<input.rows; i++) {
    for(int j=0; j<input.cols; j++) {
      if(i==0) { 
        w_neighbour = 0;
      } else {
        w_neighbour = input.at<uchar>(i-1,j);
      }
      if(j==0) {
        n_neighbour = 0;
      } else {
        n_neighbour = input.at<uchar>(i,j-1);
      }
      if(n_neighbour > 0 && w_neighbour > 0 && n_neighbour != w_neighbour) {
        int reg, equiv, tmp_reg = 0;
        if(n_neighbour > w_neighbour) {
          reg = n_neighbour;
          equiv = w_neighbour;
        } else {
          reg = w_neighbour;
          equiv = w_neighbour;
        }
        while(reg != tmp_reg) {
          tmp_reg = region_equivalence[reg];
          region_equivalence[reg] = equiv;
          reg = tmp_reg;
        }
        region_equivalence[reg] = equiv;
      }
      if(n_neighbour > 0) {
        regions[i][j] = n_neighbour;
      } else if (w_neighbour > 0) {
        regions[i][j] = w_neighbour;
      } else {
        region_count++;
        region_equivalence[region_count] = region_count;
        regions[i][j] = region_count;
      }
    }
  }

  // create a corrected region map
  for(int i=0; i<input.rows; i++) {
    for(int j=0; j<input.cols; j++) {
      if(regions[i][j] == 0) {continue;}
      regions[i][j] = region_equivalence[regions[i][j]];
    }
  }

  // add each region to a list, and determine some basic information about it
  map<int, wwp::region> region_list;
  for(int i=0; i<input.rows; i++) {
    for(int j=0; j<input.cols; j++) {
      if(region_list[regions[i][j]].smallest_x > i) region_list[regions[i][j]].smallest_x = i;
      if(region_list[regions[i][j]].smallest_y > j) region_list[regions[i][j]].smallest_y = j;
      if(region_list[regions[i][j]].largest_x < i) region_list[regions[i][j]].largest_x = i;
      if(region_list[regions[i][j]].largest_y < j) region_list[regions[i][j]].largest_y = j;
      region_list[regions[i][j]].size+=1;
      region_list[regions[i][j]].av_x+=i;
      region_list[regions[i][j]].av_y+=j;
    }
  }

  // normalize average positions and add them to the list of regions discovered
  vector<wwp::region> found_regions;
  for(map<int,wwp::region>::iterator it=++region_list.begin(); it!=region_list.end(); ++it) {
    it->second.av_x /= it->second.size;
    it->second.av_y /= it->second.size;
    found_regions.push_back(it->second);
  }

  return found_regions;
}

// Finds the distinct regions from a mask input.
// This is done by assigning each pixel a unique number
// and then each pixel takes the value of it's largest neighbour.
// Pixels with a value of zero remain at zero
vector<wwp::region> wwp::find_regions_from_mask(Mat input) {
  int count[input.rows][input.cols];
  for(int i=0; i<input.rows; i++) {
    for(int j=0; j<input.cols; j++) {
      if(input.at<uchar>(i,j) == 255) { 
        count[i][j] = i*input.cols+j+1; 
      } else { 
        count[i][j] = 0;
      }
    }
  }

  bool change = true;
  // while the count image is still being changed, keep changing
  while(change) {
    change = false;
    for(int i=0; i<input.rows; i++) {
      for(int j=0; j<input.cols; j++) {
        if(count[i][j] == 0) {continue;}
        int max = 0;
        // get the max of the current cell and it's 4 neighbours
        max = (count[i][j] > max)?count[i][j]:max;
        max = (i+1 < input.cols && count[i+1][j] > max)?count[i+1][j]:max;
        max = (i-1 > 0 && count[i-1][j] > max)?count[i-1][j]:max;
        max = (j+1 < input.cols && count[i][j+1] > max)?count[i][j+1]:max;
        max = (j-1 > 0 && count[i][j-1] > max)?count[i][j-1]:max;
        if( count[i][j] < max) {
          count[i][j] = max;
          change = true;
        }
        if( i+1 < input.rows && count[i+1][j] < max && count[i+1][j] > 0) {
          count[i+1][j] = max;
          change = true;
        }
        if( i-1 > 0 && count[i-1][j] < max && count[i-1][j] > 0) {
          count[i-1][j] = max;
          change = true;
        }
        if( j+1 < input.cols && count[i][j+1] < max && count[i][j+1] > 0) {
          count[i][j+1] = max;
          change = true;
        }
        if( j-1 > 0 && count[i][j-1] < max && count[i][j-1] > 0) {
          count[i][j-1] = max;
          change = true;
        }
      }
    }
  }

  map<int, wwp::region> region_size;
  for(int i=0; i<input.rows; i++) {
    for(int j=0; j<input.cols; j++) {
      if(region_size[count[i][j]].smallest_x > i) region_size[count[i][j]].smallest_x = i;
      if(region_size[count[i][j]].smallest_y > j) region_size[count[i][j]].smallest_y = j;
      if(region_size[count[i][j]].largest_x < i) region_size[count[i][j]].largest_x = i;
      if(region_size[count[i][j]].largest_y < j) region_size[count[i][j]].largest_y = j;
      region_size[count[i][j]].size+=1;
      region_size[count[i][j]].av_x+=i;
      region_size[count[i][j]].av_y+=j;
    }
  }

  map<int,wwp::region>::iterator it = region_size.begin();
  vector<wwp::region> found_regions;
  int i=0;
  ++it;
  for(; it!=region_size.end(); ++it,++i) {
    (*it).second.av_x /= (*it).second.size;
    (*it).second.av_y /= (*it).second.size;
    found_regions.push_back((*it).second);
  }

  return found_regions;
}

int wwp::estimate_black_line_thickness(Mat image) {
  double min_thickness, max_thickness, average_distance_to_zero, last_average_distance_to_zero, standard_deviation, sum_deviation;
  int count_zero, last_count_zero;
  Mat sharp_image, black_image, blur_image, distance_map, deviation;

  //Sharpen the image
  GaussianBlur(image, blur_image, Size(0,0), 1);
  addWeighted(image, 10.0, blur_image, -9.0, 0, sharp_image);

  //Get mask that describes where black lines are in sharpened image
  black_image = get_greyscale_in_image(sharp_image, 0, 10, 30);
  black_image = (black_image > 0);

  //Find the minimum distance between each pixel and the nearest empty pixel, and calculate the average
  distanceTransform(black_image, distance_map, CV_DIST_L1,3); //CV_DIST_L1 indicates distances are calculated with manhattan distance
  average_distance_to_zero = (double)(sum(distance_map)[0])/countNonZero(distance_map);
  
  // while the maximum thickness is outside the standard deviation of the average pixel
  // and the change in the number of pixels between iterations is not appreciable
  // remove pixels outside a standard deviation from the outside and recalculate the standard deviation
  do{
    // store the old non zero count, and the old average distance
    last_count_zero = countNonZero(distance_map);
    last_average_distance_to_zero = average_distance_to_zero;

    //Count the average distance to the nearest empty pixel, the maximum distance and the number of zero pixels in the image
    average_distance_to_zero = (double)(sum(distance_map)[0])/countNonZero(distance_map);
    minMaxIdx(distance_map, &min_thickness, &max_thickness, NULL, NULL, black_image);
    count_zero = distance_map.cols*distance_map.rows - countNonZero(distance_map);

    // calculate standard deviation => for all x_i with i<N; std_dev=sqrt( sum( (x_i-x_avg)^2 )/N )
    pow(distance_map-average_distance_to_zero,2, deviation);
    sum_deviation = sum(deviation)[0];                            // sum_deviation includes the zero pixels, which we don't want to count
    sum_deviation -= count_zero*pow(average_distance_to_zero,2);  // so we remove a (0-average_distance_to_zero)^2 for each zero value
    standard_deviation = sqrt(sum_deviation/countNonZero(distance_map));

    //remove all pixels that are outside the standard deviation and recalculate the distance to the nearest zero pixel
    distance_map = (distance_map <= average_distance_to_zero+standard_deviation);
    distanceTransform(distance_map, distance_map, CV_DIST_C,3);

  } while( (average_distance_to_zero+standard_deviation < max_thickness) && (countNonZero(distance_map) > 0.99*last_count_zero) );
  
  return last_average_distance_to_zero;
}

Mat wwp::get_greyscale_in_image(Mat image, int low_in, int high_in, int tolerance) {
  int high,low;
  high = (high_in>low_in) ? high_in : low_in;
  low  = (high_in>low_in) ? low_in : high_in;

  Mat rgb[3], red, green, blue, rg, rb, gb;
  split(image, rgb);
  // essentially calculates saturation for each colour
  blue = (rgb[0] >= low-tolerance) & (rgb[0] <= high+tolerance);
  green = (rgb[1] >= low-tolerance) & (rgb[1] <= high+tolerance);
  red = (rgb[2] >= low-tolerance) & (rgb[2] <= high+tolerance);

  // gets all colours with the correct saturation value
  Mat color_magnitude = blue & green & blue;

  // finds colours which are within tolerance of each other -> nearly greys
  rg = ( (rgb[1]-tolerance) <= rgb[2] ) & ( rgb[2] <= rgb[1]+tolerance );
  rb = ( (rgb[2]-tolerance) <= rgb[0] ) & ( rgb[0] <= rgb[2]+tolerance );
  gb = ( (rgb[0]-tolerance) <= rgb[1] ) & ( rgb[1] <= rgb[0]+tolerance );

  Mat similar_colours = rg & rb & gb;
  Mat out = color_magnitude & similar_colours;
  return out;
}

Mat wwp::get_colour_in_image(Mat image, string colour_one, std::string colour_two, float redgreen, float greenred, float redblue, float bluered, float greenblue, float bluegreen) {
  // these hold the ranges for allowed colours
  int r[2],g[2],b[2];

  // parse the rgb strings (e.g. '#FFB3AD') into the distinct colour sections
  string r_hex[2] = {colour_one.substr(1,2),colour_two.substr(1,2)};
  string g_hex[2] = {colour_one.substr(3,2),colour_two.substr(3,2)};
  string b_hex[2] = {colour_one.substr(5,2),colour_two.substr(5,2)};

  // needed to convert hex strings to ints
  stringstream to_hex;
  to_hex.flags(ios_base::hex);

  // convert hex strings of colour_one to integer values
  to_hex << r_hex[0];
  to_hex >> r[0];
  to_hex.clear();
  to_hex << g_hex[0];
  to_hex >> g[0];
  to_hex.clear();
  to_hex << b_hex[0];
  to_hex >> b[0];
  to_hex.clear();

  // repeat for second string
  to_hex << r_hex[1];
  to_hex >> r[1];
  to_hex.clear();
  to_hex << g_hex[1];
  to_hex >> g[1];
  to_hex.clear();
  to_hex << b_hex[1];
  to_hex >> b[1];
  to_hex.clear();

  Mat imgcpy;
  image.convertTo(imgcpy, CV_32S);
  Mat rgb[3], red, green, blue, rg, gr, gb, bg,  br, rb;
  split(imgcpy, rgb);

    red = ( rgb[2] >= r[0] ) & ( rgb[2] <= r[1] );
    green = ( rgb[1] >= g[0] ) & ( rgb[1] <= g[1] );
    blue = ( rgb[0] >= b[0] ) & ( rgb[0] <= b[1] );
  
  Mat saturation = red & green & blue;

  rg = rgb[2] >= redgreen*rgb[1];
  gr = rgb[1] >= greenred*rgb[2];
  rb = rgb[2] >= redblue*rgb[0];
  br = rgb[0] >= bluered*rgb[2];
  gb = rgb[1] >= greenblue*rgb[0];
  bg = rgb[0] >= bluegreen*rgb[1];

  Mat ratios = rg & gr & rb & br & gb & bg;

  Mat out = ratios & saturation;
  return out;
}