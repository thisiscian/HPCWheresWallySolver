#include "whereswally/io/io_control.h"

using namespace cv;
using namespace std;

IO_Control::IO_Control(int argc, char *argv[]) {
  variables = new IO_Variables();
  input = new Input(variables);
  output = new Output(variables); 
  input->parse_input(argc, argv);
  if(variables->get_load_config_from_file()) {
    input->load_config();
    input->parse_input(argc,argv); // want user input to override config
  }
}

int IO_Control::start(vector<Search_Pattern*> patterns) {
  Mat image;
  if(variables->get_load_results_from_file() ) {
    input->load_results();
  }
  if(variables->get_load_puzzle_from_file()) {
    image = input->load_image(); 
    for(int i=0; i<patterns.size(); i++) {
      variables->add_to_loaded_results( patterns[i]->start_search(variables->get_loaded_image() ) );
    }
  }

  Results_Analysis analyser;
  analyser.calculate_final_results(variables->get_number_of_final_results(), variables->get_loaded_results() );
  output->set_final_results( analyser.get_final_results() );
  output->output();
}
