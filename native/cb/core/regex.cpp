#include "cb/core/regex.hpp"

#include <algorithm>
#include <array>
#include <format>

namespace cb {

namespace {

// ---- byte sets ----------------------------------------------------------------------------------------

using Set = std::array<std::uint64_t, 4>;

constexpr bool set_has(const Set& s, unsigned char b) { return ((s[b >> 6] >> (b & 63)) & 1u) != 0; }
constexpr void set_add(Set& s, unsigned b) { s[b >> 6] |= std::uint64_t{1} << (b & 63); }
constexpr void set_add_range(Set& s, unsigned lo, unsigned hi) {
    for (unsigned b = lo; b <= hi; ++b) set_add(s, b);
}
constexpr void set_union(Set& s, const Set& o) {
    for (std::size_t i = 0; i < 4; ++i) s[i] |= o[i];
}
constexpr Set set_negate(Set s) {
    for (auto& w : s) w = ~w;
    return s;
}
constexpr bool set_full(const Set& s) { return s[0] == ~0ull && s[1] == ~0ull && s[2] == ~0ull && s[3] == ~0ull; }

constexpr bool is_alpha(unsigned b) { return (b >= 'a' && b <= 'z') || (b >= 'A' && b <= 'Z'); }
constexpr bool is_word_byte(unsigned char b) { return is_alpha(b) || (b >= '0' && b <= '9') || b == '_'; }
constexpr unsigned other_case(unsigned b) {
    return (b >= 'a' && b <= 'z') ? b - 32 : (b >= 'A' && b <= 'Z') ? b + 32 : b;
}
constexpr unsigned char fold(unsigned char b) { return (b >= 'A' && b <= 'Z') ? static_cast<unsigned char>(b + 32) : b; }

Set digit_set() {
    Set s{};
    set_add_range(s, '0', '9');
    return s;
}
Set word_set() {
    Set s = digit_set();
    set_add_range(s, 'a', 'z');
    set_add_range(s, 'A', 'Z');
    set_add(s, '_');
    return s;
}
Set space_set() {
    Set s{};
    for (unsigned b : {' ', '\t', '\n', '\v', '\f', '\r'}) set_add(s, b);
    return s;
}

// ---- the syntax tree ----------------------------------------------------------------------------------

enum class K { Empty, Byte, Set, TextStart, TextEnd, LineStart, LineEnd, WordB, NotWordB, Group, Seq, Alt, Repeat, Look, BackRef };

constexpr std::uint32_t kInf = 0xFFFFFFFFu;

struct Node {
    K kind = K::Empty;
    unsigned char byte = 0;
    Set set{};
    std::size_t group = 0;                       // Group: its number; BackRef: the number referred to
    std::uint32_t min = 0, max = 0;              // Repeat
    bool greedy = true;                          // Repeat
    bool negative = false, behind = false;       // Look
    bool icase = false;                          // BackRef
    std::vector<std::unique_ptr<Node>> kids;
};
using NodeP = std::unique_ptr<Node>;

NodeP make(K k) {
    auto n = std::make_unique<Node>();
    n->kind = k;
    return n;
}

// Can the node match without consuming a byte?
bool nullable(const Node& n) {
    switch (n.kind) {
        case K::Byte:
        case K::Set: return false;
        case K::Group: return nullable(*n.kids[0]);
        case K::Seq: return std::all_of(n.kids.begin(), n.kids.end(), [](const NodeP& k) { return nullable(*k); });
        case K::Alt: return std::any_of(n.kids.begin(), n.kids.end(), [](const NodeP& k) { return nullable(*k); });
        case K::Repeat: return n.min == 0 || nullable(*n.kids[0]);
        default: return true;   // anchors, boundaries, lookarounds, backreferences, empty
    }
}

// The exact number of bytes the node always consumes, if that is one number (for lookbehind).
std::optional<std::size_t> fixed_width(const Node& n) {
    switch (n.kind) {
        case K::Byte:
        case K::Set: return 1;
        case K::Group: return fixed_width(*n.kids[0]);
        case K::Seq: {
            std::size_t total = 0;
            for (const auto& k : n.kids) {
                auto w = fixed_width(*k);
                if (!w) return std::nullopt;
                total += *w;
            }
            return total;
        }
        case K::Alt: {
            std::optional<std::size_t> first;
            for (const auto& k : n.kids) {
                auto w = fixed_width(*k);
                if (!w || (first && *first != *w)) return std::nullopt;
                first = w;
            }
            return first;
        }
        case K::Repeat: {
            if (n.min != n.max) return std::nullopt;
            auto w = fixed_width(*n.kids[0]);
            if (!w) return std::nullopt;
            return *w * n.min;
        }
        case K::BackRef: return std::nullopt;
        default: return 0;   // anchors, boundaries, lookarounds, empty
    }
}

// The bytes a match can begin with; returns whether the node can match empty (so the caller continues).
bool first_bytes(const Node& n, Set& out) {
    switch (n.kind) {
        case K::Byte: set_add(out, n.byte); return false;
        case K::Set: set_union(out, n.set); return false;
        case K::BackRef: set_union(out, set_negate(Set{})); return true;
        case K::Group: return first_bytes(*n.kids[0], out);
        case K::Seq:
            for (const auto& k : n.kids) {
                if (!first_bytes(*k, out)) return false;
            }
            return true;
        case K::Alt: {
            bool any_empty = false;
            for (const auto& k : n.kids) any_empty = first_bytes(*k, out) || any_empty;
            return any_empty;
        }
        case K::Repeat: {
            const bool kid_empty = first_bytes(*n.kids[0], out);
            return kid_empty || n.min == 0;
        }
        default: return true;   // zero-width things pass through
    }
}

// Does every match have to start at text position 0?
bool begins_with_text_start(const Node& n) {
    switch (n.kind) {
        case K::TextStart: return true;
        case K::Group: return begins_with_text_start(*n.kids[0]);
        case K::Seq: return !n.kids.empty() && begins_with_text_start(*n.kids[0]);
        case K::Alt:
            return std::all_of(n.kids.begin(), n.kids.end(), [](const NodeP& k) { return begins_with_text_start(*k); });
        default: return false;
    }
}

// ---- the parser ---------------------------------------------------------------------------------------

constexpr std::size_t kMaxDepth = 200;
constexpr std::uint32_t kMaxCount = 10000;

class Parser {
public:
    Parser(std::string_view pattern, RegexFlags flags) : p_(pattern), flags_(flags) {}

