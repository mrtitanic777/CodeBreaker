// cpp17_python.cc -- the C++17 back end's Python rules (Unit.lang 'python'): a module the Python front
// end lowered (codebreaker/translate/frontends/python, docs/PYTHON_FRONTEND.md) written whole -- a
// header (records, classes, functions, the module's names) and a source with every body -- over the
// support header native/ported/include/py.hpp.
//
// The C++ reads as a person would write the same program: int64_t where Python had an int, value
// structs for records, references (py::ref) where Python shared objects, py.hpp's types -- py::str,
// py::bytes, py::list, py::dict -- where Python's semantics must hold (negative indices, slicing,
// insertion order, floor division, struct). Python's own operations arrive as calls owned by "py":
// py.hpp's core semantics are written here; everything else is a row of the API map
// (codebreaker/java/api_maps/python.json, target "cpp"), whose target is a template ({recv}, {0}, {args}).
#include <algorithm>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <mutex>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

#include "cpp17.hpp"
#include "json.hpp"

namespace cpp17 {
namespace {

const std::unordered_set<std::string>& keywords() {
    static const std::unordered_set<std::string> k = [] {
        std::unordered_set<std::string> s;
        std::istringstream in(
            "alignas alignof and and_eq asm auto bitand bitor bool break case catch char char16_t char32_t "
            "class compl const constexpr const_cast continue decltype default delete do double dynamic_cast else enum explicit "
            "export extern false float for friend goto if inline int long mutable namespace new noexcept not not_eq nullptr "
            "operator or or_eq private protected public register reinterpret_cast return short signed sizeof static static_assert "
            "static_cast struct switch template this thread_local throw true try typedef typeid typename union unsigned using "
            "virtual void volatile wchar_t while xor xor_eq NULL errno assert min max small near far stdin stdout stderr "
            // (the C runtime's macros: a Python name spelled like one cannot be a C++ name)
            "_MAX_DIR _MAX_PATH _MAX_FNAME _MAX_EXT _MAX_DRIVE EOF BUFSIZ FILENAME_MAX INFINITY NAN HUGE_VAL DOMAIN OVERFLOW "
            "UNDERFLOW TLOSS PLOSS SING EXIT_SUCCESS EXIT_FAILURE RAND_MAX CHAR_BIT CHAR_MIN CHAR_MAX INT_MAX INT_MIN UINT_MAX "
            "LONG_MAX LONG_MIN SHRT_MAX SHRT_MIN SEEK_SET SEEK_CUR SEEK_END L_tmpnam TMP_MAX CLOCKS_PER_SEC");
        std::string w;
        while (in >> w) s.insert(w);
        return s;
    }();
    return k;
}

int prec(const std::string& op) {
    static const std::unordered_map<std::string, int> p = {
        {"*", 5}, {"/", 5}, {"%", 5}, {"+", 6}, {"-", 6}, {"<<", 7}, {">>", 7}, {"<", 9}, {"<=", 9}, {">", 9}, {">=", 9},
        {"==", 10}, {"!=", 10}, {"&", 11}, {"^", 12}, {"|", 13}, {"&&", 14}, {"||", 15}};
    auto it = p.find(op);
    return it == p.end() ? 16 : it->second;
}

// a C++ name for a Python one: a keyword or macro gets "_"; a record key that is not an identifier ("bytes per
// sector") has its other characters as "_"
std::string ident(const std::string& name) {
    if (keywords().count(name)) return name + "_";
    std::string out = name;
    for (char& c : out)
        if (!(std::isalnum((unsigned char)c) || c == '_' || (unsigned char)c >= 0x80)) c = '_';
    if (!out.empty() && std::isdigit((unsigned char)out[0])) out = "_" + out;
    // _Upper is the implementation's (MSVC's _STD is a macro): the module-private _STD becomes STD_
    if (out.size() > 1 && out[0] == '_' && std::isupper((unsigned char)out[1])) out = out.substr(1) + "_";
    return out;
}

// ---------------------------------------------------------------------------------------------- the API map
// codebreaker/java/apimap.py's registry, the part this back end reads: a Python symbol's C++ target
// for dialect c++17 (Registry.resolve(name, 'python', 'cpp', 'c++17'), kept only when it resolves under
// its own name and is not 'unsupported').
struct ApiMap {
    std::vector<std::string> dialects;                       // languages.json: cpp's, oldest first
    std::unordered_map<std::string, V> recs;                 // a key -> the first symbol entry's maps
};

V read_json(const std::filesystem::path& path) {
    std::ifstream f(path, std::ios::binary);
    if (!f) return V();
    std::stringstream ss;
    ss << f.rdbuf();
    try {
        return py::json_parse(ss.str());
    } catch (...) {
        return V();
    }
}

std::vector<std::string> norm_keys(const std::string& name0) {
    std::string name = py::strip(name0);
    std::vector<std::string> keys{name};
    auto parts = py::split(name, ".");
    if (parts.size() >= 2) {
        std::string hm = parts[parts.size() - 2] + "." + parts.back();
        if (std::find(keys.begin(), keys.end(), hm) == keys.end()) keys.push_back(hm);
    }
    if (std::find(keys.begin(), keys.end(), parts.back()) == keys.end()) keys.push_back(parts.back());
    return keys;
}

const ApiMap& api_map(const std::string& root) {
    static std::mutex mu;
    static std::map<std::string, std::unique_ptr<ApiMap>> maps;
    std::lock_guard<std::mutex> lock(mu);
    auto& slot = maps[root];
    if (slot) return *slot;
    slot.reset(new ApiMap());
    ApiMap& m = *slot;
    namespace fs = std::filesystem;
    fs::path dir = fs::u8path(root);
    V cat = read_json(dir / "languages.json");
    if (cat.is_dict()) {
        const V& d = cat.get("languages").get("cpp").get("dialects");
        if (d.is_list())
            for (auto& x : d.seq()) m.dialects.push_back(str(x));
    }
    std::vector<std::string> files;
    std::error_code ec;
    for (auto& e : fs::directory_iterator(dir, ec)) {
        std::string n = e.path().filename().u8string();
        if (ends(n, ".json") && n != "languages.json") files.push_back(n);
    }
    std::sort(files.begin(), files.end());
    for (auto& fn : files) {
        V data = read_json(dir / fs::u8path(fn));
        if (!data.is_dict()) continue;
        std::string stem = fn.substr(0, fn.size() - 5);
        std::string sub = data.has("subsystem") ? str(data.get("subsystem")) : stem;
        std::string lang = data.has("language") ? str(data.get("language")) : py::split(sub, ".")[0];
        if (lang != "python") continue;
        const V& syms = data.get("symbols");
        if (!syms.is_dict()) continue;
        for (auto& kv : syms.items()) {
            V maps = kv.second.get("maps");
            if (!maps.is_dict()) maps = V::dict();
            for (auto& key : norm_keys(str(kv.first)))
                if (!m.recs.count(key)) m.recs[key] = maps;
        }
    }
    return m;
}

// the C++ template for a Python name, or empty (has = false)
std::string api_target(const std::string& root, const std::string& name, bool& has) {
    has = false;
    const ApiMap& m = api_map(root);
    auto it = m.recs.find(py::strip(name));
    if (it == m.recs.end() || !it->second.has("cpp")) return std::string();
    if (py::strip(name) != name) return std::string();
    const V& e = it->second.get("cpp");
    std::string target = str(e.get("target")), kind = e.has("kind") ? str(e.get("kind")) : "direct";
    const V& by = e.get("by_version");
    const std::string dialect = "c++17";
    if (py::truthy(by)) {
        auto pos = std::find(m.dialects.begin(), m.dialects.end(), dialect);
        const V* o = nullptr;
        if (pos != m.dialects.end()) {
            for (auto v = pos + 1; v != m.dialects.begin();) {
                --v;
                if (by.has(V(*v))) {
                    o = &by.get(V(*v));
                    break;
                }
            }
        } else if (by.has(V(dialect))) {
            o = &by.get(V(dialect));
        }
        if (o) {
            if (o->has("target")) target = str(o->get("target"));
            if (o->has("kind")) kind = str(o->get("kind"));
        }
    }
    if (kind == "unsupported") return std::string();
    has = true;
    return target;
}

// ---------------------------------------------------------------------------------------------- literals
uint32_t next_cp(const std::string& s, size_t& i) { return py::decode_cp(s, i); }

std::string oct3(unsigned ch) {
    char b[8];
    std::snprintf(b, sizeof b, "\\%03o", ch);
    return b;
}

// a C++ string literal holding the UTF-8 of `s`
std::string cpp_string(const std::string& s) {
    std::string out = "\"";
    for (unsigned char ch : s) {
        if (ch == '\\') out += "\\\\";
        else if (ch == '"') out += "\\\"";
        else if (ch == '\n') out += "\\n";
        else if (ch == '\t') out += "\\t";
        else if (ch == '\r') out += "\\r";
        else if (ch >= 0x20 && ch < 0x7F) out += (char)ch;
        else out += oct3(ch);
    }
    out += "\"";
    if (s.find('\0') != std::string::npos)   // (a NUL inside: the literal keeps its length)
        return "std::string(" + out + ", " + std::to_string(s.size()) + ")";
    return out;
}

// a py::bytes literal from a latin-1 string
std::string cpp_bytes(const std::string& s) {
    std::vector<unsigned> raw;
    for (size_t i = 0; i < s.size();) {
        uint32_t cp = next_cp(s, i);
        if (cp > 0xFF) throw std::runtime_error("UnicodeEncodeError: 'latin-1' codec can't encode a character");
        raw.push_back(cp);
    }
    std::string body;
    bool wide = false;
    for (unsigned ch : raw) {
        if (ch == 0 || ch >= 0x80) wide = true;
        if (ch == '\\') body += "\\\\";
        else if (ch == '"') body += "\\\"";
        else if (ch >= 0x20 && ch < 0x7F && ch != '?') body += (char)ch;
        else body += oct3(ch);
    }
    if (wide) return "py::bytes(\"" + body + "\", " + std::to_string(raw.size()) + ")";
    return "py::bytes(\"" + body + "\")";
}

bool lit_none(const NodeP& e) { return is(e, "Lit") && raw(e, "value").is_none(); }
bool lit_str(const NodeP& e) { return is(e, "Lit") && raw(e, "value").is_str(); }
std::string lit_s(const NodeP& e) { return str(raw(e, "value")); }
bool is_py_call(const NodeP& e) { return is(e, "Call") && sowner(sym(e)) == "py"; }
bool tname_is(const TypeP& t, const char* n) { return t && tname(t) == n; }
bool is_class(const TypeP& t, const char* n) { return t && t->kind == "class" && tname(t) == n; }

// a Lit as L2 JSON gives it (value, text), typed
NodeP make_lit(const V& value, const TypeP& t) {
    V d = V::dict();
    d.set("node", V("Lit"));
    if (!value.is_none()) d.set("value", value);
    NodeP n = l2::from_json(d);
    set_type(n, t);
    return n;
}

// ---------------------------------------------------------------------------------------------- the writer
struct PyWriter : Emitter {
    std::string module;                       // the module's qualified name
    std::string api_root;
    std::set<std::string> locals;
    std::set<std::string> class_members;      // the class whose method is being written: its members' names
    TypeP ret;
    PyWriter(const std::string& mod, const std::string& api) : Emitter("    "), module(mod), api_root(api) {}

