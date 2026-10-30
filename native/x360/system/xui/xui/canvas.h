#pragma once

// Where XUI elements draw (Surface), and a software render target (Canvas) drawing the way XUI's Direct3D renderer
// does on the console: polygons cover the pixels whose centres they contain (top-left rule, nonzero winding, no edge
// antialiasing), textures are sampled bilinearly at pixel centres (texel centres at half-texel offsets), and draws
// blend with XUI's normal blend (source alpha, inverse source alpha) unless an element's BlendMode says otherwise
// (ScopedBlend). The target holds premultiplied colour and alpha, so the finished overlay composites over the
// title's frame exactly as if it had been drawn into it.
//
// Elements describe each draw as a Paint (a colour, a gradient or a texture over a polygon), so another Surface can
// draw the same draws elsewhere (a GPU renderer) and the Canvas stays the reference its pixels are compared with.

#include "xui/font.h"
#include "xui/image.h"

#include <cstdint>
#include <functional>
#include <string>
#include <utility>
#include <vector>

namespace xui
{
    struct Rgba
    {
        float r = 0, g = 0, b = 0, a = 0; // straight alpha, 0..1
    };
    Rgba ArgbToRgba(uint32_t argb);

    struct Vec2 { float x = 0, y = 0; };

    // x' = a x + c y + tx, y' = b x + d y + ty
    struct Affine
    {
        float a = 1, b = 0, c = 0, d = 1, tx = 0, ty = 0;
        Vec2 Apply(Vec2 p) const { return { a * p.x + c * p.y + tx, b * p.x + d * p.y + ty }; }
        Affine Then(const Affine& next) const; // this first, then next
        Affine Inverse() const;
        static Affine Translate(float x, float y) { return { 1, 0, 0, 1, x, y }; }
        static Affine Scale(float x, float y) { return { x, 0, 0, y, 0, 0 }; }
    };

    struct ClipRect { int x0 = 0, y0 = 0, x1 = 0, y1 = 0; };

    enum class Wrap { kWrap, kMirror, kClamp };

    // XUI_BLEND_MODE (xuirender.h; 0, XUI_BLEND_DEFAULT, inherits the parent's): how a draw combines with the target.
    enum class BlendMode { kNormal = 1, kMultiply = 2, kDarken = 3, kLighten = 4, kAdd = 5, kSubtract = 6 };
    // The blend of every draw on this thread while it lives (an element's BlendMode for its subtree: the overlay is
    // drawn in bands on several threads, each drawing the whole tree).
    class ScopedBlend
    {
    public:
        explicit ScopedBlend(BlendMode mode);
        ~ScopedBlend();
        ScopedBlend(const ScopedBlend&) = delete;
        ScopedBlend& operator=(const ScopedBlend&) = delete;
        static BlendMode Current(); // this thread's

    private:
        BlendMode previous_;
    };

    // What covers a polygon's pixels; each pixel is evaluated at its centre (x + 0.5, y + 0.5) in device pixels.
    struct Paint
    {
        enum class Kind { kSolid, kLinear, kRadial, kTexture };
        Kind kind = Kind::kSolid;
        Rgba color; // kSolid

        // kLinear / kRadial (an XuiFigure's gradient fill): `toBrush` maps the device to brush space (u, v); the
        // gradient position t is u (linear) or 2 |(u, v) - (0.5, 0.5)| (radial). As XUI draws it (the 1888 pixel
        // shader DE5EC1EF8BA09D7D: tfetch of 2 |q| from a 128 x 1 texture): the stops are baked into 128 texels
        // (GradientTable: texel i at position (i + 0.5) / 128), sampled bilinearly at t with the texture clamped to
        // its edge texels (past the end: the last texel, not the last stop), then multiplied by `factor` and the
        // alpha by `opacity`.
        Affine toBrush;
        // A perspective-projected figure (a 3D-tilted element under XUI's projection): device -> brush is projective,
        // (u, v) = (h0 x + h1 y + h2, h3 x + h4 y + h5) / (h6 x + h7 y + h8), in place of toBrush.
        bool projective = false;
        float projection[9] = {};
        std::vector<std::pair<float, uint32_t>> stops; // position, ARGB; at least one
        std::vector<uint32_t> table;                   // the 128 texels (GradientTable(stops))
        uint32_t factor = 0xFFFFFFFF;
        float opacity = 1.0f;

        // kTexture: `toTexel` maps the device to texel space (0..width, 0..height); the bilinear sample (straight
        // alpha) is multiplied by `tint`.
        const Image* image = nullptr;
        Affine toTexel;
        Wrap wrapU = Wrap::kClamp, wrapV = Wrap::kClamp;
        Rgba tint{ 1, 1, 1, 1 };

