// pyutil.h -- the few Python behaviours the models' text depends on, made exact: int(x, 0) and
// int(x, 16) (with their ValueError), "%x", chr() as UTF-8, str.split/partition/replace, and small
// cursors for the regular expressions the models match operand text with (each matcher below names
// the Python pattern it stands for).
#pragma once

#include <cctype>
#include <cstdio>
#include <cstring>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "flow16.h"   // PyError

namespace cbinv {

enum { PY_INDEX = 1, PY_VALUE = 2, PY_OTHER = 3, PY_KEY = 4, PY_ASSERT = 5, PY_TYPE = 6 };

[[noreturn]] inline void raise(int kind, const std::string &msg) { throw PyError{kind, msg}; }

inline bool starts(const std::string &s, const char *p) { return s.compare(0, strlen(p), p) == 0; }
inline bool ends(const std::string &s, const std::string &p) {
  return s.size() >= p.size() && s.compare(s.size() - p.size(), p.size(), p) == 0;
}
inline bool has(const std::string &s, const char *p) { return s.find(p) != std::string::npos; }
inline bool has(const std::string &s, const std::string &p) { return s.find(p) != std::string::npos; }

inline std::string replace_all(std::string s, const std::string &a, const std::string &b) {
  size_t i = 0;
  while ((i = s.find(a, i)) != std::string::npos) {
    s.replace(i, a.size(), b);
    i += b.size();
  }
  return s;
}

inline std::vector<std::string> split(const std::string &s, const std::string &sep) {
  std::vector<std::string> out;
  size_t a = 0;
  for (;;) {
    size_t b = s.find(sep, a);
    if (b == std::string::npos) {
      out.push_back(s.substr(a));
      return out;
    }
    out.push_back(s.substr(a, b - a));
    a = b + sep.size();
  }
}

// a, b = s.split(sep)   (ValueError unless exactly two parts)
inline std::pair<std::string, std::string> split2(const std::string &s, const std::string &sep) {
  auto v = split(s, sep);
  if (v.size() != 2) raise(PY_VALUE, v.size() < 2 ? "not enough values to unpack" : "too many values to unpack");
  return {v[0], v[1]};
}

// s.partition(sep) -> (head, tail); tail is "" when sep is absent
inline std::pair<std::string, std::string> partition(const std::string &s, const std::string &sep) {
  size_t i = s.find(sep);
  if (i == std::string::npos) return {s, ""};
  return {s.substr(0, i), s.substr(i + sep.size())};
}

inline bool isw(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_'; }
inline bool ishex(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'); }
inline bool isd(char c) { return c >= '0' && c <= '9'; }

// int(s, 0): ValueError for anything Python would refuse (a register name, a leading zero)
inline long long int0(const std::string &s0) {
  std::string s;
  for (char c : s0)
    if (c != '_') s += c;
  size_t i = 0;
  bool neg = false;
  if (i < s.size() && (s[i] == '-' || s[i] == '+')) neg = s[i++] == '-';
  auto bad = [&]() -> long long { raise(PY_VALUE, "invalid literal for int() with base 0: '" + s0 + "'"); };
  if (s0.empty() || s0.front() == '_' || s0.back() == '_' || s0.find("__") != std::string::npos) bad();
  long long v = 0;
  if (i + 1 < s.size() && s[i] == '0' && (s[i + 1] == 'x' || s[i + 1] == 'X')) {
    i += 2;
    if (i >= s.size()) bad();
    for (; i < s.size(); i++) {
      char c = (char)tolower(s[i]);
      if (!ishex(c)) bad();
      v = v * 16 + (isd(c) ? c - '0' : c - 'a' + 10);
    }
  } else if (i + 1 < s.size() && s[i] == '0' && (s[i + 1] == 'o' || s[i + 1] == 'O' || s[i + 1] == 'b' || s[i + 1] == 'B')) {
    int base = (s[i + 1] == 'o' || s[i + 1] == 'O') ? 8 : 2;
    i += 2;
    if (i >= s.size()) bad();
    for (; i < s.size(); i++) {
      int d = s[i] - '0';
      if (d < 0 || d >= base) bad();
      v = v * base + d;
    }
  } else {
    if (i >= s.size()) bad();
    bool zero_lead = s[i] == '0';
    for (size_t j = i; j < s.size(); j++) {
      if (!isd(s[j])) bad();
      if (zero_lead && s[j] != '0') bad();
      v = v * 10 + (s[j] - '0');
    }
  }
  return neg ? -v : v;
}

// int(s, 16)
inline long long int16(const std::string &s0) {
  std::string s;
  size_t a = s0.find_first_not_of(" \t"), b = s0.find_last_not_of(" \t");
  if (a != std::string::npos) s = s0.substr(a, b - a + 1);
  size_t i = 0;
  bool neg = false;
  if (i < s.size() && (s[i] == '-' || s[i] == '+')) neg = s[i++] == '-';
  if (i + 1 < s.size() && s[i] == '0' && (s[i + 1] == 'x' || s[i + 1] == 'X')) i += 2;
  if (i >= s.size()) raise(PY_VALUE, "invalid literal for int() with base 16: '" + s0 + "'");
  long long v = 0;
  for (; i < s.size(); i++) {
    char c = (char)tolower(s[i]);
    if (c == '_') continue;
    if (!ishex(c)) raise(PY_VALUE, "invalid literal for int() with base 16: '" + s0 + "'");
    v = v * 16 + (isd(c) ? c - '0' : c - 'a' + 10);
  }
  return neg ? -v : v;
}

inline std::string hexs(long long v) {   // "%x" % v
  char b[32];
  if (v < 0) {
    snprintf(b, sizeof b, "-%llx", (unsigned long long)(-v));
  } else {
    snprintf(b, sizeof b, "%llx", (unsigned long long)v);
  }
  return b;
}

inline std::string chr(long long cp) {   // chr(cp), UTF-8
  if (cp < 0 || cp > 0x10ffff) raise(PY_VALUE, "chr() arg not in range(0x110000)");
  std::string o;
  if (cp < 0x80) {
    o += (char)cp;
  } else if (cp < 0x800) {
    o += (char)(0xc0 | (cp >> 6));
    o += (char)(0x80 | (cp & 0x3f));
  } else if (cp < 0x10000) {
    o += (char)(0xe0 | (cp >> 12));
    o += (char)(0x80 | ((cp >> 6) & 0x3f));
    o += (char)(0x80 | (cp & 0x3f));
  } else {
    o += (char)(0xf0 | (cp >> 18));
    o += (char)(0x80 | ((cp >> 12) & 0x3f));
    o += (char)(0x80 | ((cp >> 6) & 0x3f));
    o += (char)(0x80 | (cp & 0x3f));
  }
  return o;
}

inline long long floordiv(long long a, long long b) {
  long long q = a / b;
  if ((a % b != 0) && ((a < 0) != (b < 0))) q--;
  return q;
}

// a cursor over operand text, for the anchored patterns
struct Cur {
  const std::string &s;
  size_t i;
  explicit Cur(const std::string &t, size_t at = 0) : s(t), i(at) {}
  bool lit(const char *p) {
    size_t n = strlen(p);
    if (s.compare(i, n, p) != 0) return false;
    i += n;
    return true;
  }
  bool word(std::string &out) {   // \w+
    size_t a = i;
    while (i < s.size() && isw(s[i])) i++;
    out = s.substr(a, i - a);
    return i > a;
  }
  bool hexrun(std::string &out) {   // [0-9a-f]+
    size_t a = i;
    while (i < s.size() && ishex(s[i])) i++;
    out = s.substr(a, i - a);
    return i > a;
  }
  bool digits(std::string &out) {   // \d+
    size_t a = i;
    while (i < s.size() && isd(s[i])) i++;
    out = s.substr(a, i - a);
    return i > a;
  }
  void ws() {   // \s*
    while (i < s.size() && (s[i] == ' ' || s[i] == '\t' || s[i] == '\n' || s[i] == '\r' || s[i] == '\f' || s[i] == '\v')) i++;
  }
  bool end() const { return i == s.size(); }
};

// (0x[0-9a-f]+|\d+) followed by `then` (the alternation as the regex tries it)
inline bool hex_or_dec_then(Cur &c, std::string &out, const char *then, bool at_end) {
  size_t at = c.i;
  std::string h;
  if (c.lit("0x") && c.hexrun(h)) {
    size_t mid = c.i;
    if (c.lit(then) && (!at_end || c.end())) {
      out = c.s.substr(at, mid - at);
      return true;
    }
  }
  c.i = at;
  if (c.digits(h)) {
    if (c.lit(then) && (!at_end || c.end())) {
      out = h;
      return true;
    }
  }
  c.i = at;
  return false;
}

// re.match(r"(-?0x[0-9a-f]+|-?\d+)$", s) from position `at`
inline bool num_tok(const std::string &s, size_t at = 0) {
  Cur c(s, at);
  c.lit("-");
  size_t b = c.i;
  std::string h;
  if (c.lit("0x") && c.hexrun(h) && c.end()) return true;
  c.i = b;
  return c.digits(h) && c.end();
}

// `\[bp \+ (\w+)\]` matched at `at` (end = position after the match)
inline bool bp_plus_at(const std::string &s, size_t at, std::string &w, size_t *endp = nullptr) {
  Cur c(s, at);
  if (!c.lit("[bp + ") || !c.word(w) || !c.lit("]")) return false;
  if (endp) *endp = c.i;
  return true;
}

// re.finditer(r"\[bp \+ (\w+)\]", s): every group, left to right
inline std::vector<std::string> finditer_bp_plus(const std::string &s) {
  std::vector<std::string> out;
  size_t i = 0;
  while ((i = s.find("[bp + ", i)) != std::string::npos) {
    std::string w;
    size_t e;
    if (bp_plus_at(s, i, w, &e)) {
      out.push_back(w);
      i = e;
    } else {
      i++;
    }
  }
  return out;
}

// re.search(r"\[bp \+ (\w+)\]", s)
inline bool search_bp_plus(const std::string &s, std::string &w) {
  auto v = finditer_bp_plus(s);
  if (v.empty()) return false;
  w = v[0];
  return true;
}

// re.finditer(r"\[0x([0-9a-f]+)\]", s)
inline std::vector<std::string> finditer_hexmem(const std::string &s) {
  std::vector<std::string> out;
  size_t i = 0;
  while ((i = s.find("[0x", i)) != std::string::npos) {
    Cur c(s, i + 3);
    std::string h;
    if (c.hexrun(h) && c.lit("]")) {
      out.push_back(h);
      i = c.i;
    } else {
      i++;
    }
  }
  return out;
}

// re.match(r"<prefix>\[0x([0-9a-f]+)\]<suffix>$", s)
inline bool hexmem_full(const std::string &s, size_t at, std::string &h, const char *suffix = "") {
  Cur c(s, at);
  return c.lit("[0x") && c.hexrun(h) && c.lit("]") && c.lit(suffix) && c.end();
}

// re.match(r"\[bp - (\w+)\]<suffix>$", s) at `at`
inline bool bp_minus_full(const std::string &s, size_t at, std::string &w, const char *suffix = "") {
  Cur c(s, at);
  return c.lit("[bp - ") && c.word(w) && c.lit("]") && c.lit(suffix) && c.end();
}

// re.match(r"\[bp \+ (\w+)\]<suffix>$", s) at `at`
inline bool bp_plus_full(const std::string &s, size_t at, std::string &w, const char *suffix = "") {
  Cur c(s, at);
  return c.lit("[bp + ") && c.word(w) && c.lit("]") && c.lit(suffix) && c.end();
}

// an insertion-ordered dict of strings (Python's dict order: a key keeps its first place)
struct ODict {
  std::vector<std::string> keys;
  std::map<std::string, std::string> v;
  void set(const std::string &k, const std::string &val) {
    if (!v.count(k)) keys.push_back(k);
    v[k] = val;
  }
  bool count(const std::string &k) const { return v.count(k) != 0; }
  size_t size() const { return keys.size(); }
};

std::string jstr(const std::string &s);   // a JSON string literal (api.cc)

}  // namespace cbinv
