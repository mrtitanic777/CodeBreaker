// codebreaker/formats/xex_edit.hpp -- typed C++ from codebreaker.formats.xex_edit.
//
// xex_edit.py -- change an Xbox 360 executable: its privilege flags, or its code at an address.
//
//     codebreaker xex-edit privileges <in.xex>                         the privilege bits that are set
//     codebreaker xex-edit privileges <in.xex> <+bit|-bit ...> <out>   set (+) or clear (-) bits; `pal50`
//                                                                      names bit 10, PAL-50 incompatible
//     codebreaker xex-edit nop   <in.xex> <address> [count] <out>      count instructions become nop
//     codebreaker xex-edit bytes <in.xex> <address> <hex> <out>        raw bytes over the code
//     codebreaker xex-edit asm   <in.xex> <address> "<asm>" <out>      PowerPC assembled there
//     codebreaker xex-edit hook  <in.xex> <address> <cave> "<asm>" <out>
//                                                                      `b cave` at address; at the cave
//                                                                      the asm, the displaced instruction,
//                                                                      and `b address+4`
//     codebreaker xex-edit asm-only "<asm>" [address]                  the bytes, nothing written
//
// Privileges live in the header's privilege entries (key 0x30000: bits 0-31; 0x30100: bits 32-63),
// changed where they stand. A code edit writes the module as an unencrypted XEX holding the whole
// image in one block (xex.basic) and changes the image there; that is what the recompiler, the Xbox
// 360 runtime and emulators read. Several asm lines are separated by `;` or newlines. The header's
// hashes and signature are not redone: `codebreaker xex compress` / `machine` re-pack and re-sign.
// Only bit 10 is named here: the meaning of the other bits is not settled from a source this
// project trusts, so they are given by number.
//
// Typing notes:
//   * privileges / set_privileges are formats.dll's (native/formats/win/xex_edit.cc, linked in and asked through cbf_call).
//     The answer's two parts are a tuple: (the set bits, {key: offset of the entry's value}).
//   * Image builds on the typed xex (header, basic) and ppc_asm.  Its `data` is a bytearray in Python: here py::bytes,
//     replaced on each write (the same bytes either way).  tempfile.mkstemp (not in the runtime) is written below.
//   * main() returns the exit status; the module's docstring is DOC (what -h prints).
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::formats::xex_edit {

extern const py::str DOC;
extern const py::dict<py::str, int64_t> NAMED_BITS;                 // PAL-50 incompatible
extern const py::bytes NOP;

// {bit: True} for each privilege bit set (the bits, sorted), and the entries' offsets {key: value offset}.
std::tuple<py::list<int64_t>, py::dict<int64_t, int64_t>> privileges(const py::bytes& data);

// `data` with each (bit, on) change made. A bit in a privilege entry the header lacks stops.
py::bytes set_privileges(const py::bytes& data, const py::list<std::tuple<int64_t, bool>>& changes);

std::tuple<int64_t, bool> _parse_bit(const py::str& token);

// A module written as a basic XEX: its header, then the whole image from `pe_offset`.
class Image {
  public:
    py::bytes data;
    std::optional<int64_t> base;               // the header's image base (None when it names none)
    int64_t pe_offset = 0;

    explicit Image(const py::str& path);

    int64_t offset(int64_t address, int64_t size) const;
    py::bytes read(int64_t address, int64_t size) const;
    void write(int64_t address, const py::bytes& blob);
    void save(const py::str& out) const;
};

// A displaced instruction that branches relative to its own address cannot move to a cave.
bool _relative_branch(int64_t word);

void hook(Image& image, int64_t address, int64_t cave, const py::str& asm_text);

py::str _asm_text(const py::str& text);

int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::formats::xex_edit
