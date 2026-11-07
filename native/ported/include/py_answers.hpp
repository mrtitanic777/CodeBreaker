// py_answers.hpp -- a native format's answer in typed C++: py::native::ask<R>(question, bytes, args...).
//
// codebreaker/formats/*.py asks formats.dll (native/formats) its questions -- "pe.export_table" of a
// file's bytes -- and the typed path (pyengine, answers.cc) translates such a method as
//
//     ExportTable PEFile::export_table() { return py::native::ask<ExportTable>("pe.export_table", this->data); }
//
// The call goes through formats.dll's one entry point, cbf_call (cbformats.h): the arguments go as a pickle
// (protocol 4: None, bool, int, str, bytes, tuple), the answer comes back as one, and is read here into R --
// py::str, int64_t, bool, double, py::bytes, py::list / set / dict, std::optional, std::tuple,
// std::variant, and records (a generated struct with for_each_field_mut). formats.dll is the one the
// program has loaded already (the Python side's), else CB_FORMATS_DLL's, else formats.dll on the path.
// What formats.dll raises is raised here as the Python reader raised it.
#pragma once

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <optional>
#include <string>
#include <tuple>
#include <type_traits>
#include <variant>
#include <vector>

#include "py.hpp"

// the three kernel32 calls this needs, declared here: <windows.h> would bring its macros (min, max, pascal,
// near, far ...) into every translated unit
#ifndef _WINDOWS_   // (else <windows.h> came first and declares them)
extern "C" {
__declspec(dllimport) void* __stdcall GetModuleHandleW(const wchar_t*);
__declspec(dllimport) void* __stdcall LoadLibraryW(const wchar_t*);
__declspec(dllimport) void* __stdcall GetProcAddress(void*, const char*);
}
#endif

