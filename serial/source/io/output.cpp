#include "../../headers/io/output.h"

using namespace std;

Output::Output(IO_Variables *new_variables) {
  variables = new_variables;
}

void Output::show_text_results() {
  cout << "Found potential Wallys in the following locations:" << endl;
  for(int i=0; i<final_results.size(); i++ ) {
    cout << "\t \"" << final_results[i].info.get_name() << "\"";
    cout << ": " << final_results[i].certainty*100 << "\% certain of a Wally at ";
    cout << final_results[i].wally_location[0] << "x" << final_results[i].wally_location[1];
    cout << ", size " << final_results[i].scale[0] << "x" << final_results[i].scale[1] << endl;
  }
  cout << endl;
}

void Output::save_text_to_file() {
}

void Output::show_graphic_results() {
}

void Output::save_graphic_to_file() {
}

void Output::save_results_to_file() {
}

vector<Pattern_Result> Output::get_final_results() {
  return final_results;
}

void Output::set_final_results(std::vector<Pattern_Result> results) {
  final_results = results;
}

void Output::output() {
  if( variables->get_show_text_results() ) {
    show_text_results();
  }
}
