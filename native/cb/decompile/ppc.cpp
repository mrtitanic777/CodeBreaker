// cb-unity: alone (file-local names that other files in a batch also use)
// cb/decompile/ppc.cpp -- the PowerPC 750 family's instructions: a table of every encoding, read both ways (ppc.hpp).

#include "cb/decompile/ppc.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <format>
#include <unordered_map>
#include <vector>

#include "cb/core/text.hpp"

namespace cb::decompile::ppc {

namespace {

// ---- operands ---------------------------------------------------------------------------------------------------

// What an operand is. The order of a row's operands is the order they are written in the source, not the order of the fields.
enum class Op : std::uint8_t {
    RT,        // rD / rS: bits 6-10 (a general register; also the BO, TO and CRB-D fields by their own names below)
    RA,        // rA: bits 11-15
    RB,        // rB: bits 16-20
    FT,        // frD / frS
    FA,        // frA
    FB,        // frB
    FC,        // frC: bits 21-25
    Simm,      // a signed 16-bit immediate
    SimmU,     // a 16-bit immediate written unsigned in hex (addis, lis)
    Uimm,      // an unsigned 16-bit immediate
    Mem,       // d(rA): a signed 16-bit displacement and the base register
    MemQ,      // d(rA) of the paired-single loads and stores: a signed 12-bit displacement
    CrfD,      // a condition register field in bits 6-8, written crN
    CrfDOpt,   // ... left out of the text when it is cr0 (cmpwi)
    CrfS,      // a condition register field in bits 11-13
    CrbD,      // a condition register bit number, bits 6-10 / 11-15 / 16-20
    CrbA,
    CrbB,
    To,
    Sh,
    Mb,
    Me,
    Nb,
    Sr,
    Spr,       // a special register number (the halves of the field are swapped in the word)
    Tbr,       // a time base register number (likewise)
    Crm,
    Fm,
    Imm4,
    Bo,
    Bi,
    Bd,        // a conditional branch's target (14-bit displacement)
    Li,        // a branch's target (24-bit displacement)
    W15,       // the paired-single W bit (bit 16) and I field (bits 17-19)
    I12,
    W10,       // ... and of the indexed forms (bit 21, bits 22-24)
    I7,
    RsRb,      // one register that stands in both rS and rB (mr, not)
    BiCr,      // the condition register field of a branch's BI (bits 11-13), left out of the text when cr0
    MemDS,     // d(rA) of the 64-bit loads and stores: a 14-bit displacement in words (its low two bits are the opcode's)
    Sh64,      // the 6-bit shift amount of the 64-bit rotates: bits 16-20 and bit 30
    Mb64,      // the 6-bit mask begin or end: bits 21-26, the low bit first
};

constexpr std::uint32_t mask_of(Op op) {
    switch (op) {
        case Op::RT: case Op::FT: case Op::CrbD: case Op::To: case Op::Bo: return 0x03E00000;
        case Op::RA: case Op::FA: case Op::CrbA: case Op::Bi: return 0x001F0000;
        case Op::RB: case Op::FB: case Op::CrbB: case Op::Sh: case Op::Nb: return 0x0000F800;
        case Op::FC: case Op::Mb: return 0x000007C0;
        case Op::Simm: case Op::SimmU: case Op::Uimm: return 0x0000FFFF;
        case Op::Mem: return 0x001FFFFF;
        case Op::MemQ: return 0x001F0FFF;
        case Op::CrfD: case Op::CrfDOpt: return 0x03800000;
        case Op::CrfS: case Op::BiCr: return 0x001C0000;
        case Op::Me: return 0x0000003E;
        case Op::Sr: return 0x000F0000;
        case Op::Spr: case Op::Tbr: return 0x001FF800;
        case Op::Crm: return 0x000FF000;
        case Op::Fm: return 0x01FE0000;
        case Op::Imm4: return 0x0000F000;
        case Op::Bd: return 0x0000FFFC;
        case Op::Li: return 0x03FFFFFC;
        case Op::W15: return 0x00008000;
        case Op::I12: return 0x00007000;
        case Op::W10: return 0x00000400;
        case Op::I7: return 0x00000380;
        case Op::RsRb: return 0x03E0F800;
        case Op::MemDS: return 0x001FFFFC;
        case Op::Sh64: return 0x0000F802;
        case Op::Mb64: return 0x000007E0;
    }
    return 0;
}

constexpr bool optional_op(Op op) { return op == Op::CrfDOpt || op == Op::BiCr; }

// ---- the table --------------------------------------------------------------------------------------------------

constexpr std::uint8_t kRc = 1;   // bit 31 is the record bit: the name gains a "."
constexpr std::uint8_t kOe = 2;   // bit 21 is the overflow enable: the name gains an "o"
constexpr std::uint8_t kLk = 4;   // bit 31 is the link bit: the name gains an "l"
constexpr std::uint8_t kAa = 8;   // bit 30 is the absolute address bit: the name gains an "a"

// How control leaves a row's instructions.
enum class Kind : std::uint8_t { Other, B, Bc, Bclr, Bcctr, Rfi };

struct Row {
    std::string name;
    std::uint32_t pattern = 0;   // the fixed bits' values
    std::uint32_t vary = 0;      // the bits that are operands or flags
    std::array<Op, 5> ops{};
    std::uint8_t count = 0;
    std::uint8_t flags = 0;
    Kind kind = Kind::Other;
    bool wide = false;           // a 64-bit instruction: decoded and assembled only when asked for
};

constexpr std::uint32_t flag_bits(std::uint8_t flags) {
    return ((flags & (kRc | kLk)) ? 1u : 0u) | ((flags & kOe) ? 0x400u : 0u) | ((flags & kAa) ? 2u : 0u);
}

class Table {
public:
    Table() {
        build();
        for (std::size_t i = 0; i < rows_.size(); ++i) by_opcode_[rows_[i].pattern >> 26].push_back(static_cast<std::uint16_t>(i));
        for (std::size_t i = 0; i < rows_.size(); ++i) {
            const Row& r = rows_[i];
            // every spelling of the name: with the overflow, record, link and absolute bits set or clear
            for (unsigned combo = 0; combo < 16; ++combo) {
                std::uint32_t bits = 0;
                std::string name = r.name;
                if (combo & kOe) {
                    if (!(r.flags & kOe)) continue;
                    name += "o";
                    bits |= 0x400;
                }
                if (combo & kLk) {
                    if (!(r.flags & kLk)) continue;
                    name += "l";
                    bits |= 1;
                }
                if (combo & kAa) {
                    if (!(r.flags & kAa)) continue;
                    name += "a";
                    bits |= 2;
                }
                if (combo & kRc) {
                    if (!(r.flags & kRc)) continue;
                    name += ".";
                    bits |= 1;
                }
                names_.emplace(std::move(name), Spelling{static_cast<std::uint16_t>(i), bits});
            }
        }
    }

