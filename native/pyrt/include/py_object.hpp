// py_object.hpp -- the dynamic value of the porting engine's dynamic mode (native/pyrt).
//
// Where pyengine cannot prove a static type it writes C++ over py::var instead of refusing. A var is
// any Python value: None, bool, int (unbounded: an int64 inline, a big integer past it), float, str,
// bytes, bytearray, list, tuple, dict (insertion ordered), set, frozenset, range, slice, functions and
// closures, classes and their instances, modules, exceptions, generators and iterators. Its operators
// are Python's (/ is true division, % is Python's modulo and %-formatting, == compares values), and
// the runtime behind it (native/pyrt/src) is Python's object model: attribute lookup through the MRO,
// descriptors and properties, bound methods, *args / **kwargs binding, the exception hierarchy.
//
// The name is var, not object: py::object is already the base of the typed C++ classes (py.hpp), and
// that output stays byte for byte what it was.
//
//     py::var s = "text"_s;                    // literals are made once, wherever they are written
//     py::var n = py::len(s) + 1;
//     py::var parts = s.call("split", ","_s);  // a method call
//     py::var m = re.call("match", pat, s);    // a module's function (re is the module's var)
//     if (m) { ... }                           // truth is Python's
//     f(a, py::kw("sep", ""_s), py::star(xs)); // keyword, *args
//     for (py::var x : py::each(xs)) ...       // iteration
//
// Text is UTF-8 at the C++ boundary; a str counts code points.
#pragma once

#include <cstdint>
#include <cstring>
#include <exception>
#include <functional>
#include <initializer_list>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace py {

struct Obj;
struct TypeObj;
class var;

// ------------------------------------------------------------------------------------------ heap
// Every heap value: a reference count (the runtime holds a global lock while Python code runs, as
// CPython does, so counts are not atomic) and its type. Instances of classes also carry an attribute
// dict (`attrs`); builtin values subclassed by a class (class Foo(dict)) are that builtin's object
// with the class as its type.
//
// Counting references frees everything but cycles; the cycle collector (src/gc.cc, CPython's gc)
// finds those. The objects that can hold references (containers, instances, functions, cells ...)
// are tracked by it (`gc`: where it keeps them, and their finalized / weakly-referenced marks), and
// say what they hold (gc_traverse: each reference they own, once) and how to let go of it (gc_clear).
using gc_visit = void (*)(Obj* referent, void* arg);
struct Obj {
    int32_t rc = 0;
    uint32_t gc = 0;                // (the collector's: src/gc.cc)
    TypeObj* type = nullptr;
    Obj* attrs = nullptr;           // a DictObj, or null
    Obj() = default;
    Obj(const Obj&) = delete;
    Obj& operator=(const Obj&) = delete;
    virtual ~Obj();
    virtual void gc_traverse(gc_visit visit, void* arg);   // the references it owns (here: attrs)
    virtual void gc_clear();                               // let go of them (garbage in a cycle)
};
void dealloc_(Obj* o);
inline void incref(Obj* o) { if (o) ++o->rc; }
inline void decref(Obj* o) { if (o && --o->rc == 0) dealloc_(o); }

struct none_t {};
struct unbound_t {};
inline constexpr none_t None{};

// ------------------------------------------------------------------------------------------ var


struct CallArgs;
template <class... A> CallArgs pack_args_(A&&... a);

class var {
public:
    enum class Tag : uint8_t { Unbound, None, Bool, Int, Float, Ref };

    constexpr var() noexcept : tag_(Tag::Unbound), i_(0) {}   // (constant-initialised: safe at static init)
    constexpr var(unbound_t) noexcept : tag_(Tag::Unbound), i_(0) {}
    constexpr var(none_t) noexcept : tag_(Tag::None), i_(0) {}
    var(bool b) noexcept : tag_(Tag::Bool), i_(b ? 1 : 0) {}
    var(int v) noexcept : tag_(Tag::Int), i_(v) {}
    var(long v) noexcept : tag_(Tag::Int), i_(v) {}
    var(long long v) noexcept : tag_(Tag::Int), i_(v) {}
    var(unsigned v) noexcept : tag_(Tag::Int), i_(v) {}
    var(unsigned long v) noexcept : tag_(Tag::Int), i_(static_cast<int64_t>(v)) {}
    var(unsigned long long v);                       // past int64: a big int
    var(double v) noexcept : tag_(Tag::Float), f_(v) {}
    var(const char* utf8);                           // a str (not cached: prefer "..."_s)
    var(const std::string& utf8);                    // a str
    explicit var(Obj* o) noexcept : tag_(Tag::Ref), p_(o) { incref(o); }   // takes a new reference
    static var steal(Obj* o) noexcept { var v; v.tag_ = Tag::Ref; v.p_ = o; return v; }

