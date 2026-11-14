// py_stdlib.hpp -- the pieces of CPython's standard library that many typed ports each wrote for themselves,
// matching CPython 3.14 on Windows including the exception types and the message text:
//
//   py::int_parse(s, base)            int(s, base): bases 0 and 2..36, signs, 0x/0o/0b, underscores, Unicode digits
//   py::SystemExit, ImportError, ModuleNotFoundError, StopIteration
//   py::subprocess::list2cmdline      (running a program is py_subprocess.hpp: it needs <windows.h>)
//   py::tempfile                      gettempdir, mkdtemp, TemporaryDirectory
//   py::codecs                        utf-16-be decode / encode
//   py::raise_named / translate_errors  a native reader's (kind, message) error as the Python exception of that name
//   py::contextlib::redirect_stdout / redirect_stdout_null   standard output captured / dropped for a scope (RAII)
//
// (Included at the end of py.hpp.)
#pragma once

#include <fstream>
#include <mutex>
#include <random>
#include <sstream>
#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#include <process.h>
#endif

#define PY_STDLIB_TEXT_CTOR(Name, Base) template <class S, class = decltype(std::declval<const S&>().utf8())> explicit Name(const S& m) : Base(m.utf8()) {}

namespace py {

// ------------------------------------------------------------------------------------------ exceptions
struct ImportError : Exception {
    std::optional<str> name, path;
    ImportError() : Exception() {}
    explicit ImportError(const std::string& m) : Exception(m) {}
    explicit ImportError(const char* m) : Exception(m) {}
    PY_STDLIB_TEXT_CTOR(ImportError, Exception)
    const char* type() const override { return "ImportError"; }
};
struct ModuleNotFoundError : ImportError {
    ModuleNotFoundError() : ImportError() {}
    explicit ModuleNotFoundError(const std::string& m) : ImportError(m) {}
    explicit ModuleNotFoundError(const char* m) : ImportError(m) {}
    PY_STDLIB_TEXT_CTOR(ModuleNotFoundError, ImportError)
    const char* type() const override { return "ModuleNotFoundError"; }
};
struct StopIteration : Exception {
    StopIteration() : Exception() {}
    explicit StopIteration(const std::string& m) : Exception(m) {}
    explicit StopIteration(const char* m) : Exception(m) {}
    PY_STDLIB_TEXT_CTOR(StopIteration, Exception)
    const char* type() const override { return "StopIteration"; }
};
// SystemExit(code): code is None (no argument), an int or a str -- whatever sys.exit() was given. what() is
// str(e): "" for no argument, the number's digits, the text. (It derives from Exception, as the ports'
// harnesses catch Exception; Python's is a BaseException.)
struct SystemExit : Exception {
    std::variant<std::monostate, int64_t, str> code;
    SystemExit() : Exception() {}
    explicit SystemExit(int64_t c) : Exception(std::to_string(c)), code(c) {}
    explicit SystemExit(int c) : Exception(std::to_string(c)), code(static_cast<int64_t>(c)) {}
    explicit SystemExit(const str& c) : Exception(c.utf8()), code(c) {}
    explicit SystemExit(const char* c) : Exception(c), code(str(c)) {}
    const char* type() const override { return "SystemExit"; }
    // the process exit status Python's interpreter ends with: None is 0, an int itself, a str is printed
    // to stderr (by the caller) and is 1
    int64_t exit_status() const {
        if (std::holds_alternative<int64_t>(code)) return std::get<int64_t>(code);
        if (std::holds_alternative<str>(code)) return 1;
        return 0;
    }
};

// ------------------------------------------------------------------------------------------ int(s, base)
namespace stdlib_ {
// Py_UNICODE_ISSPACE
inline bool is_space(char32_t c) {
    if (c < 128) return c == ' ' || (c >= 9 && c <= 13) || (c >= 0x1C && c <= 0x1F);
    return c == 0x85 || c == 0xA0 || c == 0x1680 || (c >= 0x2000 && c <= 0x200A) || c == 0x2028 || c == 0x2029 ||
           c == 0x202F || c == 0x205F || c == 0x3000;
}
inline int digit_value(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'z') return c - 'a' + 10;
    if (c >= 'A' && c <= 'Z') return c - 'A' + 10;
    return 99;
}
// the value 0..9 of a decimal digit (each run of ten in the table starts at a zero)
inline int decimal_value(char32_t c) {
    size_t n = sizeof unicode_::decimal_ranges / sizeof unicode_::decimal_ranges[0];
    for (size_t i = 0; i < n; i++)
        if (c >= unicode_::decimal_ranges[i][0] && c <= unicode_::decimal_ranges[i][1])
            return static_cast<int>((c - unicode_::decimal_ranges[i][0]) % 10);
    return -1;
}
}  // namespace stdlib_

