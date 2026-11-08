// codebreaker/symbols/demangle.cpp -- typed front of the demangling engine (native/symbols/demangle.cc).

#include "codebreaker/symbols/demangle.hpp"

#include "py_os.hpp"

// The engine (native/symbols/demangle.cc): the answer as ASCII JSON, false for "not readable".
namespace cbs {
bool demangle_parse(const std::u32string& name, std::string& json);
bool demangle_type_name(const std::u32string& decorated, std::string& json);
}  // namespace cbs

namespace codebreaker::symbols::demangle {

namespace {

// ---- reading the engine's JSON (objects of strings, booleans and one list of strings) -------------
struct Json {
    const std::string& s;
    size_t i = 0;
    explicit Json(const std::string& text) : s(text) {}

    void blanks() { while (i < s.size() && s[i] == ' ') ++i; }
    bool eat(char c) { blanks(); if (i < s.size() && s[i] == c) { ++i; return true; } return false; }
    void need(char c) { if (!eat(c)) throw py::ValueError("bad JSON from the demangler"); }

    unsigned hex4() {
        unsigned v = 0;
        for (int k = 0; k < 4; ++k) {
            char c = s.at(i++);
            v = v * 16 + static_cast<unsigned>(c <= '9' ? c - '0' : c - 'a' + 10);
        }
        return v;
    }

    py::str string() {
        need('"');
        std::u32string out;
        while (s.at(i) != '"') {
            char c = s[i++];
            if (c != '\\') { out.push_back(static_cast<char32_t>(static_cast<unsigned char>(c))); continue; }
            char e = s.at(i++);
            if (e != 'u') { out.push_back(static_cast<char32_t>(e)); continue; }
            unsigned v = hex4();
            // a high surrogate followed by a low one is one character, as json.loads reads it
            if (v >= 0xd800 && v < 0xdc00 && i + 1 < s.size() && s[i] == '\\' && s[i + 1] == 'u') {
                size_t save = i;
                i += 2;
                unsigned lo = hex4();
                if (lo >= 0xdc00 && lo < 0xe000) { out.push_back(0x10000 + ((v - 0xd800) << 10) + (lo - 0xdc00)); continue; }
                i = save;
            }
            out.push_back(static_cast<char32_t>(v));
        }
        ++i;
        return py::str(out);
    }

    bool boolean() {
        blanks();
        if (s.compare(i, 4, "true") == 0) { i += 4; return true; }
        i += 5;
        return false;
    }

