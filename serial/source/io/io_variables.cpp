#include "whereswally/io/io_variables.h"

using namespace std;
using namespace cv;

IO_Variables::IO_Variables() {
  load_config_from_file = false;
  config_input_filename = "";

  load_puzzle_from_file = true;
  puzzle_input_filename = "";

  load_results_from_file = false;
  results_input_filename = "";

  number_of_final_results = 3;

  show_text_results = true;
  make_text_verbose = false;
  save_text_to_file = false;
  text_output_filename = "";

  show_graphic_results = true;
  make_graphic_verbose = false;
  save_graphic_to_file = false;
  graphic_output_filename = "";
}

void IO_Variables::set_number_of_final_results(int result_count){
  if(result_count > 0) {
    number_of_final_results = result_count;
  } else {
    cerr << err_msg("number_of_final_results", result_count, number_of_final_results);
  }
}

int IO_Variables::get_number_of_final_results() {
  return number_of_final_results;
}



void IO_Variables::set_weighting(string pattern_name, int weighting){
  if(weighting >=0 ) {
    pattern_weighting[pattern_name] = weighting;
  } else {
    cerr << err_msg("pattern_weighting", pattern_name.c_str(), weighting, pattern_weighting[pattern_name]);
  }
}

int IO_Variables::get_weighting(string pattern_name){
  return pattern_weighting[pattern_name];
}



void IO_Variables::set_show_text_results(bool should_print){
  show_text_results = should_print;
}

bool IO_Variables::get_show_text_results(){
  return show_text_results;
}



void IO_Variables::set_show_graphic_results(bool should_display){
  show_graphic_results = should_display;
}

bool IO_Variables::get_show_graphic_results(){
  return show_graphic_results;
}



void IO_Variables::set_make_text_verbose(bool should_print_verbose){
  make_text_verbose = should_print_verbose;
}

bool IO_Variables::get_make_text_verbose(){
  return make_text_verbose;
}



void IO_Variables::set_make_graphic_verbose(bool should_display_verbose){
  make_graphic_verbose = should_display_verbose;
}

bool IO_Variables::get_make_graphic_verbose(){
  return make_graphic_verbose;
}




void IO_Variables::set_save_text_to_file(bool should_save_text_to_file){
  save_text_to_file = should_save_text_to_file;
}

bool IO_Variables::get_save_text_to_file(){
  return save_text_to_file;
}




void IO_Variables::set_save_graphic_to_file(bool should_save_text_to_file){
  save_graphic_to_file = should_save_text_to_file;
}

bool IO_Variables::get_save_graphic_to_file(){
  return save_graphic_to_file;
}



void IO_Variables::set_load_puzzle_from_file(bool should_load_puzzle_from_file){
  load_puzzle_from_file = should_load_puzzle_from_file;
}

bool IO_Variables::get_load_puzzle_from_file(){
  return load_puzzle_from_file;
}




void IO_Variables::set_load_config_from_file(bool should_load_config_from_file){
  load_config_from_file = should_load_config_from_file;
}

bool IO_Variables::get_load_config_from_file(){
  return load_config_from_file;
}



void IO_Variables::set_load_results_from_file(bool should_load_results_from_file){
  load_results_from_file = should_load_results_from_file;
}

bool IO_Variables::get_load_results_from_file(){
  return load_results_from_file;
}



void IO_Variables::set_text_output_filename(string filename){
  string filename2 = filename;
  bool err = false;
  while( fileExists(filename2) ) {
    err = true;
      if(filename2.rfind(".") != string::npos) {
        string filetype = filename2.substr(filename2.rfind("."), string::npos);
        filename2 = filename2.substr(0, filename2.rfind("."));
        int num = strtol(filename2.substr(filename2.rfind("_")+1, string::npos).c_str(), NULL, 10);
        if(num != 0) {
          filename2.erase(filename2.rfind("_")+1,string::npos);
          stringstream numb;
          numb << num+1;
          filename2.append(numb.str());
        } else {
          filename2.append("_1");
        }
        filename2.append(filetype);
      }
  }
  if(err) {
    cerr << err_msg("text_output_filename", filename.c_str(), filename2.c_str());
  }
  text_output_filename = filename2;
}

