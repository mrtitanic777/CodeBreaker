// codebreaker/build/z80_asm.hpp -- C++17 port of codebreaker.build.z80_asm.
//
// z80_asm.py -- CodeBreaker's own Z80 assembler, for the source the Master System lane writes.
//
//     py::bytes rom = z80_asm::build(source_text);        // the ROM image the source describes
//     py::bytes rom = z80_asm::build_file(path);          // .INCLUDE / .INCBIN resolve beside it
//
// It reads the part of WLA-DX's language that source/sms_writer.py writes, and that Master System
// and Game Gear homebrew is commonly written in, so the source also builds with WLA-DX itself:
//
//     .MEMORYMAP ... .ENDME           the slots (only SLOTSIZE and SLOT n $addr are used)
//     .ROMBANKMAP ... .ENDRO          BANKSTOTAL, BANKSIZE, BANKS (the ROM's size)
//     .BANK n SLOT s                  where the following bytes go: ROM page n, seen at slot s's address
//     .ORG offset / .ORGA address     within the bank: from its start / at a CPU address
//     .DB / .BYTE  .DW / .WORD        bytes and little-endian words: numbers, labels, "strings"
//     .DSB n value                    n bytes of value
//     .INCBIN "file"                  a file's bytes
//     .DEFINE NAME value / NAME = value     constants
//     label:                          the address of the next byte
//     ; comment
//
// Numbers: $1F 1Fh %0101 31 'c'. Expressions + - * / & | ^ << >> ( ), and <x >x (low and high byte).
// Unwritten bytes of the ROM are $FF, as WLA-DX leaves them by default (.EMPTYFILL).
#pragma once

#include <tuple>

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::build::z80_asm {

// class AsmError(ValueError)
struct AsmError : py::ValueError {
    explicit AsmError(const std::string& m) : py::ValueError(m) {}
    explicit AsmError(const char* m) : py::ValueError(m) {}
    explicit AsmError(const py::str& m) : py::ValueError(m.utf8()) {}
    const char* type() const override { return "AsmError"; }
};

py::str _strip(const py::str& line);

// Comma-separated items of a data directive (strings kept whole).
py::list<py::str> _items(const py::str& text);

py::bytes build_file(const py::str& path);
py::bytes build(const py::str& source, const py::str& folder = py::str("."));

// An instruction's size on the first pass, when a label it names is not defined yet: assembled
// with every unknown name taken as a value that fits each operand kind.
py::bytes _size_guess(const py::str& line, int64_t pc);

void _put(const py::bytearray& image, int64_t bank, int64_t bank_size, int64_t pos, const py::bytes& data);

// A flat program in the syntax of sjasmplus, pasmo and the Spectrum and MSX assemblers:
//
//     ORG $addr                       where the next bytes go
//     DEFB / DB, DEFW / DW            bytes, little-endian words: numbers, labels, "strings"
//     DEFS / DS n[,value]             n bytes of value (0)
//     NAME EQU value
//     INCBIN "file"
//     label:
//
// -> (address of the first byte, the bytes up to the last one written; gaps zero).
std::tuple<int64_t, py::bytes> build_flat(const py::str& source, const py::str& folder = py::str("."));

}  // namespace codebreaker::build::z80_asm
