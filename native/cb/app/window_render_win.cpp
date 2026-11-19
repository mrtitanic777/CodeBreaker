// cb/app/window_render_win.cpp -- the CodeBreaker window drawn with Direct2D and DirectWrite (see window_render_win.hpp). The look is the approved
// Windows 11 design (state/work/ui-mockup/win11.html): Segoe UI Variable text, Segoe Fluent Icons glyphs, Fluent card, button, field, progress bar and
// InfoBar styling, light and dark.
#ifdef _WIN32
#include "cb/app/window_render_win.hpp"

#include <algorithm>
#include <cmath>

namespace cb::app::window {

std::wstring widen(const std::string& utf8) {
    if (utf8.empty()) return {};
    const int n = MultiByteToWideChar(CP_UTF8, 0, utf8.data(), static_cast<int>(utf8.size()), nullptr, 0);
    std::wstring out(static_cast<std::size_t>(n), L'\0');
    MultiByteToWideChar(CP_UTF8, 0, utf8.data(), static_cast<int>(utf8.size()), out.data(), n);
    return out;
}

std::string narrow(const std::wstring& wide) {
    if (wide.empty()) return {};
    const int n = WideCharToMultiByte(CP_UTF8, 0, wide.data(), static_cast<int>(wide.size()), nullptr, 0, nullptr, nullptr);
    std::string out(static_cast<std::size_t>(n), '\0');
    WideCharToMultiByte(CP_UTF8, 0, wide.data(), static_cast<int>(wide.size()), out.data(), n, nullptr, nullptr);
    return out;
}

namespace {

D2D1_COLOR_F rd_color(std::uint32_t rgb, float alpha = 1.0f) {
    return D2D1::ColorF(static_cast<float>((rgb >> 16) & 0xFF) / 255.0f, static_cast<float>((rgb >> 8) & 0xFF) / 255.0f,
                        static_cast<float>(rgb & 0xFF) / 255.0f, alpha);
}

D2D1_COLOR_F rd_mix(D2D1_COLOR_F a, D2D1_COLOR_F b, float t) {
    return D2D1::ColorF(a.r + (b.r - a.r) * t, a.g + (b.g - a.g) * t, a.b + (b.b - a.b) * t, a.a + (b.a - a.a) * t);
}

D2D1_COLOR_F rd_alpha(D2D1_COLOR_F c, float factor) { return D2D1::ColorF(c.r, c.g, c.b, c.a * factor); }

D2D1_RECT_F rd_rect(const Rect& r) { return D2D1::RectF(r.x, r.y, r.right(), r.bottom()); }

// The colours of the approved design, light and dark.
struct RdPalette {
    bool dark = false;
    D2D1_COLOR_F mica, card, card_stroke, stroke2, text, text2, text3, accent, accent_text, subtle, field, field_line, dash, button_bottom;
    D2D1_COLOR_F ok, warn, warn_fill, error, error_fill, info_stroke;
};

RdPalette rd_palette(bool dark, std::uint32_t accent) {
    RdPalette p;
    p.dark = dark;
    if (!dark) {
        p.mica = rd_color(0xEDF1F9);
        p.card = rd_color(0xFBFBFB);
        p.card_stroke = rd_color(0x000000, 0.0578f);
        p.stroke2 = rd_color(0x000000, 0.08f);
        p.text = rd_color(0x1B1B1B);
        p.text2 = rd_color(0x5D5D5D);
        p.text3 = rd_color(0x8A8A8A);
        p.accent = rd_color(accent ? accent : 0x005FB8);
        p.accent_text = rd_color(0xFFFFFF);
        p.subtle = rd_color(0x000000, 0.0373f);
        p.field = rd_color(0xFFFFFF);
        p.field_line = rd_color(0x868686);
        p.dash = rd_color(0x9A9A9A);
        p.button_bottom = rd_color(0x000000, 0.16f);
        p.ok = rd_color(0x0F7B0F);
        p.warn = rd_color(0x9D5D00);
        p.warn_fill = rd_color(0xFFF4CE);
        p.error = rd_color(0xC42B1C);
        p.error_fill = rd_color(0xFDE7E9);
    } else {
        p.mica = rd_color(0x202328);
        p.card = rd_color(0x2B2B2B);
        p.card_stroke = rd_color(0xFFFFFF, 0.07f);
        p.stroke2 = rd_color(0xFFFFFF, 0.08f);
        p.text = rd_color(0xFFFFFF);
        p.text2 = rd_color(0xC5C5C5);
        p.text3 = rd_color(0x9A9A9A);
        p.accent = rd_color(accent ? accent : 0x60CDFF);
        p.accent_text = rd_color(0x000000);
        p.subtle = rd_color(0xFFFFFF, 0.06f);
        p.field = rd_color(0xFFFFFF, 0.06f);
        p.field_line = rd_color(0x9A9A9A);
        p.dash = rd_color(0x6F6F6F);
        p.button_bottom = rd_color(0xFFFFFF, 0.05f);
        p.ok = rd_color(0x6CCB5F);
        p.warn = rd_color(0xFCE100);
        p.warn_fill = rd_color(0x433519);
        p.error = rd_color(0xFF99A4);
        p.error_fill = rd_color(0x442726);
    }
    return p;
}

// Everything one painting needs: the target, one brush, the palette and the model.
struct RdDraw {
    ID2D1RenderTarget* rt;
    ComPtr<ID2D1SolidColorBrush> brush;
    Painter& painter;
    const RdPalette& pal;
    const Model& model;
    const Layout& layout;

