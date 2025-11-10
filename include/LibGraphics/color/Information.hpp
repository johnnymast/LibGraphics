#pragma once

#include "internals/export.hpp"

namespace LibGraphics::Color {
    class LIBGRAPHICS_API Information {
    public:
        /**
         * @brief Checks if the given RGB color is approximately green.
         *
         * @param r Red component (0–255)
         * @param g Green component (0–255)
         * @param b Blue component (0–255)
         * @param buffer Allowed deviation from target green (default: 25)
         * @return true if the color is within the green range
         */
        static bool is_greenish(int r, int g, int b, int buffer = 25);

        /**
         * @brief Checks if the given RGB color is approximately white.
         *
         * @param r Red component (0–255)
         * @param g Green component (0–255)
         * @param b Blue component (0–255)
         * @param threshold Minimum value for each component to be considered white (default: 230)
         * @return true if all RGB components are above the threshold
         */
        static bool is_white(int r, int g, int b, int threshold = 230);
    };
}
