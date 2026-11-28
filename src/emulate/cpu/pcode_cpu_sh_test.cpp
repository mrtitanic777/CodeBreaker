// The SuperH lanes' SH-2 and SH-4 on the one emulator (engine/pcode_cpu_sh.cpp): the same run as the own core on random programs.
#include <algorithm>
#include <cstdio>
#include <format>
#include <map>
#include <random>
#include <string>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "decompile/sh2.hpp"
#include "emulate/sh2.hpp"
#include "emulate/cpu/pcode_cpu.hpp"
#include "emulate/cpu/pcode_cpu_compare_test.hpp"
#include "formats/dreamcast.hpp"
#include "source/dreamcast_test_programs.hpp"
#include "formats/sega32x.hpp"
#include "formats/saturn.hpp"

using namespace cb;
namespace sh2 = cb::emulate::sh2;

namespace {

std::string sh_describe(const sh2::RunResult& a, const sh2::RunResult& b, bool sh4) {
    std::string out;
    if (a.exit() != b.exit()) out += std::format("exit('{}' / '{}') ", a.exit(), b.exit());
    else if (a.steps != b.steps) out += std::format("(both '{}') ", a.exit());
    for (unsigned i = 0; i < 16; ++i)
        if (a.r[i] != b.r[i]) out += std::format("r{}[own {:X} emu {:X}] ", i, a.r[i], b.r[i]);
    const std::uint32_t mask = sh4 ? sh2::sr::kMaskSh4 : sh2::sr::kMask;
    if (a.pr != b.pr) out += std::format("pr[{:X}/{:X}] ", a.pr, b.pr);
    if (a.gbr != b.gbr) out += std::format("gbr[{:X}/{:X}] ", a.gbr, b.gbr);
    if (a.vbr != b.vbr) out += std::format("vbr[{:X}/{:X}] ", a.vbr, b.vbr);
    if (a.mach != b.mach) out += std::format("mach[{:X}/{:X}] ", a.mach, b.mach);
    if (a.macl != b.macl) out += std::format("macl[{:X}/{:X}] ", a.macl, b.macl);
    if ((a.sr & mask) != (b.sr & mask)) out += std::format("sr[{:X}/{:X}] ", a.sr & mask, b.sr & mask);
    if (a.steps != b.steps) out += std::format("steps[{}/{}] ", a.steps, b.steps);
    if (a.fpscr != b.fpscr || a.fpul != b.fpul || a.fr != b.fr) out += "fpu ";
    if (a.io.size() != b.io.size()) out += std::format("io[{}/{}] ", a.io.size(), b.io.size());
    else
        for (std::size_t i = 0; i < a.io.size(); ++i)
            if (a.io[i].kind != b.io[i].kind || a.io[i].address != b.io[i].address || a.io[i].value != b.io[i].value || a.io[i].size != b.io[i].size) {
                out += std::format("io@{} ", i);
                break;
            }
    return out;
}

void compare_random_programs(sh2::Variant variant, const char* title) {
    const bool sh4 = variant == sh2::Variant::Sh4;
    std::mt19937 rng(20261121);
    constexpr std::uint32_t kBase = 0x0C000000;
    std::map<std::string, unsigned> by_kind;
    std::map<std::string, std::string> example;
    unsigned runs = 0, different = 0;
    const unsigned total = [] {
        const auto given = fs::get_env("CB_PCODE_EXHAUSTIVE");
        return given && !given->empty() && *given != "0" ? static_cast<unsigned>(std::strtoul(given->c_str(), nullptr, 10)) : 6000u;
    }();
    for (unsigned n = 0; n < 2 * total; ++n) {
        const bool single = n < total;
        Bytes image(0x100000, 0);
        for (std::size_t i = 0x20000; i < 0x30000; ++i) image[i] = static_cast<std::uint8_t>(rng());
        for (std::size_t i = 0x1000; i < 0x1040; ++i) image[i] = static_cast<std::uint8_t>(rng());
        std::uint16_t first = 0;
        if (single) {
            first = static_cast<std::uint16_t>((n * 11) % 0x10000);
            if (sh4) {
                image[0x1000] = static_cast<std::uint8_t>(first);
                image[0x1001] = static_cast<std::uint8_t>(first >> 8);
            } else {
                image[0x1000] = static_cast<std::uint8_t>(first >> 8);
                image[0x1001] = static_cast<std::uint8_t>(first);
            }
        } else {
            first = sh4 ? static_cast<std::uint16_t>(image[0x1000] | image[0x1001] << 8) : static_cast<std::uint16_t>(image[0x1000] << 8 | image[0x1001]);
        }
        sh2::RunOptions options;
        options.max_steps = single ? 1 : 40;
        options.sp = kBase + 0x80000;
        for (auto& v : options.a) v = (rng() & 1) ? kBase + 0x20000 + (rng() & 0xFFFC) : static_cast<std::uint32_t>(rng() & 0xFF);
        if (rng() & 1) options.a[0] |= rng() & 3;
        for (auto& v : options.s) v = (rng() & 3) == 0 ? static_cast<std::uint32_t>(rng()) : kBase + 0x20000 + (rng() & 0xFFFC);
        options.r0 = (rng() & 1) ? static_cast<std::uint32_t>(rng() & 0xFFFF) : kBase + 0x20000 + (rng() & 0xFFFC);
        options.gbr = kBase + 0x20000 + (rng() & 0xFF00);
        std::uint32_t result_mem[2][1] = {};
        (void)result_mem;
        Bytes mem[2];
        sh2::RunResult rr[2];
        for (int core = 0; core < 2; ++core) {
            sh2::Bus bus(variant);
            bus.add_ram(kBase, 0x100000);
            bus.load(kBase, image);
            bus.map(kBase + 0xF0000, kBase + 0xF00FF, [](std::uint32_t a, unsigned) -> std::uint32_t { return a * 7 + 3; }, [](std::uint32_t, unsigned, std::uint32_t) {});
            rr[core] = core == 1 ? emulate::pcode_cpu::run(bus, kBase + 0x1000, options) : sh2::run(bus, kBase + 0x1000, options);
            const ByteView m = static_cast<const sh2::Bus&>(bus).region(kBase);
            mem[core].assign(m.begin(), m.end());
        }
        ++runs;
        const std::uint32_t mask = sh4 ? sh2::sr::kMaskSh4 : sh2::sr::kMask;
        const bool same = sh_describe(rr[0], rr[1], sh4).empty() && mem[0] == mem[1];
        (void)mask;
        if (same) continue;
        ++different;
        std::string fields = sh_describe(rr[0], rr[1], sh4) + std::format("(own '{}' emu '{}') ", rr[0].exit(), rr[1].exit());
        for (std::size_t i = 0; i < mem[0].size() && mem[0] != mem[1]; ++i)
            if (mem[0][i] != mem[1][i]) {
                fields += std::format("mem@{:X}[own {:02X} emu {:02X}] ", i, mem[0][i], mem[1][i]);
                break;
            }
        const auto insn = decompile::sh2::decode_word(first, 0, variant);
        std::string key = std::string(single ? "1: " : "N: ") + (insn ? insn->mnemonic : std::string("?"));
        if (!single && rr[0].exit() == rr[1].exit() && rr[0].steps != 0) {   // (a program: the instruction after which they differ is what is at fault)
            unsigned k0 = 0;
            for (unsigned k = 1; k <= options.max_steps && k0 == 0; ++k) {
                sh2::RunOptions o2 = options;
                o2.max_steps = k;
                sh2::RunResult ra, rb;
                Bytes ma, mb;
                for (int core = 0; core < 2; ++core) {
                    sh2::Bus bus(variant);
                    bus.add_ram(kBase, 0x100000);
                    bus.load(kBase, image);
                    bus.map(kBase + 0xF0000, kBase + 0xF00FF, [](std::uint32_t a, unsigned) -> std::uint32_t { return a * 7 + 3; }, [](std::uint32_t, unsigned, std::uint32_t) {});
                    (core == 1 ? rb : ra) = core == 1 ? emulate::pcode_cpu::run(bus, kBase + 0x1000, o2) : sh2::run(bus, kBase + 0x1000, o2);
                    const ByteView m = static_cast<const sh2::Bus&>(bus).region(kBase);
                    (core == 1 ? mb : ma).assign(m.begin(), m.end());
                }
                if (!sh_describe(ra, rb, sh4).empty() || ma != mb) k0 = k;
            }
            fields += std::format("[first differing step {}] ", k0);
        }
        ++by_kind[key];
        if (!example.count(key))
            example[key] = std::format("{:04X} {:02X}{:02X} {:02X}{:02X} {:02X}{:02X} {:02X}{:02X}: {}", first, image[0x1002], image[0x1003], image[0x1004], image[0x1005], image[0x1006],
                                       image[0x1007], image[0x1008], image[0x1009], fields);
    }
    std::printf("pcode_cpu %s instructions: %u runs, %u differ\n", title, runs, different);
    unsigned shown = 0;
    for (const auto& [k, c] : by_kind)
        if (++shown <= 40) std::printf("    %u x %s   e.g. %s\n", c, k.c_str(), example[k].c_str());
    CB_CHECK_EQ(different, 0u);
}

}  // namespace

