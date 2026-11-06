// lib_ctypes.cc -- ctypes in C++: a translated program's calls into DLLs (CodeBreaker's own native
// libraries, unicorn.dll, capstone.dll, kernel32 ...) go straight to their C functions.
//
// The types are CPython's: the simple types (c_int ... c_wchar_p), POINTER(T), T * n arrays, Structure
// and Union subclasses laid out from _fields_ (C's alignment, _pack_), CFUNCTYPE prototypes. Instances
// own their memory or view another's (from_address, a field, .contents), keeping what they point into
// alive. A call converts each argument by its argtypes entry (or by its kind), releases the global lock,
// calls through the Windows x64 convention (integers in RCX..R9, doubles in XMM0..3, the rest on the
// stack: one caller per float/integer pattern of the first four), and converts the result by restype.
// A Python callable given to a CFUNCTYPE gets one of a pool of 2,048 C entry points.
#include <cstring>
#include <memory>
#include <mutex>
#include <array>
#include <map>
#include <unordered_map>

#include <windows.h>

#include "rt.hpp"

namespace py {

TypeObj* T(const var& v);
void print_exception_(const var& e);
bool gil_ensure_();          // lib_more.cc: take the global lock if this thread has not got it (true: taken)
var type_new_(TypeObj* meta, const CallArgs& a);

namespace {

using u64 = uint64_t;

// ------------------------------------------------------------------------------------------ types
enum class CK { Simple, Pointer, Array, Struct, Union, Func };
struct CType;
struct Field {
    std::string name;
    CType* t = nullptr;
    size_t offset = 0;
    int bits = 0, bitpos = 0;
};
struct CType {
    CK kind = CK::Simple;
    char code = 0;          // simple: b B h H i I l L q Q f d ? c u z Z P
    size_t size = 0, align = 1;
    TypeObj* py = nullptr;
    CType* elem = nullptr;  // pointer target, array element
    size_t length = 0;      // array
    std::vector<Field> fields;
    bool laid_out = false;
    bool swap = false;      // simple: stored big-endian (a BigEndianStructure's field)
    CType* plain = nullptr; // (swap: the same type in native order)
    // a function prototype
    CType* res = nullptr;   // null: None (void) -- unless res_none is false and res_py is set
    bool res_void = false;
    var res_py;             // a restype that is not a ctypes type (a callable)
    std::vector<CType*> args;
};
std::unordered_map<TypeObj*, CType*>& types_() {
    static auto* m = new std::unordered_map<TypeObj*, CType*>();
    return *m;
}
TypeObj *T_meta, *T_cdata, *T_simple, *T_pointer, *T_array, *T_struct, *T_union, *T_funcptr, *T_lib, *T_loader, *T_bigstruct, *T_bigunion;
CType* layout_struct(TypeObj* t, CType* ct);

CType* ctype_of(TypeObj* t, bool must = true) {
    auto& m = types_();
    for (TypeObj* b : t->mro) {
        auto it = m.find(b);
        if (it == m.end()) continue;
        CType* ct = it->second;
        if (b != t && (ct->kind == CK::Struct || ct->kind == CK::Union)) {
            // a subclass of a structure: its own type, its base's fields first
            auto* own = new CType(*ct);
            own->py = t;
            own->laid_out = false;
            own->fields.clear();
            incref(t);
            m[t] = own;
            return layout_struct(t, own);
        }
        if (b != t) {
            auto* own = new CType(*ct);   // (class X(c_int): X's own type, the same layout)
            own->py = t;
            incref(t);
            m[t] = own;
            return own;
        }
        if ((ct->kind == CK::Struct || ct->kind == CK::Union) && !ct->laid_out) layout_struct(t, ct);
        return ct;
    }
    if (is_subtype(t, T_struct) || is_subtype(t, T_union)) {
        auto* ct = new CType();
        ct->kind = is_subtype(t, T_union) ? CK::Union : CK::Struct;
        ct->py = t;
        incref(t);
        m[t] = ct;
        return layout_struct(t, ct);
    }
    if (must) raise(E_TypeError, "'" + t->name + "' is not a ctypes type");
    return nullptr;
}
CType* ctype_of_var(const var& typ) {
    if (!typ.is_ref() || typ.ref()->type->layout != Layout::Type) raise(E_TypeError, "a ctypes type is expected, not " + tname(typ));
    return ctype_of(static_cast<TypeObj*>(typ.ref()));
}

// ------------------------------------------------------------------------------------------ instances
struct CData : DataObj {
    uint8_t* p = nullptr;
    std::unique_ptr<uint8_t[]> own;
    var base;          // what p points into (kept alive)
    var keep;          // objects this one's memory refers to (a dict, made on demand)
    CType* ct = nullptr;
};
CData* CD(const var& v) {
    if (!v.is_ref() || !isinstance_(v, T_cdata)) raise(E_TypeError, "a ctypes instance is expected, not " + tname(v) + (getenv("PYRT_DEBUG") ? " [" + repr_(v) + "]" : std::string()));
    return static_cast<CData*>(v.ref());
}
bool is_cdata(const var& v) { return v.is_ref() && v.ref()->type->layout != Layout::Type && isinstance_(v, T_cdata); }
void keep_(CData* d, const var& key, const var& v) {
    if (d->keep.is_unbound()) d->keep = new_dict();
    d->keep.setitem(key, v);
}
var make_cdata(CType* ct, uint8_t* at = nullptr, const var& base = var()) {
    auto* d = new CData();
    d->type = ct->py;
    d->ct = ct;
    if (at) {
        d->p = at;
        d->base = base;
    } else {
        // at least 16 bytes, as CPython's ctypes keeps a value of up to 16 inline (CDataObject's b_value):
        // a DLL may write past a small type's size and CPython never sees it -- unicorn's reg_read of an
        // x87 register writes its 10 bytes into the c_uint64 the binding passes
        size_t n = std::max<size_t>(ct->size, 16);
        d->own.reset(new uint8_t[n]());
        d->p = d->own.get();
    }
    return var(static_cast<Obj*>(d));
}

std::wstring to_w(const std::string& s) {
    int n = MultiByteToWideChar(CP_UTF8, 0, s.data(), (int)s.size(), nullptr, 0);
    std::wstring w((size_t)n, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, s.data(), (int)s.size(), w.data(), n);
    return w;
}
std::string from_w(const wchar_t* w, size_t n) {
    int k = WideCharToMultiByte(CP_UTF8, 0, w, (int)n, nullptr, 0, nullptr, nullptr);
    std::string s((size_t)k, '\0');
    WideCharToMultiByte(CP_UTF8, 0, w, (int)n, s.data(), k, nullptr, nullptr);
    return s;
}

int64_t sext(u64 v, size_t size) {
    switch (size) {
    case 1: return (int8_t)v;
    case 2: return (int16_t)v;
    case 4: return (int32_t)v;
    default: return (int64_t)v;
    }
}
u64 zext(u64 v, size_t size) { return size >= 8 ? v : v & ((1ull << (size * 8)) - 1); }
bool is_signed(char c) { return c == 'b' || c == 'h' || c == 'i' || c == 'l' || c == 'q'; }

u64 big_bits(const var& v);
// an integer argument's 64 bits (a Python int of any size wraps, as ctypes' integer conversion does)
u64 int_bits(const var& v) {
    if (v.is_bool()) return v.truthy() ? 1 : 0;
    if (is_int(v)) {
        if (is_big(v)) return big_bits(v);
        return (u64)v.as_int64();
    }
    if (v.is_float()) raise(E_TypeError, "int expected instead of float");
    return (u64)index_(v);
}
u64 big_bits(const var& v) {
    // the low 64 bits of an int of any size
    var lo = v & var(INT64_MAX);
    u64 r = (u64)lo.as_int64();
    if (((v >> var(63)) & var(1)).truthy()) r |= 1ull << 63;
    return r;
}

var read_value(CType* ct, uint8_t* p, const var& base);
var read_item(CType* ct, uint8_t* p, const var& base);
void write_value(CType* ct, uint8_t* p, const var& v, CData* owner, const var& key);

// the address a value stands for where a pointer is wanted
u64 pointer_bits(const var& v, CData* owner, const var& key, CType* want_elem = nullptr);

var read_value(CType* ct, uint8_t* p, const var& base) {
    if (ct->swap) {
        uint8_t tmp[8];
        for (size_t i = 0; i < ct->size; i++) tmp[i] = p[ct->size - 1 - i];
        return read_value(ct->plain, tmp, base);
    }
    switch (ct->kind) {
    case CK::Simple: {
        u64 raw = 0;
        switch (ct->code) {
        case 'f': { float f; memcpy(&f, p, 4); return var((double)f); }
        case 'd': { double d; memcpy(&d, p, 8); return var(d); }
        case '?': return var(*p != 0);
        case 'c': return new_bytes(std::string(1, (char)*p));
        case 'u': { wchar_t w; memcpy(&w, p, 2); return new_str(from_w(&w, 1)); }
        case 'z': { const char* s; memcpy(&s, p, 8); return s ? new_bytes(std::string(s)) : var(None); }
        case 'Z': { const wchar_t* s; memcpy(&s, p, 8); return s ? new_str(from_w(s, wcslen(s))) : var(None); }
        case 'P': { u64 a; memcpy(&a, p, 8); return a ? var((int64_t)a) : var(None); }
        default: break;
        }
        memcpy(&raw, p, ct->size);
        if (is_signed(ct->code)) return var(sext(raw, ct->size));
        raw = zext(raw, ct->size);
        if (raw > (u64)INT64_MAX) return var((int64_t)(raw >> 1)) * var(2) + var((int64_t)(raw & 1));
        return var((int64_t)raw);
    }
    case CK::Pointer: case CK::Func: case CK::Array: case CK::Struct: case CK::Union:
        return make_cdata(ct, p, base);
    }
    return None;
}

// a value read out of memory as a field, an item, a result or a callback's argument (CPython's CData_get): a
// fundamental simple type's is the Python value; a subclass of one (class Sort(c_void_p)) stays an instance
// of it -- over base's memory when there is a base, else a copy
var read_item(CType* ct, uint8_t* p, const var& base) {
    if (ct->kind == CK::Simple && ct->py && !ct->py->bases.empty() && ct->py->bases[0] != T_simple) {
        if (!base.is_unbound()) return make_cdata(ct, p, base);
        var o = make_cdata(ct);
        memcpy(CD(o)->p, p, ct->size);
        return o;
    }
    return read_value(ct, p, base);
}

void write_value(CType* ct, uint8_t* p, const var& v, CData* owner, const var& key) {
    if (ct->swap) {   // big-endian: written in native order, then reversed
        uint8_t tmp[8] = {0};
        write_value(ct->plain, tmp, v, owner, key);
        for (size_t i = 0; i < ct->size; i++) p[i] = tmp[ct->size - 1 - i];
        return;
    }
    switch (ct->kind) {
    case CK::Simple: {
        if (is_cdata(v) && CD(v)->ct->kind == CK::Simple && CD(v)->ct->size == ct->size && CD(v)->ct->code == ct->code) {
            memcpy(p, CD(v)->p, ct->size);
            if (!CD(v)->keep.is_unbound() && owner) keep_(owner, key, v);
            return;
        }
        switch (ct->code) {
        case 'f': { float f = (float)v.as_double(); memcpy(p, &f, 4); return; }
        case 'd': { double d = v.as_double(); memcpy(p, &d, 8); return; }
        case '?': *p = v.truthy() ? 1 : 0; return;
        case 'c': {
            if (is_int(v)) { *p = (uint8_t)v.as_int64(); return; }
            const std::string& b = raw_of(v);
            if (b.size() != 1) raise(E_TypeError, "one character bytes, bytearray or integer expected");
            *p = (uint8_t)b[0];
            return;
        }
        case 'u': { std::wstring w = to_w(str_utf8(v)); uint16_t c = w.empty() ? 0 : (uint16_t)w[0]; memcpy(p, &c, 2); return; }
        case 'z': case 'Z': case 'P': {
            u64 a = pointer_bits(v, owner, key);
            memcpy(p, &a, 8);
            return;
        }
        default: break;
        }
        u64 bits = is_int(v) && is_big(v) ? big_bits(v) : int_bits(v);
        memcpy(p, &bits, ct->size);
        return;
    }
    case CK::Pointer: case CK::Func: {
        u64 a = pointer_bits(v, owner, key, ct->elem);
        memcpy(p, &a, 8);
        return;
    }
    case CK::Array: case CK::Struct: case CK::Union: {
        if (is_cdata(v)) {
            CData* s = CD(v);
            memcpy(p, s->p, std::min(ct->size, s->ct->size));
            if (owner) keep_(owner, key, v);
            return;
        }
        if (ct->kind == CK::Array && (is_bytes(v) || has_layout(v, Layout::ByteArray)) && ct->elem->code == 'c') {
            const std::string& b = raw_of(v);
            if (b.size() > ct->size) raise(E_ValueError, "byte string too long");
            memset(p, 0, ct->size);
            memcpy(p, b.data(), b.size());
            return;
        }
        if (ct->kind == CK::Array) {
            auto items = collect(v);
            if (items.size() > ct->length) raise(E_IndexError, "too many initializers");
            for (size_t i = 0; i < items.size(); i++) write_value(ct->elem, p + i * ct->elem->size, items[i], owner, key);
            return;
        }
        raise(E_TypeError, "expected " + ct->py->name + " instance, got " + tname(v));
    }
    }
}

// a keepalive buffer for a bytes / str value a pointer is made of
struct Holder : DataObj { std::string s; std::wstring w; };
TypeObj* T_holder = nullptr;
var holder() {
    if (!T_holder) T_holder = new_type("_ctypes_buffer", {}, Layout::NativeData, true, "_ctypes");
    auto* h = new Holder();
    h->type = T_holder;
    return var(static_cast<Obj*>(h));
}

u64 cdata_address_for_pointer(CData* d) {
    // a pointer, c_void_p, c_char_p, a CFUNCTYPE instance: its value; anything else (an array, a
    // structure, a simple value): its address
    if (d->ct->kind == CK::Pointer || d->ct->kind == CK::Func || (d->ct->kind == CK::Simple && strchr("zZP", d->ct->code))) {
        u64 a;
        memcpy(&a, d->p, 8);
        return a;
    }
    return (u64)(uintptr_t)d->p;
}

u64 pointer_bits(const var& v, CData* owner, const var& key, CType*) {
    if (v.is_none()) return 0;
    if (is_int(v)) return int_bits(v);
    if (is_bytes(v) || has_layout(v, Layout::ByteArray)) {
        if (has_layout(v, Layout::ByteArray)) {
            if (owner) keep_(owner, key, v);
            return (u64)(uintptr_t)as<BytesObj>(v)->s.data();
        }
        var h = holder();
        as<Holder>(h)->s = raw_of(v);
        if (owner) keep_(owner, key, h);
        else raise(E_TypeError, "a bytes pointer needs an owner");
        return (u64)(uintptr_t)as<Holder>(h)->s.c_str();
    }
    if (is_str(v)) {
        var h = holder();
        as<Holder>(h)->w = to_w(str_utf8(v));
        if (owner) keep_(owner, key, h);
        return (u64)(uintptr_t)as<Holder>(h)->w.c_str();
    }
    if (is_cdata(v)) {
        CData* d = CD(v);
        if (owner) keep_(owner, key, v);
        return cdata_address_for_pointer(d);
    }
    raise(E_TypeError, "cannot make a pointer of " + tname(v));
}

// ------------------------------------------------------------------------------------------ layout
size_t align_up(size_t n, size_t a) { return a ? (n + a - 1) / a * a : n; }
CType* array_of(CType* e, size_t n);
u64 bswap_n(u64 v, size_t n) { u64 r = 0; for (size_t i = 0; i < n; i++) r |= ((v >> (8 * i)) & 0xFF) << (8 * (n - 1 - i)); return r; }
// a BigEndianStructure's field types: simple ones stored byte-reversed, arrays of them likewise
CType* swapped_of(CType* t) {
    static std::map<CType*, CType*> made;
    if (t->kind == CK::Array) return array_of(swapped_of(t->elem), t->length);
    if (t->kind != CK::Simple || t->size < 2 || strchr("zZP", t->code) || t->swap) return t;
    if (auto it = made.find(t); it != made.end()) return it->second;
    auto* s = new CType(*t);
    s->swap = true;
    s->plain = t;
    made[t] = s;
    return s;
}
CType* layout_struct(TypeObj* t, CType* ct) {
    bool big = is_subtype(t, T_bigstruct) || is_subtype(t, T_bigunion);
    const var* fl = nullptr;
    for (TypeObj* b : t->mro) {
        if (var* f = b->dict->get("_fields_")) { fl = f; break; }
    }
    if (!fl) { ct->size = 0; ct->align = 1; ct->laid_out = false; return ct; }   // (an incomplete type: laid out when _fields_ comes)
    size_t pack = 0;
    if (const var* pk = t->lookup("_pack_")) pack = (size_t)pk->as_int64();
    bool uni = ct->kind == CK::Union;
    // the base class's fields first (a structure deriving from a structure)
    std::vector<Field> fields;
    size_t off = 0, align = 1;
    for (size_t i = 1; i < t->mro.size(); i++) {
        auto it = types_().find(t->mro[i]);
        if (it != types_().end() && (it->second->kind == CK::Struct) && !uni && t->mro[i]->dict->get("_fields_") != fl) {
            fields = it->second->fields;
            off = it->second->size;
            align = it->second->align;
            break;
        }
    }
    size_t bit_unit_off = 0, bit_unit_size = 0;
    int bit_used = 0;
    for (var f : each(*fl)) {
        auto parts = collect(f);
        Field fd;
        fd.name = str_utf8(parts.at(0));
        fd.t = ctype_of_var(parts.at(1));
        if (big) fd.t = swapped_of(fd.t);
        size_t a = fd.t->align;
        if (pack && a > pack) a = pack;
        if (parts.size() > 2) {   // a bit field
            fd.bits = (int)parts[2].as_int64();
            if (bit_unit_size == fd.t->size && bit_used + fd.bits <= (int)(fd.t->size * 8) && !uni) {
                fd.offset = bit_unit_off;
                fd.bitpos = bit_used;
                bit_used += fd.bits;
            } else {
                off = uni ? 0 : align_up(off, a);
                fd.offset = off;
                fd.bitpos = 0;
                bit_unit_off = off;
                bit_unit_size = fd.t->size;
                bit_used = fd.bits;
                if (!uni) off += fd.t->size;
            }
        } else {
            bit_unit_size = 0;
            if (uni) fd.offset = 0;
            else {
                off = align_up(off, a);
                fd.offset = off;
                off += fd.t->size;
            }
        }
        if (uni) off = std::max(off, fd.t->size);
        align = std::max(align, a);
        fields.push_back(fd);
    }
    ct->fields = fields;
    ct->align = align;
    ct->size = align_up(off, align);
    ct->laid_out = true;
    // a descriptor per field, as CPython's CField
    for (size_t i = 0; i < ct->fields.size(); i++) {
        Field fd = ct->fields[i];
        auto* pr = new PropertyObj();
        pr->type = T_property;
        pr->name = fd.name;
        var key = new_str(fd.name);
        pr->fget = new_native(fd.name.c_str(), [fd](const CallArgs& a) -> var {
            CData* d = CD(a.pos.at(0));
            if (fd.bits) {
                u64 raw = 0;
                memcpy(&raw, d->p + fd.offset, fd.t->size);
                if (fd.t->swap) raw = bswap_n(raw, fd.t->size);
                int sh = fd.t->swap ? (int)fd.t->size * 8 - fd.bitpos - fd.bits : fd.bitpos;   // (big-endian bit fields count from the top)
                u64 v = (raw >> sh) & ((fd.bits >= 64 ? ~0ull : (1ull << fd.bits) - 1));
                if (is_signed(fd.t->code) && fd.bits < 64 && (v >> (fd.bits - 1)) & 1) return var((int64_t)(v | ~((1ull << fd.bits) - 1)));
                return var((int64_t)v);
            }
            // a char / wchar array field reads as its string (to the first NUL), as CPython's
            if (fd.t->kind == CK::Array && fd.t->elem->kind == CK::Simple && fd.t->elem->code == 'c')
                return new_bytes(std::string((const char*)d->p + fd.offset, strnlen((const char*)d->p + fd.offset, fd.t->length)));
            if (fd.t->kind == CK::Array && fd.t->elem->kind == CK::Simple && fd.t->elem->code == 'u')
                return new_str(from_w((const wchar_t*)(d->p + fd.offset), wcsnlen((const wchar_t*)(d->p + fd.offset), fd.t->length)));
            return read_item(fd.t, d->p + fd.offset, a.pos[0]);
        });
        pr->fset = new_native(fd.name.c_str(), [fd, key](const CallArgs& a) -> var {
            CData* d = CD(a.pos.at(0));
            if (fd.bits) {
                u64 raw = 0;
                memcpy(&raw, d->p + fd.offset, fd.t->size);
                if (fd.t->swap) raw = bswap_n(raw, fd.t->size);
                int sh = fd.t->swap ? (int)fd.t->size * 8 - fd.bitpos - fd.bits : fd.bitpos;
                u64 mask = (fd.bits >= 64 ? ~0ull : (1ull << fd.bits) - 1) << sh;
                raw = (raw & ~mask) | ((int_bits(a.pos.at(1)) << sh) & mask);
                if (fd.t->swap) raw = bswap_n(raw, fd.t->size);
                memcpy(d->p + fd.offset, &raw, fd.t->size);
                return None;
            }
            write_value(fd.t, d->p + fd.offset, a.pos.at(1), d, key);
            return None;
        });
        t->dict->set(new_str(fd.name), var(static_cast<Obj*>(pr)));
    }
    t->version++;
    return ct;
}

// ------------------------------------------------------------------------------------------ derived types
std::map<std::pair<CType*, size_t>, CType*>& arrays_() { static auto* m = new std::map<std::pair<CType*, size_t>, CType*>(); return *m; }
std::map<CType*, CType*>& pointers_() { static auto* m = new std::map<CType*, CType*>(); return *m; }

TypeObj* new_ctype_class(const std::string& name, TypeObj* base) {
    CallArgs c;
    var ns = new_dict();
    ns.setitem("__module__"_s, "ctypes"_s);
    c.pos = {new_str(name), new_tuple({tv(base)}), ns};
    var cls = type_new_(T_meta, c);
    TypeObj* t = T(cls);
    incref(t);   // (the table of ctypes types holds it: kept for the program's life)
    return t;
}
CType* array_of(CType* e, size_t n) {
    auto key = std::make_pair(e, n);
    auto& m = arrays_();
    if (auto it = m.find(key); it != m.end()) return it->second;
    auto* ct = new CType();
    ct->kind = CK::Array;
    ct->elem = e;
    ct->length = n;
    ct->size = e->size * n;
    ct->align = e->align;
    ct->py = new_ctype_class(e->py->name + "_Array_" + std::to_string(n), T_array);
    ct->py->dict->set(interned("_type_"), tv(e->py));
    ct->py->dict->set(interned("_length_"), var((int64_t)n));
    types_()[ct->py] = ct;
    m[key] = ct;
    return ct;
}
CType* pointer_to(CType* e) {
    auto& m = pointers_();
    if (auto it = m.find(e); it != m.end()) return it->second;
    auto* ct = new CType();
    ct->kind = CK::Pointer;
    ct->elem = e;
    ct->size = 8;
    ct->align = 8;
    ct->py = new_ctype_class("LP_" + e->py->name, T_pointer);
    ct->py->dict->set(interned("_type_"), tv(e->py));
    types_()[ct->py] = ct;
    m[e] = ct;
    return ct;
}

// ------------------------------------------------------------------------------------------ calls
template <class A> A as_arg(u64 v) {
    if constexpr (std::is_same_v<A, double>) { double d; memcpy(&d, &v, 8); return d; }
    else return v;
}
template <class R, class A0, class A1, class A2, class A3>
R call16(void* f, const u64* a) {
    using F = R (*)(A0, A1, A2, A3, u64, u64, u64, u64, u64, u64, u64, u64, u64, u64, u64, u64);
    return ((F)f)(as_arg<A0>(a[0]), as_arg<A1>(a[1]), as_arg<A2>(a[2]), as_arg<A3>(a[3]), a[4], a[5], a[6], a[7], a[8], a[9], a[10],
                  a[11], a[12], a[13], a[14], a[15]);
}
template <class R>
R dispatch(void* f, const u64* a, int mask) {
    switch (mask & 15) {
    case 0: return call16<R, u64, u64, u64, u64>(f, a);
    case 1: return call16<R, double, u64, u64, u64>(f, a);
    case 2: return call16<R, u64, double, u64, u64>(f, a);
    case 3: return call16<R, double, double, u64, u64>(f, a);
    case 4: return call16<R, u64, u64, double, u64>(f, a);
    case 5: return call16<R, double, u64, double, u64>(f, a);
    case 6: return call16<R, u64, double, double, u64>(f, a);
    case 7: return call16<R, double, double, double, u64>(f, a);
    case 8: return call16<R, u64, u64, u64, double>(f, a);
    case 9: return call16<R, double, u64, u64, double>(f, a);
    case 10: return call16<R, u64, double, u64, double>(f, a);
    case 11: return call16<R, double, double, u64, double>(f, a);
    case 12: return call16<R, u64, u64, double, double>(f, a);
    case 13: return call16<R, double, u64, double, double>(f, a);
    case 14: return call16<R, u64, double, double, double>(f, a);
    default: return call16<R, double, double, double, double>(f, a);
    }
}
thread_local DWORD t_last_error = 0;

// one argument as its 64 bits (temporaries kept in `keep` for the call)
u64 arg_bits(CType* at, const var& v, CData* keep_owner, int index, bool& is_double) {
    var key = var((int64_t)index);
    is_double = false;
    if (at) {
        if (at->kind == CK::Simple && (at->code == 'f' || at->code == 'd')) {
            is_double = true;
            double d = is_cdata(v) ? read_value(CD(v)->ct, CD(v)->p, v).as_double() : v.as_double();
            u64 bits = 0;
            if (at->code == 'f') { float f = (float)d; memcpy(&bits, &f, 4); }
            else memcpy(&bits, &d, 8);
            return bits;
        }
        if (at->kind == CK::Simple && !strchr("zZP", at->code)) {
            if (is_cdata(v)) {
                CData* d = CD(v);
                if (d->ct->kind == CK::Simple) { u64 r = 0; memcpy(&r, d->p, std::min<size_t>(8, d->ct->size)); return r; }
                return cdata_address_for_pointer(d);
            }
            if (at->code == '?') return v.truthy() ? 1 : 0;
            if (at->code == 'c' && (is_bytes(v))) return (u64)(uint8_t)raw_of(v).at(0);
            return is_int(v) && is_big(v) ? big_bits(v) : int_bits(v);
        }
        if (at->kind == CK::Struct || at->kind == CK::Union) {
            if (at->size > 8 || (at->size & (at->size - 1))) {
                // larger than 8 (or not 1/2/4/8): passed by reference to a copy, as the convention says
                var copy = make_cdata(at);
                write_value(at, CD(copy)->p, v, nullptr, key);
                keep_(keep_owner, key, copy);
                return (u64)(uintptr_t)CD(copy)->p;
            }
            u64 r = 0;
            memcpy(&r, CD(v)->p, at->size);
            return r;
        }
        // pointers (POINTER(T), c_void_p, c_char_p, c_wchar_p, CFUNCTYPE): a T instance passes its address
        if (at->kind == CK::Pointer && is_cdata(v) && CD(v)->ct->kind != CK::Pointer && CD(v)->ct->kind != CK::Array) {
            keep_(keep_owner, key, v);
            return (u64)(uintptr_t)CD(v)->p;
        }
        return pointer_bits(v, keep_owner, key);
    }
    // no argtypes: by the value's kind
    if (v.is_float()) { is_double = true; double d = v.as_double(); u64 b; memcpy(&b, &d, 8); return b; }
    if (is_cdata(v)) {
        CData* d = CD(v);
        if (d->ct->kind == CK::Simple && !strchr("zZP", d->ct->code)) {
            if (d->ct->code == 'f' || d->ct->code == 'd') {
                is_double = true;
                double x = read_value(d->ct, d->p, v).as_double();
                u64 b = 0;
                if (d->ct->code == 'f') { float f = (float)x; memcpy(&b, &f, 4); } else memcpy(&b, &x, 8);
                return b;
            }
            u64 r = 0;
            memcpy(&r, d->p, d->ct->size);
            return r;
        }
    }
    if (is_int(v) || v.is_bool()) return is_int(v) && is_big(v) ? big_bits(v) : int_bits(v);
    return pointer_bits(v, keep_owner, key);
}
var result_of(CType* rt, bool res_void, const var& res_py, u64 r, double rd) {
    if (res_void) return None;
    if (!rt) {
        if (!res_py.is_unbound() && !res_py.is_none()) return call1(res_py, var(sext(r, 4)));
        return var(sext(r, 4));   // (c_int, the default)
    }
    if (rt->kind == CK::Simple) {
        if (rt->code == 'd') return var(rd);
        if (rt->code == 'f') { float f; memcpy(&f, &rd, 4); return var((double)f); }
        uint8_t buf[8];
        memcpy(buf, &r, 8);
        return read_item(rt, buf, var());
    }
    if (rt->kind == CK::Pointer || rt->kind == CK::Func) {
        var p = make_cdata(rt);
        memcpy(CD(p)->p, &r, 8);
        return p;
    }
    raise(E_TypeError, "a structure returned by value is not supported");
}

struct Proto {
    std::vector<CType*> args;
    bool has_args = false;
    CType* res = nullptr;
    bool res_void = false;
    var res_py;
};
var do_call(void* f, const Proto& pr, const CallArgs& a, const var& errcheck, const var& self) {
    if (a.pos.size() > 16) raise(E_TypeError, "more than 16 arguments");
    if (pr.has_args && a.pos.size() < pr.args.size()) raise(E_TypeError, "this function takes at least " + std::to_string(pr.args.size()) + " arguments (" + std::to_string(a.pos.size()) + " given)");
    auto* holder_obj = new CData();
    holder_obj->type = T_cdata;
    var hold(static_cast<Obj*>(holder_obj));
    u64 slots[16] = {0};
    int mask = 0;
    for (size_t i = 0; i < a.pos.size(); i++) {
        CType* at = pr.has_args && i < pr.args.size() ? pr.args[i] : nullptr;
        bool dbl = false;
        slots[i] = arg_bits(at, a.pos[i], holder_obj, (int)i, dbl);
        if (dbl && i < 4) mask |= 1 << i;
    }
    u64 r = 0;
    double rd = 0;
    bool want_double = pr.res && pr.res->kind == CK::Simple && (pr.res->code == 'd' || pr.res->code == 'f');
    {
        gil_release g;
        SetLastError(0);
        if (want_double) rd = dispatch<double>(f, slots, mask);
        else r = dispatch<u64>(f, slots, mask);
        t_last_error = GetLastError();
    }
    var out = result_of(pr.res, pr.res_void, pr.res_py, r, rd);
    if (!errcheck.is_unbound() && !errcheck.is_none()) out = call_obj(errcheck, CallArgs{{out, self, new_tuple(a.pos)}, {}});
    return out;
}

// ------------------------------------------------------------------------------------------ callbacks
struct Callback {
    var fn;
    Proto proto;
    bool used = false;
};
constexpr int kSlots = 2048;   // (objects in reference cycles are not collected: a run of emulators may hold many)
Callback g_cb[kSlots];
std::mutex g_cb_mu;
template <int I>
u64 trampoline(u64 a0, u64 a1, u64 a2, u64 a3, u64 a4, u64 a5, u64 a6, u64 a7, u64 a8, u64 a9, u64 a10, u64 a11) {
    u64 a[12] = {a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11};
    bool took = gil_ensure_();
    u64 ret = 0;
    Callback& cb = g_cb[I];
    try {
        std::vector<var> args;
        for (size_t i = 0; i < cb.proto.args.size() && i < 12; i++) {
            CType* at = cb.proto.args[i];
            uint8_t buf[8];
            memcpy(buf, &a[i], 8);
            if (at->kind == CK::Pointer || at->kind == CK::Func) {
                var p = make_cdata(at);
                memcpy(CD(p)->p, buf, 8);
                args.push_back(p);
            } else args.push_back(read_item(at, buf, var()));
        }
        var r = call_obj(cb.fn, CallArgs{args, {}});
        if (!cb.proto.res_void && cb.proto.res && !r.is_none()) {
            if (cb.proto.res->kind == CK::Simple && !strchr("zZP", cb.proto.res->code)) ret = is_int(r) && is_big(r) ? big_bits(r) : int_bits(r);
            else if (is_int(r)) ret = int_bits(r);
            else if (is_cdata(r)) ret = cdata_address_for_pointer(CD(r));
        } else if (!cb.proto.res_void && !cb.proto.res && is_int(r)) ret = int_bits(r);
    } catch (error& e) {
        fprintf(stderr, "Exception ignored on calling ctypes callback function: %s\n", repr_(cb.fn).c_str());
        print_exception_(e.value);
    }
    if (took) gil_release_();
    return ret;
}
using tramp_t = u64 (*)(u64, u64, u64, u64, u64, u64, u64, u64, u64, u64, u64, u64);
template <int... I> constexpr std::array<tramp_t, sizeof...(I)> make_tramps(std::integer_sequence<int, I...>) { return {&trampoline<I>...}; }
const std::array<tramp_t, kSlots> g_tramps = make_tramps(std::make_integer_sequence<int, kSlots>{});

// ------------------------------------------------------------------------------------------ the function object
struct FuncPtr : CData {
    void* addr = nullptr;
    int slot = -1;
    Proto proto;          // from the CFUNCTYPE type (argtypes / restype attributes override)
    std::string name;
    ~FuncPtr() override {
        if (slot >= 0) {
            std::lock_guard<std::mutex> l(g_cb_mu);
            g_cb[slot].used = false;
            g_cb[slot].fn = var();
        }
    }
};
Proto proto_of_type(TypeObj* t) {
    Proto p;
    CType* ct = ctype_of(t, false);
    if (!ct || ct->kind != CK::Func) return p;
    p.args = ct->args;
    p.has_args = true;
    p.res = ct->res;
    p.res_void = ct->res_void;
    p.res_py = ct->res_py;
    return p;
}
Proto current_proto(const var& self) {
    auto* f = static_cast<FuncPtr*>(self.ref());
    Proto p = f->proto;
    if (self.ref()->attrs) {
        auto* d = static_cast<DictObj*>(self.ref()->attrs);
        if (var* at = d->get("argtypes")) {
            p.args.clear();
            p.has_args = !at->is_none();
            if (!at->is_none()) for (var x : each(*at)) p.args.push_back(ctype_of_var(x));
        }
        if (var* rt = d->get("restype")) {
            p.res = nullptr;
            p.res_void = rt->is_none();
            p.res_py = var();
            if (!rt->is_none()) {
                if (rt->is_ref() && rt->ref()->type->layout == Layout::Type && ctype_of(T(*rt), false)) p.res = ctype_of(T(*rt));
                else p.res_py = *rt;
            }
        }
    }
    return p;
}
var new_funcptr(TypeObj* t, void* addr, const std::string& name) {
    auto* f = new FuncPtr();
    f->type = t;
    CType* ct = ctype_of(t, false);
    if (!ct) {
        static CType* fct = nullptr;
        if (!fct) { fct = new CType(); fct->kind = CK::Func; fct->size = 8; fct->align = 8; fct->py = T_funcptr; }
        ct = fct;
    }
    f->ct = ct;
    f->own.reset(new uint8_t[16]());
    f->p = f->own.get();
    f->addr = addr;
    memcpy(f->p, &addr, 8);
    f->name = name;
    f->proto = proto_of_type(t);
    return var(static_cast<Obj*>(f));
}

// ------------------------------------------------------------------------------------------ libraries
struct Lib : DataObj { HMODULE h = nullptr; std::string name; };
var load_library(TypeObj* cls, const var& name_v) {
    std::string name = name_v.is_none() ? "" : fs_path(name_v);
    HMODULE h;
    {
        gil_release g;
        if (name.empty()) h = GetModuleHandleW(nullptr);
        else {
            std::wstring w = to_w(name);
            bool abs = w.size() > 2 && (w[1] == L':' || (w[0] == L'\\' && w[1] == L'\\'));
            h = LoadLibraryExW(w.c_str(), nullptr, abs ? LOAD_LIBRARY_SEARCH_DEFAULT_DIRS | LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR : 0);
            if (!h && abs) h = LoadLibraryExW(w.c_str(), nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);
        }
    }
    if (!h) {
        DWORD e = GetLastError();
        raise(make_exc_args(E_OSError, {var((int64_t)22), new_str("Could not find module '" + name + "' (or one of its dependencies). Try using the full path with constructor syntax."), None, var((int64_t)e)}));
    }
    auto* l = new Lib();
    l->type = cls;
    l->h = h;
    l->name = name;
    return var(static_cast<Obj*>(l));
}

}  // namespace

bool ctypes_is_cdata_(const var& v) { return is_cdata(v); }

void init_ctypes(const var& m) {
    // the metaclass: T * n, and class creation (Structure subclasses laid out on first use)
    T_meta = new_type("PyCType", {T_type}, Layout::Type, false, "_ctypes");   // (not builtin: its dunders (T * n) are looked up)
    add_static(T_meta, "__new__", [](const CallArgs& a) {
        CallArgs b;
        b.pos.assign(a.pos.begin() + 1, a.pos.end());
        b.kw = a.kw;
        return type_new_(T(a.pos.at(0)), b);
    });
    add_method(T_meta, "__mul__", [](const CallArgs& a) -> var {
        int64_t n = a.pos.at(1).as_int64();
        if (n < 0) raise(E_ValueError, "Array length must be >= 0, not " + std::to_string(n));
        return tv(array_of(ctype_of(T(a.pos[0])), (size_t)n)->py);
    });
    add_method(T_meta, "__rmul__", [](const CallArgs& a) { return tv(array_of(ctype_of(T(a.pos[0])), (size_t)a.pos.at(1).as_int64())->py); });
    auto base = [](const char* name, std::vector<TypeObj*> bases) {
        TypeObj* t = new_type(name, bases, Layout::NativeData, false, "_ctypes");
        t->type = T_meta;
        t->metatype = T_meta;
        return t;
    };
    T_cdata = base("_CData", {});
    T_simple = base("_SimpleCData", {T_cdata});
    T_pointer = base("_Pointer", {T_cdata});
    T_array = base("Array", {T_cdata});
    T_struct = base("Structure", {T_cdata});
    T_union = base("Union", {T_cdata});
    T_bigstruct = base("BigEndianStructure", {T_struct});
    T_bigunion = base("BigEndianUnion", {T_union});
    T_funcptr = base("CFuncPtr", {T_cdata});

    // instances: zeroed memory of the type's size, then __init__ fills it
    add_static(T_cdata, "__new__", [](const CallArgs& a) -> var {
        TypeObj* t = T(a.pos.at(0));
        if (is_subtype(t, T_funcptr)) {
            // CFUNCTYPE(...)(callable) / (address) / (name, dll)
            if (a.pos.size() > 1 && is_int(a.pos[1])) return new_funcptr(t, (void*)(uintptr_t)int_bits(a.pos[1]), "");
            if (a.pos.size() > 2 && is_str(a.pos[1])) {
                Lib* l = static_cast<Lib*>(a.pos[2].ref());
                void* f = (void*)GetProcAddress(l->h, str_utf8(a.pos[1]).c_str());
                if (!f) raise(E_AttributeError, "function '" + str_utf8(a.pos[1]) + "' not found");
                return new_funcptr(t, f, str_utf8(a.pos[1]));
            }
            if (a.pos.size() > 1 && callable(a.pos[1]).truthy()) {
                Proto pr = proto_of_type(t);
                for (CType* x : pr.args)
                    if (x->kind == CK::Simple && (x->code == 'f' || x->code == 'd')) raise(E_TypeError, "a callback with float arguments is not supported");
                int slot = -1;
                {
                    std::lock_guard<std::mutex> l(g_cb_mu);
                    for (int i = 0; i < kSlots; i++) if (!g_cb[i].used) { slot = i; break; }
                    if (slot < 0) raise(E_MemoryError, "no more ctypes callback entry points (" + std::to_string(kSlots) + " in use)");
                    g_cb[slot].used = true;
                    g_cb[slot].fn = a.pos[1];
                    g_cb[slot].proto = pr;
                }
                var f = new_funcptr(t, (void*)g_tramps[(size_t)slot], "");
                static_cast<FuncPtr*>(f.ref())->slot = slot;
                return f;
            }
            return new_funcptr(t, nullptr, "");
        }
        return make_cdata(ctype_of(t));
    });
    add_method(T_cdata, "__init__", [](const CallArgs& a) -> var {
        var self = a.pos.at(0);
        CData* d = CD(self);
        CType* ct = d->ct;
        if (ct->kind == CK::Func) return None;
        if (ct->kind == CK::Simple) {
            if (a.pos.size() > 1) write_value(ct, d->p, a.pos[1], d, "value"_s);
            return None;
        }
        if (ct->kind == CK::Pointer) {
            if (a.pos.size() > 1) {
                // POINTER(T)(obj): pointing at obj
                var obj = a.pos[1];
                u64 adr = is_cdata(obj) ? (u64)(uintptr_t)CD(obj)->p : pointer_bits(obj, d, "contents"_s);
                keep_(d, "contents"_s, obj);
                memcpy(d->p, &adr, 8);
            }
            return None;
        }
        if (ct->kind == CK::Array) {
            if (a.pos.size() - 1 > ct->length) raise(E_IndexError, "too many initializers");
            for (size_t i = 1; i < a.pos.size(); i++) write_value(ct->elem, d->p + (i - 1) * ct->elem->size, a.pos[i], d, var((int64_t)i - 1));
            return None;
        }
        // structures: positional in field order, then keywords by name
        size_t i = 1;
        for (auto& f : ct->fields) {
            if (i >= a.pos.size()) break;
            self.setattr(f.name.c_str(), a.pos[i++]);
        }
        if (i < a.pos.size()) raise(E_TypeError, "too many initializers");
        for (auto& [k, v] : a.kw) self.setattr(str_utf8(k).c_str(), v);
        return None;
    });
    add_classmethod(T_cdata, "from_address", [](const CallArgs& a) {
        CType* ct = ctype_of(T(a.pos.at(0)));
        return make_cdata(ct, (uint8_t*)(uintptr_t)int_bits(a.pos.at(1)), var());
    });
    add_classmethod(T_cdata, "from_buffer", [](const CallArgs& a) -> var {
        CType* ct = ctype_of(T(a.pos.at(0)));
        var buf = a.pos.at(1);
        if (buf.is_ref() && type_of(buf)->name == "memoryview") buf = buf.getattr("obj");   // (a memoryview: what it views)
        int64_t off = arg(a, 2, "offset", var(0)).as_int64();
        uint8_t* p;
        size_t n;
        if (is_cdata(buf)) { p = CD(buf)->p; n = CD(buf)->ct->size; }
        else if (has_layout(buf, Layout::ByteArray)) { p = (uint8_t*)as<BytesObj>(buf)->s.data(); n = as<BytesObj>(buf)->s.size(); }
        else raise(E_TypeError, "underlying buffer is not writable");
        if (off < 0 || (size_t)off + ct->size > n) raise(E_ValueError, "Buffer size too small (" + std::to_string(n) + " instead of at least " + std::to_string(ct->size + (size_t)off) + " bytes)");
        return make_cdata(ct, p + off, buf);
    });
    add_classmethod(T_cdata, "from_buffer_copy", [](const CallArgs& a) {
        CType* ct = ctype_of(T(a.pos.at(0)));
        var buf = a.pos.at(1);
        std::string s = is_cdata(buf) ? std::string((const char*)CD(buf)->p, CD(buf)->ct->size) : raw_of(buf);
        int64_t off = arg(a, 2, "offset", var(0)).as_int64();
        if (off < 0 || (size_t)off + ct->size > s.size()) raise(E_ValueError, "Buffer size too small (" + std::to_string(s.size()) + " instead of at least " + std::to_string(ct->size + (size_t)off) + " bytes)");
        var o = make_cdata(ct);
        memcpy(CD(o)->p, s.data() + off, ct->size);
        return o;
    });
    add_classmethod(T_cdata, "in_dll", [](const CallArgs& a) -> var {
        CType* ct = ctype_of(T(a.pos.at(0)));
        Lib* l = static_cast<Lib*>(a.pos.at(1).ref());
        void* p = (void*)GetProcAddress(l->h, str_utf8(a.pos.at(2)).c_str());
        if (!p) raise(E_ValueError, "symbol '" + str_utf8(a.pos[2]) + "' not found");
        return make_cdata(ct, (uint8_t*)p, a.pos[1]);
    });
    add_method(T_cdata, "__bytes__", [](const CallArgs& a) { CData* d = CD(a.pos[0]); return new_bytes(std::string((const char*)d->p, d->ct->size)); });
    add_method(T_cdata, "__reduce__", [](const CallArgs&) -> var { raise(E_ValueError, "ctypes objects containing pointers cannot be pickled"); });
    add_getter(T_cdata, "_b_base_", [](const var& s) { var b = CD(s)->base; return b.is_unbound() ? var(None) : b; });
    add_getter(T_cdata, "_b_needsfree_", [](const var& s) { return var(CD(s)->own != nullptr); });
    add_getter(T_cdata, "_objects", [](const var& s) { var k = CD(s)->keep; return k.is_unbound() ? var(None) : k; });

    // simple values
    auto* pv = new PropertyObj();
    pv->type = T_property;
    pv->name = "value";
    pv->fget = new_native("value", [](const CallArgs& a) { CData* d = CD(a.pos.at(0)); return read_value(d->ct, d->p, a.pos[0]); });
    pv->fset = new_native("value", [](const CallArgs& a) { CData* d = CD(a.pos.at(0)); write_value(d->ct, d->p, a.pos.at(1), d, "value"_s); return None; });
    T_simple->dict->set(interned("value"), var(static_cast<Obj*>(pv)));
    add_method(T_simple, "__repr__", [](const CallArgs& a) {
        CData* d = CD(a.pos[0]);
        return new_str(type_of(a.pos[0])->name + "(" + repr_(read_value(d->ct, d->p, a.pos[0])) + ")");
    });
    add_method(T_simple, "__bool__", [](const CallArgs& a) { CData* d = CD(a.pos[0]); return var(read_value(d->ct, d->p, a.pos[0]).truthy()); });
    add_classmethod(T_simple, "from_param", [](const CallArgs& a) { return a.pos.at(1); });

    struct S { const char* name; char code; size_t size; };
    const S simples[] = {{"c_byte", 'b', 1}, {"c_ubyte", 'B', 1}, {"c_short", 'h', 2}, {"c_ushort", 'H', 2}, {"c_int", 'i', 4},
                         {"c_uint", 'I', 4}, {"c_long", 'l', 4}, {"c_ulong", 'L', 4}, {"c_longlong", 'q', 8}, {"c_ulonglong", 'Q', 8},
                         {"c_float", 'f', 4}, {"c_double", 'd', 8}, {"c_bool", '?', 1}, {"c_char", 'c', 1}, {"c_wchar", 'u', 2},
                         {"c_char_p", 'z', 8}, {"c_wchar_p", 'Z', 8}, {"c_void_p", 'P', 8}};
    std::map<std::string, var> made;
    for (auto& s : simples) {
        TypeObj* t = base(s.name, {T_simple});
        t->module = "ctypes";
        auto* ct = new CType();
        ct->kind = CK::Simple;
        ct->code = s.code;
        ct->size = s.size;
        ct->align = s.size;
        ct->py = t;
        t->dict->set(interned("_type_"), new_str(std::string(1, s.code == 'l' ? 'l' : s.code)));
        types_()[t] = ct;
        made[s.name] = tv(t);
        mod_set(m, s.name, tv(t));
    }
    mod_set(m, "c_int8", made["c_byte"]);
    mod_set(m, "c_uint8", made["c_ubyte"]);
    mod_set(m, "c_int16", made["c_short"]);
    mod_set(m, "c_uint16", made["c_ushort"]);
    mod_set(m, "c_int32", made["c_int"]);
    mod_set(m, "c_uint32", made["c_uint"]);
    mod_set(m, "c_int64", made["c_longlong"]);
    mod_set(m, "c_uint64", made["c_ulonglong"]);
    mod_set(m, "c_size_t", made["c_ulonglong"]);
    mod_set(m, "c_ssize_t", made["c_longlong"]);
    mod_set(m, "c_time_t", made["c_longlong"]);
    mod_set(m, "c_longdouble", made["c_double"]);
    mod_set(m, "_SimpleCData", tv(T_simple));
    mod_set(m, "_Pointer", tv(T_pointer));
    mod_set(m, "Array", tv(T_array));
    mod_set(m, "Structure", tv(T_struct));
    mod_set(m, "Union", tv(T_union));
    mod_set(m, "LittleEndianStructure", tv(T_struct));
    mod_set(m, "BigEndianStructure", tv(T_bigstruct));
    mod_set(m, "BigEndianUnion", tv(T_bigunion));
    mod_set(m, "LittleEndianUnion", tv(T_union));
    mod_set(m, "_CFuncPtr", tv(T_funcptr));
    mod_set(m, "_FuncPointer", tv(T_funcptr));
    mod_set(m, "_CData", tv(T_cdata));
    mod_set(m, "ArgumentError", make_class("ArgumentError", "ArgumentError", {t::Exception}, new_dict(), {}, "ctypes"));
    mod_set(m, "RTLD_LOCAL", var(0));
    mod_set(m, "RTLD_GLOBAL", var(0));
    mod_set(m, "DEFAULT_MODE", var(0));

    // pointers
    add_getter(T_pointer, "contents", [](const var& s) -> var {
        CData* d = CD(s);
        u64 a;
        memcpy(&a, d->p, 8);
        if (!a) raise(E_ValueError, "NULL pointer access");
        return make_cdata(d->ct->elem, (uint8_t*)(uintptr_t)a, var());
    });
    add_method(T_pointer, "__getitem__", [](const CallArgs& a) -> var {
        CData* d = CD(a.pos[0]);
        u64 adr;
        memcpy(&adr, d->p, 8);
        if (!adr) raise(E_ValueError, "NULL pointer access");
        CType* e = d->ct->elem;
        if (has_layout(a.pos.at(1), Layout::Slice)) {
            auto* sl = as<SliceObj>(a.pos[1]);
            int64_t lo = sl->start.is_none() ? 0 : sl->start.as_int64(), hi = sl->stop.as_int64();
            std::vector<var> out;
            for (int64_t i = lo; i < hi; i++) out.push_back(read_item(e, (uint8_t*)(uintptr_t)adr + i * (int64_t)e->size, a.pos[0]));
            if (e->kind == CK::Simple && e->code == 'c') { std::string s; for (auto& x : out) s += raw_of(x); return new_bytes(s); }
            return new_list(out);
        }
        int64_t i = a.pos[1].as_int64();
        return read_item(e, (uint8_t*)(uintptr_t)adr + i * (int64_t)e->size, a.pos[0]);
    });
    add_method(T_pointer, "__setitem__", [](const CallArgs& a) {
        CData* d = CD(a.pos[0]);
        u64 adr;
        memcpy(&adr, d->p, 8);
        if (!adr) raise(E_ValueError, "NULL pointer access");
        CType* e = d->ct->elem;
        write_value(e, (uint8_t*)(uintptr_t)adr + a.pos.at(1).as_int64() * (int64_t)e->size, a.pos.at(2), d, a.pos[1]);
        return None;
    });
    add_method(T_pointer, "__bool__", [](const CallArgs& a) { u64 v; memcpy(&v, CD(a.pos[0])->p, 8); return var(v != 0); });
    add_classmethod(T_pointer, "from_param", [](const CallArgs& a) { return a.pos.at(1); });
    mod_def(m, "POINTER", [](const CallArgs& a) -> var {
        if (is_str(a.pos.at(0))) {
            // an incomplete type by name: a pointer to a structure named so, to be completed
            auto* ct = new CType();
            ct->kind = CK::Struct;
            ct->py = new_ctype_class(str_utf8(a.pos[0]), T_struct);
            types_()[ct->py] = ct;
            return tv(pointer_to(ct)->py);
        }
        return tv(pointer_to(ctype_of_var(a.pos[0]))->py);
    });
    mod_def(m, "pointer", [](const CallArgs& a) {
        var obj = a.pos.at(0);
        CData* d = CD(obj);
        var p = make_cdata(pointer_to(d->ct));
        u64 adr = (u64)(uintptr_t)d->p;
        memcpy(CD(p)->p, &adr, 8);
        keep_(CD(p), "contents"_s, obj);
        return p;
    });
    mod_def(m, "byref", [](const CallArgs& a) {
        var obj = a.pos.at(0);
        CData* d = CD(obj);
        int64_t off = a.pos.size() > 1 ? a.pos[1].as_int64() : 0;
        var p = make_cdata(pointer_to(d->ct));
        u64 adr = (u64)(uintptr_t)d->p + (u64)off;
        memcpy(CD(p)->p, &adr, 8);
        keep_(CD(p), "contents"_s, obj);
        return p;
    });
    mod_def(m, "addressof", [](const CallArgs& a) { return var((int64_t)(uintptr_t)CD(a.pos.at(0))->p); });
    mod_def(m, "sizeof", [](const CallArgs& a) -> var {
        var x = a.pos.at(0);
        if (x.is_ref() && x.ref()->type->layout == Layout::Type) return var((int64_t)ctype_of(T(x))->size);
        return var((int64_t)CD(x)->ct->size);
    });
    mod_def(m, "alignment", [](const CallArgs& a) -> var {
        var x = a.pos.at(0);
        if (x.is_ref() && x.ref()->type->layout == Layout::Type) return var((int64_t)ctype_of(T(x))->align);
        return var((int64_t)CD(x)->ct->align);
    });
    mod_def(m, "cast", [](const CallArgs& a) {
        var obj = a.pos.at(0);
        CType* to = ctype_of_var(a.pos.at(1));
        var p = make_cdata(to);
        u64 adr = is_cdata(obj) ? cdata_address_for_pointer(CD(obj)) : pointer_bits(obj, CD(p), "cast"_s);
        if (is_cdata(obj)) keep_(CD(p), "cast"_s, obj);
        memcpy(CD(p)->p, &adr, 8);
        return p;
    });
    auto addr_of = [](const var& v) -> uint8_t* {
        if (is_int(v)) return (uint8_t*)(uintptr_t)int_bits(v);
        if (is_cdata(v)) return (uint8_t*)(uintptr_t)cdata_address_for_pointer(CD(v));
        if (is_bytes(v) || has_layout(v, Layout::ByteArray)) return (uint8_t*)as<BytesObj>(v)->s.data();
        if (v.is_none()) return nullptr;
        raise(E_TypeError, "an address is expected, not " + tname(v));
    };
    mod_def(m, "memmove", [addr_of](const CallArgs& a) {
        uint8_t* dst = addr_of(a.pos.at(0));
        uint8_t* src = addr_of(a.pos.at(1));
        memmove(dst, src, (size_t)a.pos.at(2).as_int64());
        return var((int64_t)(uintptr_t)dst);
    });
    mod_def(m, "memset", [addr_of](const CallArgs& a) {
        uint8_t* dst = addr_of(a.pos.at(0));
        memset(dst, (int)a.pos.at(1).as_int64(), (size_t)a.pos.at(2).as_int64());
        return var((int64_t)(uintptr_t)dst);
    });
    mod_def(m, "string_at", [addr_of](const CallArgs& a) {
        const char* p = (const char*)addr_of(a.pos.at(0));
        int64_t n = arg(a, 1, "size", var(-1)).as_int64();
        if (!p) raise(E_ValueError, "NULL pointer access");
        return new_bytes(n < 0 ? std::string(p) : std::string(p, (size_t)n));
    });
    mod_def(m, "wstring_at", [addr_of](const CallArgs& a) {
        const wchar_t* p = (const wchar_t*)addr_of(a.pos.at(0));
        int64_t n = arg(a, 1, "size", var(-1)).as_int64();
        return new_str(from_w(p, n < 0 ? wcslen(p) : (size_t)n));
    });
    mod_def(m, "create_string_buffer", [](const CallArgs& a) {
        var init = a.pos.at(0);
        size_t n;
        std::string data;
        if (is_int(init)) n = (size_t)init.as_int64();
        else {
            data = raw_of(init);
            n = a.pos.size() > 1 && !a.pos[1].is_none() ? (size_t)a.pos[1].as_int64() : data.size() + 1;
        }
        CType* c = types_()[T(import("ctypes").getattr("c_char"))];
        var b = make_cdata(array_of(c, n));
        memcpy(CD(b)->p, data.data(), std::min(n, data.size()));
        return b;
    });
    mod_def(m, "create_unicode_buffer", [](const CallArgs& a) {
        var init = a.pos.at(0);
        size_t n;
        std::wstring data;
        if (is_int(init)) n = (size_t)init.as_int64();
        else {
            data = to_w(str_utf8(init));
            n = a.pos.size() > 1 && !a.pos[1].is_none() ? (size_t)a.pos[1].as_int64() : data.size() + 1;
        }
        CType* c = types_()[T(import("ctypes").getattr("c_wchar"))];
        var b = make_cdata(array_of(c, n));
        memcpy(CD(b)->p, data.data(), std::min(n, data.size()) * 2);
        return b;
    });

    // arrays
    add_method(T_array, "__len__", [](const CallArgs& a) { return var((int64_t)CD(a.pos[0])->ct->length); });
    add_method(T_array, "__getitem__", [](const CallArgs& a) -> var {
        CData* d = CD(a.pos[0]);
        CType* e = d->ct->elem;
        int64_t n = (int64_t)d->ct->length;
        if (has_layout(a.pos.at(1), Layout::Slice)) {
            auto* sl = as<SliceObj>(a.pos[1]);
            int64_t lo, hi, st, cnt;
            slice_indices(sl->start, sl->stop, sl->step, n, lo, hi, st, cnt);
            std::vector<var> out;
            for (int64_t i = 0, k = lo; i < cnt; i++, k += st) out.push_back(read_item(e, d->p + k * (int64_t)e->size, a.pos[0]));
            if (e->kind == CK::Simple && e->code == 'c') { std::string s; for (auto& x : out) s += raw_of(x); return new_bytes(s); }
            if (e->kind == CK::Simple && e->code == 'u') { std::string s; for (auto& x : out) s += str_utf8(x); return new_str(s); }
            return new_list(out);
        }
        int64_t i = norm_index(a.pos[1].as_int64(), n, "invalid index");
        return read_item(e, d->p + i * (int64_t)e->size, a.pos[0]);
    });
    add_method(T_array, "__setitem__", [](const CallArgs& a) {
        CData* d = CD(a.pos[0]);
        CType* e = d->ct->elem;
        int64_t n = (int64_t)d->ct->length;
        if (has_layout(a.pos.at(1), Layout::Slice)) {
            auto* sl = as<SliceObj>(a.pos[1]);
            int64_t lo, hi, st, cnt;
            slice_indices(sl->start, sl->stop, sl->step, n, lo, hi, st, cnt);
            auto vals = collect(a.pos.at(2));
            if ((int64_t)vals.size() != cnt) raise(E_ValueError, "Can only assign sequence of same size");
            for (int64_t i = 0, k = lo; i < cnt; i++, k += st) write_value(e, d->p + k * (int64_t)e->size, vals[(size_t)i], d, var(k));
            return None;
        }
        int64_t i = norm_index(a.pos[1].as_int64(), n, "invalid index");
        write_value(e, d->p + i * (int64_t)e->size, a.pos.at(2), d, var(i));
        return None;
    });
    add_method(T_array, "__iter__", [](const CallArgs& a) {
        var self = a.pos[0];
        auto i = std::make_shared<int64_t>(0);
        return new_iter([self, i](var& out) -> bool {
            CData* d = CD(self);
            if (*i >= (int64_t)d->ct->length) return false;
            out = read_item(d->ct->elem, d->p + (*i)++ * (int64_t)d->ct->elem->size, self);
            return true;
        });
    });
    // c_char and c_wchar arrays: .value (to the first NUL) and .raw
    auto* av = new PropertyObj();
    av->type = T_property;
    av->name = "value";
    av->fget = new_native("value", [](const CallArgs& a) -> var {
        CData* d = CD(a.pos.at(0));
        if (d->ct->elem->code == 'c') return new_bytes(std::string((const char*)d->p, strnlen((const char*)d->p, d->ct->length)));
        if (d->ct->elem->code == 'u') return new_str(from_w((const wchar_t*)d->p, wcsnlen((const wchar_t*)d->p, d->ct->length)));
        raise(E_AttributeError, "'" + tname(a.pos[0]) + "' object has no attribute 'value'");
    });
    av->fset = new_native("value", [](const CallArgs& a) -> var {
        CData* d = CD(a.pos.at(0));
        if (d->ct->elem->code == 'c') {
            const std::string& b = raw_of(a.pos.at(1));
            if (b.size() > d->ct->length) raise(E_ValueError, "byte string too long");
            memcpy(d->p, b.data(), b.size());
            if (b.size() < d->ct->length) d->p[b.size()] = 0;
            return None;
        }
        std::wstring w = to_w(str_utf8(a.pos.at(1)));
        if (w.size() > d->ct->length) raise(E_ValueError, "string too long");
        memcpy(d->p, w.data(), w.size() * 2);
        if (w.size() < d->ct->length) ((wchar_t*)d->p)[w.size()] = 0;
        return None;
    });
    T_array->dict->set(interned("value"), var(static_cast<Obj*>(av)));
    auto* ar = new PropertyObj();
    ar->type = T_property;
    ar->name = "raw";
    ar->fget = new_native("raw", [](const CallArgs& a) { CData* d = CD(a.pos.at(0)); return new_bytes(std::string((const char*)d->p, d->ct->size)); });
    ar->fset = new_native("raw", [](const CallArgs& a) {
        CData* d = CD(a.pos.at(0));
        const std::string& b = raw_of(a.pos.at(1));
        if (b.size() > d->ct->size) raise(E_ValueError, "byte string too long");
        memcpy(d->p, b.data(), b.size());
        return None;
    });
    T_array->dict->set(interned("raw"), var(static_cast<Obj*>(ar)));

    // functions: a prototype type per CFUNCTYPE call; instances are callable
    auto make_proto = [](const CallArgs& a, const char* name) {
        TypeObj* t = new_ctype_class(name, T_funcptr);
        auto* ct = new CType();
        ct->kind = CK::Func;
        ct->size = 8;
        ct->align = 8;
        ct->py = t;
        var rt = a.pos.at(0);
        if (rt.is_none()) ct->res_void = true;
        else if (rt.is_ref() && rt.ref()->type->layout == Layout::Type && ctype_of(T(rt), false)) ct->res = ctype_of(T(rt));
        else ct->res_py = rt;
        for (size_t i = 1; i < a.pos.size(); i++) ct->args.push_back(ctype_of_var(a.pos[i]));
        types_()[t] = ct;
        return tv(t);
    };
    mod_def(m, "CFUNCTYPE", [make_proto](const CallArgs& a) { return make_proto(a, "CFunctionType"); });
    mod_def(m, "WINFUNCTYPE", [make_proto](const CallArgs& a) { return make_proto(a, "WinFunctionType"); });
    mod_def(m, "PYFUNCTYPE", [make_proto](const CallArgs& a) { return make_proto(a, "CFunctionType"); });
    add_method(T_funcptr, "__call__", [](const CallArgs& a) -> var {
        var self = a.pos.at(0);
        auto* f = static_cast<FuncPtr*>(self.ref());
        if (!f->addr) raise(E_ValueError, "NULL function pointer");
        CallArgs rest;
        rest.pos.assign(a.pos.begin() + 1, a.pos.end());
        if (!a.kw.empty()) raise(E_TypeError, "a C function takes no keyword arguments");
        var errcheck;
        if (self.ref()->attrs) if (var* e = static_cast<DictObj*>(self.ref()->attrs)->get("errcheck")) errcheck = *e;
        return do_call(f->addr, current_proto(self), rest, errcheck, self);
    });
    add_method(T_funcptr, "__repr__", [](const CallArgs& a) {
        auto* f = static_cast<FuncPtr*>(a.pos[0].ref());
        char buf[64];
        snprintf(buf, sizeof buf, " at 0x%llx>", (unsigned long long)(uintptr_t)f->addr);
        return new_str("<" + type_of(a.pos[0])->name + " object" + (f->name.empty() ? "" : " " + f->name) + buf);
    });
    add_method(T_funcptr, "__bool__", [](const CallArgs& a) { return var(static_cast<FuncPtr*>(a.pos[0].ref())->addr != nullptr); });

    // libraries: CDLL / WinDLL (one convention on x64), a function per attribute (kept: argtypes stay set)
    T_lib = new_type("CDLL", {}, Layout::NativeData, false, "ctypes");
    add_static(T_lib, "__new__", [](const CallArgs& a) { return load_library(T(a.pos.at(0)), arg(a, 1, "name", None)); });
    add_method(T_lib, "__init__", [](const CallArgs&) { return None; });
    add_method(T_lib, "__getattr__", [](const CallArgs& a) -> var {
        var self = a.pos[0];
        std::string n = str_utf8(a.pos.at(1));
        if (n.size() > 4 && n.compare(0, 2, "__") == 0) raise(E_AttributeError, n);
        Lib* l = static_cast<Lib*>(self.ref());
        void* f = (void*)GetProcAddress(l->h, n.c_str());
        if (!f) raise(E_AttributeError, "function '" + n + "' not found");
        var fn = new_funcptr(T_funcptr, f, n);
        self.setattr(n.c_str(), fn);
        return fn;
    });
    add_method(T_lib, "__getitem__", [](const CallArgs& a) -> var {
        Lib* l = static_cast<Lib*>(a.pos[0].ref());
        var k = a.pos.at(1);
        void* f = is_int(k) ? (void*)GetProcAddress(l->h, MAKEINTRESOURCEA((WORD)k.as_int64())) : (void*)GetProcAddress(l->h, str_utf8(k).c_str());
        if (!f) raise(E_AttributeError, "function " + repr_(k) + " not found");
        return new_funcptr(T_funcptr, f, is_int(k) ? "" : str_utf8(k));
    });
    add_getter(T_lib, "_handle", [](const var& s) { return var((int64_t)(uintptr_t)static_cast<Lib*>(s.ref())->h); });
    add_getter(T_lib, "_name", [](const var& s) { return new_str(static_cast<Lib*>(s.ref())->name); });
    add_method(T_lib, "__repr__", [](const CallArgs& a) {
        Lib* l = static_cast<Lib*>(a.pos[0].ref());
        char buf[64];
        snprintf(buf, sizeof buf, "', handle %llx at 0x0>", (unsigned long long)(uintptr_t)l->h);
        return new_str("<" + type_of(a.pos[0])->name + " '" + l->name + buf);
    });
    mod_set(m, "CDLL", tv(T_lib));
    var windll_t = make_class("WinDLL", "WinDLL", {tv(T_lib)}, new_dict(), {}, "ctypes");
    mod_set(m, "WinDLL", windll_t);
    mod_set(m, "OleDLL", windll_t);
    mod_set(m, "PyDLL", tv(T_lib));
    // cdll.name / windll.name: the library loaded on first use
    T_loader = new_type("LibraryLoader", {}, Layout::NativeData, false, "ctypes");
    add_method(T_loader, "__getattr__", [](const CallArgs& a) -> var {
        std::string n = str_utf8(a.pos.at(1));
        if (n.size() > 1 && n[0] == '_') raise(E_AttributeError, n);
        var lib = call1(a.pos[0].getattr("_dlltype"), a.pos[1]);
        a.pos[0].setattr(n.c_str(), lib);
        return lib;
    });
    add_method(T_loader, "LoadLibrary", [](const CallArgs& a) { return call1(a.pos[0].getattr("_dlltype"), a.pos.at(1)); });
    auto loader = [](const var& dlltype) {
        auto* o = new DataObj();
        o->type = T_loader;
        var v(static_cast<Obj*>(o));
        v.setattr("_dlltype", dlltype);
        return v;
    };
    mod_set(m, "cdll", loader(tv(T_lib)));
    mod_set(m, "windll", loader(windll_t));
    mod_set(m, "oledll", loader(windll_t));
    mod_set(m, "pydll", loader(tv(T_lib)));
    mod_def(m, "get_last_error", [](const CallArgs&) { return var((int64_t)t_last_error); });
    mod_def(m, "set_last_error", [](const CallArgs& a) { DWORD o = t_last_error; t_last_error = (DWORD)a.pos.at(0).as_int64(); return var((int64_t)o); });
    mod_def(m, "GetLastError", [](const CallArgs&) { return var((int64_t)t_last_error); });
    mod_def(m, "get_errno", [](const CallArgs&) { return var((int64_t)errno); });
    mod_def(m, "set_errno", [](const CallArgs& a) { int o = errno; errno = (int)a.pos.at(0).as_int64(); return var((int64_t)o); });
    mod_def(m, "FormatError", [](const CallArgs& a) {
        DWORD code = a.pos.empty() ? t_last_error : (DWORD)a.pos[0].as_int64();
        wchar_t* buf = nullptr;
        FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, code, 0, (LPWSTR)&buf, 0, nullptr);
        std::string s = buf ? from_w(buf, wcslen(buf)) : "<no description>";
        if (buf) LocalFree(buf);
        while (!s.empty() && (s.back() == '\n' || s.back() == '\r' || s.back() == ' ')) s.pop_back();
        return new_str(s);
    });
    mod_def(m, "WinError", [](const CallArgs& a) {
        DWORD code = a.pos.empty() || a.pos[0].is_none() ? t_last_error : (DWORD)a.pos[0].as_int64();
        var msg = a.pos.size() > 1 ? a.pos[1] : import("ctypes").call("FormatError", var((int64_t)code));
        return make_exc_args(E_OSError, {None, msg, None, var((int64_t)code)});
    });
    mod_set(m, "__version__", "1.1.0"_s);
}

