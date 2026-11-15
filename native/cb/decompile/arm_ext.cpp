// cb-unity: alone
// cb/decompile/arm_ext.cpp -- ARMv6 and VFP instructions as rows read both ways (arm_ext.hpp).

#include "cb/decompile/arm_ext.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <format>
#include <string>
#include <unordered_map>
#include <vector>

#include "cb/core/error.hpp"
#include "cb/core/text.hpp"

namespace cb::decompile::arm::ext {

namespace {

// One instruction: the bits it fixes (`mask`, `value`), its name, and its operands as tokens separated by commas:
//   r<lsb>          a register (4 bits)          i<lsb>:<width>[+bias]   an unsigned immediate, written `#n`
//   m<lsb>          `[rN]`                       n<lsb>:<bit>            a register written `rN!` when the bit is set; N<lsb> always `rN!`
//   R<lsb>          an optional `, ror #8n` (2 bits at lsb)       L<lsb> optional `, lsl #n` (5 bits at 7)     A<lsb> `, asr #n` (0 is 32)
//   Q<lsb>          optional `, lsl #n` or `, asr #n` (the type at bit 6, 5 bits at 7)
//   s<lsb>:<bit>    a single register: the 4 bits at lsb, then the bit as its low bit      d<lsb>:<bit> a double: the bit as its high bit
//   v<lsb>          `[rN, #+-imm8*4]` of VLDR / VSTR (U at bit 23)       z  `#0.0`
//   W<lsb>:<bit>    `{s0-s3}` from the register at lsb and the count in bits 7:0          X<lsb>:<bit>  `{d8-d15}` (the count halved)
//   =text           the text as it is
struct Row {
    std::string name;
    std::uint32_t mask = 0, value = 0;
    std::string ops;
    Arch min = Arch::V6K;
    Sem sem = Sem::None;
    bool uncond = false;
};

constexpr std::uint32_t bits(std::uint32_t w, unsigned lsb, unsigned width) { return (w >> lsb) & ((1u << width) - 1u); }

void build_rows(std::vector<Row>& t) {
    const auto add = [&](std::string name, std::uint32_t mask, std::uint32_t value, std::string ops, Sem sem = Sem::None, Arch min = Arch::V6K, bool uncond = false) {
        t.push_back({std::move(name), mask, value, std::move(ops), min, sem, uncond});
    };
    // byte reversal, extension
    add("rev", 0x0FFF0FF0, 0x06BF0F30, "r12,r0", Sem::Rev);
    add("rev16", 0x0FFF0FF0, 0x06BF0FB0, "r12,r0", Sem::Rev16);
    add("revsh", 0x0FFF0FF0, 0x06FF0FB0, "r12,r0", Sem::Revsh);
    add("rbit", 0x0FFF0FF0, 0x06FF0F30, "r12,r0", Sem::None, Arch::V7A);
    add("sxtb16", 0x0FFF03F0, 0x068F0070, "r12,r0,R10");
    add("sxtb", 0x0FFF03F0, 0x06AF0070, "r12,r0,R10", Sem::Sxtb);
    add("sxth", 0x0FFF03F0, 0x06BF0070, "r12,r0,R10", Sem::Sxth);
    add("uxtb16", 0x0FFF03F0, 0x06CF0070, "r12,r0,R10");
    add("uxtb", 0x0FFF03F0, 0x06EF0070, "r12,r0,R10", Sem::Uxtb);
    add("uxth", 0x0FFF03F0, 0x06FF0070, "r12,r0,R10", Sem::Uxth);
    add("sxtab16", 0x0FF003F0, 0x06800070, "r12,r16,r0,R10");
    add("sxtab", 0x0FF003F0, 0x06A00070, "r12,r16,r0,R10", Sem::Sxtab);
    add("sxtah", 0x0FF003F0, 0x06B00070, "r12,r16,r0,R10", Sem::Sxtah);
    add("uxtab16", 0x0FF003F0, 0x06C00070, "r12,r16,r0,R10");
    add("uxtab", 0x0FF003F0, 0x06E00070, "r12,r16,r0,R10", Sem::Uxtab);
    add("uxtah", 0x0FF003F0, 0x06F00070, "r12,r16,r0,R10", Sem::Uxtah);
    // exclusive access
    add("ldrex", 0x0FF00FFF, 0x01900F9F, "r12,m16", Sem::Ldrex);
    add("ldrexd", 0x0FF00FFF, 0x01B00F9F, "r12,m16");
    add("ldrexb", 0x0FF00FFF, 0x01D00F9F, "r12,m16", Sem::Ldrexb);
    add("ldrexh", 0x0FF00FFF, 0x01F00F9F, "r12,m16", Sem::Ldrexh);
    add("strex", 0x0FF00FF0, 0x01800F90, "r12,r0,m16", Sem::Strex);
    add("strexd", 0x0FF00FF0, 0x01A00F90, "r12,r0,m16");
    add("strexb", 0x0FF00FF0, 0x01C00F90, "r12,r0,m16", Sem::Strexb);
    add("strexh", 0x0FF00FF0, 0x01E00F90, "r12,r0,m16", Sem::Strexh);
    add("clrex", 0xFFFFFFFF, 0xF57FF01F, "", Sem::Clrex, Arch::V6K, true);
    // multiplies
    add("umaal", 0x0FF000F0, 0x00400090, "r12,r16,r0,r8", Sem::Umaal);
    add("smmul", 0x0FF0F0F0, 0x0750F010, "r16,r0,r8", Sem::Smmul);
    add("smmulr", 0x0FF0F0F0, 0x0750F030, "r16,r0,r8");
    add("smmla", 0x0FF000F0, 0x07500010, "r16,r0,r8,r12", Sem::Smmla);
    add("smmlar", 0x0FF000F0, 0x07500030, "r16,r0,r8,r12");
    add("smmls", 0x0FF000F0, 0x075000D0, "r16,r0,r8,r12", Sem::Smmls);
    add("smmlsr", 0x0FF000F0, 0x075000F0, "r16,r0,r8,r12");
    add("smuad", 0x0FF0F0F0, 0x0700F010, "r16,r0,r8", Sem::Smuad);
    add("smuadx", 0x0FF0F0F0, 0x0700F030, "r16,r0,r8");
    add("smusd", 0x0FF0F0F0, 0x0700F050, "r16,r0,r8", Sem::Smusd);
    add("smusdx", 0x0FF0F0F0, 0x0700F070, "r16,r0,r8");
    add("smlad", 0x0FF000F0, 0x07000010, "r16,r0,r8,r12", Sem::Smlad);
    add("smladx", 0x0FF000F0, 0x07000030, "r16,r0,r8,r12");
    add("smlsd", 0x0FF000F0, 0x07000050, "r16,r0,r8,r12", Sem::Smlsd);
    add("smlsdx", 0x0FF000F0, 0x07000070, "r16,r0,r8,r12");
    add("smlald", 0x0FF000F0, 0x07400010, "r12,r16,r0,r8");
    add("smlaldx", 0x0FF000F0, 0x07400030, "r12,r16,r0,r8");
    add("smlsld", 0x0FF000F0, 0x07400050, "r12,r16,r0,r8");
    add("smlsldx", 0x0FF000F0, 0x07400070, "r12,r16,r0,r8");
    add("usad8", 0x0FF0F0F0, 0x0780F010, "r16,r0,r8", Sem::Usad8);
    add("usada8", 0x0FF000F0, 0x07800010, "r16,r0,r8,r12", Sem::Usada8);
    add("sel", 0x0FF00FF0, 0x06800FB0, "r12,r16,r0", Sem::Sel);
    // packing and saturation
    add("pkhbt", 0x0FF00070, 0x06800010, "r12,r16,r0,L7", Sem::Pkhbt);
    add("pkhtb", 0x0FF00070, 0x06800050, "r12,r16,r0,A7", Sem::Pkhtb);
    add("ssat16", 0x0FF00FF0, 0x06A00F30, "r12,i16:4+1,r0");
    add("usat16", 0x0FF00FF0, 0x06E00F30, "r12,i16:4,r0");
    add("ssat", 0x0FE00030, 0x06A00010, "r12,i16:5+1,r0,Q7", Sem::Ssat);
    add("usat", 0x0FE00030, 0x06E00010, "r12,i16:5,r0,Q7", Sem::Usat);
    // the parallel add and subtract families
    static const char* const prefixes[6] = {"s", "q", "sh", "u", "uq", "uh"};
    static const std::uint32_t prefix_bits[6] = {1, 2, 3, 5, 6, 7};
    static const char* const operations[6] = {"add16", "asx", "sax", "sub16", "add8", "sub8"};
    static const std::uint32_t operation_bits[6] = {1, 3, 5, 7, 9, 15};
    for (unsigned p = 0; p < 6; ++p)
        for (unsigned o = 0; o < 6; ++o)
            add(std::string(prefixes[p]) + operations[o], 0x0FF00FF0, 0x06000F00 | prefix_bits[p] << 20 | operation_bits[o] << 4, "r12,r16,r0");
    // the hints that come with ARMv6K
    add("yield", 0x0FFFFFFF, 0x0320F001, "", Sem::Hint);
    add("wfe", 0x0FFFFFFF, 0x0320F002, "", Sem::Hint);
    add("wfi", 0x0FFFFFFF, 0x0320F003, "", Sem::Hint);
    add("sev", 0x0FFFFFFF, 0x0320F004, "", Sem::Hint);
    // the VFP: data processing (single, then double)
    for (unsigned dbl = 0; dbl < 2; ++dbl) {
        const std::string ty = dbl ? ".f64" : ".f32";
        const std::uint32_t z = dbl ? 0x100 : 0;
        const std::string r3 = dbl ? "d12:22,d16:7,d0:5" : "s12:22,s16:7,s0:5";
        const std::string r2 = dbl ? "d12:22,d0:5" : "s12:22,s0:5";
        const std::uint32_t m = 0x0FB00F50;
        add("vmla" + ty, m, 0x0E000A00 | z, r3);
        add("vmls" + ty, m, 0x0E000A40 | z, r3);
        add("vnmls" + ty, m, 0x0E100A00 | z, r3);
        add("vnmla" + ty, m, 0x0E100A40 | z, r3);
        add("vmul" + ty, m, 0x0E200A00 | z, r3);
        add("vnmul" + ty, m, 0x0E200A40 | z, r3);
        add("vadd" + ty, m, 0x0E300A00 | z, r3);
        add("vsub" + ty, m, 0x0E300A40 | z, r3);
        add("vdiv" + ty, m, 0x0E800A00 | z, r3);
        const std::uint32_t e = 0x0FBF0FD0;
        add("vmov" + ty, e, 0x0EB00A40 | z, r2);
        add("vabs" + ty, e, 0x0EB00AC0 | z, r2);
        add("vneg" + ty, e, 0x0EB10A40 | z, r2);
        add("vsqrt" + ty, e, 0x0EB10AC0 | z, r2);
        add("vcmp" + ty, e, 0x0EB40A40 | z, r2);
        add("vcmpe" + ty, e, 0x0EB40AC0 | z, r2);
        add("vcmp" + ty, 0x0FBF0FFF, 0x0EB50A40 | z, std::string(dbl ? "d12:22" : "s12:22") + ",z");
        add("vcmpe" + ty, 0x0FBF0FFF, 0x0EB50AC0 | z, std::string(dbl ? "d12:22" : "s12:22") + ",z");
        const std::string from_int = dbl ? "d12:22,s0:5" : "s12:22,s0:5";
        const std::string to_int = dbl ? "s12:22,d0:5" : "s12:22,s0:5";
        add("vcvt" + ty + ".u32", e, 0x0EB80A40 | z, from_int);
        add("vcvt" + ty + ".s32", e, 0x0EB80AC0 | z, from_int);
        add("vcvt.u32" + ty, e, 0x0EBC0AC0 | z, to_int);
        add("vcvt.s32" + ty, e, 0x0EBD0AC0 | z, to_int);
        add("vcvtr.u32" + ty, e, 0x0EBC0A40 | z, to_int);
        add("vcvtr.s32" + ty, e, 0x0EBD0A40 | z, to_int);
        add(dbl ? "vldr" : "vldr", 0x0F300F00, 0x0D100A00 | z, std::string(dbl ? "d12:22" : "s12:22") + ",v16");
        add("vstr", 0x0F300F00, 0x0D000A00 | z, std::string(dbl ? "d12:22" : "s12:22") + ",v16");
    }
    add("vcvt.f64.f32", 0x0FBF0FD0, 0x0EB70AC0, "d12:22,s0:5");
    add("vcvt.f32.f64", 0x0FBF0FD0, 0x0EB70BC0, "s12:22,d0:5");
    // lists
    add("vpush", 0x0FBF0F00, 0x0D2D0A00, "W12:22");
    add("vpush", 0x0FBF0F00, 0x0D2D0B00, "X12:22");
    add("vpop", 0x0FBF0F00, 0x0CBD0A00, "W12:22");
    add("vpop", 0x0FBF0F00, 0x0CBD0B00, "X12:22");
    add("vldmia", 0x0F900F00, 0x0C900A00, "n16:21,W12:22");
    add("vldmia", 0x0F900F00, 0x0C900B00, "n16:21,X12:22");
    add("vstmia", 0x0F900F00, 0x0C800A00, "n16:21,W12:22");
    add("vstmia", 0x0F900F00, 0x0C800B00, "n16:21,X12:22");
    add("vldmdb", 0x0FB00F00, 0x0D300A00, "N16,W12:22");
    add("vldmdb", 0x0FB00F00, 0x0D300B00, "N16,X12:22");
    add("vstmdb", 0x0FB00F00, 0x0D200A00, "N16,W12:22");
    add("vstmdb", 0x0FB00F00, 0x0D200B00, "N16,X12:22");
    // moves between the core and the VFP, and the status register
    add("vmov", 0x0FF00F7F, 0x0E000A10, "s16:7,r12");
    add("vmov", 0x0FF00F7F, 0x0E100A10, "r12,s16:7");
    add("vmov", 0x0FF00FD0, 0x0C400B10, "d0:5,r12,r16");
    add("vmov", 0x0FF00FD0, 0x0C500B10, "r12,r16,d0:5");
    add("vmrs", 0x0FFFFFFF, 0x0EF1FA10, "=APSR_nzcv,=fpscr");
    add("vmrs", 0x0FFF0FFF, 0x0EF10A10, "r12,=fpscr");
    add("vmsr", 0x0FFF0FFF, 0x0EE10A10, "=fpscr,r12");
}

const std::vector<Row>& rows() {
    static const std::vector<Row> table = [] {
        std::vector<Row> t;
        build_rows(t);
        return t;
    }();
    return table;
}

std::string_view base_of(const std::string& name) { return std::string_view(name).substr(0, name.find('.')); }
std::string_view suffix_of(const std::string& name) {
    const std::size_t dot = name.find('.');
    return dot == std::string::npos ? std::string_view() : std::string_view(name).substr(dot);
}

// Rows by the base of their name.
const std::unordered_multimap<std::string, int>& by_base() {
    static const std::unordered_multimap<std::string, int> map = [] {
        std::unordered_multimap<std::string, int> m;
        for (std::size_t k = 0; k < rows().size(); ++k) m.emplace(std::string(base_of(rows()[k].name)), static_cast<int>(k));
        return m;
    }();
    return map;
}

std::optional<unsigned> condition_of(std::string_view s) {
    if (s.empty()) return 14u;
    if (s == "hs") return 2u;
    if (s == "lo") return 3u;
    for (unsigned c = 0; c < 14; ++c)
        if (condition_suffix(c) == s) return c;
    return std::nullopt;
}

struct Token {
    char kind = 0;
    unsigned lsb = 0, second = 0;
    unsigned bias = 0;
    std::string literal;
};

Token parse_token(std::string_view t) {
    Token k;
    k.kind = t[0];
    if (k.kind == '=') {
        k.literal = std::string(t.substr(1));
        return k;
    }
    std::size_t at = 1;
    const auto number = [&] {
        unsigned v = 0;
        while (at < t.size() && std::isdigit(static_cast<unsigned char>(t[at]))) v = v * 10 + static_cast<unsigned>(t[at++] - '0');
        return v;
    };
    if (at < t.size()) k.lsb = number();
    if (at < t.size() && t[at] == ':') {
        ++at;
        k.second = number();
    }
    if (at < t.size() && t[at] == '+') {
        ++at;
        k.bias = number();
    }
    return k;
}

std::vector<Token> tokens_of(const Row& r) {
    std::vector<Token> out;
    std::size_t at = 0;
    while (at < r.ops.size()) {
        std::size_t end = r.ops.find(',', at);
        if (end == std::string::npos) end = r.ops.size();
        out.push_back(parse_token(std::string_view(r.ops).substr(at, end - at)));
        at = end + 1;
    }
    return out;
}

std::string reg(unsigned n) { return std::string(register_name(n)); }

std::string single(unsigned field, unsigned bit) { return "s" + std::to_string(field << 1 | bit); }
std::string dbl(unsigned field, unsigned bit) { return "d" + std::to_string(field | bit << 4); }

// A token's text: a leading ", " marks an optional operand that already carries its separator; an empty text is no operand.
std::string print_token(const Token& k, std::uint32_t w) {
    switch (k.kind) {
        case 'r': return reg(bits(w, k.lsb, 4));
        case 'i': return "#" + std::to_string(bits(w, k.lsb, k.second) + k.bias);
        case 'm': return "[" + reg(bits(w, k.lsb, 4)) + "]";
        case 'n': return reg(bits(w, k.lsb, 4)) + (bits(w, k.second, 1) ? "!" : "");
        case 'N': return reg(bits(w, k.lsb, 4)) + "!";
        case 'R': {
            const unsigned rot = bits(w, k.lsb, 2);
            return rot ? ", ror #" + std::to_string(8 * rot) : "";
        }
        case 'L': {
            const unsigned amount = bits(w, 7, 5);
            return amount ? ", lsl #" + std::to_string(amount) : "";
        }
        case 'A': {
            const unsigned amount = bits(w, 7, 5);
            return ", asr #" + std::to_string(amount ? amount : 32);
        }
        case 'Q': {
            const unsigned amount = bits(w, 7, 5);
            if (bits(w, 6, 1) == 0) return amount ? ", lsl #" + std::to_string(amount) : "";
            return ", asr #" + std::to_string(amount ? amount : 32);
        }
        case 's': return single(bits(w, k.lsb, 4), bits(w, k.second, 1));
        case 'd': return dbl(bits(w, k.lsb, 4), bits(w, k.second, 1));
        case 'v': {
            const unsigned imm = bits(w, 0, 8) * 4;
            const bool up = bits(w, 23, 1) != 0;
            return "[" + reg(bits(w, k.lsb, 4)) + (imm != 0 || !up ? std::string(", #") + (up ? "" : "-") + std::to_string(imm) : std::string()) + "]";
        }
        case 'z': return "#0.0";
        case 'W': case 'X': {
            const unsigned count = bits(w, 0, 8);
            const unsigned first = bits(w, k.lsb, 4);
            const unsigned bit = bits(w, k.second, 1);
            const bool singles = k.kind == 'W';
            const unsigned n = singles ? count : count / 2;
            const std::string a = singles ? single(first, bit) : dbl(first, bit);
            if (n <= 1) return "{" + a + "}";
            const unsigned last_index = (singles ? (first << 1 | bit) : (first | bit << 4)) + n - 1;
            return "{" + a + "-" + (singles ? "s" : "d") + std::to_string(last_index) + "}";
        }
        case '=': return k.literal;
        default: return "?";
    }
}

[[noreturn]] void refuse(std::string_view line, const std::string& why) { throw ParseError("", 0, 0, std::string(line) + " (" + why + ")"); }

// The operands of a line, split at the commas outside brackets and braces.
std::vector<std::string> split(std::string_view s) {
    std::vector<std::string> out;
    int depth = 0;
    std::string cur;
    for (const char c : s) {
        if (c == '[' || c == '{') ++depth;
        if (c == ']' || c == '}') --depth;
        if (c == ',' && depth == 0) {
            out.emplace_back(text::trim(cur));
            cur.clear();
        } else {
            cur += c;
        }
    }
    if (!text::trim(cur).empty() || !out.empty()) out.emplace_back(text::trim(cur));
    return out;
}

unsigned register_of(std::string_view line, std::string_view s) {
    const auto n = register_number(s);
    if (!n) refuse(line, "a register is expected, not '" + std::string(s) + "'");
    return *n;
}

unsigned vfp_of(std::string_view line, std::string_view s, char kind) {
    const std::string t = text::lower(s);
    if (t.size() < 2 || t[0] != kind) refuse(line, std::string("a ") + kind + " register is expected");
    const auto n = text::parse_int<unsigned>(std::string_view(t).substr(1));
    if (!n || *n > 31 || (kind == 's' && *n > 31)) refuse(line, "a VFP register is s0-s31 or d0-d31");
    return *n;
}

void put(std::uint32_t& w, unsigned lsb, unsigned width, std::uint32_t v) { w |= (v & ((1u << width) - 1u)) << lsb; }

// Builds the word from a row and the operands, or throws.
std::uint32_t build_word(const Row& r, std::vector<std::string>& o, std::string_view line, std::uint32_t pc, const NameLabels& labels) {
    std::uint32_t w = 0;
    std::size_t j = 0;
    const auto need = [&]() -> const std::string& {
        if (j >= o.size()) refuse(line, "an operand is missing");
        return o[j++];
    };
    const auto number_after_hash = [&](const std::string& s) {
        std::string_view v = text::trim(s);
        if (v.starts_with('#')) v.remove_prefix(1);
        return value(v, labels, pc);
    };
    for (const Token& k : tokens_of(r)) {
        switch (k.kind) {
            case 'r': put(w, k.lsb, 4, register_of(line, need())); break;
            case 'i': {
                const std::int64_t v = number_after_hash(need()) - static_cast<std::int64_t>(k.bias);
                if (v < 0 || v >= (std::int64_t{1} << k.second)) refuse(line, "an immediate is out of range");
                put(w, k.lsb, k.second, static_cast<std::uint32_t>(v));
                break;
            }
            case 'm': case 'n': case 'N': {
                std::string s = need();
                bool wb = false;
                if (k.kind != 'm' && !s.empty() && s.back() == '!') {
                    wb = true;
                    s.pop_back();
                }
                if (k.kind == 'N' && !wb) refuse(line, "writeback ('!') is expected");
                if (k.kind == 'm') {
                    if (s.size() < 3 || s.front() != '[' || s.back() != ']') refuse(line, "[rN] is expected");
                    s = s.substr(1, s.size() - 2);
                }
                put(w, k.lsb, 4, register_of(line, s));
                if (k.kind == 'n' && wb) put(w, k.second, 1, 1);
                break;
            }
            case 'R': case 'L': case 'A': case 'Q': {
                const bool optional = k.kind != 'A';
                if (j >= o.size()) {
                    if (!optional) refuse(line, "a shift is expected");
                    break;
                }
                const std::string t = text::lower(o[j]);
                const std::string_view kind = t.size() >= 3 ? std::string_view(t).substr(0, 3) : std::string_view();
                const bool is_ror = kind == "ror", is_lsl = kind == "lsl", is_asr = kind == "asr";
                if (k.kind == 'R' && !is_ror) { if (j < o.size()) refuse(line, "ror is expected"); break; }
                if (k.kind == 'L' && !is_lsl) refuse(line, "lsl is expected");
                if (k.kind == 'A' && !is_asr) refuse(line, "asr is expected");
                if (k.kind == 'Q' && !is_lsl && !is_asr) refuse(line, "lsl or asr is expected");
                const std::int64_t n = value(text::trim(std::string_view(o[j]).substr(3)).starts_with('#') ? text::trim(std::string_view(o[j]).substr(3)).substr(1) : text::trim(std::string_view(o[j]).substr(3)), labels, pc);
                ++j;
                if (k.kind == 'R') {
                    if (n % 8 != 0 || n < 8 || n > 24) refuse(line, "a rotate is 8, 16 or 24");
                    put(w, k.lsb, 2, static_cast<std::uint32_t>(n / 8));
                } else if (is_lsl) {
                    if (n < 0 || n > 31) refuse(line, "a shift is 0 to 31");
                    put(w, 7, 5, static_cast<std::uint32_t>(n));
                } else {
                    if (n < 1 || n > 32) refuse(line, "an asr is 1 to 32");
                    put(w, 7, 5, static_cast<std::uint32_t>(n == 32 ? 0 : n));
                    if (k.kind == 'Q') put(w, 6, 1, 1);
                }
                break;
            }
            case 's': { const unsigned n = vfp_of(line, need(), 's'); put(w, k.lsb, 4, n >> 1); put(w, k.second, 1, n & 1); break; }
            case 'd': { const unsigned n = vfp_of(line, need(), 'd'); put(w, k.lsb, 4, n & 15); put(w, k.second, 1, n >> 4); break; }
            case 'v': {
                std::string s = need();
                if (s.size() < 3 || s.front() != '[' || s.back() != ']') refuse(line, "[rN, #imm] is expected");
                const std::vector<std::string> in = split(std::string_view(s).substr(1, s.size() - 2));
                if (in.empty() || in.size() > 2) refuse(line, "[rN, #imm] is expected");
                put(w, k.lsb, 4, register_of(line, in[0]));
                std::int64_t off = in.size() == 2 ? number_after_hash(in[1]) : 0;
                if (off % 4 != 0 || off > 1020 || off < -1020) refuse(line, "an offset is a multiple of 4 up to 1020");
                if (off >= 0) put(w, 23, 1, 1);
                else off = -off;
                put(w, 0, 8, static_cast<std::uint32_t>(off / 4));
                break;
            }
            case 'z': if (text::lower(need()) != "#0.0") refuse(line, "#0.0 is expected"); break;
            case 'W': case 'X': {
                std::string s = need();
                if (s.size() < 3 || s.front() != '{' || s.back() != '}') refuse(line, "a register list is expected");
                s = s.substr(1, s.size() - 2);
                const char c = k.kind == 'W' ? 's' : 'd';
                const std::size_t dash = s.find('-');
                const unsigned first = vfp_of(line, text::trim(s.substr(0, dash)), c);
                const unsigned last = dash == std::string::npos ? first : vfp_of(line, text::trim(s.substr(dash + 1)), c);
                if (last < first) refuse(line, "a register list runs upwards");
                const unsigned n = last - first + 1;
                if (c == 's') { put(w, k.lsb, 4, first >> 1); put(w, k.second, 1, first & 1); put(w, 0, 8, n); }
                else { put(w, k.lsb, 4, first & 15); put(w, k.second, 1, first >> 4); put(w, 0, 8, 2 * n); }
                break;
            }
            case '=': if (text::lower(need()) != text::lower(k.literal)) refuse(line, "'" + k.literal + "' is expected"); break;
            default: refuse(line, "a bad operand token");
        }
    }
    if (j != o.size()) refuse(line, "too many operands");
    return w;
}

}  // namespace

// A register list that runs past the register file, or is empty, or (doubles) odd, is not an instruction.
bool list_ok(const Row& r, std::uint32_t word) {
    for (const Token& k : tokens_of(r)) {
        if (k.kind == 'v' && bits(word, 23, 1) == 0 && bits(word, 0, 8) == 0) return false;   // (`[rN, #-0]` is not written)
        if (k.kind != 'W' && k.kind != 'X') continue;
        const unsigned count = bits(word, 0, 8), field = bits(word, k.lsb, 4), bit = bits(word, k.second, 1);
        if (k.kind == 'W') return count >= 1 && (field << 1 | bit) + count <= 32;
        return count >= 2 && count % 2 == 0 && count / 2 <= 16 && (field | bit << 4) + count / 2 <= 32;
    }
    return true;
}

int find_arm(std::uint32_t word, Arch arch) {
    if (arch < Arch::V6K) return -1;
    // the rows by bits 27-24, which every row fixes
    static const std::array<std::vector<int>, 16> buckets = [] {
        std::array<std::vector<int>, 16> b;
        for (std::size_t k = 0; k < rows().size(); ++k) b[(rows()[k].value >> 24) & 15].push_back(static_cast<int>(k));
        return b;
    }();
    const auto& table = rows();
    const bool unconditional = (word >> 28) == 15;
    for (const int k : buckets[(word >> 24) & 15]) {
        const Row& r = table[static_cast<std::size_t>(k)];
        if (r.min > arch || r.uncond != unconditional) continue;
        if ((word & r.mask) == r.value && list_ok(r, word)) return k;
    }
    return -1;
}

std::string text(int row, std::uint32_t word, std::uint32_t, const AddressLabels&) {
    const Row& r = rows()[static_cast<std::size_t>(row)];
    std::string out(base_of(r.name));
    if (!r.uncond) out += condition_suffix(word >> 28);
    out += suffix_of(r.name);
    std::string operands;
    for (const Token& k : tokens_of(r)) {
        const std::string t = print_token(k, word);
        if (t.empty()) continue;
        if (t.starts_with(", ")) operands += t;
        else operands += (operands.empty() ? "" : ", ") + t;
    }
    return operands.empty() ? out : out + " " + operands;
}

std::optional<std::uint32_t> assemble(std::string_view line, std::uint32_t pc, const NameLabels& labels, Arch arch) {
    if (arch < Arch::V6K) return std::nullopt;
    line = text::trim(line);
    std::size_t split_at = 0;
    while (split_at < line.size() && !text::is_space(line[split_at])) ++split_at;
    const std::string mnemonic = text::lower(line.substr(0, split_at));
    const std::string_view operand_text = text::ltrim(line.substr(split_at));
    const std::size_t dot = mnemonic.find('.');
    const std::string mnemonic_base = mnemonic.substr(0, dot);
    const std::string mnemonic_suffix = dot == std::string::npos ? std::string() : mnemonic.substr(dot);
    const auto& map = by_base();
    bool found = false;
    std::optional<ParseError> last;
    // the base as it is (condition always), then with a two-letter condition removed
    for (unsigned attempt = 0; attempt < 2; ++attempt) {
        std::string base = mnemonic_base;
        unsigned cond = 14;
        if (attempt == 1) {
            if (base.size() < 3) break;
            const std::optional<unsigned> c = condition_of(base.substr(base.size() - 2));
            if (!c) break;
            cond = *c;
            base.resize(base.size() - 2);
        }
        const auto range = map.equal_range(base);
        for (auto it = range.first; it != range.second; ++it) {
            const Row& r = rows()[static_cast<std::size_t>(it->second)];
            if (r.min > arch || suffix_of(r.name) != mnemonic_suffix || (r.uncond && attempt == 1)) continue;
            found = true;
            try {
                std::vector<std::string> o = split(operand_text);
                std::uint32_t w = build_word(r, o, line, pc, labels);
                w |= r.value;
                if (!r.uncond) w = (w & 0x0FFFFFFFu) | cond << 28;
                return w;
            } catch (const ParseError& e) {
                last = e;
            }
        }
    }
    if (found && last) throw *last;
    return std::nullopt;
}

Sem semantics(int row) { return rows()[static_cast<std::size_t>(row)].sem; }
std::string_view mnemonic(int row) { return rows()[static_cast<std::size_t>(row)].name; }
bool conditional(int row) { return !rows()[static_cast<std::size_t>(row)].uncond; }

}  // namespace cb::decompile::arm::ext
