// methods.cc -- the types of Python's builtins, the methods of its builtin types and the standard
// library calls the engine maps (methods.py and structfmt.py, rule for rule). A call not typed here
// is refused with its name (stdlib:<name>), so the coverage ledger lists exactly what is missing.
#include <algorithm>
#include <map>

#include "infer.hpp"
#include "pyfront/pyfront.hpp"

namespace pyengine {

using namespace pyfront;
using Args = std::vector<std::optional<T>>;
using Kw = OMap<std::optional<T>>;

// ------------------------------------------------------------------------------------ structfmt
struct FmtItem {
    char code;
    int64_t size;
    std::string kind;   // pad bytes bool float int
    Big lo, hi;
};

static bool struct_parse(const std::string& fmt0, std::vector<FmtItem>& items) {
    std::string fmt = fmt0;
    char order = '@';
    if (!fmt.empty() && std::string("<>!=@").find(fmt[0]) != std::string::npos) {
        order = fmt[0];
        fmt = fmt.substr(1);
    }
    static const std::map<char, std::pair<int, bool>> INTS = {{'b', {1, true}}, {'B', {1, false}}, {'h', {2, true}}, {'H', {2, false}},
                                                              {'i', {4, true}}, {'I', {4, false}}, {'l', {4, true}}, {'L', {4, false}},
                                                              {'q', {8, true}}, {'Q', {8, false}}};
    // re.findall(r'\s*(\d*)\s*([a-zA-Z?])') and the leftover check re.sub(r'[\s\d]|[a-zA-Z?]', '', fmt)
    for (char c : fmt)
        if (!(isspace((unsigned char)c) || isdigit((unsigned char)c) || isalpha((unsigned char)c) || c == '?')) return false;
    size_t i = 0;
    while (i < fmt.size()) {
        size_t j = i;
        while (j < fmt.size() && isspace((unsigned char)fmt[j])) j++;
        size_t ds = j;
        while (j < fmt.size() && isdigit((unsigned char)fmt[j])) j++;
        std::string count = fmt.substr(ds, j - ds);
        while (j < fmt.size() && isspace((unsigned char)fmt[j])) j++;
        if (j >= fmt.size()) break;
        char code = fmt[j];
        if (!(isalpha((unsigned char)code) || code == '?')) {
            i = j + 1;
            continue;
        }
        i = j + 1;
        int64_t n = count.empty() ? 1 : std::stoll(count);
        if (code == 'x') items.push_back({'x', n, "pad", Big(0), Big(0)});
        else if (code == 's') items.push_back({'s', n, "bytes", Big(n), Big(n)});
        else if (code == 'c')
            for (int64_t k = 0; k < n; k++) items.push_back({'c', 1, "bytes", Big(1), Big(1)});
        else if (code == '?')
            for (int64_t k = 0; k < n; k++) items.push_back({'?', 1, "bool", Big(0), Big(1)});
        else if (code == 'f' || code == 'd')
            for (int64_t k = 0; k < n; k++) items.push_back({code, code == 'f' ? 4 : 8, "float", Big(0), Big(0)});
        else if (INTS.count(code)) {
            auto [size, sg] = INTS.at(code);
            Big lo = sg ? -Big::pow2(8 * size - 1) : Big(0);
            Big hi = sg ? Big::pow2(8 * size - 1) - Big(1) : Big::pow2(8 * size) - Big(1);
            for (int64_t k = 0; k < n; k++) items.push_back({code, size, "int", lo, hi});
        } else
            return false;
    }
    if (order == '@') {
        bool any_wide = false, any_other = false;
        for (auto& it : items) {
            if (std::string("hHiIlLqQfd").find(it.code) != std::string::npos) any_wide = true;
            if (std::string("xcbB?s").find(it.code) == std::string::npos) any_other = true;
        }
        if (any_wide && any_other && items.size() > 1) return false;
    }
    return true;
}

// ------------------------------------------------------------------------------------ helpers
static std::string str_arg(const A::Expr* e, bool* ok = nullptr) {
    std::string s;
    bool b = e && is_str_const(e, &s);
    if (ok) *ok = b;
    return s;
}
static std::string repr_opt(const std::optional<std::string>& s) { return s ? py_repr(*s) : "None"; }
static const A::Expr* kw_value(const A::Call* n, const char* name) {
    for (auto* k : n->keywords)
        if (k->has_arg && k->arg == name) return k->value;
    return nullptr;
}
static bool kind_ok(const std::optional<T>& t, const std::string& want0) {
    if (!t) return false;
    if (t->kind == TK::Bottom || t->kind == TK::Top) return true;
    std::string k = kind_name(t->kind);
    if (k == "bool") k = "int";
    std::string want = want0;
    while (!want.empty() && want.back() == '?') want.pop_back();
    size_t pos = 0;
    while (pos <= want.size()) {
        size_t bar = want.find('|', pos);
        std::string w = want.substr(pos, bar == std::string::npos ? std::string::npos : bar - pos);
        if (w == k || (w == "bytes" && k == "bytearray")) return true;
        if (bar == std::string::npos) break;
        pos = bar + 1;
    }
    return false;
}

struct Simple {
    std::vector<std::string> sig;
    std::function<T()> res;
};
static const std::map<std::string, Simple>& simple_table() {
    static const std::map<std::string, Simple> t = [] {
        std::map<std::string, Simple> m;
        auto S = [] { return STR(); };
        auto B = [] { return BOOL(); };
        auto BY = [] { return BYTES(); };
        auto FI = [] { return FIND(); };
        auto SZ = [] { return SIZE(); };
        for (const char* n : {"str.upper", "str.lower", "str.title", "str.capitalize"}) m[n] = {{}, S};
        for (const char* n : {"str.strip", "str.lstrip", "str.rstrip"}) m[n] = {{"str?"}, S};
        m["str.startswith"] = {{"str|tuple"}, B};
        m["str.endswith"] = {{"str|tuple"}, B};
        m["str.find"] = {{"str", "int?", "int?"}, FI};
        m["str.rfind"] = {{"str", "int?", "int?"}, FI};
        m["str.index"] = {{"str"}, SZ};
        m["str.rindex"] = {{"str"}, SZ};
        m["str.count"] = {{"str"}, SZ};
        m["str.replace"] = {{"str", "str"}, S};
        for (const char* n : {"str.isdigit", "str.isalpha", "str.isalnum", "str.isspace", "str.isupper", "str.islower", "str.isidentifier",
                              "str.isprintable", "str.isascii"})
            m[n] = {{}, B};
        m["str.ljust"] = {{"int", "str?"}, S};
        m["str.rjust"] = {{"int", "str?"}, S};
        m["str.center"] = {{"int", "str?"}, S};
        m["str.zfill"] = {{"int"}, S};
        m["str.removeprefix"] = {{"str"}, S};
        m["str.removesuffix"] = {{"str"}, S};
        m["str.encode"] = {{"str?", "str?"}, BY};
        m["bytes.upper"] = {{}, BY};
        m["bytes.lower"] = {{}, BY};
        for (const char* n : {"bytes.strip", "bytes.lstrip", "bytes.rstrip"}) m[n] = {{"bytes?"}, BY};
        m["bytes.startswith"] = {{"bytes|tuple"}, B};
        m["bytes.endswith"] = {{"bytes|tuple"}, B};
        m["bytes.find"] = {{"bytes|int", "int?", "int?"}, FI};
        m["bytes.rfind"] = {{"bytes|int", "int?", "int?"}, FI};
        m["bytes.index"] = {{"bytes|int", "int?", "int?"}, SZ};
        m["bytes.count"] = {{"bytes|int"}, SZ};
        m["bytes.replace"] = {{"bytes", "bytes"}, BY};
        for (const char* n : {"bytes.isalnum", "bytes.isalpha", "bytes.isdigit", "bytes.isspace", "bytes.isupper", "bytes.islower"}) m[n] = {{}, B};
        m["bytes.hex"] = {{}, S};
        m["bytes.ljust"] = {{"int", "bytes?"}, BY};
        m["bytes.rjust"] = {{"int", "bytes?"}, BY};
        m["bytes.decode"] = {{"str?", "str?"}, S};
        return m;
    }();
    return t;
}

static const std::set<std::string> ENCODINGS = {"latin1", "latin-1", "iso-8859-1", "ascii", "utf-8", "utf8", "cp437", "cp1252"};
static const std::set<std::string> ERRORS = {"strict", "replace", "ignore"};

static std::string lower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

static T simple(Eval& ev, const A::Call* n, const std::string& name, const Args& args, const Kw& kw) {
    const Simple& sp = simple_table().at(name);
    if (!kw.empty()) {
        bool ok = name == "bytes.decode" || name == "str.encode";
        for (auto& [k, _] : kw)
            if (k != "errors" && k != "encoding") ok = false;
        if (!ok) return ev.issue(n, "syntax", "keyword argument to " + name);
    }
    if (args.size() > sp.sig.size()) return ev.issue(n, "conflict", "too many arguments to " + name);
    for (size_t i = 0; i < sp.sig.size(); i++) {
        const std::string& w = sp.sig[i];
        if (i >= args.size()) {
            if (w.back() != '?') return ev.issue(n, "conflict", "missing argument to " + name);
            continue;
        }
        if (!kind_ok(args[i], w)) return ev.issue(n, "conflict", name + " of a " + (args[i] ? kind_name(args[i]->kind) : "lambda"));
    }
    if (name == "bytes.decode" || name == "str.encode") {
        std::vector<std::optional<std::string>> consts;
        for (auto* a : n->args) {
            bool ok;
            std::string s = str_arg(a, &ok);
            consts.push_back(ok ? std::optional<std::string>(s) : std::nullopt);
        }
        bool enc_kw = false;
        for (auto* k : n->keywords) {
            bool ok;
            std::string s = str_arg(k->value, &ok);
            consts.push_back(ok ? std::optional<std::string>(s) : std::nullopt);
            if (k->has_arg && k->arg == "encoding") enc_kw = true;
        }
        std::optional<std::string> enc = (!n->args.empty() || enc_kw) ? consts[0] : std::optional<std::string>("utf-8");
        if (!enc || !ENCODINGS.count(lower(*enc))) return ev.issue(n, "stdlib", name + "(" + repr_opt(enc) + ")");
        std::vector<std::optional<std::string>> errs(consts.begin() + (consts.empty() ? 0 : 1), consts.end());
        bool bad = false;
        for (auto& e : errs)
            if (!e || !ERRORS.count(*e)) bad = true;
        if (bad) {
            std::string r = "[";
            for (size_t i = 0; i < errs.size(); i++) r += (i ? ", " : "") + repr_opt(errs[i]);
            return ev.issue(n, "stdlib", name + " errors=" + r + "]");
        }
    }
    auto ends = [&](const char* e) { std::string x = e; return name.size() >= x.size() && name.compare(name.size() - x.size(), x.size(), x) == 0; };
    bool sw = ends(".startswith") || ends(".endswith");
    if (sw && !args.empty() && args[0] && args[0]->kind == TK::Tuple) {
        std::string want = name.substr(0, name.find('.'));
        for (auto& a : args[0]->args())
            if (!kind_ok(a, want)) return ev.issue(n, "conflict", name + " of a tuple holding " + kind_name(a.kind));
    }
    return sp.res();
}

static T elem_of(Eval& ev, const T& t) { return ev.p.elem[ev.p.find(t.site)]; }

static const std::set<std::string> LAZY_OK = {"join", "extend", "update", "sort"};
static const std::set<std::string> LAZY_BUILTINS = {"sorted", "min", "max", "sum", "any", "all", "list", "set", "enumerate", "bytes", "bytearray", "frozenset"};

static bool lazy_bad(Eval& ev, const A::Call* n, const Args& args, const Kw& kw, bool ok, T& out) {
    if (ok) return false;
    bool any = false;
    for (auto& a : args)
        if (!a) any = true;
    for (auto& [_, v] : kw)
        if (!v) any = true;
    if (any) {
        out = ev.issue(n, "dynamic", "a lambda or generator as an argument");
        return true;
    }
    return false;
}

static TR sort_key(Eval& ev, const A::Call* n, const T& e, const Kw& kw, const State& state, const std::string& name, const T& result) {
    for (auto& [k, _] : kw)
        if (k != "key" && k != "reverse") return ev.issue(n, "syntax", name + "(" + k + "=)");
    const A::Expr* key = kw_value(n, "key");
    T kt = e;
    if (key) kt = ev.lambda_type(key, {e}, state);
    TK k = kt.kind;
    if (!(k == TK::Int || k == TK::Str || k == TK::Float || k == TK::Bytes || k == TK::Tuple || k == TK::Bottom || k == TK::Top || k == TK::Bool))
        return ev.issue(n, "conflict", std::string("sorting by a ") + kind_name(k));
    return name == "sorted" ? TR(result, Res("builtin", std::string("sorted"))) : TR(result, Res("bmethod", name));
}

static TR from_bytes(Eval& ev, const A::Call* n, const Args& args, const Kw& kw) {
    if (args.size() != 1 && args.size() != 2) return ev.issue(n, "conflict", "int.from_bytes arity");
    const std::optional<T>& b = args[0];
    bool ok;
    std::string order;
    if (n->args.size() > 1) order = str_arg(n->args[1], &ok);
    else order = str_arg(kw_value(n, "byteorder"), &ok);
    if (n->args.size() < 2 && !kw.has("byteorder")) {
        order = "big";
        ok = true;
    }
    if (!ok || (order != "little" && order != "big")) return ev.issue(n, "dynamic", "int.from_bytes byte order");
    const A::Expr* signed_ = kw_value(n, "signed");
    bool sg = false;
    if (signed_) {
        auto* c = as<Constant>(signed_);
        if (!c || (c->value.kind != ConstValue::Kind::True && c->value.kind != ConstValue::Kind::False))
            return ev.issue(n, "dynamic", "int.from_bytes signed=<computed>");
        sg = c->value.kind == ConstValue::Kind::True;
    }
    if (!b || !(b->kind == TK::Bytes || b->kind == TK::Bytearray || b->kind == TK::Bottom || b->kind == TK::Top))
        return ev.issue(n, "conflict", std::string("int.from_bytes of a ") + (b ? kind_name(b->kind) : "?"));
    if (b->kind == TK::Bottom || b->kind == TK::Top) return *b;
    if (!b->hi()) return TR(INT(sg ? OBig() : OBig(Big(0)), {}), Res("std", std::string("int.from_bytes")));
    Big bits = Big(8) * *b->hi();
    int ib = bits > Big(100000) ? 100000 : (int)bits.i64();
    return TR((sg && ib) ? s(ib) : u(ib), Res("std", std::string("int.from_bytes")));
}

// ------------------------------------------------------------------------------------ builtin types' methods
TR Eval::bmethod(const A::Call* n, const T& recv, const std::string& attr, const Args& args, const Kw& kw, const State& state) {
    T bad;
    if (lazy_bad(*this, n, args, kw, LAZY_OK.count(attr) != 0, bad)) return bad;
    std::string k = kind_name(recv.kind);
    if (k == "bool") k = "int";
    std::string name = ((k == "bytearray" && simple_table().count("bytes." + attr)) ? std::string("bytes") : k) + "." + attr;
    if (simple_table().count(name)) {
        T t = simple(*this, n, name, args, kw);
        if (k == "bytearray" && t.kind == TK::Bytes && attr != "decode" && attr != "hex") t = T(TK::Bytearray);
        return TR(t, Res("bmethod", name));
    }
    if (p.extended) {
        bool handled = false;
        TR r = ext_method(n, recv, attr, args, kw, state, handled);
        if (handled) return r;
    }
    if (k == "str") {
        if (attr == "split") {
            if (args.size() > 2 || (!args.empty() && !kind_ok(args[0], "str|none")) || !kw.empty())
                return issue(n, "syntax", "str." + attr + " arguments");
            int s = p.new_site(n, TK::List, where(n));
            p.grow(p.elem, s, STR());
            return TR(container(TK::List, s), Res("bmethod", "str." + attr));
        }
        if (attr == "splitlines") {
            int s = p.new_site(n, TK::List, where(n));
            p.grow(p.elem, s, STR());
            return TR(container(TK::List, s), Res("bmethod", std::string("str.splitlines")));
        }
        if (attr == "join") {
            T e;
            const std::optional<T> a = args.empty() ? std::nullopt : args[0];
            if (!a && !n->args.empty() && n->args[0]->kind == ExprKind::GeneratorExp) e = gen_elem(cast<GeneratorExp>(n->args[0]), state).first;
            else if (a) e = iteration(n->args[0], *a, state).first;
            else return issue(n, "conflict", "str.join of nothing");
            if (e.kind != TK::Str && e.kind != TK::Bottom && e.kind != TK::Top)
                return issue(n, "conflict", std::string("str.join of ") + kind_name(e.kind) + " items");
            return TR(STR(), Res("bmethod", std::string("str.join")));
        }
        if (attr == "partition" || attr == "rpartition") return TR(tuple_of({STR(), STR(), STR()}), Res("bmethod", "str." + attr));
        if (attr == "format") return issue(n, "syntax", "str.format");
    }
    if (k == "bytes" || k == "bytearray") {
        if (attr == "join") {
            T e;
            const std::optional<T> a = args.empty() ? std::nullopt : args[0];
            if (!a && !n->args.empty() && n->args[0]->kind == ExprKind::GeneratorExp) e = gen_elem(cast<GeneratorExp>(n->args[0]), state).first;
            else if (a) e = iteration(n->args[0], *a, state).first;
            else return issue(n, "conflict", "bytes.join of nothing");
            if (e.kind != TK::Bytes && e.kind != TK::Bytearray && e.kind != TK::Bottom && e.kind != TK::Top)
                return issue(n, "conflict", std::string("bytes.join of ") + kind_name(e.kind) + " items");
            return TR(T(recv.kind), Res("bmethod", std::string("bytes.join")));
        }
        if (attr == "split") {
            int s = p.new_site(n, TK::List, where(n));
            p.grow(p.elem, s, T(recv.kind));
            return TR(container(TK::List, s), Res("bmethod", std::string("bytes.split")));
        }
        if (k == "bytearray" && attr == "append") {
            if (args.empty() || !kind_ok(args[0], "int")) return issue(n, "conflict", "bytearray.append");
            return TR(NONE(), Res("bmethod", std::string("bytearray.append")));
        }
        if (k == "bytearray" && attr == "extend") {
            if (args.empty() || !args[0] || !(args[0]->kind == TK::Bytes || args[0]->kind == TK::Bytearray || args[0]->kind == TK::Bottom || args[0]->kind == TK::Top))
                return issue(n, "syntax", std::string("bytearray.extend of a ") + (!args.empty() && args[0] ? kind_name(args[0]->kind) : "?"));
            return TR(NONE(), Res("bmethod", std::string("bytearray.extend")));
        }
    }
    if (k == "list") {
        int s = recv.site;
        T e = elem_of(*this, recv);
        if (attr == "append") {
            if (args.size() != 1) return issue(n, "conflict", "list.append arity");
            p.grow(p.elem, s, fit(n, *args[0], "list item"));
            return TR(NONE(), Res("bmethod", std::string("list.append")));
        }
        if (attr == "extend") {
            const std::optional<T> a = args.empty() ? std::nullopt : args[0];
            T et;
            if (!a && !n->args.empty() && n->args[0]->kind == ExprKind::GeneratorExp) et = gen_elem(cast<GeneratorExp>(n->args[0]), state).first;
            else et = a ? iteration(n->args[0], *a, state).first : BOTTOM();
            p.grow(p.elem, s, et);
            return TR(NONE(), Res("bmethod", std::string("list.extend")));
        }
        if (attr == "insert") {
            if (args.size() > 1 && args[1]) p.grow(p.elem, s, *args[1]);
            return TR(NONE(), Res("bmethod", std::string("list.insert")));
        }
        if (attr == "pop") return TR(e, Res("bmethod", std::string("list.pop")));
        if (attr == "index" || attr == "count") return TR(SIZE(), Res("bmethod", "list." + attr));
        if (attr == "remove") return TR(NONE(), Res("bmethod", std::string("list.remove")));
        if (attr == "reverse" || attr == "clear") return TR(NONE(), Res("bmethod", "list." + attr));
        if (attr == "copy") {
            int s2 = p.new_site(n, TK::List, where(n));
            p.grow(p.elem, s2, e);
            return TR(container(TK::List, s2), Res("bmethod", std::string("list.copy")));
        }
        if (attr == "sort") return sort_key(*this, n, e, kw, state, "list.sort", NONE());
    }
    if (k == "dict") {
        int s = p.find(recv.site);
        bool hk = false;
        std::string key = !n->args.empty() ? str_arg(n->args[0], &hk) : "";
        if (attr == "get") {
            T dflt = args.size() > 1 && args[1] ? *args[1] : NONE();
            if (hk && !p.dynamic.count(s)) {
                T ft = p.fields[s].has(key) ? *p.fields[s].get(key) : BOTTOM();
                T r = p.join(ft, dflt);
                if (r.kind == TK::Top) return issue(n, "conflict", "dict.get default: " + r.why());
                return TR(r, Res("bmethod", std::string("record.get")));
            }
            if (!hk) p.make_dynamic(s, "get of a computed key");
            if (!args.empty() && args[0]) p.grow(p.keys, s, *args[0]);
            T r = p.join(p.elem[s], dflt);
            if (r.kind == TK::Top) return issue(n, "conflict", "dict.get default: " + r.why());
            return TR(r, Res("bmethod", std::string("dict.get")));
        }
        p.make_dynamic(s, "dict." + attr);
        if (attr == "setdefault") {
            if (args.size() != 2) return issue(n, "conflict", "dict.setdefault arity");
            p.grow(p.keys, s, *args[0]);
            p.grow(p.elem, s, *args[1]);
            return TR(p.elem[s], Res("bmethod", std::string("dict.setdefault")));
        }
        if (attr == "items") return TR(iter_of(tuple_of({p.keys[s], p.elem[s]}), MEM), Res("bmethod", std::string("dict.items")));
        if (attr == "keys") return TR(iter_of(p.keys[s], MEM), Res("bmethod", std::string("dict.keys")));
        if (attr == "values") return TR(iter_of(p.elem[s], MEM), Res("bmethod", std::string("dict.values")));
        if (attr == "pop") {
            if (!args.empty() && args[0]) p.grow(p.keys, s, *args[0]);
            T r = args.size() < 2 ? p.elem[s] : p.join(p.elem[s], args[1] ? *args[1] : BOTTOM());
            return TR(r, Res("bmethod", std::string("dict.pop")));
        }
        if (attr == "update") {
            if (args.size() == 1 && args[0] && args[0]->kind == TK::Dict) {
                int o = p.find(args[0]->site);
                p.make_dynamic(o, "dict.update");
                p.grow(p.keys, s, p.keys[o]);
                p.grow(p.elem, s, p.elem[o]);
                return TR(NONE(), Res("bmethod", std::string("dict.update")));
            }
            return issue(n, "syntax", "dict.update arguments");
        }
        if (attr == "copy") {
            int s2 = p.new_site(n, TK::Dict, where(n));
            p.make_dynamic(s2, "dict.copy");
            p.grow(p.keys, s2, p.keys[s]);
            p.grow(p.elem, s2, p.elem[s]);
            return TR(container(TK::Dict, s2), Res("bmethod", std::string("dict.copy")));
        }
        if (attr == "clear") return TR(NONE(), Res("bmethod", std::string("dict.clear")));
    }
    if (k == "set") {
        int s = recv.site;
        if (attr == "add") {
            if (!args.empty() && args[0]) p.grow(p.elem, s, *args[0]);
            return TR(NONE(), Res("bmethod", std::string("set.add")));
        }
        if (attr == "discard" || attr == "remove") return TR(NONE(), Res("bmethod", "set." + attr));
        if (attr == "update") {
            T et = !args.empty() && args[0] ? iteration(n->args[0], *args[0], state).first : BOTTOM();
            p.grow(p.elem, s, et);
            return TR(NONE(), Res("bmethod", std::string("set.update")));
        }
        if (attr == "issubset" || attr == "issuperset" || attr == "isdisjoint") return TR(BOOL(), Res("bmethod", "set." + attr));
    }
    if (k == "int") {
        if (attr == "bit_length") return TR(INT(Big(0), recv.fits_i64() ? OBig(Big(64)) : OBig()), Res("bmethod", std::string("int.bit_length")));
        if (attr == "to_bytes") return TR(BYTES(), Res("bmethod", std::string("int.to_bytes")));
    }
    if (k == "fn" && recv.name().rfind("builtin:", 0) == 0) {
        std::string full = recv.name().substr(8) + "." + attr;
        if (full == "int.from_bytes") return from_bytes(*this, n, args, kw);
        if (full == "bytes.fromhex") return TR(BYTES(), Res("std", std::string("bytes.fromhex")));
        return issue(n, "stdlib", full);
    }
    return issue(n, "stdlib", k + "." + attr);
}

// ------------------------------------------------------------------------------------ builtins
static const std::set<std::string> DYNAMIC_BUILTINS = {"getattr", "setattr", "hasattr", "delattr", "type", "id", "hash", "vars", "globals",
                                                       "locals", "callable", "issubclass", "exec", "eval", "compile", "iter", "next",
                                                       "memoryview", "input"};

// a value whose evaluation can call, raise or change nothing
static bool plain_value(const A::Expr* e) {
    switch (e->kind) {
        case ExprKind::Name: case ExprKind::Constant: return true;
        case ExprKind::Attribute: return plain_value(cast<Attribute>(e)->value);
        case ExprKind::UnaryOp: return cast<UnaryOp>(e)->operand->kind == ExprKind::Constant;
        case ExprKind::Tuple:
            for (auto* x : cast<Tuple>(e)->elts)
                if (!plain_value(x)) return false;
            return true;
        default: return false;
    }
}

static void drop_set_order(Eval& ev, const A::Call* n) {
    auto& is = ev.issues;
    is.erase(std::remove_if(is.begin(), is.end(), [&](const Issue& i) { return i.detail == "set iteration order" && i.line == n->pos.lineno; }), is.end());
}

static TR isinstance_(Eval& ev, const A::Call* n, const std::optional<T>& t) {
    static const std::map<std::string, std::set<TK>> IST = {
        {"int", {TK::Int, TK::Bool}}, {"str", {TK::Str}}, {"bytes", {TK::Bytes}}, {"bytearray", {TK::Bytearray}}, {"bool", {TK::Bool}},
        {"float", {TK::Float}}, {"list", {TK::List}}, {"dict", {TK::Dict}}, {"tuple", {TK::Tuple}}, {"set", {TK::Set}}};
    if (!t || n->args.size() != 2) return ev.issue(n, "conflict", "isinstance arguments");
    if (t->kind == TK::Bottom || t->kind == TK::Top) return BOOL();
    if (t->kind == TK::Variant) {   // (new) a run-time test of the variant's tag
        std::vector<const A::Expr*> specs;
        if (auto* tu = as<pyfront::Tuple>(n->args[1])) specs.assign(tu->elts.begin(), tu->elts.end());
        else specs.push_back(n->args[1]);
        std::string tags;
        for (auto* sp : specs) {
            auto* sn = as<pyfront::Name>(sp);
            if (!sn || !IST.count(sn->id)) return ev.issue(n, "dynamic", "isinstance of a variant against " + unparse(sp));
            for (auto& alt : t->args())
                if (IST.at(sn->id).count(alt.kind)) tags += (tags.empty() ? "" : ",") + std::string(kind_name(alt.kind));
        }
        return TR(BOOL(), Res("isinstance_variant", tags));
    }
    const A::Expr* spec = n->args[1];
    std::vector<const A::Expr*> names;
    if (auto* tu = as<pyfront::Tuple>(spec)) names.assign(tu->elts.begin(), tu->elts.end());
    else names.push_back(spec);
    bool answer = false;
    for (auto* x : names) {
        auto* nm = as<pyfront::Name>(x);
        if (nm && IST.count(nm->id)) {
            if (IST.at(nm->id).count(t->kind)) answer = true;
            else if (t->kind == TK::Opt && IST.at(nm->id).count(t->args()[0].kind))
                return ev.issue(n, "dynamic", "isinstance of an optional value (use `is not None`)");
        } else if (nm && t->kind == TK::Inst) {
            std::string cq = ev.lookup_class(x);
            if (cq.empty()) return ev.issue(n, "dynamic", "isinstance " + unparse(x));
            auto m1 = ev.p.mro(t->name());
            if (std::find(m1.begin(), m1.end(), cq) != m1.end()) answer = true;
            else {
                auto m2 = ev.p.mro(cq);
                if (std::find(m2.begin(), m2.end(), t->name()) != m2.end())
                    return ev.issue(n, "dynamic", "isinstance of a subclass (run-time class)");
            }
        } else if (nm) {
            if (ev.lookup_class(x).empty() && !IST.count(nm->id)) return ev.issue(n, "dynamic", "isinstance " + unparse(x));
        } else {
            return ev.issue(n, "dynamic", "isinstance " + unparse(x));
        }
    }
    return TR(BOOL(), Res("isinstance", answer ? "True" : ""));
}

std::string Eval::getattr_name(const A::Call* n) {
    std::string s;
    is_str_const(n->args[1], &s);
    return s;
}

int Eval::getattr_form(const A::Call* n, const State& state) {
    auto* fname = as<pyfront::Name>(n->func);
    if (!fname || fname->id != "getattr" || !n->keywords.empty() || (n->args.size() != 2 && n->args.size() != 3)) return 0;
    std::string a;
    if (!is_str_const(n->args[1], &a) || a.empty()) return 0;
    for (auto* x : n->args)
        if (x->kind == ExprKind::Starred) return 0;
    T ft = ev(n->func, state);
    if (ft.kind != TK::Fn || ft.name() != "builtin:getattr") return 0;
    T ot = ev(n->args[0], state);
    if (ot.kind != TK::Inst) return 0;
    int c = p.getattr_choice(ot.name(), a);
    if (c == 2 && n->args.size() != 3) return 0;   // (an AttributeError: left to the classic rule)
    return c;
}

// (new) the attribute every instance of the class has: a class attribute, or one __init__ sets on every path (a
// `self.a = ...` statement at the top of its body) -- getattr(x, "a", d) can only be x.a when no instance lacks it
static bool always_has(Program& p, const std::string& cq, const std::string& attr) {
    for (auto& c : p.mro(cq)) {
        Class** k = p.classes.get(c);
        if (!k) continue;
        if ((*k)->class_attrs.has(attr)) return true;
        Fn** init = (*k)->methods.get("__init__");
        if (!init) continue;
        const std::string& self = (*init)->params.empty() ? std::string() : (*init)->params[0];
        for (auto* st : (*init)->node->body)
            if (auto* a = as<Assign>(st))
                for (auto* t : a->targets)
                    if (auto* at = as<Attribute>(t))
                        if (auto* nm = as<pyfront::Name>(at->value); nm && nm->id == self && at->attr == attr) return true;
        return false;   // (the first class with an __init__ is the one that runs)
    }
    return false;
}

TR Eval::builtin(const A::Call* n, const std::string& name, const Args& args, const Kw& kw, const State& state) {
    // getattr(x, "a"[, default]) of an instance of the program's classes: x.a when every instance has it, the
    // default when no class of x's (nor a subclass) has it; else still dynamic
    if (name == "getattr" && p.extended && (n->args.size() == 2 || n->args.size() == 3) && n->keywords.empty() && !args.empty() && args[0]) {
        std::string an;
        const T& ot = *args[0];
        if (is_str_const(n->args[1], &an) && ot.kind == TK::Inst && p.classes.has(ot.name()) && !p.find_method(ot.name(), an)) {
            std::optional<T> at = p.attr_type(ot.name(), an);
            if (at && at->kind != TK::Top && always_has(p, ot.name(), an)) return TR(*at, Res("getattr_attr", an));
            if (!at && n->args.size() == 3) return TR(ev(n->args[2], state), Res("getattr_default", an));
        }
    }
    // next(<generator with one for>, default): the first item, or the default; a loop that stops at the
    // first. The default is evaluated before the loop, so it must be free of effects (a name, constant, attribute)
    if (name == "next" && p.extended && n->args.size() == 2 && n->keywords.empty() && n->args[0]->kind == ExprKind::GeneratorExp &&
        cast<GeneratorExp>(n->args[0])->generators.size() == 1 && plain_value(n->args[1])) {
        T e = gen_elem(cast<GeneratorExp>(n->args[0]), state).first;
        T d = (args.size() > 1 && args[1]) ? *args[1] : ev(n->args[1], state);
        return TR(p.join(e, d), Res("builtin", std::string("next")));
    }
    if (DYNAMIC_BUILTINS.count(name)) return issue(n, "dynamic", name);
    T bad;
    Kw none;
    if (lazy_bad(*this, n, args, (name == "sorted" || name == "min" || name == "max") ? none : kw, LAZY_BUILTINS.count(name) != 0, bad))
        return bad;
    std::optional<T> a0 = args.empty() ? std::nullopt : args[0];
    auto genarg0 = [&]() { return !n->args.empty() && n->args[0]->kind == ExprKind::GeneratorExp; };
    if (name == "len") {
        if (args.size() != 1 || !a0) return issue(n, "conflict", "len arity");
        TK k = a0->kind;
        if (k == TK::Dict) p.make_dynamic(a0->site, "len");
        if (k == TK::Tuple) return TR(const_int(Big((int64_t)a0->args().size())), Res("builtin", std::string("len")));
        if (k == TK::Bytes || k == TK::Bytearray) return TR(INT(Big(0), a0->hi() ? *a0->hi() : MEM), Res("builtin", std::string("len")));
        if (k == TK::Str || k == TK::List || k == TK::Dict || k == TK::Set || k == TK::Bottom || k == TK::Top)
            return TR(SIZE(), Res("builtin", std::string("len")));
        return issue(n, "conflict", std::string("len of a ") + kind_name(k));
    }
    if (name == "range") {
        bool okk = kw.empty() && args.size() >= 1 && args.size() <= 3;
        for (auto& a : args)
            if (!a || !(a->kind == TK::Int || a->kind == TK::Bottom || a->kind == TK::Top || a->kind == TK::Bool)) okk = false;
        if (!okk) return issue(n, "conflict", "range arguments");
        for (auto& a : args)
            if (a->kind == TK::Bottom) return BOTTOM();
        for (auto& a : args)
            if (a->kind == TK::Top) return *a;
        std::vector<T> as_;
        for (auto& a : args) as_.push_back(a->kind == TK::Bool ? INT(Big(0), Big(1)) : *a);
        T lo = as_.size() == 1 ? const_int(Big(0)) : as_[0];
        T hi = as_.size() == 1 ? as_[0] : as_[1];
        Big step(1);
        if (as_.size() == 3) {
            if (!int_const(n->args[2], &step)) return issue(n, "syntax", "range with a computed step");
        }
        if (step.sign() == 0) return issue(n, "syntax", "range with a computed step");
        T e = step.sign() > 0 ? INT(lo.lo(), hi.hi() ? OBig(*hi.hi() - Big(1)) : OBig()) : INT(hi.lo() ? OBig(*hi.lo() + Big(1)) : OBig(), lo.hi());
        return TR(iter_of(e, range_count(n)), Res("builtin", std::string("range")));
    }
    if (name == "enumerate") {
        if (!a0) return issue(n, "conflict", "enumerate arguments");
        auto [e, cnt] = iteration(n->args[0], *a0, state);
        T start = args.size() > 1 && args[1] ? *args[1] : (kw.has("start") && *kw.get("start") ? **kw.get("start") : const_int(Big(0)));
        if (start.kind != TK::Int) return issue(n, "conflict", "enumerate start");
        T idx = iadd(start, INT(Big(0), (cnt ? *cnt : MEM) - Big(1)));
        return TR(iter_of(tuple_of({idx, e}), cnt), Res("builtin", std::string("enumerate")));
    }
    if (name == "zip") {
        bool anyn = false;
        for (auto& a : args)
            if (!a) anyn = true;
        if (!kw.empty() || anyn) return issue(n, "syntax", "zip arguments");
        std::vector<T> es;
        for (size_t i = 0; i < args.size(); i++) es.push_back(iteration(n->args[i], *args[i], state).first);
        return TR(iter_of(tuple_of(es), MEM), Res("builtin", std::string("zip")));
    }
    if (name == "reversed") {
        if (!a0) return issue(n, "conflict", "reversed arguments");
        if (a0->kind == TK::Dict) return issue(n, "syntax", "reversed of a dict");
        auto [e, cnt] = iteration(n->args[0], *a0, state);
        return TR(iter_of(e, cnt), Res("builtin", std::string("reversed")));
    }
    if (name == "sorted") {
        T e;
        if (!a0 && genarg0()) e = gen_elem(cast<GeneratorExp>(n->args[0]), state).first;
        else if (!a0) return issue(n, "conflict", "sorted arguments");
        else {
            e = iteration(n->args[0], *a0, state).first;
            if (a0->kind == TK::Set) drop_set_order(*this, n);
        }
        int s = p.new_site(n, TK::List, where(n));
        p.grow(p.elem, s, e);
        return sort_key(*this, n, e, kw, state, "sorted", container(TK::List, s));
    }
    if (name == "min" || name == "max") {
        for (auto& [k, _] : kw)
            if (k != "key" && k != "default") return issue(n, "syntax", name + "(" + k + "=)");
        if (kw.has("key")) return issue(n, "syntax", name + " with a key");
        std::vector<std::optional<T>> ts;
        if (args.size() == 1) {
            T e;
            if (!a0 && genarg0()) e = gen_elem(cast<GeneratorExp>(n->args[0]), state).first;
            else e = iteration(n->args[0], *a0, state).first;
            if (kw.has("default")) e = p.join(e, kw.get("default")->value_or(BOTTOM()));
            ts.push_back(e);
        } else {
            ts = args;
        }
        T r = BOTTOM();
        for (auto& t : ts) {
            if (!t) return issue(n, "conflict", name + " arguments");
            r = p.join(r, t->kind == TK::Bool ? INT(Big(0), Big(1)) : *t);
        }
        if (r.kind == TK::Top) return issue(n, "conflict", name + " of mixed types: " + r.why());
        if (!(r.kind == TK::Int || r.kind == TK::Str || r.kind == TK::Float || r.kind == TK::Bytes || r.kind == TK::Tuple || r.kind == TK::Bottom))
            return issue(n, "conflict", name + " of " + kind_name(r.kind));
        if (r.kind == TK::Int && ts.size() > 1) {
            bool lo_none = false, hi_none_all = true, hi_none = false, lo_none_all = true;
            OBig minlo, minhi, maxlo, maxhi;
            for (auto& t : ts) {
                const OBig& l = t->lo();
                const OBig& h = t->hi();
                if (!l) lo_none = true;
                else {
                    lo_none_all = false;
                    if (!minlo || *l < *minlo) minlo = l;
                    if (!maxlo || *l > *maxlo) maxlo = l;
                }
                if (!h) hi_none = true;
                else {
                    hi_none_all = false;
                    if (!minhi || *h < *minhi) minhi = h;
                    if (!maxhi || *h > *maxhi) maxhi = h;
                }
            }
            if (name == "min") r = INT(lo_none ? OBig() : minlo, hi_none_all ? OBig() : minhi);
            else r = INT(lo_none_all ? OBig() : maxlo, hi_none ? OBig() : maxhi);
        }
        return TR(r, Res("builtin", name + (args.size() == 1 ? "_iter" : "")));
    }
    if (name == "abs") {
        if (!a0) return issue(n, "conflict", "abs");
        if (a0->kind == TK::Int) {
            if (a0->lo() && a0->lo()->sign() >= 0) return TR(*a0, Res("builtin", std::string("abs")));
            OBig mm;
            if (a0->lo() && a0->hi()) mm = std::max(a0->lo()->abs(), a0->hi()->abs());
            return TR(INT(Big(0), mm), Res("builtin", std::string("abs")));
        }
        if (a0->kind == TK::Float) return TR(FLOAT(), Res("builtin", std::string("abs")));
        return (a0->kind == TK::Bottom || a0->kind == TK::Top) ? TR(*a0) : TR(issue(n, "conflict", std::string("abs of ") + kind_name(a0->kind)));
    }
    if (name == "sum") {
        T e;
        std::optional<Big> cnt;
        if (!a0 && genarg0()) {
            e = gen_elem(cast<GeneratorExp>(n->args[0]), state).first;
            cnt = MEM;
        } else if (a0) {
            auto r = iteration(n->args[0], *a0, state);
            e = r.first;
            cnt = r.second;
        } else
            return issue(n, "conflict", "sum");
        if (e.kind == TK::Bool) e = INT(Big(0), Big(1));
        if (e.kind == TK::Int) {
            Big c = cnt ? *cnt : MEM;
            T r = imul(e, INT(Big(0), c));
            r = INT(r.lo() ? OBig(std::min(*r.lo(), Big(0))) : OBig(), r.hi() ? OBig(std::max(*r.hi(), Big(0))) : OBig());
            if (args.size() > 1 && args[1]) r = iadd(r, *args[1]);
            return TR(r, Res("builtin", std::string("sum")));
        }
        if (e.kind == TK::Float) return TR(FLOAT(), Res("builtin", std::string("sum")));
        return (e.kind == TK::Bottom || e.kind == TK::Top) ? TR(e) : TR(issue(n, "conflict", std::string("sum of ") + kind_name(e.kind)));
    }
    if (name == "any" || name == "all") {
        if (!a0 && genarg0()) gen_elem(cast<GeneratorExp>(n->args[0]), state);
        else if (a0) iteration(n->args[0], *a0, state);
        return TR(BOOL(), Res("builtin", name));
    }
    if (name == "int") {
        if (!kw.empty()) return issue(n, "syntax", "int keywords");
        if (args.empty()) return TR(const_int(Big(0)), Res("builtin", std::string("int")));
        if (a0->kind == TK::Int || a0->kind == TK::Bool) return TR(a0->kind == TK::Int ? *a0 : INT(Big(0), Big(1)), Res("builtin", std::string("int")));
        if (a0->kind == TK::Str) {
            Big b;
            if (args.size() == 2 && !int_const(n->args[1], &b)) return issue(n, "dynamic", "int() with a computed base");
            return TR(INT(), Res("builtin", std::string("int_str")));
        }
        if (a0->kind == TK::Float) return TR(INT(), Res("builtin", std::string("int_float")));
        return (a0->kind == TK::Bottom || a0->kind == TK::Top) ? TR(*a0) : TR(issue(n, "conflict", std::string("int() of ") + kind_name(a0->kind)));
    }
    if (name == "float") {
        // float(int) rounds correctly, as the int64 -> double conversion does
        return TR(FLOAT(), Res("builtin", std::string("float")));
    }
    if (name == "str") {
        if (args.empty()) return TR(STR(), Res("builtin", std::string("str")));
        check_str(n, *a0, false);
        return TR(STR(), Res("builtin", std::string("str")));
    }
    if (name == "repr") {
        check_str(n, a0 ? *a0 : BOTTOM(), true);
        return TR(STR(), Res("builtin", std::string("repr")));
    }
    if (name == "bool") return TR(BOOL(), Res("builtin", std::string("bool")));
    if (name == "bytes" || name == "bytearray") {
        TK bk = name == "bytes" ? TK::Bytes : TK::Bytearray;
        if (args.empty()) return TR(T(bk, {}, {}, Big(0)), Res("builtin", name));
        if (!a0 && genarg0()) {   // bytes(x for x in ...): a list of ints made first (lowered as the list), as bytes(list)
            T e = gen_elem(cast<GeneratorExp>(n->args[0]), state).first;
            if (e.kind != TK::Int && e.kind != TK::Bottom && e.kind != TK::Top) return issue(n, "conflict", name + " of a generator of " + kind_name(e.kind));
            return TR(T(bk), Res("builtin", name + "_list"));
        }
        if (!a0) return issue(n, "dynamic", "a lambda or generator as an argument");
        if (a0->kind == TK::Int) return TR(T(bk, {}, {}, a0->hi()), Res("builtin", name + "_zeros"));
        if (a0->kind == TK::Bytes || a0->kind == TK::Bytearray) return TR(T(bk, {}, {}, a0->hi()), Res("builtin", name + "_copy"));
        if (a0->kind == TK::List) {
            T e = elem_of(*this, *a0);
            if (e.kind != TK::Int && e.kind != TK::Bottom && e.kind != TK::Top)
                return issue(n, "conflict", name + " of a list of " + kind_name(e.kind));
            return TR(T(bk), Res("builtin", name + "_list"));
        }
        if (a0->kind == TK::Str) return issue(n, "syntax", name + "(str, encoding)");
        if (a0->kind == TK::Iter) return TR(T(bk), Res("builtin", name + "_iter"));
        return (a0->kind == TK::Bottom || a0->kind == TK::Top) ? TR(*a0) : TR(issue(n, "conflict", name + " of " + kind_name(a0->kind)));
    }
    if (name == "list" || name == "set") {
        TK ck = name == "list" ? TK::List : TK::Set;
        int s = p.new_site(n, ck, where(n));
        if (!args.empty()) {
            T e;
            if (!a0 && genarg0()) e = gen_elem(cast<GeneratorExp>(n->args[0]), state).first;
            else {
                e = iteration(n->args[0], *a0, state).first;
                if (ck == TK::Set && a0 && a0->kind == TK::Set) drop_set_order(*this, n);
            }
            p.grow(p.elem, s, e);
        }
        return TR(container(ck, s), Res("builtin", name));
    }
    if (name == "frozenset") {   // (new) a set that is not changed: the set type (its items as set's)
        int s = p.new_site(n, TK::Set, where(n));
        if (!args.empty()) {
            T e;
            if (!a0 && genarg0()) e = gen_elem(cast<GeneratorExp>(n->args[0]), state).first;
            else if (a0) e = iteration(n->args[0], *a0, state).first;
            else return issue(n, "dynamic", "a lambda or generator as an argument");
            p.grow(p.elem, s, e);
        }
        return TR(container(TK::Set, s), Res("builtin", std::string("set")));
    }
    if (name == "tuple") {
        if (a0 && a0->kind == TK::Tuple) return TR(*a0, Res("builtin", std::string("tuple")));
        return issue(n, "conflict", "tuple() of a sequence (its length is not in the type)");
    }
    if (name == "dict") {
        int s = p.new_site(n, TK::Dict, where(n));
        std::vector<std::string> keys;
        for (auto* k : n->keywords) {
            if (!k->has_arg) {
                spread(s, ev(k->value, state), n);
            } else {
                const std::optional<T>* v = kw.get(k->arg);
                p.grow_field(s, k->arg, fit(k->value, v && *v ? **v : BOTTOM(), "field " + k->arg));
                keys.push_back(k->arg);
            }
        }
        if (!args.empty()) {
            if (a0 && a0->kind == TK::Dict) spread(s, *a0, n);
            else return issue(n, "syntax", std::string("dict() of a ") + (a0 ? kind_name(a0->kind) : "lambda"));
        }
        if (live()) {
            std::vector<std::string> lk = keys;
            auto sk = p.spread_keys.find(s);
            if (sk != p.spread_keys.end()) lk.insert(lk.end(), sk->second.begin(), sk->second.end());
            p.literal_keys[n] = lk;
            if (builder != n) p.created(s, lk);
        }
        return TR(container(TK::Dict, s), Res("builtin", std::string("dict")));
    }
    if (name == "print") {
        for (auto& [k, _] : kw)
            if (k != "end" && k != "sep" && k != "file" && k != "flush") return issue(n, "syntax", "print keywords");
        if (kw.has("file")) return issue(n, "stdlib", "print(file=)");
        for (size_t i = 0; i < n->args.size() && i < args.size(); i++)
            if (args[i]) check_str(n->args[i], *args[i], false);
        return TR(NONE(), Res("builtin", std::string("print")));
    }
    if (name == "open") {
        bool ok;
        std::string mode = n->args.size() > 1 ? str_arg(n->args[1], &ok) : str_arg(kw_value(n, "mode"), &ok);
        if (ok && mode == "rb") return TR(named(TK::Extern, "file"), Res("builtin", std::string("open_rb")));
        if (p.extended) {
            bool handled = false;
            TR r = ext_std(n, "builtins.open", args, kw, state, handled);
            if (handled) return r;
        }
        return issue(n, "stdlib", "open(" + (ok ? py_repr(mode) : std::string("None")) + ")");
    }
    if (name == "ord") return TR(INT(Big(0), Big(0x10FFFF)), Res("builtin", std::string("ord")));
    if (name == "chr") {
        if (a0 && a0->kind != TK::Int && a0->kind != TK::Bottom && a0->kind != TK::Top) return issue(n, "conflict", std::string("chr of ") + kind_name(a0->kind));
        return TR(STR(), Res("builtin", std::string("chr")));
    }
    if (name == "hex" || name == "oct" || name == "bin") return TR(STR(), Res("builtin", name));
    if (name == "divmod") {
        if (args.size() == 2 && args[0] && args[1] && args[0]->kind == TK::Int && args[1]->kind == TK::Int)
            return TR(tuple_of({ifloordiv(*args[0], *args[1]), imod(*args[0], *args[1])}), Res("builtin", std::string("divmod")));
        return issue(n, "conflict", "divmod arguments");
    }
    if (name == "pow") {
        bool ints = true;
        for (auto& a : args)
            if (!a || a->kind != TK::Int) ints = false;
        if (args.size() == 3 && ints && args[2]->positive())
            return TR(INT(Big(0), args[2]->hi() ? OBig(*args[2]->hi() - Big(1)) : OBig()), Res("builtin", std::string("pow3")));
        if (args.size() == 2 && args[0] && args[1])
            return TR(binop(n, OpKind::Pow, *args[0], *args[1], n->args[0], n->args[1]).t, Res("builtin", std::string("pow")));
        return issue(n, "conflict", "pow arguments");
    }
    if (name == "round") {
        if (args.size() == 1) return TR(INT(), Res("builtin", std::string("round")));
        return TR(FLOAT(), Res("builtin", std::string("round2")));
    }
    if (name == "format") {
        bool ok = true;
        std::string spec = n->args.size() > 1 ? str_arg(n->args[1], &ok) : "";
        if (!ok) return issue(n, "dynamic", "format with a computed spec");
        check_format(n, a0 ? *a0 : BOTTOM(), spec);
        return TR(STR(), Res("builtin", std::string("format")));
    }
    if (name == "isinstance") return isinstance_(*this, n, a0);
    if (name == "super") {
        if (!args.empty()) return issue(n, "syntax", "super(arguments)");
        return TR(named(TK::Extern, "super"), Res("builtin", std::string("super")));
    }
    if (name == "map" || name == "filter") return issue(n, "dynamic", name);
    if (name == "object" || name == "staticmethod" || name == "classmethod" || name == "property") return issue(n, "dynamic", name);
    return issue(n, "stdlib", "builtins." + name);
}

// ------------------------------------------------------------------------------------ stdlib
TR Eval::std_call(const A::Call* n, const std::string& name, const Args& args, const Kw& kw, const State& state) {
    T bad;
    if (lazy_bad(*this, n, args, kw, false, bad)) return bad;
    if (name.rfind("struct.", 0) == 0) {
        std::string fn_ = name.substr(7);
        if (!kw.empty()) return issue(n, "syntax", name + " keywords");
        if (fn_ == "error") return TR(named(TK::Inst, "exc:struct.error"), Res("exc", std::string("struct.error")));
        bool ok = false;
        std::string fmt = !n->args.empty() ? str_arg(n->args[0], &ok) : "";
        if (!ok) return issue(n, "dynamic", name + " with a computed format");
        std::vector<FmtItem> items;
        if (!struct_parse(fmt, items)) return issue(n, "stdlib", name + "(" + py_repr(fmt) + ")");
        std::vector<FmtItem> vals;
        int64_t size = 0;
        for (auto& it : items) {
            size += it.size;
            if (it.code != 'x') vals.push_back(it);
        }
        if (fn_ == "calcsize") return TR(const_int(Big(size)), Res("std", name));
        if (fn_ == "unpack" || fn_ == "unpack_from") {
            if ((fn_ == "unpack" && args.size() != 2) || (fn_ == "unpack_from" && args.size() != 2 && args.size() != 3))
                return issue(n, "conflict", name + " arity");
            const std::optional<T>& b = args[1];
            if (!b || !(b->kind == TK::Bytes || b->kind == TK::Bytearray || b->kind == TK::Bottom || b->kind == TK::Top))
                return issue(n, "conflict", name + " of a " + (b ? kind_name(b->kind) : "?"));
            if (args.size() == 3 && args[2] && !(args[2]->kind == TK::Int || args[2]->kind == TK::Bottom || args[2]->kind == TK::Top))
                return issue(n, "conflict", name + " offset");
            std::vector<T> ts;
            for (auto& v : vals) {
                if (v.kind == "int") ts.push_back(INT(v.lo, v.hi));
                else if (v.kind == "bytes") ts.push_back(bytes_of(Big(v.size)));
                else if (v.kind == "bool") ts.push_back(BOOL());
                else ts.push_back(FLOAT());
            }
            return TR(tuple_of(ts), Res("std", name, fmt));
        }
        if (fn_ == "pack" || fn_ == "pack_into") {
            size_t k0 = fn_ == "pack" ? 1 : 3;
            std::vector<std::optional<T>> given(args.begin() + std::min(k0, args.size()), args.end());
            if (given.size() != vals.size())
                return issue(n, "conflict", name + ": " + std::to_string(given.size()) + " values for " + py_repr(fmt));
            if (fn_ == "pack_into" && (args.size() < 2 || !args[1] || !(args[1]->kind == TK::Bytearray || args[1]->kind == TK::Bottom || args[1]->kind == TK::Top)))
                return issue(n, "conflict", std::string("pack_into a ") + (args.size() > 1 && args[1] ? kind_name(args[1]->kind) : "?"));
            for (size_t i = 0; i < given.size(); i++) {
                if (!given[i]) return issue(n, "conflict", name + " value");
                TK tk = given[i]->kind;
                const std::string& kind = vals[i].kind;
                bool ok2 = tk == TK::Bottom || tk == TK::Top;
                if (kind == "int") ok2 = ok2 || tk == TK::Int || tk == TK::Bool;
                if (kind == "bytes") ok2 = ok2 || tk == TK::Bytes || tk == TK::Bytearray;
                if (kind == "bool") ok2 = ok2 || tk == TK::Bool || tk == TK::Int;
                if (kind == "float") ok2 = ok2 || tk == TK::Float || tk == TK::Int;
                if (!ok2) return issue(n, "conflict", name + " of a " + kind_name(tk) + " for " + std::string(1, vals[i].code));
            }
            return TR(fn_ == "pack" ? bytes_of(Big(size)) : NONE(), Res("std", name, fmt));
        }
        return issue(n, "stdlib", name);
    }
    static const std::map<std::string, std::pair<int, std::function<T()>>> PATH = {
        {"os.path.join", {-1, [] { return STR(); }}},           {"os.path.basename", {1, [] { return STR(); }}},
        {"os.path.dirname", {1, [] { return STR(); }}},         {"os.path.splitext", {1, [] { return tuple_of({STR(), STR()}); }}},
        {"os.path.exists", {1, [] { return BOOL(); }}},         {"os.path.isfile", {1, [] { return BOOL(); }}},
        {"os.path.isdir", {1, [] { return BOOL(); }}},          {"os.path.getsize", {1, [] { return SIZE(); }}},
        {"os.path.split", {1, [] { return tuple_of({STR(), STR()}); }}}, {"os.path.normcase", {1, [] { return STR(); }}}};
    auto pit = PATH.find(name);
    if (pit != PATH.end()) {
        if (!kw.empty()) return issue(n, "syntax", name + " keywords");
        auto strish = [](const std::optional<T>& a) { return a && (a->kind == TK::Str || a->kind == TK::Bottom || a->kind == TK::Top); };
        if (pit->second.first < 0) {
            bool ok = !args.empty();
            for (auto& a : args)
                if (!strish(a)) ok = false;
            if (!ok) return issue(n, "conflict", name + " arguments");
        } else {
            bool ok = (int)args.size() == pit->second.first;
            for (auto& a : args)
                if (!strish(a)) ok = false;
            if (!ok) return issue(n, "conflict", name + " arguments");
        }
        return TR(pit->second.second(), Res("std", name));
    }
    if (name == "os.listdir") {
        int s = p.new_site(n, TK::List, where(n));
        p.grow(p.elem, s, STR());
        return TR(container(TK::List, s), Res("std", name));
    }
    if (name == "binascii.crc32" || name == "zlib.crc32") {
        if (args.empty() || !args[0] || !(args[0]->kind == TK::Bytes || args[0]->kind == TK::Bytearray || args[0]->kind == TK::Bottom || args[0]->kind == TK::Top))
            return issue(n, "conflict", name + " arguments");
        if (args.size() > 1 && (!args[1] || !args[1]->fits_i64())) return issue(n, "int-range", name + " start value");
        return TR(u(32), Res("std", name));
    }
    if (name == "bisect.bisect_left" || name == "bisect.bisect_right" || name == "bisect.bisect") {
        if (args.size() != 2 || !kw.empty() || !args[0] || args[0]->kind != TK::List) return issue(n, "syntax", name + " arguments");
        return TR(SIZE(), Res("std", name));
    }
    if (name == "math.sqrt") return TR(FLOAT(), Res("std", name));
    if (name == "math.floor" || name == "math.ceil") return TR(INT(), Res("std", name));
    if (p.extended) {
        bool handled = false;
        TR r = ext_std(n, name, args, kw, state, handled);
        if (handled) return r;
    }
    std::string top_ = name.substr(0, name.find('.'));
    return issue(n, "stdlib", top_ != "re" ? name : "re." + name.substr(name.rfind('.') + 1));
}

}  // namespace pyengine
