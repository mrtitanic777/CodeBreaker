// cb-unity: alone (file-local names that other files in a batch also use)
// emulate/sh4.cpp -- the SH-4's instructions on the SH-2's interpreter (sh2.hpp): the FPU and the system instructions the SH-2 does not have.

#include <bit>
#include <cmath>
#include <cstring>
#include <limits>

#include "emulate/sh2.hpp"

namespace cb::emulate::sh2 {

namespace {

float as_float(std::uint32_t bits) { return std::bit_cast<float>(bits); }
std::uint32_t as_bits(float f) { return std::bit_cast<std::uint32_t>(f); }
double as_double(std::uint64_t bits) { return std::bit_cast<double>(bits); }
std::uint64_t as_bits(double d) { return std::bit_cast<std::uint64_t>(d); }

// FTRC: toward zero, saturating (a NaN gives the most negative value, as the hardware's invalid operation does).
std::uint32_t truncate_to_int(double v) {
    if (std::isnan(v)) return 0x80000000u;
    if (v >= 2147483648.0) return 0x7FFFFFFFu;
    if (v <= -2147483648.0) return 0x80000000u;
    return static_cast<std::uint32_t>(static_cast<std::int32_t>(v));
}

constexpr unsigned kCacheOpPref = 0, kCacheOpOcbi = 1, kCacheOpOcbp = 2, kCacheOpOcbwb = 3, kCacheOpMovca = 4, kCacheOpLdtlb = 5;

}  // namespace

Cpu::Sh4Outcome Cpu::execute_sh4(std::uint16_t w, std::uint32_t at, bool in_slot, Flow& flow) {
    const unsigned n = (w >> 8) & 15, m = (w >> 4) & 15;
    const Sh4Outcome ran{true, std::nullopt};
    const auto stopped = [](Fault f) { return Sh4Outcome{true, f}; };
    const auto fpu_off = [&]() -> std::optional<Fault> {
        if ((sr & sr::FD) != 0) return Fault{Fault::Kind::FpuDisabled, at, 0, 0};
        return std::nullopt;
    };
    std::uint32_t v = 0;

    if ((w >> 12) == 0xF) {
        if (auto f = fpu_off()) return stopped(*f);
        const std::optional<Fault> f = (w & 0xF) >= 6 && (w & 0xF) <= 0xC && (w & 0xF) != 0xD ? sh4_fmov(w, at) : sh4_fpu(w, at);
        return {true, f};
    }

    if ((w >> 12) == 0x0) {
        switch (w & 0xFF) {
            case 0x2B:   // RTE: PC from SPC, SR from SSR once the delay slot has run
                if (in_slot) return stopped(Fault{Fault::Kind::IllegalSlot, at, 0, 0});
                flow.taken = true;
                flow.delayed = true;
                flow.target = spc;
                flow.restore_sr = true;
                flow.new_sr = ssr;
                return ran;
            case 0x38: bus_->note(BusAccess::Kind::Cache, at, kCacheOpLdtlb); return ran;   // LDTLB: the TLB is state the log keeps
            case 0x48: sr &= ~sr::S; return ran;                                            // CLRS
            case 0x58: sr |= sr::S; return ran;                                             // SETS
            case 0x32: r[n] = ssr; return ran;
            case 0x42: r[n] = spc; return ran;
            case 0x3A: r[n] = sgr; return ran;
            case 0xFA: r[n] = dbr; return ran;
            case 0x5A: if (auto f = fpu_off()) return stopped(*f); r[n] = fpul; return ran;
            case 0x6A: if (auto f = fpu_off()) return stopped(*f); r[n] = fpscr; return ran;
            case 0x83:   // PREF @rn: a store queue address flushes its queue; anything else is a cache hint
                if (r[n] >= 0xE0000000u && r[n] < 0xE4000000u) bus_->flush_store_queue(r[n]);
                bus_->note(BusAccess::Kind::Cache, r[n], kCacheOpPref);
                return ran;
            case 0x93: bus_->note(BusAccess::Kind::Cache, r[n], kCacheOpOcbi); return ran;
            case 0xA3: bus_->note(BusAccess::Kind::Cache, r[n], kCacheOpOcbp); return ran;
            case 0xB3: bus_->note(BusAccess::Kind::Cache, r[n], kCacheOpOcbwb); return ran;
            case 0xC3:   // MOVCA.L r0,@rn: a longword store that does not fetch the line
                bus_->note(BusAccess::Kind::Cache, r[n], kCacheOpMovca);
                return {true, store(at, r[n], 4, r[0])};
            default: break;
        }
        if ((w & 0x8F) == 0x82) {   // STC Rm_BANK,Rn
            r[n] = bank[(w >> 4) & 7];
            return ran;
        }
        return {};
    }

    if ((w >> 12) == 0x4) {
        if ((w & 0xF) == 0xC || (w & 0xF) == 0xD) {   // SHAD (arithmetic) and SHLD (logical): the shift count is rm's low six bits, negative shifts right
            const std::uint32_t count = r[m];
            const bool arithmetic = (w & 0xF) == 0xC;
            if ((count & 0x80000000u) == 0) {
                r[n] <<= (count & 31);
            } else if ((count & 31) != 0) {
                const unsigned right = 32 - (count & 31);
                r[n] = arithmetic ? static_cast<std::uint32_t>(static_cast<std::int32_t>(r[n]) >> right) : r[n] >> right;
            } else {
                r[n] = arithmetic ? static_cast<std::uint32_t>(static_cast<std::int32_t>(r[n]) >> 31) : 0u;
            }
            return ran;
        }
        const auto push = [&](std::uint32_t value) -> std::optional<Fault> {
            if (auto f = store(at, r[n] - 4, 4, value)) return f;
            r[n] -= 4;
            return std::nullopt;
        };
        const auto pop = [&](std::uint32_t& value) -> std::optional<Fault> {
            if (auto f = load(at, r[n], 4, value)) return f;
            r[n] += 4;
            return std::nullopt;
        };
        switch (w & 0xFF) {
            case 0x0E: set_sr(r[n]); return ran;                                       // LDC Rn,SR (the SH-2's word, the SH-4's wider SR)
            case 0x07: if (auto f = pop(v)) return stopped(*f); set_sr(v); return ran;  // LDC.L @Rn+,SR
            case 0x33: return {true, push(ssr)};
            case 0x43: return {true, push(spc)};
            case 0x32: return {true, push(sgr)};
            case 0xF2: return {true, push(dbr)};
            case 0x37: if (auto f = pop(v)) return stopped(*f); ssr = v; return ran;
            case 0x47: if (auto f = pop(v)) return stopped(*f); spc = v; return ran;
            case 0xF6: if (auto f = pop(v)) return stopped(*f); dbr = v; return ran;
            case 0x3E: ssr = r[n]; return ran;
            case 0x4E: spc = r[n]; return ran;
            case 0xFA: dbr = r[n]; return ran;
            case 0x52: if (auto f = fpu_off()) return stopped(*f); return {true, push(fpul)};
            case 0x62: if (auto f = fpu_off()) return stopped(*f); return {true, push(fpscr)};
            case 0x56: if (auto f = fpu_off()) return stopped(*f); if (auto f = pop(v)) return stopped(*f); fpul = v; return ran;
            case 0x66: if (auto f = fpu_off()) return stopped(*f); if (auto f = pop(v)) return stopped(*f); fpscr = v & fpscr::kMask; return ran;
            case 0x5A: if (auto f = fpu_off()) return stopped(*f); fpul = r[n]; return ran;
            case 0x6A: if (auto f = fpu_off()) return stopped(*f); fpscr = r[n] & fpscr::kMask; return ran;
            default: break;
        }
        switch (w & 0x8F) {   // the banked registers: r0_bank .. r7_bank
            case 0x83: return {true, push(bank[(w >> 4) & 7])};
            case 0x87: if (auto f = pop(v)) return stopped(*f); bank[(w >> 4) & 7] = v; return ran;
            case 0x8E: bank[(w >> 4) & 7] = r[n]; return ran;
            default: break;
        }
        return {};
    }
    return {};
}

// FMOV: loads, stores and register moves of one float, or (SZ=1) of a pair. A pair is DRn (the current bank) or XDn (the other: bit 8 or bit 4 of
// the word picks it); its even register is the high word, so in little-endian memory the lower address goes to the odd register.
std::optional<Fault> Cpu::sh4_fmov(std::uint16_t w, std::uint32_t at) {
    const unsigned n = (w >> 8) & 15, m = (w >> 4) & 15;
    const bool pair = (fpscr & fpscr::SZ) != 0;
    const unsigned front = (fpscr & fpscr::FR) != 0 ? 1 : 0;
    const auto bank_of = [&](unsigned field) -> std::array<std::uint32_t, 16>& { return fp[pair && (field & 1) != 0 ? 1 - front : front]; };
    const auto first = [&](unsigned field) { return pair ? field & 14u : field; };

    const auto load_into = [&](unsigned field, std::uint32_t address) -> std::optional<Fault> {
        std::uint32_t lo = 0, hi = 0;
        if (auto f = load(at, address, 4, lo)) return f;
        if (!pair) {
            fp[front][field] = lo;
            return std::nullopt;
        }
        if (auto f = load(at, address + 4, 4, hi)) return f;
        bank_of(field)[first(field)] = hi;
        bank_of(field)[first(field) + 1] = lo;
        return std::nullopt;
    };
    const auto store_from = [&](unsigned field, std::uint32_t address) -> std::optional<Fault> {
        if (!pair) return store(at, address, 4, fp[front][field]);
        if (auto f = store(at, address, 4, bank_of(field)[first(field) + 1])) return f;
        return store(at, address + 4, 4, bank_of(field)[first(field)]);
    };
    const std::uint32_t step_bytes = pair ? 8 : 4;

    switch (w & 0xF) {
        case 0x6: return load_into(n, r[0] + r[m]);
        case 0x7: return store_from(m, r[0] + r[n]);
        case 0x8: return load_into(n, r[m]);
        case 0x9: {
            if (auto f = load_into(n, r[m])) return f;
            r[m] += step_bytes;
            return std::nullopt;
        }
        case 0xA: return store_from(m, r[n]);
        case 0xB: {
            if (auto f = store_from(m, r[n] - step_bytes)) return f;
            r[n] -= step_bytes;
            return std::nullopt;
        }
        case 0xC: {
            if (!pair) {
                fp[front][n] = fp[front][m];
            } else {
                const std::uint32_t hi = bank_of(m)[first(m)], lo = bank_of(m)[first(m) + 1];
                bank_of(n)[first(n)] = hi;
                bank_of(n)[first(n) + 1] = lo;
            }
            return std::nullopt;
        }
        default: return std::nullopt;
    }
}

std::optional<Fault> Cpu::sh4_fpu(std::uint16_t w, std::uint32_t at) {
    (void)at;
    const unsigned n = (w >> 8) & 15, m = (w >> 4) & 15;
    const bool dbl = (fpscr & fpscr::PR) != 0;
    std::array<std::uint32_t, 16>& f = fp[(fpscr & fpscr::FR) != 0 ? 1 : 0];
    const auto dr = [&](unsigned k) { return as_double(static_cast<std::uint64_t>(f[k & 14]) << 32 | f[(k & 14) + 1]); };
    const auto set_dr = [&](unsigned k, double d) {
        const std::uint64_t b = as_bits(d);
        f[k & 14] = static_cast<std::uint32_t>(b >> 32);
        f[(k & 14) + 1] = static_cast<std::uint32_t>(b);
    };
    const auto flt = [&](unsigned k) { return as_float(f[k]); };
    const auto set_flt = [&](unsigned k, float x) { f[k] = as_bits(x); };

    switch (w & 0xF) {
        case 0x0: case 0x1: case 0x2: case 0x3: {   // FADD FSUB FMUL FDIV
            const unsigned op = w & 0xF;
            if (!dbl) {
                const float a = flt(n), b = flt(m);
                set_flt(n, op == 0 ? a + b : op == 1 ? a - b : op == 2 ? a * b : a / b);
            } else {
                const double a = dr(n), b = dr(m);
                set_dr(n, op == 0 ? a + b : op == 1 ? a - b : op == 2 ? a * b : a / b);
            }
            return std::nullopt;
        }
        case 0x4: case 0x5: {   // FCMP/EQ FCMP/GT
            const bool eq = (w & 0xF) == 4;
            if (!dbl) set_t(eq ? flt(n) == flt(m) : flt(n) > flt(m));
            else set_t(eq ? dr(n) == dr(m) : dr(n) > dr(m));
            return std::nullopt;
        }
        case 0xE:   // FMAC FR0,FRm,FRn
            set_flt(n, std::fma(flt(0), flt(m), flt(n)));
            return std::nullopt;
        case 0xD: break;
        default: return std::nullopt;
    }

    switch (w & 0xFF) {
        case 0x0D: f[n] = fpul; return std::nullopt;                                    // FSTS FPUL,FRn
        case 0x1D: fpul = f[n]; return std::nullopt;                                    // FLDS FRm,FPUL
        case 0x2D:                                                                      // FLOAT FPUL,FRn / DRn
            if (!dbl) set_flt(n, static_cast<float>(static_cast<std::int32_t>(fpul)));
            else set_dr(n, static_cast<double>(static_cast<std::int32_t>(fpul)));
            return std::nullopt;
        case 0x3D: fpul = dbl ? truncate_to_int(dr(n)) : truncate_to_int(flt(n)); return std::nullopt;   // FTRC
        case 0x4D: if (!dbl) f[n] ^= 0x80000000u; else f[n & 14] ^= 0x80000000u; return std::nullopt;     // FNEG
        case 0x5D: if (!dbl) f[n] &= 0x7FFFFFFFu; else f[n & 14] &= 0x7FFFFFFFu; return std::nullopt;     // FABS
        case 0x6D:                                                                                       // FSQRT
            if (!dbl) set_flt(n, std::sqrt(flt(n)));
            else set_dr(n, std::sqrt(dr(n)));
            return std::nullopt;
        case 0x7D: set_flt(n, 1.0f / std::sqrt(flt(n))); return std::nullopt;           // FSRRA
        case 0x8D: f[n] = 0; return std::nullopt;                                       // FLDI0
        case 0x9D: f[n] = 0x3F800000u; return std::nullopt;                             // FLDI1
        case 0xAD: set_dr(n, static_cast<double>(as_float(fpul))); return std::nullopt;   // FCNVSD FPUL,DRn
        case 0xBD: fpul = as_bits(static_cast<float>(dr(n))); return std::nullopt;        // FCNVDS DRm,FPUL
        case 0xED: {   // FIPR FVm,FVn: the dot product, in FR[n+3]
            const unsigned vn = n & 0xC, vm = (n & 3) * 4;
            double sum = 0;
            for (unsigned i = 0; i < 4; ++i) sum += static_cast<double>(flt(vm + i)) * static_cast<double>(flt(vn + i));
            set_flt(vn + 3, static_cast<float>(sum));
            return std::nullopt;
        }
        case 0xFD: {
            if ((w & 0xF3FF) == 0xF1FD) {   // FTRV XMTRX,FVn: the vector times the matrix in the other bank
                const unsigned vn = n & 0xC;
                const std::array<std::uint32_t, 16>& back = fp[(fpscr & fpscr::FR) != 0 ? 0 : 1];
                std::array<float, 4> in{}, out{};
                for (unsigned i = 0; i < 4; ++i) in[i] = flt(vn + i);
                for (unsigned row = 0; row < 4; ++row) {
                    double sum = 0;
                    for (unsigned col = 0; col < 4; ++col) sum += static_cast<double>(as_float(back[row + 4 * col])) * static_cast<double>(in[col]);
                    out[row] = static_cast<float>(sum);
                }
                for (unsigned i = 0; i < 4; ++i) set_flt(vn + i, out[i]);
                return std::nullopt;
            }
            if ((w & 0xF1FF) == 0xF0FD) {   // FSCA FPUL,DRn: sine and cosine of the angle in 1/65536 turns
                const double angle = (fpul & 0xFFFF) * (2.0 * 3.14159265358979323846 / 65536.0);
                set_flt(n & 14, static_cast<float>(std::sin(angle)));
                set_flt((n & 14) + 1, static_cast<float>(std::cos(angle)));
                return std::nullopt;
            }
            if (w == 0xF3FD) fpscr ^= fpscr::SZ;   // FSCHG
            else if (w == 0xFBFD) fpscr ^= fpscr::FR;   // FRCHG
            return std::nullopt;
        }
        default: return std::nullopt;
    }
}

}  // namespace cb::emulate::sh2
