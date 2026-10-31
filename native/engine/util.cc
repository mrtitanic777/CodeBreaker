// util.cc -- hashes (Windows' CNG), JSON and repr() as Python writes them, the row buffers.
#include "util.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <bcrypt.h>

#include <chrono>
#include <cstdarg>
#include <cstdio>
#include <cstring>

namespace cbe {

void raise(int kind, const std::string& msg) { throw Error(kind, msg); }

// ---- hashes ----------------------------------------------------------------------------------
namespace {
struct Hash {
  BCRYPT_HASH_HANDLE h = nullptr;
  ULONG len = 0;
  explicit Hash(BCRYPT_ALG_HANDLE alg) {
    if (BCryptCreateHash(alg, &h, nullptr, 0, nullptr, 0, 0) != 0) raise(ERR_OTHER, "BCryptCreateHash failed");
    ULONG got = 0;
    BCryptGetProperty(alg, BCRYPT_HASH_LENGTH, (PUCHAR)&len, sizeof(len), &got, 0);
  }
  ~Hash() { if (h) BCryptDestroyHash(h); }
  void update(const void* p, size_t n) {
    const unsigned char* c = (const unsigned char*)p;
    while (n) {
      ULONG k = n > 0x40000000 ? 0x40000000 : (ULONG)n;
      BCryptHashData(h, (PUCHAR)c, k, 0);
      c += k;
      n -= k;
    }
  }
  std::string hex() {
    unsigned char out[64];
    BCryptFinishHash(h, out, len, 0);
    static const char* d = "0123456789abcdef";
    std::string s;
    for (ULONG i = 0; i < len; i++) {
      s.push_back(d[out[i] >> 4]);
      s.push_back(d[out[i] & 15]);
    }
    return s;
  }
};
}  // namespace

std::string sha1_hex(const void* data, size_t n) {
  Hash h(BCRYPT_SHA1_ALG_HANDLE);
  h.update(data, n);
  return h.hex();
}

std::string sha1_hex(const std::string& s) { return sha1_hex(s.data(), s.size()); }

std::string sha1_hex2(const std::string& a, const void* data, size_t n) {
  Hash h(BCRYPT_SHA1_ALG_HANDLE);
  h.update(a.data(), a.size());
  h.update(data, n);
  return h.hex();
}

std::string sha256_file(const std::wstring& path) {
  HANDLE f = CreateFileW(path.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                         nullptr, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, nullptr);
  if (f == INVALID_HANDLE_VALUE) raise(ERR_OTHER, "cannot open " + narrow(path));
  Hash h(BCRYPT_SHA256_ALG_HANDLE);
  std::vector<char> buf(1 << 20);
  for (;;) {
    DWORD got = 0;
    if (!ReadFile(f, buf.data(), (DWORD)buf.size(), &got, nullptr)) {
      CloseHandle(f);
      raise(ERR_OTHER, "cannot read " + narrow(path));
    }
    if (!got) break;
    h.update(buf.data(), got);
  }
  CloseHandle(f);
  return h.hex();
}

// ---- text ------------------------------------------------------------------------------------
std::wstring widen(const std::string& s) {
  if (s.empty()) return std::wstring();
  int n = MultiByteToWideChar(CP_UTF8, 0, s.data(), (int)s.size(), nullptr, 0);
  std::wstring w(n, L'\0');
  MultiByteToWideChar(CP_UTF8, 0, s.data(), (int)s.size(), &w[0], n);
  return w;
}

std::string narrow(const std::wstring& w) {
  if (w.empty()) return std::string();
  int n = WideCharToMultiByte(CP_UTF8, 0, w.data(), (int)w.size(), nullptr, 0, nullptr, nullptr);
  std::string s(n, '\0');
  WideCharToMultiByte(CP_UTF8, 0, w.data(), (int)w.size(), &s[0], n, nullptr, nullptr);
  return s;
}

namespace {
// UTF-8 to code points, leniently (a surrogate written as three bytes, as Python's surrogatepass
// writes it, is that surrogate; a stray byte is itself)
std::vector<uint32_t> points(const std::string& s) {
  std::vector<uint32_t> out;
  size_t i = 0, n = s.size();
  while (i < n) {
    unsigned char c = (unsigned char)s[i];
    uint32_t cp = c;
    int more = 0;
    if (c >= 0xF0 && c < 0xF8) { cp = c & 7; more = 3; }
    else if (c >= 0xE0) { cp = c & 15; more = 2; }
    else if (c >= 0xC0) { cp = c & 31; more = 1; }
    if (more && i + more < n) {
      bool ok = true;
      for (int k = 1; ok && k <= more; k++) {
        if (((unsigned char)s[i + k] & 0xC0) != 0x80) ok = false;
      }
      if (ok) {
        for (int k = 1; k <= more; k++) cp = (cp << 6) | ((unsigned char)s[i + k] & 0x3F);
        out.push_back(cp);
        i += more + 1;
        continue;
      }
    }
    out.push_back(c);
    i++;
  }
  return out;
}

void utf8(std::string& o, uint32_t cp) {
  if (cp < 0x80) o.push_back((char)cp);
  else if (cp < 0x800) { o.push_back((char)(0xC0 | (cp >> 6))); o.push_back((char)(0x80 | (cp & 0x3F))); }
  else if (cp < 0x10000) {
    o.push_back((char)(0xE0 | (cp >> 12)));
    o.push_back((char)(0x80 | ((cp >> 6) & 0x3F)));
    o.push_back((char)(0x80 | (cp & 0x3F)));
  } else {
    o.push_back((char)(0xF0 | (cp >> 18)));
    o.push_back((char)(0x80 | ((cp >> 12) & 0x3F)));
    o.push_back((char)(0x80 | ((cp >> 6) & 0x3F)));
    o.push_back((char)(0x80 | (cp & 0x3F)));
  }
}

// str.isprintable() for a character past ASCII, as near as the engine's keys need: the C1
// controls, the no-break and other separators, format characters, surrogates and private use
// are not printable; the rest are.
bool printable(uint32_t c) {
  if (c >= 0x80 && c <= 0xA0) return false;
  if (c == 0xAD || c == 0x1680 || (c >= 0x2000 && c <= 0x200F) || (c >= 0x2028 && c <= 0x202F) ||
      (c >= 0x205F && c <= 0x206F) || c == 0x3000 || c == 0xFEFF || (c >= 0xFFF9 && c <= 0xFFFB))
    return false;
  if (c >= 0xD800 && c <= 0xF8FF) return false;               // surrogates, private use
  if (c >= 0xF0000) return false;                             // private use planes
  if (c == 0xFFFE || c == 0xFFFF) return false;
  return true;
}
}  // namespace

std::string json_str(const std::string& s) {
  std::string o = "\"";
  char b[16];
  for (uint32_t c : points(s)) {
    switch (c) {
      case '"': o += "\\\""; continue;
      case '\\': o += "\\\\"; continue;
      case '\n': o += "\\n"; continue;
      case '\r': o += "\\r"; continue;
      case '\t': o += "\\t"; continue;
      case '\b': o += "\\b"; continue;
      case '\f': o += "\\f"; continue;
    }
    if (c >= 0x20 && c <= 0x7E) { o.push_back((char)c); continue; }
    if (c > 0xFFFF) {
      uint32_t v = c - 0x10000;
      snprintf(b, sizeof b, "\\u%04x\\u%04x", 0xD800 | (v >> 10), 0xDC00 | (v & 0x3FF));
    } else {
      snprintf(b, sizeof b, "\\u%04x", c);
    }
    o += b;
  }
  return o + "\"";
}

std::string py_repr(const std::string& s) {
  bool sq = s.find('\'') != std::string::npos, dq = s.find('"') != std::string::npos;
  char q = (sq && !dq) ? '"' : '\'';
  std::string o(1, q);
  char b[16];
  for (uint32_t c : points(s)) {
    if (c == (uint32_t)q || c == '\\') { o.push_back('\\'); o.push_back((char)c); continue; }
    if (c == '\t') { o += "\\t"; continue; }
    if (c == '\n') { o += "\\n"; continue; }
    if (c == '\r') { o += "\\r"; continue; }
    if (c < 0x20 || c == 0x7F) { snprintf(b, sizeof b, "\\x%02x", c); o += b; continue; }
    if (c < 0x7F) { o.push_back((char)c); continue; }
    if (printable(c)) { utf8(o, c); continue; }
    if (c <= 0xFF) snprintf(b, sizeof b, "\\x%02x", c);
    else if (c <= 0xFFFF) snprintf(b, sizeof b, "\\u%04x", c);
    else snprintf(b, sizeof b, "\\U%08x", c);
    o += b;
  }
  o.push_back(q);
  return o;
}

std::string py_repr_tuple(const std::vector<std::string>& items) {
  std::string o = "(";
  for (size_t i = 0; i < items.size(); i++) {
    if (i) o += ", ";
    o += py_repr(items[i]);
  }
  if (items.size() == 1) o += ",";
  return o + ")";
}

std::string fmt(const char* f, ...) {
  va_list ap;
  va_start(ap, f);
  char small[512];
  int n = vsnprintf(small, sizeof small, f, ap);
  va_end(ap);
  if (n < (int)sizeof small) return std::string(small, n < 0 ? 0 : n);
  std::string s(n + 1, '\0');
  va_start(ap, f);
  vsnprintf(&s[0], n + 1, f, ap);
  va_end(ap);
  s.resize(n);
  return s;
}

std::string py_float(double d) {
  // the shortest text that reads back the same, as repr(float) (fixed form for the times kept here)
  char b[64];
  for (int p = 1; p <= 17; p++) {
    snprintf(b, sizeof b, "%.*g", p, d);
    if (strtod(b, nullptr) == d) break;
  }
  std::string s = b;
  if (s.find_first_of(".eEn") == std::string::npos) s += ".0";      // (%g drops a whole number's ".0")
  return s;
}

// ---- JSON ------------------------------------------------------------------------------------
namespace {
struct Parser {
  const std::string& t;
  size_t i = 0;
  explicit Parser(const std::string& text) : t(text) {}
  [[noreturn]] void bad() { raise(ERR_VALUE, "bad JSON at " + std::to_string(i)); }
  void ws() { while (i < t.size() && (t[i] == ' ' || t[i] == '\t' || t[i] == '\n' || t[i] == '\r')) i++; }
  uint32_t hex4() {
    if (i + 4 > t.size()) bad();
    uint32_t v = 0;
    for (int k = 0; k < 4; k++) {
      char c = t[i++];
      v <<= 4;
      if (c >= '0' && c <= '9') v |= c - '0';
      else if (c >= 'a' && c <= 'f') v |= c - 'a' + 10;
      else if (c >= 'A' && c <= 'F') v |= c - 'A' + 10;
      else bad();
    }
    return v;
  }
  std::string str() {
    if (t[i] != '"') bad();
    i++;
    std::string o;
    while (i < t.size() && t[i] != '"') {
      char c = t[i++];
      if (c != '\\') { o.push_back(c); continue; }
      if (i >= t.size()) bad();
      char e = t[i++];
      switch (e) {
        case '"': o.push_back('"'); break;
        case '\\': o.push_back('\\'); break;
        case '/': o.push_back('/'); break;
        case 'b': o.push_back('\b'); break;
        case 'f': o.push_back('\f'); break;
        case 'n': o.push_back('\n'); break;
        case 'r': o.push_back('\r'); break;
        case 't': o.push_back('\t'); break;
        case 'u': {
          uint32_t cp = hex4();
          if (cp >= 0xD800 && cp < 0xDC00 && i + 6 <= t.size() && t[i] == '\\' && t[i + 1] == 'u') {
            size_t save = i;
            i += 2;
            uint32_t lo = hex4();
            if (lo >= 0xDC00 && lo < 0xE000) cp = 0x10000 + ((cp - 0xD800) << 10) + (lo - 0xDC00);
            else i = save;
          }
          utf8(o, cp);
          break;
        }
        default: bad();
      }
    }
    if (i >= t.size()) bad();
    i++;
    return o;
  }
  Json value() {
    ws();
    if (i >= t.size()) bad();
    Json v;
    char c = t[i];
    if (c == '"') { v.t = Json::STR; v.s = str(); }
    else if (c == '[') {
      v.t = Json::ARR;
      i++;
      ws();
      if (i < t.size() && t[i] == ']') { i++; return v; }
      for (;;) {
        v.a.push_back(value());
        ws();
        if (i < t.size() && t[i] == ',') { i++; continue; }
        if (i < t.size() && t[i] == ']') { i++; break; }
        bad();
      }
    } else if (c == '{') {
      v.t = Json::OBJ;
      i++;
      ws();
      if (i < t.size() && t[i] == '}') { i++; return v; }
      for (;;) {
        ws();
        std::string k = str();
        ws();
        if (i >= t.size() || t[i] != ':') bad();
        i++;
        v.o.emplace_back(k, value());
        ws();
        if (i < t.size() && t[i] == ',') { i++; continue; }
        if (i < t.size() && t[i] == '}') { i++; break; }
        bad();
      }
    } else if (t.compare(i, 4, "null") == 0) { i += 4; }
    else if (t.compare(i, 4, "true") == 0) { i += 4; v.t = Json::BOOL; v.b = true; }
    else if (t.compare(i, 5, "false") == 0) { i += 5; v.t = Json::BOOL; }
    else {
      size_t s = i;
      while (i < t.size() && (isdigit((unsigned char)t[i]) || strchr("+-.eE", t[i]))) i++;
      if (s == i) bad();
      v.t = Json::NUM;
      v.raw = t.substr(s, i - s);
      v.num = atof(v.raw.c_str());
    }
    return v;
  }
};
}  // namespace

Json json_parse(const std::string& text) {
  Parser p(text);
  Json v = p.value();
  p.ws();
  if (p.i != text.size()) p.bad();
  return v;
}

std::string json_inputs(const std::vector<std::vector<std::string>>& rows) {
  std::string o = "[";
  for (size_t r = 0; r < rows.size(); r++) {
    if (r) o += ", ";
    o += "[";
    for (size_t k = 0; k < rows[r].size(); k++) {
      if (k) o += ", ";
      o += json_str(rows[r][k]);
    }
    o += "]";
  }
  return o + "]";
}

// ---- row buffers -----------------------------------------------------------------------------
void Writer::field(const Field& f) {
  switch (f.type) {
    case 0: none(); break;
    case 1: integer(f.i); break;
    case 2: real(f.d); break;
    case 3: text(f.s); break;
    default: bytes(f.s); break;
  }
}

Field Reader::next() {
  if (p >= end) raise(ERR_VALUE, "row buffer: past its end");
  Field f;
  f.type = (unsigned char)*p++;
  switch (f.type) {
    case 0: break;
    case 1: memcpy(&f.i, p, 8); p += 8; break;
    case 2: memcpy(&f.d, p, 8); p += 8; break;
    case 3:
    case 4: {
      int64_t n;
      memcpy(&n, p, 8);
      p += 8;
      if (n < 0 || p + n > end) raise(ERR_VALUE, "row buffer: a field past its end");
      f.s.assign(p, (size_t)n);
      p += n;
      break;
    }
    default: raise(ERR_VALUE, "row buffer: a field of no known type");
  }
  return f;
}

std::string Reader::str() {
  Field f = next();
  return f.s;
}

std::vector<std::string> Reader::strings() {
  std::vector<std::string> out;
  while (!done()) out.push_back(str());
  return out;
}

double now() {
  using namespace std::chrono;
  return duration_cast<duration<double>>(system_clock::now().time_since_epoch()).count();
}

}  // namespace cbe
