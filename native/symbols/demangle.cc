// demangle.cc -- a decorated linker name as the declaration it was made from (Microsoft's scheme,
// 32-bit and 16-bit with near and far).  What codebreaker/symbols/demangle.py did; the text is the
// one Microsoft's own tools print.  Anything unreadable gives nothing -- a name is never guessed at.
//
// Text is handled as code points (std::u32string), so positions and lengths are the ones Python's
// str had.  A digit is an ASCII digit (and Latin-1's superscript digits, which isdigit() took and
// int() refused, make the name unreadable as they did).
#include "symbols.h"

#include <map>
#include <stdexcept>
#include <vector>

namespace cbs {
namespace {

using S = std::u32string;

struct Unreadable {};

S U(const char* a) { S s; while (*a) s.push_back((unsigned char)*a++); return s; }
bool isdig(char32_t c) {             // str.isdigit(); the superscripts are digits int() refuses
    if (c == 0xb2 || c == 0xb3 || c == 0xb9) throw Unreadable();
    return c >= '0' && c <= '9';
}
bool in(char32_t c, const char* set) { for (; *set; ++set) if ((char32_t)(unsigned char)*set == c) return true; return false; }
bool ends_with(const S& s, const S& t) { return s.size() >= t.size() && s.compare(s.size() - t.size(), t.size(), t) == 0; }
bool starts_with(const S& s, const S& t) { return s.size() >= t.size() && s.compare(0, t.size(), t) == 0; }
bool isspace32(char32_t c) {
    return c == ' ' || (c >= 9 && c <= 13) || (c >= 0x1c && c <= 0x1f) || c == 0x85 || c == 0xa0 || c == 0x1680 ||
           (c >= 0x2000 && c <= 0x200a) || c == 0x2028 || c == 0x2029 || c == 0x202f || c == 0x205f || c == 0x3000;
}
S strip(const S& s) {
    size_t a = 0, b = s.size();
    while (a < b && isspace32(s[a])) ++a;
    while (b > a && isspace32(s[b - 1])) --b;
    return s.substr(a, b - a);
}
S rstrip(const S& s) { size_t b = s.size(); while (b && isspace32(s[b - 1])) --b; return s.substr(0, b); }
S collapse(const S& s) {               // re.sub(r"\s+", " ", s)
    S out; bool sp = false;
    for (char32_t c : s) { if (isspace32(c)) { if (!sp) out.push_back(' '); sp = true; } else { out.push_back(c); sp = false; } }
    return out;
}
S joinw(std::initializer_list<S> parts, const S& sep = U(" ")) {   // " ".join(x for x in parts if x)
    S out; bool first = true;
    for (const S& p : parts) if (!p.empty()) { if (!first) out += sep; out += p; first = false; }
    return out;
}
S join(const std::vector<S>& v, const S& sep) { S out; for (size_t i = 0; i < v.size(); ++i) { if (i) out += sep; out += v[i]; } return out; }
S num(long long v) { std::string a = std::to_string(v); return S(a.begin(), a.end()); }

S _join(const S& left, const S& middle, const S& right) {
    S text = joinw({strip(left), strip(middle)}) + right;
    return strip(collapse(text));
}

const std::map<char32_t, const char*>& BASIC() {
    static const std::map<char32_t, const char*> m = {{'C', "signed char"}, {'D', "char"}, {'E', "unsigned char"}, {'F', "short"},
        {'G', "unsigned short"}, {'H', "int"}, {'I', "unsigned int"}, {'J', "long"}, {'K', "unsigned long"}, {'M', "float"},
        {'N', "double"}, {'O', "long double"}, {'X', "void"}, {'Z', "..."}};
    return m;
}
const std::map<char32_t, const char*>& EXTENDED() {
    static const std::map<char32_t, const char*> m = {{'D', "__int8"}, {'E', "unsigned __int8"}, {'F', "__int16"},
        {'G', "unsigned __int16"}, {'H', "__int32"}, {'I', "unsigned __int32"}, {'J', "__int64"}, {'K', "unsigned __int64"},
        {'N', "bool"}, {'W', "wchar_t"}};
    return m;
}
const char* convention_of(char32_t c) {
    switch (c) {
    case 'A': case 'B': return "__cdecl";
    case 'C': case 'D': return "__pascal";
    case 'E': case 'F': return "__thiscall";
    case 'G': case 'H': return "__stdcall";
    case 'I': case 'J': return "__fastcall";
    }
    return nullptr;
}
const std::map<S, const char*>& OPERATOR() {
    static const std::map<S, const char*> m = [] {
        const char* t[][2] = {
            {"2", "operator new"}, {"3", "operator delete"}, {"4", "operator="}, {"5", "operator>>"}, {"6", "operator<<"},
            {"7", "operator!"}, {"8", "operator=="}, {"9", "operator!="}, {"A", "operator[]"}, {"C", "operator->"},
            {"D", "operator*"}, {"E", "operator++"}, {"F", "operator--"}, {"G", "operator-"}, {"H", "operator+"},
            {"I", "operator&"}, {"J", "operator->*"}, {"K", "operator/"}, {"L", "operator%"}, {"M", "operator<"},
            {"N", "operator<="}, {"O", "operator>"}, {"P", "operator>="}, {"Q", "operator,"}, {"R", "operator()"},
            {"S", "operator~"}, {"T", "operator^"}, {"U", "operator|"}, {"V", "operator&&"}, {"W", "operator||"},
            {"X", "operator*="}, {"Y", "operator+="}, {"Z", "operator-="}, {"_0", "operator/="}, {"_1", "operator%="},
            {"_2", "operator>>="}, {"_3", "operator<<="}, {"_4", "operator&="}, {"_5", "operator|="}, {"_6", "operator^="},
            {"_7", "`vftable'"}, {"_8", "`vbtable'"}, {"_9", "`vcall'"}, {"_A", "`typeof'"}, {"_B", "`local static guard'"},
            {"_C", "`string'"}, {"_D", "`vbase destructor'"}, {"_E", "`vector deleting destructor'"},
            {"_F", "`default constructor closure'"}, {"_G", "`scalar deleting destructor'"},
            {"_H", "`vector constructor iterator'"}, {"_I", "`vector destructor iterator'"},
            {"_J", "`vector vbase constructor iterator'"}, {"_K", "`virtual displacement map'"},
            {"_L", "`eh vector constructor iterator'"}, {"_M", "`eh vector destructor iterator'"},
            {"_N", "`eh vector vbase constructor iterator'"}, {"_O", "`copy constructor closure'"},
            {"_S", "`local vftable'"}, {"_T", "`local vftable constructor closure'"}, {"_U", "operator new[]"},
            {"_V", "operator delete[]"}};
        std::map<S, const char*> r;
        for (auto& p : t) r[U(p[0])] = p[1];
        return r;
    }();
    return m;
}
const char* QUALIFIER[4] = {"", "const", "volatile", "const volatile"};
const char* ACCESS[3] = {"private", "protected", "public"};

int index_of(const char* set, char32_t c) {   // "ABCD".index(c), raising as Python's ValueError would
    for (int i = 0; set[i]; ++i) if ((char32_t)(unsigned char)set[i] == c) return i;
    throw Unreadable();
}

struct Depth {                            // Python's recursion limit gave None; so does a runaway here
    int& d;
    explicit Depth(int& d_) : d(d_) { if (++d > 400) { --d; throw Unreadable(); } }
    ~Depth() { --d; }
};

struct PQ { S words, far, kind; };
struct FT { S convention; std::pair<S, S> result; std::vector<S> params; S this_; };

struct Reader {
    S s; size_t i = 0;
    std::vector<S> names, types;
    int depth = 0;

