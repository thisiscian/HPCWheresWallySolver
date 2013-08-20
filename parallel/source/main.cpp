#include <whereswally.h>

using namespace std;
using namespace cv;

int main(int argc, char* argv[] ) {
  IO_Control controller(argc, argv);

  //-- Define patterns that will be used to search for Wally
  Find_Glasses fg;
  Blue_Trousers bt;
  Red_and_White rw;
  Find_Features ff;
  
  //-- Add patterns to list
  vector<Search_Pattern*> patterns;
  patterns.push_back(&rw);
  //patterns.push_back(&fg);
  patterns.push_back(&bt);
  //patterns.push_back(&ff);

  //-- Start the search for Wally
  controller.start(patterns);
}
