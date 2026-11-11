// cb_bridge.h -- what the question files of the executable-format readers (pe, ne, le, dos, omf, detect) share to
// answer from cb::formats (native/cb/formats, the one implementation): the call's bytes as a cb::ByteView, a path
// argument as a std::filesystem::path, and cb's values written into an answer.
//
// A cb::Error a reader throws crosses the interface as the Python exception that means the same (api.cc).
#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>

#include "cb/core/bytes.hpp"
#include "cb/core/fs.hpp"
#include "pyvalue.h"
#include "questions.h"

namespace cbf {

// The call's bytes, borrowed.
inline cb::ByteView view(const Bytes &b) { return cb::ByteView(b.data(), static_cast<size_t>(b.size())); }
// ... and copied, for the readers that own what they read.
inline cb::Bytes owned(const Bytes &b) { return cb::Bytes(b.begin(), b.end()); }
// A cb byte string as an argument's bytes would be.
inline Bytes borrowed(const cb::Bytes &b) { return Bytes(b.data(), b.size()); }
inline Bytes borrowed(cb::ByteView b) { return Bytes(b.data(), b.size()); }
// A str argument (UTF-8) as a path.
inline std::filesystem::path path_of(const std::string &utf8) { return cb::fs::from_text(utf8); }

inline void put(Out &o, cb::ByteView b) { o.bytes(borrowed(b)); }
inline void put(Out &o, const cb::Bytes &b) { o.bytes(borrowed(b)); }
template <class T>
void put_opt(Out &o, const std::optional<T> &v) {
    if (v) o.integer(static_cast<int64_t>(*v));
    else o.none();
}
inline void put_opt_text(Out &o, const std::optional<std::string> &s) {
    if (s) o.text(*s);
    else o.none();
}

// A dict argument's int under `key`: KeyError when it is missing, TypeError when it is not a dict.
inline int64_t key_int(const Value &d, const char *key) {
    if (d.kind != Value::Dict) raise("TypeError", "a dict is required");
    const Value *v = d.get(key);
    if (!v) raise("KeyError", std::string("'") + key + "'");
    return v->as_int();
}
inline const Value &need(const Value &d, const char *key) {
    if (d.kind != Value::Dict) raise("TypeError", "a dict is required");
    const Value *v = d.get(key);
    if (!v) raise("KeyError", std::string("'") + key + "'");
    return *v;
}
// d.get(key), None counting as absent
inline const Value *maybe(const Value &d, const char *key) {
    if (d.kind != Value::Dict) return nullptr;
    const Value *v = d.get(key);
    return v && !v->is_none() ? v : nullptr;
}
// A bytes value as cb bytes: TypeError for anything else.
inline cb::Bytes blob(const Value &v) {
    if (v.kind != Value::Bytes) raise("TypeError", "a bytes-like object is required");
    return cb::Bytes(v.s.begin(), v.s.end());
}
// t[k] of a tuple/list value: IndexError past its end.
inline const Value &item(const Value &v, size_t k) {
    if (k >= v.items.size()) raise("IndexError", "tuple index out of range");
    return v.items[k];
}

}  // namespace cbf