    struct Spelling {
        std::uint16_t row;
        std::uint32_t bits;
    };

    const std::vector<Row>& rows() const { return rows_; }
    const std::vector<std::uint16_t>& with_opcode(unsigned opcode) const { return by_opcode_[opcode]; }
    const Spelling* find(const std::string& name) const {
        const auto it = names_.find(name);
        return it == names_.end() ? nullptr : &it->second;
    }

private:
    void add(std::string name, std::uint32_t pattern, std::initializer_list<Op> ops, std::uint8_t flags = 0, Kind kind = Kind::Other) {
        Row r;
        r.name = std::move(name);
        r.pattern = pattern;
        r.flags = flags;
        r.kind = kind;
        r.wide = wide_;
        r.vary = flag_bits(flags);
        for (const Op op : ops) {
            r.ops[r.count++] = op;
            r.vary |= mask_of(op);
        }
        r.pattern &= ~r.vary;
        rows_.push_back(std::move(r));
    }
    // opcode 31's rows: the extended opcode in bits 21-30
    void x31(std::string name, unsigned xo, std::initializer_list<Op> ops, std::uint8_t flags = 0) { add(std::move(name), (31u << 26) | (xo << 1), ops, flags); }
    void x19(std::string name, unsigned xo, std::initializer_list<Op> ops, std::uint8_t flags = 0, Kind kind = Kind::Other) { add(std::move(name), (19u << 26) | (xo << 1), ops, flags, kind); }
    // opcode 59 / 63's and the paired singles' A-form rows: the extended opcode in bits 26-30
    void a_form(unsigned opcode, std::string name, unsigned xo5, std::initializer_list<Op> ops) { add(std::move(name), (opcode << 26) | (xo5 << 1), ops, kRc); }
    void d_form(unsigned opcode, std::string name, std::initializer_list<Op> ops, std::uint8_t flags = 0) { add(std::move(name), opcode << 26, ops, flags); }

