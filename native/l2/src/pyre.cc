#include "pyre.hpp"

#include <algorithm>
#include <cctype>
#include <cstring>
#include <map>
#include <mutex>
#include <stdexcept>
#include <type_traits>

#include "pystr.hpp"
#include "../../ported/include/py_unicode.hpp"
#include "value.hpp"

namespace re {

// ---------------------------------------------------------------------------------------- the tree
enum class NK { Char, Any, Class, Bol, Eol, Start, End, WordB, NWordB, Group, Seq, Alt, Rep, Look, Back };

struct ClassItem {
    uint32_t lo, hi;      // a range; or a category when cat != 0
    char cat = 0;         // 'w' 'W' 'd' 'D' 's' 'S'
};

struct Node {
    NK k;
    uint32_t ch = 0;
    bool neg = false;                       // a negated class / a negative look
    bool behind = false;                    // a lookbehind
    int width = 0;                          // its width in characters
    std::vector<ClassItem> items;
    int group = -1;                         // Group's number, Back's number
    int min = 0, max = -1;                  // Rep (-1: no bound)
    bool lazy = false;
    std::vector<std::shared_ptr<Node>> kids;
};
using NP = std::shared_ptr<Node>;

// (fixed) \w and \d as Python's for a str pattern: \d a Unicode decimal digit (str.isdecimal), \w str.isalnum
// (a letter, or a decimal, digit or numeric character) or '_' -- by CPython's own tables (py_unicode.hpp);
// under re.ASCII (flag 256) the ASCII ones alone
static bool is_word_cp(uint32_t c, bool ascii = false) {
    if (c < 0x80 || ascii) return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
    char32_t u = static_cast<char32_t>(c);
    return py::unicode_::isalpha(u) || py::unicode_::isdecimal(u) || py::unicode_::isdigit(u) || py::unicode_::isnumeric(u);
}
static bool is_digit_cp(uint32_t c, bool ascii = false) {
    if (c < 0x80 || ascii) return c >= '0' && c <= '9';
    return py::unicode_::isdecimal(static_cast<char32_t>(c));
}

static uint32_t fold(uint32_t c) { return c >= 'A' && c <= 'Z' ? c + 32 : c; }

static bool is_space_ascii(uint32_t c) { return c == ' ' || (c >= '	' && c <= ''); }

static bool cat_has(char cat, uint32_t c, bool ascii) {
    switch (cat) {
        case 'w': return is_word_cp(c, ascii);
        case 'W': return !is_word_cp(c, ascii);
        case 'd': return is_digit_cp(c, ascii);
        case 'D': return !is_digit_cp(c, ascii);
        case 's': return ascii ? is_space_ascii(c) : py::is_space_cp(c);
        case 'S': return ascii ? !is_space_ascii(c) : !py::is_space_cp(c);
    }
    return false;
}

static bool class_has(const Node& n, uint32_t c, bool icase, bool ascii) {
    bool in = false;
    for (auto& it : n.items) {
        if (it.cat ? cat_has(it.cat, c, ascii) : (c >= it.lo && c <= it.hi) ||
                                              (icase && ((fold(c) >= it.lo && fold(c) <= it.hi) ||
                                                         (c >= 'a' && c <= 'z' && c - 32 >= it.lo && c - 32 <= it.hi)))) {
            in = true;
            break;
        }
    }
    return in != n.neg;
}

// ---------------------------------------------------------------------------------------- the parser
struct Parser {
    const std::string& p;
    size_t i = 0;
    int flags;
    int ngroups = 0;
    std::vector<std::pair<std::string, int>> names;
    Parser(const std::string& pat, int f) : p(pat), flags(f) {}

    [[noreturn]] void fail(const std::string& m) { py::raise("ValueError", "re: " + m + " in " + py::str_repr(p)); }
    bool more() const { return i < p.size(); }
    uint32_t next() { return py::decode_cp(p, i); }