    // ---------------------------------------------------------------- names, types
    std::string qual(const std::string& q) {
        auto p = py::rpartition(q, ".");
        if (p.a == module || p.a.empty()) return ident(p.b);
        Strs out;
        for (auto& x : py::split(q, ".")) out.push_back(ident(x));
        return "::" + join(out, "::");
    }

    std::string ctype(const TypeP& t) {
        std::string k = tkind(t);
        if (k == "i64") return "int64_t";
        if (k == "bool") return "bool";
        if (k == "f64") return "double";
        if (k == "void") return "void";
        if (k == "class") {
            std::string n = tname(t);
            if (n == "py.optional") {
                TypeP inner = t->args[0];
                if (inner->kind == "class" && tinfo(inner, "ref")) return ctype(inner);
                return optional_of(ctype(inner));
            }
            if (n == "py.tuple") {
                Strs a;
                for (auto& x : t->args) a.push_back(ctype(x));
                return "std::tuple<" + join(a, ", ") + ">";
            }
            if (n == "py.iter" || n == "py.generator") return "py::" + n.substr(3) + "<" + ctype(t->args[0]) + ">";
            if (n == "py.list" || n == "py.set" || n == "py.dict") {
                Strs a;
                for (auto& x : t->args) a.push_back(ctype(x));
                return "py::" + n.substr(3) + "<" + join(a, ", ") + ">";
            }
            if (n == "py.variant" || n == "py.fn") {
                Strs a;
                for (auto& x : t->args) a.push_back(ctype(x));
                if (n == "py.variant") return "py::variant<" + join(a, ", ") + ">";
                std::string r = a.empty() ? "void" : a[0];
                return "std::function<" + r + "(" + join(Strs(a.begin() + (a.empty() ? 0 : 1), a.end()), ", ") + ")>";
            }
            if (starts(n, "py.")) return "py::" + py::replace(n.substr(3), ".", "::");   // (py.re.Match: py::re::Match)
            if (tinfo(t, "record")) {
                std::string base = qual(n);
                return tinfo(t, "ref") ? "py::ref<" + base + ">" : base;
            }
            if (tinfo(t, "exception")) return qual(n);
            return "py::ref<" + qual(n) + ">";
        }
        throw Decline("type " + type_repr(t));
    }

    std::string param_type(const TypeP& t, bool assigned) {
        std::string c = ctype(t);
        if (!assigned && t->kind == "class") {
            std::string n = tname(t);
            if (n == "py.str" || n == "py.bytes" || n == "py.bytearray" || n == "py.tuple" ||
                (tinfo(t, "record") && !tinfo(t, "ref")))
                return "const " + c + "&";
        }
        return c;
    }

    const char* zero(const TypeP& t) {
        std::string k = tkind(t);
        if (k == "i64") return "0";
        if (k == "bool") return "false";
        if (k == "f64") return "0.0";
        return nullptr;
    }

    std::string none_of(const TypeP& t) {
        if (is_class(t, "py.optional")) {
            TypeP inner = t->args[0];
            if (inner->kind == "class" && tinfo(inner, "ref")) return "nullptr";
            return NULLOPT;
        }
        if (t && t->kind == "class" && tinfo(t, "ref")) return "nullptr";
        throw Decline("None as " + type_repr(t));
    }

    bool is_nullable_ref(TypeP t) {
        if (!t) return false;
        if (is_class(t, "py.optional")) t = t->args[0];
        return t->kind == "class" && tinfo(t, "ref");
    }

    // ---------------------------------------------------------------- statements
    void nested(const NodeP& b, Strs& out, const std::string& level) override { inner(b, out, deeper(level)); }
    std::string cond_text(const NodeP& e) override { return expr(e); }

    void inner(const NodeP& b, Strs& out, const std::string& ind) {
        if (is(b, "Block")) {
            for (auto& x : nodes(b, "stmts")) stmt(x, out, ind);
        } else {
            stmt(b, out, ind);
        }
    }

    void stmt(const NodeP& s, Strs& out, const std::string& pad) {
        std::string k = s->kind();
        if (k == "Block") {
            emit_block(s, out, pad);
        } else if (k == "Local") {
            SymP sy = sym(s);
            locals.insert(sname(sy));
            TypeP t = type(s);
            std::string cst = has_mod(s, "const") ? "const " : "";
            std::string nm = ident(sname(sy));
            NodeP init = node(s, "init");
            if (!init) {
                const char* z = zero(t);
                if (!z) out.push_back(pad + cst + ctype(t) + " " + nm + ";");
                else out.push_back(pad + cst + ctype(t) + " " + nm + " = " + z + ";");
            } else {
                std::string iv = expr(init, t);
                bool bare = false;
                if (is(init, "Call") && sowner(sym(init)) == "py") {
                    std::string n = sname(sym(init));
                    if (n == "empty" || ((n == "list" || n == "set" || n == "dict") && nodes(init, "args").empty()) ||
                        (n == "record" && ends(iv, "{}") && !tinfo(type(init), "ref")))
                        bare = true;
                }
                if (bare) out.push_back(pad + cst + ctype(t) + " " + nm + ";");
                else out.push_back(pad + cst + ctype(t) + " " + nm + " = " + iv + ";");
            }
        } else if (k == "ExprStmt") {
            NodeP e = node(s, "expr");
            NodeP tg = is(e, "Assign") ? node(e, "target") : nullptr;
            if (tg && is(tg, "Call") && sowner(sym(tg)) == "py" && (sname(sym(tg)) == "bind" || sname(sym(tg)) == "tie")) {
                Strs names;
                for (auto& a : nodes(tg, "args")) names.push_back(ident(sname(sym(a))));
                for (auto& n : names) locals.insert(n);
                if (sname(sym(tg)) == "bind") out.push_back(pad + binding(names) + " = " + expr(node(e, "value")) + ";");
                else out.push_back(pad + "std::tie(" + join(names, ", ") + ") = " + expr(node(e, "value")) + ";");
                return;
            }
            out.push_back(pad + expr(e) + ";");
        } else if (k == "If") {
            emit_if(s, out, pad);
        } else if (k == "While") {
            emit_while(s, out, pad);
        } else if (k == "For") {
            NodeP init = nodes(s, "init")[0];
            std::string ini = ctype(type(init)) + " " + ident(sname(sym(init))) + " = " + expr(node(init, "init"));
            locals.insert(sname(sym(init)));
            Strs upd;
            for (auto& u : nodes(s, "update")) upd.push_back(expr(u));
            out.push_back(pad + "for (" + ini + "; " + expr(node(s, "cond")) + "; " + join(upd, ", ") + ") {");
            inner(node(s, "body"), out, deeper(pad));
            out.push_back(pad + "}");
        } else if (k == "ForEach") {
            NodeP v = node(s, "var");
            std::string head;
            if (has_mod(v, "bind")) {
                auto names = py::split(sname(sym(v)), ",");
                Strs ids;
                for (auto& n : names) {
                    locals.insert(n);
                    ids.push_back(ident(n));
                }
                head = binding(ids, "const auto&");
            } else {
                locals.insert(sname(sym(v)));
                head = loop_var_type(type(v)) + " " + ident(sname(sym(v)));
            }
            out.push_back(pad + range_for(head, iterable(node(s, "iterable"))));
            inner(node(s, "body"), out, deeper(pad));
            out.push_back(pad + "}");
        } else if (k == "Break") {
            out.push_back(pad + "break;");
        } else if (k == "Continue") {
            out.push_back(pad + "continue;");
        } else if (k == "Return") {
            NodeP e = node(s, "expr");
            if (!e) out.push_back(pad + "return;");
            else out.push_back(pad + "return " + expr(e, ret) + ";");
        } else if (k == "Throw") {
            NodeP e = node(s, "expr");
            if (!e) out.push_back(pad + "throw;");
            else out.push_back(pad + "throw " + expr(e) + ";");
        } else if (k == "Try") {
            out.push_back(pad + "try {");
            inner(node(s, "body"), out, deeper(pad));
            for (auto& c : nodes(s, "catches"))
                for (auto& t : types(c, "types")) {
                    out.push_back(pad + "} catch (const " + exc_type(t) + "&) {");
                    inner(node(c, "body"), out, deeper(pad));
                }
            out.push_back(pad + "}");
        } else if (k == "Comment") {
            auto lines = py::splitlines(py::strip(str(raw(s, "text"))));
            if (lines.empty()) lines.push_back("");
            for (auto& line : lines) out.push_back(pad + "// " + py::rstrip(line));
        } else if (k == "Labeled") {   // (a generator's resume points)
            out.push_back(pad + str(raw(s, "label")) + ":;");
            if (node(s, "body")) stmt(node(s, "body"), out, pad);
        } else if (k == "Goto") {
            out.push_back(pad + "goto " + str(raw(s, "label")) + ";");
        } else if (k == "Empty") {
        } else {
            throw Decline("statement " + k);
        }
    }

