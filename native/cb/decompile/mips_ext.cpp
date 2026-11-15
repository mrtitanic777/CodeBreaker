// cb-unity: alone (file-local names that other files in a batch also use)
// cb/decompile/mips_ext.cpp -- VU0 macro mode, LQC2/SQC2, EXT/INS and the VFPU (mips_ext.hpp).

#include "cb/decompile/mips_ext.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <deque>
#include <format>
#include <string>
#include <vector>

#include "cb/core/text.hpp"

namespace cb::decompile::mips::ext {

namespace {

[[noreturn]] void refuse(std::string_view name, const std::string& why) { throw ParseError("", 0, 0, std::string(name) + " (" + why + ")"); }

std::string gpr(unsigned n) { return std::string(register_name(n)); }

std::int32_t sign16(std::uint32_t v) { return static_cast<std::int16_t>(v & 0xFFFF); }

// ---- the VFPU's registers ---------------------------------------------------------------------------------------------
//
// A register is a 7-bit field: bits 0-1 the column, 2-4 the matrix, bit 5 the orientation (a row vector, or a transposed matrix), bit 6 the
// start row (a single's bits 5-6 are its row). It is written with a letter -- S single, C column vector, R row vector, M matrix, E transposed
// matrix -- and the matrix, column and row digits, so every field has one name and every name one field.

enum class Reg : std::uint8_t { Single, Vector, Matrix };

unsigned size_of(std::uint32_t word) { return 1 + ((word >> 7) & 1) + (((word >> 15) & 1) << 1); }
char size_letter(unsigned size) { return size == 1 ? 's' : size == 2 ? 'p' : size == 3 ? 't' : 'q'; }

std::string vfpu_name(Reg kind, unsigned size, unsigned r) {
    const unsigned matrix = (r >> 2) & 7, column = r & 3;
    if (kind == Reg::Single || size == 1) return std::format("S{}{}{}", matrix, column, (r >> 5) & 3);
    const unsigned row = ((r >> 6) & 1) != 0 ? (size == 3 ? 1u : 2u) : 0u;
    const bool flipped = ((r >> 5) & 1) != 0;
    const char letter = kind == Reg::Vector ? (flipped ? 'R' : 'C') : (flipped ? 'E' : 'M');
    return std::format("{}{}{}{}", letter, matrix, column, row);
}

std::optional<unsigned> vfpu_parse(std::string_view t, Reg kind, unsigned size) {
    t = text::trim(t);
    if (t.size() != 4) return std::nullopt;
    const auto digit = [&](std::size_t i) { return static_cast<unsigned>(t[i] - '0'); };
    for (std::size_t i = 1; i < 4; ++i)
        if (t[i] < '0' || t[i] > '9') return std::nullopt;
    const unsigned matrix = digit(1), column = digit(2), row = digit(3);
    if (matrix > 7 || column > 3) return std::nullopt;
    if (kind == Reg::Single || size == 1) {
        if (t[0] != 'S' || row > 3) return std::nullopt;
        return column | matrix << 2 | row << 5;
    }
    unsigned flipped = 0;
    if (kind == Reg::Vector) {
        if (t[0] != 'C' && t[0] != 'R') return std::nullopt;
        flipped = t[0] == 'R' ? 1 : 0;
    } else {
        if (t[0] != 'M' && t[0] != 'E') return std::nullopt;
        flipped = t[0] == 'E' ? 1 : 0;
    }
    const unsigned high = size == 3 ? 1u : 2u;
    if (row != 0 && row != high) return std::nullopt;
    return column | matrix << 2 | flipped << 5 | (row != 0 ? 1u : 0u) << 6;
}

// ---- the table of VFPU instructions that follow a pattern -------------------------------------------------------------------
//
// `ops` names the operands: a register (V a vector of the instruction's size, S a single, T P Q a triple, pair or quad, M a matrix of the
// instruction's size) then the field it is in (d bits 0-6, s bits 8-14, t bits 16-22); K a 5-bit constant in bits 16-20; G a general register
// (t bits 16-20). Every bit of the word that is no operand is fixed: `match` has it. The size (bits 7 and 15) is an operand of the sized
// ones; a fixed-size one says it.

struct VfpuOp {
    std::string_view name;
    std::uint32_t match;
    std::string_view ops;
    char size;   // 0: the word's own; 's' 'p' 't' 'q': this one; 'n': the instruction has none
};

constexpr auto kVfpu = std::to_array<VfpuOp>({
    {"vadd", 0x60000000, "Vd Vs Vt", 0},   {"vsub", 0x60800000, "Vd Vs Vt", 0},    {"vsbn", 0x61000000, "Vd Vs Vt", 0},   {"vdiv", 0x63800000, "Vd Vs Vt", 0},
    {"vmul", 0x64000000, "Vd Vs Vt", 0},   {"vdot", 0x64800000, "Sd Vs Vt", 0},    {"vscl", 0x65000000, "Vd Vs St", 0},   {"vhdp", 0x66000000, "Sd Vs Vt", 0},
    {"vcrs", 0x66800000, "Td Ts Tt", 't'}, {"vdet", 0x67000000, "Sd Ps Pt", 'p'},  {"vmin", 0x6D000000, "Vd Vs Vt", 0},   {"vmax", 0x6D800000, "Vd Vs Vt", 0},
    {"vscmp", 0x6E800000, "Vd Vs Vt", 0},  {"vsge", 0x6F000000, "Vd Vs Vt", 0},    {"vslt", 0x6F800000, "Vd Vs Vt", 0},
    {"vmov", 0xD0000000, "Vd Vs", 0},      {"vabs", 0xD0010000, "Vd Vs", 0},       {"vneg", 0xD0020000, "Vd Vs", 0},      {"vidt", 0xD0030000, "Vd", 0},
    {"vsat0", 0xD0040000, "Vd Vs", 0},     {"vsat1", 0xD0050000, "Vd Vs", 0},      {"vzero", 0xD0060000, "Vd", 0},        {"vone", 0xD0070000, "Vd", 0},
    {"vrcp", 0xD0100000, "Vd Vs", 0},      {"vrsq", 0xD0110000, "Vd Vs", 0},       {"vsin", 0xD0120000, "Vd Vs", 0},      {"vcos", 0xD0130000, "Vd Vs", 0},
    {"vexp2", 0xD0140000, "Vd Vs", 0},     {"vlog2", 0xD0150000, "Vd Vs", 0},      {"vsqrt", 0xD0160000, "Vd Vs", 0},     {"vasin", 0xD0170000, "Vd Vs", 0},
    {"vnrcp", 0xD0180000, "Vd Vs", 0},     {"vnsin", 0xD01A0000, "Vd Vs", 0},      {"vrexp2", 0xD01C0000, "Vd Vs", 0},    {"vfad", 0xD0460000, "Sd Vs", 0},
    {"vavg", 0xD0470000, "Sd Vs", 0},      {"vcst", 0xD0600000, "Vd K", 0},        {"vf2in", 0xD2000000, "Vd Vs K", 0},   {"vf2iz", 0xD2200000, "Vd Vs K", 0},
    {"vf2iu", 0xD2400000, "Vd Vs K", 0},   {"vf2id", 0xD2600000, "Vd Vs K", 0},    {"vi2f", 0xD2800000, "Vd Vs K", 0},    {"vmmul", 0xF0000000, "Md Ms Mt", 0},
    {"vmmov", 0xF3800000, "Md Ms", 0},     {"vmidt", 0xF3830000, "Md", 0},         {"vmzero", 0xF3860000, "Md", 0},       {"vmone", 0xF3870000, "Md", 0},
    {"mfv", 0x48600000, "Gt Sd", 'n'},     {"mtv", 0x48E00000, "Gt Sd", 'n'},      {"vnop", 0xFFFF0000, "", 'n'},         {"vsync", 0xFFFF0320, "", 'n'},
    {"vflush", 0xFFFF040D, "", 'n'},
});

std::uint32_t field_bits(char kind, char field) {
    if (kind == 'K') return 0x1F0000;
    if (kind == 'G') return field == 't' ? 0x1F0000u : field == 's' ? 0x3E00000u : 0xF800u;
    return field == 'd' ? 0x7Fu : field == 's' ? 0x7F00u : 0x7F0000u;
}

// The bits of the word that are operands, and the size's.
std::uint32_t operand_bits(const VfpuOp& e) {
    std::uint32_t bits = 0;
    for (const std::string_view& token : text::split(e.ops, ' ')) {
        if (token.empty()) continue;
        bits |= field_bits(token[0], token.size() > 1 ? token[1] : 'd');
    }
    if (e.size == 0) bits |= 0x8080;
    return bits;
}

std::uint32_t size_bits(char size) {
    switch (size) {
        case 'p': return 0x80;
        case 't': return 0x8000;
        case 'q': return 0x8080;
        default: return 0;
    }
}

struct VfpuMasks {
    std::vector<std::uint32_t> mask, match;
};
const VfpuMasks& vfpu_masks() {
    static const VfpuMasks m = [] {
        VfpuMasks out;
        for (const VfpuOp& e : kVfpu) {
            out.mask.push_back(~operand_bits(e));
            out.match.push_back(e.match | size_bits(e.size));
        }
        return out;
    }();
    return m;
}

unsigned vfpu_field(std::uint32_t word, char field) { return field == 'd' ? word & 0x7F : field == 's' ? (word >> 8) & 0x7F : (word >> 16) & 0x7F; }

std::string vfpu_text(const VfpuOp& e, std::uint32_t word) {
    const unsigned size = e.size == 0 ? size_of(word) : e.size == 's' ? 1 : e.size == 'p' ? 2 : e.size == 't' ? 3 : e.size == 'q' ? 4 : 1;
    std::string out(e.name);
    if (e.size != 'n') out += std::format(".{}", size_letter(size));
    std::string sep = " ";
    for (const std::string_view& token : text::split(e.ops, ' ')) {
        if (token.empty()) continue;
        const char kind = token[0];
        std::string operand;
        switch (kind) {
            case 'K': operand = std::to_string((word >> 16) & 0x1F); break;
            case 'G': operand = gpr((word >> 16) & 31); break;
            case 'S': operand = vfpu_name(Reg::Single, 1, vfpu_field(word, token[1])); break;
            case 'T': operand = vfpu_name(Reg::Vector, 3, vfpu_field(word, token[1])); break;
            case 'P': operand = vfpu_name(Reg::Vector, 2, vfpu_field(word, token[1])); break;
            case 'Q': operand = vfpu_name(Reg::Vector, 4, vfpu_field(word, token[1])); break;
            case 'M': operand = vfpu_name(Reg::Matrix, size, vfpu_field(word, token[1])); break;
            default: operand = vfpu_name(Reg::Vector, size, vfpu_field(word, token[1])); break;
        }
        out += sep + operand;
        sep = ", ";
    }
    return out;
}

std::optional<std::uint32_t> vfpu_assemble(std::string_view name, const std::vector<std::string>& ops) {
    const std::size_t dot = name.find('.');
    const std::string_view base = name.substr(0, dot);
    const std::string_view suffix = dot == std::string_view::npos ? std::string_view() : name.substr(dot + 1);
    for (std::size_t i = 0; i < kVfpu.size(); ++i) {
        const VfpuOp& e = kVfpu[i];
        if (e.name != base) continue;
        unsigned size = 1;
        std::uint32_t word = e.match;
        if (e.size == 'n') {
            if (!suffix.empty()) refuse(name, "this instruction has no size");
        } else {
            if (suffix.size() != 1 || std::string_view("sptq").find(suffix[0]) == std::string_view::npos) refuse(name, "the size is .s, .p, .t or .q");
            if (e.size != 0 && e.size != suffix[0]) refuse(name, std::format("this instruction is .{} only", e.size));
            size = suffix[0] == 's' ? 1 : suffix[0] == 'p' ? 2 : suffix[0] == 't' ? 3 : 4;
            word |= size_bits(suffix[0]);
        }
        const std::vector<std::string_view> tokens = [&] {
            std::vector<std::string_view> t;
            for (const std::string_view& x : text::split(e.ops, ' '))
                if (!x.empty()) t.push_back(x);
            return t;
        }();
        if (ops.size() != tokens.size()) refuse(name, std::format("{} operands where {} are written", ops.size(), tokens.size()));
        for (std::size_t k = 0; k < tokens.size(); ++k) {
            const char kind = tokens[k][0];
            if (kind == 'K') {
                const auto v = text::parse_int<unsigned>(text::trim(ops[k]));
                if (!v || *v > 31) refuse(name, "a constant is 0 to 31");
                word |= *v << 16;
            } else if (kind == 'G') {
                const auto r = register_number(ops[k]);
                if (!r) refuse(name, "'" + ops[k] + "' is not a register");
                word |= *r << 16;
            } else {
                const char field = tokens[k][1];
                const Reg reg = kind == 'S' ? Reg::Single : kind == 'M' ? Reg::Matrix : Reg::Vector;
                const unsigned want = kind == 'T' ? 3 : kind == 'P' ? 2 : kind == 'Q' ? 4 : size;
                const auto r = vfpu_parse(ops[k], reg, want);
                if (!r) refuse(name, "'" + ops[k] + "' is not a VFPU register of this size");
                word |= field == 'd' ? *r : field == 's' ? *r << 8 : *r << 16;
            }
        }
        return word;
    }
    return std::nullopt;
}

// ---- the VU0's macro mode ------------------------------------------------------------------------------------------------
//
// COP2 with bit 25 set: bits 24-21 the destination fields (x y z w), 20-16 FT, 15-11 FS, 10-6 FD, 5-0 the function. These are the first
// group of functions (0 to 0x3B); the second group (0x3C-0x3F, selected by FD and the low bits: the accumulator forms, moves, loads and stores
// through VI, divides, FTOI and ITOF) is not decoded and stays data.

enum class Vu : std::uint8_t {
    Fd,       // fd, fs, ft with a destination
    Q,        // fd, fs, $Q (ft is zero)
    I,        // fd, fs, $I
    Vi,       // id, is, it (no destination)
    ViImm,    // it, is, imm5 in FD's place
    Call,     // a 15-bit microprogram address in bits 6-20
    CallR,    // no operands
};
struct VuFn {
    std::string_view name;
    Vu kind;
};

std::array<VuFn, 0x3A> make_vu_table() {
    std::array<VuFn, 0x3A> t{};
    constexpr std::array<std::string_view, 4> axes = {"x", "y", "z", "w"};
    constexpr std::array<std::string_view, 7> groups = {"vadd", "vsub", "vmadd", "vmsub", "vmax", "vmini", "vmul"};
    static std::deque<std::string> names;
    for (std::size_t g = 0; g < groups.size(); ++g)
        for (std::size_t a = 0; a < 4; ++a) {
            names.push_back(std::string(groups[g]) + std::string(axes[a]));
            t[g * 4 + a] = {names.back(), Vu::Fd};
        }
    t[0x1C] = {"vmulq", Vu::Q};      t[0x1D] = {"vmaxi", Vu::I};      t[0x1E] = {"vmuli", Vu::I};     t[0x1F] = {"vminii", Vu::I};
    t[0x20] = {"vaddq", Vu::Q};      t[0x21] = {"vmaddq", Vu::Q};     t[0x22] = {"vaddi", Vu::I};     t[0x23] = {"vmaddi", Vu::I};
    t[0x24] = {"vsubq", Vu::Q};      t[0x25] = {"vmsubq", Vu::Q};     t[0x26] = {"vsubi", Vu::I};     t[0x27] = {"vmsubi", Vu::I};
    t[0x28] = {"vadd", Vu::Fd};      t[0x29] = {"vmadd", Vu::Fd};     t[0x2A] = {"vmul", Vu::Fd};     t[0x2B] = {"vmax", Vu::Fd};
    t[0x2C] = {"vsub", Vu::Fd};      t[0x2D] = {"vmsub", Vu::Fd};     t[0x2E] = {"vopmsub", Vu::Fd};  t[0x2F] = {"vmini", Vu::Fd};
    t[0x30] = {"viadd", Vu::Vi};     t[0x31] = {"visub", Vu::Vi};     t[0x32] = {"viaddi", Vu::ViImm};
    t[0x34] = {"viand", Vu::Vi};     t[0x35] = {"vior", Vu::Vi};      t[0x38] = {"vcallms", Vu::Call}; t[0x39] = {"vcallmsr", Vu::CallR};
    return t;
}
const std::array<VuFn, 0x3A>& vu_table() {
    static const std::array<VuFn, 0x3A> t = make_vu_table();
    return t;
}

std::string vf(unsigned n) { return std::format("$vf{}", n); }
std::string vi(unsigned n) { return std::format("$vi{}", n); }

std::string dest_text(unsigned dest) {
    std::string s;
    if (dest & 8) s += 'x';
    if (dest & 4) s += 'y';
    if (dest & 2) s += 'z';
    if (dest & 1) s += 'w';
    return s.empty() ? "0" : s;
}

// The function number the word names, or -1 when the VU0 table does not hold it or a field it needs to be zero is not.
int vu_function(std::uint32_t word) {
    if ((word >> 25) != 0x25) return -1;   // COP2 (0x12) with bit 25
    const unsigned fn = word & 0x3F;
    if (fn >= vu_table().size() || vu_table()[fn].name.empty()) return -1;
    const unsigned dest = (word >> 21) & 15, ft = (word >> 16) & 31, fs = (word >> 11) & 31, fd = (word >> 6) & 31;
    switch (vu_table()[fn].kind) {
        case Vu::Fd: return static_cast<int>(fn);
        case Vu::Q:
        case Vu::I: return ft == 0 ? static_cast<int>(fn) : -1;
        case Vu::Vi: return dest == 0 && ft < 16 && fs < 16 && fd < 16 ? static_cast<int>(fn) : -1;
        case Vu::ViImm: return dest == 0 && ft < 16 && fs < 16 ? static_cast<int>(fn) : -1;
        case Vu::Call: return dest == 0 ? static_cast<int>(fn) : -1;
        case Vu::CallR: return (word & 0x1FFFFC0) == 0 ? static_cast<int>(fn) : -1;
    }
    return -1;
}

std::string vu_text(const Insn& insn) {
    const std::uint32_t w = insn.word;
    const VuFn& f = vu_table()[w & 0x3F];
    const unsigned dest = (w >> 21) & 15, ft = (w >> 16) & 31, fs = (w >> 11) & 31, fd = (w >> 6) & 31;
    switch (f.kind) {
        case Vu::Fd: return std::format("{}.{} {}, {}, {}", f.name, dest_text(dest), vf(fd), vf(fs), vf(ft));
        case Vu::Q: return std::format("{}.{} {}, {}, $Q", f.name, dest_text(dest), vf(fd), vf(fs));
        case Vu::I: return std::format("{}.{} {}, {}, $I", f.name, dest_text(dest), vf(fd), vf(fs));
        case Vu::Vi: return std::format("{} {}, {}, {}", f.name, vi(fd), vi(fs), vi(ft));
        case Vu::ViImm: return std::format("{} {}, {}, {}", f.name, vi(ft), vi(fs), static_cast<std::int32_t>(fd ^ 16) - 16);
        case Vu::Call: return std::format("{} 0x{:X}", f.name, (w >> 6) & 0x7FFF);
        case Vu::CallR: return std::string(f.name);
    }
    return std::string(f.name);
}

std::optional<unsigned> parse_vf(std::string_view t) {
    t = text::trim(t);
    if (!t.starts_with("$vf")) return std::nullopt;
    const auto n = text::parse_int<unsigned>(t.substr(3));
    return n && *n < 32 ? n : std::nullopt;
}
std::optional<unsigned> parse_vi(std::string_view t) {
    t = text::trim(t);
    if (!t.starts_with("$vi")) return std::nullopt;
    const auto n = text::parse_int<unsigned>(t.substr(3));
    return n && *n < 16 ? n : std::nullopt;
}

std::optional<std::uint32_t> vu_assemble(std::string_view name, const std::vector<std::string>& ops, std::uint32_t pc, const NameLabels& labels) {
    const std::size_t dot = name.find('.');
    const std::string_view base = name.substr(0, dot);
    for (std::size_t fn = 0; fn < vu_table().size(); ++fn) {
        const VuFn& f = vu_table()[fn];
        if (f.name.empty() || f.name != base) continue;
        std::uint32_t word = (0x25u << 25) | static_cast<std::uint32_t>(fn);
        const auto need = [&](std::size_t n) {
            if (ops.size() != n) refuse(name, std::format("{} operands where {} are written", ops.size(), n));
        };
        const auto vfr = [&](std::size_t i) {
            const auto r = parse_vf(ops[i]);
            if (!r) refuse(name, "'" + ops[i] + "' is not a $vf register");
            return *r;
        };
        const auto vir = [&](std::size_t i) {
            const auto r = parse_vi(ops[i]);
            if (!r) refuse(name, "'" + ops[i] + "' is not a $vi register");
            return *r;
        };
        const auto destination = [&] {
            if (dot == std::string_view::npos) refuse(name, "the destination fields (.xyzw) are missing");
            const std::string_view d = name.substr(dot + 1);
            if (d == "0") return 0u;
            unsigned bits = 0;
            int last = -1;
            for (const char c : d) {
                const int at = c == 'x' ? 0 : c == 'y' ? 1 : c == 'z' ? 2 : c == 'w' ? 3 : -1;
                if (at < 0) refuse(name, "a destination is x, y, z or w");
                if (at <= last) refuse(name, "the destination fields are in the order xyzw");
                last = at;
                bits |= 8u >> at;
            }
            return bits;
        };
        switch (f.kind) {
            case Vu::Fd: need(3); word |= (destination() << 21) | (vfr(0) << 6) | (vfr(1) << 11) | (vfr(2) << 16); break;
            case Vu::Q:
                need(3);
                if (text::trim(ops[2]) != "$Q") refuse(name, "the third operand is $Q");
                word |= (destination() << 21) | (vfr(0) << 6) | (vfr(1) << 11);
                break;
            case Vu::I:
                need(3);
                if (text::trim(ops[2]) != "$I") refuse(name, "the third operand is $I");
                word |= (destination() << 21) | (vfr(0) << 6) | (vfr(1) << 11);
                break;
            case Vu::Vi: need(3); word |= (vir(0) << 6) | (vir(1) << 11) | (vir(2) << 16); break;
            case Vu::ViImm: {
                need(3);
                const std::int64_t v = value(ops[2], labels, pc);
                if (v < -16 || v > 15) refuse(name, "the immediate is -16 to 15");
                word |= (vir(0) << 16) | (vir(1) << 11) | ((static_cast<std::uint32_t>(v) & 31) << 6);
                break;
            }
            case Vu::Call: {
                need(1);
                const std::int64_t v = value(ops[0], labels, pc);
                if (v < 0 || v > 0x7FFF) refuse(name, "the address is 15 bits");
                word |= static_cast<std::uint32_t>(v) << 6;
                break;
            }
            case Vu::CallR: need(0); break;
        }
        return word;
    }
    return std::nullopt;
}

// ---- the rest, one by one ---------------------------------------------------------------------------------------------------

// The VFPU's loads and stores: the opcode, whether the register is a single or a quad, and what the low bits of the word are.
struct VfpuMem {
    std::string_view name;
    std::uint32_t opcode;     // bits 31-26
    bool single;              // lv.s, sv.s: the register's bits 6-5 are the word's bits 1-0
    std::uint32_t low;        // for the quads: bit 1 of the word (0 lv.q; lvl.q 0, lvr.q 1; sv.q: the write-back flag)
    bool flag;                // sv.q: bit 1 is the `wb` flag
};
constexpr auto kVfpuMem = std::to_array<VfpuMem>({
    {"lv.s", 0x32, true, 0, false},  {"sv.s", 0x3A, true, 0, false},  {"lv.q", 0x36, false, 0, false}, {"sv.q", 0x3E, false, 0, true},
    {"lvl.q", 0x35, false, 0, false}, {"lvr.q", 0x35, false, 1, false}, {"svl.q", 0x3D, false, 0, false}, {"svr.q", 0x3D, false, 1, false},
});

const VfpuMem* vfpu_mem_of(std::uint32_t word) {
    const unsigned op = word >> 26;
    for (const VfpuMem& m : kVfpuMem) {
        if (m.opcode != op) continue;
        if (m.single) return &m;
        if (op == 0x36 || op == 0x3E) {   // lv.q (bit 1 zero), sv.q (bit 1 is the write-back flag)
            if (op == 0x36 && (word & 2) != 0) return nullptr;
            return &m;
        }
        if (m.low == ((word >> 1) & 1)) return &m;
    }
    return nullptr;
}

unsigned vfpu_mem_register(const VfpuMem& m, std::uint32_t word) {
    return m.single ? ((word >> 16) & 31) | ((word & 3) << 5) : ((word >> 16) & 31) | ((word & 1) << 5);
}

constexpr std::array<std::string_view, 4> kBvNames = {"bvf", "bvt", "bvfl", "bvtl"};

}  // namespace

bool decode(std::uint32_t word, std::uint32_t pc, Variant variant, Insn& insn) {
    const unsigned op = word >> 26;
    if (variant == Variant::R5900) {
        if (op == 0x36 || op == 0x3E) {   // lqc2, sqc2: a VU0 register to or from memory
            insn.mnemonic = op == 0x36 ? "lqc2" : "sqc2";
            insn.form = Form::Special;
            insn.imm = sign16(word);
            return true;
        }
        if (const int fn = vu_function(word); fn >= 0) {
            insn.mnemonic = std::string(vu_table()[static_cast<std::size_t>(fn)].name);
            const Vu kind = vu_table()[static_cast<std::size_t>(fn)].kind;
            if (kind == Vu::Fd || kind == Vu::Q || kind == Vu::I) insn.mnemonic += "." + dest_text((word >> 21) & 15);
            insn.form = Form::Special;
            return true;
        }
        return false;
    }
    if (variant != Variant::Allegrex) return false;

    // EXT and INS (SPECIAL3, functions 0 and 4)
    if (op == 0x1F && (word & 0x3F) == 0 && ((word >> 11) & 31) + ((word >> 6) & 31) < 32) {
        insn.mnemonic = "ext";
        insn.form = Form::Special;
        return true;
    }
    if (op == 0x1F && (word & 0x3F) == 4 && ((word >> 11) & 31) >= ((word >> 6) & 31)) {
        insn.mnemonic = "ins";
        insn.form = Form::Special;
        return true;
    }
    // the VFPU's loads and stores
    if (const VfpuMem* m = vfpu_mem_of(word)) {
        insn.mnemonic = std::string(m->name);
        insn.form = Form::Special;
        insn.imm = sign16(word & 0xFFFC);
        return true;
    }
    // BVF BVT BVFL BVTL: bits 25-21 = 8, the type in bits 17-16, the condition register in 20-18
    if ((word >> 21) == 0x248) {
        const unsigned type = (word >> 16) & 3;
        insn.mnemonic = std::string(kBvNames[type]);
        insn.form = Form::Special;
        insn.flow = Flow::Branch;
        insn.delay_slot = true;
        insn.likely = type >= 2;
        insn.target = pc + 4 + static_cast<std::uint32_t>(sign16(word)) * 4;
        return true;
    }
    if ((word >> 24) >= 0xDC && (word >> 24) <= 0xDE) {   // the VFPU's prefixes: VPFXS, VPFXT, VPFXD
        insn.mnemonic = (word >> 24) == 0xDC ? "vpfxs" : (word >> 24) == 0xDD ? "vpfxt" : "vpfxd";
        insn.form = Form::Special;
        insn.imm = static_cast<std::int32_t>(word & 0xFFFFFF);
        return true;
    }
    const VfpuMasks& masks = vfpu_masks();
    for (std::size_t i = 0; i < kVfpu.size(); ++i) {
        if ((word & masks.mask[i]) != masks.match[i]) continue;
        const VfpuOp& e = kVfpu[i];
        insn.mnemonic = std::string(e.name);
        if (e.size != 'n') insn.mnemonic += std::format(".{}", size_letter(e.size == 0 ? size_of(word) : e.size == 's' ? 1 : e.size == 'p' ? 2 : e.size == 't' ? 3 : 4));
        insn.form = Form::Special;
        return true;
    }
    return false;
}

std::string text(const Insn& insn, const AddressLabels& labels) {
    const std::string& m = insn.mnemonic;
    const std::uint32_t w = insn.word;
    if (m == "ext") return std::format("ext {}, {}, {}, {}", gpr((w >> 16) & 31), gpr((w >> 21) & 31), (w >> 6) & 31, ((w >> 11) & 31) + 1);
    if (m == "ins") return std::format("ins {}, {}, {}, {}", gpr((w >> 16) & 31), gpr((w >> 21) & 31), (w >> 6) & 31, ((w >> 11) & 31) - ((w >> 6) & 31) + 1);
    if (m == "lqc2" || m == "sqc2") return std::format("{} {}, {}({})", m, vf((w >> 16) & 31), insn.imm, gpr((w >> 21) & 31));
    for (const VfpuMem& e : kVfpuMem) {
        if (m != e.name) continue;
        const unsigned reg = vfpu_mem_register(e, w);
        std::string out = std::format("{} {}, {}({})", m, vfpu_name(e.single ? Reg::Single : Reg::Vector, e.single ? 1 : 4, reg), insn.imm, gpr((w >> 21) & 31));
        if (e.flag && (w & 2) != 0) out += ", wb";
        return out;
    }
    for (std::size_t t = 0; t < kBvNames.size(); ++t) {
        if (m != kBvNames[t]) continue;
        std::string to = std::format("0x{:08X}", *insn.target);
        if (const auto it = labels.find(*insn.target); it != labels.end() && !it->second.empty()) to = it->second;
        return std::format("{} {}, {}", m, (w >> 18) & 7, to);
    }
    if (m == "vpfxs" || m == "vpfxt" || m == "vpfxd") return std::format("{} 0x{:X}", m, w & 0xFFFFFF);
    const std::string base = m.substr(0, m.find('.'));
    for (const VfpuOp& e : kVfpu)
        if (e.name == base && ((e.size == 'n') == (m.find('.') == std::string::npos))) {
            const std::uint32_t mask = ~operand_bits(e), match = e.match | size_bits(e.size);
            if ((w & mask) == match) return vfpu_text(e, w);
        }
    return vu_text(insn);
}

std::optional<std::uint32_t> assemble(std::string_view name, const std::vector<std::string>& ops, std::uint32_t pc, const NameLabels& labels, Variant variant) {
    const auto count = [&](std::size_t n) {
        if (ops.size() != n) refuse(name, std::format("{} operands where {} are written", ops.size(), n));
    };
    const auto number = [&](std::size_t i) { return value(ops[i], labels, pc); };
    const auto reg = [&](std::size_t i) {
        const auto r = register_number(ops[i]);
        if (!r) refuse(name, "'" + ops[i] + "' is not a register");
        return *r;
    };
    // `offset(base)`
    const auto memory = [&](std::string_view operand, std::uint32_t& base) {
        operand = text::trim(operand);
        const std::size_t open = operand.rfind('(');
        if (open == std::string_view::npos || operand.back() != ')') refuse(name, "a memory operand is offset($register)");
        const auto b = register_number(operand.substr(open + 1, operand.size() - open - 2));
        if (!b) refuse(name, "the base is not a register");
        base = *b;
        const std::string_view off = text::trim(operand.substr(0, open));
        return off.empty() ? std::int64_t{0} : value(off, labels, pc);
    };
    if (variant == Variant::R5900) {
        if (name == "lqc2" || name == "sqc2") {
            count(2);
            std::uint32_t base = 0;
            const std::int64_t off = memory(ops[1], base);
            const auto ft = parse_vf(ops[0]);
            if (!ft) refuse(name, "'" + ops[0] + "' is not a $vf register");
            if (off < -32768 || off > 32767) refuse(name, "the offset does not fit");
            return ((name == "lqc2" ? 0x36u : 0x3Eu) << 26) | (base << 21) | (*ft << 16) | (static_cast<std::uint32_t>(off) & 0xFFFF);
        }
        return vu_assemble(name, ops, pc, labels);
    }
    if (variant != Variant::Allegrex) return std::nullopt;
    if (name == "ext" || name == "ins") {
        count(4);
        const std::int64_t pos = number(2), size = number(3);
        if (pos < 0 || pos > 31 || size < 1 || pos + size > 32) refuse(name, "the field is not inside 32 bits");
        const std::uint32_t msb = name == "ext" ? static_cast<std::uint32_t>(size - 1) : static_cast<std::uint32_t>(pos + size - 1);
        return (0x1Fu << 26) | (reg(1) << 21) | (reg(0) << 16) | (msb << 11) | (static_cast<std::uint32_t>(pos) << 6) | (name == "ext" ? 0u : 4u);
    }
    for (const VfpuMem& e : kVfpuMem) {
        if (name != e.name) continue;
        const bool wb = e.flag && ops.size() == 3 && text::trim(ops[2]) == "wb";
        count(wb ? 3 : 2);
        std::uint32_t base = 0;
        const std::int64_t off = memory(ops[1], base);
        if (off < -32768 || off > 32764 || (off & 3) != 0) refuse(name, "the offset is a multiple of 4 that fits 16 bits");
        const auto r = vfpu_parse(ops[0], e.single ? Reg::Single : Reg::Vector, e.single ? 1 : 4);
        if (!r) refuse(name, "'" + ops[0] + "' is not a VFPU register of this size");
        std::uint32_t word = (e.opcode << 26) | (base << 21) | ((*r & 31) << 16) | (static_cast<std::uint32_t>(off) & 0xFFFC);
        if (e.single) {
            word |= (*r >> 5) & 3;
        } else {
            if ((*r >> 6) != 0) refuse(name, "a quad register in memory has row 0");
            word |= (*r >> 5) & 1;
            if (e.opcode == 0x35 || e.opcode == 0x3D) word |= e.low << 1;
            if (wb) word |= 2;
        }
        return word;
    }
    for (std::size_t t = 0; t < kBvNames.size(); ++t) {
        if (name != kBvNames[t]) continue;
        count(2);
        const std::int64_t cc = number(0), to = number(1);
        if (cc < 0 || cc > 7) refuse(name, "the condition register is 0 to 7");
        const std::int64_t distance = to - (static_cast<std::int64_t>(pc) + 4);
        if (distance % 4 != 0 || distance / 4 < -32768 || distance / 4 > 32767) refuse(name, "the branch is out of reach");
        return (0x49u << 24) | (static_cast<std::uint32_t>(t) << 16) | (static_cast<std::uint32_t>(cc) << 18) | (static_cast<std::uint32_t>(distance / 4) & 0xFFFF);
    }
    if (name == "vpfxs" || name == "vpfxt" || name == "vpfxd") {
        count(1);
        const std::int64_t v = number(0);
        if (v < 0 || v > 0xFFFFFF) refuse(name, "the prefix is 24 bits");
        return ((name == "vpfxs" ? 0xDCu : name == "vpfxt" ? 0xDDu : 0xDEu) << 24) | static_cast<std::uint32_t>(v);
    }
    return vfpu_assemble(name, ops);
}

}  // namespace cb::decompile::mips::ext
