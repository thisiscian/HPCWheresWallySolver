#include "whereswally/io/io_control.h"
using namespace cv;
using namespace std;

//**
//** CLASS INITIALISATION
//**

IO_Control::IO_Control(int argc, char *argv[]) {
  variables = new IO_Variables();
  input = new Input(variables);
  output = new Output(variables); 
  int err = input->parse_input(argc, argv);
  if( err != 0  || variables->get_puzzle_input_filename() == "") {
    variables->set_load_puzzle_from_file(false);
    variables->set_show_text_results(false);
    variables->set_show_graphic_results(false);
    variables->set_show_timing_results(false);
  }
}

//**
//** CONTROLLER FUNCTIONS
//**

int IO_Control::start(vector<Search_Pattern*> patterns) {
  double solve_start = omp_get_wtime();

  //-- if allowed to load puzzle from file
  if(variables->get_load_puzzle_from_file()) {
    double start = omp_get_wtime();
    Mat image = input->load_image();
    //-- get time to load image from file; purely serial
    variables->add_timing_result("Load Image", omp_get_wtime()-start);

    //-- make room for each thread to print it's current task
    for(int i=0; i<variables->get_number_of_openmp_threads(); i++) {
      cout << endl;
    }

    omp_set_num_threads(variables->get_number_of_openmp_threads());
    setNumThreads(0);
    omp_set_nested(1);
    #pragma omp parallel for default(none) shared(image, patterns)
    for(int i=0; i<patterns.size(); i++) {
      double start = omp_get_wtime();
      vector<Pattern_Result> current_pattern_result = patterns[i]->start_search( image );
      #pragma omp critical
      {
        variables->add_to_loaded_results( current_pattern_result );
        variables->add_timing_result(patterns[i]->info.get_name(), omp_get_wtime()-start);
      }
    }
  }
  variables->add_timing_result("Total Time", omp_get_wtime()-solve_start);

  Results_Analysis analyser;
  analyser.calculate_final_results(variables->get_number_of_final_results(), variables->get_loaded_results() );
  output->set_final_results( analyser.get_final_results() );
  output->output();
  delete variables;
  delete input;
  delete output;
}
