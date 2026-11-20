// cb-unity: alone
// The 8048 interpreter: the accumulator and its flags, the register banks, indirect addressing, loops and calls, the ports and the data space as logged accesses, the stops and the
// limit. Programs are written in the assembler's own syntax and run from $0400.
#include "emulate/i8048.hpp"

#include <string>

#include "build/i8048_asm.hpp"
#include "core/testing.hpp"

namespace cb::emulate::i8048 {
namespace {

struct Rig {
    Bus bus;
    RunResult result;

    RunResult run(const std::string& program, RunOptions options = {}) {
        const build::byte_asm::Image image = build::i8048_asm::build("        ORG $0400\n" + program + "\n        RET\n");
        bus.load(0x400, image.data);
        bus.set_read([](Place place, std::uint8_t address) -> std::uint8_t { return place == Place::Data ? (address == 0xA1 ? 0 : 0x5A) : 0xFF; });
        result = emulate::i8048::run(bus, 0x400, options);
        return result;
    }
    unsigned r(unsigned n) const { return result.regs.ram[n]; }
};

}  // namespace

CB_TEST("emulate.i8048.the_accumulator_and_its_flags", [] {
    Rig m;
    m.run(R"(
        MOV A,#$F0
        ADD A,#$20
        MOV R2,A
        MOV A,PSW
        MOV R3,A
        MOV A,#$09
        ADD A,#$01
        DA A
        MOV R4,A
        MOV A,#$81
        RL A
        MOV R5,A
        MOV A,#$12
        SWAP A
        MOV R6,A
    )");
    CB_CHECK_EQ(m.result.exit(), "return");
    CB_CHECK_EQ(m.r(2), 0x10u);                      // $F0 + $20: the carry is set
    CB_CHECK_EQ(m.r(3), 0x89u);                      // the PSW: C, bit 3, and the stack pointer (the return address is on the stack)
    CB_CHECK_EQ(m.r(4), 0x10u);                      // 9 + 1 adjusted to BCD
    CB_CHECK_EQ(m.r(5), 0x03u);
    CB_CHECK_EQ(m.r(6), 0x21u);
});

CB_TEST("emulate.i8048.registers_banks_and_indirect_addressing", [] {
    Rig m;
    m.run(R"(
        MOV R0,#$20
        MOV A,#$AB
        MOV @R0,A
        MOV R1,#$20
        MOV A,@R1
        MOV R2,A
        SEL RB1
        MOV R0,#7
        SEL RB0
        MOV R3,#$5C
        MOV @R1,#$3C
        XCHD A,@R1
    )");
    CB_CHECK_EQ(m.r(2), 0xABu);
    CB_CHECK_EQ(m.result.regs.ram[24], 7);           // bank 1's R0
    CB_CHECK_EQ(m.result.regs.ram[0x20], 0x3B);      // @R1 was $3C, A was $AB: the low nibbles swapped
    CB_CHECK_EQ(m.result.regs.a, 0xACu);
});

CB_TEST("emulate.i8048.loops_calls_and_the_stack", [] {
    Rig m;
    m.run(R"(
        MOV R1,#5
        MOV A,#0
    loop:
        ADD A,#2
        DJNZ R1,loop
        MOV R4,A
        CALL sub
        MOV R5,A
        JMP end
    sub:
        MOV A,#$77
        RET
    end:
    )");
    CB_CHECK_EQ(m.result.exit(), "return");
    CB_CHECK_EQ(m.r(4), 10u);
    CB_CHECK_EQ(m.r(1), 0u);
    CB_CHECK_EQ(m.r(5), 0x77u);
});

CB_TEST("emulate.i8048.ports_and_the_data_space_are_logged_accesses", [] {
    Rig m;
    m.run(R"(
        MOV A,#$0F
        OUTL P1,A
        ORL P1,#$F0
        IN A,P2
        MOV R0,#$A1
        MOVX A,@R0
        MOV R3,A
        MOV A,#$33
        MOVX @R0,A
    )");
    CB_CHECK_EQ(m.result.io.size(), 5u);
    CB_CHECK(m.result.io[0].kind == DeviceAccess::Kind::Write && m.result.io[0].place == Place::P1 && m.result.io[0].value == 0x0F);
    CB_CHECK(m.result.io[1].place == Place::P1 && m.result.io[1].value == 0xFF);
    CB_CHECK(m.result.io[2].kind == DeviceAccess::Kind::Read && m.result.io[2].place == Place::P2);
    CB_CHECK(m.result.io[3].place == Place::Data && m.result.io[3].address == 0xA1);
    CB_CHECK(m.result.io[4].kind == DeviceAccess::Kind::Write && m.result.io[4].value == 0x33);
    CB_CHECK_EQ(m.r(3), 0u);
    CB_CHECK_EQ(m.result.regs.p1, 0xFF);
});

CB_TEST("emulate.i8048.stops_and_limits", [] {
    {
        Rig m;
        m.run("        DB $01\n");
        CB_CHECK_EQ(m.result.exit(), "stopped: undefined opcode 01 at 0400");
    }
    {
        Rig m;
        RunOptions o;
        o.max_steps = 50;
        m.run("    spin:\n        JMP spin\n", o);
        CB_CHECK_EQ(m.result.exit(), "limit");
        CB_CHECK_EQ(m.result.steps, 50u);
    }
});

}  // namespace cb::emulate::i8048
