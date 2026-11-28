// cb-unity: alone (file-local names that other files in a batch also use)
// The Virtual Boy lane's NEC V810 on the one emulator (engine/pcode_cpu_v810.cpp): the same run as the own core on random instructions and programs, and on the lane's homebrew.
#include <cstdio>
#include <format>
#include <map>
#include <random>
#include <string>

#include "core/crypto.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "decompile/v810.hpp"
#include "emulate/v810.hpp"
#include "engine/pcode_cpu.hpp"
#include "engine/pcode_cpu_compare.hpp"
#include "engine/pcode_cpu_internal.hpp"
#include "engine/pcode_cpu_v810.hpp"
#include "formats/vb.hpp"

using namespace cb;
namespace v8 = cb::emulate::v810;

namespace {

constexpr std::uint32_t kWork = 0x05000000;
constexpr std::uint32_t kCode = kWork + 0x1000;

// What differs between two runs, as text (empty: nothing).
std::string describe(const v8::RunResult& a, const v8::RunResult& b) {
    std::string out;
    if (a.exit() != b.exit()) out += std::format("exit('{}' / '{}') ", a.exit(), b.exit());
    for (unsigned i = 0; i < 32; ++i)
        if (a.regs.r[i] != b.regs.r[i]) out += std::format("r{}[own {:X} emu {:X}] ", i, a.regs.r[i], b.regs.r[i]);
    if (a.regs.pc != b.regs.pc) out += std::format("pc[{:X}/{:X}] ", a.regs.pc, b.regs.pc);
    if (a.regs.psw != b.regs.psw) out += std::format("psw[{:X}/{:X}] ", a.regs.psw, b.regs.psw);
    if (a.regs.eipc != b.regs.eipc || a.regs.eipsw != b.regs.eipsw || a.regs.fepc != b.regs.fepc || a.regs.fepsw != b.regs.fepsw || a.regs.ecr != b.regs.ecr ||
        a.regs.chcw != b.regs.chcw || a.regs.adtre != b.regs.adtre)
        out += "system ";
    if (a.steps != b.steps) out += std::format("steps[{}/{}] ", a.steps, b.steps);
    if (a.io.size() != b.io.size()) out += std::format("io[{}/{}] ", a.io.size(), b.io.size());
    else
        for (std::size_t i = 0; i < a.io.size(); ++i)
            if (a.io[i].kind != b.io[i].kind || a.io[i].address != b.io[i].address || a.io[i].value != b.io[i].value || a.io[i].size != b.io[i].size) {
                out += std::format("io@{} ", i);
                break;
            }
    return out;
}

// A run as text: its exit, every register, the steps and the device accesses (what a digest of runs folds in, with the work RAM; the logged RAM's writes are in the log).
std::string fingerprint(const v8::RunResult& r) {
    std::string out = r.exit();
    for (const std::uint32_t v : r.regs.r) out += std::format(" {:X}", v);
    out += std::format(" {:X} {:X} {:X} {:X} {:X} {:X} {:X} {:X} {:X}", r.regs.pc, r.regs.psw, r.regs.eipc, r.regs.eipsw, r.regs.fepc, r.regs.fepsw, r.regs.ecr, r.regs.chcw, r.regs.adtre);
    out += std::format(" steps {}", r.steps);
    for (const v8::DeviceAccess& x : r.io) out += std::format("|{}{:X}/{}={:X}", static_cast<int>(x.kind), x.address, x.size, x.value);
    return out;
}

std::uint32_t fold(std::uint32_t digest, const v8::RunResult& r, const Bytes& work) {
    const std::string text = fingerprint(r);
    digest = crypto::crc32(ByteView(reinterpret_cast<const std::uint8_t*>(text.data()), text.size()), digest);
    return crypto::crc32(work, digest);
}

constexpr unsigned kDigestRuns = 8000;   // (the runs a digest folds in)

// A machine as the lane makes one: work RAM mirrored over 16 MB (not logged), a logged RAM, a device range that answers and logs, and the bytes of the image at kWork.
struct VbMachine {
    explicit VbMachine(const Bytes& image) : bus(0x07FFFFFF) {
        bus.memory(0x00000000, 0x0003FFFF, {}, true, true);
        bus.device(0x02000000, 0x02FFFFFF, [](std::uint32_t a, unsigned) -> std::uint32_t { return a * 7 + 3; }, [](std::uint32_t, unsigned, std::uint32_t) {});
        bus.memory(kWork, 0x05FFFFFF, image, true, false, image.size());
    }
    v8::Bus bus;
};

// A value of a register for the focused runs: a floating-point value of every class (zeros, infinities, NaNs, denormals, the limits of the conversions, ordinary ones),
// a small integer, a pointer into the RAM, or random bits.
std::uint32_t focused_value(std::mt19937& rng) {
    static constexpr std::uint32_t kSpecial[] = {0x00000000, 0x80000000, 0x7F800000, 0xFF800000, 0x7FC00000, 0xFFC00001, 0x00000001, 0x807FFFFF, 0x00800000, 0x80800000,
                                                 0x3F800000, 0xBF800000, 0x3F000000, 0xBF000000, 0x40200000, 0x40600000, 0xC0200000, 0xC0600000, 0x4F000000, 0xCF000000,
                                                 0x4EFFFFFF, 0xCF000001, 0x7F7FFFFF, 0xFF7FFFFF, 0x00FFFFFF, 0x0D800000, 0x8D800000, 0x80000001, 0x7FFFFFFF, 0xFFFFFFFF};
    switch (rng() % 6) {
        case 0: return kSpecial[rng() % std::size(kSpecial)];
        case 1: return (rng() & 0x80000000u) | ((100 + rng() % 60) << 23) | (rng() & 0x7FFFFF);   // (an ordinary float near one)
        case 2: return (rng() & 0x80000000u) | ((rng() % 255) << 23) | (rng() & 0x7FFFFF);         // (any finite float)
        case 3: return kWork + 0x2000 + (rng() & 0x1FFC);
        case 4: return static_cast<std::uint32_t>(static_cast<int>(rng() % 80) - 40);
        default: return static_cast<std::uint32_t>(rng());
    }
}

// The first instruction of a focused run: the floating point and the extended instructions, the bit strings, ldsr and stsr, caxi, the divisions and what stops a run.
std::pair<std::uint32_t, std::uint32_t> focused_instruction(std::mt19937& rng) {
    const std::uint32_t r1 = rng() & 31, r2 = rng() & 31;
    switch (rng() % 10) {
        case 0: case 1: case 2: case 3: {   // (Format VII: cmpf cvt.ws cvt.sw addf subf mulf divf xb xh rev trnc mpyhw)
            static constexpr std::uint32_t kSub[] = {0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
            const std::uint32_t sub = kSub[rng() % std::size(kSub)];
            return {0xF800 | r2 << 5 | ((sub == 8 || sub == 9) ? 0 : r1), sub << 10};
        }
        case 4: case 5: {   // (the bit strings)
            static constexpr std::uint32_t kSub[] = {0, 1, 2, 3, 8, 9, 10, 11, 12, 13, 14, 15};
            return {0x7C00 | kSub[rng() % std::size(kSub)], static_cast<std::uint32_t>(rng() & 0xFFFF)};
        }
        case 6: return {(rng() & 1 ? 0x1Cu : 0x1Du) << 10 | r2 << 5 | r1, static_cast<std::uint32_t>(rng() & 0xFFFF)};   // (ldsr, stsr)
        case 7: return {0x3Au << 10 | r2 << 5 | r1, static_cast<std::uint32_t>(rng() & 0x1C)};                            // (caxi)
        case 8: return {(rng() & 1 ? 0x09u : 0x0Bu) << 10 | r2 << 5 | r1, static_cast<std::uint32_t>(rng() & 0xFFFF)};   // (div, divu)
        default: return {(0x18u + rng() % 3) << 10 | (rng() % 3 == 0 ? r1 : 0), static_cast<std::uint32_t>(rng() & 0xFFFF)};   // (trap, reti, halt)
    }
}

// The instruction at kCode is `first` then `second` (when it has two halfwords), the rest random; registers point into the RAM or are random. Focused: the instruction is one
// of focused_instruction()'s and the registers hold focused_value()s.
void compare_random_programs(const char* title, bool single, bool focused = false) {
    std::mt19937 rng(20261122);
    std::map<std::string, unsigned> by_kind;
    std::map<std::string, std::string> example;
    const std::uint64_t emulator_before = engine::pcode_cpu::emulator_named("v810exact.sla").steps();
    unsigned runs = 0, different = 0;
    std::uint32_t digest[2] = {0, 0};   // (of the first kDigestRuns runs, on each core)
    const unsigned total = [&] {
        const auto given = fs::get_env("CB_PCODE_EXHAUSTIVE");
        if (given && !given->empty() && *given != "0") return static_cast<unsigned>(std::strtoul(given->c_str(), nullptr, 10));
        return single ? 70000u : 20000u;
    }();
    for (unsigned n = 0; n < total; ++n) {
        Bytes image(0x10000, 0);
        for (std::size_t i = 0x2000; i < 0x4000; ++i) image[i] = static_cast<std::uint8_t>(rng());   // data
        for (std::size_t i = 0x1000; i < 0x1060; ++i) image[i] = static_cast<std::uint8_t>(rng());   // code
        std::uint32_t first = 0;
        if (focused) {
            const auto [a, b] = focused_instruction(rng);
            first = a;
            image[0x1000] = static_cast<std::uint8_t>(a);
            image[0x1001] = static_cast<std::uint8_t>(a >> 8);
            image[0x1002] = static_cast<std::uint8_t>(b);
            image[0x1003] = static_cast<std::uint8_t>(b >> 8);
        } else if (single) {
            first = n % 0x10000;   // every first halfword in turn
            image[0x1000] = static_cast<std::uint8_t>(first);
            image[0x1001] = static_cast<std::uint8_t>(first >> 8);
        } else {
            first = image[0x1000] | image[0x1001] << 8;
        }
        v8::RunOptions options;
        options.max_steps = single ? 1 : 40;
        const auto pointer = [&] { return kWork + 0x2000 + (rng() & 0x1FFC); };
        for (unsigned i = 1; i < 32; ++i) {
            const unsigned kind = rng() % 6;
            options.r[i] = kind < 3 ? pointer() : kind == 3 ? static_cast<std::uint32_t>(rng()) : kind == 4 ? static_cast<std::uint32_t>(rng() % 40) : static_cast<std::uint32_t>(-static_cast<int>(rng() % 40));
        }
        if ((rng() & 7) == 0) options.r[6] = 0;
        if ((rng() & 7) == 0) options.r[7] = 0xFFFFFFFFu;
        if ((rng() & 15) == 0) options.r[28] = static_cast<std::uint32_t>(rng() % 200);   // (a bit string's length)
        if (focused) {
            for (unsigned i = 1; i < 32; ++i) options.r[i] = focused_value(rng);
            options.r[29] = kWork + 0x2000 + (rng() & 0x1FFF);   // (the bit strings' words and offsets)
            options.r[30] = kWork + 0x2000 + (rng() & 0x1FFF);
            options.r[26] = rng() % 3 ? rng() & 31 : static_cast<std::uint32_t>(rng());
            options.r[27] = rng() % 3 ? rng() & 31 : static_cast<std::uint32_t>(rng());
            options.r[28] = rng() % 50 == 0 ? 0x100000u + rng() % 3 : rng() % 300;
            options.max_steps = rng() % 4 ? 1 : 40;
        }
        options.sp = kWork + 0xFFF0;
        v8::RunResult rr[2];
        Bytes mem[2], logged[2];
        for (int core = 0; core < 2; ++core) {
            VbMachine m(image);
            rr[core] = core == 1 ? engine::pcode_cpu::run(m.bus, kCode, options) : v8::run(m.bus, kCode, options);
            const std::span<std::uint8_t> work = m.bus.bytes(kWork), low = m.bus.bytes(0);
            mem[core].assign(work.begin(), work.end());
            logged[core].assign(low.begin(), low.end());
        }
        ++runs;
        if (n < kDigestRuns)
            for (int core = 0; core < 2; ++core) digest[core] = fold(digest[core], rr[core], mem[core]);
        std::string fields = describe(rr[0], rr[1]);
        if (fields.empty() && mem[0] == mem[1] && logged[0] == logged[1]) continue;
        ++different;
        fields += std::format("(own '{}' emu '{}') ", rr[0].exit(), rr[1].exit());
        for (std::size_t i = 0; i < mem[0].size() && mem[0] != mem[1]; ++i)
            if (mem[0][i] != mem[1][i]) {
                fields += std::format("mem@{:X}[own {:02X} emu {:02X}] ", i, mem[0][i], mem[1][i]);
                break;
            }
        if (logged[0] != logged[1]) fields += "logged memory ";
        const std::array<std::uint8_t, 4> code{image[0x1000], image[0x1001], image[0x1002], image[0x1003]};
        const auto insn = decompile::v810::decode(ByteView(code.data(), 4), 0, kCode);
        const std::string key = std::string(single ? "1: " : "N: ") + (insn ? std::string(insn->mnemonic) : std::string("?"));
        ++by_kind[key];
        if (!example.count(key)) example[key] = std::format("{:04X} {:02X}{:02X} {:02X}{:02X}: {}", first, image[0x1002], image[0x1003], image[0x1004], image[0x1005], fields);
    }
    const std::uint64_t on_emulator = engine::pcode_cpu::emulator_named("v810exact.sla").steps() - emulator_before;   // (the instructions the description ran)
    std::printf("pcode_cpu %s: %u runs, %u differ, %llu instructions on the emulator, digest own %08X emulator %08X\n", title, runs, different,
                static_cast<unsigned long long>(on_emulator), digest[0], digest[1]);
    CB_CHECK(on_emulator > runs / 4);
    unsigned shown = 0;
    for (const auto& [k, c] : by_kind)
        if (++shown <= 40) std::printf("    %u x %s   e.g. %s\n", c, k.c_str(), example[k].c_str());
    CB_CHECK_EQ(different, 0u);
}

}  // namespace

CB_TEST("engine.pcode_cpu.v810_instructions_run_as_the_own_core_does", [] { compare_random_programs("v810 instructions", true); });

CB_TEST("engine.pcode_cpu.v810_programs_run_as_the_own_core_does", [] { compare_random_programs("v810 programs", false); });

CB_TEST("engine.pcode_cpu.v810_floating_point_bit_strings_and_system_instructions_run_as_the_own_core_does", [] { compare_random_programs("v810 focused", true, true); });

// The Virtual Boy lane's homebrew on both cores. (The ROMs are plain files: a function's first bytes are at its address less the ROM's base.)
CB_TEST("engine.pcode_cpu.the_virtual_boy_lane_runs_homebrew_the_same_on_both_cores", [] {
    engine::pcode_compare::compare_homebrew("vb", "vb", ".vb", [](const std::filesystem::path&, std::uint32_t address) -> std::size_t {
        return address < formats::vb::kBase ? SIZE_MAX : address - formats::vb::kBase;
    });
});
