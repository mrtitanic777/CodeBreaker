// cb/formats/pe.hpp -- a PE image (PE32 and PE32+): headers, sections and the tables of the data directories.
//
// This is the one PE reader. An `Image` owns the file's bytes and the parsed headers; everything else is read
// from them on demand. Two kinds of reader sit side by side:
//   * the loader's (exports, imports, base_relocs, module_name, debug_codeview) read the file as the loader
//     would and throw FormatError/TruncatedError on a damaged table;
//   * the inspector's (export_table, import_table, resource_tree, reloc_blocks, tls, debug_entries,
//     certificates, clr, rich) check every offset and give a shorter answer on a damaged image, never throw.
// Text from the image (names) is decoded from latin1 to UTF-8 once, here.
#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"

namespace cb::formats::pe {

inline constexpr std::uint16_t kMachineI386 = 0x014C;
inline constexpr std::uint16_t kMachineX64 = 0x8664;
inline constexpr std::uint32_t kScnCntCode = 0x00000020;
inline constexpr std::uint32_t kScnMemExecute = 0x20000000;

struct Section {
    std::string name;   // the 8-byte name with its trailing zeros cut
    std::uint32_t virtual_size = 0, virtual_address = 0, raw_size = 0, raw_ptr = 0, characteristics = 0;
    bool is_code() const { return (characteristics & (kScnCntCode | kScnMemExecute)) != 0; }
};

struct DataDirectory {
    std::uint32_t rva = 0, size = 0;   // the certificate directory holds a FILE OFFSET in `rva`
};

// The headers' facts the loader needs.
struct Header {
    std::uint32_t e_lfanew = 0;
    std::uint16_t machine = 0, n_sections = 0, opt_size = 0, characteristics = 0, magic = 0;
    std::uint32_t timestamp = 0, entry_rva = 0, n_datadirs = 0;
    bool is_plus = false;   // PE32+ (64-bit)
    std::uint64_t image_base = 0;
    bool is_dll() const { return (characteristics & 0x2000) != 0; }
};

struct Version {
    std::uint16_t major = 0, minor = 0;
};

// Every field of the optional header (PE32 and PE32+). Tolerant: a field outside the file is 0.
struct OptionalHeader {
    std::uint16_t magic = 0;
    std::uint8_t linker_major = 0, linker_minor = 0;
    std::uint32_t code_size = 0, initialized_data_size = 0, uninitialized_data_size = 0, entry_point = 0, code_base = 0;
    std::uint64_t image_base = 0;
    std::uint32_t section_alignment = 0, file_alignment = 0;
    Version os_version, image_version, subsystem_version;
    std::uint32_t image_size = 0, headers_size = 0, checksum = 0;
    std::uint16_t subsystem = 0, dll_characteristics = 0;
};

// An entry of the export directory as the loader sees it (zero rvas dropped).
struct Export {
    std::uint64_t ordinal = 0;        // ordinal base + index
    std::optional<std::string> name;  // absent: exported by ordinal only
    std::uint32_t rva = 0;
};

// An import-address-table slot: the address it sits at, and what the loader fills it with.
struct Import {
    std::uint64_t slot_va = 0;
    std::string dll;
    std::string name;  // the symbol, "#<ordinal>" for an import by ordinal, "?" when unreadable
};

struct CodeView {
    enum class Kind { Rsds, Nb10 } kind = Kind::Rsds;
    std::string guid;              // RSDS: "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX"
    std::uint32_t signature = 0;   // NB10
    std::uint32_t age = 0;
    std::string pdb;
};

struct ExportSymbol {
    std::optional<std::string> name;     // absent: by ordinal only
    std::uint64_t ordinal = 0;
    std::optional<std::uint32_t> hint;   // the name's index in the name table
    std::uint32_t rva = 0;
    std::optional<std::string> forward;  // "OTHER.Function" when passed on to another module
};
struct ExportTable {
    std::string module;
    std::uint32_t ordinal_base = 0, timestamp = 0;
    std::vector<ExportSymbol> symbols;
};

struct ImportSymbol {
    std::optional<std::string> name;       // absent: by ordinal
    std::optional<std::uint16_t> ordinal;  // by-ordinal imports
    std::optional<std::uint32_t> hint;     // by-name imports
    std::uint64_t slot_rva = 0;            // RVA of the address-table entry the loader fills in
};
struct ImportModule {
    std::string module;
    std::vector<ImportSymbol> symbols;
};

struct ResourceItem {
    std::optional<std::string> name;  // a named item; else `id`
    std::optional<std::uint32_t> id;
    std::uint32_t language = 0;
    std::uint32_t rva = 0;
    std::optional<std::uint64_t> file_offset;
    std::uint32_t size = 0;
};
struct ResourceType {
    std::optional<std::string> name;  // a named type; else `id` (the RT_* number)
    std::optional<std::uint32_t> id;
    std::vector<ResourceItem> items;  // the name and language levels, flattened
};

struct RelocBlock {
    std::uint32_t page_rva = 0, entries = 0;
};
struct Tls {
    std::uint64_t start_va = 0, end_va = 0, index_va = 0, callbacks_va = 0;
};
struct DebugEntry {
    std::uint32_t type = 0, timestamp = 0, size = 0, offset = 0;
};
struct Certificate {
    std::uint32_t length = 0;
    std::uint16_t revision = 0, type = 0;
};
struct Clr {
    Version runtime;
    std::uint32_t flags = 0, entry_token = 0;
};
struct RichEntry {
    std::uint32_t product = 0, build = 0, count = 0;
};
struct Rich {
    std::uint32_t key = 0;
    std::vector<RichEntry> entries;
};

class Image {
public:
    // Parse the headers. FormatError when it is not a PE image, TruncatedError when its headers are cut short.
    // `name` (a path) only goes into the messages.
    static Image parse(Bytes data, std::string_view name = "image");
    static Image parse(ByteView data, std::string_view name = "image") {
        return parse(Bytes(data.begin(), data.end()), name);
    }
    static Image open(const std::filesystem::path& file);
    // For probing ("is this a PE?"): nullopt instead of an error.
    static std::optional<Image> try_parse(ByteView data);

