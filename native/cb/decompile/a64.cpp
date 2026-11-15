// cb-unity: alone
// cb/decompile/a64.cpp -- AArch64: the row builder, the decoder, the text of an instruction and the assembler (decompile/a64.hpp). The rows themselves are in a64_table.cpp
// (integer, memory, floating point) and a64_simd.cpp (vector).
#include "cb/decompile/a64.hpp"

#include <algorithm>
#include <bit>
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <format>
#include <mutex>
#include <stdexcept>
#include <unordered_map>

#include "cb/core/text.hpp"
#include "cb/decompile/a64_internal.hpp"

namespace cb::decompile::a64 {

using detail::K;
using detail::Operand;
using detail::Row;

// ---- bitmask immediates and floating-point immediates --------------------------------------------------------------------------------------------------------------

std::optional<std::uint64_t> decode_bitmask(unsigned n, unsigned imms, unsigned immr, unsigned datasize) {
    const unsigned x = (n << 6) | (~imms & 0x3Fu);
    if (x == 0) return std::nullopt;
    const unsigned len = static_cast<unsigned>(std::bit_width(x)) - 1;
    if (len < 1) return std::nullopt;
    const unsigned esize = 1u << len;
    if (esize > datasize) return std::nullopt;
    const unsigned levels = esize - 1;
    const unsigned s = imms & levels, r = immr & levels;
    if (s == levels || (immr & ~levels & 0x3Fu) != 0) return std::nullopt;   // (an immr with bits above the element size is a second spelling of one that has none: not an instruction here)
    const std::uint64_t mask = esize == 64 ? ~0ull : (1ull << esize) - 1;
    const std::uint64_t welem = (1ull << (s + 1)) - 1;
    std::uint64_t elem = r == 0 ? welem : (((welem >> r) | (welem << (esize - r))) & mask);
    std::uint64_t out = 0;
    for (unsigned at = 0; at < datasize; at += esize) out |= elem << at;
    return datasize == 32 ? out & 0xFFFFFFFFull : out;
}

std::optional<BitmaskFields> encode_bitmask(std::uint64_t value, unsigned datasize) {
    if (datasize == 32) {
        if ((value >> 32) != 0) return std::nullopt;
        value |= value << 32;
    }
    if (value == 0 || value == ~0ull) return std::nullopt;
    unsigned esize = 64;
    while (esize > 2) {
        const unsigned half = esize / 2;
        const std::uint64_t mask = (1ull << half) - 1;
        if ((value & mask) != ((value >> half) & mask)) break;
        esize = half;
    }
    const std::uint64_t mask = esize == 64 ? ~0ull : (1ull << esize) - 1;
    const std::uint64_t elem = value & mask;
    const unsigned ones = static_cast<unsigned>(std::popcount(elem));
    if (ones == 0 || ones == esize) return std::nullopt;
    const std::uint64_t run = (1ull << ones) - 1;
    for (unsigned r = 0; r < esize; ++r) {
        const std::uint64_t rotated = r == 0 ? run : (((run >> r) | (run << (esize - r))) & mask);
        if (rotated == elem) {
            BitmaskFields f;
            f.n = esize == 64 ? 1 : 0;
            f.immr = r;
            f.imms = ((0u - (esize << 1)) | (ones - 1)) & 0x3Fu;
            if (datasize == 32 && f.n != 0) return std::nullopt;
            return f;
        }
    }
    return std::nullopt;
}

double expand_fp_imm8(unsigned imm8) {
    const unsigned a = (imm8 >> 7) & 1, b = (imm8 >> 6) & 1, cd = (imm8 >> 4) & 3, efgh = imm8 & 15;
    const int n = static_cast<int>(((b ^ 1u) << 2) | cd) - 3;
    const double v = (16.0 + efgh) / 16.0 * std::ldexp(1.0, n);
    return a ? -v : v;
}

// ---- names ---------------------------------------------------------------------------------------------------------------------------------------------------

std::string_view condition_name(unsigned c) {
    static const char* const names[16] = {"eq", "ne", "cs", "cc", "mi", "pl", "vs", "vc", "hi", "ls", "ge", "lt", "gt", "le", "al", "nv"};
    return names[c & 15];
}

namespace {

std::optional<unsigned> condition_by_name(std::string_view s) {
    for (unsigned c = 0; c < 16; ++c)
        if (condition_name(c) == s) return c;
    if (s == "hs") return 2u;
    if (s == "lo") return 3u;
    return std::nullopt;
}

constexpr std::uint32_t sysreg(unsigned op0, unsigned op1, unsigned crn, unsigned crm, unsigned op2) {
    return ((op0 - 2) << 14) | (op1 << 11) | (crn << 7) | (crm << 3) | op2;
}

struct SysName {
    const char* name;
    std::uint32_t packed;
};

const std::vector<SysName>& system_names() {
    static const std::vector<SysName> t = {
        {"nzcv", sysreg(3, 3, 4, 2, 0)},          {"daif", sysreg(3, 3, 4, 2, 1)},          {"fpcr", sysreg(3, 3, 4, 4, 0)},         {"fpsr", sysreg(3, 3, 4, 4, 1)},
        {"dczid_el0", sysreg(3, 3, 0, 0, 7)},     {"ctr_el0", sysreg(3, 3, 0, 0, 1)},       {"tpidr_el0", sysreg(3, 3, 13, 0, 2)},   {"tpidrro_el0", sysreg(3, 3, 13, 0, 3)},
        {"cntfrq_el0", sysreg(3, 3, 14, 0, 0)},   {"cntpct_el0", sysreg(3, 3, 14, 0, 1)},   {"cntvct_el0", sysreg(3, 3, 14, 0, 2)},  {"cntp_tval_el0", sysreg(3, 3, 14, 2, 0)},
        {"cntp_ctl_el0", sysreg(3, 3, 14, 2, 1)}, {"cntp_cval_el0", sysreg(3, 3, 14, 2, 2)}, {"cntv_tval_el0", sysreg(3, 3, 14, 3, 0)}, {"cntv_ctl_el0", sysreg(3, 3, 14, 3, 1)},
        {"cntv_cval_el0", sysreg(3, 3, 14, 3, 2)}, {"midr_el1", sysreg(3, 0, 0, 0, 0)},     {"mpidr_el1", sysreg(3, 0, 0, 0, 5)},    {"revidr_el1", sysreg(3, 0, 0, 0, 6)},
        {"tpidr2_el0", sysreg(3, 3, 13, 0, 5)}, {"currentel", sysreg(3, 0, 4, 2, 2)},     {"spsel", sysreg(3, 0, 4, 2, 0)},         {"sp_el0", sysreg(3, 0, 4, 1, 0)},       {"elr_el1", sysreg(3, 0, 4, 0, 1)},
        {"spsr_el1", sysreg(3, 0, 4, 0, 0)},      {"sctlr_el1", sysreg(3, 0, 1, 0, 0)},     {"tcr_el1", sysreg(3, 0, 2, 0, 2)},      {"ttbr0_el1", sysreg(3, 0, 2, 0, 0)},
        {"ttbr1_el1", sysreg(3, 0, 2, 0, 1)},     {"vbar_el1", sysreg(3, 0, 12, 0, 0)},     {"esr_el1", sysreg(3, 0, 5, 2, 0)},      {"far_el1", sysreg(3, 0, 6, 0, 0)},
        {"tpidr_el1", sysreg(3, 0, 13, 0, 4)},    {"id_aa64pfr0_el1", sysreg(3, 0, 0, 4, 0)}, {"id_aa64isar0_el1", sysreg(3, 0, 0, 6, 0)}, {"id_aa64mmfr0_el1", sysreg(3, 0, 0, 7, 0)},
        {"pmcr_el0", sysreg(3, 3, 9, 12, 0)},     {"pmcntenset_el0", sysreg(3, 3, 9, 12, 1)}, {"pmuserenr_el0", sysreg(3, 3, 9, 14, 0)}, {"pmccntr_el0", sysreg(3, 3, 9, 13, 0)}};
    return t;
}

}  // namespace

std::string_view system_register_name(std::uint32_t packed) {
    for (const SysName& n : system_names())
        if (n.packed == packed) return n.name;
    return {};
}

namespace {

// ---- numbers -------------------------------------------------------------------------------------------------------------------------------------------------

std::string num(std::int64_t v) {
    const std::uint64_t u = v < 0 ? 0 - static_cast<std::uint64_t>(v) : static_cast<std::uint64_t>(v);
    const std::string body = u < 10 ? std::to_string(u) : std::format("0x{:X}", u);
    return v < 0 ? "-" + body : body;
}
std::string imm_text(std::int64_t v) { return "#" + num(v); }

std::string reg_text(unsigned n, bool is64, bool sp) {
    if (n == 31) return sp ? (is64 ? "sp" : "wsp") : (is64 ? "xzr" : "wzr");
    return std::string(is64 ? "x" : "w") + std::to_string(n);
}

std::string addr_text(std::uint64_t a, const AddressLabels& labels) {
    if (a <= 0xFFFFFFFFull) {
        const auto it = labels.find(static_cast<std::uint32_t>(a));
        if (it != labels.end() && !it->second.empty()) return it->second;
    }
    return std::format("0x{:X}", a);
}

std::int64_t sext(std::uint64_t v, unsigned width) {
    const std::uint64_t m = 1ull << (width - 1);
    return static_cast<std::int64_t>((v ^ m) - m);
}

std::uint64_t fits_mask(unsigned w) { return w >= 64 ? ~0ull : (1ull << w) - 1; }

// ---- the expression evaluator --------------------------------------------------------------------------------------------------------------------------------

class Expr {
public:
    Expr(std::string_view s, const NameLabels& labels, bool lenient) : s_(s), labels_(labels), lenient_(lenient) {}

