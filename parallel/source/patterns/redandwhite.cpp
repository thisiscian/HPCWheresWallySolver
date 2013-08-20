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
  ratio_to_red = 1.7;
  white_threshold = 200;
  next = NULL;
  next_certainty = 0;
  fast_search = true;
}

//-- initialise for chaining patterns together
Red_and_White::Red_and_White(Search_Pattern *next_pattern, float next_cert) {
  info.set_name("Red and White");
  info.set_description("Locate Wally by areas with red and white stripes");
  info.set_confidence(0.7);
  ratio_to_red = 1.7;
  white_threshold = 200;
  next = next_pattern;
  next_certainty = next_cert;
  fast_search = true;
}

//**
//** SEARCH TECHNIQUE
//**

vector<Pattern_Result> Red_and_White::start_search(Mat image) {
  print_output("starting", omp_get_thread_num(), omp_get_num_threads(), info.get_name());

  double thickness = estimate_black_line_thickness(image, 50,50);

  //-- create two arrays, one red and one white, indicating where the respective colour exists on the image
  print_output("calculating red and white masks", omp_get_thread_num(), omp_get_num_threads(), info.get_name());
  Mat red_mask = get_colour_in_image(image, "#770000", "#FF4646", ratio_to_red,0,ratio_to_red,0,0,0);
  Mat white_mask = get_greyscale_in_image(image, 105, 255, 50);

  //-- find the vertical and horizontal blurs in red and white masks
  print_output("blurring red and white masks", omp_get_thread_num(), omp_get_num_threads(), info.get_name());
  Mat hred(red_mask.rows,red_mask.cols,red_mask.type());
  Mat hwhite(white_mask.rows,white_mask.cols, white_mask.type());
  int apature = 10*thickness;
  if(apature % 2 == 0) {apature += 1;}
  GaussianBlur(red_mask, hred, Size(3,1), 0);
  GaussianBlur(white_mask, hwhite, Size(3,1), 0);
  GaussianBlur(red_mask, red_mask, Size(1,apature), 0);
  GaussianBlur(white_mask, white_mask, Size(1,apature), 0);

  //-- remove the boundaries of blurred matches
  print_output("shrinking blur boundaries", omp_get_thread_num(), omp_get_num_threads(), info.get_name());
  int tolerance = 5;
  white_mask = white_mask > tolerance;
  red_mask = red_mask > tolerance;
  hred = hred > 0.5*tolerance;
  hwhite = hwhite > 0.5*tolerance;

  //-- multiply red and white masks to find places with red and white stripes, blur to merge nearby regions
  print_output("merging blurred masks", omp_get_thread_num(), omp_get_num_threads(), info.get_name());
  Mat red_and_white_stripes, horizontal_matches;
  multiply(red_mask, white_mask, red_and_white_stripes);
  multiply(hred, hwhite, horizontal_matches);
  red_and_white_stripes -= horizontal_matches;

  //-- no longer need to store these masks, so free them
  print_output("releasing undeeded masks", omp_get_thread_num(), omp_get_num_threads(), info.get_name());
  red_mask.release();
  white_mask.release();

  GaussianBlur(red_and_white_stripes, red_and_white_stripes,Size(),5);
  red_and_white_stripes  = red_and_white_stripes > tolerance;
    //-- find and number the regions located
  print_output("finding regions in mask", omp_get_thread_num(), omp_get_num_threads(), info.get_name());
  vector<region> regions_list = fast_find_regions(red_and_white_stripes);

  //-- no longer in use, so free
  red_and_white_stripes.release();

  //-- if no regions were found, quit early
  vector<Pattern_Result> results;
  if(regions_list.size() < 1) {
    print_output("done with red and white", omp_get_thread_num(), omp_get_num_threads(), info.get_name());
    return results;
  }
  //-- count total size of all matching regions
  int sum = regions_list[1].size;
  for(size_t i=0; i<regions_list.size(); i++) {
    sum += regions_list[i].size;
  }

  //-- turn regions into results
  print_output("converting regions to results", omp_get_thread_num(), omp_get_num_threads(), info.get_name());
  float wally_jumper_ratio[2] = {1.5,3.5};
  for(size_t i=0; i<regions_list.size(); i++) {
    Pattern_Result tmp;
    tmp.info = info;
    tmp.wally_location[0] = regions_list[i].av_x; // horizontal center of wally is the center of his jumper
    tmp.wally_location[1] = regions_list[i].largest_y; // vertical center of wally is approximately at the end of his jumper
    tmp.scale[0] = wally_jumper_ratio[0]*(regions_list[i].largest_x -regions_list[i].smallest_x)/2;
    tmp.scale[1] = wally_jumper_ratio[1]*(regions_list[i].largest_y -regions_list[i].smallest_y)/2;
    tmp.certainty = (float) regions_list[i].size/sum;
    if(next != NULL) {
      int size_x = 4*tmp.scale[0];
      int size_y = 4*tmp.scale[1];
      int pos_x = tmp.wally_location[0];
      int pos_y = tmp.wally_location[1];
      if(size_x > image.cols) {
        size_x = image.cols;
        pos_x = image.cols/2;
      }
      if(size_y > image.rows) {
        size_y = image.rows;
        pos_y = image.rows/2;
      }

      Mat subimage;
      getRectSubPix(image, Size(size_x,size_y), Point2f(pos_x, pos_y), subimage);
      vector<Pattern_Result> next_results = next->start_search(subimage);
      subimage.release();
      if(next_results.size() == 0) {
        tmp.certainty = 0;
      }
    } 
    results.push_back(tmp);
  }
  image.release();
  print_output("done", omp_get_thread_num(), omp_get_num_threads(), info.get_name());
  return results;
}

