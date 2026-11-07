// lower.cc -- typed functions into L2 (lower.py and translate/build_unit of the old front end, rule
// for rule). Python's own operations C++ does not have as such are calls owned by "py": their names
// are the API map's or the support header's (truthy, slice, floordiv, ...). Nothing here knows C++.
#include "lower.hpp"

#include <algorithm>
#include <functional>
#include <regex>

#include "json.hpp"
#include "pyfront/pyfront.hpp"

namespace pyengine {

using namespace pyfront;

static const std::string PY = "py";
static TyP I64() { static TyP t = prim("i64"); return t; }
static TyP BOOLT() { static TyP t = prim("bool"); return t; }
static TyP F64() { static TyP t = prim("f64"); return t; }
static TyP VOID() { static TyP t = prim("void"); return t; }
static TyP STRT() { static TyP t = cls("py.str"); return t; }
static TyP BYTEST() { static TyP t = cls("py.bytes"); return t; }

static bool is_opt_t(const TyP& t) { return t && t->kind == "class" && t->name == "py.optional"; }
static TyP opt_t(const TyP& inner) { return cls("py.optional", {inner}); }

static std::string rsplit1(const std::string& q) {
    auto p = q.rfind('.');
    return p == std::string::npos ? q : q.substr(p + 1);
}
static std::string split2_last(const std::string& q) {   // q.split('.', 2)[-1]
    auto a = q.find('.');
    if (a == std::string::npos) return q;
    auto b = q.find('.', a + 1);
    if (b == std::string::npos) return q.substr(a + 1);
    return q.substr(b + 1);
}
static std::string trunc(const std::string& s, size_t n) {
    // first n characters (UTF-8 aware)
    size_t chars = 0, i = 0;
    while (i < s.size() && chars < n) {
        unsigned char c = (unsigned char)s[i];
        size_t len = c >= 0xF0 ? 4 : c >= 0xE0 ? 3 : c >= 0xC0 ? 2 : 1;
        i += len;
        chars++;
    }
    return s.substr(0, std::min(i, s.size()));
}
static std::string latin1_to_utf8(const std::string& raw) {
    std::string out;
    for (unsigned char c : raw) {
        if (c < 0x80) out += (char)c;
        else {
            out += (char)(0xC0 | (c >> 6));
            out += (char)(0x80 | (c & 0x3F));
        }
    }
    return out;
}

// ------------------------------------------------------------------------------------ Records
static std::string camel(const std::string& s0) {
    std::string s;
    for (char c : s0) s += isalnum((unsigned char)c) ? c : ' ';
    std::string out, w;
    auto flush = [&]() {
        if (!w.empty()) {
            out += (char)toupper((unsigned char)w[0]);
            out += w.substr(1);
            w.clear();
        }
    };
    for (char c : s) {
        if (c == ' ') flush();
        else w += c;
    }
    flush();
    return out.empty() ? "Record" : out;
}
static bool ends(const std::string& s, const std::string& e) { return s.size() >= e.size() && s.compare(s.size() - e.size(), e.size(), e) == 0; }
static std::string singular(const std::string& w) {
    if (ends(w, "ies")) return w.substr(0, w.size() - 3) + "y";
    if (ends(w, "sses") || ends(w, "xes")) return w.substr(0, w.size() - 2);
    if (ends(w, "s") && !ends(w, "ss")) return w.substr(0, w.size() - 1);
    return w;
}
static const std::vector<std::string> VERBS = {"parse_", "read_", "get_", "make_", "build_", "load_", "collect_", "find_", "scan_", "compute_"};
static const std::set<std::string> GENERIC = {"out", "r", "d", "e", "x", "h", "s", "o", "v", "res", "result", "info", "rec", "item", "entry",
                                              "m", "p", "n", "it", "data", "obj", "tmp", "row", "c", "f", "t"};

static std::string lower_s(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

Records::Records(Program& p_) : p(p_) {
    // the innermost function of each line
    std::map<std::string, std::map<int, std::string>> fn_of_line;
    for (auto& [q, f] : p.fns) {
        auto& d = fn_of_line[f->module->name];
        int end = f->node->pos.end_lineno ? f->node->pos.end_lineno : f->node->pos.lineno;
        for (int line = f->node->pos.lineno; line <= end; line++) d.emplace(line, f->qname);
    }
    std::vector<Fn*> byspan;
    for (auto& [q, f] : p.fns) byspan.push_back(f);
    std::stable_sort(byspan.begin(), byspan.end(), [](Fn* a, Fn* b) { return a->lines > b->lines; });
    for (Fn* f : byspan) {
        auto& d = fn_of_line[f->module->name];
        int end = f->node->pos.end_lineno ? f->node->pos.end_lineno : f->node->pos.lineno;
        for (int line = f->node->pos.lineno; line <= end; line++) d[line] = f->qname;
    }
    auto fq_at = [&](const std::string& mod, int line) -> std::string {
        auto it = fn_of_line.find(mod);
        if (it == fn_of_line.end()) return "";
        auto jt = it->second.find(line);
        return jt == it->second.end() ? "" : jt->second;
    };
    std::map<int, std::vector<int>> members;
    for (int x = 1; x < (int)p.parent.size(); x++) members[p.find(x)].push_back(x);
    std::map<std::pair<std::string, std::string>, int> used;
    for (auto& [s, ms] : members) {
        if (!p.is_record(s)) continue;
        int h = -1;
        std::vector<int> sorted_ms = ms;
        std::sort(sorted_ms.begin(), sorted_ms.end());
        for (int m : sorted_ms) {
            auto& w = p.site_where[m];
            std::string fq = fq_at(w.first, w.second);
            if (!fq.empty()) {
                auto rt = p.returns.find(fq);
                if (holds(rt == p.returns.end() ? BOTTOM() : rt->second, s)) {
                    h = m;
                    break;
                }
            }
        }
        if (h < 0) h = sorted_ms.front();
        home[s] = h;
        auto& w = p.site_where[h];
        std::string fq = fq_at(w.first, w.second);
        owner_fn[s] = fq;
        module[s] = w.first.empty() ? "?" : w.first;
        std::string name = name_for(s, fq);
        auto k = std::make_pair(module[s], name);
        auto it = used.find(k);
        if (it != used.end()) {
            it->second++;
            name = name + std::to_string(it->second);
        } else {
            used[k] = 1;
        }
        names.emplace_back(s, name);
        name_of[s] = name;
    }
}

std::string Records::name_for(int s, const std::string& fq) {
    std::vector<std::string> hs;
    auto hit = p.hints.find(s);
    if (hit != p.hints.end())
        for (auto& h : hit->second)
            if (!GENERIC.count(lower_s(h)) && h.size() > 2) hs.push_back(h);
    std::string stem;
    bool has_stem = false;
    if (!fq.empty()) {
        std::string fname = rsplit1(fq);
        size_t i = 0;
        while (i < fname.size() && fname[i] == '_') i++;
        fname = fname.substr(i);
        for (auto& v : VERBS)
            if (fname.rfind(v, 0) == 0) fname = fname.substr(v.size());
        if (!fname.empty() && fname != "_init__") {
            stem = camel(singular(fname));
            has_stem = true;
        }
        auto rt = p.returns.find(fq);
        bool flows_out = rt != p.returns.end() && holds(rt->second, s);
        if (flows_out && has_stem) return stem;
        if (has_stem && !hs.empty()) return stem + camel(singular(hs[0]));
        if (has_stem && hit != p.hints.end() && !hit->second.empty()) {
            const std::string& h = hit->second[0];
            return h.size() > 2 ? stem + camel(h) : stem + "Record";
        }
    }
    if (!hs.empty()) return camel(singular(hs[0]));
    return (has_stem ? stem : std::string("Anon")) + "Record";
}

bool Records::holds(const T& t, int s, int depth) {
    if (depth > 4) return false;
    if (t.kind == TK::Dict) return p.find(t.site) == s;
    if (t.kind == TK::List || t.kind == TK::Set) return holds(p.elem[p.find(t.site)], s, depth + 1);
    for (auto& a : t.args())
        if (holds(a, s, depth + 1)) return true;
    return false;
}

std::vector<RecordField> Records::fields(int s) {
    s = p.find(s);
    std::set<std::string> req;
    if (p.required[s]) req = *p.required[s];
    const OMap<T>& fs = p.fields[s];
    std::vector<std::string> order;
    auto hit = home.find(s);
    if (hit != home.end()) {
        const void* nd = p.site_node[hit->second];
        auto lk = p.literal_keys.find(nd);
        if (nd && lk != p.literal_keys.end())
            for (auto& k : lk->second)
                if (fs.has(k)) order.push_back(k);
    }
    for (auto& [k, _] : fs)
        if (std::find(order.begin(), order.end(), k) == order.end()) order.push_back(k);
    std::vector<RecordField> out;
    for (auto& k : order) out.push_back({k, *fs.get(k), req.count(k) != 0});
    return out;
}

bool Records::is_ref(int s) { return p.mutated.count(p.find(s)) != 0; }

// ------------------------------------------------------------------------------------ Context
Context::Context(Program& p_, const std::string& ip) : p(p_), records(p_), int_policy(ip) {}

std::string Context::qual_record(int s) {
    s = p.find(s);
    return records.module[s] + "." + records.name_of[s];
}

TyP Context::ty(const T& t, int line, const std::string& what) {
    switch (t.kind) {
        case TK::Int:
            if (t.fits_i64() || int_policy == "checked") return I64();
            throw Refused("int-range", what + " in [" + bstr(t.lo()) + ", " + bstr(t.hi()) + "]", line);
        case TK::Bool: return BOOLT();
        case TK::Float: return F64();
        case TK::None:
            if (what != "value" && what != "return value") throw Refused("untyped", what + " is only ever None", line);
            return VOID();
        case TK::Str: return STRT();
        case TK::Bytes: return BYTEST();
        case TK::Bytearray: return cls("py.bytearray");
        case TK::Opt: return opt_t(ty(t.args()[0], line, what));
        case TK::Tuple: {
            std::vector<TyP> a;
            for (auto& x : t.args()) a.push_back(ty(x, line, what));
            return cls("py.tuple", a);
        }
        default: break;
    }
    TK k = t.kind;
    if ((k == TK::List || k == TK::Set || k == TK::Dict) && p.poisoned_sites.count(p.find(t.site)))
        throw Refused("untyped", std::string("a ") + kind_name(k) + " also given items of no single type: " + p.poisoned_sites[p.find(t.site)], line);
    if (k == TK::List || k == TK::Set) {
        int s = p.find(t.site);
        if (p.dynamic.count(s) && p.ckind[s] != k) throw Refused("conflict", p.dynamic[s], line);
        T e = p.elem[s];
        if (e.kind == TK::Bottom) throw Refused("untyped", std::string("the items of an empty ") + kind_name(k), line);
        return cls(std::string("py.") + kind_name(k), {ty(e, line, std::string(kind_name(k)) + " item")});
    }
    if (k == TK::Dict) {
        int s = p.find(t.site);
        if (p.ckind[s] != TK::Dict) throw Refused("conflict", p.dynamic.count(s) ? p.dynamic[s] : "not a dict", line);
        if (p.is_record(s)) {
            for (auto& f : records.fields(s)) {
                if (f.type.kind == TK::Bottom) throw Refused("untyped", "record field " + py_repr(f.key), line);
                if (!f.required && f.type.kind == TK::Opt) throw Refused("conflict", "record field " + py_repr(f.key) + " both optional and None", line);
            }
            used_records.insert(s);
            return record_ty(qual_record(s), records.is_ref(s));
        }
        T kt = p.keys[s], vt = p.elem[s];
        if (kt.kind == TK::Bottom || vt.kind == TK::Bottom) throw Refused("untyped", "the keys or values of an empty dict", line);
        if (vt.kind == TK::Top) throw Refused("conflict", "dict values: " + vt.why(), line);
        return cls("py.dict", {ty(kt, line, "dict key"), ty(vt, line, "dict value")});
    }
    if (k == TK::Inst) {
        if (t.name().rfind("exc:", 0) == 0) {
            std::string n = t.name().substr(4);
            auto pos = n.find("struct.error");
            if (pos != std::string::npos) n.replace(pos, 12, "StructError");
            return cls("py." + n);
        }
        Class** c = p.classes.get(t.name());
        if (c && !(*c)->exception.empty()) return cls_flag(t.name(), "exception");
        return cls_flag(t.name(), "ref");
    }
    if (k == TK::Closure) throw Refused("dynamic", "a nested function used as a value", line);
    if (k == TK::Variant) {   // (new) py.variant<A, B ...>: its alternatives are its tags
        std::vector<TyP> alts;
        for (auto& a : t.args()) alts.push_back(ty(a, line, what));
        return cls("py.variant", alts);
    }
    if (k == TK::Extern && t.name() == "file") return cls("py.file");
    if (k == TK::Extern && p.extended) {   // (new) the extensions' library objects
        const std::string& nm = t.name();
        if (nm.rfind("file:", 0) == 0) return cls("py.file");
        if (nm.rfind("re.Pattern", 0) == 0) return cls("py.re.Pattern");
        if (nm.rfind("re.Match", 0) == 0) return cls("py.re.Match");
        if (nm == "hash") return cls("py.hashlib.Hash");
    }
    if (k == TK::Iter) {
        if (what != "value") throw Refused("syntax", "an iterator as a " + what, line);
        return cls("py.iter", {ty(t.args()[0], line, "value")});
    }
    if (k == TK::Top) {
        const std::string& why = t.why();
        auto c = why.find(':');
        static const std::set<std::string> CL = {"dynamic", "stdlib", "extern", "syntax", "unresolved", "int-range", "conflict", "untyped",
                                                 "calls-refused", "frontend-error"};
        if (c != std::string::npos && CL.count(why.substr(0, c))) throw Refused(why.substr(0, c), why.substr(c + 1), line);
        throw Refused("conflict", why, line);
    }
    if (k == TK::Bottom) throw Refused("untyped", what, line);
    throw Refused("conflict", std::string("a ") + kind_name(k) + " as a value", line);
}

static bool is_ref_type(const TyP& t) { return t && t->kind == "class" && t->has_ref && t->ref; }

// ------------------------------------------------------------------------------------ FnLower
class FnLower {
public:
    FnLower(Context& ctx_, Fn* f_, Eval& ev_) : ctx(ctx_), p(ctx_.p), f(f_), ev(ev_), src(f_->module->source) {}
    Context& ctx;
    Program& p;
    Fn* f;
    Eval& ev;
    const std::string& src;
    std::vector<IRP> pre;
    int tmp = 0;
    std::map<const void*, std::vector<std::string>> decl_at;
    std::map<const void*, std::string> decl_init;
    std::map<const void*, std::set<std::string>> decl_tuple;
    std::map<std::string, TyP> local_types;
    std::set<std::string> declared;
    std::map<std::string, bool> loopvars;
    std::set<std::string> calls, classes_made;
    std::set<std::pair<std::string, std::string>> globals_used;
    TyP ret;
    const void* in_cond = nullptr;

    // -- types and positions
    T Tof(AnyNode node) {
        auto it = ev.types.find(node.p);
        if (it == ev.types.end()) {
            std::string tn = type_name(node);
            throw Refused("untyped", "no type reached " + lower_s(tn), lineno(node));
        }
        return it->second;
    }
    TyP ty(const T& t, AnyNode node, const std::string& what = "value") { return ctx.ty(t, lineno(node), what); }
    const Res* R(const void* node) {
        auto it = ev.res.find(node);
        return it == ev.res.end() ? nullptr : &it->second;
    }
    SymP lsym(const std::string& name, const TyP& t) { return sym("local", name, f->qname, false, t); }
    IRP local(const std::string& name, const TyP& t, IRP init = nullptr, std::vector<std::string> mods = {}, int line = 0) {
        auto n = node("Local", line);
        n->sym = lsym(name, t);
        n->type = t;
        n->init = std::move(init);
        n->mods = std::move(mods);
        return n;
    }
    IRP exprstmt(IRP e, int line = 0) {
        auto n = node("ExprStmt", line);
        n->expr = std::move(e);
        return n;
    }
    IRP assign_e(std::optional<std::string> op, IRP target, IRP value, TyP t) {
        auto n = node("Assign");
        n->op = std::move(op);
        n->target = std::move(target);
        n->value = std::move(value);
        n->type = std::move(t);
        return n;
    }
    IRP binary(const std::string& op, IRP l, IRP r, TyP t, int line = 0) {
        auto n = node("Binary", line);
        n->op = op;
        n->left = std::move(l);
        n->right = std::move(r);
        n->type = std::move(t);
        return n;
    }
    IRP unary(const std::string& op, IRP x, TyP t, int line = 0) {
        auto n = node("Unary", line);
        n->op = op;
        n->x = std::move(x);
        n->postfix = false;
        n->type = std::move(t);
        return n;
    }
    IRP paren_n(IRP x, TyP t) {
        auto n = node("Paren");
        n->x = std::move(x);
        n->type = std::move(t);
        return n;
    }
    IRP cast_n(IRP x, TyP t) {
        auto n = node("Cast");
        n->x = std::move(x);
        n->type = std::move(t);
        return n;
    }
    IRP block_n(std::vector<IRP> stmts) {
        auto n = node("Block");
        n->stmts = std::move(stmts);
        return n;
    }
    IRP typename_n(const std::string& c) {
        auto n = node("TypeName");
        n->type = cls(c);
        return n;
    }
    IRP select_n(IRP obj, SymP s, TyP t, int line = 0) {
        auto n = node("Select", line);
        n->obj = std::move(obj);
        n->sym = std::move(s);
        n->type = std::move(t);
        return n;
    }

    // -- the function
    IRP lower() {
        for (auto& i : ev.issues) throw Refused(i.cls, i.detail, i.line);
        auto cr = p.crashes.find(f->qname);
        if (cr != p.crashes.end()) throw Refused("frontend-error", cr->second, f->node->pos.lineno);
        std::vector<IRP> params;
        size_t skip = (f->kind == "method" || f->kind == "property") ? 1 : 0;
        if (f->kind == "classmethod") throw Refused("dynamic", "classmethod", f->node->pos.lineno);
        std::vector<std::string> names(f->params.begin() + std::min(skip, f->params.size()), f->params.end());
        names.insert(names.end(), f->kwonly.begin(), f->kwonly.end());
        for (auto& name : names) {
            T t = p.param_type(f, name);
            if (t.kind == TK::Bottom) {
                auto got = p.param_poison.find({f->qname, name});
                if (got != p.param_poison.end()) {
                    const std::string& why = got->second.second;
                    auto c = why.find(':');
                    std::string cls_ = why.substr(0, c);
                    std::string rest = c == std::string::npos ? why : why.substr(c + 1);
                    throw Refused("untyped", "parameter " + name + " (its callers pass values of no type: " + cls_ + ":" + trunc(rest, 40) +
                                                 " in " + rsplit1(got->second.first) + ")",
                                  f->node->pos.lineno);
                }
                throw Refused("untyped", "parameter " + name + " (no annotation, no call site)", f->node->pos.lineno);
            }
            TyP it = ctx.ty(t, f->node->pos.lineno, "parameter " + name);
            local_types[name] = it;
            declared.insert(name);
            std::vector<std::string> mods;
            const A::Expr* const* d = f->defaults.get(name);
            if (d && is_const(*d)) mods.push_back("default=" + default_json(cast<Constant>(*d)->value));
            auto prm = node("Param");
            prm->sym = sym("param", name, f->qname, false, it);
            prm->type = it;
            prm->mods = mods;
            params.push_back(prm);
        }
        auto rt = p.returns.find(f->qname);
        T ret_t = rt == p.returns.end() ? NONE() : rt->second;
        if (ret_t.kind == TK::Bottom) ret_t = NONE();
        if (f->name == "__init__") ret = VOID();
        else ret = ctx.ty(ret_t, f->node->pos.lineno, "return value");
        plan_locals();
        auto nb = p.native_bound.find(f->qname);
        IRP body = nb != p.native_bound.end() ? native_body(nb->second, p.native_data[f->qname]) : block(body_stmts(f->node->body));
        std::string kind = !f->cls ? "function" : (f->name == "__init__" ? "ctor" : "method");
        std::vector<std::string> mods;
        if (f->kind == "static") mods.push_back("static");
        if (f->kind == "property") mods.push_back("property");
        std::vector<TyP> ptypes;
        for (auto& x : params) ptypes.push_back(x->type);
        auto md = node("MethodDecl", f->node->pos.lineno);
        md->sym = sym(!f->cls ? "function" : (kind == "ctor" ? "ctor" : "method"), f->name, f->cls ? f->cls->qname : f->module->name,
                      f->kind == "static", nullptr, ptypes, ret);
        md->name = f->name;
        md->dkind = kind;
        md->params = params;
        md->ret = ret;
        md->body = body;
        md->mods = mods;
        std::string doc;
        if (docstring(f->node->body, doc)) md->comments.push_back(doc);
        return md;
    }

    static std::string default_json(const ConstValue& v) {
        py::V x;
        switch (v.kind) {
            case ConstValue::Kind::None: x = py::V(); break;
            case ConstValue::Kind::True: x = py::V(true); break;
            case ConstValue::Kind::False: x = py::V(false); break;
            case ConstValue::Kind::Int: x = v.fits_i64 ? py::V((long long)v.i64) : py::V(v.text); break;
            case ConstValue::Kind::Float: x = py::V(v.f); break;
            case ConstValue::Kind::Str: x = py::V(v.text); break;
            case ConstValue::Kind::Bytes: {
                x = py::V::dict();
                x.set("bytes", py::V(latin1_to_utf8(v.text)));
                break;
            }
            default: x = py::V(); break;
        }
        if (v.kind == ConstValue::Kind::Int && !v.fits_i64) return v.text;
        return py::json_dump(x);
    }

    std::vector<A::Stmt*> body_stmts(const std::vector<A::Stmt*>& stmts) {
        if (!stmts.empty())
            if (auto* e = as<ExprStmt>(stmts[0]))
                if (is_str_const(e->value)) return std::vector<A::Stmt*>(stmts.begin() + 1, stmts.end());
        return stmts;
    }

    // -- where each local is declared
    static void own_nodes(const A::Stmt* st, std::vector<AnyNode>& out) {
        std::vector<AnyNode> todo, kids;
        // the root: its children outside its statement lists and handlers
        {
            std::vector<AnyNode> all;
            child_nodes(st, all);
            std::set<const void*> skip;
            for (auto* l : stmt_lists(st, false))
                for (auto* s : *l) skip.insert(s);
            if (st->kind == StmtKind::Try || st->kind == StmtKind::TryStar)
                for (auto* h : cast<pyfront::Try>(st)->handlers) skip.insert(h);
            out.push_back(AnyNode(st));
            for (auto& k : all)
                if (!skip.count(k.p)) todo.push_back(k);
        }
        while (!todo.empty()) {
            AnyNode n = todo.back();
            todo.pop_back();
            out.push_back(n);
            kids.clear();
            child_nodes(n, kids);
            todo.insert(todo.end(), kids.begin(), kids.end());
        }
    }

    struct Use {
        std::vector<std::tuple<const void*, int, const A::Stmt*>> path;
        const A::Stmt* st;
    };

    void plan_locals() {
        OMap<std::vector<Use>> occ;
        std::map<std::string, std::vector<const A::Expr*>> stores;
        std::set<std::string> loads;
        std::function<void(const std::vector<A::Stmt*>&, const std::vector<std::tuple<const void*, int, const A::Stmt*>>&)> visit =
            [&](const std::vector<A::Stmt*>& stmts, const std::vector<std::tuple<const void*, int, const A::Stmt*>>& path) {
                for (size_t i = 0; i < stmts.size(); i++) {
                    const A::Stmt* st = stmts[i];
                    auto here = path;
                    here.emplace_back(&stmts, (int)i, st);
                    std::vector<AnyNode> own;
                    own_nodes(st, own);
                    for (auto& n : own) {
                        auto* nm = as<pyfront::Name>(n.expr());
                        if (nm && ev.locals.count(nm->id) && !declared.count(nm->id)) {
                            occ[nm->id].push_back(Use{here, st});
                            if (nm->ctx == ExprCtx::Store) stores[nm->id].push_back(nm);
                            else loads.insert(nm->id);
                        }
                    }
                    if (p.extended && (st->kind == StmtKind::FunctionDef || st->kind == StmtKind::AsyncFunctionDef) &&
                        p.nested_by_node.count(st)) {
                        // (new) the names of ours a closure reads are used where it is defined
                        Fn* nf = p.nested_by_node[st];
                        const auto& inner = p.fn_locals(nf);
                        std::set<std::string> seen_here;
                        for (auto& x : walk(AnyNode(st))) {
                            auto* nm = as<pyfront::Name>(x.expr());
                            if (nm && ev.locals.count(nm->id) && !inner.count(nm->id) && !declared.count(nm->id) && !seen_here.count(nm->id)) {
                                seen_here.insert(nm->id);
                                occ[nm->id].push_back(Use{here, st});
                                loads.insert(nm->id);
                            }
                        }
                        continue;
                    }
                    // body, orelse, finalbody; then the handlers
                    std::vector<const std::vector<A::Stmt*>*> lists;
                    switch (st->kind) {
                        case StmtKind::FunctionDef:
                        case StmtKind::AsyncFunctionDef: lists.push_back(&cast<FunctionDef>(st)->body); break;
                        case StmtKind::ClassDef: lists.push_back(&cast<ClassDef>(st)->body); break;
                        case StmtKind::For:
                        case StmtKind::AsyncFor: lists = {&cast<pyfront::For>(st)->body, &cast<pyfront::For>(st)->orelse}; break;
                        case StmtKind::While: lists = {&cast<pyfront::While>(st)->body, &cast<pyfront::While>(st)->orelse}; break;
                        case StmtKind::If: lists = {&cast<pyfront::If>(st)->body, &cast<pyfront::If>(st)->orelse}; break;
                        case StmtKind::With:
                        case StmtKind::AsyncWith: lists.push_back(&cast<With>(st)->body); break;
                        case StmtKind::Try:
                        case StmtKind::TryStar: {
                            auto* t = cast<pyfront::Try>(st);
                            lists = {&t->body, &t->orelse, &t->finalbody};
                            break;
                        }
                        default: break;
                    }
                    for (auto* l : lists)
                        if (!l->empty()) visit(*l, here);
                    if (st->kind == StmtKind::Try || st->kind == StmtKind::TryStar)
                        for (auto* h : cast<pyfront::Try>(st)->handlers) visit(h->body, here);
                }
            };
        visit(f->node->body, {});
        for (auto& [name, uses] : occ) {
            std::vector<T> ts;
            for (auto* n : stores[name]) {
                auto it = ev.types.find(n);
                if (it != ev.types.end()) ts.push_back(it->second);
            }
            if (ts.empty()) continue;
            T j = ts[0];
            for (size_t i = 1; i < ts.size(); i++) j = p.join(j, ts[i]);
            local_types[name] = ty(j, stores[name][0], "local " + name);
            std::vector<const A::For*> loop_targets;
            for (auto& u : uses)
                if (u.st->kind == StmtKind::For || u.st->kind == StmtKind::AsyncFor) {
                    auto* fs = cast<pyfront::For>(u.st);
                    auto tn = target_names(fs->target);
                    if (std::find(tn.begin(), tn.end(), name) != tn.end()) loop_targets.push_back(fs);
                }
            if (!loads.count(name) && !stores[name].empty()) {
                bool all = true;
                for (auto* n : stores[name]) {
                    bool any = false;
                    for (auto* lt : loop_targets)
                        for (auto& x : walk(lt->target))
                            if (x.p == n) any = true;
                    if (!any) all = false;
                }
                if (all) {
                    loopvars[name] = true;
                    continue;
                }
            }
            std::vector<std::vector<std::pair<const void*, int>>> paths;
            for (auto& u : uses) {
                std::vector<std::pair<const void*, int>> pp;
                for (auto& [lid, i, _s] : u.path) pp.emplace_back(lid, i);
                paths.push_back(pp);
            }
            size_t L = 0;
            for (;;) {
                bool all_long = true;
                for (auto& pp : paths)
                    if (pp.size() <= L) all_long = false;
                if (!all_long) break;
                bool same = true;
                for (auto& pp : paths)
                    if (pp[L] != paths[0][L]) same = false;
                if (!same) break;
                L++;
            }
            auto& first_path = uses[0].path;
            bool all_long = true, one_list = true;
            for (auto& pp : paths)
                if (pp.size() <= L) all_long = false;
            if (all_long)
                for (auto& pp : paths)
                    if (pp[L].first != paths[0][L].first) one_list = false;
            const A::Stmt* anchor;
            if (all_long && one_list) anchor = std::get<2>(first_path[L]);
            else anchor = std::get<2>(first_path[L == 0 ? first_path.size() - 1 : L - 1]);
            if (anchor->kind == StmtKind::For || anchor->kind == StmtKind::AsyncFor) {
                auto* fs = cast<pyfront::For>(anchor);
                auto tn = target_names(fs->target);
                // (lower.py also asks that every use lie inside the loop, but its test -- any node of
                // the loop that `is` the use or the loop -- always holds: the loop declares it)
                if (std::find(tn.begin(), tn.end(), name) != tn.end()) continue;
            }
            const A::Stmt* first_stmt = uses[0].st;
            auto* fa = as<pyfront::Assign>(first_stmt);
            auto value_names = [&](const A::Expr* v) {
                for (auto& x : walk(v))
                    if (auto* nm = as<pyfront::Name>(x.expr()))
                        if (nm->id == name) return true;
                return false;
            };
            if (anchor == first_stmt && fa && fa->targets.size() == 1 && as<pyfront::Name>(fa->targets[0]) &&
                cast<pyfront::Name>(fa->targets[0])->id == name && !value_names(fa->value)) {
                decl_init[first_stmt] = name;
            } else if (anchor == first_stmt && fa && fa->targets.size() == 1 && as<pyfront::Tuple>(fa->targets[0]) &&
                       [&] {
                           auto tn = target_names(fa->targets[0]);
                           return std::find(tn.begin(), tn.end(), name) != tn.end();
                       }() &&
                       !value_names(fa->value)) {
                decl_tuple[first_stmt].insert(name);
            } else {
                decl_at[anchor].push_back(name);
            }
        }
    }

    // -- statements
    // a function a native format answers (answers.cc): its body is the question asked --
    //   return py::native::ask<R>("pe.export_table", this->data, args...);
    // the bytes first (self.<attribute>, the data_arg-th argument, or none), then its other arguments
    IRP native_body(const std::string& question, const std::string& data) {
        int line = f->node->pos.lineno;
        size_t self = (f->cls && (f->kind == "method" || f->kind == "property")) ? 1 : 0;
        std::vector<IRP> args;
        int data_arg = -1;
        if (auto ai = p.native_args_of.find(f->qname); ai != p.native_args_of.end()) {   // args_of: f's bytes and place, inlined
            const A::Expr* d = ai->second.data;   // (None: a question of no bytes, its place the file's path)
            args.push_back(d->kind == ExprKind::Constant && is_none_const(d) ? mk_pyfn("native.no_data", {}, BYTEST(), nullptr, line) : expr(d));
            for (auto* e : ai->second.place) args.push_back(expr(e));
            for (size_t i = 1; i < f->params.size(); i++) args.push_back(name_expr(f->params[i]));
            auto r = node("Return", line);
            r->expr = mk_pyfn("native.ask", args, ret, nullptr, line, {py::V(question)});
            return block_n({r});
        }
        if (data.rfind("self:", 0) == 0) {
            if (!self) throw Refused("dynamic", "a native question of self's bytes outside a method", line);
            auto th = node("This", line);
            th->type = cls_flag(f->cls->qname, "ref");
            args.push_back(select_n(th, sym("field", data.substr(5), f->cls->qname, false, BYTEST()), BYTEST(), line));
        } else if (data.rfind("arg:", 0) == 0) {
            data_arg = std::stoi(data.substr(4));
            if (data_arg < 0 || self + data_arg >= f->params.size()) throw Refused("dynamic", "a native question's data_arg", line);
            args.push_back(name_expr(f->params[self + data_arg]));
        } else {
            args.push_back(mk_pyfn("native.no_data", {}, BYTEST(), nullptr, line));
        }
        std::vector<py::V> typeargs = {py::V(question)};
        auto wi = p.native_where.find(f->qname);
        if (wi != p.native_where.end()) {   // ask(q, _where): _where(self)'s items follow the bytes
            Fn* wf = p.any_fn(wi->second);
            calls.insert(wf->qname);
            TyP wt = ctx.ty(p.ret(wf->qname), line, "the place " + wf->name + " gives");
            auto th = node("This", line);
            th->type = cls_flag(f->cls->qname, "ref");
            auto c = node("Call", line);
            c->sym = sym("function", wf->name, wf->module->name, true, nullptr, {}, wt);
            c->args = {th};
            c->type = wt;
            args.push_back(c);
            typeargs.push_back(py::V("where"));
        }
        for (size_t i = self; i < f->params.size(); i++)
            if ((int)(i - self) != data_arg) args.push_back(name_expr(f->params[i]));
        auto r = node("Return", line);
        r->expr = mk_pyfn("native.ask", args, ret, nullptr, line, typeargs);
        return block_n({r});
    }

    IRP block(const std::vector<A::Stmt*>& stmts) {
        std::vector<IRP> out;
        for (auto* st : stmts) {
            auto it = decl_at.find(st);
            if (it != decl_at.end())
                for (auto& name : it->second) {
                    TyP t = local_types[name];
                    out.push_back(local(name, t));
                    declared.insert(name);
                }
            pre.clear();
            std::vector<IRP> got = stmt(st);
            out.insert(out.end(), pre.begin(), pre.end());
            pre.clear();
            out.insert(out.end(), got.begin(), got.end());
        }
        return block_n(out);
    }

    std::vector<IRP> stmt(const A::Stmt* st) {
        int line = st->pos.lineno;
        switch (st->kind) {
            case StmtKind::Pass: return {};
            case StmtKind::Expr: {
                auto* e = cast<ExprStmt>(st);
                if (gen_mode && e->value->kind == ExprKind::Yield) return gen_yield(cast<pyfront::Yield>(e->value), line);
                if (auto* c = as<Constant>(e->value)) {
                    if (c->value.kind == ConstValue::Kind::Str) {
                        auto cm = node("Comment");
                        cm->text = c->value.text;
                        return {cm};
                    }
                    return {};
                }
                return {exprstmt(expr(e->value), line)};
            }
            case StmtKind::Assign: return s_Assign(cast<pyfront::Assign>(st));
            case StmtKind::AnnAssign: {
                auto* a = cast<AnnAssign>(st);
                if (!a->value) return {};
                return store(a->target, expr(a->value), st);
            }
            case StmtKind::AugAssign: return {s_AugAssign(cast<AugAssign>(st))};
            case StmtKind::Return: return s_Return(cast<pyfront::Return>(st));
            case StmtKind::Raise: {
                auto* r = cast<Raise>(st);
                auto n = node("Throw", line);
                if (r->exc) n->expr = expr(r->exc);
                return {n};
            }
            case StmtKind::Assert: {
                auto* a = cast<pyfront::Assert>(st);
                IRP c = cond(a->test);
                std::vector<IRP> args;
                if (a->msg) args.push_back(expr(a->msg));
                if (!args.empty() && !ty_eq(args[0]->type, STRT())) args = {mk_pyfn("str", args, STRT())};
                auto exc = node("New");
                exc->sym = sym("ctor", "AssertionError", PY);
                exc->args = args;
                exc->type = cls("py.AssertionError");
                auto th = node("Throw");
                th->expr = exc;
                auto i = node("If", line);
                i->cond = unary("!", paren_n(c, BOOLT()), BOOLT());
                i->then = block_n({th});
                return {i};
            }
            case StmtKind::If: return {s_If(cast<pyfront::If>(st))};
            case StmtKind::While: {
                auto* w = cast<pyfront::While>(st);
                auto saved = pre;
                pre.clear();
                IRP c = cond(w->test);
                if (!pre.empty()) throw Refused("syntax", "a while condition that needs statements", line);
                pre = saved;
                auto n = node("While", line);
                n->cond = c;
                n->body = block(w->body);
                return {n};
            }
            case StmtKind::Break: return {node("Break", line)};
            case StmtKind::Continue: return {node("Continue", line)};
            case StmtKind::For:
            case StmtKind::AsyncFor: return s_For(cast<pyfront::For>(st));
            case StmtKind::Try: return {s_Try(cast<pyfront::Try>(st))};
            case StmtKind::With: return s_With(cast<With>(st));
            case StmtKind::Import:
            case StmtKind::ImportFrom: return {};
            case StmtKind::FunctionDef: return {s_FunctionDef(cast<FunctionDef>(st))};
            default: break;
        }
        throw Refused("syntax", lower_s(type_name(st)), line);
    }

    std::vector<IRP> s_Assign(const A::Assign* st) {
        int line = st->pos.lineno;
        if (st->targets.size() != 1) {   // a = b = v: v once, then each target left to right, as Python
            IRP value = expr(st->value);
            std::string tn = new_tmp("value");
            local_types[tn] = value->type;
            declared.insert(tn);
            std::vector<IRP> out = {local(tn, value->type, value, {}, line)};
            for (auto* t : st->targets)
                for (auto& x : store(t, name_expr(tn), st)) out.push_back(x);
            return out;
        }
        const A::Expr* tgt = st->targets[0];
        auto* tn = as<pyfront::Name>(tgt);
        auto vk = st->value->kind;
        if ((vk == ExprKind::ListComp || vk == ExprKind::SetComp || vk == ExprKind::DictComp) && tn) {
            bool mentions = false;
            for (auto& x : walk(st->value))
                if (auto* nm = as<pyfront::Name>(x.expr()))
                    if (nm->id == tn->id) mentions = true;
            if (!mentions) return comp_into(st, tn, st->value);
        }
        if (is_record_literal(st->value) && tn && decl_init.count(st)) return {record_builder(st, tn)};
        if (auto d = direct_unpack(tgt, st)) return *d;
        IRP value = expr(st->value);
        return store(tgt, value, st);
    }

    // a, self.b = x, y as a = x; self.b = y when that is the same program: the targets are names or
    // attributes of names, and the items after the first are names, constants and attributes, none
    // of them a target assigned before it
    std::optional<std::vector<IRP>> direct_unpack(const A::Expr* tgt, const A::Assign* st) {
        if ((tgt->kind != ExprKind::Tuple && tgt->kind != ExprKind::List) || st->value->kind != ExprKind::Tuple) return std::nullopt;
        const auto& elts = tgt->kind == ExprKind::Tuple ? cast<pyfront::Tuple>(tgt)->elts : cast<pyfront::List>(tgt)->elts;
        const auto& vals = cast<pyfront::Tuple>(st->value)->elts;
        if (elts.size() != vals.size() || elts.size() < 2) return std::nullopt;
        std::set<std::string> names, attrs;   // assigned so far: x, and base.attr
        for (size_t i = 0; i < elts.size(); i++) {
            if (vals[i]->kind == ExprKind::Starred) return std::nullopt;
            if (i > 0)
                for (auto& x : walk(vals[i])) {
                    const A::Expr* e = x.expr();
                    if (!e) continue;
                    // nothing that can call, raise or change anything: an exception there would find the
                    // earlier targets assigned, where Python assigns none
                    switch (e->kind) {
                        case ExprKind::Name: case ExprKind::Constant: case ExprKind::Attribute: case ExprKind::Tuple:
                            break;
                        case ExprKind::UnaryOp:
                            if (cast<pyfront::UnaryOp>(e)->operand->kind != ExprKind::Constant) return std::nullopt;
                            break;
                        default: return std::nullopt;
                    }
                    if (auto* nm = as<pyfront::Name>(e))
                        if (names.count(nm->id)) return std::nullopt;
                    if (auto* at = as<Attribute>(e))
                        if (auto* b = as<pyfront::Name>(at->value))
                            if (attrs.count(b->id + "." + at->attr)) return std::nullopt;
                }
            if (auto* nm = as<pyfront::Name>(elts[i])) names.insert(nm->id);
            else if (auto* at = as<Attribute>(elts[i])) {
                auto* b = as<pyfront::Name>(at->value);
                if (!b) return std::nullopt;
                attrs.insert(b->id + "." + at->attr);
            } else return std::nullopt;
        }
        int line = st->pos.lineno;
        std::vector<IRP> out;
        for (size_t i = 0; i < elts.size(); i++) {
            IRP v = expr(vals[i]);
            if (auto* nm = as<pyfront::Name>(elts[i])) {
                auto lt = local_types.find(nm->id);
                if (lt == local_types.end()) throw Refused("untyped", "local " + nm->id, line);
                v = coerce(v, lt->second);
                if (!declared.count(nm->id)) {
                    declared.insert(nm->id);
                    out.push_back(local(nm->id, lt->second, v, {}, line));
                } else out.push_back(exprstmt(assign_e(std::nullopt, mk_name(lsym(nm->id, lt->second), lt->second), v, lt->second), line));
            } else
                for (auto& x : store(elts[i], v, st)) out.push_back(x);
        }
        return out;
    }

    // ---- (new) generators: a class deriving py.generator<T>; next() resumes after each yield
    bool gen_mode = false;
    int gen_points = 0;
    TyP gen_elem;
    std::string gen_class;

    IRP this_sel(const std::string& field, const TyP& t) {
        auto th = node("This");
        th->type = cls_flag(gen_class, "ref");
        return select_n(th, sym("field", field, gen_class, false, t), t);
    }
    IRP state_is(int k) { return binary("==", this_sel("state", I64()), mk_lit(py::V(k), I64()), BOOLT()); }
    IRP set_state(int k) { return exprstmt(assign_e(std::nullopt, this_sel("state", I64()), mk_lit(py::V(k), I64()), I64())); }
    IRP label(const std::string& l) {
        auto n = node("Labeled");
        n->text = l;
        n->body = node("Empty");
        return n;
    }

    std::vector<IRP> gen_yield(const pyfront::Yield* y, int line) {
        int k = ++gen_points;
        IRP v = y->value ? coerce(expr(y->value), gen_elem) : mk_lit_none(gen_elem);
        auto r = node("Return", line);
        r->expr = mk_pyfn("some", {v}, opt_t(gen_elem));
        return {set_state(k), r, label("resume_" + std::to_string(k))};
    }

    // the generator's body, rewritten for resumption: locals and parameters are fields; a loop a
    // resume jumps into keeps its state in fields too
    bool has_label(const IRP& n) {
        bool found = false;
        ir_walk(n, [&](const IRP& x) {
            if (x->k == "Labeled") found = true;
        });
        return found;
    }
    void gen_rewrite(IRP& n, const std::map<std::string, TyP>& fields, int& loop_id) {
        if (!n) return;
        if (n->k == "Name" && n->sym && (n->sym->kind == "local" || n->sym->kind == "param") && fields.count(n->sym->name) &&
            n->sym->owner && *n->sym->owner == f->qname) {
            n = this_sel(n->sym->name, n->type);
            return;
        }
        if (n->k == "Block") {
            std::vector<IRP> out;
            for (auto& s : n->stmts) {
                if (s->k == "Local") {   // a declaration: an assignment to its field
                    if (s->init) {
                        IRP init = s->init;
                        gen_rewrite(init, fields, loop_id);
                        out.push_back(exprstmt(assign_e(std::nullopt, this_sel(s->sym->name, s->type), init, s->type)));
                    }
                    continue;
                }
                if (s->k == "ForEach" && has_label(s)) {   // a loop a resume jumps into: an index loop over fields
                    TyP it = s->iterable->type;
                    if (!(it && it->kind == "class" && (it->name == "py.list" || it->name == "py.str" || it->name == "py.bytes" ||
                                                        it->name == "py.bytearray" || it->name == "py.tuple")))
                        throw Refused("syntax", "a yield inside a loop over a " + ty_repr(it), s->line);
                    if (s->var->mods.size()) throw Refused("syntax", "a yield inside a loop that unpacks", s->line);
                    int id = ++loop_id;
                    std::string seq = "seq_" + std::to_string(id), idx = "index_" + std::to_string(id);
                    gen_fields.emplace_back(seq, it);
                    gen_fields.emplace_back(idx, I64());
                    IRP iterable = s->iterable;
                    gen_rewrite(iterable, fields, loop_id);
                    out.push_back(exprstmt(assign_e(std::nullopt, this_sel(seq, it), iterable, it)));
                    auto fo = node("For", s->line);
                    fo->inits = {exprstmt(assign_e(std::nullopt, this_sel(idx, I64()), mk_lit(py::V(0), I64()), I64()))};
                    fo->cond = binary("<", this_sel(idx, I64()), mk_pyfn("builtins.len", {this_sel(seq, it)}, I64()), BOOLT());
                    fo->update = {unary("++", this_sel(idx, I64()), I64())};
                    TyP vt = s->var->type;
                    auto ix = node("Index");
                    ix->arr = this_sel(seq, it);
                    ix->index = this_sel(idx, I64());
                    ix->type = vt;
                    IRP body = s->body;
                    gen_rewrite(body, fields, loop_id);
                    body->stmts.insert(body->stmts.begin(), exprstmt(assign_e(std::nullopt, this_sel(s->var->sym->name, vt), ix, vt)));
                    fo->body = body;
                    out.push_back(fo);
                    continue;
                }
                if (s->k == "ForEach" && !has_label(s)) {   // its own variables, copied to their fields each pass
                    IRP body = s->body;
                    IRP iterable = s->iterable;
                    gen_rewrite(iterable, fields, loop_id);
                    gen_rewrite(body, fields, loop_id);
                    std::string names = s->var->sym->name;
                    std::vector<IRP> pro;
                    size_t a = 0;
                    while (a <= names.size()) {
                        size_t b = names.find(',', a);
                        std::string nm = names.substr(a, b == std::string::npos ? std::string::npos : b - a);
                        auto fi = fields.find(nm);
                        if (fi != fields.end())
                            pro.push_back(exprstmt(assign_e(std::nullopt, this_sel(nm, fi->second),
                                                            mk_name(sym("local", nm, std::string("<loop>"), false, fi->second), fi->second), fi->second)));
                        if (b == std::string::npos) break;
                        a = b + 1;
                    }
                    body->stmts.insert(body->stmts.begin(), pro.begin(), pro.end());
                    s->body = body;
                    s->iterable = iterable;
                    out.push_back(s);
                    continue;
                }
                if (s->k == "For") {   // its counter is a field: no declaration is jumped over
                    for (auto& in : s->inits)
                        if (in->k == "Local") {
                            IRP init = in->init;
                            gen_rewrite(init, fields, loop_id);
                            in = exprstmt(assign_e(std::nullopt, this_sel(in->sym->name, in->type), init, in->type));
                        }
                }
                if ((s->k == "Try" || s->k == "Block") && has_label(s) && s->k == "Try") throw Refused("syntax", "a yield inside try", s->line);
                if (s->k == "Return" && !(s->expr && s->expr->k == "Call" && s->expr->sym && s->expr->sym->name == "some")) {   // the generator ends
                    out.push_back(set_state(-1));
                    auto r = node("Return", s->line);
                    r->expr = mk_lit_none(opt_t(gen_elem));
                    out.push_back(r);
                    continue;
                }
                IRP x = s;
                gen_rewrite(x, fields, loop_id);
                out.push_back(x);
            }
            n->stmts = out;
            return;
        }
        if (n->k == "Return" && n->expr && n->expr->k == "Call" && n->expr->sym && n->expr->sym->name == "some") {
            gen_rewrite(n->expr->args[0], fields, loop_id);
            return;
        }
        for (IRP* slot : {&n->target, &n->x, &n->left, &n->right, &n->cond, &n->then, &n->orelse, &n->body, &n->init, &n->iterable,
                          &n->arr, &n->index, &n->obj, &n->value, &n->expr})
            if (*slot) gen_rewrite(*slot, fields, loop_id);
        for (auto* lst : {&n->args, &n->stmts, &n->parts, &n->update, &n->inits})
            for (auto& x : *lst) gen_rewrite(x, fields, loop_id);
    }
    std::vector<std::pair<std::string, TyP>> gen_fields;

    // the generator: its class, and the function that makes one
    std::vector<IRP> lower_generator() {
        int line = f->node->pos.lineno;
        for (auto& i : ev.issues) throw Refused(i.cls, i.detail, i.line);
        if (f->kind != "function") throw Refused("syntax", "a generator method", line);
        auto rt = p.returns.find(f->qname);
        T it = rt == p.returns.end() ? BOTTOM() : rt->second;
        if (it.kind != TK::Iter) throw Refused("untyped", "the items of a generator", line);
        gen_elem = ctx.ty(it.args()[0], line, "yielded value");
        gen_class = f->module->name + "." + camel(f->name) + "Generator";
        gen_mode = true;
        std::vector<IRP> params;
        for (auto& name : f->params) {
            T t = p.param_type(f, name);
            if (t.kind == TK::Bottom) throw Refused("untyped", "parameter " + name + " (no annotation, no call site)", line);
            TyP pt = ctx.ty(t, line, "parameter " + name);
            local_types[name] = pt;
            declared.insert(name);
            auto prm = node("Param");
            prm->sym = sym("param", name, f->qname, false, pt);
            prm->type = pt;
            params.push_back(prm);
        }
        if (!f->kwonly.empty()) throw Refused("syntax", "a generator with keyword-only parameters", line);
        ret = VOID();
        plan_locals();
        IRP body = block(body_stmts(f->node->body));
        // every name of the function is a field
        std::map<std::string, TyP> fields;
        for (auto& [name, t] : local_types) fields[name] = t;
        int loop_id = 0;
        gen_rewrite(body, fields, loop_id);
        std::vector<IRP> stmts;
        for (int k = 1; k <= gen_points; k++) {   // resume where the last yield left off
            auto g = node("Goto");
            g->text = "resume_" + std::to_string(k);
            auto i = node("If");
            i->cond = state_is(k);
            i->then = block_n({g});
            stmts.push_back(i);
        }
        auto done = node("If");
        done->cond = binary("<", this_sel("state", I64()), mk_lit(py::V(0), I64()), BOOLT());
        auto r0 = node("Return");
        r0->expr = mk_lit_none(opt_t(gen_elem));
        done->then = block_n({r0});
        stmts.push_back(done);
        stmts.insert(stmts.end(), body->stmts.begin(), body->stmts.end());
        stmts.push_back(set_state(-1));
        auto r1 = node("Return");
        r1->expr = mk_lit_none(opt_t(gen_elem));
        stmts.push_back(r1);
        auto next = node("MethodDecl", line);
        next->sym = sym("method", "next", gen_class, false, nullptr, {}, opt_t(gen_elem));
        next->name = "next";
        next->dkind = "method";
        next->ret = opt_t(gen_elem);
        next->body = block_n(stmts);
        next->mods = {"override"};
        // the constructor: the arguments into their fields, at the start
        std::vector<IRP> cbody;
        for (auto& prm : params)
            cbody.push_back(exprstmt(assign_e(std::nullopt, this_sel(prm->sym->name, prm->type), mk_name(prm->sym, prm->type), prm->type)));
        auto ctor = node("MethodDecl", line);
        std::vector<TyP> ptypes;
        for (auto& prm : params) ptypes.push_back(prm->type);
        ctor->sym = sym("ctor", camel(f->name) + "Generator", gen_class, false, nullptr, ptypes, VOID());
        ctor->name = "__init__";
        ctor->dkind = "ctor";
        ctor->params = params;
        ctor->ret = VOID();
        ctor->body = block_n(cbody);
        auto cd = node("ClassDecl", line);
        cd->name = camel(f->name) + "Generator";
        cd->qname = gen_class;
        cd->dkind = "class";
        cd->mods = {"generator"};
        cd->extends = cls("py.generator", {gen_elem});
        for (auto& [name, t] : fields) {
            auto fd = node("FieldDecl");
            fd->sym = sym("field", name, gen_class, false, t);
            fd->type = t;
            cd->members.push_back(fd);
        }
        for (auto& [name, t] : gen_fields) {
            auto fd = node("FieldDecl");
            fd->sym = sym("field", name, gen_class, false, t);
            fd->type = t;
            cd->members.push_back(fd);
        }
        auto sf = node("FieldDecl");
        sf->sym = sym("field", "state", gen_class, false, I64());
        sf->type = I64();
        sf->init = mk_lit(py::V(0), I64());
        cd->members.push_back(sf);
        cd->members.push_back(ctor);
        cd->members.push_back(next);
        // the function: makes the generator
        TyP itt = cls("py.iter", {gen_elem});
        std::vector<IRP> args;
        for (auto& prm : params) args.push_back(mk_name(prm->sym, prm->type));
        auto nw = node("New", line);
        nw->sym = sym("ctor", cd->name, gen_class);
        nw->args = args;
        nw->type = cls_flag(gen_class, "ref");
        nw->memory = "gc";
        auto rr = node("Return", line);
        rr->expr = nw;
        auto md = node("MethodDecl", line);
        md->sym = sym("function", f->name, f->module->name, false, nullptr, ptypes, itt);
        md->name = f->name;
        md->dkind = "function";
        md->params = params;
        md->ret = itt;
        md->body = block_n({rr});
        md->mods = {"generator"};
        std::string doc;
        if (docstring(f->node->body, doc)) md->comments.push_back(doc);
        return {cd, md};
    }

    // (new) a nested function: a capturing lambda in a local of type py.fn<R, P...>
    IRP s_FunctionDef(const A::FunctionDef* st) {
        int line = st->pos.lineno;
        auto it = p.nested_by_node.find(st);
        if (it == p.nested_by_node.end()) throw Refused("syntax", "nested function", line);
        Fn* nf = it->second;
        Eval ev2(p, nf, true);
        p.suspend = 0;
        ev2.run();
        FnLower fl2(ctx, nf, ev2);
        IRP md;
        try {
            md = fl2.lower();
        } catch (const Refused& r) {
            throw Refused("calls-refused", "nested function " + nf->name + " (" + trunc(r.issue.reason(), 60) + ")", r.issue.line);
        }
        calls.insert(fl2.calls.begin(), fl2.calls.end());
        globals_used.insert(fl2.globals_used.begin(), fl2.globals_used.end());
        std::vector<TyP> fargs = {md->ret};
        for (auto& prm : md->params) fargs.push_back(prm->type);
        TyP ft = cls("py.fn", fargs);
        auto lam = node("Lambda", line);
        lam->params = md->params;
        lam->body = md->body;
        lam->type = md->ret;
        declared.insert(st->name);
        closure_types[st->name] = ft;
        return local(st->name, ft, lam, {"closure"}, line);
    }
    std::map<std::string, TyP> closure_types;

    IRP s_AugAssign(const A::AugAssign* st) {
        int line = st->pos.lineno;
        T t = Tof(st);
        IRP target = target_expr(st->target);
        IRP value = expr(st->value);
        IRP cur = expr_of_target(st->target, target);
        T tt = ev.types.count(st->target) ? Tof(st->target) : t;
        IRP nw = binop(st, st->op, cur, value, tt, Tof(st->value), t);
        static const std::set<std::string> AOPS = {"+", "-", "*", "&", "|", "^", "<<", ">>", "/", "%"};
        if (nw->k == "Binary" && nw->op && AOPS.count(*nw->op) && nw->left == cur)
            return exprstmt(assign_e(nw->op, target, value, nw->type), line);
        if (nw->k == "Concat" && !nw->parts.empty() && nw->parts[0] == cur) {
            IRP v;
            if (nw->parts.size() > 2) {
                v = node("Concat");
                v->parts.assign(nw->parts.begin() + 1, nw->parts.end());
                v->type = nw->type;
            } else {
                v = nw->parts[1];
            }
            return exprstmt(assign_e(std::string("+"), target, v, nw->type), line);
        }
        return exprstmt(assign_e(std::nullopt, target, nw, nw->type), line);
    }

    IRP expr_of_target(const A::Expr* tgt, const IRP& lowered) {
        if (as<pyfront::Name>(tgt)) return mk_name(lowered->sym, lowered->type);
        return lowered;
    }

    std::vector<IRP> store(const A::Expr* tgt, IRP value, const A::Stmt* st) {
        int line = st->pos.lineno;
        if (auto* nm = as<pyfront::Name>(tgt)) {
            const std::string& name = nm->id;
            auto it = local_types.find(name);
            if (it == local_types.end()) throw Refused("untyped", "local " + name, line);
            TyP t = it->second;
            value = coerce(value, t);
            if (decl_init.count(st) && !declared.count(name)) {
                declared.insert(name);
                return {local(name, t, value, {}, line)};
            }
            return {exprstmt(assign_e(std::nullopt, mk_name(lsym(name, t), t), value, t), line)};
        }
        if (tgt->kind == ExprKind::Tuple || tgt->kind == ExprKind::List) return unpack(tgt, value, st);
        if (auto* sub = as<Subscript>(tgt); sub && sub->slice->kind == ExprKind::Slice) {   // a[lo:hi] = v
            auto* sl = cast<pyfront::Slice>(sub->slice);
            if (sl->step) throw Refused("syntax", "slice assignment with a step", line);
            IRP obj = expr(sub->value);
            IRP lo = sl->lower ? expr(sl->lower) : mk_lit_none(VOID());
            IRP hi = sl->upper ? expr(sl->upper) : mk_lit_none(VOID());
            if (obj->type && obj->type->is_class("py.list")) value = coerce(value, obj->type);
            return {exprstmt(mk_pyfn("set_slice", {lo, hi, value}, VOID(), obj, line), line)};
        }
        IRP target = target_expr(tgt);
        value = coerce(value, target->type);
        return {exprstmt(assign_e(std::nullopt, target, value, target->type), line)};
    }

    std::vector<IRP> unpack(const A::Expr* tgt, IRP value, const A::Stmt* st) {
        int line = st->pos.lineno;
        const std::vector<pyfront::Expr*>& elts = tgt->kind == ExprKind::Tuple ? cast<pyfront::Tuple>(tgt)->elts : cast<pyfront::List>(tgt)->elts;
        std::vector<std::string> names;
        bool all_names = true;
        for (auto* e : elts) {
            auto* nm = as<pyfront::Name>(e);
            if (!nm) all_names = false;
            else names.push_back(nm->id);
        }
        const std::vector<TyP>* tts = (value->type && value->type->is_class("py.tuple")) ? &value->type->args : nullptr;
        if (!tts || tts->size() != elts.size()) throw Refused("conflict", "unpacking a non-tuple", line);
        if (!all_names) {   // a, self.b, (c, d) = v: v once into a temporary, then each target left to right
            std::string tn = new_tmp("parts");
            local_types[tn] = value->type;
            declared.insert(tn);
            std::vector<IRP> out = {local(tn, value->type, value, {}, line)};
            for (auto& n : names)
                if (!declared.count(n)) {
                    auto lt = local_types.find(n);
                    if (lt == local_types.end()) throw Refused("untyped", "local " + n, line);
                    out.push_back(local(n, lt->second));
                    declared.insert(n);
                }
            for (size_t i = 0; i < elts.size(); i++)
                for (auto& x : store(elts[i], mk_pyfn("get", {name_expr(tn)}, (*tts)[i], nullptr, 0, {py::V((long long)i)}), st)) out.push_back(x);
            return out;
        }
        if (names.size() == 1) {
            const std::string& n = names[0];
            auto lt = local_types.find(n);
            IRP item = coerce(mk_pyfn("get", {value}, (*tts)[0], nullptr, 0, {py::V(0)}), lt == local_types.end() ? nullptr : lt->second);
            auto dt = decl_tuple.find(st);
            if (dt != decl_tuple.end() && dt->second.count(n) && !declared.count(n)) {
                declared.insert(n);
                TyP t = local_types[n];
                return {local(n, t, item, {}, line)};
            }
            return store(elts[0], item, st);
        }
        std::set<std::string> fresh;
        auto dt = decl_tuple.find(st);
        if (dt != decl_tuple.end()) fresh = dt->second;
        bool all = true;
        for (size_t i = 0; i < names.size(); i++) {
            auto lt = local_types.find(names[i]);
            if (!(fresh.count(names[i]) && !declared.count(names[i]) && lt != local_types.end() && ty_eq(lt->second, (*tts)[i]))) all = false;
        }
        if (all) {
            for (auto& n : names) declared.insert(n);
            std::vector<IRP> ns;
            for (auto& n : names) ns.push_back(name_expr(n));
            return {exprstmt(assign_e(std::nullopt, mk_pyfn("bind", ns, value->type), value, value->type), line)};
        }
        std::vector<IRP> out;
        for (auto& n : names) {
            if (!declared.count(n)) {
                TyP t = local_types[n];
                out.push_back(local(n, t));
                declared.insert(n);
            }
        }
        std::vector<IRP> ns;
        for (auto& n : names) ns.push_back(name_expr(n));
        out.push_back(exprstmt(assign_e(std::nullopt, mk_pyfn("tie", ns, value->type), value, value->type), line));
        return out;
    }

    IRP name_expr(const std::string& name) {
        TyP t = local_types[name];
        return mk_name(lsym(name, t), t);
    }

    IRP target_expr(const A::Expr* tgt) {
        int line = tgt->pos.lineno;
        if (auto* nm = as<pyfront::Name>(tgt)) {
            if (!local_types.count(nm->id)) throw Refused("untyped", "local " + nm->id, line);
            if (!declared.count(nm->id)) {
                TyP t = local_types[nm->id];
                pre.push_back(local(nm->id, t));
                declared.insert(nm->id);
            }
            return name_expr(nm->id);
        }
        if (auto* at = as<Attribute>(tgt)) {
            const Res* r = R(tgt);
            if (!r || r->k != "field") throw Refused("dynamic", "attribute store", line);
            IRP obj = expr(at->value);
            auto atype = p.attr_type(r->A(), r->B());
            if (!atype) throw Refused("frontend-error", "AttributeError: 'NoneType' object has no attribute 'kind'", line);
            TyP ft = ctx.ty(*atype, line, "attribute " + r->B());
            return select_n(obj, sym("field", r->B(), r->A(), false, ft), ft, line);
        }
        if (auto* sub = as<Subscript>(tgt)) {
            const Res* r = R(tgt);
            IRP obj = expr(sub->value);
            if (r && r->k == "dict_store" && r->a && obj->type && obj->type->record) return field_ref(obj, *r->a, sub->value, line, true);
            if (sub->slice->kind == ExprKind::Slice) throw Refused("syntax", "slice assignment", line);
            IRP idx = expr(sub->slice);
            TyP et = elem_type(obj->type, line);
            auto n = node("Index", line);
            n->arr = obj;
            n->index = idx;
            n->type = et;
            return n;
        }
        throw Refused("syntax", "assignment target " + type_name(tgt), line);
    }

    TyP elem_type(const TyP& t, int line) {
        if (t && t->kind == "class" && (t->name == "py.list" || t->name == "py.set") && !t->args.empty()) return t->args[0];
        if (t && t->is_class("py.dict")) return t->args[1];
        if (t && t->kind == "class" && (t->name == "py.bytes" || t->name == "py.bytearray")) return I64();
        if (t && t->is_class("py.str")) return STRT();
        throw Refused("conflict", "item of " + ty_repr(t), line);
    }

    std::vector<IRP> s_Return(const A::Return* st) {
        int line = st->pos.lineno;
        if (!st->value) {
            auto r = node("Return", line);
            if (!ty_eq(ret, VOID())) r->expr = mk_lit_none(ret);
            return {r};
        }
        auto vk = st->value->kind;
        if (vk == ExprKind::ListComp || vk == ExprKind::SetComp || vk == ExprKind::DictComp) {
            std::string name = new_tmp("result");
            TyP t = ty(Tof(st->value), st->value);
            std::vector<IRP> out = {local(name, t, empty(t))};
            local_types[name] = t;
            declared.insert(name);
            auto loops = comp_loops(st->value, name, t);
            out.insert(out.end(), loops.begin(), loops.end());
            auto r = node("Return", line);
            r->expr = name_expr(name);
            out.push_back(r);
            return out;
        }
        if (ty_eq(ret, VOID())) {
            IRP e = expr(st->value);
            if (is_none_const(st->value)) return {node("Return", line)};
            return {exprstmt(e), node("Return", line)};
        }
        auto r = node("Return", line);
        r->expr = coerce(expr(st->value), ret);
        return {r};
    }

    IRP s_If(const A::If* st) {
        IRP c = cond(st->test);
        auto saved_outer = pre;
        pre.clear();
        IRP then = block(st->body);
        IRP orelse;
        if (!st->orelse.empty()) {
            if (st->orelse.size() == 1 && st->orelse[0]->kind == StmtKind::If && !decl_at.count(st->orelse[0])) {
                auto saved = pre;
                pre.clear();
                IRP inner = s_If(cast<pyfront::If>(st->orelse[0]));
                if (!pre.empty()) {
                    std::vector<IRP> v = pre;
                    v.push_back(inner);
                    orelse = block_n(v);
                } else {
                    orelse = inner;
                }
                pre = saved;
            } else {
                orelse = block(st->orelse);
            }
        }
        pre = saved_outer;
        auto n = node("If", st->pos.lineno);
        n->cond = c;
        n->then = then;
        n->orelse = orelse;
        return n;
    }

    std::vector<IRP> s_For(const A::For* st) {
        int line = st->pos.lineno;
        const A::Expr* it = st->iter;
        const Res* r = R(it);
        auto* tn = as<pyfront::Name>(st->target);
        if (r && r->is("builtin", "range") && tn) return range_loop(st);
        IRP iterable = expr(it);
        const A::Expr* tgt = st->target;
        std::vector<std::string> names;
        size_t nelts = 0;
        if (tn) names.push_back(tn->id);
        else if (auto* tu = as<pyfront::Tuple>(tgt)) {
            nelts = tu->elts.size();
            for (auto* e : tu->elts)
                if (auto* nm = as<pyfront::Name>(e)) names.push_back(nm->id);
        } else if (auto* li = as<pyfront::List>(tgt)) {
            nelts = li->elts.size();
            for (auto* e : li->elts)
                if (auto* nm = as<pyfront::Name>(e)) names.push_back(nm->id);
        }
        if (!tn && names.size() != nelts) throw Refused("syntax", "loop target", line);
        bool inner_only = true;
        for (auto& n : names)
            if (!loop_local(st, n)) {
                inner_only = false;
                break;
            }
        TyP elem_t = ty(Tof(tgt), tgt, "loop item");
        if (inner_only) {
            for (auto& n : names) declared.insert(n);
            IRP var;
            if (tn) var = local(tn->id, local_types[tn->id]);
            else {
                std::string joined;
                for (size_t i = 0; i < names.size(); i++) joined += (i ? "," : "") + names[i];
                var = local(joined, elem_t, nullptr, {"bind"});
            }
            IRP body = block(st->body);
            auto fe = node("ForEach", line);
            fe->var = var;
            fe->iterable = iterable;
            fe->body = body;
            return {fe};
        }
        std::string tmpn = new_tmp("item");
        local_types[tmpn] = elem_t;
        IRP var = local(tmpn, elem_t);
        std::vector<IRP> pre_;
        for (auto& n : names)
            if (!declared.count(n)) {
                TyP t = local_types[n];
                pre_.push_back(local(n, t));
                declared.insert(n);
            }
        std::vector<IRP> first = tn ? store(tgt, name_expr(tmpn), st) : unpack(tgt, name_expr(tmpn), st);
        IRP body = block(st->body);
        first.insert(first.end(), body->stmts.begin(), body->stmts.end());
        body->stmts = first;
        auto fe = node("ForEach", line);
        fe->var = var;
        fe->iterable = iterable;
        fe->body = body;
        pre_.push_back(fe);
        return pre_;
    }

    bool loop_local(const A::For* st, const std::string& name) {
        auto lv = loopvars.find(name);
        if (lv != loopvars.end() && lv->second) return true;
        std::set<const void*> inside;
        for (auto& x : walk(st)) inside.insert(x.p);
        for (auto& x : walk(AnyNode(f->node)))
            if (auto* nm = as<pyfront::Name>(x.expr()))
                if (nm->id == name && !inside.count(nm)) return false;
        auto* tt = as<pyfront::Tuple>(st->target);
        for (auto& x : walk(st)) {
            auto* nm = as<pyfront::Name>(x.expr());
            if (nm && nm->id == name && nm->ctx == ExprCtx::Store && nm != st->target &&
                !(tt && std::find(tt->elts.begin(), tt->elts.end(), nm) != tt->elts.end()))
                return false;
        }
        return !declared.count(name) || (lv != loopvars.end() && lv->second);
    }

    std::vector<IRP> range_loop(const A::For* st) {
        int line = st->pos.lineno;
        auto* call = cast<pyfront::Call>(st->iter);
        std::vector<IRP> args;
        for (auto* a : call->args) args.push_back(expr(a));
        Big step(1);
        if (call->args.size() == 3) int_const(call->args[2], &step);
        IRP start, stop;
        if (args.size() == 1) {
            start = mk_lit(py::V(0), I64());
            stop = args[0];
        } else {
            start = args[0];
            stop = args[1];
        }
        const std::string& name = cast<pyfront::Name>(st->target)->id;
        auto ltt = local_types.find(name);
        TyP it_t = ltt == local_types.end() ? I64() : ltt->second;
        bool inner = loop_local(st, name);
        std::vector<IRP> stmts;
        const A::Expr* bound_node = call->args.size() < 3 ? call->args.back() : call->args[1];
        if (!pure(bound_node, st)) {
            std::string sname = new_tmp("stop");
            local_types[sname] = I64();
            stmts.push_back(local(sname, I64(), stop, {"const"}));
            stop = name_expr(sname);
        }
        std::string var;
        if (inner) {
            var = name;
            declared.insert(name);
        } else {
            var = new_tmp(name);
            local_types[var] = I64();
            if (!declared.count(name)) {
                stmts.push_back(local(name, it_t));
                declared.insert(name);
            }
        }
        IRP v = mk_name(lsym(var, I64()), I64());
        std::vector<IRP> init = {local(var, I64(), start)};
        IRP c = binary(step.sign() > 0 ? "<" : ">", v, stop, BOOLT());
        IRP upd;
        if (step == Big(1)) upd = unary("++", v, I64());
        else if (step == Big(-1)) upd = unary("--", v, I64());
        else upd = assign_e(std::string("+"), v, mk_lit(py::V((long long)step.i64()), I64()), I64());
        IRP body = block(st->body);
        if (!inner) body->stmts.insert(body->stmts.begin(), exprstmt(assign_e(std::nullopt, name_expr(name), v, I64())));
        auto fo = node("For", line);
        fo->inits = init;
        fo->cond = c;
        fo->update = {upd};
        fo->body = body;
        stmts.push_back(fo);
        return stmts;
    }

    bool pure(const A::Expr* n, const A::Stmt* loop) {
        if (is_const(n)) return true;
        auto* nm = as<pyfront::Name>(n);
        if (nm && local_types.count(nm->id)) {
            for (auto& x : walk(loop)) {
                auto* m = as<pyfront::Name>(x.expr());
                if (m && m->id == nm->id && m->ctx == ExprCtx::Store) return false;
            }
            return true;
        }
        return false;
    }

    IRP s_Try(const A::Try* st) {
        IRP body = block(st->body);
        std::vector<IRP> catches;
        for (auto* h : st->handlers) {
            std::vector<TyP> types;
            if (!h->type) {
                types.push_back(cls("py.Exception"));
            } else {
                std::vector<const A::Expr*> names;
                if (auto* tu = as<pyfront::Tuple>(h->type)) names.assign(tu->elts.begin(), tu->elts.end());
                else names.push_back(h->type);
                for (auto* n : names) {
                    auto* nm = as<pyfront::Name>(n);
                    std::string lc = nm ? ev.lookup_class(n) : "";
                    if (nm && !lc.empty()) types.push_back(cls_ref(lc, false));
                    else if (nm) types.push_back(cls("py." + nm->id));
                    else if (auto* at = as<Attribute>(n)) types.push_back(cls(unparse(n) == "struct.error" ? "py.StructError" : "py." + at->attr));
                    else throw Refused("frontend-error", "AttributeError: except type", h->pos.lineno);
                }
            }
            auto c = node("Catch", h->pos.lineno);
            c->types = types;
            c->body = block(h->body);
            catches.push_back(c);
        }
        auto t = node("Try", st->pos.lineno);
        t->body = body;
        t->catches = catches;
        return t;
    }

    std::vector<IRP> s_With(const A::With* st) {
        std::vector<IRP> out;
        for (auto* item : st->items) {
            IRP v = expr(item->context_expr);
            if (item->optional_vars) {
                auto got = store(item->optional_vars, v, st);
                out.insert(out.end(), got.begin(), got.end());
            } else {
                out.push_back(exprstmt(v));
            }
        }
        IRP b = block(st->body);
        out.insert(out.end(), b->stmts.begin(), b->stmts.end());
        return {block_n(out)};
    }

    // -- comprehensions
    std::string new_tmp(const std::string& base) {
        tmp++;
        auto taken = [&](const std::string& n) { return local_types.count(n) || ev.locals.count(n); };
        std::string name = (taken(base) || tmp > 1) ? base + "_" + std::to_string(tmp) : base;
        while (taken(name)) {
            tmp++;
            name = base + "_" + std::to_string(tmp);
        }
        return name;
    }

    IRP empty(const TyP& t) { return mk_pyfn("empty", {}, t); }

    std::vector<IRP> comp_into(const A::Assign* st, const pyfront::Name* tgt, const A::Expr* comp) {
        const std::string& name = tgt->id;
        TyP t = local_types[name];
        std::vector<IRP> out;
        if (!declared.count(name)) {
            out.push_back(local(name, t, empty(t)));
            declared.insert(name);
        } else {
            out.push_back(exprstmt(assign_e(std::nullopt, name_expr(name), empty(t), t)));
        }
        auto loops = comp_loops(comp, name, t);
        out.insert(out.end(), loops.begin(), loops.end());
        return out;
    }

    std::vector<IRP> comp_loops(const A::Expr* comp, const std::string& into, const TyP& t) {
        IRP acc = name_expr(into);
        auto add = [&]() -> IRP {
            if (auto* dc = as<DictComp>(comp)) {
                IRP k = expr(dc->key);
                IRP v = coerce(expr(dc->value), t->args[1]);
                auto ix = node("Index");
                ix->arr = acc;
                ix->index = k;
                ix->type = t->args[1];
                return exprstmt(assign_e(std::nullopt, ix, v, t->args[1]));
            }
            const A::Expr* elt = comp->kind == ExprKind::ListComp ? cast<ListComp>(comp)->elt : cast<SetComp>(comp)->elt;
            IRP e = coerce(expr(elt), t->args[0]);
            return exprstmt(mk_pyfn(t->name == "py.list" ? "list.append" : "set.add", {e}, VOID(), acc));
        };
        const std::vector<comprehension*>& gens = comp->kind == ExprKind::ListComp ? cast<ListComp>(comp)->generators
                                                  : comp->kind == ExprKind::SetComp ? cast<SetComp>(comp)->generators
                                                                                    : cast<DictComp>(comp)->generators;
        return gen_loops(gens, 0, add);
    }

    std::vector<IRP> gen_loops(const std::vector<comprehension*>& gens, size_t gi, const std::function<IRP()>& innermost) {
        const comprehension* g = gens[gi];
        auto saved = pre;
        pre.clear();
        const Res* r = R(g->iter);
        bool is_range = r && r->is("builtin", "range");
        IRP iterable = !is_range ? expr(g->iter) : nullptr;
        const A::Expr* tgt = g->target;
        auto* tn = as<pyfront::Name>(tgt);
        std::vector<std::string> names;
        size_t nelts = 0;
        const std::vector<pyfront::Expr*>* elts = nullptr;
        if (tn) names.push_back(tn->id);
        else {
            if (auto* tu = as<pyfront::Tuple>(tgt)) elts = &tu->elts;
            else if (auto* li = as<pyfront::List>(tgt)) elts = &li->elts;
            if (elts) {
                nelts = elts->size();
                for (auto* e : *elts)
                    if (auto* nm = as<pyfront::Name>(e)) names.push_back(nm->id);
            }
        }
        if (!tn && names.size() != nelts) throw Refused("syntax", "comprehension target", tgt->pos.lineno);
        TyP elem_t = ty(Tof(tgt), tgt, "comprehension item");
        for (auto& n : names) {
            auto lt = local_types.find(n);
            if (lt != local_types.end() && declared.count(n) && !ty_eq(lt->second, elem_t))
                throw Refused("syntax", "comprehension variable " + n + " shadows a local of another type", tgt->pos.lineno);
        }
        IRP var;
        if (tn) {
            local_types[tn->id] = elem_t;
            var = local(tn->id, elem_t);
        } else {
            for (size_t i = 0; i < elts->size() && i < elem_t->args.size(); i++) local_types[cast<pyfront::Name>((*elts)[i])->id] = elem_t->args[i];
            std::string joined;
            for (size_t i = 0; i < names.size(); i++) joined += (i ? "," : "") + names[i];
            var = local(joined, elem_t, nullptr, {"bind"});
        }
        std::vector<IRP> body_stmts;
        if (gi + 1 < gens.size()) {
            auto inner = gen_loops(gens, gi + 1, innermost);
            body_stmts.insert(body_stmts.end(), inner.begin(), inner.end());
        } else {
            pre.clear();
            IRP st = innermost();
            body_stmts.insert(body_stmts.end(), pre.begin(), pre.end());
            if (st->k == "Block") body_stmts.insert(body_stmts.end(), st->stmts.begin(), st->stmts.end());   // (its statements, no braces of their own)
            else body_stmts.push_back(st);
            pre.clear();
        }
        for (auto it = g->ifs.rbegin(); it != g->ifs.rend(); ++it) {
            pre.clear();
            IRP c = cond(*it);
            if (!pre.empty()) throw Refused("syntax", "comprehension condition needing statements", (*it)->pos.lineno);
            auto i = node("If");
            i->cond = c;
            i->then = block_n(body_stmts);
            body_stmts = {i};
        }
        IRP lp;
        if (is_range && tn) {
            auto* call = cast<pyfront::Call>(g->iter);
            std::vector<IRP> args;
            for (auto* a : call->args) args.push_back(expr(a));
            IRP start = args.size() == 1 ? mk_lit(py::V(0), I64()) : args[0];
            IRP stop = args.size() == 1 ? args[0] : args[1];
            Big step(1);
            if (args.size() == 3) int_const(call->args[2], &step);
            IRP v = name_expr(tn->id);
            auto lo = node("Local");
            lo->sym = v->sym;
            lo->type = I64();
            lo->init = start;
            lp = node("For");
            lp->inits = {lo};
            lp->cond = binary(step.sign() > 0 ? "<" : ">", v, stop, BOOLT());
            lp->update = {step == Big(1) ? unary("++", v, I64()) : assign_e(std::string("+"), v, mk_lit(py::V((long long)step.i64()), I64()), I64())};
            lp->body = block_n(body_stmts);
        } else {
            lp = node("ForEach");
            lp->var = var;
            lp->iterable = iterable;
            lp->body = block_n(body_stmts);
        }
        std::vector<IRP> out = pre;
        out.push_back(lp);
        pre = saved;
        return out;
    }

    IRP hoist_comp(const A::Expr* n, const std::string& kind) {
        TyP t = ty(Tof(n), n);
        std::string name = new_tmp(kind == "dict" ? "table" : "items");
        local_types[name] = t;
        declared.insert(name);
        std::vector<IRP> stmts = {local(name, t, empty(t))};
        auto loops = comp_loops(n, name, t);
        stmts.insert(stmts.end(), loops.begin(), loops.end());
        pre.insert(pre.end(), stmts.begin(), stmts.end());
        return name_expr(name);
    }

    IRP gen_as_list(const GeneratorExp* gen) {
        TyP et = ty(Tof(gen->elt), gen->elt, "generator item");
        TyP t = cls("py.list", {et});
        std::string name = new_tmp("items");
        local_types[name] = t;
        declared.insert(name);
        IRP acc = name_expr(name);
        auto lo = node("Local");
        lo->sym = acc->sym;
        lo->type = t;
        lo->init = empty(t);
        std::vector<IRP> stmts = {lo};
        auto add = [&]() -> IRP { return exprstmt(mk_pyfn("list.append", {coerce(expr(gen->elt), et)}, VOID(), acc)); };
        auto loops = gen_loops(gen->generators, 0, add);
        stmts.insert(stmts.end(), loops.begin(), loops.end());
        pre.insert(pre.end(), stmts.begin(), stmts.end());
        return acc;
    }

    IRP any_all(const A::Call* n, const std::string& which) {
        const A::Expr* gen = n->args[0];
        if (gen->kind != ExprKind::GeneratorExp) {
            IRP seq = expr(gen);
            return mk_pyfn(which, {seq}, BOOLT());
        }
        auto* g = cast<GeneratorExp>(gen);
        std::string name = new_tmp(which == "any" ? "found" : "every");
        local_types[name] = BOOLT();
        declared.insert(name);
        IRP acc = name_expr(name);
        auto lo = node("Local");
        lo->sym = acc->sym;
        lo->type = BOOLT();
        lo->init = mk_lit(py::V(which == "all"), BOOLT());
        std::vector<IRP> stmts = {lo};
        auto decide = [&]() -> IRP {
            IRP c = cond(g->elt);
            if (which == "all") c = unary("!", paren_n(c, BOOLT()), BOOLT());
            auto i = node("If");
            i->cond = c;
            i->then = block_n({exprstmt(assign_e(std::nullopt, acc, mk_lit(py::V(which == "any"), BOOLT()), BOOLT())), node("Break")});
            return i;
        };
        auto loops = gen_loops(g->generators, 0, decide);
        if (g->generators.size() > 1) throw Refused("syntax", which + " over nested loops (break leaves only the inner one)", n->pos.lineno);
        stmts.insert(stmts.end(), loops.begin(), loops.end());
        pre.insert(pre.end(), stmts.begin(), stmts.end());
        return acc;
    }

    // next(<generator>, default): first = default; for (...) if (<filter>) { first = <item>; break; }
    IRP next_first(const A::Call* n, const TyP& rt) {
        auto* g = cast<GeneratorExp>(n->args[0]);
        std::string name = new_tmp("first");
        local_types[name] = rt;
        declared.insert(name);
        IRP acc = name_expr(name);
        std::vector<IRP> stmts = {local(name, rt, coerce(expr(n->args[1]), rt))};
        auto take = [&]() -> IRP { return block_n({exprstmt(assign_e(std::nullopt, acc, coerce(expr(g->elt), rt), rt)), node("Break")}); };
        auto loops = gen_loops(g->generators, 0, take);
        stmts.insert(stmts.end(), loops.begin(), loops.end());
        pre.insert(pre.end(), stmts.begin(), stmts.end());
        return acc;
    }

    // -- records
    bool is_record_literal(const A::Expr* n) {
        auto it = ev.types.find(n);
        if (it == ev.types.end() || it->second.kind != TK::Dict || !p.is_record(it->second.site)) return false;
        if (n->kind == ExprKind::Dict) return true;
        auto* c = as<pyfront::Call>(n);
        auto* fnm = c ? as<pyfront::Name>(c->func) : nullptr;
        return fnm && fnm->id == "dict";
    }

    IRP record_builder(const A::Assign* st, const pyfront::Name* tgt) {
        TyP t = local_types[tgt->id];
        IRP value = expr(st->value);
        declared.insert(tgt->id);
        return local(tgt->id, t, value, {}, st->pos.lineno);
    }

    bool effects(const IRP& e) {
        static const std::set<std::string> PURE = {"tuple", "get", "unwrap", "truthy", "is_none", "is_not_none", "opt_value", "list", "str", "empty",
                                                   "record", "len"};
        bool out = false;
        ir_walk(e, [&](const IRP& x) {
            if (out) return;
            if (x->k == "Assign" || x->k == "New") out = true;
            else if (x->k == "Call") {
                if (!(x->sym->owner && *x->sym->owner == PY && PURE.count(x->sym->name))) out = true;
            } else if (x->k == "Index") out = true;
        });
        return out;
    }

    IRP record_value(const A::Expr* n) {
        T t = Tof(n);
        TyP rt = ty(t, n);
        int s = p.find(t.site);
        auto fields = ctx.records.fields(s);
        std::vector<std::pair<std::string, IRP>> given;
        if (auto* d = as<pyfront::Dict>(n)) {
            for (size_t i = 0; i < d->values.size(); i++) {
                if (!d->keys[i]) {
                    IRP src_ = expr(d->values[i]);
                    auto sf = spread_fields(src_, d->values[i]);
                    given.insert(given.end(), sf.begin(), sf.end());
                } else {
                    std::string key;
                    is_str_const(d->keys[i], &key);
                    given.emplace_back(key, expr(d->values[i]));
                }
            }
        } else {
            auto* c = cast<pyfront::Call>(n);
            for (auto* a : c->args) {
                IRP src_ = expr(a);
                auto sf = spread_fields(src_, a);
                given.insert(given.end(), sf.begin(), sf.end());
            }
            for (auto* kw : c->keywords) {
                if (!kw->has_arg) {
                    IRP src_ = expr(kw->value);
                    auto sf = spread_fields(src_, kw->value);
                    given.insert(given.end(), sf.begin(), sf.end());
                } else {
                    given.emplace_back(kw->arg, expr(kw->value));
                }
            }
        }
        std::map<std::string, IRP> by_key;
        for (auto& [k, e] : given) by_key[k] = e;   // dict(given): the last one wins
        std::vector<std::string> order;
        for (auto& [k, _] : given) order.push_back(k);
        std::vector<std::string> forder;
        for (auto& f_ : fields)
            if (by_key.count(f_.key)) forder.push_back(f_.key);
        int impure = 0;
        for (auto& [k, e] : given)
            if (effects(e)) impure++;
        if (order != forder && impure > 1) {
            for (auto& k : order) {
                IRP e = by_key[k];
                if (effects(e)) {
                    std::string nm = new_tmp(k);
                    local_types[nm] = e->type;
                    declared.insert(nm);
                    pre.push_back(local(nm, e->type, e));
                    by_key[k] = name_expr(nm);
                }
            }
        }
        std::vector<IRP> args;
        std::vector<py::V> keys;
        for (auto& f_ : fields) {
            TyP want = ty(f_.type, n, "field " + f_.key);
            if (!f_.required) want = !is_opt_t(want) ? opt_t(want) : want;
            if (by_key.count(f_.key)) args.push_back(coerce(by_key[f_.key], want));
            else args.push_back(mk_lit_none(want));
            keys.push_back(py::V(f_.key));
        }
        return mk_pyfn("record", args, rt, nullptr, n->pos.lineno, keys);
    }

    std::vector<std::pair<std::string, IRP>> spread_fields(const IRP& src_, const A::Expr* node_) {
        if (!(src_->type && src_->type->record)) throw Refused("syntax", "** of a dict", node_->pos.lineno);
        T st = Tof(node_);
        std::vector<std::pair<std::string, IRP>> out;
        for (auto& f_ : ctx.records.fields(st.site)) out.emplace_back(f_.key, field_ref(src_, f_.key, node_, node_->pos.lineno, false, true));
        return out;
    }

    IRP field_ref(const IRP& obj, const std::string& key, const A::Expr* node_, int line, bool store_ = false, bool raw = false) {
        T t = Tof(node_);
        int s = p.find(t.site);
        auto fs = ctx.records.fields(s);
        const RecordField* info = nullptr;
        for (auto& f_ : fs)
            if (f_.key == key) info = &f_;
        if (!info) throw Refused("conflict", "no field " + py_repr(key), line);
        TyP ftt = ty(info->type, node_, "field " + key);
        if (!info->required && (raw || store_)) ftt = !is_opt_t(ftt) ? opt_t(ftt) : ftt;
        IRP sel = select_n(obj, sym("field", key, obj->type->name, false, ftt), ftt, line);
        if (!info->required && !raw && !store_) return mk_pyfn("opt_value", {sel}, ftt);
        return sel;
    }

    // -- expressions
    IRP cond(const A::Expr* n) {
        in_cond = n;
        IRP e = expr(n);
        return truthy(e);
    }
    IRP truthy(const IRP& e) {
        if (ty_eq(e->type, BOOLT())) return e;
        return mk_pyfn("truthy", {e}, BOOLT());
    }

    static std::string tag_of(const TyP& t) {
        if (!t) return "?";
        if (t->kind == "i64") return "int";
        if (t->kind == "f64") return "float";
        if (t->kind == "bool") return "bool";
        if (t->is_class("py.str")) return "str";
        if (t->is_class("py.bytes")) return "bytes";
        return ty_repr(t);
    }
    IRP coerce(const IRP& e, const TyP& want) {
        if (!want || ty_eq(e->type, want)) return e;
        if (want->is_class("py.variant") && e->type && !e->type->is_class("py.variant"))   // (new)
            return mk_pyfn("variant.of", {e}, want, nullptr, 0, {py::V(tag_of(e->type))});
        if (e->type && e->type->is_class("py.variant") && !want->is_class("py.variant"))   // (new) narrowed by a test
            return mk_pyfn("variant.get", {e}, want, nullptr, 0, {py::V(tag_of(want))});
        if (e->k == "Lit" && !e->has_value) return mk_lit_none(want);
        // a tuple literal as a tuple of other item types ((None, 0) where (str?, int) is meant): item by item
        if (e->k == "Call" && !e->target && e->sym && e->sym->name == "tuple" && e->sym->kind == "function" && want->is_class("py.tuple") &&
            want->args.size() == e->args.size()) {
            std::vector<IRP> items;
            for (size_t i = 0; i < e->args.size(); i++) items.push_back(coerce(e->args[i], want->args[i]));
            return mk_pyfn("tuple", items, want, nullptr, e->line);
        }
        if (is_opt_t(e->type) && !is_opt_t(want))
            if (ty_eq(e->type->args[0], want)) return mk_pyfn("unwrap", {e}, want);
        if (is_opt_t(want) && ty_eq(e->type, want->args[0])) return e;
        if (want->kind == "class" && e->type && e->type->kind == "class" && is_ref_type(want) && is_ref_type(e->type)) return e;
        if (ty_eq(want, I64()) && ty_eq(e->type, BOOLT())) return cast_n(e, I64());
        return e;
    }

    IRP expr(const A::Expr* n) {
        int line = n->pos.lineno;
        switch (n->kind) {
            case ExprKind::Constant: return e_Constant(cast<Constant>(n));
            case ExprKind::Name: return e_Name(cast<pyfront::Name>(n));
            case ExprKind::Attribute: return e_Attribute(cast<Attribute>(n));
            case ExprKind::Subscript: return e_Subscript(cast<Subscript>(n));
            case ExprKind::BinOp: {
                auto* b = cast<BinOp>(n);
                IRP a = expr(b->left);
                IRP c = expr(b->right);
                return binop(n, b->op, a, c, Tof(b->left), Tof(b->right), Tof(n));
            }
            case ExprKind::UnaryOp: return e_UnaryOp(cast<UnaryOp>(n));
            case ExprKind::BoolOp: return e_BoolOp(cast<BoolOp>(n));
            case ExprKind::Compare: return e_Compare(cast<Compare>(n));
            case ExprKind::IfExp: {
                auto* ie = cast<IfExp>(n);
                TyP t = ty(Tof(n), n);
                IRP c = cond(ie->test);
                auto saved = pre;
                pre.clear();
                IRP a = coerce(expr(ie->body), t);
                IRP b = coerce(expr(ie->orelse), t);
                if (!pre.empty()) throw Refused("syntax", "a conditional branch that needs statements", line);
                pre = saved;
                auto cn = node("Cond", line);
                cn->cond = c;
                cn->then = a;
                cn->orelse = b;
                cn->type = t;
                return cn;
            }
            case ExprKind::JoinedStr: return e_JoinedStr(cast<JoinedStr>(n));
            case ExprKind::Tuple: {
                TyP t = ty(Tof(n), n);
                std::vector<IRP> args;
                for (auto* e : cast<pyfront::Tuple>(n)->elts) args.push_back(expr(e));
                return mk_pyfn("tuple", args, t, nullptr, line);
            }
            case ExprKind::List:
            case ExprKind::Set: {
                TyP t = ty(Tof(n), n);
                std::vector<IRP> args;
                const auto& elts = n->kind == ExprKind::List ? cast<pyfront::List>(n)->elts : cast<pyfront::Set>(n)->elts;
                for (auto* e : elts) args.push_back(coerce(expr(e), t->args[0]));
                return mk_pyfn(n->kind == ExprKind::List ? "list" : "set", args, t, nullptr, line);
            }
            case ExprKind::Dict: {
                T t = Tof(n);
                if (p.is_record(t.site)) return record_value(n);
                TyP rt = ty(t, n);
                auto* d = cast<pyfront::Dict>(n);
                std::vector<IRP> args;
                for (size_t i = 0; i < d->values.size(); i++) {
                    if (!d->keys[i]) throw Refused("syntax", "** in a dict", line);
                    IRP k = coerce(expr(d->keys[i]), rt->args[0]);
                    IRP v = coerce(expr(d->values[i]), rt->args[1]);
                    args.push_back(mk_pyfn("tuple", {k, v}, cls("py.tuple", rt->args)));
                }
                return mk_pyfn("dict", args, rt, nullptr, line);
            }
            case ExprKind::ListComp: return hoist_comp(n, "list");
            case ExprKind::SetComp: return hoist_comp(n, "set");
            case ExprKind::DictComp: return hoist_comp(n, "dict");
            case ExprKind::GeneratorExp: throw Refused("syntax", "generator expression as a value", line);
            case ExprKind::Lambda: throw Refused("dynamic", "lambda", line);
            case ExprKind::Call: return e_Call(cast<pyfront::Call>(n));
            default: break;
        }
        throw Refused("syntax", lower_s(name_of(n->kind)), line);
    }

    IRP e_Constant(const Constant* n) {
        const ConstValue& v = n->value;
        T t = Tof(n);
        int line = n->pos.lineno;
        switch (v.kind) {
            case ConstValue::Kind::None: return mk_lit_none(VOID());
            case ConstValue::Kind::True: return mk_lit(py::V(true), BOOLT());
            case ConstValue::Kind::False: return mk_lit(py::V(false), BOOLT());
            case ConstValue::Kind::Int: {
                if (!t.fits_i64()) throw Refused("int-range", "literal " + v.text, line);
                std::string text;
                std::optional<std::string> tx;
                if (source_segment(src, f->module->lines, n->pos, text) && !text.empty()) {
                    static const std::regex re("[0-9a-fA-FxXoObB_]+");
                    if (std::regex_match(text, re)) tx = text;
                }
                return mk_lit(py::V((long long)v.i64), I64(), tx);
            }
            case ConstValue::Kind::Float: {
                std::string text;
                std::optional<std::string> tx;
                if (source_segment(src, f->module->lines, n->pos, text)) tx = text;
                return mk_lit(py::V(v.f), F64(), tx);
            }
            case ConstValue::Kind::Str: return mk_lit(py::V(v.text), STRT());
            case ConstValue::Kind::Bytes: return mk_lit(py::V(latin1_to_utf8(v.text)), BYTEST());
            default: break;
        }
        throw Refused("syntax", "constant", line);
    }

    IRP e_Name(const pyfront::Name* n) {
        const Res* r = R(n);
        T t = Tof(n);
        int line = n->pos.lineno;
        if (!r || r->k == "local") {
            const std::string& name = n->id;
            if ((f->kind == "method" || f->kind == "property") && !f->params.empty() && name == f->params[0]) {
                auto th = node("This", line);
                th->type = cls_flag(f->cls->qname, "ref");
                return th;
            }
            if (!local_types.count(name)) throw Refused("untyped", "local " + name, line);
            IRP e = name_expr(name);
            TyP flow = ty(t, n);
            return narrowed(e, flow);
        }
        if (r->k == "global") {
            globals_used.insert({r->A(), r->B()});
            TyP gt = ty(p.global_type(r->A(), r->B()), n, "global " + r->B());
            return select_n(typename_n(r->A()), sym("global", r->B(), r->A(), true, gt), gt, line);
        }
        if (r->k == "free") {   // (new) a variable of an enclosing function, captured by reference
            TyP ft = ty(t, n, "captured " + n->id);
            return mk_name(sym("local", n->id, r->A(), false, ft), ft);
        }
        throw Refused("dynamic", r->k + " " + n->id + " as a value", line);
    }

    IRP narrowed(const IRP& e, const TyP& flow) {
        if (e->type && e->type->is_class("py.variant") && flow && !flow->is_class("py.variant") && !ty_eq(flow, VOID()))   // (new)
            return mk_pyfn("variant.get", {e}, flow, nullptr, 0, {py::V(tag_of(flow))});
        if (is_opt_t(e->type) && !is_opt_t(flow)) {
            if (ty_eq(flow, VOID())) return e;
            return mk_pyfn("unwrap", {e}, flow);
        }
        return e;
    }

    IRP e_Attribute(const Attribute* n) {
        const Res* r = R(n);
        int line = n->pos.lineno;
        if (!r) throw Refused("dynamic", "attribute ." + n->attr, line);
        if (r->k == "field") {
            IRP obj = expr(n->value);
            auto at = p.attr_type(r->A(), r->B());
            if (!at) throw Refused("frontend-error", "AttributeError: 'NoneType' object has no attribute 'kind'", line);
            TyP ft = ctx.ty(*at, line, "attribute " + r->B());
            return select_n(obj, sym("field", r->B(), r->A(), false, ft), ft, line);
        }
        if (r->k == "property") {
            IRP obj = expr(n->value);
            calls.insert(r->A());
            TyP t = ty(Tof(n), n);
            Fn* pf = *p.fns.get(r->A());
            auto c = node("Call", line);
            c->target = obj;
            c->sym = sym("method", pf->name, pf->cls->qname, false, nullptr, {}, t);
            c->type = t;
            return c;
        }
        if (r->k == "global") {
            globals_used.insert({r->A(), r->B()});
            TyP gt = ty(p.global_type(r->A(), r->B()), n, "global " + r->B());
            return select_n(typename_n(r->A()), sym("global", r->B(), r->A(), true, gt), gt, line);
        }
        if (r->k == "classattr") {
            TyP t = ty(p.class_attr_type(r->A(), r->B()), n, "class attribute " + r->B());
            return select_n(typename_n(r->A()), sym("field", r->B(), r->A(), true, t), t, line);
        }
        if (r->k == "stdattr") {
            TyP t = ty(Tof(n), n);
            return mk_pyfn(r->A(), {}, t, nullptr, line);
        }
        throw Refused("dynamic", "attribute " + n->attr + " as a value", line);
    }

    IRP e_Subscript(const Subscript* n) {
        const Res* r0 = R(n);
        Res none_res("?");
        const Res* r = r0 ? r0 : &none_res;
        int line = n->pos.lineno;
        TyP t = ty(Tof(n), n);
        IRP obj = expr(n->value);
        if (r->k == "slice") {
            auto* sl = cast<pyfront::Slice>(n->slice);
            IRP lo = sl->lower ? expr(sl->lower) : mk_lit_none(VOID());
            IRP hi = sl->upper ? expr(sl->upper) : mk_lit_none(VOID());
            std::vector<IRP> args = {lo, hi};
            if (sl->step) args.push_back(expr(sl->step));
            return mk_pyfn("slice", args, t, obj, line);
        }
        if (r->k == "tuple_slice") {
            int64_t len = (int64_t)obj->type->args.size();
            auto norm = [len](const std::optional<std::string>& s, int64_t dflt) {
                if (!s) return dflt;
                Big b = Big::from_decimal(*s);
                int64_t v = b.fits_i64() ? b.i64() : (b.sign() < 0 ? -len - 1 : len + 1);
                if (v < 0) v += len;
                return std::clamp<int64_t>(v, 0, len);
            };
            int64_t lo = norm(r->a, 0), hi = norm(r->b, len);
            if (effects(obj)) throw Refused("syntax", "slice of a computed tuple", line);
            std::vector<IRP> parts;
            for (int64_t i = lo; i < hi; i++) parts.push_back(mk_pyfn("get", {obj}, obj->type->args[i], nullptr, 0, {py::V((long long)i)}));
            return mk_pyfn("tuple", parts, t);
        }
        if (r->k == "tuple_index") {
            if (!r->a) throw Refused("syntax", "computed tuple index", line);
            int64_t len = (int64_t)obj->type->args.size();
            int64_t i = Big::from_decimal(*r->a).i64();
            i = ((i % len) + len) % len;
            return mk_pyfn("get", {obj}, t, nullptr, line, {py::V((long long)i)});
        }
        if (r->k == "field_get") return field_ref(obj, r->A(), n->value, line);
        IRP idx = expr(n->slice);
        if (r->k == "re_group") return mk_pyfn("re.Match.group", {idx}, t, obj, line, {py::V(r->B())});
        if (r->k == "dict_get") return mk_pyfn("dict.at", {idx}, t, obj, line);
        auto ix = node("Index", line);
        ix->arr = obj;
        ix->index = idx;
        ix->type = t;
        return ix;
    }

    static std::string opstr(OpKind op) {
        switch (op) {
            case OpKind::Add: return "+";
            case OpKind::Sub: return "-";
            case OpKind::Mult: return "*";
            case OpKind::Div: return "/";
            case OpKind::FloorDiv: return "//";
            case OpKind::Mod: return "%";
            case OpKind::LShift: return "<<";
            case OpKind::RShift: return ">>";
            case OpKind::BitAnd: return "&";
            case OpKind::BitOr: return "|";
            case OpKind::BitXor: return "^";
            case OpKind::Pow: return "**";
            default: return "@";
        }
    }

    IRP binop(AnyNode n, OpKind op, IRP a, IRP b, T ta, T tb, const T& tr) {
        int line = lineno(n);
        std::string o = opstr(op);
        if (o == "@") throw Refused("frontend-error", "KeyError: <class 'ast.MatMult'>", line);
        TyP rt = ty(tr, n);
        a = coerce(a, a->type);
        if (is_opt_t(a->type) || is_opt_t(b->type)) throw Refused("conflict", "arithmetic on a value that may be None", line);
        TK ka = ta.kind, kb = tb.kind;
        if (ka == TK::Bool) {
            ka = TK::Int;
            ta = INT(Big(0), Big(1));
            a = cast_n(a, I64());
        }
        if (kb == TK::Bool) {
            kb = TK::Int;
            tb = INT(Big(0), Big(1));
            b = cast_n(b, I64());
        }
        if (ka == TK::Int && kb == TK::Int) return int_op(n, o, a, b, ta, tb, tr, rt);
        if (ty_eq(rt, F64())) {
            if (o == "/") return mk_pyfn("truediv", {a, b}, F64(), nullptr, line);
            if (o == "//" || o == "%" || o == "**") return mk_pyfn(o == "//" ? "floordiv" : o == "%" ? "mod" : "pow", {a, b}, F64(), nullptr, line);
            return binary(o, as_double(a), b, F64(), line);
        }
        if (ka == TK::Str && o == "+") {
            auto c = node("Concat", line);
            if (a->k == "Concat") c->parts = a->parts;
            else c->parts.push_back(a);
            if (b->k == "Concat") c->parts.insert(c->parts.end(), b->parts.begin(), b->parts.end());
            else c->parts.push_back(b);
            c->type = STRT();
            return c;
        }
        if (ka == TK::Str && o == "%") {
            std::vector<IRP> vals;
            if (b->k == "Call" && b->sym->name == "tuple" && b->sym->owner && *b->sym->owner == PY) vals = b->args;
            else vals = {b};
            std::vector<IRP> args = {a};
            args.insert(args.end(), vals.begin(), vals.end());
            return mk_pyfn("percent", args, STRT(), nullptr, line);
        }
        if (o == "*" && (ka == TK::Str || ka == TK::Bytes || ka == TK::List)) return mk_pyfn("repeat", {a, b}, rt, nullptr, line);
        if (o == "+") return mk_pyfn("concat", {a, b}, rt, nullptr, line);
        if (ka == TK::Set && kb == TK::Set) return mk_pyfn("set" + o, {a, b}, rt, nullptr, line);
        throw Refused("conflict", std::string(kind_name(ka)) + " " + o + " " + kind_name(kb), line);
    }

    IRP as_double(const IRP& e) {
        if (ty_eq(e->type, I64())) return cast_n(e, F64());
        return e;
    }

    IRP int_op(AnyNode n, const std::string& o, IRP a, IRP b, const T& ta, const T& tb, const T& tr, const TyP&) {
        int line = lineno(n);
        bool checked = ctx.int_policy == "checked" && !tr.fits_i64();
        if (o == "+" || o == "-" || o == "*") {
            if (checked) return mk_pyfn(std::string("checked_") + (o == "+" ? "add" : o == "-" ? "sub" : "mul"), {a, b}, I64(), nullptr, line);
            return binary(o, a, b, I64(), line);
        }
        if (o == "&" || o == "|" || o == "^") return binary(o, a, b, I64(), line);
        if (o == "//" || o == "%") {
            if (ta.nonneg() && tb.positive()) return binary(o == "//" ? "/" : "%", a, b, I64(), line);
            return mk_pyfn(o == "//" ? "floordiv" : "mod", {a, b}, I64(), nullptr, line);
        }
        if (o == "<<") {
            if (!checked && ta.nonneg() && tb.nonneg() && tb.hi() && *tb.hi() < Big(63)) return binary("<<", a, b, I64(), line);
            return mk_pyfn(checked ? "checked_shl" : "shl", {a, b}, I64(), nullptr, line);
        }
        if (o == ">>") {
            if (ta.nonneg() && tb.nonneg() && tb.hi() && *tb.hi() < Big(63)) return binary(">>", a, b, I64(), line);
            return mk_pyfn("shr", {a, b}, I64(), nullptr, line);
        }
        if (o == "**") return mk_pyfn(checked ? "checked_pow" : "ipow", {a, b}, I64(), nullptr, line);
        if (o == "/") return mk_pyfn("truediv", {a, b}, F64(), nullptr, line);
        throw Refused("conflict", "int " + o + " int", line);
    }

    IRP e_UnaryOp(const UnaryOp* n) {
        int line = n->pos.lineno;
        IRP x = expr(n->operand);
        if (n->op == UnaryOpKind::Not) return unary("!", paren(truthy(x)), BOOLT(), line);
        TyP t = ty(Tof(n), n);
        if (ty_eq(x->type, BOOLT())) x = cast_n(x, I64());
        if (n->op == UnaryOpKind::USub) {
            if (ctx.int_policy == "checked" && ty_eq(t, I64()) && !Tof(n).fits_i64()) return mk_pyfn("checked_neg", {x}, I64());
            return unary("-", paren(x), t, line);
        }
        if (n->op == UnaryOpKind::UAdd) return x;
        if (n->op == UnaryOpKind::Invert) return unary("~", paren(x), t, line);
        throw Refused("syntax", "unary", line);
    }

    IRP paren(const IRP& x) {
        if (x->k == "Binary" || x->k == "Cond" || x->k == "Assign") return paren_n(x, x->type);
        return x;
    }

    IRP e_BoolOp(const BoolOp* n) {
        int line = n->pos.lineno;
        T t = Tof(n);
        std::string op = n->op == BoolOpKind::And ? "&&" : "||";
        const Res* r = R(n);
        if (r && r->k == "mixedbool" && in_cond != n) throw Refused("conflict", "value of and/or over " + r->A(), line);
        if (t.kind == TK::Bool) {
            auto saved = pre;
            std::vector<IRP> parts;
            for (size_t i = 0; i < n->values.size(); i++) {
                pre.clear();
                IRP c = cond(n->values[i]);
                if (!pre.empty() && i > 0) throw Refused("syntax", "a lazy operand that needs statements", line);
                saved.insert(saved.end(), pre.begin(), pre.end());
                parts.push_back(c);
            }
            pre = saved;
            IRP e = parts[0];
            for (size_t i = 1; i < parts.size(); i++) e = binary(op, paren_bool(e, op), paren_bool(parts[i], op), BOOLT(), line);
            return e;
        }
        TyP rt = ty(t, n);
        if (n->values.size() != 2) throw Refused("syntax", "value of a chained and/or", line);
        IRP a = expr(n->values[0]);
        auto saved = pre;
        pre.clear();
        IRP b = expr(n->values[1]);
        if (!pre.empty() || effects(b)) throw Refused("syntax", "value of and/or with a right side that has effects", line);
        pre = saved;
        if (is_opt_t(a->type) && !is_opt_t(rt)) return mk_pyfn(op == "||" ? "or_none" : "and_none", {a, coerce(b, rt)}, rt, nullptr, line);
        return mk_pyfn(op == "||" ? "or" : "and", {coerce(a, rt), coerce(b, rt)}, rt, nullptr, line);
    }

    IRP paren_bool(const IRP& e, const std::string& op) {
        if (e->k == "Binary" && e->op && (*e->op == "&&" || *e->op == "||") && *e->op != op) return paren_n(e, BOOLT());
        return e;
    }

    IRP e_Compare(const Compare* n) {
        int line = n->pos.lineno;
        const A::Expr* left_node = n->left;
        IRP a = expr(left_node);
        std::vector<IRP> parts;
        for (size_t i = 0; i < n->ops.size(); i++) {
            IRP b = expr(n->comparators[i]);
            if (i < n->ops.size() - 1 && effects(b)) throw Refused("syntax", "chained comparison with a computed middle", line);
            parts.push_back(compare(n, n->ops[i], a, b, left_node, n->comparators[i]));
            a = b;
            left_node = n->comparators[i];
        }
        IRP e = parts[0];
        for (size_t i = 1; i < parts.size(); i++) e = binary("&&", e, parts[i], BOOLT(), line);
        return e;
    }

    IRP compare(const Compare* n, CmpOpKind op, IRP a, IRP b, const A::Expr* an, const A::Expr* bn) {
        int line = n->pos.lineno;
        if (op == CmpOpKind::Is || op == CmpOpKind::IsNot || ((op == CmpOpKind::Eq || op == CmpOpKind::NotEq) && is_none_const(bn))) {
            bool pos = op == CmpOpKind::Is || op == CmpOpKind::Eq;
            IRP x = !is_none_const(an) ? a : b;
            return mk_pyfn(pos ? "is_none" : "is_not_none", {x}, BOOLT(), nullptr, line);
        }
        if (op == CmpOpKind::In || op == CmpOpKind::NotIn) {
            bool neg = op == CmpOpKind::NotIn;
            if (bn->kind == ExprKind::Tuple && !effects(a)) {
                if (b->k == "Call" && b->sym->name == "tuple") {
                    std::vector<IRP> parts;
                    for (auto& x : b->args) parts.push_back(binary(neg ? "!=" : "==", a, coerce(x, a->type), BOOLT()));
                    if (!parts.empty()) {
                        IRP e = parts[0];
                        for (size_t i = 1; i < parts.size(); i++) e = binary(neg ? "&&" : "||", e, parts[i], BOOLT());
                        return parts.size() > 1 ? paren_n(e, BOOLT()) : e;
                    }
                }
            }
            T bt = Tof(bn);
            if (bt.kind == TK::Tuple) {
                IRP e = mk_pyfn("in", {a, b}, BOOLT(), nullptr, line);
                return neg ? unary("!", e, BOOLT()) : e;
            }
            if (bt.kind == TK::Dict && p.is_record(bt.site)) {
                std::string key;
                bool hk = is_str_const(an, &key);
                const RecordField* info = nullptr;
                auto fs = ctx.records.fields(bt.site);
                if (hk)
                    for (auto& f_ : fs)
                        if (f_.key == key) info = &f_;
                if (!info) return mk_lit(py::V(neg), BOOLT());
                if (info->required) {
                    if (effects(b)) throw Refused("syntax", "membership in a computed record", line);
                    return mk_lit(py::V(!neg), BOOLT());
                }
                IRP sel = select_n(b, sym("field", key, b->type->name), cls("py.optional"));
                return mk_pyfn(!neg ? "is_not_none" : "is_none", {sel}, BOOLT());
            }
            IRP e = mk_pyfn("contains", {a}, BOOLT(), b, line);
            return neg ? unary("!", e, BOOLT()) : e;
        }
        std::string o;
        switch (op) {
            case CmpOpKind::Eq: o = "=="; break;
            case CmpOpKind::NotEq: o = "!="; break;
            case CmpOpKind::Lt: o = "<"; break;
            case CmpOpKind::LtE: o = "<="; break;
            case CmpOpKind::Gt: o = ">"; break;
            default: o = ">="; break;
        }
        if (is_opt_t(a->type) || is_opt_t(b->type)) {
            if (o == "==" || o == "!=") return binary(o, a, b, BOOLT(), line);
            throw Refused("conflict", "ordering a value that may be None", line);
        }
        if (ty_eq(a->type, BOOLT()) && ty_eq(b->type, I64())) a = cast_n(a, I64());
        if (ty_eq(b->type, BOOLT()) && ty_eq(a->type, I64())) b = cast_n(b, I64());
        if (a->type && b->type && ((a->type->kind == "i64" && b->type->kind == "f64") || (a->type->kind == "f64" && b->type->kind == "i64"))) {
            a = as_double(a);
            b = as_double(b);
        }
        return binary(o, a, b, BOOLT(), line);
    }

    IRP e_JoinedStr(const JoinedStr* n) {
        std::vector<IRP> parts;
        for (auto* v : n->values) {
            if (auto* c = as<Constant>(v)) {
                if (!c->value.text.empty()) parts.push_back(mk_lit(py::V(c->value.text), STRT()));
                continue;
            }
            auto* fv = as<FormattedValue>(v);
            if (!fv) continue;
            IRP x = expr(fv->value);
            if (fv->format_spec) {
                std::string spec;
                for (auto* c : cast<JoinedStr>(fv->format_spec)->values) {
                    std::string s;
                    if (is_str_const(c, &s)) spec += s;
                }
                parts.push_back(mk_pyfn("format", {x, mk_lit(py::V(spec), STRT())}, STRT()));
            } else if (fv->conversion == 114) {
                parts.push_back(mk_pyfn("repr", {x}, STRT()));
            } else if (ty_eq(x->type, STRT())) {
                parts.push_back(x);
            } else {
                parts.push_back(mk_pyfn("str", {x}, STRT()));
            }
        }
        if (parts.empty()) return mk_lit(py::V(""), STRT());
        if (parts.size() == 1) return ty_eq(parts[0]->type, STRT()) ? parts[0] : mk_pyfn("str", parts, STRT());
        auto c = node("Concat", n->pos.lineno);
        c->parts = parts;
        c->type = STRT();
        return c;
    }

    IRP lambda_expr(const pyfront::Lambda* lam, const std::vector<TyP>& ptypes) {
        // the lambda's parameters are its own: a local of the function with the same name keeps its type after it
        std::map<std::string, std::optional<TyP>> outer;
        for (size_t i = 0; i < lam->args->args.size() && i < ptypes.size(); i++) {
            const std::string& a = lam->args->args[i]->arg;
            auto it = local_types.find(a);
            if (!outer.count(a)) outer[a] = it == local_types.end() ? std::nullopt : std::optional<TyP>(it->second);
        }
        struct Restore {
            std::map<std::string, TyP>& lt;
            std::map<std::string, std::optional<TyP>>& o;
            ~Restore() {
                for (auto& [k, v] : o)
                    if (v) lt[k] = *v;
                    else lt.erase(k);
            }
        } restore{local_types, outer};
        std::vector<IRP> params;
        for (size_t i = 0; i < lam->args->args.size() && i < ptypes.size(); i++) {
            const std::string& a = lam->args->args[i]->arg;
            local_types[a] = ptypes[i];
            auto prm = node("Param");
            prm->sym = sym("param", a, f->qname, false, ptypes[i]);
            prm->type = ptypes[i];
            params.push_back(prm);
        }
        auto saved = pre;
        pre.clear();
        IRP body = expr(lam->body);
        if (!pre.empty()) throw Refused("syntax", "a lambda that needs statements", lam->pos.lineno);
        pre = saved;
        auto l = node("Lambda");
        l->params = params;
        l->body = body;
        l->type = body->type;
        return l;
    }

    // -- calls
    IRP e_Call(const pyfront::Call* n) {
        if (p.extended) {   // (new) getattr with a constant name, as inference chose it
            auto* fname = as<pyfront::Name>(n->func);
            std::string a;
            if (fname && fname->id == "getattr" && n->keywords.empty() && (n->args.size() == 2 || n->args.size() == 3) &&
                is_str_const(n->args[1], &a) && !a.empty() && ev.types.count(n->func)) {
                T ft = Tof(n->func), ot = ev.types.count(n->args[0]) ? Tof(n->args[0]) : BOTTOM();
                if (ft.kind == TK::Fn && ft.name() == "builtin:getattr" && ot.kind == TK::Inst) {
                    int c = p.getattr_choice(ot.name(), a);
                    if (c == 1) return expr(p.getattr_attr(n, a));
                    if (c == 2 && n->args.size() == 3) return expr(n->args[2]);
                }
            }
        }
        const Res* r = R(n);
        int line = n->pos.lineno;
        if (!r) throw Refused("dynamic", "call", line);
        const std::string& kind = r->k;
        T t = Tof(n);
        if (kind == "fn" || kind == "method" || kind == "super" || kind == "basecall") {
            Fn* cf = p.any_fn(r->A());
            calls.insert(cf->qname);
            std::vector<IRP> args = call_args(n, cf, kind != "fn" || cf->kind == "method");
            TyP rt = call_ret(cf, n);
            if (kind == "fn") {
                if (cf->cls && cf->kind == "static") {
                    auto c = node("Call", line);
                    c->target = typename_n(cf->cls->qname);
                    c->sym = sym("method", cf->name, cf->cls->qname, true, nullptr, {}, rt);
                    c->args = args;
                    c->type = rt;
                    return c;
                }
                auto c = node("Call", line);
                c->sym = sym("function", cf->name, cf->module->name, true, nullptr, {}, rt);
                c->args = args;
                c->type = rt;
                return c;
            }
            if (kind == "super" || kind == "basecall") {
                if (cf->name == "__init__") {
                    auto c = node("Call", line);
                    c->sym = sym("ctor", "__init__", cf->cls->qname);
                    c->args = args;
                    c->typeargs = {py::V("base")};
                    c->type = VOID();
                    return c;
                }
                auto c = node("Call", line);
                c->target = typename_n(cf->cls->qname);
                c->sym = sym("method", cf->name, cf->cls->qname, false, nullptr, {}, rt);
                c->args = args;
                c->typeargs = {py::V("base")};
                c->type = rt;
                return c;
            }
            IRP recv;
            if (auto* fa = as<Attribute>(n->func)) recv = expr(fa->value);
            auto c = node("Call", line);
            c->target = recv;
            c->sym = sym("method", cf->name, cf->cls->qname, false, nullptr, {}, rt);
            c->args = args;
            c->type = rt;
            return c;
        }
        if (kind == "closure") {   // (new) a nested function's call
            Fn* nf = *p.nested.get(r->A());
            std::vector<IRP> args = call_args(n, nf, false);
            TyP rt = call_ret(nf, n);
            const std::string& fname = cast<pyfront::Name>(n->func)->id;
            std::string owner = nf->parent->qname;
            auto c = node("Call", line);
            c->sym = sym("local", fname, owner, false, nullptr, {}, rt);
            c->args = args;
            c->type = rt;
            return c;
        }
        if (kind == "ctor") {
            const std::string cq = r->A();
            Class* c = *p.classes.get(cq);
            Fn* init = p.find_method(cq, "__init__");
            std::vector<IRP> args = init ? call_args(n, init, true) : std::vector<IRP>{};
            if (init) calls.insert(init->qname);
            classes_made.insert(cq);
            auto nw = node("New", line);
            nw->sym = sym("ctor", c->name, cq);
            if (!c->exception.empty()) {
                if (!init) {
                    args.clear();
                    for (auto* a : n->args) args.push_back(expr(a));
                    if (!args.empty() && !ty_eq(args[0]->type, STRT())) args = {mk_pyfn("str", args, STRT())};
                }
                nw->args = args;
                nw->type = cls_flag(cq, "exception");
                nw->memory = "stack";
                return nw;
            }
            nw->args = args;
            nw->type = cls_flag(cq, "ref");
            nw->memory = "gc";
            return nw;
        }
        if (kind == "exc") {
            std::vector<IRP> args;
            for (auto* a : n->args) args.push_back(expr(a));
            if (!args.empty() && !ty_eq(args[0]->type, STRT())) args = {mk_pyfn("str", args, STRT())};
            std::string name = r->A();
            auto pos = name.find("struct.error");
            if (pos != std::string::npos) name.replace(pos, 12, "StructError");
            auto nw = node("New", line);
            nw->sym = sym("ctor", name, PY);
            nw->args = args;
            nw->type = cls("py." + name);
            return nw;
        }
        if (kind == "getattr_attr") {   // getattr(x, "a", d) where every instance has a: x.a
            T ot = Tof(n->args[0]);
            std::string owner = p.attr_owner(ot.name(), r->A());
            TyP ft = ty(t, n);
            return select_n(expr(n->args[0]), sym("field", r->A(), owner.empty() ? ot.name() : owner, false, ft), ft, line);
        }
        if (kind == "getattr_default") return coerce(expr(n->args[2]), ty(t, n));   // (no class of x's has a: the default)
        if (kind == "builtin") return builtin_call(n, r->A(), t);
        if (kind == "bmethod") {
            IRP c = method_call(n, r->A(), t);
            if (p.extended && r->b && r->A().rfind("re.", 0) == 0 && c->k == "Call") c->typeargs = {py::V(r->B())};
            return c;
        }
        if (kind == "std") return std_call(n, *r, t);
        if (kind == "native_call") {   // native.call("q", data, args...): the question asked (answers.cc), typed
            if (!n->keywords.empty() || n->args.size() < 2) throw Refused("dynamic", "native.call with keywords", line);
            std::vector<IRP> args;
            if (is_none_const(n->args[1])) args.push_back(mk_pyfn("native.no_data", {}, BYTEST(), nullptr, line));
            else args.push_back(expr(n->args[1]));
            for (size_t i = 2; i < n->args.size(); i++) args.push_back(expr(n->args[i]));
            return mk_pyfn("native.ask", args, ty(t, n), nullptr, line, {py::V(r->A())});
        }
        if (kind == "isinstance_variant") {   // (new) a test of the variant's tag
            IRP x = expr(n->args[0]);
            std::vector<py::V> tags;
            std::string ts = r->A();
            size_t a = 0;
            while (!ts.empty() && a <= ts.size()) {
                size_t b = ts.find(',', a);
                tags.push_back(py::V(ts.substr(a, b == std::string::npos ? std::string::npos : b - a)));
                if (b == std::string::npos) break;
                a = b + 1;
            }
            if (tags.empty()) return mk_lit(py::V(false), BOOLT());
            return mk_pyfn("variant.is", {x}, BOOLT(), nullptr, line, tags);
        }
        if (kind == "isinstance") {
            for (auto* a : n->args)
                if (effects(expr(a))) throw Refused("syntax", "isinstance of a computed value", line);
            return mk_lit(py::V(r->A() == "True"), BOOLT());
        }
        throw Refused("dynamic", "call (" + kind + ")", line);
    }

    TyP call_ret(Fn* cf, const pyfront::Call* n) {
        T t = Tof(n);
        if (cf->name == "__init__") return VOID();
        return t.kind != TK::None ? ty(t, n) : VOID();
    }

    std::vector<IRP> call_args(const pyfront::Call* n, Fn* cf, bool bound) {
        std::vector<std::string> names = (bound || (cf->kind != "method" && cf->kind != "property")) ? cf->bound() : cf->params;
        if (cf->kind == "static") names = cf->params;
        std::map<std::string, const A::Expr*> given;
        std::vector<const A::Expr*> pos(n->args.begin(), n->args.end());
        const Res* r = R(n);
        if (as<Attribute>(n->func) && r && r->k == "basecall" && !pos.empty()) pos.erase(pos.begin());
        for (size_t i = 0; i < names.size() && i < pos.size(); i++) given[names[i]] = pos[i];
        for (auto* k : n->keywords) given[k->has_arg ? k->arg : ""] = k->value;
        std::vector<IRP> out;
        std::vector<std::string> all = names;
        all.insert(all.end(), cf->kwonly.begin(), cf->kwonly.end());
        for (auto& name : all) {
            TyP pt = ty(p.param_type(cf, name), n, "parameter " + name);
            auto g = given.find(name);
            if (g != given.end()) out.push_back(coerce(expr(g->second), pt));
            else if (const A::Expr* const* d = cf->defaults.get(name)) {
                if (!is_const(*d)) throw Refused("syntax", "a computed default of " + name, n->pos.lineno);
                out.push_back(coerce(default_lit(cast<Constant>(*d)), pt));
            } else
                throw Refused("conflict", "missing argument " + name, n->pos.lineno);
        }
        return out;
    }

    IRP default_lit(const Constant* d) {
        const ConstValue& v = d->value;
        switch (v.kind) {
            case ConstValue::Kind::None: return mk_lit_none(VOID());
            case ConstValue::Kind::True: return mk_lit(py::V(true), BOOLT());
            case ConstValue::Kind::False: return mk_lit(py::V(false), BOOLT());
            case ConstValue::Kind::Int: return mk_lit(py::V((long long)v.i64), I64());
            case ConstValue::Kind::Str: return mk_lit(py::V(v.text), STRT());
            case ConstValue::Kind::Bytes: return mk_lit(py::V(latin1_to_utf8(v.text)), BYTEST());
            case ConstValue::Kind::Float: return mk_lit(py::V(v.f), F64());
            default: break;
        }
        throw Refused("syntax", "default", 0);
    }

    IRP builtin_call(const pyfront::Call* n, const std::string& name, const T& t) {
        int line = n->pos.lineno;
        TyP rt = t.kind != TK::None ? ty(t, n) : VOID();
        if (name == "any" || name == "all") return any_all(n, name);
        if (name == "next") return next_first(n, rt);
        if (name == "open" && p.extended) {   // (new) open(path, mode, encoding=..., errors=..., newline=...)
            std::vector<IRP> args = {expr(n->args[0])};
            std::string mode = "r";
            const A::Expr* me = n->args.size() > 1 ? n->args[1] : nullptr;
            for (auto* k : n->keywords)
                if (k->has_arg && k->arg == "mode") me = k->value;
            if (me) is_str_const(me, &mode);
            args.push_back(mk_lit(py::V(mode), STRT()));
            for (auto* k : n->keywords)
                if (k->has_arg && k->arg != "mode") args.push_back(mk_pyfn("kw", {expr(k->value)}, VOID(), nullptr, 0, {py::V(k->arg)}));
            return mk_pyfn("open", args, rt, nullptr, line);
        }
        std::vector<IRP> args;
        for (auto* a : n->args) {
            if (auto* g = as<GeneratorExp>(a)) args.push_back(gen_as_list(g));
            else args.push_back(expr(a));
        }
        if (name == "sorted" || name == "list.sort") {
            const A::Expr* key = nullptr;
            const A::Expr* rev = nullptr;
            for (auto* k : n->keywords) {
                if (k->has_arg && k->arg == "key" && !key) key = k->value;
                if (k->has_arg && k->arg == "reverse" && !rev) rev = k->value;
            }
            std::vector<IRP> extra;
            if (key) {
                TyP et = rt->args[0];
                auto* lam = as<pyfront::Lambda>(key);
                if (!lam) throw Refused("dynamic", "key function", line);
                extra.push_back(lambda_expr(lam, {et}));
            } else {
                extra.push_back(mk_lit_none(VOID()));
            }
            if (rev) extra.push_back(truthy(expr(rev)));
            args.insert(args.end(), extra.begin(), extra.end());
            return mk_pyfn("sorted", args, rt, nullptr, line);
        }
        if (name == "dict") {
            if (is_record_literal(n)) return record_value(n);
            throw Refused("syntax", "dict() call", line);
        }
        if (name == "min" || name == "max" || name == "min_iter" || name == "max_iter") {
            for (auto* k : n->keywords)
                if (k->has_arg && k->arg == "default") throw Refused("syntax", name + "(default=)", line);
            for (auto& a : args) a = coerce(a, rt);
        }
        if (name == "print") {
            for (auto* k : n->keywords) args.push_back(mk_pyfn("kw", {expr(k->value)}, VOID(), nullptr, 0, {py::V(k->arg)}));
        } else if (!n->keywords.empty() && name != "enumerate") {
            throw Refused("syntax", name + " keywords", line);
        }
        if (name == "enumerate" && !n->keywords.empty()) args.push_back(expr(n->keywords[0]->value));
        static const std::set<std::string> BARE = {"sorted", "enumerate", "print", "zip", "reversed"};
        return mk_pyfn(BARE.count(name) ? name : "builtins." + name, args, rt, nullptr, line);
    }

    IRP method_call(const pyfront::Call* n, const std::string& name, const T& t) {
        int line = n->pos.lineno;
        TyP rt = t.kind != TK::None ? ty(t, n) : VOID();
        const A::Expr* recv_node = cast<Attribute>(n->func)->value;
        IRP recv = expr(recv_node);
        if (name == "record.get") {
            std::string key;
            bool hk = is_str_const(n->args[0], &key);
            T st = Tof(recv_node);
            auto fs = ctx.records.fields(st.site);
            const RecordField* info = nullptr;
            if (hk)
                for (auto& f_ : fs)
                    if (f_.key == key) info = &f_;
            if (!info) return n->args.size() > 1 ? coerce(expr(n->args[1]), rt) : mk_lit_none(rt);
            IRP sel = field_ref(recv, key, recv_node, line, false, true);
            if (info->required) return coerce(sel, rt);
            if (n->args.size() < 2 || is_none_const(n->args[1])) return sel;
            IRP d = n->args.size() > 1 ? coerce(expr(n->args[1]), rt) : mk_lit_none(rt);
            return mk_pyfn("value_or", {sel, d}, rt, nullptr, line);
        }
        if (name == "list.sort") return builtin_call_sort(n, recv);
        std::vector<IRP> args;
        for (auto* a : n->args) {
            if (auto* g = as<GeneratorExp>(a)) args.push_back(gen_as_list(g));
            else args.push_back(expr(a));
        }
        for (auto* k : n->keywords) args.push_back(mk_pyfn("kw", {expr(k->value)}, VOID(), nullptr, 0, {py::V(k->arg)}));
        if ((name == "list.append" || name == "list.insert" || name == "set.add") && recv->type && recv->type->kind == "class" &&
            !recv->type->args.empty() && !args.empty())
            args.back() = coerce(args.back(), recv->type->args[0]);
        if (name == "bytes.decode" || name == "str.encode") {
            std::string enc = "utf-8", errs = "strict";
            bool ok = true;
            if (!n->args.empty()) enc = [&] {
                std::string s;
                ok = is_str_const(n->args[0], &s);
                return s;
            }();
            else if (const A::Expr* e = [&]() -> const A::Expr* {
                         for (auto* k : n->keywords)
                             if (k->has_arg && k->arg == "encoding") return k->value;
                         return nullptr;
                     }())
                is_str_const(e, &enc);
            if (n->args.size() > 1) is_str_const(n->args[1], &errs);
            else
                for (auto* k : n->keywords)
                    if (k->has_arg && k->arg == "errors") {
                        is_str_const(k->value, &errs);
                        break;
                    }
            enc = lower_s(enc);
            if (enc == "latin-1" || enc == "iso-8859-1") enc = "latin1";
            else if (enc == "utf8") enc = "utf-8";
            return mk_pyfn(name, {mk_lit(py::V(enc), STRT()), mk_lit(py::V(errs), STRT())}, rt, recv, line);
        }
        return mk_pyfn(name, args, rt, recv, line);
    }

    IRP builtin_call_sort(const pyfront::Call* n, const IRP& recv) {
        const A::Expr* key = nullptr;
        const A::Expr* rev = nullptr;
        for (auto* k : n->keywords) {
            if (k->has_arg && k->arg == "key" && !key) key = k->value;
            if (k->has_arg && k->arg == "reverse" && !rev) rev = k->value;
        }
        std::vector<IRP> args;
        TyP et = recv->type->args[0];
        if (key) {
            auto* lam = as<pyfront::Lambda>(key);
            if (!lam) throw Refused("dynamic", "key function", n->pos.lineno);
            args.push_back(lambda_expr(lam, {et}));
        } else {
            args.push_back(mk_lit_none(VOID()));
        }
        if (rev) args.push_back(truthy(expr(rev)));
        return mk_pyfn("list.sort", args, VOID(), recv, n->pos.lineno);
    }

    IRP std_call(const pyfront::Call* n, const Res& r, const T& t) {
        int line = n->pos.lineno;
        TyP rt = t.kind != TK::None ? ty(t, n) : VOID();
        const std::string& name = r.A();
        std::vector<IRP> args;
        for (auto* a : n->args) {
            auto* lam = as<pyfront::Lambda>(a);
            if (lam && (name == "re.sub" || name == "re.subn") && lam->args->args.size() == 1) {   // the replacement a lambda of the match
                args.push_back(lambda_expr(lam, {ty(Tof(AnyNode(lam->args->args[0], AnyNode::Arg)), a, "the match")}));
                continue;
            }
            args.push_back(expr(a));
        }
        if (name.rfind("struct.", 0) == 0) return mk_pyfn(name, args, rt, nullptr, line, {py::V(r.B())});
        if (name == "int.from_bytes") {
            std::string order = "big";
            if (n->args.size() > 1) is_str_const(n->args[1], &order);
            else
                for (auto* k : n->keywords)
                    if (k->has_arg && k->arg == "byteorder") {
                        std::string s;
                        if (is_str_const(k->value, &s) && !s.empty()) order = s;
                        break;
                    }
            bool sg = false;
            for (auto* k : n->keywords)
                if (k->has_arg && k->arg == "signed") {
                    auto* c = as<Constant>(k->value);
                    sg = c && c->value.kind == ConstValue::Kind::True;
                    break;
                }
            return mk_pyfn(name, {args[0], mk_lit(py::V(order), STRT()), mk_lit(py::V(sg), BOOLT())}, rt, nullptr, line);
        }
        if (p.extended && name.rfind("re.", 0) == 0 && r.b) {
            for (auto* k : n->keywords) args.push_back(mk_pyfn("kw", {expr(k->value)}, VOID(), nullptr, 0, {py::V(k->arg)}));
            return mk_pyfn(name, args, rt, nullptr, line, {py::V(r.B())});
        }
        if (p.extended && !n->keywords.empty() && ext_keywords(name)) {
            for (auto* k : n->keywords) args.push_back(mk_pyfn("kw", {expr(k->value)}, VOID(), nullptr, 0, {py::V(k->arg)}));
            return mk_pyfn(name, args, rt, nullptr, line);
        }
        if (!n->keywords.empty()) throw Refused("syntax", name + " keywords", line);
        return mk_pyfn(name, args, rt, nullptr, line);
    }

    static bool ext_keywords(const std::string& name) {
        return name == "os.makedirs" || name == "shutil.rmtree" || name.rfind("re.", 0) == 0;
    }
};

// ------------------------------------------------------------------------------------ the units
static void types_of(const TyP& t, std::set<std::string>& out) {
    if (!t) return;
    if (t->kind == "class") {
        if (t->name.rfind("py.", 0) != 0) out.insert(t->name);
        for (auto& a : t->args) types_of(a, out);
    }
}
static std::set<std::string> types_in(const IRP& md) {
    std::set<std::string> out;
    ir_walk(md, [&](const IRP& x) { types_of(x->type, out); });
    types_of(md->ret, out);
    for (auto& prm : md->params) types_of(prm->type, out);
    return out;
}

static std::string owner_module(Program& p, const std::string& q) {
    if (p.modules.has(q)) return q;
    auto pos = q.rfind('.');
    return pos == std::string::npos ? q : q.substr(0, pos);
}

static size_t attr_order(Program& p, Class* c, const std::string& attr) {
    std::vector<Fn*> meths;
    for (auto& [_, f] : c->methods) meths.push_back(f);
    std::stable_sort(meths.begin(), meths.end(), [](Fn* a, Fn* b) {
        bool ai = a->name != "__init__", bi = b->name != "__init__";
        if (ai != bi) return ai < bi;
        return a->node->pos.lineno < b->node->pos.lineno;
    });
    std::vector<std::string> order;
    for (Fn* f : meths)
        for (auto& n : walk(AnyNode(f->node)))
            if (auto* at = as<Attribute>(n.expr()))
                if (at->ctx == ExprCtx::Store && std::find(order.begin(), order.end(), at->attr) == order.end()) order.push_back(at->attr);
    auto it = std::find(order.begin(), order.end(), attr);
    return it != order.end() ? (size_t)(it - order.begin()) : order.size();
}

static py::V const_value(const ConstValue& v) {
    switch (v.kind) {
        case ConstValue::Kind::True: return py::V(true);
        case ConstValue::Kind::False: return py::V(false);
        case ConstValue::Kind::Int: return v.fits_i64 ? py::V((long long)v.i64) : py::V(v.text);
        case ConstValue::Kind::Float: return py::V(v.f);
        case ConstValue::Kind::Str: return py::V(v.text);
        case ConstValue::Kind::Bytes: return py::V(latin1_to_utf8(v.text));
        default: return py::V();
    }
}

TranslateResult translate(Program& p, const std::string& int_policy, const std::vector<std::string>* wanted_mods) {
    TranslateResult out;
    Context ctx(p, int_policy);
    std::map<std::string, std::unique_ptr<Eval>> evals;
    // lower every function
    for (auto& [q, f] : p.fns) {
        Lowered L;
        auto ev = std::make_unique<Eval>(p, f, true);
        try {
            p.suspend = 0;
            ev->run();
            p.suspend = 0;
        } catch (const std::exception& e) {
            p.suspend = 0;
            L.refused = Refused("frontend-error", std::string("RuntimeError: ") + e.what(), f->node->pos.lineno);
            out.lowered[q] = L;
            continue;
        }
        L.all = ev->issues;
        FnLower fl(ctx, f, *ev);
        L.has_fl = true;
        try {
            if (p.extended && f->generator) {   // (new) a generator: its class, and the function making one
                auto both = fl.lower_generator();
                L.extra = both[0];
                L.md = both[1];
            } else
            L.md = fl.lower();
        } catch (const Refused& r) {
            L.refused = r;
        } catch (const std::exception& e) {
            L.refused = Refused("frontend-error", std::string("RuntimeError: ") + e.what(), f->node->pos.lineno);
        }
        L.calls = fl.calls;
        L.globals_used = fl.globals_used;
        out.lowered[q] = std::move(L);
    }
    // (new) the specializations, each lowered as its own overload
    std::vector<std::pair<std::string, Fn*>> spec_list(p.specs.begin(), p.specs.end());
    for (auto& [q, f] : spec_list) {
        Lowered L;
        Eval ev(p, f, true);
        try {
            p.suspend = 0;
            ev.run();
            FnLower fl(ctx, f, ev);
            L.has_fl = true;
            try {
                L.md = fl.lower();
            } catch (const Refused& r) {
                L.refused = r;
            }
            L.calls = fl.calls;
            L.globals_used = fl.globals_used;
        } catch (const std::exception& e) {
            L.refused = Refused("frontend-error", std::string("RuntimeError: ") + e.what(), f->node->pos.lineno);
        }
        out.lowered[q] = std::move(L);
    }
    // module names the translations read
    std::map<std::pair<std::string, std::string>, Refused> bad_globals;
    std::map<std::pair<std::string, std::string>, IRP> global_decls;
    std::map<std::pair<std::string, std::string>, std::set<std::string>> global_calls;   // what each name's value calls
    std::map<std::pair<std::string, std::string>, std::set<std::pair<std::string, std::string>>> global_reads;
    std::set<std::pair<std::string, std::string>> needed;
    for (auto& [q, L] : out.lowered)
        if (L.has_fl && L.md) needed.insert(L.globals_used.begin(), L.globals_used.end());
    std::map<std::string, std::unique_ptr<ModuleEval>> mevals;
    std::vector<std::unique_ptr<Fn>> modfns;
    // a module name the module's own statements change (TABLE[k] = v in a loop, TABLE.update(...), f(TABLE)):
    // its value is more than its first assignment, which is all a declaration here would hold
    auto changed_by_module = [](Module* mm, const std::string& name) {
        auto root = [](const A::Expr* e) -> std::string {
            while (e) {
                if (auto* nm = as<pyfront::Name>(e)) return nm->id;
                if (auto* sb = as<Subscript>(e)) e = sb->value;
                else if (auto* at = as<Attribute>(e)) e = at->value;
                else return "";
            }
            return "";
        };
        for (auto* st : mm->statements)
            for (const AnyNode& n : walk(AnyNode(st))) {
                if (auto* s2 = n.stmt()) {
                    if (auto* a = as<pyfront::Assign>(s2)) {
                        for (auto* t : a->targets)
                            if (!as<pyfront::Name>(t) && root(t) == name) return true;
                    } else if (auto* g = as<AugAssign>(s2)) {
                        if (root(g->target) == name) return true;
                    } else if (s2->kind == StmtKind::Delete) {
                        for (const AnyNode& d : walk(AnyNode(s2)))
                            if (auto* nm = as<pyfront::Name>(d.expr()); nm && nm->id == name) return true;
                    }
                }
                if (auto* c = as<pyfront::Call>(n.expr())) {
                    if (root(c->func) == name && c->func->kind == ExprKind::Attribute) return true;   // TABLE.update(...)
                    for (auto* a : c->args)
                        if (root(a) == name) return true;   // f(TABLE): f may change it
                }
            }
        return false;
    };
    for (auto& key : needed) {
        auto& [mod, name] = key;
        Module* mm = *p.modules.get(mod);
        if (changed_by_module(mm, name)) {
            bad_globals.emplace(key, Refused("dynamic", "module name " + name + " is changed by the module's statements"));
            continue;
        }
        const std::vector<const A::Expr*>* values = mm->assigns.get(name);
        size_t nv = values ? values->size() : 0;
        if (nv != 1 || !(*values)[0]) {
            bad_globals.emplace(key, Refused("dynamic", "module name " + name + " assigned " + std::to_string(nv) + " times"));
            continue;
        }
        if (!mevals.count(mod)) {
            auto me = std::make_unique<ModuleEval>(p, mm, true);
            me->run();
            mevals[mod] = std::move(me);
        }
        // the module's top level, seen as a function
        auto mf = std::unique_ptr<Fn>(new Fn(*p.fns.items.front().second));
        mf->qname = mod + ".<module>";
        mf->module = mm;
        mf->cls = nullptr;
        mf->kind = "function";
        mf->params.clear();
        mf->kwonly.clear();
        mf->name = "<module>";
        Fn* mfr = mf.get();
        modfns.push_back(std::move(mf));
        FnLower fl(ctx, mfr, *mevals[mod]);
        const A::Expr* v0 = (*values)[0];
        try {
            TyP t = ctx.ty(p.global_type(mod, name), v0->pos.lineno, "module name " + name);
            IRP init = fl.coerce(fl.expr(v0), t);
            if (!fl.pre.empty()) throw Refused("syntax", "module name " + name + " needs statements", v0->pos.lineno);
            auto fd = node("FieldDecl", v0->pos.lineno);
            fd->sym = sym("global", name, mod, true, t);
            fd->type = t;
            fd->init = init;
            fd->mods = {"static"};
            if (t->kind == "i64" || t->kind == "bool" || t->kind == "f64" || t->name == "py.str" || t->name == "py.bytes" || t->name == "py.tuple")
                fd->mods.push_back("const");
            global_decls[key] = fd;
            global_calls[key] = fl.calls;
            global_reads[key] = fl.globals_used;
            for (auto& g : fl.globals_used)
                if (!needed.count(g) && g != key) bad_globals.insert_or_assign(key, Refused("syntax", "module name " + name + " refers to another"));
        } catch (const Refused& r) {
            bad_globals.insert_or_assign(key, r);
        } catch (const std::exception& e) {
            bad_globals.insert_or_assign(key, Refused("frontend-error", std::string("RuntimeError: ") + e.what()));
        }
    }
    for (bool again = true; again;) {   // (extended: a name whose value reads a refused name or calls a refused function is refused too)
    again = false;
    for (auto& [q, L] : out.lowered) {
        if (!L.has_fl || !L.md) continue;
        for (auto& g : L.globals_used) {
            auto it = bad_globals.find(g);
            if (it != bad_globals.end()) {
                L.md = nullptr;
                L.refused = Refused("calls-refused", "module name " + g.second + " (" + trunc(it->second.issue.reason(), 60) + ")");
                break;
            }
        }
    }
    // close the refusals: a caller of a refused function is refused
    bool changed = true;
    while (changed) {
        changed = false;
        for (auto& [q, L] : out.lowered) {
            if (!L.md || !L.has_fl) continue;
            for (auto& c : L.calls) {
                auto it = out.lowered.find(c);
                if (it == out.lowered.end()) {
                    int line = L.md->line;
                    L.md = nullptr;
                    L.refused = Refused("calls-refused", rsplit1(c) + " (not in the translation)", line);
                    changed = true;
                    break;
                }
                if (!it->second.md) {
                    int line = L.md->line;
                    L.md = nullptr;
                    L.refused = Refused("calls-refused", split2_last(c) + " (" + trunc(it->second.refused->issue.reason(), 60) + ")", line);
                    changed = true;
                    break;
                }
            }
        }
    }
    if (p.extended)
        for (auto& [key, d] : global_decls) {
            if (bad_globals.count(key)) continue;
            for (auto& g : global_reads[key]) {
                auto bg = bad_globals.find(g);
                if (g != key && (bg != bad_globals.end() || !global_decls.count(g))) {
                    bad_globals.insert_or_assign(key, Refused("calls-refused", "module name " + g.second +
                                                 (bg != bad_globals.end() ? " (" + trunc(bg->second.issue.reason(), 60) + ")" : "")));
                    again = true;
                    break;
                }
            }
            if (bad_globals.count(key)) continue;
            for (auto& c : global_calls[key]) {
                auto it = out.lowered.find(c);
                if (it == out.lowered.end() || !it->second.md) {
                    bad_globals.insert_or_assign(key, Refused("calls-refused", split2_last(c) + " (from module name " + key.second + ")"));
                    again = true;
                    break;
                }
            }
        }
    }
    if (p.extended)   // (a refused name is not written)
        for (auto& [key, _] : bad_globals) global_decls.erase(key);
    // the types every translated function uses (and the types of their fields, to a fixpoint)
    std::set<std::string> used_all;
    for (auto& [q, L] : out.lowered)
        if (L.has_fl && L.md) {
            auto s = types_in(L.md);
            used_all.insert(s.begin(), s.end());
        }
    for (auto& [_, d] : global_decls) types_of(d->type, used_all);
    std::map<std::string, int> recs;
    for (auto& [s, n] : ctx.records.names) recs[ctx.records.module[s] + "." + n] = s;
    std::vector<std::string> todo(used_all.begin(), used_all.end());
    while (!todo.empty()) {
        std::string q = todo.back();
        todo.pop_back();
        std::set<std::string> more;
        if (recs.count(q)) {
            for (auto& f_ : ctx.records.fields(recs[q])) {
                try {
                    types_of(ctx.ty(f_.type, 0, "field " + f_.key), more);
                } catch (const Refused&) {
                }
            }
        } else if (p.classes.has(q)) {
            auto ai = p.attrs.find(q);
            if (ai != p.attrs.end()) {
                std::vector<std::string> names;
                for (auto& [a, _] : ai->second) names.push_back(a);
                for (auto& a : names) {
                    try {
                        auto at = p.attr_type(q, a);
                        if (at) types_of(ctx.ty(*at, 0, "attribute " + a), more);
                    } catch (const Refused&) {
                    }
                }
            }
            for (auto& b : (*p.classes.get(q))->bases) more.insert(b);
        }
        for (auto& x : more)
            if (!used_all.count(x)) {
                used_all.insert(x);
                todo.push_back(x);
            }
    }
    // the units
    std::set<std::string> want;
    if (wanted_mods) want.insert(wanted_mods->begin(), wanted_mods->end());
    else
        for (auto& [m, _] : p.modules) want.insert(m);
    std::vector<std::string> mtodo(want.begin(), want.end());
    std::sort(mtodo.begin(), mtodo.end());
    size_t mi = 0;
    while (mi < mtodo.size()) {
        std::string mod = mtodo[mi++];
        if (out.modules.count(mod) || !p.modules.has(mod)) continue;
        Module* m = *p.modules.get(mod);
        ModuleResult r;
        r.name = mod;
        std::set<std::string> used;
        for (auto& [q, f] : p.fns) {
            if (f->module != m) continue;
            auto it = out.lowered.find(q);
            if (it == out.lowered.end()) continue;
            r.lines[q] = f->lines;
            if (!it->second.md) r.refused.emplace_back(q, it->second.refused->issue);
            else {
                if (it->second.extra) r.extra.push_back(it->second.extra);
                r.translated.emplace_back(q, it->second.md);
                auto s = types_in(it->second.md);
                used.insert(s.begin(), s.end());
            }
        }
        for (auto& [k, d] : global_decls)
            if (k.first == mod) types_of(d->type, used);
        auto translated_md = [&](const std::string& q) -> IRP {
            for (auto& [qq, md] : r.translated)
                if (qq == q) return md;
            return nullptr;
        };
        auto refused_issue = [&](const std::string& q) -> const Issue* {
            for (auto& [qq, i] : r.refused)
                if (qq == q) return &i;
            return nullptr;
        };
        std::vector<IRP> types;
        // records declared here that the translation uses
        for (auto& [s, name] : ctx.records.names) {
            std::string q = ctx.records.module[s] + "." + name;
            if (ctx.records.module[s] != mod) continue;
            bool any_ref = false;
            if (!(used_all.count(q) || used.count(q)))
            for (auto& [s2, n2] : ctx.records.names) {
                std::string q2 = ctx.records.module[s2] + "." + n2;
                if (used.count(q2) && s2 != s)
                    for (auto& f2 : ctx.records.fields(s2)) {
                        try {
                            std::set<std::string> ts;
                            types_of(ctx.ty(f2.type, 0), ts);
                            if (ts.count(q)) any_ref = true;
                        } catch (const Refused&) {
                        }
                    }
            }
            if (!(used_all.count(q) || used.count(q) || any_ref)) continue;
            std::vector<IRP> members;
            try {
                for (auto& f_ : ctx.records.fields(s)) {
                    TyP t = ctx.ty(f_.type, 0, "field " + f_.key);
                    auto fd = node("FieldDecl");
                    fd->sym = sym("field", f_.key, q, false, t);
                    fd->type = t;
                    if (!f_.required) fd->mods = {"optional"};
                    members.push_back(fd);
                }
            } catch (const Refused& e) {
                auto cm = node("Comment");
                cm->text = "a field has no type: " + e.issue.reason();
                members = {cm};
            }
            auto cd = node("ClassDecl");
            cd->name = name;
            cd->qname = q;
            cd->dkind = "record";
            if (ctx.records.is_ref(s)) cd->mods = {"ref"};
            cd->members = members;
            types.push_back(cd);
        }
        // classes
        for (auto& [_, c] : m->classes) {
            std::vector<IRP> methods;
            for (auto& [__, f] : c->methods)
                if (IRP md = translated_md(f->qname)) methods.push_back(md);
            if (methods.empty() && !used.count(c->qname) && !used_all.count(c->qname)) continue;
            std::vector<IRP> members;
            OMap<T> attrs;
            auto ai = p.attrs.find(c->qname);
            if (ai != p.attrs.end()) attrs = ai->second;
            auto any_assigned = p.assigned_attrs_any(c->qname);
            for (auto& [name, __] : c->class_attrs)
                if (any_assigned.count(name) && !attrs.has(name)) {
                    auto it = p.class_attr_types.find(c->qname + "\x1f" + name);
                    attrs.set(name, it == p.class_attr_types.end() ? BOTTOM() : it->second);
                }
            std::vector<std::string> anames;
            for (auto& [a, __] : attrs) anames.push_back(a);
            std::vector<size_t> ord;
            for (auto& a : anames) ord.push_back(attr_order(p, c, a));
            std::vector<size_t> idx(anames.size());
            for (size_t i = 0; i < idx.size(); i++) idx[i] = i;
            std::stable_sort(idx.begin(), idx.end(), [&](size_t a, size_t b) { return ord[a] < ord[b]; });
            for (size_t i : idx) {
                const std::string& attr = anames[i];
                TyP it;
                try {
                    auto at = p.attr_type(c->qname, attr);
                    if (!at) throw Refused("frontend-error", "attribute");
                    it = ctx.ty(*at, 0, "attribute " + attr);
                } catch (const Refused& e) {
                    auto cm = node("Comment");
                    cm->text = attr + ": not translated (" + e.issue.reason() + ")";
                    members.push_back(cm);
                    continue;
                }
                IRP init;
                if (const A::Expr* const* v = c->class_attrs.get(attr)) {
                    auto* cc = as<Constant>(*v);
                    if (cc && cc->value.kind != ConstValue::Kind::None && cc->value.kind != ConstValue::Kind::Bytes) init = mk_lit(const_value(cc->value), it);
                }
                auto fd = node("FieldDecl");
                fd->sym = sym("field", attr, c->qname, false, it);
                fd->type = it;
                fd->init = init;
                members.push_back(fd);
            }
            for (auto& [name, v] : c->class_attrs) {
                if (attrs.has(name) || p.attr_owner(c->qname, name) != c->qname) continue;
                auto tt = p.class_attr_types.find(c->qname + "\x1f" + name);
                auto* cc = as<Constant>(v);
                if (tt == p.class_attr_types.end() || !cc) continue;
                TyP it;
                try {
                    it = ctx.ty(tt->second, 0, "class attribute " + name);
                } catch (const Refused&) {
                    continue;
                }
                auto fd = node("FieldDecl");
                fd->sym = sym("field", name, c->qname, true, it);
                fd->type = it;
                fd->init = cc->value.kind == ConstValue::Kind::None ? mk_lit_none(it) : mk_lit(const_value(cc->value), it);
                fd->mods = {"static", "const"};
                members.push_back(fd);
            }
            members.insert(members.end(), methods.begin(), methods.end());
            for (auto& [__, f] : c->methods)
                if (const Issue* is = refused_issue(f->qname)) {
                    auto cm = node("Comment");
                    cm->text = "not translated: " + f->name + " -- " + is->reason();
                    members.push_back(cm);
                }
            auto cd = node("ClassDecl");
            cd->name = c->name;
            cd->qname = c->qname;
            cd->dkind = "class";
            if (!c->exception.empty()) cd->mods = {"exception:" + c->exception};
            if (!c->bases.empty()) cd->extends = cls(c->bases[0]);
            cd->members = members;
            types.push_back(cd);
        }
        types.insert(types.end(), r.extra.begin(), r.extra.end());   // (new) generator classes
        // the module's functions and names
        std::vector<IRP> members;
        std::vector<std::pair<int, IRP>> gd;
        for (auto& [k, d] : global_decls)
            if (k.first == mod) gd.emplace_back(d->line, d);
        std::stable_sort(gd.begin(), gd.end(), [](auto& a, auto& b) { return a.first < b.first; });
        for (auto& [_, d] : gd) members.push_back(d);
        for (auto& [_, f] : m->functions) {
            auto so = p.specs_of.find(f->qname);
            if (so != p.specs_of.end() && !translated_md(f->qname)) {   // (new) its overloads, when all translate
                bool all = true;
                for (Fn* c : so->second)
                    if (!out.lowered[c->qname].md) all = false;
                if (all) {
                    for (Fn* c : so->second) members.push_back(out.lowered[c->qname].md);
                    continue;
                }
            }
            if (IRP md = translated_md(f->qname)) members.push_back(md);
            else if (const Issue* is = refused_issue(f->qname)) {
                auto cm = node("Comment");
                cm->text = "not translated: " + f->name + " -- " + is->reason();
                members.push_back(cm);
            }
        }
        auto mcd = node("ClassDecl");
        mcd->name = rsplit1(mod);
        mcd->qname = mod;
        mcd->dkind = "module";
        mcd->members = members;
        types.push_back(mcd);
        std::set<std::string> deps;
        for (auto& q : used) {
            std::string own = owner_module(p, q);
            if (own != mod && p.modules.has(own)) deps.insert(own);
        }
        for (auto& [q, md] : r.translated)
            ir_walk(md, [&](const IRP& x) {
                if (x->sym && x->sym->owner && *x->sym->owner != "py") {
                    std::string own = owner_module(p, *x->sym->owner);
                    if (p.modules.has(own) && own != mod) deps.insert(own);
                }
            });
        for (auto& decl : types) {
            std::set<std::string> names;
            if (decl->extends) names.insert(decl->extends->name);
            for (auto& x : decl->members)
                if (x->k == "FieldDecl") types_of(x->type, names);
            for (auto& q : names) {
                std::string own = owner_module(p, q);
                if (own != mod && p.modules.has(own)) deps.insert(own);
            }
        }
        auto u = node("Unit");
        u->path = m->path;
        u->package = mod;
        u->imports.assign(deps.begin(), deps.end());
        u->members = types;
        r.unit = u;
        for (auto& dep : u->imports)
            if (!out.modules.count(dep) && std::find(mtodo.begin() + mi, mtodo.end(), dep) == mtodo.end()) mtodo.push_back(dep);
        out.modules[mod] = std::move(r);
    }
    return out;
}

}  // namespace pyengine