    std::int64_t run() {
        const std::int64_t v = parse_or();
        skip();
        if (i_ != s_.size()) fail("unexpected '" + std::string(s_.substr(i_)) + "'");
        return v;
    }

private:
    [[noreturn]] void fail(const std::string& why) const { throw ParseError("", 0, 0, "bad expression '" + std::string(s_) + "': " + why); }
    void skip() {
        while (i_ < s_.size() && (s_[i_] == ' ' || s_[i_] == '\t')) ++i_;
    }
    bool eat(std::string_view token) {
        skip();
        if (s_.substr(i_).starts_with(token)) {
            i_ += token.size();
            return true;
        }
        return false;
    }
    std::int64_t parse_or() {
        std::int64_t v = parse_and();
        while (true) {
            skip();
            if (i_ < s_.size() && s_[i_] == '|') {
                ++i_;
                v |= parse_and();
            } else {
                return v;
            }
        }
    }
    std::int64_t parse_and() {
        std::int64_t v = parse_shift();
        while (true) {
            skip();
            if (i_ < s_.size() && s_[i_] == '&') {
                ++i_;
                v &= parse_shift();
            } else {
                return v;
            }
        }
    }
    std::int64_t parse_shift() {
        std::int64_t v = parse_add();
        while (true) {
            if (eat("<<")) v = static_cast<std::int64_t>(static_cast<std::uint64_t>(v) << (parse_add() & 63));
            else if (eat(">>")) v >>= (parse_add() & 63);
            else return v;
        }
    }
    std::int64_t parse_add() {
        std::int64_t v = parse_mul();
        while (true) {
            skip();
            if (i_ < s_.size() && s_[i_] == '+') {
                ++i_;
                v += parse_mul();
            } else if (i_ < s_.size() && s_[i_] == '-') {
                ++i_;
                v -= parse_mul();
            } else {
                return v;
            }
        }
    }
    std::int64_t parse_mul() {
        std::int64_t v = parse_unary();
        while (true) {
            skip();
            if (i_ < s_.size() && s_[i_] == '*') {
                ++i_;
                v *= parse_unary();
            } else {
                return v;
            }
        }
    }
    std::int64_t parse_unary() {
        skip();
        if (i_ >= s_.size()) fail("an operand is missing");
        if (s_[i_] == '-') {
            ++i_;
            return -parse_unary();
        }
        if (s_[i_] == '+') {
            ++i_;
            return parse_unary();
        }
        if (s_[i_] == '~') {
            ++i_;
            return ~parse_unary();
        }
        if (s_[i_] == '(') {
            ++i_;
            const std::int64_t v = parse_or();
            if (!eat(")")) fail("missing ')'");
            return v;
        }
        if (s_.substr(i_).starts_with(":lo12:")) {
            i_ += 6;
            return parse_unary() & 0xFFF;
        }
        const char c = s_[i_];
        if (std::isdigit(static_cast<unsigned char>(c))) {
            std::size_t j = i_;
            unsigned base = 10;
            if (c == '0' && j + 1 < s_.size() && (s_[j + 1] == 'x' || s_[j + 1] == 'X')) {
                base = 16;
                j += 2;
            } else if (c == '0' && j + 1 < s_.size() && (s_[j + 1] == 'b' || s_[j + 1] == 'B')) {
                base = 2;
                j += 2;
            }
            std::uint64_t v = 0;
            const std::size_t start = j;
            while (j < s_.size() && std::isxdigit(static_cast<unsigned char>(s_[j]))) {
                const char d = s_[j];
                const unsigned digit = std::isdigit(static_cast<unsigned char>(d)) ? static_cast<unsigned>(d - '0') : static_cast<unsigned>(std::tolower(d) - 'a' + 10);
                if (digit >= base) break;
                v = v * base + digit;
                ++j;
            }
            if (j == start) fail("a number is missing its digits");
            i_ = j;
            return static_cast<std::int64_t>(v);
        }
        if (std::isalpha(static_cast<unsigned char>(c)) || c == '_' || c == '.' || c == '$') {
            std::size_t j = i_;
            while (j < s_.size() && (std::isalnum(static_cast<unsigned char>(s_[j])) || s_[j] == '_' || s_[j] == '.' || s_[j] == '$')) ++j;
            const std::string_view name = s_.substr(i_, j - i_);
            i_ = j;
            const auto it = labels_.find(name);
            if (it != labels_.end()) return it->second;
            if (lenient_) return 0;
            fail("undefined symbol '" + std::string(name) + "'");
        }
        fail(std::string("unexpected '") + c + "'");
    }