    NodeP parse() {
        inline_flags();
        NodeP root = parse_alt(0);
        if (i_ < p_.size()) fail("unmatched ')'", i_);
        for (std::size_t ref : refs_) {
            if (ref > ngroups_) fail(std::format("backreference \\{} to a group that does not exist", ref), 0);
        }
        return root;
    }
    std::size_t group_count() const { return ngroups_; }
    std::vector<std::pair<std::string, std::size_t>> take_names() { return std::move(names_); }

private:
    [[noreturn]] void fail(const std::string& what, std::size_t at) const {
        throw ParseError("<regex>", 1, static_cast<std::uint32_t>(at + 1), what + " in pattern '" + std::string(p_) + "'");
    }
    bool more() const { return i_ < p_.size(); }
    char peek(std::size_t ahead = 0) const { return i_ + ahead < p_.size() ? p_[i_ + ahead] : '\0'; }
    bool eat(char c) {
        if (more() && p_[i_] == c) {
            ++i_;
            return true;
        }
        return false;
    }
    bool starts_with(std::string_view s) const { return p_.substr(i_).starts_with(s); }
    bool icase() const { return has_flag(flags_, RegexFlags::IgnoreCase); }

    // A leading group of inline flags: (?i) (?m) (?s) and combinations.
    void inline_flags() {
        while (starts_with("(?")) {
            std::size_t j = i_ + 2;
            RegexFlags add = RegexFlags::None;
            while (j < p_.size() && (p_[j] == 'i' || p_[j] == 'm' || p_[j] == 's')) {
                add = add | (p_[j] == 'i' ? RegexFlags::IgnoreCase : p_[j] == 'm' ? RegexFlags::Multiline : RegexFlags::DotAll);
                ++j;
            }
            if (j == i_ + 2 || j >= p_.size() || p_[j] != ')') return;
            flags_ = flags_ | add;
            i_ = j + 1;
        }
    }

    NodeP parse_alt(std::size_t depth) {
        if (depth > kMaxDepth) fail("pattern nests too deeply", i_);
        std::vector<NodeP> branches;
        branches.push_back(parse_seq(depth));
        while (eat('|')) branches.push_back(parse_seq(depth));
        if (branches.size() == 1) return std::move(branches[0]);
        NodeP alt = make(K::Alt);
        alt->kids = std::move(branches);
        return alt;
    }

    NodeP parse_seq(std::size_t depth) {
        NodeP seq = make(K::Seq);
        while (more() && peek() != '|' && peek() != ')') {
            NodeP atom = parse_atom(depth);
            seq->kids.push_back(parse_quantifier(std::move(atom)));
        }
        return seq;
    }

    // `{n}`, `{n,}`, `{n,m}` at the cursor; if it is not one, nothing is consumed and false is returned.
    bool try_braces(std::uint32_t& lo, std::uint32_t& hi) {
        std::size_t j = i_ + 1;
        auto number = [&](std::uint32_t& out) {
            const std::size_t start = j;
            std::uint64_t v = 0;
            while (j < p_.size() && p_[j] >= '0' && p_[j] <= '9') {
                v = v * 10 + static_cast<std::uint64_t>(p_[j] - '0');
                if (v > kMaxCount) fail("repeat count too large", i_);
                ++j;
            }
            out = static_cast<std::uint32_t>(v);
            return j > start;
        };
        std::uint32_t a = 0, b = 0;
        if (!number(a)) return false;
        if (j < p_.size() && p_[j] == '}') {
            lo = hi = a;
            i_ = j + 1;
            return true;
        }
        if (j >= p_.size() || p_[j] != ',') return false;
        ++j;
        if (j < p_.size() && p_[j] == '}') {
            lo = a;
            hi = kInf;
            i_ = j + 1;
            return true;
        }
        if (!number(b) || j >= p_.size() || p_[j] != '}') return false;
        if (b < a) fail("repeat range is backwards", i_);
        lo = a;
        hi = b;
        i_ = j + 1;
        return true;
    }

    NodeP parse_quantifier(NodeP atom) {
        std::uint32_t lo = 0, hi = 0;
        const std::size_t at = i_;
        if (eat('*')) { lo = 0; hi = kInf; }
        else if (eat('+')) { lo = 1; hi = kInf; }
        else if (eat('?')) { lo = 0; hi = 1; }
        else if (peek() == '{' && try_braces(lo, hi)) {}
        else return atom;
        switch (atom->kind) {
            case K::TextStart: case K::TextEnd: case K::LineStart: case K::LineEnd: case K::WordB: case K::NotWordB:
            case K::Look: case K::Empty:
                fail("nothing to repeat", at);
            default: break;
        }
        NodeP rep = make(K::Repeat);
        rep->min = lo;
        rep->max = hi;
        rep->greedy = !eat('?');
        rep->kids.push_back(std::move(atom));
        return rep;
    }

