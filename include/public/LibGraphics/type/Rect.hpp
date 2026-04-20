#pragma once
#include "LibGraphics/export.hpp"

#include <algorithm>

using std::max;
using std::min;

namespace LibGraphics::Type {

    struct LIBGRAPHICS_API Rect {
        int X;
        int Y;
        int Width;
        int Height;

        [[nodiscard]] bool contains(int px, int py) const {
            return px >= X && px < X + Width && py >= Y && py < Y + Height;
        }

        [[nodiscard]] int area() const {
            return Width * Height;
        }

        [[nodiscard]] Rect intersect(const Rect& other) const {
            const int nx = max(X, other.X);
            const int ny = max(Y, other.Y);
            const int nw = min(X + Width, other.X + other.Width) - nx;
            const int nh = min(Y + Height, other.Y + other.Height) - ny;
            return nw > 0 && nh > 0 ? Rect{nx, ny, nw, nh} : Rect{0, 0, 0, 0};
        }
    };
}
