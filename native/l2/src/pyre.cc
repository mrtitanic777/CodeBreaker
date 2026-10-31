#include "pyre.hpp"

#include <cstring>
#include <map>
#include <mutex>
#include <stdexcept>
#include <type_traits>

#include "pystr.hpp"
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

static bool is_word_cp(uint32_t c) {
    if (c < 0x80) return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
    // (Unicode's letters and digits, roughly: Latin-1's letters, and past it all but punctuation and spaces)
    if (c < 0x100) return (c >= 0xc0 && c != 0xd7 && c != 0xf7) || c == 0xaa || c == 0xb5 || c == 0xba ||
                          c == 0xb2 || c == 0xb3 || c == 0xb9 || (c >= 0xbc && c <= 0xbe);
    if (py::is_space_cp(c)) return false;
    if (c >= 0x2000 && c <= 0x2bff) return false;      // punctuation, symbols, arrows, boxes
    if (c >= 0x3000 && c <= 0x303f) return false;
    if (c >= 0xfe30 && c <= 0xfe4f) return false;
    if (c >= 0xff00 && c <= 0xff0f) return false;
    return true;
}
static bool is_digit_cp(uint32_t c) { return c >= '0' && c <= '9'; }

static uint32_t fold(uint32_t c) { return c >= 'A' && c <= 'Z' ? c + 32 : c; }

static bool cat_has(char cat, uint32_t c) {
    switch (cat) {
        case 'w': return is_word_cp(c);
        case 'W': return !is_word_cp(c);
        case 'd': return is_digit_cp(c);
        case 'D': return !is_digit_cp(c);
        case 's': return py::is_space_cp(c);
        case 'S': return !py::is_space_cp(c);
    }
    return false;
}

