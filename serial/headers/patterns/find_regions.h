#ifndef hpc_whereswally_serial_find_regions
#define hpc_whereswally_serial_find_regions

  #include <vector>
  #include <climits>
  #include <opencv2/core/core.hpp>

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

  std::vector<region> find_regions_from_mask(cv::Mat input);

#endif
