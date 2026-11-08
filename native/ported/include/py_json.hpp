// py_json.hpp -- py.hpp values as JSON (what Python's json.dumps writes for the same values), for the
// gates and tests that compare a port's results with the Python's.
#pragma once

#include <algorithm>
#include <cstdio>
#include <string>
#include <type_traits>
#include <typeinfo>

#include "py.hpp"

namespace py {
namespace json {

inline void put(std::string& o, int64_t v) { o += std::to_string(v); }
inline void put(std::string& o, int v) { o += std::to_string(v); }
inline void put(std::string& o, bool v) { o += v ? "true" : "false"; }
inline void put(std::string& o, double v) { o += to_str(v).utf8(); }
inline void put(std::string& o, none_t) { o += "null"; }
inline void put(std::string& o, const str& s) {
    o += '"';
    for (char32_t c : s.u32()) {
        if (c == '"') o += "\\\"";
        else if (c == '\\') o += "\\\\";
        else if (c == '\n') o += "\\n";   // (as json.dumps: the short escapes for these five)
        else if (c == '\r') o += "\\r";
        else if (c == '\t') o += "\\t";
        else if (c == '\b') o += "\\b";
        else if (c == '\f') o += "\\f";
        else if (c >= 0x20 && c < 0x7F) o += static_cast<char>(c);
        else if (c < 0x10000) {
            char b[8];
            std::snprintf(b, sizeof b, "\\u%04x", static_cast<unsigned>(c));
            o += b;
        } else {
            unsigned v = static_cast<unsigned>(c) - 0x10000;
            char b[16];
            std::snprintf(b, sizeof b, "\\u%04x\\u%04x", 0xD800 + (v >> 10), 0xDC00 + (v & 0x3FF));
            o += b;
        }
    }
    o += '"';
}
// bytes have no JSON form; the tests write them as their latin-1 text
inline void put(std::string& o, const bytes& b) { put(o, b.decode("latin1")); }
template <class T> void put(std::string& o, const std::optional<T>& v);
template <class T> void put(std::string& o, const list<T>& v);
template <class K, class V> void put(std::string& o, const dict<K, V>& v);
template <class... T> void put(std::string& o, const std::tuple<T...>& t);
template <class T> void put(std::string& o, const std::vector<T>& v);
template <class T> void put(std::string& o, const set<T>& v);
template <class... A> void put(std::string& o, const std::variant<A...>& v) {   // (a variant is its value)
    std::visit([&](const auto& x) { put(o, x); }, v);
}
template <class R, class = decltype(for_each_field(std::declval<const R&>(), std::declval<void (*)(const char*, int)>()))>
void put(std::string& o, const R& r);
template <class R> void put(std::string& o, const std::shared_ptr<R>& r) {
    if (r) put(o, *r);
    else o += "null";
}
template <class T> void put(std::string& o, const std::optional<T>& v) {
    if (v) put(o, *v);
    else o += "null";
}
template <class L> void put_seq_(std::string& o, const L& v) {
    o += '[';
    bool first = true;
    for (const auto& x : v) {
        if (!first) o += ", ";
        put(o, x);
        first = false;
    }
    o += ']';
}
template <class T> void put(std::string& o, const list<T>& v) { put_seq_(o, v); }
template <class T> void put(std::string& o, const std::vector<T>& v) { put_seq_(o, v); }
// a set has no JSON form or order; the tests write it as a list ordered by repr()
template <class T> void put(std::string& o, const set<T>& s) {
    std::vector<std::pair<std::string, T>> v;
    for (const auto& x : s) v.emplace_back(repr(x).utf8(), x);
    std::sort(v.begin(), v.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
    o += '[';
    for (size_t i = 0; i < v.size(); i++) {
        if (i) o += ", ";
        put(o, v[i].second);
    }
    o += ']';
}
template <class K, class V> void put(std::string& o, const dict<K, V>& d) {  // keys as JSON keys: text
    o += '{';
    bool first = true;
    for (const auto& kv : d.items()) {
        if (!first) o += ", ";
        if constexpr (std::is_same_v<K, str>) put(o, std::get<0>(kv));
        else put(o, to_str(std::get<0>(kv)));
        o += ": ";
        put(o, std::get<1>(kv));
        first = false;
    }
    o += '}';
}
template <class... T> void put(std::string& o, const std::tuple<T...>& t) {
    o += '[';
    size_t i = 0;
    std::apply([&](const auto&... x) { ((o += (i++ ? ", " : ""), put(o, x)), ...); }, t);
    o += ']';
}
template <class R, class> void put(std::string& o, const R& r) {
    o += '{';
    bool first = true;
    for_each_field(r, [&](const char* k, const auto& v) {
        if (!first) o += ", ";
        o += "\""; o += k; o += "\"";
        o += ": ";
        put(o, v);
        first = false;
    });
    o += '}';
}

// the value of f(), or {"error": "<Python exception name>"}
template <class F> std::string attempt(F&& f) {
    std::string v;
    try {
        f(v);
    } catch (const Exception& e) {
        v = std::string("{\"error\": \"") + e.type() + "\"}";
    } catch (const std::bad_optional_access&) {
        v = "{\"error\": \"KeyError\"}";
    } catch (const std::exception& e) {
        v = std::string("{\"error\": \"C++ ") + typeid(e).name() + "\"}";
    }
    return v;
}

}  // namespace json
}  // namespace py
