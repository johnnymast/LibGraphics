#pragma once

namespace LibGraphics::Match {
    class Region {
    public:
        int X = 0;
        int Y = 0;
        int Width = 0;
        int Height = 0;

        explicit Region(const int x = 0, const int y = 0, const int width = 0, const int height = 0)
            : X(x), Y(y), Width(width), Height(height) {}

        [[nodiscard]] Region Center() const;
    };
}