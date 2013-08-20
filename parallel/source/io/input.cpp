#include "whereswally/io/input.h"
using namespace std;
using namespace cv;
using namespace wwp;

//**
//** CLASS INITIALISATION
//**

Input::Input(IO_Variables *new_variables) {
  variables = new_variables;
}

//**
//** HELPER FUNCTIONS
//**

//-- wrapper that allows tolower to be used as a function pointer (used for TRUE/FALSE->true/false)
int wwp::to_lower_case(int c) {
  return tolower(c);
}

//-- removes the whitespace is a string, to enable more simple parsing of strings
string wwp::remove_whitespace(string in_string) {
  for(size_t i=0; i<in_string.size(); i++) {
    if(isspace(in_string[i])) {
      in_string.erase(i,1); 
      i--;
    }
  }
  return in_string;
}

string wwp::get_option_flag(int num, char* argv[]) {
  string input = argv[num]; 
  if(input.find("=") != string::npos) {
    return input.substr(0,input.find("=")+1);
  } else if(input.size() >= 2 && input[1]=='-') {
    input+="=";
    return input;
  } else {
    input+=" ";
    return input;
  }
}

//-- parses user input to get the option, e.g. --flag=option of -flag option will return option
string wwp::get_option_variable(int *num, int argc, char* argv[]) {
  string in_string = argv[*num];
  if(in_string.size() >= 2 && in_string[1] == '-') {
    size_t eq_location = in_string.find("=");
    if(eq_location == string::npos) {
      return "";
    }
    else {
      return in_string.substr(eq_location+1);
    }
  } else if (*num+1 < argc) {
    *num+=1;
    return argv[*num];
  } else { 
    return "";
  }
}

//**
//**  INPUT FUNCTIONS
//** 

//-- set final results if format is correct
int Input::option_number_of_final_results(int *num, int argc, char *argv[]) {
  string input = get_option_flag(*num, argv);
  string option = get_option_variable(num, argc, argv);
  if(option == "" || option[0] == '-') {
    cerr << err_msg("option_number_of_final_results", input.c_str());
    return BAD_COMMAND_USAGE; 
  }
  int n = strtol (option.c_str(),NULL,10);
  variables->set_number_of_final_results(n);
  return NO_ERRORS;
}

//-- set omp threads if format is correct
int Input::option_number_of_openmp_threads(int *num, int argc, char *argv[]) {
  string input = get_option_flag(*num, argv);
  string option = get_option_variable(num, argc, argv);

  if(option == "" || option[0] == '-') {
    cerr << err_msg("option_number_of_openmp_threads", input.c_str());
    return BAD_COMMAND_USAGE; 
  }
  int n = strtol (option.c_str(),NULL,10);
  variables->set_number_of_openmp_threads(n);
  return NO_ERRORS;
}

//-- load a puzzle if format is correct
int Input::option_load_puzzle(int *num, int argc, char *argv[]) {
  string input = get_option_flag(*num, argv);
  string option = get_option_variable(num, argc, argv);

  if(option == "" || option[0] == '-') { 
    cerr << err_msg("option_load_puzzle", input.c_str());
    return BAD_COMMAND_USAGE;
  }
  
  variables->set_puzzle_input_filename(option);
  return NO_ERRORS;
}


//-- show text based results if format is correct
int Input::option_show_text(int *num, int argc, char *argv[]) {
  string input = get_option_flag(*num, argv);
  string option = get_option_variable(num, argc, argv);

  if(option == "" || option[0] == '-') { 
    cerr << err_msg("option_show_text", input.c_str());
    return BAD_COMMAND_USAGE;
  }
  
  transform(option.begin(), option.end(), option.begin(), to_lower_case); //converts whole string to lower case
  if(option == "verbose") {
    variables->set_show_text_results(true);
    variables->set_make_text_verbose(true);
  } else if(option == "true" ) {
    variables->set_show_text_results(true);
    variables->set_make_text_verbose(false);
  } else if(option == "false") {
    variables->set_show_text_results(false);
    variables->set_make_text_verbose(false);
  } else {
    cerr << err_msg("option_show_text", input.c_str());
    return BAD_COMMAND_USAGE;
  }
  return NO_ERRORS;
}

