// call.cc -- calling: Python's argument binding (positional, keyword, defaults, *args, **kwargs,
// positional-only and keyword-only parameters, with CPython's TypeError messages), function objects
// made from C++ functions and closures, builtins, bound methods, calling a class, a callable instance.
#include <cstring>

#include "rt.hpp"

namespace py {

static int g_depth = 0;
int g_recursion_limit = 1000;

void CallArgs::add(const kw_arg& k) { kw.push_back({interned(k.name), k.value}); }
CallArgs args_of_(std::initializer_list<call_item> a) {
    CallArgs c;
    for (auto& x : a) {
        switch (x.kind) {
        case 0: c.pos.push_back(x.v); break;
        case 1: c.kw.push_back({interned(x.name), x.v}); break;
        case 2: c.add(star_arg{x.v}); break;
        default: c.add(starstar_arg{x.v}); break;
        }
    }
    return c;
}
void CallArgs::add(const star_arg& s) {
    if (is_list(s.value)) { for (auto& x : list_items(s.value)) pos.push_back(x); return; }
    if (is_tuple(s.value)) { for (auto& x : tuple_items(s.value)) pos.push_back(x); return; }
    for (auto& x : collect(s.value)) pos.push_back(x);
}
void CallArgs::add(const starstar_arg& s) {
    if (is_dict(s.value)) {
        for (auto& e : static_cast<DictObj*>(s.value.ref())->entries)
            if (!e.key.is_unbound()) {
                if (!is_str(e.key)) raise(E_TypeError, "keywords must be strings");
                kw.push_back({e.key, e.value});
            }
        return;
    }
    var keys = s.value.call("keys");
    for (auto& k : collect(keys)) kw.push_back({k, s.value.getitem(k)});
}

// ------------------------------------------------------------------------------------------ functions
static void parse_sig(FuncObj* f, const char* sig) {
    // "a, b=, /, *rest, key=, **kw"
    std::string s = sig ? sig : "";
    size_t i = 0;
    bool kwonly = false;
    std::vector<Param> ps;
    int slash_at = -1;
    while (i < s.size()) {
        while (i < s.size() && (s[i] == ' ' || s[i] == ',')) i++;
        if (i >= s.size()) break;
        size_t j = i;
        while (j < s.size() && s[j] != ',') j++;
        std::string tok = s.substr(i, j - i);
        while (!tok.empty() && tok.back() == ' ') tok.pop_back();
        i = j;
        if (tok == "/") { slash_at = (int)ps.size(); continue; }
        if (tok == "*") { kwonly = true; continue; }
        Param p;
        if (tok.rfind("**", 0) == 0) { p.kind = Param::VarKw; p.name = tok.substr(2); }
        else if (tok[0] == '*') { p.kind = Param::VarArgs; p.name = tok.substr(1); kwonly = true; }
        else {
            if (tok.back() == '=') { p.has_default = true; tok.pop_back(); }
            p.name = tok;
            p.kind = kwonly ? Param::KwOnly : Param::Normal;
        }
        ps.push_back(p);
    }
    if (slash_at >= 0)
        for (int k = 0; k < slash_at; k++) ps[k].kind = Param::PosOnly;
    f->params = ps;
    f->npos = 0;
    for (size_t k = 0; k < ps.size(); k++) {
        if (ps[k].kind == Param::PosOnly || ps[k].kind == Param::Normal) f->npos++;
        if (ps[k].kind == Param::VarArgs) f->varargs = (int)k;
        if (ps[k].kind == Param::VarKw) f->varkw = (int)k;
    }
}

var make_function(const char* name, const char* qualname, const char* sig, std::vector<var> defaults,
                  invoker fn, int nslots, bool generator, const char* module) {
    auto* f = new FuncObj();
    f->type = T_function;
    f->name = name;
    f->qualname = qualname ? qualname : name;
    f->module = module ? module : "";
    parse_sig(f, sig);
    f->defaults.resize(f->params.size());
    size_t di = 0;
    for (size_t k = 0; k < f->params.size(); k++)
        if (f->params[k].has_default && di < defaults.size()) f->defaults[k] = defaults[di++];
    f->fn = std::move(fn);
    f->nslots = nslots;
    f->generator = generator;
    return var(f);
}

static std::string fname(FuncObj* f) { return f->qualname.empty() ? f->name : f->qualname; }

static std::string missing_list(const std::vector<std::string>& m) {
    std::string out;
    for (size_t i = 0; i < m.size(); i++) {
        if (i) out += (i + 1 == m.size()) ? (m.size() > 2 ? ", and " : " and ") : ", ";
        out += "'" + m[i] + "'";
    }
    return out;
}

// bind a call's arguments to the parameters' slots, Python's rules and messages
static void bind_args(FuncObj* f, const var* self, const CallArgs& a, var* slots) {
    size_t np = f->params.size();
    size_t npos_given = a.pos.size() + (self ? 1 : 0);
    auto posarg = [&](size_t i) -> const var& { return self ? (i == 0 ? *self : a.pos[i - 1]) : a.pos[i]; };
    size_t npos = (size_t)f->npos;
    size_t k = 0;
    for (; k < npos && k < npos_given; k++) slots[k] = posarg(k);
    if (npos_given > npos) {
        if (f->varargs < 0) {
            size_t with_dflt = 0;
            for (size_t q = 0; q < npos; q++) if (f->params[q].has_default) with_dflt++;
            std::string takes = with_dflt ? "from " + std::to_string(npos - with_dflt) + " to " + std::to_string(npos)
                                          : std::to_string(npos);
            raise(E_TypeError, fname(f) + "() takes " + takes + " positional argument" + (npos == 1 && !with_dflt ? "" : "s") +
                                   " but " + std::to_string(npos_given) + (npos_given == 1 ? " was" : " were") + " given");
        }
        std::vector<var> extra;
        for (size_t q = npos; q < npos_given; q++) extra.push_back(posarg(q));
        slots[f->varargs] = new_tuple(std::move(extra));
    } else if (f->varargs >= 0) {
        slots[f->varargs] = new_tuple();
    }
    var kwd;
    if (f->varkw >= 0) { kwd = new_dict_obj(); slots[f->varkw] = kwd; }
    for (auto& [kn, kv] : a.kw) {
        const std::string& n = str_utf8(kn);
        bool placed = false;
        for (size_t q = 0; q < np; q++) {
            const Param& p = f->params[q];
            if ((p.kind == Param::Normal || p.kind == Param::KwOnly) && p.name == n) {
                if (!slots[q].is_unbound()) raise(E_TypeError, fname(f) + "() got multiple values for argument '" + n + "'");
                slots[q] = kv;
                placed = true;
                break;
            }
        }
        if (placed) continue;
        if (kwd.is_unbound()) {
            for (size_t q = 0; q < np; q++)
                if (f->params[q].kind == Param::PosOnly && f->params[q].name == n)
                    raise(E_TypeError, fname(f) + "() got some positional-only arguments passed as keyword arguments: '" + n + "'");
            raise(E_TypeError, fname(f) + "() got an unexpected keyword argument '" + n + "'");
        }
        static_cast<DictObj*>(kwd.ref())->set(kn, kv);
    }
    std::vector<std::string> missing_pos, missing_kw;
    for (size_t q = 0; q < np; q++) {
        const Param& p = f->params[q];
        if (p.kind == Param::VarArgs || p.kind == Param::VarKw) continue;
        if (!slots[q].is_unbound()) continue;
        if (p.has_default) { slots[q] = f->defaults[q]; continue; }
        (p.kind == Param::KwOnly ? missing_kw : missing_pos).push_back(p.name);
    }
    if (!missing_pos.empty())
        raise(E_TypeError, fname(f) + "() missing " + std::to_string(missing_pos.size()) + " required positional argument" +
                               (missing_pos.size() == 1 ? "" : "s") + ": " + missing_list(missing_pos));
    if (!missing_kw.empty())
        raise(E_TypeError, fname(f) + "() missing " + std::to_string(missing_kw.size()) + " required keyword-only argument" +
                               (missing_kw.size() == 1 ? "" : "s") + ": " + missing_list(missing_kw));
}

var make_generator_(FuncObj* f, std::vector<var> slots);   // gen.cc

struct depth_guard {
    depth_guard() {
        if (++g_depth > g_recursion_limit) {
            --g_depth;
            raise(E_RecursionError, "maximum recursion depth exceeded");
        }
    }
    ~depth_guard() { --g_depth; }
};

// the judge's call trace: PYRT_TRACE=<file> PYRT_TRACE_PREFIX=<module prefix> records every call of a
// function of those modules -- its parameters' reprs and its result's (or the exception's type), each as
// the FNV-1a hash of the repr with addresses masked -- for comparing with the Python run's
struct Trace {
    FILE* f = nullptr;
    std::string prefix;
    bool on = false;
    int depth = 0;
};
static Trace& trace() {
    static Trace* t = [] {
        auto* x = new Trace();
        const char* p = getenv("PYRT_TRACE");
        if (p && *p) {
            x->f = fopen(p, "wb");
            const char* pre = getenv("PYRT_TRACE_PREFIX");
            x->prefix = pre ? pre : "";
            x->on = x->f != nullptr;
        }
        return x;
    }();
    return *t;
}
static std::string masked_hash(const var& v) {
    std::string r;
    try {
        r = repr_(v);
    } catch (...) {
        r = "<repr failed>";
    }
    std::string m;
    for (size_t i = 0; i < r.size(); i++) {
        if (r[i] == '0' && i + 1 < r.size() && r[i + 1] == 'x') {
            m += "0x?";
            i += 2;
            while (i < r.size() && isxdigit((unsigned char)r[i])) i++;
            i--;
            continue;
        }
        m.push_back(r[i]);
    }
    uint64_t h = 1469598103934665603ULL;
    for (unsigned char c : m) { h ^= c; h *= 1099511628211ULL; }
    char b[24];
    snprintf(b, sizeof b, "%016llx", (unsigned long long)h);
    return b;
}
static var traced_call(FuncObj* f, var* slots, size_t n) {
    Trace& tr = trace();
    std::string head = f->module + ":" + f->qualname + "(";
    for (size_t i = 0; i < n; i++) head += (i ? "," : "") + masked_hash(slots[i]);
    head += ")";
    var r;
    try {
        r = f->fn(slots);
    } catch (error& e) {
        fprintf(tr.f, "%s raise %s\n", head.c_str(), type_of(e.value)->name.c_str());
        throw;
    }
    fprintf(tr.f, "%s -> %s\n", head.c_str(), masked_hash(r).c_str());
    return r;
}

// the functions being run (their qualnames), for tracebacks; direct C++ calls between module functions are
// not in it
static thread_local std::vector<const FuncObj*>* t_calls = nullptr;
struct call_frame {
    explicit call_frame(const FuncObj* f) {
        if (!t_calls) t_calls = new std::vector<const FuncObj*>();
        t_calls->push_back(f);
    }
    ~call_frame() { t_calls->pop_back(); }
};
std::string call_stack_() {
    std::string o;
    if (!t_calls) return o;
    for (auto* f : *t_calls) o += "  in " + f->module + "." + f->qualname + "\n";
    return o;
}

static var call_function(FuncObj* f, const var* self, const CallArgs& a) {
    size_t n = f->params.size();
    call_frame frame(f);
    if (trace().on && !f->generator && f->module.compare(0, trace().prefix.size(), trace().prefix) == 0) {
        depth_guard g;
        std::vector<var> slots(n);
        bind_args(f, self, a, slots.data());
        return traced_call(f, slots.data(), n);
    }
    if (f->generator) {
        std::vector<var> slots(n);
        bind_args(f, self, a, slots.data());
        return make_generator_(f, std::move(slots));
    }
    depth_guard g;
    if (n <= 8) {
        var slots[8];
        bind_args(f, self, a, slots);
        return f->fn(slots);
    }
    std::vector<var> slots(n);
    bind_args(f, self, a, slots.data());
    return f->fn(slots.data());
}

var call_obj(const var& f, const CallArgs& a) {
    if (!f.is_ref()) raise(E_TypeError, "'" + tname(f) + "' object is not callable");
    Obj* o = f.ref();
    TypeObj* t = o->type;
    if (t == T_function) return call_function(static_cast<FuncObj*>(o), nullptr, a);
    if (t == T_native) {
        auto* nf = static_cast<NativeObj*>(o);
        if (nf->self.is_unbound()) return nf->fn(a);
        CallArgs b;
        b.pos.reserve(a.pos.size() + 1);
        b.pos.push_back(nf->self);
        b.pos.insert(b.pos.end(), a.pos.begin(), a.pos.end());
        b.kw = a.kw;
        return nf->fn(b);
    }
    if (t == T_method) {
        auto* m = static_cast<MethodObj*>(o);
        if (m->func.is_ref() && m->func.ref()->type == T_function) return call_function(static_cast<FuncObj*>(m->func.ref()), &m->self, a);
        CallArgs b;
        b.pos.push_back(m->self);
        b.pos.insert(b.pos.end(), a.pos.begin(), a.pos.end());
        b.kw = a.kw;
        return call_obj(m->func, b);
    }
    if (t->layout == Layout::Type && (t == T_type || is_subtype(t, T_type))) {
        auto* cls = static_cast<TypeObj*>(o);
        // a metaclass's own __call__
        if (t != T_type) {
            if (const var* c = t->lookup("__call__"); c && c->is_ref()) {
                if (c->ref()->type == T_function) return call_function(static_cast<FuncObj*>(c->ref()), &f, a);
                return call_obj(bind(*c, f, t), a);
            }
        }
        return call_type(cls, a);
    }
    if (t == T_staticmethod) return call_obj(static_cast<WrapObj*>(o)->f, a);
    const var* c = t->lookup("__call__");
    if (!c) {
        static bool debug = getenv("PYRT_DEBUG") != nullptr;
        if (debug) fprintf(stderr, "pyrt: calling %.200s\n", repr_(f).c_str());
        raise(E_TypeError, "'" + t->name + "' object is not callable");
    }
    if (c->is_ref() && c->ref()->type == T_function) return call_function(static_cast<FuncObj*>(c->ref()), &f, a);
    return call_obj(bind(*c, f, t), a);
}
var var::call_(const CallArgs& a) const { return call_obj(*this, a); }
var call0(const var& f) { return call_obj(f, CallArgs()); }
var call1(const var& f, const var& x) {
    CallArgs a;
    a.pos.push_back(x);
    return call_obj(f, a);
}
var call2(const var& f, const var& x, const var& y) {
    CallArgs a;
    a.pos.push_back(x);
    a.pos.push_back(y);
    return call_obj(f, a);
}

var var::callmethod_(const char* name, const CallArgs& a) const {
    TypeObj* t = type_of(*this);
    var n = interned(name);
    // the common case: a function or builtin of the type, not shadowed by the instance
    if (!(t->layout == Layout::Type && is_ref() && ref()->type->layout == Layout::Type) && t != T_module && t != T_super) {
        const var* m = t->lookup(n);
        bool shadow = is_ref() && ref()->attrs && static_cast<DictObj*>(ref()->attrs)->get(n);
        if (m && !shadow && m->is_ref()) {
            TypeObj* mt = m->ref()->type;
            if (mt == T_function) return call_function(static_cast<FuncObj*>(m->ref()), this, a);
            if (mt == T_native) {
                CallArgs b;
                b.pos.reserve(a.pos.size() + 1);
                b.pos.push_back(*this);
                b.pos.insert(b.pos.end(), a.pos.begin(), a.pos.end());
                b.kw = a.kw;
                return static_cast<NativeObj*>(m->ref())->fn(b);
            }
        }
    }
    return call_obj(getattr(n), a);
}

// ------------------------------------------------------------------------------------------ natives
var new_native(const char* name, native_fn fn) {
    auto* n = new NativeObj();
    n->type = T_native;
    n->name = name;
    n->fn = std::move(fn);
    return var(n);
}
var native(const char* name, native_fn fn) { return new_native(name, std::move(fn)); }
var bind_native(const var& nat, const var& self) {
    auto* src = static_cast<NativeObj*>(nat.ref());
    auto* n = new NativeObj();
    n->type = T_native;
    n->name = src->name;
    n->fn = src->fn;
    n->self = self;
    return var(n);
}

var arg(const CallArgs& a, size_t i, const char* name, const var& dflt) {
    if (i < a.pos.size()) return a.pos[i];
    if (name)
        for (auto& [k, v] : a.kw)
            if (str_utf8(k) == name) return v;
    if (dflt.is_unbound()) raise(E_TypeError, std::string("missing required argument '") + (name ? name : "?") + "'");
    return dflt;
}
var kwarg(const CallArgs& a, const char* name, const var& dflt) {
    for (auto& [k, v] : a.kw)
        if (str_utf8(k) == name) return v;
    return dflt;
}
void check_args(const CallArgs& a, const char* fname_, size_t min, size_t max) {
    size_t n = a.pos.size();
    if (n < min || n > max) {
        if (min == max)
            raise(E_TypeError, std::string(fname_) + "() takes exactly " + std::to_string(min) + " argument" + (min == 1 ? "" : "s") +
                                   " (" + std::to_string(n) + " given)");
        if (n < min)
            raise(E_TypeError, std::string(fname_) + " expected at least " + std::to_string(min) + " argument" + (min == 1 ? "" : "s") +
                                   ", got " + std::to_string(n));
        raise(E_TypeError, std::string(fname_) + " expected at most " + std::to_string(max) + " argument" + (max == 1 ? "" : "s") +
                               ", got " + std::to_string(n));
    }
}

// ------------------------------------------------------------------------------------------ cells
cell::cell() {
    auto* c = new CellObj();
    c->type = T_cell;
    c_ = var(c);
}
var& cell::operator*() const { return static_cast<CellObj*>(c_.ref())->v; }

// ------------------------------------------------------------------------------------------ classes
var type_new_(TypeObj* meta, const CallArgs& a);

var call_type(TypeObj* t, const CallArgs& a) {
    if (t == T_type) {
        if (a.pos.size() == 1 && a.kw.empty()) return tv(type_of(a.pos[0]));
        return type_new_(T_type, a);
    }
    if (t->abstract_instances) raise(E_TypeError, "cannot create '" + t->name + "' instances");
    const var* nw = t->lookup("__new__");
    var obj;
    var cls = tv(t);
    if (nw) {
        var f = nw->is_ref() && nw->ref()->type == T_staticmethod ? static_cast<WrapObj*>(nw->ref())->f : *nw;
        CallArgs b;
        b.pos.reserve(a.pos.size() + 1);
        b.pos.push_back(cls);
        b.pos.insert(b.pos.end(), a.pos.begin(), a.pos.end());
        b.kw = a.kw;
        obj = call_obj(f, b);
    } else {
        raise(E_TypeError, "cannot create '" + t->name + "' instances");
    }
    if (!isinstance_(obj, t)) return obj;
    const var* in = type_of(obj)->lookup("__init__");
    if (in && in->is_ref()) {
        TypeObj* it = in->ref()->type;
        if (it == T_function) {
            var r = call_function(static_cast<FuncObj*>(in->ref()), &obj, a);
            if (!r.is_none()) raise(E_TypeError, "__init__() should return None, not '" + tname(r) + "'");
        } else if (it == T_native) {
            CallArgs b;
            b.pos.push_back(obj);
            b.pos.insert(b.pos.end(), a.pos.begin(), a.pos.end());
            b.kw = a.kw;
            static_cast<NativeObj*>(in->ref())->fn(b);
        } else {
            call_obj(bind(*in, obj, type_of(obj)), a);
        }
    }
    return obj;
}

}  // namespace py
