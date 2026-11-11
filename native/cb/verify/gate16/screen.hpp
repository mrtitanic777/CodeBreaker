// cb/verify/gate16/screen.hpp -- a picture of a machine's screen: what QEMU's `screendump` writes (a binary PPM), read,
// shrunk the way the gate's thresholds were set, and saved as a PNG for whoever looks at a failure.
//
// A picture is 8-bit RGB, rows from the top. The gate looks at a few pixels of it, and at a copy shrunk to 160x120 or
// 90x50 (the share of white, the share of black): `resized` is Pillow's default bicubic filter with its antialiasing
// support and 22-bit fixed-point coefficients, bit for bit, because the thresholds were calibrated on pictures
// shrunk that way and a different filter would move them.
#pragma once

#include <cstdint>
#include <optional>

#include "cb/core/bytes.hpp"

namespace cb::verify::gate16 {

struct Rgb {
    std::uint8_t r = 0, g = 0, b = 0;
    friend constexpr bool operator==(const Rgb&, const Rgb&) = default;
    constexpr unsigned sum() const { return unsigned{r} + g + b; }
};

class Picture {
public:
    Picture() = default;
    // `rgb` is width * height * 3 bytes (a size that does not fit is a UsageError).
    Picture(std::uint32_t width, std::uint32_t height, Bytes rgb);

    // A binary PPM (P6, 8 bits a channel). nullopt for bytes that are not one -- or not all of one yet, which is what
    // a screendump still being written looks like.
    static std::optional<Picture> try_from_ppm(ByteView ppm);

    std::uint32_t width() const { return width_; }
    std::uint32_t height() const { return height_; }
    const Bytes& rgb() const { return rgb_; }

    // The pixel at (x, y); nullopt outside the picture.
    std::optional<Rgb> at(std::uint32_t x, std::uint32_t y) const;

    // This picture shrunk (or stretched) to the given size, as Pillow's Image.resize(size) does by default.
    Picture resized(std::uint32_t width, std::uint32_t height) const;

    // The picture as a PNG file (8-bit RGB, no filtering, one IDAT).
    Bytes to_png() const;

private:
    std::uint32_t width_ = 0, height_ = 0;
    Bytes rgb_;
};

}  // namespace cb::verify::gate16
