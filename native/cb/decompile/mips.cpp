// cb/decompile/mips.cpp -- the MIPS R3000A's instructions: a table of every encoding, read both ways (mips.hpp).

#include "cb/decompile/mips.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <format>
#include <unordered_map>

#include "cb/core/text.hpp"

namespace cb::decompile::mips {

namespace {

// ---- the table --------------------------------------------------------------------------------------------------

struct Def {
    std::string_view name;
    Form form;
    std::uint32_t match;   // the word's fixed bits ...
    std::uint32_t mask;    // ... and which bits those are (everything else is an operand)
    Flow flow = Flow::None;
    bool delay = false;
};

constexpr std::uint32_t op(std::uint32_t o) { return o << 26; }
constexpr std::uint32_t kSpecialMask = 0xFC00003F;   // opcode and function
constexpr std::uint32_t kNoSa = 0xFC0007FF;          // ... and shamt zero
constexpr std::uint32_t kRegimmMask = 0xFC1F0000;

constexpr auto kDefs = std::to_array<Def>({
    {"sll", Form::RdRtSa, 0x00, 0xFFE0003F},
    {"srl", Form::RdRtSa, 0x02, 0xFFE0003F},
    {"sra", Form::RdRtSa, 0x03, 0xFFE0003F},
    {"sllv", Form::RdRtRs, 0x04, kNoSa},
    {"srlv", Form::RdRtRs, 0x06, kNoSa},
    {"srav", Form::RdRtRs, 0x07, kNoSa},
    {"jr", Form::Rs, 0x08, 0xFC1FFFFF, Flow::Indirect, true},
    {"jalr", Form::RdRs, 0x09, 0xFC1F07FF, Flow::Call, true},
    {"syscall", Form::None, 0x0C, kSpecialMask},
    {"break", Form::None, 0x0D, kSpecialMask},
    {"mfhi", Form::Rd, 0x10, 0xFFFF07FF},
    {"mthi", Form::Rs, 0x11, 0xFC1FFFFF},
    {"mflo", Form::Rd, 0x12, 0xFFFF07FF},
    {"mtlo", Form::Rs, 0x13, 0xFC1FFFFF},
    {"mult", Form::RsRt, 0x18, 0xFC00FFFF},
    {"multu", Form::RsRt, 0x19, 0xFC00FFFF},
    {"div", Form::RsRt, 0x1A, 0xFC00FFFF},
    {"divu", Form::RsRt, 0x1B, 0xFC00FFFF},
    {"add", Form::RdRsRt, 0x20, kNoSa},
    {"addu", Form::RdRsRt, 0x21, kNoSa},
    {"sub", Form::RdRsRt, 0x22, kNoSa},
    {"subu", Form::RdRsRt, 0x23, kNoSa},
    {"and", Form::RdRsRt, 0x24, kNoSa},
    {"or", Form::RdRsRt, 0x25, kNoSa},
    {"xor", Form::RdRsRt, 0x26, kNoSa},
    {"nor", Form::RdRsRt, 0x27, kNoSa},
    {"slt", Form::RdRsRt, 0x2A, kNoSa},
    {"sltu", Form::RdRsRt, 0x2B, kNoSa},
    {"bltz", Form::RsBranch, op(1) | (0u << 16), kRegimmMask, Flow::Branch, true},
    {"bgez", Form::RsBranch, op(1) | (1u << 16), kRegimmMask, Flow::Branch, true},
    {"bltzal", Form::RsBranch, op(1) | (16u << 16), kRegimmMask, Flow::Call, true},
    {"bgezal", Form::RsBranch, op(1) | (17u << 16), kRegimmMask, Flow::Call, true},
    {"j", Form::Jump, op(2), 0xFC000000, Flow::Jump, true},
    {"jal", Form::Jump, op(3), 0xFC000000, Flow::Call, true},
    {"beq", Form::RsRtBranch, op(4), 0xFC000000, Flow::Branch, true},
    {"bne", Form::RsRtBranch, op(5), 0xFC000000, Flow::Branch, true},
    {"blez", Form::RsBranch, op(6), kRegimmMask, Flow::Branch, true},
    {"bgtz", Form::RsBranch, op(7), kRegimmMask, Flow::Branch, true},
    {"addi", Form::RtRsImm, op(8), 0xFC000000},
    {"addiu", Form::RtRsImm, op(9), 0xFC000000},
    {"slti", Form::RtRsImm, op(10), 0xFC000000},
    {"sltiu", Form::RtRsImm, op(11), 0xFC000000},
    {"andi", Form::RtRsImm, op(12), 0xFC000000},
    {"ori", Form::RtRsImm, op(13), 0xFC000000},
    {"xori", Form::RtRsImm, op(14), 0xFC000000},
    {"lui", Form::RtImm, op(15), 0xFFE00000},
    {"mfc0", Form::RtCop, op(0x10) | (0u << 21), 0xFFE007FF},
    {"mtc0", Form::RtCop, op(0x10) | (4u << 21), 0xFFE007FF},
    {"rfe", Form::None, 0x42000010, 0xFFFFFFFF},
    {"mfc2", Form::RtCop, op(0x12) | (0u << 21), 0xFFE007FF},
    {"cfc2", Form::RtCop, op(0x12) | (2u << 21), 0xFFE007FF},
    {"mtc2", Form::RtCop, op(0x12) | (4u << 21), 0xFFE007FF},
    {"ctc2", Form::RtCop, op(0x12) | (6u << 21), 0xFFE007FF},
    {"bc2f", Form::CopBranch, op(0x12) | (8u << 21) | (0u << 16), 0xFFFF0000, Flow::Branch, true},
    {"bc2t", Form::CopBranch, op(0x12) | (8u << 21) | (1u << 16), 0xFFFF0000, Flow::Branch, true},
    {"lb", Form::RtMem, op(0x20), 0xFC000000},
    {"lh", Form::RtMem, op(0x21), 0xFC000000},
    {"lwl", Form::RtMem, op(0x22), 0xFC000000},
    {"lw", Form::RtMem, op(0x23), 0xFC000000},
    {"lbu", Form::RtMem, op(0x24), 0xFC000000},
    {"lhu", Form::RtMem, op(0x25), 0xFC000000},
    {"lwr", Form::RtMem, op(0x26), 0xFC000000},
    {"sb", Form::RtMem, op(0x28), 0xFC000000},
    {"sh", Form::RtMem, op(0x29), 0xFC000000},
    {"swl", Form::RtMem, op(0x2A), 0xFC000000},
    {"sw", Form::RtMem, op(0x2B), 0xFC000000},
    {"swr", Form::RtMem, op(0x2E), 0xFC000000},
    {"lwc2", Form::RtMem, op(0x32), 0xFC000000},
    {"swc2", Form::RtMem, op(0x3A), 0xFC000000},
});

// The GTE's commands: the function in the low six bits, and the whole 25-bit command word the PlayStation SDK's macros make
// of it (what the disassembler writes as the bare name).
struct GteDef {
    std::string_view name;
    std::uint32_t function;
    std::uint32_t usual;   // the command's 25 bits as the SDK writes them
};
constexpr auto kGte = std::to_array<GteDef>({
    {"rtps", 0x01, 0x0180001},  {"nclip", 0x06, 0x1400006}, {"op", 0x0C, 0x178000C},   {"dpcs", 0x10, 0x0780010},
    {"intpl", 0x11, 0x0980011}, {"mvmva", 0x12, 0x0400012}, {"ncds", 0x13, 0x0E80413}, {"cdp", 0x14, 0x1280414},
    {"ncdt", 0x16, 0x0F80416},  {"nccs", 0x1B, 0x108041B},  {"cc", 0x1C, 0x138041C},   {"ncs", 0x1E, 0x0C8041E},
    {"nct", 0x20, 0x0D80420},   {"sqr", 0x28, 0x0A80428},   {"dcpl", 0x29, 0x0680029}, {"dpct", 0x2A, 0x0F8002A},
    {"avsz3", 0x2D, 0x158002D}, {"avsz4", 0x2E, 0x168002E}, {"rtpt", 0x30, 0x0280030}, {"gpf", 0x3D, 0x190003D},
    {"gpl", 0x3E, 0x1A0003E},   {"ncct", 0x3F, 0x118043F},
});

// The bits of a command that are fields: sf (19) and lm (10), and for MVMVA also mx (18-17), v (16-15) and cv (14-13). The others the
// hardware ignores (for the commands but MVMVA that includes 18-13); the SDK fills 24-20 with a command number and leaves the rest zero.
constexpr std::uint32_t kSfLm = 0x80400;
constexpr std::uint32_t kFunctionBits = 0x3F;
constexpr std::uint32_t ignored_mask(bool mvmva) { return 0x1FFFFFF & ~(mvmva ? 0xFE400u : kSfLm) & ~kFunctionBits; }

const GteDef* gte_by_function(std::uint32_t function) {
    for (const GteDef& g : kGte)
        if (g.function == function) return &g;
    return nullptr;
}
const GteDef* gte_by_name(std::string_view name) {
    for (const GteDef& g : kGte)
        if (g.name == name) return &g;
    return nullptr;
}

constexpr std::array<std::string_view, 32> kRegisters = {"$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3", "$t0", "$t1", "$t2",
                                                         "$t3",   "$t4", "$t5", "$t6", "$t7", "$s0", "$s1", "$s2", "$s3", "$s4", "$s5",
                                                         "$s6",   "$s7", "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra"};

std::int32_t sign16(std::uint32_t v) { return static_cast<std::int16_t>(v & 0xFFFF); }

bool unsigned_immediate(std::string_view name) { return name == "andi" || name == "ori" || name == "xori" || name == "lui"; }
bool coprocessor_register(std::string_view name) { return name == "lwc2" || name == "swc2"; }

}  // namespace

std::string_view register_name(unsigned number) { return kRegisters.at(number & 31); }

std::optional<unsigned> register_number(std::string_view t) {
    t = text::trim(t);
    if (t.empty()) return std::nullopt;
    const std::string_view bare = t.front() == '$' ? t.substr(1) : t;
    if (bare.empty()) return std::nullopt;
    if (bare == "s8") return 30;
    if (std::isdigit(static_cast<unsigned char>(bare[0]))) {
        const auto n = text::parse_int<unsigned>(bare);
        if (n && *n < 32) return *n;
        return std::nullopt;
    }
    if (t.front() != '$') return std::nullopt;   // a name without the dollar is a label
    for (unsigned i = 0; i < kRegisters.size(); ++i)
        if (kRegisters[i].substr(1) == bare) return i;
    return std::nullopt;
}

// ---- decoding ---------------------------------------------------------------------------------------------------

std::optional<Insn> decode_word(std::uint32_t word, std::uint32_t pc) {
    Insn insn;
    insn.word = word;
    insn.pc = pc;
    for (unsigned i = 0; i < 4; ++i) insn.raw[i] = static_cast<std::uint8_t>(word >> (8 * i));
    insn.rs = static_cast<std::uint8_t>((word >> 21) & 31);
    insn.rt = static_cast<std::uint8_t>((word >> 16) & 31);
    insn.rd = static_cast<std::uint8_t>((word >> 11) & 31);
    insn.shamt = static_cast<std::uint8_t>((word >> 6) & 31);

    if ((word >> 25) == 0x25) {   // a GTE command: COP2 with bit 25 set
        const std::uint32_t command = word & 0x1FFFFFF;
        const GteDef* g = gte_by_function(command & kFunctionBits);
        insn.imm = static_cast<std::int32_t>(command);
        if (!g) {
            insn.mnemonic = "cop2";
            insn.form = Form::Raw;
            return insn;
        }
        // NCLIP has no fields: its sf and lm must be zero, whatever else is set is the ignored bits
        if (g->name == "nclip" && (command & kSfLm) != 0) {
            insn.mnemonic = "cop2";
            insn.form = Form::Raw;
            return insn;
        }
        insn.mnemonic = std::string(g->name);
        insn.form = Form::Command;
        return insn;
    }
    if (word == 0) {
        insn.mnemonic = "nop";
        insn.form = Form::None;
        return insn;
    }
    for (const Def& d : kDefs) {
        if ((word & d.mask) != d.match) continue;
        insn.mnemonic = std::string(d.name);
        insn.form = d.form;
        insn.flow = d.flow;
        insn.delay_slot = d.delay;
        const std::int32_t offset = sign16(word);
        switch (d.form) {
            case Form::None:
                insn.imm = static_cast<std::int32_t>((word >> 6) & 0xFFFFF);   // syscall, break: the code
                if (d.name == "rfe") insn.imm = 0;
                break;
            case Form::RtRsImm:
            case Form::RtImm: insn.imm = unsigned_immediate(d.name) ? static_cast<std::int32_t>(word & 0xFFFF) : offset; break;
            case Form::RtMem: insn.imm = offset; break;
            case Form::RsRtBranch:
            case Form::RsBranch:
            case Form::CopBranch: insn.target = pc + 4 + static_cast<std::uint32_t>(offset) * 4; break;
            case Form::Jump: insn.target = ((pc + 4) & 0xF0000000u) | ((word & 0x3FFFFFF) << 2); break;
            default: break;
        }
        if (d.name == "jr") insn.flow = insn.rs == 31 ? Flow::Return : Flow::Indirect;
        if (d.name == "beq" && insn.rs == 0 && insn.rt == 0) insn.flow = Flow::Jump;   // `b`
        if (d.name == "bgezal" && insn.rs == 0) insn.flow = Flow::Call;
        return insn;
    }
    return std::nullopt;
}

std::optional<Insn> decode(ByteView code, std::size_t at, std::uint32_t pc) {
    if (at + 4 > code.size()) return std::nullopt;
    const std::uint32_t word = static_cast<std::uint32_t>(code[at]) | static_cast<std::uint32_t>(code[at + 1]) << 8 |
                               static_cast<std::uint32_t>(code[at + 2]) << 16 | static_cast<std::uint32_t>(code[at + 3]) << 24;
    return decode_word(word, pc);
}

// ---- text -------------------------------------------------------------------------------------------------------

namespace {

std::string address_text(std::uint32_t address, const AddressLabels& labels) {
    if (const auto it = labels.find(address); it != labels.end() && !it->second.empty()) return it->second;
    return std::format("0x{:08X}", address);
}

std::string signed_text(std::int32_t v) { return std::to_string(v); }

std::string cop_register(unsigned n) { return "$" + std::to_string(n); }

// The command's fields as the assembler reads them: sf and lm (MVMVA also mx, v, cv), and the ignored bits when they are not the SDK's.
std::string command_text(const Insn& insn) {
    const std::uint32_t command = static_cast<std::uint32_t>(insn.imm);
    const GteDef* g = gte_by_function(command & kFunctionBits);
    const bool mvmva = insn.mnemonic == "mvmva";
    const std::uint32_t mask = ignored_mask(mvmva);
    const std::uint32_t ignored = command & mask;
    const std::uint32_t sf = (command >> 19) & 1, lm = (command >> 10) & 1;
    if (mvmva) {
        std::string out = std::format("mvmva {}, {}, {}, {}, {}", sf, (command >> 17) & 3, (command >> 15) & 3, (command >> 13) & 3, lm);
        if (ignored != (g->usual & mask)) out += std::format(", 0x{:X}", ignored);
        return out;
    }
    if (command == g->usual) return insn.mnemonic;
    if (insn.mnemonic == "nclip") return std::format("nclip 0, 0, 0x{:X}", ignored);   // (sf and lm are zero, so only the ignored bits differ)
    std::string out = std::format("{} {}, {}", insn.mnemonic, sf, lm);
    if (ignored != (g->usual & mask)) out += std::format(", 0x{:X}", ignored);
    return out;
}

}  // namespace

std::string text(const Insn& insn, const AddressLabels& labels, std::string_view immediate) {
    const std::string& m = insn.mnemonic;
    const auto reg = [](unsigned n) { return std::string(register_name(n)); };
    const auto imm_text = [&](std::string_view fallback) { return immediate.empty() ? std::string(fallback) : std::string(immediate); };
    switch (insn.form) {
        case Form::None:
            if (m == "nop" || m == "rfe") return m;
            return insn.imm == 0 ? m : std::format("{} 0x{:X}", m, insn.imm);
        case Form::RdRsRt:
            if (m == "or" && insn.rt == 0) return std::format("move {}, {}", reg(insn.rd), reg(insn.rs));
            return std::format("{} {}, {}, {}", m, reg(insn.rd), reg(insn.rs), reg(insn.rt));
        case Form::RdRtSa: return std::format("{} {}, {}, {}", m, reg(insn.rd), reg(insn.rt), insn.shamt);
        case Form::RdRtRs: return std::format("{} {}, {}, {}", m, reg(insn.rd), reg(insn.rt), reg(insn.rs));
        case Form::Rs: return std::format("{} {}", m, reg(insn.rs));
        case Form::Rd: return std::format("{} {}", m, reg(insn.rd));
        case Form::RdRs:
            if (insn.rd == 31) return std::format("{} {}", m, reg(insn.rs));
            return std::format("{} {}, {}", m, reg(insn.rd), reg(insn.rs));
        case Form::RsRt: return std::format("{} {}, {}", m, reg(insn.rs), reg(insn.rt));
        case Form::RtRsImm: {
            const std::string value = unsigned_immediate(m) ? std::format("0x{:X}", insn.imm) : signed_text(insn.imm);
            return std::format("{} {}, {}, {}", m, reg(insn.rt), reg(insn.rs), imm_text(value));
        }
        case Form::RtImm: return std::format("{} {}, {}", m, reg(insn.rt), imm_text(std::format("0x{:X}", insn.imm)));
        case Form::RsRtBranch: {
            const std::string to = address_text(*insn.target, labels);
            if (m == "beq" && insn.rs == 0 && insn.rt == 0) return "b " + to;
            if (insn.rt == 0) return std::format("{} {}, {}", m == "beq" ? "beqz" : "bnez", reg(insn.rs), to);
            return std::format("{} {}, {}, {}", m, reg(insn.rs), reg(insn.rt), to);
        }
        case Form::RsBranch: return std::format("{} {}, {}", m, reg(insn.rs), address_text(*insn.target, labels));
        case Form::Jump: return std::format("{} {}", m, address_text(*insn.target, labels));
        case Form::RtMem: {
            const std::string rt = coprocessor_register(m) ? cop_register(insn.rt) : reg(insn.rt);
            return std::format("{} {}, {}({})", m, rt, imm_text(signed_text(insn.imm)), reg(insn.rs));
        }
        case Form::RtCop: return std::format("{} {}, {}", m, reg(insn.rt), cop_register(insn.rd));
        case Form::CopBranch: return std::format("{} {}", m, address_text(*insn.target, labels));
        case Form::Command: return command_text(insn);
        case Form::Raw: return std::format("cop2 0x{:X}", insn.imm);
    }
    return m;
}

std::string text(const Insn& insn, const AddressLabels& labels) { return text(insn, labels, std::string_view()); }

// ---- expressions ------------------------------------------------------------------------------------------------

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
    // an operator that is not the start of a longer one
    bool eat_op(char c, char not_followed) {
        skip();
        if (i_ < s_.size() && s_[i_] == c && !(i_ + 1 < s_.size() && s_[i_ + 1] == not_followed)) {
            ++i_;
            return true;
        }
        return false;
    }

