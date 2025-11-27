#include "../include/LibGraphics/color/Information.hpp"

#include <algorithm>

namespace LibGraphics::Color {
    bool Information::is_greenish(int r, int g, int b, int buffer) {
        // Doel: kleur #708d00 (RGB: 112, 141, 0)
        constexpr int target_r = 112;
        constexpr int target_g = 141;
        constexpr int target_b = 0;

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
