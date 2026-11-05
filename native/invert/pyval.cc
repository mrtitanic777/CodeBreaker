// pyval.cc -- pyval.h's equality, JSON form and "%s".
#include "pyval.h"

#include <cstdlib>

namespace cbinv {

bool operator==(const V &x, const V &y) {
  if (x.isnum() || y.isnum()) return x.isnum() && y.isnum() && x.i == y.i;
  if (x.k != y.k) return false;
  switch (x.k) {
    case V::NONE: return true;
    case V::STR: return x.s == y.s;
    case V::DICT:
      if (x.keys.size() != y.keys.size()) return false;
      for (size_t j = 0; j < x.keys.size(); j++) {
        const V *o = y.find(x.keys[j]);
        if (!o || !(*o == x.a[j])) return false;
      }
      return true;
    default: return x.a == y.a;
  }
}

namespace {

void put(const V &v, std::string &o) {
  switch (v.k) {
    case V::NONE: o += "null"; return;
    case V::BOOL: o += v.i ? "true" : "false"; return;
    case V::INT: o += std::to_string(v.i); return;
    case V::STR: o += jstr(v.s); return;
    case V::TUP:
    case V::LIST: {
      if (v.k == V::LIST) o += "{\"L\": ";
      o += '[';
      for (size_t j = 0; j < v.a.size(); j++) {
        if (j) o += ", ";
        put(v.a[j], o);
      }
      o += ']';
      if (v.k == V::LIST) o += '}';
      return;
    }
    case V::DICT:
      o += "{\"D\": [";
      for (size_t j = 0; j < v.a.size(); j++) {
        if (j) o += ", ";
        o += '[' + jstr(v.keys[j]) + ", ";
        put(v.a[j], o);
        o += ']';
      }
      o += "]}";
      return;
  }
}

struct Reader {
  const std::string &s;
  size_t i = 0;
  [[noreturn]] void bad() { raise(PY_VALUE, "bad pyval JSON at " + std::to_string(i)); }
  void ws() {
    while (i < s.size() && (s[i] == ' ' || s[i] == '\n' || s[i] == '\r' || s[i] == '\t')) i++;
  }
  bool eat(char c) {
    ws();
    if (i < s.size() && s[i] == c) {
      i++;
      return true;
    }
    return false;
  }
  std::string str() {
    if (!eat('"')) bad();
    std::string o;
    while (i < s.size() && s[i] != '"') {
      char c = s[i++];
      if (c != '\\') {
        o += c;
        continue;
      }
      if (i >= s.size()) bad();
      char e = s[i++];
      switch (e) {
        case 'n': o += '\n'; break;
        case 't': o += '\t'; break;
        case 'r': o += '\r'; break;
        case 'b': o += '\b'; break;
        case 'f': o += '\f'; break;
        case 'u': {
          if (i + 4 > s.size()) bad();
          long cp = std::strtol(s.substr(i, 4).c_str(), nullptr, 16);
          i += 4;
          o += chr(cp);
          break;
        }
        default: o += e;
      }
    }
    if (i >= s.size()) bad();
    i++;
    return o;
  }
  std::vector<V> arr() {   // after '['
    std::vector<V> xs;
    if (eat(']')) return xs;
    for (;;) {
      xs.push_back(val());
      if (eat(']')) return xs;
      if (!eat(',')) bad();
    }
  }
  V val() {
    ws();
    if (i >= s.size()) bad();
    char c = s[i];
    if (c == 'n' && s.compare(i, 4, "null") == 0) return i += 4, V();
    if (c == 't' && s.compare(i, 4, "true") == 0) return i += 4, Bool(true);
    if (c == 'f' && s.compare(i, 5, "false") == 0) return i += 5, Bool(false);
    if (c == '"') return S(str());
    if (c == '[') {
      i++;
      V r;
      r.k = V::TUP;
      r.a = arr();
      return r;
    }
    if (c == '{') {
      i++;
      std::string tag = str();
      if (!eat(':') || !eat('[')) bad();
      std::vector<V> xs = arr();
      if (!eat('}')) bad();
      if (tag == "L") return L(std::move(xs));
      if (tag != "D") bad();
      V d = D();
      for (auto &kv : xs) {
        if (kv.a.size() != 2 || kv.a[0].k != V::STR) bad();
        d.set(kv.a[0].s, kv.a[1]);
      }
      return d;
    }
    size_t a = i;
    if (s[i] == '-') i++;
    while (i < s.size() && isd(s[i])) i++;
    if (i == a) bad();
    return I(std::stoll(s.substr(a, i - a)));
  }
};

}  // namespace

std::string to_json(const V &v) {
  std::string o;
  put(v, o);
  return o;
}

V from_json(const std::string &s) {
  Reader r{s};
  return r.val();
}

std::string pystr(const V &v) {
  switch (v.k) {
    case V::NONE: return "None";
    case V::BOOL: return v.i ? "True" : "False";
    case V::INT: return std::to_string(v.i);
    case V::STR: return v.s;
    default: return to_json(v);
  }
}

}  // namespace cbinv
