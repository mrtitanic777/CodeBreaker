#include "xui/canvas.h"

#include <algorithm>
#include <cmath>

namespace xui
{
    Rgba ArgbToRgba(uint32_t argb)
    {
        return { float((argb >> 16) & 0xFF) / 255.0f, float((argb >> 8) & 0xFF) / 255.0f, float(argb & 0xFF) / 255.0f,
                 float(argb >> 24) / 255.0f };
    }

    Affine Affine::Then(const Affine& n) const
    {
        return { n.a * a + n.c * b, n.b * a + n.d * b, n.a * c + n.c * d, n.b * c + n.d * d, n.a * tx + n.c * ty + n.tx,
                 n.b * tx + n.d * ty + n.ty };
    }

    Affine Affine::Inverse() const
    {
        const float det = a * d - b * c;
        if (det == 0)
            return { 0, 0, 0, 0, 0, 0 };
        const float ia = d / det, ib = -b / det, ic = -c / det, id = a / det;
        return { ia, ib, ic, id, -(ia * tx + ic * ty), -(ib * tx + id * ty) };
    }

    Canvas::Canvas(int width, int height) : width_(width), height_(height), px_(size_t(width) * height * 4, 0.0f) {}

    void Canvas::Clear()
    {
        std::fill(px_.begin(), px_.end(), 0.0f);
    }

    namespace
    {
        thread_local BlendMode t_blend = BlendMode::kNormal;
    }

    ScopedBlend::ScopedBlend(BlendMode mode) : previous_(t_blend)
    {
        t_blend = mode;
    }

    ScopedBlend::~ScopedBlend()
    {
        t_blend = previous_;
    }

    void Canvas::Blend(int x, int y, Rgba s)
    {
        if (s.a <= 0.0f || x < 0 || y < 0 || x >= width_ || y >= height_)
            return;
        float* p = &px_[(size_t(y) * width_ + x) * 4];
        const float inv = 1.0f - s.a;
        const float c[3] = { s.r, s.g, s.b };
        switch (t_blend)
        {
        case BlendMode::kNormal:
            for (int i = 0; i < 3; i++)
                p[i] = c[i] * s.a + p[i] * inv;
            break;
        // The others work on the colour already there (the target is opaque where they are used: the dashboards'
        // blade skins over their backgrounds), weighted by the source's alpha; the target's alpha blends as normal.
        case BlendMode::kMultiply:
            for (int i = 0; i < 3; i++)
                p[i] = p[i] * (inv + c[i] * s.a);
            break;
        case BlendMode::kDarken:
            for (int i = 0; i < 3; i++)
                p[i] = p[i] * inv + std::min(p[i], c[i]) * s.a;
            break;
        case BlendMode::kLighten:
            for (int i = 0; i < 3; i++)
                p[i] = p[i] * inv + std::max(p[i], c[i]) * s.a;
            break;
        case BlendMode::kAdd:
            for (int i = 0; i < 3; i++)
                p[i] = std::min(1.0f, p[i] + c[i] * s.a);
            break;
        case BlendMode::kSubtract:
            for (int i = 0; i < 3; i++)
                p[i] = std::max(0.0f, p[i] - c[i] * s.a);
            break;
        }
        p[3] = s.a + p[3] * inv;
    }

    void Canvas::FillPolygon(const std::vector<Vec2>& points, const std::function<Rgba(float, float)>& shade,
                             const ClipRect& clip)
    {
        if (points.size() < 3)
            return;
        float minY = points[0].y, maxY = points[0].y;
        for (const Vec2& p : points)
        {
            minY = std::min(minY, p.y);
            maxY = std::max(maxY, p.y);
        }
        const int y0 = std::max(clip.y0, int(std::ceil(minY - 0.5f)));
        const int y1 = std::min(clip.y1, int(std::ceil(maxY - 0.5f)));
        std::vector<std::pair<float, int>> crossings;
        for (int y = y0; y < y1; y++)
        {
            const float cy = float(y) + 0.5f;
            crossings.clear();
            for (size_t i = 0; i < points.size(); i++)
            {
                const Vec2 a = points[i], b = points[(i + 1) % points.size()];
                if (a.y == b.y)
                    continue;
                // Half-open in y (top edge inclusive) so shared vertices are counted once.
                const bool down = a.y < b.y;
                const float top = down ? a.y : b.y, bottom = down ? b.y : a.y;
                if (cy < top || cy >= bottom)
                    continue;
                const float x = a.x + (cy - a.y) * (b.x - a.x) / (b.y - a.y);
                crossings.push_back({ x, down ? 1 : -1 });
            }
            std::sort(crossings.begin(), crossings.end());
            int winding = 0;
            for (size_t i = 0; i + 1 < crossings.size(); i++)
            {
                winding += crossings[i].second;
                if (winding == 0)
                    continue;
                // Pixels whose centre lies in [left, right): the left edge is inclusive.
                const int x0 = std::max(clip.x0, int(std::ceil(crossings[i].first - 0.5f)));
                const int x1 = std::min(clip.x1, int(std::ceil(crossings[i + 1].first - 0.5f)));
                for (int x = x0; x < x1; x++)
                    Blend(x, y, shade(float(x) + 0.5f, cy));
            }
        }
    }

