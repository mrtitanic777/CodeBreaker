// tui.hpp -- what lib_rich.cc (rich's Text, Table, Syntax and markup) and lib_tui.cc (textual) share: colours,
// styles, the cells a renderable is drawn into, and rendering any renderable to lines of a given width.
#pragma once
#include <string>
#include <vector>

#include "rt.hpp"

namespace py {
namespace tui {

// ------------------------------------------------------------------------------------------ colours
// a colour with alpha (textual's Color: r g b and a 0..1); `set` false: none (rich's default, CSS's unset)
struct Color {
    bool set = false;
    int r = 0, g = 0, b = 0;
    double a = 1.0;
    bool automatic = false;   // textual's "auto": the contrasting text colour of the background, with a as its alpha
    static Color rgb(int r, int g, int b, double a = 1.0) {
        Color c;
        c.set = true, c.r = r, c.g = g, c.b = b, c.a = a;
        return c;
    }
    bool operator==(const Color& o) const { return set == o.set && r == o.r && g == o.g && b == o.b && a == o.a && automatic == o.automatic; }
    bool operator!=(const Color& o) const { return !(*this == o); }
};
bool parse_color(const std::string& text, Color& out);   // #rgb #rrggbb #rrggbbaa, names, rgb(...), transparent
Color blend(const Color& from, const Color& to, double factor);   // textual's Color.blend (alpha kept from `from`... as textual)
Color over(const Color& under, const Color& top);                 // textual's under + top: top blended in by its alpha, opaque
Color lighten(const Color& c, double amount);                     // in L*a*b, as textual's Color.lighten
Color contrast_text(const Color& bg, double alpha);               // white or black by brightness
std::string hex(const Color& c);

// ------------------------------------------------------------------------------------------ styles
// a rich style: colours and attributes, each unset (-1) or set; a + b is b over a
struct Style {
    Color fg, bg;
    int8_t bold = -1, dim = -1, italic = -1, underline = -1, reverse = -1, strike = -1;
    Style operator+(const Style& o) const;
    bool operator==(const Style& o) const;
};
Style parse_style(const std::string& text);   // "bold #61afef", "dim", "on #000000", "b", "not bold" (cached)

// ------------------------------------------------------------------------------------------ cells
struct Cell {
    char32_t ch = U' ';   // 0: the right half of a wide character
    Style s;
};
using Line = std::vector<Cell>;
int cwidth(char32_t c);                          // cells a code point takes (0 combining, 2 wide)
int cell_len(const std::u32string& s);

// ------------------------------------------------------------------------------------------ text
// rich's Text: the characters, a base style and styled spans over them (later spans win)
struct Span {
    int a = 0, b = 0;
    Style s;
};
struct TextData {
    std::u32string plain;
    Style base;
    std::vector<Span> spans;
    std::string justify;          // "", left, right, center, full
    bool no_wrap = false;
    bool crop = false;            // overflow crop (no fold of long words: cut at the width)
    int tab_size = 8;
};
TextData markup_text(const std::string& markup, const Style& base = Style());   // rich markup ([b red]...[/])
bool text_data(const var& v, TextData& out);   // a rich Text object's data (false: not a Text)
var make_text(TextData t);                     // a new rich Text object
std::vector<Line> render_text(const TextData& t, int width, const Style& base, bool wrap = true, const std::string& justify = "");

// ------------------------------------------------------------------------------------------ any renderable
// str (markup), Text, Table, Syntax, or anything else as str(): its lines at `width` (each exactly that wide,
// padded with `base`), and its minimum / maximum width
std::vector<Line> render(const var& r, int width, const Style& base, bool wrap = true);
void measure(const var& r, int max_width, int& min_w, int& max_w);
std::u32string u32(const std::string& s);
std::string utf8(const std::u32string& s);

void init_rich();     // the rich modules' registration (lib_rich.cc)
void ensure_rich();   // their types, made on first use

}  // namespace tui
}  // namespace py
