// codebreaker/source/pe_writer.hpp -- typed C++ of codebreaker.source.pe_writer
//
// pe_writer.py -- a 32-bit Windows program as assembly source that builds back into it, byte for byte.
//
//     python -m codebreaker pe-source <program> [--out DIR] [--stub SOURCE.ASM]
//
//     Result r = pe_writer::reconstruct(path, outdir);
//     r.source, r.image, r.same, r.instructions, r.raw, r.rounds
//
// The whole file is one NASM source assembled flat (-f bin): one section for the headers and one
// for every section of the image, each placed at its file offset (start=) and at its address
// (vstart=), so a label's value is the address the program sees. Nothing the loader patches is a
// number: every place the base-relocation table names is written as the label it points at, the
// import table's addresses are written as `label - IMAGE`, and the entry point as the label of the
// entry. Calls and jumps go to labels.
//
// Code is what is reached from the entry point, from the starts of functions, and from every
// relocated pointer into code (function pointers, the tables of a switch), each path stopping at
// an instruction that would cut through a relocated place; what is not reached is data.
//
// An instruction is written as its mnemonic only where the assembler turns that text back into the
// original's bytes: every round assembles the whole file, reads where each instruction went (the
// assembler's map of labels) and compares its bytes; one that differs, or that the assembler
// rejects, is written as its bytes (with its mnemonic beside it) in the next round, until nothing
// differs. The DOS program in front of the headers (a stub) is either kept as the original's bytes
// or, given --stub, assembled from its own source (dos-source) and included.
//
// Typing notes (how the Python's dynamic parts are typed here):
//   * An instruction is a capstone CsInsn with detail: `Insn` below holds what the three whole-file writers (and
//     dos_writer) read of it. `decode` is `next(md.disasm(code, address, 1), None)` on platforms.decoder(bits,
//     detail=True): the very capstone.dll the Python package carries, loaded by emulate/capstone_x86.
//   * The sections are `Sec` records (the PE's section dict with the writer's own keys added); the Python's
//     dicts keyed by an address are py::dict<int64_t, ...>.
//   * `reconstruct`'s `stub` argument is None (look for the program's DOS stub's source), False (keep its bytes) or a
//     path: a std::variant<std::monostate, bool, py::str>. The Python's `_stub(w, outdir, say)` took the writer;
//     ne_writer passed a stand-in object with only `.d` and `.pe.e_lfanew`, so here it takes those two.
//   * The result of reconstruct is the Python's dict as a record (`Result`).
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>
#include <variant>
#include <vector>

#include "codebreaker/formats/pe.hpp"
#include "codebreaker/source/image_writer.hpp"
#include "codebreaker/translate/l2.hpp"
#include "py.hpp"

namespace codebreaker::source::pe_writer {

namespace pe = codebreaker::formats::pe;

// One instruction as capstone decodes it (with detail): the fields the writers read.
struct Insn {
    int64_t address = 0;
    int64_t size = 0;
    py::str mnemonic;
    py::str op_str;
    py::bytes bytes;
    int64_t imm_size = 0, imm_offset = 0, disp_size = 0, disp_offset = 0;
    py::list<int64_t> prefix;                // the four prefix bytes
    py::list<int64_t> groups;                // capstone's instruction groups (X86_GRP_*)
    int64_t op_count = 0;
    int64_t op0_type = 0;                    // operands[0].type and .imm, when there is one
    int64_t op0_imm = 0;
};

// next(md.disasm(code, address, 1), None): the first instruction of `code` as the x86 decoder of `bits` reads it.
std::optional<Insn> decode(int64_t bits, const py::bytes& code, int64_t address);

// A branch, call or loop (the Python's _BRANCH pattern, `^(j\w+|call|loop\w*|jecxz|jcxz)$`).
bool is_branch(const py::str& mnemonic);

// The target a relative branch names as its operand (None for any other instruction).
std::optional<int64_t> branch_target(const Insn& i);

extern const py::set<py::str> _STOP;
extern const py::set<int64_t> _SEGS;
extern const py::set<py::str> _ARITH;
extern const py::dict<py::str, int64_t> _RR;
// _RR as a construct-on-first-use value: another file's static initialisation (the NE and LE writers' _PRELUDE) may run
// before this file's globals exist.
const py::dict<py::str, int64_t>& RR();
extern const py::list<py::str> _PRELUDE;

// A section of the image: the PE's own fields, and the writer's.
struct Sec {
    py::str name;
    int64_t vsize = 0, vaddr = 0, rawsize = 0, rawptr = 0, chars = 0;
    int64_t va = 0;           // where it is in memory (image base + vaddr)
    bool file = false;        // stored in the file
    int64_t extent = 0;       // how far it reaches in memory (to the next section)
    bool code = false;
    py::str nasm;             // its name in the source
};

class Writer {
  public:
    py::str path;
    pe::PEFile pe;
    py::bytes d;
    int64_t B = 0;
    int64_t headers = 0;
    std::vector<Sec> secs;
    py::dict<int64_t, int64_t> sites;               // VA of a relocated dword -> the address it holds
    py::dict<int64_t, py::str> imports;             // IAT slot VA -> label
    py::dict<int64_t, int64_t> rva_fields;          // file offset of an RVA field -> the RVA
    int64_t tail = 0;
    py::dict<int64_t, Insn> insns;                  // VA -> instruction
    py::dict<int64_t, int64_t> covered;             // VA of every byte of an instruction -> its start
    py::set<int64_t> starts;
    py::dict<int64_t, py::str> placed;
    translate::l2::Dict l2;

