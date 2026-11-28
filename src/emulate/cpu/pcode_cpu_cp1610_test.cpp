// cb-unity: alone
// The Intellivision lane's CP1610 on the one emulator (engine/pcode_cpu_cp1610.cpp). The own core (emulate/cp1610's interpreter) was retired once the emulator matched it on
// random instructions and programs (24,000 runs) and on the lane's whole homebrew corpus (every function in every state, and the verdicts on edited copies). What the random
// runs gave on both is kept as a digest (CRC-32 of every run's exit, registers, flags, steps, device accesses and RAM), so a change in what the emulator does shows. The own
// core's unit tests follow, on the emulator.
#include <array>
#include <cstdio>
#include <format>
#include <memory>
#include <random>
#include <string>

#include "core/crypto.hpp"
#include "core/testing.hpp"
#include "emulate/cp1610.hpp"
#include "emulate/cpu/pcode_cpu_cp1610.hpp"
#include "source/processors.hpp"

using namespace cb;
namespace c = cb::emulate::cp1610;

namespace {

std::string describe_cp1610(const c::RunResult& a, const c::RunResult& b) {
    std::string out;
    if (a.exit() != b.exit()) out += std::format("exit('{}' / '{}') ", a.exit(), b.exit());
    for (unsigned i = 0; i < 8; ++i)
        if (a.regs.r[i] != b.regs.r[i]) out += std::format("r{}[own {:X} emu {:X}] ", i, a.regs.r[i], b.regs.r[i]);
    if (a.regs.status() != b.regs.status()) out += std::format("flags[own {:X} emu {:X}] ", a.regs.status(), b.regs.status());
    if (a.steps != b.steps) out += std::format("steps[{}/{}] ", a.steps, b.steps);
    if (a.io.size() != b.io.size()) out += std::format("io[{}/{}] ", a.io.size(), b.io.size());
    else
        for (std::size_t i = 0; i < a.io.size(); ++i)
            if (a.io[i].kind != b.io[i].kind || a.io[i].address != b.io[i].address || a.io[i].value != b.io[i].value) {
                out += std::format("io@{}[own {} {:X} {:X} emu {} {:X} {:X}] ", i, static_cast<int>(a.io[i].kind), a.io[i].address, a.io[i].value, static_cast<int>(b.io[i].kind),
                                   b.io[i].address, b.io[i].value);
                break;
            }
    return out;
}

// A bus as the Intellivision lane makes one, with a ROM of random words at $5000 and a counter in the STIC's reads (the order of reads shows).
struct Cp1610World {
    c::Bus bus;
    std::shared_ptr<unsigned> reads = std::make_shared<unsigned>(0);
    explicit Cp1610World(const std::vector<std::uint16_t>& rom, const std::vector<std::uint16_t>& ram, std::uint16_t base = 0x5000) {
        auto n = reads;
        bus.device(0x0000, 0x007F, [n](std::uint16_t a) -> std::uint16_t { return static_cast<std::uint16_t>((a * 3 + (*n)++ * 5) & 0x3FFF); }, {}, true, 0x3FFF);
        bus.memory(0x0100, 0x01EF, std::span<const std::uint16_t>(ram).subspan(0, 0xF0), true, false, 0xFF);
        bus.device(0x01F0, 0x01FF, [](std::uint16_t a) -> std::uint16_t { return a >= 0x01FE ? 0xFF : 0; }, {}, true, 0xFFFF);
        bus.memory(0x0200, 0x035F, std::span<const std::uint16_t>(ram).subspan(0xF0, 0x160), true, false, 0xFFFF);
        bus.memory(0x1000, 0x1FFF, std::vector<std::uint16_t>(0x1000, 0x00AF), false, false);
        bus.memory(0x3800, 0x39FF, {}, true, true, 0xFF);
        bus.memory(base, static_cast<std::uint16_t>(base + 0xFFF), rom, false);   // (a cartridge's ROM sits at $5000; one at $D000 has addresses with the top bit set)
    }
};

// A run folded into the digest: everything describe_cp1610 compares (against an empty run), each device access, and the RAM.
std::uint32_t cp1610_fold(std::uint32_t digest, const c::RunResult& r, c::Bus& bus) {
    std::string text = describe_cp1610(c::RunResult{}, r);
    for (const auto& x : r.io) text += std::format("|{}{:X}={:X}", static_cast<int>(x.kind), x.address, x.value);
    digest = crypto::crc32(ByteView(reinterpret_cast<const std::uint8_t*>(text.data()), text.size()), digest);
    for (const std::uint16_t lo : std::array<std::uint16_t, 3>{0x0100, 0x0200, 0x3800}) {
        const std::span<std::uint16_t> words = bus.words(lo);
        digest = crypto::crc32(ByteView(reinterpret_cast<const std::uint8_t*>(words.data()), words.size() * sizeof(std::uint16_t)), digest);
    }
    return digest;
}

}  // namespace

