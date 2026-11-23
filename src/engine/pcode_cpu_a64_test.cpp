// cb-unity: alone
// The Switch lane's AArch64 on the one emulator (engine/pcode_cpu_a64.cpp): the same run as the own core (emulate/a64) on random instructions and programs, and on the lane's
// homebrew.
#include <bit>
#include <cstdio>
#include <format>
#include <map>
#include <memory>
#include <random>
#include <string>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "decompile/a64.hpp"
#include "emulate/a64.hpp"
#include "engine/pcode_cpu_a64.hpp"
#include "engine/pcode_cpu_compare.hpp"

namespace cb {
namespace {

namespace ea = cb::emulate::a64;
namespace da = cb::decompile::a64;

constexpr std::uint64_t kCode = 0x10000, kCodeSize = 0x1000, kData = 0x100000, kDataSize = 0x10000, kRo = 0x200000, kRoSize = 0x1000;

// What one run leaves: the registers, the way it ended, the kernel calls and what changed in memory.
struct Outcome {
    engine::pcode_cpu::A64End end;
    ea::Registers regs;
    std::uint64_t steps = 0;
    std::vector<ea::SvcCall> calls;
    std::vector<std::pair<std::uint64_t, std::uint8_t>> changes;
};

struct Case {
    ea::Regions regions;
    ea::Registers regs;
    std::uint64_t seed_counter = 0;
};

// The own core's loop (emulate::a64::run's), on a Cpu set up by the caller.
engine::pcode_cpu::A64End run_own(ea::Cpu& cpu, std::uint64_t max_steps) {
    engine::pcode_cpu::A64End end;
    while (true) {
        if (cpu.regs.pc == ea::kReturnAddress) {
            end.end = ea::End::Return;
            break;
        }
        if (cpu.steps >= max_steps) {
            end.end = ea::End::Limit;
            break;
        }
        if (const std::optional<std::string> why = cpu.step()) {
            end.end = ea::End::Stopped;
            end.reason = *why;
            break;
        }
    }
    return end;
}

Outcome play(const Case& c, bool emulator, std::uint64_t max_steps) {
    ea::Memory memory(c.regions);
    memory.map_writable(kData, kDataSize);
    ea::Cpu cpu(memory);
    cpu.regs = c.regs;
    Outcome o;
    o.end = emulator ? engine::pcode_cpu::run_a64(cpu, max_steps) : run_own(cpu, max_steps);
    o.regs = cpu.regs;
    o.steps = cpu.steps;
    o.calls = cpu.calls;
    o.changes = memory.changes(1 << 20);
    return o;
}

std::string describe(const Outcome& a, const Outcome& b) {
    std::string out;
    if (a.end.end != b.end.end || a.end.reason != b.end.reason) out += std::format("end('{}' '{}' / '{}' '{}') ", static_cast<int>(a.end.end), a.end.reason, static_cast<int>(b.end.end), b.end.reason);
    const auto reg = [&](const std::string& name, std::uint64_t x, std::uint64_t y) {
        if (x != y) out += std::format("{}[own {:X} emu {:X}] ", name, x, y);
    };
    for (unsigned i = 0; i < 31; ++i) reg(std::format("x{}", i), a.regs.x[i], b.regs.x[i]);
    reg("sp", a.regs.sp, b.regs.sp);
    reg("pc", a.regs.pc, b.regs.pc);
    reg("nzcv", a.regs.nzcv, b.regs.nzcv);
    reg("fpcr", a.regs.fpcr, b.regs.fpcr);
    reg("fpsr", a.regs.fpsr, b.regs.fpsr);
    reg("tpidr", a.regs.tpidr_el0, b.regs.tpidr_el0);
    reg("tpidrro", a.regs.tpidrro_el0, b.regs.tpidrro_el0);
    for (unsigned i = 0; i < 32; ++i) {
        reg(std::format("v{}.lo", i), a.regs.v[i].lo, b.regs.v[i].lo);
        reg(std::format("v{}.hi", i), a.regs.v[i].hi, b.regs.v[i].hi);
    }
    reg("steps", a.steps, b.steps);
    if (a.calls.size() != b.calls.size()) out += std::format("calls[{}/{}] ", a.calls.size(), b.calls.size());
    else
        for (std::size_t i = 0; i < a.calls.size(); ++i)
            if (a.calls[i].number != b.calls[i].number || a.calls[i].args != b.calls[i].args || a.calls[i].note != b.calls[i].note) {
                out += std::format("call@{} ", i);
                break;
            }
    if (a.changes != b.changes) {
        std::size_t i = 0;
        while (i < a.changes.size() && i < b.changes.size() && a.changes[i] == b.changes[i]) ++i;
        out += std::format("memory@{} [own {} emu {}] ", i, i < a.changes.size() ? std::format("{:X}={:02X}", a.changes[i].first, a.changes[i].second) : "-",
                           i < b.changes.size() ? std::format("{:X}={:02X}", b.changes[i].first, b.changes[i].second) : "-");
    }
    return out;
}

std::uint64_t value(std::mt19937_64& rng) {
    switch (rng() % 14) {
        case 0: return 0;
        case 1: return 1;
        case 2: return ~0ull;
        case 3: return rng() % 64;
        case 4: return 0x7FFFFFFFull + rng() % 3 - 1;
        case 5: return 0x80000000ull + rng() % 3 - 1;
        case 6: return 0x7FFFFFFFFFFFFFFFull + rng() % 3 - 1;
        case 7: return 0x8000000000000000ull;
        case 8: case 9: return kData + rng() % (kDataSize - 0x100);
        case 10: return (kData + rng() % (kDataSize - 0x100)) & ~7ull;
        case 11: return kRo + rng() % kRoSize;
        case 12: return kCode + (rng() % kCodeSize & ~3ull);
        default: return rng();
    }
}

// A 64-bit pattern that is interesting as a double, as two singles, as a half, or just bits.
std::uint64_t vector_value(std::mt19937_64& rng) {
    static const double specials[] = {0.0, -0.0, 1.0, -1.0, 0.5, 2.0, 3.5, -7.25, 1e300, -1e300, 1e-300, 4.9e-324, 65504.0, 65520.0, 1e10, 123456789.0, 0.1, 1.0 / 3.0};
    static const float fspecials[] = {0.0f, -0.0f, 1.0f, -1.0f, 0.5f, 2.0f, 3.5f, -7.25f, 3e38f, -3e38f, 1e-38f, 1e-45f, 65504.0f, 1e10f, 16777216.0f, 0.1f};
    const auto single = [&] { return std::bit_cast<std::uint32_t>(fspecials[rng() % std::size(fspecials)]); };
    switch (rng() % 10) {
        case 0: case 1: return std::bit_cast<std::uint64_t>(specials[rng() % std::size(specials)]);
        case 2: case 3: return static_cast<std::uint64_t>(single()) | (rng() % 2 ? static_cast<std::uint64_t>(single()) << 32 : 0);
        case 4: return 0x7FF8000000000000ull | (rng() % 2);   // NaN
        case 5: return rng() % 2 ? 0x7FF0000000000000ull : 0xFFF0000000000000ull;   // infinity
        case 6: return 0x7FC00000u | (rng() % 2 ? 0x80000000u : 0);
        case 7: return rng() % 100000;
        default: return rng();
    }
}

Case make_case(std::mt19937_64& rng, const std::vector<std::uint32_t>& words) {
    Case c;
    auto regions = std::make_shared<std::vector<ea::Region>>();
    ea::Region code;
    code.address = kCode;
    for (const std::uint32_t w : words)
        for (unsigned i = 0; i < 4; ++i) code.bytes.push_back(static_cast<std::uint8_t>(w >> (8 * i)));
    code.bytes.resize(kCodeSize, 0);
    regions->push_back(std::move(code));
    ea::Region ro;
    ro.address = kRo;
    for (std::size_t i = 0; i < kRoSize; ++i) ro.bytes.push_back(static_cast<std::uint8_t>(rng()));
    regions->push_back(std::move(ro));
    ea::Region data;   // (the data: the writable memory starts from this)
    data.address = kData;
    data.writable = true;
    for (std::size_t i = 0; i < kDataSize; ++i) data.bytes.push_back(static_cast<std::uint8_t>(rng() % 5 == 0 ? rng() : i * 7));
    regions->push_back(std::move(data));
    c.regions = regions;
    for (std::uint64_t& x : c.regs.x) x = value(rng);
    c.regs.x[30] = ea::kReturnAddress;
    c.regs.sp = kData + 0x8000 + (rng() % 0x100 & ~0xFull);
    c.regs.pc = kCode;
    c.regs.nzcv = static_cast<std::uint32_t>(rng() & 0xF0000000u);
    for (ea::V128& v : c.regs.v) v = {vector_value(rng), rng() % 3 == 0 ? vector_value(rng) : 0};
    c.regs.tpidr_el0 = kData + 0x4000;
    c.regs.tpidrro_el0 = kData + 0x5000;
    return c;
}

// A word the own core's decoder knows, drawn from the instruction rows evenly: `per_row` of each row at most.
struct Word {
    std::uint32_t word;
    std::uint16_t row;
    da::Op op;
    std::string mnemonic;
};

std::vector<Word> draw_words(std::mt19937_64& rng, unsigned per_row, unsigned attempts) {
    std::map<std::uint16_t, unsigned> taken;
    std::vector<Word> words;
    for (unsigned n = 0; n < attempts; ++n) {
        std::uint32_t w = static_cast<std::uint32_t>(rng());
        if (rng() % 4 == 0) w = (w & 0xFFFF03E0u) | ((rng() % 32) << 5) | (w & 31);   // (a base of 31, the stack pointer, or a nearby register)
        const std::optional<da::Insn> insn = da::decode_word(w, kCode);
        if (!insn) continue;
        if (insn->op == da::Op::Svc || insn->op == da::Op::Brk || insn->op == da::Op::Hlt || insn->op == da::Op::Udf) {
            if (taken[insn->row]++ >= per_row / 8 + 1) continue;
        } else if (taken[insn->row]++ >= per_row) {
            continue;
        }
        words.push_back({w, insn->row, insn->op, std::string(insn->mnemonic)});
    }
    return words;
}

bool is_flow(da::Op op) {
    switch (op) {
        case da::Op::B: case da::Op::Bl: case da::Op::BCond: case da::Op::Cbz: case da::Op::Tbz: case da::Op::Br: case da::Op::Blr: case da::Op::Ret: case da::Op::Eret: case da::Op::Svc:
        case da::Op::Hvc: case da::Op::Smc: case da::Op::Brk: case da::Op::Hlt: case da::Op::Udf: return true;
        default: return false;
    }
}

void compare_random(bool single, const char* title) {
    std::mt19937_64 rng(single ? 20261201 : 20261202);
    const unsigned scale = [] {
        const auto given = fs::get_env("CB_PCODE_EXHAUSTIVE");
        return given && !given->empty() && *given != "0" ? static_cast<unsigned>(std::strtoul(given->c_str(), nullptr, 10)) : 1u;
    }();
    const std::vector<Word> words = draw_words(rng, single ? 24 * scale : 10 * scale, single ? 3000000 : 1500000);
    std::map<std::string, unsigned> by_kind;
    std::map<std::string, std::string> example;
    unsigned runs = 0, different = 0;
    std::uint64_t executed = 0;
    engine::pcode_cpu::reset_a64_share();
    std::vector<std::uint32_t> pool;   // (the straight-line words programs are made of)
    for (const Word& w : words)
        if (!is_flow(w.op)) pool.push_back(w.word);
    const unsigned program_count = static_cast<unsigned>(words.size() / 6);
    const unsigned total = single ? static_cast<unsigned>(words.size()) : program_count;
    for (unsigned n = 0; n < total; ++n) {
        std::vector<std::uint32_t> code;
        std::string culprit_name = "?";
        if (single) {
            code = {words[n].word, 0xD65F03C0u /* ret */};
            culprit_name = words[n].mnemonic;
        } else {
            for (unsigned k = 0; k < 24; ++k) code.push_back(pool[rng() % pool.size()]);
            code.push_back(0xD65F03C0u);
        }
        const Case c = make_case(rng, code);
        const std::uint64_t steps = single ? 1 : 40;
        const Outcome run[2] = {play(c, false, steps), play(c, true, steps)};
        ++runs;
        executed += run[0].steps;
        std::string fields = describe(run[0], run[1]);
        if (fields.empty()) continue;
        ++different;
        if (!single) {   // (a program: the first instruction after which they differ)
            unsigned k0 = 0;
            for (unsigned k = 1; k <= steps && k0 == 0; ++k)
                if (!describe(play(c, false, k), play(c, true, k)).empty()) k0 = k;
            if (k0 != 0) {
                const Outcome before = play(c, false, k0 - 1);
                std::uint64_t at = before.regs.pc;
                std::uint32_t w = 0;
                if (at >= kCode && at < kCode + code.size() * 4) w = code[(at - kCode) / 4];
                const auto insn = da::decode_word(w, at);
                culprit_name = insn ? std::string(insn->mnemonic) : "?";
                fields = std::format("[step {} {:08X}] ", k0, w) + describe(play(c, false, k0), play(c, true, k0));
            }
        } else {
            fields = std::format("{:08X} ", code[0]) + fields;
        }
        const std::string key = std::string(single ? "1: " : "N: ") + culprit_name;
        ++by_kind[key];
        if (!example.count(key)) example[key] = fields;
    }
    const engine::pcode_cpu::A64Share share = engine::pcode_cpu::a64_share();
    std::printf("pcode_cpu %s: %u runs, %llu instructions, %u differ; steps on the emulator %llu, on the own core %llu\n", title, runs, static_cast<unsigned long long>(executed), different,
                static_cast<unsigned long long>(share.emulator), static_cast<unsigned long long>(share.own_core));
    unsigned shown = 0;
    for (const auto& [k, count] : by_kind)
        if (++shown <= 80) std::printf("    %u x %s   e.g. %s\n", count, k.c_str(), example[k].substr(0, 500).c_str());
    CB_CHECK(share.emulator > share.own_core / 4);
    CB_CHECK_EQ(different, 0u);
}

}  // namespace

// Sequences of the exclusive loads and stores, the ordered ones and clrex on a few addresses: the monitor must be the own core's, across the two cores' instructions.
void compare_exclusives() {
    std::mt19937_64 rng(20261203);
    unsigned runs = 0, different = 0;
    std::map<std::string, std::string> example;
    engine::pcode_cpu::reset_a64_share();
    for (unsigned n = 0; n < 4000; ++n) {
        std::vector<std::uint32_t> code;
        const auto reg = [&] { return static_cast<std::uint32_t>(3 + rng() % 7); };
        const auto base = [&] { return static_cast<std::uint32_t>(1 + rng() % 2); };
        for (unsigned k = 0; k < 10; ++k) {
            const std::uint32_t size = static_cast<std::uint32_t>(rng() % 4) << 30, rt = reg(), rt2 = reg(), rs = reg(), rn = rng() % 8 == 0 ? 31 : base();
            const std::uint32_t pair_size = (rng() % 2 ? 3u : 2u) << 30;
            switch (rng() % 12) {
                case 0: code.push_back(0x085F7C00u | size | rn << 5 | rt); break;                                        // ldxr
                case 1: code.push_back(0x085FFC00u | size | rn << 5 | rt); break;                                        // ldaxr
                case 2: case 3: code.push_back(0x08007C00u | size | rs << 16 | rn << 5 | rt); break;                     // stxr
                case 4: code.push_back(0x0800FC00u | size | rs << 16 | rn << 5 | rt); break;                             // stlxr
                case 5: code.push_back(0x887F0000u | (pair_size & 0x40000000u) | 0x80000000u | rt2 << 10 | rn << 5 | rt); break;   // ldxp
                case 6: code.push_back(0x88200000u | pair_size | rs << 16 | rt2 << 10 | rn << 5 | rt); break;            // stxp
                case 7: code.push_back(0xD503305Fu); break;                                                              // clrex
                case 8: code.push_back(0x08DFFC00u | size | rn << 5 | rt); break;                                        // ldar
                case 9: code.push_back(0x089FFC00u | size | rn << 5 | rt); break;                                        // stlr
                case 10: code.push_back(0x8820FC00u | pair_size | rs << 16 | rt2 << 10 | rn << 5 | rt); break;           // stlxp
                default: code.push_back(0x88FF7C00u | pair_size | rn << 5 | rt); break;                                  // (a compare-and-swap: the own core's)
            }
        }
        code.push_back(0xD65F03C0u);
        Case c = make_case(rng, code);
        c.regs.x[1] = kData + 0x100 + 16 * (rng() % 3);
        c.regs.x[2] = rng() % 2 ? c.regs.x[1] : kData + 0x100 + 16 * (rng() % 3);
        const Outcome run[2] = {play(c, false, 40), play(c, true, 40)};
        ++runs;
        const std::string fields = describe(run[0], run[1]);
        if (fields.empty()) continue;
        if (++different <= 6) std::printf("  program %u: %s\n", n, fields.substr(0, 300).c_str());
    }
    const engine::pcode_cpu::A64Share share = engine::pcode_cpu::a64_share();
    std::printf("pcode_cpu a64 exclusives: %u runs, %u differ; steps on the emulator %llu, on the own core %llu\n", runs, different, static_cast<unsigned long long>(share.emulator),
                static_cast<unsigned long long>(share.own_core));
    CB_CHECK_EQ(different, 0u);
}
}  // namespace cb

