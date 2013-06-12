#include "../../headers/io/io_variables.h"

using namespace std;

IO_Variables::IO_Variables() {
}

void IO_Variables::set_number_of_final_results(int result_count){
}

void IO_Variables::set_weighting(string pattern_name, int weighting){
}

void IO_Variables::set_print_results(bool should_print){
}

void IO_Variables::set_display_results(bool should_display){
}

void IO_Variables::set_verbose_print(bool should_print_verbose){
}

void IO_Variables::set_verbose_display(bool should_display_verbose){
}

void IO_Variables::set_save_print_to_file(bool should_save_print_to_file){
}

void IO_Variables::set_save_print_to_file(string filename){
}

void IO_Variables::set_print_output_filename(bool should_save_display_to_file){
}

void IO_Variables::set_load_results_from_file(bool should_load_results_from_file){
}

void IO_Variables::set_display_output_filename(string filename){
}

int IO_Variables::get_number_of_final_results() {
  int placeholder = -1;
  return placeholder;
}

int IO_Variables::get_weighting(string){
  int placeholder = -1;
  return placeholder;
}

bool IO_Variables::get_print_results(){
  bool placeholder = 0;
  return placeholder;
}

bool IO_Variables::get_display_results(){
  bool placeholder = 0;
  return placeholder;
}

bool IO_Variables::get_verbose_print(){
  bool placeholder = 0;
  return placeholder;
}

bool IO_Variables::get_verbose_display(){
  bool placeholder = 0;
  return placeholder;
}

bool IO_Variables::get_save_print_to_file(){
  bool placeholder = 0;
  return placeholder;
}

bool IO_Variables::get_save_display_to_file(){
  bool placeholder = 0;
  return placeholder;
}

bool IO_Variables::get_load_puzzle_from_file(){
  bool placeholder = 0;
  return placeholder;
}

bool IO_Variables::get_load_config_from_file(){
  bool placeholder = 0;
  return placeholder;
}

bool IO_Variables::get_load_results_from_file(){
  bool placeholder = 0;
  return placeholder;
}

string IO_Variables::get_print_output_filename(){
  string placeholder = "err_undefined";
  return placeholder;
}

string IO_Variables::get_display_output_filename(){
  string placeholder = "err_undefined";
  return placeholder;
}

string IO_Variables::get_puzzle_image_filename(){
  string placeholder = "err_undefined";
  return placeholder;
}

string IO_Variables::get_loaded_results_filename(){
  string placeholder = "err_undefined";
  return placeholder;
}

string IO_Variables::get_config_filename(){
  string placeholder = "err_undefined";
  return placeholder;
}