    NodeP byte_node(unsigned char b) const {
        if (icase() && is_alpha(b)) {
            NodeP n = make(K::Set);
            set_add(n->set, b);
            set_add(n->set, other_case(b));
            return n;
        }
        NodeP n = make(K::Byte);
        n->byte = b;
        return n;
    }

    NodeP set_node(const Set& s) const {
        NodeP n = make(K::Set);
        n->set = s;
        return n;
    }

    NodeP parse_atom(std::size_t depth) {
        const std::size_t at = i_;
        const char c = p_[i_++];
        switch (c) {
            case '(': return parse_group(depth, at);
            case '[': return parse_class();
            case '.': {
                Set s = set_negate(Set{});
                if (!has_flag(flags_, RegexFlags::DotAll)) s[0] &= ~(std::uint64_t{1} << '\n');
                return set_node(s);
            }
            case '^': return make(has_flag(flags_, RegexFlags::Multiline) ? K::LineStart : K::TextStart);
            case '$': return make(has_flag(flags_, RegexFlags::Multiline) ? K::LineEnd : K::TextEnd);
            case '\\': return parse_escape(at);
            case '*': case '+': case '?': fail("nothing to repeat", at);
            case '{': {
                --i_;
                std::uint32_t lo = 0, hi = 0;
                const std::size_t save = i_;
                if (try_braces(lo, hi)) fail("nothing to repeat", save);
                i_ = save + 1;   // not a quantifier: a literal brace
                return byte_node('{');
            }
            default: return byte_node(static_cast<unsigned char>(c));
        }
    }

    NodeP parse_group(std::size_t depth, std::size_t open_at) {
        auto close = [&](NodeP inner) {
            if (!eat(')')) fail("missing ')'", open_at);
            return inner;
        };
        if (eat('?')) {
            if (eat(':')) return close(parse_alt(depth + 1));
            if (peek() == '=' || peek() == '!') {
                NodeP look = make(K::Look);
                look->negative = p_[i_++] == '!';
                look->kids.push_back(parse_alt(depth + 1));
                return close(std::move(look));
            }
            if (peek() == '<' && (peek(1) == '=' || peek(1) == '!')) {
                ++i_;
                NodeP look = make(K::Look);
                look->behind = true;
                look->negative = p_[i_++] == '!';
                look->kids.push_back(parse_alt(depth + 1));
                if (!fixed_width(*look->kids[0])) fail("lookbehind must have a fixed width", open_at);
                return close(std::move(look));
            }
            if (peek() == '<' || (peek() == 'P' && peek(1) == '<')) {
                if (peek() == 'P') ++i_;
                ++i_;   // '<'
                std::string name;
                while (more() && peek() != '>') name += p_[i_++];
                if (!eat('>')) fail("unterminated group name", open_at);
                if (name.empty() || !(is_alpha(static_cast<unsigned char>(name[0])) || name[0] == '_'))
                    fail("bad group name", open_at);
                for (const auto& n : names_) {
                    if (n.first == name) fail("duplicate group name '" + name + "'", open_at);
                }
                const std::size_t number = ++ngroups_;
                names_.emplace_back(name, number);
                NodeP g = make(K::Group);
                g->group = number;
                g->kids.push_back(parse_alt(depth + 1));
                return close(std::move(g));
            }
            fail("unsupported group syntax", open_at);
        }
        const std::size_t number = ++ngroups_;
        NodeP g = make(K::Group);
        g->group = number;
        g->kids.push_back(parse_alt(depth + 1));
        return close(std::move(g));
    }

    int hex_value(char c) const {
        if (c >= '0' && c <= '9') return c - '0';
        if (c >= 'a' && c <= 'f') return c - 'a' + 10;
        if (c >= 'A' && c <= 'F') return c - 'A' + 10;
        return -1;
    }

    // The escape after a backslash that denotes ONE byte (shared by classes and the main grammar): the byte, or
    // nullopt if the escape is something else (a set, a boundary). `i_` is just after the escape letter.
    std::optional<unsigned char> byte_escape(char e, std::size_t at) {
        switch (e) {
            case 'n': return '\n';
            case 'r': return '\r';
            case 't': return '\t';
            case 'f': return '\f';
            case 'v': return '\v';
            case '0': return static_cast<unsigned char>(0);
            case 'x': {
                const int hi = hex_value(peek()), lo = hex_value(peek(1));
                if (hi < 0 || lo < 0) fail("\\x needs two hex digits", at);
                i_ += 2;
                return static_cast<unsigned char>(hi * 16 + lo);
            }
            default: break;
        }
        const auto u = static_cast<unsigned char>(e);
        if (!is_alpha(u) && !(e >= '0' && e <= '9')) return u;   // an escaped punctuation character is itself
        return std::nullopt;
    }