    std::int64_t bit_or() {
        std::int64_t v = bit_xor();
        while (eat_op('|', '\0')) v |= bit_xor();
        return v;
    }
    std::int64_t bit_xor() {
        std::int64_t v = bit_and();
        while (eat_op('^', '\0')) v ^= bit_and();
        return v;
    }
    std::int64_t bit_and() {
        std::int64_t v = shift();
        while (eat_op('&', '\0')) v &= shift();
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
            if (eat_op('+', '\0')) v += product();
            else if (eat_op('-', '\0')) v -= product();
            else return v;
        }
    }
    std::int64_t product() {
        std::int64_t v = unary();
        for (;;) {
            if (eat_op('*', '\0')) {
                v *= unary();
            } else if (eat_op('/', '\0')) {
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
    std::int64_t primary() {
        skip();
        if (i_ >= s_.size()) fail("an operand is missing");
        if (eat("%hi(")) return ((close(bit_or()) + 0x8000) >> 16) & 0xFFFF;
        if (eat("%lo(")) return close(bit_or()) & 0xFFFF;
        if (eat("(")) return close(bit_or());
        const char c = s_[i_];
        if (std::isdigit(static_cast<unsigned char>(c))) return number();
        if (c == '.' && (i_ + 1 >= s_.size() || !(std::isalnum(static_cast<unsigned char>(s_[i_ + 1])) || s_[i_ + 1] == '_'))) {
            ++i_;
            return pc_;   // `.` is here
        }
        if (std::isalpha(static_cast<unsigned char>(c)) || c == '_' || c == '.' || c == '@') {
            const std::size_t from = i_;
            while (i_ < s_.size() && (std::isalnum(static_cast<unsigned char>(s_[i_])) || s_[i_] == '_' || s_[i_] == '.' || s_[i_] == '@')) ++i_;
            const std::string_view name = s_.substr(from, i_ - from);
            const auto it = labels_.find(name);
            if (it == labels_.end()) throw z80::UnknownLabel(std::string(name));
            return it->second;
        }
        fail(std::string("unexpected '") + c + "'");
    }
    std::int64_t close(std::int64_t v) {
        if (!eat(")")) fail("a ')' is missing");
        return v;
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

[[noreturn]] void refuse(std::string_view line, const std::string& why) { throw ParseError("", 0, 0, std::string(line) + " (" + why + ")"); }

void put_word(Bytes& out, std::uint32_t word) {
    for (unsigned i = 0; i < 4; ++i) out.push_back(static_cast<std::uint8_t>(word >> (8 * i)));
}

unsigned need_register(std::string_view line, std::string_view operand) {
    const auto r = register_number(operand);
    if (!r) refuse(line, "'" + std::string(operand) + "' is not a register");
    return *r;
}

// The field bits of a command word from operands.
struct Operands {
    std::string_view line;
    std::vector<std::string> list;
    const NameLabels& labels;
    std::uint32_t pc;

    void count(std::size_t n) const {
        if (list.size() != n) refuse(line, std::format("{} operands where {} are written", list.size(), n));
    }
    std::int64_t number(std::size_t i) const { return value(list[i], labels, pc); }
    unsigned reg(std::size_t i) const { return need_register(line, list[i]); }
};

// `imm($reg)` or `($reg)`: the offset expression and the base register.
bool split_memory(std::string_view operand, std::string_view& offset, std::string_view& base) {
    operand = text::trim(operand);
    if (operand.empty() || operand.back() != ')') return false;
    int depth = 0;
    for (std::size_t i = operand.size(); i-- > 0;) {
        if (operand[i] == ')') ++depth;
        if (operand[i] == '(' && --depth == 0) {
            base = text::trim(operand.substr(i + 1, operand.size() - i - 2));
            offset = text::trim(operand.substr(0, i));
            return register_number(base).has_value();
        }
    }
    return false;
}

std::uint32_t field(std::string_view line, std::int64_t v, std::int64_t lo, std::int64_t hi, std::string_view what) {
    if (v < lo || v > hi) refuse(line, std::format("{} {} does not fit", what, v));
    return static_cast<std::uint32_t>(v);
}

std::uint32_t branch_offset(std::string_view line, std::int64_t target, std::uint32_t pc) {
    const std::int64_t distance = target - (static_cast<std::int64_t>(pc) + 4);
    if (distance % 4 != 0) refuse(line, "a branch target is not a word");
    if (distance / 4 < -32768 || distance / 4 > 32767) refuse(line, "the branch is out of reach");
    return static_cast<std::uint32_t>(distance / 4) & 0xFFFF;
}

}  // namespace

Bytes assemble(std::string_view line, std::uint32_t pc, const NameLabels& labels) {
    line = text::trim(line);
    std::size_t split = 0;
    while (split < line.size() && !text::is_space(line[split])) ++split;
    const std::string name = text::lower(line.substr(0, split));
    Operands ops{line, z80::split_operands(line.substr(split)), labels, pc};
    Bytes out;

    // ---- pseudo-instructions
    if (name == "nop") {
        ops.count(0);
        put_word(out, 0);
        return out;
    }
    if (name == "move") {
        ops.count(2);
        put_word(out, (ops.reg(0) << 11) | (ops.reg(1) << 21) | 0x25);   // or rd, rs, $zero (as GNU as writes it)
        return out;
    }
    if (name == "b") {
        ops.count(1);
        put_word(out, op(4) | branch_offset(line, ops.number(0), pc));
        return out;
    }
    if (name == "beqz" || name == "bnez") {
        ops.count(2);
        put_word(out, op(name == "beqz" ? 4 : 5) | (ops.reg(0) << 21) | branch_offset(line, ops.number(1), pc));
        return out;
    }
    if (name == "li") {
        ops.count(2);
        const std::int64_t v = ops.number(1);
        const unsigned rt = ops.reg(0);
        if (v >= -32768 && v <= 32767) {
            put_word(out, op(9) | (rt << 16) | (static_cast<std::uint32_t>(v) & 0xFFFF));            // addiu rt, $zero, v
        } else if (v >= 0 && v <= 0xFFFF) {
            put_word(out, op(13) | (rt << 16) | static_cast<std::uint32_t>(v));                       // ori rt, $zero, v
        } else if (v >= -0x80000000LL && v <= 0xFFFFFFFFLL) {
            const std::uint32_t u = static_cast<std::uint32_t>(v);
            put_word(out, op(15) | (rt << 16) | (u >> 16));                                           // lui rt, hi
            if ((u & 0xFFFF) != 0) put_word(out, op(13) | (rt << 21) | (rt << 16) | (u & 0xFFFF));    // ori rt, rt, lo
        } else {
            refuse(line, "the value does not fit in 32 bits");
        }
        return out;
    }
    if (name == "la") {
        ops.count(2);
        const std::uint32_t u = static_cast<std::uint32_t>(ops.number(1));
        const unsigned rt = ops.reg(0);
        put_word(out, op(15) | (rt << 16) | (((u + 0x8000) >> 16) & 0xFFFF));                         // lui rt, %hi
        put_word(out, op(9) | (rt << 21) | (rt << 16) | (u & 0xFFFF));                                  // addiu rt, rt, %lo
        return out;
    }

    // ---- GTE commands
    if (const GteDef* g = gte_by_name(name)) {
        std::uint32_t command = g->usual;
        if (!ops.list.empty()) {
            const bool mvmva = name == "mvmva";
            const std::size_t fields = mvmva ? 5 : 2;
            if (ops.list.size() != fields && ops.list.size() != fields + 1)
                refuse(line, std::format("{} takes {} fields (and the ignored bits)", name, fields));
            command = g->function;
            const std::uint32_t sf = field(line, ops.number(0), 0, 1, "sf");
            std::uint32_t lm = 0;
            if (mvmva) {
                command |= field(line, ops.number(1), 0, 3, "mx") << 17;
                command |= field(line, ops.number(2), 0, 3, "v") << 15;
                command |= field(line, ops.number(3), 0, 3, "cv") << 13;
                lm = field(line, ops.number(4), 0, 1, "lm");
            } else {
                lm = field(line, ops.number(1), 0, 1, "lm");
            }
            command |= (sf << 19) | (lm << 10);
            if (ops.list.size() == fields + 1) {
                const std::uint32_t ignored = field(line, ops.number(fields), 0, 0x1FFFFFF, "the ignored bits");
                if ((ignored & ~ignored_mask(mvmva)) != 0) refuse(line, "those bits are fields");
                command |= ignored;
            } else {
                command |= g->usual & ignored_mask(mvmva);
            }
        }
        put_word(out, op(0x12) | (1u << 25) | command);
        return out;
    }
    if (name == "cop2") {
        ops.count(1);
        put_word(out, op(0x12) | (1u << 25) | field(line, ops.number(0), 0, 0x1FFFFFF, "the command"));
        return out;
    }

    // ---- the table
    const Def* def = nullptr;
    for (const Def& d : kDefs) {
        if (d.name == name) def = &d;
    }
    if (!def) refuse(line, "not an instruction");
    std::uint32_t word = def->match;
    switch (def->form) {
        case Form::None:
            if (def->name == "rfe") {
                ops.count(0);
            } else {
                if (ops.list.size() > 1) refuse(line, "too many operands");
                if (!ops.list.empty()) word |= field(line, ops.number(0), 0, 0xFFFFF, "the code") << 6;
            }
            break;
        case Form::RdRsRt: ops.count(3); word |= (ops.reg(0) << 11) | (ops.reg(1) << 21) | (ops.reg(2) << 16); break;
        case Form::RdRtSa: ops.count(3); word |= (ops.reg(0) << 11) | (ops.reg(1) << 16) | (field(line, ops.number(2), 0, 31, "the shift") << 6); break;
        case Form::RdRtRs: ops.count(3); word |= (ops.reg(0) << 11) | (ops.reg(1) << 16) | (ops.reg(2) << 21); break;
        case Form::Rs: ops.count(1); word |= ops.reg(0) << 21; break;
        case Form::Rd: ops.count(1); word |= ops.reg(0) << 11; break;
        case Form::RdRs:
            if (ops.list.size() == 1) {
                word |= (31u << 11) | (ops.reg(0) << 21);
            } else {
                ops.count(2);
                word |= (ops.reg(0) << 11) | (ops.reg(1) << 21);
            }
            break;
        case Form::RsRt: ops.count(2); word |= (ops.reg(0) << 21) | (ops.reg(1) << 16); break;
        case Form::RtRsImm:
            ops.count(3);
            word |= (ops.reg(0) << 16) | (ops.reg(1) << 21) | (field(line, ops.number(2), -32768, 65535, "the immediate") & 0xFFFF);
            break;
        case Form::RtImm:
            ops.count(2);
            word |= (ops.reg(0) << 16) | (field(line, ops.number(1), -32768, 65535, "the immediate") & 0xFFFF);
            break;
        case Form::RsRtBranch: ops.count(3); word |= (ops.reg(0) << 21) | (ops.reg(1) << 16) | branch_offset(line, ops.number(2), pc); break;
        case Form::RsBranch: ops.count(2); word |= (ops.reg(0) << 21) | branch_offset(line, ops.number(1), pc); break;
        case Form::CopBranch: ops.count(1); word |= branch_offset(line, ops.number(0), pc); break;
        case Form::Jump: {
            ops.count(1);
            const std::int64_t to = ops.number(0);
            if (to % 4 != 0) refuse(line, "a jump target is not a word");
            if (((to >> 28) & 0xF) != ((static_cast<std::int64_t>(pc) + 4) >> 28 & 0xF)) refuse(line, "the jump is into another 256 MB region");
            word |= static_cast<std::uint32_t>(to >> 2) & 0x3FFFFFF;
            break;
        }
        case Form::RtMem: {
            ops.count(2);
            std::string_view offset, base;
            if (!split_memory(ops.list[1], offset, base)) refuse(line, "a memory operand is offset($register)");
            const unsigned rt = coprocessor_register(def->name) ? field(line, register_number(ops.list[0]).value_or(99), 0, 31, "the register") : ops.reg(0);
            const std::int64_t off = offset.empty() ? 0 : value(offset, labels, pc);
            word |= (rt << 16) | (need_register(line, base) << 21) | (field(line, off, -32768, 65535, "the offset") & 0xFFFF);
            break;
        }
        case Form::RtCop: ops.count(2); word |= (ops.reg(0) << 16) | (ops.reg(1) << 11); break;
        case Form::Command:
        case Form::Raw: break;
    }
    put_word(out, word);
    return out;
}

}  // namespace cb::decompile::mips