    var(const var& o) noexcept : tag_(o.tag_), i_(o.i_) { if (tag_ == Tag::Ref) incref(p_); }
    var(var&& o) noexcept : tag_(o.tag_), i_(o.i_) { o.tag_ = Tag::Unbound; o.i_ = 0; }
    var& operator=(const var& o) noexcept {
        if (o.tag_ == Tag::Ref) incref(o.p_);
        Obj* old = tag_ == Tag::Ref ? p_ : nullptr;
        tag_ = o.tag_; i_ = o.i_;
        decref(old);
        return *this;
    }
    var& operator=(var&& o) noexcept {
        if (this != &o) {
            Obj* old = tag_ == Tag::Ref ? p_ : nullptr;
            tag_ = o.tag_; i_ = o.i_;
            o.tag_ = Tag::Unbound; o.i_ = 0;
            decref(old);
        }
        return *this;
    }
    ~var() { if (tag_ == Tag::Ref) decref(p_); }

    // what it is
    Tag tag() const { return tag_; }
    bool is_unbound() const { return tag_ == Tag::Unbound; }
    bool is_none() const { return tag_ == Tag::None; }
    bool is_bool() const { return tag_ == Tag::Bool; }
    bool is_small_int() const { return tag_ == Tag::Int; }
    bool is_float() const { return tag_ == Tag::Float; }
    bool is_ref() const { return tag_ == Tag::Ref; }
    Obj* ref() const { return tag_ == Tag::Ref ? p_ : nullptr; }
    int64_t small() const { return i_; }            // Int and Bool
    double flt() const { return f_; }
    TypeObj* type() const;                           // type(x)

    // Python's truth (bool(x)); `if (x)`, `x && y` in a condition
    explicit operator bool() const;
    bool truthy() const { return static_cast<bool>(*this); }

    // attributes: x.attr("name") reads (getattr), x.setattr("name", v) writes
    var attr(const char* name) const;
    var getattr(const char* name) const;
    var getattr(const var& name) const;
    void setattr(const char* name, const var& v) const;
    void delattr(const char* name) const;
    bool hasattr(const char* name) const;

    // items: x[k] reads, x[k] = v writes, py::del(x[k]) deletes
    var operator[](const var& key) const;          // x[k] reads; x.setitem(k, v) writes
    var getitem(const var& key) const;
    void setitem(const var& key, const var& v) const;
    void delitem(const var& key) const;
    var slice(const var& lo, const var& hi, const var& step = None) const;   // x[lo:hi:step]

    // calls: f(a, b, py::kw("k", v), py::star(xs), py::starstar(d))
    template <class... A> var operator()(A&&... a) const { return call_(pack_args_(std::forward<A>(a)...)); }
    var operator()(std::initializer_list<struct call_item> a) const;      // f({a, b}): left-to-right evaluation
    var call(const char* name, std::initializer_list<struct call_item> a) const;
    var call_(const CallArgs& a) const;
    // a method call: x.call("name", args...) == x.name(args...), without making the bound method
    template <class... A> var call(const char* name, A&&... a) const { return callmethod_(name, pack_args_(std::forward<A>(a)...)); }
    var callmethod_(const char* name, const CallArgs& a) const;

    // in-place operators (x += y: __iadd__, else __add__)
    var& operator+=(const var& o);
    var& operator-=(const var& o);
    var& operator*=(const var& o);
    var& operator/=(const var& o);
    var& operator%=(const var& o);
    var& operator|=(const var& o);
    var& operator&=(const var& o);
    var& operator^=(const var& o);
    var& operator<<=(const var& o);
    var& operator>>=(const var& o);

