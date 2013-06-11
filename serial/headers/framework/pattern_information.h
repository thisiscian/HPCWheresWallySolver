#ifndef hpc_wally_pattern_information
#define hpc_wally_pattern_information

  #include <string>

  class Pattern_Information {
    private:
      int id;
      std::string name;
      std::string description;
      float confidence;
    public:
      void set_id(int new_id);
      void set_name(std::string new_name);
      void set_description(std::string new_description);
      void set_confidence(float new_confidence);

      std::string get_id();
      std::string get_name();
      std::string get_description();
      float get_confidence();
      
      Pattern_Information();
  };

#endif
