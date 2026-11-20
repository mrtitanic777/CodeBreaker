// cb-unity: alone
// The CP1610 interpreter: flags, every addressing mode, SDBD, the stack, branches and calls, shifts, the status word, the stops and the device log. Programs are written in the
// assembler's own syntax and run from $5000.
#include "emulate/cp1610.hpp"

#include <string>

#include "build/cp1610_asm.hpp"
#include "core/testing.hpp"

namespace cb::emulate::cp1610 {
namespace {

// A machine with the program at $5000 (read-only), 768 words of RAM at $0100 and a logged device at $0000-$007F that reads $A5.
struct Rig {
    Bus bus;
    RunResult result;

    RunResult run(const std::string& program, RunOptions options = {}) {
        const Bytes raw = build::cp1610_asm::build("        FORMAT RAW\n        SEGMENT $50,$50\n        ORG $5000\n" + program + "\n        MOVR R5,R7\n");
        std::vector<std::uint16_t> words;
        for (std::size_t i = 0; i + 1 < raw.size(); i += 2) words.push_back(static_cast<std::uint16_t>(raw[i] << 8 | raw[i + 1]));
        bus.memory(0x5000, 0x50FF, words, false);
        bus.memory(0x0100, 0x03FF, {}, true);
        bus.device(0x0000, 0x007F, [](std::uint16_t) -> std::uint16_t { return 0xA5; }, {}, true, 0x3FFF);
        result = emulate::cp1610::run(bus, 0x5000, options);
        return result;
    }
    unsigned r(unsigned n) const { return result.regs.r[n]; }
    std::uint16_t ram(std::uint16_t address) { return bus.words(0x0100)[address - 0x0100]; }
};

}  // namespace

CB_TEST("emulate.cp1610.the_flags_of_the_arithmetic", [] {
    Rig m;
    m.run(R"(
        SDBD
        MVII #$7FFF,R0
        ADDI #1,R0
        GSWD R0
        MVII #5,R1
        CMPI #7,R1
        GSWD R2
        MVII #7,R4
        CMPI #7,R4
        GSWD R3
    )");
    CB_CHECK_EQ(m.result.exit(), "return");
    CB_CHECK_EQ(m.r(0), 0xA0A0u);                   // S and OV, in both bytes
    CB_CHECK_EQ(m.r(1), 5u);                        // (CMP does not write)
    CB_CHECK_EQ(m.r(2), 0x8080u);                   // 5 - 7: S, and the carry clear for a borrow
    CB_CHECK_EQ(m.r(3), 0x5050u);                   // 7 - 7: zero, no borrow
});

CB_TEST("emulate.cp1610.every_addressing_mode", [] {
    Rig m;
    m.run(R"(
        MVII #$123,R1
        MVO R1,$0200
        MVI $0200,R2
        MVII #$0300,R4
        MVO@ R1,R4
        MVO@ R1,R4
        MVII #$0300,R3
        MVI@ R3,R0
        MVII #$0300,R4
        MVI@ R4,R1
        ADD@ R4,R1
        SUBI #1,R1
    )");
    CB_CHECK_EQ(m.result.exit(), "return");
    CB_CHECK_EQ(m.ram(0x200), 0x123u);
    CB_CHECK_EQ(m.r(2), 0x123u);
    CB_CHECK_EQ(m.ram(0x300), 0x123u);
    CB_CHECK_EQ(m.ram(0x301), 0x123u);
    CB_CHECK_EQ(m.r(0), 0x123u);
    CB_CHECK_EQ(m.r(4), 0x0302u);                   // two reads through @R4
    CB_CHECK_EQ(m.r(1), 0x123u + 0x123u - 1);
});

CB_TEST("emulate.cp1610.the_stack_and_double_byte_reads", [] {
    Rig m;
    m.run(R"(
        MVII #7,R0
        MVO@ R0,R6
        MVII #0,R0
        MVI@ R6,R1
        MVII #$34,R0
        MVO R0,$0300
        MVII #$12,R0
        MVO R0,$0301
        MVII #$0300,R4
        SDBD
        MVI@ R4,R2
    )");
    CB_CHECK_EQ(m.r(1), 7u);
    CB_CHECK_EQ(m.r(6), 0x02F1u);                   // back where the run put it (past the return address)
    CB_CHECK_EQ(m.r(2), 0x1234u);
    CB_CHECK_EQ(m.r(4), 0x0302u);
});

CB_TEST("emulate.cp1610.branches_calls_and_returns", [] {
    Rig m;
    m.run(R"(
        MVII #5,R0
        XORR R1,R1
    loop:
        ADDR R0,R1
        DECR R0
        BNEQ loop
        JSR R4,fn
        MVII #3,R3
        B done
    fn:
        MVII #9,R2
        MOVR R4,R7
    done:
        CMPI #4,R3
        BLT less
        MVII #1,R0
    less:
    )");
    CB_CHECK_EQ(m.result.exit(), "return");
    CB_CHECK_EQ(m.r(1), 15u);
    CB_CHECK_EQ(m.r(2), 9u);
    CB_CHECK_EQ(m.r(3), 3u);
    CB_CHECK_EQ(m.r(0), 0u);                        // 3 < 4: the branch skipped `MVII #1,R0`
});

CB_TEST("emulate.cp1610.shifts_rotates_and_the_status_word", [] {
    Rig m;
    m.run(R"(
        MVII #$81,R0
        SLL R0,2
        SDBD
        MVII #$8000,R1
        MOVR R1,R3
        SLR R1
        SAR R3
        SETC
        SDBD
        MVII #$8000,R2
        RLC R2
        MVII #$12,R1
        SWAP R1
        MOVR R1,R4
        MVII #$F0,R1
        RSWD R1
        GSWD R1
    )");
    CB_CHECK_EQ(m.r(0), 0x204u);
    CB_CHECK_EQ(m.r(3), 0xC000u);
    CB_CHECK_EQ(m.r(2), 1u);                        // the carry came round
    CB_CHECK_EQ(m.r(4), 0x1200u);
    CB_CHECK_EQ(m.r(1), 0xF0F0u);                   // RSWD of $F0 set all four flags
});

CB_TEST("emulate.cp1610.device_accesses_are_logged", [] {
    Rig m;
    m.run(R"(
        MVII #1,R0
        MVO R0,$0020
        MVI $0021,R1
    )");
    CB_CHECK_EQ(m.r(1), 0xA5u);
    CB_CHECK_EQ(m.result.io.size(), 2u);
    CB_CHECK(m.result.io[0].kind == DeviceAccess::Kind::Write && m.result.io[0].address == 0x20 && m.result.io[0].value == 1);
    CB_CHECK(m.result.io[1].kind == DeviceAccess::Kind::Read && m.result.io[1].address == 0x21 && m.result.io[1].value == 0xA5);
});

CB_TEST("emulate.cp1610.stops_and_limits", [] {
    {
        Rig m;
        m.run("        HLT\n");
        CB_CHECK_EQ(m.result.exit(), "stopped: halt at 5000");
    }
    {
        Rig m;
        m.run("        DECLE $278\n");   // MVO@ R0,R7: not an instruction
        CB_CHECK_EQ(m.result.exit(), "stopped: undefined instruction at 5000");
    }
    {
        Rig m;
        RunOptions o;
        o.max_steps = 50;
        m.run("    spin:\n        B spin\n", o);
        CB_CHECK_EQ(m.result.exit(), "limit");
        CB_CHECK_EQ(m.result.steps, 50u);
    }
});

}  // namespace cb::emulate::cp1610