    void fill(const Rect& r, float radius, D2D1_COLOR_F c) {
        brush->SetColor(c);
        if (radius > 0) rt->FillRoundedRectangle(D2D1::RoundedRect(rd_rect(r), radius, radius), brush.get());
        else rt->FillRectangle(rd_rect(r), brush.get());
    }
    // A line `width` thick drawn inside `r`.
    void stroke(const Rect& r, float radius, D2D1_COLOR_F c, float width = 1.0f, ID2D1StrokeStyle* style = nullptr) {
        brush->SetColor(c);
        const float h = width / 2;
        const D2D1_RECT_F in = D2D1::RectF(r.x + h, r.y + h, r.right() - h, r.bottom() - h);
        if (radius > 0) rt->DrawRoundedRectangle(D2D1::RoundedRect(in, std::max(0.0f, radius - h), std::max(0.0f, radius - h)), brush.get(), width, style);
        else rt->DrawRectangle(in, brush.get(), width, style);
    }
    void circle(const Rect& r, D2D1_COLOR_F c) {
        brush->SetColor(c);
        rt->FillEllipse(D2D1::Ellipse(D2D1::Point2F(r.x + r.w / 2, r.y + r.h / 2), r.w / 2, r.h / 2), brush.get());
    }
    void text(const std::wstring& s, const Rect& r, int kind, float size, int weight, D2D1_COLOR_F c, DWRITE_TEXT_ALIGNMENT align = DWRITE_TEXT_ALIGNMENT_LEADING,
              bool wrap = false, DWRITE_PARAGRAPH_ALIGNMENT vertical = DWRITE_PARAGRAPH_ALIGNMENT_CENTER) {
        if (s.empty()) return;
        IDWriteTextFormat* f = painter_format(kind, size, weight, align, vertical, wrap);
        if (f == nullptr) return;
        brush->SetColor(c);
        rt->DrawText(s.c_str(), static_cast<UINT32>(s.size()), f, rd_rect(r), brush.get(), D2D1_DRAW_TEXT_OPTIONS_CLIP);
    }
    void glyph(wchar_t code, const Rect& r, float size, D2D1_COLOR_F c) {
        const std::wstring s(1, code);
        text(s, r, 2, size, DWRITE_FONT_WEIGHT_NORMAL, c, DWRITE_TEXT_ALIGNMENT_CENTER, false);
    }
    IDWriteTextFormat* painter_format(int kind, float size, int weight, DWRITE_TEXT_ALIGNMENT align, DWRITE_PARAGRAPH_ALIGNMENT vertical, bool wrap);

