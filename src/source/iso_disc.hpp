// source/iso_disc.hpp -- the small disc the console writers (Saturn, Dreamcast) build their rebuilt first program into: the system area,
// an ISO 9660 volume whose root directory holds the one file, and the file's data in the sectors after. The readers (formats/saturn,
// formats/dreamcast) find the same two parts in it. (A whole folder written as a disc is build/disc_image.)
#pragma once

#include <cstdint>
#include <string>
#include <string_view>

#include "core/bytes.hpp"

namespace cb::source::iso_disc {

inline constexpr std::size_t kSector = 2048;

// A disc of 2048-byte sectors: `system_area` (the 16 sectors before the volume descriptors) at the start, then the volume (system and
// volume identifier `system_id` / CODEBREAKER) whose root directory, in sector 17, has the file `name` (";1" is added) at sector 18,
// and `program` in the sectors from 18 on. `volume_base` is added to the sectors the records name (a disc whose volume starts later
// in the image).
Bytes minimal(ByteView system_area, ByteView program, const std::string& name, std::string_view system_id, std::uint32_t volume_base = 0);

}  // namespace cb::source::iso_disc
