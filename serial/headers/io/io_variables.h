/*
 *    Part of the serial 'Where's Wally Solver' program.
 *
 *    Defines the IO_Variables class, used for storing data used in I/O
 *    Also enables data transfer between the Input class (input.h) and the Output class (output.h) 
 *
 *    cian booth - this.is.cian@gmail.com
 *
 */

#ifndef hpc_whereswally_serial_io_variables
#define hpc_whereswally_serial_io_variables

  #include <iostream>
  #include <string>
  #include <map>
  #include <fstream>
  #include "../framework/search_pattern.h"

  class IO_Variables {
    private:
      // the number of results to be shown to the user
      int number_of_final_results;
  
      // the user defined weighting of each pattern, for use when judging 'best' algorithms
      std::map<std::string, int> pattern_weighting;

      // should print results to terminal. If verbose, print all results
      bool print_results;
      bool verbose_print;

      // should display results graphically. If verbose, display all results
      bool display_results;
      bool verbose_display;

      // should save 'print' results to a file
      bool save_print_to_file;
      std::string print_output_filename;

      // should save 'display' results to a file
      bool save_display_to_file;
      std::string display_output_filename;

      // should load the puzzle to be solved from a file
      bool load_puzzle_from_file; 
      std::string puzzle_image_filename;

      // should load the configuration of IO_Variables from a file
      bool load_config_from_file; 
      std::string config_filename;

      // should load the results from a previously saved file
      bool load_results_from_file;
      std::string load_results_filename;
      std::vector<Pattern_Result> load_results;
    public:
      IO_Variables();

      void set_number_of_final_results(int result_count);
      int get_number_of_final_results();

      void set_weighting(std::string pattern_name, int weighting);
      int get_weighting(std::string);

      void set_print_results(bool should_print);
      bool get_print_results();

      void set_display_results(bool should_display);
      bool get_display_results();

      void set_verbose_print(bool should_print_verbose);
      bool get_verbose_print();

      void set_verbose_display(bool should_display_verbose);
      bool get_verbose_display();

      void set_save_print_to_file(bool should_save_print_to_file);
      bool get_save_print_to_file();

      void set_save_display_to_file(bool should_save_print_to_file);
      bool get_save_display_to_file();

      void set_load_puzzle_from_file(bool should_load_puzzle_from_file);
      bool get_load_puzzle_from_file();

      void set_load_config_from_file(bool should_load_config_from_file);
      bool get_load_config_from_file();

      void set_load_results_from_file(bool should_load_results_from_file);
      bool get_load_results_from_file();

      void set_print_output_filename(std::string filename);
      std::string get_print_output_filename();

      void set_display_output_filename(std::string filename);
      std::string get_display_output_filename();

      void set_puzzle_image_filename(std::string filename);
      std::string get_puzzle_image_filename();

      void set_config_filename(std::string filename);
      std::string get_config_filename();

      void set_load_results_filename(std::string filename);
      std::string get_load_results_filename();
      void add_to_load_results(std::vector<Pattern_Result>);
      std::vector<Pattern_Result> get_load_results();
  };

#endif
