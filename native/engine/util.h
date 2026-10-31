// util.h -- what every part of the engine uses: hashes, JSON as Python's json module writes it,
// Python's repr() of text, the row buffers handed across the C interface, and the errors.
#pragma once
#include <cstdint>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace cbe {

// ---- errors ----------------------------------------------------------------------------------
// Each says which Python exception it stands for, so the shim raises the same kind the Python
// engine raised: a LookupError (no step makes a kind, an undeclared read), sqlite3's errors, or an
// exception a Python step raised (carried by its number in the shim's table).
enum ErrKind { ERR_OTHER = 2, ERR_LOOKUP = 1, ERR_SQLITE = 4, ERR_PYTHON = 3, ERR_KEY = 5, ERR_VALUE = 6,
               ERR_OS = 7 };

struct Error : std::runtime_error {
  int kind;
  int64_t py = 0;            // ERR_PYTHON: the exception's number in the shim's table
  bool py_lookup = false;    // ... and whether it is a LookupError (KeyError, IndexError ...)
  int code = 0;              // ERR_SQLITE: SQLite's result code
  Error(int k, const std::string& m) : std::runtime_error(m), kind(k) {}
  bool lookup() const { return kind == ERR_LOOKUP || kind == ERR_KEY || (kind == ERR_PYTHON && py_lookup); }
};

[[noreturn]] void raise(int kind, const std::string& msg);

// ---- hashes ----------------------------------------------------------------------------------
std::string sha1_hex(const void* data, size_t n);
std::string sha1_hex(const std::string& s);
std::string sha1_hex2(const std::string& a, const void* data, size_t n);   // SHA-1 of a then data
std::string sha256_file(const std::wstring& path);                          // throws Error on I/O

// ---- text ------------------------------------------------------------------------------------
std::wstring widen(const std::string& utf8);
std::string narrow(const std::wstring& w);
std::string json_str(const std::string& utf8);          // json.dumps(str) (ensure_ascii)
std::string py_repr(const std::string& utf8);           // repr(str)
std::string py_repr_tuple(const std::vector<std::string>& items);   // repr(tuple of str)
std::string fmt(const char* f, ...);
std::string py_float(double d);                         // repr(float)

// A JSON value, as much as the engine reads: the inputs column ([[kind, key, hash], ...]).
struct Json {
  enum T { NUL, BOOL, NUM, STR, ARR, OBJ } t = NUL;
  bool b = false;
  double num = 0;
  std::string raw;                                     // a number's text
  std::string s;
  std::vector<Json> a;
  std::vector<std::pair<std::string, Json>> o;
};
Json json_parse(const std::string& text);              // throws Error(ERR_VALUE)
// json.dumps([list(i) for i in inputs]) with Python's default separators
std::string json_inputs(const std::vector<std::vector<std::string>>& rows);

// ---- the row buffer --------------------------------------------------------------------------
// What a query gives back, and lists handed in: a run of fields, each a type byte and its bytes.
//   0 None   1 int (8 bytes)   2 float (8 bytes)   3 text (8-byte length, UTF-8)   4 bytes (length, data)
struct Field {
  int type = 0;
  int64_t i = 0;
  double d = 0;
  std::string s;                                       // text or bytes
  static Field none() { return Field(); }
  static Field integer(int64_t v) { Field f; f.type = 1; f.i = v; return f; }
  static Field real(double v) { Field f; f.type = 2; f.d = v; return f; }
  static Field text(std::string v) { Field f; f.type = 3; f.s = std::move(v); return f; }
  static Field bytes(std::string v) { Field f; f.type = 4; f.s = std::move(v); return f; }
  bool null() const { return type == 0; }
};
using Row = std::vector<Field>;

struct Writer {
  std::string out;
  void none() { out.push_back(0); }
  void integer(int64_t v) { out.push_back(1); out.append(reinterpret_cast<const char*>(&v), 8); }
  void real(double v) { out.push_back(2); out.append(reinterpret_cast<const char*>(&v), 8); }
  void text(const std::string& s) { put(3, s); }
  void bytes(const std::string& s) { put(4, s); }
  void field(const Field& f);
  void put(char t, const std::string& s) {
    int64_t n = (int64_t)s.size();
    out.push_back(t);
    out.append(reinterpret_cast<const char*>(&n), 8);
    out.append(s);
  }
};

struct Reader {
  const char* p;
  const char* end;
  Reader(const void* data, size_t n) : p((const char*)data), end((const char*)data + n) {}
  bool done() const { return p >= end; }
  Field next();
  std::string str();                                   // a text field (None -> "")
  std::vector<std::string> strings();                  // every field left, as text
};

double now();                                           // time.time()

}  // namespace cbe