    std::string loop_var_type(const TypeP& t) {
        std::string c = ctype(t);
        std::string k = tkind(t);
        if (k == "i64" || k == "bool" || k == "f64") return c;
        return "const " + c + "&";
    }

    std::string exc_type(const TypeP& t) {
        std::string n = tname(t);
        if (starts(n, "py.")) return "py::" + n.substr(3);
        return qual(n);
    }

    std::string iterable(const NodeP& e) {
        if (is_py_call(e)) {
            std::string n = sname(sym(e));
            if (n == "enumerate") {
                Strs a;
                for (auto& x : nodes(e, "args")) a.push_back(expr(x));
                return "py::enumerate(" + join(a, ", ") + ")";
            }
            if (n == "zip" || n == "reversed") {
                Strs a;
                for (auto& x : nodes(e, "args")) a.push_back(expr(x));
                return "py::" + n + "(" + join(a, ", ") + ")";
            }
            if (n == "dict.items" || n == "dict.keys" || n == "dict.values")
                return operand(node(e, "target")) + "." + py::split(n, ".")[1] + "()";
        }
        if (is_class(type(e), "py.tuple")) return "py::tuple_items(" + expr(e) + ")";   // (its items are of one type)
        return is(e, "Lit") ? operand(e) : expr(e);
    }

    // ---------------------------------------------------------------- expressions
    std::string operand(const NodeP& e) {
        if (lit_str(e) && tname_is(type(e), "py.str")) return "py::str(" + cpp_string(lit_s(e)) + ")";
        std::string s = expr(e);
        if (is(e, "Binary") || is(e, "Cond") || is(e, "Assign") || is(e, "Concat") ||
            (is(e, "Unary") && !py::truthy(raw(e, "postfix"))) ||
            (!s.empty() && s[0] == '*'))   // (an optional's value: *x.f() would be *(x.f()))
            return "(" + s + ")";
        return s;
    }

    std::string sub(const NodeP& e, int parent, bool right = false) {
        std::string s = expr(e);
        if (is(e, "Paren")) return s;
        if (is(e, "Binary")) {
            int p = prec(str(raw(e, "op")));
            if (p > parent || (right && p == parent)) return "(" + s + ")";
        } else if (is(e, "Cond") || is(e, "Assign")) {
            return "(" + s + ")";
        }
        return s;
    }

    std::string expr(const NodeP& e, const TypeP& want = nullptr) {
        std::string k = e ? e->kind() : std::string("NoneType");
        static const std::set<std::string> known = {"Paren", "Lit",    "Name", "TypeName", "This", "Select", "Index",
                                                    "Unary", "Binary", "Concat", "Assign", "Cond", "Cast", "New",
                                                    "Lambda", "Call"};
        if (!known.count(k)) throw Decline("expression " + k);
        if (want && lit_none(e)) return none_of(want);
        if (k == "Paren") return "(" + expr(node(e, "x")) + ")";
        if (k == "Lit") return x_lit(e);
        if (k == "Name") return ident(sname(sym(e)));
        if (k == "TypeName") return qual(tname(type(e)));
        if (k == "This") return "self()";
        if (k == "Select") return x_select(e);
        if (k == "Index") return operand(node(e, "arr")) + "[" + expr(node(e, "index")) + "]";
        if (k == "Unary") return x_unary(e);
        if (k == "Binary") {
            std::string op = str(raw(e, "op"));
            int p = prec(op);
            NodeP l = node(e, "left");
            if (op == "<<" && tkind(type(l)) == "i64" && !(is(l, "Name") || is(l, "Select") || is(l, "Call") || is(l, "Index"))) {
                // (a literal, or an expression of literals, is a C++ int: shifted as Python's int, it is int64_t)
                std::string ls = is(l, "Lit") ? "INT64_C(" + expr(l) + ")" : "static_cast<int64_t>(" + expr(l) + ")";
                if (starts(ls, "INT64_C(INT64_C(")) ls = expr(l);
                return ls + " " + op + " " + sub(node(e, "right"), p, true);
            }
            return sub(l, p) + " " + op + " " + sub(node(e, "right"), p, true);
        }
        if (k == "Concat") return x_concat(e);
        if (k == "Assign") return x_assign(e);
        if (k == "Cond") return x_cond(e);
        if (k == "Cast") return "static_cast<" + ctype(type(e)) + ">(" + expr(node(e, "x")) + ")";
        if (k == "New") return x_new(e);
        if (k == "Lambda") {
            Strs ps;
            for (auto& p : nodes(e, "params")) ps.push_back(param_type(type(p), false) + " " + ident(sname(sym(p))));
            NodeP body = node(e, "body");
            if (is(body, "Block")) {   // (a nested def: a closure with statements)
                TypeP ft = is_class(type(e), "py.fn") ? type(e) : want;
                std::string r = ft && is_class(ft, "py.fn") && !ft->args.empty() ? ctype(ft->args[0]) : "void";
                TypeP saved = ret;
                ret = ft && !ft->args.empty() ? ft->args[0] : nullptr;
                Strs lines;
                inner(body, lines, "        ");
                ret = saved;
                return "[&](" + join(ps, ", ") + ") -> " + r + " {\n" + join(lines, "\n") + "\n    }";
            }
            return "[&](" + join(ps, ", ") + ") { return " + expr(body) + "; }";
        }
        return x_call(e);
    }

    std::string x_lit(const NodeP& e) {
        const V& v = raw(e, "value");
        TypeP t = type(e);
        if (v.is_none()) {
            if (!t || t->kind == "void") return "py::none";
            return none_of(t);
        }
        if (v.is_bool()) return v.b ? "true" : "false";
        double fv;
        if (v.is_str() && tkind(t) == "f64" && lit_float(e, fv)) return float_lit(fv);
        if (v.is_int()) {
            std::string text = str(raw(e, "text"));
            std::string txt;
            if (!text.empty() && re::rx("0[xX][0-9a-fA-F_]+").fullmatch(text)) {
                std::string s = py::replace(text, "_", "");
                txt = s.size() > 2 ? "0x" + py::upper(s.substr(2)) : s;
            } else if (!text.empty() && re::rx("0[oO][0-7_]+").fullmatch(text)) {
                txt = "0" + py::replace(text.substr(2), "_", "");
            } else if (!text.empty() && re::rx("0[bB][01_]+").fullmatch(text)) {
                txt = "0b" + py::replace(text.substr(2), "_", "");
            } else {
                txt = v.u64 ? std::to_string((unsigned long long)v.i) : std::to_string(v.i);
            }
            bool big = v.u64 || v.i > 2147483647LL || v.i < -2147483648LL;
            if (big) {
                if (!v.u64 && v.i == INT64_MIN) return "INT64_MIN";
                return "INT64_C(" + txt + ")";
            }
            return txt;
        }
        if (v.is_float()) return float_lit(v.f);
        if (v.is_str()) {
            if (t && t->kind == "class" && (tname(t) == "py.bytes" || tname(t) == "py.bytearray")) return cpp_bytes(v.str());
            return cpp_string(v.str());
        }
        throw Decline("literal " + py::repr(v));
    }

    std::string float_lit(double f) {
        std::string r = py::float_repr(f);
        if (r == "inf") return "INFINITY";
        if (r == "-inf") return "-INFINITY";
        if (r == "nan") return "NAN";
        return (r.find('.') != std::string::npos || r.find('e') != std::string::npos) ? r : r + ".0";
    }

    std::string str_arg(const NodeP& e) {
        if (lit_str(e) && tname_is(type(e), "py.str")) return cpp_string(lit_s(e));
        return expr(e);
    }

    std::string x_select(const NodeP& e) {
        SymP s = sym(e);
        if (skind(s) == "global") return qual(sowner(s) + "." + sname(s));
        NodeP obj = node(e, "obj");
        if (is(obj, "TypeName")) return qual(tname(type(obj))) + "::" + ident(sname(s));
        if (is(obj, "This")) {
            if (locals.count(sname(s))) return "this->" + ident(sname(s));
            return ident(sname(s));
        }
        TypeP ot = type(obj);
        bool arrow = ot && ot->kind == "class" &&
                     (tinfo(ot, "ref") || (tname(ot) == "py.optional" && !ot->args.empty() && tinfo(ot->args[0], "ref")));
        return operand(obj) + (arrow ? "->" : ".") + ident(sname(s));
    }

