#include "whereswally/io/error_message.h"
using namespace std;

//-- Print error for entering bad numbers for -n option
string Error_Message::bad_number_of_final_results(va_list vl) {
  stringstream output;
  int attempt_val = va_arg(vl, int);
  int current_val = va_arg(vl, int);
  va_end(vl);
  output << "Error: attempted to set 'number_of_final_results' to ";
  output << attempt_val << ", which should be greater than zero. ";
  output << "'number_of_final_results' will remain at " << current_val << ".";
  output << endl;
  return output.str();
}

//-- Print error for entering bad number of -omp option
string Error_Message::bad_number_of_openmp_threads(va_list vl) {
  stringstream output;
  int attempt_val = va_arg(vl, int);
  int current_val = va_arg(vl, int);
  va_end(vl);
  output << "Error: attempted to set 'number_of_openmp_threads' to ";
  output << attempt_val << ", which should be greater than or equal to one. ";
  output << "'number_of_openmp_threads' will remain at " << current_val << ".";
  output << endl;
  return output.str();
}

//-- Error message for bad -p option
string Error_Message::bad_puzzle_input_filename(va_list vl) {
  stringstream output;
  string attempt_val = va_arg(vl, char*);
  string current_val = va_arg(vl, char*);
  output << "Error: puzzle input file '" << attempt_val;
  output << "' does not exist.";
  if(current_val == "") {
    output << " Currently, no puzzle will be solved.";
  } else {
    output << " Continuing with '" << current_val;
    output << "' as puzzle input file.";
  }
  output << endl;
  return output.str();
}

//-- Error message for bad results addition
string Error_Message::bad_results_input_filename(va_list vl) {
  stringstream output;
  string attempt_val = va_arg(vl, char*);
  string current_val = va_arg(vl, char*);
  output << "Error: results input file '" << attempt_val;
  output << "' does not exist.";
  if(current_val == "") {
    output << " Currently, no previous results will be loaded.";
  } else {
    output << " Continuing with '" << current_val;
    output << "' as results input file.";
  }
  output << endl;
  return output.str();
}

//-- Error Message for a bad number of final results
string Error_Message::bad_option_number_of_final_results(va_list vl) {
  stringstream output;
  string form = va_arg(vl, char*);
  output << "Error: " << form << " expects a single integer as an argument\n";
  output << "\te.g. " << form << "10";
  output << endl;
  return output.str();
}

//-- Error message for a band number of omp threads
string Error_Message::bad_option_number_of_openmp_threads(va_list vl) {
  stringstream output;
  string form = va_arg(vl, char*);
  output << "Error: " << form << " expects a single integer as an argument\n";
  output << "\te.g. " << form << "10";
  output << endl;
  return output.str();
}
//-- Error Message for a bad call to load a puzzle
string Error_Message::bad_option_load_puzzle(va_list vl) {
  stringstream output;
  string form = va_arg(vl, char*);
  output << "Error: " << form << " expects a single filename as an argument\n";
  output << "\te.g. " << form << "example_input.png";
  output << endl;
  return output.str();
}

//-- Error Message for a bad call to show text output
string Error_Message::bad_option_show_text(va_list vl) {
  stringstream output;
  string form = va_arg(vl, char*);
  output << "Error: " << form << " expects TRUE, FALSE or VERBOSE as an argument \n";
  output << "\te.g. " << form << "TRUE";
  output << endl;
  return output.str();
}

//-- Error message for a bad call to show graphic output
string Error_Message::bad_option_show_graphic(va_list vl) {
  stringstream output;
  string form = va_arg(vl, char*);
  output << "Error: " << form << " expects TRUE, FALSE or VERBOSE as an argument \n";
  output << "\te.g. " << form << "TRUE";
  output << endl;
  return output.str();
}
//-- Error message for a bad call to show timing
string Error_Message::bad_option_show_timing(va_list vl) {
  stringstream output;
  string form = va_arg(vl, char*);
  output << "Error: " << form << " expects TRUE or FALSE as an argument \n";
  output << "\te.g. " << form << "TRUE";
  output << endl;
  return output.str();
}

//-- Print help message
string Error_Message::bad_option_help(va_list vl) {
  stringstream output;
  string prog_name = va_arg(vl, char*);
  output << "usage: " << prog_name << " [options [files]]" << endl;
  output << "\t-omp,--openmp=N\tSets the number of threads that OpenMP can use" << endl;
  output << "\t-n,--number-of-final-results=N\tSets the number of results to be output" << endl;
  output << "\t-p,--load-puzzle=FILE\tAttempts to locate Wally in FILE" << endl;
  output << "\t-Oc,--show-text={TRUE,FALSE,VERBOSE}\tPrints the results to the terminal" << endl;
  output << "\t-Og,--show-graphic={TRUE,FALSE,VERBOSE}\tShows the results graphically, in a new window" << endl;
  output << "\t-Ot,--show-timing={TRUE,FALSE}\tPrints the timing results to the terminal" << endl;
  output << "\t-h,--help\tDisplays this help message" << endl;
  return output.str();
}

//-- Error for bad error messages
string Error_Message::unrecognised_error(string bad_error) {
  stringstream output;
  output << "Error Error: \"" << bad_error << "\" is not a recognised error!";
  output << endl;
  return output.str();
}

//-- Given specific inputs, print error message
string Error_Message::operator()(string key, ...) {
  va_list vl;
  if(key == "number_of_final_results") {
    va_start(vl, 2);
    string output = bad_number_of_final_results(vl);
    va_end(vl);
    return output;
  } if(key == "number_of_openmp_threads") {
    va_start(vl, 2);
    string output = bad_number_of_openmp_threads(vl);
    va_end(vl);
    return output;
  } else if(key == "puzzle_input_filename") {
    va_start(vl, 2);
    string output = bad_puzzle_input_filename(vl);
    va_end(vl);
    return output;
  } else if(key == "results_input_filename") {
    va_start(vl, 2);
    string output = bad_results_input_filename(vl);
    va_end(vl);
    return output;
  } else if(key == "option_number_of_final_results") {
    va_start(vl, 1);
    string output = bad_option_number_of_final_results(vl);
    va_end(vl);
    return output;
  } if(key == "option_number_of_openmp_threads") {
    va_start(vl, 2);
    string output = bad_option_number_of_openmp_threads(vl);
    va_end(vl);
    return output;
  } else if(key == "option_load_puzzle") {
    va_start(vl, 1);
    string output = bad_option_load_puzzle(vl);
    va_end(vl);
    return output;
  } else if(key == "option_show_text") {
    va_start(vl, 1);
    string output = bad_option_show_text(vl);
    va_end(vl);
    return output;
  } else if(key == "option_show_graphic") {
    va_start(vl, 1);
    string output = bad_option_show_graphic(vl);
    va_end(vl);
    return output;
  } else if(key == "option_show_timing") {
    va_start(vl, 1);
    string output = bad_option_show_timing(vl);
    va_end(vl);
    return output;
  } else if(key == "option_help") {
    va_start(vl, 1);
    string output = bad_option_help(vl);
    va_end(vl);
    return output;
  } else {
    string output = unrecognised_error(key);
    return output;
  }
}