        // This paint's straight-alpha colour at device point (x, y): the reference every Surface follows.
        Rgba At(float x, float y) const;
    };

    // A render target elements draw on (Element::Render).
    class Surface
    {
    public:
        virtual ~Surface() = default;
        int Width() const { return width_; }
        int Height() const { return height_; }

        // The pixels of `points` (device coordinates, nonzero winding) inside `clip`, painted with `paint` and blended
        // with this thread's BlendMode (ScopedBlend).
        virtual void FillPaint(const std::vector<Vec2>& points, const Paint& paint, const ClipRect& clip) = 0;

        // A textured parallelogram: `toDevice` maps image texel space (0..w, 0..h) to device pixels; colour is
        // multiplied by `tint` (straight alpha).
        void DrawImage(const Image& image, const Affine& toDevice, Rgba tint, const ClipRect& clip,
                       Wrap wrapU = Wrap::kClamp, Wrap wrapV = Wrap::kClamp);
        // The texel rectangle (sx0, sy0) .. (sx1, sy1) of an image, sampled clamped to the whole image (nine-grid
        // slices).
        void DrawImagePart(const Image& image, float sx0, float sy0, float sx1, float sy1, const Affine& toDevice,
                           Rgba tint, const ClipRect& clip);

        // XAM draws the HUD into an 852 x 480 target (1 unit = 1 pixel) that the console scales to the screen, so an
        // image covers the whole units whose centres it contains: its edges land on the unit grid (a skin image at
        // y 1.5 .. 25.5 shows as 1 .. 25; the owner's real party row, NOTES session 26). `unitToDevice` maps HUD units
        // to this target; without a grid (the default) nothing snaps.
        void SetUnitGrid(const Affine& unitToDevice)
        {
            grid_ = unitToDevice;
            hasGrid_ = true;
        }
        const Affine* UnitGrid() const { return hasGrid_ ? &grid_ : nullptr; }

    protected:
        Surface(int width, int height) : width_(width), height_(height) {}
        int width_, height_;

    private:
        Affine grid_;
        bool hasGrid_ = false;
    };

    // The software target: premultiplied float RGBA, drawn on the CPU (in bands on several threads: every draw is
    // clipped to its band's rows).
    class Canvas : public Surface
    {
    public:
        Canvas(int width, int height);
        void Clear();

        void FillPaint(const std::vector<Vec2>& points, const Paint& paint, const ClipRect& clip) override;
        // Pixels the polygon (device coordinates, nonzero winding) covers, shaded per pixel centre.
        void FillPolygon(const std::vector<Vec2>& points, const std::function<Rgba(float x, float y)>& shade,
                         const ClipRect& clip);
        // A glyph's coverage at integer device position (x, y) = the pen position.
        void DrawGlyph(const GlyphBitmap& glyph, int x, int y, Rgba color, const ClipRect& clip);

        void Blend(int x, int y, Rgba src); // straight-alpha source over the pixel
        // Premultiplied RGBA8, rows top to bottom.
        void ToRgba8(std::vector<uint8_t>& out) const;
        // Rows y0..y1-1 only (the overlay is cleared, drawn and converted in bands); `out` holds the whole image.
        void ClearRows(int y0, int y1);
        void ToRgba8Rows(std::vector<uint8_t>& out, int y0, int y1) const;

    private:
        std::vector<float> px_; // premultiplied r, g, b, a
    };

    Rgba SampleBilinear(const Image& image, float u, float v, Wrap wrapU, Wrap wrapV); // u, v in texels
    // XUI_PROBE=<x>,<y>: the CPU canvas logs every draw that reaches that pixel (the element drawing it, its paint's
    // colour there, the blend, the pixel after), for comparing layer stacks with a reference.
    void SetProbeLabel(const std::string* label); // the element drawing now (this thread)
    // Gradient stops' colour at position t (bytes per channel, as Paint describes), before the factor.
    uint32_t GradientColor(const std::vector<std::pair<float, uint32_t>>& stops, float t);
    // XUI's gradient texture: 128 texels, texel i the stops' colour at (i + 0.5) / 128.
    std::vector<uint32_t> GradientTable(const std::vector<std::pair<float, uint32_t>>& stops);
    // A gradient texture sampled at t as the GPU samples it: bilinear between texel centres, clamped at the edges.
    Rgba SampleGradient(const std::vector<uint32_t>& table, float t);
    uint32_t MulColor(uint32_t a, uint32_t b); // per channel, bytes: (a b + 127) / 255
}