    NP alt() {
        std::vector<NP> branches{seq()};
        while (more() && p[i] == '|') {
            i++;
            branches.push_back(seq());
        }
        if (branches.size() == 1) return branches[0];
        auto n = std::make_shared<Node>();
        n->k = NK::Alt;
        n->kids = branches;
        return n;
    }
    NP seq() {
        auto n = std::make_shared<Node>();
        n->k = NK::Seq;
        while (more() && p[i] != '|' && p[i] != ')') {
            NP a = atom();
            if (!a) continue;
            a = quant(a);
            n->kids.push_back(a);
        }
        return n;
    }
    bool parse_int(size_t& j, int& v) {
        size_t s = j;
        v = 0;
        while (j < p.size() && p[j] >= '0' && p[j] <= '9') v = v * 10 + (p[j++] - '0');
        return j > s;
    }
    NP quant(NP a) {
        while (more()) {
            char c = p[i];
            int mn, mx;
            if (c == '*') mn = 0, mx = -1, i++;
            else if (c == '+') mn = 1, mx = -1, i++;
            else if (c == '?') mn = 0, mx = 1, i++;
            else if (c == '{') {
                size_t j = i + 1;
                int a1 = 0, b1 = -1;
                bool has_a = parse_int(j, a1);
                if (j < p.size() && p[j] == '}') {
                    if (!has_a) return a;
                    b1 = a1;
                } else if (j < p.size() && p[j] == ',') {
                    j++;
                    int bb;
                    if (parse_int(j, bb)) b1 = bb;
                    if (j >= p.size() || p[j] != '}') return a;
                    if (!has_a) a1 = 0;
                } else
                    return a;
                i = j + 1;
                mn = a1, mx = b1;
            } else
                return a;
            auto r = std::make_shared<Node>();
            r->k = NK::Rep;
            r->min = mn;
            r->max = mx;
            if (more() && p[i] == '?') {
                r->lazy = true;
                i++;
            }
            r->kids.push_back(a);
            a = r;
        }
        return a;
    }
    void escape_item(std::vector<ClassItem>& items, uint32_t c) {
        switch (c) {
            case 'w': case 'W': case 'd': case 'D': case 's': case 'S': {
                ClassItem it{0, 0, (char)c};
                items.push_back(it);
                return;
            }
        }
        uint32_t v = simple_escape(c);
        items.push_back({v, v, 0});
    }
    uint32_t simple_escape(uint32_t c) {
        switch (c) {
            case 'n': return '\n';
            case 't': return '\t';
            case 'r': return '\r';
            case 'f': return '\f';
            case 'v': return '\v';
            case 'a': return 7;
            case '0': {   // \0, \0o, \0oo: octal (as sre_parse)
                uint32_t v = 0;
                for (int k = 0; k < 2 && more() && p[i] >= '0' && p[i] <= '7'; k++) v = v * 8 + (uint32_t)(p[i++] - '0');
                return v;
            }
            case '1': case '2': case '3': case '4': case '5': case '6': case '7': {
                // in a class: up to three octal digits (\041); outside one the caller has taken back-references
                uint32_t v = c - '0';
                for (int k = 0; k < 2 && more() && p[i] >= '0' && p[i] <= '7'; k++) v = v * 8 + (uint32_t)(p[i++] - '0');
                return v;
            }
            case 'x': {
                uint32_t v = 0;
                for (int k = 0; k < 2; k++) {
                    char h = p[i++];
                    v = v * 16 + (uint32_t)(h <= '9' ? h - '0' : (h | 32) - 'a' + 10);
                }
                return v;
            }
        }
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) fail("bad escape \\" + std::string(1, (char)c));
        return c;
    }
    NP cls() {
        auto n = std::make_shared<Node>();
        n->k = NK::Class;
        if (more() && p[i] == '^') {
            n->neg = true;
            i++;
        }
        bool first = true;
        while (true) {
            if (!more()) fail("unterminated character set");
            if (p[i] == ']' && !first) {
                i++;
                break;
            }
            first = false;
            uint32_t lo;
            if (p[i] == '\\') {
                i++;
                uint32_t c = next();
                if (std::strchr("wWdDsS", (int)c) && c < 0x80) {
                    escape_item(n->items, c);
                    continue;
                }
                lo = simple_escape(c);
            } else
                lo = next();
            if (more() && p[i] == '-' && i + 1 < p.size() && p[i + 1] != ']') {
                i++;
                uint32_t hi;
                if (p[i] == '\\') {
                    i++;
                    hi = simple_escape(next());
                } else
                    hi = next();
                n->items.push_back({lo, hi, 0});
            } else
                n->items.push_back({lo, lo, 0});
        }
        return n;
    }
    NP make(NK k) {
        auto n = std::make_shared<Node>();
        n->k = k;
        return n;
    }
    NP atom() {
        uint32_t c = next();
        switch (c) {
            case '.': return make(NK::Any);
            case '^': return make(NK::Bol);
            case '$': return make(NK::Eol);
            case '[': return cls();
            case '(': return group();
            case '\\': {
                uint32_t e = next();
                switch (e) {
                    case 'b': return make(NK::WordB);
                    case 'B': return make(NK::NWordB);
                    case 'A': return make(NK::Start);
                    case 'Z': case 'z': return make(NK::End);   // (\z: Python 3.14's spelling of \Z)
                    case 'w': case 'W': case 'd': case 'D': case 's': case 'S': {
                        auto n = make(NK::Class);
                        escape_item(n->items, e);
                        return n;
                    }
                }
                // \ooo (three octal digits, the first 0-3) is a character, as sre_parse reads it; else a back-reference
                if (e >= '0' && e <= '3' && i + 1 < p.size() && p[i] >= '0' && p[i] <= '7' && p[i + 1] >= '0' && p[i + 1] <= '7') {
                    auto n = make(NK::Char);
                    n->ch = (uint32_t)((e - '0') * 64 + (p[i] - '0') * 8 + (p[i + 1] - '0'));
                    i += 2;
                    return n;
                }
                if (e >= '1' && e <= '9') {
                    int v = (int)(e - '0');
                    if (more() && p[i] >= '0' && p[i] <= '9') v = v * 10 + (p[i++] - '0');
                    auto n = make(NK::Back);
                    n->group = v;
                    return n;
                }
                auto n = make(NK::Char);
                n->ch = simple_escape(e);
                return n;
            }
            default: {
                auto n = make(NK::Char);
                n->ch = c;
                return n;
            }
        }
    }
    NP group() {
        if (more() && p[i] == '?') {
            i++;
            char c = p[i];
            if (c == ':') {
                i++;
                NP inner = alt();
                close();
                return inner;
            }
            if (c == '=' || c == '!') {
                i++;
                auto n = make(NK::Look);
                n->neg = c == '!';
                n->kids.push_back(alt());
                close();
                return n;
            }
            if (c == '<' && i + 1 < p.size() && (p[i + 1] == '=' || p[i + 1] == '!')) {
                bool neg = p[i + 1] == '!';
                i += 2;
                auto n = make(NK::Look);
                n->neg = neg;
                n->behind = true;
                n->kids.push_back(alt());
                n->width = width(n->kids[0].get());
                if (n->width < 0) fail("look-behind requires fixed-width pattern");
                close();
                return n;
            }
            if (c == 'P' && i + 1 < p.size() && p[i + 1] == '<') {
                i += 2;
                size_t e = p.find('>', i);
                std::string name = p.substr(i, e - i);
                i = e + 1;
                int g = ++ngroups;
                names.push_back({name, g});
                auto n = make(NK::Group);
                n->group = g;
                n->kids.push_back(alt());
                close();
                return n;
            }
            if (c == 'P' && i + 1 < p.size() && p[i + 1] == '=') {
                i += 2;
                size_t e = p.find(')', i);
                std::string name = p.substr(i, e - i);
                i = e + 1;
                for (auto& nm : names)
                    if (nm.first == name) {
                        auto n = make(NK::Back);
                        n->group = nm.second;
                        return n;
                    }
                fail("unknown group name");
            }
            // inline flags: (?ms) at the start
            int f = 0;
            while (more() && p[i] != ')' && p[i] != ':') {
                char fl = p[i++];
                if (fl == 'm') f |= M;
                else if (fl == 's') f |= S;
                else if (fl == 'i') f |= I;
                else if (fl == 'x' || fl == 'u' || fl == 'a' || fl == 'L') {
                } else
                    fail("unknown flag");
            }
            if (more() && p[i] == ':') {          // (?s:...) -- scoped: taken as global here
                i++;
                flags |= f;
                NP inner = alt();
                close();
                return inner;
            }
            close();
            flags |= f;
            return nullptr;
        }
        int g = ++ngroups;
        auto n = make(NK::Group);
        n->group = g;
        n->kids.push_back(alt());
        close();
        return n;
    }
    void close() {
        if (!more() || p[i] != ')') fail("missing )");
        i++;
    }
    int width(const Node* n) {
        switch (n->k) {
            case NK::Char: case NK::Any: case NK::Class: return 1;
            case NK::Bol: case NK::Eol: case NK::Start: case NK::End: case NK::WordB: case NK::NWordB: case NK::Look: return 0;
            case NK::Group: return width(n->kids[0].get());
            case NK::Seq: {
                int w = 0;
                for (auto& k : n->kids) {
                    int x = width(k.get());
                    if (x < 0) return -1;
                    w += x;
                }
                return w;
            }
            case NK::Alt: {
                int w = -2;
                for (auto& k : n->kids) {
                    int x = width(k.get());
                    if (x < 0 || (w != -2 && x != w)) return -1;
                    w = x;
                }
                return w < 0 ? 0 : w;
            }
            case NK::Rep: {
                if (n->min != n->max) return -1;
                int x = width(n->kids[0].get());
                return x < 0 ? -1 : x * n->min;
            }
            default: return -1;
        }
    }
};

