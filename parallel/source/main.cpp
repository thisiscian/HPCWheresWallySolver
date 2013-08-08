#include <whereswally.h>

using namespace std;
using namespace cv;

int main(int argc, char* argv[] ) {
  IO_Control controller(argc, argv);

  vector<Search_Pattern*> patterns;
  Find_Glasses fg;
  Blue_Trousers bt;
  Red_and_White rw;
  //Red_and_White rw(&bt, 0.5);
  Find_Features ff;
  
  //patterns.push_back(&rw);
  //patterns.push_back(&fg);
  //patterns.push_back(&bt);
  patterns.push_back(&ff);
  controller.start(patterns);
}
