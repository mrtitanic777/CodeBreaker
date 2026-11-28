// A measure, not a check: the CPU time a core takes to run the same loop on the own core and on the emulator, for the two cores the emulator is the default of (the
// 6502 and the Z80), so a change to the emulator's stepping is seen on them. The best of five runs of 300,000 instructions each.
#include <algorithm>
#include <cstdio>

#include "core/cpu_time.hpp"
#include "core/testing.hpp"
#include "emulate/m6502.hpp"
#include "emulate/z80.hpp"
#include "decompile/engine/pcode_emulator.hh"
#include "emulate/cpu/pcode_cpu.hpp"
#include "emulate/cpu/pcode_cpu_internal.hpp"

using namespace cb;

namespace {

template <class F>
double best_of_five(F&& run) {
    // (the thread's CPU time ticks in about 15 ms: each measure runs the loop many times, and the best of five measures is kept)
    constexpr int kReps = 60;
    double best = 1e9;
    for (int i = 0; i < 5; ++i) {
        const double start = thread_cpu_seconds();
        for (int k = 0; k < kReps; ++k) run();
        best = std::min(best, (thread_cpu_seconds() - start) / kReps);
    }
    return best;
}

}  // namespace

CB_TEST("emulate.pcode_cpu.bench_6502_and_z80_loops", [] {
    constexpr std::uint64_t kSteps = 300000;
    {   // LDX #0; LDY #0; loop: INX; BNE loop; INY; BNE loop; RTS
        const Bytes program = {0xA2, 0x00, 0xA0, 0x00, 0xE8, 0xD0, 0xFD, 0xC8, 0xD0, 0xFA, 0x60};
        Bytes image(0x1000, 0);
        image.insert(image.end(), program.begin(), program.end());
        emulate::m6502::RunOptions options;
        options.max_steps = kSteps;
        const double emu = best_of_five([&] {
            emulate::m6502::Bus bus{ByteView(image)};
            emulate::pcode_cpu::run(bus, 0x1000, options);
        });
        std::printf("bench 6502: %llu instructions: emulator %.1f ms (the own core was retired)\n", static_cast<unsigned long long>(kSteps), emu * 1e3);
    }
    {   // LD B,0; LD C,0; loop: DJNZ loop; DEC C; JR NZ,loop; RET
        const Bytes program = {0x06, 0x00, 0x0E, 0x00, 0x10, 0xFE, 0x0D, 0x20, 0xFB, 0xC9};
        Bytes image = program;
        emulate::z80::RunOptions options;
        options.max_steps = kSteps;
        const double emu = best_of_five([&] {
            emulate::z80::Bus bus{ByteView(image)};
            emulate::pcode_cpu::run(bus, 0, options);
        });
        std::printf("bench z80: %llu instructions: emulator %.1f ms (the own core was retired)\n", static_cast<unsigned long long>(kSteps), emu * 1e3);
    }
});

// A measure: the emulator alone, stepped with nothing between the steps (no adapter), on the 6502 loop.
CB_TEST("emulate.pcode_cpu.bench_raw_emulator_steps", [] {
    decompile::sleigh::Emulator& emu = emulate::pcode_cpu::emulator_named("6502.sla");
    const std::uint8_t program[] = {0xA2, 0x00, 0xA0, 0x00, 0xE8, 0xD0, 0xFD, 0xC8, 0xD0, 0xFA, 0x60};
    emu.write(0x1000, program, sizeof program);
    constexpr int kSteps = 131000;
    const auto run = [&] {
        emu.setPc(0x1000);
        for (int i = 0; i < kSteps; ++i) emu.step();
    };
    const double t = best_of_five(run);
    std::printf("bench raw 6502 emulator: %d steps %.1f ms (%.1f ns/step)\n", kSteps, t * 1e3, t * 1e9 / kSteps);
    const double tb = best_of_five([&] {
        emu.setPc(0x1000);
        emu.runBlocks(kSteps);
    });
    std::printf("bench raw 6502 blocks: %d instructions %.1f ms (%.1f ns/instruction)\n", kSteps, tb * 1e3, tb * 1e9 / kSteps);
});
