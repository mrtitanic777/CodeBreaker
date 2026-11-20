// Tests of the Z80 machine models: what is firmware, what a port answers, what a run notes of both. Expected values are worked out from the machines'
// documentation (the ColecoVision's VDP status has bit 7 set in the vertical blank; the CPC's gate array is written through BC = $7Fxx).

#include "emulate/z80_machines.hpp"

#include "core/testing.hpp"

namespace cb::emulate::z80_machines {
namespace {

Bytes memory_with(std::uint16_t at, const Bytes& code) {
    Bytes m(0x10000, 0);
    std::copy(code.begin(), code.end(), m.begin() + at);
    return m;
}

CB_TEST("emulate.z80_machines.the_coleco_bios_is_ret_and_a_call_into_it_is_noted_by_name", [] {
    // CALL $1FBE (BLK_WRT_VRAM); OUT ($BF),A; IN A,($BF); RET
    Bytes mem = memory_with(0x8000, {0xCD, 0xBE, 0x1F, 0xD3, 0xBF, 0xDB, 0xBF, 0xC9});
    stub_firmware(coleco(), mem);
    CB_CHECK_EQ(mem[0x1FBE], 0xC9);
    CB_CHECK_EQ(mem[0x0000], 0xC9);
    CB_CHECK_EQ(mem[0x2000], 0x00);   // the BIOS ends at $1FFF
    Trace trace;
    z80::Bus bus = bus_for(coleco(), mem, trace);
    z80::RunOptions options;
    options.regs.a = std::uint8_t{0x12};
    const z80::RunResult r = run(coleco(), bus, trace, 0x8000, options);
    CB_CHECK_EQ(r.exit(), "return");
    CB_CHECK_EQ(r.regs.a, 0x80);   // the VDP status: the frame flag
    CB_CHECK((trace.calls == std::vector<std::string>{"BLK_WRT_VRAM"}));
    CB_CHECK((trace.ports == std::vector<std::string>{"out bf 12", "in bf 80"}));
});

CB_TEST("emulate.z80_machines.a_call_to_a_bios_address_with_no_name_is_noted_by_its_address", [] {
    Bytes mem = memory_with(0x8000, {0xCD, 0x00, 0x10, 0xC9});   // CALL $1000
    stub_firmware(coleco(), mem);
    Trace trace;
    z80::Bus bus = bus_for(coleco(), mem, trace);
    (void)run(coleco(), bus, trace, 0x8000);
    CB_CHECK((trace.calls == std::vector<std::string>{"1000"}));
});

CB_TEST("emulate.z80_machines.the_cpc_ports_are_noted_with_all_sixteen_bits", [] {
    // LD BC,$7F8D; OUT (C),C; CALL $BB06 (KM_WAIT_CHAR); LD BC,$F5FF; IN A,(C); RET
    Bytes mem = memory_with(0x4000, {0x01, 0x8D, 0x7F, 0xED, 0x49, 0xCD, 0x06, 0xBB, 0x01, 0xFF, 0xF5, 0xED, 0x78, 0xC9});
    stub_firmware(cpc(), mem);
    CB_CHECK_EQ(mem[0xBB06], 0xC9);
    CB_CHECK_EQ(mem[0xB8FF], 0x00);
    Trace trace;
    z80::Bus bus = bus_for(cpc(), mem, trace);
    const z80::RunResult r = run(cpc(), bus, trace, 0x4000);
    CB_CHECK_EQ(r.exit(), "return");
    CB_CHECK((trace.ports == std::vector<std::string>{"out 7f8d 8d", "in f5ff ff"}));
    CB_CHECK((trace.calls == std::vector<std::string>{"KM_WAIT_CHAR"}));
    CB_CHECK_EQ(r.regs.a, 0xFF);   // no key pressed, the vertical sync set
});

CB_TEST("emulate.z80_machines.in_firmware_is_the_machines_own_region", [] {
    CB_CHECK(in_firmware(coleco(), 0x1FFF));
    CB_CHECK(!in_firmware(coleco(), 0x2000));
    CB_CHECK(in_firmware(cpc(), 0xB900));
    CB_CHECK(in_firmware(cpc(), 0xBDFF));
    CB_CHECK(!in_firmware(cpc(), 0xBE00));
});

}  // namespace
}  // namespace cb::emulate::z80_machines
