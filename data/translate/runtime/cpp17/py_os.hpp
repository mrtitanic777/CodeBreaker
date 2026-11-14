// py_os.hpp -- the porting engine's library rows over py.hpp (included at its end): open() in every mode
// and the file object, os / os.path / shutil / time, hashlib, re (over native/l2's pyre, when its header
// is on the include path), tagged variants. Each behaves as CPython 3 on Windows does, error for error.
#pragma once
#include <array>
#include <chrono>
#include <cstdlib>
#include <sys/stat.h>
#include <set>
#include <thread>
#include <variant>
#if defined(_M_X64) || defined(__x86_64__)
#include <immintrin.h>
#if defined(_MSC_VER) && !defined(__clang__)
#include <intrin.h>
#else
#include <cpuid.h>
#endif
#endif
#ifdef _WIN32
#include <direct.h>
#include <io.h>
#include <sys/utime.h>
#endif

namespace py {

#define PY_OS_ERROR(Name)                                                \
    struct Name : OSError {                                             \
        using OSError::OSError;                                         \
        const char* type() const override { return #Name; }             \
    };
PY_OS_ERROR(FileExistsError)
PY_OS_ERROR(IsADirectoryError)
PY_OS_ERROR(NotADirectoryError)
#undef PY_OS_ERROR

namespace fs_ = std::filesystem;
inline fs_::path fspath_(const str& p) { return fs_::u8path(p.utf8()); }
inline str pystr_(const fs_::path& p) { return str(p.u8string()); }

// ------------------------------------------------------------------------------------------ files
// open(path, mode, encoding=, errors=, newline=): a handle shared by every name holding it (closed when
// the last one goes, or by close()). Text mode decodes (default cp1252, Windows' locale codec) and
// translates newlines as Python's universal newlines; writing translates "\n" to "\r\n" unless newline=.
// a std::string of n bytes whose contents the caller fills in at once (a read): not zeroed first where the library can leave it
inline void resize_uninit_(std::string& s, size_t n) {
#if defined(_MSVC_STL_VERSION)
    s._Resize_and_overwrite(n, [](char*, size_t m) { return m; });
#elif defined(__cpp_lib_string_resize_and_overwrite)
    s.resize_and_overwrite(n, [](char*, size_t m) { return m; });
#else
    s.resize(n);
#endif
}
class file {
    struct H {
        std::FILE* f = nullptr;
        bool text = false, readable = false, writable = false;
        std::string enc = "cp1252", errors = "strict";
        bool has_newline = false;
        std::string newline;
        std::u32string buf;   // text read ahead (decoded, newlines translated)
        size_t bpos = 0;
        bool buffered = false;
        ~H() {
            if (f) std::fclose(f);
        }
    };
    std::shared_ptr<H> h_;
    H& h() const {
        if (!h_ || !h_->f) throw ValueError("I/O operation on closed file.");
        return *h_;
    }
    std::string rest_() const {
        std::string s;
        std::FILE* f = h().f;
        // (a regular file: its remaining size is known, so the string is made once and read into directly)
        int64_t here = _ftelli64(f);
        if (here >= 0 && _fseeki64(f, 0, SEEK_END) == 0) {
            int64_t end = _ftelli64(f);
            _fseeki64(f, here, SEEK_SET);
            if (end > here) resize_uninit_(s, static_cast<size_t>(end - here));
            size_t got = s.empty() ? 0 : std::fread(&s[0], 1, s.size(), f);
            s.resize(got);
        }
        char tmp[65536];
        size_t n;
        while ((n = std::fread(tmp, 1, sizeof tmp, f)) > 0) s.append(tmp, n);
        return s;
    }
    void fill_() const {   // (text) the rest of the file, decoded
        H& x = h();
        if (x.buffered) return;
        str t = bytes(rest_()).decode(x.enc.c_str(), x.errors.c_str());
        const std::u32string& u = t.u32();
        if (!x.has_newline) {
            x.buf.reserve(u.size());
            for (size_t i = 0; i < u.size(); i++) {
                if (u[i] == '\r') {
                    x.buf.push_back('\n');
                    if (i + 1 < u.size() && u[i + 1] == '\n') i++;
                } else {
                    x.buf.push_back(u[i]);
                }
            }
        } else {
            x.buf = u;
        }
        x.bpos = 0;
        x.buffered = true;
    }
    void need_(bool rd) const {
        H& x = h();
        if (rd ? !x.readable : !x.writable) throw OSError(rd ? "not readable" : "not writable");   // io.UnsupportedOperation
    }