// ---------------------------------------------------------------------------------------- the program
// The tree is compiled to a flat program run by a backtracking machine with an explicit stack on the heap, so
// matching never depends on native recursion (only a look-around nests a call, and only as deep as the pattern
// nests them). Every choice point and every change to a capture or counter is an entry on the one stack; a failure
// pops entries, undoing the changes, until it reaches a choice point.
enum class Op : uint8_t {
    Char, Any, Class, Bol, Eol, Start, End, WordB, NWordB,
    Split,     // try a (the next instruction), on failure b
    Jmp,       // go to a
    GStart,    // remember the position as group a's start
    GEnd,      // group a := (start, here)
    Back,      // back-reference to group a
    Look,      // look-around n; its body follows, ends with SubEnd; continue at a
    SubEnd,    // end of a look-around's body
    RepChar,   // n repeated mn..mx (lazy or greedy) over a single-character node
    RepInit,   // a general repeat (registers a: passes done, a+1: where the last pass began): none done yet
    RepTest,   // the loop head, as sre's MAX_UNTIL / MIN_UNTIL: another pass of the body, or leave at b
    RepAgain,  // start of a further pass: remember where it began
    RepEnd,    // end of a pass: count it, back to the head at b
    Match
};

struct Ins {
    Op op;
    bool lazy = false;
    const Node* n = nullptr;
    int a = 0, b = 0;
    int mn = 0, mx = -1;
};

struct Prog {
    std::vector<Ins> code;
    int ncap = 0;        // slots 0 .. ncap-1 are the captures, then each group's start, then the repeats' registers
    int nvals = 0;
};