    std::string_view s_;
    const NameLabels& labels_;
    bool lenient_;
    std::size_t i_ = 0;
};

// ---- operand text --------------------------------------------------------------------------------------------------------------------------------------------

const char* const kShiftNames[4] = {"lsl", "lsr", "asr", "ror"};
const char* const kExtendNames[8] = {"uxtb", "uxth", "uxtw", "uxtx", "sxtb", "sxth", "sxtw", "sxtx"};

struct PrintCtx {
    std::uint64_t pc;
    const AddressLabels& labels;
    std::string_view lo12;
};

std::string base_text(std::uint32_t w) { return reg_text(detail::bits(w, 5, 5), true, true); }

std::string bar_text(unsigned c) {
    switch (c) {
        case 1: return "oshld";
        case 2: return "oshst";
        case 3: return "osh";
        case 5: return "nshld";
        case 6: return "nshst";
        case 7: return "nsh";
        case 9: return "ishld";
        case 10: return "ishst";
        case 11: return "ish";
        case 13: return "ld";
        case 14: return "st";
        case 15: return "sy";
        default: return imm_text(c);
    }
}

std::string prefetch_text(unsigned t) {
    const unsigned type = t >> 3, target = (t >> 1) & 3, policy = t & 1;
    if (type > 2 || target > 2) return imm_text(t);
    static const char* const types[3] = {"pld", "pli", "pst"};
    static const char* const targets[3] = {"l1", "l2", "l3"};
    return std::string(types[type]) + targets[target] + (policy ? "strm" : "keep");
}

std::string print_operand(const Row& row, const Operand& o, std::uint32_t w, const PrintCtx& c) {
    using detail::bits;
    const unsigned field = o.width ? bits(w, o.pos, o.width) : 0;
    const bool sf = (w >> 31) & 1;
    switch (o.kind) {
        case K::X: return reg_text(field, true, false);
        case K::W: return reg_text(field, false, false);
        case K::R: return reg_text(field, sf, false);
        case K::XS: return reg_text(field, true, true);
        case K::WS: return reg_text(field, false, true);
        case K::RS: return reg_text(field, sf, true);
        case K::FB: return "b" + std::to_string(field);
        case K::FH: return "h" + std::to_string(field);
        case K::FS: return "s" + std::to_string(field);
        case K::FD: return "d" + std::to_string(field);
        case K::FQ: return "q" + std::to_string(field);
        case K::U: return imm_text(field);
        case K::I: return imm_text(sext(field, o.width));
        case K::AIMM: {
            const unsigned imm = bits(w, 10, 12);
            if (!c.lo12.empty() && !bits(w, 22, 1)) return ":lo12:" + std::string(c.lo12);
            return imm_text(imm) + (bits(w, 22, 1) ? ", lsl #12" : "");
        }
        case K::SREG: {
            const unsigned h = bits(w, 22, 2), i = bits(w, 10, 6);
            std::string t = reg_text(bits(w, 16, 5), sf, false);
            if (h != 0 || i != 0) t += std::string(", ") + kShiftNames[h] + " #" + std::to_string(i);
            return t;
        }
        case K::EREG: {
            const unsigned opt = bits(w, 13, 3), i = bits(w, 10, 3);
            std::string t = reg_text(bits(w, 16, 5), sf && (opt & 3) == 3, false);
            if (opt == (sf ? 3u : 2u) && ((bits(w, 0, 5) == 31 && bits(w, 29, 1) == 0) || bits(w, 5, 5) == 31)) {   // (with sp as a register, the extend that is a plain shift is written lsl)
                if (i != 0) t += ", lsl #" + std::to_string(i);
                return t;
            }
            t += std::string(", ") + kExtendNames[opt];
            if (i != 0) t += " #" + std::to_string(i);
            return t;
        }
        case K::LI: {
            const std::optional<std::uint64_t> v = decode_bitmask(bits(w, 22, 1), bits(w, 10, 6), bits(w, 16, 6), sf ? 64 : 32);
            return "#" + (v ? std::format("0x{:X}", *v) : std::string("?"));
        }
        case K::MWI: {
            const unsigned hw = bits(w, 21, 2);
            return imm_text(bits(w, 5, 16)) + (hw ? ", lsl #" + std::to_string(16 * hw) : std::string());
        }
        case K::MOVZV: {
            const std::uint64_t v = static_cast<std::uint64_t>(bits(w, 5, 16)) << (16 * bits(w, 21, 2));
            return "#" + num(static_cast<std::int64_t>(v));
        }
        case K::MOVNV: {
            std::uint64_t v = ~(static_cast<std::uint64_t>(bits(w, 5, 16)) << (16 * bits(w, 21, 2)));
            if (!sf) v &= 0xFFFFFFFFull;
            return "#" + num(sf ? static_cast<std::int64_t>(v) : static_cast<std::int32_t>(static_cast<std::uint32_t>(v)));
        }
        case K::LSLI: {
            const unsigned size = sf ? 64 : 32;
            return imm_text(size - 1 - bits(w, 10, 6));
        }
        case K::SHRI: return imm_text(bits(w, 16, 6));
        case K::BFX: {
            const unsigned r = bits(w, 16, 6), u = bits(w, 10, 6);
            return imm_text(r) + ", " + imm_text(u - r + 1);
        }
        case K::BFIZ: {
            const unsigned size = sf ? 64 : 32, r = bits(w, 16, 6), u = bits(w, 10, 6);
            return imm_text((size - r) & (size - 1)) + ", " + imm_text(u + 1);
        }
        case K::COND: return std::string(condition_name(field));
        case K::CONDINV: return std::string(condition_name(field ^ 1));
        case K::PR: return addr_text(c.pc + static_cast<std::uint64_t>(sext(field, o.width) * 4), c.labels);
        case K::PADR: {
            const std::int64_t imm = sext((bits(w, 5, 19) << 2) | bits(w, 29, 2), 21);
            return addr_text(c.pc + static_cast<std::uint64_t>(imm), c.labels);
        }
        case K::PADRP: {
            const std::int64_t imm = sext((bits(w, 5, 19) << 2) | bits(w, 29, 2), 21);
            return addr_text((c.pc & ~0xFFFull) + static_cast<std::uint64_t>(imm * 4096), c.labels);
        }
        case K::MU: {
            const std::int64_t off = static_cast<std::int64_t>(bits(w, 10, 12)) << o.param;
            if (!c.lo12.empty()) return "[" + base_text(w) + ", :lo12:" + std::string(c.lo12) + "]";
            return "[" + base_text(w) + (off ? ", " + imm_text(off) : std::string()) + "]";
        }
        case K::MT: {
            const std::int64_t off = sext(bits(w, 12, 9), 9);
            return "[" + base_text(w) + (off ? ", " + imm_text(off) : std::string()) + "]";
        }
        case K::MPRE: return "[" + base_text(w) + ", " + imm_text(sext(bits(w, 12, 9), 9)) + "]!";
        case K::MPOST: return "[" + base_text(w) + "], " + imm_text(sext(bits(w, 12, 9), 9));
        case K::MR: {
            const unsigned opt = bits(w, 13, 3), s = bits(w, 12, 1);
            std::string t = "[" + base_text(w) + ", " + reg_text(bits(w, 16, 5), (opt & 1) != 0, false);
            if (opt == 3) {
                if (s) t += ", lsl #" + std::to_string(o.param);
            } else {
                t += std::string(", ") + kExtendNames[opt];
                if (s) t += " #" + std::to_string(o.param);
            }
            return t + "]";
        }
        case K::MPO: {
            const std::int64_t off = sext(bits(w, 15, 7), 7) * (1ll << o.param);
            return "[" + base_text(w) + (off ? ", " + imm_text(off) : std::string()) + "]";
        }
        case K::MPPRE: return "[" + base_text(w) + ", " + imm_text(sext(bits(w, 15, 7), 7) * (1ll << o.param)) + "]!";
        case K::MPPOST: return "[" + base_text(w) + "], " + imm_text(sext(bits(w, 15, 7), 7) * (1ll << o.param));
        case K::MX: return "[" + base_text(w) + "]";
        case K::SYSREG: {
            const std::uint32_t packed = bits(w, 5, 15);
            const std::string_view name = system_register_name(packed);
            if (!name.empty()) return std::string(name);
            return std::format("s{}_{}_c{}_c{}_{}", 2 + (packed >> 14), (packed >> 11) & 7, (packed >> 7) & 15, (packed >> 3) & 15, packed & 7);
        }
        case K::BAR: return bar_text(field);
        case K::PF: return prefetch_text(field);
        case K::LIT: return o.literal;
        case K::TBIT: return imm_text((bits(w, 31, 1) << 5) | bits(w, 19, 5));
        case K::FPIMM: return "#" + std::format("{:.8f}", expand_fp_imm8(bits(w, 13, 8)));
        case K::FBITS: return imm_text(64 - bits(w, 10, 6));
        default: break;
    }
    return detail::print_simd_operand(row, o, w);
}

std::string row_text(const Row& row, std::uint32_t w, const PrintCtx& c) {
    std::string t = row.name;
    if (row.cond_suffix) t += "." + std::string(condition_name(w & 15));
    bool first = true;
    for (const Operand& o : row.operands) {
        t += first ? " " : ", ";
        first = false;
        t += print_operand(row, o, w, c);
    }
    return t;
}

}  // namespace

// ---- the builder and the table ---------------------------------------------------------------------------------------------------------------------------------

namespace detail {

namespace {

K kind_by_name(std::string_view n) {
    static const std::unordered_map<std::string_view, K> kinds = {
        {"X", K::X},         {"W", K::W},         {"R", K::R},       {"XS", K::XS},       {"WS", K::WS},       {"RS", K::RS},       {"FB", K::FB},       {"FH", K::FH},
        {"FS", K::FS},       {"FD", K::FD},       {"FQ", K::FQ},     {"U", K::U},         {"I", K::I},         {"AIMM", K::AIMM},   {"SREG", K::SREG},   {"EREG", K::EREG},
        {"LI", K::LI},       {"MWI", K::MWI},     {"MOVZV", K::MOVZV}, {"MOVNV", K::MOVNV}, {"LSLI", K::LSLI},  {"SHRI", K::SHRI},   {"BFX", K::BFX},     {"BFIZ", K::BFIZ},
        {"COND", K::COND},   {"CONDINV", K::CONDINV}, {"PR", K::PR}, {"PADR", K::PADR},   {"PADRP", K::PADRP}, {"MU", K::MU},       {"MT", K::MT},       {"MPRE", K::MPRE},
        {"MPOST", K::MPOST}, {"MR", K::MR},       {"MPO", K::MPO},   {"MPPRE", K::MPPRE}, {"MPPOST", K::MPPOST}, {"MX", K::MX},     {"SYSREG", K::SYSREG}, {"BAR", K::BAR},
        {"PF", K::PF},       {"TBIT", K::TBIT},   {"FPIMM", K::FPIMM}, {"FBITS", K::FBITS}};
    const auto it = kinds.find(n);
    if (it != kinds.end()) return it->second;
    return simd_kind_by_name(n);
}

}  // namespace

Row& Builder::add(std::string name, Op op, std::string_view pattern, std::string_view spec, Flow flow, When when) {
    Row r;
    r.name = std::move(name);
    r.op = op;
    r.flow = flow;
    r.when = when;
    int pos = 32;
    struct Field {
        int lo = -1, width = 0;
    };
    std::array<Field, 128> fields{};
    for (const char c : pattern) {
        if (c == ' ') continue;
        if (--pos < 0) throw std::logic_error("a64 row '" + r.name + "': pattern longer than 32 bits: " + std::string(pattern));
        if (c == '0' || c == '1') {
            r.mask |= 1u << pos;
            if (c == '1') r.bits |= 1u << pos;
        } else {
            Field& f = fields[static_cast<unsigned char>(c) & 127];
            if (f.lo >= 0 && f.lo != pos + 1) throw std::logic_error("a64 row '" + r.name + "': field '" + std::string(1, c) + "' is not contiguous: " + std::string(pattern));
            f.lo = pos;
            ++f.width;
        }
    }
    if (pos != 0) throw std::logic_error("a64 row '" + r.name + "': pattern shorter than 32 bits: " + std::string(pattern));
    std::size_t at = 0;
    while (at < spec.size()) {
        std::size_t end = spec.find(',', at);
        if (end == std::string_view::npos) end = spec.size();
        const std::string_view token = spec.substr(at, end - at);
        at = end + 1;
        Operand o;
        if (token.starts_with('=')) {
            o.kind = K::LIT;
            o.literal = std::string(token.substr(1));
            r.operands.push_back(std::move(o));
            continue;
        }
        std::size_t i = 0;
        while (i < token.size() && std::isupper(static_cast<unsigned char>(token[i]))) ++i;
        const std::string_view kind_name = token.substr(0, i);
        unsigned param = 0;
        while (i < token.size() && std::isdigit(static_cast<unsigned char>(token[i]))) param = param * 10 + static_cast<unsigned>(token[i++] - '0');
        o.kind = kind_by_name(kind_name);
        o.param = static_cast<std::uint8_t>(param);
        if (i < token.size()) {
            const Field& f = fields[static_cast<unsigned char>(token[i]) & 127];
            if (f.lo < 0) throw std::logic_error("a64 row '" + r.name + "': operand '" + std::string(token) + "' names a field the pattern lacks: " + std::string(pattern));
            o.pos = static_cast<std::uint8_t>(f.lo);
            o.width = static_cast<std::uint8_t>(f.width);
        }
        r.operands.push_back(std::move(o));
    }
    rows_.push_back(std::move(r));
    return rows_.back();
}

void Builder::add_cond_suffix(std::string name, Op op, std::string_view pattern, std::string_view spec, Flow flow) {
    add(std::move(name), op, pattern, spec, flow).cond_suffix = true;
}

namespace {

struct Table {
    std::vector<Row> rows;
    std::array<std::vector<std::uint16_t>, 16> buckets;
    std::unordered_map<std::string, std::vector<std::uint16_t>> by_name;
};

const Table& table() {
    static const Table t = [] {
        Builder b;
        add_integer_rows(b);
        add_memory_rows(b);
        add_fp_rows(b);
        add_simd_rows(b);
        Table out;
        out.rows = std::move(b.rows());
        if (out.rows.size() > 0xFFFF) throw std::logic_error("a64: too many rows");
        for (std::size_t i = 0; i < out.rows.size(); ++i) {
            const Row& r = out.rows[i];
            for (std::uint32_t k = 0; k < 16; ++k)
                if ((((k << 25) ^ r.bits) & r.mask & 0x1E000000u) == 0) out.buckets[k].push_back(static_cast<std::uint16_t>(i));
            out.by_name[r.name].push_back(static_cast<std::uint16_t>(i));
        }
        return out;
    }();
    return t;
}

}  // namespace

const std::vector<Row>& rows() { return table().rows; }

const std::vector<std::uint16_t>& table_bucket(unsigned bucket) { return table().buckets[bucket & 15]; }

const std::vector<std::uint16_t>& rows_named(std::string_view name) {
    static const std::vector<std::uint16_t> none;
    const Table& t = table();
    const auto it = t.by_name.find(std::string(name));
    return it == t.by_name.end() ? none : it->second;
}

}  // namespace detail

// ---- decode and text -----------------------------------------------------------------------------------------------------------------------------------------

std::optional<Insn> decode_word(std::uint32_t w, std::uint64_t pc) {
    const std::vector<Row>& all = detail::rows();
    const auto& bucket = detail::table_bucket((w >> 25) & 15);
    for (const std::uint16_t index : bucket) {
        const Row& r = all[index];
        if ((w & r.mask) != r.bits || (r.when && !r.when(w))) continue;
        Insn insn;
        insn.at = pc;
        insn.word = w;
        insn.op = r.op;
        insn.row = index;
        insn.mnemonic = r.name;
        insn.flow = r.flow;
        for (unsigned k = 0; k < 4; ++k) insn.raw[k] = static_cast<std::uint8_t>(w >> (8 * k));
        for (const Operand& o : r.operands) {
            if (o.kind == K::PR) insn.target = pc + static_cast<std::uint64_t>(sext(detail::bits(w, o.pos, o.width), o.width) * 4);
            else if (o.kind == K::PADR) insn.target = pc + static_cast<std::uint64_t>(sext((detail::bits(w, 5, 19) << 2) | detail::bits(w, 29, 2), 21));
            else if (o.kind == K::PADRP) insn.target = (pc & ~0xFFFull) + static_cast<std::uint64_t>(sext((detail::bits(w, 5, 19) << 2) | detail::bits(w, 29, 2), 21) * 4096);
        }
        return insn;
    }
    return std::nullopt;
}

std::optional<Insn> decode(ByteView code, std::size_t offset, std::uint64_t pc) {
    if (offset + 4 > code.size()) return std::nullopt;
    const std::uint32_t w = static_cast<std::uint32_t>(code[offset]) | static_cast<std::uint32_t>(code[offset + 1]) << 8 | static_cast<std::uint32_t>(code[offset + 2]) << 16 |
                            static_cast<std::uint32_t>(code[offset + 3]) << 24;
    return decode_word(w, pc);
}

std::string text(const Insn& insn, const AddressLabels& labels, std::string_view lo12) {
    const PrintCtx c{insn.at, labels, lo12};
    return row_text(detail::rows()[insn.row], insn.word, c);
}

bool is_mnemonic(std::string_view mnemonic) {
    std::string m = cb::text::lower(mnemonic);
    if (const std::size_t dot = m.find('.'); dot != std::string::npos && m.starts_with("b.")) m = m.substr(0, dot);
    return !detail::rows_named(m).empty();
}

std::int64_t value(std::string_view expr, const NameLabels& labels, bool lenient) { return Expr(expr, labels, lenient).run(); }

// ---- the assembler -------------------------------------------------------------------------------------------------------------------------------------------

namespace {

struct Reg {
    unsigned n = 0;
    bool is64 = true, sp = false, zr = false;
};

std::optional<Reg> parse_gp(std::string_view s) {
    const std::string t = cb::text::lower(cb::text::trim(s));
    if (t == "sp") return Reg{31, true, true, false};
    if (t == "wsp") return Reg{31, false, true, false};
    if (t == "xzr") return Reg{31, true, false, true};
    if (t == "wzr") return Reg{31, false, false, true};
    if (t == "fp") return Reg{29, true, false, false};
    if (t == "lr") return Reg{30, true, false, false};
    if (t.size() < 2 || t.size() > 3 || (t[0] != 'x' && t[0] != 'w')) return std::nullopt;
    unsigned n = 0;
    for (std::size_t i = 1; i < t.size(); ++i) {
        if (!std::isdigit(static_cast<unsigned char>(t[i]))) return std::nullopt;
        n = n * 10 + static_cast<unsigned>(t[i] - '0');
    }
    if (n > 30 || (t.size() == 3 && t[1] == '0')) return std::nullopt;
    return Reg{n, t[0] == 'x', false, false};
}

std::optional<unsigned> parse_scalar(std::string_view s, char kind) {
    const std::string t = cb::text::lower(cb::text::trim(s));
    if (t.size() < 2 || t.size() > 3 || t[0] != kind) return std::nullopt;
    unsigned n = 0;
    for (std::size_t i = 1; i < t.size(); ++i) {
        if (!std::isdigit(static_cast<unsigned char>(t[i]))) return std::nullopt;
        n = n * 10 + static_cast<unsigned>(t[i] - '0');
    }
    if (n > 31 || (t.size() == 3 && t[1] == '0')) return std::nullopt;
    return n;
}

struct Parse {
    std::uint32_t word = 0;
    const std::vector<std::string_view>& tokens;
    std::size_t at = 0;
    std::uint64_t pc = 0;
    const NameLabels& labels;
    bool lenient = false;
    bool sf_set = false;
    bool arr_set = false;
    std::string error;