    // text: str(x) and repr(x) as UTF-8
    std::string str() const;
    std::string repr() const;
    // the UTF-8 of a str value (TypeError otherwise)
    const std::string& utf8() const;
    int64_t as_int64() const;                        // an int (or bool) that fits int64; TypeError / OverflowError
    double as_double() const;                        // float(x) of an int or float

private:
    Tag tag_;
    union {
        int64_t i_;
        double f_;
        Obj* p_;
    };
};

inline const var None_{None};

// ------------------------------------------------------------------------------------------ calls
struct kw_arg { const char* name; var value; };
struct star_arg { var value; };
struct starstar_arg { var value; };
inline kw_arg kw(const char* name, var v) { return {name, std::move(v)}; }
inline star_arg star(var v) { return {std::move(v)}; }
inline starstar_arg starstar(var v) { return {std::move(v)}; }

// a call's arguments once *args and **kwargs are spread
struct CallArgs {
    std::vector<var> pos;
    std::vector<std::pair<var, var>> kw;      // (name as a str var, value), in the call's order
    void add(const var& v) { pos.push_back(v); }
    void add(var&& v) { pos.push_back(std::move(v)); }
    void add(const kw_arg& k);
    void add(const star_arg& s);
    void add(const starstar_arg& s);
    template <class T, class = std::enable_if_t<!std::is_same_v<std::decay_t<T>, var> && std::is_constructible_v<var, T>>>
    void add(T&& v) { pos.emplace_back(std::forward<T>(v)); }
};
template <class... A> CallArgs pack_args_(A&&... a) {
    CallArgs c;
    c.pos.reserve(sizeof...(A));
    (c.add(std::forward<A>(a)), ...);
    return c;
}
// arguments in braces, f({a(), b()}): evaluated left to right, as Python evaluates them (a C++ call's
// own arguments are evaluated in no set order)
struct call_item {
    int kind;                 // 0 positional, 1 keyword, 2 *star, 3 **starstar
    const char* name = nullptr;
    var v;
    call_item(const var& x) : kind(0), v(x) {}
    call_item(var&& x) : kind(0), v(std::move(x)) {}
    template <class T, class = std::enable_if_t<!std::is_same_v<std::decay_t<T>, var> && std::is_constructible_v<var, T>>>
    call_item(T&& x) : kind(0), v(std::forward<T>(x)) {}
    call_item(const kw_arg& k) : kind(1), name(k.name), v(k.value) {}
    call_item(const star_arg& s) : kind(2), v(s.value) {}
    call_item(const starstar_arg& s) : kind(3), v(s.value) {}
};
CallArgs args_of_(std::initializer_list<call_item> a);
inline var var::operator()(std::initializer_list<call_item> a) const { return call_(args_of_(a)); }
inline var var::call(const char* name, std::initializer_list<call_item> a) const { return callmethod_(name, args_of_(a)); }

inline var var::attr(const char* name) const { return getattr(name); }
inline var var::operator[](const var& key) const { return getitem(key); }

// ------------------------------------------------------------------------------------------ literals
var str_lit_(const char* p, size_t n);       // made once per literal (keyed by its address)
var bytes_lit_(const char* p, size_t n);
var int_lit_(const char* digits);            // an int literal past int64
inline namespace literals {
inline var operator""_s(const char* p, size_t n) { return str_lit_(p, n); }
inline var operator""_b(const char* p, size_t n) { return bytes_lit_(p, n); }
}  // namespace literals

