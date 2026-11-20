// app/window_render_win.hpp -- the CodeBreaker window drawn with Direct2D and DirectWrite (Windows only).
//
// One Painter draws the whole window from a Model and its Layout onto any Direct2D render target: the window's own (HwndRenderTarget) and a WIC
// bitmap (`codebreaker app --screenshot`) go through the same code, so the screenshots show what the window draws.
#pragma once
#ifdef _WIN32

#include <d2d1.h>
#include <dwrite.h>
#include <windows.h>

#include <cstdint>
#include <map>
#include <string>
#include <tuple>

#include "app/window_layout.hpp"

namespace cb::app::window {

// A COM interface pointer that releases itself.
template <class T>
class ComPtr {
public:
    ComPtr() = default;
    ComPtr(const ComPtr& o) : p_(o.p_) { if (p_) p_->AddRef(); }
    ComPtr(ComPtr&& o) noexcept : p_(o.p_) { o.p_ = nullptr; }
    ComPtr& operator=(ComPtr o) noexcept { std::swap(p_, o.p_); return *this; }
    ~ComPtr() { reset(); }
    void reset() { if (p_) { p_->Release(); p_ = nullptr; } }
    T* get() const { return p_; }
    T* operator->() const { return p_; }
    T** put() { reset(); return &p_; }
    explicit operator bool() const { return p_ != nullptr; }

private:
    T* p_ = nullptr;
};

// What the system says the window should look like.
struct Appearance {
    bool dark = false;
    std::uint32_t accent = 0;           // 0xRRGGBB of the filled-accent colour for this theme; 0: the design's default
    bool backdrop = false;              // the Mica backdrop is behind the window: the background is left transparent
    bool rounded_corners = false;       // draw the window's own rounded outline (a screenshot has no DWM to do it)
};

class Painter {
public:
    Painter();
    // False when Direct2D or DirectWrite cannot be had.
    bool ok() const { return factory_ && write_; }
    ID2D1Factory* d2d() const { return factory_.get(); }
    IDWriteFactory* dwrite() const { return write_.get(); }

    // A window's render target, premultiplied alpha so the backdrop shows through where the window leaves it clear.
    ComPtr<ID2D1HwndRenderTarget> make_window_target(HWND window, UINT width, UINT height) const;

    // Draw everything. The caller has set the target's DPI (so the layout's DIPs are right) and calls BeginDraw/EndDraw.
    void paint(ID2D1RenderTarget* target, const Model& model, const Layout& layout, const Appearance& look);

    // The width of `text` in `size` DIPs of the body font: for the layouts that need to fit (the output path).
    float measure(const std::wstring& text, float size, bool semibold) const;

    // A cached text format: kind 0 body, 1 display, 2 icons, 3 monospace; align is a DWRITE_TEXT_ALIGNMENT, vertical a DWRITE_PARAGRAPH_ALIGNMENT.
    IDWriteTextFormat* format_for(int kind, float size, int weight, int align, int vertical, bool wrap);

private:
    struct FormatKey {
        int kind;          // 0 body, 1 display, 2 icons
        int size10;
        int weight;
        int align;         // DWRITE_TEXT_ALIGNMENT
        int vertical;      // DWRITE_PARAGRAPH_ALIGNMENT
        bool wrap;
        bool operator<(const FormatKey& o) const {
            return std::tie(kind, size10, weight, align, vertical, wrap) < std::tie(o.kind, o.size10, o.weight, o.align, o.vertical, o.wrap);
        }
    };
    std::wstring family(int kind) const;

    ComPtr<ID2D1Factory> factory_;
    ComPtr<IDWriteFactory> write_;
    std::map<FormatKey, ComPtr<IDWriteTextFormat>> formats_;
    std::wstring body_, display_, icons_, mono_;
};

// UTF-8 <-> UTF-16.
std::wstring widen(const std::string& utf8);
std::string narrow(const std::wstring& wide);

}  // namespace cb::app::window

#endif  // _WIN32
