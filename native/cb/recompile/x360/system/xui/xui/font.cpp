// .xtt font reader and glyph rasteriser (see font.h). The rasteriser accumulates, per pixel, the signed area each
// outline edge covers to the pixel's left (the "accumulation buffer" technique of font-rs / stb_truetype's v2
// rasteriser), then a running sum along each row gives exact coverage; written from the technique, not their code.

#include "xui/font.h"

#include "xui/inflate.h"

#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <cstring>
#include <format>

namespace xui
{
    namespace
    {
        uint32_t Be32(const uint8_t* p) { return uint32_t(p[0]) << 24 | uint32_t(p[1]) << 16 | uint32_t(p[2]) << 8 | p[3]; }
        uint16_t Be16(const uint8_t* p) { return uint16_t(p[0] << 8 | p[1]); }
        int16_t S16(const uint8_t* p) { return int16_t(Be16(p)); }

        struct Accumulator
        {
            int w, h;
            std::vector<float> a;
            Accumulator(int width, int height) : w(width), h(height), a(size_t(width + 2) * height + 2, 0.0f) {}

            void Line(float x0, float y0, float x1, float y1)
            {
                if (y0 == y1)
                    return;
                float dir = 1.0f;
                if (y0 > y1)
                {
                    std::swap(x0, x1);
                    std::swap(y0, y1);
                    dir = -1.0f;
                }
                const float dxdy = (x1 - x0) / (y1 - y0);
                float x = x0;
                if (y0 < 0)
                    x -= y0 * dxdy;
                const int rowEnd = std::min(h, int(std::ceil(y1)));
                const int stride = w + 2;
                for (int y = std::max(0, int(y0)); y < rowEnd; y++)
                {
                    // The part of the edge inside this row, its height (signed by direction) and x extent.
                    const float dy = std::min(float(y + 1), y1) - std::max(float(y), y0);
                    const float xnext = x + dxdy * dy;
                    const float d = dy * dir;
                    const float xa = std::min(x, xnext), xb = std::max(x, xnext);
                    const float xaFloor = std::floor(xa);
                    const int ia = int(xaFloor);
                    const int ib = int(std::ceil(xb));
                    float* row = a.data() + size_t(y) * stride;
                    if (ia < 0 || ib + 1 >= stride)
                    {
                        x = xnext; // outside the bitmap: cannot happen for edges inside the computed bounds
                        continue;
                    }
                    if (ib <= ia + 1)
                    {
                        // Within one pixel column: the area right of the edge's mean x goes to the next pixel.
                        const float mid = 0.5f * (x + xnext) - xaFloor;
                        row[ia] += d - d * mid;
                        row[ia + 1] += d * mid;
                    }
                    else
                    {
                        // Crossing several columns: the covered area grows linearly, quadratically at the ends.
                        const float s = 1.0f / (xb - xa);
                        const float fa = xa - xaFloor;
                        const float a0 = 0.5f * s * (1.0f - fa) * (1.0f - fa);
                        const float fb = xb - std::ceil(xb) + 1.0f;
                        const float am = 0.5f * s * fb * fb;
                        row[ia] += d * a0;
                        if (ib == ia + 2)
                            row[ia + 1] += d * (1.0f - a0 - am);
                        else
                        {
                            const float a1 = s * (1.5f - fa);
                            row[ia + 1] += d * (a1 - a0);
                            for (int i = ia + 2; i < ib - 1; i++)
                                row[i] += d * s;
                            const float a2 = a1 + float(ib - ia - 3) * s;
                            row[ib - 1] += d * (1.0f - a2 - am);
                        }
                        row[ib] += d * am;
                    }
                    x = xnext;
                }
            }
        };
    }

