#include "whereswally/patterns/pattern_tools.h"

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

// Finds the distinct regions from a mask input.
// This is done by assigning each pixel a unique number
// and then each pixel takes the value of it's largest neighbour.
// Pixels with a value of zero remain at zero
vector<wwp::region> wwp::find_regions_from_mask(Mat input) {
  Mat counter(input.size(), CV_32SC1); // CV_32SC1 indicates type int with 1 channel, which is needed for the counting mask
  for(int i=0; i<counter.rows; i++) {
    for(int j=0; j<counter.cols; j++) {
      if(input.at<uchar>(i,j) == 255) { 
        counter.at<int>(i,j) = i*counter.cols+j+1; 
      } else { 
        counter.at<int>(i,j) = 0;
      }
    }
  }

  bool change = true;
  // while the counter image is still being changed, keep changing
  while(change) {
    change = false;
    for(int i=0; i<counter.rows; i++) {
      for(int j=0; j<counter.cols; j++) {
        if(counter.at<int>(i,j) == 0) {continue;}
        int max = 0;
        // get the max of the current cell and it's 4 neighbours
        max = (counter.at<int>(i,j) > max)?counter.at<int>(i,j):max;
        max = (i+1 < counter.cols && counter.at<int>(i+1,j) > max)?counter.at<int>(i+1,j):max;
        max = (i-1 > 0 && counter.at<int>(i-1,j) > max)?counter.at<int>(i-1,j):max;
        max = (j+1 < counter.cols && counter.at<int>(i,j+1) > max)?counter.at<int>(i,j+1):max;
        max = (j-1 > 0 && counter.at<int>(i,j-1) > max)?counter.at<int>(i,j-1):max;
        if( counter.at<int>(i,j) < max) {
          counter.at<int>(i,j) = max;
          change = true;
        }
        if( i+1 < counter.rows && counter.at<int>(i+1,j) < max && counter.at<int>(i+1,j) > 0) {
          counter.at<int>(i+1,j) = max;
          change = true;
        }
        if( i-1 > 0 && counter.at<int>(i-1,j) < max && counter.at<int>(i-1,j) > 0) {
          counter.at<int>(i-1,j) = max;
          change = true;
        }
        if( j+1 < counter.cols && counter.at<int>(i,j+1) < max && counter.at<int>(i,j+1) > 0) {
          counter.at<int>(i,j+1) = max;
          change = true;
        }
        if( j-1 > 0 && counter.at<int>(i,j-1) < max && counter.at<int>(i,j-1) > 0) {
          counter.at<int>(i,j-1) = max;
          change = true;
        }
      }
    }
  }

  map<int, wwp::region> region_size;
  for(int i=0; i<counter.rows; i++) {
    for(int j=0; j<counter.cols; j++) {
      if(region_size[counter.at<int>(i,j)].smallest_x > i) region_size[counter.at<int>(i,j)].smallest_x = i;
      if(region_size[counter.at<int>(i,j)].smallest_y > j) region_size[counter.at<int>(i,j)].smallest_y = j;
      if(region_size[counter.at<int>(i,j)].largest_x < i) region_size[counter.at<int>(i,j)].largest_x = i;
      if(region_size[counter.at<int>(i,j)].largest_y < j) region_size[counter.at<int>(i,j)].largest_y = j;
      region_size[counter.at<int>(i,j)].size+=1;
      region_size[counter.at<int>(i,j)].av_x+=i;
      region_size[counter.at<int>(i,j)].av_y+=j;
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
