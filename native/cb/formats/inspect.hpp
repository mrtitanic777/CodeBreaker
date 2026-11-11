// cb/formats/inspect.hpp -- everything an inspector shows about a file, as one typed description.
//
//     Description d = inspect::describe(path);           // the same parts whatever the file is; never throws
//     inspect::strings(path, {.min_length = 6});         // the runs of printable characters, with where they load
//     inspect::hexdump(path, 0x400, 256);                // the classic offset / hex / text listing
//     inspect::resource_body(path, offset, size, type);  // a resource made showable: an image, text, or hex
//     inspect::folder_map(folder);                       // every file of a folder identified and tallied
//     inspect::markdown(d) / inspect::render(...)        // the reports; `info_command()` is `codebreaker info`
//
// One description covers every kind of MZ-family file the corpus holds -- 32-bit Windows (PE), 16-bit Windows (NE),
// virtual device drivers (LE/LX, and the W3 bundle of them), plain DOS programs -- and, for any other file, what
// detect::identify says it is. The file is read by the format readers (pe, ne, le, detect); nothing is parsed a second
// time here. Describing a file never throws: a part that cannot be read is left empty and named in `notes`
// (the readers' strict errors are caught per part, which is what lets one damaged table not hide the rest).
//
// The description is typed; `to_json` is the whole thing as the document `info --json` prints, keys in the order a
// reader expects them. `render` and `markdown` are the text reports; they work on that document (an ordered JSON
// tree), which is why `render` is also public: it is the generic "record or table of records as aligned text" view.
//
// Xbox containers and assets are described by readers that are not in the tree yet; `DescribeHooks::xbox_summary` is
// the seam they plug into (unset: such a file gets its format line only).
#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/cli.hpp"
#include "cb/core/json_fwd.hpp"
#include "cb/formats/detect.hpp"
#include "cb/formats/le.hpp"
#include "cb/formats/ne.hpp"
#include "cb/formats/pe.hpp"

