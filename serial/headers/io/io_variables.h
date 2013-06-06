#ifndef hpc_wally_io_variables
#define hpc_wally_io_variables

class IO_Variables {
  private:
    int number_of_final_results;
    map<std::string, int> pattern_weighting;
    bool print_results, display_results;
    bool verbose_print, verbose_display;
    bool save_print_to_file, save_display_to_file;
    bool load_puzzle_from_file, load_config_from_file, load_results_from_file;
    std::string print_output_filename, display_output_filename;
    std::string puzzle_image_filename, loaded_results_filename, config_filename;
  public:
    void set_number_of_final_results(int result_count);
    void set_weighting(std::string pattern_name, int weighting);
    void set_print_results(bool should_print);
    void set_display_results(bool should_display);
    void set_verbose_print(bool should_print_verbose);
    void set_verbose_display(bool should_display_verbose);
    void set_save_print_to_file(bool should_save_print_to_file);
    void set_save_print_to_file(std::string filename);
    void set_print_output_filename(bool should_save_display_to_file);
    void set_load_results_from_file(bool should_load_results_from_file);
    void set_display_output_filename(std::string filename);

    int get_number_of_final_results();
    int get_weighting(std::string);
    bool get_print_results(), display_results();
    bool get_verbose_print(), get_verbose_display();
    bool get_save_print_to_file(), get_save_display_to_file();
    bool get_load_puzzle_from_file(), get_load_config_from_file(), get_load_results_from_file();
    std::string get_print_output_filename(), get_display_output_filename();
    std::string get_puzzle_image_filename(), get_loaded_results_filename(), get_config_filename();

    IO_Variables();
};

#endif
