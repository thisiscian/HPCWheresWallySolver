#include <whereswally/patterns.h>
#include <ctime>
#include <iostream>
#include <iomanip>
#include "opencv2/highgui/highgui.hpp"
using namespace std;
using namespace cv;
using namespace wwp;

//**
//**  CLASS INITIALISATION
//**

Find_Glasses::Find_Glasses() {
  info.set_name("Find Glasses");
  info.set_description("Locate Wally by areas that look like glasses");
  info.set_confidence(0.1);
  rescale = 2; //-- change the size of the image to get better resolution
  thresholds = 3;
  allowed_irregularity = 0.4;
}

//**
//** HELPER FUNCTIONS
//**

//-- calculate the angle between two points
double Find_Glasses::calculate_angle(Point p0, Point p1, Point p2) {
  int line1[2] = {p1.x-p0.x,p1.y-p0.y};
  int line2[2] = {p2.x-p0.x,p2.y-p0.y};
  return (line1[0]*line2[0]+line1[1]*line2[1])/sqrt((line1[1]*line1[1]+line1[0]*line1[0])*(line2[1]*line2[1]+line2[0]*line2[0])+1e-10);
}

//-- extract region information from a polynomial contour
region Find_Glasses::extract_contour_info(vector<Point> contour) {
  region reg;
  reg.size = contourArea(contour)/rescale;
  for(size_t i=0; i<contour.size(); i++) {
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

//**
//**  SEARCH PATTERN
//**

vector<Pattern_Result> Find_Glasses::start_search(Mat image) {
  
  //-- get thickness of line in image
  double avg_thickness = estimate_black_line_thickness(image, 50, 50); 
  int max_thickness = floor(avg_thickness)+1;
  int min_thickness = floor(avg_thickness)-1;
  min_thickness = (min_thickness < 1)? 1 : min_thickness;

  //-- estimate largest size a pair of glasses could be
  int max_glasses_size[2] = {30*max_thickness, 14*max_thickness};

  //-- decompose the image into subsections such that each subimage
  //--  is twice the size of the max glasses size in both directions
  //--  and so that each subimage overlaps the other subimages by a max glasses size
  int decomposition[2] = {image.cols/(max_glasses_size[0])-1, image.rows/(max_glasses_size[1])-1};
  if(decomposition[0] < 1) {decomposition[0] = 1;}
  if(decomposition[1] < 1) {decomposition[1] = 1;}

  Mat skin_colour = get_colour_in_image(image, "#907070", "#FFE0e0", 0,0,0,0,0,0);
  GaussianBlur(skin_colour, skin_colour, Size(), 5);
  skin_colour = (skin_colour > 20)/255;
 //-- convert to grey and resize to get more resolution on image
  Mat grey_image;
  cvtColor(image,grey_image,CV_RGB2GRAY);
  multiply(skin_colour,grey_image,grey_image);
  resize(grey_image, grey_image, Size(), rescale,rescale,INTER_CUBIC);
   
  //-- sharpen image with unsharp mask
  Mat blur_grey;
  GaussianBlur(grey_image, blur_grey, Size(), 1);
  addWeighted(grey_image, 1.5, blur_grey, -0.7, 0, grey_image);


  vector<Pattern_Result> results;
  #pragma omp parallel for default(none) shared(decomposition, grey_image, results) firstprivate(max_thickness,min_thickness)
  for(int i=0; i<decomposition[1]*decomposition[0]; i++) {
    int x = i%decomposition[0];
    int y = i/decomposition[0];
    vector<region> circle_list;
    Mat threshold_grey;
    Mat subimage = grey_image(Range(y*(grey_image.rows-1)/(decomposition[1]+1),(y+2)*(grey_image.rows-1)/(decomposition[1]+1)), Range(x*(grey_image.cols-1)/(decomposition[0]+1), (x+2)*(grey_image.cols-1)/(decomposition[0]+1)));
    //-- scan the image through varying thresholds, which allow blurred lines to be progressively accessed
    for(int j=0; j<thresholds; j++) {
      threshold(subimage, threshold_grey, 50*(j+1)/thresholds+75, 255, THRESH_BINARY); // thresh over range of 125-225

      vector< vector<Point> > contours;
      findContours(threshold_grey, contours,CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
      //-- check that each contour found is approximately circular
      for(size_t k=0; k<contours.size(); k++) {
        vector<Point> approx=contours[k];
        approxPolyDP(Mat(contours[k]), approx, arcLength(Mat(contours[k]), true)*0.02, true);
        //-- check that contour is not too small or too large
        if(approx.size() >= 6 && isContourConvex(Mat(approx)) && contourArea(approx) >= 6*rescale*pow(min_thickness,2) && contourArea(approx) <= 1000*rescale*pow(max_thickness,2)) {
          double irregularity = 0;
          // irregularity is the average angular deviation from the angle a regular polygon with the same number of sides would have
          for(size_t l=2; l<approx.size(); l++) {
            irregularity += fabs(fabs(cos(180.0-360.0/approx.size()))-fabs(calculate_angle(approx[l], approx[l-1], approx[l-2])));
          }
          if(irregularity <= allowed_irregularity*(approx.size()-2)) {
            circle_list.push_back(extract_contour_info(approx));
          }
        } 
      }
    }
    threshold_grey.release();
    
    //-- scane the circle_list to find pairs that resemble glasses
    while(circle_list.size() > 0) {
      int width = (circle_list[0].largest_x-circle_list[0].smallest_x);
      int height = (circle_list[0].largest_y-circle_list[0].smallest_y);
      double aspect = min((double)(width)/height, (double)(height)/width);
      for(size_t i=1; i< circle_list.size(); i++) {
        int myWidth = (circle_list[i].largest_x-circle_list[i].smallest_x);
        int myHeight = (circle_list[i].largest_y-circle_list[i].smallest_y);
        int dist_x = abs(circle_list[0].av_x-circle_list[i].av_x); 
        int dist_y = abs(circle_list[0].av_y-circle_list[i].av_y); 
        bool nearby_x = (  dist_x < 2*width && dist_x < 2*myWidth && dist_x > 0.4*(myWidth+width));
        bool nearby_y = ( dist_y < 0.1*height && dist_y < 0.1*myHeight);
        //-- if circles are not nearby, try another circle
        if( !nearby_x && !nearby_y ) {
          continue;
        }
        bool sameWidth = ( 1- min((double)(width)/myWidth, (double)(myWidth)/width) < 0.4 );
        bool sameHeight = ( 1- min((double)(height)/myHeight, (double)(myHeight)/height) < 0.4 );
        if( sameWidth && sameHeight) {
          Pattern_Result tmp;
          tmp.info = info;
          tmp.wally_location[0] = (circle_list[0].av_x+circle_list[i].av_x)/2 + x*(grey_image.cols-1)/((decomposition[0]+1)*rescale);
          //-- extra 16*... is to move the center to where the center of Wally would be 
          tmp.wally_location[1] = (circle_list[0].av_y+circle_list[i].av_y)/2 + y*(grey_image.rows-1)/((decomposition[1]+1)*rescale)+16*(circle_list[0].largest_y-circle_list[0].smallest_y);
          tmp.scale[0] = (max(circle_list[0].largest_x, circle_list[i].largest_x) - min(circle_list[0].smallest_x, circle_list[i].smallest_x))/2+1;
          tmp.scale[1] = (max(circle_list[0].largest_y, circle_list[i].largest_y) - min(circle_list[0].smallest_y, circle_list[i].smallest_y))/2+1;

          double minSize = min((double)(circle_list[0].size)/circle_list[i].size, (double)(circle_list[i].size)/circle_list[0].size);
          double myAspect = min((double)(myHeight)/myWidth, (double)(myWidth)/myHeight);
          double minAspect =  min(aspect/myAspect ,myAspect/aspect );

          tmp.certainty = minSize * minAspect * (1-fabs(2-tmp.scale[0]/tmp.scale[1])/(tmp.scale[0]/tmp.scale[1]));
          if(tmp.certainty <= 0) {
            continue;
          }
          //-- resize so that it covers a full wally, not just the glasses
          tmp.scale[0]*=5;
          tmp.scale[1]*=34;
          #pragma omp critical (export_results)
          {
            results.push_back(tmp);
          }
        }
      }
      circle_list.erase(circle_list.begin());
    }
    subimage.release();
  }
  grey_image.release();
  return results;
}
