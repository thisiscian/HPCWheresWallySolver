#ifndef hpc_whereswally_serial_pattern_information
#define hpc_whereswally_serial_pattern_information

  #include <string>
  #include <iostream>

  class Pattern_Information {
    private:
      std::string name;
      std::string description;
      float confidence;
    public:
      Pattern_Information();

      void set_name(std::string new_name);
      std::string get_name() const;

      void set_description(std::string new_description);
      std::string get_description() const;

      void set_confidence(float new_confidence);
      float get_confidence() const;

      const bool operator==(const Pattern_Information &rhs);
      const bool operator!=(const Pattern_Information &rhs);
  };


#endif
