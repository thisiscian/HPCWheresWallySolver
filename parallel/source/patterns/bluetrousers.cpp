#include <whereswally/patterns.h>
#include "opencv2/highgui/highgui.hpp"
using namespace std;
using namespace cv;
using namespace wwp;

Blue_Trousers::Blue_Trousers() {
  info.set_name("Blue Trousers");
  info.set_description("Locate Wally by the blue of his trousers");
  info.set_confidence(0.7);
  next = NULL;
  next_certainty = 0;
}

Blue_Trousers::Blue_Trousers(Search_Pattern *next_pattern, float next_cert) {
  info.set_name("Blue Trousers");
  info.set_description("Locate Wally by the blue of his trousers");
  info.set_confidence(0.7);
  next = next_pattern;
  next_certainty = next_cert;
}

vector<Pattern_Result> Blue_Trousers::start_search(Mat image) {
  Mat subimage;
  getRectSubPix(image, Size(image.cols,image.rows/2), Point2f(image.cols/2, 3*image.rows/4), subimage);
  Mat blue_trousers = get_colour_in_image(subimage, "#000000", "#FFFFFF",0,1,0,1,0,0.8);
  //GaussianBlur(blue_trousers, blue_trousers, Size(3,3), 1);
  blue_trousers = blue_trousers > 0;
  subimage.release();

  // find and number the regions located
  vector<region> regions_list = fast_find_regions(blue_trousers);
  //vector<region> regions_list = find_regions_from_mask(blue_trousers);
  blue_trousers.release();

  // turn regions into results
  vector<Pattern_Result> results;
  if(regions_list.size() < 1) {
    return results;
  }
  int sum = regions_list[1].size;
  for(int i=1; i<regions_list.size(); i++) {
    sum += regions_list[i].size;
  }
  float tmp_max = 0;
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

      vector<Pattern_Result> next_results = next->start_search(image);
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
    }
    results.push_back(tmp);
  }
  return results;
}