    bool Font::Load(std::vector<uint8_t> data, std::string& error)
    {
        file_ = std::move(data);
        if (file_.size() < 0x118 || std::memcmp(file_.data(), "xttf", 4) != 0)
            return (error = "not an xttf font", false);
        const uint32_t compressed = Be32(&file_[0x10C]), size = Be32(&file_[0x110]);
        if (0x118 + uint64_t(compressed) > file_.size() || !Inflate(&file_[0x118], compressed, directory_, true) ||
            directory_.size() != size)
            return (error = "bad table directory", false);
        const uint16_t count = Be16(&directory_[4]);
        if (12 + size_t(count) * 16 > directory_.size())
            return (error = "bad table directory", false);
        for (uint16_t i = 0; i < count; i++)
        {
            const uint8_t* e = &directory_[12 + size_t(i) * 16];
            tables_[std::string(reinterpret_cast<const char*>(e), 4)] = { Be32(e + 8), Be32(e + 12) };
        }
        size_t n;
        const uint8_t* head = Table("head", n);
        if (!head || n < 54)
            return (error = "no head table", false);
        unitsPerEm_ = Be16(head + 18);
        if (S16(head + 50) != 0 && S16(head + 50) != 1)
            return (error = "unknown location format", false);
        const uint8_t* hhea = Table("hhea", n);
        if (!hhea || n < 36)
            return (error = "no hhea table", false);
        ascent_ = S16(hhea + 4);
        descent_ = S16(hhea + 6);
        lineGap_ = S16(hhea + 8);
        const uint16_t metrics = Be16(hhea + 34);
        const uint8_t* hmtx = Table("hmtx", n);
        if (!hmtx || n < size_t(metrics) * 4)
            return (error = "no hmtx table", false);
        for (uint16_t i = 0; i < metrics; i++)
            advances_.push_back(Be16(hmtx + i * 4));

        const auto glyf = tables_.find("xglf");
        const uint8_t* loc = Table("xloc", n);
        if (glyf == tables_.end() || !loc || n < 8 || uint64_t(glyf->second.first) + glyf->second.second > file_.size())
            return (error = "no xglf/xloc tables", false);
        glyphBase_ = glyf->second.first;
        glyphSize_ = glyf->second.second;
        for (size_t i = 0; i + 4 <= n; i += 4)
            locations_.push_back(Be32(loc + i));

        // Character map: a Windows Unicode subtable, format 4 (BMP) or 12.
        const uint8_t* cmap = Table("cmap", n);
        if (!cmap || n < 4)
            return (error = "no cmap table", false);
        uint32_t chosen = 0;
        for (uint16_t i = 0; i < Be16(cmap + 2) && 4 + i * 8 + 8 <= n; i++)
        {
            const uint8_t* r = cmap + 4 + i * 8;
            const uint16_t platform = Be16(r), encoding = Be16(r + 2);
            if ((platform == 3 && (encoding == 1 || encoding == 10)) || platform == 0)
                if (!chosen || encoding == 10)
                    chosen = Be32(r + 4);
        }
        if (!chosen || chosen + 4 > n)
            return (error = "no Unicode character map", false);
        const uint8_t* t = cmap + chosen;
        if (Be16(t) == 4)
        {
            const uint16_t segments = Be16(t + 6) / 2;
            const uint8_t* ends = t + 14;
            const uint8_t* starts = ends + segments * 2 + 2;
            const uint8_t* deltas = starts + segments * 2;
            const uint8_t* ranges = deltas + segments * 2;
            for (uint16_t s = 0; s < segments; s++)
            {
                const uint16_t start = Be16(starts + s * 2), end = Be16(ends + s * 2), range = Be16(ranges + s * 2);
                const int16_t delta = S16(deltas + s * 2);
                for (uint32_t c = start; c <= end && c != 0xFFFF; c++)
                {
                    uint32_t g;
                    if (range == 0)
                        g = (c + uint32_t(delta)) & 0xFFFF;
                    else
                    {
                        const uint8_t* p = ranges + s * 2 + range + (c - start) * 2;
                        if (p + 2 > cmap + n)
                            continue;
                        g = Be16(p);
                        if (g)
                            g = (g + uint32_t(delta)) & 0xFFFF;
                    }
                    if (g)
                        cmap_[char32_t(c)] = g;
                }
            }
        }
        else if (Be16(t) == 12)
        {
            const uint32_t groups = Be32(t + 12);
            for (uint32_t gi = 0; gi < groups && chosen + 16 + (gi + 1) * 12 <= n; gi++)
            {
                const uint8_t* g = t + 16 + gi * 12;
                for (uint32_t c = Be32(g); c <= Be32(g + 4); c++)
                    cmap_[char32_t(c)] = Be32(g + 8) + (c - Be32(g));
            }
        }
        else
            return (error = std::format("character map format {}", Be16(t)), false);

        if (const uint8_t* names = Table("name", n); names && n >= 6)
        {
            const uint16_t records = Be16(names + 2), storage = Be16(names + 4);
            for (uint16_t i = 0; i < records && 6 + i * 12 + 12 <= n; i++)
            {
                const uint8_t* r = names + 6 + i * 12;
                if (Be16(r) != 3 || Be16(r + 6) != 1)
                    continue;
                const uint16_t length = Be16(r + 8), offset = Be16(r + 10);
                if (size_t(storage) + offset + length > n)
                    continue;
                name_.clear();
                for (uint16_t k = 0; k + 1 < length; k += 2)
                    name_ += char(Be16(names + storage + offset + k));
            }
        }
        return true;
    }

