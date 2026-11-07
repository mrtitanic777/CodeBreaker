// ext.cc -- the engine's extensions over the old front end's rules: what the old engine refused
// first and most, mapped (docs/NATIVE.md "The porting engine in C++"):
//
//   re          re.match / search / fullmatch / sub / subn / split / findall / finditer / escape /
//               compile, Pattern and Match methods. The pattern is read here: how many groups,
//               their names, and which can be left unset (inside ?, *, {0,}, an alternative, a
//               negative look-around, a conditional) -- so m.group(1) is a str where the group
//               always takes part and Optional[str] where it may not. Every call carries its
//               pattern (typeargs) for the back end's Python-compatible regex engine (native/l2's
//               pyre).
//   files       open() in every text and binary mode, read / readline / readlines / write /
//               writelines / seek / tell / close, iterating a text file's lines, `with open(...)`.
//   os ...      os.makedirs, os.path.abspath / realpath / normpath / expanduser / relpath / isabs /
//               getmtime, os.getcwd, os.remove / unlink / rmdir / rename / replace, os.environ.get,
//               os.getenv, os.walk, shutil.copyfile / copy / copy2 / move / rmtree / which,
//               time.time / perf_counter / monotonic / sleep, hashlib digests.
#include <algorithm>
#include <map>

#include "infer.hpp"
#include "pyfront/pyfront.hpp"

