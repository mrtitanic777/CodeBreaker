// cb-unity: alone
// The Neo Geo Pocket lane's TLCS-900/H on the one emulator (engine/pcode_cpu_tlcs900.cpp): random instructions and programs as the retired own core (emulate/tlcs900's
// interpreter) ran them, kept as digests (the own core's unit tests are engine/pcode_cpu_tlcs900_unit_test).
#include <cstdio>
#include <format>
#include <random>
#include <string>

#include "core/crypto.hpp"
#include "core/testing.hpp"
#include "emulate/tlcs900.hpp"
#include "engine/pcode_cpu_tlcs900.hpp"

namespace t = cb::emulate::tlcs900;

namespace {

constexpr std::uint32_t kRam = 0x004000, kRamSize = 0x3000, kDevice = 0x008000, kRom = 0x200000, kRomSize = 0x10000, kBios = 0xFF0000;

// A run as text: its exit, every register, the steps, the device accesses and the BIOS calls (what a digest of runs folds in, with the RAM).
std::string fingerprint(const t::RunResult& r) {
    std::string out = r.exit();
    for (const auto& bank : r.regs.bank)
        for (const std::uint32_t v : bank) out += std::format(" {:X}", v);
    out += std::format(" {:X} {:X} {:X} {:X} {:X} {:X} {:X} {:X}", r.regs.xix, r.regs.xiy, r.regs.xiz, r.regs.xsp, r.regs.pc, r.regs.sr, r.regs.f_alt, r.regs.intnest);
    for (unsigned k = 0; k < 4; ++k) out += std::format(" {:X} {:X} {:X} {:X}", r.regs.dmas[k], r.regs.dmad[k], r.regs.dmac[k], r.regs.dmam[k]);
    out += std::format(" steps {}", r.steps);
    for (const t::DeviceAccess& x : r.io) out += std::format("|{}{:X}/{}={:X}", static_cast<int>(x.kind), x.address, x.size, x.value);
    for (const t::SoftwareInterrupt& x : r.swis) out += std::format("|swi {} {:X} {:X} {:X} {:X} {:X} {:X}", x.number, x.pc, x.function, x.regs[0], x.regs[1], x.regs[2], x.regs[3]);
    return out;
}

std::uint32_t fold(std::uint32_t digest, const t::RunResult& r, const cb::Bytes& ram) {
    const std::string text = fingerprint(r);
    digest = cb::crypto::crc32(cb::ByteView(reinterpret_cast<const std::uint8_t*>(text.data()), text.size()), digest);
    return cb::crypto::crc32(ram, digest);
}

struct Case {
    cb::Bytes rom, ram, bios;
    t::RunOptions options;
};

// One run: a bus of its own on the case's memory.
t::RunResult play(const Case& c, std::uint64_t max_steps, cb::Bytes& ram_after) {
    t::Bus bus;
    bus.device(kDevice, kDevice + 0x1FF, [](std::uint32_t a, unsigned size) -> std::uint32_t { return (a * 7 + 3) * (size + 1); }, [](std::uint32_t, unsigned, std::uint32_t) {});
    bus.memory(kRam, kRam + kRamSize - 1, c.ram, true, false);
    bus.memory(kRom, kRom + kRomSize - 1, c.rom, false, false);
    bus.memory(kBios, 0xFFFFFF, c.bios, false, false);
    t::RunOptions o = c.options;
    o.max_steps = max_steps;
    const t::RunResult run = cb::engine::pcode_cpu::run(bus, kRom, o);
    const std::span<std::uint8_t> ram = bus.bytes(kRam);
    ram_after.assign(ram.begin(), ram.end());
    return run;
}

// 6,000 single instructions or programs of up to 40 steps over random ROM, RAM, registers, SR and banks, a device range that answers and logs, and the BIOS: each run's
// exit, registers, steps, device accesses and BIOS calls, and the RAM, folded into a CRC-32.
std::uint32_t digest_of(bool single) {
    std::mt19937 rng(single ? 20261124 : 20261125);
    std::uint32_t digest = 0;
    for (unsigned n = 0; n < 6000; ++n) {
        Case c;
        c.rom.resize(kRomSize);
        for (std::uint8_t& b : c.rom) b = static_cast<std::uint8_t>(rng());
        if (rng() % 3 == 0)   // (code that is mostly instructions that name a register or a short address)
            for (std::size_t i = 0; i < 64; ++i) c.rom[i] = static_cast<std::uint8_t>((rng() & 1) ? 0xC8 + rng() % 0x28 : rng());
        c.ram.resize(kRamSize);
        for (std::uint8_t& b : c.ram) b = static_cast<std::uint8_t>(rng());
        c.bios.assign(0x10000, 0x0E);
        const auto value = [&]() -> std::uint32_t {
            switch (rng() % 6) {
                case 0: return kRam + rng() % kRamSize;
                case 1: return kRom + rng() % kRomSize;
                case 2: return kDevice + rng() % 0x200;
                case 3: return rng() & 0xFF;
                default: return static_cast<std::uint32_t>(rng());
            }
        };
        t::RunOptions& o = c.options;
        o.xwa = value();
        o.xbc = value();
        o.xde = value();
        o.xhl = value();
        o.xix = value();
        o.xiy = value();
        o.xiz = value();
        o.sr = static_cast<std::uint16_t>((rng() & 0xF3FF) | ((rng() & 3) << 8));
        o.sp = kRam + 0x2000 + (rng() & 0xFF);
        for (unsigned k = 0; k < rng() % 3; ++k) o.args.push_back(static_cast<std::uint16_t>(rng()));
        cb::Bytes ram;
        const t::RunResult run = play(c, single ? 1 : 40, ram);
        digest = fold(digest, run, ram);
    }
    return digest;
}

}  // namespace

// What 6,000 runs of each kind gave on the retired core, and on the emulator the same when the core was retired (2026-11-27).
CB_TEST("engine.pcode_cpu.tlcs900_instructions_run_as_the_own_core_did", [] {
    const std::uint32_t digest = digest_of(true);
    std::printf("pcode_cpu tlcs900 instructions: digest %08X\n", digest);
    CB_CHECK_EQ(digest, 0x13F1EF2Cu);
});

CB_TEST("engine.pcode_cpu.tlcs900_programs_run_as_the_own_core_did", [] {
    const std::uint32_t digest = digest_of(false);
    std::printf("pcode_cpu tlcs900 programs: digest %08X\n", digest);
    CB_CHECK_EQ(digest, 0x643AB369u);
});