CB_TEST("emulate.pcode_cpu.sh2_instructions_run_as_the_own_core_does", [] { compare_random_programs(sh2::Variant::Sh2, "sh2"); });

CB_TEST("emulate.pcode_cpu.sh4_instructions_run_as_the_own_core_does", [] { compare_random_programs(sh2::Variant::Sh4, "sh4"); });

// The lanes' homebrew on both cores.
CB_TEST("emulate.pcode_cpu.the_32x_lane_runs_homebrew_the_same_on_both_cores", [] {
    emulate::pcode_compare::compare_homebrew("sega32x", "sega32x", ".32x", [](const std::filesystem::path& path, std::uint32_t address) -> std::size_t {
        if (address < formats::sega32x::kSdram) return SIZE_MAX;   // (a 68000 function: its address is the ROM's)
        const formats::sega32x::Rom rom = formats::sega32x::read(path);
        return rom.header.sh2_offset + ((address & sh2::kPhysicalMask) - formats::sega32x::kSdram);
    },
    [](std::uint32_t address) { return address >= formats::sega32x::kSdram; });
});

// The 32X's other processor: the 68000's functions, whose addresses are the ROM's.
CB_TEST("emulate.pcode_cpu.the_32x_lane_runs_its_68000_functions_the_same_on_both_cores", [] {
    emulate::pcode_compare::compare_homebrew("sega32x", "sega32x", ".32x", [](const std::filesystem::path&, std::uint32_t address) -> std::size_t { return address; },
                                            [](std::uint32_t address) { return address < formats::sega32x::kSdram; });
});