void init_ctypes_util(const var& m) {
    mod_def(m, "find_library", [](const CallArgs& a) -> var {
        // a DLL by name on the search path, as ctypes.util's Windows find_library
        std::string n = str_utf8(a.pos.at(0));
        std::wstring w = to_w(n);
        if (n.size() < 4 || _stricmp(n.c_str() + n.size() - 4, ".dll") != 0) w += L".dll";
        wchar_t buf[MAX_PATH];
        if (SearchPathW(nullptr, w.c_str(), nullptr, MAX_PATH, buf, nullptr)) return new_str(from_w(buf, wcslen(buf)));
        return None;
    });
    mod_def(m, "find_msvcrt", [](const CallArgs&) { return None; });
}

void init_wintypes(const var& m) {
    var c = import("ctypes");
    auto g = [&](const char* n) { return c.getattr(n); };
    struct A { const char* name; const char* as; };
    const A al[] = {{"BYTE", "c_ubyte"}, {"WORD", "c_ushort"}, {"DWORD", "c_ulong"}, {"CHAR", "c_char"}, {"WCHAR", "c_wchar"},
                    {"UINT", "c_uint"}, {"INT", "c_int"}, {"DOUBLE", "c_double"}, {"FLOAT", "c_float"}, {"BOOLEAN", "c_byte"},
                    {"BOOL", "c_long"}, {"VARIANT_BOOL", "c_short"}, {"ULONG", "c_ulong"}, {"LONG", "c_long"}, {"USHORT", "c_ushort"},
                    {"SHORT", "c_short"}, {"_LARGE_INTEGER", "c_longlong"}, {"LARGE_INTEGER", "c_longlong"},
                    {"_ULARGE_INTEGER", "c_ulonglong"}, {"ULARGE_INTEGER", "c_ulonglong"}, {"LPCOLESTR", "c_wchar_p"},
                    {"LPOLESTR", "c_wchar_p"}, {"OLESTR", "c_wchar_p"}, {"LPCWSTR", "c_wchar_p"}, {"LPWSTR", "c_wchar_p"},
                    {"LPCSTR", "c_char_p"}, {"LPSTR", "c_char_p"}, {"LPCVOID", "c_void_p"}, {"LPVOID", "c_void_p"},
                    {"WPARAM", "c_ulonglong"}, {"LPARAM", "c_longlong"}, {"ATOM", "c_ushort"}, {"LANGID", "c_ushort"},
                    {"COLORREF", "c_ulong"}, {"LGRPID", "c_ulong"}, {"LCTYPE", "c_ulong"}, {"LCID", "c_ulong"},
                    {"HANDLE", "c_void_p"}, {"HACCEL", "c_void_p"}, {"HBITMAP", "c_void_p"}, {"HBRUSH", "c_void_p"},
                    {"HCOLORSPACE", "c_void_p"}, {"HDC", "c_void_p"}, {"HDESK", "c_void_p"}, {"HDWP", "c_void_p"},
                    {"HENHMETAFILE", "c_void_p"}, {"HFONT", "c_void_p"}, {"HGDIOBJ", "c_void_p"}, {"HGLOBAL", "c_void_p"},
                    {"HHOOK", "c_void_p"}, {"HICON", "c_void_p"}, {"HINSTANCE", "c_void_p"}, {"HKEY", "c_void_p"},
                    {"HKL", "c_void_p"}, {"HLOCAL", "c_void_p"}, {"HMENU", "c_void_p"}, {"HMETAFILE", "c_void_p"},
                    {"HMODULE", "c_void_p"}, {"HMONITOR", "c_void_p"}, {"HPALETTE", "c_void_p"}, {"HPEN", "c_void_p"},
                    {"HRGN", "c_void_p"}, {"HRSRC", "c_void_p"}, {"HSTR", "c_void_p"}, {"HTASK", "c_void_p"},
                    {"HWINSTA", "c_void_p"}, {"HWND", "c_void_p"}, {"SC_HANDLE", "c_void_p"}, {"SERVICE_STATUS_HANDLE", "c_void_p"}};
    for (auto& x : al) mod_set(m, x.name, g(x.as));
    mod_set(m, "MAX_PATH", var(260));
    var pointer = c.getattr("POINTER");
    for (auto& [n, base] : std::vector<std::pair<const char*, const char*>>{{"LPDWORD", "c_ulong"}, {"PDWORD", "c_ulong"}, {"LPBOOL", "c_long"},
                                                                         {"PBOOL", "c_long"}, {"LPBYTE", "c_ubyte"}, {"PBYTE", "c_ubyte"},
                                                                         {"LPWORD", "c_ushort"}, {"PWORD", "c_ushort"}, {"LPLONG", "c_long"},
                                                                         {"PLONG", "c_long"}, {"LPUINT", "c_uint"}, {"PUINT", "c_uint"},
                                                                         {"PULONG", "c_ulong"}, {"LPHANDLE", "c_void_p"}, {"PHANDLE", "c_void_p"}})
        mod_set(m, n, call1(pointer, g(base)));
    // the small structures (as wintypes defines them)
    auto st = [&](const char* name, std::vector<std::pair<const char*, const char*>> fields) {
        var ns = new_dict();
        ns.setitem("__module__"_s, "ctypes.wintypes"_s);
        std::vector<var> fl;
        for (auto& [fn, ft] : fields) fl.push_back(new_tuple({new_str(fn), g(ft)}));
        ns.setitem("_fields_"_s, new_list(fl));
        var cls = call_obj(tv(T_meta), CallArgs{{new_str(name), new_tuple({g("Structure")}), ns}, {}});
        mod_set(m, name, cls);
        return cls;
    };
    st("FILETIME", {{"dwLowDateTime", "c_ulong"}, {"dwHighDateTime", "c_ulong"}});
    st("POINT", {{"x", "c_long"}, {"y", "c_long"}});
    st("RECT", {{"left", "c_long"}, {"top", "c_long"}, {"right", "c_long"}, {"bottom", "c_long"}});
    st("SIZE", {{"cx", "c_long"}, {"cy", "c_long"}});
    st("SYSTEMTIME", {{"wYear", "c_ushort"}, {"wMonth", "c_ushort"}, {"wDayOfWeek", "c_ushort"}, {"wDay", "c_ushort"},
                      {"wHour", "c_ushort"}, {"wMinute", "c_ushort"}, {"wSecond", "c_ushort"}, {"wMilliseconds", "c_ushort"}});
}

}  // namespace py
