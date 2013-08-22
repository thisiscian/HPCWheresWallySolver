#include <whereswally/patterns.h>
#include "opencv2/highgui/highgui.hpp"
#include <omp.h>
using namespace std;
using namespace cv;
using namespace wwp;

//**
//** CLASS INITIALISATION
//**

Red_and_White::Red_and_White() {
  info.set_name("Red and White");
  info.set_description("Locate Wally by areas with red and white stripes");
  info.set_confidence(0.7);
}

//**
//** SEARCH TECHNIQUE
//**

vector<Pattern_Result> Red_and_White::start_search(Mat image) {
  double thickness = estimate_black_line_thickness(image, 50,50);
  if(thickness < 1 || thickness != thickness) thickness = 1;

  //-- create two arrays, one red and one white, indicating where the respective colour exists on the image
  Mat red_mask = get_colour_in_image(image, "#770000", "#FF4646", 1.7,0,1.7,0,0,0);
  Mat white_mask = get_greyscale_in_image(image, 105, 255, 50);

  //-- find the vertical and horizontal blurs in red and white masks
  Mat hred(red_mask.rows,red_mask.cols,red_mask.type());
  Mat hwhite(white_mask.rows,white_mask.cols, white_mask.type());
  int aperture = 5*floor(thickness+0.5);
  cout << "apt=" << aperture << endl;
  if(aperture % 2 == 0) {aperture += 1;}
  GaussianBlur(red_mask, hred, Size(3,1), 0);
  GaussianBlur(white_mask, hwhite, Size(3,1), 0);
  GaussianBlur(red_mask, red_mask, Size(1,aperture), 0);
  GaussianBlur(white_mask, white_mask, Size(1,aperture), 0);

  //-- remove the boundaries of blurred matches
  int tolerance = 5;
  white_mask = white_mask > tolerance;
  red_mask = red_mask > tolerance;
  hred = hred > 0.5*tolerance;
  hwhite = hwhite > 0.5*tolerance;

  //-- multiply red and white masks to find places with red and white stripes, blur to merge nearby regions
  Mat red_and_white_stripes, horizontal_matches;
  multiply(red_mask, white_mask, red_and_white_stripes);
  multiply(hred, hwhite, horizontal_matches);
  red_and_white_stripes -= horizontal_matches;

  //-- no longer need to store these masks, so free them
  red_mask.release();
  white_mask.release();

  GaussianBlur(red_and_white_stripes, red_and_white_stripes,Size(),5);
  red_and_white_stripes  = red_and_white_stripes > tolerance;
  //-- find and number the regions located

  vector<region> regions_list = fast_find_regions(red_and_white_stripes);
  cout << "regions_list:" << regions_list.size() << endl;
  //-- no longer in use, so free
  red_and_white_stripes.release();

  //-- if no regions were found, quit early
  vector<Pattern_Result> results;
  if(regions_list.size() < 1) {
    return results;
  }
  //-- count total size of all matching regions
  int sum = 0;
  for(size_t i=0; i<regions_list.size(); i++) {
    sum += regions_list[i].size;
  }

  //-- turn regions into results
  float wally_jumper_ratio[2] = {1.5,3.5};
  for(size_t i=0; i<regions_list.size(); i++) {
    Pattern_Result tmp;
    tmp.info = info;
    tmp.wally_location[0] = regions_list[i].av_x; // horizontal center of wally is the center of his jumper
    tmp.wally_location[1] = regions_list[i].largest_y; // vertical center of wally is approximately at the end of his jumper
    tmp.scale[0] = wally_jumper_ratio[0]*(regions_list[i].largest_x -regions_list[i].smallest_x)/2;
    tmp.scale[1] = wally_jumper_ratio[1]*(regions_list[i].largest_y -regions_list[i].smallest_y)/2;
    tmp.certainty = (float)regions_list[i].size/sum;
    results.push_back(tmp);
  }
  image.release();
  return results;
}