struct Compiler {
    Prog& P;
    int ngroups;
    int nextreg;
    Compiler(Prog& p, int ng) : P(p), ngroups(ng) {
        P.ncap = 2 * (ng + 1);
        nextreg = P.ncap + ng + 1;
    }
    int emit(Op op, const Node* n = nullptr, int a = 0, int b = 0) {
        Ins i;
        i.op = op;
        i.n = n;
        i.a = a;
        i.b = b;
        P.code.push_back(i);
        return (int)P.code.size() - 1;
    }
    int here() const { return (int)P.code.size(); }
    void comp(const Node* n) {
        switch (n->k) {
            case NK::Char: emit(Op::Char, n); break;
            case NK::Any: emit(Op::Any, n); break;
            case NK::Class: emit(Op::Class, n); break;
            case NK::Bol: emit(Op::Bol); break;
            case NK::Eol: emit(Op::Eol); break;
            case NK::Start: emit(Op::Start); break;
            case NK::End: emit(Op::End); break;
            case NK::WordB: emit(Op::WordB); break;
            case NK::NWordB: emit(Op::NWordB); break;
            case NK::Seq:
                for (auto& k : n->kids) comp(k.get());
                break;
            case NK::Alt: {
                std::vector<int> jmps;
                for (size_t b = 0; b < n->kids.size(); b++) {
                    if (b + 1 < n->kids.size()) {
                        int sp = emit(Op::Split);
                        P.code[sp].a = sp + 1;
                        comp(n->kids[b].get());
                        jmps.push_back(emit(Op::Jmp));
                        P.code[sp].b = here();
                    } else
                        comp(n->kids[b].get());
                }
                for (int j : jmps) P.code[j].a = here();
                break;
            }
            case NK::Group:
                emit(Op::GStart, n, n->group);
                comp(n->kids[0].get());
                emit(Op::GEnd, n, n->group);
                break;
            case NK::Back: emit(Op::Back, n, n->group); break;
            case NK::Look: {
                int l = emit(Op::Look, n);
                comp(n->kids[0].get());
                emit(Op::SubEnd);
                P.code[l].a = here();
                break;
            }
            case NK::Rep: {
                const Node* c = n->kids[0].get();
                if (c->k == NK::Char || c->k == NK::Any || c->k == NK::Class) {
                    int r = emit(Op::RepChar, c);
                    P.code[r].mn = n->min;
                    P.code[r].mx = n->max;
                    P.code[r].lazy = n->lazy;
                    break;
                }
                int reg = nextreg;
                nextreg += 2;
                emit(Op::RepInit, n, reg);
                int t = emit(Op::RepTest, n, reg);
                P.code[t].mn = n->min;
                P.code[t].mx = n->max;
                P.code[t].lazy = n->lazy;
                emit(Op::RepAgain, n, reg);
                comp(c);
                emit(Op::RepEnd, n, reg, t);
                P.code[t].b = here();
                break;
            }
        }
    }
};

static std::shared_ptr<Prog> compile_prog(const Node* root, int ngroups) {
    auto p = std::make_shared<Prog>();
    Compiler c(*p, ngroups);
    c.comp(root);
    c.emit(Op::Match);
    p->nvals = c.nextreg;
    return p;
}

Regex::Regex(const std::string& pattern, int flags) : pattern_(pattern) {
    Parser ps(pattern, flags);
    root_ = ps.alt();
    if (ps.more()) ps.fail("unbalanced parenthesis");
    ngroups_ = ps.ngroups;
    flags_ = ps.flags;
    names_ = ps.names;
    prog_ = compile_prog(root_.get(), ngroups_);
}

// ---------------------------------------------------------------------------------------- the matcher
// One stack entry, 16 bytes: a = (pc or slot) << 3 | kind; b = a count; c = a position or a saved value.
enum EntKind : uint32_t { E_CHOICE = 0, E_UNDO = 1, E_GREEDY = 2, E_LAZY = 3, E_START = 4 };
struct Ent {
    uint32_t a;
    uint32_t b;
    uint64_t c;
    uint32_t kind() const { return a & 7; }
    uint32_t arg() const { return a >> 3; }
};
static inline Ent mk(uint32_t kind, uint32_t arg, uint32_t b, uint64_t c) { return Ent{(arg << 3) | kind, b, c}; }

struct Ctx {
    const std::string& s;
    size_t end;
    int flags;
    const Prog& P;
    std::vector<long> vals;     // the captures, then the registers
    std::vector<Ent> st;
    size_t nch = 0;             // choice points on the stack
    int depth = 0;              // look-around nesting
    // the final test, at the program's end
    int mode = 0;
    bool must_advance = false;
    size_t start = 0;
    size_t found_end = 0;

    Ctx(const std::string& s_, size_t e, int f, const Prog& p) : s(s_), end(e), flags(f), P(p), vals((size_t)p.nvals, -1) {}

