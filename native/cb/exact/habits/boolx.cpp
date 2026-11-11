#include "cb/exact/habits/boolx.hpp"

#include <algorithm>
#include <array>
#include <compare>
#include <format>
#include <map>
#include <set>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>

#include "cb/core/text.hpp"

namespace cb::exact::habits::boolx {

namespace {

constexpr int kMaxSteps = 400;            // instructions one run may execute (a loop is refused)
constexpr std::size_t kMaxCases = 300000;  // assignments one stretch may be asked to be tried on

// ---- the unknowns -------------------------------------------------------------------------------------

// What a stretch asks to be decided: a Boolean byte in memory (0/1), a word read from memory and masked with a
// constant (zero or not: 0/1), or how two operands compare (0 less, 1 equal, 2 greater).
struct Key {
    enum class Kind { Boolean, Masked, Comparison };
    Kind kind = Kind::Boolean;
    std::string a;            // the memory operand, or the first operand compared
    std::string b;            // the second operand compared
    std::int64_t mask = 0;    // Masked: the constant
    friend auto operator<=>(const Key&, const Key&) = default;
    int domain() const { return kind == Kind::Comparison ? 3 : 2; }
};

std::string describe(const Key& k) {
    switch (k.kind) {
        case Key::Kind::Boolean: return "bool " + k.a;
        case Key::Kind::Masked: return std::format("{} & {}", k.a, k.mask);
        case Key::Kind::Comparison: break;
    }
    return std::format("{} ? {}", k.a, k.b);
}

std::string value_name(const Key& k, int v) {
    if (k.kind != Key::Kind::Comparison) return std::to_string(v);
    return v == 0 ? "less" : v == 1 ? "equal" : "greater";
}

// The unknowns of one comparison of two stretches, each with a number (assignments are vectors by that number).
class KeyTable {
  public:
    int intern(const Key& k) {
        const auto [it, added] = ids_.try_emplace(k, static_cast<int>(keys_.size()));
        if (added) keys_.push_back(k);
        return it->second;
    }
    const Key& key(int id) const { return keys_[static_cast<std::size_t>(id)]; }
    std::size_t size() const { return keys_.size(); }