    explicit Writer(const py::str& path);

    static py::str _secname(const py::str& name, int64_t i);

    // what is where
    const Sec* section_of(int64_t va) const;
    // File offset of an address in a section's file part, or None.
    std::optional<int64_t> offset(int64_t va) const;
    // The RVA fields of the import directory (descriptors, lookup and address tables).
    py::dict<int64_t, int64_t> _import_rvas() const;

    // code
    void _decode();
    // Every relocated place inside the instruction is its whole 4-byte displacement or immediate.
    bool _fits(const Insn& i) const;
    static std::optional<int64_t> _target(const Insn& i);

    // names
    // The addresses that need a label: what relocated places, import fields and branches name.
    py::set<int64_t> wanted() const;
    py::str name(int64_t va) const;
    // How the source spells an address: its label, a label plus a distance, or IMAGE plus one.
    py::str ref(int64_t va) const;

    // source
    // An instruction's lines: its mnemonic in NASM's spelling, or its bytes with the relocated places and
    // branch distances as expressions (raw).
    py::list<py::str> text(const Insn& i, bool raw) const;
    std::optional<py::str> _nasm(const Insn& i, const std::optional<int64_t>& tgt, const py::list<int64_t>& sites) const;
    // The file's text. -> (text, {line number: instruction VA})
    std::tuple<py::str, py::dict<int64_t, int64_t>> source(const py::set<int64_t>& raw = py::set<int64_t>(), bool mark_all = true,
                                                           const std::optional<py::str>& stub = std::nullopt);
    void _bss(image_writer::Assembly& L, const Sec& s) const;
    // Bytes from a to end: labels where they stand, relocated places and RVA fields as expressions, the rest as db lines.
    void _data(image_writer::Assembly& L, int64_t a, int64_t end, bool header = false) const;
    std::optional<int64_t> _va_of_offset(int64_t o) const;
};

// The instructions the assembler did not give back: compared where each one went, its length (to the next
// instruction's label) and its bytes, outside the places whose value depends on where things are (relocated
// addresses, branch distances).
py::set<int64_t> _differing(const Writer& w, const py::bytes& data, const py::dict<py::str, std::tuple<int64_t, int64_t>>& labels,
                            const py::set<int64_t>& raw);

// The DOS program in front of the headers, from its own source (dos-source) when that comes back as the
// original's bytes; None (its bytes kept) when it does not, or when it is only the 128-byte "cannot be run
// in DOS mode" kind too small to be worth it. `d` is the file, `e_lfanew` where its new header is.
std::optional<py::str> _stub(const py::bytes& d, int64_t e_lfanew, const py::str& outdir, const std::function<void(const py::str&)>& say);

// What reconstruct gives (the Python's dict: source, image, same, rounds, raw, log, instructions).
struct Result {
    py::str source;
    std::optional<py::str> image;
    bool same = false;
    int64_t rounds = 0;
    int64_t raw = 0;             // how many instructions are written as bytes
    py::str log;
    int64_t instructions = 0;

    image_writer::FlatReport report() const;
};

using StubArg = std::variant<std::monostate, bool, py::str>;   // None, False, or the stub's source

Result reconstruct(const py::str& path, const py::str& outdir, const StubArg& stub = std::monostate(),
                   const std::function<void(const py::str&)>& log = nullptr);

// python -m codebreaker pe-source <program> [--out DIR] [--stub FILE]
int64_t main(const py::list<py::str>& argv);

extern const py::str DOC;

}  // namespace codebreaker::source::pe_writer
