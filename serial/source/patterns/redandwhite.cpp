#include <whereswally/patterns.h>
#include "whereswally/patterns/redandwhite.h"

using namespace std;
using namespace cv;
using namespace wwp;

Red_and_White::Red_and_White() {
  info.set_name("Red and White");
  info.set_description("Locate Wally by areas red and white stripes");
  info.set_confidence(0.7);
  ratio_to_red = 2.5;
  white_threshold = 200;
}

vector<Pattern_Result> Red_and_White::start_search(Mat image) {
  Mat image_rgb[3];
  Mat red_and_white_stripes;
  split(image, image_rgb);

//  // split images are stored in bgr format , hence red->[2]
//  Mat more_red_than_blue = image_rgb[2] > ratio_to_red*image_rgb[0];
//  Mat more_red_than_green = image_rgb[2] > ratio_to_red*image_rgb[1];
//  Mat red_mask = more_red_than_blue & more_red_than_green; // masks areas with pure red areas

  float tolerance[3] = {2.5,0,0.4};
  int invert[3] = {255, 255, 0};
  Mat red_mask = get_colour_in_image(image, "#FFFFFF", "#000000", tolerance, invert);
  Mat white_mask = get_greyscale_in_image(image, 200, 255, 25);

  // blur image to get an overlap, could also use shift the masks in the 4 cardinal directions
  Size ksize(1,7);// by varying this, control over vertical and horizontal stripes can be found
  GaussianBlur(red_mask, red_mask,ksize, 3);
  GaussianBlur(white_mask, white_mask,ksize, 3);
  
  // multiply red and white masks to find places with red and white stripes
  multiply(red_mask, white_mask, red_and_white_stripes, 1.0/255.0);
  ksize = Size(5,5);
  GaussianBlur(red_and_white_stripes, red_and_white_stripes,ksize, 3);
  red_and_white_stripes  = red_and_white_stripes > 0;

  // find and number the regions located
  vector<region> regions_list = find_regions_from_mask(red_and_white_stripes);
  
  // turn regions into results
  vector<Pattern_Result> results;
  if(regions_list.size() < 2) {
    return results;
  }
  int sum = regions_list[1].size;
  for(int i=2; i<regions_list.size(); i++) {
    sum += regions_list[i].size;
  }
  for(int i=1; i<regions_list.size(); i++) {
    Pattern_Result tmp;
    tmp.info = info;
    tmp.wally_location[0] = regions_list[i].av_x;
    tmp.wally_location[1] = regions_list[i].av_y;
    tmp.scale[0] = (regions_list[i].largest_x -regions_list[i].smallest_x)/2;
    tmp.scale[1] = (regions_list[i].largest_y -regions_list[i].smallest_y)/2;
    tmp.certainty = (float) regions_list[i].size/sum;

    results.push_back(tmp);
  }
}