    NodeP parse_escape(std::size_t at) {
        if (!more()) fail("pattern ends with a backslash", at);
        const char e = p_[i_++];
        switch (e) {
            case 'd': return set_node(digit_set());
            case 'D': return set_node(set_negate(digit_set()));
            case 'w': return set_node(word_set());
            case 'W': return set_node(set_negate(word_set()));
            case 's': return set_node(space_set());
            case 'S': return set_node(set_negate(space_set()));
            case 'b': return make(K::WordB);
            case 'B': return make(K::NotWordB);
            case 'A': return make(K::TextStart);
            case 'z': return make(K::TextEnd);
            default: break;
        }
        if (e >= '1' && e <= '9') {
            NodeP r = make(K::BackRef);
            r->group = static_cast<std::size_t>(e - '0');
            r->icase = icase();
            refs_.push_back(r->group);
            return r;
        }
        if (auto b = byte_escape(e, at)) return byte_node(*b);
        fail(std::format("unknown escape \\{}", e), at);
    }

    NodeP parse_class() {
        const std::size_t open_at = i_ - 1;
        const bool negated = eat('^');
        Set set{};
        bool first = true;
        while (true) {
            if (!more()) fail("unterminated character class", open_at);
            if (peek() == ']' && !first) {
                ++i_;
                break;
            }
            first = false;
            // one class item: a byte (maybe the start of a range) or a whole set
            std::optional<unsigned> lo = class_item(set, open_at);
            if (!lo) {   // it was a set escape like \d
                if (peek() == '-' && peek(1) != ']') fail("a set cannot start a range", i_);
                continue;
            }
            if (peek() == '-' && peek(1) != ']' && i_ + 1 < p_.size()) {
                ++i_;   // '-'
                Set unused{};
                std::optional<unsigned> hi = class_item(unused, open_at);
                if (!hi) fail("a set cannot end a range", i_);
                if (*hi < *lo) fail("character class range is backwards", i_);
                set_add_range(set, *lo, *hi);
            } else {
                set_add(set, *lo);
            }
        }
        if (icase()) {
            Set folded = set;
            for (unsigned b = 'A'; b <= 'z'; ++b) {
                if (is_alpha(b) && set_has(set, static_cast<unsigned char>(b))) set_add(folded, other_case(b));
            }
            set = folded;
        }
        return set_node(negated ? set_negate(set) : set);
    }

    // Reads one item of a class. A single byte is returned; a set escape (\d \w \s and negations) is added to
    // `set` and nullopt returned.
    std::optional<unsigned> class_item(Set& set, std::size_t open_at) {
        if (!more()) fail("unterminated character class", open_at);
        const std::size_t at = i_;
        const auto c = static_cast<unsigned char>(p_[i_++]);
        if (c >= 0x80) fail("non-ASCII byte inside a character class", at);
        if (c != '\\') return c;
        if (!more()) fail("pattern ends with a backslash", at);
        const char e = p_[i_++];
        switch (e) {
            case 'd': set_union(set, digit_set()); return std::nullopt;
            case 'D': set_union(set, set_negate(digit_set())); return std::nullopt;
            case 'w': set_union(set, word_set()); return std::nullopt;
            case 'W': set_union(set, set_negate(word_set())); return std::nullopt;
            case 's': set_union(set, space_set()); return std::nullopt;
            case 'S': set_union(set, set_negate(space_set())); return std::nullopt;
            case 'b': return 8u;   // backspace inside a class
            default: break;
        }
        if (auto b = byte_escape(e, at)) return *b;
        fail(std::format("unknown escape \\{} in a character class", e), at);
    }

    std::string_view p_;
    std::size_t i_ = 0;
    RegexFlags flags_;
    std::size_t ngroups_ = 0;
    std::vector<std::pair<std::string, std::size_t>> names_;
    std::vector<std::size_t> refs_;
};

// ---- the program --------------------------------------------------------------------------------------

enum class Op : std::uint8_t {
    Char, Set, RepGreedy, RepLazy, Split, Jmp, Save, Mark, Progress, TextStart, TextEnd, LineStart, LineEnd,
    WordB, NotWordB, BackRef, Look, LookEnd, Match,
};

struct Inst {
    Op op = Op::Match;
    std::uint32_t a = 0, b = 0, c = 0;
};

constexpr std::size_t kMaxProgram = 500000;

}  // namespace

struct Regex::Program {
    std::string pattern;
    RegexFlags flags = RegexFlags::None;
    std::uint64_t budget = kDefaultRegexSteps;
    std::vector<Inst> code;
    std::vector<Set> sets;
    std::size_t groups = 0;       // capture groups, excluding group 0
    std::size_t slots = 0;        // capture slots (2 per group incl. 0) + loop registers
    std::shared_ptr<const std::vector<std::pair<std::string, std::size_t>>> names;
    bool anchored = false;        // can only match at text position 0
    bool prefilter = false;       // `first` lists the only bytes a match can start with
    Set first{};
};

namespace {

// ---- the compiler -------------------------------------------------------------------------------------

class Compiler {
public:
    explicit Compiler(Regex::Program& p) : p_(p) {}

    void run(const Node& root) {
        loops_ = 0;
        slots_base_ = 2 * (p_.groups + 1);
        emit(root);
        push({Op::Match, 0, 0, 0});
        p_.slots = slots_base_ + loops_;
    }

private:
    std::size_t push(Inst i) {
        if (p_.code.size() >= kMaxProgram) throw ParseError("<regex>", 1, 0, "pattern is too large");
        p_.code.push_back(i);
        return p_.code.size() - 1;
    }
    std::uint32_t here() const { return static_cast<std::uint32_t>(p_.code.size()); }
    std::uint32_t add_set(const Set& s) {
        p_.sets.push_back(s);
        return static_cast<std::uint32_t>(p_.sets.size() - 1);
    }