    // ---- controls
    bool hot(Control c) const { return model.hot == c; }
    bool down(Control c) const { return model.pressed == c && model.hot == c; }
    void focus_ring(Control c, const Rect& r) {
        if (model.focus != c) return;
        if (c == Control::Plus) {
            brush->SetColor(pal.text);
            rt->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(r.x + r.w / 2, r.y + r.h / 2), r.w / 2 + 3, r.h / 2 + 3), brush.get(), 2.0f);
        } else if (c != Control::None) {
            stroke(Rect{r.x - 3, r.y - 3, r.w + 6, r.h + 6}, 6, pal.text, 2.0f);
        }
    }
    void button(Control c, const Rect& r, const std::wstring& label, wchar_t icon, bool enabled) {
        const float dim = enabled ? 1.0f : 0.45f;
        D2D1_COLOR_F fill = pal.field;
        if (enabled && down(c)) fill = rd_mix(fill, pal.text, pal.dark ? 0.04f : 0.05f);
        else if (enabled && hot(c)) fill = rd_mix(fill, pal.text, pal.dark ? 0.07f : 0.035f);
        if (pal.dark) fill = rd_alpha(rd_mix(rd_color(0x2B2B2B), D2D1::ColorF(1, 1, 1), enabled && down(c) ? 0.06f : enabled && hot(c) ? 0.11f : 0.08f), dim);
        else fill = rd_alpha(fill, dim);
        fill_button(r, fill);
        stroke(r, 4, rd_alpha(pal.stroke2, dim));
        brush->SetColor(rd_alpha(pal.button_bottom, dim));
        rt->DrawLine(D2D1::Point2F(r.x + 4, r.bottom() - 0.5f), D2D1::Point2F(r.right() - 4, r.bottom() - 0.5f), brush.get(), 1.0f);
        const float text_w = painter.measure(label, 14, false);
        const float content = (icon ? 24.0f : 0.0f) + text_w;
        float x = r.x + (r.w - content) / 2;
        if (icon) {
            glyph(icon, Rect{x, r.y, 16, r.h}, 16, rd_alpha(pal.text, dim));
            x += 24;
        }
        text(label, Rect{x, r.y, text_w + 4, r.h}, 0, 14, DWRITE_FONT_WEIGHT_NORMAL, rd_alpha(pal.text, dim));
        if (enabled) focus_ring(c, r);
    }
    void fill_button(const Rect& r, D2D1_COLOR_F c) { fill(r, 4, c); }
};

IDWriteTextFormat* RdDraw::painter_format(int kind, float size, int weight, DWRITE_TEXT_ALIGNMENT align, DWRITE_PARAGRAPH_ALIGNMENT vertical, bool wrap) {
    return painter.format_for(kind, size, weight, align, vertical, wrap);
}

}  // namespace

// ---- the painter ------------------------------------------------------------------------------------------------------------------------

Painter::Painter() {
    D2D1_FACTORY_OPTIONS options{};
    if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory), &options, reinterpret_cast<void**>(factory_.put())))) factory_.reset();
    if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(write_.put())))) write_.reset();
    if (!write_) return;
    ComPtr<IDWriteFontCollection> fonts;
    write_->GetSystemFontCollection(fonts.put(), FALSE);
    const auto pick = [&](std::initializer_list<const wchar_t*> names) -> std::wstring {
        for (const wchar_t* n : names) {
            UINT32 index = 0;
            BOOL exists = FALSE;
            if (fonts && SUCCEEDED(fonts->FindFamilyName(n, &index, &exists)) && exists) return n;
        }
        return *(names.end() - 1);
    };
    body_ = pick({L"Segoe UI Variable Text", L"Segoe UI"});
    display_ = pick({L"Segoe UI Variable Display", L"Segoe UI"});
    icons_ = pick({L"Segoe Fluent Icons", L"Segoe MDL2 Assets"});
}

std::wstring Painter::family(int kind) const { return kind == 1 ? display_ : kind == 2 ? icons_ : body_; }

IDWriteTextFormat* Painter::format_for(int kind, float size, int weight, int align, int vertical, bool wrap) {
    const FormatKey key{kind, static_cast<int>(size * 10), weight, align, vertical, wrap};
    const auto it = formats_.find(key);
    if (it != formats_.end()) return it->second.get();
    ComPtr<IDWriteTextFormat> f;
    if (FAILED(write_->CreateTextFormat(family(kind).c_str(), nullptr, static_cast<DWRITE_FONT_WEIGHT>(weight), DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, size,
                                        L"en-us", f.put())))
        return nullptr;
    f->SetTextAlignment(static_cast<DWRITE_TEXT_ALIGNMENT>(align));
    f->SetParagraphAlignment(static_cast<DWRITE_PARAGRAPH_ALIGNMENT>(vertical));
    f->SetWordWrapping(wrap ? DWRITE_WORD_WRAPPING_WRAP : DWRITE_WORD_WRAPPING_NO_WRAP);
    if (!wrap) {
        ComPtr<IDWriteInlineObject> sign;
        if (SUCCEEDED(write_->CreateEllipsisTrimmingSign(f.get(), sign.put()))) {
            const DWRITE_TRIMMING trimming{DWRITE_TRIMMING_GRANULARITY_CHARACTER, 0, 0};
            f->SetTrimming(&trimming, sign.get());
        }
    }
    IDWriteTextFormat* raw = f.get();
    formats_[key] = std::move(f);
    return raw;
}

