// codebreaker/recompile/disassemble.hpp -- typed C++ port of codebreaker.recompile.disassemble.
//
// disassemble.py -- Xbox 360 PowerPC as (mnemonic, operands) text, for the recompiler.
//
//     disassemble::listing(code, base_va)              (va, raw word, mnemonic, operands), every word
//     disassemble::classify(code, base_va, entries)    offsets flow reaches from `entries`: code
//     disassemble::code_section(path)                  (bytes, address, entry) of an image's code
//
//     python -m codebreaker.recompile.disassemble <image or .xex> [--limit N]
//
// A word is read as VMX128 first (codebreaker/decompile/vmx128.py), the Xenon's vector set: Capstone reads most
// VMX128 words as other, later instructions (ISA 2.07/3.0, which the Xenon does not have). Everything else is
// Capstone's (one decoder per thread: a handle is not safe to share, and the whole-image recompile runs on a
// thread pool). The vector builders read VMX128 operands in that table's order. Every instruction is one word,
// four-byte aligned, so there is no boundary to recover: code is what flow reaches, the rest is data.
//
// Typing notes:
//   * `platforms.decoder(32, arch="ppc", detail=True)` (no port of codebreaker.platforms) is the capstone.dll
//     Python's `capstone` package carries, loaded by path: hand it to set_capstone_dll() once (as analyze/native
//     does), one handle per thread (big-endian 32-bit PowerPC, detail on). Without a DLL (or one that does not
//     load) Capstone is "not installed", as `import capstone` failing is in the Python: `_md()` is then false.
//     The .cpp includes capstone's own headers (the ones that package ships).
//   * listing() is a Python generator; here it returns the whole list (the words from `start`, `count` at most:
//     as the Python, a count of 0 or less still gives the first word). A row is (address, raw word, mnemonic,
//     operands).
//   * Python's ctypes-free `capstone.CsInsn` is Insn here: the address, mnemonic and operand text, whether it
//     is in the jump or call group, and the values of its immediate operands.
//   * image() returns (flat image, base, entry, sections [(name, address, size, executable)]); a PowerPC image
//     is formats::powerpc_image::Image, a .xex is read through its basefile.
//   * main(argv) takes sys.argv[1:] and returns the exit code (2 for no arguments or no Capstone); it prints
//     with py::print. Without a `--limit` argument 60 words are listed.
//   * not ported: the `__name__ == "__main__"` block (`sys.exit(main(sys.argv[1:]))`).
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::recompile::disassemble {

inline constexpr int64_t WORD = 4;

// flow does not go on past these; calls (bl bla bcl bctrl bclrl) come back
extern const py::set<py::str> _ENDERS;

// Names the capstone.dll to decode with (the one Python's capstone package loads).
void set_capstone_dll(const py::str& path);

// This thread's decoder is there (Capstone is installed and loads).
bool _md();

// One instruction as Capstone decodes it (detail on).
struct Insn {
    int64_t address = 0;
    py::str mnemonic;
    py::str op_str;
    bool jump = false;            // ins.group(CS_GRP_JUMP)
    bool call = false;            // ins.group(CS_GRP_CALL)
    py::list<int64_t> imm;        // the operands of type PPC_OP_IMM, their values, in order
};

// A word in VMX128's primary opcodes (4, 5, 6) -- a vector instruction, not data.
bool is_vmx128(const py::bytes& raw4);

// md.disasm(code[off:off+4], vaddr+off, count=1): the instruction, or None (no decoder, a word short of the
// end, or one Capstone does not read)
std::optional<Insn> _decode_one(const py::bytes& code, int64_t off, int64_t vaddr);

// The byte offsets of `code` that flow reaches from `entries` (addresses).
py::set<int64_t> classify(const py::bytes& code, int64_t base_vaddr, const py::list<int64_t>& entries);

// [(start, end)] -- the runs of offsets flow never reaches.
py::list<std::tuple<int64_t, int64_t>> data_spans(const py::bytes& code, int64_t base_vaddr, const py::list<int64_t>& entries,
                                                  int64_t minrun = WORD);

// (address, raw word, mnemonic, operands) for every word from `start`, `count` at most. A word neither decoder
// reads is `.long 0x...`.
py::list<std::tuple<int64_t, py::bytes, py::str, py::str>> listing(const py::bytes& code, int64_t base_vaddr, int64_t start = 0,
                                                                  std::optional<int64_t> count = std::nullopt);

// (image bytes, base address, entry, sections [(name, address, size, executable)]) for a PowerPC image or a .xex
// (its basefile).
std::tuple<py::bytes, int64_t, int64_t, py::list<std::tuple<py::str, int64_t, int64_t, bool>>> image(const py::str& path);

// (bytes, address, entry) of the image's code section.
std::tuple<py::bytes, int64_t, int64_t> code_section(const py::str& path);

int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::recompile::disassemble
