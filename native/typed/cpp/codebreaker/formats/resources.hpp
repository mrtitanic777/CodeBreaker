// codebreaker/formats/resources.hpp -- typed C++ of codebreaker.formats.resources
//
// resources.py -- the resources of a Windows module: reading them out and decoding each kind.
//
// A module's dialogs, menus, strings, icons and version block are source its authors wrote, kept in
// the file as data. This reads the resource tree of a 32-bit (PE) or 16-bit (NE) module and decodes
// the kinds whose layout is known into plain values that say what the resource IS -- so two
// builds of a resource can be compared by meaning, and so it can be written back out as `.rc` text
// (build/resources.py does that).
//
//     for (const Resource& r : resources::read(path).vec())     // every resource of a PE or NE file
//         r.type, r.name, r.language, r.flags, r.data
//     resources::type_name(r.type)             // "DIALOG", "STRING", ... or the type as it stands
//     resources::decode(r, wide)               // the resource as a value; None (std::monostate) when its kind is opaque
//     resources::icon_file(group, member_data) // a whole .ico / .cur file from a group resource
//     resources::bitmap_file(data)             // a .bmp file from a bitmap resource
//     resources::type_library(data)            // what a type library says of itself (name, GUIDs, types)
//
// `wide` is true for a 32-bit module (text is UTF-16, fields are aligned to four bytes) and false
// for a 16-bit one (text is single-byte, fields are packed). A type or a name is an int (an
// ordinal) or a str.
//
// Two things about 16-bit modules: a resource's length is stored in alignment units, so its data
// comes with up to a unit of padding after it (`same_data` compares with that in mind); and a
// 16-bit resource has no language.
//
// Typing notes (how the Python's dynamic parts are typed here):
//   * A type or a name is `Ident`, std::variant<int64_t, py::str> (an ordinal, or text).
//   * The decoders were answered by formats.dll (native/formats/win/resources.cc) as dicts and tuples; the
//     dicts with fixed keys are the structs below, written in that file's own order of fields, and its C++ is
//     carried over on typed values (cbf::Bytes, which keeps Python's rules: IndexError past the end,
//     struct.error for a value that does not fit). An exception it raised is thrown as the py:: exception
//     of the same Python type.
//   * dialog's `font` is None, (points, face), or -- in an extended template -- (points, weight, italic,
//     charset, face): std::optional of a std::variant of those two tuples.
//   * menu's items are MenuItem (a plain menu) or ExtMenuItem (an extended one); `Menu::items` holds one
//     list or the other.
//   * version's answer, the tuple (fixed, strings, translations), is the struct `Version`; `fixed` is the 13
//     numbers of the fixed part (11 after decode(), which drops the file date).
//   * decode()'s answer is std::variant<std::monostate (None), strings, Dialog, Menu, accelerators, Version>.
//   * icon_file takes member_data as a std::function from a member id to its bytes.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>
#include <variant>

#include "codebreaker/formats/ne.hpp"
#include "codebreaker/formats/pe.hpp"
#include "py.hpp"
#include "py_os.hpp"

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

// ---------------------------------------------------------------------------------------------
// reading the tree
// ---------------------------------------------------------------------------------------------
// Every resource of a module, in the order the file lists them. [] when it has none.
py::list<Resource> read(const py::str& path);
// The resources of a 32-bit module (the directory itself is walked by PEFile.resource_tree).
py::list<Resource> read_pe(const pe::PEFile& pe);
// The resource table of a 16-bit module: per type, a list of (offset, length, flags, name).
py::list<Resource> read_ne(const ne::NEFile& nef);
// Two resource bodies are the same when one is the other plus zero padding (a 16-bit
// module stores lengths in alignment units).
bool same_data(const py::bytes& a, const py::bytes& b);

// ---------------------------------------------------------------------------------------------
// decoding each kind
// ---------------------------------------------------------------------------------------------
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
// -> its self-description, or None when `data` is not a type library. Only the documented layout (MSFT)
// is read that far; of the earlier one (SLTG) just the format is reported. Either way a type library is
// carried as a binary payload: rebuilding one needs the interface compiler, which is not in this toolchain.
std::optional<TypeLibrary> type_library(const py::bytes& data);

// A type library's contents as an outline in interface-definition syntax: for reading, not
// for compiling (members and their types are not in it).
py::str type_library_outline(const TypeLibrary& library);

// A resource as a value that means the same for any two encodings of it, or None when its
// kind is not one this module decodes (it is then compared as bytes).
using Decoded = std::variant<std::monostate, py::list<py::str>, Dialog, Menu, py::list<std::tuple<int64_t, int64_t, int64_t>>, Version>;
Decoded decode(const Resource& resource, bool wide);

}  // namespace codebreaker::formats::resources
