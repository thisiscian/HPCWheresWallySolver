#include "../headers/io/error_message.h"
using namespace std;

string Error_Message::bad_number_of_final_results(va_list vl) {
  stringstream output;
  int attempt_val = va_arg(vl, int);
  int current_val = va_arg(vl, int);
  va_end(vl);
  output << "Error: attempted to set 'number_of_final_results' to ";
  output << attempt_val << ", which should be greater than or equal to zero. ";
  output << "'number_of_final_results' will remain at " << current_val << ".";
  output << endl;
  return output.str();
}

string Error_Message::bad_pattern_weighting(va_list vl) {
  stringstream output;
  char* pattern_name = va_arg(vl, char*);
  int attempt_val = va_arg(vl, int);
  int current_val = va_arg(vl, int);
  output << "Error: attempted to set 'pattern_weighting[\"";
  output << pattern_name << "\"]' to " << attempt_val;
  output << ", which should be greater than zero. ";
  output << "'pattern_weighting[\"" << pattern_name << "\"]' will remain";
  output << " at " << current_val << ".";
  output << endl;
  return output.str();
}


string Error_Message::bad_config_input_filename(va_list vl) {
  stringstream output;
  char* attempt_val = va_arg(vl, char*);
  char* current_val = va_arg(vl, char*);
  output << "Error: config input file '" << attempt_val;
  output << "' does not exist.";
  if(current_val == "") {
    output << " Continuing with default values.";
  } else {
    output << " Continuing with '" << current_val;
    output << "' as config input file.";
  }
  output << endl;
  return output.str();
}

