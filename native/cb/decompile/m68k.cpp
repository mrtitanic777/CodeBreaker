#include "cb/decompile/m68k.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <format>
#include <unordered_map>

#include "cb/core/text.hpp"

namespace cb::decompile::m68k {

namespace {

// Addressing-mode categories (68000 programmer's manual, table 2-4).
constexpr unsigned kData = 1, kMemory = 2, kControl = 4, kAlterable = 8;

constexpr unsigned kAll = kData | kMemory | kControl | kAlterable;

// The categories (mode, reg) belongs to, or 0 if it is not a mode.
unsigned category(unsigned mode, unsigned reg) {
    switch (mode) {
        case 0: return kData | kAlterable;
        case 1: return kAlterable;
        case 2:
        case 5:
        case 6: return kAll;
        case 3:
        case 4: return kData | kMemory | kAlterable;
        default: break;
    }
    if (reg <= 1) return kAll;
    if (reg <= 3) return kData | kMemory | kControl;
    if (reg == 4) return kData | kMemory;
    return 0;
}

bool mode_ok(unsigned mode, unsigned reg, unsigned need, bool no_an = false, bool no_imm = false) {
    const unsigned c = category(mode, reg);
    if (c == 0 || (c & need) != need) return false;
    if (no_an && mode == 1) return false;
    if (no_imm && mode == 7 && reg == 4) return false;
    return true;
}

constexpr std::int32_t s16(unsigned v) { return static_cast<std::int16_t>(static_cast<std::uint16_t>(v)); }
constexpr std::int32_t s8(unsigned v) { return static_cast<std::int8_t>(static_cast<std::uint8_t>(v)); }

// The size a two-bit size field (opcode bits 6-7) names: 0 B, 1 W, 2 L.
constexpr Size size_of_field(unsigned ss) { return ss == 0 ? Size::Byte : ss == 1 ? Size::Word : Size::Long; }

constexpr std::array<std::string_view, 16> kConditions = {"T",  "F",  "HI", "LS", "CC", "CS", "NE", "EQ",
                                                          "VC", "VS", "PL", "MI", "GE", "LT", "GT", "LE"};
// Line 6: 0 BRA, 1 BSR, then the conditions HI .. LE.
std::string branch_suffix(unsigned c) { return c == 0 ? "RA" : c == 1 ? "SR" : std::string(kConditions[c]); }

Op make_op(OpKind kind, unsigned reg = 0, std::int64_t value = 0) {
    Op o;
    o.kind = kind;
    o.reg = static_cast<std::uint8_t>(reg);
    o.value = value;
    return o;
}

Op immediate(std::uint32_t value, Size size) {
    Op o = make_op(OpKind::Imm, 0, value);
    o.size = size;
    return o;
}

// ---- the disassembler --------------------------------------------------------------------------------------------

// One instruction's decode: reads the opcode word and extension words from `code` and builds the Insn. An opcode word
// that is not an instruction is nullopt all the way up; running out of code sets `truncated_`, which decode() turns
// into nullopt too (the words read past the end are zero, which no decision here can mind).
class Decoder {
public:
    Decoder(ByteView code, std::size_t at, std::uint32_t pc) : code_(code), at_(at), pos_(at), pc_(pc) {}

    std::optional<Insn> run() {
        w_ = word();
        if (truncated_) return std::nullopt;
        std::optional<Insn> insn = dispatch();
        if (truncated_) return std::nullopt;
        return insn;
    }

private:
    std::uint16_t word() {
        if (pos_ + 2 > code_.size()) {
            truncated_ = true;
            return 0;
        }
        const std::uint16_t v = static_cast<std::uint16_t>(code_[pos_] << 8 | code_[pos_ + 1]);
        pos_ += 2;
        return v;
    }
    std::uint32_t long_word() {
        const std::uint32_t hi = word();
        return hi << 16 | word();
    }
    // The PC a PC-relative extension word, which starts at code offset `ext_at`, is relative to.
    std::uint32_t ext_pc(std::size_t ext_at) const { return pc_ + static_cast<std::uint32_t>(ext_at - at_); }

    // The operand of effective address (mode, reg), reading its extension words; nullopt if it is not valid (a mode
    // the 68000 does not have, an index word using the 68020's fields, an immediate byte with a dirty high byte).
    std::optional<Op> read_ea(unsigned mode, unsigned reg, Size size) {
        switch (mode) {
            case 0: return make_op(OpKind::Dreg, reg);
            case 1: return make_op(OpKind::Areg, reg);
            case 2: return make_op(OpKind::Indirect, reg);
            case 3: return make_op(OpKind::PostInc, reg);
            case 4: return make_op(OpKind::PreDec, reg);
            case 5: return make_op(OpKind::Disp, reg, s16(word()));
            default: break;
        }
        if (mode == 6 || (mode == 7 && reg == 3)) {
            const std::size_t ext_at = pos_;
            const unsigned ext = word();
            if (ext & 0x0700) return std::nullopt;   // the 68000 has only the brief format, bits 8-10 zero
            IndexReg x;
            x.address = (ext & 0x8000) != 0;
            x.number = static_cast<std::uint8_t>((ext >> 12) & 7);
            x.size = (ext & 0x0800) ? Size::Long : Size::Word;
            if (mode == 6) {
                Op o = make_op(OpKind::Index, reg, s8(ext & 0xFF));
                o.index = x;
                return o;
            }
            Op o = make_op(OpKind::PcIndex, 0, ext_pc(ext_at) + static_cast<std::uint32_t>(s8(ext & 0xFF)));
            o.index = x;
            return o;
        }
        if (reg == 0) return make_op(OpKind::AbsW, 0, static_cast<std::uint32_t>(s16(word())));
        if (reg == 1) return make_op(OpKind::AbsL, 0, long_word());
        if (reg == 2) {
            const std::size_t ext_at = pos_;
            return make_op(OpKind::PcDisp, 0, ext_pc(ext_at) + static_cast<std::uint32_t>(s16(word())));
        }
        if (reg == 4) {
            if (size == Size::Byte) {
                const unsigned v = word();
                if (v & 0xFF00) return std::nullopt;
                return immediate(v, Size::Byte);
            }
            if (size == Size::Word) return immediate(word(), Size::Word);
            return immediate(long_word(), Size::Long);
        }
        return std::nullopt;
    }

    // The operand of effective address (mode, reg), checked against the categories `need`.
    std::optional<Op> ea(unsigned mode, unsigned reg, Size size, unsigned need, bool no_an = false, bool no_imm = false) {
        if (!mode_ok(mode, reg, need, no_an, no_imm)) return std::nullopt;
        return read_ea(mode, reg, size);
    }

