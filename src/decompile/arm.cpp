// decompile/arm.cpp -- the ARM7TDMI's instructions: ARM and Thumb decoded into one set of fields, written as text, and assembled back
// (arm.hpp).

#include "decompile/arm.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <format>

#include "core/text.hpp"
#include "decompile/arm_ext.hpp"

namespace cb::decompile::arm {

namespace {

constexpr std::array<std::string_view, 16> kRegisters = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "r12", "sp", "lr", "pc"};
constexpr std::array<std::string_view, 15> kConditions = {"eq", "ne", "cs", "cc", "mi", "pl", "vs", "vc", "hi", "ls", "ge", "lt", "gt", "le", ""};
constexpr std::array<std::string_view, 4> kShifts = {"lsl", "lsr", "asr", "ror"};
constexpr std::array<std::string_view, 16> kDataNames = {"and", "eor", "sub", "rsb", "add", "adc", "sbc", "rsc", "tst", "teq", "cmp", "cmn", "orr", "mov", "bic", "mvn"};
constexpr std::uint8_t kAlways = 14;

constexpr bool is_test(Op op) { return op == Op::Tst || op == Op::Teq || op == Op::Cmp || op == Op::Cmn; }
constexpr bool is_data(Op op) { return static_cast<std::uint8_t>(op) < 16; }

std::int32_t sext(std::uint32_t v, unsigned bits) {
    const unsigned shift = 32 - bits;
    return static_cast<std::int32_t>(v << shift) >> shift;
}

constexpr std::uint32_t rotate_left(std::uint32_t v, unsigned n) { return rotate_right(v, 32 - (n & 31)); }

// The smallest even rotate that makes `value` an 8-bit number (what GNU as picks); nullopt when there is none.
std::optional<unsigned> minimal_rotate(std::uint32_t value) {
    for (unsigned r = 0; r < 16; ++r)
        if (rotate_left(value, 2 * r) <= 0xFF) return r;
    return std::nullopt;
}

void fill_raw(Insn& i) {
    for (unsigned k = 0; k < i.length; ++k) i.raw[k] = static_cast<std::uint8_t>(i.word >> (8 * k));
}

// How control leaves the instruction (called once its fields are known).
void set_flow(Insn& i) {
    const bool always = i.cond == kAlways;
    i.flow = Flow::None;
    switch (i.op) {
        case Op::B: i.flow = always ? Flow::Jump : Flow::Branch; return;
        case Op::Bl: i.flow = Flow::Call; return;
        case Op::Blx: i.flow = Flow::Call; return;
        case Op::Bx: i.flow = !always ? Flow::None : i.rm == 14 ? Flow::Return : Flow::Indirect; return;
        case Op::Ldm:
            if (always && (i.list & 0x8000)) i.flow = i.rn == 13 ? Flow::Return : Flow::Indirect;
            return;
        case Op::Ldr:
            if (always && i.rd == 15) i.flow = Flow::Indirect;
            return;
        default: break;
    }
    if (is_data(i.op) && !is_test(i.op) && i.rd == 15 && always)
        i.flow = (i.op == Op::Mov && !i.imm_form && i.rm == 14 && !i.shift_by_reg && i.shift == Shift::Lsl && i.amount == 0) ? Flow::Return : Flow::Indirect;
}

// ---- decoding: ARM ------------------------------------------------------------------------------------------------------------

// The register operand of a data-processing instruction or an addressing mode: Rm and its shift by an immediate.
void register_with_shift(Insn& i, std::uint32_t w) {
    i.rm = w & 15;
    i.shift = static_cast<Shift>((w >> 5) & 3);
    const unsigned amount = (w >> 7) & 31;
    i.amount = static_cast<std::uint8_t>(amount);
    if (amount == 0 && i.shift != Shift::Lsl) {
        if (i.shift == Shift::Ror) i.rrx = true;
        else i.amount = 32;
    }
}

bool arm_data(Insn& i, std::uint32_t w, bool immediate) {
    i.op = static_cast<Op>((w >> 21) & 15);
    i.s = (w >> 20) & 1;
    i.rn = (w >> 16) & 15;
    i.rd = (w >> 12) & 15;
    if (is_test(i.op) && (!i.s || i.rd != 0)) return false;
    if ((i.op == Op::Mov || i.op == Op::Mvn) && i.rn != 0) return false;
    if (immediate) {
        i.imm_form = true;
        i.rot = (w >> 8) & 15;
        i.imm = rotate_right(w & 0xFF, 2u * i.rot);
    } else if (w & 0x10) {
        if (w & 0x80) return false;
        i.rm = w & 15;
        i.shift = static_cast<Shift>((w >> 5) & 3);
        i.shift_by_reg = true;
        i.rs = (w >> 8) & 15;
    } else {
        register_with_shift(i, w);
    }
    return true;
}

bool arm_multiply(Insn& i, std::uint32_t w) {
    if ((w & 0x0FC00000) == 0) {
        const bool accumulate = (w >> 21) & 1;
        i.op = accumulate ? Op::Mla : Op::Mul;
        i.s = (w >> 20) & 1;
        i.rd = (w >> 16) & 15;
        i.rn = (w >> 12) & 15;
        i.rs = (w >> 8) & 15;
        i.rm = w & 15;
        return accumulate || i.rn == 0;
    }
    if ((w & 0x0F800000) == 0x00800000) {
        const bool is_signed = (w >> 22) & 1, accumulate = (w >> 21) & 1;
        i.op = is_signed ? (accumulate ? Op::Smlal : Op::Smull) : (accumulate ? Op::Umlal : Op::Umull);
        i.s = (w >> 20) & 1;
        i.rn = (w >> 16) & 15;   // RdHi
        i.rd = (w >> 12) & 15;   // RdLo
        i.rs = (w >> 8) & 15;
        i.rm = w & 15;
        return true;
    }
    if ((w & 0x0FB00FF0) == 0x01000090) {
        i.op = Op::Swp;
        i.size = ((w >> 22) & 1) ? 1 : 4;
        i.rn = (w >> 16) & 15;
        i.rd = (w >> 12) & 15;
        i.rm = w & 15;
        return true;
    }
    return false;
}

bool arm_halfword(Insn& i, std::uint32_t w, Arch arch) {
    const bool pre = (w >> 24) & 1, up = (w >> 23) & 1, immediate = (w >> 22) & 1, wb = (w >> 21) & 1, load = (w >> 20) & 1;
    const unsigned kind = (w >> 5) & 3;
    if (!pre && wb) return false;
    if (!immediate && ((w >> 8) & 15) != 0) return false;
    const bool pair = !load && kind >= 2;   // LDRD (S=1, H=0) and STRD (S=1, H=1): ARMv5TE
    if (!load && kind != 1 && !(pair && arch >= Arch::V5TE)) return false;
    if (pair && (((w >> 12) & 1) || ((w >> 12) & 15) == 14)) return false;   // (an odd or lr first register is undefined)
    i.op = (load || kind == 2) ? Op::Ldr : Op::Str;
    if (pair) {
        i.size = 8;
        i.sign = false;
    }
    if (!pair) {
        i.sign = kind >= 2;
        i.size = kind == 2 ? 1 : 2;
    }
    i.pre = pre;
    i.up = up;
    i.wb = pre ? wb : true;
    i.rn = (w >> 16) & 15;
    i.rd = (w >> 12) & 15;
    i.imm_form = immediate;
    if (immediate) i.imm = ((w >> 4) & 0xF0) | (w & 15);
    else i.rm = w & 15;
    return true;
}

bool arm_misc(Insn& i, std::uint32_t w, bool immediate, Arch arch) {
    if (!immediate && arch >= Arch::V5TE) {
        if ((w & 0x0FFFFFF0) == 0x012FFF30) {   // BLX rm
            i.op = Op::Blx;
            i.rm = w & 15;
            return true;
        }
        if ((w & 0x0FFF0FF0) == 0x016F0F10) {   // CLZ rd, rm
            i.op = Op::Clz;
            i.rd = (w >> 12) & 15;
            i.rm = w & 15;
            return true;
        }
        if ((w & 0x0F900FF0) == 0x01000050) {   // QADD QSUB QDADD QDSUB rd, rm, rn
            i.op = static_cast<Op>(static_cast<unsigned>(Op::Qadd) + ((w >> 21) & 3));
            i.rd = (w >> 12) & 15;
            i.rn = (w >> 16) & 15;
            i.rm = w & 15;
            return true;
        }
        if ((w & 0x0F900090) == 0x01000080) {   // SMLAxy, SMLAWy and SMULWy, SMLALxy, SMULxy
            const unsigned kind = (w >> 21) & 3;
            i.rm = w & 15;
            i.rs = (w >> 8) & 15;
            i.rd = (w >> 16) & 15;
            i.rn = (w >> 12) & 15;
            if (kind == 1) {   // bit 5 tells the two apart; the y bit (6) picks rs's half
                i.op = (w & 0x20) ? Op::Smulw : Op::Smlaw;
                i.xy = static_cast<std::uint8_t>(((w >> 6) & 1) << 1);
                return i.op != Op::Smulw || i.rn == 0;
            }
            i.xy = static_cast<std::uint8_t>(((w >> 5) & 1) | ((w >> 6) & 1) << 1);   // x picks rm's half, y rs's
            i.op = kind == 0 ? Op::Smla : kind == 2 ? Op::Smlalxy : Op::Smul;
            if (i.op == Op::Smlalxy) std::swap(i.rd, i.rn);   // (rd: the low word, rn: the high word, as UMLAL has them)
            return i.op != Op::Smul || i.rn == 0;
        }
        if ((w & 0xFFF000F0) == 0xE1200070) {   // BKPT (always unconditional)
            i.op = Op::Bkpt;
            i.imm = ((w >> 4) & 0xFFF0) | (w & 15);
            return true;
        }
    }
    if (!immediate) {
        if ((w & 0x0FFFFFF0) == 0x012FFF10) {
            i.op = Op::Bx;
            i.rm = w & 15;
            return true;
        }
        if ((w & 0x0FBF0FFF) == 0x010F0000) {
            i.op = Op::Mrs;
            i.rd = (w >> 12) & 15;
            i.spsr = (w >> 22) & 1;
            return true;
        }
        if ((w & 0x0FB0FFF0) == 0x0120F000) {
            i.op = Op::Msr;
            i.spsr = (w >> 22) & 1;
            i.fields = (w >> 16) & 15;
            i.rm = w & 15;
            return i.fields != 0;
        }
        return false;
    }
    if ((w & 0x0FB0F000) == 0x0320F000) {
        i.op = Op::Msr;
        i.spsr = (w >> 22) & 1;
        i.fields = (w >> 16) & 15;
        i.imm_form = true;
        i.rot = (w >> 8) & 15;
        i.imm = rotate_right(w & 0xFF, 2u * i.rot);
        return i.fields != 0;
    }
    return false;
}

bool arm_transfer(Insn& i, std::uint32_t w, bool register_offset) {
    if (register_offset && (w & 0x10)) return false;
    const bool pre = (w >> 24) & 1, up = (w >> 23) & 1, byte = (w >> 22) & 1, wb = (w >> 21) & 1, load = (w >> 20) & 1;
    i.op = load ? Op::Ldr : Op::Str;
    i.size = byte ? 1 : 4;
    i.pre = pre;
    i.up = up;
    i.wb = pre ? wb : true;
    i.user = !pre && wb;
    i.rn = (w >> 16) & 15;
    i.rd = (w >> 12) & 15;
    if (register_offset) register_with_shift(i, w);
    else {
        i.imm_form = true;
        i.imm = w & 0xFFF;
    }
    return true;
}

// The instructions of the unconditional space (the condition field 1111): BLX with an immediate, and PLD.
std::optional<Insn> decode_unconditional(std::uint32_t w, std::uint32_t pc) {
    Insn i;
    i.word = w;
    i.pc = pc & ~3u;
    i.isa = Isa::Arm;
    i.length = 4;
    i.cond = kAlways;
    fill_raw(i);
    if ((w & 0x0E000000) == 0x0A000000) {   // BLX label: always Thumb state at the target; the H bit is its halfword
        i.op = Op::Blx;
        i.target = (i.pc + 8 + (static_cast<std::uint32_t>(sext(w & 0xFFFFFF, 24)) << 2) + (((w >> 24) & 1) << 1)) | 1u;
    } else if ((w & 0xFF70F000) == 0xF550F000) {   // PLD [rn, #offset]
        i.op = Op::Pld;
        i.pre = true;
        i.up = (w >> 23) & 1;
        i.size = 1;
        i.rn = (w >> 16) & 15;
        i.imm_form = true;
        i.imm = w & 0xFFF;
    } else if ((w & 0xFF70F010) == 0xF750F000) {   // PLD [rn, +-rm, shift]
        i.op = Op::Pld;
        i.pre = true;
        i.up = (w >> 23) & 1;
        i.size = 1;
        i.rn = (w >> 16) & 15;
        register_with_shift(i, w);
    } else {
        return std::nullopt;
    }
    set_flow(i);
    return i;
}

}  // namespace

std::optional<Insn> decode_arm(std::uint32_t w, std::uint32_t pc, Arch arch) {
    const unsigned cond = w >> 28;
    // the ARMv6 and later rows (ARM state): found first for the conditional words that no core table says, below, and for the unconditional space here
    const auto from_ext = [&](std::uint32_t word) -> std::optional<Insn> {
        if (arch < Arch::V6K) return std::nullopt;
        const int row = ext::find_arm(word, arch);
        if (row < 0) return std::nullopt;
        Insn e;
        e.word = word;
        e.pc = pc & ~3u;
        e.isa = Isa::Arm;
        e.length = 4;
        e.op = Op::Ext;
        e.ext = static_cast<std::int16_t>(row);
        e.cond = ext::conditional(row) ? static_cast<std::uint8_t>(word >> 28) : kAlways;
        fill_raw(e);
        return e;
    };
    if (cond == 15) {
        if (auto e = from_ext(w)) return e;
        return arch >= Arch::V5TE ? decode_unconditional(w, pc) : std::nullopt;
    }
    if (auto e = from_ext(w)) return e;   // (the rows fix every bit that tells an instruction apart, so they come before the core tables)
    Insn i;
    i.word = w;
    i.pc = pc & ~3u;
    i.isa = Isa::Arm;
    i.length = 4;
    i.cond = static_cast<std::uint8_t>(cond);
    fill_raw(i);
    bool ok = false;
    switch ((w >> 25) & 7) {
        case 0:
            if ((w & 0x90) == 0x90) ok = (w & 0x60) == 0 ? arm_multiply(i, w) : arm_halfword(i, w, arch);
            else if ((w & 0x01900000) == 0x01000000) ok = arm_misc(i, w, false, arch);
            else ok = arm_data(i, w, false);
            break;
        case 1: ok = (w & 0x01900000) == 0x01000000 ? arm_misc(i, w, true, arch) : arm_data(i, w, true); break;
        case 2: ok = arm_transfer(i, w, false); break;
        case 3: ok = arm_transfer(i, w, true); break;
        case 4:
            i.op = ((w >> 20) & 1) ? Op::Ldm : Op::Stm;
            i.pre = (w >> 24) & 1;
            i.up = (w >> 23) & 1;
            i.hat = (w >> 22) & 1;
            i.wb = (w >> 21) & 1;
            i.rn = (w >> 16) & 15;
            i.list = static_cast<std::uint16_t>(w & 0xFFFF);
            ok = i.list != 0;
            break;
        case 5:
            i.op = ((w >> 24) & 1) ? Op::Bl : Op::B;
            i.target = i.pc + 8 + (static_cast<std::uint32_t>(sext(w & 0xFFFFFF, 24)) << 2);
            ok = true;
            break;
        case 7:
            if (((w >> 24) & 15) == 15) {
                i.op = Op::Swi;
                i.imm = w & 0xFFFFFF;
                ok = true;
            } else if (arch >= Arch::V5TE && ((w >> 24) & 15) == 14 && (w & 0x10)) {   // MCR, MRC (CDP is not one)
                i.op = ((w >> 20) & 1) ? Op::Mrc : Op::Mcr;
                i.opc1 = (w >> 21) & 7;
                i.crn = (w >> 16) & 15;
                i.rd = (w >> 12) & 15;
                i.cp = (w >> 8) & 15;
                i.opc2 = (w >> 5) & 7;
                i.crm = w & 15;
                ok = true;
            }
            break;
        default: break;
    }
    if (!ok) return std::nullopt;
    set_flow(i);
    return i;
}

