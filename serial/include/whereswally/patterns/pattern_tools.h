#ifndef hpc_whereswally_serial_pattern_tools
#define hpc_whereswally_serial_pattern_tools

  #include <vector>
  #include <climits>
  #include <opencv2/core/core.hpp>

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

    // from a binary (0,255) single channel Mat, find the distinct regions with values of 255
    std::vector<region> find_regions_from_mask(cv::Mat input);

    // overlay two binary single channel Mats, taking only their intersection
    std::vector<region> region_overlay_AND(cv::Mat left, cv::Mat right);

    // overlay two binary single channel Mats, taking their union
    std::vector<region> region_overlay_OR(cv::Mat left, cv::Mat right);
  }

#endif
