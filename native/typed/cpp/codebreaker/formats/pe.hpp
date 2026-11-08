// codebreaker/formats/pe.hpp -- typed C++ from codebreaker.formats.pe.
//
// pe.py -- PE32 / PE32+ reader for the era's 32-bit modules, answered by the C++ (native/formats/pe:
// formats.dll, through native.py). Its Python reader is gone (docs/NATIVE.md, step 4): this module
// is the API -- each method's def is its signature and its documentation, and formats.dll answers.
//
// Windows 3.1 was NE all the way down, so `ne.py` was the only loader the toolchain needed. This is
// the first release in the lineage where the core ships in two worlds at once: the 16-bit
// compatibility layer (KRNL386, USER, GDI -- still NE) and the new 32-bit engines (KERNEL32, USER32,
// GDI32 and 600+ other modules -- PE). This is the 32-bit half: enough to tier and measure the
// corpus (section table, code size), feed the ordinal work (export directory, name<->ordinal), and
// show a module's structure (PE Explorer's views).
//
// Typing and runtime notes:
//   * The Python asked formats.dll through a ctypes/pickle bridge (codebreaker.formats.native). Here the
//     very C++ the DLL is built from (native/formats/pe/pe.cc: cbf::pe::Image) is called directly, with typed
//     results; the dicts with fixed keys that the DLL answered are the structs below. Like the DLL, every
//     method reads the image again from `data` (PEFile keeps nothing else); an error the reader raises
//     (cbf::PyError) is thrown as the same py:: exception.
//   * Addresses and the image base can be 64-bit unsigned (PE32+): those fields are uint64_t. Python's
//     unbounded ints are int64_t elsewhere; an address sum past 64 bits is OverflowError (the DLL raises it too,
//     except BuildMeta::entry_va, whose Python int goes past 64 bits only for a corrupt PE32+ image base).
//   * pe.code_sections' answer is (va, bytes, name) -- three items, as the DLL gives, though the Python docstring
//     says two.
//   * `type` of a ResourceType is a name or a number: std::variant<py::str, int64_t>.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>
#include <variant>

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::formats::pe {

// one section header (the dict pe.parse gives)
struct Section {
    py::str name;
    int64_t vsize = 0, vaddr = 0, rawsize = 0, rawptr = 0, chars = 0;
};

// pe.build_meta
struct BuildMeta {
    py::str machine, magic, kind;
    uint64_t image_base = 0;
    int64_t entry_rva = 0;
    uint64_t entry_va = 0;
    int64_t timestamp = 0;
    py::str linker_version, subsystem;
};

// pe.debug_codeview: RSDS -> {format, guid, age, pdb}; NB10 -> {format, signature, age, pdb}
struct CodeView {
    py::str format;
    std::optional<py::str> guid;          // RSDS
    std::optional<int64_t> signature;     // NB10
    int64_t age = 0;
    py::str pdb;
};

// pe.optional_header
struct OptionalHeader {
    int64_t magic = 0;
    py::str linker_version;
    int64_t code_size = 0, initialized_data_size = 0, uninitialized_data_size = 0, entry_point = 0, code_base = 0;
    uint64_t image_base = 0;
    int64_t section_alignment = 0, file_alignment = 0;
    py::str os_version, image_version, subsystem_version;
    int64_t image_size = 0, headers_size = 0, checksum = 0, subsystem = 0;
    py::str subsystem_name;
    int64_t dll_characteristics = 0;
};

struct ExportSymbol {
    py::str name;
    int64_t ordinal = 0, hint = 0, rva = 0;
    py::str forward;
};
// pe.export_table
struct ExportTable {
    py::str module;
    int64_t ordinal_base = 0, timestamp = 0;
    py::list<ExportSymbol> symbols;
};

struct ImportSymbol {
    py::str name;
    int64_t ordinal = 0, hint = 0, slot = 0;
};
struct ImportModule {
    py::str module;
    py::list<ImportSymbol> symbols;
};

struct ResourceItem {
    py::str name;
    std::optional<int64_t> id;            // None for an item that goes by a name
    int64_t language = 0, rva = 0;
    std::optional<int64_t> offset;
    int64_t size = 0;
};
struct ResourceType {
    std::variant<py::str, int64_t> type;  // the type's name, or its number
    std::optional<int64_t> id;            // None for a type that goes by a name
    py::list<ResourceItem> items;
};

struct Tls {
    uint64_t start_va = 0, end_va = 0, index_va = 0, callbacks_va = 0;
};
struct DebugEntry {
    int64_t type = 0, timestamp = 0, size = 0, offset = 0;
};
struct Certificate {
    int64_t length = 0, revision = 0, type = 0;
};
struct Clr {
    py::str runtime_version;
    int64_t flags = 0, entry_token = 0;
};
struct Rich {
    int64_t key = 0;
    py::list<std::tuple<int64_t, int64_t, int64_t>> entries;   // (product, build, count)
};

// A PE image read from `path`: e_lfanew, machine, image_base, entry, sections, datadirs ...
// (pe.parse's attributes), every method answered by the C++ reader.
class PEFile {
  public:
    py::str path;
    py::bytes data;
    int64_t e_lfanew = 0, machine = 0, n_sections = 0, timestamp = 0, opt_size = 0, characteristics = 0, magic = 0;
    bool is_plus = false;
    int64_t entry = 0;
    uint64_t image_base = 0;
    int64_t n_datadir = 0;
    py::list<std::tuple<int64_t, int64_t>> datadirs;   // (rva, size)
    py::list<Section> sections;

    explicit PEFile(const py::str& path);

    py::str machine_name() const;
    bool is_dll() const;
    std::optional<int64_t> _rva_to_off(int64_t rva) const;
    // Sum of raw sizes of executable/code sections.
    int64_t code_bytes() const;
    // The export directory's internal name, or '' if not exported.
    py::str module_name() const;
    // {name: rva} for every export that has a name.
    py::dict<py::str, int64_t> export_addrs() const;
    // [(ordinal, name_or_None, rva)] from the export directory.
    py::list<std::tuple<int64_t, std::optional<py::str>, int64_t>> exports() const;
    // {iat_slot_va: (dll, name)} from the import directory (data dir 1).
    //
    // A `call dword ptr [iat_slot_va]` in the code resolves to an imported
    // API through this map -- the PE analogue of NE's module.ordinal imports,
    // and what lets pe_trace name cross-module calls instead of leaving them
    // as raw `[0x....]`.  name is the imported symbol, or '#<ordinal>' for a
    // by-ordinal import.
    py::dict<uint64_t, std::tuple<py::str, py::str>> imports() const;
    // [(va_start, bytes, name)] for executable sections.
    py::list<std::tuple<uint64_t, py::bytes, py::str>> code_sections() const;
    // Set of RVAs that carry an absolute 32-bit address (HIGHLOW, type 3).
    //
    // The PE analogue of NE's FIXUPP relocation sites. Data directory 5 (.reloc)
    // lists every field the loader would patch if the image were not loaded at
    // its preferred ImageBase. On disk each such field already holds the
    // preferred-base absolute value, so a faithful reconstruction that emits the
    // literal reproduces the shipped bytes exactly; the mask only guards the
    // rare case where our encoding differs at a relocated field. Type 3 = the
    // 4-byte absolute (HIGHLOW) used throughout a 32-bit image; type 0 = padding.
    py::set<int64_t> base_relocs() const;
    // Structural build metadata: machine, bitness, image base, entry, link
    // timestamp, linker version, subsystem, EXE/DLL. All from the headers.
    BuildMeta build_meta() const;
    // The CodeView record from the DEBUG data directory (index 6), or None.
    //
    // Modern toolchains emit an RSDS record here pointing at the matching PDB
    // (a GUID+age key); older ones emit NB10 (a timestamp+age key). This is the
    // PE debug-directory form, distinct from the appended NB09 CodeView blob
    // that symbol_engine/codeview.py reads.
    std::optional<CodeView> debug_codeview() const;
    // One little-endian value at a file offset ('B' 'H' 'I' 'Q'), or 0 when it lies outside the file.
    uint64_t _u(const py::str& fmt, std::optional<int64_t> off) const;
    // The zero-terminated string at a file offset ('' when the offset is not in the file).
    py::str _text(std::optional<int64_t> off, int64_t limit = 512) const;
    // File offset of an RVA. One that lies in the headers, before any section, is its own
    // offset; one that lies nowhere is None.
    std::optional<int64_t> file_offset(int64_t rva) const;
    // The optional header's fields by name (PE32 and PE32+).
    OptionalHeader optional_header() const;
    // (rva, size) of one data directory; (0, 0) when the image has none at that index.
    std::tuple<int64_t, int64_t> directory(int64_t index) const;
    // {'module', 'ordinal_base', 'timestamp', 'symbols': [{name, ordinal, hint, rva, forward}]}.
    // `hint` is the name's index in the name table (-1 for an export by ordinal only);
    // `forward` is 'OTHER.Function' when the export is passed on to another module.
    ExportTable export_table() const;
    // [{'module', 'symbols': [{name, ordinal, hint, slot}]}] from the import directory, or from
    // the delay-load directory. An import by ordinal has name '' and hint -1; one by name has
    // ordinal -1. `slot` is the RVA of the address-table entry the loader fills in.
    py::list<ImportModule> import_table(bool delay = false) const;
    // [{'type': name or number, 'id', 'items': [{name, id, language, rva, offset, size}]}] --
    // the resource directory's three levels (type, name, language) flattened to one list of
    // items per type. `id` is None for a type or item that goes by a name.
    py::list<ResourceType> resource_tree() const;
    // [(page_rva, entries)] -- the blocks of the base relocation directory.
    py::list<std::tuple<int64_t, int64_t>> reloc_blocks() const;
    // The thread-local storage directory's addresses, or None.
    std::optional<Tls> tls() const;
    // [{type, timestamp, size, offset}] -- every record of the debug directory (`type` is the
    // IMAGE_DEBUG_TYPE number).
    py::list<DebugEntry> debug_entries() const;
    // [{length, revision, type}] -- the attribute certificates (this directory holds a FILE
    // OFFSET, not an RVA).
    py::list<Certificate> certificates() const;
    // The .NET runtime header, or None for a native image.
    std::optional<Clr> clr() const;
    // The linker's 'Rich' record between the DOS stub and the PE header: {key, entries:
    // [(product, build, count)]}, or None when the image has none.
    std::optional<Rich> rich() const;
};

PEFile open_pe(const py::str& path);

// Whether the file is a .NET assembly: a PE image with a CLR runtime header (data directory
// 14). Reads the headers only.
bool is_dotnet(const py::str& path);

}  // namespace codebreaker::formats::pe