// ---- decoding: Thumb-2 32-bit integer instructions ----------------------------------------------------------------------------

// The value of a Thumb modified immediate (imm12 = i:imm3:imm8), nullopt for the encodings that are unpredictable.
std::optional<std::uint32_t> thumb_expand(std::uint32_t imm12) {
    const std::uint32_t imm8 = imm12 & 0xFF;
    if ((imm12 >> 10) == 0) {
        switch ((imm12 >> 8) & 3) {
            case 0: return imm8;
            case 1: return imm8 == 0 ? std::nullopt : std::optional<std::uint32_t>(imm8 << 16 | imm8);
            case 2: return imm8 == 0 ? std::nullopt : std::optional<std::uint32_t>(imm8 << 24 | imm8 << 8);
            default: return imm8 == 0 ? std::nullopt : std::optional<std::uint32_t>(imm8 * 0x01010101u);
        }
    }
    return rotate_right(0x80 | (imm12 & 0x7F), (imm12 >> 7) & 31);
}

// The 32-bit Thumb instructions the interpreter runs as the fields they mean (data processing, loads and stores, block transfers, multiplies, shifts); their text is
// the disassembler's (`ext` = t2::kOpaque), and the few that need more than the common fields are `Op::Ext` rows of t2 (MOVT, bit fields, extensions).
std::optional<Insn> decode_thumb32_int(std::uint16_t a, std::uint16_t b, std::uint32_t pc_tagged) {
    Insn i;
    i.word = static_cast<std::uint32_t>(a) | static_cast<std::uint32_t>(b) << 16;
    i.pc = pc_tagged & ~1u;
    i.isa = Isa::Thumb;
    i.length = 4;
    i.cond = kAlways;
    i.v6 = true;
    i.ext = t2::kOpaque;
    fill_raw(i);
    const unsigned rn = a & 15, rd = (b >> 8) & 15, rt = (b >> 12) & 15;
    const auto shifted = [&]() {
        i.rm = b & 15;
        i.shift = static_cast<Shift>((b >> 4) & 3);
        const unsigned amount = ((b >> 12) & 7) << 2 | ((b >> 6) & 3);
        i.amount = static_cast<std::uint8_t>(amount);
        if (amount == 0 && i.shift != Shift::Lsl) {
            if (i.shift == Shift::Ror) i.rrx = true;
            else i.amount = 32;
        }
    };
    const auto data_op = [&](unsigned op4, bool s, unsigned n, unsigned d) -> bool {
        static constexpr int map[16] = {static_cast<int>(Op::And), static_cast<int>(Op::Bic), static_cast<int>(Op::Orr), -1, static_cast<int>(Op::Eor), -1, -1, -1,
                                        static_cast<int>(Op::Add), -1, static_cast<int>(Op::Adc), static_cast<int>(Op::Sbc), -1, static_cast<int>(Op::Sub), static_cast<int>(Op::Rsb), -1};
        if (op4 == 3 && n == 15) {   // ORN with no first operand is MVN
            i.op = Op::Mvn;
            i.s = s;
            i.rn = 0;
            i.rd = static_cast<std::uint8_t>(d);
            return d != 15;
        }
        if (map[op4] < 0) return false;
        i.op = static_cast<Op>(map[op4]);
        i.s = s;
        i.rn = static_cast<std::uint8_t>(n);
        i.rd = static_cast<std::uint8_t>(d);
        if (d == 15 && s) {   // the test forms
            if (i.op == Op::And) i.op = Op::Tst;
            else if (i.op == Op::Eor) i.op = Op::Teq;
            else if (i.op == Op::Add) i.op = Op::Cmn;
            else if (i.op == Op::Sub) i.op = Op::Cmp;
            else return false;
            i.rd = 0;
        } else if (n == 15 && i.op == Op::Orr) {   // ORR with no first operand is MOV
            i.op = Op::Mov;
            i.rn = 0;
        } else if (d == 15 || n == 15) {
            return false;   // (the pc as an operand is not run)
        }
        return true;
    };
    // data processing, modified immediate
    if ((a & 0xFA00) == 0xF000 && !(b & 0x8000)) {
        const std::uint32_t imm12 = ((a >> 10) & 1u) << 11 | ((b >> 12) & 7u) << 8 | (b & 0xFFu);
        const auto value = thumb_expand(imm12);
        if (!value || !data_op((a >> 5) & 15, (a >> 4) & 1, rn, rd)) return std::nullopt;
        i.imm_form = true;
        i.imm = *value;
        i.rot = (imm12 >> 10) != 0 ? 1 : 0;
        return i;
    }
    if ((a & 0xFFF0) == 0xF3C0 || (a & 0xFFF0) == 0xF340 || (a & 0xFFF0) == 0xF360) {   // UBFX, SBFX, BFI, BFC
        if ((b & 0x8000) || rd == 15 || rd == 13) return std::nullopt;
        const unsigned lsb = ((b >> 12) & 7) << 2 | ((b >> 6) & 3), field = b & 31;
        i.op = Op::Ext;
        i.rd = static_cast<std::uint8_t>(rd);
        i.rn = static_cast<std::uint8_t>(rn);
        i.imm = lsb;
        if ((a & 0xFFF0) == 0xF360) {
            if (field < lsb) return std::nullopt;
            i.ext = t2::kBfi;
            i.rs = static_cast<std::uint8_t>(field - lsb + 1);   // the width
            return i;
        }
        if (rn == 15 || rn == 13 || lsb + field + 1 > 32) return std::nullopt;
        i.ext = (a & 0xFFF0) == 0xF3C0 ? t2::kUbfx : t2::kSbfx;
        i.rs = static_cast<std::uint8_t>(field + 1);
        return i;
    }
    // plain binary immediate: ADDW, SUBW, MOVW, MOVT
    if ((a & 0xFA00) == 0xF200 && !(b & 0x8000)) {
        const unsigned op5 = (a >> 4) & 31;
        const std::uint32_t imm12 = ((a >> 10) & 1u) << 11 | ((b >> 12) & 7u) << 8 | (b & 0xFFu);
        if ((op5 == 0x00 || op5 == 0x0A) && rn != 15 && rd != 15) {
            i.op = op5 == 0 ? Op::Add : Op::Sub;
            i.rn = static_cast<std::uint8_t>(rn);
            i.rd = static_cast<std::uint8_t>(rd);
            i.imm_form = true;
            i.imm = imm12;
            return i;
        }
        if (op5 == 0x04 && rd != 15 && rd != 13) {   // MOVW
            i.op = Op::Mov;
            i.rd = static_cast<std::uint8_t>(rd);
            i.imm_form = true;
            i.imm = rn << 12 | imm12;
            return i;
        }
        if (op5 == 0x0C && rd != 15 && rd != 13) {   // MOVT
            i.op = Op::Ext;
            i.ext = t2::kMovt;
            i.rd = static_cast<std::uint8_t>(rd);
            i.imm = rn << 12 | imm12;
            return i;
        }
        return std::nullopt;
    }
    // data processing, shifted register
    if ((a & 0xFE00) == 0xEA00 && !(b & 0x8000)) {
        if (!data_op((a >> 5) & 15, (a >> 4) & 1, rn, rd)) return std::nullopt;
        shifted();
        if (i.rm == 15 || i.rm == 13) return std::nullopt;
        return i;
    }
    // load and store, one register
    if ((a >> 9) == 0x7C) {
        const bool sign = (a >> 8) & 1, imm12 = (a >> 7) & 1, load = (a >> 4) & 1;
        const unsigned size = (a >> 5) & 3;
        if (size == 3 || (!load && sign) || (rt == 15 && !load) || rt == 13) return std::nullopt;
        if (rt == 15 && size != 2) return std::nullopt;   // (PLD, PLI)
        i.op = load ? Op::Ldr : Op::Str;
        i.size = static_cast<std::uint8_t>(size == 0 ? 1 : size == 1 ? 2 : 4);
        i.sign = sign;
        i.rd = static_cast<std::uint8_t>(rt);
        i.rn = static_cast<std::uint8_t>(rn);
        if (rn == 15) {   // a literal load of a word
            if (!load || sign || size != 2) return std::nullopt;
            i.imm_form = true;
            i.pre = true;
            i.up = imm12;   // (the U bit takes bit 7's place)
            i.imm = b & 0xFFF;
            set_flow(i);
            return i;
        }
        if (imm12) {
            i.imm_form = true;
            i.pre = true;
            i.up = true;
            i.imm = b & 0xFFF;
        } else if ((b & 0x0800) != 0) {
            const bool p = (b >> 10) & 1, u = (b >> 9) & 1, wback = (b >> 8) & 1;
            if ((p && u && !wback) || (!p && !wback)) return std::nullopt;   // (LDRT and STRT, undefined)
            i.imm_form = true;
            i.pre = p;
            i.up = u;
            i.wb = wback;
            i.imm = b & 0xFF;
        } else if ((b & 0x0FC0) == 0) {
            i.imm_form = false;
            i.pre = true;
            i.up = true;
            i.rm = b & 15;
            i.shift = Shift::Lsl;
            i.amount = static_cast<std::uint8_t>((b >> 4) & 3);
            if (i.rm == 15 || i.rm == 13) return std::nullopt;
        } else {
            return std::nullopt;
        }
        set_flow(i);
        return i;
    }
    // block transfers: LDM, STM (POP.W and PUSH.W are the ones on the stack pointer)
    if ((a >> 9) == 0x74 && !((a >> 6) & 1) && (((a >> 7) & 3) == 1 || ((a >> 7) & 3) == 2)) {
        const bool load = (a >> 4) & 1, down = ((a >> 7) & 3) == 2;
        i.op = load ? Op::Ldm : Op::Stm;
        i.pre = down;
        i.up = !down;
        i.wb = (a >> 5) & 1;
        i.rn = static_cast<std::uint8_t>(rn);
        i.list = b;
        if (rn == 15 || b == 0 || (b & 0x2000) || (!load && (b & 0x8000))) return std::nullopt;
        set_flow(i);
        return i;
    }
    // multiplies
    if ((a & 0xFFF0) == 0xFB00 && (b & 0x00F0) == 0) {
        const bool accumulate = (b >> 12) != 15;
        i.op = accumulate ? Op::Mla : Op::Mul;
        i.rd = static_cast<std::uint8_t>(rd);
        i.rm = b & 15;
        i.rs = static_cast<std::uint8_t>(rn);
        i.rn = static_cast<std::uint8_t>(accumulate ? rt : 0);
        if (rd == 15 || rd == 13 || i.rm == 15 || i.rm == 13 || rn == 15 || rn == 13) return std::nullopt;
        return i;
    }
    if (((a & 0xFFF0) == 0xFBA0 || (a & 0xFFF0) == 0xFB80 || (a & 0xFFF0) == 0xFBE0 || (a & 0xFFF0) == 0xFBC0) && (b & 0x00F0) == 0) {
        const unsigned kind = (a >> 4) & 15;   // 0xA umull, 0x8 smull, 0xE umlal, 0xC smlal
        i.op = kind == 0xA ? Op::Umull : kind == 0x8 ? Op::Smull : kind == 0xE ? Op::Umlal : Op::Smlal;
        i.rd = static_cast<std::uint8_t>(rt);   // RdLo
        i.rn = static_cast<std::uint8_t>(rd);   // RdHi
        i.rm = b & 15;
        i.rs = static_cast<std::uint8_t>(rn);
        if (rt == 15 || rd == 15 || rt == 13 || rd == 13 || rt == rd || i.rm == 15 || rn == 15) return std::nullopt;
        return i;
    }
    // shifts by a register
    if ((a & 0xFF80) == 0xFA00 && (b & 0xF0F0) == 0xF000) {
        i.op = Op::Mov;
        i.s = (a >> 4) & 1;
        i.rd = static_cast<std::uint8_t>(rd);
        i.rm = static_cast<std::uint8_t>(rn);
        i.rs = b & 15;
        i.shift = static_cast<Shift>((a >> 5) & 3);
        i.shift_by_reg = true;
        if (rd == 15 || rd == 13 || rn == 15 || rn == 13 || i.rs == 15 || i.rs == 13) return std::nullopt;
        return i;
    }
    // sign and zero extension with a rotate, CLZ
    if (a == 0xFA0F || a == 0xFA1F || a == 0xFA4F || a == 0xFA5F) {
        if ((b & 0xF0C0) != 0xF080 || rd == 15 || rd == 13 || (b & 15) == 15 || (b & 15) == 13) return std::nullopt;
        i.op = Op::Ext;
        i.ext = static_cast<std::int16_t>(t2::kExtend32 + (a == 0xFA0F ? 0 : a == 0xFA1F ? 1 : a == 0xFA4F ? 2 : 3));
        i.rd = static_cast<std::uint8_t>(rd);
        i.rm = b & 15;
        i.amount = static_cast<std::uint8_t>(8 * ((b >> 4) & 3));
        return i;
    }
    if ((a & 0xFFF0) == 0xFAB0 && (b & 0xF0F0) == 0xF080 && (b & 15) == (a & 15)) {
        i.op = Op::Clz;
        i.rd = static_cast<std::uint8_t>(rd);
        i.rm = a & 15;
        if (rd == 15 || rd == 13 || i.rm == 15 || i.rm == 13) return std::nullopt;
        return i;
    }
    return std::nullopt;
}

// ---- decoding: Thumb ----------------------------------------------------------------------------------------------------------

