// source/threedo_writer.hpp -- a 3DO program (the LaunchMe of a disc, or a bare AIF) as assembly source that builds back into it.
//
//     image_writer::Written r = threedo_writer::reconstruct("game.iso", "out");
//     r.source           game.s (GNU as syntax, `.syntax unified`, ARM state: what armcc-built and gcc-built 3DO programs are)
//     r.same             the source, built, is the AIF image word for word (and, for a disc, the disc with the image put back is the disc)
//     r.functions        {address: name}
//
// The ARM60 is big-endian and CodeBreaker's ARM tables are little-endian, so the source is written for the image with each aligned word reversed
// (formats/threedo `native()`): every word is its value, an instruction as text and a data word as `.word` (a string is its words; one
// byte-wise `.ascii` would build the wrong bytes), and the rebuilt words are reversed again to compare. Code is what flow reaches from the entry
// (word 3's BL): branches, calls, and `mov lr, pc` + `mov pc, rN` calls that come back; a function that is only called through a pointer
// is data. An instruction is written as text only when the text assembles back to its own bits; the rare others stay `.word` with the instruction as
// a comment. Functions are `entry` and `sub_<address>`, jump targets `L_<address>`, the words PC-relative loads read `D_<address>`. A literal-pool word
// that is the address of one of the Madam or Clio registers is written by the register's name (MADAM_MCTL, CLIO_IRQ0 ...). The image is at address 0.
//
// It builds with CodeBreaker's own assembler (the one assembler). A disc's rebuilt program is kept beside the source as <stem>.built.iso (the disc with
// the new image put where LaunchMe was), a bare AIF as <stem>.built.aif.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <string>
#include <string_view>

#include "formats/threedo.hpp"
#include "source/image_writer.hpp"

namespace cb::source::threedo_writer {

// Madam's and Clio's registers by address, by the names the 3DO hardware documentation gives them.
const std::map<std::uint32_t, std::string>& hardware();

image_writer::Written write(const formats::threedo::Program& program, std::string_view file_name, const std::filesystem::path& folder);
image_writer::Written reconstruct(const std::filesystem::path& file, const std::filesystem::path& folder);

}  // namespace cb::source::threedo_writer