//-- show graphic output
int Input::option_show_graphic(int *num, int argc, char *argv[]) {
  string input = get_option_flag(*num, argv);
  string option = get_option_variable(num, argc, argv);

  if(option == "" || option[0] == '-') { 
    cerr << err_msg("option_show_graphic", input.c_str());
    return BAD_COMMAND_USAGE;
  }

  transform(option.begin(), option.end(), option.begin(), to_lower_case); //converts string to lower case
  if(option == "verbose") {
    variables->set_show_graphic_results(true);
    variables->set_make_graphic_verbose(true);
  } else if(option == "true" ) {
    variables->set_show_graphic_results(true);
    variables->set_make_graphic_verbose(false);
  } else if(option == "false") {
    variables->set_show_graphic_results(false);
    variables->set_make_graphic_verbose(false);
  } else {
    cerr << err_msg("option_show_graphic", input.c_str());
    return BAD_COMMAND_USAGE;
  }
  return NO_ERRORS;
}

//-- show timing results
int Input::option_show_timing(int *num, int argc, char *argv[]) {
  string input = get_option_flag(*num, argv);
  string option = get_option_variable(num, argc, argv);

  if(option == "" || option[0] == '-') { 
    cerr << err_msg("option_show_timing", input.c_str());
    return BAD_COMMAND_USAGE;
  }
  
  transform(option.begin(), option.end(), option.begin(), to_lower_case); //converts string to lower case
  if(option == "true") {
    variables->set_show_timing_results(true);
  } else if (option == "false") {
    variables->set_show_timing_results(false);
  } else {
    cerr << err_msg("option_show_timing", input.c_str());
    return BAD_COMMAND_USAGE;
  }
  return NO_ERRORS;
}

//-- show help message
int Input::option_help(int *num, int argc, char *argv[]) {
  cerr << err_msg("option_help", argv[0]);
  return NO_ERRORS_QUIT;
}


//-- parse user input
int Input::parse_input(int argc, char* argv[]) {
  string argv_i, file;
  int err = NO_ERRORS;
  if(argc == 1) {
    cerr << err_msg("option_help", argv[0]);
    return NO_ERRORS_QUIT;
  } else {
    int i=1;
    while(i<argc && err == NO_ERRORS) {
      argv_i = argv[i];
      if (argv_i == "-omp" || argv_i.find("--openmp") != string::npos) {
          err = option_number_of_openmp_threads(&i, argc, argv);
      } else if(argv_i == "-n" || argv_i.find("--number-of-final-results") != string::npos) {
          err = option_number_of_final_results(&i, argc, argv);
      }  else if(argv_i == "-p" || argv_i.find("--load-puzzle") != string::npos) {
          err = option_load_puzzle(&i, argc, argv);
      } else if(argv_i == "-Oc" || argv_i.find("--show-text") != string::npos) {
          err = option_show_text(&i, argc, argv);
      } else if(argv_i == "-Og" || argv_i.find("--show-graphic") != string::npos) {
          err = option_show_graphic(&i, argc, argv);
      } else if(argv_i == "-Ot" || argv_i.find("--show-timing") != string::npos) {
          err = option_show_timing(&i, argc, argv);
      } else if(argv_i == "-h" || argv_i == "--help" ) {
          err = option_help(&i, argc, argv);
      } else if(argv_i[0] == '-') {
        cerr << "Error: unrecognised command \"" << argv_i << "\", exiting" << endl;
        err = UNRECOGNISED_COMMAND;
      } else {
        variables->set_puzzle_input_filename(argv_i);
      }
      i++;
    }
  }
  return err;
}

//-- load the puzzle image from file
Mat Input::load_image() {
  Mat image = imread(variables->get_puzzle_input_filename(), 1);
  variables->set_loaded_image(image);
  return image;
}