    uint32_t cp_at(size_t i, size_t& next) const {
        next = i;
        uint32_t c = py::decode_cp(s, next);
        if (next > end) next = end;
        return c;
    }
    bool word_before(size_t i) const {
        if (i == 0) return false;
        size_t p = py::prev_cp(s, i), q;
        return is_word_cp(cp_at(p, q), (flags & A) != 0);
    }
    bool word_at(size_t i) const {
        if (i >= end) return false;
        size_t q;
        return is_word_cp(cp_at(i, q), (flags & A) != 0);
    }
    // one character node at i: where it ends, or npos
    size_t one(const Node* n, size_t i) const {
        if (i >= end) return std::string::npos;
        size_t nx;
        uint32_t c = cp_at(i, nx);
        switch (n->k) {
            case NK::Char:
                if (c == n->ch || ((flags & I) && fold(c) == fold(n->ch))) return nx;
                return std::string::npos;
            case NK::Any: return (c != '\n' || (flags & S)) ? nx : std::string::npos;
            case NK::Class: return class_has(*n, c, (flags & I) != 0, (flags & A) != 0) ? nx : std::string::npos;
            default: return std::string::npos;
        }
    }

    bool logging() const { return depth > 0 || nch > 0; }
    void setv(int slot, long v) {
        if (logging()) st.push_back(mk(E_UNDO, (uint32_t)slot, 0, (uint64_t)vals[(size_t)slot]));
        vals[(size_t)slot] = v;
    }
    void push_choice(int pc, size_t pos) {
        st.push_back(mk(E_CHOICE, (uint32_t)pc, 0, pos));
        nch++;
    }
    // the position of the character before pos in the run of characters that began at `from`
    size_t step_back(size_t from, size_t pos) const {
        if ((unsigned char)s[pos - 1] < 0x80) return pos - 1;
        size_t p = py::prev_cp(s, pos);
        if (p >= from) {
            size_t q;
            cp_at(p, q);
            if (q == pos) return p;
        }
        size_t cur = from, last = from;     // (an odd byte sequence: walk the run forward)
        while (cur < pos) {
            last = cur;
            size_t nx;
            cp_at(cur, nx);
            cur = nx;
        }
        return last;
    }
    // pop everything above base, undoing the changes
    void unwind(size_t base) {
        while (st.size() > base) {
            const Ent& e = st.back();
            switch (e.kind()) {
                case E_UNDO: vals[e.arg()] = (long)e.c; break;
                case E_CHOICE: case E_GREEDY: case E_LAZY: nch--; break;
                default: break;
            }
            st.pop_back();
        }
    }
    // a look-around matched: drop its choice points, keep the changes it made so a later failure undoes them
    void keep_changes(size_t base) {
        size_t w = base;
        for (size_t r = base; r < st.size(); r++) {
            uint32_t k = st[r].kind();
            if (k == E_UNDO) st[w++] = st[r];
            else if (k != E_START) nch--;
        }
        st.resize(w);
        if (depth == 0 && nch == 0) st.resize(base);   // (nothing left to go back to)
    }