std::optional<Insn> decode_thumb(std::uint16_t a, std::uint16_t b, std::uint32_t pc_tagged, Arch arch) {
    if (arch >= Arch::V7A && a >= 0xE800) {
        if (auto t = decode_thumb32_int(a, b, pc_tagged)) return t;
    }
    if (arch >= Arch::V6K) {   // the ARMv6 and ARMv7-A additions (decompile/arm_ext: t2)
        if (auto t = t2::decode(a, b, pc_tagged, arch)) return t;
    }
    const std::uint32_t pc = pc_tagged & ~1u;
    Insn i;
    i.word = a;
    i.pc = pc;
    i.isa = Isa::Thumb;
    i.length = 2;
    i.cond = kAlways;
    const unsigned low3 = a & 7, mid3 = (a >> 3) & 7, high3 = (a >> 6) & 7, reg8 = (a >> 8) & 7;
    const auto shift_op = [&](Shift s) {
        i.op = Op::Mov;
        i.s = true;
        i.rd = static_cast<std::uint8_t>(low3);
        i.rm = static_cast<std::uint8_t>(low3);
        i.shift = s;
        i.shift_by_reg = true;
        i.rs = static_cast<std::uint8_t>(mid3);
    };
    const auto set_target = [&](std::int32_t offset) { i.target = (pc + 4 + static_cast<std::uint32_t>(offset)) | 1u; };
    if ((a >> 11) == 3) {   // 2: add and subtract
        i.op = ((a >> 9) & 1) ? Op::Sub : Op::Add;
        i.s = true;
        i.rd = static_cast<std::uint8_t>(low3);
        i.rn = static_cast<std::uint8_t>(mid3);
        if ((a >> 10) & 1) {
            i.imm_form = true;
            i.imm = high3;
        } else {
            i.rm = static_cast<std::uint8_t>(high3);
        }
    } else if ((a >> 13) == 0) {   // 1: move shifted register
        i.op = Op::Mov;
        i.s = true;
        i.rd = static_cast<std::uint8_t>(low3);
        i.rm = static_cast<std::uint8_t>(mid3);
        i.shift = static_cast<Shift>((a >> 11) & 3);
        const unsigned amount = (a >> 6) & 31;
        i.amount = static_cast<std::uint8_t>(amount == 0 && i.shift != Shift::Lsl ? 32 : amount);
    } else if ((a >> 13) == 1) {   // 3: move, compare, add and subtract an immediate
        i.imm_form = true;
        i.imm = a & 0xFF;
        i.s = true;
        switch ((a >> 11) & 3) {
            case 0: i.op = Op::Mov; i.rd = static_cast<std::uint8_t>(reg8); break;
            case 1: i.op = Op::Cmp; i.rn = static_cast<std::uint8_t>(reg8); break;
            case 2: i.op = Op::Add; i.rd = i.rn = static_cast<std::uint8_t>(reg8); break;
            default: i.op = Op::Sub; i.rd = i.rn = static_cast<std::uint8_t>(reg8); break;
        }
    } else if ((a >> 10) == 0x10) {   // 4: ALU operations
        const unsigned code = (a >> 6) & 15;
        i.s = true;
        i.rd = static_cast<std::uint8_t>(low3);
        switch (code) {
            case 0: case 1: case 5: case 6: case 12: case 14: {   // AND EOR ADC SBC ORR BIC
                static constexpr Op ops[16] = {Op::And, Op::Eor, Op::And, Op::And, Op::And, Op::Adc, Op::Sbc, Op::And, Op::And, Op::And, Op::And, Op::And, Op::Orr, Op::And, Op::Bic, Op::And};
                i.op = ops[code];
                i.rn = i.rd;
                i.rm = static_cast<std::uint8_t>(mid3);
                break;
            }
            case 2: shift_op(Shift::Lsl); break;
            case 3: shift_op(Shift::Lsr); break;
            case 4: shift_op(Shift::Asr); break;
            case 7: shift_op(Shift::Ror); break;
            case 8: case 10: case 11:   // TST CMP CMN
                i.op = code == 8 ? Op::Tst : code == 10 ? Op::Cmp : Op::Cmn;
                i.rn = static_cast<std::uint8_t>(low3);
                i.rd = 0;
                i.rm = static_cast<std::uint8_t>(mid3);
                break;
            case 9:   // NEG
                i.op = Op::Rsb;
                i.rn = static_cast<std::uint8_t>(mid3);
                i.imm_form = true;
                break;
            case 13:
                i.op = Op::Mul;
                i.rm = static_cast<std::uint8_t>(mid3);
                i.rs = i.rd;
                break;
            default:   // 15: MVN
                i.op = Op::Mvn;
                i.rm = static_cast<std::uint8_t>(mid3);
                break;
        }
    } else if ((a >> 10) == 0x11) {   // 5: hi register operations and BX
        const unsigned op = (a >> 8) & 3, h1 = (a >> 7) & 1, h2 = (a >> 6) & 1;
        const std::uint8_t hs = static_cast<std::uint8_t>((h2 << 3) | mid3), hd = static_cast<std::uint8_t>((h1 << 3) | low3);
        if (op == 3) {
            if (low3 != 0 || (h1 && arch < Arch::V5TE)) return std::nullopt;
            i.op = h1 ? Op::Blx : Op::Bx;   // (BLX rm: ARMv5)
            i.rm = hs;
        } else {
            if (!h1 && !h2 && (op != 2 || arch < Arch::V6K)) return std::nullopt;   // two low registers: undefined on the ARM7TDMI (a `mov` is fine from ARMv6)
            i.v6 = arch >= Arch::V6K;
            i.op = op == 0 ? Op::Add : op == 1 ? Op::Cmp : Op::Mov;
            i.s = op == 1;
            if (op == 1) i.rn = hd;
            else i.rd = hd;
            if (op == 0) i.rn = hd;
            i.rm = hs;
        }
    } else if ((a >> 11) == 9) {   // 6: PC-relative load
        i.op = Op::Ldr;
        i.rd = static_cast<std::uint8_t>(reg8);
        i.rn = 15;
        i.imm_form = true;
        i.imm = (a & 0xFF) * 4u;
    } else if ((a >> 12) == 5) {   // 7, 8: load and store with a register offset
        i.rd = static_cast<std::uint8_t>(low3);
        i.rn = static_cast<std::uint8_t>(mid3);
        i.rm = static_cast<std::uint8_t>(high3);
        const bool hi = (a >> 11) & 1, lo = (a >> 10) & 1;
        if (!((a >> 9) & 1)) {
            i.op = hi ? Op::Ldr : Op::Str;
            i.size = lo ? 1 : 4;
        } else {
            i.op = (lo || hi) ? Op::Ldr : Op::Str;
            i.size = (lo && !hi) ? 1 : 2;
            i.sign = lo;
        }
    } else if ((a >> 13) == 3) {   // 9: load and store with an immediate offset
        const bool byte = (a >> 12) & 1;
        i.op = ((a >> 11) & 1) ? Op::Ldr : Op::Str;
        i.size = byte ? 1 : 4;
        i.rd = static_cast<std::uint8_t>(low3);
        i.rn = static_cast<std::uint8_t>(mid3);
        i.imm_form = true;
        i.imm = ((a >> 6) & 31) * (byte ? 1u : 4u);
    } else if ((a >> 12) == 8) {   // 10: halfword
        i.op = ((a >> 11) & 1) ? Op::Ldr : Op::Str;
        i.size = 2;
        i.rd = static_cast<std::uint8_t>(low3);
        i.rn = static_cast<std::uint8_t>(mid3);
        i.imm_form = true;
        i.imm = ((a >> 6) & 31) * 2u;
    } else if ((a >> 12) == 9) {   // 11: SP-relative
        i.op = ((a >> 11) & 1) ? Op::Ldr : Op::Str;
        i.rd = static_cast<std::uint8_t>(reg8);
        i.rn = 13;
        i.imm_form = true;
        i.imm = (a & 0xFF) * 4u;
    } else if ((a >> 12) == 10) {   // 12: load an address
        i.op = Op::Add;
        i.rd = static_cast<std::uint8_t>(reg8);
        i.rn = ((a >> 11) & 1) ? 13 : 15;
        i.imm_form = true;
        i.imm = (a & 0xFF) * 4u;
    } else if ((a >> 8) == 0xB0) {   // 13: add to the stack pointer
        i.op = ((a >> 7) & 1) ? Op::Sub : Op::Add;
        i.rd = i.rn = 13;
        i.imm_form = true;
        i.imm = (a & 0x7F) * 4u;
    } else if ((a & 0xF600) == 0xB400) {   // 14: push and pop
        const bool load = (a >> 11) & 1, extra = (a >> 8) & 1;
        i.list = static_cast<std::uint16_t>((a & 0xFF) | (extra ? (load ? 0x8000 : 0x4000) : 0));
        if (i.list == 0) return std::nullopt;
        i.op = load ? Op::Ldm : Op::Stm;
        i.rn = 13;
        i.pre = !load;
        i.up = load;
        i.wb = true;
    } else if ((a >> 12) == 0xB) {
        return std::nullopt;
    } else if ((a >> 12) == 0xC) {   // 15: multiple load and store
        const bool load = (a >> 11) & 1;
        i.list = static_cast<std::uint16_t>(a & 0xFF);
        if (i.list == 0) return std::nullopt;
        i.op = load ? Op::Ldm : Op::Stm;
        i.rn = static_cast<std::uint8_t>(reg8);
        i.pre = false;
        i.up = true;
        i.wb = !(load && (i.list >> reg8) & 1);
    } else if ((a >> 12) == 0xD) {   // 16, 17: conditional branch, software interrupt
        const unsigned c = (a >> 8) & 15;
        if (c == 14) return std::nullopt;
        if (c == 15) {
            i.op = Op::Swi;
            i.imm = a & 0xFF;
        } else {
            i.op = Op::B;
            i.cond = static_cast<std::uint8_t>(c);
            set_target(sext(a & 0xFF, 8) * 2);
        }
    } else if ((a >> 11) == 0x1C) {   // 18: unconditional branch
        i.op = Op::B;
        set_target(sext(a & 0x7FF, 11) * 2);
    } else if ((a >> 12) == 0xF) {   // 19: long branch with link
        if (((a >> 11) & 1)) return std::nullopt;
        const bool blx = arch >= Arch::V5TE && (b >> 11) == 0x1D && (b & 1) == 0;   // the second halfword 11101: BLX, to ARM state
        if ((b >> 11) != 0x1F && !blx) return std::nullopt;
        i.op = blx ? Op::Blx : Op::Bl;
        i.length = 4;
        i.word = static_cast<std::uint32_t>(a) | static_cast<std::uint32_t>(b) << 16;
        set_target((sext(a & 0x7FF, 11) * 4096) + static_cast<std::int32_t>((b & 0x7FF) << 1));
        if (blx) i.target = *i.target & ~3u;   // (a word address, in ARM state)
    } else {
        return std::nullopt;   // (the 0xE800 group: BLX, a Thumb-2 prefix)
    }
    fill_raw(i);
    if (i.length == 4) {
        i.raw[2] = static_cast<std::uint8_t>(b);
        i.raw[3] = static_cast<std::uint8_t>(b >> 8);
    }
    set_flow(i);
    return i;
}

std::optional<Insn> decode(ByteView code, std::size_t at, std::uint32_t pc, Arch arch) {
    if (pc & 1) {
        if (at + 2 > code.size()) return std::nullopt;
        const std::uint16_t a = static_cast<std::uint16_t>(code[at] | code[at + 1] << 8);
        std::uint16_t b = 0;
        if ((a >> 11) == 0x1E || (arch >= Arch::V7A && a >= 0xE800)) {
            if (at + 4 > code.size()) return std::nullopt;
            b = static_cast<std::uint16_t>(code[at + 2] | code[at + 3] << 8);
        }
        return decode_thumb(a, b, pc, arch);
    }
    if (at + 4 > code.size()) return std::nullopt;
    const std::uint32_t w = static_cast<std::uint32_t>(code[at]) | static_cast<std::uint32_t>(code[at + 1]) << 8 |
                            static_cast<std::uint32_t>(code[at + 2]) << 16 | static_cast<std::uint32_t>(code[at + 3]) << 24;
    return decode_arm(w, pc, arch);
}

// ---- registers and conditions -------------------------------------------------------------------------------------------------

std::string_view register_name(unsigned number) { return kRegisters.at(number & 15); }

std::optional<unsigned> register_number(std::string_view t) {
    const std::string s = text::lower(text::trim(t));
    for (unsigned k = 0; k < 16; ++k)
        if (s == kRegisters[k]) return k;
    if (s == "ip") return 12u;
    if (s == "fp") return 11u;
    if (s == "sl") return 10u;
    if (s == "sb") return 9u;
    if (s.size() >= 2 && s[0] == 'r') {
        const auto n = text::parse_int<unsigned>(std::string_view(s).substr(1));
        if (n && *n < 16 && s.size() <= 3) return *n;
    }
    return std::nullopt;
}

std::string_view condition_suffix(unsigned cond) { return kConditions.at(cond % 15); }

