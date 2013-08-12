#include <whereswally/patterns.h>
#include <ctime>
#include <iostream>
#include <iomanip>
#include "opencv2/highgui/highgui.hpp"
using namespace std;
using namespace cv;
using namespace wwp;

Find_Glasses::Find_Glasses() {
  info.set_name("Find Glasses");
  info.set_description("Locate Wally by areas that look like glasses");
  info.set_confidence(0.1);
  rescale = 3;
  thresholds = 3;
  allowed_irregularity = 0.4;
}

// this is heavily inspired by the OpenCV sample squares.cpp
double Find_Glasses::calculate_angle(Point p0, Point p1, Point p2) {
  double line1[2] = {p1.x -p0.x,p1.y-p0.y};
  double line2[2] = {p2.x -p0.x,p2.y-p0.y};
  return (line1[0]*line2[0]+line1[1]*line2[1])/sqrt((line1[1]*line1[1]+line1[0]*line1[0])*(line2[1]*line2[1]+line2[0]*line2[0])+1e-10);
}

region Find_Glasses::extract_contour_info(vector<Point> contour) {
  region reg;
  reg.smallest_x = INT_MAX;
  reg.smallest_y = INT_MAX;
  reg.largest_x = 0;
  reg.largest_y = 0;
  reg.av_x = 0;
  reg.av_y = 0;
  reg.size = contourArea(contour)/rescale;
  for(int i=0; i<contour.size(); i++) {
    if(contour[i].x < reg.smallest_x) { reg.smallest_x = contour[i].x;}
    if(contour[i].y < reg.smallest_y) { reg.smallest_y = contour[i].y;}
    if(contour[i].x > reg.largest_x) { reg.largest_x = contour[i].x;}
    if(contour[i].y > reg.largest_y) { reg.largest_y = contour[i].y;}
    reg.av_x += contour[i].x;
    reg.av_y += contour[i].y;
  }
  reg.av_x /= (contour.size()*rescale);
  reg.av_y /= (contour.size()*rescale);
  reg.smallest_x /= rescale;
  reg.smallest_y /= rescale;
  reg.largest_x /= rescale;
  reg.largest_y /= rescale;
  return reg;
}

vector<Pattern_Result> Find_Glasses::start_search(Mat image) {
  double avg_thickness = estimate_black_line_thickness(image, 50, 50); 
  int max_thickness = floor(avg_thickness)+1;
  int min_thickness = floor(avg_thickness)-1;
  min_thickness = (min_thickness < 1)? 1 : min_thickness;

  Mat grey_image;
  cvtColor(image,grey_image,CV_RGB2GRAY);
  resize(grey_image, grey_image, Size(), rescale,rescale,INTER_CUBIC);

  // sharpen image; remove a weighted blur
  Mat blur_grey;
  GaussianBlur(grey_image, blur_grey, Size(0,0), 1);
  addWeighted(grey_image, 1.5, blur_grey, -0.7, 0, grey_image);

  vector< vector<Point> > contours;
  vector<region> circle_list, CircleList;
  Mat threshold_grey;

  vector<Point> approx; // this holds the polynomial approximation of a contour
  double irregularity; // this will contain information about how irregular each circle of the glasses is 
  
  Mat output = Mat::zeros(image.rows, image.cols, CV_8UC3); 
  // for a range of different thresholds ( x>threshold: x=255, x<= threshold: x=0 ), attempt to find contours
  for(int i=0; i<thresholds; i++) {
    //threshold allows for more lines to "connect" in an image, allowing hard to see lines to complete regions
    threshold(grey_image, threshold_grey, 50*(i+1)/thresholds+75, 255, THRESH_BINARY); // thresh over range of 125-225
    findContours(threshold_grey, contours,CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
    for(int j=0; j<contours.size(); j++) {
      approxPolyDP(Mat(contours[j]), approx, arcLength(Mat(contours[j]), true)*0.02, true);
      if(approx.size() >= 6 && isContourConvex(Mat(approx)) && contourArea(approx) >= 6*rescale*pow(min_thickness,2) && contourArea(approx) <= 1000*rescale*pow(max_thickness,2)) {
        irregularity = 0;
        for(int k=2; k<approx.size(); k++) {
          irregularity += fabs(fabs(cos(180.0-360.0/approx.size()))-fabs(calculate_angle(approx[j], approx[j-1], approx[j-2])));
        }
        if(irregularity <= 0.4*(approx.size()-2)) {
          circle_list.push_back(extract_contour_info(approx));
        }
      }
    }
    //clear vectors of data for next iteration
    approx.clear();
    contours.clear();
  }

  threshold_grey.release();
  grey_image.release();
  
  vector<Pattern_Result> results;
  int initial_size = circle_list.size();
  while(circle_list.size() > 0) {
    int width = (circle_list[0].largest_x-circle_list[0].smallest_x);
    int height = (circle_list[0].largest_y-circle_list[0].smallest_y);
    double aspect = min((double)(width)/height, (double)(height)/width);
    for(int i=1; i< circle_list.size(); i++) {
      int myWidth = (circle_list[i].largest_x-circle_list[i].smallest_x);
      int myHeight = (circle_list[i].largest_y-circle_list[i].smallest_y);
      int dist_x = abs(circle_list[0].av_x-circle_list[i].av_x); 
      int dist_y = abs(circle_list[0].av_y-circle_list[i].av_y); 
      bool nearby_x = (  dist_x < 2*width && dist_x < 2*myWidth && dist_x > 0.4*(myWidth+width));
      bool nearby_y = ( dist_y < 0.1*height && dist_y < 0.1*myHeight);
      if( !nearby_x && !nearby_y ) {
        continue;
      }
      bool sameWidth = ( 1- min((double)(width)/myWidth, (double)(myWidth)/width) < 0.4 );
      bool sameHeight = ( 1- min((double)(height)/myHeight, (double)(myHeight)/height) < 0.4 );
      if( sameWidth && sameHeight) {
        Pattern_Result tmp;
        tmp.info = info;
        tmp.wally_location[0] = (circle_list[0].av_x+circle_list[i].av_x)/2;
        tmp.wally_location[1] = (circle_list[0].av_y+circle_list[i].av_y)/2;//+13*(circle_list[0].largest_y-circle_list[0].smallest_y);
        tmp.scale[0] = (max(circle_list[0].largest_x, circle_list[i].largest_x) - min(circle_list[0].smallest_x, circle_list[i].smallest_x))/2+1;
        tmp.scale[1] = (max(circle_list[0].largest_y, circle_list[i].largest_y) - min(circle_list[0].smallest_y, circle_list[i].smallest_y))/2+1;
        double minSize = min((double)(circle_list[0].size)/circle_list[i].size, (double)(circle_list[i].size)/circle_list[0].size);
        double myAspect = min((double)(myHeight)/myWidth, (double)(myWidth)/myHeight);
        double minAspect =  min(aspect/myAspect ,myAspect/aspect ); //min(tmp.scale[0]/tmp.scale[1], tmp.scale[1]/tmp.scale[0]);

        tmp.certainty = minSize * minAspect * (1-fabs(2-tmp.scale[0]/tmp.scale[1])/(tmp.scale[0]/tmp.scale[1]));
        if(tmp.certainty <= 0) {
          continue;
        }
        //tmp.scale[0]*=5;
        //tmp.scale[1]*=34;
        results.push_back(tmp);
      }
    }
    circle_list.erase(circle_list.begin());
  }
  return results;
}
