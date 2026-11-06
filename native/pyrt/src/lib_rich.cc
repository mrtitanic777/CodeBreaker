// lib_rich.cc -- the part of rich CodeBreaker's terminal interface (codebreaker/ui/terminal.py) uses, in C++:
//
//   rich.text.Text       Text(text, style), append, plain, from_markup, assemble, stylize, copy, len / bool / str
//   rich.table.Table     Table.grid(padding=, expand=), add_column(style=, justify=, width=, ratio=, no_wrap=),
//                        add_row: rich's column widths (measure, collapse, ratio distribute / reduce)
//   rich.syntax.Syntax   Syntax(code, "c" | "nasm", theme="monokai", line_numbers=, word_wrap=, background_color=):
//                        pygments' C and NASM lexers' token classes, monokai's colours, rich's number column
//   markup               [b #61afef]...[/], [dim], \[ escapes
//
// Rendering any of them (or a str, as markup) to cells of a given width is here too (tui.hpp): textual's widgets
// in lib_tui.cc draw with it. Everything is rich's own algorithm where what the user sees depends on it (word
// wrap, the column widths, the line number column); styles are colours and the six attributes.
#include "tui.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstring>
#include <map>
#include <mutex>
#include <unordered_map>
#include <unordered_set>

namespace py {

TypeObj* T(const var& v);   // object.cc

namespace tui {

// ------------------------------------------------------------------------------------------ strings
std::u32string u32(const std::string& s) { return to_u32(s); }
std::string utf8(const std::u32string& s) { return from_u32(s); }

int cwidth(char32_t c) {
    if (c < 0x20) return 0;
    if (c < 0x300) return 1;
    if ((c >= 0x300 && c <= 0x36F) || (c >= 0x200B && c <= 0x200F) || (c >= 0xFE00 && c <= 0xFE0F) || c == 0x20E3) return 0;
    if ((c >= 0x1100 && c <= 0x115F) || (c >= 0x2E80 && c <= 0x303E) || (c >= 0x3041 && c <= 0x33FF) ||
        (c >= 0x3400 && c <= 0x4DBF) || (c >= 0x4E00 && c <= 0x9FFF) || (c >= 0xA000 && c <= 0xA4CF) ||
        (c >= 0xAC00 && c <= 0xD7A3) || (c >= 0xF900 && c <= 0xFAFF) || (c >= 0xFE30 && c <= 0xFE4F) ||
        (c >= 0xFF00 && c <= 0xFF60) || (c >= 0xFFE0 && c <= 0xFFE6) || (c >= 0x1F300 && c <= 0x1F64F) ||
        (c >= 0x1F900 && c <= 0x1F9FF) || (c >= 0x1F680 && c <= 0x1F6FF) || (c >= 0x20000 && c <= 0x3FFFD))
        return 2;
    return 1;
}
int cell_len(const std::u32string& s) {
    int n = 0;
    for (char32_t c : s) n += cwidth(c);
    return n;
}

// ------------------------------------------------------------------------------------------ colours
static const std::unordered_map<std::string, std::array<int, 3>>& color_names() {
    // the ANSI colours as textual draws them (its MONOKAI terminal theme)
    static const std::unordered_map<std::string, std::array<int, 3>> m = {
        {"black", {26, 26, 26}}, {"red", {244, 0, 95}}, {"green", {152, 224, 36}}, {"yellow", {253, 151, 31}},
        {"blue", {157, 101, 255}}, {"magenta", {244, 0, 95}}, {"cyan", {88, 209, 235}}, {"white", {196, 197, 181}},
        {"bright_black", {98, 94, 76}}, {"bright_red", {244, 0, 95}}, {"bright_green", {152, 224, 36}},
        {"bright_yellow", {224, 213, 97}}, {"bright_blue", {157, 101, 255}}, {"bright_magenta", {244, 0, 95}},
        {"bright_cyan", {88, 209, 235}}, {"bright_white", {246, 246, 239}}, {"grey", {128, 128, 128}},
        {"gray", {128, 128, 128}}, {"orange", {255, 165, 0}}, {"purple", {128, 0, 128}}, {"pink", {255, 192, 203}},
    };
    return m;
}
static int hexv(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}
bool parse_color(const std::string& text0, Color& out) {
    std::string t = text0;
    while (!t.empty() && isspace((unsigned char)t.back())) t.pop_back();
    size_t a = 0;
    while (a < t.size() && isspace((unsigned char)t[a])) a++;
    t = t.substr(a);
    std::string low = t;
    for (auto& c : low) c = (char)tolower((unsigned char)c);
    if (low == "transparent" || low == "initial") { out = Color::rgb(0, 0, 0, 0); return true; }
    if (low == "auto") { out = Color::rgb(0, 0, 0, 1); out.automatic = true; return true; }
    if (low == "default" || low == "ansi_default") { out = Color(); return true; }
    if (!t.empty() && t[0] == '#') {
        std::string h = t.substr(1);
        for (char c : h)
            if (hexv(c) < 0) return false;
        if (h.size() == 3 || h.size() == 4) {
            out = Color::rgb(hexv(h[0]) * 17, hexv(h[1]) * 17, hexv(h[2]) * 17, h.size() == 4 ? hexv(h[3]) * 17 / 255.0 : 1.0);
            return true;
        }
        if (h.size() == 6 || h.size() == 8) {
            auto byte = [&](size_t i) { return hexv(h[i]) * 16 + hexv(h[i + 1]); };
            out = Color::rgb(byte(0), byte(2), byte(4), h.size() == 8 ? byte(6) / 255.0 : 1.0);
            return true;
        }
        return false;
    }
    if (low.rfind("rgb", 0) == 0) {
        size_t o = low.find('('), c = low.find(')');
        if (o == std::string::npos || c == std::string::npos) return false;
        std::vector<double> v;
        std::string inner = low.substr(o + 1, c - o - 1);
        size_t i = 0;
        while (i < inner.size()) {
            size_t j = inner.find(',', i);
            if (j == std::string::npos) j = inner.size();
            v.push_back(atof(inner.substr(i, j - i).c_str()));
            i = j + 1;
        }
        if (v.size() < 3) return false;
        out = Color::rgb((int)v[0], (int)v[1], (int)v[2], v.size() > 3 ? v[3] : 1.0);
        return true;
    }
    if (low.rfind("ansi_", 0) == 0) low = low.substr(5);
    auto& names = color_names();
    auto it = names.find(low);
    if (it == names.end()) return false;
    out = Color::rgb(it->second[0], it->second[1], it->second[2]);
    return true;
}
Color blend(const Color& from, const Color& to, double f) {
    if (!to.set) return from;
    if (!from.set) return to;
    if (f <= 0) return from;
    if (f >= 1) return to;
    return Color::rgb((int)(from.r + (to.r - from.r) * f), (int)(from.g + (to.g - from.g) * f), (int)(from.b + (to.b - from.b) * f),
                      from.a + (to.a - from.a) * f);
}
Color contrast_text(const Color& bg, double alpha) {
    double br = (299 * bg.r / 255.0 + 587 * bg.g / 255.0 + 114 * bg.b / 255.0) / 1000;
    return br < 0.5 ? Color::rgb(255, 255, 255, alpha) : Color::rgb(0, 0, 0, alpha);
}
Color over(const Color& under, const Color& top0) {
    if (!top0.set) return under;
    Color top = top0;
    if (top.automatic) {
        Color base = under.set ? under : Color::rgb(0, 0, 0);
        top = contrast_text(base, top0.a);
    }
    if (!under.set) {
        Color c = top;
        c.a = 1.0;
        return c;
    }
    Color r = blend(under, top, top.a);
    r.a = 1.0;
    return r;
}
static int clampi(int v, int lo, int hi) { return v < lo ? lo : v > hi ? hi : v; }
Color lighten(const Color& c, double amount) {
    double r = c.r / 255.0, g = c.g / 255.0, b = c.b / 255.0;
    r = r > 0.04045 ? std::pow((r + 0.055) / 1.055, 2.4) : r / 12.92;
    g = g > 0.04045 ? std::pow((g + 0.055) / 1.055, 2.4) : g / 12.92;
    b = b > 0.04045 ? std::pow((b + 0.055) / 1.055, 2.4) : b / 12.92;
    double x = (r * 41.24 + g * 35.76 + b * 18.05) / 95.047;
    double y = (r * 21.26 + g * 71.52 + b * 7.22) / 100;
    double z = (r * 1.93 + g * 11.92 + b * 95.05) / 108.883;
    double off = 16.0 / 116;
    x = x > 0.008856 ? std::pow(x, 1.0 / 3) : 7.787 * x + off;
    y = y > 0.008856 ? std::pow(y, 1.0 / 3) : 7.787 * y + off;
    z = z > 0.008856 ? std::pow(z, 1.0 / 3) : 7.787 * z + off;
    double L = 116 * y - 16, A = 500 * (x - y), B = 200 * (y - z);
    L += amount * 100;
    y = (L + 16) / 116;
    x = A / 500 + y;
    z = y - B / 200;
    y = y > 0.2068930344 ? std::pow(y, 3) : (y - off) / 7.787;
    x = x > 0.2068930344 ? 0.95047 * std::pow(x, 3) : 0.122059 * (x - off);
    z = z > 0.2068930344 ? 1.08883 * std::pow(z, 3) : 0.139827 * (z - off);
    r = x * 3.2406 + y * -1.5372 + z * -0.4986;
    g = x * -0.9689 + y * 1.8758 + z * 0.0415;
    b = x * 0.0557 + y * -0.2040 + z * 1.0570;
    r = r > 0.0031308 ? 1.055 * std::pow(r, 1 / 2.4) - 0.055 : 12.92 * r;
    g = g > 0.0031308 ? 1.055 * std::pow(g, 1 / 2.4) - 0.055 : 12.92 * g;
    b = b > 0.0031308 ? 1.055 * std::pow(b, 1 / 2.4) - 0.055 : 12.92 * b;
    return Color::rgb(clampi((int)(r * 255), 0, 255), clampi((int)(g * 255), 0, 255), clampi((int)(b * 255), 0, 255), c.a);
}
std::string hex(const Color& c) {
    char b[16];
    snprintf(b, sizeof b, "#%02X%02X%02X", c.r, c.g, c.b);
    return b;
}

// ------------------------------------------------------------------------------------------ styles
Style Style::operator+(const Style& o) const {
    Style s = *this;
    if (o.fg.set) s.fg = o.fg;
    if (o.bg.set) s.bg = o.bg;
    if (o.bold >= 0) s.bold = o.bold;
    if (o.dim >= 0) s.dim = o.dim;
    if (o.italic >= 0) s.italic = o.italic;
    if (o.underline >= 0) s.underline = o.underline;
    if (o.reverse >= 0) s.reverse = o.reverse;
    if (o.strike >= 0) s.strike = o.strike;
    return s;
}
bool Style::operator==(const Style& o) const {
    return fg == o.fg && bg == o.bg && bold == o.bold && dim == o.dim && italic == o.italic && underline == o.underline &&
           reverse == o.reverse && strike == o.strike;
}
static int8_t* attr_of(Style& s, const std::string& w) {
    if (w == "bold" || w == "b") return &s.bold;
    if (w == "dim" || w == "d") return &s.dim;
    if (w == "italic" || w == "i") return &s.italic;
    if (w == "underline" || w == "u" || w == "underline2" || w == "uu") return &s.underline;
    if (w == "reverse" || w == "r") return &s.reverse;
    if (w == "strike" || w == "s") return &s.strike;
    return nullptr;
}
Style parse_style(const std::string& text) {
    static std::mutex mu;
    static std::unordered_map<std::string, Style> cache;
    {
        std::lock_guard<std::mutex> g(mu);
        auto it = cache.find(text);
        if (it != cache.end()) return it->second;
    }
    Style s;
    std::vector<std::string> words;
    std::string cur;
    for (char c : text) {
        if (isspace((unsigned char)c)) {
            if (!cur.empty()) words.push_back(cur), cur.clear();
        } else cur += c;
    }
    if (!cur.empty()) words.push_back(cur);
    for (size_t i = 0; i < words.size(); i++) {
        std::string w = words[i];
        for (auto& c : w) c = (char)tolower((unsigned char)c);
        if (w == "none" || w == "null") continue;
        if (w == "on" && i + 1 < words.size()) {
            Color c;
            if (parse_color(words[++i], c)) s.bg = c;
            continue;
        }
        if (w == "not" && i + 1 < words.size()) {
            std::string n = words[++i];
            if (int8_t* p = attr_of(s, n)) *p = 0;
            continue;
        }
        if (int8_t* p = attr_of(s, w)) {
            *p = 1;
            continue;
        }
        if (w == "blink" || w == "blink2" || w == "conceal" || w == "frame" || w == "encircle" || w == "overline" || w.rfind("link", 0) == 0) continue;
        Color c;
        if (parse_color(words[i], c)) s.fg = c;
    }
    std::lock_guard<std::mutex> g(mu);
    cache[text] = s;
    return s;
}
static Style style_arg(const var& v) {
    if (v.is_none() || v.is_unbound()) return Style();
    if (is_str(v)) return parse_style(str_utf8(v));
    return parse_style(str_(v));
}

// ------------------------------------------------------------------------------------------ markup
TextData markup_text(const std::string& markup, const Style& base) {
    TextData t;
    t.base = base;
    std::u32string s = u32(markup);
    struct Open { int at; std::string tag; };
    std::vector<Open> stack;
    std::vector<Span> spans;
    size_t i = 0, n = s.size();
    auto tag_at = [&](size_t p, size_t& end) -> bool {   // [a-z#/@][^[]*?]
        if (p >= n || s[p] != U'[' || p + 1 >= n) return false;
        char32_t c = s[p + 1];
        if (!((c >= U'a' && c <= U'z') || c == U'#' || c == U'/' || c == U'@')) return false;
        for (size_t q = p + 1; q < n; q++) {
            if (s[q] == U'[') return false;
            if (s[q] == U']') { end = q; return true; }
        }
        return false;
    };
    while (i < n) {
        if (s[i] == U'\\') {
            size_t j = i;
            while (j < n && s[j] == U'\\') j++;
            size_t end;
            if (tag_at(j, end)) {
                size_t count = j - i;
                t.plain.append(count / 2, U'\\');
                if (count % 2) {   // escaped: the tag is text
                    t.plain += s.substr(j, end - j + 1);
                    i = end + 1;
                } else
                    i = j;
                continue;
            }
            t.plain.append(j - i, U'\\');
            i = j;
            continue;
        }
        size_t end;
        if (s[i] == U'[' && tag_at(i, end)) {
            std::string tag = utf8(s.substr(i + 1, end - i - 1));
            i = end + 1;
            if (tag[0] == '/') {
                std::string name = tag.substr(1);
                while (!name.empty() && isspace((unsigned char)name.back())) name.pop_back();
                int k = (int)stack.size() - 1;
                if (!name.empty()) {
                    while (k >= 0 && stack[k].tag != name) k--;
                }
                if (k >= 0) {
                    Open o = stack[k];
                    stack.erase(stack.begin() + k);
                    if (o.tag[0] != '@') spans.push_back({o.at, (int)t.plain.size(), parse_style(o.tag)});
                }
            } else
                stack.push_back({(int)t.plain.size(), tag});
            continue;
        }
        t.plain += s[i++];
    }
    while (!stack.empty()) {
        Open o = stack.back();
        stack.pop_back();
        if (o.tag[0] != '@') spans.push_back({o.at, (int)t.plain.size(), parse_style(o.tag)});
    }
    std::reverse(spans.begin(), spans.end());
    std::stable_sort(spans.begin(), spans.end(), [](const Span& a, const Span& b) { return a.a < b.a; });
    t.spans = spans;
    return t;
}

// ------------------------------------------------------------------------------------------ Text
static TypeObj* T_text = nullptr;
struct TextObj : DataObj { TextData d; std::string style_text; };
static TextObj* TO(const var& v) { return static_cast<TextObj*>(v.ref()); }
bool text_data(const var& v, TextData& out) {
    if (!v.is_ref() || v.ref()->type != T_text) return false;
    out = TO(v)->d;
    return true;
}
var make_text(TextData t) {
    auto* o = new TextObj();
    o->type = T_text;
    o->d = std::move(t);
    return var(static_cast<Obj*>(o));
}
static void append_any(TextData& d, const var& x, const var& style) {
    TextData other;
    if (text_data(x, other)) {
        int off = (int)d.plain.size(), len = (int)other.plain.size();
        d.plain += other.plain;
        if (!(other.base == Style())) d.spans.push_back({off, off + len, other.base});
        for (auto sp : other.spans) d.spans.push_back({sp.a + off, sp.b + off, sp.s});
        return;
    }
    if (x.is_none()) return;
    std::u32string s = u32(is_str(x) ? str_utf8(x) : str_(x));
    int off = (int)d.plain.size();
    d.plain += s;
    if (!style.is_none() && !style.is_unbound()) {
        Style st = style_arg(style);
        if (!(st == Style()) && !s.empty()) d.spans.push_back({off, off + (int)s.size(), st});
    }
}
static var assemble(const CallArgs& a, size_t first) {
    TextData d;
    d.base = style_arg(kwarg(a, "style", None));
    var j = kwarg(a, "justify", None);
    if (is_str(j)) d.justify = str_utf8(j);
    for (size_t i = first; i < a.pos.size(); i++) {
        const var& p = a.pos[i];
        if (is_tuple(p)) {
            auto& items = tuple_items(p);
            append_any(d, items.at(0), items.size() > 1 ? items[1] : var(None));
        } else
            append_any(d, p, None);
    }
    return make_text(std::move(d));
}

// ------------------------------------------------------------------------------------------ text rendering
// rich's divide_line: break offsets so each line fits `width` (words fold where longer than the width)
static std::vector<int> divide_line(const std::u32string& s, int width, bool fold) {
    std::vector<int> breaks;
    int cell_offset = 0;
    size_t i = 0, n = s.size();
    auto isws = [](char32_t c) { return c == U' ' || c == U'\t' || c == U'\n' || c == U'\r' || c == 0x3000 || c == 0xA0; };
    while (i < n) {
        size_t start = i;
        while (i < n && isws(s[i])) i++;
        if (i >= n) break;
        while (i < n && !isws(s[i])) i++;
        size_t wend_core = i;
        while (i < n && isws(s[i])) i++;
        std::u32string word = s.substr(start, i - start);
        int word_length = cell_len(s.substr(start, wend_core - start));
        int remaining = width - cell_offset;
        if (remaining >= word_length) {
            cell_offset += cell_len(word);
        } else if (word_length > width) {
            if (fold) {
                // chop_cells(word, width)
                std::vector<std::u32string> parts;
                std::u32string cur;
                int w = 0;
                for (char32_t c : word) {
                    int cw = cwidth(c);
                    if (w + cw > width && !cur.empty()) {
                        parts.push_back(cur);
                        cur.clear();
                        w = 0;
                    }
                    cur += c;
                    w += cw;
                }
                if (!cur.empty()) parts.push_back(cur);
                size_t st = start;
                for (size_t k = 0; k < parts.size(); k++) {
                    if (st) breaks.push_back((int)st);
                    if (k + 1 == parts.size()) cell_offset = cell_len(parts[k]);
                    else st += parts[k].size();
                }
            } else {
                if (start) breaks.push_back((int)start);
                cell_offset = cell_len(word);
            }
        } else if (cell_offset && start) {
            breaks.push_back((int)start);
            cell_offset = cell_len(word);
        }
    }
    return breaks;
}
static Line fit(const std::u32string& chars, const std::vector<Style>& st, int width, const Style& pad, const std::string& justify) {
    Line out;
    int w = 0;
    for (size_t i = 0; i < chars.size(); i++) {
        int cw = cwidth(chars[i]);
        if (cw == 0) continue;
        if (w + cw > width) {
            if (w < width) out.push_back({U' ', st[i]}), w++;
            break;
        }
        out.push_back({chars[i], st[i]});
        if (cw == 2) out.push_back({0, st[i]});
        w += cw;
    }
    int extra = width - w;
    if (extra > 0) {
        if (justify == "right") {
            Line l(extra, Cell{U' ', pad});
            l.insert(l.end(), out.begin(), out.end());
            return l;
        }
        if (justify == "center") {
            int left = extra / 2;
            Line l(left, Cell{U' ', pad});
            l.insert(l.end(), out.begin(), out.end());
            l.insert(l.end(), extra - left, Cell{U' ', pad});
            return l;
        }
        out.insert(out.end(), extra, Cell{U' ', pad});
    }
    return out;
}
std::vector<Line> render_text(const TextData& t, int width, const Style& base, bool wrap, const std::string& justify_over) {
    std::vector<Line> lines;
    if (width <= 0) width = 0;
    Style b = base + t.base;
    std::vector<Style> st(t.plain.size(), b);
    for (auto& sp : t.spans) {
        int a = std::max(0, sp.a), e = std::min((int)t.plain.size(), sp.b);
        for (int i = a; i < e; i++) st[i] = st[i] + sp.s;
    }
    std::string justify = !justify_over.empty() ? justify_over : t.justify;
    size_t start = 0;
    for (;;) {
        size_t nl = t.plain.find(U'\n', start);
        size_t end = nl == std::u32string::npos ? t.plain.size() : nl;
        // the line, its tabs expanded
        std::u32string chars;
        std::vector<Style> cst;
        int col = 0;
        for (size_t i = start; i < end; i++) {
            if (t.plain[i] == U'\t') {
                int k = t.tab_size - (col % t.tab_size);
                for (int j = 0; j < k; j++) chars += U' ', cst.push_back(st[i]);
                col += k;
            } else if (t.plain[i] != U'\r') {
                chars += t.plain[i];
                cst.push_back(st[i]);
                col += cwidth(t.plain[i]);
            }
        }
        if (wrap && !t.no_wrap && width > 0) {
            std::vector<int> br = divide_line(chars, width, !t.crop);
            size_t s0 = 0;
            br.push_back((int)chars.size());
            for (int b2 : br) {
                std::u32string piece = chars.substr(s0, b2 - s0);
                std::vector<Style> ps(cst.begin() + s0, cst.begin() + b2);
                // rstrip_end(width): trailing whitespace past the width goes
                while (!piece.empty() && cell_len(piece) > width && piece.back() == U' ') piece.pop_back(), ps.pop_back();
                lines.push_back(fit(piece, ps, width, b, justify));
                s0 = b2;
            }
        } else
            lines.push_back(fit(chars, cst, width, b, justify));
        if (nl == std::u32string::npos) break;
        start = nl + 1;
    }
    return lines;
}
static void measure_text(const TextData& t, int& mn, int& mx) {
    mx = 0;
    size_t start = 0;
    for (;;) {
        size_t nl = t.plain.find(U'\n', start);
        size_t end = nl == std::u32string::npos ? t.plain.size() : nl;
        mx = std::max(mx, cell_len(t.plain.substr(start, end - start)));
        if (nl == std::u32string::npos) break;
        start = nl + 1;
    }
    mn = 0;
    bool any = false;
    size_t i = 0;
    while (i < t.plain.size()) {
        while (i < t.plain.size() && (t.plain[i] == U' ' || t.plain[i] == U'\n' || t.plain[i] == U'\t')) i++;
        size_t s = i;
        while (i < t.plain.size() && !(t.plain[i] == U' ' || t.plain[i] == U'\n' || t.plain[i] == U'\t')) i++;
        if (i > s) any = true, mn = std::max(mn, cell_len(t.plain.substr(s, i - s)));
    }
    if (!any) mn = mx;
    if (t.no_wrap) mn = mx;
}

// ------------------------------------------------------------------------------------------ Table
static TypeObj* T_table = nullptr;
struct Column {
    Style style;
    std::string justify = "left";
    int width = -1;
    int ratio = -1;
    bool no_wrap = false;
    var header;
};
struct TableObj : DataObj {
    std::vector<Column> cols;
    std::vector<std::vector<var>> rows;
    std::vector<Style> row_styles;
    int pt = 0, pr = 1, pb = 0, pl = 1;
    bool expand = false, collapse_padding = false, pad_edge = true, show_header = true, grid = false;
};
static TableObj* TB(const var& v) { return static_cast<TableObj*>(v.ref()); }
static void padding_of(const var& p, int& t, int& r, int& b, int& l) {
    if (is_int(p)) { t = r = b = l = (int)p.as_int64(); return; }
    std::vector<var> v = collect(p);
    if (v.size() == 1) t = r = b = l = (int)v[0].as_int64();
    else if (v.size() == 2) t = b = (int)v[0].as_int64(), r = l = (int)v[1].as_int64();
    else if (v.size() == 4) t = (int)v[0].as_int64(), r = (int)v[1].as_int64(), b = (int)v[2].as_int64(), l = (int)v[3].as_int64();
}
// the padding of a column's cells (rich's Table._get_padding_width / get_padding)
static void cell_padding(const TableObj* tb, size_t col, bool first_row, bool last_row, int& t, int& r, int& b, int& l) {
    t = tb->pt, r = tb->pr, b = tb->pb, l = tb->pl;
    bool first_col = col == 0, last_col = col + 1 == tb->cols.size();
    if (tb->collapse_padding) {
        if (!first_col) l = std::max(0, l - r);
        if (!last_row) b = std::max(0, t - b);
    }
    if (!tb->pad_edge) {
        if (first_col) l = 0;
        if (last_col) r = 0;
        if (first_row) t = 0;
        if (last_row) b = 0;
    }
}
static int pad_width(const TableObj* tb, size_t col) {
    int t, r, b, l;
    cell_padding(tb, col, false, false, t, r, b, l);
    return l + r;
}
static long long pyround(double x) { return (long long)std::nearbyint(x); }   // (banker's, as Python's round)
static std::vector<int> ratio_reduce(int total, std::vector<int> ratios, const std::vector<int>& maximums, const std::vector<int>& values) {
    for (size_t i = 0; i < ratios.size(); i++)
        if (!maximums[i]) ratios[i] = 0;
    int total_ratio = 0;
    for (int r : ratios) total_ratio += r;
    if (!total_ratio) return values;
    int remaining = total;
    std::vector<int> out;
    for (size_t i = 0; i < ratios.size(); i++) {
        if (ratios[i] && total_ratio > 0) {
            int d = (int)std::min<long long>(maximums[i], pyround((double)ratios[i] * remaining / total_ratio));
            out.push_back(values[i] - d);
            remaining -= d;
            total_ratio -= ratios[i];
        } else
            out.push_back(values[i]);
    }
    return out;
}
static std::vector<int> ratio_distribute(int total, std::vector<int> ratios, const std::vector<int>* minimums = nullptr) {
    if (minimums)
        for (size_t i = 0; i < ratios.size(); i++)
            if (!(*minimums)[i]) ratios[i] = 0;
    int total_ratio = 0;
    for (int r : ratios) total_ratio += r;
    int remaining = total;
    std::vector<int> out;
    for (size_t i = 0; i < ratios.size(); i++) {
        int mn = minimums ? (*minimums)[i] : 0;
        int d = total_ratio > 0 ? std::max(mn, (int)std::ceil((double)ratios[i] * remaining / total_ratio)) : remaining;
        out.push_back(d);
        total_ratio -= ratios[i];
        remaining -= d;
    }
    return out;
}
static void measure_column(const TableObj* tb, size_t c, int max_width, int& mn, int& mx) {
    const Column& col = tb->cols[c];
    int pw = pad_width(tb, c);
    if (col.width >= 0) {
        mn = mx = std::min(col.width + pw, max_width);
        return;
    }
    mn = 0, mx = 0;
    bool any = false;
    for (auto& row : tb->rows) {
        if (c >= row.size()) continue;
        int a, b;
        measure(row[c], max_width, a, b);
        a += pw, b += pw;
        mn = std::max(mn, a), mx = std::max(mx, b);
        any = true;
    }
    if (!any) mn = mx = pw;
    mn = std::min(mn, max_width), mx = std::min(mx, max_width);
    if (mn > mx) mn = mx;
}
static std::vector<int> column_widths(const TableObj* tb, int max_width) {
    size_t n = tb->cols.size();
    std::vector<int> mins(n), maxs(n), widths(n);
    for (size_t i = 0; i < n; i++) {
        measure_column(tb, i, max_width, mins[i], maxs[i]);
        widths[i] = maxs[i] ? maxs[i] : 1;
    }
    if (tb->expand) {
        std::vector<int> ratios;
        bool any = false;
        for (auto& c : tb->cols)
            if (c.ratio >= 0) ratios.push_back(c.ratio), any = any || c.ratio > 0;
        if (any) {
            std::vector<int> fixed(n), flex_min;
            int fixed_sum = 0;
            for (size_t i = 0; i < n; i++) {
                fixed[i] = tb->cols[i].ratio >= 0 ? 0 : maxs[i];
                fixed_sum += fixed[i];
                if (tb->cols[i].ratio >= 0) flex_min.push_back((tb->cols[i].width > 0 ? tb->cols[i].width : 1) + pad_width(tb, i));
            }
            std::vector<int> flex = ratio_distribute(max_width - fixed_sum, ratios, &flex_min);
            size_t k = 0;
            for (size_t i = 0; i < n; i++)
                if (tb->cols[i].ratio >= 0) widths[i] = fixed[i] + flex[k++];
        }
    }
    int table_width = 0;
    for (int w : widths) table_width += w;
    if (table_width > max_width) {
        std::vector<bool> wrapable(n);
        for (size_t i = 0; i < n; i++) wrapable[i] = tb->cols[i].width < 0 && !tb->cols[i].no_wrap;
        // _collapse_widths
        int total = table_width, excess = total - max_width;
        bool anyw = false;
        for (bool b : wrapable) anyw = anyw || b;
        if (anyw) {
            while (total && excess > 0) {
                int max_col = 0;
                for (size_t i = 0; i < n; i++)
                    if (wrapable[i]) max_col = std::max(max_col, widths[i]);
                int second = 0;
                for (size_t i = 0; i < n; i++)
                    if (wrapable[i] && widths[i] != max_col) second = std::max(second, widths[i]);
                int diff = max_col - second;
                std::vector<int> ratios(n);
                bool anyr = false;
                for (size_t i = 0; i < n; i++) ratios[i] = (widths[i] == max_col && wrapable[i]) ? 1 : 0, anyr = anyr || ratios[i];
                if (!anyr || !diff) break;
                std::vector<int> max_reduce(n, std::min(excess, diff));
                widths = ratio_reduce(excess, ratios, max_reduce, widths);
                total = 0;
                for (int w : widths) total += w;
                excess = total - max_width;
            }
        }
        table_width = 0;
        for (int w : widths) table_width += w;
        if (table_width > max_width) {
            widths = ratio_reduce(table_width - max_width, std::vector<int>(n, 1), widths, widths);
            table_width = 0;
            for (int w : widths) table_width += w;
        }
        for (size_t i = 0; i < n; i++) {
            int a, b;
            measure_column(tb, i, widths[i], a, b);
            widths[i] = b;
        }
        table_width = 0;
        for (int w : widths) table_width += w;
    }
    if (table_width < max_width && tb->expand) {
        std::vector<int> pads = ratio_distribute(max_width - table_width, widths);
        for (size_t i = 0; i < n; i++) widths[i] += pads[i];
    }
    return widths;
}
static std::vector<Line> render_table(const TableObj* tb, int width, const Style& base) {
    std::vector<Line> out;
    if (tb->cols.empty()) return out;
    std::vector<int> widths = column_widths(tb, width);
    int table_width = 0;
    for (int w : widths) table_width += w;
    size_t nrows = tb->rows.size();
    for (size_t r = 0; r < nrows; r++) {
        const auto& row = tb->rows[r];
        Style rs = base + tb->row_styles[r];
        std::vector<std::vector<Line>> cells;
        size_t height = 0;
        for (size_t c = 0; c < tb->cols.size(); c++) {
            int t, rp, b, l;
            cell_padding(tb, c, r == 0, r + 1 == nrows, t, rp, b, l);
            const Column& col = tb->cols[c];
            Style cs = rs + col.style;
            int inner = std::max(0, widths[c] - l - rp);
            std::vector<Line> lines;
            var cell = c < row.size() ? row[c] : var(None);
            TextData td;
            if (is_str(cell)) {
                td = markup_text(str_utf8(cell));
                td.no_wrap = td.no_wrap || col.no_wrap;
                lines = render_text(td, inner, cs, true, col.justify);
            } else if (text_data(cell, td)) {
                td.no_wrap = td.no_wrap || col.no_wrap;
                lines = render_text(td, inner, cs, true, td.justify.empty() ? col.justify : td.justify);
            } else if (cell.is_none())
                lines = {Line(inner, Cell{U' ', cs})};
            else
                lines = render(cell, inner, cs);
            if (lines.empty()) lines.push_back(Line(inner, Cell{U' ', cs}));
            std::vector<Line> padded;
            for (int k = 0; k < t; k++) padded.push_back(Line(widths[c], Cell{U' ', cs}));
            for (auto& ln : lines) {
                Line x(l, Cell{U' ', cs});
                x.insert(x.end(), ln.begin(), ln.end());
                x.insert(x.end(), rp, Cell{U' ', cs});
                x.resize(widths[c], Cell{U' ', cs});
                padded.push_back(x);
            }
            for (int k = 0; k < b; k++) padded.push_back(Line(widths[c], Cell{U' ', cs}));
            height = std::max(height, padded.size());
            cells.push_back(std::move(padded));
        }
        for (size_t h = 0; h < height; h++) {
            Line ln;
            for (size_t c = 0; c < cells.size(); c++) {
                Style cs = rs + tb->cols[c].style;
                if (h < cells[c].size()) ln.insert(ln.end(), cells[c][h].begin(), cells[c][h].end());
                else ln.insert(ln.end(), widths[c], Cell{U' ', cs});
            }
            ln.resize(std::max(table_width, width), Cell{U' ', base});
            if ((int)ln.size() > width) ln.resize(width);
            out.push_back(ln);
        }
    }
    return out;
}
static void measure_table(const TableObj* tb, int max_width, int& mn, int& mx) {
    std::vector<int> w = column_widths(tb, max_width);
    int total = 0;
    for (int x : w) total += x;
    mn = mx = 0;
    for (size_t i = 0; i < tb->cols.size(); i++) {
        int a, b;
        measure_column(tb, i, total, a, b);
        mn += a, mx += b;
    }
}

// ------------------------------------------------------------------------------------------ Syntax
static TypeObj* T_syntax = nullptr;
struct SyntaxObj : DataObj {
    std::string code, lexer;
    bool line_numbers = false, word_wrap = false;
    int start_line = 1, tab_size = 4;
    Color background;
};
static SyntaxObj* SY(const var& v) { return static_cast<SyntaxObj*>(v.ref()); }

// monokai (pygments' colours, as rich reads them)
enum Tok { TText, TComment, TKeyword, TKeywordNs, TOperator, TPunct, TName, TFunction, TNumber, TString, TEscape, TError, TBuiltin, TLabel };
static Style tok_style(Tok t) {
    auto fg = [](const char* h) { Style s; parse_color(h, s.fg); return s; };
    switch (t) {
    case TComment: return fg("#959077");
    case TKeyword: return fg("#66d9ef");
    case TKeywordNs: case TOperator: return fg("#ff4689");
    case TFunction: return fg("#a6e22e");
    case TNumber: case TEscape: return fg("#ae81ff");
    case TString: return fg("#e6db74");
    case TError: { Style s = fg("#ed007e"); parse_color("#1e0010", s.bg); return s; }
    default: return fg("#f8f8f2");
    }
}
struct Token { size_t a, b; Tok t; };
static bool is_ident_start(char32_t c) { return c == U'_' || (c >= U'a' && c <= U'z') || (c >= U'A' && c <= U'Z') || c >= 0x80; }
static bool is_ident(char32_t c) { return is_ident_start(c) || (c >= U'0' && c <= U'9'); }

// pygments' CLexer, its token classes: comments and the preprocessor, strings (escapes), numbers, keywords and
// types, a function's name where it is declared or defined, operators and punctuation
static std::vector<Token> lex_c(const std::u32string& s) {
    static const std::unordered_set<std::string> keywords = {
        "asm", "auto", "break", "case", "const", "continue", "default", "do", "else", "enum", "extern", "for", "goto", "if",
        "register", "restricted", "return", "sizeof", "struct", "static", "switch", "typedef", "volatile", "while", "union",
        "thread_local", "alignas", "alignof", "static_assert", "_Pragma", "inline", "restrict", "_Alignas", "_Alignof",
        "_Noreturn", "_Static_assert", "_Thread_local", "_Atomic", "_Complex", "_Imaginary", "_Generic",
        "_inline", "__inline", "naked", "thread", "__asm", "__int8", "__int16", "__int32", "__int64", "__wchar_t",
        "__declspec", "__fastcall", "__stdcall", "__cdecl", "__finally", "__except", "__try", "__leave", "__based",
        "__noop", "__assume", "__identifier", "__forceinline", "__w64", "__ptr64", "__ptr32", "__unaligned", "__raise",
        "__alignof", "__event", "__if_exists", "__if_not_exists", "__interface", "__virtual_inheritance", "__multiple_inheritance",
        "__single_inheritance", "__hook", "__unhook", "__super", "__uuidof", "__restrict", "__sptr", "__uptr",
        "int", "long", "float", "short", "double", "char", "unsigned", "signed", "void", "_Bool", "_BitInt",
        "int8_t", "int16_t", "int32_t", "int64_t", "uint8_t", "uint16_t", "uint32_t", "uint64_t", "int_least8_t",
        "int_least16_t", "int_least32_t", "int_least64_t", "uint_least8_t", "uint_least16_t", "uint_least32_t",
        "uint_least64_t", "int_fast8_t", "int_fast16_t", "int_fast32_t", "int_fast64_t", "uint_fast8_t", "uint_fast16_t",
        "uint_fast32_t", "uint_fast64_t", "intptr_t", "uintptr_t", "intmax_t", "uintmax_t", "clock_t", "clockid_t",
        "cpu_set_t", "cpumask_t", "dev_t", "gid_t", "id_t", "ino_t", "key_t", "mode_t", "nfds_t", "pid_t", "rlim_t",
        "sig_t", "sighandler_t", "siginfo_t", "sigset_t", "sigval_t", "socklen_t", "timer_t", "uid_t", "size_t",
        "ssize_t", "off_t", "wchar_t", "ptrdiff_t", "sig_atomic_t", "fpos_t", "time_t", "va_list", "jmp_buf", "FILE",
        "DIR", "div_t", "ldiv_t", "mbstate_t", "wctrans_t", "wint_t", "wctype_t", "_Decimal32", "_Decimal64", "_Decimal128",
        "true", "false", "NULL", "bool",
    };
    std::vector<Token> out;
    size_t i = 0, n = s.size();
    int depth = 0;
    bool line_start = true;
    while (i < n) {
        char32_t c = s[i];
        if (c == U'\n') { out.push_back({i, i + 1, TText}); i++; line_start = true; continue; }
        if (c == U' ' || c == U'\t' || c == U'\r') { size_t a = i; while (i < n && (s[i] == U' ' || s[i] == U'\t' || s[i] == U'\r')) i++; out.push_back({a, i, TText}); continue; }
        bool at_start = line_start;
        line_start = false;
        if (c == U'#' && at_start) {   // a preprocessor line (with its continuations)
            size_t a = i;
            while (i < n && s[i] != U'\n') {
                if (s[i] == U'\\' && i + 1 < n && s[i + 1] == U'\n') i += 2;
                else i++;
            }
            out.push_back({a, i, TComment});
            continue;
        }
        if (c == U'/' && i + 1 < n && s[i + 1] == U'/') { size_t a = i; while (i < n && s[i] != U'\n') i++; out.push_back({a, i, TComment}); continue; }
        if (c == U'/' && i + 1 < n && s[i + 1] == U'*') {
            size_t a = i;
            size_t e = s.find(U"*/", i + 2);
            i = e == std::u32string::npos ? n : e + 2;
            out.push_back({a, i, TComment});
            continue;
        }
        if (c == U'"' || (c == U'L' && i + 1 < n && s[i + 1] == U'"')) {
            size_t a = i;
            if (c == U'L') i++;
            i++;
            size_t seg = a;
            while (i < n && s[i] != U'"' && s[i] != U'\n') {
                if (s[i] == U'\\' && i + 1 < n) {
                    if (i > seg) out.push_back({seg, i, TString});
                    size_t e = i + 2;
                    if (s[i + 1] == U'x') while (e < n && hexv((char)s[e]) >= 0 && s[e] < 128) e++;
                    else if (s[i + 1] >= U'0' && s[i + 1] <= U'7') while (e < n && e < i + 4 && s[e] >= U'0' && s[e] <= U'7') e++;
                    out.push_back({i, e, TEscape});
                    i = seg = e;
                    continue;
                }
                i++;
            }
            if (i < n && s[i] == U'"') i++;
            out.push_back({seg, i, TString});
            continue;
        }
        if (c == U'\'') {
            size_t a = i++;
            while (i < n && s[i] != U'\'' && s[i] != U'\n') i += (s[i] == U'\\' && i + 1 < n) ? 2 : 1;
            if (i < n && s[i] == U'\'') i++;
            out.push_back({a, i, TString});
            continue;
        }
        if ((c >= U'0' && c <= U'9') || (c == U'.' && i + 1 < n && s[i + 1] >= U'0' && s[i + 1] <= U'9')) {
            size_t a = i;
            if (c == U'0' && i + 1 < n && (s[i + 1] == U'x' || s[i + 1] == U'X')) {
                i += 2;
                while (i < n && s[i] < 128 && hexv((char)s[i]) >= 0) i++;
            } else {
                while (i < n && ((s[i] >= U'0' && s[i] <= U'9') || s[i] == U'.')) i++;
                if (i < n && (s[i] == U'e' || s[i] == U'E')) {
                    i++;
                    if (i < n && (s[i] == U'+' || s[i] == U'-')) i++;
                    while (i < n && s[i] >= U'0' && s[i] <= U'9') i++;
                }
            }
            while (i < n && (s[i] == U'u' || s[i] == U'U' || s[i] == U'l' || s[i] == U'L' || s[i] == U'f' || s[i] == U'F')) i++;
            out.push_back({a, i, TNumber});
            continue;
        }
        if (is_ident_start(c)) {
            size_t a = i;
            while (i < n && is_ident(s[i])) i++;
            std::string w = utf8(s.substr(a, i - a));
            size_t j = i;
            while (j < n && (s[j] == U' ' || s[j] == U'\t')) j++;
            Tok t = TName;
            if (keywords.count(w)) t = TKeyword;
            else if (j < n && s[j] == U'(' && depth == 0) t = TFunction;
            else if (at_start && j < n && s[j] == U':' && !(j + 1 < n && s[j + 1] == U':')) t = TLabel;
            out.push_back({a, i, t});
            continue;
        }
        if (c == U'{') depth++;
        if (c == U'}') depth = std::max(0, depth - 1);
        if (std::u32string(U"-+*/%=<>!&|^~?:").find(c) != std::u32string::npos) {
            size_t a = i;
            while (i < n && std::u32string(U"-+*/%=<>!&|^~?:").find(s[i]) != std::u32string::npos && !(s[i] == U'/' && i + 1 < n && (s[i + 1] == U'/' || s[i + 1] == U'*'))) i++;
            out.push_back({a, i, TOperator});
            continue;
        }
        out.push_back({i, i + 1, TPunct});
        i++;
    }
    return out;
}

// pygments' NasmLexer (case-insensitive): at a line's start a label, a directive or a mnemonic (anything else is an
// error token there); then the instruction's arguments: strings, numbers, registers, punctuation, names
static std::vector<Token> lex_nasm(const std::u32string& s) {
    std::vector<Token> out;
    size_t i = 0, n = s.size();
    bool args = false;
    auto lower = [](char32_t c) { return (c >= U'A' && c <= U'Z') ? (char32_t)(c + 32) : c; };
    auto id_start = [&](char32_t c) { c = lower(c); return (c >= U'a' && c <= U'z') || c == U'$' || c == U'.' || c == U'_' || c == U'?'; };
    auto id_char = [&](char32_t c) { return id_start(c) || (c >= U'0' && c <= U'9') || c == U'#' || c == U'@' || c == U'~' || c >= 0x80; };
    auto word_at = [&](size_t p) { size_t e = p; while (e < n && id_char(s[e])) e++; std::string w; for (size_t k = p; k < e; k++) w += (char)lower(s[k]); return std::make_pair(e, w); };
    auto is_hexd = [&](char32_t c) { c = lower(c); return (c >= U'0' && c <= U'9') || (c >= U'a' && c <= U'f'); };
    auto is_register = [](const std::string& w) {
        static const std::unordered_set<std::string> regs = {
            "al", "ah", "bl", "bh", "cl", "ch", "dl", "dh", "ax", "bx", "cx", "dx", "eax", "ebx", "ecx", "edx", "rax", "rbx",
            "rcx", "rdx", "sp", "bp", "esp", "ebp", "rsp", "rbp", "si", "di", "esi", "edi", "rsi", "rdi", "cs", "ds", "es",
            "fs", "gs", "ss", "st0", "st1", "st2", "st3", "st4", "st5", "st6", "st7", "cr0", "cr1", "cr2", "cr3", "cr4",
            "dr0", "dr1", "dr2", "dr3", "dr6", "dr7", "tr3", "tr4", "tr5", "tr6", "tr7"};
        if (regs.count(w)) return true;
        if (w.size() >= 3 && w[0] == 'm' && w[1] == 'm' && w.size() == 3 && w[2] >= '0' && w[2] <= '7') return true;
        if (w.size() >= 2 && w[0] == 'r' && isdigit((unsigned char)w[1])) return true;
        if (w.size() >= 4 && (w[0] == 'x' || w[0] == 'y' || w[0] == 'z') && w[1] == 'm' && w[2] == 'm') return true;
        if (w.size() == 2 && w[0] == 'k' && w[1] >= '0' && w[1] <= '7') return true;
        return false;
    };
    static const std::unordered_set<std::string> directives = {"bits", "use16", "use32", "section", "segment", "absolute", "extern",
                                                                "global", "org", "align", "struc", "endstruc", "common", "cpu",
                                                                "group", "uppercase", "import", "export", "library", "module"};
    auto declkw = [](const std::string& w) {
        if (w == "times") return true;
        if (w.size() == 2 && w[0] == 'd' && std::string("bwdqt").find(w[1]) != std::string::npos) return true;
        if (w.size() == 4 && w.compare(0, 3, "res") == 0 && std::string("bwdqt").find(w[3]) != std::string::npos) return true;
        return false;
    };
    while (i < n) {
        char32_t c = s[i];
        // whitespace (both states)
        if (c == U'\n' || c == U'\r') {
            size_t a = i;
            while (i < n && (s[i] == U'\n' || s[i] == U'\r')) i++;
            out.push_back({a, i, TText});
            args = false;
            continue;
        }
        if (c == U' ' || c == U'\t') { size_t a = i; while (i < n && (s[i] == U' ' || s[i] == U'\t')) i++; out.push_back({a, i, TText}); continue; }
        if (c == U';' || c == U'#') { size_t a = i; while (i < n && s[i] != U'\n') i++; out.push_back({a, i, TComment}); continue; }
        if (!args) {
            if (c == U'%') { size_t a = i; while (i < n && s[i] != U'\n') i++; out.push_back({a, i, TComment}); continue; }
            if (id_start(c)) {
                auto [e, w] = word_at(i);
                if (e < n && s[e] == U':') { out.push_back({i, e + 1, TLabel}); i = e + 1; continue; }
                Tok t = directives.count(w) && e < n && (s[e] == U' ' || s[e] == U'\t' || s[e] == U'\n') ? TKeyword : declkw(w) ? TKeyword : TFunction;
                out.push_back({i, e, t});
                i = e;
                args = true;
                continue;
            }
            out.push_back({i, i + 1, TError});
            i++;
            continue;
        }
        if (c == U'"' || c == U'\'' || c == U'`') {
            size_t a = i++;
            while (i < n && s[i] != c && s[i] != U'\n') i += (s[i] == U'\\' && i + 1 < n) ? 2 : 1;
            if (i < n && s[i] == c) i++;
            out.push_back({a, i, TString});
            continue;
        }
        if (c >= U'0' && c <= U'9') {
            size_t a = i;
            // hex: 0x.., [0-9][0-9a-f]*h ; octal q ; binary b ; float ; decimal
            if (c == U'0' && i + 1 < n && lower(s[i + 1]) == U'x' && i + 2 < n && is_hexd(s[i + 2])) {
                i += 2;
                while (i < n && is_hexd(s[i])) i++;
            } else {
                size_t e = i;
                while (e < n && is_hexd(s[e])) e++;
                if (e < n && lower(s[e]) == U'h') i = e + 1;
                else {
                    while (i < n && s[i] >= U'0' && s[i] <= U'9') i++;
                    if (i < n && lower(s[i]) == U'q') i++;
                    else if (i < n && s[i] == U'.') {
                        size_t f = i + 1;
                        if (f < n && lower(s[f]) == U'e') f++;
                        if (f < n && s[f] >= U'0' && s[f] <= U'9') { while (f < n && s[f] >= U'0' && s[f] <= U'9') f++; i = f; }
                    }
                }
            }
            out.push_back({a, i, TNumber});
            continue;
        }
        if (c == U'$' && i + 1 < n && s[i + 1] == U'0') {
            size_t a = i;
            i += 2;
            while (i < n && is_hexd(s[i])) i++;
            out.push_back({a, i, TNumber});
            continue;
        }
        if (std::u32string(U",{}():[]").find(c) != std::u32string::npos) {
            size_t a = i;
            while (i < n && std::u32string(U",{}():[]").find(s[i]) != std::u32string::npos) i++;
            out.push_back({a, i, TPunct});
            continue;
        }
        if (std::u32string(U"&|^<>+*/%~-").find(c) != std::u32string::npos) {
            size_t a = i;
            while (i < n && std::u32string(U"&|^<>+*/%~-").find(s[i]) != std::u32string::npos) i++;
            out.push_back({a, i, TOperator});
            continue;
        }
        if (c == U'$') { size_t a = i; while (i < n && s[i] == U'$') i++; out.push_back({a, i, TKeyword}); continue; }
        if (id_start(c) || id_char(c)) {
            auto [e, w] = word_at(i);
            if (e == i) e = i + 1;
            Tok t = TName;
            if (w == "seg" || w == "wrt" || w == "strict" || w == "rel" || w == "abs") t = TOperator;
            else if (w == "byte" || w == "word" || w == "dword" || w == "qword") t = TKeyword;
            else if (is_register(w)) t = TBuiltin;
            out.push_back({i, e, t});
            i = e;
            continue;
        }
        out.push_back({i, i + 1, TError});
        i++;
    }
    return out;
}
static std::vector<Line> render_syntax(const SyntaxObj* sy, int width, const Style& base0) {
    Style bg_style;
    bg_style.bg = sy->background.set ? sy->background : Color::rgb(0x27, 0x28, 0x22);
    Style base = base0 + bg_style + tok_style(TText);
    // _process_code: a final newline, tabs expanded
    std::string code = sy->code;
    bool ends_on_nl = !code.empty() && code.back() == '\n';
    if (!ends_on_nl) code += "\n";
    std::u32string s;
    {
        std::u32string raw = u32(code);
        int col = 0;
        for (char32_t c : raw) {
            if (c == U'\t') {
                int k = sy->tab_size - (col % sy->tab_size);
                s.append(k, U' ');
                col += k;
            } else {
                s += c;
                col = c == U'\n' ? 0 : col + 1;
            }
        }
    }
    std::string lx = sy->lexer;
    for (auto& ch : lx) ch = (char)tolower((unsigned char)ch);
    std::vector<Token> toks = (lx == "nasm" || lx == "asm") ? lex_nasm(s) : (lx == "c" || lx == "cpp" || lx == "c++" || lx == "h") ? lex_c(s) : std::vector<Token>{{0, s.size(), TText}};
    TextData t;
    t.plain = s;
    t.base = Style();
    for (auto& tk : toks)
        if (tk.b > tk.a) t.spans.push_back({(int)tk.a, (int)tk.b, tok_style(tk.t)});
    t.no_wrap = !sy->word_wrap;
    // the lines (Text.split: a trailing empty line kept when the code ended on a newline)
    std::vector<std::pair<size_t, size_t>> lines;
    {
        size_t st = 0;
        for (;;) {
            size_t nl = s.find(U'\n', st);
            if (nl == std::u32string::npos) {
                if (st < s.size() || ends_on_nl) lines.push_back({st, s.size()});
                break;
            }
            lines.push_back({st, nl});
            st = nl + 1;
        }
        if (!ends_on_nl && !lines.empty() && lines.back().first == lines.back().second && lines.back().first == s.size()) lines.pop_back();
    }
    std::vector<Line> out;
    int numw = 0;
    if (sy->line_numbers) {
        int count = 0;
        for (char c : sy->code) count += c == '\n';
        numw = (int)std::to_string(sy->start_line + count).size() + 2;
    }
    int code_width = std::max(0, sy->line_numbers ? width - numw - 1 : width);
    Style num_style = base;
    {
        Color b = bg_style.bg, f = tok_style(TText).fg;
        num_style.fg = Color::rgb((int)(b.r + (f.r - b.r) * 0.3), (int)(b.g + (f.g - b.g) * 0.3), (int)(b.b + (f.b - b.b) * 0.3));
    }
    Style pointer_style = base;   // (rich's highlight_number_style, for the two columns before the number)
    {
        Color b = bg_style.bg, f = tok_style(TText).fg;
        pointer_style.fg = Color::rgb((int)(b.r + (f.r - b.r) * 0.9), (int)(b.g + (f.g - b.g) * 0.9), (int)(b.b + (f.b - b.b) * 0.9));
        pointer_style.bold = 1;
    }
    int line_no = sy->start_line;
    for (auto [a, e] : lines) {
        TextData lt;
        lt.plain = s.substr(a, e - a);
        for (auto& sp : t.spans) {
            int x = std::max((int)a, sp.a), y = std::min((int)e, sp.b);
            if (y > x) lt.spans.push_back({x - (int)a, y - (int)a, sp.s});
        }
        lt.no_wrap = !sy->word_wrap;
        std::vector<Line> wrapped = render_text(lt, code_width, base, sy->word_wrap);
        for (size_t k = 0; k < wrapped.size(); k++) {
            Line ln;
            if (sy->line_numbers) {
                std::u32string num;
                if (k == 0) {
                    std::string d = std::to_string(line_no);
                    num = u32(std::string("  ") + std::string(std::max(0, numw - 2 - (int)d.size()), ' ') + d + " ");
                } else
                    num = std::u32string(numw + 1, U' ');
                for (size_t q = 0; q < num.size(); q++) ln.push_back({num[q], k == 0 && q < 2 ? pointer_style : num_style});
            }
            ln.insert(ln.end(), wrapped[k].begin(), wrapped[k].end());
            ln.resize(width, Cell{U' ', base});
            out.push_back(ln);
        }
        line_no++;
    }
    return out;
}

// ------------------------------------------------------------------------------------------ any renderable
std::vector<Line> render(const var& r, int width, const Style& base, bool wrap) {
    if (width < 0) width = 0;
    if (r.is_none() || r.is_unbound()) return {};
    if (r.is_ref()) {
        TypeObj* t = r.ref()->type;
        if (t == T_text) return render_text(TO(r)->d, width, base, wrap);
        if (t == T_table) return render_table(TB(r), width, base);
        if (t == T_syntax) return render_syntax(SY(r), width, base);
    }
    std::string s = is_str(r) ? str_utf8(r) : str_(r);
    return render_text(markup_text(s), width, base, wrap);
}
void measure(const var& r, int max_width, int& mn, int& mx) {
    mn = mx = 0;
    if (r.is_none() || r.is_unbound()) return;
    if (r.is_ref() && r.ref()->type == T_table) {
        measure_table(TB(r), max_width, mn, mx);
    } else if (r.is_ref() && r.ref()->type == T_syntax) {
        const SyntaxObj* sy = SY(r);
        int w = 0;
        std::u32string s = u32(sy->code);
        size_t st = 0;
        int count = 0;
        for (;;) {
            size_t nl = s.find(U'\n', st);
            w = std::max(w, cell_len(s.substr(st, (nl == std::u32string::npos ? s.size() : nl) - st)));
            if (nl == std::u32string::npos) break;
            st = nl + 1;
            count++;
        }
        if (sy->line_numbers) w += (int)std::to_string(sy->start_line + count).size() + 3;
        mn = mx = w;
    } else {
        TextData t;
        if (!text_data(r, t)) t = markup_text(is_str(r) ? str_utf8(r) : str_(r));
        measure_text(t, mn, mx);
    }
    mn = std::min(mn, max_width), mx = std::min(mx, max_width);
    if (mn > mx) mn = mx;
    if (mn < 0) mn = 0;
}

// ------------------------------------------------------------------------------------------ the modules
static var text_new(const CallArgs& a) {
    TextData d;
    var text = arg(a, 1, "text", ""_s);
    d.plain = u32(is_str(text) ? str_utf8(text) : str_(text));
    var style = arg(a, 2, "style", ""_s);
    d.base = style_arg(style);
    var j = kwarg(a, "justify", None);
    if (is_str(j)) d.justify = str_utf8(j);
    var nw = kwarg(a, "no_wrap", None);
    if (!nw.is_none()) d.no_wrap = nw.truthy();
    var ov = kwarg(a, "overflow", None);
    if (is_str(ov) && str_utf8(ov) == "crop") d.crop = true;
    var ts = kwarg(a, "tab_size", None);
    if (!ts.is_none()) d.tab_size = (int)ts.as_int64();
    auto* o = new TextObj();
    o->type = T_text;
    o->d = std::move(d);
    o->style_text = is_str(style) ? str_utf8(style) : "";
    return var(static_cast<Obj*>(o));
}
void ensure_rich() {
    if (T_text) return;
    T_text = new_type("Text", {}, Layout::NativeData, false, "rich.text");
    add_static(T_text, "__new__", [](const CallArgs& a) { return text_new(a); });
    add_method(T_text, "__init__", [](const CallArgs&) { return None; });
    add_method(T_text, "append", [](const CallArgs& a) {
        append_any(TO(a.pos[0])->d, arg(a, 1, "text"), arg(a, 2, "style", None));
        return a.pos[0];
    });
    add_method(T_text, "append_text", [](const CallArgs& a) {
        append_any(TO(a.pos[0])->d, a.pos.at(1), None);
        return a.pos[0];
    });
    add_method(T_text, "stylize", [](const CallArgs& a) {
        TextData& d = TO(a.pos[0])->d;
        int n = (int)d.plain.size();
        int s = (int)arg(a, 2, "start", 0).as_int64();
        var e = arg(a, 3, "end", None);
        int en = e.is_none() ? n : (int)e.as_int64();
        if (s < 0) s += n;
        if (en < 0) en += n;
        s = std::max(0, std::min(s, n)), en = std::max(0, std::min(en, n));
        if (en > s) d.spans.push_back({s, en, style_arg(arg(a, 1, "style"))});
        return None;
    });
    add_method(T_text, "copy", [](const CallArgs& a) { return make_text(TO(a.pos[0])->d); });
    add_getter(T_text, "plain", [](const var& self) { return new_str(utf8(TO(self)->d.plain)); });
    add_getter(T_text, "style", [](const var& self) { return new_str(TO(self)->style_text); });
    add_method(T_text, "__str__", [](const CallArgs& a) { return new_str(utf8(TO(a.pos[0])->d.plain)); });
    add_method(T_text, "__repr__", [](const CallArgs& a) { return new_str("<text " + repr_(new_str(utf8(TO(a.pos[0])->d.plain))) + ">"); });
    add_method(T_text, "__len__", [](const CallArgs& a) { return var((int64_t)TO(a.pos[0])->d.plain.size()); });
    add_method(T_text, "__bool__", [](const CallArgs& a) { return var(!TO(a.pos[0])->d.plain.empty()); });
    add_method(T_text, "__contains__", [](const CallArgs& a) {
        TextData o;
        std::u32string needle = text_data(a.pos.at(1), o) ? o.plain : u32(str_(a.pos[1]));
        return var(TO(a.pos[0])->d.plain.find(needle) != std::u32string::npos);
    });
    add_method(T_text, "__eq__", [](const CallArgs& a) {
        TextData o;
        if (!text_data(a.pos.at(1), o)) return var(false);
        return var(TO(a.pos[0])->d.plain == o.plain);
    });
    add_method(T_text, "__hash__", [](const CallArgs& a) { return var((int64_t)(intptr_t)a.pos[0].ref()); });
    add_method(T_text, "__add__", [](const CallArgs& a) {
        TextData d = TO(a.pos[0])->d;
        append_any(d, a.pos.at(1), None);
        return make_text(std::move(d));
    });
    add_static(T_text, "from_markup", [](const CallArgs& a) {
        TextData d = markup_text(str_utf8(arg(a, 0, "text")), style_arg(kwarg(a, "style", None)));
        var j = kwarg(a, "justify", None);
        if (is_str(j)) d.justify = str_utf8(j);
        return make_text(std::move(d));
    });
    add_static(T_text, "assemble", [](const CallArgs& a) { return assemble(a, 0); });
    add_static(T_text, "styled", [](const CallArgs& a) {
        TextData d;
        append_any(d, arg(a, 0, "text"), arg(a, 1, "style", None));
        return make_text(std::move(d));
    });

    T_table = new_type("Table", {}, Layout::NativeData, false, "rich.table");
    auto make_table = [](const CallArgs& a, bool grid, size_t first) {
        auto* tb = new TableObj();
        tb->type = T_table;
        var self(static_cast<Obj*>(tb));
        tb->grid = grid;
        if (grid) tb->pt = tb->pr = tb->pb = tb->pl = 0, tb->collapse_padding = true, tb->pad_edge = false, tb->show_header = false;
        var p = kwarg(a, "padding", None);
        if (!p.is_none()) padding_of(p, tb->pt, tb->pr, tb->pb, tb->pl);
        var cp = kwarg(a, "collapse_padding", None);
        if (!cp.is_none()) tb->collapse_padding = cp.truthy();
        var pe = kwarg(a, "pad_edge", None);
        if (!pe.is_none()) tb->pad_edge = pe.truthy();
        var ex = kwarg(a, "expand", None);
        if (!ex.is_none()) tb->expand = ex.truthy();
        var sh = kwarg(a, "show_header", None);
        if (!sh.is_none()) tb->show_header = sh.truthy();
        for (size_t i = first; i < a.pos.size(); i++) {
            Column c;
            c.header = a.pos[i];
            tb->cols.push_back(c);
        }
        return self;
    };
    add_static(T_table, "__new__", [make_table](const CallArgs& a) { return make_table(a, false, 1); });
    add_method(T_table, "__init__", [](const CallArgs&) { return None; });
    add_static(T_table, "grid", [make_table](const CallArgs& a) { return make_table(a, true, 0); });
    add_method(T_table, "add_column", [](const CallArgs& a) {
        TableObj* tb = TB(a.pos[0]);
        Column c;
        c.header = arg(a, 1, "header", ""_s);
        c.style = style_arg(kwarg(a, "style", None));
        var j = kwarg(a, "justify", None);
        if (is_str(j)) c.justify = str_utf8(j);
        var w = kwarg(a, "width", None);
        if (!w.is_none()) c.width = (int)w.as_int64();
        var r = kwarg(a, "ratio", None);
        if (!r.is_none()) c.ratio = (int)r.as_int64();
        c.no_wrap = kwarg(a, "no_wrap", false).truthy();
        tb->cols.push_back(c);
        return None;
    });
    add_method(T_table, "add_row", [](const CallArgs& a) {
        TableObj* tb = TB(a.pos[0]);
        std::vector<var> cells(a.pos.begin() + 1, a.pos.end());
        while (cells.size() > tb->cols.size()) tb->cols.push_back(Column());   // (rich adds the columns a row needs)
        tb->rows.push_back(cells);
        tb->row_styles.push_back(style_arg(kwarg(a, "style", None)));
        return None;
    });
    add_getter(T_table, "row_count", [](const var& self) { return var((int64_t)TB(self)->rows.size()); });

    T_syntax = new_type("Syntax", {}, Layout::NativeData, false, "rich.syntax");
    add_static(T_syntax, "__new__", [](const CallArgs& a) {
        auto* sy = new SyntaxObj();
        sy->type = T_syntax;
        var self(static_cast<Obj*>(sy));
        sy->code = str_utf8(arg(a, 1, "code"));
        sy->lexer = str_(arg(a, 2, "lexer"));
        sy->line_numbers = kwarg(a, "line_numbers", false).truthy();
        sy->word_wrap = kwarg(a, "word_wrap", false).truthy();
        var sl = kwarg(a, "start_line", None);
        if (!sl.is_none()) sy->start_line = (int)sl.as_int64();
        var ts = kwarg(a, "tab_size", None);
        if (!ts.is_none()) sy->tab_size = (int)ts.as_int64();
        var bg = kwarg(a, "background_color", None);
        if (is_str(bg)) parse_color(str_utf8(bg), sy->background);
        return self;
    });
    add_method(T_syntax, "__init__", [](const CallArgs&) { return None; });
    add_getter(T_syntax, "code", [](const var& self) { return new_str(SY(self)->code); });

}
void init_rich() {
    register_native_module("rich", [](const var& m) { ensure_rich(); mod_set(m, "__path__", new_list()); });
    register_native_module("rich.text", [](const var& m) { ensure_rich(); mod_set(m, "Text", tv(T_text)); });
    register_native_module("rich.table", [](const var& m) { ensure_rich(); mod_set(m, "Table", tv(T_table)); });
    register_native_module("rich.syntax", [](const var& m) { ensure_rich(); mod_set(m, "Syntax", tv(T_syntax)); });
}

}  // namespace tui
}  // namespace py
