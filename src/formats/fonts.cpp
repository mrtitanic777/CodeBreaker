#include "formats/fonts.hpp"

#include <algorithm>
#include <format>

#include "core/fs.hpp"
#include "core/io.hpp"
#include "core/log.hpp"
#include "core/text.hpp"
#include "formats/detect.hpp"
#include "formats/ne.hpp"

namespace cb::formats::fonts {

namespace {

constexpr std::size_t kFntHeaderSize = 0x76;   // every version's header is at least this long
constexpr std::size_t kFntNameLimit = 64;      // the face name is a NUL-terminated string of at most this many bytes

// The text of `bytes` up to its first NUL, as UTF-8 (the fields are Latin-1).
std::string fnt_string(ByteView bytes) {
    const auto end = std::find(bytes.begin(), bytes.end(), std::uint8_t{0});
    return text::from_latin1(bytes.first(static_cast<std::size_t>(end - bytes.begin())));
}

// The code points of a UTF-8 text; a byte that does not belong to a sequence is U+FFFD.
std::vector<char32_t> fnt_code_points(std::string_view utf8) {
    std::vector<char32_t> out;
    for (std::size_t i = 0; i < utf8.size();) {
        const auto lead = static_cast<unsigned char>(utf8[i]);
        const std::size_t length = lead < 0x80 ? 1 : (lead >> 5) == 6 ? 2 : (lead >> 4) == 14 ? 3 : (lead >> 3) == 30 ? 4 : 0;
        bool ok = length != 0 && i + length <= utf8.size();
        char32_t cp = length == 1 ? lead : static_cast<char32_t>(lead & (0xFF >> (length + 1)));
        for (std::size_t k = 1; ok && k < length; ++k) {
            const auto next = static_cast<unsigned char>(utf8[i + k]);
            ok = (next & 0xC0) == 0x80;
            cp = (cp << 6) | (next & 0x3F);
        }
        out.push_back(ok ? cp : U'�');
        i += ok ? length : 1;
    }
    return out;
}

}  // namespace

Font Font::parse(std::shared_ptr<const Bytes> data, std::size_t at) {
    const Bytes& d = *data;
    auto fail = [&] { return FormatError("fonts", at, "no font here"); };
    if (at > d.size() || d.size() - at < kFntHeaderSize) throw fail();
    io::ByteReader r(ByteView(d).subspan(at), "fonts.fnt");
    Font f;
    f.data_ = data;
    f.at_ = at;
    f.version = r.u16();
    if (f.version != 0x0100 && f.version != 0x0200 && f.version != 0x0300) throw fail();
    f.size = r.u32();
    f.copyright = fnt_string(r.bytes(60));   // 6..66
    r.seek(0x42);
    f.vector = (r.u16() & 1) != 0;
    f.points = r.u16();
    r.seek(0x4A);
    f.ascent = r.u16();
    r.seek(0x50);
    f.italic = r.u8() != 0;
    r.seek(0x53);
    f.weight = r.u16();
    f.charset = r.u8();
    f.width = r.u16();
    f.height = r.u16();
    r.seek(0x5B);
    f.average_width = r.u16();
    r.seek(0x5D);
    f.max_width = r.u16();
    f.first = r.u8();
    f.last = r.u8();
    f.default_char = r.u8();
    r.seek(0x63);
    f.row_bytes = r.u16();
    r.seek(0x69);
    const std::uint32_t face = r.u32();
    r.seek(0x71);
    f.bits = r.u32();

    if (face != 0 && at + face < d.size()) {
        const ByteView window = ByteView(d).subspan(at + face, std::min<std::size_t>(kFntNameLimit, d.size() - at - face));
        const auto end = std::find(window.begin(), window.end(), std::uint8_t{0});
        if (end != window.end() && end != window.begin()) f.name = text::from_latin1(window.first(static_cast<std::size_t>(end - window.begin())));
    }

    const int count = f.last - f.first + 1;
    if (f.vector || f.last < f.first) return f;
    if (f.version < 0x0200) {
        f.read_strike(count);
        return f;
    }
    const std::size_t table = f.version < 0x0300 ? 0x76 : 0x94;
    const std::size_t step = f.version < 0x0300 ? 4 : 6;
    for (int i = 0; i < count; ++i) {
        const std::size_t p = at + table + step * static_cast<std::size_t>(i);
        if (p + step > d.size()) break;
        io::ByteReader entry(ByteView(d).subspan(p, step));
        const int w = entry.u16();
        f.places[f.first + i] = Place{w, step == 4 ? entry.u16() : entry.u32()};
    }
    return f;
}

// A 1.x font's characters are columns of one bitmap: fixed-pitch ones at multiples of the width, proportional ones where the table
// of column offsets says (one of three table positions in use; the one whose offsets run upward and fit the bitmap is the table).
void Font::read_strike(int count) {
    const Bytes& d = *data_;
    std::vector<std::uint32_t> offsets;
    if (width != 0) {
        for (int i = 0; i <= count; ++i) offsets.push_back(static_cast<std::uint32_t>(i * width));
    } else {
        for (const std::size_t table : {0x75, 0x76, 0x74}) {
            if (at_ + table + 2 * static_cast<std::size_t>(count + 1) > d.size()) continue;
            io::ByteReader r(ByteView(d).subspan(at_ + table));
            std::vector<std::uint32_t> got;
            for (int i = 0; i <= count; ++i) got.push_back(r.u16());
            if (std::is_sorted(got.begin(), got.end()) && got.back() <= static_cast<std::uint32_t>(row_bytes) * 8) {
                offsets = std::move(got);
                break;
            }
        }
    }
    for (std::size_t i = 0; i + 1 < offsets.size(); ++i)
        places[first + static_cast<int>(i)] = Place{static_cast<int>(offsets[i + 1] - offsets[i]), offsets[i]};
}

std::vector<std::string> Font::glyph(char32_t code) const {
    auto found = places.find(static_cast<int>(code));
    if (found == places.end()) found = places.find(first + default_char);
    if (found == places.end()) return std::vector<std::string>(static_cast<std::size_t>(height));
    const Bytes& d = *data_;
    const Place& place = found->second;
    std::vector<std::string> rows;
    for (int y = 0; y < height; ++y) {
        std::string line;
        for (int x = 0; x < place.width; ++x) {
            std::size_t p;
            unsigned bit;
            if (version < 0x0200) {
                const std::size_t column = place.offset + static_cast<std::size_t>(x);
                p = at_ + bits + static_cast<std::size_t>(y) * row_bytes + (column >> 3);
                bit = column & 7;
            } else {
                p = at_ + place.offset + static_cast<std::size_t>(x >> 3) * height + y;
                bit = static_cast<unsigned>(x) & 7;
            }
            line += (p < d.size() && ((d[p] >> (7 - bit)) & 1) != 0) ? '#' : ' ';
        }
        rows.push_back(std::move(line));
    }
    return rows;
}

std::vector<std::string> Font::render(std::string_view utf8) const {
    std::vector<std::string> rows(static_cast<std::size_t>(height));
    for (const char32_t code : fnt_code_points(utf8)) {
        const std::vector<std::string> g = glyph(code);
        for (std::size_t y = 0; y < g.size(); ++y) rows[y] += g[y] + ' ';
    }
    return rows;
}

std::string Font::describe() const {
    const char* kind_name = vector ? "vector" : width != 0 ? "fixed" : "proportional";
    return std::format("{:<14} {:>2}pt {:>2}px  {:<12} chars {}..{}  version {:x}.{:02x}", name.empty() ? "?" : name, points, height, kind_name,
                       first, last, version >> 8, version & 0xFF);
}

std::vector<Font> fonts_in(const std::filesystem::path& file) {
    std::vector<Font> out;
    if (detect::format_of(file) == "NE") {
        const ne::Module module = ne::Module::open_file(file);
        const auto data = std::make_shared<const Bytes>(module.data().begin(), module.data().end());
        for (const ne::Resource& resource : module.resources()) {
            if (resource.type != "FONT") continue;
            try {
                out.push_back(Font::parse(data, resource.file_offset));
            } catch (const FormatError&) {
                // a FONT resource that is not a font is skipped
            }
        }
        return out;
    }
    try {
        out.push_back(Font::parse(fs::read_file(file)));
    } catch (const FormatError&) {
    }
    return out;
}

cli::Command command(const Environment& environment) {
    cli::Command c;
    c.name = "fonts";
    c.summary = "list and draw the raster fonts in a .FON/.FNT";
    c.area = "Formats";
    c.params = {
        cli::Positional{"file", "a .FON or .FNT file (or a module name of the selected corpus)"},
        cli::Option{"--index", "", "the font to draw with (default 0)", cli::Arg::Value, "N"},
        cli::Option{"--show", "", "draw this text in a font of the file", cli::Arg::Value, "TEXT"},
    };
    c.handler = [environment](const cli::Args& a) {
        const std::string requested = a.required("file");
        std::optional<std::filesystem::path> path;
        if (std::filesystem::is_regular_file(fs::from_text(requested))) path = fs::from_text(requested);
        else if (environment.retail) path = environment.retail(requested);
        if (!path || path->empty()) {
            err("no file '{}'\n", requested);
            return kExitFailed;
        }
        const std::vector<Font> found = fonts_in(*path);
        out("{}: {} font(s)\n", fs::to_text(path->filename()), found.size());
        for (std::size_t i = 0; i < found.size(); ++i) out("  [{}] {}\n", i, found[i].describe());
        const std::optional<std::string> show = a.value("show");
        if (show && !found.empty()) {
            const std::size_t index = a.integer<std::size_t>("index").value_or(0);
            if (index >= found.size()) throw UsageError(std::format("--index {} is out of range (the file has {} fonts)", index, found.size()));
            const Font& font = found[index];
            if (font.vector) {
                out("  a vector font has no bitmaps to draw\n");
                return kExitOk;
            }
            for (const std::string& row : font.render(*show)) out("  {}\n", row);
        }
        return kExitOk;
    };
    return c;
}

}  // namespace cb::formats::fonts