namespace {

std::optional<std::uint8_t> condition_number(std::string_view s) {
    for (unsigned k = 0; k < 14; ++k)
        if (s == kConditions[k]) return static_cast<std::uint8_t>(k);
    if (s == "hs") return std::uint8_t{2};
    if (s == "lo") return std::uint8_t{3};
    if (s == "al") return kAlways;
    return std::nullopt;
}

// ---- text ---------------------------------------------------------------------------------------------------------------------

std::string imm_text(std::uint32_t v) { return v < 10 ? std::to_string(v) : std::format("0x{:X}", v); }

std::string signed_imm_text(bool up, std::uint32_t v) { return std::string("#") + (up ? "" : "-") + imm_text(v); }

std::string target_text(std::uint32_t target, const AddressLabels& labels) {
    const auto it = labels.find(target & ~1u);
    if (it != labels.end() && !it->second.empty()) return it->second;
    return std::format("0x{:08X}", target & ~1u);
}

std::string shift_suffix(const Insn& i) {
    if (i.shift_by_reg) return std::format(", {} {}", kShifts[static_cast<unsigned>(i.shift)], register_name(i.rs));
    if (i.rrx) return ", rrx";
    if (i.shift == Shift::Lsl && i.amount == 0) return "";
    return std::format(", {} #{}", kShifts[static_cast<unsigned>(i.shift)], i.amount);
}

std::string register_text(const Insn& i) { return std::string(register_name(i.rm)) + shift_suffix(i); }

std::string operand2(const Insn& i) {
    if (!i.imm_form) return register_text(i);
    const std::optional<unsigned> least = minimal_rotate(i.imm);
    if (i.thumb() || (least && *least == i.rot)) return "#" + imm_text(i.imm);
    return std::format("#{}, {}", imm_text(rotate_left(i.imm, 2u * i.rot) & 0xFF), 2 * i.rot);
}

std::string list_text(unsigned list) {
    std::string out;
    for (unsigned r = 0; r < 16; ++r) {
        if (!((list >> r) & 1)) continue;
        unsigned end = r;
        while (end + 1 < 13 && ((list >> (end + 1)) & 1)) ++end;   // (a range is of r0-r12: sp, lr and pc are named)
        if (!out.empty()) out += ", ";
        if (end - r >= 2) {
            out += std::format("{}-{}", register_name(r), register_name(end));
            r = end;
        } else {
            out += register_name(r);
        }
    }
    return "{" + out + "}";
}

std::string psr_fields(const Insn& i) {
    std::string out = i.spsr ? "spsr_" : "cpsr_";
    if (i.fields & 8) out += 'f';
    if (i.fields & 4) out += 's';
    if (i.fields & 2) out += 'x';
    if (i.fields & 1) out += 'c';
    return out;
}

// The address a PC-relative access reads: the program counter as the state sees it (the Thumb one word-aligned), plus the offset.
std::uint32_t pc_relative(const Insn& i) {
    const std::uint32_t base = i.thumb() ? ((i.pc + 4) & ~3u) : i.pc + 8;
    return i.up ? base + i.imm : base - i.imm;
}

std::string memory_operand(const Insn& i, const AddressLabels& labels) {
    const std::string base(register_name(i.rn));
    if (i.rn == 15 && i.imm_form && i.pre && !i.wb && !i.user && i.size == 4 && !i.sign && (i.up || i.imm != 0)) {
        const auto it = labels.find(pc_relative(i));
        if (it != labels.end() && !it->second.empty()) return it->second;
    }
    std::string offset;
    if (i.imm_form) {
        if (!i.pre || i.imm != 0 || !i.up) offset = ", " + signed_imm_text(i.up, i.imm);
    } else {
        offset = std::format(", {}{}{}", i.up ? "" : "-", register_name(i.rm), shift_suffix(i));
    }
    if (i.pre) return "[" + base + offset + "]" + (i.wb ? "!" : "");
    return "[" + base + "]" + offset;
}

std::string arm_text(const Insn& i, const AddressLabels& labels) {
    const std::string cond(condition_suffix(i.cond));
    if (is_data(i.op)) {
        const std::string name(kDataNames[static_cast<unsigned>(i.op)]);
        if (i.op == Op::Mov && i.cond == kAlways && !i.imm_form && !i.s && i.rd == 0 && i.rm == 0 && !i.shift_by_reg && i.shift == Shift::Lsl && i.amount == 0 && !i.rrx)
            return "nop";
        const std::string head = name + (i.s && !is_test(i.op) ? "s" : "") + cond + " ";
        if (is_test(i.op)) return std::format("{}{}, {}", head, register_name(i.rn), operand2(i));
        if (i.op == Op::Mov || i.op == Op::Mvn) return std::format("{}{}, {}", head, register_name(i.rd), operand2(i));
        return std::format("{}{}, {}, {}", head, register_name(i.rd), register_name(i.rn), operand2(i));
    }
    switch (i.op) {
        case Op::Mul: return std::format("mul{}{} {}, {}, {}", i.s ? "s" : "", cond, register_name(i.rd), register_name(i.rm), register_name(i.rs));
        case Op::Mla: return std::format("mla{}{} {}, {}, {}, {}", i.s ? "s" : "", cond, register_name(i.rd), register_name(i.rm), register_name(i.rs), register_name(i.rn));
        case Op::Umull: case Op::Umlal: case Op::Smull: case Op::Smlal: {
            const char* name = i.op == Op::Umull ? "umull" : i.op == Op::Umlal ? "umlal" : i.op == Op::Smull ? "smull" : "smlal";
            return std::format("{}{}{} {}, {}, {}, {}", name, i.s ? "s" : "", cond, register_name(i.rd), register_name(i.rn), register_name(i.rm), register_name(i.rs));
        }
        case Op::Ldr: case Op::Str: {
            std::string name = i.op == Op::Ldr ? "ldr" : "str";
            if (i.size == 8) name += "d";
            else if (i.sign) name += i.size == 1 ? "sb" : "sh";
            else if (i.size == 2) name += "h";
            else if (i.size == 1) name += "b";
            if (i.user) name += "t";
            if (i.size == 8) return std::format("{}{} {}, {}, {}", name, cond, register_name(i.rd), register_name(i.rd + 1), memory_operand(i, labels));
            return std::format("{}{} {}, {}", name, cond, register_name(i.rd), memory_operand(i, labels));
        }
        case Op::Ldm: case Op::Stm: {
            const bool load = i.op == Op::Ldm;
            if (i.rn == 13 && i.wb && !i.hat && ((!load && i.pre && !i.up) || (load && !i.pre && i.up)))
                return std::format("{}{} {}", load ? "pop" : "push", cond, list_text(i.list));
            const char* mode = i.pre ? (i.up ? "ib" : "db") : (i.up ? "ia" : "da");
            return std::format("{}{}{} {}{}, {}{}", load ? "ldm" : "stm", mode, cond, register_name(i.rn), i.wb ? "!" : "", list_text(i.list), i.hat ? "^" : "");
        }
        case Op::Swp: return std::format("swp{}{} {}, {}, [{}]", i.size == 1 ? "b" : "", cond, register_name(i.rd), register_name(i.rm), register_name(i.rn));
        case Op::B: return std::format("b{} {}", cond, target_text(*i.target, labels));
        case Op::Bl: return std::format("bl{} {}", cond, target_text(*i.target, labels));
        case Op::Bx: return std::format("bx{} {}", cond, register_name(i.rm));
        case Op::Blx: return i.target ? std::format("blx {}", target_text(*i.target, labels)) : std::format("blx{} {}", cond, register_name(i.rm));
        case Op::Clz: return std::format("clz{} {}, {}", cond, register_name(i.rd), register_name(i.rm));
        case Op::Qadd: case Op::Qsub: case Op::Qdadd: case Op::Qdsub: {
            static constexpr const char* names[] = {"qadd", "qsub", "qdadd", "qdsub"};
            return std::format("{}{} {}, {}, {}", names[static_cast<unsigned>(i.op) - static_cast<unsigned>(Op::Qadd)], cond, register_name(i.rd), register_name(i.rm), register_name(i.rn));
        }
        case Op::Smla: return std::format("smla{}{}{} {}, {}, {}, {}", (i.xy & 1) ? 't' : 'b', (i.xy & 2) ? 't' : 'b', cond, register_name(i.rd), register_name(i.rm), register_name(i.rs), register_name(i.rn));
        case Op::Smul: return std::format("smul{}{}{} {}, {}, {}", (i.xy & 1) ? 't' : 'b', (i.xy & 2) ? 't' : 'b', cond, register_name(i.rd), register_name(i.rm), register_name(i.rs));
        case Op::Smlalxy: return std::format("smlal{}{}{} {}, {}, {}, {}", (i.xy & 1) ? 't' : 'b', (i.xy & 2) ? 't' : 'b', cond, register_name(i.rd), register_name(i.rn), register_name(i.rm), register_name(i.rs));
        case Op::Smlaw: return std::format("smlaw{}{} {}, {}, {}, {}", (i.xy & 2) ? 't' : 'b', cond, register_name(i.rd), register_name(i.rm), register_name(i.rs), register_name(i.rn));
        case Op::Smulw: return std::format("smulw{}{} {}, {}, {}", (i.xy & 2) ? 't' : 'b', cond, register_name(i.rd), register_name(i.rm), register_name(i.rs));
        case Op::Bkpt: return std::format("bkpt 0x{:X}", i.imm);
        case Op::Pld: return "pld " + memory_operand(i, labels);
        case Op::Mcr: case Op::Mrc: return std::format("{}{} p{}, {}, {}, c{}, c{}, {}", i.op == Op::Mcr ? "mcr" : "mrc", cond, i.cp, i.opc1, register_name(i.rd), i.crn, i.crm, i.opc2);
        case Op::Swi: return std::format("swi{} 0x{:X}", cond, i.imm);
        case Op::Mrs: return std::format("mrs{} {}, {}", cond, register_name(i.rd), i.spsr ? "spsr" : "cpsr");
        case Op::Msr: return std::format("msr{} {}, {}", cond, psr_fields(i), operand2(i));
        default: return "?";
    }
}

std::string thumb_text(const Insn& i, const AddressLabels& labels) {
    const auto reg = [](unsigned r) { return std::string(register_name(r)); };
    const std::string shift_name(kShifts[static_cast<unsigned>(i.shift)]);
    switch (i.op) {
        case Op::Mov:
            if (i.imm_form) return std::format("movs {}, #{}", reg(i.rd), imm_text(i.imm));
            if (i.shift_by_reg) return std::format("{}s {}, {}", shift_name, reg(i.rd), reg(i.rs));
            if (!(i.shift == Shift::Lsl && i.amount == 0)) return std::format("{}s {}, {}, #{}", shift_name, reg(i.rd), reg(i.rm), i.amount);
            if (i.rd == 8 && i.rm == 8 && !i.s) return "nop";
            return std::format("mov{} {}, {}", i.s ? "s" : "", reg(i.rd), reg(i.rm));
        case Op::Add: case Op::Sub: {
            const char* name = i.op == Op::Add ? "add" : "sub";
            if (i.imm_form) {
                if (i.rd == 13 && i.rn == 13) return std::format("{} sp, #{}", name, imm_text(i.imm));
                if (i.rn == 13 || i.rn == 15) return std::format("{} {}, {}, #{}", name, reg(i.rd), reg(i.rn), imm_text(i.imm));
                if ((i.word >> 13) == 1) return std::format("{}s {}, #{}", name, reg(i.rd), imm_text(i.imm));
                return std::format("{}s {}, {}, #{}", name, reg(i.rd), reg(i.rn), imm_text(i.imm));
            }
            if (!i.s) return std::format("{} {}, {}", name, reg(i.rd), reg(i.rm));
            return std::format("{}s {}, {}, {}", name, reg(i.rd), reg(i.rn), reg(i.rm));
        }
        case Op::And: case Op::Eor: case Op::Adc: case Op::Sbc: case Op::Orr: case Op::Bic: case Op::Mvn:
            return std::format("{}s {}, {}", kDataNames[static_cast<unsigned>(i.op)], reg(i.rd), reg(i.rm));
        case Op::Tst: case Op::Cmn: return std::format("{} {}, {}", kDataNames[static_cast<unsigned>(i.op)], reg(i.rn), reg(i.rm));
        case Op::Cmp: return i.imm_form ? std::format("cmp {}, #{}", reg(i.rn), imm_text(i.imm)) : std::format("cmp {}, {}", reg(i.rn), reg(i.rm));
        case Op::Rsb: return std::format("negs {}, {}", reg(i.rd), reg(i.rn));
        case Op::Mul: return std::format("muls {}, {}, {}", reg(i.rd), reg(i.rm), reg(i.rs));
        case Op::Ldr: case Op::Str: return arm_text(i, labels);
        case Op::Ldm: case Op::Stm: {
            const bool load = i.op == Op::Ldm;
            if (i.rn == 13 && i.wb && ((!load && i.pre && !i.up) || (load && !i.pre && i.up))) return std::format("{} {}", load ? "pop" : "push", list_text(i.list));
            return std::format("{}ia {}{}, {}", load ? "ldm" : "stm", reg(i.rn), i.wb ? "!" : "", list_text(i.list));
        }
        case Op::B: return std::format("b{}{} {}", condition_suffix(i.cond), i.length == 4 ? ".w" : "", target_text(*i.target, labels));
        case Op::Bl: return std::format("bl {}", target_text(*i.target, labels));
        case Op::Bx: return std::format("bx {}", reg(i.rm));
        case Op::Blx: return i.target ? std::format("blx {}", target_text(*i.target, labels)) : std::format("blx {}", reg(i.rm));
        case Op::Swi: return std::format("swi 0x{:X}", i.imm);
        default: return "?";
    }
}

}  // namespace

std::string text(const Insn& insn, const AddressLabels& labels) {
    if (insn.thumb() && insn.length == 4 && insn.ext == t2::kOpaque) return t2::text(insn, labels);   // (a 32-bit instruction the interpreter runs, written as the disassembler does)
    if (insn.op == Op::Ext) return insn.thumb() ? t2::text(insn, labels) : ext::text(insn.ext, insn.word, insn.pc, labels);
    return insn.thumb() ? thumb_text(insn, labels) : arm_text(insn, labels);
}

// ---- expressions --------------------------------------------------------------------------------------------------------------

namespace {

class Expr {
public:
    Expr(std::string_view text, const NameLabels& labels, std::uint32_t pc) : s_(text), labels_(labels), pc_(pc) {}

    std::int64_t run() {
        const std::int64_t v = bit_or();
        skip();
        if (i_ != s_.size()) fail("unexpected '" + std::string(s_.substr(i_)) + "'");
        return v;
    }

private:
    [[noreturn]] void fail(const std::string& why) const { throw ParseError("", 0, 0, std::string(s_) + " (" + why + ")"); }
    void skip() {
        while (i_ < s_.size() && text::is_space(s_[i_])) ++i_;
    }
    bool eat(std::string_view token) {
        skip();
        if (s_.substr(i_, token.size()) == token) {
            i_ += token.size();
            return true;
        }
        return false;
    }
    bool eat_op(char c) {
        skip();
        if (i_ < s_.size() && s_[i_] == c && !(i_ + 1 < s_.size() && (s_[i_ + 1] == c && (c == '<' || c == '>')))) {
            ++i_;
            return true;
        }
        return false;
    }