// int(s, base) for a str. ValueError ("invalid literal for int() with base N: 'text'", the digit-limit text,
// the base text), OverflowError past int64.
inline int64_t int_parse(const str& s, int64_t base = 10) {
    if ((base != 0 && base < 2) || base > 36) throw ValueError("int() base must be >= 2 and <= 36, or 0");
    // _PyUnicode_TransformDecimalAndSpaceToASCII: white space becomes ' ', decimal digits ASCII, other non-ASCII '?'
    std::string t;
    for (char32_t c : s.u32()) {
        if (c < 128) t += static_cast<char>(c);   // (ASCII as it is: the parser's white space is the blank and TAB LF VT FF CR only)
        else if (stdlib_::is_space(c)) t += ' ';
        else {
            int d = stdlib_::decimal_value(c);
            t += d >= 0 ? static_cast<char>('0' + d) : '?';
        }
    }
    auto bad = [&]() {
        std::u32string r = repr(s).u32();
        if (r.size() > 200) r.resize(200);   // (%.200R)
        throw ValueError("invalid literal for int() with base " + std::to_string(base) + ": " + str(r).utf8());
    };
    const size_t n = t.size();
    size_t i = 0;
    auto ascii_space = [](char c) { return c == ' ' || (c >= 9 && c <= 13); };
    auto at = [&](size_t k) -> char { return k < n ? t[k] : '\0'; };
    while (i < n && ascii_space(t[i])) i++;
    bool neg = false;
    if (at(i) == '+') i++;
    else if (at(i) == '-') { neg = true; i++; }
    int64_t b = base;
    bool error_if_nonzero = false;
    if (b == 0) {
        char x = at(i + 1);
        if (at(i) != '0') b = 10;
        else if (x == 'x' || x == 'X') b = 16;
        else if (x == 'o' || x == 'O') b = 8;
        else if (x == 'b' || x == 'B') b = 2;
        else { error_if_nonzero = true; b = 10; }   // ("old" octal: invalid unless it is all zeros)
    }
    if (at(i) == '0') {
        char x = at(i + 1);
        if ((b == 16 && (x == 'x' || x == 'X')) || (b == 8 && (x == 'o' || x == 'O')) || (b == 2 && (x == 'b' || x == 'B'))) {
            i += 2;
            if (at(i) == '_') i++;   // (one underscore allowed here)
        }
    }
    if (at(i) == '_') bad();
    const size_t start = i;
    int64_t digits = 0;
    char prev = 0;
    while (i < n && (stdlib_::digit_value(t[i]) < b || t[i] == '_')) {
        if (t[i] == '_') {
            if (prev == '_') bad();
        } else {
            digits++;
        }
        prev = t[i];
        i++;
    }
    if (prev == '_') bad();   // (no trailing underscore)
    if (error_if_nonzero) {
        for (size_t k = start; k < i; k++)
            if (t[k] != '0' && t[k] != '_') bad();
    }
    if (i == start) bad();
    uint64_t v = 0;
    bool overflow = false;
    for (size_t k = start; k < i; k++) {
        if (t[k] == '_') continue;
        uint64_t d = static_cast<uint64_t>(stdlib_::digit_value(t[k]));
        uint64_t ub = static_cast<uint64_t>(b);
        if (v > (UINT64_MAX - d) / ub) overflow = true;
        else v = v * ub + d;
    }
    while (i < n && ascii_space(t[i])) i++;
    if (i != n) bad();
    if ((b & (b - 1)) != 0 && digits > 4300)
        throw ValueError("Exceeds the limit (4300 digits) for integer string conversion: value has " + std::to_string(digits) +
                         " digits; use sys.set_int_max_str_digits() to increase the limit");
    if (overflow || (!neg && v > static_cast<uint64_t>(INT64_MAX)) || (neg && v > static_cast<uint64_t>(INT64_MAX) + 1))
        throw OverflowError("int too large for int64");
    return neg ? static_cast<int64_t>(0 - v) : static_cast<int64_t>(v);
}

