// cb-unity: alone (file-local names that other files in a batch also use)
// emulate/cpu/pcode_ops_ppc.cpp -- see pcode_ops_ppc.hpp.
#include "emulate/cpu/pcode_ops_ppc.hpp"

#include "emulate/powerpc_fpu.hpp"

#include <algorithm>
#include <bit>
#include <cmath>
#include <cstring>
#include <limits>
#include <string>
#include <stdexcept>
#include <string_view>
#include <unordered_map>
#include <unordered_set>

namespace cb::emulate::pcode_ops_ppc {

namespace fpu = cb::emulate::powerpc::fpu;

namespace {

constexpr std::uint64_t kSign = std::uint64_t{1} << 63;

double as_double(std::uint64_t bits) { return std::bit_cast<double>(bits); }
std::uint64_t as_bits(double x) { return std::bit_cast<std::uint64_t>(x); }
double to_single(double x) { return static_cast<double>(static_cast<float>(x)); }   // a double rounded to single precision, held as a double

std::int64_t sext(std::uint64_t v, unsigned bits) {
    const unsigned shift = 64 - bits;
    return static_cast<std::int64_t>(v << shift) >> shift;
}

// ---- the quantization of a paired-single load or store (the GQRs) --------------------------------------------------------------------------------------------------

enum Quant : unsigned { kFloat = 0, kU8 = 4, kU16 = 5, kS8 = 6, kS16 = 7 };   // (types 1 to 3 are reserved and read as float)

struct Quantization {
    unsigned type = kFloat;
    int scale = 0;
    bool integer() const { return type >= kU8; }
    unsigned size() const { return type == kU8 || type == kS8 ? 1 : type == kU16 || type == kS16 ? 2 : 4; }
    bool is_signed() const { return type == kS8 || type == kS16; }
};

int scale_of(std::uint32_t six_bits) { return six_bits >= 32 ? static_cast<int>(six_bits) - 64 : static_cast<int>(six_bits); }

Quantization for_load(const pw::Machine& m, unsigned i) {
    const std::uint32_t q = m.gqr[i & 7];   // LD_TYPE bits 16-18, LD_SCALE bits 24-29
    return {(q >> 16) & 7, scale_of((q >> 24) & 63)};
}

Quantization for_store(const pw::Machine& m, unsigned i) {
    const std::uint32_t q = m.gqr[i & 7];   // ST_TYPE bits 0-2, ST_SCALE bits 8-13
    return {q & 7, scale_of((q >> 8) & 63)};
}

double load_value(Emu& emu, std::uint32_t ea, const Quantization& q) {
    const std::uint64_t raw = emu.busRead(ea, q.size());
    if (!q.integer()) return static_cast<double>(std::bit_cast<float>(static_cast<std::uint32_t>(raw)));
    const double v = q.is_signed() ? static_cast<double>(sext(raw, 8 * q.size())) : static_cast<double>(raw);
    return std::ldexp(v, -q.scale);
}

void store_value(Emu& emu, std::uint32_t ea, double value, const Quantization& q) {
    if (!q.integer()) {
        emu.busWrite(ea, 4, std::bit_cast<std::uint32_t>(static_cast<float>(value)));
        return;
    }
    const unsigned bits = 8 * q.size();
    const double lo = q.is_signed() ? -std::ldexp(1.0, static_cast<int>(bits) - 1) : 0.0;
    const double hi = q.is_signed() ? std::ldexp(1.0, static_cast<int>(bits) - 1) - 1 : std::ldexp(1.0, static_cast<int>(bits)) - 1;
    double v = std::ldexp(value, q.scale);
    v = std::isnan(v) ? 0.0 : std::trunc(v);   // (toward zero, a NaN stores zero)
    v = std::clamp(v, lo, hi);
    emu.busWrite(ea, q.size(), static_cast<std::uint64_t>(static_cast<std::int64_t>(v)) & ((std::uint64_t{1} << bits) - 1));
}

// ---- the paired-single arithmetic ---------------------------------------------------------------------------------------------------------------------------------------

}  // namespace

void paired_single(PairedState& s, std::uint32_t w) {
    Emu& emu = s.emu;
    const Regs& R = s.regs;
    pw::Machine& m = s.machine;
    const unsigned op = w >> 26;
    const unsigned frt = (w >> 21) & 31, ra = (w >> 16) & 31, rb = (w >> 11) & 31, frc = (w >> 6) & 31;
    const auto gpr = [&](unsigned i) { return emu.reg(R.gpr[i]); };
    const auto f0 = [&](unsigned i) { return emu.reg(R.fpr[i]); };   // the first half of a paired-single register: the floating register
    const auto set0 = [&](unsigned i, std::uint64_t bits) { emu.setReg(R.fpr[i], bits); };
    auto& second = m.ps1;   // the second half

    // psq_l, psq_lu, psq_st, psq_stu (a 12-bit displacement) and the indexed forms of opcode 4: W (one value only), I (which quantization register)
    const auto quantized = [&](bool store, bool update, std::uint32_t ea, bool one, unsigned index) {
        if (store) {
            const Quantization q = for_store(m, index);
            store_value(emu, ea, as_double(f0(frt)), q);
            if (!one) store_value(emu, ea + q.size(), as_double(second[frt]), q);
        } else {
            const Quantization q = for_load(m, index);
            set0(frt, as_bits(load_value(emu, ea, q)));   // (a fault of the second read leaves the first half loaded, as the interpreter does)
            second[frt] = as_bits(one ? 1.0 : load_value(emu, ea + q.size(), q));
        }
        if (update) emu.setReg(R.gpr[ra], ea);
    };
    if (op != 4) {
        const std::uint32_t ea = static_cast<std::uint32_t>((ra ? gpr(ra) : 0) + static_cast<std::uint64_t>(sext(w & 0xFFF, 12)));
        quantized(op >= 60, op == 57 || op == 61, ea, ((w >> 15) & 1) != 0, (w >> 12) & 7);
        return;
    }
    const unsigned xo10 = (w >> 1) & 0x3FF, xo6 = (w >> 1) & 63, xo5 = (w >> 1) & 31;
    if (xo6 == 6 || xo6 == 7 || xo6 == 38 || xo6 == 39) {   // psq_lx, psq_stx, psq_lux, psq_stux
        const std::uint32_t ea = static_cast<std::uint32_t>((ra ? gpr(ra) : 0) + gpr(rb));
        quantized(xo6 == 7 || xo6 == 39, xo6 >= 38, ea, ((w >> 10) & 1) != 0, (w >> 7) & 7);
        return;
    }
    const std::uint64_t a0 = f0(ra), b0 = f0(rb), c0 = f0(frc);
    const std::uint64_t a1 = second[ra], b1 = second[rb], c1 = second[frc];
    const auto compare = [&](std::uint64_t x, std::uint64_t y, bool ordered) { emu.setReg(R.cr[frt >> 2], fpu::compare(m.fpscr, x, y, ordered)); };
    switch (xo10) {
        case 0: case 32: compare(a0, b0, xo10 == 32); return;                // ps_cmpu0, ps_cmpo0
        case 64: case 96: compare(a1, b1, xo10 == 96); return;               // ps_cmpu1, ps_cmpo1
        case 40: set0(frt, b0 ^ kSign); second[frt] = b1 ^ kSign; return;    // ps_neg
        case 72: set0(frt, b0); second[frt] = b1; return;                    // ps_mr
        case 136: set0(frt, b0 | kSign); second[frt] = b1 | kSign; return;   // ps_nabs
        case 264: set0(frt, b0 & ~kSign); second[frt] = b1 & ~kSign; return; // ps_abs
        case 528: set0(frt, a0); second[frt] = b0; return;                   // ps_merge00
        case 560: set0(frt, a0); second[frt] = b1; return;                   // ps_merge01
        case 592: set0(frt, a1); second[frt] = b0; return;                   // ps_merge10
        case 624: set0(frt, a1); second[frt] = b1; return;                   // ps_merge11
        case 1014: {   // dcbz_l: the 32-byte block of the locked cache is cleared
            const std::uint32_t ea = static_cast<std::uint32_t>((ra ? gpr(ra) : 0) + gpr(rb)) & ~31u;
            for (unsigned i = 0; i < 32; i += 8) emu.busWrite(ea + i, 8, 0);
            return;
        }
        default: break;
    }
    const auto pair = [&](fpu::Op what, std::uint64_t x0, std::uint64_t y0, std::uint64_t z0, std::uint64_t x1, std::uint64_t y1, std::uint64_t z1) {
        const fpu::Pair r = fpu::execute_paired(m.fpscr, what, x0, y0, z0, x1, y1, z1);
        if (r.first.written) set0(frt, r.first.bits);
        if (r.second.written) second[frt] = r.second.bits;
    };
    const auto as_single = [](std::uint64_t bits) { return as_bits(to_single(as_double(bits))); };   // (a value copied through unchanged, as a single)
    switch (xo5) {
        case 18: pair(fpu::Op::Div, a0, b0, 0, a1, b1, 0); return;                  // ps_div
        case 20: pair(fpu::Op::Sub, a0, b0, 0, a1, b1, 0); return;                  // ps_sub
        case 21: pair(fpu::Op::Add, a0, b0, 0, a1, b1, 0); return;                  // ps_add
        case 23: set0(frt, as_double(a0) >= 0.0 ? c0 : b0); second[frt] = as_double(a1) >= 0.0 ? c1 : b1; return;   // ps_sel
        case 24: pair(fpu::Op::Res, 0, b0, 0, 0, b1, 0); return;                    // ps_res
        case 25: pair(fpu::Op::Mul, a0, 0, c0, a1, 0, c1); return;                  // ps_mul
        case 26: pair(fpu::Op::Rsqrte, 0, b0, 0, 0, b1, 0); return;                 // ps_rsqrte
        case 28: pair(fpu::Op::Msub, a0, b0, c0, a1, b1, c1); return;               // ps_msub
        case 29: pair(fpu::Op::Madd, a0, b0, c0, a1, b1, c1); return;               // ps_madd
        case 30: pair(fpu::Op::Nmsub, a0, b0, c0, a1, b1, c1); return;              // ps_nmsub
        case 31: pair(fpu::Op::Nmadd, a0, b0, c0, a1, b1, c1); return;              // ps_nmadd
        case 10: {                                                                  // ps_sum0: ps0 = a0 + b1, ps1 = c1
            const fpu::Result r = fpu::execute(m.fpscr, fpu::Op::Add, true, a0, b1, 0);
            if (r.written) set0(frt, r.bits), second[frt] = as_single(c1);
            return;
        }
        case 11: {                                                                  // ps_sum1: ps0 = c0, ps1 = a0 + b1
            const fpu::Result r = fpu::execute(m.fpscr, fpu::Op::Add, true, a0, b1, 0);
            if (r.written) set0(frt, as_single(c0)), second[frt] = r.bits;
            return;
        }
        case 12: pair(fpu::Op::Mul, a0, 0, c0, a1, 0, c0); return;                  // ps_muls0
        case 13: pair(fpu::Op::Mul, a0, 0, c1, a1, 0, c1); return;                  // ps_muls1
        case 14: pair(fpu::Op::Madd, a0, b0, c0, a1, b1, c0); return;               // ps_madds0
        case 15: pair(fpu::Op::Madd, a0, b0, c1, a1, b1, c1); return;               // ps_madds1
        default: throw pw::Fault(pw::StopReason::Unsupported);
    }
}

Regs find_regs(const Emu& emu) {
    static const char* const kGpr[32] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15",
                                         "r16", "r17", "r18", "r19", "r20", "r21", "r22", "r23", "r24", "r25", "r26", "r27", "r28", "r29", "r30", "r31"};
    static const char* const kFpr[32] = {"f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15",
                                         "f16", "f17", "f18", "f19", "f20", "f21", "f22", "f23", "f24", "f25", "f26", "f27", "f28", "f29", "f30", "f31"};
    static const char* const kCr[8] = {"cr0", "cr1", "cr2", "cr3", "cr4", "cr5", "cr6", "cr7"};
    static const char* const kVr[32] = {"vs32", "vs33", "vs34", "vs35", "vs36", "vs37", "vs38", "vs39", "vs40", "vs41", "vs42", "vs43", "vs44", "vs45", "vs46", "vs47",
                                        "vs48", "vs49", "vs50", "vs51", "vs52", "vs53", "vs54", "vs55", "vs56", "vs57", "vs58", "vs59", "vs60", "vs61", "vs62", "vs63"};
    Regs r;
    for (unsigned i = 0; i < 32; ++i) {
        r.gpr[i] = emu.regRef(kGpr[i]);
        r.fpr[i] = emu.regRef(kFpr[i]);
        r.vr[i] = emu.regRef(kVr[i]);
    }
    for (unsigned i = 0; i < 8; ++i) r.cr[i] = emu.regRef(kCr[i]);
    r.lr = emu.regRef("LR");
    r.ctr = emu.regRef("CTR");
    r.ca = emu.regRef("xer_ca");
    r.so = emu.regRef("xer_so");
    r.ov = emu.regRef("xer_ov");
    r.ov32 = emu.regRef("xer_ov32");
    r.ca32 = emu.regRef("xer_ca32");
    r.count = emu.regRef("xer_count");
    r.xer = emu.regRef("XER");
    r.reserve = emu.regRef("RESERVE");
    r.reserve_address = emu.regRef("RESERVE_ADDRESS");
    r.reserve_length = emu.regRef("RESERVE_LENGTH");
    r.msr = emu.regRef("MSR");
    r.vrsave = emu.regRef("spr100");   // VRSAVE is SPR 256
    for (unsigned i = 32; i < 128; ++i)
        if (emu.hasReg("v" + std::to_string(i))) r.vr_wide.push_back(emu.regRef("v" + std::to_string(i)));
    for (const char* name : {"fp_fx", "fp_fex", "fp_vx", "fp_ox", "fp_ux", "fp_zx", "fp_xx", "fp_vxsnan", "fp_vxisi", "fp_vxidi", "fp_vxzdz", "fp_vximz", "fp_vxvc", "fp_fr", "fp_fi", "fp_c",
                             "fp_cc0", "fp_cc1", "fp_cc2", "fp_cc3", "fp_reserve1", "fp_vxsoft", "fp_vxsqrt", "fp_vxcvi", "fp_ve", "fp_oe", "fp_ue", "fp_ze", "fp_xe", "fp_ni", "fp_rn0", "fp_rn1", "fp_drn"})
        if (emu.hasReg(name)) r.status.push_back(emu.regRef(name));
    return r;
}

pcode_ops::Binding bind(Emu& emu, Flavor flavor, VectorContext& context) {
    std::vector<pcode_ops::Entry> entries;
    add_vector_entries(entries, flavor);
    std::unordered_map<std::string_view, pcode_ops::Handler> served;
    for (const pcode_ops::Entry& e : entries)
        if (!served.emplace(e.name, e.handler).second) throw std::logic_error("pcode_ops_ppc: operation listed twice: " + std::string(e.name));
    pcode_ops::Binding result;
    std::unordered_set<std::string_view> seen;
    for (std::size_t i = 0; i < emu.userOpCount(); ++i) {
        const std::string& name = emu.userOpName(i);
        seen.insert(name);
        if (const auto it = served.find(name); it != served.end()) {
            emu.bindUserOp(i, it->second, &context);
            ++result.bound;
        } else {
            result.unbound.push_back(name);
        }
    }
    for (const pcode_ops::Entry& e : entries)
        if (!seen.count(e.name)) result.unknown.push_back(std::string(e.name));
    return result;
}

}  // namespace cb::emulate::pcode_ops_ppc