static bool class_has(const Node& n, uint32_t c, bool icase) {
    bool in = false;
    for (auto& it : n.items) {
        if (it.cat ? cat_has(it.cat, c) : (c >= it.lo && c <= it.hi) ||
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
            case '0': return 0;
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
                    case 'Z': return make(NK::End);
                    case 'w': case 'W': case 'd': case 'D': case 's': case 'S': {
                        auto n = make(NK::Class);
                        escape_item(n->items, e);
                        return n;
                    }
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

Regex::Regex(const std::string& pattern, int flags) : pattern_(pattern) {
    Parser ps(pattern, flags);
    root_ = ps.alt();
    if (ps.more()) ps.fail("unbalanced parenthesis");
    ngroups_ = ps.ngroups;
    flags_ = ps.flags;
    names_ = ps.names;
}

// ---------------------------------------------------------------------------------------- the matcher
template <typename F>
struct Ref;
template <typename R, typename... A>
struct Ref<R(A...)> {
    void* obj;
    R (*call)(void*, A...);
    template <typename T, typename = typename std::enable_if<!std::is_same<typename std::decay<T>::type, Ref>::value>::type>
    Ref(T& t) : obj((void*)&t), call([](void* o, A... a) -> R { return (*(T*)o)(a...); }) {}
    Ref(const Ref&) = default;
    R operator()(A... a) const { return call(obj, a...); }
};
using Cont = Ref<bool(size_t)>;

struct Ctx {
    const std::string& s;
    size_t end;
    int flags;
    std::vector<long> caps;

    uint32_t cp_at(size_t i, size_t& next) const {
        next = i;
        uint32_t c = py::decode_cp(s, next);
        if (next > end) next = end;
        return c;
    }
    bool word_before(size_t i) const {
        if (i == 0) return false;
        size_t p = py::prev_cp(s, i), q;
        return is_word_cp(cp_at(p, q));
    }
    bool word_at(size_t i) const {
        if (i >= end) return false;
        size_t q;
        return is_word_cp(cp_at(i, q));
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
            case NK::Class: return class_has(*n, c, (flags & I) != 0) ? nx : std::string::npos;
            default: return std::string::npos;
        }
    }

    bool seq(const Node* n, size_t idx, size_t i, Cont k) {
        if (idx == n->kids.size()) return k(i);
        const Node* c = n->kids[idx].get();
        // (a run of single characters matched without recursion)
        while (c->k == NK::Char || c->k == NK::Any || c->k == NK::Class) {
            size_t j = one(c, i);
            if (j == std::string::npos) return false;
            i = j;
            idx++;
            if (idx == n->kids.size()) return k(i);
            c = n->kids[idx].get();
        }
        auto rest = [&](size_t j) { return seq(n, idx + 1, j, k); };
        return m(c, i, Cont(rest));
    }

    bool m(const Node* n, size_t i, Cont k) {
        switch (n->k) {
            case NK::Char: case NK::Any: case NK::Class: {
                size_t j = one(n, i);
                return j != std::string::npos && k(j);
            }
            case NK::Bol:
                if (i == 0 || ((flags & M) && s[i - 1] == '\n')) return k(i);
                return false;
            case NK::Eol:
                if (i == end || (i < end && s[i] == '\n' && ((flags & M) || i + 1 == end))) return k(i);
                return false;
            case NK::Start: return i == 0 && k(i);
            case NK::End: return i == end && k(i);
            case NK::WordB: return word_before(i) != word_at(i) && k(i);
            case NK::NWordB: return word_before(i) == word_at(i) && k(i);
            case NK::Seq: return seq(n, 0, i, k);
            case NK::Alt:
                for (auto& b : n->kids)
                    if (m(b.get(), i, k)) return true;
                return false;
            case NK::Group: {
                int g = n->group;
                auto done = [&](size_t j) {
                    long os = caps[2 * g], oe = caps[2 * g + 1];
                    caps[2 * g] = (long)i;
                    caps[2 * g + 1] = (long)j;
                    if (k(j)) return true;
                    caps[2 * g] = os;
                    caps[2 * g + 1] = oe;
                    return false;
                };
                return m(n->kids[0].get(), i, Cont(done));
            }
            case NK::Back: {
                int g = n->group;
                if (caps[2 * g] < 0) return false;
                size_t len = (size_t)(caps[2 * g + 1] - caps[2 * g]);
                if (i + len > end || s.compare(i, len, s, (size_t)caps[2 * g], len) != 0) return false;
                return k(i + len);
            }
            case NK::Look: {
                auto saved = caps;
                auto any = [](size_t) { return true; };
                bool hit;
                if (n->behind) {
                    size_t st = i;
                    bool ok = true;
                    for (int w = 0; w < n->width; w++) {
                        if (st == 0) {
                            ok = false;
                            break;
                        }
                        st = py::prev_cp(s, st);
                    }
                    auto at_i = [&](size_t j) { return j == i; };
                    hit = ok && m(n->kids[0].get(), st, Cont(at_i));
                } else
                    hit = m(n->kids[0].get(), i, Cont(any));
                if (n->neg) {
                    caps = saved;
                    return !hit && k(i);
                }
                if (!hit) {
                    caps = saved;
                    return false;
                }
                if (k(i)) return true;
                caps = saved;
                return false;
            }
            case NK::Rep: return rep(n, i, k);
        }
        return false;
    }

    bool rep(const Node* n, size_t i, Cont k) {
        const Node* c = n->kids[0].get();
        int mn = n->min, mx = n->max;
        if (c->k == NK::Char || c->k == NK::Any || c->k == NK::Class) {
            if (!n->lazy) {
                std::vector<size_t> at{i};
                size_t p = i;
                while (mx < 0 || (int)at.size() - 1 < mx) {
                    size_t j = one(c, p);
                    if (j == std::string::npos) break;
                    at.push_back(j);
                    p = j;
                }
                for (int cnt = (int)at.size() - 1; cnt >= mn; cnt--)
                    if (k(at[(size_t)cnt])) return true;
                return false;
            }
            size_t p = i;
            int cnt = 0;
            for (; cnt < mn; cnt++) {
                size_t j = one(c, p);
                if (j == std::string::npos) return false;
                p = j;
            }
            while (true) {
                if (k(p)) return true;
                if (mx >= 0 && cnt >= mx) return false;
                size_t j = one(c, p);
                if (j == std::string::npos) return false;
                p = j;
                cnt++;
            }
        }
        return rep_general(n, c, 0, i, k);
    }

    bool rep_general(const Node* n, const Node* c, int cnt, size_t i, Cont k) {
        int mn = n->min, mx = n->max;
        auto more = [&](size_t j) {
            if (j == i && cnt >= mn) return false;             // (an empty pass does not repeat)
            return rep_general(n, c, cnt + 1, j, k);
        };
        if (n->lazy) {
            if (cnt >= mn && k(i)) return true;
            if (mx >= 0 && cnt >= mx) return false;
            return m(c, i, Cont(more));
        }
        if (mx < 0 || cnt < mx)
            if (m(c, i, Cont(more))) return true;
        return cnt >= mn && k(i);
    }
};

std::string Match::group(int g) const {
    if (!ok || caps[2 * g] < 0) return "";
    return s->substr((size_t)caps[2 * g], (size_t)(caps[2 * g + 1] - caps[2 * g]));
}

// mode: 0 match, 1 fullmatch, 2 search
Match Regex::run(const std::shared_ptr<const std::string>& sp, size_t pos, size_t endpos, int mode, bool must_advance) const {
    const std::string& s = *sp;
    if (endpos > s.size()) endpos = s.size();
    Match out;
    out.s = sp;
    Ctx c{s, endpos, flags_, std::vector<long>(2 * (ngroups_ + 1), -1)};
    size_t start = pos;
    size_t found_end = 0;
    auto fin = [&](size_t j) {
        if (mode == 1 && j != endpos) return false;
        if (must_advance && j == start) return false;
        found_end = j;
        return true;
    };
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
        if (c.m(r, start, Cont(fin))) {
            c.caps[0] = (long)start;
            c.caps[1] = (long)found_end;
            out.caps = c.caps;
            out.ok = true;
            return out;
        }
        if (mode != 2 || start >= endpos) break;
        py::decode_cp(s, start);
        must_advance = false;
        std::fill(c.caps.begin(), c.caps.end(), -1);
    }
    out.caps = c.caps;
    return out;
}

static std::shared_ptr<const std::string> own(const std::string& s) { return std::make_shared<const std::string>(s); }
Match Regex::match(const std::string& s, size_t pos, size_t endpos) const { return run(own(s), pos, endpos, 0); }
Match Regex::fullmatch(const std::string& s, size_t pos, size_t endpos) const { return run(own(s), pos, endpos, 1); }
Match Regex::search(const std::string& s, size_t pos, size_t endpos) const { return run(own(s), pos, endpos, 2); }

std::vector<Match> Regex::finditer(const std::string& s0) const {
    std::vector<Match> out;
    auto sp = own(s0);
    const std::string& s = *sp;
    size_t pos = 0;
    bool adv = false;
    while (pos <= s.size()) {
        Match m = run(sp, pos, s.size(), 2, adv);
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
    const std::string& s = *sp;
    std::string out;
    size_t pos = 0, last = 0;
    bool adv = false;
    int n = 0;
    while (pos <= s.size() && (count <= 0 || n < count)) {
        Match m = run(sp, pos, s.size(), 2, adv);
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
        char e = repl[++i];
        if (e >= '0' && e <= '9') {
            int g = e - '0';
            if (i + 1 < repl.size() && repl[i + 1] >= '0' && repl[i + 1] <= '9') g = g * 10 + (repl[++i] - '0');
            pieces.push_back({lit});
            lit.clear();
            pieces.push_back({"", g});
        } else if (e == 'g' && i + 1 < repl.size() && repl[i + 1] == '<') {
            size_t close = repl.find('>', i);
            std::string name = repl.substr(i + 2, close - i - 2);
            int g = -1;
            if (!name.empty() && name[0] >= '0' && name[0] <= '9') g = std::stoi(name);
            for (auto& nm : names_)
                if (nm.first == name) g = nm.second;
            pieces.push_back({lit});
            lit.clear();
            pieces.push_back({"", g});
            i = close;
        } else if (e == 'n') lit += '\n';
        else if (e == 't') lit += '\t';
        else if (e == 'r') lit += '\r';
        else if (e == '\\') lit += '\\';
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