    bool fail(std::string why) {
        if (error.empty()) error = std::move(why);
        return false;
    }
    bool more() const { return at < tokens.size(); }
    std::string_view take() { return tokens[at++]; }
    void put(unsigned pos, unsigned width, std::uint64_t v) {
        const std::uint32_t m = width >= 32 ? 0xFFFFFFFFu : ((1u << width) - 1);
        word = (word & ~(m << pos)) | ((static_cast<std::uint32_t>(v) & m) << pos);
    }
    bool sf() const { return (word >> 31) & 1; }
};

std::int64_t eval(Parse& p, std::string_view s) { return value(s, p.labels, p.lenient); }

std::string_view strip_hash(std::string_view s) {
    s = cb::text::trim(s);
    if (s.starts_with('#')) s.remove_prefix(1);
    return cb::text::trim(s);
}

bool starts_word(std::string_view s, std::string_view word) {
    s = cb::text::trim(s);
    if (s.size() < word.size() || cb::text::lower(s.substr(0, word.size())) != word) return false;
    return s.size() == word.size() || s[word.size()] == ' ' || s[word.size()] == '\t' || s[word.size()] == '#';
}

// "lsl #3" -> (name, amount); the amount is optional.
bool split_modifier(std::string_view s, std::string& name, std::optional<std::int64_t>& amount, Parse& p) {
    s = cb::text::trim(s);
    std::size_t i = 0;
    while (i < s.size() && std::isalpha(static_cast<unsigned char>(s[i]))) ++i;
    if (i == 0) return false;
    name = cb::text::lower(s.substr(0, i));
    const std::string_view rest = cb::text::trim(s.substr(i));
    amount.reset();
    if (!rest.empty()) amount = eval(p, strip_hash(rest));
    return true;
}

// The inside of a bracketed token: "[x1, #8]" -> {"x1", "#8"}; the `!` suffix is reported.
bool unbracket(std::string_view t, std::vector<std::string_view>& parts, bool& bang) {
    t = cb::text::trim(t);
    bang = false;
    if (t.ends_with('!')) {
        bang = true;
        t.remove_suffix(1);
        t = cb::text::trim(t);
    }
    if (t.size() < 2 || t.front() != '[' || t.back() != ']') return false;
    t = t.substr(1, t.size() - 2);
    parts = cb::text::split_trim(t, ',');
    return !parts.empty();
}

bool parse_base(Parse& p, std::string_view s) {
    const std::optional<Reg> r = parse_gp(s);
    if (!r || !r->is64 || r->zr) return p.fail("the base register of a memory operand is an x register or sp");
    p.put(5, 5, r->n);
    return true;
}

bool parse_gp_kind(Parse& p, const Operand& o, bool fixed64, bool use_sf, bool sp_ok) {
    if (!p.more()) return p.fail("an operand is missing");
    const std::optional<Reg> r = parse_gp(p.take());
    if (!r) return p.fail("a register is expected");
    if (r->n == 31 && (r->sp != sp_ok)) return p.fail(sp_ok ? "sp is expected, not the zero register" : "the zero register is expected, not sp");
    if (use_sf) {
        if (!p.sf_set) {
            p.put(31, 1, r->is64);
            p.sf_set = true;
        } else if (p.sf() != r->is64) {
            return p.fail("register widths differ");
        }
    } else if (r->is64 != fixed64) {
        return p.fail(fixed64 ? "an x register is expected" : "a w register is expected");
    }
    p.put(o.pos, 5, r->n);
    return true;
}

bool parse_imm_field(Parse& p, const Operand& o, bool is_signed) {
    if (!p.more()) return p.fail("an operand is missing");
    const std::string_view t = p.take();
    if (t.find('[') != std::string_view::npos) return p.fail("an immediate is expected");
    std::int64_t v;
    try {
        v = eval(p, strip_hash(t));
    } catch (const ParseError& e) {
        return p.fail(e.what());
    }
    if (!p.lenient) {
        if (is_signed) {
            const std::int64_t lo = -(1ll << (o.width - 1)), hi = (1ll << (o.width - 1)) - 1;
            if (v < lo || v > hi) return p.fail("immediate out of range");
        } else if (v < 0 || static_cast<std::uint64_t>(v) > fits_mask(o.width)) {
            return p.fail("immediate out of range");
        }
    }
    p.put(o.pos, o.width, static_cast<std::uint64_t>(v));
    return true;
}

bool parse_target(Parse& p, std::string_view t, std::int64_t& target) {
    try {
        target = eval(p, cb::text::trim(t));
    } catch (const ParseError& e) {
        return p.fail(e.what());
    }
    if (p.lenient && target == 0) target = static_cast<std::int64_t>(p.pc);
    return true;
}

bool parse_operand(const Row& row, const Operand& o, Parse& p) {
    using detail::bits;
    (void)row;
    switch (o.kind) {
        case K::X: return parse_gp_kind(p, o, true, false, false);
        case K::W: return parse_gp_kind(p, o, false, false, false);
        case K::R: return parse_gp_kind(p, o, false, true, false);
        case K::XS: return parse_gp_kind(p, o, true, false, true);
        case K::WS: return parse_gp_kind(p, o, false, false, true);
        case K::RS: return parse_gp_kind(p, o, false, true, true);
        case K::FB:
        case K::FH:
        case K::FS:
        case K::FD:
        case K::FQ: {
            if (!p.more()) return p.fail("an operand is missing");
            const char c = o.kind == K::FB ? 'b' : o.kind == K::FH ? 'h' : o.kind == K::FS ? 's' : o.kind == K::FD ? 'd' : 'q';
            const std::optional<unsigned> n = parse_scalar(p.take(), c);
            if (!n) return p.fail(std::string("a ") + c + " register is expected");
            p.put(o.pos, 5, *n);
            return true;
        }
        case K::U: return parse_imm_field(p, o, false);
        case K::I: return parse_imm_field(p, o, true);
        case K::AIMM: {
            if (!p.more()) return p.fail("an operand is missing");
            std::int64_t v;
            try {
                v = eval(p, strip_hash(p.take()));
            } catch (const ParseError& e) {
                return p.fail(e.what());
            }
            unsigned shift = 0;
            if (p.more() && starts_word(p.tokens[p.at], "lsl")) {
                std::string name;
                std::optional<std::int64_t> amount;
                split_modifier(p.take(), name, amount, p);
                if (!amount || (*amount != 0 && *amount != 12)) return p.fail("the shift of an immediate is lsl #0 or lsl #12");
                shift = *amount == 12 ? 1 : 0;
            }
            if (shift == 0 && v > 4095 && (v & 0xFFF) == 0 && (v >> 12) <= 4095 && !p.more()) {   // (a multiple of 4096 is the immediate shifted left by 12)
                v >>= 12;
                shift = 1;
            }
            if (!p.lenient && (v < 0 || v > 4095)) return p.fail("immediate out of range (0..4095)");
            p.put(10, 12, static_cast<std::uint64_t>(v));
            p.put(22, 1, shift);
            return true;
        }
        case K::SREG: {
            Operand reg;
            reg.pos = 16;
            if (!parse_gp_kind(p, reg, false, true, false)) return false;
            unsigned type = 0;
            std::int64_t amount = 0;
            if (p.more()) {
                static const std::string_view names[4] = {"lsl", "lsr", "asr", "ror"};
                for (unsigned k = 0; k < 4; ++k) {
                    if (!starts_word(p.tokens[p.at], names[k])) continue;
                    std::string name;
                    std::optional<std::int64_t> a;
                    split_modifier(p.take(), name, a, p);
                    if (!a) return p.fail("a shift needs an amount");
                    type = k;
                    amount = *a;
                    break;
                }
            }
            if (!p.lenient && (amount < 0 || amount >= (p.sf() ? 64 : 32))) return p.fail("shift amount out of range");
            p.put(22, 2, type);
            p.put(10, 6, static_cast<std::uint64_t>(amount));
            return true;
        }
        case K::EREG: {
            if (!p.more()) return p.fail("an operand is missing");
            const std::optional<Reg> r = parse_gp(p.take());
            if (!r || r->sp) return p.fail("a register is expected");
            p.put(16, 5, r->n);
            unsigned option = r->is64 ? 3 : 2;
            std::int64_t amount = 0;
            if (p.more()) {
                std::string name = cb::text::lower(p.tokens[p.at].substr(0, 4));
                bool known = false;
                for (unsigned k = 0; k < 8; ++k) known = known || starts_word(p.tokens[p.at], kExtendNames[k]);
                known = known || starts_word(p.tokens[p.at], "lsl");
                if (known) {
                    std::optional<std::int64_t> a;
                    std::string nm;
                    split_modifier(p.take(), nm, a, p);
                    if (nm == "lsl") {
                        option = r->is64 ? 3 : 2;
                    } else {
                        for (unsigned k = 0; k < 8; ++k)
                            if (nm == kExtendNames[k]) option = k;
                    }
                    amount = a.value_or(0);
                }
            }
            const bool x_index = p.sf() && (option & 3) == 3;
            if (r->is64 != x_index) return p.fail("the index register's width does not suit the extend");
            if (!p.lenient && (amount < 0 || amount > 4)) return p.fail("extend amount out of range");
            p.put(13, 3, option);
            p.put(10, 3, static_cast<std::uint64_t>(amount));
            return true;
        }
        case K::LI: {
            if (!p.more()) return p.fail("an operand is missing");
            std::int64_t v;
            try {
                v = eval(p, strip_hash(p.take()));
            } catch (const ParseError& e) {
                return p.fail(e.what());
            }
            const unsigned size = p.sf() ? 64 : 32;
            std::uint64_t u = static_cast<std::uint64_t>(v);
            if (size == 32 && v < 0 && v >= -(1ll << 31)) u &= 0xFFFFFFFFull;
            const std::optional<BitmaskFields> f = encode_bitmask(u, size);
            if (!f) return p.fail("not a bitmask immediate");
            p.put(22, 1, f->n);
            p.put(16, 6, f->immr);
            p.put(10, 6, f->imms);
            return true;
        }
        case K::MWI: {
            if (!p.more()) return p.fail("an operand is missing");
            std::int64_t v;
            try {
                v = eval(p, strip_hash(p.take()));
            } catch (const ParseError& e) {
                return p.fail(e.what());
            }
            if (!p.lenient && (v < 0 || v > 0xFFFF)) return p.fail("immediate out of range (0..65535)");
            unsigned hw = 0;
            if (p.more() && starts_word(p.tokens[p.at], "lsl")) {
                std::string name;
                std::optional<std::int64_t> a;
                split_modifier(p.take(), name, a, p);
                if (!a || *a % 16 != 0 || *a < 0 || *a > (p.sf() ? 48 : 16)) return p.fail("the shift of a move is lsl #0, #16, #32 or #48");
                hw = static_cast<unsigned>(*a / 16);
            }
            p.put(5, 16, static_cast<std::uint64_t>(v));
            p.put(21, 2, hw);
            return true;
        }
        case K::MOVZV:
        case K::MOVNV: {
            if (!p.more()) return p.fail("an operand is missing");
            std::int64_t v;
            try {
                v = eval(p, strip_hash(p.take()));
            } catch (const ParseError& e) {
                return p.fail(e.what());
            }
            const unsigned size = p.sf() ? 64 : 32;
            std::uint64_t u = static_cast<std::uint64_t>(v);
            if (size == 32) {
                if (v < -(1ll << 31) || v > 0xFFFFFFFFll) return p.fail("value does not fit a w register");
                u &= 0xFFFFFFFFull;
            }
            if (o.kind == K::MOVNV) {
                u = ~u;
                if (size == 32) u &= 0xFFFFFFFFull;
            }
            for (unsigned hw = 0; hw < size / 16; ++hw) {
                if ((u & ~(0xFFFFull << (16 * hw))) == 0) {
                    p.put(5, 16, u >> (16 * hw));
                    p.put(21, 2, hw);
                    return true;
                }
            }
            return p.fail("not a single 16-bit chunk");
        }
        case K::LSLI: {
            if (!p.more()) return p.fail("an operand is missing");
            std::int64_t v;
            try {
                v = eval(p, strip_hash(p.take()));
            } catch (const ParseError& e) {
                return p.fail(e.what());
            }
            const std::int64_t size = p.sf() ? 64 : 32;
            if (v < 0 || v >= size) return p.fail("shift amount out of range");
            p.put(16, 6, static_cast<std::uint64_t>((size - v) % size));
            p.put(10, 6, static_cast<std::uint64_t>(size - 1 - v));
            return true;
        }
        case K::SHRI: {
            if (!p.more()) return p.fail("an operand is missing");
            std::int64_t v;
            try {
                v = eval(p, strip_hash(p.take()));
            } catch (const ParseError& e) {
                return p.fail(e.what());
            }
            const std::int64_t size = p.sf() ? 64 : 32;
            if (v < 0 || v >= size) return p.fail("shift amount out of range");
            p.put(16, 6, static_cast<std::uint64_t>(v));
            p.put(10, 6, static_cast<std::uint64_t>(size - 1));
            return true;
        }
        case K::BFX:
        case K::BFIZ: {
            if (p.at + 2 > p.tokens.size()) return p.fail("lsb and width are expected");
            std::int64_t lsb, width;
            try {
                lsb = eval(p, strip_hash(p.take()));
                width = eval(p, strip_hash(p.take()));
            } catch (const ParseError& e) {
                return p.fail(e.what());
            }
            const std::int64_t size = p.sf() ? 64 : 32;
            if (lsb < 0 || lsb >= size || width < 1 || lsb + width > size) return p.fail("lsb or width out of range");
            if (o.kind == K::BFX) {
                p.put(16, 6, static_cast<std::uint64_t>(lsb));
                p.put(10, 6, static_cast<std::uint64_t>(lsb + width - 1));
            } else {
                p.put(16, 6, static_cast<std::uint64_t>((size - lsb) % size));
                p.put(10, 6, static_cast<std::uint64_t>(width - 1));
            }
            return true;
        }
        case K::COND:
        case K::CONDINV: {
            if (!p.more()) return p.fail("a condition is expected");
            const std::optional<unsigned> c = condition_by_name(cb::text::lower(cb::text::trim(p.take())));
            if (!c) return p.fail("a condition is expected");
            p.put(o.pos, o.width, o.kind == K::COND ? *c : (*c ^ 1u));
            return true;
        }
        case K::PR: {
            if (!p.more()) return p.fail("a target is expected");
            std::int64_t target;
            if (!parse_target(p, p.take(), target)) return false;
            const std::int64_t d = target - static_cast<std::int64_t>(p.pc);
            if (!p.lenient) {
                if (d % 4 != 0) return p.fail("the target is not word aligned");
                const std::int64_t lim = 1ll << (o.width + 1);
                if (d < -lim || d >= lim) return p.fail("the target is out of range");
            }
            p.put(o.pos, o.width, static_cast<std::uint64_t>(d >> 2));
            return true;
        }
        case K::PADR:
        case K::PADRP: {
            if (!p.more()) return p.fail("a target is expected");
            std::int64_t target;
            if (!parse_target(p, p.take(), target)) return false;
            std::int64_t d;
            if (o.kind == K::PADR) {
                d = target - static_cast<std::int64_t>(p.pc);
                if (!p.lenient && (d < -(1ll << 20) || d >= (1ll << 20))) return p.fail("the target is out of range");
            } else {
                d = (target >> 12) - (static_cast<std::int64_t>(p.pc) >> 12);
                if (!p.lenient && (d < -(1ll << 20) || d >= (1ll << 20))) return p.fail("the target is out of range");
            }
            p.put(29, 2, static_cast<std::uint64_t>(d & 3));
            p.put(5, 19, static_cast<std::uint64_t>(d >> 2));
            return true;
        }
        case K::MU:
        case K::MT:
        case K::MPRE:
        case K::MPO:
        case K::MPPRE:
        case K::MX: {
            if (!p.more()) return p.fail("a memory operand is expected");
            std::vector<std::string_view> parts;
            bool bang = false;
            if (!unbracket(p.take(), parts, bang)) return p.fail("a memory operand is expected");
            const bool wants_bang = o.kind == K::MPRE || o.kind == K::MPPRE;
            if (bang != wants_bang) return p.fail("the pre-index form does not match");
            if (parts.size() > 2) return p.fail("a memory operand has a base and an offset");
            if (!parse_base(p, parts[0])) return false;
            std::int64_t off = 0;
            if (parts.size() == 2) {
                const std::string_view part = parts[1];
                if (parse_gp(part)) return p.fail("a register offset does not suit this form");
                try {
                    off = eval(p, strip_hash(part));
                } catch (const ParseError& e) {
                    return p.fail(e.what());
                }
            } else if (wants_bang) {
                return p.fail("the pre-index form needs an offset");
            }
            switch (o.kind) {
                case K::MU: {
                    const std::int64_t unit = 1ll << o.param;
                    if (!p.lenient && (off < 0 || off % unit != 0 || (off >> o.param) > 4095)) return p.fail("offset out of range for this form");
                    p.put(10, 12, static_cast<std::uint64_t>(off >> o.param));
                    break;
                }
                case K::MT:
                case K::MPRE:
                    if (!p.lenient && (off < -256 || off > 255)) return p.fail("offset out of range (-256..255)");
                    p.put(12, 9, static_cast<std::uint64_t>(off));
                    break;
                case K::MPO:
                case K::MPPRE: {
                    const std::int64_t unit = 1ll << o.param;
                    if (!p.lenient && (off % unit != 0 || (off / unit) < -64 || (off / unit) > 63)) return p.fail("offset out of range for a pair");
                    p.put(15, 7, static_cast<std::uint64_t>(off / unit));
                    break;
                }
                default:
                    if (off != 0) return p.fail("no offset is allowed here");
                    break;
            }
            return true;
        }
        case K::MPOST:
        case K::MPPOST: {
            if (p.at + 2 > p.tokens.size()) return p.fail("a post-indexed memory operand is expected");
            std::vector<std::string_view> parts;
            bool bang = false;
            if (!unbracket(p.take(), parts, bang) || bang || parts.size() != 1) return p.fail("a post-indexed memory operand is expected");
            if (!parse_base(p, parts[0])) return false;
            std::int64_t off;
            try {
                off = eval(p, strip_hash(p.take()));
            } catch (const ParseError& e) {
                return p.fail(e.what());
            }
            if (o.kind == K::MPOST) {
                if (!p.lenient && (off < -256 || off > 255)) return p.fail("offset out of range (-256..255)");
                p.put(12, 9, static_cast<std::uint64_t>(off));
            } else {
                const std::int64_t unit = 1ll << o.param;
                if (!p.lenient && (off % unit != 0 || (off / unit) < -64 || (off / unit) > 63)) return p.fail("offset out of range for a pair");
                p.put(15, 7, static_cast<std::uint64_t>(off / unit));
            }
            return true;
        }
        case K::MR: {
            if (!p.more()) return p.fail("a memory operand is expected");
            std::vector<std::string_view> parts;
            bool bang = false;
            if (!unbracket(p.take(), parts, bang) || bang || parts.size() < 2 || parts.size() > 3) return p.fail("a register-offset memory operand is expected");
            if (!parse_base(p, parts[0])) return false;
            const std::optional<Reg> index = parse_gp(parts[1]);
            if (!index || index->sp) return p.fail("an index register is expected");
            unsigned option = index->is64 ? 3 : 2;
            std::optional<std::int64_t> amount;
            bool extend_given = false;
            if (parts.size() == 3) {
                std::string nm;
                if (!split_modifier(parts[2], nm, amount, p)) return p.fail("an extend is expected");
                extend_given = true;
                if (nm == "lsl") {
                    if (!index->is64) return p.fail("lsl needs an x index register");
                    option = 3;
                } else {
                    bool known = false;
                    for (unsigned k = 0; k < 8; ++k)
                        if (nm == kExtendNames[k]) {
                            option = k;
                            known = true;
                        }
                    if (!known || (option & 3) < 2) return p.fail("this extend does not suit a memory operand");
                }
            }
            if ((option & 1) != (index->is64 ? 1u : 0u)) return p.fail("the index register's width does not suit the extend");
            unsigned s = 0;
            if (amount) {
                if (*amount == static_cast<std::int64_t>(o.param)) s = 1;
                else if (*amount != 0 || o.param == 0) return p.fail("the index shift is 0 or the access size");
                if (*amount == 0 && o.param == 0) s = 1;
            }
            (void)extend_given;
            p.put(16, 5, index->n);
            p.put(13, 3, option);
            p.put(12, 1, s);
            return true;
        }
        case K::SYSREG: {
            if (!p.more()) return p.fail("a system register is expected");
            const std::string t = cb::text::lower(cb::text::trim(p.take()));
            for (const SysName& n : system_names())
                if (t == n.name) {
                    p.put(5, 15, n.packed);
                    return true;
                }
            // s<op0>_<op1>_c<n>_c<m>_<op2>
            unsigned v[5];
            std::size_t i = 0;
            bool ok = i < t.size() && t[i++] == 's';
            for (unsigned k = 0; ok && k < 5; ++k) {
                if (k == 2 || k == 3) {
                    ok = i < t.size() && t[i++] == 'c';
                }
                unsigned n = 0, digits = 0;
                while (ok && i < t.size() && std::isdigit(static_cast<unsigned char>(t[i]))) {
                    n = n * 10 + static_cast<unsigned>(t[i++] - '0');
                    ++digits;
                }
                ok = ok && digits > 0;
                v[k] = n;
                if (ok && k < 4) ok = i < t.size() && t[i++] == '_';
            }
            if (!ok || i != t.size() || v[0] < 2 || v[0] > 3 || v[1] > 7 || v[2] > 15 || v[3] > 15 || v[4] > 7) return p.fail("unknown system register");
            p.put(5, 15, sysreg(v[0], v[1], v[2], v[3], v[4]));
            return true;
        }
        case K::BAR: {
            if (!p.more()) return p.fail("a barrier option is expected");
            const std::string t = cb::text::lower(cb::text::trim(p.take()));
            for (unsigned c = 0; c < 16; ++c)
                if (bar_text(c) == t) {
                    p.put(o.pos, o.width, c);
                    return true;
                }
            if (t.starts_with('#')) {
                try {
                    const std::int64_t v = eval(p, strip_hash(t));
                    if (v >= 0 && v < 16) {
                        p.put(o.pos, o.width, static_cast<std::uint64_t>(v));
                        return true;
                    }
                } catch (const ParseError&) {
                }
            }
            return p.fail("unknown barrier option");
        }
        case K::PF: {
            if (!p.more()) return p.fail("a prefetch operation is expected");
            const std::string t = cb::text::lower(cb::text::trim(p.take()));
            for (unsigned c = 0; c < 32; ++c)
                if (prefetch_text(c) == t) {
                    p.put(o.pos, o.width, c);
                    return true;
                }
            if (t.starts_with('#')) {
                try {
                    const std::int64_t v = eval(p, strip_hash(t));
                    if (v >= 0 && v < 32) {
                        p.put(o.pos, o.width, static_cast<std::uint64_t>(v));
                        return true;
                    }
                } catch (const ParseError&) {
                }
            }
            return p.fail("unknown prefetch operation");
        }
        case K::LIT: {
            if (!p.more() || cb::text::lower(cb::text::trim(p.take())) != o.literal) return p.fail("'" + o.literal + "' is expected");
            return true;
        }
        case K::TBIT: {
            if (!p.more()) return p.fail("a bit number is expected");
            std::int64_t v;
            try {
                v = eval(p, strip_hash(p.take()));
            } catch (const ParseError& e) {
                return p.fail(e.what());
            }
            if (v < 0 || v > 63 || ((v >= 32) != p.sf())) return p.fail("the bit number does not suit the register's width");
            p.put(19, 5, static_cast<std::uint64_t>(v & 31));
            return true;
        }
        case K::FPIMM: {
            if (!p.more()) return p.fail("an immediate is expected");
            const std::string t(strip_hash(p.take()));
            char* end = nullptr;
            const double v = std::strtod(t.c_str(), &end);
            if (t.empty() || end != t.c_str() + t.size()) return p.fail("a floating-point immediate is expected");
            for (unsigned k = 0; k < 256; ++k) {
                if (expand_fp_imm8(k) == v) {
                    p.put(13, 8, k);
                    return true;
                }
            }
            return p.fail("not an encodable floating-point immediate");
        }
        case K::FBITS: {
            if (!p.more()) return p.fail("the number of fraction bits is expected");
            std::int64_t v;
            try {
                v = eval(p, strip_hash(p.take()));
            } catch (const ParseError& e) {
                return p.fail(e.what());
            }
            if (v < 1 || v > (p.sf() ? 64 : 32)) return p.fail("fraction bits out of range");
            p.put(10, 6, static_cast<std::uint64_t>(64 - v));
            return true;
        }
        default: break;
    }
    return detail::parse_simd_operand(row, o, p.tokens, p.at, p.word, p.arr_set, p.error, p.labels, p.lenient);
}

std::vector<std::string_view> split_operands(std::string_view s) {
    std::vector<std::string_view> out;
    int depth = 0;
    std::size_t start = 0;
    for (std::size_t i = 0; i < s.size(); ++i) {
        const char c = s[i];
        if (c == '[' || c == '{' || c == '(') ++depth;
        else if (c == ']' || c == '}' || c == ')') --depth;
        else if (c == ',' && depth == 0) {
            out.push_back(cb::text::trim(s.substr(start, i - start)));
            start = i + 1;
        }
    }
    const std::string_view last = cb::text::trim(s.substr(start));
    if (!last.empty() || !out.empty()) out.push_back(last);
    return out;
}

}  // namespace

std::uint32_t assemble_word(std::string_view line, std::uint64_t pc, const NameLabels& labels, bool lenient) {
    std::string_view rest = cb::text::trim(line);
    const std::size_t sp = rest.find_first_of(" \t");
    std::string mnemonic = cb::text::lower(sp == std::string_view::npos ? rest : rest.substr(0, sp));
    const std::string_view operands = sp == std::string_view::npos ? std::string_view() : cb::text::trim(rest.substr(sp + 1));
    std::optional<unsigned> cond;
    std::string base = mnemonic;
    if (mnemonic.starts_with("b.")) {
        cond = condition_by_name(mnemonic.substr(2));
        if (!cond) throw ParseError("", 0, 0, "unknown condition in '" + std::string(line) + "'");
        base = "b";
    }
    const std::vector<std::uint16_t>& candidates = detail::rows_named(base);
    if (candidates.empty()) throw ParseError("", 0, 0, "unknown mnemonic '" + mnemonic + "'");
    const std::vector<std::string_view> tokens = split_operands(operands);
    std::string first_error;
    for (const std::uint16_t index : candidates) {
        const Row& row = detail::rows()[index];
        if (row.cond_suffix != cond.has_value()) continue;
        Parse p{row.bits, tokens, 0, pc, labels, lenient, false, false, {}};
        bool ok = true;
        for (const Operand& o : row.operands) {
            if (!parse_operand(row, o, p)) {
                ok = false;
                break;
            }
        }
        if (ok && p.at != tokens.size()) {
            ok = false;
            p.fail("too many operands");
        }
        if (ok && row.fix) p.word = row.fix(p.word);
        if (ok && cond) p.put(0, 4, *cond);
        if (ok && !lenient && (((p.word & row.mask) != row.bits) || (row.when && !row.when(p.word)))) {
            ok = false;
            p.fail("these operands do not make this instruction");
        }
        if (ok) return p.word;
        if (first_error.empty()) first_error = p.error;
    }
    throw ParseError("", 0, 0, "cannot assemble '" + std::string(line) + "': " + (first_error.empty() ? std::string("the operands do not fit") : first_error));
}

Bytes assemble(std::string_view line, std::uint64_t pc, const NameLabels& labels, bool lenient) {
    const std::uint32_t w = assemble_word(line, pc, labels, lenient);
    return {static_cast<std::uint8_t>(w), static_cast<std::uint8_t>(w >> 8), static_cast<std::uint8_t>(w >> 16), static_cast<std::uint8_t>(w >> 24)};
}

}  // namespace cb::decompile::a64
