// source/risc_source.hpp -- what the writers of the 68000, MIPS, ARM, SuperH and PowerPC consoles say in source, through the one disassembler and the
// one assembler: the processor's SLEIGH description (source/processors: read_described, assemble_described).
//
// The decompiler's own decoders still decide where code is: which bytes are an instruction, where flow goes. The text of an instruction is the
// description's, with the program's names put where its numbers are, and the source builds back through the same description under the one
// directive layer (build/mips_asm): `.org`, `.equ`, `name:`, `.byte`, `.word`, `.ascii`, `.space`, `;` comments.
//
//     .equ VDP_CTRL, 0xc00004
//             .org 0x200
//     reset:
//             move.w #0x80,(VDP_CTRL).l
//             jsr sub_000220.l
//
// An instruction whose text does not assemble back to its own bytes (an immediate's unused byte, an encoding the text cannot tell apart) stays as
// `.byte` with the instruction in a comment, so the source still builds the same bytes.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "core/bytes.hpp"
#include "decompile/labels.hpp"
#include "decompile/m68k.hpp"

namespace cb::source::risc_source {

// The text the description reads `bytes` (one instruction, at `pc`) as, when it assembles back to the same bytes; nullopt when it does not (or
// the bytes are no instruction).
// `following`: the bytes after the instruction, for a processor whose branch reads its delay slot's instruction with it (SuperH).
std::optional<std::string> described(std::string_view cpu, ByteView bytes, std::uint64_t pc, ByteView following = {});

// The text the description reads `bytes` (one instruction) as, whether or not it assembles back to the same bytes; nullopt for bytes that are no instruction.
std::optional<std::string> read_text(std::string_view cpu, ByteView bytes, std::uint64_t pc);

// One instruction as a line of source (without its indent): the description's text with every number literal whose value is one of `operands`
// and has a name in `names` written as its name; `immediate` (when not empty) written in place of the last number literal (a MIPS address half:
// `%hi(name)`); or `.byte` of its bytes with the text in a comment. `symbols` (the program's labels and equates, name -> value): the line with
// the names put in is kept only when it assembles to the instruction's own bytes, else the line with fewer names, else the description's plain text.
std::string code(std::string_view cpu, ByteView bytes, std::uint64_t pc, const std::map<std::uint32_t, std::string>& names, std::span<const std::uint32_t> operands,
                 const std::string& immediate = {}, ByteView following = {},
                 const decompile::NameLabels* symbols = nullptr);

// A 68000 instruction: `code` with the names of the addresses its operands and its target name (absolute and PC-relative operands, long immediates,
// branch targets).
std::string m68k_code(const decompile::m68k::Insn& insn, const decompile::AddressLabels& names);

// Bytes as lines: `.byte` rows of 16, `.ascii` for a run of four or more printable characters, `.space n, v` for a run of 16 or more of one value.
std::vector<std::string> data_lines(ByteView data);

// `.equ name, value` for a name defined as a number, the names lined up.
std::string equ(const std::string& name, std::uint32_t value, unsigned digits);

// The image `source` builds to, through the processor's description. AsmError (a ParseError) for source that does not assemble.
Bytes assemble(std::string_view cpu, std::string_view source, const std::filesystem::path& folder);

}  // namespace cb::source::risc_source