    std::optional<Insn> done(std::string_view mnemonic, Size size, std::initializer_list<Op> operands, Flow flow = Flow::None,
                             std::optional<std::uint32_t> target = std::nullopt) {
        Insn insn;
        insn.mnemonic = mnemonic;
        insn.size = size;
        for (const Op& o : operands) insn.ops[insn.op_count++] = o;
        const std::size_t length = pos_ - at_;
        CB_ASSERT(length <= insn.raw.size(), "a 68000 instruction is at most 10 bytes");
        insn.length = static_cast<std::uint8_t>(length);
        insn.flow = flow;
        insn.target = target;
        insn.pc = pc_;
        insn.word = w_;
        if (!truncated_) std::copy_n(code_.begin() + static_cast<std::ptrdiff_t>(at_), length, insn.raw.begin());
        return insn;
    }

    // `ea`, then `done` with it among the operands: the common shape "<mnemonic>.<size> <ea>[,<other>]".
    std::optional<Insn> dispatch();
    std::optional<Insn> line0(unsigned mode, unsigned reg, unsigned hi);
    std::optional<Insn> moves(unsigned mode, unsigned reg, unsigned hi);
    std::optional<Insn> line4(unsigned mode, unsigned reg, unsigned hi);
    std::optional<Insn> line5(unsigned mode, unsigned reg, unsigned hi);
    std::optional<Insn> line6();
    std::optional<Insn> arith(unsigned line, unsigned mode, unsigned reg, unsigned hi);
    std::optional<Insn> shifts(unsigned mode, unsigned reg, unsigned hi);

