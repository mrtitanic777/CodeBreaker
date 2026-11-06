// lib_more.cc -- more library modules: enum (Enum, IntEnum, Flag, auto: members made when the
// class is), dataclasses (dataclass, field, asdict, replace), contextlib (contextmanager, closing,
// suppress, nullcontext, redirect_stdout, ExitStack), threading (threads under the global lock,
// Lock, RLock, Event, local), heapq, operator, importlib, types, typing, abc, uuid, traceback,
// warnings, atexit, binascii, keyword; and the global interpreter lock itself.
#include <windows.h>
#include <bcrypt.h>

#include <algorithm>
#include <condition_variable>
#include <cstring>
#include <mutex>
#include <thread>

#include "rt.hpp"

namespace py {

var coroutine_type_();                    // gen.cc
var async_generator_type_();
var types_coroutine_(const var& f);

extern var g_sys;
var type_new_(TypeObj* meta, const CallArgs& a);
bool module_registered_(const std::string& name);
bool module_source_(const std::string& name, std::string& file, bool& package);
void print_exception_(const var& e);

// ------------------------------------------------------------------------------------------ the GIL
static std::mutex* g_gil = new std::mutex();
static thread_local int t_gil_held = 0;
void gil_acquire_() { g_gil->lock(); t_gil_held = 1; }
void gil_release_() { t_gil_held = 0; g_gil->unlock(); }
static bool g_threads_started = false;
static const DWORD g_main_tid = GetCurrentThreadId();   // (at static initialisation: the main thread)
gil_release::gil_release() { if (g_threads_started && t_gil_held) gil_release_(); }
gil_release::~gil_release() { if (g_threads_started && !t_gil_held) gil_acquire_(); }
bool gil_ensure_() {   // (a ctypes callback on a thread of its own, or while its caller had released the lock)
    // a callback on a thread the program did not start (a DLL's worker): from now on the program has threads, so
    // the global lock is taken and given as CPython's is (the main thread is in the foreign call meanwhile)
    if (!g_threads_started && GetCurrentThreadId() != g_main_tid) g_threads_started = true;
    if (g_threads_started && !t_gil_held) { gil_acquire_(); return true; }
    return false;
}

static var nat(const char* n, native_fn f) { return new_native(n, std::move(f)); }
static var mkcls(const char* name, std::vector<var> bases, std::vector<std::pair<const char*, var>> members, const char* module) {
    var ns = new_dict();
    for (auto& [k, v] : members) ns.setitem(new_str(k), v);
    return make_class(name, name, bases, ns, {}, module);
}

// ------------------------------------------------------------------------------------------ enum
static TypeObj* T_EnumMeta;
static var Enum_, IntEnum_, Flag_, IntFlag_, auto_cls, StrEnum_;
static int64_t g_auto_counter = 0;

static bool is_descriptor_or_dunder(const std::string& k, const var& v) {
    if (k.size() > 4 && k.compare(0, 2, "__") == 0 && k.compare(k.size() - 2, 2, "__") == 0) return true;
    if (!k.empty() && k[0] == '_' && (k.size() < 2 || k[1] != '_') && k.back() == '_') return true;   // _sunder_
    if (!v.is_ref()) return false;
    TypeObj* t = v.ref()->type;
    return t == T_function || t == T_native || t == T_property || t == T_staticmethod || t == T_classmethod || t->layout == Layout::Type ||
           (!t->builtin && (t->lookup("__get__") || t->lookup("__set__")));
}

static var enum_new_class(const CallArgs& a) {
    TypeObj* meta = T(a.pos.at(0));
    CallArgs b;
    b.pos.assign(a.pos.begin() + 1, a.pos.end());
    var ns = b.pos.at(2);
    // the members: the plain values of the body, in order
    std::vector<std::pair<var, var>> members;
    var clean = new_dict();
    for (var k : each(ns)) {
        var v = ns.getitem(k);
        std::string ks = str_utf8(k);
        if (is_descriptor_or_dunder(ks, v) || (ks.size() > 1 && ks[0] == '_' && ks[1] == '_')) { clean.setitem(k, v); continue; }
        if (ks == "_ignore_") continue;
        members.push_back({k, v});
    }
    b.pos[2] = clean;
    var cls = type_new_(meta, b);
    TypeObj* t = T(cls);
    // which builtin the members are: int (IntEnum), str (StrEnum), or plain objects
    Layout lay = t->layout;
    std::vector<var> order;
    var map = new_dict(), v2m = new_dict();
    int64_t last = 0;
    bool is_flag = false;
    for (TypeObj* m : t->mro) if (!Flag_.is_unbound() && m == T(Flag_)) is_flag = true;
    for (auto& [k, v0] : members) {
        var v = v0;
        if (v.is_ref() && !auto_cls.is_unbound() && v.ref()->type == T(auto_cls)) {
            if (const var* gnv = t->lookup("_generate_next_value_"); gnv && gnv->is_ref() && gnv->ref()->type == T_function) {
                std::vector<var> lastv;
                for (auto& o : order) lastv.push_back(o.getattr("_value_"));
                v = call_obj(*gnv, pack_args_(k, var((int64_t)order.size() + 1), var((int64_t)order.size()), new_list(lastv)));
            } else if (is_flag) v = last ? var(last * 2) : var(1);
            else v = var(last + 1);
        }
        if (is_int(v)) last = v.as_int64();
        // an alias: a value already given
        bool alias = false;
        for (auto& o : order)
            if (o.getattr("_value_") == v) { t->dict->set(k, o); map.setitem(k, o); alias = true; break; }
        if (alias) continue;
        var member;
        std::vector<var> init_args = is_tuple(v) && lay == Layout::Object ? tuple_items(v) : std::vector<var>{v};
        // the class's own __new__ (HTTPStatus's: int.__new__(cls, value) and the phrase): it makes the member
        var* own_new = t->dict->get("__new__");
        if (own_new && own_new->is_ref() && (own_new->ref()->type == T_function || own_new->ref()->type == T_staticmethod)) {
            var f = own_new->ref()->type == T_staticmethod ? static_cast<WrapObj*>(own_new->ref())->f : *own_new;
            CallArgs c;
            c.pos.push_back(cls);
            for (auto& x : (is_tuple(v) ? tuple_items(v) : std::vector<var>{v})) c.pos.push_back(x);
            member = call_obj(f, c);
            member.setattr("_name_", k);
            var* own = member.is_ref() && member.ref()->attrs ? static_cast<DictObj*>(member.ref()->attrs)->get("_value_") : nullptr;
            if (!own) member.setattr("_value_", lay == Layout::Int ? new_int(to_big(member)) : v);
            var mv = member.getattr("_value_");
            t->dict->set(k, member);
            map.setitem(k, member);
            try { v2m.setitem(mv, member); } catch (error&) {}
            order.push_back(member);
            if (is_int(mv)) last = mv.as_int64();
            continue;
        }
        if (lay == Layout::Int) {
            auto* o = new BigObj();
            o->type = t;
            o->v = to_big(v);
            member = var(static_cast<Obj*>(o));
        } else if (lay == Layout::Str) {
            var s = str(v);
            auto* o = new StrObj();
            o->type = t;
            o->s = str_utf8(s);
            o->n = len_(s);
            o->ascii = o->n == (int64_t)o->s.size();
            member = var(static_cast<Obj*>(o));
        } else {
            auto* o = new Obj();
            o->type = t;
            member = var(o);
        }
        member.setattr("_name_", k);
        member.setattr("_value_", v);
        if (const var* in = t->lookup("__init__"); in && in->is_ref() && in->ref()->type == T_function) {
            CallArgs c;
            c.pos = init_args;
            call_obj(bind(*in, member, t), c);
        }
        t->dict->set(k, member);
        map.setitem(k, member);
        try { v2m.setitem(v, member); } catch (error&) {}
        order.push_back(member);
    }
    t->dict->set("_member_names_"_s, new_list([&] { std::vector<var> n; for (auto& o : order) n.push_back(o.getattr("_name_")); return n; }()));
    t->dict->set("_member_map_"_s, map);
    t->dict->set("_value2member_map_"_s, v2m);
    t->dict->set("__members_order__"_s, new_list(order));
    return cls;
}

static var enum_lookup(const var& cls, const var& value) {
    TypeObj* t = T(cls);
    var* v2m = t->dict->get("_value2member_map_");
    if (v2m) {
        try {
            if (var* m = static_cast<DictObj*>(v2m->ref())->get(value)) return *m;
        } catch (error&) {}
    }
    if (is(value.is_ref() && isinstance_(value, t) ? value : var(), value) && isinstance_(value, t)) return value;
    var* ord = t->dict->get("__members_order__");
    if (ord) for (var m : each(*ord)) if (m.getattr("_value_") == value) return m;
    if (const var* mi = t->lookup("_missing_"); mi && mi->is_ref() && mi->ref()->type == T_classmethod) {
        var r = call_obj(static_cast<WrapObj*>(mi->ref())->f, pack_args_(cls, value));
        if (!r.is_none()) return r;
    }
    raise(E_ValueError, repr_(value) + " is not a valid " + t->qualname);
}

static void init_enum(const var& m) {
    T_EnumMeta = new_type("EnumType", {T_type}, Layout::Type, true, "enum");
    add_static(T_EnumMeta, "__new__", enum_new_class);
    add_method(T_EnumMeta, "__call__", [](const CallArgs& a) {
        if (a.pos.size() == 2 && a.kw.empty()) return enum_lookup(a.pos[0], a.pos[1]);
        // the functional API: Enum('Name', names, module=, qualname=, start=): names a string, a sequence of
        // names or of (name, value) pairs, or a mapping
        var cls = a.pos.at(0), name = arg(a, 1, "value"), names = arg(a, 2, "names", None);
        var module = kwarg(a, "module", None), start = kwarg(a, "start", var(1));
        var ns = new_dict();
        if (is_str(names)) names = names.call("replace", ","_s, " "_s).call("split");
        if (isinstance_(names, T_dict)) {
            for (var k : each(names)) ns.setitem(k, names.getitem(k));
        } else {
            int64_t i = 0;
            bool is_flag = false;
            for (TypeObj* m2 : T(cls)->mro) if (!Flag_.is_unbound() && m2 == T(Flag_)) is_flag = true;
            for (var it : each(names)) {
                if (is_str(it)) ns.setitem(it, is_flag ? var((int64_t)1 << i) : start + var(i));
                else ns.setitem(it.getitem(var(0)), it.getitem(var(1)));
                i++;
            }
        }
        ns.setitem("__module__"_s, module.is_none() ? var("__main__"_s) : module);
        var q = kwarg(a, "qualname", None);
        if (!q.is_none()) ns.setitem("__qualname__"_s, q);
        CallArgs c;
        c.pos = {tv(type_of(cls)), name, new_tuple({cls}), ns};
        return enum_new_class(c);
    });
    add_method(T_EnumMeta, "__iter__", [](const CallArgs& a) {
        var* ord = T(a.pos[0])->dict->get("__members_order__");
        return iter(ord ? *ord : new_list());
    });
    add_method(T_EnumMeta, "__len__", [](const CallArgs& a) { var* ord = T(a.pos[0])->dict->get("__members_order__"); return var(ord ? len_(*ord) : 0); });
    add_method(T_EnumMeta, "__getitem__", [](const CallArgs& a) {
        var* map = T(a.pos[0])->dict->get("_member_map_");
        if (map) if (var* v = static_cast<DictObj*>(map->ref())->get(a.pos.at(1))) return *v;
        raise(make_exc_args(E_KeyError, {a.pos.at(1)}));
    });
    add_method(T_EnumMeta, "__contains__", [](const CallArgs& a) {
        var* ord = T(a.pos[0])->dict->get("__members_order__");
        if (!ord) return var(false);
        for (var x : each(*ord)) if (is(x, a.pos.at(1)) || x.getattr("_value_") == a.pos[1]) return var(true);
        return var(false);
    });
    add_getter(T_EnumMeta, "__members__", [](const var& c) { var* map = T(c)->dict->get("_member_map_"); return map ? *map : new_dict(); });
    add_method(T_EnumMeta, "__repr__", [](const CallArgs& a) { return new_str("<enum '" + T(a.pos[0])->name + "'>"); });

    var ns = new_dict();
    ns.setitem("__repr__"_s, nat("__repr__", [](const CallArgs& a) {
        var s = a.pos[0];
        return new_str("<" + type_of(s)->name + "." + str_utf8(s.getattr("_name_")) + ": " + repr_(s.getattr("_value_")) + ">");
    }));
    ns.setitem("__str__"_s, nat("__str__", [](const CallArgs& a) { return new_str(type_of(a.pos[0])->name + "." + str_utf8(a.pos[0].getattr("_name_"))); }));
    ns.setitem("__format__"_s, nat("__format__", [](const CallArgs& a) { return format(str(a.pos[0]), a.pos.at(1)); }));
    ns.setitem("__hash__"_s, nat("__hash__", [](const CallArgs& a) { return hash(a.pos[0].getattr("_name_")); }));
    ns.setitem("__reduce_ex__"_s, nat("__reduce_ex__", [](const CallArgs& a) { return new_tuple({tv(type_of(a.pos[0])), new_tuple({a.pos[0].getattr("_value_")})}); }));
    auto* pn = new PropertyObj();
    pn->type = T_property;
    pn->name = "name";
    pn->fget = nat("name", [](const CallArgs& a) { return a.pos[0].getattr("_name_"); });
    ns.setitem("name"_s, var(static_cast<Obj*>(pn)));
    auto* pv = new PropertyObj();
    pv->type = T_property;
    pv->name = "value";
    pv->fget = nat("value", [](const CallArgs& a) { return a.pos[0].getattr("_value_"); });
    ns.setitem("value"_s, var(static_cast<Obj*>(pv)));
    ns.setitem("__module__"_s, "enum"_s);
    // Enum._convert_(name, module, filter, source=None, *, boundary=None, as_global=False): an enum of a module's
    // names that pass filter (socket's AddressFamily ...), sorted by value, its members put back in the module
    ns.setitem("_convert_"_s, classmethod(def("_convert_", "cls, name, module, filter, source=, *, boundary=, as_global=", {None, None, var(false)},
                                              [](var cls, var name, var module, var filter, var source, var boundary, var as_global) -> var {
        (void)boundary;
        var mod = import("sys").getattr("modules").getitem(module);
        var src = source.is_none() ? mod.getattr("__dict__") : source.getattr("__dict__");
        std::vector<std::pair<var, var>> members;
        for (var k : each(src))
            if (call1(filter, k).truthy()) members.push_back({k, src.getitem(k)});
        bool by_value = true;
        for (auto& m2 : members) if (!is_int(m2.second)) by_value = false;
        std::stable_sort(members.begin(), members.end(), [&](const std::pair<var, var>& x, const std::pair<var, var>& y) {
            if (by_value && !(x.second == y.second)) return lt_(x.second, y.second);
            return str_utf8(x.first) < str_utf8(y.first);
        });
        var body = new_dict();
        for (auto& m2 : members) body.setitem(m2.first, m2.second);
        body.setitem("__module__"_s, module);
        CallArgs c2;
        c2.pos = {tv(type_of(cls)), name, new_tuple({cls}), body};
        var e = enum_new_class(c2);
        var mm = e.getattr("__members__");
        for (var k : each(mm)) mod.setattr(str_utf8(k).c_str(), mm.getitem(k));
        (void)as_global;
        mod.setattr(str_utf8(name).c_str(), e);
        return e;
    })));
    CallArgs c;
    c.pos = {tv(T_EnumMeta), "Enum"_s, new_tuple(), ns};
    Enum_ = enum_new_class(c);
    // IntEnum: str() and format() are the int's
    var ins = new_dict();
    ins.setitem("__str__"_s, nat("__str__", [](const CallArgs& a) { return new_str(to_big(a.pos[0]).str()); }));
    ins.setitem("__format__"_s, nat("__format__", [](const CallArgs& a) { return format(new_int(to_big(a.pos[0])), a.pos.at(1)); }));
    ins.setitem("__hash__"_s, nat("__hash__", [](const CallArgs& a) { return var(hash_int(to_big(a.pos[0]))); }));
    ins.setitem("__module__"_s, "enum"_s);
    c.pos = {tv(T_EnumMeta), "IntEnum"_s, new_tuple({t::int_, Enum_}), ins};
    IntEnum_ = enum_new_class(c);
    var fns = new_dict();
    fns.setitem("__module__"_s, "enum"_s);
    fns.setitem("__or__"_s, nat("__or__", [](const CallArgs& a) { return enum_lookup(tv(type_of(a.pos[0])), a.pos[0].getattr("_value_") | a.pos.at(1).getattr("_value_")); }));
    fns.setitem("__and__"_s, nat("__and__", [](const CallArgs& a) { return enum_lookup(tv(type_of(a.pos[0])), a.pos[0].getattr("_value_") & a.pos.at(1).getattr("_value_")); }));
    fns.setitem("__contains__"_s, nat("__contains__", [](const CallArgs& a) {
        var x = a.pos[0].getattr("_value_"), y = a.pos.at(1).getattr("_value_");
        return var((x & y) == y);
    }));
    fns.setitem("__bool__"_s, nat("__bool__", [](const CallArgs& a) { return var(a.pos[0].getattr("_value_").truthy()); }));
    c.pos = {tv(T_EnumMeta), "Flag"_s, new_tuple({Enum_}), fns};
    Flag_ = enum_new_class(c);
    var ifns = new_dict();
    ifns.setitem("__module__"_s, "enum"_s);
    c.pos = {tv(T_EnumMeta), "IntFlag"_s, new_tuple({t::int_, Flag_}), ifns};
    IntFlag_ = enum_new_class(c);
    var sns = new_dict();
    sns.setitem("__module__"_s, "enum"_s);
    sns.setitem("__str__"_s, nat("__str__", [](const CallArgs& a) { return new_str(str_utf8(a.pos[0].getattr("_value_"))); }));
    sns.setitem("__format__"_s, nat("__format__", [](const CallArgs& a) { return format(a.pos[0].getattr("_value_"), a.pos.at(1)); }));
    c.pos = {tv(T_EnumMeta), "StrEnum"_s, new_tuple({t::str, Enum_}), sns};
    StrEnum_ = enum_new_class(c);
    auto_cls = mkcls("auto", {}, {{"__init__", def("__init__", "self", {}, [](var self) { (void)self; return var(None); })}}, "enum");
    mod_set(m, "Enum", Enum_);
    mod_set(m, "IntEnum", IntEnum_);
    // @global_enum: the members become names of the enum's module (calendar's MONDAY ...)
    mod_def(m, "global_enum", [](const CallArgs& a) {
        var cls = a.pos.at(0);
        var mods = import("sys").getattr("modules");
        var mod = mods.call("get", cls.getattr("__module__"));
        if (!mod.is_none())
            for (var k : each(cls.getattr("__members__"))) mod.setattr(str_utf8(k).c_str(), cls.getattr("__members__").getitem(k));
        return cls;
    });
    // @_simple_enum(etype): a plain class's body made an enum of etype (http.HTTPStatus ...)
    mod_def(m, "_simple_enum", [](const CallArgs& a) {
        var etype = a.pos.empty() ? kwarg(a, "etype", Enum_) : a.pos[0];
        return new_native("_simple_enum", [etype](const CallArgs& b) {
            var cls = b.pos.at(0);
            var ns = new_dict();
            var d = cls.getattr("__dict__");
            for (var k : each(d)) {
                std::string ks = str_utf8(k);
                if (ks == "__dict__" || ks == "__weakref__") continue;
                ns.setitem(k, d.getitem(k));
            }
            CallArgs c;
            c.pos = {tv(T_EnumMeta), cls.getattr("__name__"), new_tuple({etype}), ns};
            return enum_new_class(c);
        });
    });
    mod_set(m, "Flag", Flag_);
    mod_set(m, "IntFlag", IntFlag_);
    mod_set(m, "StrEnum", StrEnum_);
    mod_set(m, "auto", auto_cls);
    mod_set(m, "EnumMeta", tv(T_EnumMeta));
    mod_set(m, "EnumType", tv(T_EnumMeta));
    mod_def(m, "unique", [](const CallArgs& a) { return a.pos.at(0); });
}

// ------------------------------------------------------------------------------------------ dataclasses
static var Field_cls, MISSING;
static var dataclass_apply(const var& c, const var& opts) {
    TypeObj* t = T(c);
    bool eq = opts.getitem("eq"_s).truthy(), frozen = opts.getitem("frozen"_s).truthy(), order = opts.getitem("order"_s).truthy();
    bool do_init = opts.getitem("init"_s).truthy(), do_repr = opts.getitem("repr"_s).truthy();
    // the fields: the bases' first, then this class's annotations in order
    std::vector<std::string> names;
    std::vector<var> fields;
    for (size_t k = t->mro.size(); k-- > 1;) {
        var* df = t->mro[k]->dict->get("__dataclass_fields__");
        if (!df) continue;
        for (var n : each(*df)) {
            std::string ns = str_utf8(n);
            auto it = std::find(names.begin(), names.end(), ns);
            if (it == names.end()) { names.push_back(ns); fields.push_back(df->getitem(n)); }
            else fields[(size_t)(it - names.begin())] = df->getitem(n);
        }
    }
    var* ann = t->dict->get("__annotations__");
    if (ann) {
        for (var n : each(*ann)) {
            std::string ns = str_utf8(n);
            var a = ann->getitem(n);
            std::string as = is_str(a) ? str_utf8(a) : repr_(a);
            if (as.find("ClassVar") != std::string::npos) continue;
            var f;
            var* dv = t->dict->get(n);
            if (dv && dv->is_ref() && dv->ref()->type == T(Field_cls)) f = *dv;
            else {
                f = call0(Field_cls);
                f.setattr("default", dv ? *dv : MISSING);
                f.setattr("default_factory", MISSING);
                f.setattr("init", true);
                f.setattr("repr", true);
                f.setattr("compare", true);
                f.setattr("kw_only", false);
            }
            f.setattr("name", n);
            f.setattr("type", a);
            // a field's default stays a class attribute; a field() object does not
            if (dv && dv->is_ref() && dv->ref()->type == T(Field_cls)) {
                var d = f.getattr("default");
                if (is(d, MISSING)) t->dict->del(n);
                else t->dict->set(n, d);
            }
            auto it = std::find(names.begin(), names.end(), ns);
            if (it == names.end()) { names.push_back(ns); fields.push_back(f); }
            else fields[(size_t)(it - names.begin())] = f;
        }
    }
    var fd = new_dict();
    for (size_t i = 0; i < names.size(); i++) fd.setitem(new_str(names[i]), fields[i]);
    t->dict->set("__dataclass_fields__"_s, fd);
    std::string cname = t->qualname;
    if (do_init && !t->dict->get("__init__")) {
        t->dict->set("__init__"_s, nat("__init__", [names, fields, cname, frozen](const CallArgs& a) {
            var self = a.pos.at(0);
            std::vector<size_t> init_idx;
            for (size_t i = 0; i < fields.size(); i++) if (fields[i].getattr("init").truthy()) init_idx.push_back(i);
            size_t np = a.pos.size() - 1;
            if (np > init_idx.size())
                raise(E_TypeError, cname + ".__init__() takes " + std::to_string(init_idx.size() + 1) + " positional arguments but " + std::to_string(np + 1) + " were given");
            std::vector<var> vals(fields.size());
            for (size_t k = 0; k < np; k++) vals[init_idx[k]] = a.pos[k + 1];
            for (auto& [kn, kv] : a.kw) {
                auto it = std::find(names.begin(), names.end(), str_utf8(kn));
                if (it == names.end()) raise(E_TypeError, cname + ".__init__() got an unexpected keyword argument '" + str_utf8(kn) + "'");
                vals[(size_t)(it - names.begin())] = kv;
            }
            for (size_t i = 0; i < fields.size(); i++) {
                if (vals[i].is_unbound()) {
                    var d = fields[i].getattr("default"), df = fields[i].getattr("default_factory");
                    if (!is(df, MISSING)) vals[i] = call0(df);
                    else if (!is(d, MISSING)) vals[i] = d;
                    else if (fields[i].getattr("init").truthy())
                        raise(E_TypeError, cname + ".__init__() missing 1 required positional argument: '" + names[i] + "'");
                    else continue;
                }
                Obj* p = self.ref();
                if (!p->attrs) { var d2 = new_dict_obj(); p->attrs = d2.ref(); incref(p->attrs); }
                static_cast<DictObj*>(p->attrs)->set(new_str(names[i]), vals[i]);
            }
            if (const var* post = type_of(self)->lookup("__post_init__")) call0(bind(*post, self, type_of(self)));
            (void)frozen;
            return None_;
        }));
    }
    if (do_repr && !t->dict->get("__repr__")) {
        t->dict->set("__repr__"_s, nat("__repr__", [names, fields](const CallArgs& a) {
            std::string r = type_of(a.pos[0])->qualname + "(";
            bool first = true;
            for (size_t i = 0; i < names.size(); i++) {
                if (!fields[i].getattr("repr").truthy()) continue;
                r += (first ? "" : ", ") + names[i] + "=" + repr_(a.pos[0].getattr(new_str(names[i])));
                first = false;
            }
            return new_str(r + ")");
        }));
    }
    auto astuple = [names, fields](const var& self) {
        std::vector<var> v;
        for (size_t i = 0; i < names.size(); i++) if (fields[i].getattr("compare").truthy()) v.push_back(self.getattr(new_str(names[i])));
        return new_tuple(v);
    };
    if (eq && !t->dict->get("__eq__")) {
        t->dict->set("__eq__"_s, nat("__eq__", [astuple](const CallArgs& a) {
            if (type_of(a.pos.at(1)) != type_of(a.pos[0])) return NotImplemented;
            return var(astuple(a.pos[0]) == astuple(a.pos[1]));
        }));
        if (!t->dict->get("__hash__")) t->dict->set("__hash__"_s, frozen ? nat("__hash__", [astuple](const CallArgs& a) { return hash(astuple(a.pos[0])); }) : var(None));
    }
    if (order) {
        const char* ops[] = {"__lt__", "__le__", "__gt__", "__ge__"};
        for (int k = 0; k < 4; k++)
            t->dict->set(new_str(ops[k]), nat(ops[k], [astuple, k](const CallArgs& a) {
                if (type_of(a.pos.at(1)) != type_of(a.pos[0])) return NotImplemented;
                var x = astuple(a.pos[0]), y = astuple(a.pos[1]);
                return var(k == 0 ? x < y : k == 1 ? x <= y : k == 2 ? x > y : x >= y);
            }));
    }
    if (frozen) {
        t->dict->set("__setattr__"_s, def("__setattr__", "self, name, value", {}, [](var self, var name, var value) -> var {
            (void)self; (void)value;
            raise(make_exc_args(T(import("dataclasses").getattr("FrozenInstanceError")), {new_str("cannot assign to field '" + str_utf8(name) + "'")}));
        }));
    }
    return c;
}

static void init_dataclasses(const var& m) {
    MISSING = mkcls("_MISSING_TYPE", {}, {{"__repr__", nat("__repr__", [](const CallArgs&) { return "MISSING"_s; })}}, "dataclasses");
    MISSING = call0(MISSING);
    Field_cls = mkcls("Field", {}, {{"__repr__", nat("__repr__", [](const CallArgs& a) { return new_str("Field(name=" + repr_(getattr(a.pos[0], "name"_s, None)) + ")"); })}}, "dataclasses");
    mod_set(m, "MISSING", MISSING);
    mod_set(m, "Field", Field_cls);
    mod_set(m, "FrozenInstanceError", mkcls("FrozenInstanceError", {t::AttributeError}, {}, "dataclasses"));
    mod_def(m, "field", [](const CallArgs& a) {
        var f = call0(Field_cls);
        f.setattr("default", kwarg(a, "default", MISSING));
        f.setattr("default_factory", kwarg(a, "default_factory", MISSING));
        f.setattr("init", kwarg(a, "init", true));
        f.setattr("repr", kwarg(a, "repr", true));
        f.setattr("compare", kwarg(a, "compare", true));
        f.setattr("kw_only", kwarg(a, "kw_only", false));
        f.setattr("metadata", kwarg(a, "metadata", None));
        return f;
    });
    mod_def(m, "dataclass", [](const CallArgs& a) {
        var opts = new_dict();
        for (const char* k : {"init", "repr", "eq"}) opts.setitem(new_str(k), kwarg(a, k, true));
        for (const char* k : {"order", "frozen", "unsafe_hash", "slots", "kw_only"}) opts.setitem(new_str(k), kwarg(a, k, false));
        if (!a.pos.empty()) return dataclass_apply(a.pos[0], opts);
        return nat("wrap", [opts](const CallArgs& b) { return dataclass_apply(b.pos.at(0), opts); });
    });
    mod_def(m, "fields", [](const CallArgs& a) {
        var c = a.pos.at(0);
        var fd = getattr(c, "__dataclass_fields__"_s, None);
        if (fd.is_none()) raise(E_TypeError, "must be called with a dataclass type or instance");
        std::vector<var> v;
        for (var k : each(fd)) v.push_back(fd.getitem(k));
        return new_tuple(v);
    });
    mod_def(m, "is_dataclass", [](const CallArgs& a) { return var(!getattr(a.pos.at(0), "__dataclass_fields__"_s, None).is_none()); });
    mod_def(m, "asdict", [](const CallArgs& a) {
        std::function<var(const var&)> conv = [&](const var& x) -> var {
            var fd = getattr(x, "__dataclass_fields__"_s, None);
            if (!fd.is_none() && !(x.is_ref() && x.ref()->type->layout == Layout::Type)) {
                var d = new_dict();
                for (var k : each(fd)) d.setitem(k, conv(x.getattr(k)));
                return d;
            }
            if (is_list(x)) { std::vector<var> v; for (auto& y : list_items(x)) v.push_back(conv(y)); return new_list(v); }
            if (is_tuple(x)) { std::vector<var> v; for (auto& y : tuple_items(x)) v.push_back(conv(y)); return new_tuple(v); }
            if (is_dict(x)) { var d = new_dict(); for (var k : each(x)) d.setitem(conv(k), conv(x.getitem(k))); return d; }
            return x;
        };
        return conv(a.pos.at(0));
    });
    mod_def(m, "replace", [](const CallArgs& a) {
        var obj = a.pos.at(0);
        var fd = obj.getattr("__dataclass_fields__");
        CallArgs b;
        for (var k : each(fd)) {
            bool given = false;
            for (auto& [kn, kv] : a.kw) if (str_utf8(kn) == str_utf8(k)) { b.kw.push_back({kn, kv}); given = true; }
            if (!given && fd.getitem(k).getattr("init").truthy()) b.kw.push_back({k, obj.getattr(k)});
        }
        return call_obj(tv(type_of(obj)), b);
    });
}

// ------------------------------------------------------------------------------------------ contextlib
static TypeObj* T_gcm;
static void init_contextlib(const var& m) {
    T_gcm = new_type("_GeneratorContextManager", {}, Layout::NativeData, true, "contextlib");
    add_method(T_gcm, "__enter__", [](const CallArgs& a) {
        var gen = a.pos[0].getattr("gen");
        var out;
        if (!next_(gen, out)) raise(E_RuntimeError, "generator didn't yield");
        return out;
    });
    add_method(T_gcm, "__exit__", [](const CallArgs& a) {
        var gen = a.pos[0].getattr("gen");
        var typ = a.pos.at(1), val = a.pos.at(2);
        if (typ.is_none()) {
            var out;
            if (next_(gen, out)) raise(E_RuntimeError, "generator didn't stop");
            return var(false);
        }
        if (val.is_none()) val = call0(typ);
        try {
            gen.call("throw", val);
        } catch (error& e) {
            if (isinstance_(e.value, E_StopIteration)) return var(!is(e.value, val));
            if (is(e.value, val)) return var(false);
            throw;
        }
        raise(E_RuntimeError, "generator didn't stop after throw()");
    });
    add_method(T_gcm, "__call__", [](const CallArgs& a) {
        // used as a decorator: a fresh manager around each call
        var factory = a.pos[0].getattr("factory"), fargs = a.pos[0].getattr("fargs");
        var f = a.pos.at(1);
        return nat("inner", [factory, fargs, f](const CallArgs& c) {
            var cm = call_obj(factory, [&] { CallArgs b; b.pos = collect(fargs); return b; }());
            var v = cm.call("__enter__");
            (void)v;
            try {
                var r = call_obj(f, c);
                cm.call("__exit__", None, None, None);
                return r;
            } catch (error& e) {
                if (!cm.call("__exit__", tv(type_of(e.value)), e.value, None).truthy()) throw;
                return var(None);
            }
        });
    });
    mod_def(m, "contextmanager", [](const CallArgs& a) {
        var f = a.pos.at(0);
        var w = nat("helper", [f](const CallArgs& c) {
            auto* o = new DataObj();
            o->type = T_gcm;
            var cm(static_cast<Obj*>(o));
            cm.setattr("gen", call_obj(f, c));
            cm.setattr("factory", f);
            cm.setattr("fargs", new_tuple(c.pos));
            return cm;
        });
        try { w.setattr("__name__", f.getattr("__name__")); } catch (error&) {}
        return w;
    });
    mod_set(m, "closing", mkcls("closing", {}, {
        {"__init__", def("__init__", "self, thing", {}, [](var self, var thing) { self.setattr("thing", thing); return var(None); })},
        {"__enter__", def("__enter__", "self", {}, [](var self) { return self.getattr("thing"); })},
        {"__exit__", def("__exit__", "self, *exc", {}, [](var self, var exc) { (void)exc; self.getattr("thing").call("close"); return var(false); })}}, "contextlib"));
    mod_set(m, "nullcontext", mkcls("nullcontext", {}, {
        {"__init__", def("__init__", "self, enter_result=", {None}, [](var self, var r) { self.setattr("enter_result", r); return var(None); })},
        {"__enter__", def("__enter__", "self", {}, [](var self) { return self.getattr("enter_result"); })},
        {"__exit__", def("__exit__", "self, *exc", {}, [](var self, var exc) { (void)self; (void)exc; return var(false); })}}, "contextlib"));
    mod_set(m, "suppress", mkcls("suppress", {}, {
        {"__init__", def("__init__", "self, *exceptions", {}, [](var self, var ex) { self.setattr("exceptions", ex); return var(None); })},
        {"__enter__", def("__enter__", "self", {}, [](var self) { (void)self; return var(None); })},
        {"__exit__", def("__exit__", "self, typ, val, tb", {}, [](var self, var typ, var val, var tb) {
             (void)tb;
             if (typ.is_none()) return var(false);
             return var(issubclass(typ, self.getattr("exceptions")).truthy());
         })}}, "contextlib"));
    auto redirect = [](const char* stream) {
        std::string s = stream;
        return mkcls(s == "stdout" ? "redirect_stdout" : "redirect_stderr", {}, {
            {"__init__", def("__init__", "self, new_target", {}, [](var self, var t) { self.setattr("new_target", t); return var(None); })},
            {"__enter__", def("__enter__", "self", {}, [s](var self) {
                 self.setattr("old", g_sys.getattr(s.c_str()));
                 g_sys.setattr(s.c_str(), self.getattr("new_target"));
                 return self.getattr("new_target");
             })},
            {"__exit__", def("__exit__", "self, *exc", {}, [s](var self, var exc) { (void)exc; g_sys.setattr(s.c_str(), self.getattr("old")); return var(false); })}}, "contextlib");
    };
    mod_set(m, "redirect_stdout", redirect("stdout"));
    mod_set(m, "redirect_stderr", redirect("stderr"));
    mod_set(m, "ExitStack", mkcls("ExitStack", {}, {
        {"__init__", def("__init__", "self", {}, [](var self) { self.setattr("_cbs", new_list()); return var(None); })},
        {"__enter__", def("__enter__", "self", {}, [](var self) { return self; })},
        {"enter_context", def("enter_context", "self, cm", {}, [](var self, var cm) {
             var r = cm.call("__enter__");
             self.getattr("_cbs").call("append", cm);
             return r;
         })},
        {"callback", def("callback", "self, fn, *args", {}, [](var self, var fn, var args) {
             self.getattr("_cbs").call("append", new_tuple({fn, args}));
             return fn;
         })},
        {"close", def("close", "self", {}, [](var self) { self.call("__exit__", None, None, None); return var(None); })},
        {"__exit__", def("__exit__", "self, typ, val, tb", {}, [](var self, var typ, var val, var tb) {
             (void)tb;
             var cbs = self.getattr("_cbs");
             bool suppressed = false;
             for (int64_t i = len_(cbs) - 1; i >= 0; i--) {
                 var cb = cbs.getitem(var(i));
                 if (is_tuple(cb)) call_obj(tuple_items(cb)[0], [&] { CallArgs b; b.pos = collect(tuple_items(cb)[1]); return b; }());
                 else if (cb.call("__exit__", typ, val, None).truthy()) suppressed = true;
             }
             cbs.call("clear");
             return var(suppressed);
         })}}, "contextlib"));
}

// ------------------------------------------------------------------------------------------ threading
static TypeObj *T_lock, *T_event;
struct LockObj : DataObj {
    std::recursive_mutex rm;
    std::mutex mu;
    bool reentrant = false;
    int depth = 0;
    DWORD owner = 0;
};
static bool lock_acquire(LockObj* l, bool blocking, double timeout) {
    DWORD me = GetCurrentThreadId();
    if (l->reentrant && l->depth > 0 && l->owner == me) { l->depth++; return true; }
    // try without giving up the global lock first
    if (l->mu.try_lock()) { l->owner = me; l->depth = 1; return true; }
    if (!blocking) return false;
    if (!g_threads_started) {
        if (!l->reentrant) raise(E_RuntimeError, "deadlock: a lock taken twice by its only thread");
    }
    gil_release g;
    auto start = GetTickCount64();
    for (;;) {
        if (l->mu.try_lock()) break;
        if (timeout >= 0 && (double)(GetTickCount64() - start) / 1000.0 >= timeout) return false;
        Sleep(1);
    }
    l->owner = me;
    l->depth = 1;
    return true;
}
static void lock_release(LockObj* l) {
    if (l->depth <= 0) raise(E_RuntimeError, "release unlocked lock");
    if (--l->depth == 0) { l->owner = 0; l->mu.unlock(); }
}
static var new_lock(bool reentrant) {
    auto* l = new LockObj();
    l->type = T_lock;
    l->reentrant = reentrant;
    return var(static_cast<Obj*>(l));
}
struct ThreadState { var target, args, kwargs; std::thread th; bool started = false, done = false; var self; };
static void init_threading(const var& m) {
    T_lock = new_type("lock", {}, Layout::NativeData, true, "_thread");
    add_method(T_lock, "acquire", [](const CallArgs& a) {
        return var(lock_acquire(static_cast<LockObj*>(a.pos[0].ref()), arg(a, 1, "blocking", true).truthy(), arg(a, 2, "timeout", -1).as_double()));
    });
    add_method(T_lock, "release", [](const CallArgs& a) { lock_release(static_cast<LockObj*>(a.pos[0].ref())); return None; });
    add_method(T_lock, "locked", [](const CallArgs& a) { return var(static_cast<LockObj*>(a.pos[0].ref())->depth > 0); });
    add_method(T_lock, "__enter__", [](const CallArgs& a) { lock_acquire(static_cast<LockObj*>(a.pos[0].ref()), true, -1); return var(true); });
    add_method(T_lock, "__exit__", [](const CallArgs& a) { lock_release(static_cast<LockObj*>(a.pos[0].ref())); return var(false); });
    mod_def(m, "Lock", [](const CallArgs&) { return new_lock(false); });
    mod_def(m, "RLock", [](const CallArgs&) { return new_lock(true); });
    mod_def(m, "get_ident", [](const CallArgs&) { return var((int64_t)GetCurrentThreadId()); });
    mod_def(m, "active_count", [](const CallArgs&) { return var(1); });
    // Thread: runs target under the global lock
    var tns = new_dict();
    tns.setitem("__init__"_s, def("__init__", "self, group=, target=, name=, args=, kwargs=, *, daemon=", {None, None, None, new_tuple(), None, None},
                                  [](var self, var g, var target, var name, var args, var kwargs, var daemon) {
                                      (void)g;
                                      self.setattr("_target", target);
                                      self.setattr("_args", args);
                                      self.setattr("_kwargs", kwargs.is_none() ? new_dict() : kwargs);
                                      self.setattr("name", name.is_none() ? "Thread"_s : name);
                                      self.setattr("daemon", daemon.is_none() ? var(false) : daemon);
                                      self.setattr("_handle", None);
                                      return var(None);
                                  }));
    tns.setitem("run"_s, def("run", "self", {}, [](var self) {
        var t = self.getattr("_target");
        if (!t.is_none()) call_obj(t, [&] { CallArgs b; b.pos = collect(self.getattr("_args")); for (var k : each(self.getattr("_kwargs"))) b.kw.push_back({k, self.getattr("_kwargs").getitem(k)}); return b; }());
        return var(None);
    }));
    tns.setitem("start"_s, def("start", "self", {}, [](var self) {
        if (!g_threads_started) { g_threads_started = true; gil_acquire_(); }
        auto* st = new ThreadState();
        st->self = self;
        st->th = std::thread([st]() {
            gil_acquire_();
            try {
                st->self.call("run");
            } catch (error& e) {
                print_exception_(e.value);
            }
            st->done = true;
            gil_release_();
        });
        auto* holder = new DataObj();
        holder->type = T_data;
        var h(static_cast<Obj*>(holder));
        self.setattr("_handle", var((int64_t)(intptr_t)st));
        return var(None);
    }));
    tns.setitem("join"_s, def("join", "self, timeout=", {None}, [](var self, var timeout) {
        var h = self.getattr("_handle");
        if (h.is_none()) raise(E_RuntimeError, "cannot join thread before it is started");
        auto* st = (ThreadState*)(intptr_t)h.as_int64();
        if (st->th.joinable()) {
            if (timeout.is_none()) {
                gil_release g;
                st->th.join();
            } else {
                gil_release g;
                auto start = GetTickCount64();
                while (!st->done && (double)(GetTickCount64() - start) / 1000.0 < timeout.as_double()) Sleep(1);
                if (st->done) st->th.join();
            }
        }
        return var(None);
    }));
    tns.setitem("is_alive"_s, def("is_alive", "self", {}, [](var self) {
        var h = self.getattr("_handle");
        if (h.is_none()) return var(false);
        return var(!((ThreadState*)(intptr_t)h.as_int64())->done);
    }));
    var thread = make_class("Thread", "Thread", {}, tns, {}, "threading");
    mod_set(m, "Thread", thread);
    mod_def(m, "current_thread", [](const CallArgs&) {
        static var main;
        if (main.is_unbound()) {
            main = call0(import("threading").getattr("Thread"));
            main.setattr("name", "MainThread"_s);
        }
        return main;
    });
    mod_def(m, "main_thread", [](const CallArgs&) { return import("threading").call("current_thread"); });
    var ens = new_dict();
    ens.setitem("__init__"_s, def("__init__", "self", {}, [](var self) { self.setattr("_flag", false); return var(None); }));
    ens.setitem("set"_s, def("set", "self", {}, [](var self) { self.setattr("_flag", true); return var(None); }));
    ens.setitem("clear"_s, def("clear", "self", {}, [](var self) { self.setattr("_flag", false); return var(None); }));
    ens.setitem("is_set"_s, def("is_set", "self", {}, [](var self) { return self.getattr("_flag"); }));
    ens.setitem("wait"_s, def("wait", "self, timeout=", {None}, [](var self, var timeout) {
        auto start = GetTickCount64();
        while (!self.getattr("_flag").truthy()) {
            if (!timeout.is_none() && (double)(GetTickCount64() - start) / 1000.0 >= timeout.as_double()) return var(false);
            gil_release g;
            Sleep(1);
        }
        return var(true);
    }));
    mod_set(m, "Event", make_class("Event", "Event", {}, ens, {}, "threading"));
    // Condition: CPython's algorithm (a waiter token per wait, notify sets n of them), waiting as Event does
    var cns = new_dict();
    cns.setitem("__init__"_s, def("__init__", "self, lock=", {None}, [](var self, var lock) {
        self.setattr("_lock", lock.is_none() ? new_lock(true) : lock);
        self.setattr("_waiters", new_list());
        return var(None);
    }));
    cns.setitem("acquire"_s, def("acquire", "self, *args", {}, [](var self, var args) { return call_obj(self.getattr("_lock").getattr("acquire"), CallArgs{collect(args), {}}); }));
    cns.setitem("release"_s, def("release", "self", {}, [](var self) { return self.getattr("_lock").call("release"); }));
    cns.setitem("__enter__"_s, def("__enter__", "self", {}, [](var self) { return self.getattr("_lock").call("acquire"); }));
    cns.setitem("__exit__"_s, def("__exit__", "self, *args", {}, [](var self, var) { self.getattr("_lock").call("release"); return var(None); }));
    cns.setitem("wait"_s, def("wait", "self, timeout=", {None}, [](var self, var timeout) {
        var token = new_list({var(false)});
        self.getattr("_waiters").call("append", token);
        self.getattr("_lock").call("release");
        auto start = GetTickCount64();
        bool got = true;
        while (!token[0].truthy()) {
            if (!timeout.is_none() && (double)(GetTickCount64() - start) / 1000.0 >= timeout.as_double()) { got = false; break; }
            gil_release g;
            Sleep(1);
        }
        self.getattr("_lock").call("acquire");
        if (!got) {
            var w = self.getattr("_waiters");
            for (int64_t i = 0; i < len_(w); i++) if (is(w[var(i)], token)) { w.call("pop", var(i)); break; }
        }
        return var(got);
    }));
    cns.setitem("wait_for"_s, def("wait_for", "self, predicate, timeout=", {None}, [](var self, var pred, var timeout) {
        auto start = GetTickCount64();
        var r = call0(pred);
        while (!r.truthy()) {
            var left = None;
            if (!timeout.is_none()) {
                double l = timeout.as_double() - (double)(GetTickCount64() - start) / 1000.0;
                if (l <= 0) break;
                left = var(l);
            }
            self.call("wait", left);
            r = call0(pred);
        }
        return r;
    }));
    cns.setitem("notify"_s, def("notify", "self, n=", {var(1)}, [](var self, var n) {
        var w = self.getattr("_waiters");
        for (int64_t k = n.as_int64(); k > 0 && len_(w) > 0; k--) w.call("pop", var(0)).setitem(var(0), var(true));
        return var(None);
    }));
    cns.setitem("notify_all"_s, def("notify_all", "self", {}, [](var self) {
        var w = self.getattr("_waiters");
        while (len_(w) > 0) w.call("pop", var(0)).setitem(var(0), var(true));
        return var(None);
    }));
    mod_set(m, "Condition", make_class("Condition", "Condition", {}, cns, {}, "threading"));
    // local: an object whose attributes are per thread (one thread per value here unless threads run)
    mod_set(m, "local", mkcls("local", {}, {}, "threading"));
    var sns = new_dict();
    sns.setitem("__init__"_s, def("__init__", "self, value=", {1}, [](var self, var v) { self.setattr("_lock", new_lock(false)); (void)v; return var(None); }));
    sns.setitem("acquire"_s, def("acquire", "self, blocking=, timeout=", {true, None}, [](var self, var b, var t) { (void)t; return self.getattr("_lock").call("acquire", b); }));
    sns.setitem("release"_s, def("release", "self", {}, [](var self) { return self.getattr("_lock").call("release"); }));
    sns.setitem("__enter__"_s, def("__enter__", "self", {}, [](var self) { return self.getattr("_lock").call("acquire"); }));
    sns.setitem("__exit__"_s, def("__exit__", "self, *a", {}, [](var self, var a) { (void)a; self.getattr("_lock").call("release"); return var(false); }));
    var sem = make_class("Semaphore", "Semaphore", {}, sns, {}, "threading");
    mod_set(m, "Semaphore", sem);
    mod_set(m, "BoundedSemaphore", sem);
}

// ------------------------------------------------------------------------------------------ the rest
static std::vector<var>* g_atexit = new std::vector<var>();
void run_atexit_() {
    for (size_t i = g_atexit->size(); i-- > 0;) {
        try { call0((*g_atexit)[i]); } catch (error& e) { print_exception_(e.value); }
    }
    g_atexit->clear();
}

static var special_form(const char* name) {
    static TypeObj* T_sf = nullptr;
    if (!T_sf) {
        T_sf = new_type("_SpecialForm", {}, Layout::NativeData, true, "typing");
        add_method(T_sf, "__getitem__", [](const CallArgs& a) { return a.pos[0]; });
        add_method(T_sf, "__call__", [](const CallArgs& a) { return a.pos.size() > 1 ? a.pos[1] : var(None); });
        add_method(T_sf, "__or__", [](const CallArgs& a) { return a.pos[0]; });
        add_method(T_sf, "__ror__", [](const CallArgs& a) { return a.pos[0]; });
        add_method(T_sf, "__repr__", [](const CallArgs& a) { return new_str("typing." + str_utf8(a.pos[0].getattr("_name"))); });
        add_method(T_sf, "__mro_entries__", [](const CallArgs&) { return new_tuple({t::object}); });
    }
    auto* o = new DataObj();
    o->type = T_sf;
    var v(static_cast<Obj*>(o));
    v.setattr("_name", new_str(name));
    return v;
}

// concurrent.futures: the executors run each task when it is submitted (in order, under the global lock),
// so results, exceptions and their order are the program's; only the parallelism is not
static void init_futures(const var& m) {
    var fns = new_dict();
    fns.setitem("__init__"_s, def("__init__", "self", {}, [](var self) {
        self.setattr("_done", false);
        self.setattr("_result", None);
        self.setattr("_exc", None);
        self.setattr("_cbs", new_list());
        return var(None);
    }));
    fns.setitem("result"_s, def("result", "self, timeout=", {None}, [](var self, var timeout) {
        (void)timeout;
        var e = self.getattr("_exc");
        if (!e.is_none()) raise(e);
        return self.getattr("_result");
    }));
    fns.setitem("exception"_s, def("exception", "self, timeout=", {None}, [](var self, var timeout) { (void)timeout; return self.getattr("_exc"); }));
    fns.setitem("done"_s, def("done", "self", {}, [](var self) { return self.getattr("_done"); }));
    fns.setitem("running"_s, def("running", "self", {}, [](var self) { (void)self; return var(false); }));
    fns.setitem("cancelled"_s, def("cancelled", "self", {}, [](var self) { (void)self; return var(false); }));
    fns.setitem("cancel"_s, def("cancel", "self", {}, [](var self) { (void)self; return var(false); }));
    fns.setitem("add_done_callback"_s, def("add_done_callback", "self, fn", {}, [](var self, var fn) { call1(fn, self); return var(None); }));
    var Future = make_class("Future", "Future", {}, fns, {}, "concurrent.futures");
    mod_set(m, "Future", Future);
    var ens = new_dict();
    ens.setitem("__init__"_s, def("__init__", "self, max_workers=, *args, **kwargs", {None}, [](var self, var mw, var a, var k) {
        (void)a; (void)k;
        self.setattr("_max_workers", mw);
        return var(None);
    }));
    ens.setitem("submit"_s, def("submit", "self, fn, *args, **kwargs", {}, [Future](var self, var fn, var args, var kwargs) {
        (void)self;
        var f = call0(Future);
        CallArgs c;
        c.pos = collect(args);
        for (var k : each(kwargs)) c.kw.push_back({k, kwargs.getitem(k)});
        try {
            f.setattr("_result", call_obj(fn, c));
        } catch (error& e) {
            if (isinstance_(e.value, E_SystemExit) || isinstance_(e.value, E_KeyboardInterrupt)) throw;
            f.setattr("_exc", e.value);
        }
        f.setattr("_done", true);
        return f;
    }));
    ens.setitem("map"_s, def("map", "self, fn, *iterables, timeout=, chunksize=", {None, 1}, [](var self, var fn, var its, var timeout, var cs) {
        (void)self; (void)timeout; (void)cs;
        CallArgs c;
        c.pos.push_back(fn);
        for (auto& x : collect(its)) c.pos.push_back(x);
        // (submitted at once, as an executor's map does; results in order)
        return iter(new_list(collect(call_obj(builtin("map"), c))));
    }));
    ens.setitem("shutdown"_s, def("shutdown", "self, wait=, *, cancel_futures=", {true, false}, [](var self, var w, var c) { (void)self; (void)w; (void)c; return var(None); }));
    ens.setitem("__enter__"_s, def("__enter__", "self", {}, [](var self) { return self; }));
    ens.setitem("__exit__"_s, def("__exit__", "self, *exc", {}, [](var self, var exc) { (void)self; (void)exc; return var(false); }));
    var Exec = make_class("Executor", "Executor", {}, ens, {}, "concurrent.futures");
    mod_set(m, "Executor", Exec);
    mod_set(m, "ThreadPoolExecutor", make_class("ThreadPoolExecutor", "ThreadPoolExecutor", {Exec}, new_dict(), {}, "concurrent.futures"));
    mod_set(m, "ProcessPoolExecutor", make_class("ProcessPoolExecutor", "ProcessPoolExecutor", {Exec}, new_dict(), {}, "concurrent.futures"));
    mod_def(m, "as_completed", [](const CallArgs& a) { return iter(new_list(collect(a.pos.at(0)))); });
    mod_def(m, "wait", [](const CallArgs& a) {
        var done = new_set();
        for (var f : each(a.pos.at(0))) done.call("add", f);
        return new_tuple({done, new_set()});
    });
    mod_set(m, "FIRST_COMPLETED", "FIRST_COMPLETED"_s);
    mod_set(m, "FIRST_EXCEPTION", "FIRST_EXCEPTION"_s);
    mod_set(m, "ALL_COMPLETED", "ALL_COMPLETED"_s);
    mod_set(m, "TimeoutError", t::TimeoutError);
    mod_set(m, "CancelledError", make_class("CancelledError", "CancelledError", {t::Exception}, new_dict(), {}, "concurrent.futures"));
    var bp = make_class("BrokenExecutor", "BrokenExecutor", {t::RuntimeError}, new_dict(), {}, "concurrent.futures");
    mod_set(m, "BrokenExecutor", bp);
}

// contextvars: a ContextVar's value is the current context's; Context.run(fn) runs fn with a copy, so
// what fn sets does not outlive it
static std::vector<var>* g_ctxvars = new std::vector<var>();
static void init_contextvars(const var& m) {
    var missing = new_tuple({"<missing>"_s});
    var cns = new_dict();
    cns.setitem("__init__"_s, def("__init__", "self, name, *, default=", {var()}, [](var self, var name, var dflt) {
        self.setattr("name", name);
        self.setattr("_default", dflt.is_unbound() ? var(None) : dflt);
        self.setattr("_has_default", var(!dflt.is_unbound()));
        self.setattr("_value", None);
        self.setattr("_set", false);
        g_ctxvars->push_back(self);
        return var(None);
    }));
    cns.setitem("get"_s, def("get", "self, *default", {}, [](var self, var dflt) {
        if (self.getattr("_set").truthy()) return self.getattr("_value");
        if (len_(dflt)) return tuple_items(dflt)[0];
        if (self.getattr("_has_default").truthy()) return self.getattr("_default");
        raise(make_exc_args(E_LookupError, {self}));
    }));
    cns.setitem("set"_s, def("set", "self, value", {}, [](var self, var value) {
        var token = new_tuple({self, self.getattr("_value"), self.getattr("_set")});
        self.setattr("_value", value);
        self.setattr("_set", true);
        return token;
    }));
    cns.setitem("reset"_s, def("reset", "self, token", {}, [](var self, var token) {
        self.setattr("_value", tuple_items(token)[1]);
        self.setattr("_set", tuple_items(token)[2]);
        return var(None);
    }));
    mod_set(m, "ContextVar", make_class("ContextVar", "ContextVar", {}, cns, {}, "contextvars"));
    var xns = new_dict();
    xns.setitem("run"_s, def("run", "self, fn, *args, **kwargs", {}, [](var self, var fn, var args, var kwargs) {
        (void)self;
        std::vector<std::pair<var, var>> saved;
        for (auto& v : *g_ctxvars) saved.push_back({v.getattr("_value"), v.getattr("_set")});
        auto restore = [&] {
            for (size_t i = 0; i < saved.size(); i++) {
                (*g_ctxvars)[i].setattr("_value", saved[i].first);
                (*g_ctxvars)[i].setattr("_set", saved[i].second);
            }
        };
        CallArgs c;
        c.pos = collect(args);
        for (var k : each(kwargs)) c.kw.push_back({k, kwargs.getitem(k)});
        try {
            var r = call_obj(fn, c);
            restore();
            return r;
        } catch (...) {
            restore();
            throw;
        }
    }));
    xns.setitem("copy"_s, def("copy", "self", {}, [](var self) { return call0(tv(type_of(self))); }));
    var Context = make_class("Context", "Context", {}, xns, {}, "contextvars");
    mod_set(m, "Context", Context);
    mod_def(m, "copy_context", [Context](const CallArgs&) { return call0(Context); });
    (void)missing;
}

void init_db_modules();
void init_ast_module(const var& m);
void init_inspect_module(const var& m);
// ------------------------------------------------------------------------------------------ abc
static TypeObj* g_abcmeta = nullptr;
// is sub a subclass of the ABC t: really, through t's registry (and its subclasses'), or by t's hook
static bool abc_subclass(TypeObj* t, TypeObj* sub) {
    if (is_subtype(sub, t)) return true;
    if (var* reg = t->dict->get("_abc_registry"))
        for (var r : each(*reg)) {
            TypeObj* rt = T(r);
            if (rt->metatype == g_abcmeta ? abc_subclass(rt, sub) : is_subtype(sub, rt)) return true;
        }
    if (var* hook = t->dict->get("_abc_hook")) {
        bool all = true;
        for (var n : each(*hook)) {
            const var* v = sub->lookup(n);
            if (!v || v->is_none()) { all = false; break; }
        }
        if (all) return true;
    }
    // the ABC's own subclasses' registries (Sequence's include MutableSequence's)
    return false;
}
// collections.abc: ABCs over the builtins (registered) and the protocols (hooks), as CPython's
void make_collections_abc(const var& m) {
    import("abc");
    auto mk = [&](const char* name, std::vector<var> bases, std::vector<const char*> hook, std::vector<var> builtins) {
        var ns = new_dict();
        ns.setitem("__module__"_s, "collections.abc"_s);
        if (!hook.empty()) {
            std::vector<var> h;
            for (auto* x : hook) h.push_back(new_str(x));
            ns.setitem("_abc_hook"_s, new_tuple(h));
        }
        CallArgs c;
        c.pos = {new_str(name), new_tuple(bases), ns};
        var cls = type_new_(g_abcmeta, c);
        add_classmethod(T(cls), "__class_getitem__", [](const CallArgs& a) { return a.pos[0]; });   // (Iterable[str]: the class)
        for (auto& b : builtins) cls.call("register", b);
        mod_set(m, name, cls);
        return cls;
    };
    var hashable = mk("Hashable", {}, {"__hash__"}, {});
    var iterable = mk("Iterable", {}, {"__iter__"}, {});
    var iterator = mk("Iterator", {iterable}, {"__iter__", "__next__"}, {});
    add_method(T(iterator), "__iter__", [](const CallArgs& a) { return a.pos.at(0); });   // (Iterator's mixin: iter(it) is it)
    mk("Reversible", {iterable}, {"__reversed__", "__iter__"}, {});
    mk("Generator", {iterator}, {"__iter__", "__next__", "send", "throw", "close"}, {t::generator});
    {
        var awaitable = mk("Awaitable", {}, {"__await__"}, {});
        mk("Coroutine", {awaitable}, {"__await__", "send", "throw", "close"}, {coroutine_type_()});
        var aiterable = mk("AsyncIterable", {}, {"__aiter__"}, {});
        var aiterator = mk("AsyncIterator", {aiterable}, {"__aiter__", "__anext__"}, {});
        mk("AsyncGenerator", {aiterator}, {"__aiter__", "__anext__", "asend", "athrow", "aclose"}, {async_generator_type_()});
    }
    var sized = mk("Sized", {}, {"__len__"}, {});
    var container = mk("Container", {}, {"__contains__"}, {});
    mk("Callable", {}, {"__call__"}, {});
    var collection = mk("Collection", {sized, iterable, container}, {"__len__", "__iter__", "__contains__"}, {});
    var set_ = mk("Set", {collection}, {}, {t::frozenset, t::set});
    var mset = mk("MutableSet", {set_}, {}, {t::set});
    var mapping = mk("Mapping", {collection}, {}, {t::dict});
    var mmap = mk("MutableMapping", {mapping}, {}, {t::dict});
    var seq = mk("Sequence", {collection}, {}, {t::tuple, t::str, t::range, t::list, t::bytes, t::bytearray});
    var mseq = mk("MutableSequence", {seq}, {}, {t::list, t::bytearray});
    // the mixin methods (CPython's _collections_abc), over the abstract ones a subclass defines
    auto key_error = [](const var& e) { return isinstance_(e, E_KeyError); };
    TypeObj* M = T(mapping);
    add_method(M, "get", [key_error](const CallArgs& a) -> var {
        try { return a.pos[0].getitem(a.pos.at(1)); } catch (error& e) { if (!key_error(e.value)) throw; }
        return a.pos.size() > 2 ? a.pos[2] : var(None);
    });
    add_method(M, "__contains__", [key_error](const CallArgs& a) -> var {
        try { a.pos[0].getitem(a.pos.at(1)); return var(true); } catch (error& e) { if (!key_error(e.value)) throw; }
        return var(false);
    });
    add_method(M, "keys", [](const CallArgs& a) { return new_list(collect(a.pos[0])); });
    add_method(M, "items", [](const CallArgs& a) {
        std::vector<var> o;
        for (var k : each(a.pos[0])) o.push_back(new_tuple({k, a.pos[0].getitem(k)}));
        return new_list(o);
    });
    add_method(M, "values", [](const CallArgs& a) {
        std::vector<var> o;
        for (var k : each(a.pos[0])) o.push_back(a.pos[0].getitem(k));
        return new_list(o);
    });
    add_method(M, "__eq__", [M](const CallArgs& a) -> var {
        if (!isinstance_(a.pos.at(1), M) && !is_dict(a.pos[1])) return NotImplemented;
        return var(call1(t::dict, a.pos[0].call("items")) == call1(t::dict, a.pos[1].call("items")));
    });
    TypeObj* MM = T(mmap);
    add_method(MM, "pop", [key_error](const CallArgs& a) -> var {
        try {
            var v = a.pos[0].getitem(a.pos.at(1));
            a.pos[0].delitem(a.pos[1]);
            return v;
        } catch (error& e) {
            if (!key_error(e.value) || a.pos.size() < 3) throw;
        }
        return a.pos[2];
    });
    add_method(MM, "popitem", [](const CallArgs& a) -> var {
        var it = iter(a.pos[0]), k;
        if (!next_(it, k)) raise(E_KeyError, "popitem(): dictionary is empty");
        var v = a.pos[0].getitem(k);
        a.pos[0].delitem(k);
        return new_tuple({k, v});
    });
    add_method(MM, "clear", [](const CallArgs& a) {
        for (var k : collect(a.pos[0])) a.pos[0].delitem(k);
        return None;
    });
    add_method(MM, "update", [](const CallArgs& a) {
        var self = a.pos[0];
        if (a.pos.size() > 1) {
            var o = a.pos[1];
            if (hasattr(o, "keys"_s).truthy()) for (var k : each(o.call("keys"))) self.setitem(k, o.getitem(k));
            else for (var kv : each(o)) { auto p = collect(kv); self.setitem(p.at(0), p.at(1)); }
        }
        for (auto& [k, v] : a.kw) self.setitem(k, v);
        return None;
    });
    add_method(MM, "setdefault", [key_error](const CallArgs& a) -> var {
        try { return a.pos[0].getitem(a.pos.at(1)); } catch (error& e) { if (!key_error(e.value)) throw; }
        var d = a.pos.size() > 2 ? a.pos[2] : var(None);
        a.pos[0].setitem(a.pos[1], d);
        return d;
    });
    TypeObj* S = T(seq);
    add_method(S, "__iter__", [](const CallArgs& a) {
        var self = a.pos[0];
        auto i = std::make_shared<int64_t>(0);
        return new_iter([self, i](var& out) -> bool {
            try { out = self.getitem(var((*i)++)); return true; } catch (error& e) { if (isinstance_(e.value, E_IndexError)) return false; throw; }
        });
    });
    add_method(S, "__contains__", [](const CallArgs& a) {
        for (var x : each(a.pos[0])) if (is(x, a.pos.at(1)) || x == a.pos[1]) return var(true);
        return var(false);
    });
    add_method(S, "__reversed__", [](const CallArgs& a) {
        auto v = collect(a.pos[0]);
        std::reverse(v.begin(), v.end());
        return iter(new_list(v));
    });
    add_method(S, "index", [](const CallArgs& a) -> var {
        int64_t i = 0;
        for (var x : each(a.pos[0])) { if (is(x, a.pos.at(1)) || x == a.pos[1]) return var(i); i++; }
        raise(E_ValueError, "value not in sequence");
    });
    add_method(S, "count", [](const CallArgs& a) {
        int64_t n = 0;
        for (var x : each(a.pos[0])) if (is(x, a.pos.at(1)) || x == a.pos[1]) n++;
        return var(n);
    });
    TypeObj* MS = T(mseq);
    add_method(MS, "append", [](const CallArgs& a) { a.pos[0].call("insert", var(len_(a.pos[0])), a.pos.at(1)); return None; });
    add_method(MS, "extend", [](const CallArgs& a) {
        var self = a.pos[0], vals = a.pos.at(1);
        if (is(vals, self)) vals = new_list(collect(vals));
        for (var v : each(vals)) self.call("append", v);
        return None;
    });
    add_method(MS, "clear", [](const CallArgs& a) {
        for (int64_t n = len_(a.pos[0]); n > 0; n--) a.pos[0].call("pop");
        return None;
    });
    add_method(MS, "reverse", [](const CallArgs& a) {
        var self = a.pos[0];
        int64_t n = len_(self);
        for (int64_t i = 0; i < n / 2; i++) {
            var x = self.getitem(var(i)), y = self.getitem(var(n - i - 1));
            self.setitem(var(i), y);
            self.setitem(var(n - i - 1), x);
        }
        return None;
    });
    add_method(MS, "pop", [](const CallArgs& a) {
        var idx = a.pos.size() > 1 ? a.pos[1] : var(-1);
        var v = a.pos[0].getitem(idx);
        a.pos[0].delitem(idx);
        return v;
    });
    add_method(MS, "remove", [](const CallArgs& a) { a.pos[0].delitem(a.pos[0].call("index", a.pos.at(1))); return None; });
    add_method(MS, "__iadd__", [](const CallArgs& a) { a.pos[0].call("extend", a.pos.at(1)); return a.pos[0]; });
    TypeObj* ST = T(set_);
    add_method(ST, "isdisjoint", [](const CallArgs& a) {
        for (var x : each(a.pos.at(1))) if (in(x, a.pos[0])) return var(false);
        return var(true);
    });
    add_method(ST, "__le__", [](const CallArgs& a) {
        if (len_(a.pos[0]) > len_(a.pos.at(1))) return var(false);
        for (var x : each(a.pos[0])) if (!in(x, a.pos[1])) return var(false);
        return var(true);
    });
    add_method(ST, "__eq__", [](const CallArgs& a) -> var {
        if (len_(a.pos[0]) != len_(a.pos.at(1))) return var(false);
        for (var x : each(a.pos[0])) if (!in(x, a.pos[1])) return var(false);
        return var(true);
    });
    TypeObj* MST = T(mset);
    add_method(MST, "remove", [](const CallArgs& a) {
        if (!in(a.pos.at(1), a.pos[0])) raise(make_exc_args(E_KeyError, {a.pos[1]}));
        a.pos[0].call("discard", a.pos[1]);
        return None;
    });
    add_method(MST, "clear", [](const CallArgs& a) {
        for (var x : collect(a.pos[0])) a.pos[0].call("discard", x);
        return None;
    });
    add_method(MST, "__ior__", [](const CallArgs& a) { for (var x : each(a.pos.at(1))) a.pos[0].call("add", x); return a.pos[0]; });
    mk("ByteString", {seq}, {}, {t::bytes, t::bytearray});
    var view = mk("MappingView", {sized}, {}, {});
    mk("KeysView", {view, set_}, {}, {});
    mk("ItemsView", {view, set_}, {}, {});
    mk("ValuesView", {view, collection}, {}, {});
    mk("Awaitable", {}, {"__await__"}, {});
    mk("Coroutine", {}, {"__await__", "send", "throw", "close"}, {});
    mk("AsyncIterable", {}, {"__aiter__"}, {});
    mk("AsyncIterator", {}, {"__aiter__", "__anext__"}, {});
    mk("Buffer", {}, {"__buffer__"}, {t::bytes, t::bytearray});
}

void init_extra_modules();
void init_more_modules() {
    init_db_modules();
    register_native_module("ast", init_ast_module);
    register_native_module("inspect", init_inspect_module);
    register_native_module("contextvars", init_contextvars);
    register_native_module("concurrent", [](const var& m) { (void)m; });
    register_native_module("concurrent.futures", init_futures);
    register_native_module("concurrent.futures.process", [](const var& m) {
        mod_set(m, "BrokenProcessPool", import("concurrent.futures").getattr("BrokenExecutor"));
    });
    register_native_module("enum", init_enum);
    register_native_module("dataclasses", init_dataclasses);
    register_native_module("contextlib", init_contextlib);
    register_native_module("threading", init_threading);
    register_native_module("_thread", [](const var& m) {
        var th = import("threading");
        mod_set(m, "allocate_lock", th.getattr("Lock"));
        mod_set(m, "get_ident", th.getattr("get_ident"));
    });
    register_native_module("heapq", [](const var& m) {
        auto siftdown = [](std::vector<var>& h, size_t start, size_t pos) {
            var item = h[pos];
            while (pos > start) {
                size_t parent = (pos - 1) >> 1;
                if (item < h[parent]) { h[pos] = h[parent]; pos = parent; continue; }
                break;
            }
            h[pos] = item;
        };
        auto siftup = [siftdown](std::vector<var>& h, size_t pos) {
            size_t end = h.size(), start = pos;
            var item = h[pos];
            size_t child = 2 * pos + 1;
            while (child < end) {
                size_t right = child + 1;
                if (right < end && !(h[child] < h[right])) child = right;
                h[pos] = h[child];
                pos = child;
                child = 2 * pos + 1;
            }
            h[pos] = item;
            siftdown(h, start, pos);
        };
        mod_def(m, "heappush", [siftdown](const CallArgs& a) { auto& h = list_items(a.pos.at(0)); h.push_back(a.pos.at(1)); siftdown(h, 0, h.size() - 1); return None; });
        mod_def(m, "heappop", [siftup](const CallArgs& a) {
            auto& h = list_items(a.pos.at(0));
            if (h.empty()) raise(E_IndexError, "index out of range");
            var last = h.back();
            h.pop_back();
            if (h.empty()) return last;
            var r = h[0];
            h[0] = last;
            siftup(h, 0);
            return r;
        });
        mod_def(m, "heapify", [siftup](const CallArgs& a) {
            auto& h = list_items(a.pos.at(0));
            for (size_t i = h.size() / 2; i-- > 0;) siftup(h, i);
            return None;
        });
        mod_def(m, "heappushpop", [siftup](const CallArgs& a) {
            auto& h = list_items(a.pos.at(0));
            var item = a.pos.at(1);
            if (!h.empty() && h[0] < item) { std::swap(item, h[0]); siftup(h, 0); }
            return item;
        });
        mod_def(m, "heapreplace", [siftup](const CallArgs& a) {
            auto& h = list_items(a.pos.at(0));
            if (h.empty()) raise(E_IndexError, "index out of range");
            var r = h[0];
            h[0] = a.pos.at(1);
            siftup(h, 0);
            return r;
        });
        auto nbest = [](const CallArgs& a, bool largest) {
            int64_t n = a.pos.at(0).as_int64();
            CallArgs b;
            b.pos = {a.pos.at(1)};
            var key = kwarg(a, "key", None);
            if (!key.is_none()) b.kw.push_back({"key"_s, key});
            if (largest) b.kw.push_back({"reverse"_s, true});
            var s = call_obj(builtin("sorted"), b);
            auto& v = list_items(s);
            if ((int64_t)v.size() > n) v.resize((size_t)std::max<int64_t>(0, n));
            return s;
        };
        mod_def(m, "nlargest", [nbest](const CallArgs& a) { return nbest(a, true); });
        mod_def(m, "nsmallest", [nbest](const CallArgs& a) { return nbest(a, false); });
    });
    register_native_module("operator", [](const var& m) {
        mod_def(m, "itemgetter", [](const CallArgs& a) {
            std::vector<var> items = a.pos;
            return nat("itemgetter", [items](const CallArgs& c) {
                if (items.size() == 1) return c.pos.at(0).getitem(items[0]);
                std::vector<var> v;
                for (auto& k : items) v.push_back(c.pos.at(0).getitem(k));
                return new_tuple(v);
            });
        });
        mod_def(m, "attrgetter", [](const CallArgs& a) {
            std::vector<var> names = a.pos;
            return nat("attrgetter", [names](const CallArgs& c) {
                auto get = [](var o, const std::string& dotted) {
                    size_t st = 0;
                    for (size_t i = 0; i <= dotted.size(); i++)
                        if (i == dotted.size() || dotted[i] == '.') { o = o.getattr(new_str(dotted.substr(st, i - st))); st = i + 1; }
                    return o;
                };
                if (names.size() == 1) return get(c.pos.at(0), str_utf8(names[0]));
                std::vector<var> v;
                for (auto& n : names) v.push_back(get(c.pos.at(0), str_utf8(n)));
                return new_tuple(v);
            });
        });
        mod_def(m, "methodcaller", [](const CallArgs& a) {
            std::string name = str_utf8(a.pos.at(0));
            CallArgs rest;
            rest.pos.assign(a.pos.begin() + 1, a.pos.end());
            rest.kw = a.kw;
            return nat("methodcaller", [name, rest](const CallArgs& c) { return call_obj(c.pos.at(0).getattr(new_str(name)), rest); });
        });
        auto bin = [m](const char* n, var (*f)(const var&, const var&)) { mod_def(m, n, [f](const CallArgs& a) { return f(a.pos.at(0), a.pos.at(1)); }); };
        bin("add", [](const var& x, const var& y) { return x + y; });
        bin("sub", [](const var& x, const var& y) { return x - y; });
        bin("mul", [](const var& x, const var& y) { return x * y; });
        bin("truediv", [](const var& x, const var& y) { return x / y; });
        bin("floordiv", [](const var& x, const var& y) { return floordiv(x, y); });
        bin("mod", [](const var& x, const var& y) { return x % y; });
        bin("or_", [](const var& x, const var& y) { return x | y; });
        bin("and_", [](const var& x, const var& y) { return x & y; });
        bin("xor", [](const var& x, const var& y) { return x ^ y; });
        bin("lshift", [](const var& x, const var& y) { return x << y; });
        bin("rshift", [](const var& x, const var& y) { return x >> y; });
        bin("eq", [](const var& x, const var& y) { return eq(x, y); });
        // (each comparison's own value, as an __lt__ ... returns it: z3's expressions, numpy's arrays)
        bin("ne", [](const var& x, const var& y) { return compare_value_(x, y, 3); });
        bin("lt", [](const var& x, const var& y) { return compare_value_(x, y, 0); });
        bin("le", [](const var& x, const var& y) { return compare_value_(x, y, 1); });
        bin("gt", [](const var& x, const var& y) { return compare_value_(x, y, 4); });
        bin("ge", [](const var& x, const var& y) { return compare_value_(x, y, 5); });
        bin("getitem", [](const var& x, const var& y) { return x.getitem(y); });
        bin("contains", [](const var& x, const var& y) { return var(in(y, x)); });
        bin("is_", [](const var& x, const var& y) { return var(is(x, y)); });
        bin("pow", [](const var& x, const var& y) { return pow(x, y); });
        mod_def(m, "neg", [](const CallArgs& a) { return -a.pos.at(0); });
        mod_def(m, "not_", [](const CallArgs& a) { return var(!a.pos.at(0)); });
        mod_def(m, "truth", [](const CallArgs& a) { return var(a.pos.at(0).truthy()); });
        mod_def(m, "index", [](const CallArgs& a) { return var(index_(a.pos.at(0))); });
        mod_def(m, "invert", [](const CallArgs& a) { return ~a.pos.at(0); });
    });
    register_native_module("importlib", [](const var& m) {
        mod_def(m, "import_module", [](const CallArgs& a) {
            std::string n = str_utf8(a.pos.at(0));
            if (!n.empty() && n[0] == '.') {
                std::string pkg = str_utf8(arg(a, 1, "package"));
                size_t dots = 0;
                while (dots < n.size() && n[dots] == '.') dots++;
                for (size_t k = 1; k < dots; k++) pkg = pkg.substr(0, pkg.rfind('.'));
                n = pkg + (n.size() > dots ? "." + n.substr(dots) : "");
            }
            return import(n.c_str());
        });
        mod_def(m, "reload", [](const CallArgs& a) { return a.pos.at(0); });
        mod_def(m, "invalidate_caches", [](const CallArgs&) { return None; });
        var util = import("importlib.util");
        mod_set(m, "util", util);
    });
    register_native_module("importlib.util", [](const var& m) {
        mod_def(m, "find_spec", [](const CallArgs& a) {
            // a ModuleSpec of the program's modules: name, origin (the source file it was translated from, or
            // "built-in"), submodule_search_locations (a package's folder)
            std::string n = str_utf8(a.pos.at(0));
            if (!module_registered_(n)) return var(None);
            var spec = call0(import("types").getattr("SimpleNamespace"));
            spec.setattr("name", a.pos[0]);
            std::string file;
            bool pkg = false;
            if (module_source_(n, file, pkg)) {
                spec.setattr("origin", new_str(file));
                spec.setattr("has_location", true);
                std::string dir = file.substr(0, file.find_last_of("/\\"));
                spec.setattr("submodule_search_locations", pkg ? new_list({new_str(dir)}) : var(None));
            } else {
                spec.setattr("origin", "built-in"_s);
                spec.setattr("has_location", false);
                spec.setattr("submodule_search_locations", None);
            }
            spec.setattr("parent", new_str(pkg ? n : n.substr(0, n.rfind('.') == std::string::npos ? 0 : n.rfind('.'))));
            spec.setattr("loader", None);
            spec.setattr("cached", None);
            return spec;
        });
    });
    register_native_module("importlib.resources", [](const var& m) {
        // files(package): the folder of the package's source (the DLLs a package ships sit beside it)
        mod_def(m, "files", [](const CallArgs& a) {
            var p = a.pos.at(0);
            std::string n = is_str(p) ? str_utf8(p) : str_utf8(p.getattr("__name__"));
            std::string file;
            bool pkg = false;
            if (!module_source_(n, file, pkg)) raise(E_ModuleNotFoundError, "No module named '" + n + "'");
            return call1(import("pathlib").getattr("Path"), new_str(file.substr(0, file.find_last_of("/\\"))));
        });
        mod_def(m, "as_file", [](const CallArgs& a) { return call1(import("contextlib").getattr("nullcontext"), a.pos.at(0)); });
    });
    register_native_module("sysconfig", [](const var& m) {
        mod_def(m, "get_path", [](const CallArgs&) { return ""_s; });
        mod_def(m, "get_platform", [](const CallArgs&) { return "win-amd64"_s; });
        mod_def(m, "get_config_var", [](const CallArgs&) { return None; });
        mod_def(m, "get_config_vars", [](const CallArgs&) { return new_dict(); });
        mod_def(m, "get_python_version", [](const CallArgs&) { return "3.14"_s; });
    });
    register_native_module("types", [](const var& m) {
        var sns = new_dict();
        sns.setitem("__init__"_s, nat("__init__", [](const CallArgs& a) {
            for (auto& [k, v] : a.kw) a.pos[0].setattr(str_utf8(k).c_str(), v);
            if (a.pos.size() > 1) for (var k : each(a.pos[1])) a.pos[0].setattr(str_utf8(k).c_str(), a.pos[1].getitem(k));
            return None;
        }));
        sns.setitem("__repr__"_s, nat("__repr__", [](const CallArgs& a) {
            std::string r = "namespace(";
            int i = 0;
            var d = a.pos[0].getattr("__dict__");
            for (var k : each(d)) r += (i++ ? ", " : "") + str_utf8(k) + "=" + repr_(d.getitem(k));
            return new_str(r + ")");
        }));
        sns.setitem("__eq__"_s, nat("__eq__", [](const CallArgs& a) {
            if (type_of(a.pos.at(1)) != type_of(a.pos[0])) return NotImplemented;
            return var(a.pos[0].getattr("__dict__") == a.pos[1].getattr("__dict__"));
        }));
        mod_set(m, "SimpleNamespace", make_class("SimpleNamespace", "SimpleNamespace", {}, sns, {}, "types"));
        // traceback and frame objects: translated code keeps neither, so these are types nothing is an instance of
        // (annotations and isinstance checks name them)
        for (const char* n : {"TracebackType", "FrameType", "CodeType"}) {
            var tns = new_dict();
            tns.setitem("__module__"_s, "builtins"_s);
            mod_set(m, n, make_class(n, n, {}, tns, {}, "builtins"));
        }
        mod_set(m, "ModuleType", t::module);
        mod_set(m, "FunctionType", t::function);
        mod_set(m, "BuiltinFunctionType", t::builtin_function);
        mod_set(m, "MethodType", t::method);
        mod_set(m, "GeneratorType", t::generator);
        {
            mod_set(m, "CoroutineType", coroutine_type_());
            mod_set(m, "AsyncGeneratorType", async_generator_type_());
            mod_def(m, "coroutine", [](const CallArgs& a) { return types_coroutine_(a.pos.at(0)); });
        }
        mod_set(m, "NoneType", t::NoneType);
        // list[int] and its kind: the origin itself (annotations are not evaluated for meaning)
        mod_def(m, "GenericAlias", [](const CallArgs& a) { return a.pos.at(0); });
        {
            // mappingproxy: a read-only view of a mapping (a type of its own: pprint dispatches on its __repr__)
            var ns = new_dict();
            ns.setitem("__init__"_s, def("__init__", "self, mapping", {}, [](var self, var mp) { self.setattr("_mapping", mp); return var(None); }));
            ns.setitem("__getitem__"_s, def("__getitem__", "self, key", {}, [](var self, var k) { return self.getattr("_mapping").getitem(k); }));
            ns.setitem("__iter__"_s, def("__iter__", "self", {}, [](var self) { return iter(self.getattr("_mapping")); }));
            ns.setitem("__len__"_s, def("__len__", "self", {}, [](var self) { return var(len_(self.getattr("_mapping"))); }));
            ns.setitem("__contains__"_s, def("__contains__", "self, key", {}, [](var self, var k) { return var(in(k, self.getattr("_mapping"))); }));
            for (const char* meth : {"keys", "values", "items", "copy"})
                ns.setitem(new_str(meth), nat(meth, [meth](const CallArgs& a) { return a.pos.at(0).getattr("_mapping").call(meth); }));
            ns.setitem("get"_s, def("get", "self, key, default=", {None}, [](var self, var k, var d) { return self.getattr("_mapping").call("get", k, d); }));
            ns.setitem("__repr__"_s, def("__repr__", "self", {}, [](var self) { return new_str("mappingproxy(" + repr_(self.getattr("_mapping")) + ")"); }));
            ns.setitem("__eq__"_s, def("__eq__", "self, other", {}, [](var self, var o) { return var(self.getattr("_mapping") == o); }));
            mod_set(m, "MappingProxyType", make_class("mappingproxy", "mappingproxy", {}, ns, {}, "builtins"));
        }
        mod_set(m, "BuiltinMethodType", t::builtin_function);
        mod_set(m, "LambdaType", t::function);
    });
    add_static(T_module, "__new__", [](const CallArgs& a) {
        auto* mo = new ModuleObj();
        mo->type = T_module;
        mo->name = str_utf8(a.pos.at(1));
        var v(static_cast<Obj*>(mo));
        mo->cell("__name__")->v = a.pos[1];
        mo->cell("__doc__")->v = a.pos.size() > 2 ? a.pos[2] : var(None);
        return v;
    });
    register_native_module("typing", [](const var& m) {
        for (const char* n : {"Any", "Optional", "Union", "List", "Dict", "Tuple", "Set", "FrozenSet", "Callable", "Iterable", "Iterator", "Generator",
                              "Sequence", "Mapping", "MutableMapping", "Type", "ClassVar", "Final", "Literal", "BinaryIO", "TextIO", "IO", "Annotated",
                              "NoReturn", "Self", "TypeAlias", "Deque", "DefaultDict", "Counter", "OrderedDict", "Awaitable", "Coroutine", "Hashable", "Sized"})
            mod_set(m, n, special_form(n));
        mod_set(m, "TYPE_CHECKING", false);
        mod_def(m, "TypeVar", [](const CallArgs& a) { return special_form(str_utf8(a.pos.at(0)).c_str()); });
        mod_def(m, "NewType", [](const CallArgs& a) { return a.pos.at(1); });
        mod_def(m, "cast", [](const CallArgs& a) { return a.pos.at(1); });
        mod_def(m, "overload", [](const CallArgs& a) { return a.pos.at(0); });
        mod_def(m, "final", [](const CallArgs& a) { return a.pos.at(0); });
        mod_def(m, "no_type_check", [](const CallArgs& a) { return a.pos.at(0); });
        mod_def(m, "runtime_checkable", [](const CallArgs& a) { return a.pos.at(0); });
        mod_def(m, "get_type_hints", [](const CallArgs& a) { return getattr(a.pos.at(0), "__annotations__"_s, new_dict()); });
        var gen = mkcls("Generic", {}, {}, "typing");
        add_classmethod(T(gen), "__class_getitem__", [](const CallArgs& a) { return a.pos[0]; });
        mod_set(m, "Generic", gen);
        mod_set(m, "Protocol", gen);
        mod_set(m, "NamedTuple", t::tuple);
        mod_set(m, "TypedDict", t::dict);
    });
    register_native_module("abc", [](const var& m) {
        // ABCMeta: a metaclass of its own, for register() (virtual subclasses are not consulted by isinstance)
        static TypeObj* T_abcmeta = new_type("ABCMeta", {T_type}, Layout::Type, true, "abc");
        add_static(T_abcmeta, "__new__", [](const CallArgs& a) {
            CallArgs b;
            b.pos.assign(a.pos.begin() + 1, a.pos.end());
            return type_new_(T(a.pos.at(0)), b);
        });
        // register(): a virtual subclass, in the class's own _abc_registry; isinstance / issubclass consult
        // it, and the structural hook (_abc_hook: the methods an instance's type must have: Iterable's
        // __iter__ ...), as collections.abc's __subclasshook__s do
        add_method(T_abcmeta, "register", [](const CallArgs& a) {
            TypeObj* t = T(a.pos.at(0));
            var* reg = t->dict->get("_abc_registry");
            if (!reg) { t->dict->set(interned("_abc_registry"), new_list()); reg = t->dict->get("_abc_registry"); }
            reg->call("append", a.pos.at(1));
            return a.pos[1];
        });
        add_method(T_abcmeta, "__subclasscheck__", [](const CallArgs& a) { return var(abc_subclass(T(a.pos.at(0)), T(a.pos.at(1)))); });
        add_method(T_abcmeta, "__instancecheck__", [](const CallArgs& a) { return var(abc_subclass(T(a.pos.at(0)), type_of(a.pos.at(1)))); });
        add_method(T_abcmeta, "_abc_registry_clear", [](const CallArgs&) { return None; });
        g_abcmeta = T_abcmeta;
        mod_set(m, "ABCMeta", tv(T_abcmeta));
        CallArgs c;
        c.pos = {"ABC"_s, new_tuple(), new_dict()};
        c.pos[2].setitem("__module__"_s, "abc"_s);
        c.pos[2].setitem("__slots__"_s, new_tuple());
        mod_set(m, "ABC", type_new_(T_abcmeta, c));
        mod_def(m, "get_cache_token", [](const CallArgs&) { return var(0); });
        mod_def(m, "abstractmethod", [](const CallArgs& a) { return a.pos.at(0); });
        mod_def(m, "abstractproperty", [](const CallArgs& a) { return property(a.pos.at(0)); });
    });
    register_native_module("uuid", [](const var& m) {
        var uns = new_dict();
        uns.setitem("__str__"_s, def("__str__", "self", {}, [](var self) {
            std::string h = str_utf8(self.getattr("hex"));
            return new_str(h.substr(0, 8) + "-" + h.substr(8, 4) + "-" + h.substr(12, 4) + "-" + h.substr(16, 4) + "-" + h.substr(20));
        }));
        uns.setitem("__repr__"_s, def("__repr__", "self", {}, [](var self) { return new_str("UUID('" + str_(self) + "')"); }));
        var U = make_class("UUID", "UUID", {}, uns, {}, "uuid");
        mod_set(m, "UUID", U);
        mod_def(m, "uuid4", [U](const CallArgs&) {
            unsigned char b[16];
            BCryptGenRandom(nullptr, b, 16, BCRYPT_USE_SYSTEM_PREFERRED_RNG);
            b[6] = (unsigned char)((b[6] & 0x0F) | 0x40);
            b[8] = (unsigned char)((b[8] & 0x3F) | 0x80);
            static const char* hx = "0123456789abcdef";
            std::string h;
            for (unsigned char c : b) { h.push_back(hx[c >> 4]); h.push_back(hx[c & 15]); }
            auto* o = new Obj();
            o->type = T(U);
            var u(o);
            u.setattr("hex", new_str(h));
            return u;
        });
    });
    register_native_module("traceback", [](const var& m) {
        auto fmt = []() {
            var e = current_exception();
            if (e.is_none()) return std::string("NoneType: None\n");
            std::string s = str_(e);
            TypeObj* t = type_of(e);
            std::string name = t->module == "builtins" ? t->qualname : t->module + "." + t->qualname;
            return "Traceback (most recent call last):\n  (translated code)\n" + name + (s.empty() ? "" : ": " + s) + "\n";
        };
        mod_def(m, "format_exc", [fmt](const CallArgs&) { return new_str(fmt()); });
        mod_def(m, "print_exc", [fmt](const CallArgs&) { g_sys.getattr("stderr").call("write", new_str(fmt())); return None; });
        mod_def(m, "format_exception", [](const CallArgs& a) {
            var e = a.pos.size() == 1 ? a.pos[0] : a.pos.at(1);
            std::string s = str_(e);
            return new_list({"Traceback (most recent call last):\n"_s, new_str(type_of(e)->qualname + (s.empty() ? "" : ": " + s) + "\n")});
        });
        mod_def(m, "print_exception", [](const CallArgs& a) { print_exception_(a.pos.size() == 1 ? a.pos[0] : a.pos.at(1)); return None; });
    });
    register_native_module("warnings", [](const var& m) {
        mod_def(m, "warn", [](const CallArgs& a) {
            var cat = arg(a, 1, "category", t::UserWarning);
            std::string msg = str_(a.pos.at(0));
            g_sys.getattr("stderr").call("write", new_str(T(cat)->name + ": " + msg + "\n"));
            return None;
        });
        for (const char* n : {"simplefilter", "filterwarnings", "resetwarnings"}) mod_def(m, n, [](const CallArgs&) { return None; });
        // warnings._deprecated(name, message=, *, remove): a DeprecationWarning, which CPython's default filters
        // do not show outside __main__
        mod_def(m, "_deprecated", [](const CallArgs&) { return None; });
        mod_set(m, "catch_warnings", import("contextlib").getattr("nullcontext"));
    });
    register_native_module("atexit", [](const var& m) {
        mod_def(m, "register", [](const CallArgs& a) {
            var f = a.pos.at(0);
            if (a.pos.size() > 1 || !a.kw.empty()) {
                CallArgs rest;
                rest.pos.assign(a.pos.begin() + 1, a.pos.end());
                rest.kw = a.kw;
                g_atexit->push_back(nat("atexit_cb", [f, rest](const CallArgs&) { return call_obj(f, rest); }));
            } else g_atexit->push_back(f);
            return f;
        });
        mod_def(m, "unregister", [](const CallArgs&) { return None; });
    });
    register_native_module("binascii", [](const var& m) {
        mod_def(m, "crc32", [](const CallArgs& a) {
            static uint32_t table[256];
            static bool init = false;
            if (!init) {
                for (uint32_t i = 0; i < 256; i++) { uint32_t c = i; for (int k = 0; k < 8; k++) c = c & 1 ? 0xEDB88320u ^ (c >> 1) : c >> 1; table[i] = c; }
                init = true;
            }
            uint32_t crc = a.pos.size() > 1 ? (uint32_t)a.pos[1].as_int64() : 0;
            crc = ~crc;
            for (unsigned char c : raw_of(a.pos.at(0))) crc = table[(crc ^ c) & 0xFF] ^ (crc >> 8);
            return var((int64_t)(uint32_t)~crc);
        });
        mod_def(m, "hexlify", [](const CallArgs& a) { return new_bytes(str_utf8(a.pos.at(0).call("hex"))); });
        mod_def(m, "b2a_hex", [](const CallArgs& a) { return new_bytes(str_utf8(a.pos.at(0).call("hex"))); });
        mod_def(m, "unhexlify", [](const CallArgs& a) { return t::bytes.call("fromhex", is_str(a.pos.at(0)) ? a.pos[0] : new_str(raw_of(a.pos[0]))); });
        mod_def(m, "a2b_hex", [](const CallArgs& a) { return t::bytes.call("fromhex", is_str(a.pos.at(0)) ? a.pos[0] : new_str(raw_of(a.pos[0]))); });
        var err = mkcls("Error", {t::ValueError}, {}, "binascii");
        mod_set(m, "Error", err);
        mod_set(m, "Incomplete", mkcls("Incomplete", {t::Exception}, {}, "binascii"));
        static const char* B64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
        mod_def(m, "b2a_base64", [](const CallArgs& a) {
            const std::string& s = raw_of(a.pos.at(0));
            bool nl = kwarg(a, "newline", true).truthy();
            std::string o;
            size_t i = 0;
            for (; i + 2 < s.size(); i += 3) {
                uint32_t v = ((unsigned char)s[i] << 16) | ((unsigned char)s[i + 1] << 8) | (unsigned char)s[i + 2];
                o += B64[v >> 18]; o += B64[(v >> 12) & 63]; o += B64[(v >> 6) & 63]; o += B64[v & 63];
            }
            if (i + 1 == s.size()) {
                uint32_t v = (unsigned char)s[i] << 16;
                o += B64[v >> 18]; o += B64[(v >> 12) & 63]; o += "==";
            } else if (i + 2 == s.size()) {
                uint32_t v = ((unsigned char)s[i] << 16) | ((unsigned char)s[i + 1] << 8);
                o += B64[v >> 18]; o += B64[(v >> 12) & 63]; o += B64[(v >> 6) & 63]; o += '=';
            }
            if (nl) o += '\n';
            return new_bytes(o);
        });
        mod_def(m, "a2b_base64", [err](const CallArgs& a) {
            // CPython's non-strict decoding: characters outside the alphabet skipped, padding ends a quad
            std::string s = is_str(a.pos.at(0)) ? str_utf8(a.pos[0]) : raw_of(a.pos[0]);
            bool strict = kwarg(a, "strict_mode", false).truthy();
            std::string o;
            uint32_t acc = 0;
            int bits = 0, quad = 0, pads = 0;
            for (unsigned char c : s) {
                if (c == '=') {
                    if (quad >= 2 && quad + ++pads >= 4) { quad = 0; pads = 0; acc = 0; bits = 0; }
                    continue;
                }
                const char* p = c ? strchr(B64, c) : nullptr;
                if (!p) {
                    if (strict) raise(make_exc_args(T(err), {"Only base64 data is allowed"_s}));
                    continue;
                }
                pads = 0;
                acc = (acc << 6) | (uint32_t)(p - B64);
                bits += 6;
                quad = (quad + 1) & 3;
                if (bits >= 8) {
                    bits -= 8;
                    o += (char)((acc >> bits) & 0xFF);
                }
            }
            if (quad == 1) {
                size_t nd = 0;
                for (unsigned char c : s) if (c && c != '=' && strchr(B64, c)) nd++;
                raise(make_exc_args(T(err), {new_str("Invalid base64-encoded string: number of data characters (" + std::to_string(nd) + ") cannot be 1 more than a multiple of 4")}));
            }
            if (quad != 0) raise(make_exc_args(T(err), {"Incorrect padding"_s}));
            return new_bytes(o);
        });
    });
    register_native_module("zlib", [](const var& m) {
        mod_set(m, "crc32", import("binascii").getattr("crc32"));
        mod_def(m, "adler32", [](const CallArgs& a) {
            uint32_t s1 = 1, s2 = 0;
            if (a.pos.size() > 1) { uint32_t v = (uint32_t)a.pos[1].as_int64(); s1 = v & 0xFFFF; s2 = v >> 16; }
            for (unsigned char c : raw_of(a.pos.at(0))) { s1 = (s1 + c) % 65521; s2 = (s2 + s1) % 65521; }
            return var((int64_t)((s2 << 16) | s1));
        });
        for (const char* n : {"compress", "decompress", "decompressobj", "compressobj"})
            mod_def(m, n, [n](const CallArgs&) -> var { raise(E_NotImplementedError, std::string("zlib.") + n + ": deflate is not in the C++ runtime yet"); });
        mod_set(m, "error", t::Exception);
    });
    register_native_module("keyword", [](const var& m) {
        std::vector<var> kw;
        for (const char* k : {"False", "None", "True", "and", "as", "assert", "async", "await", "break", "class", "continue", "def", "del", "elif", "else", "except",
                              "finally", "for", "from", "global", "if", "import", "in", "is", "lambda", "nonlocal", "not", "or", "pass", "raise", "return",
                              "try", "while", "with", "yield"})
            kw.push_back(new_str(k));
        var kl = new_list(kw);
        mod_set(m, "kwlist", kl);
        mod_def(m, "iskeyword", [kl](const CallArgs& a) { return var(in(a.pos.at(0), kl)); });
    });
    // modules with no C++ yet: importing works, using them says so
    for (const char* n : {"random", "argparse", "difflib", "csv", "zipfile", "gzip", "socket",
                          "ctypes", "asyncio", "datetime", "hmac", "secrets", "multiprocessing",
                          "faulthandler", "msvcrt", "locale", "shlex", "queue", "signal", "weakref", "pprint", "tomllib", "unittest"}) {
        std::string name = n;
        register_native_module(n, [](const var& m) {
            std::string nm = str_utf8(m.getattr("__name__"));
            mod_def(m, "__getattr__", [nm](const CallArgs& a) -> var {
                std::string at = str_utf8(a.pos.at(0));
                if (at.size() > 4 && at.compare(0, 2, "__") == 0) raise(E_AttributeError, at);
                // a placeholder that raises when called: the module's API is not in the C++ runtime yet
                return nat(at.c_str(), [nm, at](const CallArgs&) -> var {
                    raise(E_NotImplementedError, nm + "." + at + " is not in the C++ runtime yet");
                });
            });
        });
    }
    init_extra_modules();   // (after the placeholders: the modules it has replace theirs)
}

}  // namespace py
