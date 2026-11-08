// py_os.hpp -- the porting engine's library rows over py.hpp (included at its end): open() in every mode
// and the file object, os / os.path / shutil / time, hashlib, re (over native/l2's pyre, when its header
// is on the include path), tagged variants. Each behaves as CPython 3 on Windows does, error for error.
#pragma once
#include <array>
#include <chrono>
#include <cstdlib>
#include <thread>
#include <variant>

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
        char tmp[65536];
        size_t n;
        while ((n = std::fread(tmp, 1, sizeof tmp, h().f)) > 0) s.append(tmp, n);
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
        std::string s(static_cast<size_t>(n), '\0');
        size_t got = std::fread(&s[0], 1, s.size(), h().f);
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
namespace os {
// (CPython's mapping of the system's error codes to its OSError subclasses)
[[noreturn]] inline void raise_os_(const std::error_code& ec, const str& path) {
    std::string msg = "[WinError " + std::to_string(ec.value()) + "] " + ec.message() + ": " + repr(path).utf8();
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
inline std::optional<str> get(const str& k) {
#ifdef _WIN32
    std::wstring w = fs_::u8path(k.utf8()).wstring();
    const wchar_t* v = _wgetenv(w.c_str());
    if (!v) return std::nullopt;
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
}  // namespace environ_
inline std::optional<str> getenv(const str& k) { return environ_::get(k); }
inline str getenv(const str& k, const str& dflt) { return environ_::get(k, dflt); }

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

inline void makedirs(const str& p, int64_t mode = 0777, bool exist_ok = false) {
    (void)mode;
    nul_check_("mkdir", p);
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
inline void copyfile(const str& a, const str& b) {
    std::error_code ec;
    if (!fs_::exists(fspath_(a), ec)) throw FileNotFoundError("[Errno 2] No such file or directory: " + repr(a).utf8());
    fs_::copy_file(fspath_(a), fspath_(b), fs_::copy_options::overwrite_existing, ec);
    if (ec) throw PermissionError("[Errno 13] Permission denied: " + repr(b).utf8());
}
inline str copy(const str& a, const str& b) {
    str dst = os::path::isdir(b) ? os::path::join(b, os::path::basename(a)) : b;
    copyfile(a, dst);
    return dst;
}
inline str copy2(const str& a, const str& b) { return copy(a, b); }
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
}  // namespace time

// ------------------------------------------------------------------------------------------ hashlib
namespace hashlib {
class Hash {
    struct S {
        int kind;   // 0 md5, 1 sha1, 2 sha256, 3 sha512
        std::string data;
    };
    std::shared_ptr<S> s_;
    static uint32_t rol(uint32_t x, int n) { return (x << n) | (x >> (32 - n)); }
    static uint32_t ror(uint32_t x, int n) { return (x >> n) | (x << (32 - n)); }
    static uint64_t ror64(uint64_t x, int n) { return (x >> n) | (x << (64 - n)); }
    static std::string pad_(const std::string& m, size_t block, bool big, size_t lenbytes) {
        std::string p = m;
        p.push_back(static_cast<char>(0x80));
        while (p.size() % block != block - lenbytes) p.push_back(0);
        uint64_t bits = static_cast<uint64_t>(m.size()) * 8;
        for (size_t i = 0; i < lenbytes; i++) {
            size_t sh = big ? (lenbytes - 1 - i) * 8 : i * 8;
            p.push_back(sh < 64 ? static_cast<char>((bits >> sh) & 0xFF) : 0);
        }
        return p;
    }
    static std::string md5_(const std::string& m) {
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
        uint32_t h[4] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476};
        std::string p = pad_(m, 64, false, 8);
        for (size_t o = 0; o < p.size(); o += 64) {
            uint32_t w[16];
            for (int i = 0; i < 16; i++)
                w[i] = static_cast<uint32_t>(static_cast<unsigned char>(p[o + 4 * i])) |
                       static_cast<uint32_t>(static_cast<unsigned char>(p[o + 4 * i + 1])) << 8 |
                       static_cast<uint32_t>(static_cast<unsigned char>(p[o + 4 * i + 2])) << 16 |
                       static_cast<uint32_t>(static_cast<unsigned char>(p[o + 4 * i + 3])) << 24;
            uint32_t a = h[0], b = h[1], c = h[2], d = h[3];
            for (int i = 0; i < 64; i++) {
                uint32_t f;
                int g;
                if (i < 16) f = (b & c) | (~b & d), g = i;
                else if (i < 32) f = (d & b) | (~d & c), g = (5 * i + 1) % 16;
                else if (i < 48) f = b ^ c ^ d, g = (3 * i + 5) % 16;
                else f = c ^ (b | ~d), g = (7 * i) % 16;
                uint32_t t = d;
                d = c;
                c = b;
                b = b + rol(a + f + K[i] + w[g], R[i]);
                a = t;
            }
            h[0] += a, h[1] += b, h[2] += c, h[3] += d;
        }
        std::string out;
        for (uint32_t x : h)
            for (int i = 0; i < 4; i++) out.push_back(static_cast<char>((x >> (8 * i)) & 0xFF));
        return out;
    }
    static std::string sha1_(const std::string& m) {
        uint32_t h[5] = {0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0};
        std::string p = pad_(m, 64, true, 8);
        for (size_t o = 0; o < p.size(); o += 64) {
            uint32_t w[80];
            for (int i = 0; i < 16; i++)
                w[i] = static_cast<uint32_t>(static_cast<unsigned char>(p[o + 4 * i])) << 24 |
                       static_cast<uint32_t>(static_cast<unsigned char>(p[o + 4 * i + 1])) << 16 |
                       static_cast<uint32_t>(static_cast<unsigned char>(p[o + 4 * i + 2])) << 8 |
                       static_cast<uint32_t>(static_cast<unsigned char>(p[o + 4 * i + 3]));
            for (int i = 16; i < 80; i++) w[i] = rol(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);
            uint32_t a = h[0], b = h[1], c = h[2], d = h[3], e = h[4];
            for (int i = 0; i < 80; i++) {
                uint32_t f, k;
                if (i < 20) f = (b & c) | (~b & d), k = 0x5A827999;
                else if (i < 40) f = b ^ c ^ d, k = 0x6ED9EBA1;
                else if (i < 60) f = (b & c) | (b & d) | (c & d), k = 0x8F1BBCDC;
                else f = b ^ c ^ d, k = 0xCA62C1D6;
                uint32_t t = rol(a, 5) + f + e + k + w[i];
                e = d, d = c, c = rol(b, 30), b = a, a = t;
            }
            h[0] += a, h[1] += b, h[2] += c, h[3] += d, h[4] += e;
        }
        std::string out;
        for (uint32_t x : h)
            for (int i = 3; i >= 0; i--) out.push_back(static_cast<char>((x >> (8 * i)) & 0xFF));
        return out;
    }
    static std::string sha256_(const std::string& m) {
        static const uint32_t K[64] = {
            0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01,
            0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
            0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da, 0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147,
            0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
            0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070, 0x19a4c116, 0x1e376c08,
            0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
            0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};
        uint32_t h[8] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};
        std::string p = pad_(m, 64, true, 8);
        for (size_t o = 0; o < p.size(); o += 64) {
            uint32_t w[64];
            for (int i = 0; i < 16; i++)
                w[i] = static_cast<uint32_t>(static_cast<unsigned char>(p[o + 4 * i])) << 24 |
                       static_cast<uint32_t>(static_cast<unsigned char>(p[o + 4 * i + 1])) << 16 |
                       static_cast<uint32_t>(static_cast<unsigned char>(p[o + 4 * i + 2])) << 8 |
                       static_cast<uint32_t>(static_cast<unsigned char>(p[o + 4 * i + 3]));
            for (int i = 16; i < 64; i++) {
                uint32_t s0 = ror(w[i - 15], 7) ^ ror(w[i - 15], 18) ^ (w[i - 15] >> 3);
                uint32_t s1 = ror(w[i - 2], 17) ^ ror(w[i - 2], 19) ^ (w[i - 2] >> 10);
                w[i] = w[i - 16] + s0 + w[i - 7] + s1;
            }
            uint32_t a = h[0], b = h[1], c = h[2], d = h[3], e = h[4], f = h[5], g = h[6], hh = h[7];
            for (int i = 0; i < 64; i++) {
                uint32_t t1 = hh + (ror(e, 6) ^ ror(e, 11) ^ ror(e, 25)) + ((e & f) ^ (~e & g)) + K[i] + w[i];
                uint32_t t2 = (ror(a, 2) ^ ror(a, 13) ^ ror(a, 22)) + ((a & b) ^ (a & c) ^ (b & c));
                hh = g, g = f, f = e, e = d + t1, d = c, c = b, b = a, a = t1 + t2;
            }
            h[0] += a, h[1] += b, h[2] += c, h[3] += d, h[4] += e, h[5] += f, h[6] += g, h[7] += hh;
        }
        std::string out;
        for (uint32_t x : h)
            for (int i = 3; i >= 0; i--) out.push_back(static_cast<char>((x >> (8 * i)) & 0xFF));
        return out;
    }
    static std::string sha512_(const std::string& m) {
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
        uint64_t h[8] = {0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL, 0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL,
                         0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL, 0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL};
        std::string p = pad_(m, 128, true, 16);
        for (size_t o = 0; o < p.size(); o += 128) {
            uint64_t w[80];
            for (int i = 0; i < 16; i++) {
                w[i] = 0;
                for (int k = 0; k < 8; k++) w[i] = (w[i] << 8) | static_cast<unsigned char>(p[o + 8 * i + k]);
            }
            for (int i = 16; i < 80; i++) {
                uint64_t s0 = ror64(w[i - 15], 1) ^ ror64(w[i - 15], 8) ^ (w[i - 15] >> 7);
                uint64_t s1 = ror64(w[i - 2], 19) ^ ror64(w[i - 2], 61) ^ (w[i - 2] >> 6);
                w[i] = w[i - 16] + s0 + w[i - 7] + s1;
            }
            uint64_t a = h[0], b = h[1], c = h[2], d = h[3], e = h[4], f = h[5], g = h[6], hh = h[7];
            for (int i = 0; i < 80; i++) {
                uint64_t t1 = hh + (ror64(e, 14) ^ ror64(e, 18) ^ ror64(e, 41)) + ((e & f) ^ (~e & g)) + K[i] + w[i];
                uint64_t t2 = (ror64(a, 28) ^ ror64(a, 34) ^ ror64(a, 39)) + ((a & b) ^ (a & c) ^ (b & c));
                hh = g, g = f, f = e, e = d + t1, d = c, c = b, b = a, a = t1 + t2;
            }
            h[0] += a, h[1] += b, h[2] += c, h[3] += d, h[4] += e, h[5] += f, h[6] += g, h[7] += hh;
        }
        std::string out;
        for (uint64_t x : h)
            for (int i = 7; i >= 0; i--) out.push_back(static_cast<char>((x >> (8 * i)) & 0xFF));
        return out;
    }

  public:
    Hash() = default;
    Hash(int kind, const std::string& data) : s_(std::make_shared<S>(S{kind, data})) {}
    void update(const bytes& b) const { s_->data += b.raw(); }
    bytes digest() const {
        switch (s_->kind) {
            case 0: return bytes(md5_(s_->data));
            case 1: return bytes(sha1_(s_->data));
            case 2: return bytes(sha256_(s_->data));
            default: return bytes(sha512_(s_->data));
        }
    }
    str hexdigest() const { return digest().hex(); }
};
inline Hash md5(const bytes& b = bytes()) { return Hash(0, b.raw()); }
inline Hash sha1(const bytes& b = bytes()) { return Hash(1, b.raw()); }
inline Hash sha256(const bytes& b = bytes()) { return Hash(2, b.raw()); }
inline Hash sha512(const bytes& b = bytes()) { return Hash(3, b.raw()); }
}  // namespace hashlib

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
    std::vector<std::string> names_;
    int64_t cp_(long byte) const {
        if (byte < 0) return -1;
        int64_t n = 0;
        const std::string& s = *m_.s;
        for (long i = 0; i < byte && i < static_cast<long>(s.size()); i++)
            if ((static_cast<unsigned char>(s[i]) & 0xC0) != 0x80) n++;
        return n;
    }

  public:
    Match() = default;
    explicit Match(::re::Match m) : m_(std::move(m)) {}
    explicit operator bool() const { return m_.ok; }
    bool empty() const { return !m_.ok; }
    str group(int64_t g = 0) const {   // (a group the pattern always sets)
        if (g < 0 || g > m_.ngroups()) throw IndexError("no such group");
        return str(m_.group(static_cast<int>(g)));
    }
    std::optional<str> group_opt(int64_t g = 0) const {
        if (g < 0 || g > m_.ngroups()) throw IndexError("no such group");
        if (!m_.has(static_cast<int>(g))) return std::nullopt;
        return str(m_.group(static_cast<int>(g)));
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
inline std::optional<Match> opt_(::re::Match m) {
    if (!m) return std::nullopt;
    return Match(std::move(m));
}
inline int flags_(int64_t f) {
    int out = 0;
    if (f & 2) out |= ::re::I;
    if (f & 8) out |= ::re::M;
    if (f & 16) out |= ::re::S;
    return out;
}
// a compiled pattern: the regex, compiled once (pyre's own cache), and its text
class Pattern {
    const ::re::Regex* r_ = nullptr;

  public:
    Pattern() = default;
    Pattern(const str& p, int64_t flags) : r_(rx_(p, flags)) {}
    // pyre's errors as Python's: re.error is PatternError
    template <class F> static auto pyre_(F f) -> decltype(f()) {
        try {
            return f();
        } catch (const ::re::Error& e) {
            if (e.kind == "PatternError") throw PatternError(e.what());
            if (e.kind == "IndexError") throw IndexError(e.what());
            throw ValueError(e.what());
        }
    }
    static const ::re::Regex* rx_(const str& p, int64_t flags) { return pyre_([&] { return &::re::rx(p.utf8(), flags_(flags)); }); }
    const ::re::Regex& rx() const { return *r_; }
    str pattern() const { return str(r_->pattern()); }
    int64_t groups() const { return r_->groups(); }
    // (pos / endpos are code points, as Python's; pyre takes UTF-8 offsets)
    static size_t at_(const std::string& u, int64_t cp) {
        if (cp <= 0) return 0;
        int64_t n = 0;
        for (size_t i = 0; i < u.size(); i++)
            if ((static_cast<unsigned char>(u[i]) & 0xC0) != 0x80 && n++ == cp) return i;
        return u.size();
    }
    static size_t end_(const std::string& u, int64_t cp) { return cp == INT64_MAX ? std::string::npos : at_(u, cp); }
    std::optional<Match> match(const str& s, int64_t pos = 0, int64_t endpos = INT64_MAX) const {
        std::string u = s.utf8();
        return opt_(r_->match(u, at_(u, pos), end_(u, endpos)));
    }
    std::optional<Match> fullmatch(const str& s, int64_t pos = 0, int64_t endpos = INT64_MAX) const {
        std::string u = s.utf8();
        return opt_(r_->fullmatch(u, at_(u, pos), end_(u, endpos)));
    }
    std::optional<Match> search(const str& s, int64_t pos = 0, int64_t endpos = INT64_MAX) const {
        std::string u = s.utf8();
        return opt_(r_->search(u, at_(u, pos), end_(u, endpos)));
    }
    list<Match> finditer(const str& s) const {
        list<Match> out;
        for (auto& m : r_->finditer(s.utf8())) out.append(Match(m));
        return out;
    }
    list<str> findall(const str& s) const {
        list<str> out;
        for (auto& x : r_->findall(s.utf8())) out.append(str(x));
        return out;
    }
    template <class... T> list<std::tuple<T...>> findall_groups(const str& s) const {
        list<std::tuple<T...>> out;
        for (auto& g : r_->findall_groups(s.utf8())) {
            std::tuple<T...> t;
            size_t i = 0;
            std::apply([&](auto&... x) { ((x = str(g[i++])), ...); }, t);
            out.append(t);
        }
        return out;
    }
    str sub(const str& repl, const str& s, int64_t count = 0) const {
        return str(pyre_([&] { return r_->sub(repl.utf8(), s.utf8(), static_cast<int>(count)); }));
    }
    template <class F> str sub_fn(const F& fn, const str& s, int64_t count = 0) const {
        return str(r_->sub_fn([&](const ::re::Match& m) { return fn(Match(m)).utf8(); }, s.utf8(), static_cast<int>(count)));
    }
    // (new) Pattern.sub(fn, s): the replacement a function of the match
    template <class F, class = decltype(std::declval<const F&>()(std::declval<const Match&>()))>
    str sub(const F& fn, const str& s, int64_t count = 0) const { return sub_fn(fn, s, count); }
    template <class F, class = decltype(std::declval<const F&>()(std::declval<const Match&>()))>
    std::tuple<str, int64_t> subn(const F& fn, const str& s, int64_t count = 0) const {
        int64_t n = static_cast<int64_t>(r_->finditer(s.utf8()).size());
        if (count > 0 && n > count) n = count;
        return {sub_fn(fn, s, count), n};
    }
    std::tuple<str, int64_t> subn(const str& repl, const str& s, int64_t count = 0) const {
        int64_t n = static_cast<int64_t>(r_->finditer(s.utf8()).size());
        if (count > 0 && n > count) n = count;
        return {sub(repl, s, count), n};
    }
    list<str> split(const str& s, int64_t maxsplit = 0) const {
        list<str> out;
        std::string t = s.utf8();
        size_t at = 0;
        int64_t n = 0;
        for (auto& m : r_->finditer(t)) {
            if (maxsplit > 0 && n >= maxsplit) break;
            out.append(str(t.substr(at, static_cast<size_t>(m.start()) - at)));
            for (int g = 1; g <= m.ngroups(); g++) out.append(str(m.group(g)));
            at = static_cast<size_t>(m.end());
            n++;
        }
        out.append(str(t.substr(at)));
        return out;
    }
};
inline Pattern compile(const str& p, int64_t flags = 0) { return Pattern(p, flags); }
inline std::optional<Match> match(const str& p, const str& s, int64_t flags = 0) { return Pattern(p, flags).match(s); }
inline std::optional<Match> fullmatch(const str& p, const str& s, int64_t flags = 0) { return Pattern(p, flags).fullmatch(s); }
inline std::optional<Match> search(const str& p, const str& s, int64_t flags = 0) { return Pattern(p, flags).search(s); }
inline list<Match> finditer(const str& p, const str& s, int64_t flags = 0) { return Pattern(p, flags).finditer(s); }
inline list<str> findall(const str& p, const str& s, int64_t flags = 0) { return Pattern(p, flags).findall(s); }
inline str sub(const str& p, const str& repl, const str& s, int64_t count = 0, int64_t flags = 0) {
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
inline str escape(const str& s) { return str(::re::escape(s.utf8())); }
constexpr int64_t I = 2, IGNORECASE = 2, M = 8, MULTILINE = 8, S = 16, DOTALL = 16, X = 64, VERBOSE = 64, A = 256, ASCII = 256;
}  // namespace re
}  // namespace py
#endif