string IO_Variables::get_text_output_filename(){
  return text_output_filename;
}



void IO_Variables::set_graphic_output_filename(string filename){
  string filename2 = filename;
  bool err = false;
  while( fileExists(filename2) ) {
    err = true;
      if(filename2.rfind(".") != string::npos) {
        string filetype = filename2.substr(filename2.rfind("."), string::npos);
        filename2 = filename2.substr(0, filename2.rfind("."));
        int num = strtol(filename2.substr(filename2.rfind("_")+1, string::npos).c_str(), NULL, 10);
        if(num != 0) {
          filename2.erase(filename2.rfind("_")+1,string::npos);
          stringstream numb;
          numb << num+1;
          filename2.append(numb.str());
        } else {
          filename2.append("_1");
        }
        filename2.append(filetype);
      }
  }
  if(err) {
    cerr << err_msg("graphic_output_filename", filename.c_str(), filename2.c_str());
  }
  graphic_output_filename = filename2;
}

string IO_Variables::get_graphic_output_filename(){
  return graphic_output_filename;
}



void IO_Variables::set_puzzle_input_filename(string filename){
  fstream check_puzzle;
  check_puzzle.open(filename.c_str());
  if(check_puzzle.good() || filename == "") {
    puzzle_input_filename = filename;
  } else {
   cerr << err_msg("puzzle_input_filename", filename.c_str(), puzzle_input_filename.c_str());
  }
  check_puzzle.close();
}

string IO_Variables::get_puzzle_input_filename(){
  return puzzle_input_filename;
}

void IO_Variables::set_loaded_image(Mat image) { 
  loaded_image = image;
}

Mat IO_Variables::get_loaded_image() {
  Mat clone = loaded_image.clone();
  return loaded_image.clone();
}



void IO_Variables::set_config_input_filename(string filename){
  fstream check_config;
  check_config.open(filename.c_str());
  if(check_config.good() || filename == "") {
    config_input_filename = filename;
  } else {
   cerr << err_msg("config_input_filename", filename.c_str(), config_input_filename.c_str()); 
  }
  check_config.close();
}

string IO_Variables::get_config_input_filename(){
  return config_input_filename;
}


void IO_Variables::set_results_input_filename(string filename){
  fstream check_load_results;
  check_load_results.open(filename.c_str());
  if(check_load_results.good() || filename == "") {
    results_input_filename = filename;
  } else {
   cerr << err_msg("results_input_filename", filename.c_str(), results_input_filename.c_str());
  }
  check_load_results.close();
}

string IO_Variables::get_results_input_filename(){
  return results_input_filename;
}

void IO_Variables::add_to_loaded_results(vector<Pattern_Result> new_results) {
  for(int i=0; i<new_results.size(); i++) {
    loaded_results.push_back(new_results[i]);
  }
}

vector<Pattern_Result> IO_Variables::get_loaded_results() {
  return loaded_results;
}


void IO_Variables::set_save_results_to_file(bool should_save) {
  save_results_to_file = should_save;
}

bool IO_Variables::get_save_results_to_file() {
  return save_results_to_file;
}

void IO_Variables::set_results_output_filename(std::string filename) {
  string filename2 = filename;
  bool err = false;
  while( fileExists(filename2) ) {
    err = true;
      if(filename2.rfind(".") != string::npos) {
        string filetype = filename2.substr(filename2.rfind("."), string::npos);
        filename2 = filename2.substr(0, filename2.rfind("."));
        int num = strtol(filename2.substr(filename2.rfind("_")+1, string::npos).c_str(), NULL, 10);
        if(num != 0) {
          filename2.erase(filename2.rfind("_")+1,string::npos);
          stringstream numb;
          numb << num+1;
          filename2.append(numb.str());
        } else {
          filename2.append("_1");
        }
        filename2.append(filetype);
      }
  }
  if(err) {
    cerr << err_msg("results_output_filename", filename.c_str(), filename2.c_str());
  }
  results_output_filename = filename2;
}

std::string IO_Variables::get_results_output_filename() {
  return results_output_filename;
}

bool IO_Variables::fileExists(std::string filename)
{
    struct stat buf;
    if (stat(filename.c_str(), &buf) != -1)
    {
        return true;
    }
    return false;
}
