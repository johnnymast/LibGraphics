#pragma once
#include "LibGraphics/export.hpp"

#include <algorithm>

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

        Rect intersect(const Rect& other) const {
            int nx = std::max(X, other.X);
            int ny = std::max(Y, other.Y);
            int nw = std::min(X + Width, other.X + other.Width) - nx;
            int nh = std::min(Y + Height, other.Y + other.Height) - ny;
            return nw > 0 && nh > 0 ? Rect{nx, ny, nw, nh} : Rect{0, 0, 0, 0};
        }
    };
}
