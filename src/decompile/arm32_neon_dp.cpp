// cb-unity: alone
// decompile/arm32_neon_dp.cpp -- see arm32_neon_dp.hpp. The tables are the ARM ARM's A7.4.1 to A7.4.5 (the groups by the U, A, B and C fields) and the encodings of A8.8.
#include "decompile/arm32_neon_dp.hpp"

namespace cb::decompile::arm::t2 {

namespace {

constexpr std::uint32_t bits(std::uint32_t w, unsigned lsb, unsigned width) { return (w >> lsb) & ((1u << width) - 1u); }

using Op = NeonOp;

NeonDp base(std::uint32_t w) {
    NeonDp r;
    r.d = bits(w, 22, 1) << 4 | bits(w, 12, 4);
    r.n = bits(w, 7, 1) << 4 | bits(w, 16, 4);
    r.m = bits(w, 5, 1) << 4 | bits(w, 0, 4);
    r.u = bits(w, 24, 1) != 0;
    return r;
}

// Three registers of the same length.
std::optional<NeonDp> three_same(std::uint32_t w) {
    NeonDp r = base(w);
    r.q = bits(w, 6, 1) != 0;
    r.size = bits(w, 20, 2);
    const unsigned opc = bits(w, 8, 4), b4 = bits(w, 4, 1), s = r.size;
    const bool u = r.u;
    if (r.q && ((r.d | r.n | r.m) & 1)) return std::nullopt;
    const auto set = [&](Op op) { r.op = op; return r; };
    const auto not64 = [&] { return s != 3; };
    if (opc >= 12 && opc != 12) {   // floating point: sz (bit 20) is 0 for single precision, bit 21 picks the second form
        if (bits(w, 20, 1) != 0) return std::nullopt;
        r.f = true;
        r.size = 2;
        const unsigned alt = bits(w, 21, 1);
        switch (opc) {
            case 13:
                if (b4 == 0) {
                    if (!u) return set(alt ? Op::FSub : Op::FAdd);
                    if (r.q && !alt) return std::nullopt;   // (VPADD is a doubleword instruction)
                    if (!alt) { if (r.q) return std::nullopt; return set(Op::FPadd); }
                    return set(Op::FAbd);
                }
                if (!u) return set(alt ? Op::FMls : Op::FMla);
                return alt ? std::nullopt : std::optional<NeonDp>(set(Op::FMul));
            case 14:
                if (b4 == 0) {
                    if (!u) return alt ? std::nullopt : std::optional<NeonDp>(set(Op::FCeq));
                    return set(alt ? Op::FCgt : Op::FCge);
                }
                if (!u) return std::nullopt;
                return set(alt ? Op::FAcgt : Op::FAcge);
            case 15:
                if (b4 == 0) {
                    if (!u) return set(alt ? Op::FMin : Op::FMax);
                    if (r.q) return std::nullopt;
                    return set(alt ? Op::FPmin : Op::FPmax);
                }
                if (u) return std::nullopt;
                return set(alt ? Op::Rsqrts : Op::Recps);
            default: return std::nullopt;
        }
    }
    switch (opc) {
        case 0: if (b4 == 0) { if (!not64()) return std::nullopt; r.sat = false; return set(Op::Hadd); } r.sat = true; return set(Op::Qadd);
        case 1:
            if (b4 == 0) return not64() ? std::optional<NeonDp>(set(Op::Rhadd)) : std::nullopt;
            if (!u) return set(s == 0 ? Op::And : s == 1 ? Op::Bic : s == 2 ? Op::Orr : Op::Orn);
            return set(s == 0 ? Op::Eor : s == 1 ? Op::Bsl : s == 2 ? Op::Bit : Op::Bif);
        case 2: if (b4 == 0) return not64() ? std::optional<NeonDp>(set(Op::Hsub)) : std::nullopt; return set(Op::Qsub);
        case 3: if (!not64()) return std::nullopt; return set(b4 ? Op::Cge : Op::Cgt);
        case 4: r.sat = b4 != 0; r.round = false; return set(Op::ShiftReg);
        case 5: r.sat = b4 != 0; r.round = true; return set(Op::ShiftReg);
        case 6: if (!not64()) return std::nullopt; return set(b4 ? Op::Min : Op::Max);
        case 7: if (!not64()) return std::nullopt; return set(b4 ? Op::Aba : Op::Abd);
        case 8:
            if (b4 == 0) return set(u ? Op::Sub : Op::Add);
            if (!not64()) return std::nullopt;
            return set(u ? Op::Ceq : Op::Tst);
        case 9:
            if (!not64()) return std::nullopt;
            if (b4 == 0) return set(u ? Op::Mls : Op::Mla);
            if (u) return s == 0 ? std::optional<NeonDp>(set(Op::MulP)) : std::nullopt;
            return set(Op::Mul);
        case 10: if (!not64() || r.q) return std::nullopt; return set(b4 ? Op::Pmin : Op::Pmax);
        case 11:
            if (b4 == 0) {
                if (s != 1 && s != 2) return std::nullopt;
                r.round = u;
                r.u = false;
                r.sat = true;
                return set(Op::Qdmulh);
            }
            if (u || !not64() || r.q) return std::nullopt;
            return set(Op::Padd);
        case 12:   // VFMA, VFMS (VFPv4)
            if (b4 == 0 || u || bits(w, 20, 1) != 0) return std::nullopt;
            r.f = true;
            r.size = 2;
            return set(bits(w, 21, 1) ? Op::Fms : Op::Fma);
        default: return std::nullopt;
    }
}

// Three registers of different lengths (bit 6 is 0) or two registers and a scalar (bit 6 is 1), size not 11.
std::optional<NeonDp> long_or_scalar(std::uint32_t w) {
    NeonDp r = base(w);
    r.size = bits(w, 20, 2);
    const unsigned opc = bits(w, 8, 4);
    const bool u = r.u;
    const auto set = [&](Op op) { r.op = op; return r; };
    if (bits(w, 6, 1) == 0) {
        r.q = false;
        const unsigned s = r.size;
        const bool d_even = (r.d & 1) == 0;
        switch (opc) {
            case 0: case 2: if (!d_even) return std::nullopt; return set(opc == 0 ? Op::Addl : Op::Subl);
            case 1: case 3: if (!d_even || (r.n & 1)) return std::nullopt; return set(opc == 1 ? Op::Addw : Op::Subw);
            case 4: case 6: if ((r.n | r.m) & 1) return std::nullopt; r.round = u; return set(opc == 4 ? Op::AddHn : Op::SubHn);
            case 5: if (!d_even) return std::nullopt; return set(Op::Abal);
            case 7: if (!d_even) return std::nullopt; return set(Op::Abdl);
            case 8: if (!d_even) return std::nullopt; return set(Op::Mlal);
            case 10: if (!d_even) return std::nullopt; return set(Op::Mlsl);
            case 12: if (!d_even) return std::nullopt; return set(Op::Mull);
            case 9: case 11: case 13:
                if (u || !d_even || (s != 1 && s != 2)) return std::nullopt;
                r.sat = true;
                return set(opc == 9 ? Op::Qdmlal : opc == 11 ? Op::Qdmlsl : Op::Qdmull);
            case 14: if (u || s != 0 || !d_even) return std::nullopt; return set(Op::MullP);
            default: return std::nullopt;
        }
    }
    // two registers and a scalar
    r.scalar = true;
    const unsigned s = r.size;
    if (s == 0) return std::nullopt;
    if (s == 1) {
        r.idx = bits(w, 5, 1) << 1 | bits(w, 3, 1);
        r.m = bits(w, 0, 3);
    } else {
        r.idx = bits(w, 5, 1);
        r.m = bits(w, 0, 4);
    }
    const bool longform = opc == 2 || opc == 6 || opc == 3 || opc == 7 || opc == 10 || opc == 11;
    r.q = longform ? false : bits(w, 24, 1) != 0;
    if (longform && (r.d & 1)) return std::nullopt;
    if (r.q && ((r.d | r.n) & 1)) return std::nullopt;
    switch (opc) {
        case 0: case 4: case 8: r.f = false; break;
        case 1: case 5: case 9: r.f = true; if (s != 2) return std::nullopt; r.size = 2; break;
        default: break;
    }
    switch (opc) {
        case 0: return set(Op::Mla);
        case 1: return set(Op::FMla);
        case 4: return set(Op::Mls);
        case 5: return set(Op::FMls);
        case 8: return set(Op::Mul);
        case 9: return set(Op::FMul);
        case 2: return set(Op::Mlal);
        case 6: return set(Op::Mlsl);
        case 10: return set(Op::Mull);
        case 3: case 7: case 11:
            if (u || (s != 1 && s != 2)) return std::nullopt;
            r.sat = true;
            return set(opc == 3 ? Op::Qdmlal : opc == 7 ? Op::Qdmlsl : Op::Qdmull);
        case 12: case 13:
            if (s != 1 && s != 2) return std::nullopt;
            r.sat = true;
            r.round = opc == 13;
            r.u = false;
            r.q = bits(w, 24, 1) != 0;
            if (r.q && ((r.d | r.n) & 1)) return std::nullopt;
            return set(Op::Qdmulh);
        default: return std::nullopt;
    }
}

// Two registers and a shift amount (not the modified immediates).
std::optional<NeonDp> shift_group(std::uint32_t w) {
    NeonDp r = base(w);
    r.q = bits(w, 6, 1) != 0;
    const unsigned imm6 = bits(w, 16, 6), l = bits(w, 7, 1), opc = bits(w, 8, 4);
    const bool u = r.u, b6 = r.q;
    const auto set = [&](Op op) { r.op = op; return r; };
    // the element size and the shift count of the forms that shift by a count
    unsigned esize_log = 0, right = 0, left = 0;   // esize as log2 of bytes
    if (l) { esize_log = 3; right = 64 - imm6; left = imm6; }
    else if (imm6 >> 3 == 1) { esize_log = 0; right = 16 - imm6; left = imm6 - 8; }
    else if (imm6 >> 4 == 1) { esize_log = 1; right = 32 - imm6; left = imm6 - 16; }
    else if (imm6 >> 5 == 1) { esize_log = 2; right = 64 - imm6; left = imm6 - 32; }
    else return std::nullopt;
    const auto qcheck = [&] { return !(r.q && ((r.d | r.m) & 1)); };
    switch (opc) {
        case 0: case 1: case 2: case 3:   // VSHR, VSRA, VRSHR, VRSRA
            if (!qcheck()) return std::nullopt;
            r.size = esize_log;
            r.imm = right;
            r.round = opc >= 2 && (opc & 2);
            r.acc = (opc & 1) != 0;
            return set(Op::Shr);
        case 4:   // VSRI
            if (!u || !qcheck()) return std::nullopt;
            r.size = esize_log; r.imm = right;
            return set(Op::Sri);
        case 5:   // VSHL, VSLI
            if (!qcheck()) return std::nullopt;
            r.size = esize_log; r.imm = left;
            return set(u ? Op::Sli : Op::ShlI);
        case 6:   // VQSHLU
            if (!u || !qcheck()) return std::nullopt;
            r.size = esize_log; r.imm = left; r.sat = true; r.u = false;
            return set(Op::Qshlu);
        case 7:   // VQSHL
            if (!qcheck()) return std::nullopt;
            r.size = esize_log; r.imm = left; r.sat = true;
            return set(Op::QshlI);
        case 8: case 9: {   // VSHRN, VRSHRN, VQSHRUN, VQRSHRUN, VQSHRN, VQRSHRN
            if (l || r.m & 1) return std::nullopt;
            r.q = false;
            r.size = esize_log;   // the destination's element; the source is twice as wide
            r.imm = right;
            r.round = b6;
            if (opc == 8) { r.sat = u; return set(u ? Op::Qshrun : Op::ShrN); }
            if (esize_log == 3) return std::nullopt;
            r.sat = true;
            return set(Op::Qshrn);
        }
        case 10: {   // VSHLL, VMOVL
            if (l || b6 || (r.d & 1)) return std::nullopt;
            r.q = false;
            r.size = esize_log;
            if (esize_log == 3) return std::nullopt;
            r.imm = left;
            return set(Op::Shll);
        }
        case 14: case 15: {   // VCVT between floating point and fixed point (32 bit)
            if (l || (imm6 >> 5) != 1 || !qcheck()) return std::nullopt;
            r.f = true;
            r.size = 2;
            r.imm = 64 - imm6;
            r.acc = opc == 15;   // to fixed
            return set(Op::CvtFixed);
        }
        default: return std::nullopt;
    }
}

std::optional<NeonDp> misc_group(std::uint32_t w) {
    NeonDp r = base(w);
    r.q = bits(w, 6, 1) != 0;
    r.size = bits(w, 18, 2);
    r.u = false;   // (bit 24 is 1 for the whole group: it is no signedness)
    const unsigned opc1 = bits(w, 16, 2), opc2 = bits(w, 7, 4), s = r.size;
    const auto set = [&](Op op) { r.op = op; return r; };
    const bool changes_width = opc1 == 2 && (opc2 == 4 || opc2 == 5 || opc2 == 6 || opc2 == 12 || opc2 == 14);   // (bit 6 is part of the operation there)
    if (!changes_width && r.q && ((r.d | r.m) & 1)) return std::nullopt;
    switch (opc1) {
        case 0:
            switch (opc2) {
                case 0: case 1: case 2: {
                    const unsigned group = 8u >> opc2;   // 8, 4, 2 bytes
                    if (s + opc2 >= 3) return std::nullopt;
                    r.imm = group;
                    return set(Op::Rev);
                }
                case 4: case 5: if (s == 3) return std::nullopt; r.u = opc2 & 1; return set(Op::Paddl);
                case 12: case 13: if (s == 3) return std::nullopt; r.u = opc2 & 1; return set(Op::Padal);
                case 8: if (s == 3) return std::nullopt; return set(Op::Cls);
                case 9: if (s == 3) return std::nullopt; return set(Op::Clz);
                case 10: if (s != 0) return std::nullopt; return set(Op::Cnt);
                case 11: if (s != 0) return std::nullopt; return set(Op::Mvn);
                case 14: if (s == 3) return std::nullopt; r.sat = true; return set(Op::Qabs);
                case 15: if (s == 3) return std::nullopt; r.sat = true; return set(Op::Qneg);
                default: return std::nullopt;
            }
        case 1: {
            const bool fl = opc2 >> 3 != 0;
            const unsigned op = opc2 & 7;
            if (fl && s != 2) return std::nullopt;
            if (!fl && s == 3) return std::nullopt;
            r.f = fl;
            switch (op) {
                case 0: case 1: case 2: case 3: case 4: r.imm = op; return set(Op::CmpZero);   // gt, ge, eq, le, lt
                case 6: return set(Op::Abs);
                case 7: return set(Op::Neg);
                default: return std::nullopt;
            }
        }
        case 2:
            switch (opc2) {
                case 0: if (s != 0) return std::nullopt; return set(Op::Swp);
                case 1: if (s == 3) return std::nullopt; return set(Op::Trn);
                case 2: if (s == 3 || (!r.q && s == 2)) return std::nullopt; return set(Op::Uzp);
                case 3: if (s == 3 || (!r.q && s == 2)) return std::nullopt; return set(Op::Zip);
                case 4: case 5: {   // VMOVN, VQMOVUN, VQMOVN: bits 7:6
                    if (s == 3 || (r.m & 1)) return std::nullopt;
                    const unsigned op = bits(w, 6, 2);
                    r.q = false;
                    r.size = s;   // the destination's element
                    if (op == 0) { r.sat = false; r.u = false; return set(Op::Movn); }
                    r.sat = true;
                    r.u = op == 3;
                    return set(op == 1 ? Op::Qmovun : Op::Qmovn);
                }
                case 6: if (bits(w, 6, 1) || s == 3 || (r.d & 1)) return std::nullopt; r.q = false; r.imm = 8u << s; return set(Op::Shll);   // VSHLL by the element size
                case 12: case 14:   // VCVT between half and single precision
                    if (s != 1 || bits(w, 6, 1)) return std::nullopt;
                    r.q = false;
                    r.acc = opc2 == 14;   // to single
                    if (r.acc ? (r.d & 1) : (r.m & 1)) return std::nullopt;
                    return set(Op::Cvt16);
                default: return std::nullopt;
            }
        case 3: {
            if ((opc2 >> 2) == 2) {   // VRECPE, VRSQRTE: 10Fx
                if (s != 2) return std::nullopt;
                r.f = (opc2 >> 1) & 1;
                return set((opc2 & 1) ? Op::Rsqrte : Op::Recpe);
            }
            if ((opc2 >> 2) == 3) {   // VCVT between integer and single precision
                if (s != 2) return std::nullopt;
                r.f = true;
                r.imm = opc2 & 3;   // 0: s32 to f32, 1: u32 to f32, 2: f32 to s32, 3: f32 to u32
                return set(Op::CvtIntFloat);
            }
            return std::nullopt;
        }
    }
    return std::nullopt;
}

}  // namespace

std::optional<NeonDp> neon_dp(std::uint32_t w, bool thumb) {
    const unsigned top = w >> 24;
    if (top == 0xF2 || top == 0xF3) {
        if (bits(w, 23, 1) == 0) return three_same(w);
        if (bits(w, 4, 1) == 1) {
            if (bits(w, 19, 3) == 0 && bits(w, 7, 1) == 0) return std::nullopt;   // a modified immediate (arm32_neon.cpp)
            return shift_group(w);
        }
        if (bits(w, 20, 2) != 3) return long_or_scalar(w);
        if (bits(w, 24, 1) == 0) {   // VEXT
            NeonDp r = base(w);
            r.op = Op::Ext;
            r.q = bits(w, 6, 1) != 0;
            r.imm = bits(w, 8, 4);
            if (r.q ? ((r.d | r.n | r.m) & 1) != 0 : r.imm > 7) return std::nullopt;
            return r;
        }
        if (bits(w, 11, 1) == 0) return misc_group(w);
        if (bits(w, 10, 2) == 2) {   // VTBL, VTBX
            NeonDp r = base(w);
            r.op = Op::Tbl;
            r.imm = bits(w, 8, 2);
            r.tbx = bits(w, 6, 1) != 0;
            if (r.n + r.imm > 31) return std::nullopt;
            return r;
        }
        if (bits(w, 7, 5) == 0x18) {   // VDUP (scalar)
            NeonDp r = base(w);
            r.op = Op::DupScalar;
            r.q = bits(w, 6, 1) != 0;
            const unsigned imm4 = bits(w, 16, 4);
            if (r.q && (r.d & 1)) return std::nullopt;
            if (imm4 & 1) { r.size = 0; r.idx = imm4 >> 1; }
            else if (imm4 & 2) { r.size = 1; r.idx = imm4 >> 2; }
            else if (imm4 & 4) { r.size = 2; r.idx = imm4 >> 3; }
            else return std::nullopt;
            return r;
        }
        return std::nullopt;
    }
    // the transfers between the core registers and the scalars (cond 1110 in Thumb, any condition but 1111 in ARM)
    const unsigned cond = w >> 28;
    if (thumb ? cond == 14 : cond != 15) {   // the VFP forms
        NeonDp v;
        v.cond = cond;
        const bool dbl = bits(w, 8, 1) != 0;
        const auto single_or_double = [&](unsigned field, unsigned bit) { return dbl ? (bit << 4 | field) : (field << 1 | bit); };
        v.size = dbl ? 3 : 2;
        v.d = single_or_double(bits(w, 12, 4), bits(w, 22, 1));
        v.n = single_or_double(bits(w, 16, 4), bits(w, 7, 1));
        v.m = single_or_double(bits(w, 0, 4), bits(w, 5, 1));
        if ((w & 0x0F800E10u) == 0x0E800A00u && (bits(w, 20, 2) == 2 || bits(w, 20, 2) == 1)) {   // VFMA, VFMS, VFNMA, VFNMS
            if (!dbl && false) return std::nullopt;
            v.op = Op::VFma;
            v.imm = bits(w, 20, 2) == 2 ? bits(w, 6, 1) : (bits(w, 6, 1) ? 2 : 3);
            return v;
        }
        if ((w & 0x0FBA0E50u) == 0x0EBA0A40u) {   // VCVT between floating point and fixed point
            v.op = Op::VCvtFx;
            v.acc = bits(w, 18, 1) != 0;
            v.u = bits(w, 16, 1) != 0;
            v.idx = bits(w, 7, 1) ? 32 : 16;
            const unsigned imm5 = bits(w, 0, 4) << 1 | bits(w, 5, 1);
            if (imm5 > v.idx) return std::nullopt;
            v.imm = v.idx - imm5;
            return v;
        }
        if ((w & 0x0FBE0F50u) == 0x0EB20A40u) {   // VCVTB, VCVTT between half and single precision
            v.op = Op::VCvtHalf;
            v.size = 2;
            v.d = single_or_double(bits(w, 12, 4), bits(w, 22, 1));
            v.d = bits(w, 12, 4) << 1 | bits(w, 22, 1);
            v.m = bits(w, 0, 4) << 1 | bits(w, 5, 1);
            v.acc = bits(w, 16, 1) != 0;
            v.imm = bits(w, 7, 1);
            return v;
        }
        if ((w & 0x0FE00FD0u) == 0x0C400A10u) {   // VMOV between two core registers and two singles
            v.op = Op::VMovPair;
            v.u = bits(w, 20, 1) != 0;
            v.rt = bits(w, 12, 4);
            v.idx = bits(w, 16, 4);
            v.m = bits(w, 0, 4) << 1 | bits(w, 5, 1);
            if (v.rt == 15 || v.idx == 15 || v.m == 31 || (v.u && v.rt == v.idx) || (thumb && (v.rt == 13 || v.idx == 13))) return std::nullopt;
            return v;
        }
    }
    if (bits(w, 24, 4) != 0xE || bits(w, 8, 4) != 0xB || bits(w, 4, 1) != 1 || bits(w, 0, 4) != 0) return std::nullopt;
    if (thumb ? cond != 14 : cond == 15) return std::nullopt;
    NeonDp r = base(w);
    r.cond = cond;
    r.rt = bits(w, 12, 4);
    if (r.rt == 15 || (thumb && r.rt == 13)) return std::nullopt;
    if (bits(w, 20, 1) == 0 && bits(w, 23, 1) == 0) {   // VMOV (core register to scalar)
        const unsigned opc = bits(w, 21, 2) << 2 | bits(w, 5, 2);
        r.d = bits(w, 7, 1) << 4 | bits(w, 16, 4);
        r.op = Op::MovToScalar;
        if (opc & 8) { r.size = 0; r.idx = opc & 7; }
        else if (opc & 1) { r.size = 1; r.idx = opc >> 1; }
        else if (!(opc & 2)) { r.size = 2; r.idx = opc >> 2; }
        else return std::nullopt;
        return r;
    }
    if (bits(w, 20, 1) == 1) {   // VMOV (scalar to core register)
        const unsigned opc = bits(w, 21, 2) << 2 | bits(w, 5, 2);
        r.m = bits(w, 7, 1) << 4 | bits(w, 16, 4);
        r.op = Op::MovFromScalar;
        r.u = bits(w, 23, 1) != 0;
        if (opc & 8) { r.size = 0; r.idx = opc & 7; }
        else if (opc & 1) { r.size = 1; r.idx = opc >> 1; }
        else if (!(opc & 2)) { r.size = 2; r.idx = opc >> 2; if (r.u) return std::nullopt; }
        else return std::nullopt;
        return r;
    }
    if (bits(w, 23, 1) == 1 && bits(w, 20, 1) == 0 && bits(w, 6, 1) == 0) {   // VDUP (core register)
        r.op = Op::DupCore;
        r.d = bits(w, 7, 1) << 4 | bits(w, 16, 4);
        r.q = bits(w, 21, 1) != 0;
        const unsigned be = bits(w, 22, 1) << 1 | bits(w, 5, 1);
        if (be == 2) r.size = 0;
        else if (be == 1) r.size = 1;
        else if (be == 0) r.size = 2;
        else return std::nullopt;
        if (r.q && (r.d & 1)) return std::nullopt;
        return r;
    }
    return std::nullopt;
}

}  // namespace cb::decompile::arm::t2