  private:
    std::map<Key, int> ids_;
    std::vector<Key> keys_;
};

// What each unknown is taken to be in one run: -1 = not decided yet.
using Assignment = std::vector<std::int8_t>;

// ---- reading the instructions -------------------------------------------------------------------------

bool is_word_char(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_'; }

constexpr std::array<std::string_view, 4> kWord = {"ax", "cx", "dx", "bx"};
constexpr std::array<std::string_view, 4> kByte = {"al", "cl", "dl", "bl"};
constexpr std::array<std::string_view, 4> kHigh = {"ah", "ch", "dh", "bh"};

int index_in(const std::array<std::string_view, 4>& names, std::string_view s) {
    for (std::size_t i = 0; i < names.size(); ++i) {
        if (names[i] == s) return static_cast<int>(i);
    }
    return -1;
}

// 0x1f or 123 (lower-case hex digits)
std::optional<std::int64_t> parse_number(std::string_view s) {
    const auto digits = [](std::string_view d, bool hex) {
        return !d.empty() && std::all_of(d.begin(), d.end(), [hex](char c) { return (c >= '0' && c <= '9') || (hex && c >= 'a' && c <= 'f'); });
    };
    if (s.starts_with("0x") ? !digits(s.substr(2), true) : !digits(s, false)) return std::nullopt;
    return text::parse_int<std::int64_t>(s, 0);
}

// A memory operand: `[inner]`, optionally after `byte ptr ` or `word ptr `. Its address text has SI and DI replaced
// by what they held at the start (or were last loaded from) when the operand is read, so it is kept in pieces.
struct Memory {
    struct Piece {
        std::string literal;
        int base = -1;   // 0: SI, 1: DI, or -1 for the literal
    };
    std::vector<Piece> pieces;
    bool frame_slot = false;   // the address starts "bp - ": a temporary of the frame
};

Memory parse_memory_inner(std::string_view inner) {
    Memory m;
    m.frame_slot = inner.starts_with("bp - ");
    std::string literal;
    std::size_t i = 0;
    while (i < inner.size()) {
        if (!is_word_char(inner[i])) {
            literal += inner[i++];
            continue;
        }
        std::size_t j = i;
        while (j < inner.size() && is_word_char(inner[j])) ++j;
        const std::string_view word = inner.substr(i, j - i);
        if (word == "si" || word == "di") {
            if (!literal.empty()) m.pieces.push_back({std::move(literal), -1});
            literal.clear();
            m.pieces.push_back({"", word == "si" ? 0 : 1});
        } else {
            literal += word;
        }
        i = j;
    }
    if (!literal.empty()) m.pieces.push_back({std::move(literal), -1});
    return m;
}

std::optional<Memory> parse_memory(std::string_view op) {
    if (op.starts_with("byte ptr ") || op.starts_with("word ptr ")) op.remove_prefix(9);
    if (op.size() < 3 || op.front() != '[' || op.back() != ']') return std::nullopt;
    const std::string_view inner = op.substr(1, op.size() - 2);
    if (inner.find(']') != std::string_view::npos) return std::nullopt;
    return parse_memory_inner(inner);
}

struct Operand {
    enum class Form { Word, Byte, Number, Memory, Other };
    Form form = Form::Other;
    int reg = -1;               // Word, Byte: which of ax cx dx bx
    std::int64_t number = 0;
    Memory memory;
    std::string text;
};

Operand classify(std::string_view op) {
    Operand o;
    o.text = std::string(op);
    if (const int w = index_in(kWord, op); w >= 0) {
        o.form = Operand::Form::Word;
        o.reg = w;
    } else if (const int b = index_in(kByte, op); b >= 0) {
        o.form = Operand::Form::Byte;
        o.reg = b;
    } else if (const auto n = parse_number(op)) {
        o.form = Operand::Form::Number;
        o.number = *n;
    } else if (auto m = parse_memory(op)) {
        o.form = Operand::Form::Memory;
        o.memory = std::move(*m);
    }
    return o;
}

enum class Jump { Always, Equal, NotEqual, Below, AboveOrEqual, Above, BelowOrEqual };

struct Step {
    enum class Kind { Jump, MoveToBase, MoveToRegister, MoveToTemporary, Increment, Nothing, FlipBoolean, Combine, Mask, Compare, TestBoolean, ShiftRight, Refuse };
    Kind kind = Kind::Refuse;
    std::string text;       // the instruction (for what a refusal says)
    Jump jump = Jump::Always;
    std::uint32_t target = 0;
    int reg = -1;           // the register written (an index of ax cx dx bx)
    int base = -1;          // MoveToBase: SI (0) or DI (1)
    bool conjunction = false;   // Combine: and, else or
    Operand source;
    Operand other;          // Compare: the second operand; Mask: the constant is in `source.number`
    std::string why;        // Refuse
};

Step refuse(std::string_view text, std::string why) {
    Step s;
    s.kind = Step::Kind::Refuse;
    s.text = std::string(text);
    s.why = std::move(why);
    return s;
}

std::optional<Jump> jump_of(std::string_view op) {
    if (op == "jmp") return Jump::Always;
    if (op == "je") return Jump::Equal;
    if (op == "jne") return Jump::NotEqual;
    if (op == "jb") return Jump::Below;
    if (op == "jae") return Jump::AboveOrEqual;
    if (op == "ja") return Jump::Above;
    if (op == "jbe") return Jump::BelowOrEqual;
    return std::nullopt;
}

// An instruction made ready to run. What cannot be run is a Refuse step: it refuses the stretch only if it is reached.
Step compile(std::string_view text) {
    std::size_t n = 0;
    while (n < text.size() && is_word_char(text[n])) ++n;
    if (n == 0) return refuse(text, "instruction " + std::string(text));
    const std::string_view op = text.substr(0, n);
    const std::string_view rest = text::ltrim(text.substr(n));
    std::vector<std::string_view> a;
    if (!rest.empty()) a = text::split_trim(rest, ',');
    const std::string whole(text);

    Step s;
    s.text = whole;
    if (const auto jump = jump_of(op)) {
        const auto target = a.empty() ? std::nullopt : text::parse_int<std::uint32_t>(a[0], 0);
        if (!target) return refuse(text, "a jump this cannot read: " + whole);
        s.kind = Step::Kind::Jump;
        s.jump = *jump;
        s.target = *target;
        return s;
    }
    if (op == "mov") {
        if (a.size() != 2) return refuse(text, "mov with " + std::to_string(a.size()) + " operands");
        const Operand d = classify(a[0]);
        s.source = classify(a[1]);
        if (a[0] == "si" || a[0] == "di") {
            s.kind = Step::Kind::MoveToBase;
            s.base = a[0] == "si" ? 0 : 1;
        } else if (d.form == Operand::Form::Word || d.form == Operand::Form::Byte) {
            s.kind = Step::Kind::MoveToRegister;
            s.reg = d.reg;
        } else if (d.form == Operand::Form::Memory && d.memory.frame_slot &&
                   (s.source.form == Operand::Form::Word || s.source.form == Operand::Form::Byte)) {
            s.kind = Step::Kind::MoveToTemporary;
            s.other = d;
        } else {
            return refuse(text, "store " + whole);
        }
        return s;
    }
    // The rest read their operands in the order below, and one that is not there is reached only after the tests before
    // it have passed (a missing operand where the original would have stopped on it is refused).
    const auto missing = [&] { return refuse(text, "operands missing in " + whole); };
    if (op == "inc") {
        if (a.empty()) return missing();
        if (index_in(kWord, a[0]) >= 0) {
            s.kind = Step::Kind::Increment;
            s.reg = index_in(kWord, a[0]);
            return s;
        }
    }
    if (op == "xor") {
        if (a.empty()) return missing();
        if (index_in(kHigh, a[0]) >= 0) {
            if (a.size() < 2) return missing();
            if (a[1] == a[0]) {
                s.kind = Step::Kind::Nothing;   // clearing the high byte of a register that holds a byte: no change to a 0/1 value
                return s;
            }
        }
        if (index_in(kByte, a[0]) >= 0) {
            if (a.size() < 2) return missing();
            if (a[1] == "1") {
                s.kind = Step::Kind::FlipBoolean;
                s.reg = index_in(kByte, a[0]);
                return s;
            }
        }
    }
    if (op == "and" || op == "or") {
        if (a.empty()) return missing();
        if (index_in(kByte, a[0]) >= 0) {
            s.kind = Step::Kind::Combine;
            s.reg = index_in(kByte, a[0]);
            s.conjunction = op == "and";
            s.source = classify(a.size() > 1 ? a[1] : std::string_view());   // (a missing one refuses when it is read)
            return s;
        }
    }
    if (op == "and" && index_in(kWord, a[0]) >= 0) {
        if (a.size() < 2) return missing();
        if (parse_number(a[1])) {
            s.kind = Step::Kind::Mask;
            s.reg = index_in(kWord, a[0]);
            s.source = classify(a[1]);
            return s;
        }
    }
    if (op == "cmp") {
        if (a.size() < 2) return missing();
        s.kind = Step::Kind::Compare;
        s.source = classify(a[0]);
        s.other = classify(a[1]);
        return s;
    }
    if (op == "test") {
        if (a.size() < 2) return missing();
        if (a[1] == "1" && classify(a[0]).form == Operand::Form::Memory) {
            s.kind = Step::Kind::TestBoolean;
            s.source = classify(a[0]);
            return s;
        }
    }
    if (op == "shr") {
        if (a.empty()) return missing();
        if (index_in(kWord, a[0]) >= 0) {
            if (a.size() < 2) return missing();
            if (a[1] == "1") {
                s.kind = Step::Kind::ShiftRight;
                s.reg = index_in(kWord, a[0]);
                return s;
            }
        }
    }
    return refuse(text, "instruction " + whole);
}

// A stretch made ready: its steps, and where each offset is.
struct Compiled {
    std::vector<Step> steps;
    std::unordered_map<std::uint32_t, std::size_t> at;   // lookup only
    std::uint32_t lo = 0;
    std::uint32_t hi = 0;
    Bases bases;
};

Compiled compile_stretch(const Stretch& s) {
    Compiled c;
    c.lo = s.lo;
    c.hi = s.hi;
    c.bases = s.bases;
    c.steps.reserve(s.instructions.size());
    for (const Instruction& ins : s.instructions) {
        c.at[ins.offset] = c.steps.size();   // an offset met twice names the later instruction
        c.steps.push_back(compile(ins.text));
    }
    return c;
}

// ---- running a stretch --------------------------------------------------------------------------------

struct Flow {
    bool leaves = false;   // jumps out of the stretch (else falls through its end)
    std::uint32_t target = 0;
};

// How one run ended: it finished (`flow`), it needs an unknown decided (`key`), or it holds something refused (`why`).
struct RunResult {
    enum class Status { Finished, NeedsKey, Refused };
    Status status = Status::Finished;
    Flow flow;
    int key = -1;
    std::string why;
};

// A value an operand has during a run.
struct Value {
    enum class Kind { Number, Memory, Register, Masked };
    Kind kind = Kind::Number;
    std::int64_t number = 0;   // Number: the value; Masked: 0 or 1
    std::string text;          // Memory: the address key; Register: its name
    std::string label() const {
        switch (kind) {
            case Kind::Number: return std::to_string(number);
            case Kind::Memory: return "mem " + text;
            case Kind::Register: return "reg " + text;
            case Kind::Masked: break;
        }
        return "bits " + std::to_string(number);
    }
};

// What the last flag-setting instruction left.
struct Flags {
    enum class Kind { Unknown, Comparison, Logical };
    Kind kind = Kind::Unknown;
    int comparison = 0;                // Comparison: 0 less, 1 equal, 2 greater
    std::optional<bool> zero;          // Logical: ZF, if known
    std::optional<bool> carry;         // Logical: CF, if known
};

class Interpreter {
  public:
    Interpreter(const Compiled& code, KeyTable& keys, const Assignment& assignment) : code_(code), keys_(keys), assignment_(assignment) {
        bases_[0] = code.bases.si;
        bases_[1] = code.bases.di;
    }

    RunResult run() {
        std::size_t k = 0;
        int steps = 0;
        while (k < code_.steps.size()) {
            if (++steps > kMaxSteps) return refused("loop");
            const Step& s = code_.steps[k];
            if (s.kind == Step::Kind::Jump) {
                const auto take = taken(s);
                if (!take) return result_;
                if (!*take) {
                    ++k;
                    continue;
                }
                if (s.target == code_.hi) return finished(false, 0);
                if (code_.lo <= s.target && s.target < code_.hi) {
                    const auto to = code_.at.find(s.target);
                    if (to == code_.at.end()) return refused("jump into an instruction");
                    k = to->second;
                    continue;
                }
                return finished(true, s.target);
            }
            if (!execute(s)) return result_;
            ++k;
        }
        return finished(false, 0);
    }

  private:
    // ---- stopping
    bool stop_refused(std::string why) {
        result_ = {RunResult::Status::Refused, {}, -1, std::move(why)};
        return false;
    }
    RunResult refused(std::string why) {
        stop_refused(std::move(why));
        return result_;
    }
    RunResult finished(bool leaves, std::uint32_t target) {
        result_ = {RunResult::Status::Finished, {leaves, target}, -1, {}};
        return result_;
    }

    // ---- values
    std::string memory_key(const Memory& m) const {
        std::string out;
        for (const Memory::Piece& p : m.pieces) {
            if (p.base < 0) out += p.literal;
            else out += "<" + bases_[static_cast<std::size_t>(p.base)] + ">";
        }
        return out;
    }

    static std::string register_name(int reg) { return std::string(kWord[static_cast<std::size_t>(reg)]); }

    Value read_register(int reg) const {
        const auto& v = regs_[static_cast<std::size_t>(reg)];
        if (v) return *v;
        Value r;
        r.kind = Value::Kind::Register;
        r.text = register_name(reg);
        return r;
    }

    // the operand's value; a refusal is recorded and nullopt returned
    std::optional<Value> value_of(const Operand& op) {
        Value v;
        switch (op.form) {
            case Operand::Form::Word:
            case Operand::Form::Byte: return read_register(op.reg);   // (a byte register is read as its whole register)
            case Operand::Form::Number:
                v.number = op.number;
                return v;
            case Operand::Form::Memory: {
                const std::string key = memory_key(op.memory);
                if (const auto it = temporaries_.find(key); it != temporaries_.end()) return it->second;
                v.kind = Value::Kind::Memory;
                v.text = key;
                return v;
            }
            case Operand::Form::Other: break;
        }
        stop_refused("operand " + op.text);
        return std::nullopt;
    }

    // what the unknown is in this run; if it is not decided, the run stops asking for it
    std::optional<int> atom(const Key& key) {
        const int id = keys_.intern(key);
        if (static_cast<std::size_t>(id) < assignment_.size() && assignment_[static_cast<std::size_t>(id)] >= 0) {
            return assignment_[static_cast<std::size_t>(id)];
        }
        result_ = {RunResult::Status::NeedsKey, {}, id, {}};
        return std::nullopt;
    }

    std::optional<int> boolean(const Value& v) {
        if (v.kind == Value::Kind::Number) return static_cast<int>(v.number & 1);
        if (v.kind == Value::Kind::Memory) {
            Key k;
            k.kind = Key::Kind::Boolean;
            k.a = v.text;
            return atom(k);
        }
        stop_refused("not a Boolean: " + v.label());
        return std::nullopt;
    }

    static Value number(std::int64_t n) {
        Value v;
        v.number = n;
        return v;
    }

    static Flags logical(bool zero, std::optional<bool> carry) {
        Flags f;
        f.kind = Flags::Kind::Logical;
        f.zero = zero;
        f.carry = carry;
        return f;
    }

    // ---- a conditional jump: is it taken? (nullopt: the run stopped)
    std::optional<bool> taken(const Step& s) {
        if (s.jump == Jump::Always) return true;
        if (flags_.kind == Flags::Kind::Unknown) {
            stop_refused("jump without flags");
            return std::nullopt;
        }
        if (flags_.kind == Flags::Kind::Comparison) {
            const int c = flags_.comparison;
            switch (s.jump) {
                case Jump::Equal: return c == 1;
                case Jump::NotEqual: return c != 1;
                case Jump::Below: return c == 0;
                case Jump::AboveOrEqual: return c != 0;
                case Jump::Above: return c == 2;
                default: return c != 2;   // BelowOrEqual
            }
        }
        if (s.jump == Jump::Equal || s.jump == Jump::NotEqual) {
            if (!flags_.zero) {
                stop_refused("zero flag not known");
                return std::nullopt;
            }
            return s.jump == Jump::Equal ? *flags_.zero : !*flags_.zero;
        }
        if (s.jump == Jump::Below || s.jump == Jump::AboveOrEqual) {
            if (!flags_.carry) {
                stop_refused("carry not known");
                return std::nullopt;
            }
            return s.jump == Jump::Below ? *flags_.carry : !*flags_.carry;
        }
        stop_refused(s.text.substr(0, s.text.find(' ')) + " after a logical operation");
        return std::nullopt;
    }

    // ---- the other instructions; false: the run stopped
    bool execute(const Step& s) {
        switch (s.kind) {
            case Step::Kind::Jump: break;   // (handled by run)
            case Step::Kind::Refuse: return stop_refused(s.why);
            case Step::Kind::MoveToBase:
                bases_[static_cast<std::size_t>(s.base)] = s.source.form == Operand::Form::Memory ? memory_key(s.source.memory) : s.source.text;
                return true;
            case Step::Kind::MoveToRegister: {
                const auto v = value_of(s.source);
                if (!v) return false;
                regs_[static_cast<std::size_t>(s.reg)] = *v;
                return true;
            }
            case Step::Kind::MoveToTemporary: {
                const auto v = value_of(s.source);
                if (!v) return false;
                temporaries_[memory_key(s.other.memory)] = *v;
                return true;
            }
            case Step::Kind::Increment: {
                const auto& v = regs_[static_cast<std::size_t>(s.reg)];
                if (!v || v->kind != Value::Kind::Number) return stop_refused("inc of an unknown");
                regs_[static_cast<std::size_t>(s.reg)] = number(v->number + 1);
                return true;
            }
            case Step::Kind::Nothing: return true;
            case Step::Kind::FlipBoolean: {
                const auto b = boolean(read_register(s.reg));
                if (!b) return false;
                regs_[static_cast<std::size_t>(s.reg)] = number(1 - *b);
                flags_ = logical(*b == 1, false);
                return true;
            }
            case Step::Kind::Combine: {
                const auto x = boolean(read_register(s.reg));
                if (!x) return false;
                const auto operand = value_of(s.source);
                if (!operand) return false;
                const auto y = boolean(*operand);
                if (!y) return false;
                const int r = s.conjunction ? (*x & *y) : (*x | *y);
                regs_[static_cast<std::size_t>(s.reg)] = number(r);
                flags_ = logical(r == 0, false);
                return true;
            }
            case Step::Kind::Mask: {
                const auto& v = regs_[static_cast<std::size_t>(s.reg)];
                if (!v || v->kind != Value::Kind::Memory) return stop_refused("and of " + (v ? v->label() : "an unknown"));
                Key k;
                k.kind = Key::Kind::Masked;
                k.a = v->text;
                k.mask = s.source.number;
                const auto bit = atom(k);
                if (!bit) return false;
                Value masked;
                masked.kind = Value::Kind::Masked;
                masked.number = *bit;
                regs_[static_cast<std::size_t>(s.reg)] = masked;
                flags_ = logical(*bit == 0, false);
                return true;
            }
            case Step::Kind::Compare: {
                const auto x = value_of(s.source);
                if (!x) return false;
                const auto y = value_of(s.other);
                if (!y) return false;
                flags_.kind = Flags::Kind::Comparison;
                if (x->kind == Value::Kind::Number && y->kind == Value::Kind::Number) {
                    flags_.comparison = x->number < y->number ? 0 : x->number == y->number ? 1 : 2;
                    return true;
                }
                Key k;
                k.kind = Key::Kind::Comparison;
                k.a = x->label();
                k.b = y->label();
                const auto outcome = atom(k);
                if (!outcome) return false;
                flags_.comparison = *outcome;
                return true;
            }
            case Step::Kind::TestBoolean: {
                const auto v = value_of(s.source);
                if (!v) return false;
                const auto b = boolean(*v);
                if (!b) return false;
                flags_ = logical(*b == 0, false);
                return true;
            }
            case Step::Kind::ShiftRight: {
                const auto& v = regs_[static_cast<std::size_t>(s.reg)];
                if (!v) return stop_refused("shr of an unknown");
                const auto b = boolean(*v);
                if (!b) return false;
                // the bit shifted out is the Boolean; what is left is 0 (the zero flag would follow, but no jump on it is
                // known to be made after a shift, so it is left unknown and a jump on it is refused)
                flags_ = logical(false, *b == 1);
                flags_.zero.reset();
                regs_[static_cast<std::size_t>(s.reg)] = number(0);
                return true;
            }
        }
        return true;
    }

    const Compiled& code_;
    KeyTable& keys_;
    const Assignment& assignment_;
    std::array<std::string, 2> bases_;
    std::array<std::optional<Value>, 4> regs_;
    std::map<std::string, Value> temporaries_;   // lookup only
    Flags flags_;
    RunResult result_;
};

// ---- every case a stretch asks for --------------------------------------------------------------------

// The unknowns a stretch asks for, over every run it takes to decide them all, and which ways out it has.
struct Cases {
    std::set<int> keys;
    bool falls_through = false;
    bool jumps_out = false;
};

// Refused (why) or the cases.
std::variant<Cases, std::string> enumerate(const Compiled& code, KeyTable& keys) {
    Cases cases;
    std::vector<Assignment> todo{Assignment{}};
    std::size_t finished_count = 0;
    while (!todo.empty()) {
        const Assignment assignment = std::move(todo.back());
        todo.pop_back();
        const RunResult r = Interpreter(code, keys, assignment).run();
        switch (r.status) {
            case RunResult::Status::Refused: return r.why;
            case RunResult::Status::NeedsKey: {
                const std::size_t id = static_cast<std::size_t>(r.key);
                for (int v = 0; v < keys.key(r.key).domain(); ++v) {
                    Assignment next = assignment;
                    if (next.size() <= id) next.resize(id + 1, -1);
                    next[id] = static_cast<std::int8_t>(v);
                    todo.push_back(std::move(next));
                }
                break;
            }
            case RunResult::Status::Finished:
                ++finished_count;
                (r.flow.leaves ? cases.jumps_out : cases.falls_through) = true;
                for (std::size_t id = 0; id < assignment.size(); ++id) {
                    if (assignment[id] >= 0) cases.keys.insert(static_cast<int>(id));
                }
                break;
        }
        if (finished_count + todo.size() > kMaxCases) return std::string("too many cases");
    }
    return cases;
}

}  // namespace

Verdict same(const Stretch& original, const Stretch& compiled) {
    const Compiled w = compile_stretch(original);
    const Compiled g = compile_stretch(compiled);
    KeyTable keys;
    const auto a = enumerate(w, keys);
    if (const auto* why = std::get_if<std::string>(&a)) return {false, std::nullopt, *why};
    const auto b = enumerate(g, keys);
    if (const auto* why = std::get_if<std::string>(&b)) return {false, std::nullopt, *why};
    const Cases& cw = std::get<Cases>(a);
    const Cases& cg = std::get<Cases>(b);

    // every unknown either stretch asks about, in a fixed order
    std::vector<int> ids;
    {
        std::set<int> all = cw.keys;
        all.insert(cg.keys.begin(), cg.keys.end());
        ids.assign(all.begin(), all.end());
        std::sort(ids.begin(), ids.end(), [&](int x, int y) { return keys.key(x) < keys.key(y); });
    }
    std::size_t n = 1;
    for (const int id : ids) {
        n *= static_cast<std::size_t>(keys.key(id).domain());
        if (n > kMaxCases) break;   // (more than can be tried; no need to know by how much)
    }
    if (ids.empty() || !(cw.falls_through && cw.jumps_out)) return {false, std::nullopt, "not a condition: it always goes the same way"};
    if (n > kMaxCases) return {false, std::nullopt, "too many cases"};

    // try every combination of outcomes (the last unknown changes fastest)
    std::set<std::pair<std::uint32_t, std::uint32_t>> elses;
    Assignment assignment(keys.size(), -1);
    for (std::size_t case_number = 0; case_number < n; ++case_number) {
        std::size_t rest = case_number;
        for (std::size_t i = ids.size(); i-- > 0;) {
            const std::size_t domain = static_cast<std::size_t>(keys.key(ids[i]).domain());
            assignment[static_cast<std::size_t>(ids[i])] = static_cast<std::int8_t>(rest % domain);
            rest /= domain;
        }
        const RunResult rw = Interpreter(w, keys, assignment).run();
        const RunResult rg = Interpreter(g, keys, assignment).run();
        if (rw.status != RunResult::Status::Finished || rg.status != RunResult::Status::Finished) {
            return {false, std::nullopt, "a case one of them does not have"};
        }
        if (rw.flow.leaves != rg.flow.leaves) {
            std::string with;
            for (const int id : ids) {
                if (!with.empty()) with += ", ";
                with += describe(keys.key(id)) + "=" + value_name(keys.key(id), assignment[static_cast<std::size_t>(id)]);
            }
            return {false, std::nullopt,
                    std::format("with {} the original {}, the compiled {}", with, rw.flow.leaves ? "goes out" : "goes on",
                                rg.flow.leaves ? "goes out" : "goes on")};
        }
        if (rw.flow.leaves) elses.insert({rw.flow.target, rg.flow.target});
    }
    std::set<std::uint32_t> from_original;
    std::set<std::uint32_t> from_compiled;
    for (const auto& [x, y] : elses) {
        from_original.insert(x);
        from_compiled.insert(y);
    }
    if (from_original.size() > 1 || from_compiled.size() > 1) return {false, std::nullopt, "more than one way out"};
    Verdict v;
    v.same = true;
    if (!elses.empty()) v.else_targets = ElseTargets{elses.begin()->first, elses.begin()->second};
    return v;
}

}  // namespace cb::exact::habits::boolx
