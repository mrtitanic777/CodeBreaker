// cb-unity: alone
// cb/decompile/a64_simd.cpp -- the vector (Advanced SIMD) rows and operand kinds of the A64 table: structure loads and stores, the vector arithmetic, logical, compare, shift,
// permute and conversion groups, the copy (dup / ins / umov / smov) group, modified immediates, and the scalar forms of the common ones.
#include <bit>
#include <cctype>
#include <format>
#include <stdexcept>

#include "cb/core/text.hpp"
#include "cb/decompile/a64_internal.hpp"

namespace cb::decompile::a64::detail {

namespace {

const char* const kArr[4][2] = {{"8b", "16b"}, {"4h", "8h"}, {"2s", "4s"}, {"1d", "2d"}};
const char kElem[4] = {'b', 'h', 's', 'd'};

std::string bin(unsigned v, unsigned n) {
    std::string s;
    for (unsigned i = n; i-- > 0;) s += ((v >> i) & 1) ? '1' : '0';
    return s;
}

std::string imm_text(std::int64_t v) {
    const std::uint64_t u = v < 0 ? 0 - static_cast<std::uint64_t>(v) : static_cast<std::uint64_t>(v);
    return std::string("#") + (v < 0 ? "-" : "") + (u < 10 ? std::to_string(u) : std::format("0x{:X}", u));
}
std::string vname(unsigned n, const std::string& arr) { return "v" + std::to_string(n) + "." + arr; }
std::string base_reg(unsigned n) { return n == 31 ? std::string("sp") : "x" + std::to_string(n); }
std::string vq(unsigned size, unsigned q) { return kArr[size & 3][q & 1]; }

// ---- predicates ---------------------------------------------------------------------------------------------------------------------------------------------

unsigned size_of(std::uint32_t w) { return bits(w, 22, 2); }
unsigned q_of(std::uint32_t w) { return bits(w, 30, 1); }
bool ls_no_1d(std::uint32_t w) { return !(bits(w, 10, 2) == 3 && !q_of(w)); }
bool no_1d(std::uint32_t w) { return !(size_of(w) == 3 && !q_of(w)); }
bool not_size3(std::uint32_t w) { return size_of(w) != 3; }
bool not_size3_or_1d(std::uint32_t w) { return size_of(w) < 3; }
bool size0(std::uint32_t w) { return size_of(w) == 0; }
bool fp_ok(std::uint32_t w) { return !(bits(w, 22, 1) && !q_of(w)); }   // (2d needs Q)
bool size_lt3_or_pmull(std::uint32_t w) { return size_of(w) < 3; }
bool size01(std::uint32_t w) { return size_of(w) < 2; }
bool size12(std::uint32_t w) { return size_of(w) == 1 || size_of(w) == 2; }
bool narrow_ok(std::uint32_t w) { return size_of(w) < 3; }
bool n_eq_m(std::uint32_t w) { return bits(w, 5, 5) == bits(w, 16, 5); }
std::uint32_t fix_m(std::uint32_t w) { return (w & ~(31u << 16)) | (bits(w, 5, 5) << 16); }

unsigned imm5_size(std::uint32_t w) {   // the element size of a copy instruction: the lowest set bit of imm5
    const unsigned i = bits(w, 16, 5);
    return i == 0 ? 4 : static_cast<unsigned>(std::countr_zero(i));
}
bool copy_ok(std::uint32_t w) { return imm5_size(w) < 4; }
bool copy_ok_q(std::uint32_t w) { return imm5_size(w) < 4 && !(imm5_size(w) == 3 && !q_of(w)); }   // (a 1d arrangement is not a vector)
bool umov_ok(std::uint32_t w) { return imm5_size(w) < 3 ? !q_of(w) : (imm5_size(w) == 3 && q_of(w)); }
bool umov_alias(std::uint32_t w) { return umov_ok(w) && imm5_size(w) >= 2; }
bool umov_plain(std::uint32_t w) { return umov_ok(w) && imm5_size(w) < 2; }
bool smov_ok(std::uint32_t w) { return imm5_size(w) < 3 && (imm5_size(w) == 2 ? q_of(w) : true); }
bool dup_gp_ok(std::uint32_t w) { return copy_ok_q(w) && bits(w, 16, 5) == (1u << imm5_size(w)); }
bool ins_elem_ok(std::uint32_t w) {   // imm4's low bits below the element size must be zero
    const unsigned size = imm5_size(w);
    return size < 4 && (bits(w, 11, 4) & ((1u << size) - 1)) == 0;
}
bool ins_elem_ok_all(std::uint32_t w) { return ins_elem_ok(w) && bits(w, 30, 1) == 1; }

unsigned immh_size(std::uint32_t w) {
    const unsigned h = bits(w, 19, 4);
    return h == 0 ? 4 : static_cast<unsigned>(std::bit_width(h)) - 1;
}
bool shift_ok(std::uint32_t w) { return immh_size(w) < 4 && !(immh_size(w) == 3 && !q_of(w)); }
bool shift_ok_scalar(std::uint32_t w) { return immh_size(w) == 3; }
bool shift_narrow_ok(std::uint32_t w) { return immh_size(w) < 3; }
bool shift_fixed_ok(std::uint32_t w) { return immh_size(w) >= 2 && immh_size(w) < 4 && !(immh_size(w) == 3 && !q_of(w)); }

// ---- operand text -------------------------------------------------------------------------------------------------------------------------------------------

unsigned count_of_list(const Row& row, std::uint32_t w) {
    (void)row;
    switch (bits(w, 12, 4)) {
        case 0: return 4;
        case 2: return 4;
        case 4: return 3;
        case 6: return 3;
        case 7: return 1;
        case 8: return 2;
        default: return 2;   // 10
    }
}

unsigned lane_selem(std::uint32_t w) { return (bits(w, 13, 1) << 1 | bits(w, 21, 1)) + 1; }
unsigned lane_esize_log(unsigned scale, std::uint32_t w) { return scale == 2 && bits(w, 10, 2) == 1 ? 3 : scale; }
unsigned post_bytes(unsigned param, std::uint32_t w) {
    if (param == 0) return count_of_list(Row{}, w) * (q_of(w) ? 16 : 8);
    if (param == 1) return lane_selem(w) << lane_esize_log(bits(w, 14, 2), w);
    return lane_selem(w) << bits(w, 10, 2);
}
bool lane_scale1_ok(std::uint32_t w) { return bits(w, 10, 1) == 0; }
bool lane_scale2_ok(std::uint32_t w) { return bits(w, 10, 2) == 0 || (bits(w, 10, 2) == 1 && bits(w, 12, 1) == 0); }

std::string modimm_value(unsigned param, std::uint32_t w) {
    const unsigned imm8 = (bits(w, 16, 3) << 5) | bits(w, 5, 5);
    const unsigned cmode = bits(w, 12, 4);
    switch (param) {
        case 0: {   // 32-bit shifted
            const unsigned sh = 8 * ((cmode >> 1) & 3);
            return imm_text(imm8) + (sh ? ", lsl #" + std::to_string(sh) : std::string());
        }
        case 1: {   // 16-bit shifted
            const unsigned sh = 8 * ((cmode >> 1) & 1);
            return imm_text(imm8) + (sh ? ", lsl #8" : std::string());
        }
        case 2: return imm_text(imm8) + ", msl #" + std::to_string(cmode & 1 ? 16 : 8);
        case 3: return imm_text(imm8);
        case 4: {
            std::uint64_t v = 0;
            for (unsigned i = 0; i < 8; ++i)
                if ((imm8 >> i) & 1) v |= 0xFFull << (8 * i);
            return std::format("#0x{:X}", v);
        }
        default: return "#" + std::format("{:.8f}", expand_fp_imm8(imm8));
    }
}

}  // namespace

K simd_kind_by_name(std::string_view name) {
    static const std::pair<std::string_view, K> kinds[] = {
        {"V", K::V}, {"VF", K::VF}, {"VB", K::VB}, {"VW", K::VW}, {"VPL", K::VPL}, {"VT", K::VT}, {"VSH", K::VSH}, {"VSHW", K::VSHW}, {"FZ", K::FZ}, {"FZW", K::FZW},
        {"VEL", K::VEL}, {"VELI", K::VEL2}, {"RG", K::RG}, {"RGQ", K::RGQ}, {"BYEL", K::BYEL}, {"BYELF", K::BYELF}, {"SHL", K::SHL}, {"SHR", K::SHR}, {"SHLL", K::SHLL}, {"VMA", K::VMA},
        {"VMI", K::VMI}, {"SLIST", K::SLIST}, {"TLIST", K::TLIST}, {"SPOST", K::SPOST}, {"SLANE", K::SLANE}, {"RLIST", K::RLIST}, {"VTOP", K::VD1}};
    for (const auto& [n, k] : kinds)
        if (n == name) return k;
    throw std::logic_error("a64: unknown operand kind '" + std::string(name) + "'");
}

std::string print_simd_operand(const Row& row, const Operand& o, std::uint32_t w) {
    const unsigned field = o.width ? bits(w, o.pos, o.width) : 0;
    const unsigned q = q_of(w);
    switch (o.kind) {
        case K::V: return vname(field, vq(size_of(w), q));
        case K::VF:
            if (o.param == 1) return vname(field, bits(w, 22, 1) ? "2d" : "2s");
            return vname(field, bits(w, 22, 1) ? (q ? "2d" : "1d") : (q ? "4s" : "2s"));
        case K::VB: return vname(field, q ? "16b" : "8b");
        case K::VW: {
            static const char* const wide[4] = {"8h", "4s", "2d", "?"};
            return vname(field, wide[size_of(w) & 3]);
        }
        case K::VPL: {
            static const char* const pl[4][2] = {{"4h", "8h"}, {"2s", "4s"}, {"1d", "2d"}, {"?", "?"}};
            return vname(field, pl[size_of(w) & 3][q]);
        }
        case K::VT: return vname(field, vq(imm5_size(w) & 3, q));
        case K::VSH: return vname(field, vq(immh_size(w) & 3, q));
        case K::VSHW: {
            static const char* const wide[4] = {"8h", "4s", "2d", "?"};
            return vname(field, wide[immh_size(w) & 3]);
        }
        case K::FZ: return std::string(1, kElem[(o.param == 1 ? imm5_size(w) : size_of(w)) & 3]) + std::to_string(field);
        case K::FZW: return std::string(1, kElem[(size_of(w) + 1) & 3]) + std::to_string(field);
        case K::VEL: {
            const unsigned size = imm5_size(w);
            return "v" + std::to_string(field) + "." + kElem[size & 3] + "[" + std::to_string(bits(w, 16, 5) >> (size + 1)) + "]";
        }
        case K::VEL2: {
            const unsigned size = imm5_size(w);
            return "v" + std::to_string(field) + "." + kElem[size & 3] + "[" + std::to_string(bits(w, 11, 4) >> size) + "]";
        }
        case K::RGQ: return (q ? "x" : "w") + std::string(field == 31 ? "zr" : std::to_string(field));
        case K::RG: return (imm5_size(w) == 3 ? "x" : "w") + std::string(field == 31 ? "zr" : std::to_string(field));
        case K::BYEL: {
            const unsigned size = size_of(w);
            const unsigned h = bits(w, 11, 1), l = bits(w, 21, 1), m = bits(w, 20, 1);
            if (size == 1) return "v" + std::to_string(bits(w, 16, 4)) + ".h[" + std::to_string(h << 2 | l << 1 | m) + "]";
            return "v" + std::to_string(m << 4 | bits(w, 16, 4)) + ".s[" + std::to_string(h << 1 | l) + "]";
        }
        case K::BYELF: {
            const unsigned h = bits(w, 11, 1), l = bits(w, 21, 1), m = bits(w, 20, 1);
            const std::string reg = "v" + std::to_string(m << 4 | bits(w, 16, 4));
            return bits(w, 22, 1) ? reg + ".d[" + std::to_string(h) + "]" : reg + ".s[" + std::to_string(h << 1 | l) + "]";
        }
        case K::SHL: {
            const unsigned size = immh_size(w);
            return imm_text(static_cast<std::int64_t>(bits(w, 16, 7)) - (8 << size));
        }
        case K::SHR: {
            const unsigned size = immh_size(w);
            return imm_text((16 << size) - static_cast<std::int64_t>(bits(w, 16, 7)));
        }
        case K::SHLL: return imm_text(8 << size_of(w));
        case K::VMA: {
            static const char* const a32[2] = {"2s", "4s"};
            static const char* const a16[2] = {"4h", "8h"};
            static const char* const a8[2] = {"8b", "16b"};
            static const char* const a64[2] = {"1d", "2d"};
            const char* const* t = o.param == 1 ? a16 : o.param == 3 ? a8 : o.param == 4 ? a64 : a32;
            if (o.param == 6) return vname(field, "2d");
            return vname(field, t[q]);
        }
        case K::VMI: return modimm_value(o.param, w);
        case K::SLIST: {
            const unsigned n = count_of_list(row, w), t = bits(w, 0, 5);
            std::string s = "{";
            for (unsigned i = 0; i < n; ++i) s += (i ? ", " : "") + vname((t + i) % 32, vq(bits(w, 10, 2), q));
            return s + "}";
        }
        case K::TLIST: {
            const unsigned n = bits(w, 13, 2) + 1, t = bits(w, 5, 5);
            std::string s = "{";
            for (unsigned i = 0; i < n; ++i) s += (i ? ", " : "") + vname((t + i) % 32, "16b");
            return s + "}";
        }
        case K::SLANE: {
            const unsigned scale = o.param, n = lane_selem(w), t = bits(w, 0, 5);
            const char e = kElem[lane_esize_log(scale, w)];
            unsigned idx;
            if (scale == 0) idx = q << 3 | bits(w, 12, 1) << 2 | bits(w, 10, 2);
            else if (scale == 1) idx = q << 2 | bits(w, 12, 1) << 1 | (bits(w, 10, 2) >> 1);
            else idx = bits(w, 10, 2) == 0 ? (q << 1 | bits(w, 12, 1)) : q;
            std::string s = "{";
            for (unsigned i = 0; i < n; ++i) s += (i ? ", " : "") + vname((t + i) % 32, std::string(1, e));
            return s + "}[" + std::to_string(idx) + "]";
        }
        case K::RLIST: {
            const unsigned n = lane_selem(w), t = bits(w, 0, 5);
            std::string s = "{";
            for (unsigned i = 0; i < n; ++i) s += (i ? ", " : "") + vname((t + i) % 32, vq(bits(w, 10, 2), q));
            return s + "}";
        }
        case K::SPOST: {
            const unsigned m = bits(w, 16, 5);
            const std::string base = "[" + base_reg(bits(w, 5, 5)) + "]";
            if (m != 31) return base + ", x" + std::to_string(m);
            return base + ", " + imm_text(post_bytes(o.param, w));
        }
        case K::VD1: return "v" + std::to_string(field) + ".d[1]";
        default: break;
    }
    throw std::logic_error("a64: operand kind not printable");
}

namespace {

// ---- reading operands -----------------------------------------------------------------------------------------------------------------------------------------

struct VReg {
    unsigned n = 0;
    std::string arr;               // "4s" or "" (element form: elem)
    char elem = 0;                 // 'b' 'h' 's' 'd' for `v1.s[2]`
    std::optional<unsigned> index;
};

std::optional<VReg> parse_v(std::string_view s) {
    s = cb::text::trim(s);
    if (s.size() < 2 || (s[0] != 'v' && s[0] != 'V')) return std::nullopt;
    std::size_t i = 1;
    VReg r;
    while (i < s.size() && std::isdigit(static_cast<unsigned char>(s[i]))) r.n = r.n * 10 + static_cast<unsigned>(s[i++] - '0');
    if (i == 1 || r.n > 31) return std::nullopt;
    if (i == s.size()) return r;
    if (s[i] != '.') return std::nullopt;
    ++i;
    std::string rest = cb::text::lower(s.substr(i));
    const std::size_t br = rest.find('[');
    if (br != std::string::npos) {
        if (rest.back() != ']' || br != 1) return std::nullopt;
        r.elem = rest[0];
        unsigned idx = 0;
        for (std::size_t k = br + 1; k + 1 < rest.size(); ++k) {
            if (!std::isdigit(static_cast<unsigned char>(rest[k]))) return std::nullopt;
            idx = idx * 10 + static_cast<unsigned>(rest[k] - '0');
        }
        r.index = idx;
        return r;
    }
    r.arr = rest;
    return r;
}

std::optional<std::pair<unsigned, unsigned>> arrangement(const std::string& a) {   // (size, q)
    for (unsigned size = 0; size < 4; ++size)
        for (unsigned q = 0; q < 2; ++q)
            if (a == kArr[size][q]) return std::pair{size, q};
    return std::nullopt;
}

std::optional<unsigned> elem_size(char c) {
    for (unsigned i = 0; i < 4; ++i)
        if (kElem[i] == c) return i;
    return std::nullopt;
}

std::optional<unsigned> scalar_reg(std::string_view s, char kind) {
    s = cb::text::trim(s);
    if (s.size() < 2 || std::tolower(static_cast<unsigned char>(s[0])) != kind) return std::nullopt;
    unsigned n = 0;
    for (std::size_t i = 1; i < s.size(); ++i) {
        if (!std::isdigit(static_cast<unsigned char>(s[i]))) return std::nullopt;
        n = n * 10 + static_cast<unsigned>(s[i] - '0');
    }
    return n > 31 ? std::nullopt : std::optional<unsigned>(n);
}

struct Put {
    std::uint32_t& word;
    void operator()(unsigned pos, unsigned width, std::uint64_t v) const {
        const std::uint32_t m = width >= 32 ? 0xFFFFFFFFu : ((1u << width) - 1);
        word = (word & ~(m << pos)) | ((static_cast<std::uint32_t>(v) & m) << pos);
    }
};

}  // namespace

bool parse_simd_operand(const Row& row, const Operand& o, const std::vector<std::string_view>& tokens, std::size_t& at, std::uint32_t& word, bool& arr_set, std::string& error,
                        const NameLabels& labels, bool lenient) {
    const auto fail = [&](const std::string& why) {
        if (error.empty()) error = why;
        return false;
    };
    const Put put{word};
    const auto more = [&] { return at < tokens.size(); };
    const auto number = [&](std::string_view t) -> std::int64_t {
        t = cb::text::trim(t);
        if (t.starts_with('#')) t.remove_prefix(1);
        return value(t, labels, lenient);
    };
    // a size and Q fixed by an arrangement must agree with what an earlier operand fixed
    const auto set_arr = [&](unsigned size, unsigned q) -> bool {
        if (arr_set) return size_of(word) == size && q_of(word) == q;
        const K first = row.operands.empty() ? K::U : row.operands[0].kind;
        if ((first == K::FZ || first == K::FZW) && size_of(word) != size) return false;   // (a scalar destination wrote the size)
        put(22, 2, size);
        put(30, 1, q);
        arr_set = true;
        return true;
    };
    if (!more()) return fail("an operand is missing");
    const std::string_view tok = tokens[at];
    switch (o.kind) {
        case K::V: case K::VF: case K::VB: case K::VW: case K::VPL: case K::VT: case K::VSH: case K::VSHW: case K::VMA: {
            const std::optional<VReg> r = parse_v(tok);
            if (!r || r->arr.empty()) return fail("a vector register with an arrangement is expected");
            const auto a = arrangement(r->arr);
            ++at;
            put(o.pos, 5, r->n);
            if (o.kind == K::VMA) {
                if (!a) return fail("bad arrangement");
                const unsigned want = o.param == 0 ? 2 : o.param == 1 ? 1 : o.param == 3 ? 0 : 3;
                if (a->first != want || (o.param == 6 && a->second != 1)) return fail("this arrangement does not suit the immediate");
                put(30, 1, a->second);
                return true;
            }
            if (o.kind == K::VW || o.kind == K::VSHW) {
                const unsigned size = r->arr == "8h" ? 0 : r->arr == "4s" ? 1 : r->arr == "2d" ? 2 : 9;
                if (size == 9) return fail("a wide arrangement is expected");
                if (o.kind == K::VW) return set_arr(size, q_of(word)) || (arr_set && size_of(word) == size);
                put(19, 4, 1ull << size);
                return true;
            }
            if (o.kind == K::VPL) {
                static const char* const pl[3][2] = {{"4h", "8h"}, {"2s", "4s"}, {"1d", "2d"}};
                for (unsigned size = 0; size < 3; ++size)
                    for (unsigned q = 0; q < 2; ++q)
                        if (r->arr == pl[size][q]) return set_arr(size, q);
                return fail("a pairwise-long arrangement is expected");
            }
            if (!a) return fail("bad arrangement");
            if (o.kind == K::VF && o.param == 1) {
                if (r->arr == "2s") { put(22, 1, 0); return true; }
                if (r->arr == "2d") { put(22, 1, 1); return true; }
                return fail("2s or 2d is expected");
            }
            if (o.kind == K::VF) {
                if (a->first == 2) return arr_set ? (bits(word, 22, 1) == 0 && q_of(word) == a->second) : (put(22, 1, 0), put(30, 1, a->second), arr_set = true);
                if (a->first == 3 && a->second == 1) return arr_set ? (bits(word, 22, 1) == 1 && q_of(word) == 1) : (put(22, 1, 1), put(30, 1, 1), arr_set = true);
                return fail("a floating-point arrangement is expected");
            }
            if (o.kind == K::VB) {
                if (a->first != 0) return fail("a byte arrangement is expected");
                put(30, 1, a->second);
                return true;
            }
            if (o.kind == K::VT) {
                put(16, 5, 1ull << a->first);   // (the index bits come with a VEL operand; a bare dup keeps them zero)
                put(30, 1, a->second);
                return true;
            }
            if (o.kind == K::VSH) {
                put(19, 4, 1ull << a->first);
                put(30, 1, a->second);
                return true;
            }
            return set_arr(a->first, a->second);
        }
        case K::FZ:
        case K::FZW: {
            for (unsigned size = 0; size < 4; ++size) {
                const std::optional<unsigned> n = scalar_reg(tok, kElem[size]);
                if (!n) continue;
                ++at;
                put(o.pos, 5, *n);
                if (o.param == 1) {
                    put(16, 5, 1ull << size);
                    return true;
                }
                if (o.kind == K::FZ) {
                    put(22, 2, size);
                    return true;
                }
                if (size == 0) return fail("a wider scalar register is expected");
                put(22, 2, size - 1);
                return true;
            }
            return fail("a scalar register is expected");
        }
        case K::VEL: {
            const std::optional<VReg> r = parse_v(tok);
            if (!r || !r->index) return fail("a vector element is expected");
            const std::optional<unsigned> size = elem_size(r->elem);
            if (!size) return fail("bad element size");
            ++at;
            put(o.pos, 5, r->n);
            if (*r->index >= (16u >> *size)) return fail("element index out of range");
            put(16, 5, (*r->index << (*size + 1)) | (1u << *size));
            return true;
        }
        case K::VEL2: {
            const std::optional<VReg> r = parse_v(tok);
            if (!r || !r->index) return fail("a vector element is expected");
            const std::optional<unsigned> size = elem_size(r->elem);
            if (!size || *size != imm5_size(word)) return fail("the element sizes differ");
            ++at;
            put(o.pos, 5, r->n);
            if (*r->index >= (16u >> *size)) return fail("element index out of range");
            put(11, 4, *r->index << *size);
            return true;
        }
        case K::RG:
        case K::RGQ: {
            const std::string t = cb::text::lower(cb::text::trim(tok));
            bool is64 = false;
            unsigned n = 0;
            if (t == "wzr" || t == "xzr") {
                is64 = t[0] == 'x';
                n = 31;
            } else if ((t[0] == 'w' || t[0] == 'x') && t.size() >= 2) {
                is64 = t[0] == 'x';
                for (std::size_t i = 1; i < t.size(); ++i) {
                    if (!std::isdigit(static_cast<unsigned char>(t[i]))) return fail("a general register is expected");
                    n = n * 10 + static_cast<unsigned>(t[i] - '0');
                }
                if (n > 30) return fail("a general register is expected");
            } else {
                return fail("a general register is expected");
            }
            if (o.kind == K::RG && (imm5_size(word) == 3) != is64) return fail("the register width does not suit the element size");
            ++at;
            put(o.pos, 5, n);
            if (o.kind == K::RGQ) put(30, 1, is64);
            return true;
        }
        case K::BYEL:
        case K::BYELF: {
            const std::optional<VReg> r = parse_v(tok);
            if (!r || !r->index) return fail("a vector element is expected");
            const std::optional<unsigned> size = elem_size(r->elem);
            if (!size) return fail("bad element size");
            ++at;
            const unsigned idx = *r->index;
            if (o.kind == K::BYEL) {
                if (*size == 1) {
                    if (r->n > 15 || idx > 7) return fail("this element does not exist");
                    put(16, 4, r->n);
                    put(11, 1, idx >> 2);
                    put(21, 1, (idx >> 1) & 1);
                    put(20, 1, idx & 1);
                    return arr_set ? size_of(word) == 1 : (put(22, 2, 1), arr_set = true);
                }
                if (*size != 2 || idx > 3) return fail("this element does not exist");
                put(16, 4, r->n & 15);
                put(20, 1, r->n >> 4);
                put(11, 1, idx >> 1);
                put(21, 1, idx & 1);
                return arr_set ? size_of(word) == 2 : (put(22, 2, 2), arr_set = true);
            }
            if (*size == 2) {
                if (idx > 3) return fail("this element does not exist");
                put(11, 1, idx >> 1);
                put(21, 1, idx & 1);
                put(22, 1, 0);
            } else if (*size == 3) {
                if (idx > 1) return fail("this element does not exist");
                put(11, 1, idx);
                put(21, 1, 0);
                put(22, 1, 1);
            } else {
                return fail("a floating-point element is s or d");
            }
            put(16, 4, r->n & 15);
            put(20, 1, r->n >> 4);
            return true;
        }
        case K::SHL:
        case K::SHR:
        case K::SHLL: {
            ++at;
            std::int64_t v;
            try {
                v = number(tok);
            } catch (const ParseError& e) {
                return fail(e.what());
            }
            if (o.kind == K::SHLL) {
                if (v != (8 << size_of(word))) return fail("the shift of shll is the element size");
                return true;
            }
            const unsigned size = immh_size(word);
            if (size > 3) return fail("an arrangement comes before the shift");
            const std::int64_t esize = 8 << size;
            std::int64_t enc;
            if (o.kind == K::SHL) {
                if (!lenient && (v < 0 || v >= esize)) return fail("shift out of range");
                enc = esize + v;
            } else {
                if (!lenient && (v < 1 || v > esize)) return fail("shift out of range");
                enc = 2 * esize - v;
            }
            put(16, 7, static_cast<std::uint64_t>(enc));
            return true;
        }
        case K::VMI: {
            ++at;
            std::int64_t v = 0;
            std::string shift_kind;
            std::optional<std::int64_t> shift;
            if (o.param >= 5) {
                const std::string t(cb::text::trim(tok));
                const std::string_view s = t.starts_with('#') ? std::string_view(t).substr(1) : std::string_view(t);
                char* end = nullptr;
                const std::string tmp(s);
                const double d = std::strtod(tmp.c_str(), &end);
                if (tmp.empty() || end != tmp.c_str() + tmp.size()) return fail("a floating-point immediate is expected");
                for (unsigned k = 0; k < 256; ++k)
                    if (expand_fp_imm8(k) == d) {
                        put(16, 3, k >> 5);
                        put(5, 5, k & 31);
                        return true;
                    }
                return fail("not an encodable floating-point immediate");
            }
            try {
                v = number(tok);
            } catch (const ParseError& e) {
                return fail(e.what());
            }
            if (more()) {
                const std::string t = cb::text::lower(cb::text::trim(tokens[at]));
                if (t.starts_with("lsl") || t.starts_with("msl")) {
                    shift_kind = t.substr(0, 3);
                    std::string_view rest = cb::text::trim(std::string_view(t).substr(3));
                    if (rest.starts_with('#')) rest.remove_prefix(1);
                    try {
                        shift = value(rest, labels, lenient);
                    } catch (const ParseError& e) {
                        return fail(e.what());
                    }
                    ++at;
                }
            }
            unsigned imm8 = 0;
            if (o.param == 4) {
                const std::uint64_t u = static_cast<std::uint64_t>(v);
                for (unsigned i = 0; i < 8; ++i) {
                    const unsigned b = static_cast<unsigned>(u >> (8 * i)) & 0xFF;
                    if (b != 0 && b != 0xFF) return fail("each byte of a 64-bit vector immediate is 00 or FF");
                    if (b) imm8 |= 1u << i;
                }
            } else {
                if (v < 0 || v > 255) return fail("immediate out of range (0..255)");
                imm8 = static_cast<unsigned>(v);
            }
            put(16, 3, imm8 >> 5);
            put(5, 5, imm8 & 31);
            const std::uint32_t cmode = bits(word, 12, 4);
            const std::int64_t sh = shift.value_or(0);
            if (o.param == 0) {
                if (shift_kind == "msl" || sh % 8 != 0 || sh < 0 || sh > 24) return fail("the shift is lsl #0, #8, #16 or #24");
                put(13, 2, static_cast<std::uint64_t>(sh / 8));
            } else if (o.param == 1) {
                if (shift_kind == "msl" || (sh != 0 && sh != 8)) return fail("the shift is lsl #0 or #8");
                put(13, 1, sh == 8);
            } else if (o.param == 2) {
                if (shift_kind != "msl" || (sh != 8 && sh != 16)) return fail("the shift is msl #8 or #16");
                put(12, 1, sh == 16);
            } else if (shift) {
                return fail("no shift here");
            }
            (void)cmode;
            return true;
        }
        case K::SLIST: {
            const std::string_view t = cb::text::trim(tok);
            if (t.size() < 2 || t.front() != '{' || t.back() != '}') return fail("a register list is expected");
            ++at;
            const std::vector<std::string_view> regs = cb::text::split_trim(t.substr(1, t.size() - 2), ',');
            std::optional<VReg> first;
            unsigned prev = 0;
            for (std::size_t i = 0; i < regs.size(); ++i) {
                const std::optional<VReg> r = parse_v(regs[i]);
                if (!r || r->arr.empty()) return fail("a vector register is expected");
                if (i == 0) first = r;
                else if (r->n != (prev + 1) % 32 || r->arr != first->arr) return fail("the registers of a list are consecutive and alike");
                prev = r->n;
            }
            if (!first || regs.empty() || regs.size() > 4) return fail("a list of one to four registers is expected");
            const auto a = arrangement(first->arr);
            if (!a) return fail("bad arrangement");
            put(0, 5, first->n);
            put(10, 2, a->first);
            put(30, 1, a->second);
            // the opcode names the number of registers: it is fixed by the row, and must agree
            return count_of_list(row, word) == regs.size() || fail("the number of registers does not suit this instruction");
        }
        case K::SLANE: {
            std::string_view t = cb::text::trim(tok);
            const std::size_t close = t.find("}[");
            if (t.size() < 5 || t.front() != '{' || close == std::string_view::npos || t.back() != ']') return fail("a lane list is expected");
            ++at;
            const std::vector<std::string_view> regs = cb::text::split_trim(t.substr(1, close - 1), ',');
            const std::string idx_text(t.substr(close + 2, t.size() - close - 3));
            unsigned idx = 0;
            for (const char c : idx_text) {
                if (!std::isdigit(static_cast<unsigned char>(c))) return fail("a lane index is expected");
                idx = idx * 10 + static_cast<unsigned>(c - '0');
            }
            unsigned first = 0, prev = 0;
            char elem = 0;
            for (std::size_t i = 0; i < regs.size(); ++i) {
                const std::optional<VReg> r = parse_v(regs[i]);
                if (!r || r->arr.size() != 1) return fail("a vector element type is expected");
                if (i == 0) {
                    first = r->n;
                    elem = r->arr[0];
                } else if (r->n != (prev + 1) % 32 || r->arr[0] != elem) {
                    return fail("the registers of a list are consecutive and alike");
                }
                prev = r->n;
            }
            if (regs.empty() || regs.size() != lane_selem(word)) return fail("the number of registers does not suit this instruction");
            const unsigned scale = o.param;
            put(0, 5, first);
            if (scale == 0) {
                if (elem != 'b' || idx > 15) return fail("a b lane 0..15 is expected");
                put(30, 1, idx >> 3);
                put(12, 1, (idx >> 2) & 1);
                put(10, 2, idx & 3);
            } else if (scale == 1) {
                if (elem != 'h' || idx > 7) return fail("an h lane 0..7 is expected");
                put(30, 1, idx >> 2);
                put(12, 1, (idx >> 1) & 1);
                put(10, 2, (idx & 1) << 1);
            } else if (elem == 's' && idx < 4) {
                put(10, 2, 0);
                put(30, 1, idx >> 1);
                put(12, 1, idx & 1);
            } else if (elem == 'd' && idx < 2) {
                put(10, 2, 1);
                put(30, 1, idx);
                put(12, 1, 0);
            } else {
                return fail("an s lane 0..3 or a d lane 0..1 is expected");
            }
            return true;
        }
        case K::RLIST: {
            const std::string_view t = cb::text::trim(tok);
            if (t.size() < 2 || t.front() != '{' || t.back() != '}') return fail("a register list is expected");
            ++at;
            const std::vector<std::string_view> regs = cb::text::split_trim(t.substr(1, t.size() - 2), ',');
            std::optional<VReg> first;
            unsigned prev = 0;
            for (std::size_t i = 0; i < regs.size(); ++i) {
                const std::optional<VReg> r = parse_v(regs[i]);
                if (!r || r->arr.empty()) return fail("a vector register is expected");
                if (i == 0) first = r;
                else if (r->n != (prev + 1) % 32 || r->arr != first->arr) return fail("the registers of a list are consecutive and alike");
                prev = r->n;
            }
            if (!first || regs.size() != lane_selem(word)) return fail("the number of registers does not suit this instruction");
            const auto a = arrangement(first->arr);
            if (!a) return fail("bad arrangement");
            put(0, 5, first->n);
            put(10, 2, a->first);
            put(30, 1, a->second);
            return true;
        }
        case K::TLIST: {
            const std::string_view t = cb::text::trim(tok);
            if (t.size() < 2 || t.front() != '{' || t.back() != '}') return fail("a register list is expected");
            ++at;
            const std::vector<std::string_view> regs = cb::text::split_trim(t.substr(1, t.size() - 2), ',');
            if (regs.empty() || regs.size() > 4) return fail("a list of one to four registers is expected");
            unsigned first = 0, prev = 0;
            for (std::size_t i = 0; i < regs.size(); ++i) {
                const std::optional<VReg> r = parse_v(regs[i]);
                if (!r || r->arr != "16b") return fail("a 16b vector register is expected");
                if (i == 0) first = r->n;
                else if (r->n != (prev + 1) % 32) return fail("the registers of a list are consecutive");
                prev = r->n;
            }
            put(5, 5, first);
            put(13, 2, regs.size() - 1);
            return true;
        }
        case K::SPOST: {
            if (at + 2 > tokens.size()) return fail("a post-indexed memory operand is expected");
            std::string_view base = cb::text::trim(tokens[at]);
            if (base.size() < 3 || base.front() != '[' || base.back() != ']') return fail("a post-indexed memory operand is expected");
            base = cb::text::trim(base.substr(1, base.size() - 2));
            const std::string b = cb::text::lower(base);
            unsigned n = 0;
            if (b == "sp") {
                n = 31;
            } else if (b.size() >= 2 && b[0] == 'x') {
                for (std::size_t i = 1; i < b.size(); ++i) {
                    if (!std::isdigit(static_cast<unsigned char>(b[i]))) return fail("a base register is expected");
                    n = n * 10 + static_cast<unsigned>(b[i] - '0');
                }
                if (n > 30) return fail("a base register is expected");
            } else {
                return fail("a base register is expected");
            }
            at += 1;
            const std::string_view off = cb::text::trim(tokens[at++]);
            put(5, 5, n);
            if (!off.empty() && (off[0] == 'x' || off[0] == 'X')) {
                unsigned m = 0;
                for (std::size_t i = 1; i < off.size(); ++i) {
                    if (!std::isdigit(static_cast<unsigned char>(off[i]))) return fail("a post-index register is expected");
                    m = m * 10 + static_cast<unsigned>(off[i] - '0');
                }
                if (m > 30) return fail("a post-index register is expected");
                put(16, 5, m);
                return true;
            }
            std::int64_t v;
            try {
                v = number(off);
            } catch (const ParseError& e) {
                return fail(e.what());
            }
            if (!lenient && v != static_cast<std::int64_t>(post_bytes(o.param, word))) return fail("the post-index is the number of bytes transferred");
            put(16, 5, 31);
            return true;
        }
        case K::VD1: {
            const std::optional<VReg> r = parse_v(tok);
            if (!r || r->elem != 'd' || !r->index || *r->index != 1) return fail("v<n>.d[1] is expected");
            ++at;
            put(o.pos, 5, r->n);
            return true;
        }
        default: break;
    }
    return fail("unsupported operand");
}

// ---- the rows -----------------------------------------------------------------------------------------------------------------------------------------------------

void add_simd_rows(Builder& b) {
    // ---- structure loads and stores: multiple structures ----
    const struct {
        const char* ld;
        const char* st;
        unsigned opcode;
        bool no_1d;
    } lists[] = {{"ld4", "st4", 0, true}, {"ld1", "st1", 2, false}, {"ld3", "st3", 4, true}, {"ld1", "st1", 6, false}, {"ld1", "st1", 7, false}, {"ld2", "st2", 8, true},
                 {"ld1", "st1", 10, false}};
    for (const auto& l : lists) {
        for (unsigned load = 0; load < 2; ++load) {
            const char* name = load ? l.ld : l.st;
            const std::string op = bin(l.opcode, 4);
            const When w = l.no_1d ? ls_no_1d : nullptr;
            b.add(name, Op::SimdLdStMulti, std::string("0 q 0011000 ") + (load ? "1" : "0") + " 000000 " + op + " zz nnnnn ttttt", "SLIST,MX", Flow::None, w);
            b.add(name, Op::SimdLdStMulti, std::string("0 q 0011001 ") + (load ? "1" : "0") + " 0 mmmmm " + op + " zz nnnnn ttttt", "SLIST,SPOST", Flow::None, w);
        }
    }

    // ---- structure loads and stores: one lane, and load and replicate ----
    for (unsigned scale = 0; scale < 3; ++scale) {
        for (unsigned high = 0; high < 2; ++high) {
            for (unsigned r = 0; r < 2; ++r) {
                const unsigned selem = high * 2 + r + 1;
                for (unsigned load = 0; load < 2; ++load) {
                    const std::string name = std::string(load ? "ld" : "st") + std::to_string(selem);
                    const std::string head = "0 q 001101";
                    const std::string tail = " " + bin(scale << 1 | high, 3) + " s zz nnnnn ttttt";
                    const When ok = scale == 0 ? nullptr : scale == 1 ? lane_scale1_ok : lane_scale2_ok;
                    const std::string kind = "SLANE" + std::to_string(scale);
                    b.add(name, Op::SimdLdStSingle, head + "0 " + bin(load, 1) + bin(r, 1) + " 00000" + tail, kind + ",MX", Flow::None, ok);
                    b.add(name, Op::SimdLdStSingle, head + "1 " + bin(load, 1) + bin(r, 1) + " mmmmm" + tail, kind + ",SPOST1", Flow::None, ok);
                }
            }
        }
    }
    for (unsigned high = 0; high < 2; ++high) {
        for (unsigned r = 0; r < 2; ++r) {
            const std::string name = "ld" + std::to_string(high * 2 + r + 1) + "r";
            b.add(name, Op::SimdLdStSingle, "0 q 0011010 1 " + bin(r, 1) + " 00000 11" + bin(high, 1) + " 0 zz nnnnn ttttt", "RLIST,MX");
            b.add(name, Op::SimdLdStSingle, "0 q 0011011 1 " + bin(r, 1) + " mmmmm 11" + bin(high, 1) + " 0 zz nnnnn ttttt", "RLIST,SPOST2");
        }
    }

    // ---- three registers, same type ----
    struct Three {
        const char* name;
        unsigned u, opcode;
        When when;
    };
    const Three ints[] = {
        {"shadd", 0, 0, not_size3_or_1d}, {"sqadd", 0, 1, no_1d}, {"srhadd", 0, 2, not_size3_or_1d}, {"shsub", 0, 4, not_size3_or_1d}, {"sqsub", 0, 5, no_1d}, {"cmgt", 0, 6, no_1d},
        {"cmge", 0, 7, no_1d}, {"sshl", 0, 8, no_1d}, {"sqshl", 0, 9, no_1d}, {"srshl", 0, 10, no_1d}, {"sqrshl", 0, 11, no_1d}, {"smax", 0, 12, not_size3_or_1d},
        {"smin", 0, 13, not_size3_or_1d}, {"sabd", 0, 14, not_size3_or_1d}, {"saba", 0, 15, not_size3_or_1d}, {"add", 0, 16, no_1d}, {"cmtst", 0, 17, no_1d},
        {"mla", 0, 18, not_size3_or_1d}, {"mul", 0, 19, not_size3_or_1d}, {"smaxp", 0, 20, not_size3_or_1d}, {"sminp", 0, 21, not_size3_or_1d}, {"sqdmulh", 0, 22, size12},
        {"addp", 0, 23, no_1d}, {"uhadd", 1, 0, not_size3_or_1d}, {"uqadd", 1, 1, no_1d}, {"urhadd", 1, 2, not_size3_or_1d}, {"uhsub", 1, 4, not_size3_or_1d},
        {"uqsub", 1, 5, no_1d}, {"cmhi", 1, 6, no_1d}, {"cmhs", 1, 7, no_1d}, {"ushl", 1, 8, no_1d}, {"uqshl", 1, 9, no_1d}, {"urshl", 1, 10, no_1d},
        {"uqrshl", 1, 11, no_1d}, {"umax", 1, 12, not_size3_or_1d}, {"umin", 1, 13, not_size3_or_1d}, {"uabd", 1, 14, not_size3_or_1d}, {"uaba", 1, 15, not_size3_or_1d},
        {"sub", 1, 16, no_1d}, {"cmeq", 1, 17, no_1d}, {"mls", 1, 18, not_size3_or_1d}, {"pmul", 1, 19, size0}, {"umaxp", 1, 20, not_size3_or_1d},
        {"uminp", 1, 21, not_size3_or_1d}, {"sqrdmulh", 1, 22, size12}};
    for (const Three& t : ints)
        b.add(t.name, Op::SimdThree, "0 q " + bin(t.u, 1) + " 01110 zz 1 mmmmm " + bin(t.opcode, 5) + " 1 nnnnn ddddd", "Vd,Vn,Vm", Flow::None, t.when);
    // logical (the arrangement is bytes; the size field picks the operation)
    const struct {
        const char* name;
        unsigned u, size;
    } logic[] = {{"and", 0, 0}, {"bic", 0, 1}, {"orr", 0, 2}, {"orn", 0, 3}, {"eor", 1, 0}, {"bsl", 1, 1}, {"bit", 1, 2}, {"bif", 1, 3}};
    for (const auto& l : logic) {
        if (std::string_view(l.name) == "orr") b.add("mov", Op::SimdThree, "0 q 0 01110 10 1 mmmmm 00011 1 nnnnn ddddd", "VBd,VBn", Flow::None, n_eq_m).fix = fix_m;
        b.add(l.name, Op::SimdThree, "0 q " + bin(l.u, 1) + " 01110 " + bin(l.size, 2) + " 1 mmmmm 00011 1 nnnnn ddddd", "VBd,VBn,VBm");
    }
    // floating point (a: bit 23, sz: bit 22)
    const struct {
        const char* name;
        unsigned u, a, opcode;
    } fps[] = {{"fmaxnm", 0, 0, 24}, {"fmla", 0, 0, 25}, {"fadd", 0, 0, 26}, {"fmulx", 0, 0, 27}, {"fcmeq", 0, 0, 28}, {"fmax", 0, 0, 30}, {"frecps", 0, 0, 31}, {"fminnm", 0, 1, 24},
               {"fmls", 0, 1, 25}, {"fsub", 0, 1, 26}, {"fmin", 0, 1, 30}, {"frsqrts", 0, 1, 31}, {"fmaxnmp", 1, 0, 24}, {"faddp", 1, 0, 26}, {"fmul", 1, 0, 27},
               {"fcmge", 1, 0, 28}, {"facge", 1, 0, 29}, {"fmaxp", 1, 0, 30}, {"fdiv", 1, 0, 31}, {"fminnmp", 1, 1, 24}, {"fabd", 1, 1, 26}, {"fcmgt", 1, 1, 28},
               {"facgt", 1, 1, 29}, {"fminp", 1, 1, 30}};
    for (const auto& f : fps)
        b.add(f.name, Op::SimdThree, "0 q " + bin(f.u, 1) + " 01110 " + bin(f.a, 1) + " z 1 mmmmm " + bin(f.opcode, 5) + " 1 nnnnn ddddd", "VFd,VFn,VFm", Flow::None, fp_ok);

    // ---- three registers, different sizes ----
    const struct {
        const char* name;
        unsigned opcode;
        int form;   // 0 long: Vw,V,V   1 wide: Vw,Vw,V   2 narrow: V,Vw,Vw
        bool unsigned_too;
        const char* un;
    } diff[] = {{"saddl", 0, 0, true, "uaddl"},  {"saddw", 1, 1, true, "uaddw"}, {"ssubl", 2, 0, true, "usubl"},   {"ssubw", 3, 1, true, "usubw"},   {"addhn", 4, 2, true, "raddhn"},
                {"sabal", 5, 0, true, "uabal"},  {"subhn", 6, 2, true, "rsubhn"}, {"sabdl", 7, 0, true, "uabdl"},   {"smlal", 8, 0, true, "umlal"},   {"sqdmlal", 9, 0, false, nullptr},
                {"smlsl", 10, 0, true, "umlsl"}, {"sqdmlsl", 11, 0, false, nullptr}, {"smull", 12, 0, true, "umull"}, {"sqdmull", 13, 0, false, nullptr}};
    for (const auto& d : diff) {
        for (unsigned u = 0; u < (d.unsigned_too ? 2u : 1u); ++u) {
            for (unsigned q = 0; q < 2; ++q) {
                const std::string name = std::string(u ? d.un : d.name) + (q ? "2" : "");
                const std::string spec = d.form == 0 ? "VWd,Vn,Vm" : d.form == 1 ? "VWd,VWn,Vm" : "Vd,VWn,VWm";
                b.add(name, Op::SimdThreeDiff, "0 " + bin(q, 1) + " " + bin(u, 1) + " 01110 zz 1 mmmmm " + bin(d.opcode, 4) + " 00 nnnnn ddddd", spec, Flow::None,
                      d.opcode == 9 || d.opcode == 11 || d.opcode == 13 ? size12 : narrow_ok);
            }
        }
    }

    // ---- two registers, miscellaneous ----
    const struct {
        const char* name;
        unsigned u, opcode;
        const char* spec;
        When when;
    } misc[] = {{"rev64", 0, 0, "Vd,Vn", not_size3_or_1d}, {"rev16", 0, 1, "Vd,Vn", size0}, {"cls", 0, 4, "Vd,Vn", not_size3_or_1d}, {"cnt", 0, 5, "VBd,VBn", size0},
                {"cmgt", 0, 8, "Vd,Vn,=#0", no_1d}, {"cmeq", 0, 9, "Vd,Vn,=#0", no_1d}, {"cmlt", 0, 10, "Vd,Vn,=#0", no_1d}, {"abs", 0, 11, "Vd,Vn", no_1d},
                {"sqabs", 0, 7, "Vd,Vn", no_1d}, {"suqadd", 0, 3, "Vd,Vn", no_1d}, {"rev32", 1, 0, "Vd,Vn", size01}, {"clz", 1, 4, "Vd,Vn", not_size3_or_1d},
                {"cmge", 1, 8, "Vd,Vn,=#0", no_1d}, {"cmle", 1, 9, "Vd,Vn,=#0", no_1d}, {"neg", 1, 11, "Vd,Vn", no_1d}, {"sqneg", 1, 7, "Vd,Vn", no_1d},
                {"usqadd", 1, 3, "Vd,Vn", no_1d}};
    for (const auto& m : misc) b.add(m.name, Op::SimdTwoMisc, "0 q " + bin(m.u, 1) + " 01110 zz 10000 " + bin(m.opcode, 5) + " 10 nnnnn ddddd", m.spec, Flow::None, m.when);
    b.add("mvn", Op::SimdTwoMisc, "0 q 1 01110 00 10000 00101 10 nnnnn ddddd", "VBd,VBn");
    b.add("rbit", Op::SimdTwoMisc, "0 q 1 01110 01 10000 00101 10 nnnnn ddddd", "VBd,VBn");
    // pairwise long
    const struct {
        const char* name;
        unsigned u, opcode;
    } plong[] = {{"saddlp", 0, 2}, {"sadalp", 0, 6}, {"uaddlp", 1, 2}, {"uadalp", 1, 6}};
    for (const auto& p : plong) b.add(p.name, Op::SimdTwoMisc, "0 q " + bin(p.u, 1) + " 01110 zz 10000 " + bin(p.opcode, 5) + " 10 nnnnn ddddd", "VPLd,Vn", Flow::None, not_size3);
    // narrowing
    const struct {
        const char* name;
        unsigned u, opcode;
    } narrow[] = {{"xtn", 0, 18}, {"sqxtn", 0, 20}, {"sqxtun", 1, 18}, {"uqxtn", 1, 20}};
    for (const auto& n : narrow)
        for (unsigned q = 0; q < 2; ++q)
            b.add(std::string(n.name) + (q ? "2" : ""), Op::SimdTwoMisc, "0 " + bin(q, 1) + " " + bin(n.u, 1) + " 01110 zz 10000 " + bin(n.opcode, 5) + " 10 nnnnn ddddd", "Vd,VWn", Flow::None,
                  narrow_ok);
    for (unsigned q = 0; q < 2; ++q) b.add(std::string("shll") + (q ? "2" : ""), Op::SimdTwoMisc, "0 " + bin(q, 1) + " 1 01110 zz 10000 10011 10 nnnnn ddddd", "VWd,Vn,SHLL", Flow::None, narrow_ok);
    // floating point (a: bit 23, sz: bit 22)
    const struct {
        const char* name;
        unsigned u, a, opcode;
        const char* spec;
    } fmisc[] = {{"frintn", 0, 0, 24, "VFd,VFn"}, {"frintm", 0, 0, 25, "VFd,VFn"}, {"fcvtns", 0, 0, 26, "VFd,VFn"}, {"fcvtms", 0, 0, 27, "VFd,VFn"}, {"fcvtas", 0, 0, 28, "VFd,VFn"},
                 {"scvtf", 0, 0, 29, "VFd,VFn"}, {"frintp", 0, 1, 24, "VFd,VFn"}, {"frintz", 0, 1, 25, "VFd,VFn"}, {"fcvtps", 0, 1, 26, "VFd,VFn"}, {"fcvtzs", 0, 1, 27, "VFd,VFn"},
                 {"frecpe", 0, 1, 29, "VFd,VFn"}, {"fcmgt", 0, 1, 12, "VFd,VFn,=#0.0"}, {"fcmeq", 0, 1, 13, "VFd,VFn,=#0.0"}, {"fcmlt", 0, 1, 14, "VFd,VFn,=#0.0"},
                 {"fabs", 0, 1, 15, "VFd,VFn"}, {"frinta", 1, 0, 24, "VFd,VFn"}, {"frintx", 1, 0, 25, "VFd,VFn"}, {"fcvtnu", 1, 0, 26, "VFd,VFn"}, {"fcvtmu", 1, 0, 27, "VFd,VFn"},
                 {"fcvtau", 1, 0, 28, "VFd,VFn"}, {"ucvtf", 1, 0, 29, "VFd,VFn"}, {"frinti", 1, 1, 25, "VFd,VFn"}, {"fcvtpu", 1, 1, 26, "VFd,VFn"}, {"fcvtzu", 1, 1, 27, "VFd,VFn"},
                 {"frsqrte", 1, 1, 29, "VFd,VFn"}, {"fcmge", 1, 1, 12, "VFd,VFn,=#0.0"}, {"fcmle", 1, 1, 13, "VFd,VFn,=#0.0"}, {"fneg", 1, 1, 15, "VFd,VFn"},
                 {"fsqrt", 1, 1, 31, "VFd,VFn"}};
    for (const auto& f : fmisc)
        b.add(f.name, Op::SimdTwoMisc, "0 q " + bin(f.u, 1) + " 01110 " + bin(f.a, 1) + " z 10000 " + bin(f.opcode, 5) + " 10 nnnnn ddddd", f.spec, Flow::None, fp_ok);

    // ---- across lanes ----
    const struct {
        const char* name;
        unsigned u, opcode;
        bool wide;
    } across[] = {{"saddlv", 0, 3, true}, {"smaxv", 0, 10, false}, {"sminv", 0, 26, false}, {"addv", 0, 27, false}, {"uaddlv", 1, 3, true}, {"umaxv", 1, 10, false}, {"uminv", 1, 26, false}};
    for (const auto& a : across)
        b.add(a.name, Op::SimdAcross, "0 q " + bin(a.u, 1) + " 01110 zz 11000 " + bin(a.opcode, 5) + " 10 nnnnn ddddd", a.wide ? "FZWd,Vn" : "FZd,Vn", Flow::None,
              [](std::uint32_t w) { return size_of(w) < 3 && !(size_of(w) == 2 && !q_of(w)); });
    const struct {
        const char* name;
        unsigned a, opcode;
    } facross[] = {{"fmaxnmv", 0, 12}, {"fmaxv", 0, 15}, {"fminnmv", 1, 12}, {"fminv", 1, 15}};
    for (const auto& a : facross)
        b.add(a.name, Op::SimdAcross, "0 1 1 01110 " + bin(a.a, 1) + " 0 11000 " + bin(a.opcode, 5) + " 10 nnnnn ddddd", "FSd,VFn");

    // ---- permute, extract, table lookup ----
    const struct {
        const char* name;
        unsigned opcode;
    } perm[] = {{"uzp1", 1}, {"trn1", 2}, {"zip1", 3}, {"uzp2", 5}, {"trn2", 6}, {"zip2", 7}};
    for (const auto& p : perm) b.add(p.name, Op::SimdPermute, "0 q 001110 zz 0 mmmmm 0 " + bin(p.opcode, 3) + " 10 nnnnn ddddd", "Vd,Vn,Vm", Flow::None, no_1d);
    b.add("ext", Op::SimdExt, "0 q 101110 00 0 mmmmm 0 iiii 0 nnnnn ddddd", "VBd,VBn,VBm,Ui", Flow::None, [](std::uint32_t w) { return q_of(w) || bits(w, 14, 1) == 0; });
    b.add("tbl", Op::SimdTable, "0 q 001110 00 0 mmmmm 0 ll 0 00 nnnnn ddddd", "VBd,TLIST,VBm");
    b.add("tbx", Op::SimdTable, "0 q 001110 00 0 mmmmm 0 ll 1 00 nnnnn ddddd", "VBd,TLIST,VBm");

    // ---- copy ----
    b.add("dup", Op::SimdCopy, "0 q 0 01110000 iiiii 0 0000 1 nnnnn ddddd", "VTd,VELn", Flow::None, copy_ok_q);
    b.add("dup", Op::SimdCopy, "0 q 0 01110000 iiiii 0 0001 1 nnnnn ddddd", "VTd,RGn", Flow::None, dup_gp_ok);
    b.add("smov", Op::SimdCopy, "0 q 0 01110000 iiiii 0 0101 1 nnnnn ddddd", "RGQd,VELn", Flow::None, smov_ok);
    b.add("mov", Op::SimdCopy, "0 q 0 01110000 iiiii 0 0111 1 nnnnn ddddd", "RGQd,VELn", Flow::None, umov_alias);
    b.add("ins", Op::SimdCopy, "0 1 0 01110000 iiiii 0 0011 1 nnnnn ddddd", "VELd,RGn", Flow::None, copy_ok);   // (the assembler takes ins for what is printed as mov)
    b.add("ins", Op::SimdCopy, "0 1 1 01110000 iiiii 0 jjjj 1 nnnnn ddddd", "VELd,VELIn", Flow::None, ins_elem_ok);
    b.add("umov", Op::SimdCopy, "0 q 0 01110000 iiiii 0 0111 1 nnnnn ddddd", "RGQd,VELn", Flow::None, umov_plain);
    b.add("umov", Op::SimdCopy, "0 q 0 01110000 iiiii 0 0111 1 nnnnn ddddd", "RGQd,VELn", Flow::None, umov_alias);   // (the assembler takes umov for s and d too)
    b.add("mov", Op::SimdCopy, "0 1 0 01110000 iiiii 0 0011 1 nnnnn ddddd", "VELd,RGn", Flow::None, copy_ok);
    b.add("mov", Op::SimdCopy, "0 1 1 01110000 iiiii 0 jjjj 1 nnnnn ddddd", "VELd,VELIn", Flow::None, ins_elem_ok);
    b.add("mov", Op::SimdScalarCopy, "0 1 0 11110000 iiiii 0 0000 1 nnnnn ddddd", "FZ1d,VELn", Flow::None, copy_ok);

    // ---- modified immediate ----
    const struct {
        const char* name;
        unsigned op;
        const char* cmode;   // the fixed bits of cmode (x = a bit the shift sets)
        const char* spec;
    } modimm[] = {{"movi", 0, "0xx0", "VMA0d,VMI0"}, {"orr", 0, "0xx1", "VMA0d,VMI0"}, {"movi", 0, "10x0", "VMA1d,VMI1"}, {"orr", 0, "10x1", "VMA1d,VMI1"},
                  {"movi", 0, "110x", "VMA0d,VMI2"}, {"movi", 0, "1110", "VMA3d,VMI3"}, {"fmov", 0, "1111", "VMA0d,VMI5"}, {"mvni", 1, "0xx0", "VMA0d,VMI0"},
                  {"bic", 1, "0xx1", "VMA0d,VMI0"}, {"mvni", 1, "10x0", "VMA1d,VMI1"}, {"bic", 1, "10x1", "VMA1d,VMI1"}, {"mvni", 1, "110x", "VMA0d,VMI2"}};
    for (const auto& m : modimm) {
        std::string cm = m.cmode;
        for (char& c : cm)
            if (c == 'x') c = 'k';
        b.add(m.name, Op::SimdModImm, "0 q " + bin(m.op, 1) + " 0111100000 xxx " + std::string(cm) + " 0 1 yyyyy ddddd", m.spec);
    }
    b.add("movi", Op::SimdModImm, "0 q 1 0111100000 xxx 1110 0 1 yyyyy ddddd", "VMA6d,VMI4", Flow::None, [](std::uint32_t w) { return q_of(w) == 1; });
    b.add("movi", Op::SimdModImm, "0 0 1 0111100000 xxx 1110 0 1 yyyyy ddddd", "FDd,VMI4");
    b.add("fmov", Op::SimdModImm, "0 1 1 0111100000 xxx 1111 0 1 yyyyy ddddd", "VMA6d,VMI6");

    // ---- shift by immediate ----
    const struct {
        const char* name;
        unsigned u, opcode;
        bool right;
    } shifts[] = {{"sshr", 0, 0, true}, {"ssra", 0, 2, true}, {"srshr", 0, 4, true}, {"srsra", 0, 6, true}, {"shl", 0, 10, false}, {"ushr", 1, 0, true}, {"usra", 1, 2, true},
                  {"urshr", 1, 4, true}, {"ursra", 1, 6, true}, {"sri", 1, 8, true}, {"sli", 1, 10, false}, {"sqshlu", 1, 12, false}, {"sqshl", 0, 14, false}, {"uqshl", 1, 14, false}};
    for (const auto& s : shifts) {
        b.add(s.name, Op::SimdShift, "0 q " + bin(s.u, 1) + " 011110 hhhh mmm " + bin(s.opcode, 5) + " 1 nnnnn ddddd", std::string("VSHd,VSHn,") + (s.right ? "SHR" : "SHL"), Flow::None, shift_ok);
        b.add(s.name, Op::SimdScalarShift, "0 1 " + bin(s.u, 1) + " 111110 1hhh mmm " + bin(s.opcode, 5) + " 1 nnnnn ddddd", std::string("FDd,FDn,") + (s.right ? "SHR" : "SHL"));
    }
    const struct {
        const char* name;
        unsigned u, opcode;
    } shn[] = {{"shrn", 0, 16}, {"rshrn", 0, 17}, {"sqshrn", 0, 18}, {"sqrshrn", 0, 19}, {"sqshrun", 1, 16}, {"sqrshrun", 1, 17}, {"uqshrn", 1, 18}, {"uqrshrn", 1, 19}};
    for (const auto& s : shn)
        for (unsigned q = 0; q < 2; ++q)
            b.add(std::string(s.name) + (q ? "2" : ""), Op::SimdShift, "0 " + bin(q, 1) + " " + bin(s.u, 1) + " 011110 hhhh mmm " + bin(s.opcode, 5) + " 1 nnnnn ddddd", "VSHd,VSHWn,SHR", Flow::None,
                  shift_narrow_ok);
    for (unsigned u = 0; u < 2; ++u) {
        for (unsigned q = 0; q < 2; ++q) {
            const std::string sfx = q ? "2" : "";
            // sxtl / uxtl: the shift is zero
            b.add(std::string(u ? "uxtl" : "sxtl") + sfx, Op::SimdShift, "0 " + bin(q, 1) + " " + bin(u, 1) + " 011110 hhhh 000 10100 1 nnnnn ddddd", "VSHWd,VSHn", Flow::None,
                  [](std::uint32_t w) { return shift_narrow_ok(w) && bits(w, 19, 4) == (1u << immh_size(w)); });
            b.add(std::string(u ? "ushll" : "sshll") + sfx, Op::SimdShift, "0 " + bin(q, 1) + " " + bin(u, 1) + " 011110 hhhh mmm 10100 1 nnnnn ddddd", "VSHWd,VSHn,SHL", Flow::None, shift_narrow_ok);
        }
    }
    const struct {
        const char* name;
        unsigned u, opcode;
    } fixed[] = {{"scvtf", 0, 28}, {"ucvtf", 1, 28}, {"fcvtzs", 0, 31}, {"fcvtzu", 1, 31}};
    for (const auto& f : fixed) {
        b.add(f.name, Op::SimdShift, "0 q " + bin(f.u, 1) + " 011110 hhhh mmm " + bin(f.opcode, 5) + " 1 nnnnn ddddd", "VSHd,VSHn,SHR", Flow::None, shift_fixed_ok);
        b.add(f.name, Op::SimdScalarShift, "0 1 " + bin(f.u, 1) + " 111110 01hh mmm " + bin(f.opcode, 5) + " 1 nnnnn ddddd", "FSd,FSn,SHR");
        b.add(f.name, Op::SimdScalarShift, "0 1 " + bin(f.u, 1) + " 111110 1hhh mmm " + bin(f.opcode, 5) + " 1 nnnnn ddddd", "FDd,FDn,SHR");
    }

    // ---- by element ----
    const struct {
        const char* name;
        unsigned u, opcode;
        int form;   // 0 same, 1 long (name gets 2 for Q)
    } byel[] = {{"mla", 1, 0, 0}, {"mls", 1, 4, 0}, {"mul", 0, 8, 0}, {"sqdmulh", 0, 12, 0}, {"sqrdmulh", 0, 13, 0}, {"smlal", 0, 2, 1}, {"umlal", 1, 2, 1}, {"smlsl", 0, 6, 1},
                {"umlsl", 1, 6, 1}, {"smull", 0, 10, 1}, {"umull", 1, 10, 1}, {"sqdmull", 0, 11, 1}, {"sqdmlal", 0, 3, 1}, {"sqdmlsl", 0, 7, 1}};
    for (const auto& e : byel) {
        const auto ok = [](std::uint32_t w) { return size12(w); };
        if (e.form == 0) {
            b.add(e.name, Op::SimdByElem, "0 q " + bin(e.u, 1) + " 01111 zz l m mmmm " + bin(e.opcode, 4) + " h 0 nnnnn ddddd", "Vd,Vn,BYELm", Flow::None, ok);
        } else {
            for (unsigned q = 0; q < 2; ++q)
                b.add(std::string(e.name) + (q ? "2" : ""), Op::SimdByElem, "0 " + bin(q, 1) + " " + bin(e.u, 1) + " 01111 zz l m mmmm " + bin(e.opcode, 4) + " h 0 nnnnn ddddd", "VWd,Vn,BYELm",
                      Flow::None, ok);
        }
    }
    const struct {
        const char* name;
        unsigned u, opcode;
    } fbyel[] = {{"fmla", 0, 1}, {"fmls", 0, 5}, {"fmul", 0, 9}, {"fmulx", 1, 9}};
    for (const auto& f : fbyel)
        b.add(f.name, Op::SimdByElem, "0 q " + bin(f.u, 1) + " 01111 1 z l m mmmm " + bin(f.opcode, 4) + " h 0 nnnnn ddddd", "VFd,VFn,BYELFm", Flow::None,
              [](std::uint32_t w) { return fp_ok(w) && !(bits(w, 22, 1) && bits(w, 21, 1)); });

    // ---- scalar forms ----
    const struct {
        const char* name;
        unsigned u, opcode;
    } sthree[] = {{"add", 0, 16}, {"sub", 1, 16}, {"cmeq", 1, 17}, {"cmgt", 0, 6}, {"cmge", 0, 7}, {"cmhi", 1, 6}, {"cmhs", 1, 7}, {"cmtst", 0, 17}, {"sshl", 0, 8}, {"ushl", 1, 8}};
    for (const auto& s : sthree)
        b.add(s.name, Op::SimdScalarThree, "01 " + bin(s.u, 1) + " 11110 11 1 mmmmm " + bin(s.opcode, 5) + " 1 nnnnn ddddd", "FDd,FDn,FDm");
    const struct {
        const char* name;
        unsigned u, a, opcode;
    } sfp[] = {{"scvtf", 0, 0, 29}, {"ucvtf", 1, 0, 29}, {"fcvtzs", 0, 1, 27}, {"fcvtzu", 1, 1, 27}, {"fcvtns", 0, 0, 26}, {"fcvtnu", 1, 0, 26}, {"fcvtms", 0, 0, 27}, {"fcvtmu", 1, 0, 27},
               {"fcvtas", 0, 0, 28}, {"fcvtau", 1, 0, 28}, {"fcvtps", 0, 1, 26}, {"fcvtpu", 1, 1, 26}, {"frecpe", 0, 1, 29}, {"frsqrte", 1, 1, 29}};
    for (const auto& s : sfp)
        for (unsigned z = 0; z < 2; ++z)
            b.add(s.name, Op::SimdScalarTwoMisc, "01 " + bin(s.u, 1) + " 11110 " + bin(s.a, 1) + bin(z, 1) + " 10000 " + bin(s.opcode, 5) + " 10 nnnnn ddddd", z ? "FDd,FDn" : "FSd,FSn");
    const struct {
        const char* name;
        unsigned u, a, opcode;
    } sfp3[] = {{"fmulx", 0, 0, 27}, {"fcmeq", 0, 0, 28}, {"frecps", 0, 0, 31}, {"frsqrts", 0, 1, 31}, {"fcmge", 1, 0, 28}, {"facge", 1, 0, 29}, {"fabd", 1, 1, 26}, {"fcmgt", 1, 1, 28},
                {"facgt", 1, 1, 29}};
    for (const auto& s : sfp3)
        for (unsigned z = 0; z < 2; ++z)
            b.add(s.name, Op::SimdScalarThree, "01 " + bin(s.u, 1) + " 11110 " + bin(s.a, 1) + bin(z, 1) + " 1 mmmmm " + bin(s.opcode, 5) + " 1 nnnnn ddddd", z ? "FDd,FDn,FDm" : "FSd,FSn,FSm");
    const struct {
        const char* name;
        unsigned u, opcode;
    } scmp[] = {{"cmgt", 0, 8}, {"cmeq", 0, 9}, {"cmlt", 0, 10}, {"cmge", 1, 8}, {"cmle", 1, 9}};
    for (const auto& c : scmp) b.add(c.name, Op::SimdScalarTwoMisc, "01 " + bin(c.u, 1) + " 11110 11 10000 " + bin(c.opcode, 5) + " 10 nnnnn ddddd", "FDd,FDn,=#0");
    b.add("addp", Op::SimdScalarPair, "01 0 11110 11 11000 11011 10 nnnnn ddddd", "FDd,Vn");
    b.add("faddp", Op::SimdScalarPair, "01 1 11110 0 0 11000 01101 10 nnnnn ddddd", "FSd,VF1n");
    b.add("faddp", Op::SimdScalarPair, "01 1 11110 0 1 11000 01101 10 nnnnn ddddd", "FDd,VF1n");
    b.add("abs", Op::SimdScalarTwoMisc, "01 0 11110 11 10000 01011 10 nnnnn ddddd", "FDd,FDn");
    b.add("neg", Op::SimdScalarTwoMisc, "01 1 11110 11 10000 01011 10 nnnnn ddddd", "FDd,FDn");
    // between the register files: the upper half
    b.add("fmov", Op::FpInt, "1 0 0 11110 10 1 01 110 000000 nnnnn ddddd", "Xd,VTOPn");
    b.add("fmov", Op::FpInt, "1 0 0 11110 10 1 01 111 000000 nnnnn ddddd", "VTOPd,Xn");
}

}  // namespace cb::decompile::a64::detail
