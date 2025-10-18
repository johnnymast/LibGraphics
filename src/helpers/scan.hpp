#pragma once
#include <vector>
#include <cstdint>

namespace helpers {

    int scan_up(const std::vector<std::vector<uint8_t>>& image,
                int start_x = 0,
                int start_y = 600,
                int steps = 600,
                bool debug = false);

    int scan_down(const std::vector<std::vector<uint8_t>>& image,
                  int start_x = 0,
                  int start_y = 0,
                  int steps = 600,
                  bool debug = false);

    int scan_left(const std::vector<std::vector<uint8_t>>& image,
                  int start_x = 0,
                  int start_y = 600,
                  int steps = 600,
                  bool debug = false);

    int scan_right(const std::vector<std::vector<uint8_t>>& image,
                   int start_x = 0,
                   int start_y = 600,
                   int steps = 600,
                   bool debug = false);

} // namespace helpers

