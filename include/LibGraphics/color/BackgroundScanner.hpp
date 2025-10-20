#pragma once

#include "LibGraphics/export.hpp"

#include <vector>
#include <cstdint>

namespace LibGraphics::Color {
    class LIBGRAPHICS_API BackgroundScanner {
    public:

        static int background_color_change_up(const std::vector<std::vector<uint8_t>>& image,
                                   int start_x = 0,
                                   int start_y = 600,
                                   int max_attempts = 600,
                                   bool debug = false);

        static int background_color_change_down(const std::vector<std::vector<uint8_t>>& image,
                                         int start_x = 0,
                                         int start_y = 0,
                                         int max_attempts = 600,
                                         bool debug = false);

        static int background_color_change_left(const std::vector<std::vector<uint8_t>>& image,
                                         int start_x = 0,
                                         int start_y = 600,
                                         int max_attempts = 600,
                                         bool debug = false);

        static int background_color_change_right(const std::vector<std::vector<uint8_t>>& image,
                                          int start_x = 0,
                                          int start_y = 600,
                                          int max_attempts = 600,
                                          bool debug = false);

    };
}