    // run the program from pc at i; true at its end (the look-around's end must be at `target` when that is >= 0).
    // Entries above `base` are the run's own: false leaves the stack at base.
    bool exec(int pc, size_t i, long target, size_t base) {
        const std::vector<Ins>& code = P.code;
        for (;;) {
            const Ins& in = code[(size_t)pc];
            bool ok = true;
            switch (in.op) {
                case Op::Char: case Op::Any: case Op::Class: {
                    size_t j = one(in.n, i);
                    if (j == std::string::npos) ok = false;
                    else i = j, pc++;
                    break;
                }
                case Op::Bol:
                    if (i == 0 || ((flags & M) && s[i - 1] == '\n')) pc++;
                    else ok = false;
                    break;
                case Op::Eol:
                    if (i == end || (i < end && s[i] == '\n' && ((flags & M) || i + 1 == end))) pc++;
                    else ok = false;
                    break;
                case Op::Start:
                    if (i == 0) pc++;
                    else ok = false;
                    break;
                case Op::End:
                    if (i == end) pc++;
                    else ok = false;
                    break;
                case Op::WordB:
                    if (word_before(i) != word_at(i)) pc++;
                    else ok = false;
                    break;
                case Op::NWordB:
                    if (word_before(i) == word_at(i)) pc++;
                    else ok = false;
                    break;
                case Op::Split:
                    push_choice(in.b, i);
                    pc = in.a;
                    break;
                case Op::Jmp: pc = in.a; break;
                case Op::GStart:
                    setv(P.ncap + in.a, (long)i);
                    pc++;
                    break;
                case Op::GEnd:
                    setv(2 * in.a, vals[(size_t)(P.ncap + in.a)]);
                    setv(2 * in.a + 1, (long)i);
                    pc++;
                    break;
                case Op::Back: {
                    int g = in.a;
                    if (vals[(size_t)(2 * g)] < 0) {
                        ok = false;
                        break;
                    }
                    size_t len = (size_t)(vals[(size_t)(2 * g + 1)] - vals[(size_t)(2 * g)]);
                    if (i + len > end || s.compare(i, len, s, (size_t)vals[(size_t)(2 * g)], len) != 0) ok = false;
                    else i += len, pc++;
                    break;
                }
                case Op::Look: {
                    const Node* ln = in.n;
                    size_t from = i;
                    bool can = true;
                    if (ln->behind)
                        for (int w = 0; w < ln->width; w++) {
                            if (from == 0) {
                                can = false;
                                break;
                            }
                            from = py::prev_cp(s, from);
                        }
                    bool hit = false;
                    size_t b2 = st.size();
                    if (can) {
                        depth++;
                        hit = exec(pc + 1, from, ln->behind ? (long)i : -1, b2);
                        depth--;
                    }
                    if (ln->neg) {
                        if (hit) {
                            unwind(b2);
                            ok = false;
                        } else
                            pc = in.a;
                    } else if (hit) {
                        keep_changes(b2);
                        pc = in.a;
                    } else
                        ok = false;
                    break;
                }
                case Op::SubEnd:
                    if (target >= 0 && (long)i != target) ok = false;
                    else return true;
                    break;
                case Op::RepChar: {
                    const Node* c = in.n;
                    int mn = in.mn, mx = in.mx;
                    size_t p = i;
                    if (!in.lazy) {
                        long cnt = 0;
                        while (mx < 0 || cnt < mx) {
                            size_t j = one(c, p);
                            if (j == std::string::npos) break;
                            p = j;
                            cnt++;
                        }
                        if (cnt < mn) {
                            ok = false;
                            break;
                        }
                        if (cnt > mn) {
                            st.push_back(mk(E_START, 0, 0, i));
                            st.push_back(mk(E_GREEDY, (uint32_t)pc, (uint32_t)cnt, p));
                            nch++;
                        }
                    } else {
                        int cnt = 0;
                        for (; cnt < mn; cnt++) {
                            size_t j = one(c, p);
                            if (j == std::string::npos) break;
                            p = j;
                        }
                        if (cnt < mn) {
                            ok = false;
                            break;
                        }
                        if (mx < 0 || cnt < mx) {
                            st.push_back(mk(E_LAZY, (uint32_t)pc, (uint32_t)cnt, p));
                            nch++;
                        }
                    }
                    i = p;
                    pc++;
                    break;
                }
                case Op::RepInit:
                    setv(in.a, 0);
                    setv(in.a + 1, -1);
                    pc++;
                    break;
                case Op::RepTest: {
                    // (as sre: a pass that matched nothing is counted but not repeated; the tail is tried from there)
                    long cnt = vals[(size_t)in.a];
                    bool again = (in.mx < 0 || cnt < in.mx) && (long)i != vals[(size_t)(in.a + 1)];
                    if (cnt < in.mn) pc += 2;                         // (not enough passes yet: the body, no questions)
                    else if (!in.lazy) {
                        if (again) {
                            push_choice(in.b, i);
                            pc++;
                        } else
                            pc = in.b;
                    } else {
                        if (again) push_choice(pc + 1, i);
                        pc = in.b;
                    }
                    break;
                }
                case Op::RepAgain:
                    setv(in.a + 1, (long)i);
                    pc++;
                    break;
                case Op::RepEnd:
                    setv(in.a, vals[(size_t)in.a] + 1);
                    pc = in.b;
                    break;
                case Op::Match:
                    if ((mode == 1 && i != end) || (must_advance && i == start)) ok = false;
                    else {
                        found_end = i;
                        return true;
                    }
                    break;
            }
            if (ok) continue;
            // a failure: back to the latest choice point
            for (;;) {
                if (st.size() <= base) return false;
                Ent& e = st.back();
                uint32_t k = e.kind();
                if (k == E_UNDO) {
                    vals[e.arg()] = (long)e.c;
                    st.pop_back();
                } else if (k == E_CHOICE) {
                    pc = (int)e.arg();
                    i = (size_t)e.c;
                    st.pop_back();
                    nch--;
                    break;
                } else if (k == E_GREEDY) {
                    const Ins& ri = code[e.arg()];
                    size_t from = (size_t)st[st.size() - 2].c;
                    size_t np = step_back(from, (size_t)e.c);
                    pc = (int)e.arg() + 1;
                    i = np;
                    e.c = np;
                    e.b--;
                    if ((int)e.b <= ri.mn) {
                        st.pop_back();
                        st.pop_back();
                        nch--;
                    }
                    break;
                } else {   // E_LAZY
                    const Ins& ri = code[e.arg()];
                    size_t j = (ri.mx >= 0 && (int)e.b >= ri.mx) ? std::string::npos : one(ri.n, (size_t)e.c);
                    if (j == std::string::npos) {
                        st.pop_back();
                        nch--;
                        continue;
                    }
                    pc = (int)e.arg() + 1;
                    i = j;
                    e.c = j;
                    e.b++;
                    if (ri.mx >= 0 && (int)e.b >= ri.mx) {
                        st.pop_back();
                        nch--;
                    }
                    break;
                }
            }
        }
    }
};


// the code-point index of a byte offset: a table of counts at every 128th byte, built on the first call and shared by
// the matches of one subject, then at most 127 bytes counted -- constant time, not a rescan from the start
struct CpIndex {
    std::vector<uint32_t> blocks;     // blocks[k]: the code points that begin before byte 128*k
    bool built = false;
};
static const size_t CP_BLOCK = 128;