    std::string x_unary(const NodeP& e) {
        std::string op = str(raw(e, "op"));
        NodeP xn = node(e, "x");
        if (py::truthy(raw(e, "postfix"))) return operand(xn) + op;
        std::string x = expr(xn);
        if (!is(xn, "Paren") && !re::rx("[\\w:.<>]*(\\(.*\\)|\\[.*\\])?|-?\\d+").fullmatch(x)) x = "(" + x + ")";
        if (op == "-" && starts(x, "-")) x = "(" + x + ")";
        return op + x;
    }

    std::string x_concat(const NodeP& e) {
        Strs parts;
        auto& ps = nodes(e, "parts");
        for (size_t i = 0; i < ps.size(); i++) {
            const NodeP& x = ps[i];
            std::string s = expr(x);
            if (i == 0 && is(x, "Lit")) s = "py::str(" + cpp_string(lit_s(x)) + ")";
            else if (lit_str(x)) s = cpp_string(lit_s(x));
            else if (is(x, "Binary") || is(x, "Cond")) s = "(" + s + ")";
            parts.push_back(s);
        }
        return join(parts, " + ");
    }

    std::string x_assign(const NodeP& e) {
        NodeP tg = node(e, "target");
        std::string t = expr(tg);
        std::string v = expr(node(e, "value"), type(tg));
        if (is(tg, "Index")) {
            TypeP at = type(node(tg, "arr"));
            if (at && tname(at) == "py.dict")
                return operand(node(tg, "arr")) + ".set(" + expr(node(tg, "index")) + ", " + v + ")";
        }
        std::string op = str(raw(e, "op"));
        if (!op.empty()) return t + " " + op + "= " + v;
        return t + " = " + v;
    }

    std::string x_cond(const NodeP& e) {
        TypeP t = type(e);
        NodeP th = node(e, "then"), el = node(e, "orelse");
        std::string a = expr(th, t);
        std::string b = expr(el, t);
        if (is_class(t, "py.optional") && !is_nullable_ref(t)) {
            std::string ct = ctype(t);
            if (!lit_none(th) && !type_eq(type(th), t)) a = ct + "(" + a + ")";
            if (!lit_none(el) && !type_eq(type(el), t)) b = ct + "(" + b + ")";
        } else if (is_class(t, "py.str")) {
            if (lit_str(th)) a = "py::str(" + cpp_string(lit_s(th)) + ")";
        } else {
            if (is(el, "Cond")) b = "(" + b + ")";
        }
        if (is(el, "Cond")) b = expr(el, t);
        std::string c = sub(node(e, "cond"), 15);
        std::string mid = !starts(a, "std::optional") ? sub(th, 15) : a;
        return c + " ? " + mid + " : " + b;
    }

    std::string args_of(const NodeP& e) {
        Strs a;
        for (auto& x : nodes(e, "args")) a.push_back(expr(x));
        return join(a, ", ");
    }

    std::string x_new(const NodeP& e) {
        SymP s = sym(e);
        std::string args = args_of(e);
        if (sowner(s) == "py") return "py::" + sname(s) + "(" + args + ")";
        if (str(raw(e, "memory")) == "stack" || tinfo(type(e), "exception")) return qual(sowner(s)) + "(" + args + ")";
        return make_shared(qual(sowner(s)), args);
    }

    bool typearg_has(const NodeP& e, const char* what) {
        const V& ta = raw(e, "typeargs");
        if (!ta.is_seq()) return false;
        for (auto& x : ta.seq())
            if (x.is_str() && x.str() == what) return true;
        return false;
    }

    std::string x_call(const NodeP& e) {
        SymP s = sym(e);
        if (sowner(s) == "py") return py_call(e);
        std::string args = args_of(e);
        if (skind(s) == "ctor" && typearg_has(e, "base")) return qual(sowner(s)) + "(" + args + ")";
        NodeP t = node(e, "target");
        if (!t) {
            if (skind(s) == "function") {
                // a module function a member of the method's class hides (Run.ended() calling ended(e)): qualified
                if (sowner(s) == module && class_members.count(sname(s))) {
                    Strs q;
                    for (auto& x : py::split(module, ".")) q.push_back(ident(x));
                    return "::" + join(q, "::") + "::" + ident(sname(s)) + "(" + args + ")";
                }
                return qual(sowner(s) + "." + sname(s)) + "(" + args + ")";
            }
            return ident(sname(s)) + "(" + args + ")";
        }
        if (is(t, "TypeName")) return qual(tname(type(t))) + "::" + ident(sname(s)) + "(" + args + ")";
        if (is(t, "This")) return ident(sname(s)) + "(" + args + ")";
        return operand(t) + "->" + ident(sname(s)) + "(" + args + ")";
    }

    std::string typed_item(const NodeP& x, const TypeP& t) {
        if (lit_none(x)) return none_of(t);
        if (lit_str(x) && t && tname(t) == "py.str") return "py::str(" + cpp_string(lit_s(x)) + ")";
        return expr(x);
    }

    // -- the porting engine's library rows (py_os.hpp): open and files, os / os.path / shutil / time,
    // hashlib, re, variants. Keyword arguments go to their slot; a slot left out before a given one takes
    // Python's default.
    std::string variant_tag(const std::string& tag) {
        if (tag == "int") return "int64_t";
        if (tag == "float") return "double";
        if (tag == "bool") return "bool";
        if (tag == "str" || tag == "bytes" || tag == "bytearray") return "py::" + tag;
        throw Decline("variant tag " + tag);
    }

    bool ext_call(const NodeP& e, const std::string& n, std::string& out) {
        static const std::map<std::string, std::vector<std::pair<std::string, std::string>>> slots = {
            // name -> [(keyword, default)] for every slot after the receiver
            {"open", {{"file", ""}, {"mode", "py::str(\"r\")"}, {"encoding", "std::nullopt"}, {"errors", "std::nullopt"}, {"newline", "std::nullopt"}}},
            {"os.makedirs", {{"name", ""}, {"mode", "0777"}, {"exist_ok", "false"}}},
            {"shutil.rmtree", {{"path", ""}, {"ignore_errors", "false"}}},
            {"re.sub", {{"pattern", ""}, {"repl", ""}, {"string", ""}, {"count", "0"}, {"flags", "0"}}},
            {"re.subn", {{"pattern", ""}, {"repl", ""}, {"string", ""}, {"count", "0"}, {"flags", "0"}}},
            {"re.split", {{"pattern", ""}, {"string", ""}, {"maxsplit", "0"}, {"flags", "0"}}},
            {"re.match", {{"pattern", ""}, {"string", ""}, {"flags", "0"}}},
            {"re.search", {{"pattern", ""}, {"string", ""}, {"flags", "0"}}},
            {"re.fullmatch", {{"pattern", ""}, {"string", ""}, {"flags", "0"}}},
            {"re.findall", {{"pattern", ""}, {"string", ""}, {"flags", "0"}}},
            {"re.finditer", {{"pattern", ""}, {"string", ""}, {"flags", "0"}}},
            {"re.compile", {{"pattern", ""}, {"flags", "0"}}},
            {"re.Pattern.sub", {{"repl", ""}, {"string", ""}, {"count", "0"}}},
            {"re.Pattern.subn", {{"repl", ""}, {"string", ""}, {"count", "0"}}},
            {"re.Pattern.split", {{"string", ""}, {"maxsplit", "0"}}},
        };
        static const char* prefixes[] = {"os.", "shutil.", "time.", "hashlib.", "re.", "file.", "hash.", "variant."};
        bool mine = n == "open";
        for (auto p : prefixes) mine = mine || starts(n, p);
        if (!mine) return false;
        static const std::set<std::string> old_rows = {"os.listdir", "os.path.join", "os.path.exists", "os.path.isdir", "os.path.isfile",
                                                       "os.path.getsize", "os.path.basename", "os.path.dirname", "os.path.split",
                                                       "os.path.splitext", "os.path.normcase"};
        if (old_rows.count(n)) return false;   // (the API map's rows, as the old front end wrote them)
        const auto& a = nodes(e, "args");
        TypeP et = type(e);
        NodeP tgt = node(e, "target");
        // the arguments in slot order
        Strs pos, args;
        std::map<std::string, std::string> kws;
        for (auto& x : a) {
            if (is(x, "Call") && sname(sym(x)) == "kw") kws[str(raw(x, "typeargs")[0])] = expr(nodes(x, "args")[0]);
            else pos.push_back(typed_item(x, type(x)));
        }
        auto sl = slots.find(n);
        if (sl != slots.end()) {
            const auto& sv = sl->second;
            size_t last = pos.size();
            for (size_t i = 0; i < sv.size(); i++)
                if (kws.count(sv[i].first)) last = std::max(last, i + 1);
            for (size_t i = 0; i < last; i++) {
                if (i < pos.size()) args.push_back(pos[i]);
                else if (kws.count(sv[i].first)) args.push_back(kws[sv[i].first]);
                else if (!sv[i].second.empty()) args.push_back(sv[i].second);
                else throw Decline(n + " without its " + sv[i].first);
                kws.erase(sv[i].first);
            }
        } else {
            args = pos;
        }
        if (!kws.empty()) throw Decline(n + "(" + kws.begin()->first + "=)");
        if (starts(n, "re."))   // (py::re is over str: a bytes pattern's \w and \s are ASCII's)
            for (auto& x : a)
                if (tname_is(type(x), "py.bytes") || tname_is(type(x), "py.bytearray")) throw Decline(n + " over bytes");
        std::string recv = tgt ? recv_of(tgt) : std::string();
        std::string as = join(args, ", ");
        auto list_of_tuples = [&](const TypeP& t) {
            return is_class(t, "py.list") && !t->args.empty() && is_class(t->args[0], "py.tuple");
        };
        std::string tuple_types = list_of_tuples(et) ? struct_types(et->args[0]) : std::string();
        if (n == "open") {
            out = "py::open(" + as + ")";
        } else if (starts(n, "file.")) {
            std::string m = n.substr(5);
            bool text = tname_is(et, "py.str") || (is_class(et, "py.list") && !et->args.empty() && tname_is(et->args[0], "py.str"));
            if ((m == "read" || m == "readline" || m == "readlines") && !et) throw Decline("file." + m + " of no type");
            if ((m == "read" || m == "readline" || m == "readlines") && text) m += "_text";
            out = recv + "." + m + "(" + as + ")";
        } else if (starts(n, "hash.")) {
            out = recv + "." + n.substr(5) + "(" + as + ")";
        } else if (n == "variant.of" || n == "variant.is" || n == "variant.get") {
            std::string tag = variant_tag(str(raw(e, "typeargs")[0]));
            if (n == "variant.of") out = ctype(et) + "(std::in_place_type<" + tag + ">, " + as + ")";
            else if (n == "variant.is") out = "std::holds_alternative<" + tag + ">(" + as + ")";
            else out = "py::variant_get_<" + tag + ">(" + as + ")";
        } else if (starts(n, "re.Match.")) {
            std::string m = n.substr(9);
            if (m == "group" && is_class(et, "py.optional")) m = "group_opt";
            if (m == "groups") m = "groups_opt";
            out = recv + "." + m + "(" + as + ")";
        } else if (starts(n, "re.Pattern.")) {
            std::string m = n.substr(11);
            if (m == "findall" && !tuple_types.empty()) out = recv + ".template findall_groups<" + tuple_types + ">(" + as + ")";
            else out = recv + "." + m + "(" + as + ")";
        } else if (starts(n, "re.") && n.size() > 3 && std::isupper((unsigned char)n[3])) {
            out = "py::re::" + n.substr(3);   // re.M, re.DOTALL ...
        } else if (n == "re.findall" && !tuple_types.empty()) {
            Strs rest(args.begin() + 1, args.end());
            out = "py::re::compile(" + args[0] + (args.size() > 2 ? ", " + args[2] : std::string()) + ").template findall_groups<" +
                  tuple_types + ">(" + args[1] + ")";
        } else if (n == "os.sep" || n == "os.linesep" || n == "os.pathsep" || n == "os.curdir" || n == "os.pardir" || n == "os.altsep") {
            out = "py::" + py::replace(n, ".", "::");   // (values, not calls)
        } else if (n == "os.environ.get" || n == "os.getenv") {
            out = "py::os::environ_::get(" + as + ")";
        } else {
            out = "py::" + py::replace(n, ".", "::") + "(" + as + ")";
        }
        return true;
    }

