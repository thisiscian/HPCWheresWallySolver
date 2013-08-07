#include "whereswally/io/input.h"

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

int Input::option_config(int *num, int argc, char *argv[]) {
  string arg = argv[*num];
  string file;
  if(arg[1] == '-') {
    file = arg.substr(arg.find("=")+1);
    arg.erase(arg.find("=")+1);
  } else {
    arg.append(" ");
    if(*num+1 >= argc) {
      cerr << err_msg("option_config", arg.c_str());
      return BAD_COMMAND_USAGE; 
    }
    (*num)++;
    file = argv[*num];
  }
  if(file[0] == '-' || (argc > *num+1 && argv[*num+1][0] != '-') ) {
    cerr << err_msg("option_config", arg.c_str());
    return BAD_COMMAND_USAGE; 
  }
  variables->set_load_config_from_file(true);
  variables->set_config_input_filename(file);
return UNDEFINED;
}

int Input::option_number_of_results(int *num, int argc, char *argv[]) {
  return UNDEFINED;
}

int Input::option_pattern_weighting(int *num, int argc, char *argv[]) {
  return UNDEFINED;
}

int Input::option_load_puzzle(int *num, int argc, char *argv[]) {
  return UNDEFINED;
}

int Input::option_load_results(int *num, int argc, char *argv[]) {
  return UNDEFINED;
}

int Input::option_show_text(int *num, int argc, char *argv[]) {
  return UNDEFINED;
}

int Input::option_show_graphic(int *num, int argc, char *argv[]) {
  return UNDEFINED;
}

int Input::option_save_text(int *num, int argc, char *argv[]) {
  return UNDEFINED;
}

int Input::option_save_graphic(int *num, int argc, char *argv[]) {
  return UNDEFINED;
}

int Input::option_save_results(int *num, int argc, char *argv[]) {
  return UNDEFINED;
}

int Input::option_help(int *num, int argc, char *argv[]) {
  return UNDEFINED;
}



