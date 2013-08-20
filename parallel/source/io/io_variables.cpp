#include "whereswally/io/io_variables.h"
using namespace std;
using namespace cv;

//**
//** CLASS INITIALISATION
//**

IO_Variables::IO_Variables() {
  load_puzzle_from_file = true;
  puzzle_input_filename = "";

  number_of_final_results = 3;
  number_of_openmp_threads = 1;

  show_text_results = true;
  make_text_verbose = false;

  show_graphic_results = true;
  make_graphic_verbose = false;

  show_timing_results = true;
}

Time_Message::Time_Message(string msg, double t) {
  message = msg;
  time = t;
}

//**
//** GET/SET METHODS
//**

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

void IO_Variables::set_number_of_openmp_threads(int thread_count) {
  // must be at least 1 thread for program to execute
  if(thread_count >= 1) { 
    number_of_openmp_threads = thread_count;
  } else {
    cerr << err_msg("number_of_openmp_threads", thread_count, number_of_openmp_threads);
  }
}

int IO_Variables::get_number_of_openmp_threads() {
  return number_of_openmp_threads;
}


void IO_Variables::set_show_text_results(bool should_print){
  show_text_results = should_print;
}

bool IO_Variables::get_show_text_results(){
  return show_text_results;
}


void IO_Variables::set_show_timing_results(bool should_show){
  show_timing_results = should_show;
}

bool IO_Variables::get_show_timing_results(){
  return show_timing_results;
}

void IO_Variables::add_timing_result(string message, double time) {
  timing_results.push_back(Time_Message(message,time));
}

vector<Time_Message> IO_Variables::get_timing_results() {
  return timing_results;
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

void IO_Variables::set_load_puzzle_from_file(bool should_load_puzzle_from_file){
  load_puzzle_from_file = should_load_puzzle_from_file;
}

bool IO_Variables::get_load_puzzle_from_file(){
  return load_puzzle_from_file;
}

void IO_Variables::set_load_results_from_file(bool should_load_results_from_file){
  load_results_from_file = should_load_results_from_file;
}

bool IO_Variables::get_load_results_from_file(){
  return load_results_from_file;
}

void IO_Variables::set_puzzle_input_filename(string filename){
  fstream check_puzzle;
  check_puzzle.open(filename.c_str());
  //-- should make sure program is able to write to file
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

void IO_Variables::add_to_results(vector<Pattern_Result> new_results) {
  for(size_t i=0; i<new_results.size(); i++) {
    results.push_back(new_results[i]);
  }
}

vector<Pattern_Result> IO_Variables::get_results() {
  return results;
}



