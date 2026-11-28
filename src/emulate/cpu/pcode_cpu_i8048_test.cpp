// cb-unity: alone
// The Odyssey 2 lane's Intel 8048 on the one emulator (engine/pcode_cpu_i8048.cpp). The own core (emulate/i8048's interpreter) was retired once the emulator matched it on
// every opcode and on random programs (12,000 runs each), on every operand of ADD/DA, ADDC and XCHD (196,608 runs), and on the lane's tests (machine_o2_test); the lane has
// no homebrew folder in the samples. What those runs gave on both is kept as a digest (CRC-32 of every run's exit, steps, registers, accesses and data space), so a change in
// what the emulator does shows. The devices answer with a count of the accesses made, so a read made more or fewer times shows. The own core's unit tests follow, on the emulator.
#include <array>
#include <cstdio>
#include <cstdlib>
#include <format>
#include <map>
#include <memory>
#include <random>
#include <string>

#include "core/crypto.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "emulate/i8048.hpp"
#include "emulate/cpu/pcode_cpu_i8048.hpp"
#include "source/processors.hpp"

namespace cb {

namespace e48t = cb::emulate::i8048;

namespace {

std::string i8048_describe(const e48t::RunResult& a, const e48t::RunResult& b) {
    std::string out;
    if (a.exit() != b.exit()) out += std::format("exit('{}' / '{}') ", a.exit(), b.exit());
    if (a.steps != b.steps) out += std::format("steps[{}/{}] ", a.steps, b.steps);
    const auto reg = [&](const std::string& name, unsigned x, unsigned y) {
        if (x != y) out += std::format("{}[own {:X} emu {:X}] ", name, x, y);
    };
    const e48t::Registers &p = a.regs, &q = b.regs;
    reg("a", p.a, q.a);
    reg("pc", p.pc, q.pc);
    reg("c", p.c, q.c);
    reg("ac", p.ac, q.ac);
    reg("f0", p.f0, q.f0);
    reg("f1", p.f1, q.f1);
    reg("rb1", p.rb1, q.rb1);
    reg("mb1", p.mb1, q.mb1);
    reg("sp", p.sp, q.sp);
    reg("timer", p.timer, q.timer);
    reg("timer_flag", p.timer_flag, q.timer_flag);
    reg("timer_on", p.timer_on, q.timer_on);
    reg("counter_on", p.counter_on, q.counter_on);
    reg("ext_int", p.ext_int, q.ext_int);
    reg("timer_int", p.timer_int, q.timer_int);
    reg("p1", p.p1, q.p1);
    reg("p2", p.p2, q.p2);
    for (std::size_t i = 0; i < p.ram.size(); ++i) reg(std::format("ram{}", i), p.ram[i], q.ram[i]);
    if (a.fault.has_value() != b.fault.has_value()) out += "fault-presence ";
    else if (a.fault && (a.fault->pc != b.fault->pc || a.fault->reason != b.fault->reason)) out += std::format("fault['{}' / '{}'] ", a.fault->reason, b.fault->reason);
    if (a.io.size() != b.io.size()) out += std::format("io[{}/{}] ", a.io.size(), b.io.size());
    else
        for (std::size_t i = 0; i < a.io.size(); ++i)
            if (a.io[i].kind != b.io[i].kind || a.io[i].place != b.io[i].place || a.io[i].address != b.io[i].address || a.io[i].value != b.io[i].value) {
                out += std::format("io@{} ", i);
                break;
            }
    return out;
}

// A machine as the lane makes it: program memory, a data space that keeps what is written, and devices whose answers change with every access.
struct I8048Machine {
    e48t::Bus bus;
    std::shared_ptr<std::array<std::uint8_t, 256>> data = std::make_shared<std::array<std::uint8_t, 256>>();
    I8048Machine(const Bytes& image) {
        bus.load(0, image);
        for (std::size_t i = 0; i < data->size(); ++i) (*data)[i] = static_cast<std::uint8_t>(i * 7 + 1);
        auto reads = std::make_shared<unsigned>(0);
        auto d = data;
        bus.set_read([d, reads](e48t::Place place, std::uint8_t address) -> std::uint8_t {
            const unsigned n = (*reads)++;
            switch (place) {
                case e48t::Place::Data: return (*d)[address];
                case e48t::Place::T0: case e48t::Place::T1: case e48t::Place::Int: return static_cast<std::uint8_t>((n >> 1) & 1);
                default: return static_cast<std::uint8_t>(n * 37 + static_cast<unsigned>(place) * 11 + 0x5A);
            }
        });
        bus.set_write([d](e48t::Place place, std::uint8_t address, std::uint8_t value) {
            if (place == e48t::Place::Data) (*d)[address] = value;
        });
    }
};

// A run folded into the digest: everything i8048_describe compares (against an empty run), each access, and the data space.
std::uint32_t i8048_fold(std::uint32_t digest, const e48t::RunResult& r, ByteView data) {
    std::string text = i8048_describe(e48t::RunResult{}, r);
    for (const auto& x : r.io) text += std::format("|{}{}{:X}={:X}", static_cast<int>(x.kind), static_cast<int>(x.place), x.address, x.value);
    digest = crypto::crc32(ByteView(reinterpret_cast<const std::uint8_t*>(text.data()), text.size()), digest);
    return crypto::crc32(data, digest);
}

std::uint32_t i8048_random_digest(bool single) {
    std::mt19937 rng(single ? 20261126u : 20261127u);
    std::uint32_t digest = 0;
    for (unsigned n = 0; n < 12000; ++n) {
        Bytes image(4096, 0);
        for (std::size_t i = 0x400; i < 0x1000; ++i) image[i] = static_cast<std::uint8_t>(rng());
        // where the run starts: mostly inside a page, now and then in the last bytes of one (a jump or a table read there leaves the page)
        std::uint16_t entry = static_cast<std::uint16_t>(0x400 + (rng() % 0xB00));
        if (const unsigned where = rng() % 8; where == 0) entry = static_cast<std::uint16_t>((entry & 0xF00) | 0xFE);
        else if (where == 1) entry = static_cast<std::uint16_t>((entry & 0xF00) | 0xFF);
        if (entry > 0xFF8) entry = 0xFF0;
        if (single) image[entry] = static_cast<std::uint8_t>(n % 256);   // every opcode, each many times with other registers and operands
        e48t::RunOptions options;
        options.max_steps = single ? 1 : 40;
        options.a = static_cast<std::uint8_t>((rng() & 3) == 0 ? rng() & 0x0F : rng());
        for (std::uint8_t& r : options.r) r = static_cast<std::uint8_t>((rng() & 1) ? rng() & 0x3F : rng());
        I8048Machine machine(image);
        const e48t::RunResult rr = emulate::pcode_cpu::run(machine.bus, entry, options);
        digest = i8048_fold(digest, rr, ByteView(machine.data->data(), machine.data->size()));
    }
    return digest;
}

}  // namespace

CB_TEST("emulate.pcode_cpu.i8048_instructions_run_as_the_own_core_did", [] {
    const std::uint32_t digest = i8048_random_digest(true);
    std::printf("pcode_cpu i8048 instructions: digest %08X\n", digest);
    CB_CHECK_EQ(digest, 0x80BFA05Du);
});

CB_TEST("emulate.pcode_cpu.i8048_programs_run_as_the_own_core_did", [] {
    const std::uint32_t digest = i8048_random_digest(false);
    std::printf("pcode_cpu i8048 programs: digest %08X\n", digest);
    CB_CHECK_EQ(digest, 0x648A0AE4u);
});

// Every accumulator and operand through ADD and then DA (the carries DA reads are those the add left), ADDC, and the register exchange XCHD at every pair of low digits.
CB_TEST("emulate.pcode_cpu.i8048_decimal_adjust_and_exchange_run_as_the_own_core_did_on_every_pair_of_operands", [] {
    std::uint32_t digest = 0;
    for (unsigned program = 0; program < 3; ++program) {
        for (unsigned a = 0; a < 256; ++a) {
            for (unsigned x = 0; x < 256; ++x) {
                Bytes image(4096, 0);
                std::uint16_t at = 0x400;
                if (program == 0) {   // ADD A,#x then DA A
                    image[at] = 0x03, image[at + 1] = static_cast<std::uint8_t>(x), image[at + 2] = 0x57;
                } else if (program == 1) {   // ADD A,#x then ADDC A,#x then DA A
                    image[at] = 0x03, image[at + 1] = static_cast<std::uint8_t>(x), image[at + 2] = 0x13, image[at + 3] = static_cast<std::uint8_t>(x), image[at + 4] = 0x57;
                } else {   // XCHD A,@R0 with R0 pointing at a byte of internal RAM that holds x (R1 is stored there: R0 = 1)
                    image[at] = 0x30;
                }
                e48t::RunOptions options;
                options.max_steps = program == 0 ? 2 : program == 1 ? 3 : 1;
                options.a = static_cast<std::uint8_t>(a);
                options.r[0] = program == 2 ? 1 : 0;
                options.r[1] = static_cast<std::uint8_t>(x);
                I8048Machine machine(image);
                digest = i8048_fold(digest, emulate::pcode_cpu::run(machine.bus, at, options), {});
            }
        }
    }
    std::printf("pcode_cpu i8048 arithmetic: digest %08X\n", digest);
    CB_CHECK_EQ(digest, 0x0EDD200Au);
});

}  // namespace cb

