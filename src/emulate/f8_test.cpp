// cb-unity: alone
// The F8 interpreter: the status flags, the scratchpad and ISAR, the data counter, loops and calls, decimal add, the ports as logged accesses, the stops and the limit. Programs are
// written in the assembler's own syntax and run from 0x0800.
#include "emulate/f8.hpp"

#include <string>

#include "source/processors.hpp"
#include "core/testing.hpp"

namespace cb::emulate::f8 {
namespace {

struct Rig {
    Bus bus;
    RunResult result;

    RunResult run(const std::string& program, RunOptions options = {}) {
        const source::processors::Image image = source::processors::assemble_described(source::processors::find("f8"), ".org 0x0800\n" + program + "\n        POP\n", ".");
        bus.load(0x0800, image.data);
        bus.make_writable(0x2800, 0x2FFF);
        bus.set_in([](std::uint8_t) -> std::uint8_t { return 0xFF; });
        result = emulate::f8::run(bus, 0x0800, options);
        return result;
    }
    unsigned r(unsigned n) const { return result.regs.scratch[n]; }
};

}  // namespace

CB_TEST("emulate.f8.the_status_flags", [] {
    Rig m;
    m.run(R"(
        LIS 0x5
        AI 0xfb
        LR J,W
        LR A,R9
        LR R3,A
        LI 0x7f
        AI 0x1
        LR J,W
    )");
    CB_CHECK_EQ(m.result.exit(), "return");
    CB_CHECK_EQ(m.r(3), unsigned(w::S | w::C | w::Z));   // 5 - 5 = 0: positive, carry, zero
    CB_CHECK_EQ(m.r(9), unsigned(w::O));                 // 0x7F + 1 = 0x80: overflow, negative
});

CB_TEST("emulate.f8.the_scratchpad_through_isar", [] {
    Rig m;
    m.run(R"(
        LISU 0x1
        LISL 0x0
        LI 0x11
        LR I,A
        LI 0x22
        LR I,A
        LI 0x33
        LR D,A
        LR A,S
        LR R4,A
        LR A,IS
        LR R2,A
    )");
    CB_CHECK_EQ(m.r(8), 0x11u);
    CB_CHECK_EQ(m.r(10), 0x33u);
    CB_CHECK_EQ(m.r(4), 0x22u);
    CB_CHECK_EQ(m.r(2), 9u);                             // ISAR: up twice, down once
});

CB_TEST("emulate.f8.the_data_counter_and_memory", [] {
    Rig m;
    m.run(R"(
        DCI 0x2800
        LI 0xab
        ST
        LI 0xcd
        ST
        DCI 0x2800
        LM
        LR R2,A
        LM
        LR R3,A
    )");
    CB_CHECK_EQ(m.bus.read(0x2800), 0xAB);
    CB_CHECK_EQ(m.bus.read(0x2801), 0xCD);
    CB_CHECK_EQ(m.r(2), 0xABu);
    CB_CHECK_EQ(m.r(3), 0xCDu);
    CB_CHECK_EQ(m.result.regs.dc0, 0x2802u);
});

CB_TEST("emulate.f8.loops_calls_and_returns", [] {
    Rig m;
    m.run(R"(
        LR K,P
        LIS 0x5
        LR R1,A
        LIS 0x0
        LR R2,A
    loop:
        LR A,R2
        AS R1
        LR R2,A
        DS R1
        BF 0x4,loop
        PI sub
        LR R5,A
        BR end
    sub:
        LIS 0x7
        POP
    end:
        LR P,K
    )");
    CB_CHECK_EQ(m.result.exit(), "return");
    CB_CHECK_EQ(m.r(2), 15u);
    CB_CHECK_EQ(m.r(1), 0u);
    CB_CHECK_EQ(m.r(5), 7u);
});

CB_TEST("emulate.f8.decimal_add_and_logic", [] {
    Rig m;
    m.run(R"(
        LI 0x19
        LR R1,A
        LI 0x28
        ASD R1
        LR R2,A
        LI 0xf0
        NI 0x3c
        LR R3,A
        XI 0xff
        LR R4,A
    )");
    CB_CHECK_EQ(m.r(2), 0x47u);
    CB_CHECK_EQ(m.r(3), 0x30u);
    CB_CHECK_EQ(m.r(4), 0xCFu);
});

CB_TEST("emulate.f8.ports_are_logged_accesses", [] {
    Rig m;
    m.run(R"(
        LI 0xf
        OUT 0x1
        INS 0x4
        LR R6,A
        OUTS 0x5
    )");
    CB_CHECK_EQ(m.result.io.size(), 3u);
    CB_CHECK(m.result.io[0].kind == PortAccess::Kind::Out && m.result.io[0].port == 1 && m.result.io[0].value == 0x0F);
    CB_CHECK(m.result.io[1].kind == PortAccess::Kind::In && m.result.io[1].port == 4 && m.result.io[1].value == 0xFF);
    CB_CHECK(m.result.io[2].kind == PortAccess::Kind::Out && m.result.io[2].port == 5 && m.result.io[2].value == 0xFF);
    CB_CHECK_EQ(m.r(6), 0xFFu);
});

CB_TEST("emulate.f8.stops_and_limits", [] {
    {
        Rig m;
        m.run("        .byte 0x2d\n");
        CB_CHECK_EQ(m.result.exit(), "stopped: undefined opcode 2D at 0800");
    }
    {
        Rig m;
        RunOptions o;
        o.max_steps = 50;
        m.run("    spin:\n        BR spin\n", o);
        CB_CHECK_EQ(m.result.exit(), "limit");
        CB_CHECK_EQ(m.result.steps, 50u);
    }
});

}  // namespace cb::emulate::f8