    // a method's receiver: an optional value the front end narrowed is read as its value
    std::string recv_of(const NodeP& t) {
        TypeP tt = type(t);
        if (is_class(tt, "py.optional") && !is_nullable_ref(tt)) return operand(t) + ".value()";
        std::string s = operand(t);
        return starts(s, "*") ? "(" + s + ")" : s;   // (an unwrapped optional: (*m).group(1))
    }

    bool api_has(const std::string& n) {
        bool has = false;
        api_target(api_root, n, has);
        return has;
    }

    // -- Python's operations
    std::string py_call(const NodeP& e) {
        std::string n = sname(sym(e));
        const auto& a = nodes(e, "args");
        TypeP et = type(e);
        auto X = [&](const NodeP& x, const TypeP& w = nullptr) { return expr(x, w); };
        auto all = [&](size_t from = 0) {
            Strs v;
            for (size_t i = from; i < a.size(); i++) v.push_back(X(a[i]));
            return v;
        };
        if (n == "truthy") return truthy(a[0]);
        if (n == "some") return ctype(et) + "(" + X(a[0]) + ")";   // (a generator's next value)
        {
            std::string r;
            if (ext_call(e, n, r)) return r;
        }
        if (n == "is_none" || n == "is_not_none") {
            const NodeP& x = a[0];
            bool neg = n == "is_not_none";
            TypeP t = type(x);
            if (is_class(t, "py.optional") && !is_nullable_ref(t))
                return neg ? operand(x) + ".has_value()" : "!" + operand(x) + ".has_value()";
            if (t && t->kind == "void") return !neg ? "true" : "false";
            if (t && is_nullable_ref(t)) return operand(x) + (neg ? " != " : " == ") + "nullptr";
            if (is(x, "Select") && t && tname(t) == "py.optional")
                return neg ? operand(x) + ".has_value()" : "!" + operand(x) + ".has_value()";
            return !neg ? "false" : "true";
        }
        if (n == "unwrap") {
            const NodeP& x = a[0];
            if (is_nullable_ref(type(x))) return X(x);
            return "*" + operand(x);
        }
        if (n == "opt_value") return operand(a[0]) + ".value()";
        if (n == "value_or") return operand(a[0]) + ".value_or(" + X(a[1], et) + ")";
        if (n == "empty") return ctype(et) + "{}";
        if (n == "tuple") {
            Strs items;
            bool any_lit = false;
            for (size_t i = 0; i < a.size() && i < et->args.size(); i++) items.push_back(typed_item(a[i], et->args[i]));
            for (auto& x : a) any_lit = any_lit || is(x, "Lit");
            if (any_lit) return ctype(et) + "{" + join(items, ", ") + "}";
            return "std::make_tuple(" + join(items, ", ") + ")";
        }
        if (n == "in") return "py::in(" + expr(a[0]) + ", " + expr(a[1]) + ")";
        if (n == "any" || n == "all") return "py::" + n + "_of(" + expr(a[0]) + ")";
        if (n == "get") {
            const NodeP& x = a[0];
            if (is_py_call(x) && (sname(sym(x)) == "struct.unpack_from" || sname(sym(x)) == "struct.unpack") &&
                type(x)->args.size() == 1)
                return struct_unpack(x, true);
            return "std::get<" + py::str_of(raw(e, "typeargs")[0]) + ">(" + X(x) + ")";
        }
        if (n == "list" || n == "set") {
            std::string ct = ctype(et);
            Strs items;
            for (auto& x : a) items.push_back(typed_item(x, et->args[0]));
            return ct + "{" + join(items, ", ") + "}";
        }
        if (n == "dict") {
            std::string ct = ctype(et);
            Strs items;
            for (auto& p : a) {
                auto& pa = nodes(p, "args");
                std::string k = typed_item(pa[0], et->args[0]);
                items.push_back("{" + k + ", " + typed_item(pa[1], et->args[1]) + "}");
            }
            return ct + "{" + join(items, ", ") + "}";
        }
        if (n == "record") {
            std::string name = qual(tname(et));
            Strs vals;
            bool all_none = true;
            for (auto& x : a) {
                vals.push_back(!lit_none(x) ? typed_item(x, type(x)) : "{}");
                all_none = all_none && lit_none(x);
            }
            std::string vs = all_none ? "" : join(vals, ", ");
            if (tinfo(et, "ref")) return make_shared(name, name + "{" + vs + "}");
            return name + "{" + vs + "}";
        }
        if (n == "slice") {
            std::string obj = operand(node(e, "target"));
            std::string lo = lit_none(a[0]) ? "py::none" : X(a[0]);
            std::string hi = lit_none(a[1]) ? "py::none" : X(a[1]);
            if (a.size() > 2) return obj + ".slice(" + lo + ", " + hi + ", " + X(a[2]) + ")";
            if (hi == "py::none") return obj + ".slice_from(" + lo + ")";
            if (lo == "py::none") return obj + ".slice_to(" + hi + ")";
            return obj + ".slice(" + lo + ", " + hi + ")";
        }
        if (n == "set_slice") {   // a[lo:hi] = v
            std::string obj = operand(node(e, "target"));
            std::string lo = lit_none(a[0]) ? "py::none" : X(a[0]);
            std::string hi = lit_none(a[1]) ? "py::none" : X(a[1]);
            return obj + ".set_slice(" + lo + ", " + hi + ", " + X(a[2]) + ")";
        }
        static const std::set<std::string> arith = {"floordiv",    "mod",         "shl",         "shr",
                                                    "ipow",        "truediv",     "checked_add", "checked_sub",
                                                    "checked_mul", "checked_shl", "checked_pow", "checked_neg", "pow"};
        if (arith.count(n)) return "py::" + n + "(" + join(all(), ", ") + ")";
        if (n == "concat") {
            if (is_class(et, "py.tuple")) return "std::tuple_cat(" + X(a[0]) + ", " + X(a[1]) + ")";
            return operand(a[0]) + " + " + operand(a[1]);
        }
        if (n == "repeat") {
            std::string first = (is(a[0], "Lit") && tname_is(type(a[0]), "py.bytes")) ? cpp_bytes(lit_s(a[0])) : operand(a[0]);
            return "py::repeat(" + first + ", " + X(a[1]) + ")";
        }
        if (n == "percent") {
            Strs v{str_arg(a[0])};
            for (size_t i = 1; i < a.size(); i++) v.push_back(X(a[i]));
            return "py::percent(" + join(v, ", ") + ")";
        }
        if (n == "format") return "py::format(" + X(a[0]) + ", " + cpp_string(lit_s(a[1])) + ")";
        if (n == "str") return "py::to_str(" + X(a[0]) + ")";
        if (n == "repr") return "py::repr(" + X(a[0]) + ")";
        if (n == "or" || n == "and") {
            std::string l = X(a[0], et);
            return "py::" + n + "_(" + l + ", " + X(a[1], et) + ")";
        }
        if (n == "or_none" || n == "and_none") {
            std::string l = X(a[0]);
            return "py::or_none(" + l + ", " + X(a[1], et) + ")";
        }
        // (new) `a or b` / `a and b` whose b has effects: b evaluated only when Python evaluates it
        if (n == "or_lazy" || n == "and_lazy" || n == "or_none_lazy") {
            bool none = n == "or_none_lazy";
            std::string l = none ? X(a[0]) : X(a[0], et), r = X(a[1], et), T = ctype(et);
            bool plain = a[0] && a[0]->kind() == "Name";
            std::string v = plain ? l : "v_";
            std::string pick = none ? "(py::truthy(" + v + ") ? *" + v + " : " + T + "(" + r + "))"
                               : n == "or_lazy" ? "(py::truthy(" + v + ") ? " + v + " : " + T + "(" + r + "))"
                                                : "(py::truthy(" + v + ") ? " + T + "(" + r + ") : " + v + ")";
            if (plain) return pick;
            return "[&]() -> " + T + " { auto v_ = " + l + "; return " + pick + "; }()";
        }
        if (n == "contains") return operand(node(e, "target")) + ".contains(" + X(a[0]) + ")";
        if (n == "dict.at") return operand(node(e, "target")) + ".at(" + X(a[0]) + ")";
        if (starts(n, "struct.")) return struct_call(e);
        if (n == "native.no_data") return "py::bytes{}";
        if (n == "native.ask") {   // a native format's answer (py_answers.hpp), decoded as the function's type
            const V& ta = raw(e, "typeargs");
            Strs v{cpp_string(str(ta[0]))};
            for (auto& x : a) v.push_back(X(x));
            bool where = ta.is_seq() && ta.seq().size() > 1 && str(ta[1]) == "where";   // (the place tuple after the bytes)
            return std::string(where ? "py::native::ask_where<" : "py::native::ask<") + ctype(type(e)) + ">(" + join(v, ", ") + ")";
        }
        if (n == "sorted") {
            std::string key = is(a[1], "Lit") ? "nullptr" : X(a[1]);
            std::string rev = a.size() > 2 ? X(a[2]) : "false";
            if (key == "nullptr" && rev == "false") return "py::sorted(" + X(a[0]) + ")";
            return "py::sorted(" + X(a[0]) + ", " + key + ", " + rev + ")";
        }
        if (n == "list.sort") {
            std::string key = is(a[0], "Lit") ? "nullptr" : X(a[0]);
            std::string rev = a.size() > 1 ? X(a[1]) : "false";
            if (key == "nullptr" && rev == "false") return operand(node(e, "target")) + ".sort()";
            return operand(node(e, "target")) + ".sort(" + key + ", " + rev + ")";
        }
        if (n == "print") {
            Strs v;
            for (auto& x : a)
                if (!(is(x, "Call") && sname(sym(x)) == "kw")) v.push_back(X(x));
            return "py::print(" + join(v, ", ") + ")";
        }
        if (n == "enumerate") return "py::enumerate(" + join(all(), ", ") + ")";
        if ((n == "builtins.list" || n == "builtins.set" || n == "builtins.dict") && a.empty()) return ctype(et) + "{}";
        bool has = false;
        std::string tpl = api_target(api_root, n, has);
        if (!has) throw Decline("no C++ for " + n);
        return templ(tpl, e);
    }

