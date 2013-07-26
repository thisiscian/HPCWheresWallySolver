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
  info.set_confidence(0.4);
  scale = 2;
  thresholds = 3;
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
  reg.size = contourArea(contour)/scale;
  for(int i=0; i<contour.size(); i++) {
    if(contour[i].x < reg.smallest_x) { reg.smallest_x = contour[i].x;}
    if(contour[i].y < reg.smallest_y) { reg.smallest_y = contour[i].y;}
    if(contour[i].x > reg.largest_x) { reg.largest_x = contour[i].x;}
    if(contour[i].y > reg.largest_y) { reg.largest_y = contour[i].y;}
    reg.av_x += contour[i].x;
    reg.av_y += contour[i].y;
  }
  reg.av_x /= (contour.size()*scale);
  reg.av_y /= (contour.size()*scale);
  reg.smallest_x /= scale;
  reg.smallest_y /= scale;
  reg.largest_x /= scale;
  reg.largest_y /= scale;
  return reg;
}

vector<Pattern_Result> Find_Glasses::start_search(Mat image) {
  // glasses are black -> greyify image

  Mat black_image, blur_image, distance_map, rgb[3];
  GaussianBlur(image, blur_image, Size(0,0), 1);
  addWeighted(image, 10.0, blur_image, -9.0, 0, black_image);

  black_image = get_greyscale_in_image(black_image, 0, 10, 30);
  black_image = (black_image > 0);
  
  distanceTransform(black_image, distance_map, CV_DIST_C,3);
  double averageDist = (double)(sum(distance_map)[0])/countNonZero(distance_map);
  double mini, maxi;
  minMaxIdx(distance_map, &mini, &maxi, NULL, NULL, black_image);
//  Mat secondPass = distance_map < (averageDist+maxi)/2 & distance_map > 0;
//  distanceTransform(secondPass, secondPass, CV_DIST_C,3);
  int countZero = distance_map.cols*distance_map.rows - countNonZero(distance_map);
  Mat near_dev;
  pow(distance_map-averageDist,2, near_dev);
  double standard_deviation = sqrt((sum(near_dev)[0]-countZero*pow(averageDist,2))/countNonZero(distance_map));
 
  int lastNonZero = countNonZero(distance_map);
  while(standard_deviation < maxi-averageDist && (1-double(countNonZero(distance_map))/lastNonZero < 0.01)) {
    lastNonZero = countNonZero(distance_map);
    distance_map = distance_map < maxi & distance_map > 0;
    distanceTransform(distance_map, distance_map, CV_DIST_C,3);

    countZero = distance_map.cols*distance_map.rows - countNonZero(distance_map);
    pow(distance_map-averageDist,2, near_dev);
    standard_deviation = sqrt((sum(near_dev)[0]-countZero*pow(averageDist,2))/countNonZero(distance_map));
    minMaxIdx(distance_map, &mini, &maxi, NULL, NULL, black_image);
    averageDist = (double)(sum(distance_map)[0])/countNonZero(distance_map);
 }
  
/*
  while(maxi > averageDist) {
    distance_map = secondPass;
    secondPass = secondPass < (averageDist+maxi)/2 & secondPass > 0;
    distanceTransform(secondPass, secondPass, CV_DIST_C,3);
    averageDist = (double)(sum(secondPass)[0])/countNonZero(secondPass);
    minMaxIdx(secondPass, &mini, &maxi, NULL, NULL, black_image);
    cerr << averageDist << " " << maxi << ": " << maxi*maxi/averageDist << " " << maxi*(maxi+1)/averageDist<< endl;
  }
  averageDist = (double)(sum(distance_map)[0])/countNonZero(distance_map);
  minMaxIdx(distance_map, &mini, &maxi, NULL, NULL, black_image);
  cerr << averageDist << " " << maxi << ": " << maxi*maxi/averageDist << " " << maxi*(maxi+1)/averageDist<< endl;
*/
  
  int short_length = (maxi*maxi/averageDist);
  int long_length = (maxi*(maxi+1)/averageDist)+1;

  split(image, rgb);
  rgb[0] = rgb[0].mul(black_image/255);
  rgb[1] = rgb[1].mul(black_image/255);
  rgb[2] = rgb[2].mul(black_image/255);
  merge(rgb, 3, black_image);

  Mat grey_image;
  cvtColor(image,grey_image,CV_RGB2GRAY);
  resize(grey_image, grey_image, Size(), scale,scale,INTER_CUBIC);

  // sharpen image; remove a weighted blur
  Mat blur_grey;
  GaussianBlur(grey_image, blur_grey, Size(0,0), 1);
  addWeighted(grey_image, 1.5, blur_grey, -0.7, 0, grey_image);


  vector< vector<Point> > contours, circles;
  vector<region> circle_list, CircleList;
  Mat threshold_grey;
  // for a range of different thresholds ( x>threshold: x=255, x<= threshold: x=0 ), attempt to find contours
  cerr << "completed " <<  0 << "/" << thresholds << " threshold scans" << endl;   
  for(int i=0; i<thresholds; i++) {
    threshold(grey_image, threshold_grey, 50*(i+1)/thresholds+75, 255, THRESH_BINARY); // thresh over range of 125-225
    findContours(threshold_grey, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
    vector<Point> approx; // polynomial approximation of a curve
    for(int j=0; j<contours.size(); j++) {
      approxPolyDP(Mat(contours[j]), approx, arcLength(Mat(contours[j]), true)*0.02, true);
      if(approx.size() >= 6 && isContourConvex(Mat(approx)) && contourArea(approx) >= 6*scale*pow(short_length,2) && contourArea(approx) <= 100*scale*pow(long_length,2)) {
        double irregularity = 0;
        for(int k=2; k<approx.size(); k++) {
          irregularity += fabs(fabs(cos(180.0-360.0/approx.size()))-fabs(calculate_angle(approx[j], approx[j-1], approx[j-2])));
        }
        if(irregularity <= 0.15*(approx.size()-2)) {
          circle_list.push_back(extract_contour_info(approx));
          circles.push_back(approx);
        }
      }
    }
    cerr << "\033[A\033[2K" << "completed " << i+1 << "/" << thresholds << " threshold scans" << endl;   
  }
 
  vector<Pattern_Result> results;
  int initial_size = circle_list.size();
  double estimation_time = 0;
  cerr << "completed " << 0 << "/" << initial_size << " scans for glasses, estimated completion time is ???" << endl;
  time_t prev_time = time(NULL), current_time;
  while(circle_list.size() > 0) {
    int width = (circle_list[0].largest_x-circle_list[0].smallest_x);
    int height = (circle_list[0].largest_y-circle_list[0].smallest_y);
    double aspect = min((double)(width)/height, (double)(height)/width);
    for(int i=1; i< circle_list.size(); i++) {
      int myWidth = (circle_list[i].largest_x-circle_list[i].smallest_x);
      int myHeight = (circle_list[i].largest_y-circle_list[i].smallest_y);
      int dist_x = fabs(circle_list[0].av_x-circle_list[i].av_x); 
      int dist_y = fabs(circle_list[0].av_y-circle_list[i].av_y); 
      bool nearby_x = (  dist_x < 4*width && dist_x < 4*myWidth && dist_x > 0.4*(myWidth+width));
      bool nearby_y = ( dist_y < 0.5*height && dist_y < 0.5*myHeight);
      if( !nearby_x && !nearby_y ) {
        continue;
      }
      bool sameWidth = ( 1- min((double)(width)/myWidth, (double)(myWidth)/width) < 0.4 );
      bool sameHeight = ( 1- min((double)(height)/myHeight, (double)(myHeight)/height) < 0.4 );
      if( sameWidth && sameHeight) {
        Pattern_Result tmp;
        tmp.info = info;
        tmp.wally_location[0] = (circle_list[0].av_x+circle_list[i].av_x)/2;
        tmp.wally_location[1] = (circle_list[0].av_y+circle_list[i].av_y)/2+13*(circle_list[0].largest_y-circle_list[0].smallest_y);
        tmp.scale[0] = (max(circle_list[0].largest_x, circle_list[i].largest_x) - min(circle_list[0].smallest_x, circle_list[i].smallest_x))/2+1;
        tmp.scale[1] = (max(circle_list[0].largest_y, circle_list[i].largest_y) - min(circle_list[0].smallest_y, circle_list[i].smallest_y))/2+1;
        double minSize = min((double)(circle_list[0].size)/circle_list[i].size, (double)(circle_list[i].size)/circle_list[0].size);
        double myAspect = min((double)(myHeight)/myWidth, (double)(myWidth)/myHeight);
        double minAspect =  min(aspect/myAspect ,myAspect/aspect ); //min(tmp.scale[0]/tmp.scale[1], tmp.scale[1]/tmp.scale[0]);

        tmp.certainty = minSize * minAspect * (1-fabs(2-tmp.scale[0]/tmp.scale[1])/(tmp.scale[0]/tmp.scale[1]));
        tmp.scale[0]*=5;
        tmp.scale[1]*=34;
        bool check = true;
        for(int j=0; j<results.size(); j++) {
          if( tmp == results[j]) { check = false; break;}
          if( results[j] > tmp) { check = false; break;}
          if( tmp > results[j]) { check = false; results[j].certainty = tmp.certainty; break;}
        }
        if(check) {
          results.push_back(tmp);
        }
      }
    }
    circle_list.erase(circle_list.begin());
    if(circle_list.size() % 10 == 0) {
      time_t current_time = time(NULL);
      estimation_time = difftime(current_time, prev_time)*circle_list.size()/(60*60);
      prev_time = current_time;
    }
    cerr << "\033[A\033[2K" << "completed " << initial_size-circle_list.size() << "/" << initial_size << " scans for glasses, estimated completion time is " << estimation_time << endl;
  }
 return results;
}