// ------------------------------------------------------------------------------------------ operators
var operator+(const var& a, const var& b);
var operator-(const var& a, const var& b);
var operator*(const var& a, const var& b);
var operator/(const var& a, const var& b);   // true division
var operator%(const var& a, const var& b);   // modulo, or %-formatting of a str / bytes
var operator<<(const var& a, const var& b);
var operator>>(const var& a, const var& b);
var operator&(const var& a, const var& b);
var operator|(const var& a, const var& b);
var operator^(const var& a, const var& b);
var operator-(const var& a);
var operator+(const var& a);
var operator~(const var& a);
var floordiv(const var& a, const var& b);    // a // b
var pow(const var& a, const var& b);         // a ** b
var pow(const var& a, const var& b, const var& m);
var matmul(const var& a, const var& b);
bool operator==(const var& a, const var& b);
bool operator!=(const var& a, const var& b);
bool operator<(const var& a, const var& b);
bool operator<=(const var& a, const var& b);
bool operator>(const var& a, const var& b);
bool operator>=(const var& a, const var& b);
var eq(const var& a, const var& b);          // a == b as Python's value (an __eq__ may return anything)
var lt(const var& a, const var& b);
var compare_value_(const var& a, const var& b, int op);   // a comparison as Python's value (op: 0 < 1 <= 2 == 3 != 4 > 5 >=)
bool is(const var& a, const var& b);         // a is b
bool in(const var& x, const var& container); // x in container
inline bool not_in(const var& x, const var& c) { return !in(x, c); }
inline bool is_not(const var& a, const var& b) { return !is(a, b); }
bool not_(const var& a);
// a or b / a and b as values; the right side evaluated only when needed
template <class F, class = std::enable_if_t<std::is_invocable_v<F> && !std::is_convertible_v<F, var>>> var or_(const var& a, F&& b) { if (a) return a; return var(b()); }
template <class F, class = std::enable_if_t<std::is_invocable_v<F> && !std::is_convertible_v<F, var>>> var and_(const var& a, F&& b) { if (!a) return a; return var(b()); }
inline var or_(const var& a, const var& b) { return a ? a : b; }
inline var and_(const var& a, const var& b) { return a ? b : a; }

// ------------------------------------------------------------------------------------------ exceptions
// A raised Python exception travels as a C++ exception holding the exception object.
struct error : std::exception {
    var value;                                  // the exception instance
    explicit error(var v) : value(std::move(v)) {}
    const char* what() const noexcept override;
    bool matches(const var& cls_or_tuple) const;   // except <cls>:
    mutable std::string what_;
};
[[noreturn]] void raise(const var& exc_or_class);                       // raise X / raise X()
[[noreturn]] void raise_from(const var& exc, const var& cause);         // raise X from Y
[[noreturn]] void raise(TypeObj* cls, const std::string& msg);          // raise ValueError("msg")
[[noreturn]] void reraise();                                             // bare raise
var current_exception();                         // sys.exc_info()[1] in an except block
struct handling_ {                               // an except block's exception, for bare raise / __context__
    explicit handling_(const error& e);
    ~handling_();
};
void assert_(const var& test, const var& msg = None);   // assert test, msg
[[noreturn]] void name_error(const char* name);

// ------------------------------------------------------------------------------------------ iteration
var iter(const var& x);                          // iter(x)
var next(const var& it);                         // next(it): StopIteration at the end
var next(const var& it, const var& dflt);
bool next_(const var& it, var& out);             // false at the end (StopIteration swallowed)
// for (py::var x : py::each(xs))
class each {
public:
    explicit each(const var& x);
    struct iterator {
        each* e;
        bool operator!=(const iterator&) const { return e != nullptr; }
        iterator& operator++() { if (!e->advance_()) e = nullptr; return *this; }
        const var& operator*() const { return e->cur_; }
    };
    iterator begin() { return {advance_() ? this : nullptr}; }
    iterator end() { return {nullptr}; }
private:
    bool advance_();
    var src_, it_, cur_;
    int64_t i_ = 0;
    int mode_ = 0;                               // 0: iterator protocol; 1: list by index; 2: tuple; 3: range
    int64_t stop_ = 0, step_ = 1;
};
// a, b = x: exactly n items (ValueError otherwise); with a starred target at `star`
std::vector<var> unpack(const var& x, size_t n);
std::vector<var> unpack_star(const var& x, size_t before, size_t after);   // [before..., list, after...]

// ------------------------------------------------------------------------------------------ generators
// A generator function's body runs on its own fiber; y(v) yields v and gives back what send() sent.
class yielder {
public:
    var operator()(const var& v);                // yield v
    var yield_from(const var& it);               // yield from it
    var await_(const var& awaitable);            // await x (in an async def)
    bool async_next(const var& ait, var& out);   // async for: out = await anext(ait); false at StopAsyncIteration
    void* impl_ = nullptr;
};
var aiter_(const var& x);                        // async for: type(x).__aiter__(x)
var awith_exit_of(const var& mgr);               // async with: the bound __aexit__ (__aenter__ checked too)
var awith_enter(const var& mgr);                 // async with: __aenter__() (awaited by the caller)
var with_exit_call(const var& exit_fn, const error& e);   // exit_fn(type, value, tb)'s value
// match: the subject's items as a list ([before..., [starred], after...]), or None when it is not a sequence of that length
var match_sequence_(const var& subj, size_t before, size_t after, bool star);
// match: the values of the keys in order (then, with rest, a dict of the other items), or None
var match_mapping_(const var& subj, const var& keys, bool rest);
// match: the attributes a class pattern names (npos by __match_args__, then the keywords'), or None
var match_class_(const var& subj, const var& cls, size_t npos, const var& kwd_names);