    std::string templ(const std::string& tpl, const NodeP& e) {
        static const std::set<std::string> bare = {"bytes.decode", "str.encode", "builtins.format", "int.from_bytes",
                                                   "int.to_bytes"};   // (they take const char*)
        std::string n = sname(sym(e));
        Strs args;
        for (auto& x : nodes(e, "args")) {
            if (is(x, "Call") && sname(sym(x)) == "kw") args.push_back(expr(nodes(x, "args")[0]));
            else args.push_back(bare.count(n) ? str_arg(x) : typed_item(x, type(x)));
        }
        NodeP t = node(e, "target");
        std::string recv = t ? recv_of(t) : std::string();
        std::string commas;
        for (auto& x : args) commas += ", " + x;
        std::string out = py::replace(tpl, "{recv}", recv);
        out = py::replace(out, "{,args}", commas);
        out = py::replace(out, "{args}", join(args, ", "));
        if (out.find("{T}") != std::string::npos) {
            TypeP et = type(e);
            out = py::replace(out, "{T}", tkind(et) != "void" ? ctype(et) : "void");
        }
        for (size_t i = 0; i < args.size(); i++) out = py::replace(out, "{" + std::to_string(i) + "}", args[i]);
        if (re::rx("\\{\\d\\}").search(out)) {
            out = re::rx(", \\{\\d\\}").sub("", out);
            out = re::rx("\\{\\d\\}").sub("", out);
        }
        return out;
    }

    std::string struct_types(const TypeP& t) {
        Strs v;
        for (auto& x : t->args) v.push_back(ctype(x));
        return join(v, ", ");
    }

    std::string struct_unpack(const NodeP& x, bool one = false) {
        std::string fmt = str(raw(x, "typeargs")[0]);
        auto& a = nodes(x, "args");
        std::string buf = expr(a[1]);
        std::string off = a.size() > 2 ? ", " + expr(a[2]) : "";
        std::string fn = sname(sym(x)) == "struct.unpack_from" ? "unpack_from" : "unpack";
        return "py::" + fn + (one ? "1" : "") + "<" + struct_types(type(x)) + ">(" + cpp_string(fmt) + ", " + buf + off + ")";
    }

    std::string struct_call(const NodeP& e) {
        std::string n = sname(sym(e));
        if (n == "struct.unpack_from" || n == "struct.unpack") return struct_unpack(e);
        std::string fmt = str(raw(e, "typeargs")[0]);
        Strs a{cpp_string(fmt)};
        auto& args = nodes(e, "args");
        for (size_t i = 1; i < args.size(); i++) a.push_back(expr(args[i]));
        if (n == "struct.pack") return "py::pack(" + join(a, ", ") + ")";
        if (n == "struct.pack_into") return "py::pack_into(" + join(a, ", ") + ")";
        if (n == "struct.calcsize") return type(e) ? "0" : "None";
        throw Decline(n);
    }

    // a value whose truth is always True: a record (a non-empty dict), a non-empty tuple, a library object
    bool always_true(const TypeP& t) {
        if (!t || t->kind != "class") return false;
        std::string n = tname(t);
        if (tinfo(t, "record") && !tinfo(t, "ref")) return true;
        if (n == "py.tuple") return !t->args.empty();
        return n == "py.re.Match" || n == "py.re.Pattern" || n == "py.file" || n == "py.hashlib.Hash";
    }

