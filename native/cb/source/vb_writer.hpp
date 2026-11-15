// cb/source/vb_writer.hpp -- a Virtual Boy cartridge as V810 assembly source that builds back into it.
//
//     image_writer::Written r = vb_writer::reconstruct("game.vb", "out");
//     r.source           game.s (GNU-style V810 syntax; CodeBreaker's own build/v810_asm reads it too)
//     r.same             the source, built, is the cartridge file byte for byte
//     r.functions        {address: name}   (addresses as the CPU sees them: $07000000 + the file offset)
//
// The header is written field by field (`.ascii` title, the reserved bytes, maker, game code, version). Code is what flow reaches from the reset slot and from every
// interrupt and exception slot that holds code: branches, `jr`, `jal`, and a `movhi / movea / jmp [reg]` with a constant address. The slots are named (`reset`,
// `vector_gamepad`, `vector_vip` ...); other functions are `sub_<address>`, jump targets `L_<address>`. A constant built by `movhi` with a `movea`, `addi`, load or store that
// completes an address of the ROM becomes `hi(D_07012345)` / `lo(D_07012345)` with a label on that data, and one that completes the address of a hardware register
// (the VIP at $0005F800, the VSU at $01000000, the hardware control registers at $02000000) uses its name (`VIP_INTPND`, `VSU_S1INT`, `HW_SCR`), defined by `.equ` lines.
// An instruction whose text would not assemble to its bytes is written as `.byte` with the instruction in a comment. Runs of one byte are `.fill count,value`.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <string>
#include <string_view>

#include "cb/formats/vb.hpp"
#include "cb/source/image_writer.hpp"

namespace cb::source::vb_writer {

// The hardware registers by address: the VIP's, the VSU's and the hardware control registers.
const std::map<std::uint32_t, std::string>& hardware();

image_writer::Written write(const formats::vb::Rom& rom, std::string_view file_name, const std::filesystem::path& folder);
image_writer::Written reconstruct(const std::filesystem::path& rom_file, const std::filesystem::path& folder);

}  // namespace cb::source::vb_writer
