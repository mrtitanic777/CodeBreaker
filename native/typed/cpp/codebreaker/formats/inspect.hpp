// codebreaker/formats/inspect.hpp -- typed C++ of codebreaker.formats.inspect (see inspect.cpp).
//
// inspect.py -- everything an inspector shows about an executable, as one description.
//
//     Description info = inspect::describe(path);     // the same parts whatever the file is
//     inspect::strings(path);                         // [StringRow]
//     inspect::hexdump(path, 0x400, 256);             // the classic offset / hex / text listing
//     inspect::resource_body(path, offset, size);     // a resource shown: an image, text, or a hex listing
//
// One description for every kind of file the corpus holds -- 32-bit Windows (PE), 16-bit Windows (NE), virtual
// device drivers (LE/LX, and the W3 bundle of them), plain DOS programs (MZ) -- and, for a file that is not an MZ
// executable, what detect.identify says it is.  Describing a file never raises: a part that cannot be read is left
// empty and named in `notes`.
//
// Typing notes (the Python's plain dict, as typed values):
//   * `Description` holds the parts that are the same list of records whatever the format (sections, exports,
//     imports, resources) as structs; the parts whose shape differs from one format to another (headers, summary,
//     dos, and the PE/driver extras: directories, relocations, tls, debug, certificates, clr, rich, load_config,
//     symbols_file, driver, members, xex) are `Value`s -- an ordered JSON-like tree (None, bool, int, str, list,
//     ordered dict), because the Python genuinely mixes types there (summary["entry"] is an int in a PE and a
//     "seg:off" string in an NE).  `to_value(description)` is the whole dict, keys in the order the Python's dict had
//     them; render(), markdown() and the command work on that.
//   * the native readers of inspect (native/formats/win/inspect.cc: strings, hex_lines, categorize) were answered in
//     pickled values through a registry; here they are the same algorithm in typed C++ (strings / hexdump below).
//   * `formats.xbox.summary` (the Xbox containers and assets) has no typed port yet: `xbox_summary()` is the hook it
//     is called through; unset, a file that wants it gets an ImportError in `notes`, as a failed reader's would.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>
#include <variant>
#include <vector>

#include "codebreaker/formats/detect.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::formats::inspect {

// ---- an ordered JSON-like tree (what the Python's plain dicts / lists / scalars are, where they mix types)
class Value {
  public:
    enum class Kind { None, Bool, Int, Str, List, Dict, Float };
    Kind kind = Kind::None;
    bool b = false;
    int64_t i = 0;
    double d = 0.0;               // Float
    py::str s;
    std::vector<Value> items;     // List: the items; Dict: the values (parallel to keys)
    std::vector<py::str> keys;    // Dict: the keys, in insertion order

    Value() = default;
    explicit Value(bool v) : kind(Kind::Bool), b(v) {}
    Value(int64_t v) : kind(Kind::Int), i(v) {}
    Value(int v) : kind(Kind::Int), i(v) {}
    Value(double v) : kind(Kind::Float), d(v) {}
    Value(const py::str& v) : kind(Kind::Str), s(v) {}
    Value(const char* v) : kind(Kind::Str), s(v) {}
    static Value none() { return Value(); }
    static Value list() { Value v; v.kind = Kind::List; return v; }
    static Value dict() { Value v; v.kind = Kind::Dict; return v; }
    static Value from_strings(const py::list<py::str>& xs);

    bool is_none() const { return kind == Kind::None; }
    bool is_dict() const { return kind == Kind::Dict; }
    bool is_list() const { return kind == Kind::List; }
    // list: append
    Value& push(Value v);
    // dict: d[key] = v (a key already there keeps its place)
    Value& set(const py::str& key, Value v);
    // dict: the value at key, or nullptr
    const Value* get(const py::str& key) const;
    Value* get(const py::str& key);
    bool has(const py::str& key) const { return get(key) != nullptr; }
    // d[key]: KeyError when absent
    const Value& at(const py::str& key) const;
    // dict.pop(key, None)-like removal; true when it was there
    bool erase(const py::str& key);
    // d.get(key, dflt) read as int / str
    int64_t get_int(const py::str& key, int64_t dflt = 0) const;
    py::str get_str(const py::str& key, const py::str& dflt = py::str("")) const;
    // truthiness (Python's: empty and zero are false)
    bool truthy() const;
    int64_t size() const { return static_cast<int64_t>(kind == Kind::Dict ? keys.size() : items.size()); }
};

// json.dumps(value[, indent=N]) (ensure_ascii; keys in insertion order, or sorted when `sort_keys`)
py::str dumps(const Value& v, std::optional<int64_t> indent = std::nullopt, bool sort_keys = false);

// ---- the vocabulary
extern const py::list<py::str> SECTIONS;
extern const py::list<py::str> DIRECTORIES;
const py::dict<int64_t, py::str>& RESOURCE_TYPES();

using Address = std::variant<int64_t, py::str>;   // a section / export address: a number (PE, LE) or "seg:off" (NE, DOS)

struct Section {
    py::str name;
    Address address;
    int64_t size = 0, file_offset = 0, file_size = 0;
    py::list<py::str> flags;
    bool code = false;
};

