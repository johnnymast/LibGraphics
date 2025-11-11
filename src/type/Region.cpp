#include "LibGraphics/type/Region.hpp"

namespace LibGraphics::Type {
  Region Region::Center() const
  {
    const int centerX = X + Width / 2;
    const int centerY = Y + Height / 2;
    return Region(centerX, centerY, Width, Height);
  }
}