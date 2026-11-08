// codebreaker/formats/resources.cpp -- typed C++ of codebreaker.formats.resources (see resources.hpp).
//
// The decoders are the C++ of native/formats/win/resources.cc (which answered in pickled values), written out
// on typed values over native/formats/core's cbf::Bytes (core/bytes.cc, core/text.cc), so a damaged resource
// gives the Python's exception: IndexError past the end, struct.error for a value that does not fit.

#include "codebreaker/formats/resources.hpp"

#include <algorithm>
#include <string>
#include <vector>

#include "../../../../formats/core/bytes.h"
#include "../../../../formats/core/text.h"

namespace codebreaker::formats::resources {

namespace {

namespace cb = ::cbf;

// ---- the exception an answer carried: native._exception(kind, message)
[[noreturn]] void raise_py(const cb::PyError& e) {
    const std::string& k = e.kind;
    const std::string& m = e.message;
    if (k == "struct.error") throw py::StructError(m);
    if (k == "KeyError") throw py::KeyError(py::repr(py::str(m)));
    if (k == "UnicodeDecodeError") throw py::UnicodeDecodeError("'ascii' codec can't decode bytes in position 0-0: " + m);
    if (k == "UnicodeEncodeError") throw py::UnicodeEncodeError("'latin-1' codec can't encode characters in position 0-0: " + m);
    if (k == "ValueError") throw py::ValueError(m);
    if (k == "IndexError") throw py::IndexError(m);
    if (k == "TypeError") throw py::TypeError(m);
    if (k == "MemoryError") throw py::MemoryError(m);
    if (k == "ZeroDivisionError") throw py::ZeroDivisionError(m);
    if (k == "OverflowError") throw py::OverflowError(m);
    if (k == "AttributeError") throw py::AttributeError(m);
    if (k == "OSError") throw py::OSError(m);
    if (k == "AssertionError") throw py::AssertionError(m);
    throw py::RuntimeError(m);
}

// runs a call into the bytes reader, its exceptions made the Python's
template <class F>
auto native(F&& f) -> decltype(f()) {
    try {
        return f();
    } catch (const cb::PyError& e) {
        raise_py(e);
    }
}

cb::Bytes B(const py::bytes& b) { return cb::Bytes(b.raw()); }
py::bytes PB(const std::string& s) { return py::bytes(s); }

// data.decode("latin1")
py::str latin1_str(cb::Bytes b) {
    std::u32string s;
    for (const uint8_t c : b) s.push_back(c);
    return py::str(std::move(s));
}

// data.decode("utf-16-le", "surrogatepass"): a lone surrogate stays a code point of its own
py::str utf16le_pass(cb::Bytes b) {
    if (b.size() & 1) cb::raise("UnicodeDecodeError", "truncated data");
    std::u32string out;
    for (int64_t i = 0; i < b.size(); i += 2) {
        uint32_t u = b.u16(i);
        if (u >= 0xD800 && u < 0xDC00 && i + 3 < b.size()) {
            uint32_t w = b.u16(i + 2);
            if (w >= 0xDC00 && w < 0xE000) {
                out.push_back(0x10000 + ((u - 0xD800) << 10) + (w - 0xDC00));
                i += 2;
                continue;
            }
        }
        out.push_back(u);
    }
    return py::str(std::move(out));
}

// the fields of a template, read in order (a packed 16-bit one, or a 4-aligned 32-bit one)
struct Reader {
    cb::Bytes d;
    bool wide;
    int64_t p;
    Reader(cb::Bytes data, bool w, int64_t at = 0) : d(data), wide(w), p(at) {}