int Input::parse_input(int argc, char* argv[]) {
  string argv_i, file;
  if(argc == 1) {
    cerr << err_msg("option_help", argv[0]);
    return NO_ERRORS_QUIT;
  } else {
    for(int i=1; i<argc; i++) {
      argv_i = argv[i]; 
      if( argv_i == "-c" || argv_i.substr(0,9) == "--config="){
        option_config(&i, argc, argv);
      } else if(argv_i.find("--config=") != string::npos) {
        variables->set_load_config_from_file(true);
        file = argv_i.substr(argv_i.find("=")+1);
        variables->set_config_input_filename(file);
      } else if (argv_i == "-omp" ) {
        i++;
        int n = strtol(argv[i], NULL, 10);
        variables->set_number_of_openmp_threads(n);
      } else if (argv_i.find("--openmp=") != string::npos) {
        int n = strtol(argv_i.substr(argv_i.find("=")+1).c_str(),NULL,10);
        variables->set_number_of_openmp_threads(n);
      } else if(argv_i == "-n") {
        i++;
        int n = strtol (argv[i],NULL,10);
        variables->set_number_of_final_results(n);
      } else if(argv_i.find("--number-of-final-results=") != string::npos) {
        int n = strtol (argv_i.substr(argv_i.find("=")+1).c_str(),NULL,10);
        variables->set_number_of_final_results(n);
      } /*else if(argv_i == "-w") {
          int weight;
          string pattern_name, pattern_weight_str;
          i++;
          file = argv[i];
          while(file.find(",") != string::npos) {
            if(file.find(":") == string::npos || file.find(":") > file.find(",")) {
              cerr << "Error: expect weightings in the form of NAME:WEIGHT" << endl;
              return BAD_COMMAND_USAGE;
            }
            pattern_name = file.substr(1, file.find(":")-2); // parses pattern_name from string
            pattern_weight_str = file.substr(file.find(":")+1, file.find(",")-file.find(":")-1); //parses pattern_weighting from string
            weight = strtol (pattern_weight_str.c_str(),NULL,10); // converts  str to long int
            variables->set_weighting(pattern_name,weight);
            file.erase(0,file.find(",")+1); // erase 'used' data
          }
          if(file.find(":") == string::npos) {
            cerr << "Error: expect weightings in the form of NAME:WEIGHT" << endl;
            return BAD_COMMAND_USAGE;
          }
          pattern_name = file.substr(1, file.find(":")-2); // parses pattern_name from string
          pattern_weight_str = file.substr(file.find(":")+1); //parses pattern_weighting from string
          weight = strtol (pattern_weight_str.c_str(),NULL,10); // converts str to long int
          variables->set_weighting(pattern_name,weight);
      } else if(argv_i.find("--pattern-weighting=") != string::npos) {
          int weight;
          string pattern_name, pattern_weight_str;
          file = argv_i.substr(argv_i.find("=")+1);
          while(file.find(",") != string::npos) {
            if(file.find(":") == string::npos || file.find(":") > file.find(",")) {
              cerr << "Error: expect weightings in the form of NAME:WEIGHT" << endl;
              return BAD_COMMAND_USAGE;
            }
            pattern_name = file.substr(1, file.find(":")-2); // parses pattern_name from string
            pattern_weight_str = file.substr(file.find(":")+1, file.find(",")-file.find(":")-1); //parses pattern_weighting from string
            weight = strtol (pattern_weight_str.c_str(),NULL,10); // converts  str to long int
            variables->set_weighting(pattern_name,weight);
            file.erase(0,file.find(",")+1); // erase 'used' data
          }
          if(file.find(":") == string::npos) {
            cerr << "Error: expect weightings in the form of NAME:WEIGHT" << endl;
            return BAD_COMMAND_USAGE;
          }
          pattern_name = file.substr(1, file.find(":")-2); // parses pattern_name from string
          pattern_weight_str = file.substr(file.find(":")+1); //parses pattern_weighting from string
          weight = strtol (pattern_weight_str.c_str(),NULL,10); // converts string to long int
          variables->set_weighting(pattern_name,weight);
      } */ else if(argv_i == "-p") {
        i++;
        file = argv[i];
        variables->set_load_puzzle_from_file(true);
        variables->set_puzzle_input_filename(file);
      } else if(argv_i.find("--load-puzzle=") != string::npos) {
        file = argv_i.substr(argv_i.find("=")+1);
        variables->set_load_puzzle_from_file(true);
        variables->set_puzzle_input_filename(file);
      } /*else if(argv_i == "-l") {
        i++;
        file = argv[i];
        variables->set_load_results_from_file(true);
        variables->set_results_input_filename(file);
      } else if(argv_i.find("--load-results=") != string::npos) {
        file = argv_i.substr(argv_i.find("=")+1);
        variables->set_load_results_from_file(true);
        variables->set_results_input_filename(file);
      } */ else if(argv_i == "-Ot") {
        i++;
        file = argv[i];
        if(file == "VERBOSE") {
          variables->set_show_text_results(true);
          variables->set_make_text_verbose(true);
        } else if(file == "TRUE" ) {
          variables->set_show_text_results(true);
          variables->set_make_text_verbose(false);
        } else if(file == "FALSE") {
          variables->set_show_text_results(false);
          variables->set_make_text_verbose(false);
        } else {
          cerr << "Error: -Ot expects VERBOSE,TRUE or FALSE as input values" << endl;
          return BAD_COMMAND_USAGE;
        }
      } else if(argv_i.find("--print-output=") != string::npos) {
        file = argv_i.substr(argv_i.find("=")+1);
        if(file == "VERBOSE") {
          variables->set_show_text_results(true);
          variables->set_make_text_verbose(true);
        } else if(file == "TRUE" ) {
          variables->set_show_text_results(true);
          variables->set_make_text_verbose(false);
        } else if(file == "FALSE") {
          variables->set_show_text_results(false);
          variables->set_make_text_verbose(false);
        } else {
          cerr << "Error: --print-output expects VERBOSE,TRUE or FALSE as input values" << endl;
          return BAD_COMMAND_USAGE;
        }
      } else if(argv_i == "-Og") {
        i++;
        file = argv[i];
        if(file == "VERBOSE") {
          variables->set_show_graphic_results(true);
          variables->set_make_graphic_verbose(true);
        } else if(file == "TRUE" ) {
          variables->set_show_graphic_results(true);
          variables->set_make_graphic_verbose(false);
        } else if(file == "FALSE") {
          variables->set_show_graphic_results(false);
          variables->set_make_graphic_verbose(false);
        } else {
          cerr << "Error: -Og expects VERBOSE,TRUE or FALSE as input values" << endl;
          return BAD_COMMAND_USAGE;
        }
      } else if(argv_i.find("--show-graphic=") != string::npos) {
        file = argv_i.substr(argv_i.find("=")+1);
        if(file == "VERBOSE") {
          variables->set_show_graphic_results(true);
          variables->set_make_graphic_verbose(true);
        } else if(file == "TRUE" ) {
          variables->set_show_graphic_results(true);
          variables->set_make_graphic_verbose(false);
        } else if(file == "FALSE") {
          variables->set_show_graphic_results(false);
          variables->set_make_graphic_verbose(false);
        } else {
          cerr << "Error: --show-graphic expects VERBOSE,TRUE or FALSE as input values" << endl;
          return BAD_COMMAND_USAGE;
        }
      } else if(argv_i == "-Oc") {
        i++;
        file = argv[i];
        if(file == "TRUE") {
          variables->set_show_timing_results(true);
        } else if (file == "FALSE") {
          variables->set_show_timing_results(false);
        } else {
          cerr << "Error: -Oc expects TRUE or FALSE as input values" << endl;
        }
      } else if(argv_i.find("--show-timing-results=") != string::npos) {
        file = argv_i.substr(argv_i.find("=")+1);
        if(file == "TRUE") {
          variables->set_show_timing_results(true);
        } else if (file == "FALSE") {
          variables->set_show_timing_results(false);
        } else {
          cerr << "Error: -Oc expects TRUE or FALSE as input values" << endl;
        }
      } /* else if(argv_i == "-St") {
        i++;
        file = argv[i];
        variables->set_save_text_to_file(true);
        variables->set_text_output_filename(file);
      } else if(argv_i.find("--save-text-output=") != string::npos) {
        file = argv_i.substr(argv_i.find("=")+1);
        variables->set_save_text_to_file(true);
        variables->set_text_output_filename(file);
      } else if(argv_i == "-Si") {
        i++;
        file = argv[i];
        variables->set_save_graphic_to_file(true);
        variables->set_graphic_output_filename(file);
      } else if(argv_i.find("--save-image-output=") != string::npos) {
        file = argv_i.substr(argv_i.find("=")+1);
        variables->set_save_graphic_to_file(true);
        variables->set_graphic_output_filename(file);
      } */ else if(argv_i == "-h") {
          cerr << err_msg("option_help", argv[0]);
          return NO_ERRORS_QUIT;
      } else if(argv_i == "--help") {
          cerr << err_msg("option_help", argv[0]);
          return NO_ERRORS_QUIT;
      } else {
        cerr << "Error: unrecognised command \"" << argv_i << "\", exiting" << endl;
        return UNRECOGNISED_COMMAND;
      }
    }
  }
  return 0;
}