    std::string truthy(const NodeP& x) {
        TypeP t = type(x);
        std::string s = operand(x);
        if (!t) throw Decline("truth of an untyped value");
        const std::string& k = t->kind;
        if (k == "bool") return expr(x);
        if (k == "i64") return s + " != 0";
        if (k == "f64") return s + " != 0.0";
        if (k == "class") {
            if (tname(t) == "py.optional") {
                TypeP inner = t->args[0];
                if (is_nullable_ref(t)) return s + " != nullptr";
                if (inner->kind == "i64") return s + ".value_or(0) != 0";
                if (inner->kind == "bool") return s + ".value_or(false)";
                if (always_true(inner)) return s + ".has_value()";
                return "(" + s + ".has_value() && !" + s + "->empty())";
            }
            if (tinfo(t, "ref")) return s + " != nullptr";
            if (always_true(t)) return "true";
            if (tname(t) == "py.tuple") return !t->args.empty() ? "true" : "false";
            if (tname(t) == "py.variant") return "py::truthy(" + expr(x) + ")";   // (the truth of the value it holds)
            return "!" + s + ".empty()";
        }
        if (k == "void") return "false";
        throw Decline("truth of " + type_repr(t));
    }
};

// ---------------------------------------------------------------------------------------------- the module
std::string header_path(const std::string& m) { return py::replace(m, ".", "/") + ".hpp"; }
std::string source_path(const std::string& m) { return py::replace(m, ".", "/") + ".cpp"; }

std::string qname_of(const NodeP& c) { return str(raw(c, "qname")); }
std::string cname_of(const NodeP& c) { return str(raw(c, "name")); }
std::string extends_of(const NodeP& c) {
    TypeP t = type(c, "extends");
    return t ? tname(t) : std::string();
}
bool has_extends(const NodeP& c) { return type(c, "extends") != nullptr; }

struct ByQ {
    std::vector<NodeP> order;
    std::map<std::string, NodeP> m;
    explicit ByQ(const std::vector<NodeP>& cs) {
        for (auto& c : cs) {
            if (!m.count(qname_of(c))) order.push_back(c);
            m[qname_of(c)] = c;
        }
        // (dict((c.qname, c) ...): a later class of the same name takes the earlier's place)
        for (auto& c : order) c = m[qname_of(c)];
    }
    NodeP get(const std::string& q) const {
        auto it = m.find(q);
        return it == m.end() ? nullptr : it->second;
    }
};

std::vector<NodeP> bases_first(const std::vector<NodeP>& classes) {
    ByQ by(classes);
    std::vector<NodeP> out;
    std::set<std::string> seen;
    std::function<void(const NodeP&)> add = [&](const NodeP& c) {
        if (seen.count(qname_of(c))) return;
        seen.insert(qname_of(c));
        if (has_extends(c) && by.get(extends_of(c))) add(by.get(extends_of(c)));
        out.push_back(c);
    };
    for (auto& c : classes) add(c);
    return out;
}

void value_records(const TypeP& t, std::vector<std::string>& out) {
    if (t && t->kind == "class") {
        if (tinfo(t, "record") && !tinfo(t, "ref")) out.push_back(tname(t));
        if (tname(t) == "py.optional" || tname(t) == "py.tuple")
            for (auto& a : t->args) value_records(a, out);
    }
}

// a value record holding another value record needs that one complete first
std::vector<NodeP> records_ordered(const std::vector<NodeP>& records) {
    ByQ by(records);
    std::vector<NodeP> out;
    std::set<std::string> seen;
    std::function<void(const NodeP&, std::vector<std::string>)> add = [&](const NodeP& r, std::vector<std::string> stack) {
        std::string q = qname_of(r);
        if (seen.count(q) || std::find(stack.begin(), stack.end(), q) != stack.end()) return;
        std::vector<std::string> d;
        for (auto& f : nodes(r, "members"))
            if (is(f, "FieldDecl")) value_records(type(f), d);
        stack.push_back(q);
        for (auto& x : d)
            if (by.get(x)) add(by.get(x), stack);
        seen.insert(q);
        out.push_back(r);
    };
    for (auto& r : records) add(r, {});
    return out;
}

Strs record_decl(PyWriter& w, const NodeP& r) {
    Strs out{"struct " + ident(cname_of(r)) + " {"};
    for (auto& f : nodes(r, "members")) {
        if (is(f, "Comment")) {
            out.push_back("    // " + str(raw(f, "text")));
            continue;
        }
        TypeP t = type(f);
        bool opt = has_mod(f, "optional");
        std::string ct = (opt && !is_class(t, "py.optional")) ? w.ctype(mk_type("class", "py.optional", {t})) : w.ctype(t);
        const char* z = !opt ? w.zero(t) : nullptr;
        out.push_back("    " + ct + " " + ident(sname(sym(f))) + (z ? std::string(" = ") + z : "") + ";");
    }
    out.push_back("};");
    // its keys and values as the Python dict had them (an absent optional key is no key): for repr, JSON, tests
    out.push_back("template <class F> void for_each_field(const " + ident(cname_of(r)) + "& r, F&& f) {");
    for (auto& fd : nodes(r, "members")) {
        if (!is(fd, "FieldDecl")) continue;
        std::string n = ident(sname(sym(fd)));
        if (has_mod(fd, "optional"))
            out.push_back("    if (r." + n + ") f(\"" + sname(sym(fd)) + "\", *r." + n + ");");
        else
            out.push_back("    f(\"" + sname(sym(fd)) + "\", r." + n + ");");
    }
    out.push_back("}");
    // the same, writable (py::native::ask fills a record from a native format's answer): f(key, member, required)
    out.push_back("template <class F> void for_each_field_mut(" + ident(cname_of(r)) + "& r, F&& f) {");
    for (auto& fd : nodes(r, "members")) {
        if (!is(fd, "FieldDecl")) continue;
        std::string n = ident(sname(sym(fd)));
        out.push_back("    f(\"" + sname(sym(fd)) + "\", r." + n + ", " + (has_mod(fd, "optional") ? "false" : "true") + ");");
    }
    out.push_back("}");
    return out;
}

bool has_ctor(const ByQ& by, const std::string& q) {
    NodeP c = by.get(q);
    while (c) {
        for (auto& m : nodes(c, "members"))
            if (is(m, "MethodDecl") && str(raw(m, "kind")) == "ctor") return true;
        c = has_extends(c) ? by.get(extends_of(c)) : nullptr;
    }
    return false;
}

std::set<std::string> assigned(const NodeP& m) {
    std::set<std::string> out;
    for (auto& x : walk(m))
        if (is(x, "Assign")) {
            NodeP tg = node(x, "target");
            if (is(tg, "Name")) out.insert(sname(sym(tg)));
            else if (is(tg, "Call") && sowner(sym(tg)) == "py" && sname(sym(tg)) == "tie")   // a, b = ... into names
                for (auto& a : nodes(tg, "args"))
                    if (is(a, "Name")) out.insert(sname(sym(a)));
        }
    return out;
}

// a parameter's default (a constant), as C++, or empty
std::string default_of(PyWriter& w, const NodeP& p, bool& has) {
    has = false;
    for (auto& mod : mods(p)) {
        if (!starts(mod, "default=")) continue;
        V v = py::json_parse(mod.substr(8));
        TypeP t = type(p);
        if (v.is_none()) {
            if (is_class(t, "py.optional")) {
                has = true;
                return w.none_of(t);
            }
            return std::string();
        }
        has = true;
        if (v.is_bool()) return v.b ? "true" : "false";
        if (v.is_int() || v.is_float()) return w.expr(make_lit(v, t));
        if (v.is_dict() && v.has("bytes")) return cpp_bytes(str(v.get("bytes")));
        if (v.is_str()) return "py::str(" + cpp_string(v.str()) + ")";
        has = false;
    }
    return std::string();
}

bool base_has(const ByQ& by, const NodeP& c, const std::string& name) {
    NodeP b = has_extends(c) ? by.get(extends_of(c)) : nullptr;
    while (b) {
        for (auto& x : nodes(b, "members"))
            if (is(x, "MethodDecl") && str(raw(x, "name")) == name) return true;
        b = has_extends(b) ? by.get(extends_of(b)) : nullptr;
    }
    return false;
}

bool sub_has(const ByQ& by, const NodeP& c, const std::string& name) {
    for (auto& d : by.order) {
        NodeP k = d;
        while (k && has_extends(k)) {
            if (extends_of(k) == qname_of(c)) {
                for (auto& x : nodes(d, "members"))
                    if (is(x, "MethodDecl") && str(raw(x, "name")) == name) return true;
                break;
            }
            k = by.get(extends_of(k));
        }
    }
    return false;
}

std::string signature_(PyWriter& w, const NodeP& m, const NodeP& c, bool decl, const ByQ* classes);
// (a Decline names the function it was met in: the porting engine refuses that one function)
std::string decline_where(const NodeP& m) { return sowner(sym(m)) + "." + str(raw(m, "name")); }
std::string signature(PyWriter& w, const NodeP& m, const NodeP& c, bool decl, const ByQ* classes = nullptr) {
    try {
        return signature_(w, m, c, decl, classes);
    } catch (Decline& e) {
        if (e.where.empty()) e.where = decline_where(m);
        throw;
    }
}
std::string signature_(PyWriter& w, const NodeP& m, const NodeP& c, bool decl, const ByQ* classes) {
    auto as = assigned(m);
    Strs parts;
    for (auto& p : nodes(m, "params")) {
        bool has = false;
        std::string d = decl ? default_of(w, p, has) : std::string();
        parts.push_back(w.param_type(type(p), as.count(sname(sym(p))) > 0) + " " + ident(sname(sym(p))) +
                        (has && !d.empty() ? " = " + d : ""));
    }
    std::string params = join(parts, ", ");
    if (str(raw(m, "kind")) == "ctor") {
        std::string name = ident(cname_of(c));
        if (decl) return std::string(nodes(m, "params").size() == 1 ? "explicit " : "") + name + "(" + params + ")";
        return name + "::" + name + "(" + params + ")";
    }
    std::string ret = w.ctype(type(m, "ret"));
    std::string mname = ident(str(raw(m, "name")));
    if (!c) return ret + " " + mname + "(" + params + ")";
    if (!decl) return ret + " " + ident(cname_of(c)) + "::" + mname + "(" + params + ")";
    bool stat = has_mod(m, "static");
    std::string virt, override_ = has_mod(m, "override") ? " override" : "";
    if (classes && !stat) {
        if (base_has(*classes, c, str(raw(m, "name")))) override_ = " override";
        else if (sub_has(*classes, c, str(raw(m, "name")))) virt = "virtual ";
    }
    return std::string(stat ? "static " : "") + virt + ret + " " + mname + "(" + params + ")" + override_;
}

Strs definition_(PyWriter& w, const NodeP& m, const NodeP& c);
Strs definition(PyWriter& w, const NodeP& m, const NodeP& c) {
    w.class_members.clear();
    if (c)
        for (auto& x : nodes(c, "members")) {
            if (is(x, "MethodDecl")) w.class_members.insert(str(raw(x, "name")));
            else if (is(x, "FieldDecl")) w.class_members.insert(sname(sym(x)));
        }
    try {
        return definition_(w, m, c);
    } catch (Decline& e) {
        if (e.where.empty()) e.where = decline_where(m);
        throw;
    }
}
Strs definition_(PyWriter& w, const NodeP& m, const NodeP& c) {
    w.locals.clear();
    for (auto& p : nodes(m, "params")) w.locals.insert(sname(sym(p)));
    w.ret = type(m, "ret");
    std::vector<NodeP> stmts = nodes(node(m, "body"), "stmts");
    std::string init;
    if (str(raw(m, "kind")) == "ctor" && !stmts.empty() && is(stmts[0], "ExprStmt")) {
        NodeP e = node(stmts[0], "expr");
        if (is(e, "Call") && skind(sym(e)) == "ctor" && w.typearg_has(e, "base")) {
            init = " : " + w.expr(e);
            stmts.erase(stmts.begin());
        }
    }
    Strs out;
    const V& cm = raw(m, "comments");
    if (cm.is_seq())
        for (auto& text : cm.seq())
            for (auto& line : py::splitlines(py::strip(str(text)))) out.push_back("// " + py::rstrip(line));
    out.push_back(signature(w, m, c, false) + init + " {");
    Strs body;
    for (auto& s : stmts) w.stmt(s, body, "    ");
    out.insert(out.end(), body.begin(), body.end());
    out.push_back("}");
    return out;
}

Strs class_decl(PyWriter& w, const NodeP& c, const std::vector<NodeP>& classes) {
    ByQ by(classes);
    std::string base = has_extends(c) && by.get(extends_of(c)) ? extends_of(c) : std::string();
    std::string exc;
    bool has_exc = false;
    for (auto& m : mods(c))
        if (starts(m, "exception:")) {
            exc = m.substr(10);
            has_exc = true;
            break;
        }
    std::string head;
    TypeP ext = type(c, "extends");
    if (ext && is_class(ext, "py.generator")) head = "class " + ident(cname_of(c)) + " : public " + w.ctype(ext) + " {";
    else if (!base.empty()) head = "class " + ident(cname_of(c)) + " : public " + w.qual(base) + " {";
    else if (has_exc && !exc.empty()) head = "class " + ident(cname_of(c)) + " : public py::" + exc + " {";
    else head = "class " + ident(cname_of(c)) + " : public py::object {";
    Strs out{head, "public:"};
    bool any_field = false;
    for (auto& f : nodes(c, "members")) {
        if (!is(f, "FieldDecl")) continue;
        any_field = true;
        std::string stat = has_mod(f, "static") ? "static " : "";
        TypeP t = type(f);
        std::string k = tkind(t);
        std::string cst = has_mod(f, "const") && (k == "i64" || k == "bool" || k == "f64") ? "constexpr " : "";
        if (!stat.empty() && cst.empty()) stat = "inline static const ";
        NodeP fi = node(f, "init");
        std::string init = fi ? " = " + w.expr(fi, t) : "";
        if (init.empty() && stat.empty()) {
            const char* z = w.zero(t);
            init = z ? std::string(" = ") + z : "";
        }
        out.push_back("    " + stat + cst + w.ctype(t) + " " + ident(sname(sym(f))) + init + ";");
    }
    std::vector<NodeP> methods;
    for (auto& m : nodes(c, "members"))
        if (is(m, "MethodDecl")) methods.push_back(m);
    bool ctor = false;
    for (auto& m : methods) ctor = ctor || str(raw(m, "kind")) == "ctor";
    if (!ctor && !base.empty() && has_ctor(by, base)) out.push_back("    using " + w.qual(base) + "::" + ident(cname_of(by.get(base))) + ";");
    if (has_exc && !exc.empty() && !ctor) out.push_back("    using py::" + exc + "::" + exc + ";");
    if (has_exc)   // (its Python name, as an uncaught exception reports it)
        out.push_back("    const char* type() const override { return \"" + cname_of(c) + "\"; }");
    if (!methods.empty() || any_field) out.push_back("");
    for (auto& m : methods) out.push_back("    " + signature(w, m, c, true, &by) + ";");
    bool self_used = false;
    for (auto& m : methods) {
        for (auto& line : definition(w, m, c))
            if (line.find("self()") != std::string::npos) {
                self_used = true;
                break;
            }
        if (self_used) break;
    }
    if (self_used)
        out.push_back("    py::ref<" + ident(cname_of(c)) + "> self() { return std::static_pointer_cast<" + ident(cname_of(c)) +
                      ">(shared_from_this()); }");
    std::vector<NodeP> notes;
    for (auto& x : nodes(c, "members"))
        if (is(x, "Comment")) notes.push_back(x);
    if (!notes.empty()) {
        out.push_back("");
        out.push_back("    // Left in Python (the front end refused them):");
        for (auto& x : notes) out.push_back("    //   " + py::replace(str(raw(x, "text")), "not translated: ", ""));
    }
    out.push_back("};");
    return out;
}

}  // namespace

// An ir.Unit of the Python front end -> (header text, source text, {function: (first, last) source line})
// -- the lines say where each function's body is in the source, so a compiler's error can be pinned on it.
V write_unit_python(const NodeP& unit, const std::string& api_root) {
    std::string mod = str(raw(unit, "package"));
    PyWriter w(mod, api_root);
    Strs nsparts;
    for (auto& x : py::split(mod, ".")) nsparts.push_back(ident(x));
    std::string ns = join(nsparts, "::");
    std::vector<NodeP> records, classes, modules;
    for (auto& t : nodes(unit, "types")) {
        std::string k = str(raw(t, "kind"));
        if (k == "record") records.push_back(t);
        else if (k == "class") classes.push_back(t);
        else if (k == "module") modules.push_back(t);
    }
    classes = bases_first(classes);
    records = records_ordered(records);
    Strs H{"// " + header_path(mod) + " -- C++ from " + mod + " by CodeBreaker's Python front end.", "#pragma once", "",
           "#include \"py.hpp\""};
    const V& imports = raw(unit, "imports");
    if (imports.is_seq())
        for (auto& dep : imports.seq()) H.push_back("#include \"" + header_path(str(dep)) + "\"");
    H.push_back("");
    H.push_back("namespace " + ns + " {");
    H.push_back("");
    for (auto& c : classes) H.push_back("class " + ident(cname_of(c)) + ";");
    for (auto& r : records) H.push_back("struct " + ident(cname_of(r)) + ";");
    if (!classes.empty() || !records.empty()) H.push_back("");
    for (auto& r : records) {
        auto d = record_decl(w, r);
        H.insert(H.end(), d.begin(), d.end());
        H.push_back("");
    }
    Strs S{"// " + source_path(mod) + " -- C++ from " + mod + " by CodeBreaker's Python front end.", "",
           "#include \"" + header_path(mod) + "\"", "", "namespace " + ns + " {", ""};
    V spans = V::dict();
    for (auto& c : classes) {
        auto d = class_decl(w, c, classes);
        H.insert(H.end(), d.begin(), d.end());
        H.push_back("");
    }
    for (auto& m : modules) {
        std::vector<NodeP> notes;
        bool fields = false;
        for (auto& x : nodes(m, "members")) {
            if (is(x, "Comment")) notes.push_back(x);
            if (is(x, "FieldDecl")) {
                fields = true;
                std::string cst = has_mod(x, "const") ? "const " : "";
                H.push_back("extern " + cst + w.ctype(type(x)) + " " + ident(sname(sym(x))) + ";");
                S.push_back(cst + w.ctype(type(x)) + " " + ident(sname(sym(x))) + " = " + w.expr(node(x, "init"), type(x)) + ";");
            }
        }
        if (fields) {
            H.push_back("");
            S.push_back("");
        }
        for (auto& x : nodes(m, "members"))
            if (is(x, "MethodDecl")) H.push_back(signature(w, x, nullptr, true) + ";");
        if (!notes.empty()) {
            H.push_back("");
            H.push_back("// Left in Python (the front end refused them):");
            for (auto& x : notes) H.push_back("//   " + py::replace(str(raw(x, "text")), "not translated: ", ""));
        }
    }
    H.push_back("");
    H.push_back("}  // namespace " + ns);
    H.push_back("");
    auto span = [&](const std::string& key, size_t start, size_t end) {
        spans.set(V(key), V::tuple({V((long long)start), V((long long)end)}));
    };
    for (auto& c : classes)
        for (auto& x : nodes(c, "members"))
            if (is(x, "MethodDecl")) {
                size_t start = S.size() + 1;
                auto d = definition(w, x, c);
                S.insert(S.end(), d.begin(), d.end());
                span(sowner(sym(x)) + "." + str(raw(x, "name")), start, S.size());
                S.push_back("");
            }
    for (auto& m : modules)
        for (auto& x : nodes(m, "members"))
            if (is(x, "MethodDecl")) {
                size_t start = S.size() + 1;
                auto d = definition(w, x, nullptr);
                S.insert(S.end(), d.begin(), d.end());
                span(mod + "." + str(raw(x, "name")), start, S.size());
                S.push_back("");
            }
    S.push_back("}  // namespace " + ns);
    S.push_back("");
    for (size_t i = 0; i < S.size(); i++)   // a native format's answers asked (py::native::ask): its header, here only
        if (S[i].find("py::native::ask<") != std::string::npos) {
            S.insert(S.begin() + 3, "#include \"py_answers.hpp\"");
            break;
        }
    return V::tuple({V(join(H, "\n")), V(join(S, "\n")), spans});
}

V write_unit(const V& unit_doc, const std::string& api_root) {
    NodeP unit = l2::from_json(unit_doc);
    if (semantics(raw(unit, "lang")) != Lang::Python) throw Decline("a Java unit is written whole: program()");
    return write_unit_python(unit, api_root);
}

std::string cpp_name(const std::string& python_name) { return ident(python_name); }

}  // namespace cpp17
