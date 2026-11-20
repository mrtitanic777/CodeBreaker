// source/z80_source.hpp -- what the Z80-family writers (Master System, ZX Spectrum, MSX, ColecoVision, CPC, Neo Geo's sound CPU) say in source, through the
// one disassembler and the one assembler: the Z80's SLEIGH description (source/processors: read_described, assemble_described).
//
// The decompiler's Z80 tables (decompile/z80) still decide where code is: which bytes are an instruction, where flow goes. The text of an
// instruction is the description's, with the program's names put where its numbers are, and the source builds back through the same
// description under the one directive layer (build/mips_asm): `.org`, `.equ`, `name:`, `.byte`, `.ascii`, `.space`, `;` comments.
//
//     .equ ULA, 0xfe                              (a port or a fixed address, written by its name)
//             .org 0x8000
//     start:
//             CALL sub_8010                       (a target is its label)
//             LD HL,D_8020
//             OUT (ULA),A
//
// An instruction whose text does not assemble back to its own bytes (a prefix that does nothing, an undocumented mirror) stays as `.byte`
// with the instruction in a comment, so the source still builds the same bytes.
#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <vector>

#include "core/bytes.hpp"
#include "decompile/labels.hpp"
#include "decompile/z80.hpp"
#include "source/image_writer.hpp"

namespace cb::source::z80_source {

// One instruction as a line of source (without its indent): the description's text with `names` (labels and fixed addresses) put where the
// instruction's address operand is and `ports` where its port is, or `.byte` of its bytes with the text in a comment.
std::string code(const decompile::z80::Insn& insn, const decompile::AddressLabels& names, const image_writer::Names& ports);

// The address of a label this instruction names that the assembler's first pass meets before the label's line, behind a register or a condition
// (`LD HL,D_8020`, `JR NZ,L_0090`): it reads the register as the unknown name and finds no instruction. Such a label is defined once ahead of the
// code (`.equ`, which its line then repeats with the same value). `constants`: the addresses already defined ahead of the code.
std::optional<std::uint32_t> forward_reference(const decompile::z80::Insn& insn, const decompile::AddressLabels& names, const std::set<std::uint32_t>& constants);

// Bytes as lines: `.byte` rows of 16, `.ascii` for a run of four or more printable characters, `.space n, v` for a run of 16 or more of one value.
std::vector<std::string> data_lines(ByteView data);

// `.equ name, value` for a name defined as a number, the names lined up.
std::string equ(const std::string& name, std::uint32_t value, unsigned digits);

// The image `source` builds to, through the Z80's description. AsmError (a ParseError) for source that does not assemble.
Bytes assemble(std::string_view source, const std::filesystem::path& folder);

}  // namespace cb::source::z80_source
