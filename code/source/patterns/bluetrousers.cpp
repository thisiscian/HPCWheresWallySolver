#include <whereswally/patterns.h>
#include "opencv2/highgui/highgui.hpp"
using namespace std;
using namespace cv;
using namespace wwp;

//**
//**  CLASS INITIALISATION
//**

Blue_Trousers::Blue_Trousers() {
  info.set_name("Blue Trousers");
  info.set_description("Locate Wally by the blue of his trousers");
  info.set_confidence(0.7);
  next = NULL;
  next_certainty = 0;
}

//**
//**  SEARCH PATTERN
//**

vector<Pattern_Result> Blue_Trousers::start_search(Mat image) {
  //-- Wally's trousers are light blue
  Mat blue_trousers = get_colour_in_image(image, "#000000", "#AAAAFF",0,0,0,0.1,0,1.1);
  double thickness = estimate_black_line_thickness(image, 50,50);
  if(thickness < 1 || thickness != thickness) thickness = 1;
  int estimated_size = 600*thickness*thickness;

  int aperture = 8*thickness;
  if(aperture % 2 == 0) {aperture += 1;}
 
  //-- remove small elements and then blur nearby elements together
  GaussianBlur(blue_trousers, blue_trousers, Size(aperture,aperture), 0);
  blue_trousers = blue_trousers > 180;
  GaussianBlur(blue_trousers, blue_trousers, Size(2*aperture+1,2*aperture+1), 0);
  blue_trousers = blue_trousers > 0;
  GaussianBlur(blue_trousers, blue_trousers, Size(2*aperture+1,2*aperture+1), 0);
  blue_trousers = blue_trousers > 0;

  //-- find and number the regions located
  vector<region> regions_list = fast_find_regions(blue_trousers);

  blue_trousers.release();

  // turn regions into results
  vector<Pattern_Result> results;
  if(regions_list.size() < 1) {
    return results;
  }

  int sum = 0;
  for(size_t i=0; i<regions_list.size(); i++) {
    sum += regions_list[i].size;
  }
  for(size_t i=0; i<regions_list.size(); i++) {
    Pattern_Result tmp;
    tmp.info = info;
    tmp.wally_location[0] = regions_list[i].av_x;
    tmp.wally_location[1] = regions_list[i].smallest_y;
    tmp.scale[0] = (7.0/4)*(regions_list[i].largest_x -regions_list[i].smallest_x);
    tmp.scale[1] = (regions_list[i].largest_y -regions_list[i].smallest_y);
    tmp.certainty = exp(-pow(tmp.scale[0]*tmp.scale[1]-estimated_size,2)*5e-8);
    if((float)tmp.scale[1]/tmp.scale[0] > 2 && (float) tmp.scale[1]/tmp.scale[0] < 5) {
      results.push_back(tmp);   
    }
  }
  image.release();
  return results;
}
