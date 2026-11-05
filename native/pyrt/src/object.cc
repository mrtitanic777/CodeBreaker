// object.cc -- the object model: values' types, truth, attribute lookup through the MRO with the
// descriptor protocol, classes (C3 MRO, metaclasses, __init_subclass__, __set_name__), super(),
// isinstance, property / classmethod / staticmethod, literals and interned names.
#include <algorithm>
#include <cstring>

#include "rt.hpp"

namespace py {

TypeObj *T_object, *T_type, *T_none, *T_bool, *T_int, *T_float, *T_complex, *T_str, *T_bytes,
    *T_bytearray, *T_list, *T_tuple, *T_dict, *T_set, *T_frozenset, *T_range, *T_slice, *T_function,
    *T_native, *T_method, *T_module, *T_cell, *T_generator, *T_iter, *T_property, *T_staticmethod,
    *T_classmethod, *T_super, *T_notimpl, *T_ellipsis, *T_data;

Obj::~Obj() {
    if (attrs) decref(attrs);
}
void dealloc_(Obj* o) { delete o; }

TypeObj::~TypeObj() {
    if (dict) decref(dict);
}

var tv(TypeObj* t) { return var(static_cast<Obj*>(t)); }
var type_var(TypeObj* t) { return tv(t); }
TypeObj* T(const var& v) {
    if (!v.is_ref() || v.ref()->type->layout != Layout::Type) raise(E_TypeError, "not a type: " + tname(v));
    return static_cast<TypeObj*>(v.ref());
}

TypeObj* type_of(const var& v) {
    switch (v.tag()) {
    case var::Tag::None: return T_none;
    case var::Tag::Bool: return T_bool;
    case var::Tag::Int: return T_int;
    case var::Tag::Float: return T_float;
    case var::Tag::Ref: return v.ref()->type;
    default: raise(E_UnboundLocalError, "cannot access a variable that has no value yet");
    }
}
TypeObj* var::type() const { return type_of(*this); }
std::string tname(const var& v) { return v.is_unbound() ? "unbound" : type_of(v)->name; }

var::var(unsigned long long v) {
    if (v <= (unsigned long long)INT64_MAX) { tag_ = Tag::Int; i_ = (int64_t)v; return; }
    tag_ = Tag::Unbound; i_ = 0;
    *this = new_int(Big::from_decimal(std::to_string(v)));
}
var::var(const char* utf8) : tag_(Tag::Unbound), i_(0) { *this = new_str(utf8); }
var::var(const std::string& utf8) : tag_(Tag::Unbound), i_(0) { *this = new_str(utf8); }

// ------------------------------------------------------------------------------------------ truth
var::operator bool() const {
    switch (tag_) {
    case Tag::None: return false;
    case Tag::Bool:
    case Tag::Int: return i_ != 0;
    case Tag::Float: return f_ != 0.0;
    case Tag::Unbound: type_of(*this);
    case Tag::Ref: break;
    }
    Obj* o = p_;
    switch (o->type->layout) {
    case Layout::Str: if (o->type == T_str) return static_cast<StrObj*>(o)->n != 0; break;
    case Layout::List: if (o->type == T_list) return !static_cast<ListObj*>(o)->v.empty(); break;
    case Layout::Tuple: if (o->type == T_tuple) return !static_cast<TupleObj*>(o)->v.empty(); break;
    case Layout::Dict: if (o->type == T_dict) return static_cast<DictObj*>(o)->used != 0; break;
    case Layout::Int: if (o->type == T_int) return static_cast<BigObj*>(o)->v.sign() != 0; break;
    default: break;
    }
    var m;
    if (lookup_special(*this, "__bool__", m)) {
        var r = call_obj(m, CallArgs());
        if (!r.is_bool()) raise(E_TypeError, "__bool__ should return bool, returned " + tname(r));
        return r.small() != 0;
    }
    if (lookup_special(*this, "__len__", m)) return index_(call_obj(m, CallArgs())) != 0;
    return true;
}
bool not_(const var& a) { return !a; }

// ------------------------------------------------------------------------------------------ interned
var interned(const char* name) {
    static std::unordered_map<const char*, Obj*>* byptr = new std::unordered_map<const char*, Obj*>();
    static std::unordered_map<std::string, Obj*>* bytext = new std::unordered_map<std::string, Obj*>();
    auto it = byptr->find(name);
    // (keyed by the address for string literals; a name in a buffer that is reused is checked by its text)
    if (it != byptr->end() && static_cast<StrObj*>(it->second)->s == name) return var(it->second);
    auto jt = bytext->find(name);
    Obj* o;
    if (jt != bytext->end()) o = jt->second;
    else {
        var s = new_str(name);
        o = s.ref();
        incref(o);                  // (held for ever)
        (*bytext)[name] = o;
    }
    (*byptr)[name] = o;
    return var(o);
}
var str_lit_(const char* p, size_t n) {
    static std::unordered_map<const char*, Obj*>* lits = new std::unordered_map<const char*, Obj*>();
    auto it = lits->find(p);
    if (it != lits->end()) return var(it->second);
    var s = new_str(std::string(p, n));
    incref(s.ref());
    (*lits)[p] = s.ref();
    return s;
}
var bytes_lit_(const char* p, size_t n) {
    static std::unordered_map<const char*, Obj*>* lits = new std::unordered_map<const char*, Obj*>();
    auto it = lits->find(p);
    if (it != lits->end()) return var(it->second);
    var s = new_bytes(std::string(p, n));
    incref(s.ref());
    (*lits)[p] = s.ref();
    return s;
}
var int_lit_(const char* digits) {
    static std::unordered_map<const char*, Obj*>* lits = new std::unordered_map<const char*, Obj*>();
    auto it = lits->find(digits);
    if (it != lits->end()) return var(it->second);
    var v = new_int(Big::from_decimal(digits));
    if (v.is_ref()) { incref(v.ref()); (*lits)[digits] = v.ref(); }
    return v;
}

// ------------------------------------------------------------------------------------------ types
const var* TypeObj::lookup(const var& name) const {
    for (TypeObj* t : mro)
        if (t->dict)
            if (var* v = t->dict->get(name)) return v;
    return nullptr;
}
const var* TypeObj::lookup(const char* name) const {
    for (TypeObj* t : mro)
        if (t->dict)
            if (var* v = t->dict->get(name)) return v;
    return nullptr;
}

bool is_subtype(TypeObj* a, TypeObj* b) {
    if (a == b || b == T_object) return true;
    for (TypeObj* t : a->mro)
        if (t == b) return true;
    return false;
}
bool isinstance_(const var& x, TypeObj* t) { return is_subtype(type_of(x), t); }

static std::vector<TypeObj*> c3(TypeObj* self, const std::vector<TypeObj*>& bases) {
    std::vector<std::vector<TypeObj*>> seqs;
    for (TypeObj* b : bases) seqs.push_back(b->mro);
    seqs.push_back(bases);
    std::vector<TypeObj*> out{self};
    for (;;) {
        seqs.erase(std::remove_if(seqs.begin(), seqs.end(), [](auto& s) { return s.empty(); }), seqs.end());
        if (seqs.empty()) return out;
        TypeObj* cand = nullptr;
        for (auto& s : seqs) {
            TypeObj* h = s[0];
            bool in_tail = false;
            for (auto& s2 : seqs)
                if (std::find(s2.begin() + 1, s2.end(), h) != s2.end()) { in_tail = true; break; }
            if (!in_tail) { cand = h; break; }
        }
        if (!cand) raise(E_TypeError, "Cannot create a consistent method resolution order (MRO) for bases");
        out.push_back(cand);
        for (auto& s : seqs)
            if (s[0] == cand) s.erase(s.begin());
    }
}

TypeObj* new_type(const char* name, std::vector<TypeObj*> bases, Layout layout, bool builtin, const char* module) {
    auto* t = new TypeObj();
    t->type = T_type;
    t->name = name;
    t->qualname = name;
    t->module = module ? module : "builtins";
    if (bases.empty() && T_object && std::strcmp(name, "object") != 0) bases.push_back(T_object);
    t->bases = bases;
    t->layout = layout;
    t->builtin = builtin;
    if (T_object || std::strcmp(name, "object") == 0) t->mro = c3(t, bases);
    var d = new_dict_obj();
    t->dict = static_cast<DictObj*>(d.ref());
    incref(t->dict);
    t->dict->owner_type = t;
    incref(t);                       // builtin types live for ever
    for (TypeObj* b : bases)
        if (b->is_exception) t->is_exception = true;
    return t;
}

static var new_native_method(const char* name, native_fn fn) { return new_native(name, std::move(fn)); }
void add_method(TypeObj* t, const char* name, native_fn fn) { t->dict->set(interned(name), new_native_method(name, std::move(fn))); }
void add_static(TypeObj* t, const char* name, native_fn fn) {
    auto* w = new WrapObj();
    w->type = T_staticmethod;
    w->f = new_native(name, std::move(fn));
    t->dict->set(interned(name), var(w));
}
void add_classmethod(TypeObj* t, const char* name, native_fn fn) {
    auto* w = new WrapObj();
    w->type = T_classmethod;
    w->f = new_native(name, std::move(fn));
    t->dict->set(interned(name), var(w));
}
void add_getter(TypeObj* t, const char* name, std::function<var(const var&)> get) {
    auto* p = new PropertyObj();
    p->type = T_property;
    p->name = name;
    p->fget = new_native(name, [get](const CallArgs& a) { return get(a.pos[0]); });
    t->dict->set(interned(name), var(p));
}

// ------------------------------------------------------------------------------------------ descriptors
static bool is_data_descriptor(const var& a) {
    if (!a.is_ref()) return false;
    TypeObj* t = a.ref()->type;
    if (t == T_property) return true;
    if (t->builtin) return false;
    return t->lookup("__set__") || t->lookup("__delete__");
}

var bind(const var& attr, const var& self, TypeObj* t) {
    if (!attr.is_ref()) return attr;
    Obj* o = attr.ref();
    TypeObj* at = o->type;
    if (at == T_function) {
        if (self.is_unbound()) return attr;
        auto* m = new MethodObj();
        m->type = T_method;
        m->func = attr;
        m->self = self;
        return var(m);
    }
    if (at == T_native) {
        if (self.is_unbound()) return attr;
        return bind_native(attr, self);
    }
    if (at == T_staticmethod) return static_cast<WrapObj*>(o)->f;
    if (at == T_classmethod) {
        auto* m = new MethodObj();
        m->type = T_method;
        m->func = static_cast<WrapObj*>(o)->f;
        m->self = tv(t);
        return var(m);
    }
    if (at == T_property) {
        if (self.is_unbound()) return attr;
        auto* p = static_cast<PropertyObj*>(o);
        if (p->fget.is_none() || p->fget.is_unbound()) raise(E_AttributeError, "property '" + p->name + "' of '" + t->name + "' object has no getter");
        return call1(p->fget, self);
    }
    if (!at->builtin) {
        if (const var* g = at->lookup("__get__"))
            return call_obj(bind(*g, attr, at), pack_args_(self.is_unbound() ? var(None) : self, tv(t)));
    }
    return attr;
}

bool lookup_special(const var& o, const char* name, var& out) {
    TypeObj* t = type_of(o);
    const var* f = t->lookup(name);
    if (!f) return false;
    out = bind(*f, o, t);
    return true;
}

// ------------------------------------------------------------------------------------------ getattr
static var type_special_attr(TypeObj* t, const std::string& n, bool& found) {
    found = true;
    if (n == "__name__") return new_str(t->name);
    if (n == "__qualname__") return new_str(t->qualname);
    if (n == "__module__") {
        if (var* m = t->dict->get("__module__")) return *m;
        return new_str(t->module);
    }
    if (n == "__dict__") return var(static_cast<Obj*>(t->dict));
    if (n == "__mro__") {
        std::vector<var> v;
        for (TypeObj* b : t->mro) v.push_back(tv(b));
        return new_tuple(v);
    }
    if (n == "__bases__") {
        std::vector<var> v;
        for (TypeObj* b : t->bases) v.push_back(tv(b));
        return new_tuple(v);
    }
    if (n == "__base__") return t->bases.empty() ? var(None) : tv(t->bases[0]);
    if (n == "__doc__") {
        if (var* d = t->dict->get("__doc__")) return *d;
        return None;
    }
    if (n == "__class__") return tv(t->metatype ? t->metatype : T_type);
    found = false;
    return var();
}

static var getattr_type(TypeObj* t, const var& name, bool& found) {
    const std::string& n = utf8_of(name);
    TypeObj* meta = t->metatype ? t->metatype : T_type;
    // a data descriptor of the metaclass
    const var* ma = meta->lookup(name);
    if (ma && is_data_descriptor(*ma)) { found = true; return bind(*ma, tv(t), meta); }
    if (const var* a = t->lookup(name)) {
        found = true;
        const var& v = *a;
        if (v.is_ref()) {
            TypeObj* at = v.ref()->type;
            if (at == T_staticmethod) return static_cast<WrapObj*>(v.ref())->f;
            if (at == T_classmethod) return bind(v, tv(t), t);
            if (at == T_function || at == T_native || at == T_property) return v;
            if (!at->builtin && at->lookup("__get__"))
                return call_obj(bind(*at->lookup("__get__"), v, at), pack_args_(var(None), tv(t)));
        }
        return v;
    }
    var sp = type_special_attr(t, n, found);
    if (found) return sp;
    if (ma) { found = true; return bind(*ma, tv(t), meta); }
    found = false;
    return var();
}

static var getattr_module(ModuleObj* m, const var& name, bool& found);
var super_getattr_(SuperObj* s, const var& name, bool& found);

static var getattr_impl(const var& o, const var& name, bool& found) {
    found = true;
    TypeObj* t = type_of(o);
    if (t->layout == Layout::Type && o.is_ref() && o.ref()->type->layout == Layout::Type) {
        // (a class object: o is the class)
        return getattr_type(static_cast<TypeObj*>(o.ref()), name, found);
    }
    if (o.is_ref() && t == T_module) return getattr_module(static_cast<ModuleObj*>(o.ref()), name, found);
    if (o.is_ref() && t == T_super) {
        var v = super_getattr_(static_cast<SuperObj*>(o.ref()), name, found);
        if (found) return v;
        found = true;
    }
    const var* a = t->lookup(name);
    if (a && is_data_descriptor(*a)) return bind(*a, o, t);
    if (o.is_ref() && o.ref()->attrs)
        if (var* v = static_cast<DictObj*>(o.ref()->attrs)->get(name)) return *v;
    if (a) return bind(*a, o, t);
    const std::string& n = utf8_of(name);
    if (n == "__class__") return tv(t);
    if (n == "__dict__" && o.is_ref()) {
        Obj* p = o.ref();
        if (!p->attrs && !t->builtin) { var d = new_dict_obj(); p->attrs = d.ref(); incref(p->attrs); }
        if (p->attrs) return var(p->attrs);
    }
    if (n == "__doc__") return None;
    // FuncObj and friends: their own attributes
    if (o.is_ref()) {
        Obj* p = o.ref();
        if (t == T_function) {
            auto* f = static_cast<FuncObj*>(p);
            if (n == "__name__") return new_str(f->name);
            if (n == "__qualname__") return new_str(f->qualname);
            if (n == "__module__") return new_str(f->module);
            if (n == "__doc__") return f->doc.is_unbound() ? var(None) : f->doc;
            if (n == "__wrapped__" && !f->wrapped.is_unbound()) return f->wrapped;
            if (n == "__defaults__") {
                std::vector<var> d;
                for (auto& x : f->defaults) if (!x.is_unbound()) d.push_back(x);
                return d.empty() ? var(None) : new_tuple(d);
            }
            if (n == "__annotations__") return f->annotations.is_unbound() ? new_dict_obj() : f->annotations;
        } else if (t == T_native) {
            if (n == "__name__" || n == "__qualname__") return new_str(static_cast<NativeObj*>(p)->name);
            if (n == "__self__") return static_cast<NativeObj*>(p)->self;
        } else if (t == T_method) {
            auto* m = static_cast<MethodObj*>(p);
            if (n == "__self__") return m->self;
            if (n == "__func__") return m->func;
            return getattr_impl(m->func, name, found);
        } else if (t == T_property) {
            auto* pr = static_cast<PropertyObj*>(p);
            if (n == "fget") return pr->fget;
            if (n == "fset") return pr->fset;
            if (n == "__doc__") return pr->doc;
        } else if (t == T_staticmethod || t == T_classmethod) {
            if (n == "__func__" || n == "__wrapped__") return static_cast<WrapObj*>(p)->f;
        } else if (t->is_exception || t->layout == Layout::Exception) {
            auto* e = static_cast<ExcObj*>(p);
            if (n == "args") return e->args;
            if (n == "__cause__") return e->cause.is_unbound() ? var(None) : e->cause;
            if (n == "__context__") return e->context.is_unbound() ? var(None) : e->context;
            if (n == "__traceback__") return None;
            if (n == "__suppress_context__") return var(e->suppress_context);
        }
    }
    // __getattr__
    if (const var* ga = t->lookup("__getattr__")) {
        try {
            return call1(bind(*ga, o, t), name);
        } catch (error& e) {
            if (!isinstance_(e.value, E_AttributeError)) throw;
        }
    }
    found = false;
    return var();
}

[[noreturn]] static void no_attr(const var& o, const var& name) {
    TypeObj* t = type_of(o);
    if (o.is_ref() && t->layout == Layout::Type && o.ref()->type->layout == Layout::Type)
        raise(E_AttributeError, "type object '" + static_cast<TypeObj*>(o.ref())->name + "' has no attribute '" + utf8_of(name) + "'");
    if (o.is_ref() && t == T_module)
        raise(E_AttributeError, "module '" + static_cast<ModuleObj*>(o.ref())->name + "' has no attribute '" + utf8_of(name) + "'");
    var e = make_exc(E_AttributeError, "'" + t->name + "' object has no attribute '" + utf8_of(name) + "'");
    e.setattr("name", name);
    e.setattr("obj", o);
    raise(e);
}

var var::getattr(const var& name) const {
    bool found;
    if (const TypeObj* t = type_of(*this); t->lookup("__getattribute__") && !t->builtin) {
        const var* ga = t->lookup("__getattribute__");
        if (ga->is_ref() && ga->ref()->type == T_function) return call1(bind(*ga, *this, type_of(*this)), name);
    }
    var v = getattr_impl(*this, name, found);
    if (!found) no_attr(*this, name);
    return v;
}
var var::getattr(const char* name) const { return getattr(interned(name)); }
bool var::hasattr(const char* name) const {
    try {
        bool found;
        getattr_impl(*this, interned(name), found);
        return found;
    } catch (error& e) {
        if (isinstance_(e.value, E_AttributeError)) return false;
        throw;
    }
}

// ------------------------------------------------------------------------------------------ setattr
static void setattr_v(const var& o, const var& name, const var& v) {
    TypeObj* t = type_of(o);
    if (o.is_ref() && t->layout == Layout::Type && o.ref()->type->layout == Layout::Type) {
        auto* ty = static_cast<TypeObj*>(o.ref());
        TypeObj* meta = ty->metatype ? ty->metatype : T_type;
        if (const var* sa = meta->lookup("__setattr__"); sa && !meta->builtin) {
            call_obj(bind(*sa, o, meta), pack_args_(name, v));
            return;
        }
        ty->dict->set(name, v);
        return;
    }
    if (o.is_ref() && t == T_module) {
        auto* m = static_cast<ModuleObj*>(o.ref());
        m->cell(utf8_of(name))->v = v;
        return;
    }
    if (!t->builtin) {
        if (const var* sa = t->lookup("__setattr__")) {
            if (sa->is_ref() && sa->ref()->type == T_function) {
                call_obj(bind(*sa, o, t), pack_args_(name, v));
                return;
            }
        }
    }
    if (const var* a = t->lookup(name)) {
        if (a->is_ref()) {
            TypeObj* at = a->ref()->type;
            if (at == T_property) {
                auto* p = static_cast<PropertyObj*>(a->ref());
                if (p->fset.is_none() || p->fset.is_unbound()) raise(E_AttributeError, "property '" + utf8_of(name) + "' of '" + t->name + "' object has no setter");
                call2(p->fset, o, v);
                return;
            }
            if (!at->builtin)
                if (const var* s = at->lookup("__set__")) {
                    call_obj(bind(*s, *a, at), pack_args_(o, v));
                    return;
                }
        }
    }
    if (!o.is_ref()) raise(E_AttributeError, "'" + t->name + "' object has no attribute '" + utf8_of(name) + "'");
    Obj* p = o.ref();
    if (t == T_function) {
        auto* f = static_cast<FuncObj*>(p);
        const std::string& n = utf8_of(name);
        if (n == "__name__") { f->name = utf8_of(v); return; }
        if (n == "__qualname__") { f->qualname = utf8_of(v); return; }
        if (n == "__doc__") { f->doc = v; return; }
        if (n == "__module__") { f->module = v.is_none() ? "" : utf8_of(v); return; }
        if (n == "__wrapped__") { f->wrapped = v; return; }
    }
    if (t->is_exception) {
        const std::string& n = utf8_of(name);
        auto* e = static_cast<ExcObj*>(p);
        if (n == "__cause__") { e->cause = v; e->suppress_context = true; return; }
        if (n == "__context__") { e->context = v; return; }
        if (n == "args") { e->args = new_tuple(collect(v)); return; }
        if (n == "__traceback__") return;
    }
    if (t->builtin && t != T_function && !t->is_exception && t != T_data && t->layout != Layout::NativeData)
        raise(E_AttributeError, "'" + t->name + "' object has no attribute '" + utf8_of(name) + "' (it cannot be set)");
    if (!p->attrs) {
        var d = new_dict_obj();
        p->attrs = d.ref();
        incref(p->attrs);
    }
    static_cast<DictObj*>(p->attrs)->set(name, v);
}
void var::setattr(const char* name, const var& v) const { setattr_v(*this, interned(name), v); }
void var::delattr(const char* name) const {
    TypeObj* t = type_of(*this);
    var n = interned(name);
    if (const var* da = t->lookup("__delattr__"); da && !t->builtin) {
        call1(bind(*da, *this, t), n);
        return;
    }
    if (is_ref() && t->layout == Layout::Type && ref()->type->layout == Layout::Type) {
        if (!static_cast<TypeObj*>(ref())->dict->del(n)) no_attr(*this, n);
        return;
    }
    if (is_ref() && t == T_module) {
        auto* m = static_cast<ModuleObj*>(ref());
        auto it = m->cells.find(name);
        if (it == m->cells.end() || it->second->v.is_unbound()) no_attr(*this, n);
        it->second->v = var();
        return;
    }
    if (const var* a = t->lookup(n); a && a->is_ref() && a->ref()->type == T_property) {
        auto* p = static_cast<PropertyObj*>(a->ref());
        if (p->fdel.is_none() || p->fdel.is_unbound()) raise(E_AttributeError, "property has no deleter");
        call1(p->fdel, *this);
        return;
    }
    if (!is_ref() || !ref()->attrs || !static_cast<DictObj*>(ref()->attrs)->del(n)) no_attr(*this, n);
}

var getattr(const var& o, const var& name) {
    if (!is_str(name)) raise(E_TypeError, "attribute name must be string, not '" + tname(name) + "'");
    return o.getattr(name);
}
var getattr(const var& o, const var& name, const var& dflt) {
    if (!is_str(name)) raise(E_TypeError, "attribute name must be string, not '" + tname(name) + "'");
    try {
        bool found;
        var v = getattr_impl(o, name, found);
        return found ? v : dflt;
    } catch (error& e) {
        if (isinstance_(e.value, E_AttributeError)) return dflt;
        throw;
    }
}
void setattr(const var& o, const var& name, const var& v) {
    if (!is_str(name)) raise(E_TypeError, "attribute name must be string, not '" + tname(name) + "'");
    setattr_v(o, name, v);
}
var hasattr(const var& o, const var& name) {
    if (!is_str(name)) raise(E_TypeError, "attribute name must be string, not '" + tname(name) + "'");
    try {
        bool found;
        getattr_impl(o, name, found);
        return found;
    } catch (error& e) {
        if (isinstance_(e.value, E_AttributeError)) return false;
        throw;
    }
}
void delattr(const var& o, const var& name) { o.delattr(utf8_of(name).c_str()); }

// ------------------------------------------------------------------------------------------ modules' attributes
ModuleObj::~ModuleObj() {
    for (auto& [k, c] : cells) decref(c);
}
CellObj* ModuleObj::cell(const std::string& n) {
    auto it = cells.find(n);
    if (it != cells.end()) return it->second;
    auto* c = new CellObj();
    c->type = T_cell;
    incref(c);
    cells[n] = c;
    order.push_back(n);
    return c;
}
static var getattr_module(ModuleObj* m, const var& name, bool& found) {
    const std::string& n = utf8_of(name);
    found = true;
    auto it = m->cells.find(n);
    if (it != m->cells.end() && !it->second->v.is_unbound()) return it->second->v;
    if (n == "__name__") return new_str(m->name);
    if (n == "__dict__") {
        var d = new_dict_obj();
        for (auto& k : m->order) {
            auto c = m->cells[k];
            if (!c->v.is_unbound()) static_cast<DictObj*>(d.ref())->set(new_str(k), c->v);
        }
        return d;
    }
    auto ga = m->cells.find("__getattr__");
    if (ga != m->cells.end() && !ga->second->v.is_unbound()) return call1(ga->second->v, name);
    found = false;
    return var();
}

// ------------------------------------------------------------------------------------------ classes
static const char* const kNewIsStatic = "__new__";

var new_dict() { return new_dict_obj(); }

var make_class(const char* name, const char* qualname, const std::vector<var>& bases_v, const var& ns,
               const std::vector<std::pair<const char*, var>>& kw, const char* module) {
    std::vector<TypeObj*> bases;
    for (auto& b : bases_v) {
        if (!b.is_ref() || b.ref()->type->layout != Layout::Type) {
            // __mro_entries__ (typing.Generic[...], NamedTuple ...)
            var me;
            if (b.is_ref() && lookup_special(b, "__mro_entries__", me)) {
                for (auto& x : collect(call1(me, new_tuple(bases_v)))) bases.push_back(T(x));
                continue;
            }
            raise(E_TypeError, "bases must be types");
        }
        bases.push_back(static_cast<TypeObj*>(b.ref()));
    }
    // the metaclass: given, or the most derived of the bases'
    var meta;
    std::vector<std::pair<const char*, var>> rest;
    for (auto& [k, v] : kw) {
        if (std::strcmp(k, "metaclass") == 0) meta = v;
        else rest.push_back({k, v});
    }
    if (meta.is_unbound()) {
        TypeObj* m = T_type;
        for (TypeObj* b : bases) {
            TypeObj* bm = b->metatype ? b->metatype : T_type;
            if (is_subtype(bm, m)) m = bm;
        }
        meta = tv(m);
    }
    TypeObj* mt = T(meta);
    if (mt != T_type) {
        // a metaclass of its own: type(name, bases, ns, **kw) through it
        CallArgs a;
        a.add(new_str(name));
        a.add(new_tuple(bases_v));
        a.add(ns);
        for (auto& [k, v] : rest) a.add(kw_arg{k, v});
        var cls = call_obj(meta, a);
        return cls;
    }
    Layout layout = Layout::Object;
    for (TypeObj* b : bases)
        if (b->layout != Layout::Object) { layout = b->layout; break; }
    auto* t = new TypeObj();
    t->type = T_type;
    t->name = name;
    t->qualname = qualname ? qualname : name;
    t->module = module ? module : "__main__";
    if (bases.empty()) bases.push_back(T_object);
    t->bases = bases;
    t->mro = c3(t, bases);
    t->layout = layout;
    for (TypeObj* b : bases) {
        if (b->is_exception) t->is_exception = true;
        if (b->metatype && !t->metatype) t->metatype = b->metatype;
    }
    var tvar = var(static_cast<Obj*>(t));
    // the namespace becomes the class dict (a copy: the body's dict is the class's own)
    var d = new_dict_obj();
    auto* dict = static_cast<DictObj*>(d.ref());
    auto* src = static_cast<DictObj*>(ns.ref());
    for (auto& e : src->entries) {
        if (e.key.is_unbound()) continue;
        var v = e.value;
        // __new__, __init_subclass__ and __class_getitem__ are implicitly static / class methods
        if (is_str(e.key)) {
            const std::string& k = str_utf8(e.key);
            if (k == kNewIsStatic && v.is_ref() && v.ref()->type == T_function) {
                auto* w = new WrapObj(); w->type = T_staticmethod; w->f = v; v = var(w);
            } else if ((k == "__init_subclass__" || k == "__class_getitem__") && v.is_ref() && v.ref()->type == T_function) {
                auto* w = new WrapObj(); w->type = T_classmethod; w->f = v; v = var(w);
            }
        }
        dict->set(e.key, v);
    }
    if (!dict->get("__module__")) dict->set(interned("__module__"), new_str(t->module));
    if (!dict->get("__qualname__")) dict->set(interned("__qualname__"), new_str(t->qualname));
    else t->qualname = utf8_of(*dict->get("__qualname__"));
    if (!dict->get("__doc__")) dict->set(interned("__doc__"), None);
    t->dict = dict;
    incref(dict);
    dict->owner_type = t;
    // __set_name__ on the attributes
    for (auto& e : dict->entries) {
        if (e.key.is_unbound() || !e.value.is_ref()) continue;
        TypeObj* at = e.value.ref()->type;
        if (at == T_property) {
            auto* p = static_cast<PropertyObj*>(e.value.ref());
            if (p->name.empty()) p->name = str_utf8(e.key);
            continue;
        }
        if (!at->builtin)
            if (const var* sn = at->lookup("__set_name__")) call_obj(bind(*sn, e.value, at), pack_args_(tvar, e.key));
    }
    // __init_subclass__ of the parent
    if (t->mro.size() > 1) {
        TypeObj* parent = t->mro[1];
        if (const var* is = parent->lookup("__init_subclass__")) {
            if (is->is_ref() && is->ref()->type == T_classmethod) {
                CallArgs a;
                a.add(tvar);
                for (auto& [k, v] : rest) a.add(kw_arg{k, v});
                call_obj(static_cast<WrapObj*>(is->ref())->f, a);
            }
        }
    }
    return tvar;
}

// type(name, bases, ns) and type.__new__(meta, name, bases, ns): a class whose metaclass is meta
var type_new_(TypeObj* meta, const CallArgs& a) {
    var name = a.pos.at(0), bases = a.pos.at(1), ns = a.pos.at(2);
    std::vector<var> bv = collect(bases);
    std::string n = utf8_of(name);
    var nsd = ns;
    if (!is_dict(ns)) {
        nsd = new_dict_obj();
        for (auto& k : collect(ns)) static_cast<DictObj*>(nsd.ref())->set(k, ns.getitem(k));
    }
    var mod;
    if (var* m = static_cast<DictObj*>(nsd.ref())->get("__module__")) mod = *m;
    std::string ms = mod.is_unbound() ? "__main__" : (is_str(mod) ? str_utf8(mod) : "__main__");
    std::vector<std::pair<const char*, var>> kw;
    var cls = make_class(n.c_str(), n.c_str(), bv, nsd, kw, ms.c_str());
    TypeObj* t = T(cls);
    if (meta != T_type) {
        t->metatype = meta;
        t->type = meta;
        incref(meta);
    }
    return cls;
}

// ------------------------------------------------------------------------------------------ super
var super_(const var& cls, const var& self) {
    auto* s = new SuperObj();
    s->type = T_super;
    s->cls = cls;
    s->self = self;
    return var(s);
}
// super().name: after cls in the MRO of type(self) (or of self when it is a class: classmethods)
var super_getattr_(SuperObj* s, const var& name, bool& found) {
    TypeObj* cls = T(s->cls);
    TypeObj* start;
    bool self_is_type = s->self.is_ref() && s->self.ref()->type->layout == Layout::Type &&
                        is_subtype(static_cast<TypeObj*>(s->self.ref()), cls);
    start = self_is_type ? static_cast<TypeObj*>(s->self.ref()) : type_of(s->self);
    auto& mro = start->mro;
    auto it = std::find(mro.begin(), mro.end(), cls);
    if (it != mro.end()) ++it;
    for (; it != mro.end(); ++it) {
        if (var* v = (*it)->dict->get(name)) {
            found = true;
            const var& a = *v;
            if (a.is_ref() && a.ref()->type == T_classmethod) return bind(a, var(), start);
            if (a.is_ref() && a.ref()->type == T_staticmethod) return static_cast<WrapObj*>(a.ref())->f;
            if (self_is_type) return a;
            return bind(a, s->self, start);
        }
    }
    found = false;
    return var();
}

// ------------------------------------------------------------------------------------------ isinstance
static bool isinstance_one(const var& x, const var& cls) {
    if (cls.is_ref() && cls.ref()->type->layout == Layout::Type) {
        auto* t = static_cast<TypeObj*>(cls.ref());
        TypeObj* meta = t->metatype ? t->metatype : T_type;
        if (meta != T_type)
            if (const var* ic = meta->lookup("__instancecheck__"))
                return call1(bind(*ic, cls, meta), x).truthy();
        if (isinstance_(x, t)) return true;
        // (an instance whose __class__ says otherwise)
        return false;
    }
    if (is_tuple(cls)) {
        for (auto& c : tuple_items(cls))
            if (isinstance_one(x, c)) return true;
        return false;
    }
    // typing unions (int | str)
    var args;
    if (cls.is_ref() && cls.hasattr("__args__")) {
        for (auto& c : collect(cls.getattr("__args__")))
            if (isinstance_one(x, c)) return true;
        return false;
    }
    raise(E_TypeError, "isinstance() arg 2 must be a type, a tuple of types, or a union");
}
var isinstance(const var& x, const var& cls) { return isinstance_one(x, cls); }
static bool issubclass_one(TypeObj* c, const var& cls) {
    if (cls.is_ref() && cls.ref()->type->layout == Layout::Type) {
        auto* t = static_cast<TypeObj*>(cls.ref());
        TypeObj* meta = t->metatype ? t->metatype : T_type;
        if (meta != T_type)
            if (const var* sc = meta->lookup("__subclasscheck__"))
                return call1(bind(*sc, cls, meta), tv(c)).truthy();
        return is_subtype(c, t);
    }
    if (is_tuple(cls)) {
        for (auto& x : tuple_items(cls))
            if (issubclass_one(c, x)) return true;
        return false;
    }
    raise(E_TypeError, "issubclass() arg 2 must be a class, a tuple of classes, or a union");
}
var issubclass(const var& c, const var& cls) {
    if (!c.is_ref() || c.ref()->type->layout != Layout::Type) raise(E_TypeError, "issubclass() arg 1 must be a class");
    return issubclass_one(static_cast<TypeObj*>(c.ref()), cls);
}
var type(const var& x) { return tv(type_of(x)); }

// ------------------------------------------------------------------------------------------ statements
var class_name(const var& ns, const var& key, const var& fallback) {
    if (var* v = static_cast<DictObj*>(ns.ref())->get(key)) return *v;
    return fallback;
}
var with_exit_of(const var& mgr) {
    TypeObj* t = type_of(mgr);
    const var* ex = t->lookup("__exit__");
    if (!ex || !t->lookup("__enter__"))
        raise(E_TypeError, "'" + t->name + "' object does not support the context manager protocol");
    return bind(*ex, mgr, t);
}
var with_enter(const var& mgr) {
    TypeObj* t = type_of(mgr);
    return call0(bind(*t->lookup("__enter__"), mgr, t));
}
bool with_exit_exc(const var& exit_fn, const error& e) {
    CallArgs a;
    a.pos = {tv(type_of(e.value)), e.value, None};
    return call_obj(exit_fn, a).truthy();
}

// ------------------------------------------------------------------------------------------ wrappers
var classmethod(const var& f) {
    auto* w = new WrapObj();
    w->type = T_classmethod;
    w->f = f;
    return var(w);
}
var staticmethod(const var& f) {
    auto* w = new WrapObj();
    w->type = T_staticmethod;
    w->f = f;
    return var(w);
}
var property(const var& fget, const var& fset, const var& fdel, const var& doc) {
    auto* p = new PropertyObj();
    p->type = T_property;
    p->fget = fget;
    p->fset = fset;
    p->fdel = fdel;
    p->doc = doc;
    if (fget.is_ref() && fget.ref()->type == T_function) {
        p->name = static_cast<FuncObj*>(fget.ref())->name;
        if (doc.is_none()) p->doc = static_cast<FuncObj*>(fget.ref())->doc;
    }
    return var(p);
}

}  // namespace py
