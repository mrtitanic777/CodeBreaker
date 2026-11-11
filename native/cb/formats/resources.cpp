#include "cb/formats/resources.hpp"

#include <algorithm>
#include <format>

#include "cb/core/error.hpp"
#include "cb/core/io.hpp"
#include "cb/core/text.hpp"

namespace cb::formats::resources {

namespace {

constexpr std::string_view kReader = "resources";

[[noreturn]] void bad(std::string what, std::uint64_t offset = 0) { throw FormatError(kReader, offset, std::move(what)); }

// The fields of a template in order: packed for a 16-bit module, aligned to four bytes where the format says so for a
// 32-bit one (UTF-16 text).
class TemplateReader {
public:
    TemplateReader(ByteView data, bool wide, std::size_t at = 0) : r_(data, kReader), wide_(wide) { r_.seek(at); }

    std::uint8_t u8() { return r_.u8(); }
    std::uint16_t u16() { return r_.u16(); }
    std::int16_t i16() { return r_.i16(); }
    std::uint32_t u32() { return r_.u32(); }
    ByteView take(std::size_t n) {
        const auto v = r_.try_bytes(n);
        if (!v) bad("resource body ends early", r_.pos());
        return *v;
    }
    void align(std::size_t n = 4) { r_.align(n); }
    std::size_t pos() const { return r_.pos(); }
    void seek(std::size_t at) { r_.seek(at); }
    bool at_end() const { return r_.eof(); }
    std::optional<std::uint8_t> peek_u8() const { return r_.peek_u8(); }