CB_TEST("engine.pcode_cpu.a64_instructions_run_as_the_own_core_does", [] { cb::compare_random(true, "a64 instructions"); });

CB_TEST("engine.pcode_cpu.a64_programs_run_as_the_own_core_does", [] { cb::compare_random(false, "a64 programs"); });

// The lane's homebrew on both cores. (An NRO's flat image is the file: a function's first bytes are at its address.)
CB_TEST("engine.pcode_cpu.the_switch_lane_runs_homebrew_the_same_on_both_cores", [] {
    cb::engine::pcode_cpu::reset_a64_share();
    cb::engine::pcode_compare::compare_homebrew("switch", "switch", ".nro", [](const std::filesystem::path& path, std::uint32_t address) -> std::size_t {
        return address < cb::fs::file_size(path) ? address : SIZE_MAX;
    });
    const cb::engine::pcode_cpu::A64Share share = cb::engine::pcode_cpu::a64_share();
    const double total = static_cast<double>(share.emulator + share.own_core);
    std::printf("pcode_cpu switch homebrew steps: %llu on the emulator, %llu on the own core (%.2f%%)\n", static_cast<unsigned long long>(share.emulator),
                static_cast<unsigned long long>(share.own_core), total > 0 ? 100.0 * static_cast<double>(share.own_core) / total : 0.0);
    std::multimap<std::uint64_t, std::string, std::greater<>> ordered;
    for (const auto& [name, n] : share.own_core_by_mnemonic) ordered.emplace(n, name);
    unsigned shown = 0;
    for (const auto& [n, name] : ordered)
        if (++shown <= 25) std::printf("    %llu x %s\n", static_cast<unsigned long long>(n), name.c_str());
});

CB_TEST("engine.pcode_cpu.a64_exclusives_run_as_the_own_core_does", [] { cb::compare_exclusives(); });
