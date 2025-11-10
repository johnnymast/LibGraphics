#include "../include/LibGraphics/color/Information.hpp"

#include <algorithm>

namespace LibGraphics::Color {
    bool Information::is_greenish(int r, int g, int b, int buffer) {
        constexpr int target_r = 71;
        constexpr int target_g = 145;
        constexpr int target_b = 114;

        return (r >= target_r - buffer && r <= target_r + buffer) &&
               (g >= target_g - buffer && g <= target_g + buffer) &&
               (b >= target_b - buffer && b <= target_b + buffer);
    }

    bool Information::is_white(int r, int g, int b, int threshold) {
        int brightness    = (r + g + b) / 3;   // = 229
        int min_component = std::min({r, g, b}); // = 229
        int max_component = std::max({r, g, b}); // = 229
        int chroma_diff   = max_component - min_component; // = 0

        return brightness >= threshold &&
               min_component >= threshold &&
               chroma_diff <= 10;
    }

}