    const uint8_t* Font::Table(const char* tag, size_t& size) const
    {
        const auto it = tables_.find(tag);
        if (it == tables_.end() || uint64_t(it->second.first) + it->second.second > directory_.size())
            return nullptr;
        size = it->second.second;
        return directory_.data() + it->second.first;
    }

    uint32_t Font::GlyphIndex(char32_t c) const
    {
        const auto it = cmap_.find(c);
        return it == cmap_.end() ? 0 : it->second;
    }

    int Font::Advance(uint32_t glyph) const
    {
        if (advances_.empty())
            return 0;
        return advances_[std::min<size_t>(glyph, advances_.size() - 1)];
    }

    bool Font::GlyphData(uint32_t glyph, const uint8_t*& data, size_t& size)
    {
        if (glyph + 1 >= locations_.size())
            return false;
        const uint32_t start = locations_[glyph], end = locations_[glyph + 1];
        if (start == end)
            return false; // no outline (a space)
        const uint32_t page = start >> 16;
        auto it = pages_.find(page);
        if (it == pages_.end())
        {
            std::vector<uint8_t> inflated;
            const size_t offset = size_t(page) * 4096;
            if (offset >= glyphSize_ ||
                !Inflate(&file_[glyphBase_ + offset], std::min<size_t>(4096, glyphSize_ - offset), inflated, true))
                return false;
            it = pages_.emplace(page, std::move(inflated)).first;
        }
        const size_t from = start & 0xFFFF;
        const size_t to = (end >> 16) == page ? (end & 0xFFFF) : it->second.size();
        if (from >= to || to > it->second.size())
            return false;
        data = it->second.data() + from;
        size = to - from;
        return true;
    }