string Error_Message::bad_puzzle_input_filename(va_list vl) {
  stringstream output;
  char* attempt_val = va_arg(vl, char*);
  char* current_val = va_arg(vl, char*);
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

string Error_Message::bad_results_input_filename(va_list vl) {
  stringstream output;
  char* attempt_val = va_arg(vl, char*);
  char* current_val = va_arg(vl, char*);
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

string Error_Message::bad_text_output_filename(va_list vl) {
  stringstream output;
  char* attempt_val = va_arg(vl, char*);
  char* new_val = va_arg(vl, char*);
  output << "Error: text output file '" << attempt_val;
  output << "' already exists.";
  output << " Saving text output as '" << new_val << "'.";
  output << endl;
  return output.str();
}

string Error_Message::bad_graphic_output_filename(va_list vl) {
  stringstream output;
  char* attempt_val = va_arg(vl, char*);
  char* new_val = va_arg(vl, char*);
  output << "Error: graphic output file '" << attempt_val;
  output << "' already exists.";
  output << " Saving graphic output as '" << new_val << "'.";
  output << endl;
  return output.str();
}

string Error_Message::bad_results_output_filename(va_list vl) {
  stringstream output;
  char* attempt_val = va_arg(vl, char*);
  char* new_val = va_arg(vl, char*);
  output << "Error: results output file '" << attempt_val;
  output << "' already exists.";
  output << " Saving results output as '" << new_val << "'.";
  output << endl;
  return output.str();
}

string Error_Message::bad_option_config(va_list vl) {
  stringstream output;
  char* form = va_arg(vl, char*);
  output << "Error: " << form << " expects a single filename as an argument\n";
  output << "\te.g. " << form << "sample_files/test_config.cfg";
  output << endl;
  return output.str();
}

string Error_Message::bad_option_number_of_final_results(va_list vl) {
  stringstream output;
  char* form = va_arg(vl, char*);
  output << "Error: " << form << " expects a single integer as an argument\n";
  output << "\te.g. " << form << "10";
  output << endl;
  return output.str();
}

string Error_Message::bad_option_pattern_weighting(va_list vl) {
  stringstream output;
  char* form = va_arg(vl, char*);
  output << "Error: " << form << " expects a comma separated list of patterns and weightings\n";
  output << "\te.g. " << form << "\"Red and Black\":2,\"Stripes\":1,\"Wally\":5";
  output << endl;
  return output.str();
}

string Error_Message::bad_option_load_puzzle(va_list vl) {
  stringstream output;
  char* form = va_arg(vl, char*);
  output << "Error: " << form << " expects a single filename as an argument\n";
  output << "\te.g. " << form << "sample_files/test_wally_puzzle.png";
  output << endl;
  return output.str();
}

string Error_Message::bad_option_load_results(va_list vl) {
  stringstream output;
  char* form = va_arg(vl, char*);
  output << "Error: " << form << " expects a single filename as an argument\n";
  output << "\te.g. " << form << "sample_files/test_results.dat";
  output << endl;
  return output.str();
}

string Error_Message::bad_option_show_text(va_list vl) {
  stringstream output;
  char* form = va_arg(vl, char*);
  output << "Error: " << form << " expects TRUE, FALSE or VERBOSE as an argument \n";
  output << "\te.g. " << form << "TRUE";
  output << endl;
  return output.str();
}

string Error_Message::bad_option_show_graphic(va_list vl) {
  stringstream output;
  char* form = va_arg(vl, char*);
  output << "Error: " << form << " expects TRUE, FALSE or VERBOSE as an argument \n";
  output << "\te.g. " << form << "TRUE";
  output << endl;
  return output.str();
}

string Error_Message::bad_option_save_text(va_list vl) {
  stringstream output;
  char* form = va_arg(vl, char*);
  output << "Error: " << form << " expects a single filename as an argument\n";
  output << "\te.g. " << form << "sample_files/test_text_output.txt";
  output << endl;
  return output.str();
}

string Error_Message::bad_option_save_graphic(va_list vl) {
  stringstream output;
  char* form = va_arg(vl, char*);
  output << "Error: " << form << " expects a single filename as an argument\n";
  output << "\te.g. " << form << "sample_files/test_graphic_output.png";
  output << endl;
  return output.str();
}

string Error_Message::bad_option_save_results(va_list vl) {
  stringstream output;
  char* form = va_arg(vl, char*);
  output << "Error: " << form << " expects a single filename as an argument\n";
  output << "\te.g. " << form << "sample_files/test_results.dat";
  output << endl;
  return output.str();
}

string Error_Message::bad_option_help(va_list vl) {
  stringstream output;
  char* prog_name = va_arg(vl, char*);
  output << "usage: " << prog_name << " [options [files]]" << endl;
  output << "\t-c,--config=CONFIG_FILE\tSets the puzzle solving configuration as described by the given file" << endl;
  output << "\t-n,--number-of-final-results=N\tSets the number of results to be output" << endl;
  output << "\t-w,--pattern-weighting=\"NAME\":WEIGHT[,\"NAME\":WEIGHT[,...]]]\tSets the weighting for each pattern" << endl;
  output << "\t-p,--load-puzzle=FILE\tAttempts to locate Wally in FILE" << endl;
  output << "\t-l,--load-results=RESULTS_FILE\tLoads results from a previous execution" << endl;
  output << "\t-Op,--show-text={TRUE,FALSE,VERBOSE}\tPrints the results to the terminal" << endl;
  output << "\t-Od,--show-graphic={TRUE,FALSE,VERBOSE}\tShows the results graphically, in a new window" << endl;
  output << "\t-St,--save-text=FILE\tSaves the textual format of the results to FILE" << endl;
  output << "\t-Si,--save-graphic=FILE\tSaves the graphical format of the results to FILE" << endl;
  output << "\t-Sr,--save-results=FILE\tSaves the results to FILE, which can be loaded later" << endl;
  output << "\t-h,--help\tDisplays this help message" << endl;
  return output.str();
}

string Error_Message::unrecognised_error(string bad_error) {
  stringstream output;
  output << "Error Error: \"" << bad_error << "\" is not a recognised error!";
  output << endl;
  return output.str();
}

string Error_Message::operator()(string key, ...) {
  va_list vl;
  if(key == "number_of_final_results") {
    va_start(vl, 2);
    string output = bad_number_of_final_results(vl);
    va_end(vl);
    return output;
  } else if(key == "pattern_weighting") {
    va_start(vl, 3);
    string output = bad_pattern_weighting(vl);
    va_end(vl);
    return output;
  } else if(key == "config_input_filename") {
    va_start(vl, 2);
    string output = bad_config_input_filename(vl);
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
  } else if(key == "text_output_filename") {
    va_start(vl, 2);
    string output = bad_text_output_filename(vl);
    va_end(vl);
    return output;
  } else if(key == "graphic_output_filename") {
    va_start(vl, 2);
    string output = bad_graphic_output_filename(vl);
    va_end(vl);
    return output;
  } else if(key == "results_output_filename") {
    va_start(vl, 2);
    string output = bad_results_output_filename(vl);
    va_end(vl);
    return output;
  } else if(key == "option_config") {
    va_start(vl, 1);
    string output = bad_option_config(vl);
    va_end(vl);
    return output;
  } else if(key == "option_number_of_final_results") {
    va_start(vl, 1);
    string output = bad_option_number_of_final_results(vl);
    va_end(vl);
    return output;
  } else if(key == "option_pattern_weighting") {
    va_start(vl, 1);
    string output = bad_option_pattern_weighting(vl);
    va_end(vl);
    return output;
  } else if(key == "option_load_puzzle") {
    va_start(vl, 1);
    string output = bad_option_load_puzzle(vl);
    va_end(vl);
    return output;
  } else if(key == "option_load_results") {
    va_start(vl, 1);
    string output = bad_option_load_results(vl);
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
  } else if(key == "option_save_text") {
    va_start(vl, 1);
    string output = bad_option_save_text(vl);
    va_end(vl);
    return output;
  } else if(key == "option_save_graphic") {
    va_start(vl, 1);
    string output = bad_option_save_graphic(vl);
    va_end(vl);
    return output;
  } else if(key == "option_save_results") {
    va_start(vl, 1);
    string output = bad_option_save_results(vl);
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