    void build() {
        using enum Op;
        // simplified mnemonics come first: the first row a word matches is how it is written
        add("nop", 0x60000000, {});
        d_form(14, "li", {RT, Simm});
        d_form(15, "lis", {RT, SimmU});
        x31("mr", 444, {RA, RsRb}, kRc);
        x31("not", 124, {RA, RsRb}, kRc);
        add("mflr", 0x7C0802A6, {RT});
        add("mtlr", 0x7C0803A6, {RT});
        add("mfctr", 0x7C0902A6, {RT});
        add("mtctr", 0x7C0903A6, {RT});
        add("mfxer", 0x7C0102A6, {RT});
        add("mtxer", 0x7C0103A6, {RT});
        add("cmpw", (31u << 26) | (0u << 1), {CrfDOpt, RA, RB});
        add("cmplw", (31u << 26) | (32u << 1), {CrfDOpt, RA, RB});
        add("cmpwi", 11u << 26, {CrfDOpt, RA, Simm});
        add("cmplwi", 10u << 26, {CrfDOpt, RA, Uimm});
        // the branches: unconditional through the link and count registers, then the conditional ones on a bit of a field
        add("blr", (19u << 26) | (20u << 21) | (16u << 1), {}, kLk, Kind::Bclr);
        add("bctr", (19u << 26) | (20u << 21) | (528u << 1), {}, kLk, Kind::Bcctr);
        struct Cond {
            const char* name;
            unsigned bo, bit;
        };
        const Cond conds[] = {{"blt", 12, 0}, {"bgt", 12, 1}, {"beq", 12, 2}, {"bso", 12, 3}, {"bge", 4, 0}, {"ble", 4, 1}, {"bne", 4, 2}, {"bns", 4, 3}};
        for (const Cond& c : conds) {
            add(c.name, (16u << 26) | (c.bo << 21) | (c.bit << 16), {BiCr, Bd}, kLk | kAa, Kind::Bc);
            add(std::string(c.name) + "lr", (19u << 26) | (c.bo << 21) | (c.bit << 16) | (16u << 1), {BiCr}, kLk, Kind::Bclr);
            add(std::string(c.name) + "ctr", (19u << 26) | (c.bo << 21) | (c.bit << 16) | (528u << 1), {BiCr}, kLk, Kind::Bcctr);
        }
        add("bdnz", (16u << 26) | (16u << 21), {Bd}, kLk | kAa, Kind::Bc);
        add("bdz", (16u << 26) | (18u << 21), {Bd}, kLk | kAa, Kind::Bc);
        // the general branches
        add("b", 18u << 26, {Li}, kLk | kAa, Kind::B);
        add("bc", 16u << 26, {Bo, Bi, Bd}, kLk | kAa, Kind::Bc);
        add("bclr", (19u << 26) | (16u << 1), {Bo, Bi}, kLk, Kind::Bclr);
        add("bcctr", (19u << 26) | (528u << 1), {Bo, Bi}, kLk, Kind::Bcctr);
        add("sc", 0x44000002, {});
        // opcode 19: the condition register
        x19("mcrf", 0, {CrfD, CrfS});
        x19("crnor", 33, {CrbD, CrbA, CrbB});
        add("rfi", (19u << 26) | (50u << 1), {}, 0, Kind::Rfi);
        x19("crandc", 129, {CrbD, CrbA, CrbB});
        x19("isync", 150, {});
        x19("crxor", 193, {CrbD, CrbA, CrbB});
        x19("crnand", 225, {CrbD, CrbA, CrbB});
        x19("crand", 257, {CrbD, CrbA, CrbB});
        x19("creqv", 289, {CrbD, CrbA, CrbB});
        x19("crorc", 417, {CrbD, CrbA, CrbB});
        x19("cror", 449, {CrbD, CrbA, CrbB});
        // the immediate forms
        d_form(3, "twi", {To, RA, Simm});
        d_form(7, "mulli", {RT, RA, Simm});
        d_form(8, "subfic", {RT, RA, Simm});
        d_form(12, "addic", {RT, RA, Simm});
        d_form(13, "addic.", {RT, RA, Simm});
        d_form(14, "addi", {RT, RA, Simm});
        d_form(15, "addis", {RT, RA, SimmU});
        d_form(20, "rlwimi", {RA, RT, Sh, Mb, Me}, kRc);
        d_form(21, "rlwinm", {RA, RT, Sh, Mb, Me}, kRc);
        d_form(23, "rlwnm", {RA, RT, RB, Mb, Me}, kRc);
        d_form(24, "ori", {RA, RT, Uimm});
        d_form(25, "oris", {RA, RT, Uimm});
        d_form(26, "xori", {RA, RT, Uimm});
        d_form(27, "xoris", {RA, RT, Uimm});
        d_form(28, "andi.", {RA, RT, Uimm});
        d_form(29, "andis.", {RA, RT, Uimm});
        // loads and stores
        const struct {
            unsigned opcode;
            const char* name;
            bool fp;
        } memory[] = {{32, "lwz", 0}, {33, "lwzu", 0}, {34, "lbz", 0}, {35, "lbzu", 0}, {36, "stw", 0}, {37, "stwu", 0}, {38, "stb", 0}, {39, "stbu", 0},
                      {40, "lhz", 0}, {41, "lhzu", 0}, {42, "lha", 0}, {43, "lhau", 0}, {44, "sth", 0}, {45, "sthu", 0}, {46, "lmw", 0}, {47, "stmw", 0},
                      {48, "lfs", 1}, {49, "lfsu", 1}, {50, "lfd", 1}, {51, "lfdu", 1}, {52, "stfs", 1}, {53, "stfsu", 1}, {54, "stfd", 1}, {55, "stfdu", 1}};
        for (const auto& m : memory) {
            if (m.fp) d_form(m.opcode, m.name, {FT, Mem});
            else d_form(m.opcode, m.name, {RT, Mem});
        }
        d_form(56, "psq_l", {FT, MemQ, W15, I12});
        d_form(57, "psq_lu", {FT, MemQ, W15, I12});
        d_form(60, "psq_st", {FT, MemQ, W15, I12});
        d_form(61, "psq_stu", {FT, MemQ, W15, I12});
        // opcode 31
        x31("tw", 4, {To, RA, RB});
        x31("subfc", 8, {RT, RA, RB}, kOe | kRc);
        x31("addc", 10, {RT, RA, RB}, kOe | kRc);
        x31("mulhwu", 11, {RT, RA, RB}, kRc);
        x31("mfcr", 19, {RT});
        x31("lwarx", 20, {RT, RA, RB});
        x31("lwzx", 23, {RT, RA, RB});
        x31("slw", 24, {RA, RT, RB}, kRc);
        x31("cntlzw", 26, {RA, RT}, kRc);
        x31("and", 28, {RA, RT, RB}, kRc);
        x31("subf", 40, {RT, RA, RB}, kOe | kRc);
        x31("dcbst", 54, {RA, RB});
        x31("lwzux", 55, {RT, RA, RB});
        x31("andc", 60, {RA, RT, RB}, kRc);
        x31("mulhw", 75, {RT, RA, RB}, kRc);
        x31("mfmsr", 83, {RT});
        x31("dcbf", 86, {RA, RB});
        x31("lbzx", 87, {RT, RA, RB});
        x31("neg", 104, {RT, RA}, kOe | kRc);
        x31("lbzux", 119, {RT, RA, RB});
        x31("nor", 124, {RA, RT, RB}, kRc);
        x31("subfe", 136, {RT, RA, RB}, kOe | kRc);
        x31("adde", 138, {RT, RA, RB}, kOe | kRc);
        x31("mtcrf", 144, {Crm, RT});
        x31("mtmsr", 146, {RT});
        add("stwcx.", (31u << 26) | (150u << 1) | 1, {RT, RA, RB});
        x31("stwx", 151, {RT, RA, RB});
        x31("stwux", 183, {RT, RA, RB});
        x31("subfze", 200, {RT, RA}, kOe | kRc);
        x31("addze", 202, {RT, RA}, kOe | kRc);
        x31("mtsr", 210, {Sr, RT});
        x31("stbx", 215, {RT, RA, RB});
        x31("subfme", 232, {RT, RA}, kOe | kRc);
        x31("addme", 234, {RT, RA}, kOe | kRc);
        x31("mullw", 235, {RT, RA, RB}, kOe | kRc);
        x31("mtsrin", 242, {RT, RB});
        x31("dcbtst", 246, {RA, RB});
        x31("stbux", 247, {RT, RA, RB});
        x31("add", 266, {RT, RA, RB}, kOe | kRc);
        x31("dcbt", 278, {RA, RB});
        x31("lhzx", 279, {RT, RA, RB});
        x31("eqv", 284, {RA, RT, RB}, kRc);
        x31("tlbie", 306, {RB});
        x31("eciwx", 310, {RT, RA, RB});
        x31("lhzux", 311, {RT, RA, RB});
        x31("xor", 316, {RA, RT, RB}, kRc);
        x31("mfspr", 339, {RT, Spr});
        x31("lhax", 343, {RT, RA, RB});
        x31("tlbia", 370, {});
        x31("mftb", 371, {RT, Tbr});
        x31("lhaux", 375, {RT, RA, RB});
        x31("sthx", 407, {RT, RA, RB});
        x31("orc", 412, {RA, RT, RB}, kRc);
        x31("ecowx", 438, {RT, RA, RB});
        x31("sthux", 439, {RT, RA, RB});
        x31("or", 444, {RA, RT, RB}, kRc);
        x31("divwu", 459, {RT, RA, RB}, kOe | kRc);
        x31("mtspr", 467, {Spr, RT});
        x31("dcbi", 470, {RA, RB});
        x31("nand", 476, {RA, RT, RB}, kRc);
        x31("divw", 491, {RT, RA, RB}, kOe | kRc);
        x31("mcrxr", 512, {CrfD});
        x31("lswx", 533, {RT, RA, RB});
        x31("lwbrx", 534, {RT, RA, RB});
        x31("lfsx", 535, {FT, RA, RB});
        x31("srw", 536, {RA, RT, RB}, kRc);
        x31("tlbsync", 566, {});
        x31("lfsux", 567, {FT, RA, RB});
        x31("mfsr", 595, {RT, Sr});
        x31("lswi", 597, {RT, RA, Nb});
        x31("sync", 598, {});
        x31("lfdx", 599, {FT, RA, RB});
        x31("lfdux", 631, {FT, RA, RB});
        x31("mfsrin", 659, {RT, RB});
        x31("stswx", 661, {RT, RA, RB});
        x31("stwbrx", 662, {RT, RA, RB});
        x31("stfsx", 663, {FT, RA, RB});
        x31("stfsux", 695, {FT, RA, RB});
        x31("stswi", 725, {RT, RA, Nb});
        x31("stfdx", 727, {FT, RA, RB});
        x31("stfdux", 759, {FT, RA, RB});
        x31("lhbrx", 790, {RT, RA, RB});
        x31("sraw", 792, {RA, RT, RB}, kRc);
        x31("srawi", 824, {RA, RT, Sh}, kRc);
        x31("eieio", 854, {});
        x31("sthbrx", 918, {RT, RA, RB});
        x31("extsh", 922, {RA, RT}, kRc);
        x31("extsb", 954, {RA, RT}, kRc);
        x31("icbi", 982, {RA, RB});
        x31("stfiwx", 983, {FT, RA, RB});
        x31("dcbz", 1014, {RA, RB});
        // opcode 59: single-precision floating point
        a_form(59, "fdivs", 18, {FT, FA, FB});
        a_form(59, "fsubs", 20, {FT, FA, FB});
        a_form(59, "fadds", 21, {FT, FA, FB});
        a_form(59, "fres", 24, {FT, FB});
        a_form(59, "fmuls", 25, {FT, FA, FC});
        a_form(59, "fmsubs", 28, {FT, FA, FC, FB});
        a_form(59, "fmadds", 29, {FT, FA, FC, FB});
        a_form(59, "fnmsubs", 30, {FT, FA, FC, FB});
        a_form(59, "fnmadds", 31, {FT, FA, FC, FB});
        // opcode 63: double-precision floating point and the status register
        add("fcmpu", (63u << 26) | (0u << 1), {CrfD, FA, FB});
        add("frsp", (63u << 26) | (12u << 1), {FT, FB}, kRc);
        add("fctiw", (63u << 26) | (14u << 1), {FT, FB}, kRc);
        add("fctiwz", (63u << 26) | (15u << 1), {FT, FB}, kRc);
        a_form(63, "fdiv", 18, {FT, FA, FB});
        a_form(63, "fsub", 20, {FT, FA, FB});
        a_form(63, "fadd", 21, {FT, FA, FB});
        a_form(63, "fsel", 23, {FT, FA, FC, FB});
        a_form(63, "fmul", 25, {FT, FA, FC});
        a_form(63, "frsqrte", 26, {FT, FB});
        a_form(63, "fmsub", 28, {FT, FA, FC, FB});
        a_form(63, "fmadd", 29, {FT, FA, FC, FB});
        a_form(63, "fnmsub", 30, {FT, FA, FC, FB});
        a_form(63, "fnmadd", 31, {FT, FA, FC, FB});
        add("fcmpo", (63u << 26) | (32u << 1), {CrfD, FA, FB});
        add("mtfsb1", (63u << 26) | (38u << 1), {CrbD}, kRc);
        add("fneg", (63u << 26) | (40u << 1), {FT, FB}, kRc);
        add("mcrfs", (63u << 26) | (64u << 1), {CrfD, CrfS});
        add("mtfsb0", (63u << 26) | (70u << 1), {CrbD}, kRc);
        add("fmr", (63u << 26) | (72u << 1), {FT, FB}, kRc);
        add("mtfsfi", (63u << 26) | (134u << 1), {CrfD, Imm4}, kRc);
        add("fnabs", (63u << 26) | (136u << 1), {FT, FB}, kRc);
        add("fabs", (63u << 26) | (264u << 1), {FT, FB}, kRc);
        add("mffs", (63u << 26) | (583u << 1), {FT}, kRc);
        add("mtfsf", (63u << 26) | (711u << 1), {Fm, FB}, kRc);
        // opcode 4: Gekko's paired singles
        a_form(4, "ps_div", 18, {FT, FA, FB});
        a_form(4, "ps_sub", 20, {FT, FA, FB});
        a_form(4, "ps_add", 21, {FT, FA, FB});
        a_form(4, "ps_sel", 23, {FT, FA, FC, FB});
        a_form(4, "ps_res", 24, {FT, FB});
        a_form(4, "ps_mul", 25, {FT, FA, FC});
        a_form(4, "ps_rsqrte", 26, {FT, FB});
        a_form(4, "ps_msub", 28, {FT, FA, FC, FB});
        a_form(4, "ps_madd", 29, {FT, FA, FC, FB});
        a_form(4, "ps_nmsub", 30, {FT, FA, FC, FB});
        a_form(4, "ps_nmadd", 31, {FT, FA, FC, FB});
        a_form(4, "ps_sum0", 10, {FT, FA, FC, FB});
        a_form(4, "ps_sum1", 11, {FT, FA, FC, FB});
        a_form(4, "ps_muls0", 12, {FT, FA, FC});
        a_form(4, "ps_muls1", 13, {FT, FA, FC});
        a_form(4, "ps_madds0", 14, {FT, FA, FC, FB});
        a_form(4, "ps_madds1", 15, {FT, FA, FC, FB});
        add("ps_cmpu0", (4u << 26) | (0u << 1), {CrfD, FA, FB});
        add("ps_cmpo0", (4u << 26) | (32u << 1), {CrfD, FA, FB});
        add("ps_cmpu1", (4u << 26) | (64u << 1), {CrfD, FA, FB});
        add("ps_cmpo1", (4u << 26) | (96u << 1), {CrfD, FA, FB});
        add("ps_neg", (4u << 26) | (40u << 1), {FT, FB}, kRc);
        add("ps_mr", (4u << 26) | (72u << 1), {FT, FB}, kRc);
        add("ps_nabs", (4u << 26) | (136u << 1), {FT, FB}, kRc);
        add("ps_abs", (4u << 26) | (264u << 1), {FT, FB}, kRc);
        add("ps_merge00", (4u << 26) | (528u << 1), {FT, FA, FB}, kRc);
        add("ps_merge01", (4u << 26) | (560u << 1), {FT, FA, FB}, kRc);
        add("ps_merge10", (4u << 26) | (592u << 1), {FT, FA, FB}, kRc);
        add("ps_merge11", (4u << 26) | (624u << 1), {FT, FA, FB}, kRc);
        add("psq_lx", (4u << 26) | (6u << 1), {FT, RA, RB, W10, I7});
        add("psq_stx", (4u << 26) | (7u << 1), {FT, RA, RB, W10, I7});
        add("psq_lux", (4u << 26) | (38u << 1), {FT, RA, RB, W10, I7});
        add("psq_stux", (4u << 26) | (39u << 1), {FT, RA, RB, W10, I7});
        add("dcbz_l", (4u << 26) | (1014u << 1), {RA, RB});
        // the 64-bit instructions of the Cell's PPU (and the Xbox 360's Xenon): decoded and assembled only when asked for (`wide`)
        wide_ = true;
        add("ld", (58u << 26) | 0, {RT, MemDS});
        add("ldu", (58u << 26) | 1, {RT, MemDS});
        add("lwa", (58u << 26) | 2, {RT, MemDS});
        add("std", (62u << 26) | 0, {RT, MemDS});
        add("stdu", (62u << 26) | 1, {RT, MemDS});
        add("rldicl", (30u << 26) | (0u << 2), {RA, RT, Sh64, Mb64}, kRc);
        add("rldicr", (30u << 26) | (1u << 2), {RA, RT, Sh64, Mb64}, kRc);
        add("rldic", (30u << 26) | (2u << 2), {RA, RT, Sh64, Mb64}, kRc);
        add("rldimi", (30u << 26) | (3u << 2), {RA, RT, Sh64, Mb64}, kRc);
        add("rldcl", (30u << 26) | (8u << 1), {RA, RT, RB, Mb64}, kRc);
        add("rldcr", (30u << 26) | (9u << 1), {RA, RT, RB, Mb64}, kRc);
        add("cmpdi", (11u << 26) | (1u << 21), {CrfDOpt, RA, Simm});
        add("cmpldi", (10u << 26) | (1u << 21), {CrfDOpt, RA, Uimm});
        add("cmpd", (31u << 26) | (1u << 21) | (0u << 1), {CrfDOpt, RA, RB});
        add("cmpld", (31u << 26) | (1u << 21) | (32u << 1), {CrfDOpt, RA, RB});
        d_form(2, "tdi", {To, RA, Simm});
        x31("td", 68, {To, RA, RB});
        x31("ldx", 21, {RT, RA, RB});
        x31("ldux", 53, {RT, RA, RB});
        x31("lwax", 341, {RT, RA, RB});
        x31("lwaux", 373, {RT, RA, RB});
        x31("stdx", 149, {RT, RA, RB});
        x31("stdux", 181, {RT, RA, RB});
        x31("ldarx", 84, {RT, RA, RB});
        add("stdcx.", (31u << 26) | (214u << 1) | 1, {RT, RA, RB});
        x31("sld", 27, {RA, RT, RB}, kRc);
        x31("srd", 539, {RA, RT, RB}, kRc);
        x31("srad", 794, {RA, RT, RB}, kRc);
        add("sradi", (31u << 26) | (413u << 2), {RA, RT, Sh64}, kRc);
        x31("cntlzd", 58, {RA, RT}, kRc);
        x31("extsw", 986, {RA, RT}, kRc);
        x31("mulld", 233, {RT, RA, RB}, kOe | kRc);
        x31("mulhd", 73, {RT, RA, RB}, kRc);
        x31("mulhdu", 9, {RT, RA, RB}, kRc);
        x31("divd", 489, {RT, RA, RB}, kOe | kRc);
        x31("divdu", 457, {RT, RA, RB}, kOe | kRc);
        add("fcfid", (63u << 26) | (846u << 1), {FT, FB}, kRc);
        add("fctid", (63u << 26) | (814u << 1), {FT, FB}, kRc);
        add("fctidz", (63u << 26) | (815u << 1), {FT, FB}, kRc);
        wide_ = false;
    }