    std::int64_t bit_or() {
        std::int64_t v = bit_xor();
        while (eat_op('|')) v |= bit_xor();
        return v;
    }
    std::int64_t bit_xor() {
        std::int64_t v = bit_and();
        while (eat_op('^')) v ^= bit_and();
        return v;
    }
    std::int64_t bit_and() {
        std::int64_t v = shift();
        while (eat_op('&')) v &= shift();
        return v;
    }
    std::int64_t shift() {
        std::int64_t v = sum();
        for (;;) {
            if (eat("<<")) {
                const std::int64_t n = sum();
                if (n < 0 || n > 62) fail("shift out of range");
                v <<= n;
            } else if (eat(">>")) {
                const std::int64_t n = sum();
                if (n < 0 || n > 62) fail("shift out of range");
                v >>= n;
            } else {
                return v;
            }
        }
    }
    std::int64_t sum() {
        std::int64_t v = product();
        for (;;) {
            if (eat_op('+')) v += product();
            else if (eat_op('-')) v -= product();
            else return v;
        }
    }
    std::int64_t product() {
        std::int64_t v = unary();
        for (;;) {
            if (eat_op('*')) {
                v *= unary();
            } else if (eat_op('/')) {
                const std::int64_t d = unary();
                if (d == 0) fail("division by zero");
                v /= d;
            } else {
                return v;
            }
        }
    }
    std::int64_t unary() {
        skip();
        if (eat("-")) return -unary();
        if (eat("+")) return unary();
        if (eat("~")) return ~unary();
        return primary();
    }
    static bool name_start(char c) { return std::isalpha(static_cast<unsigned char>(c)) || c == '_' || c == '.' || c == '$'; }
    static bool name_char(char c) { return name_start(c) || std::isdigit(static_cast<unsigned char>(c)); }
    std::int64_t primary() {
        skip();
        if (i_ >= s_.size()) fail("an operand is missing");
        if (eat("(")) {
            const std::int64_t v = bit_or();
            if (!eat(")")) fail("a ')' is missing");
            return v;
        }
        const char c = s_[i_];
        if (std::isdigit(static_cast<unsigned char>(c))) return number();
        if (c == '.' && (i_ + 1 >= s_.size() || !name_char(s_[i_ + 1]))) {
            ++i_;
            return pc_;   // `.` is here
        }
        if (name_start(c)) {
            const std::size_t from = i_;
            while (i_ < s_.size() && name_char(s_[i_])) ++i_;
            const std::string_view name = s_.substr(from, i_ - from);
            const auto it = labels_.find(name);
            if (it == labels_.end()) throw z80::UnknownLabel(std::string(name));
            return it->second;
        }
        fail(std::string("unexpected '") + c + "'");
    }
    std::int64_t number() {
        const std::size_t from = i_;
        int base = 10;
        if (s_.substr(i_, 2) == "0x" || s_.substr(i_, 2) == "0X") {
            base = 16;
            i_ += 2;
        } else if (s_.substr(i_, 2) == "0b" || s_.substr(i_, 2) == "0B") {
            base = 2;
            i_ += 2;
        }
        const std::size_t digits = i_;
        while (i_ < s_.size() && std::isxdigit(static_cast<unsigned char>(s_[i_])) && (base == 16 || std::isdigit(static_cast<unsigned char>(s_[i_])))) ++i_;
        const auto v = text::parse_int<std::uint64_t>(s_.substr(digits, i_ - digits), base);
        if (!v || i_ == digits) fail("'" + std::string(s_.substr(from, i_ - from)) + "' is not a number");
        return static_cast<std::int64_t>(*v);
    }

    std::string_view s_;
    const NameLabels& labels_;
    std::uint32_t pc_;
    std::size_t i_ = 0;
};

}  // namespace

std::int64_t value(std::string_view expr, const NameLabels& labels, std::uint32_t pc) { return Expr(expr, labels, pc).run(); }

// ---- encoding -----------------------------------------------------------------------------------------------------------------

namespace {

[[noreturn]] void refuse(std::string_view line, const std::string& why) { throw ParseError("", 0, 0, std::string(line) + " (" + why + ")"); }
[[noreturn]] void refuse(const Insn& i, const std::string& why) { refuse(text(i), why); }

Bytes bytes_of(std::uint32_t word, unsigned length) {
    Bytes out;
    for (unsigned k = 0; k < length; ++k) out.push_back(static_cast<std::uint8_t>(word >> (8 * k)));
    return out;
}

std::uint32_t shifter(const Insn& i) {
    const std::uint32_t type = static_cast<std::uint32_t>(i.shift) << 5;
    if (i.shift_by_reg) return static_cast<std::uint32_t>(i.rs) << 8 | type | 0x10 | i.rm;
    std::uint32_t amount = i.amount;
    if (i.rrx) {
        amount = 0;
    } else if (i.shift == Shift::Lsl) {
        if (amount > 31) refuse(i, "a shift of 32 or more");
    } else if (i.shift == Shift::Ror) {
        if (amount < 1 || amount > 31) refuse(i, "a rotate by " + std::to_string(amount));
    } else {
        if (amount < 1 || amount > 32) refuse(i, "a shift by " + std::to_string(amount));
        if (amount == 32) amount = 0;
    }
    return amount << 7 | type | i.rm;
}

// The rotate and 8 bits of an immediate (the instruction's, or the one a MSR takes).
std::uint32_t immediate12(const Insn& i) {
    if (i.rot_fixed) {
        const std::uint32_t imm8 = rotate_left(i.imm, 2u * i.rot);
        if (imm8 > 0xFF) refuse(i, "the rotated immediate is not 8 bits");
        return static_cast<std::uint32_t>(i.rot) << 8 | imm8;
    }
    const std::optional<unsigned> rot = minimal_rotate(i.imm);
    if (!rot) refuse(i, std::format("the immediate 0x{:X} is not an 8-bit value rotated by an even amount", i.imm));
    return *rot << 8 | (rotate_left(i.imm, 2 * *rot) & 0xFF);
}

std::uint32_t branch_offset24(const Insn& i) {
    if (!i.target) refuse(i, "a branch without a target");
    const std::int64_t distance = static_cast<std::int64_t>(*i.target & ~1u) - (static_cast<std::int64_t>(i.pc) + 8);
    if (distance % 4 != 0) refuse(i, "a branch target is not a word");
    if (distance / 4 < -(1 << 23) || distance / 4 >= (1 << 23)) refuse(i, "the branch is out of reach");
    return static_cast<std::uint32_t>(distance / 4) & 0xFFFFFF;
}

std::uint32_t encode_arm(const Insn& i) {
    std::uint32_t w = static_cast<std::uint32_t>(i.cond) << 28;
    if (i.op == Op::Blx && i.target) {   // BLX label: the unconditional space, the H bit the target's halfword
        const std::int64_t distance = static_cast<std::int64_t>(*i.target & ~1u) - (static_cast<std::int64_t>(i.pc) + 8);
        if (distance % 2 != 0) refuse(i, "a branch target is not a halfword");
        if (distance / 4 < -(1 << 23) || distance / 4 >= (1 << 23)) refuse(i, "the branch is out of reach");
        return 0xFA000000u | ((static_cast<std::uint32_t>(distance) >> 1) & 1) << 24 | ((static_cast<std::uint32_t>(distance) >> 2) & 0xFFFFFF);
    }
    if (is_data(i.op)) {
        const bool test = is_test(i.op);
        w |= static_cast<std::uint32_t>(i.op) << 21 | ((i.s || test) ? 1u << 20 : 0u);
        if (!(i.op == Op::Mov || i.op == Op::Mvn)) w |= static_cast<std::uint32_t>(i.rn) << 16;
        if (!test) w |= static_cast<std::uint32_t>(i.rd) << 12;
        w |= i.imm_form ? (1u << 25 | immediate12(i)) : shifter(i);
        return w;
    }
    switch (i.op) {
        case Op::Mul: case Op::Mla:
            return w | (i.op == Op::Mla ? 1u << 21 : 0u) | (i.s ? 1u << 20 : 0u) | static_cast<std::uint32_t>(i.rd) << 16 |
                   (i.op == Op::Mla ? static_cast<std::uint32_t>(i.rn) << 12 : 0u) | static_cast<std::uint32_t>(i.rs) << 8 | 0x90 | i.rm;
        case Op::Umull: case Op::Umlal: case Op::Smull: case Op::Smlal: {
            const bool is_signed = i.op == Op::Smull || i.op == Op::Smlal, accumulate = i.op == Op::Umlal || i.op == Op::Smlal;
            return w | 1u << 23 | (is_signed ? 1u << 22 : 0u) | (accumulate ? 1u << 21 : 0u) | (i.s ? 1u << 20 : 0u) | static_cast<std::uint32_t>(i.rn) << 16 |
                   static_cast<std::uint32_t>(i.rd) << 12 | static_cast<std::uint32_t>(i.rs) << 8 | 0x90 | i.rm;
        }
        case Op::Ldr: case Op::Str: {
            const bool load = i.op == Op::Ldr;
            const bool pair = i.size == 8;   // LDRD, STRD: the load bit is clear, the S and H bits say which
            const bool halfword_class = i.sign || i.size == 2 || pair;
            if (!i.pre && !i.wb) refuse(i, "a post-indexed access writes the base back");
            if (pair && ((i.rd & 1) || i.rd == 14)) refuse(i, "a doubleword access starts at an even register below lr");
            w |= (i.pre ? 1u << 24 : 0u) | (i.up ? 1u << 23 : 0u) | (load && !pair ? 1u << 20 : 0u) | static_cast<std::uint32_t>(i.rn) << 16 | static_cast<std::uint32_t>(i.rd) << 12;
            if (halfword_class) {
                if (i.user) refuse(i, "no unprivileged halfword access");
                if (!load && (i.sign || (i.size != 2 && !pair))) refuse(i, "a signed store");
                w |= 0x90 | (i.pre && i.wb ? 1u << 21 : 0u);
                w |= pair ? (load ? 2u << 5 : 3u << 5) : i.sign ? (i.size == 1 ? 2u << 5 : 3u << 5) : 1u << 5;
                if (i.imm_form) {
                    if (i.imm > 255) refuse(i, "the offset does not fit 8 bits");
                    return w | 1u << 22 | (i.imm & 0xF0) << 4 | (i.imm & 15);
                }
                if (i.shift != Shift::Lsl || i.amount != 0 || i.rrx || i.shift_by_reg) refuse(i, "a halfword access takes no shifted register");
                return w | i.rm;
            }
            if (i.size == 1) w |= 1u << 22;
            if (i.pre ? i.wb : i.user) w |= 1u << 21;
            if (i.imm_form) {
                if (i.imm > 0xFFF) refuse(i, "the offset does not fit 12 bits");
                return w | (1u << 26) | i.imm;
            }
            return w | (3u << 25) | shifter(i);
        }
        case Op::Ldm: case Op::Stm:
            return w | 4u << 25 | (i.pre ? 1u << 24 : 0u) | (i.up ? 1u << 23 : 0u) | (i.hat ? 1u << 22 : 0u) | (i.wb ? 1u << 21 : 0u) | (i.op == Op::Ldm ? 1u << 20 : 0u) |
                   static_cast<std::uint32_t>(i.rn) << 16 | i.list;
        case Op::Swp: return w | 0x01000090 | (i.size == 1 ? 1u << 22 : 0u) | static_cast<std::uint32_t>(i.rn) << 16 | static_cast<std::uint32_t>(i.rd) << 12 | i.rm;
        case Op::B: return w | 0x0A000000 | branch_offset24(i);
        case Op::Bl: return w | 0x0B000000 | branch_offset24(i);
        case Op::Bx: return w | 0x012FFF10 | i.rm;
        case Op::Blx: return w | 0x012FFF30 | i.rm;
        case Op::Clz: return w | 0x016F0F10 | static_cast<std::uint32_t>(i.rd) << 12 | i.rm;
        case Op::Qadd: case Op::Qsub: case Op::Qdadd: case Op::Qdsub:
            return w | 0x01000050 | (static_cast<std::uint32_t>(i.op) - static_cast<std::uint32_t>(Op::Qadd)) << 21 | static_cast<std::uint32_t>(i.rn) << 16 |
                   static_cast<std::uint32_t>(i.rd) << 12 | i.rm;
        case Op::Smla: case Op::Smul: case Op::Smlalxy: {
            const std::uint32_t kind = i.op == Op::Smla ? 0u : i.op == Op::Smlalxy ? 2u : 3u;
            const bool wide = i.op == Op::Smlalxy;   // (rd: low, rn: high; the encoding has the high word first)
            return w | 0x01000080 | kind << 21 | static_cast<std::uint32_t>(wide ? i.rn : i.rd) << 16 | (i.op == Op::Smul ? 0u : static_cast<std::uint32_t>(wide ? i.rd : i.rn) << 12) |
                   static_cast<std::uint32_t>(i.rs) << 8 | (i.xy >> 1 & 1u) << 6 | (i.xy & 1u) << 5 | i.rm;
        }
        case Op::Smlaw: case Op::Smulw:
            return w | 0x01200080 | (i.op == Op::Smulw ? 0x20u : 0u) | static_cast<std::uint32_t>(i.rd) << 16 | (i.op == Op::Smulw ? 0u : static_cast<std::uint32_t>(i.rn) << 12) |
                   static_cast<std::uint32_t>(i.rs) << 8 | (i.xy >> 1 & 1u) << 6 | i.rm;
        case Op::Bkpt: return w | 0x01200070 | (i.imm & 0xFFF0) << 4 | (i.imm & 15);
        case Op::Pld: {
            if (i.imm_form) {
                if (i.imm > 0xFFF) refuse(i, "the offset does not fit 12 bits");
                return 0xF550F000u | (i.up ? 1u << 23 : 0u) | static_cast<std::uint32_t>(i.rn) << 16 | i.imm;
            }
            return 0xF750F000u | (i.up ? 1u << 23 : 0u) | static_cast<std::uint32_t>(i.rn) << 16 | shifter(i);
        }
        case Op::Mcr: case Op::Mrc:
            return w | 0x0E000010 | static_cast<std::uint32_t>(i.opc1 & 7) << 21 | (i.op == Op::Mrc ? 1u << 20 : 0u) | static_cast<std::uint32_t>(i.crn) << 16 |
                   static_cast<std::uint32_t>(i.rd) << 12 | static_cast<std::uint32_t>(i.cp) << 8 | static_cast<std::uint32_t>(i.opc2 & 7) << 5 | i.crm;
        case Op::Swi:
            if (i.imm > 0xFFFFFF) refuse(i, "the comment field is 24 bits");
            return w | 0x0F000000 | i.imm;
        case Op::Mrs: return w | 0x010F0000 | (i.spsr ? 1u << 22 : 0u) | static_cast<std::uint32_t>(i.rd) << 12;
        case Op::Msr:
            w |= 0x0120F000 | (i.spsr ? 1u << 22 : 0u) | static_cast<std::uint32_t>(i.fields) << 16;
            return i.imm_form ? (w | 1u << 25 | immediate12(i)) : (w | i.rm);
        default: refuse(i, "not an ARM instruction");
    }
}

// Thumb: the format is chosen from the fields, the way GNU as does it. Each operand has to fit what the format has room for.
Bytes encode_thumb(const Insn& i) {
    if (i.cond != kAlways && i.op != Op::B) refuse(i, "no conditional execution in Thumb state");
    const auto low = [](unsigned r) { return r < 8; };
    const auto two = [](unsigned h) { return bytes_of(h, 2); };
    const bool plain = !i.shift_by_reg && !i.rrx && i.shift == Shift::Lsl && i.amount == 0;
    const auto word_offset = [&](std::uint32_t limit, unsigned scale) {
        if (i.imm % scale != 0 || i.imm > limit) refuse(i, std::format("the offset {} does not fit", i.imm));
        return i.imm / scale;
    };
    const auto need = [&](bool ok, const char* why) {
        if (!ok) refuse(i, why);
    };
    const auto hi_register = [&](unsigned op, unsigned rd, unsigned rm) {
        return two(0x4400 | op << 8 | (rd >> 3) << 7 | (rm >> 3) << 6 | (rm & 7) << 3 | (rd & 7));
    };
    const auto alu = [&](unsigned code, unsigned rd, unsigned rm) { return two(0x4000 | code << 6 | rm << 3 | rd); };
    switch (i.op) {
        case Op::Mov:
            if (i.imm_form) {
                need(i.s && low(i.rd) && i.imm <= 255, "a Thumb `movs` takes a low register and an 8-bit immediate");
                return two(0x2000 | i.rd << 8 | i.imm);
            }
            if (i.shift_by_reg) {
                need(i.s && i.rd == i.rm && low(i.rd) && low(i.rs) && !i.rrx, "a Thumb shift by a register takes low registers and sets the flags");
                const unsigned code = i.shift == Shift::Lsl ? 2 : i.shift == Shift::Lsr ? 3 : i.shift == Shift::Asr ? 4 : 7;
                return alu(code, i.rd, i.rs);
            }
            if (plain && !i.s) {
                need(!low(i.rd) || !low(i.rm) || i.v6, "a Thumb `mov` between low registers sets the flags (`movs`)");
                return hi_register(2, i.rd, i.rm);
            }
            need(i.s && low(i.rd) && low(i.rm) && !i.rrx && i.shift != Shift::Ror, "a Thumb shift takes low registers, sets the flags, and is LSL, LSR or ASR");
            if (i.shift == Shift::Lsl) need(i.amount < 32, "a shift of 32 or more");
            else need(i.amount >= 1 && i.amount <= 32, "a shift by 0");
            return two(static_cast<unsigned>(i.shift) << 11 | (i.amount & 31) << 6 | i.rm << 3 | i.rd);
        case Op::Add: case Op::Sub: {
            const bool add = i.op == Op::Add;
            if (i.imm_form) {
                if (i.rn == 13 && i.rd == 13 && !i.s) return two((add ? 0xB000 : 0xB080) | word_offset(508, 4));
                if (add && !i.s && (i.rn == 15 || i.rn == 13)) {
                    need(low(i.rd), "a low register");
                    return two((i.rn == 13 ? 0xA800 : 0xA000) | i.rd << 8 | word_offset(1020, 4));
                }
                need(i.s && low(i.rd) && low(i.rn), "a Thumb add or subtract of an immediate takes low registers and sets the flags");
                if (i.rd == i.rn && i.imm <= 255) return two((add ? 0x3000 : 0x3800) | i.rd << 8 | i.imm);
                need(i.imm <= 7, "the immediate does not fit 3 bits");
                return two((add ? 0x1C00 : 0x1E00) | i.imm << 6 | i.rn << 3 | i.rd);
            }
            need(plain, "a Thumb add or subtract takes no shifted register");
            if (i.s) {
                need(low(i.rd) && low(i.rn) && low(i.rm), "low registers");
                return two((add ? 0x1800 : 0x1A00) | i.rm << 6 | i.rn << 3 | i.rd);
            }
            need(add && i.rd == i.rn && (!low(i.rd) || !low(i.rm)), "a Thumb `add` of registers sets the flags unless one is a high register");
            return hi_register(0, i.rd, i.rm);
        }
        case Op::And: case Op::Eor: case Op::Adc: case Op::Sbc: case Op::Orr: case Op::Bic: {
            need(i.s && !i.imm_form && plain && i.rd == i.rn && low(i.rd) && low(i.rm), "a Thumb data-processing instruction takes two low registers and sets the flags");
            static constexpr unsigned code[16] = {0, 1, 0, 0, 0, 5, 6, 0, 0, 0, 0, 0, 12, 0, 14, 0};
            return alu(code[static_cast<unsigned>(i.op)], i.rd, i.rm);
        }
        case Op::Mvn:
            need(i.s && !i.imm_form && plain && low(i.rd) && low(i.rm), "a Thumb `mvns` takes low registers");
            return alu(15, i.rd, i.rm);
        case Op::Tst: case Op::Cmn:
            need(!i.imm_form && plain && low(i.rn) && low(i.rm), "a Thumb test takes low registers");
            return alu(i.op == Op::Tst ? 8 : 11, i.rn, i.rm);
        case Op::Cmp:
            if (i.imm_form) {
                need(low(i.rn) && i.imm <= 255, "a Thumb `cmp` takes a low register and an 8-bit immediate");
                return two(0x2800 | i.rn << 8 | i.imm);
            }
            need(plain, "a Thumb compare takes no shifted register");
            if (low(i.rn) && low(i.rm)) return alu(10, i.rn, i.rm);
            return hi_register(1, i.rn, i.rm);
        case Op::Rsb:
            need(i.s && i.imm_form && i.imm == 0 && low(i.rd) && low(i.rn), "only `negs rd, rs` reverse-subtracts in Thumb");
            return alu(9, i.rd, i.rn);
        case Op::Mul: {
            need(i.s && low(i.rd) && low(i.rm) && low(i.rs) && (i.rs == i.rd || i.rm == i.rd), "a Thumb `muls rd, rm, rd` takes low registers");
            return alu(13, i.rd, i.rs == i.rd ? i.rm : i.rs);
        }
        case Op::Ldr: case Op::Str: {
            const bool load = i.op == Op::Ldr;
            need(i.pre && i.up && !i.wb && !i.user, "a Thumb access is a plain positive offset");
            if (i.rn == 15) {
                need(load && i.size == 4 && !i.sign && i.imm_form && low(i.rd), "only a word load is PC-relative in Thumb");
                return two(0x4800 | i.rd << 8 | word_offset(1020, 4));
            }
            if (i.rn == 13) {
                need(i.size == 4 && i.imm_form && low(i.rd), "only a word access is SP-relative in Thumb");
                return two(0x9000 | (load ? 0x0800u : 0u) | i.rd << 8 | word_offset(1020, 4));
            }
            need(low(i.rd) && low(i.rn), "a Thumb access takes low registers");
            if (!i.imm_form) {
                need(low(i.rm) && plain, "a Thumb register offset is a low register");
                if (i.size == 4) return two(0x5000 | (load ? 0x0800u : 0u) | i.rm << 6 | i.rn << 3 | i.rd);
                if (i.size == 1 && !i.sign) return two(0x5400 | (load ? 0x0800u : 0u) | i.rm << 6 | i.rn << 3 | i.rd);
                if (i.size == 2 && !i.sign) return two((load ? 0x5A00u : 0x5200u) | i.rm << 6 | i.rn << 3 | i.rd);
                need(load, "a signed store");
                return two((i.size == 1 ? 0x5600u : 0x5E00u) | i.rm << 6 | i.rn << 3 | i.rd);
            }
            need(!i.sign, "a signed load takes a register offset in Thumb");
            if (i.size == 4) return two(0x6000 | (load ? 0x0800u : 0u) | word_offset(124, 4) << 6 | i.rn << 3 | i.rd);
            if (i.size == 1) return two(0x7000 | (load ? 0x0800u : 0u) | word_offset(31, 1) << 6 | i.rn << 3 | i.rd);
            return two(0x8000 | (load ? 0x0800u : 0u) | word_offset(62, 2) << 6 | i.rn << 3 | i.rd);
        }
        case Op::Ldm: case Op::Stm: {
            const bool load = i.op == Op::Ldm;
            if (i.rn == 13 && i.wb && ((!load && i.pre && !i.up) || (load && !i.pre && i.up))) {
                const unsigned extra = load ? 0x8000u : 0x4000u;
                need((i.list & ~(0xFFu | extra)) == 0 && i.list != 0, "push takes r0-r7 and lr, pop r0-r7 and pc");
                return two((load ? 0xBC00u : 0xB400u) | ((i.list & extra) ? 0x100u : 0u) | (i.list & 0xFF));
            }
            need(!i.pre && i.up && low(i.rn) && i.list != 0 && (i.list & ~0xFFu) == 0, "a Thumb ldmia or stmia takes a low base and the registers r0-r7");
            need(i.wb == !(load && ((i.list >> i.rn) & 1)), "a Thumb ldmia or stmia writes the base back (unless it is loaded)");
            return two(0xC000 | (load ? 0x0800u : 0u) | i.rn << 8 | i.list);
        }
        case Op::B: {
            if (!i.target) refuse(i, "a branch without a target");
            const std::int64_t distance = static_cast<std::int64_t>(*i.target & ~1u) - (static_cast<std::int64_t>(i.pc) + 4);
            need(distance % 2 == 0, "a branch target is not a halfword");
            if (i.length == 4) {   // B.W and B<cond>.W (ARMv7-A)
                const std::uint32_t d = static_cast<std::uint32_t>(distance);
                if (i.cond == kAlways) {
                    need(distance >= -(1 << 24) && distance < (1 << 24), "the branch is out of reach");
                    const unsigned s = (d >> 24) & 1, j1 = (~((d >> 23) ^ s)) & 1, j2 = (~((d >> 22) ^ s)) & 1;
                    Bytes out = two(0xF000 | s << 10 | ((d >> 12) & 0x3FF));
                    const Bytes second = two(0x9000 | j1 << 13 | j2 << 11 | ((d >> 1) & 0x7FF));
                    out.insert(out.end(), second.begin(), second.end());
                    return out;
                }
                need(distance >= -(1 << 20) && distance < (1 << 20), "the conditional branch is out of reach");
                Bytes out = two(0xF000 | ((d >> 20) & 1) << 10 | static_cast<unsigned>(i.cond) << 6 | ((d >> 12) & 0x3F));
                const Bytes second = two(0x8000 | ((d >> 18) & 1) << 13 | ((d >> 19) & 1) << 11 | ((d >> 1) & 0x7FF));
                out.insert(out.end(), second.begin(), second.end());
                return out;
            }
            if (i.cond == kAlways) {
                need(distance >= -2048 && distance <= 2046, "the branch is out of reach");
                return two(0xE000 | ((static_cast<std::uint32_t>(distance) >> 1) & 0x7FF));
            }
            need(distance >= -256 && distance <= 254, "the conditional branch is out of reach");
            return two(0xD000 | static_cast<unsigned>(i.cond) << 8 | ((static_cast<std::uint32_t>(distance) >> 1) & 0xFF));
        }
        case Op::Bl: {
            if (!i.target) refuse(i, "a branch without a target");
            const std::int64_t distance = static_cast<std::int64_t>(*i.target & ~1u) - (static_cast<std::int64_t>(i.pc) + 4);
            need(distance % 2 == 0, "a branch target is not a halfword");
            need(distance >= -(1 << 24) && distance < (1 << 24), "the call is out of reach");
            const std::uint32_t d = static_cast<std::uint32_t>(distance);
            const unsigned sign = (d >> 24) & 1, j1 = (~((d >> 23) ^ sign)) & 1, j2 = (~((d >> 22) ^ sign)) & 1;   // (the same bits as the pair of the ARMv4T within 4 MB)
            Bytes out = two(0xF000 | sign << 10 | ((d >> 12) & 0x3FF));
            const Bytes second = two(0xD000 | j1 << 13 | j2 << 11 | ((d >> 1) & 0x7FF));
            out.insert(out.end(), second.begin(), second.end());
            return out;
        }
        case Op::Bx: return two(0x4700 | i.rm << 3);
        case Op::Blx: {
            if (!i.target) return two(0x4780 | i.rm << 3);
            // (the target is a word address: the processor clears the low two bits of the sum, so the distance is from the pc's word)
            const std::int64_t distance = static_cast<std::int64_t>(*i.target & ~1u) - ((static_cast<std::int64_t>(i.pc) + 4) & ~3LL);
            need(distance % 4 == 0, "a BLX target is not a word");
            need(distance >= -(1 << 22) && distance < (1 << 22), "the call is out of reach");
            const std::uint32_t d = static_cast<std::uint32_t>(distance);
            Bytes out = two(0xF000 | ((d >> 12) & 0x7FF));
            const Bytes second = two(0xE800 | ((d >> 1) & 0x7FF));
            out.insert(out.end(), second.begin(), second.end());
            return out;
        }
        case Op::Swi:
            need(i.imm <= 255, "a Thumb SWI comment is 8 bits");
            return two(0xDF00 | i.imm);
        default: refuse(i, "not a Thumb instruction");
    }
}

}  // namespace

