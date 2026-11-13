// cb/source/pe_writer.hpp -- a 32-bit Windows program as assembly source that builds back into it, byte for byte.
//
//     pe_writer::Result r = pe_writer::reconstruct("lz32.dll", "out", {.nasm = nasm});
//     r.source, r.image, r.same, r.instructions, r.raw, r.rounds
//
// The whole file is one NASM source assembled flat (-f bin): one section for the headers and one for every section of the image, each
// placed at its file offset (start=) and at its address (vstart=), so a label's value is the address the program sees. Nothing the loader
// patches is a number: every place the base-relocation table names is written as the label it points at, the import table's addresses
// are written as `label - IMAGE`, and the entry point as the label of the entry. Calls and jumps go to labels.
//
// Code is what is reached from the entry point, from the starts of functions, and from every relocated pointer into code (function
// pointers, the tables of a switch), each path stopping at an instruction that would cut through a relocated place; what is not reached
// is data. An instruction is written as its mnemonic only where NASM gives the original's bytes back (checked every round from the
// assembler's map, source/flat_rounds); one that differs, or that the assembler rejects, is written as its bytes with its mnemonic
// beside it. The DOS program in front of the headers (the stub) is the original's bytes, or comes from its own source when that builds
// back (`Options::find_stub`, source/stub.hpp).
#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/formats/pe.hpp"
#include "cb/platforms/decoder.hpp"
#include "cb/source/flat_rounds.hpp"
#include "cb/source/stub.hpp"
#include "cb/source/x86_flat.hpp"
#include "cb/translate/l2.hpp"

namespace cb::source::pe_writer {

// An address in the program. It is wider than the processor's 32 bits only so that the sum of the image base and a 32-bit field (an
// import table's RVA) is never wrapped into another address.
using Va = std::uint64_t;

struct Options {
    std::filesystem::path nasm;                         // the assembler (settings: nasm); NotFoundError when it is not there
    Stub stub = SearchStub{};
    FindStub find_stub;                                 // used for SearchStub; none: the stub's bytes are kept
    std::function<void(std::string_view)> say;          // progress, one line a round
};

// A section of the image: what the file says of it, and the writer's own facts.
struct Section {
    std::string name;                // its name in the file
    std::uint32_t virtual_size = 0, virtual_address = 0, raw_size = 0, raw_ptr = 0, characteristics = 0;
    Va va = 0;                       // where it is in memory (the image base and its address)
    Va extent = 0;                   // how far it reaches in memory: to the next section
    bool stored = false;             // it has bytes in the file
    bool code = false;
    std::string nasm_name;           // its name in the source
};

// The program, read and decoded, as the file written whole. Its rounds are flat_rounds::flat's. An instruction is named by its address.
class Image final : public flat_rounds::FlatImage<Va> {
public:
    // `file_name` is how the source names the program. FormatError when `pe` has no sections or lies about its tables.
    Image(formats::pe::Image pe, std::string file_name);
    static Image open(const std::filesystem::path& program);

    // The DOS program in front of the headers: assembled from its source at `stub` (included by NASM), instead of the bytes kept.
    void set_stub(std::optional<std::filesystem::path> stub) { stub_ = std::move(stub); }

    const formats::pe::Image& pe() const { return pe_; }
    Va image_base() const { return base_; }
    std::span<const Section> sections() const { return sections_; }
    std::size_t instruction_count() const { return insns_.size(); }
    // The instructions found as code, by address.
    const std::map<Va, x86_flat::Insn>& instructions() const { return insns_; }
    // Where each function begins: the entry and the functions found, and every place a call lands, where an instruction was found.
    const std::set<Va>& function_starts() const { return starts_; }
    // The places the base-relocation table names: the address of each relocated dword, and the address it holds.
    const std::map<Va, std::uint32_t>& relocated() const { return sites_; }
    // The last writing, as one L2 tree (each function and section a fragment); build the document with `assembly().document(arena)`.
    const translate::l2::Assembly& assembly() const { return assembly_; }

