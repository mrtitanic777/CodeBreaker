// lib_array.cc -- the array module (array.array): a typed sequence of numbers kept as their machine values,
// as CPython's. fontTools' tables are read and written through it (byteswap, frombytes, tobytes). The type
// codes and item sizes are Windows' (l and L are 4 bytes); values are checked on the way in as CPython
// checks them (OverflowError out of range, TypeError for a float where an integer goes).
#include "rt.hpp"

#include <cmath>
#include <cstring>

namespace py {

namespace {

struct Code {
    char c;
    int size;
    bool is_float, is_signed;
};
const Code* code_of(const std::string& tc) {
    static const Code codes[] = {{'b', 1, false, true},  {'B', 1, false, false}, {'h', 2, false, true}, {'H', 2, false, false},
                                 {'i', 4, false, true},  {'I', 4, false, false}, {'l', 4, false, true}, {'L', 4, false, false},
                                 {'q', 8, false, true},  {'Q', 8, false, false}, {'f', 4, true, true},  {'d', 8, true, true}};
    if (tc.size() == 1)
        for (auto& c : codes)
            if (c.c == tc[0]) return &c;
    raise(E_ValueError, "bad typecode (must be b, B, u, w, h, H, i, I, l, L, q, Q, f or d)");
}

// an item as the array keeps it: checked against its type's range
var checked(const Code* c, const var& v) {
    if (c->is_float) {
        if (!is_int(v) && !v.is_float()) raise(E_TypeError, "must be real number, not " + T(type(v))->name);
        return var(v.as_double());
    }
    if (!is_int(v)) raise(E_TypeError, "'" + T(type(v))->name + "' object cannot be interpreted as an integer");
    if (c->size == 8 && !c->is_signed) {   // Q: 0 .. 2**64-1 (past int64)
        if (lt(v, var(0)).truthy() || lt(var(0xFFFFFFFFFFFFFFFFULL), v).truthy())
            raise(E_OverflowError, "unsigned long long is greater than maximum");
        return v;
    }
    if (c->size == 8) {
        if (!v.is_small_int()) raise(E_OverflowError, "Python int too large to convert to C long long");
        return v;
    }
    if (!v.is_small_int()) raise(E_OverflowError, "Python int too large to convert to C long");
    int64_t x = v.as_int64();
    int64_t lo = c->is_signed ? -(int64_t(1) << (c->size * 8 - 1)) : 0;
    int64_t hi = c->is_signed ? (int64_t(1) << (c->size * 8 - 1)) - 1 : (int64_t(1) << (c->size * 8)) - 1;
    if (x < lo || x > hi) {   // (CPython's words, per type)
        bool low = x < lo;
        switch (c->c) {
            case 'b': raise(E_OverflowError, low ? "signed char is less than minimum" : "signed char is greater than maximum");
            case 'B': raise(E_OverflowError, low ? "unsigned byte integer is less than minimum" : "unsigned byte integer is greater than maximum");
            case 'h': raise(E_OverflowError, low ? "signed short integer is less than minimum" : "signed short integer is greater than maximum");
            case 'H': raise(E_OverflowError, low ? "unsigned short is less than minimum" : "unsigned short is greater than maximum");
            case 'i': case 'l': raise(E_OverflowError, "Python int too large to convert to C long");
            default: raise(E_OverflowError, low ? "can't convert negative value to unsigned int" : "Python int too large to convert to C unsigned long");
        }
    }
    return v;
}

std::string pack(const Code* c, const std::vector<var>& items) {
    std::string out(items.size() * c->size, '\0');
    char* p = out.data();
    for (auto& v : items) {
        if (c->c == 'f') { float f = (float)v.as_double(); memcpy(p, &f, 4); }
        else if (c->c == 'd') { double d = v.as_double(); memcpy(p, &d, 8); }
        else if (c->c == 'Q') {
            uint64_t u = v.is_small_int() ? (uint64_t)v.as_int64() : std::stoull(str_utf8(str(v)));
            memcpy(p, &u, 8);
        } else { int64_t x = v.as_int64(); memcpy(p, &x, c->size); }   // (little-endian: the low bytes)
        p += c->size;
    }
    return out;
}

std::vector<var> unpack(const Code* c, const std::string& raw) {
    if (raw.size() % c->size) raise(E_ValueError, "bytes length not a multiple of item size");
    std::vector<var> out;
    out.reserve(raw.size() / c->size);
    for (size_t at = 0; at < raw.size(); at += c->size) {
        const char* p = raw.data() + at;
        switch (c->c) {
            case 'b': out.push_back(var((int64_t)(int8_t)p[0])); break;
            case 'B': out.push_back(var((int64_t)(uint8_t)p[0])); break;
            case 'h': { int16_t x; memcpy(&x, p, 2); out.push_back(var((int64_t)x)); break; }
            case 'H': { uint16_t x; memcpy(&x, p, 2); out.push_back(var((int64_t)x)); break; }
            case 'i': case 'l': { int32_t x; memcpy(&x, p, 4); out.push_back(var((int64_t)x)); break; }
            case 'I': case 'L': { uint32_t x; memcpy(&x, p, 4); out.push_back(var((int64_t)x)); break; }
            case 'q': { int64_t x; memcpy(&x, p, 8); out.push_back(var(x)); break; }
            case 'Q': { uint64_t x; memcpy(&x, p, 8); out.push_back(var((unsigned long long)x)); break; }
            case 'f': { float x; memcpy(&x, p, 4); out.push_back(var((double)x)); break; }
            case 'd': { double x; memcpy(&x, p, 8); out.push_back(var(x)); break; }
        }
    }
    return out;
}

const Code* code(const var& self) { return code_of(str_utf8(self.getattr("typecode"))); }
var items(const var& self) { return self.getattr("_items"); }
std::vector<var>& vec(const var& self) { return list_items(items(self)); }

var g_array;   // the class

var make(const var& typecode, std::vector<var> its) {
    var a = call2(g_array, typecode, None);
    list_items(items(a)) = std::move(its);
    return a;
}

// the items an initializer or extend() argument gives
std::vector<var> items_of(const Code* c, const var& src) {
    if (is_bytes(src)) return unpack(c, bytes_raw(src));
    if (isinstance(src, g_array).truthy()) {
        if (str_utf8(src.getattr("typecode"))[0] != c->c && !(code(src)->is_float && c->is_float))
            raise(E_TypeError, "array.array() argument 1 must be a unicode character, not array");
        return vec(src);
    }
    if (is_str(src)) raise(E_TypeError, "cannot use a str to initialize an array with typecode '" + std::string(1, c->c) + "'");
    std::vector<var> out;
    for (var v : each(src)) out.push_back(checked(c, v));
    return out;
}

void init_array(const var& m) {
    var ns = new_dict();
    ns.setitem("__module__"_s, "array"_s);
    ns.setitem("__init__"_s, def("__init__", "self, typecode, initializer=, /", {None}, [](var self, var tc, var init) -> var {
        if (!is_str(tc)) raise(E_TypeError, "array() argument 1 must be a unicode character, not " + T(type(tc))->name);
        const Code* c = code_of(str_utf8(tc));
        self.setattr("typecode", tc);
        self.setattr("itemsize", var((int64_t)c->size));
        self.setattr("_items", new_list(init.is_none() ? std::vector<var>{} : items_of(c, init)));
        return var(None);
    }));
    ns.setitem("tobytes"_s, def("tobytes", "self", {}, [](var self) -> var { return new_bytes(pack(code(self), vec(self))); }));
    ns.setitem("frombytes"_s, def("frombytes", "self, buffer, /", {}, [](var self, var b) -> var {
        if (!is_bytes(b)) b = call1(t::bytes, b);
        auto more = unpack(code(self), bytes_raw(b));
        auto& v = vec(self);
        v.insert(v.end(), more.begin(), more.end());
        return var(None);
    }));
    ns.setitem("byteswap"_s, def("byteswap", "self", {}, [](var self) -> var {
        const Code* c = code(self);
        std::string raw = pack(c, vec(self));
        for (size_t at = 0; at < raw.size(); at += c->size) std::reverse(raw.begin() + at, raw.begin() + at + c->size);
        vec(self) = unpack(c, raw);
        return var(None);
    }));
    ns.setitem("tolist"_s, def("tolist", "self", {}, [](var self) -> var { return new_list(vec(self)); }));
    ns.setitem("fromlist"_s, def("fromlist", "self, list, /", {}, [](var self, var l) -> var {
        if (!is_list(l)) raise(E_TypeError, "arg must be list");
        auto more = items_of(code(self), l);
        auto& v = vec(self);
        v.insert(v.end(), more.begin(), more.end());
        return var(None);
    }));
    ns.setitem("append"_s, def("append", "self, v, /", {}, [](var self, var x) -> var {
        vec(self).push_back(checked(code(self), x));
        return var(None);
    }));
    ns.setitem("extend"_s, def("extend", "self, bb, /", {}, [](var self, var src) -> var {
        if (is_bytes(src)) raise(E_TypeError, "can only extend with array of same kind");
        auto more = items_of(code(self), src);
        auto& v = vec(self);
        v.insert(v.end(), more.begin(), more.end());
        return var(None);
    }));
    ns.setitem("insert"_s, def("insert", "self, i, v, /", {}, [](var self, var i, var x) -> var {
        items(self).call("insert", i, checked(code(self), x));
        return var(None);
    }));
    for (const char* name : {"pop", "index", "count", "remove", "reverse", "__len__", "__iter__", "__contains__", "__reversed__"})
        ns.setitem(new_str(name), new_native(name, [name](const CallArgs& a) -> var {
            CallArgs rest;
            rest.pos.assign(a.pos.begin() + 1, a.pos.end());
            return call_obj(items(a.pos.at(0)).getattr(name), rest);
        }));
    ns.setitem("__getitem__"_s, def("__getitem__", "self, i", {}, [](var self, var i) -> var {
        var r = items(self).getitem(i);
        if (isinstance(i, t::slice).truthy()) return make(self.getattr("typecode"), collect(r));
        return r;
    }));
    ns.setitem("__setitem__"_s, def("__setitem__", "self, i, v", {}, [](var self, var i, var x) -> var {
        const Code* c = code(self);
        if (isinstance(i, t::slice).truthy()) {
            if (!isinstance(x, g_array).truthy()) raise(E_TypeError, "can only assign array (not \"" + T(type(x))->name + "\") to array slice");
            items(self).setitem(i, new_list(items_of(c, x)));
        } else items(self).setitem(i, checked(c, x));
        return var(None);
    }));
    ns.setitem("__delitem__"_s, def("__delitem__", "self, i", {}, [](var self, var i) -> var {
        items(self).delitem(i);
        return var(None);
    }));
    ns.setitem("__eq__"_s, def("__eq__", "self, other", {}, [](var self, var o) -> var {
        if (!isinstance(o, g_array).truthy()) return NotImplemented;
        return eq(items(self), items(o));
    }));
    ns.setitem("__ne__"_s, def("__ne__", "self, other", {}, [](var self, var o) -> var {
        if (!isinstance(o, g_array).truthy()) return NotImplemented;
        return var(!eq(items(self), items(o)).truthy());
    }));
    ns.setitem("__add__"_s, def("__add__", "self, other", {}, [](var self, var o) -> var {
        if (!isinstance(o, g_array).truthy()) raise(E_TypeError, "can only append array (not \"" + T(type(o))->name + "\") to array");
        std::vector<var> v = vec(self);
        auto more = items_of(code(self), o);
        v.insert(v.end(), more.begin(), more.end());
        return make(self.getattr("typecode"), v);
    }));
    ns.setitem("__iadd__"_s, def("__iadd__", "self, other", {}, [](var self, var o) -> var {
        self.call("extend", o);
        return self;
    }));
    ns.setitem("__mul__"_s, def("__mul__", "self, n", {}, [](var self, var n) -> var {
        return make(self.getattr("typecode"), collect(items(self) * n));
    }));
    ns.setitem("__copy__"_s, def("__copy__", "self", {}, [](var self) -> var { return make(self.getattr("typecode"), vec(self)); }));
    ns.setitem("__deepcopy__"_s, def("__deepcopy__", "self, memo", {}, [](var self, var) -> var { return make(self.getattr("typecode"), vec(self)); }));
    ns.setitem("__reduce_ex__"_s, def("__reduce_ex__", "self, protocol", {}, [](var self, var) -> var {
        return new_tuple({g_array, new_tuple({self.getattr("typecode"), new_list(vec(self))})});
    }));
    ns.setitem("__repr__"_s, def("__repr__", "self", {}, [](var self) -> var {
        std::string tc = str_utf8(self.getattr("typecode"));
        if (vec(self).empty()) return new_str("array('" + tc + "')");
        return new_str("array('" + tc + "', " + str_utf8(repr(items(self))) + ")");
    }));
    ns.setitem("buffer_info"_s, def("buffer_info", "self", {}, [](var self) -> var {
        return new_tuple({var((int64_t)0), var((int64_t)vec(self).size())});
    }));
    ns.setitem("__hash__"_s, None);
    g_array = make_class("array", "array", {}, ns, {}, "array");
    mod_set(m, "array", g_array);
    mod_set(m, "ArrayType", g_array);
    mod_set(m, "typecodes", "bBuwhHiIlLqQfd"_s);
}

}  // namespace

void init_array_module() { register_native_module("array", init_array); }

}  // namespace py
