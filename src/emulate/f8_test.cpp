// cb-unity: alone
// The F8 interpreter: the status flags, the scratchpad and ISAR, the data counter, loops and calls, decimal add, the ports as logged accesses, the stops and the limit. Programs are
// written in the assembler's own syntax and run from $0800.
#include "emulate/f8.hpp"

#include <string>

#include "build/f8_asm.hpp"
#include "core/testing.hpp"

namespace cb::emulate::f8 {
namespace {

struct Rig {
    Bus bus;
    RunResult result;

    RunResult run(const std::string& program, RunOptions options = {}) {
        const build::byte_asm::Image image = build::f8_asm::build("        ORG $0800\n" + program + "\n        POP\n");
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
        LIS 5
        AI $FB
        LR J,W
        LR A,R9
        LR R3,A
        LI $7F
        AI $01
        LR J,W
    )");
    CB_CHECK_EQ(m.result.exit(), "return");
    CB_CHECK_EQ(m.r(3), unsigned(w::S | w::C | w::Z));   // 5 - 5 = 0: positive, carry, zero
    CB_CHECK_EQ(m.r(9), unsigned(w::O));                 // $7F + 1 = $80: overflow, negative
});

CB_TEST("emulate.f8.the_scratchpad_through_isar", [] {
    Rig m;
    m.run(R"(
        LISU 1
        LISL 0
        LI $11
        LR I,A
        LI $22
        LR I,A
        LI $33
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
        DCI $2800
        LI $AB
        ST
        LI $CD
        ST
        DCI $2800
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
        LIS 5
        LR R1,A
        LIS 0
        LR R2,A
    loop:
        LR A,R2
        AS R1
        LR R2,A
        DS R1
        BF 4,loop
        PI sub
        LR R5,A
        BR end
    sub:
        LIS 7
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
        LI $19
        LR R1,A
        LI $28
        ASD R1
        LR R2,A
        LI $F0
        NI $3C
        LR R3,A
        XI $FF
        LR R4,A
    )");
    CB_CHECK_EQ(m.r(2), 0x47u);
    CB_CHECK_EQ(m.r(3), 0x30u);
    CB_CHECK_EQ(m.r(4), 0xCFu);
});

CB_TEST("emulate.f8.ports_are_logged_accesses", [] {
    Rig m;
    m.run(R"(
        LI $0F
        OUT $01
        INS 4
        LR R6,A
        OUTS 5
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
        m.run("        DB $2D\n");
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