    ByteView code_;
    std::size_t at_;
    std::size_t pos_;
    std::uint32_t pc_;
    std::uint16_t w_ = 0;
    bool truncated_ = false;
};

std::optional<std::uint32_t> absolute(const Op& op) {
    if (op.kind == OpKind::AbsW || op.kind == OpKind::AbsL || op.kind == OpKind::PcDisp) {
        return static_cast<std::uint32_t>(op.value);
    }
    return std::nullopt;
}

std::optional<Insn> Decoder::dispatch() {
    const unsigned w = w_;
    const unsigned line = w >> 12;
    const unsigned mode = (w >> 3) & 7, reg = w & 7;
    const unsigned hi = (w >> 9) & 7;
    switch (line) {
        case 0: return line0(mode, reg, hi);
        case 1:
        case 2:
        case 3: return moves(mode, reg, hi);
        case 4: return line4(mode, reg, hi);
        case 5: return line5(mode, reg, hi);
        case 6: return line6();
        case 7:
            if (w & 0x0100) return std::nullopt;
            return done("MOVEQ", Size::None, {make_op(OpKind::Quick, 0, s8(w & 0xFF)), make_op(OpKind::Dreg, hi)});
        case 8:
        case 9:
        case 0xB:
        case 0xC:
        case 0xD: return arith(line, mode, reg, hi);
        case 0xE: return shifts(mode, reg, hi);
        default: return std::nullopt;   // line A and line F: no 68000 instruction
    }
}

std::optional<Insn> Decoder::line0(unsigned mode, unsigned reg, unsigned hi) {
    const unsigned w = w_;
    if (w == 0x003C || w == 0x007C || w == 0x023C || w == 0x027C || w == 0x0A3C || w == 0x0A7C) {
        const std::string_view mn = (w >> 8) == 0x00 ? "ORI" : (w >> 8) == 0x02 ? "ANDI" : "EORI";
        const bool to_sr = (w & 0x40) != 0;
        const unsigned v = word();
        if (!to_sr && (v & 0xFF00)) return std::nullopt;
        const Size size = to_sr ? Size::Word : Size::Byte;
        return done(mn, size, {immediate(v, size), make_op(to_sr ? OpKind::Sr : OpKind::Ccr)});
    }
    static constexpr std::array<std::string_view, 4> kBitOps = {"BTST", "BCHG", "BCLR", "BSET"};
    if (w & 0x0100) {
        if (mode == 1) {   // MOVEP
            const unsigned opm = (w >> 6) & 3;
            const Size size = (opm & 1) ? Size::Long : Size::Word;
            const Op d = make_op(OpKind::Disp, reg, s16(word()));
            const Op dn = make_op(OpKind::Dreg, hi);
            return opm < 2 ? done("MOVEP", size, {d, dn}) : done("MOVEP", size, {dn, d});
        }
        const unsigned t = (w >> 6) & 3;
        const unsigned need = t == 0 ? kData : kData | kAlterable;
        const std::optional<Op> dst = ea(mode, reg, Size::Byte, need);
        if (!dst) return std::nullopt;
        return done(kBitOps[t], mode == 0 ? Size::Long : Size::Byte, {make_op(OpKind::Dreg, hi), *dst});
    }
    const unsigned top = (w >> 9) & 7;
    if (top == 4) {   // static bit operations
        const unsigned t = (w >> 6) & 3;
        const unsigned v = word();
        if (v & 0xFF00) return std::nullopt;
        const unsigned need = t == 0 ? kData : kData | kAlterable;
        const std::optional<Op> dst = ea(mode, reg, Size::Byte, need, false, true);
        if (!dst) return std::nullopt;
        return done(kBitOps[t], mode == 0 ? Size::Long : Size::Byte, {immediate(v, Size::Byte), *dst});
    }
    std::string_view name;
    switch (top) {
        case 0: name = "ORI"; break;
        case 1: name = "ANDI"; break;
        case 2: name = "SUBI"; break;
        case 3: name = "ADDI"; break;
        case 5: name = "EORI"; break;
        case 6: name = "CMPI"; break;
        default: break;
    }
    if (!name.empty() && ((w >> 6) & 3) != 3) {
        const Size size = size_of_field((w >> 6) & 3);
        const std::optional<Op> imm = read_ea(7, 4, size);
        if (!imm) return std::nullopt;
        const std::optional<Op> dst = ea(mode, reg, size, kData | kAlterable);
        if (!dst) return std::nullopt;
        return done(name, size, {*imm, *dst});
    }
    return std::nullopt;
}

// MOVE and MOVEA (lines 1-3).
std::optional<Insn> Decoder::moves(unsigned mode, unsigned reg, unsigned hi) {
    const unsigned line = w_ >> 12;
    const Size size = line == 1 ? Size::Byte : line == 3 ? Size::Word : Size::Long;
    const std::optional<Op> src = ea(mode, reg, size, 0, size == Size::Byte);
    if (!src) return std::nullopt;
    const unsigned dmode = (w_ >> 6) & 7, dreg = hi;
    if (dmode == 1) {
        if (size == Size::Byte) return std::nullopt;
        return done("MOVEA", size, {*src, make_op(OpKind::Areg, dreg)});
    }
    const std::optional<Op> dst = ea(dmode, dreg, size, kData | kAlterable);
    if (!dst) return std::nullopt;
    return done("MOVE", size, {*src, *dst});
}

std::optional<Insn> Decoder::line4(unsigned mode, unsigned reg, unsigned hi) {
    const unsigned w = w_;
    if (w == 0x4AFC) return done("ILLEGAL", Size::None, {});
    if ((w & 0xFFF0) == 0x4E40) return done("TRAP", Size::None, {make_op(OpKind::Quick, 0, w & 0xF)}, Flow::Call);
    if ((w & 0xFFF8) == 0x4E50) {
        const Op a = make_op(OpKind::Areg, reg);
        return done("LINK", Size::None, {a, immediate(word(), Size::Word)});
    }
    if ((w & 0xFFF8) == 0x4E58) return done("UNLK", Size::None, {make_op(OpKind::Areg, reg)});
    if ((w & 0xFFF8) == 0x4E60) return done("MOVE", Size::Long, {make_op(OpKind::Areg, reg), make_op(OpKind::Usp)});
    if ((w & 0xFFF8) == 0x4E68) return done("MOVE", Size::Long, {make_op(OpKind::Usp), make_op(OpKind::Areg, reg)});
    switch (w) {
        case 0x4E70: return done("RESET", Size::None, {});
        case 0x4E71: return done("NOP", Size::None, {});
        case 0x4E73: return done("RTE", Size::None, {}, Flow::Return);
        case 0x4E75: return done("RTS", Size::None, {}, Flow::Return);
        case 0x4E76: return done("TRAPV", Size::None, {});
        case 0x4E77: return done("RTR", Size::None, {}, Flow::Return);
        case 0x4E72: return done("STOP", Size::None, {immediate(word(), Size::Word)}, Flow::Halt);
        default: break;
    }
    if ((w & 0xFFC0) == 0x4E80) {
        const std::optional<Op> dst = ea(mode, reg, Size::Long, kControl);
        if (!dst) return std::nullopt;
        return done("JSR", Size::None, {*dst}, Flow::Call, absolute(*dst));
    }
    if ((w & 0xFFC0) == 0x4EC0) {
        const std::optional<Op> dst = ea(mode, reg, Size::Long, kControl);
        if (!dst) return std::nullopt;
        const std::optional<std::uint32_t> t = absolute(*dst);
        return done("JMP", Size::None, {*dst}, t ? Flow::Jump : Flow::Indirect, t);
    }
    if ((w & 0xF1C0) == 0x41C0) {
        const std::optional<Op> src = ea(mode, reg, Size::Long, kControl);
        if (!src) return std::nullopt;
        return done("LEA", Size::None, {*src, make_op(OpKind::Areg, hi)});
    }
    if ((w & 0xF1C0) == 0x4180) {
        const std::optional<Op> src = ea(mode, reg, Size::Word, kData);
        if (!src) return std::nullopt;
        return done("CHK", Size::Word, {*src, make_op(OpKind::Dreg, hi)});
    }
    if ((w & 0xFFF8) == 0x4840) return done("SWAP", Size::None, {make_op(OpKind::Dreg, reg)});
    if ((w & 0xFFF8) == 0x4880) return done("EXT", Size::Word, {make_op(OpKind::Dreg, reg)});
    if ((w & 0xFFF8) == 0x48C0) return done("EXT", Size::Long, {make_op(OpKind::Dreg, reg)});
    if ((w & 0xFFC0) == 0x4840) {
        const std::optional<Op> src = ea(mode, reg, Size::Long, kControl);
        if (!src) return std::nullopt;
        return done("PEA", Size::None, {*src});
    }
    if ((w & 0xFB80) == 0x4880) {   // MOVEM
        const Size size = (w & 0x40) ? Size::Long : Size::Word;
        const unsigned mask = word();
        Op regs = make_op(OpKind::Regs, 0, mask);
        if (w & 0x0400) {   // memory to registers
            if (!(mode_ok(mode, reg, kControl) || mode == 3)) return std::nullopt;
            const std::optional<Op> src = mode == 3 ? ea(mode, reg, size, 0) : ea(mode, reg, size, kControl);
            if (!src) return std::nullopt;
            return done("MOVEM", size, {*src, regs});
        }
        if (!(mode_ok(mode, reg, kControl | kAlterable) || mode == 4)) return std::nullopt;
        const std::optional<Op> dst = mode == 4 ? ea(mode, reg, size, 0) : ea(mode, reg, size, kControl | kAlterable);
        if (!dst) return std::nullopt;
        regs.predecrement = mode == 4;
        return done("MOVEM", size, {regs, *dst});
    }
    // NBCD, TAS and the three MOVEs that name SR or CCR: an effective address read at `read_size`, and (for the MOVEs)
    // the register operand that goes before or after it.
    struct Single {
        unsigned mask, value;
        std::string_view mnemonic;
        Size size, read_size;
        unsigned need;
        std::optional<OpKind> other;
        bool other_first;
    };
    static constexpr Single kSingle[] = {
        {0xFFC0, 0x4800, "NBCD", Size::None, Size::Byte, kData | kAlterable, std::nullopt, false},
        {0xFFC0, 0x40C0, "MOVE", Size::Word, Size::Word, kData | kAlterable, OpKind::Sr, true},   // MOVE SR,<ea>
        {0xFFC0, 0x44C0, "MOVE", Size::Word, Size::Word, kData, OpKind::Ccr, false},              // MOVE <ea>,CCR
        {0xFFC0, 0x46C0, "MOVE", Size::Word, Size::Word, kData, OpKind::Sr, false},               // MOVE <ea>,SR
        {0xFFC0, 0x4AC0, "TAS", Size::None, Size::Byte, kData | kAlterable, std::nullopt, false},
    };
    for (const Single& s : kSingle) {
        if ((w & s.mask) != s.value) continue;
        const std::optional<Op> e = ea(mode, reg, s.read_size, s.need);
        if (!e) return std::nullopt;
        if (!s.other) return done(s.mnemonic, s.size, {*e});
        const Op other = make_op(*s.other);
        return s.other_first ? done(s.mnemonic, s.size, {other, *e}) : done(s.mnemonic, s.size, {*e, other});
    }
    const unsigned ss = (w >> 6) & 3;
    if (ss != 3) {
        std::string_view mn;
        switch (w >> 8) {
            case 0x40: mn = "NEGX"; break;
            case 0x42: mn = "CLR"; break;
            case 0x44: mn = "NEG"; break;
            case 0x46: mn = "NOT"; break;
            case 0x4A: mn = "TST"; break;
            default: break;
        }
        if (!mn.empty()) {
            const Size size = size_of_field(ss);
            const std::optional<Op> e = ea(mode, reg, size, kData | kAlterable);
            if (!e) return std::nullopt;
            return done(mn, size, {*e});
        }
    }
    return std::nullopt;
}

// Scc, DBcc, ADDQ, SUBQ.
std::optional<Insn> Decoder::line5(unsigned mode, unsigned reg, unsigned hi) {
    const unsigned w = w_;
    const unsigned ss = (w >> 6) & 3;
    if (ss == 3) {
        const std::string cond(kConditions[(w >> 8) & 0xF]);
        if (mode == 1) {
            const std::size_t disp_at = pos_;
            const std::uint32_t t = ext_pc(disp_at) + static_cast<std::uint32_t>(s16(word()));
            return done("DB" + cond, Size::None, {make_op(OpKind::Dreg, reg), make_op(OpKind::Label, 0, t)}, Flow::Branch, t);
        }
        const std::optional<Op> dst = ea(mode, reg, Size::Byte, kData | kAlterable);
        if (!dst) return std::nullopt;
        return done("S" + cond, Size::None, {*dst});
    }
    const unsigned q = hi == 0 ? 8 : hi;
    const Size size = size_of_field(ss);
    if (mode == 1 && size == Size::Byte) return std::nullopt;
    const std::optional<Op> dst = ea(mode, reg, size, kAlterable);
    if (!dst) return std::nullopt;
    return done((w & 0x0100) ? "SUBQ" : "ADDQ", size, {make_op(OpKind::Quick, 0, q), *dst});
}

std::optional<Insn> Decoder::line6() {
    const unsigned w = w_;
    const unsigned c = (w >> 8) & 0xF;
    const unsigned d8 = w & 0xFF;
    const std::string mn = "B" + branch_suffix(c);
    std::uint32_t t;
    Op op;
    Size size;
    if (d8 == 0) {
        const std::size_t disp_at = pos_;
        t = ext_pc(disp_at) + static_cast<std::uint32_t>(s16(word()));
        op = make_op(OpKind::Label, 0, t);
        size = Size::Word;
    } else {
        t = pc_ + 2 + static_cast<std::uint32_t>(s8(d8));
        op = make_op(OpKind::Label8, 0, d8);
        op.target = t;
        size = Size::Short;
    }
    const Flow flow = c == 0 ? Flow::Jump : c == 1 ? Flow::Call : Flow::Branch;
    return done(mn, size, {op}, flow, t);
}

// OR SUB CMP AND ADD and their relatives (lines 8, 9, B, C, D).
std::optional<Insn> Decoder::arith(unsigned line, unsigned mode, unsigned reg, unsigned hi) {
    const unsigned w = w_;
    const unsigned opm = (w >> 6) & 7;
    const unsigned ss = opm & 3;
    if ((line == 8 || line == 0xC) && ss == 3) {   // DIVU DIVS / MULU MULS
        const std::string mn = std::string(line == 8 ? "DIV" : "MUL") + ((w & 0x0100) ? "S" : "U");
        const std::optional<Op> src = ea(mode, reg, Size::Word, kData);
        if (!src) return std::nullopt;
        return done(mn, Size::Word, {*src, make_op(OpKind::Dreg, hi)});
    }
    if ((line == 9 || line == 0xB || line == 0xD) && ss == 3) {   // SUBA CMPA ADDA
        const Size size = (w & 0x0100) ? Size::Long : Size::Word;
        const std::string_view mn = line == 9 ? "SUBA" : line == 0xB ? "CMPA" : "ADDA";
        const std::optional<Op> src = ea(mode, reg, size, 0);
        if (!src) return std::nullopt;
        return done(mn, size, {*src, make_op(OpKind::Areg, hi)});
    }
    const Size size = size_of_field(ss);
    const bool to_ea = (w & 0x0100) != 0;
    if (to_ea && (mode == 0 || mode == 1)) {
        const Op rx = make_op(mode ? OpKind::PreDec : OpKind::Dreg, reg);   // the pair of SBCD ABCD SUBX ADDX
        const Op ry = make_op(mode ? OpKind::PreDec : OpKind::Dreg, hi);
        if ((line == 8 || line == 0xC) && ss == 0) return done(line == 8 ? "SBCD" : "ABCD", Size::None, {rx, ry});   // SBCD ABCD
        if (line == 0xC) {   // EXG
            OpKind first, second;
            if (ss == 1 && mode == 0) {
                first = second = OpKind::Dreg;
            } else if (ss == 1 && mode == 1) {
                first = second = OpKind::Areg;
            } else if (ss == 2 && mode == 1) {
                first = OpKind::Dreg;
                second = OpKind::Areg;
            } else {
                return std::nullopt;
            }
            return done("EXG", Size::None, {make_op(first, hi), make_op(second, reg)});
        }
        if (line == 9 || line == 0xD) return done(line == 9 ? "SUBX" : "ADDX", size, {rx, ry});   // SUBX ADDX
        if (line == 0xB && mode == 1) return done("CMPM", size, {make_op(OpKind::PostInc, reg), make_op(OpKind::PostInc, hi)});
        if (line == 0xB) return done("EOR", size, {make_op(OpKind::Dreg, hi), make_op(OpKind::Dreg, reg)});   // EOR Dn,Dn
        return std::nullopt;
    }
    std::string_view mn;
    switch (line) {
        case 8: mn = "OR"; break;
        case 9: mn = "SUB"; break;
        case 0xB: mn = to_ea ? "EOR" : "CMP"; break;
        case 0xC: mn = "AND"; break;
        default: mn = "ADD"; break;
    }
    if (to_ea) {
        const unsigned need = mn == "EOR" ? kData | kAlterable : kMemory | kAlterable;
        const std::optional<Op> dst = ea(mode, reg, size, need);
        if (!dst) return std::nullopt;
        return done(mn, size, {make_op(OpKind::Dreg, hi), *dst});
    }
    const bool no_an = mn == "OR" || mn == "AND" || size == Size::Byte;
    const std::optional<Op> src = ea(mode, reg, size, 0, no_an);
    if (!src) return std::nullopt;
    return done(mn, size, {*src, make_op(OpKind::Dreg, hi)});
}

// Line E: the shifts and rotates, of a register or (one bit, a word) of memory.
std::optional<Insn> Decoder::shifts(unsigned mode, unsigned reg, unsigned hi) {
    const unsigned w = w_;
    static constexpr std::array<std::string_view, 4> kNames = {"AS", "LS", "ROX", "RO"};
    const unsigned ss = (w >> 6) & 3;
    const char direction = (w & 0x0100) ? 'L' : 'R';
    if (ss == 3) {
        if (hi & 4) return std::nullopt;
        const std::optional<Op> dst = ea(mode, reg, Size::Word, kMemory | kAlterable);
        if (!dst) return std::nullopt;
        return done(std::string(kNames[hi & 3]) + direction, Size::Word, {*dst});
    }
    const unsigned t = (w >> 3) & 3;
    const Op count = (w & 0x20) ? make_op(OpKind::Dreg, hi) : make_op(OpKind::Quick, 0, hi == 0 ? 8 : hi);
    return done(std::string(kNames[t]) + direction, size_of_field(ss), {count, make_op(OpKind::Dreg, reg)});
}

// ---- text --------------------------------------------------------------------------------------------------------

std::string reg_name(bool address, unsigned n) {
    if (address && n == 7) return "SP";
    return std::format("{}{}", address ? 'A' : 'D', n);
}

// $1F, $00001F with `digits`, -$10 for a negative number.
std::string hex(std::int64_t v, int digits = 0) {
    if (v < 0) return std::format("-${:X}", -v);
    return std::format("${:0{}X}", static_cast<std::uint64_t>(v), digits);
}

// The label of an address, or "" (an address no label can name, or a label with an empty name, has none).
const std::string* find_label(const AddressLabels& labels, std::int64_t address) {
    if (address < 0 || address > 0xFFFFFFFFll) return nullptr;
    const auto it = labels.find(static_cast<std::uint32_t>(address));
    return it == labels.end() || it->second.empty() ? nullptr : &it->second;
}

std::string labelled(const AddressLabels& labels, std::int64_t address) {
    const std::string* name = find_label(labels, address);
    return name ? *name : hex(address);
}

// A MOVEM register list: D0-D7/A0-A6 (the mask is reversed for -(An)).
std::string register_list(unsigned mask, bool predecrement) {
    if (predecrement) {
        unsigned reversed = 0;
        for (unsigned i = 0; i < 16; ++i) reversed |= ((mask >> i) & 1u) << (15 - i);
        mask = reversed;
    }
    std::string out;
    unsigned i = 0;
    while (i < 16) {
        if (!(mask & (1u << i))) {
            ++i;
            continue;
        }
        unsigned j = i;
        while (j + 1 < 16 && (mask & (1u << (j + 1))) && (j + 1) / 8 == i / 8) ++j;
        const auto name = [](unsigned n) { return n == 15 ? std::string("SP") : reg_name(n >= 8, n % 8); };
        if (!out.empty()) out += '/';
        out += i == j ? name(i) : name(i) + "-" + name(j);
        i = j + 1;
    }
    return out.empty() ? "0" : out;
}

std::string index_text(const IndexReg& x) { return std::format("{}.{}", reg_name(x.address, x.number), size_name(x.size)); }

std::string shape_of(std::string_view mnemonic, Size size, std::span<const Op> ops) {
    std::string out(mnemonic);
    if (size != Size::None) {
        out += '.';
        out += size_name(size);
    }
    out += ' ';
    bool first = true;
    for (const Op& o : ops) {
        if (!first) out += ',';
        first = false;
        out += o.key();
    }
    return out;
}

}  // namespace

std::string_view size_name(Size size) {
    switch (size) {
        case Size::Byte: return "B";
        case Size::Word: return "W";
        case Size::Long: return "L";
        case Size::Short: return "S";
        case Size::None: break;
    }
    return "";
}

std::string_view kind_name(OpKind kind) {
    switch (kind) {
        case OpKind::Dreg: return "D";
        case OpKind::Areg: return "A";
        case OpKind::Indirect: return "ind";
        case OpKind::PostInc: return "post";
        case OpKind::PreDec: return "pre";
        case OpKind::Disp: return "disp";
        case OpKind::Index: return "index";
        case OpKind::AbsW: return "absw";
        case OpKind::AbsL: return "absl";
        case OpKind::PcDisp: return "pcdisp";
        case OpKind::PcIndex: return "pcindex";
        case OpKind::Imm: return "imm";
        case OpKind::Quick: return "quick";
        case OpKind::Label: return "label";
        case OpKind::Label8: return "label8";
        case OpKind::Regs: return "regs";
        case OpKind::Ccr: return "ccr";
        case OpKind::Sr: return "sr";
        case OpKind::Usp: return "usp";
        case OpKind::Expr: return "expr";
    }
    return "";
}

std::string Op::key() const {
    switch (kind) {
        case OpKind::Dreg:
        case OpKind::Areg:
        case OpKind::Indirect:
        case OpKind::PostInc:
        case OpKind::PreDec:
        case OpKind::Disp:
        case OpKind::Index:   // (the index register and its size are the extension word's)
            return std::format("{}{}", kind_name(kind), reg);
        case OpKind::Quick: return std::format("#{}", value);
        case OpKind::Label8: return std::format("d8:{}", value);
        default: return std::string(kind_name(kind));
    }
}

std::string Insn::shape() const { return shape_of(mnemonic, size, operands()); }

std::optional<Insn> decode(ByteView code, std::size_t at, std::uint32_t pc) { return Decoder(code, at, pc).run(); }

std::string operand_text(const Op& o, const AddressLabels& labels) {
    switch (o.kind) {
        case OpKind::Dreg: return reg_name(false, o.reg);
        case OpKind::Areg: return reg_name(true, o.reg);
        case OpKind::Indirect: return std::format("({})", reg_name(true, o.reg));
        case OpKind::PostInc: return std::format("({})+", reg_name(true, o.reg));
        case OpKind::PreDec: return std::format("-({})", reg_name(true, o.reg));
        case OpKind::Disp: return std::format("{}({})", hex(o.value), reg_name(true, o.reg));
        case OpKind::Index: return std::format("{}({},{})", hex(o.value), reg_name(true, o.reg), index_text(o.index));
        case OpKind::AbsW: {
            const std::string* name = find_label(labels, o.value);
            return std::format("({}).W", name ? *name : hex(o.value < 0x8000 ? o.value & 0xFFFF : o.value, 4));
        }
        case OpKind::AbsL: {
            const std::string* name = find_label(labels, o.value);
            return std::format("({}).L", name ? *name : hex(o.value, 8));
        }
        case OpKind::PcDisp: return std::format("{}(PC)", labelled(labels, o.value));
        case OpKind::PcIndex: return std::format("{}(PC,{})", labelled(labels, o.value), index_text(o.index));
        case OpKind::Imm: {
            const std::string* name = o.size == Size::Long ? find_label(labels, o.value) : nullptr;
            return "#" + (name ? *name : hex(o.value));
        }
        case OpKind::Quick: return std::format("#{}", o.value);
        case OpKind::Label:
        case OpKind::Expr: return labelled(labels, o.value);
        case OpKind::Label8: return labelled(labels, o.target);
        case OpKind::Regs: return register_list(static_cast<unsigned>(o.value), o.predecrement);
        case OpKind::Ccr: return "CCR";
        case OpKind::Sr: return "SR";
        case OpKind::Usp: return "USP";
    }
    return "";
}

std::string text(const Insn& insn, const AddressLabels& labels) {
    std::string out = insn.mnemonic;
    if (insn.size != Size::None) {
        out += '.';
        out += size_name(insn.size);
    }
    bool first = true;
    for (const Op& o : insn.operands()) {
        out += first ? ' ' : ',';
        first = false;
        out += operand_text(o, labels);
    }
    return out;
}

// ---- the assembler -----------------------------------------------------------------------------------------------

namespace {

// {shape: opcode word} for every opcode word that decodes (the first with each shape). Looked up, never iterated.
const std::unordered_map<std::string, std::uint16_t>& shapes() {
    static const std::unordered_map<std::string, std::uint16_t> table = [] {
        std::unordered_map<std::string, std::uint16_t> t;
        t.reserve(50000);
        std::array<std::uint8_t, 10> code{};
        for (unsigned w = 0; w < 0x10000; ++w) {
            code[0] = static_cast<std::uint8_t>(w >> 8);
            code[1] = static_cast<std::uint8_t>(w & 0xFF);
            if (const std::optional<Insn> insn = decode(code, 0, 0)) t.emplace(insn->shape(), static_cast<std::uint16_t>(w));
        }
        return t;
    }();
    return table;
}

[[noreturn]] void fail(const std::string& what) { throw ParseError("", 0, 0, what); }

struct Register {
    bool address;
    std::uint8_t number;
};

// D0-D7, A0-A7, SP (case does not matter, the ends may be blank).
std::optional<Register> parse_register(std::string_view s) {
    s = text::trim(s);
    if (s.size() == 2 && (s[0] == 'S' || s[0] == 's') && (s[1] == 'P' || s[1] == 'p')) return Register{true, 7};
    if (s.size() != 2 || s[1] < '0' || s[1] > '7') return std::nullopt;
    const char k = static_cast<char>(s[0] & ~0x20);
    if (k != 'D' && k != 'A') return std::nullopt;
    return Register{k == 'A', static_cast<std::uint8_t>(s[1] - '0')};
}

// D0-D3/A0-A2 as the bit mask of a MOVEM list; nullopt for a name that is not a register.
std::optional<unsigned> parse_register_list(std::string_view s) {
    unsigned mask = 0;
    for (std::string_view part : text::split(s, '/')) {
        const std::size_t dash = part.find('-');
        const std::string_view a = part.substr(0, dash);
        const std::string_view b = dash == std::string_view::npos ? std::string_view() : part.substr(dash + 1);
        const std::optional<Register> ra = parse_register(a), rb = parse_register(b.empty() ? a : b);
        if (!ra || !rb) return std::nullopt;
        for (unsigned i = ra->number + (ra->address ? 8u : 0u); i <= rb->number + (rb->address ? 8u : 0u); ++i) mask |= 1u << i;
    }
    return mask;
}

bool is_word(std::string_view s) {
    return !s.empty() && std::ranges::all_of(s, [](char c) { return std::isalnum(static_cast<unsigned char>(c)) || c == '_'; });
}

// D0-D7 / A0-A7 joined by a dash (D0-D3, A0-A6), upper case: a register list of one run.
bool is_register_range(std::string_view u) {
    const auto reg = [](std::string_view r) { return r == "SP" || (r.size() == 2 && (r[0] == 'D' || r[0] == 'A') && r[1] >= '0' && r[1] <= '7'); };
    const std::size_t dash = u.find('-');
    return dash != std::string_view::npos && reg(u.substr(0, dash)) && reg(u.substr(dash + 1));
}

// One operand's text as an Op (extension values are in `value`; a bare value is an Expr).
Op parse_operand(std::string_view operand, const NameLabels& labels, std::uint32_t pc, Size imm_size) {
    const std::string_view t = text::trim(operand);
    const std::string u = text::upper(t);
    if (u == "CCR") return make_op(OpKind::Ccr);
    if (u == "SR") return make_op(OpKind::Sr);
    if (u == "USP") return make_op(OpKind::Usp);
    if (const std::optional<Register> r = parse_register(t)) return make_op(r->address ? OpKind::Areg : OpKind::Dreg, r->number);
    if (t.starts_with('#')) {
        Op o = make_op(OpKind::Imm, 0, value(t.substr(1), labels, pc));
        o.size = imm_size;
        return o;
    }
    // (An) (An)+ -(An)
    {
        std::string_view body = t;
        const bool plus = body.ends_with('+');
        if (plus) body.remove_suffix(1);
        if (body.size() >= 3 && body.front() == '(' && body.back() == ')') {
            const std::string_view inner = body.substr(1, body.size() - 2);
            const std::optional<Register> r = is_word(inner) ? parse_register(inner) : std::nullopt;
            if (r && r->address) return make_op(plus ? OpKind::PostInc : OpKind::Indirect, r->number);
        }
        if (!plus && t.size() >= 4 && t.starts_with("-(") && t.back() == ')') {
            const std::string_view inner = t.substr(2, t.size() - 3);
            const std::optional<Register> r = is_word(inner) ? parse_register(inner) : std::nullopt;
            if (r && r->address) return make_op(OpKind::PreDec, r->number);
        }
    }
    // d(An,Xn.s) d(PC,Xn.s) d(An) d(PC): what comes before the last '(' is the displacement expression
    const std::size_t lp = t.rfind('(');
    if (t.ends_with(')') && lp != std::string_view::npos) {
        const std::string_view head = t.substr(0, lp);
        const std::string_view inner = t.substr(lp + 1, t.size() - lp - 2);
        const std::size_t comma = inner.find(',');
        if (comma != std::string_view::npos) {
            // word, blanks, comma, blanks, word, optional .W / .L
            const std::string_view base = inner.substr(0, comma);
            std::string_view rest = text::ltrim(inner.substr(comma + 1));
            Size xs = Size::Word;
            bool shaped = is_word(text::rtrim(base));
            if (shaped && rest.size() > 2 && rest[rest.size() - 2] == '.') {
                const char c = static_cast<char>(rest.back() & ~0x20);
                if (c == 'W' || c == 'L') {
                    xs = c == 'L' ? Size::Long : Size::Word;
                    rest.remove_suffix(2);
                } else {
                    shaped = false;
                }
            }
            if (shaped && is_word(rest)) {
                const std::string_view base_name = text::rtrim(base);
                const std::string base_upper = text::upper(base_name);
                const std::optional<Register> x = parse_register(rest);
                const std::optional<Register> a = parse_register(base_name);
                if (!x) fail(std::format("\"{}\" is not an index register", rest));
                const std::int64_t disp = text::trim(head).empty() ? 0 : value(head, labels, pc);
                IndexReg index{x->address, x->number, xs};
                if (base_upper == "PC") {
                    Op o = make_op(OpKind::PcIndex, 0, disp);
                    o.index = index;
                    return o;
                }
                if (!a || !a->address) fail(std::format("\"{}\" is not a base register", base_name));
                Op o = make_op(OpKind::Index, a->number, disp);
                o.index = index;
                return o;
            }
        } else if (is_word(inner) && !text::trim(head).empty()) {
            const bool pc_relative = text::iequals(inner, "PC");
            const std::optional<Register> a = parse_register(inner);
            if (pc_relative || (a && a->address)) {
                const std::int64_t v = value(head, labels, pc);
                return pc_relative ? make_op(OpKind::PcDisp, 0, v) : make_op(OpKind::Disp, a->number, v);
            }
        }
    }
    // (xxx).W (xxx).L xxx.W xxx.L
    if (t.size() > 2 && t[t.size() - 2] == '.' && ((t.back() & ~0x20) == 'W' || (t.back() & ~0x20) == 'L')) {
        std::string_view body = t.substr(0, t.size() - 2);
        if (body.size() > 2 && body.front() == '(' && body.back() == ')') body = body.substr(1, body.size() - 2);
        return make_op((t.back() & ~0x20) == 'W' ? OpKind::AbsW : OpKind::AbsL, 0, value(body, labels, pc));
    }
    if (t.find('/') != std::string_view::npos || is_register_range(u)) {
        const std::optional<unsigned> mask = parse_register_list(t);
        if (!mask) fail(std::format("\"{}\" is not a register list", t));
        return make_op(OpKind::Regs, 0, *mask);
    }
    return make_op(OpKind::Expr, 0, value(t, labels, pc));
}

// A difference of 32-bit addresses as a signed number (the address space wraps).
std::int64_t wrap(std::int64_t v) { return static_cast<std::int32_t>(static_cast<std::uint32_t>(v)); }

// One way a parsed line can be read; the first whose shape is an opcode word is the instruction.
struct Candidate {
    std::string mnemonic;
    Size size;
    std::vector<Op> ops;
};

std::uint16_t reverse_bits(std::uint32_t mask) {
    std::uint16_t out = 0;
    for (unsigned i = 0; i < 16; ++i) out = static_cast<std::uint16_t>(out | (((mask >> i) & 1u) << (15 - i)));
    return out;
}

bool is_branch_mnemonic(std::string_view mn) {
    if (mn.size() < 3 || mn[0] != 'B') return false;
    const std::string_view cc = mn.substr(1);
    if (cc == "RA" || cc == "SR") return true;
    for (std::size_t c = 2; c < kConditions.size(); ++c) {
        if (cc == kConditions[c]) return true;
    }
    return false;
}

// The ways a parsed line can be read: branches by their size, values as absolute words or longs, quick values, and so on.
std::vector<Candidate> candidates(const std::string& mn, Size size, std::vector<Op> ops, std::uint32_t pc) {
    std::vector<Candidate> out;
    if (is_branch_mnemonic(mn)) {
        if (ops.empty() || ops[0].kind != OpKind::Expr) fail(mn + " needs a target address");
        if (size == Size::Long) fail("a branch is .S or .W");
        const std::int64_t target = ops[0].value;
        const std::int64_t d = wrap(target - (static_cast<std::int64_t>(pc) + 2));
        const bool fits = d >= -128 && d <= 127 && d != 0;
        if (size == Size::Short || size == Size::Byte) {   // a short branch as written: never made long
            if (!fits) fail("short branch out of range");
        }
        if (size == Size::Short || size == Size::Byte || (size == Size::None && fits)) {
            Op o = make_op(OpKind::Label8, 0, d & 0xFF);
            o.target = static_cast<std::uint32_t>(target);
            out.push_back({mn, Size::Short, {o}});
        } else {
            out.push_back({mn, Size::Word, {make_op(OpKind::Label, 0, target)}});
        }
        return out;
    }
    if (mn.starts_with("DB")) {
        if (ops.size() < 2 || ops[1].kind != OpKind::Expr) fail(mn + " needs a register and a target address");
        out.push_back({mn, Size::None, {ops[0], make_op(OpKind::Label, 0, ops[1].value)}});
        return out;
    }
    if ((mn == "JMP" || mn == "JSR") && !ops.empty() && ops[0].kind == OpKind::Expr) {
        out.push_back({mn, Size::None, {make_op(OpKind::AbsL, 0, ops[0].value)}});
        return out;
    }
    if (mn == "MOVEM") {
        if (ops.size() < 2) fail("MOVEM needs a register list and an address");
        const OpKind last = ops.back().kind;
        const bool list_is_last = last == OpKind::Dreg || last == OpKind::Areg || last == OpKind::Regs;
        std::vector<Op> listed;
        for (std::size_t i = 0; i < ops.size(); ++i) {
            const Op& o = ops[i];
            const bool is_list = o.kind == OpKind::Regs || o.kind == OpKind::Dreg || o.kind == OpKind::Areg;
            if (is_list && (list_is_last ? i == 1 : i == 0)) {
                Op r = make_op(OpKind::Regs, 0, o.kind == OpKind::Regs ? o.value : 1ll << (o.reg + (o.kind == OpKind::Areg ? 8 : 0)));
                r.predecrement = last == OpKind::PreDec && i == 0;
                listed.push_back(r);
            } else {
                listed.push_back(o);
            }
        }
        if (listed[0].kind == OpKind::Regs && listed[1].kind == OpKind::PreDec) listed[0].value = reverse_bits(static_cast<std::uint32_t>(listed[0].value));
        out.push_back({mn, size, std::move(listed)});
        return out;
    }
    // quick forms take the value in the opcode word
    const std::string_view prefix = std::string_view(mn).substr(0, 2);
    const bool quick_mnemonic = mn == "ADDQ" || mn == "SUBQ" || mn == "MOVEQ" || mn == "TRAP";
    if (quick_mnemonic || ((prefix == "AS" || prefix == "LS" || prefix == "RO") && !ops.empty() && ops[0].kind == OpKind::Imm)) {
        if (!ops.empty() && ops[0].kind == OpKind::Imm) {
            ops[0] = make_op(OpKind::Quick, 0, mn == "MOVEQ" ? s8(static_cast<unsigned>(ops[0].value & 0xFF)) : ops[0].value);
        }
        out.push_back({mn, mn == "MOVEQ" || mn == "TRAP" ? Size::None : size, std::move(ops)});
        return out;
    }
    // a bare value is an absolute address: word if it fits, else long
    std::vector<std::vector<Op>> variants(1);
    for (const Op& o : ops) {
        if (o.kind == OpKind::Expr) {
            const std::int64_t v = o.value & 0xFFFFFFFFll;
            std::vector<Op> choices;
            if (v <= 0x7FFF || v >= 0xFFFF8000ll) choices.push_back(make_op(OpKind::AbsW, 0, v));
            choices.push_back(make_op(OpKind::AbsL, 0, v));
            std::vector<std::vector<Op>> next;
            for (const std::vector<Op>& x : variants) {
                for (const Op& c : choices) {
                    next.push_back(x);
                    next.back().push_back(c);
                }
            }
            variants = std::move(next);
        } else {
            for (std::vector<Op>& x : variants) x.push_back(o);
        }
    }
    const auto in = [&](std::initializer_list<std::string_view> names) { return std::ranges::find(names, mn) != names.end(); };
    for (const std::vector<Op>& v : variants) {
        out.push_back({mn, size, v});
        if (in({"MOVE", "ADD", "SUB", "CMP"}) && v.size() == 2 && v[1].kind == OpKind::Areg) out.push_back({mn + "A", size, v});
        if (in({"ADD", "SUB", "AND", "OR", "EOR", "CMP"}) && !v.empty() && v[0].kind == OpKind::Imm) out.push_back({mn + "I", size, v});
        if (in({"BTST", "BCHG", "BCLR", "BSET"}) && v.size() == 2) out.push_back({mn, v[1].kind == OpKind::Dreg ? Size::Long : Size::Byte, v});
        if (in({"MOVE", "ORI", "ANDI", "EORI"}) && v.size() == 2 && v[1].kind == OpKind::Ccr) {
            out.push_back({mn, mn != "MOVE" ? Size::Byte : Size::Word, v});
        }
        if (in({"MOVE", "ORI", "ANDI", "EORI"}) && v.size() == 2 && (v[0].kind == OpKind::Sr || v[1].kind == OpKind::Sr)) {
            out.push_back({mn, Size::Word, v});
        }
    }
    return out;
}

std::uint16_t brief(const IndexReg& x) {
    return static_cast<std::uint16_t>((x.address ? 0x8000 : 0) | (x.number << 12) | (x.size == Size::Long ? 0x0800 : 0));
}

// The opcode word, then the extension words of the operands.
Bytes encode(std::uint16_t word, const Candidate& c, std::uint32_t pc) {
    Bytes out{static_cast<std::uint8_t>(word >> 8), static_cast<std::uint8_t>(word & 0xFF)};
    const auto put16 = [&](std::uint32_t v) {
        out.push_back(static_cast<std::uint8_t>((v >> 8) & 0xFF));
        out.push_back(static_cast<std::uint8_t>(v & 0xFF));
    };
    const auto put32 = [&](std::uint32_t v) {
        put16(v >> 16);
        put16(v & 0xFFFF);
    };
    if (c.mnemonic == "MOVEM") {
        for (const Op& o : c.ops) {
            if (o.kind == OpKind::Regs) {
                put16(static_cast<std::uint32_t>(o.value));
                break;
            }
        }
    }
    const bool bit_op = c.mnemonic == "BTST" || c.mnemonic == "BCHG" || c.mnemonic == "BCLR" || c.mnemonic == "BSET";
    const bool logic_immediate = c.mnemonic == "ORI" || c.mnemonic == "ANDI" || c.mnemonic == "EORI";
    for (const Op& o : c.ops) {
        switch (o.kind) {
            case OpKind::Imm: {
                Size sz = c.mnemonic == "LINK" || c.mnemonic == "STOP" ? Size::Word : o.size;
                if (logic_immediate && c.ops.back().kind == OpKind::Ccr) sz = Size::Byte;
                if (bit_op) sz = Size::Byte;
                if (sz == Size::Long) put32(static_cast<std::uint32_t>(o.value));
                else put16(static_cast<std::uint32_t>(o.value) & (sz == Size::Byte ? 0xFFu : 0xFFFFu));
                break;
            }
            case OpKind::Disp:
                if (o.value < -0x8000 || o.value > 0xFFFF) fail("displacement does not fit 16 bits");
                put16(static_cast<std::uint32_t>(o.value) & 0xFFFF);
                break;
            case OpKind::Index:
                if (o.value < -0x80 || o.value > 0xFF) fail("displacement does not fit 8 bits");
                put16(brief(o.index) | (static_cast<std::uint32_t>(o.value) & 0xFF));
                break;
            case OpKind::AbsW: put16(static_cast<std::uint32_t>(o.value) & 0xFFFF); break;
            case OpKind::AbsL: put32(static_cast<std::uint32_t>(o.value)); break;
            case OpKind::PcDisp: {
                const std::int64_t d = wrap(o.value - (static_cast<std::int64_t>(pc) + static_cast<std::int64_t>(out.size())));
                if (d < -0x8000 || d > 0x7FFF) fail("PC-relative operand out of range");
                put16(static_cast<std::uint32_t>(d) & 0xFFFF);
                break;
            }
            case OpKind::PcIndex: {
                const std::int64_t d = wrap(o.value - (static_cast<std::int64_t>(pc) + static_cast<std::int64_t>(out.size())));
                if (d < -0x80 || d > 0x7F) fail("PC-relative operand out of range");
                put16(brief(o.index) | (static_cast<std::uint32_t>(d) & 0xFF));
                break;
            }
            case OpKind::Label: {
                const std::int64_t d = wrap(o.value - (static_cast<std::int64_t>(pc) + static_cast<std::int64_t>(out.size())));
                if (d < -0x8000 || d > 0x7FFF) fail("branch out of range");
                put16(static_cast<std::uint32_t>(d) & 0xFFFF);
                break;
            }
            default: break;   // registers, the quick and short-branch values (in the opcode word), CCR SR USP, the list
        }
    }
    return out;
}

Size parse_size(std::string_view s) {
    if (s.empty()) return Size::None;
    if (s == "B") return Size::Byte;
    if (s == "W") return Size::Word;
    if (s == "L") return Size::Long;
    if (s == "S") return Size::Short;
    fail(std::format("unknown size .{}", s));
}

}  // namespace

std::int64_t value(std::string_view expr, const NameLabels& labels, std::uint32_t pc) {
    return z80::value(text::trim(expr) == "*" ? std::string_view("$") : expr, labels, pc);
}

Bytes assemble(std::string_view line_in, std::uint32_t pc, const NameLabels& labels) {
    const std::string_view line = text::trim(line_in);
    const std::size_t blank = line.find_first_of(" \t");
    const std::string_view head = line.substr(0, blank);
    const std::string_view rest = blank == std::string_view::npos ? std::string_view() : line.substr(blank + 1);
    const std::size_t dot = head.find('.');
    const std::string mnemonic = text::upper(head.substr(0, dot));
    const Size size = parse_size(dot == std::string_view::npos ? std::string_view() : std::string_view(text::upper(head.substr(dot + 1))));
    std::vector<Op> ops;
    if (!text::trim(rest).empty()) {
        for (const std::string& piece : z80::split_operands(rest)) {
            ops.push_back(parse_operand(piece, labels, pc, size == Size::None ? Size::Word : size));
        }
    }
    const auto& table = shapes();
    for (const Candidate& c : candidates(mnemonic, size, std::move(ops), pc)) {
        const auto it = table.find(shape_of(c.mnemonic, c.size, c.ops));
        if (it != table.end()) return encode(it->second, c, pc);
    }
    fail(std::format("not an instruction: \"{}\"", line));
}

}  // namespace cb::decompile::m68k