CB_TEST("emulate.pcode_cpu.the_cp1610_runs_random_programs_as_the_own_core_did", [] {
    std::mt19937 rng(20261122);
    constexpr unsigned total = 12000;
    std::uint32_t digest = 0;
    const auto pointer = [&]() -> std::uint16_t {
        switch (rng() % 6) {
            case 0: return static_cast<std::uint16_t>(rng() & 0x7F);
            case 1: return static_cast<std::uint16_t>(0x3800 + (rng() & 0x1FF));
            case 2: return static_cast<std::uint16_t>(rng());
            case 3: return static_cast<std::uint16_t>((rng() & 1 ? 0xD000 : 0x5000) + (rng() & 0xFFF));
            default: return static_cast<std::uint16_t>(0x0100 + (rng() % 0x260));
        }
    };
    for (unsigned n = 0; n < 2 * total; ++n) {
        const bool single = n < total;
        std::vector<std::uint16_t> rom(0x1000), ram(0x250);
        for (auto& w : rom) w = static_cast<std::uint16_t>(rng() & 0x3FF);
        for (auto& w : ram) w = static_cast<std::uint16_t>(rng() & 0xFFFF);
        rom[0xF00] = 0x00AF;   // (a return through R5 after the program)
        const std::uint16_t base = (rng() & 1) != 0 ? 0xD000 : 0x5000;
        c::RunOptions options;
        options.max_steps = single ? 1 : 40;
        if (single) {
            rom[0] = static_cast<std::uint16_t>(n % 1024);
            if ((n / 1024) % 4 == 3) {   // (after SDBD)
                rom[1] = rom[0];
                rom[0] = 0x001;
                options.max_steps = 2;
            }
        }
        for (auto& r : options.r) r = (rng() & 1) ? pointer() : static_cast<std::uint16_t>(rng());
        options.sp = (rng() & 3) == 0 ? pointer() : static_cast<std::uint16_t>(0x02F0);
        Cp1610World w(rom, ram, base);
        const c::RunResult rr = emulate::pcode_cpu::run(w.bus, base, options);
        digest = cp1610_fold(digest, rr, w.bus);
    }
    std::printf("pcode_cpu cp1610 instructions: digest %08X\n", digest);
    CB_CHECK_EQ(digest, 0xFF368DE5u);
});

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
        result = emulate::pcode_cpu::run(bus, 0x5000, options);
        return result;
    }
    unsigned r(unsigned n) const { return result.regs.r[n]; }
    std::uint16_t ram(std::uint16_t address) { return bus.words(0x0100)[address - 0x0100]; }
};

}  // namespace

CB_TEST("emulate.pcode_cpu.cp1610_the_flags_of_the_arithmetic", [] {
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

CB_TEST("emulate.pcode_cpu.cp1610_every_addressing_mode", [] {
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

CB_TEST("emulate.pcode_cpu.cp1610_the_stack_and_double_byte_reads", [] {
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

CB_TEST("emulate.pcode_cpu.cp1610_branches_calls_and_returns", [] {
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

CB_TEST("emulate.pcode_cpu.cp1610_shifts_rotates_and_the_status_word", [] {
    Rig m;
    m.run(R"(
        MVII #0x81,R0
        SLL R0, 2
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

CB_TEST("emulate.pcode_cpu.cp1610_device_accesses_are_logged", [] {
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

CB_TEST("emulate.pcode_cpu.cp1610_stops_and_limits", [] {
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
