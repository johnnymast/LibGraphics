#include "LibGraphics/color/BackgroundScanner.hpp"

#include <iostream>
#include <cstdint>

namespace LibGraphics::Color {
    int BackgroundScanner::background_color_change_up(const std::vector<std::vector<uint8_t> > &image,
                                                      int start_x, int start_y, int max_attempts, bool debug) {
        const auto empty = std::make_tuple(0, 0, 0);
        auto last = empty;
        int y = start_y;
        int step = 0;

        auto ret = std::make_tuple(image[y][start_x], image[y][start_x], image[y][start_x]);
        last = ret;

        for (int attempt = 0; attempt < max_attempts; ++attempt) {
            y -= 1;
            step += 1;

            if (y < 0) {
                break;
            }

            if (debug) {
                std::cerr << "background_color_change_up x: " << start_x << " y: " << y << "\n";
            }

            auto ret = std::make_tuple(image[y][start_x], image[y][start_x], image[y][start_x]);

            if (last != empty && ret != last) {
                return step;
            }

            last = ret;
        }

        return -1;
    }


    int BackgroundScanner::background_color_change_down(const std::vector<std::vector<uint8_t> > &image,
                                                        int start_x, int start_y, int max_attempts, bool debug) {
        const auto empty = std::make_tuple(0, 0, 0);
        auto last = empty;
        int step = 0;
        int y = start_y;
        int height = ((int) image.size() - 1); // -1 is to change it from human-readable to an index.

        auto ret = std::make_tuple(image[y][start_x], image[y][start_x], image[y][start_x]);
        last = ret;

        for (int attempt = 0; attempt < max_attempts; ++attempt) {
            y += 1;
            step += 1;

            if (y > height || step > max_attempts) {
                break;
            }

            if (debug) {
                std::cerr << "background_color_change_down x: " << start_x << " y: " << y << "\n";
            }

            auto ret = std::make_tuple(image[y][start_x], image[y][start_x], image[y][start_x]);

            if (last != empty && ret != last) {
                return step;
            }

            last = ret;
        }

        return -1;
    }

    int BackgroundScanner::background_color_change_left(const std::vector<std::vector<uint8_t> > &image,
                                                        int start_x, int start_y, int max_attempts, bool debug) {
        const auto empty = std::make_tuple(0, 0, 0);
        auto last = empty;
        int step = 0;

        auto ret = std::make_tuple(image[start_y][start_x], image[start_y][start_x], image[start_y][start_x]);
        last = ret;

        for (int attempt = 0; attempt < max_attempts; ++attempt) {
            start_x -= 1;
            step += 1;

            if (start_x < 0) {
                break;
            }

            if (debug) {
                std::cerr << "background_color_change_left x: " << start_x << " y: " << start_y << "step: " << step <<
                        "\n";
            }

            auto ret = std::make_tuple(image[start_y][start_x], image[start_y][start_x], image[start_y][start_x]);

            if (last != empty && ret != last) {
                return step;
            }

            last = ret;
        }

        return -1;
    }

    int BackgroundScanner::background_color_change_right(const std::vector<std::vector<uint8_t> > &image,
                                                         int start_x, int start_y, int max_attempts, bool debug) {
        const auto empty = std::make_tuple(0, 0, 0);
        auto last = empty;

        int height = (int) image.size();
        int width = (int) image[0].size();
        int step = 0;

        // last need to be set
        auto ret = std::make_tuple(image[start_y][start_x], image[start_y][start_x], image[start_y][start_x]);
        last = ret;

        for (int attempt = 0; attempt < max_attempts; ++attempt) {
            start_x += 1;
            step += 1;

            if (step > max_attempts || start_y < 0 || start_y >= height || start_x < 0 || start_x >= width) {
                return -1;
            }

            if (debug) {
                std::cerr << "background_color_change_right x: " << start_x << " y: " << start_y << "\n";
            }

            auto ret = std::make_tuple(image[start_y][start_x], image[start_y][start_x], image[start_y][start_x]);

            if (last != empty && ret != last) {
                return step;
            }

            last = ret;
        }

        return -1;
    }


    // wrappers

    static std::vector<std::vector<uint8_t>> toMatrix(const Image& img) {
        std::vector<std::vector<uint8_t>> matrix(img.height, std::vector<uint8_t>(img.width));
        for (int y = 0; y < img.height; ++y) {
            for (int x = 0; x < img.width; ++x) {
                int idx = (y * img.width + x) * img.channels;
                matrix[y][x] = img.data[idx]; // take first channel (e.g. red or gray)
            }
        }
        return matrix;
    }

    int BackgroundScanner::background_color_change_up(const Image &img,
                                                      int start_x,
                                                      int start_y,
                                                      int max_attempts,
                                                      bool debug) {
        return background_color_change_up(toMatrix(img), start_x, start_y, max_attempts, debug);
    }

    int BackgroundScanner::background_color_change_down(const Image &img,
                                                        int start_x,
                                                        int start_y,
                                                        int max_attempts,
                                                        bool debug) {
        return background_color_change_down(toMatrix(img), start_x, start_y, max_attempts, debug);
    }

    int BackgroundScanner::background_color_change_left(const Image &img,
                                                        int start_x,
                                                        int start_y,
                                                        int max_attempts,
                                                        bool debug) {
        return background_color_change_left(toMatrix(img), start_x, start_y, max_attempts, debug);
    }

    int BackgroundScanner::background_color_change_right(const Image &img,
                                                         int start_x,
                                                         int start_y,
                                                         int max_attempts,
                                                         bool debug) {
        return background_color_change_right(toMatrix(img), start_x, start_y, max_attempts, debug);
    }
}