    namespace
    {
        int WrapIndex(int i, int n, Wrap w)
        {
            switch (w)
            {
            case Wrap::kWrap: return ((i % n) + n) % n;
            case Wrap::kMirror:
            {
                const int period = 2 * n;
                int m = ((i % period) + period) % period;
                return m < n ? m : period - 1 - m;
            }
            default: return std::clamp(i, 0, n - 1);
            }
        }
    }

    Rgba SampleBilinear(const Image& image, float u, float v, Wrap wrapU, Wrap wrapV)
    {
        const int w = int(image.width), h = int(image.height);
        if (w == 0 || h == 0)
            return {};
        const float fu = u - 0.5f, fv = v - 0.5f;
        const int iu = int(std::floor(fu)), iv = int(std::floor(fv));
        const float tu = fu - float(iu), tv = fv - float(iv);
        // Straight-alpha texels are filtered as they are, as the GPU filters the straight-alpha texture (the colour of
        // transparent texels bleeds into edges exactly as on the console); the blend multiplies by alpha afterwards.
        float acc[4] = {};
        for (int j = 0; j < 2; j++)
            for (int i = 0; i < 2; i++)
            {
                const float weight = (i ? tu : 1 - tu) * (j ? tv : 1 - tv);
                if (weight == 0)
                    continue;
                const uint8_t* t = &image.rgba[(size_t(WrapIndex(iv + j, h, wrapV)) * w + WrapIndex(iu + i, w, wrapU)) * 4];
                const float a = t[3] / 255.0f;
                acc[0] += weight * t[0] / 255.0f;
                acc[1] += weight * t[1] / 255.0f;
                acc[2] += weight * t[2] / 255.0f;
                acc[3] += weight * a;
            }
        return { acc[0], acc[1], acc[2], acc[3] };
    }

    void Canvas::DrawImage(const Image& image, const Affine& toDevice, Rgba tint, const ClipRect& clip, Wrap wrapU,
                           Wrap wrapV)
    {
        const float w = float(image.width), h = float(image.height);
        const Affine toTexel = toDevice.Inverse();
        const std::vector<Vec2> quad = { toDevice.Apply({ 0, 0 }), toDevice.Apply({ w, 0 }), toDevice.Apply({ w, h }),
                                         toDevice.Apply({ 0, h }) };
        FillPolygon(quad, [&](float x, float y) {
            const Vec2 t = toTexel.Apply({ x, y });
            Rgba s = SampleBilinear(image, t.x, t.y, wrapU, wrapV);
            return Rgba{ s.r * tint.r, s.g * tint.g, s.b * tint.b, s.a * tint.a };
        }, clip);
    }

    void Canvas::DrawImagePart(const Image& image, float sx0, float sy0, float sx1, float sy1, const Affine& toDevice,
                               Rgba tint, const ClipRect& clip)
    {
        // toDevice maps the part's texel rectangle (sx0..sx1, sy0..sy1) to the device; sampling clamps to the image.
        const Affine toTexel = toDevice.Inverse();
        const std::vector<Vec2> quad = { toDevice.Apply({ sx0, sy0 }), toDevice.Apply({ sx1, sy0 }),
                                         toDevice.Apply({ sx1, sy1 }), toDevice.Apply({ sx0, sy1 }) };
        FillPolygon(quad, [&](float x, float y) {
            const Vec2 t = toTexel.Apply({ x, y });
            Rgba s = SampleBilinear(image, t.x, t.y, Wrap::kClamp, Wrap::kClamp);
            return Rgba{ s.r * tint.r, s.g * tint.g, s.b * tint.b, s.a * tint.a };
        }, clip);
    }

    void Canvas::DrawGlyph(const GlyphBitmap& g, int x, int y, Rgba color, const ClipRect& clip)
    {
        for (int j = 0; j < g.height; j++)
        {
            const int py = y + g.top + j;
            if (py < clip.y0 || py >= clip.y1)
                continue;
            for (int i = 0; i < g.width; i++)
            {
                const int px = x + g.left + i;
                const uint8_t c = g.coverage[size_t(j) * g.width + i];
                if (!c || px < clip.x0 || px >= clip.x1)
                    continue;
                Blend(px, py, { color.r, color.g, color.b, color.a * float(c) / 255.0f });
            }
        }
    }

    void Canvas::ToRgba8(std::vector<uint8_t>& out) const
    {
        out.resize(px_.size());
        for (size_t i = 0; i < px_.size(); i++)
            out[i] = uint8_t(std::clamp(px_[i] * 255.0f + 0.5f, 0.0f, 255.0f));
    }

    void Canvas::ClearRows(int y0, int y1)
    {
        std::fill(px_.begin() + size_t(y0) * width_ * 4, px_.begin() + size_t(y1) * width_ * 4, 0.0f);
    }

    void Canvas::ToRgba8Rows(std::vector<uint8_t>& out, int y0, int y1) const
    {
        for (size_t i = size_t(y0) * width_ * 4, end = size_t(y1) * width_ * 4; i < end; i++)
            out[i] = uint8_t(std::clamp(px_[i] * 255.0f + 0.5f, 0.0f, 255.0f));
    }
}
