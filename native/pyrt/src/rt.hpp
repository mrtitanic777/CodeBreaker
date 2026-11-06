// rt.hpp -- the runtime's inside: the heap objects behind py::var, the type objects, and the helpers
// every part of native/pyrt shares. Generated code never includes it (py_object.hpp is its API).
#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "big.hpp"
#include "py_object.hpp"

namespace py {

using pyengine::Big;

// ------------------------------------------------------------------------------------------ types
// The kind of storage a type's instances have (a class deriving from a builtin keeps its storage).
enum class Layout : uint8_t {
    Object, Type, Int, Float, Complex, Str, Bytes, ByteArray, List, Tuple, Dict, Set, FrozenSet, Range,
    Slice, Function, Native, Method, Module, Cell, Exception, Generator, Iter, Property, StaticMethod,
    ClassMethod, Super, NativeData
};

struct DictObj;
struct StrObj;

struct TypeObj : Obj {
    std::string name, qualname, module;
    std::vector<TypeObj*> bases;
    std::vector<TypeObj*> mro;            // C3 (itself first)
    DictObj* dict = nullptr;              // the class namespace
    Layout layout = Layout::Object;
    bool builtin = false;
    bool is_exception = false;
    bool abstract_instances = false;      // (instances may not be made: NoneType ...)
    uint64_t version = 0;                 // bumped when the dict (or a base's) changes
    TypeObj* metatype = nullptr;          // the metaclass when not type
    bool no_dict = false;                 // __slots__ (and no base with a __dict__): instances have only those
    std::vector<std::string> slots;       // (this class's own slot names)
    ~TypeObj() override;
    // attribute of the type through its MRO (null: absent); borrowed
    const var* lookup(const var& name) const;
    const var* lookup(const char* name) const;
};

// ------------------------------------------------------------------------------------------ values
struct BigObj : Obj { Big v; };
struct FloatObj : Obj { double v = 0; };   // (a float subclass's instance)
struct StrObj : Obj {
    std::string s;                        // UTF-8
    int64_t n = 0;                        // code points
    bool ascii = true;
    mutable int64_t h = -1;
    mutable std::u32string* cps = nullptr;   // decoded, for indexing a non-ASCII str
    ~StrObj() override { delete cps; }
    const std::u32string& u32() const;
};
struct BytesObj : Obj { std::string s; mutable int64_t h = -1; };   // bytes and bytearray
struct ListObj : Obj { std::vector<var> v; };
struct TupleObj : Obj { std::vector<var> v; mutable int64_t h = -1; };

// dict: insertion ordered (entries in order, a hash index over them), as CPython's
struct DictObj : Obj {
    struct Entry { int64_t hash; var key, value; };
    std::vector<Entry> entries;           // deleted entries have an unbound key
    std::vector<int32_t> index;           // -1 empty, -2 deleted, else entry position
    int64_t used = 0;
    uint64_t version = 0;                 // (iteration checks "changed size during iteration")
    TypeObj* owner_type = nullptr;        // a class namespace: its type (bumps its version)
    int64_t find(const var& key, int64_t h) const;      // entry position or -1
    var* get(const var& key) const;
    var* get(const char* name) const;
    void set(const var& key, const var& v);
    bool del(const var& key);
    void clear();
    void rebuild_(size_t cap);
    var default_factory;                  // collections.defaultdict
    int ordered_kind = 0;                 // 0 dict, 1 OrderedDict, 2 Counter, 3 defaultdict
};

// set: CPython's own table (linear probes then perturbation), so iteration order is CPython's
struct SetObj : Obj {
    struct Slot { int64_t hash = 0; var key; bool dummy = false; };
    std::vector<Slot> table;              // size a power of two
    int64_t fill = 0, used = 0;
    int64_t finger = 0;                   // pop()
    mutable int64_t h = -1;               // frozenset
    SetObj() { table.resize(8); }
    bool contains(const var& key, int64_t h) const;
    bool add(const var& key, int64_t h);
    bool discard(const var& key, int64_t h);
    void resize_(int64_t minused);
    void clear() { table.assign(8, Slot()); fill = used = 0; }
};

struct RangeObj : Obj { var start, stop, step; int64_t a = 0, b = 0, s = 1; bool small = true; };
struct SliceObj : Obj { var start, stop, step; };

struct Param {
    std::string name;
    enum Kind : uint8_t { PosOnly, Normal, VarArgs, KwOnly, VarKw } kind = Normal;
    bool has_default = false;
};
struct FuncObj : Obj {
    std::string name, qualname, module;
    std::vector<Param> params;
    std::vector<var> defaults;            // per param (unbound where none)
    invoker fn;
    int nslots = 0;
    bool generator = false;
    uint8_t async_kind = 0;               // 1: async def (a coroutine), 2: an async generator, 3: @types.coroutine
    int npos = 0;                         // PosOnly + Normal
    int varargs = -1, varkw = -1;
    var doc, annotations, wrapped, dict_extra;
    int firstlineno = 0;                  // a lambda's line in its file (co_firstlineno; inspect.getsource finds it by it)
};
struct NativeObj : Obj { std::string name; native_fn fn; var self; };   // a builtin (bound to self when set)
struct MethodObj : Obj { var func, self; };
struct CellObj : Obj { var v; };
struct ModuleObj : Obj {
    std::string name;
    std::unordered_map<std::string, CellObj*> cells;   // owned (one reference each)
    std::vector<std::string> order;
    module_def* def = nullptr;
    bool initialized = false;
    ~ModuleObj() override;
    CellObj* cell(const std::string& n);
};
struct ExcObj : Obj {
    var args;                             // a tuple
    var cause, context, traceback;
    bool suppress_context = false;
};
struct PropertyObj : Obj { var fget, fset, fdel, doc; std::string name; };
struct WrapObj : Obj { var f; };          // staticmethod / classmethod
struct SuperObj : Obj { var cls, self; };
// native data for the library modules (a compiled pattern, a match, a file, a hash ...)
struct DataObj : Obj {
    virtual ~DataObj() override = default;
};
// a native iterator: next() fills out, false at the end
struct IterObj : Obj {
    std::function<bool(var&)> next;
};

// ------------------------------------------------------------------------------------------ helpers
extern TypeObj *T_object, *T_type, *T_none, *T_bool, *T_int, *T_float, *T_complex, *T_str, *T_bytes,
    *T_bytearray, *T_list, *T_tuple, *T_dict, *T_set, *T_frozenset, *T_range, *T_slice, *T_function,
    *T_native, *T_method, *T_module, *T_cell, *T_generator, *T_iter, *T_property, *T_staticmethod,
    *T_classmethod, *T_super, *T_notimpl, *T_ellipsis, *T_data;
extern TypeObj *E_BaseException, *E_Exception, *E_TypeError, *E_ValueError, *E_KeyError, *E_IndexError,
    *E_AttributeError, *E_StopIteration, *E_ZeroDivisionError, *E_OverflowError, *E_NameError,
    *E_UnboundLocalError, *E_RuntimeError, *E_NotImplementedError, *E_AssertionError, *E_OSError,
    *E_FileNotFoundError, *E_FileExistsError, *E_PermissionError, *E_IsADirectoryError,
    *E_NotADirectoryError, *E_ImportError, *E_ModuleNotFoundError, *E_UnicodeDecodeError,
    *E_UnicodeEncodeError, *E_LookupError, *E_GeneratorExit, *E_SystemExit, *E_RecursionError,
    *E_MemoryError, *E_TimeoutError, *E_EOFError, *E_KeyboardInterrupt, *E_UnicodeError, *E_SyntaxError;

template <class O> O* as(const var& v) { return static_cast<O*>(v.ref()); }
inline bool has_layout(const var& v, Layout l) { return v.is_ref() && v.ref()->type->layout == l; }

TypeObj* type_of(const var& v);
var tv(TypeObj* t);                                // the var of a type
var new_str(std::string utf8);
var new_str_cp(const std::u32string& cps);
var new_bytes(std::string raw, bool bytearray = false);
var new_list(std::vector<var> v = {});
var new_tuple(std::vector<var> v = {});
var new_dict_obj(TypeObj* t = nullptr);
var new_set(bool frozen = false);
var new_int(const Big& b);                         // normalized: small when it fits
var new_iter(std::function<bool(var&)> next);
var new_native(const char* name, native_fn fn);
var bind_native(const var& native, const var& self);
Big to_big(const var& v);                          // an int (or bool)
bool is_big(const var& v);
var interned(const char* name);                    // a str made once per name
var make_exc(TypeObj* cls, const std::string& msg);
var make_exc_args(TypeObj* cls, std::vector<var> args);
[[noreturn]] void raise_type(const char* fmt_msg, const var& a);
[[noreturn]] void raise_stop(const var& value);
std::string tname(const var& v);                   // type(v).__name__
var call_type(TypeObj* t, const CallArgs& a);
var call_obj(const var& f, const CallArgs& a);
var call0(const var& f);
var call1(const var& f, const var& a);
var call2(const var& f, const var& a, const var& b);
bool lookup_special(const var& o, const char* name, var& out);   // a dunder through the type (bound)
var bind(const var& attr, const var& self, TypeObj* t);           // the descriptor protocol's __get__
bool is_subtype(TypeObj* a, TypeObj* b);
bool isinstance_(const var& x, TypeObj* t);
TypeObj* new_type(const char* name, std::vector<TypeObj*> bases, Layout layout, bool builtin = true,
                  const char* module = "builtins");
void add_method(TypeObj* t, const char* name, native_fn fn);   // fn sees self as a.pos[0]
void add_static(TypeObj* t, const char* name, native_fn fn);
void add_classmethod(TypeObj* t, const char* name, native_fn fn);   // a.pos[0] is the class
void add_getter(TypeObj* t, const char* name, std::function<var(const var&)> get);
bool eq_(const var& a, const var& b);              // a == b, bool (identity first, as containers do)
int cmp_(const var& a, const var& b);              // a < b ? -1 ... for sort (via __lt__)
bool lt_(const var& a, const var& b);
std::string repr_(const var& v);
std::string str_(const var& v);
int64_t hash_int(const Big& b);
int64_t hash_double(double d);
int64_t index_(const var& v);                      // __index__: an int to int64
int64_t norm_index(int64_t i, int64_t n, const char* what);   // negative from the end; IndexError
void slice_indices(const var& slice_lo, const var& hi, const var& step, int64_t n, int64_t& lo, int64_t& stop, int64_t& st, int64_t& count);
std::vector<var> collect(const var& iterable);     // list(iterable)'s items
var arg(const CallArgs& a, size_t i, const char* name, const var& dflt = var());   // positional or keyword
var kwarg(const CallArgs& a, const char* name, const var& dflt = var());
void check_args(const CallArgs& a, const char* fname, size_t min, size_t max);
std::string utf8_of(const var& s);                 // str's UTF-8 (TypeError otherwise)
const std::string& raw_of(const var& b);           // bytes / bytearray (TypeError otherwise)
std::string fs_path(const var& p);                 // os.fspath: str or a PathLike's __fspath__

// the code points of a str (the ASCII fast path: bytes are code points)
int64_t cp_count(const std::string& s);
std::u32string to_u32(const std::string& s);
std::string from_u32(const std::u32string& s);
void append_utf8(std::string& out, char32_t c);
size_t cp_offset(const StrObj* s, int64_t i);       // byte offset of code point i
int64_t cp_index(const StrObj* s, size_t byte);     // code point index of a byte offset

// formatting
std::string repr_float(double d);
std::string format_spec(const var& v, const std::string& spec);   // format(v, spec) for builtins
std::string percent_format(const var& fmt, const var& args, bool bytes);
std::string int_to_base(const Big& b, int base, bool upper = false);

// registration of the parts (each part's init)
void init_types();
void init_object();
void init_int();
void init_str();
void init_bytes();
void init_containers();
void init_dict();
void init_builtins();
void init_exceptions();
void init_gen();
void init_modules();          // the library modules: registered, made on first import
using native_module_init = void (*)(const var& module);
void register_native_module(const char* name, native_module_init init);
void mod_set(const var& module, const char* name, const var& v);
void mod_def(const var& module, const char* name, native_fn fn);

// the global interpreter lock (threading.Thread runs with it; blocking calls release it)
struct gil_release {
    gil_release();
    ~gil_release();
};
void gil_acquire_();
void gil_release_();

}  // namespace py
