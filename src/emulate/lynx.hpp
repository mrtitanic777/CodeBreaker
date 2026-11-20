// emulate/lynx.hpp -- the Atari Lynx as a machine: the bus a program runs on, and the boot ROM played to see what a cartridge loads.
//
// Memory (formats/lynx.hpp has the register names): RAM $0000-$FBFF; Suzy's registers $FC00-$FCFF and Mikey's $FD00-$FDFF, every access
// to which is logged (these are the effects a function has outside RAM); the boot ROM at $FE00-$FFF7 is absent, and $FFF8-$FFFF is RAM
// but for the memory map control $FFF9, which is logged too. What the devices answer a read is fixed and plausible: Suzy idle (SPRSYS 0, no
// button down), Mikey with every interrupt pending (INTSET $FF), its serial port ready to send (SERCTL $A0) and each timer done (CTLB bit 3).
//
// `boot` plays the boot ROM for a cartridge, in the way Felix does (it traps the ROM's routines rather than running them): decrypt the
// first stage to $0200 and run it; the ROM's shift routine at $FE00 sets the cartridge address to block A of the bank; reads of RCART0
// ($FCB2) give the cartridge's bytes in turn. The run is watched: every store of a byte just read from RCART0 is one byte of the cartridge
// copied to RAM, and the copies that run on together are the segments; a jump out of one loaded segment into another is an entry. It
// stops when the loaders have stopped reading the cartridge for a while (the program is running), when the loaders hang in the ROM, on
// an undocumented opcode, or at a step limit. The run is deterministic: nothing in it depends on time.
#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "core/bytes.hpp"
#include "emulate/m6502.hpp"
#include "formats/lynx.hpp"

namespace cb::emulate::lynx {

inline constexpr std::uint64_t kBootSteps = 4000000;       // instructions of loader and program before the run is cut
inline constexpr std::uint64_t kIdleSteps = 30000;         // instructions without a cartridge read, once something has loaded, after which the program is taken to be running

struct Boot {
    std::vector<formats::lynx::Segment> segments;   // by file offset
    std::vector<std::uint32_t> entries;             // the addresses the loaders jumped to, in the order first jumped to
    std::uint64_t steps = 0;
    std::string ended;                              // "running" | "rom" | "fault: ..." | "limit" | "no first stage"
};

// Play the boot ROM for `program` (a BS93 object needs no run: its segment and entry are its header's).
Boot boot(const formats::lynx::Program& program);

// A bus for a program whose RAM is `image` (64 KB; what is at $FC00 and above is not used): the devices above.
m6502::Bus make_bus(ByteView image);

}  // namespace cb::emulate::lynx