    void emit(const Node& n) {
        switch (n.kind) {
            case K::Empty: break;
            case K::Byte: push({Op::Char, n.byte, 0, 0}); break;
            case K::Set: push({Op::Set, add_set(n.set), 0, 0}); break;
            case K::TextStart: push({Op::TextStart, 0, 0, 0}); break;
            case K::TextEnd: push({Op::TextEnd, 0, 0, 0}); break;
            case K::LineStart: push({Op::LineStart, 0, 0, 0}); break;
            case K::LineEnd: push({Op::LineEnd, 0, 0, 0}); break;
            case K::WordB: push({Op::WordB, 0, 0, 0}); break;
            case K::NotWordB: push({Op::NotWordB, 0, 0, 0}); break;
            case K::BackRef: push({Op::BackRef, static_cast<std::uint32_t>(n.group), n.icase ? 1u : 0u, 0}); break;
            case K::Group:
                push({Op::Save, static_cast<std::uint32_t>(2 * n.group), 0, 0});
                emit(*n.kids[0]);
                push({Op::Save, static_cast<std::uint32_t>(2 * n.group + 1), 0, 0});
                break;
            case K::Seq:
                for (const auto& k : n.kids) emit(*k);
                break;
            case K::Alt: emit_alt(n); break;
            case K::Repeat: emit_repeat(n); break;
            case K::Look: emit_look(n); break;
        }
    }

    void emit_alt(const Node& n) {
        std::vector<std::size_t> jumps;
        for (std::size_t i = 0; i < n.kids.size(); ++i) {
            if (i + 1 < n.kids.size()) {
                const std::size_t split = push({Op::Split, here() + 1, 0, 0});
                emit(*n.kids[i]);
                jumps.push_back(push({Op::Jmp, 0, 0, 0}));
                p_.code[split].b = here();
            } else {
                emit(*n.kids[i]);
            }
        }
        for (std::size_t j : jumps) p_.code[j].a = here();
    }

    void emit_repeat(const Node& n) {
        const Node& child = *n.kids[0];
        if (child.kind == K::Byte || child.kind == K::Set) {
            Set s = child.set;
            if (child.kind == K::Byte) set_add(s, child.byte);
            push({n.greedy ? Op::RepGreedy : Op::RepLazy, add_set(s), n.min, n.max});
            return;
        }
        for (std::uint32_t i = 0; i < n.min; ++i) emit(child);
        if (n.max == kInf) {
            const std::size_t split = push({Op::Split, 0, 0, 0});
            const bool guard = nullable(child);
            const std::uint32_t reg = static_cast<std::uint32_t>(slots_base_ + loops_);
            if (guard) {
                ++loops_;
                push({Op::Mark, reg, 0, 0});
            }
            emit(child);
            if (guard) push({Op::Progress, reg, 0, 0});
            push({Op::Jmp, static_cast<std::uint32_t>(split), 0, 0});
            const std::uint32_t exit = here();
            p_.code[split] = n.greedy ? Inst{Op::Split, static_cast<std::uint32_t>(split + 1), exit, 0}
                                      : Inst{Op::Split, exit, static_cast<std::uint32_t>(split + 1), 0};
        } else {
            std::vector<std::size_t> splits;
            for (std::uint32_t i = n.min; i < n.max; ++i) {
                splits.push_back(push({Op::Split, 0, 0, 0}));
                emit(child);
            }
            const std::uint32_t exit = here();
            for (std::size_t s : splits) {
                p_.code[s] = n.greedy ? Inst{Op::Split, static_cast<std::uint32_t>(s + 1), exit, 0}
                                      : Inst{Op::Split, exit, static_cast<std::uint32_t>(s + 1), 0};
            }
        }
    }

    void emit_look(const Node& n) {
        const std::size_t look = push({Op::Look, 0, (n.negative ? 1u : 0u) | (n.behind ? 2u : 0u),
                                       static_cast<std::uint32_t>(n.behind ? *fixed_width(*n.kids[0]) : 0)});
        emit(*n.kids[0]);
        push({Op::LookEnd, 0, 0, 0});
        p_.code[look].a = here();
    }

    Regex::Program& p_;
    std::size_t loops_ = 0;
    std::size_t slots_base_ = 0;
};

// ---- the matcher --------------------------------------------------------------------------------------

struct BudgetExceeded {};

enum class FrameKind : std::uint8_t { Choice, Undo, Greedy, Lazy };

struct Frame {
    FrameKind kind;
    std::uint32_t pc;
    std::size_t pos;
    std::size_t extra;
};

class Machine {
public:
    Machine(const Regex::Program& prog, std::string_view subject)
        : prog_(prog), s_(reinterpret_cast<const unsigned char*>(subject.data())), n_(subject.size()),
          steps_left_(prog.budget), caps_(prog.slots, -1) {}

    // Try to match with the whole match starting at `start`. On success caps() holds the groups.
    bool run_at(std::size_t start, bool require_end) {
        std::fill(caps_.begin(), caps_.end(), -1);
        stack_.clear();
        caps_[0] = static_cast<std::ptrdiff_t>(start);
        return exec(0, start, 0, require_end);
    }
    const std::vector<std::ptrdiff_t>& caps() const { return caps_; }
    std::size_t size() const { return n_; }
    const unsigned char* text() const { return s_; }

private:
    void step() {
        if (steps_left_ == 0) throw BudgetExceeded{};
        --steps_left_;
    }

    bool word_at(std::size_t i) const { return i < n_ && is_word_byte(s_[i]); }

    void unwind(std::size_t floor) {
        while (stack_.size() > floor) {
            const Frame f = stack_.back();
            stack_.pop_back();
            if (f.kind == FrameKind::Undo) caps_[f.pc] = static_cast<std::ptrdiff_t>(f.extra);
        }
    }