// ------------------------------------------------------------------------------------------ functions
// a function object from a C++ function or closure of N vars. sig is the Python parameter list:
//   "a, b=, *rest, key=, **kw"   ("=" marks a default, given in order in defaults; "/" and "*" as Python's)
// A generator function's C++ takes a py::yielder& first.
using invoker = std::function<var(var* slots)>;
var make_function(const char* name, const char* qualname, const char* sig, std::vector<var> defaults,
                  invoker fn, int nslots, bool generator, const char* module);
template <size_t... I, class F> invoker bind_slots_(F f, std::index_sequence<I...>) {
    return [f](var* s) -> var { return var(f(s[I]...)); };
}
yielder& current_yielder_();                     // the generator whose body is starting
template <size_t... I, class F> invoker bind_gen_slots_(F f, std::index_sequence<I...>) {
    return [f](var* s) -> var { return var(f(current_yielder_(), s[I]...)); };
}
template <class F> struct fn_arity_ : fn_arity_<decltype(&F::operator())> {};
template <class R, class... P> struct fn_arity_<R (*)(P...)> { static constexpr size_t n = sizeof...(P); };
template <class R, class... P> struct fn_arity_<R(P...)> { static constexpr size_t n = sizeof...(P); };
template <class C, class R, class... P> struct fn_arity_<R (C::*)(P...) const> { static constexpr size_t n = sizeof...(P); };
template <class C, class R, class... P> struct fn_arity_<R (C::*)(P...)> { static constexpr size_t n = sizeof...(P); };
// a closure cell (a local an inner function captures). A def's C++ lambda captures the cells it
// shares by value; def() copies the lambda once with a recorder on, so the function object knows
// which cells its lambda holds and the cycle collector can follow them (a nested function that calls
// itself is a cycle through its own cell).
struct CellObj;
inline thread_local std::vector<Obj*>* cell_recorder_ = nullptr;
class cell {
public:
    cell();
    cell(const cell& o) : c_(o.c_) { if (cell_recorder_) cell_recorder_->push_back(c_.ref()); }
    cell(cell&& o) noexcept : c_(std::move(o.c_)) {}
    cell& operator=(const cell&) = default;
    cell& operator=(cell&&) = default;
    var& operator*() const;
    var* operator->() const { return &**this; }
private:
    var c_;
};
// a copy of a closure made while recording the cells it captures (the copy is what the function keeps)
template <class F> F record_cells_(const F& fn, std::vector<Obj*>& cells) {
    if constexpr (std::is_class_v<F>) {
        std::vector<Obj*>* prev = cell_recorder_;
        cell_recorder_ = &cells;
        F copy(fn);
        cell_recorder_ = prev;
        return copy;
    } else {
        return fn;
    }
}
void set_closure_(const var& f, std::vector<Obj*> cells);   // the cells a function's lambda holds
// def name(sig): fn is a C++ function of as many py::var parameters as sig names
template <class F> var def(const char* name, const char* sig, std::vector<var> defaults, F fn,
                           const char* qualname = nullptr, const char* module = nullptr) {
    constexpr size_t N = fn_arity_<std::decay_t<F>>::n;
    std::vector<Obj*> cells;
    var f = make_function(name, qualname ? qualname : name, sig, std::move(defaults),
                          bind_slots_(record_cells_(fn, cells), std::make_index_sequence<N>{}), (int)N, false, module);
    if (!cells.empty()) set_closure_(f, std::move(cells));
    return f;
}
// a generator function: fn's first parameter is the py::yielder&
template <class F> var def_generator(const char* name, const char* sig, std::vector<var> defaults, F fn,
                                     const char* qualname = nullptr, const char* module = nullptr) {
    constexpr size_t N = fn_arity_<std::decay_t<F>>::n - 1;
    std::vector<Obj*> cells;
    var f = make_function(name, qualname ? qualname : name, sig, std::move(defaults),
                          bind_gen_slots_(record_cells_(fn, cells), std::make_index_sequence<N>{}), (int)N, true, module);
    if (!cells.empty()) set_closure_(f, std::move(cells));
    return f;
}
// a def's annotations, as their source text: {"param", "text", ..., "return", "text"} (__annotations__)
var annotate_(const var& f, std::initializer_list<const char*> name_text);
var doc_(const var& f, const char* doc);         // a def's __doc__
// async def: a coroutine function (kind 1) or an async generator function (kind 2); fn as def_generator's
var mark_async_(const var& f, int kind);
template <class F> var def_coroutine(const char* name, const char* sig, std::vector<var> defaults, F fn,
                                     const char* qualname = nullptr, const char* module = nullptr) {
    return mark_async_(def_generator(name, sig, std::move(defaults), fn, qualname, module), 1);
}
template <class F> var def_async_generator(const char* name, const char* sig, std::vector<var> defaults, F fn,
                                           const char* qualname = nullptr, const char* module = nullptr) {
    return mark_async_(def_generator(name, sig, std::move(defaults), fn, qualname, module), 2);
}
// a native builtin taking the call's arguments as they come
using native_fn = std::function<var(const CallArgs&)>;
var native(const char* name, native_fn fn);

