// emulate/z80_test.cpp -- the Z80 machine's bus (the CPU runs on the one emulator: engine/pcode_cpu_z80_test).
#include "emulate/z80.hpp"

#include <format>
#include <string>
#include <vector>

#include "core/error.hpp"
#include "core/testing.hpp"

namespace z = cb::emulate::z80;
using cb::Bytes;

CB_TEST("emulate.z80.bus_ranges_and_log", [] {
    z::Bus bus;
    std::vector<std::pair<int, int>> written;
    bus.map(0xFF00, 0xFF0F, [](std::uint16_t a) -> std::uint8_t { return static_cast<std::uint8_t>(a + 1); },
            [&](std::uint16_t a, std::uint8_t v) { written.emplace_back(a, v); });
    bus.map(0xFE00, 0xFE00);   // no callbacks: reads 0, swallows writes
    bus.write(0x10, 7);
    CB_CHECK_EQ(bus.read(0x10), 7);
    CB_CHECK(bus.log().empty());
    CB_CHECK_EQ(bus.read(0xFF03), 4);
    bus.write(0xFF04, 9);
    bus.write(0xFE00, 1);
    CB_CHECK_EQ(bus.read(0xFE00), 0);
    CB_CHECK_EQ(written.size(), 1u);
    CB_CHECK_EQ(written[0].first, 0xFF04);
    CB_CHECK_EQ(bus.log().size(), 4u);
    CB_CHECK(bus.log()[0].kind == z::BusAccess::Kind::Read);
    CB_CHECK(bus.log()[1].kind == z::BusAccess::Kind::Write);
    CB_CHECK_EQ(z::to_text(z::BusAccess::Kind::In), "in");
    CB_CHECK_EQ(z::to_text(z::BusAccess::Kind::Write), "w");
});

CB_TEST("emulate.z80.bus_memory_ownership", [] {
    cb::Bytes ram(z::kMemorySize, 0);
    {
        z::Bus bus = z::Bus::over(ram);
        bus.write(0x4321, 0x99);
    }
    CB_CHECK_EQ(ram[0x4321], 0x99);   // in place
    cb::Bytes small(10, 0);
    CB_CHECK_THROWS_AS(cb::UsageError, (void)z::Bus::over(small));
    cb::Bytes big(z::kMemorySize + 1, 0);
    CB_CHECK_THROWS_AS(cb::UsageError, (void)z::Bus(big));
    const cb::Bytes image = {9, 8, 7};
    z::Bus own(image);
    CB_CHECK_EQ(own.read(1), 8);
    z::Bus moved = std::move(own);
    CB_CHECK_EQ(moved.read(2), 7);
});

