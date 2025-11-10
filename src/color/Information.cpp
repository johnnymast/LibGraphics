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
        int brightness = (r + g + b) / 3;
        int min_component = std::min({r, g, b});
        // Strenger: elk kanaal moet in de buurt van wit zijn
        return brightness >= threshold && min_component >= threshold;
    }

}
