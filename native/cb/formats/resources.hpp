// cb/formats/resources.hpp -- the resources of a Windows module: reading them out and decoding each kind.
//
// A module's dialogs, menus, strings, icons and version block are source its authors wrote, kept in the file as
// data. This reads the resource table of a 16-bit (NE) module, takes the resource entries a 32-bit (PE) reader found,
// and decodes the kinds whose layout is known into plain values that say what the resource IS -- so two builds of a
// resource can be compared by meaning, and so it can be written back out as `.rc` text (build/resources does that).
//
// `wide` is true for a 32-bit module (text is UTF-16, fields are aligned to four bytes) and false for a 16-bit one
// (text is single-byte, fields are packed). A type or a name is an ordinal or text. All text is UTF-8 here (a 16-bit
// module's single-byte text is read as latin-1; a lone UTF-16 surrogate becomes U+FFFD).
//
// Two things about 16-bit modules: a resource's length is stored in alignment units, so its data comes with up to a
// unit of padding after it (`same_data` compares with that in mind); and a 16-bit resource has no language.
//
// Walking a PE file's resource directory is the PE reader's (cb::formats::pe): it hands the entries to
// `from_pe_entries`. Until it lands, the old PE port does that through the shim at the old header path.
#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "cb/core/bytes.hpp"

namespace cb::formats::resources {

// Standard resource types.
inline constexpr std::uint32_t kCursor = 1, kBitmap = 2, kIcon = 3, kMenu = 4, kDialog = 5, kString = 6, kFontDir = 7,
                               kFont = 8, kAccelerator = 9, kRcData = 10, kMessageTable = 11, kGroupCursor = 12,
                               kGroupIcon = 14, kVersion = 16;

// A resource type or name: an ordinal, or text (a real closed union: the file stores one or the other).
using Ident = std::variant<std::uint32_t, std::string>;

// "DIALOG", "STRING", "GROUP_ICON" ...; "#<n>" for an ordinal that is not a standard type; the text itself otherwise.
std::string type_name(const Ident& type);
// The standard type a group resource holds the members of (GROUP_ICON -> ICON, GROUP_CURSOR -> CURSOR).
std::optional<std::uint32_t> member_type_of(std::uint32_t group_type);
// A dialog control's class by its predefined atom ($80 BUTTON ... $85 COMBOBOX), if it is one.
std::optional<std::string_view> control_class_name(std::uint32_t atom);

// The identity of a resource: what two builds of a module are matched on.
struct ResourceKey {
    Ident type;
    Ident name;
    std::optional<std::uint32_t> language;
    friend auto operator<=>(const ResourceKey&, const ResourceKey&) = default;
};

struct Resource {
    Ident type;
    Ident name;
    std::optional<std::uint32_t> language;   // none in a 16-bit module
    std::uint16_t flags = 0;                 // the 16-bit memory flags, else 0
    Bytes data;