  public:
    file() = default;
    static file open(const str& path, const str& mode0 = str("r"), const std::optional<str>& encoding = std::nullopt,
                     const std::optional<str>& errors = std::nullopt, const std::optional<str>& newline = std::nullopt) {
        std::string mode = mode0.utf8();
        auto x = std::make_shared<H>();
        char kind = 0;
        bool plus = false, bin = false;
        for (char c : mode) {
            if (c == 'r' || c == 'w' || c == 'a' || c == 'x') {
                if (kind) throw ValueError("must have exactly one of create/read/write/append mode");
                kind = c;
            } else if (c == '+') plus = true;
            else if (c == 'b') bin = true;
            else if (c == 't') {
            } else throw ValueError("invalid mode: '" + mode + "'");
        }
        if (!kind) throw ValueError("Must have exactly one of create/read/write/append mode and at most one plus");
        if (bin && encoding) throw ValueError("binary mode doesn't take an encoding argument");
        x->text = !bin;
        x->readable = kind == 'r' || plus;
        x->writable = kind != 'r' || plus;
        if (encoding) {
            x->enc = py::str(encoding->lower()).utf8();
            if (x->enc == "utf_8" || x->enc == "utf8") x->enc = "utf-8";
            if (x->enc == "utf-8-sig") x->enc = "utf-8";
        }
        if (errors) x->errors = errors->utf8();
        if (newline) {
            x->has_newline = true;
            x->newline = newline->utf8();
        }
        for (char32_t c : path.u32())
            if (c == 0) throw ValueError("embedded null character");
        if (path.empty()) throw FileNotFoundError("[Errno 2] No such file or directory: ''");   // (the C runtime says EINVAL)
        fs_::path p = fspath_(path);
        std::error_code ec;
        bool exists = fs_::exists(p, ec);
        if (fs_::is_directory(p, ec)) throw PermissionError("[Errno 13] Permission denied: " + repr(path).utf8());
        if (kind == 'x' && exists) throw FileExistsError("[Errno 17] File exists: " + repr(path).utf8());
        // (otherwise the system decides, as for Python: a name it will not take is EINVAL, OSError; a missing
        // file or folder ENOENT, FileNotFoundError -- "^\s*x" is the latter, its "folder" ^ being absent)
        std::wstring m = kind == 'r' ? (plus ? L"r+b" : L"rb") : kind == 'w' || kind == 'x' ? (plus ? L"w+b" : L"wb") : (plus ? L"a+b" : L"ab");
#ifdef _WIN32
        m += L"N";   // (PEP 446: not inherited by child processes, so a child never holds the file open)
        x->f = _wfopen(p.wstring().c_str(), m.c_str());
#else
        x->f = std::fopen(p.string().c_str(), std::string(m.begin(), m.end()).c_str());
#endif
        if (!x->f) {
            int e = errno;
            if (e == ENOENT) throw FileNotFoundError("[Errno 2] No such file or directory: " + repr(path).utf8());
            if (e == EACCES) throw PermissionError("[Errno 13] Permission denied: " + repr(path).utf8());
            if (e == EEXIST) throw FileExistsError("[Errno 17] File exists: " + repr(path).utf8());
            throw OSError("[Errno " + std::to_string(e) + "] " + std::strerror(e) + ": " + repr(path).utf8());
        }
        file out;
        out.h_ = x;
        return out;
    }
    // binary reads
    bytes read(int64_t n = -1) const {
        need_(true);
        if (n < 0) return bytes(rest_());
        std::string s;
        resize_uninit_(s, static_cast<size_t>(n));
        size_t got = n ? std::fread(&s[0], 1, s.size(), h().f) : 0;
        s.resize(got);
        return bytes(std::move(s));
    }
    bytes readline() const {
        need_(true);
        std::string s;
        int c;
        while ((c = std::fgetc(h().f)) != EOF) {
            s.push_back(static_cast<char>(c));
            if (c == '\n') break;
        }
        return bytes(std::move(s));
    }
    list<bytes> readlines() const {
        list<bytes> out;
        for (bytes b = readline(); !b.raw().empty(); b = readline()) out.append(b);
        return out;
    }
    // text reads
    str read_text(int64_t n = -1) const {
        need_(true);
        fill_();
        H& x = h();
        size_t take = n < 0 ? x.buf.size() - x.bpos : std::min<size_t>(static_cast<size_t>(n), x.buf.size() - x.bpos);
        str out(x.buf.substr(x.bpos, take));
        x.bpos += take;
        return out;
    }
    str readline_text() const {
        need_(true);
        fill_();
        H& x = h();
        size_t e = x.buf.find(U'\n', x.bpos);
        size_t take = e == std::u32string::npos ? x.buf.size() - x.bpos : e + 1 - x.bpos;
        str out(x.buf.substr(x.bpos, take));
        x.bpos += take;
        return out;
    }
    list<str> readlines_text() const {
        list<str> out;
        for (str s = readline_text(); !s.u32().empty(); s = readline_text()) out.append(s);
        return out;
    }
    // for line in f (text): its lines, read when the loop starts
    std::vector<str>::const_iterator begin() const {
        lines_ = std::make_shared<std::vector<str>>(readlines_text().vec());
        return lines_->cbegin();
    }
    std::vector<str>::const_iterator end() const { return lines_->cend(); }
    mutable std::shared_ptr<std::vector<str>> lines_;
    int64_t write(const bytes& b) const {
        need_(false);
        if (h().text) throw TypeError("write() argument must be str, not bytes");
        std::fwrite(b.raw().data(), 1, b.raw().size(), h().f);
        return static_cast<int64_t>(b.raw().size());
    }
    int64_t write(const bytearray& b) const { return write(bytes(b.view())); }
    int64_t write(const str& s) const {
        need_(false);
        H& x = h();
        if (!x.text) throw TypeError("a bytes-like object is required, not 'str'");
        std::u32string u;
        const std::u32string& in = s.u32();
        std::string nl = x.has_newline ? x.newline : "\r\n";
        if (nl.empty() || nl == "\n") u = in;
        else
            for (char32_t c : in) {
                if (c == '\n')
                    for (char k : nl) u.push_back(static_cast<char32_t>(k));
                else u.push_back(c);
            }
        bytes b = str(u).encode(x.enc.c_str(), x.errors.c_str());
        std::fwrite(b.raw().data(), 1, b.raw().size(), x.f);
        return static_cast<int64_t>(in.size());
    }
    template <class T> void writelines(const list<T>& xs) const {
        for (const auto& x : xs) write(x);
    }
    int64_t seek(int64_t off, int64_t whence = 0) const {
        H& x = h();
        if (x.text && (whence != 0 && off != 0)) throw OSError("can't do nonzero cur-relative seeks");
        x.buffered = false;
        x.buf.clear();
        x.bpos = 0;
        if (whence == 0 && off < 0) throw ValueError("negative seek position " + std::to_string(off));
        _fseeki64(x.f, off, whence == 0 ? SEEK_SET : whence == 1 ? SEEK_CUR : SEEK_END);
        return static_cast<int64_t>(_ftelli64(x.f));
    }
    int64_t tell() const { return static_cast<int64_t>(_ftelli64(h().f)); }
    void flush() const { std::fflush(h().f); }
    void close() const {
        if (h_ && h_->f) {
            std::fclose(h_->f);
            h_->f = nullptr;
        }
    }
    bool closed() const { return !h_ || !h_->f; }
};
inline file open(const str& path, const str& mode = str("r"), const std::optional<str>& encoding = std::nullopt,
                 const std::optional<str>& errors = std::nullopt, const std::optional<str>& newline = std::nullopt) {
    return file::open(path, mode, encoding, errors, newline);
}
inline file open_rb(const str& path) { return file::open(path, str("rb")); }

// ------------------------------------------------------------------------------------------ os
#ifdef _WIN32
#ifndef _WINDOWS_
extern "C" __declspec(dllimport) int __stdcall SetEnvironmentVariableW(const wchar_t* lpName, const wchar_t* lpValue);   // (what <windows.h> declares)
#endif
#endif
namespace os {
// (CPython's mapping of the system's error codes to its OSError subclasses)
[[noreturn]] inline void raise_os_(const std::error_code& ec, const str& path) {
    // (OSError's text drops the system message's trailing spaces, line ends and period)
    std::string sys = ec.message();
    while (!sys.empty() && (static_cast<unsigned char>(sys.back()) <= ' ' || sys.back() == '.')) sys.pop_back();
    std::string msg = "[WinError " + std::to_string(ec.value()) + "] " + sys + ": " + repr(path).utf8();
    switch (ec.value()) {
        case 2: case 3: case 15: case 53: case 67: case 161: throw FileNotFoundError(msg);   // (file, path, drive, network names)
        case 5: case 32: case 33: throw PermissionError(msg);
        case 80: case 183: throw FileExistsError(msg);
        case 267: throw NotADirectoryError(msg);
        default: throw OSError(msg);   // (123, an invalid name, among them)
    }
}
inline str fspath(const str& p) { return p; }   // (a path that is a str: itself)
inline const str sep = str("\\");
inline const str altsep = str("/");
inline const str linesep = str("\r\n");
inline const str pathsep = str(";");
inline const str curdir = str(".");
inline const str pardir = str("..");
inline str getcwd() { return pystr_(fs_::current_path()); }
inline void chdir(const str& p) {
    std::error_code ec;
    fs_::current_path(fspath_(p), ec);
    if (ec) throw FileNotFoundError("[WinError 2] The system cannot find the file specified: " + repr(p).utf8());
}
namespace environ_ {   // os.environ ("environ" is a macro of the C runtime)
#ifdef _WIN32
// variables set to an empty value (the C runtime cannot hold one: it reads them as not set)
inline std::set<std::u32string>& empty_vars_() {
    static std::set<std::u32string> s;
    return s;
}
#endif
inline std::optional<str> get(const str& k) {
#ifdef _WIN32
    std::wstring w = fs_::u8path(k.utf8()).wstring();
    const wchar_t* v = _wgetenv(w.c_str());
    if (!v) return empty_vars_().count(str(k).upper().u32()) ? std::optional<str>(str("")) : std::nullopt;
    return pystr_(fs_::path(v));
#else
    const char* v = std::getenv(k.utf8().c_str());
    if (!v) return std::nullopt;
    return str(v);
#endif
}
inline str get(const str& k, const str& dflt) {
    auto v = get(k);
    return v ? *v : dflt;
}
inline str at(const str& k) {
    auto v = get(k);
    if (!v) throw KeyError(repr(k).utf8());
    return *v;
}
// dict(os.environ): the process environment in the order the system gives it. On Windows os.environ has its names
// upper-cased (a later entry of the same name replaces the earlier in place), and a name may begin with '=' ("=C:").
inline dict<str, str> copy() {
    dict<str, str> out;
#ifdef _WIN32
    (void)_wgetenv(L"PATH");   // (_wenviron exists once the wide environment has been asked)
    for (wchar_t** e = _wenviron; e && *e; ++e) {
        std::wstring entry(*e);
        size_t eq = entry.find(L'=', 1);
        if (entry.empty() || entry[0] == L'=' || eq == std::wstring::npos) continue;   // (CPython leaves the hidden "=C:" entries out)
        out.set(pystr_(fs_::path(entry.substr(0, eq))).upper(), pystr_(fs_::path(entry.substr(eq + 1))));
    }
    for (const std::u32string& n : empty_vars_())
        if (!out.contains(str(n))) out.set(str(n), str(""));
#else
    for (char** e = ::environ; e && *e; ++e) {
        std::string entry(*e);
        size_t eq = entry.find('=', 1);
        if (entry.empty() || eq == std::string::npos) continue;
        out.set(str(entry.substr(0, eq)), str(entry.substr(eq + 1)));
    }
#endif
    return out;
}
// list(os.environ.items())
inline list<std::tuple<str, str>> items() {
    list<std::tuple<str, str>> out;
    for (const auto& kv : copy().items()) out.append(std::make_tuple(std::get<0>(kv), std::get<1>(kv)));
    return out;
}
// os.environ[k] = v (os.putenv): ValueError for a name that is empty or holds an '=' past its first character
inline void set(const str& k, const str& v) {
    if (k.empty() || k.find(str("="), 1) != -1) throw ValueError("illegal environment variable name");
    if (k.find(str(std::string(1, '\0'))) != -1 || v.find(str(std::string(1, '\0'))) != -1) throw ValueError("embedded null character");
#ifdef _WIN32
    if (k.u32()[0] == U'=') throw OSError("[Errno 22] Invalid argument");   // (the C runtime refuses a name that begins with '=')
    std::wstring wk = fs_::u8path(k.utf8()).wstring(), wv = fs_::u8path(v.utf8()).wstring();
    if (wv.empty()) {   // (_wputenv_s of "" removes the variable; an empty value is set through the system)
        empty_vars_().insert(str(k).upper().u32());
        SetEnvironmentVariableW(wk.c_str(), L"");
    } else {
        empty_vars_().erase(str(k).upper().u32());
        _wputenv_s(wk.c_str(), wv.c_str());
    }
#else
    setenv(k.utf8().c_str(), v.utf8().c_str(), 1);
#endif
}
// os.environ.pop(k, None): the old value, if there was one
inline std::optional<str> pop(const str& k) {
    std::optional<str> old = get(k);
    if (!old) return std::nullopt;
#ifdef _WIN32
    empty_vars_().erase(str(k).upper().u32());
    _wputenv_s(fs_::u8path(k.utf8()).wstring().c_str(), L"");   // (an empty value removes the variable)
    SetEnvironmentVariableW(fs_::u8path(k.utf8()).wstring().c_str(), nullptr);
#else
    unsetenv(k.utf8().c_str());
#endif
    return old;
}
}  // namespace environ_
inline std::optional<str> getenv(const str& k) { return environ_::get(k); }
inline str getenv(const str& k, const str& dflt) { return environ_::get(k, dflt); }

// os.stat(path): the fields of os.stat_result the ports read. mtime is exact (the file time's 100 ns); atime and ctime
// (creation time on Windows) are the C runtime's whole seconds.
struct stat_result {
    int64_t st_mode = 0, st_size = 0, st_nlink = 1;
    double st_atime = 0, st_mtime = 0, st_ctime = 0;
    int64_t st_atime_ns = 0, st_mtime_ns = 0, st_ctime_ns = 0;   // (st_ctime: the creation time on Windows)
};
inline stat_result stat(const str& p) {
    nul_check_("stat", p);
    std::error_code ec;
    fs_::path q = fspath_(p);
    if (p.empty()) throw FileNotFoundError("[WinError 3] The system cannot find the path specified: ''");
    fs_::file_status st = fs_::status(q, ec);
    if (ec || st.type() == fs_::file_type::not_found) raise_os_(ec ? ec : std::error_code(2, std::system_category()), p);
    stat_result r;
    bool dir = st.type() == fs_::file_type::directory;
    bool writable = (st.permissions() & fs_::perms::owner_write) != fs_::perms::none;
    if (dir) {
        r.st_mode = 040000 | (writable ? 0777 : 0555);
    } else {
        r.st_mode = 0100000 | (writable ? 0666 : 0444);
        std::string ext = std::get<1>(path::splitext(p)).lower().utf8();
        if (ext == ".exe" || ext == ".bat" || ext == ".cmd" || ext == ".com") r.st_mode |= 0111;
        auto n = fs_::file_size(q, ec);
        if (!ec) r.st_size = static_cast<int64_t>(n);
    }
    auto nl = fs_::hard_link_count(q, ec);
    r.st_nlink = ec ? 1 : static_cast<int64_t>(nl);
    auto t = fs_::last_write_time(q, ec);
    if (!ec) {
        int64_t ticks = std::chrono::duration_cast<std::chrono::duration<int64_t, std::ratio<1, 10000000>>>(t.time_since_epoch()).count();   // 100 ns since 1601
        int64_t sec = ticks / 10000000 - 11644473600LL, nsec = (ticks % 10000000) * 100;
        r.st_mtime = static_cast<double>(sec) + static_cast<double>(nsec) * 1e-9;
        r.st_mtime_ns = sec * 1000000000LL + nsec;
    }
#ifdef _WIN32
    struct _stat64 cs;
    if (_wstat64(q.c_str(), &cs) == 0) {
        r.st_atime = static_cast<double>(cs.st_atime);
        r.st_ctime = static_cast<double>(cs.st_ctime);
        r.st_atime_ns = static_cast<int64_t>(cs.st_atime) * 1000000000LL;
        r.st_ctime_ns = static_cast<int64_t>(cs.st_ctime) * 1000000000LL;
    }
#endif
    return r;
}

// os.utime(path, ns=(atime_ns, mtime_ns)) / os.utime(path, (atime, mtime)) / os.utime(path) (now), as CPython does on Windows
inline void utime_ns(const str& p, int64_t atime_ns, int64_t mtime_ns) {
    nul_check_("utime", p);
    // (the C runtime sets both times to whole seconds, then the file time sets the modification time exactly, to 100 ns; the access
    // time stays at its whole second: <windows.h> is not included here, and the C runtime cannot set the finer one)
    int64_t asec = atime_ns / 1000000000LL;
    if (atime_ns % 1000000000LL < 0) asec -= 1;
    int64_t msec = mtime_ns / 1000000000LL;
    if (mtime_ns % 1000000000LL < 0) msec -= 1;
    std::error_code ec;
    {
#ifdef _WIN32
        struct __utimbuf64 ub;
        ub.actime = asec;
        ub.modtime = msec;
        const std::wstring wp = fspath_(p).wstring();
        std::error_code dec;
        if (!fs_::is_directory(fspath_(p), dec) && _wutime64(wp.c_str(), &ub) != 0) {
            if (errno == ENOENT) raise_os_(std::error_code(2, std::system_category()), p);
            // (the C runtime cannot open a read-only file to set its times, the system call CPython makes can: lift the attribute for the call)
            struct _stat64 cs;
            if (_wstat64(wp.c_str(), &cs) != 0 || (cs.st_mode & _S_IWRITE)) raise_os_(std::error_code(5, std::system_category()), p);
            _wchmod(wp.c_str(), _S_IREAD | _S_IWRITE);
            const int rc = _wutime64(wp.c_str(), &ub);
            _wchmod(wp.c_str(), _S_IREAD);
            if (rc != 0) raise_os_(std::error_code(5, std::system_category()), p);
        }
#endif
    }
    const int64_t ticks = (mtime_ns / 100) + 116444736000000000LL;   // (100 ns since 1601)
    fs_::last_write_time(fspath_(p), fs_::file_time_type(std::chrono::duration<int64_t, std::ratio<1, 10000000>>(ticks)), ec);
    if (ec) raise_os_(ec, p);
}
inline void utime(const str& p, const std::optional<std::tuple<double, double>>& times = std::nullopt) {
    if (!times) {
        const int64_t now = static_cast<int64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
        utime_ns(p, now, now);
        return;
    }
    auto split = [](double d) {   // _PyTime_DoubleToDenominator, rounding down
        double ip;
        double fp = std::modf(d, &ip) * 1e9;
        fp = std::floor(fp);
        if (fp >= 1e9) { fp -= 1e9; ip += 1.0; }
        else if (fp < 0) { fp += 1e9; ip -= 1.0; }
        return static_cast<int64_t>(ip) * 1000000000LL + static_cast<int64_t>(fp);
    };
    utime_ns(p, split(std::get<0>(*times)), split(std::get<1>(*times)));
}
// os.chmod(path, mode): on Windows only the read-only attribute follows the write bit
inline void chmod(const str& p, int64_t mode) {
    nul_check_("chmod", p);
#ifdef _WIN32
    if (_wchmod(fspath_(p).wstring().c_str(), (mode & 0200) ? (_S_IREAD | _S_IWRITE) : _S_IREAD) != 0) raise_os_(std::error_code(errno == ENOENT ? 2 : 5, std::system_category()), p);
#else
    std::error_code ec;
    fs_::permissions(fspath_(p), static_cast<fs_::perms>(mode & 07777), ec);
    if (ec) raise_os_(ec, p);
#endif
}

namespace path {
inline bool isabs(const str& s) {
    const auto& p = s.u32();
    if (p.size() >= 2 && sep_(p[0]) && sep_(p[1])) return true;
    auto d = splitdrive_(p);
    return !d.second.empty() && sep_(d.second[0]) && !d.first.empty();
}
// ntpath.normpath
inline str normpath(const str& s) {
    std::u32string p = s.u32();
    for (auto& c : p)
        if (c == '/') c = '\\';
    auto d = splitdrive_(p);
    std::u32string prefix = d.first, rest = d.second;
    if (!rest.empty() && rest[0] == '\\') {
        prefix += U"\\";
        size_t i = 0;
        while (i < rest.size() && rest[i] == '\\') i++;
        rest = rest.substr(i);
    }
    std::vector<std::u32string> comps, out;
    size_t i = 0;
    while (i <= rest.size()) {
        size_t j = rest.find(U'\\', i);
        if (j == std::u32string::npos) j = rest.size();
        comps.push_back(rest.substr(i, j - i));
        i = j + 1;
    }
    for (auto& c : comps) {
        if (c.empty() || c == U".") continue;
        if (c == U"..") {
            if (!out.empty() && out.back() != U"..") out.pop_back();
            else if (!prefix.empty() && prefix.back() == '\\') {
            } else out.push_back(c);
        } else out.push_back(c);
    }
    if (prefix.empty() && out.empty()) return str(".");
    std::u32string r = prefix;
    for (size_t k = 0; k < out.size(); k++) r += (k ? U"\\" : U"") + out[k];
    return str(r);
}
inline str abspath(const str& s) {
    // (Windows' GetFullPathNameW, which ntpath.abspath calls: a last name's trailing spaces and dots go)
    std::u32string r = (isabs(s) ? normpath(s) : normpath(os::path::join(py::os::getcwd(), s))).u32();
    size_t cut = r.size();
    while (cut > 0 && (r[cut - 1] == ' ' || r[cut - 1] == '.')) cut--;
    if (cut < r.size() && cut > 0 && r[cut - 1] != '\\' && r[cut - 1] != ':') r.resize(cut);
    return str(r);
}
inline str realpath(const str& s) {
    std::error_code ec;
    auto c = fs_::weakly_canonical(fspath_(abspath(s)), ec);
    return ec ? abspath(s) : pystr_(c);
}
inline str expanduser(const str& s) {
    const auto& u = s.u32();
    if (u.empty() || u[0] != '~') return s;
    size_t i = 1;
    while (i < u.size() && !sep_(u[i])) i++;
    auto home = environ_::get(str("USERPROFILE"));
    if (!home) return s;
    if (i != 1) return os::path::join(dirname(*home), str(u.substr(1, i - 1))) + str(u.substr(i));
    return *home + str(u.substr(i));
}
inline str relpath(const str& p, const std::optional<str>& start = std::nullopt) {
    str a = abspath(p), b = abspath(start ? *start : str("."));
    auto split_all = [](const str& x) {
        std::vector<str> v;
        for (const auto& c : py::split(x, str("\\"))) if (!c.u32().empty()) v.push_back(c);
        return v;
    };
    auto da = splitdrive_(a.u32()), db = splitdrive_(b.u32());
    if (str(da.first).lower() != str(db.first).lower())
        throw ValueError("path is on mount " + repr(str(da.first)).utf8() + ", start on mount " + repr(str(db.first)).utf8());
    auto pa = split_all(str(da.second)), pb = split_all(str(db.second));
    size_t i = 0;
    while (i < pa.size() && i < pb.size() && pa[i].lower() == pb[i].lower()) i++;
    std::u32string r;
    for (size_t k = i; k < pb.size(); k++) r += (r.empty() ? U"" : U"\\") + std::u32string(U"..");
    for (size_t k = i; k < pa.size(); k++) r += (r.empty() ? U"" : U"\\") + pa[k].u32();
    return r.empty() ? str(".") : str(r);
}
inline bool islink(const str& s) {
    if (has_nul_(s)) return false;
    std::error_code ec;
    return fs_::is_symlink(fspath_(s), ec);
}
inline double getmtime(const str& s) {
    nul_check_("stat", s);
    std::error_code ec;
    auto t = fs_::last_write_time(fspath_(s), ec);
    if (ec) throw FileNotFoundError("[WinError 2] The system cannot find the file specified: " + repr(s).utf8());
    auto sys = std::chrono::duration_cast<std::chrono::nanoseconds>(t.time_since_epoch()).count();
    return static_cast<double>(sys - 116444736000000000LL * 100) / 1e9;   // (file time epoch 1601 -> 1970)
}
}  // namespace path

#ifdef _WIN32
// os.makedirs as Python's (os.py) writes it: the parents first, then mkdir, one system call a folder; the
// error is the system's own, by _wmkdir (the CRT's mapping of CreateDirectoryW), as raise_os_ words it
inline void makedirs_win_(const str& name, bool exist_ok) {
    auto [head, tail] = path::split(name);
    if (tail.utf8().empty()) std::tie(head, tail) = path::split(head);
    const std::string h = head.utf8(), t = tail.utf8();
    if (!h.empty() && !t.empty() && _waccess(fspath_(head).wstring().c_str(), 0) != 0) {
        try {
            makedirs_win_(head, exist_ok);
        } catch (const FileExistsError&) {
        }
        if (t == ".") return;
    }
    if (_wmkdir(fspath_(name).wstring().c_str()) == 0) return;
    int e = errno;
    int code = e == EEXIST ? 183 : e == ENOENT ? 3 : e == EACCES ? 5 : 0;
    if (code == 0) {
        unsigned long dos = 0;
        _get_doserrno(&dos);
        code = static_cast<int>(dos);
    }
    std::error_code ec;
    if (!exist_ok || !fs_::is_directory(fspath_(name), ec)) raise_os_(std::error_code(code, std::system_category()), name);
}
#endif
inline void makedirs(const str& p, int64_t mode = 0777, bool exist_ok = false) {
    (void)mode;
    nul_check_("mkdir", p);
#ifdef _WIN32
    makedirs_win_(p, exist_ok);
    return;
#endif
    std::error_code ec;
    fs_::path q = fspath_(p);
    if (fs_::exists(q, ec)) {
        if (!exist_ok || !fs_::is_directory(q, ec)) throw FileExistsError("[WinError 183] Cannot create a file when that file already exists: " + repr(p).utf8());
        return;
    }
    fs_::create_directories(q, ec);
    if (ec) throw OSError(ec.message());
}
inline void remove(const str& p) {
    nul_check_("remove", p);
    std::error_code ec;
    fs_::path q = fspath_(p);
    if (fs_::is_directory(q, ec)) throw PermissionError("[WinError 5] Access is denied: " + repr(p).utf8());
    if (!fs_::remove(q, ec)) throw FileNotFoundError("[WinError 2] The system cannot find the file specified: " + repr(p).utf8());
}
inline void unlink(const str& p) { remove(p); }
inline void rmdir(const str& p) {
    nul_check_("rmdir", p);
    std::error_code ec;
    fs_::path q = fspath_(p);
    if (!fs_::exists(q, ec)) throw FileNotFoundError("[WinError 2] The system cannot find the file specified: " + repr(p).utf8());
    if (!fs_::is_directory(q, ec)) throw NotADirectoryError("[WinError 267] The directory name is invalid: " + repr(p).utf8());
    if (!fs_::is_empty(q, ec)) throw OSError("[WinError 145] The directory is not empty: " + repr(p).utf8());
    fs_::remove(q, ec);
}
inline void removedirs(const str& p) { rmdir(p); }
inline void replace(const str& a, const str& b) {
    std::error_code ec;
    if (!fs_::exists(fspath_(a), ec)) throw FileNotFoundError("[WinError 2] The system cannot find the file specified: " + repr(a).utf8());
    fs_::rename(fspath_(a), fspath_(b), ec);
    if (ec) throw PermissionError(ec.message());
}
inline void rename(const str& a, const str& b) {
    std::error_code ec;
    if (fs_::exists(fspath_(b), ec)) throw FileExistsError("[WinError 183] Cannot create a file when that file already exists: " + repr(a).utf8());
    replace(a, b);
}
// os.walk(top): (dirpath, dirnames, filenames) top-down, lazily, as CPython's: a folder is read when the loop
// reaches it, and the folders gone into are the ones in dirnames when the loop moves on (dirs[:] = ... prunes
// or reorders them, as it does Python's); a folder that cannot be read is passed over, a link to one not gone into
class walk_t {
    str top_;

