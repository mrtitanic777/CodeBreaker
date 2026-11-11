#include "cb/verify/gate16/screen.hpp"

#include <algorithm>
#include <cmath>
#include <vector>

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "zlib.h"

namespace cb::verify::gate16 {
namespace {

constexpr std::uint32_t kMaxSide = 1u << 14;   // no screen is wider than this; a header that says so is not a picture
constexpr int kPrecisionBits = 32 - 8 - 2;     // Pillow's fixed-point coefficients (Resample.c)

// ---- Pillow's bicubic resample (libImaging/Resample.c, 8 bits a channel) ---------------------------------------------

double bicubic(double x) {
    constexpr double a = -0.5;
    x = std::abs(x);
    if (x < 1.0)
        return ((a + 2.0) * x - (a + 3.0)) * x * x + 1;
    if (x < 2.0)
        return (((x - 5) * x + 8) * x - 4) * a;
    return 0.0;
}

// For each output pixel along one axis: the first input pixel it reads, how many it reads, and its coefficients.
struct Coefficients {
    std::size_t width = 0;                   // coefficients kept per output pixel
    std::vector<std::int64_t> first, count;  // per output pixel
    std::vector<std::int64_t> weight;        // `width` per output pixel, 22-bit fixed point
};

Coefficients precompute(std::uint32_t in_size, std::uint32_t out_size) {
    const double scale = static_cast<double>(in_size) / out_size;
    const double filter_scale = std::max(scale, 1.0);   // (a shrink widens the filter: the antialiasing)
    const double support = 2.0 * filter_scale;
    Coefficients c;
    c.width = static_cast<std::size_t>(std::ceil(support)) * 2 + 1;
    c.first.resize(out_size);
    c.count.resize(out_size);
    c.weight.assign(std::size_t{out_size} * c.width, 0);
    std::vector<double> k(c.width);
    const double inverse_scale = 1.0 / filter_scale;
    for (std::uint32_t out = 0; out < out_size; ++out) {
        const double center = (out + 0.5) * scale;
        const std::int64_t lo = std::max<std::int64_t>(0, static_cast<std::int64_t>(center - support + 0.5));
        const std::int64_t hi = std::min<std::int64_t>(in_size, static_cast<std::int64_t>(center + support + 0.5));
        const std::int64_t n = hi - lo;
        std::fill(k.begin(), k.end(), 0.0);
        double total = 0.0;
        for (std::int64_t x = 0; x < n; ++x) {
            const double w = bicubic((static_cast<double>(x + lo) - center + 0.5) * inverse_scale);
            k[static_cast<std::size_t>(x)] = w;
            total += w;
        }
        if (total != 0.0)
            for (std::int64_t x = 0; x < n; ++x)
                k[static_cast<std::size_t>(x)] /= total;
        c.first[out] = lo;
        c.count[out] = n;
        for (std::size_t x = 0; x < c.width; ++x) {
            const double v = k[x] * (1 << kPrecisionBits);
            c.weight[std::size_t{out} * c.width + x] = static_cast<std::int64_t>(v < 0 ? v - 0.5 : v + 0.5);
        }
    }
    return c;
}

std::uint8_t clip8(std::int64_t accumulated) {
    return static_cast<std::uint8_t>(std::clamp<std::int64_t>(accumulated >> kPrecisionBits, 0, 255));   // (arithmetic shift)
}

Bytes resample_horizontal(const Bytes& in, std::uint32_t w, std::uint32_t h, std::uint32_t out_w) {
    const Coefficients c = precompute(w, out_w);
    Bytes out(std::size_t{out_w} * h * 3);
    for (std::uint32_t y = 0; y < h; ++y)
        for (std::uint32_t x = 0; x < out_w; ++x)
            for (std::size_t channel = 0; channel < 3; ++channel) {
                std::int64_t sum = std::int64_t{1} << (kPrecisionBits - 1);
                for (std::int64_t i = 0; i < c.count[x]; ++i)
                    sum += std::int64_t{in[((std::size_t{y} * w) + static_cast<std::size_t>(c.first[x] + i)) * 3 + channel]} *
                           c.weight[std::size_t{x} * c.width + static_cast<std::size_t>(i)];
                out[(std::size_t{y} * out_w + x) * 3 + channel] = clip8(sum);
            }
    return out;
}

Bytes resample_vertical(const Bytes& in, std::uint32_t w, std::uint32_t h, std::uint32_t out_h) {
    const Coefficients c = precompute(h, out_h);
    Bytes out(std::size_t{w} * out_h * 3);
    for (std::uint32_t y = 0; y < out_h; ++y)
        for (std::uint32_t x = 0; x < w; ++x)
            for (std::size_t channel = 0; channel < 3; ++channel) {
                std::int64_t sum = std::int64_t{1} << (kPrecisionBits - 1);
                for (std::int64_t i = 0; i < c.count[y]; ++i)
                    sum += std::int64_t{in[((static_cast<std::size_t>(c.first[y] + i) * w) + x) * 3 + channel]} *
                           c.weight[std::size_t{y} * c.width + static_cast<std::size_t>(i)];
                out[(std::size_t{y} * w + x) * 3 + channel] = clip8(sum);
            }
    return out;
}

// ---- PNG ------------------------------------------------------------------------------------------------------------------

void put_be32(Bytes& out, std::uint32_t v) {
    for (int shift = 24; shift >= 0; shift -= 8)
        out.push_back(static_cast<std::uint8_t>(v >> shift));
}

void put_chunk(Bytes& out, std::string_view type, ByteView data) {
    put_be32(out, static_cast<std::uint32_t>(data.size()));
    const std::size_t crc_from = out.size();
    append(out, as_bytes(type));
    append(out, data);
    put_be32(out, crypto::crc32(ByteView(out).subspan(crc_from)));
}

}  // namespace

Picture::Picture(std::uint32_t width, std::uint32_t height, Bytes rgb)
    : width_(width), height_(height), rgb_(std::move(rgb)) {
    if (std::size_t{width} * height * 3 != rgb_.size())
        throw UsageError("a picture of this size needs width * height * 3 bytes");
}

std::optional<Picture> Picture::try_from_ppm(ByteView ppm) {
    std::size_t at = 2;
    if (ppm.size() < 2 || ppm[0] != 'P' || ppm[1] != '6')
        return std::nullopt;
    // A header token: white space and #-comments skipped, then up to the next white space.
    const auto token = [&]() -> std::optional<std::uint32_t> {
        while (at < ppm.size()) {
            if (ppm[at] == '#')
                while (at < ppm.size() && ppm[at] != '\n')
                    ++at;
            else if (ppm[at] == ' ' || ppm[at] == '\t' || ppm[at] == '\n' || ppm[at] == '\r')
                ++at;
            else
                break;
        }
        std::uint64_t value = 0;
        std::size_t digits = 0;
        while (at < ppm.size() && ppm[at] >= '0' && ppm[at] <= '9') {
            value = value * 10 + (ppm[at++] - '0');
            if (++digits > 9)
                return std::nullopt;
        }
        // (the token must end at white space or the end of the data, and have had a digit)
        if (digits == 0 || (at < ppm.size() && ppm[at] != ' ' && ppm[at] != '\t' && ppm[at] != '\n' && ppm[at] != '\r' &&
                            ppm[at] != '#'))
            return std::nullopt;
        return static_cast<std::uint32_t>(value);
    };
    const auto w = token(), h = token(), maxval = token();
    if (!w || !h || !maxval || *w == 0 || *h == 0 || *w > kMaxSide || *h > kMaxSide || *maxval != 255)
        return std::nullopt;
    ++at;   // the one white space after the maximum
    const std::size_t need = std::size_t{*w} * *h * 3;
    if (ppm.size() < at || ppm.size() - at < need)
        return std::nullopt;
    return Picture(*w, *h, Bytes(ppm.begin() + static_cast<std::ptrdiff_t>(at),
                                 ppm.begin() + static_cast<std::ptrdiff_t>(at + need)));
}

std::optional<Rgb> Picture::at(std::uint32_t x, std::uint32_t y) const {
    if (x >= width_ || y >= height_)
        return std::nullopt;
    const std::size_t i = (std::size_t{y} * width_ + x) * 3;
    return Rgb{rgb_[i], rgb_[i + 1], rgb_[i + 2]};
}

Picture Picture::resized(std::uint32_t width, std::uint32_t height) const {
    if (width == 0 || height == 0)
        throw UsageError("a picture cannot be resized to nothing");
    if (width_ == 0 || height_ == 0)
        return Picture(width, height, Bytes(std::size_t{width} * height * 3, 0));
    Bytes pixels = rgb_;
    std::uint32_t w = width_, h = height_;
    if (width != w) {
        pixels = resample_horizontal(pixels, w, h, width);
        w = width;
    }
    if (height != h) {
        pixels = resample_vertical(pixels, w, h, height);
        h = height;
    }
    return Picture(w, h, std::move(pixels));
}

Bytes Picture::to_png() const {
    Bytes out = {0x89, 'P', 'N', 'G', '\r', '\n', 0x1A, '\n'};
    Bytes header;
    put_be32(header, width_);
    put_be32(header, height_);
    append(header, Bytes{8, 2, 0, 0, 0});   // 8 bits, RGB, deflate, no filter method but 0, no interlace
    put_chunk(out, "IHDR", header);
    // each row: filter type 0, then its pixels
    Bytes raw;
    raw.reserve(rgb_.size() + height_);
    for (std::uint32_t y = 0; y < height_; ++y) {
        raw.push_back(0);
        const auto row = ByteView(rgb_).subspan(std::size_t{y} * width_ * 3, std::size_t{width_} * 3);
        append(raw, row);
    }
    z_stream zs{};
    if (deflateInit(&zs, Z_DEFAULT_COMPRESSION) != Z_OK)
        throw InternalError("zlib: deflateInit failed");
    Bytes packed(deflateBound(&zs, static_cast<uLong>(raw.size())));
    zs.next_in = raw.data();
    zs.avail_in = static_cast<uInt>(raw.size());
    zs.next_out = packed.data();
    zs.avail_out = static_cast<uInt>(packed.size());
    const int rc = deflate(&zs, Z_FINISH);
    packed.resize(zs.total_out);
    deflateEnd(&zs);
    if (rc != Z_STREAM_END)
        throw InternalError("zlib: deflate did not finish");
    put_chunk(out, "IDAT", packed);
    put_chunk(out, "IEND", {});
    return out;
}

}  // namespace cb::verify::gate16
