#pragma once

// A software render target for the system UI overlay, drawing the way XUI's Direct3D renderer does on the console:
// triangles/polygons cover the pixels whose centres they contain (top-left rule, no edge antialiasing), textures are
// sampled bilinearly at pixel centres (texel centres at half-texel offsets), and draws blend with XUI's normal blend
// (source alpha, inverse source alpha) unless an element's BlendMode says otherwise (ScopedBlend). The target holds premultiplied colour and alpha, so the finished
// overlay composites over the title's frame exactly as if it had been drawn into it.

#include "xui/font.h"
#include "xui/image.h"

#include <cstdint>
#include <functional>
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

    private:
        BlendMode previous_;
    };

    class Canvas
    {
    public:
        Canvas(int width, int height);
        int Width() const { return width_; }
        int Height() const { return height_; }
        void Clear();

        // Pixels the polygon (device coordinates, nonzero winding) covers, shaded per pixel centre.
        void FillPolygon(const std::vector<Vec2>& points, const std::function<Rgba(float x, float y)>& shade,
                         const ClipRect& clip);
        // A textured parallelogram: `toDevice` maps image texel space (0..w, 0..h) to device pixels; colour is
        // multiplied by `tint` (straight alpha). `source` limits sampling to a texel rectangle (nine-grid slices).
        void DrawImage(const Image& image, const Affine& toDevice, Rgba tint, const ClipRect& clip,
                       Wrap wrapU = Wrap::kClamp, Wrap wrapV = Wrap::kClamp);
        void DrawImagePart(const Image& image, float sx0, float sy0, float sx1, float sy1, const Affine& toDevice,
                           Rgba tint, const ClipRect& clip);
        // A glyph's coverage at integer device position (x, y) = the pen position.
        void DrawGlyph(const GlyphBitmap& glyph, int x, int y, Rgba color, const ClipRect& clip);

        // XAM draws the HUD into an 852 x 480 target (1 unit = 1 pixel) that the console scales to the screen, so an
        // image covers the whole units whose centres it contains: its edges land on the unit grid (a skin image at
        // y 1.5 .. 25.5 shows as 1 .. 25; the user's real party row). `unitToDevice` maps HUD units
        // to this canvas; without a grid (the default) nothing snaps.
        void SetUnitGrid(const Affine& unitToDevice)
        {
            grid_ = unitToDevice;
            hasGrid_ = true;
        }
        const Affine* UnitGrid() const { return hasGrid_ ? &grid_ : nullptr; }

        void Blend(int x, int y, Rgba src); // straight-alpha source over the pixel
        // Premultiplied RGBA8, rows top to bottom.
        void ToRgba8(std::vector<uint8_t>& out) const;
        // Rows y0..y1-1 only (the overlay is cleared, drawn and converted in bands); `out` holds the whole image.
        void ClearRows(int y0, int y1);
        void ToRgba8Rows(std::vector<uint8_t>& out, int y0, int y1) const;

    private:
        int width_, height_;
        std::vector<float> px_; // premultiplied r, g, b, a
        Affine grid_;
        bool hasGrid_ = false;
    };

    Rgba SampleBilinear(const Image& image, float u, float v, Wrap wrapU, Wrap wrapV); // u, v in texels
}
