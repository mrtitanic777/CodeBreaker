// app/window_render_win.cpp -- the CodeBreaker window drawn with Direct2D and DirectWrite (see window_render_win.hpp). The look is the approved
// Windows 11 design (work/ui-mockup/win11.html): Segoe UI Variable text, Segoe Fluent Icons glyphs, Fluent card, button, field, progress bar and
// InfoBar styling, light and dark.
#ifdef _WIN32
#include "app/window_render_win.hpp"

#include "app/window_convert.hpp"
#include "app/window_functions.hpp"
#include "app/window_pages.hpp"

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


// ---- the rail, the Tools page and the Settings page ----------------------------------------------------------------------------------------

// A drop-down list over the page: the rows of `items`, the chosen one marked, the one under the pointer or the keyboard lit.
void rd_popup(RdDraw& d, const LanguageList& list, const std::vector<LanguageItem>& items, int selected, int hot) {
    const RdPalette& pal = d.pal;
    for (int i = 3; i >= 1; --i)   // a soft shadow
        d.fill(Rect{list.outline.x - static_cast<float>(i), list.outline.y - static_cast<float>(i) + 2, list.outline.w + 2.0f * static_cast<float>(i), list.outline.h + 2.0f * static_cast<float>(i)},
               kCardRadius, rd_color(0x000000, pal.dark ? 0.10f : 0.045f));
    d.fill(list.outline, kCardRadius, pal.dark ? rd_color(0x323232) : rd_color(0xFCFCFC));
    d.stroke(list.outline, kCardRadius, pal.dark ? rd_color(0xFFFFFF, 0.12f) : rd_color(0x000000, 0.12f));
    for (std::size_t i = 0; i < list.rows.size() && i < items.size(); ++i) {
        const LanguageItem& item = items[i];
        const Rect& row = list.rows[i];
        const bool chosen = static_cast<int>(i) == selected;
        if (item.supported && static_cast<int>(i) == hot) d.fill(row, 4, rd_alpha(pal.text, pal.dark ? 0.09f : 0.06f));
        else if (chosen) d.fill(row, 4, rd_alpha(pal.text, pal.dark ? 0.06f : 0.04f));
        if (chosen) d.fill(Rect{row.x, row.y + (row.h > kLanguageRowHeight ? 8.0f : (row.h - 16) / 2), 3, 16}, 1.5f, pal.accent);
        const bool reason = !item.supported && !item.reason.empty();
        d.text(widen(item.name), Rect{row.x + 12, row.y + (reason ? 4.0f : 0.0f), row.w - 24, reason ? 22.0f : row.h}, 0, 14, DWRITE_FONT_WEIGHT_NORMAL, item.supported ? pal.text : pal.text3);
        if (reason) d.text(widen(item.reason), Rect{row.x + 12, row.y + 26, row.w - 24, 16}, 0, 12, DWRITE_FONT_WEIGHT_NORMAL, pal.text3);
    }
}

// A text box: the field, its bottom line (accent while it has the keyboard), its text or its grey hint, the caret.
void rd_text_box(RdDraw& d, const Rect& r, const std::wstring& text, const std::wstring& hint, bool focused, int caret_chars) {
    const RdPalette& pal = d.pal;
    d.fill(r, 4, pal.field);
    d.stroke(r, 4, pal.stroke2);
    d.brush->SetColor(focused ? pal.accent : pal.field_line);
    d.rt->DrawLine(D2D1::Point2F(r.x + 4, r.bottom() - 0.5f), D2D1::Point2F(r.right() - 4, r.bottom() - 0.5f), d.brush.get(), focused ? 2.0f : 1.0f);
    const Rect inner{r.x + 10, r.y, std::max(0.0f, r.w - 20), r.h};
    const float width = d.painter.measure(text, 14, false);
    const bool overflow = width > inner.w;
    if (text.empty()) d.text(hint, inner, 0, 14, DWRITE_FONT_WEIGHT_NORMAL, pal.text3);
    else d.text(text, inner, 0, 14, DWRITE_FONT_WEIGHT_NORMAL, pal.text, overflow ? DWRITE_TEXT_ALIGNMENT_TRAILING : DWRITE_TEXT_ALIGNMENT_LEADING);   // (too long: its end shows)
    if (focused) {
        const std::wstring before = text.substr(0, static_cast<std::size_t>(std::clamp(caret_chars, 0, static_cast<int>(text.size()))));
        float x = inner.x + std::min(d.painter.measure(before, 14, false), inner.w);
        if (overflow) x = inner.right();
        d.brush->SetColor(pal.text);
        d.rt->DrawLine(D2D1::Point2F(x + 0.5f, r.y + 7), D2D1::Point2F(x + 0.5f, r.bottom() - 7), d.brush.get(), 1.0f);
    }
}

// A Fluent switch.
void rd_switch(RdDraw& d, const Rect& sw, bool on, bool focused) {
    const RdPalette& pal = d.pal;
    if (on) d.fill(sw, 10, pal.accent);
    else d.stroke(sw, 10, pal.field_line, 1.0f);
    const float knob = on ? 12.0f : 10.0f;
    const float kx = on ? sw.right() - 5 - knob : sw.x + 5;
    d.circle(Rect{kx, sw.y + (sw.h - knob) / 2, knob, knob}, on ? pal.accent_text : pal.text2);
    if (focused) d.stroke(Rect{sw.x - 3, sw.y - 3, sw.w + 6, sw.h + 6}, 12, pal.text, 2.0f);
}

// A closed drop-down: the chosen text and a chevron.
void rd_combo(RdDraw& d, const Rect& r, const std::wstring& shown, bool open, bool hot) {
    const RdPalette& pal = d.pal;
    D2D1_COLOR_F fill = pal.field;
    if (hot || open) fill = rd_mix(fill, pal.text, pal.dark ? 0.04f : 0.035f);
    d.fill(r, 4, fill);
    d.stroke(r, 4, pal.stroke2);
    d.brush->SetColor(open ? pal.accent : pal.field_line);
    d.rt->DrawLine(D2D1::Point2F(r.x + 4, r.bottom() - 0.5f), D2D1::Point2F(r.right() - 4, r.bottom() - 0.5f), d.brush.get(), open ? 2.0f : 1.0f);
    d.text(shown, Rect{r.x + 11, r.y, r.w - 44, r.h}, 0, 14, DWRITE_FONT_WEIGHT_NORMAL, pal.text);
    d.glyph(open ? L'' : L'', Rect{r.right() - 30, r.y, 20, r.h}, 12, pal.text2);
}

// The character stream behind a running conversion: the newest lines, monospace, in the accent colour at low strength, bottom-aligned in the drop
// area and fading out toward its top, with a block cursor after the line being typed. `keep_clear` is the progress group (icon to Cancel): lines
// that pass behind it fade almost away (smoothly, over a short distance), so the stream never competes with what a person reads.
void rd_stream(RdDraw& d, const Rect& drop, const Rect& keep_clear) {
    const Model& m = d.model;
    const RdPalette& pal = d.pal;
    if (m.stream.empty()) return;
    constexpr float kSize = 11.5f, kLine = 16.0f, kInset = 14.0f;
    const float char_w = kSize * 0.6f;   // (Cascadia Mono and Consolas advance 0.6 em)
    const Rect area{drop.x + kInset, drop.y + kInset, drop.w - 2 * kInset, drop.h - 2 * kInset};
    const std::size_t rows = static_cast<std::size_t>(std::max(1.0f, std::floor(area.h / kLine)));
    const std::size_t first = m.stream.size() > rows ? m.stream.size() - rows : 0;
    const float strength = pal.dark ? 0.34f : 0.30f;
    d.rt->PushAxisAlignedClip(rd_rect(area), D2D1_ANTIALIAS_MODE_ALIASED);
    float y = area.bottom() - static_cast<float>(m.stream.size() - first) * kLine;
    for (std::size_t i = first; i < m.stream.size(); ++i, y += kLine) {
        const float from_top = std::clamp((y - area.y) / (area.h * 0.45f), 0.0f, 1.0f);   // the top 45% fades out
        // behind the progress group: down to 15%, easing in over 32 DIPs above and below it
        const float mid = y + kLine / 2;
        const float outside = std::max(keep_clear.y - mid, mid - keep_clear.bottom());   // < 0 inside the group
        const float behind = 0.15f + 0.85f * std::clamp(outside / 32.0f, 0.0f, 1.0f);
        const D2D1_COLOR_F c = rd_alpha(pal.accent, strength * (0.15f + 0.85f * from_top) * behind);
        d.text(widen(m.stream[i]), Rect{area.x, y, area.w, kLine}, 3, kSize, DWRITE_FONT_WEIGHT_NORMAL, c);
        if (i + 1 == m.stream.size() && m.stream_cursor) {
            const float x = area.x + static_cast<float>(widen(m.stream[i]).size()) * char_w;
            if (x + char_w <= area.right()) d.fill(Rect{x + 1, y + 2, char_w, kLine - 4}, 0, rd_alpha(pal.accent, strength * 1.8f * behind));
        }
    }
    d.rt->PopAxisAlignedClip();
}

// The fields of a form, clipped to `clip`: labels (a required one marked), text boxes, Browse buttons, drop-downs, switches.
void rd_form(RdDraw& d, const FormLayout& form, const std::vector<FieldItem>& fields, const Rect& clip, const Model& m) {
    const RdPalette& pal = d.pal;
    d.rt->PushAxisAlignedClip(rd_rect(clip), D2D1_ANTIALIAS_MODE_ALIASED);
    for (std::size_t i = 0; i < fields.size() && i < form.fields.size(); ++i) {
        const FieldItem& f = fields[i];
        const FieldRects& r = form.fields[i];
        const int idx = static_cast<int>(i);
        const bool focused = m.focus == Control::Field && m.focus_index == idx;
        if (r.label.bottom() < clip.y || r.label.y > clip.bottom()) continue;
        if (f.kind == 4) {
            rd_switch(d, r.input, !f.text.empty(), focused);
            d.text(widen(f.label), r.label, 0, 14, DWRITE_FONT_WEIGHT_NORMAL, pal.text);
            if (r.note.h > 0) d.text(widen(f.note), r.note, 0, 12, DWRITE_FONT_WEIGHT_NORMAL, pal.text3);
            continue;
        }
        d.text(widen(f.label), r.label, 0, 14, DWRITE_FONT_WEIGHT_NORMAL, pal.text);
        if (r.note.h > 0) d.text(widen(f.note), r.note, 0, 12, DWRITE_FONT_WEIGHT_NORMAL, pal.text3);
        if (f.required) d.text(L"*", Rect{r.label.x + d.painter.measure(widen(f.label), 14, false) + 4, r.label.y, 12, r.label.h}, 0, 14, DWRITE_FONT_WEIGHT_SEMI_BOLD, pal.error);
        if (f.kind == 3) {
            rd_combo(d, r.input, widen(f.text), m.field_popup == idx, m.hot == Control::Field && m.hot_index == idx);
            if (focused) d.stroke(Rect{r.input.x - 3, r.input.y - 3, r.input.w + 6, r.input.h + 6}, 6, pal.text, 2.0f);
        } else {
            rd_text_box(d, r.input, widen(f.text), widen(f.hint), focused, focused ? static_cast<int>(widen(f.text.substr(0, static_cast<std::size_t>(std::max(0, m.caret)))).size()) : 0);
        }
        if (r.browse.w > 0) {
            const bool hot = m.hot == Control::FieldBrowse && m.hot_index == idx;
            D2D1_COLOR_F fill = pal.dark ? rd_mix(rd_color(0x2B2B2B), D2D1::ColorF(1, 1, 1), hot ? 0.11f : 0.08f) : rd_mix(pal.field, pal.text, hot ? 0.035f : 0.0f);
            d.fill(r.browse, 4, fill);
            d.stroke(r.browse, 4, pal.stroke2);
            const float text_w = d.painter.measure(L"Browse…", 14, false);
            const float x = r.browse.x + (r.browse.w - 24 - text_w) / 2;
            d.glyph(L'', Rect{x, r.browse.y, 16, r.browse.h}, 16, pal.text);
            d.text(L"Browse…", Rect{x + 24, r.browse.y, text_w + 4, r.browse.h}, 0, 14, DWRITE_FONT_WEIGHT_NORMAL, pal.text);
            if (m.focus == Control::FieldBrowse && m.focus_index == idx) d.stroke(Rect{r.browse.x - 3, r.browse.y - 3, r.browse.w + 6, r.browse.h + 6}, 6, pal.text, 2.0f);
        }
    }
    d.rt->PopAxisAlignedClip();
}

