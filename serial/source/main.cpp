#include <whereswally.h>

using namespace std;
using namespace cv;

int main(int argc, char* argv[] ) {

  IO_Control controller(argc, argv);

  vector<Search_Pattern*> patterns;
  Red_and_White rw;
  Find_Glasses fg;
  Find_Features ff;
  
//  patterns.push_back(&rw);
//  patterns.push_back(&fg);
  patterns.push_back(&ff);
  controller.start(patterns);
}
