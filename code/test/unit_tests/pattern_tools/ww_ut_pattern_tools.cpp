#include "ww_ut_pattern_tools.h"
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

using namespace std;
using namespace cv;
using namespace CppUnit;
using namespace wwp;

CPPUNIT_TEST_SUITE_REGISTRATION(pattern_tools_test);

// initialises variables for tests to use
void pattern_tools_test::setUp() {
  region_mask = imread("../../sample_files/test_region.png", 0);
  greyscale = imread("../../sample_files/test_greyscale.png", CV_LOAD_IMAGE_COLOR);
  colour = imread("../../sample_files/test_colour.png", CV_LOAD_IMAGE_COLOR);
  monitor.silence();
}

// frees variables test have finished using
void pattern_tools_test::tearDown() {
  region_mask.release();
  monitor.unsilence();
}

// test that regions can be found
void pattern_tools_test::test_find_regions_from_mask() {
  region one,two,three,four;
  one.size = 2401;
  one.av_x = 24;
  one.av_y = 24;
  one.smallest_x = 0;
  one.largest_x = 48;
  one.smallest_y = 0;
  one.largest_y = 48;

  two.size = 2176;
  two.av_x = 23.8966;
  two.av_y = 74.8966;
  two.smallest_x = 0;
  two.largest_x = 48;
  two.smallest_y = 51;
  two.largest_y = 99;

  three.size = 964;
  three.av_x = 72.7033;
  three.av_y = 25.138;
  three.smallest_x = 54;
  three.largest_x = 93;
  three.smallest_y = 8;
  three.largest_y = 44;

  four.size = 1704;
  four.av_x = 79.9454;
  four.av_y = 79.5164;
  four.smallest_x = 54;
  four.largest_x = 99;
  four.smallest_y = 53;
  four.largest_y = 99;
  
  vector<region> cmp_regions;
  cmp_regions.push_back(one);
  cmp_regions.push_back(two);
  cmp_regions.push_back(three);
  cmp_regions.push_back(four);

  vector<region> tmp_regions = find_regions_from_mask(region_mask);
  // there are 4 regions (plus the null region) in the sample image;
  CPPUNIT_ASSERT( tmp_regions.size() == 4 );

  // the regions found should match the above defined regions
  CPPUNIT_ASSERT( tmp_regions == cmp_regions );
}

void pattern_tools_test::test_region_comparison_operators() {
  region one,two,three;
  one.size = 2401;
  one.av_x = 24;
  one.av_y = 24;
  one.smallest_x = 0;
  one.largest_x = 48;
  one.smallest_y = 0;
  one.largest_y = 48;

  two.size = 2401;
  two.av_x = 24;
  two.av_y = 24;
  two.smallest_x = 0;
  two.largest_x = 48;
  two.smallest_y = 0;
  two.largest_y = 48;

  three.size = 2176;
  three.av_x = 23.8966;
  three.av_y = 74.8966;
  three.smallest_x = 0;
  three.largest_x = 48;
  three.smallest_y = 51;
  three.largest_y = 99;

  CPPUNIT_ASSERT( one == two );
  CPPUNIT_ASSERT( !(one == three) );
}

void pattern_tools_test::test_get_greyscale_in_image() {
  // output should be a single channel binary matrix (values are 0 or 255)
  Mat output = get_greyscale_in_image(greyscale,0,255,255);
  CPPUNIT_ASSERT( output.channels() == 1 );
  for(int i=0; i<output.rows; i++) {
    for(int j=0; j<output.cols; j++) {
      int x = output.at<uchar>(i,j);
      CPPUNIT_ASSERT( (x == 0) || (x == 255) );
    }
  }

  // should be able to locate regions of pure colour
  Mat black = Mat::zeros(output.size(), 0);
  Mat white = Mat::zeros(output.size(), 0);
  Mat grey = Mat::zeros(output.size(), 0);
  Mat darkgrey = Mat::zeros(output.size(), 0);
    for(int i=0; i<50; i++) {
    for(int j=0; j<50; j++) {
      black.at<uchar>(i,j) = 255;
      grey.at<uchar>(i+50,j) = 255;
      darkgrey.at<uchar>(i,j+50) = 255;
      white.at<uchar>(i+50,j+50) = 255;
    }
  }
  // check black corner
  output = get_greyscale_in_image(greyscale,0,0,0);
  CPPUNIT_ASSERT ( countNonZero(output != black) == 0 );
  //check white corner
  output = get_greyscale_in_image(greyscale,255,255,0);
  CPPUNIT_ASSERT ( countNonZero(output != white) == 0 );
  //check dark grey corner
  output = get_greyscale_in_image(greyscale,13,13,0);
  CPPUNIT_ASSERT ( countNonZero(output != darkgrey) == 0 );

  //should be able to check a range of colours
  output = get_greyscale_in_image(greyscale,0,13,0);
  CPPUNIT_ASSERT ( countNonZero(output != (black+darkgrey)) == 0 );
  
  //should be able to search with a tolerance of colour
  output = get_greyscale_in_image(greyscale, 128,128, 28);
  CPPUNIT_ASSERT ( countNonZero(output != grey) == 0 );
  
  //should be able to check with a range of tolerances
  output = get_greyscale_in_image(greyscale, 0, 128,28);
  CPPUNIT_ASSERT ( countNonZero(output != (grey+darkgrey+black)) == 0 );
}

