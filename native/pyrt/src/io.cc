// io.cc -- files and streams: open() in text and binary modes (universal newlines on read, \n
// written as \r\n in text mode as on Windows, encodings through the codecs), the file object's
// methods and iteration and with-protocol, io.StringIO / io.BytesIO, and sys (argv, path, modules,
// stdout / stderr / stdin, exit, executable, platform, getrecursionlimit ...).
#include <windows.h>
#include <io.h>
#include <fcntl.h>

#include <cerrno>
#include <cstdio>
#include <cstring>

#include "rt.hpp"

namespace py {

var bytes_decode_(const std::string& raw, const std::string& enc, const std::string& errors);
var str_encode_(const std::string& s, const std::string& enc, const std::string& errors);
var os_error(int err, const std::string& filename, const std::string& filename2 = "");
extern var g_sys_modules;
extern std::vector<std::string> g_argv;
extern int g_recursion_limit;
var g_sys;

static std::wstring wide(const std::string& s) {
    int n = MultiByteToWideChar(CP_UTF8, 0, s.data(), (int)s.size(), nullptr, 0);
    std::wstring w((size_t)n, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, s.data(), (int)s.size(), w.data(), n);
    return w;
}
std::wstring wide_(const std::string& s) { return wide(s); }

// ------------------------------------------------------------------------------------------ the file object
static TypeObj *T_file, *T_stringio, *T_bytesio;
struct FileObj : DataObj {
    FILE* fp = nullptr;
    bool binary = false, readable = false, writable = false, closed = false, owned = true;
    std::string name, mode, encoding = "utf-8", errors = "strict";
    var newline;                // None: universal
    bool console = false;       // stdout / stderr: written through as UTF-8
    std::string pending;        // (text read-ahead)
    ~FileObj() override {
        if (fp && owned && !closed) fclose(fp);
    }
};
// io.StringIO / BytesIO
struct MemObj : DataObj {
    std::string buf;
    size_t pos = 0;
    bool binary = false, closed = false;
};
static FileObj* F(const var& v) {
    if (!v.is_ref() || v.ref()->type != T_file) raise(E_TypeError, "not a file");
    return static_cast<FileObj*>(v.ref());
}
static void check_open(FileObj* f) {
    if (f->closed) raise(E_ValueError, "I/O operation on closed file.");
}
static std::string read_all(FILE* fp) {
    std::string out;
    char buf[65536];
    size_t n;
    while ((n = fread(buf, 1, sizeof buf, fp)) > 0) out.append(buf, n);
    return out;
}
static std::string universal(const std::string& s) {
    std::string o;
    o.reserve(s.size());
    for (size_t i = 0; i < s.size(); i++) {
        if (s[i] == '\r') {
            o.push_back('\n');
            if (i + 1 < s.size() && s[i + 1] == '\n') i++;
        } else o.push_back(s[i]);
    }
    return o;
}
static var decode_text(FileObj* f, const std::string& raw) {
    var s = bytes_decode_(raw, f->encoding, f->errors);
    if (f->newline.is_none()) return new_str(universal(str_utf8(s)));
    return s;
}

var open_file(const var& file, const std::string& mode, int64_t buffering, const var& encoding, const var& errors,
              const var& newline) {
    (void)buffering;
    std::string path = fs_path(file);
    bool b = mode.find('b') != std::string::npos;
    char kind = 0;
    bool plus = mode.find('+') != std::string::npos;
    for (char c : mode) if (c == 'r' || c == 'w' || c == 'a' || c == 'x') kind = c;
    if (!kind) kind = 'r';
    std::wstring wm;
    if (kind == 'r') wm = plus ? L"r+b" : L"rb";
    else if (kind == 'w') wm = plus ? L"w+b" : L"wb";
    else if (kind == 'a') wm = plus ? L"a+b" : L"ab";
    else wm = plus ? L"w+bx" : L"wbx";
    if (path.find('\0') != std::string::npos) raise(E_ValueError, "embedded null character in path");
    std::wstring wp = wide(path);
    // a directory: IsADirectoryError / PermissionError as CPython on Windows
    DWORD attrs = GetFileAttributesW(wp.c_str());
    if (attrs != INVALID_FILE_ATTRIBUTES && (attrs & FILE_ATTRIBUTE_DIRECTORY)) raise(os_error(EACCES, path));
    FILE* fp = _wfopen(wp.c_str(), wm.c_str());
    if (!fp) {
        int e = errno;
        if (kind == 'x' && attrs != INVALID_FILE_ATTRIBUTES) e = EEXIST;
        raise(os_error(e, path));
    }
    auto* f = new FileObj();
    f->type = T_file;
    f->fp = fp;
    f->binary = b;
    f->readable = kind == 'r' || plus;
    f->writable = kind != 'r' || plus;
    f->name = path;
    f->mode = mode;
    if (!encoding.is_none() && !encoding.is_unbound()) f->encoding = str_utf8(encoding);
    if (!errors.is_none() && !errors.is_unbound()) f->errors = str_utf8(errors);
    f->newline = newline.is_unbound() ? var(None) : newline;
    var r(static_cast<Obj*>(f));
    if (!f->binary) r.setattr("name", file);
    else r.setattr("name", file);
    return r;
}

static void file_write_raw(FileObj* f, const std::string& s) {
    if (f->console) {
        HANDLE h = GetStdHandle(f->fp == stderr ? STD_ERROR_HANDLE : STD_OUTPUT_HANDLE);
        DWORD mode;
        if (GetConsoleMode(h, &mode)) {
            std::wstring w = wide(s);
            DWORD n;
            WriteConsoleW(h, w.data(), (DWORD)w.size(), &n, nullptr);
            return;
        }
    }
    if (fwrite(s.data(), 1, s.size(), f->fp) != s.size()) raise(os_error(errno, f->name));
}
static var file_write(FileObj* f, const var& data) {
    check_open(f);
    if (!f->writable) raise(make_exc(T(builtin("OSError")), "not writable"));
    if (f->binary) {
        const std::string& raw = raw_of(data);
        file_write_raw(f, raw);
        return var((int64_t)raw.size());
    }
    if (!is_str(data)) raise(E_TypeError, "write() argument must be str, not " + tname(data));
    const std::string& s = str_utf8(data);
    std::string nl = f->newline.is_none() ? "\r\n" : str_utf8(f->newline);
    if (f->console) nl = "\n";
    std::string text;
    if (nl == "\n" || nl.empty()) text = s;
    else {
        text.reserve(s.size() + s.size() / 16);
        for (char c : s) {
            if (c == '\n') text += nl;
            else text.push_back(c);
        }
    }
    file_write_raw(f, raw_of(str_encode_(text, f->encoding, f->errors)));
    return var(cp_count(s));
}
static std::string read_bytes(FileObj* f, int64_t n) {
    if (n < 0) return read_all(f->fp);
    std::string out((size_t)n, '\0');
    size_t got = fread(out.data(), 1, (size_t)n, f->fp);
    out.resize(got);
    return out;
}
static var file_read(FileObj* f, int64_t n) {
    check_open(f);
    if (!f->readable) raise(make_exc(T(builtin("OSError")), "not readable"));
    if (f->binary) return new_bytes(read_bytes(f, n));
    if (n < 0) {
        std::string raw = f->pending + read_all(f->fp);
        f->pending.clear();
        return decode_text(f, raw);
    }
    // n characters: read whole UTF-8 sequences until n code points
    std::string raw = f->pending;
    f->pending.clear();
    while (cp_count(universal(raw)) < n) {
        int c = fgetc(f->fp);
        if (c == EOF) break;
        raw.push_back((char)c);
    }
    return decode_text(f, raw);
}
static bool read_line_raw(FileObj* f, std::string& line) {
    line.clear();
    int c;
    while ((c = fgetc(f->fp)) != EOF) {
        line.push_back((char)c);
        if (f->binary) { if (c == '\n') break; continue; }
        if (c == '\n') break;
        if (c == '\r') {
            int d = fgetc(f->fp);
            if (d == '\n') line.push_back('\n');
            else if (d != EOF) ungetc(d, f->fp);
            break;
        }
    }
    return !line.empty();
}
static var file_readline(FileObj* f) {
    check_open(f);
    std::string line;
    read_line_raw(f, line);
    if (f->binary) return new_bytes(line);
    return decode_text(f, line);
}

static var mem_new(bool binary, const var& init) {
    auto* m = new MemObj();
    m->type = binary ? T_bytesio : T_stringio;
    m->binary = binary;
    if (!init.is_unbound() && !init.is_none()) m->buf = binary ? raw_of(init) : str_utf8(init);
    return var(static_cast<Obj*>(m));
}
static MemObj* M(const var& v) { return static_cast<MemObj*>(v.ref()); }

// ------------------------------------------------------------------------------------------ sys.stdout
static var std_stream(FILE* fp, const char* name, bool write) {
    auto* f = new FileObj();
    f->type = T_file;
    f->fp = fp;
    f->owned = false;
    f->readable = !write;
    f->writable = write;
    f->name = name;
    f->mode = write ? "w" : "r";
    f->console = write;
    f->newline = "\n"_s;
    return var(static_cast<Obj*>(f));
}
void flush_std_() {
    fflush(stdout);
    fflush(stderr);
}

void print_exception_(const var& e) {
    flush_std_();
    std::string msg;
    var ctx = getattr(e, interned("__cause__"), None);
    if (!ctx.is_none()) {
        print_exception_(ctx);
        fputs("\nThe above exception was the direct cause of the following exception:\n\n", stderr);
    }
    fputs("Traceback (most recent call last):\n  (translated code)\n", stderr);
    std::string s;
    try {
        s = str_(e);
    } catch (...) {
        s = "<exception str() failed>";
    }
    TypeObj* t = type_of(e);
    std::string name = t->module == "builtins" ? t->qualname : t->module + "." + t->qualname;
    if (s.empty()) fprintf(stderr, "%s\n", name.c_str());
    else fprintf(stderr, "%s: %s\n", name.c_str(), s.c_str());
    fflush(stderr);
}

bool module_registered_(const std::string& name);
void set_sys_argv_() {
    std::vector<var> a;
    for (size_t i = 0; i < g_argv.size(); i++) a.push_back(new_str(g_argv[i]));
    mod_set(g_sys, "argv", new_list(a));
    mod_set(g_sys, "orig_argv", new_list(a));
}

void init_io() {
    T_file = new_type("TextIOWrapper", {}, Layout::NativeData, true, "_io");
    T_stringio = new_type("StringIO", {}, Layout::NativeData, true, "_io");
    T_bytesio = new_type("BytesIO", {}, Layout::NativeData, true, "_io");
    TypeObj* t = T_file;
    add_method(t, "write", [](const CallArgs& a) { return file_write(F(a.pos[0]), a.pos.at(1)); });
    add_method(t, "writelines", [](const CallArgs& a) {
        for (var x : each(a.pos.at(1))) file_write(F(a.pos[0]), x);
        return None;
    });
    add_method(t, "read", [](const CallArgs& a) {
        var n = arg(a, 1, "size", -1);
        return file_read(F(a.pos[0]), n.is_none() ? -1 : n.as_int64());
    });
    add_method(t, "readline", [](const CallArgs& a) { return file_readline(F(a.pos[0])); });
    add_method(t, "readlines", [](const CallArgs& a) {
        std::vector<var> out;
        for (;;) {
            var l = file_readline(F(a.pos[0]));
            if (!l) break;
            out.push_back(l);
        }
        return new_list(out);
    });
    add_method(t, "__iter__", [](const CallArgs& a) {
        var self = a.pos[0];
        return new_iter([self](var& out) {
            out = file_readline(F(self));
            return out.truthy();
        });
    });
    add_method(t, "__next__", [](const CallArgs& a) {
        var l = file_readline(F(a.pos[0]));
        if (!l) raise_stop(None);
        return l;
    });
    add_method(t, "seek", [](const CallArgs& a) {
        FileObj* f = F(a.pos[0]);
        check_open(f);
        int64_t off = a.pos.at(1).as_int64();
        int whence = a.pos.size() > 2 ? (int)a.pos[2].as_int64() : 0;
        f->pending.clear();
        if (_fseeki64(f->fp, off, whence) != 0) raise(os_error(errno, ""));
        return var((int64_t)_ftelli64(f->fp));
    });
    add_method(t, "tell", [](const CallArgs& a) { FileObj* f = F(a.pos[0]); check_open(f); return var((int64_t)_ftelli64(f->fp)); });
    add_method(t, "truncate", [](const CallArgs& a) {
        FileObj* f = F(a.pos[0]);
        fflush(f->fp);
        int64_t at = a.pos.size() > 1 && !a.pos[1].is_none() ? a.pos[1].as_int64() : _ftelli64(f->fp);
        _chsize_s(_fileno(f->fp), at);
        return var(at);
    });
    add_method(t, "flush", [](const CallArgs& a) { FileObj* f = F(a.pos[0]); if (!f->closed) fflush(f->fp); return None; });
    add_method(t, "close", [](const CallArgs& a) {
        FileObj* f = F(a.pos[0]);
        if (!f->closed && f->owned && f->fp) fclose(f->fp);
        f->closed = true;
        return None;
    });
    add_method(t, "fileno", [](const CallArgs& a) { return var((int64_t)_fileno(F(a.pos[0])->fp)); });
    add_method(t, "isatty", [](const CallArgs& a) { return var(_isatty(_fileno(F(a.pos[0])->fp)) != 0); });
    add_method(t, "readable", [](const CallArgs& a) { return var(F(a.pos[0])->readable); });
    add_method(t, "writable", [](const CallArgs& a) { return var(F(a.pos[0])->writable); });
    add_method(t, "seekable", [](const CallArgs&) { return var(true); });
    add_method(t, "__enter__", [](const CallArgs& a) { check_open(F(a.pos[0])); return a.pos[0]; });
    add_method(t, "__exit__", [](const CallArgs& a) {
        FileObj* f = F(a.pos[0]);
        if (!f->closed && f->owned && f->fp) fclose(f->fp);
        f->closed = true;
        return var(false);
    });
    add_method(t, "reconfigure", [](const CallArgs& a) {
        FileObj* f = F(a.pos[0]);
        var e = kwarg(a, "encoding");
        if (!e.is_unbound()) f->encoding = str_utf8(e);
        var er = kwarg(a, "errors");
        if (!er.is_unbound()) f->errors = str_utf8(er);
        return None;
    });
    add_getter(t, "closed", [](const var& s) { return var(F(s)->closed); });
    add_getter(t, "mode", [](const var& s) { return new_str(F(s)->mode); });
    add_getter(t, "encoding", [](const var& s) { return new_str(F(s)->encoding); });
    add_getter(t, "buffer", [](const var& s) {
        // the binary layer of a text stream: the same FILE*, bytes in and out
        FileObj* f = F(s);
        auto* b = new FileObj();
        b->type = T_file;
        b->fp = f->fp;
        b->owned = false;
        b->binary = true;
        b->readable = f->readable;
        b->writable = f->writable;
        b->name = f->name;
        b->mode = f->mode + "b";
        return var(static_cast<Obj*>(b));
    });

    for (TypeObj* mt : {T_stringio, T_bytesio}) {
        bool binary = mt == T_bytesio;
        add_static(mt, "__new__", [binary](const CallArgs& a) { return mem_new(binary, a.pos.size() > 1 ? a.pos[1] : kwarg(a, binary ? "initial_bytes" : "initial_value")); });
        add_method(mt, "__init__", [](const CallArgs&) { return None; });
        add_method(mt, "write", [binary](const CallArgs& a) {
            MemObj* m = M(a.pos[0]);
            if (m->closed) raise(E_ValueError, "I/O operation on closed file.");
            std::string s = binary ? raw_of(a.pos.at(1)) : str_utf8(a.pos.at(1));
            if (m->pos > m->buf.size()) m->buf.resize(m->pos, '\0');
            m->buf.replace(m->pos, std::min(s.size(), m->buf.size() - m->pos), s);
            m->pos += s.size();
            return var(binary ? (int64_t)s.size() : cp_count(s));
        });
        add_method(mt, "getvalue", [binary](const CallArgs& a) { return binary ? new_bytes(M(a.pos[0])->buf) : new_str(M(a.pos[0])->buf); });
        add_method(mt, "read", [binary](const CallArgs& a) {
            MemObj* m = M(a.pos[0]);
            int64_t n = a.pos.size() > 1 && !a.pos[1].is_none() ? a.pos[1].as_int64() : -1;
            size_t start = std::min(m->pos, m->buf.size());
            size_t take = n < 0 ? m->buf.size() - start : std::min((size_t)n, m->buf.size() - start);
            if (!binary && n >= 0) {
                // n characters
                size_t e = start;
                int64_t k = 0;
                while (e < m->buf.size() && k < n) { e++; while (e < m->buf.size() && ((unsigned char)m->buf[e] & 0xC0) == 0x80) e++; k++; }
                take = e - start;
            }
            std::string s = m->buf.substr(start, take);
            m->pos = start + take;
            return binary ? new_bytes(s) : new_str(s);
        });
        add_method(mt, "readline", [binary](const CallArgs& a) {
            MemObj* m = M(a.pos[0]);
            size_t start = std::min(m->pos, m->buf.size());
            size_t e = m->buf.find('\n', start);
            e = e == std::string::npos ? m->buf.size() : e + 1;
            m->pos = e;
            std::string s = m->buf.substr(start, e - start);
            return binary ? new_bytes(s) : new_str(s);
        });
        add_method(mt, "readlines", [](const CallArgs& a) {
            std::vector<var> out;
            for (;;) {
                var l = a.pos[0].call("readline");
                if (!l) break;
                out.push_back(l);
            }
            return new_list(out);
        });
        add_method(mt, "__iter__", [](const CallArgs& a) {
            var self = a.pos[0];
            return new_iter([self](var& out) { out = self.call("readline"); return out.truthy(); });
        });
        add_method(mt, "seek", [](const CallArgs& a) {
            MemObj* m = M(a.pos[0]);
            int64_t off = a.pos.at(1).as_int64();
            int whence = a.pos.size() > 2 ? (int)a.pos[2].as_int64() : 0;
            int64_t base = whence == 0 ? 0 : whence == 1 ? (int64_t)m->pos : (int64_t)m->buf.size();
            m->pos = (size_t)std::max<int64_t>(0, base + off);
            return var((int64_t)m->pos);
        });
        add_method(mt, "tell", [](const CallArgs& a) { return var((int64_t)M(a.pos[0])->pos); });
        add_method(mt, "truncate", [](const CallArgs& a) {
            MemObj* m = M(a.pos[0]);
            size_t at = a.pos.size() > 1 && !a.pos[1].is_none() ? (size_t)a.pos[1].as_int64() : m->pos;
            if (at < m->buf.size()) m->buf.resize(at);
            return var((int64_t)at);
        });
        add_method(mt, "getbuffer", [](const CallArgs& a) { return new_bytes(M(a.pos[0])->buf); });
        add_method(mt, "flush", [](const CallArgs&) { return None; });
        add_method(mt, "close", [](const CallArgs& a) { M(a.pos[0])->closed = true; return None; });
        add_method(mt, "isatty", [](const CallArgs&) { return var(false); });
        add_method(mt, "readable", [](const CallArgs&) { return var(true); });
        add_method(mt, "writable", [](const CallArgs&) { return var(true); });
        add_method(mt, "seekable", [](const CallArgs&) { return var(true); });
        add_method(mt, "__enter__", [](const CallArgs& a) { return a.pos[0]; });
        add_method(mt, "__exit__", [](const CallArgs& a) { M(a.pos[0])->closed = true; return var(false); });
        add_getter(mt, "closed", [](const var& s) { return var(M(s)->closed); });
    }

    var builtins = builtin("print").is_unbound() ? var() : var();
    (void)builtins;
    extern var g_builtins;
    mod_def(g_builtins, "open", [](const CallArgs& a) {
        var mode = arg(a, 1, "mode", "r"_s);
        return open_file(arg(a, 0, "file"), str_utf8(mode), arg(a, 2, "buffering", -1).as_int64(), arg(a, 3, "encoding", None),
                         arg(a, 4, "errors", None), arg(a, 5, "newline", None));
    });

    // sys
    auto* m = new ModuleObj();
    m->type = T_module;
    m->name = "sys";
    g_sys = var(static_cast<Obj*>(m));
    mod_set(g_sys, "__name__", "sys"_s);
    // stdout / stderr in binary mode: text written as UTF-8 with \n kept
    _setmode(_fileno(stdout), _O_BINARY);
    _setmode(_fileno(stderr), _O_BINARY);
    mod_set(g_sys, "stdout", std_stream(stdout, "<stdout>", true));
    mod_set(g_sys, "stderr", std_stream(stderr, "<stderr>", true));
    mod_set(g_sys, "stdin", std_stream(stdin, "<stdin>", false));
    mod_set(g_sys, "__stdout__", g_sys.getattr("stdout"));
    mod_set(g_sys, "__stderr__", g_sys.getattr("stderr"));
    mod_set(g_sys, "argv", new_list());
    mod_set(g_sys, "path", new_list());
    mod_set(g_sys, "platform", "win32"_s);
    mod_set(g_sys, "byteorder", "little"_s);
    mod_set(g_sys, "maxsize", var(INT64_MAX));
    mod_set(g_sys, "version", "3.14.0 (CodeBreaker pyrt)"_s);
    {
        // sys.version_info: a tuple with names (major, minor ...), as CPython's struct sequence
        TypeObj* vt = new_type("version_info", {T_tuple}, Layout::Tuple, false, "sys");
        var vi = new_tuple({3, 14, 0, "final"_s, 0});
        vi.ref()->type = vt;
        const char* names[] = {"major", "minor", "micro", "releaselevel", "serial"};
        for (int i = 0; i < 5; i++) vi.setattr(names[i], vi[var(i)]);
        mod_set(g_sys, "version_info", vi);
        mod_set(g_sys, "hexversion", var((int64_t)0x030E00F0));
    }
    {
        wchar_t buf[MAX_PATH];
        GetModuleFileNameW(nullptr, buf, MAX_PATH);
        int len = WideCharToMultiByte(CP_UTF8, 0, buf, -1, nullptr, 0, nullptr, nullptr);
        std::string s((size_t)(len - 1), '\0');
        WideCharToMultiByte(CP_UTF8, 0, buf, -1, s.data(), len, nullptr, nullptr);
        mod_set(g_sys, "executable", new_str(s));
    }
    if (g_sys_modules.is_unbound()) g_sys_modules = new_dict_obj();
    mod_set(g_sys, "modules", g_sys_modules);
    {
        // sys.flags (every flag off: a translated program takes no interpreter options) and sys.implementation
        auto ns = [](const char* name) {
            auto* mo = new ModuleObj();
            mo->type = T_module;
            mo->name = name;
            return var(static_cast<Obj*>(mo));
        };
        var flags = ns("sys.flags");
        for (const char* f : {"debug", "inspect", "interactive", "optimize", "dont_write_bytecode", "no_user_site", "no_site",
                              "ignore_environment", "verbose", "bytes_warning", "quiet", "hash_randomization", "isolated",
                              "dev_mode", "utf8_mode", "warn_default_encoding", "safe_path", "int_max_str_digits", "gil",
                              "thread_inherit_context", "context_aware_warnings"})
            mod_set(flags, f, var(0));
        mod_set(g_sys, "flags", flags);
        var impl = ns("sys.implementation");
        mod_set(impl, "name", "cpython"_s);
        mod_set(impl, "cache_tag", None);
        mod_set(impl, "version", new_tuple({3, 14, 0, "final"_s, 0}));
        mod_set(g_sys, "implementation", impl);
    }
    mod_def(g_sys, "exit", [](const CallArgs& a) -> var { raise(make_exc_args(E_SystemExit, a.pos.empty() ? std::vector<var>{} : std::vector<var>{a.pos[0]})); });
    mod_def(g_sys, "getrecursionlimit", [](const CallArgs&) { return var(g_recursion_limit); });
    mod_def(g_sys, "setrecursionlimit", [](const CallArgs& a) { g_recursion_limit = (int)a.pos.at(0).as_int64(); return None; });
    mod_def(g_sys, "exc_info", [](const CallArgs&) {
        var e = current_exception();
        if (e.is_none()) return new_tuple({None, None, None});
        return new_tuple({tv(type_of(e)), e, None});
    });
    mod_def(g_sys, "exception", [](const CallArgs&) { return current_exception(); });
    mod_def(g_sys, "getdefaultencoding", [](const CallArgs&) { return "utf-8"_s; });
    mod_def(g_sys, "getfilesystemencoding", [](const CallArgs&) { return "utf-8"_s; });
    mod_def(g_sys, "intern", [](const CallArgs& a) { return a.pos.at(0); });
    mod_def(g_sys, "getsizeof", [](const CallArgs&) { return var(64); });
    mod_def(g_sys, "audit", [](const CallArgs&) { return None; });
    mod_def(g_sys, "addaudithook", [](const CallArgs&) { return None; });
    mod_def(g_sys, "getfilesystemencodeerrors", [](const CallArgs&) { return "surrogatepass"_s; });
    mod_def(g_sys, "is_finalizing", [](const CallArgs&) { return var(false); });
    mod_def(g_sys, "_getframe", [](const CallArgs&) -> var { raise(E_NotImplementedError, "sys._getframe: frame introspection is not available in translated code"); });
    static_cast<DictObj*>(g_sys_modules.ref())->set("sys"_s, g_sys);
    extern var g_builtins;
    static_cast<DictObj*>(g_sys_modules.ref())->set("builtins"_s, g_builtins);

    // io
    register_native_module("io", [](const var& mod) {
        mod_set(mod, "StringIO", tv(T_stringio));
        mod_set(mod, "BytesIO", tv(T_bytesio));
        mod_set(mod, "TextIOWrapper", tv(T_file));
        mod_set(mod, "open", builtin("open"));
        mod_set(mod, "SEEK_SET", 0);
        mod_set(mod, "SEEK_CUR", 1);
        mod_set(mod, "SEEK_END", 2);
        mod_set(mod, "UnsupportedOperation", t::OSError);
        mod_set(mod, "DEFAULT_BUFFER_SIZE", 8192);
        mod_set(mod, "Reader", t::object);
        mod_set(mod, "Writer", t::object);
        mod_set(mod, "__all__", new_list({"BlockingIOError"_s, "open"_s, "open_code"_s, "IOBase"_s, "RawIOBase"_s, "FileIO"_s,
                                          "BytesIO"_s, "StringIO"_s, "BufferedIOBase"_s, "BufferedReader"_s, "BufferedWriter"_s,
                                          "BufferedRWPair"_s, "BufferedRandom"_s, "TextIOBase"_s, "TextIOWrapper"_s,
                                          "UnsupportedOperation"_s, "SEEK_SET"_s, "SEEK_CUR"_s, "SEEK_END"_s, "DEFAULT_BUFFER_SIZE"_s,
                                          "text_encoding"_s, "IncrementalNewlineDecoder"_s, "Reader"_s, "Writer"_s}));
        mod_def(mod, "text_encoding", [](const CallArgs& a) {
            var e = a.pos.empty() ? var(None) : a.pos[0];
            return e.is_none() ? "utf-8"_s : e;
        });
        mod_def(mod, "open_code", [](const CallArgs& a) { return builtin("open")(a.pos.at(0), "rb"_s); });
        // the stream classes Python code derives from or wraps (IOBase ... BufferedReader, FileIO): CPython's
        // own, from _pyio (translated with the program when stdlib.txt lists it); open/StringIO/BytesIO
        // stay the runtime's
        mod_def(mod, "__getattr__", [](const CallArgs& a) -> var {
            std::string n = str_utf8(a.pos.at(0));
            static const char* from_pyio[] = {"IOBase", "RawIOBase", "BufferedIOBase", "TextIOBase", "BufferedReader", "BufferedWriter",
                                              "BufferedRandom", "BufferedRWPair", "FileIO", "IncrementalNewlineDecoder"};
            for (const char* k : from_pyio)
                if (n == k) {
                    if (!module_registered_("_pyio")) raise(E_AttributeError, "module 'io' has no attribute '" + n + "' (_pyio is not in this program)");
                    var v = import("_pyio").getattr(k);
                    mod_set(import("io"), k, v);
                    return v;
                }
            raise(E_AttributeError, "module 'io' has no attribute '" + n + "'");
        });
    });
}

}  // namespace py