    // text up to its terminator (UTF-16 for a 32-bit module, single-byte for a 16-bit one), the terminator consumed
    std::string text() {
        if (wide_) {
            const ByteView d = r_.data();
            std::size_t e = r_.pos();
            while (e + 2 <= d.size() && !(d[e] == 0 && d[e + 1] == 0)) e += 2;
            if (e + 2 > d.size()) bad("unterminated text", r_.pos());
            std::string s = text::from_utf16le(d.subspan(r_.pos(), e - r_.pos()));
            r_.seek(e + 2);
            return s;
        }
        return text::from_latin1(r_.cstr());
    }
    // 0xFFFF (0xFF in a 16-bit module) and an ordinal, or text
    Ident text_or_ordinal() {
        if (wide_) {
            if (const auto marker = r_.peek_u16(); marker && *marker == 0xFFFF) {
                r_.skip(2);
                return Ident(std::uint32_t{r_.u16()});
            }
        } else if (const auto marker = r_.peek_u8(); marker && *marker == 0xFF) {
            r_.skip(1);
            return Ident(std::uint32_t{r_.u16()});
        }
        return Ident(text());
    }

private:
    io::ByteReader r_;
    bool wide_;
};

std::string utf16_text(ByteView b) { return text::from_utf16le(b); }

}  // namespace

// ---- the names of things -------------------------------------------------------------------------------

std::string type_name(const Ident& type) {
    if (const std::string* s = std::get_if<std::string>(&type)) return *s;
    const std::uint32_t n = std::get<std::uint32_t>(type);
    switch (n) {
        case 1: return "CURSOR";
        case 2: return "BITMAP";
        case 3: return "ICON";
        case 4: return "MENU";
        case 5: return "DIALOG";
        case 6: return "STRING";
        case 7: return "FONTDIR";
        case 8: return "FONT";
        case 9: return "ACCELERATOR";
        case 10: return "RCDATA";
        case 11: return "MESSAGETABLE";
        case 12: return "GROUP_CURSOR";
        case 14: return "GROUP_ICON";
        case 16: return "VERSION";
        case 17: return "DLGINCLUDE";
        case 19: return "PLUGPLAY";
        case 20: return "VXD";
        case 21: return "ANICURSOR";
        case 22: return "ANIICON";
        case 23: return "HTML";
        case 24: return "MANIFEST";
        default: return std::format("#{}", n);
    }
}

std::optional<std::uint32_t> member_type_of(std::uint32_t group_type) {
    if (group_type == kGroupIcon) return kIcon;
    if (group_type == kGroupCursor) return kCursor;
    return std::nullopt;
}

std::optional<std::string_view> control_class_name(std::uint32_t atom) {
    switch (atom) {
        case 0x80: return "BUTTON";
        case 0x81: return "EDIT";
        case 0x82: return "STATIC";
        case 0x83: return "LISTBOX";
        case 0x84: return "SCROLLBAR";
        case 0x85: return "COMBOBOX";
        default: return std::nullopt;
    }
}

std::string Resource::describe() const {
    auto ident = [](const Ident& i) {
        return std::holds_alternative<std::uint32_t>(i) ? std::to_string(std::get<std::uint32_t>(i)) : "'" + std::get<std::string>(i) + "'";
    };
    return std::format("<{} {} lang={} {} bytes>", type_name(type), ident(name), language ? std::to_string(*language) : "None", data.size());
}

// ---- reading the tables ------------------------------------------------------------------------------------

ExecutableKind kind_of(ByteView file) {
    if (file.size() < 0x40 || file[0] != 'M' || file[1] != 'Z') return ExecutableKind::Other;
    io::ByteReader r(file, kReader);
    r.seek(0x3C);
    const std::uint64_t at = r.u32();
    const auto sig = io::try_range(file, at, 2);
    if (!sig) return ExecutableKind::Other;
    if ((*sig)[0] == 'P' && (*sig)[1] == 'E') return ExecutableKind::Pe;
    if ((*sig)[0] == 'N' && (*sig)[1] == 'E') return ExecutableKind::Ne;
    return ExecutableKind::Other;
}

std::vector<Resource> read_ne(ByteView d) {
    if (kind_of(d) != ExecutableKind::Ne) bad("not an NE module");
    io::ByteReader head(d, kReader);
    head.seek(0x3C);
    const std::uint64_t ne_off = head.u32();
    head.seek(ne_off + 0x24);
    const std::uint16_t rsrc = head.u16();
    const std::uint16_t resname = head.u16();

    std::vector<Resource> out;
    if (rsrc == resname) return out;   // no resource table
    const std::uint64_t base = ne_off + rsrc;
    auto ident = [&](std::uint16_t v) -> Ident {
        if (v & 0x8000) return Ident(std::uint32_t{static_cast<std::uint16_t>(v & 0x7FFF)});
        io::ByteReader r(d, kReader);
        r.seek(base + v);
        const std::uint8_t n = r.u8();
        return Ident(text::from_latin1(r.bytes(n)));
    };
    io::ByteReader r(d, kReader);
    r.seek(base);
    const unsigned shift = r.u16();   // the alignment shift: lengths and offsets are in units of 2^shift
    if (shift > 15) return out;
    while (r.remaining() >= 8) {
        const std::uint16_t type_id = r.u16();
        const std::uint16_t count = r.u16();
        if (type_id == 0) break;
        r.skip(4);
        for (std::uint16_t k = 0; k < count; ++k) {
            if (r.remaining() < 12) return out;
            const std::uint64_t offset = std::uint64_t{r.u16()} << shift;
            const std::uint64_t length = std::uint64_t{r.u16()} << shift;
            const std::uint16_t flags = r.u16();
            const std::uint16_t id = r.u16();
            Resource res;
            res.type = ident(type_id);
            res.name = ident(id);
            res.flags = flags;
            // A resource that runs past the end of the file gets what is there (an inspector's leniency, as the old
            // reader's slices gave).
            const std::uint64_t start = std::min<std::uint64_t>(offset, d.size());
            const std::uint64_t stop = std::min<std::uint64_t>(offset + length, d.size());
            res.data.assign(d.begin() + static_cast<std::ptrdiff_t>(start), d.begin() + static_cast<std::ptrdiff_t>(stop));
            out.push_back(std::move(res));
            r.skip(4);   // the resource's handle and usage words
        }
    }
    return out;
}

std::vector<Resource> from_pe_entries(ByteView image, std::span<const PeResourceEntry> entries) {
    std::vector<Resource> out;
    for (const PeResourceEntry& e : entries) {
        if (!e.file_offset) continue;
        const auto data = io::try_range(image, *e.file_offset, e.size);
        if (!data) continue;
        Resource res;
        res.type = e.type;
        res.name = e.name;
        res.language = e.language;
        res.data.assign(data->begin(), data->end());
        out.push_back(std::move(res));
    }
    return out;
}

bool same_data(ByteView a, ByteView b) {
    const std::size_t n = std::min(a.size(), b.size());
    if (!std::equal(a.begin(), a.begin() + static_cast<std::ptrdiff_t>(n), b.begin())) return false;
    auto zeros = [](ByteView v) { return std::all_of(v.begin(), v.end(), [](std::uint8_t c) { return c == 0; }); };
    return zeros(a.subspan(n)) && zeros(b.subspan(n));
}

// ---- strings -------------------------------------------------------------------------------------------

std::vector<std::string> strings(ByteView data, bool wide) {
    TemplateReader r(data, wide);
    std::vector<std::string> out;
    for (int k = 0; k < 16; ++k) {
        if (r.at_end()) {
            out.emplace_back();
            continue;
        }
        if (wide) {
            const std::size_t n = r.u16();
            out.push_back(utf16_text(r.take(2 * n)));
        } else {
            const std::size_t n = r.u8();
            out.push_back(text::from_latin1(r.take(n)));
        }
    }
    const ByteView rest = data.subspan(r.pos());
    if (std::any_of(rest.begin(), rest.end(), [](std::uint8_t b) { return b != 0; })) bad("data after the sixteenth string", r.pos());
    return out;
}

// ---- dialog --------------------------------------------------------------------------------------------

Dialog dialog(ByteView data, bool wide) {
    TemplateReader r(data, wide);
    Dialog dlg;
    std::size_t count;
    const bool extended = wide && data.size() >= 4 && data[0] == 0x01 && data[1] == 0x00 && data[2] == 0xFF && data[3] == 0xFF;
    if (extended) {
        r.seek(4);
        dlg.extended = true;
        dlg.help = r.u32();
        dlg.exstyle = r.u32();
        dlg.style = r.u32();
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
    dlg.window_class = r.text_or_ordinal();
    dlg.title = r.text();
    if (dlg.style & 0x40) {   // DS_SETFONT
        DialogFont font;
        font.points = r.u16();
        if (extended) {
            font.weight = r.u16();
            font.italic = r.u8();
            font.charset = r.u8();
        }
        font.face = r.text();
        dlg.font = std::move(font);
    }
    for (std::size_t k = 0; k < count; ++k) {
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
        if (wide) {
            c.window_class = r.text_or_ordinal();
        } else {
            const auto atom = r.peek_u8();
            if (atom && (*atom & 0x80)) c.window_class = Ident(std::uint32_t{r.u8()});
            else c.window_class = Ident(r.text());
        }
        c.text = r.text_or_ordinal();
        const std::size_t n = wide ? r.u16() : r.u8();
        const ByteView extra = r.take(n);
        c.extra.assign(extra.begin(), extra.end());
        dlg.controls.push_back(std::move(c));
    }
    return dlg;
}

// ---- menu ----------------------------------------------------------------------------------------------

namespace {

constexpr int kMaxMenuDepth = 32;

std::vector<ExtMenuItem> menu_level_ext(TemplateReader& r, int depth) {
    if (depth > kMaxMenuDepth) bad("menus nested too deeply", r.pos());
    std::vector<ExtMenuItem> items;
    while (true) {
        r.align();
        ExtMenuItem item;
        item.type = r.u32();
        item.state = r.u32();
        item.id = r.u32();
        const std::uint16_t kind = r.u16();
        item.text = r.text();
        if (kind & 0x01) {   // opens a submenu
            r.align();
            item.help = r.u32();
            item.items = menu_level_ext(r, depth + 1);
        }
        items.push_back(std::move(item));
        if (kind & 0x80) return items;
    }
}

std::vector<MenuItem> menu_level(TemplateReader& r, int depth) {
    if (depth > kMaxMenuDepth) bad("menus nested too deeply", r.pos());
    std::vector<MenuItem> items;
    while (true) {
        const std::uint16_t flags = r.u16();
        MenuItem item;
        item.flags = flags & ~0x80;
        if (!(flags & 0x10)) item.id = r.u16();
        item.text = r.text();
        if (flags & 0x10) item.items = menu_level(r, depth + 1);
        items.push_back(std::move(item));
        if (flags & 0x80) return items;
    }
}

}  // namespace

Menu menu(ByteView data, bool wide) {
    TemplateReader r(data, wide);
    const std::uint16_t version = r.u16();
    const std::uint16_t offset = r.u16();
    Menu out;
    if (version == 1 && wide) {
        out.extended = true;
        out.help = r.u32();
        r.seek(std::size_t{4} + offset);
        out.items = menu_level_ext(r, 0);
    } else if (version == 0) {
        r.seek(std::size_t{4} + offset);
        out.items = menu_level(r, 0);
    } else {
        bad(std::format("menu template version {}", version));
    }
    return out;
}

// ---- accelerators -----------------------------------------------------------------------------------------

std::vector<Accelerator> accelerators(ByteView data, bool wide) {
    const std::size_t entry = wide ? 8 : 5;
    std::vector<Accelerator> out;
    for (std::size_t p = 0; p + entry <= data.size(); p += entry) {
        io::ByteReader r(data, kReader);
        r.seek(p);
        Accelerator a;
        std::uint16_t flags;
        if (wide) {
            flags = r.u16();
            a.key = r.u16();
            a.id = r.u16();
        } else {
            flags = r.u8();
            a.key = r.u16();
            a.id = r.u16();
        }
        a.flags = flags & 0x7F;
        out.push_back(a);
        if (flags & 0x80) break;   // the last entry
    }
    return out;
}

// ---- version ------------------------------------------------------------------------------------------------

namespace {

constexpr int kMaxVersionDepth = 8;

struct VersionNode {
    std::string key;
    Bytes value;
    std::vector<VersionNode> kids;
    std::size_t end = 0;
};

std::uint16_t u16_at(ByteView d, std::size_t p) {
    io::ByteReader r(d, kReader);
    r.seek(p);
    return r.u16();
}

VersionNode version_node(ByteView data, bool wide, std::size_t p, std::size_t limit, int depth) {
    if (depth > kMaxVersionDepth) bad("version blocks nested too deeply", p);
    const std::size_t length = u16_at(data, p);
    std::size_t value_length = u16_at(data, p + 2);
    const unsigned kind = wide ? u16_at(data, p + 4) : 0;
    if (length < 4) bad("version block length", p);
    const std::size_t end = std::min(p + length, limit);
    TemplateReader r(data, wide, p + (wide ? 6 : 4));
    VersionNode n;
    n.key = r.text();
    r.align();
    if (wide && kind == 1 && r.pos() + 2 * value_length <= end) value_length *= 2;   // counted in characters
    const std::size_t value_at = std::min(r.pos(), data.size());
    const std::size_t value_end = std::max(value_at, std::min(r.pos() + value_length, end));
    n.value.assign(data.begin() + static_cast<std::ptrdiff_t>(value_at), data.begin() + static_cast<std::ptrdiff_t>(std::min(value_end, data.size())));
    std::size_t q = (r.pos() + value_length + 3) & ~std::size_t{3};
    while (q + 4 <= end) {
        VersionNode k = version_node(data, wide, q, end, depth + 1);
        q = (k.end + 3) & ~std::size_t{3};
        n.kids.push_back(std::move(k));
    }
    n.end = end;
    return n;
}

// the text of a value, up to its first NUL
std::string value_text(const Bytes& value, bool wide) {
    const std::string s = wide ? text::from_utf16le(value) : text::from_latin1(value);
    return s.substr(0, s.find('\0'));
}

std::uint32_t u32_le_at(ByteView d, std::size_t p) {
    io::ByteReader r(d, kReader);
    r.seek(p);
    return r.u32();
}

}  // namespace

Version version(ByteView data, bool wide) {
    const VersionNode top = version_node(data, wide, 0, data.size(), 0);
    if (top.key != "VS_VERSION_INFO") bad("not a version block");
    Version out;
    if (top.value.size() >= 52) {
        io::ByteReader r(top.value, kReader);
        FixedFileInfo f;
        f.signature = r.u32();
        f.struct_version = r.u32();
        f.file_version_ms = r.u32();
        f.file_version_ls = r.u32();
        f.product_version_ms = r.u32();
        f.product_version_ls = r.u32();
        f.file_flags_mask = r.u32();
        f.file_flags = r.u32();
        f.os = r.u32();
        f.type = r.u32();
        f.subtype = r.u32();
        f.date_ms = r.u32();
        f.date_ls = r.u32();
        out.fixed = f;
    }
    for (const VersionNode& k : top.kids) {
        if (k.key == "StringFileInfo") {
            for (const VersionNode& lang : k.kids) {
                VersionStringTable table;
                table.language_block = lang.key;
                for (const VersionNode& e : lang.kids) table.entries.emplace_back(e.key, value_text(e.value, wide));
                out.strings.push_back(std::move(table));
            }
        } else if (k.key == "VarFileInfo") {
            for (const VersionNode& v : k.kids) {
                for (std::size_t i = 0; i + 1 < v.value.size(); i += 2) out.translations.push_back(static_cast<std::uint16_t>(v.value[i] | (v.value[i + 1] << 8)));
            }
        } else {
            bad("a version block with a '" + k.key + "' part");
        }
    }
    return out;
}

// ---- icon / cursor groups, bitmaps ------------------------------------------------------------------

Group group(ByteView data) {
    io::ByteReader r(data, kReader);
    (void)r.u16();   // reserved
    const std::uint16_t kind = r.u16();
    const std::uint16_t count = r.u16();
    if ((kind != 1 && kind != 2) || 6 + 14 * std::size_t{count} > data.size()) bad("not an icon or cursor group");
    Group g;
    g.kind = static_cast<GroupKind>(kind);
    for (std::uint16_t i = 0; i < count; ++i) {
        GroupEntry e;
        if (g.kind == GroupKind::Icons) {
            e.width = r.u8();
            e.height = r.u8();
            e.colours = r.u8();
            r.skip(1);
        } else {
            e.width = r.u16();
            e.height = r.u16();
        }
        e.planes = r.u16();
        e.bits = r.u16();
        e.bytes_in_resource = r.u32();
        e.member_id = r.u16();
        g.entries.push_back(e);
    }
    return g;
}

Bytes icon_file(ByteView group_data, const std::function<Bytes(std::uint16_t)>& member_data) {
    const Group g = group(group_data);
    io::ByteWriter table, body;
    std::uint64_t at = 6 + 16 * g.entries.size();
    for (const GroupEntry& e : g.entries) {
        Bytes member = member_data(e.member_id);
        ByteView image = member;
        if (g.kind == GroupKind::Cursors) {
            // a cursor member starts with its hot spot, which a .cur file keeps in the directory
            io::ByteReader hot(image, kReader);
            const std::uint16_t hx = hot.u16();
            const std::uint16_t hy = hot.u16();
            image = image.subspan(4);
            table.u8(static_cast<std::uint8_t>(e.width & 0xFF));
            table.u8(static_cast<std::uint8_t>((e.height / 2) & 0xFF));   // the cursor's height counts its mask too
            table.u8(0);
            table.u8(0);
            table.u16(hx);
            table.u16(hy);
        } else {
            table.u8(static_cast<std::uint8_t>(e.width));
            table.u8(static_cast<std::uint8_t>(e.height));
            table.u8(e.colours);
            table.u8(0);
            table.u16(e.planes);
            table.u16(e.bits);
        }
        if (image.size() > 0xFFFFFFFFull || at > 0xFFFFFFFFull) bad("an icon file past 4 GB");
        table.u32(static_cast<std::uint32_t>(image.size()));
        table.u32(static_cast<std::uint32_t>(at));
        body.bytes(image);
        at += image.size();
    }
    io::ByteWriter out;
    out.u16(0);
    out.u16(static_cast<std::uint16_t>(g.kind));
    out.u16(static_cast<std::uint16_t>(g.entries.size()));
    out.bytes(table.data());
    out.bytes(body.data());
    return out.take();
}

Bytes bitmap_file(ByteView data) {
    const std::uint64_t size = data.size() >= 4 ? u32_le_at(data, 0) : 0;
    std::uint64_t palette = 0;
    if (size == 12 && data.size() >= 12) {   // an OS/2-style core header: 3 bytes a colour
        const unsigned bits = u16_at(data, 10);
        palette = bits <= 8 ? 3ull << bits : 0;
    } else if (size >= 40 && data.size() >= 40) {
        const unsigned bits = u16_at(data, 14);
        const std::uint32_t compression = u32_le_at(data, 16);
        const std::uint32_t used = u32_le_at(data, 32);
        palette = 4ull * (used ? used : (bits <= 8 ? 1ull << bits : 0)) + (compression == 3 && size == 40 ? 12 : 0);
    }
    const std::uint64_t pixels_at = 14 + size + palette;
    if (pixels_at > 0xFFFFFFFFull || 14 + data.size() > 0xFFFFFFFFull) bad("a bitmap past 4 GB");
    io::ByteWriter out;
    out.bytes(as_bytes("BM"));
    out.u32(static_cast<std::uint32_t>(14 + data.size()));
    out.u16(0);
    out.u16(0);
    out.u32(static_cast<std::uint32_t>(pixels_at));
    out.bytes(data);
    return out.take();
}

// ---- type libraries -----------------------------------------------------------------------------------------

std::optional<TypeLibrary> type_library(ByteView data) {
    const bool msft = data.size() >= 4 && as_text(data.first(4)) == "MSFT";
    const bool sltg = data.size() >= 4 && as_text(data.first(4)) == "SLTG";
    if (!msft && !sltg) return std::nullopt;
    TypeLibrary out;
    out.format = msft ? "MSFT" : "SLTG";
    if (sltg || data.size() < 0x54) return out;

    // An inspector's reading: a library that is cut short gives what was read before the cut.
    try {
        io::ByteReader r(data, kReader);
        auto i32_at = [&](std::size_t at) {
            r.seek(at);
            return r.i32();
        };
        const std::int64_t own_guid = i32_at(8);
        r.seek(0x14);
        const std::uint32_t flags = r.u32();
        r.seek(0x20);
        const std::uint32_t count = r.u32();
        const std::int64_t own_name = i32_at(0x38);
        const std::uint64_t directory = 0x54 + 4ull * count + ((flags & 0x100) ? 4 : 0);
        std::int64_t seg[15];
        for (unsigned i = 0; i < 15; ++i) {
            seg[i] = i32_at(directory + 16 * i);
            (void)i32_at(directory + 16 * i + 4);   // the segment's length must be there too
        }
        const std::int64_t info_at = seg[0], guid_at = seg[5], name_at = seg[7];
        auto guid = [&](std::int64_t at) -> std::string {
            if (at < 0) return "";
            const ByteView b = io::checked_range(data, static_cast<std::uint64_t>(guid_at + at), 16);
            io::ByteReader g(b, kReader);
            const std::uint32_t a = g.u32();
            const std::uint16_t c = g.u16();
            const std::uint16_t e = g.u16();
            return std::format("{:08X}-{:04X}-{:04X}-{:02X}{:02X}-{:02X}{:02X}{:02X}{:02X}{:02X}{:02X}", a, c, e, b[8], b[9], b[10], b[11], b[12],
                               b[13], b[14], b[15]);
        };
        auto name = [&](std::int64_t at) -> std::string {
            if (at < 0) return "";
            r.seek(static_cast<std::uint64_t>(name_at + at + 8));
            const std::size_t length = r.u8();
            const ByteView b = io::checked_range(data, static_cast<std::uint64_t>(name_at + at + 12), length);
            return text::from_latin1(b);
        };
        out.name = name(own_name);
        out.guid = guid(own_guid);
        static constexpr std::string_view kKinds[] = {"enum", "struct", "module", "interface", "dispinterface", "coclass", "typedef", "union"};
        for (std::uint32_t n = 0; n < count; ++n) {
            const std::uint64_t at = static_cast<std::uint64_t>(info_at) + 0x64ull * n;
            r.seek(at);
            const std::uint32_t k = r.u32() & 0xF;
            TypeLibraryType t;
            t.kind = k < 8 ? std::string(kKinds[k]) : std::format("type {}", k);
            t.name = name(i32_at(at + 0x34));
            t.guid = guid(i32_at(at + 0x2C));
            out.types.push_back(std::move(t));
        }
    } catch (const FormatError&) {
        // the cut: what was read stays
    }
    return out;
}

std::string type_library_outline(const TypeLibrary& library) {
    std::vector<std::string> out;
    out.push_back("// Outline of a type library (" + library.format + " layout). Not compilable: only what each type is called.");
    if (!library.guid.empty()) out.push_back("[ uuid(" + library.guid + ") ]");
    out.push_back("library " + (library.name.empty() ? std::string("unnamed") : library.name) + "\n{");
    for (const TypeLibraryType& t : library.types) {
        out.push_back("    " + (t.guid.empty() ? std::string() : "[ uuid(" + t.guid + ") ] ") + t.kind + " " +
                      (t.name.empty() ? std::string("/* no name */") : t.name) + ";");
    }
    out.push_back("}");
    out.push_back("");
    return text::join(out, "\n");
}

// ---- decoding by kind ------------------------------------------------------------------------------------------

std::optional<Decoded> decode(const Resource& resource, bool wide) {
    const std::uint32_t* type = std::get_if<std::uint32_t>(&resource.type);
    if (!type) return std::nullopt;
    try {   // one resource of many: one that does not decode is compared as bytes
        switch (*type) {
            case kString: return Decoded(strings(resource.data, wide));
            case kDialog: return Decoded(dialog(resource.data, wide));
            case kMenu: return Decoded(menu(resource.data, wide));
            case kAccelerator: return Decoded(accelerators(resource.data, wide));
            case kVersion: {
                Version v = version(resource.data, wide);
                if (v.fixed) v.fixed->date_ms = v.fixed->date_ls = 0;   // a compiler stamps it
                return Decoded(std::move(v));
            }
            default: return std::nullopt;
        }
    } catch (const FormatError&) {
        return std::nullopt;
    }
}

}  // namespace cb::formats::resources
