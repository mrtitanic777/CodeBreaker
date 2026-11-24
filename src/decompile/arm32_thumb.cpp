// cb-unity: alone
// decompile/arm32_thumb.cpp -- ARMv7-A Thumb: the 32-bit integer instructions the interpreter runs beyond what arm.cpp's decode_thumb32_int reads (arm_ext.hpp: t2::decode_more).
// Each is read from the ARM ARM's Thumb tables (A6.3): the loads and stores of one register (every size and form, the literal forms, the memory hints), of two registers,
// the exclusive ones and the table branches; ORN, PKH, ADR, SSAT and USAT; the multiplies; the extensions that accumulate; the parallel arithmetic, the saturating
// arithmetic, the byte reversals and SEL; the hints, the barriers, MRS and MSR. An encoding the manual leaves UNPREDICTABLE or undefined is not decoded (the interpreter
// stops there as not run, as before). Where an instruction is an ARM row of arm_ext in another dress (REV, SEL, the parallel add and subtract, the DSP multiplies), it is
// that row with the ARM word it means in `Insn::imm`, and the interpreter runs the row.

#include <bit>

#include "decompile/arm_ext.hpp"

namespace cb::decompile::arm::t2 {

namespace {

constexpr std::uint8_t kAlways = 14;

Insn base(std::uint16_t a, std::uint16_t b, std::uint32_t pc_tagged) {
    Insn i;
    i.word = static_cast<std::uint32_t>(a) | static_cast<std::uint32_t>(b) << 16;
    i.pc = pc_tagged & ~1u;
    i.isa = Isa::Thumb;
    i.length = 4;
    i.cond = kAlways;
    i.v6 = true;
    i.ext = kOpaque;
    for (unsigned k = 0; k < 4; ++k) i.raw[k] = static_cast<std::uint8_t>(i.word >> (8 * k));
    return i;
}

bool sp_or_pc(unsigned r) { return r == 13 || r == 15; }

// The instruction as the ARM row of the word `arm` (condition always).
std::optional<Insn> as_row(Insn i, std::uint32_t arm) {
    const std::uint32_t w = 0xE0000000u | arm;
    const int row = ext::find_arm(w, Arch::V7A);
    if (row < 0) return std::nullopt;
    i.op = Op::Ext;
    i.ext = static_cast<std::int16_t>(row);
    i.imm = w;
    return i;
}

// Load and store of one register: a = 1111 1000 S imm12 size L Rn.
std::optional<Insn> single_transfer(Insn i, unsigned a, unsigned b) {
    const bool sign = (a >> 8) & 1, imm12 = (a >> 7) & 1, load = (a >> 4) & 1;
    const unsigned size = (a >> 5) & 3, rn = a & 15, rt = b >> 12;
    if (size == 3 || (!load && sign) || (size == 2 && sign)) return std::nullopt;
    const bool hint = load && rt == 15 && size != 2;   // PLD, PLDW, PLI and the unallocated hints: nothing is read
    if (!load && (rt == 15 || (size != 2 && rt == 13))) return std::nullopt;
    if (load && rt == 13 && size != 2) return std::nullopt;
    i.op = hint ? Op::Pld : load ? Op::Ldr : Op::Str;
    i.size = static_cast<std::uint8_t>(size == 0 ? 1 : size == 1 ? 2 : 4);
    i.sign = sign;
    i.rd = static_cast<std::uint8_t>(rt);
    i.rn = static_cast<std::uint8_t>(rn);
    if (rn == 15) {   // the literal forms (a load only): Rn is the pc, the U bit takes the place of bit 7
        if (!load) return std::nullopt;
        i.imm_form = true;
        i.pre = true;
        i.up = imm12;
        i.imm = b & 0xFFF;
        return i;
    }
    if (imm12) {
        i.imm_form = true;
        i.pre = true;
        i.up = true;
        i.imm = b & 0xFFF;
    } else if ((b & 0x0800) != 0) {
        const bool p = (b >> 10) & 1, u = (b >> 9) & 1, w = (b >> 8) & 1;
        if (!p && !w) return std::nullopt;
        if (p && u && !w) {   // LDRT, STRT: the unprivileged access (there is no privilege to drop here: an ordinary access)
            if (rt == 15 || (rt == 13 && size != 2)) return std::nullopt;
            i.user = true;
        } else if (hint) {
            if (!(p && !u && !w)) return std::nullopt;
        } else if (w && (rn == rt)) {
            return std::nullopt;
        }
        i.imm_form = true;
        i.pre = p;
        i.up = u;
        i.wb = w;
        i.imm = b & 0xFF;
    } else if ((b & 0x0FC0) == 0) {
        i.imm_form = false;
        i.pre = true;
        i.up = true;
        i.rm = b & 15;
        i.shift = Shift::Lsl;
        i.amount = static_cast<std::uint8_t>((b >> 4) & 3);
        if (sp_or_pc(i.rm)) return std::nullopt;
    } else {
        return std::nullopt;
    }
    if (load && rt == 15 && size == 2) i.flow = Flow::Indirect;
    return i;
}

// Load and store dual or exclusive, table branch: a = 1110 100 P U 1 W L Rn.
std::optional<Insn> dual_exclusive(Insn i, unsigned a, unsigned b) {
    const bool p = (a >> 8) & 1, u = (a >> 7) & 1, w = (a >> 5) & 1, l = (a >> 4) & 1;
    const unsigned rn = a & 15, rt = b >> 12, rt2 = (b >> 8) & 15, imm8 = b & 0xFF;
    if (p || w) {   // LDRD, STRD
        if (!l && rn == 15) return std::nullopt;
        if (sp_or_pc(rt) || sp_or_pc(rt2) || (l && rt == rt2)) return std::nullopt;
        if (w && (rn == rt || rn == rt2)) return std::nullopt;
        if (l && rn == 15 && w) return std::nullopt;
        i.op = l ? Op::Ldr : Op::Str;
        i.size = 8;
        i.rd = static_cast<std::uint8_t>(rt);
        i.rs = static_cast<std::uint8_t>(rt2);
        i.rn = static_cast<std::uint8_t>(rn);
        i.imm_form = true;
        i.imm = imm8 * 4;
        i.pre = p;
        i.up = u;
        i.wb = w;
        return i;
    }
    if (!u) {   // STREX, LDREX
        if (rn == 15 || sp_or_pc(rt)) return std::nullopt;
        i.rn = static_cast<std::uint8_t>(rn);
        i.imm = imm8 * 4;
        if (!l) {
            if (sp_or_pc(rt2) || rt2 == rn || rt2 == rt) return std::nullopt;
            i.ext = kStrexT;
            i.rs = static_cast<std::uint8_t>(rt2);   // the status register
        } else {
            if (rt2 != 15) return std::nullopt;
            i.ext = kLdrexT;
        }
        i.op = Op::Ext;
        i.rd = static_cast<std::uint8_t>(rt);
        i.size = 4;
        return i;
    }
    const unsigned op3 = (b >> 4) & 15, rm = b & 15;
    if (!l) {   // STREXB, STREXH, STREXD
        if (op3 != 4 && op3 != 5 && op3 != 7) return std::nullopt;
        if (rn == 15 || sp_or_pc(rt) || sp_or_pc(rm) || rm == rn || rm == rt) return std::nullopt;
        if (op3 == 7 && (sp_or_pc(rt2) || rm == rt2)) return std::nullopt;
        if (op3 != 7 && rt2 != 15) return std::nullopt;
        i.op = Op::Ext;
        i.ext = op3 == 7 ? kStrexdT : kStrexT;
        i.size = static_cast<std::uint8_t>(op3 == 4 ? 1 : op3 == 5 ? 2 : 8);
        i.rn = static_cast<std::uint8_t>(rn);
        i.rd = static_cast<std::uint8_t>(rt);
        i.rs = static_cast<std::uint8_t>(rm);   // the status register
        i.rm = static_cast<std::uint8_t>(rt2);
        i.imm = 0;
        return i;
    }
    if (op3 == 0 || op3 == 1) {   // TBB, TBH
        if (rt != 15 || rt2 != 0 || (b & 0x00E0) != 0 || rn == 13 || sp_or_pc(rm)) return std::nullopt;
        i.op = Op::Ext;
        i.ext = kTb;
        i.size = static_cast<std::uint8_t>(op3 == 0 ? 1 : 2);
        i.rn = static_cast<std::uint8_t>(rn);
        i.rm = static_cast<std::uint8_t>(rm);
        i.flow = Flow::Indirect;
        return i;
    }
    if (op3 != 4 && op3 != 5 && op3 != 7) return std::nullopt;   // LDREXB, LDREXH, LDREXD
    if (rn == 15 || sp_or_pc(rt) || rm != 15) return std::nullopt;
    if (op3 == 7 && (sp_or_pc(rt2) || rt2 == rt)) return std::nullopt;
    if (op3 != 7 && rt2 != 15) return std::nullopt;
    i.op = Op::Ext;
    i.ext = op3 == 7 ? kLdrexdT : kLdrexT;
    i.size = static_cast<std::uint8_t>(op3 == 4 ? 1 : op3 == 5 ? 2 : 8);
    i.rn = static_cast<std::uint8_t>(rn);
    i.rd = static_cast<std::uint8_t>(rt);
    i.rm = static_cast<std::uint8_t>(rt2);
    i.imm = 0;
    return i;
}

}  // namespace

bool defined_arm_operands(const Insn& i) {
    const auto pc = [](unsigned r) { return r == 15; };
    if (static_cast<unsigned>(i.op) < 16) {
        if (!i.imm_form && i.shift_by_reg && (pc(i.rd) || pc(i.rm) || pc(i.rs) || pc(i.rn))) return false;
        return true;
    }
    switch (i.op) {
        case Op::Ldr: case Op::Str:
            if ((i.wb || !i.pre) && (pc(i.rn) || i.rn == i.rd)) return false;
            if ((i.wb || !i.pre) && !i.imm_form && i.rm == i.rn) return false;
            if (i.op == Op::Str && pc(i.rd)) return false;
            if (i.op == Op::Ldr && pc(i.rd) && (i.size != 4 || i.wb || !i.pre)) return false;
            return !(!i.imm_form && pc(i.rm));
        case Op::Ldm: case Op::Stm: return !pc(i.rn) && !(i.op == Op::Stm && (i.list & 0x8000));
        case Op::B: case Op::Bl: case Op::Swi: case Op::Mrs: case Op::Msr: case Op::Bkpt: case Op::Pld: case Op::Mcr: case Op::Mrc: case Op::Ext: return true;
        case Op::Blx: return i.target.has_value() || !pc(i.rm);
        case Op::Bx: return !pc(i.rm);
        default: return !(pc(i.rd) || pc(i.rn) || pc(i.rm) || pc(i.rs));
    }
}

std::optional<Insn> decode_more(std::uint16_t a, std::uint16_t b, std::uint32_t pc_tagged) {
    Insn i = base(a, b, pc_tagged);
    const unsigned rn = a & 15, rd = (b >> 8) & 15, rt = b >> 12, rm = b & 15;
    if (std::optional<Insn> neon = decode_neon(neon_arm_word(a, b), true, pc_tagged, i.word, Arch::V7A)) return neon;
    // MCR and MRC (the coprocessors other than the VFP's, which are rows): a = 1110 1110 opc1 L CRn, b = Rt cp opc2 1 CRm
    if ((a & 0xFF00) == 0xEE00 && (b & 0x10) && (((b >> 8) & 14) != 10)) {
        if (rt == 13 || ((a & 0x10) == 0 && rt == 15)) return std::nullopt;
        i.op = (a & 0x10) ? Op::Mrc : Op::Mcr;
        i.opc1 = static_cast<std::uint8_t>((a >> 5) & 7);
        i.crn = static_cast<std::uint8_t>(rn);
        i.rd = static_cast<std::uint8_t>(rt);
        i.cp = static_cast<std::uint8_t>((b >> 8) & 15);
        i.opc2 = static_cast<std::uint8_t>((b >> 5) & 7);
        i.crm = static_cast<std::uint8_t>(rm);
        return i;
    }
    // load and store, one register, and the memory hints
    if ((a >> 9) == 0x7C) return single_transfer(i, a, b);
    // load and store dual, load and store exclusive, table branch
    if ((a & 0xFE40) == 0xE840) return dual_exclusive(i, a, b);
    // data processing, plain binary immediate: ADR, SSAT, USAT
    if ((a & 0xFA00) == 0xF200 && !(b & 0x8000)) {
        const unsigned op5 = (a >> 4) & 31;
        const unsigned imm12 = ((a >> 10) & 1u) << 11 | ((b >> 12) & 7u) << 8 | (b & 0xFFu);
        if ((op5 == 0x00 || op5 == 0x0A) && rn == 15) {   // ADR: the pc, word-aligned, plus or minus
            if (sp_or_pc(rd)) return std::nullopt;
            i.op = Op::Ext;
            i.ext = kAdr;
            i.rd = static_cast<std::uint8_t>(rd);
            i.up = op5 == 0;
            i.imm = imm12;
            return i;
        }
        const unsigned imm5 = ((b >> 12) & 7) << 2 | ((b >> 6) & 3);
        if (op5 == 0x10 || op5 == 0x12 || op5 == 0x18 || op5 == 0x1A) {   // SSAT, SSAT16, USAT, USAT16
            if (sp_or_pc(rd) || sp_or_pc(rn) || (b & 0x20)) return std::nullopt;
            const bool is_signed = op5 < 0x18, sh = op5 & 2;
            i.op = Op::Ext;
            i.rd = static_cast<std::uint8_t>(rd);
            i.rn = static_cast<std::uint8_t>(rn);
            if (sh && imm5 == 0) {   // the 16-bit form
                if (b & 0x10) return std::nullopt;
                i.ext = is_signed ? kSsat16T : kUsat16T;
                i.imm = (b & 15) + (is_signed ? 1 : 0);
            } else {
                i.ext = is_signed ? kSsatT : kUsatT;
                i.imm = (b & 31) + (is_signed ? 1 : 0);
                i.shift = sh ? Shift::Asr : Shift::Lsl;
                i.amount = static_cast<std::uint8_t>(imm5);
            }
            return i;
        }
        return std::nullopt;
    }
    // branches and miscellaneous control: the hints, the barriers, MSR and MRS
    if ((a & 0xF800) == 0xF000 && (b & 0x8000) && !(b & 0x5000) && ((a >> 7) & 7) == 7) {
        if ((a & 0xFFE0) == 0xF380 && (b & 0xF0FF) == 0x8000) {   // MSR (register)
            i.op = Op::Msr;
            i.rm = static_cast<std::uint8_t>(rn);
            i.spsr = (a >> 4) & 1;
            i.fields = static_cast<std::uint8_t>((b >> 8) & 15);
            if (sp_or_pc(rn) || i.fields == 0) return std::nullopt;
            return i;
        }
        if (a == 0xF3AF && (b & 0xFF00) == 0x8000) {   // hints (the unallocated ones are NOPs)
            i.op = Op::Ext;
            i.ext = kNop32;
            return i;
        }
        if (a == 0xF3BF && (b & 0xFF00) == 0x8F00 && ((b >> 4) & 15) >= 2 && ((b >> 4) & 15) <= 6 && ((b >> 4) & 15) != 3) {   // CLREX, DSB, DMB, ISB
            i.op = Op::Ext;
            i.ext = kNop32;
            return i;
        }
        if ((a == 0xF3EF || a == 0xF3FF) && (b & 0xF0FF) == 0x8000) {   // MRS
            if (sp_or_pc(rd)) return std::nullopt;
            i.op = Op::Mrs;
            i.rd = static_cast<std::uint8_t>(rd);
            i.spsr = a == 0xF3FF;
            return i;
        }
        return std::nullopt;
    }
    // multiplies and the DSP multiplies
    if ((a & 0xFF80) == 0xFB00 && (b & 0x00C0) == 0) {
        const unsigned op1 = (a >> 4) & 7, op2 = (b >> 4) & 3, ra = rt;
        if (sp_or_pc(rd) || sp_or_pc(rn) || sp_or_pc(rm)) return std::nullopt;
        const bool acc = ra != 15;
        if (acc && sp_or_pc(ra)) return std::nullopt;
        const std::uint32_t common = static_cast<std::uint32_t>(rd) << 16 | static_cast<std::uint32_t>(ra) << 12 | static_cast<std::uint32_t>(rm) << 8 | static_cast<std::uint32_t>(rn);
        switch (op1) {
            case 0:
                if (op2 != 1) return std::nullopt;   // (MUL and MLA are decode_thumb32_int's)
                if (ra == 15) return std::nullopt;
                i.op = Op::Ext;
                i.ext = kMls;
                i.rd = static_cast<std::uint8_t>(rd);
                i.rn = static_cast<std::uint8_t>(rn);
                i.rm = static_cast<std::uint8_t>(rm);
                i.rs = static_cast<std::uint8_t>(ra);
                return i;
            case 1:   // SMLAxy, SMULxy: x picks Rn's half, y Rm's
                i.op = acc ? Op::Smla : Op::Smul;
                i.rd = static_cast<std::uint8_t>(rd);
                i.rm = static_cast<std::uint8_t>(rn);
                i.rs = static_cast<std::uint8_t>(rm);
                i.rn = static_cast<std::uint8_t>(acc ? ra : 0);
                i.xy = static_cast<std::uint8_t>(((b >> 5) & 1) | ((b >> 4) & 1) << 1);
                return i;
            case 2: case 4:   // SMLAD, SMUAD, SMLSD, SMUSD (the X bit)
                if (op2 > 1) return std::nullopt;
                return as_row(i, (op1 == 2 ? (acc ? 0x07000010u : 0x0700F010u) : (acc ? 0x07000050u : 0x0700F050u)) | common | static_cast<std::uint32_t>(op2) << 5);
            case 3:   // SMLAWy, SMULWy
                if (op2 > 1) return std::nullopt;
                i.op = acc ? Op::Smlaw : Op::Smulw;
                i.rd = static_cast<std::uint8_t>(rd);
                i.rm = static_cast<std::uint8_t>(rn);
                i.rs = static_cast<std::uint8_t>(rm);
                i.rn = static_cast<std::uint8_t>(acc ? ra : 0);
                i.xy = static_cast<std::uint8_t>(op2 << 1);
                return i;
            case 5:   // SMMLA, SMMUL (R)
                if (op2 > 1) return std::nullopt;
                return as_row(i, (acc ? 0x07500010u : 0x0750F010u) | common | static_cast<std::uint32_t>(op2) << 5);
            case 6:   // SMMLS (R)
                if (op2 > 1 || !acc) return std::nullopt;
                return as_row(i, 0x075000D0u | common | static_cast<std::uint32_t>(op2) << 5);
            default:  // USAD8, USADA8
                if (op2 != 0) return std::nullopt;
                return as_row(i, (acc ? 0x07800010u : 0x0780F010u) | common);
        }
    }
    // long multiplies: SMLALxy, SMLALD, SMLSLD, UMAAL (SMULL, UMULL, SMLAL, UMLAL are decode_thumb32_int's; SDIV and UDIV are not in the Cortex-A9)
    if ((a & 0xFF80) == 0xFB80) {
        const unsigned op1 = (a >> 4) & 7, op2 = (b >> 4) & 15, lo = rt, hi = rd;
        if (sp_or_pc(lo) || sp_or_pc(hi) || lo == hi || sp_or_pc(rn) || sp_or_pc(rm)) return std::nullopt;
        const std::uint32_t common = static_cast<std::uint32_t>(hi) << 16 | static_cast<std::uint32_t>(lo) << 12 | static_cast<std::uint32_t>(rm) << 8 | static_cast<std::uint32_t>(rn);
        if (op1 == 4 && (op2 & 12) == 8) {   // SMLALBB, BT, TB, TT
            i.op = Op::Smlalxy;
            i.rd = static_cast<std::uint8_t>(lo);
            i.rn = static_cast<std::uint8_t>(hi);
            i.rm = static_cast<std::uint8_t>(rn);
            i.rs = static_cast<std::uint8_t>(rm);
            i.xy = static_cast<std::uint8_t>(((op2 >> 1) & 1) | (op2 & 1) << 1);
            return i;
        }
        if ((op1 == 4 || op1 == 5) && (op2 & 14) == 12) return as_row(i, (op1 == 4 ? 0x07400010u : 0x07400050u) | common | static_cast<std::uint32_t>(op2 & 1) << 5);
        if (op1 == 6 && op2 == 6) return as_row(i, 0x00400090u | common);
        return std::nullopt;
    }
    // the register-controlled forms and the extensions, parallel arithmetic, saturating arithmetic and the byte reversals (a = 1111 1010 ...)
    if ((a & 0xFF00) == 0xFA00 && rt == 15) {
        const unsigned op1 = (a >> 4) & 7;
        if ((a & 0x80) == 0) {   // the extensions: SXTAH ... UXTB16 (the ones on Rn = pc that decode_thumb32_int does not take: SXTB16, UXTB16)
            if ((b & 0x00C0) != 0x0080 || op1 > 5 || sp_or_pc(rd) || sp_or_pc(rm)) return std::nullopt;
            if (rn == 15 && op1 != 2 && op1 != 3) return std::nullopt;
            if (rn == 13) return std::nullopt;
            static constexpr std::uint32_t kAcc[6] = {0x06B00070, 0x06F00070, 0x06800070, 0x06C00070, 0x06A00070, 0x06E00070};
            static constexpr std::uint32_t kPlain[6] = {0, 0, 0x068F0070, 0x06CF0070, 0, 0};
            const std::uint32_t arm = (rn == 15 ? kPlain[op1] : kAcc[op1] | static_cast<std::uint32_t>(rn) << 16) | static_cast<std::uint32_t>(rd) << 12 | ((b >> 4) & 3u) << 10 | rm;
            return as_row(i, arm);
        }
        if (!(b & 0x80)) {   // parallel add and subtract
            const unsigned prefix = (b >> 4) & 3, unsigned_form = (b >> 6) & 1;
            static constexpr int kOperation[8] = {9, 1, 3, -1, 15, 7, 5, -1};   // ADD8 ADD16 ASX . SUB8 SUB16 SAX .
            if (prefix == 3 || kOperation[op1] < 0 || sp_or_pc(rd) || sp_or_pc(rn) || sp_or_pc(rm)) return std::nullopt;
            static constexpr unsigned kPrefixBits[2][3] = {{1, 2, 3}, {5, 6, 7}};
            return as_row(i, 0x06000F00u | kPrefixBits[unsigned_form][prefix] << 20 | static_cast<std::uint32_t>(kOperation[op1]) << 4 | static_cast<std::uint32_t>(rn) << 16 | static_cast<std::uint32_t>(rd) << 12 | rm);
        }
        if ((b & 0x00C0) == 0x0080) {   // miscellaneous: QADD ... QDSUB, REV, REV16, RBIT, REVSH, SEL
            const unsigned op2 = (b >> 4) & 3;
            if (sp_or_pc(rd) || sp_or_pc(rm)) return std::nullopt;
            if (op1 == 0) {
                if (sp_or_pc(rn)) return std::nullopt;
                // (Thumb's op2: QADD, QDADD, QSUB, QDSUB)
                static constexpr Op kOps[4] = {Op::Qadd, Op::Qdadd, Op::Qsub, Op::Qdsub};
                i.op = kOps[op2];
                i.rd = static_cast<std::uint8_t>(rd);
                i.rm = static_cast<std::uint8_t>(rm);
                i.rn = static_cast<std::uint8_t>(rn);
                return i;
            }
            if (op1 == 1) {
                if (rn != rm) return std::nullopt;
                static constexpr std::uint32_t kRev[4] = {0x06BF0F30, 0x06BF0FB0, 0x06FF0F30, 0x06FF0FB0};
                return as_row(i, kRev[op2] | static_cast<std::uint32_t>(rd) << 12 | rm);
            }
            if (op1 == 2 && op2 == 0) {
                if (sp_or_pc(rn)) return std::nullopt;
                return as_row(i, 0x06800FB0u | static_cast<std::uint32_t>(rn) << 16 | static_cast<std::uint32_t>(rd) << 12 | rm);
            }
        }
        return std::nullopt;
    }
    return std::nullopt;
}

}  // namespace cb::decompile::arm::t2
