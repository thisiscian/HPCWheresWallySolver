#ifndef hpc_whereswally_serial_pattern_tools
#define hpc_whereswally_serial_pattern_tools

  #include <vector>
  #include <climits>
  #include <iostream>
  #include <sstream>
  #include <opencv2/core/core.hpp>
  #include <opencv2/imgproc/imgproc.hpp> 

  // wwp - where's wally patterns, small name for ease of use
  namespace wwp {
    // class containing information about distinct regions
    class region {
      public:
        int size;
        float av_x;
        float av_y;
        int smallest_x;
        int smallest_y;
        int largest_x;
        int largest_y;
        region();
    };
    bool operator==(region lhs, region rhs);
  
    class homography {
      public:
      float a;
      int b;
      float c;
      int d;
    };
    bool operator<(homography lhs, homography rhs);

    void print_output(std::string message, int thread_num, int num_threads, std::string pattern_name);

    // from a binary (0,255) single channel Mat, find the distinct regions with values of 255
    std::vector<region> fast_find_regions(cv::Mat input);
    std::vector<region> find_regions_from_mask(cv::Mat input);

    // from a colour input image, estimate thickness
    double estimate_black_line_thickness(cv::Mat input, int limit, int tolerance);

    // find all instances of a colour between two #rrggbb strings, and some given ratio
    cv::Mat get_colour_in_image(cv::Mat image, std::string colour_one, std::string colour_two, float redgreen, float greenred, float redblue, float bluered, float greenblue, float bluegreen);

    // find all instances of greyscale in image, with a tolerance
    cv::Mat get_greyscale_in_image(cv::Mat image, int low, int high, int tolerance);

// UNIMPLEMENTED
    // overlay two binary single channel Mats, taking only their intersection
    std::vector<region> region_overlay_AND(cv::Mat left, cv::Mat right);

    // overlay two binary single channel Mats, taking their union
    std::vector<region> region_overlay_OR(cv::Mat left, cv::Mat right);

  }

#endif
