#include "sdk_config.h"

#include <algorithm>
#include <atomic>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <map>
#include <set>
#include <sstream>
#include <thread>

#include "app_paths.h"
#include "coff_lib.h"
#include "msvc_demangle.h"
#include "version.h"

namespace fs = std::filesystem;

namespace xexmagic {
namespace {

std::string Lower(std::string s) {
  for (char& c : s) c = (char)std::tolower((unsigned char)c);
  return s;
}

std::string Trim(const std::string& s) {
  size_t a = s.find_first_not_of(" \t\r\n");
  if (a == std::string::npos) return "";
  size_t b = s.find_last_not_of(" \t\r\n");
  return s.substr(a, b - a + 1);
}

std::string JsonEscape(const std::string& s) {
  std::string o;
  for (char c : s) {
    switch (c) {
      case '"': o += "\\\""; break;
      case '\\': o += "\\\\"; break;
      case '\n': o += "\\n"; break;
      case '\r': break;
      case '\t': o += "\\t"; break;
      default:
        if ((unsigned char)c < 0x20) continue;
        o += c;
    }
  }
  return o;
}

// Width of the primitive types the SDK headers actually use. Anything absent
// leaves the field size 0, which makes the enclosing struct unsized rather than
// silently wrong -- a bad size would produce confident false matches.
uint32_t PrimitiveSize(const std::string& type) {
  static const std::map<std::string, uint32_t> kSizes = {
      {"char", 1},      {"signed char", 1},   {"unsigned char", 1},
      {"BYTE", 1},      {"UCHAR", 1},         {"CHAR", 1},
      {"BOOLEAN", 1},   {"short", 2},         {"unsigned short", 2},
      {"WORD", 2},      {"USHORT", 2},        {"SHORT", 2},
      {"WCHAR", 2},     {"int", 4},           {"unsigned int", 4},
      {"long", 4},      {"unsigned long", 4}, {"DWORD", 4},
      {"ULONG", 4},     {"LONG", 4},          {"UINT", 4},
      {"INT", 4},       {"BOOL", 4},          {"FLOAT", 4},
      {"float", 4},     {"HRESULT", 4},       {"HANDLE", 4},
      {"__int64", 8},   {"unsigned __int64", 8},
      {"LONGLONG", 8},  {"ULONGLONG", 8},     {"DWORDLONG", 8},
      {"double", 8},    {"XUID", 8},          {"LARGE_INTEGER", 8},
      {"ULARGE_INTEGER", 8},
      // BSD/Winsock spellings; the same primitives under other names.
      {"u_char", 1},    {"u_short", 2},       {"u_int", 4},
      {"u_long", 4},    {"UINT8", 1},         {"INT8", 1},
      {"UINT16", 2},    {"INT16", 2},         {"UINT32", 4},
      {"INT32", 4},     {"UINT64", 8},        {"INT64", 8},
      {"ULONG_PTR", 4}, {"LONG_PTR", 4},      {"SIZE_T", 4},
      {"DWORD_PTR", 4}, {"UINT_PTR", 4},      {"INT_PTR", 4},
      {"unsigned", 4},  {"signed", 4},
  };
  auto it = kSizes.find(type);
  return it == kSizes.end() ? 0 : it->second;
}

// Pointers are 4 bytes on the 360 regardless of what they point at.
bool IsPointer(const std::string& decl) {
  return decl.find('*') != std::string::npos;
}

bool IsIdentChar(char c) {
  return std::isalnum((unsigned char)c) || c == '_';
}

// Comments and preprocessor lines removed, so the declaration scanner sees
// only code. Line continuations are honoured, otherwise a multi-line macro
// leaves its tail behind and every following declaration is misparsed.
std::string StripCommentsAndDirectives(const std::string& src) {
  std::string out;
  out.reserve(src.size());
  bool line_comment = false, block_comment = false, directive = false;
  bool at_line_start = true;
  for (size_t i = 0; i < src.size(); i++) {
    char c = src[i];
    char n = i + 1 < src.size() ? src[i + 1] : '\0';
    if (line_comment || directive) {
      if (c == '\n') {
        // A backslash before the newline continues the line.
        size_t j = out.size();
        bool continued = false;
        for (size_t k = i; k-- > 0;) {
          if (src[k] == '\\') { continued = true; break; }
          if (src[k] != ' ' && src[k] != '\t' && src[k] != '\r') break;
        }
        (void)j;
        if (!continued) { line_comment = directive = false; out += '\n'; }
      }
      continue;
    }
    if (block_comment) {
      if (c == '*' && n == '/') { block_comment = false; i++; }
      continue;
    }
    if (c == '/' && n == '/') { line_comment = true; continue; }
    if (c == '/' && n == '*') { block_comment = true; i++; continue; }
    if (at_line_start && c == '#') { directive = true; continue; }
    at_line_start = (c == '\n') || (at_line_start && (c == ' ' || c == '\t'));
    out += c;
  }
  return out;
}

// Splits on commas that are not inside brackets.
std::vector<std::string> SplitTopLevel(const std::string& s) {
  std::vector<std::string> out;
  int depth = 0;
  std::string cur;
  for (char c : s) {
    if (c == '(' || c == '[') depth++;
    if (c == ')' || c == ']') depth--;
    if (c == ',' && depth == 0) {
      out.push_back(cur);
      cur.clear();
      continue;
    }
    cur += c;
  }
  if (!Trim(cur).empty()) out.push_back(cur);
  return out;
}

std::vector<std::string> Tokenize(const std::string& s) {
  std::vector<std::string> toks;
  size_t i = 0;
  while (i < s.size()) {
    if (std::isspace((unsigned char)s[i])) { i++; continue; }
    if (IsIdentChar(s[i])) {
      size_t j = i;
      while (j < s.size() && IsIdentChar(s[j])) j++;
      toks.push_back(s.substr(i, j - i));
      i = j;
      continue;
    }
    toks.push_back(std::string(1, s[i]));
    i++;
  }
  return toks;
}

// Calling conventions, export decorations and storage classes - noise between
// the return type and the name.
bool IsDeclNoise(const std::string& t) {
  static const std::set<std::string> kNoise = {
      "extern", "static", "inline", "__inline", "FORCEINLINE", "WINAPI",
      "APIENTRY", "NTAPI", "CALLBACK", "STDMETHODCALLTYPE", "STDAPICALLTYPE",
      "__stdcall", "__cdecl", "__fastcall", "XBOXAPI", "DECLSPEC_NORETURN",
      "DECLSPEC_NOTHROW", "DECLSPEC_NOINLINE", "DECLSPEC_DEPRECATED",
      "DECLSPEC_SELECTANY", "XAPI", "XBOXTOOLAPI", "_CRTIMP", "__declspec",
  };
  return kNoise.count(t) != 0;
}

// SAL annotations. Some take arguments (__out_bcount(n)), which the caller
// strips along with the token.
bool IsSalAnnotation(const std::string& t) {
  if (t.size() > 2 && t[0] == '_' && t[1] == '_') {
    static const char* kPrefixes[] = {"__in",       "__out",  "__inout",
                                      "__deref",    "__field", "__reserved",
                                      "__success",  "__range", "__nullterminated",
                                      "__notnull",  "__opt",  "__checkReturn",
                                      "__bcount",   "__ecount", "__format_string",
                                      "__typefix",  "__override"};
    for (const char* p : kPrefixes)
      if (t.compare(0, strlen(p), p) == 0) return true;
  }
  return t == "IN" || t == "OUT" || t == "OPTIONAL" || t == "FAR";
}

std::string JoinTokens(const std::vector<std::string>& toks, size_t begin,
                       size_t end) {
  std::string out;
  for (size_t i = begin; i < end; i++) {
    // "VOID *" rather than "VOID*", so appending the declarator reads as
    // `VOID *pDestination` the way the header wrote it.
    if (!out.empty() && !(out.back() == '*' || out.back() == '&')) out += ' ';
    out += toks[i];
  }
  return out;
}

}  // namespace

SdkConfig& SdkConfig::Get() {
  static SdkConfig instance;
  return instance;
}

std::string SdkConfig::ConfigPath() {
  // %APPDATA%\XEXMagic. This used to resolve to the working directory while
  // claiming to be the executable's, so the setting was saved and looked for
  // in different places depending on how the tool was started - which read as
  // "the SDK path never saves".
  return AppDataDir() + "xexmagic.json";
}

void SdkConfig::Load() {
  std::ifstream f(ConfigPath());
  if (!f) {
    // A setting written by an older build landed next to the executable or in
    // whatever directory it was started from. Take it once; the next Save()
    // puts it where it belongs.
    std::error_code ec;
    std::ifstream legacy((fs::current_path(ec) / "xexmagic.json").string());
    if (!legacy) return;
    std::stringstream ls;
    ls << legacy.rdbuf();
    const std::string ltext = ls.str();
    size_t lp = ltext.find("\"sdk_path\"");
    if (lp == std::string::npos) return;
    lp = ltext.find('"', lp + 10);
    if (lp == std::string::npos) return;
    size_t lq = ltext.find('"', lp + 1);
    if (lq == std::string::npos) return;
    std::string lvalue = ltext.substr(lp + 1, lq - lp - 1);
    std::string ignored;
    if (!lvalue.empty()) SetSdkPath(lvalue, &ignored);
    return;
  }

  std::stringstream ss;
  ss << f.rdbuf();
  const std::string text = ss.str();

  // Deliberately a hand-rolled read of one key: pulling in a JSON library for
  // a single string is not worth the dependency.
  const std::string key = "\"sdk_path\"";
  size_t p = text.find(key);
  if (p == std::string::npos) return;
  p = text.find('"', p + key.size());
  if (p == std::string::npos) return;
  size_t q = p + 1;
  std::string value;
  while (q < text.size() && text[q] != '"') {
    if (text[q] == '\\' && q + 1 < text.size()) {
      q++;
      value += text[q] == 'n' ? '\n' : text[q];
    } else {
      value += text[q];
    }
    q++;
  }

  std::string ignored;
  if (!value.empty()) {
    SetSdkPath(value, &ignored);
  }
}

void SdkConfig::Save() const {
  std::ofstream f(ConfigPath(), std::ios::trunc);
  if (!f) return;
  f << "{\n  \"sdk_path\": \"" << JsonEscape(sdk_path_) << "\"\n}\n";
}

bool SdkConfig::ScanIncludeDirs(const std::string& path,
                                std::vector<std::string>* out) const {
  std::error_code ec;
  if (!fs::is_directory(path, ec)) return false;

  // Xbox 360 XDK layouts seen in the wild. Checked in order; all that exist
  // are used, because xbox headers and shared Win32 headers live apart.
  static const char* kCandidates[] = {
      "include/xbox", "include/win32", "include", "xbox/include", "PC/include",
  };

  for (const char* rel : kCandidates) {
    fs::path p = fs::path(path) / rel;
    if (fs::is_directory(p, ec)) {
      out->push_back(p.string());
    }
  }

  // A path pointing straight at a header directory is fine too.
  if (out->empty()) {
    for (const auto& e : fs::directory_iterator(path, ec)) {
      if (e.is_regular_file(ec) && Lower(e.path().extension().string()) == ".h") {
        out->push_back(path);
        break;
      }
    }
  }

  return !out->empty();
}

void SdkConfig::ScanLibDirs(const std::string& path,
                            std::vector<std::string>* out) const {
  std::error_code ec;
  // The 360 PPC libs; win32 libs in the same SDK are host-side and useless for
  // naming guest code, so they are not indexed.
  static const char* kCandidates[] = {"lib/xbox", "libs/xbox", "xbox/lib",
                                      "lib"};
  for (const char* rel : kCandidates) {
    fs::path p = fs::path(path) / rel;
    if (fs::is_directory(p, ec)) {
      out->push_back(p.string());
      break;  // first hit wins; "lib" is the fallback for flat layouts
    }
  }
}

bool SdkConfig::SetSdkPath(const std::string& path, std::string* error) {
  const std::string trimmed = Trim(path);

  if (trimmed.empty()) {
    sdk_path_.clear();
    include_dirs_.clear();
    structs_.clear();
    indexed_ = false;
    header_count_ = 0;
    Save();
    return true;
  }

  std::error_code ec;
  if (!fs::is_directory(trimmed, ec)) {
    if (error) *error = "Not a directory: " + trimmed;
    return false;
  }

  std::vector<std::string> dirs;
  if (!ScanIncludeDirs(trimmed, &dirs)) {
    if (error) {
      *error =
          "No headers found. Expected an 'include' directory under " + trimmed +
          " (or point directly at a folder of .h files).";
    }
    return false;
  }

  sdk_path_ = trimmed;
  include_dirs_ = std::move(dirs);
  lib_dirs_.clear();
  ScanLibDirs(trimmed, &lib_dirs_);
  structs_.clear();
  symbols_.clear();
  indexed_ = false;
  header_count_ = 0;
  lib_count_ = 0;
  Save();
  return true;
}

// Extremely small struct reader: it is not a C parser and does not try to be.
// It walks `typedef struct {...} NAME;` and `struct NAME {...};` bodies, takes
// one field per line, and accumulates offsets from known primitive widths.
// Anything it cannot size marks the struct unsized so it never produces a
// confident-but-wrong match.
// Function declarations, so a decompiled body can be given the argument types
// and names it was compiled from. Declarations span lines and the struct
// parser is line-oriented, so this reads the file whole and splits it into
// statements at top-level semicolons and braces.
void SdkConfig::ParseHeaderPrototypes(const std::string& path,
                                      IndexStage* out) {
  std::ifstream f(path, std::ios::binary);
  if (!f) return;
  std::stringstream ss;
  ss << f.rdbuf();
  const std::string src = StripCommentsAndDirectives(ss.str());
  const std::string short_path = fs::path(path).filename().string();

  size_t stmt_begin = 0;
  int depth = 0;
  for (size_t i = 0; i <= src.size(); i++) {
    char c = i < src.size() ? src[i] : ';';
    if (c == '(' || c == '[') { depth++; continue; }
    if (c == ')' || c == ']') { if (depth > 0) depth--; continue; }
    if (depth > 0) continue;
    if (c != ';' && c != '{' && c != '}') continue;

    std::string stmt = src.substr(stmt_begin, i - stmt_begin);
    stmt_begin = i + 1;
    // A brace means a body or an aggregate - either way not a declaration.
    if (c != ';') continue;

    size_t open = stmt.find('(');
    if (open == std::string::npos) continue;
    // Function pointer typedefs and anything with a nested declarator are not
    // callable names.
    std::string head = Trim(stmt.substr(0, open));
    if (head.empty() || head.find("typedef") != std::string::npos) continue;

    // The parameter list is the outermost parenthesised group; anything after
    // its close (an inline body, a trailing qualifier) disqualifies it.
    int d = 0;
    size_t close = std::string::npos;
    for (size_t j = open; j < stmt.size(); j++) {
      if (stmt[j] == '(') d++;
      else if (stmt[j] == ')' && --d == 0) { close = j; break; }
    }
    if (close == std::string::npos) continue;
    if (!Trim(stmt.substr(close + 1)).empty()) continue;

    std::vector<std::string> head_toks = Tokenize(head);
    while (!head_toks.empty() && IsDeclNoise(head_toks.back()))
      head_toks.pop_back();
    if (head_toks.size() < 2) continue;  // needs a return type and a name
    std::string name = head_toks.back();
    if (!IsIdentChar(name[0]) || std::isdigit((unsigned char)name[0])) continue;
    head_toks.pop_back();
    // A macro invocation, not a declaration.
    if (head_toks.back() == ",") continue;

    std::vector<std::string> ret_toks;
    for (const auto& t : head_toks)
      if (!IsDeclNoise(t)) ret_toks.push_back(t);
    if (ret_toks.empty()) continue;

    SdkPrototype proto;
    proto.name = name;
    proto.header = short_path;
    proto.ret = JoinTokens(ret_toks, 0, ret_toks.size());

    bool ok = true;
    for (const auto& raw : SplitTopLevel(stmt.substr(open + 1, close - open - 1))) {
      std::vector<std::string> toks = Tokenize(raw);
      // Drop SAL annotations, including the argument list of the ones that
      // take one.
      std::vector<std::string> clean;
      for (size_t t = 0; t < toks.size(); t++) {
        if (!IsSalAnnotation(toks[t])) { clean.push_back(toks[t]); continue; }
        if (t + 1 < toks.size() && toks[t + 1] == "(") {
          int pd = 0;
          for (t++; t < toks.size(); t++) {
            if (toks[t] == "(") pd++;
            else if (toks[t] == ")" && --pd == 0) break;
          }
        }
      }
      if (clean.empty()) continue;
      if (clean.size() == 1 &&
          (clean[0] == "void" || clean[0] == "VOID"))
        continue;  // f(void)
      if (clean.size() == 1 && clean[0] == ".") { ok = false; break; }  // varargs

      // A trailing array subscript is a pointer in the ABI; drop it and keep
      // the declarator a pointer.
      bool array = false;
      while (!clean.empty() && clean.back() == "]") {
        int bd = 0;
        while (!clean.empty()) {
          if (clean.back() == "]") bd++;
          else if (clean.back() == "[") { if (--bd == 0) { clean.pop_back(); break; } }
          clean.pop_back();
        }
        array = true;
      }
      if (clean.empty()) { ok = false; break; }

      std::string pname, ptype;
      // The name is the trailing identifier, when there is a type in front of
      // it. `f(HANDLE)` declares an unnamed parameter, not a variable, and
      // `unsigned long` ends in an identifier that is part of the type.
      static const std::set<std::string> kTypeWords = {
          "void",  "char",     "short",  "int",    "long",   "unsigned",
          "signed", "float",   "double", "const",  "volatile", "struct",
          "union", "enum",     "VOID",   "CONST",  "__int64"};
      if (clean.size() >= 2 && IsIdentChar(clean.back()[0]) &&
          !std::isdigit((unsigned char)clean.back()[0]) &&
          !kTypeWords.count(clean.back())) {
        pname = clean.back();
        clean.pop_back();
      }
      ptype = JoinTokens(clean, 0, clean.size());
      if (array) ptype += " *";
      if (ptype.empty()) { ok = false; break; }
      proto.params.push_back({ptype, pname});
    }
    if (!ok) continue;
    // Later headers do not get to overwrite an earlier declaration of the same
    // function; the first one found wins and stays stable across runs, which
    // is why the merge below happens in file order.
    out->prototypes.emplace_back(name, std::move(proto));
  }
}

void SdkConfig::ParseHeader(const std::string& path, IndexStage* out) {
  std::ifstream f(path);
  if (!f) return;
  out->header = true;

  std::string line;
  bool in_struct = false;
  // The opening brace is very often on the line AFTER the typedef, so the
  // header line only arms the parser; the brace is what enters the body.
  bool pending = false;
  int depth = 0;
  SdkStruct cur;
  uint32_t offset = 0;
  bool sizable = true;

  // Nested `struct {` / `union {` members. A union's width is its widest
  // member, a struct's is the sum -- computed here rather than assumed, so
  // types like in_addr resolve from their actual declaration.
  struct Scope {
    bool is_union;
    uint32_t saved_offset;  // enclosing offset to restore on close
    uint32_t max_member;    // widest member seen (unions)
    uint32_t unused;
    bool sizable;
  };
  std::vector<Scope> scopes;

  const std::string short_path =
      fs::path(path).filename().string();

  while (std::getline(f, line)) {
    // Strip // comments; block comments are left alone (rare mid-declaration).
    size_t c = line.find("//");
    if (c != std::string::npos) line = line.substr(0, c);
    std::string t = Trim(line);
    if (t.empty()) continue;

    if (!in_struct) {
      if (pending) {
        // Armed by a previous line; the body starts at the brace.
        if (t.find('{') != std::string::npos) {
          in_struct = true;
          pending = false;
          depth = 1;
        } else if (t.find(';') != std::string::npos) {
          pending = false;  // it was a declaration after all
        }
        continue;
      }

      const bool is_typedef = t.rfind("typedef struct", 0) == 0;
      const bool is_plain = t.rfind("struct ", 0) == 0;
      if (!is_typedef && !is_plain) continue;
      if (t.find(';') != std::string::npos && t.find('{') == std::string::npos) {
        continue;  // forward declaration
      }

      cur = SdkStruct();
      cur.header = short_path;
      offset = 0;
      sizable = true;

      // struct NAME {  -> take the name here; typedefs name it at the close.
      if (is_plain) {
        std::string rest = Trim(t.substr(7));
        size_t sp = rest.find_first_of(" \t{:");
        cur.name = sp == std::string::npos ? rest : rest.substr(0, sp);
      }
      // typedef struct _NAME  -> remember the tag; a typedef with no trailing
      // name (rare) then still has something usable.
      if (is_typedef && cur.name.empty()) {
        std::string rest = Trim(t.substr(14));
        size_t sp = rest.find_first_of(" \t{");
        std::string tag = sp == std::string::npos ? rest : rest.substr(0, sp);
        while (!tag.empty() && tag[0] == '_') tag.erase(tag.begin());
        cur.name = tag;
      }

      if (t.find('{') != std::string::npos) {
        in_struct = true;
        depth = 1;
      } else {
        pending = true;
      }
      continue;
    }

    // Inside a struct body.
    const int opens = (int)std::count(t.begin(), t.end(), '{');
    const int closes = (int)std::count(t.begin(), t.end(), '}');

    // A nested `struct {` / `union {` member. Push a scope and accumulate it
    // separately; on close it becomes one field whose width is the sum (struct)
    // or the widest member (union). This is what lets in_addr and friends be
    // computed instead of hardcoded.
    if (opens && closes == 0 &&
        (t.rfind("union", 0) == 0 || t.rfind("struct", 0) == 0)) {
      scopes.push_back({t.rfind("union", 0) == 0, offset, 0, 0, true});
      offset = 0;
      depth += opens;
      continue;
    }

    depth += opens;
    depth -= closes;

    // Closing a nested member: `} name;`. Must START with the brace -- a line
    // like `struct { u_short a,b; } S_un_w;` also contains one but is a whole
    // member, handled further down.
    if (t[0] == '}' && !scopes.empty() && depth >= 1) {
      Scope sc = scopes.back();
      scopes.pop_back();

      const uint32_t inner = sc.is_union ? sc.max_member : offset;

      size_t brace = t.find('}');
      std::string tail = Trim(t.substr(brace + 1));
      if (!tail.empty() && tail.back() == ';') tail.pop_back();
      tail = Trim(tail);

      offset = sc.saved_offset;

      if (inner && sc.sizable) {
        const uint32_t align = inner >= 8 ? 8 : (inner >= 4 ? 4 : inner);
        if (align > 1 && (offset % align)) offset += align - (offset % align);

        SdkField fld;
        fld.type = sc.is_union ? "union" : "struct";
        fld.name = tail.empty() ? "" : tail;
        fld.offset = offset;
        fld.size = inner;
        cur.fields.push_back(fld);
        offset += inner;
      } else {
        sizable = false;
      }

      // Feed the completed member up to an enclosing union.
      if (!scopes.empty() && scopes.back().is_union) {
        scopes.back().max_member =
            std::max(scopes.back().max_member, inner);
      }
      continue;
    }

    if (depth <= 0) {
      // Closing line: `} NAME;` for typedefs.
      size_t brace = t.find('}');
      std::string tail = Trim(t.substr(brace + 1));
      if (!tail.empty() && tail.back() == ';') tail.pop_back();
      tail = Trim(tail);
      if (!tail.empty()) {
        // The typedef name is what code actually writes, so it wins over the
        // struct tag picked up at the opening line.
        size_t comma = tail.find(',');
        std::string typedef_name =
            Trim(comma == std::string::npos ? tail : tail.substr(0, comma));
        // Strip a pointer alias (`} FOO, *PFOO;` -> FOO).
        while (!typedef_name.empty() &&
               (typedef_name[0] == '*' || typedef_name[0] == ' ')) {
          typedef_name.erase(typedef_name.begin());
        }
        if (!typedef_name.empty()) cur.name = typedef_name;
      }

      if (!cur.name.empty() && !cur.fields.empty()) {
        cur.size = sizable ? offset : 0;
        out->structs.push_back(cur);
      }
      in_struct = false;
      continue;
    }

    // One field per line: `TYPE name;` / `TYPE name[4];` / `TYPE* name;`
    if (t.back() != ';') continue;
    if (t.rfind("typedef", 0) == 0) continue;
    if (t[0] == '}') continue;  // handled by the scope close above
    std::string decl = t.substr(0, t.size() - 1);

    // A whole nested aggregate on one line: `struct { u_short a,b; } name;`
    // Its width is computed from the members between the braces, the same way
    // the multi-line form is, so no type needs a hardcoded size.
    if (decl.rfind("struct", 0) == 0 || decl.rfind("union", 0) == 0) {
      const bool is_union = decl.rfind("union", 0) == 0;
      size_t ob = decl.find('{'), cb = decl.rfind('}');
      if (ob == std::string::npos || cb == std::string::npos || cb < ob) continue;

      const std::string inner_text = decl.substr(ob + 1, cb - ob - 1);
      const std::string member_name = Trim(decl.substr(cb + 1));

      uint32_t total = 0, widest = 0;
      bool inner_ok = true;
      size_t sp = 0;
      while (sp < inner_text.size()) {
        size_t semi = inner_text.find(';', sp);
        std::string one = Trim(inner_text.substr(
            sp, semi == std::string::npos ? semi : semi - sp));
        if (!one.empty()) {
          size_t ls = one.find_last_of(" \t*");
          if (ls == std::string::npos) { inner_ok = false; break; }
          const std::string ty = Trim(one.substr(0, ls + 1));
          // `u_short s_w1,s_w2;` declares two members of the same type.
          const std::string names = one.substr(ls + 1);
          uint32_t count = 1;
          for (char ch : names) if (ch == ',') count++;
          uint32_t w = IsPointer(one) ? 4 : PrimitiveSize(ty);
          if (!w) { inner_ok = false; break; }
          total += w * count;
          widest = std::max(widest, w * count);
        }
        if (semi == std::string::npos) break;
        sp = semi + 1;
      }

      const uint32_t inner = is_union ? widest : total;
      if (inner_ok && inner) {
        const uint32_t align = inner >= 8 ? 8 : (inner >= 4 ? 4 : inner);
        if (align > 1 && (offset % align)) offset += align - (offset % align);
        SdkField fld;
        fld.type = is_union ? "union" : "struct";
        fld.name = member_name;
        fld.offset = offset;
        fld.size = inner;
        cur.fields.push_back(fld);
        if (!scopes.empty() && scopes.back().is_union) {
          scopes.back().max_member = std::max(scopes.back().max_member, inner);
        } else {
          offset += inner;
        }
      } else {
        if (!scopes.empty()) scopes.back().sizable = false; else sizable = false;
      }
      continue;
    }

    // `DWORD a, b, c;` declares three fields of one type. Expand to one
    // declarator each so every field gets its own offset, instead of a single
    // field named "a, b, c".
    std::vector<std::string> decls;
    if (decl.find(',') != std::string::npos &&
        decl.find('(') == std::string::npos) {
      size_t ls = decl.find_last_of(" \t*", decl.find(','));
      if (ls != std::string::npos) {
        const std::string shared_type = Trim(decl.substr(0, ls + 1));
        std::string rest = decl.substr(ls + 1);
        size_t sp = 0;
        while (true) {
          size_t comma = rest.find(',', sp);
          std::string one = Trim(rest.substr(
              sp, comma == std::string::npos ? comma : comma - sp));
          if (!one.empty()) decls.push_back(shared_type + " " + one);
          if (comma == std::string::npos) break;
          sp = comma + 1;
        }
      }
    }
    if (decls.empty()) decls.push_back(decl);

    for (const std::string& one_decl : decls) {
    size_t last_space = one_decl.find_last_of(" \t*");
    if (last_space == std::string::npos) continue;

    SdkField fld;
    fld.name = Trim(one_decl.substr(last_space + 1));
    fld.type = Trim(one_decl.substr(0, last_space + 1));
    if (fld.name.empty() || fld.type.empty()) continue;

    // Array suffix.
    size_t br = fld.name.find('[');
    if (br != std::string::npos) {
      std::string count = fld.name.substr(br + 1);
      size_t rb = count.find(']');
      if (rb != std::string::npos) count = count.substr(0, rb);
      fld.name = fld.name.substr(0, br);
      fld.array_count = (uint32_t)strtoul(count.c_str(), nullptr, 0);
      if (fld.array_count == 0) fld.array_count = 1;  // [] or a macro
    }

    uint32_t elem = IsPointer(fld.type) ? 4 : PrimitiveSize(Trim(fld.type));
    // Trailing '*' ends up on the type via find_last_of above.
    if (elem == 0 && IsPointer(one_decl)) elem = 4;

    fld.offset = offset;
    fld.size = elem * fld.array_count;

    if (elem == 0) {
      // Unknown width. Mark the innermost scope unsized, not the whole struct,
      // so an unresolvable member inside a union does not poison the rest.
      if (!scopes.empty()) {
        scopes.back().sizable = false;
      } else {
        sizable = false;
      }
    } else {
      const uint32_t align = elem >= 8 ? 8 : elem;
      if (align > 1 && (offset % align)) offset += align - (offset % align);
      fld.offset = offset;
      offset += fld.size;

      // Inside a union every member starts at 0 and the widest one wins.
      if (!scopes.empty() && scopes.back().is_union) {
        scopes.back().max_member =
            std::max(scopes.back().max_member, fld.size);
        offset = 0;
        fld.offset = 0;
      }
    }

    cur.fields.push_back(fld);
    }  // for each declarator on this line
  }
}

// Pull every defined symbol out of one archive. Only symbols that own code are
// kept: an import stub or a data blob cannot be matched against a function in
// the image, so indexing them would only add noise.
void SdkConfig::ParseLib(const std::string& path, IndexStage* out) {
  std::ifstream f(path, std::ios::binary);
  if (!f) return;

  std::vector<uint8_t> data((std::istreambuf_iterator<char>(f)),
                            std::istreambuf_iterator<char>());
  if (data.empty()) return;

  std::vector<::CoffObject> objects;
  if (!ParseCoffLib(data.data(), data.size(), objects)) return;

  out->lib = true;
  const std::string lib_name = fs::path(path).filename().string();

  for (const auto& obj : objects) {
    // Size of the code in this member, attributed to its defined symbols.
    uint32_t text_size = 0;
    for (const auto& sec : obj.sections) {
      if (sec.name.rfind(".text", 0) == 0) text_size += sec.raw_size;
    }
    if (!text_size) continue;

    for (const auto& sym : obj.all_symbols) {
      if (sym.section_number <= 0) continue;      // external/absolute
      if (sym.name.empty() || sym.name[0] == '.') continue;

      // IMAGE_SYM_CLASS_EXTERNAL. Statics and internal labels cannot be what a
      // caller in the image linked against, so indexing them is noise.
      if (sym.storage_class != 2) continue;

      // MSVC data decorations: ??_C@ string literals, ??_7 vftables, ??_R type
      // descriptors. None of them are functions.
      if (sym.name.rfind("??_", 0) == 0) continue;
      // Compiler scaffolding.
      if (sym.name.rfind("__real@", 0) == 0) continue;
      if (sym.name.rfind("$", 0) == 0) continue;

      SdkSymbol s;
      s.name = sym.name;
      s.demangled = MsvcDemangle(sym.name);
      s.lib = lib_name;
      s.obj = obj.member_name;
      s.code_size = text_size;
      out->symbols.push_back(std::move(s));
    }

    BuildSigsForObject(obj, lib_name, out);
  }
}

// Turn each function in a .text section into a comparable signature.
//
// A member usually holds one function per section (MSVC compiles with function
// -level linking), but where several share a section the extent runs from one
// defined symbol to the next.
void SdkConfig::BuildSigsForObject(const ::CoffObject& obj,
                                   const std::string& lib_name,
                                   IndexStage* out) {
  for (size_t si = 0; si < obj.sections.size(); si++) {
    const CoffSection& sec = obj.sections[si];
    if (sec.name.rfind(".text", 0) != 0) continue;
    if (sec.data.empty()) continue;

    // Defined function symbols in this section, by offset.
    std::vector<std::pair<uint32_t, const CoffSymbol*>> defs;
    for (const auto& sym : obj.all_symbols) {
      if (sym.section_number != (int16_t)(si + 1)) continue;
      if (sym.storage_class != 2) continue;  // external only
      if (sym.name.empty() || sym.name[0] == '.') continue;
      if (sym.name.rfind("??_", 0) == 0) continue;
      if (sym.value >= sec.data.size()) continue;
      defs.push_back({sym.value, &sym});
    }
    if (defs.empty()) continue;
    std::sort(defs.begin(), defs.end(),
              [](const auto& a, const auto& b) { return a.first < b.first; });

    for (size_t d = 0; d < defs.size(); d++) {
      const uint32_t start = defs[d].first;
      const uint32_t end = (d + 1 < defs.size())
                               ? defs[d + 1].first
                               : (uint32_t)sec.data.size();
      if (end <= start) continue;
      const uint32_t size = end - start;

      // Too small to be distinctive: a 1-2 instruction body collides with
      // every other stub in the SDK and would produce confident wrong names.
      if (size < 32) continue;

      CodeSig sig;
      sig.name = defs[d].second->name;
      sig.demangled = MsvcDemangle(sig.name);
      sig.lib = lib_name;
      sig.size = size;

      // Relocated fields are rewritten at link time; mask them.
      for (const auto& rel : sec.relocations) {
        if (rel.offset < start || rel.offset >= end) continue;
        // PPC COFF relocs patch either a 16-bit immediate or a 24/32-bit
        // branch field. Masking the whole containing instruction word is
        // simplest and never under-masks.
        const uint32_t insn_off = (rel.offset - start) & ~3u;
        sig.mask.push_back({insn_off, 4});
      }
      std::sort(sig.mask.begin(), sig.mask.end());
      sig.mask.erase(std::unique(sig.mask.begin(), sig.mask.end()),
                     sig.mask.end());

      // A body that is mostly relocated carries almost no identity.
      if (sig.mask.size() * 4 > size / 2) continue;

      sig.hash = HashMasked(sec.data.data() + start, size, sig.mask);
      sig.body.assign(sec.data.begin() + start, sec.data.begin() + end);
      out->sigs.emplace_back(size, std::move(sig));
    }
  }
}

uint64_t SdkConfig::HashMasked(
    const uint8_t* code, uint32_t size,
    const std::vector<std::pair<uint32_t, uint32_t>>& mask) {
  // FNV-1a over the bytes, skipping masked ranges.
  uint64_t h = 1469598103934665603ULL;
  size_t mi = 0;
  for (uint32_t i = 0; i < size;) {
    while (mi < mask.size() && mask[mi].first + mask[mi].second <= i) mi++;
    if (mi < mask.size() && i >= mask[mi].first &&
        i < mask[mi].first + mask[mi].second) {
      i = mask[mi].first + mask[mi].second;  // skip the whole masked field
      continue;
    }
    h ^= code[i++];
    h *= 1099511628211ULL;
  }
  return h;
}

const std::vector<SdkConfig::CodeSig>* SdkConfig::SigsForSize(
    uint32_t size) const {
  auto it = sigs_.find(size);
  return it == sigs_.end() ? nullptr : &it->second;
}

namespace {

// Instructions per window. Eight PPC instructions is long enough that a run is
// specific to one routine, short enough to survive an inliner that dropped the
// prologue/epilogue and reordered around the edges.
constexpr uint32_t kWindowInsns = 8;
constexpr uint32_t kWindowBytes = kWindowInsns * 4;

uint64_t HashWindow(const uint8_t* p) {
  uint64_t h = 1469598103934665603ULL;
  for (uint32_t i = 0; i < kWindowBytes; i++) {
    h ^= p[i];
    h *= 1099511628211ULL;
  }
  return h;
}

// A window of nothing but zeroes, nops or a repeated instruction carries no
// identity -- it would match half the image.
bool WindowIsDistinctive(const uint8_t* p) {
  uint32_t distinct = 0;
  uint32_t words[kWindowInsns];
  for (uint32_t i = 0; i < kWindowInsns; i++) {
    words[i] = ((uint32_t)p[i * 4] << 24) | ((uint32_t)p[i * 4 + 1] << 16) |
               ((uint32_t)p[i * 4 + 2] << 8) | (uint32_t)p[i * 4 + 3];
    if (words[i] == 0 || words[i] == 0x60000000u) return false;  // 0 / nop
  }
  for (uint32_t i = 0; i < kWindowInsns; i++) {
    bool seen = false;
    for (uint32_t j = 0; j < i; j++) {
      if (words[j] == words[i]) { seen = true; break; }
    }
    if (!seen) distinct++;
  }
  return distinct >= 5;
}

}  // namespace

// Index instruction windows from every library body so inlined copies can be
// spotted inside larger functions.
//
// Only a handful of windows per body are kept -- indexing every offset would
// cost hundreds of megabytes for no extra detection, since an inlined body
// large enough to matter contains several of them.
void SdkConfig::BuildWindowIndex() {
  windows_.clear();

  // hash -> owner, plus a flag for hashes seen under more than one name.
  std::map<uint64_t, WindowOwner> owners;
  std::map<uint64_t, bool> ambiguous;

  for (const auto& [size, list] : sigs_) {
    for (const auto& sig : list) {
      if (sig.body.size() < kWindowBytes) continue;

      // Mask relocated words so a window is comparable across the link.
      std::vector<uint8_t> body = sig.body;
      for (const auto& m : sig.mask) {
        for (uint32_t i = 0; i < m.second && m.first + i < body.size(); i++) {
          body[m.first + i] = 0;
        }
      }

      const uint32_t last = (uint32_t)body.size() - kWindowBytes;
      const uint32_t step = std::max<uint32_t>(4, (last / 3) & ~3u);

      for (uint32_t off = 0; off <= last; off += step) {
        const uint8_t* p = body.data() + off;
        if (!WindowIsDistinctive(p)) continue;
        const uint64_t h = HashWindow(p);

        auto it = owners.find(h);
        if (it == owners.end()) {
          owners[h] = {sig.name, sig.demangled, sig.lib};
        } else if (it->second.name != sig.name) {
          ambiguous[h] = true;  // shared boilerplate, not an identity
        }
      }
    }
  }

  for (auto& [h, owner] : owners) {
    if (!ambiguous.count(h)) windows_[h] = owner;
  }
}

std::vector<SdkConfig::InlineHit> SdkConfig::FindInlined(const uint8_t* code,
                                                         uint32_t size) const {
  std::vector<InlineHit> hits;
  if (!code || size < kWindowBytes || windows_.empty()) return hits;

  // Every window is an independent lookup, so the image is split across the
  // cores and each slice keeps its own run list. Slices are merged in address
  // order and a run that crosses a boundary is joined by the same rule the
  // sequential walk used, so the result does not depend on how it was split.
  const uint32_t last = size - kWindowBytes;
  unsigned nthreads = std::thread::hardware_concurrency();
  if (nthreads == 0) nthreads = 4;
  const uint32_t windows = last / 4 + 1;
  if (windows < 4096) nthreads = 1;  // not worth the threads
  if (nthreads > windows) nthreads = windows;
  const uint32_t per = ((windows + nthreads - 1) / nthreads) * 4;

  std::vector<std::vector<InlineHit>> slices(nthreads);
  auto scan = [&](unsigned t) {
    // The image side cannot know which fields were relocated, so try both the
    // raw window and one with the low half-word of each instruction cleared --
    // that is where link-time fixups land (branch displacement, lis/ori
    // halves).
    std::vector<uint8_t> masked(kWindowBytes);
    auto& out = slices[t];
    const uint32_t begin = t * per;
    const uint32_t end = std::min<uint32_t>(begin + per, last + 4);
    for (uint32_t off = begin; off < end; off += 4) {
      const uint8_t* p = code + off;

      const WindowOwner* owner = nullptr;
      auto it = windows_.find(HashWindow(p));
      if (it != windows_.end()) {
        owner = &it->second;
      } else {
        memcpy(masked.data(), p, kWindowBytes);
        for (uint32_t i = 0; i < kWindowInsns; i++) {
          masked[i * 4 + 2] = 0;
          masked[i * 4 + 3] = 0;
        }
        auto it2 = windows_.find(HashWindow(masked.data()));
        if (it2 != windows_.end()) owner = &it2->second;
      }
      if (!owner) continue;

      // Extend a run rather than reporting every overlapping window.
      if (!out.empty() && out.back().name == owner->name &&
          off <= out.back().offset + out.back().insns * 4) {
        out.back().insns = (off - out.back().offset) / 4 + kWindowInsns;
        continue;
      }

      InlineHit h;
      h.name = owner->name;
      h.demangled = owner->demangled;
      h.lib = owner->lib;
      h.offset = off;
      h.insns = kWindowInsns;
      out.push_back(std::move(h));
    }
  };

  if (nthreads == 1) {
    scan(0);
  } else {
    std::vector<std::thread> pool;
    for (unsigned t = 1; t < nthreads; t++) pool.emplace_back(scan, t);
    scan(0);
    for (auto& th : pool) th.join();
  }

  for (auto& slice : slices) {
    for (auto& h : slice) {
      if (!hits.empty() && hits.back().name == h.name &&
          h.offset <= hits.back().offset + hits.back().insns * 4) {
        uint32_t reach = (h.offset - hits.back().offset) / 4 + h.insns;
        if (reach > hits.back().insns) hits.back().insns = reach;
        continue;
      }
      hits.push_back(std::move(h));
    }
  }

  return hits;
}

size_t SdkConfig::BuildIndex(bool include_libs) {
  structs_.clear();
  prototypes_.clear();
  symbols_.clear();
  sigs_.clear();
  sig_total_ = 0;
  header_count_ = 0;
  lib_count_ = 0;

  // Enumerate first, parse in parallel, merge in file order. Parsing is a
  // per-file read plus text work with nothing shared, so it scales with the
  // machine; the merge is what keeps the result identical to a serial run.
  std::error_code ec;
  std::vector<std::string> headers, libs;
  for (const auto& dir : include_dirs_) {
    for (auto it = fs::recursive_directory_iterator(dir, ec);
         it != fs::recursive_directory_iterator(); it.increment(ec)) {
      if (ec) break;
      if (!it->is_regular_file(ec)) continue;
      if (Lower(it->path().extension().string()) != ".h") continue;
      headers.push_back(it->path().string());
    }
  }
  // The libraries never change between runs, so their symbols and signatures
  // come back from Libraries.XDG instead of being parsed again.
  bool libs_cached = false;
  uint64_t lib_fingerprint = 0;
  if (include_libs) {
    lib_fingerprint = LibraryFingerprint();
    libs_cached = LoadLibraryCache(lib_fingerprint);
  }

  if (include_libs && !libs_cached) {
    for (const auto& dir : lib_dirs_) {
      for (auto it = fs::recursive_directory_iterator(dir, ec);
           it != fs::recursive_directory_iterator(); it.increment(ec)) {
        if (ec) break;
        if (!it->is_regular_file(ec)) continue;
        if (Lower(it->path().extension().string()) != ".lib") continue;
        libs.push_back(it->path().string());
      }
    }
  }
  std::sort(headers.begin(), headers.end());
  std::sort(libs.begin(), libs.end());

  const size_t nfiles = headers.size() + libs.size();
  std::vector<IndexStage> stages(nfiles);
  {
    std::atomic<size_t> next{0};
    unsigned nthreads = std::thread::hardware_concurrency();
    if (nthreads == 0) nthreads = 4;
    auto worker = [&]() {
      for (;;) {
        size_t i = next.fetch_add(1);
        if (i >= nfiles) return;
        if (i < headers.size()) {
          ParseHeader(headers[i], &stages[i]);
          ParseHeaderPrototypes(headers[i], &stages[i]);
        } else {
          ParseLib(libs[i - headers.size()], &stages[i]);
        }
      }
    };
    std::vector<std::thread> pool;
    for (unsigned t = 1; t < nthreads; t++) pool.emplace_back(worker);
    worker();
    for (auto& t : pool) t.join();
  }

  for (auto& st : stages) {
    if (st.header) header_count_++;
    if (st.lib) lib_count_++;
    for (auto& s : st.structs) structs_.push_back(std::move(s));
    for (auto& p : st.prototypes) prototypes_.emplace(p.first, std::move(p.second));
    for (auto& s : st.symbols) symbols_.push_back(std::move(s));
    for (auto& s : st.sigs) {
      sigs_[s.first].push_back(std::move(s.second));
      sig_total_++;
    }
  }

  std::sort(structs_.begin(), structs_.end(),
            [](const SdkStruct& a, const SdkStruct& b) {
              if (a.name != b.name) return a.name < b.name;
              return a.fields.size() > b.fields.size();  // richest first
            });
  // The same header is reachable through more than one include dir, so the
  // same declaration gets parsed twice. Keep one per name.
  structs_.erase(std::unique(structs_.begin(), structs_.end(),
                             [](const SdkStruct& a, const SdkStruct& b) {
                               return a.name == b.name;
                             }),
                 structs_.end());

  ResolveNestedSizes();

  if (include_libs && !libs_cached) {
    // Sorted and deduplicated before it is cached, so a reload gets the same
    // vector the parse produced rather than repeating this every start.
    std::sort(symbols_.begin(), symbols_.end(),
              [](const SdkSymbol& a, const SdkSymbol& b) {
                return a.name < b.name;
              });
    symbols_.erase(
        std::unique(symbols_.begin(), symbols_.end(),
                    [](const SdkSymbol& a, const SdkSymbol& b) {
                      return a.name == b.name && a.lib == b.lib;
                    }),
        symbols_.end());
  }

  // Already in hand when the cache supplied it; otherwise derive it from the
  // bodies, then store it and let the bodies go.
  if (!libs_cached) BuildWindowIndex();
  if (include_libs && !libs_cached) SaveLibraryCache(lib_fingerprint);

  indexed_ = true;
  return structs_.size();
}

// ---------------------------------------------------------------------------
// Library cache (%APPDATA%\XEXMagic\Libraries.XDG)
// ---------------------------------------------------------------------------

namespace {

// Little-endian binary, written and read in one pass. A text format would be
// larger than the archives it describes.
struct XdgWriter {
  std::string buf;
  void U32(uint32_t v) { buf.append((const char*)&v, 4); }
  void U64(uint64_t v) { buf.append((const char*)&v, 8); }
  void Str(const std::string& s) {
    U32((uint32_t)s.size());
    buf.append(s);
  }
  void Bytes(const std::vector<uint8_t>& b) {
    U32((uint32_t)b.size());
    buf.append((const char*)b.data(), b.size());
  }
};

struct XdgReader {
  const char* p = nullptr;
  const char* end = nullptr;
  bool ok = true;
  uint32_t U32() {
    if (p + 4 > end) { ok = false; return 0; }
    uint32_t v;
    memcpy(&v, p, 4);
    p += 4;
    return v;
  }
  uint64_t U64() {
    if (p + 8 > end) { ok = false; return 0; }
    uint64_t v;
    memcpy(&v, p, 8);
    p += 8;
    return v;
  }
  std::string Str() {
    uint32_t n = U32();
    if (!ok || p + n > end) { ok = false; return ""; }
    std::string s(p, n);
    p += n;
    return s;
  }
  std::vector<uint8_t> Bytes() {
    uint32_t n = U32();
    if (!ok || p + n > end) { ok = false; return {}; }
    std::vector<uint8_t> b((const uint8_t*)p, (const uint8_t*)p + n);
    p += n;
    return b;
  }
};

constexpr uint32_t kXdgMagic = 0x4C474458;  // "XDGL"

void HashInto(uint64_t& h, const void* data, size_t len) {
  const uint8_t* p = (const uint8_t*)data;
  for (size_t i = 0; i < len; i++) {
    h ^= p[i];
    h *= 1099511628211ull;
  }
}

}  // namespace

std::string SdkConfig::LibraryCachePath() {
  return AppDataDir() + "Libraries.XDG";
}

uint64_t SdkConfig::LibraryFingerprint() const {
  uint64_t h = 1469598103934665603ull;
  HashInto(h, sdk_path_.data(), sdk_path_.size());
  std::error_code ec;
  std::vector<std::string> libs;
  for (const auto& dir : lib_dirs_) {
    for (auto it = fs::recursive_directory_iterator(dir, ec);
         it != fs::recursive_directory_iterator(); it.increment(ec)) {
      if (ec) break;
      if (!it->is_regular_file(ec)) continue;
      if (Lower(it->path().extension().string()) != ".lib") continue;
      libs.push_back(it->path().string());
    }
  }
  std::sort(libs.begin(), libs.end());
  for (const auto& path : libs) {
    HashInto(h, path.data(), path.size());
    uintmax_t size = fs::file_size(path, ec);
    HashInto(h, &size, sizeof(size));
    auto when = fs::last_write_time(path, ec).time_since_epoch().count();
    HashInto(h, &when, sizeof(when));
  }
  return h;
}

void SdkConfig::SaveLibraryCache(uint64_t fingerprint) const {
  XdgWriter w;
  w.buf.reserve(1 << 24);
  w.U32(kXdgMagic);
  w.U32(kSchemaVersion);
  w.Str(BuildId());
  w.U64(fingerprint);
  w.U64(lib_count_);

  w.U32((uint32_t)symbols_.size());
  for (const auto& s : symbols_) {
    w.Str(s.name);
    w.Str(s.demangled);
    w.Str(s.lib);
    w.Str(s.obj);
    w.U32(s.code_size);
  }

  w.U32((uint32_t)sigs_.size());
  for (const auto& kv : sigs_) {
    w.U32(kv.first);
    w.U32((uint32_t)kv.second.size());
    for (const auto& sig : kv.second) {
      w.Str(sig.name);
      w.Str(sig.demangled);
      w.Str(sig.lib);
      w.U32(sig.size);
      w.U32((uint32_t)sig.mask.size());
      for (const auto& m : sig.mask) {
        w.U32(m.first);
        w.U32(m.second);
      }
      w.U64(sig.hash);
    }
  }

  // The window index rather than the bodies it was built from. Bodies are a
  // copy of every library function's code - 222 MB of it - and nothing but
  // BuildWindowIndex ever reads them, so the derived index is both smaller and
  // ready to use.
  std::map<std::string, uint32_t> owner_id;
  std::vector<const WindowOwner*> owners;
  for (const auto& kv : windows_) {
    const std::string key = kv.second.lib + "|" + kv.second.name;
    if (owner_id.emplace(key, (uint32_t)owners.size()).second)
      owners.push_back(&kv.second);
  }
  w.U32((uint32_t)owners.size());
  for (const auto* o : owners) {
    w.Str(o->name);
    w.Str(o->demangled);
    w.Str(o->lib);
  }
  w.U32((uint32_t)windows_.size());
  for (const auto& kv : windows_) {
    w.U64(kv.first);
    w.U32(owner_id[kv.second.lib + "|" + kv.second.name]);
  }

  const std::string path = LibraryCachePath();
  FILE* fp = fopen(path.c_str(), "wb");
  if (!fp) {
    printf("Libraries.XDG: could not be written\n");
    fflush(stdout);
    return;
  }
  fwrite(w.buf.data(), 1, w.buf.size(), fp);
  fclose(fp);
  printf("Libraries.XDG: wrote %zu bytes (%zu symbols, %zu signatures)\n",
         w.buf.size(), symbols_.size(), sig_total_);
  fflush(stdout);
}

bool SdkConfig::LoadLibraryCache(uint64_t fingerprint) {
  const std::string path = LibraryCachePath();
  FILE* fp = fopen(path.c_str(), "rb");
  if (!fp) return false;
  fseek(fp, 0, SEEK_END);
  long len = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  std::string data((size_t)(len > 0 ? len : 0), '\0');
  size_t got = data.empty() ? 0 : fread(&data[0], 1, data.size(), fp);
  fclose(fp);
  if (data.empty() || got != data.size()) return false;

  XdgReader r{data.data(), data.data() + data.size()};
  if (r.U32() != kXdgMagic) return false;
  if (r.U32() != kSchemaVersion) return false;
  const std::string built = r.Str();
  if (!r.ok) return false;
  if (built != BuildId()) {
    printf("Libraries.XDG: written by XEXMagic %s, this is %s - reindexing\n",
           built.c_str(), BuildId().c_str());
    fflush(stdout);
    return false;
  }
  if (r.U64() != fingerprint) {
    printf("Libraries.XDG: the SDK libraries changed - reindexing\n");
    fflush(stdout);
    return false;
  }
  const uint64_t libs = r.U64();

  std::vector<SdkSymbol> symbols;
  uint32_t n = r.U32();
  symbols.reserve(n);
  for (uint32_t i = 0; i < n && r.ok; i++) {
    SdkSymbol s;
    s.name = r.Str();
    s.demangled = r.Str();
    s.lib = r.Str();
    s.obj = r.Str();
    s.code_size = r.U32();
    symbols.push_back(std::move(s));
  }

  std::map<uint32_t, std::vector<CodeSig>> sigs;
  size_t total = 0;
  uint32_t groups = r.U32();
  for (uint32_t g = 0; g < groups && r.ok; g++) {
    uint32_t size = r.U32();
    uint32_t count = r.U32();
    auto& vec = sigs[size];
    vec.reserve(count);
    for (uint32_t i = 0; i < count && r.ok; i++) {
      CodeSig sig;
      sig.name = r.Str();
      sig.demangled = r.Str();
      sig.lib = r.Str();
      sig.size = r.U32();
      uint32_t nmask = r.U32();
      for (uint32_t m = 0; m < nmask && r.ok; m++) {
        uint32_t off = r.U32();
        uint32_t l = r.U32();
        sig.mask.emplace_back(off, l);
      }
      sig.hash = r.U64();
      vec.push_back(std::move(sig));
      total++;
    }
  }

  std::vector<WindowOwner> owners;
  uint32_t nowners = r.U32();
  owners.reserve(nowners);
  for (uint32_t i = 0; i < nowners && r.ok; i++) {
    WindowOwner o;
    o.name = r.Str();
    o.demangled = r.Str();
    o.lib = r.Str();
    owners.push_back(std::move(o));
  }
  std::map<uint64_t, WindowOwner> windows;
  uint32_t nwindows = r.U32();
  for (uint32_t i = 0; i < nwindows && r.ok; i++) {
    uint64_t h = r.U64();
    uint32_t id = r.U32();
    if (id >= owners.size()) { r.ok = false; break; }
    windows.emplace(h, owners[id]);
  }

  if (!r.ok) {
    printf("Libraries.XDG: truncated - reindexing\n");
    fflush(stdout);
    return false;
  }

  symbols_ = std::move(symbols);
  sigs_ = std::move(sigs);
  windows_ = std::move(windows);
  sig_total_ = total;
  lib_count_ = (size_t)libs;
  printf("Libraries.XDG: loaded %ld bytes (%zu symbols, %zu signatures, %zu "
         "windows from %zu libs)\n", len, symbols_.size(), sig_total_,
         windows_.size(), lib_count_);
  fflush(stdout);
  return true;
}

const SdkConfig::SdkPrototype* SdkConfig::FindPrototype(
    const std::string& name) const {
  auto it = prototypes_.find(name);
  if (it != prototypes_.end()) return &it->second;
  // Statically linked C++ code carries a demangled signature as its name;
  // the declaration is indexed under the bare function name.
  size_t open = name.find('(');
  if (open == std::string::npos) return nullptr;
  size_t end = open;
  while (end > 0 && std::isspace((unsigned char)name[end - 1])) end--;
  size_t start = end;
  while (start > 0 && IsIdentChar(name[start - 1])) start--;
  if (start == end) return nullptr;
  it = prototypes_.find(name.substr(start, end - start));
  return it != prototypes_.end() ? &it->second : nullptr;
}

const SdkConfig::SdkSymbol* SdkConfig::FindSymbol(const std::string& name) const {
  for (const auto& s : symbols_) {
    if (s.name == name) return &s;
  }
  // Undecorated match: the image's import table carries plain names while the
  // archive may hold the decorated form.
  const std::string n = Lower(name);
  for (const auto& s : symbols_) {
    if (Lower(s.demangled) == n) return &s;
  }
  return nullptr;
}

std::vector<const SdkConfig::SdkSymbol*> SdkConfig::SearchSymbols(
    const std::string& needle, size_t limit) const {
  std::vector<const SdkSymbol*> out;
  const std::string n = Lower(needle);
  for (const auto& s : symbols_) {
    if (n.empty() || Lower(s.name).find(n) != std::string::npos ||
        Lower(s.demangled).find(n) != std::string::npos) {
      out.push_back(&s);
      if (out.size() >= limit) break;
    }
  }
  return out;
}

// A field whose type is another SDK struct starts out unsized, which leaves the
// whole enclosing struct unsized and every later offset at 0 -- useless for
// matching. Once every header is parsed those nested types ARE known, so walk
// the set repeatedly, filling in what became resolvable, until nothing changes.
void SdkConfig::ResolveNestedSizes() {
  std::map<std::string, size_t> by_name;
  for (size_t i = 0; i < structs_.size(); i++) {
    by_name[Lower(structs_[i].name)] = i;
  }

  // Bounded: each pass must resolve at least one struct or we stop. Cyclic or
  // genuinely unknown types simply stay unsized.
  for (int pass = 0; pass < 8; pass++) {
    bool progress = false;

    for (auto& s : structs_) {
      if (s.size) continue;  // already sized

      uint32_t offset = 0;
      bool sizable = true;

      for (auto& f : s.fields) {
        uint32_t elem = f.size / (f.array_count ? f.array_count : 1);

        if (elem == 0) {
          std::string base = Trim(f.type);
          // Drop qualifiers so "const XNADDR" resolves.
          for (const char* q : {"const ", "volatile ", "struct ", "union "}) {
            size_t at = base.find(q);
            if (at == 0) base = Trim(base.substr(strlen(q)));
          }
          auto it = by_name.find(Lower(base));
          if (it != by_name.end() && structs_[it->second].size &&
              &structs_[it->second] != &s) {
            elem = structs_[it->second].size;
          }
        }

        if (elem == 0) {
          sizable = false;
          break;
        }

        const uint32_t align = elem >= 8 ? 8 : (elem >= 4 ? 4 : elem);
        if (align > 1 && (offset % align)) offset += align - (offset % align);
        f.offset = offset;
        f.size = elem * f.array_count;
        offset += f.size;
      }

      if (sizable && offset) {
        s.size = offset;
        progress = true;
      }
    }

    if (!progress) break;
  }
}

const SdkStruct* SdkConfig::FindStruct(const std::string& name) const {
  const std::string needle = Lower(name);
  for (const auto& s : structs_) {
    if (Lower(s.name) == needle) return &s;
  }
  return nullptr;
}

std::vector<const SdkStruct*> SdkConfig::SearchStructs(const std::string& needle,
                                                       size_t limit) const {
  std::vector<const SdkStruct*> out;
  const std::string n = Lower(needle);
  for (const auto& s : structs_) {
    if (n.empty() || Lower(s.name).find(n) != std::string::npos) {
      out.push_back(&s);
      if (out.size() >= limit) break;
    }
  }
  return out;
}

std::vector<SdkConfig::Candidate> SdkConfig::MatchLayout(
    uint32_t size, const std::vector<uint32_t>& offsets, size_t limit) const {
  std::vector<Candidate> out;

  for (const auto& s : structs_) {
    if (s.fields.empty()) continue;

    int score = 0;
    size_t hits = 0;
    for (uint32_t off : offsets) {
      for (const auto& f : s.fields) {
        if (f.offset == off) {
          hits++;
          score += 10;
          break;
        }
      }
    }

    // Every observed offset must land on a declared field. A struct that only
    // explains some of the accesses is the wrong struct, not a partial match.
    if (!offsets.empty() && hits != offsets.size()) continue;

    std::string why = std::to_string(hits) + "/" +
                      std::to_string(offsets.size()) + " offsets";

    if (size && s.size) {
      if (s.size == size) {
        score += 50;
        why += ", exact size";
      } else if (s.size >= size) {
        score += 5;
        why += ", fits";
      } else {
        continue;  // declared struct is smaller than what the code touches
      }
    }

    out.push_back({&s, score, why});
  }

  std::sort(out.begin(), out.end(),
            [](const Candidate& a, const Candidate& b) {
              if (a.score != b.score) return a.score > b.score;
              return a.def->name < b.def->name;
            });
  if (out.size() > limit) out.resize(limit);
  return out;
}

std::string SdkConfig::ConfigJson() const {
  std::ostringstream o;
  o << "{\"sdk_path\":\"" << JsonEscape(sdk_path_) << "\",\"include_dirs\":[";
  for (size_t i = 0; i < include_dirs_.size(); i++) {
    if (i) o << ",";
    o << "\"" << JsonEscape(include_dirs_[i]) << "\"";
  }
  o << "],\"lib_dirs\":[";
  for (size_t i = 0; i < lib_dirs_.size(); i++) {
    if (i) o << ",";
    o << "\"" << JsonEscape(lib_dirs_[i]) << "\"";
  }
  o << "],\"indexed\":" << (indexed_ ? "true" : "false")
    << ",\"structs\":" << structs_.size()
    << ",\"headers\":" << header_count_
    << ",\"symbols\":" << symbols_.size()
    << ",\"sigs\":" << sig_total_
    << ",\"libs\":" << lib_count_ << "}";
  return o.str();
}

std::string SdkConfig::StructJson(const SdkStruct& s) const {
  std::ostringstream o;
  o << "{\"name\":\"" << JsonEscape(s.name) << "\",\"header\":\""
    << JsonEscape(s.header) << "\",\"size\":" << s.size << ",\"fields\":[";
  for (size_t i = 0; i < s.fields.size(); i++) {
    const auto& f = s.fields[i];
    if (i) o << ",";
    o << "{\"offset\":" << f.offset << ",\"type\":\"" << JsonEscape(f.type)
      << "\",\"name\":\"" << JsonEscape(f.name) << "\",\"size\":" << f.size
      << ",\"count\":" << f.array_count << "}";
  }
  o << "]}";
  return o.str();
}

}  // namespace xexmagic
