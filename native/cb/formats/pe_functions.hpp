// cb/formats/pe_functions.hpp -- where the functions of a 32-bit (PE) module are, and what they call.
//
// An executable section is swept linearly with Capstone (32-bit x86) and the result is turned into evidence a
// reconstruction can cite and check: function starts, each function's extent, resolved calls, and which function
// reads the address of a string or a global. It is evidence extraction, not understanding: it says where the
// functions are and what they call; a human still reads them.
//
// Function starts come from three sources:
//   1. the export table (RVAs the binary itself declares) and the entry point -- ground truth;
//   2. direct `call <imm>` targets seen in the sweep that lie in a code section -- real entry points;
//   3. standard prologues: `push ebp; mov ebp,esp` (55 8B EC) and the MS hot-patch form
//      `mov edi,edi; push ebp; mov ebp,esp` (8B FF 55 8B EC).
// A function runs from its start to the next start (or the end of its section). Only starts the sweep decoded count;
// an export, the entry point or a call target the sweep stepped over (it began inside another instruction) is decoded
// from there until the sweep's own listing is reached.
//
// Calls resolve through the import address table (`call [IAT slot]` -> `DLL!name`) and the export / known-start set
// (`call <imm>` -> an exported name or a local `sub_<va>`), which is what makes the carve checkable rather than a raw
// listing.
//
// Addresses are 32-bit virtual addresses (`Va`): the module is a PE32 image. Capstone's operand facts (type, immediate,
// memory base/index/displacement, where in the instruction they sit) are kept as the decoder gave them. Decoding needs
// capstone.dll (cb/core/capstone_api.hpp); a missing DLL is a NotFoundError.
#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/ordered_map.hpp"
#include "cb/formats/pe.hpp"

namespace cb::formats::pe_functions {

using Va = std::uint32_t;

enum class OperandKind : std::uint8_t { Invalid, Register, Immediate, Memory };

// One operand of Capstone's x86 detail. `imm` is meaningful for an immediate; `base`, `index` (Capstone register ids,
// 0 = none) and `disp` (signed) for a memory operand.
struct Operand {
    OperandKind kind = OperandKind::Invalid;
    std::int64_t imm = 0;
    std::uint32_t base = 0, index = 0;
    std::int64_t disp = 0;
};

// A decoded instruction. `disp` / `disp_size` / `disp_offset` and `imm_size` / `imm_offset` say where the
// displacement and the immediate sit among the instruction's bytes (Capstone's encoding facts).
struct Instruction {
    Va address = 0;
    std::uint8_t size = 0;
    std::string mnemonic, op_str;
    std::vector<Operand> operands;
    std::int64_t disp = 0;
    std::uint8_t disp_size = 0, disp_offset = 0, imm_size = 0, imm_offset = 0;

    // The immediate of a `call imm`: the call's target.
    std::optional<Va> call_target() const;
};

// One carved function. `name` is the export's name (or "Ord<n>") when the function is exported.
struct Function {
    Va start = 0, end = 0;
    std::optional<std::string> name;
};

// A listing row: the instruction's address (or offset) and its text, "mnemonic operands".
struct ListingRow {
    std::uint32_t at = 0;
    std::string text;
};

// A line of one function's disassembly.
struct DisasmLine {
    Va address = 0;
    std::string mnemonic, op_str;
};

// What an import address table slot holds.
struct ImportName {
    std::string dll, name;
};

// An executable section as loaded: its virtual address, bytes as stored, and name.
struct CodeSection {
    Va va = 0;
    ByteView data;
    std::string name;
    Va end() const { return va + static_cast<Va>(data.size()); }
    bool contains(Va address) const { return address >= va && address < end(); }
};

// The carve of one PE32 module. Built once by `open` / `from_image`; the queries that decode (`instruction_at`,
// `calls_in`, `data_refs` ...) are safe to call from several threads.
class PeCarve {
public:
    // Reads and carves the module. FormatError / TruncatedError when it is not a PE image; UnsupportedError for PE32+.
    static PeCarve open(const std::filesystem::path& file);
    static PeCarve from_image(pe::Image image);
    PeCarve(PeCarve&&) noexcept;
    PeCarve& operator=(PeCarve&&) noexcept;
    ~PeCarve();

    const pe::Image& image() const { return *image_; }
    Va image_base() const { return image_base_; }
    std::span<const CodeSection> sections() const { return sections_; }
    // The section holding an address, or nullptr.
    const CodeSection* section_of(Va address) const;

    // The IAT slot -> import map, in the table's order; and export address -> name (a name given twice for one
    // address: the later one; an export without a name is "Ord<ordinal>").
    const OrderedMap<Va, ImportName>& imports() const { return imports_; }
    const std::map<Va, std::string>& exports() const { return exports_; }

