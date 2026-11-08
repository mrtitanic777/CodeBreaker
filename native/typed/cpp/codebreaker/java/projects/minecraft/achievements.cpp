// codebreaker/java/projects/minecraft/achievements.cpp -- typed C++ port of
// codebreaker.java.projects.minecraft.achievements (see achievements.hpp for the notes on the port).
//
// The small functions the Python front end produced (normalize_title_id, _humanize, _read,
// _block_class_textures, _distribute_score, _const_name, _xml, emit_code, _guess_content_dir) are kept as they
// were; the rest is ported by hand.

#include "codebreaker/java/projects/minecraft/achievements.hpp"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <map>
#include <string>
#include <vector>

#include "json.hpp"
#include "zlib.h"    // native/decompiler/core/zlib (inflate + deflate of the PNG's IDAT)
#include "codebreaker/java/console/assets.hpp"
#include "codebreaker/java/textutil.hpp"

namespace codebreaker::java::projects::minecraft::achievements {

namespace assets = ::codebreaker::java::console::assets;
namespace textutil = ::codebreaker::java::textutil;

// --- title id --------------------------------------------------------------

// Accept 'FFFE1234', '0xFFFE1234', 'fffe1234' -> canonical 8-hex-digit UPPER string, or None.
std::optional<py::str> normalize_title_id(const std::optional<py::str>& tid) {
    if (!(tid.has_value() && !tid->empty())) {
        return std::nullopt;
    }
    py::str s = tid->strip().lower();
    if (s.startswith(py::str("0x"))) {
        s = s.slice_from(2);
    }
    if (py::re::fullmatch(py::str("[0-9a-f]{1,8}"), s).has_value()) {
        return py::percent("%08X", py::int_from_str(s, 16));
    }
    return std::nullopt;
}

// A stable, self-issued title id derived from `seed` (e.g. the game name). NOT guaranteed unique --
// the user must check it against a title-id list. Returns an 8-hex-digit UPPER string.
py::str suggest_title_id(const std::optional<py::str>& seed) {
    int64_t num = py::crc32((seed.has_value() && !seed->empty() ? *seed : py::str("title")).encode("utf-8")) & 0xFFFF;
    return py::percent("%04X%04X", _HOMEBREW_PUBLISHER, num);
}

const py::re::Pattern& NEW_ACH_() {
    static const py::re::Pattern p = py::re::compile(py::str(R"rx(new\s+Achievement\s*\(\s*(\d+)\s*,\s*"([^"]+)")rx"));
    return p;
}
const py::re::Pattern& SPECIAL_() {
    static const py::re::Pattern p = py::re::compile(py::str(R"rx(setSpecial\s*\(\s*\))rx"));
    return p;
}
const py::re::Pattern& PARENT_() {
    static const py::re::Pattern p =
        py::re::compile(py::str(R"rx("[^"]+",[^,]+,[^,]+,[^,]+,\s*([A-Za-z_]\w*)\s*\))rx"));
    return p;
}
const py::re::Pattern& ICON_() {
    static const py::re::Pattern p =
        py::re::compile(py::str(R"rx("[^"]+"\s*,[^,]+,[^,]+,\s*((?:Item|Block)::s_\w+))rx"));
    return p;
}

// camelCase / snake key -> a readable title. openInventory -> Open Inventory,
// onARail -> On A Rail.
py::str _humanize(const py::str& key) {
    py::str s = key.replace(py::str("_"), py::str(" "));
    s = py::re::sub(py::str("([a-z])([A-Z])"), py::str("\\1 \\2"), s);
    s = py::re::sub(py::str("([A-Z])([A-Z][a-z])"), py::str("\\1 \\2"), s);
    return !s.empty() ? s.slice_to(1).upper() + s.slice_from(1) : key;
}

// Parse a .lang file for `achievement.<key>=Name` and `achievement.<key>.desc=Desc`. Returns
// {key: {'name':..., 'desc':...}}. Empty if the file is missing.
py::dict<py::str, LangName> load_lang_names(const std::optional<py::str>& lang_file) {
    py::dict<py::str, LangName> out;
    if (!(lang_file.has_value() && !lang_file->empty()) || !py::os::path::isfile(*lang_file)) {
        return out;
    }
    py::str text = textutil::read_text(*lang_file, py::str("replace"), false);
    for (const py::str& line : py::split(text, py::str("\n"))) {   // (the file's newlines are already universal)
        if (!line.contains(py::str("=")) || !line.startswith(py::str("achievement."))) {
            continue;
        }
        int64_t eq = line.find(py::str("="));
        py::str k = line.slice_to(eq);
        py::str v = line.slice_from(eq + 1);
        py::str rest = k.slice_from(py::len(py::str("achievement.")));
        if (rest.endswith(py::str(".desc"))) {
            py::str key = rest.slice_to(-5);
            LangName n = out.contains(key) ? out.at(key) : LangName();
            n.desc = v;
            out.set(key, n);
        } else {
            LangName n = out.contains(rest) ? out.at(rest) : LangName();
            n.name = v;
            out.set(rest, n);
        }
    }
    return out;
}

// Find every achievement the program defines. Returns a sorted list of
// {id, key, name, desc, hidden, parent}. `hidden` = the program's 'special/secret' flag.
py::list<Achievement> scan_achievements(const py::str& code_dir, const std::optional<py::str>& lang_file) {
    py::dict<py::str, LangName> names = load_lang_names(lang_file);
    py::dict<int64_t, Achievement> found;
    for (const auto& [dp, _dirs, files] : py::os::walk(code_dir)) {
        for (const py::str& f : files) {
            if (!(f.endswith(py::str(".cpp")) || f.endswith(py::str(".java")))) {
                continue;
            }
            py::str text = textutil::read_text(py::os::path::join(dp, f), py::str("replace"), true);   // (unreadable -> '')
            if (!text.contains(py::str("new Achievement("))) {
                continue;
            }
            for (const py::str& line : py::split(text, py::str("\n"))) {
                std::optional<py::re::Match> m = NEW_ACH_().search(line);
                if (!m.has_value()) {
                    continue;
                }
                int64_t aid = py::int_from_str(m->group(1));
                py::str key = m->group(2);
                std::optional<py::re::Match> pm = PARENT_().search(line);
                std::optional<py::str> parent = pm.has_value() ? std::optional<py::str>(pm->group(1)) : std::nullopt;
                if (parent.has_value() && (*parent == "NULL" || *parent == "0" || *parent == "null")) {
                    parent = std::nullopt;
                }
                std::optional<py::re::Match> im = ICON_().search(line);
                LangName nm = names.contains(key) ? names.at(key) : LangName();
                Achievement a;
                a.id = aid;
                a.key = key;
                a.name = (nm.name.has_value() && !nm.name->empty()) ? *nm.name : _humanize(key);
                a.desc = (nm.desc.has_value() && !nm.desc->empty())
                             ? *nm.desc
                             : py::percent("Earn the %s achievement.", _humanize(key));
                a.hidden = SPECIAL_().search(line).has_value();
                a.parent = parent;
                a.icon = im.has_value() ? std::optional<py::str>(im->group(1)) : std::nullopt;   // e.g. 'Item::s_book' / 'Block::s_wood'
                found.set(aid, a);
            }
        }
    }
    py::list<Achievement> result;
    for (int64_t k : py::sorted(found.keys())) {
        result.append(found.at(k));
    }
    return result;
}

// --- icon resolution: symbol -> atlas sprite -------------------------------
// Items place their sprite with setIconCoord(col,row) (or setIconIndex(n)) in gui/items.png. Blocks
// carry a terrain.png index either as a literal 2nd ctor int (base Block(id, tex, mat)) OR, for a
// subclass, as `this->blockIndexInTexture = N;` inside the subclass constructor. Resolve both.
namespace {
const py::re::Pattern& ITEM_DEF_() {
    static const py::re::Pattern p = py::re::compile(py::str(R"rx(Item::(s_\w+)\s*=\s*(.+?);)rx"));
    return p;
}
const py::re::Pattern& ICON_COORD_() {
    static const py::re::Pattern p = py::re::compile(py::str(R"rx(setIconCoord\(\s*(\d+)\s*,\s*(\d+)\s*\))rx"));
    return p;
}
const py::re::Pattern& ICON_INDEX_() {
    static const py::re::Pattern p = py::re::compile(py::str(R"rx(setIconIndex\(\s*(\d+)\s*\))rx"));
    return p;
}
const py::re::Pattern& BLOCK_ASSIGN_() {
    static const py::re::Pattern p = py::re::compile(py::str(R"rx(Block::(s_\w+)\s*=\s*\(new\s+(\w+)\(([^)]*)\))rx"));
    return p;
}
const py::re::Pattern& BIT_LITERAL_() {
    static const py::re::Pattern p = py::re::compile(py::str(R"rx(blockIndexInTexture\s*=\s*(\d+)\s*;)rx"));
    return p;
}
const py::re::Pattern& INT_ONLY_() {
    static const py::re::Pattern p = py::re::compile(py::str(R"rx(^\s*(\d+)\s*$)rx"));
    return p;
}
}  // namespace

py::str _read(const py::str& path) {
    return textutil::read_text(path, py::str("replace"), true);   // tolerant: errors='replace', missing -> ''
}

// {BlockSubclass: terrainIndex} from each subclass ctor's literal `blockIndexInTexture = N;`.
// The base `Block` sets it from a ctor arg (no literal) and is intentionally absent here.
py::dict<py::str, int64_t> _block_class_textures(const py::str& code_dir) {
    py::dict<py::str, int64_t> out;
    for (const auto& [dp, _dirs, files] : py::os::walk(code_dir)) {
        for (const py::str& f : files) {
            if (!(f.startswith(py::str("Block")) && f.endswith(py::str(".cpp"))) || f == "Block.cpp") {
                continue;
            }
            std::optional<py::re::Match> m = BIT_LITERAL_().search(_read(py::os::path::join(dp, f)));
            if (m.has_value()) {
                out.set(f.slice_to(-4), py::int_from_str(m->group(1)));
            }
        }
    }
    return out;
}

// Map an icon symbol (s_book / s_wood) to (atlas, col, row), atlas in {'items','terrain'}, by
// reading the game's Item/Block definitions. Unresolvable symbols are simply absent.
py::dict<py::str, std::tuple<py::str, int64_t, int64_t>> resolve_icon_atlas(const py::str& code_dir) {
    py::dict<py::str, std::tuple<py::str, int64_t, int64_t>> out;
    py::dict<py::str, int64_t> class_tex = _block_class_textures(code_dir);
    std::optional<py::str> item_cpp;
    std::optional<py::str> block_cpp;
    for (const auto& [dp, _dirs, files] : py::os::walk(code_dir)) {
        for (const py::str& f : files) {
            if (f == "Item.cpp") {
                item_cpp = py::os::path::join(dp, f);
            } else if (f == "Block.cpp") {
                block_cpp = py::os::path::join(dp, f);
            }
        }
    }
    if (item_cpp.has_value()) {
        py::str text = _read(*item_cpp);
        for (const py::re::Match& m : ITEM_DEF_().finditer(text)) {
            py::str sym = m.group(1);
            py::str body = m.group(2);
            std::optional<py::re::Match> cm = ICON_COORD_().search(body);
            if (cm.has_value()) {
                out.set(sym, std::make_tuple(py::str("items"), py::int_from_str(cm->group(1)), py::int_from_str(cm->group(2))));
                continue;
            }
            std::optional<py::re::Match> im = ICON_INDEX_().search(body);
            if (im.has_value()) {
                int64_t n = py::int_from_str(im->group(1));
                out.set(sym, std::make_tuple(py::str("items"), py::mod(n, 16), py::floordiv(n, 16)));
            }
        }
    }
    if (block_cpp.has_value()) {
        py::str text = _read(*block_cpp);
        for (const py::re::Match& m : BLOCK_ASSIGN_().finditer(text)) {
            py::str sym = m.group(1);
            py::str cls = m.group(2);
            py::str args = m.group(3);
            std::optional<int64_t> tex = class_tex.contains(cls) ? std::optional<int64_t>(class_tex.at(cls)) : std::nullopt;   // subclass ctor's literal index
            if (!tex.has_value()) {                    // base Block(id, tex, mat): 2nd arg literal
                py::list<py::str> parts = py::split(args, py::str(","));
                if (py::len(parts) >= 2 && INT_ONLY_().match(parts[1]).has_value()) {
                    tex = py::int_from_str(parts[1].strip());
                }
            }
            if (tex.has_value()) {
                out.set(sym, std::make_tuple(py::str("terrain"), py::mod(*tex, 16), py::floordiv(*tex, 16)));
            }
        }
    }
    return out;
}

// --- PNG: what wire_icons needs of Pillow (Image.open().convert('RGBA'), crop, resize NEAREST, save) -----------
namespace {

struct Rgba {
    int64_t w = 0;
    int64_t h = 0;
    std::vector<unsigned char> px;   // w * h * 4, row by row
    unsigned char* at(int64_t x, int64_t y) { return &px[static_cast<size_t>((y * w + x) * 4)]; }
    const unsigned char* at(int64_t x, int64_t y) const { return &px[static_cast<size_t>((y * w + x) * 4)]; }
};

uint32_t be32(const std::string& d, size_t at) {
    return (static_cast<uint32_t>(static_cast<unsigned char>(d[at])) << 24) |
           (static_cast<uint32_t>(static_cast<unsigned char>(d[at + 1])) << 16) |
           (static_cast<uint32_t>(static_cast<unsigned char>(d[at + 2])) << 8) |
           static_cast<uint32_t>(static_cast<unsigned char>(d[at + 3]));
}

void put_be32(std::string& o, uint32_t v) {
    for (int k = 3; k >= 0; --k) o.push_back(static_cast<char>((v >> (8 * k)) & 0xFF));
}

void put_chunk(std::string& o, const char* tag, const std::string& data) {
    put_be32(o, static_cast<uint32_t>(data.size()));
    std::string td = std::string(tag, 4) + data;
    o += td;
    put_be32(o, static_cast<uint32_t>(py::crc32(py::bytes(td))));
}

int paeth_(int a, int b, int c) {
    int p = a + b - c, pa = std::abs(p - a), pb = std::abs(p - b), pc = std::abs(p - c);
    if (pa <= pb && pa <= pc) return a;
    return pb <= pc ? b : c;
}

// Undo the PNG row filters of `rows` scanlines of `stride` bytes (each led by its filter byte) read from
// `src` at `off`; the unfiltered bytes (without the filter bytes) are appended to `out`.
void unfilter_rows(const std::string& src, size_t off, int64_t rows, int64_t stride, int64_t bpp, std::string& out) {
    std::string prev(static_cast<size_t>(stride), '\0');
    for (int64_t y = 0; y < rows; ++y) {
        size_t o = off + static_cast<size_t>(y * (stride + 1));
        int ft = static_cast<unsigned char>(src[o]);
        std::string row = src.substr(o + 1, static_cast<size_t>(stride));
        for (int64_t i = 0; i < stride; ++i) {
            int a = i >= bpp ? static_cast<unsigned char>(row[static_cast<size_t>(i - bpp)]) : 0;
            int b = static_cast<unsigned char>(prev[static_cast<size_t>(i)]);
            int c = i >= bpp ? static_cast<unsigned char>(prev[static_cast<size_t>(i - bpp)]) : 0;
            int x = static_cast<unsigned char>(row[static_cast<size_t>(i)]);
            switch (ft) {
                case 0: break;
                case 1: x += a; break;
                case 2: x += b; break;
                case 3: x += (a + b) / 2; break;
                case 4: x += paeth_(a, b, c); break;
                default: throw py::OSError("unknown filter type");
            }
            row[static_cast<size_t>(i)] = static_cast<char>(x & 0xFF);
        }
        out += row;
        prev = row;
    }
}

// Image.open(path).convert('RGBA') for a PNG.
Rgba read_png(const py::str& path) {
    std::string d = py::open_rb(path).read().raw();
    static const unsigned char SIG[8] = {0x89, 'P', 'N', 'G', '\r', '\n', 0x1a, '\n'};
    if (d.size() < 8 || std::memcmp(d.data(), SIG, 8) != 0) {
        throw assets::UnidentifiedImageError("cannot identify image file");
    }
    if (assets::png_needs_rgba(py::bytes(d))) {
        d = std::get<0>(assets::normalize_png_to_rgba(py::bytes(d))).raw();   // any other colour type -> 8-bit RGBA
    }
    size_t pos = 8;
    std::string ihdr, idat;
    bool seen_end = false;
    while (pos + 8 <= d.size()) {
        uint32_t len = be32(d, pos);
        std::string type = d.substr(pos + 4, 4);
        if (pos + 12 + static_cast<size_t>(len) > d.size()) throw py::OSError("image file is truncated");
        std::string data = d.substr(pos + 8, len);
        pos += 12 + static_cast<size_t>(len);
        if (type == "IHDR") ihdr = data;
        else if (type == "IDAT") idat += data;
        else if (type == "IEND") { seen_end = true; break; }
    }
    (void)seen_end;
    if (ihdr.size() != 13) throw assets::UnidentifiedImageError("cannot identify image file");
    int64_t w = be32(ihdr, 0), h = be32(ihdr, 4);
    int depth = static_cast<unsigned char>(ihdr[8]), ctype = static_cast<unsigned char>(ihdr[9]),
        interlace = static_cast<unsigned char>(ihdr[12]);
    if (ctype != 6 || (depth != 8 && depth != 16) || interlace > 1) throw py::OSError("unsupported PNG");
    int64_t bpp = depth / 8 * 4;
    // the inflated image data
    std::string raw;
    {
        z_stream zs;
        std::memset(&zs, 0, sizeof zs);
        if (inflateInit(&zs) != Z_OK) throw py::OSError("zlib error");
        raw.resize(static_cast<size_t>(h * (1 + w * bpp)) * (interlace ? 2 : 1) + 1024);
        zs.next_in = reinterpret_cast<Bytef*>(&idat[0]);
        zs.avail_in = static_cast<uInt>(idat.size());
        zs.next_out = reinterpret_cast<Bytef*>(&raw[0]);
        zs.avail_out = static_cast<uInt>(raw.size());
        int rc = inflate(&zs, Z_FINISH);
        raw.resize(raw.size() - zs.avail_out);
        inflateEnd(&zs);
        if (rc != Z_STREAM_END) throw py::OSError("image file is truncated");
    }
    Rgba img;
    img.w = w;
    img.h = h;
    img.px.assign(static_cast<size_t>(w * h * 4), 0);
    auto put = [&](int64_t x, int64_t y, const std::string& row, int64_t i) {
        unsigned char* p = img.at(x, y);
        for (int c = 0; c < 4; ++c) p[c] = static_cast<unsigned char>(row[static_cast<size_t>(i * bpp + c * (depth / 8))]);   // (16 bit: the high byte)
    };
    if (!interlace) {
        int64_t stride = w * bpp;
        if (static_cast<int64_t>(raw.size()) < h * (stride + 1)) throw py::OSError("image file is truncated");
        std::string flat;
        unfilter_rows(raw, 0, h, stride, bpp, flat);
        for (int64_t y = 0; y < h; ++y) {
            std::string row = flat.substr(static_cast<size_t>(y * stride), static_cast<size_t>(stride));
            for (int64_t x = 0; x < w; ++x) put(x, y, row, x);
        }
    } else {
        static const int P[7][4] = {{0, 0, 8, 8}, {4, 0, 8, 8}, {0, 4, 4, 8}, {2, 0, 4, 4}, {0, 2, 2, 4}, {1, 0, 2, 2}, {0, 1, 1, 2}};
        size_t off = 0;
        for (const auto& p : P) {
            int64_t pw = (w - p[0] + p[2] - 1) / p[2], ph = (h - p[1] + p[3] - 1) / p[3];
            if (w - p[0] <= 0 || h - p[1] <= 0 || pw <= 0 || ph <= 0) continue;
            int64_t stride = pw * bpp;
            if (raw.size() < off + static_cast<size_t>(ph * (stride + 1))) throw py::OSError("image file is truncated");
            std::string flat;
            unfilter_rows(raw, off, ph, stride, bpp, flat);
            off += static_cast<size_t>(ph * (stride + 1));
            for (int64_t y = 0; y < ph; ++y) {
                std::string row = flat.substr(static_cast<size_t>(y * stride), static_cast<size_t>(stride));
                for (int64_t x = 0; x < pw; ++x) put(p[0] + x * p[2], p[1] + y * p[3], row, x);
            }
        }
    }
    return img;
}

// Image.save(path) as an 8-bit RGBA PNG (one IDAT, filter 0 rows).
void write_png(const py::str& path, const Rgba& img) {
    std::string scan;
    scan.reserve(img.px.size() + static_cast<size_t>(img.h));
    for (int64_t y = 0; y < img.h; ++y) {
        scan.push_back('\0');
        scan.append(reinterpret_cast<const char*>(img.at(0, y)), static_cast<size_t>(img.w * 4));
    }
    std::string zdata;
    {
        z_stream zs;
        std::memset(&zs, 0, sizeof zs);
        if (deflateInit(&zs, Z_DEFAULT_COMPRESSION) != Z_OK) throw py::OSError("zlib error");
        zdata.resize(deflateBound(&zs, static_cast<uLong>(scan.size())));
        zs.next_in = reinterpret_cast<Bytef*>(&scan[0]);
        zs.avail_in = static_cast<uInt>(scan.size());
        zs.next_out = reinterpret_cast<Bytef*>(&zdata[0]);
        zs.avail_out = static_cast<uInt>(zdata.size());
        int rc = deflate(&zs, Z_FINISH);
        zdata.resize(zdata.size() - zs.avail_out);
        deflateEnd(&zs);
        if (rc != Z_STREAM_END) throw py::OSError("zlib error");
    }
    std::string png("\x89PNG\r\n\x1a\n", 8);
    std::string hdr;
    put_be32(hdr, static_cast<uint32_t>(img.w));
    put_be32(hdr, static_cast<uint32_t>(img.h));
    hdr += std::string("\x08\x06\x00\x00\x00", 5);
    put_chunk(png, "IHDR", hdr);
    put_chunk(png, "IDAT", zdata);
    put_chunk(png, "IEND", "");
    py::file f = py::open(path, py::str("wb"));
    f.write(py::bytes(png));
    f.close();
}

// img.crop((l, t, r, b)): pixels outside the image come out as transparent black.
Rgba crop(const Rgba& img, int64_t l, int64_t t, int64_t r, int64_t b) {
    Rgba o;
    o.w = r - l;
    o.h = b - t;
    o.px.assign(static_cast<size_t>(o.w * o.h * 4), 0);
    for (int64_t y = 0; y < o.h; ++y) {
        for (int64_t x = 0; x < o.w; ++x) {
            int64_t sx = l + x, sy = t + y;
            if (sx >= 0 && sx < img.w && sy >= 0 && sy < img.h) std::memcpy(o.at(x, y), img.at(sx, sy), 4);
        }
    }
    return o;
}

// img.resize((size, size), Image.NEAREST)
Rgba resize_nearest(const Rgba& img, int64_t size) {
    if (size <= 0) throw py::ValueError("height and width must be > 0");
    Rgba o;
    o.w = size;
    o.h = size;
    o.px.assign(static_cast<size_t>(size * size * 4), 0);
    double sx = static_cast<double>(img.w) / static_cast<double>(size), sy = static_cast<double>(img.h) / static_cast<double>(size);
    // Pillow's nearest scale (Geometry.c ImagingScaleAffine): the source position of an output pixel is
    // accumulated, starting at half a step -- not computed as (x + 0.5) * step; the roundings differ.
    std::vector<int64_t> xin(static_cast<size_t>(size)), yin(static_cast<size_t>(size));
    double xo = sx * 0.5, yo = sy * 0.5;
    for (int64_t i = 0; i < size; ++i) {
        xin[static_cast<size_t>(i)] = std::min<int64_t>(static_cast<int64_t>(xo), img.w - 1);
        yin[static_cast<size_t>(i)] = std::min<int64_t>(static_cast<int64_t>(yo), img.h - 1);
        xo += sx;
        yo += sy;
    }
    for (int64_t y = 0; y < size; ++y) {
        for (int64_t x = 0; x < size; ++x) {
            std::memcpy(o.at(x, y), img.at(xin[static_cast<size_t>(x)], yin[static_cast<size_t>(y)]), 4);
        }
    }
    return o;
}

// A locked-achievement tile: desaturated + darkened, alpha preserved.
Rgba _locked_variant(const Rgba& img) {
    Rgba g = img;   // img.convert('LA').convert('RGBA'): grey = Pillow's L (ITU-R 601-2 luma), alpha kept
    for (int64_t y = 0; y < g.h; ++y) {
        for (int64_t x = 0; x < g.w; ++x) {
            unsigned char* p = g.at(x, y);
            uint32_t l = (static_cast<uint32_t>(p[0]) * 19595u + static_cast<uint32_t>(p[1]) * 38470u +
                          static_cast<uint32_t>(p[2]) * 7471u + 0x8000u) >> 16;
            p[0] = p[1] = p[2] = static_cast<unsigned char>(l);
        }
    }
    for (int64_t y = 0; y < g.h; ++y) {
        for (int64_t x = 0; x < g.w; ++x) {
            unsigned char* p = g.at(x, y);
            p[0] = static_cast<unsigned char>(p[0] * 40 / 100);
            p[1] = static_cast<unsigned char>(p[1] * 40 / 100);
            p[2] = static_cast<unsigned char>(p[2] * 40 / 100);
        }
    }
    return g;
}

const py::dict<py::str, py::str>& ATLAS_FILE_() {
    static const py::dict<py::str, py::str> d = py::dict<py::str, py::str>{
        {py::str("items"), py::os::path::join(py::str("gui"), py::str("items.png"))}, {py::str("terrain"), py::str("terrain.png")}};
    return d;
}

}  // namespace

// For each achievement whose icon resolves, crop its 16x16 sprite from the atlas, scale to
// `tile`x`tile` (nearest, crisp), and write unlocked + locked PNGs under out_dir/achievement_images/.
// Mutates each entry with 'image_unlocked'/'image_locked' (basenames) when wired. Returns
// {'wired': [...keys], 'unresolved': [(key, reason)]}.
IconWiring wire_icons(const py::list<py::ref<Entry>>& entries, const py::str& code_dir, const py::str& content_dir,
                      const py::str& out_dir, int64_t tile) {
    IconWiring result;
    py::list<py::str>& wired = result.wired;
    py::list<std::tuple<py::str, py::str>>& unresolved = result.unresolved;
    if (!_HAS_PIL) {
        for (const py::ref<Entry>& e : entries) {
            unresolved.append(std::make_tuple(e->key, py::str("Pillow not installed")));
        }
        return IconWiring{py::list<py::str>(), unresolved};
    }
    py::dict<py::str, std::tuple<py::str, int64_t, int64_t>> atlas_map = resolve_icon_atlas(code_dir);
    std::map<std::string, Rgba> atlases;
    // FIXED BUG assets-in-project-root: write the achievement PNGs UNDER the deploy Content dir
    // (Debug\Content\achievement_images), never the project root. Refs below are made relative to out_dir
    // (where achievements.xlast lives) so the XLAST tool still resolves them.
    py::str img_dir = py::os::path::join(content_dir, py::str("achievement_images"));
    for (const py::ref<Entry>& e : entries) {
        const std::optional<py::str>& icon = e->icon;
        std::optional<py::str> sym;
        if (icon.has_value() && !icon->empty()) {
            py::list<py::str> parts = py::split(*icon, py::str("::"));
            sym = parts[-1];
        }
        if (!(sym.has_value() && !sym->empty()) || !atlas_map.contains(*sym)) {
            unresolved.append(std::make_tuple(e->key, py::percent("icon %s not resolvable to an atlas sprite",
                                                                  (icon.has_value() && !icon->empty()) ? *icon : py::str("(none)"))));
            continue;
        }
        auto [atlas, col, row] = atlas_map.at(*sym);
        if (atlases.find(atlas.utf8()) == atlases.end()) {
            py::str ap = py::os::path::join(content_dir, ATLAS_FILE_().at(atlas).replace(py::str("/"), py::os::sep));
            if (!py::os::path::isfile(ap)) {
                unresolved.append(std::make_tuple(e->key, py::percent("atlas %s missing at %s", atlas, ap)));
                continue;
            }
            atlases[atlas.utf8()] = read_png(ap);
        }
        // (Python also has `elif atlases[atlas] is None: ... 'atlas %s missing'` here: the entry is never None.)
        const Rgba& sheet = atlases[atlas.utf8()];
        Rgba sprite = crop(sheet, col * 16, row * 16, col * 16 + 16, row * 16 + 16);
        sprite = resize_nearest(sprite, tile);
        if (!py::os::path::isdir(img_dir)) {
            py::os::makedirs(img_dir);
        }
        py::str un = py::percent("%s_unlocked.png", e->key);
        py::str lk = py::percent("%s_locked.png", e->key);
        write_png(py::os::path::join(img_dir, un), sprite);
        write_png(py::os::path::join(img_dir, lk), _locked_variant(sprite));
        e->image_unlocked = py::os::path::relpath(py::os::path::join(img_dir, un), out_dir).replace(py::str("/"), py::str("\\"));
        e->image_locked = py::os::path::relpath(py::os::path::join(img_dir, lk), out_dir).replace(py::str("/"), py::str("\\"));
        wired.append(e->key);
    }
    return result;
}

const py::dict<py::str, Profile>& PROFILES() {
    static const py::dict<py::str, Profile> d = py::dict<py::str, Profile>{
        {py::str("retail"), Profile{99, 1000}}, {py::str("arcade"), Profile{12, 200}}};
    return d;
}

// Split `total` gamerscore across n achievements as evenly as possible in steps of 5, summing
// EXACTLY to total (the last few absorb the remainder).
py::list<int64_t> _distribute_score(int64_t n, int64_t total) {
    if (n <= 0) {
        return py::list<int64_t>{};
    }
    int64_t base = py::floordiv(py::floordiv(total, n), 5) * 5;
    py::list<int64_t> scores = py::repeat(py::list<int64_t>{base}, n);
    int64_t remaining = total - base * n;
    int64_t i = 0;
    while (remaining > 0 && i < n) {
        scores[i] = py::checked_add(scores[i], 5);
        remaining -= 5;
        i += 1;
    }
    // if base was 0 (n huge), fall back to 1s so nothing is zero-valued
    if (base == 0 && total >= n) {
        scores = py::repeat(py::list<int64_t>{py::floordiv(total, n)}, n);
        scores[-1] = py::checked_add(scores[-1], py::checked_sub(total, py::sum(scores)));
    }
    return scores;
}

// Assign a platform achievement id (1-based, as consoles expect) + gamerscore to each. Returns
// {'entries':[...], 'profile':..., 'total_score':..., 'warnings':[...]}.
Mapping map_to_platform(const py::list<Achievement>& achievements, const py::str& profile,
                        const std::optional<py::str>& /*title_profile*/) {
    Profile prof = PROFILES().get(profile, PROFILES().at("retail"));
    int64_t n = py::len(achievements);
    py::list<py::str> warnings;
    if (n > prof.max_count) {
        warnings.append(py::percent("%d achievements exceeds the %s limit of %d -- split across titles or raise the profile.",
                                    n, profile, prof.max_count));
    }
    py::list<int64_t> scores = _distribute_score(n, prof.score);
    py::list<py::ref<Entry>> entries;
    int64_t i = 0;
    for (const Achievement& a : achievements) {
        auto e = std::make_shared<Entry>();
        e->platform_id = i + 1;            // consoles number achievements from 1
        e->source_id = a.id;
        e->key = a.key;
        e->name = a.name;
        e->desc = a.desc;
        e->gamerscore = i < py::len(scores) ? scores[i] : 0;
        e->hidden = a.hidden;
        e->icon = a.icon;
        entries.append(e);
        i += 1;
    }
    int64_t total = 0;
    for (const py::ref<Entry>& e : entries) {
        total += e->gamerscore;
    }
    if (total != prof.score && n > 0) {
        warnings.append(py::percent("gamerscore sums to %d, not the %s target of %d.", total, profile, prof.score));
    }
    Mapping m;
    m.entries = entries;
    m.profile = profile;
    m.total_score = total;
    m.warnings = warnings;
    m.count = n;
    m.limit = prof.max_count;
    return m;
}

// --- emission ---------------------------------------------------------------
py::str _const_name(const py::str& key) {
    return py::str("ACH_") + py::re::sub(py::str("([a-z])([A-Z])"), py::str("\\1_\\2"), key).upper();
}

py::str render_report(const Mapping& mapping, const py::str& delivery, const std::optional<IconWiring>& icons,
                      const std::optional<py::str>& title_id, bool title_suggested) {
    const py::list<py::ref<Entry>>& e = mapping.entries;
    py::list<py::str> L = py::list<py::str>{
        py::str("# ACHIEVEMENTS.md -- achievement conversion"), py::str(""),
        py::percent("The program defines %d achievement(s). A console does NOT let code invent achievements at "
                    "runtime -- each must be declared in the title configuration and unlocked through a platform "
                    "call that writes to the signed-in profile over the online service. This maps the program's "
                    "achievements onto that model.", mapping.count),
        py::str(""),
        // PROFILES[mapping['profile']]: a KeyError for a profile that is not one of the two
        py::percent("- profile: **%s** (max %d, %d gamerscore)   assigned total: **%d**   delivery: **%s**",
                    mapping.profile, mapping.limit, PROFILES().at(mapping.profile).score, mapping.total_score, delivery),
        py::str("")};
    for (const py::str& w : mapping.warnings) {
        L.append(py::percent("> WARNING: %s", w));
    }
    if (!mapping.warnings.empty()) {
        L.append(py::str(""));
    }
    L = L + py::list<py::str>{py::str("## Achievement table"), py::str(""),
                              py::str("| # | Key | Name | Gamerscore | Hidden | Tile art |"),
                              py::str("|---:|---|---|---:|:---:|---|")};
    for (const py::ref<Entry>& x : e) {
        py::str art = (x->image_unlocked.has_value() && !x->image_unlocked->empty())
                          ? py::percent("`%s`", *x->image_unlocked)
                          : py::str("TODO");
        L.append(py::percent("| %d | `%s` | %s | %d | %s | %s |", x->platform_id, x->key, x->name, x->gamerscore,
                             x->hidden ? "yes" : "", art));
    }
    if (icons.has_value()) {
        L = L + py::list<py::str>{
            py::str(""), py::str("### Tile art"), py::str(""),
            py::str("Achievement tiles were auto-wired from the game's own item/block icons (cropped from the "
                    "texture atlas, scaled to 64x64; a darkened/desaturated locked variant is generated). They "
                    "land in `achievement_images/` and are referenced by the XLAST project."),
            py::str(""),
            py::percent("- auto-wired: **%d**   needs manual art: **%d**", py::len(icons->wired), py::len(icons->unresolved))};
        if (!icons->unresolved.empty()) {
            L = L + py::list<py::str>{py::str(""), py::str("Icons not auto-resolved (add art by hand in the XLAST project):"), py::str("")};
            for (const auto& [key, reason] : icons->unresolved) {
                L.append(py::percent("- `%s` \xE2\x80\x94 %s", key, reason));
            }
        }
        L.append(py::str(""));
    }
    L = L + py::list<py::str>{
        py::str(""), py::str("## What the console requires (and the source program does not provide)"), py::str(""),
        py::str("1. **Declared up front.** Every achievement above must be entered into the title's "
                "configuration tool (Xbox 360: an XLAST project, compiled to an SPA blob embedded in the "
                "build). A skeleton is emitted as `achievements.xlast` -- import it, add the locked/unlocked "
                "tile art, and build the SPA."),
        py::str("2. **Unlocked via a networked write.** Each in-code unlock becomes a call to the platform "
                "achievement write (Xbox 360: `XUserWriteAchievements`). This is the telemetry/networking "
                "path you flagged: it needs"),
        py::str("   - a **Title ID** assigned to the build,"),
        py::str("   - a **signed-in profile** (tie this to the *User / profile* decision),"),
        py::str("   - the **network** for immediate sync -- **offline it queues and syncs on next connect**."),
        py::str("3. **Budget + limits.** Gamerscore must sum to the profile target and the count must stay "
                "under the limit (validated above)."),
        py::str("4. **Hidden achievements.** The program's \"special/secret\" flag maps to the console "
                "\"hidden\" attribute (shown above)."),
        py::str(""),
        py::str("## Title ID"), py::str(""),
        py::str("The achievement config (SPA) and the built XEX are both keyed to a **32-bit Title ID** "
                "(high 16 bits = publisher, low 16 = title number). It must be the SAME in the XLAST project "
                "and on the XEX, or unlocks will not bind."),
        py::str(""),
        py::percent("- Title ID: **0x%s**%s", (title_id.has_value() && !title_id->empty()) ? *title_id : py::str("00000000"),
                    title_suggested ? "  _(suggested \xE2\x80\x94 see below)_" : ""),
        py::str(""),
        py::str("Since Xbox 360 titles are no longer submitted to Microsoft, you **self-issue** this: there is "
                "no database to request one from. Pick a value and make sure it does not collide with an "
                "existing title (community title-ID lists exist to check against). The suggested id uses a high "
                "publisher prefix (`0xFFFE`) unlikely to clash and a title number derived from the name; "
                "override it with your own if you prefer."),
        py::str(""),
        py::str("Set the same value in **Visual Studio -> project properties -> Xbox 360 -> Title ID** (it is "
                "written into the XEX at build), and in the XLAST project (already set in the emitted "
                "`achievements.xlast`)."),
        py::str(""),
        py::str("## Code seam"), py::str(""),
        py::str("Emitted `Achievements.h/.cpp` provides `Achievement_Unlock(userIndex, ACH_*)`. The converter "
                "routes each in-code unlock (e.g. the program's `triggerAchievement`) to it; on hardware it "
                "performs the platform write, off-hardware it is a safe no-op. The `ACH_*` id constants match "
                "the table above."),
        py::str("")};
    if (delivery != "online") {
        L = L + py::list<py::str>{
            py::percent("> NOTE: delivery is **%s** -- unlocks are tracked locally only, no online write. Switch "
                        "the *Achievements* decision to `xbox_live` for real profile achievements.", delivery),
            py::str("")};
    }
    return py::join(py::str("\n"), L) + py::str("\n");
}

// Write an XLAST-style achievement config skeleton (XML) to import into the submission tool.
py::str emit_xlast(const Mapping& mapping, const py::str& out_path, const std::optional<py::str>& title_id) {
    std::optional<py::str> n = normalize_title_id(title_id);
    py::str tid = n.has_value() ? *n : py::str("00000000");
    py::list<py::str> L = py::list<py::str>{
        py::str("<?xml version=\"1.0\" encoding=\"utf-8\"?>"),
        py::str("<!-- Achievement config skeleton emitted by CodeBreaker. Import into the platform submission/config"),
        py::str("     tool (Xbox 360: XLAST), then build the SPA. The titleId below MUST match the Title ID set"),
        py::str("     on the XEX in the Visual Studio project (Xbox 360 property pages). -->"),
        py::percent("<XLASTProject titleId=\"0x%s\">", tid),
        py::str("  <Configuration>"),
        py::str("    <Achievements>")};
    for (const py::ref<Entry>& x : mapping.entries) {
        L.append(py::percent("      <Achievement id=\"%d\" name=\"%s\" gamerscore=\"%d\" hidden=\"%s\">",
                             x->platform_id, _xml(x->name), x->gamerscore, x->hidden ? "true" : "false"));
        L.append(py::percent("        <LockedDescription>%s</LockedDescription>", _xml(x->desc)));
        L.append(py::percent("        <UnlockedDescription>%s</UnlockedDescription>", _xml(x->desc)));
        if (x->image_unlocked.has_value() && !x->image_unlocked->empty()) {
            L.append(py::percent("        <Image type=\"unlocked\">%s</Image>", _xml(*x->image_unlocked)));
            if (!x->image_locked.has_value()) {
                throw py::KeyError("image_locked");   // x['image_locked'] of a row that has only image_unlocked
            }
            L.append(py::percent("        <Image type=\"locked\">%s</Image>", _xml(*x->image_locked)));
        } else {
            L.append(py::str("        <!-- TODO: add locked/unlocked 64x64 tile (icon not auto-resolved) -->"));
        }
        L.append(py::str("      </Achievement>"));
    }
    L = L + py::list<py::str>{py::str("    </Achievements>"), py::str("  </Configuration>"), py::str("</XLASTProject>"), py::str("")};
    textutil::write_text(out_path, py::join(py::str("\n"), L));
    return out_path;
}

py::str _xml(const py::str& s) {
    return s.replace(py::str("&"), py::str("&amp;")).replace(py::str("<"), py::str("&lt;")).replace(py::str(">"), py::str("&gt;")).replace(py::str("\""), py::str("&quot;"));
}

// Write Achievements.h + Achievements.cpp: the ACH_* id constants and the unlock seam.
py::list<py::str> emit_code(const Mapping& mapping, const py::str& out_dir, const py::str& delivery) {
    py::list<py::str> h = py::list<py::str>{py::str("// Achievements.h -- CodeBreaker-emitted achievement ids + unlock seam. See ACHIEVEMENTS.md."), py::str("#ifndef J2X_ACHIEVEMENTS_H"), py::str("#define J2X_ACHIEVEMENTS_H"), py::str(""), py::str("// Platform achievement ids (match the title config / XLAST project).")};
    for (const py::ref<Entry>& x : mapping.entries) {
        h.append(py::percent("#define %-28s %d", _const_name(x->key), x->platform_id));
    }
    h = h + py::list<py::str>{py::str(""), py::str("// Unlock an achievement for a signed-in user. On hardware this writes to the profile over"), py::str("// the online service (queues offline); off-hardware it is a no-op returning true."), py::str("bool Achievement_Unlock(int userIndex, int achievementId);"), py::str(""), py::str("#endif // J2X_ACHIEVEMENTS_H"), py::str("")};

    bool live = delivery == "online";
    py::list<py::str> c = py::list<py::str>{py::str("// Achievements.cpp -- CodeBreaker-emitted. Routes in-code unlocks to the platform write."), py::str("#include \"stdafx.h\""), py::str("#include \"Achievements.h\""), py::str(""), py::str("#if defined(_XBOX) || defined(_XENON)"), py::str("  #include <xtl.h>"), py::str("#endif"), py::str("#include <string.h>"), py::str(""), py::str("bool Achievement_Unlock(int userIndex, int achievementId)"), py::str("{")};
    if (live) {
        c = c + py::list<py::str>{py::str("#if defined(_XBOX) || defined(_XENON)"), py::str("    XUSER_ACHIEVEMENT ach;"), py::str("    ach.dwUserIndex = (DWORD)userIndex;"), py::str("    ach.dwAchievementId = (DWORD)achievementId;"), py::str("    XOVERLAPPED ov; memset(&ov, 0, sizeof(ov));"), py::str("    // async LIVE write; queues + syncs later if offline."), py::str("    DWORD r = XUserWriteAchievements(1, &ach, &ov);"), py::str("    if (r != ERROR_IO_PENDING && r != ERROR_SUCCESS) return false;"), py::str("    while (!XHasOverlappedIoCompleted(&ov)) { }"), py::str("    DWORD ext = 0; return XGetOverlappedResult(&ov, &ext, TRUE) == ERROR_SUCCESS;"), py::str("#else"), py::str("    (void)userIndex; (void)achievementId; return true;"), py::str("#endif")};
    } else {
        c = c + py::list<py::str>{py::percent("    // J2X-STUB[achievements-local]: %s delivery -- track locally, no online write.", delivery), py::str("    (void)userIndex; (void)achievementId; return true;")};
    }
    c = c + py::list<py::str>{py::str("}"), py::str("")};
    py::str hp = py::os::path::join(out_dir, py::str("Achievements.h"));
    py::str cp = py::os::path::join(out_dir, py::str("Achievements.cpp"));
    textutil::write_text(hp, py::join(py::str("\n"), h));
    textutil::write_text(cp, py::join(py::str("\n"), c));
    return py::list<py::str>{hp, cp};
}

// Scan, map, wire icons, and emit ACHIEVEMENTS.md + achievements.xlast + Achievements.h/.cpp.
// `content_dir` (with the game's atlases) enables auto-wiring the achievement tile art. `title_id`
// (self-issued, no MS submission needed) is written into the XLAST; if omitted, a stable one is
// suggested from `title_name`. Returns a dict including the icon-wiring result + title id.
RunResult run(const py::str& code_dir, const py::str& out_dir, const std::optional<py::str>& lang_file,
              const py::str& profile, const py::str& delivery, const std::optional<py::str>& content_dir,
              const std::optional<py::str>& title_id, const std::optional<py::str>& /*title_name*/) {
    if (!py::os::path::isdir(out_dir)) {
        py::os::makedirs(out_dir);
    }
    py::list<Achievement> achs = scan_achievements(code_dir, lang_file);
    Mapping mapping = map_to_platform(achs, profile);
    std::optional<py::str> cdir = (content_dir.has_value() && !content_dir->empty()) ? content_dir : _guess_content_dir(out_dir);
    IconWiring icons;
    if (cdir.has_value() && !cdir->empty()) {
        icons = wire_icons(mapping.entries, code_dir, *cdir, out_dir);
    } else {
        // Say WHY, per entry. Reporting {'wired': [], 'unresolved': []} here -- zero of BOTH -- read as
        // "nothing to do" and is exactly how 16 TODO tiles went unnoticed. A missing atlas source is an
        // unresolved icon, not an absence of work.
        for (const py::ref<Entry>& e : mapping.entries) {
            icons.unresolved.append(std::make_tuple(
                e->key, py::percent("no Content dir with terrain.png / gui/items.png found near %s "
                                    "(run `codebreaker java assets` first, or pass --content-dir)", out_dir)));
        }
    }
    // Title ID resolution, in precedence order: an explicit --title-id, else the project's
    // platform.config.json (the ONE place it lives -- `codebreaker java vcxproj` stamps the SAME value into the
    // XEX, and the two MUST match or unlocks never bind to a profile), else the conventional "unset"
    // placeholder 00000000. We do NOT auto-issue one: the user picks it (Suggest button / --title-id).
    std::optional<py::str> t1 = normalize_title_id(title_id);
    py::str tid = t1.has_value() ? *t1 : py::str("");
    if (!t1.has_value()) {
        std::optional<py::str> t2 = normalize_title_id(_title_id_from_config(out_dir));
        tid = t2.has_value() ? *t2 : py::str("00000000");
    }
    bool suggested = false;
    py::str report = py::os::path::join(out_dir, py::str("ACHIEVEMENTS.md"));
    {
        // (Python opens the file before it renders the report: a render that raises leaves an empty file.)
        py::file f = py::open(report, py::str("w"), py::str("utf-8"));
        f.write(render_report(mapping, delivery, icons, tid, suggested));
        f.close();
    }
    py::str xlast = emit_xlast(mapping, py::os::path::join(out_dir, py::str("achievements.xlast")), tid);
    py::list<py::str> code = emit_code(mapping, out_dir, delivery);
    RunResult r;
    r.mapping = mapping;
    r.report = report;
    r.xlast = xlast;
    r.code = code;
    r.content_dir = cdir;
    r.count = mapping.count;
    r.icons = icons;
    r.title_id = tid;
    r.title_suggested = suggested;
    return r;
}

// The project's title_id from <out_dir>/platform.config.json, or None. Shared with vcxproj.py so the
// XLAST/SPA and the XEX are stamped from the same value.
std::optional<py::str> _title_id_from_config(const py::str& out_dir) {
    py::str path = py::os::path::join(out_dir, py::str("platform.config.json"));
    if (!py::os::path::isfile(path)) {
        return std::nullopt;
    }
    try {
        py::file f = py::open(path, py::str("r"), py::str("utf-8"));
        py::str text = f.read_text();
        f.close();
        py::V cfg = py::json_parse(text.utf8());
        if (!cfg.is_dict()) {
            throw py::AttributeError("config object has no attribute 'get'");   // json.load(f).get(...) of a list / str / number
        }
        const py::V& t = cfg.get("title_id");
        if (t.is_str() && !t.str().empty()) return py::str(t.str());
        if (t.is_int() && t.as_int() != 0) return py::str(std::to_string(t.as_int()));   // (str(int), as normalize_title_id reads it)
        return std::nullopt;   // None, 0, '', [], {}, False -- and a float / list / dict / True, which are never a title id
    } catch (const py::OSError&) {
        return std::nullopt;
    } catch (const py::ValueError&) {
        return std::nullopt;
    } catch (const py::PyError& e) {
        if (e.type == "ValueError") return std::nullopt;
        throw;
    }
}

// Find the game's Content folder (the one with the texture atlases) near out_dir.
//
// FIXED BUG achievement-art-content-dir: this only looked at <out_dir>/Content and <out_dir>, but the
// asset pipeline deliberately writes assets to <out_dir>\Debug\Content -- that IS `game:\Content`,
// the path next to the Debug .xex (see the assets-in-project-root note in vcxproj.py; other configs are
// mirrored from Debug by the DeployContent target). So the guess always came back None on a real project
// and wire_icons was never even called: the achievements step reported "0 auto-wired, 0 need manual art"
// -- zero of BOTH, which reads like "nothing to do" rather than "the atlases were never found" -- and all
// 16 tiles stayed TODO. Debug is checked first because it is the canonical deploy dir.
std::optional<py::str> _guess_content_dir(const py::str& out_dir) {
    py::list<py::str> cands = py::list<py::str>{py::os::path::join(out_dir, py::str("Debug"), py::str("Content")), py::os::path::join(out_dir, py::str("Content")), out_dir, py::os::path::join(out_dir, py::str("Release"), py::str("Content")), py::os::path::join(out_dir, py::str("Release_LTCG"), py::str("Content"))};
    for (const py::str& cand : cands) {
        if (py::os::path::isfile(py::os::path::join(cand, py::str("terrain.png"))) || py::os::path::isfile(py::os::path::join(cand, py::str("gui"), py::str("items.png")))) {
            return cand;
        }
    }
    return std::nullopt;
}

}  // namespace codebreaker::java::projects::minecraft::achievements
