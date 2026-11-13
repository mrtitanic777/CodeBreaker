// cb/invert/operand_match.hpp -- matchers for the operand text the 16-bit models read (Capstone's `[bp + 6]`, `word ptr [0x1c]`), as cursors over
// the text where a regular expression would be slower than the shape is long. Each says which pattern it stands for.
#pragma once

#include <cstddef>
#include <cstring>
#include <map>
#include <string>
#include <vector>

#include "cb/invert/model_text.hpp"

namespace cb::invert::model {

// a cursor over operand text, for the anchored patterns
class Cursor {
public:
  const std::string &s;
  size_t i;
  explicit Cursor(const std::string &t, size_t at = 0) : s(t), i(at) {}
  bool lit(const char *p) {
    size_t n = strlen(p);
    if (s.compare(i, n, p) != 0) return false;
    i += n;
    return true;
  }
  bool word(std::string &out) {   // \w+
    size_t a = i;
    while (i < s.size() && is_word_char(s[i])) i++;
    out = s.substr(a, i - a);
    return i > a;
  }
  bool hexrun(std::string &out) {   // [0-9a-f]+
    size_t a = i;
    while (i < s.size() && is_hex_char(s[i])) i++;
    out = s.substr(a, i - a);
    return i > a;
  }
  bool digits(std::string &out) {   // \d+
    size_t a = i;
    while (i < s.size() && is_digit_char(s[i])) i++;
    out = s.substr(a, i - a);
    return i > a;
  }
  void ws() {   // \s*
    while (i < s.size() && (s[i] == ' ' || s[i] == '\t' || s[i] == '\n' || s[i] == '\r' || s[i] == '\f' || s[i] == '\v')) i++;
  }
  bool end() const { return i == s.size(); }
};

// (0x[0-9a-f]+|\d+) followed by `then` (the alternation as the regex tries it)
inline bool hex_or_dec_then(Cursor &c, std::string &out, const char *then, bool at_end) {
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
  Cursor c(s, at);
  c.lit("-");
  size_t b = c.i;
  std::string h;
  if (c.lit("0x") && c.hexrun(h) && c.end()) return true;
  c.i = b;
  return c.digits(h) && c.end();
}

// `\[bp \+ (\w+)\]` matched at `at` (end = position after the match)
inline bool bp_plus_at(const std::string &s, size_t at, std::string &w, size_t *endp = nullptr) {
  Cursor c(s, at);
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
    Cursor c(s, i + 3);
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
  Cursor c(s, at);
  return c.lit("[0x") && c.hexrun(h) && c.lit("]") && c.lit(suffix) && c.end();
}

// re.match(r"\[bp - (\w+)\]<suffix>$", s) at `at`
inline bool bp_minus_full(const std::string &s, size_t at, std::string &w, const char *suffix = "") {
  Cursor c(s, at);
  return c.lit("[bp - ") && c.word(w) && c.lit("]") && c.lit(suffix) && c.end();
}

// re.match(r"\[bp \+ (\w+)\]<suffix>$", s) at `at`
inline bool bp_plus_full(const std::string &s, size_t at, std::string &w, const char *suffix = "") {
  Cursor c(s, at);
  return c.lit("[bp + ") && c.word(w) && c.lit("]") && c.lit(suffix) && c.end();
}

// Strings by name, in the order the names first came (a name set again keeps its place).
struct OrderedStrings {
  std::vector<std::string> keys;
  std::map<std::string, std::string> v;
  void set(const std::string &k, const std::string &val) {
    if (!v.count(k)) keys.push_back(k);
    v[k] = val;
  }
  bool count(const std::string &k) const { return v.count(k) != 0; }
  size_t size() const { return keys.size(); }
};


}  // namespace cb::invert::model
