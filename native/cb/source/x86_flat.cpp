#include "cb/source/x86_flat.hpp"

#include <algorithm>
#include <array>
#include <cstring>
#include <format>

#include "cb/core/capstone_api.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"

namespace cb::source::x86_flat {
namespace {

constexpr std::array<std::string_view, 12> kArith = {"add", "or", "adc", "sbb", "and", "sub", "xor", "cmp", "push", "imul", "mov", "test"};
constexpr std::array<RegisterPairOp, 9> kPairOps = {{{"adc", 0x10}, {"add", 0x00}, {"and", 0x20}, {"cmp", 0x38}, {"mov", 0x88},
                                                     {"or", 0x08}, {"sbb", 0x18}, {"sub", 0x28}, {"xor", 0x30}}};
constexpr std::array<std::string_view, 24> kRegisters = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi", "ax", "cx", "dx", "bx",
                                                         "sp",  "bp",  "si",  "di",  "al",  "cl",  "dl",  "bl",  "ah", "ch", "dh", "bh"};

bool hex_digit(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'); }
bool decimal_digit(char c) { return c >= '0' && c <= '9'; }

}  // namespace

std::optional<Insn> decode(const platforms::Decoder& decoder, ByteView code, std::uint32_t address) {
    std::optional<Insn> out;
    decoder.each(code, address, 1, [&](const cs_insn& c) {
        Insn i;
        i.address = static_cast<std::uint32_t>(c.address);
        i.size = static_cast<std::uint8_t>(c.size);
        i.mnemonic = c.mnemonic;
        i.op_str = c.op_str;
        std::copy_n(c.bytes, std::min<std::size_t>(c.size, i.bytes.size()), i.bytes.begin());
        if (c.detail != nullptr) {
            const cs_x86& x = c.detail->x86;
            i.imm_size = x.encoding.imm_size;
            i.imm_offset = x.encoding.imm_offset;
            i.disp_size = x.encoding.disp_size;
            i.disp_offset = x.encoding.disp_offset;
            std::copy_n(x.prefix, i.prefix.size(), i.prefix.begin());
        }
        out = std::move(i);
        return false;
    });
    return out;
}

bool is_branch(std::string_view m) {
    // j<cc>, jmp, jcxz, jecxz; call; loop, loope, loopne (a mnemonic is all word characters)
    return m == "call" || m.starts_with("loop") || (m.size() >= 2 && m.starts_with('j'));
}

std::optional<std::uint32_t> branch_target(const Insn& i) {
    if (!is_branch(i.mnemonic) || i.imm_size == 0) return std::nullopt;
    const std::string_view s = i.op_str;
    if (s.size() <= 2 || !s.starts_with("0x") || !std::ranges::all_of(s.substr(2), hex_digit)) return std::nullopt;
    return text::parse_int<std::uint32_t>(s.substr(2), 16);   // (nothing for a number that is not 32 bits)
}

bool is_string_op(std::string_view m, bool quad_words) {
    for (const std::string_view prefix : {"rep ", "repe ", "repne "}) {
        if (m.starts_with(prefix)) {
            m.remove_prefix(prefix.size());
            break;
        }
    }
    for (const std::string_view op : {"movs", "cmps", "stos", "lods", "scas", "ins", "outs"}) {
        if (!m.starts_with(op) || m.size() != op.size() + 1) continue;
        const char width = m.back();
        return width == 'b' || width == 'w' || width == 'd' || (quad_words && width == 'q');
    }
    return false;
}

bool is_segment_prefix(std::uint8_t b) { return b == 0x26 || b == 0x2E || b == 0x36 || b == 0x3E || b == 0x64 || b == 0x65; }

bool is_arith(std::string_view m) { return std::ranges::find(kArith, m) != kArith.end(); }

bool is_register(std::string_view name) { return std::ranges::find(kRegisters, name) != kRegisters.end(); }

std::span<const RegisterPairOp> register_pair_ops() { return kPairOps; }

bool has_register_pair_form(std::string_view m) {
    return std::ranges::any_of(kPairOps, [&](const RegisterPairOp& op) { return op.mnemonic == m; });
}

std::optional<std::string> register_pair_text(const Insn& i) {
    if (!has_register_pair_form(i.mnemonic)) return std::nullopt;
    const std::size_t comma = i.op_str.find(", ");
    if (comma == std::string::npos) return std::nullopt;
    const std::string_view first = std::string_view(i.op_str).substr(0, comma), second = std::string_view(i.op_str).substr(comma + 2);
    if (!is_register(first) || !is_register(second)) return std::nullopt;
    const std::uint8_t opcode = i.bytes[i.bytes[0] == 0x66 ? 1 : 0];
    return std::format("{}{} {}", i.mnemonic, (opcode & 2) ? "_" : "", i.op_str);
}

std::string nasm_operands(std::string_view operands) {
    static const Regex st_reg(R"(\bst\((\d)\))");
    static const Regex segment_bracket(R"(\b([cdefgs]s):\[)");
    std::string ops = text::replace_all(operands, " ptr ", " ");
    ops = text::replace_all(ops, "xmmword ", "oword ");
    ops = text::replace_all(ops, "xword ", "tword ");
    ops = st_reg.replace_all(ops, "st$1");
    return segment_bracket.replace_all(ops, "[$1:");
}

std::optional<TrailingNumber> trailing_number(std::string_view t) {
    // -?0x[0-9a-f]+ first (where a match begins earlier than the decimal one), then -?\d+
    std::size_t j = t.size();
    while (j > 0 && hex_digit(t[j - 1])) --j;
    std::size_t start = t.size();
    bool hex = false;
    if (j < t.size() && j >= 2 && t[j - 1] == 'x' && t[j - 2] == '0') {
        start = j - 2;
        hex = true;
    } else {
        std::size_t d = t.size();
        while (d > 0 && decimal_digit(t[d - 1])) --d;
        if (d == t.size()) return std::nullopt;
        start = d;
    }
    const bool negative = start > 0 && t[start - 1] == '-';
    const std::string_view digits = hex ? t.substr(start + 2) : t.substr(start);
    const auto value = text::parse_int<std::int64_t>(digits, hex ? 16 : 10);
    if (!value) return std::nullopt;
    return TrailingNumber{negative ? start - 1 : start, negative ? -*value : *value};
}

std::string db_bytes(ByteView b, std::size_t lo, std::size_t hi) {
    std::string out;
    for (std::size_t k = lo; k < hi && k < b.size(); ++k) {
        if (k > lo) out += ',';
        out += std::format("0x{:02x}", b[k]);
    }
    return out;
}

std::string with_original(std::string line, const Insn& i) {
    const std::size_t pad = line.size() < 40 ? 40 - line.size() : 1;
    return line + std::string(pad, ' ') + "; " + i.mnemonic + " " + i.op_str;
}

}  // namespace cb::source::x86_flat