    ByteView data() const { return data_; }
    const Header& header() const { return header_; }
    std::span<const DataDirectory> data_directories() const { return datadirs_; }
    std::span<const Section> sections() const { return sections_; }

    std::string machine_name() const;  // "i386", "x64", "arm", "arm64", else "0x%04x"
    OptionalHeader optional_header() const;
    // image_base + entry rva; nullopt when the entry point is 0.
    std::optional<std::uint64_t> entry_va() const;

    // ---- addressing -------------------------------------------------------------------------------------
    // File offset of an RVA that lies in a section, else nullopt.
    std::optional<std::uint64_t> rva_to_offset(std::uint64_t rva) const;
    // The same, also taking the headers (an RVA below SizeOfHeaders is its own offset); nullopt outside the file.
    std::optional<std::uint64_t> file_offset(std::uint64_t rva) const;
    // The raw bytes of a section as stored (clamped to the file).
    ByteView section_data(const Section& s) const;
    // `len` bytes at an RVA, if the whole range lies inside the file.
    std::optional<ByteView> read_rva(std::uint64_t rva, std::uint64_t len) const;
    std::uint64_t code_bytes() const;  // sum of the raw sizes of the code sections
    struct CodeSection {
        std::uint64_t va;
        const Section* section;
    };
    std::vector<CodeSection> code_sections() const;
    DataDirectory directory(std::size_t index) const;  // {0,0} when the image has none at that index

    // ---- the loader's readers (throw on a damaged table) --------------------------------------------------
    std::string module_name() const;  // the export directory's internal name, "" if none
    std::vector<Export> exports() const;
    // The import address table: slot address -> imported API, in first-appearance order (a slot met again keeps
    // its place and takes the later value).
    std::vector<Import> imports() const;
    // The RVAs of every absolute 32-bit address (HIGHLOW) the base relocation directory lists, ascending, each once.
    std::vector<std::uint64_t> base_relocs() const;
    std::optional<CodeView> debug_codeview() const;

    // ---- the inspector's readers (never throw) -------------------------------------------------------------
    ExportTable export_table() const;
    std::vector<ImportModule> import_table(bool delay_load) const;
    std::vector<ResourceType> resource_tree() const;
    std::vector<RelocBlock> reloc_blocks() const;
    std::optional<Tls> tls() const;
    std::vector<DebugEntry> debug_entries() const;
    std::vector<Certificate> certificates() const;
    std::optional<Clr> clr() const;
    std::optional<Rich> rich() const;
    bool is_dotnet() const;  // has a CLR runtime header

private:
    Image() = default;
    std::uint64_t u8_at(std::uint64_t off) const;
    std::uint64_t u16_at(std::uint64_t off) const;
    std::uint64_t u32_at(std::uint64_t off) const;
    std::uint64_t word_at(std::uint64_t off) const;  // pointer-sized: 4 bytes in PE32, 8 in PE32+
    std::string text_at(std::optional<std::uint64_t> off, std::uint64_t limit = 512) const;
    std::optional<std::uint64_t> directory_offset(std::size_t index) const;

    Bytes data_;
    Header header_;
    std::vector<DataDirectory> datadirs_;
    std::vector<Section> sections_;
};

// "i386", "x64", "arm", "arm64"; nullptr when the machine is not named.
const char* machine_label(std::uint16_t machine);
// "native", "GUI", "console", "OS/2", "POSIX", "Windows CE GUI", "EFI application", "Xbox"; nullptr when not named.
const char* subsystem_label(std::uint32_t subsystem);

// Whether the file is a .NET assembly (a CLR runtime header in data directory 14); false for anything unreadable.
// Maps the file instead of reading it, so a big file costs nothing.
bool is_dotnet_file(const std::filesystem::path& file);

}  // namespace cb::formats::pe