    // After a successful positive lookaround: keep only the capture undo records above `floor`, so that the
    // choices made inside the lookaround can never be re-entered (it is atomic) but outer backtracking still
    // restores the captures it set.
    void keep_undo_only(std::size_t floor) {
        std::size_t out = floor;
        for (std::size_t i = floor; i < stack_.size(); ++i) {
            if (stack_[i].kind == FrameKind::Undo) stack_[out++] = stack_[i];
        }
        stack_.resize(out);
    }

    void set_slot(std::uint32_t slot, std::size_t pos) {
        stack_.push_back({FrameKind::Undo, slot, 0, static_cast<std::size_t>(caps_[slot])});
        caps_[slot] = static_cast<std::ptrdiff_t>(pos);
    }

    // Run from `pc` at `pos` until the program's Match (or, inside a lookaround, its LookEnd). Backtracking
    // never goes below stack height `floor`. Returns whether it matched.
    bool exec(std::uint32_t pc, std::size_t pos, std::size_t floor, bool require_end) {
        const Inst* code = prog_.code.data();
        for (;;) {
            step();
            const Inst& in = code[pc];
            bool ok = false;   // does this instruction succeed (and fall through to pc + 1)?
            switch (in.op) {
                case Op::Char:
                    if (pos < n_ && s_[pos] == in.a) { ++pos; ok = true; }
                    break;
                case Op::Set:
                    if (pos < n_ && set_has(prog_.sets[in.a], s_[pos])) { ++pos; ok = true; }
                    break;
                case Op::RepGreedy: {
                    const Set& set = prog_.sets[in.a];
                    const std::size_t room = n_ - pos;
                    const std::size_t limit = in.c == kInf ? room : std::min<std::size_t>(in.c, room);
                    std::size_t k = 0;
                    while (k < limit && set_has(set, s_[pos + k])) ++k;
                    spend(k >> 4);
                    if (k >= in.b) {
                        const std::size_t lo = pos + in.b, cur = pos + k;
                        if (cur > lo) stack_.push_back({FrameKind::Greedy, pc + 1, lo, cur});
                        pos = cur;
                        ok = true;
                    }
                    break;
                }
                case Op::RepLazy: {
                    const Set& set = prog_.sets[in.a];
                    std::size_t k = 0;
                    while (k < in.b && pos + k < n_ && set_has(set, s_[pos + k])) ++k;
                    if (k == in.b) {
                        pos += k;
                        if (in.c == kInf || in.b < in.c) stack_.push_back({FrameKind::Lazy, pc, pos, in.b});
                        ok = true;
                    }
                    break;
                }
                case Op::Split:
                    stack_.push_back({FrameKind::Choice, in.b, pos, 0});
                    pc = in.a;
                    continue;
                case Op::Jmp: pc = in.a; continue;
                case Op::Save:
                case Op::Mark:
                    set_slot(in.a, pos);
                    ok = true;
                    break;
                case Op::Progress: ok = caps_[in.a] != static_cast<std::ptrdiff_t>(pos); break;
                case Op::TextStart: ok = pos == 0; break;
                case Op::TextEnd: ok = pos == n_; break;
                case Op::LineStart: ok = pos == 0 || s_[pos - 1] == '\n'; break;
                case Op::LineEnd: ok = pos == n_ || s_[pos] == '\n'; break;
                case Op::WordB: ok = (pos > 0 && word_at(pos - 1)) != word_at(pos); break;
                case Op::NotWordB: ok = (pos > 0 && word_at(pos - 1)) == word_at(pos); break;
                case Op::BackRef: {
                    const std::ptrdiff_t gs = caps_[2 * in.a], ge = caps_[2 * in.a + 1];
                    if (gs < 0 || ge < 0) break;   // an unset group matches nothing
                    const std::size_t len = static_cast<std::size_t>(ge - gs);
                    if (len > n_ - pos) break;
                    bool same = true;
                    for (std::size_t i = 0; i < len && same; ++i) {
                        const unsigned char x = s_[static_cast<std::size_t>(gs) + i], y = s_[pos + i];
                        same = in.b != 0 ? fold(x) == fold(y) : x == y;
                    }
                    if (same) { pos += len; ok = true; }
                    break;
                }
                case Op::Look: {
                    const bool negative = (in.b & 1u) != 0, behind = (in.b & 2u) != 0;
                    const std::size_t base = stack_.size();
                    bool matched = false;
                    if (!behind) matched = exec(pc + 1, pos, base, false);
                    else if (pos >= in.c) matched = exec(pc + 1, pos - in.c, base, false);
                    if (matched) {
                        if (negative) unwind(base);
                        else keep_undo_only(base);
                    }
                    if (matched != negative) {
                        pc = in.a;
                        continue;
                    }
                    break;   // the assertion failed
                }
                case Op::LookEnd: return true;
                case Op::Match:
                    if (require_end && pos != n_) break;
                    caps_[1] = static_cast<std::ptrdiff_t>(pos);
                    return true;
            }
            if (ok) {
                ++pc;
                continue;
            }
            // ---- fail: resume from the most recent choice
            if (!backtrack(pc, pos, floor)) return false;
        }
    }

    void spend(std::size_t n) {
        if (steps_left_ < n) throw BudgetExceeded{};
        steps_left_ -= n;
    }