    bool wide_ = false;
    std::vector<Row> rows_;
    std::array<std::vector<std::uint16_t>, 64> by_opcode_;
    std::unordered_map<std::string, Spelling> names_;
};

const Table& table() {
    static const Table t;
    return t;
}

[[noreturn]] void refuse(std::string_view line, const std::string& why) { throw ParseError("", 0, 0, std::string(line) + " (" + why + ")"); }

constexpr std::uint32_t field(std::uint32_t word, std::uint32_t mask) {
    unsigned shift = 0;
    while (!((mask >> shift) & 1)) ++shift;
    return (word & mask) >> shift;
}
constexpr std::uint32_t place(std::uint32_t value, std::uint32_t mask) {
    unsigned shift = 0;
    while (!((mask >> shift) & 1)) ++shift;
    return (value << shift) & mask;
}
constexpr std::int32_t sign_extend(std::uint32_t v, unsigned bits) { return static_cast<std::int32_t>(v << (32 - bits)) >> (32 - bits); }

// The special register field holds its two five-bit halves swapped.
constexpr std::uint32_t spr_of(std::uint32_t word) { return ((word >> 16) & 31) | (((word >> 11) & 31) << 5); }
constexpr std::uint32_t spr_field(std::uint32_t n) { return ((n & 31) << 16) | (((n >> 5) & 31) << 11); }

// Is every operand of the word one the row can say (a register that stands twice is the same in both places)?
bool matches(const Row& row, std::uint32_t word) {
    if ((word & ~row.vary) != row.pattern) return false;
    for (unsigned i = 0; i < row.count; ++i) {
        if (row.ops[i] == Op::RsRb && ((word >> 21) & 31) != ((word >> 11) & 31)) return false;
    }
    return true;
}

std::string hex(std::uint32_t v) { return v < 10 ? std::to_string(v) : std::format("0x{:X}", v); }

std::string target_text(std::uint32_t target, const AddressLabels& labels) {
    if (const auto it = labels.find(target); it != labels.end() && !it->second.empty()) return it->second;
    return std::format("0x{:08X}", target);
}

// One operand as the source writes it ("" for an optional one that is left out).
std::string operand_text(const Insn& insn, const Row& row, unsigned index, const AddressLabels& labels, std::string_view immediate) {
    const std::uint32_t w = insn.word;
    const Op op = row.ops[index];
    switch (op) {
        case Op::RT: case Op::RA: case Op::RB: return std::format("r{}", field(w, mask_of(op)));
        case Op::RsRb: return std::format("r{}", field(w, 0x03E00000));
        case Op::FT: case Op::FA: case Op::FB: case Op::FC: return std::format("f{}", field(w, mask_of(op)));
        case Op::Simm: return immediate.empty() ? std::to_string(sign_extend(w & 0xFFFF, 16)) : std::string(immediate);
        case Op::SimmU: case Op::Uimm: return immediate.empty() ? hex(w & 0xFFFF) : std::string(immediate);
        case Op::Mem: {
            const std::string d = immediate.empty() ? std::to_string(sign_extend(w & 0xFFFF, 16)) : std::string(immediate);
            return std::format("{}(r{})", d, field(w, 0x001F0000));
        }
        case Op::MemQ: return std::format("{}(r{})", sign_extend(w & 0xFFF, 12), field(w, 0x001F0000));
        case Op::MemDS: return std::format("{}(r{})", sign_extend(w & 0xFFFC, 16), field(w, 0x001F0000));
        case Op::Sh64: return std::to_string(((w >> 11) & 31) | (((w >> 1) & 1) << 5));
        case Op::Mb64: return std::to_string(((w >> 6) & 31) | (((w >> 5) & 1) << 5));
        case Op::CrfD: return std::format("cr{}", field(w, mask_of(op)));
        case Op::CrfDOpt: return field(w, mask_of(op)) == 0 ? "" : std::format("cr{}", field(w, mask_of(op)));
        case Op::CrfS: return std::format("cr{}", field(w, mask_of(op)));
        case Op::BiCr: return field(w, mask_of(op)) == 0 ? "" : std::format("cr{}", field(w, mask_of(op)));
        case Op::CrbD: case Op::CrbA: case Op::CrbB: case Op::To: case Op::Sh: case Op::Mb: case Op::Me: case Op::Nb: case Op::Sr: case Op::Imm4:
        case Op::Bo: case Op::Bi: case Op::W15: case Op::I12: case Op::W10: case Op::I7:
            return std::to_string(field(w, mask_of(op)));
        case Op::Spr: case Op::Tbr: return std::to_string(spr_of(w));
        case Op::Crm: case Op::Fm: return hex(field(w, mask_of(op)));
        case Op::Bd: case Op::Li: return insn.target ? target_text(*insn.target, labels) : "0";
    }
    return "";
}

}  // namespace

// ---- decoding ---------------------------------------------------------------------------------------------------

std::optional<Insn> decode_word(std::uint32_t word, std::uint32_t pc, bool wide) {
    const Table& t = table();
    for (const std::uint16_t i : t.with_opcode(word >> 26)) {
        const Row& row = t.rows()[i];
        if (row.wide && !wide) continue;
        if (!matches(row, word)) continue;
        Insn insn;
        insn.word = word;
        insn.pc = pc;
        insn.row = i;
        insn.mnemonic = row.name;
        if (row.flags & kOe && (word & 0x400)) insn.mnemonic += "o";
        if (row.flags & kLk && (word & 1)) insn.mnemonic += "l";
        if (row.flags & kAa && (word & 2)) insn.mnemonic += "a";
        if (row.flags & kRc && (word & 1)) insn.mnemonic += ".";
        const bool link = (row.flags & kLk) && (word & 1);
        const unsigned bo = (word >> 21) & 31;
        const bool always = (bo & 0x14) == 0x14;
        switch (row.kind) {
            case Kind::B: {
                const std::int32_t disp = sign_extend(word & 0x03FFFFFC, 26);
                insn.target = (word & 2) ? static_cast<std::uint32_t>(disp) : pc + static_cast<std::uint32_t>(disp);
                insn.flow = link ? Flow::Call : Flow::Jump;
                break;
            }
            case Kind::Bc: {
                const std::int32_t disp = sign_extend(word & 0xFFFC, 16);
                insn.target = (word & 2) ? static_cast<std::uint32_t>(disp) : pc + static_cast<std::uint32_t>(disp);
                insn.flow = link ? Flow::Call : bo == 20 ? Flow::Jump : Flow::Branch;
                break;
            }
            case Kind::Bclr: insn.flow = always ? (link ? Flow::Call : Flow::Return) : Flow::Branch; break;
            case Kind::Bcctr: insn.flow = always ? (link ? Flow::Call : Flow::Indirect) : Flow::Branch; break;
            case Kind::Rfi: insn.flow = Flow::Return; break;
            case Kind::Other: break;
        }
        return insn;
    }
    return std::nullopt;
}

std::optional<Insn> decode(ByteView code, std::size_t at, std::uint32_t pc, bool wide) {
    if (at + 4 > code.size()) return std::nullopt;
    const std::uint32_t word = static_cast<std::uint32_t>(code[at]) << 24 | static_cast<std::uint32_t>(code[at + 1]) << 16 | static_cast<std::uint32_t>(code[at + 2]) << 8 | code[at + 3];
    return decode_word(word, pc, wide);
}

bool has_immediate(const Insn& insn) {
    const Row& row = table().rows()[insn.row];
    for (unsigned i = 0; i < row.count; ++i) {
        if (row.ops[i] == Op::Simm || row.ops[i] == Op::SimmU || row.ops[i] == Op::Uimm || row.ops[i] == Op::Mem) return true;
    }
    return false;
}

std::string text(const Insn& insn, const AddressLabels& labels, std::string_view immediate) {
    const Row& row = table().rows()[insn.row];
    std::string out = insn.mnemonic;
    bool first = true;
    for (unsigned i = 0; i < row.count; ++i) {
        const std::string operand = operand_text(insn, row, i, labels, immediate);
        if (operand.empty()) continue;
        out += first ? " " : ",";
        out += operand;
        first = false;
    }
    return out;
}

std::string text(const Insn& insn, const AddressLabels& labels) { return text(insn, labels, {}); }

// ---- expressions ------------------------------------------------------------------------------------------------

namespace {

[[noreturn]] void fail(const std::string& why) { throw ParseError("", 0, 0, why); }

class Expr {
public:
    Expr(std::string_view s, const NameLabels& labels, std::uint32_t pc) : s_(s), labels_(labels), pc_(pc) {}
    std::int64_t run() {
        const std::int64_t v = bit_or();
        skip();
        if (i_ != s_.size()) fail("unexpected '" + std::string(1, s_[i_]) + "' in '" + std::string(s_) + "'");
        return v;
    }

private:
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
    // a single-character operator that is not the start of a longer one (`<<`, `>>`)
    bool eat_op(char c) {
        skip();
        if (i_ < s_.size() && s_[i_] == c && !((c == '<' || c == '>') && i_ + 1 < s_.size() && s_[i_ + 1] == c)) {
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
        return relocated(primary());
    }
    // `x@ha`, `x@l`, `x@h`: the parts of a 32-bit address a lis / addi pair carries
    std::int64_t relocated(std::int64_t v) {
        if (eat("@ha")) return ((v + 0x8000) >> 16) & 0xFFFF;
        if (eat("@l")) return v & 0xFFFF;
        if (eat("@h")) return (v >> 16) & 0xFFFF;
        return v;
    }
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
        if (c == '.' && (i_ + 1 >= s_.size() || !(std::isalnum(static_cast<unsigned char>(s_[i_ + 1])) || s_[i_ + 1] == '_'))) {
            ++i_;
            return pc_;   // `.` is here
        }
        if (std::isalpha(static_cast<unsigned char>(c)) || c == '_' || c == '.' || c == '$') {
            const std::size_t from = i_;
            while (i_ < s_.size() && (std::isalnum(static_cast<unsigned char>(s_[i_])) || s_[i_] == '_' || s_[i_] == '.' || s_[i_] == '$')) ++i_;
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

// ---- assembling -------------------------------------------------------------------------------------------------

namespace {

std::uint32_t range(std::string_view line, std::int64_t v, std::int64_t lo, std::int64_t hi, std::string_view what) {
    if (v < lo || v > hi) refuse(line, std::format("{} {} does not fit", what, v));
    return static_cast<std::uint32_t>(v);
}

// r5, 5 or %r5 (prefix 'r'); f5, 5 (prefix 'f'); cr5, 5 (prefix "cr").
std::uint32_t register_number(std::string_view line, std::string_view token, std::string_view prefix) {
    std::string t = text::lower(text::trim(token));
    std::string_view digits = t;
    if (digits.starts_with('%')) digits.remove_prefix(1);
    if (digits.starts_with(prefix)) digits.remove_prefix(prefix.size());
    const auto n = digits.empty() ? std::nullopt : text::parse_int<std::uint32_t>(digits);
    const std::uint32_t limit = prefix == "cr" ? 7 : 31;
    if (!n || *n > limit) refuse(line, "'" + std::string(token) + "' is not a register");
    return *n;
}

// `d(rA)`, `(rA)`: the displacement expression and the base register.
bool split_memory(std::string_view operand, std::string_view& displacement, std::string_view& base) {
    operand = text::trim(operand);
    if (operand.empty() || operand.back() != ')') return false;
    int depth = 0;
    for (std::size_t i = operand.size(); i-- > 0;) {
        if (operand[i] == ')') ++depth;
        if (operand[i] == '(' && --depth == 0) {
            base = text::trim(operand.substr(i + 1, operand.size() - i - 2));
            displacement = text::trim(operand.substr(0, i));
            return true;
        }
    }
    return false;
}

}  // namespace

Bytes assemble(std::string_view line, std::uint32_t pc, const NameLabels& labels, bool wide) {
    line = text::trim(line);
    std::size_t split = 0;
    while (split < line.size() && !text::is_space(line[split])) ++split;
    const std::string name = text::lower(line.substr(0, split));
    const Table& t = table();
    const Table::Spelling* spelling = t.find(name);
    if (!spelling || (t.rows()[spelling->row].wide && !wide)) refuse(line, "unknown instruction '" + name + "'");
    const Row& row = t.rows()[spelling->row];
    const std::vector<std::string> tokens = z80::split_operands(line.substr(split));

    unsigned required = 0;
    for (unsigned i = 0; i < row.count; ++i) required += !optional_op(row.ops[i]);
    const bool all = tokens.size() == row.count;
    if (!all && tokens.size() != required) refuse(line, std::format("{} operands where {} are written", tokens.size(), required));

    std::uint32_t word = row.pattern | spelling->bits;
    unsigned next = 0;
    const auto number = [&](const std::string& token) { return value(token, labels, pc); };
    for (unsigned i = 0; i < row.count; ++i) {
        const Op op = row.ops[i];
        if (optional_op(op) && !all) continue;   // (left out: the field is zero)
        const std::string& token = tokens[next++];
        switch (op) {
            case Op::RT: case Op::RA: case Op::RB: word |= place(register_number(line, token, "r"), mask_of(op)); break;
            case Op::RsRb: {
                const std::uint32_t r = register_number(line, token, "r");
                word |= place(r, 0x03E00000) | place(r, 0x0000F800);
                break;
            }
            case Op::FT: case Op::FA: case Op::FB: case Op::FC: word |= place(register_number(line, token, "f"), mask_of(op)); break;
            case Op::Simm: case Op::SimmU: case Op::Uimm: word |= range(line, number(token), -32768, 65535, "the immediate") & 0xFFFF; break;
            case Op::Mem: case Op::MemQ: {
                std::string_view displacement, base;
                if (!split_memory(token, displacement, base)) refuse(line, "'" + token + "' is not offset(register)");
                const std::int64_t d = displacement.empty() ? 0 : number(std::string(displacement));
                if (op == Op::Mem) word |= (range(line, d, -32768, 65535, "the displacement") & 0xFFFF);
                else word |= range(line, d, -2048, 4095, "the displacement") & 0xFFF;
                word |= place(register_number(line, base, "r"), 0x001F0000);
                break;
            }
            case Op::MemDS: {
                std::string_view displacement, base;
                if (!split_memory(token, displacement, base)) refuse(line, "'" + token + "' is not offset(register)");
                const std::int64_t d = displacement.empty() ? 0 : number(std::string(displacement));
                if (d % 4 != 0) refuse(line, "the displacement is not a multiple of four");
                word |= (range(line, d, -32768, 32764, "the displacement") & 0xFFFC);
                word |= place(register_number(line, base, "r"), 0x001F0000);
                break;
            }
            case Op::Sh64: {
                const std::uint32_t v = range(line, number(token), 0, 63, "the shift");
                word |= (v & 31) << 11 | (v >> 5) << 1;
                break;
            }
            case Op::Mb64: {
                const std::uint32_t v = range(line, number(token), 0, 63, "the mask bit");
                word |= (v & 31) << 6 | (v >> 5) << 5;
                break;
            }
            case Op::CrfD: case Op::CrfDOpt: case Op::CrfS: case Op::BiCr: word |= place(register_number(line, token, "cr"), mask_of(op)); break;
            case Op::CrbD: case Op::CrbA: case Op::CrbB: case Op::To: case Op::Sh: case Op::Mb: case Op::Me: case Op::Nb: case Op::Bo: case Op::Bi:
                word |= place(range(line, number(token), 0, 31, "the field"), mask_of(op));
                break;
            case Op::Sr: word |= place(range(line, number(token), 0, 15, "the segment register"), mask_of(op)); break;
            case Op::Imm4: word |= place(range(line, number(token), 0, 15, "the immediate"), mask_of(op)); break;
            case Op::Crm: case Op::Fm: word |= place(range(line, number(token), 0, 255, "the mask"), mask_of(op)); break;
            case Op::Spr: case Op::Tbr: word |= spr_field(range(line, number(token), 0, 1023, "the register number")); break;
            case Op::W15: case Op::W10: word |= place(range(line, number(token), 0, 1, "W"), mask_of(op)); break;
            case Op::I12: case Op::I7: word |= place(range(line, number(token), 0, 7, "I"), mask_of(op)); break;
            case Op::Bd: case Op::Li: {
                const std::int64_t target = number(token);
                const bool absolute = (word & 2) != 0;
                // (addresses are 32 bits and wrap: a branch from 0xFFFFFFF0 to 0x10 goes forward)
                const std::int64_t distance = static_cast<std::int32_t>(static_cast<std::uint32_t>(absolute ? target : target - static_cast<std::int64_t>(pc)));
                if (distance % 4 != 0) refuse(line, "a branch target is not a word");
                const unsigned bits = op == Op::Bd ? 16 : 26;
                const std::int64_t reach = std::int64_t{1} << (bits - 1);
                if (distance < -reach || distance >= reach) refuse(line, "the branch is out of reach");
                word |= static_cast<std::uint32_t>(distance) & mask_of(op);
                break;
            }
        }
    }
    return {static_cast<std::uint8_t>(word >> 24), static_cast<std::uint8_t>(word >> 16), static_cast<std::uint8_t>(word >> 8), static_cast<std::uint8_t>(word)};
}

std::size_t row_count() { return table().rows().size(); }
std::string_view row_name(std::size_t row) { return table().rows().at(row).name; }
std::uint32_t row_pattern(std::size_t row) { return table().rows().at(row).pattern; }
std::uint32_t row_variable_bits(std::size_t row) { return table().rows().at(row).vary; }

}  // namespace cb::decompile::ppc