Bytes encode(const Insn& insn) {
    if (insn.op == Op::Ext) return bytes_of(insn.word, insn.length);
    return insn.thumb() ? encode_thumb(insn) : bytes_of(encode_arm(insn), 4);
}

// ---- assembling ---------------------------------------------------------------------------------------------------------------

namespace {

struct Context {
    std::string_view line;
    Isa isa;
    std::uint32_t pc;
    const NameLabels& labels;
    Arch arch = Arch::V4T;
};

std::vector<std::string> split_operands(std::string_view s) {
    std::vector<std::string> out;
    int depth = 0;
    std::size_t from = 0;
    for (std::size_t k = 0; k <= s.size(); ++k) {
        if (k < s.size()) {
            const char c = s[k];
            if (c == '[' || c == '{' || c == '(') ++depth;
            else if (c == ']' || c == '}' || c == ')') --depth;
            if (!(c == ',' && depth == 0)) continue;
        }
        const std::string_view piece = text::trim(s.substr(from, k - from));
        if (!piece.empty() || k < s.size()) out.emplace_back(piece);
        from = k + 1;
    }
    while (!out.empty() && out.back().empty()) out.pop_back();
    return out;
}

unsigned reg_of(const Context& c, std::string_view operand) {
    const auto r = register_number(operand);
    if (!r) refuse(c.line, "'" + std::string(operand) + "' is not a register");
    return *r;
}

std::int64_t number_of(const Context& c, std::string_view operand) {
    operand = text::trim(operand);
    if (operand.starts_with('#')) operand = text::ltrim(operand.substr(1));
    return value(operand, c.labels, c.pc);
}

// The kinds of mnemonic and how each takes its suffixes.
enum class Kind : std::uint8_t { Data, Multiply, Transfer, Block, Push, Pop, Branch, BranchLink, BranchExchange, Swi, Mrs, Msr, Swap, Nop, ShiftName, Rrx, Neg,
                                  // ARMv5TE
                                  BranchLinkExchange, Clz, Saturating, Multiply16, Breakpoint, Preload, Coprocessor };

struct Base {
    std::string_view name;
    Kind kind;
    Op op;
    Shift shift;
    std::uint8_t xy = 0;   // the half-word selectors a 16-bit multiply's name carries
    bool v5 = false;       // ARMv5TE only
};

constexpr auto kBases = std::to_array<Base>({
    {"and", Kind::Data, Op::And, Shift::Lsl}, {"eor", Kind::Data, Op::Eor, Shift::Lsl}, {"sub", Kind::Data, Op::Sub, Shift::Lsl},
    {"rsb", Kind::Data, Op::Rsb, Shift::Lsl}, {"add", Kind::Data, Op::Add, Shift::Lsl}, {"adc", Kind::Data, Op::Adc, Shift::Lsl},
    {"sbc", Kind::Data, Op::Sbc, Shift::Lsl}, {"rsc", Kind::Data, Op::Rsc, Shift::Lsl}, {"tst", Kind::Data, Op::Tst, Shift::Lsl},
    {"teq", Kind::Data, Op::Teq, Shift::Lsl}, {"cmp", Kind::Data, Op::Cmp, Shift::Lsl}, {"cmn", Kind::Data, Op::Cmn, Shift::Lsl},
    {"orr", Kind::Data, Op::Orr, Shift::Lsl}, {"mov", Kind::Data, Op::Mov, Shift::Lsl}, {"bic", Kind::Data, Op::Bic, Shift::Lsl},
    {"mvn", Kind::Data, Op::Mvn, Shift::Lsl},
    {"mul", Kind::Multiply, Op::Mul, Shift::Lsl}, {"mla", Kind::Multiply, Op::Mla, Shift::Lsl}, {"umull", Kind::Multiply, Op::Umull, Shift::Lsl},
    {"umlal", Kind::Multiply, Op::Umlal, Shift::Lsl}, {"smull", Kind::Multiply, Op::Smull, Shift::Lsl}, {"smlal", Kind::Multiply, Op::Smlal, Shift::Lsl},
    {"ldr", Kind::Transfer, Op::Ldr, Shift::Lsl}, {"str", Kind::Transfer, Op::Str, Shift::Lsl},
    {"ldm", Kind::Block, Op::Ldm, Shift::Lsl}, {"stm", Kind::Block, Op::Stm, Shift::Lsl},
    {"push", Kind::Push, Op::Stm, Shift::Lsl}, {"pop", Kind::Pop, Op::Ldm, Shift::Lsl},
    {"b", Kind::Branch, Op::B, Shift::Lsl}, {"bl", Kind::BranchLink, Op::Bl, Shift::Lsl}, {"bx", Kind::BranchExchange, Op::Bx, Shift::Lsl},
    {"swi", Kind::Swi, Op::Swi, Shift::Lsl}, {"svc", Kind::Swi, Op::Swi, Shift::Lsl}, {"mrs", Kind::Mrs, Op::Mrs, Shift::Lsl},
    {"msr", Kind::Msr, Op::Msr, Shift::Lsl}, {"swp", Kind::Swap, Op::Swp, Shift::Lsl}, {"nop", Kind::Nop, Op::Mov, Shift::Lsl},
    {"lsl", Kind::ShiftName, Op::Mov, Shift::Lsl}, {"lsr", Kind::ShiftName, Op::Mov, Shift::Lsr}, {"asr", Kind::ShiftName, Op::Mov, Shift::Asr},
    {"ror", Kind::ShiftName, Op::Mov, Shift::Ror}, {"rrx", Kind::Rrx, Op::Mov, Shift::Ror}, {"neg", Kind::Neg, Op::Rsb, Shift::Lsl},
    {"blx", Kind::BranchLinkExchange, Op::Blx, Shift::Lsl, 0, true}, {"clz", Kind::Clz, Op::Clz, Shift::Lsl, 0, true},
    {"qadd", Kind::Saturating, Op::Qadd, Shift::Lsl, 0, true}, {"qsub", Kind::Saturating, Op::Qsub, Shift::Lsl, 0, true},
    {"qdadd", Kind::Saturating, Op::Qdadd, Shift::Lsl, 0, true}, {"qdsub", Kind::Saturating, Op::Qdsub, Shift::Lsl, 0, true},
    {"smlabb", Kind::Multiply16, Op::Smla, Shift::Lsl, 0, true}, {"smlabt", Kind::Multiply16, Op::Smla, Shift::Lsl, 2, true},
    {"smlatb", Kind::Multiply16, Op::Smla, Shift::Lsl, 1, true}, {"smlatt", Kind::Multiply16, Op::Smla, Shift::Lsl, 3, true},
    {"smulbb", Kind::Multiply16, Op::Smul, Shift::Lsl, 0, true}, {"smulbt", Kind::Multiply16, Op::Smul, Shift::Lsl, 2, true},
    {"smultb", Kind::Multiply16, Op::Smul, Shift::Lsl, 1, true}, {"smultt", Kind::Multiply16, Op::Smul, Shift::Lsl, 3, true},
    {"smlalbb", Kind::Multiply16, Op::Smlalxy, Shift::Lsl, 0, true}, {"smlalbt", Kind::Multiply16, Op::Smlalxy, Shift::Lsl, 2, true},
    {"smlaltb", Kind::Multiply16, Op::Smlalxy, Shift::Lsl, 1, true}, {"smlaltt", Kind::Multiply16, Op::Smlalxy, Shift::Lsl, 3, true},
    {"smlawb", Kind::Multiply16, Op::Smlaw, Shift::Lsl, 0, true}, {"smlawt", Kind::Multiply16, Op::Smlaw, Shift::Lsl, 2, true},
    {"smulwb", Kind::Multiply16, Op::Smulw, Shift::Lsl, 0, true}, {"smulwt", Kind::Multiply16, Op::Smulw, Shift::Lsl, 2, true},
    {"bkpt", Kind::Breakpoint, Op::Bkpt, Shift::Lsl, 0, true}, {"pld", Kind::Preload, Op::Pld, Shift::Lsl, 0, true},
    {"mcr", Kind::Coprocessor, Op::Mcr, Shift::Lsl, 0, true}, {"mrc", Kind::Coprocessor, Op::Mrc, Shift::Lsl, 0, true},
});

// What follows a mnemonic's base.
struct Suffix {
    std::uint8_t cond = kAlways;
    bool s = false;
    std::string size;   // "", b, bt, t, h, sh, sb (a transfer); ia ib da db fd fa ed ea (a block); b (a swap)
};

std::optional<Suffix> condition_only(std::string_view rest) {
    Suffix out;
    if (rest.empty()) return out;
    if (const auto c = condition_number(rest)) {
        out.cond = *c;
        return out;
    }
    return std::nullopt;
}

std::optional<Suffix> parse_suffix(Kind kind, std::string_view rest) {
    switch (kind) {
        case Kind::Data: case Kind::Multiply: case Kind::ShiftName: case Kind::Rrx: case Kind::Neg: {
            if (const auto c = condition_only(rest)) return c;
            if (rest.starts_with('s')) {
                if (const auto c = condition_only(rest.substr(1))) {
                    Suffix out = *c;
                    out.s = true;
                    return out;
                }
            }
            if (rest.ends_with('s') && rest.size() > 1) {   // (the divided syntax: the condition first)
                if (const auto c = condition_only(rest.substr(0, rest.size() - 1))) {
                    Suffix out = *c;
                    out.s = true;
                    return out;
                }
            }
            return std::nullopt;
        }
        case Kind::Transfer: {
            static constexpr std::string_view sizes[] = {"", "b", "bt", "t", "h", "sh", "sb", "d"};
            for (const std::string_view size : sizes) {
                if (rest.starts_with(size))
                    if (auto c = condition_only(rest.substr(size.size()))) {
                        c->size = std::string(size);
                        return c;
                    }
                if (!size.empty() && rest.ends_with(size))
                    if (auto c = condition_only(rest.substr(0, rest.size() - size.size()))) {
                        c->size = std::string(size);
                        return c;
                    }
            }
            return std::nullopt;
        }
        case Kind::Block: {
            static constexpr std::string_view modes[] = {"", "ia", "ib", "da", "db", "fd", "fa", "ed", "ea"};
            for (const std::string_view mode : modes) {
                if (rest.starts_with(mode))
                    if (auto c = condition_only(rest.substr(mode.size()))) {
                        c->size = std::string(mode);
                        return c;
                    }
                if (!mode.empty() && rest.ends_with(mode))
                    if (auto c = condition_only(rest.substr(0, rest.size() - mode.size()))) {
                        c->size = std::string(mode);
                        return c;
                    }
            }
            return std::nullopt;
        }
        case Kind::Swap: {
            if (auto c = condition_only(rest)) return c;
            if (rest.starts_with('b'))
                if (auto c = condition_only(rest.substr(1))) {
                    c->size = "b";
                    return c;
                }
            if (rest.ends_with('b'))
                if (auto c = condition_only(rest.substr(0, rest.size() - 1))) {
                    c->size = "b";
                    return c;
                }
            return std::nullopt;
        }
        default: return condition_only(rest);
    }
}

bool starts_shift(std::string_view operand) {
    const std::string s = text::lower(operand);
    if (s == "rrx") return true;
    for (const std::string_view name : kShifts)
        if (s.starts_with(name) && s.size() > 3 && (s[3] == ' ' || s[3] == '#')) return true;
    return false;
}

// `lsl #2`, `asr r3`, `rrx`: the shift of a register operand.
void parse_shift(const Context& c, Insn& i, std::string_view spec) {
    const std::string s = text::lower(text::trim(spec));
    if (s == "rrx") {
        i.shift = Shift::Ror;
        i.rrx = true;
        return;
    }
    bool found = false;
    for (unsigned k = 0; k < 4; ++k) {
        if (!s.starts_with(kShifts[k])) continue;
        i.shift = static_cast<Shift>(k);
        found = true;
    }
    if (!found || s.size() < 4) refuse(c.line, "'" + std::string(spec) + "' is not a shift");
    const std::string_view arg = text::trim(std::string_view(s).substr(3));
    if (arg.starts_with('#')) {
        const std::int64_t n = number_of(c, arg);
        if (n < 0 || n > 32) refuse(c.line, "a shift by " + std::to_string(n));
        if (n == 0 && i.shift != Shift::Lsl) refuse(c.line, "a shift by 0 (use #32, or rrx)");
        i.amount = static_cast<std::uint8_t>(n);
    } else {
        i.shift_by_reg = true;
        i.rs = static_cast<std::uint8_t>(reg_of(c, arg));
    }
}

// The second operand of a data-processing instruction from operand `k`: `#imm`, `#imm8, rot`, `rm`, `rm, lsl #n`.
void parse_operand2(const Context& c, Insn& i, const std::vector<std::string>& o, std::size_t k) {
    if (k >= o.size()) refuse(c.line, "an operand is missing");
    if (o[k].starts_with('#')) {
        i.imm_form = true;
        const std::int64_t v = number_of(c, o[k]);
        if (v < -2147483648LL || v > 4294967295LL) refuse(c.line, "the immediate does not fit 32 bits");
        i.imm = static_cast<std::uint32_t>(v);
        if (k + 1 < o.size()) {
            const std::int64_t rot = number_of(c, o[k + 1]);
            if (rot < 0 || rot > 30 || rot % 2 != 0) refuse(c.line, "the rotate is an even number from 0 to 30");
            i.rot = static_cast<std::uint8_t>(rot / 2);
            i.rot_fixed = true;
            i.imm = rotate_right(i.imm & 0xFF, static_cast<unsigned>(rot));
        }
        return;
    }
    i.rm = static_cast<std::uint8_t>(reg_of(c, o[k]));
    if (k + 1 < o.size()) parse_shift(c, i, o[k + 1]);
}

// `{r0-r3, lr}` (and a trailing `^`).
std::uint16_t parse_list(const Context& c, std::string_view operand, bool* hat) {
    operand = text::trim(operand);
    if (operand.ends_with('^')) {
        if (hat) *hat = true;
        operand = text::rtrim(operand.substr(0, operand.size() - 1));
    }
    if (operand.size() < 2 || operand.front() != '{' || operand.back() != '}') refuse(c.line, "a register list is written in braces");
    std::uint32_t list = 0;
    for (const std::string& item : split_operands(operand.substr(1, operand.size() - 2))) {
        const std::size_t dash = item.find('-');
        const unsigned from = reg_of(c, dash == std::string::npos ? item : item.substr(0, dash));
        const unsigned to = dash == std::string::npos ? from : reg_of(c, item.substr(dash + 1));
        if (to < from) refuse(c.line, "a register range runs upwards");
        for (unsigned r = from; r <= to; ++r) list |= 1u << r;
    }
    return static_cast<std::uint16_t>(list);
}

// The offset after a base register: `#imm`, `#-imm`, `rm`, `-rm`, `rm, lsl #n`.
void parse_offset(const Context& c, Insn& i, const std::vector<std::string>& parts, std::size_t k) {
    const std::string_view first = parts.at(k);
    if (first.starts_with('#')) {
        i.imm_form = true;
        const std::string_view body = text::ltrim(first.substr(1));
        const std::int64_t v = value(body, c.labels, c.pc);
        i.up = !(v < 0 || body.starts_with('-'));
        i.imm = static_cast<std::uint32_t>(v < 0 ? -v : v);
        return;
    }
    std::string_view r = first;
    if (r.starts_with('-')) {
        i.up = false;
        r = text::ltrim(r.substr(1));
    } else if (r.starts_with('+')) {
        r = text::ltrim(r.substr(1));
    }
    i.rm = static_cast<std::uint8_t>(reg_of(c, r));
    if (k + 1 < parts.size()) parse_shift(c, i, parts[k + 1]);
}

// The address operand of a load or store, from the text after the destination register.
void parse_address(const Context& c, Insn& i, std::string_view address) {
    address = text::trim(address);
    if (address.starts_with('=')) refuse(c.line, "literal pools (`ldr rd, =value`) are not assembled; write the word with .word and load it from its label");
    if (!address.starts_with('[')) {   // a label: a PC-relative load
        const std::int64_t target = value(address, c.labels, c.pc);
        const std::int64_t base = c.isa == Isa::Thumb ? ((static_cast<std::int64_t>(c.pc) + 4) & ~3LL) : static_cast<std::int64_t>(c.pc) + 8;
        i.rn = 15;
        i.imm_form = true;
        i.up = target >= base;
        i.imm = static_cast<std::uint32_t>(target >= base ? target - base : base - target);
        return;
    }
    int depth = 0;
    std::size_t close = 0;
    for (std::size_t k = 0; k < address.size(); ++k) {
        if (address[k] == '[') ++depth;
        if (address[k] == ']' && --depth == 0) {
            close = k;
            break;
        }
    }
    if (close == 0) refuse(c.line, "a ']' is missing");
    const std::vector<std::string> inner = split_operands(address.substr(1, close - 1));
    const std::string_view after = text::trim(address.substr(close + 1));
    if (inner.empty()) refuse(c.line, "a base register is missing");
    i.rn = static_cast<std::uint8_t>(reg_of(c, inner[0]));
    if (after.starts_with(',')) {   // post-indexed
        if (inner.size() != 1) refuse(c.line, "a post-indexed access has the base alone in brackets");
        i.pre = false;
        i.wb = true;
        parse_offset(c, i, split_operands(after.substr(1)), 0);
        return;
    }
    i.pre = true;
    if (after == "!") i.wb = true;
    else if (!after.empty()) refuse(c.line, "unexpected '" + std::string(after) + "'");
    if (inner.size() == 1) {
        i.imm_form = true;
        return;
    }
    parse_offset(c, i, inner, 1);
}

Insn parse(const Context& c) {
    const std::string_view line = c.line;
    std::size_t split = 0;
    while (split < line.size() && !text::is_space(line[split])) ++split;
    const std::string mnemonic = text::lower(line.substr(0, split));
    const std::string_view operands = text::ltrim(line.substr(split));
    std::vector<std::string> o = split_operands(operands);

    const Base* base = nullptr;
    Suffix suffix;
    for (const Base& b : kBases) {
        if ((b.v5 && c.arch < Arch::V5TE) || !mnemonic.starts_with(b.name) || (base && b.name.size() <= base->name.size())) continue;
        if (const auto s = parse_suffix(b.kind, std::string_view(mnemonic).substr(b.name.size()))) {
            base = &b;
            suffix = *s;
        }
    }
    if (!base) refuse(line, "not an instruction");

    Insn i;
    i.v6 = c.arch >= Arch::V6K;
    i.pc = c.pc;
    i.isa = c.isa;
    i.length = c.isa == Isa::Thumb ? 2 : 4;
    i.op = base->op;
    i.cond = suffix.cond;
    i.s = suffix.s;
    const auto count = [&](std::size_t n) {
        if (o.size() != n) refuse(line, std::format("{} operands where {} are written", o.size(), n));
    };
    const auto at_least = [&](std::size_t n) {
        if (o.size() < n) refuse(line, "an operand is missing");
    };
    const auto text_after_first = [&]() {
        const std::size_t comma = operands.find(',');
        if (comma == std::string_view::npos) refuse(line, "an operand is missing");
        return operands.substr(comma + 1);
    };
    switch (base->kind) {
        case Kind::Data: {
            at_least(2);
            if (is_test(i.op)) {
                i.s = true;
                i.rn = static_cast<std::uint8_t>(reg_of(c, o[0]));
                parse_operand2(c, i, o, 1);
            } else if (i.op == Op::Mov || i.op == Op::Mvn) {
                i.rd = static_cast<std::uint8_t>(reg_of(c, o[0]));
                parse_operand2(c, i, o, 1);
            } else {
                i.rd = static_cast<std::uint8_t>(reg_of(c, o[0]));
                const bool two_operands = o.size() == 2 || (o.size() == 3 && starts_shift(o[2]));
                if (two_operands) {
                    i.rn = i.rd;
                    parse_operand2(c, i, o, 1);
                } else {
                    i.rn = static_cast<std::uint8_t>(reg_of(c, o[1]));
                    parse_operand2(c, i, o, 2);
                }
            }
            break;
        }
        case Kind::ShiftName: case Kind::Rrx: {
            at_least(2);
            i.op = Op::Mov;
            i.rd = static_cast<std::uint8_t>(reg_of(c, o[0]));
            if (base->kind == Kind::Rrx) {
                count(2);
                i.rm = static_cast<std::uint8_t>(reg_of(c, o[1]));
                i.shift = Shift::Ror;
                i.rrx = true;
                break;
            }
            i.shift = base->shift;
            const bool three = o.size() == 3;
            i.rm = static_cast<std::uint8_t>(three ? reg_of(c, o[1]) : i.rd);
            const std::string& amount = o[three ? 2 : 1];
            if (amount.starts_with('#')) {
                const std::int64_t n = number_of(c, amount);
                if (n < 0 || n > 32 || (n == 0 && i.shift != Shift::Lsl)) refuse(line, "a shift by " + std::to_string(n));
                i.amount = static_cast<std::uint8_t>(n);
            } else {
                i.shift_by_reg = true;
                i.rs = static_cast<std::uint8_t>(reg_of(c, amount));
            }
            break;
        }
        case Kind::Neg:
            count(2);
            i.rd = static_cast<std::uint8_t>(reg_of(c, o[0]));
            i.rn = static_cast<std::uint8_t>(reg_of(c, o[1]));
            i.imm_form = true;
            break;
        case Kind::Nop:
            count(0);
            if (c.isa == Isa::Thumb) i.rd = i.rm = 8;
            break;
        case Kind::Multiply:
            if (i.op == Op::Mul) {
                at_least(2);
                i.rd = static_cast<std::uint8_t>(reg_of(c, o[0]));
                i.rm = static_cast<std::uint8_t>(reg_of(c, o[1]));
                i.rs = static_cast<std::uint8_t>(o.size() > 2 ? reg_of(c, o[2]) : i.rd);
            } else if (i.op == Op::Mla) {
                count(4);
                i.rd = static_cast<std::uint8_t>(reg_of(c, o[0]));
                i.rm = static_cast<std::uint8_t>(reg_of(c, o[1]));
                i.rs = static_cast<std::uint8_t>(reg_of(c, o[2]));
                i.rn = static_cast<std::uint8_t>(reg_of(c, o[3]));
            } else {
                count(4);
                i.rd = static_cast<std::uint8_t>(reg_of(c, o[0]));
                i.rn = static_cast<std::uint8_t>(reg_of(c, o[1]));
                i.rm = static_cast<std::uint8_t>(reg_of(c, o[2]));
                i.rs = static_cast<std::uint8_t>(reg_of(c, o[3]));
            }
            break;
        case Kind::Transfer: {
            at_least(2);
            i.rd = static_cast<std::uint8_t>(reg_of(c, o[0]));
            std::string_view pair_text;
            if (suffix.size == "d") {   // ldrd rd, rd2, [...]: the second register is the next one
                at_least(3);
                if (reg_of(c, o[1]) != i.rd + 1u) refuse(line, "the second register of a doubleword access is the one after the first");
                pair_text = text_after_first();
                pair_text = pair_text.substr(pair_text.find(',') + 1);
            }
            const std::string& size = suffix.size;
            if (size == "d" && c.arch < Arch::V5TE) refuse(line, "LDRD and STRD are ARMv5TE");
            i.size = size == "d" ? 8 : size == "b" || size == "bt" || size == "sb" ? 1 : size == "h" || size == "sh" ? 2 : 4;
            i.sign = size == "sb" || size == "sh";
            i.user = size == "t" || size == "bt";
            if (i.sign && i.op == Op::Str) refuse(line, "a signed store");
            parse_address(c, i, suffix.size == "d" ? pair_text : text_after_first());
            break;
        }
        case Kind::Block: case Kind::Push: case Kind::Pop: {
            if (base->kind == Kind::Block) {
                count(2);
                std::string_view rn = text::trim(o[0]);
                if (rn.ends_with('!')) {
                    i.wb = true;
                    rn = text::rtrim(rn.substr(0, rn.size() - 1));
                }
                i.rn = static_cast<std::uint8_t>(reg_of(c, rn));
                i.list = parse_list(c, o[1], &i.hat);
                const bool load = i.op == Op::Ldm;
                std::string mode = suffix.size.empty() ? "ia" : suffix.size;
                if (mode == "fd") mode = load ? "ia" : "db";
                else if (mode == "ed") mode = load ? "ib" : "da";
                else if (mode == "fa") mode = load ? "da" : "ib";
                else if (mode == "ea") mode = load ? "db" : "ia";
                i.pre = mode == "ib" || mode == "db";
                i.up = mode == "ia" || mode == "ib";
            } else {
                count(1);
                i.rn = 13;
                i.wb = true;
                i.list = parse_list(c, o[0], nullptr);
                i.pre = base->kind == Kind::Push;
                i.up = base->kind == Kind::Pop;
            }
            if (i.list == 0) refuse(line, "an empty register list");
            break;
        }
        case Kind::Swap:
            count(3);
            i.size = suffix.size == "b" ? 1 : 4;
            i.rd = static_cast<std::uint8_t>(reg_of(c, o[0]));
            i.rm = static_cast<std::uint8_t>(reg_of(c, o[1]));
            if (o[2].size() < 3 || o[2].front() != '[' || o[2].back() != ']') refuse(line, "the address is written in brackets");
            i.rn = static_cast<std::uint8_t>(reg_of(c, o[2].substr(1, o[2].size() - 2)));
            break;
        case Kind::Branch: case Kind::BranchLink:
            count(1);
            i.target = static_cast<std::uint32_t>(value(o[0], c.labels, c.pc));
            break;
        case Kind::BranchExchange:
            count(1);
            i.rm = static_cast<std::uint8_t>(reg_of(c, o[0]));
            break;
        case Kind::BranchLinkExchange:
            count(1);
            if (const auto r = register_number(o[0])) {
                i.rm = static_cast<std::uint8_t>(*r);
            } else {
                if (i.cond != kAlways) refuse(line, "BLX with a label is unconditional");
                const std::uint32_t t = static_cast<std::uint32_t>(value(o[0], c.labels, c.pc));
                i.target = c.isa == Isa::Arm ? (t | 1u) : (t & ~3u);   // (always the other state: a Thumb target is an odd address)
            }
            break;
        case Kind::Clz:
            count(2);
            i.rd = static_cast<std::uint8_t>(reg_of(c, o[0]));
            i.rm = static_cast<std::uint8_t>(reg_of(c, o[1]));
            break;
        case Kind::Saturating:
            count(3);
            i.rd = static_cast<std::uint8_t>(reg_of(c, o[0]));
            i.rm = static_cast<std::uint8_t>(reg_of(c, o[1]));
            i.rn = static_cast<std::uint8_t>(reg_of(c, o[2]));
            break;
        case Kind::Multiply16:
            i.xy = base->xy;
            if (i.op == Op::Smul || i.op == Op::Smulw) {
                count(3);
                i.rd = static_cast<std::uint8_t>(reg_of(c, o[0]));
                i.rm = static_cast<std::uint8_t>(reg_of(c, o[1]));
                i.rs = static_cast<std::uint8_t>(reg_of(c, o[2]));
            } else if (i.op == Op::Smlalxy) {
                count(4);
                i.rd = static_cast<std::uint8_t>(reg_of(c, o[0]));
                i.rn = static_cast<std::uint8_t>(reg_of(c, o[1]));
                i.rm = static_cast<std::uint8_t>(reg_of(c, o[2]));
                i.rs = static_cast<std::uint8_t>(reg_of(c, o[3]));
            } else {
                count(4);
                i.rd = static_cast<std::uint8_t>(reg_of(c, o[0]));
                i.rm = static_cast<std::uint8_t>(reg_of(c, o[1]));
                i.rs = static_cast<std::uint8_t>(reg_of(c, o[2]));
                i.rn = static_cast<std::uint8_t>(reg_of(c, o[3]));
            }
            break;
        case Kind::Breakpoint: {
            count(1);
            const std::int64_t n = number_of(c, o[0]);
            if (n < 0 || n > 0xFFFF) refuse(line, "the comment field is 16 bits");
            i.imm = static_cast<std::uint32_t>(n);
            break;
        }
        case Kind::Preload:
            if (i.cond != kAlways) refuse(line, "PLD is unconditional");
            i.size = 1;
            parse_address(c, i, operands);
            if (!i.pre || i.wb) refuse(line, "PLD takes [rn, offset] without writeback");
            break;
        case Kind::Coprocessor: {
            if (o.size() != 5 && o.size() != 6) refuse(line, "mcr and mrc take a coprocessor, two opcodes, a register and two coprocessor registers");
            const auto numbered = [&](const std::string& operand, char letter, std::int64_t limit) {
                const std::string t = text::lower(text::trim(operand));
                if (t.size() < 2 || t[0] != letter) refuse(line, "'" + operand + "' is not a " + letter + "N operand");
                const std::int64_t n = value(std::string_view(t).substr(1), c.labels, c.pc);
                if (n < 0 || n >= limit) refuse(line, "'" + operand + "' is out of range");
                return static_cast<std::uint8_t>(n);
            };
            i.cp = numbered(o[0], 'p', 16);
            const std::int64_t opc1 = number_of(c, o[1]);
            if (opc1 < 0 || opc1 > 7) refuse(line, "the first opcode is 0 to 7");
            i.opc1 = static_cast<std::uint8_t>(opc1);
            i.rd = static_cast<std::uint8_t>(reg_of(c, o[2]));
            i.crn = numbered(o[3], 'c', 16);
            i.crm = numbered(o[4], 'c', 16);
            if (o.size() == 6) {
                const std::int64_t opc2 = number_of(c, o[5]);
                if (opc2 < 0 || opc2 > 7) refuse(line, "the second opcode is 0 to 7");
                i.opc2 = static_cast<std::uint8_t>(opc2);
            }
            break;
        }
        case Kind::Swi: {
            count(1);
            const std::int64_t n = number_of(c, o[0]);
            if (n < 0 || n > 0xFFFFFF) refuse(line, "the comment field is 24 bits");
            i.imm = static_cast<std::uint32_t>(n);
            break;
        }
        case Kind::Mrs: {
            count(2);
            i.rd = static_cast<std::uint8_t>(reg_of(c, o[0]));
            const std::string psr = text::lower(o[1]);
            if (psr != "cpsr" && psr != "spsr") refuse(line, "cpsr or spsr");
            i.spsr = psr == "spsr";
            break;
        }
        case Kind::Msr: {
            at_least(2);
            const std::string psr = text::lower(o[0]);
            if (!psr.starts_with("cpsr") && !psr.starts_with("spsr")) refuse(line, "cpsr or spsr");
            i.spsr = psr[0] == 's';
            const std::string_view letters = std::string_view(psr).substr(4);
            if (letters.empty()) {
                i.fields = 9;
            } else {
                if (letters[0] != '_' || letters.size() < 2) refuse(line, "a PSR field is _f, _s, _x or _c");
                for (const char ch : letters.substr(1)) {
                    if (ch == 'f') i.fields |= 8;
                    else if (ch == 's') i.fields |= 4;
                    else if (ch == 'x') i.fields |= 2;
                    else if (ch == 'c') i.fields |= 1;
                    else refuse(line, "a PSR field is _f, _s, _x or _c");
                }
            }
            parse_operand2(c, i, o, 1);
            break;
        }
    }
    return i;
}

}  // namespace