int Input::load_config() {
  string current_line;
  fstream config;
  config.open(variables->get_config_input_filename().c_str(), fstream::in);
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
      } else if (current_line.find("show_text_results=") != string::npos) {
        string val = current_line.substr(current_line.find("=")+1,string::npos);
        variables->set_show_text_results(val == "true");
      } else if (current_line.find("show_graphic_results=") != string::npos) {
        string val = current_line.substr(current_line.find("=")+1,string::npos);
        variables->set_show_graphic_results(val == "true");
      } else if (current_line.find("make_text_verbose=") != string::npos) {
        string val = current_line.substr(current_line.find("=")+1,string::npos);
        variables->set_make_text_verbose(val == "true");
      } else if (current_line.find("make_graphic_verbose=") != string::npos) {
        string val = current_line.substr(current_line.find("=")+1,string::npos);
        variables->set_make_graphic_verbose(val == "true");
      } else if (current_line.find("save_graphic_to_file=") != string::npos) {
        string val = current_line.substr(current_line.find("=")+1,string::npos);
        variables->set_save_graphic_to_file(val == "true");
      } else if (current_line.find("save_text_to_file=") != string::npos) {
        string val = current_line.substr(current_line.find("=")+1,string::npos);
        variables->set_save_text_to_file(val == "true");
      } else if (current_line.find("save_results_to_file=") != string::npos) {
        string val = current_line.substr(current_line.find("=")+1,string::npos);
        variables->set_save_results_to_file(val == "true");
      } else if (current_line.find("load_puzzle_from_file=") != string::npos) {
        string val = current_line.substr(current_line.find("=")+1,string::npos);
        variables->set_load_puzzle_from_file(val == "true");
      } else if (current_line.find("load_results_from_file=") != string::npos) {
        string val = current_line.substr(current_line.find("=")+1,string::npos);
        variables->set_load_results_from_file(val == "true");
      } else if (current_line.find("graphic_output_filename=") != string::npos) {
        string val=current_line.substr(current_line.find("=")+1,string::npos);
        variables->set_graphic_output_filename(val);
      } else if (current_line.find("text_output_filename=") != string::npos) {
        string val=current_line.substr(current_line.find("=")+1,string::npos);
        variables->set_text_output_filename(val);
      } else if (current_line.find("results_output_filename=") != string::npos) {
        string val=current_line.substr(current_line.find("=")+1,string::npos);
        variables->set_results_output_filename(val);
      } else if (current_line.find("puzzle_input_filename=") != string::npos) {
        string val=current_line.substr(current_line.find("=")+1,string::npos);
        variables->set_puzzle_input_filename(val);
      } else if (current_line.find("results_input_filename=") != string::npos) {
        string val=current_line.substr(current_line.find("=")+1,string::npos);
        variables->set_results_input_filename(val);
      } else {
        cerr << "Error: config file contained some errors" << endl;
        cerr << "\tProblem with: " << current_line << endl;
        return BAD_CONFIG_FILE;
      }
  }
  config.close(); 
  return 0;
}

int Input::load_results() {
  string current_line;
  fstream results;
  results.open(variables->get_results_input_filename().c_str(), fstream::in);
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
  variables->add_to_loaded_results(result_list);
  return 0;
}

Mat Input::load_image() {
  Mat image = imread(variables->get_puzzle_input_filename(), 1);
  variables->set_loaded_image(image);
  return image;
}