// ------------------------------------------------------------------------------------------ subprocess
namespace subprocess {
// subprocess.list2cmdline(seq): the command line Windows is given for an argument list
inline str list2cmdline(const list<str>& seq) {
    std::u32string result;
    bool first = true;
    for (const str& arg : seq) {
        const std::u32string& a = arg.u32();
        std::u32string bs_buf;
        if (!first) result.push_back(U' ');
        first = false;
        bool needquote = a.find(U' ') != std::u32string::npos || a.find(U'\t') != std::u32string::npos || a.empty();
        if (needquote) result.push_back(U'"');
        for (char32_t c : a) {
            if (c == U'\\') {
                bs_buf.push_back(c);   // (don't know if it will be a literal or an escape)
            } else if (c == U'"') {
                result.append(bs_buf.size() * 2, U'\\');   // (double the backslashes before a quote)
                bs_buf.clear();
                result += U"\\\"";
            } else {
                result += bs_buf;
                bs_buf.clear();
                result.push_back(c);
            }
        }
        result += bs_buf;   // (trailing backslashes: kept)
        if (needquote) {
            result += bs_buf;   // (and doubled before the closing quote)
            result.push_back(U'"');
        }
    }
    return str(result);
}
}  // namespace subprocess

// ------------------------------------------------------------------------------------------ tempfile
namespace tempfile {
namespace detail_ {
inline std::string random_name() {
    static std::mt19937_64 rng{std::random_device{}()};
    static const char chars[] = "abcdefghijklmnopqrstuvwxyz0123456789_";
    std::string s;
    for (int k = 0; k < 8; k++) s.push_back(chars[rng() % (sizeof chars - 1)]);
    return s;
}
// the OSError of a failed system call on `path`, as CPython raises it
[[noreturn]] inline void raise_(const std::error_code& ec, const str& path) {
    std::string text = ec.message();
    while (!text.empty() && (static_cast<unsigned char>(text.back()) <= ' ' || text.back() == '.')) text.pop_back();
    std::string msg = "[WinError " + std::to_string(ec.value()) + "] " + text + ": " + repr(path).utf8();
    switch (ec.value()) {
        case 2: case 3: case 15: case 18: case 53: case 67: case 161: case 206: throw FileNotFoundError(msg);
        case 5: case 16: case 19: case 32: case 33: case 65: case 82: throw PermissionError(msg);
        case 80: case 183: throw FileExistsError(msg);
        case 267: throw NotADirectoryError(msg);
        default: throw OSError(msg);
    }
}
inline int64_t& tempdir_set_() {   // 0 = not looked for yet
    static int64_t v = 0;
    return v;
}
inline str& tempdir_() {
    static str v;
    return v;
}
}  // namespace detail_

// forget the folder gettempdir settled on (the tests change the environment; CPython: tempfile.tempdir = None)
inline void gettempdir_reset() { detail_::tempdir_set_() = 0; }

// tempfile.gettempdir(): the first of TMPDIR / TEMP / TMP, the user's AppData\Local\Temp, %SYSTEMROOT%\Temp,
// c:\temp, c:\tmp, \temp, \tmp and the current folder in which a file can be made
inline str gettempdir() {
    if (detail_::tempdir_set_()) return detail_::tempdir_();
    list<str> cands;
    for (const char* k : {"TMPDIR", "TEMP", "TMP"}) {
        auto v = os::environ_::get(str(k));
        if (v && !v->empty()) cands.append(*v);
    }
    cands.append(os::path::expanduser(str("~\\AppData\\Local\\Temp")));
    {
        auto sr = os::environ_::get(str("SYSTEMROOT"));
        cands.append(sr ? *sr + str("\\Temp") : str("%SYSTEMROOT%\\Temp"));
    }
    for (const char* c : {"c:\\temp", "c:\\tmp", "\\temp", "\\tmp"}) cands.append(str(c));
    cands.append(os::getcwd());
    for (str dir : cands) {
        if (dir != str(".")) dir = os::path::abspath(dir);
        for (int seq = 0; seq < 100; seq++) {
            str file = os::path::join(dir, str("tmp") + str(detail_::random_name()));
            std::FILE* f = nullptr;
            errno = 0;
            f = std::fopen(file.utf8().c_str(), "wx");
            if (f) {
                std::fclose(f);
                std::error_code ec;
                std::filesystem::remove(fspath_(file), ec);
                detail_::tempdir_() = dir;
                detail_::tempdir_set_() = 1;
                return dir;
            }
            if (errno == EEXIST) continue;
            if (errno == EACCES && os::path::isdir(dir)) continue;   // (CPython: a write-protected name, try another)
            break;
        }
    }
    std::string shown;
    for (const str& c : cands) shown += (shown.empty() ? "[" : ", ") + repr(c).utf8();
    throw FileNotFoundError("No usable temporary directory found in " + shown + "]");
}

// tempfile.mkdtemp(suffix=None, prefix=None, dir=None): a new folder <dir>\<prefix><8 random chars><suffix>
inline str mkdtemp(const std::optional<str>& suffix = std::nullopt, const std::optional<str>& prefix = std::nullopt,
                   const std::optional<str>& dir = std::nullopt) {
    str d = dir ? *dir : gettempdir();
    str pre = prefix ? *prefix : str("tmp");
    str suf = suffix ? *suffix : str("");
    for (int seq = 0; seq < 10000; seq++) {
        str file = os::path::join(d, pre + str(detail_::random_name()) + suf);
        std::error_code ec;
        bool made = std::filesystem::create_directory(fspath_(file), ec);
        if (!ec && made) return os::path::abspath(file);
        if (!ec && !made) continue;   // (it was there)
        if (ec.value() == 183 || ec.value() == 80) continue;   // FileExistsError
        if (ec.value() == 5 && os::path::isdir(d)) continue;    // PermissionError on a folder that can be written to
        detail_::raise_(ec, file);
    }
    throw FileExistsError("No usable temporary directory name found");
}

// tempfile.TemporaryDirectory(suffix=None, prefix=None, dir=None): a folder made as mkdtemp does and removed,
// with what is in it, when this goes (or at cleanup())
struct TemporaryDirectory {
    str name;
    explicit TemporaryDirectory(const std::optional<str>& suffix = std::nullopt, const std::optional<str>& prefix = std::nullopt,
                                const std::optional<str>& dir = std::nullopt)
        : name(mkdtemp(suffix, prefix, dir)) {}
    ~TemporaryDirectory() { cleanup(); }
    void cleanup() {
        if (name.empty()) return;
        std::error_code ec;
        std::filesystem::remove_all(fspath_(name), ec);
        name = str("");
    }
    TemporaryDirectory(const TemporaryDirectory&) = delete;
    TemporaryDirectory& operator=(const TemporaryDirectory&) = delete;
};
}  // namespace tempfile

