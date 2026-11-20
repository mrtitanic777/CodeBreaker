// source/described_source.hpp -- what every writer that says a program in a processor's SLEIGH description needs, whatever the processor: the
// instruction's text as the one disassembler (processors::read_described) reads it, kept only when the one assembler (assemble_described)
// builds it back into the same bytes, the program's names put where its numbers are, the data lines, and the build.
//
// The source is in the description's syntax under the one directive layer (build/mips_asm): `.org`, `.equ`, `.context`, `name:`, `.byte`, `.half`,
// `.ascii`, `.space`, `;` comments. z80_source and m6502_source say what is particular to their processors.
#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "core/bytes.hpp"
#include "decompile/labels.hpp"
#include "source/processors.hpp"

namespace cb::source::described_source {

// The description's context variables an instruction is read in (the 65816's register widths): {{"m16", 1}, {"x16", 0}}.
using Context = std::vector<std::pair<std::string_view, int>>;

// The instruction at the start of `bytes`, sitting at `pc`, as the description reads it under `context`, when that text assembles back to
// exactly the instruction's own `size` bytes; nullopt otherwise (not an instruction, or a text another encoding shares).
std::optional<std::string> checked_text(const processors::Processor& cpu, ByteView bytes, std::uint64_t pc, std::size_t size, const Context& context = {});

// `.byte` of the instruction's bytes with its text (or `what`, when the description reads none) in a comment: it builds the same bytes.
std::string byte_line(const processors::Processor& cpu, ByteView bytes, std::uint64_t pc, const Context& context, std::string_view what);

// `text` with each number written 0x<hex> that is not an immediate (`#0x..`) and has a name in `names` written as its name.
std::string name_numbers(const std::string& text, const decompile::AddressLabels& names);

// `text` with its last number 0x<value> written as `name` (only the one in parentheses when `in_parentheses`).
std::string name_number(const std::string& text, std::uint32_t value, const std::string& name, bool in_parentheses);

// `.context m16=1 x16=0`.
std::string context_line(const Context& context);

// Bytes as lines: `.byte` rows of 16, `.ascii` for a run of four or more printable characters, `.space n, v` for a run of 16 or more of one value.
std::vector<std::string> data_lines(ByteView data);

// `.equ name, value` for a name defined as a number, the names lined up.
std::string equ(const std::string& name, std::uint32_t value, unsigned digits);

// The image `source` builds to through the processor's description. AsmError (a ParseError) for source that does not assemble.
Bytes assemble(const processors::Processor& cpu, std::string_view source, const std::filesystem::path& folder);

}  // namespace cb::source::described_source