    flat_rounds::Source<Va> write(const std::set<Va>& raw, bool final) override;
    std::set<Va> differing(ByteView built, const flat_rounds::LabelMap& labels, const std::set<Va>& raw) override;

    // The file's text with `raw` written as bytes. `mark_all`: a label on every instruction (the rounds need them to find where each went).
    flat_rounds::Source<Va> source(const std::set<Va>& raw, bool mark_all);

private:
    ByteView data() const { return pe_.data(); }
    ByteView bytes(std::size_t lo, std::size_t hi) const;      // [lo, hi) of the file, cut to its end
    const Section* section_of(Va va) const;
    std::optional<std::size_t> offset(Va va) const;            // the file offset of an address in a section's file part (or the headers)
    std::optional<Va> va_of_offset(std::size_t offset) const;
    bool in_code(Va va) const;
    std::map<std::size_t, std::uint32_t> import_rva_fields() const;

    void decode_code();
    bool fits(const x86_flat::Insn& insn) const;
    std::set<Va> wanted() const;
    std::string name(Va va) const;                              // the name an address would have
    std::string ref(Va va) const;                               // how the source spells an address
    std::vector<std::string> text(const x86_flat::Insn& insn, bool raw) const;
    std::optional<std::string> nasm_text(const x86_flat::Insn& insn, const std::optional<std::uint32_t>& target,
                                         const std::vector<std::uint32_t>& inside) const;
    void write_nobits(translate::l2::Assembly& out, const Section& s) const;
    void write_data(translate::l2::Assembly& out, Va from, Va to, bool header) const;

    formats::pe::Image pe_;
    std::string file_name_;
    Va base_ = 0;
    std::uint32_t headers_ = 0;                                  // the size of the headers, in the file and in memory
    std::size_t tail_ = 0;                                       // bytes after the last section
    std::vector<Section> sections_;
    std::map<Va, std::uint32_t> sites_;                          // relocated dword -> the address it holds
    std::unordered_map<Va, std::string> imports_;                // an import address table slot -> its label (looked up only)
    std::map<std::size_t, std::uint32_t> rva_fields_;            // file offset of an RVA field of the import directory -> the RVA
    std::optional<std::filesystem::path> stub_;
    platforms::DecoderSet decoders_;
    std::map<Va, x86_flat::Insn> insns_;
    std::unordered_map<Va, Va> covered_;                         // every byte of an instruction -> where its instruction starts (looked up only)
    std::set<Va> starts_;
    std::map<Va, std::string> placed_;                           // the labelled addresses of the last writing
    translate::l2::Assembly assembly_;
};

struct Result {
    std::filesystem::path source;
    std::optional<std::filesystem::path> image;   // the file built
    bool same = false;                            // it is the original
    std::size_t rounds = 0;
    std::size_t raw = 0;                          // instructions written as bytes
    std::string log;
    std::size_t instructions = 0;

    flat_rounds::FlatReport report() const;
};

// Write `program` as <outdir>/<name>.asm (name: its file name in lower case), assembled with NASM, rounds until nothing differs.
// NotFoundError when there is no assembler; FormatError for a program that is not a PE file; ToolError when NASM rejects what is not the
// writer's to blame.
Result reconstruct(const std::filesystem::path& program, const std::filesystem::path& outdir, const Options& options);

// What the command `pe-source <program> [--out DIR] [--stub SOURCE.ASM] [--keep-stub]` needs of its surroundings.
struct Environment {
    std::filesystem::path nasm;
    std::filesystem::path state;     // the default out folder is <state>/pe_source/<program name>
    FindStub find_stub;
};
// The command line without the command's name; `say` takes every line printed. -> the exit code (2: no program named).
int command(std::span<const std::string> args, const Environment& env, const std::function<void(std::string_view)>& say);

// The command's usage text.
std::string_view usage();

}  // namespace cb::source::pe_writer
