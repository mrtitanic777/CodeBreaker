// codebreaker/exact/x86.hpp -- C++ from codebreaker.exact.x86.
//
// x86.py -- 16-bit code disassembled, as the period's compilers wrote it.
//
// Microsoft's languages (C 3.00-5.x, Pascal, FORTRAN) compiled floating point for the 8087 emulator
// (-FPi): each ESC instruction an interrupt the emulator takes - INT 34h-3Bh stand for the ESC opcodes
// D8h-DFh (the next byte is the instruction's modrm), INT 3Ch for an ESC with a segment override (the
// next byte the segment and the opcode), INT 3Dh for FWAIT. A plain disassembler reads the bytes after
// the interrupt as instructions of their own and loses its way; here they are read as what they
// stand for.
//
//     for (const Insn& i : x86::disasm(img, at, end)) i.address i.size i.bytes i.mnemonic i.op_str
#pragma once

#include <functional>
#include <optional>

#include "py.hpp"

namespace codebreaker::exact::x86 {

struct Insn {
    int64_t address = 0;
    py::bytes bytes;
    py::str mnemonic;
    py::str op_str;
    int64_t size = 0;

    Insn() = default;
    Insn(int64_t a, const py::bytes& b, const py::str& m, const py::str& o);
    friend bool operator==(const Insn& a, const Insn& b) {
        return a.address == b.address && a.bytes == b.bytes && a.mnemonic == b.mnemonic && a.op_str == b.op_str;
    }
};

// The one thing x86.py takes from outside: `_CS = platforms.decoder(16)`, the 16-bit x86 decoder (capstone).
// codebreaker.platforms has no typed port yet, so the decoder is a small interface: given the bytes at
// `address`, the first instruction they decode to, or nullopt when they decode to none
// (`next(_CS.disasm(code, address), None)`). Install it with set_decoder before the first call.
using Decoder = std::function<std::optional<Insn>(const py::bytes& code, int64_t address)>;
void set_decoder(Decoder d);

// (Python's generator: the whole sequence at once)
py::list<Insn> disasm(const py::bytes& img, int64_t at, int64_t end, bool emulator = true);

// {table address: its length} - the jump tables of a switch kept in the code (jmp cs:[bx+T]): the words
// at T, as long as each is a place in [lo, hi)
py::dict<int64_t, int64_t> tables(const py::list<Insn>& insns, const py::bytes& img, int64_t lo, int64_t hi, int64_t base = 0);

// the instructions of a function's code [lo, hi), its switch tables stepped over (they are data)
py::list<Insn> body(const py::bytes& img, int64_t lo, int64_t hi, bool emulator = true, int64_t base = 0);

}  // namespace codebreaker::exact::x86
