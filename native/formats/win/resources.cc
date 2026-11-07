// resources.cc -- the resources of a Windows module, decoded: codebreaker/formats/resources.py.
// The decoders (strings, dialog, menu, accelerators, version, group, type_library), the 16-bit
// resource table and the .ico / .cur / .bmp makers; the shim keeps the Resource class, the
// PE walk over pe.resource_tree and decode()'s choice of decoder.
#include <string>
#include <vector>

#include "../core/build.h"
#include "../core/questions.h"
#include "../core/text.h"

namespace cbf::resources {
namespace {

// data.decode("utf-16-le", "surrogatepass"): a lone surrogate stays a code point of its own
std::string utf16le_pass(Bytes b) {
    if (b.size() & 1) raise("UnicodeDecodeError", "truncated data");
    std::string out;
    for (int64_t i = 0; i < b.size(); i += 2) {
        uint32_t u = b.u16(i);
        if (u >= 0xD800 && u < 0xDC00 && i + 3 < b.size()) {
            uint32_t w = b.u16(i + 2);
            if (w >= 0xDC00 && w < 0xE000) {
                put_codepoint(out, 0x10000 + ((u - 0xD800) << 10) + (w - 0xDC00));
                i += 2;
                continue;
            }
        }
        put_codepoint(out, u);          // put_codepoint writes a surrogate as its three-byte form
    }
    return out;
}

struct Reader {
    Bytes d;
    bool wide;
    int64_t p;
    Reader(Bytes data, bool w, int64_t at = 0) : d(data), wide(w), p(at) {}