  public:
    explicit walk_t(const str& top) : top_(top) {}
    struct iterator {
        std::vector<str> todo;
        std::tuple<str, list<str>, list<str>> cur;
        bool done = true;
        void fill() {
            while (!todo.empty()) {
                str d = todo.back();
                todo.pop_back();
                std::error_code ec;
                fs_::directory_iterator it(fspath_(d), ec);
                if (ec) continue;
                list<str> dirs, files;
                for (; it != fs_::directory_iterator(); it.increment(ec)) {
                    std::error_code e2;
                    (it->is_directory(e2) ? dirs : files).append(str(it->path().filename().u8string()));
                    if (ec) break;
                }
                cur = std::make_tuple(d, dirs, files);
                done = false;
                return;
            }
            done = true;
        }
        iterator& operator++() {
            const str& d = std::get<0>(cur);
            const list<str>& dirs = std::get<1>(cur);
            for (int64_t i = dirs.size() - 1; i >= 0; i--) {
                str sub = path::join(d, dirs[i]);
                std::error_code ec;
                if (!fs_::is_symlink(fspath_(sub), ec)) todo.push_back(sub);
            }
            fill();
            return *this;
        }
        const std::tuple<str, list<str>, list<str>>& operator*() const { return cur; }
        bool operator!=(const iterator& o) const { return done != o.done; }
        bool operator==(const iterator& o) const { return done == o.done; }
    };
    iterator begin() const {
        nul_check_("scandir", top_);      // (os.walk's scandir raises it: not an OSError, so not skipped)
        iterator it;
        std::error_code ec;
        if (fs_::is_directory(fspath_(top_), ec)) it.todo.push_back(top_);
        it.fill();
        return it;
    }
    iterator end() const { return iterator{}; }
};
inline walk_t walk(const str& top) { return walk_t(top); }
}  // namespace os

namespace shutil {
inline void copyfile(const str& a, const str& b) {   // shutil.copyfile: the bytes (the copy has the time of its writing, and no attributes)
    if (os::path::isdir(a)) throw IsADirectoryError("[Errno 21] Is a directory: " + repr(a).utf8());
    file in = file::open(a, str("rb"));
    file out = file::open(b, str("wb"));
    for (;;) {
        const bytes chunk = in.read(1 << 20);
        if (chunk.raw().empty()) break;
        out.write(chunk);
    }
    out.close();
    in.close();
}
// shutil.copymode / copystat / copy / copy2 (Windows: the mode is the read-only bit, the times are exact to 100 ns)
inline void copymode(const str& src, const str& dst) { os::chmod(dst, os::stat(src).st_mode & 07777); }
inline void copystat(const str& src, const str& dst) {
    const os::stat_result st = os::stat(src);
    os::utime_ns(dst, st.st_atime_ns, st.st_mtime_ns);
    os::chmod(dst, st.st_mode & 07777);
}
inline str copy(const str& a, const str& b) {
    str dst = os::path::isdir(b) ? os::path::join(b, os::path::basename(a)) : b;
    copyfile(a, dst);
    copymode(a, dst);
    return dst;
}
inline str copy2(const str& a, const str& b) {   // copy, and the times and mode with it
    str dst = os::path::isdir(b) ? os::path::join(b, os::path::basename(a)) : b;
    copyfile(a, dst);
    copystat(a, dst);
    return dst;
}
// shutil.Error: str() is the list of (src, dst, why) tuples
struct Error : Exception {
    list<std::tuple<str, str, str>> errors;
    explicit Error(const list<std::tuple<str, str, str>>& e) : Exception(repr(e).utf8()), errors(e) {}
    const char* type() const override { return "Error"; }
};
// shutil.copytree(src, dst, symlinks=False, ignore=None, copy_function=copy2, dirs_exist_ok=False); links are followed
// (symlinks=True, ignore_dangling_symlinks are not offered)
using IgnoreFn = std::function<set<str>(const str&, const list<str>&)>;
inline str copytree(const str& src, const str& dst, const IgnoreFn& ignore = nullptr, const std::function<str(const str&, const str&)>& copy_function = nullptr,
                    bool dirs_exist_ok = false) {
    std::vector<std::pair<str, bool>> entries;   // (name, is_dir): the folder in the order the system lists it
    {
        std::error_code ec;
        auto it = fs_::directory_iterator(fspath_(src), ec);
        if (ec) os::raise_os_(ec, src);
        for (const auto& e : it) {
            std::error_code e2;
            entries.emplace_back(str(e.path().filename().u8string()), fs_::is_directory(e.path(), e2));
        }
    }
    set<str> ignored;
    if (ignore) {
        list<str> names;
        for (const auto& e : entries) names.append(e.first);
        ignored = ignore(src, names);
    }
    os::makedirs(dst, 0777, dirs_exist_ok);
    list<std::tuple<str, str, str>> errors;
    for (const auto& e : entries) {
        if (ignored.contains(e.first)) continue;
        const str srcname = os::path::join(src, e.first), dstname = os::path::join(dst, e.first);
        try {
            if (e.second) copytree(srcname, dstname, ignore, copy_function, dirs_exist_ok);
            else if (copy_function) copy_function(srcname, dstname);
            else copy2(srcname, dstname);
        } catch (const Error& err) {
            for (const auto& x : err.errors) errors.append(x);
        } catch (const OSError& why) {
            errors.append(std::make_tuple(srcname, dstname, str(why.what())));
        }
    }
    try {
        copystat(src, dst);
    } catch (const OSError&) {   // (on Windows an OSError here carries a winerror: it is not reported)
    }
    if (!errors.empty()) throw Error(errors);
    return dst;
}
inline str move(const str& a, const str& b) {
    str dst = os::path::isdir(b) ? os::path::join(b, os::path::basename(a)) : b;
    std::error_code ec;
    fs_::rename(fspath_(a), fspath_(dst), ec);
    if (ec) {
        fs_::copy(fspath_(a), fspath_(dst), fs_::copy_options::recursive | fs_::copy_options::overwrite_existing, ec);
        if (ec) throw FileNotFoundError("[Errno 2] No such file or directory: " + repr(a).utf8());
        fs_::remove_all(fspath_(a), ec);
    }
    return dst;
}
inline void rmtree(const str& p, bool ignore_errors = false) {
    std::error_code ec;
    if (!fs_::exists(fspath_(p), ec)) {
        if (ignore_errors) return;
        throw FileNotFoundError("[WinError 3] The system cannot find the path specified: " + repr(p).utf8());
    }
    fs_::remove_all(fspath_(p), ec);
    if (ec && !ignore_errors) throw OSError(ec.message());
}
inline std::optional<str> which(const str& cmd) {
    auto path = os::environ_::get(str("PATH"));
    auto ext = os::environ_::get(str("PATHEXT"));
    list<str> exts = py::split(ext ? *ext : str(".COM;.EXE;.BAT;.CMD"), str(";"));
    auto has_ext = [&](const str& c) {
        for (const auto& e : exts)
            if (c.lower().endswith(e.lower())) return true;
        return false;
    };
    auto try_dir = [&](const str& d) -> std::optional<str> {
        str base = os::path::join(d, cmd);
        if (has_ext(cmd) && os::path::isfile(base)) return base;
        for (const auto& e : exts) {
            str c = base + e;
            if (os::path::isfile(c)) return c;
        }
        return std::nullopt;
    };
    if (!std::get<0>(os::path::split(cmd)).u32().empty()) return try_dir(str(""));
    if (auto r = try_dir(os::getcwd())) return r;
    if (path)
        for (const auto& d : py::split(*path, str(";")))
            if (!d.u32().empty())
                if (auto r = try_dir(d)) return r;
    return std::nullopt;
}
}  // namespace shutil

namespace time {
inline double time() {
    return std::chrono::duration<double>(std::chrono::system_clock::now().time_since_epoch()).count();
}
inline double perf_counter() {
    return std::chrono::duration<double>(std::chrono::steady_clock::now().time_since_epoch()).count();
}
inline double monotonic() { return perf_counter(); }
inline void sleep(double s) {
    if (s < 0) throw ValueError("sleep length must be non-negative");
    std::this_thread::sleep_for(std::chrono::duration<double>(s));
}
// time.struct_time: the fields in the order of its tuple (tm_mon 1-12, tm_wday 0 = Monday, tm_yday 1-366)
struct struct_time {
    int64_t tm_year = 1970, tm_mon = 1, tm_mday = 1, tm_hour = 0, tm_min = 0, tm_sec = 0, tm_wday = 3, tm_yday = 1, tm_isdst = 0;
};
namespace detail_ {
inline struct_time from_tm_(const std::tm& t) {
    struct_time r;
    r.tm_year = t.tm_year + 1900;
    r.tm_mon = t.tm_mon + 1;
    r.tm_mday = t.tm_mday;
    r.tm_hour = t.tm_hour;
    r.tm_min = t.tm_min;
    r.tm_sec = t.tm_sec;
    r.tm_wday = (t.tm_wday + 6) % 7;
    r.tm_yday = t.tm_yday + 1;
    r.tm_isdst = t.tm_isdst;
    return r;
}
// the seconds a time function takes: now, or the number rounded down (_PyTime_ObjectToTime_t, ROUND_FLOOR)
inline std::time_t seconds_(const std::optional<double>& secs) {
    if (!secs) return static_cast<std::time_t>(time());
    const double d = *secs;
    if (std::isnan(d)) throw ValueError("Invalid value NaN (not a number)");
    const double f = std::floor(d);
    if (f < -9223372036854775808.0 || f >= 9223372036854775808.0) throw OverflowError("timestamp out of range for platform time_t");
    return static_cast<std::time_t>(f);
}
}  // namespace detail_
inline struct_time gmtime(const std::optional<double>& secs = std::nullopt) {
    const std::time_t t = detail_::seconds_(secs);
    std::tm tm{};
#ifdef _WIN32
    if (gmtime_s(&tm, &t) != 0) throw OSError("[Errno 22] Invalid argument");
#else
    if (!gmtime_r(&t, &tm)) throw OSError("[Errno 22] Invalid argument");
#endif
    return detail_::from_tm_(tm);
}
inline struct_time localtime(const std::optional<double>& secs = std::nullopt) {
    const std::time_t t = detail_::seconds_(secs);
    std::tm tm{};
#ifdef _WIN32
    if (localtime_s(&tm, &t) != 0) throw OSError("[Errno 22] Invalid argument");
#else
    if (!localtime_r(&t, &tm)) throw OSError("[Errno 22] Invalid argument");
#endif
    return detail_::from_tm_(tm);
}
// time.strftime(format, t=localtime()): the C runtime's wcsftime (as CPython uses on Windows), after CPython's range checks
inline str strftime(const str& fmt, const std::optional<struct_time>& when = std::nullopt) {
    const struct_time t = when ? *when : localtime();
    std::tm tm{};
    tm.tm_year = static_cast<int>(t.tm_year - 1900);
    tm.tm_mon = static_cast<int>(t.tm_mon - 1);
    tm.tm_mday = static_cast<int>(t.tm_mday);
    tm.tm_hour = static_cast<int>(t.tm_hour);
    tm.tm_min = static_cast<int>(t.tm_min);
    tm.tm_sec = static_cast<int>(t.tm_sec);
    tm.tm_wday = static_cast<int>((t.tm_wday + 1) % 7);
    tm.tm_yday = static_cast<int>(t.tm_yday - 1);
    tm.tm_isdst = static_cast<int>(t.tm_isdst);
    if (tm.tm_mon == -1) tm.tm_mon = 0;
    else if (tm.tm_mon < 0 || tm.tm_mon > 11) throw ValueError("month out of range");
    if (tm.tm_mday == 0) tm.tm_mday = 1;
    else if (tm.tm_mday < 0 || tm.tm_mday > 31) throw ValueError("day of month out of range");
    if (tm.tm_hour < 0 || tm.tm_hour > 23) throw ValueError("hour out of range");
    if (tm.tm_min < 0 || tm.tm_min > 59) throw ValueError("minute out of range");
    if (tm.tm_sec < 0 || tm.tm_sec > 61) throw ValueError("seconds out of range");
    if (tm.tm_wday < 0) throw ValueError("day of week out of range");
    if (tm.tm_yday == -1) tm.tm_yday = 0;
    else if (tm.tm_yday < 0 || tm.tm_yday > 365) throw ValueError("day of year out of range");
    if (tm.tm_isdst < -1) tm.tm_isdst = -1;
    else if (tm.tm_isdst > 1) tm.tm_isdst = 1;
    if (t.tm_year < 1 || t.tm_year > 9999) throw ValueError("strftime() requires year in [1; 9999]");
    // the C runtime aborts on a conversion it does not know; CPython turns that into ValueError("Invalid format string")
    const std::u32string& f = fmt.u32();
    for (size_t i = 0; i < f.size(); i++) {
        if (f[i] != U'%') continue;
        i++;
        if (i < f.size() && f[i] == U'#') i++;
        if (i >= f.size() || std::u32string(U"aAbBcCdDeFgGhHIjmMnprRStTuUVwWxXyYzZ%").find(f[i]) == std::u32string::npos) throw ValueError("Invalid format string");
        // (the C runtime also refuses a field a directive reads when it is out of the range it draws: the seconds 60 and 61 among them)
        const auto in = [](int v, int lo, int hi) { return v >= lo && v <= hi; };
        const std::u32string use_s = U"cTXr", use_h = U"cTXrRHIp", use_m = U"cTXrRM", use_date = U"cxDFdeCyYGgVUWjmbBh";
        const char32_t d = f[i];
        bool ok = true;
        if (std::u32string(U"aAuwcU W").find(d) != std::u32string::npos) ok = ok && in(tm.tm_wday, 0, 6);
        if (std::u32string(U"bBhmcxDF").find(d) != std::u32string::npos) ok = ok && in(tm.tm_mon, 0, 11);
        if (std::u32string(U"deFxDc").find(d) != std::u32string::npos) ok = ok && in(tm.tm_mday, 1, 31);
        if (std::u32string(U"jUW").find(d) != std::u32string::npos) ok = ok && in(tm.tm_yday, 0, 365);
        if (use_h.find(d) != std::u32string::npos) ok = ok && in(tm.tm_hour, 0, 23);
        if (use_m.find(d) != std::u32string::npos) ok = ok && in(tm.tm_min, 0, 59);
        if (use_s.find(d) != std::u32string::npos || d == U'S') ok = ok && in(tm.tm_sec, 0, 59);
        (void)use_date;
        if (!ok) throw ValueError("Invalid format string");
    }
    if (f.empty()) return str("");
    std::wstring wfmt;
    for (char32_t c : f) {
        if (c >= 0x10000) { wfmt.push_back(static_cast<wchar_t>(0xD800 + ((c - 0x10000) >> 10))); wfmt.push_back(static_cast<wchar_t>(0xDC00 + ((c - 0x10000) & 0x3FF))); }
        else wfmt.push_back(static_cast<wchar_t>(c));
    }
    for (size_t size = 1024; size <= 256 * wfmt.size() + 1024; size *= 2) {
        std::wstring buf(size, L'\0');
        const size_t n = std::wcsftime(&buf[0], size, wfmt.c_str(), &tm);
        if (n > 0) {
            buf.resize(n);
            std::u32string out;
            for (size_t i = 0; i < buf.size(); i++) {
                char32_t c = buf[i];
                if (c >= 0xD800 && c < 0xDC00 && i + 1 < buf.size()) c = 0x10000 + ((c - 0xD800) << 10) + (buf[++i] - 0xDC00);
                out.push_back(c);
            }
            return str(out);
        }
    }
    return str("");
}
// time.mktime(t): the local time as seconds (OverflowError "mktime argument out of range")
inline double mktime(const struct_time& t) {
    std::tm tm{};
    tm.tm_year = static_cast<int>(t.tm_year - 1900);
    tm.tm_mon = static_cast<int>(t.tm_mon - 1);
    tm.tm_mday = static_cast<int>(t.tm_mday);
    tm.tm_hour = static_cast<int>(t.tm_hour);
    tm.tm_min = static_cast<int>(t.tm_min);
    tm.tm_sec = static_cast<int>(t.tm_sec);
    tm.tm_isdst = static_cast<int>(t.tm_isdst);
    tm.tm_wday = -1;   // (a sentinel: mktime sets it when it succeeds)
    const std::time_t r = std::mktime(&tm);
    if (r == static_cast<std::time_t>(-1) && tm.tm_wday == -1) throw OverflowError("mktime argument out of range");
    return static_cast<double>(r);
}
}  // namespace time

// ------------------------------------------------------------------------------------------ hashlib
#if defined(_MSC_VER) && !defined(__clang__)
#pragma optimize("t", on)
#endif
namespace hashlib {
// SHA-1 and SHA-256 on the x86 SHA extensions, when the processor has them (one block a call; the portable
// code in Hash is the answer elsewhere). The same digests, a few times faster.
#if (defined(_M_X64) || defined(__x86_64__)) && (defined(_MSC_VER) || defined(__GNUC__) || defined(__clang__))
#define PY_HAVE_SHANI 1
#if defined(_MSC_VER) && !defined(__clang__)
#define PY_SHANI_TARGET
#else
#define PY_SHANI_TARGET __attribute__((target("sha,ssse3,sse4.1")))
#endif
namespace shani_ {
inline bool available() {
    static const bool ok = [] {
        int r[4];
#if defined(_MSC_VER) && !defined(__clang__)
        __cpuid(r, 0);
        if (r[0] < 7) return false;
        __cpuid(r, 1);
        const bool sse = (r[2] & (1 << 9)) && (r[2] & (1 << 19));   // SSSE3, SSE4.1
        __cpuidex(r, 7, 0);
        return sse && (r[1] & (1 << 29)) != 0;   // SHA
#else
        unsigned a, b, c, d;
        if (!__get_cpuid_count(7, 0, &a, &b, &c, &d)) return false;
        const bool sha = (b & (1u << 29)) != 0;
        if (!__get_cpuid(1, &a, &b, &c, &d)) return false;
        return sha && (c & (1u << 9)) && (c & (1u << 19));
#endif
    }();
    return ok;
}
PY_SHANI_TARGET inline void sha1_blocks(uint32_t state[5], const unsigned char* data, size_t nblocks) {
    const __m128i MASK = _mm_set_epi64x(0x0001020304050607ULL, 0x08090a0b0c0d0e0fULL);
    __m128i ABCD = _mm_shuffle_epi32(_mm_loadu_si128(reinterpret_cast<const __m128i*>(state)), 0x1B);
    __m128i E0 = _mm_set_epi32(static_cast<int>(state[4]), 0, 0, 0);
    for (; nblocks; nblocks--, data += 64) {
    const __m128i ABCD_SAVE = ABCD, E_SAVE = E0;
    __m128i M[4], Ec, Ep;
    for (int i = 0; i < 4; i++) M[i] = _mm_shuffle_epi8(_mm_loadu_si128(reinterpret_cast<const __m128i*>(data + 16 * i)), MASK);
    Ec = _mm_add_epi32(E0, M[0]);
    Ep = ABCD;
    ABCD = _mm_sha1rnds4_epu32(ABCD, Ec, 0);
#define PY_SHA1_NI_GROUP(F)                                                                     \
    {                                                                                           \
        Ec = _mm_sha1nexte_epu32(Ep, M[g & 3]);                                                 \
        Ep = ABCD;                                                                              \
        ABCD = _mm_sha1rnds4_epu32(ABCD, Ec, F);                                                \
        if (g >= 3 && g <= 18)                                                                  \
            M[(g + 1) & 3] = _mm_sha1msg2_epu32(                                                \
                _mm_xor_si128(_mm_sha1msg1_epu32(M[(g + 1) & 3], M[(g + 2) & 3]), M[(g + 3) & 3]), M[g & 3]); \
    }
    int g = 1;
    for (; g < 5; g++) PY_SHA1_NI_GROUP(0)
    for (; g < 10; g++) PY_SHA1_NI_GROUP(1)
    for (; g < 15; g++) PY_SHA1_NI_GROUP(2)
    for (; g < 20; g++) PY_SHA1_NI_GROUP(3)
#undef PY_SHA1_NI_GROUP
    Ec = _mm_sha1nexte_epu32(Ep, E_SAVE);
    ABCD = _mm_add_epi32(ABCD, ABCD_SAVE);
    E0 = Ec;
    }
    _mm_storeu_si128(reinterpret_cast<__m128i*>(state), _mm_shuffle_epi32(ABCD, 0x1B));
    state[4] = static_cast<uint32_t>(_mm_extract_epi32(E0, 3));
}
PY_SHANI_TARGET inline void sha256_blocks(uint32_t state[8], const unsigned char* data, size_t nblocks, const uint32_t* K) {
    const __m128i MASK = _mm_set_epi64x(0x0c0d0e0f08090a0bULL, 0x0405060700010203ULL);
    __m128i TMP = _mm_shuffle_epi32(_mm_loadu_si128(reinterpret_cast<const __m128i*>(&state[0])), 0xB1);   // CDAB
    __m128i STATE1 = _mm_shuffle_epi32(_mm_loadu_si128(reinterpret_cast<const __m128i*>(&state[4])), 0x1B);   // EFGH
    __m128i STATE0 = _mm_alignr_epi8(TMP, STATE1, 8);                                                         // ABEF
    STATE1 = _mm_blend_epi16(STATE1, TMP, 0xF0);                                                              // CDGH
    for (; nblocks; nblocks--, data += 64) {
    const __m128i ABEF_SAVE = STATE0, CDGH_SAVE = STATE1;
    __m128i M[4];
    for (int i = 0; i < 4; i++) M[i] = _mm_shuffle_epi8(_mm_loadu_si128(reinterpret_cast<const __m128i*>(data + 16 * i)), MASK);
#define PY_SHA256_NI_GROUP(g, SCHEDULE)                                                                       \
    {                                                                                                       \
        __m128i MSG = _mm_add_epi32(M[(g) & 3], _mm_loadu_si128(reinterpret_cast<const __m128i*>(K + 4 * (g)))); \
        STATE1 = _mm_sha256rnds2_epu32(STATE1, STATE0, MSG);                                                \
        MSG = _mm_shuffle_epi32(MSG, 0x0E);                                                                 \
        STATE0 = _mm_sha256rnds2_epu32(STATE0, STATE1, MSG);                                                \
        if (SCHEDULE) {   /* the words of group g + 4, into the slot group g held */                         \
            __m128i t = _mm_add_epi32(_mm_sha256msg1_epu32(M[(g) & 3], M[((g) + 1) & 3]),                   \
                                      _mm_alignr_epi8(M[((g) + 3) & 3], M[((g) + 2) & 3], 4));              \
            M[(g) & 3] = _mm_sha256msg2_epu32(t, M[((g) + 3) & 3]);                                         \
        }                                                                                                   \
    }
    PY_SHA256_NI_GROUP(0, 1) PY_SHA256_NI_GROUP(1, 1) PY_SHA256_NI_GROUP(2, 1) PY_SHA256_NI_GROUP(3, 1)
    PY_SHA256_NI_GROUP(4, 1) PY_SHA256_NI_GROUP(5, 1) PY_SHA256_NI_GROUP(6, 1) PY_SHA256_NI_GROUP(7, 1)
    PY_SHA256_NI_GROUP(8, 1) PY_SHA256_NI_GROUP(9, 1) PY_SHA256_NI_GROUP(10, 1) PY_SHA256_NI_GROUP(11, 1)
    PY_SHA256_NI_GROUP(12, 0) PY_SHA256_NI_GROUP(13, 0) PY_SHA256_NI_GROUP(14, 0) PY_SHA256_NI_GROUP(15, 0)
#undef PY_SHA256_NI_GROUP
    STATE0 = _mm_add_epi32(STATE0, ABEF_SAVE);
    STATE1 = _mm_add_epi32(STATE1, CDGH_SAVE);
    }
    TMP =_mm_shuffle_epi32(STATE0, 0x1B);                // FEBA
    STATE1 = _mm_shuffle_epi32(STATE1, 0xB1);             // DCHG
    STATE0 = _mm_blend_epi16(TMP, STATE1, 0xF0);          // DCBA
    STATE1 = _mm_alignr_epi8(STATE1, TMP, 8);             // HGFE
    _mm_storeu_si128(reinterpret_cast<__m128i*>(&state[0]), STATE0);
    _mm_storeu_si128(reinterpret_cast<__m128i*>(&state[4]), STATE1);
}
}  // namespace shani_
#endif
class Hash {
    // streaming state: the words, the bytes of an unfinished block, the length so far
    struct S {
        int kind;   // 0 md5, 1 sha1, 2 sha256, 3 sha512
        uint32_t h32[8];
        uint64_t h64[8];
        std::string pending;
        uint64_t total = 0;
    };
    std::shared_ptr<S> s_;
    static uint32_t rol(uint32_t x, int n) { return (x << n) | (x >> (32 - n)); }
    static uint32_t ror(uint32_t x, int n) { return (x >> n) | (x << (32 - n)); }
    static uint64_t ror64(uint64_t x, int n) { return (x >> n) | (x << (64 - n)); }
    static uint32_t be32_(const unsigned char* q) {
#if defined(_MSC_VER)
        uint32_t v;
        std::memcpy(&v, q, 4);
        return _byteswap_ulong(v);
#else
        return static_cast<uint32_t>(q[0]) << 24 | static_cast<uint32_t>(q[1]) << 16 | static_cast<uint32_t>(q[2]) << 8 | q[3];
#endif
    }
    static uint32_t le32_(const unsigned char* q) {
        return static_cast<uint32_t>(q[0]) | static_cast<uint32_t>(q[1]) << 8 | static_cast<uint32_t>(q[2]) << 16 | static_cast<uint32_t>(q[3]) << 24;
    }
    static void md5_block_(uint32_t* h, const unsigned char* q) {
        static const uint32_t K[64] = {
            0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501, 0x698098d8, 0x8b44f7af,
            0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821, 0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
            0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8, 0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8,
            0x676f02d9, 0x8d2a4c8a, 0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
            0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665, 0xf4292244, 0x432aff97,
            0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1, 0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
            0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};
        static const int R[64] = {7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 5, 9,  14, 20, 5, 9,
                                  14, 20, 5, 9,  14, 20, 5, 9,  14, 20, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
                                  4, 11, 16, 23, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};
            uint32_t w[16];
            for (int i = 0; i < 16; i++) w[i] = le32_(q + 4 * i);
            uint32_t a = h[0], b = h[1], c = h[2], d = h[3];
#define PY_MD5_STEP(a, b, c, d, F, g, i) a = b + rol(a + (F) + K[i] + w[g], R[i])
            for (int i = 0; i < 16; i += 4) {
                PY_MD5_STEP(a, b, c, d, (b & c) | (~b & d), i, i);
                PY_MD5_STEP(d, a, b, c, (a & b) | (~a & c), i + 1, i + 1);
                PY_MD5_STEP(c, d, a, b, (d & a) | (~d & b), i + 2, i + 2);
                PY_MD5_STEP(b, c, d, a, (c & d) | (~c & a), i + 3, i + 3);
            }
            for (int i = 16; i < 32; i += 4) {
                PY_MD5_STEP(a, b, c, d, (d & b) | (~d & c), (5 * i + 1) & 15, i);
                PY_MD5_STEP(d, a, b, c, (c & a) | (~c & b), (5 * (i + 1) + 1) & 15, i + 1);
                PY_MD5_STEP(c, d, a, b, (b & d) | (~b & a), (5 * (i + 2) + 1) & 15, i + 2);
                PY_MD5_STEP(b, c, d, a, (a & c) | (~a & d), (5 * (i + 3) + 1) & 15, i + 3);
            }
            for (int i = 32; i < 48; i += 4) {
                PY_MD5_STEP(a, b, c, d, b ^ c ^ d, (3 * i + 5) & 15, i);
                PY_MD5_STEP(d, a, b, c, a ^ b ^ c, (3 * (i + 1) + 5) & 15, i + 1);
                PY_MD5_STEP(c, d, a, b, d ^ a ^ b, (3 * (i + 2) + 5) & 15, i + 2);
                PY_MD5_STEP(b, c, d, a, c ^ d ^ a, (3 * (i + 3) + 5) & 15, i + 3);
            }
            for (int i = 48; i < 64; i += 4) {
                PY_MD5_STEP(a, b, c, d, c ^ (b | ~d), (7 * i) & 15, i);
                PY_MD5_STEP(d, a, b, c, b ^ (a | ~c), (7 * (i + 1)) & 15, i + 1);
                PY_MD5_STEP(c, d, a, b, a ^ (d | ~b), (7 * (i + 2)) & 15, i + 2);
                PY_MD5_STEP(b, c, d, a, d ^ (c | ~a), (7 * (i + 3)) & 15, i + 3);
            }
#undef PY_MD5_STEP
            h[0] += a, h[1] += b, h[2] += c, h[3] += d;
    }
    static void sha1_block_(uint32_t* h, const unsigned char* q, size_t nb = 1) {
#ifdef PY_HAVE_SHANI
            if (shani_::available()) {
                shani_::sha1_blocks(h, q, nb);
                return;
            }
#endif
            uint32_t w[80];
            for (int i = 0; i < 16; i++) w[i] = be32_(q + 4 * i);
            for (int i = 16; i < 80; i++) w[i] = rol(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);
            uint32_t a = h[0], b = h[1], c = h[2], d = h[3], e = h[4];
#define PY_SHA1_ROUND(F, K, i)                      \
    {                                               \
        uint32_t t = rol(a, 5) + (F) + e + (K) + w[i]; \
        e = d, d = c, c = rol(b, 30), b = a, a = t; \
    }
            for (int i = 0; i < 20; i++) PY_SHA1_ROUND(((b & c) | (~b & d)), 0x5A827999u, i)
            for (int i = 20; i < 40; i++) PY_SHA1_ROUND((b ^ c ^ d), 0x6ED9EBA1u, i)
            for (int i = 40; i < 60; i++) PY_SHA1_ROUND(((b & c) | (b & d) | (c & d)), 0x8F1BBCDCu, i)
            for (int i = 60; i < 80; i++) PY_SHA1_ROUND((b ^ c ^ d), 0xCA62C1D6u, i)
#undef PY_SHA1_ROUND
            h[0] += a, h[1] += b, h[2] += c, h[3] += d, h[4] += e;
    }
    static void sha256_block_(uint32_t* h, const unsigned char* q, size_t nb = 1) {
        static const uint32_t K[64] = {
            0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01,
            0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
            0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da, 0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147,
            0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
            0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070, 0x19a4c116, 0x1e376c08,
            0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
            0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};
#ifdef PY_HAVE_SHANI
            if (shani_::available()) {
                shani_::sha256_blocks(h, q, nb, K);
                return;
            }
#endif
            uint32_t w[64];
            for (int i = 0; i < 16; i++) w[i] = be32_(q + 4 * i);
            for (int i = 16; i < 64; i++) {
                uint32_t s0 = ror(w[i - 15], 7) ^ ror(w[i - 15], 18) ^ (w[i - 15] >> 3);
                uint32_t s1 = ror(w[i - 2], 17) ^ ror(w[i - 2], 19) ^ (w[i - 2] >> 10);
                w[i] = w[i - 16] + s0 + w[i - 7] + s1;
            }
            uint32_t a = h[0], b = h[1], c = h[2], d = h[3], e = h[4], f = h[5], g = h[6], hh = h[7];
#define PY_SHA256_ROUND(a, b, c, d, e, f, g, h, i)                                                          \
    {                                                                                                         \
        uint32_t t1 = h + (ror(e, 6) ^ ror(e, 11) ^ ror(e, 25)) + ((e & f) ^ (~e & g)) + K[i] + w[i];         \
        d += t1;                                                                                              \
        h = t1 + (ror(a, 2) ^ ror(a, 13) ^ ror(a, 22)) + ((a & b) ^ (a & c) ^ (b & c));                       \
    }
            for (int i = 0; i < 64; i += 8) {
                PY_SHA256_ROUND(a, b, c, d, e, f, g, hh, i)
                PY_SHA256_ROUND(hh, a, b, c, d, e, f, g, i + 1)
                PY_SHA256_ROUND(g, hh, a, b, c, d, e, f, i + 2)
                PY_SHA256_ROUND(f, g, hh, a, b, c, d, e, i + 3)
                PY_SHA256_ROUND(e, f, g, hh, a, b, c, d, i + 4)
                PY_SHA256_ROUND(d, e, f, g, hh, a, b, c, i + 5)
                PY_SHA256_ROUND(c, d, e, f, g, hh, a, b, i + 6)
                PY_SHA256_ROUND(b, c, d, e, f, g, hh, a, i + 7)
            }
#undef PY_SHA256_ROUND
            h[0] += a, h[1] += b, h[2] += c, h[3] += d, h[4] += e, h[5] += f, h[6] += g, h[7] += hh;
    }
    static void sha512_block_(uint64_t* h, const unsigned char* p) {
        static const uint64_t K[80] = {
            0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL, 0xb5c0fbcfec4d3b2fULL, 0xe9b5dba58189dbbcULL, 0x3956c25bf348b538ULL,
            0x59f111f1b605d019ULL, 0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL, 0xd807aa98a3030242ULL, 0x12835b0145706fbeULL,
            0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL, 0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL, 0x9bdc06a725c71235ULL,
            0xc19bf174cf692694ULL, 0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL, 0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL,
            0x2de92c6f592b0275ULL, 0x4a7484aa6ea6e483ULL, 0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL, 0x983e5152ee66dfabULL,
            0xa831c66d2db43210ULL, 0xb00327c898fb213fULL, 0xbf597fc7beef0ee4ULL, 0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL,
            0x06ca6351e003826fULL, 0x142929670a0e6e70ULL, 0x27b70a8546d22ffcULL, 0x2e1b21385c26c926ULL, 0x4d2c6dfc5ac42aedULL,
            0x53380d139d95b3dfULL, 0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL, 0x81c2c92e47edaee6ULL, 0x92722c851482353bULL,
            0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL, 0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL, 0xd192e819d6ef5218ULL,
            0xd69906245565a910ULL, 0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL, 0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL,
            0x2748774cdf8eeb99ULL, 0x34b0bcb5e19b48a8ULL, 0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL, 0x5b9cca4f7763e373ULL,
            0x682e6ff3d6b2b8a3ULL, 0x748f82ee5defb2fcULL, 0x78a5636f43172f60ULL, 0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL,
            0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL, 0xbef9a3f7b2c67915ULL, 0xc67178f2e372532bULL, 0xca273eceea26619cULL,
            0xd186b8c721c0c207ULL, 0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL, 0x06f067aa72176fbaULL, 0x0a637dc5a2c898a6ULL,
            0x113f9804bef90daeULL, 0x1b710b35131c471bULL, 0x28db77f523047d84ULL, 0x32caab7b40c72493ULL, 0x3c9ebe0a15c9bebcULL,
            0x431d67c49c100d4cULL, 0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL, 0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL};
            uint64_t w[80];
            for (int i = 0; i < 16; i++) {
                w[i] = 0;
                for (int k = 0; k < 8; k++) w[i] = (w[i] << 8) | p[8 * i + k];
            }
            for (int i = 16; i < 80; i++) {
                uint64_t s0 = ror64(w[i - 15], 1) ^ ror64(w[i - 15], 8) ^ (w[i - 15] >> 7);
                uint64_t s1 = ror64(w[i - 2], 19) ^ ror64(w[i - 2], 61) ^ (w[i - 2] >> 6);
                w[i] = w[i - 16] + s0 + w[i - 7] + s1;
            }
            uint64_t a = h[0], b = h[1], c = h[2], d = h[3], e = h[4], f = h[5], g = h[6], hh = h[7];
#define PY_SHA512_ROUND(a, b, c, d, e, f, g, h, i)                                                          \
    {                                                                                                         \
        uint64_t t1 = h + (ror64(e, 14) ^ ror64(e, 18) ^ ror64(e, 41)) + ((e & f) ^ (~e & g)) + K[i] + w[i];  \
        d += t1;                                                                                              \
        h = t1 + (ror64(a, 28) ^ ror64(a, 34) ^ ror64(a, 39)) + ((a & b) ^ (a & c) ^ (b & c));                \
    }
            for (int i = 0; i < 80; i += 8) {
                PY_SHA512_ROUND(a, b, c, d, e, f, g, hh, i)
                PY_SHA512_ROUND(hh, a, b, c, d, e, f, g, i + 1)
                PY_SHA512_ROUND(g, hh, a, b, c, d, e, f, i + 2)
                PY_SHA512_ROUND(f, g, hh, a, b, c, d, e, i + 3)
                PY_SHA512_ROUND(e, f, g, hh, a, b, c, d, i + 4)
                PY_SHA512_ROUND(d, e, f, g, hh, a, b, c, i + 5)
                PY_SHA512_ROUND(c, d, e, f, g, hh, a, b, i + 6)
                PY_SHA512_ROUND(b, c, d, e, f, g, hh, a, i + 7)
            }
#undef PY_SHA512_ROUND
            h[0] += a, h[1] += b, h[2] += c, h[3] += d, h[4] += e, h[5] += f, h[6] += g, h[7] += hh;
    }
    static size_t block_size_(int kind) { return kind == 3 ? 128 : 64; }
    static void init_(S& s) {
        static const uint32_t i0[4] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476};
        static const uint32_t i1[5] = {0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0};
        static const uint32_t i2[8] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};
        static const uint64_t i3[8] = {0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL, 0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL, 0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL, 0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL};
        switch (s.kind) {
            case 0: std::memcpy(s.h32, i0, sizeof i0); break;
            case 1: std::memcpy(s.h32, i1, sizeof i1); break;
            case 2: std::memcpy(s.h32, i2, sizeof i2); break;
            default: std::memcpy(s.h64, i3, sizeof i3); break;
        }
    }
    static void block_(S& s, const unsigned char* q) {
        switch (s.kind) {
            case 0: md5_block_(s.h32, q); break;
            case 1: sha1_block_(s.h32, q); break;
            case 2: sha256_block_(s.h32, q); break;
            default: sha512_block_(s.h64, q); break;
        }
    }
    static void feed_(S& s, const char* d, size_t n) {
        const size_t B = block_size_(s.kind);
        s.total += n;
        const unsigned char* p = reinterpret_cast<const unsigned char*>(d);
        if (!s.pending.empty()) {
            size_t take = std::min(n, B - s.pending.size());
            s.pending.append(d, take);
            p += take;
            n -= take;
            if (s.pending.size() < B) return;
            block_(s, reinterpret_cast<const unsigned char*>(s.pending.data()));
            s.pending.clear();
        }
#ifdef PY_HAVE_SHANI
        if ((s.kind == 1 || s.kind == 2) && n >= B && shani_::available()) {   // all the whole blocks in one call
            const size_t nb = n / B;
            if (s.kind == 1) sha1_block_(s.h32, p, nb);
            else sha256_block_(s.h32, p, nb);
            p += nb * B;
            n -= nb * B;
        }
#endif
        for (; n >= B; p += B, n -= B) block_(s, p);
        s.pending.assign(reinterpret_cast<const char*>(p), n);
    }
    static std::string finish_(S s) {   // (a copy: the digest does not end the hash)
        const size_t B = block_size_(s.kind), lenbytes = s.kind == 3 ? 16 : 8;
        const bool big = s.kind != 0;
        std::string tail = s.pending;
        tail.push_back(static_cast<char>(0x80));
        while (tail.size() % B != B - lenbytes) tail.push_back(0);
        const uint64_t bits = s.total * 8;
        for (size_t i = 0; i < lenbytes; i++) {
            size_t sh = big ? (lenbytes - 1 - i) * 8 : i * 8;
            tail.push_back(sh < 64 ? static_cast<char>((bits >> sh) & 0xFF) : 0);
        }
        for (size_t o = 0; o < tail.size(); o += B) block_(s, reinterpret_cast<const unsigned char*>(tail.data()) + o);
        std::string out;
        if (s.kind == 3) {
            for (int k = 0; k < 8; k++)
                for (int i = 7; i >= 0; i--) out.push_back(static_cast<char>((s.h64[k] >> (8 * i)) & 0xFF));
        } else {
            const int words = s.kind == 0 ? 4 : s.kind == 1 ? 5 : 8;
            for (int k = 0; k < words; k++) {
                if (s.kind == 0) for (int i = 0; i < 4; i++) out.push_back(static_cast<char>((s.h32[k] >> (8 * i)) & 0xFF));
                else for (int i = 3; i >= 0; i--) out.push_back(static_cast<char>((s.h32[k] >> (8 * i)) & 0xFF));
            }
        }
        return out;
    }

