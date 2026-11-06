// builtins.cc -- the builtin types (made here, before anything else), the constructors of int, float,
// bool, object and type, and the builtins module: print, sorted, min/max, sum, any/all, enumerate,
// zip, map, filter, reversed, divmod, round, chr/ord, hex/oct/bin, int(s, base) with Python's
// parsing, iter/next, getattr family, vars/dir; and the runtime's start.
#include <windows.h>
#include <shellapi.h>

#include <algorithm>
#include <cmath>
#include <cstring>

#include "rt.hpp"

namespace py {

namespace t {
var object, type, NoneType, bool_, int_, float_, complex, str, bytes, bytearray, list, tuple, dict, set, frozenset, range,
    slice, function, builtin_function, method, module, generator, property, staticmethod, classmethod, super, memoryview,
    enumerate, zip, map, filter, reversed, cell, NotImplementedType, ellipsis;
var BaseException, Exception, SystemExit, KeyboardInterrupt, GeneratorExit, StopIteration, ArithmeticError,
    ZeroDivisionError, OverflowError, FloatingPointError, AssertionError, AttributeError, BufferError, EOFError,
    ImportError, ModuleNotFoundError, LookupError, IndexError, KeyError, MemoryError, NameError, UnboundLocalError,
    OSError, FileNotFoundError, FileExistsError, PermissionError, IsADirectoryError, NotADirectoryError, TimeoutError,
    InterruptedError, ChildProcessError, ConnectionError, BrokenPipeError, ProcessLookupError, BlockingIOError,
    ReferenceError, RuntimeError, NotImplementedError, RecursionError, SyntaxError, SystemError, TypeError, ValueError,
    UnicodeError, UnicodeDecodeError, UnicodeEncodeError, Warning, UserWarning, DeprecationWarning, RuntimeWarning,
    StopAsyncIteration;
}  // namespace t

var g_builtins;   // the builtins module
var sorted_(const CallArgs& a);
void init_ops();
void init_io();
var type_new_(TypeObj* meta, const CallArgs& a);
extern int g_recursion_limit;

// ------------------------------------------------------------------------------------------ bootstrap
static std::vector<TypeObj*>* g_boot = nullptr;
static TypeObj* boot(const char* name, TypeObj* base, Layout l) {
    TypeObj* t = new_type(name, base ? std::vector<TypeObj*>{base} : std::vector<TypeObj*>{}, l);
    g_boot->push_back(t);
    return t;
}
void init_types() {
    g_boot = new std::vector<TypeObj*>();
    T_object = boot("object", nullptr, Layout::Object);
    T_type = boot("type", T_object, Layout::Type);
    T_dict = boot("dict", T_object, Layout::Dict);
    T_str = boot("str", T_object, Layout::Str);
    for (TypeObj* t : *g_boot) {
        t->type = T_type;
        t->dict->type = T_dict;
        t->mro.clear();
        t->mro.push_back(t);
        if (t != T_object) t->mro.push_back(T_object);
        if (t != T_object && t->bases.empty()) t->bases = {T_object};
    }
    T_none = new_type("NoneType", {}, Layout::Object);
    T_none->abstract_instances = true;
    T_int = new_type("int", {}, Layout::Int);
    T_bool = new_type("bool", {T_int}, Layout::Int);
    T_float = new_type("float", {}, Layout::Float);
    T_complex = new_type("complex", {}, Layout::Complex);
    T_bytes = new_type("bytes", {}, Layout::Bytes);
    T_bytearray = new_type("bytearray", {}, Layout::ByteArray);
    T_list = new_type("list", {}, Layout::List);
    T_tuple = new_type("tuple", {}, Layout::Tuple);
    T_set = new_type("set", {}, Layout::Set);
    T_frozenset = new_type("frozenset", {}, Layout::FrozenSet);
    T_range = new_type("range", {}, Layout::Range);
    T_slice = new_type("slice", {}, Layout::Slice);
    T_function = new_type("function", {}, Layout::Function);
    T_native = new_type("builtin_function_or_method", {}, Layout::Native);
    T_method = new_type("method", {}, Layout::Method);
    T_module = new_type("module", {}, Layout::Module);
    T_cell = new_type("cell", {}, Layout::Cell);
    T_generator = new_type("generator", {}, Layout::Generator);
    T_iter = new_type("iterator", {}, Layout::Iter);
    T_property = new_type("property", {}, Layout::Property);
    T_staticmethod = new_type("staticmethod", {}, Layout::StaticMethod);
    T_classmethod = new_type("classmethod", {}, Layout::ClassMethod);
    T_super = new_type("super", {}, Layout::Super);
    T_notimpl = new_type("NotImplementedType", {}, Layout::Object);
    T_ellipsis = new_type("ellipsis", {}, Layout::Object);
    T_data = new_type("native_data", {}, Layout::NativeData);
    for (TypeObj* t : {T_int, T_float, T_bytes, T_bytearray, T_list, T_tuple, T_set, T_frozenset, T_range, T_slice, T_function, T_native,
                       T_method, T_module, T_cell, T_generator, T_iter, T_property, T_staticmethod, T_classmethod, T_super,
                       T_notimpl, T_ellipsis, T_data, T_none, T_complex})
        (void)t;
    T_type->layout = Layout::Type;
}

// ------------------------------------------------------------------------------------------ int / float / bool
std::string str_repr_ext_(const std::string& s);
var int_from(const std::string& text_in, int base) {
    std::string s;
    // strip whitespace, underscores between digits
    size_t a = 0, b = text_in.size();
    while (a < b && isspace((unsigned char)text_in[a])) a++;
    while (b > a && isspace((unsigned char)text_in[b - 1])) b--;
    std::string t = text_in.substr(a, b - a);
    auto bad = [&]() -> var {
        raise(E_ValueError, "invalid literal for int() with base " + std::to_string(base) + ": " + str_repr_ext_(text_in));
    };
    (void)bad;
    bool neg = false;
    size_t i = 0;
    if (i < t.size() && (t[i] == '+' || t[i] == '-')) { neg = t[i] == '-'; i++; }
    int bs = base;
    if (bs == 0 || bs == 16 || bs == 8 || bs == 2) {
        if (i + 1 < t.size() && t[i] == '0') {
            char p = (char)tolower((unsigned char)t[i + 1]);
            int pb = p == 'x' ? 16 : p == 'o' ? 8 : p == 'b' ? 2 : 0;
            if (pb && (bs == 0 || bs == pb)) { bs = pb; i += 2; if (i < t.size() && t[i] == '_') i++; }
        }
        if (bs == 0) {
            bs = 10;
            // base 0: no leading zeros for decimal (other than 0 itself)
            size_t j = i;
            bool allzero = true;
            for (; j < t.size(); j++) if (t[j] != '0' && t[j] != '_') allzero = false;
            if (t.size() - i > 1 && t[i] == '0' && !allzero) bad();
        }
    }
    if (bs < 2 || bs > 36) raise(E_ValueError, "int() base must be >= 2 and <= 36, or 0");
    if (i >= t.size()) bad();
    Big v(0);
    int64_t small = 0;
    bool use_small = true;
    char prev = 0;
    for (; i < t.size(); i++) {
        char c = t[i];
        if (c == '_') {
            if (prev == '_' || prev == 0 || i + 1 >= t.size()) bad();
            prev = c;
            continue;
        }
        int d;
        if (c >= '0' && c <= '9') d = c - '0';
        else if (c >= 'a' && c <= 'z') d = c - 'a' + 10;
        else if (c >= 'A' && c <= 'Z') d = c - 'A' + 10;
        else bad();
        if (d >= bs) bad();
        prev = c;
        if (use_small) {
            if (small > (INT64_MAX - d) / bs) { use_small = false; v = Big(small); }
            else { small = small * bs + d; continue; }
        }
        v = v * Big(bs) + Big(d);
    }
    if (use_small) return var(neg ? -small : small);
    return new_int(neg ? -v : v);
}
std::string str_repr_(const std::string& s);
std::string str_repr_ext_(const std::string& s) { return str_repr_(s); }

static double parse_float(const std::string& in) {
    std::string s;
    size_t a = 0, b = in.size();
    while (a < b && isspace((unsigned char)in[a])) a++;
    while (b > a && isspace((unsigned char)in[b - 1])) b--;
    s = in.substr(a, b - a);
    std::string low = s;
    for (auto& c : low) c = (char)tolower((unsigned char)c);
    std::string body = low;
    bool neg = false;
    if (!body.empty() && (body[0] == '+' || body[0] == '-')) { neg = body[0] == '-'; body = body.substr(1); }
    if (body == "inf" || body == "infinity") return neg ? -INFINITY : INFINITY;
    if (body == "nan") return NAN;
    std::string clean;
    for (char c : s) if (c != '_') clean.push_back(c);
    char* end = nullptr;
    double d = std::strtod(clean.c_str(), &end);
    if (clean.empty() || *end || clean.find_first_of("xXpP") != std::string::npos)
        raise(E_ValueError, "could not convert string to float: " + str_repr_(in));
    return d;
}

static var int_new(const CallArgs& a) {
    TypeObj* cls = T(a.pos.at(0));
    var r;
    if (a.pos.size() == 1 && a.kw.empty()) r = 0;
    else {
        var x = arg(a, 1, "x", 0);
        var base = arg(a, 2, "base", None);
        if (!base.is_none()) {
            if (is_str(x)) r = int_from(str_utf8(x), (int)base.as_int64());
            else if (is_bytes(x)) r = int_from(raw_of(x), (int)base.as_int64());
            else raise(E_TypeError, "int() can't convert non-string with explicit base");
        } else if (is_int(x)) r = x.is_bool() ? var(x.small()) : x;
        else if (x.is_float()) {
            double d = x.flt();
            if (std::isnan(d)) raise(E_ValueError, "cannot convert float NaN to integer");
            if (std::isinf(d)) raise(E_OverflowError, "cannot convert float infinity to integer");
            double tr = std::trunc(d);
            if (std::fabs(tr) < 9.2e18) r = (int64_t)tr;
            else {
                char buf[400];
                snprintf(buf, sizeof buf, "%.0f", tr);
                r = new_int(Big::from_decimal(buf));
            }
        } else if (is_str(x)) r = int_from(str_utf8(x), 10);
        else if (is_bytes(x)) r = int_from(raw_of(x), 10);
        else {
            var m;
            if (lookup_special(x, "__int__", m) || lookup_special(x, "__index__", m)) r = call0(m);
            else if (lookup_special(x, "__trunc__", m)) r = call0(m);
            else raise(E_TypeError, "int() argument must be a string, a bytes-like object or a real number, not '" + tname(x) + "'");
        }
    }
    if (cls == T_int) return r;
    auto* o = new BigObj();
    o->type = cls;
    o->v = to_big(r);
    return var(static_cast<Obj*>(o));
}
static var float_new(const CallArgs& a) {
    TypeObj* cls = T(a.pos.at(0));
    double d = 0;
    if (a.pos.size() > 1) {
        const var& x = a.pos[1];
        if (is_str(x)) d = parse_float(str_utf8(x));
        else if (is_bytes(x)) d = parse_float(raw_of(x));
        else {
            var m;
            if (!x.is_float() && !is_int(x) && lookup_special(x, "__float__", m)) d = call0(m).as_double();
            else d = x.as_double();
        }
    }
    if (cls == T_float) return d;
    auto* o = new FloatObj();
    o->type = cls;
    o->v = d;
    return var(static_cast<Obj*>(o));
}

static var object_new(const CallArgs& a) {
    TypeObj* cls = T(a.pos.at(0));
    switch (cls->layout) {
    case Layout::Object: {
        auto* o = new Obj();
        o->type = cls;
        return var(o);
    }
    case Layout::Exception: {
        auto* e = new ExcObj();
        e->type = cls;
        e->args = new_tuple();
        return var(static_cast<Obj*>(e));
    }
    default: {
        // the builtin base's own __new__ with no arguments
        for (TypeObj* b : cls->mro)
            if (b->builtin && b != T_object)
                if (var* n = b->dict->get("__new__")) {
                    var f = n->is_ref() && n->ref()->type == T_staticmethod ? static_cast<WrapObj*>(n->ref())->f : *n;
                    return call1(f, a.pos[0]);
                }
        raise(E_TypeError, "object.__new__(" + cls->name + ") is not safe");
    }
    }
}

// ------------------------------------------------------------------------------------------ numbers' methods
static void init_numbers() {
    add_static(T_int, "__new__", int_new);
    add_static(T_float, "__new__", float_new);
    add_static(T_bool, "__new__", [](const CallArgs& a) { return var(a.pos.size() > 1 && a.pos[1].truthy()); });
    add_method(T_int, "bit_length", [](const CallArgs& a) { return var((int64_t)to_big(a.pos[0]).bit_length()); });
    add_method(T_int, "bit_count", [](const CallArgs& a) {
        Big b = to_big(a.pos[0]).abs();
        int64_t n = 0;
        while (b.sign() > 0) { if (floormod(b, Big(2)) == Big(1)) n++; b = shr(b, 1); }
        return var(n);
    });
    add_method(T_int, "__index__", [](const CallArgs& a) { return a.pos[0].is_bool() ? var(a.pos[0].small()) : a.pos[0]; });
    add_method(T_int, "__int__", [](const CallArgs& a) { return a.pos[0].is_bool() ? var(a.pos[0].small()) : a.pos[0]; });
    add_method(T_int, "__float__", [](const CallArgs& a) { return var(a.pos[0].as_double()); });
    add_method(T_int, "__hash__", [](const CallArgs& a) { return var(hash_(a.pos[0])); });
    add_method(T_int, "__repr__", [](const CallArgs& a) { return new_str(to_big(a.pos[0]).str()); });
    add_method(T_int, "__format__", [](const CallArgs& a) { return new_str(format_spec(new_int(to_big(a.pos[0])), str_utf8(a.pos.at(1)))); });
    add_method(T_int, "conjugate", [](const CallArgs& a) { return a.pos[0]; });
    add_getter(T_int, "real", [](const var& x) { return new_int(to_big(x)); });
    add_getter(T_int, "imag", [](const var&) { return var(0); });
    add_getter(T_int, "numerator", [](const var& x) { return new_int(to_big(x)); });
    add_getter(T_int, "denominator", [](const var&) { return var(1); });
    add_method(T_int, "to_bytes", [](const CallArgs& a) {
        Big v = to_big(a.pos[0]);
        int64_t length = arg(a, 1, "length", 1).as_int64();
        std::string order = str_utf8(arg(a, 2, "byteorder", "big"_s));
        bool sig = kwarg(a, "signed", false).truthy();
        if (v.sign() < 0 && !sig) raise(E_OverflowError, "can't convert negative int to unsigned");
        Big m = v;
        if (v.sign() < 0) m = v + shl(Big(1), (int)(length * 8));
        std::string out((size_t)length, '\0');
        Big x = m;
        for (int64_t i = 0; i < length; i++) {
            out[(size_t)i] = (char)floormod(x, Big(256)).i64();
            x = shr(x, 8);
        }
        bool overflow = x.sign() != 0;
        if (sig) {
            Big lim = shl(Big(1), (int)(length * 8 - 1));
            overflow = v >= lim || v < -lim;
        }
        if (overflow) raise(E_OverflowError, "int too big to convert");
        if (order == "big") std::reverse(out.begin(), out.end());
        return new_bytes(out);
    });
    add_classmethod(T_int, "from_bytes", [](const CallArgs& a) {
        std::string b;
        var src = arg(a, 1, "bytes");
        if (is_bytes(src)) b = raw_of(src);
        else for (var x : each(src)) b.push_back((char)x.as_int64());
        std::string order = str_utf8(arg(a, 2, "byteorder", "big"_s));
        bool sig = kwarg(a, "signed", false).truthy();
        if (order == "little") std::reverse(b.begin(), b.end());
        Big v(0);
        if (b.size() <= 7) {
            int64_t s = 0;
            for (unsigned char c : b) s = s * 256 + c;
            v = Big(s);
        } else {
            for (unsigned char c : b) v = shl(v, 8) + Big((int64_t)c);
        }
        if (sig && !b.empty() && ((unsigned char)b[0] & 0x80)) v = v - shl(Big(1), (int)(b.size() * 8));
        var r = new_int(v);
        TypeObj* cls = T(a.pos[0]);
        if (cls != T_int) return call1(a.pos[0], r);
        return r;
    });
    add_method(T_float, "is_integer", [](const CallArgs& a) { double d = a.pos[0].as_double(); return var(std::isfinite(d) && d == std::floor(d)); });
    add_method(T_float, "__repr__", [](const CallArgs& a) { return new_str(repr_float(a.pos[0].as_double())); });
    add_method(T_float, "__int__", [](const CallArgs& a) { return int_new(pack_args_(tv(T_int), var(a.pos[0].as_double()))); });
    add_method(T_float, "__float__", [](const CallArgs& a) { return var(a.pos[0].as_double()); });
    add_method(T_float, "__format__", [](const CallArgs& a) { return new_str(format_spec(var(a.pos[0].as_double()), str_utf8(a.pos.at(1)))); });
    add_method(T_float, "hex", [](const CallArgs& a) {
        char b[64];
        snprintf(b, sizeof b, "%a", a.pos[0].as_double());
        return new_str(b);
    });
    add_getter(T_float, "real", [](const var& x) { return var(x.as_double()); });
    add_getter(T_float, "imag", [](const var&) { return var(0.0); });
    add_method(T_float, "__round__", [](const CallArgs& a) { return round(var(a.pos[0].as_double()), a.pos.size() > 1 ? a.pos[1] : var(None)); });
    add_method(T_float, "__trunc__", [](const CallArgs& a) { return int_new(pack_args_(tv(T_int), var(a.pos[0].as_double()))); });
}

// ------------------------------------------------------------------------------------------ object and type
static var object_repr(const CallArgs& a) {
    const var& x = a.pos[0];
    TypeObj* t = type_of(x);
    char b[32];
    snprintf(b, sizeof b, "0x%016llx", (unsigned long long)(uintptr_t)x.ref());
    std::string mod = t->module == "builtins" ? "" : t->module + ".";
    return new_str("<" + mod + t->qualname + " object at " + b + ">");
}
static void init_object_type() {
    add_static(T_object, "__new__", object_new);
    add_method(T_object, "__init__", [](const CallArgs&) { return None; });
    add_method(T_object, "__repr__", object_repr);
    add_method(T_object, "__str__", [](const CallArgs& a) { return new_str(repr_(a.pos[0])); });
    add_method(T_object, "__eq__", [](const CallArgs& a) { return is(a.pos[0], a.pos.at(1)) ? var(true) : NotImplemented; });
    add_method(T_object, "__ne__", [](const CallArgs& a) { return is(a.pos[0], a.pos.at(1)) ? var(false) : NotImplemented; });
    add_method(T_object, "__hash__", [](const CallArgs& a) { return var(hash_(a.pos[0])); });
    add_method(T_object, "__format__", [](const CallArgs& a) {
        if (!str_utf8(a.pos.at(1)).empty()) raise(E_TypeError, "unsupported format string passed to " + tname(a.pos[0]) + ".__format__");
        return new_str(str_(a.pos[0]));
    });
    add_method(T_object, "__setattr__", [](const CallArgs& a) {
        // object.__setattr__(self, name, value): the generic store, past any override
        var self = a.pos.at(0);
        Obj* p = self.ref();
        if (!p->attrs) { var d = new_dict_obj(); p->attrs = d.ref(); incref(p->attrs); }
        static_cast<DictObj*>(p->attrs)->set(a.pos.at(1), a.pos.at(2));
        return None;
    });
    add_method(T_object, "__getattribute__", [](const CallArgs& a) { return a.pos[0].getattr(a.pos.at(1)); });
    add_method(T_object, "__delattr__", [](const CallArgs& a) {
        Obj* p = a.pos[0].ref();
        if (!p->attrs || !static_cast<DictObj*>(p->attrs)->del(a.pos.at(1))) raise(E_AttributeError, str_utf8(a.pos[1]));
        return None;
    });
    add_method(T_object, "__reduce_ex__", [](const CallArgs& a) { return new_tuple({tv(type_of(a.pos[0]))}); });
    add_method(T_object, "__dir__", [](const CallArgs& a) { return dir(a.pos[0]); });
    add_classmethod(T_object, "__init_subclass__", [](const CallArgs&) { return None; });
    add_method(T_object, "__sizeof__", [](const CallArgs&) { return var(32); });

    add_static(T_type, "__new__", [](const CallArgs& a) {
        TypeObj* meta = T(a.pos.at(0));
        CallArgs b;
        b.pos.assign(a.pos.begin() + 1, a.pos.end());
        b.kw = a.kw;
        if (b.pos.size() == 1) return tv(type_of(b.pos[0]));
        return type_new_(meta, b);
    });
    add_method(T_type, "__init__", [](const CallArgs&) { return None; });
    add_method(T_type, "mro", [](const CallArgs& a) {
        std::vector<var> v;
        for (TypeObj* b : T(a.pos[0])->mro) v.push_back(tv(b));
        return new_list(v);
    });
    add_method(T_type, "__subclasses__", [](const CallArgs&) { return new_list(); });
    add_method(T_type, "__instancecheck__", [](const CallArgs& a) { return var(isinstance_(a.pos.at(1), T(a.pos[0]))); });
    add_method(T_type, "__subclasscheck__", [](const CallArgs& a) { return var(is_subtype(T(a.pos.at(1)), T(a.pos[0]))); });
    add_method(T_type, "__repr__", [](const CallArgs& a) { return new_str(repr_(a.pos[0])); });
    add_method(T_type, "__or__", [](const CallArgs& a) { return new_tuple({a.pos[0], a.pos.at(1)}); });

    add_static(T_property, "__new__", [](const CallArgs& a) {
        return property(arg(a, 1, "fget", None), arg(a, 2, "fset", None), arg(a, 3, "fdel", None), arg(a, 4, "doc", None));
    });
    add_method(T_property, "setter", [](const CallArgs& a) {
        auto* p = static_cast<PropertyObj*>(a.pos[0].ref());
        var r = property(p->fget, a.pos.at(1), p->fdel, p->doc);
        static_cast<PropertyObj*>(r.ref())->name = p->name;
        return r;
    });
    add_method(T_property, "deleter", [](const CallArgs& a) {
        auto* p = static_cast<PropertyObj*>(a.pos[0].ref());
        return property(p->fget, p->fset, a.pos.at(1), p->doc);
    });
    add_method(T_property, "getter", [](const CallArgs& a) {
        auto* p = static_cast<PropertyObj*>(a.pos[0].ref());
        return property(a.pos.at(1), p->fset, p->fdel, p->doc);
    });
    add_method(T_property, "__get__", [](const CallArgs& a) {
        if (a.pos.at(1).is_none()) return a.pos[0];
        return bind(a.pos[0], a.pos[1], type_of(a.pos[1]));
    });
    add_static(T_staticmethod, "__new__", [](const CallArgs& a) { return staticmethod(a.pos.at(1)); });
    add_static(T_classmethod, "__new__", [](const CallArgs& a) { return classmethod(a.pos.at(1)); });
    add_static(T_super, "__new__", [](const CallArgs& a) { return super_(a.pos.at(1), a.pos.at(2)); });
    add_method(T_function, "__get__", [](const CallArgs& a) {
        if (a.pos.at(1).is_none()) return a.pos[0];
        return bind(a.pos[0], a.pos[1], type_of(a.pos[1]));
    });
    add_method(T_function, "__call__", [](const CallArgs& a) {
        CallArgs b;
        b.pos.assign(a.pos.begin() + 1, a.pos.end());
        b.kw = a.kw;
        return call_obj(a.pos[0], b);
    });
    add_static(T_none, "__new__", [](const CallArgs&) { return var(None); });
    add_method(T_none, "__repr__", [](const CallArgs&) { return "None"_s; });
}

// ------------------------------------------------------------------------------------------ the builtin functions
static var min_max(const CallArgs& a, bool is_max) {
    var key = kwarg(a, "key", None);
    var dflt = kwarg(a, "default");
    std::vector<var> items = a.pos.size() == 1 ? collect(a.pos[0]) : a.pos;
    const char* name = is_max ? "max" : "min";
    if (items.empty()) {
        if (!dflt.is_unbound()) return dflt;
        if (a.pos.empty()) raise(E_TypeError, std::string(name) + " expected at least 1 argument, got 0");
        raise(E_ValueError, std::string(name) + "() iterable argument is empty");
    }
    var best = items[0];
    var bk = key.is_none() ? best : call1(key, best);
    for (size_t i = 1; i < items.size(); i++) {
        var k = key.is_none() ? items[i] : call1(key, items[i]);
        if (is_max ? (bk < k) : (k < bk)) { best = items[i]; bk = k; }
    }
    return best;
}
var round(const var& x, const var& nd) {
    if (is_int(x)) {
        if (nd.is_none() || nd.is_unbound()) return x.is_bool() ? var(x.small()) : x;
        int64_t n = nd.as_int64();
        if (n >= 0) return x;
        Big p = ipow(Big(10), (int)-n), v = to_big(x);
        Big q = floordiv(v, p), r = v - q * p;
        Big twice = r * Big(2);
        if (twice > p || (twice == p && floormod(q, Big(2)) == Big(1))) q = q + Big(1);
        return new_int(q * p);
    }
    if (!x.is_float() && !has_layout(x, Layout::Float)) {
        var m;
        if (lookup_special(x, "__round__", m)) return nd.is_none() || nd.is_unbound() ? call0(m) : call1(m, nd);
        raise(E_TypeError, "type " + tname(x) + " doesn't define __round__ method");
    }
    double d = x.as_double();
    if (nd.is_none() || nd.is_unbound()) {
        if (std::isnan(d)) raise(E_ValueError, "cannot convert float NaN to integer");
        if (std::isinf(d)) raise(E_OverflowError, "cannot convert float infinity to integer");
        double r = std::nearbyint(d);   // (round half to even: the default FP mode)
        return int_new(pack_args_(tv(T_int), var(r)));
    }
    int64_t n = nd.as_int64();
    if (!std::isfinite(d) || d == 0) return d;
    // correctly rounded: via the shortest repr's decimal digits, as CPython (round half even)
    char buf[64];
    snprintf(buf, sizeof buf, "%.*f", (int)std::max<int64_t>(0, std::min<int64_t>(n, 300)), d);
    if (n < 0) {
        double p = std::pow(10.0, (double)-n);
        double r = std::nearbyint(d / p) * p;
        return r;
    }
    return std::strtod(buf, nullptr);
}
var divmod(const var& a, const var& b) { return new_tuple({floordiv(a, b), a % b}); }
var abs(const var& x) {
    if (x.is_small_int() || x.is_bool()) {
        int64_t v = x.small();
        if (v == INT64_MIN) return new_int(-Big(v));
        return v < 0 ? -v : v;
    }
    if (x.is_float()) return std::fabs(x.flt());
    if (is_big(x)) return new_int(to_big(x).abs());
    var m;
    if (lookup_special(x, "__abs__", m)) return call0(m);
    raise(E_TypeError, "bad operand type for abs(): '" + tname(x) + "'");
}
var chr(const var& i) {
    int64_t c = i.as_int64();
    if (c < 0 || c > 0x10FFFF) raise(E_ValueError, "chr() arg not in range(0x110000)");
    std::string o;
    append_utf8(o, (char32_t)c);
    return new_str(o);
}
var ord(const var& c) {
    if (is_str(c)) {
        auto u = to_u32(str_utf8(c));
        if (u.size() != 1) raise(E_TypeError, "ord() expected a character, but string of length " + std::to_string(u.size()) + " found");
        return var((int64_t)u[0]);
    }
    if (is_bytes(c)) {
        const std::string& s = raw_of(c);
        if (s.size() != 1) raise(E_TypeError, "ord() expected a character, but string of length " + std::to_string(s.size()) + " found");
        return var((int64_t)(unsigned char)s[0]);
    }
    raise(E_TypeError, "ord() expected string of length 1, but " + tname(c) + " found");
}
static var base_str(const var& i, int base, const char* prefix) {
    var v = i;
    if (!is_int(v)) {
        var m;
        if (lookup_special(v, "__index__", m)) v = call0(m);
        else raise(E_TypeError, "'" + tname(i) + "' object cannot be interpreted as an integer");
    }
    Big b = to_big(v);
    return new_str((b.sign() < 0 ? "-" : "") + std::string(prefix) + int_to_base(b, base));
}
var hex(const var& i) { return base_str(i, 16, "0x"); }
var oct(const var& i) { return base_str(i, 8, "0o"); }
var bin(const var& i) { return base_str(i, 2, "0b"); }
var callable(const var& x) {
    if (!x.is_ref()) return false;
    TypeObj* t = x.ref()->type;
    if (t == T_function || t == T_native || t == T_method || t->layout == Layout::Type) return true;
    return t->lookup("__call__") != nullptr;
}
var vars(const var& x) { return x.getattr("__dict__"); }
var dir(const var& x) {
    var s = new_set();
    if (x.is_ref() && x.ref()->attrs)
        for (auto& e : static_cast<DictObj*>(x.ref()->attrs)->entries)
            if (!e.key.is_unbound()) s.call("add", e.key);
    if (x.is_ref() && x.ref()->type == T_module) {
        for (auto& [k, c] : static_cast<ModuleObj*>(x.ref())->cells)
            if (!c->v.is_unbound()) s.call("add", new_str(k));
    } else {
        TypeObj* t = x.is_ref() && x.ref()->type->layout == Layout::Type ? static_cast<TypeObj*>(x.ref()) : type_of(x);
        for (TypeObj* b : t->mro)
            for (auto& e : b->dict->entries)
                if (!e.key.is_unbound()) s.call("add", e.key);
    }
    std::vector<var> v = collect(s);
    CallArgs a;
    a.add(new_list(v));
    return sorted_(a);
}
var exec_unsupported(const char* what) {
    raise(E_NotImplementedError, std::string(what) + " of a runtime string cannot run in translated code");
}

void flush_std_();
var print_(const CallArgs& a) {
    var sep = kwarg(a, "sep", None), end = kwarg(a, "end", None), file = kwarg(a, "file", None), flush = kwarg(a, "flush", false);
    std::string s;
    std::string sp = sep.is_none() ? " " : str_utf8(sep);
    for (size_t i = 0; i < a.pos.size(); i++) {
        if (i) s += sp;
        s += str_(a.pos[i]);
    }
    s += end.is_none() ? "\n" : str_utf8(end);
    if (file.is_none()) {
        extern var g_sys;
        file = g_sys.getattr("stdout");
    }
    if (file.is_none()) return None;
    file.call("write", new_str(s));
    if (flush.truthy()) file.call("flush");
    return None;
}

var builtin(const char* name) { return g_builtins.getattr(name); }

static void def(const char* name, native_fn fn) { mod_def(g_builtins, name, std::move(fn)); }
static void put(const char* name, const var& v) { mod_set(g_builtins, name, v); }

void init_builtins() {
    init_numbers();
    init_object_type();
    auto* m = new ModuleObj();
    m->type = T_module;
    m->name = "builtins";
    g_builtins = var(static_cast<Obj*>(m));
    // the types
    t::object = tv(T_object); t::type = tv(T_type); t::NoneType = tv(T_none); t::bool_ = tv(T_bool); t::int_ = tv(T_int);
    t::float_ = tv(T_float); t::complex = tv(T_complex); t::str = tv(T_str); t::bytes = tv(T_bytes); t::bytearray = tv(T_bytearray);
    t::list = tv(T_list); t::tuple = tv(T_tuple); t::dict = tv(T_dict); t::set = tv(T_set); t::frozenset = tv(T_frozenset);
    t::range = tv(T_range); t::slice = tv(T_slice); t::function = tv(T_function); t::builtin_function = tv(T_native);
    t::method = tv(T_method); t::module = tv(T_module); t::generator = tv(T_generator); t::property = tv(T_property);
    t::staticmethod = tv(T_staticmethod); t::classmethod = tv(T_classmethod); t::super = tv(T_super); t::cell = tv(T_cell);
    t::NotImplementedType = tv(T_notimpl); t::ellipsis = tv(T_ellipsis);
    for (auto& [n, v] : std::vector<std::pair<const char*, var>>{
             {"object", t::object}, {"type", t::type}, {"bool", t::bool_}, {"int", t::int_}, {"float", t::float_}, {"complex", t::complex},
             {"str", t::str}, {"bytes", t::bytes}, {"bytearray", t::bytearray}, {"list", t::list}, {"tuple", t::tuple}, {"dict", t::dict},
             {"set", t::set}, {"frozenset", t::frozenset}, {"range", t::range}, {"slice", t::slice}, {"property", t::property},
             {"staticmethod", t::staticmethod}, {"classmethod", t::classmethod}, {"super", t::super}})
        put(n, v);
    for (TypeObj* e : {E_BaseException, E_Exception, E_TypeError, E_ValueError, E_KeyError, E_IndexError, E_AttributeError,
                       E_StopIteration, E_ZeroDivisionError, E_OverflowError, E_NameError, E_UnboundLocalError, E_RuntimeError,
                       E_NotImplementedError, E_AssertionError, E_OSError, E_FileNotFoundError, E_FileExistsError,
                       E_PermissionError, E_IsADirectoryError, E_NotADirectoryError, E_ImportError, E_ModuleNotFoundError,
                       E_UnicodeDecodeError, E_UnicodeEncodeError, E_LookupError, E_GeneratorExit, E_SystemExit,
                       E_RecursionError, E_MemoryError, E_TimeoutError, E_EOFError, E_KeyboardInterrupt, E_UnicodeError, E_SyntaxError})
        put(e->name.c_str(), tv(e));
    for (auto* v : {&t::ArithmeticError, &t::FloatingPointError, &t::BufferError, &t::InterruptedError, &t::ChildProcessError,
                    &t::ConnectionError, &t::BrokenPipeError, &t::ProcessLookupError, &t::BlockingIOError, &t::ReferenceError,
                    &t::SystemError, &t::Warning, &t::UserWarning, &t::DeprecationWarning, &t::RuntimeWarning, &t::StopAsyncIteration})
        put(T(*v)->name.c_str(), *v);
    put("EnvironmentError", t::OSError);
    put("IOError", t::OSError);
    put("WindowsError", t::OSError);
    put("NotImplemented", NotImplemented);
    put("Ellipsis", Ellipsis);
    put("__debug__", var(true));
    put("None", None);
    put("True", true);
    put("False", false);
    def("len", [](const CallArgs& a) { check_args(a, "len", 1, 1); return len(a.pos[0]); });
    def("repr", [](const CallArgs& a) { check_args(a, "repr", 1, 1); return repr(a.pos[0]); });
    def("ascii", [](const CallArgs& a) { return ascii(a.pos.at(0)); });
    def("hash", [](const CallArgs& a) { return hash(a.pos.at(0)); });
    def("id", [](const CallArgs& a) { return id(a.pos.at(0)); });
    def("abs", [](const CallArgs& a) { return abs(a.pos.at(0)); });
    def("divmod", [](const CallArgs& a) { return divmod(a.pos.at(0), a.pos.at(1)); });
    def("round", [](const CallArgs& a) { return round(a.pos.at(0), arg(a, 1, "ndigits", None)); });
    def("pow", [](const CallArgs& a) { return a.pos.size() > 2 ? pow(a.pos[0], a.pos[1], a.pos[2]) : pow(a.pos.at(0), a.pos.at(1)); });
    def("chr", [](const CallArgs& a) { return chr(a.pos.at(0)); });
    def("ord", [](const CallArgs& a) { return ord(a.pos.at(0)); });
    def("hex", [](const CallArgs& a) { return hex(a.pos.at(0)); });
    def("oct", [](const CallArgs& a) { return oct(a.pos.at(0)); });
    def("bin", [](const CallArgs& a) { return bin(a.pos.at(0)); });
    def("format", [](const CallArgs& a) { return format(a.pos.at(0), a.pos.size() > 1 ? a.pos[1] : var(None)); });
    def("isinstance", [](const CallArgs& a) { check_args(a, "isinstance", 2, 2); return isinstance(a.pos[0], a.pos[1]); });
    def("issubclass", [](const CallArgs& a) { return issubclass(a.pos.at(0), a.pos.at(1)); });
    def("callable", [](const CallArgs& a) { return callable(a.pos.at(0)); });
    def("getattr", [](const CallArgs& a) {
        if (a.pos.size() == 3) return getattr(a.pos[0], a.pos[1], a.pos[2]);
        check_args(a, "getattr", 2, 3);
        return getattr(a.pos[0], a.pos[1]);
    });
    def("setattr", [](const CallArgs& a) { setattr(a.pos.at(0), a.pos.at(1), a.pos.at(2)); return None; });
    def("hasattr", [](const CallArgs& a) { return hasattr(a.pos.at(0), a.pos.at(1)); });
    def("delattr", [](const CallArgs& a) { delattr(a.pos.at(0), a.pos.at(1)); return None; });
    def("vars", [](const CallArgs& a) { return vars(a.pos.at(0)); });
    def("dir", [](const CallArgs& a) { return dir(a.pos.at(0)); });
    def("iter", [](const CallArgs& a) {
        if (a.pos.size() == 2) {
            var f = a.pos[0], sentinel = a.pos[1];
            return new_iter([f, sentinel](var& out) {
                out = call0(f);
                return !(out == sentinel);
            });
        }
        return iter(a.pos.at(0));
    });
    def("next", [](const CallArgs& a) { return a.pos.size() > 1 ? next(a.pos[0], a.pos[1]) : next(a.pos.at(0)); });
    def("print", print_);
    def("sorted", sorted_);
    def("min", [](const CallArgs& a) { return min_max(a, false); });
    def("max", [](const CallArgs& a) { return min_max(a, true); });
    def("sum", [](const CallArgs& a) {
        var total = arg(a, 1, "start", 0);
        if (is_str(total)) raise(E_TypeError, "sum() can't sum strings [use ''.join(seq) instead]");
        for (var x : each(a.pos.at(0))) total = total + x;
        return total;
    });
    def("any", [](const CallArgs& a) {
        for (var x : each(a.pos.at(0))) if (x) return var(true);
        return var(false);
    });
    def("all", [](const CallArgs& a) {
        for (var x : each(a.pos.at(0))) if (!x) return var(false);
        return var(true);
    });
    def("enumerate", [](const CallArgs& a) {
        var it = iter(arg(a, 0, "iterable"));
        auto n = std::make_shared<var>(arg(a, 1, "start", 0));
        return new_iter([it, n](var& out) {
            var x;
            if (!next_(it, x)) return false;
            out = new_tuple({*n, x});
            *n = *n + 1;
            return true;
        });
    });
    def("zip", [](const CallArgs& a) {
        std::vector<var> its;
        for (auto& x : a.pos) its.push_back(iter(x));
        bool strict = kwarg(a, "strict", false).truthy();
        return new_iter([its, strict](var& out) {
            if (its.empty()) return false;
            std::vector<var> row;
            for (size_t i = 0; i < its.size(); i++) {
                var x;
                if (!next_(its[i], x)) {
                    if (strict && i > 0) raise(E_ValueError, "zip() argument " + std::to_string(i + 1) + " is shorter than argument 1");
                    if (strict && i == 0) {
                        for (size_t j = 1; j < its.size(); j++) {
                            var y;
                            if (next_(its[j], y)) raise(E_ValueError, "zip() argument " + std::to_string(j + 1) + " is longer than argument 1");
                        }
                    }
                    return false;
                }
                row.push_back(x);
            }
            out = new_tuple(std::move(row));
            return true;
        });
    });
    def("map", [](const CallArgs& a) {
        var f = a.pos.at(0);
        std::vector<var> its;
        for (size_t i = 1; i < a.pos.size(); i++) its.push_back(iter(a.pos[i]));
        return new_iter([f, its](var& out) {
            CallArgs c;
            for (auto& it : its) {
                var x;
                if (!next_(it, x)) return false;
                c.pos.push_back(x);
            }
            out = call_obj(f, c);
            return true;
        });
    });
    def("filter", [](const CallArgs& a) {
        var f = a.pos.at(0), it = iter(a.pos.at(1));
        return new_iter([f, it](var& out) {
            var x;
            while (next_(it, x)) {
                if (f.is_none() ? x.truthy() : call1(f, x).truthy()) { out = x; return true; }
            }
            return false;
        });
    });
    def("reversed", [](const CallArgs& a) {
        var x = a.pos.at(0);
        var m;
        if (lookup_special(x, "__reversed__", m)) return call0(m);
        std::vector<var> v;
        int64_t n = len_(x);
        for (int64_t i = n - 1; i >= 0; i--) v.push_back(x.getitem(var(i)));
        return iter(new_list(v));
    });
    def("globals", [](const CallArgs&) -> var { raise(E_NotImplementedError, "globals() is the module's: translated as its dict"); });
    def("exec", [](const CallArgs&) { return exec_unsupported("exec"); });
    def("eval", [](const CallArgs&) { return exec_unsupported("eval"); });
    def("compile", [](const CallArgs&) { return exec_unsupported("compile"); });
    def("input", [](const CallArgs& a) {
        if (!a.pos.empty()) { fputs(str_(a.pos[0]).c_str(), stdout); fflush(stdout); }
        char buf[65536];
        if (!fgets(buf, sizeof buf, stdin)) raise(E_EOFError, "EOF when reading a line");
        std::string s = buf;
        while (!s.empty() && (s.back() == '\n' || s.back() == '\r')) s.pop_back();
        return new_str(s);
    });
    def("__import__", [](const CallArgs& a) {
        var fromlist = arg(a, 3, "fromlist", None);
        std::string n = str_utf8(a.pos.at(0));
        if (!fromlist.is_none() && fromlist.truthy()) return import(n.c_str());
        return import_top(n.c_str());
    });
    def("exit", [](const CallArgs& a) -> var { raise(make_exc_args(E_SystemExit, a.pos.empty() ? std::vector<var>{} : std::vector<var>{a.pos[0]})); });
    put("quit", g_builtins.getattr("exit"));
    put("memoryview", t::bytes);   // (memoryview(b) reads as the bytes it views: tobytes and slicing)
    mod_set(g_builtins, "__name__", "builtins"_s);
}

// ------------------------------------------------------------------------------------------ start
void init_io();
void init_modules();
static bool g_started = false;
void ensure_runtime_() {
    if (g_started) return;
    g_started = true;
    SetConsoleOutputCP(CP_UTF8);
    init_types();
    init_exceptions();
    init_ops();
    init_str();
    init_bytes();
    init_containers();
    init_dict();
    init_builtins();
    init_gen();
    init_io();
    init_modules();
}
std::vector<std::string> g_argv;
void init_runtime(int argc, char** argv) {
    ensure_runtime_();
    // argv as UTF-8 (the wide command line)
    int n = 0;
    LPWSTR* w = CommandLineToArgvW(GetCommandLineW(), &n);
    g_argv.clear();
    for (int i = 0; i < n; i++) {
        int len = WideCharToMultiByte(CP_UTF8, 0, w[i], -1, nullptr, 0, nullptr, nullptr);
        std::string s((size_t)(len > 0 ? len - 1 : 0), '\0');
        WideCharToMultiByte(CP_UTF8, 0, w[i], -1, s.data(), len, nullptr, nullptr);
        g_argv.push_back(s);
    }
    LocalFree(w);
    (void)argc;
    (void)argv;
    extern void set_sys_argv_();
    set_sys_argv_();
}

}  // namespace py