    // Every direct call target the sweep saw (anywhere, not only in code), ascending.
    std::span<const Va> call_targets() const { return call_targets_; }
    // The functions, by start ascending.
    const std::map<Va, Function>& functions() const { return functions_; }

    // The addresses the sweep decoded, ascending; and the instruction at one of them (nullopt when the address was not
    // decoded). The bytes are decoded when asked for.
    std::span<const Va> instruction_addresses() const { return addresses_; }
    bool has_instruction(Va address) const;
    std::optional<Instruction> instruction_at(Va address) const;

    // A call/jmp instruction's destination as a name: an export, `sub_<va>` for a local address, `DLL!name` for an import;
    // nullopt for anything else (an indirect through something that is not an import slot stays unresolved).
    std::optional<std::string> resolve_call(const Instruction& instruction) const;
    // The resolved call/jmp targets inside the function carved at `start`, in code order (empty when there is none).
    std::vector<std::string> calls_in(Va start) const;
    // The function's instructions (empty when there is no function at `start`).
    std::vector<DisasmLine> disassemble(Va start) const;

    // The data edges (resolve_call gives the control-flow ones): which function reads or pushes the address of a string
    // or a global. A `push imm32`, `mov reg,imm32` or absolute `[disp]` whose value lands inside the image is a data
    // reference; a branch's direct target (a code edge) and a recognised IAT slot (an import edge) are not.
    // {function start: the referenced addresses, ascending}.
    const std::map<Va, std::vector<Va>>& data_refs() const;
    // The functions (ascending) that reference the data address.
    std::vector<Va> xrefs_to(Va address) const;
    // {function start: [(address, text)]}: the strings each function references; `text_at` maps an address to its string.
    std::map<Va, std::vector<std::pair<Va, std::string>>> string_refs(
        const std::unordered_map<Va, std::string>& text_at) const;
    // Whether an address lies in one of the image's sections (virtual extent, or raw if larger).
    bool in_image(std::uint64_t address) const;

private:
    PeCarve() = default;
    struct Lazy;

    std::unique_ptr<pe::Image> image_;
    Va image_base_ = 0;
    std::vector<CodeSection> sections_;
    OrderedMap<Va, ImportName> imports_;
    std::map<Va, std::string> exports_;
    std::vector<Va> call_targets_, addresses_;
    std::map<Va, Function> functions_;
    std::unique_ptr<Lazy> lazy_;
};

// One PeCarve per module file, kept: carving sweeps the whole module, and several callers want the same one.
class CarveCache {
public:
    std::shared_ptr<const PeCarve> get(const std::filesystem::path& file);

private:
    std::mutex mutex_;
    std::map<std::string, std::shared_ptr<const PeCarve>> kept_;
};

// The functions PeCarve finds, with the same answer, without keeping anything for every instruction of the module.
std::vector<Function> function_spans(const pe::Image& image);

// Linearly decodes the blob as 32-bit code; one row per instruction with CONTIGUOUS coverage (a byte that does not
// decode becomes a 1-byte "(bad)" row, which the byte-exact reconcile turns into `db`). Capstone is used because
// objdump's linear sweep splits prefixed instructions (a segment or operand-size prefix is peeled onto its own row),
// which that reconcile cannot realign; Capstone decodes each prefixed instruction as one unit. Rows are at
// `base + offset`.
std::vector<ListingRow> disassemble_listing(ByteView code, Va base = 0);

// One PE executable section, shaped like an NE segment for the assembly reconciler: the listing of its bytes, the
// functions inside it (offsets relative to the section), and which bytes a base relocation covers.
struct PeSeg {
    std::string module;                    // the name it was asked for
    std::filesystem::path path;
    Va base = 0;                           // image base
    Va va = 0;                             // the section's address
    Bytes code;
    std::string name;
    std::vector<ListingRow> rows;          // offsets from 0
    std::unordered_map<std::uint32_t, std::size_t> row_at;   // offset -> index in `rows`
    std::map<std::uint32_t, Function> funcs;                 // the carve's functions inside the section, offsets
    std::map<std::uint32_t, std::string> export_names;       // offset -> exported name
    std::vector<bool> reloc_mask;          // a base relocation covers the byte (HIGHLOW: 4 bytes each)
    std::map<std::uint32_t, std::string> fixups;   // resolved call sites, offset -> "DLL!name"; filled by the annotator
    std::shared_ptr<const PeCarve> carve;  // kept for call resolution

    // The section named `section`; by default the ".text" section, else the largest. `carve`: a carve to reuse (a
    // multi-section module is otherwise swept again for every section). FormatError when the module has no executable
    // section, or none of that name.
    static PeSeg open(const std::filesystem::path& file, std::string_view module,
                      const std::optional<std::string>& section = std::nullopt,
                      std::shared_ptr<const PeCarve> carve = nullptr);
};

}  // namespace cb::formats::pe_functions
