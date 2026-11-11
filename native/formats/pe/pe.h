// pe.h -- a PE image (32-bit and PE32+): headers, sections, and the data directories' tables.
//
// The C++ form of codebreaker/formats/pe.py's PEFile, fact for fact.  Two kinds of reader sit
// side by side, as they do there: the loader's (exports, imports, base_relocs, module_name,
// debug_codeview), which read the file as the loader would and raise on a damaged table, and
// the inspector's (export_table, import_table, resource_tree, ...), which check every offset and
// give a shorter answer instead.  Both keep the Python's arithmetic and its order of reads, so a
// damaged image fails here exactly where it failed there.
#pragma once

#include <cstdint>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "../core/bytes.h"

namespace cbf::pe {

constexpr uint16_t kMachineI386 = 0x014C;
constexpr uint32_t kScnCntCode = 0x00000020;
constexpr uint32_t kScnMemExecute = 0x20000000;

struct Section {
    Bytes name;          // the 8-byte name with its trailing zeros cut (latin1 when shown)
    uint32_t vsize, vaddr, rawsize, rawptr, chars;
    bool is_code() const { return (chars & (kScnCntCode | kScnMemExecute)) != 0; }
};

struct DataDirectory {
    uint32_t va, size;
};

struct Export {                      // exports(): ordinal, name (absent: by ordinal only), rva
    int64_t ordinal;
    std::optional<Bytes> name;
    uint32_t rva;
};

struct Import {                      // imports(): the IAT slot's address -> (module, symbol)
    uint64_t slot;
    std::string dll, name;           // already as text (latin1 bytes; '#<ordinal>' and '?' as written)
    bool dll_latin1 = true;
};

struct CodeView {                    // debug_codeview()
    std::string format;              // "RSDS" | "NB10"
    std::string guid;                // RSDS
    uint32_t signature = 0;          // NB10
    uint32_t age = 0;
    Bytes pdb;
};

struct ExportSymbol {                // export_table()
    std::string name;                // latin1 text
    int64_t ordinal, hint;
    uint32_t rva;
    std::string forward;
};
struct ExportTable {
    std::string module;
    uint32_t ordinal_base = 0, timestamp = 0;
    std::vector<ExportSymbol> symbols;
};

struct ImportSymbol {                // import_table()
    std::string name;
    int64_t ordinal, hint, slot;
};
struct ImportModule {
    std::string module;
    std::vector<ImportSymbol> symbols;
};

struct ResourceItem {
    std::optional<std::string> name_text;  // a named item's name (UTF-8); else "#<id>"
    std::optional<uint32_t> id;            // None for a named item
    uint32_t language;
    uint32_t rva;
    std::optional<int64_t> offset;
    uint32_t size;
};
struct ResourceType {
    std::optional<std::string> name;       // a named type's name; else the number in `id`
    std::optional<uint32_t> id;
    std::vector<ResourceItem> items;
};

struct Tls {
    uint64_t start_va, end_va, index_va, callbacks_va;
};
struct DebugEntry {
    uint32_t type, timestamp, size, offset;
};
struct Certificate {
    uint32_t length;
    uint16_t revision, type;
};
struct Clr {
    uint16_t major, minor;
    uint32_t flags, entry_token;
};
struct RichEntry {
    uint32_t product, build, count;
};
struct Rich {
    uint32_t key;
    std::vector<RichEntry> entries;
};

class Image {
public:
    // PEFile(path): ValueError for a file that is not a PE image, struct.error for one whose
    // headers are cut short.  `path` is only for the messages.
    Image(Bytes data, const std::string &path);

    Bytes data;
    uint32_t e_lfanew = 0;
    uint16_t machine = 0, n_sections = 0, opt_size = 0, characteristics = 0, magic = 0;
    uint32_t timestamp = 0, entry = 0, n_datadir = 0;
    bool is_plus = false;
    uint64_t image_base = 0;
    std::vector<DataDirectory> datadirs;
    std::vector<Section> sections;

    std::string machine_name() const;
    bool is_dll() const { return (characteristics & 0x2000) != 0; }
    std::optional<int64_t> rva_to_off(int64_t rva) const;
    uint64_t code_bytes() const;
    std::string module_name() const;                         // latin1 text
    std::vector<Export> exports() const;
    std::vector<Import> imports() const;                     // in first-insertion order, last value kept
    std::vector<std::pair<uint64_t, Section>> code_sections() const;
    std::vector<int64_t> base_relocs() const;                // in the order found (a set: duplicates kept once)
    std::optional<CodeView> debug_codeview() const;

    // the inspector's readers: offsets checked, never raise
    uint64_t u(char fmt, std::optional<int64_t> off) const;  // _u: 'B' 'H' 'I' 'Q'
    std::string text(std::optional<int64_t> off, int64_t limit = 512) const;   // _text, latin1
    std::optional<int64_t> file_offset(int64_t rva) const;
    DataDirectory directory(int64_t index) const;
    ExportTable export_table() const;
    std::vector<ImportModule> import_table(bool delay) const;
    std::vector<ResourceType> resource_tree() const;
    std::vector<std::pair<uint32_t, uint32_t>> reloc_blocks() const;
    std::optional<Tls> tls() const;
    std::vector<DebugEntry> debug_entries() const;
    std::vector<Certificate> certificates() const;
    std::optional<Clr> clr() const;
    std::optional<Rich> rich() const;
    DataDirectory clr_directory() const;
};

// _MACHINE / _SUBSYSTEM names
const char *machine_label(uint16_t machine);       // nullptr when not named
const char *subsystem_label(uint32_t subsystem);

// is_dotnet(path): reads the headers only; False for anything unreadable
bool is_dotnet(const std::string &path_utf8);

}  // namespace cbf::pe
