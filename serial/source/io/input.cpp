#include "../../headers/io/input.h"

using namespace std;
using namespace cv;

Input::Input(IO_Variables *new_variables) {
  variables = new_variables;
}

int Input::parse_input(int argc, char* argv[]) {
  if(argc == 1) {
    cerr << "usage: " << argv[0] << " [options [files]]" << endl;
    cerr << "\t-c,--config=CONFIG_FILE\tSets the puzzle solving configuration as described by the given file" << endl;
    cerr << "\t-n,--number-of-results=N\tSets the number of results to be output" << endl;
    cerr << "\t-p,--solve-puzzle=PUZZLE_FILE\tAttempts to locate Wally in the PUZZLE_FILE" << endl;
    cerr << "\t-l,--load-results=RESULTS_FILE\tLoads results from a previous execution" << endl;
    cerr << "\t-Op,--print-output={TRUE,FALSE}\tPrints the results to the terminal" << endl;
    cerr << "\t-Od,--display-output={TRUE,FALSE}\tShows the results graphically, in a new window" << endl;
    cerr << "\t-St,--save-text-output=OUTPUT_FILE\tSaves the textual format of the results to OUTPUT_FILE" << endl;
    cerr << "\t-Si,--save-image-output=OUTPUT_FILE\tSaves the graphical format of the results to OUTPUT_FILE" << endl;
    cerr << "\t-h,--help[=COMMAND]\tDisplays this help message, or more detailed help associated with a specific command" << endl;
    return NO_ERRORS;
  } else {
    
  }
  return UNDEFINED;
}

int Input::load_config() {
  return UNDEFINED;
}

int Input::load_results() {
  return UNDEFINED;
}

void Input::set_variables(IO_Variables new_variables) {
}

Mat Input::load_image() {
  Mat placeholder;
  return placeholder;
}