  public:
    Hash() = default;
    Hash(int kind, const std::string& data) : s_(std::make_shared<S>()) {
        s_->kind = kind;
        init_(*s_);
        feed_(*s_, data.data(), data.size());
    }
    void update(const bytes& b) const { feed_(*s_, b.raw().data(), b.raw().size()); }
    bytes digest() const { return bytes(finish_(*s_)); }
    str hexdigest() const { return digest().hex(); }
};
inline Hash md5(const bytes& b = bytes()) { return Hash(0, b.raw()); }
inline Hash sha1(const bytes& b = bytes()) { return Hash(1, b.raw()); }
inline Hash sha256(const bytes& b = bytes()) { return Hash(2, b.raw()); }
inline Hash sha512(const bytes& b = bytes()) { return Hash(3, b.raw()); }

// hashlib.blake2b(data=b"", *, digest_size=64, key=b""): RFC 7693 (no salt / person / tree parameters)
class blake2b {
    struct S {
        uint64_t h[8];
        uint64_t t = 0;          // bytes compressed so far
        std::string pending;     // the unprocessed tail (kept until more data arrives, as the last block is flagged)
        int64_t digest_size = 64;
    };
    std::shared_ptr<S> s_ = std::make_shared<S>();

    static const uint64_t* iv_() {
        static const uint64_t v[8] = {0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL, 0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL,
                                      0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL, 0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL};
        return v;
    }
    static void compress_(uint64_t h[8], const unsigned char* blk, uint64_t t, bool last) {
        static const unsigned char sigma[12][16] = {
            {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}, {14, 10, 4, 8, 9, 15, 13, 6, 1, 12, 0, 2, 11, 7, 5, 3},
            {11, 8, 12, 0, 5, 2, 15, 13, 10, 14, 3, 6, 7, 1, 9, 4}, {7, 9, 3, 1, 13, 12, 11, 14, 2, 6, 5, 10, 4, 0, 15, 8},
            {9, 0, 5, 7, 2, 4, 10, 15, 14, 1, 11, 12, 6, 8, 3, 13}, {2, 12, 6, 10, 0, 11, 8, 3, 4, 13, 7, 5, 15, 14, 1, 9},
            {12, 5, 1, 15, 14, 13, 4, 10, 0, 7, 6, 3, 9, 2, 8, 11}, {13, 11, 7, 14, 12, 1, 3, 9, 5, 0, 15, 4, 8, 6, 2, 10},
            {6, 15, 14, 9, 11, 3, 0, 8, 12, 2, 13, 7, 1, 4, 10, 5}, {10, 2, 8, 4, 7, 6, 1, 5, 15, 11, 9, 14, 3, 12, 13, 0},
            {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}, {14, 10, 4, 8, 9, 15, 13, 6, 1, 12, 0, 2, 11, 7, 5, 3}};
        auto rotr = [](uint64_t x, int n) { return (x >> n) | (x << (64 - n)); };
        uint64_t m[16], v[16];
        for (int i = 0; i < 16; i++) {
            m[i] = 0;
            for (int k = 7; k >= 0; k--) m[i] = (m[i] << 8) | blk[i * 8 + k];
        }
        for (int i = 0; i < 8; i++) {
            v[i] = h[i];
            v[i + 8] = iv_()[i];
        }
        v[12] ^= t;
        if (last) v[14] = ~v[14];
        auto G = [&](int a, int b, int c, int d, uint64_t x, uint64_t y) {
            v[a] = v[a] + v[b] + x;
            v[d] = rotr(v[d] ^ v[a], 32);
            v[c] = v[c] + v[d];
            v[b] = rotr(v[b] ^ v[c], 24);
            v[a] = v[a] + v[b] + y;
            v[d] = rotr(v[d] ^ v[a], 16);
            v[c] = v[c] + v[d];
            v[b] = rotr(v[b] ^ v[c], 63);
        };
        for (int r = 0; r < 12; r++) {
            const unsigned char* q = sigma[r];
            G(0, 4, 8, 12, m[q[0]], m[q[1]]);
            G(1, 5, 9, 13, m[q[2]], m[q[3]]);
            G(2, 6, 10, 14, m[q[4]], m[q[5]]);
            G(3, 7, 11, 15, m[q[6]], m[q[7]]);
            G(0, 5, 10, 15, m[q[8]], m[q[9]]);
            G(1, 6, 11, 12, m[q[10]], m[q[11]]);
            G(2, 7, 8, 13, m[q[12]], m[q[13]]);
            G(3, 4, 9, 14, m[q[14]], m[q[15]]);
        }
        for (int i = 0; i < 8; i++) h[i] ^= v[i] ^ v[i + 8];
    }
    static void feed_(S& s, const char* d, size_t n) {
        // a full pending block is compressed only once more data follows it: the last block carries the final flag
        auto flush_full = [&]() {
            s.t += 128;
            compress_(s.h, reinterpret_cast<const unsigned char*>(s.pending.data()), s.t, false);
            s.pending.clear();
        };
        while (n) {
            if (s.pending.size() == 128) flush_full();
            const size_t take = std::min(n, 128 - s.pending.size());
            s.pending.append(d, take);
            d += take;
            n -= take;
        }
    }

