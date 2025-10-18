#include "scan.hpp"

#include <thread>
#include <iostream>

namespace helpers {
    int scan_up(const std::vector<std::vector<uint8_t> > &image,
                int start_x, int start_y, int steps, bool debug) {
        const auto empty = std::make_tuple(0, 0, 0);
        auto last = empty;
        int y = start_y;
        int step = 0;

        for (int attempt = 0; attempt < steps; ++attempt) {
            y -= 1;
            step += 1;

            if (debug) {
                std::cerr << "scan_up x: " << start_x << " y: " << y << "\n";
            }

            auto ret = std::make_tuple(image[y][start_x], image[y][start_x], image[y][start_x]);

            if (last != empty && ret != last) {
                return step;
            }

            last = ret;
        }

        return -1;
    }

    int scan_down(const std::vector<std::vector<uint8_t> > &image,
                  int start_x, int start_y, int steps, bool debug) {
        const auto empty = std::make_tuple(0, 0, 0);
        auto last = empty;
        int y = start_y;

        for (int attempt = 0; attempt < steps; ++attempt) {
            if (debug) {
                std::cerr << "scan_down x: " << start_x << " y: " << y << "\n";
            }

            auto ret = std::make_tuple(image[y][start_x], image[y][start_x], image[y][start_x]);

            if (last != empty && ret != last) {
                return y - 1;
            }

            last = ret;
            y += 1;
        }

        return -1;
    }

    int scan_right(const std::vector<std::vector<uint8_t> > &image,
                   int start_x, int start_y, int steps, bool debug) {
        const auto empty = std::make_tuple(0, 0, 0);
        auto last = empty;

        int height = image.size();
        int width = image[0].size();

        for (int attempt = 0; attempt < steps; ++attempt) {
            start_x += 1;

            if (debug) {
                std::cerr << "scan_right x: " << start_x << " y: " << start_y << "\n";
            }

            if (start_y < 0 || start_y >= height || start_x < 0 || start_x >= width) {
                return -1;
            }

            auto ret = std::make_tuple(image[start_y][start_x], image[start_y][start_x], image[start_y][start_x]);

            if (last != empty && ret != last) {
                return start_x - 1;
            }

            last = ret;
        }

        return -1;
    }

    int scan_left(const std::vector<std::vector<uint8_t> > &image,
                  int start_x, int start_y, int steps, bool debug) {
        const auto empty = std::make_tuple(0, 0, 0);
        auto last = empty;

        for (int attempt = 0; attempt < steps; ++attempt) {
            start_x -= 1;

            if (debug) {
                std::cerr << "scan_left x: " << start_x << " y: " << start_y << "\n";
            }

            auto ret = std::make_tuple(image[start_y][start_x], image[start_y][start_x], image[start_y][start_x]);

            if (last != empty && ret != last) {
                return start_x + 1;
            }

            last = ret;
        }

        return -1;
    }
}  // namespace helpers