    int64_t u8() { p += 1; return d.at(p - 1); }
    int64_t u16() { p += 2; return d.u16(p - 2); }
    int64_t i16() { p += 2; return d.i16(p - 2); }
    int64_t u32() { p += 4; return d.u32(p - 4); }
    cb::Bytes take(int64_t n) {
        if (p + n > d.size()) cb::raise("ValueError", "resource body ends early");
        p += n;
        return d.slice(p - n, p);
    }
    void align(int64_t n = 4) { p = (p + n - 1) / n * n; }
    py::str text() {
        if (wide) {
            int64_t e = p;
            while (e + 2 <= d.size() && !(d.at(e) == 0 && d.at(e + 1) == 0)) e += 2;
            if (e + 2 > d.size()) cb::raise("ValueError", "unterminated text");
            py::str s = utf16le_pass(d.slice(p, e));
            p = e + 2;
            return s;
        }
        int64_t e = d.index_byte(0, p);
        py::str s = latin1_str(d.slice(p, e));
        p = e + 1;
        return s;
    }
    Ident text_or_ordinal() {
        if (wide) {
            if (d.u16(p) == 0xFFFF) {
                p += 2;
                return Ident(u16());
            }
        } else if (d.at(p) == 0xFF) {
            p += 1;
            return Ident(u16());
        }
        return Ident(text());
    }
};

// ---- strings -------------------------------------------------------------------------------
py::list<py::str> strings_(cb::Bytes data, bool wide) {
    Reader r(data, wide);
    py::list<py::str> out;
    for (int k = 0; k < 16; ++k) {
        if (r.p >= data.size()) {
            out.append(py::str(""));
            continue;
        }
        if (wide) {
            int64_t n = r.u16();
            out.append(utf16le_pass(r.take(2 * n)));
        } else {
            out.append(latin1_str(r.take(r.u8())));
        }
    }
    cb::Bytes rest = data.slice(r.p);
    for (const uint8_t b : rest)
        if (b) cb::raise("ValueError", "data after the sixteenth string");
    return out;
}

// ---- dialog --------------------------------------------------------------------------------
Dialog dialog_(cb::Bytes data, bool wide) {
    Reader r(data, wide);
    Dialog dlg;
    bool extended = false;
    int64_t count;
    if (wide && data.slice(0, 4).is("\x01\x00\xff\xff", 4)) {
        r.p = 4;
        extended = true;
        dlg.extended = true;
        int64_t help = r.u32(), ex = r.u32();
        dlg.style = r.u32();
        dlg.help = help;
        dlg.exstyle = ex;
        count = r.u16();
    } else {
        dlg.style = r.u32();
        if (wide) dlg.exstyle = r.u32();
        count = wide ? r.u16() : r.u8();
    }
    dlg.x = r.i16();
    dlg.y = r.i16();
    dlg.cx = r.i16();
    dlg.cy = r.i16();
    dlg.menu = r.text_or_ordinal();
    dlg.class_ = r.text_or_ordinal();
    dlg.title = r.text();
    if (dlg.style & 0x40) {                                  // DS_SETFONT
        if (extended) {
            int64_t points = r.u16(), weight = r.u16();
            int64_t italic = r.u8(), charset = r.u8();
            dlg.font = DialogFontExtended(points, weight, italic, charset, r.text());
        } else {
            int64_t points = r.u16();
            dlg.font = DialogFontPlain(points, r.text());
        }
    }
    for (int64_t k = 0; k < count; ++k) {
        DialogControl c;
        if (wide) r.align();
        auto rect = [&] {
            c.x = r.i16();
            c.y = r.i16();
            c.cx = r.i16();
            c.cy = r.i16();
        };
        if (extended) {
            c.help = r.u32();
            c.exstyle = r.u32();
            c.style = r.u32();
            rect();
            c.id = r.u32();
        } else if (wide) {
            c.style = r.u32();
            c.exstyle = r.u32();
            rect();
            c.id = r.u16();
        } else {
            rect();
            c.id = r.u16();
            c.style = r.u32();
        }
        if (wide) c.class_ = r.text_or_ordinal();
        else c.class_ = (data.at(r.p) & 0x80) ? Ident(r.u8()) : Ident(r.text());
        c.text = r.text_or_ordinal();
        int64_t n = wide ? r.u16() : r.u8();
        c.extra = PB(r.take(n).str());
        dlg.controls.append(c);
    }
    return dlg;
}

// ---- menu ----------------------------------------------------------------------------------
py::list<ExtMenuItem> menu_level_ext(Reader& r) {
    py::list<ExtMenuItem> items;
    while (true) {
        r.align();
        ExtMenuItem item;
        item.type = r.u32();
        item.state = r.u32();
        item.id = r.u32();
        int64_t kind = r.u16();
        item.text = r.text();
        if (kind & 0x01) {                                   // opens a submenu
            r.align();
            item.help = r.u32();
            item.items = menu_level_ext(r);
        }
        items.append(item);
        if (kind & 0x80) return items;
    }
}

py::list<MenuItem> menu_level(Reader& r) {
    py::list<MenuItem> items;
    while (true) {
        int64_t flags = r.u16();
        MenuItem item;
        item.flags = flags & ~0x80;
        if (!(flags & 0x10)) item.id = r.u16();
        item.text = r.text();
        if (flags & 0x10) item.items = menu_level(r);
        items.append(item);
        if (flags & 0x80) return items;
    }
}

Menu menu_(cb::Bytes data, bool wide) {
    Reader r(data, wide);
    int64_t version_ = r.u16(), offset = r.u16();
    Menu out;
    if (version_ == 1 && wide) {
        out.extended = true;
        out.help = r.u32();
        r.p = 4 + offset;
        out.items = menu_level_ext(r);
    } else if (version_ == 0) {
        out.extended = false;
        out.help = 0;
        r.p = 4 + offset;
        out.items = menu_level(r);
    } else {
        cb::raise("ValueError", "menu template version " + std::to_string(version_));
    }
    return out;
}

// ---- accelerators --------------------------------------------------------------------------
py::list<std::tuple<int64_t, int64_t, int64_t>> accelerators_(cb::Bytes data, bool wide) {
    int64_t size = wide ? 8 : 5;
    py::list<std::tuple<int64_t, int64_t, int64_t>> out;
    for (int64_t p = 0; p < data.size() - size + 1; p += size) {
        int64_t flags, key, ident;
        if (wide) {
            flags = data.u16(p);
            key = data.u16(p + 2);
            ident = data.u16(p + 4);
        } else {
            flags = data.u8(p);
            key = data.u16(p + 1);
            ident = data.u16(p + 3);
        }
        out.append(std::make_tuple(flags & 0x7F, key, ident));
        if (flags & 0x80) break;
    }
    return out;
}

// ---- version -------------------------------------------------------------------------------
struct VNode {
    py::str key;
    cb::Bytes value;
    bool text = false;
    std::vector<VNode> kids;
    int64_t end = 0;
};

VNode vnode(cb::Bytes data, bool wide, int64_t p, int64_t limit) {
    int64_t length, vlen, kind, at;
    if (wide) {
        length = data.u16(p);
        vlen = data.u16(p + 2);
        kind = data.u16(p + 4);
        at = p + 6;
    } else {
        length = data.u16(p);
        vlen = data.u16(p + 2);
        kind = 0;
        at = p + 4;
    }
    Reader r(data, wide, at);
    if (length < 4) cb::raise("ValueError", "version block length");
    int64_t end = std::min(p + length, limit);
    VNode n;
    n.key = r.text();
    r.align();
    if (wide && kind == 1 && r.p + 2 * vlen <= end) vlen *= 2;      // counted in characters
    n.value = data.slice(r.p, std::min(r.p + vlen, end));
    int64_t q = (r.p + vlen + 3) & ~int64_t(3);
    while (q + 4 <= end) {
        VNode k = vnode(data, wide, q, end);
        q = (k.end + 3) & ~int64_t(3);
        n.kids.push_back(std::move(k));
    }
    n.text = kind == 1;
    n.end = end;
    return n;
}

py::str text_of(cb::Bytes value, bool wide) {
    std::string s = wide ? cb::utf16le(value) : cb::latin1(value);
    size_t z = s.find('\0');
    return py::str(z == std::string::npos ? s : s.substr(0, z));
}

Version version_(cb::Bytes data, bool wide) {
    VNode top = vnode(data, wide, 0, data.size());
    if (top.key != py::str("VS_VERSION_INFO")) cb::raise("ValueError", "not a version block");
    Version out;
    if (top.value.size() >= 52) {
        py::list<int64_t> fixed;
        for (int k = 0; k < 13; ++k) fixed.append(top.value.u32(4 * k));
        out.fixed = fixed;
    }
    for (const VNode& k : top.kids) {
        if (k.key == py::str("StringFileInfo")) {
            for (const VNode& lang : k.kids) {
                py::list<std::tuple<py::str, py::str>> entries;
                for (const VNode& e : lang.kids) entries.append(std::make_tuple(e.key, text_of(e.value, wide)));
                out.strings.set(lang.key, entries);
            }
        } else if (k.key == py::str("VarFileInfo")) {
            for (const VNode& v : k.kids)
                for (int64_t i = 0; i + 1 < v.value.size(); i += 2) out.translations.append(v.value.u16(i));
        } else {
            cb::raise("ValueError", "a version block with a '" + k.key.utf8() + "' part");
        }
    }
    return out;
}

// ---- icon / cursor groups, bitmaps ---------------------------------------------------------
int64_t group_entries(cb::Bytes data, std::vector<GroupEntry>& out) {
    int64_t kind = data.u16(2), count = data.u16(4);
    data.u16(0);
    if ((kind != 1 && kind != 2) || 6 + 14 * count > data.size()) cb::raise("ValueError", "not an icon or cursor group");
    for (int64_t i = 0; i < count; ++i) {
        int64_t p = 6 + 14 * i;
        if (kind == 1) {
            out.push_back(GroupEntry(data.u8(p), data.u8(p + 1), data.u8(p + 2), data.u16(p + 4), data.u16(p + 6), data.u32(p + 8), data.u16(p + 12)));
        } else {
            out.push_back(GroupEntry(data.u16(p), data.u16(p + 2), 0, data.u16(p + 4), data.u16(p + 6), data.u32(p + 8), data.u16(p + 12)));
        }
    }
    return kind;
}

void le16(std::string& s, uint64_t v) { s += char(v); s += char(v >> 8); }
void le32(std::string& s, uint64_t v) { le16(s, v & 0xFFFF); le16(s, (v >> 16) & 0xFFFF); }
void packI(std::string& s, int64_t v) {
    if (v < 0 || v > 0xFFFFFFFFLL) cb::raise("struct.error", "'I' format requires 0 <= number <= 4294967295");
    le32(s, v);
}
void packB(std::string& s, int64_t v) {
    if (v < 0 || v > 255) cb::raise("struct.error", "ubyte format requires 0 <= number <= 255");
    s += char(v);
}
void packH(std::string& s, int64_t v) {
    if (v < 0 || v > 65535) cb::raise("struct.error", "ushort format requires 0 <= number <= 65535");
    le16(s, v);
}

// resources.icon_file(group_data, [member bytes ...]) -> the file
std::string icon_file_(cb::Bytes group_data, const std::vector<py::bytes>& members) {
    std::vector<GroupEntry> es;
    int64_t kind = group_entries(group_data, es);
    std::string head, table, body;
    le16(head, 0);
    le16(head, kind);
    le16(head, es.size());
    int64_t at = 6 + 16 * static_cast<int64_t>(es.size());
    for (size_t i = 0; i < es.size(); ++i) {
        const GroupEntry& e = es[i];
        int64_t w = std::get<0>(e), h = std::get<1>(e), colours = std::get<2>(e), planes = std::get<3>(e), bits = std::get<4>(e);
        cb::Bytes image = B(members[i]);
        if (kind == 2) {
            int64_t hx = image.u16(0), hy = image.u16(2);
            image = image.slice(4);
            packB(table, w & 0xFF);
            packB(table, (h / 2) & 0xFF);
            packB(table, 0);
            packB(table, 0);
            packH(table, hx);
            packH(table, hy);
        } else {
            packB(table, w);
            packB(table, h);
            packB(table, colours);
            packB(table, 0);
            packH(table, planes);
            packH(table, bits);
        }
        packI(table, image.size());
        packI(table, at);
        body += image.str();
        at += image.size();
    }
    return head + table + body;
}

std::string bitmap_file_(cb::Bytes data) {
    int64_t size = data.size() >= 4 ? data.u32(0) : 0, palette = 0;
    if (size == 12 && data.size() >= 12) {
        int64_t bits = data.u16(10);
        palette = bits <= 8 ? 3 * (int64_t(1) << bits) : 0;
    } else if (size >= 40 && data.size() >= 40) {
        int64_t bits = data.u16(14), compression = data.u32(16), used = data.u32(32);
        palette = 4 * (used ? used : (bits <= 8 ? (int64_t(1) << bits) : 0)) + (compression == 3 && size == 40 ? 12 : 0);
    }
    std::string s = "BM";
    le32(s, 14 + data.size());
    le16(s, 0);
    le16(s, 0);
    packI(s, 14 + size + palette);
    return s + data.str();
}

// ---- type libraries ------------------------------------------------------------------------
const char* kTypeKinds[] = {"enum", "struct", "module", "interface", "dispinterface", "coclass", "typedef", "union"};

std::optional<TypeLibrary> type_library_(cb::Bytes data) {
    cb::Bytes tag = data.slice(0, 4);
    std::string kind = tag.is("MSFT", 4) ? "MSFT" : tag.is("SLTG", 4) ? "SLTG" : "";
    if (kind.empty()) return std::nullopt;
    TypeLibrary out;
    out.format = py::str(kind);
    out.name = py::str("");
    out.guid = py::str("");
    if (kind == "SLTG" || data.size() < 0x54) return out;
    try {
        int64_t own_guid = data.i32(8), flags = data.u32(0x14), count = data.u32(0x20), own_name = data.i32(0x38);
        int64_t directory = 0x54 + 4 * count + ((flags & 0x100) ? 4 : 0);
        int64_t seg[15];
        for (int i = 0; i < 15; ++i) {
            seg[i] = data.i32(directory + 16 * i);
            data.i32(directory + 16 * i + 4);
        }
        int64_t info_at = seg[0], guid_at = seg[5], name_at = seg[7];
        auto guid = [&](int64_t at) -> std::string {
            if (at < 0) return "";
            cb::Bytes b = data.slice(guid_at + at, guid_at + at + 16);
            uint32_t a = b.u32(0);
            uint32_t cc = b.u16(4), e = b.u16(6);
            auto pad = [](std::string s, size_t n) { return std::string(n > s.size() ? n - s.size() : 0, '0') + s; };
            auto hx = [&](int64_t from, int64_t to) {
                std::string s;
                cb::Bytes x = b.slice(from, to);
                for (const uint8_t v : x) s += pad(cb::hex(v, true), 2);
                return s;
            };
            return pad(cb::hex(a, true), 8) + "-" + pad(cb::hex(cc, true), 4) + "-" + pad(cb::hex(e, true), 4) + "-" + hx(8, 10) + "-" + hx(10, 16);
        };
        auto name = [&](int64_t at) -> py::str {
            if (at < 0) return py::str("");
            int64_t length = data.at(name_at + at + 8);
            return latin1_str(data.slice(name_at + at + 12, name_at + at + 12 + length));
        };
        py::str nm = name(own_name);
        std::string gd = guid(own_guid);
        out.name = nm;
        out.guid = py::str(gd);
        for (int64_t n = 0; n < count; ++n) {
            int64_t at = info_at + 0x64 * n;
            int64_t k = data.u32(at) & 0xF;
            std::string kn = k < 8 ? kTypeKinds[k] : "type " + std::to_string(k);
            py::str tn = name(data.i32(at + 0x34));
            std::string tg = guid(data.i32(at + 0x2C));
            out.types.append(std::make_tuple(py::str(kn), tn, py::str(tg)));
        }
    } catch (const cb::PyError& e) {
        if (e.kind != "struct.error" && e.kind != "IndexError") throw;
    }
    return out;
}

// resources.read_ne(data, ne_off, rsrc_tab, resname_tab) -> [(type, name, flags, bytes)]
py::list<std::tuple<Ident, Ident, int64_t, py::bytes>> read_ne_(cb::Bytes d, int64_t ne_off, int64_t rsrc, int64_t resname) {
    py::list<std::tuple<Ident, Ident, int64_t, py::bytes>> out;
    if (rsrc == resname) return out;
    int64_t base = ne_off + rsrc;
    auto ident = [&](int64_t v) -> Ident {
        if (v & 0x8000) return Ident(v & 0x7FFF);
        int64_t n = d.at(base + v);
        return Ident(latin1_str(d.slice(base + v + 1, base + v + 1 + n)));
    };
    int64_t shift = d.u16(base);
    if (shift > 15) return out;
    int64_t p = base + 2;
    while (p + 8 <= d.size()) {
        int64_t tid = d.u16(p), count = d.u16(p + 2);
        if (tid == 0) break;
        p += 8;
        for (int64_t k = 0; k < count; ++k) {
            if (p + 12 > d.size()) return out;
            int64_t off = cb::py_shl(d.u16(p), shift), length = cb::py_shl(d.u16(p + 2), shift), flags = d.u16(p + 4), rid = d.u16(p + 6);
            Ident t = ident(tid), n = ident(rid);
            out.append(std::make_tuple(t, n, flags, PB(d.slice(off, off + length).str())));
            p += 12;
        }
    }
    return out;
}

py::str ident_repr(const Ident& v) {
    return std::visit([](const auto& x) { return py::repr(x); }, v);
}

}  // namespace

const py::dict<int64_t, py::str>& TYPES() {
    static const py::dict<int64_t, py::str> t = {
        {1, py::str("CURSOR")},      {2, py::str("BITMAP")},       {3, py::str("ICON")},        {4, py::str("MENU")},
        {5, py::str("DIALOG")},      {6, py::str("STRING")},       {7, py::str("FONTDIR")},     {8, py::str("FONT")},
        {9, py::str("ACCELERATOR")}, {10, py::str("RCDATA")},      {11, py::str("MESSAGETABLE")}, {12, py::str("GROUP_CURSOR")},
        {14, py::str("GROUP_ICON")}, {16, py::str("VERSION")},     {17, py::str("DLGINCLUDE")}, {19, py::str("PLUGPLAY")},
        {20, py::str("VXD")},        {21, py::str("ANICURSOR")},   {22, py::str("ANIICON")},    {23, py::str("HTML")},
        {24, py::str("MANIFEST")}};
    return t;
}

const py::dict<int64_t, int64_t>& MEMBER_OF() {
    static const py::dict<int64_t, int64_t> t = {{GROUP_ICON, ICON}, {GROUP_CURSOR, CURSOR}};
    return t;
}

const py::dict<int64_t, py::str>& CONTROL_CLASSES() {
    static const py::dict<int64_t, py::str> t = {{0x80, py::str("BUTTON")}, {0x81, py::str("EDIT")},      {0x82, py::str("STATIC")},
                                                 {0x83, py::str("LISTBOX")}, {0x84, py::str("SCROLLBAR")}, {0x85, py::str("COMBOBOX")}};
    return t;
}

py::str Resource::repr() const {
    return py::str("<") + type_name(type) + py::str(" ") + ident_repr(name) + py::str(" lang=") +
           (language ? py::repr(*language) : py::str("None")) + py::str(" ") + py::str(std::to_string(data.size())) + py::str(" bytes>");
}

py::str type_name(const Ident& t) {
    if (const int64_t* n = std::get_if<int64_t>(&t)) return TYPES().get(*n, py::str("#" + std::to_string(*n)));
    return std::get<py::str>(t);
}

// ---------------------------------------------------------------------------------------------
// reading the tree
// ---------------------------------------------------------------------------------------------
py::list<Resource> read(const py::str& path) {
    py::file f = py::open_rb(path);
    py::bytes head = f.read(0x40);
    if (head.size() < 0x40 || head.slice(0, 2) != py::bytes("MZ")) throw py::ValueError(path.utf8() + ": not an executable");
    f.seek(py::unpack_from1<uint32_t>("<I", head, 0x3C));
    py::bytes sig = f.read(2);
    f.close();
    if (sig == py::bytes("PE")) return read_pe(pe::open_pe(path));
    if (sig == py::bytes("NE")) return read_ne(ne::NEFile(path));
    return py::list<Resource>();
}

py::list<Resource> read_pe(const pe::PEFile& pe_) {
    py::list<Resource> out;
    py::list<pe::ResourceType> tree = pe_.resource_tree();
    for (const pe::ResourceType& t : tree.vec()) {
        Ident rtype = t.id ? Ident(*t.id) : std::visit([](const auto& x) { return Ident(x); }, t.type);
        for (const pe::ResourceItem& item : t.items.vec()) {
            std::optional<int64_t> off = item.offset;
            int64_t size = item.size;
            if (!off || *off + size > pe_.data.size()) continue;
            Ident name = item.id ? Ident(*item.id) : Ident(item.name);
            out.append(Resource(rtype, name, item.language, 0, pe_.data.slice(*off, *off + size)));
        }
    }
    return out;
}

py::list<Resource> read_ne(const ne::NEFile& nef) {
    py::list<Resource> out;
    auto rows = native([&] { return read_ne_(B(nef.data), nef.ne_off, nef.h.rsrc_tab, nef.h.resname_tab); });
    for (const auto& row : rows.vec()) out.append(Resource(std::get<0>(row), std::get<1>(row), std::nullopt, std::get<2>(row), std::get<3>(row)));
    return out;
}

bool same_data(const py::bytes& a, const py::bytes& b) {
    int64_t n = std::min(a.size(), b.size());
    if (a.slice(0, n) != b.slice(0, n)) return false;
    for (char c : a.slice(n, a.size()).raw())
        if (c) return false;
    for (char c : b.slice(n, b.size()).raw())
        if (c) return false;
    return true;
}

// ---------------------------------------------------------------------------------------------
// decoding each kind
// ---------------------------------------------------------------------------------------------
py::list<py::str> strings(const py::bytes& data, bool wide) { return native([&] { return strings_(B(data), wide); }); }
Dialog dialog(const py::bytes& data, bool wide) { return native([&] { return dialog_(B(data), wide); }); }
Menu menu(const py::bytes& data, bool wide) { return native([&] { return menu_(B(data), wide); }); }
py::list<std::tuple<int64_t, int64_t, int64_t>> accelerators(const py::bytes& data, bool wide) {
    return native([&] { return accelerators_(B(data), wide); });
}
Version version(const py::bytes& data, bool wide) { return native([&] { return version_(B(data), wide); }); }

std::tuple<int64_t, py::list<GroupEntry>> group(const py::bytes& data) {
    return native([&] {
        std::vector<GroupEntry> es;
        int64_t kind = group_entries(B(data), es);
        return std::make_tuple(kind, py::list<GroupEntry>(es));
    });
}

py::bytes icon_file(const py::bytes& group_data, const std::function<py::bytes(int64_t)>& member_data) {
    std::vector<py::bytes> members;
    py::list<GroupEntry> entries = std::get<1>(group(group_data));
    for (const GroupEntry& entry : entries.vec()) members.push_back(member_data(std::get<6>(entry)));
    return native([&] { return PB(icon_file_(B(group_data), members)); });
}

py::bytes bitmap_file(const py::bytes& data) { return native([&] { return PB(bitmap_file_(B(data))); }); }

std::optional<TypeLibrary> type_library(const py::bytes& data) { return native([&] { return type_library_(B(data)); }); }

py::str type_library_outline(const TypeLibrary& library) {
    py::list<py::str> out;
    out.append(py::str("// Outline of a type library (") + library.format + py::str(" layout). Not compilable: only what each type is called."));
    if (!library.guid.u32().empty()) out.append(py::str("[ uuid(") + library.guid + py::str(") ]"));
    out.append(py::str("library ") + (library.name.u32().empty() ? py::str("unnamed") : library.name) + py::str("\n{"));
    for (const auto& t : library.types.vec()) {
        const py::str& kind = std::get<0>(t);
        const py::str& name = std::get<1>(t);
        const py::str& guid = std::get<2>(t);
        out.append(py::str("    ") + (guid.u32().empty() ? py::str("") : py::str("[ uuid(") + guid + py::str(") ] ")) + kind + py::str(" ") +
                   (name.u32().empty() ? py::str("/* no name */") : name) + py::str(";"));
    }
    out.append(py::str("}"));
    out.append(py::str(""));
    return py::join(py::str("\n"), out);
}

Decoded decode(const Resource& resource, bool wide) {
    const int64_t* type = std::get_if<int64_t>(&resource.type);
    if (!type) return std::monostate{};
    try {
        switch (*type) {
            case STRING: return strings(resource.data, wide);
            case DIALOG: return dialog(resource.data, wide);
            case MENU: return menu(resource.data, wide);
            case ACCELERATOR: return accelerators(resource.data, wide);
            case VERSION: {
                Version v = version(resource.data, wide);
                if (v.fixed) {                               // without the file date: a compiler stamps it
                    py::list<int64_t> head;
                    for (int64_t i = 0; i < 11; ++i) head.append((*v.fixed)[i]);
                    v.fixed = head;
                }
                return v;
            }
            default: return std::monostate{};
        }
    } catch (const py::ValueError&) {                        // (UnicodeDecodeError is one)
        return std::monostate{};
    } catch (const py::IndexError&) {
        return std::monostate{};
    } catch (const py::StructError&) {
        return std::monostate{};
    }
}

}  // namespace codebreaker::formats::resources