    // Pop frames until one gives a place to resume at; false when the stack is exhausted down to `floor`.
    bool backtrack(std::uint32_t& pc, std::size_t& pos, std::size_t floor) {
        const Inst* code = prog_.code.data();
        while (stack_.size() > floor) {
            step();
            const Frame f = stack_.back();
            stack_.pop_back();
            switch (f.kind) {
                case FrameKind::Undo: caps_[f.pc] = static_cast<std::ptrdiff_t>(f.extra); break;
                case FrameKind::Choice:
                    pc = f.pc;
                    pos = f.pos;
                    return true;
                case FrameKind::Greedy: {
                    // f.pos is the shortest allowed end, f.extra the end just tried: try one byte less
                    std::size_t cur = f.extra - 1;
                    const Inst& next = code[f.pc];
                    if (next.op == Op::Char) {   // skip ends the following literal cannot match at
                        while (cur > f.pos && !(cur < n_ && s_[cur] == next.a)) --cur;
                    }
                    if (cur > f.pos) stack_.push_back({FrameKind::Greedy, f.pc, f.pos, cur});
                    pc = f.pc;
                    pos = cur;
                    return true;
                }
                case FrameKind::Lazy: {
                    const Inst& rep = code[f.pc];
                    if (f.pos < n_ && set_has(prog_.sets[rep.a], s_[f.pos])) {
                        const std::size_t count = f.extra + 1;
                        const std::size_t next_pos = f.pos + 1;
                        if (rep.c == kInf || count < rep.c) stack_.push_back({FrameKind::Lazy, f.pc, next_pos, count});
                        pc = f.pc + 1;
                        pos = next_pos;
                        return true;
                    }
                    break;
                }
            }
        }
        return false;
    }

