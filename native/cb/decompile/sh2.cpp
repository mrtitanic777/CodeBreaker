// cb/decompile/sh2.cpp -- the SH-2's instructions: a table of every encoding, read both ways (sh2.hpp).

#include "cb/decompile/sh2.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <format>
#include <vector>

#include "cb/core/text.hpp"
#include "cb/decompile/mips.hpp"

namespace cb::decompile::sh2 {

namespace {

// ---- the table --------------------------------------------------------------------------------------------------

using O = Operand;

struct Def {
    std::string_view name;
    std::uint16_t mask;    // which bits of the word are fixed ...
    std::uint16_t bits;    // ... and what they are (everything else is an operand)
    O a, b;
    std::uint8_t size;     // 1, 2 or 4: the unit of a displacement; 0 where there is none
    Flow flow;
    bool delay;
};

constexpr std::uint16_t kRm = 0xF00F;   // rn and rm: nnnn mmmm in the middle, the last nibble fixed
constexpr std::uint16_t kRn = 0xF0FF;   // one register in bits 8-11
constexpr std::uint16_t kAll = 0xFFFF;
constexpr std::uint16_t kTop = 0xF000;  // the first nibble fixed: a field of 8 or 12 bits, or a displacement
constexpr std::uint16_t kByte = 0xFF00;

constexpr auto kDefs = std::to_array<Def>({
    // 0xxx
    {"stc", kRn, 0x0002, O::Sr, O::RegN, 0, Flow::None, false},
    {"stc", kRn, 0x0012, O::Gbr, O::RegN, 0, Flow::None, false},
    {"stc", kRn, 0x0022, O::Vbr, O::RegN, 0, Flow::None, false},
    {"bsrf", kRn, 0x0003, O::RegN, O::None, 0, Flow::Call, true},
    {"braf", kRn, 0x0023, O::RegN, O::None, 0, Flow::Indirect, true},
    {"mov.b", kRm, 0x0004, O::RegM, O::R0IndexN, 1, Flow::None, false},
    {"mov.w", kRm, 0x0005, O::RegM, O::R0IndexN, 2, Flow::None, false},
    {"mov.l", kRm, 0x0006, O::RegM, O::R0IndexN, 4, Flow::None, false},
    {"mul.l", kRm, 0x0007, O::RegM, O::RegN, 0, Flow::None, false},
    {"clrt", kAll, 0x0008, O::None, O::None, 0, Flow::None, false},
    {"sett", kAll, 0x0018, O::None, O::None, 0, Flow::None, false},
    {"clrmac", kAll, 0x0028, O::None, O::None, 0, Flow::None, false},
    {"nop", kAll, 0x0009, O::None, O::None, 0, Flow::None, false},
    {"div0u", kAll, 0x0019, O::None, O::None, 0, Flow::None, false},
    {"movt", kRn, 0x0029, O::RegN, O::None, 0, Flow::None, false},
    {"sts", kRn, 0x000A, O::Mach, O::RegN, 0, Flow::None, false},
    {"sts", kRn, 0x001A, O::Macl, O::RegN, 0, Flow::None, false},
    {"sts", kRn, 0x002A, O::Pr, O::RegN, 0, Flow::None, false},
    {"rts", kAll, 0x000B, O::None, O::None, 0, Flow::Return, true},
    {"sleep", kAll, 0x001B, O::None, O::None, 0, Flow::None, false},
    {"rte", kAll, 0x002B, O::None, O::None, 0, Flow::Return, true},
    {"mov.b", kRm, 0x000C, O::R0IndexM, O::RegN, 1, Flow::None, false},
    {"mov.w", kRm, 0x000D, O::R0IndexM, O::RegN, 2, Flow::None, false},
    {"mov.l", kRm, 0x000E, O::R0IndexM, O::RegN, 4, Flow::None, false},
    {"mac.l", kRm, 0x000F, O::PostM, O::PostN, 4, Flow::None, false},
    // 1nmd
    {"mov.l", kTop, 0x1000, O::RegM, O::DispN, 4, Flow::None, false},
    // 2nmx
    {"mov.b", kRm, 0x2000, O::RegM, O::IndN, 1, Flow::None, false},
    {"mov.w", kRm, 0x2001, O::RegM, O::IndN, 2, Flow::None, false},
    {"mov.l", kRm, 0x2002, O::RegM, O::IndN, 4, Flow::None, false},
    {"mov.b", kRm, 0x2004, O::RegM, O::PreN, 1, Flow::None, false},
    {"mov.w", kRm, 0x2005, O::RegM, O::PreN, 2, Flow::None, false},
    {"mov.l", kRm, 0x2006, O::RegM, O::PreN, 4, Flow::None, false},
    {"div0s", kRm, 0x2007, O::RegM, O::RegN, 0, Flow::None, false},
    {"tst", kRm, 0x2008, O::RegM, O::RegN, 0, Flow::None, false},
    {"and", kRm, 0x2009, O::RegM, O::RegN, 0, Flow::None, false},
    {"xor", kRm, 0x200A, O::RegM, O::RegN, 0, Flow::None, false},
    {"or", kRm, 0x200B, O::RegM, O::RegN, 0, Flow::None, false},
    {"cmp/str", kRm, 0x200C, O::RegM, O::RegN, 0, Flow::None, false},
    {"xtrct", kRm, 0x200D, O::RegM, O::RegN, 0, Flow::None, false},
    {"mulu.w", kRm, 0x200E, O::RegM, O::RegN, 0, Flow::None, false},
    {"muls.w", kRm, 0x200F, O::RegM, O::RegN, 0, Flow::None, false},
    // 3nmx
    {"cmp/eq", kRm, 0x3000, O::RegM, O::RegN, 0, Flow::None, false},
    {"cmp/hs", kRm, 0x3002, O::RegM, O::RegN, 0, Flow::None, false},
    {"cmp/ge", kRm, 0x3003, O::RegM, O::RegN, 0, Flow::None, false},
    {"div1", kRm, 0x3004, O::RegM, O::RegN, 0, Flow::None, false},
    {"dmulu.l", kRm, 0x3005, O::RegM, O::RegN, 0, Flow::None, false},
    {"cmp/hi", kRm, 0x3006, O::RegM, O::RegN, 0, Flow::None, false},
    {"cmp/gt", kRm, 0x3007, O::RegM, O::RegN, 0, Flow::None, false},
    {"sub", kRm, 0x3008, O::RegM, O::RegN, 0, Flow::None, false},
    {"subc", kRm, 0x300A, O::RegM, O::RegN, 0, Flow::None, false},
    {"subv", kRm, 0x300B, O::RegM, O::RegN, 0, Flow::None, false},
    {"add", kRm, 0x300C, O::RegM, O::RegN, 0, Flow::None, false},
    {"dmuls.l", kRm, 0x300D, O::RegM, O::RegN, 0, Flow::None, false},
    {"addc", kRm, 0x300E, O::RegM, O::RegN, 0, Flow::None, false},
    {"addv", kRm, 0x300F, O::RegM, O::RegN, 0, Flow::None, false},
    // 4nxx
    {"shll", kRn, 0x4000, O::RegN, O::None, 0, Flow::None, false},
    {"shlr", kRn, 0x4001, O::RegN, O::None, 0, Flow::None, false},
    {"sts.l", kRn, 0x4002, O::Mach, O::PreN, 4, Flow::None, false},
    {"stc.l", kRn, 0x4003, O::Sr, O::PreN, 4, Flow::None, false},
    {"rotl", kRn, 0x4004, O::RegN, O::None, 0, Flow::None, false},
    {"rotr", kRn, 0x4005, O::RegN, O::None, 0, Flow::None, false},
    {"lds.l", kRn, 0x4006, O::PostN, O::Mach, 4, Flow::None, false},
    {"ldc.l", kRn, 0x4007, O::PostN, O::Sr, 4, Flow::None, false},
    {"shll2", kRn, 0x4008, O::RegN, O::None, 0, Flow::None, false},
    {"shlr2", kRn, 0x4009, O::RegN, O::None, 0, Flow::None, false},
    {"lds", kRn, 0x400A, O::RegN, O::Mach, 0, Flow::None, false},
    {"jsr", kRn, 0x400B, O::IndN, O::None, 0, Flow::Call, true},
    {"ldc", kRn, 0x400E, O::RegN, O::Sr, 0, Flow::None, false},
    {"mac.w", kRm, 0x400F, O::PostM, O::PostN, 2, Flow::None, false},
    {"dt", kRn, 0x4010, O::RegN, O::None, 0, Flow::None, false},
    {"cmp/pz", kRn, 0x4011, O::RegN, O::None, 0, Flow::None, false},
    {"sts.l", kRn, 0x4012, O::Macl, O::PreN, 4, Flow::None, false},
    {"stc.l", kRn, 0x4013, O::Gbr, O::PreN, 4, Flow::None, false},
    {"cmp/pl", kRn, 0x4015, O::RegN, O::None, 0, Flow::None, false},
    {"lds.l", kRn, 0x4016, O::PostN, O::Macl, 4, Flow::None, false},
    {"ldc.l", kRn, 0x4017, O::PostN, O::Gbr, 4, Flow::None, false},
    {"shll8", kRn, 0x4018, O::RegN, O::None, 0, Flow::None, false},
    {"shlr8", kRn, 0x4019, O::RegN, O::None, 0, Flow::None, false},
    {"lds", kRn, 0x401A, O::RegN, O::Macl, 0, Flow::None, false},
    {"tas.b", kRn, 0x401B, O::IndN, O::None, 1, Flow::None, false},
    {"ldc", kRn, 0x401E, O::RegN, O::Gbr, 0, Flow::None, false},
    {"shal", kRn, 0x4020, O::RegN, O::None, 0, Flow::None, false},
    {"shar", kRn, 0x4021, O::RegN, O::None, 0, Flow::None, false},
    {"sts.l", kRn, 0x4022, O::Pr, O::PreN, 4, Flow::None, false},
    {"stc.l", kRn, 0x4023, O::Vbr, O::PreN, 4, Flow::None, false},
    {"rotcl", kRn, 0x4024, O::RegN, O::None, 0, Flow::None, false},
    {"rotcr", kRn, 0x4025, O::RegN, O::None, 0, Flow::None, false},
    {"lds.l", kRn, 0x4026, O::PostN, O::Pr, 4, Flow::None, false},
    {"ldc.l", kRn, 0x4027, O::PostN, O::Vbr, 4, Flow::None, false},
    {"shll16", kRn, 0x4028, O::RegN, O::None, 0, Flow::None, false},
    {"shlr16", kRn, 0x4029, O::RegN, O::None, 0, Flow::None, false},
    {"lds", kRn, 0x402A, O::RegN, O::Pr, 0, Flow::None, false},
    {"jmp", kRn, 0x402B, O::IndN, O::None, 0, Flow::Indirect, true},
    {"ldc", kRn, 0x402E, O::RegN, O::Vbr, 0, Flow::None, false},
    // 5nmd
    {"mov.l", kTop, 0x5000, O::DispM, O::RegN, 4, Flow::None, false},
    // 6nmx
    {"mov.b", kRm, 0x6000, O::IndM, O::RegN, 1, Flow::None, false},
    {"mov.w", kRm, 0x6001, O::IndM, O::RegN, 2, Flow::None, false},
    {"mov.l", kRm, 0x6002, O::IndM, O::RegN, 4, Flow::None, false},
    {"mov", kRm, 0x6003, O::RegM, O::RegN, 0, Flow::None, false},
    {"mov.b", kRm, 0x6004, O::PostM, O::RegN, 1, Flow::None, false},
    {"mov.w", kRm, 0x6005, O::PostM, O::RegN, 2, Flow::None, false},
    {"mov.l", kRm, 0x6006, O::PostM, O::RegN, 4, Flow::None, false},
    {"not", kRm, 0x6007, O::RegM, O::RegN, 0, Flow::None, false},
    {"swap.b", kRm, 0x6008, O::RegM, O::RegN, 0, Flow::None, false},
    {"swap.w", kRm, 0x6009, O::RegM, O::RegN, 0, Flow::None, false},
    {"negc", kRm, 0x600A, O::RegM, O::RegN, 0, Flow::None, false},
    {"neg", kRm, 0x600B, O::RegM, O::RegN, 0, Flow::None, false},
    {"extu.b", kRm, 0x600C, O::RegM, O::RegN, 0, Flow::None, false},
    {"extu.w", kRm, 0x600D, O::RegM, O::RegN, 0, Flow::None, false},
    {"exts.b", kRm, 0x600E, O::RegM, O::RegN, 0, Flow::None, false},
    {"exts.w", kRm, 0x600F, O::RegM, O::RegN, 0, Flow::None, false},
    // 7nii
    {"add", kTop, 0x7000, O::ImmS, O::RegN, 0, Flow::None, false},
    // 8xxx
    {"mov.b", kByte, 0x8000, O::R0, O::DispM, 1, Flow::None, false},
    {"mov.w", kByte, 0x8100, O::R0, O::DispM, 2, Flow::None, false},
    {"mov.b", kByte, 0x8400, O::DispM, O::R0, 1, Flow::None, false},
    {"mov.w", kByte, 0x8500, O::DispM, O::R0, 2, Flow::None, false},
    {"cmp/eq", kByte, 0x8800, O::ImmS, O::R0, 0, Flow::None, false},
    {"bt", kByte, 0x8900, O::Branch8, O::None, 0, Flow::Branch, false},
    {"bf", kByte, 0x8B00, O::Branch8, O::None, 0, Flow::Branch, false},
    {"bt/s", kByte, 0x8D00, O::Branch8, O::None, 0, Flow::Branch, true},
    {"bf/s", kByte, 0x8F00, O::Branch8, O::None, 0, Flow::Branch, true},
    // 9ndd, Axxx, Bxxx
    {"mov.w", kTop, 0x9000, O::PcDisp, O::RegN, 2, Flow::None, false},
    {"bra", kTop, 0xA000, O::Branch12, O::None, 0, Flow::Jump, true},
    {"bsr", kTop, 0xB000, O::Branch12, O::None, 0, Flow::Call, true},
    // Cxxx
    {"mov.b", kByte, 0xC000, O::R0, O::GbrDisp, 1, Flow::None, false},
    {"mov.w", kByte, 0xC100, O::R0, O::GbrDisp, 2, Flow::None, false},
    {"mov.l", kByte, 0xC200, O::R0, O::GbrDisp, 4, Flow::None, false},
    {"trapa", kByte, 0xC300, O::ImmU, O::None, 0, Flow::None, false},
    {"mov.b", kByte, 0xC400, O::GbrDisp, O::R0, 1, Flow::None, false},
    {"mov.w", kByte, 0xC500, O::GbrDisp, O::R0, 2, Flow::None, false},
    {"mov.l", kByte, 0xC600, O::GbrDisp, O::R0, 4, Flow::None, false},
    {"mova", kByte, 0xC700, O::PcDisp, O::R0, 4, Flow::None, false},
    {"tst", kByte, 0xC800, O::ImmU, O::R0, 0, Flow::None, false},
    {"and", kByte, 0xC900, O::ImmU, O::R0, 0, Flow::None, false},
    {"xor", kByte, 0xCA00, O::ImmU, O::R0, 0, Flow::None, false},
    {"or", kByte, 0xCB00, O::ImmU, O::R0, 0, Flow::None, false},
    {"tst.b", kByte, 0xCC00, O::ImmU, O::GbrR0, 1, Flow::None, false},
    {"and.b", kByte, 0xCD00, O::ImmU, O::GbrR0, 1, Flow::None, false},
    {"xor.b", kByte, 0xCE00, O::ImmU, O::GbrR0, 1, Flow::None, false},
    {"or.b", kByte, 0xCF00, O::ImmU, O::GbrR0, 1, Flow::None, false},
    // Dndd, Enii
    {"mov.l", kTop, 0xD000, O::PcDisp, O::RegN, 4, Flow::None, false},
    {"mov", kTop, 0xE000, O::ImmS, O::RegN, 0, Flow::None, false},
});

// The definitions whose first nibble is `top`, in table order.
const std::array<std::vector<const Def*>, 16>& by_top_nibble() {
    static const std::array<std::vector<const Def*>, 16> table = [] {
        std::array<std::vector<const Def*>, 16> t;
        for (const Def& d : kDefs) t[d.bits >> 12].push_back(&d);
        return t;
    }();
    return table;
}

std::int32_t sign_extend(std::uint32_t v, unsigned bits) {
    const std::uint32_t top = 1u << (bits - 1);
    return static_cast<std::int32_t>((v ^ top) - top);
}


}  // namespace

std::optional<Insn> decode_word(std::uint16_t word, std::uint32_t pc) {
    for (const Def* d : by_top_nibble()[word >> 12]) {
        if ((word & d->mask) != d->bits) continue;
        Insn i;
        i.word = word;
        i.pc = pc;
        i.mnemonic = std::string(d->name);
        i.a = d->a;
        i.b = d->b;
        i.n = (word >> 8) & 15;
        i.m = (word >> 4) & 15;
        i.size = d->size;
        i.flow = d->flow;
        i.delay_slot = d->delay;
        i.raw = {static_cast<std::uint8_t>(word >> 8), static_cast<std::uint8_t>(word)};
        for (const O o : {d->a, d->b}) {
            switch (o) {
                case O::ImmS: i.imm = sign_extend(word & 0xFF, 8); break;
                case O::ImmU: i.imm = word & 0xFF; break;
                case O::DispN: case O::DispM: i.imm = static_cast<std::int32_t>(word & 15) * d->size; break;
                case O::GbrDisp: i.imm = static_cast<std::int32_t>(word & 0xFF) * d->size; break;
                case O::PcDisp: {
                    const std::uint32_t base = d->size == 4 ? (pc + 4) & ~3u : pc + 4;
                    i.imm = static_cast<std::int32_t>(word & 0xFF) * d->size;
                    i.literal = base + static_cast<std::uint32_t>(i.imm);
                    break;
                }
                case O::Branch8: i.target = pc + 4 + static_cast<std::uint32_t>(sign_extend(word & 0xFF, 8) * 2); break;
                case O::Branch12: i.target = pc + 4 + static_cast<std::uint32_t>(sign_extend(word & 0xFFF, 12) * 2); break;
                default: break;
            }
        }
        return i;
    }
    return std::nullopt;
}

std::optional<Insn> decode(ByteView code, std::size_t at, std::uint32_t pc) {
    if (at + 2 > code.size()) return std::nullopt;
    return decode_word(static_cast<std::uint16_t>(code[at] << 8 | code[at + 1]), pc);
}

// ---- text -------------------------------------------------------------------------------------------------------

std::string_view register_name(unsigned number) {
    static constexpr std::array<std::string_view, 16> names = {"r0", "r1", "r2",  "r3",  "r4",  "r5",  "r6",  "r7",
                                                               "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"};
    return names.at(number & 15);
}

std::optional<unsigned> register_number(std::string_view t) {
    t = text::trim(t);
    if (t.size() == 2 && (t[0] == 's' || t[0] == 'S') && (t[1] == 'p' || t[1] == 'P')) return 15;
    if (t.size() < 2 || t.size() > 3 || (t[0] != 'r' && t[0] != 'R')) return std::nullopt;
    const auto n = text::parse_int<unsigned>(t.substr(1));
    if (!n || *n > 15) return std::nullopt;
    if (t.size() == 3 && t[1] == '0') return std::nullopt;   // (r05 is not a register's name)
    return *n;
}

namespace {

std::string address_text(std::uint32_t address, const AddressLabels& labels) {
    const auto it = labels.find(address);
    if (it != labels.end() && !it->second.empty()) return it->second;
    return std::format("0x{:08X}", address);
}

std::string operand_text(const Insn& i, O o, const AddressLabels& labels) {
    switch (o) {
        case O::None: return {};
        case O::RegN: return std::string(register_name(i.n));
        case O::RegM: return std::string(register_name(i.m));
        case O::R0: return "r0";
        case O::ImmS: return std::format("#{}", i.imm);
        case O::ImmU: return i.imm > 9 ? std::format("#0x{:X}", i.imm) : std::format("#{}", i.imm);
        case O::IndN: return std::format("@{}", register_name(i.n));
        case O::IndM: return std::format("@{}", register_name(i.m));
        case O::PostN: return std::format("@{}+", register_name(i.n));
        case O::PostM: return std::format("@{}+", register_name(i.m));
        case O::PreN: return std::format("@-{}", register_name(i.n));
        case O::DispN: return std::format("@({},{})", i.imm, register_name(i.n));
        case O::DispM: return std::format("@({},{})", i.imm, register_name(i.m));
        case O::R0IndexN: return std::format("@(r0,{})", register_name(i.n));
        case O::R0IndexM: return std::format("@(r0,{})", register_name(i.m));
        case O::GbrDisp: return std::format("@({},gbr)", i.imm);
        case O::GbrR0: return "@(r0,gbr)";
        case O::PcDisp: {
            const auto it = labels.find(*i.literal);
            if (it != labels.end() && !it->second.empty()) return it->second;
            return std::format("@({},pc)", i.imm);
        }
        case O::Branch8: case O::Branch12: return address_text(*i.target, labels);
        case O::Sr: return "sr";
        case O::Gbr: return "gbr";
        case O::Vbr: return "vbr";
        case O::Mach: return "mach";
        case O::Macl: return "macl";
        case O::Pr: return "pr";
    }
    return {};
}

}  // namespace

std::string text(const Insn& insn, const AddressLabels& labels) {
    const std::string a = operand_text(insn, insn.a, labels);
    if (a.empty()) return insn.mnemonic;
    const std::string b = operand_text(insn, insn.b, labels);
    return std::format("{:<8}{}", insn.mnemonic, b.empty() ? a : a + "," + b);
}

// ---- expressions ------------------------------------------------------------------------------------------------

std::int64_t value(std::string_view expr, const NameLabels& labels, std::uint32_t pc) { return mips::value(expr, labels, pc); }

// ---- assembling -------------------------------------------------------------------------------------------------

namespace {

[[noreturn]] void fail(std::string_view line, const std::string& why) { throw ParseError("", 0, 0, std::string(line) + " (" + why + ")"); }

// One operand of an assembly line, by its shape.
struct Parsed {
    enum class Kind : std::uint8_t { Reg, Imm, Ind, Post, Pre, DispReg, R0Index, GbrDisp, GbrR0, PcDisp, Control, Bare };
    Kind kind = Kind::Bare;
    unsigned reg = 0;
    std::string_view expr;
    O control = O::None;
};

std::optional<O> control_register(std::string_view t) {
    const std::string name = text::lower(t);
    if (name == "sr") return O::Sr;
    if (name == "gbr") return O::Gbr;
    if (name == "vbr") return O::Vbr;
    if (name == "mach") return O::Mach;
    if (name == "macl") return O::Macl;
    if (name == "pr") return O::Pr;
    return std::nullopt;
}

// Split `text` at its commas that are not inside parentheses.
std::vector<std::string_view> operands_of(std::string_view t) {
    std::vector<std::string_view> out;
    int depth = 0;
    std::size_t from = 0;
    for (std::size_t i = 0; i < t.size(); ++i) {
        if (t[i] == '(') ++depth;
        else if (t[i] == ')') --depth;
        else if (t[i] == ',' && depth == 0) {
            out.push_back(text::trim(t.substr(from, i - from)));
            from = i + 1;
        }
    }
    if (!text::trim(t.substr(from)).empty() || !out.empty()) out.push_back(text::trim(t.substr(from)));
    return out;
}

Parsed parse_operand(std::string_view line, std::string_view t) {
    t = text::trim(t);
    if (t.empty()) fail(line, "an operand is missing");
    Parsed p;
    if (t[0] == '#') {
        p.kind = Parsed::Kind::Imm;
        p.expr = text::trim(t.substr(1));
        return p;
    }
    if (t[0] == '@') {
        const std::string_view rest = text::trim(t.substr(1));
        if (rest.starts_with('-')) {
            const auto r = register_number(rest.substr(1));
            if (!r) fail(line, "a register is missing after @-");
            p.kind = Parsed::Kind::Pre;
            p.reg = *r;
            return p;
        }
        if (rest.starts_with('(')) {
            if (!rest.ends_with(')')) fail(line, "a ')' is missing");
            const std::vector<std::string_view> parts = operands_of(rest.substr(1, rest.size() - 2));
            if (parts.size() != 2) fail(line, "@(disp,register) takes a displacement and a register");
            const auto base = register_number(parts[1]);
            const bool first_r0 = register_number(parts[0]) == 0u;
            if (base) {
                if (first_r0) {
                    p.kind = Parsed::Kind::R0Index;
                } else {
                    p.kind = Parsed::Kind::DispReg;
                    p.expr = parts[0];
                }
                p.reg = *base;
                return p;
            }
            const std::string second = text::lower(parts[1]);
            if (second == "gbr") {
                p.kind = first_r0 ? Parsed::Kind::GbrR0 : Parsed::Kind::GbrDisp;
                p.expr = parts[0];
                return p;
            }
            if (second == "pc") {
                p.kind = Parsed::Kind::PcDisp;
                p.expr = parts[0];
                return p;
            }
            fail(line, "the base of @(disp,base) is a register, gbr or pc");
        }
        const bool post = rest.ends_with('+');
        const auto r = register_number(post ? rest.substr(0, rest.size() - 1) : rest);
        if (!r) fail(line, "a register is missing after @");
        p.kind = post ? Parsed::Kind::Post : Parsed::Kind::Ind;
        p.reg = *r;
        return p;
    }
    if (const auto r = register_number(t)) {
        p.kind = Parsed::Kind::Reg;
        p.reg = *r;
        return p;
    }
    if (const auto c = control_register(t)) {
        p.kind = Parsed::Kind::Control;
        p.control = *c;
        return p;
    }
    p.kind = Parsed::Kind::Bare;
    p.expr = t;
    return p;
}

// Can an operand written like `p` be the table's operand `o`?
bool shape_matches(O o, const Parsed& p) {
    using K = Parsed::Kind;
    switch (o) {
        case O::None: return false;
        case O::RegN: case O::RegM: return p.kind == K::Reg;
        case O::R0: return p.kind == K::Reg && p.reg == 0;
        case O::ImmS: case O::ImmU: return p.kind == K::Imm;
        case O::IndN: case O::IndM: return p.kind == K::Ind;
        case O::PostN: case O::PostM: return p.kind == K::Post;
        case O::PreN: return p.kind == K::Pre;
        case O::DispN: case O::DispM: return p.kind == K::DispReg;
        case O::R0IndexN: case O::R0IndexM: return p.kind == K::R0Index;
        case O::GbrDisp: return p.kind == K::GbrDisp;
        case O::GbrR0: return p.kind == K::GbrR0;
        case O::PcDisp: return p.kind == K::PcDisp || p.kind == K::Bare;
        case O::Branch8: case O::Branch12: return p.kind == K::Bare;
        case O::Sr: case O::Gbr: case O::Vbr: case O::Mach: case O::Macl: case O::Pr: return p.kind == K::Control && p.control == o;
    }
    return false;
}

// A displacement in bytes as its field: a multiple of the unit, 0 up to `limit` units.
std::uint32_t scaled(std::string_view line, std::int64_t bytes, unsigned unit, unsigned limit, std::string_view what) {
    unit = std::max(unit, 1u);
    if (bytes < 0 || bytes % unit != 0 || bytes / unit > static_cast<std::int64_t>(limit))
        fail(line, std::format("{} is not a displacement of {} to {} bytes in steps of {}", bytes, what, limit * unit, unit));
    return static_cast<std::uint32_t>(bytes / unit);
}

}  // namespace

Bytes assemble(std::string_view line_in, std::uint32_t pc, const NameLabels& labels) {
    const std::string_view line = text::trim(line_in);
    std::size_t split = 0;
    while (split < line.size() && !text::is_space(line[split])) ++split;
    const std::string mnemonic = text::lower(line.substr(0, split));
    if (mnemonic.empty()) fail(line, "no instruction");
    const std::vector<std::string_view> raw = operands_of(text::trim(line.substr(split)));
    std::vector<Parsed> operands;
    for (const std::string_view r : raw) operands.push_back(parse_operand(line, r));

    bool known = false;
    for (const Def& d : kDefs) {
        if (d.name != mnemonic) continue;
        known = true;
        const std::size_t wanted = d.a == O::None ? 0 : d.b == O::None ? 1 : 2;
        if (wanted != operands.size()) continue;
        if (wanted >= 1 && !shape_matches(d.a, operands[0])) continue;
        if (wanted == 2 && !shape_matches(d.b, operands[1])) continue;

        std::uint32_t word = d.bits;
        for (std::size_t k = 0; k < wanted; ++k) {
            const O o = k == 0 ? d.a : d.b;
            const Parsed& p = operands[k];
            const auto number = [&](std::string_view expr) { return value(expr, labels, pc); };
            switch (o) {
                case O::RegN: case O::IndN: case O::PostN: case O::PreN: case O::R0IndexN: word |= p.reg << 8; break;
                case O::RegM: case O::IndM: case O::PostM: case O::R0IndexM: word |= p.reg << 4; break;
                case O::ImmS: {
                    const std::int64_t v = number(p.expr);
                    if (v < -128 || v > 255) fail(line, std::format("#{} does not fit in 8 bits", v));
                    word |= static_cast<std::uint32_t>(v) & 0xFF;
                    break;
                }
                case O::ImmU: {
                    const std::int64_t v = number(p.expr);
                    if (v < 0 || v > 255) fail(line, std::format("#{} does not fit in 8 bits", v));
                    word |= static_cast<std::uint32_t>(v);
                    break;
                }
                case O::DispN: word |= p.reg << 8 | scaled(line, number(p.expr), d.size, 15, "@(disp,rn)"); break;
                case O::DispM: word |= p.reg << 4 | scaled(line, number(p.expr), d.size, 15, "@(disp,rm)"); break;
                case O::GbrDisp: word |= scaled(line, number(p.expr), d.size, 255, "@(disp,gbr)"); break;
                case O::PcDisp: {
                    const std::uint32_t base = d.size == 4 ? (pc + 4) & ~3u : pc + 4;
                    const std::int64_t bytes = p.kind == Parsed::Kind::PcDisp ? number(p.expr) : number(p.expr) - static_cast<std::int64_t>(base);
                    word |= scaled(line, bytes, d.size, 255, "@(disp,pc)");
                    break;
                }
                case O::Branch8: case O::Branch12: {
                    const std::int64_t bytes = number(p.expr) - (static_cast<std::int64_t>(pc) + 4);
                    const std::int64_t half = bytes / 2;
                    const std::int64_t limit = o == O::Branch8 ? 128 : 2048;
                    if (bytes % 2 != 0 || half < -limit || half >= limit) fail(line, "the branch target is out of reach");
                    word |= static_cast<std::uint32_t>(half) & (o == O::Branch8 ? 0xFF : 0xFFF);
                    break;
                }
                default: break;
            }
        }
        return {static_cast<std::uint8_t>(word >> 8), static_cast<std::uint8_t>(word)};
    }
    fail(line, known ? "the operands do not fit the instruction" : "unknown instruction " + mnemonic);
}

}  // namespace cb::decompile::sh2
