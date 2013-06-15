#include "../../headers/io/io_control.h"

IO_Control::IO_Control(int argc, char *argv[]) {
  variables = new IO_Variables();
  input = new Input(variables);
  output = new Output(variables); 
  input->parse_input(argc, argv);
//  list_of_patterns = new Search_Pattern[]
}

int IO_Control::start() {
  Mat image;
  
  if(variables->get_load_config_from_file()) {
    input->load_config();
  }
  input->parse_input(argc,argv); // want user input to override config

  if(variables->get_load_image_from_file()) {
    image = input->load_image(); 
  } else if (variables->get_load_results_from_file() ) {
    input->load_results();
  }
}