    int64_t u8() { p += 1; return d.at(p - 1); }
    int64_t u16() { p += 2; return d.u16(p - 2); }
    int64_t i16() { p += 2; return d.i16(p - 2); }
    int64_t u32() { p += 4; return d.u32(p - 4); }
    Bytes take(int64_t n) {
        if (p + n > d.size()) raise("ValueError", "resource body ends early");
        p += n;
        return d.slice(p - n, p);
    }
    void align(int64_t n = 4) { p = (p + n - 1) / n * n; }
    std::string text() {
        if (wide) {
            int64_t e = p;
            while (e + 2 <= d.size() && !(d.at(e) == 0 && d.at(e + 1) == 0)) e += 2;
            if (e + 2 > d.size()) raise("ValueError", "unterminated text");
            std::string s = utf16le_pass(d.slice(p, e));
            p = e + 2;
            return s;
        }
        int64_t e = d.index_byte(0, p);
        std::string s = latin1(d.slice(p, e));
        p = e + 1;
        return s;
    }
    Value text_or_ordinal() {
        if (wide) {
            if (d.u16(p) == 0xFFFF) {
                p += 2;
                return vint(u16());
            }
        } else if (d.at(p) == 0xFF) {
            p += 1;
            return vint(u16());
        }
        return vstr(text());
    }
};

// ---- strings -------------------------------------------------------------------------------
Value strings(Bytes data, bool wide) {
    Reader r(data, wide);
    Value out = vlist();
    for (int k = 0; k < 16; ++k) {
        if (r.p >= data.size()) {
            push(out, vstr(""));
            continue;
        }
        if (wide) {
            int64_t n = r.u16();
            push(out, vstr(utf16le_pass(r.take(2 * n))));
        } else {
            push(out, vstr(latin1(r.take(r.u8()))));
        }
    }
    Bytes rest = data.slice(r.p);
    for (const uint8_t b : rest)
        if (b) raise("ValueError", "data after the sixteenth string");
    return out;
}

// ---- dialog --------------------------------------------------------------------------------
Value dialog(Bytes data, bool wide) {
    Reader r(data, wide);
    Value dlg = vdict();
    put(dlg, "extended", vbool(false));
    put(dlg, "help", vint(0));
    put(dlg, "exstyle", vint(0));
    bool extended = false;
    int64_t style, count;
    if (wide && data.slice(0, 4).is("\x01\x00\xff\xff", 4)) {
        r.p = 4;
        extended = true;
        put(dlg, "extended", vbool(true));
        int64_t help = r.u32(), ex = r.u32();
        style = r.u32();
        put(dlg, "help", vint(help));
        put(dlg, "exstyle", vint(ex));
        put(dlg, "style", vint(style));
        count = r.u16();
    } else {
        style = r.u32();
        put(dlg, "style", vint(style));
        if (wide) put(dlg, "exstyle", vint(r.u32()));
        count = wide ? r.u16() : r.u8();
    }
    int64_t x = r.i16(), y = r.i16(), cx = r.i16(), cy = r.i16();
    put(dlg, "x", vint(x));
    put(dlg, "y", vint(y));
    put(dlg, "cx", vint(cx));
    put(dlg, "cy", vint(cy));
    Value menu = r.text_or_ordinal();
    Value cls = r.text_or_ordinal();
    Value title = vstr(r.text());
    put(dlg, "menu", menu);
    put(dlg, "class", cls);
    put(dlg, "title", title);
    put(dlg, "font", vnone());
    if (style & 0x40) {                                  // DS_SETFONT
        if (extended) {
            int64_t points = r.u16(), weight = r.u16();
            int64_t italic = r.u8(), charset = r.u8();
            put(dlg, "font", vtuple_of({vint(points), vint(weight), vint(italic), vint(charset), vstr(r.text())}));
        } else {
            int64_t points = r.u16();
            put(dlg, "font", vtuple_of({vint(points), vstr(r.text())}));
        }
    }
    Value controls = vlist();
    for (int64_t k = 0; k < count; ++k) {
        Value c = vdict();
        put(c, "help", vint(0));
        put(c, "exstyle", vint(0));
        if (wide) r.align();
        auto rect = [&] {
            int64_t a = r.i16(), b = r.i16(), cc = r.i16(), dd = r.i16();
            put(c, "x", vint(a));
            put(c, "y", vint(b));
            put(c, "cx", vint(cc));
            put(c, "cy", vint(dd));
        };
        if (extended) {
            int64_t help = r.u32(), ex = r.u32(), st = r.u32();
            put(c, "help", vint(help));
            put(c, "exstyle", vint(ex));
            put(c, "style", vint(st));
            rect();
            put(c, "id", vint(r.u32()));
        } else if (wide) {
            int64_t st = r.u32(), ex = r.u32();
            put(c, "style", vint(st));
            put(c, "exstyle", vint(ex));
            rect();
            put(c, "id", vint(r.u16()));
        } else {
            rect();
            int64_t id = r.u16(), st = r.u32();
            put(c, "id", vint(id));
            put(c, "style", vint(st));
        }
        if (wide) put(c, "class", r.text_or_ordinal());
        else put(c, "class", (data.at(r.p) & 0x80) ? vint(r.u8()) : vstr(r.text()));
        put(c, "text", r.text_or_ordinal());
        int64_t n = wide ? r.u16() : r.u8();
        put(c, "extra", vbytes(r.take(n)));
        push(controls, std::move(c));
    }
    put(dlg, "controls", std::move(controls));
    return dlg;
}

// ---- menu ----------------------------------------------------------------------------------
Value menu_level_ext(Reader &r) {
    Value items = vlist();
    while (true) {
        r.align();
        Value item = vdict();
        int64_t type = r.u32(), state = r.u32(), id = r.u32();
        put(item, "type", vint(type));
        put(item, "state", vint(state));
        put(item, "id", vint(id));
        put(item, "help", vint(0));
        put(item, "items", vnone());
        int64_t kind = r.u16();
        put(item, "text", vstr(r.text()));
        if (kind & 0x01) {                               // opens a submenu
            r.align();
            put(item, "help", vint(r.u32()));
            put(item, "items", menu_level_ext(r));
        }
        push(items, std::move(item));
        if (kind & 0x80) return items;
    }
}
Value menu_level(Reader &r) {
    Value items = vlist();
    while (true) {
        int64_t flags = r.u16();
        Value item = vdict();
        put(item, "flags", vint(flags & ~0x80));
        put(item, "id", vint(0));
        put(item, "items", vnone());
        if (!(flags & 0x10)) put(item, "id", vint(r.u16()));
        put(item, "text", vstr(r.text()));
        if (flags & 0x10) put(item, "items", menu_level(r));
        push(items, std::move(item));
        if (flags & 0x80) return items;
    }
}
Value menu(Bytes data, bool wide) {
    Reader r(data, wide);
    int64_t version = r.u16(), offset = r.u16();
    Value out = vdict();
    if (version == 1 && wide) {
        put(out, "extended", vbool(true));
        put(out, "help", vint(r.u32()));
        r.p = 4 + offset;
        put(out, "items", menu_level_ext(r));
    } else if (version == 0) {
        put(out, "extended", vbool(false));
        put(out, "help", vint(0));
        r.p = 4 + offset;
        put(out, "items", menu_level(r));
    } else {
        raise("ValueError", "menu template version " + dec(version));
    }
    return out;
}

// ---- accelerators --------------------------------------------------------------------------
Value accelerators(Bytes data, bool wide) {
    int64_t size = wide ? 8 : 5;
    Value out = vlist();
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
        push(out, vtuple_of({vint(flags & 0x7F), vint(key), vint(ident)}));
        if (flags & 0x80) break;
    }
    return out;
}

// ---- version -------------------------------------------------------------------------------
struct VNode {
    std::string key;
    Bytes value;
    bool text;
    std::vector<VNode> kids;
    int64_t end;
};

VNode vnode(Bytes data, bool wide, int64_t p, int64_t limit) {
    int64_t length, vlen, kind;
    int64_t at;
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
    if (length < 4) raise("ValueError", "version block length");
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

std::string text_of(Bytes value, bool wide) {
    std::string s = wide ? utf16le(value) : latin1(value);
    size_t z = s.find('\0');
    return z == std::string::npos ? s : s.substr(0, z);
}

Value version(Bytes data, bool wide) {
    VNode top = vnode(data, wide, 0, data.size());
    if (top.key != "VS_VERSION_INFO") raise("ValueError", "not a version block");
    Value fixed = vnone();
    if (top.value.size() >= 52) {
        fixed = vtuple();
        for (int k = 0; k < 13; ++k) push(fixed, vint(top.value.u32(4 * k)));
    }
    Value table = vdict(), translations = vlist();
    for (const VNode &k : top.kids) {
        if (k.key == "StringFileInfo") {
            for (const VNode &lang : k.kids) {
                Value entries = vlist();
                for (const VNode &e : lang.kids) push(entries, vtuple_of({vstr(e.key), vstr(text_of(e.value, wide))}));
                put(table, lang.key.c_str(), std::move(entries));
            }
        } else if (k.key == "VarFileInfo") {
            for (const VNode &v : k.kids)
                for (int64_t i = 0; i + 1 < v.value.size(); i += 2) push(translations, vint(v.value.u16(i)));
        } else {
            raise("ValueError", "a version block with a '" + k.key + "' part");
        }
    }
    return vtuple_of({fixed, table, translations});
}

// ---- icon / cursor groups, bitmaps ---------------------------------------------------------
struct Entry {
    int64_t w, h, colours, planes, bits, size, ident;
};
int64_t group_entries(Bytes data, std::vector<Entry> &out) {
    int64_t kind = data.u16(2), count = data.u16(4);
    data.u16(0);
    if ((kind != 1 && kind != 2) || 6 + 14 * count > data.size()) raise("ValueError", "not an icon or cursor group");
    for (int64_t i = 0; i < count; ++i) {
        int64_t p = 6 + 14 * i;
        Entry e;
        if (kind == 1) {
            e = {data.u8(p), data.u8(p + 1), data.u8(p + 2), data.u16(p + 4), data.u16(p + 6), data.u32(p + 8), data.u16(p + 12)};
        } else {
            e = {data.u16(p), data.u16(p + 2), 0, data.u16(p + 4), data.u16(p + 6), data.u32(p + 8), data.u16(p + 12)};
        }
        out.push_back(e);
    }
    return kind;
}

void group(Call &c) {
    std::vector<Entry> es;
    int64_t kind = group_entries(c.data, es);
    Value list = vlist();
    for (auto &e : es)
        push(list, vtuple_of({vint(e.w), vint(e.h), vint(e.colours), vint(e.planes), vint(e.bits), vint(e.size), vint(e.ident)}));
    emit(c.out, vtuple_of({vint(kind), list}));
}

void le16(std::string &s, uint64_t v) { s += char(v); s += char(v >> 8); }
void le32(std::string &s, uint64_t v) { le16(s, v & 0xFFFF); le16(s, (v >> 16) & 0xFFFF); }
void packI(std::string &s, int64_t v) {
    if (v < 0 || v > 0xFFFFFFFFLL) raise("struct.error", "'I' format requires 0 <= number <= 4294967295");
    le32(s, v);
}
void packB(std::string &s, int64_t v) {
    if (v < 0 || v > 255) raise("struct.error", "ubyte format requires 0 <= number <= 255");
    s += char(v);
}
void packH(std::string &s, int64_t v) {
    if (v < 0 || v > 65535) raise("struct.error", "ushort format requires 0 <= number <= 65535");
    le16(s, v);
}

// resources.icon_file(group_data, [member bytes ...]) -> the file
void icon_file(Call &c) {
    std::vector<Entry> es;
    int64_t kind = group_entries(c.data, es);
    const Value &members = c.arg(0);
    std::string head, table, body;
    le16(head, 0);
    le16(head, kind);
    le16(head, es.size());
    int64_t at = 6 + 16 * static_cast<int64_t>(es.size());
    for (size_t i = 0; i < es.size(); ++i) {
        const Entry &e = es[i];
        Bytes image = members[i].as_bytes();
        if (kind == 2) {
            int64_t hx = image.u16(0), hy = image.u16(2);
            image = image.slice(4);
            packB(table, e.w & 0xFF);
            packB(table, (e.h / 2) & 0xFF);
            packB(table, 0);
            packB(table, 0);
            packH(table, hx);
            packH(table, hy);
        } else {
            packB(table, e.w);
            packB(table, e.h);
            packB(table, e.colours);
            packB(table, 0);
            packH(table, e.planes);
            packH(table, e.bits);
        }
        packI(table, image.size());
        packI(table, at);
        body += image.str();
        at += image.size();
    }
    c.out.bytes(Bytes(head + table + body));
}

void bitmap_file(Call &c) {
    Bytes data = c.data;
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
    c.out.bytes(Bytes(s + data.str()));
}

// ---- type libraries ------------------------------------------------------------------------
const char *kTypeKinds[] = {"enum", "struct", "module", "interface", "dispinterface", "coclass", "typedef", "union"};

void type_library(Call &c) {
    Bytes data = c.data;
    Bytes tag = data.slice(0, 4);
    std::string kind = tag.is("MSFT", 4) ? "MSFT" : tag.is("SLTG", 4) ? "SLTG" : "";
    if (kind.empty()) {
        c.out.none();
        return;
    }
    Value out = vdict();
    put(out, "format", vstr(kind));
    put(out, "name", vstr(""));
    put(out, "guid", vstr(""));
    Value types = vlist();
    put(out, "types", types);
    if (kind == "SLTG" || data.size() < 0x54) {
        emit(c.out, out);
        return;
    }
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
            Bytes b = data.slice(guid_at + at, guid_at + at + 16);
            uint32_t a = b.u32(0);
            uint32_t cc = b.u16(4), e = b.u16(6);
            auto pad = [](std::string s, size_t n) { return std::string(n > s.size() ? n - s.size() : 0, '0') + s; };
            auto hx = [&](int64_t from, int64_t to) {
                std::string s;
                Bytes x = b.slice(from, to);
                for (const uint8_t v : x) s += pad(hex(v, true), 2);
                return s;
            };
            return pad(hex(a, true), 8) + "-" + pad(hex(cc, true), 4) + "-" + pad(hex(e, true), 4) + "-" + hx(8, 10) + "-" + hx(10, 16);
        };
        auto name = [&](int64_t at) -> std::string {
            if (at < 0) return "";
            int64_t length = data.at(name_at + at + 8);
            return latin1(data.slice(name_at + at + 12, name_at + at + 12 + length));
        };
        std::string nm = name(own_name), gd = guid(own_guid);
        put(out, "name", vstr(nm));
        put(out, "guid", vstr(gd));
        for (int64_t n = 0; n < count; ++n) {
            int64_t at = info_at + 0x64 * n;
            int64_t k = data.u32(at) & 0xF;
            std::string kn = k < 8 ? kTypeKinds[k] : "type " + dec(k);
            std::string tn = name(data.i32(at + 0x34));
            std::string tg = guid(data.i32(at + 0x2C));
            Value *t = lookup(out, vstr("types"));
            push(*t, vtuple_of({vstr(kn), vstr(tn), vstr(tg)}));
        }
    } catch (const PyError &e) {
        if (e.kind != "struct.error" && e.kind != "IndexError") throw;
    }
    emit(c.out, out);
}