    explicit Reader(const S& t) : s(t) {}

    S peek(size_t n = 1) const { return i >= s.size() ? S() : s.substr(i, n); }
    char32_t peekc() const { return i < s.size() ? s[i] : 0; }
    bool at(char32_t c) const { return i < s.size() && s[i] == c; }
    char32_t take() { if (i >= s.size()) throw Unreadable(); return s[i++]; }

    long long number() {
        bool negative = at('?');
        if (negative) ++i;
        char32_t c = take();
        long long value;
        if (isdig(c)) value = (long long)(c - '0') + 1;
        else {
            value = 0; --i;
            while (!at('@')) {
                c = take();
                if (!(c >= 'A' && c <= 'P')) throw Unreadable();
                value = value * 16 + (long long)(c - 65);
            }
            ++i;
        }
        return negative ? -value : value;
    }

    S identifier() {
        size_t end = s.find(U'@', i);
        if (end == S::npos || end == i) throw Unreadable();
        S text = s.substr(i, end - i);
        i = end + 1;
        return text;
    }

    S templ() {
        Depth g(depth);
        std::vector<S> on = std::move(names), ot = std::move(types);
        names.clear(); types.clear();
        S base;
        if (at('?')) { ++i; base = op(); }
        else { base = identifier(); names.push_back(base); }
        std::vector<S> args;
        while (!at('@')) {
            if (at('$')) {
                ++i;
                char32_t kind = take();
                if (kind != '0') throw Unreadable();
                args.push_back(num(number()));
            } else args.push_back(argument());
        }
        ++i;
        names = std::move(on); types = std::move(ot);
        S text = base + U("<") + join(args, U(",")) + U(">");
        return ends_with(text, U(">>")) ? text + U(" ") : text;
    }

