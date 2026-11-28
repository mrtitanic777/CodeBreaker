// cb-unity: alone
// The Channel F lane's F8 on the one emulator (engine/pcode_cpu_f8.cpp). The own core (emulate/f8's interpreter) was retired once the emulator matched it: on every opcode
// and on random programs (8,000 runs each), on every pair of operands through the arithmetic that sets flags (458,752 runs), and on the lane's tests. What those runs gave
// on both is kept here as a digest (CRC-32 of every run's exit, steps, registers, port accesses and memory), so a change in what the emulator does shows. The lane has no
// homebrew folder in the samples. The own core's unit tests follow, on the emulator: programs written in the assembler's own syntax, run from 0x0800.
#include <cstdio>
#include <format>
#include <random>
#include <string>

#include "core/crypto.hpp"
#include "core/testing.hpp"
#include "emulate/f8.hpp"
#include "emulate/cpu/pcode_cpu_f8.hpp"
#include "source/processors.hpp"

namespace cb {

namespace f8c = cb::emulate::f8;

namespace {

// A run as text: its exit, steps, every register, the scratchpad and the port accesses.
std::string f8_record(const f8c::RunResult& r) {
    std::string out = std::format("{}|{}|{:X} {:X} {:X} {:X} {:X} {:X} {:X}|", r.exit(), r.steps, r.regs.a, r.regs.status, r.regs.isar, r.regs.pc0, r.regs.pc1, r.regs.dc0, r.regs.dc1);
    for (const std::uint8_t s : r.regs.scratch) out += std::format("{:02X}", s);
    for (const f8c::PortAccess& x : r.io) out += std::format("|{}{:X}={:X}", x.kind == f8c::PortAccess::Kind::In ? 'i' : 'o', x.port, x.value);
    return out;
}

std::uint32_t f8_fold(std::uint32_t digest, const f8c::RunResult& r, ByteView memory) {
    const std::string text = f8_record(r);
    digest = crypto::crc32(ByteView(reinterpret_cast<const std::uint8_t*>(text.data()), text.size()), digest);
    return crypto::crc32(memory, digest);
}

// A machine as the lane makes it: a ROM, some RAM that is written, ports that answer. `low_ram` makes the BIOS area writable too (the lane's is not).
struct F8Machine {
    f8c::Bus bus;
    F8Machine(const Bytes& image, bool low_ram) {
        bus.load(0, image);
        bus.make_writable(0x2800, 0x2FFF);
        if (low_ram) bus.make_writable(0, 0x7FF);
        bus.set_in([](std::uint8_t port) -> std::uint8_t { return static_cast<std::uint8_t>(port * 37 + 5); });
    }
};

constexpr std::uint16_t kF8Rom = 0x0800, kF8Ram = 0x2800;

std::uint32_t f8_random_digest(bool single) {
    std::mt19937 rng(single ? 20261124u : 20261125u);
    std::uint32_t digest = 0;
    for (unsigned n = 0; n < 8000; ++n) {
        Bytes image(0x10000, 0);
        for (std::size_t i = 0x800; i < 0x1000; ++i) image[i] = static_cast<std::uint8_t>(rng());
        for (std::size_t i = kF8Ram; i < 0x3000; ++i) image[i] = static_cast<std::uint8_t>(rng());
        std::uint16_t entry = kF8Rom;
        if (single) {   // every opcode, each many times with other registers and operands
            entry = (n / 256) % 3 == 0 ? kF8Ram : kF8Rom;
            image[entry] = static_cast<std::uint8_t>(n % 256);
        } else {
            entry = (rng() & 1) ? kF8Ram : kF8Rom;
        }
        f8c::RunOptions options;
        options.max_steps = single ? 1 : 40;
        // values at the edges of the byte, where a flag of an add or a compare turns (the overflow of a subtraction whose partial sums wrap is one)
        const auto value = [&]() -> std::uint8_t {
            static constexpr std::uint8_t kEdges[] = {0x00, 0x01, 0x7F, 0x80, 0x81, 0xFE, 0xFF};
            return (rng() % 3) == 0 ? kEdges[rng() % sizeof kEdges] : (rng() & 3) == 0 ? static_cast<std::uint8_t>(rng() & 0x0F) : static_cast<std::uint8_t>(rng());
        };
        if (single) image[entry + 1] = value();
        image[0] = value();   // (what DC0 = 0 reads)
        options.a = value();
        for (std::uint8_t& r : options.r) r = value();
        const bool low_ram = (rng() & 3) == 0;
        F8Machine machine(image, low_ram);
        const f8c::RunResult rr = emulate::pcode_cpu::run(machine.bus, entry, options);
        digest = f8_fold(digest, rr, machine.bus.memory());
    }
    return digest;
}

}  // namespace

CB_TEST("emulate.pcode_cpu.f8_instructions_run_as_the_own_core_did", [] {
    const std::uint32_t digest = f8_random_digest(true);
    std::printf("pcode_cpu f8 instructions: digest %08X\n", digest);
    CB_CHECK_EQ(digest, 0x3CBB9205u);
});

CB_TEST("emulate.pcode_cpu.f8_programs_run_as_the_own_core_did", [] {
    const std::uint32_t digest = f8_random_digest(false);
    std::printf("pcode_cpu f8 programs: digest %08X\n", digest);
    CB_CHECK_EQ(digest, 0x50E5EFCCu);
});

// Every pair of operands through the arithmetic that sets flags: an immediate, a byte of memory (DC0 is 0: the first byte of the image) and a scratchpad register, as the
// operand of a compare, an add and a decimal add, with the accumulator at each value.
CB_TEST("emulate.pcode_cpu.f8_arithmetic_runs_as_the_own_core_did_on_every_pair_of_operands", [] {
    std::uint32_t digest = 0;
    for (const unsigned op : {0x25u, 0x24u, 0x8Du, 0x88u, 0x89u, 0xC0u, 0xD0u}) {   // CI, AI, CM, AM, AMD, AS R0, ASD R0
        for (unsigned a = 0; a < 256; ++a) {
            for (unsigned x = 0; x < 256; ++x) {
                Bytes image(0x10000, 0);
                image[kF8Rom] = static_cast<std::uint8_t>(op);
                image[kF8Rom + 1] = static_cast<std::uint8_t>(x);   // (the immediate of CI and AI)
                image[0] = static_cast<std::uint8_t>(x);            // (what CM, AM and AMD read at DC0 = 0)
                f8c::RunOptions options;
                options.max_steps = 1;
                options.a = static_cast<std::uint8_t>(a);
                options.r[0] = static_cast<std::uint8_t>(x);
                F8Machine machine(image, false);
                const f8c::RunResult rr = emulate::pcode_cpu::run(machine.bus, kF8Rom, options);
                digest = f8_fold(digest, rr, {});
            }
        }
    }
    std::printf("pcode_cpu f8 arithmetic: digest %08X\n", digest);
    CB_CHECK_EQ(digest, 0xC62025F6u);
});

}  // namespace cb

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
        result = emulate::pcode_cpu::run(bus, 0x0800, options);
        return result;
    }
    unsigned r(unsigned n) const { return result.regs.scratch[n]; }
};

}  // namespace

CB_TEST("emulate.pcode_cpu.f8_the_status_flags", [] {
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

CB_TEST("emulate.pcode_cpu.f8_the_scratchpad_through_isar", [] {
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

CB_TEST("emulate.pcode_cpu.f8_the_data_counter_and_memory", [] {
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

CB_TEST("emulate.pcode_cpu.f8_loops_calls_and_returns", [] {
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

CB_TEST("emulate.pcode_cpu.f8_decimal_add_and_logic", [] {
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

CB_TEST("emulate.pcode_cpu.f8_ports_are_logged_accesses", [] {
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

CB_TEST("emulate.pcode_cpu.f8_stops_and_limits", [] {
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
