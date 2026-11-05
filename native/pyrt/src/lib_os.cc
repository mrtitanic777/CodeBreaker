// lib_os.cc -- the operating system modules on Windows, as CPython behaves there: os (files,
// directories, environ, stat, walk, scandir ...), os.path (ntpath's rules), shutil, glob, fnmatch,
// tempfile, subprocess (CreateProcessW, list2cmdline's quoting, pipes, timeouts), time, stat, errno.
#include <windows.h>
#include <bcrypt.h>
#include <fcntl.h>
#include <io.h>
#include <direct.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/utime.h>

#include <algorithm>
#include <cerrno>
#include <chrono>
#include <ctime>
#include <thread>

#include "rt.hpp"

namespace py {

var os_error(int err, const std::string& filename, const std::string& filename2 = "");
void flush_std_();
std::wstring wide_(const std::string& s);
var open_file(const var& file, const std::string& mode, int64_t buffering, const var& encoding, const var& errors, const var& newline);
static std::string narrow(const std::wstring& w) {
    int n = WideCharToMultiByte(CP_UTF8, 0, w.data(), (int)w.size(), nullptr, 0, nullptr, nullptr);
    std::string s((size_t)n, '\0');
    WideCharToMultiByte(CP_UTF8, 0, w.data(), (int)w.size(), s.data(), n, nullptr, nullptr);
    return s;
}
static var win_error(DWORD e, const std::string& fn, const std::string& fn2 = "") {
    int err;
    switch (e) {
    case ERROR_FILE_NOT_FOUND: case ERROR_PATH_NOT_FOUND: case ERROR_INVALID_DRIVE: case ERROR_BAD_NETPATH: err = ENOENT; break;
    case ERROR_ACCESS_DENIED: case ERROR_SHARING_VIOLATION: case ERROR_LOCK_VIOLATION: err = EACCES; break;
    case ERROR_ALREADY_EXISTS: case ERROR_FILE_EXISTS: err = EEXIST; break;
    case ERROR_DIR_NOT_EMPTY: err = ENOTEMPTY; break;
    case ERROR_DIRECTORY: err = ENOTDIR; break;
    case ERROR_INVALID_NAME: err = EINVAL; break;
    default: err = EINVAL; break;
    }
    var x = os_error(err, fn, fn2);
    x.setattr("winerror", var((int64_t)e));
    return x;
}

std::string fs_path(const var& p) {
    if (is_str(p)) return str_utf8(p);
    if (is_bytes(p)) return raw_of(p);
    var m;
    if (lookup_special(p, "__fspath__", m)) return fs_path(call0(m));
    raise(E_TypeError, "expected str, bytes or os.PathLike object, not " + tname(p));
}

// ------------------------------------------------------------------------------------------ ntpath
static bool is_sep(char c) { return c == '\\' || c == '/'; }
static void splitdrive(const std::string& p, std::string& drive, std::string& rest) {
    if (p.size() >= 2 && p[1] == ':' && isalpha((unsigned char)p[0])) { drive = p.substr(0, 2); rest = p.substr(2); return; }
    if (p.size() >= 2 && is_sep(p[0]) && is_sep(p[1])) {
        // UNC: \\server\share
        size_t i = p.find_first_of("\\/", 2);
        if (i == std::string::npos) { drive = p; rest = ""; return; }
        size_t j = p.find_first_of("\\/", i + 1);
        if (j == std::string::npos) j = p.size();
        drive = p.substr(0, j);
        rest = p.substr(j);
        return;
    }
    drive = "";
    rest = p;
}
std::string path_join(const std::vector<std::string>& parts) {
    std::string rdrive, rpath;
    splitdrive(parts[0], rdrive, rpath);
    for (size_t k = 1; k < parts.size(); k++) {
        std::string pd, pp;
        splitdrive(parts[k], pd, pp);
        if (!pp.empty() && is_sep(pp[0])) {
            if (!pd.empty() || rdrive.empty()) rdrive = pd;
            rpath = pp;
            continue;
        } else if (!pd.empty() && pd != rdrive) {
            std::string a = pd, b = rdrive;
            for (auto& c : a) c = (char)tolower((unsigned char)c);
            for (auto& c : b) c = (char)tolower((unsigned char)c);
            if (a != b) { rdrive = pd; rpath = pp; continue; }
            rdrive = pd;
        }
        if (!rpath.empty() && !is_sep(rpath.back())) rpath += "\\";
        rpath += pp;
    }
    if (!rpath.empty() && !is_sep(rpath[0]) && !rdrive.empty() && rdrive.back() != ':') return rdrive + "\\" + rpath;
    return rdrive + rpath;
}
static std::pair<std::string, std::string> path_split(const std::string& p) {
    std::string d, r;
    splitdrive(p, d, r);
    size_t i = r.size();
    while (i > 0 && !is_sep(r[i - 1])) i--;
    std::string head = r.substr(0, i), tail = r.substr(i);
    std::string h = head;
    while (!h.empty() && is_sep(h.back())) h.pop_back();
    if (h.empty()) h = head;
    return {d + h, tail};
}
static std::pair<std::string, std::string> path_splitext(const std::string& p) {
    size_t sep = p.find_last_of("\\/");
    size_t dot = p.rfind('.');
    if (dot == std::string::npos || (sep != std::string::npos && dot < sep)) return {p, ""};
    size_t start = sep == std::string::npos ? 0 : sep + 1;
    // leading dots of the name do not start an extension
    size_t k = start;
    while (k < dot && p[k] == '.') k++;
    if (k == dot) return {p, ""};
    return {p.substr(0, dot), p.substr(dot)};
}
std::string path_normpath(const std::string& path) {
    if (path.empty()) return ".";
    std::string p = path;
    std::replace(p.begin(), p.end(), '/', '\\');
    std::string drive, rest;
    splitdrive(p, drive, rest);
    std::string prefix;
    if (!rest.empty() && rest[0] == '\\') {
        prefix = "\\";
        size_t i = 0;
        while (i < rest.size() && rest[i] == '\\') i++;
        rest = rest.substr(i);
    }
    std::vector<std::string> comps, out;
    size_t s = 0;
    for (size_t i = 0; i <= rest.size(); i++)
        if (i == rest.size() || rest[i] == '\\') { comps.push_back(rest.substr(s, i - s)); s = i + 1; }
    for (auto& c : comps) {
        if (c.empty() || c == ".") continue;
        if (c == "..") {
            if (!out.empty() && out.back() != "..") { out.pop_back(); continue; }
            if (!prefix.empty()) continue;
            out.push_back(c);
            continue;
        }
        out.push_back(c);
    }
    std::string r = drive + prefix;
    for (size_t i = 0; i < out.size(); i++) r += (i ? "\\" : "") + out[i];
    if (r.empty()) r = ".";
    return r;
}
std::string getcwd_() {
    wchar_t buf[32768];
    DWORD n = GetCurrentDirectoryW(32768, buf);
    return narrow(std::wstring(buf, n));
}
static bool isabs(const std::string& p) {
    std::string d, r;
    splitdrive(p.size() > 3 ? p.substr(0, 3) : p, d, r);
    if (p.size() >= 2 && is_sep(p[0]) && is_sep(p[1])) return true;
    std::string drive, rest;
    splitdrive(p, drive, rest);
    return !rest.empty() && is_sep(rest[0]) && (!drive.empty() || true) && (drive.empty() ? true : true);
}
std::string path_abspath(const std::string& p) {
    std::string q = p;
    if (q.empty()) q = getcwd_();
    std::wstring w = wide_(q);
    wchar_t buf[32768];
    DWORD n = GetFullPathNameW(w.c_str(), 32768, buf, nullptr);
    if (n == 0 || n >= 32768) return path_normpath(path_join({getcwd_(), q}));
    return narrow(std::wstring(buf, n));
}
static std::string normcase(const std::string& p) {
    std::string r = p;
    std::replace(r.begin(), r.end(), '/', '\\');
    std::wstring w = wide_(r);
    if (!w.empty()) {
        std::wstring o(w.size(), L'\0');
        LCMapStringEx(LOCALE_NAME_INVARIANT, LCMAP_LOWERCASE, w.data(), (int)w.size(), o.data(), (int)o.size(), nullptr, nullptr, 0);
        return narrow(o);
    }
    return r;
}
static std::string relpath(const std::string& path, const std::string& start) {
    std::string a = path_abspath(path), b = path_abspath(start);
    std::string ad, ar, bd, br;
    splitdrive(a, ad, ar);
    splitdrive(b, bd, br);
    if (normcase(ad) != normcase(bd)) raise(E_ValueError, "path is on mount '" + ad + "', start on mount '" + bd + "'");
    auto parts = [](const std::string& s) {
        std::vector<std::string> v;
        size_t st = 0;
        for (size_t i = 0; i <= s.size(); i++)
            if (i == s.size() || is_sep(s[i])) { if (i > st) v.push_back(s.substr(st, i - st)); st = i + 1; }
        return v;
    };
    auto pa = parts(ar), pb = parts(br);
    size_t i = 0;
    while (i < pa.size() && i < pb.size() && normcase(pa[i]) == normcase(pb[i])) i++;
    std::vector<std::string> out;
    for (size_t k = i; k < pb.size(); k++) out.push_back("..");
    for (size_t k = i; k < pa.size(); k++) out.push_back(pa[k]);
    if (out.empty()) return ".";
    std::string r;
    for (size_t k = 0; k < out.size(); k++) r += (k ? "\\" : "") + out[k];
    return r;
}
static bool stat_w(const std::string& p, struct _stat64& st) {
    if (p.find('\0') != std::string::npos) return false;
    std::wstring w = wide_(p);
    // trailing separators on a file: CPython's stat fails; on a dir it works
    return _wstat64(w.c_str(), &st) == 0;
}
static bool exists(const std::string& p) {
    if (p.empty()) return false;
    DWORD a = GetFileAttributesW(wide_(p).c_str());
    return a != INVALID_FILE_ATTRIBUTES;
}
static bool isdir(const std::string& p) {
    if (p.empty()) return false;
    DWORD a = GetFileAttributesW(wide_(p).c_str());
    return a != INVALID_FILE_ATTRIBUTES && (a & FILE_ATTRIBUTE_DIRECTORY);
}
static bool isfile(const std::string& p) {
    if (p.empty()) return false;
    DWORD a = GetFileAttributesW(wide_(p).c_str());
    return a != INVALID_FILE_ATTRIBUTES && !(a & FILE_ATTRIBUTE_DIRECTORY);
}
static std::string expanduser(const std::string& p) {
    if (p.empty() || p[0] != '~') return p;
    size_t i = 1;
    while (i < p.size() && !is_sep(p[i])) i++;
    std::string home;
    if (const char* up = getenv("USERPROFILE")) home = up;
    else {
        const char* hd = getenv("HOMEDRIVE");
        const char* hp = getenv("HOMEPATH");
        if (!hd || !hp) return p;
        home = std::string(hd) + hp;
    }
    if (i != 1) home = path_join({path_split(home).first, p.substr(1, i - 1)});
    return home + p.substr(i);
}

// a path argument's type comes back: bytes in, bytes out
static var path_out(const var& like, const std::string& s) { return is_bytes(like) ? new_bytes(s) : new_str(s); }
static std::string P0(const CallArgs& a, size_t i = 0) { return fs_path(a.pos.at(i)); }

static var stat_result(const struct _stat64& st, const std::string& p) {
    var r = new_tuple({var((int64_t)st.st_mode), var((int64_t)st.st_ino), var((int64_t)st.st_dev), var((int64_t)st.st_nlink),
                       var((int64_t)st.st_uid), var((int64_t)st.st_gid), var((int64_t)st.st_size), var((int64_t)st.st_atime),
                       var((int64_t)st.st_mtime), var((int64_t)st.st_ctime)});
    // exact times (CPython gives float seconds with sub-second precision)
    double mt = (double)st.st_mtime, at = (double)st.st_atime, ct = (double)st.st_ctime;
    WIN32_FILE_ATTRIBUTE_DATA fa;
    if (!p.empty() && GetFileAttributesExW(wide_(p).c_str(), GetFileExInfoStandard, &fa)) {
        auto ft = [](const FILETIME& f) {
            uint64_t v = ((uint64_t)f.dwHighDateTime << 32) | f.dwLowDateTime;
            return (double)(v - 116444736000000000ULL) / 1e7;
        };
        mt = ft(fa.ftLastWriteTime);
        at = ft(fa.ftLastAccessTime);
        ct = ft(fa.ftCreationTime);
    }
    var o = call0(builtin("object"));
    (void)o;
    static TypeObj* T_stat = nullptr;
    if (!T_stat) T_stat = new_type("stat_result", {}, Layout::NativeData, true, "os");
    auto* s = new DataObj();
    s->type = T_stat;
    var sv(static_cast<Obj*>(s));
    sv.setattr("st_mode", var((int64_t)st.st_mode));
    sv.setattr("st_size", var((int64_t)st.st_size));
    sv.setattr("st_mtime", var(mt));
    sv.setattr("st_atime", var(at));
    sv.setattr("st_ctime", var(ct));
    sv.setattr("st_mtime_ns", var((int64_t)(mt * 1e9)));
    sv.setattr("st_ino", var((int64_t)st.st_ino));
    sv.setattr("st_dev", var((int64_t)st.st_dev));
    sv.setattr("st_nlink", var((int64_t)st.st_nlink));
    sv.setattr("st_uid", 0);
    sv.setattr("st_gid", 0);
    sv.setattr("_tuple", r);
    return sv;
}
static var do_stat(const std::string& p) {
    struct _stat64 st;
    if (!stat_w(p, st)) {
        DWORD e = GetLastError();
        if (!exists(p)) raise(os_error(ENOENT, p));
        raise(win_error(e, p));
    }
    return stat_result(st, p);
}

static std::vector<std::string> listdir_(const std::string& p) {
    std::string q = p.empty() ? "." : p;
    std::wstring pat = wide_(q);
    if (!pat.empty() && pat.back() != L'\\' && pat.back() != L'/') pat += L"\\";
    pat += L"*";
    WIN32_FIND_DATAW fd;
    HANDLE h = FindFirstFileW(pat.c_str(), &fd);
    std::vector<std::string> out;
    if (h == INVALID_HANDLE_VALUE) {
        DWORD e = GetLastError();
        if (e == ERROR_FILE_NOT_FOUND) return out;
        if (isfile(q)) raise(win_error(ERROR_DIRECTORY, q));
        raise(win_error(e == ERROR_PATH_NOT_FOUND ? ERROR_PATH_NOT_FOUND : e, q));
    }
    do {
        std::wstring n = fd.cFileName;
        if (n == L"." || n == L"..") continue;
        out.push_back(narrow(n));
    } while (FindNextFileW(h, &fd));
    FindClose(h);
    return out;
}

static void makedirs_(const std::string& p, bool exist_ok) {
    if (isdir(p)) {
        if (!exist_ok) raise(os_error(EEXIST, p));
        return;
    }
    auto [head, tail] = path_split(p);
    if (tail.empty()) { auto ht = path_split(head); head = ht.first; tail = ht.second; }
    if (!head.empty() && !tail.empty() && !exists(head)) {
        try {
            makedirs_(head, true);
        } catch (error& e) {
            if (!isinstance_(e.value, E_FileExistsError)) throw;
        }
    }
    if (tail == ".") return;
    if (!CreateDirectoryW(wide_(p).c_str(), nullptr)) {
        DWORD e = GetLastError();
        if (e == ERROR_ALREADY_EXISTS && (exist_ok && isdir(p))) return;
        raise(win_error(e, p));
    }
}

static void rmtree_(const std::string& p, bool ignore, const var& onerror) {
    auto fail = [&](const char* fn, const std::string& path, error& e) {
        if (ignore) return;
        if (!onerror.is_none() && !onerror.is_unbound()) {
            call_obj(onerror, pack_args_(builtin("print"), new_str(path), new_tuple({tv(type_of(e.value)), e.value, None})));
            return;
        }
        (void)fn;
        throw e;
    };
    DWORD a = GetFileAttributesW(wide_(p).c_str());
    if (a == INVALID_FILE_ATTRIBUTES) {
        if (ignore) return;
        try { raise(win_error(GetLastError(), p)); } catch (error& e) { fail("lstat", p, e); return; }
    }
    if ((a & FILE_ATTRIBUTE_DIRECTORY) && !(a & FILE_ATTRIBUTE_REPARSE_POINT)) {
        std::vector<std::string> names;
        try { names = listdir_(p); } catch (error& e) { fail("scandir", p, e); }
        for (auto& n : names) {
            std::string c = path_join({p, n});
            DWORD ca = GetFileAttributesW(wide_(c).c_str());
            if (ca != INVALID_FILE_ATTRIBUTES && (ca & FILE_ATTRIBUTE_DIRECTORY) && !(ca & FILE_ATTRIBUTE_REPARSE_POINT)) rmtree_(c, ignore, onerror);
            else {
                if (ca & FILE_ATTRIBUTE_READONLY) SetFileAttributesW(wide_(c).c_str(), ca & ~FILE_ATTRIBUTE_READONLY);
                BOOL ok = (ca & FILE_ATTRIBUTE_DIRECTORY) ? RemoveDirectoryW(wide_(c).c_str()) : DeleteFileW(wide_(c).c_str());
                if (!ok) { try { raise(win_error(GetLastError(), c)); } catch (error& e) { fail("unlink", c, e); } }
            }
        }
        if (!RemoveDirectoryW(wide_(p).c_str())) { try { raise(win_error(GetLastError(), p)); } catch (error& e) { fail("rmdir", p, e); } }
    } else {
        if (!DeleteFileW(wide_(p).c_str())) { try { raise(win_error(GetLastError(), p)); } catch (error& e) { fail("unlink", p, e); } }
    }
}

static void copyfile_(const std::string& src, const std::string& dst) {
    if (!CopyFileW(wide_(src).c_str(), wide_(dst).c_str(), FALSE)) {
        DWORD e = GetLastError();
        if (!exists(src)) raise(os_error(ENOENT, src));
        raise(win_error(e, dst));
    }
}

static var environ_dict;

// ------------------------------------------------------------------------------------------ subprocess
static std::string list2cmdline(const std::vector<std::string>& args) {
    std::string out;
    for (size_t k = 0; k < args.size(); k++) {
        const std::string& arg = args[k];
        if (k) out.push_back(' ');
        bool need = arg.empty() || arg.find_first_of(" \t") != std::string::npos;
        if (need) out.push_back('"');
        std::string bs;
        for (char c : arg) {
            if (c == '\\') bs.push_back(c);
            else if (c == '"') { out += std::string(bs.size() * 2, '\\'); bs.clear(); out += "\\\""; }
            else { if (!bs.empty()) { out += bs; bs.clear(); } out.push_back(c); }
        }
        if (!bs.empty()) out += bs;
        if (need) { out += bs; out.push_back('"'); }
    }
    return out;
}
static const int PIPE = -1, STDOUT = -2, DEVNULL = -3;
static TypeObj *T_popen, *T_completed;
static var E_TimeoutExpired, E_CalledProcessError, E_SubprocessError;

struct PopenObj : DataObj {
    HANDLE proc = nullptr;
    HANDLE in_w = nullptr, out_r = nullptr, err_r = nullptr;
    DWORD pid = 0;
    bool text = false;
    std::string encoding = "utf-8", errors = "strict";
    var returncode;
    ~PopenObj() override {
        if (in_w) CloseHandle(in_w);
        if (out_r) CloseHandle(out_r);
        if (err_r) CloseHandle(err_r);
        if (proc) CloseHandle(proc);
    }
};
var bytes_decode_(const std::string& raw, const std::string& enc, const std::string& errors);
static var decode_out(PopenObj* p, const std::string& raw, bool text) {
    if (!text) return new_bytes(raw);
    var s = bytes_decode_(raw, p->encoding, p->errors);
    // universal newlines
    std::string o;
    const std::string& t = str_utf8(s);
    for (size_t i = 0; i < t.size(); i++) {
        if (t[i] == '\r') { o.push_back('\n'); if (i + 1 < t.size() && t[i + 1] == '\n') i++; }
        else o.push_back(t[i]);
    }
    return new_str(o);
}

static HANDLE handle_for(const var& v, bool write, HANDLE& parent_end, bool& made) {
    made = false;
    SECURITY_ATTRIBUTES sa{sizeof sa, nullptr, TRUE};
    if (v.is_none() || v.is_unbound()) return GetStdHandle(write ? STD_OUTPUT_HANDLE : STD_INPUT_HANDLE);
    if (is_int(v)) {
        int64_t k = v.as_int64();
        if (k == PIPE) {
            HANDLE r, w;
            CreatePipe(&r, &w, &sa, 0);
            if (write) { SetHandleInformation(r, HANDLE_FLAG_INHERIT, 0); parent_end = r; made = true; return w; }
            SetHandleInformation(w, HANDLE_FLAG_INHERIT, 0);
            parent_end = w;
            made = true;
            return r;
        }
        if (k == DEVNULL) {
            made = true;
            return CreateFileW(L"NUL", write ? GENERIC_WRITE : GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, &sa, OPEN_EXISTING, 0, nullptr);
        }
        HANDLE h = (HANDLE)_get_osfhandle((int)k);
        HANDLE d;
        DuplicateHandle(GetCurrentProcess(), h, GetCurrentProcess(), &d, 0, TRUE, DUPLICATE_SAME_ACCESS);
        made = true;
        return d;
    }
    // a file object
    var fd = v.call("fileno");
    v.call("flush");
    HANDLE h = (HANDLE)_get_osfhandle((int)fd.as_int64());
    HANDLE d;
    DuplicateHandle(GetCurrentProcess(), h, GetCurrentProcess(), &d, 0, TRUE, DUPLICATE_SAME_ACCESS);
    made = true;
    return d;
}

static std::wstring env_block(const var& env) {
    std::vector<std::wstring> items;
    var e = env.is_none() || env.is_unbound() ? environ_dict : env;
    for (var k : each(e)) items.push_back(wide_(str_utf8(k)) + L"=" + wide_(str_utf8(e.getitem(k))));
    std::sort(items.begin(), items.end(), [](const std::wstring& a, const std::wstring& b) { return _wcsicmp(a.c_str(), b.c_str()) < 0; });
    std::wstring blk;
    for (auto& s : items) { blk += s; blk.push_back(L'\0'); }
    blk.push_back(L'\0');
    return blk;
}

static var popen_new(const CallArgs& a) {
    var args = arg(a, 1, "args");
    var stdin_v = kwarg(a, "stdin", None), stdout_v = kwarg(a, "stdout", None), stderr_v = kwarg(a, "stderr", None);
    var cwd = kwarg(a, "cwd", None), env = kwarg(a, "env", None), shell = kwarg(a, "shell", false);
    bool text = kwarg(a, "text", false).truthy() || kwarg(a, "universal_newlines", false).truthy();
    var enc = kwarg(a, "encoding", None), errs = kwarg(a, "errors", None);
    if (!enc.is_none() || !errs.is_none()) text = true;
    var cflags = kwarg(a, "creationflags", 0);
    std::string cmd;
    std::string exe0;
    if (is_str(args) || is_bytes(args)) cmd = fs_path(args);
    else {
        std::vector<std::string> v;
        for (var x : each(args)) v.push_back(fs_path(x));
        if (!v.empty()) exe0 = v[0];
        cmd = list2cmdline(v);
    }
    if (shell.truthy()) {
        const char* comspec = getenv("COMSPEC");
        cmd = std::string(comspec ? comspec : "cmd.exe") + " /c \"" + cmd + "\"";
    }
    auto* p = new PopenObj();
    p->type = T_popen;
    var self(static_cast<Obj*>(p));
    p->text = text;
    if (!enc.is_none()) p->encoding = str_utf8(enc);
    else if (text) {
        // locale.getpreferredencoding(False) on Windows: the ANSI code page
        UINT cp = GetACP();
        p->encoding = cp == 65001 ? "utf-8" : cp == 1252 ? "cp1252" : "cp" + std::to_string(cp);
    }
    if (!errs.is_none()) p->errors = str_utf8(errs);
    STARTUPINFOW si{};
    si.cb = sizeof si;
    si.dwFlags = STARTF_USESTDHANDLES;
    bool m_in, m_out, m_err;
    HANDLE hin = handle_for(stdin_v, false, p->in_w, m_in);
    HANDLE hout = handle_for(stdout_v, true, p->out_r, m_out);
    HANDLE herr;
    bool m_err2 = false;
    if (is_int(stderr_v) && stderr_v.as_int64() == STDOUT) { herr = hout; m_err = false; }
    else if (stderr_v.is_none()) { herr = GetStdHandle(STD_ERROR_HANDLE); m_err = false; }
    else herr = handle_for(stderr_v, true, p->err_r, m_err);
    (void)m_err2;
    si.hStdInput = hin;
    si.hStdOutput = hout;
    si.hStdError = herr;
    std::wstring wcmd = wide_(cmd);
    std::wstring wcwd = cwd.is_none() ? L"" : wide_(fs_path(cwd));
    std::wstring blk = env_block(env);
    PROCESS_INFORMATION pi{};
    BOOL ok = CreateProcessW(nullptr, wcmd.data(), nullptr, nullptr, TRUE, CREATE_UNICODE_ENVIRONMENT | (DWORD)cflags.as_int64(),
                             (LPVOID)blk.data(), cwd.is_none() ? nullptr : wcwd.c_str(), &si, &pi);
    DWORD err = GetLastError();
    if (m_in) CloseHandle(hin);
    if (m_out) CloseHandle(hout);
    if (m_err) CloseHandle(herr);
    if (!ok) {
        var x = win_error(err, exe0.empty() ? cmd : exe0);
        if (err == ERROR_FILE_NOT_FOUND || err == ERROR_PATH_NOT_FOUND) {
            x = os_error(ENOENT, exe0.empty() ? cmd : exe0);
            x.setattr("winerror", var((int64_t)err));
        }
        raise(x);
    }
    CloseHandle(pi.hThread);
    p->proc = pi.hProcess;
    p->pid = pi.dwProcessId;
    self.setattr("args", args);
    self.setattr("pid", var((int64_t)p->pid));
    self.setattr("returncode", None);
    self.setattr("stdin", None);
    self.setattr("stdout", None);
    self.setattr("stderr", None);
    return self;
}
static void read_handle(HANDLE h, std::string* out) {
    char buf[65536];
    DWORD n;
    while (ReadFile(h, buf, sizeof buf, &n, nullptr) && n > 0) out->append(buf, n);
}
// communicate(input, timeout): (stdout, stderr)
static var communicate(const var& self, const var& input, const var& timeout) {
    auto* p = static_cast<PopenObj*>(self.ref());
    std::string out, err;
    std::thread to, te;
    if (p->out_r) to = std::thread(read_handle, p->out_r, &out);
    if (p->err_r) te = std::thread(read_handle, p->err_r, &err);
    if (p->in_w) {
        if (!input.is_none() && !input.is_unbound()) {
            std::string data = is_str(input) ? raw_of(input.call("encode", new_str(p->encoding))) : raw_of(input);
            DWORD w;
            WriteFile(p->in_w, data.data(), (DWORD)data.size(), &w, nullptr);
        }
        CloseHandle(p->in_w);
        p->in_w = nullptr;
    }
    DWORD ms = timeout.is_none() || timeout.is_unbound() ? INFINITE : (DWORD)(timeout.as_double() * 1000);
    DWORD r;
    {
        gil_release g;
        r = WaitForSingleObject(p->proc, ms);
    }
    if (r == WAIT_TIMEOUT) {
        TerminateProcess(p->proc, 1);
        WaitForSingleObject(p->proc, INFINITE);
        if (to.joinable()) to.join();
        if (te.joinable()) te.join();
        var x = make_exc_args(T(E_TimeoutExpired), {self.getattr("args"), timeout});
        x.setattr("cmd", self.getattr("args"));
        x.setattr("timeout", timeout);
        x.setattr("output", p->out_r ? decode_out(p, out, p->text) : var(None));
        x.setattr("stdout", x.getattr("output"));
        x.setattr("stderr", p->err_r ? decode_out(p, err, p->text) : var(None));
        raise(x);
    }
    if (to.joinable()) to.join();
    if (te.joinable()) te.join();
    DWORD code;
    GetExitCodeProcess(p->proc, &code);
    p->returncode = var((int64_t)(int32_t)code);
    self.setattr("returncode", p->returncode);
    return new_tuple({p->out_r ? decode_out(p, out, p->text) : var(None), p->err_r ? decode_out(p, err, p->text) : var(None)});
}

static var run(const CallArgs& a) {
    CallArgs b;
    b.pos = {tv(T_popen), arg(a, 0, "args")};
    var input = kwarg(a, "input", None), timeout = kwarg(a, "timeout", None);
    bool check = kwarg(a, "check", false).truthy(), capture = kwarg(a, "capture_output", false).truthy();
    for (auto& [k, v] : a.kw) {
        const std::string& n = str_utf8(k);
        if (n == "input" || n == "timeout" || n == "check" || n == "capture_output") continue;
        b.kw.push_back({k, v});
    }
    if (capture) {
        b.kw.push_back({"stdout"_s, var(PIPE)});
        b.kw.push_back({"stderr"_s, var(PIPE)});
    }
    if (!input.is_none()) b.kw.push_back({"stdin"_s, var(PIPE)});
    var proc = popen_new(b);
    var oe = communicate(proc, input, timeout);
    var rc = proc.getattr("returncode");
    CallArgs c;
    c.pos = {proc.getattr("args"), rc, tuple_items(oe)[0], tuple_items(oe)[1]};
    var cp = call_obj(tv(T_completed), c);
    if (check && rc.as_int64() != 0) {
        var x = make_exc_args(T(E_CalledProcessError), {rc, proc.getattr("args")});
        x.setattr("returncode", rc);
        x.setattr("cmd", proc.getattr("args"));
        x.setattr("output", tuple_items(oe)[0]);
        x.setattr("stdout", tuple_items(oe)[0]);
        x.setattr("stderr", tuple_items(oe)[1]);
        raise(x);
    }
    return cp;
}

// ------------------------------------------------------------------------------------------ fnmatch / glob
static bool fnmatch_case(const std::u32string& name, size_t ni, const std::u32string& pat, size_t pi) {
    while (pi < pat.size()) {
        char32_t c = pat[pi];
        if (c == '*') {
            while (pi < pat.size() && pat[pi] == '*') pi++;
            if (pi == pat.size()) return true;
            for (size_t k = ni; k <= name.size(); k++)
                if (fnmatch_case(name, k, pat, pi)) return true;
            return false;
        }
        if (ni >= name.size()) return false;
        if (c == '?') { ni++; pi++; continue; }
        if (c == '[') {
            size_t j = pi + 1;
            bool neg = false;
            if (j < pat.size() && pat[j] == '!') { neg = true; j++; }
            size_t start = j;
            if (j < pat.size() && pat[j] == ']') j++;
            while (j < pat.size() && pat[j] != ']') j++;
            if (j >= pat.size()) {
                if (name[ni] != '[') return false;
                ni++; pi++;
                continue;
            }
            bool hit = false;
            for (size_t k = start; k < j; k++) {
                if (k + 2 < j && pat[k + 1] == '-') { if (name[ni] >= pat[k] && name[ni] <= pat[k + 2]) hit = true; k += 2; }
                else if (name[ni] == pat[k]) hit = true;
            }
            if (hit == neg) return false;
            ni++;
            pi = j + 1;
            continue;
        }
        if (name[ni] != c) return false;
        ni++;
        pi++;
    }
    return ni == name.size();
}
static bool fnmatch_(const std::string& name, const std::string& pat, bool norm) {
    std::string n = norm ? normcase(name) : name, p = norm ? normcase(pat) : pat;
    return fnmatch_case(to_u32(n), 0, to_u32(p), 0);
}
static bool has_magic(const std::string& s) { return s.find_first_of("*?[") != std::string::npos; }
static void glob_in(const std::string& dir, const std::string& pat, bool dironly, std::vector<std::string>& out, bool hidden_ok) {
    std::string d = dir.empty() ? "." : dir;
    if (!isdir(d)) return;
    std::vector<std::string> names;
    try { names = listdir_(d); } catch (error&) { return; }
    for (auto& n : names) {
        if (!hidden_ok && !n.empty() && n[0] == '.' && (pat.empty() || pat[0] != '.')) continue;
        if (!fnmatch_(n, pat, true)) continue;
        if (dironly && !isdir(path_join({d, n}))) continue;
        out.push_back(n);
    }
}
static void rlistdir(const std::string& dir, std::vector<std::string>& out, bool dironly) {
    std::string d = dir.empty() ? "." : dir;
    std::vector<std::string> names;
    try { names = listdir_(d); } catch (error&) { return; }
    for (auto& n : names) {
        if (!n.empty() && n[0] == '.') continue;
        std::string full = dir.empty() ? n : path_join({dir, n});
        bool isd = isdir(full);
        if (!dironly || isd) out.push_back(full);
        if (isd) rlistdir(full, out, dironly);
    }
}
static std::vector<std::string> glob_(const std::string& pathname, bool recursive);
static std::vector<std::string> glob_(const std::string& pathname, bool recursive) {
    std::vector<std::string> out;
    auto [dirname, basename] = path_split(pathname);
    if (!has_magic(pathname)) {
        if (basename.empty() ? isdir(dirname) : exists(pathname)) out.push_back(pathname);
        return out;
    }
    if (dirname.empty()) {
        if (recursive && basename == "**") { out.push_back(""); rlistdir("", out, false); out.erase(out.begin()); return out; }
        std::vector<std::string> names;
        glob_in("", basename, false, names, false);
        return names;
    }
    std::vector<std::string> dirs;
    if (dirname != pathname && has_magic(dirname)) dirs = glob_(dirname, recursive);
    else dirs = {dirname};
    for (auto& d : dirs) {
        if (recursive && basename == "**") {
            std::vector<std::string> sub;
            sub.push_back(path_join({d, ""}));
            rlistdir(d, sub, false);
            for (auto& s : sub) out.push_back(s);
            continue;
        }
        if (has_magic(basename)) {
            std::vector<std::string> names;
            glob_in(d, basename, false, names, false);
            for (auto& n : names) out.push_back(path_join({d, n}));
        } else if (basename.empty() ? isdir(d) : exists(path_join({d, basename}))) {
            out.push_back(path_join({d, basename}));
        }
    }
    return out;
}

// ------------------------------------------------------------------------------------------ tempfile
static std::string gettempdir_() {
    for (const char* e : {"TMPDIR", "TEMP", "TMP"})
        if (const char* v = getenv(e)) if (*v && isdir(v)) return path_normpath(path_abspath(v));
    return path_abspath(".");
}
static std::string rand_name() {
    static const char* chars = "abcdefghijklmnopqrstuvwxyz0123456789_";
    std::string s;
    unsigned char b[8];
    BCryptGenRandom(nullptr, b, 8, BCRYPT_USE_SYSTEM_PREFERRED_RNG);
    for (int i = 0; i < 8; i++) s.push_back(chars[b[i] % 37]);
    return s;
}

// ------------------------------------------------------------------------------------------ the modules
static var g_os, g_path;
static var str_list(const std::vector<std::string>& v, const var& like) {
    std::vector<var> o;
    for (auto& s : v) o.push_back(path_out(like, s));
    return new_list(o);
}

static void init_path(const var& m) {
    mod_set(m, "sep", "\\"_s);
    mod_set(m, "altsep", "/"_s);
    mod_set(m, "extsep", "."_s);
    mod_set(m, "pathsep", ";"_s);
    mod_set(m, "curdir", "."_s);
    mod_set(m, "pardir", ".."_s);
    mod_set(m, "defpath", ".;C:\\bin"_s);
    mod_set(m, "devnull", "nul"_s);
    mod_def(m, "join", [](const CallArgs& a) {
        if (a.pos.empty()) raise(E_TypeError, "join() missing 1 required positional argument: 'path'");
        std::vector<std::string> parts;
        for (auto& x : a.pos) parts.push_back(fs_path(x));
        return path_out(a.pos[0], path_join(parts));
    });
    mod_def(m, "basename", [](const CallArgs& a) { return path_out(a.pos.at(0), path_split(P0(a)).second); });
    mod_def(m, "dirname", [](const CallArgs& a) { return path_out(a.pos.at(0), path_split(P0(a)).first); });
    mod_def(m, "split", [](const CallArgs& a) { auto [h, t] = path_split(P0(a)); return new_tuple({path_out(a.pos[0], h), path_out(a.pos[0], t)}); });
    mod_def(m, "splitext", [](const CallArgs& a) { auto [h, t] = path_splitext(P0(a)); return new_tuple({path_out(a.pos[0], h), path_out(a.pos[0], t)}); });
    mod_def(m, "splitdrive", [](const CallArgs& a) { std::string d, r; splitdrive(P0(a), d, r); return new_tuple({path_out(a.pos[0], d), path_out(a.pos[0], r)}); });
    mod_def(m, "exists", [](const CallArgs& a) { try { return var(exists(P0(a))); } catch (error&) { return var(false); } });
    mod_def(m, "lexists", [](const CallArgs& a) { return var(exists(P0(a))); });
    mod_def(m, "isfile", [](const CallArgs& a) { try { return var(isfile(P0(a))); } catch (error&) { return var(false); } });
    mod_def(m, "isdir", [](const CallArgs& a) { try { return var(isdir(P0(a))); } catch (error&) { return var(false); } });
    mod_def(m, "islink", [](const CallArgs& a) {
        DWORD at = GetFileAttributesW(wide_(P0(a)).c_str());
        return var(at != INVALID_FILE_ATTRIBUTES && (at & FILE_ATTRIBUTE_REPARSE_POINT));
    });
    mod_def(m, "isabs", [](const CallArgs& a) {
        std::string p = P0(a);
        std::string d, r;
        splitdrive(p, d, r);
        if (p.size() >= 2 && is_sep(p[0]) && is_sep(p[1])) return var(true);
        return var(!d.empty() && !r.empty() && is_sep(r[0]));
    });
    mod_def(m, "abspath", [](const CallArgs& a) { return path_out(a.pos.at(0), path_abspath(P0(a))); });
    mod_def(m, "realpath", [](const CallArgs& a) {
        std::string p = path_abspath(P0(a));
        HANDLE h = CreateFileW(wide_(p).c_str(), 0, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, nullptr);
        if (h != INVALID_HANDLE_VALUE) {
            wchar_t buf[32768];
            DWORD n = GetFinalPathNameByHandleW(h, buf, 32768, FILE_NAME_NORMALIZED);
            CloseHandle(h);
            if (n > 0 && n < 32768) {
                std::wstring w(buf, n);
                if (w.rfind(L"\\\\?\\UNC\\", 0) == 0) w = L"\\\\" + w.substr(8);
                else if (w.rfind(L"\\\\?\\", 0) == 0) w = w.substr(4);
                return path_out(a.pos[0], narrow(w));
            }
        }
        return path_out(a.pos[0], p);
    });
    mod_def(m, "normpath", [](const CallArgs& a) { return path_out(a.pos.at(0), path_normpath(P0(a))); });
    mod_def(m, "normcase", [](const CallArgs& a) { return path_out(a.pos.at(0), normcase(P0(a))); });
    mod_def(m, "relpath", [](const CallArgs& a) {
        std::string start = a.pos.size() > 1 && !a.pos[1].is_none() ? fs_path(a.pos[1]) : kwarg(a, "start", None).is_none() ? "." : fs_path(kwarg(a, "start"));
        if (P0(a).empty()) raise(E_ValueError, "no path specified");
        return path_out(a.pos.at(0), relpath(P0(a), start));
    });
    mod_def(m, "expanduser", [](const CallArgs& a) { return path_out(a.pos.at(0), expanduser(P0(a))); });
    mod_def(m, "expandvars", [](const CallArgs& a) {
        std::string p = P0(a), o;
        for (size_t i = 0; i < p.size(); i++) {
            if (p[i] == '%') {
                size_t j = p.find('%', i + 1);
                if (j != std::string::npos) {
                    std::string n = p.substr(i + 1, j - i - 1);
                    var* v = static_cast<DictObj*>(environ_dict.ref())->get(new_str(n));
                    if (v) { o += str_utf8(*v); i = j; continue; }
                }
            }
            o.push_back(p[i]);
        }
        return path_out(a.pos[0], o);
    });
    mod_def(m, "getsize", [](const CallArgs& a) { return do_stat(P0(a)).getattr("st_size"); });
    mod_def(m, "getmtime", [](const CallArgs& a) { return do_stat(P0(a)).getattr("st_mtime"); });
    mod_def(m, "getatime", [](const CallArgs& a) { return do_stat(P0(a)).getattr("st_atime"); });
    mod_def(m, "getctime", [](const CallArgs& a) { return do_stat(P0(a)).getattr("st_ctime"); });
    mod_def(m, "samefile", [](const CallArgs& a) { return var(normcase(path_abspath(P0(a))) == normcase(path_abspath(P0(a, 1)))); });
    mod_def(m, "commonpath", [](const CallArgs& a) {
        std::vector<std::string> ps;
        for (var x : each(a.pos.at(0))) ps.push_back(path_normpath(fs_path(x)));
        if (ps.empty()) raise(E_ValueError, "commonpath() arg is an empty sequence");
        auto parts = [](const std::string& s) {
            std::vector<std::string> v;
            size_t st = 0;
            for (size_t i = 0; i <= s.size(); i++)
                if (i == s.size() || s[i] == '\\') { v.push_back(s.substr(st, i - st)); st = i + 1; }
            return v;
        };
        auto common = parts(ps[0]);
        for (size_t k = 1; k < ps.size(); k++) {
            auto p = parts(ps[k]);
            size_t i = 0;
            while (i < common.size() && i < p.size() && normcase(common[i]) == normcase(p[i])) i++;
            common.resize(i);
        }
        std::string r;
        for (size_t i = 0; i < common.size(); i++) r += (i ? "\\" : "") + common[i];
        if (r.size() == 2 && r[1] == ':') r += "\\";
        return new_str(r);
    });
    mod_def(m, "commonprefix", [](const CallArgs& a) {
        std::vector<std::string> ps;
        for (var x : each(a.pos.at(0))) ps.push_back(fs_path(x));
        if (ps.empty()) return ""_s;
        std::string p = *std::min_element(ps.begin(), ps.end()), q = *std::max_element(ps.begin(), ps.end());
        size_t i = 0;
        while (i < p.size() && i < q.size() && p[i] == q[i]) i++;
        return new_str(p.substr(0, i));
    });
}

static var scandir_entry(const std::string& dir, const std::string& name) {
    static TypeObj* T_entry = nullptr;
    if (!T_entry) {
        T_entry = new_type("DirEntry", {}, Layout::NativeData, true, "os");
        add_method(T_entry, "is_dir", [](const CallArgs& a) { return var(isdir(str_utf8(a.pos[0].getattr("path")))); });
        add_method(T_entry, "is_file", [](const CallArgs& a) { return var(isfile(str_utf8(a.pos[0].getattr("path")))); });
        add_method(T_entry, "is_symlink", [](const CallArgs&) { return var(false); });
        add_method(T_entry, "stat", [](const CallArgs& a) { return do_stat(str_utf8(a.pos[0].getattr("path"))); });
        add_method(T_entry, "__fspath__", [](const CallArgs& a) { return a.pos[0].getattr("path"); });
    }
    auto* e = new DataObj();
    e->type = T_entry;
    var ev(static_cast<Obj*>(e));
    ev.setattr("name", new_str(name));
    ev.setattr("path", new_str(path_join({dir, name})));
    return ev;
}

void init_os_modules() {
    register_native_module("os.path", [](const var& m) { init_path(m); g_path = m; });
    register_native_module("ntpath", [](const var& m) { init_path(m); });
    register_native_module("os", [](const var& m) {
        g_os = m;
        var path = import("os.path");
        mod_set(m, "path", path);
        mod_set(m, "name", "nt"_s);
        mod_set(m, "sep", "\\"_s);
        mod_set(m, "altsep", "/"_s);
        mod_set(m, "pathsep", ";"_s);
        mod_set(m, "linesep", "\r\n"_s);
        mod_set(m, "curdir", "."_s);
        mod_set(m, "pardir", ".."_s);
        mod_set(m, "extsep", "."_s);
        mod_set(m, "devnull", "nul"_s);
        mod_set(m, "SEEK_SET", 0);
        mod_set(m, "SEEK_CUR", 1);
        mod_set(m, "SEEK_END", 2);
        mod_set(m, "O_RDONLY", _O_RDONLY);
        mod_set(m, "O_WRONLY", _O_WRONLY);
        mod_set(m, "O_RDWR", _O_RDWR);
        mod_set(m, "O_CREAT", _O_CREAT);
        mod_set(m, "O_EXCL", _O_EXCL);
        mod_set(m, "O_TRUNC", _O_TRUNC);
        mod_set(m, "O_BINARY", _O_BINARY);
        mod_set(m, "O_APPEND", _O_APPEND);
        mod_set(m, "error", t::OSError);
        mod_set(m, "PathLike", t::object);
        // environ: the process environment as a dict (subprocess and getenv read it)
        environ_dict = new_dict_obj();
        wchar_t* blk = GetEnvironmentStringsW();
        for (wchar_t* p = blk; *p; p += wcslen(p) + 1) {
            std::wstring s = p;
            if (s.empty() || s[0] == L'=') continue;   // (=C: and the like: not in os.environ)
            size_t eq = s.find(L'=', 1);
            if (eq == std::wstring::npos) continue;
            std::string k = narrow(s.substr(0, eq));
            for (auto& c : k) c = (char)toupper((unsigned char)c);
            environ_dict.setitem(new_str(k), new_str(narrow(s.substr(eq + 1))));
        }
        FreeEnvironmentStringsW(blk);
        mod_set(m, "environ", environ_dict);
        mod_def(m, "getenv", [](const CallArgs& a) {
            var* v = static_cast<DictObj*>(environ_dict.ref())->get(new_str(str_utf8(a.pos.at(0))));
            if (!v) {
                std::string k = str_utf8(a.pos[0]);
                for (auto& c : k) c = (char)toupper((unsigned char)c);
                v = static_cast<DictObj*>(environ_dict.ref())->get(new_str(k));
            }
            return v ? *v : arg(a, 1, "default", None);
        });
        mod_def(m, "putenv", [](const CallArgs& a) { SetEnvironmentVariableW(wide_(str_utf8(a.pos.at(0))).c_str(), wide_(str_utf8(a.pos.at(1))).c_str()); return None; });
        mod_def(m, "fspath", [](const CallArgs& a) {
            var p = a.pos.at(0);
            if (is_str(p) || is_bytes(p)) return p;
            return new_str(fs_path(p));
        });
        mod_def(m, "fsencode", [](const CallArgs& a) { return new_bytes(fs_path(a.pos.at(0))); });
        mod_def(m, "fsdecode", [](const CallArgs& a) { return new_str(fs_path(a.pos.at(0))); });
        mod_def(m, "getcwd", [](const CallArgs&) { return new_str(getcwd_()); });
        mod_def(m, "chdir", [](const CallArgs& a) {
            if (!SetCurrentDirectoryW(wide_(P0(a)).c_str())) raise(win_error(GetLastError(), P0(a)));
            return None;
        });
        mod_def(m, "listdir", [](const CallArgs& a) {
            var p = a.pos.empty() ? "."_s : a.pos[0];
            return str_list(listdir_(fs_path(p)), p);
        });
        mod_def(m, "scandir", [](const CallArgs& a) {
            std::string d = a.pos.empty() ? "." : P0(a);
            std::vector<var> v;
            for (auto& n : listdir_(d)) v.push_back(scandir_entry(d, n));
            var it = iter(new_list(v));
            return it;
        });
        mod_def(m, "mkdir", [](const CallArgs& a) {
            std::string p = P0(a);
            if (!CreateDirectoryW(wide_(p).c_str(), nullptr)) raise(win_error(GetLastError(), p));
            return None;
        });
        mod_def(m, "makedirs", [](const CallArgs& a) {
            makedirs_(P0(a), (a.pos.size() > 2 ? a.pos[2] : kwarg(a, "exist_ok", false)).truthy());
            return None;
        });
        auto remove = [](const CallArgs& a) {
            std::string p = P0(a);
            if (!DeleteFileW(wide_(p).c_str())) raise(win_error(GetLastError(), p));
            return None;
        };
        mod_def(m, "remove", remove);
        mod_def(m, "unlink", remove);
        mod_def(m, "rmdir", [](const CallArgs& a) {
            std::string p = P0(a);
            if (!RemoveDirectoryW(wide_(p).c_str())) raise(win_error(GetLastError(), p));
            return None;
        });
        mod_def(m, "removedirs", [](const CallArgs& a) {
            std::string p = P0(a);
            if (!RemoveDirectoryW(wide_(p).c_str())) raise(win_error(GetLastError(), p));
            for (auto ht = path_split(p); !ht.first.empty() && ht.first != p; ht = path_split(ht.first)) {
                if (!RemoveDirectoryW(wide_(ht.first).c_str())) break;
                p = ht.first;
            }
            return None;
        });
        mod_def(m, "rename", [](const CallArgs& a) {
            std::string s = P0(a), d = P0(a, 1);
            if (!MoveFileExW(wide_(s).c_str(), wide_(d).c_str(), 0)) raise(win_error(GetLastError(), s, d));
            return None;
        });
        mod_def(m, "replace", [](const CallArgs& a) {
            std::string s = P0(a), d = P0(a, 1);
            if (!MoveFileExW(wide_(s).c_str(), wide_(d).c_str(), MOVEFILE_REPLACE_EXISTING)) raise(win_error(GetLastError(), s, d));
            return None;
        });
        mod_def(m, "stat", [](const CallArgs& a) { return do_stat(P0(a)); });
        mod_def(m, "lstat", [](const CallArgs& a) { return do_stat(P0(a)); });
        mod_def(m, "utime", [](const CallArgs& a) {
            std::string p = P0(a);
            var times = a.pos.size() > 1 ? a.pos[1] : kwarg(a, "times", None);
            struct __utimbuf64 ub;
            if (times.is_none()) {
                if (_wutime64(wide_(p).c_str(), nullptr) != 0) raise(os_error(errno, p));
                return None;
            }
            auto tv2 = collect(times);
            ub.actime = (__time64_t)tv2[0].as_double();
            ub.modtime = (__time64_t)tv2[1].as_double();
            if (_wutime64(wide_(p).c_str(), &ub) != 0) raise(os_error(errno, p));
            return None;
        });
        mod_def(m, "getpid", [](const CallArgs&) { return var((int64_t)GetCurrentProcessId()); });
        mod_def(m, "cpu_count", [](const CallArgs&) {
            SYSTEM_INFO si;
            GetSystemInfo(&si);
            return var((int64_t)si.dwNumberOfProcessors);
        });
        mod_def(m, "urandom", [](const CallArgs& a) {
            std::string s((size_t)a.pos.at(0).as_int64(), '\0');
            BCryptGenRandom(nullptr, (PUCHAR)s.data(), (ULONG)s.size(), BCRYPT_USE_SYSTEM_PREFERRED_RNG);
            return new_bytes(s);
        });
        mod_def(m, "walk", [](const CallArgs& a) {
            std::string top = P0(a);
            bool topdown = arg(a, 1, "topdown", true).truthy();
            var onerror = arg(a, 2, "onerror", None);
            // eager walk in Python's order (topdown: a dir before its children; dirnames may be pruned)
            struct Frame { std::string dir; var dirs; var files; };
            std::vector<var> out;
            std::function<void(const std::string&)> rec = [&](const std::string& d) {
                std::vector<std::string> names;
                try { names = listdir_(d); } catch (error& e) { if (!onerror.is_none()) call1(onerror, e.value); return; }
                std::vector<var> dirs, files;
                for (auto& n : names) (isdir(path_join({d, n})) ? dirs : files).push_back(new_str(n));
                var dl = new_list(dirs), fl = new_list(files);
                var row = new_tuple({new_str(d), dl, fl});
                if (topdown) out.push_back(row);
                for (var n : each(new_list(list_items(dl)))) rec(path_join({d, str_utf8(n)}));
                if (!topdown) out.push_back(row);
            };
            // (topdown: the caller's pruning of dirnames happens after the whole walk here: walk lazily instead)
            if (topdown) {
                // lazily, so the caller's pruning of dirnames (del dirs[:], dirs.remove) is seen
                auto stack = std::make_shared<std::vector<std::string>>();
                auto pending = std::make_shared<std::pair<std::string, var>>();
                stack->push_back(top);
                var on = onerror;
                return new_iter([stack, pending, on](var& res) {
                    if (!pending->second.is_unbound()) {
                        auto ds = collect(pending->second);
                        for (size_t k = ds.size(); k-- > 0;) stack->push_back(path_join({pending->first, str_utf8(ds[k])}));
                        pending->second = var();
                    }
                    while (!stack->empty()) {
                        std::string d = stack->back();
                        stack->pop_back();
                        std::vector<std::string> names;
                        try { names = listdir_(d); } catch (error& e) { if (!on.is_none()) call1(on, e.value); continue; }
                        std::vector<var> dirs, files;
                        for (auto& n : names) (isdir(path_join({d, n})) ? dirs : files).push_back(new_str(n));
                        var dl = new_list(dirs);
                        res = new_tuple({new_str(d), dl, new_list(files)});
                        *pending = {d, dl};
                        return true;
                    }
                    return false;
                });
            }
            rec(top);
            return iter(new_list(out));
        });
        mod_def(m, "access", [](const CallArgs& a) { return var(exists(P0(a))); });
        mod_set(m, "F_OK", 0);
        mod_set(m, "R_OK", 4);
        mod_set(m, "W_OK", 2);
        mod_set(m, "X_OK", 1);
        mod_def(m, "chmod", [](const CallArgs& a) {
            std::string p = P0(a);
            int mode = (int)a.pos.at(1).as_int64();
            DWORD at = GetFileAttributesW(wide_(p).c_str());
            if (at == INVALID_FILE_ATTRIBUTES) raise(win_error(GetLastError(), p));
            if (mode & 0200) at &= ~FILE_ATTRIBUTE_READONLY;
            else at |= FILE_ATTRIBUTE_READONLY;
            SetFileAttributesW(wide_(p).c_str(), at);
            return None;
        });
        mod_def(m, "_exit", [](const CallArgs& a) -> var { flush_std_(); ExitProcess((UINT)a.pos.at(0).as_int64()); });
        mod_def(m, "get_terminal_size", [](const CallArgs&) {
            CONSOLE_SCREEN_BUFFER_INFO ci;
            int64_t c = 80, r = 24;
            if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ci)) { c = ci.srWindow.Right - ci.srWindow.Left + 1; r = ci.srWindow.Bottom - ci.srWindow.Top + 1; }
            return new_tuple({var(c), var(r)});
        });
        mod_def(m, "add_dll_directory", [](const CallArgs& a) { AddDllDirectory(wide_(P0(a)).c_str()); return None; });
        mod_def(m, "close", [](const CallArgs& a) { _close((int)a.pos.at(0).as_int64()); return None; });
        mod_def(m, "fdopen", [](const CallArgs& a) -> var {
            int fd = (int)a.pos.at(0).as_int64();
            // a file object over the descriptor: re-opened by its path when known
            (void)fd;
            raise(E_NotImplementedError, "os.fdopen: use open() on the path");
        });
    });

    register_native_module("shutil", [](const var& m) {
        mod_def(m, "rmtree", [](const CallArgs& a) {
            rmtree_(P0(a), arg(a, 1, "ignore_errors", false).truthy(), kwarg(a, "onerror", kwarg(a, "onexc", None)));
            return None;
        });
        auto copy = [](const CallArgs& a, bool meta) {
            std::string s = P0(a), d = P0(a, 1);
            if (isdir(d)) d = path_join({d, path_split(s).second});
            copyfile_(s, d);
            (void)meta;
            return new_str(d);
        };
        mod_def(m, "copy", [copy](const CallArgs& a) { return copy(a, false); });
        mod_def(m, "copy2", [copy](const CallArgs& a) { return copy(a, true); });
        mod_def(m, "copyfile", [](const CallArgs& a) {
            std::string s = P0(a), d = P0(a, 1);
            if (normcase(path_abspath(s)) == normcase(path_abspath(d))) raise(make_exc_args(E_OSError, {new_str("'" + s + "' and '" + d + "' are the same file")}));
            copyfile_(s, d);
            return a.pos.at(1);
        });
        mod_def(m, "copymode", [](const CallArgs&) { return None; });
        mod_def(m, "copystat", [](const CallArgs&) { return None; });
        mod_def(m, "copyfileobj", [](const CallArgs& a) {
            for (;;) {
                var chunk = a.pos.at(0).call("read", 1 << 20);
                if (!chunk) break;
                a.pos.at(1).call("write", chunk);
            }
            return None;
        });
        mod_def(m, "copytree", [](const CallArgs& a) {
            std::string s = P0(a), d = P0(a, 1);
            bool dirs_exist_ok = kwarg(a, "dirs_exist_ok", false).truthy();
            var ignore = kwarg(a, "ignore", None);
            std::function<void(const std::string&, const std::string&)> rec = [&](const std::string& src, const std::string& dst) {
                makedirs_(dst, dirs_exist_ok || dst != d);
                std::vector<std::string> names = listdir_(src);
                std::vector<std::string> skip;
                if (!ignore.is_none()) {
                    std::vector<var> nv;
                    for (auto& n : names) nv.push_back(new_str(n));
                    for (var x : each(call2(ignore, new_str(src), new_list(nv)))) skip.push_back(str_utf8(x));
                }
                for (auto& n : names) {
                    if (std::find(skip.begin(), skip.end(), n) != skip.end()) continue;
                    std::string sp = path_join({src, n}), dp = path_join({dst, n});
                    if (isdir(sp)) rec(sp, dp);
                    else copyfile_(sp, dp);
                }
            };
            rec(s, d);
            return a.pos.at(1);
        });
        mod_def(m, "move", [](const CallArgs& a) {
            std::string s = P0(a), d = P0(a, 1);
            if (isdir(d)) d = path_join({d, path_split(s).second});
            if (!MoveFileExW(wide_(s).c_str(), wide_(d).c_str(), MOVEFILE_COPY_ALLOWED | MOVEFILE_REPLACE_EXISTING)) raise(win_error(GetLastError(), s, d));
            return new_str(d);
        });
        mod_def(m, "which", [](const CallArgs& a) {
            std::string cmd = P0(a);
            var pathv = kwarg(a, "path", None);
            std::string path;
            if (!pathv.is_none()) path = fs_path(pathv);
            else if (var* v = static_cast<DictObj*>(environ_dict.ref())->get("PATH"_s)) path = str_utf8(*v);
            std::vector<std::string> exts;
            std::string pathext = ".COM;.EXE;.BAT;.CMD;.VBS;.JS;.WS;.MSC";
            if (var* v = static_cast<DictObj*>(environ_dict.ref())->get("PATHEXT"_s)) pathext = str_utf8(*v);
            size_t st = 0;
            for (size_t i = 0; i <= pathext.size(); i++)
                if (i == pathext.size() || pathext[i] == ';') { if (i > st) exts.push_back(pathext.substr(st, i - st)); st = i + 1; }
            auto candidates = [&](const std::string& base) {
                std::vector<std::string> v;
                std::string low = normcase(base);
                bool has = false;
                for (auto& e : exts) if (low.size() >= e.size() && low.compare(low.size() - e.size(), e.size(), normcase(e)) == 0) has = true;
                if (has) v.push_back(base);
                for (auto& e : exts) v.push_back(base + e);
                if (!has) v.push_back(base);
                return v;
            };
            std::vector<std::string> dirs;
            if (cmd.find_first_of("\\/") != std::string::npos) dirs.push_back("");
            else {
                dirs.push_back(getcwd_());
                st = 0;
                for (size_t i = 0; i <= path.size(); i++)
                    if (i == path.size() || path[i] == ';') { if (i > st) dirs.push_back(path.substr(st, i - st)); st = i + 1; }
            }
            for (auto& d : dirs)
                for (auto& c : candidates(d.empty() ? cmd : path_join({d, cmd})))
                    if (isfile(c)) return new_str(c);
            return var(None);
        });
        mod_def(m, "disk_usage", [](const CallArgs& a) {
            ULARGE_INTEGER fr, tot, totfree;
            GetDiskFreeSpaceExW(wide_(P0(a)).c_str(), &fr, &tot, &totfree);
            return new_tuple({var((int64_t)tot.QuadPart), var((int64_t)(tot.QuadPart - totfree.QuadPart)), var((int64_t)fr.QuadPart)});
        });
        mod_set(m, "Error", t::OSError);
    });

    register_native_module("fnmatch", [](const var& m) {
        mod_def(m, "fnmatch", [](const CallArgs& a) { return var(fnmatch_(fs_path(a.pos.at(0)), fs_path(a.pos.at(1)), true)); });
        mod_def(m, "fnmatchcase", [](const CallArgs& a) { return var(fnmatch_(fs_path(a.pos.at(0)), fs_path(a.pos.at(1)), false)); });
        mod_def(m, "filter", [](const CallArgs& a) {
            std::vector<var> o;
            std::string p = fs_path(a.pos.at(1));
            for (var x : each(a.pos.at(0))) if (fnmatch_(fs_path(x), p, true)) o.push_back(x);
            return new_list(o);
        });
    });
    register_native_module("glob", [](const var& m) {
        mod_def(m, "glob", [](const CallArgs& a) {
            var p = arg(a, 0, "pathname");
            bool rec = kwarg(a, "recursive", false).truthy();
            var root = kwarg(a, "root_dir", None);
            std::string pat = fs_path(p);
            if (!root.is_none()) {
                std::string cwd = getcwd_();
                SetCurrentDirectoryW(wide_(fs_path(root)).c_str());
                auto r = glob_(pat, rec);
                SetCurrentDirectoryW(wide_(cwd).c_str());
                return str_list(r, p);
            }
            return str_list(glob_(pat, rec), p);
        });
        mod_def(m, "iglob", [](const CallArgs& a) {
            return iter(str_list(glob_(fs_path(a.pos.at(0)), kwarg(a, "recursive", false).truthy()), a.pos[0]));
        });
        mod_def(m, "escape", [](const CallArgs& a) {
            std::string s = fs_path(a.pos.at(0)), o;
            for (char c : s) { if (c == '*' || c == '?' || c == '[') { o += "["; o.push_back(c); o += "]"; } else o.push_back(c); }
            return new_str(o);
        });
    });

    register_native_module("tempfile", [](const var& m) {
        mod_def(m, "gettempdir", [](const CallArgs&) { return new_str(gettempdir_()); });
        mod_def(m, "mkdtemp", [](const CallArgs& a) {
            var suffix = arg(a, 0, "suffix", None), prefix = arg(a, 1, "prefix", None), dir = arg(a, 2, "dir", None);
            std::string d = dir.is_none() ? gettempdir_() : fs_path(dir);
            for (int i = 0; i < 100; i++) {
                std::string p = path_join({d, (prefix.is_none() ? "tmp" : str_utf8(prefix)) + rand_name() + (suffix.is_none() ? "" : str_utf8(suffix))});
                if (CreateDirectoryW(wide_(p).c_str(), nullptr)) return new_str(path_abspath(p));
                if (GetLastError() != ERROR_ALREADY_EXISTS) raise(win_error(GetLastError(), p));
            }
            raise(os_error(EEXIST, d));
        });
        mod_def(m, "mkstemp", [](const CallArgs& a) {
            var suffix = arg(a, 0, "suffix", None), prefix = arg(a, 1, "prefix", None), dir = arg(a, 2, "dir", None);
            std::string d = dir.is_none() ? gettempdir_() : fs_path(dir);
            for (int i = 0; i < 100; i++) {
                std::string p = path_join({d, (prefix.is_none() ? "tmp" : str_utf8(prefix)) + rand_name() + (suffix.is_none() ? "" : str_utf8(suffix))});
                int fd = _wopen(wide_(p).c_str(), _O_RDWR | _O_CREAT | _O_EXCL | _O_BINARY, _S_IREAD | _S_IWRITE);
                if (fd >= 0) return new_tuple({var((int64_t)fd), new_str(path_abspath(p))});
            }
            raise(os_error(EEXIST, d));
        });
        // TemporaryDirectory: a context manager whose name is a fresh directory, removed at exit
        static TypeObj* T_tmpdir = new_type("TemporaryDirectory", {}, Layout::NativeData, true, "tempfile");
        add_static(T_tmpdir, "__new__", [](const CallArgs& a) {
            var mk = import("tempfile").getattr("mkdtemp");
            CallArgs b;
            b.kw = a.kw;
            for (size_t i = 1; i < a.pos.size(); i++) b.pos.push_back(a.pos[i]);
            CallArgs c;
            for (auto& [k, v] : b.kw) if (str_utf8(k) != "ignore_cleanup_errors" && str_utf8(k) != "delete") c.kw.push_back({k, v});
            c.pos = b.pos;
            var name = call_obj(mk, c);
            auto* o = new DataObj();
            o->type = T_tmpdir;
            var ov(static_cast<Obj*>(o));
            ov.setattr("name", name);
            return ov;
        });
        add_method(T_tmpdir, "__init__", [](const CallArgs&) { return None; });
        add_method(T_tmpdir, "__enter__", [](const CallArgs& a) { return a.pos[0].getattr("name"); });
        add_method(T_tmpdir, "__exit__", [](const CallArgs& a) { rmtree_(str_utf8(a.pos[0].getattr("name")), true, None); return var(false); });
        add_method(T_tmpdir, "cleanup", [](const CallArgs& a) { rmtree_(str_utf8(a.pos[0].getattr("name")), true, None); return None; });
        mod_set(m, "TemporaryDirectory", tv(T_tmpdir));
        static TypeObj* T_ntf = new_type("_TemporaryFileWrapper", {}, Layout::NativeData, true, "tempfile");
        add_method(T_ntf, "__getattr__", [](const CallArgs& a) { return a.pos[0].getattr("file").getattr(a.pos.at(1)); });
        add_method(T_ntf, "__enter__", [](const CallArgs& a) { return a.pos[0]; });
        add_method(T_ntf, "__exit__", [](const CallArgs& a) {
            a.pos[0].getattr("file").call("close");
            if (a.pos[0].getattr("delete").truthy()) DeleteFileW(wide_(str_utf8(a.pos[0].getattr("name"))).c_str());
            return var(false);
        });
        add_method(T_ntf, "close", [](const CallArgs& a) {
            a.pos[0].getattr("file").call("close");
            if (a.pos[0].getattr("delete").truthy()) DeleteFileW(wide_(str_utf8(a.pos[0].getattr("name"))).c_str());
            return None;
        });
        auto ntf = [](const CallArgs& a, bool named) {
            var mode = kwarg(a, "mode", a.pos.empty() ? var("w+b"_s) : a.pos[0]);
            var suffix = kwarg(a, "suffix", None), prefix = kwarg(a, "prefix", None), dir = kwarg(a, "dir", None);
            bool del = kwarg(a, "delete", true).truthy();
            std::string d = dir.is_none() ? gettempdir_() : fs_path(dir);
            std::string p = path_join({d, (prefix.is_none() ? "tmp" : str_utf8(prefix)) + rand_name() + (suffix.is_none() ? "" : str_utf8(suffix))});
            std::string md = str_utf8(mode);
            if (md.find('w') != std::string::npos && md.find('x') == std::string::npos) {}
            var f = open_file(new_str(p), md, -1, kwarg(a, "encoding", None), None, kwarg(a, "newline", None));
            auto* o = new DataObj();
            o->type = T_ntf;
            var ov(static_cast<Obj*>(o));
            ov.setattr("file", f);
            ov.setattr("name", new_str(p));
            ov.setattr("delete", var(del || !named));
            return ov;
        };
        mod_def(m, "NamedTemporaryFile", [ntf](const CallArgs& a) { return ntf(a, true); });
        mod_def(m, "TemporaryFile", [ntf](const CallArgs& a) { return ntf(a, false); });
        mod_set(m, "tempdir", None);
    });

    register_native_module("subprocess", [](const var& m) {
        T_popen = new_type("Popen", {}, Layout::NativeData, true, "subprocess");
        add_static(T_popen, "__new__", popen_new);
        add_method(T_popen, "__init__", [](const CallArgs&) { return None; });
        add_method(T_popen, "communicate", [](const CallArgs& a) { return communicate(a.pos[0], arg(a, 1, "input", None), arg(a, 2, "timeout", None)); });
        add_method(T_popen, "wait", [](const CallArgs& a) {
            auto* p = static_cast<PopenObj*>(a.pos[0].ref());
            var timeout = arg(a, 1, "timeout", None);
            DWORD r;
            {
                gil_release g;
                r = WaitForSingleObject(p->proc, timeout.is_none() ? INFINITE : (DWORD)(timeout.as_double() * 1000));
            }
            if (r == WAIT_TIMEOUT) raise(make_exc_args(T(E_TimeoutExpired), {a.pos[0].getattr("args"), timeout}));
            DWORD code;
            GetExitCodeProcess(p->proc, &code);
            a.pos[0].setattr("returncode", var((int64_t)(int32_t)code));
            return a.pos[0].getattr("returncode");
        });
        add_method(T_popen, "poll", [](const CallArgs& a) {
            auto* p = static_cast<PopenObj*>(a.pos[0].ref());
            DWORD code;
            if (GetExitCodeProcess(p->proc, &code) && code != STILL_ACTIVE) a.pos[0].setattr("returncode", var((int64_t)(int32_t)code));
            return a.pos[0].getattr("returncode");
        });
        auto kill = [](const CallArgs& a) {
            auto* p = static_cast<PopenObj*>(a.pos[0].ref());
            TerminateProcess(p->proc, 1);
            return None;
        };
        add_method(T_popen, "kill", kill);
        add_method(T_popen, "terminate", kill);
        add_method(T_popen, "__enter__", [](const CallArgs& a) { return a.pos[0]; });
        add_method(T_popen, "__exit__", [](const CallArgs& a) {
            auto* p = static_cast<PopenObj*>(a.pos[0].ref());
            WaitForSingleObject(p->proc, INFINITE);
            return var(false);
        });
        mod_set(m, "Popen", tv(T_popen));
        // CompletedProcess(args, returncode, stdout=None, stderr=None)
        var ns = new_dict();
        ns.setitem("__init__"_s, def("__init__", "self, args, returncode, stdout=, stderr=", {None, None},
                                     [](var self, var args, var rc, var out, var err) {
                                         self.setattr("args", args);
                                         self.setattr("returncode", rc);
                                         self.setattr("stdout", out);
                                         self.setattr("stderr", err);
                                         return var(None);
                                     }));
        ns.setitem("check_returncode"_s, def("check_returncode", "self", {}, [](var self) {
            if (self.getattr("returncode").as_int64() != 0) {
                var x = make_exc_args(T(E_CalledProcessError), {self.getattr("returncode"), self.getattr("args")});
                x.setattr("returncode", self.getattr("returncode"));
                x.setattr("cmd", self.getattr("args"));
                raise(x);
            }
            return var(None);
        }));
        ns.setitem("__repr__"_s, def("__repr__", "self", {}, [](var self) {
            std::string r = "CompletedProcess(args=" + repr_(self.getattr("args")) + ", returncode=" + repr_(self.getattr("returncode"));
            if (!self.getattr("stdout").is_none()) r += ", stdout=" + repr_(self.getattr("stdout"));
            if (!self.getattr("stderr").is_none()) r += ", stderr=" + repr_(self.getattr("stderr"));
            return new_str(r + ")");
        }));
        var cp = make_class("CompletedProcess", "CompletedProcess", {}, ns, {}, "subprocess");
        T_completed = T(cp);
        mod_set(m, "CompletedProcess", cp);
        var se = make_class("SubprocessError", "SubprocessError", {t::Exception}, new_dict(), {}, "subprocess");
        E_SubprocessError = se;
        var ten = new_dict();
        ten.setitem("__str__"_s, def("__str__", "self", {}, [](var self) {
            return new_str("Command '" + repr_(self.getattr("cmd")).substr(0) + "' timed out after " + str_(self.getattr("timeout")) + " seconds");
        }));
        E_TimeoutExpired = make_class("TimeoutExpired", "TimeoutExpired", {se}, ten, {}, "subprocess");
        var cen = new_dict();
        cen.setitem("__str__"_s, def("__str__", "self", {}, [](var self) {
            return new_str("Command " + repr_(self.getattr("cmd")) + " returned non-zero exit status " + str_(self.getattr("returncode")) + ".");
        }));
        E_CalledProcessError = make_class("CalledProcessError", "CalledProcessError", {se}, cen, {}, "subprocess");
        mod_set(m, "SubprocessError", se);
        mod_set(m, "TimeoutExpired", E_TimeoutExpired);
        mod_set(m, "CalledProcessError", E_CalledProcessError);
        mod_set(m, "PIPE", PIPE);
        mod_set(m, "STDOUT", STDOUT);
        mod_set(m, "DEVNULL", DEVNULL);
        mod_set(m, "CREATE_NO_WINDOW", (int64_t)CREATE_NO_WINDOW);
        mod_set(m, "CREATE_NEW_PROCESS_GROUP", (int64_t)CREATE_NEW_PROCESS_GROUP);
        mod_def(m, "run", run);
        mod_def(m, "list2cmdline", [](const CallArgs& a) {
            std::vector<std::string> v;
            for (var x : each(a.pos.at(0))) v.push_back(fs_path(x));
            return new_str(list2cmdline(v));
        });
        auto simple = [](const CallArgs& a, int what) {
            CallArgs b;
            b.pos = {arg(a, 0, "args")};
            b.kw = a.kw;
            if (what >= 1) b.kw.push_back({"stdout"_s, var(PIPE)});
            if (what == 1) b.kw.push_back({"check"_s, true});
            var r = run(b);
            if (what == 0) return r.getattr("returncode");
            if (what == 2) return r.getattr("returncode");
            return r.getattr("stdout");
        };
        mod_def(m, "call", [simple](const CallArgs& a) { return simple(a, 0); });
        mod_def(m, "check_output", [simple](const CallArgs& a) { return simple(a, 1); });
        mod_def(m, "check_call", [](const CallArgs& a) {
            CallArgs b;
            b.pos = {arg(a, 0, "args")};
            b.kw = a.kw;
            b.kw.push_back({"check"_s, true});
            run(b);
            return var(0);
        });
    });

    register_native_module("time", [](const var& m) {
        mod_def(m, "time", [](const CallArgs&) {
            FILETIME ft;
            GetSystemTimePreciseAsFileTime(&ft);
            uint64_t v = ((uint64_t)ft.dwHighDateTime << 32) | ft.dwLowDateTime;
            return var((double)(v - 116444736000000000ULL) / 1e7);
        });
        mod_def(m, "time_ns", [](const CallArgs&) {
            FILETIME ft;
            GetSystemTimePreciseAsFileTime(&ft);
            uint64_t v = ((uint64_t)ft.dwHighDateTime << 32) | ft.dwLowDateTime;
            return var((int64_t)(v - 116444736000000000ULL) * 100);
        });
        auto perf = [](const CallArgs&) {
            static LARGE_INTEGER f = [] { LARGE_INTEGER x; QueryPerformanceFrequency(&x); return x; }();
            LARGE_INTEGER c;
            QueryPerformanceCounter(&c);
            return var((double)c.QuadPart / (double)f.QuadPart);
        };
        mod_def(m, "perf_counter", perf);
        mod_def(m, "monotonic", perf);
        mod_def(m, "process_time", [](const CallArgs&) {
            FILETIME c, e, k, u;
            GetProcessTimes(GetCurrentProcess(), &c, &e, &k, &u);
            auto t = [](FILETIME f) { return (double)(((uint64_t)f.dwHighDateTime << 32) | f.dwLowDateTime) / 1e7; };
            return var(t(k) + t(u));
        });
        mod_def(m, "perf_counter_ns", [](const CallArgs&) {
            static LARGE_INTEGER f = [] { LARGE_INTEGER x; QueryPerformanceFrequency(&x); return x; }();
            LARGE_INTEGER c;
            QueryPerformanceCounter(&c);
            return var((int64_t)((double)c.QuadPart * 1e9 / (double)f.QuadPart));
        });
        mod_def(m, "sleep", [](const CallArgs& a) {
            double s = a.pos.at(0).as_double();
            gil_release g;
            Sleep((DWORD)(s * 1000));
            return None;
        });
        static TypeObj* T_tm = new_type("struct_time", {T_tuple}, Layout::Tuple, true, "time");
        auto tm_of = [](const std::tm& t) {
            int yday = t.tm_yday + 1;
            int wday = (t.tm_wday + 6) % 7;
            var r = new_tuple({t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec, wday, yday, t.tm_isdst});
            r.ref()->type = T_tm;
            r.setattr("tm_year", t.tm_year + 1900);
            r.setattr("tm_mon", t.tm_mon + 1);
            r.setattr("tm_mday", t.tm_mday);
            r.setattr("tm_hour", t.tm_hour);
            r.setattr("tm_min", t.tm_min);
            r.setattr("tm_sec", t.tm_sec);
            r.setattr("tm_wday", wday);
            r.setattr("tm_yday", yday);
            return r;
        };
        mod_def(m, "localtime", [tm_of](const CallArgs& a) {
            __time64_t t = a.pos.empty() || a.pos[0].is_none() ? _time64(nullptr) : (__time64_t)a.pos[0].as_double();
            std::tm tm;
            _localtime64_s(&tm, &t);
            return tm_of(tm);
        });
        mod_def(m, "gmtime", [tm_of](const CallArgs& a) {
            __time64_t t = a.pos.empty() || a.pos[0].is_none() ? _time64(nullptr) : (__time64_t)a.pos[0].as_double();
            std::tm tm;
            _gmtime64_s(&tm, &t);
            return tm_of(tm);
        });
        mod_def(m, "strftime", [](const CallArgs& a) {
            std::tm tm{};
            if (a.pos.size() > 1) {
                auto v = collect(a.pos[1]);
                tm.tm_year = (int)v[0].as_int64() - 1900;
                tm.tm_mon = (int)v[1].as_int64() - 1;
                tm.tm_mday = (int)v[2].as_int64();
                tm.tm_hour = (int)v[3].as_int64();
                tm.tm_min = (int)v[4].as_int64();
                tm.tm_sec = (int)v[5].as_int64();
                tm.tm_wday = ((int)v[6].as_int64() + 1) % 7;
                tm.tm_yday = (int)v[7].as_int64() - 1;
                tm.tm_isdst = v.size() > 8 ? (int)v[8].as_int64() : -1;
            } else {
                __time64_t t = _time64(nullptr);
                _localtime64_s(&tm, &t);
            }
            std::wstring f = wide_(str_utf8(a.pos.at(0)));
            wchar_t buf[1024];
            size_t n = wcsftime(buf, 1024, f.c_str(), &tm);
            return new_str(narrow(std::wstring(buf, n)));
        });
        mod_def(m, "mktime", [](const CallArgs& a) {
            auto v = collect(a.pos.at(0));
            std::tm tm{};
            tm.tm_year = (int)v[0].as_int64() - 1900;
            tm.tm_mon = (int)v[1].as_int64() - 1;
            tm.tm_mday = (int)v[2].as_int64();
            tm.tm_hour = (int)v[3].as_int64();
            tm.tm_min = (int)v[4].as_int64();
            tm.tm_sec = (int)v[5].as_int64();
            tm.tm_isdst = -1;
            return var((double)_mktime64(&tm));
        });
        mod_def(m, "ctime", [](const CallArgs& a) {
            __time64_t t = a.pos.empty() || a.pos[0].is_none() ? _time64(nullptr) : (__time64_t)a.pos[0].as_double();
            char buf[64];
            _ctime64_s(buf, sizeof buf, &t);
            std::string s = buf;
            while (!s.empty() && s.back() == '\n') s.pop_back();
            return new_str(s);
        });
        mod_set(m, "timezone", (int64_t)_timezone);
    });

    register_native_module("errno", [](const var& m) {
        for (auto& [n, v] : std::vector<std::pair<const char*, int>>{{"ENOENT", ENOENT}, {"EEXIST", EEXIST}, {"EACCES", EACCES}, {"EPERM", EPERM},
                                                                    {"EISDIR", EISDIR}, {"ENOTDIR", ENOTDIR}, {"EINVAL", EINVAL},
                                                                    {"ENOTEMPTY", ENOTEMPTY}, {"EAGAIN", EAGAIN}, {"EINTR", EINTR}, {"EPIPE", EPIPE},
                                                                    {"EBADF", EBADF}, {"ENOSPC", ENOSPC}, {"EXDEV", EXDEV}, {"ESRCH", ESRCH}})
            mod_set(m, n, v);
    });
    register_native_module("stat", [](const var& m) {
        mod_set(m, "S_IFDIR", 0040000);
        mod_set(m, "S_IFREG", 0100000);
        mod_set(m, "S_IWRITE", 0200);
        mod_set(m, "S_IREAD", 0400);
        mod_set(m, "S_IEXEC", 0100);
        mod_set(m, "S_IWUSR", 0200);
        mod_set(m, "S_IRUSR", 0400);
        mod_set(m, "S_IXUSR", 0100);
        mod_def(m, "S_ISDIR", [](const CallArgs& a) { return var((a.pos.at(0).as_int64() & 0170000) == 0040000); });
        mod_def(m, "S_ISREG", [](const CallArgs& a) { return var((a.pos.at(0).as_int64() & 0170000) == 0100000); });
    });
    register_native_module("platform", [](const var& m) {
        mod_def(m, "system", [](const CallArgs&) { return "Windows"_s; });
        mod_def(m, "machine", [](const CallArgs&) { return "AMD64"_s; });
        mod_def(m, "python_implementation", [](const CallArgs&) { return "CPython"_s; });
        mod_def(m, "node", [](const CallArgs&) {
            wchar_t buf[256];
            DWORD n = 256;
            GetComputerNameW(buf, &n);
            return new_str(narrow(std::wstring(buf, n)));
        });
    });
}

}  // namespace py