    S op() {
        S code(1, take());
        if (code == U("_")) code.push_back(take());
        if (code == U("0")) return U("{constructor}");
        if (code == U("1")) return U("{destructor}");
        if (code == U("B")) return U("{conversion}");
        auto it = OPERATOR().find(code);
        if (it == OPERATOR().end()) throw Unreadable();
        return U(it->second);
    }

    S piece() {
        Depth g(depth);
        char32_t c = peekc();
        S text;
        if (i < s.size() && isdig(c)) {
            ++i;
            if ((size_t)(c - '0') >= names.size()) throw Unreadable();
            return names[c - '0'];
        }
        if (peek(2) == U("?$")) { i += 2; text = templ(); }
        else if (i < s.size() && c == '?') { ++i; return U("`") + num(number()) + U("'"); }
        else text = identifier();
        bool have = false;
        for (auto& n : names) if (n == text) have = true;
        if (names.size() < 10 && !have) names.push_back(text);
        return text;
    }

    std::vector<S> qualified() {
        Depth g(depth);
        S base;
        if (at('?') && peek(2) != U("?$")) { ++i; base = op(); }
        else base = piece();
        std::vector<S> scopes;
        while (!at('@')) {
            if (i >= s.size()) throw Unreadable();
            scopes.push_back(piece());
        }
        ++i;
        if (!scopes.empty()) {
            S last = rstrip(scopes[0]);
            size_t lt = last.find(U'<');
            if (lt != S::npos && !starts_with(last, U("operator"))) last = last.substr(0, lt);
            if (base == U("{constructor}")) base = last;
            else if (base == U("{destructor}")) base = U("~") + last;
        }
        std::vector<S> out(scopes.rbegin(), scopes.rend());
        out.push_back(base);
        return out;
    }

    PQ pointee_qualifier() {
        char32_t c = take();
        if (in(c, "ABCD")) return {U(QUALIFIER[index_of("ABCD", c)]), S(), U("data")};
        if (in(c, "EFGH")) return {U(QUALIFIER[index_of("EFGH", c)]), U("__far"), U("data")};
        if (in(c, "IJKL")) return {U(QUALIFIER[index_of("IJKL", c)]), U("__huge"), U("data")};
        if (in(c, "67")) return {S(), c == '7' ? U("__far") : S(), U("function")};
        if (in(c, "89")) return {S(), c == '9' ? U("__far") : S(), U("member function")};
        if (in(c, "QRST")) return {U(QUALIFIER[index_of("QRST", c)]), S(), U("member")};
        throw Unreadable();
    }