// ------------------------------------------------------------------------------------------ codecs
namespace codecs {
namespace detail_ {
inline std::string char_repr(char32_t c) {
    char b[16];
    if (c < 0x100) std::snprintf(b, sizeof b, "\\x%02x", static_cast<unsigned>(c));
    else if (c < 0x10000) std::snprintf(b, sizeof b, "\\u%04x", static_cast<unsigned>(c));
    else std::snprintf(b, sizeof b, "\\U%08x", static_cast<unsigned>(c));
    return b;
}
}  // namespace detail_

// bytes.decode("utf-16-be", errors): errors is strict, replace or ignore. No byte order mark is looked for.
inline str utf_16_be_decode(const bytes& data, const char* errors = "strict") {
    const std::string& b = data.raw();
    const std::string err(errors);
    std::u32string out;
    size_t n = b.size(), q = 0;
    auto unit = [&](size_t k) { return static_cast<char32_t>((static_cast<unsigned char>(b[k]) << 8) | static_cast<unsigned char>(b[k + 1])); };
    auto fail = [&](size_t start, size_t end, const char* why) {   // (the bytes [start, end) are bad)
        if (err == "strict") {
            std::string m;
            if (end == start + 1) {
                char x[8];
                std::snprintf(x, sizeof x, "0x%02x", static_cast<unsigned>(static_cast<unsigned char>(b[start])));
                m = std::string("'utf-16-be' codec can't decode byte ") + x + " in position " + std::to_string(start) + ": " + why;
            } else {
                m = "'utf-16-be' codec can't decode bytes in position " + std::to_string(start) + "-" + std::to_string(end - 1) + ": " + why;
            }
            throw UnicodeDecodeError(m);
        }
        if (err == "replace") out.push_back(0xFFFD);
        else if (err != "ignore") throw LookupError("unknown error handler name '" + err + "'");
    };
    while (q < n) {
        if (n - q < 2) {
            fail(q, n, "truncated data");
            break;
        }
        char32_t ch = unit(q);
        q += 2;
        if (ch < 0xD800 || ch > 0xDFFF) {
            out.push_back(ch);
            continue;
        }
        if (ch < 0xDC00) {   // a high surrogate: a low one must follow
            if (n - q < 2) {
                fail(q - 2, n, "unexpected end of data");
                q = n;
                continue;
            }
            char32_t ch2 = unit(q);
            if (ch2 >= 0xDC00 && ch2 <= 0xDFFF) {
                out.push_back(0x10000 + ((ch - 0xD800) << 10) + (ch2 - 0xDC00));
                q += 2;
                continue;
            }
            fail(q - 2, q, "illegal UTF-16 surrogate");
            continue;
        }
        fail(q - 2, q, "illegal encoding");   // a low surrogate on its own
    }
    return str(out);
}

// str.encode("utf-16-be", errors): a lone surrogate is an error (strict), '?' (replace) or dropped (ignore)
inline bytes utf_16_be_encode(const str& s, const char* errors = "strict") {
    const std::u32string& u = s.u32();
    const std::string err(errors);
    std::string out;
    auto put = [&](unsigned v) {
        out.push_back(static_cast<char>(v >> 8));
        out.push_back(static_cast<char>(v & 0xFF));
    };
    for (size_t i = 0; i < u.size();) {
        char32_t c = u[i];
        if (c >= 0xD800 && c <= 0xDFFF) {
            size_t j = i;
            while (j < u.size() && u[j] >= 0xD800 && u[j] <= 0xDFFF) j++;
            if (err == "strict") {
                // (CPython reports the first surrogate alone, not the run it is in)
                throw UnicodeEncodeError("'utf-16-be' codec can't encode character '" + detail_::char_repr(c) + "' in position " + std::to_string(i) +
                                         ": surrogates not allowed");
            }
            if (err == "replace") for (size_t k = i; k < j; k++) put('?');
            else if (err != "ignore") throw LookupError("unknown error handler name '" + err + "'");
            i = j;
            continue;
        }
        if (c >= 0x10000) {
            unsigned v = static_cast<unsigned>(c) - 0x10000;
            put(0xD800 + (v >> 10));
            put(0xDC00 + (v & 0x3FF));
        } else {
            put(static_cast<unsigned>(c));
        }
        i++;
    }
    return bytes(out);
}
}  // namespace codecs


