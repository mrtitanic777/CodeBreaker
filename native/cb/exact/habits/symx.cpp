#include "cb/exact/habits/symx.hpp"

#include <algorithm>
#include <format>
#include <string_view>
#include <variant>

#include "cb/core/text.hpp"

namespace cb::exact::habits::symx {

namespace {

using Kind = Expr::Kind;

constexpr std::array<std::string_view, kRegisterCount> kRegisters = {"ax", "cx", "dx", "bx", "si", "di"};

int register_index(std::string_view name) {
    for (std::size_t i = 0; i < kRegisters.size(); ++i) {
        if (kRegisters[i] == name) return static_cast<int>(i);
    }
    return -1;
}

// The low and high byte registers of ax cx dx bx, and the 16-bit register each is part of.
std::optional<std::string_view> low_byte_of(std::string_view name) {
    if (name == "al") return "ax";
    if (name == "cl") return "cx";
    if (name == "dl") return "dx";
    if (name == "bl") return "bx";
    return std::nullopt;
}

std::optional<std::string_view> high_byte_of(std::string_view name) {
    if (name == "ah") return "ax";
    if (name == "ch") return "cx";
    if (name == "dh") return "dx";
    if (name == "bh") return "bx";
    return std::nullopt;
}

// The stretch holds something the symbolic run does not know. Caught where the run is made (`run_stretch`) and
// turned into a Result / a verdict; it never reaches a caller.
class Unknown : public UnsupportedError {
  public:
    explicit Unknown(std::string why) : UnsupportedError("symx", 0, why), why_(std::move(why)) {}
    const std::string& why() const { return why_; }
    CB_ERROR_BOILERPLATE(Unknown)

