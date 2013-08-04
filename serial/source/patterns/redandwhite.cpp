#include <whereswally/patterns.h>
#include "opencv2/highgui/highgui.hpp"
using namespace std;
using namespace cv;
using namespace wwp;

Red_and_White::Red_and_White() {
  info.set_name("Red and White");
  info.set_description("Locate Wally by areas with red and white stripes");
  info.set_confidence(0.7);
  ratio_to_red = 2.5;
  white_threshold = 200;
}

vector<Pattern_Result> Red_and_White::start_search(Mat image) {
  Mat red_and_white_stripes;

  // get red and white in image
  Mat red_mask = get_colour_in_image(image, "#770000", "#FF4646", 1.7,0,1.7,0,0,0);
  Mat white_mask = get_colour_in_image(image, "#999999", "#FFFFFF", 1.1,0.7,1.1,0.7,0.9,0.9);
//  Mat white_mask = get_greyscale_in_image(image, 200, 255, 100);
  // blur image to get an overlap, could also use shift the masks in the 4 cardinal directions
  Size ksize(0,11);// by varying this, control over vertical and horizontal stripes can be found
  GaussianBlur(red_mask, red_mask,ksize, 3);
  GaussianBlur(white_mask, white_mask,ksize, 3);
  threshold(white_mask, white_mask, 30, 255, THRESH_BINARY);
  threshold(red_mask, red_mask, 30, 255, THRESH_BINARY);
  
  // multiply red and white masks to find places with red and white stripes, blur to merge nearby regions
  multiply(red_mask, white_mask, red_and_white_stripes, 1.0/255.0);
  ksize = Size(5,5);
  GaussianBlur(red_and_white_stripes, red_and_white_stripes,ksize, 3);
  red_and_white_stripes  = red_and_white_stripes > 0;

  // find and number the regions located
  vector<region> regions_list = find_regions_from_mask(red_and_white_stripes);
  
  // turn regions into results
  vector<Pattern_Result> results;
  if(regions_list.size() < 1) {
    return results;
  }
  int sum = regions_list[1].size;
  for(int i=1; i<regions_list.size(); i++) {
    sum += regions_list[i].size;
  }
  for(int i=0; i<regions_list.size(); i++) {
    Pattern_Result tmp;
    tmp.info = info;
    tmp.wally_location[1] = regions_list[i].largest_x;
    tmp.wally_location[0] = regions_list[i].av_y;
    tmp.scale[1] = 3.5*(regions_list[i].largest_x -regions_list[i].smallest_x)/2;
    tmp.scale[0] = 2*(regions_list[i].largest_y -regions_list[i].smallest_y)/2;
    tmp.certainty = (float) regions_list[i].size/sum;

    results.push_back(tmp);
  }
  return results;
}

