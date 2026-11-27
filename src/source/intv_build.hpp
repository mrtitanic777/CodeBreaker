// source/intv_build.hpp -- the Intellivision cartridge layer over the one assembler: as1600 source in, the cartridge file out.
//
//     Bytes file = intv_build::build(source_text);     // the .rom file (or the raw file) the source describes
//
// The instructions are the CP1610 description's (decompile/engine/languages CP1600.sla, through the one assembler) in as1600's dialect
// (config/dialects/as1600.json: `$` hex, `;` comments); this layer is what says how the file is laid out:
//
//     SEGMENT $50,$68                 a segment of pages $50 to $68 (the words $5000-$68FF) begins; the address counter is at its first word
//     ORG $5100                       where the next word goes (inside the segment)
//     DECLE 1, $02, label ...         words (numbers up to 16 bits, labels)           FILL count, value     count words of value
//     ROMTAIL $00, $10 ...            the bytes that follow the segments in a .rom file (the Intellicart's tables)
//     FORMAT RAW                      the output is the raw file (big-endian words from the first segment's start, no table, no CRC)
//     NAME EQU value                  constants (also `NAME = value`)
//     label  or  label:               the word address of the next word
//     MVII #$1234,R0 ...              an instruction; an instruction after SDBD is double-byte
//
// Words nothing is written to are zero. The CRC of each segment is worked out. Two passes: the first gives each instruction its size
// and each label its address.
#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "core/bytes.hpp"

namespace cb::source::intv_build {

// AsmError (a ParseError), with the line, for anything that does not assemble.
Bytes build(std::string_view source);

// The decles of one instruction line at `pc` (as1600's text; `labels`: name -> value; `sdbd`: it follows an SDBD). ParseError when it does not assemble.
std::vector<std::uint16_t> assemble_line(std::string_view line, std::uint32_t pc, const std::unordered_map<std::string, std::int64_t>& labels = {}, bool sdbd = false);

}  // namespace cb::source::intv_build