// resources.read_ne(data, ne_off, rsrc_tab, resname_tab) -> [(type, name, flags, bytes)]
void read_ne(Call &c) {
    Bytes d = c.data;
    int64_t ne_off = c.int_arg(0), rsrc = c.int_arg(1), resname = c.int_arg(2);
    Value out = vlist();
    if (rsrc == resname) {
        emit(c.out, out);
        return;
    }
    int64_t base = ne_off + rsrc;
    auto ident = [&](int64_t v) -> Value {
        if (v & 0x8000) return vint(v & 0x7FFF);
        int64_t n = d.at(base + v);
        return vstr(latin1(d.slice(base + v + 1, base + v + 1 + n)));
    };
    int64_t shift = d.u16(base);
    if (shift > 15) {
        emit(c.out, out);
        return;
    }
    int64_t p = base + 2;
    try {
        while (p + 8 <= d.size()) {
            int64_t tid = d.u16(p), count = d.u16(p + 2);
            if (tid == 0) break;
            p += 8;
            for (int64_t k = 0; k < count; ++k) {
                if (p + 12 > d.size()) {
                    emit(c.out, out);
                    return;
                }
                int64_t off = d.u16(p) << shift, length = d.u16(p + 2) << shift, flags = d.u16(p + 4), rid = d.u16(p + 6);
                Value t = ident(tid), n = ident(rid);
                push(out, vtuple_of({t, n, vint(flags), vbytes(d.slice(off, off + length))}));
                p += 12;
            }
        }
    } catch (const PyError &) {
        throw;
    }
    emit(c.out, out);
}

template <Value (*F)(Bytes, bool)>
void dec_q(Call &c) {
    emit(c.out, F(c.data, c.bool_arg(0)));
}

}  // namespace

CBF_QUESTIONS(resources) {
    add("resources.strings", dec_q<strings>);
    add("resources.dialog", dec_q<dialog>);
    add("resources.menu", dec_q<menu>);
    add("resources.accelerators", dec_q<accelerators>);
    add("resources.version", dec_q<version>);
    add("resources.group", group, "(int, list[(int, int, int, int, int, int, int)])");
    add("resources.icon_file", icon_file, "bytes");
    add("resources.bitmap_file", bitmap_file, "bytes");
    add("resources.type_library", type_library);
    add("resources.read_ne", read_ne, "list[(int|str, int|str, int, bytes)]");
}

}  // namespace cbf::resources
