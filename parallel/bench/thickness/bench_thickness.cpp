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
  double average;
  omp_set_num_threads(num_threads);

  Mat image = imread("line_width_speed_test.png", CV_LOAD_IMAGE_COLOR);

  Mat subimage;
  double time = omp_get_wtime();
  subimage = image.rowRange(Range(0,datasize*(image.rows-1)));
	for(int i=0; i<iterations; i++) {
  	average = estimate_black_line_thickness(subimage, 50, 50);
  }
	cout << omp_get_wtime()-time << " " << average << endl;
}
