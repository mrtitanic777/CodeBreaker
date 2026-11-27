// emulate/z80_machines.hpp -- the machines built round a Z80 whose programs are run as functions: what is mapped where, what answers
// the ports, and what is firmware.
//
//     Trace trace;
//     z80::Bus bus = bus_for(coleco(), memory, trace);       // memory: the 64 KB as loaded; the BIOS area reads as RET
//     z80::RunResult r = engine::pcode_cpu::run(bus, entry, run_options(coleco(), trace));    // trace.ports: every IN/OUT with the full port; trace.calls: the BIOS entries called
//
// The firmware of these machines (the ColecoVision's BIOS, the Amstrad CPC's jumpblock) is not part of the program, and it is not run: every
// byte of it reads as RET, so a call into it comes back at once, the same way in the original's run and the rebuilt one's, and the call is
// noted by its documented name (formats/coleco, formats/cpc). The CPC's ports are noted with all sixteen bits of the address, as it decodes its
// devices (gate array $7Fxx, CRTC $BCxx / $BDxx, PPI $F4xx-$F7xx, ROM select $DFxx) by the high byte. A read of a port answers what an idle
// machine does: the VDP in its vertical blank, no key or button pressed.
#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "core/bytes.hpp"
#include "emulate/z80.hpp"

namespace cb::emulate::z80_machines {

// An inclusive range of addresses.
struct Range {
    std::uint32_t low = 0, high = 0;
};

struct Machine {
    std::string_view name;
    // The firmware's addresses [low, high]: every byte RET, every call noted.
    std::vector<Range> firmware;
    // The names of the firmware's entry points (an address with none is noted by its number).
    const std::map<std::uint32_t, std::string>* names = nullptr;
    // The RAM a run is handed (filled from a seed, except under the program and the firmware).
    std::vector<Range> ram;
    std::uint16_t stack = 0xFFF0;
    // Does the machine decode the high byte of a port (the CPC does; the ColecoVision looks at the low byte only, so what the Z80 puts on the high
    // lines -- A for OUT (n),A -- is not part of the port noted)?
    bool decodes_high_byte = false;
    std::function<std::uint8_t(std::uint16_t port)> port_in;
};

const Machine& coleco();   // ColecoVision: BIOS $0000-$1FFF, RAM $6000-$7FFF, cartridge at $8000
const Machine& cpc();      // Amstrad CPC: the firmware jumpblock $B900-$BDFF, RAM everywhere else

// What a run did outside memory.
struct Trace {
    std::vector<std::string> ports;   // "out 7f8d 89", "in f5ff ff"
    std::vector<std::string> calls;   // "KM_WAIT_CHAR", or "bb06" for an address with no name
};

bool in_firmware(const Machine& machine, std::uint16_t address);

// `memory` with the firmware's bytes made RET (64 KB).
void stub_firmware(const Machine& machine, ByteSpan memory);

// A bus over a copy of `memory` whose ports answer as the machine's do and note themselves in `trace` (which must outlive the bus).
z80::Bus bus_for(const Machine& machine, ByteView memory, Trace& trace);

// The options a run on the machine takes: its stack, and each instruction fetched from firmware noted in `trace` as a call (`trace` must outlive the run).
z80::RunOptions run_options(const Machine& machine, Trace& trace, z80::RunOptions options = {});


}  // namespace cb::emulate::z80_machines
