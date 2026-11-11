#include "cb/build/ppc_asm.hpp"

#include <algorithm>
#include <array>
#include <format>
#include <string>

#include "cb/core/io.hpp"
#include "cb/core/text.hpp"

namespace cb::build::ppc_asm {

namespace {

class LineFault : public Error {
public:
    using Error::Error;
    CB_ERROR_BOILERPLATE(LineFault)
};
[[noreturn]] void fail(const std::string& reason) { throw LineFault(reason); }

// ---- operands -------------------------------------------------------------------------------------------------

class Operands {
public:
    explicit Operands(std::vector<std::string_view> tokens) : tokens_(std::move(tokens)) {}
    std::string_view operator[](std::size_t i) const {
        if (i >= tokens_.size()) fail(std::format("missing operand {}", i + 1));
        return tokens_[i];
    }
    std::size_t size() const { return tokens_.size(); }

private:
    std::vector<std::string_view> tokens_;
};

// r0..r31, or the bare number.
std::uint32_t reg(std::string_view token) {
    std::string t = text::lower(text::trim(token));
    std::string_view digits = t;
    if (digits.starts_with('r')) digits.remove_prefix(1);
    const auto n = digits.empty() || digits.front() == '+' || digits.front() == '-'
                       ? std::nullopt
                       : text::parse_int<std::uint32_t>(digits);
    if (!n || *n > 31) fail(std::format("bad register: {}", token));
    return *n;
}

// An immediate field of `bits` bits. Signed fields take the signed range and, as assemblers do, the unsigned
// bit pattern of the same width (0xFFFF for -1); unsigned fields take 0..2^bits-1.
std::uint32_t imm(std::string_view token, unsigned bits = 16, bool is_signed = true) {
    const auto v = parse_number(token);
    if (!v) fail(std::format("bad immediate: {}", token));
    const std::int64_t top = (std::int64_t{1} << bits) - 1;
    const std::int64_t lo = is_signed ? -(std::int64_t{1} << (bits - 1)) : 0;
    if (*v < lo || *v > top) fail(std::format("immediate out of range: {}", token));
    return static_cast<std::uint32_t>(*v & top);
}

struct Memory {
    std::uint32_t displacement;
    std::uint32_t base;
};

// `D(rA)`: a 16-bit displacement and the base register.
Memory memory(std::string_view token) {
    token = text::trim(token);
    const std::size_t open = token.find('(');
    if (open == std::string_view::npos || !token.ends_with(')')) fail(std::format("bad memory operand: {}", token));
    const std::string_view disp = text::trim(token.substr(0, open));
    const std::string_view base = text::trim(token.substr(open + 1, token.size() - open - 2));
    if (disp.empty()) fail(std::format("bad memory operand: {}", token));
    return {imm(disp, 16), reg(base)};
}

// A branch target: a label, or an address.
std::int64_t target(std::string_view token, const Labels& labels) {
    if (const auto it = labels.find(std::string(token)); it != labels.end()) return it->second;
    const auto v = parse_number(token);
    if (!v) fail(std::format("bad branch target: {}", token));
    return *v;
}

// ---- encodings -------------------------------------------------------------------------------------------------

constexpr std::uint32_t kXForm = 31;

constexpr std::uint32_t d_form(std::uint32_t op, std::uint32_t rt, std::uint32_t ra, std::uint32_t value) {
    return ((op & 0x3F) << 26) | ((rt & 0x1F) << 21) | ((ra & 0x1F) << 16) | (value & 0xFFFF);
}
constexpr std::uint32_t x_form(std::uint32_t op, std::uint32_t rt, std::uint32_t ra, std::uint32_t rb, std::uint32_t xo,
                               std::uint32_t rc = 0) {
    return ((op & 0x3F) << 26) | ((rt & 0x1F) << 21) | ((ra & 0x1F) << 16) | ((rb & 0x1F) << 11) | ((xo & 0x3FF) << 1) |
           (rc & 1);
}
// The SPR field holds the two halves of the register number swapped.
constexpr std::uint32_t spr_field(std::uint32_t spr) { return ((spr & 0x1F) << 5) | ((spr >> 5) & 0x1F); }
constexpr std::uint32_t move_spr(std::uint32_t rt, std::uint32_t spr, std::uint32_t xo) {
    return (kXForm << 26) | (rt << 21) | (spr_field(spr) << 11) | (xo << 1);
}
constexpr std::uint32_t kMfspr = 339;
constexpr std::uint32_t kMtspr = 467;

struct Spr {
    std::string_view name;
    std::uint32_t number;
};
constexpr std::array kSprs{Spr{"xer", 1}, Spr{"lr", 8}, Spr{"ctr", 9}};

std::uint32_t spr_operand(std::string_view token) {
    const std::string name = text::lower(token);
    for (const Spr& s : kSprs) {
        if (s.name == name) return s.number;
    }
    return imm(token, 10, false);
}

// X-form three-register operations. `Logical` ones name the destination first but encode it in the RA field
// (`or rA,rS,rB`); `SubFrom` is the simplified `sub rD,rA,rB` = `subf rD,rB,rA`.
enum class Order { Plain, Logical, SubFrom };
struct XOp {
    std::string_view name;
    std::uint32_t xo;
    Order order;
};
constexpr std::array kXOps{
    XOp{"add", 266, Order::Plain},   XOp{"add.", 266, Order::Plain},   XOp{"subf", 40, Order::Plain},
    XOp{"subf.", 40, Order::Plain},  XOp{"sub", 40, Order::SubFrom},   XOp{"and", 28, Order::Logical},
    XOp{"and.", 28, Order::Logical}, XOp{"or", 444, Order::Logical},   XOp{"or.", 444, Order::Logical},
    XOp{"xor", 316, Order::Logical}, XOp{"xor.", 316, Order::Logical}, XOp{"nand", 476, Order::Logical},
    XOp{"nor", 124, Order::Logical}, XOp{"slw", 24, Order::Logical},   XOp{"srw", 536, Order::Logical},
    XOp{"sraw", 792, Order::Logical}, XOp{"mullw", 235, Order::Plain}, XOp{"mullw.", 235, Order::Plain},
    XOp{"divw", 491, Order::Plain},  XOp{"divwu", 459, Order::Plain},  XOp{"andc", 60, Order::Logical},
};

// D-form ALU and logical operations: opcode, whether the immediate is signed, whether the first register is
// the destination written in the RA field (the logical immediates).
struct DOp {
    std::string_view name;
    std::uint32_t op;
    bool is_signed;
    bool logical;
};
constexpr std::array kDOps{
    DOp{"addi", 14, true, false},   DOp{"addis", 15, true, false},  DOp{"subi", 14, true, false},
    DOp{"ori", 24, false, true},    DOp{"oris", 25, false, true},   DOp{"xori", 26, false, true},
    DOp{"xoris", 27, false, true},  DOp{"andi.", 28, false, true},  DOp{"andis.", 29, false, true},
    DOp{"mulli", 7, true, false},   DOp{"cmpwi", 11, true, false},  DOp{"cmplwi", 10, false, false},
};

struct MemOp {
    std::string_view name;
    std::uint32_t op;
};
constexpr std::array kMemOps{
    MemOp{"lwz", 32}, MemOp{"lwzu", 33}, MemOp{"lbz", 34}, MemOp{"lbzu", 35}, MemOp{"lhz", 40},
    MemOp{"lhzu", 41}, MemOp{"lha", 42}, MemOp{"stw", 36}, MemOp{"stwu", 37}, MemOp{"stb", 38},
    MemOp{"stbu", 39}, MemOp{"sth", 44}, MemOp{"sthu", 45},
};

// Conditional branches by simplified mnemonic: BO and BI fields.
struct CondBranch {
    std::string_view name;
    std::uint32_t bo;
    std::uint32_t bi;
};
constexpr std::array kCondBranches{
    CondBranch{"beq", 12, 2},  CondBranch{"bne", 4, 2},  CondBranch{"blt", 12, 0}, CondBranch{"bge", 4, 0},
    CondBranch{"bgt", 12, 1},  CondBranch{"ble", 4, 1},  CondBranch{"bdnz", 16, 0}, CondBranch{"bdz", 18, 0},
    CondBranch{"bso", 12, 3},  CondBranch{"bns", 4, 3},
};

struct Fixed {
    std::string_view name;
    std::uint32_t word;
};
constexpr std::array kFixed{
    Fixed{"nop", 0x60000000}, Fixed{"blr", 0x4E800020},  Fixed{"blrl", 0x4E800021}, Fixed{"bctr", 0x4E800420},
    Fixed{"bctrl", 0x4E800421}, Fixed{"trap", 0x7FE00008}, Fixed{"tw", 0x7FE00008},   // tw 31,r0,r0: an unconditional trap
};

template <class Table>
const typename Table::value_type* find_named(const Table& table, std::string_view name) {
    for (const auto& entry : table) {
        if (entry.name == name) return &entry;
    }
    return nullptr;
}

// The branch distance `delta` as a field of a branch whose displacement is `bits` wide (a multiple of 4).
std::uint32_t branch_field(std::int64_t delta, unsigned bits, std::uint32_t mask) {
    if (delta & 3) fail("branch target not 4-aligned");
    const std::int64_t limit = std::int64_t{1} << (bits - 1);
    if (delta < -limit || delta >= limit) fail(std::format("branch target out of range ({} bytes)", delta));
    return static_cast<std::uint32_t>(delta) & mask;
}

std::optional<std::uint32_t> encode(std::string_view source, std::uint32_t address, const Labels& labels) {
    std::string_view line = source.substr(0, source.find('#'));
    line = text::trim(line.substr(0, line.find(';')));
    if (line.empty() || line.ends_with(':')) return std::nullopt;

    std::string commas_as_space(line);
    std::replace(commas_as_space.begin(), commas_as_space.end(), ',', ' ');
    std::vector<std::string_view> parts = text::split_ws(commas_as_space);
    const std::string mn = text::lower(parts[0]);
    parts.erase(parts.begin());
    const Operands ops(std::move(parts));
    const bool dotted = mn.ends_with('.');

    if (const Fixed* f = find_named(kFixed, mn)) return f->word;

    // ---- simplified mnemonics ------------------------------------------------------------------------------
    if (mn == "mflr") return move_spr(reg(ops[0]), 8, kMfspr);
    if (mn == "mtlr") return move_spr(reg(ops[0]), 8, kMtspr);
    if (mn == "mfctr") return move_spr(reg(ops[0]), 9, kMfspr);
    if (mn == "mtctr") return move_spr(reg(ops[0]), 9, kMtspr);
    if (mn == "mtspr") {   // mtspr spr, rS
        const std::uint32_t spr = spr_operand(ops[0]);
        return move_spr(reg(ops[1]), spr, kMtspr);
    }
    if (mn == "mfspr") {   // mfspr rD, spr
        const std::uint32_t rd = reg(ops[0]);
        return move_spr(rd, spr_operand(ops[1]), kMfspr);
    }
    if (mn == "li" || mn == "lis") {
        const std::uint32_t rt = reg(ops[0]);
        return d_form(mn == "li" ? 14 : 15, rt, 0, imm(ops[1]));
    }
    if (mn == "mr" || mn == "mr.") {   // or rA,rS,rS
        const std::uint32_t ra = reg(ops[0]);
        const std::uint32_t rs = reg(ops[1]);
        return x_form(kXForm, rs, ra, rs, 444, dotted ? 1 : 0);
    }
    if (mn == "not") {   // nor rA,rS,rS
        const std::uint32_t ra = reg(ops[0]);
        const std::uint32_t rs = reg(ops[1]);
        return x_form(kXForm, rs, ra, rs, 124);
    }

    // ---- branches ------------------------------------------------------------------------------------------
    if (mn == "b" || mn == "ba" || mn == "bl" || mn == "bla") {
        const bool absolute = mn == "ba" || mn == "bla";
        const std::uint32_t link = mn.ends_with('l') || mn == "bla" ? 1 : 0;
        std::int64_t dest = target(ops[0], labels);
        // an absolute target is the sign-extended 26-bit field: addresses at the very top wrap to negative
        if (absolute && dest >= 0x80000000LL && dest <= 0xFFFFFFFFLL) dest -= 0x100000000LL;
        const std::uint32_t li = branch_field(absolute ? dest : dest - address, 26, 0x03FFFFFC);
        return (18u << 26) | li | (absolute ? 2u : 0u) | link;
    }
    const CondBranch* cond = find_named(kCondBranches, mn);
    if (cond != nullptr || mn == "bc" || mn == "bcl") {
        std::uint32_t bo = 0, bi = 0, link = 0;
        std::int64_t dest = 0;
        if (cond != nullptr) {
            bo = cond->bo;
            bi = cond->bi;
            dest = target(ops[0], labels);
        } else {
            bo = imm(ops[0], 5, false);
            bi = imm(ops[1], 5, false);
            dest = target(ops[2], labels);
            link = mn == "bcl" ? 1 : 0;
        }
        const std::uint32_t bd = branch_field(dest - address, 16, 0xFFFC);
        return (16u << 26) | (bo << 21) | (bi << 16) | bd | link;
    }

    // ---- D-form --------------------------------------------------------------------------------------------
    if (const DOp* d = find_named(kDOps, mn)) {
        if (mn == "subi") {   // addi rD,rA,-value
            const std::uint32_t rt = reg(ops[0]);
            const std::uint32_t ra = reg(ops[1]);
            return d_form(14, rt, ra, 0u - imm(ops[2], 16, false));
        }
        if (mn == "cmpwi" || mn == "cmplwi") {
            const std::uint32_t ra = reg(ops[0]);
            return d_form(d->op, 0, ra, imm(ops[1], 16, d->is_signed));
        }
        const std::uint32_t first = reg(ops[0]);
        const std::uint32_t second = reg(ops[1]);
        const std::uint32_t value = ops.size() > 2 ? imm(ops[2], 16, d->is_signed) : 0;
        return d->logical ? d_form(d->op, second, first, value) : d_form(d->op, first, second, value);
    }
    if (const MemOp* m = find_named(kMemOps, mn)) {
        const Memory mem = memory(ops[1]);
        return d_form(m->op, reg(ops[0]), mem.base, mem.displacement);
    }

    // ---- X-form --------------------------------------------------------------------------------------------
    if (const XOp* x = find_named(kXOps, mn)) {
        const std::uint32_t first = reg(ops[0]);
        const std::uint32_t second = reg(ops[1]);
        const std::uint32_t third = reg(ops[2]);
        const std::uint32_t rc = dotted ? 1 : 0;
        switch (x->order) {
            case Order::Plain: return x_form(kXForm, first, second, third, x->xo, rc);
            case Order::Logical: return x_form(kXForm, second, first, third, x->xo, rc);
            case Order::SubFrom: return x_form(kXForm, first, third, second, x->xo, rc);
        }
    }
    if (mn == "cmpw" || mn == "cmplw") {
        const std::uint32_t ra = reg(ops[0]);
        const std::uint32_t rb = reg(ops[1]);
        return x_form(kXForm, 0, ra, rb, mn == "cmpw" ? 0 : 32);
    }
    if (mn == "neg" || mn == "neg.") {
        const std::uint32_t rt = reg(ops[0]);
        return x_form(kXForm, rt, reg(ops[1]), 0, 104, dotted ? 1 : 0);
    }
    if (mn == "extsb" || mn == "extsh" || mn == "extsb." || mn == "extsh.") {   // extsb rA,rS
        const std::uint32_t ra = reg(ops[0]);
        const std::uint32_t rs = reg(ops[1]);
        return x_form(kXForm, rs, ra, 0, mn.starts_with("extsb") ? 954 : 922, dotted ? 1 : 0);
    }

    fail(std::format("unsupported mnemonic: {}", mn));
}

}  // namespace

std::optional<std::uint32_t> assemble_one(std::string_view line, std::uint32_t address, const Labels& labels) {
    try {
        return encode(line, address, labels);
    } catch (const LineFault& e) {
        throw AsmError(0, line, e.what());
    }
}

Bytes assemble(std::string_view text, std::uint32_t base_address) {
    struct Line {
        std::uint32_t number;
        std::string_view text;   // comments removed, trimmed
    };
    std::vector<Line> lines;
    std::uint32_t n = 0;
    for (const std::string_view raw : text::lines(text)) {
        ++n;
        std::string_view s = raw.substr(0, raw.find('#'));
        s = text::trim(s.substr(0, s.find(';')));
        if (!s.empty()) lines.push_back({n, s});
    }
    // pass 1: every instruction is four bytes, so labels need no encoding
    Labels labels;
    std::uint32_t address = base_address;
    for (const Line& l : lines) {
        if (l.text.ends_with(':')) {
            labels[std::string(text::trim(l.text.substr(0, l.text.size() - 1)))] = address;
        } else {
            address += 4;
        }
    }
    // pass 2: encode
    io::ByteWriter out;
    address = base_address;
    for (const Line& l : lines) {
        try {
            const std::optional<std::uint32_t> word = encode(l.text, address, labels);
            if (!word) continue;
            out.u32_be(*word);
            address += 4;
        } catch (const LineFault& e) {
            throw AsmError(l.number, l.text, e.what());
        }
    }
    return out.take();
}

}  // namespace cb::build::ppc_asm
