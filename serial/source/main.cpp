#include <whereswally.h>

using namespace std;
using namespace cv;

int main(int argc, char* argv[] ) {

  IO_Control controller(argc, argv);

  vector<Search_Pattern*> patterns;
  Red_and_White rw;
  
  patterns.push_back(&rw);
  controller.start(patterns);
}
