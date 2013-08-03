#include <whereswally.h>

using namespace std;
using namespace cv;

int main(int argc, char* argv[] ) {
  IO_Control controller(argc, argv);

  vector<Search_Pattern*> patterns;
  Find_Glasses fg;
  Blue_Trousers bt(&fg, 0.5);
  Red_and_White rw(&bt, 2/3);
  Find_Features ff;
  
//  patterns.push_back(&rw);
  patterns.push_back(&ff);
  controller.start(patterns);
}
