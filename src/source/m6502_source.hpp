// source/m6502_source.hpp -- what the 6502-family writers (NES, Atari 2600, 5200 and 7800, Lynx, PC Engine, SNES) say in source, through the one
// disassembler and the one assembler: the SLEIGH descriptions of the 6502, the 65C02, the HuC6280 and the 65816 (source/processors:
// read_described, assemble_described; source/described_source has what every processor shares).
//
// The decompiler's tables (decompile/m6502, huc6280, w65816) still decide where code is: which bytes are an instruction, where flow goes.
// The text of an instruction is the description's, with the program's names put where its numbers are, and the source builds back through
// the same description under the one directive layer (build/mips_asm):
//
//     .equ PPUSTATUS, 0x2002                      (a register or a fixed address, written by its name)
//             .org 0xc000                         (the address the next byte is seen at; the file is the bytes in the order written)
//     reset:
//             SEI
//             LDA #0x0
//             STA PPUCTRL
//             JSR sub_C00C                        (a target is its label)
//             .half nmi,reset,irq                 (the vectors)
//
// The 65816's accumulator and index widths are context of its description (m16, x16); the source says them with `.context m16=1 x16=0` ahead of
// the instructions they change, and the text of an immediate is read and assembled under them.
//
// An instruction whose text does not assemble back to its own bytes (an absolute form of a zero-page address, an undocumented opcode) stays as
// `.byte` with the description's reading in a comment, so the source still builds the same bytes.
#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>

#include "core/bytes.hpp"
#include "decompile/labels.hpp"
#include "source/described_source.hpp"

namespace cb::source::m6502_source {

enum class Cpu : std::uint8_t {
    M6502,     // the NMOS 6502 (NES, Atari 2600 / 5200 / 7800)
    M65C02,    // the CMOS 65SC02 (Atari Lynx)
    Huc6280,   // PC Engine
    W65816,    // SNES
};

using described_source::Context;

// The instruction's text as the description reads it, when that text assembles back to exactly its bytes (nullopt: the writer keeps it as bytes).
std::optional<std::string> described(Cpu cpu, ByteView bytes, std::uint32_t pc, const Context& context = {});

// One instruction as a line of source (without its indent): the description's text with `names` (address -> name; labels, registers) put where
// its numbers are, or `.byte` of its bytes with the text in a comment. `bytes` are its own, sitting at `pc`.
std::string code(Cpu cpu, ByteView bytes, std::uint32_t pc, const decompile::AddressLabels& names, const Context& context = {});

// The 65816's context for an instruction of these widths, as `.context m16=1 x16=0` writes it.
Context widths(bool m16, bool x16);

// Bytes as lines (`.byte` rows of 16, `.ascii`, `.space`) and `.equ name, value`, as for every processor.
std::vector<std::string> data_lines(ByteView data);
std::string equ(const std::string& name, std::uint32_t value, unsigned digits);

// The image `source` builds to, through the processor's description. AsmError (a ParseError) for source that does not assemble.
Bytes assemble(Cpu cpu, std::string_view source, const std::filesystem::path& folder);

}  // namespace cb::source::m6502_source

#include "decompile/m6502.hpp"
#include "source/dialect.hpp"

namespace cb::source::m6502_source {

// `code` (what `code()` made for `insn`) with an instruction the description's text cannot say back -- the absolute form of a zero-page address -- said as the
// dialect has it (DASM's `STA.w WSYNC`), when the dialect has such a form; a fallback line otherwise gets its comment said in the dialect.
std::string in_dialect(const dialect::Dialect& dialect, const decompile::m6502::Insn& insn, const decompile::AddressLabels& names, const std::string& code);

}  // namespace cb::source::m6502_source

namespace cb::source::m6502_source {

// The same for an instruction of any 6502-family decoder: whether it is an absolute-mode one, its mnemonic and operand.
std::string in_dialect_absolute(const dialect::Dialect& dialect, std::string_view mnemonic, bool absolute, std::uint32_t operand, const decompile::AddressLabels& names,
                                const std::string& code);

}  // namespace cb::source::m6502_source