long Match::cp_offset(long byte) const {
    if (byte < 0) return -1;
    const std::string& t = *s;
    size_t b = (size_t)byte < t.size() ? (size_t)byte : t.size();
    if (!cpx) cpx = std::make_shared<CpIndex>();
    if (!cpx->built) {
        cpx->blocks.assign(t.size() / CP_BLOCK + 1, 0);
        uint32_t n = 0;
        for (size_t i = 0; i < t.size(); i++) {
            if (i % CP_BLOCK == 0) cpx->blocks[i / CP_BLOCK] = n;
            if (((unsigned char)t[i] & 0xC0) != 0x80) n++;
        }
        if (t.size() % CP_BLOCK == 0) cpx->blocks[t.size() / CP_BLOCK] = n;
        cpx->built = true;
    }
    uint32_t n = cpx->blocks[b / CP_BLOCK];
    for (size_t i = b / CP_BLOCK * CP_BLOCK; i < b; i++)
        if (((unsigned char)t[i] & 0xC0) != 0x80) n++;
    return (long)n;
}

std::string Match::group(int g) const {
    if (!ok || caps[2 * g] < 0) return "";
    return s->substr((size_t)caps[2 * g], (size_t)(caps[2 * g + 1] - caps[2 * g]));
}

// mode: 0 match, 1 fullmatch, 2 search
Match Regex::run(const std::shared_ptr<const std::string>& sp, const std::shared_ptr<CpIndex>& cpx, size_t pos, size_t endpos, int mode, bool must_advance) const {
    const std::string& s = *sp;
    if (endpos > s.size()) endpos = s.size();
    Match out;
    out.s = sp;
    out.cpx = cpx;
    Ctx c(s, endpos, flags_, *prog_);
    c.mode = mode;
    const size_t ncap = (size_t)(2 * (ngroups_ + 1));
    size_t start = pos;
    // (a pattern that begins with a literal: candidate starts found by that character)
    const Node* r = root_.get();
    uint32_t lead = 0;
    if (mode == 2 && !(flags_ & I)) {
        const Node* f = r;
        while (f->k == NK::Seq && !f->kids.empty()) f = f->kids[0].get();
        while (f->k == NK::Group) f = f->kids[0].get();
        if (f->k == NK::Seq && !f->kids.empty()) f = f->kids[0].get();
        if (f->k == NK::Char && f->ch < 0x80) lead = f->ch;
    }
    while (start <= endpos) {
        if (lead) {
            size_t k = s.find((char)lead, start);
            if (k == std::string::npos || k >= endpos) break;
            if (k != start) must_advance = false;
            start = k;
        }
        c.must_advance = must_advance;
        c.start = start;
        c.st.clear();
        c.nch = 0;
        c.depth = 0;
        if (c.exec(0, start, -1, 0)) {
            c.vals[0] = (long)start;
            c.vals[1] = (long)c.found_end;
            out.caps.assign(c.vals.begin(), c.vals.begin() + (long)ncap);
            out.ok = true;
            return out;
        }
        if (mode != 2 || start >= endpos) break;
        py::decode_cp(s, start);
        must_advance = false;
        std::fill(c.vals.begin(), c.vals.begin() + (long)ncap, -1);
    }
    out.caps.assign(ncap, -1);
    return out;
}

static std::shared_ptr<const std::string> own(const std::string& s) { return std::make_shared<const std::string>(s); }
static std::shared_ptr<CpIndex> new_index() { return std::make_shared<CpIndex>(); }
Match Regex::match(const std::string& s, size_t pos, size_t endpos) const { return run(own(s), new_index(), pos, endpos, 0); }
Match Regex::fullmatch(const std::string& s, size_t pos, size_t endpos) const { return run(own(s), new_index(), pos, endpos, 1); }
Match Regex::search(const std::string& s, size_t pos, size_t endpos) const { return run(own(s), new_index(), pos, endpos, 2); }

std::vector<Match> Regex::finditer(const std::string& s0) const {
    std::vector<Match> out;
    auto sp = own(s0);
    auto cpx = new_index();
    const std::string& s = *sp;
    size_t pos = 0;
    bool adv = false;
    while (pos <= s.size()) {
        Match m = run(sp, cpx, pos, s.size(), 2, adv);
        if (!m) break;
        out.push_back(m);
        adv = m.end() == m.start();
        pos = (size_t)m.end();
    }
    return out;
}

std::vector<std::string> Regex::findall(const std::string& s) const {
    std::vector<std::string> out;
    for (auto& m : finditer(s)) out.push_back(m.group(ngroups_ >= 1 ? 1 : 0));
    return out;
}

std::vector<std::vector<std::string>> Regex::findall_groups(const std::string& s) const {
    std::vector<std::vector<std::string>> out;
    for (auto& m : finditer(s)) {
        std::vector<std::string> g;
        for (int k = 1; k <= ngroups_; k++) g.push_back(m.group(k));
        out.push_back(g);
    }
    return out;
}

