#include "../headers/framework/results_analysis.h"
#include "../headers/framework/pattern_information.h"
#include "../headers/framework/search_pattern.h"
#include "../headers/io/io_control.h"

#include "../headers/patterns/redandwhite.h"

using namespace std;
using namespace cv;

class Trivial_Search_Pattern : public Search_Pattern {
  public:
    Trivial_Search_Pattern() {
      info.set_name("trivial");
      info.set_description("this is trivial");
      info.set_confidence(0.5);
    }
    vector<Pattern_Result> start_search(Mat image) {
      vector<Pattern_Result> bad_results;
        Pattern_Result res;
        res.info = info;
        res.certainty= 0.5;
        res.wally_location[0] = 500;
        res.wally_location[1] = 300;
        res.scale[0] = 80;
        res.scale[1] = 90;
      bad_results.push_back(res);
        res.certainty= 0.4;
        res.wally_location[0] = 800;
        res.wally_location[1] = 20;
        res.scale[0] = 20;
        res.scale[1] = 20;
      bad_results.push_back(res);
        res.certainty= 0.3;
        res.wally_location[0] = 1500;
        res.wally_location[1] = 600;
        res.scale[0] = 100;
        res.scale[1] = 200;
      bad_results.push_back(res);
        res.certainty= 0.2;
        res.wally_location[0] = 2;
        res.wally_location[1] = 2;
        res.scale[0] = 2;
        res.scale[1] = 2;
      bad_results.push_back(res);
        res.certainty= 0.1;
        res.wally_location[0] = 1;
        res.wally_location[1] = 1;
        res.scale[0] = 1;
        res.scale[1] = 1;
      bad_results.push_back(res);
        res.certainty= 0.0;
        res.wally_location[0] = 0;
        res.wally_location[1] = 0;
        res.scale[0] = 0;
        res.scale[1] = 0;
      bad_results.push_back(res);
 
      return bad_results;
    }
};

int main(int argc, char* argv[] ) {

  IO_Control controller(argc, argv);

  vector<Search_Pattern*> patterns;
  Trivial_Search_Pattern tsp;
  Red_and_White rw;
  
  patterns.push_back(&rw);
  controller.start(patterns);
}