CB_TEST("emulate.pcode_cpu.the_saturn_lane_runs_homebrew_the_same_on_both_cores", [] {
    emulate::pcode_compare::compare_homebrew("saturn", "saturn", ".iso", [](const std::filesystem::path& path, std::uint32_t address) -> std::size_t {
        // (the bytes at the address, found in the image)
        const formats::saturn::Disc disc = formats::saturn::read(path);
        const Bytes memory = disc.memory();
        const std::size_t index = (address & sh2::kPhysicalMask) - formats::saturn::kHighRam;
        if (index + 16 > memory.size()) return SIZE_MAX;
        const Bytes file = fs::read_file(path);
        const auto found = std::search(file.begin(), file.end(), memory.begin() + static_cast<std::ptrdiff_t>(index), memory.begin() + static_cast<std::ptrdiff_t>(index) + 16);
        return found == file.end() ? SIZE_MAX : static_cast<std::size_t>(found - file.begin());
    });
});

CB_TEST("emulate.pcode_cpu.the_dreamcast_lane_runs_homebrew_the_same_on_both_cores", [] {
    // The release is 1ST_READ.BIN alone: it goes in a disc of its own (as source/dreamcast_homebrew_test does), in a samples folder of its own.
    std::filesystem::path root = testing::beside_checkout("homebrew-samples");
    if (!std::filesystem::exists(root)) root = testing::beside_checkout("test-inputs/homebrew-samples");
    const std::filesystem::path program_file = root / "dreamcast" / "openmenu-1.3.0" / "1ST_READ.BIN";
    if (!std::filesystem::is_regular_file(program_file)) return;
    fs::ScopedTempDir dir("cb_pcode_dc_samples");
    const Bytes on_disc = fs::read_file(program_file);
    const Bytes program = formats::dreamcast::looks_scrambled(on_disc) ? formats::dreamcast::descramble(on_disc) : on_disc;
    fs::make_dirs(dir.path() / "dreamcast");
    fs::write_file(dir.path() / "dreamcast" / "game.iso", source::dreamcast_test::iso(program, false));
    const fs::ScopedEnv samples("CB_HOMEBREW_SAMPLES", fs::to_text(dir.path()));
    emulate::pcode_compare::compare_homebrew("dreamcast", "dreamcast", ".iso", [](const std::filesystem::path&, std::uint32_t address) -> std::size_t {
        return 18 * 2048 + ((address & sh2::kPhysicalMask) - formats::dreamcast::kFirstReadBase);   // (the program's sectors begin at sector 18 of the test disc)
    });
});