Bytes assemble(std::string_view line, std::uint32_t pc, const NameLabels& labels, Arch arch) {
    line = text::trim(line);
    const Context c{line, (pc & 1) ? Isa::Thumb : Isa::Arm, pc & ~1u, labels, arch};
    const unsigned alignment = c.isa == Isa::Thumb ? 2 : 4;
    if (c.pc % alignment != 0) refuse(line, "an instruction is not at a multiple of its size");
    if (c.isa == Isa::Arm && arch >= Arch::V6K) {
        if (const std::optional<std::uint32_t> w = ext::assemble(line, c.pc, labels, arch)) return bytes_of(*w, 4);
    }
    if (c.isa == Isa::Thumb && arch >= Arch::V6K) {
        if (const std::optional<Bytes> b = t2::assemble(line, c.pc, labels)) return *b;
        // B.W and B<cond>.W: the same line without the `.w`, written in the wide encoding
        std::size_t split = 0;
        while (split < line.size() && !text::is_space(line[split])) ++split;
        const std::string head = text::lower(line.substr(0, split));
        if (arch >= Arch::V7A && head.size() >= 3 && head.ends_with(".w") && head[0] == 'b' && head != "bl.w" && head != "bx.w") {
            const std::string narrow = head.substr(0, head.size() - 2);
            if (narrow == "b" || (narrow.size() == 3 && std::any_of(kConditions.begin(), kConditions.end(), [&](std::string_view k) { return !k.empty() && k == std::string_view(narrow).substr(1); }))) {
                const std::string rewritten = narrow + std::string(line.substr(split));
                const Context wide{rewritten, c.isa, c.pc, labels, arch};
                Insn i = parse(wide);
                i.length = 4;
                return encode(i);
            }
        }
    }
    return encode(parse(c));
}

}  // namespace cb::decompile::arm
