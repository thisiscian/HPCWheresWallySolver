#include "../../headers/io/input.h"

using namespace std;
using namespace cv;

int to_lower_case(int c) {
  return tolower(c);
}

string remove_whitespace(string in_string) {
  for(int i=0; i<in_string.size(); i++) {
    if(isspace(in_string[i])) {
      in_string.erase(i,1); 
      i--;
    }
  }
  return in_string;
}

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
  string current_line;
  fstream config;
  config.open(variables->get_config_filename().c_str(), fstream::in);
  while(config.good()) {
    getline(config, current_line);
      if(current_line.find("#") != string::npos) {
        current_line.erase(current_line.find("#"), string::npos); // ignores anything after a #
      }
      transform(current_line.begin(), current_line.end(), current_line.begin(), to_lower_case); //converts string to lower case
      current_line = remove_whitespace(current_line);
      if(current_line.size() == 0 ) { 
        continue; 
      } else if (current_line.find("number_of_final_results=") != string::npos) {
        int val;
        stringstream string_val;

        string_val.str(current_line.substr(current_line.find("=")+1,string::npos));
        string_val >> val;
        variables->set_number_of_final_results(val);
      } else if (current_line.find("pattern_weighting{") != string::npos) {
        getline(config, current_line);
        while(current_line.find("}") == string::npos) {
          int val;
          stringstream string_val;
          if(config.bad()) {
            cerr << "Error: config file contained some errors" << endl;
            return BAD_CONFIG_FILE;
          }
          size_t first_quote = current_line.find('"');
          size_t second_quote = current_line.find('"',first_quote+1);
          if(first_quote == string::npos || second_quote == string::npos) {
            cerr << "Error: config file contained some errors" << endl;
            return BAD_CONFIG_FILE;
          }
          string pattern_name = current_line.substr(first_quote+1, second_quote-first_quote-1);
          transform(current_line.begin(), current_line.end(), current_line.begin(), to_lower_case); //converts string to lower case
          current_line = remove_whitespace(current_line);
          string_val.str(current_line.substr(current_line.find("=")+1,string::npos));
          string_val >> val;
          variables->set_weighting(pattern_name, val);

          getline(config, current_line);
          transform(current_line.begin(), current_line.end(), current_line.begin(), to_lower_case); //converts string to lower case
          current_line = remove_whitespace(current_line);
        }
      } else if (current_line.find("print_results=") != string::npos) {
        string val = current_line.substr(current_line.find("=")+1,string::npos);
        variables->set_print_results(val == "true");
      } else if (current_line.find("display_results=") != string::npos) {
        string val = current_line.substr(current_line.find("=")+1,string::npos);
        variables->set_display_results(val == "true");
      } else if (current_line.find("verbose_print=") != string::npos) {
        string val = current_line.substr(current_line.find("=")+1,string::npos);
        variables->set_verbose_print(val == "true");
      } else if (current_line.find("verbose_display=") != string::npos) {
        string val = current_line.substr(current_line.find("=")+1,string::npos);
        variables->set_verbose_display(val == "true");
      } else if (current_line.find("save_display_to_file=") != string::npos) {
        string val = current_line.substr(current_line.find("=")+1,string::npos);
        variables->set_save_display_to_file(val == "true");
      } else if (current_line.find("save_print_to_file=") != string::npos) {
        string val = current_line.substr(current_line.find("=")+1,string::npos);
        variables->set_save_print_to_file(val == "true");
      } else if (current_line.find("load_puzzle_from_file=") != string::npos) {
        string val = current_line.substr(current_line.find("=")+1,string::npos);
        variables->set_load_puzzle_from_file(val == "true");
      } else if (current_line.find("load_results_from_file=") != string::npos) {
        string val = current_line.substr(current_line.find("=")+1,string::npos);
        variables->set_load_results_from_file(val == "true");
      } else if (current_line.find("display_output_filename=") != string::npos) {
        string val=current_line.substr(current_line.find("=")+1,string::npos);
        variables->set_display_output_filename(val);
      } else if (current_line.find("print_output_filename=") != string::npos) {
        string val=current_line.substr(current_line.find("=")+1,string::npos);
        variables->set_print_output_filename(val);
      } else if (current_line.find("puzzle_image_filename=") != string::npos) {
        string val=current_line.substr(current_line.find("=")+1,string::npos);
        variables->set_puzzle_image_filename(val);
      } else if (current_line.find("load_results_filename=") != string::npos) {
        string val=current_line.substr(current_line.find("=")+1,string::npos);
        variables->set_load_results_filename(val);
      } else {
        cerr << "Error: config file contained some errors" << endl;
        cerr << current_line << endl;
        return BAD_CONFIG_FILE;
      }
  }
  config.close(); 
  return 0;
}

int Input::load_results() {
  string current_line;
  fstream results;
  results.open(variables->get_load_results_filename().c_str(), fstream::in);
  vector<Pattern_Result> result_list;
  while(results.good()) {
    getline(results, current_line);
    if(current_line == "") continue;
    Pattern_Result this_result;
    stringstream result_data;

    string name, description;
    float confidence;

    size_t first_quote = current_line.find('"')+1;
    size_t second_quote = current_line.find('"',first_quote);
    size_t third_quote = current_line.find('"', second_quote+1)+1;
    size_t fourth_quote = current_line.find('"',third_quote);
    if(first_quote == 0 || second_quote == 0 || third_quote == 0 || fourth_quote == 0) {
      cerr << "Error: results file contained some errors" << endl;
      return BAD_RESULTS_FILE;
    }

    name = current_line.substr(first_quote,second_quote-first_quote);
    description = current_line.substr(third_quote,fourth_quote-third_quote);
    current_line = current_line.substr(fourth_quote+1, string::npos);
    result_data.str(current_line);
    
    if(result_data.bad()){
      cerr << "Error: results file contained some errors" << endl;
      return BAD_RESULTS_FILE;
    }
    result_data >> confidence;
    if(result_data.bad()){
      cerr << "Error: results file contained some errors" << endl;
      return BAD_RESULTS_FILE;
    }
    result_data >> this_result.wally_location[0];
    if(result_data.bad()){
      cerr << "Error: results file contained some errors" << endl;
      return BAD_RESULTS_FILE;
    }
    result_data >> this_result.wally_location[1];
    if(result_data.bad()){
      cerr << "Error: results file contained some errors" << endl;
      return BAD_RESULTS_FILE;
    }
    result_data >> this_result.scale[0];
    if(result_data.bad()){
      cerr << "Error: results file contained some errors" << endl;
      return BAD_RESULTS_FILE;
    }
    result_data >> this_result.scale[1];
    if(result_data.bad()){
      cerr << "Error: results file contained some errors" << endl;
      return BAD_RESULTS_FILE;
    }
    result_data >> this_result.certainty;
    this_result.info.set_name(name);
    this_result.info.set_description(description);
    this_result.info.set_confidence(confidence);

    result_list.push_back(this_result);
  }
  variables->add_to_load_results(result_list);
  return 0;
}

Mat Input::load_image() {
  Mat placeholder;
  return placeholder;
}
