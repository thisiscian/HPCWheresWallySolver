#include <whereswally/patterns.h>
#include "opencv2/highgui/highgui.hpp"
#include <omp.h>
using namespace std;
using namespace cv;
using namespace wwp;

Red_and_White::Red_and_White() {
  info.set_name("Red and White");
  info.set_description("Locate Wally by areas with red and white stripes");
  info.set_confidence(0.7);
  ratio_to_red = 1.7;
  white_threshold = 200;
  next = NULL;
  next_certainty = 0;
}

Red_and_White::Red_and_White(Search_Pattern *next_pattern, float next_cert) {
  info.set_name("Red and White");
  info.set_description("Locate Wally by areas with red and white stripes");
  info.set_confidence(0.7);
  ratio_to_red = 1.7;
  white_threshold = 200;
  next = next_pattern;
  next_certainty = next_cert;
}

vector<Pattern_Result> Red_and_White::start_search(Mat image) {
  print_output("starting", omp_get_thread_num(), omp_get_num_threads(), info.get_name());

  Mat red_mask, white_mask, red_and_white_stripes;

  // create two arrays, one red and one white, indicating where the respective colour exists on the image
    print_output("calculating red and white masks", omp_get_thread_num(), omp_get_num_threads(), info.get_name());
  red_mask = get_colour_in_image(image, "#770000", "#FF4646", ratio_to_red,0,ratio_to_red,0,0,0);
  white_mask = get_greyscale_in_image(image, 105, 255, 50);

  // blur image to get an overlap, could also use shift the masks in the 4 cardinal directions
    print_output("blurring red and white masks", omp_get_thread_num(), omp_get_num_threads(), info.get_name());
  Size kvertsize(1,21);// by varying this, control over vertical and horizontal stripes can be found
  Size khozsize(21,1);// for horizontal control

  Mat hred,hwhite,hrw;
  GaussianBlur(red_mask, hred, khozsize, 0);
  GaussianBlur(white_mask, hwhite, khozsize, 0);

  GaussianBlur(red_mask, red_mask, kvertsize, 0);
  GaussianBlur(white_mask, white_mask, kvertsize, 0);
  white_mask = white_mask > 30;
  red_mask = red_mask > 30;
  hred = hred > 30;
  hwhite = hwhite > 30;
  
  // multiply red and white masks to find places with red and white stripes, blur to merge nearby regions
    print_output("merging blurred masks", omp_get_thread_num(), omp_get_num_threads(), info.get_name());
  multiply(red_mask, white_mask, red_and_white_stripes);
  multiply(hred, hwhite, hrw);
  red_and_white_stripes -= hrw;
  red_mask.release();
  white_mask.release();
  GaussianBlur(red_and_white_stripes, red_and_white_stripes,Size(),3);
  red_and_white_stripes  = red_and_white_stripes > 0;
  // find and number the regions located
    print_output("finding regions in mask", omp_get_thread_num(), omp_get_num_threads(), info.get_name());
  //vector<region> regions_list = find_regions_from_mask(red_and_white_stripes);
  vector<region> regions_list = fast_find_regions(red_and_white_stripes);
  red_and_white_stripes.release();

  // turn regions into results
  vector<Pattern_Result> results;
  if(regions_list.size() < 1) {
    print_output("done", omp_get_thread_num(), omp_get_num_threads(), info.get_name());
    return results;
  }
  int sum = regions_list[1].size;
  for(int i=1; i<regions_list.size(); i++) {
    sum += regions_list[i].size;
  }
  float tmp_max = 0;
  print_output("converting regions to results", omp_get_thread_num(), omp_get_num_threads(), info.get_name());
  for(int i=0; i<regions_list.size(); i++) {
    Pattern_Result tmp;
    tmp.info = info;
    tmp.wally_location[0] = regions_list[i].av_x;
    tmp.wally_location[1] = regions_list[i].largest_y;
    tmp.scale[0] = 1.5*(regions_list[i].largest_x -regions_list[i].smallest_x)/2;
    tmp.scale[1] = 1.8*(regions_list[i].largest_y -regions_list[i].smallest_y)/2;
    tmp.certainty = (float) regions_list[i].size/sum;
    if(next != NULL) {
      Mat subimage;
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