  public:
    explicit blake2b(const bytes& data = bytes(), int64_t digest_size = 64, const bytes& key = bytes()) {
        if (digest_size < 1 || digest_size > 64) throw ValueError("digest_size for Blake2b must be between 1 and 64 bytes, here it is " + std::to_string(digest_size));
        if (key.raw().size() > 64) throw ValueError("maximum key length is 64 bytes");
        s_->digest_size = digest_size;
        for (int i = 0; i < 8; i++) s_->h[i] = iv_()[i];
        s_->h[0] ^= 0x01010000ULL ^ (static_cast<uint64_t>(key.raw().size()) << 8) ^ static_cast<uint64_t>(digest_size);
        if (!key.raw().empty()) {
            std::string k = key.raw();
            k.resize(128, '\0');
            feed_(*s_, k.data(), k.size());
        }
        feed_(*s_, data.raw().data(), data.raw().size());
    }
    void update(const bytes& b) const { feed_(*s_, b.raw().data(), b.raw().size()); }
    blake2b copy() const {
        blake2b c;
        *c.s_ = *s_;
        return c;
    }
    bytes digest() const {
        S s = *s_;   // (a copy: the digest does not end the hash)
        s.t += s.pending.size();
        std::string blk = s.pending;
        blk.resize(128, '\0');
        compress_(s.h, reinterpret_cast<const unsigned char*>(blk.data()), s.t, true);
        std::string out(static_cast<size_t>(s.digest_size), '\0');
        for (int64_t i = 0; i < s.digest_size; i++) out[static_cast<size_t>(i)] = static_cast<char>((s.h[i / 8] >> (8 * (i % 8))) & 0xFF);
        return bytes(out);
    }
    str hexdigest() const { return digest().hex(); }
    int64_t digest_size() const { return s_->digest_size; }
    int64_t block_size() const { return 128; }
    str name() const { return str("blake2b"); }
};
}  // namespace hashlib
#if defined(_MSC_VER) && !defined(__clang__)
#pragma optimize("", on)
#endif