void rd_scrollbar(RdDraw& d, const Rect& area, float content, float scroll) {
    if (content <= area.h || area.h <= 0) return;
    const float thumb_h = std::max(24.0f, area.h * area.h / content);
    const float track = area.h - thumb_h;
    const float at = track * std::clamp(scroll / std::max(1.0f, content - area.h), 0.0f, 1.0f);
    d.fill(Rect{area.right() - 4, area.y + at, 3, thumb_h}, 1.5f, rd_alpha(d.pal.text3, 0.6f));
}

void rd_rail(RdDraw& d, const Layout& l, const Model& m) {
    const RdPalette& pal = d.pal;
    const struct {
        Control id;
        Page page;
        const Rect* rect;
        wchar_t glyph;
        const wchar_t* label;
    } items[] = {{Control::NavConvert, Page::Convert, &l.rail_convert, L'', L"Convert"},
                 {Control::NavTools, Page::Tools, &l.rail_tools, L'', L"Tools"},
                 {Control::NavSettings, Page::Settings, &l.rail_settings, L'', L"Settings"}};
    for (const auto& it : items) {
        const Rect& r = *it.rect;
        const bool on = m.page == it.page;
        if (on) d.fill(r, 6, rd_alpha(pal.text, pal.dark ? 0.07f : 0.055f));
        else if (m.hot == it.id) d.fill(r, 6, rd_alpha(pal.text, pal.dark ? 0.05f : 0.035f));
        if (on) d.fill(Rect{r.x, r.y + 18, 3, 20}, 1.5f, pal.accent);
        d.glyph(it.glyph, Rect{r.x, r.y + 8, r.w, 22}, 18, pal.text);
        d.text(it.label, Rect{r.x - 4, r.y + 32, r.w + 8, 16}, 0, 10, DWRITE_FONT_WEIGHT_NORMAL, on ? pal.text : pal.text2, DWRITE_TEXT_ALIGNMENT_CENTER);
        if (it.id == Control::NavTools && m.activity) d.circle(Rect{r.right() - 14, r.y + 6, 8, 8}, pal.accent);   // a tool is running
        if (m.focus == it.id) d.stroke(Rect{r.x - 1, r.y - 1, r.w + 2, r.h + 2}, 7, pal.text, 2.0f);
    }
}