    ResourceKey key() const { return {type, name, language}; }
    // <DIALOG 'name' lang=1033 120 bytes>
    std::string describe() const;
};

// ---- reading the tables ------------------------------------------------------------------------------

enum class ExecutableKind { Pe, Ne, Other };
// MZ stub, then the signature at e_lfanew: "PE" or "NE". Other for anything else.
ExecutableKind kind_of(ByteView file);

// The resource table of a 16-bit module (the whole file): per type, a list of (offset, length, flags, name). Empty when
// the module has none, or stores them with a shift this reader does not know (above 15). FormatError if the file is
// not an NE module; a table that runs past the file's end gives the resources before it.
std::vector<Resource> read_ne(ByteView file);

// One resource entry of a 32-bit module as its resource directory gives it.
struct PeResourceEntry {
    Ident type;
    Ident name;
    std::optional<std::uint32_t> language;
    std::optional<std::uint64_t> file_offset;   // where the data sits in the file (none: it is not in the file)
    std::uint32_t size = 0;
};
// The resources of a 32-bit module from its directory's entries; an entry whose data is not wholly in the file is left
// out. `image` is the whole file.
std::vector<Resource> from_pe_entries(ByteView image, std::span<const PeResourceEntry> entries);

// Two resource bodies are the same when one is the other plus zero padding (a 16-bit module stores lengths in
// alignment units).
bool same_data(ByteView a, ByteView b);

// ---- decoding each kind -------------------------------------------------------------------------------

// A string-table block: its 16 strings (block n holds the ids (n-1)*16 .. (n-1)*16+15).
std::vector<std::string> strings(ByteView data, bool wide);

struct DialogControl {
    std::uint32_t help = 0;         // extended templates only
    std::uint32_t exstyle = 0;
    std::uint32_t style = 0;
    std::int16_t x = 0, y = 0, cx = 0, cy = 0;
    std::uint32_t id = 0;
    Ident window_class;             // a name, or one of the predefined atoms
    Ident text;                     // text, or an ordinal (an icon, a bitmap)
    Bytes extra;                    // the control's creation data
};

struct DialogFont {
    std::uint16_t points = 0;
    std::uint16_t weight = 0;       // extended templates only
    std::uint8_t italic = 0;
    std::uint8_t charset = 0;
    std::string face;
};

struct Dialog {
    bool extended = false;          // a DIALOGEX template
    std::uint32_t style = 0;
    std::uint32_t exstyle = 0;
    std::uint32_t help = 0;
    std::int16_t x = 0, y = 0, cx = 0, cy = 0;
    Ident menu;                     // empty text: none
    Ident window_class;             // empty text: the default
    std::string title;
    std::optional<DialogFont> font; // when the style has DS_SETFONT
    std::vector<DialogControl> controls;
};
// FormatError for a template that is cut short or malformed.
Dialog dialog(ByteView data, bool wide);

// A plain menu's item.
struct MenuItem {
    std::string text;
    std::uint16_t id = 0;
    std::uint16_t flags = 0;                      // without the "last item" bit
    std::optional<std::vector<MenuItem>> items;   // set when it opens a submenu
};
// An extended menu's item.
struct ExtMenuItem {
    std::string text;
    std::uint32_t id = 0;
    std::uint32_t type = 0;
    std::uint32_t state = 0;
    std::uint32_t help = 0;
    std::optional<std::vector<ExtMenuItem>> items;
};
struct Menu {
    bool extended = false;
    std::uint32_t help = 0;
    std::variant<std::vector<MenuItem>, std::vector<ExtMenuItem>> items;
};
Menu menu(ByteView data, bool wide);

struct Accelerator {
    std::uint16_t flags = 0;   // without the "last entry" mark
    std::uint16_t key = 0;
    std::uint16_t id = 0;
};
std::vector<Accelerator> accelerators(ByteView data, bool wide);

// VS_FIXEDFILEINFO.
struct FixedFileInfo {
    std::uint32_t signature = 0, struct_version = 0;
    std::uint32_t file_version_ms = 0, file_version_ls = 0, product_version_ms = 0, product_version_ls = 0;
    std::uint32_t file_flags_mask = 0, file_flags = 0, os = 0, type = 0, subtype = 0;
    std::uint32_t date_ms = 0, date_ls = 0;
    friend bool operator==(const FixedFileInfo&, const FixedFileInfo&) = default;
};
struct VersionStringTable {
    std::string language_block;                                       // "040904E4"
    std::vector<std::pair<std::string, std::string>> entries;         // (key, text) in file order
};
struct Version {
    std::optional<FixedFileInfo> fixed;
    std::vector<VersionStringTable> strings;
    std::vector<std::uint16_t> translations;   // (language, code page) pairs of the VarFileInfo, flattened
};
Version version(ByteView data, bool wide);

enum class GroupKind : std::uint16_t { Icons = 1, Cursors = 2 };
struct GroupEntry {
    std::uint16_t width = 0, height = 0;
    std::uint8_t colours = 0;           // icons only
    std::uint16_t planes = 0, bits = 0;
    std::uint32_t bytes_in_resource = 0;
    std::uint16_t member_id = 0;        // the ICON / CURSOR resource that holds the image
};
struct Group {
    GroupKind kind = GroupKind::Icons;
    std::vector<GroupEntry> entries;
};
// An icon or cursor group. FormatError if it is not one.
Group group(ByteView data);

// The .ico (or .cur) file a group resource and its members came from. `member_data(id)` gives a member's bytes. A
// cursor member starts with its hot spot, which a .cur file keeps in the directory instead.
Bytes icon_file(ByteView group_data, const std::function<Bytes(std::uint16_t member_id)>& member_data);

// The .bmp file of a bitmap resource (the resource is the file without its 14-byte header).
Bytes bitmap_file(ByteView data);

struct TypeLibraryType {
    std::string kind;   // "interface", "coclass", "typedef" ...
    std::string name;
    std::string guid;
};
// What a type library says of itself.
struct TypeLibrary {
    std::string format;   // "MSFT" or "SLTG"
    std::string name;
    std::string guid;
    std::vector<TypeLibraryType> types;
};
// Its self-description, or nullopt when `data` is not a type library. Only the documented layout (MSFT) is read that far;
// of the earlier one (SLTG) just the format is reported. A library cut short gives what was read before the cut. Either
// way a type library is carried as a binary payload: rebuilding one needs the interface compiler, which is not in this
// toolchain.
std::optional<TypeLibrary> type_library(ByteView data);

// A type library's contents as an outline in interface-definition syntax: for reading, not for compiling (members and
// their types are not in it).
std::string type_library_outline(const TypeLibrary& library);

// A resource as a value that means the same for any two encodings of it.
using Decoded = std::variant<std::vector<std::string>, Dialog, Menu, std::vector<Accelerator>, Version>;
// nullopt when the resource's kind is not one this module decodes, or its body does not decode (it is then compared as
// bytes). A version's file date is dropped (a compiler stamps it).
std::optional<Decoded> decode(const Resource& resource, bool wide);

}  // namespace cb::formats::resources