    void Font::Outline(uint32_t glyph, std::vector<std::vector<Point>>& contours, int depth)
    {
        const uint8_t* d;
        size_t size;
        if (depth > 8 || !GlyphData(glyph, d, size) || size < 10)
            return;
        const int16_t count = S16(d);
        if (count < 0)
        {
            // Composite: components with an offset and an optional 2x2 transform (F2Dot14).
            size_t at = 10;
            for (;;)
            {
                if (at + 4 > size)
                    return;
                const uint16_t flags = Be16(d + at), component = Be16(d + at + 2);
                at += 4;
                float dx, dy;
                if (flags & 1)
                {
                    dx = S16(d + at);
                    dy = S16(d + at + 2);
                    at += 4;
                }
                else
                {
                    dx = int8_t(d[at]);
                    dy = int8_t(d[at + 1]);
                    at += 2;
                }
                if (!(flags & 2))
                    dx = dy = 0; // point-matched placement (not used by these fonts)
                float m[4] = { 1, 0, 0, 1 };
                if (flags & 8)
                {
                    m[0] = m[3] = S16(d + at) / 16384.0f;
                    at += 2;
                }
                else if (flags & 0x40)
                {
                    m[0] = S16(d + at) / 16384.0f;
                    m[3] = S16(d + at + 2) / 16384.0f;
                    at += 4;
                }
                else if (flags & 0x80)
                {
                    for (int i = 0; i < 4; i++)
                        m[i] = S16(d + at + i * 2) / 16384.0f;
                    at += 8;
                }
                std::vector<std::vector<Point>> part;
                Outline(component, part, depth + 1);
                for (auto& c : part)
                {
                    for (Point& p : c)
                    {
                        const float x = p.x, y = p.y;
                        p.x = x * m[0] + y * m[2] + dx;
                        p.y = x * m[1] + y * m[3] + dy;
                    }
                    contours.push_back(std::move(c));
                }
                if (!(flags & 0x20))
                    return;
            }
        }
        size_t at = 10;
        if (at + size_t(count) * 2 + 2 > size)
            return;
        std::vector<uint16_t> ends(static_cast<size_t>(count));
        for (int16_t i = 0; i < count; i++)
            ends[size_t(i)] = Be16(d + at + i * 2);
        at += size_t(count) * 2;
        const size_t points = count ? size_t(ends.back()) + 1 : 0;
        at += 2 + Be16(d + at); // instructions (unused: no hinting)
        std::vector<uint8_t> flags;
        while (flags.size() < points && at < size)
        {
            const uint8_t f = d[at++];
            flags.push_back(f);
            if ((f & 8) && at < size)
                for (uint8_t r = d[at++]; r && flags.size() < points; r--)
                    flags.push_back(f);
        }
        if (flags.size() < points)
            return;
        std::vector<Point> pts(points);
        int v = 0;
        for (size_t i = 0; i < points; i++)
        {
            if (flags[i] & 2)
                v += (flags[i] & 0x10) ? d[at] : -int(d[at]), at++;
            else if (!(flags[i] & 0x10))
                v += S16(d + at), at += 2;
            pts[i].x = float(v);
            pts[i].on = flags[i] & 1;
        }
        v = 0;
        for (size_t i = 0; i < points; i++)
        {
            if (flags[i] & 4)
                v += (flags[i] & 0x20) ? d[at] : -int(d[at]), at++;
            else if (!(flags[i] & 0x20))
                v += S16(d + at), at += 2;
            pts[i].y = float(v);
        }
        if (at > size)
            return;
        size_t start = 0;
        for (uint16_t end : ends)
        {
            if (end < start || end >= points)
                return;
            contours.emplace_back(pts.begin() + start, pts.begin() + end + 1);
            start = size_t(end) + 1;
        }
    }

