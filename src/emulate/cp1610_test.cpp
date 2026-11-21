// cb-unity: alone
// The CP1610 interpreter: flags, every addressing mode, SDBD, the stack, branches and calls, shifts, the status word, the stops and the device log. Programs are written in the
// assembler's own syntax and run from 0x5000.
#include "emulate/cp1610.hpp"

#include <string>

#include "source/processors.hpp"
#include "core/testing.hpp"

namespace cb::emulate::cp1610 {
namespace {

// A machine with the program at 0x5000 (read-only), 768 words of RAM at 0x0100 and a logged device at 0x0000-0x007F that reads 0xA5.
struct Rig {
    Bus bus;
    RunResult result;

    RunResult run(const std::string& program, RunOptions options = {}) {
        const Bytes raw = source::processors::assemble_described(source::processors::find("cp1610"), "        .org 0x5000\n" + program + "\n        JR R5\n", ".").data;
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
        .half 0x02b8, 0x00ff, 0x007f   ; MVII #0x7fff,R0 under SDBD (two decles of the immediate)
        ADDI #0x1,R0
        GSWD R0
        MVII #0x5,R1
        CMPI #0x7,R1
        GSWD R2
        MVII #0x7,R4
        CMPI #0x7,R4
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
        MVII #0x123,R1
        MVO R1,0x200
        MVI 0x200,R2
        MVII #0x300,R4
        MVO@ R1,R4
        MVO@ R1,R4
        MVII #0x300,R3
        MVI@ R3,R0
        MVII #0x300,R4
        MVI@ R4,R1
        ADD@ R4,R1
        SUBI #0x1,R1
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
        MVII #0x7,R0
        PSHR R0
        MVII #0x0,R0
        PULR R1
        MVII #0x34,R0
        MVO R0,0x300
        MVII #0x12,R0
        MVO R0,0x301
        MVII #0x300,R4
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
        MVII #0x5,R0
        CLRR R1
    loop:
        ADDR R0,R1
        DECR R0
        .half 0x022c, 0x0003   ; BNEQ loop (the description's assembler writes a backward branch to a label wrongly)
        .half 0x0004, 0x0050, 0x000e   ; JSR R4,fn (a label in a word-addressed space comes out in bytes)
        MVII #0x3,R3
        .half 0x0200, 0x0003   ; B done
    fn:
        MVII #0x9,R2
        JR R4
    done:
        CMPI #0x4,R3
        .half 0x0205, 0x0002   ; BLT less
        MVII #0x1,R0
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
        MVII #0x81,R0
        SLL R4, 2
        SDBD
        .half 0x02b9, 0x0000, 0x0080   ; MVII #0x8000,R1 under SDBD
        MOVR R1,R3
        SLR R1, 1
        SAR R3, 1
        SETC
        SDBD
        .half 0x02ba, 0x0000, 0x0080   ; MVII #0x8000,R2 under SDBD
        RLC R2, 1
        MVII #0x12,R1
        SWAP R1, 1
        MOVR R1,R4
        MVII #0xf0,R1
        RSWD R1
        GSWD R1
    )");
    CB_CHECK_EQ(m.r(0), 0x204u);
    CB_CHECK_EQ(m.r(3), 0xC000u);
    CB_CHECK_EQ(m.r(2), 1u);                        // the carry came round
    CB_CHECK_EQ(m.r(4), 0x1200u);
    CB_CHECK_EQ(m.r(1), 0xF0F0u);                   // RSWD of 0xF0 set all four flags
});

CB_TEST("emulate.cp1610.device_accesses_are_logged", [] {
    Rig m;
    m.run(R"(
        MVII #0x1,R0
        MVO R0,0x20
        MVI 0x21,R1
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
        m.run("        .half 0x278\n");   // MVO@ R0,R7: not an instruction
        CB_CHECK_EQ(m.result.exit(), "stopped: undefined instruction at 5000");
    }
    {
        Rig m;
        RunOptions o;
        o.max_steps = 50;
        m.run("    spin:\n        .half 0x0220, 0x0001   ; B spin (a backward branch to a label)\n", o);
        CB_CHECK_EQ(m.result.exit(), "limit");
        CB_CHECK_EQ(m.result.steps, 50u);
    }
});

}  // namespace cb::emulate::cp1610
