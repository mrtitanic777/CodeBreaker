// Blocks on the one emulator (decompile/engine/pcode_emulator.cc, runBlocks): the same run as single steps, for the 6502 and the Z80 (the descriptions the lanes use by
// default): random programs, with devices in the address space, step limits that fall inside a block, and code that writes to the bytes it runs. The registers, the pc,
// the step count, the exit, every device access and all of memory must be the same.
#include <algorithm>
#include <cstdio>
#include <random>
#include <string>

#include "core/testing.hpp"
#include "decompile/engine/pcode_emulator.hh"
#include "emulate/m6502.hpp"
#include "emulate/z80.hpp"
#include "engine/pcode_cpu.hpp"

using namespace cb;

namespace {

struct BlocksMode {
    explicit BlocksMode(bool on) { decompile::sleigh::Emulator::setBlocksOverride(on ? 1 : 0); }
    ~BlocksMode() { decompile::sleigh::Emulator::setBlocksOverride(-1); }
};

template <class Result>
bool same_6502(const Result& a, const Result& b) {
    if (a.exit() != b.exit() || a.steps != b.steps || a.regs.a != b.regs.a || a.regs.x != b.regs.x || a.regs.y != b.regs.y || a.regs.s != b.regs.s || a.regs.p != b.regs.p ||
        a.regs.pc != b.regs.pc || a.io.size() != b.io.size())
        return false;
    for (std::size_t i = 0; i < a.io.size(); ++i)
        if (a.io[i].kind != b.io[i].kind || a.io[i].address != b.io[i].address || a.io[i].value != b.io[i].value) return false;
    return true;
}

bool same_z80(const emulate::z80::RunResult& a, const emulate::z80::RunResult& b) {
    const auto& x = a.regs;
    const auto& y = b.regs;
    if (a.exit() != b.exit() || a.steps != b.steps || x.a != y.a || x.f != y.f || x.b != y.b || x.c != y.c || x.d != y.d || x.e != y.e || x.h != y.h || x.l != y.l || x.a2 != y.a2 ||
        x.f2 != y.f2 || x.b2 != y.b2 || x.c2 != y.c2 || x.d2 != y.d2 || x.e2 != y.e2 || x.h2 != y.h2 || x.l2 != y.l2 || x.ix != y.ix || x.iy != y.iy || x.sp != y.sp || x.pc != y.pc ||
        x.i != y.i || x.r != y.r || x.iff1 != y.iff1 || x.iff2 != y.iff2 || x.im != y.im || x.halted != y.halted || a.io.size() != b.io.size())
        return false;
    for (std::size_t i = 0; i < a.io.size(); ++i)
        if (a.io[i].kind != b.io[i].kind || a.io[i].where != b.io[i].where || a.io[i].value != b.io[i].value) return false;
    return true;
}

}  // namespace

CB_TEST("engine.pcode_cpu.6502_blocks_run_as_single_steps_do", [] {
    std::mt19937 rng(20261123);
    unsigned runs = 0, different = 0, self_modifying = 0;
    for (unsigned n = 0; n < 3000; ++n) {
        Bytes image(0x10000, 0);
        for (std::size_t i = 0; i < 0x400; ++i) image[i] = static_cast<std::uint8_t>(rng());   // (zero page, stack, and the code at $40: stores reach the code)
        const std::uint16_t entry = (n % 3 == 0) ? 0x0040 : 0x0200;
        if (n % 3 != 0)
            for (std::size_t i = 0; i < 0x100; ++i) image[0x200 + i] = static_cast<std::uint8_t>(rng());
        if (n % 50 == 0) {   // LDA #$EA; STA $0207; LDA #1; LDX #5; ...: the store changes the operand of an instruction in the same straight run
            const Bytes program = {0xA9, 0xEA, 0x8D, 0x07, 0x02, 0xA9, 0x01, 0xA2, 0x05, 0xE8, 0xE8, 0xD0, 0xFB, 0x60};
            std::copy(program.begin(), program.end(), image.begin() + 0x200);
        }
        emulate::m6502::RunOptions options;
        options.a = static_cast<std::uint8_t>(rng());
        options.x = static_cast<std::uint8_t>(rng() & 0x3F);
        options.y = static_cast<std::uint8_t>(rng() & 0x3F);
        options.p = static_cast<std::uint8_t>(rng());
        options.max_steps = 1 + rng() % 400;
        emulate::m6502::RunResult r[2];
        Bytes mem[2];
        for (int mode = 0; mode < 2; ++mode) {
            const BlocksMode forced(mode == 1);
            emulate::m6502::Bus bus{ByteView(image)};
            bus.map(0xD000, 0xD0FF, [](std::uint16_t a) -> std::uint8_t { return static_cast<std::uint8_t>(a * 5 + 1); }, [](std::uint16_t, std::uint8_t) {});
            r[mode] = engine::pcode_cpu::run(bus, entry, options);
            mem[mode].assign(bus.memory().begin(), bus.memory().end());
        }
        ++runs;
        if (!same_6502(r[0], r[1]) || mem[0] != mem[1]) {
            if (++different <= 5)
                std::printf("  6502 run %u: step mode %s steps %llu pc %x, blocks %s steps %llu pc %x%s\n", n, r[0].exit().c_str(), static_cast<unsigned long long>(r[0].steps), r[0].regs.pc,
                            r[1].exit().c_str(), static_cast<unsigned long long>(r[1].steps), r[1].regs.pc, mem[0] != mem[1] ? " (memory differs)" : "");
        }
        if (mem[0] != image && std::equal(mem[0].begin() + 0x40, mem[0].begin() + 0x140, image.begin() + 0x40, image.begin() + 0x140) == false) ++self_modifying;
    }
    std::printf("6502 blocks: %u runs, %u differ (%u wrote to bytes of the code region)\n", runs, different, self_modifying);
    CB_CHECK_EQ(different, 0u);
});