    py::list<py::str> strings() {
        py::list<py::str> out;
        need('[');
        if (eat(']')) return out;
        do out.append(string()); while (eat(','));
        need(']');
        return out;
    }
};

Parsed parsed_from(const std::string& text) {
    Parsed p;
    Json j(text);
    j.need('{');
    do {
        py::str key = j.string();
        j.need(':');
        std::string k = key.utf8();
        if (k == "name") p.name = j.string();
        else if (k == "base") p.base = j.string();
        else if (k == "class") p.class_ = j.string();
        else if (k == "kind") p.kind = j.string();
        else if (k == "access") p.access = j.string();
        else if (k == "static") p.static_ = j.boolean();
        else if (k == "virtual") p.virtual_ = j.boolean();
        else if (k == "far") p.far_ = j.boolean();
        else if (k == "convention") p.convention = j.string();
        else if (k == "returns") p.returns = j.string();
        else if (k == "parameters") p.parameters = j.strings();
        else if (k == "const") p.const_ = j.string();
        else if (k == "text") p.text = j.string();
        else throw py::ValueError("unknown key from the demangler");
    } while (j.eat(','));
    j.need('}');
    return p;
}

// a class, struct or union named outright, not through a pointer or reference
bool by_value_type(const py::str& t) {
    return t.startswith(std::make_tuple("class ", "struct ", "union ")) && !t.endswith(std::make_tuple("*", "&"));
}

const py::re::Pattern& stdcall_pattern() {
    // (Python: _?([A-Za-z_]\w*)@(\d+)\Z -- see decimal_value for why the digits are \w+ here)
    static const py::re::Pattern p = py::re::compile(py::str("_?([A-Za-z_]\\w*)@(\\w+)\\Z"));
    return p;
}
const py::re::Pattern& fastcall_pattern() {
    static const py::re::Pattern p = py::re::compile(py::str("@([A-Za-z_]\\w*)@(\\w+)\\Z"));
    return p;
}

// int() of the digits a `\d+` took. The regex engine's \d is ASCII only, so the patterns take `\w+` and the
// digits are checked here: str.isdecimal() is Python's \d, and each Unicode decimal digit has its value.
std::optional<int64_t> decimal_value(const py::str& digits) {
    if (!digits.isdecimal()) return std::nullopt;
    int64_t v = 0;
    for (char32_t c : digits.u32()) {
        int64_t d = static_cast<int64_t>(c) - '0';
        if (c >= 0x80)
            for (const auto& r : py::unicode_::decimal_ranges)
                if (c >= r[0] && c <= r[1]) { d = static_cast<int64_t>(c - r[0]) % 10; break; }
        if (v > (INT64_MAX - d) / 10) throw py::OverflowError("int too large for int64");   // Python's int is unbounded
        v = v * 10 + d;
    }
    return v;
}

}  // namespace

// A type's name as run-time type information keeps it -- `.?AVCFoo@@` (a class), `.?AU...` (a
// struct) -- as C++ writes it: "CFoo", "Outer::Inner", "std::vector<int>". None if unreadable.
std::optional<py::str> type_name(const py::str& decorated) {
    if (decorated.empty()) return std::nullopt;
    std::string json;
    if (!cbs::demangle_type_name(decorated.u32(), json)) return std::nullopt;
    return Json(json).string();
}

// What a decorated C++ name says, or None.
std::optional<Parsed> parse(const py::str& name) {
    if (name.empty()) return std::nullopt;
    std::string json;
    if (!cbs::demangle_parse(name.u32(), json)) return std::nullopt;
    return parsed_from(json);
}

// The declaration a decorated C++ name was made from, or None when it is not one.
std::optional<py::str> declaration(const py::str& name) {
    std::optional<Parsed> p = parse(name);
    if (!p) return std::nullopt;
    return p->text;
}

// Just the qualified name (`CFoo::AddRef`), or None.
std::optional<py::str> short_(const py::str& name) {
    std::optional<Parsed> p = parse(name);
    if (!p) return std::nullopt;
    return p->name;
}

// A C linker name without its convention's decoration:
// (name, convention or "", bytes of arguments or None).
std::tuple<py::str, py::str, std::optional<int64_t>> plain(const py::str& name) {
    if (std::optional<py::re::Match> m = stdcall_pattern().match(name))
        if (std::optional<int64_t> n = decimal_value(m->group(2)))
            return {m->group(1), py::str("__stdcall"), n};
    if (std::optional<py::re::Match> m = fastcall_pattern().match(name))
        if (std::optional<int64_t> n = decimal_value(m->group(2)))
            return {m->group(1), py::str("__fastcall"), n};
    if (!name.empty() && name.startswith(py::str("_")) &&
        py::re::fullmatch(py::str("_[A-Za-z_]\\w*"), name))
        return {name.slice(1, name.size()), py::str("__cdecl"), std::nullopt};
    return {name, py::str(), std::nullopt};
}

// The best readable form of any linker name: a C++ declaration, a C name without its
// decoration, or the name as it stands.
py::str readable(const py::str& name) {
    if (!name.empty() && name.startswith(py::str("?"))) {
        std::optional<py::str> d = declaration(name);
        return (d && !d->empty()) ? *d : name;
    }
    return std::get<0>(plain(name));
}

// Bytes of arguments the function removes on return, as far as the name alone settles it
// (None when it does not: a caller-cleans function, a class passed by value).
std::optional<int64_t> stack_bytes(const py::str& name) {
    std::optional<Parsed> p = parse(name);
    if (!p || p->kind != py::str("function")) return std::get<2>(plain(name));
    if ((p->convention != py::str("__stdcall") && p->convention != py::str("__thiscall")) || p->far_)
        return std::nullopt;                                 // 16-bit: a parameter's size depends on the memory model
    bool by_value = by_value_type(p->returns);
    if (by_value && !p->returns.startswith(py::str("class ")))
        return std::nullopt;                                 // a small plain structure comes back in registers
    int64_t total = by_value ? 4 : 0;                        // a class comes back through a hidden pointer
    if (p->convention == py::str("__stdcall") && !p->access.empty() && !p->static_)
        total += 4;                                          // `this` is on the stack
    for (int64_t k = 0; k < p->parameters.size(); ++k) {
        const py::str t = p->parameters[k];
        if (t == py::str("...") || by_value_type(t)) return std::nullopt;
        total += (t == py::str("double") || t == py::str("__int64") || t == py::str("unsigned __int64")) ? 8 : 4;
    }
    return total;
}

// python -m codebreaker demangle <name> [<name> ...]
int64_t main(const py::list<py::str>& argv) {
    if (argv.empty()) {
        py::print(py::str("python -m codebreaker demangle <name> [<name> ...]"));
        return 2;
    }
    for (int64_t k = 0; k < argv.size(); ++k) {
        const py::str name = argv[k];
        if (parse(name) || !name.startswith(py::str("?")))
            py::print(readable(name));
        else
            py::print(name + py::str("  (not readable)"));
    }
    return 0;
}

}  // namespace codebreaker::symbols::demangle