    FT function_type(bool with_this) {
        Depth g(depth);
        FT f;
        S second = peek(2).size() > 1 ? peek(2).substr(1) : S();
        bool skip = at('E') && !(second == U("A") || second == U("C") || second == U("G") || second == U("I"));
        if (with_this && !skip) f.this_ = pointee_qualifier().words;
        const char* conv = convention_of(take());
        if (!conv) throw Unreadable();
        f.convention = U(conv);
        if (at('@')) { ++i; f.result = {S(), S()}; }
        else f.result = result();
        f.params = parameters();
        return f;
    }

    std::pair<S, S> result() {
        Depth g(depth);
        if (at('?')) {
            ++i;
            S q = U(QUALIFIER[index_of("ABCDEFGH", take()) % 4]);
            auto lr = type();
            return {lr.first + (q.empty() ? S() : U(" ") + q), lr.second};
        }
        return type();
    }

    std::vector<S> parameters() {
        std::vector<S> out;
        if (at('X')) { ++i; out.push_back(U("void")); }
        else {
            while (!(at('@') || at('Z'))) {
                if (i >= s.size()) throw Unreadable();
                out.push_back(argument());
            }
            if (take() == 'Z') out.push_back(U("..."));
        }
        if (at('Z')) ++i;
        return out;
    }

    S argument() {
        Depth g(depth);
        char32_t c = peekc();
        if (i < s.size() && isdig(c)) {
            ++i;
            if ((size_t)(c - '0') >= types.size()) throw Unreadable();
            return types[c - '0'];
        }
        size_t start = i;
        auto lr = (i < s.size() && c == '?') ? result() : type();
        S text = _join(lr.first, S(), lr.second);
        if (i - start > 1 && types.size() < 10) types.push_back(text);
        return text;
    }