// ------------------------------------------------------------------------------------------ native errors
// raise_named("ValueError", message): the Python exception a native reader's failure stands for (its name as Python prints
// it: "struct.error" for struct.error). An unknown name is RuntimeError.
[[noreturn]] inline void raise_named(const std::string& kind, const std::string& m) {
    if (kind == "struct.error") throw StructError(m);
    if (kind == "ValueError") throw ValueError(m);
    if (kind == "IndexError") throw IndexError(m);
    if (kind == "KeyError") throw KeyError(m);
    if (kind == "TypeError") throw TypeError(m);
    if (kind == "OverflowError") throw OverflowError(m);
    if (kind == "ZeroDivisionError") throw ZeroDivisionError(m);
    if (kind == "MemoryError") throw MemoryError(m);
    if (kind == "OSError") throw OSError(m);
    if (kind == "FileNotFoundError") throw FileNotFoundError(m);
    if (kind == "FileExistsError") throw FileExistsError(m);
    if (kind == "PermissionError") throw PermissionError(m);
    if (kind == "EOFError") throw EOFError(m);
    if (kind == "StopIteration") throw StopIteration(m);
    if (kind == "UnicodeDecodeError") throw UnicodeDecodeError(m);
    if (kind == "UnicodeEncodeError") throw UnicodeEncodeError(m);
    if (kind == "AttributeError") throw AttributeError(m);
    if (kind == "AssertionError") throw AssertionError(m);
    if (kind == "NotImplementedError") throw NotImplementedError(m);
    throw RuntimeError(m);
}
// translate_errors<E>(f): f(), where an E (a native reader's error with .kind and .message: cbf::PyError) leaves as the
// Python exception of that name, and a failed allocation as MemoryError
template <class E, class F> auto translate_errors(F&& f) -> decltype(f()) {
    try {
        return memory_guard(f);
    } catch (const E& e) {
        raise_named(e.kind, e.message);
    }
}

