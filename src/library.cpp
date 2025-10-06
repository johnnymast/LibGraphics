#include "library.hpp"
#include "export.hpp"
#include <iostream>

LIBGRAPHICS_API void hello() {
    std::cout << "Hello from LibGraphics!" << std::endl;
}
