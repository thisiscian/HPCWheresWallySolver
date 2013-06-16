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
  } else {
    string output = unrecognised_error(key);
    return output;
  }
}

