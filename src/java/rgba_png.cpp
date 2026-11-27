#include "java/rgba_png.hpp"

#include <algorithm>
#include <cstring>
#include <vector>

#include "core/error.hpp"

namespace cb::java::rgba_png {

Image crop(const Image& image, std::int64_t left, std::int64_t top, std::int64_t right, std::int64_t bottom) {
    if (right <= left || bottom <= top) return blank(0, 0);
    Image out = blank(static_cast<std::uint32_t>(right - left), static_cast<std::uint32_t>(bottom - top));
    for (std::uint32_t y = 0; y < out.height; ++y) {
        for (std::uint32_t x = 0; x < out.width; ++x) {
            const std::int64_t sx = left + x, sy = top + y;
            if (sx >= 0 && sx < image.width && sy >= 0 && sy < image.height) {
                std::memcpy(out.pixel(x, y), image.pixel(static_cast<std::uint32_t>(sx), static_cast<std::uint32_t>(sy)), 4);
            }
        }
    }
    return out;
}

Image resize_nearest(const Image& image, std::uint32_t size) {
    if (size == 0) throw UsageError("resize_nearest: the size must be at least 1");
    Image out = blank(size, size);
    if (image.width == 0 || image.height == 0) return out;
    const double sx = static_cast<double>(image.width) / size, sy = static_cast<double>(image.height) / size;
    // The source position of an output pixel is accumulated from half a step, not computed as (i + 0.5) * step:
    // the roundings differ, and this is how Pillow's nearest scale does it.
    std::vector<std::uint32_t> xin(size), yin(size);
    double xo = sx * 0.5, yo = sy * 0.5;
    for (std::uint32_t i = 0; i < size; ++i) {
        xin[i] = std::min(static_cast<std::uint32_t>(xo), image.width - 1);
        yin[i] = std::min(static_cast<std::uint32_t>(yo), image.height - 1);
        xo += sx;
        yo += sy;
    }
    for (std::uint32_t y = 0; y < size; ++y) {
        for (std::uint32_t x = 0; x < size; ++x) std::memcpy(out.pixel(x, y), image.pixel(xin[x], yin[y]), 4);
    }
    return out;
}

}  // namespace cb::java::rgba_png
