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
  ratio_to_red = 2.5;
  white_threshold = 200;
  next = NULL;
  next_certainty = 0;
}

Red_and_White::Red_and_White(Search_Pattern *next_pattern, float next_cert) {
  info.set_name("Red and White");
  info.set_description("Locate Wally by areas with red and white stripes");
  info.set_confidence(0.7);
  ratio_to_red = 2.5;
  white_threshold = 200;
  next = next_pattern;
  next_certainty = next_cert;
}

vector<Pattern_Result> Red_and_White::start_search(Mat image) {
  stringstream output_stream;

  print_output("starting", omp_get_thread_num(), omp_get_num_threads(), info.get_name());

  Mat red_and_white_stripes;

  print_output("calculating red and white masks", omp_get_thread_num(), omp_get_num_threads(), info.get_name());
  // get red and white in image
  Mat red_mask = get_colour_in_image(image, "#770000", "#FF4646", 1.7,0,1.7,0,0,0);
  Mat white_mask = get_colour_in_image(image, "#999999", "#FFFFFF", 1.1,0.7,1.1,0.7,0.9,0.9);
  //  Mat white_mask = get_greyscale_in_image(image, 200, 255, 100);
  // blur image to get an overlap, could also use shift the masks in the 4 cardinal directions
  Size ksize(0,3);// by varying this, control over vertical and horizontal stripes can be found
  GaussianBlur(red_mask, red_mask,ksize, 1);
  GaussianBlur(white_mask, white_mask,ksize, 1);
  threshold(white_mask, white_mask, 30, 255, THRESH_BINARY);
  threshold(red_mask, red_mask, 30, 255, THRESH_BINARY);
  
  print_output("merging_masks", omp_get_thread_num(), omp_get_num_threads(), info.get_name());
  // multiply red and white masks to find places with red and white stripes, blur to merge nearby regions
  multiply(red_mask, white_mask, red_and_white_stripes);
  GaussianBlur(red_and_white_stripes, red_and_white_stripes,Size(0,51),1);
  red_and_white_stripes  = red_and_white_stripes > 0;
  GaussianBlur(red_and_white_stripes, red_and_white_stripes,Size(0,51),1);
  red_and_white_stripes  = red_and_white_stripes > 0;

  print_output("finding regions in masks", omp_get_thread_num(), omp_get_num_threads(), info.get_name());
  // find and number the regions located
  vector<region> regions_list = find_regions_from_mask(red_and_white_stripes);
  
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
    tmp.wally_location[1] = regions_list[i].largest_x;
    tmp.wally_location[0] = regions_list[i].av_y;
    tmp.scale[1] = 3.5*(regions_list[i].largest_x -regions_list[i].smallest_x)/2;
    tmp.scale[0] = 2*(regions_list[i].largest_y -regions_list[i].smallest_y)/2;
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
      float max_certainty = 0;
      float next_confidence = 1;
      if(next_results.size() > 0) {
        next_confidence = next_results[0].info.get_confidence();
        for(int j=0; j<next_results.size(); j++) {
          if(max_certainty < next_results[j].certainty) {
            max_certainty = next_results[j].certainty;
          }
        }
      } else {
        tmp.certainty = 0;
      }
//      if(tmp.certainty > tmp_max) {tmp_max = tmp.certainty; cout << tmp.certainty << " " << max_certainty << " " << next_confidence << " " << info.get_confidence() << endl;}
//      tmp.certainty += (1-tmp.certainty)*max_certainty;
//      tmp.certainty = (tmp.certainty*info.get_confidence() +max_certainty*next_confidence)/2; //(next_confidence*info.get_confidence());
    } 
    results.push_back(tmp);
  }
  print_output("done", omp_get_thread_num(), omp_get_num_threads(), info.get_name());
  return results;
}

