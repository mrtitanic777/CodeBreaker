// pyval.h -- a Python value as the models build them (None, bool, int, str, tuple, list, dict), with
// Python's indexing (IndexError, TypeError), dict.get / d[k] (KeyError) and == (1 == True, a tuple
// never equal to a list), and its JSON form for the shim:
//   None null, bool, int, str as themselves; a tuple a JSON array; a list {"L": [...]}; a dict
//   {"D": [[key, value], ...]} in insertion order.
#pragma once

#include <initializer_list>
#include <string>
#include <vector>

#include "pyutil.h"

namespace cbinv {

struct V {
  enum K : unsigned char { NONE, BOOL, INT, STR, TUP, LIST, DICT };
  K k = NONE;
  long long i = 0;
  std::string s;
  std::vector<V> a;               // TUP / LIST items; DICT values
  std::vector<std::string> keys;  // DICT keys, a[j] is keys[j]'s value

  bool none() const { return k == NONE; }
  bool isnum() const { return k == INT || k == BOOL; }
  bool seq() const { return k == TUP || k == LIST; }
  bool is(const char *p) const { return k == STR && s == p; }
  bool num(long long v) const { return isnum() && i == v; }
  long long n() const {
    if (!isnum()) raise(PY_TYPE, "an integer is required");
    return i;
  }
  size_t len() const {
    if (k == STR) return s.size();
    if (k == NONE || isnum()) raise(PY_TYPE, "object has no len()");
    return a.size();
  }
  const V &operator[](long long j) const {
    if (!seq()) raise(PY_TYPE, "object is not subscriptable");
    long long m = (long long)a.size();
    if (j < 0) j += m;
    if (j < 0 || j >= m) raise(PY_INDEX, k == TUP ? "tuple index out of range" : "list index out of range");
    return a[(size_t)j];
  }
  const V *find(const std::string &key) const {
    for (size_t j = 0; j < keys.size(); j++)
      if (keys[j] == key) return &a[j];
    return nullptr;
  }
  const V &get(const std::string &key) const {   // d.get(key)
    static const V nil;
    if (k != DICT) raise(PY_TYPE, "object has no attribute 'get'");
    const V *p = find(key);
    return p ? *p : nil;
  }
  const V &at(const std::string &key) const {    // d[key]
    if (k != DICT) raise(PY_TYPE, "object is not subscriptable");
    const V *p = find(key);
    if (!p) raise(PY_KEY, "'" + key + "'");
    return *p;
  }
  void set(const std::string &key, const V &v) {
    for (size_t j = 0; j < keys.size(); j++)
      if (keys[j] == key) {
        a[j] = v;
        return;
      }
    keys.push_back(key);
    a.push_back(v);
  }
  bool truthy() const {
    switch (k) {
      case NONE: return false;
      case BOOL:
      case INT: return i != 0;
      case STR: return !s.empty();
      default: return !a.empty();
    }
  }
};

bool operator==(const V &x, const V &y);
inline bool operator!=(const V &x, const V &y) { return !(x == y); }

inline V I(long long v) {
  V r;
  r.k = V::INT;
  r.i = v;
  return r;
}
inline V Bool(bool b) {
  V r;
  r.k = V::BOOL;
  r.i = b ? 1 : 0;
  return r;
}
inline V S(const std::string &s) {
  V r;
  r.k = V::STR;
  r.s = s;
  return r;
}
inline V T(std::initializer_list<V> xs) {
  V r;
  r.k = V::TUP;
  r.a.assign(xs.begin(), xs.end());
  return r;
}
inline V L(std::vector<V> xs = {}) {
  V r;
  r.k = V::LIST;
  r.a = std::move(xs);
  return r;
}
inline V D() {
  V r;
  r.k = V::DICT;
  return r;
}

std::string to_json(const V &v);
V from_json(const std::string &s);   // ValueError on bad text
std::string pystr(const V &v);       // "%s" % v for the ints and strs the models format

}  // namespace cbinv
