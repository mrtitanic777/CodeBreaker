// The lanes' 6502 on the one emulator (engine/pcode_cpu): its random programs as its retired core ran them (a digest). The Z80's are in
// emulate/cpu/pcode_cpu_z80_test.
#include "emulate/cpu/pcode_cpu.hpp"

#include <cstdio>
#include <format>
#include <random>
#include <string>
#include <vector>

#include "core/crypto.hpp"
#include "core/testing.hpp"
#include "decompile/m6502.hpp"

using namespace cb;
namespace m = cb::emulate::m6502;

namespace {

std::string describe(const m::RunResult& r) {
    return std::format("{} A={:02X} X={:02X} Y={:02X} P={:02X} S={:02X} pc={:04X} steps={} io={}", r.exit(), r.regs.a, r.regs.x, r.regs.y, r.regs.p, r.regs.s,
                       r.regs.pc, r.steps, r.io.size());
}

}  // namespace

// A random program of documented opcodes at $8000, with random bytes after it, a device range at $4000-$40FF and RAM elsewhere. What 1,500 such runs gave
// on the old core (emulate/m6502's interpreter) and on the emulator, the same when the core was retired, is kept as a digest (CRC-32 of every run's exit,
// registers, steps, device accesses and memory).
CB_TEST("emulate.pcode_cpu.runs_random_programs_as_the_own_core_did", [] {
    std::vector<std::uint8_t> documented;
    const auto& table = decompile::m6502::opcode_table(decompile::m6502::Variant::Nmos);
    for (unsigned op = 0; op < 256; ++op)
        if (table[op]) documented.push_back(static_cast<std::uint8_t>(op));
    std::mt19937 rng(20261120);
    std::uint32_t digest = 0;
    for (int n = 0; n < 1500; ++n) {
        Bytes image(0x10000, 0);
        for (std::size_t i = 0; i < 0x10000; ++i) image[i] = static_cast<std::uint8_t>(rng());
        for (std::size_t i = 0x8000; i < 0x8040; ++i) image[i] = documented[rng() % documented.size()];
        m::RunOptions options;
        options.a = static_cast<std::uint8_t>(rng());
        options.x = static_cast<std::uint8_t>(rng());
        options.y = static_cast<std::uint8_t>(rng());
        options.p = static_cast<std::uint8_t>(m::status::U | (rng() & 0xCF));
        options.max_steps = 200;
        m::Bus bus(image);
        bus.map(0x4000, 0x40FF, [](std::uint16_t a) -> std::uint8_t { return static_cast<std::uint8_t>(a * 7 + 3); }, [](std::uint16_t, std::uint8_t) {});
        const m::RunResult r = emulate::pcode_cpu::run(bus, 0x8000, options);
        std::string text = describe(r);
        for (const m::DeviceAccess& x : r.io) text += std::format("|{}{:X}={:X}", static_cast<int>(x.kind), x.address, x.value);
        digest = crypto::crc32(ByteView(reinterpret_cast<const std::uint8_t*>(text.data()), text.size()), digest);
        digest = crypto::crc32(ByteView(bus.memory().data(), bus.memory().size()), digest);
    }
    std::printf("pcode_cpu random programs: digest %08X\n", digest);
    CB_CHECK_EQ(digest, 0x15018420u);
});
