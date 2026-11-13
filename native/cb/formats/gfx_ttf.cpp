// A DefineFont3 font as a TrueType file (gfx.hpp's font_to_ttf): the SWF outlines at 2048 units to the em, the tables head, hhea, maxp, OS/2,
// hmtx, cmap, loca, glyf, name and post written the way a font builder lays out a font with this little in it.
#include <algorithm>
#include <chrono>
#include <cmath>
#include <map>
#include <set>

#include "cb/core/io.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/gfx.hpp"

namespace cb::formats::gfx {

namespace {

constexpr int kSwfEm = 20480;               // the SWF glyph coordinate system
constexpr int kUnitsPerEm = 2048;
constexpr std::uint32_t kMagic = 0x5F0F3CF5;
constexpr std::int64_t kSecondsFrom1904To1970 = 2082844800;
constexpr unsigned kDefineFont3Code = 75;

struct Range {
    unsigned bit;
    std::uint32_t first, last;
};
constexpr Range kUnicodeRanges[] = {
#include "cb/formats/gfx_os2_ranges.inc"
};

// A SWF coordinate in font units, rounded half to even.
int to_font_units(std::int64_t v) {
    const std::int64_t whole = v / (kSwfEm / kUnitsPerEm), rest = v % (kSwfEm / kUnitsPerEm);
    constexpr std::int64_t half = kSwfEm / kUnitsPerEm / 2;
    if (rest > half || (rest == half && (whole & 1) != 0)) return static_cast<int>(whole + 1);
    if (rest < -half || (rest == -half && (whole & 1) != 0)) return static_cast<int>(whole - 1);
    return static_cast<int>(whole);
}

struct Point {
    int x = 0, y = 0;
    bool on_curve = true;
};

// What a glyph outline is as TrueType stores it: points and the index of each contour's last point.
struct Outline {
    std::vector<Point> points;
    std::vector<std::uint16_t> contour_ends;
    bool empty() const { return contour_ends.empty(); }
};

// The SWF contours as TrueType ones: y runs the other way, a quadratic curve is an off-curve and an on-curve point, a contour that returns to
// its first point does not repeat it, and a contour of one point is dropped.
Outline outline_of(const std::vector<Contour>& contours) {
    Outline outline;
    for (const Contour& contour : contours) {
        const std::size_t first = outline.points.size();
        for (const PathOp& op : contour) {
            if (op.kind == PathOp::Kind::Curve) outline.points.push_back({to_font_units(op.control_x), -to_font_units(op.control_y), false});
            outline.points.push_back({to_font_units(op.x), -to_font_units(op.y), true});
        }
        if (outline.points.size() == first) continue;
        std::size_t last = outline.points.size() - 1;
        if (last == first) {   // an anchor, not an outline
            outline.points.pop_back();
            continue;
        }
        if (outline.points[first].x == outline.points[last].x && outline.points[first].y == outline.points[last].y) {
            outline.points.pop_back();
            --last;
        }
        outline.contour_ends.push_back(static_cast<std::uint16_t>(last));
    }
    return outline;
}

struct Bounds {
    int x_min = 0, y_min = 0, x_max = 0, y_max = 0;
};

Bounds bounds_of(const Outline& outline) {
    Bounds b{outline.points[0].x, outline.points[0].y, outline.points[0].x, outline.points[0].y};
    for (const Point& p : outline.points) {
        b.x_min = std::min(b.x_min, p.x);
        b.y_min = std::min(b.y_min, p.y);
        b.x_max = std::max(b.x_max, p.x);
        b.y_max = std::max(b.y_max, p.y);
    }
    return b;
}

// One glyph's data: the header, the contour ends, no instructions, then the flags and the coordinate deltas packed greedily.
Bytes compile_glyph(const Outline& outline) {
    constexpr unsigned kOnCurve = 1, kXShort = 2, kYShort = 4, kRepeat = 8, kXSame = 0x10, kYSame = 0x20;
    io::ByteWriter w;
    const Bounds b = bounds_of(outline);
    w.u16_be(static_cast<std::uint16_t>(outline.contour_ends.size()));
    for (const int v : {b.x_min, b.y_min, b.x_max, b.y_max}) w.u16_be(static_cast<std::uint16_t>(v));
    for (const std::uint16_t end : outline.contour_ends) w.u16_be(end);
    w.u16_be(0);   // no instructions
    Bytes flags, xs, ys;
    int px = 0, py = 0;
    int last_flag = -1;
    unsigned repeat = 0;
    for (const Point& p : outline.points) {
        unsigned flag = p.on_curve ? kOnCurve : 0;
        const int dx = p.x - px, dy = p.y - py;
        px = p.x;
        py = p.y;
        if (dx == 0) {
            flag |= kXSame;
        } else if (dx >= -255 && dx <= 255) {
            flag |= kXShort | (dx > 0 ? kXSame : 0u);
            xs.push_back(static_cast<std::uint8_t>(std::abs(dx)));
        } else {
            xs.push_back(static_cast<std::uint8_t>(dx >> 8));
            xs.push_back(static_cast<std::uint8_t>(dx));
        }
        if (dy == 0) {
            flag |= kYSame;
        } else if (dy >= -255 && dy <= 255) {
            flag |= kYShort | (dy > 0 ? kYSame : 0u);
            ys.push_back(static_cast<std::uint8_t>(std::abs(dy)));
        } else {
            ys.push_back(static_cast<std::uint8_t>(dy >> 8));
            ys.push_back(static_cast<std::uint8_t>(dy));
        }
        if (static_cast<int>(flag) == last_flag && repeat != 255) {
            ++repeat;
            if (repeat == 1) {
                flags.push_back(static_cast<std::uint8_t>(flag));
            } else {
                flags[flags.size() - 2] = static_cast<std::uint8_t>(flag | kRepeat);
                flags[flags.size() - 1] = static_cast<std::uint8_t>(repeat);
            }
        } else {
            repeat = 0;
            flags.push_back(static_cast<std::uint8_t>(flag));
        }
        last_flag = static_cast<int>(flag);
    }
    w.bytes(flags);
    w.bytes(xs);
    w.bytes(ys);
    return w.take();
}

// ---- cmap format 4 ----

// Splits a run of consecutive character codes into the segments that store it in the fewest bytes: runs of consecutive glyph ids are worth a
// segment of their own when long enough. Returns the start codes after the first and the end codes (the run's own first start is known).
void split_range(std::uint32_t start_code, std::uint32_t end_code, const std::map<std::uint32_t, std::uint32_t>& cmap, std::vector<std::uint32_t>& starts,
                 std::vector<std::uint32_t>& ends) {
    if (start_code == end_code) {
        ends.push_back(end_code);
        return;
    }
    std::uint32_t last_id = cmap.at(start_code), last_code = start_code;
    bool in_order = false;
    std::uint32_t ordered_begin = 0;
    std::vector<std::pair<std::uint32_t, std::uint32_t>> sub;
    for (std::uint32_t code = start_code + 1; code <= end_code; ++code) {
        const std::uint32_t id = cmap.at(code);
        if (id == last_id + 1) {
            if (!in_order) {
                in_order = true;
                ordered_begin = last_code;
            }
        } else if (in_order) {
            in_order = false;
            sub.push_back({ordered_begin, last_code});
        }
        last_id = id;
        last_code = code;
    }
    if (in_order) sub.push_back({ordered_begin, last_code});

    // a new segment costs 8 bytes, not using one costs 2 bytes a character
    std::vector<std::pair<std::uint32_t, std::uint32_t>> kept;
    for (const auto& [b, e] : sub) {
        if (b == start_code && e == end_code) break;   // the whole run: fine as it is
        const std::uint32_t threshold = (b == start_code || e == end_code) ? 4 : 8;
        if (e - b + 1 > threshold) kept.push_back({b, e});
    }
    if (kept.empty()) {
        ends.push_back(end_code);
        return;
    }
    if (kept.front().first != start_code) kept.insert(kept.begin(), {start_code, kept.front().first - 1});
    if (kept.back().second != end_code) kept.push_back({kept.back().second + 1, end_code});
    for (std::size_t i = 1; i < kept.size(); ++i) {   // fill the holes: the stretches where the glyph ids are not consecutive
        if (kept[i - 1].second + 1 != kept[i].first) {
            kept.insert(kept.begin() + static_cast<std::ptrdiff_t>(i), {kept[i - 1].second + 1, kept[i].first - 1});
            ++i;
        }
    }
    for (std::size_t i = 0; i < kept.size(); ++i) {
        if (i != 0) starts.push_back(kept[i].first);
        ends.push_back(kept[i].second);
    }
}

Bytes cmap_table(const std::map<std::uint32_t, std::uint32_t>& cmap) {
    std::vector<std::uint32_t> starts, ends;
    if (cmap.empty()) {
        starts = {0xFFFF};
        ends = {0xFFFF};
    } else {
        std::uint32_t last = cmap.begin()->first;
        starts.push_back(last);
        for (auto it = std::next(cmap.begin()); it != cmap.end(); ++it) {
            if (it->first == last + 1) {
                last = it->first;
                continue;
            }
            const std::uint32_t run_start = starts.back();
            split_range(run_start, last, cmap, starts, ends);
            starts.push_back(it->first);
            last = it->first;
        }
        split_range(starts.back(), last, cmap, starts, ends);
        starts.push_back(0xFFFF);
        ends.push_back(0xFFFF);
    }
    std::vector<std::int16_t> deltas;
    std::vector<std::uint16_t> range_offsets, glyph_array;
    const std::size_t segments = ends.size();
    for (std::size_t i = 0; i + 1 < segments; ++i) {
        std::vector<std::uint32_t> ids;
        for (std::uint32_t code = starts[i]; code <= ends[i]; ++code) ids.push_back(cmap.at(code));
        bool consecutive = true;
        for (std::size_t k = 0; k < ids.size(); ++k) consecutive = consecutive && ids[k] == ids[0] + k;
        if (consecutive) {
            deltas.push_back(static_cast<std::int16_t>(static_cast<std::uint16_t>(ids[0] - starts[i])));
            range_offsets.push_back(0);
        } else {
            deltas.push_back(0);
            range_offsets.push_back(static_cast<std::uint16_t>(2 * (segments + glyph_array.size() - i)));
            for (const std::uint32_t id : ids) glyph_array.push_back(static_cast<std::uint16_t>(id));
        }
    }
    deltas.push_back(1);   // 0xFFFF + 1 wraps to 0: the closing segment maps to .notdef
    range_offsets.push_back(0);

    io::ByteWriter sub;
    unsigned entry_selector = 0;
    while ((2u << entry_selector) <= segments) ++entry_selector;
    const unsigned search_range = 2u << entry_selector;
    sub.u16_be(4);
    sub.u16_be(static_cast<std::uint16_t>(16 + 8 * segments + 2 * glyph_array.size()));
    sub.u16_be(0);
    sub.u16_be(static_cast<std::uint16_t>(2 * segments));
    sub.u16_be(static_cast<std::uint16_t>(search_range));
    sub.u16_be(static_cast<std::uint16_t>(entry_selector));
    sub.u16_be(static_cast<std::uint16_t>(2 * segments - search_range));
    for (const std::uint32_t e : ends) sub.u16_be(static_cast<std::uint16_t>(e));
    sub.u16_be(0);
    for (const std::uint32_t s : starts) sub.u16_be(static_cast<std::uint16_t>(s));
    for (const std::int16_t d : deltas) sub.u16_be(static_cast<std::uint16_t>(d));
    for (const std::uint16_t o : range_offsets) sub.u16_be(o);
    for (const std::uint16_t g : glyph_array) sub.u16_be(g);

    io::ByteWriter w;   // the one subtable for both the Unicode (0, 3) and the Windows (3, 1) encoding
    w.u16_be(0);
    w.u16_be(2);
    for (const auto [platform, encoding] : {std::pair<unsigned, unsigned>{0, 3}, {3, 1}}) {
        w.u16_be(static_cast<std::uint16_t>(platform));
        w.u16_be(static_cast<std::uint16_t>(encoding));
        w.u32_be(4 + 8 * 2);
    }
    w.bytes(sub.data());
    return w.take();
}

// ---- name ----

// ASCII as the Mac Roman and the UTF-16 big-endian forms of a name; a character without a plain Mac Roman form is '?'.
Bytes mac_string(std::string_view utf8) {
    Bytes out;
    for (const char c : utf8) out.push_back(static_cast<unsigned char>(c) < 0x80 ? static_cast<std::uint8_t>(c) : std::uint8_t{'?'});
    return out;
}

Bytes windows_string(std::string_view utf8) {
    Bytes little = text::to_utf16le(utf8);
    for (std::size_t i = 0; i + 1 < little.size(); i += 2) std::swap(little[i], little[i + 1]);
    return little;
}

// The family name, the style "Regular" and the PostScript name, for the Mac (1, 0, 0) and the Windows (3, 1, 0x409) platforms; a text the
// pool already holds is not stored again.
Bytes name_table(const std::string& family) {
    struct Record {
        unsigned platform, encoding, language, id;
        Bytes text;
    };
    std::vector<Record> records;
    const std::pair<unsigned, std::string> names[] = {{1, family}, {2, "Regular"}, {6, family}};
    for (const auto& [id, value] : names) records.push_back({1, 0, 0, id, mac_string(value)});
    for (const auto& [id, value] : names) records.push_back({3, 1, 0x409, id, windows_string(value)});
    Bytes pool;
    std::map<Bytes, std::size_t> offsets;
    io::ByteWriter w;
    w.u16_be(0);
    w.u16_be(static_cast<std::uint16_t>(records.size()));
    w.u16_be(static_cast<std::uint16_t>(6 + 12 * records.size()));
    for (const Record& r : records) {
        auto found = offsets.find(r.text);
        if (found == offsets.end()) {
            found = offsets.emplace(r.text, pool.size()).first;
            append(pool, r.text);
        }
        w.u16_be(static_cast<std::uint16_t>(r.platform));
        w.u16_be(static_cast<std::uint16_t>(r.encoding));
        w.u16_be(static_cast<std::uint16_t>(r.language));
        w.u16_be(static_cast<std::uint16_t>(r.id));
        w.u16_be(static_cast<std::uint16_t>(r.text.size()));
        w.u16_be(static_cast<std::uint16_t>(found->second));
    }
    w.bytes(pool);
    return w.take();
}

std::uint32_t table_checksum(ByteView table) {
    std::uint32_t sum = 0;
    for (std::size_t i = 0; i < table.size(); i += 4) {
        std::uint32_t word = 0;
        for (std::size_t k = 0; k < 4; ++k) word = (word << 8) | (i + k < table.size() ? table[i + k] : 0u);
        sum += word;
    }
    return sum;
}

// Python's round of a quotient: halves go to the even neighbour.
int round_half_even(double v) { return static_cast<int>(std::nearbyint(v)); }

}  // namespace

TrueType font_to_ttf(ByteView data, unsigned font_id, const std::optional<std::string>& family) {
    std::optional<Bytes> found;
    for (const Movie& movie : find_movies(data)) {
        for (Tag& tag : parse_tags(movie.body)) {
            if (tag.code == kDefineFont3Code && tag.body.size() >= 2 && (tag.body[0] | (tag.body[1] << 8)) == static_cast<int>(font_id)) {
                found = std::move(tag.body);
                break;
            }
        }
        if (found) break;
    }
    if (!found) throw FormatError("gfx", 0, std::format("DefineFont3 id={} not found", font_id));
    const Bytes& font = *found;

    io::ByteReader r(font, "DefineFont3");
    r.skip(2);
    const unsigned flags = r.u8();
    r.skip(1);
    const unsigned name_length = r.u8();
    const std::string name = [&] {
        const ByteView raw = r.bytes(name_length);
        const auto end = std::find(raw.begin(), raw.end(), std::uint8_t{0});
        return text::from_latin1(raw.first(static_cast<std::size_t>(end - raw.begin())));
    }();
    const std::size_t count = r.u16();
    const std::size_t offsets_at = r.pos();
    const bool wide_offsets = (flags & 0x08) != 0;
    const auto offset_of = [&](std::size_t index) {
        io::ByteReader at(font, "DefineFont3 offsets");
        at.seek(offsets_at + index * (wide_offsets ? 4 : 2));
        return std::size_t{wide_offsets ? at.u32() : at.u16()};
    };
    std::vector<std::vector<Contour>> shapes;
    for (std::size_t g = 0; g < count; ++g) shapes.push_back(glyph_contours(font, offsets_at + offset_of(g)));
    r.seek(offsets_at + offset_of(count));
    std::vector<std::uint32_t> codes;
    for (std::size_t g = 0; g < count; ++g) codes.push_back(r.u16());
    const int ascent = r.i16();
    const int descent = r.i16();
    r.skip(2);   // the leading
    std::vector<int> advances;
    for (std::size_t g = 0; g < count; ++g) advances.push_back(r.i16());

    // the glyphs: .notdef (empty) and g0 ... gN
    std::vector<Outline> outlines(count + 1);
    std::vector<int> advance(count + 1);
    advance[0] = count != 0 ? std::max(0, to_font_units(advances[0])) : 600;
    for (std::size_t g = 0; g < count; ++g) {
        outlines[g + 1] = outline_of(shapes[g]);
        advance[g + 1] = std::max(0, to_font_units(advances[g]));
    }
    std::map<std::uint32_t, std::uint32_t> cmap;
    for (std::size_t g = 0; g < count; ++g)
        if (codes[g] != 0) cmap.emplace(codes[g], static_cast<std::uint32_t>(g + 1));

    // glyf and loca
    std::vector<Bytes> glyph_data;
    for (const Outline& outline : outlines) glyph_data.push_back(outline.empty() ? Bytes{} : compile_glyph(outline));
    std::size_t total = 0, odd = 0;
    for (const Bytes& g : glyph_data) {
        total += g.size();
        odd += g.size() % 2;
    }
    if (total < 0x20000 && odd != 0 && total + odd < 0x20000)   // pad the odd-sized glyphs so that loca can use short offsets
        for (Bytes& g : glyph_data)
            if (g.size() % 2 == 1) g.push_back(0);
    std::vector<std::uint32_t> locations;
    io::ByteWriter glyf;
    for (const Bytes& g : glyph_data) {
        locations.push_back(static_cast<std::uint32_t>(glyf.size()));
        glyf.bytes(g);
    }
    locations.push_back(static_cast<std::uint32_t>(glyf.size()));
    const bool short_loca = locations.back() < 0x20000 && std::all_of(locations.begin(), locations.end(), [](std::uint32_t l) { return l % 2 == 0; });
    io::ByteWriter loca;
    for (const std::uint32_t l : locations) {
        if (short_loca) loca.u16_be(static_cast<std::uint16_t>(l / 2));
        else loca.u32_be(l);
    }

    // what the outlines say of the font as a whole
    Bounds all;
    bool any = false;
    unsigned max_points = 0, max_contours = 0;
    int min_rsb = 0, max_extent = 0;
    bool first_extent = true;
    for (std::size_t g = 0; g < outlines.size(); ++g) {
        if (outlines[g].empty()) continue;
        const Bounds b = bounds_of(outlines[g]);
        if (!any) all = b;
        any = true;
        all = {std::min(all.x_min, b.x_min), std::min(all.y_min, b.y_min), std::max(all.x_max, b.x_max), std::max(all.y_max, b.y_max)};
        max_points = std::max<unsigned>(max_points, static_cast<unsigned>(outlines[g].points.size()));
        max_contours = std::max<unsigned>(max_contours, static_cast<unsigned>(outlines[g].contour_ends.size()));
        const int width = b.x_max - b.x_min, rsb = advance[g] - width;   // (the left side bearings are all 0)
        min_rsb = first_extent ? rsb : std::min(min_rsb, rsb);
        max_extent = first_extent ? width : std::max(max_extent, width);
        first_extent = false;
    }
    const int advance_max = *std::max_element(advance.begin(), advance.end());
    const int asc = to_font_units(ascent), dsc = -std::abs(to_font_units(descent));

    // hmtx: the trailing glyphs of one width share the last full record
    std::size_t metrics = advance.size();
    while (metrics > 1 && advance[metrics - 1] == advance[metrics - 2]) --metrics;
    io::ByteWriter hmtx;
    for (std::size_t g = 0; g < metrics; ++g) {
        hmtx.u16_be(static_cast<std::uint16_t>(advance[g]));
        hmtx.u16_be(0);
    }
    for (std::size_t g = metrics; g < advance.size(); ++g) hmtx.u16_be(0);

    io::ByteWriter hhea;
    hhea.u32_be(0x00010000);
    hhea.u16_be(static_cast<std::uint16_t>(asc));
    hhea.u16_be(static_cast<std::uint16_t>(dsc));
    hhea.u16_be(0);   // line gap
    hhea.u16_be(static_cast<std::uint16_t>(advance_max));
    hhea.u16_be(0);   // min left side bearing
    hhea.u16_be(static_cast<std::uint16_t>(min_rsb));
    hhea.u16_be(static_cast<std::uint16_t>(max_extent));
    hhea.u16_be(1);   // caret slope rise
    hhea.zeros(2 + 2 + 8 + 2);
    hhea.u16_be(static_cast<std::uint16_t>(metrics));

    io::ByteWriter maxp;
    maxp.u32_be(0x00010000);
    maxp.u16_be(static_cast<std::uint16_t>(outlines.size()));
    maxp.u16_be(static_cast<std::uint16_t>(max_points));
    maxp.u16_be(static_cast<std::uint16_t>(max_contours));
    maxp.u16_be(0);
    maxp.u16_be(0);
    maxp.u16_be(2);   // zones
    maxp.zeros(16);

    // OS/2 (version 3): the average width of the glyphs that have one, the range of the cmap's characters and the Unicode blocks they are in
    double width_sum = 0;
    int widths = 0;
    for (const int a : advance) {
        if (a > 0) {
            width_sum += a;
            ++widths;
        }
    }
    std::uint32_t unicode_ranges[4] = {};
    for (const auto& [code, id] : cmap)
        for (const Range& range : kUnicodeRanges)
            if (code >= range.first && code <= range.last) unicode_ranges[range.bit / 32] |= 1u << (range.bit % 32);
    io::ByteWriter os2;
    os2.u16_be(3);
    os2.u16_be(static_cast<std::uint16_t>(widths != 0 ? round_half_even(width_sum / widths) : 0));
    os2.u16_be(400);   // weight
    os2.u16_be(5);     // width class
    os2.u16_be(4);     // fsType
    os2.zeros(20);     // subscript, superscript and strikeout
    os2.u16_be(0);     // family class
    os2.zeros(10);     // panose
    for (const std::uint32_t range : unicode_ranges) os2.u32_be(range);
    os2.bytes(as_bytes("????"));
    os2.u16_be(0);     // fsSelection
    os2.u16_be(cmap.empty() ? 0 : static_cast<std::uint16_t>(cmap.begin()->first));
    os2.u16_be(cmap.empty() ? 0 : static_cast<std::uint16_t>(std::min<std::uint32_t>(cmap.rbegin()->first, 0xFFFF)));
    os2.u16_be(static_cast<std::uint16_t>(asc));
    os2.u16_be(static_cast<std::uint16_t>(dsc));
    os2.u16_be(0);     // typographic line gap
    os2.u16_be(static_cast<std::uint16_t>(asc));
    os2.u16_be(static_cast<std::uint16_t>(-dsc));
    os2.zeros(8);      // code page ranges
    os2.zeros(4);      // x height, cap height
    os2.u16_be(0);     // default character
    os2.u16_be(32);    // break character
    os2.u16_be(0);     // max context

    // post, format 2: every glyph but .notdef by a name of its own
    io::ByteWriter post;
    post.u32_be(0x00020000);
    post.zeros(28);
    post.u16_be(static_cast<std::uint16_t>(outlines.size()));
    post.u16_be(0);   // .notdef is the standard glyph 0
    for (std::size_t g = 1; g < outlines.size(); ++g) post.u16_be(static_cast<std::uint16_t>(257 + g));
    for (std::size_t g = 0; g < count; ++g) {
        const std::string glyph_name = "g" + std::to_string(g);
        post.u8(static_cast<std::uint8_t>(glyph_name.size()));
        post.bytes(as_bytes(glyph_name));
    }

    const std::string family_name = family ? *family : text::replace_all(name, "-", "");
    const auto now = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    io::ByteWriter head;
    head.u32_be(0x00010000);
    head.u32_be(0x00010000);   // font revision 1.0
    head.u32_be(0);            // checksum adjustment, below
    head.u32_be(kMagic);
    head.u16_be(1);
    head.u16_be(kUnitsPerEm);
    head.u64_be(static_cast<std::uint64_t>(now + kSecondsFrom1904To1970));
    head.u64_be(static_cast<std::uint64_t>(now + kSecondsFrom1904To1970));
    for (const int v : {any ? all.x_min : 0, any ? all.y_min : 0, any ? all.x_max : 0, any ? all.y_max : 0}) head.u16_be(static_cast<std::uint16_t>(v));
    head.u16_be(0);   // mac style
    head.u16_be(3);   // lowest recommended size in pixels
    head.u16_be(2);   // direction hint
    head.u16_be(short_loca ? 0 : 1);
    head.u16_be(0);   // glyph data format

    struct Table {
        std::string tag;
        Bytes data;
    };
    std::vector<Table> tables = {{"head", head.take()}, {"hhea", hhea.take()}, {"maxp", maxp.take()}, {"OS/2", os2.take()}, {"hmtx", hmtx.take()},
                                 {"cmap", cmap_table(cmap)}, {"loca", loca.take()}, {"glyf", glyf.take()}, {"name", name_table(family_name)}, {"post", post.take()}};
    // the directory lists the tables by tag; their data follows in the order above, each padded to four bytes
    std::vector<std::size_t> by_tag(tables.size());
    for (std::size_t i = 0; i < by_tag.size(); ++i) by_tag[i] = i;
    std::sort(by_tag.begin(), by_tag.end(), [&](std::size_t a, std::size_t b) { return tables[a].tag < tables[b].tag; });
    std::vector<std::uint32_t> offsets(tables.size());
    std::size_t at = 12 + 16 * tables.size();
    for (std::size_t i = 0; i < tables.size(); ++i) {
        offsets[i] = static_cast<std::uint32_t>(at);
        at += (tables[i].data.size() + 3) / 4 * 4;
    }
    unsigned selector = 0;
    while ((2u << selector) <= tables.size()) ++selector;
    io::ByteWriter file;
    file.u32_be(0x00010000);
    file.u16_be(static_cast<std::uint16_t>(tables.size()));
    file.u16_be(static_cast<std::uint16_t>(16u << selector));
    file.u16_be(static_cast<std::uint16_t>(selector));
    file.u16_be(static_cast<std::uint16_t>(16 * tables.size() - (16u << selector)));
    for (const std::size_t i : by_tag) {
        file.bytes(as_bytes(tables[i].tag));
        file.u32_be(table_checksum(tables[i].data));
        file.u32_be(offsets[i]);
        file.u32_be(static_cast<std::uint32_t>(tables[i].data.size()));
    }
    for (const Table& table : tables) {
        file.bytes(table.data);
        file.align(4);
    }
    Bytes bytes = file.take();
    // the checksum adjustment makes the whole file's words sum to 0xB1B0AFBA
    const std::uint32_t adjustment = 0xB1B0AFBAu - table_checksum(bytes);
    for (int k = 0; k < 4; ++k) bytes[offsets[0] + 8 + k] = static_cast<std::uint8_t>(adjustment >> (24 - 8 * k));
    return TrueType{name, count, std::move(bytes)};
}

}  // namespace cb::formats::gfx