    const Regex::Program& prog_;
    const unsigned char* s_;
    std::size_t n_;
    std::uint64_t steps_left_;
    std::vector<std::ptrdiff_t> caps_;
    std::vector<Frame> stack_;
};

}  // namespace

// ---- RegexMatch ---------------------------------------------------------------------------------------

std::string_view RegexMatch::group(std::size_t g) const {
    if (!has(g)) return {};
    return subject_.substr(static_cast<std::size_t>(caps_[2 * g]), static_cast<std::size_t>(caps_[2 * g + 1] - caps_[2 * g]));
}

std::optional<std::string_view> RegexMatch::named(std::string_view name) const {
    if (names_ == nullptr) return std::nullopt;
    for (const auto& [n, index] : *names_) {
        if (n == name) return has(index) ? std::optional<std::string_view>(group(index)) : std::nullopt;
    }
    return std::nullopt;
}

// ---- Regex --------------------------------------------------------------------------------------------

Regex::Regex(std::string_view pattern, RegexFlags flags, std::uint64_t step_budget) {
    Parser parser(pattern, flags);
    NodeP root = parser.parse();
    auto prog = std::make_shared<Program>();
    prog->pattern = std::string(pattern);
    prog->flags = flags;
    prog->budget = step_budget;
    prog->groups = parser.group_count();
    auto names = parser.take_names();
    if (!names.empty()) {
        prog->names = std::make_shared<const std::vector<std::pair<std::string, std::size_t>>>(std::move(names));
    }
    Compiler(*prog).run(*root);
    prog->anchored = begins_with_text_start(*root);
    Set first{};
    const bool empty_ok = first_bytes(*root, first);
    prog->prefilter = !empty_ok && !set_full(first);
    prog->first = first;
    prog_ = std::move(prog);
}

Result<Regex> Regex::compile(std::string_view pattern, RegexFlags flags, std::uint64_t step_budget) {
    try {
        return Regex(pattern, flags, step_budget);
    } catch (const ParseError& e) {
        return e;
    }
}

const std::string& Regex::pattern() const { return prog_->pattern; }
std::size_t Regex::group_count() const { return prog_->groups; }

std::optional<std::size_t> Regex::group_index(std::string_view name) const {
    if (!prog_->names) return std::nullopt;
    for (const auto& [n, index] : *prog_->names) {
        if (n == name) return index;
    }
    return std::nullopt;
}

struct RegexMatchAccess {
    static RegexMatch make(std::string_view subject, const std::vector<std::ptrdiff_t>& caps, std::size_t groups,
                           const std::shared_ptr<const std::vector<std::pair<std::string, std::size_t>>>& names) {
        RegexMatch out;
        out.subject_ = subject;
        out.caps_.assign(caps.begin(), caps.begin() + static_cast<std::ptrdiff_t>(2 * (groups + 1)));
        out.names_ = names;
        return out;
    }
};

namespace {

enum class Mode { Search, Match, Full };

// The one place a match is attempted: from `from`, in the given mode, reusing `m`.
std::optional<RegexMatch> find_match(const Regex::Program& prog, Machine& m, std::string_view subject, std::size_t from,
                               Mode mode) {
    const std::size_t n = subject.size();
    if (from > n) return std::nullopt;
    auto done = [&]() { return RegexMatchAccess::make(subject, m.caps(), prog.groups, prog.names); };
    if (mode != Mode::Search) {
        if (m.run_at(from, mode == Mode::Full)) return done();
        return std::nullopt;
    }
    for (std::size_t s = from; s <= n; ++s) {
        if (prog.prefilter) {
            while (s < n && !set_has(prog.first, static_cast<unsigned char>(subject[s]))) ++s;
            if (s >= n) return std::nullopt;   // a match needs at least one byte
        }
        if (m.run_at(s, false)) return done();
        if (prog.anchored) return std::nullopt;
    }
    return std::nullopt;
}

}  // namespace

namespace {

Result<std::optional<RegexMatch>> attempt(const Regex::Program& prog, std::string_view subject, std::size_t from, Mode mode) {
    try {
        Machine m(prog, subject);
        return find_match(prog, m, subject, from, mode);
    } catch (const BudgetExceeded&) {
        return UnsupportedError("regex", from, std::format("step budget of {} exceeded matching /{}/", prog.budget, prog.pattern));
    }
}

}  // namespace

Result<std::optional<RegexMatch>> Regex::try_search(std::string_view s, std::size_t from) const {
    return attempt(*prog_, s, from, Mode::Search);
}
Result<std::optional<RegexMatch>> Regex::try_match(std::string_view s, std::size_t from) const {
    return attempt(*prog_, s, from, Mode::Match);
}
Result<std::optional<RegexMatch>> Regex::try_full_match(std::string_view s) const {
    return attempt(*prog_, s, 0, Mode::Full);
}

std::optional<RegexMatch> Regex::search(std::string_view s, std::size_t from) const { return try_search(s, from).value(); }
std::optional<RegexMatch> Regex::match(std::string_view s, std::size_t from) const { return try_match(s, from).value(); }
std::optional<RegexMatch> Regex::full_match(std::string_view s) const { return try_full_match(s).value(); }

std::size_t Regex::for_each_match(std::string_view subject, const std::function<bool(const RegexMatch&)>& fn) const {
    std::size_t count = 0;
    try {
        Machine m(*prog_, subject);
        std::size_t from = 0;
        while (from <= subject.size()) {
            auto found = find_match(*prog_, m, subject, from, Mode::Search);
            if (!found) break;
            ++count;
            if (!fn(*found)) break;
            const std::size_t end = found->end();
            from = end == found->start() ? end + 1 : end;   // an empty match moves the scan one byte on
        }
    } catch (const BudgetExceeded&) {
        throw UnsupportedError("regex", 0, std::format("step budget of {} exceeded matching /{}/", prog_->budget, prog_->pattern));
    }
    return count;
}

std::vector<std::string> Regex::find_all(std::string_view subject) const {
    std::vector<std::string> out;
    for_each_match(subject, [&](const RegexMatch& m) {
        out.emplace_back(m.whole());
        return true;
    });
    return out;
}

namespace {

// A replacement template pre-parsed into pieces.
struct Piece {
    std::string literal;
    std::optional<std::size_t> group;
};

std::vector<Piece> parse_template(const Regex& re, std::string_view t) {
    std::vector<Piece> pieces;
    std::string lit;
    auto flush = [&] {
        if (!lit.empty()) pieces.push_back({std::move(lit), std::nullopt});
        lit.clear();
    };
    for (std::size_t i = 0; i < t.size(); ++i) {
        if (t[i] != '$' || i + 1 >= t.size()) {
            lit += t[i];
            continue;
        }
        const char n = t[i + 1];
        if (n == '$') { lit += '$'; ++i; }
        else if (n == '&') { flush(); pieces.push_back({{}, 0}); ++i; }
        else if (n >= '0' && n <= '9') {
            const std::size_t g = static_cast<std::size_t>(n - '0');
            if (g > re.group_count()) throw UsageError(std::format("replacement refers to group {} but /{}/ has {}", g, re.pattern(), re.group_count()));
            flush();
            pieces.push_back({{}, g});
            ++i;
        } else if (n == '{') {
            const std::size_t close = t.find('}', i + 2);
            if (close == std::string_view::npos) throw UsageError("replacement has an unterminated ${name}");
            const std::string_view name = t.substr(i + 2, close - i - 2);
            auto g = re.group_index(name);
            if (!g) throw UsageError(std::format("replacement refers to unknown group '{}'", name));
            flush();
            pieces.push_back({{}, *g});
            i = close;
        } else {
            lit += '$';
        }
    }
    flush();
    return pieces;
}

}  // namespace

std::string Regex::replace_all(std::string_view subject, const std::function<std::string(const RegexMatch&)>& fn) const {
    std::string out;
    std::size_t copied = 0;
    for_each_match(subject, [&](const RegexMatch& m) {
        out.append(subject.substr(copied, m.start() - copied));
        out += fn(m);
        copied = m.end();
        return true;
    });
    out.append(subject.substr(copied));
    return out;
}

std::string Regex::replace_all(std::string_view subject, std::string_view replacement) const {
    const std::vector<Piece> pieces = parse_template(*this, replacement);
    return replace_all(subject, std::function<std::string(const RegexMatch&)>([&](const RegexMatch& m) {
                           std::string r;
                           for (const Piece& p : pieces) r += p.group ? std::string(m.group(*p.group)) : p.literal;
                           return r;
                       }));
}

std::string Regex::replace_first(std::string_view subject, std::string_view replacement) const {
    const std::vector<Piece> pieces = parse_template(*this, replacement);
    auto m = search(subject);
    if (!m) return std::string(subject);
    std::string out(subject.substr(0, m->start()));
    for (const Piece& p : pieces) out += p.group ? std::string(m->group(*p.group)) : p.literal;
    out.append(subject.substr(m->end()));
    return out;
}

std::vector<std::string> Regex::split(std::string_view subject) const {
    std::vector<std::string> out;
    std::size_t copied = 0;
    for_each_match(subject, [&](const RegexMatch& m) {
        if (m.end() == m.start()) return true;   // an empty match does not split
        out.emplace_back(subject.substr(copied, m.start() - copied));
        copied = m.end();
        return true;
    });
    out.emplace_back(subject.substr(copied));
    return out;
}

}  // namespace cb
