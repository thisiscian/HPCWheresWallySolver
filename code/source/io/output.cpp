#include "whereswally/io/output.h"
using namespace std;
using namespace cv;

//**
//** CLASS INITIALISATION;
//** 

Output::Output(IO_Variables *new_variables) {
  variables = new_variables;
}

//**
//** HELPER FUNCTIONS
//**

void Output::show_text_results() {
  cout << "Found potential Wallys in the following locations:" << endl;
  for(size_t i=0; i<final_results.size(); i++ ) {
    cout << "\t \"" << final_results[i].info.get_name() << "\"";
    cout << "(" << i+1 << "):";
    cout << final_results[i].certainty*100 << "\% certain of a Wally at ";
    cout << final_results[i].wally_location[0] << "x" << final_results[i].wally_location[1];
    cout << ", size " << final_results[i].scale[0] << "x" << final_results[i].scale[1] << endl;
  }
  cout << endl;
}

void Output::show_graphic_results() {
  Mat image = variables->get_loaded_image();
  Point center;
  Size scale;
  string window_name = "Where's Wally Solver";
  namedWindow(window_name, CV_WINDOW_NORMAL);
  resizeWindow(window_name, 1440,900);
  Mat mask(image.rows, image.cols, CV_8UC3, Scalar(0,0,0));
  // for the top results, draw a circle indicating Wally's location and a box labelling the result
  for(size_t i=0; i<final_results.size(); i++ ) {
    center.x = final_results[i].wally_location[0];
    center.y = final_results[i].wally_location[1];
    scale.width = final_results[i].scale[0];
    scale.height = final_results[i].scale[1];
    ellipse(mask,center, scale,0,0,360,Scalar(1,1,1),-1);
    }
  multiply(image, mask, mask);
  image = image/2 + mask/2;
  for(int i=(int)final_results.size()-1; i>=0; i-- ) {
    center.x = final_results[i].wally_location[0];
    center.y = final_results[i].wally_location[1];
    scale.width = final_results[i].scale[0];
    scale.height = final_results[i].scale[1];
    stringstream text;
    text.precision(3);
    text.width(2);
    text << "#" << i+1; // << ": " << final_results[i].certainty*100 << "%" << flush;
    int fontFace = FONT_HERSHEY_SCRIPT_SIMPLEX; 
    int baseline = 0;
    Size textSize = getTextSize(text.str(), fontFace, 0.4, 2, &baseline);
    int len=8;
    for(int i=0; i<len;i++) {
      ellipse(image,center, scale,0,360*(2*i)/(2*len),360*(2*i+1)/(2*len),Scalar(255,255,255),4);
      ellipse(image,center, scale,0,360*(2*i+1)/(2*len), 360*(2*i+2)/(2*len),Scalar(0,0,255),4);
    }
    rectangle(image, Point(center.x-textSize.width/2,center.y+scale.height-textSize.height-12),Point(center.x+textSize.width/2,center.y+scale.height+textSize.height-12), Scalar::all(255), -1);
    rectangle(image, Point(center.x-textSize.width/2,center.y+scale.height-textSize.height-12),Point(center.x+textSize.width/2,center.y+scale.height+textSize.height-12), Scalar::all(0), 2);
    putText(image, text.str(), Point(center.x-(textSize.width/2),center.y+scale.height-textSize.height),fontFace, 0.4, Scalar::all(0), 1);
}

  imshow(window_name, image);
  imwrite("out.png",image);
  waitKey();
  destroyWindow(window_name);
}

void Output::show_timing_results() {
  vector<Time_Message> timing_results = variables->get_timing_results();
  cout << "Section Timings (in seconds)" << endl;
  cout << "----------------------------" << endl;
  for(size_t i=0; i<timing_results.size(); i++) { 
    cout << timing_results[i].message << ": " << timing_results[i].time << endl;
  }
}

//**
//** GET/SET METHODS
//**

vector<Pattern_Result> Output::get_final_results() {
  return final_results;
}

void Output::set_final_results(std::vector<Pattern_Result> results) {
  final_results = results;
}

//**
//**  OUTPUT DATA
//**

void Output::output() {
  if( variables->get_show_text_results() ) {
    show_text_results();
  }
  if( variables->get_show_timing_results() ) {
    show_timing_results();
  }
  if( variables->get_show_graphic_results() ) { 
    show_graphic_results();
  }
}
