// cb-unity: alone (file-local names that other files in a batch also use)
// The AArch64 description's named operations (engine/pcode_ops_a64.cpp) and the instructions of the description that the adapter fixes, against Unicorn: every Advanced SIMD and
// floating-point instruction word drawn at random runs on the description (through the adapter, with nothing handed to the own core) and on Unicorn's AArch64 (QEMU's translator, CPU
// model "max"), from the same registers, and the registers they leave are compared. Each user operation the run reached is credited with the word's verdict, so the report says per
// operation how many words checked it and how many differed; an operation no word could check on Unicorn (its CPU lacks the extension) is listed as unverified.
#include <bit>
#include <cstdio>
#include <cstring>
#include <format>
#include <map>
#include <memory>
#include <random>
#include <set>
#include <string>
#include <vector>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "core/unicorn_api.hpp"
#include "decompile/a64.hpp"
#include "emulate/a64.hpp"
#include "emulate/cpu/pcode_cpu_a64.hpp"
#include "emulate/cpu/pcode_cpu_a64_unicorn.hpp"
#include "emulate/cpu/pcode_cpu_internal.hpp"
#include "emulate/cpu/pcode_ops_a64.hpp"

namespace cb {
namespace {

namespace ea = cb::emulate::a64;
namespace a64_uc = cb::emulate::a64_uc;
namespace da = cb::decompile::a64;
using u64 = std::uint64_t;
using u32 = std::uint32_t;

constexpr u64 kCode = a64_uc::kCode;
using a64_uc::Unicorn;
using a64_uc::run_unicorn;

std::mt19937_64 g_rng(0xA64C0DE);

u64 edge(unsigned ew) {
    const unsigned w = 8 * ew;
    const u64 mask = w >= 64 ? ~0ull : (u64{1} << w) - 1;
    switch (g_rng() % 10) {
        case 0: return 0;
        case 1: return mask;
        case 2: return u64{1} << (w - 1);
        case 3: return (u64{1} << (w - 1)) - 1;
        case 4: return 1;
        case 5: return (u64{1} << (g_rng() % w)) & mask;
        default: return g_rng() & mask;
    }
}

u64 float_lane(unsigned ew) {
    static const double d[] = {0.0, -0.0, 1.0, -1.0, 0.5, 2.0, 3.5, -7.25, 1e300, -1e300, 1e-300, 4.9e-324, 65504.0, 65520.0, 1e10, 123456789.0, 0.1, 1.0 / 3.0, 2.5, 3.5, -2.5, 1e38, 255.0, 256.5};
    const double v = d[g_rng() % std::size(d)];
    switch (ew) {
        case 8:
            switch (g_rng() % 12) {
                case 0: return 0x7FF8000000000000ull;
                case 1: return 0x7FF0000000000001ull;   // a signaling NaN
                case 2: return g_rng() % 2 ? 0x7FF0000000000000ull : 0xFFF0000000000000ull;
                case 3: return g_rng();
                default: return std::bit_cast<u64>(v);
            }
        case 4:
            switch (g_rng() % 12) {
                case 0: return 0x7FC00000u;
                case 1: return 0x7F800001u;
                case 2: return g_rng() % 2 ? 0x7F800000u : 0xFF800000u;
                case 3: return g_rng() & 0xFFFFFFFFu;
                default: return std::bit_cast<std::uint32_t>(static_cast<float>(v));
            }
        default:
            switch (g_rng() % 6) {
                case 0: return 0x7E00;
                case 1: return 0x7C01;
                case 2: return g_rng() % 2 ? 0x7C00 : 0xFC00;
                case 3: return g_rng() & 0xFFFF;
                default: {   // a half from the number
                    const float f = static_cast<float>(v);
                    const std::uint32_t b = std::bit_cast<std::uint32_t>(f);
                    return ((b >> 16) & 0x8000u) | (((b >> 23) & 0xFF) > 112 && ((b >> 23) & 0xFF) < 143 ? ((((b >> 23) & 0xFF) - 112) << 10) | ((b >> 13) & 0x3FF) : 0);
                }
            }
    }
}

ea::V128 random_vector() {
    const unsigned ew = 1u << (g_rng() % 4);
    const bool floats = g_rng() % 2 == 0;
    ea::V128 v;
    std::uint8_t b[16];
    const unsigned fw = floats ? (g_rng() % 3 == 0 ? 2 : g_rng() % 2 ? 4 : 8) : ew;
    for (unsigned i = 0; i < 16; i += fw) {
        const u64 lane = floats ? float_lane(fw) : edge(fw);
        for (unsigned k = 0; k < fw; ++k) b[i + k] = static_cast<std::uint8_t>(lane >> (8 * k));
    }
    std::memcpy(&v.lo, b, 8);
    std::memcpy(&v.hi, b + 8, 8);
    return v;
}

ea::Registers random_registers() {
    ea::Registers r;
    for (u64& x : r.x) x = g_rng() % 4 == 0 ? edge(8) : g_rng() % 3 == 0 ? g_rng() % 70 : g_rng();
    r.pc = kCode;
    r.nzcv = static_cast<u32>(g_rng() & 0xF0000000u);
    for (ea::V128& v : r.v) v = random_vector();
    if (g_rng() % 3 == 0) {   // (a few registers repeated: the operand and the destination often are the same)
        r.v[1] = r.v[2];
        r.v[0] = r.v[3 + g_rng() % 4];
    }
    return r;
}

std::string first_word(const std::string& text) {
    const std::size_t end = text.find(' ');
    return text.substr(0, end);
}

struct Tally {
    unsigned words = 0, same = 0, differ = 0, lacks = 0, refused = 0;
    std::string example;
};

std::string describe(const ea::Registers& want, const ea::Registers& got) {
    std::string out;
    for (unsigned i = 0; i < 31; ++i)
        if (want.x[i] != got.x[i]) out += std::format("x{}[unicorn {:X} desc {:X}] ", i, want.x[i], got.x[i]);
    if (want.nzcv != got.nzcv) out += std::format("nzcv[unicorn {:X} desc {:X}] ", want.nzcv, got.nzcv);
    for (unsigned i = 0; i < 32; ++i) {
        if (want.v[i].lo != got.v[i].lo) out += std::format("v{}.lo[unicorn {:X} desc {:X}] ", i, want.v[i].lo, got.v[i].lo);
        if (want.v[i].hi != got.v[i].hi) out += std::format("v{}.hi[unicorn {:X} desc {:X}] ", i, want.v[i].hi, got.v[i].hi);
    }
    return out;
}

}  // namespace

CB_TEST("emulate.pcode_ops.aarch64_simd_and_fp_words_run_on_the_description_as_on_unicorn", [] {
    const auto given = fs::get_env("CB_PCODE_EXHAUSTIVE");
    const unsigned scale = given && !given->empty() && *given != "0" ? static_cast<unsigned>(std::strtoul(given->c_str(), nullptr, 10)) : 1u;
    Unicorn uni;
    emulate::pcode_ops::set_a64_lane_compat(false);   // (the manual's NaN rules for the maximum and minimum: Unicorn follows the manual)
    struct Restore {
        ~Restore() { emulate::pcode_ops::set_a64_lane_compat(true); }
    } restore;
    std::map<std::string, Tally> by_mnemonic, by_op, lane_op;
    std::map<std::string, unsigned> unbound_hits;
    std::vector<std::string_view> trace;
    // words: by instruction row of the own core's table (so each form is reached), then purely random words of the SIMD and FP space (what only the description decodes)
    std::map<std::uint16_t, unsigned> taken;
    std::vector<u32> words;
    for (unsigned n = 0; n < 4000000 && words.size() < 40000u * scale; ++n) {
        const u32 w = static_cast<u32>(g_rng());
        if ((w & 0x0E000000u) != 0x0E000000u) continue;
        const std::optional<da::Insn> insn = da::decode_word(w, kCode);
        if (insn) {
            if (insn->op == da::Op::Svc || insn->op == da::Op::Brk || insn->op == da::Op::Hlt || insn->op == da::Op::Udf || insn->op == da::Op::Hvc || insn->op == da::Op::Smc) continue;
            if (taken[insn->row]++ >= 30 * scale) continue;
        } else if (g_rng() % 40 != 0) {
            continue;
        }
        words.push_back(w);
    }
    for (const u32 base : {0x4E284800u, 0x4E285800u, 0x4E286800u, 0x4E287800u, 0x5E000000u, 0x5E001000u, 0x5E002000u, 0x5E003000u, 0x5E004000u, 0x5E005000u, 0x5E006000u, 0x5E280800u, 0x5E281800u,
                           0x5E282800u})   // (AESE, AESD, AESMC, AESIMC, SHA1C, SHA1P, SHA1M, SHA1SU0, SHA256H, SHA256H2, SHA256SU1, SHA1H, SHA1SU1, SHA256SU0, SHA256SU1: too few of them among random words)
        for (unsigned k = 0; k < 40; ++k) words.push_back(base | ((base & 0x00FF0000u) == 0 ? static_cast<u32>(g_rng() % 32) << 16 : 0u) | static_cast<u32>(g_rng() % 32) << 5 | static_cast<u32>(g_rng() % 32));
    emulate::pcode_cpu::A64Share before = emulate::pcode_cpu::a64_share();
    (void)before;
    auto& emu = emulate::pcode_cpu::emulator_named("AARCH64.sla");
    unsigned run = 0;
    for (const u32 word : words) {
        const ea::Registers in = random_registers();
        ea::Registers want, got;
        std::string why;
        auto regions = std::make_shared<std::vector<ea::Region>>();
        ea::Region region;
        region.address = kCode;
        for (unsigned i = 0; i < 4; ++i) region.bytes.push_back(static_cast<std::uint8_t>(word >> (8 * i)));
        regions->push_back(std::move(region));
        ea::Memory memory(regions);
        ea::Cpu cpu(memory);
        cpu.regs = in;
        const std::map<std::string, std::uint64_t> unimplemented_before = emu.unimplementedOps();
        trace.clear();
        emulate::pcode_ops::trace_a64(&trace);
        const emulate::pcode_cpu::A64End end = emulate::pcode_cpu::run_a64(cpu, 1, true);
        emulate::pcode_ops::trace_a64(nullptr);
        got = cpu.regs;
        const std::optional<da::Insn> insn = da::decode_word(word, kCode);
        const std::string mnemonic = insn ? std::string(insn->mnemonic) : "?" + first_word(emu.text(kCode));
        Tally& m = by_mnemonic[mnemonic];
        ++m.words;
        ++run;
        std::set<std::string> ops;
        for (const std::string_view name : trace) ops.insert(std::string(name));
        for (const auto& [name, n] : emu.unimplementedOps()) {
            const auto it = unimplemented_before.find(name);
            if (it == unimplemented_before.end() || it->second != n) ++unbound_hits[name];
        }
        const bool has_unbound = emu.unimplementedOps() != unimplemented_before;
        if (end.end == ea::End::Stopped && !end.reason.empty() && end.reason.find("does not run") != std::string::npos) {
            ++m.refused;
            continue;
        }
        if (!run_unicorn(uni, word, in, want, why)) {
            ++m.lacks;
            for (const std::string& op : ops) ++by_op[op].lacks, ++by_op[op].words;
            continue;
        }
        const std::string diff = describe(want, got);
        const bool same = diff.empty();
        if (same) ++m.same;
        else {
            ++m.differ;
            if (m.example.empty()) m.example = std::format("{:08X} {} {}", word, emu.text(kCode), diff.substr(0, 260));
        }
        const bool lane_runs = !emulate::pcode_cpu::lane_stops_at(word);   // (a word the own core stops at is not run by the lane: the description's result there is reported, not required)
        for (const std::string& op : ops) {
            for (Tally* t : {&by_op[op], lane_runs ? &lane_op[op] : nullptr}) {
                if (t == nullptr) continue;
                ++t->words;
                if (same) ++t->same;
                else {
                    ++t->differ;
                    if (t->example.empty()) t->example = std::format("{:08X} {} {}", word, mnemonic, diff.substr(0, 200));
                }
            }
        }
        (void)has_unbound;
    }
    unsigned bad = 0;
    std::printf("pcode_ops a64 vs Unicorn: %u words, by mnemonic:\n", run);
    for (const auto& [name, t] : by_mnemonic) {
        if (t.differ != 0) {
            std::printf("  DIFF %-12s %4u words: %u same, %u differ, %u unicorn lacks, %u description refuses   e.g. %s\n", name.c_str(), t.words, t.same, t.differ, t.lacks, t.refused, t.example.c_str());
        }
    }
    for (const auto& [name, t] : lane_op)
        if (t.differ != 0) {
            ++bad;
            std::printf("  LANE DIFF %-18s %4u words the lane runs: %u same, %u differ   e.g. %s\n", name.c_str(), t.words, t.same, t.differ, t.example.c_str());
        }
    std::printf("pcode_ops a64 vs Unicorn: the operations\n");
    for (const auto& [name, t] : by_op) std::printf("  %-18s %4u words: %u same, %u differ, %u on a CPU Unicorn lacks%s%s\n", name.c_str(), t.words, t.same, t.differ, t.lacks, t.differ ? "   e.g. " : "", t.example.c_str());
    std::printf("pcode_ops a64: operations reached with nothing to serve them:\n");
    const auto shapes = emulate::pcode_ops::a64_recorded_shapes();
    for (const auto& [name, n] : unbound_hits) {
        std::printf("  %s (%u words)", name.c_str(), n);
        const auto it = shapes.find(name);
        if (it != shapes.end())
            for (const std::string& shape : it->second) std::printf("  [%s]", shape.c_str());
        std::printf("\n");
    }
    {
        const std::vector<std::string> unserved = emulate::pcode_ops::a64_unserved(emu);
        std::size_t neon = 0;
        for (const std::string& name : unserved) neon += name.rfind("NEON_", 0) == 0;
        std::printf("pcode_ops a64: %zu NEON operations and %zu SVE operations of the description are not served:", neon, unserved.size() - neon);
        for (const std::string& name : unserved)
            if (name.rfind("NEON_", 0) == 0) std::printf(" %s", name.c_str());
        std::printf("\n");
    }
    std::printf("pcode_ops a64: mnemonics Unicorn lacks and the description refuses:\n");
    for (const auto& [name, t] : by_mnemonic)
        if (t.same == 0 && t.differ == 0) std::printf("  %-12s %4u words, unicorn lacks %u, description refuses %u\n", name.c_str(), t.words, t.lacks, t.refused);
    CB_CHECK_EQ(bad, 0u);
});

}  // namespace cb
