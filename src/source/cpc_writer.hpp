// source/cpc_writer.hpp -- an Amstrad CPC program (a disk's binary, an AMSDOS file or a snapshot's RAM) as assembly source that builds back into it.
//
//     image_writer::Written r = cpc_writer::reconstruct("game.dsk", "out");
//     r.source                  game.asm (sjasmplus / pasmo syntax: ORG ...)
//     r.same                    the source, built, is the program block byte for byte
//
// Written through source/z80_writer from the program's execution address (a snapshot's program counter), with the firmware's names: a call to
// the jumpblock ($B900-$BDFF) is written `CALL KM_WAIT_CHAR`, as the Firmware Guide names it. The head comment lists the disk's files and the
// registers of a snapshot. (Ports are decoded by the high byte on the CPC and are reached through BC, so none is named.)
#pragma once

#include <filesystem>
#include <string_view>

#include "formats/cpc.hpp"
#include "source/image_writer.hpp"
#include "source/z80_writer.hpp"

namespace cb::source::cpc_writer {

// The firmware's entry points by name.
const z80_writer::Machine& machine();

// The source of `program` (read from a file named `file_name`) written to `folder` as <stem>.asm, and rebuilt.
image_writer::Written write(const formats::cpc::Program& program, std::string_view file_name, const std::filesystem::path& folder);

// The same for a file (IoError when it cannot be read, FormatError when it is not a CPC program).
image_writer::Written reconstruct(const std::filesystem::path& file, const std::filesystem::path& folder);

}  // namespace cb::source::cpc_writer