void rd_tools(RdDraw& d, const Layout& l, const Model& m, Painter& painter) {
    const RdPalette& pal = d.pal;
    const ToolsLayout t = compute_tools_layout(l, m);
    for (const Rect* card : {&t.list_card, &t.form_card}) {
        d.fill(*card, kCardRadius, pal.card);
        d.stroke(*card, kCardRadius, pal.card_stroke);
    }
    // ---- the list
    d.text(L"Tools", t.list_title, 1, 20, DWRITE_FONT_WEIGHT_SEMI_BOLD, pal.text);
    {
        const bool focused = m.focus == Control::ToolSearch;
        d.fill(t.search, 4, pal.field);
        d.stroke(t.search, 4, pal.stroke2);
        d.brush->SetColor(focused ? pal.accent : pal.field_line);
        d.rt->DrawLine(D2D1::Point2F(t.search.x + 4, t.search.bottom() - 0.5f), D2D1::Point2F(t.search.right() - 4, t.search.bottom() - 0.5f), d.brush.get(), focused ? 2.0f : 1.0f);
        d.glyph(L'', Rect{t.search.x + 10, t.search.y, 16, t.search.h}, 14, pal.text3);
        const Rect inner{t.search.x + 34, t.search.y, t.search.w - 44, t.search.h};
        if (m.tool_query.empty()) d.text(widen(m.tools_count_text), inner, 0, 14, DWRITE_FONT_WEIGHT_NORMAL, pal.text3);
        else d.text(widen(m.tool_query), inner, 0, 14, DWRITE_FONT_WEIGHT_NORMAL, pal.text);
        if (focused) {
            const std::wstring q = widen(m.tool_query);
            const std::size_t chars = widen(m.tool_query.substr(0, static_cast<std::size_t>(std::max(0, m.caret)))).size();
            const float x = inner.x + std::min(painter.measure(q.substr(0, std::min(chars, q.size())), 14, false), inner.w);
            d.brush->SetColor(pal.text);
            d.rt->DrawLine(D2D1::Point2F(x + 0.5f, t.search.y + 7), D2D1::Point2F(x + 0.5f, t.search.bottom() - 7), d.brush.get(), 1.0f);
        }
    }
    d.rt->PushAxisAlignedClip(rd_rect(t.list_area), D2D1_ANTIALIAS_MODE_ALIASED);
    for (std::size_t i = 0; i < m.tool_rows.size() && i < t.rows.size(); ++i) {
        const ToolRowItem& row = m.tool_rows[i];
        const Rect& r = t.rows[i];
        if (r.bottom() < t.list_area.y || r.y > t.list_area.bottom()) continue;
        if (row.heading) {
            d.text(widen(row.text), Rect{r.x + 12, r.y + 8, r.w - 24, r.h - 8}, 0, 12, DWRITE_FONT_WEIGHT_SEMI_BOLD, pal.text2, DWRITE_TEXT_ALIGNMENT_LEADING, false, DWRITE_PARAGRAPH_ALIGNMENT_FAR);
            continue;
        }
        const bool selected = row.tool == m.tool_selected;
        if (selected) d.fill(r, 4, rd_alpha(pal.text, pal.dark ? 0.07f : 0.055f));
        else if (m.hot == Control::ToolRow && m.hot_index == row.tool) d.fill(r, 4, rd_alpha(pal.text, pal.dark ? 0.04f : 0.03f));
        if (selected) d.fill(Rect{r.x, r.y + 9, 3, 16}, 1.5f, pal.accent);
        d.text(widen(row.text), Rect{r.x + 12, r.y, r.w - 24, r.h}, 0, 14, DWRITE_FONT_WEIGHT_NORMAL, pal.text);
        if (m.focus == Control::ToolRow && m.focus_index == row.tool) d.stroke(Rect{r.x + 1, r.y + 1, r.w - 2, r.h - 2}, 4, pal.text, 2.0f);
    }
    d.rt->PopAxisAlignedClip();
    rd_scrollbar(d, t.list_area, t.list_content_height, m.tool_scroll);

    // ---- the selected tool
    if (m.tool_selected < 0) {
        d.text(L"Choose a tool from the list.", t.form_title, 0, 14, DWRITE_FONT_WEIGHT_NORMAL, pal.text2);
        return;
    }
    d.text(widen(m.tool_title), t.form_title, 1, 20, DWRITE_FONT_WEIGHT_SEMI_BOLD, pal.text);
    d.text(widen(m.tool_description), t.form_desc, 0, 14, DWRITE_FONT_WEIGHT_NORMAL, pal.text2, DWRITE_TEXT_ALIGNMENT_LEADING, true, DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
    rd_form(d, t.form, m.tool_fields, t.form_card, m);
    // Run (Stop while it runs): the one accent button
    {
        const Rect& r = t.run;
        const bool enabled = m.tool_can_run || m.tool_running;
        const float dim = enabled ? 1.0f : 0.45f;
        D2D1_COLOR_F fill = pal.accent;
        if (enabled && m.pressed == Control::Run) fill = rd_mix(fill, D2D1::ColorF(0, 0, 0), 0.25f);
        else if (enabled && m.hot == Control::Run) fill = rd_mix(fill, pal.dark ? D2D1::ColorF(1, 1, 1) : D2D1::ColorF(0, 0, 0), 0.10f);
        d.fill(r, 4, rd_alpha(fill, dim));
        const std::wstring label = m.tool_running ? L"Stop" : L"Run";
        d.glyph(m.tool_running ? L'' : L'', Rect{r.x + 14, r.y, 16, r.h}, 14, rd_alpha(pal.accent_text, dim));
        d.text(label, Rect{r.x + 38, r.y, r.w - 44, r.h}, 0, 14, DWRITE_FONT_WEIGHT_NORMAL, rd_alpha(pal.accent_text, dim));
        if (enabled && m.focus == Control::Run) d.stroke(Rect{r.x - 3, r.y - 3, r.w + 6, r.h + 6}, 6, pal.text, 2.0f);
        d.text(widen(m.tool_note), t.run_note, 0, 12, DWRITE_FONT_WEIGHT_NORMAL, pal.text3);
    }
    // the thin progress bar: determinate when the command prints percentages, else a moving segment while it runs, full when it has finished
    {
        d.fill(t.bar, 1.5f, rd_alpha(pal.text3, 0.3f));
        if (m.tool_running && m.tool_percent < 0) {
            const float seg = t.bar.w * 0.3f;
            d.fill(Rect{t.bar.x + (t.bar.w - seg) * m.tool_phase, t.bar.y, seg, t.bar.h}, 1.5f, pal.accent);
        } else if (m.tool_running || m.tool_done) {
            const float f = m.tool_running ? static_cast<float>(std::clamp(m.tool_percent, 0, 100)) / 100.0f : 1.0f;
            if (f > 0) d.fill(Rect{t.bar.x, t.bar.y, std::max(3.0f, t.bar.w * f), t.bar.h}, 1.5f, pal.accent);
        }
    }
    // the output pane: monospace, the most recent lines that fit (scrolled up by output_scroll lines)
    {
        const Rect& o = t.output;
        d.fill(o, 6, pal.dark ? rd_color(0x1F1F1F) : rd_color(0xF6F6F6));
        d.stroke(o, 6, pal.stroke2);
        const float line_h = 19.0f;
        const int capacity = std::max(1, static_cast<int>((o.h - 20) / line_h));
        std::vector<std::string> lines;
        std::size_t at = 0;
        while (at <= m.tool_output.size()) {
            std::size_t end = m.tool_output.find('\n', at);
            if (end == std::string::npos) end = m.tool_output.size();
            lines.push_back(m.tool_output.substr(at, end - at));
            at = end + 1;
        }
        if (!lines.empty() && lines.back().empty()) lines.pop_back();
        const int skip = std::clamp(static_cast<int>(m.output_scroll), 0, std::max(0, static_cast<int>(lines.size()) - capacity));
        const int last = static_cast<int>(lines.size()) - skip;
        const int first = std::max(0, last - capacity);
        float y = o.y + 10;
        for (int i = first; i < last; ++i) {
            const std::string& line = lines[static_cast<std::size_t>(i)];
            const bool ok = line.rfind("100%  Done", 0) == 0 || line.rfind("Checked:", 0) == 0 || line.find("identical") != std::string::npos;
            d.text(widen(line), Rect{o.x + 14, y, o.w - 28, line_h}, 3, 12.5f, DWRITE_FONT_WEIGHT_NORMAL, ok ? pal.ok : pal.text2);
            y += line_h;
        }
    }
}

// A plain button (Change..., Locate...): the field fill, a thin outline, an optional glyph and its words centred.
void rd_plain_button(RdDraw& d, const Rect& r, wchar_t glyph, const std::wstring& label, bool hot, bool focused) {
    const RdPalette& pal = d.pal;
    D2D1_COLOR_F fill = pal.dark ? rd_mix(rd_color(0x2B2B2B), D2D1::ColorF(1, 1, 1), hot ? 0.11f : 0.08f) : rd_mix(pal.field, pal.text, hot ? 0.035f : 0.0f);
    d.fill(r, 4, fill);
    d.stroke(r, 4, pal.stroke2);
    const float text_w = d.painter.measure(label, 14, false);
    const float glyph_w = glyph != 0 ? 24.0f : 0.0f;
    const float x = r.x + (r.w - glyph_w - text_w) / 2;
    if (glyph != 0) d.glyph(glyph, Rect{x, r.y, 16, r.h}, 14, pal.text);
    d.text(label, Rect{x + glyph_w, r.y, text_w + 4, r.h}, 0, 14, DWRITE_FONT_WEIGHT_NORMAL, pal.text);
    if (focused) d.stroke(Rect{r.x - 3, r.y - 3, r.w + 6, r.h + 6}, 6, pal.text, 2.0f);
}

void rd_settings(RdDraw& d, const Layout& l, const Model& m) {
    const RdPalette& pal = d.pal;
    const SettingsLayout s = compute_settings_layout(l, m);
    d.fill(s.card, kCardRadius, pal.card);
    d.stroke(s.card, kCardRadius, pal.card_stroke);
    if (m.settings_advanced) {
        const bool hot = m.hot == Control::SettingsBack;
        if (hot) d.fill(s.back, 4, rd_alpha(pal.text, pal.dark ? 0.06f : 0.04f));
        d.glyph(L'\xE72B', s.back, 16, pal.text);
        if (m.focus == Control::SettingsBack) d.stroke(s.back, 4, pal.text, 2.0f);
        d.text(L"Advanced", s.title, 1, 20, DWRITE_FONT_WEIGHT_SEMI_BOLD, pal.text);
        d.text(L"Tool locations, limits and options for developers. A change is saved when you make it.", s.desc, 0, 14, DWRITE_FONT_WEIGHT_NORMAL, pal.text2);
        rd_form(d, s.form, m.setting_fields, s.fields_area, m);
        rd_scrollbar(d, s.fields_area, s.form.content_height, m.setting_scroll);
        return;
    }
    d.text(L"Settings", s.title, 1, 20, DWRITE_FONT_WEIGHT_SEMI_BOLD, pal.text);
    d.text(L"Changes are saved as you make them.", s.desc, 0, 14, DWRITE_FONT_WEIGHT_NORMAL, pal.text2);

    const D2D1_COLOR_F row_fill = rd_alpha(pal.text, pal.dark ? 0.06f : 0.0373f);
    d.rt->PushAxisAlignedClip(rd_rect(s.fields_area), D2D1_ANTIALIAS_MODE_ALIASED);
    d.text(L"General", s.general_heading, 0, 12, DWRITE_FONT_WEIGHT_SEMI_BOLD, pal.text2);
    for (std::size_t i = 0; i < m.setting_fields.size() && i < s.form.fields.size() && i < s.field_rows.size(); ++i) {
        const FieldItem& f = m.setting_fields[i];
        const FieldRects& r = s.form.fields[i];
        const int idx = static_cast<int>(i);
        const bool focused = m.focus == Control::Field && m.focus_index == idx;
        d.fill(s.field_rows[i], 6, row_fill);
        d.text(widen(f.label), r.label, 0, 14, DWRITE_FONT_WEIGHT_NORMAL, pal.text);
        const bool path = f.kind == 0 || f.kind == 1;
        const std::string note = path ? (f.text.empty() ? f.hint : f.text) : f.note;
        d.text(widen(note), r.note, 0, 12, DWRITE_FONT_WEIGHT_NORMAL, pal.text2);
        if (f.kind == 3) {
            rd_combo(d, r.input, widen(f.text), m.field_popup == idx, m.hot == Control::Field && m.hot_index == idx);
            if (focused) d.stroke(Rect{r.input.x - 3, r.input.y - 3, r.input.w + 6, r.input.h + 6}, 6, pal.text, 2.0f);
        } else if (f.kind == 4) {
            rd_switch(d, r.input, !f.text.empty(), focused);
        } else if (path) {
            rd_plain_button(d, r.browse, L'\xE8B7', L"Change\x2026", m.hot == Control::FieldBrowse && m.hot_index == idx,
                            m.focus == Control::FieldBrowse && m.focus_index == idx);
        } else {
            rd_text_box(d, r.input, widen(f.text), widen(f.hint), focused, focused ? static_cast<int>(widen(f.text.substr(0, static_cast<std::size_t>(std::max(0, m.caret)))).size()) : 0);
        }
    }
    d.text(L"What this computer can convert", s.can_heading, 0, 12, DWRITE_FONT_WEIGHT_SEMI_BOLD, pal.text2);
    const auto icon_row = [&](const Rect& row, wchar_t glyph, D2D1_COLOR_F tone, const std::string& label, const std::string& note, float right_w) {
        d.fill(row, 6, row_fill);
        d.glyph(glyph, Rect{row.x + 16, row.y, 16, row.h}, 16, tone);
        const float w = std::max(0.0f, row.w - 44 - 16 - right_w);
        d.text(widen(label), Rect{row.x + 44, row.y + 10, w, 20}, 0, 14, DWRITE_FONT_WEIGHT_NORMAL, pal.text);
        d.text(widen(note), Rect{row.x + 44, row.y + 31, w, 17}, 0, 12, DWRITE_FONT_WEIGHT_NORMAL, pal.text2);
    };
    for (std::size_t i = 0; i < m.capabilities.size() && i < s.capability_rows.size(); ++i) {
        const CapabilityItem& c = m.capabilities[i];
        const Rect& locate = s.locate[i];
        icon_row(s.capability_rows[i], c.ready ? L'\xE73E' : L'\xE7BA', c.ready ? pal.ok : pal.warn, c.label, c.note, locate.w > 0 ? locate.w + 16 : 0.0f);
        if (locate.w > 0)
            rd_plain_button(d, locate, 0, L"Locate\x2026", m.hot == Control::Locate && m.hot_index == static_cast<int>(i),
                            m.focus == Control::Locate && m.focus_index == static_cast<int>(i));
    }
    {
        const Rect& r = s.advanced_row;
        icon_row(r, L'\xE713', pal.text2, "Advanced", "Tool locations, limits and options for developers", 32);
        if (m.hot == Control::SettingsAdvanced) d.fill(r, 6, rd_alpha(pal.text, pal.dark ? 0.04f : 0.03f));
        d.glyph(L'\xE76C', Rect{r.right() - 40, r.y, 24, r.h}, 12, pal.text2);
        if (m.focus == Control::SettingsAdvanced) d.stroke(Rect{r.x + 1, r.y + 1, r.w - 2, r.h - 2}, 6, pal.text, 2.0f);
    }
    d.rt->PopAxisAlignedClip();
    rd_scrollbar(d, s.fields_area, s.form.content_height, m.setting_scroll);
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
    mono_ = pick({L"Cascadia Mono", L"Consolas", L"Courier New"});
}

std::wstring Painter::family(int kind) const { return kind == 1 ? display_ : kind == 2 ? icons_ : kind == 3 ? mono_ : body_; }

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

// ---- the Convert page's lists: Recent, the batch queue, the output box and the preview ------------------------------------------------------------

namespace {

ComPtr<ID2D1StrokeStyle> rd_dashes(RdDraw& d, float on, float off) {
    ComPtr<ID2D1StrokeStyle> style;
    const float pattern[2] = {on, off};
    const D2D1_STROKE_STYLE_PROPERTIES sp = D2D1::StrokeStyleProperties(D2D1_CAP_STYLE_FLAT, D2D1_CAP_STYLE_FLAT, D2D1_CAP_STYLE_FLAT, D2D1_LINE_JOIN_MITER, 10.0f,
                                                                        D2D1_DASH_STYLE_CUSTOM, 0.0f);
    d.painter.d2d()->CreateStrokeStyle(sp, pattern, 2, style.put());
    return style;
}

// A small button of a list row (Remove): a Fluent button without an icon; `hot` is whether the pointer is over this one.
void rd_row_button(RdDraw& d, const Rect& r, const std::wstring& label, bool hot) {
    const RdPalette& pal = d.pal;
    D2D1_COLOR_F fill = pal.dark ? rd_mix(rd_color(0x2B2B2B), D2D1::ColorF(1, 1, 1), hot ? 0.11f : 0.08f) : rd_mix(pal.field, pal.text, hot ? 0.035f : 0.0f);
    d.fill(r, 4, fill);
    d.stroke(r, 4, pal.stroke2);
    d.text(label, r, 0, 13, DWRITE_FONT_WEIGHT_NORMAL, pal.text, DWRITE_TEXT_ALIGNMENT_CENTER);
}

// "Recent" under the drop area: a row per conversion (the file, a chip with what it became, files written and when).
void rd_recent(RdDraw& d, const Layout& l) {
    const Model& m = d.model;
    const RdPalette& pal = d.pal;
    if (l.recent_rows.empty()) return;
    d.text(L"Recent", l.recent_heading, 0, 13, DWRITE_FONT_WEIGHT_SEMI_BOLD, pal.text2);
    for (std::size_t i = 0; i < l.recent_rows.size() && i < m.recent.size(); ++i) {
        const Rect& row = l.recent_rows[i];
        const RecentItem& item = m.recent[i];
        const int idx = static_cast<int>(i);
        const bool hot = (m.hot == Control::RecentRow || m.hot == Control::RecentRemove) && m.hot_index == idx;
        if (hot) d.fill(row, 4, rd_alpha(pal.text, pal.dark ? 0.06f : 0.045f));
        const D2D1_COLOR_F name_color = item.missing ? pal.text3 : pal.text;
        d.glyph(item.glyph_console ? L'\xE7FC' : L'\xE8A5', Rect{row.x + 12, row.y, 20, row.h}, 16, item.missing ? pal.text3 : pal.text);
        const float name_w = std::min(d.painter.measure(widen(item.name), 14, false) + 4, row.w * 0.4f);
        d.text(widen(item.name), Rect{row.x + 44, row.y, name_w, row.h}, 0, 14, DWRITE_FONT_WEIGHT_NORMAL, name_color);
        const float chip_w = d.painter.measure(widen(item.chip), 12, false) + 18;
        if (!item.chip.empty()) {
            const Rect chip{row.x + 44 + name_w + 8, row.y + (row.h - 22) / 2, chip_w, 22};
            d.stroke(chip, 11, pal.stroke2);
            d.text(widen(item.chip), chip, 0, 12, DWRITE_FONT_WEIGHT_NORMAL, item.missing ? pal.text3 : pal.text2, DWRITE_TEXT_ALIGNMENT_CENTER);
        }
        const float meta_right = row.right() - 16 - (item.missing && i < l.recent_remove.size() ? l.recent_remove[i].w + 12 : 0.0f);
        d.text(widen(item.meta), Rect{row.x + 44 + name_w + chip_w + 24, row.y, std::max(0.0f, meta_right - (row.x + 44 + name_w + chip_w + 24)), row.h}, 0, 12,
               DWRITE_FONT_WEIGHT_NORMAL, item.missing ? pal.warn : pal.text3, DWRITE_TEXT_ALIGNMENT_TRAILING);
        if (item.missing && i < l.recent_remove.size()) rd_row_button(d, l.recent_remove[i], L"Remove", m.hot == Control::RecentRemove && m.hot_index == idx);
        if (m.focus == Control::RecentRow && m.focus_index == idx) d.stroke(Rect{row.x + 1, row.y + 1, row.w - 2, row.h - 2}, 4, pal.text, 2.0f);
        if (m.focus == Control::RecentRemove && m.focus_index == idx && i < l.recent_remove.size())
            d.stroke(Rect{l.recent_remove[i].x - 3, l.recent_remove[i].y - 3, l.recent_remove[i].w + 6, l.recent_remove[i].h + 6}, 6, pal.text, 2.0f);
    }
}

// The batch queue in the Program card: a row per file (its state icon, name, the stage of the running one, a bar and the status), Cancel all and how far it is.
void rd_queue(RdDraw& d, const Layout& l) {
    const Model& m = d.model;
    const RdPalette& pal = d.pal;
    const QueueLayout q = compute_queue_layout(l, m);
    d.rt->PushAxisAlignedClip(rd_rect(q.area), D2D1_ANTIALIAS_MODE_ALIASED);
    for (std::size_t i = 0; i < m.queue.size() && i < q.rows.size(); ++i) {
        const QueueRowItem& item = m.queue[i];
        const QueueRowRects& r = q.rows[i];
        if (r.row.bottom() < q.area.y || r.row.y > q.area.bottom()) continue;
        const bool running = item.state == 1;
        d.fill(r.row, 4, pal.dark ? rd_color(0xFFFFFF, 0.035f) : rd_color(0xFFFFFF, 0.7f));
        d.stroke(r.row, 4, running ? pal.accent : pal.stroke2, running ? 1.5f : 1.0f);
        wchar_t glyph = L'\xE8A5';
        D2D1_COLOR_F tone = pal.text3;
        if (running) tone = pal.accent;
        else if (item.state == 2) { glyph = L'\xE73E'; tone = pal.ok; }
        else if (item.state == 3) { glyph = L'\xE7BA'; tone = pal.warn; }
        else if (item.state == 4) { glyph = L'\xEA39'; tone = pal.error; }
        d.glyph(glyph, r.icon, 16, tone);
        const float name_w = std::min(d.painter.measure(widen(item.name), 14, false) + 6, r.text.w);
        d.text(widen(item.name), Rect{r.text.x, r.text.y, name_w, r.text.h}, 0, 14, DWRITE_FONT_WEIGHT_NORMAL, item.state == 5 ? pal.text3 : pal.text);
        if (running && !item.detail.empty() && r.text.w > name_w + 20)
            d.text(widen(item.detail), Rect{r.text.x + name_w + 4, r.text.y, r.text.w - name_w - 4, r.text.h}, 0, 12, DWRITE_FONT_WEIGHT_NORMAL, pal.text3);
        d.fill(r.bar, 2, rd_alpha(pal.text3, 0.3f));
        const float f = item.state == 2 ? 1.0f : running ? static_cast<float>(std::clamp(item.percent, 0, 100)) / 100.0f : 0.0f;
        if (f > 0) d.fill(Rect{r.bar.x, r.bar.y, std::max(3.0f, r.bar.w * f), r.bar.h}, 2, pal.accent);
        d.text(widen(item.status), r.status, 0, 13, DWRITE_FONT_WEIGHT_NORMAL, pal.text2, DWRITE_TEXT_ALIGNMENT_TRAILING, true);
    }
    d.rt->PopAxisAlignedClip();
    rd_scrollbar(d, q.area, q.content_height, m.queue_scroll);
    d.button(Control::Cancel, l.cancel, L"Cancel all", L'\xE711', true);
    d.text(widen("Drop more files to add them to the list. " + m.queue_note + "."), l.hint, 0, 13, DWRITE_FONT_WEIGHT_NORMAL, pal.text3);
}

// The output folder's box: its path, and a dashed accent outline while the pointer is over it or the folder is being dragged out of it (it is the drag source).
void rd_output_box(RdDraw& d, const Layout& l) {
    const Model& m = d.model;
    const RdPalette& pal = d.pal;
    const bool active = m.hot == Control::OutputBox || m.drag_out;
    if (active) {
        d.fill(l.field, 4, pal.subtle);
        const ComPtr<ID2D1StrokeStyle> dashes = rd_dashes(d, 4.0f, 3.0f);
        d.stroke(l.field, 4, pal.accent, 2.0f, dashes.get());
    } else {
        d.fill(l.field, 4, pal.field);
        d.stroke(l.field, 4, pal.stroke2);
        d.brush->SetColor(pal.field_line);
        d.rt->DrawLine(D2D1::Point2F(l.field.x + 4, l.field.bottom() - 0.5f), D2D1::Point2F(l.field.right() - 4, l.field.bottom() - 0.5f), d.brush.get(), 1.0f);
    }
    const float inset = active ? 36.0f : 11.0f;
    if (active) d.glyph(L'\xE8B7', Rect{l.field.x + 10, l.field.y, 20, l.field.h}, 16, pal.accent);
    d.text(rd_fit_middle(d.painter, widen(m.output_path), l.field.w - inset - 11, 14), Rect{l.field.x + inset, l.field.y, l.field.w - inset - 11, l.field.h}, 0, 14,
           DWRITE_FONT_WEIGHT_NORMAL, pal.text);
    if (m.focus == Control::OutputBox) d.stroke(Rect{l.field.x - 3, l.field.y - 3, l.field.w + 6, l.field.h + 6}, 6, pal.text, 2.0f);
    if (m.drag_ghost) {   // (screenshots) what the system draws under the pointer while the folder is dragged: its icon, name and size
        const std::size_t slash = m.output_path.find_last_of("\\/");
        const std::string leaf = slash == std::string::npos ? m.output_path : m.output_path.substr(slash + 1);
        const std::string count = m.done_note.substr(0, m.done_note.find(" written"));
        const Rect ghost{l.field.right() - 90, l.field.bottom() - 12, 150, 38};
        for (int i = 3; i >= 1; --i) d.fill(Rect{ghost.x - static_cast<float>(i), ghost.y - static_cast<float>(i) + 2, ghost.w + 2.0f * static_cast<float>(i), ghost.h + 2.0f * static_cast<float>(i)}, 8, rd_color(0x000000, 0.06f));
        d.fill(ghost, 6, pal.dark ? rd_color(0x383838) : rd_color(0xFFFFFF));
        d.stroke(ghost, 6, pal.stroke2);
        d.glyph(L'\xE8B7', Rect{ghost.x + 10, ghost.y, 20, ghost.h}, 16, pal.accent);
        const float leaf_w = d.painter.measure(widen(leaf), 13, false) + 6;
        d.text(widen(leaf), Rect{ghost.x + 38, ghost.y, leaf_w, ghost.h}, 0, 13, DWRITE_FONT_WEIGHT_NORMAL, pal.text);
        d.text(widen(count), Rect{ghost.x + 38 + leaf_w + 6, ghost.y, ghost.w - 44 - leaf_w, ghost.h}, 0, 13, DWRITE_FONT_WEIGHT_NORMAL, pal.text3);
    }
}

D2D1_COLOR_F rd_token_color(const RdPalette& pal, preview::Tok kind) {
    switch (kind) {
        case preview::Tok::Keyword: return pal.dark ? rd_color(0x569CD6) : rd_color(0x0451A5);
        case preview::Tok::Comment: return pal.dark ? rd_color(0x6A9955) : rd_color(0x008000);
        case preview::Tok::Number: return pal.dark ? rd_color(0xB5CEA8) : rd_color(0x098658);
        case preview::Tok::String: return pal.dark ? rd_color(0xCE9178) : rd_color(0xA31515);
        case preview::Tok::Plain: break;
    }
    return pal.text;
}

// UTF-16 index of each byte offset of a UTF-8 line (so a span's bytes find their characters).
std::vector<UINT32> rd_wide_index(const std::string& line) {
    std::vector<UINT32> at(line.size() + 1, 0);
    UINT32 w = 0;
    for (std::size_t i = 0; i < line.size();) {
        const unsigned char c = static_cast<unsigned char>(line[i]);
        const std::size_t len = c < 0x80 ? 1 : c >= 0xF0 ? 4 : c >= 0xE0 ? 3 : c >= 0xC0 ? 2 : 1;
        for (std::size_t k = 0; k < len && i + k < line.size(); ++k) at[i + k] = w;
        w += len == 4 ? 2 : 1;
        i += len;
    }
    at[line.size()] = w;
    return at;
}

// One line of code at (x, y), its spans in their colours (keywords semibold); one text layout so the monospace advance is the font's own.
void rd_code_line(RdDraw& d, const std::string& line, const std::vector<preview::Span>& spans, float x, float y, ID2D1SolidColorBrush* const brushes[5]) {
    if (line.empty()) return;
    IDWriteTextFormat* format = d.painter_format(3, kCodeFontSize, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT_NEAR, false);
    if (format == nullptr) return;
    const std::wstring wide = widen(line);
    ComPtr<IDWriteTextLayout> layout;
    if (FAILED(d.painter.dwrite()->CreateTextLayout(wide.c_str(), static_cast<UINT32>(wide.size()), format, 8000.0f, kCodeLineHeight, layout.put()))) return;
    const std::vector<UINT32> at = rd_wide_index(line);
    for (const preview::Span& s : spans) {
        const std::size_t end = std::min<std::size_t>(line.size(), static_cast<std::size_t>(s.start) + s.length);
        if (s.start >= line.size() || end <= s.start) continue;
        const DWRITE_TEXT_RANGE range{at[s.start], at[end] - at[s.start]};
        layout->SetDrawingEffect(brushes[static_cast<int>(s.kind)], range);
        if (s.kind == preview::Tok::Keyword) layout->SetFontWeight(DWRITE_FONT_WEIGHT_SEMI_BOLD, range);
    }
    d.rt->DrawTextLayout(D2D1::Point2F(x, y + 2), layout.get(), brushes[0], D2D1_DRAW_TEXT_OPTIONS_NONE);
}

// The Source card's preview: the output folder's files on the left, the selected file's source on the right.
void rd_preview(RdDraw& d, const Layout& l) {
    const Model& m = d.model;
    const RdPalette& pal = d.pal;
    const PreviewLayout p = compute_preview_layout(l, m);
    const D2D1_COLOR_F code_fill = pal.dark ? rd_color(0x1F1F1F) : rd_color(0xF6F6F6);
    d.fill(l.preview, 6, code_fill);
    d.fill(Rect{l.preview_list.x, l.preview_list.y, l.preview_list.w, l.preview_list.h}, 6, pal.dark ? rd_color(0x262626) : rd_color(0xFBFBFB));
    d.fill(Rect{l.preview_list.right() - 8, l.preview_list.y, 8, l.preview_list.h}, 0, pal.dark ? rd_color(0x262626) : rd_color(0xFBFBFB));
    d.stroke(l.preview, 6, pal.stroke2);
    d.brush->SetColor(pal.stroke2);
    d.rt->DrawLine(D2D1::Point2F(l.preview_list.right() - 0.5f, l.preview_list.y + 1), D2D1::Point2F(l.preview_list.right() - 0.5f, l.preview_list.bottom() - 1), d.brush.get(), 1.0f);

    // the files
    d.rt->PushAxisAlignedClip(rd_rect(p.list_area), D2D1_ANTIALIAS_MODE_ALIASED);
    for (std::size_t i = 0; i < m.preview_files.size() && i < p.file_rows.size(); ++i) {
        const Rect& r = p.file_rows[i];
        if (r.bottom() < p.list_area.y || r.y > p.list_area.bottom()) continue;
        const bool selected = static_cast<int>(i) == m.preview_selected;
        if (selected) d.fill(r, 4, rd_alpha(pal.text, pal.dark ? 0.08f : 0.06f));
        else if (m.hot == Control::PreviewFile && m.hot_index == static_cast<int>(i)) d.fill(r, 4, rd_alpha(pal.text, pal.dark ? 0.045f : 0.035f));
        d.glyph(m.preview_files[i].script ? L'\xE90F' : L'\xE943', Rect{r.x + 8, r.y, 20, r.h}, 14, pal.text2);
        d.text(widen(m.preview_files[i].name), Rect{r.x + 36, r.y, std::max(0.0f, r.w - 44), r.h}, 0, 13.5f, DWRITE_FONT_WEIGHT_NORMAL, pal.text);
        if (m.focus == Control::PreviewFile && m.focus_index == static_cast<int>(i)) d.stroke(Rect{r.x + 1, r.y + 1, r.w - 2, r.h - 2}, 4, pal.text, 2.0f);
    }
    d.rt->PopAxisAlignedClip();
    rd_scrollbar(d, p.list_area, p.list_content_height, m.preview_list_scroll);

    // the code
    ID2D1SolidColorBrush* brushes[5] = {};
    ComPtr<ID2D1SolidColorBrush> owned[5];
    const preview::Tok kinds[5] = {preview::Tok::Plain, preview::Tok::Keyword, preview::Tok::Comment, preview::Tok::Number, preview::Tok::String};
    for (int k = 0; k < 5; ++k) {
        if (FAILED(d.rt->CreateSolidColorBrush(rd_token_color(pal, kinds[k]), owned[k].put()))) return;
        brushes[static_cast<int>(kinds[k])] = owned[k].get();
    }
    // the font's own advance, for the sideways scroll
    float advance = kCodeCharWidth;
    if (IDWriteTextFormat* f = d.painter_format(3, kCodeFontSize, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT_NEAR, false)) {
        ComPtr<IDWriteTextLayout> probe;
        if (SUCCEEDED(d.painter.dwrite()->CreateTextLayout(L"MMMMMMMMMM", 10, f, 1000.0f, 30.0f, probe.put()))) {
            DWRITE_TEXT_METRICS tm{};
            probe->GetMetrics(&tm);
            if (tm.width > 0) advance = tm.width / 10.0f;
        }
    }
    if (m.preview_lines.empty()) {
        d.text(widen(m.preview_note.empty() ? std::string("This file is empty.") : m.preview_note), Rect{p.code_area.x, p.code_area.y, p.code_area.w, 24}, 0, 13, DWRITE_FONT_WEIGHT_NORMAL, pal.text3);
    } else {
        d.rt->PushAxisAlignedClip(rd_rect(p.code_area), D2D1_ANTIALIAS_MODE_ALIASED);
        const std::size_t first = static_cast<std::size_t>(std::clamp(static_cast<int>(m.preview_scroll), 0, preview_max_first_line(m, p)));
        const float x = p.code_area.x - m.preview_hscroll * advance;
        for (std::size_t i = first, k = 0; i < m.preview_lines.size() && k < static_cast<std::size_t>(p.visible_lines) + 1; ++i, ++k) {
            static const std::vector<preview::Span> none;
            rd_code_line(d, m.preview_lines[i], i < m.preview_spans.size() ? m.preview_spans[i] : none, x, p.code_area.y + static_cast<float>(k) * kCodeLineHeight, brushes);
        }
        d.rt->PopAxisAlignedClip();
        rd_scrollbar(d, p.code_area, static_cast<float>(m.preview_lines.size()) * kCodeLineHeight, static_cast<float>(first) * kCodeLineHeight);
        if (m.focus == Control::CodeView) d.stroke(Rect{l.preview_code.x + 2, l.preview_code.y + 2, l.preview_code.w - 4, l.preview_code.h - 4}, 4, pal.text, 2.0f);
        if (p.note_area.h > 0) d.text(widen(m.preview_note), p.note_area, 0, 12, DWRITE_FONT_WEIGHT_NORMAL, pal.text3);
    }
}

}  // namespace

// The Program card (every view but Done): its title and description, the Language drop-down in its header, the InfoBar of a file that cannot be
// converted, and the drop area -- the "+" and Browse when idle, the progress over the character stream while working.
void rd_program_card(RdDraw& d, View view, const Layout& l) {
    const Model& model = d.model;
    const RdPalette& pal = d.pal;
    d.fill(l.left_card, kCardRadius, pal.card);
    d.stroke(l.left_card, kCardRadius, pal.card_stroke);
    d.text(L"Program", l.left_title, 1, 20, DWRITE_FONT_WEIGHT_SEMI_BOLD, pal.text);
    d.text(L"The file you want to turn back into source code.", l.left_desc, 0, 14, DWRITE_FONT_WEIGHT_NORMAL, pal.text2);

    // the Language drop-down (a Fluent ComboBox), in the header: the label, the closed control with the chosen language and a chevron
    {
        const bool enabled = control_enabled(Control::Language, view);
        const float dim = enabled ? 1.0f : 0.45f;
        d.text(L"Language", l.language_label, 0, 14, DWRITE_FONT_WEIGHT_NORMAL, rd_alpha(pal.text2, dim), DWRITE_TEXT_ALIGNMENT_TRAILING);
        D2D1_COLOR_F fill = pal.field;
        if (enabled && (model.hot == Control::Language || model.language_open)) fill = rd_mix(fill, pal.text, pal.dark ? 0.04f : 0.035f);
        d.fill(l.language_combo, 4, rd_alpha(fill, dim));
        d.stroke(l.language_combo, 4, rd_alpha(pal.stroke2, dim));
        d.brush->SetColor(model.language_open ? pal.accent : rd_alpha(pal.field_line, dim));
        d.rt->DrawLine(D2D1::Point2F(l.language_combo.x + 4, l.language_combo.bottom() - 0.5f), D2D1::Point2F(l.language_combo.right() - 4, l.language_combo.bottom() - 0.5f),
                       d.brush.get(), model.language_open ? 2.0f : 1.0f);
        const std::size_t chosen = static_cast<std::size_t>(std::clamp(model.language_selected, 0, std::max(0, static_cast<int>(model.languages.size()) - 1)));
        const std::wstring shown = model.languages.empty() ? std::wstring(L"Original") : widen(model.languages[chosen].name);
        d.text(shown, Rect{l.language_combo.x + 11, l.language_combo.y, l.language_combo.w - 44, l.language_combo.h}, 0, 14, DWRITE_FONT_WEIGHT_NORMAL, rd_alpha(pal.text, dim));
        d.glyph(model.language_open ? L'\xE70E' : L'\xE70D', Rect{l.language_combo.right() - 30, l.language_combo.y, 20, l.language_combo.h}, 12, rd_alpha(pal.text2, dim));
        if (enabled) d.focus_ring(Control::Language, l.language_combo);
    }

    if (view == View::Unsupported || view == View::Failed) {
        const bool failed = view == View::Failed;
        const D2D1_COLOR_F tone = failed ? pal.error : pal.warn;
        d.fill(l.info_bar, 4, failed ? pal.error_fill : pal.warn_fill);
        d.stroke(l.info_bar, 4, pal.stroke2);
        d.glyph(failed ? L'\xEA39' : L'\xE7BA', l.info_icon, 16, tone);
        d.text(widen(model.info_title), l.info_title, 0, 14, DWRITE_FONT_WEIGHT_SEMI_BOLD, pal.text);
        d.text(widen(model.info_message), l.info_message, 0, 14, DWRITE_FONT_WEIGHT_NORMAL, pal.text, DWRITE_TEXT_ALIGNMENT_LEADING, true,
               DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
        d.button(Control::TryAnother, l.try_another, L"Try another file", 0, true);
    }

    // several files: the queue takes the drop area's place
    if (view == View::Working && !model.queue.empty()) {
        rd_queue(d, l);
        return;
    }

    // the dashed drop area
    {
        ComPtr<ID2D1StrokeStyle> dashes;
        const float pattern[2] = {3.0f, 2.0f};
        D2D1_STROKE_STYLE_PROPERTIES sp = D2D1::StrokeStyleProperties(D2D1_CAP_STYLE_FLAT, D2D1_CAP_STYLE_FLAT, D2D1_CAP_STYLE_FLAT, D2D1_LINE_JOIN_MITER, 10.0f,
                                                                      D2D1_DASH_STYLE_CUSTOM, 0.0f);
        d.painter.d2d()->CreateStrokeStyle(sp, pattern, 2, dashes.put());
        if (model.dragging) {
            d.fill(l.drop, 8, rd_alpha(pal.accent, 0.12f));
            d.stroke(l.drop, 8, pal.accent, 2.0f);
        } else {
            d.fill(l.drop, 8, pal.subtle);
            d.stroke(l.drop, 8, pal.dash, 2.0f, dashes.get());
        }
    }

    if (view == View::Working) {
        const float left = std::min({l.file_icon.x, l.file_name.x, l.bar.x, l.cancel.x});
        const float right = std::max({l.file_icon.right(), l.file_name.right(), l.bar.right(), l.cancel.right()});
        rd_stream(d, l.drop, Rect{left, l.file_icon.y, right - left, l.cancel.bottom() - l.file_icon.y});
        d.circle(l.file_icon, rd_alpha(pal.accent, 0.16f));
        d.glyph(L'\xE8A5', l.file_icon, 24.0f, pal.accent);
        d.text(widen(model.file_name), l.file_name, 0, 16, DWRITE_FONT_WEIGHT_SEMI_BOLD, pal.text, DWRITE_TEXT_ALIGNMENT_CENTER);
        d.fill(l.bar, 2, rd_alpha(pal.text3, 0.4f));
        const float filled = l.bar.w * static_cast<float>(std::clamp(model.percent, 0, 100)) / 100.0f;
        if (filled > 0) d.fill(Rect{l.bar.x, l.bar.y, std::max(filled, 4.0f), l.bar.h}, 2, pal.accent);
        d.text(widen(model.stage_label), l.bar_label, 0, 14, DWRITE_FONT_WEIGHT_NORMAL, pal.text);
        d.text(widen(std::to_string(model.percent) + "%"), l.bar_percent, 0, 14, DWRITE_FONT_WEIGHT_NORMAL, pal.text2, DWRITE_TEXT_ALIGNMENT_TRAILING);
        d.text(widen(model.detail), l.detail, 0, 12, DWRITE_FONT_WEIGHT_NORMAL, pal.text3);
        d.button(Control::Cancel, l.cancel, L"Cancel", L'\xE711', true);
    } else {
        // the idle group: the round "+" button (it opens the file picker) and two lines
        D2D1_COLOR_F plus = pal.accent;
        if (model.pressed == Control::Plus && model.hot == Control::Plus) plus = rd_mix(plus, D2D1::ColorF(0, 0, 0), 0.25f);
        else if (model.hot == Control::Plus) plus = rd_mix(plus, pal.dark ? D2D1::ColorF(1, 1, 1) : D2D1::ColorF(0, 0, 0), 0.10f);
        d.circle(l.plus, plus);
        d.glyph(L'\xE710', l.plus, 28, pal.accent_text);
        d.focus_ring(Control::Plus, l.plus);
        d.text(L"Drag a file here", l.drop_line1, 0, 15, DWRITE_FONT_WEIGHT_SEMI_BOLD, pal.text, DWRITE_TEXT_ALIGNMENT_CENTER);
        d.text(widen(model.drop_hint), l.drop_line2, 0, 13, DWRITE_FONT_WEIGHT_NORMAL, pal.text2, DWRITE_TEXT_ALIGNMENT_CENTER);
    }
    rd_recent(d, l);
}

// ---- the Source card's tabs: Functions, Check and Compare ------------------------------------------------------------------------------------

namespace {

D2D1_COLOR_F rd_amber() { return rd_color(0xA05A00); }

// A rounded clip: everything drawn until the layer is popped stays inside `r`.
ComPtr<ID2D1Layer> rd_push_rounded_clip(RdDraw& d, const Rect& r, float radius) {
    ComPtr<ID2D1RoundedRectangleGeometry> geometry;
    ComPtr<ID2D1Layer> layer;
    if (FAILED(d.painter.d2d()->CreateRoundedRectangleGeometry(D2D1::RoundedRect(rd_rect(r), radius, radius), geometry.put()))) return layer;
    if (FAILED(d.rt->CreateLayer(nullptr, layer.put()))) return layer;
    d.rt->PushLayer(D2D1::LayerParameters(D2D1::InfiniteRect(), geometry.get()), layer.get());
    return layer;
}

// The verdict chip of a function: a pill with its words (a check or a warning mark in front of the two that were judged).
void rd_chip(RdDraw& d, const Rect& area, convert::FunctionVerdict verdict) {
    const RdPalette& pal = d.pal;
    const std::wstring words = widen(verdict_name(verdict));
    D2D1_COLOR_F fill = rd_alpha(pal.text, 0.06f), ink = pal.text2;
    wchar_t mark = 0;
    if (verdict == convert::FunctionVerdict::Matches) {
        fill = rd_alpha(pal.ok, pal.dark ? 0.16f : 0.14f);
        ink = pal.ok;
        mark = L'\xE73E';
    } else if (verdict == convert::FunctionVerdict::Differs) {
        fill = pal.warn_fill;
        ink = pal.warn;
        mark = L'\xE7BA';
    }
    const float text_w = d.painter.measure(words, 12, true);
    const float w = std::min(area.w, text_w + (mark != 0 ? 20.0f : 0.0f) + 20);
    const Rect chip{area.x, area.y + (area.h - 22) / 2, w, 22};
    d.fill(chip, 11, fill);
    float x = chip.x + 10;
    if (mark != 0) {
        d.glyph(mark, Rect{x, chip.y, 14, chip.h}, 11, ink);
        x += 18;
    }
    d.text(words, Rect{x, chip.y, text_w + 4, chip.h}, 0, 12, verdict == convert::FunctionVerdict::NotChecked ? DWRITE_FONT_WEIGHT_NORMAL : DWRITE_FONT_WEIGHT_SEMI_BOLD, ink);
}

// An arc of a ring, from `a0` to `a1` degrees clockwise from the top.
void rd_ring_arc(RdDraw& d, float cx, float cy, float radius, float a0, float a1, D2D1_COLOR_F color, float width) {
    if (a1 - a0 <= 0.01f) return;
    d.brush->SetColor(color);
    if (a1 - a0 >= 359.9f) {
        d.rt->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(cx, cy), radius, radius), d.brush.get(), width);
        return;
    }
    const auto point = [&](float deg) {
        const float rad = (deg - 90.0f) * 3.14159265f / 180.0f;
        return D2D1::Point2F(cx + radius * std::cos(rad), cy + radius * std::sin(rad));
    };
    ComPtr<ID2D1PathGeometry> path;
    ComPtr<ID2D1GeometrySink> sink;
    if (FAILED(d.painter.d2d()->CreatePathGeometry(path.put())) || FAILED(path->Open(sink.put()))) return;
    sink->BeginFigure(point(a0), D2D1_FIGURE_BEGIN_HOLLOW);
    sink->AddArc(D2D1::ArcSegment(point(a1), D2D1::SizeF(radius, radius), 0, D2D1_SWEEP_DIRECTION_CLOCKWISE, a1 - a0 > 180.0f ? D2D1_ARC_SIZE_LARGE : D2D1_ARC_SIZE_SMALL));
    sink->EndFigure(D2D1_FIGURE_END_OPEN);
    sink->Close();
    d.rt->DrawGeometry(path.get(), d.brush.get(), width);
}

// The tab strip: Files, Functions, Check; the shown one is semibold with the accent underline.
void rd_tabs(RdDraw& d, const Layout& l) {
    const Model& m = d.model;
    const RdPalette& pal = d.pal;
    d.brush->SetColor(pal.stroke2);
    d.rt->DrawLine(D2D1::Point2F(l.tabs.x, l.tabs.bottom() - 0.5f), D2D1::Point2F(l.tabs.right(), l.tabs.bottom() - 0.5f), d.brush.get(), 1.0f);
    const struct {
        Control id;
        SourceTab tab;
        const Rect* rect;
        const wchar_t* label;
    } tabs[] = {{Control::TabFiles, SourceTab::Files, &l.tab_files, L"Files"},
                {Control::TabFunctions, SourceTab::Functions, &l.tab_functions, L"Functions"},
                {Control::TabCheck, SourceTab::Check, &l.tab_check, L"Check"}};
    for (const auto& t : tabs) {
        const bool on = m.source_tab == t.tab;
        const float text_w = d.painter.measure(t.label, 14, on);
        const Rect r{t.rect->x, l.tabs.y, t.rect->w, l.tabs.h};
        d.text(t.label, Rect{r.x + (r.w - text_w) / 2 - 2, r.y, text_w + 4, r.h - 3}, 0, 14, on ? DWRITE_FONT_WEIGHT_SEMI_BOLD : DWRITE_FONT_WEIGHT_NORMAL,
               on || m.hot == t.id ? pal.text : pal.text2);
        if (on) d.fill(Rect{r.x + (r.w - text_w) / 2 - 2, r.bottom() - 3, text_w + 4, 3}, 1.5f, pal.accent);
        if (m.focus == t.id) d.stroke(Rect{r.x - 4, r.y + 1, r.w + 8, r.h - 3}, 5, pal.text, 2.0f);
    }
}

// The segmented bar of the Functions view: matches, differs, not checked, as shares of all the functions.
void rd_segments(RdDraw& d, const Rect& bar, const fnsrc::Counts& c) {
    const RdPalette& pal = d.pal;
    d.fill(bar, 3, rd_alpha(pal.text, pal.dark ? 0.14f : 0.10f));
    if (c.total <= 0) return;
    const ComPtr<ID2D1Layer> clip = rd_push_rounded_clip(d, bar, 3);
    const float total = static_cast<float>(c.total);
    float x = bar.x;
    const float ok_w = bar.w * static_cast<float>(c.matches) / total, bad_w = bar.w * static_cast<float>(c.differs) / total;
    d.fill(Rect{x, bar.y, ok_w, bar.h}, 0, pal.ok);
    x += ok_w;
    d.fill(Rect{x, bar.y, std::max(bad_w, c.differs > 0 ? 3.0f : 0.0f), bar.h}, 0, rd_amber());
    if (clip) d.rt->PopLayer();
}

void rd_functions(RdDraw& d, const Layout& l) {
    const Model& m = d.model;
    const RdPalette& pal = d.pal;
    const FunctionsLayout f = compute_functions_layout(l, m);
    const fnsrc::Counts counts = fnsrc::count(m.functions);
    rd_segments(d, f.bar, counts);
    // the words: the first figures are heavier
    const std::string summary = functions_summary(counts);
    const std::size_t of = summary.find(" behave like");
    if (counts.judged() > 0 && of != std::string::npos) {
        const std::wstring lead = widen(summary.substr(0, of)), rest = widen(summary.substr(of));
        const float lead_w = d.painter.measure(lead, 13, true);
        d.text(lead, Rect{f.summary.x, f.summary.y, lead_w + 4, f.summary.h}, 0, 13, DWRITE_FONT_WEIGHT_SEMI_BOLD, pal.text);
        d.text(rest, Rect{f.summary.x + lead_w, f.summary.y, std::max(0.0f, f.summary.w - lead_w), f.summary.h}, 0, 13, DWRITE_FONT_WEIGHT_NORMAL, pal.text2);
    } else {
        d.text(widen(summary), f.summary, 0, 13, DWRITE_FONT_WEIGHT_NORMAL, pal.text2);
    }
    // Show: All
    {
        const bool hot = m.hot == Control::FnFilter;
        D2D1_COLOR_F fill = pal.dark ? rd_mix(rd_color(0x2B2B2B), D2D1::ColorF(1, 1, 1), hot ? 0.11f : 0.08f) : rd_mix(pal.field, pal.text, hot ? 0.035f : 0.0f);
        d.fill(f.filter, 4, fill);
        d.stroke(f.filter, 4, pal.stroke2);
        d.glyph(L'\xE71C', Rect{f.filter.x + 10, f.filter.y, 18, f.filter.h}, 14, pal.text);
        d.text(L"Show: " + widen(filter_name(m.fn_filter)), Rect{f.filter.x + 34, f.filter.y, f.filter.w - 40, f.filter.h}, 0, 14, DWRITE_FONT_WEIGHT_NORMAL, pal.text);
        if (m.focus == Control::FnFilter) d.stroke(Rect{f.filter.x - 3, f.filter.y - 3, f.filter.w + 6, f.filter.h + 6}, 6, pal.text, 2.0f);
    }
    // the rows
    d.rt->PushAxisAlignedClip(rd_rect(f.list_area), D2D1_ANTIALIAS_MODE_ALIASED);
    for (std::size_t i = 0; i < f.shown.size(); ++i) {
        const Rect& r = f.rows[i];
        if (r.bottom() < f.list_area.y || r.y > f.list_area.bottom()) continue;
        const int index = f.shown[i];
        const convert::FunctionRecord& fn = m.functions[static_cast<std::size_t>(index)];
        if (m.hot == Control::FnRow && m.hot_index == index) d.fill(r, 4, rd_alpha(pal.text, pal.dark ? 0.07f : 0.055f));
        const FnRowParts p = fn_row_parts(r);
        d.glyph(L'\xE943', p.icon, 14, pal.text3);
        d.text(widen(fn.name), p.name, 0, 14, DWRITE_FONT_WEIGHT_NORMAL, pal.text);
        d.text(widen(fn.address), p.address, 3, 12, DWRITE_FONT_WEIGHT_NORMAL, pal.text3);
        rd_chip(d, p.chip, fn.verdict);
        if (m.focus == Control::FnRow && m.focus_index == index) d.stroke(Rect{r.x + 1, r.y + 1, r.w - 2, r.h - 2}, 4, pal.text, 2.0f);
    }
    d.rt->PopAxisAlignedClip();
    if (f.shown.empty()) d.text(widen(empty_filter_text(m.fn_filter)), Rect{f.list_area.x + 12, f.list_area.y, f.list_area.w - 24, 32}, 0, 13, DWRITE_FONT_WEIGHT_NORMAL, pal.text3);
    rd_scrollbar(d, f.list_area, f.content_height, m.fn_scroll);
}

void rd_check(RdDraw& d, const Layout& l) {
    const Model& m = d.model;
    const RdPalette& pal = d.pal;
    const CheckLayout c = compute_check_layout(l, m);
    const fnsrc::Counts counts = fnsrc::count(m.functions);
    if (c.judged) {
        const float cx = c.ring.x + c.ring.w / 2, cy = c.ring.y + c.ring.h / 2, width = 14, radius = c.ring.w / 2 - width / 2;
        const float total = static_cast<float>(counts.total);
        const float a_ok = 360.0f * static_cast<float>(counts.matches) / total, a_bad = 360.0f * static_cast<float>(counts.differs) / total;
        rd_ring_arc(d, cx, cy, radius, 0, 360, rd_alpha(pal.text, pal.dark ? 0.14f : 0.10f), width);
        rd_ring_arc(d, cx, cy, radius, 0, a_ok, pal.ok, width);
        rd_ring_arc(d, cx, cy, radius, a_ok, a_ok + a_bad, rd_amber(), width);
        d.text(std::to_wstring(counts.percent_alike()) + L"%", Rect{c.ring.x, cy - 24, c.ring.w, 32}, 1, 28, DWRITE_FONT_WEIGHT_SEMI_BOLD, pal.text, DWRITE_TEXT_ALIGNMENT_CENTER);
        d.text(L"behaves alike", Rect{c.ring.x, cy + 8, c.ring.w, 18}, 0, 12, DWRITE_FONT_WEIGHT_NORMAL, pal.text2, DWRITE_TEXT_ALIGNMENT_CENTER);
        d.text(L"Checked by running both side by side", c.title, 0, 15, DWRITE_FONT_WEIGHT_SEMI_BOLD, pal.text);
        d.text(L"Each function of the rebuilt program ran beside the original with the same inputs. The share that did the same thing is shown; "
               L"the ones nobody ran count against it.",
               c.body, 0, 13, DWRITE_FONT_WEIGHT_NORMAL, pal.text2, DWRITE_TEXT_ALIGNMENT_LEADING, true, DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
        const struct {
            D2D1_COLOR_F color;
            const wchar_t* label;
            int count;
        } rows[3] = {{pal.ok, L"Behaves like the original", counts.matches}, {rd_amber(), L"Differs", counts.differs}, {pal.text3, L"Not checked", counts.unchecked}};
        for (int i = 0; i < 3; ++i) {
            if (i == 2) d.stroke(c.dot[i], 4, rows[i].color, 1.5f);
            else d.circle(c.dot[i], rows[i].color);
            d.text(rows[i].label, c.label[i], 0, 14, DWRITE_FONT_WEIGHT_NORMAL, pal.text);
            d.text(std::to_wstring(rows[i].count), c.count[i], 0, 14, DWRITE_FONT_WEIGHT_SEMI_BOLD, pal.text);
        }
        if (c.show.w > 0) {
            const bool hot = m.hot == Control::CheckShow;
            d.fill(c.show, 4, hot ? rd_mix(pal.accent, pal.text, 0.1f) : pal.accent);
            d.glyph(L'\xE7BA', Rect{c.show.x + 12, c.show.y, 16, c.show.h}, 14, pal.accent_text);
            d.text(L"Show what differs", Rect{c.show.x + 36, c.show.y, c.show.w - 40, c.show.h}, 0, 14, DWRITE_FONT_WEIGHT_NORMAL, pal.accent_text);
            if (m.focus == Control::CheckShow) d.stroke(Rect{c.show.x - 3, c.show.y - 3, c.show.w + 6, c.show.h + 6}, 6, pal.text, 2.0f);
        }
    } else {
        d.circle(c.mark, rd_alpha(pal.text, pal.dark ? 0.10f : 0.07f));
        d.glyph(L'\xE9D9', c.mark, 24, pal.text2);
        d.text(L"This conversion was not checked by running it.", c.title, 1, 17, DWRITE_FONT_WEIGHT_SEMI_BOLD, pal.text, DWRITE_TEXT_ALIGNMENT_CENTER);
        d.text(L"Every function is listed on the Functions tab as Not checked. A check runs the rebuilt program beside the original and compares what "
               L"they do.",
               c.body, 0, 13, DWRITE_FONT_WEIGHT_NORMAL, pal.text2, DWRITE_TEXT_ALIGNMENT_CENTER, true, DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
    }
    // Check again: enabled only when the route can judge again
    const bool on = m.can_recheck, hot = on && m.hot == Control::CheckAgain;
    const float dim = on ? 1.0f : 0.45f;
    D2D1_COLOR_F fill = pal.dark ? rd_mix(rd_color(0x2B2B2B), D2D1::ColorF(1, 1, 1), hot ? 0.11f : 0.08f) : rd_mix(pal.field, pal.text, hot ? 0.035f : 0.0f);
    d.fill(c.again, 4, rd_alpha(fill, dim));
    d.stroke(c.again, 4, rd_alpha(pal.stroke2, dim));
    d.glyph(L'\xE72C', Rect{c.again.x + 12, c.again.y, 16, c.again.h}, 14, rd_alpha(pal.text, dim));
    d.text(L"Check again", Rect{c.again.x + 36, c.again.y, c.again.w - 40, c.again.h}, 0, 14, DWRITE_FONT_WEIGHT_NORMAL, rd_alpha(pal.text, dim));
    if (on && m.focus == Control::CheckAgain) d.stroke(Rect{c.again.x - 3, c.again.y - 3, c.again.w + 6, c.again.h + 6}, 6, pal.text, 2.0f);
    if (c.note.h > 0) d.text(widen(kRecheckUnavailable), c.note, 0, 12, DWRITE_FONT_WEIGHT_NORMAL, pal.text3, c.judged ? DWRITE_TEXT_ALIGNMENT_LEADING : DWRITE_TEXT_ALIGNMENT_CENTER);
}

// One side of Compare: its header, and its lines (lit where they match the clicked line).
void rd_compare_side(RdDraw& d, const Rect& pane, const Rect& head, const Rect& code, const wchar_t* title, const std::vector<std::string>& lines,
                     const std::vector<std::vector<preview::Span>>& spans, const std::string& note, int side, float first, ID2D1SolidColorBrush* const brushes[5]) {
    const Model& m = d.model;
    const RdPalette& pal = d.pal;
    d.fill(pane, 6, pal.dark ? rd_color(0x1F1F1F) : rd_color(0xF6F6F6));
    d.fill(head, 6, pal.dark ? rd_color(0x262626) : rd_color(0xFBFBFB));
    d.fill(Rect{head.x, head.y + 8, head.w, head.h - 8}, 0, pal.dark ? rd_color(0x262626) : rd_color(0xFBFBFB));
    d.stroke(pane, 6, pal.stroke2);
    d.brush->SetColor(pal.stroke2);
    d.rt->DrawLine(D2D1::Point2F(head.x + 1, head.bottom() - 0.5f), D2D1::Point2F(head.right() - 1, head.bottom() - 0.5f), d.brush.get(), 1.0f);
    d.glyph(L'\xE943', Rect{head.x + 10, head.y, 18, head.h}, 13, pal.text2);
    d.text(title, Rect{head.x + 34, head.y, head.w - 44, head.h}, 0, 12.5f, DWRITE_FONT_WEIGHT_SEMI_BOLD, pal.text);
    if (lines.empty()) {
        d.text(widen(note), Rect{code.x, code.y, code.w, 24}, 0, 13, DWRITE_FONT_WEIGHT_NORMAL, pal.text3);
        return;
    }
    d.rt->PushAxisAlignedClip(rd_rect(code), D2D1_ANTIALIAS_MODE_ALIASED);
    const std::size_t from = static_cast<std::size_t>(std::max(0.0f, first));
    for (std::size_t i = from, k = 0; i < lines.size() && k < static_cast<std::size_t>(code.h / kCodeLineHeight) + 1; ++i, ++k) {
        const float y = code.y + static_cast<float>(k) * kCodeLineHeight;
        if (compare_line_lit(m, side, static_cast<int>(i))) d.fill(Rect{pane.x + 1, y, pane.w - 2, kCodeLineHeight}, 0, rd_alpha(pal.accent, pal.dark ? 0.16f : 0.13f));
        else if (m.hot == (side == 0 ? Control::CmpAsmLine : Control::CmpCLine) && m.hot_index == static_cast<int>(i))
            d.fill(Rect{pane.x + 1, y, pane.w - 2, kCodeLineHeight}, 0, rd_alpha(pal.text, 0.04f));
        static const std::vector<preview::Span> none;
        rd_code_line(d, lines[i], i < spans.size() ? spans[i] : none, code.x, y, brushes);
    }
    d.rt->PopAxisAlignedClip();
    rd_scrollbar(d, code, static_cast<float>(lines.size()) * kCodeLineHeight, static_cast<float>(from) * kCodeLineHeight);
}

void rd_compare(RdDraw& d, const Layout& l) {
    const Model& m = d.model;
    const RdPalette& pal = d.pal;
    if (m.cmp_index < 0 || static_cast<std::size_t>(m.cmp_index) >= m.functions.size()) return;
    const convert::FunctionRecord& fn = m.functions[static_cast<std::size_t>(m.cmp_index)];
    const CompareLayout c = compute_compare_layout(l, m);
    // All functions
    {
        const bool hot = m.hot == Control::FnAll;
        D2D1_COLOR_F fill = pal.dark ? rd_mix(rd_color(0x2B2B2B), D2D1::ColorF(1, 1, 1), hot ? 0.11f : 0.08f) : rd_mix(pal.field, pal.text, hot ? 0.035f : 0.0f);
        d.fill(c.all, 4, fill);
        d.stroke(c.all, 4, pal.stroke2);
        d.glyph(L'\xE72B', Rect{c.all.x + 10, c.all.y, 18, c.all.h}, 14, pal.text);
        d.text(L"All functions", Rect{c.all.x + 34, c.all.y, c.all.w - 40, c.all.h}, 0, 14, DWRITE_FONT_WEIGHT_NORMAL, pal.text);
        if (m.focus == Control::FnAll) d.stroke(Rect{c.all.x - 3, c.all.y - 3, c.all.w + 6, c.all.h + 6}, 6, pal.text, 2.0f);
    }
    rd_combo(d, c.picker, widen(fn.name), m.cmp_picker_open, m.hot == Control::FnPicker);
    if (m.focus == Control::FnPicker) d.stroke(Rect{c.picker.x - 3, c.picker.y - 3, c.picker.w + 6, c.picker.h + 6}, 6, pal.text, 2.0f);
    rd_chip(d, c.chip, fn.verdict);
    d.text(L"Click a line to see its match on the other side.", c.hint, 0, 12, DWRITE_FONT_WEIGHT_NORMAL, pal.text3, DWRITE_TEXT_ALIGNMENT_TRAILING);
    ID2D1SolidColorBrush* brushes[5] = {};
    ComPtr<ID2D1SolidColorBrush> owned[5];
    const preview::Tok kinds[5] = {preview::Tok::Plain, preview::Tok::Keyword, preview::Tok::Comment, preview::Tok::Number, preview::Tok::String};
    for (int k = 0; k < 5; ++k) {
        if (FAILED(d.rt->CreateSolidColorBrush(rd_token_color(pal, kinds[k]), owned[k].put()))) return;
        brushes[static_cast<int>(kinds[k])] = owned[k].get();
    }
    rd_compare_side(d, c.asm_pane, c.asm_head, c.asm_code, L"Original (disassembly)", m.cmp_asm, m.cmp_asm_spans, m.cmp_asm_note, 0, m.cmp_scroll_asm, brushes);
    rd_compare_side(d, c.c_pane, c.c_head, c.c_code, L"Rebuilt source (C)", m.cmp_c, m.cmp_c_spans, m.cmp_c_note, 1, m.cmp_scroll_c, brushes);
}

// The function drop-down of Compare, dropped down over the panes.
void rd_picker(RdDraw& d, const Layout& l) {
    const Model& m = d.model;
    const RdPalette& pal = d.pal;
    const PickerLayout p = compute_picker_layout(l, m);
    if (p.rows.empty()) return;
    for (int i = 3; i >= 1; --i)
        d.fill(Rect{p.outline.x - static_cast<float>(i), p.outline.y - static_cast<float>(i) + 2, p.outline.w + 2.0f * static_cast<float>(i), p.outline.h + 2.0f * static_cast<float>(i)},
               kCardRadius, rd_color(0x000000, pal.dark ? 0.10f : 0.045f));
    d.fill(p.outline, kCardRadius, pal.dark ? rd_color(0x323232) : rd_color(0xFCFCFC));
    d.stroke(p.outline, kCardRadius, pal.dark ? rd_color(0xFFFFFF, 0.12f) : rd_color(0x000000, 0.12f));
    for (std::size_t i = 0; i < p.rows.size(); ++i) {
        const int index = p.first + static_cast<int>(i);
        const Rect& row = p.rows[i];
        if (index == m.cmp_picker_hot) d.fill(row, 4, rd_alpha(pal.text, pal.dark ? 0.09f : 0.06f));
        if (index == m.cmp_index) d.fill(Rect{row.x, row.y + 8, 3, 16}, 1.5f, pal.accent);
        d.text(widen(m.functions[static_cast<std::size_t>(index)].name), Rect{row.x + 12, row.y, row.w - 24, row.h}, 0, 14, DWRITE_FONT_WEIGHT_NORMAL, pal.text);
    }
}

// The command palette: a scrim over the window and the box near its top with the search and the rows.
void rd_palette_overlay(RdDraw& d, const Layout& l) {
    const Model& m = d.model;
    const RdPalette& pal = d.pal;
    d.fill(Rect{0, 0, l.width, l.height}, 0, rd_color(0x000000, pal.dark ? 0.38f : 0.20f));
    const PaletteLayout p = compute_palette_layout(l, m);
    for (int i = 4; i >= 1; --i)
        d.fill(Rect{p.box.x - static_cast<float>(i), p.box.y - static_cast<float>(i) + 3, p.box.w + 2.0f * static_cast<float>(i), p.box.h + 2.0f * static_cast<float>(i)}, kCardRadius + 2,
               rd_color(0x000000, pal.dark ? 0.12f : 0.06f));
    d.fill(p.box, kCardRadius, pal.dark ? rd_color(0x2B2B2B) : rd_color(0xFBFBFB));
    d.stroke(p.box, kCardRadius, pal.dark ? rd_color(0xFFFFFF, 0.12f) : rd_color(0x000000, 0.12f));
    // the search
    d.glyph(L'\xE721', Rect{p.input.x + 12, p.input.y, 20, p.input.h}, 15, pal.text2);
    const std::wstring typed = widen(m.palette_query);
    const Rect field{p.input.x + 44, p.input.y, p.input.w - 44 - 120, p.input.h};
    if (typed.empty()) d.text(L"Search tools, recent conversions and pages", field, 0, 16, DWRITE_FONT_WEIGHT_NORMAL, pal.text3);
    else d.text(typed, field, 0, 16, DWRITE_FONT_WEIGHT_NORMAL, pal.text);
    const float caret_x = field.x + std::min(d.painter.measure(typed, 16, false), field.w) + 1;
    d.brush->SetColor(pal.accent);
    d.rt->DrawLine(D2D1::Point2F(caret_x + 3, p.input.y + 13), D2D1::Point2F(caret_x + 3, p.input.bottom() - 13), d.brush.get(), 1.5f);
    d.text(L"Esc to close", p.close_hint, 0, 12, DWRITE_FONT_WEIGHT_NORMAL, pal.text3, DWRITE_TEXT_ALIGNMENT_TRAILING);
    d.brush->SetColor(pal.stroke2);
    d.rt->DrawLine(D2D1::Point2F(p.box.x + 1, p.input.bottom() + 0.5f), D2D1::Point2F(p.box.right() - 1, p.input.bottom() + 0.5f), d.brush.get(), 1.0f);
    if (m.palette_result.rows.empty()) {
        d.text(L"Nothing matches.", Rect{p.box.x + 16, p.input.bottom() + 1, p.box.w - 32, 40}, 0, 13, DWRITE_FONT_WEIGHT_NORMAL, pal.text3);
        return;
    }
    int pick = 0;
    for (std::size_t i = 0; i < m.palette_result.rows.size() && i < p.rows.size(); ++i) {
        const palette::Row& row = m.palette_result.rows[i];
        const Rect& r = p.rows[i];
        if (row.heading) {
            if (r.w > 0) d.text(widen(row.text), Rect{r.x + 10, r.y, r.w - 20, r.h}, 0, 12, DWRITE_FONT_WEIGHT_SEMI_BOLD, pal.text2);
            continue;
        }
        const bool chosen = pick == m.palette_selected;
        ++pick;
        if (r.w <= 0) continue;
        const palette::Item& item = m.palette_items[static_cast<std::size_t>(row.item)];
        if (chosen) d.fill(r, 4, rd_alpha(pal.text, pal.dark ? 0.08f : 0.06f));
        wchar_t glyph = L'\xE943';
        if (item.kind == palette::Kind::Recent) glyph = L'\xE7C3';
        else if (item.kind == palette::Kind::GoTo) glyph = item.index == static_cast<int>(palette::Place::Convert) ? L'\xE8E5' : item.index == static_cast<int>(palette::Place::Tools) ? L'\xE90F' : L'\xE713';
        d.glyph(glyph, Rect{r.x + 8, r.y, 20, r.h}, 15, item.kind == palette::Kind::Tool ? pal.accent : pal.text2);
        const float title_w = d.painter.measure(widen(item.title), 14, false) + 4;
        d.text(widen(item.title), Rect{r.x + 40, r.y, std::min(title_w, r.w - 150), r.h}, 0, 14, DWRITE_FONT_WEIGHT_NORMAL, pal.text);
        d.text(widen(item.note), Rect{r.x + 40 + title_w + 12, r.y, std::max(0.0f, r.w - 40 - title_w - 12 - 90), r.h}, 0, 12, DWRITE_FONT_WEIGHT_NORMAL, pal.text3);
        if (chosen) d.text(L"Enter", Rect{r.right() - 90, r.y, 80, r.h}, 0, 12, DWRITE_FONT_WEIGHT_NORMAL, pal.accent, DWRITE_TEXT_ALIGNMENT_TRAILING);
        else if (!item.hint.empty()) d.text(widen(item.hint), Rect{r.right() - 90, r.y, 80, r.h}, 0, 12, DWRITE_FONT_WEIGHT_NORMAL, pal.text3, DWRITE_TEXT_ALIGNMENT_TRAILING);
    }
}

}  // namespace

// The Source card (Done): the finished file, then where its source is, with Open folder, Copy path and Convert another file.
void rd_source_card(RdDraw& d, const Layout& l) {
    const Model& model = d.model;
    const RdPalette& pal = d.pal;
    d.fill(l.right_card, kCardRadius, pal.card);
    d.stroke(l.right_card, kCardRadius, pal.card_stroke);
    d.text(L"Source", l.right_title, 1, 20, DWRITE_FONT_WEIGHT_SEMI_BOLD, pal.text);
    d.text(L"Where the finished source code goes.", l.right_desc, 0, 14, DWRITE_FONT_WEIGHT_NORMAL, pal.text2);
    const bool with_preview = l.tabs.w > 0;   // the tabs with the file list and code view, the finished file at the top left; else the centred column
    d.circle(l.file_icon, pal.ok);
    d.glyph(L'\xE73E', l.file_icon, with_preview ? 22.0f : 26.0f, D2D1::ColorF(1, 1, 1));
    const DWRITE_TEXT_ALIGNMENT side = with_preview ? DWRITE_TEXT_ALIGNMENT_LEADING : DWRITE_TEXT_ALIGNMENT_CENTER;
    d.text(widen(model.done_file_name.empty() ? model.file_name : model.done_file_name), l.file_name, 0, 16, DWRITE_FONT_WEIGHT_SEMI_BOLD, pal.text, side);
    const std::string note = model.done_note.empty() ? std::string("Conversion finished.") : "Conversion finished. " + model.done_note;
    d.text(widen(note), l.detail, 0, 13, DWRITE_FONT_WEIGHT_NORMAL, pal.text2, side);
    if (!with_preview) d.text(L"Output", l.output_label, 0, 14, DWRITE_FONT_WEIGHT_NORMAL, pal.text);
    if (l.field.w > 0) rd_output_box(d, l);
    if (!with_preview) d.text(widen(kDragHint), l.hint, 0, 12, DWRITE_FONT_WEIGHT_NORMAL, pal.text3);
    d.button(Control::OpenFolder, l.open_folder, L"Open folder", L'\xE838', true);
    if (l.copy_path.w > 0) d.button(Control::CopyPath, l.copy_path, L"Copy path", L'\xE8C8', true);
    d.button(Control::Browse, l.browse, L"Convert another file", L'\xE8E5', true);
    if (with_preview) {
        rd_tabs(d, l);
        if (model.source_tab == SourceTab::Files) rd_preview(d, l);
        else if (model.source_tab == SourceTab::Functions) {
            if (model.cmp_index >= 0) rd_compare(d, l);
            else rd_functions(d, l);
        } else {
            rd_check(d, l);
        }
    }
}

// Draw `draw` at `alpha` (a layer when it is not fully shown; nothing at all when it is not shown).
template <typename Draw>
void rd_with_alpha(ID2D1RenderTarget* rt, float alpha, Draw&& draw) {
    if (alpha <= 0.001f) return;
    if (alpha >= 0.999f) {
        draw();
        return;
    }
    rt->PushLayer(D2D1::LayerParameters(D2D1::InfiniteRect(), nullptr, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE, D2D1::IdentityMatrix(), alpha), nullptr);
    draw();
    rt->PopLayer();
}

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

    const View view = model.view;
    rd_with_alpha(rt, model.chrome_alpha, [&] {
    if (l.rail.w > 0) rd_rail(d, l, model);
    if (model.page == Page::Tools) {
        rd_tools(d, l, model, *this);
    } else if (model.page == Page::Settings) {
        rd_settings(d, l, model);
    } else {
        // ---- the one card: Program, or Source when Done; while the card changes, the old one fades out before the new one fades in
        if (model.fade_from_alpha > 0) {
            const Layout from = compute_layout(model.fade_from, l.width, l.height, l.rail.w, extras_for(model, model.fade_from));
            RdDraw old{rt, d.brush, *this, pal, model, from};
            rd_with_alpha(rt, model.fade_from_alpha, [&] {
                if (model.fade_from == View::Done) rd_source_card(old, from);
                else rd_program_card(old, model.fade_from, from);
            });
        }
        rd_with_alpha(rt, model.card_alpha, [&] {
            if (view == View::Done) rd_source_card(d, l);
            else rd_program_card(d, view, l);
        });
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
        // over the output box of the preview layout (which has no room for the hint) the status bar says what the box is for
        const bool hint = view == View::Done && l.preview_list.w > 0 && (model.hot == Control::OutputBox || model.drag_out);
        d.text(widen(hint ? std::string(kDragHint) : model.status_text), l.status_text, 0, 12, DWRITE_FONT_WEIGHT_NORMAL, pal.text2);
        d.text(widen(model.platforms), l.platforms, 0, 12, DWRITE_FONT_WEIGHT_NORMAL, pal.text2, DWRITE_TEXT_ALIGNMENT_TRAILING);
    }
    });   // (everything below the title bar, at chrome_alpha)

    // ---- the welcome: the window's name in the middle of the empty window as it opens, fading in and out before the page appears
    if (model.welcome && model.welcome_alpha > 0.001f) {
        const float mid = (kTitleBarHeight + l.status_bar.bottom()) / 2;
        d.text(L"Welcome to CodeBreaker", Rect{0, mid - 34, l.width, 46}, 1, 34, DWRITE_FONT_WEIGHT_SEMI_BOLD, rd_alpha(pal.text, model.welcome_alpha),
               DWRITE_TEXT_ALIGNMENT_CENTER);
        d.text(L"Any program, back to its source.", Rect{0, mid + 16, l.width, 24}, 0, 15, DWRITE_FONT_WEIGHT_NORMAL,
               rd_alpha(pal.text2, model.welcome_alpha), DWRITE_TEXT_ALIGNMENT_CENTER);
    }

    // ---- the drop-downs, dropped down over everything else
    if (model.language_open && !model.languages.empty() && model.page == Page::Convert)
        rd_popup(d, language_list_layout(l, model.languages), model.languages, model.language_selected, model.language_hot);
    if (model.cmp_picker_open && model.page == Page::Convert && view == View::Done && model.source_tab == SourceTab::Functions) rd_picker(d, l);
    if (model.field_popup >= 0) {
        const std::vector<FieldItem>& fields = model.page == Page::Settings ? model.setting_fields : model.tool_fields;
        const FormLayout form = model.page == Page::Settings ? compute_settings_layout(l, model).form : compute_tools_layout(l, model).form;
        if (static_cast<std::size_t>(model.field_popup) < fields.size() && static_cast<std::size_t>(model.field_popup) < form.fields.size()) {
            const FieldItem& f = fields[static_cast<std::size_t>(model.field_popup)];
            std::vector<LanguageItem> items;
            int chosen = 0;
            for (std::size_t k = 0; k < f.choices.size(); ++k) {
                items.push_back({f.choices[k], f.choices[k], true, {}});
                if (f.choices[k] == f.text) chosen = static_cast<int>(k);
            }
            rd_popup(d, popup_list_layout(form.fields[static_cast<std::size_t>(model.field_popup)].input, l.status_bar.y - 8, kTitleBarHeight + 8, items), items, chosen, model.language_hot);
        }
    }
    // ---- the command palette, over a scrim over everything
    if (model.palette_open) rd_palette_overlay(d, l);
}

}  // namespace cb::app::window
#endif  // _WIN32
