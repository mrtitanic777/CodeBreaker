// cb-unity: alone
#include "decompile/v810.hpp"

#include <algorithm>
#include <cctype>
#include <format>
#include <unordered_map>
#include <vector>

#include "core/text.hpp"

namespace cb::decompile::v810 {

namespace {

using Table = std::array<std::optional<OpcodeEntry>, 64>;

Table build_main() {
    Table t{};
    const auto put = [&](unsigned op, std::string_view m, Mode mode) { t[op] = OpcodeEntry{m, mode}; };
    put(0x00, "mov", Mode::R1R2);   put(0x01, "add", Mode::R1R2);   put(0x02, "sub", Mode::R1R2);   put(0x03, "cmp", Mode::R1R2);
    put(0x04, "shl", Mode::R1R2);   put(0x05, "shr", Mode::R1R2);   put(0x06, "jmp", Mode::Jmp);     put(0x07, "sar", Mode::R1R2);
    put(0x08, "mul", Mode::R1R2);   put(0x09, "div", Mode::R1R2);   put(0x0A, "mulu", Mode::R1R2);   put(0x0B, "divu", Mode::R1R2);
    put(0x0C, "or", Mode::R1R2);    put(0x0D, "and", Mode::R1R2);   put(0x0E, "xor", Mode::R1R2);    put(0x0F, "not", Mode::R1R2);
    put(0x10, "mov", Mode::S5R2);   put(0x11, "add", Mode::S5R2);   put(0x12, "setf", Mode::U5R2);   put(0x13, "cmp", Mode::S5R2);
    put(0x14, "shl", Mode::U5R2);   put(0x15, "shr", Mode::U5R2);   put(0x16, "cli", Mode::None);    put(0x17, "sar", Mode::U5R2);
    put(0x18, "trap", Mode::Trap);  put(0x19, "reti", Mode::None);  put(0x1A, "halt", Mode::None);
    put(0x1C, "ldsr", Mode::LdSr);  put(0x1D, "stsr", Mode::StSr);  put(0x1E, "sei", Mode::None);
    put(0x1F, "", Mode::None);   // (the bit-string instructions: the sub-table names them)
    put(0x28, "movea", Mode::Imm16S); put(0x29, "addi", Mode::Imm16S); put(0x2A, "jr", Mode::Jump26); put(0x2B, "jal", Mode::Jump26);
    put(0x2C, "ori", Mode::Imm16U); put(0x2D, "andi", Mode::Imm16U); put(0x2E, "xori", Mode::Imm16U); put(0x2F, "movhi", Mode::Imm16U);
    put(0x30, "ld.b", Mode::Load);  put(0x31, "ld.h", Mode::Load);  put(0x33, "ld.w", Mode::Load);
    put(0x34, "st.b", Mode::Store); put(0x35, "st.h", Mode::Store); put(0x37, "st.w", Mode::Store);
    put(0x38, "in.b", Mode::Load);  put(0x39, "in.h", Mode::Load);  put(0x3A, "caxi", Mode::Load);   put(0x3B, "in.w", Mode::Load);
    put(0x3C, "out.b", Mode::Store); put(0x3D, "out.h", Mode::Store); put(0x3F, "out.w", Mode::Store);
    put(0x3E, "", Mode::None);   // (the extended instructions)
    return t;
}

std::array<std::optional<OpcodeEntry>, 64> build_extended() {
    std::array<std::optional<OpcodeEntry>, 64> t{};
    t[0x00] = OpcodeEntry{"cmpf.s", Mode::R1R2};  t[0x02] = OpcodeEntry{"cvt.ws", Mode::R1R2};  t[0x03] = OpcodeEntry{"cvt.sw", Mode::R1R2};
    t[0x04] = OpcodeEntry{"addf.s", Mode::R1R2};  t[0x05] = OpcodeEntry{"subf.s", Mode::R1R2};  t[0x06] = OpcodeEntry{"mulf.s", Mode::R1R2};
    t[0x07] = OpcodeEntry{"divf.s", Mode::R1R2};  t[0x08] = OpcodeEntry{"xb", Mode::R2};       t[0x09] = OpcodeEntry{"xh", Mode::R2};
    t[0x0A] = OpcodeEntry{"rev", Mode::R1R2};     t[0x0B] = OpcodeEntry{"trnc.sw", Mode::R1R2}; t[0x0C] = OpcodeEntry{"mpyhw", Mode::R1R2};
    return t;
}

std::array<std::optional<OpcodeEntry>, 32> build_bitstring() {
    std::array<std::optional<OpcodeEntry>, 32> t{};
    const std::string_view names[16] = {"sch0bsu", "sch0bsd", "sch1bsu", "sch1bsd", "", "", "", "", "orbsu", "andbsu", "xorbsu", "movbsu", "ornbsu", "andnbsu", "xornbsu", "notbsu"};
    for (unsigned i = 0; i < 16; ++i) {
        if (!names[i].empty()) t[i] = OpcodeEntry{names[i], Mode::None};
    }
    return t;
}

std::int32_t sext(std::uint32_t v, unsigned bits) {
    const std::uint32_t m = 1u << (bits - 1);
    return static_cast<std::int32_t>((v ^ m) - m);
}

std::string number_text(std::int64_t v) {
    if (v >= -9 && v <= 9) return std::to_string(v);
    return v < 0 ? std::format("-0x{:X}", static_cast<std::uint64_t>(-v)) : std::format("0x{:X}", static_cast<std::uint64_t>(v));
}

std::string reg_text(unsigned r) { return "r" + std::to_string(r); }

}  // namespace

const std::array<std::optional<OpcodeEntry>, 64>& opcode_table() {
    static const Table t = build_main();
    return t;
}
const std::array<std::optional<OpcodeEntry>, 64>& extended_table() {
    static const std::array<std::optional<OpcodeEntry>, 64> t = build_extended();
    return t;
}
const std::array<std::optional<OpcodeEntry>, 32>& bitstring_table() {
    static const std::array<std::optional<OpcodeEntry>, 32> t = build_bitstring();
    return t;
}

std::string_view system_register_name(unsigned n) {
    switch (n) {
        case 0: return "eipc";
        case 1: return "eipsw";
        case 2: return "fepc";
        case 3: return "fepsw";
        case 4: return "ecr";
        case 5: return "psw";
        case 6: return "pir";
        case 7: return "tkcw";
        case 24: return "chcw";
        case 25: return "adtre";
        default: return {};
    }
}

// ---- the mnemonics, for the assembler -----------------------------------------------------------------------------------

namespace {

struct Candidate {
    unsigned id;
    Mode mode;
    unsigned cond = 0;
};

const std::unordered_map<std::string, std::vector<Candidate>>& by_mnemonic() {
    static const std::unordered_map<std::string, std::vector<Candidate>> table = [] {
        std::unordered_map<std::string, std::vector<Candidate>> m;
        for (unsigned op = 0; op < 64; ++op) {
            const auto& e = opcode_table()[op];
            if (!e || e->mnemonic.empty()) continue;
            m[std::string(e->mnemonic)].push_back({op, e->mode});
        }
        for (unsigned s = 0; s < 64; ++s) {
            if (const auto& e = extended_table()[s]) m[std::string(e->mnemonic)].push_back({id::kExtendedBase + s, e->mode});
        }
        for (unsigned s = 0; s < 32; ++s) {
            if (const auto& e = bitstring_table()[s]) m[std::string(e->mnemonic)].push_back({id::kBitStringBase + s, Mode::None});
        }
        for (unsigned c = 0; c < 16; ++c) m["b" + std::string(kConditions[c])].push_back({id::kBranch, Mode::Branch, c});
        for (const auto& [alias, c] : {std::pair<const char*, unsigned>{"bc", 1}, {"bz", 2}, {"bnc", 9}, {"bnz", 10}}) m[alias].push_back({id::kBranch, Mode::Branch, c});
        m["nop"].push_back({id::kMov, Mode::None});
        return m;
    }();
    return table;
}

}  // namespace

bool is_mnemonic(std::string_view mnemonic) { return by_mnemonic().contains(text::lower(mnemonic)); }

// ---- decoding -----------------------------------------------------------------------------------------------------------

std::optional<Insn> decode(ByteView code, std::size_t offset, std::uint32_t pc) {
    if (offset + 2 > code.size()) return std::nullopt;
    const std::uint32_t hw0 = code[offset] | code[offset + 1] << 8;
    Insn i;
    i.at = pc;
    i.raw[0] = code[offset];
    i.raw[1] = code[offset + 1];
    i.r1 = hw0 & 31;
    i.r2 = (hw0 >> 5) & 31;
    if ((hw0 >> 13) == 4) {   // a branch
        const std::uint32_t d = hw0 & 0x1FF;
        if (d & 1) return std::nullopt;
        i.size = 2;
        i.id = id::kBranch;
        i.cond = (hw0 >> 9) & 15;
        i.mnemonic = {};   // (set below: needs static storage)
        i.mode = Mode::Branch;
        i.imm = sext(d, 9);
        i.target = pc + static_cast<std::uint32_t>(i.imm);
        i.flow = i.cond == 5 ? Flow::Jump : i.cond == 13 ? Flow::None : Flow::Branch;
        i.r1 = i.r2 = 0;
        static const std::array<std::string, 16> names = [] {
            std::array<std::string, 16> n;
            for (unsigned c = 0; c < 16; ++c) n[c] = "b" + std::string(kConditions[c]);
            return n;
        }();
        i.mnemonic = names[i.cond];
        return i;
    }
    const unsigned opcode = hw0 >> 10;
    const auto& entry = opcode_table()[opcode];
    if (!entry) return std::nullopt;
    i.size = opcode >= 0x28 ? 4 : 2;
    if (offset + i.size > code.size()) return std::nullopt;
    std::uint32_t hw1 = 0;
    if (i.size == 4) {
        i.raw[2] = code[offset + 2];
        i.raw[3] = code[offset + 3];
        hw1 = code[offset + 2] | code[offset + 3] << 8;
    }
    i.id = opcode;
    i.mnemonic = entry->mnemonic;
    i.mode = entry->mode;
    switch (opcode) {
        case id::kBitString: {
            const auto& b = bitstring_table()[i.r1];
            if (!b || i.r2 != 0) return std::nullopt;
            i.id = id::kBitStringBase + i.r1;
            i.mnemonic = b->mnemonic;
            i.imm = i.r1;
            i.r1 = 0;
            return i;
        }
        case id::kExtended: {
            const unsigned sub = hw1 >> 10;
            const auto& e = extended_table()[sub];
            if (!e || (hw1 & 0x3FF) != 0) return std::nullopt;
            if (e->mode == Mode::R2 && i.r1 != 0) return std::nullopt;
            i.id = id::kExtendedBase + sub;
            i.mnemonic = e->mnemonic;
            i.mode = e->mode;
            return i;
        }
        default: break;
    }
    switch (i.mode) {
        case Mode::None:
            if (i.r1 != 0 || i.r2 != 0) return std::nullopt;
            if (opcode == id::kReti) i.flow = Flow::Return;
            if (opcode == id::kHalt) i.flow = Flow::Halt;
            break;
        case Mode::R1R2: break;
        case Mode::Jmp:
            if (i.r2 != 0) return std::nullopt;
            i.flow = i.r1 == 31 ? Flow::Return : Flow::Indirect;
            break;
        case Mode::S5R2: i.imm = sext(i.r1, 5); i.r1 = 0; break;
        case Mode::U5R2: i.imm = i.r1; i.r1 = 0; break;
        case Mode::Trap:
            if (i.r2 != 0) return std::nullopt;
            i.imm = i.r1;
            i.r1 = 0;
            break;
        case Mode::LdSr:
        case Mode::StSr: i.imm = i.r1; i.r1 = 0; break;
        case Mode::Jump26: {
            const std::uint32_t d = ((hw0 & 0x3FF) << 16) | hw1;
            if (d & 1) return std::nullopt;
            i.imm = sext(d, 26);
            i.target = pc + static_cast<std::uint32_t>(i.imm);
            i.flow = opcode == id::kJal ? Flow::Call : Flow::Jump;
            i.r1 = i.r2 = 0;
            break;
        }
        case Mode::Imm16S: i.imm = sext(hw1, 16); break;
        case Mode::Imm16U: i.imm = static_cast<std::int32_t>(hw1); break;
        case Mode::Load:
        case Mode::Store: i.imm = sext(hw1, 16); break;
        default: return std::nullopt;
    }
    return i;
}

}  // namespace cb::decompile::v810
