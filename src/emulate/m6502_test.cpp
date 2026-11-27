// emulate/m6502_test.cpp -- the 6502 machine's bus and its device ranges (the CPU runs on the one emulator: engine/pcode_cpu_m6502_test).

#include "emulate/m6502.hpp"

#include <string>
#include <vector>

#include "core/error.hpp"
#include "core/testing.hpp"

namespace m = cb::emulate::m6502;
using cb::Bytes;

CB_TEST("emulate.m6502.bus_ranges_and_log", [] {
    m::Bus bus;
    std::vector<std::pair<int, int>> written;
    bus.map(0x2000, 0x2007, [](std::uint16_t a) -> std::uint8_t { return static_cast<std::uint8_t>(a * 7 + 3); },
            [&](std::uint16_t a, std::uint8_t v) { written.emplace_back(a, v); });
    bus.map(0x4000, 0x4017, {}, {});   // a range with no callbacks: reads 0, swallows writes
    bus.write(0x10, 0x55);
    CB_CHECK_EQ(bus.read(0x10), 0x55);
    CB_CHECK(bus.log().empty());       // plain RAM is not logged
    CB_CHECK_EQ(bus.read(0x2003), static_cast<std::uint8_t>(0x2003 * 7 + 3));
    bus.write(0x2001, 0x1FF & 0xFF);
    bus.write(0x4005, 9);
    CB_CHECK_EQ(bus.read(0x4005), 0);
    CB_CHECK_EQ(bus.memory()[0x2001], 0);   // the RAM behind a device range is untouched
    CB_CHECK_EQ(bus.memory()[0x4005], 0);
    CB_CHECK_EQ(written.size(), 1u);
    CB_CHECK_EQ(written[0].first, 0x2001);
    CB_CHECK_EQ(written[0].second, 0xFF);
    CB_CHECK_EQ(bus.log().size(), 4u);
    CB_CHECK(bus.log()[0].kind == m::DeviceAccess::Kind::Read);
    CB_CHECK_EQ(bus.log()[0].address, 0x2003);
    CB_CHECK(bus.log()[1].kind == m::DeviceAccess::Kind::Write);
    CB_CHECK_EQ(bus.log()[1].value, 0xFF);
    CB_CHECK_EQ(bus.log()[3].address, 0x4005);
});

CB_TEST("emulate.m6502.bus_first_range_wins", [] {
    m::Bus bus;
    bus.map(0x100, 0x1FF, [](std::uint16_t) -> std::uint8_t { return 1; });
    bus.map(0x100, 0x1FF, [](std::uint16_t) -> std::uint8_t { return 2; });
    CB_CHECK_EQ(bus.read(0x150), 1);
});

CB_TEST("emulate.m6502.bus_over_the_callers_memory", [] {
    Bytes ram(m::kMemorySize, 0);
    {
        m::Bus bus = m::Bus::over(ram);
        bus.write(0x1234, 0xEE);
        CB_CHECK_EQ(bus.read(0x1234), 0xEE);
    }
    CB_CHECK_EQ(ram[0x1234], 0xEE);   // changed in place
    Bytes small(100, 0);
    CB_CHECK_THROWS_AS(cb::UsageError, (void)m::Bus::over(small));
    Bytes big(m::kMemorySize + 1, 0);
    CB_CHECK_THROWS_AS(cb::UsageError, (void)m::Bus(big));
    const Bytes image = {1, 2, 3};
    m::Bus own(image);
    CB_CHECK_EQ(own.read(2), 3);
    CB_CHECK_EQ(own.read(3), 0);
    CB_CHECK_EQ(own.memory().size(), m::kMemorySize);
    m::Bus moved = std::move(own);   // the memory goes with it
    CB_CHECK_EQ(moved.read(1), 2);
});

