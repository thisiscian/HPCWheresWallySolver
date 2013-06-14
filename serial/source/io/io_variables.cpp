#include "../../headers/io/io_variables.h"
using namespace std;

IO_Variables::IO_Variables() {
  number_of_final_results = 3;
  print_results = true;
  display_results = true;
  verbose_print = false;
  verbose_display = false;
  save_print_to_file = false;
  save_display_to_file = false;
  load_puzzle_from_file = true;
  load_config_from_file = false;
  load_results_from_file = false;
  print_output_filename = "";
  display_output_filename = "";
  puzzle_image_filename = "";
  config_filename = "";
  load_results_filename = "";
  
}

void IO_Variables::set_number_of_final_results(int result_count){
  if(result_count > 0) {
    number_of_final_results = result_count;
  } else {
    cerr << "Error: attempted to set 'number_of_final_results' to a value less than or equal to 0. Continuing with previous value" << endl;
  }
}

int IO_Variables::get_number_of_final_results() {
  return number_of_final_results;
}



void IO_Variables::set_weighting(string pattern_name, int weighting){
  if(weighting >=0 ) {
    pattern_weighting[pattern_name] = weighting;
  } else {
    cerr << "Error: attempted to set a 'pattern_weighting' to a value less than 0. Ignoring addition." << endl;
  }
}

int IO_Variables::get_weighting(string pattern_name){
  return pattern_weighting[pattern_name];
}



void IO_Variables::set_print_results(bool should_print){
  print_results = should_print;
}

bool IO_Variables::get_print_results(){
  return print_results;
}



void IO_Variables::set_display_results(bool should_display){
  display_results = should_display;
}

bool IO_Variables::get_display_results(){
  return display_results;
}



void IO_Variables::set_verbose_print(bool should_print_verbose){
  verbose_print = should_print_verbose;
}

bool IO_Variables::get_verbose_print(){
  return verbose_print;
}



void IO_Variables::set_verbose_display(bool should_display_verbose){
  verbose_display = should_display_verbose;
}

bool IO_Variables::get_verbose_display(){
  return verbose_display;
}




void IO_Variables::set_save_print_to_file(bool should_save_print_to_file){
  save_print_to_file = should_save_print_to_file;
}

bool IO_Variables::get_save_print_to_file(){
  return save_print_to_file;
}




void IO_Variables::set_save_display_to_file(bool should_save_print_to_file){
  save_display_to_file = should_save_print_to_file;
}

bool IO_Variables::get_save_display_to_file(){
  return save_display_to_file;
}



void IO_Variables::set_load_puzzle_from_file(bool should_load_puzzle_from_file){
  if(load_puzzle_from_file != should_load_puzzle_from_file) { 
    if(should_load_puzzle_from_file == true ) {
      cerr << "\t Setting 'load_results_from_file' to false and 'load_puzzle_from_file' to true." << endl;
    } else { 
      cerr << "\t Setting 'load_results_from_file' to true and 'load_puzzle_from_file' to false." << endl;
    }
  }
  load_puzzle_from_file = should_load_puzzle_from_file;
  load_results_from_file = !should_load_puzzle_from_file;
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
  if(load_results_from_file != should_load_results_from_file) { 
    if(should_load_results_from_file == true ) {
      cerr << "\t Setting 'load_results_from_file' to true and 'load_puzzle_from_file' to false." << endl;
    } else { 
      cerr << "\t Setting 'load_results_from_file' to false and 'load_puzzle_from_file' to true." << endl;
    }
  }
  load_results_from_file = should_load_results_from_file;
  load_puzzle_from_file = !should_load_results_from_file;
}

bool IO_Variables::get_load_results_from_file(){
  return load_results_from_file;
}



void IO_Variables::set_print_output_filename(string filename){
  print_output_filename = filename;
}

string IO_Variables::get_print_output_filename(){
  return print_output_filename;
}



void IO_Variables::set_display_output_filename(string filename){
  display_output_filename = filename;
}

string IO_Variables::get_display_output_filename(){
  return display_output_filename;
}



void IO_Variables::set_puzzle_image_filename(string filename){
  fstream check_puzzle;
  check_puzzle.open(filename.c_str());
  if(check_puzzle.good() || filename == "") {
    puzzle_image_filename = filename;
  } else {
   cerr << "Error: attempted to set 'puzzle_image_filename' to a file that does not exist. Continuing with previous value." << endl; 
  }
  check_puzzle.close();
}

string IO_Variables::get_puzzle_image_filename(){
  return puzzle_image_filename;
}



void IO_Variables::set_config_filename(string filename){
  fstream check_config;
  check_config.open(filename.c_str());
  if(check_config.good() || filename == "") {
    config_filename = filename;
  } else {
   cerr << "Error: attempted to set 'config_filename' to a file that does not exist. Continuing with previous value." << endl; 
  }
  check_config.close();
}

string IO_Variables::get_config_filename(){
  return config_filename;
}


void IO_Variables::set_load_results_filename(string filename){
  fstream check_load_results;
  check_load_results.open(filename.c_str());
  if(check_load_results.good() || filename == "") {
    load_results_filename = filename;
  } else {
   cerr << "Error: attempted to set 'load_results_filename' to a file that does not exist. Continuing with previous value." << endl; 
  }
  check_load_results.close();
}

string IO_Variables::get_load_results_filename(){
  return load_results_filename;
}

void IO_Variables::add_to_load_results(vector<Pattern_Result> new_results) {
  for(int i=0; i<new_results.size(); i++) {
    load_results.push_back(new_results[i]);
  }
}

vector<Pattern_Result> IO_Variables::get_load_results() {
  return load_results;
}