float Painter::measure(const std::wstring& text, float size, bool semibold) const {
    if (text.empty() || !write_) return 0;
    ComPtr<IDWriteTextFormat> f;
    if (FAILED(write_->CreateTextFormat(family(0).c_str(), nullptr, semibold ? DWRITE_FONT_WEIGHT_SEMI_BOLD : DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL,
                                        DWRITE_FONT_STRETCH_NORMAL, size, L"en-us", f.put())))
        return 0;
    f->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
    ComPtr<IDWriteTextLayout> layout;
    if (FAILED(write_->CreateTextLayout(text.c_str(), static_cast<UINT32>(text.size()), f.get(), 4096.0f, 100.0f, layout.put()))) return 0;
    DWRITE_TEXT_METRICS m{};
    layout->GetMetrics(&m);
    return m.widthIncludingTrailingWhitespace;
}

ComPtr<ID2D1HwndRenderTarget> Painter::make_window_target(HWND window, UINT width, UINT height) const {
    ComPtr<ID2D1HwndRenderTarget> target;
    D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties();
    props.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED);
    D2D1_HWND_RENDER_TARGET_PROPERTIES hwnd_props = D2D1::HwndRenderTargetProperties(window, D2D1::SizeU(width, height));
    if (FAILED(factory_->CreateHwndRenderTarget(props, hwnd_props, target.put()))) target.reset();
    return target;
}

namespace {

// A trimmed-in-the-middle copy of `path` that fits `width` DIPs in the body font at `size`.
std::wstring rd_fit_middle(const Painter& painter, const std::wstring& path, float width, float size) {
    if (painter.measure(path, size, false) <= width) return path;
    std::size_t lo = 4, hi = path.size();
    std::wstring best = L"…";
    while (lo < hi) {
        const std::size_t keep = (lo + hi) / 2;
        const std::size_t head = keep / 2 + keep % 2, tail = keep - head;
        const std::wstring candidate = path.substr(0, head) + L"…" + path.substr(path.size() - tail);
        if (painter.measure(candidate, size, false) <= width) {
            best = candidate;
            lo = keep + 1;
        } else {
            hi = keep;
        }
    }
    return best;
}

}  // namespace

