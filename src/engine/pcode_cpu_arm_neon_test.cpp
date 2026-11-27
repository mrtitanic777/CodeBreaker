// cb-unity: alone
// The Advanced SIMD data-processing instructions and the NEON/core transfers (decompile/arm32_neon_dp.hpp) of 32-bit ARM: the own core (emulate/arm32_neon_dp.cpp) and the one
// emulator's adapter (engine/pcode_neon_arm32.cpp) each run random instructions from random registers, ARM and Thumb, and their results are compared with Unicorn's Cortex-A15
// (QEMU's translator: ARMv7-A, VFPv4, Advanced SIMD), register by register: the core registers, the flags, d0-d31 and the floating point status register.
#include <array>
#include <bit>
#include <cstdio>
#include <cstdlib>
#include <format>
#include <map>
#include <memory>
#include <optional>
#include <random>
#include <string>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "decompile/arm.hpp"
#include "decompile/arm32_neon_dp.hpp"
#include "decompile/arm_ext.hpp"
#include "emulate/arm.hpp"
#include "engine/pcode_cpu.hpp"
#include "engine/pcode_cpu_arm.hpp"
#include "engine/pcode_cpu_arm_unicorn.hpp"
#include "engine/pcode_neon_arm32.hpp"

namespace cb {
namespace {

namespace ar = cb::emulate::arm;
namespace da = cb::decompile::arm;
namespace t2 = cb::decompile::arm::t2;
namespace uc = cb::engine::arm_uc;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

constexpr u32 kVitaCode = 0x81000000;

unsigned runs_from_env(const char* name, unsigned fallback) {
    const std::optional<std::string> v = cb::fs::get_env(name);
    return v && !v->empty() ? static_cast<unsigned>(std::strtoul(v->c_str(), nullptr, 10)) : fallback;
}

u32 random_word32(std::mt19937& rng) {
    static const u32 specials[] = {0u, 0x80000000u, 0x3F800000u, 0xBF800000u, 0x7F800000u, 0xFF800000u, 0x7FC00000u, 0x7F800001u, 0xFFC00001u, 1u, 0x80000001u, 0x007FFFFFu,
                                   0x00800000u, 0x7F7FFFFFu, 0x40000000u, 0xC0000000u, 0x3F000000u, 0x00400000u, 0x807FFFFFu, 0x40400000u, 0x3FC00000u, 0x7E800000u, 0x7F000000u, 0x01000000u};
    static const u32 bytes[] = {0x00, 0x01, 0x7F, 0x80, 0xFF, 0xFE, 0x02};
    static const u32 halves[] = {0x0000, 0x7FFF, 0x8000, 0xFFFF, 0x0001, 0xFFFE};
    switch (rng() % 20) {
        case 0: case 1: case 2: case 3: case 4: case 5: case 6: return static_cast<u32>(rng());
        case 7: case 8: case 9: case 10: return specials[rng() % (sizeof specials / sizeof specials[0])];
        case 11: case 12: case 13: { const u32 b = bytes[rng() % 7]; return b * 0x01010101u; }
        case 14: case 15: { const u32 h = halves[rng() % 6]; return h * 0x00010001u; }
        case 16: case 17: return rng() % 40 - 8;
        case 18: return rng() % 2 ? 0x80000000u : 0x7FFFFFFFu;
        default: return (bytes[rng() % 7]) | (bytes[rng() % 7] << 8) | (bytes[rng() % 7] << 16) | (bytes[rng() % 7] << 24);
    }
}

uc::State random_state(std::mt19937& rng) {
    uc::State s;
    for (u32& r : s.r) r = random_word32(rng);
    for (u64& d : s.d) d = static_cast<u64>(random_word32(rng)) | static_cast<u64>(random_word32(rng)) << 32;
    s.nzcv = static_cast<u32>(rng()) & 0xF0000000u;
    s.fpscr = (static_cast<u32>(rng()) & 0xF0000000u) | (static_cast<u32>(rng()) & 0x0BC00000u);   // (the flags, QC, DN, FZ, the rounding mode: Advanced SIMD sees none of the last four)
    return s;
}

// One word of the family: a data-processing instruction, or a transfer between the core registers and the scalars.
u32 random_neon_word(std::mt19937& rng) {
    const u32 r = static_cast<u32>(rng());
    switch (rng() % 14) {
        case 10: return (rng() % 2 ? 0xEEA00A00u : 0xEE900A00u) | (r & 0x004FF1EFu);   // VFMA, VFMS, VFNMA, VFNMS
        case 11: return 0xEEBA0A40u | (r & 0x0045F1AFu);                               // VCVT between floating point and fixed point
        case 12: return 0xEEB20A40u | (r & 0x0041F0AFu);                               // VCVTB, VCVTT
        case 13: return 0xEC400A10u | (r & 0x001FF02Fu);                               // VMOV between two core registers and two singles
        case 0: return 0xEE000B10u | (r & 0x00FFF0E0u);   // VMOV (core register to scalar, scalar to core register), VDUP: the bits that fix the class are set
        default: return (rng() % 2 ? 0xF2000000u : 0xF3000000u) | (r & 0x01FFFFFFu);
    }
}

// The instruction as bytes in the form the state it runs in has: an ARM word, or the two halfwords of Thumb.
void encode(u32 w, bool thumb, std::uint8_t out[4]) {
    if (!thumb) {
        for (unsigned k = 0; k < 4; ++k) out[k] = static_cast<std::uint8_t>(w >> (8 * k));
        return;
    }
    const u32 first = (w >> 24) == 0xF2 ? 0xEF00u | (w >> 16 & 0xFF) : (w >> 24) == 0xF3 ? 0xFF00u | (w >> 16 & 0xFF) : w >> 16;
    const u32 second = w & 0xFFFF;
    out[0] = static_cast<std::uint8_t>(first);
    out[1] = static_cast<std::uint8_t>(first >> 8);
    out[2] = static_cast<std::uint8_t>(second);
    out[3] = static_cast<std::uint8_t>(second >> 8);
}

// The own core on one instruction.
bool run_own(const std::uint8_t code[4], bool thumb, const uc::State& in, uc::State& out, std::string& why) {
    ar::Bus bus(ar::System::Vita);
    Bytes mem(0x1000);
    for (unsigned k = 0; k < 4; ++k) mem[k] = code[k];
    const auto regions = std::make_shared<std::vector<emulate::paged_memory::Region>>();
    regions->push_back({kVitaCode, mem});
    bus.set_regions(regions);
    ar::Cpu cpu(bus);
    cpu.set_cpsr(0x1F | (in.nzcv & 0xF0000000u));
    for (unsigned i = 0; i < 13; ++i) cpu.r[i] = in.r[i];
    cpu.r[13] = 0x20000;
    cpu.r[14] = 0;
    for (unsigned i = 0; i < 32; ++i) {
        cpu.set_vfp_single(2 * i, static_cast<u32>(in.d[i]));
        cpu.set_vfp_single(2 * i + 1, static_cast<u32>(in.d[i] >> 32));
    }
    cpu.set_vfp_status(in.fpscr);
    cpu.jump_to(kVitaCode | (thumb ? 1u : 0u));
    if (const std::optional<ar::Fault> fault = cpu.step()) {
        why = fault->text();
        return false;
    }
    out = in;
    for (unsigned i = 0; i < 13; ++i) out.r[i] = cpu.r[i];
    for (unsigned i = 0; i < 32; ++i) out.d[i] = static_cast<u64>(cpu.vfp_single(2 * i)) | static_cast<u64>(cpu.vfp_single(2 * i + 1)) << 32;
    out.nzcv = cpu.cpsr & 0xF0000000u;
    out.fpscr = cpu.vfp_status();
    return true;
}

// The lane library's version (engine/pcode_neon_arm32.cpp) on the same instruction.
void run_engine(const t2::NeonDp& op, const uc::State& in, uc::State& out) {
    engine::pcode_neon_arm32::Regs regs;
    for (unsigned i = 0; i < 32; ++i) regs.d[i] = in.d[i];
    for (unsigned i = 0; i < 13; ++i) regs.r[i] = in.r[i];
    regs.r[13] = 0x20000;   // (the stack pointer Unicorn is given)
    for (unsigned i = 0; i < 13; ++i) regs.r[i] = in.r[i];
    const u32 bits = engine::pcode_neon_arm32::run(op, regs);
    out = in;
    for (unsigned i = 0; i < 32; ++i) out.d[i] = regs.d[i];
    for (unsigned i = 0; i < 13; ++i) out.r[i] = regs.r[i];
    out.fpscr = in.fpscr | bits;
}

std::string difference(const uc::State& a, const uc::State& b, u32 fpscr_mask = 0xFFFFFFFFu) {
    std::string out;
    for (unsigned i = 0; i < 13; ++i)
        if (a.r[i] != b.r[i]) out += std::format("r{}[{:X}/{:X}] ", i, a.r[i], b.r[i]);
    for (unsigned i = 0; i < 32; ++i)
        if (a.d[i] != b.d[i]) out += std::format("d{}[{:016X}/{:016X}] ", i, a.d[i], b.d[i]);
    if (a.nzcv != b.nzcv) out += std::format("nzcv[{:X}/{:X}] ", a.nzcv, b.nzcv);
    if ((a.fpscr ^ b.fpscr) & fpscr_mask) out += std::format("fpscr[{:08X}/{:08X}] ", a.fpscr, b.fpscr);
    return out;
}

const char* const kOpNames[] = {
    "Hadd", "Rhadd", "Qadd", "Hsub", "Qsub", "Cgt", "Cge", "ShiftReg", "Max", "Min", "Abd", "Aba", "Add", "Sub", "Tst", "Ceq", "Mla", "Mls", "Mul", "MulP", "Pmax", "Pmin", "Qdmulh", "Padd", "And", "Bic", "Orr", "Orn",
    "Eor", "Bsl", "Bit", "Bif", "Fma", "Fms", "FAdd", "FSub", "FPadd", "FAbd", "FMla", "FMls", "FMul", "FCeq", "FCge", "FCgt", "FAcge", "FAcgt", "FMax", "FMin", "FPmax", "FPmin", "Recps", "Rsqrts", "Addl", "Addw", "Subl",
    "Subw", "AddHn", "SubHn", "Abal", "Abdl", "Mlal", "Mlsl", "Mull", "MullP", "Qdmlal", "Qdmlsl", "Qdmull", "Shr", "Sri", "ShlI", "Sli", "Qshlu", "QshlI", "ShrN", "Qshrn", "Qshrun", "Shll", "CvtFixed", "Rev", "Paddl",
    "Padal", "Cls", "Clz", "Cnt", "Mvn", "Qabs", "Qneg", "CmpZero", "Abs", "Neg", "Swp", "Trn", "Uzp", "Zip", "Movn", "Qmovn", "Qmovun", "Cvt16", "Recpe", "Rsqrte", "CvtIntFloat", "Ext", "Tbl", "DupScalar",
    "MovToScalar", "MovFromScalar", "DupCore", "VFma", "VCvtFx", "VCvtHalf", "VMovPair"};

// The own core against Unicorn on random instructions: the number of instructions run, of differences, of words Unicorn runs that the decoder does not read, and of words the
// decoder reads that Unicorn refuses.
CB_TEST("engine.pcode_cpu_arm_neon.the_own_core_matches_unicorn_on_random_instructions", [] {
    uc::Unicorn unicorn;
    std::mt19937 rng(20261124);
    const unsigned count = runs_from_env("CB_ARM_NEON_RUNS", 20000);
    unsigned compared = 0, different = 0, engine_different = 0, undecoded = 0, spurious = 0, unrun = 0;
    std::map<std::string, unsigned> per_op, bad_op;
    std::map<std::string, std::string> sample;
    std::map<std::string, unsigned> missing, refused;
    for (unsigned n = 0; n < count; ++n) {
        const u32 w = random_neon_word(rng);
        const bool thumb = rng() % 2 != 0;
        if (thumb && (w >> 28) != 0xF && (w >> 28) != 0xE) continue;
        const u32 as_thumb = thumb && (w >> 24 & 0xF) == 0xE ? (w & 0x0FFFFFFFu) | 0xE0000000u : w;
        const std::optional<t2::NeonDp> op = t2::neon_dp(as_thumb, thumb);
        std::uint8_t code[4];
        encode(as_thumb, thumb, code);
        uc::State in = random_state(rng);
        const bool vfp_form = op && op->op >= t2::NeonOp::VFma;
        if (vfp_form) in.fpscr &= 0xF0000000u;   // (the VFP forms run with the control bits clear; the own core keeps no exception flags of the VFP)
        const u32 fpscr_mask = vfp_form ? 0xF0000000u : 0xFFFFFFFFu;
        uc::State want, got;
        std::string why;
        const bool unicorn_ran = uc::run_unicorn(unicorn, code, 4, thumb, in, want, why);
        if (!op) {
            if (unicorn_ran && (as_thumb >> 24 == 0xF2 || as_thumb >> 24 == 0xF3 || (as_thumb >> 24 & 0xF) == 0xE)) {   // an Advanced SIMD word the decoder does not read, that is no modified immediate
                const bool immediate = t2::neon_modified_immediate(as_thumb) || t2::vfp_move_immediate(as_thumb) || t2::neon_transfer(as_thumb);
                const bool core = (as_thumb >> 24) == 0xEE || (as_thumb >> 24 & 0xF) == 0xE;
                const unsigned rt = as_thumb >> 12 & 15;
                if (core && (rt == 15 || rt == 13)) continue;   // (UNPREDICTABLE)
                // Two encodings the manual leaves unallocated and QEMU runs: VMUL (floating point) with bit 21 set, and VQDMULL with U set.
                if ((as_thumb & 0x0FBA0E50u) == 0x0EBA0A40u) continue;   // (VCVT with more fraction bits than the fixed point number has: UNPREDICTABLE)
                if ((as_thumb & 0xFFA00F10u) == 0xF3200D10u || (as_thumb & 0xFF800F50u) == 0xF3800D00u) continue;
                if (!immediate) {
                    ++undecoded;
                    const u32 mask = core ? 0xFF900F70u : ((as_thumb >> 23 & 1) && (as_thumb >> 20 & 3) == 3) ? 0xFF9F0F50u : 0xFF900F50u;
                    const std::string key = std::format("{:08X}", as_thumb & mask);
                    if (++missing[key] == 1) sample[key] = std::format("{:08X}{}", as_thumb, thumb ? " (thumb)" : "");
                }
            }
            continue;
        }
        if ((op->op == t2::NeonOp::Swp || op->op == t2::NeonOp::Trn || op->op == t2::NeonOp::Uzp || op->op == t2::NeonOp::Zip) && op->d == op->m) continue;   // (the manual leaves it UNKNOWN)
        const std::string name = std::string(kOpNames[static_cast<unsigned>(op->op)]) + (op->f ? ".f" : "") + (op->q ? ".q" : "");
        if (!unicorn_ran) {
            ++spurious;
            if (++refused[name] <= 1) std::printf("    spurious: %s %08X (%s)\n", name.c_str(), as_thumb, why.c_str());
            continue;
        }
        if (!run_own(code, thumb, in, got, why)) {
            ++unrun;
            if (++bad_op["notrun " + name] <= 1) std::printf("    own core did not run: %s %08X %s\n", name.c_str(), as_thumb, why.c_str());
            continue;
        }
        ++compared;
        ++per_op[name];
        {
            uc::State lanes;
            run_engine(*op, in, lanes);
            const std::string diff = difference(lanes, want);
            if (!diff.empty()) {
                ++engine_different;
                if (++bad_op["lane library " + name] <= 2) {
                    std::printf("    DIFF (lane library) %s %08X thumb=%d library/unicorn: %s\n", name.c_str(), as_thumb, thumb ? 1 : 0, diff.substr(0, 400).c_str());
                    std::printf("         in n=%016llX m=%016llX d=%016llX (n,m,d = d%u, d%u, d%u) %s\n", static_cast<unsigned long long>(in.d[op->n & 31]), static_cast<unsigned long long>(in.d[op->m & 31]),
                                static_cast<unsigned long long>(in.d[op->d & 31]), op->n, op->m, op->d, op->q ? "(+1 each)" : "");
                }
            }
        }
        const std::string diff = difference(got, want, fpscr_mask);
        if (!diff.empty()) {
            ++different;
            if (++bad_op[name] <= 2) {
                std::printf("    DIFF %s %08X thumb=%d own/unicorn: %s\n", name.c_str(), as_thumb, thumb ? 1 : 0, diff.substr(0, 400).c_str());
                std::printf("         in n=%016llX m=%016llX d=%016llX (n,m,d = d%u, d%u, d%u) %s\n", static_cast<unsigned long long>(in.d[op->n & 31]), static_cast<unsigned long long>(in.d[op->m & 31]),
                            static_cast<unsigned long long>(in.d[op->d & 31]), op->n, op->m, op->d, op->q ? "(+1 each)" : "");
            }
        }
    }
    std::printf("neon own core and lane library vs unicorn: %u compared, own core differing %u, lane library differing %u, %u words unicorn runs that the decoder lacks, %u the decoder reads that unicorn refuses, %u not run, %zu distinct forms\n", compared,
                different, engine_different, undecoded, spurious, unrun, per_op.size());
    for (const auto& [k, v] : bad_op) std::printf("    differs: %s x%u\n", k.c_str(), v);
    CB_CHECK_EQ(different, 0u);
    CB_CHECK_EQ(engine_different, 0u);
    CB_CHECK_EQ(spurious, 0u);
    CB_CHECK_EQ(unrun, 0u);
    for (const auto& [k, v] : missing) std::printf("    undecoded: %s x%u e.g. %s\n", k.c_str(), v, sample[k].c_str());
    CB_CHECK_EQ(undecoded, 0u);
});

// The adapter and the own core on a program: d0-d31 are loaded from memory, one instruction runs, d0-d31 and the status register are stored; each core's stores are compared
// with Unicorn's result for the instruction from the same registers.
CB_TEST("engine.pcode_cpu_arm_neon.the_adapter_and_the_own_core_match_unicorn_on_programs", [] {
    uc::Unicorn unicorn;
    std::mt19937 rng(7161124);
    constexpr u32 kCode = 0x81000000, kData = 0x82000000, kStack = 0x83000000, kReturn = 0x100;
    const unsigned count = runs_from_env("CB_ARM_NEON_PROGRAMS", 3000);
    unsigned compared = 0, different[2] = {0, 0};
    std::map<std::string, unsigned> shown;
    for (unsigned n = 0; n < count; ++n) {
        const u32 w = random_neon_word(rng);
        const bool thumb = rng() % 2 != 0;
        if (thumb && (w >> 28) != 0xF && (w >> 28) != 0xE) continue;
        const std::optional<t2::NeonDp> op = t2::neon_dp(w, thumb);
        if (!op) continue;
        if ((op->op == t2::NeonOp::MovToScalar || op->op == t2::NeonOp::MovFromScalar || op->op == t2::NeonOp::DupCore) && (op->rt < 5 || op->rt > 12)) continue;
        if ((op->op == t2::NeonOp::Swp || op->op == t2::NeonOp::Trn || op->op == t2::NeonOp::Uzp || op->op == t2::NeonOp::Zip) && op->d == op->m) continue;
        if (op->op == t2::NeonOp::VMovPair && (op->rt < 5 || op->rt > 12 || op->idx < 5 || op->idx > 12)) continue;
        if (!thumb && (w >> 24) != 0xF2 && (w >> 24) != 0xF3 && (w >> 28) != 0xE) continue;
        uc::State in = random_state(rng);
        in.fpscr = 0;
        in.nzcv = 0;
        in.r[0] = kData;
        in.r[1] = kData + 128;
        in.r[2] = kData + 0x1000;
        in.r[3] = kData + 0x1080;
        in.r[12] = 0;
        std::uint8_t one[4];
        encode(w, thumb, one);
        uc::State want;
        std::string why;
        if (!uc::run_unicorn(unicorn, one, 4, thumb, in, want, why)) continue;
        Bytes code;
        const auto put32 = [&](u32 word) { for (unsigned k = 0; k < 4; ++k) code.push_back(static_cast<std::uint8_t>(word >> (8 * k))); };
        const auto put16 = [&](u32 half) { code.push_back(static_cast<std::uint8_t>(half)); code.push_back(static_cast<std::uint8_t>(half >> 8)); };
        const auto both = [&](u32 arm_word) { if (thumb) { put16(arm_word >> 16); put16(arm_word & 0xFFFF); } else put32(arm_word); };
        both(0xEC900B20u);   // vldmia r0, {d0-d15}
        both(0xECD10B20u);   // vldmia r1, {d16-d31}
        for (unsigned k = 0; k < 4; ++k) code.push_back(one[k]);
        both(0xEC820B20u);   // vstmia r2, {d0-d15}
        both(0xECC30B20u);   // vstmia r3, {d16-d31}
        both(0xEEF14A10u);   // vmrs r4, fpscr
        if (thumb) { put16(0xF8C2); put16(0x4100); put16(0x4770); }   // str r4, [r2, #256]; bx lr
        else { put32(0xE5824100u); put32(0xE12FFF1Eu); }
        Bytes data(0x4000), stack(0x1000);
        for (unsigned i = 0; i < 32; ++i)
            for (unsigned k = 0; k < 8; ++k) data[i * 8 + k] = static_cast<std::uint8_t>(in.d[i] >> (8 * k));
        const auto regions = std::make_shared<std::vector<emulate::paged_memory::Region>>();
        regions->push_back({kCode, code});
        regions->push_back({kData, data});
        regions->push_back({kStack, stack});
        ar::RunOptions options;
        options.a = {kData, kData + 128, kData + 0x1000, kData + 0x1080};
        for (unsigned k = 0; k < 8; ++k) options.s[k] = in.r[4 + k];
        options.sp = kStack + 0x800;
        options.return_address = kReturn;
        options.max_steps = 64;
        ++compared;
        for (int core = 0; core < 2; ++core) {
            ar::Bus bus(ar::System::Vita);
            bus.set_regions(regions);
            const ar::RunResult result = core == 1 ? engine::pcode_cpu::run(bus, kCode | (thumb ? 1u : 0u), options) : ar::run(bus, kCode | (thumb ? 1u : 0u), options);
            Bytes after = data;
            for (const auto& [address, value] : bus.memory().changes(1 << 20))
                if (address >= kData && address - kData < after.size()) after[address - kData] = value;
            uc::State got = in;
            for (unsigned i = 0; i < 32; ++i) {
                got.d[i] = 0;
                for (unsigned k = 0; k < 8; ++k) got.d[i] |= static_cast<u64>(after[0x1000 + i * 8 + k]) << (8 * k);
            }
            got.fpscr = after[0x1100] | after[0x1101] << 8 | after[0x1102] << 16 | static_cast<u32>(after[0x1103]) << 24;
            for (unsigned i = 4; i < 13; ++i) got.r[i] = result.r[i];
            got.r[4] = want.r[4];   // (the status register's copy, not compared as a register)
            uc::State expected = want;
            for (unsigned i = 0; i < 4; ++i) { got.r[i] = want.r[i]; }
            const std::string diff = result.end == ar::End::Return ? difference(got, expected, core == 0 && op->op >= t2::NeonOp::VFma ? 0xF0000000u : 0xFFFFFFFFu) : "did not return: " + result.exit();
            if (!diff.empty()) {
                ++different[core];
                if (++shown[std::format("{}{}", core, op->op == t2::NeonOp::Add ? "" : std::to_string(static_cast<unsigned>(op->op)))] <= 1)
                    std::printf("    DIFF %s %08X thumb=%d: %s\n", core ? "adapter" : "own core", w, thumb ? 1 : 0, diff.substr(0, 300).c_str());
            }
        }
    }
    std::printf("neon programs vs unicorn: %u compared, own core differing %u, adapter differing %u\n", compared, different[0], different[1]);
    CB_CHECK_EQ(different[0], 0u);
    CB_CHECK_EQ(different[1], 0u);
});

}  // namespace
}  // namespace cb