    const GlyphBitmap& Font::Rasterize(uint32_t glyph, float scale, float subX, float subY)
    {
        // Cache by glyph, size and the fractional pen/baseline position in 1/8 pixels.
        std::lock_guard lock(cacheMutex_);
        const int sub = std::clamp(int(subX * 8.0f), 0, 7), subV = std::clamp(int(subY * 8.0f), 0, 7);
        const uint64_t key = uint64_t(glyph) << 40 | uint64_t(std::bit_cast<uint32_t>(scale)) << 8 |
            uint64_t(sub) << 3 | uint64_t(subV);
        if (const auto it = cache_.find(key); it != cache_.end())
            return it->second;
        GlyphBitmap& bitmap = cache_[key];
        std::vector<std::vector<Point>> contours;
        Outline(glyph, contours, 0);
        if (contours.empty())
            return bitmap;
        const float shift = float(sub) / 8.0f, shiftY = float(subV) / 8.0f;
        // Lines in pixel space (y down), quadratic curves flattened.
        std::vector<std::array<float, 4>> lines;
        float minX = 1e9f, minY = 1e9f, maxX = -1e9f, maxY = -1e9f;
        auto emit = [&](float x0, float y0, float x1, float y1) {
            lines.push_back({ x0, y0, x1, y1 });
            minX = std::min({ minX, x0, x1 });
            maxX = std::max({ maxX, x0, x1 });
            minY = std::min({ minY, y0, y1 });
            maxY = std::max({ maxY, y0, y1 });
        };
        for (const auto& c : contours)
        {
            const size_t n = c.size();
            if (n < 2)
                continue;
            auto px = [&](const Point& p) { return p.x * scale + shift; };
            auto py = [&](const Point& p) { return -p.y * scale + shiftY; };
            // Start at an on-curve point (or the midpoint of the first two off-curve points).
            size_t first = 0;
            while (first < n && !c[first].on)
                first++;
            float sx, sy;
            if (first == n)
            {
                sx = (px(c[0]) + px(c[1])) * 0.5f;
                sy = (py(c[0]) + py(c[1])) * 0.5f;
                first = 0;
            }
            else
            {
                sx = px(c[first]);
                sy = py(c[first]);
            }
            float cx = sx, cy = sy;
            for (size_t k = 1; k <= n; k++)
            {
                const Point& p = c[(first + k) % n];
                if (p.on)
                {
                    emit(cx, cy, px(p), py(p));
                    cx = px(p);
                    cy = py(p);
                    continue;
                }
                const Point& next = c[(first + k + 1) % n];
                float ex, ey;
                if (next.on)
                {
                    ex = px(next);
                    ey = py(next);
                    k++;
                }
                else
                {
                    ex = (px(p) + px(next)) * 0.5f;
                    ey = (py(p) + py(next)) * 0.5f;
                }
                const float qx = px(p), qy = py(p);
                const float devx = cx - 2 * qx + ex, devy = cy - 2 * qy + ey;
                const int steps = std::clamp(int(std::ceil(std::sqrt(std::sqrt(devx * devx + devy * devy) * 4.0f))), 1, 64);
                float lx = cx, ly = cy;
                for (int s = 1; s <= steps; s++)
                {
                    const float t = float(s) / float(steps), u = 1.0f - t;
                    const float nx = u * u * cx + 2 * u * t * qx + t * t * ex;
                    const float ny = u * u * cy + 2 * u * t * qy + t * t * ey;
                    emit(lx, ly, nx, ny);
                    lx = nx;
                    ly = ny;
                }
                cx = ex;
                cy = ey;
            }
            if (cx != sx || cy != sy)
                emit(cx, cy, sx, sy);
        }
        if (lines.empty())
            return bitmap;
        bitmap.left = int(std::floor(minX));
        bitmap.top = int(std::floor(minY));
        bitmap.width = int(std::ceil(maxX)) - bitmap.left + 1;
        bitmap.height = int(std::ceil(maxY)) - bitmap.top + 1;
        Accumulator acc(bitmap.width, bitmap.height);
        for (const auto& l : lines)
            acc.Line(l[0] - bitmap.left, l[1] - bitmap.top, l[2] - bitmap.left, l[3] - bitmap.top);
        bitmap.coverage.resize(size_t(bitmap.width) * bitmap.height);
        for (int y = 0; y < bitmap.height; y++)
        {
            float sum = 0;
            const float* row = acc.a.data() + size_t(y) * (bitmap.width + 2);
            for (int x = 0; x < bitmap.width; x++)
            {
                sum += row[x];
                bitmap.coverage[size_t(y) * bitmap.width + x] = uint8_t(std::min(1.0f, std::fabs(sum)) * 255.0f + 0.5f);
            }
        }
        return bitmap;
    }
}