// ------------------------------------------------------------------------------------------ contextlib
namespace contextlib {
namespace detail_ {
inline std::recursive_mutex& capture_mutex_() {
    static std::recursive_mutex m;
    return m;
}
inline std::mutex& silence_mutex_() {
    static std::mutex m;
    return m;
}
inline int& silence_depth_() {
    static int n = 0;
    return n;
}
inline int& silence_saved_() {
    static int fd = -1;
    return fd;
}
}  // namespace detail_
// with contextlib.redirect_stdout(io.StringIO()) as buf: ... buf.getvalue()
// File descriptor 1 goes to a temporary file for the scope, so everything written to standard output (print, std::cout, C stdio,
// a child process) is caught. getvalue() ends the capture and gives the text (UTF-8, errors replaced); the destructor ends it
// when it has not been asked for. Captures exclude one another (a recursive lock held for the scope).
class redirect_stdout {
  public:
    redirect_stdout() : lock_(detail_::capture_mutex_()) {
        std::fflush(stdout);
        std::cout.flush();
#ifdef _WIN32
        static int serial = 0;
        path_ = (std::filesystem::temp_directory_path() / ("cbc" + std::to_string(_getpid()) + "_" + std::to_string(serial++) + ".tmp")).string();
        const int fd = _open(path_.c_str(), _O_RDWR | _O_BINARY | _O_CREAT | _O_TRUNC, 0600);
        saved_ = _dup(1);
        _dup2(fd, 1);
        _close(fd);
#endif
    }
    redirect_stdout(const redirect_stdout&) = delete;
    redirect_stdout& operator=(const redirect_stdout&) = delete;
    ~redirect_stdout() {
        if (!done_) getvalue();
    }
    str getvalue() {
        if (!done_) {
#ifdef _WIN32
            std::fflush(stdout);
            std::cout.flush();
            _dup2(saved_, 1);
            _close(saved_);
            std::ifstream f(path_, std::ios::binary);
            std::stringstream ss;
            ss << f.rdbuf();
            std::string raw = ss.str();
            for (size_t n = 0; (n = raw.find("\r\n", n)) != std::string::npos;) raw.erase(n, 1);   // (text mode wrote \n as \r\n)
            text_ = raw;
            f.close();
            std::remove(path_.c_str());
#endif
            done_ = true;
        }
        return bytes(text_).decode("utf-8", "replace");
    }

  private:
    std::lock_guard<std::recursive_mutex> lock_;
    std::string path_, text_;
    int saved_ = -1;
    bool done_ = false;
};
// with contextlib.redirect_stdout(io.StringIO()): ...   (never read back): standard output goes to NUL for the scope; scopes
// may nest or overlap across threads (the first in redirects, the last out restores)
class redirect_stdout_null {
  public:
    redirect_stdout_null() {
        std::lock_guard<std::mutex> g(detail_::silence_mutex_());
        if (detail_::silence_depth_()++ == 0) {
            std::fflush(stdout);
            std::cout.flush();
#ifdef _WIN32
            detail_::silence_saved_() = _dup(1);
            const int nul = _open("NUL", _O_WRONLY | _O_BINARY);
            _dup2(nul, 1);
            _close(nul);
#endif
        }
    }
    redirect_stdout_null(const redirect_stdout_null&) = delete;
    redirect_stdout_null& operator=(const redirect_stdout_null&) = delete;
    ~redirect_stdout_null() {
        std::lock_guard<std::mutex> g(detail_::silence_mutex_());
        if (--detail_::silence_depth_() == 0) {
            std::fflush(stdout);
            std::cout.flush();
#ifdef _WIN32
            _dup2(detail_::silence_saved_(), 1);
            _close(detail_::silence_saved_());
#endif
        }
    }
};
}  // namespace contextlib

}  // namespace py
#undef PY_STDLIB_TEXT_CTOR