// ------------------------------------------------------------------------------------------ classes
// class Name(bases, **kw): body fills ns; the class is made by its metaclass (type, or the bases')
var make_class(const char* name, const char* qualname, const std::vector<var>& bases, const var& ns,
               const std::vector<std::pair<const char*, var>>& kw = {}, const char* module = nullptr);
var new_dict();                                  // a class body's namespace (and dict displays)
var super_(const var& cls, const var& self);    // super() inside a method of cls
var isinstance(const var& x, const var& cls);
var issubclass(const var& c, const var& cls);
var type(const var& x);                          // type(x)
var type_var(TypeObj* t);
var classmethod(const var& f);
var staticmethod(const var& f);
var property(const var& fget, const var& fset = None, const var& fdel = None, const var& doc = None);

// ------------------------------------------------------------------------------------------ modules
// One per Python module: its globals are cells the C++ holds as references (`static py::var& x =
// M.global("x");`), so a module attribute set from outside (mod.x = 1) is what the code reads.
class module_def {
public:
    module_def(const char* name, void (*init)(), bool package = false, const char* file = nullptr);
    var& global(const char* name);               // the cell of a module-level name
    const var& module() const { return mod_; }
    const char* name() const { return name_; }
    void (*init_)();
    bool package_;
    const char* file_;
private:
    const char* name_;
    var mod_;
};
var import(const char* dotted);                  // import a.b.c: the module a.b.c (a, a.b imported first)
var import_top(const char* dotted);              // import a.b.c binds a: the top package
var import_from(const var& module, const char* name);   // from m import name (a submodule too)
void import_star(const var& module, module_def& into);  // from m import *
var builtin(const char* name);                   // builtins.name (a module may rebind its own)
inline var or_builtin(const var& v, const char* name) { return v.is_unbound() ? builtin(name) : v; }   // a module name a builtin stands in for until bound

// ------------------------------------------------------------------------------------------ builtins
// (the type objects: py::t::str, py::t::int_, py::t::ValueError ... in py_builtins.hpp's namespace t)
namespace t {
extern var object, type, NoneType, bool_, int_, float_, complex, str, bytes, bytearray, list, tuple, dict,
    set, frozenset, range, slice, function, builtin_function, method, module, generator, property,
    staticmethod, classmethod, super, memoryview, enumerate, zip, map, filter, reversed, cell, NotImplementedType,
    ellipsis;
extern var BaseException, Exception, SystemExit, KeyboardInterrupt, GeneratorExit, StopIteration,
    ArithmeticError, ZeroDivisionError, OverflowError, FloatingPointError, AssertionError, AttributeError,
    BufferError, EOFError, ImportError, ModuleNotFoundError, LookupError, IndexError, KeyError,
    MemoryError, NameError, UnboundLocalError, OSError, FileNotFoundError, FileExistsError,
    PermissionError, IsADirectoryError, NotADirectoryError, TimeoutError, InterruptedError,
    ChildProcessError, ConnectionError, BrokenPipeError, ProcessLookupError, BlockingIOError,
    ReferenceError, RuntimeError, NotImplementedError, RecursionError, SyntaxError,
    SystemError, TypeError, ValueError, UnicodeError, UnicodeDecodeError, UnicodeEncodeError,
    Warning, UserWarning, DeprecationWarning, RuntimeWarning, StopAsyncIteration;
}  // namespace t
extern var NotImplemented, Ellipsis;
TypeObj* T(const var& type_var);                 // the TypeObj of a type var

