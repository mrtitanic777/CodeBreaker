#include "emulate/z80_machines.hpp"

#include <algorithm>
#include <format>

#include "formats/coleco.hpp"
#include "formats/cpc.hpp"

namespace cb::emulate::z80_machines {

namespace {

std::uint8_t coleco_port(std::uint16_t port) {
    const std::uint8_t p = static_cast<std::uint8_t>(port);
    if (p >= 0xA0 && p <= 0xBF) return (p & 1) != 0 ? 0x80 : 0x00;   // the VDP: status (F, the frame flag, set) or data
    return 0xFF;                                                      // controllers and the rest: nothing pressed
}

std::uint8_t cpc_port(std::uint16_t port) {
    (void)port;   // PPI port B ($F5xx) has the vertical sync in bit 0 (set: in the blank); keys read as not pressed
    return 0xFF;
}

}  // namespace

const Machine& coleco() {
    static const Machine kMachine{.name = "coleco",
                                  .firmware = {{0x0000, 0x1FFF}},
                                  .names = &formats::coleco::bios_names(),
                                  .ram = {{0x6000, 0x7FFF}},
                                  .stack = 0x73B9,
                                  .port_in = coleco_port};
    return kMachine;
}

const Machine& cpc() {
    static const Machine kMachine{.name = "cpc",
                                  .firmware = {{0xB900, 0xBDFF}},
                                  .names = &formats::cpc::firmware_names(),
                                  .ram = {{0x0040, 0xB8FF}, {0xBE00, 0xBFEF}, {0xC000, 0xFFFF}},
                                  .stack = 0xBFF0,
                                  .decodes_high_byte = true,
                                  .port_in = cpc_port};
    return kMachine;
}

bool in_firmware(const Machine& machine, std::uint16_t address) {
    return std::any_of(machine.firmware.begin(), machine.firmware.end(), [&](const Range& r) { return r.low <= address && address <= r.high; });
}

void stub_firmware(const Machine& machine, ByteSpan memory) {
    for (const Range& r : machine.firmware)
        for (std::size_t a = r.low; a <= r.high && a < memory.size(); ++a) memory[a] = 0xC9;
}

z80::Bus bus_for(const Machine& machine, ByteView memory, Trace& trace) {
    const Machine* m = &machine;
    Trace* t = &trace;
    return z80::Bus(
        memory,
        [m, t](std::uint16_t port) {
            const std::uint8_t v = m->port_in ? m->port_in(port) : 0xFF;
            t->ports.push_back(std::format("in {:x} {:x}", m->decodes_high_byte ? port : port & 0xFF, v));
            return v;
        },
        [m, t](std::uint16_t port, std::uint8_t value) { t->ports.push_back(std::format("out {:x} {:x}", m->decodes_high_byte ? port : port & 0xFF, value)); });
}

z80::RunResult run(const Machine& machine, z80::Bus& bus, Trace& trace, std::uint16_t entry, z80::RunOptions options) {
    options.sp = machine.stack;
    const Machine* m = &machine;
    Trace* t = &trace;
    options.before_step = [m, t](std::uint16_t pc) {
        if (!in_firmware(*m, pc)) return;
        std::string name;
        if (m->names) {
            if (const auto it = m->names->find(pc); it != m->names->end()) name = it->second;
        }
        t->calls.push_back(name.empty() ? std::format("{:04x}", pc) : name);
    };
    return z80::run(bus, entry, options);
}

}  // namespace cb::emulate::z80_machines
