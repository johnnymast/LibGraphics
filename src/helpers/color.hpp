#pragma once

namespace helpers {

    class Color {
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
    };

} // namespace helpers