struct ExportSymbol {
    py::str name;
    int64_t ordinal = 0, hint = 0;
    py::str forward;
    Address address;
    bool ne = false;                 // an NE export: key order name, ordinal, hint, address, forward, movable, exported
    bool movable = false, exported = false;   // NE only
};
struct Exports {
    py::str module;
    int64_t count = 0;
    std::optional<int64_t> ordinal_base;      // PE only
    py::list<ExportSymbol> symbols;
};

struct ImportSymbol {
    py::str name;
    int64_t ordinal = 0, hint = 0;
    std::optional<int64_t> slot;              // PE only
};
struct ImportModule {
    py::str module;
    int64_t count = 0;
    bool delay = false;
    py::list<ImportSymbol> symbols;
};

struct ResourceItem {
    py::str name;
    bool pe = false;                          // a PE item has id and rva; an NE item has neither
    std::optional<int64_t> id;
    int64_t language = 0;
    std::optional<int64_t> rva;
    std::optional<int64_t> offset;
    int64_t size = 0;
    int64_t type_id = 0;
    py::str preview;
    bool wide = false;
};
struct ResourceType {
    py::str type;
    std::optional<int64_t> id;
    int64_t count = 0;
    py::list<ResourceItem> items;
};
struct Resources {
    int64_t count = 0;
    py::list<ResourceType> types;
};

// What describe() found. Parts the Python's dict held in insertion order after `resources` are `extras`.
struct Description {
    py::str path, name;
    int64_t size = 0;
    std::optional<py::str> format, kind;
    bool ok = false;
    py::list<py::str> notes;
    Value summary = Value::dict();
    Value dos;                                // None, or the DOS header's fields (a dict; {} for a header that is not there)
    Value headers = Value::dict();
    py::list<Section> sections;
    Exports exports;
    py::list<ImportModule> imports;
    Resources resources;
    std::vector<std::pair<py::str, Value>> extras;   // directories, relocations, tls, ... members, xex: in insertion order
    Value& extra(const py::str& key);                // d[key] (inserted as None when new)
    const Value* find_extra(const py::str& key) const;
};

Value to_value(const Description& d);
Value to_value(const Section& s);
Value to_value(const Exports& e);
Value to_value(const ImportModule& m);
Value to_value(const Resources& r);

// Everything known about the file at `path`; never raises.
Description describe(const py::str& path);

// ---- folders
struct FolderFormat {
    py::str format, family, command;
    int64_t files = 0, bytes = 0;
    py::str example;
};
struct FolderOther {
    py::str extension;
    int64_t files = 0, bytes = 0;
    py::str example;
};
struct FolderMap {
    py::str folder;
    int64_t files = 0, bytes = 0;
    py::list<FolderFormat> formats;
    py::list<FolderOther> unrecognised;
};
Value to_value(const FolderMap& m);

// Every file under `folder`, identified and tallied by format (the map of a directory).
FolderMap folder_map(const py::str& folder, const std::function<void(const py::str&)>& log = nullptr);

// A text file, named by what its extension says it holds; None when it is not text.
std::optional<detect::Detected> _text_kind(const py::str& path, const py::str& name);

// 1.5 KB / 12 B ...
py::str _size(double n);

// A one-page markdown report of a description (to_value(describe(path))) or of a folder map (to_value(folder_map)).
py::str markdown(const Value& info, const std::optional<Value>& xex_header = std::nullopt);

// What a .xex's own header says, for `info`; {} when it is not one.
Value xex_facts(const py::str& path);

// ---- strings, bytes, resource bodies
struct StringRow {
    int64_t offset = 0;
    std::optional<Address> address;           // where that byte is loaded: an address (PE), "seg:off" (NE), None elsewhere
    py::str kind, category, text;             // kind: 'ascii' | 'utf16'
};
Value to_value(const StringRow& r);

// every run of printable characters in the file, 8-bit and 16-bit, in file order
py::list<StringRow> strings(const py::str& path, int64_t min_len = 4, bool wide = true, std::optional<int64_t> limit = std::nullopt);
// the category of a string ('format', 'module', 'path', 'constant', 'error', 'other')
py::str categorize(const py::str& text);
// the classic hex listing of `data`, one line per 16 bytes, addresses from `base`
py::list<py::str> _hex_lines(const py::bytes& data, int64_t base = 0);
// `length` bytes of the file from `offset`, as offset / hex / text lines
py::str hexdump(const py::str& path, int64_t offset = 0, int64_t length = 256);

struct ResourceBody {
    py::str kind;                             // 'image' | 'text' | 'hex'
    std::optional<py::str> content_type;      // images
    std::optional<py::bytes> bytes;           // images
    std::optional<py::str> text;              // text and hex
};
// A resource's content made showable (see inspect.py).
ResourceBody resource_body(const py::str& path, int64_t offset, int64_t size, int64_t type_id = -1, bool wide = true,
                           int64_t limit = 1 << 20);

// ---- as text
py::str _cell(const py::str& key, const Value& value);
// Any part of a description as readable lines: a dict as `name  value` lines, a list of records as a table.
py::list<py::str> render(const Value& value, const py::str& key = py::str(""), int64_t indent = 0);

// The formats.xbox summary hook (see the top).
using XboxSummary = std::function<Value(const py::str& path, const detect::Detected& match)>;
XboxSummary& xbox_summary();

// python -m codebreaker info ...
int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::formats::inspect