// ------------------------------------------------------------------------------------------ generators
// a generator function makes a class deriving generator<T>: next() resumes after each yield and gives
// nothing at the end; its call is an iter<T>, which for-loops and list() consume
template <class T> struct generator : object {
    virtual std::optional<T> next() = 0;
    virtual ~generator() {}
};
template <class T> class iter {
    std::shared_ptr<generator<T>> g_;

  public:
    using value_type = T;
    iter() = default;
    template <class G> iter(std::shared_ptr<G> g) : g_(std::move(g)) {}
    struct it {
        generator<T>* g;
        std::optional<T> cur;
        it& operator++() {
            cur = g->next();
            return *this;
        }
        const T& operator*() const { return *cur; }
        bool operator!=(const it&) const { return cur.has_value(); }
    };
    it begin() const {
        it i{g_.get(), std::nullopt};
        if (g_) i.cur = g_->next();
        return i;
    }
    it end() const { return {nullptr, std::nullopt}; }
    std::optional<T> next() const { return g_->next(); }
};

// ------------------------------------------------------------------------------------------ variants
// py.variant<A, B ...>: where scalars of several kinds meet; its tags are its alternatives
template <class... A> using variant = std::variant<A...>;
template <class... A> bool truthy(const std::variant<A...>& v) {
    return std::visit([](const auto& x) { return truthy(x); }, v);
}
template <class... A> str to_str(const std::variant<A...>& v) {
    return std::visit([](const auto& x) { return to_str(x); }, v);
}
template <class... A> str repr(const std::variant<A...>& v) {
    return std::visit([](const auto& x) { return repr(x); }, v);
}
template <class T, class... A> T variant_get_(const std::variant<A...>& v) {
    if (!std::holds_alternative<T>(v)) throw TypeError("variant does not hold this kind");
    return std::get<T>(v);
}

}  // namespace py

