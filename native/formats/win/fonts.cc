// fonts.cc -- raster fonts (.FNT, and the FONT resources of a .FON): codebreaker/formats/fonts.py.
// A Font comes back as the dict of its attributes; glyph and render are asked by (data, at).
#include <string>
#include <utility>
#include <vector>

#include "../core/questions.h"
#include "../core/text.h"

namespace cbf::fonts {
namespace {

struct Font {
    Bytes d;
    int64_t at = 0, version = 0, size = 0;
    std::string copyright, name;
    bool vector = false, italic = false;
    int64_t points = 0, ascent = 0, weight = 0, charset = 0, width = 0, height = 0, average_width = 0,
            max_width = 0, first = 0, last = 0, def = 0, row_bytes = 0, bits = 0;
    std::vector<std::pair<int64_t, std::pair<int64_t, int64_t>>> where;   // code -> (width, offset), in order

    int64_t u16(int64_t o) const { return d.u16(at + o); }
    int64_t u32(int64_t o) const { return d.u32(at + o); }
    const std::pair<int64_t, int64_t> *find(int64_t code) const {
        for (auto &kv : where)
            if (kv.first == code) return &kv.second;
        return nullptr;
    }
    void put(int64_t code, int64_t w, int64_t off) {      // a dict assignment: later wins, position kept
        for (auto &kv : where)
            if (kv.first == code) {
                kv.second = {w, off};
                return;
            }
        where.push_back({code, {w, off}});
    }

    void strike(int64_t n) {
        std::vector<int64_t> offs;
        if (width) {
            for (int64_t i = 0; i <= n; ++i) offs.push_back(i * width);
        } else {
            for (int64_t table : {0x75, 0x76, 0x74}) {
                if (at + table + 2 * (n + 1) > d.size()) continue;
                std::vector<int64_t> got;
                for (int64_t i = 0; i <= n; ++i) got.push_back(d.u16(at + table + 2 * i));
                bool ok = true;
                for (size_t i = 0; i + 1 < got.size(); ++i)
                    if (got[i] > got[i + 1]) ok = false;
                if (ok && got.back() <= row_bytes * 8) {
                    offs = got;
                    break;
                }
            }
        }
        for (size_t i = 0; i + 1 < offs.size(); ++i) put(first + (int64_t)i, offs[i + 1] - offs[i], offs[i]);
    }

    Font(Bytes data, int64_t at_) : d(data), at(at_) {
        version = u16(0);
        size = u32(2);
        {
            Bytes s = d.slice(at + 6, at + 66);
            int64_t z = s.find_byte(0);
            copyright = latin1(z >= 0 ? s.slice(0, z) : s);
        }
        vector = (u16(0x42) & 1) != 0;
        points = u16(0x44);
        ascent = u16(0x4A);
        italic = d.at(at + 0x50) != 0;
        weight = u16(0x53);
        charset = d.at(at + 0x55);
        width = u16(0x56);
        height = u16(0x58);
        average_width = u16(0x5B);
        max_width = u16(0x5D);
        first = d.at(at + 0x5F);
        last = d.at(at + 0x60);
        def = d.at(at + 0x61);
        row_bytes = u16(0x63);
        int64_t face = u32(0x69);
        int64_t end = (face && at + face < d.size()) ? d.find_byte(0, at + face, at + face + 64) : -1;
        name = end > 0 ? latin1(d.slice(at + face, end)) : "";
        bits = u32(0x71);
        int64_t n = last - first + 1;
        if (vector || last < first) {
        } else if (version < 0x0200) {
            strike(n);
        } else {
            int64_t table = version < 0x0300 ? 0x76 : 0x94, step = version < 0x0300 ? 4 : 6;
            for (int64_t i = 0; i < n; ++i) {
                int64_t p = at + table + step * i;
                if (p + step > d.size()) break;
                int64_t w = d.u16(p);
                int64_t off = step == 4 ? d.u16(p + 2) : d.u32(p + 2);
                put(first + i, w, off);
            }
        }
    }

    std::vector<std::string> glyph(int64_t code) const {
        if (!find(code)) code = first + def;
        const auto *g = find(code);
        if (!g) return std::vector<std::string>(static_cast<size_t>(height), "");
        int64_t w = g->first, off = g->second;
        std::vector<std::string> rows;
        for (int64_t y = 0; y < height; ++y) {
            std::string line;
            for (int64_t x = 0; x < w; ++x) {
                int64_t p, bit;
                if (version < 0x0200) {
                    p = at + bits + y * row_bytes + ((off + x) >> 3);
                    bit = (off + x) & 7;
                } else {
                    p = at + off + (x >> 3) * height + y;
                    bit = x & 7;
                }
                line += (p < d.size() && ((d.at(p) >> (7 - bit)) & 1)) ? '#' : ' ';
            }
            rows.push_back(line);
        }
        return rows;
    }
};

Font parse(Bytes data, int64_t at) {
    if (at + 0x76 > data.size()) raise("ValueError", "no font at 0x" + hex(static_cast<uint64_t>(at)));
    uint16_t v = static_cast<uint16_t>(data.u16(at));
    if (v != 0x0100 && v != 0x0200 && v != 0x0300) raise("ValueError", "no font at 0x" + hex(static_cast<uint64_t>(at)));
    return Font(data, at);
}

void rows_out(Out &o, const std::vector<std::string> &rows) {
    o.begin_list();
    for (auto &r : rows) o.text(r);
    o.end_list();
}

// fonts.font(data, at) -> the Font's attributes
void font(Call &c) {
    Font f = parse(c.data, c.int_arg(0));
    Out &o = c.out;
    o.begin_dict();
    o.item("version", f.version);
    o.item("size", f.size);
    o.item_text("copyright", f.copyright);
    o.key("vector");
    o.boolean(f.vector);
    o.item("points", f.points);
    o.item("ascent", f.ascent);
    o.key("italic");
    o.boolean(f.italic);
    o.item("weight", f.weight);
    o.item("charset", f.charset);
    o.item("width", f.width);
    o.item("height", f.height);
    o.item("average_width", f.average_width);
    o.item("max_width", f.max_width);
    o.item("first", f.first);
    o.item("last", f.last);
    o.item("default", f.def);
    o.item("row_bytes", f.row_bytes);
    o.item_text("name", f.name);
    o.item("bits", f.bits);
    o.key("_where");
    o.begin_dict();
    for (auto &kv : f.where) {
        o.integer(kv.first);
        o.begin_tuple();
        o.integer(kv.second.first);
        o.integer(kv.second.second);
        o.end_tuple();
    }
    o.end_dict();
    o.key("widths");
    o.begin_dict();
    for (auto &kv : f.where) {
        o.integer(kv.first);
        o.integer(kv.second.first);
    }
    o.end_dict();
    o.end_dict();
}

// fonts.glyph(data, at, code), fonts.render(data, at, text)
void glyph(Call &c) { rows_out(c.out, parse(c.data, c.int_arg(0)).glyph(c.int_arg(1))); }
void render(Call &c) {
    Font f = parse(c.data, c.int_arg(0));
    std::vector<std::string> rows(static_cast<size_t>(f.height), "");
    for (uint32_t cp : codepoints(c.str_arg(1))) {
        auto g = f.glyph(cp);
        for (size_t y = 0; y < g.size(); ++y) rows[y] += g[y] + " ";
    }
    rows_out(c.out, rows);
}

}  // namespace

CBF_QUESTIONS(fonts) {
    add("fonts.font", font);
    add("fonts.glyph", glyph);
    add("fonts.render", render);
}

}  // namespace cbf::fonts
