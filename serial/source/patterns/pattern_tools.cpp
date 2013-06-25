#include "whereswally/patterns/pattern_tools.h"

using namespace std;
using namespace cv;

wwp::region::region() {
  largest_x = 0;
  largest_y = 0;
  smallest_x = INT_MAX;
  smallest_y = INT_MAX;
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
      if(region_size[counter.at<int>(i,j)].smallest_x > j) region_size[counter.at<int>(i,j)].smallest_x = j;
      if(region_size[counter.at<int>(i,j)].smallest_y > i) region_size[counter.at<int>(i,j)].smallest_y = i;
      if(region_size[counter.at<int>(i,j)].largest_x < j) region_size[counter.at<int>(i,j)].largest_x = j;
      if(region_size[counter.at<int>(i,j)].largest_y < i) region_size[counter.at<int>(i,j)].largest_y = i;
      region_size[counter.at<int>(i,j)].size+=1;
      region_size[counter.at<int>(i,j)].av_x+=j;
      region_size[counter.at<int>(i,j)].av_y+=i;
    }
  }

  map<int,wwp::region>::iterator it;
  vector<wwp::region> found_regions;
  int i=0;
  for(it=region_size.begin(); it!=region_size.end(); ++it,++i) {
    (*it).second.av_x /= (*it).second.size;
    (*it).second.av_y /= (*it).second.size;
    found_regions.push_back((*it).second);
  }

  return found_regions;
}