void pattern_tools_test::test_get_colour_in_image() {
  // output should be a single channel binary matrix (values are 0 or 255)
  Mat output = get_colour_in_image(colour,"#000000","#FFFFFF",0,0,0,0,0,0);
  CPPUNIT_ASSERT( output.channels() == 1 );
  for(int i=0; i<output.rows; i++) {
    for(int j=0; j<output.cols; j++) {
      int x = output.at<uchar>(i,j);
      CPPUNIT_ASSERT( (x == 0) || (x == 255) );
    }
  }
  Mat black = Mat::zeros(output.size(), 0);
  Mat green = Mat::zeros(output.size(), 0);
  Mat red = Mat::zeros(output.size(), 0);
  Mat yellow = Mat::zeros(output.size(), 0);
  Mat skyblue = Mat::zeros(output.size(), 0);
  Mat blue = Mat::zeros(output.size(), 0);
  Mat pink = Mat::zeros(output.size(), 0);
  Mat white = Mat::zeros(output.size(), 0);
    for(int i=0; i<25; i++) {
    for(int j=0; j<50; j++) {
      black.at<uchar>(i,j) = 255;
      green.at<uchar>(i,j+50) = 255;
      red.at<uchar>(i+25,j) = 255;
      yellow.at<uchar>(i+25,j+50) = 255;
      skyblue.at<uchar>(i+50,j) = 255;
      blue.at<uchar>(i+50,j+50) = 255;
      pink.at<uchar>(i+75,j) = 255;
      white.at<uchar>(i+75,j+50) = 255;
    }
  }

  // specific colours should be findable
  output = get_colour_in_image(colour,"#000000","#000000",0,0,0,0,0,0);
  CPPUNIT_ASSERT ( countNonZero(output != black) == 0 );
  output = get_colour_in_image(colour,"#FF0000","#FF0000",0,0,0,0,0,0);
  CPPUNIT_ASSERT ( countNonZero(output != red) == 0 );
  output = get_colour_in_image(colour,"#00FF00","#00FF00",0,0,0,0,0,0);
  CPPUNIT_ASSERT ( countNonZero(output != green) == 0 );
  output = get_colour_in_image(colour,"#0000FF","#0000FF",0,0,0,0,0,0);
  CPPUNIT_ASSERT ( countNonZero(output != blue) == 0 );
  output = get_colour_in_image(colour,"#FFFF00","#FFFF00",0,0,0,0,0,0);
  CPPUNIT_ASSERT ( countNonZero(output != yellow) == 0 );
  output = get_colour_in_image(colour,"#FF00FF","#FF00FF",0,0,0,0,0,0);
  CPPUNIT_ASSERT ( countNonZero(output != pink) == 0 );
  output = get_colour_in_image(colour,"#00FFFF","#00FFFF",0,0,0,0,0,0);
  CPPUNIT_ASSERT ( countNonZero(output != skyblue) == 0 );
  output = get_colour_in_image(colour,"#FFFFFF","#FFFFFF",0,0,0,0,0,0);
  CPPUNIT_ASSERT ( countNonZero(output != white) == 0 );

  // types of colours should be findable
  // just red
  output = get_colour_in_image(colour, "#010000", "#FFFEFE", 1.2,0,1.2,0,0,0);
  CPPUNIT_ASSERT (countNonZero(output != red) == 0);
  // anything on the side of the spectrum
  output = get_colour_in_image(colour, "#000000", "#FFFFFF", 1,0,1,0,0,0);
  CPPUNIT_ASSERT (countNonZero(output != red+pink+yellow+white+black) == 0);
  
}

int main(int argc, char* argv[]) {
  Test *suite = TestFactoryRegistry::getRegistry().makeTest();

  TextUi::TestRunner runner;
  runner.addTest( suite );

  runner.setOutputter( new CompilerOutputter(&runner.result(),cerr));

  bool wasSucessful = runner.run();
  return wasSucessful ? 0 : 1;
}
