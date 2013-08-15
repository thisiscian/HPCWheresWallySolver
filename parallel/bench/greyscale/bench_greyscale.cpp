#include<whereswally.h>
#include<cstdlib>
#include<iostream>
using namespace std;
using namespace cv;
using namespace wwp;

int main(int argc, char* argv[]) {
  if(argc < 3) { return 1; }
  int num_threads = atoi(argv[1]);
  double datasize = atof(argv[2]);
	int iterations = atoi(argv[3]);
  Mat greyscale;
  omp_set_num_threads(num_threads);

  Mat image = imread("greyscale_bench.png", CV_LOAD_IMAGE_COLOR);

  Mat subimage;
  double time = omp_get_wtime();
  subimage = image.rowRange(Range(0,datasize*(image.rows-1)));
	for(int i=0; i<iterations; i++) {
  	greyscale = get_greyscale_in_image(subimage, 0, 255, 0);
  }
	cout << omp_get_wtime()-time << endl;
  return 0;
}
