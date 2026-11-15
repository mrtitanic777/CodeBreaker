// cb-unity: alone
#include "cb/decompile/tlcs900.hpp"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <format>
#include <map>
#include <unordered_map>

#include "cb/core/text.hpp"
#include "cb/decompile/asm_value.hpp"

namespace cb::decompile::tlcs900 {

const std::array<std::string_view, 16> kConditions = {"F", "LT", "LE", "ULE", "OV", "MI", "Z", "C", "T", "GE", "GT", "UGT", "NOV", "PL", "NZ", "NC"};

namespace {

constexpr std::uint8_t B = kSizeB, W = kSizeW, L = kSizeL, BW = B | W, WL = W | L, BWL = B | W | L;

// ---- the table -----------------------------------------------------------------------------------------------------------------------------------------------------------
using enum Tok;

std::vector<Row> make_rows() {
    std::vector<Row> t;
    const auto row = [&](Ctx ctx, std::uint8_t sizes, std::uint8_t op, std::uint8_t mask, Op id, Tok a = None, Tok b = None, std::uint8_t flags = 0) {
        t.push_back(Row{ctx, sizes, op, mask, id, a, b, flags});
    };
    const auto fixed = [&](std::uint8_t op, std::uint8_t mask, Op id, Tok a = None, Tok b = None, std::uint8_t flags = 0, std::uint8_t sizes = 0) {
        row(Ctx::Fixed, sizes, op, mask, id, a, b, flags);
    };

    // ---- the single bytes
    fixed(0x00, 0xFF, Op::Nop);
    fixed(0x02, 0xFF, Op::Push, Sr);
    fixed(0x03, 0xFF, Op::Pop, Sr);
    fixed(0x05, 0xFF, Op::Halt);
    fixed(0x06, 0xFF, Op::Ei, Ib);
    fixed(0x07, 0xFF, Op::Reti);
    fixed(0x08, 0xFF, Op::Ld, A8, Ib);
    fixed(0x09, 0xFF, Op::Push, Ib);
    fixed(0x0A, 0xFF, Op::Ld, A8, Iw, 0, W);
    fixed(0x0B, 0xFF, Op::Push, Iw, None, 0, W);
    fixed(0x0C, 0xFF, Op::Incf);
    fixed(0x0D, 0xFF, Op::Decf);
    fixed(0x0E, 0xFF, Op::Ret);
    fixed(0x0F, 0xFF, Op::Retd, D16);
    fixed(0x10, 0xFF, Op::Rcf);
    fixed(0x11, 0xFF, Op::Scf);
    fixed(0x12, 0xFF, Op::Ccf);
    fixed(0x13, 0xFF, Op::Zcf);
    fixed(0x14, 0xFF, Op::Push, Tok::A);
    fixed(0x15, 0xFF, Op::Pop, Tok::A);
    fixed(0x16, 0xFF, Op::Ex, F, Fp);
    fixed(0x17, 0xFF, Op::Ldf, Ib);
    fixed(0x18, 0xFF, Op::Push, F);
    fixed(0x19, 0xFF, Op::Pop, F);
    fixed(0x1A, 0xFF, Op::Jp, T16);
    fixed(0x1B, 0xFF, Op::Jp, T24);
    fixed(0x1C, 0xFF, Op::Call, T16);
    fixed(0x1D, 0xFF, Op::Call, T24);
    fixed(0x1E, 0xFF, Op::Calr, Rel16);
    fixed(0x20, 0xF8, Op::Ld, R8, Ib);
    fixed(0x28, 0xF8, Op::Push, R16);
    fixed(0x30, 0xF8, Op::Ld, R16, Iw);
    fixed(0x38, 0xF8, Op::Push, R32);
    fixed(0x40, 0xF8, Op::Ld, R32, Il);
    fixed(0x48, 0xF8, Op::Pop, R16);
    fixed(0x58, 0xF8, Op::Pop, R32);
    fixed(0x60, 0xF0, Op::Jr, Cc, Rel8, kOptCc);
    fixed(0x70, 0xF0, Op::Jrl, Cc, Rel16, kOptCc);
    fixed(0xF7, 0xFF, Op::Ldx, A8, Ib, kPad);
    fixed(0xF8, 0xF8, Op::Swi, L3);

    // ---- a register
    const auto reg = [&](std::uint8_t sizes, std::uint8_t op, std::uint8_t mask, Op id, Tok a = None, Tok b = None) { row(Ctx::Reg, sizes, op, mask, id, a, b); };
    reg(BWL, 0x03, 0xFF, Op::Ld, M, Iz);
    reg(BWL, 0x04, 0xFF, Op::Push, M);
    reg(BWL, 0x05, 0xFF, Op::Pop, M);
    reg(BW, 0x06, 0xFF, Op::Cpl, M);
    reg(BW, 0x07, 0xFF, Op::Neg, M);
    reg(BW, 0x08, 0xFF, Op::Mul, Mw, Iz);
    reg(BW, 0x09, 0xFF, Op::Muls, Mw, Iz);
    reg(BW, 0x0A, 0xFF, Op::Div, Mw, Iz);
    reg(BW, 0x0B, 0xFF, Op::Divs, Mw, Iz);
    reg(L, 0x0C, 0xFF, Op::Link, M, D16);
    reg(L, 0x0D, 0xFF, Op::Unlk, M);
    reg(W, 0x0E, 0xFF, Op::Bs1f, Tok::A, M);
    reg(W, 0x0F, 0xFF, Op::Bs1b, Tok::A, M);
    reg(B, 0x10, 0xFF, Op::Daa, M);
    reg(WL, 0x12, 0xFF, Op::Extz, M);
    reg(WL, 0x13, 0xFF, Op::Exts, M);
    reg(WL, 0x14, 0xFF, Op::Paa, M);
    reg(W, 0x16, 0xFF, Op::Mirr, M);
    reg(W, 0x19, 0xFF, Op::Mula, M);
    reg(BW, 0x1C, 0xFF, Op::Djnz, M, Rel8);
    const Op bits[5] = {Op::Andcf, Op::Orcf, Op::Xorcf, Op::Ldcf, Op::Stcf};
    for (unsigned k = 0; k < 5; ++k) {
        reg(BW, static_cast<std::uint8_t>(0x20 + k), 0xFF, bits[k], Bit4, M);
        reg(BW, static_cast<std::uint8_t>(0x28 + k), 0xFF, bits[k], Tok::A, M);
    }
    reg(BWL, 0x2E, 0xFF, Op::Ldc, Cr, M);
    reg(BWL, 0x2F, 0xFF, Op::Ldc, M, Cr);
    const Op bitops[5] = {Op::Res, Op::Set, Op::Chg, Op::Bit, Op::Tset};
    for (unsigned k = 0; k < 5; ++k) reg(BW, static_cast<std::uint8_t>(0x30 + k), 0xFF, bitops[k], Bit4, M);
    reg(W, 0x38, 0xFF, Op::Minc1, Iw, M);
    reg(W, 0x39, 0xFF, Op::Minc2, Iw, M);
    reg(W, 0x3A, 0xFF, Op::Minc4, Iw, M);
    reg(W, 0x3C, 0xFF, Op::Mdec1, Iw, M);
    reg(W, 0x3D, 0xFF, Op::Mdec2, Iw, M);
    reg(W, 0x3E, 0xFF, Op::Mdec4, Iw, M);
    reg(BW, 0x40, 0xF8, Op::Mul, Rmul, M);
    reg(BW, 0x48, 0xF8, Op::Muls, Rmul, M);
    reg(BW, 0x50, 0xF8, Op::Div, Rmul, M);
    reg(BW, 0x58, 0xF8, Op::Divs, Rmul, M);
    reg(BWL, 0x60, 0xF8, Op::Inc, N3, M);
    reg(BWL, 0x68, 0xF8, Op::Dec, N3, M);
    reg(BW, 0x70, 0xF0, Op::Scc, Cc, M);
    const Op alu[8] = {Op::Add, Op::Adc, Op::Sub, Op::Sbc, Op::And, Op::Xor, Op::Or, Op::Cp};
    // 0x80 add r,R; 0x88 ld r,R; 0x90 adc; 0x98 ld R,r; 0xA0 sub; 0xA8 ld R,#3; 0xB0 sbc; 0xB8 ex; 0xC0 and; 0xC8 alu imm; 0xD0 xor; 0xD8 cp #3; 0xE0 or; 0xE8 shifts; 0xF0 cp
    reg(BWL, 0x80, 0xF8, Op::Add, Rz, M);
    reg(BWL, 0x88, 0xF8, Op::Ld, Rz, M);
    reg(BWL, 0x90, 0xF8, Op::Adc, Rz, M);
    reg(BWL, 0x98, 0xF8, Op::Ld, M, Rz);
    reg(BWL, 0xA0, 0xF8, Op::Sub, Rz, M);
    reg(BWL, 0xA8, 0xF8, Op::Ld, M, L3);
    reg(BWL, 0xB0, 0xF8, Op::Sbc, Rz, M);
    reg(BW, 0xB8, 0xF8, Op::Ex, Rz, M);
    reg(BWL, 0xC0, 0xF8, Op::And, Rz, M);
    for (unsigned k = 0; k < 8; ++k) reg(BWL, static_cast<std::uint8_t>(0xC8 + k), 0xFF, alu[k], M, Iz);
    reg(BWL, 0xD0, 0xF8, Op::Xor, Rz, M);
    reg(BWL, 0xD8, 0xF8, Op::Cp, M, L3);
    reg(BWL, 0xE0, 0xF8, Op::Or, Rz, M);
    const Op shifts[8] = {Op::Rlc, Op::Rrc, Op::Rl, Op::Rr, Op::Sla, Op::Sra, Op::Sll, Op::Srl};
    for (unsigned k = 0; k < 8; ++k) {
        reg(BWL, static_cast<std::uint8_t>(0xE8 + k), 0xFF, shifts[k], Cnt4, M);
        reg(BWL, static_cast<std::uint8_t>(0xF8 + k), 0xFF, shifts[k], Tok::A, M);
    }
    reg(BWL, 0xF0, 0xF8, Op::Cp, Rz, M);

    // ---- a memory location as the source
    const auto src = [&](std::uint8_t sizes, std::uint8_t op, std::uint8_t mask, Op id, Tok a = None, Tok b = None, std::uint8_t flags = 0) { row(Ctx::Src, sizes, op, mask, id, a, b, flags); };
    src(BW, 0x04, 0xFF, Op::Push, M);
    src(B, 0x06, 0xFF, Op::Rld, Tok::A, M);
    src(B, 0x07, 0xFF, Op::Rrd, Tok::A, M);
    src(BW, 0x10, 0xFF, Op::Ldi, BlkD, BlkS, kBlockInc);
    src(BW, 0x11, 0xFF, Op::Ldir, BlkD, BlkS, kBlockInc);
    src(BW, 0x12, 0xFF, Op::Ldd, BlkD, BlkS, kBlockDec);
    src(BW, 0x13, 0xFF, Op::Lddr, BlkD, BlkS, kBlockDec);
    src(BW, 0x14, 0xFF, Op::Cpi, Acc, BlkS, kBlockInc);
    src(BW, 0x15, 0xFF, Op::Cpir, Acc, BlkS, kBlockInc);
    src(BW, 0x16, 0xFF, Op::Cpd, Acc, BlkS, kBlockDec);
    src(BW, 0x17, 0xFF, Op::Cpdr, Acc, BlkS, kBlockDec);
    src(BW, 0x19, 0xFF, Op::Ld, A16, M);
    src(BWL, 0x20, 0xF8, Op::Ld, Rz, M);
    src(BW, 0x30, 0xF8, Op::Ex, M, Rz);
    for (unsigned k = 0; k < 8; ++k) src(BW, static_cast<std::uint8_t>(0x38 + k), 0xFF, alu[k], M, Iz);
    src(BW, 0x40, 0xF8, Op::Mul, Rmul, M);
    src(BW, 0x48, 0xF8, Op::Muls, Rmul, M);
    src(BW, 0x50, 0xF8, Op::Div, Rmul, M);
    src(BW, 0x58, 0xF8, Op::Divs, Rmul, M);
    src(BW, 0x60, 0xF8, Op::Inc, N3, M);
    src(BW, 0x68, 0xF8, Op::Dec, N3, M);
    for (unsigned k = 0; k < 8; ++k) src(BW, static_cast<std::uint8_t>(0x78 + k), 0xFF, shifts[k], M);
    for (unsigned k = 0; k < 8; ++k) {
        src(BWL, static_cast<std::uint8_t>(0x80 + 0x10 * k), 0xF8, alu[k], Rz, M);
        src(BWL, static_cast<std::uint8_t>(0x88 + 0x10 * k), 0xF8, alu[k], M, Rz);
    }

    // ---- a memory location as the destination (and the address operations)
    const auto dst = [&](std::uint8_t width, std::uint8_t op, std::uint8_t mask, Op id, Tok a = None, Tok b = None, std::uint8_t flags = 0) { row(Ctx::Dst, width, op, mask, id, a, b, flags); };
    dst(B, 0x00, 0xFF, Op::Ld, M, Ib);
    dst(W, 0x02, 0xFF, Op::Ld, M, Iw);
    dst(B, 0x04, 0xFF, Op::Pop, M);
    dst(W, 0x06, 0xFF, Op::Pop, M);
    dst(B, 0x14, 0xFF, Op::Ld, M, A16);
    dst(W, 0x16, 0xFF, Op::Ld, M, A16);
    dst(W, 0x20, 0xF8, Op::Lda, R16, M);
    for (unsigned k = 0; k < 5; ++k) dst(B, static_cast<std::uint8_t>(0x28 + k), 0xFF, bits[k], Tok::A, M);
    dst(L, 0x30, 0xF8, Op::Lda, R32, M);
    dst(B, 0x40, 0xF8, Op::Ld, M, R8);
    dst(W, 0x50, 0xF8, Op::Ld, M, R16);
    dst(L, 0x60, 0xF8, Op::Ld, M, R32);
    const Op membits[10] = {Op::Andcf, Op::Orcf, Op::Xorcf, Op::Ldcf, Op::Stcf, Op::Tset, Op::Res, Op::Set, Op::Chg, Op::Bit};
    for (unsigned k = 0; k < 10; ++k) dst(B, static_cast<std::uint8_t>(0x80 + 8 * k), 0xF8, membits[k], L3, M);
    dst(B, 0xD0, 0xF0, Op::Jp, Cc, M, kOptCc);
    dst(B, 0xE0, 0xF0, Op::Call, Cc, M, kOptCc);
    dst(0, 0xF0, 0xF0, Op::Ret, Cc, None, kOptCc | kBare);
    return t;
}

// ---- registers --------------------------------------------------------------------------------------------------------------------------------------------------------------
constexpr std::array<const char*, 8> kR16 = {"WA", "BC", "DE", "HL", "IX", "IY", "IZ", "SP"};
constexpr std::array<const char*, 8> kR32 = {"XWA", "XBC", "XDE", "XHL", "XIX", "XIY", "XIZ", "XSP"};
// The byte parts of each extended register, by position 0..3 (the low byte is first).
constexpr std::array<std::array<const char*, 4>, 8> kBytes = {{{"A", "W", "QA", "QW"}, {"C", "B", "QC", "QB"}, {"E", "D", "QE", "QD"}, {"L", "H", "QL", "QH"},
                                                            {"IXL", "IXH", "QIXL", "QIXH"}, {"IYL", "IYH", "QIYL", "QIYH"}, {"IZL", "IZH", "QIZL", "QIZH"}, {"SPL", "SPH", "QSPL", "QSPH"}}};

struct Place {
    unsigned group, pos;
    std::string prefix, suffix;
};

std::optional<Place> place_of(std::uint8_t code) {
    if (code >= 0xE0) return Place{static_cast<unsigned>((code >> 2) & 7u), static_cast<unsigned>(code & 3u), "", ""};
    if (code >= 0xD0) return Place{static_cast<unsigned>((code >> 2) & 3u), static_cast<unsigned>(code & 3u), "", "'"};
    if (code >= 0x40) return std::nullopt;   // (banks 0-3 only)
    return Place{static_cast<unsigned>((code & 0x0Fu) >> 2), static_cast<unsigned>(code & 3u), "R", std::to_string(code >> 4)};
}

std::uint8_t current_code(unsigned size, unsigned r) {   // the register code of the short form's register r
    if (size == 1) return static_cast<std::uint8_t>(0xE0 + 4 * (r >> 1) + ((r & 1) ^ 1));
    return static_cast<std::uint8_t>(0xE0 + 4 * r);
}

// The short-form register number of a code (the C8+r forms), if the code is one.
std::optional<unsigned> short_number(std::uint8_t code, unsigned size) {
    if (code < 0xE0) return std::nullopt;
    const unsigned group = (code >> 2) & 7u, pos = code & 3u;
    if (size == 1) return pos < 2 && group < 4 ? std::optional<unsigned>(2 * group + (pos ^ 1u)) : std::nullopt;
    return pos == 0 ? std::optional<unsigned>(group) : std::nullopt;
}

}  // namespace

std::optional<std::string> rcode_name(std::uint8_t code, unsigned size) {
    const std::optional<Place> p = place_of(code);
    if (!p) return std::nullopt;
    if (size == 1) return p->prefix + kBytes[p->group][p->pos] + p->suffix;
    if (size == 2) {
        if (p->pos & 1u) return std::nullopt;
        return p->prefix + (p->pos == 2 ? "Q" : "") + kR16[p->group] + p->suffix;
    }
    if (p->pos != 0) return std::nullopt;
    return std::string(kR32[p->group]) + p->suffix;
}

std::optional<RegName> register_by_name(std::string_view name) {
    static const std::unordered_map<std::string, RegName> table = [] {
        std::unordered_map<std::string, RegName> t;
        for (unsigned code = 0; code < 256; ++code) {
            for (const unsigned size : {1u, 2u, 4u}) {
                if (const std::optional<std::string> n = rcode_name(static_cast<std::uint8_t>(code), size)) t.try_emplace(*n, RegName{static_cast<std::uint8_t>(code), size});
            }
        }
        return t;
    }();
    const auto it = table.find(std::string(name));
    return it == table.end() ? std::nullopt : std::optional<RegName>(it->second);
}

std::string control_register_name(unsigned code) {
    static const std::map<unsigned, const char*> names = {{0x00, "DMAS0"}, {0x04, "DMAS1"}, {0x08, "DMAS2"}, {0x0C, "DMAS3"}, {0x10, "DMAD0"}, {0x14, "DMAD1"}, {0x18, "DMAD2"},
                                                         {0x1C, "DMAD3"}, {0x20, "DMAC0"}, {0x22, "DMAM0"}, {0x24, "DMAC1"}, {0x26, "DMAM1"}, {0x28, "DMAC2"}, {0x2A, "DMAM2"},
                                                         {0x2C, "DMAC3"}, {0x2E, "DMAM3"}, {0x30, "INTNEST"}};
    const auto it = names.find(code);
    return it != names.end() ? it->second : std::format("CR_0x{:02X}", code);
}

const std::vector<Row>& rows() {
    static const std::vector<Row> table = make_rows();
    return table;
}

std::string_view op_name(Op id) {
    static constexpr std::array<std::string_view, static_cast<std::size_t>(Op::Bs1b) + 1> names = {
        "nop",  "push", "pop",  "ei",    "halt",  "reti",  "ld",    "incf",  "decf",  "ret",   "retd",  "rcf",   "scf",   "ccf",   "zcf",   "ex",    "ldf",   "jp",    "call",  "calr",
        "jr",   "jrl",  "swi",  "lda",   "ldx",   "link",  "unlk",  "cpl",   "neg",   "mul",   "muls",  "div",   "divs",  "daa",   "extz",  "exts",  "paa",   "mirr",  "mula",  "djnz",
        "andcf", "orcf", "xorcf", "ldcf", "stcf", "ldc",   "res",   "set",   "chg",   "bit",   "tset",  "minc1", "minc2", "minc4", "mdec1", "mdec2", "mdec4", "inc",   "dec",   "scc",
        "add",  "adc",  "sub",  "sbc",   "and",   "xor",   "or",    "cp",    "rlc",   "rrc",   "rl",    "rr",    "sla",   "sra",   "sll",   "srl",   "rld",   "rrd",   "ldi",   "ldir",
        "ldd",  "lddr", "cpi",  "cpir",  "cpd",   "cpdr",  "bs1f",  "bs1b"};
    return names[static_cast<std::size_t>(id)];
}

namespace {

// ---- what a token is ---------------------------------------------------------------------------------------------------------------------------------------------------------
std::size_t trailing_size(Tok t, unsigned z) {
    switch (t) {
        case Ib: case Bit4: case Cnt4: case Cr: case Rel8: case A8: return 1;
        case Iw: case D16: case A16: case T16: case Rel16: return 2;
        case T24: return 3;
        case Il: return 4;
        case Iz: return z;
        default: return 0;
    }
}

bool carries_size(const Row& r) {
    if (r.ctx == Ctx::Reg) return true;   // (the register is the operand)
    for (const Tok t : {r.a, r.b}) {
        if (t == Rz || t == R8 || t == R16 || t == R32 || t == Rmul || t == Acc) return true;
    }
    return false;
}

unsigned width_of(const Row& r) { return r.sizes & L ? 4 : r.sizes & W ? 2 : r.sizes & B ? 1 : 0; }

// ---- decoding ----------------------------------------------------------------------------------------------------------------------------------------------------------------
std::int32_t s8(std::uint8_t v) { return static_cast<std::int8_t>(v); }
std::int32_t s16(std::uint32_t v) { return static_cast<std::int16_t>(v); }

struct Reader {
    ByteView code;
    std::size_t base;   // the offset of the instruction's first byte
    std::size_t at = 0;   // bytes consumed
    bool ok = true;
    std::uint8_t next() {
        if (base + at >= code.size()) {
            ok = false;
            return 0;
        }
        return code[base + at++];
    }
    std::uint32_t le(unsigned n) {
        std::uint32_t v = 0;
        for (unsigned k = 0; k < n; ++k) v |= static_cast<std::uint32_t>(next()) << (8 * k);
        return v;
    }
};

bool valid_code(std::uint8_t code, unsigned size) { return rcode_name(code, size).has_value(); }

// The memory operand of a first byte in the memory classes (the reader is just past the first byte).
std::optional<Mem> read_mem(std::uint8_t first, Reader& in, std::uint32_t pc) {
    Mem m;
    if (first < 0xC0) {
        const unsigned low = first & 0x0Fu;
        m.reg = current_code(4, low & 7u);
        m.is_short = true;
        if (low < 8) {
            m.kind = MemKind::Reg;
        } else {
            m.kind = MemKind::RegD8;
            m.disp = s8(in.next());
        }
        return in.ok ? std::optional<Mem>(m) : std::nullopt;
    }
    switch (first & 7u) {
        case 0: m.kind = MemKind::Abs8; m.addr = in.next(); break;
        case 1: m.kind = MemKind::Abs16; m.addr = in.le(2); break;
        case 2: m.kind = MemKind::Abs24; m.addr = in.le(3); break;
        case 3: {
            const std::uint8_t mode = in.next();
            switch (mode & 3u) {
                case 0:
                    if (!valid_code(mode, 4)) return std::nullopt;
                    m.kind = MemKind::Reg;
                    m.reg = mode;
                    break;
                case 1:
                    if (!valid_code(mode & 0xFC, 4)) return std::nullopt;
                    m.kind = MemKind::RegD16;
                    m.reg = mode & 0xFC;
                    m.disp = s16(in.le(2));
                    break;
                case 2: return std::nullopt;
                default:
                    if (mode == 0x03 || mode == 0x07) {
                        const std::uint8_t base = in.next(), idx = in.next();
                        const unsigned isz = mode == 0x03 ? 1 : 2;
                        if (!valid_code(base, 4) || !valid_code(idx, isz)) return std::nullopt;
                        m.kind = isz == 1 ? MemKind::RegIdx8 : MemKind::RegIdx16;
                        m.reg = base;
                        m.idx = idx;
                    } else if (mode == 0x13) {
                        const std::int32_t d = s16(in.le(2));
                        m.kind = MemKind::PcRel;
                        m.addr = (pc + 4 + static_cast<std::uint32_t>(d)) & 0xFFFFFF;
                    } else {
                        return std::nullopt;
                    }
            }
            break;
        }
        case 4:
        case 5: {
            const std::uint8_t code = in.next();
            const unsigned s = code & 3u;
            if (s == 3 || !valid_code(code & 0xFC, 4)) return std::nullopt;
            m.kind = (first & 7u) == 4 ? MemKind::PreDec : MemKind::PostInc;
            m.reg = code & 0xFC;
            m.step = static_cast<std::uint8_t>(1u << s);
            break;
        }
        default: return std::nullopt;
    }
    return in.ok ? std::optional<Mem>(m) : std::nullopt;
}

// The register of twice the size a byte / word register code stands for (the destination of mul / div).
std::optional<std::uint8_t> widened(std::uint8_t code, unsigned z) {
    if ((code & 3u) != 0 || !valid_code(code, 2 * z)) return std::nullopt;
    return code;
}

std::uint32_t target_of(const Insn& i, Tok t, std::uint32_t value) {
    (void)i;
    (void)t;
    return value & 0xFFFFFF;
}

void finish_flow(Insn& i) {
    const Row& r = *i.row;
    const bool abs_mem = (r.ctx == Ctx::Dst) && (i.m.kind == MemKind::Abs8 || i.m.kind == MemKind::Abs16 || i.m.kind == MemKind::Abs24 || i.m.kind == MemKind::PcRel);
    const unsigned cc = r.a == Cc ? (i.op & 0x0Fu) : 8u;
    switch (r.id) {
        case Op::Jp:
            if (r.a == T16 || r.a == T24) {
                i.flow = Flow::Jump;
            } else if (abs_mem) {
                i.target = i.m.addr;
                i.flow = cc == 8 ? Flow::Jump : Flow::Branch;
            } else {
                i.flow = cc == 8 ? Flow::Indirect : Flow::None;
            }
            break;
        case Op::Call:
            if (abs_mem) i.target = i.m.addr;
            i.flow = (r.a == T16 || r.a == T24 || abs_mem) ? Flow::Call : Flow::None;
            break;
        case Op::Calr: i.flow = Flow::Call; break;
        case Op::Jr:
        case Op::Jrl: i.flow = cc == 8 ? Flow::Jump : Flow::Branch; break;
        case Op::Djnz: i.flow = Flow::Branch; break;
        case Op::Ret: i.flow = cc == 8 ? Flow::Return : Flow::None; break;
        case Op::Retd:
        case Op::Reti: i.flow = Flow::Return; break;
        default: break;
    }
    if (i.flow == Flow::Jump || i.flow == Flow::Branch || i.flow == Flow::Call) {
        for (unsigned k = 0; k < 2; ++k) {
            const Tok t = k == 0 ? r.a : r.b;
            if (t == Rel8 || t == Rel16 || t == T16 || t == T24) i.target = i.v[k];
        }
    }
}

}  // namespace

std::optional<Insn> decode(ByteView code, std::size_t offset, std::uint32_t pc) {
    if (offset >= code.size()) return std::nullopt;
    Reader in{code, offset};
    Insn insn;
    insn.at = pc;
    const std::uint8_t first = in.next();
    insn.first = first;
    const std::vector<Row>& table = rows();
    Ctx ctx;
    unsigned z = 0;
    if (first < 0x80 || first == 0xF7 || first >= 0xF8) {
        ctx = Ctx::Fixed;
    } else if (first >= 0xB0 && first < 0xC0) {
        ctx = Ctx::Dst;
    } else if (first < 0xB0) {
        ctx = Ctx::Src;
        z = first < 0x90 ? 1 : first < 0xA0 ? 2 : 4;
    } else if (first >= 0xF0 && first <= 0xF5) {
        ctx = Ctx::Dst;
    } else if (first >= 0xC0 && first <= 0xC5) {
        ctx = Ctx::Src;
        z = 1;
    } else if (first >= 0xD0 && first <= 0xD5) {
        ctx = Ctx::Src;
        z = 2;
    } else if (first >= 0xE0 && first <= 0xE5) {
        ctx = Ctx::Src;
        z = 4;
    } else if (first == 0xC7 || first == 0xD7 || first == 0xE7 || (first >= 0xC8 && first <= 0xCF) || (first >= 0xD8 && first <= 0xDF) || (first >= 0xE8 && first <= 0xEF)) {
        ctx = Ctx::Reg;
        z = (first & 0xF0) == 0xC0 ? 1 : (first & 0xF0) == 0xD0 ? 2 : 4;
        if (first & 8u) {
            insn.reg = current_code(z, first & 7u);
        } else {
            insn.reg = in.next();
            if (!valid_code(insn.reg, z)) return std::nullopt;
        }
    } else {
        return std::nullopt;
    }
    insn.z = static_cast<std::uint8_t>(z);
    if (ctx == Ctx::Src || ctx == Ctx::Dst) {
        const std::optional<Mem> m = read_mem(first, in, pc);
        if (!m) return std::nullopt;
        insn.m = *m;
    }
    const std::uint8_t op = ctx == Ctx::Fixed ? first : in.next();
    if (!in.ok) return std::nullopt;
    insn.op = op;
    const Row* found = nullptr;
    for (const Row& r : table) {
        if (r.ctx != ctx || (op & r.mask) != r.op) continue;
        if ((ctx == Ctx::Src || ctx == Ctx::Reg) && !(r.sizes & z)) continue;
        found = &r;
        break;
    }
    if (!found) return std::nullopt;
    const Row& row = *found;
    insn.row = found;
    // what the table's rule for the first byte and the operation byte forbids
    if (row.flags & kBare) {
        if (first != 0xB0) return std::nullopt;
    }
    if (row.a == BlkS || row.b == BlkS) {
        if (insn.m.kind != MemKind::Reg || !insn.m.is_short) return std::nullopt;
        const unsigned idx = (insn.m.reg >> 2) & 7u;
        if (row.a == BlkD && idx != 3 && idx != 5) return std::nullopt;
    }
    for (const Tok t : {row.a, row.b}) {
        if (t == Rmul && z == 1 && !(op & 1u)) return std::nullopt;
        if (t == Mw && !widened(insn.reg, z)) return std::nullopt;
        if (t == Mw && z == 1 && (insn.reg & 3u) != 0) return std::nullopt;
    }
    // the bytes after the operation
    std::uint32_t values[2] = {0, 0};
    const bool pad = row.flags & kPad;
    for (unsigned k = 0; k < 2; ++k) {
        const Tok t = k == 0 ? row.a : row.b;
        const std::size_t n = trailing_size(t, z);
        if (n == 0) continue;
        if (pad && in.next() != 0) return std::nullopt;
        std::uint32_t v = in.le(static_cast<unsigned>(n));
        if (t == Rel8) v = static_cast<std::uint32_t>(s8(static_cast<std::uint8_t>(v)));
        if (t == Rel16 || t == D16) v = static_cast<std::uint32_t>(s16(v));
        values[k] = v;
    }
    if (pad && in.next() != 0) return std::nullopt;
    if (!in.ok || in.at > insn.raw.size()) return std::nullopt;
    insn.size = static_cast<std::uint8_t>(in.at);
    for (unsigned k = 0; k < 2; ++k) {
        const Tok t = k == 0 ? row.a : row.b;
        if (t == Rel8 || t == Rel16) values[k] = target_of(insn, t, pc + insn.size + values[k]);
        insn.v[k] = values[k];
    }
    std::copy(code.begin() + static_cast<std::ptrdiff_t>(offset), code.begin() + static_cast<std::ptrdiff_t>(offset + in.at), insn.raw.begin());
    finish_flow(insn);
    return insn;
}

// ---- text ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
namespace {

// The mnemonic of a row at an operand size (z: the size for register and source rows, 0 for the others, which have a width of their own).
std::string mnemonic_for(const Row& r, unsigned z) {
    std::string m(op_name(r.id));
    const unsigned size = (r.ctx == Ctx::Src || r.ctx == Ctx::Reg) ? z : width_of(r);
    if (!carries_size(r) && size == 2) m += 'w';
    return m;
}

std::string hex(std::uint32_t v, unsigned digits) { return std::format("0x{:0{}X}", v, digits); }

std::string named(std::uint32_t v, unsigned digits, const AddressLabels& labels) {
    const auto it = labels.find(v);
    return it != labels.end() ? it->second : hex(v, digits);
}

std::string reg_text(std::uint8_t code, unsigned size) {
    const std::optional<std::string> n = rcode_name(code, size);
    return n ? *n : std::format("?{:02X}", code);
}

unsigned data_width(const Insn& i) { return i.z ? i.z : width_of(*i.row); }

std::string mem_text(const Insn& i, const AddressLabels& labels) {
    const Mem& m = i.m;
    switch (m.kind) {
        case MemKind::Reg: return "(" + reg_text(m.reg, 4) + ")";
        case MemKind::RegD8:
        case MemKind::RegD16: return std::format("({}{}{})", reg_text(m.reg, 4), m.disp < 0 ? '-' : '+', std::abs(m.disp));
        case MemKind::Abs8: return "(" + named(m.addr, 2, labels) + ")";
        case MemKind::Abs16: return "(" + named(m.addr, 4, labels) + ")";
        case MemKind::Abs24: return "(" + named(m.addr, 6, labels) + ")";
        case MemKind::RegIdx8: return std::format("({}+{})", reg_text(m.reg, 4), reg_text(m.idx, 1));
        case MemKind::RegIdx16: return std::format("({}+{})", reg_text(m.reg, 4), reg_text(m.idx, 2));
        case MemKind::PcRel: return "(pc:" + named(m.addr, 6, labels) + ")";
        case MemKind::PreDec:
        case MemKind::PostInc: {
            const std::string step = m.step == data_width(i) ? std::string() : "/" + std::to_string(m.step);
            return m.kind == MemKind::PreDec ? "(-" + reg_text(m.reg, 4) + step + ")" : "(" + reg_text(m.reg, 4) + "+" + step + ")";
        }
    }
    return {};
}

std::string token_text(const Insn& i, Tok t, unsigned slot, const AddressLabels& labels) {
    const Row& r = *i.row;
    const unsigned z = i.z;
    const unsigned low3 = i.op & 7u;
    switch (t) {
        case None: return {};
        case M: return r.ctx == Ctx::Reg ? reg_text(i.reg, z) : mem_text(i, labels);
        case Mw: return reg_text(i.reg, 2 * z);
        case Rz: return reg_text(current_code(z, low3), z);
        case R8: return reg_text(current_code(1, low3), 1);
        case R16: return reg_text(current_code(2, low3), 2);
        case R32: return reg_text(current_code(4, low3), 4);
        case Rmul: return z == 1 ? reg_text(current_code(2, low3 >> 1), 2) : reg_text(current_code(4, low3), 4);
        case Tok::A: return "A";
        case Acc: return z == 1 ? "A" : "WA";
        case Ib: return hex(i.v[slot], 2);
        case Iw: return hex(i.v[slot], 4);
        case Il: return i.v[slot] >= 0x200000 ? named(i.v[slot], 8, labels) : hex(i.v[slot], 8);   // (a label for an address of the cartridge only)
        case Iz: return z == 1 ? hex(i.v[slot], 2) : z == 2 ? hex(i.v[slot], 4) : i.v[slot] >= 0x200000 ? named(i.v[slot], 8, labels) : hex(i.v[slot], 8);
        case L3: return std::to_string(low3);
        case N3: return std::to_string(low3 == 0 ? 8 : low3);
        case Bit4: return std::to_string(i.v[slot] & 15u);
        case Cnt4: return std::to_string((i.v[slot] & 15u) == 0 ? 16 : (i.v[slot] & 15u));
        case Cc: return std::string(kConditions[i.op & 15u]);
        case Cr: return control_register_name(i.v[slot] & 0xFF);
        case Rel8:
        case Rel16: return named(i.v[slot], 6, labels);
        case D16: return std::to_string(static_cast<std::int32_t>(i.v[slot]));
        case T16: return named(i.v[slot], 4, labels);
        case T24: return named(i.v[slot], 6, labels);
        case A8: return "(" + named(i.v[slot], 2, labels) + ")";
        case A16: return "(" + named(i.v[slot], 4, labels) + ")";
        case BlkD: {
            const unsigned idx = (i.m.reg >> 2) & 7u;
            return std::string("(") + (idx == 3 ? "XDE" : "XIX") + (r.flags & kBlockDec ? "-" : "+") + ")";
        }
        case BlkS: return "(" + reg_text(i.m.reg, 4) + (r.flags & kBlockDec ? "-" : "+") + ")";
        case Sr: return "SR";
        case F: return "F";
        case Fp: return "F'";
    }
    return {};
}

}  // namespace

std::string text(const Insn& insn, const AddressLabels& labels) {
    const Row& r = *insn.row;
    if (r.id == Op::Ei && insn.v[0] == 7) return "di";
    std::string out = mnemonic_for(r, insn.z);
    std::string operands;
    const auto add = [&](Tok t, unsigned slot) {
        if (t == None) return;
        if (t == Cc && (r.flags & kOptCc) && (insn.op & 15u) == 8) return;
        if (!operands.empty()) operands += ',';
        operands += token_text(insn, t, slot, labels);
    };
    add(r.a, 0);
    add(r.b, 1);
    return operands.empty() ? out : out + " " + operands;
}

// ---- the assembler -------------------------------------------------------------------------------------------------------------------------------------------------------
namespace {

struct Variant {
    const Row* row;
    unsigned z;
};

const std::unordered_map<std::string, std::vector<Variant>>& variants() {
    static const std::unordered_map<std::string, std::vector<Variant>> table = [] {
        std::unordered_map<std::string, std::vector<Variant>> t;
        for (const Row& r : rows()) {
            if (r.ctx == Ctx::Src || r.ctx == Ctx::Reg) {
                for (const unsigned z : {1u, 2u, 4u}) {
                    if (r.sizes & z) t[mnemonic_for(r, z)].push_back(Variant{&r, z});
                }
            } else {
                t[mnemonic_for(r, 0)].push_back(Variant{&r, 0});
            }
        }
        return t;
    }();
    return table;
}

std::string lower(std::string_view s) { return text::lower(s); }

std::string upper(std::string_view s) {
    std::string out(s);
    for (char& c : out) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    return out;
}

std::vector<std::string> split_operands(std::string_view s) {
    std::vector<std::string> out;
    int depth = 0;
    std::string cur;
    for (const char c : s) {
        if (c == '(') ++depth;
        if (c == ')') --depth;
        if (c == ',' && depth == 0) {
            out.push_back(std::string(text::trim(cur)));
            cur.clear();
        } else {
            cur += c;
        }
    }
    if (!text::trim(cur).empty() || !out.empty()) out.push_back(std::string(text::trim(cur)));
    return out;
}

struct Fail {};   // thrown inside a variant's binding: this variant does not take the line

struct Ctxt {
    std::uint32_t pc;
    const NameLabels& labels;
    bool lenient;
};

// A number as the text spelt it: its value, and how many hex digits a `0x` literal had (0 for anything else), and whether it was a plain decimal.
struct Number {
    std::int64_t value = 0;
    unsigned digits = 0;
    bool decimal = false;
};

Number number_of(std::string_view s, const Ctxt& cx) {
    Number n;
    const std::string_view e = text::trim(s);
    if (e.empty()) throw Fail{};
    for (std::size_t k = 0; k < e.size();) {   // (a register is not a number, even when a first pass lets an unknown name stand for 0)
        if (std::isalpha(static_cast<unsigned char>(e[k]))) {
            std::size_t j = k;
            while (j < e.size() && (std::isalnum(static_cast<unsigned char>(e[j])) || e[j] == '\'' || e[j] == '_')) ++j;
            if (register_by_name(e.substr(k, j - k))) throw Fail{};
            k = j;
        } else {
            ++k;
        }
    }
    if (e.size() > 2 && e[0] == '0' && (e[1] == 'x' || e[1] == 'X') && std::all_of(e.begin() + 2, e.end(), [](char c) { return std::isxdigit(static_cast<unsigned char>(c)) != 0; })) {
        n.digits = static_cast<unsigned>(e.size() - 2);
    } else {
        n.decimal = std::all_of(e.begin(), e.end(), [](char c) { return std::isdigit(static_cast<unsigned char>(c)) != 0; });
    }
    try {
        n.value = asm_value::eval(e, cx.labels, cx.lenient);
    } catch (const Error&) {
        throw Fail{};
    }
    return n;
}

struct Piece {
    Bytes pre;     // the bytes before the operation byte
    Bytes post;    // the bytes after it
};

std::uint8_t step_bits(unsigned step) { return static_cast<std::uint8_t>(step == 1 ? 0 : step == 2 ? 1 : 2); }

// Encode a memory operand's text for the first-byte class (`src`: the size's class, else the destination class).
struct MemOperand {
    Mem m;
    bool digits_forced = false;
    unsigned digits = 0;
    bool step_given = false;
};

MemOperand parse_mem(std::string_view s, const Ctxt& cx) {
    std::string_view t = text::trim(s);
    if (t.size() < 2 || t.front() != '(' || t.back() != ')') throw Fail{};
    t = text::trim(t.substr(1, t.size() - 2));
    MemOperand out;
    Mem& m = out.m;
    const auto reg32 = [&](std::string_view name) -> std::optional<std::uint8_t> {
        const std::optional<RegName> r = register_by_name(name);
        if (r && r->size == 4) return r->code;
        return std::nullopt;
    };
    if (t.starts_with("pc:")) {
        m.kind = MemKind::PcRel;
        m.addr = static_cast<std::uint32_t>(number_of(t.substr(3), cx).value) & 0xFFFFFF;
        return out;
    }
    // -REG[/n] or REG+[/n]
    const auto stepped = [&](std::string_view reg, std::string_view tail) {
        const std::optional<std::uint8_t> r = reg32(reg);
        if (!r) throw Fail{};
        m.reg = *r;
        if (!tail.empty()) {
            if (tail.front() != '/') throw Fail{};
            const std::string_view n = tail.substr(1);
            if (n != "1" && n != "2" && n != "4") throw Fail{};
            m.step = static_cast<std::uint8_t>(n[0] - '0');
            out.step_given = true;
        }
    };
    if (t.front() == '-') {
        const std::string_view rest = t.substr(1);
        const std::size_t slash = rest.find('/');
        if (reg32(rest.substr(0, slash))) {
            m.kind = MemKind::PreDec;
            stepped(rest.substr(0, slash), slash == std::string_view::npos ? std::string_view() : rest.substr(slash));
            return out;
        }
    }
    std::size_t j = 0;
    while (j < t.size() && (std::isalnum(static_cast<unsigned char>(t[j])) || t[j] == '\'' || t[j] == '_')) ++j;
    const std::string_view head = t.substr(0, j);
    if (const std::optional<std::uint8_t> r = reg32(head)) {
        const std::string_view tail = text::trim(t.substr(j));
        m.reg = *r;
        if (tail.empty()) {
            m.kind = MemKind::Reg;
            return out;
        }
        if (tail.front() == '+' && (tail.size() == 1 || tail[1] == '/')) {
            m.kind = MemKind::PostInc;
            stepped(head, tail.substr(1));
            return out;
        }
        if (tail.front() != '+' && tail.front() != '-') throw Fail{};
        const std::string_view rest = text::trim(tail.substr(1));
        if (tail.front() == '+') {
            if (const std::optional<RegName> ir = register_by_name(rest); ir && (ir->size == 1 || ir->size == 2)) {
                m.kind = ir->size == 1 ? MemKind::RegIdx8 : MemKind::RegIdx16;
                m.idx = ir->code;
                return out;
            }
        }
        std::int64_t d = number_of(rest, cx).value;
        if (tail.front() == '-') d = -d;
        if (d < -32768 || d > 32767) throw Fail{};
        m.kind = d >= -128 && d <= 127 && m.reg >= 0xE0 ? MemKind::RegD8 : MemKind::RegD16;
        m.disp = static_cast<std::int32_t>(d);
        return out;
    }
    const Number n = number_of(t, cx);
    if (n.value < 0 || n.value > 0xFFFFFF) throw Fail{};
    m.addr = static_cast<std::uint32_t>(n.value);
    out.digits = n.digits;
    if (n.digits != 0) {
        if (n.digits != 2 && n.digits != 4 && n.digits != 6) throw Fail{};
        if ((n.digits == 2 && m.addr > 0xFF) || (n.digits == 4 && m.addr > 0xFFFF)) throw Fail{};
        m.kind = n.digits == 2 ? MemKind::Abs8 : n.digits == 4 ? MemKind::Abs16 : MemKind::Abs24;
    } else {
        m.kind = m.addr <= 0xFF ? MemKind::Abs8 : m.addr <= 0xFFFF ? MemKind::Abs16 : MemKind::Abs24;
    }
    return out;
}

// The bytes before the operation byte for a memory operand: its class, and the operand.
Bytes mem_bytes(const MemOperand& mo, Ctx ctx, unsigned z, unsigned width, const Ctxt& cx) {
    const Mem& m = mo.m;
    const std::uint8_t base = ctx == Ctx::Dst ? 0xB0 : z == 1 ? 0x80 : z == 2 ? 0x90 : 0xA0;
    const std::uint8_t ext = ctx == Ctx::Dst ? 0xF0 : z == 1 ? 0xC0 : z == 2 ? 0xD0 : 0xE0;
    Bytes b;
    switch (m.kind) {
        case MemKind::Reg:
            if (m.reg >= 0xE0) {
                b.push_back(static_cast<std::uint8_t>(base + ((m.reg >> 2) & 7u)));
            } else {
                b = {static_cast<std::uint8_t>(ext + 3), m.reg};
            }
            break;
        case MemKind::RegD8: b = {static_cast<std::uint8_t>(base + 8 + ((m.reg >> 2) & 7u)), static_cast<std::uint8_t>(m.disp)}; break;
        case MemKind::RegD16: b = {static_cast<std::uint8_t>(ext + 3), static_cast<std::uint8_t>(m.reg | 1), static_cast<std::uint8_t>(m.disp), static_cast<std::uint8_t>(m.disp >> 8)}; break;
        case MemKind::RegIdx8: b = {static_cast<std::uint8_t>(ext + 3), 0x03, m.reg, m.idx}; break;
        case MemKind::RegIdx16: b = {static_cast<std::uint8_t>(ext + 3), 0x07, m.reg, m.idx}; break;
        case MemKind::Abs8: b = {ext, static_cast<std::uint8_t>(m.addr)}; break;
        case MemKind::Abs16: b = {static_cast<std::uint8_t>(ext + 1), static_cast<std::uint8_t>(m.addr), static_cast<std::uint8_t>(m.addr >> 8)}; break;
        case MemKind::Abs24:
            b = {static_cast<std::uint8_t>(ext + 2), static_cast<std::uint8_t>(m.addr), static_cast<std::uint8_t>(m.addr >> 8), static_cast<std::uint8_t>(m.addr >> 16)};
            break;
        case MemKind::PcRel: {
            const std::int64_t d = static_cast<std::int64_t>(m.addr) - (static_cast<std::int64_t>(cx.pc) + 4);
            if (!cx.lenient && (d < -32768 || d > 32767)) throw Fail{};
            b = {static_cast<std::uint8_t>(ext + 3), 0x13, static_cast<std::uint8_t>(d), static_cast<std::uint8_t>(d >> 8)};
            break;
        }
        case MemKind::PreDec:
        case MemKind::PostInc: {
            const unsigned step = mo.step_given ? m.step : width;
            if (step != 1 && step != 2 && step != 4) throw Fail{};
            if ((m.reg & 3u) != 0) throw Fail{};
            b = {static_cast<std::uint8_t>(ext + (m.kind == MemKind::PreDec ? 4 : 5)), static_cast<std::uint8_t>(m.reg | step_bits(step))};
            break;
        }
    }
    return b;
}

// Everything one variant's binding gathers.
struct Bound {
    int low = -1;              // the value for the operation byte's low bits
    std::uint8_t low_mask = 0;
    std::optional<MemOperand> mem;
    std::optional<RegName> reg;   // Ctx::Reg: the first byte's register
    Bytes tail;                // the bytes after the operation, in order
    std::vector<std::pair<std::size_t, std::pair<Tok, std::int64_t>>> relatives;   // (offset in tail, (token, target)) to fill in once the size is known
    std::uint8_t block_reg = 0;
};

void put_low(Bound& b, int value, std::uint8_t mask) {
    if (b.low >= 0) throw Fail{};
    b.low = value;
    b.low_mask = mask;
}

void put_le(Bytes& out, std::int64_t v, std::size_t n) {
    for (std::size_t k = 0; k < n; ++k) out.push_back(static_cast<std::uint8_t>(v >> (8 * k)));
}

void check_range(std::int64_t v, std::int64_t lo, std::int64_t hi, const Ctxt& cx) {
    if (!cx.lenient && (v < lo || v > hi)) throw Fail{};
}

void bind_token(const Variant& var, Tok t, std::string_view s, Bound& b, const Ctxt& cx) {
    const Row& r = *var.row;
    const unsigned z = var.z;
    const auto reg_of = [&](std::string_view name, unsigned size) {
        const std::optional<RegName> rn = register_by_name(name);
        if (!rn || rn->size != size) throw Fail{};
        return *rn;
    };
    switch (t) {
        case None: break;
        case M:
            if (r.ctx == Ctx::Reg) {
                b.reg = reg_of(s, z);
            } else {
                b.mem = parse_mem(s, cx);
            }
            break;
        case Mw: {
            const RegName rn = reg_of(s, 2 * z);
            if ((rn.code & 3u) != 0) throw Fail{};
            b.reg = RegName{rn.code, z};
            break;
        }
        case Rz:
        case R8:
        case R16:
        case R32: {
            const unsigned size = t == Rz ? z : t == R8 ? 1 : t == R16 ? 2 : 4;
            const RegName rn = reg_of(s, size);
            const std::optional<unsigned> n = short_number(rn.code, size);
            if (!n) throw Fail{};
            put_low(b, static_cast<int>(*n), 7);
            break;
        }
        case Rmul: {
            const RegName rn = reg_of(s, z == 1 ? 2 : 4);
            const std::optional<unsigned> n = short_number(rn.code, z == 1 ? 2 : 4);
            if (!n) throw Fail{};
            put_low(b, z == 1 ? static_cast<int>(*n * 2 + 1) : static_cast<int>(*n), 7);
            break;
        }
        case Tok::A:
            if (upper(s) != "A") throw Fail{};
            break;
        case Acc:
            if (upper(s) != (z == 1 ? "A" : "WA")) throw Fail{};
            break;
        case Ib: {
            const std::int64_t v = number_of(s, cx).value;
            check_range(v, -128, 255, cx);
            put_le(b.tail, v, 1);
            break;
        }
        case Iw: {
            const std::int64_t v = number_of(s, cx).value;
            check_range(v, -32768, 65535, cx);
            put_le(b.tail, v, 2);
            break;
        }
        case Il: {
            const Number n = number_of(s, cx);
            check_range(n.value, -2147483648LL, 4294967295LL, cx);
            put_le(b.tail, n.value, 4);
            break;
        }
        case Iz: {
            const Number n = number_of(s, cx);
            check_range(n.value, z == 1 ? -128 : z == 2 ? -32768 : -2147483648LL, z == 1 ? 255 : z == 2 ? 65535 : 4294967295LL, cx);
            put_le(b.tail, n.value, z);
            break;
        }
        case L3:
        case N3: {
            const Number n = number_of(s, cx);
            if (!n.decimal) throw Fail{};
            if (t == L3) {
                if (n.value < 0 || n.value > 7) throw Fail{};
                put_low(b, static_cast<int>(n.value), 7);
            } else {
                if (n.value < 1 || n.value > 8) throw Fail{};
                put_low(b, static_cast<int>(n.value & 7), 7);
            }
            break;
        }
        case Bit4: {
            const std::int64_t v = number_of(s, cx).value;
            check_range(v, 0, 15, cx);
            put_le(b.tail, v & 15, 1);
            break;
        }
        case Cnt4: {
            const std::int64_t v = number_of(s, cx).value;
            check_range(v, 1, 16, cx);
            put_le(b.tail, v & 15, 1);
            break;
        }
        case Cc: {
            const std::string name = upper(s);
            const auto it = std::find(kConditions.begin(), kConditions.end(), name);
            if (it == kConditions.end()) throw Fail{};
            put_low(b, static_cast<int>(it - kConditions.begin()), 15);
            break;
        }
        case Cr: {
            const std::string name = upper(s);
            unsigned found = 256;
            if (name.starts_with("CR_0X") && name.size() > 5 && name.size() <= 7) {
                const auto n = text::parse_int<unsigned>(std::string_view(name).substr(5), 16);
                if (n && *n < 256) found = *n;
            }
            for (unsigned c = 0; c < 256 && found == 256; ++c) {
                if (control_register_name(c) == name) {
                    found = c;
                    break;
                }
            }
            if (found == 256) throw Fail{};
            b.tail.push_back(static_cast<std::uint8_t>(found));
            break;
        }
        case Rel8:
        case Rel16: {
            const std::int64_t target = number_of(s, cx).value;
            b.relatives.push_back({b.tail.size(), {t, target}});
            put_le(b.tail, 0, t == Rel8 ? 1 : 2);
            break;
        }
        case D16: {
            const std::int64_t v = number_of(s, cx).value;
            check_range(v, -32768, 32767, cx);
            put_le(b.tail, v, 2);
            break;
        }
        case T16:
        case T24: {
            const Number n = number_of(s, cx);
            if (n.value < 0 || n.value > 0xFFFFFF) throw Fail{};
            if (t == T16 && (n.value > 0xFFFF || n.digits == 6)) throw Fail{};
            if (t == T24 && n.digits == 4) throw Fail{};
            put_le(b.tail, n.value, t == T16 ? 2 : 3);
            break;
        }
        case A8:
        case A16: {
            std::string_view e = text::trim(s);
            if (e.size() < 2 || e.front() != '(' || e.back() != ')') throw Fail{};
            const Number n = number_of(e.substr(1, e.size() - 2), cx);
            if (n.value < 0 || n.value > (t == A8 ? 0xFF : 0xFFFF)) throw Fail{};
            if (t == A8 && n.digits != 0 && n.digits != 2) throw Fail{};
            if (t == A16 && n.digits != 0 && n.digits != 4) throw Fail{};
            put_le(b.tail, n.value, t == A8 ? 1 : 2);
            break;
        }
        case BlkD:
        case BlkS: {
            const std::string_view e = text::trim(s);
            const char sign = (r.flags & kBlockDec) ? '-' : '+';
            if (e.size() < 4 || e.front() != '(' || e.back() != ')' || e[e.size() - 2] != sign) throw Fail{};
            const std::string_view name = e.substr(1, e.size() - 3);
            const RegName rn = reg_of(name, 4);
            if (!short_number(rn.code, 4)) throw Fail{};
            if (t == BlkS) {
                b.mem = MemOperand{};
                b.mem->m.kind = MemKind::Reg;
                b.mem->m.reg = rn.code;
            } else {
                b.block_reg = rn.code;
            }
            break;
        }
        case Sr:
            if (upper(s) != "SR") throw Fail{};
            break;
        case F:
            if (upper(s) != "F") throw Fail{};
            break;
        case Fp:
            if (upper(s) != "F'") throw Fail{};
            break;
    }
}

std::optional<Bytes> try_variant(const Variant& var, std::vector<std::string> ops, const Ctxt& cx) {
    const Row& r = *var.row;
    std::vector<Tok> toks;
    for (const Tok t : {r.a, r.b}) {
        if (t != None) toks.push_back(t);
    }
    if (!toks.empty() && toks[0] == Cc && (r.flags & kOptCc) && ops.size() + 1 == toks.size()) {
        ops.insert(ops.begin(), "T");
    }
    if (ops.size() != toks.size()) return std::nullopt;
    try {
        Bound b;
        // the Cc of a bare / optional row is bound like any other token; a memory operand is bound before its siblings so a block pair can check it
        for (std::size_t k = 0; k < toks.size(); ++k) bind_token(var, toks[k], ops[k], b, cx);
        if (r.a == BlkD) {
            const unsigned dest = (b.block_reg >> 2) & 7u, src = (b.mem->m.reg >> 2) & 7u;
            if (!((dest == 2 && src == 3) || (dest == 4 && src == 5))) throw Fail{};
        }
        // the operation byte and what comes before it
        int low = b.low < 0 ? 0 : b.low;
        std::uint8_t op = static_cast<std::uint8_t>(r.op | low);
        if (b.low >= 0 && (r.mask & b.low_mask) != 0) throw Fail{};
        Bytes out;
        switch (r.ctx) {
            case Ctx::Fixed: break;
            case Ctx::Reg: {
                if (!b.reg) throw Fail{};
                const std::optional<unsigned> n = short_number(b.reg->code, var.z);
                const std::uint8_t cls = var.z == 1 ? 0xC0 : var.z == 2 ? 0xD0 : 0xE0;
                if (n) {
                    out.push_back(static_cast<std::uint8_t>(cls + 8 + *n));
                } else {
                    out = {static_cast<std::uint8_t>(cls + 7), b.reg->code};
                }
                break;
            }
            case Ctx::Src:
            case Ctx::Dst: {
                if (r.flags & kBare) {
                    out.push_back(0xB0);
                    break;
                }
                if (!b.mem) throw Fail{};
                if (r.a == BlkS || r.b == BlkS) {
                    if (b.mem->m.kind != MemKind::Reg || b.mem->m.reg < 0xE0) throw Fail{};
                }
                out = mem_bytes(*b.mem, r.ctx, var.z, r.ctx == Ctx::Dst ? width_of(r) : var.z, cx);
                break;
            }
        }
        std::size_t pad = (r.flags & kPad) ? 1 : 0;
        const std::size_t total = out.size() + 1 + b.tail.size() + (pad ? 3 : 0);
        out.push_back(op);
        Bytes tail = b.tail;
        for (const auto& [offset, info] : b.relatives) {
            const std::int64_t disp = info.second - (static_cast<std::int64_t>(cx.pc) + static_cast<std::int64_t>(total));
            const Tok t = info.first;
            check_range(disp, t == Rel8 ? -128 : -32768, t == Rel8 ? 127 : 32767, cx);
            for (std::size_t k = 0; k < (t == Rel8 ? 1u : 2u); ++k) tail[offset + k] = static_cast<std::uint8_t>(disp >> (8 * k));
        }
        if (pad) {
            // ldx: 00 n 00 imm 00
            out.push_back(0);
            out.push_back(tail[0]);
            out.push_back(0);
            out.push_back(tail[1]);
            out.push_back(0);
        } else {
            out.insert(out.end(), tail.begin(), tail.end());
        }
        return out;
    } catch (const Fail&) {
        return std::nullopt;
    }
}

}  // namespace

Bytes assemble(std::string_view line, std::uint32_t pc, const NameLabels& labels, bool lenient) {
    const std::string_view t = text::trim(line);
    const std::size_t sp = t.find_first_of(" \t");
    std::string mnemonic = lower(sp == std::string_view::npos ? t : t.substr(0, sp));
    std::vector<std::string> ops = split_operands(sp == std::string_view::npos ? std::string_view() : text::trim(t.substr(sp + 1)));
    if (mnemonic == "di" && ops.empty()) {
        mnemonic = "ei";
        ops = {"7"};
    }
    const Ctxt cx{pc, labels, lenient};
    const auto it = variants().find(mnemonic);
    if (it == variants().end()) throw ParseError("", 0, 0, "'" + std::string(t) + "' is not an instruction");
    std::optional<Bytes> best;
    for (const Variant& v : it->second) {
        std::optional<Bytes> b = try_variant(v, ops, cx);
        if (b && (!best || b->size() < best->size())) best = std::move(b);
    }
    if (!best) throw ParseError("", 0, 0, "'" + std::string(t) + "' does not assemble (operands the instruction does not take, or a value out of range)");
    return *best;
}

}  // namespace cb::decompile::tlcs900