  private:
    std::string why_;
};

// ---- building values ----------------------------------------------------------------------------------

Expr node(Kind kind, std::vector<Expr> args = {}) {
    Expr e;
    e.kind = kind;
    e.args = std::move(args);
    return e;
}

Expr constant(std::int64_t n) {
    Expr e;
    e.kind = Kind::Const;
    e.n = n & 0xFFFF;
    return e;
}

Expr linker_word() { return node(Kind::Linker); }

Expr unknown_register(std::string_view name) {
    Expr e;
    e.kind = Kind::Reg;
    e.name = std::string(name);
    return e;
}

bool is_constant(const Expr& x) { return x.kind == Kind::Const || x.kind == Kind::Linker; }

// What combining no terms gives, per operator.
std::int64_t unit(Kind op) {
    switch (op) {
        case Kind::Product: return 1;
        case Kind::And: return 0xFFFF;
        default: return 0;   // Sum, Or, Xor
    }
}

std::int64_t fold(Kind op, std::int64_t a, std::int64_t b) {
    switch (op) {
        case Kind::Sum: return a + b;
        case Kind::Product: return a * b;
        case Kind::Or: return a | b;
        case Kind::And: return a & b;
        default: return a ^ b;
    }
}

std::string_view tag(Kind k) {
    switch (k) {
        case Kind::Const: return "c";
        case Kind::Linker: return "f";
        case Kind::Reg: return "r";
        case Kind::Neg: return "neg";
        case Kind::Lo: return "lo";
        case Kind::LoHi: return "lohi";
        case Kind::Zx: return "zx";
        case Kind::Sum: return "+";
        case Kind::Product: return "*";
        case Kind::Or: return "|";
        case Kind::And: return "&";
        case Kind::Xor: return "^";
        case Kind::Mem: return "m";
        case Kind::Ord: return "ord";
        case Kind::Shr: return "shr";
        case Kind::High: return "high";
        case Kind::Div: return "div";
        case Kind::Mod: return "mod";
        case Kind::Res: return "res";
        case Kind::Cmp: return "cmp";
    }
    return "?";
}

std::string_view size_word(Size s) { return s == Size::Byte ? "byte" : "word"; }

// A text that is the same for values that are the same up to the constants (every constant is "K"): the sort key
// that puts the terms of a sum in a canonical order, and what two values are compared through.
void canon_into(std::string& out, const Expr& x) {
    if (is_constant(x)) {
        out += 'K';
        return;
    }
    out += '(';
    out += tag(x.kind);
    switch (x.kind) {
        case Kind::Reg:
            out += ' ';
            out += x.name;
            break;
        case Kind::Sum:
        case Kind::Product:
        case Kind::Or:
        case Kind::And:
        case Kind::Xor:
            out += " [";
            for (std::size_t i = 0; i < x.args.size(); ++i) {
                if (i != 0) out += ' ';
                canon_into(out, x.args[i]);
            }
            out += ']';
            break;
        case Kind::Mem:
            out += ' ';
            canon_into(out, x.args[0]);
            out += ' ';
            out += size_word(x.size);
            out += ' ';
            out += std::to_string(x.n);
            break;
        case Kind::Ord:
            out += ' ';
            out += std::to_string(x.n);
            out += ' ';
            if (x.args.empty()) out += "None";
            else canon_into(out, x.args[0]);
            break;
        case Kind::Shr:
            out += ' ';
            canon_into(out, x.args[0]);
            out += ' ';
            out += std::to_string(x.n);
            break;
        default:   // Neg Lo LoHi Zx High Div Mod Res Cmp: the arguments
            for (const Expr& a : x.args) {
                out += ' ';
                canon_into(out, a);
            }
            break;
    }
    out += ')';
}

std::string canon(const Expr& x) {
    std::string s;
    canon_into(s, x);
    return s;
}

std::string canon(const Slot& s) {
    std::string out = "(";
    out += size_word(s.size);
    out += ' ';
    canon_into(out, s.value);
    out += ')';
    return out;
}

// The terms of `op` (Sum Product Or And Xor), the ones that are themselves that operator spread out, the
// constants folded into one (a linker's word makes it "some constant"), in canonical order.
Expr combine(Kind op, const std::vector<Expr>& terms) {
    std::vector<Expr> out;
    std::optional<Expr> k;
    for (const Expr& x : terms) {
        const std::vector<Expr> single{x};
        for (const Expr& y : (x.kind == op ? x.args : single)) {
            if (!is_constant(y)) {
                out.push_back(y);
            } else if (!k) {
                k = y;
            } else if (k->kind == Kind::Linker || y.kind == Kind::Linker) {
                k = linker_word();
            } else {
                k = constant(fold(op, k->n, y.n));
            }
        }
    }
    if (k && *k != constant(unit(op))) out.push_back(*k);
    if (out.empty()) return constant(unit(op));
    if (out.size() == 1) return out[0];
    std::vector<std::pair<std::string, Expr>> keyed;
    keyed.reserve(out.size());
    for (Expr& e : out) keyed.emplace_back(canon(e), std::move(e));
    std::stable_sort(keyed.begin(), keyed.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
    std::vector<Expr> sorted;
    sorted.reserve(keyed.size());
    for (auto& kv : keyed) sorted.push_back(std::move(kv.second));
    return node(op, std::move(sorted));
}

Expr negate(const Expr& x) {
    if (x.kind == Kind::Const) return constant(-x.n);
    return node(Kind::Neg, {x});
}

// Are the two the same value, treating a constant the linker fills in as equal to any constant?
bool unify(const Expr& a, const Expr& b) {
    if (a == b) return true;
    if (is_constant(a) && is_constant(b)) return a.kind == Kind::Linker || b.kind == Kind::Linker;
    if (a.kind != b.kind || a.n != b.n || a.name != b.name || a.size != b.size || a.args.size() != b.args.size()) return false;
    for (std::size_t i = 0; i < a.args.size(); ++i) {
        if (!unify(a.args[i], b.args[i])) return false;
    }
    return true;
}

bool unify(const std::vector<Expr>& a, const std::vector<Expr>& b) {
    if (a.size() != b.size()) return false;
    for (std::size_t i = 0; i < a.size(); ++i) {
        if (!unify(a[i], b[i])) return false;
    }
    return true;
}

bool unify(const std::vector<Store>& a, const std::vector<Store>& b) {
    if (a.size() != b.size()) return false;
    for (std::size_t i = 0; i < a.size(); ++i) {
        if (a[i].size != b[i].size || !unify(a[i].address, b[i].address) || !unify(a[i].value, b[i].value)) return false;
    }
    return true;
}

bool unify(const std::vector<Slot>& a, const std::vector<Slot>& b) {
    if (a.size() != b.size()) return false;
    for (std::size_t i = 0; i < a.size(); ++i) {
        if (a[i].size != b[i].size || !unify(a[i].value, b[i].value)) return false;
    }
    return true;
}

bool unify(const std::optional<Expr>& a, const std::optional<Expr>& b) {
    if (!a || !b) return !a && !b;   // nothing known is the same only as nothing known
    return unify(*a, *b);
}

std::string show(const std::vector<Expr>& xs) {
    std::string out = "[";
    for (std::size_t i = 0; i < xs.size(); ++i) {
        if (i != 0) out += ", ";
        out += show(xs[i]);
    }
    return out + "]";
}

// ---- reading the instruction text ---------------------------------------------------------------------

bool is_digits(std::string_view s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), [](char c) { return c >= '0' && c <= '9'; });
}

// 0x1f or 123 (lower-case hex digits, as the disassembler prints them)
bool is_unsigned_number(std::string_view s) {
    if (s.starts_with("0x")) {
        s.remove_prefix(2);
        return !s.empty() && std::all_of(s.begin(), s.end(), [](char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'); });
    }
    return is_digits(s);
}

bool is_number(std::string_view s) { return is_unsigned_number(s.starts_with('-') ? s.substr(1) : s); }

bool is_word_char(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_'; }

struct Parsed {
    std::string_view op;
    std::vector<std::string_view> args;
};

// "mov ax, word ptr [bp - 2]" -> mov, {ax, word ptr [bp - 2]}
Parsed parse_instruction(std::string_view text) {
    std::size_t n = 0;
    while (n < text.size() && is_word_char(text[n])) ++n;
    if (n == 0 || (n < text.size() && (text[n] != ' ' || n + 1 == text.size()))) throw Unknown(std::string(text));
    Parsed p;
    p.op = text.substr(0, n);
    if (n < text.size()) {
        for (const std::string_view a : text::split(text.substr(n + 1), ", ")) p.args.push_back(text::trim(a));
    }
    return p;
}

// operand i of the instruction, or the stretch is refused (an instruction with too few operands is not one this knows)
std::string_view operand(const Parsed& p, std::size_t i, std::string_view whole) {
    if (i >= p.args.size()) throw Unknown(std::string(whole));
    return p.args[i];
}

struct Operand {
    enum class Form { Memory, Address, Register, Immediate };
    Form form = Form::Immediate;
    Size size = Size::Word;       // Memory
    std::string_view text;        // Memory/Address: what is inside the brackets; Register/Immediate: the operand
};

Operand parse_operand(std::string_view o) {
    for (const auto& [prefix, size] : {std::pair{std::string_view("word ptr ["), Size::Word}, std::pair{std::string_view("byte ptr ["), Size::Byte}}) {
        if (o.starts_with(prefix) && o.ends_with(']') && o.size() > prefix.size() + 1) {
            return {Operand::Form::Memory, size, o.substr(prefix.size(), o.size() - prefix.size() - 1)};
        }
    }
    if (o.size() >= 3 && o.front() == '[' && o.back() == ']') return {Operand::Form::Address, Size::Word, o.substr(1, o.size() - 2)};
    if (register_index(o) >= 0 || low_byte_of(o) || high_byte_of(o)) return {Operand::Form::Register, Size::Word, o};
    if (is_number(o)) return {Operand::Form::Immediate, Size::Word, o};
    throw Unknown("operand " + std::string(o));
}

// ---- the symbolic machine -----------------------------------------------------------------------------

class Machine {
  public:
    Machine() {
        for (std::size_t i = 0; i < kRegisters.size(); ++i) state.regs[i] = unknown_register(kRegisters[i]);
    }

    State state;

    Expr& reg(std::string_view name) { return state.regs[static_cast<std::size_t>(register_index(name))]; }

    // A number in the text: a constant, or "some constant" if the linker fills in one of that value.
    static Expr number(std::string_view t, const std::set<std::int64_t>* linker_words) {
        const std::optional<std::int64_t> v = text::parse_int<std::int64_t>(t, 0);
        if (!v) throw Unknown("a number this cannot hold: " + std::string(t));
        return linker_words != nullptr && linker_words->contains(*v) ? linker_word() : constant(*v);
    }

    // What the text inside brackets adds up to: registers, and numbers with the sign before them.
    Expr address(std::string_view t, const std::set<std::int64_t>* linker_words) {
        std::vector<Expr> terms;
        bool negative = false;
        for (const std::string_view tok : text::split_ws(t)) {
            if (tok == "+") {
                negative = false;
            } else if (tok == "-") {
                negative = true;
            } else if (register_index(tok) >= 0) {
                terms.push_back(reg(tok));
            } else if (tok == "bp") {
                terms.push_back(unknown_register("bp"));
            } else if (is_unsigned_number(tok)) {
                const Expr v = number(tok, linker_words);
                terms.push_back(negative ? negate(v) : v);
            } else {
                throw Unknown(std::string(t));
            }
        }
        return combine(Kind::Sum, terms);
    }

    // The offset from bp of the frame temporary an address names, if it names one.
    static std::optional<std::int64_t> slot_of(const Expr& a) {
        const Expr bp = unknown_register("bp");
        if (a == bp) return 0;
        if (a.kind == Kind::Sum && a.args.size() == 2 && (a.args[0] == bp || a.args[1] == bp)) {
            const Expr& other = a.args[0] == bp ? a.args[1] : a.args[0];
            if (other == bp) return std::nullopt;   // bp + bp
            if (other.kind == Kind::Const) return other.n;
        }
        return std::nullopt;
    }

    Expr get(std::string_view o, const std::set<std::int64_t>* fx) {
        const Operand p = parse_operand(o);
        switch (p.form) {
            case Operand::Form::Immediate: return number(p.text, fx);
            case Operand::Form::Address: return address(p.text, fx);
            case Operand::Form::Register:
                if (register_index(p.text) >= 0) return reg(p.text);
                if (const auto full = low_byte_of(p.text)) return node(Kind::Lo, {reg(*full)});
                throw Unknown("operand " + std::string(o));
            case Operand::Form::Memory: break;
        }
        const Expr a = address(p.text, fx);
        if (const auto s = slot_of(a)) {
            if (const Slot* held = state.slots.find(*s)) {
                if (held->size != p.size) throw Unknown("a temporary read in another size");
                return held->value;
            }
            return memory(a, p.size, 0);
        }
        return memory(a, p.size, static_cast<std::int64_t>(state.stores.size()));
    }

    void put(std::string_view o, const Expr& v, const std::set<std::int64_t>* fx) {
        const Operand p = parse_operand(o);
        if (p.form == Operand::Form::Register) {
            if (register_index(p.text) >= 0) {
                reg(p.text) = v;
            } else if (const auto full = low_byte_of(p.text)) {
                reg(*full) = node(Kind::LoHi, {v, reg(*full)});
            } else {
                throw Unknown("operand " + std::string(o));
            }
            return;
        }
        if (p.form != Operand::Form::Memory) throw Unknown("operand " + std::string(o));
        const Expr a = address(p.text, fx);
        if (const auto s = slot_of(a)) state.slots.insert_or_assign(*s, Slot{p.size, v});
        else state.stores.push_back({a, p.size, v});
    }

    void set_flags_of(const Expr& result) { state.flags = node(Kind::Res, {result}); }

  private:
    static Expr memory(const Expr& a, Size size, std::int64_t after_stores) {
        Expr e = node(Kind::Mem, {a});
        e.size = size;
        e.n = after_stores;
        return e;
    }
};

std::optional<Kind> commutative_operator(std::string_view op) {
    if (op == "add") return Kind::Sum;
    if (op == "or") return Kind::Or;
    if (op == "and") return Kind::And;
    if (op == "xor") return Kind::Xor;
    return std::nullopt;
}

// The compiler's way to a Boolean's value: mov R,0 / jcc over / inc R. If instruction k starts one, its opcode.
std::optional<std::uint8_t> boolean_jump_at(std::span<const Instruction> ins, std::size_t k, const Parsed& p) {
    if (p.op != "mov" || p.args.size() != 2 || p.args[1] != "0" || register_index(p.args[0]) < 0 || k + 2 >= ins.size()) return std::nullopt;
    const Bytes& jump = ins[k + 1].bytes;
    if (jump.size() < 2 || jump[0] < 0x70 || jump[0] > 0x7F || jump[1] != 1) return std::nullopt;
    if (ins[k + 2].text != "inc " + std::string(p.args[0])) return std::nullopt;
    return jump[0];
}

struct Refused {
    std::size_t instruction = 0;
    std::string why;
};

// One step of the machine: instruction k (index into `ins`); returns how far the cursor moves.
std::size_t step(Machine& m, std::span<const Instruction> ins, std::size_t k, const std::set<std::int64_t>* fx) {
    const std::string& t = ins[k].text;
    const Parsed p = parse_instruction(t);
    const std::string_view op = p.op;
    const auto arg = [&](std::size_t i) { return operand(p, i, t); };

    if (const auto jcc = boolean_jump_at(ins, k, p)) {
        Expr b;
        b.kind = Kind::Ord;
        b.n = *jcc;
        if (m.state.flags) b.args.push_back(*m.state.flags);
        m.reg(p.args[0]) = b;
        return 3;
    }
    if (op == "mov" || op == "lea") {
        const Expr v = m.get(arg(1), fx);
        m.put(arg(0), v, fx);
    } else if (op == "xchg") {
        const Expr x = m.get(arg(0), fx);
        const Expr y = m.get(arg(1), fx);
        m.put(arg(0), y, fx);
        m.put(arg(1), x, fx);
    } else if (op == "push") {
        m.state.pushes.push_back(m.get(arg(0), fx));
    } else if ((op == "xor" || op == "sub") && arg(0) == arg(1) && register_index(arg(0)) >= 0) {
        m.reg(arg(0)) = constant(0);
        m.set_flags_of(constant(0));
    } else if (op == "xor" && arg(0) == arg(1) && high_byte_of(arg(0))) {
        Expr& full = m.reg(*high_byte_of(arg(0)));
        if (full.kind == Kind::LoHi) full = node(Kind::Zx, {full.args[0]});
        else if (full.kind != Kind::Zx) full = node(Kind::Zx, {node(Kind::Lo, {full})});
    } else if (op == "and" && arg(1) == "0xff" && register_index(arg(0)) >= 0) {
        Expr& r = m.reg(arg(0));
        if (r.kind != Kind::Zx) r = node(Kind::Zx, {node(Kind::Lo, {r})});
        m.set_flags_of(r);
    } else if ((op == "or" || op == "and") && arg(0) == arg(1)) {
        m.set_flags_of(m.get(arg(0), fx));
    } else if (const auto combining = commutative_operator(op)) {
        const Expr x = m.get(arg(0), fx);
        const Expr y = m.get(arg(1), fx);
        const Expr v = combine(*combining, {x, y});
        m.put(arg(0), v, fx);
        m.set_flags_of(v);
    } else if (op == "sub") {
        const Expr x = m.get(arg(0), fx);
        const Expr y = negate(m.get(arg(1), fx));
        const Expr v = combine(Kind::Sum, {x, y});
        m.put(arg(0), v, fx);
        m.set_flags_of(v);
    } else if (op == "cmp") {
        const Expr x = m.get(arg(0), fx);
        const Expr y = m.get(arg(1), fx);
        m.state.flags = node(Kind::Cmp, {x, y});
    } else if (op == "inc" || op == "dec") {
        const Expr x = m.get(arg(0), fx);
        const Expr v = combine(Kind::Sum, {x, constant(op == "inc" ? 1 : -1)});
        m.put(arg(0), v, fx);
        m.set_flags_of(v);
    } else if ((op == "shl" || op == "shr") && (arg(1) == "1" || arg(1) == "cl")) {
        std::int64_t count = 1;
        if (arg(1) == "cl") {
            const Expr& cx = m.reg("cx");
            if (cx.kind != Kind::Const) throw Unknown("a shift by an unknown count");
            count = cx.n;
        }
        const Expr x = m.get(arg(0), fx);
        Expr v;
        if (op == "shl") {
            v = combine(Kind::Product, {x, constant(count >= 16 ? 0 : std::int64_t{1} << count)});   // (a constant keeps 16 bits)
        } else if (x.kind == Kind::Shr) {
            v = node(Kind::Shr, {x.args[0]});
            v.n = x.n + count;
        } else {
            v = node(Kind::Shr, {x});
            v.n = count;
        }
        m.put(arg(0), v, fx);
        m.set_flags_of(v);
    } else if (op == "mul") {
        const Expr v = combine(Kind::Product, {m.reg("ax"), m.get(arg(0), fx)});
        m.reg("ax") = v;
        m.reg("dx") = node(Kind::High, {v});
        m.set_flags_of(v);
    } else if (op == "div") {
        const Expr dx = m.reg("dx");
        const Expr ax = m.reg("ax");
        const Expr d = m.get(arg(0), fx);
        m.reg("ax") = node(Kind::Div, {dx, ax, d});
        m.reg("dx") = node(Kind::Mod, {dx, ax, d});
        m.state.flags.reset();
    } else {
        throw Unknown(t);
    }
    return 1;
}

std::variant<State, Refused> run_stretch(std::span<const Instruction> ins, const LinkerWords& linker_words) {
    Machine m;
    std::size_t k = 0;
    try {
        while (k < ins.size()) {
            const auto fixed = linker_words.find(k);
            k += step(m, ins, k, fixed == linker_words.end() ? nullptr : &fixed->second);
        }
    } catch (const Unknown& u) {   // the isolation point: a refusal is this module's answer, not a fault
        return Refused{k, u.why()};
    }
    return std::move(m.state);
}

}  // namespace

std::string show(const Expr& e) {
    switch (e.kind) {
        case Kind::Const: return std::to_string(e.n);
        case Kind::Linker: return "fixup";
        case Kind::Reg: return e.name;
        default: break;
    }
    std::string out = "(";
    out += tag(e.kind);
    switch (e.kind) {
        case Kind::Mem:
            out += std::format(" {} {} after {} stores", show(e.args[0]), size_word(e.size), e.n);
            break;
        case Kind::Ord:
            out += std::format(" jcc {:02x}", e.n);
            if (!e.args.empty()) out += " on " + show(e.args[0]);
            break;
        case Kind::Shr:
            out += " " + show(e.args[0]) + " by " + std::to_string(e.n);
            break;
        default:
            for (const Expr& a : e.args) out += " " + show(a);
            break;
    }
    return out + ")";
}

const Expr& State::reg(std::string_view name) const {
    const int i = register_index(name);
    CB_ASSERT(i >= 0, "not one of the six registers symx tracks");
    return regs[static_cast<std::size_t>(i)];
}

Result<State> try_run(std::span<const Instruction> ins, const LinkerWords& linker_words) {
    auto result = run_stretch(ins, linker_words);
    if (auto* refused = std::get_if<Refused>(&result)) {
        return UnsupportedError("symx", refused->instruction, std::format("instruction {}: {}", refused->instruction, refused->why));
    }
    return std::move(std::get<State>(result));
}

Verdict same(std::span<const Instruction> want, std::span<const Instruction> got, const LinkerWords& linker_words, Options options) {
    auto w = run_stretch(want, {});
    auto g = run_stretch(got, linker_words);
    for (const auto* r : {&w, &g}) {
        if (const auto* refused = std::get_if<Refused>(r)) return {false, "not known: " + refused->why};
    }
    const State& sw = std::get<State>(w);
    const State& sg = std::get<State>(g);
    if (!unify(sw.pushes, sg.pushes)) return {false, "other values pushed: " + show(sw.pushes) + " / " + show(sg.pushes)};
    if (!unify(sw.stores, sg.stores)) return {false, "other values stored"};
    // the frame's temporaries: the same values, whichever temporary holds which (compared in canonical order)
    const auto in_canonical_order = [](const State& s) {
        std::vector<std::pair<std::string, Slot>> keyed;
        for (const auto& [offset, slot] : s.slots) keyed.emplace_back(canon(slot), slot);
        std::stable_sort(keyed.begin(), keyed.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
        std::vector<Slot> out;
        for (auto& kv : keyed) out.push_back(std::move(kv.second));
        return out;
    };
    if (!unify(in_canonical_order(sw), in_canonical_order(sg))) return {false, "other values left in the frame"};
    if (options.registers) {
        for (std::size_t i = 0; i < kRegisterCount; ++i) {
            if (!unify(sw.regs[i], sg.regs[i])) return {false, "other values left in the registers"};
        }
    }
    if (options.flags && !unify(sw.flags, sg.flags)) return {false, "another condition"};
    return {true, ""};
}

}  // namespace cb::exact::habits::symx