// ------------------------------------------------------------------------------------------ re
#if __has_include("pyre.hpp")
#include "pyre.hpp"
namespace py {
namespace re {
// a Match: Python's positions are code points; pyre's are UTF-8 bytes
class Match {
    ::re::Match m_;
    const std::vector<std::pair<std::string, int>>* names_ = nullptr;   // (?P<name>...): name, group (the compiled pattern's, kept by pyre's cache)
    // (constant time: pyre keeps a table of code-point counts for the subject, built once and shared by its matches)
    int64_t cp_(long byte) const { return m_.cp_offset(byte); }

  public:
    Match() = default;
    explicit Match(::re::Match m, const std::vector<std::pair<std::string, int>>* names = nullptr) : m_(std::move(m)), names_(names) {}
    explicit operator bool() const { return m_.ok; }
    // the group a name or number stands for: IndexError "no such group" as Python
    int index_(const str& name) const {
        if (names_)
            for (const auto& kv : *names_)
                if (kv.first == name.utf8()) return kv.second;
        throw IndexError("no such group");
    }
    str group(const str& name) const { return piece_(index_(name)); }   // m.group("name")
    std::optional<str> group_opt(const str& name) const { return group_opt(static_cast<int64_t>(index_(name))); }
    int64_t start(const str& name) const { return start(static_cast<int64_t>(index_(name))); }
    int64_t end(const str& name) const { return end(static_cast<int64_t>(index_(name))); }
    // m.string: the text the match was made on
    str string() const { return str::from_utf8(m_.s->data(), m_.s->size()); }
    // m.groupdict(): {name: text or None}, in the order the groups are named
    dict<str, std::optional<str>> groupdict() const {
        dict<str, std::optional<str>> out;
        if (names_)
            for (const auto& kv : *names_) out[str(kv.first)] = group_opt(static_cast<int64_t>(kv.second));
        return out;
    }
    // m.lastindex / m.lastgroup are not kept by pyre
    bool empty() const { return !m_.ok; }
    str piece_(int g) const {   // (the text of a group, decoded from the subject in place; '' where it did not take part)
        if (!m_.has(g)) return str();
        return str::from_utf8(m_.s->data() + m_.start(g), static_cast<size_t>(m_.end(g) - m_.start(g)));
    }
    str group(int64_t g = 0) const {   // (a group the pattern always sets)
        if (g < 0 || g > m_.ngroups()) throw IndexError("no such group");
        return piece_(static_cast<int>(g));
    }
    std::optional<str> group_opt(int64_t g = 0) const {
        if (g < 0 || g > m_.ngroups()) throw IndexError("no such group");
        if (!m_.has(static_cast<int>(g))) return std::nullopt;
        return piece_(static_cast<int>(g));
    }
    int64_t start(int64_t g = 0) const { return cp_(m_.start(static_cast<int>(g))); }
    int64_t end(int64_t g = 0) const { return cp_(m_.end(static_cast<int>(g))); }
    std::tuple<int64_t, int64_t> span(int64_t g = 0) const { return {start(g), end(g)}; }
    list<std::optional<str>> groups_opt() const {
        list<std::optional<str>> out;
        for (int g = 1; g <= m_.ngroups(); g++) out.append(group_opt(g));
        return out;
    }
    const ::re::Match& raw() const { return m_; }
};
inline bool truthy(const Match& m) { return static_cast<bool>(m); }
inline std::optional<Match> opt_(::re::Match m, const std::vector<std::pair<std::string, int>>* names = nullptr) {
    if (!m) return std::nullopt;
    return Match(std::move(m), names);
}
inline int flags_(int64_t f) {
    int out = 0;
    if (f & 2) out |= ::re::I;
    if (f & 8) out |= ::re::M;
    if (f & 16) out |= ::re::S;
    if (f & 256) out |= ::re::A;
    return out;
}
// a compiled pattern: the regex, compiled once (pyre's own cache, per thread in front), and its text
class Pattern {
    const ::re::Regex* r_ = nullptr;