namespace cb::emulate::i8048 {
namespace {

struct Rig {
    Bus bus;
    RunResult result;

    RunResult run(const std::string& program, RunOptions options = {}) {
        const source::processors::Image image = source::processors::assemble_described(source::processors::find("i8048"), ".org 0x0400\n" + program + "\n        RET\n", ".");
        bus.load(0x400, image.data);
        bus.set_read([](Place place, std::uint8_t address) -> std::uint8_t { return place == Place::Data ? (address == 0xA1 ? 0 : 0x5A) : 0xFF; });
        result = emulate::pcode_cpu::run(bus, 0x400, options);
        return result;
    }
    unsigned r(unsigned n) const { return result.regs.ram[n]; }
};

}  // namespace

CB_TEST("emulate.pcode_cpu.i8048_the_accumulator_and_its_flags", [] {
    Rig m;
    m.run(R"(
        MOV A,#0xf0
        ADD A,#0x20
        MOV R2,A
        MOV A,PSW
        MOV R3,A
        MOV A,#0x9
        ADD A,#0x1
        DA A
        MOV R4,A
        MOV A,#0x81
        RL A
        MOV R5,A
        MOV A,#0x12
        SWAP A
        MOV R6,A
    )");
    CB_CHECK_EQ(m.result.exit(), "return");
    CB_CHECK_EQ(m.r(2), 0x10u);                      // 0xF0 + 0x20: the carry is set
    CB_CHECK_EQ(m.r(3), 0x89u);                      // the PSW: C, bit 3, and the stack pointer (the return address is on the stack)
    CB_CHECK_EQ(m.r(4), 0x10u);                      // 9 + 1 adjusted to BCD
    CB_CHECK_EQ(m.r(5), 0x03u);
    CB_CHECK_EQ(m.r(6), 0x21u);
});

CB_TEST("emulate.pcode_cpu.i8048_registers_banks_and_indirect_addressing", [] {
    Rig m;
    m.run(R"(
        MOV R0,#0x20
        MOV A,#0xab
        MOV @R0,A
        MOV R1,#0x20
        MOV A,@R1
        MOV R2,A
        SEL RB1
        MOV R0,#0x7
        SEL RB0
        MOV R3,#0x5c
        MOV @R1,#0x3c
        XCHD A,@R1
    )");
    CB_CHECK_EQ(m.r(2), 0xABu);
    CB_CHECK_EQ(m.result.regs.ram[24], 7);           // bank 1's R0
    CB_CHECK_EQ(m.result.regs.ram[0x20], 0x3B);      // @R1 was 0x3C, A was 0xAB: the low nibbles swapped
    CB_CHECK_EQ(m.result.regs.a, 0xACu);
});

CB_TEST("emulate.pcode_cpu.i8048_loops_calls_and_the_stack", [] {
    Rig m;
    m.run(R"(
        MOV R1,#0x5
        MOV A,#0x0
    loop:
        ADD A,#0x2
        DJNZ R1,loop
        MOV R4,A
        CALL sub
        MOV R5,A
        JMP end
    sub:
        MOV A,#0x77
        RET
    end:
    )");
    CB_CHECK_EQ(m.result.exit(), "return");
    CB_CHECK_EQ(m.r(4), 10u);
    CB_CHECK_EQ(m.r(1), 0u);
    CB_CHECK_EQ(m.r(5), 0x77u);
});

CB_TEST("emulate.pcode_cpu.i8048_ports_and_the_data_space_are_logged_accesses", [] {
    Rig m;
    m.run(R"(
        MOV A,#0xf
        OUTL P1,A
        ORL P1,#0xf0
        IN A,P2
        MOV R0,#0xa1
        MOVX A,@R0
        MOV R3,A
        MOV A,#0x33
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

CB_TEST("emulate.pcode_cpu.i8048_stops_and_limits", [] {
    {
        Rig m;
        m.run("        .byte 0x1\n");
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