namespace py::native {

// ------------------------------------------------------------------------------------------ values
struct Val {   // an answer, read from its pickle
    enum Kind { None, Bool, Int, Float, Str, Bytes, Tuple, List, Dict, Set } kind = None;
    int64_t i = 0;
    double f = 0;
    std::string s;                // Str (UTF-8), Bytes
    std::vector<Val> items;       // Tuple, List, Set; a Dict as key, value, key, value ...
    const Val* get(const char* key) const {
        if (kind != Dict) return nullptr;
        for (size_t k = 0; k + 1 < items.size(); k += 2)
            if (items[k].kind == Str && items[k].s == key) return &items[k + 1];
        return nullptr;
    }
};

[[noreturn]] inline void bad_answer(const std::string& why) { throw RuntimeError("formats.dll answer: " + why); }

// the pickle subset formats.dll writes (core/pyvalue.cc's Out)
inline Val read_pickle(const uint8_t* p, size_t n) {
    size_t at = 0;
    std::vector<Val> stack;
    std::vector<size_t> marks;
    std::vector<Val> memo;
    auto need = [&](size_t k) {
        if (at + k > n) bad_answer("cut short");
    };
    auto u = [&](int k) {
        need(k);
        uint64_t v = 0;
        for (int b = 0; b < k; b++) v |= uint64_t(p[at + b]) << (8 * b);
        at += k;
        return v;
    };
    auto text = [&](size_t len, Val::Kind kind) {
        need(len);
        Val v;
        v.kind = kind;
        v.s.assign(reinterpret_cast<const char*>(p + at), len);
        at += len;
        stack.push_back(std::move(v));
    };
    auto top = [&]() -> Val& {
        if (stack.empty()) bad_answer("stack empty");
        return stack.back();
    };
    auto pop_mark = [&]() {
        if (marks.empty()) bad_answer("no mark");
        size_t m = marks.back();
        marks.pop_back();
        std::vector<Val> got(std::make_move_iterator(stack.begin() + m), std::make_move_iterator(stack.end()));
        stack.resize(m);
        return got;
    };
    while (at < n) {
        uint8_t op = p[at++];
        switch (op) {
            case 0x80: at++; break;                 // PROTO
            case 0x95: at += 8; break;              // FRAME
            case '.': {
                if (stack.size() != 1) bad_answer("stack not one value at the end");
                return std::move(stack.back());
            }
            case 'N': stack.push_back(Val{}); break;
            case 0x88: case 0x89: { Val v; v.kind = Val::Bool; v.i = op == 0x88; stack.push_back(v); break; }
            case 'K': { Val v; v.kind = Val::Int; v.i = int64_t(u(1)); stack.push_back(v); break; }
            case 'M': { Val v; v.kind = Val::Int; v.i = int64_t(u(2)); stack.push_back(v); break; }
            case 'J': { Val v; v.kind = Val::Int; v.i = int64_t(int32_t(uint32_t(u(4)))); stack.push_back(v); break; }
            case 0x8a: {                            // LONG1: two's complement, little-endian
                size_t len = size_t(u(1));
                need(len);
                if (len > 8) bad_answer("an int past 64 bits");
                uint64_t v = 0;
                for (size_t b = 0; b < len; b++) v |= uint64_t(p[at + b]) << (8 * b);
                if (len && len < 8 && (p[at + len - 1] & 0x80)) v |= ~uint64_t(0) << (8 * len);
                at += len;
                Val x;
                x.kind = Val::Int;
                x.i = int64_t(v);
                stack.push_back(x);
                break;
            }
            case 'G': {                             // BINFLOAT: big-endian double
                need(8);
                uint64_t b = 0;
                for (int k = 0; k < 8; k++) b = (b << 8) | p[at + k];
                at += 8;
                Val v;
                v.kind = Val::Float;
                std::memcpy(&v.f, &b, 8);
                stack.push_back(v);
                break;
            }
            case 0x8c: text(size_t(u(1)), Val::Str); break;
            case 'X': text(size_t(u(4)), Val::Str); break;
            case 0x8d: text(size_t(u(8)), Val::Str); break;
            case 'C': text(size_t(u(1)), Val::Bytes); break;
            case 'B': text(size_t(u(4)), Val::Bytes); break;
            case 0x8e: text(size_t(u(8)), Val::Bytes); break;
            case ')': { Val v; v.kind = Val::Tuple; stack.push_back(v); break; }
            case 0x85: case 0x86: case 0x87: {      // TUPLE1..3
                size_t k = size_t(op - 0x84);
                if (stack.size() < k) bad_answer("tuple of too few");
                Val v;
                v.kind = Val::Tuple;
                v.items.assign(std::make_move_iterator(stack.end() - k), std::make_move_iterator(stack.end()));
                stack.resize(stack.size() - k);
                stack.push_back(std::move(v));
                break;
            }
            case '(': marks.push_back(stack.size()); break;
            case 't': { Val v; v.kind = Val::Tuple; v.items = pop_mark(); stack.push_back(std::move(v)); break; }
            case ']': { Val v; v.kind = Val::List; stack.push_back(v); break; }
            case '}': { Val v; v.kind = Val::Dict; stack.push_back(v); break; }
            case 0x8f: { Val v; v.kind = Val::Set; stack.push_back(v); break; }
            case 'a': { Val x = std::move(top()); stack.pop_back(); top().items.push_back(std::move(x)); break; }
            case 'e': case 'u': case 0x90: {        // APPENDS, SETITEMS, ADDITEMS
                std::vector<Val> got = pop_mark();
                Val& c = top();
                for (auto& x : got) c.items.push_back(std::move(x));
                break;
            }
            case 's': {
                Val v = std::move(top()); stack.pop_back();
                Val k = std::move(top()); stack.pop_back();
                top().items.push_back(std::move(k));
                top().items.push_back(std::move(v));
                break;
            }
            case 0x91: { Val v; v.kind = Val::Set; v.items = pop_mark(); stack.push_back(std::move(v)); break; }   // FROZENSET
            case 0x94: memo.push_back(top()); break;                       // MEMOIZE
            case 'q': at += 1; break;                                     // BINPUT (the memo is MEMOIZE's)
            case 'r': at += 4; break;                                     // LONG_BINPUT
            case 'h': { size_t k = size_t(u(1)); if (k >= memo.size()) bad_answer("memo"); stack.push_back(memo[k]); break; }
            case 'j': { size_t k = size_t(u(4)); if (k >= memo.size()) bad_answer("memo"); stack.push_back(memo[k]); break; }
            default: bad_answer("pickle opcode " + std::to_string(op));
        }
    }
    bad_answer("no STOP");
}

// ------------------------------------------------------------------------------------------ arguments

struct Args {   // a tuple pickle of the call's arguments
    std::string b = std::string("\x80\x04(", 3);
    void op(char c) { b += c; }
    void le(uint64_t v, int k) {
        for (int i = 0; i < k; i++) b += char((v >> (8 * i)) & 0xFF);
    }
    void put(std::nullopt_t) { op('N'); }
    void put(bool v) { op(v ? char(0x88) : char(0x89)); }
    void put(int64_t v) {
        if (v >= INT32_MIN && v <= INT32_MAX) { op('J'); le(uint64_t(uint32_t(int32_t(v))), 4); }
        else { op(char(0x8a)); b += char(8); le(uint64_t(v), 8); }
    }
    void put(int v) { put(int64_t(v)); }
    void put(const std::string& utf8) { op('X'); le(utf8.size(), 4); b += utf8; }
    void put(const str& s) { put(s.utf8()); }
    void put(const bytes& v) { op('B'); le(v.raw().size(), 4); b += v.raw(); }
    template <class T> void put(const std::optional<T>& v) {
        if (v) put(*v);
        else op('N');
    }
    template <class T> void put(const list<T>& xs) {
        op(']');
        op('(');
        for (const auto& x : xs) put(x);
        op('e');
    }
    template <class... T> void put(const std::tuple<T...>& t) {
        op('(');
        std::apply([&](const auto&... x) { (put(x), ...); }, t);
        op('t');
    }
    template <class K, class V> void put(const dict<K, V>& d) {
        op('}');
        op('(');
        for (const auto& k : d.keys()) {
            put(k);
            put(d.at(k));
        }
        op('u');
    }
    // a record: the dict the Python side passes (a field it may lack, absent, is left out)
    template <class T, std::enable_if_t<is_record<T>::value, int> = 0> void put(const T& r) {
        op('}');
        op('(');
        for_each_field_mut(const_cast<T&>(r), [&](const char* name, auto& field, bool required) {
            using F = std::decay_t<decltype(field)>;
            if constexpr (is_optional_<F>::value)
                if (!required && !field) return;
            put(std::string(name));
            put(field);
        });
        op('u');
    }
    template <class T> struct is_optional_ : std::false_type {};
    template <class T> struct is_optional_<std::optional<T>> : std::true_type {};
    std::string done() { return b + "t."; }
};

// ------------------------------------------------------------------------------------------ the answer read as R

template <class R> R read(const Val& v);

template <class T> struct reader {
    static T get(const Val& v) {
        if constexpr (std::is_same_v<T, bool>) {
            if (v.kind != Val::Bool && v.kind != Val::Int) bad_answer("not a bool");
            return v.i != 0;
        } else if constexpr (std::is_integral_v<T>) {
            if (v.kind != Val::Int && v.kind != Val::Bool) bad_answer("not an int");
            return T(v.i);
        } else if constexpr (std::is_floating_point_v<T>) {
            if (v.kind == Val::Int) return T(v.i);
            if (v.kind != Val::Float) bad_answer("not a float");
            return T(v.f);
        } else if constexpr (is_record<T>::value) {
            if (v.kind != Val::Dict) bad_answer("not a record");
            T r{};
            for_each_field_mut(r, [&](const char* name, auto& field, bool required) {
                const Val* x = v.get(name);
                using F = std::decay_t<decltype(field)>;
                if (!x) {
                    if (required) bad_answer(std::string("a record without its field ") + name);
                    return;
                }
                field = read<F>(*x);
            });
            return r;
        } else {
            static_assert(sizeof(T) == 0, "py::native::ask: no reading of this type");
        }
    }
};
template <> struct reader<str> {
    static str get(const Val& v) {
        if (v.kind != Val::Str) bad_answer("not a str");
        return str(v.s);
    }
};
template <> struct reader<bytes> {
    static bytes get(const Val& v) {
        if (v.kind != Val::Bytes) bad_answer("not bytes");
        return bytes(v.s);
    }
};
template <class T> struct reader<std::optional<T>> {
    static std::optional<T> get(const Val& v) {
        if (v.kind == Val::None) return std::nullopt;
        return read<T>(v);
    }
};
template <class T> struct reader<list<T>> {
    static list<T> get(const Val& v) {
        if (v.kind != Val::List && v.kind != Val::Tuple) bad_answer("not a list");
        list<T> out;
        for (auto& x : v.items) out.append(read<T>(x));
        return out;
    }
};
template <class T> struct reader<set<T>> {
    static set<T> get(const Val& v) {
        if (v.kind != Val::Set && v.kind != Val::List) bad_answer("not a set");
        set<T> out;
        for (auto& x : v.items) out.add(read<T>(x));
        return out;
    }
};
template <class K, class V> struct reader<dict<K, V>> {
    static dict<K, V> get(const Val& v) {
        if (v.kind != Val::Dict) bad_answer("not a dict");
        dict<K, V> out;
        for (size_t k = 0; k + 1 < v.items.size(); k += 2) out[read<K>(v.items[k])] = read<V>(v.items[k + 1]);
        return out;
    }
};
template <class... A> struct reader<std::tuple<A...>> {
    static std::tuple<A...> get(const Val& v) {
        if (v.kind != Val::Tuple || v.items.size() != sizeof...(A)) bad_answer("not a tuple of " + std::to_string(sizeof...(A)));
        return at(v, std::index_sequence_for<A...>{});
    }
    template <size_t... I> static std::tuple<A...> at(const Val& v, std::index_sequence<I...>) { return {read<A>(v.items[I])...}; }
};
template <class... A> struct reader<std::variant<A...>> {   // the alternative the value's kind is
    static std::variant<A...> get(const Val& v) {
        std::optional<std::variant<A...>> out;
        (try_one<A>(v, out), ...);
        if (!out) bad_answer("none of a variant's kinds");
        return *out;
    }
    template <class T> static void try_one(const Val& v, std::optional<std::variant<A...>>& out) {
        if (out) return;
        bool fits = false;
        if constexpr (std::is_same_v<T, str>) fits = v.kind == Val::Str;
        else if constexpr (std::is_same_v<T, bytes>) fits = v.kind == Val::Bytes;
        else if constexpr (std::is_same_v<T, bool>) fits = v.kind == Val::Bool;
        else if constexpr (std::is_integral_v<T>) fits = v.kind == Val::Int;
        else if constexpr (std::is_floating_point_v<T>) fits = v.kind == Val::Float;
        else fits = true;
        if (fits) out = read<T>(v);
    }
};
template <class R> R read(const Val& v) { return reader<R>::get(v); }

// ------------------------------------------------------------------------------------------ the call
using cbf_call_fn = int (*)(const char*, const uint8_t*, size_t, const uint8_t*, size_t, const uint8_t**, size_t*);
inline cbf_call_fn entry() {
    static cbf_call_fn fn = [] {
        void* m = GetModuleHandleW(L"formats.dll");
        if (!m) {
            const wchar_t* env = _wgetenv(L"CB_FORMATS_DLL");
            m = LoadLibraryW(env && *env ? env : L"formats.dll");
        }
        if (!m) throw RuntimeError("formats.dll is required: not loaded (CB_FORMATS_DLL names it)");
        auto f = reinterpret_cast<cbf_call_fn>(GetProcAddress(m, "cbf_call"));
        if (!f) throw RuntimeError("formats.dll has no cbf_call");
        return f;
    }();
    return fn;
}

[[noreturn]] inline void raise_as(const Val& e) {   // (name, message) as the Python reader raised it
    std::string name = e.items.size() > 0 ? e.items[0].s : "RuntimeError", msg = e.items.size() > 1 ? e.items[1].s : "";
    if (name == "ValueError") throw ValueError(msg);
    if (name == "KeyError") throw KeyError(msg);
    if (name == "IndexError") throw IndexError(msg);
    if (name == "TypeError") throw TypeError(msg);
    if (name == "struct.error") throw StructError(msg);
    if (name == "OverflowError") throw OverflowError(msg);
    if (name == "ZeroDivisionError") throw ZeroDivisionError(msg);
    if (name == "UnicodeDecodeError") throw UnicodeDecodeError(msg);
    if (name == "FileNotFoundError") throw FileNotFoundError(msg);
    if (name == "OSError") throw OSError(msg);
    if (name == "EOFError") throw EOFError(msg);
    if (name == "AttributeError") throw AttributeError(msg);
    if (name == "NotImplementedError") throw NotImplementedError(msg);
    if (name == "UnicodeEncodeError") throw UnicodeEncodeError(msg);
    if (name == "PermissionError") throw PermissionError(msg);
    if (name == "AssertionError") throw AssertionError(msg);
    throw RuntimeError(name + ": " + msg);
}

template <class R> R answer(const char* question, const bytes& data, const std::string& blob);

template <class R, class... A> R ask(const char* question, const bytes& data, const A&... args) {
    Args a;
    (a.put(args), ...);
    return answer<R>(question, data, a.done());
}

// ask(q, where): the place the reader's where(self) gives -- a tuple -- goes before the call's own arguments
template <class R, class... W, class... A> R ask_where(const char* question, const bytes& data, const std::tuple<W...>& place, const A&... args) {
    Args a;
    std::apply([&](const W&... w) { (a.put(w), ...); }, place);
    (a.put(args), ...);
    return answer<R>(question, data, a.done());
}

template <class R> R answer(const char* question, const bytes& data, const std::string& blob) {
    const uint8_t* out = nullptr;
    size_t n = 0;
    const std::string& raw = data.raw();
    int status = entry()(question, reinterpret_cast<const uint8_t*>(raw.data()), raw.size(),
                         reinterpret_cast<const uint8_t*>(blob.data()), blob.size(), &out, &n);
    Val v = read_pickle(out, n);
    if (status == 1) raise_as(v);
    if (status != 0) throw RuntimeError(std::string("formats.dll does not answer ") + question + ": " + (v.kind == Val::Str ? v.s : ""));
    if constexpr (std::is_void_v<R>) return;
    else return read<R>(v);
}

}  // namespace py::native