namespace pyengine {

using namespace pyfront;
using Args = std::vector<std::optional<T>>;
using Kw = OMap<std::optional<T>>;

// ------------------------------------------------------------------------------------ regex groups
const std::string RE_COMPUTED = std::string("\x02computed");

ReInfo re_analyze(const std::string& pat, bool verbose) {
    ReInfo info;
    info.optional.push_back(false);
    if (pat == RE_COMPUTED) {      // (new) groups 1.. may be anything, and may not have taken part: None
        info.unknown = true;
        info.groups = 99;
        info.optional.resize(100, true);
        info.optional[0] = false;
        return info;
    }
    struct Frame {
        int group = 0;               // 0: not capturing
        std::vector<int> inside;
        bool alt = false;
        bool negative = false;       // a negative look-around or a conditional: nothing inside takes part for sure
    };
    std::vector<Frame> st(1);
    auto mark = [&](const std::vector<int>& gs) {
        for (int g : gs) info.optional[g] = true;
    };
    size_t i = 0, n = pat.size();
    auto quant_optional = [&](size_t j) -> bool {
        // the quantifier after an atom ending before j: ? * {0 {, make it optional
        if (j >= n) return false;
        char c = pat[j];
        if (c == '?' || c == '*') return true;
        if (c == '{') {
            size_t k = j + 1;
            if (k < n && pat[k] == ',') return true;
            if (k < n && pat[k] == '0') {
                size_t e = k;
                while (e < n && isdigit((unsigned char)pat[e])) e++;
                bool zero = true;
                for (size_t z = k; z < e; z++)
                    if (pat[z] != '0') zero = false;
                return zero && e < n && (pat[e] == ',' || pat[e] == '}');
            }
        }
        return false;
    };
    while (i < n) {
        char c = pat[i];
        if (verbose && (c == ' ' || c == '\t' || c == '\n' || c == '\r')) {
            i++;
            continue;
        }
        if (verbose && c == '#') {
            while (i < n && pat[i] != '\n') i++;
            continue;
        }
        if (c == '\\') {
            i += 2;
            continue;
        }
        if (c == '[') {
            i++;
            if (i < n && pat[i] == '^') i++;
            if (i < n && pat[i] == ']') i++;
            while (i < n && pat[i] != ']') {
                if (pat[i] == '\\') i++;
                i++;
            }
            i++;
            continue;
        }
        if (c == '|') {
            st.back().alt = true;
            i++;
            continue;
        }
        if (c == '(') {
            Frame f;
            i++;
            if (i < n && pat[i] == '?') {
                i++;
                if (i < n && pat[i] == 'P' && i + 1 < n && pat[i + 1] == '<') {
                    size_t e = pat.find('>', i + 2);
                    if (e == std::string::npos) {
                        info.ok = false;
                        return info;
                    }
                    f.group = ++info.groups;
                    info.optional.push_back(false);
                    info.names[pat.substr(i + 2, e - i - 2)] = f.group;
                    i = e + 1;
                } else if (i < n && pat[i] == 'P' && i + 1 < n && pat[i + 1] == '=') {
                    size_t e = pat.find(')', i);
                    i = e == std::string::npos ? n : e;   // a backreference: no group
                    continue;
                } else if (i < n && pat[i] == '#') {
                    size_t e = pat.find(')', i);
                    i = e == std::string::npos ? n : e + 1;
                    continue;
                } else if (i < n && (pat[i] == '!' || (pat[i] == '<' && i + 1 < n && pat[i + 1] == '!'))) {
                    f.negative = true;
                    i += pat[i] == '<' ? 2 : 1;
                } else if (i < n && pat[i] == '(') {
                    f.negative = true;      // (?(id)yes|no)
                    size_t e = pat.find(')', i);
                    i = e == std::string::npos ? n : e + 1;
                } else {
                    // (?: (?= (?<= or inline flags (?aiLmsux) / (?aiLmsux-imsx:...)
                    size_t j = i;
                    while (j < n && isalpha((unsigned char)pat[j])) j++;
                    if (j < n && pat[j] == '-') {
                        j++;
                        while (j < n && isalpha((unsigned char)pat[j])) j++;
                    }
                    if (j < n && pat[j] == ')') {   // global inline flags
                        if (pat.substr(i, j - i).find('x') != std::string::npos) verbose = true;
                        i = j + 1;
                        continue;
                    }
                    if (j < n && pat[j] == ':') i = j + 1;
                    else if (i < n && (pat[i] == '=' || pat[i] == ':')) i++;
                    else if (i + 1 < n && pat[i] == '<' && pat[i + 1] == '=') i += 2;
                    else if (i < n && pat[i] == '>') i++;   // atomic group
                    else {
                        info.ok = false;
                        return info;
                    }
                }
            } else {
                f.group = ++info.groups;
                info.optional.push_back(false);
            }
            st.push_back(f);
            continue;
        }
        if (c == ')') {
            if (st.size() < 2) {
                info.ok = false;
                return info;
            }
            Frame f = st.back();
            st.pop_back();
            if (f.alt || f.negative) mark(f.inside);
            i++;
            std::vector<int> all = f.inside;
            if (f.group) all.push_back(f.group);
            if (f.negative && f.group) info.optional[f.group] = true;
            if (quant_optional(i)) mark(all);
            st.back().inside.insert(st.back().inside.end(), all.begin(), all.end());
            continue;
        }
        i++;
    }
    if (st.size() != 1) {
        info.ok = false;
        return info;
    }
    if (st.back().alt) mark(st.back().inside);
    return info;
}

static const std::string SEP = "\x1f";

// the types carry the pattern: "re.Pattern\x1f<verbose>\x1f<pattern>"
static T re_type(const char* what, const std::string& pat, bool verbose) {
    return named(TK::Extern, std::string(what) + SEP + (verbose ? "x" : "") + SEP + pat);
}
static bool re_split(const std::string& name, std::string& what, std::string& pat, bool& verbose) {
    auto a = name.find(SEP);
    if (a == std::string::npos) return false;
    auto b = name.find(SEP, a + 1);
    if (b == std::string::npos) return false;
    what = name.substr(0, a);
    verbose = name.substr(a + 1, b - a - 1) == "x";
    pat = name.substr(b + 1);
    return what == "re.Pattern" || what == "re.Match";
}

static bool str_const(const A::Expr* e, std::string& out) { return e && is_str_const(e, &out); }
static const A::Expr* kw_value(const A::Call* n, const char* name) {
    for (auto* k : n->keywords)
        if (k->has_arg && k->arg == name) return k->value;
    return nullptr;
}
static bool flags_verbose(const A::Expr* e) {
    // re.X / re.VERBOSE anywhere in a constant flags expression
    if (!e) return false;
    for (auto& x : walk(e))
        if (auto* at = as<Attribute>(x.expr()))
            if (at->attr == "X" || at->attr == "VERBOSE") return true;
    return false;
}
static bool is_kind(const std::optional<T>& t, std::initializer_list<TK> ks) {
    if (!t) return false;
    if (t->kind == TK::Bottom || t->kind == TK::Top) return true;
    for (TK k : ks)
        if (t->kind == k) return true;
    return false;
}

static T group_type(const ReInfo& ri, int g) { return (g > 0 && g < (int)ri.optional.size() && ri.optional[g]) ? opt(STR()) : STR(); }

// the result of the re call `fn` with pattern `pat` (the module's functions and a Pattern's methods)
static TR re_result(Eval& ev, const A::Call* n, const std::string& fn, const std::string& pat, bool verbose, const Args& rest,
                    const Kw& kw, const std::string& resname, const State& state) {
    ReInfo ri = re_analyze(pat, verbose);
    if (!ri.ok) return ev.issue(n, "stdlib", fn + " of a pattern the engine cannot read");
    Res res(resname.rfind("re.", 0) == 0 && resname.find('.', 3) == std::string::npos ? "std" : "bmethod", resname, pat);
    auto need_str = [&](size_t i) { return i < rest.size() && is_kind(rest[i], {TK::Str}); };
    if (fn == "match" || fn == "search" || fn == "fullmatch") {
        if (!need_str(0)) return ev.issue(n, "conflict", "re." + fn + " of a " + (rest.empty() || !rest[0] ? std::string("?") : kind_name(rest[0]->kind)));
        return TR(opt(re_type("re.Match", pat, verbose)), res);
    }
    if (fn == "finditer") {
        if (!need_str(0)) return ev.issue(n, "conflict", "re.finditer arguments");
        return TR(iter_of(re_type("re.Match", pat, verbose), MEM), res);
    }
    if (fn == "sub" || fn == "subn") {
        if (rest.size() < 2) return ev.issue(n, "conflict", "re." + fn + " arity");
        if (!rest[0] || !is_kind(rest[0], {TK::Str})) {
            // (new) the replacement a lambda of the match: its result must be a str (re.sub(p, lambda m: ..., s))
            bool module_fn = res.k == "std";
            size_t at = module_fn ? 1 : 0;
            const A::Expr* repl = at < n->args.size() ? n->args[at] : nullptr;
            auto* lam = repl ? as<pyfront::Lambda>(repl) : nullptr;
            if (!lam || lam->args->args.size() != 1) return ev.issue(n, "dynamic", "re." + fn + " with a function");
            T r = ev.lambda_type(repl, {re_type("re.Match", pat, verbose)}, state);
            if (r.kind != TK::Str && r.kind != TK::Bottom) return ev.issue(n, "conflict", "re." + fn + " with a function of a " + kind_name(r.kind));
        }
        if (!need_str(1)) return ev.issue(n, "conflict", "re." + fn + " arguments");
        if (fn == "subn") return TR(tuple_of({STR(), SIZE()}), res);
        return TR(STR(), res);
    }
    if (fn == "split") {
        if (!need_str(0)) return ev.issue(n, "conflict", "re.split arguments");
        int s = ev.p.new_site(n, TK::List, ev.where(n));
        bool any_opt = false;
        for (int g = 1; g <= ri.groups; g++) any_opt = any_opt || ri.optional[g];
        ev.p.grow(ev.p.elem, s, any_opt ? opt(STR()) : STR());
        return TR(container(TK::List, s), res);
    }
    if (fn == "findall") {
        if (!need_str(0)) return ev.issue(n, "conflict", "re.findall arguments");
        if (ri.unknown) return ev.issue(n, "dynamic", "re.findall of a computed pattern (str or tuple items)");
        int s = ev.p.new_site(n, TK::List, ev.where(n));
        if (ri.groups <= 1) ev.p.grow(ev.p.elem, s, STR());
        else ev.p.grow(ev.p.elem, s, tuple_of(std::vector<T>(ri.groups, STR())));
        return TR(container(TK::List, s), res);
    }
    (void)kw;
    (void)state;
    return ev.issue(n, "stdlib", "re." + fn);
}

static TR re_match_method(Eval& ev, const A::Call* n, const std::string& attr, const std::string& pat, bool verbose, const Args& args) {
    ReInfo ri = re_analyze(pat, verbose);
    Res res("bmethod", "re.Match." + attr, pat);
    auto group_of = [&](const A::Expr* a, int& g) -> bool {
        Big b;
        std::string nm;
        if (int_const(a, &b) && b.fits_i64() && b.i64() >= 0 && b.i64() <= ri.groups) {
            g = (int)b.i64();
            return true;
        }
        if (str_const(a, nm) && ri.names.count(nm)) {
            g = ri.names[nm];
            return true;
        }
        return false;
    };
    if (attr == "group") {
        if (n->args.empty()) return TR(STR(), res);
        std::vector<T> ts;
        for (auto* a : n->args) {
            int g;
            if (!group_of(a, g)) return ev.issue(n, "dynamic", "Match.group of a computed group");
            ts.push_back(group_type(ri, g));
        }
        return TR(ts.size() == 1 ? ts[0] : tuple_of(ts), res);
    }
    if (attr == "groups") {
        if (!n->args.empty()) return ev.issue(n, "syntax", "Match.groups(default)");
        if (ri.unknown) return ev.issue(n, "dynamic", "Match.groups of a computed pattern");
        std::vector<T> ts;
        for (int g = 1; g <= ri.groups; g++) ts.push_back(group_type(ri, g));
        return TR(tuple_of(ts), res);
    }
    if (attr == "start" || attr == "end") {
        int g = 0;
        if (!n->args.empty() && !group_of(n->args[0], g)) return ev.issue(n, "dynamic", "Match." + attr + " of a computed group");
        return TR(ri.optional[g] ? FIND() : SIZE(), res);
    }
    if (attr == "span") {
        int g = 0;
        if (!n->args.empty() && !group_of(n->args[0], g)) return ev.issue(n, "dynamic", "Match.span of a computed group");
        T b = ri.optional[g] ? FIND() : SIZE();
        return TR(tuple_of({b, b}), res);
    }
    (void)args;
    return ev.issue(n, "stdlib", "re.Match." + attr);
}

// ------------------------------------------------------------------------------------ files
static std::string file_mode(const std::string& name) { return name.size() > 5 ? name.substr(5) : "rb"; }   // "file:<mode>"; "file" is rb
static bool binary_mode(const std::string& m) { return m.find('b') != std::string::npos; }

static TR file_method(Eval& ev, const A::Call* n, const T& recv, const std::string& attr, const Args& args) {
    std::string mode = file_mode(recv.name());
    bool bin = binary_mode(mode);
    bool reading = mode.find('r') != std::string::npos || mode.find('+') != std::string::npos;
    bool writing = mode.find('w') != std::string::npos || mode.find('a') != std::string::npos || mode.find('+') != std::string::npos;
    Res res("bmethod", "file." + attr);
    T content = bin ? BYTES() : STR();
    if (attr == "read" && reading) {
        if (args.size() > 1 || (args.size() == 1 && !is_kind(args[0], {TK::Int, TK::Bool}))) return ev.issue(n, "conflict", "file.read arguments");
        return TR(content, res);
    }
    if (attr == "readline" && reading) return TR(content, res);
    if (attr == "readlines" && reading) {
        int s = ev.p.new_site(n, TK::List, ev.where(n));
        ev.p.grow(ev.p.elem, s, content);
        return TR(container(TK::List, s), res);
    }
    if (attr == "write" && writing) {
        if (args.size() != 1 || !is_kind(args[0], bin ? std::initializer_list<TK>{TK::Bytes, TK::Bytearray} : std::initializer_list<TK>{TK::Str}))
            return ev.issue(n, "conflict", std::string("file.write of a ") + (args.size() == 1 && args[0] ? kind_name(args[0]->kind) : "?"));
        return TR(SIZE(), res);
    }
    if (attr == "writelines" && writing) {
        if (args.size() != 1 || !args[0]) return ev.issue(n, "conflict", "file.writelines arguments");
        return TR(NONE(), res);
    }
    if (attr == "close" || attr == "flush") return TR(NONE(), res);
    if (attr == "seek") {
        if (args.empty() || args.size() > 2) return ev.issue(n, "conflict", "file.seek arity");
        return TR(SIZE(), res);
    }
    if (attr == "tell") return TR(SIZE(), res);
    return ev.issue(n, "stdlib", "file." + attr);
}

// ------------------------------------------------------------------------------------ hooks
static const std::set<std::string>& simple_std_str() {
    static const std::set<std::string> s = {"os.path.abspath", "os.path.realpath", "os.path.normpath", "os.path.expanduser",
                                            "os.path.expandvars", "os.getcwd", "os.path.commonpath", "re.escape"};
    return s;
}

TR Eval::ext_std(const A::Call* n, const std::string& name, const Args& args, const Kw& kw, const State& state, bool& handled) {
    handled = true;
    auto strs = [&](size_t lo, size_t hi) {
        if (args.size() < lo || args.size() > hi) return false;
        for (auto& a : args)
            if (!is_kind(a, {TK::Str})) return false;
        return true;
    };
    // open() in the modes the old engine refused
    if (name == "builtins.open") {
        std::string mode = "r";
        const A::Expr* me = n->args.size() > 1 ? n->args[1] : kw_value(n, "mode");
        if (me && !str_const(me, mode)) return issue(n, "dynamic", "open with a computed mode");
        std::string m2 = mode;
        m2.erase(std::remove(m2.begin(), m2.end(), 't'), m2.end());
        static const std::set<std::string> OK = {"r", "w", "a", "x", "rb", "wb", "ab", "xb", "r+", "r+b", "rb+", "w+", "w+b", "wb+"};
        if (!OK.count(m2)) return issue(n, "stdlib", "open(" + py_repr(mode) + ")");
        for (auto& [k, _] : kw)
            if (k != "mode" && k != "encoding" && k != "errors" && k != "newline") return issue(n, "syntax", "open(" + k + "=)");
        if (args.empty() || !is_kind(args[0], {TK::Str})) return issue(n, "conflict", "open of a non-path");
        if (binary_mode(m2) && (kw.has("encoding") || kw.has("errors"))) return issue(n, "conflict", "open in binary mode with an encoding");
        return TR(named(TK::Extern, "file:" + m2), Res("builtin", std::string("open")));
    }
    if (name.rfind("re.", 0) == 0) {
        std::string fn = name.substr(3);
        if (fn == "escape") {
            if (!strs(1, 1)) return issue(n, "conflict", "re.escape arguments");
            return TR(STR(), Res("std", name));
        }
        if (fn == "compile") {
            std::string pat;
            if (n->args.empty()) return issue(n, "conflict", "re.compile arity");
            if (!str_const(n->args[0], pat)) {
                // (new) a pattern computed at run time: compiled there (py::re reads it then, re.error if it cannot)
                if (args.empty() || !args[0] || args[0]->kind != TK::Str) return issue(n, "dynamic", "re.compile of a computed pattern");
                pat = RE_COMPUTED;
            }
            bool verbose = flags_verbose(n->args.size() > 1 ? n->args[1] : kw_value(n, "flags"));
            if (!re_analyze(pat, verbose).ok) return issue(n, "stdlib", "re.compile of a pattern the engine cannot read");
            if (pat == RE_COMPUTED) return TR(re_type("re.Pattern", pat, verbose), Res("std", name));
            return TR(re_type("re.Pattern", pat, verbose), Res("std", name, pat));
        }
        static const std::set<std::string> FNS = {"match", "search", "fullmatch", "sub", "subn", "split", "findall", "finditer"};
        if (!FNS.count(fn)) {
            handled = false;
            return BOTTOM();
        }
        // the pattern: a constant, or a compiled Pattern
        std::string pat;
        bool verbose = false;
        std::string what;
        if (n->args.empty()) return issue(n, "conflict", "re." + fn + " arity");
        if (!str_const(n->args[0], pat)) {
            if (!args.empty() && args[0] && args[0]->kind == TK::Extern && re_split(args[0]->name(), what, pat, verbose) && what == "re.Pattern") {
            } else if (!args.empty() && args[0] && args[0]->kind == TK::Str) {   // (new) a str computed at run time
                pat = RE_COMPUTED;
                size_t flag_at = (fn == "sub" || fn == "subn") ? 4 : (fn == "split" ? 3 : 2);
                verbose = flags_verbose(n->args.size() > flag_at ? n->args[flag_at] : kw_value(n, "flags"));
            } else
                return issue(n, "dynamic", "re." + fn + " of a computed pattern");
        } else {
            size_t flag_at = (fn == "sub" || fn == "subn") ? 4 : (fn == "split" ? 3 : 2);
            verbose = flags_verbose(n->args.size() > flag_at ? n->args[flag_at] : kw_value(n, "flags"));
        }
        for (auto& [k, _] : kw)
            if (k != "flags" && k != "count" && k != "maxsplit") return issue(n, "syntax", "re." + fn + "(" + k + "=)");
        Args rest(args.begin() + 1, args.end());
        return re_result(*this, n, fn, pat, verbose, rest, kw, name, state);
    }
    if (name == "os.makedirs") {
        if (args.empty() || !is_kind(args[0], {TK::Str})) return issue(n, "conflict", "os.makedirs arguments");
        for (auto& [k, _] : kw)
            if (k != "exist_ok" && k != "mode") return issue(n, "syntax", "os.makedirs(" + k + "=)");
        return TR(NONE(), Res("std", name));
    }
    if (simple_std_str().count(name)) {
        if (name == "os.getcwd" ? !args.empty() : !strs(1, name == "os.path.commonpath" ? 1 : 1)) return issue(n, "conflict", name + " arguments");
        if (!kw.empty()) return issue(n, "syntax", name + " keywords");
        return TR(STR(), Res("std", name));
    }
    if (name == "os.path.relpath") {
        if (!strs(1, 2) || !kw.empty()) return issue(n, "conflict", name + " arguments");
        return TR(STR(), Res("std", name));
    }
    if (name == "os.path.isabs" || name == "os.path.islink") {
        if (!strs(1, 1)) return issue(n, "conflict", name + " arguments");
        return TR(BOOL(), Res("std", name));
    }
    if (name == "os.path.getmtime") {
        if (!strs(1, 1)) return issue(n, "conflict", name + " arguments");
        return TR(FLOAT(), Res("std", name));
    }
    if (name == "os.remove" || name == "os.unlink" || name == "os.rmdir" || name == "os.removedirs" || name == "os.chdir") {
        if (!strs(1, 1) || !kw.empty()) return issue(n, "conflict", name + " arguments");
        return TR(NONE(), Res("std", name));
    }
    if (name == "os.rename" || name == "os.replace") {
        if (!strs(2, 2) || !kw.empty()) return issue(n, "conflict", name + " arguments");
        return TR(NONE(), Res("std", name));
    }
    if (name == "os.environ.get" || name == "os.getenv") {
        if (args.empty() || args.size() > 2 || !is_kind(args[0], {TK::Str}) || !kw.empty()) return issue(n, "conflict", name + " arguments");
        if (args.size() == 2) {
            if (!args[1]) return issue(n, "conflict", name + " default");
            T j = p.join(opt(STR()), *args[1]);
            T r = args[1]->kind == TK::Str ? STR() : j;
            if (r.kind == TK::Top) return issue(n, "conflict", name + " default: " + r.why());
            return TR(r, Res("std", name));
        }
        return TR(opt(STR()), Res("std", name));
    }
    if (name == "os.walk") {
        if (args.empty() || !is_kind(args[0], {TK::Str}) || args.size() > 1 || !kw.empty()) return issue(n, "conflict", "os.walk arguments");
        const char* base = reinterpret_cast<const char*>(n);
        int d = p.new_site(base + 1, TK::List, where(n));
        int f = p.new_site(base + 2, TK::List, where(n));
        p.grow(p.elem, d, STR());
        p.grow(p.elem, f, STR());
        return TR(iter_of(tuple_of({STR(), container(TK::List, d), container(TK::List, f)}), MEM), Res("std", name));
    }
    if (name == "shutil.copyfile" || name == "shutil.copy" || name == "shutil.copy2" || name == "shutil.move") {
        if (!strs(2, 2) || !kw.empty()) return issue(n, "conflict", name + " arguments");
        return TR(STR(), Res("std", name));
    }
    if (name == "shutil.rmtree") {
        if (args.size() != 1 || !is_kind(args[0], {TK::Str})) return issue(n, "conflict", name + " arguments");
        for (auto& [k, _] : kw)
            if (k != "ignore_errors") return issue(n, "syntax", name + "(" + k + "=)");
        return TR(NONE(), Res("std", name));
    }
    if (name == "shutil.which") {
        if (!strs(1, 1) || !kw.empty()) return issue(n, "conflict", name + " arguments");
        return TR(opt(STR()), Res("std", name));
    }
    if (name == "time.time" || name == "time.perf_counter" || name == "time.monotonic") {
        if (!args.empty()) return issue(n, "conflict", name + " arguments");
        return TR(FLOAT(), Res("std", name));
    }
    if (name == "time.sleep") {
        if (args.size() != 1 || !is_kind(args[0], {TK::Int, TK::Float})) return issue(n, "conflict", name + " arguments");
        return TR(NONE(), Res("std", name));
    }
    if (name == "hashlib.md5" || name == "hashlib.sha1" || name == "hashlib.sha256" || name == "hashlib.sha512") {
        if (args.size() > 1 || (args.size() == 1 && !is_kind(args[0], {TK::Bytes, TK::Bytearray})) || !kw.empty())
            return issue(n, "conflict", name + " arguments");
        return TR(named(TK::Extern, "hash"), Res("std", name));
    }
    handled = false;
    return BOTTOM();
}

TR Eval::ext_match_index(const A::Subscript* n, const std::string& pat, bool verbose) {
    ReInfo ri = re_analyze(pat, verbose);
    Big b;
    std::string nm;
    int g = -1;
    if (int_const(n->slice, &b) && b.fits_i64() && b.i64() >= 0 && b.i64() <= ri.groups) g = (int)b.i64();
    else if (str_const(n->slice, nm) && ri.names.count(nm)) g = ri.names[nm];
    if (g < 0) return issue(n, "dynamic", "a match indexed by a computed group");
    return TR(group_type(ri, g), Res("re_group", std::string("re.Match.group"), pat));
}

TR Eval::ext_method(const A::Call* n, const T& recv, const std::string& attr, const Args& args, const Kw& kw, const State& state,
                    bool& handled) {
    handled = true;
    if (recv.kind == TK::Extern) {
        const std::string& nm = recv.name();
        if (nm == "file" || nm.rfind("file:", 0) == 0) {
            if (!kw.empty()) return issue(n, "syntax", "file." + attr + " keywords");
            return file_method(*this, n, recv, attr, args);
        }
        if (nm == "hash") {
            if (attr == "hexdigest") return TR(STR(), Res("bmethod", std::string("hash.hexdigest")));
            if (attr == "digest") return TR(BYTES(), Res("bmethod", std::string("hash.digest")));
            if (attr == "update") {
                if (args.size() != 1 || !is_kind(args[0], {TK::Bytes, TK::Bytearray})) return issue(n, "conflict", "hash.update arguments");
                return TR(NONE(), Res("bmethod", std::string("hash.update")));
            }
            return issue(n, "stdlib", "hashlib." + attr);
        }
        std::string what, pat;
        bool verbose;
        if (re_split(nm, what, pat, verbose)) {
            if (what == "re.Pattern") {
                static const std::set<std::string> FNS = {"match", "search", "fullmatch", "sub", "subn", "split", "findall", "finditer"};
                if (!FNS.count(attr)) return issue(n, "stdlib", "re.Pattern." + attr);
                for (auto& [k, _] : kw)
                    if (k != "count" && k != "maxsplit" && k != "pos") return issue(n, "syntax", "re.Pattern." + attr + "(" + k + "=)");
                // (py::re's findall and finditer take the string alone: pos and endpos are match/search/fullmatch's)
                if ((attr == "findall" || attr == "finditer") && (args.size() > 1 || kw.has("pos")))
                    return issue(n, "stdlib", "re.Pattern." + attr + " with pos/endpos");
                return re_result(*this, n, attr, pat, verbose, args, kw, "re.Pattern." + attr, state);
            }
            if (!kw.empty()) return issue(n, "syntax", "re.Match." + attr + " keywords");
            return re_match_method(*this, n, attr, pat, verbose, args);
        }
    }
    handled = false;
    return BOTTOM();
}

}  // namespace pyengine