namespace cb::formats::inspect {

// ---- the vocabulary ------------------------------------------------------------------------------------------
// The names `info --section` accepts, in the order a description lists them.
extern const std::vector<std::string_view> kSections;
// The sixteen PE data directories by index.
extern const std::vector<std::string_view> kDirectoryNames;
// The predefined resource types (RT_*) by number; empty for a number that has none.
std::string_view resource_type_name(std::uint32_t type_id);

// "label: value" pairs shown as they are: what a non-executable file is, an Xbox container's facts, a .xex's header.
using Facts = std::vector<std::pair<std::string, std::string>>;

// A place in a file's address space: nothing, a linear address (PE, LE objects), or segment and offset.
struct SegmentAddress {
    enum class Style : std::uint8_t {
        NeSegment,  // "3:01a4"        a 1-based NE segment number and a 16-bit offset
        RealMode,   // "1234:0010"     segment and offset in hex
        Driver,     // "1:00001000"    an LE object number and a 32-bit offset
    };
    std::uint32_t segment = 0, offset = 0;
    Style style = Style::NeSegment;
    friend bool operator==(const SegmentAddress&, const SegmentAddress&) = default;
};
using Address = std::variant<std::monostate, std::uint64_t, SegmentAddress>;
std::string address_text(const Address& a);  // "" for none, "0x..." for a linear address, else "seg:off"

// ---- the parts every format fills ----------------------------------------------------------------------------
// A section of a PE, a segment of an NE, an object of an LE, the program of a DOS file.
struct Section {
    std::string name;
    Address address;
    std::uint64_t size = 0, file_offset = 0, file_size = 0;
    std::vector<std::string> flags;
    bool code = false;
};

struct NeExportInfo {
    bool movable = false, exported = false;
};
struct ExportSymbol {
    std::optional<std::string> name;  // absent: exported by ordinal only
    std::uint64_t ordinal = 0;
    std::optional<std::uint32_t> hint;  // PE: the name's index in the name table
    Address address;
    std::optional<std::string> forward;  // PE: "OTHER.Function"
    std::optional<NeExportInfo> ne;      // present for an NE export
};
struct Exports {
    std::string module;
    std::optional<std::uint32_t> ordinal_base;  // PE only
    std::vector<ExportSymbol> symbols;
};

struct ImportSymbol {
    std::optional<std::string> name;      // absent: by ordinal
    std::optional<std::uint32_t> ordinal;
    std::optional<std::uint32_t> hint;    // PE: by-name imports
    std::optional<std::uint64_t> slot;    // PE: RVA of the address-table entry the loader fills in
};
struct ImportModule {
    std::string module;
    bool delay = false;  // from the delay-load directory
    std::vector<ImportSymbol> symbols;
};

enum class Preview { Image, Text, Hex };
const char* preview_name(Preview p);  // "image" | "text" | "hex"

struct ResourceItem {
    std::optional<std::string> name;  // a named item; an NE item's id is its name ("#12")
    std::optional<std::uint32_t> id;  // PE only
    std::uint32_t language = 0;
    std::optional<std::uint32_t> rva;  // PE only
    std::optional<std::uint64_t> offset;  // in the file
    std::uint64_t size = 0;
    std::int32_t type_id = -1;  // the RT_* number; -1 for a type that goes by a name
    Preview preview = Preview::Hex;
    bool wide = true;       // strings inside are UTF-16 (a 32-bit module)
    bool from_pe = false;   // a PE item has an id and an rva, an NE item has neither
};
struct ResourceType {
    std::string type;  // "DIALOG", "#255", or the name it goes by
    std::optional<std::int32_t> id;
    std::vector<ResourceItem> items;
};
struct Resources {
    std::vector<ResourceType> types;
    std::size_t count() const;
};

// ---- the summary ------------------------------------------------------------------------------------------------
// The handful of facts worth a first look, the same for every executable format.
struct Summary {
    std::string format, type, machine, module, description, subsystem, linker, linked;
    std::uint64_t image_base = 0;
    Address entry;  // none: no entry point
    std::uint64_t code_bytes = 0;
    std::size_t sections = 0, exports = 0, import_modules = 0, imports = 0, resources = 0;
};

// ---- headers and the parts only some formats have -----------------------------------------------------------------
struct PeHeaders {
    bool read = true;  // false: the headers could not be read (they are shown empty)
    std::string machine;
    std::uint16_t machine_id = 0, sections = 0, optional_header_size = 0;
    std::uint32_t timestamp = 0;
    std::vector<std::string> characteristics;
    pe::OptionalHeader optional;
};
struct NeHeaders {
    ne::Header header;
};
// An LE/LX header, with the cpu and os named, and whether it was an LE or an LX.
struct LeHeaders {
    std::string signature;  // "LE" | "LX"
    le::Header header;
};
struct W3Headers {
    std::uint16_t version = 0;
    std::size_t members = 0;
};
using Headers = std::variant<std::monostate, PeHeaders, NeHeaders, LeHeaders, W3Headers, detect::DosHeader>;

struct DirectoryRow {
    std::string name;
    std::uint32_t rva = 0, size = 0;
};
struct RelocationSummary {
    std::size_t blocks = 0, entries = 0, addresses = 0;
};
struct DebugRow {
    std::string type;  // "CodeView", "FPO", ... or the number
    std::uint32_t timestamp = 0, size = 0, offset = 0;
    std::string linked;
};
// What only a PE has.
struct PeExtras {
    std::vector<DirectoryRow> directories;
    RelocationSummary relocations;
    std::optional<pe::Tls> tls;
    std::vector<DebugRow> debug;
    std::vector<pe::Certificate> certificates;
    std::optional<pe::Clr> clr;
    std::optional<pe::Rich> rich;
    bool load_config = false;
    std::optional<pe::CodeView> symbols_file;
};
struct NeExtras {
    std::size_t relocation_records = 0;
};
struct DosExtras {
    std::uint16_t relocation_count = 0;
    std::vector<std::string> entries;  // "ssss:oooo", the first 512
};

// One LE/LX image: a driver file, or one member of a W3 bundle.
struct DriverDescription {
    LeHeaders headers;
    std::vector<Section> sections;
    std::vector<ImportModule> imports;
    std::optional<le::Ddb> driver;
    Exports exports;
    Resources resources;
    Summary summary;
};
struct BundleMember {
    std::string name;
    std::uint32_t offset = 0, size = 0;
    std::vector<std::string> notes;
    std::optional<DriverDescription> driver;  // absent when the member is not an LE/LX or could not be read
};
struct DriverExtras {
    std::optional<le::Ddb> driver;  // the device descriptor block
};
struct BundleExtras {
    std::vector<BundleMember> members;
};
using Extras = std::variant<std::monostate, PeExtras, NeExtras, DosExtras, DriverExtras, BundleExtras>;

// What describe() found.
struct Description {
    std::string path, name;  // name: the file name in lower case
    std::uint64_t size = 0;
    std::optional<std::string> format;  // "PE", "NE", "LE", "LX", "W3", "MZ"; a non-executable's format line
    std::optional<std::string> kind;    // the lane ("pe", "ne", "le", "dos"), or the family of a non-executable
    bool ok = false;
    std::vector<std::string> notes;  // parts that were damaged, and why
    std::optional<Summary> summary;  // executables
    Facts facts;                     // non-executables: what the file is
    std::optional<std::optional<detect::DosHeader>> dos;  // the DOS header every MZ file starts with (inner nullopt: too short)
    Headers headers;
    std::vector<Section> sections;
    Exports exports;
    std::vector<ImportModule> imports;
    Resources resources;
    Extras extras;
    Facts xex;  // an .xex's own header (set by the `info` command)
};

// Hooks for what the tree does not read yet.
struct DescribeHooks {
    // The facts an Xbox / Xbox 360 container or asset adds to its format line (the first detect::identify match).
    std::function<Facts(const std::filesystem::path&, const detect::Detected&)> xbox_summary;
};

// Everything known about the file at `path`; never throws.
Description describe(const std::filesystem::path& path, const DescribeHooks& hooks = {});

// The description as the `info --json` document.
Json to_json(const Description& d);

// ---- folders ------------------------------------------------------------------------------------------------------
struct FolderFormat {
    std::string format, family, command;
    std::uint64_t files = 0, bytes = 0;
    std::string example;
};
struct FolderOther {
    std::string extension;
    std::uint64_t files = 0, bytes = 0;
    std::string example;
};
struct FolderMap {
    std::string folder;
    std::uint64_t files = 0, bytes = 0;
    std::vector<FolderFormat> formats;       // largest first
    std::vector<FolderOther> unrecognised;   // by extension, largest first
};
// Every file under `folder`, identified and tallied by format. `progress` hears of every 2000 files.
FolderMap folder_map(const std::filesystem::path& folder, const std::function<void(std::string_view)>& progress = {});
Json to_json(const FolderMap& m);

// A text file named by what its extension says it holds; nullopt when it is not text (or has no such extension).
std::optional<detect::Detected> text_kind(const std::filesystem::path& path);

// ---- strings, bytes, resource bodies --------------------------------------------------------------------------------
enum class StringKind { Ascii, Utf16 };
enum class StringCategory { Format, Module, Path, Constant, Error, Other };
const char* string_kind_name(StringKind k);          // "ascii" | "utf16"
const char* string_category_name(StringCategory c);  // "format" | "module" | "path" | "constant" | "error" | "other"
// What a string looks like: a printf format, a module file name, a path, a CONSTANT_NAME, an error message, or other.
StringCategory categorize(std::string_view text);

struct StringOptions {
    std::size_t min_length = 4;
    bool wide = true;                    // also the 16-bit (UTF-16, printable ASCII) runs
    std::optional<std::size_t> limit;    // the first `limit` rows only
};
struct StringRow {
    std::uint64_t offset = 0;
    std::optional<Address> address;  // where that byte is loaded: an address (PE), "seg:off" (NE), nothing elsewhere
    StringKind kind = StringKind::Ascii;
    StringCategory category = StringCategory::Other;
    std::string text;
};
// Every run of printable characters in `data`, in file order, 8-bit before 16-bit at the same offset (no addresses).
std::vector<StringRow> find_strings(ByteView data, const StringOptions& options = {});
// The same for the file at `path`, with each row's load address. IoError when the file cannot be read.
std::vector<StringRow> strings(const std::filesystem::path& path, const StringOptions& options = {});
Json to_json(const std::vector<StringRow>& rows);

// The classic hex listing of `data`, one line per 16 bytes, addresses counted from `base`.
std::vector<std::string> hex_lines(ByteView data, std::uint64_t base = 0);
// `length` bytes of the file from `offset` as offset / hex / text lines joined with newlines (fewer at the end of the file).
std::string hexdump(const std::filesystem::path& path, std::uint64_t offset = 0, std::uint64_t length = 256);

struct ResourceBody {
    enum class Kind { Image, Text, Hex } kind = Kind::Hex;
    std::string content_type;  // an image: "image/bmp" or "image/x-icon"
    Bytes bytes;               // an image: the file it would be
    std::string text;          // text and hex
};
// A resource's content made showable: bitmaps, icons and cursors as image files; version information, string
// tables, manifests and readable raw data as text; anything else (and anything damaged) as a hex listing.
// `offset` and `size` are an item's from describe(); `wide` is false for a 16-bit module.
ResourceBody resource_body(const std::filesystem::path& path, std::uint64_t offset, std::uint64_t size,
                           std::int32_t type_id = -1, bool wide = true, std::uint64_t limit = 1u << 20);

// ---- reports --------------------------------------------------------------------------------------------------------
// One value of a description as a table cell: yes/no, numbers (hex for the keys that hold addresses and sizes of
// things in memory), lists joined by spaces.
std::string cell(std::string_view key, const Json& value);
// Any part of a description as readable lines: a record as `name  value` lines, a list of records as a table.
std::vector<std::string> render(const Json& value, std::string_view key = "", int indent = 0);
// A one-page markdown report of a description, or of a folder map; or of either one's document (`to_json`).
std::string markdown(const Json& document, const Facts& xex = {});
std::string markdown(const Description& d);
std::string markdown(const FolderMap& m);
// 1.5 KB / 12 B ...
std::string size_text(std::uint64_t bytes);

// What a .xex's own header says; empty when the file is not one.
Facts xex_facts(const std::filesystem::path& path);

// ---- the `info` command ------------------------------------------------------------------------------------------------
struct InfoEnvironment {
    // A corpus module name -> its retail file; an existing path is returned as it is. Without it only paths work.
    std::function<std::optional<std::filesystem::path>(std::string_view)> resolve;
    DescribeHooks hooks;
};
// `codebreaker info <file-or-module|folder> [--json] [--section NAME] [--offset N] [--length N] [--min N]
// [--markdown] [--out FILE.md] [--unpack OUT.exe]`. Writes through cb::out; returns the exit code.
cli::Command info_command(InfoEnvironment env = {});

}  // namespace cb::formats::inspect
