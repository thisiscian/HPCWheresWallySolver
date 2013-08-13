#include<whereswally.h>
#include<cstdlib>
#include<iostream>
using namespace std;
using namespace cv;
using namespace wwp;

int main(int argc, char* argv[]) {
  if(argc < 3) { return 1; }
  int num_threads = atoi(argv[1]);
  int iterations = atoi(argv[2]);
  double average;
  omp_set_num_threads(num_threads);

  Mat image = imread("line_width_speed_test.png", CV_LOAD_IMAGE_COLOR);

  Mat subimage;
  double time = omp_get_wtime();
  for(int i=1; i<=iterations; i++) {
    subimage = image(Range(0,i+1*(image.cols-1)/iterations),Range(0,i*(image.rows-1)/iterations));
		for(int j=0; j<10; j++) {
    	average = estimate_black_line_thickness(subimage, 50, 50);
		}
  }
	cout << omp_get_wtime()-time << endl;
}
