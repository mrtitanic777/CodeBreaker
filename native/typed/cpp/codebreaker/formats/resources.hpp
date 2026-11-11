// codebreaker/formats/resources.hpp -- compat shim over cb/formats/resources.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module itself is native/cb/formats/resources.{hpp,cpp}; this header keeps the old py:: surface (the records the
// callers read, `Ident` as variant<int64_t, py::str>, ...) for the ports that have not been refactored yet, and is deleted
// with the last of them. Conversion and forwarding only. `read()` / `read_pe()` still walk a PE file's resource directory
// through the old PE port (pe::PEFile::resource_tree) until cb::formats::pe lands; the NE table is read by the new module.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>
#include <variant>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/formats/resources.hpp"
#include "codebreaker/formats/ne.hpp"
#include "codebreaker/formats/pe.hpp"

namespace codebreaker::formats::resources {

inline constexpr int64_t CURSOR = 1, BITMAP = 2, ICON = 3, MENU = 4, DIALOG = 5, STRING = 6, FONTDIR = 7, FONT = 8,
                         ACCELERATOR = 9, RCDATA = 10;
inline constexpr int64_t MESSAGETABLE = 11, GROUP_CURSOR = 12, GROUP_ICON = 14, VERSION = 16;

// a resource type's name by number
const py::dict<int64_t, py::str>& TYPES();
// the member type of a group type
const py::dict<int64_t, int64_t>& MEMBER_OF();
// a dialog control's class atom by number
const py::dict<int64_t, py::str>& CONTROL_CLASSES();

// a type or a name: an ordinal, or text
using Ident = std::variant<int64_t, py::str>;

// One resource: `type` and `name` (int or str), `language` (None in a 16-bit module),
// `flags` (the 16-bit memory flags, else 0) and its `data`.
struct Resource {
    Ident type;
    Ident name;
    std::optional<int64_t> language;
    int64_t flags = 0;
    py::bytes data;

    Resource() = default;
    Resource(const Ident& type_, const Ident& name_, std::optional<int64_t> language_, int64_t flags_, const py::bytes& data_)
        : type(type_), name(name_), language(language_), flags(flags_), data(data_) {}

