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
  #include <sys/stat.h>
  #include <opencv2/highgui/highgui.hpp>
  #include <whereswally/framework.h>
  #include <whereswally/io/error_message.h>

  class IO_Variables {
    private:
      // config file
      bool load_config_from_file;
      std::string config_input_filename; 

      // image to be solved
      bool load_puzzle_from_file; 
      std::string puzzle_input_filename;

      // load previously created results instead of image
      bool load_results_from_file;
      std::string results_output_filename;
      std::vector<Pattern_Result> loaded_results;

      // the number of results to be shown to the user
      int number_of_final_results;

      // the user defined weighting for each search pattern
      std::map<std::string, int> pattern_weighting;

      // text output related variables
      bool show_text_results; // print results to terminal
      bool make_text_verbose; // print all produced results
      bool save_text_to_file; // print the results to a file
      std::string text_output_filename; // text results filename

      // graphical output
      bool show_graphic_results; // show results graphically
      bool make_graphic_verbose; // show all results graphically
      bool save_graphic_to_file; // save graphic results to a file
      std::string graphic_output_filename; // graphic results filename

      // save result data to file
      bool save_results_to_file;
      std::string results_input_filename;

      cv::Mat loaded_image;

      Error_Message err_msg;
    public:
      IO_Variables();

      // get&set functions for the above variables
      void set_load_config_from_file(bool should_load_config_from_file);
      bool get_load_config_from_file();
      void set_config_input_filename(std::string filename);
      std::string get_config_input_filename();

      void set_load_puzzle_from_file(bool should_load);
      bool get_load_puzzle_from_file();
      void set_puzzle_input_filename(std::string filename);
      std::string get_puzzle_input_filename();
      void set_loaded_image(cv::Mat image);
      cv::Mat get_loaded_image();

      void set_load_results_from_file(bool should_load_results_from_file);
      bool get_load_results_from_file();
      void set_results_input_filename(std::string filename);
      std::string get_results_input_filename();
      void add_to_loaded_results(std::vector<Pattern_Result>);
      std::vector<Pattern_Result> get_loaded_results();

      void set_number_of_final_results(int result_count);
      int get_number_of_final_results();

      void set_weighting(std::string pattern_name, int weighting);
      int get_weighting(std::string);

      void set_show_text_results(bool should_show);
      bool get_show_text_results();
      void set_make_text_verbose(bool should_make);
      bool get_make_text_verbose();
      void set_save_text_to_file(bool should_save);
      bool get_save_text_to_file();
      void set_text_output_filename(std::string filename);
      std::string get_text_output_filename();

      void set_show_graphic_results(bool should_show);
      bool get_show_graphic_results();
      void set_make_graphic_verbose(bool should_make);
      bool get_make_graphic_verbose();
      void set_save_graphic_to_file(bool should_save);
      bool get_save_graphic_to_file();
      void set_graphic_output_filename(std::string filename);
      std::string get_graphic_output_filename();

      void set_save_results_to_file(bool should_save);
      bool get_save_results_to_file();
      void set_results_output_filename(std::string filename);
      std::string get_results_output_filename();


      bool fileExists(std::string filename);
  };

#endif