var len(const var& x);
int64_t len_(const var& x);
var repr(const var& x);
var str(const var& x);
var ascii(const var& x);
var hash(const var& x);
int64_t hash_(const var& x);
var id(const var& x);
var abs(const var& x);
var divmod(const var& a, const var& b);
var round(const var& x, const var& nd = None);
var chr(const var& i);
var ord(const var& c);
var hex(const var& i);
var oct(const var& i);
var bin(const var& i);
var format(const var& x, const var& spec = None);
var getattr(const var& o, const var& name);
var getattr(const var& o, const var& name, const var& dflt);
void setattr(const var& o, const var& name, const var& v);
var hasattr(const var& o, const var& name);
void delattr(const var& o, const var& name);
var callable(const var& x);
var vars(const var& x);
var dir(const var& x);
template <class... A> var print(A&&... a) { return print_(pack_args_(std::forward<A>(a)...)); }
var print_(const CallArgs& a);
var globals_of(module_def& m);                   // globals()
var exec_unsupported(const char* what);          // exec / eval of a runtime string: refused at run time

// containers
var list(std::initializer_list<var> items = {});
var list_from(std::vector<var> items);
var tuple(std::initializer_list<var> items = {});
var tuple_from(std::vector<var> items);
var dict(std::initializer_list<std::pair<var, var>> items = {});
var set(std::initializer_list<var> items = {});
var frozenset_from(const var& iterable);
var range(const var& stop);
var range(const var& start, const var& stop, const var& step = 1);
var slice_obj(const var& lo, const var& hi, const var& step);
var bytes_from(const std::string& raw);
var str_from(const std::string& utf8);
var int_from(const std::string& text, int base = 10);
// list displays with *spread, dict displays with **spread
var list_display(std::initializer_list<var> items, std::initializer_list<int> starred);
var tuple_display(std::initializer_list<var> items, std::initializer_list<int> starred);
var set_display(std::initializer_list<var> items, std::initializer_list<int> starred);
void dict_update_(const var& d, const var& mapping);   // {**m}

// f-strings: format(value, spec) after !r / !s / !a
var fstr(std::initializer_list<var> parts);           // ''.join of the pieces
var fmt(const var& v, char conv, const var& spec);    // one {v!conv:spec}

// what translated statements use
inline bool truthy_(const var& v) { return static_cast<bool>(v); }
// a name of a class body being run: py::at(ns, "x"_s) = v stores it (an unbound value deletes it)
struct slot_ {
    var d, k;
    slot_& operator=(const var& v) {
        if (v.is_unbound()) d.delitem(k);
        else d.setitem(k, v);
        return *this;
    }
};
inline slot_ at(const var& d, const var& k) { return {d, k}; }
var class_name(const var& ns, const var& key, const var& fallback);   // a class body's name: its namespace, then outside
var with_exit_of(const var& mgr);                // with: the manager's bound __exit__ (looked up before __enter__)
var with_enter(const var& mgr);                  // with: __enter__()
bool with_exit_exc(const var& exit_fn, const error& e);   // with: __exit__(type, value, tb) -- true: suppressed

// raw access for native modules
const std::string& str_utf8(const var& s);
bool is_str(const var& x);
bool is_bytes(const var& x);       // bytes or bytearray
const std::string& bytes_raw(const var& b);
bool is_int(const var& x);         // int or bool
bool is_list(const var& x);
bool is_tuple(const var& x);
bool is_dict(const var& x);
std::vector<var>& list_items(const var& l);
const std::vector<var>& tuple_items(const var& t);
std::vector<var> to_vector(const var& iterable);

// the runtime: once, before any module (the generated main does it)
void init_runtime(int argc, char** argv);
int run_main(const char* module, int argc, char** argv);   // python -m module: SystemExit's code

}  // namespace py

using py::literals::operator""_s;
using py::literals::operator""_b;