std::string Regex::sub_fn(const std::function<std::string(const Match&)>& fn, const std::string& s0, int count) const {
    auto sp = own(s0);
    auto cpx = new_index();
    const std::string& s = *sp;
    std::string out;
    size_t pos = 0, last = 0;
    bool adv = false;
    int n = 0;
    while (pos <= s.size() && (count <= 0 || n < count)) {
        Match m = run(sp, cpx, pos, s.size(), 2, adv);
        if (!m) break;
        out.append(s, last, (size_t)m.start() - last);
        out += fn(m);
        last = (size_t)m.end();
        adv = m.end() == m.start();
        pos = (size_t)m.end();
        n++;
    }
    out.append(s, last, std::string::npos);
    return out;
}

std::string Regex::sub(const std::string& repl, const std::string& s, int count) const {
    // the template: \1 .. \99, \g<n>, \g<name>, and the character escapes
    struct Piece {
        std::string text;
        int group = -1;
    };
    std::vector<Piece> pieces;
    std::string lit;
    for (size_t i = 0; i < repl.size(); i++) {
        char c = repl[i];
        if (c != '\\' || i + 1 >= repl.size()) {
            lit += c;
            continue;
        }
        // (Python's template: parsed whole before any match, so a bad one raises even where nothing matches)
        size_t at = i;
        char e = repl[++i];
        auto bad = [&](const std::string& m) { throw Error("PatternError", m + " at position " + std::to_string(at)); };
        auto oct = [](char ch) { return ch >= '0' && ch <= '7'; };
        auto group_ref = [&](int g) {
            if (g > ngroups_) bad("invalid group reference " + std::to_string(g));
            pieces.push_back({lit});
            lit.clear();
            pieces.push_back({"", g});
        };
        if (e == '0') {   // \0, \0o, \0oo: an octal character
            int v = 0;
            for (int k = 0; k < 2 && i + 1 < repl.size() && oct(repl[i + 1]); k++) v = v * 8 + (repl[++i] - '0');
            lit += (char)v;
        } else if (e >= '1' && e <= '9') {
            if (oct(e) && i + 2 < repl.size() && oct(repl[i + 1]) && oct(repl[i + 2])) {   // \ooo: an octal character
                int v = (e - '0') * 64 + (repl[i + 1] - '0') * 8 + (repl[i + 2] - '0');
                if (v > 0377) bad(std::string("octal escape value \\") + repl.substr(i, 3) + " outside of range 0-0o377");
                i += 2;
                lit += (char)v;
            } else {
                int g = e - '0';
                if (i + 1 < repl.size() && repl[i + 1] >= '0' && repl[i + 1] <= '9') g = g * 10 + (repl[++i] - '0');
                group_ref(g);
            }
        } else if (e == 'g') {
            if (i + 1 >= repl.size() || repl[i + 1] != '<') bad("missing <");
            size_t close = repl.find('>', i + 2);
            if (close == std::string::npos) bad("missing >, unterminated name");
            std::string name = repl.substr(i + 2, close - i - 2);
            if (name.empty()) bad("missing group name");
            int g = -1;
            if (std::all_of(name.begin(), name.end(), [](char ch) { return ch >= '0' && ch <= '9'; })) {
                g = std::stoi(name);
            } else {
                bool ident = !(name[0] >= '0' && name[0] <= '9');
                for (char ch : name) ident = ident && (std::isalnum((unsigned char)ch) || ch == '_' || (unsigned char)ch >= 0x80);
                if (!ident) bad("bad character in group name '" + name + "'");
                for (auto& nm : names_)
                    if (nm.first == name) g = nm.second;
                if (g < 0) throw Error("IndexError", "unknown group name '" + name + "'");
            }
            group_ref(g);
            i = close;
        } else if (e == 'n') lit += '\n';
        else if (e == 't') lit += '\t';
        else if (e == 'r') lit += '\r';
        else if (e == 'f') lit += '\f';
        else if (e == 'v') lit += '\v';
        else if (e == 'a') lit += '\a';
        else if (e == 'b') lit += '\b';
        else if (e == '\\') lit += '\\';
        else if ((e >= 'a' && e <= 'z') || (e >= 'A' && e <= 'Z')) bad(std::string("bad escape \\") + e);
        else {
            lit += '\\';
            lit += e;
        }
    }
    pieces.push_back({lit});
    return sub_fn([&](const Match& m) {
        std::string o;
        for (auto& p : pieces) o += p.group >= 0 ? m.group(p.group) : p.text;
        return o;
    }, s, count);
}

std::string escape(const std::string& s) {
    static const char* special = "()[]{}?*+-|^$\\.&~# \t\n\r\v\f";
    std::string out;
    for (char c : s) {
        if (c && std::strchr(special, c)) out += '\\';
        out += c;
    }
    return out;
}

const Regex& rx(const std::string& pattern, int flags) {
    static std::mutex mu;
    static std::map<std::pair<std::string, int>, std::unique_ptr<Regex>> cache;
    std::lock_guard<std::mutex> lock(mu);
    auto& slot = cache[{pattern, flags}];
    if (!slot) slot.reset(new Regex(pattern, flags));
    return *slot;
}

}  // namespace re
