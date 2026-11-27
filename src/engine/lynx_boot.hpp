// engine/lynx_boot.hpp -- the Atari Lynx's boot ROM played for a cartridge, on the one emulator's 65C02 (pcode_cpu::M6502Cpu), to see what it loads.
//
//     const emulate::lynx::Boot b = engine::lynx_boot::boot(program);   // the segments the loaders copied and the entries they jumped to
//
// In the way Felix does it (the ROM's routines are trapped rather than run): decrypt the first stage to $0200 and run it; the ROM's shift routine at $FE00
// sets the cartridge address to block A of the bank; reads of RCART0 ($FCB2) give the cartridge's bytes in turn. The run is watched: every store of a byte just
// read from RCART0 is one byte of the cartridge copied to RAM, and the copies that run on together are the segments; a jump out of one loaded segment into
// another is an entry. It stops when the loaders have stopped reading the cartridge for a while (the program is running), when the loaders hang in the ROM, on
// an undocumented opcode, or at a step limit. The run is deterministic: nothing in it depends on time.
#pragma once

#include "emulate/lynx.hpp"
#include "formats/lynx.hpp"

namespace cb::engine::lynx_boot {

// Play the boot ROM for `program` (a BS93 object needs no run: its segment and entry are its header's).
emulate::lynx::Boot boot(const formats::lynx::Program& program);

}  // namespace cb::engine::lynx_boot