void Painter::paint(ID2D1RenderTarget* rt, const Model& model, const Layout& l, const Appearance& look) {
    if (!rt || !ok()) return;
    const RdPalette pal = rd_palette(look.dark, look.accent);
    RdDraw d{rt, {}, *this, pal, model, l};
    if (FAILED(rt->CreateSolidColorBrush(D2D1::ColorF(0, 0, 0, 1), d.brush.put()))) return;
    rt->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);

    // ---- the background: the backdrop shows through when the system has one, else the solid colour
    const Rect all{0, 0, l.width, l.height};
    rt->Clear(D2D1::ColorF(0, 0, 0, 0));
    if (!look.backdrop) d.fill(all, look.rounded_corners ? kCardRadius : 0.0f, pal.mica);

    // ---- the title bar
    {
        const Rect& ic = l.app_icon;
        d.fill(ic, 4, pal.accent);
        d.stroke(Rect{ic.x + 4, ic.y + 4, ic.w - 8, ic.h - 8}, 2, pal.mica, 2.0f);
        d.text(L"CodeBreaker", l.title_text, 0, 12, DWRITE_FONT_WEIGHT_NORMAL, model.window_active ? pal.text : pal.text3);
        const struct {
            Control id;
            const Rect* rect;
            wchar_t glyph;
        } caps[] = {{Control::CaptionMinimize, &l.caption_minimize, L''},
                    {Control::CaptionMaximize, &l.caption_maximize, model.maximized ? L'' : L''},
                    {Control::CaptionClose, &l.caption_close, L''}};
        for (const auto& c : caps) {
            D2D1_COLOR_F glyph_color = model.window_active ? pal.text : pal.text3;
            if (model.hot == c.id) {
                if (c.id == Control::CaptionClose) {
                    d.fill(*c.rect, 0, model.pressed == c.id ? rd_color(0xC42B1C, 0.9f) : rd_color(0xC42B1C));
                    glyph_color = D2D1::ColorF(1, 1, 1);
                } else {
                    d.fill(*c.rect, 0, rd_alpha(pal.dark ? D2D1::ColorF(1, 1, 1) : D2D1::ColorF(0, 0, 0), model.pressed == c.id ? 0.04f : 0.06f));
                }
            }
            d.glyph(c.glyph, *c.rect, 10, glyph_color);
        }
    }

    // ---- the cards
    for (const Rect* card : {&l.left_card, &l.right_card}) {
        d.fill(*card, kCardRadius, pal.card);
        d.stroke(*card, kCardRadius, pal.card_stroke);
    }
    d.text(L"Program", l.left_title, 1, 20, DWRITE_FONT_WEIGHT_SEMI_BOLD, pal.text);
    d.text(L"The file you want to turn back into source code.", l.left_desc, 0, 14, DWRITE_FONT_WEIGHT_NORMAL, pal.text2);
    d.text(L"Source", l.right_title, 1, 20, DWRITE_FONT_WEIGHT_SEMI_BOLD, pal.text);
    d.text(L"Where the finished source code goes.", l.right_desc, 0, 14, DWRITE_FONT_WEIGHT_NORMAL, pal.text2);

    // ---- the left card
    const View view = model.view;
    if (view == View::Unsupported || view == View::Failed) {
        const bool failed = view == View::Failed;
        const D2D1_COLOR_F tone = failed ? pal.error : pal.warn;
        d.fill(l.info_bar, 4, failed ? pal.error_fill : pal.warn_fill);
        d.stroke(l.info_bar, 4, pal.stroke2);
        d.glyph(failed ? L'' : L'', l.info_icon, 16, tone);
        d.text(widen(model.info_title), l.info_title, 0, 14, DWRITE_FONT_WEIGHT_SEMI_BOLD, pal.text);
        d.text(widen(model.info_message), l.info_message, 0, 14, DWRITE_FONT_WEIGHT_NORMAL, pal.text, DWRITE_TEXT_ALIGNMENT_LEADING, true,
               DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
        d.button(Control::TryAnother, l.try_another, L"Try another file", 0, true);
    }

    // the dashed drop area
    {
        ComPtr<ID2D1StrokeStyle> dashes;
        const float pattern[2] = {3.0f, 2.0f};
        D2D1_STROKE_STYLE_PROPERTIES sp = D2D1::StrokeStyleProperties(D2D1_CAP_STYLE_FLAT, D2D1_CAP_STYLE_FLAT, D2D1_CAP_STYLE_FLAT, D2D1_LINE_JOIN_MITER, 10.0f,
                                                                      D2D1_DASH_STYLE_CUSTOM, 0.0f);
        d2d()->CreateStrokeStyle(sp, pattern, 2, dashes.put());
        if (model.dragging) {
            d.fill(l.drop, 8, rd_alpha(pal.accent, 0.12f));
            d.stroke(l.drop, 8, pal.accent, 2.0f);
        } else {
            d.fill(l.drop, 8, pal.subtle);
            d.stroke(l.drop, 8, pal.dash, 2.0f, dashes.get());
        }
    }

    if (view == View::Working || view == View::Done) {
        const bool done = view == View::Done;
        d.circle(l.file_icon, done ? pal.ok : rd_alpha(pal.accent, 0.16f));
        d.glyph(done ? L'' : L'', l.file_icon, done ? 26.0f : 24.0f, done ? D2D1::ColorF(1, 1, 1) : pal.accent);
        d.text(widen(model.file_name), l.file_name, 0, 16, DWRITE_FONT_WEIGHT_SEMI_BOLD, pal.text, DWRITE_TEXT_ALIGNMENT_CENTER);
        if (!done) {
            d.fill(l.bar, 2, rd_alpha(pal.text3, 0.4f));
            const float filled = l.bar.w * static_cast<float>(std::clamp(model.percent, 0, 100)) / 100.0f;
            if (filled > 0) d.fill(Rect{l.bar.x, l.bar.y, std::max(filled, 4.0f), l.bar.h}, 2, pal.accent);
            d.text(widen(model.stage_label), l.bar_label, 0, 14, DWRITE_FONT_WEIGHT_NORMAL, pal.text);
            d.text(widen(std::to_string(model.percent) + "%"), l.bar_percent, 0, 14, DWRITE_FONT_WEIGHT_NORMAL, pal.text2, DWRITE_TEXT_ALIGNMENT_TRAILING);
            d.text(widen(model.detail), l.detail, 0, 12, DWRITE_FONT_WEIGHT_NORMAL, pal.text3);
            d.button(Control::Cancel, l.cancel, L"Cancel", L'', true);
        } else {
            d.text(L"Conversion finished", l.detail, 0, 13, DWRITE_FONT_WEIGHT_NORMAL, pal.text2, DWRITE_TEXT_ALIGNMENT_CENTER);
            d.button(Control::Browse, l.browse, L"Convert another file", L'', true);
        }
    } else {
        // the idle group: the round "+" button, two lines, Browse
        D2D1_COLOR_F plus = pal.accent;
        if (model.pressed == Control::Plus && model.hot == Control::Plus) plus = rd_mix(plus, pal.dark ? D2D1::ColorF(0, 0, 0) : D2D1::ColorF(0, 0, 0), 0.25f);
        else if (model.hot == Control::Plus) plus = rd_mix(plus, pal.dark ? D2D1::ColorF(1, 1, 1) : D2D1::ColorF(0, 0, 0), 0.10f);
        d.circle(l.plus, plus);
        d.glyph(L'', l.plus, 28, pal.accent_text);
        d.focus_ring(Control::Plus, l.plus);
        d.text(L"Drag a file here", l.drop_line1, 0, 15, DWRITE_FONT_WEIGHT_SEMI_BOLD, pal.text, DWRITE_TEXT_ALIGNMENT_CENTER);
        d.text(L".exe, .dll, ROMs, disc images and more", l.drop_line2, 0, 13, DWRITE_FONT_WEIGHT_NORMAL, pal.text2, DWRITE_TEXT_ALIGNMENT_CENTER);
        d.button(Control::Browse, l.browse, L"Browse…", L'', true);
    }

    // ---- the right card
    {
        const bool done = view == View::Done;
        d.text(L"Output", l.output_label, 0, 14, DWRITE_FONT_WEIGHT_NORMAL, pal.text);
        d.fill(l.field, 4, pal.field);
        d.stroke(l.field, 4, pal.stroke2);
        d.brush->SetColor(pal.field_line);
        rt->DrawLine(D2D1::Point2F(l.field.x + 4, l.field.bottom() - 0.5f), D2D1::Point2F(l.field.right() - 4, l.field.bottom() - 0.5f), d.brush.get(), 1.0f);
        if (done)
            d.text(rd_fit_middle(*this, widen(model.output_path), l.field.w - 22, 14), Rect{l.field.x + 11, l.field.y, l.field.w - 22, l.field.h}, 0, 14,
                   DWRITE_FONT_WEIGHT_NORMAL, pal.text);
        d.button(Control::OpenFolder, l.open_folder, L"Open folder", L'', done);
        d.button(Control::CopyPath, l.copy_path, L"Copy path", L'', done);
        d.text(done ? widen(model.done_note) : std::wstring(L"Appears here when the conversion is finished."), l.hint, 0, 12, DWRITE_FONT_WEIGHT_NORMAL,
               done ? pal.text2 : pal.text3);
    }

    // ---- the status bar
    {
        d.brush->SetColor(pal.stroke2);
        rt->DrawLine(D2D1::Point2F(l.status_bar.x, l.status_bar.y + 0.5f), D2D1::Point2F(l.status_bar.right(), l.status_bar.y + 0.5f), d.brush.get(), 1.0f);
        D2D1_COLOR_F dot = pal.ok;
        if (view == View::Working) dot = pal.accent;
        else if (view == View::Unsupported) dot = pal.warn;
        else if (view == View::Failed) dot = pal.error;
        d.circle(l.status_dot, dot);
        d.text(widen(model.status_text), l.status_text, 0, 12, DWRITE_FONT_WEIGHT_NORMAL, pal.text2);
        d.text(widen(model.platforms), l.platforms, 0, 12, DWRITE_FONT_WEIGHT_NORMAL, pal.text2, DWRITE_TEXT_ALIGNMENT_TRAILING);
    }
}

}  // namespace cb::app::window
#endif  // _WIN32