    std::pair<S, S> type() {
        Depth g(depth);
        char32_t c = take();
        if (c == '_') {
            c = take();
            auto it = EXTENDED().find(c);
            if (it == EXTENDED().end()) throw Unreadable();
            return {U(it->second), S()};
        }
        auto b = BASIC().find(c);
        if (b != BASIC().end() && c != 'Z') return {U(b->second), S()};
        if (in(c, "TUV")) {
            const char* kw = c == 'T' ? "union " : c == 'U' ? "struct " : "class ";
            return {U(kw) + join(qualified(), U("::")), S()};
        }
        if (c == 'W') {
            take();
            return {U("enum ") + join(qualified(), U("::")), S()};
        }
        if (in(c, "ABPQRS")) {
            S mark = in(c, "AB") ? U("&") : U("*");
            S own = c == 'B' ? U("volatile") : c == 'Q' ? U("const") : c == 'R' ? U("volatile") : c == 'S' ? U("const volatile") : S();
            S sown = own.empty() ? S() : U(" ") + own;
            PQ pq = pointee_qualifier();
            if (pq.kind == U("member")) {
                S cls = join(qualified(), U("::"));
                auto lr = type();
                return {_join(lr.first, pq.words, S()) + U(" ") + cls + U("::") + mark + sown, lr.second};
            }
            if (ends_with(pq.kind, U("function"))) {
                S cls = starts_with(pq.kind, U("member")) ? join(qualified(), U("::")) + U("::") : S();
                FT f = function_type(!cls.empty());
                S fc = joinw({pq.far, f.convention});
                S inner = U("(") + fc + (cls.empty() ? S() : U(" ")) + cls + mark;
                if (cls.empty()) inner = U("(") + fc + mark;
                return {_join(f.result.first, S(), S()) + U(" ") + inner + sown,
                        U(")(") + join(f.params, U(",")) + U(")") + f.this_ + f.result.second};
            }
            if (at('Y')) {
                ++i;
                long long count = number();
                std::vector<long long> dims;
                for (long long k = 0; k < count; ++k) dims.push_back(number());
                auto lr = type();
                S right = U(")");
                for (long long d : dims) right += U("[") + num(d) + U("]");
                return {_join(lr.first, pq.words, S()) + U(" (") + (pq.far.empty() ? S() : pq.far + U(" ")) + mark + sown,
                        right + lr.second};
            }
            auto lr = type();
            if (!lr.second.empty()) return {lr.first + mark, lr.second};
            S words = pq.words;
            if (!words.empty() && ends_with(lr.first, words)) words.clear();
            return {joinw({lr.first, words, pq.far, mark, own}), S()};
        }
        throw Unreadable();
    }
};

S replace_first(const S& text, const S& from, const S& to) {
    size_t at = text.find(from);
    if (at == S::npos) return text;
    return text.substr(0, at) + to + text.substr(at + from.size());
}

// ---- the answer, as JSON (non-ASCII as \u escapes, so Python's str comes back exactly) -----------
void jstr(std::string& o, const S& s) {
    static const char* hex = "0123456789abcdef";
    o.push_back('"');
    auto u = [&](unsigned v) { o += "\\u"; o.push_back(hex[(v >> 12) & 15]); o.push_back(hex[(v >> 8) & 15]); o.push_back(hex[(v >> 4) & 15]); o.push_back(hex[v & 15]); };
    for (char32_t c : s) {
        if (c == '"') o += "\\\"";
        else if (c == '\\') o += "\\\\";
        else if (c < 0x20 || (c >= 0x7f && c < 0x10000)) u((unsigned)c);
        else if (c < 0x7f) o.push_back((char)c);
        else { unsigned v = (unsigned)c - 0x10000; u(0xd800 + (v >> 10)); u(0xdc00 + (v & 0x3ff)); }
    }
    o.push_back('"');
}

struct Out {
    std::vector<std::pair<std::string, std::string>> kv;   // key -> JSON value, in insertion order
    void set(const std::string& k, const std::string& v) {
        for (auto& p : kv) if (p.first == k) { p.second = v; return; }
        kv.push_back({k, v});
    }
    void str(const std::string& k, const S& v) { std::string j; jstr(j, v); set(k, j); }
    void boolean(const std::string& k, bool v) { set(k, v ? "true" : "false"); }
    void list(const std::string& k, const std::vector<S>& v) {
        std::string j = "[";
        for (size_t n = 0; n < v.size(); ++n) { if (n) j += ","; jstr(j, v[n]); }
        set(k, j + "]");
    }
    std::string json() const {
        std::string o = "{";
        for (size_t n = 0; n < kv.size(); ++n) { if (n) o += ","; o += "\"" + kv[n].first + "\":" + kv[n].second; }
        return o + "}";
    }
};

std::string read(const S& name) {
    if (name.empty() || name[0] != '?') throw Unreadable();
    Reader r(name.substr(1));
    Out out;
    if (r.peek(4) == U("?_C@")) {
        out.str("name", U("`string'")); out.str("kind", U("data")); out.str("text", U("`string'"));
        return out.json();
    }
    std::vector<S> names = r.qualified();
    S qname = join(names, U("::"));
    out.str("name", qname); out.str("base", names.back());
    out.str("class", join(std::vector<S>(names.begin(), names.end() - 1), U("::")));
    out.str("kind", U("function")); out.str("access", S()); out.boolean("static", false); out.boolean("virtual", false);
    out.boolean("far", false); out.str("convention", S()); out.str("returns", S()); out.list("parameters", {});
    out.str("const", S());
    if (r.i >= r.s.size()) { out.str("kind", U("name")); out.str("text", qname); return out.json(); }
    char32_t code = r.take();
    if (in(code, "01234")) {
        auto lr = r.type();
        S left = lr.first, right = lr.second;
        if (right.empty() && ends_with(left, U("* const"))) left = left.substr(0, left.size() - 6);
        PQ pq = r.i < r.s.size() ? r.pointee_qualifier() : PQ{};
        S access = in(code, "012") ? U(ACCESS[code - '0']) : S();
        left = joinw({left, pq.words, pq.far});
        S text = (access.empty() ? S() : access + U(": static ")) + _join(left, qname, right);
        out.str("kind", U("data")); out.str("access", access); out.boolean("static", !access.empty());
        out.str("returns", _join(left, S(), right)); out.str("text", text);
        return out.json();
    }
    if (in(code, "67")) {
        S words;
        if (in(r.peekc(), "ABCDEFGH") && r.i < r.s.size()) words = U(QUALIFIER[index_of("ABCDEFGH", r.take()) % 4]);
        std::vector<S> owners;
        while (r.i < r.s.size() && !r.at('@')) owners.push_back(join(r.qualified(), U("::")));
        S text = (words.empty() ? S() : words + U(" ")) + qname;
        if (!owners.empty()) text += U("{for `") + join(owners, U("'s `")) + U("'}");
        out.str("kind", U("data")); out.str("text", text);
        return out.json();
    }
    if (in(code, "89")) { out.str("kind", U("name")); out.str("text", qname); return out.json(); }
    if (!(code >= 'A' && code <= 'Z')) throw Unreadable();
    int n = (int)(code - 65);
    bool member = n < 24, far = (n & 1) != 0;
    int shape = member ? (n % 8) / 2 : 0;
    long long adjust = 0;
    if (shape == 3) adjust = r.number();
    FT f = r.function_type(member && shape != 1);
    std::pair<S, S> result = f.result;
    if (names.back() == U("{conversion}")) {
        names.back() = U("operator ") + _join(result.first, S(), result.second);
        result = {S(), S()};
        qname = join(names, U("::"));
    }
    S access = member ? U(ACCESS[n / 8]) : S();
    std::vector<S> params = f.params;
    out.str("name", qname); out.str("base", names.back()); out.str("access", access); out.boolean("static", shape == 1);
    out.boolean("virtual", shape == 2 || shape == 3); out.boolean("far", far); out.str("convention", f.convention);
    out.str("returns", _join(result.first, S(), result.second));
    out.list("parameters", params.size() == 1 && params[0] == U("void") ? std::vector<S>() : params);
    out.str("const", f.this_);
    S lead;
    if (member) lead = access + U(": ") + (shape == 1 ? U("static ") : (shape == 2 || shape == 3) ? U("virtual ") : S());
    S how = (far ? U("__far ") : S()) + f.convention;
    S text;
    if (result.first.find(U'(') != S::npos) {
        S middle = result.first + U(" ") + how + U(" ") + qname + U("(") + join(params, U(",")) + U(")");
        text = lead + collapse(middle) + result.second;
    } else text = lead + joinw({result.first, how, qname}) + U("(") + join(params, U(",")) + U(")");
    if (shape == 3)
        text = U("[thunk]:") + replace_first(text, qname + U("("), qname + U("`adjustor{") + num(adjust) + U("}' ("));
    out.str("text", text + f.this_);
    return out.json();
}

}  // namespace

bool demangle_parse(const std::u32string& name, std::string& json) {
    try { json = read(name); return true; }
    catch (const Unreadable&) { return false; }
    catch (const std::out_of_range&) { return false; }
}

bool demangle_type_name(const std::u32string& d, std::string& json) {
    if (d.size() < 5 || d.compare(0, 3, U(".?A")) != 0 || !in(d[3], "VUTW")) return false;
    try {
        Reader r(d.substr(4));
        S t = strip(join(r.qualified(), U("::")));
        json.clear(); jstr(json, t);
        return true;
    } catch (const Unreadable&) { return false; }
    catch (const std::out_of_range&) { return false; }
}

}  // namespace cbs
