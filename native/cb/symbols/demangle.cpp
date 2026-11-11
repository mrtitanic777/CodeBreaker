// cb/symbols/demangle.cpp -- Microsoft's decorated names (see demangle.hpp).
//
// The reader is a recursive descent over the name. A name it cannot read unwinds with a FormatError that the public
// entry points turn into "no answer"; the unwinding stays inside this file.
#include "cb/symbols/demangle.hpp"

#include <array>
#include <cstring>
#include <map>
#include <string_view>
#include <utility>

#include "cb/core/error.hpp"
#include "cb/core/log.hpp"
#include "cb/core/text.hpp"

namespace cb::symbols::demangle {
namespace {

using std::string;
using std::string_view;

// A type in two parts, the text that goes before the declarator and the text after it ("int (" and ")(int)").
struct Split {
    string left;
    string right;
};

bool is_digit(char c) { return c >= '0' && c <= '9'; }
bool in(char c, string_view set) { return c != '\0' && set.find(c) != string_view::npos; }

// Spaces collapsed to one, ends trimmed.
string collapse(string_view s) {
    string out;
    bool space = false;
    for (char c : s) {
        if (text::is_space(c)) {
            if (!space) out.push_back(' ');
            space = true;
        } else {
            out.push_back(c);
            space = false;
        }
    }
    return string(text::trim(out));
}

// The parts that are not empty, joined by spaces.
string join_words(std::initializer_list<string_view> parts) {
    string out;
    for (string_view p : parts) {
        if (p.empty()) continue;
        if (!out.empty()) out.push_back(' ');
        out.append(p);
    }
    return out;
}

// "left middle right" with the spaces tidied: where a declarator name goes inside a type.
string join_type(string_view left, string_view middle, string_view right) {
    return collapse(join_words({text::trim(left), text::trim(middle)}) + string(right));
}

string number_text(std::int64_t v) { return std::to_string(v); }

string_view basic_type(char c) {
    switch (c) {
        case 'C': return "signed char";
        case 'D': return "char";
        case 'E': return "unsigned char";
        case 'F': return "short";
        case 'G': return "unsigned short";
        case 'H': return "int";
        case 'I': return "unsigned int";
        case 'J': return "long";
        case 'K': return "unsigned long";
        case 'M': return "float";
        case 'N': return "double";
        case 'O': return "long double";
        case 'X': return "void";
        default: return {};
    }
}

string_view extended_type(char c) {
    switch (c) {
        case 'D': return "__int8";
        case 'E': return "unsigned __int8";
        case 'F': return "__int16";
        case 'G': return "unsigned __int16";
        case 'H': return "__int32";
        case 'I': return "unsigned __int32";
        case 'J': return "__int64";
        case 'K': return "unsigned __int64";
        case 'N': return "bool";
        case 'W': return "wchar_t";
        default: return {};
    }
}

string_view convention_of(char c) {
    switch (c) {
        case 'A': case 'B': return "__cdecl";
        case 'C': case 'D': return "__pascal";
        case 'E': case 'F': return "__thiscall";
        case 'G': case 'H': return "__stdcall";
        case 'I': case 'J': return "__fastcall";
        default: return {};
    }
}

// The operator or special name a `?X` or `?_X` code stands for.
string_view operator_name(string_view code) {
    static const std::map<string_view, string_view> table = {
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
    auto it = table.find(code);
    return it == table.end() ? string_view() : it->second;
}

constexpr std::array<string_view, 4> kQualifier = {"", "const", "volatile", "const volatile"};
constexpr std::array<string_view, 3> kAccess = {"private", "protected", "public"};

// What a pointer, reference or member says of the thing it points at.
struct PointeeKind {
    string words;   // const / volatile of the pointee
    string far;     // __far / __huge
    string kind;    // "data", "function", "member function", "member"
};

// The function type behind a name: its convention, result, parameters and what the method is qualified with.
struct FunctionType {
    string convention;
    Split result;
    std::vector<string> params;
    string this_qualifier;
};

class Reader {
public:
    explicit Reader(string_view text) : s_(text) {}

    // ---- the name's own structure ----
    size_t position() const { return i_; }
    bool at_end() const { return i_ >= s_.size(); }
    string_view peek(size_t n) const { return i_ >= s_.size() ? string_view() : s_.substr(i_, n); }

    [[noreturn]] void fail() const { throw FormatError("demangle", i_, "not a name this reader understands"); }

    char take() {
        if (i_ >= s_.size()) fail();
        return s_[i_++];
    }
    bool at(char c) const { return i_ < s_.size() && s_[i_] == c; }
    char peek_char() const { return i_ < s_.size() ? s_[i_] : '\0'; }
    void skip(size_t n) { i_ += n; }

    // A number: a digit stands for itself plus one, otherwise hex digits A-P ending at '@'; '?' makes it negative.
    std::int64_t number() {
        const bool negative = at('?');
        if (negative) ++i_;
        char c = take();
        std::int64_t value = 0;
        if (is_digit(c)) {
            value = c - '0' + 1;
        } else {
            --i_;
            while (!at('@')) {
                c = take();
                if (c < 'A' || c > 'P') fail();
                if (value > (std::int64_t{1} << 55)) fail();
                value = value * 16 + (c - 'A');
            }
            ++i_;
        }
        return negative ? -value : value;
    }

    // A plain name, up to its '@'.
    string identifier() {
        const size_t end = s_.find('@', i_);
        if (end == string_view::npos || end == i_) fail();
        string text(s_.substr(i_, end - i_));
        i_ = end + 1;
        return text;
    }

    // A template instance: `?$name@args@`.
    string template_instance() {
        Depth guard(*this);
        std::vector<string> saved_names = std::move(names_);
        std::vector<string> saved_types = std::move(types_);
        names_.clear();
        types_.clear();
        string base;
        if (at('?')) {
            ++i_;
            base = operator_code();
        } else {
            base = identifier();
            names_.push_back(base);
        }
        std::vector<string> args;
        while (!at('@')) {
            if (at('$')) {
                ++i_;
                if (take() != '0') fail();   // only `$0` constant arguments are read
                args.push_back(number_text(number()));
            } else {
                args.push_back(argument());
            }
        }
        ++i_;
        names_ = std::move(saved_names);
        types_ = std::move(saved_types);
        string text = base + "<" + text::join(args, ",") + ">";
        if (text.ends_with(">>")) text.push_back(' ');
        return text;
    }

    // The code after a '?' that names an operator or a special member.
    string operator_code() {
        string code(1, take());
        if (code == "_") code.push_back(take());
        if (code == "0") return "{constructor}";
        if (code == "1") return "{destructor}";
        if (code == "B") return "{conversion}";
        const string_view name = operator_name(code);
        if (name.empty()) fail();
        return string(name);
    }

    // One piece of a qualified name: a back-reference digit, a template, or an identifier; the first ten distinct
    // pieces can be referred back to.
    string piece() {
        Depth guard(*this);
        const char c = peek_char();
        if (!at_end() && is_digit(c)) {
            ++i_;
            if (static_cast<size_t>(c - '0') >= names_.size()) fail();
            return names_[static_cast<size_t>(c - '0')];
        }
        string text;
        if (peek(2) == "?$") {
            i_ += 2;
            text = template_instance();
        } else if (!at_end() && c == '?') {
            ++i_;
            return "`" + number_text(number()) + "'";
        } else {
            text = identifier();
        }
        bool known = false;
        for (const string& n : names_) known = known || n == text;
        if (names_.size() < 10 && !known) names_.push_back(text);
        return text;
    }

    // A qualified name, outermost scope first: ["Outer", "Inner", "name"].
    std::vector<string> qualified() {
        Depth guard(*this);
        string base;
        if (at('?') && peek(2) != "?$") {
            ++i_;
            base = operator_code();
        } else {
            base = piece();
        }
        std::vector<string> scopes;
        while (!at('@')) {
            if (at_end()) fail();
            scopes.push_back(piece());
        }
        ++i_;
        if (!scopes.empty()) {
            string last(text::rtrim(scopes[0]));
            const size_t lt = last.find('<');
            if (lt != string::npos && !last.starts_with("operator")) last = last.substr(0, lt);
            if (base == "{constructor}") base = last;
            else if (base == "{destructor}") base = "~" + last;
        }
        std::vector<string> out(scopes.rbegin(), scopes.rend());
        out.push_back(base);
        return out;
    }

    PointeeKind pointee_kind() {
        const char c = take();
        if (in(c, "ABCD")) return {string(kQualifier[static_cast<size_t>(c - 'A')]), "", "data"};
        if (in(c, "EFGH")) return {string(kQualifier[static_cast<size_t>(c - 'E')]), "__far", "data"};
        if (in(c, "IJKL")) return {string(kQualifier[static_cast<size_t>(c - 'I')]), "__huge", "data"};
        if (in(c, "67")) return {"", c == '7' ? "__far" : "", "function"};
        if (in(c, "89")) return {"", c == '9' ? "__far" : "", "member function"};
        if (in(c, "QRST")) return {string(kQualifier[static_cast<size_t>(c - 'Q')]), "", "member"};
        fail();
    }

    FunctionType function_type(bool with_this) {
        Depth guard(*this);
        FunctionType f;
        const string_view two = peek(2);
        const string_view second = two.size() > 1 ? two.substr(1) : string_view();
        const bool skip = at('E') && !(second == "A" || second == "C" || second == "G" || second == "I");
        if (with_this && !skip) f.this_qualifier = pointee_kind().words;
        const string_view convention = convention_of(take());
        if (convention.empty()) fail();
        f.convention = string(convention);
        if (at('@')) {
            ++i_;   // a constructor or destructor has no result
        } else {
            f.result = result();
        }
        f.params = parameters();
        return f;
    }

    Split result() {
        Depth guard(*this);
        if (at('?')) {
            ++i_;
            const char q = take();
            if (!in(q, "ABCDEFGH")) fail();
            Split t = type();
            const string_view qualifier = kQualifier[static_cast<size_t>(q - 'A') % 4];
            return {t.left + (qualifier.empty() ? string() : " " + string(qualifier)), t.right};
        }
        return type();
    }

    std::vector<string> parameters() {
        std::vector<string> out;
        if (at('X')) {
            ++i_;
            out.push_back("void");
        } else {
            while (!(at('@') || at('Z'))) {
                if (at_end()) fail();
                out.push_back(argument());
            }
            if (take() == 'Z') out.push_back("...");
        }
        if (at('Z')) ++i_;
        return out;
    }

    // One parameter: a back-reference to an earlier one, or a type (the first ten longer than a letter can be
    // referred back to).
    string argument() {
        Depth guard(*this);
        const char c = peek_char();
        if (!at_end() && is_digit(c)) {
            ++i_;
            if (static_cast<size_t>(c - '0') >= types_.size()) fail();
            return types_[static_cast<size_t>(c - '0')];
        }
        const size_t start = i_;
        Split t = (!at_end() && c == '?') ? result() : type();
        string text = join_type(t.left, "", t.right);
        if (i_ - start > 1 && types_.size() < 10) types_.push_back(text);
        return text;
    }

    Split type() {
        Depth guard(*this);
        char c = take();
        if (c == '_') {
            const string_view t = extended_type(take());
            if (t.empty()) fail();
            return {string(t), ""};
        }
        if (c != 'Z') {
            const string_view t = basic_type(c);
            if (!t.empty()) return {string(t), ""};
        }
        if (in(c, "TUV")) {
            const char* keyword = c == 'T' ? "union " : c == 'U' ? "struct " : "class ";
            return {keyword + text::join(qualified(), "::"), ""};
        }
        if (c == 'W') {
            take();   // the enum's underlying size
            return {"enum " + text::join(qualified(), "::"), ""};
        }
        if (!in(c, "ABPQRS")) fail();
        return pointer_type(c);
    }

private:
    // RAII guard on the recursion depth: a runaway name is "not readable".
    class Depth {
    public:
        explicit Depth(Reader& r) : r_(r) {
            if (++r_.depth_ > 400) {
                --r_.depth_;
                r_.fail();
            }
        }
        ~Depth() { --r_.depth_; }
        Depth(const Depth&) = delete;
        Depth& operator=(const Depth&) = delete;

    private:
        Reader& r_;
    };

    // A reference (A, B) or pointer (P, Q, R, S) and what it points at.
    Split pointer_type(char c) {
        const string mark = in(c, "AB") ? "&" : "*";
        const string own = c == 'B' ? "volatile" : c == 'Q' ? "const" : c == 'R' ? "volatile" : c == 'S' ? "const volatile" : "";
        const string own_after = own.empty() ? string() : " " + own;
        const PointeeKind pk = pointee_kind();
        if (pk.kind == "member") {   // pointer to data member
            const string cls = text::join(qualified(), "::");
            Split t = type();
            return {join_type(t.left, pk.words, "") + " " + cls + "::" + mark + own_after, t.right};
        }
        if (pk.kind.ends_with("function")) {
            const string cls = pk.kind.starts_with("member") ? text::join(qualified(), "::") + "::" : string();
            FunctionType f = function_type(!cls.empty());
            const string convention = join_words({pk.far, f.convention});
            string inner = cls.empty() ? "(" + convention + mark : "(" + convention + " " + cls + mark;
            return {join_type(f.result.left, "", "") + " " + inner + own_after,
                    ")(" + text::join(f.params, ",") + ")" + f.this_qualifier + f.result.right};
        }
        if (at('Y')) {   // pointer to array
            ++i_;
            const std::int64_t count = number();
            std::vector<std::int64_t> dims;
            for (std::int64_t k = 0; k < count; ++k) dims.push_back(number());
            Split t = type();
            string right = ")";
            for (std::int64_t d : dims) right += "[" + number_text(d) + "]";
            return {join_type(t.left, pk.words, "") + " (" + (pk.far.empty() ? string() : pk.far + " ") + mark + own_after,
                    right + t.right};
        }
        Split t = type();
        if (!t.right.empty()) return {t.left + mark, t.right};
        string words = pk.words;
        if (!words.empty() && t.left.ends_with(words)) words.clear();   // `char const` already said it
        return {join_words({t.left, words, pk.far, mark, own}), ""};
    }

    string_view s_;
    size_t i_ = 0;
    std::vector<string> names_;   // the pieces of qualified names seen, for back-references
    std::vector<string> types_;   // the parameter types seen, for back-references
    int depth_ = 0;
};

string replace_first_text(const string& text, const string& from, const string& to) {
    const size_t at = text.find(from);
    return at == string::npos ? text : text.substr(0, at) + to + text.substr(at + from.size());
}

// The reading of a whole name starting with '?'.
Declaration read(string_view name) {
    if (name.empty() || name[0] != '?') throw FormatError("demangle", 0, "not a decorated C++ name");
    Reader r(name.substr(1));
    Declaration out;
    if (r.peek(4) == "?_C@") {
        out.name = out.text = "`string'";
        out.kind = Kind::Data;
        return out;
    }
    std::vector<string> names = r.qualified();
    string qname = text::join(names, "::");
    out.name = qname;
    out.base = names.back();
    out.owner = text::join(std::span<const string>(names).first(names.size() - 1), "::");
    out.kind = Kind::Function;
    if (r.at_end()) {
        out.kind = Kind::Name;
        out.text = qname;
        return out;
    }
    const char code = r.take();
    if (in(code, "01234")) {   // a variable: 0-2 static members, 3-4 globals
        Split t = r.type();
        if (t.right.empty() && t.left.ends_with("* const")) t.left.resize(t.left.size() - 6);
        PointeeKind pk;
        if (!r.at_end()) pk = r.pointee_kind();
        const string access = in(code, "012") ? string(kAccess[static_cast<size_t>(code - '0')]) : string();
        t.left = join_words({t.left, pk.words, pk.far});
        out.kind = Kind::Data;
        out.access = access;
        out.is_static = !access.empty();
        out.returns = join_type(t.left, "", t.right);
        out.text = (access.empty() ? string() : access + ": static ") + join_type(t.left, qname, t.right);
        return out;
    }
    if (in(code, "67")) {   // `vftable'-style names "for" their owners
        string words;
        if (in(r.peek_char(), "ABCDEFGH") && !r.at_end()) {
            words = string(kQualifier[static_cast<size_t>(r.take() - 'A') % 4]);
        }
        std::vector<string> owners;
        while (!r.at_end() && !r.at('@')) owners.push_back(text::join(r.qualified(), "::"));
        string text = (words.empty() ? string() : words + " ") + qname;
        if (!owners.empty()) text += "{for `" + text::join(owners, "'s `") + "'}";
        out.kind = Kind::Data;
        out.text = text;
        return out;
    }
    if (in(code, "89")) {
        out.kind = Kind::Name;
        out.text = qname;
        return out;
    }
    if (code < 'A' || code > 'Z') r.fail();
    const int n = code - 'A';
    const bool member = n < 24;
    const bool far = (n & 1) != 0;
    const int shape = member ? (n % 8) / 2 : 0;   // 0 plain, 1 static, 2 virtual, 3 virtual thunk
    std::int64_t adjust = 0;
    if (shape == 3) adjust = r.number();
    FunctionType f = r.function_type(member && shape != 1);
    Split result = f.result;
    if (names.back() == "{conversion}") {
        names.back() = "operator " + join_type(result.left, "", result.right);
        result = {};
        qname = text::join(names, "::");
    }
    const string access = member ? string(kAccess[static_cast<size_t>(n / 8)]) : string();
    out.name = qname;
    out.base = names.back();
    out.access = access;
    out.is_static = shape == 1;
    out.is_virtual = shape == 2 || shape == 3;
    out.far = far;
    out.convention = f.convention;
    out.returns = join_type(result.left, "", result.right);
    if (!(f.params.size() == 1 && f.params[0] == "void")) out.parameters = f.params;
    out.const_qualifier = f.this_qualifier;
    string lead;
    if (member) lead = access + ": " + (shape == 1 ? "static " : (shape == 2 || shape == 3) ? "virtual " : "");
    const string how = (far ? "__far " : "") + f.convention;
    const string params = text::join(f.params, ",");
    string text;
    if (result.left.find('(') != string::npos) {   // a function returning a pointer to function
        text = lead + collapse(result.left + " " + how + " " + qname + "(" + params + ")") + result.right;
    } else {
        text = lead + join_words({result.left, how, qname}) + "(" + params + ")";
    }
    if (shape == 3) {
        text = "[thunk]:" + replace_first_text(text, qname + "(", qname + "`adjustor{" + number_text(adjust) + "}' (");
    }
    out.text = text + f.this_qualifier;
    return out;
}

// Does `n` read as an identifier ([A-Za-z_][A-Za-z0-9_]*)?
bool identifier_text(string_view n) { return text::is_identifier(n); }

}  // namespace

std::optional<Declaration> parse(std::string_view name) {
    if (name.empty()) return std::nullopt;
    try {
        return read(name);
    } catch (const FormatError&) {   // not a name this reader understands: no answer, never a guess
        return std::nullopt;
    }
}

std::optional<std::string> type_name(std::string_view decorated) {
    if (decorated.size() < 5 || decorated.substr(0, 3) != ".?A" || !in(decorated[3], "VUTW")) return std::nullopt;
    try {
        Reader r(decorated.substr(4));
        return string(text::trim(text::join(r.qualified(), "::")));
    } catch (const FormatError&) {
        return std::nullopt;
    }
}

std::optional<std::string> declaration(std::string_view name) {
    std::optional<Declaration> p = parse(name);
    if (!p) return std::nullopt;
    return std::move(p->text);
}

std::optional<std::string> short_name(std::string_view name) {
    std::optional<Declaration> p = parse(name);
    if (!p) return std::nullopt;
    return std::move(p->name);
}

PlainName plain(std::string_view name) {
    // `@N` after the last '@', digits to the end: what a __stdcall or __fastcall name says it removes from the stack.
    const size_t at = name.rfind('@');
    std::optional<std::uint32_t> bytes;
    if (at != string_view::npos) {
        const string_view digits = name.substr(at + 1);
        if (!digits.empty() && is_digit(digits[0])) bytes = text::parse_int<std::uint32_t>(digits);   // (parse_int would take a sign)
    }
    if (bytes) {
        const string_view head = name.substr(0, at);
        if (head.starts_with('@')) {   // __fastcall: @name@8
            if (identifier_text(head.substr(1))) return {string(head.substr(1)), "__fastcall", bytes};
        } else {                       // __stdcall: _name@12 or name@12; the underscore is the convention's if a name remains
            if (head.starts_with('_') && identifier_text(head.substr(1))) return {string(head.substr(1)), "__stdcall", bytes};
            if (identifier_text(head)) return {string(head), "__stdcall", bytes};
        }
    }
    if (name.starts_with('_') && identifier_text(name.substr(1))) return {string(name.substr(1)), "__cdecl", std::nullopt};   // _name
    return {string(name), "", std::nullopt};
}

std::string readable(std::string_view name) {
    if (name.starts_with('?')) {
        std::optional<string> d = declaration(name);
        return d ? std::move(*d) : string(name);
    }
    return plain(name).name;
}

std::optional<std::uint32_t> stack_bytes(std::string_view name) {
    const std::optional<Declaration> p = parse(name);
    if (!p || p->kind != Kind::Function) return plain(name).argument_bytes;
    if ((p->convention != "__stdcall" && p->convention != "__thiscall") || p->far) {
        return std::nullopt;   // 16-bit: a parameter's size depends on the memory model
    }
    const auto by_value = [](const string& t) {
        return (t.starts_with("class ") || t.starts_with("struct ") || t.starts_with("union ")) && !t.ends_with('*') &&
               !t.ends_with('&');
    };
    const bool returns_by_value = by_value(p->returns);
    if (returns_by_value && !p->returns.starts_with("class ")) return std::nullopt;   // a small structure comes back in registers
    std::uint32_t total = returns_by_value ? 4 : 0;   // a class comes back through a hidden pointer
    if (p->convention == "__stdcall" && !p->access.empty() && !p->is_static) total += 4;   // `this` is on the stack
    for (const string& t : p->parameters) {
        if (t == "..." || by_value(t)) return std::nullopt;
        total += (t == "double" || t == "__int64" || t == "unsigned __int64") ? 8 : 4;
    }
    return total;
}

int run(std::span<const std::string> args) {
    if (args.empty()) {
        out_text("codebreaker demangle <name> [<name> ...]\n");
        return kExitUsage;
    }
    for (const string& name : args) {
        if (parse(name) || !name.starts_with('?')) out("{}\n", readable(name));
        else out("{}  (not readable)\n", name);
    }
    return kExitOk;
}

}  // namespace cb::symbols::demangle