// Zeros that were run as code and then written to: the page held nothing when they were decoded. The second call must run the RET stored at $3000, not the old NOP.
CB_TEST("engine.pcode_cpu.a_write_to_code_decoded_from_an_empty_page_is_seen", [] {
    Bytes image(0x10000, 0);
    const Bytes program = {0xCD, 0x00, 0x30, 0x3E, 0xC9, 0x32, 0x00, 0x30, 0xCD, 0x00, 0x30, 0xC9};   // CALL 3000h; LD A,0C9h; LD (3000h),A; CALL 3000h; RET
    std::copy(program.begin(), program.end(), image.begin() + 0x200);
    image[0x4000] = 0xC9;   // (the first call runs the zeros up to this RET)
    emulate::z80::RunOptions options;
    options.max_steps = 20000;
    options.sp = 0xF000;
    emulate::z80::Bus own_bus{ByteView(image)};
    const emulate::z80::RunResult own = emulate::z80::run(own_bus, 0x200, options);
    for (const bool blocks : {false, true}) {
        const BlocksMode forced(blocks);
        emulate::z80::Bus bus{ByteView(image)};
        const emulate::z80::RunResult emulator = engine::pcode_cpu::run(bus, 0x200, options);
        CB_CHECK(same_z80(own, emulator));
    }
});

CB_TEST("engine.pcode_cpu.z80_blocks_run_as_single_steps_do", [] {
    std::mt19937 rng(20261124);
    unsigned runs = 0, different = 0;
    for (unsigned n = 0; n < 3000; ++n) {
        Bytes image(0x10000, 0);
        for (std::size_t i = 0; i < 0x400; ++i) image[i] = static_cast<std::uint8_t>(rng());
        const std::uint16_t entry = (n % 3 == 0) ? 0x0040 : 0x0200;
        if (n % 3 != 0)
            for (std::size_t i = 0; i < 0x100; ++i) image[0x200 + i] = static_cast<std::uint8_t>(rng());
        if (n % 50 == 0) {   // LD A,0; LD (0208h),A; LD A,1; LD B,9 (its operand is the byte stored); DJNZ $; RET
            const Bytes program = {0x3E, 0x00, 0x32, 0x08, 0x02, 0x3E, 0x01, 0x06, 0x09, 0x10, 0xFE, 0xC9};
            std::copy(program.begin(), program.end(), image.begin() + 0x200);
        }
        emulate::z80::RunOptions options;
        options.max_steps = 1 + rng() % 400;
        options.sp = 0xF000 + (rng() & 0xFF);
        options.regs.a = static_cast<std::uint8_t>(rng());
        options.regs.h = static_cast<std::uint8_t>(rng() & 3);
        options.regs.l = static_cast<std::uint8_t>(rng());
        options.regs.b = static_cast<std::uint8_t>(rng() & 7);
        emulate::z80::RunResult r[2];
        Bytes mem[2];
        for (int mode = 0; mode < 2; ++mode) {
            const BlocksMode forced(mode == 1);
            emulate::z80::Bus bus{ByteView(image)};
            bus.map(0xD000, 0xD0FF, [](std::uint16_t a) -> std::uint8_t { return static_cast<std::uint8_t>(a * 5 + 1); }, [](std::uint16_t, std::uint8_t) {});
            r[mode] = engine::pcode_cpu::run(bus, entry, options);
            mem[mode].assign(bus.memory().begin(), bus.memory().end());
        }
        ++runs;
        if (!same_z80(r[0], r[1]) || mem[0] != mem[1]) {
            if (++different <= 5)
                std::printf("  z80 run %u: step mode %s steps %llu pc %x, blocks %s steps %llu pc %x%s\n", n, r[0].exit().c_str(), static_cast<unsigned long long>(r[0].steps), r[0].regs.pc,
                            r[1].exit().c_str(), static_cast<unsigned long long>(r[1].steps), r[1].regs.pc, mem[0] != mem[1] ? " (memory differs)" : "");
        }
    }
    std::printf("z80 blocks: %u runs, %u differ\n", runs, different);
    CB_CHECK_EQ(different, 0u);
});
