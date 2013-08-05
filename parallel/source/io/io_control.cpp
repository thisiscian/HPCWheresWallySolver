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
  if(variables->get_load_results_from_file() ) {
    input->load_results();
  }
  double start = omp_get_wtime();
  if(variables->get_load_puzzle_from_file()) {
    Mat image = input->load_image();
    for(int i=0; i<2; i++) {
      cout << endl;
    }
    omp_set_num_threads(1);
    omp_set_nested(1);
    #pragma omp parallel for
    for(int i=0; i<patterns.size(); i++) {
      vector<Pattern_Result> current_pattern_result = patterns[i]->start_search( image );
      #pragma omp critical
      {
        variables->add_to_loaded_results( current_pattern_result );
      }
    }
  }
  double stop = omp_get_wtime();
  cout << "stop time = " << stop-start << endl;

  Results_Analysis analyser;
  analyser.calculate_final_results(variables->get_number_of_final_results(), variables->get_loaded_results() );
  output->set_final_results( analyser.get_final_results() );
  output->output();
}