    std::tuple<Ident, Ident, std::optional<int64_t>> key() const { return std::make_tuple(type, name, language); }
    // <DIALOG 'name' lang=1033 120 bytes>
    py::str repr() const;
};

py::str type_name(const Ident& t);

// Every resource of a module, in the order the file lists them. [] when it has none.
py::list<Resource> read(const py::str& path);
// The resources of a 32-bit module (the directory itself is walked by PEFile.resource_tree).
py::list<Resource> read_pe(const pe::PEFile& pe);
// The resource table of a 16-bit module: per type, a list of (offset, length, flags, name).
py::list<Resource> read_ne(const ne::NEFile& nef);
// Two resource bodies are the same when one is the other plus zero padding (a 16-bit
// module stores lengths in alignment units).
bool same_data(const py::bytes& a, const py::bytes& b);

// A string-table block: its 16 strings (block n holds the ids (n-1)*16 .. (n-1)*16+15).
py::list<py::str> strings(const py::bytes& data, bool wide);

struct DialogControl {
    int64_t help = 0, exstyle = 0, x = 0, y = 0, cx = 0, cy = 0, style = 0, id = 0;
    Ident class_;       // `class`: a str or one of CONTROL_CLASSES' atoms
    Ident text;
    py::bytes extra;
};
using DialogFontPlain = std::tuple<int64_t, py::str>;                                   // (points, face)
using DialogFontExtended = std::tuple<int64_t, int64_t, int64_t, int64_t, py::str>;     // (points, weight, italic, charset, face)
// A dialog template.
struct Dialog {
    bool extended = false;
    int64_t style = 0, exstyle = 0, help = 0, x = 0, y = 0, cx = 0, cy = 0;
    Ident menu;
    Ident class_;       // `class`
    py::str title;
    std::optional<std::variant<DialogFontPlain, DialogFontExtended>> font;
    py::list<DialogControl> controls;
};
Dialog dialog(const py::bytes& data, bool wide);

// A plain menu's item.
struct MenuItem {
    py::str text;
    int64_t id = 0, flags = 0;
    std::optional<py::list<MenuItem>> items;     // None unless it opens a submenu
};
// An extended menu's item.
struct ExtMenuItem {
    py::str text;
    int64_t id = 0, type = 0, state = 0, help = 0;
    std::optional<py::list<ExtMenuItem>> items;
};
// A menu template.
struct Menu {
    bool extended = false;
    int64_t help = 0;
    std::variant<py::list<MenuItem>, py::list<ExtMenuItem>> items;
};
Menu menu(const py::bytes& data, bool wide);

// An accelerator table -> [(flags, key, id)]; the last entry's end mark is not in flags.
py::list<std::tuple<int64_t, int64_t, int64_t>> accelerators(const py::bytes& data, bool wide);

// A version block.
struct Version {
    std::optional<py::list<int64_t>> fixed;                                   // the numbers of the fixed part (or None)
    py::dict<py::str, py::list<std::tuple<py::str, py::str>>> strings;       // {language block: [(key, text)]}
    py::list<int64_t> translations;
};
Version version(const py::bytes& data, bool wide);

// An icon or cursor group -> (kind, [entry]): kind 1 = icons, 2 = cursors; an entry is
// (width, height, colours, planes, bits, bytes, member id).
using GroupEntry = std::tuple<int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t>;
std::tuple<int64_t, py::list<GroupEntry>> group(const py::bytes& data);

// The .ico (or .cur) file a group resource and its members came from. `member_data(id)`
// gives a member's bytes. A cursor member starts with its hot spot, which a .cur file keeps in
// the directory instead.
py::bytes icon_file(const py::bytes& group_data, const std::function<py::bytes(int64_t)>& member_data);

// The .bmp file of a bitmap resource (the resource is the file without its 14-byte header).
py::bytes bitmap_file(const py::bytes& data);

// What a type library says of itself.
struct TypeLibrary {
    py::str format, name, guid;
    py::list<std::tuple<py::str, py::str, py::str>> types;       // (kind, name, guid)
};
std::optional<TypeLibrary> type_library(const py::bytes& data);

// A type library's contents as an outline in interface-definition syntax: for reading, not
// for compiling (members and their types are not in it).
py::str type_library_outline(const TypeLibrary& library);

// A resource as a value that means the same for any two encodings of it, or None when its
// kind is not one this module decodes (it is then compared as bytes).
using Decoded = std::variant<std::monostate, py::list<py::str>, Dialog, Menu, py::list<std::tuple<int64_t, int64_t, int64_t>>, Version>;
Decoded decode(const Resource& resource, bool wide);

// ---- the bodies: all conversion --------------------------------------------------------------------------

namespace shim_ {

namespace nr = cb::formats::resources;

inline Ident to_old(const nr::Ident& i) {
    if (const auto* n = std::get_if<std::uint32_t>(&i)) return Ident(int64_t(*n));
    return Ident(cb::compat::to_py(std::get<std::string>(i)));
}
inline nr::Ident to_new(const Ident& i) {
    if (const auto* n = std::get_if<int64_t>(&i)) return nr::Ident(cb::compat::int_from_py<std::uint32_t>(*n));
    return nr::Ident(cb::compat::from_py(std::get<py::str>(i)));
}

inline Resource to_old(const nr::Resource& r) {
    std::optional<int64_t> language;
    if (r.language) language = int64_t(*r.language);
    return Resource(to_old(r.type), to_old(r.name), language, r.flags, cb::compat::to_py(cb::ByteView(r.data)));
}
inline nr::Resource to_new(const Resource& r) {
    nr::Resource out;
    out.type = to_new(r.type);
    out.name = to_new(r.name);
    if (r.language) out.language = cb::compat::int_from_py<std::uint32_t>(*r.language);
    out.flags = cb::compat::int_from_py<std::uint16_t>(r.flags);
    out.data = cb::compat::from_py(r.data);
    return out;
}
inline py::list<Resource> to_old(const std::vector<nr::Resource>& rs) {
    py::list<Resource> out;
    for (const nr::Resource& r : rs) out.append(to_old(r));
    return out;
}

inline Dialog to_old(const nr::Dialog& d) {
    Dialog out;
    out.extended = d.extended;
    out.style = d.style;
    out.exstyle = d.exstyle;
    out.help = d.help;
    out.x = d.x;
    out.y = d.y;
    out.cx = d.cx;
    out.cy = d.cy;
    out.menu = to_old(d.menu);
    out.class_ = to_old(d.window_class);
    out.title = cb::compat::to_py(d.title);
    if (d.font) {
        if (d.extended) {
            out.font = std::variant<DialogFontPlain, DialogFontExtended>(DialogFontExtended(d.font->points, d.font->weight, d.font->italic, d.font->charset, cb::compat::to_py(d.font->face)));
        } else {
            out.font = std::variant<DialogFontPlain, DialogFontExtended>(DialogFontPlain(d.font->points, cb::compat::to_py(d.font->face)));
        }
    }
    for (const nr::DialogControl& c : d.controls) {
        DialogControl o;
        o.help = c.help;
        o.exstyle = c.exstyle;
        o.x = c.x;
        o.y = c.y;
        o.cx = c.cx;
        o.cy = c.cy;
        o.style = c.style;
        o.id = c.id;
        o.class_ = to_old(c.window_class);
        o.text = to_old(c.text);
        o.extra = cb::compat::to_py(cb::ByteView(c.extra));
        out.controls.append(o);
    }
    return out;
}

inline py::list<MenuItem> to_old(const std::vector<nr::MenuItem>& items) {
    py::list<MenuItem> out;
    for (const nr::MenuItem& i : items) {
        MenuItem m;
        m.text = cb::compat::to_py(i.text);
        m.id = i.id;
        m.flags = i.flags;
        if (i.items) m.items = to_old(*i.items);
        out.append(m);
    }
    return out;
}
inline py::list<ExtMenuItem> to_old(const std::vector<nr::ExtMenuItem>& items) {
    py::list<ExtMenuItem> out;
    for (const nr::ExtMenuItem& i : items) {
        ExtMenuItem m;
        m.text = cb::compat::to_py(i.text);
        m.id = i.id;
        m.type = i.type;
        m.state = i.state;
        m.help = i.help;
        if (i.items) m.items = to_old(*i.items);
        out.append(m);
    }
    return out;
}
inline Menu to_old(const nr::Menu& m) {
    Menu out;
    out.extended = m.extended;
    out.help = m.help;
    std::visit([&](const auto& items) { out.items = to_old(items); }, m.items);
    return out;
}

inline py::list<std::tuple<int64_t, int64_t, int64_t>> to_old(const std::vector<nr::Accelerator>& as) {
    py::list<std::tuple<int64_t, int64_t, int64_t>> out;
    for (const nr::Accelerator& a : as) out.append(std::make_tuple(int64_t(a.flags), int64_t(a.key), int64_t(a.id)));
    return out;
}

// the 13 numbers of VS_FIXEDFILEINFO, in the order the old reader listed them
inline py::list<int64_t> fixed_numbers(const nr::FixedFileInfo& f, std::size_t count) {
    const int64_t all[13] = {f.signature, f.struct_version, f.file_version_ms, f.file_version_ls, f.product_version_ms, f.product_version_ls,
                             f.file_flags_mask, f.file_flags, f.os, f.type, f.subtype, f.date_ms, f.date_ls};
    py::list<int64_t> out;
    for (std::size_t i = 0; i < count; ++i) out.append(all[i]);
    return out;
}
// `fixed_count` is 13 for version() and 11 for decode(), which drops the file date
inline Version to_old(const nr::Version& v, std::size_t fixed_count) {
    Version out;
    if (v.fixed) out.fixed = fixed_numbers(*v.fixed, fixed_count);
    for (const nr::VersionStringTable& t : v.strings) {
        py::list<std::tuple<py::str, py::str>> entries;
        for (const auto& [k, text] : t.entries) entries.append(std::make_tuple(cb::compat::to_py(k), cb::compat::to_py(text)));
        out.strings.set(cb::compat::to_py(t.language_block), entries);
    }
    for (std::uint16_t t : v.translations) out.translations.append(int64_t(t));
    return out;
}

inline TypeLibrary to_old(const nr::TypeLibrary& lib) {
    TypeLibrary out;
    out.format = cb::compat::to_py(lib.format);
    out.name = cb::compat::to_py(lib.name);
    out.guid = cb::compat::to_py(lib.guid);
    for (const nr::TypeLibraryType& t : lib.types) {
        out.types.append(std::make_tuple(cb::compat::to_py(t.kind), cb::compat::to_py(t.name), cb::compat::to_py(t.guid)));
    }
    return out;
}
inline nr::TypeLibrary to_new(const TypeLibrary& lib) {
    nr::TypeLibrary out;
    out.format = cb::compat::from_py(lib.format);
    out.name = cb::compat::from_py(lib.name);
    out.guid = cb::compat::from_py(lib.guid);
    for (const auto& t : lib.types.vec()) {
        out.types.push_back({cb::compat::from_py(std::get<0>(t)), cb::compat::from_py(std::get<1>(t)), cb::compat::from_py(std::get<2>(t))});
    }
    return out;
}

}  // namespace shim_

inline const py::dict<int64_t, py::str>& TYPES() {
    static const py::dict<int64_t, py::str> t = [] {
        py::dict<int64_t, py::str> out;
        for (int64_t n : {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 14, 16, 17, 19, 20, 21, 22, 23, 24}) {
            out.set(n, cb::compat::to_py(cb::formats::resources::type_name(cb::formats::resources::Ident(std::uint32_t(n)))));
        }
        return out;
    }();
    return t;
}
inline const py::dict<int64_t, int64_t>& MEMBER_OF() {
    static const py::dict<int64_t, int64_t> t = {{GROUP_ICON, ICON}, {GROUP_CURSOR, CURSOR}};
    return t;
}
inline const py::dict<int64_t, py::str>& CONTROL_CLASSES() {
    static const py::dict<int64_t, py::str> t = [] {
        py::dict<int64_t, py::str> out;
        for (int64_t atom = 0x80; atom <= 0x85; ++atom) {
            out.set(atom, cb::compat::to_py(*cb::formats::resources::control_class_name(std::uint32_t(atom))));
        }
        return out;
    }();
    return t;
}

inline py::str Resource::repr() const { return cb::compat::to_py(shim_::to_new(*this).describe()); }

inline py::str type_name(const Ident& t) { return cb::compat::to_py(cb::formats::resources::type_name(shim_::to_new(t))); }

inline py::list<Resource> read(const py::str& path) {
    const cb::Bytes file = cb::compat::guard([&] { return cb::fs::read_file(cb::fs::from_text(cb::compat::from_py(path))); });
    switch (cb::formats::resources::kind_of(file)) {
        case cb::formats::resources::ExecutableKind::Pe: return read_pe(pe::open_pe(path));
        case cb::formats::resources::ExecutableKind::Ne: return read_ne(ne::NEFile(path));
        case cb::formats::resources::ExecutableKind::Other: break;
    }
    if (file.size() < 0x40 || file[0] != 'M' || file[1] != 'Z') throw py::ValueError(path.utf8() + ": not an executable");
    return py::list<Resource>();
}

inline py::list<Resource> read_pe(const pe::PEFile& pe_) {
    std::vector<cb::formats::resources::PeResourceEntry> entries;
    py::list<pe::ResourceType> tree = pe_.resource_tree();
    for (const pe::ResourceType& t : tree.vec()) {
        const Ident type = t.id ? Ident(*t.id) : std::visit([](const auto& x) { return Ident(x); }, t.type);
        for (const pe::ResourceItem& item : t.items.vec()) {
            cb::formats::resources::PeResourceEntry e;
            e.type = shim_::to_new(type);
            e.name = shim_::to_new(item.id ? Ident(*item.id) : Ident(item.name));
            e.language = cb::compat::int_from_py<std::uint32_t>(item.language);
            if (item.offset) e.file_offset = cb::compat::int_from_py<std::uint64_t>(*item.offset);
            e.size = cb::compat::int_from_py<std::uint32_t>(item.size);
            entries.push_back(std::move(e));
        }
    }
    return shim_::to_old(cb::formats::resources::from_pe_entries(cb::compat::from_py(pe_.data), entries));
}

inline py::list<Resource> read_ne(const ne::NEFile& nef) {
    return cb::compat::guard([&] { return shim_::to_old(cb::formats::resources::read_ne(cb::compat::from_py(nef.data))); });
}

inline bool same_data(const py::bytes& a, const py::bytes& b) {
    return cb::formats::resources::same_data(cb::compat::from_py(a), cb::compat::from_py(b));
}

inline py::list<py::str> strings(const py::bytes& data, bool wide) {
    return cb::compat::to_py(cb::compat::guard([&] { return cb::formats::resources::strings(cb::compat::from_py(data), wide); }));
}
inline Dialog dialog(const py::bytes& data, bool wide) {
    return shim_::to_old(cb::compat::guard([&] { return cb::formats::resources::dialog(cb::compat::from_py(data), wide); }));
}
inline Menu menu(const py::bytes& data, bool wide) {
    return shim_::to_old(cb::compat::guard([&] { return cb::formats::resources::menu(cb::compat::from_py(data), wide); }));
}
inline py::list<std::tuple<int64_t, int64_t, int64_t>> accelerators(const py::bytes& data, bool wide) {
    return shim_::to_old(cb::compat::guard([&] { return cb::formats::resources::accelerators(cb::compat::from_py(data), wide); }));
}
inline Version version(const py::bytes& data, bool wide) {
    return shim_::to_old(cb::compat::guard([&] { return cb::formats::resources::version(cb::compat::from_py(data), wide); }), 13);
}

inline std::tuple<int64_t, py::list<GroupEntry>> group(const py::bytes& data) {
    const cb::formats::resources::Group g = cb::compat::guard([&] { return cb::formats::resources::group(cb::compat::from_py(data)); });
    py::list<GroupEntry> entries;
    const bool icons = g.kind == cb::formats::resources::GroupKind::Icons;
    for (const cb::formats::resources::GroupEntry& e : g.entries) {
        entries.append(GroupEntry(e.width, e.height, icons ? int64_t(e.colours) : 0, e.planes, e.bits, e.bytes_in_resource, e.member_id));
    }
    return std::make_tuple(int64_t(g.kind), entries);
}

inline py::bytes icon_file(const py::bytes& group_data, const std::function<py::bytes(int64_t)>& member_data) {
    return cb::compat::guard([&] {
        const cb::Bytes made = cb::formats::resources::icon_file(cb::compat::from_py(group_data), [&](std::uint16_t id) {
            return cb::compat::from_py(member_data(id));
        });
        return cb::compat::to_py(cb::ByteView(made));
    });
}

inline py::bytes bitmap_file(const py::bytes& data) {
    return cb::compat::guard([&] { return cb::compat::to_py(cb::ByteView(cb::formats::resources::bitmap_file(cb::compat::from_py(data)))); });
}

inline std::optional<TypeLibrary> type_library(const py::bytes& data) {
    const auto lib = cb::formats::resources::type_library(cb::compat::from_py(data));
    if (!lib) return std::nullopt;
    return shim_::to_old(*lib);
}

inline py::str type_library_outline(const TypeLibrary& library) {
    return cb::compat::to_py(cb::formats::resources::type_library_outline(shim_::to_new(library)));
}

inline Decoded decode(const Resource& resource, bool wide) {
    const auto got = cb::formats::resources::decode(shim_::to_new(resource), wide);
    if (!got) return std::monostate{};
    namespace nr = cb::formats::resources;
    struct Visit {
        Decoded operator()(const std::vector<std::string>& s) const { return cb::compat::to_py(s); }
        Decoded operator()(const nr::Dialog& d) const { return shim_::to_old(d); }
        Decoded operator()(const nr::Menu& m) const { return shim_::to_old(m); }
        Decoded operator()(const std::vector<nr::Accelerator>& a) const { return shim_::to_old(a); }
        Decoded operator()(const nr::Version& v) const { return shim_::to_old(v, 11); }   // the file date is dropped
    };
    return std::visit(Visit{}, *got);
}

}  // namespace codebreaker::formats::resources