  public:
    Pattern() = default;
    Pattern(const str& p, int64_t flags) : r_(rx_(p, flags)) {}
    Pattern(const char* p, int64_t flags) : r_(pyre_([&] { return &::re::rx(p, std::strlen(p), flags_(flags)); })) {}
    // pyre's errors as Python's: re.error is PatternError
    template <class F> static auto pyre_(F f) -> decltype(f()) {
        try {
            return f();
        } catch (const ::re::Error& e) {
            if (e.kind == "PatternError") throw PatternError(e.what());
            if (e.kind == "IndexError") throw IndexError(e.what());
            if (e.kind == "OverflowError") throw OverflowError(e.what());
            throw ValueError(e.what());
        }
    }
    static const ::re::Regex* rx_(const str& p, int64_t flags) {
        auto v = p.utf8_view();
        return pyre_([&] { return &::re::rx(v->s.data(), v->s.size(), flags_(flags)); });
    }
    const ::re::Regex& rx() const { return *r_; }
    str pattern() const { return str(r_->pattern()); }
    int64_t groups() const { return r_->groups(); }
    // the subject: the str's UTF-8 form (made once per str) and the code-point index shared by the matches made on it
    static ::re::Subject subj_(const str& s) {
        auto v = s.utf8_view();
        std::shared_ptr<const std::string> text(v, &v->s);       // (shares the view's lifetime: no copy of the text)
        if (v->ascii) return ::re::Subject{text, nullptr, true};   // (offsets need no table)
        std::shared_ptr<void> x = std::atomic_load(&v->aux);
        if (!x) {
            std::shared_ptr<void> fresh = ::re::new_index(false);
            std::shared_ptr<void> none;
            if (std::atomic_compare_exchange_strong(&v->aux, &none, fresh)) x = fresh;
            else x = none;
        }
        return ::re::Subject{text, std::static_pointer_cast<::re::CpIndex>(x), false};
    }
    // (pos / endpos are code points, as Python's; pyre takes UTF-8 offsets)
    static size_t at_(const ::re::Subject& u, int64_t cp, bool ascii) {
        if (cp <= 0) return 0;
        if (ascii) return static_cast<size_t>(cp) < u.s->size() ? static_cast<size_t>(cp) : u.s->size();
        int64_t n = 0;
        for (size_t i = 0; i < u.s->size(); i++)
            if ((static_cast<unsigned char>((*u.s)[i]) & 0xC0) != 0x80 && n++ == cp) return i;
        return u.s->size();
    }
    static size_t end_(const ::re::Subject& u, int64_t cp, bool ascii) { return cp == INT64_MAX ? std::string::npos : at_(u, cp, ascii); }
    template <class F> static std::optional<Match> at1_(const str& s, int64_t pos, int64_t endpos, F f, const std::vector<std::pair<std::string, int>>* names) {
        auto u = subj_(s);
        return opt_(f(u, at_(u, pos, u.ascii), end_(u, endpos, u.ascii)), names);
    }
    std::optional<Match> match(const str& s, int64_t pos = 0, int64_t endpos = INT64_MAX) const {
        return at1_(s, pos, endpos, [&](const ::re::Subject& u, size_t a, size_t b) { return r_->match_in(u, a, b); }, &r_->names());
    }
    std::optional<Match> fullmatch(const str& s, int64_t pos = 0, int64_t endpos = INT64_MAX) const {
        return at1_(s, pos, endpos, [&](const ::re::Subject& u, size_t a, size_t b) { return r_->fullmatch_in(u, a, b); }, &r_->names());
    }
    std::optional<Match> search(const str& s, int64_t pos = 0, int64_t endpos = INT64_MAX) const {
        return at1_(s, pos, endpos, [&](const ::re::Subject& u, size_t a, size_t b) { return r_->search_in(u, a, b); }, &r_->names());
    }
    list<Match> finditer(const str& s) const {
        list<Match> out;
        for (auto& m : r_->finditer_in(subj_(s))) out.append(Match(m, &r_->names()));
        return out;
    }
    list<str> findall(const str& s) const {
        list<str> out;
        auto u = subj_(s);
        ::re::Match m;
        size_t pos = 0;
        bool adv = false;
        int g = r_->groups() >= 1 ? 1 : 0;
        while (r_->next_in(m, u, pos, adv))
            out.append(m.has(g) ? str::from_utf8(u.s->data() + m.start(g), static_cast<size_t>(m.end(g) - m.start(g))) : str());
        return out;
    }
    template <class... T> list<std::tuple<T...>> findall_groups(const str& s) const {
        list<std::tuple<T...>> out;
        for (auto& g : r_->findall_groups_in(subj_(s))) {
            std::tuple<T...> t;
            size_t i = 0;
            std::apply([&](auto&... x) { ((x = str(g[i++])), ...); }, t);
            out.append(t);
        }
        return out;
    }
    // (nothing replaced: the text itself, not a re-decoded copy)
    str sub_(const str& repl, const str& s, int64_t count, int64_t* n) const {
        int k = 0;
        std::string o = pyre_([&] { return r_->sub_in(repl.utf8(), subj_(s), static_cast<int>(count), &k); });
        if (n) *n = k;
        return k == 0 ? s : str(o);
    }
    str sub(const str& repl, const str& s, int64_t count = 0) const { return sub_(repl, s, count, nullptr); }
    template <class F> str sub_fn_(const F& fn, const str& s, int64_t count, int64_t* n) const {
        int k = 0;
        std::string o = r_->sub_fn_in([&](const ::re::Match& m) { return fn(Match(m, &r_->names())).utf8(); }, subj_(s), static_cast<int>(count), &k);
        if (n) *n = k;
        return k == 0 ? s : str(o);
    }
    template <class F> str sub_fn(const F& fn, const str& s, int64_t count = 0) const { return sub_fn_(fn, s, count, nullptr); }
    // (new) Pattern.sub(fn, s): the replacement a function of the match
    template <class F, class = decltype(std::declval<const F&>()(std::declval<const Match&>()))>
    str sub(const F& fn, const str& s, int64_t count = 0) const { return sub_fn(fn, s, count); }
    template <class F, class = decltype(std::declval<const F&>()(std::declval<const Match&>()))>
    std::tuple<str, int64_t> subn(const F& fn, const str& s, int64_t count = 0) const {
        int64_t n = 0;
        str r = sub_fn_(fn, s, count, &n);
        return {r, n};
    }
    std::tuple<str, int64_t> subn(const str& repl, const str& s, int64_t count = 0) const {
        int64_t n = 0;
        str r = sub_(repl, s, count, &n);
        return {r, n};
    }
    list<str> split(const str& s, int64_t maxsplit = 0) const {
        list<str> out;
        auto u = subj_(s);
        const std::string& t = *u.s;
        size_t at = 0;
        int64_t n = 0;
        ::re::Match m;
        size_t pos = 0;
        bool adv = false;
        while ((maxsplit <= 0 || n < maxsplit) && r_->next_in(m, u, pos, adv)) {
            out.append(str::from_utf8(t.data() + at, static_cast<size_t>(m.start()) - at));
            for (int g = 1; g <= m.ngroups(); g++)
                out.append(m.has(g) ? str::from_utf8(t.data() + m.start(g), static_cast<size_t>(m.end(g) - m.start(g))) : str());
            at = static_cast<size_t>(m.end());
            n++;
        }
        out.append(str::from_utf8(t.data() + at, t.size() - at));
        return out;
    }
};
inline Pattern compile(const str& p, int64_t flags = 0) { return Pattern(p, flags); }
inline Pattern compile(const char* p, int64_t flags = 0) { return Pattern(p, flags); }
inline std::optional<Match> match(const str& p, const str& s, int64_t flags = 0) { return Pattern(p, flags).match(s); }
inline std::optional<Match> match(const char* p, const str& s, int64_t flags = 0) { return Pattern(p, flags).match(s); }
inline std::optional<Match> fullmatch(const str& p, const str& s, int64_t flags = 0) { return Pattern(p, flags).fullmatch(s); }
inline std::optional<Match> fullmatch(const char* p, const str& s, int64_t flags = 0) { return Pattern(p, flags).fullmatch(s); }
inline std::optional<Match> search(const str& p, const str& s, int64_t flags = 0) { return Pattern(p, flags).search(s); }
inline std::optional<Match> search(const char* p, const str& s, int64_t flags = 0) { return Pattern(p, flags).search(s); }
inline list<Match> finditer(const str& p, const str& s, int64_t flags = 0) { return Pattern(p, flags).finditer(s); }
inline list<Match> finditer(const char* p, const str& s, int64_t flags = 0) { return Pattern(p, flags).finditer(s); }
inline list<str> findall(const str& p, const str& s, int64_t flags = 0) { return Pattern(p, flags).findall(s); }
inline list<str> findall(const char* p, const str& s, int64_t flags = 0) { return Pattern(p, flags).findall(s); }
inline str sub(const str& p, const str& repl, const str& s, int64_t count = 0, int64_t flags = 0) {
    return Pattern(p, flags).sub(repl, s, count);
}
inline str sub(const char* p, const str& repl, const str& s, int64_t count = 0, int64_t flags = 0) {
    return Pattern(p, flags).sub(repl, s, count);
}
template <class F, class = decltype(std::declval<const F&>()(std::declval<const Match&>()))>
str sub(const str& p, const F& fn, const str& s, int64_t count = 0, int64_t flags = 0) {
    return Pattern(p, flags).sub_fn(fn, s, count);
}
inline std::tuple<str, int64_t> subn(const str& p, const str& repl, const str& s, int64_t count = 0, int64_t flags = 0) {
    return Pattern(p, flags).subn(repl, s, count);
}
template <class F, class = decltype(std::declval<const F&>()(std::declval<const Match&>()))>
std::tuple<str, int64_t> subn(const str& p, const F& fn, const str& s, int64_t count = 0, int64_t flags = 0) {
    return Pattern(p, flags).subn(fn, s, count);
}
inline list<str> split(const str& p, const str& s, int64_t maxsplit = 0, int64_t flags = 0) {
    return Pattern(p, flags).split(s, maxsplit);
}
inline list<str> split(const char* p, const str& s, int64_t maxsplit = 0, int64_t flags = 0) {
    return Pattern(p, flags).split(s, maxsplit);
}
inline str escape(const str& s) { return str(::re::escape(s.utf8())); }
constexpr int64_t I = 2, IGNORECASE = 2, M = 8, MULTILINE = 8, S = 16, DOTALL = 16, X = 64, VERBOSE = 64, A = 256, ASCII = 256;
}  // namespace re
}  // namespace py
#endif
