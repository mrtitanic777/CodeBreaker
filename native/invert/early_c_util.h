// early_c_util.h -- the Python behaviours early_c.cc and early_c_text.cc mirror: str objects with
// their identity (`is`), Python's re (native/l2's pyre), slicing, split, strip.
#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "pyre.hpp"
#include "pyutil.h"

namespace cbinv {
namespace ecu {

enum { PY_ATTR = 7 };   // AttributeError (`None.group`)

typedef std::shared_ptr<const std::string> SP;

// A Python str or None.  Identity matters once (`s.ax is not old`): a literal and every string of
// one character are one object (CPython's interned constants and its latin-1 singletons); str(int)
// and every other string made is a new object; a value copied keeps its object.
struct Str {
  SP p;
  Str() {}
  explicit Str(SP q) : p(std::move(q)) {}
  bool none() const { return !p; }
  const std::string &s() const { return *p; }
};

inline Str lit(const std::string &s) {
  thread_local std::unordered_map<std::string, SP> *tab = nullptr;
  if (!tab) tab = new std::unordered_map<std::string, SP>();
  auto it = tab->find(s);
  if (it != tab->end()) return Str(it->second);
  SP p = std::make_shared<const std::string>(s);
  (*tab)[s] = p;
  return Str(p);
}
inline Str fr(const std::string &s) { return s.size() <= 1 ? lit(s) : Str(std::make_shared<const std::string>(s)); }
inline Str fromint(long long v) { return Str(std::make_shared<const std::string>(std::to_string(v))); }
inline std::string S(const Str &x) { return x.p ? *x.p : std::string("None"); }
inline bool eq(const Str &a, const Str &b) {
  if (a.none() || b.none()) return a.none() && b.none();
  return *a.p == *b.p;
}
inline bool truthy(const Str &a) { return a.p && !a.p->empty(); }
inline Str orS(const Str &a, const char *b) { return truthy(a) ? a : lit(b); }
inline bool is(const Str &a, const char *b) { return a.p && *a.p == b; }

// re
inline const re::Regex &RX(const std::string &p) { return re::rx(p); }
inline re::Match mt(const std::string &p, const std::string &s) { return re::rx(p).match(s); }
inline re::Match sr(const std::string &p, const std::string &s) { return re::rx(p).search(s); }
inline re::Match fm(const std::string &p, const std::string &s) { return re::rx(p).fullmatch(s); }
// patterns made from the data (not cached: Python's own cache is bounded)
inline re::Match srd(const std::string &p, const std::string &s) { return re::Regex(p).search(s); }
inline Str G(const re::Match &m, int k) { return m.has(k) ? fr(m.group(k)) : Str(); }
inline std::string g(const re::Match &m, int k) { return m.group(k); }

// `re.match(r"(0x[0-9a-f]+|\d+)$", s)`
inline bool isnum(const std::string &s) {
  if (s.size() > 2 && s[0] == '0' && s[1] == 'x') {
    bool ok = true;
    for (size_t i = 2; i < s.size(); i++)
      if (!ishex(s[i])) { ok = false; break; }
    if (ok) return true;
  }
  if (s.empty()) return false;
  for (char c : s)
    if (!isd(c)) return false;
  return true;
}
inline bool isdigits(const std::string &s) {   // str.isdigit() (ASCII text)
  if (s.empty()) return false;
  for (char c : s)
    if (!isd(c)) return false;
  return true;
}

inline bool isws(char c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v'; }
inline std::string strip(const std::string &s) {
  size_t a = 0, b = s.size();
  while (a < b && isws(s[a])) a++;
  while (b > a && isws(s[b - 1])) b--;
  return s.substr(a, b - a);
}
inline std::string lstrip_sp(const std::string &s) {   // lstrip(" ")
  size_t a = 0;
  while (a < s.size() && s[a] == ' ') a++;
  return s.substr(a);
}
inline std::string rstrip(const std::string &s) {
  size_t b = s.size();
  while (b > 0 && isws(s[b - 1])) b--;
  return s.substr(0, b);
}
inline long long indent(const std::string &s) { return (long long)s.size() - (long long)lstrip_sp(s).size(); }
inline std::string sl(const std::string &s, long long a) {   // s[a:]
  long long n = (long long)s.size();
  if (a < 0) a = std::max(0LL, a + n);
  return a >= n ? std::string() : s.substr((size_t)a);
}
inline std::string sl(const std::string &s, long long a, long long b) {   // s[a:b]
  long long n = (long long)s.size();
  if (a < 0) a = std::max(0LL, a + n);
  if (b < 0) b = std::max(0LL, b + n);
  if (b > n) b = n;
  return a >= b ? std::string() : s.substr((size_t)a, (size_t)(b - a));
}
inline std::vector<std::string> splitn(const std::string &s, const std::string &sep, int n) {   // s.split(sep, n)
  std::vector<std::string> out;
  size_t a = 0;
  while ((int)out.size() < n) {
    size_t b = s.find(sep, a);
    if (b == std::string::npos) break;
    out.push_back(s.substr(a, b - a));
    a = b + sep.size();
  }
  out.push_back(s.substr(a));
  return out;
}
inline std::vector<std::string> wsplit(const std::string &s) {   // s.split()
  std::vector<std::string> out;
  size_t i = 0;
  while (i < s.size()) {
    while (i < s.size() && isws(s[i])) i++;
    if (i >= s.size()) break;
    size_t j = i;
    while (j < s.size() && !isws(s[j])) j++;
    out.push_back(s.substr(i, j - i));
    i = j;
  }
  return out;
}
inline std::pair<std::string, std::string> rpartition(const std::string &s, const std::string &sep) {   // (head, tail)
  size_t i = s.rfind(sep);
  if (i == std::string::npos) return {"", s};
  return {s.substr(0, i), s.substr(i + sep.size())};
}
inline long long count(const std::string &s, const std::string &x) {
  long long n = 0;
  size_t i = 0;
  while ((i = s.find(x, i)) != std::string::npos) n++, i += x.size();
  return n;
}
inline std::string join(const std::vector<std::string> &v, const std::string &sep) {
  std::string o;
  for (size_t i = 0; i < v.size(); i++) o += (i ? sep : "") + v[i];
  return o;
}
inline std::string spaces(long long n) { return std::string((size_t)std::max(0LL, n), ' '); }
inline std::string X(long long v) { return hexs(v); }
inline std::string D(long long v) { return std::to_string(v); }
inline std::string strip_left(const std::string &s, size_t n) { return n >= s.size() ? std::string() : s.substr(n); }

template <class T>
T &at(std::vector<T> &v, long long k) {
  long long n = (long long)v.size();
  if (k < 0) k += n;
  if (k < 0 || k >= n) raise(PY_INDEX, "list index out of range");
  return v[(size_t)k];
}
template <class T>
const T &at(const std::vector<T> &v, long long k) {
  long long n = (long long)v.size();
  if (k < 0) k += n;
  if (k < 0 || k >= n) raise(PY_INDEX, "list index out of range");
  return v[(size_t)k];
}
inline std::string bool_attr() { raise(PY_ATTR, "'NoneType' object has no attribute 'group'"); }

}  // namespace ecu
}  // namespace cbinv
