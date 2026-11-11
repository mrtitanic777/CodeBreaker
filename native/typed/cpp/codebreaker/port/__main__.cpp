// codebreaker/port/__main__.cpp -- typed C++ port of codebreaker.port.__main__ (see __main__.hpp).
//
// python -m codebreaker.port SOURCE_DIR OUT_DIR [--compiler gcc|clang|msvc] [--runtime NAME] [--cc PATH]
//
// SOURCE_DIR is read, never written: its C (*.c, *.h, in it or its src/) and its assembler modules (*.asm, in it or
// its asm/). OUT_DIR gets the port: the modern sources, the platform layer, a build script, the program built, and
// PORT.md - every change made and why, what the compiler still says, what could not be ported.

#include "codebreaker/port/__main__.hpp"

#include <algorithm>
#include <atomic>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <map>
#include <mutex>
#include <thread>

#include "py_json.hpp"
#include "py_subprocess.hpp"
#include "json.hpp"
#include "value.hpp"

#include "codebreaker/base/cache.hpp"
#include "codebreaker/base/ctext.hpp"
#include "codebreaker/base/parallel.hpp"
#include "codebreaker/jobs/pool.hpp"
#include "codebreaker/port/arith16.hpp"
#include "codebreaker/port/asmdata.hpp"
#include "codebreaker/port/oldlayout.hpp"
#include "codebreaker/port/sanitize.hpp"
#include "codebreaker/port/verify.hpp"

#ifndef PSAPI_VERSION
#define PSAPI_VERSION 2
#endif
#include <psapi.h>
#undef near
#undef far

namespace codebreaker::port::__main__ {

using base::ctext::blank;
using py::dict;
using py::list;
using py::set;
using py::str;
namespace fs = std::filesystem;



namespace {

str S(const char* s) { return str(s); }
// the Python's "%..." % args
template <class... A> str F(const char* f, const A&... a) { return py::percent(str(f), a...); }
str esc(const str& s) { return py::re::escape(s); }
str ws1(const str& x) { return py::re::sub(S(R"(\s+)"), S(" "), x); }
// x.replace(a, b, 1)
str replace1(const str& x, const str& a, const str& b) {
    int64_t at = x.find(a);
    if (at < 0) return x;
    return x.slice(0, at) + b + x.slice(at + a.size(), x.size());
}
str sl(const str& s, int64_t a, int64_t b) { return s.slice(a, b); }
bool fullm(const char* pat, const str& s) { return static_cast<bool>(py::re::fullmatch(S(pat), s)); }
int64_t to_int(const str& s) { return py::int_parse(s, 10); }
str stars_of(int64_t n) { return str(std::string(static_cast<size_t>(std::max<int64_t>(n, 0)), '*')); }
str join(const str& sep, const list<str>& l) { return py::join(sep, l); }

// json.load(open(path)) (a text file as Python opens it here: cp1252)
py::V read_json(const str& path) {
    py::file f = py::open(path, S("r"), S("cp1252"));
    str t = f.read_text();
    f.close();
    try {
        return py::json_parse(t.utf8());
    } catch (const py::PyError& e) {                         // (json.JSONDecodeError is a ValueError)
        if (e.type == "ValueError") throw py::ValueError(std::string(e.what()));
        throw;
    }
}

// the strings of a JSON list
list<str> strs_of(const py::V& v) {
    list<str> out;
    if (v.is_list())
        for (const py::V& x : v.seq()) out.append(str(x.str()));
    return out;
}

// what a missing key of a compiler is: KeyError
const list<str>& need_list(const std::optional<list<str>>& v, const char* key) {
    if (!v) throw py::KeyError(py::repr(S(key)).utf8());
    return *v;
}

str read_latin1(const str& path) {
    py::file f = py::open(path, S("r"), S("latin1"));
    str t = f.read_text();
    f.close();
    return t;
}
std::string read_all(const str& path) {
    py::file f = py::open_rb(path);
    std::string s = f.read().raw();
    f.close();
    return s;
}
void write_text(const str& path, const str& text, const char* enc, const char* newline = "\n") {
    py::file f = py::open(path, S("w"), S(enc), std::nullopt, S(newline));
    f.write(text);
    f.close();
}

std::vector<str> sorted_strs(const list<str>& l) {
    std::vector<str> v;
    for (const str& s : l) v.push_back(s);
    std::sort(v.begin(), v.end());
    return v;
}
list<str> to_list(const std::vector<str>& v) {
    list<str> l;
    for (const str& s : v) l.append(s);
    return l;
}

int64_t getpid_() { return static_cast<int64_t>(GetCurrentProcessId()); }

// the group number of a named group of a pattern
int named(const py::re::Pattern& p, const char* name) {
    for (const auto& nm : p.rx().names())
        if (nm.first == name) return nm.second;
    throw py::IndexError("no such group");
}

// str.format(**kw) for the fields a command template has ({name}); {{ and }} as the braces
str format_with(const str& tpl, const dict<str, str>& kw) {
    std::u32string out;
    const std::u32string& u = tpl.u32();
    size_t i = 0;
    while (i < u.size()) {
        char32_t c = u[i];
        if (c == U'{') {
            if (i + 1 < u.size() && u[i + 1] == U'{') {
                out += U'{';
                i += 2;
                continue;
            }
            size_t e = u.find(U'}', i + 1);
            if (e == std::u32string::npos) throw py::ValueError("expected '}' before end of string");
            str name(u.substr(i + 1, e - i - 1));
            if (name.empty()) throw py::IndexError("Replacement index 0 out of range for positional args tuple");
            if (!kw.contains(name)) throw py::KeyError(py::repr(name).utf8());
            out += kw.at(name).u32();
            i = e + 1;
        } else if (c == U'}') {
            if (i + 1 < u.size() && u[i + 1] == U'}') {
                out += U'}';
                i += 2;
                continue;
            }
            throw py::ValueError("Single '}' encountered in format string");
        } else {
            out += c;
            i++;
        }
    }
    return str(out);
}

// shutil.which(cmd) as Python 3.14 writes it (the runtime's py::shutil::which is the older one: it returns a given path as it was
// given, and Python's now joins the folder and the name - "C:/x" + "\\" + "cmd.exe")
std::optional<str> which_(const str& cmd0) {
    auto [dirname, cmd] = py::os::path::split(cmd0);
    list<str> path;
    if (!dirname.empty()) {
        path.append(dirname);
    } else {
        auto p = py::os::getenv(S("PATH"));
        if (!p || p->empty()) return std::nullopt;
        path = py::split(*p, py::os::pathsep);
        if (NeedCurrentDirectoryForExePathW(py::subprocess::to_wide(cmd).c_str())) path.insert(0, S("."));
    }
    auto pe = py::os::getenv(S("PATHEXT"));
    str source = (pe && !pe->empty()) ? *pe : S(".COM;.EXE;.BAT;.CMD;.VBS;.JS;.WS;.MSC");
    list<str> pathext;
    for (const str& ext : py::split(source, py::os::pathsep))
        if (!ext.empty()) pathext.append(ext.rstrip(S(".")));
    list<str> files;
    for (const str& ext : pathext) files.append(cmd + ext);
    str normcmd = cmd.upper();
    bool direct = false;
    for (const str& ext : pathext)
        if (normcmd.endswith(ext.upper())) direct = true;
    if (direct) files.insert(0, cmd);                        // (X_OK: the direct match if and only if the extension is in PATHEXT)
    set<str> seen;
    for (const str& d : path) {
        str normdir = py::os::path::normcase(d);
        if (seen.contains(normdir)) continue;
        seen.add(normdir);
        for (const str& f : files) {
            str name = py::os::path::join(d, f);
            if (py::os::path::exists(name) && !py::os::path::isdir(name)) return name;
        }
    }
    return std::nullopt;
}

// the compiler's folder first on the PATH while its runs are going (see the header)
class PathGuard {
    static std::mutex& mu() { static std::mutex m; return m; }
    static int& count() { static int c = 0; return c; }
    static std::wstring& saved() { static std::wstring s; return s; }

  public:
    explicit PathGuard(const str& cc) {
        std::lock_guard<std::mutex> lk(mu());
        if (count()++ == 0) {
            wchar_t buf[32768];
            DWORD n = GetEnvironmentVariableW(L"PATH", buf, 32768);
            saved().assign(buf, n);
            std::wstring dir = py::subprocess::to_wide(py::os::path::dirname(cc));
            std::wstring now = dir + L";" + saved();
            _wputenv_s(L"PATH", now.c_str());
        }
    }
    ~PathGuard() {
        std::lock_guard<std::mutex> lk(mu());
        if (--count() == 0) _wputenv_s(L"PATH", saved().c_str());
    }
    PathGuard(const PathGuard&) = delete;
    PathGuard& operator=(const PathGuard&) = delete;
};

// this process' environment as {NAME: value} (os.environ: upper-case names on Windows)
dict<str, str> environ_copy() {
    dict<str, str> out;
    LPWCH block = GetEnvironmentStringsW();
    for (LPWCH p = block; p && *p;) {
        std::wstring e(p);
        p += e.size() + 1;
        if (e.empty() || e[0] == L'=') continue;
        size_t eq = e.find(L'=');
        if (eq == std::wstring::npos) continue;
        std::string k, v;
        auto u8 = [](const std::wstring& w) {
            int m = WideCharToMultiByte(CP_UTF8, 0, w.data(), static_cast<int>(w.size()), nullptr, 0, nullptr, nullptr);
            std::string s(static_cast<size_t>(m), '\0');
            WideCharToMultiByte(CP_UTF8, 0, w.data(), static_cast<int>(w.size()), s.data(), m, nullptr, nullptr);
            return s;
        };
        out[str(u8(e.substr(0, eq))).upper()] = str(u8(e.substr(eq + 1)));
    }
    if (block) FreeEnvironmentStringsW(block);
    return out;
}

// a Compiler from the JSON object of compilers.json / settings.json (over `c`: dict.update)
void update_compiler(Compiler& c, const py::V& v) {
    if (!v.is_dict()) return;
    for (const auto& kv : v.items()) {
        std::string k = kv.first.str();
        if (k == "find") c.find = strs_of(kv.second);
        else if (k == "check") c.check = strs_of(kv.second);
        else if (k == "build") c.build = strs_of(kv.second);
        else if (k == "setup") c.setup = strs_of(kv.second);
        else if (k == "diag" && kv.second.is_str()) c.diag = str(kv.second.str());
    }
}

}  // namespace

// ---------------------------------------------------------------- the folder of the port package
namespace {
str here_() {
    auto root = py::os::getenv(S("CB_CHECKOUT_ROOT"));
    return py::os::path::join(py::os::path::join(root ? *root : S("."), S("codebreaker")), S("port"));
}
}  // namespace
const str HERE = here_();

// compilers.json, over it the user's own (settings.json: port.compilers)
Compilers load_compilers() {
    Compilers c;
    py::V doc = read_json(py::os::path::join(HERE, S("compilers.json")));
    for (const auto& kv : doc.items())
        if (kv.second.is_dict()) update_compiler(c.setdefault(str(kv.first.str()), Compiler()), kv.second);
    try {                                                   // (the user's own, from settings.json)
        py::V s = read_json(py::os::path::join(HERE, S(".."), S(".."), S("settings.json")));
        const py::V& comps = s.get("port").get("compilers");
        if (comps.is_dict())
            for (const auto& kv : comps.items()) update_compiler(c.setdefault(str(kv.first.str()), Compiler()), kv.second);
    } catch (const py::OSError&) {
    } catch (const py::ValueError&) {
    }
    return c;
}

// the first of `given` and `names` that is a program (on the PATH, or a path/glob that exists)
std::optional<str> find(const list<str>& names, const std::optional<str>& given) {
    list<str> all;
    if (given && !given->empty()) all.append(*given);
    all.extend(names);
    for (const str& c : all) {
        std::optional<str> p = which_(c);
        if (p && !p->empty()) return p;
        std::vector<str> g;
        for (const str& x : sanitize::glob(c)) g.push_back(x);
        std::sort(g.begin(), g.end(), [](const str& a, const str& b) { return a > b; });   // (sorted, reverse=True)
        if (!g.empty() && !g[0].empty()) return g[0];
    }
    return std::nullopt;
}

// the environment of a compiler's run: this process' own, its folder first on the PATH
dict<str, str> env_for(const str& cc) {
    dict<str, str> e = environ_copy();                      // (a compiler's own DLLs beside it, on the path)
    e[S("PATH")] = py::os::path::dirname(cc) + py::os::pathsep + e.get(S("PATH"), S(""));
    return e;
}

// a compiler's command, after its setup script (MSVC's vcvars) in the same shell
Process run(const Compiler& spec, const list<str>& argv_in, const str& cwd, const str& cc) {
    list<str> argv = argv_in;
    std::optional<str> setup;
    if (spec.setup && !spec.setup->empty()) setup = find(*spec.setup);
    if (setup) {
        list<str> a = {S("cmd"), S("/d"), S("/c"), S("call"), *setup, S(">nul"), S("&&")};
        a.extend(argv_in);
        argv = a;
    }
    py::subprocess::RunOptions o;
    o.cwd = cwd;
    o.errors = "replace";
    PathGuard g(cc);
    py::subprocess::CompletedProcess p = py::subprocess::run(argv, o);
    return Process{p.returncode, p.stdout_ ? *p.stdout_ : S(""), p.stderr_ ? *p.stderr_ : S("")};
}

// a command template: {srcs} the list, the others str.format'ed with kw
list<str> args(const list<str>& tpl, const dict<str, str>& kw, const std::optional<list<str>>& srcs) {
    list<str> out;
    for (const str& a : tpl) {
        if (a == "{srcs}") {
            if (!srcs) throw py::KeyError(py::repr(S("srcs")).utf8());
            out.extend(*srcs);
        } else {
            out.append(format_with(a, kw));
        }
    }
    return out;
}

list<Diagnostic> diagnostics(const Compiler& spec, const str& text, const str& folder) {
    (void)folder;
    list<Diagnostic> out;
    static const py::re::Pattern NOTE(R"PY(^(?P<file>[^:\n]+?(?::[\\/][^:\n]+)?):(?P<line>\d+):(?P<col>\d+): note: (?P<text>.*)$)PY", 0);
    for (const str& l : py::splitlines(text)) {
        str ls = l.strip();
        auto n = NOTE.match(ls);
        if (n && !out.empty()) {                            // (a note: what the complaint before it expected)
            Diagnostic& last = out[out.size() - 1];
            str ntext = n->group(named(NOTE, "text"));
            last.notes.append(ntext);
            if (ntext.contains(S("in expansion of macro"))) {   // (the outermost use: the last such note)
                if (!last.macro_at) last.macro_at = std::make_tuple(last.file, last.line, last.col);   // (where it is in the
                last.file = py::os::path::basename(n->group(named(NOTE, "file")));                  // source: the macro's
                last.line = to_int(n->group(named(NOTE, "line")));                                  // use)
                last.col = to_int(n->group(named(NOTE, "col")));
            }
            continue;
        }
        if (!spec.diag) throw py::KeyError(py::repr(S("diag")).utf8());
        py::re::Pattern dp(*spec.diag, 0);
        auto m = dp.match(ls);
        if (m) {
            Diagnostic d;
            d.file = py::os::path::basename(m->group(named(dp, "file")));
            d.line = to_int(m->group(named(dp, "line")));
            std::optional<str> col = m->group_opt(named(dp, "col"));
            d.col = (col && !col->empty()) ? to_int(*col) : 0;
            d.kind = m->group(named(dp, "kind"));
            d.text = m->group(named(dp, "text"));
            out.append(d);
        }
    }
    return out;
}

std::tuple<dict<str, str>, dict<str, str>, dict<str, str>> read_tree(const str& src) {
    dict<str, str> files, asm_, paths;
    for (const str& d : {src, py::os::path::join(src, S("src"))}) {
        std::vector<str> ps;
        for (const str& p : sanitize::glob(py::os::path::join(d, S("*.[ch]")))) ps.push_back(p);
        std::sort(ps.begin(), ps.end());
        for (const str& p : ps) files[py::os::path::basename(p)] = read_latin1(p);
    }
    for (const str& d : {src, py::os::path::join(src, S("asm")), py::os::path::join(src, S("src"))}) {
        std::vector<str> ps;
        for (const str& p : sanitize::glob(py::os::path::join(d, S("*.asm")))) ps.push_back(p);
        std::sort(ps.begin(), ps.end());
        for (const str& p : ps) {
            asm_[py::os::path::basename(p)] = read_latin1(p);
            paths[py::os::path::basename(p)] = p;
        }
    }
    return {files, asm_, paths};
}

// the bytes this process has committed (Windows: its private bytes; elsewhere: its resident set) - or None
std::optional<int64_t> memory_used() {
    PROCESS_MEMORY_COUNTERS pmc{};
    pmc.cb = sizeof pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, pmc.cb)) return static_cast<int64_t>(pmc.PagefileUsage);
    return std::nullopt;
}

// a watch on the port's own memory: above the ceiling (settings.json port.max_memory_gb, 4 by default) it
// stops - where it was written to OUT/port-memory.txt - before it can starve the machine (a rule that runs away)
void memory_guard(const str& out) {
    double limit = 4;
    try {
        py::V s = read_json(py::os::path::join(HERE, S(".."), S(".."), S("settings.json")));
        const py::V& conf = s.get("port");
        if (conf.is_dict() && conf.get("max_memory_gb").is_int()) limit = static_cast<double>(conf.get("max_memory_gb").as_int());
        else if (conf.is_dict() && conf.get("max_memory_gb").is_float()) limit = conf.get("max_memory_gb").as_float();
    } catch (const py::OSError&) {
    } catch (const py::ValueError&) {
    }
    limit *= 1024.0 * 1024.0 * 1024.0;
    if (!memory_used()) return;
    std::thread([out, limit] {
        for (;;) {
            int64_t used = memory_used().value_or(0);
            if (static_cast<double>(used) > limit) {
                try {
                    py::os::makedirs(out, 0777, true);
                    py::file f = py::open(py::os::path::join(out, S("port-memory.txt")), S("w"));
                    f.write(F("the port stopped itself: %.1f GB used, the ceiling %.1f GB (settings.json port.max_memory_gb) - where it was:\n\n",
                              used / 1073741824.0, limit / 1073741824.0));
                    f.close();
                } catch (const py::OSError&) {
                }
                std::cerr << F("port: stopped - %.1f GB of memory, above the ceiling of %.1f GB (where it was: %s)\n", used / 1073741824.0,
                               limit / 1073741824.0, py::os::path::join(out, S("port-memory.txt"))).utf8();
                std::cerr.flush();
                std::_Exit(3);
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }).detach();
}

// the include names a text mentions ("x.h"), in order
static list<str> quoted_includes(const str& t) {
    return py::re::findall(S(R"PY(#\s*include\s*"([^"]+)")PY"), t);
}

// what one file's check depends on: the compiler and its command, the file, every header it reaches
str check_key(const Compiler& spec, const str& cc, const str& n, const dict<str, str>& res) {
    py::hashlib::Hash h = py::hashlib::sha256(py::bytes(("(" + py::repr(py::os::path::abspath(cc)).utf8() + ", " +
                                                         py::repr(spec.check).utf8() + ", " + py::repr(n).utf8() + ")")));
    std::vector<str> todo = {n};
    set<str> seen;
    while (!todo.empty()) {
        str x = todo.back();
        todo.pop_back();
        if (seen.contains(x)) continue;
        seen.add(x);
        str t = res.get(x, S(""));
        h.update(py::bytes(x.utf8() + std::string(1, '\0') + t.encode("latin1", "replace").raw() + std::string(1, '\0')));
        for (const str& inc : quoted_includes(t)) todo.push_back(inc);
    }
    return h.hexdigest();
}

// an object file whole: not empty, its first bytes not nothing (a build cut short leaves either)
bool sound(const str& path) {
    try {
        py::file f = py::open_rb(path);
        std::string head = f.read(16).raw();
        f.close();
        bool nonzero = false;
        for (char c : head)
            if (c != '\0') nonzero = true;
        return py::os::path::getsize(path) > 64 && nonzero;
    } catch (const py::OSError&) {
        return false;
    }
}

namespace {
struct CacheDiag;
// one complaint as the typed cache keeps it: (file, line, col, kind, text, notes, macro_at)
using KeptDiag = std::tuple<str, int64_t, int64_t, str, str, list<str>, std::optional<std::tuple<str, int64_t, int64_t>>>;
KeptDiag keep_of(const Diagnostic& d) { return KeptDiag{d.file, d.line, d.col, d.kind, d.text, d.notes, d.macro_at}; }
Diagnostic diag_of(const KeptDiag& k) {
    Diagnostic d;
    d.file = std::get<0>(k);
    d.line = std::get<1>(k);
    d.col = std::get<2>(k);
    d.kind = std::get<3>(k);
    d.text = std::get<4>(k);
    d.notes = std::get<5>(k);
    d.macro_at = std::get<6>(k);
    return d;
}
pre_ansi::Diag to_pre(const Diagnostic& d) {
    pre_ansi::Diag p;
    p.file = d.file;
    p.line = d.line;
    p.col = d.col;
    p.kind = d.kind;
    p.text = d.text;
    p.notes = d.notes;
    return p;
}
}  // namespace

// the port built a C file at a time, side by side (the workbench's workers, at most four), each file's object
// kept by all it depends on (the compiler and its command, the file, every header it reaches): a file as it was
// the last time not compiled again; then the objects linked. A compiler of another form (MSVC's): all at once
Process build(const Compiler& spec, const str& cc, const list<str>& tpl, const str& out, const list<str>& srcs, const str& exe,
              const str& objdir_in) {
    int64_t at = tpl.contains(S("{out}")) ? tpl.index(S("{out}")) : -1;
    dict<str, str> kw0;
    kw0[S("cc")] = cc;
    kw0[S("inc")] = S(".");
    kw0[S("out")] = exe;
    if ((spec.setup && !spec.setup->empty()) || !tpl.contains(S("{srcs}")) || at < 1 || tpl[at - 1] != "-o")
        return run(spec, args(tpl, kw0, srcs), out, cc);
    list<str> rest;
    for (int64_t i = 0; i < tpl.size(); i++)
        if (i != at - 1 && i != at && tpl[i] != "{srcs}") rest.append(tpl[i]);
    dict<str, str> kw1;
    kw1[S("cc")] = cc;
    kw1[S("inc")] = S(".");
    kw1[S("out")] = S("");
    list<str> cargv = args(rest, kw1);
    str objdir = py::os::path::abspath(objdir_in) + F(".%d", getpid_());   // (its own: two ports of one folder at once apart)
    py::os::makedirs(objdir, 0777, true);
    std::mutex tmu;
    dict<str, std::optional<py::bytes>> texts;

    auto text = [&](const str& n) -> std::optional<py::bytes> {
        std::lock_guard<std::mutex> lk(tmu);
        if (!texts.contains(n)) {
            try {
                texts[n] = py::bytes(read_all(py::os::path::join(out, n)));
            } catch (const py::OSError&) {
                texts[n] = std::nullopt;
            }
        }
        return texts.at(n);
    };
    auto key = [&](const str& n) -> str {
        list<str> c1;
        for (int64_t i = 1; i < cargv.size(); i++) c1.append(cargv[i]);
        py::hashlib::Hash h = py::hashlib::sha256(py::bytes("(" + py::repr(py::os::path::abspath(cc)).utf8() + ", " + py::repr(c1).utf8() + ", " +
                                                            py::repr(n).utf8() + ")"));
        std::vector<str> todo = {n};
        set<str> seen;
        while (!todo.empty()) {
            str x = todo.back();
            todo.pop_back();
            if (seen.contains(x)) continue;
            seen.add(x);
            std::optional<py::bytes> t = text(x);
            if (!t) continue;
            h.update(py::bytes(x.utf8() + std::string(1, '\0') + t->raw() + std::string(1, '\0')));
            for (const str& m : py::re::findall(S(R"PY(#\s*include\s*[<"]([^>"]+)[>"])PY"), t->decode("latin1"), py::re::A)) todo.push_back(m);
        }
        return h.hexdigest();
    };

    list<str> cs;
    for (const str& n : srcs)
        if (n.endswith(S(".c"))) cs.append(n);
    dict<str, str> keys;
    for (const str& n : cs) keys[n] = key(n);

    auto one = [&](const str& n) -> std::tuple<int64_t, str> {
        str k = keys.at(n);
        str o = base::cache::path(S("port-objects"), k) + S(".o");
        str mine = py::os::path::join(objdir, n.slice(0, n.size() - 2) + S(".o"));
        if (!base::cache::off() && sound(o)) {                                  // (as the last time: its object, and what it said then)
            try {
                py::shutil::copyfile(o, mine);
                std::error_code ec;
                fs::last_write_time(fs::u8path(o.utf8()), fs::file_time_type::clock::now(), ec);   // (os.utime(o))
                py::file f = py::open(o.slice(0, o.size() - 2) + S(".txt"), S("r"), S("utf-8"), S("replace"));
                str t = f.read_text();
                f.close();
                return {0, t};
            } catch (const py::OSError&) {
            }
        }
        list<str> argv = cargv.copy();
        argv.append(S("-c"));
        argv.append(n);
        argv.append(S("-o"));
        argv.append(mine);
        Process p = run(spec, argv, out, cc);
        if (p.returncode == 0 && !base::cache::off() && sound(mine)) {
            try {
                py::os::makedirs(py::os::path::dirname(o), 0777, true);
                str tmp = o + F(".%d.%s.tmp", getpid_(), std::to_string(static_cast<uint64_t>(std::hash<std::thread::id>{}(std::this_thread::get_id()))));
                py::shutil::copyfile(mine, tmp);
                py::file f = py::open(o.slice(0, o.size() - 2) + S(".txt"), S("w"), S("utf-8"));
                f.write(p.stdout_ + p.stderr_);
                f.close();
                py::os::replace(tmp, o);
            } catch (const py::OSError&) {
            }
        }
        return {p.returncode, p.stdout_ + p.stderr_};
    };

    int64_t jobs = std::min<int64_t>(std::min<int64_t>(4, base::parallel::workers()), cs.size() ? cs.size() : 1);
    list<std::tuple<int64_t, str>> done = jobs::pool::threads(one, cs, jobs);
    str logs;
    bool failed = false;
    for (const auto& rt : done) {
        logs += std::get<1>(rt);
        if (std::get<0>(rt)) failed = true;
    }
    if (failed) {
        py::shutil::rmtree(objdir, true);
        return Process{1, logs, S("")};
    }
    str here = py::os::path::abspath(out);
    list<str> objs;
    for (const str& n : srcs) {
        if (n.endswith(S(".c")))
            objs.append(py::os::path::relpath(py::os::path::join(objdir, n.slice(0, n.size() - 2) + S(".o")), here).replace(S("\\"), S("/")));
        else
            objs.append(n);
    }
    dict<str, str> kw2 = kw0;
    Process p = run(spec, args(tpl, kw2, objs), out, cc);
    py::shutil::rmtree(objdir, true);
    base::cache::hold();                                    // (the kept objects count toward the cache cap)
    return Process{p.returncode, logs + p.stdout_, p.stderr_};
}

// the C files to check again: those whose text is not what it was, and those that include (themselves or
// through another header) a header whose text is not
list<str> changed_files(const dict<str, str>& res, const dict<str, str>& before, const list<str>& cfiles) {
    if (before.empty()) return cfiles.copy();
    std::map<str, set<str>> inc;
    for (const auto& kv : res.items()) {
        set<str> s;
        for (const str& h : quoted_includes(std::get<1>(kv))) s.add(h);
        inc[std::get<0>(kv)] = s;
    }
    set<str> changed, hdrs;
    for (const auto& kv : res.items()) {
        const str& n = std::get<0>(kv);
        if (!before.contains(n) || before.at(n) != std::get<1>(kv)) changed.add(n);
    }
    for (const str& n : changed)
        if (!n.endswith(S(".c"))) hdrs.add(n);
    std::function<bool(const str&, set<str>&)> reaches = [&](const str& n, set<str>& seen) -> bool {
        auto it = inc.find(n);
        if (it == inc.end()) return false;
        for (const str& h : it->second) {
            if (hdrs.contains(h)) return true;
            if (!seen.contains(h)) {
                seen.add(h);
                if (reaches(h, seen)) return true;
            }
        }
        return false;
    };
    list<str> out;
    for (const str& n : cfiles) {
        set<str> seen;
        if (changed.contains(n) || reaches(n, seen)) out.append(n);
    }
    return out;
}

// {C name: its place in the data segment} of a SYMBOLS list (the recon's: name, C or D, address)
dict<str, int64_t> read_symbols(const str& src) {
    for (const str& d : {src, py::os::path::join(src, S("src"))}) {
        str p = py::os::path::join(d, S("SYMBOLS"));
        if (py::os::path::isfile(p)) {
            dict<str, int64_t> out;
            for (const str& l : py::split(read_latin1(p), S("\n"))) {
                list<str> w = py::split(py::split(l, S("#"), 1)[0]);
                if (w.size() == 3 && w[1] == "D") {
                    try {
                        out[w[0].startswith(S("_")) ? sl(w[0], 1, w[0].size()) : w[0]] = py::int_parse(w[2], 16);
                    } catch (const py::ValueError&) {
                    }
                }
            }
            return out;
        }
    }
    return dict<str, int64_t>();
}

// {C name: its bytes} of the variables a BSS list names (the recon's: what the old build defined beginning
// without a value, by DS offset - 'range from to', 'hole at length', 'name offset')
dict<str, int64_t> read_bss(const str& src) {
    for (const str& d : {src, py::os::path::join(src, S("src"))}) {
        str p = py::os::path::join(d, S("BSS"));
        if (!py::os::path::isfile(p)) continue;
        std::vector<std::pair<int64_t, str>> ents;
        std::optional<std::pair<int64_t, int64_t>> rng;
        std::vector<std::pair<int64_t, int64_t>> holes;
        for (const str& l : py::split(read_latin1(p), S("\n"))) {
            list<str> w = py::split(py::split(l, S("#"), 1)[0]);
            if (w.size() == 3 && w[0] == "range") {
                rng = {py::int_parse(w[1], 16), py::int_parse(w[2], 16)};
            } else if (w.size() == 3 && w[0] == "hole") {
                holes.push_back({py::int_parse(w[1], 16), py::int_parse(w[2], 16)});
            } else if (w.size() == 2) {
                ents.push_back({py::int_parse(w[1], 16), w[0]});
            }
        }
        std::sort(ents.begin(), ents.end());
        dict<str, int64_t> out;
        for (size_t i = 0; i < ents.size(); i++) {
            int64_t off = ents[i].first;
            const str& name = ents[i].second;
            int64_t end = i + 1 < ents.size() ? ents[i + 1].first : (rng ? rng->second : off + 2);
            int64_t best = end;
            for (const auto& h : holes)
                if (off < h.first && h.first < end) best = std::min(best, h.first);
            end = best;
            out[name.startswith(S("_")) ? sl(name, 1, name.size()) : name] = end - off;
        }
        return out;
    }
    return dict<str, int64_t>();
}

// ---------------------------------------------------------------- the profile
RuntimeProfile RuntimeProfile::load(const str& path) {
    py::V doc = read_json(path);
    RuntimeProfile p;
    if (doc.get("name").is_str()) p.name = str(doc.get("name").str());
    p.libc = strs_of(doc.get("libc"));
    p.runtime_defines = strs_of(doc.get("runtime_defines"));
    p.platform_calls = strs_of(doc.get("platform").get("calls"));
    auto optb = [&](const char* k) -> std::optional<bool> {
        const py::V& v = doc.get(k);
        if (v.is_bool()) return v.as_int() != 0;
        return std::nullopt;
    };
    p.text_mode = optb("text_mode");
    p.console_text = optb("console_text");
    p.exit_status = optb("exit_status");
    const py::V& notes = doc.get("notes");
    if (notes.is_dict())
        for (const auto& kv : notes.items()) p.notes.set(str(kv.first.str()), str(kv.second.str()));
    if (doc.get("widths").get("int").is_int()) p.width_int = doc.get("widths").get("int").as_int();
    if (doc.get("char_signed").is_bool()) p.char_signed = doc.get("char_signed").as_int() != 0;
    if (doc.get("c_underscore").is_bool()) p.c_underscore = doc.get("c_underscore").as_int() != 0;
    return p;
}

str strip_asm_comment(const str& l) { return py::split(l, S(";"), 1)[0]; }

namespace {
// the names a PUBLIC line of an assembler text makes public
set<str> public_names(const str& text) {
    set<str> pubs;
    for (const str& l : py::splitlines(text)) {
        auto m = py::re::match(S(R"(\s*PUBLIC\s+(.+))"), strip_asm_comment(l), py::re::I);
        if (m)
            for (const str& x : py::split(m->group(1), S(","))) {
                str t = x.strip();
                if (!t.empty()) pubs.add(t);
            }
    }
    return pubs;
}
}  // namespace

// what an assembler module of the program is: the old runtime's (it makes public what the runtime defines,
// or every name it makes public is the C library's or the platform layer's), data alone (no instruction in a
// code segment), or code of the program - to be ported as assembly language
std::tuple<str, list<str>> asm_role(const str& text, const RuntimeProfile& profile) {
    set<str> pubs = public_names(text);
    std::vector<str> sorted_pubs;
    for (const str& p : pubs) sorted_pubs.push_back(p);
    std::sort(sorted_pubs.begin(), sorted_pubs.end());
    list<str> sp = to_list(sorted_pubs);
    set<str> known;
    for (const str& s : profile.libc) known.add(s);
    for (const str& s : profile.platform_calls) known.add(s);
    for (const str& s : profile.runtime_defines) known.add(s);
    set<str> names;
    for (const str& p : pubs) names.add(p.startswith(S("_")) ? sl(p, 1, p.size()) : p);
    set<str> defs_minus;
    for (const str& s : profile.runtime_defines)
        if (s != "fopen" && s != "fprintf" && s != "malloc") defs_minus.add(s);
    bool inter = false;
    for (const str& n : names)
        if (defs_minus.contains(n)) inter = true;
    if (inter || pubs.contains(S("_main"))) return {S("runtime"), sp};
    bool has_end = false, has_main = false;
    for (const str& l : py::splitlines(text)) {
        if (py::re::match(S(R"(\s*END\s+\w+)"), strip_asm_comment(l), py::re::I)) has_end = true;
        if (py::re::match(S(R"(\s*_main\b)"), strip_asm_comment(l))) has_main = true;
    }
    if (has_end && !has_main) return {S("runtime"), sp};      // (END start: the start-up code - the C library's now)
    list<asm16::LineP> lines = asm16::parse(text);
    std::optional<str> seg;
    bool code = false;
    set<str> code_segs = asm16::code_segments(lines);
    for (const asm16::LineP& l : lines) {
        const std::optional<asm16::Directive>& d = l->directive;
        if (d && d->kind == "SEGMENT") {
            seg = (d->name && code_segs.contains(*d->name)) ? d->name : std::nullopt;
        } else if (d && d->kind == "ENDS") {
            seg = std::nullopt;
        } else if (seg && l->op) {
            code = true;
        }
    }
    if (!code) return {S("data"), sp};
    bool own = false;
    for (const str& p : pubs) {
        bool a = py::re::match(S("_[A-Za-z]"), p) && !known.contains(sl(p, 1, p.size()));
        bool b = py::re::match(S("[A-Z][A-Z0-9_]*$"), p) && !known.contains(p.lower());
        if (a || b) own = true;                              // (C names: _x is C's x, CAPS a pascal one;
    }                                                        //  __x and cret the runtime's)
    return {own ? S("code") : S("runtime"), sp};
}

const dict<str, str> NEW_TYPES = {
    {S(""), S("int16_t")}, {S("int"), S("int16_t")}, {S("short"), S("int16_t")}, {S("short int"), S("int16_t")},
    {S("unsigned"), S("uint16_t")}, {S("unsigned int"), S("uint16_t")}, {S("unsigned short"), S("uint16_t")},
    {S("long"), S("int32_t")}, {S("long int"), S("int32_t")}, {S("unsigned long"), S("uint32_t")}, {S("char"), S("char")},
    {S("unsigned char"), S("unsigned char")}, {S("void"), S("void")}};

// {name: the type it returns, today's spelling} from the C program's declarations of functions it does not
// define (extern T NEAR f();) - what it expects of the assembly's
dict<str, str> c_declared(const list<str>& texts) {
    set<str> macros;
    for (const str& t : texts) {
        for (const auto& m : py::re::finditer(S(R"((?m)^\s*#\s*define\s+(\w+)[ \t]+([\w \t]+?)\s*$)"), t)) {
            bool sub = true;
            for (const str& w : py::split(m.group(2)))
                if (!asm16::CALL_WORDS.contains(w)) sub = false;
            if (sub) macros.add(m.group(1));
        }
    }
    dict<str, str> out;
    for (const str& t : texts) {
        for (const auto& m : py::re::finditer(S(R"(\bextern\s+([\w\s*]+?)\b([A-Za-z_]\w*)\s*\(\s*\)\s*;)"), blank(t))) {
            list<str> words;
            for (const str& w : py::re::findall(S(R"(\w+|\*)"), m.group(1)))
                if (!asm16::CALL_WORDS.contains(w) && !macros.contains(w)) words.append(w);
            int64_t stars = words.count(S("*"));
            list<str> bw;
            for (const str& w : words)
                if (w != "*") bw.append(w);
            str base = join(S(" "), bw);
            if (stars) {
                if (!out.contains(m.group(2))) out[m.group(2)] = base + S(" ") + stars_of(stars);
            } else if (NEW_TYPES.contains(base)) {
                if (!out.contains(m.group(2))) out[m.group(2)] = NEW_TYPES.at(base);
            }
        }
    }
    return out;
}

const dict<str, int64_t> NEW_SIZES = {
    {S("char"), 1}, {S("signed char"), 1}, {S("unsigned char"), 1}, {S("int8_t"), 1}, {S("uint8_t"), 1}, {S("int16_t"), 2},
    {S("uint16_t"), 2}, {S("short"), 2}, {S("int32_t"), 4}, {S("uint32_t"), 4}, {S("long"), 4}, {S("int"), 4}, {S("unsigned"), 4}};

// [(old address in the data segment, name)] of the program's variables, where the recovered source has them
// (a SYMBOLS file beside it: name, D, address - as the original image placed them)
list<std::tuple<int64_t, str>> data_places(const str& asm_path) {
    str p = py::os::path::join(py::os::path::dirname(asm_path), S("SYMBOLS"));
    std::vector<std::tuple<int64_t, str>> out;
    if (py::os::path::isfile(p)) {
        for (const str& l : py::split(read_latin1(p), S("\n"))) {
            list<str> w = py::split(l);
            if (w.size() == 3 && w[1] == "D" && fullm("[0-9a-fA-F]+", w[2])) out.push_back({py::int_parse(w[2], 16), w[0]});
        }
    }
    std::sort(out.begin(), out.end());
    list<std::tuple<int64_t, str>> r;
    for (const auto& x : out) r.append(x);
    return r;
}

// {alias: (owner, byte offset now)} of the names the old linker gave places inside other variables
dict<str, std::tuple<str, int64_t>> alias_places(const dict<str, std::tuple<str, str>>& aliases, const asm16::CTypes& ct) {
    dict<str, std::tuple<str, int64_t>> out;
    for (const auto& kv : aliases.items()) {
        const str& al = std::get<0>(kv);
        const str& where = std::get<0>(std::get<1>(kv));
        const str& od = std::get<1>(std::get<1>(kv));
        auto m = py::re::fullmatch(S(R"(\(?&?\s*(\w+)\s*(?:\[\s*(\d+)\s*\])?\)?)"), where.strip());
        if (!m) continue;
        str owner = m->group(1);
        std::optional<str> k_ = m->group_opt(2);
        int64_t k = (k_ && !k_->empty()) ? to_int(*k_) : 0;
        str before_owner = py::split(od, owner)[0];
        str base = py::re::sub(S(R"(\b(extern|static)\b)"), S(""), before_owner).strip();
        int64_t size;
        if (before_owner.contains(S("*")) || py::re::search(S(R"(\(\s*\*)"), od)) {
            size = 8;
        } else if (base.startswith(S("struct")) || base.startswith(S("union"))) {
            const asm16::Layout* lay = ct.new_.get(ws1(base));
            size = lay ? lay->size : 1;
        } else {
            str b = ws1(base);
            size = NEW_SIZES.contains(b) ? NEW_SIZES.at(b) : 1;
        }
        out[al] = {owner, k * size};
    }
    return out;
}

namespace {
// call_arg_types' set of far variables, kept for the list of originals it was made of (the Python: function attributes)
struct FarCache {
    std::mutex mu;
    list<str> key;                  // (the originals' list: the same shared list is the same key)
    bool have = false;
    set<str> far;
};
FarCache& far_cache() {
    static FarCache c;
    return c;
}
}  // namespace

// [(old size, what it points to or None)] of the arguments the C program calls an assembly routine with: a
// pointer to a variable it declares (&g), a pointer variable (p) - far ones 4 bytes, as they were pushed
list<std::tuple<int64_t, std::optional<str>>> call_arg_types(const str& fn, const list<str>& texts, const list<str>& origs,
                                                             const asm16::CTypes& ct) {
    set<str> far;
    {
        FarCache& fc = far_cache();
        std::lock_guard<std::mutex> lk(fc.mu);
        if (!fc.have || &fc.key.vec() != &origs.vec()) {
            set<str> f;
            for (const str& t : origs)
                for (const auto& d : py::re::finditer(S(R"((?m)^[^;(){}]*\bfar\b[^;(){}]*?\b(\w+)\s*(?:\[[^\]]*\])?\s*;)"), blank(t)))
                    f.add(d.group(1));
            fc.far = f;
            fc.key = origs;
            fc.have = true;
        }
        far = fc.far;
    }
    using Arg = std::tuple<int64_t, std::optional<str>>;
    list<Arg> best;
    auto count_to = [](const list<Arg>& l) {
        int64_t n = 0;
        for (const Arg& a : l)
            if (std::get<1>(a) && !std::get<1>(a)->empty()) n++;
        return n;
    };
    str fe = esc(fn);
    for (const str& t : texts) {
        str bt = blank(t);
        for (const str& line : py::split(bt, S("\n"))) {
            if (!py::re::search(S(R"(\b)") + fe + S(R"(\s*\()"), line) ||
                py::re::match(S(R"(\s*(?:extern\b|\w[\w\s*]*\b)") + fe + S(R"(\s*\([^)]*\)\s*$))"), line))
                continue;
            std::optional<list<pre_ansi::Span>> spans = pre_ansi::call_args(line, fn);
            if (!spans || spans->empty()) continue;
            list<Arg> out;
            for (const pre_ansi::Span& sp : *spans) {
                str arg = sl(line, std::get<0>(sp), std::get<1>(sp)).strip();
                auto m = py::re::fullmatch(S(R"(&\s*(\w+))"), arg);
                std::optional<std::tuple<str, str>> v;
                if (m) v = ct.var(m->group(1));
                if (v) {
                    int64_t stars = replace1(py::re::sub(S(R"(\[[^\]]*\])"), S(""), std::get<1>(*v)), m->group(1), S("")).count(S("*"));
                    out.append({far.contains(m->group(1)) ? 4 : 2, std::get<0>(*v).strip() + (stars ? S(" ") + stars_of(stars) : S(""))});
                    continue;
                }
                bool word = fullm(R"(\w+)", arg);
                std::optional<py::re::Match> loc;
                if (word)
                    loc = py::re::search(S(R"(\b((?:struct|union)\s+\w+|char|unsigned\s+char|int16_t|uint16_t)\s*(\*+)\s*)") + esc(arg) + S(R"(\s*[;,)=])"), bt);
                if (loc) {                                   // (a variable of the calling file: a parameter, a local)
                    int64_t stars = loc->group(2).size();
                    out.append({2, ws1(loc->group(1)) + (stars > 1 ? S(" ") + stars_of(stars - 1) : S(""))});
                    continue;
                }
                if (word && ct.is_pointer(arg)) {
                    out.append({far.contains(arg) ? 4 : 2, ct.pointee(arg)});
                    continue;
                }
                std::optional<std::tuple<str, str>> v2;
                if (word) v2 = ct.var(arg);
                bool lng = word && static_cast<bool>(py::re::search(S(R"(\b(?:long|int32_t|uint32_t)\s+(?:[^;(){}]*,\s*)?)") + esc(arg) + S(R"(\s*[;,=])"), bt));
                bool wide = (v2 && py::re::search(S(R"(\b(long|int32_t|uint32_t)\b)"), std::get<0>(*v2))) || lng ||
                            fullm(R"((?:0[xX][0-9a-fA-F]+|\d+)[lL])", arg);
                out.append({wide ? 4 : 2, std::nullopt});
            }
            if (count_to(out) > count_to(best)) best = out;
        }
    }
    return best;
}

// {routine: its lines} - the old runtime's routines that are straight lines to a RET
dict<str, list<str>> runtime_tails(const dict<str, str>& mods) {
    dict<str, list<str>> out;
    for (const auto& kv : mods.items()) {
        list<str> lines = py::splitlines(std::get<1>(kv));
        set<str> pubs = public_names(std::get<1>(kv));
        for (int64_t i = 0; i < lines.size(); i++) {
            auto m = py::re::match(S(R"(\s*(\w+)(?:\s+PROC\b.*|\s*:\s*)$)"), strip_asm_comment(lines[i]), py::re::I);
            if (!m || !pubs.contains(m->group(1))) continue;
            std::optional<list<str>> body = list<str>();
            for (int64_t j = i + 1; j < lines.size(); j++) {
                str c = strip_asm_comment(lines[j]).strip();
                if (c.empty()) continue;
                if (py::re::match(S(R"((?i)(?:\w+\s*:|j\w+\b|call\b|loop\w*\b|\w+\s+(?:PROC|ENDP|ENDS)\b))"), c)) {
                    body = std::nullopt;                     // (not straight: left to the C library's)
                    break;
                }
                body->append(S("\t") + c);
                if (py::re::match(S(R"((?i)ret\b)"), c)) break;
            }
            if (body && !body->empty() && py::re::match(S(R"((?i)\s*ret\b)"), (*body)[body->size() - 1])) out[m->group(1)] = *body;
        }
    }
    return out;
}

// JMP to one of the old runtime's straight routines: its instructions in place of the jump
str inline_tails(const str& text, const dict<str, list<str>>& tails, list<str>& notes, const str& name) {
    if (tails.empty()) return text;
    list<str> out;
    int64_t n = 0;
    for (const str& l : py::splitlines(text)) {
        auto m = py::re::match(S(R"((?i)\s*jmp\s+(?:near\s+ptr\s+|short\s+)?(\w+)\s*$)"), strip_asm_comment(l));
        if (m && tails.contains(m->group(1))) {
            out.append(F("\t; (JMP %s - the old runtime's: its instructions here)", m->group(1)));
            out.extend(tails.at(m->group(1)));
            n += 1;
            continue;
        }
        out.append(l);
    }
    if (n) {
        std::vector<str> used;
        for (const auto& kv : tails.items())
            if (py::re::search(S(R"((?i)\bjmp\s+)") + std::get<0>(kv) + S(R"(\b)"), text)) used.push_back(std::get<0>(kv));
        std::sort(used.begin(), used.end());
        used.erase(std::unique(used.begin(), used.end()), used.end());
        notes.append(F("%s: %d jump(s) into the old runtime's own ends (%s): their instructions in place", name, n, join(S(", "), to_list(used))));
    }
    return join(S("\n"), out) + (text.endswith(S("\n")) ? S("\n") : S(""));
}

// ---------------------------------------------------------------- asm_data_defs
namespace {
struct Place {
    str c;
    std::optional<str> seg;
    int64_t off = 0;
    int64_t own = 0;
};
bool same_at(const Place& a, const Place& b) { return a.seg == b.seg && a.off == b.off; }
}  // namespace

// the variables of an assembler module of data alone (placed by the old linker): C definitions, with the
// C program's types where it declares them
DataDefs asm_data_defs(const str& text, const asm16::CTypes& ct) {
    list<asm16::LineP> lines = asm16::parse(text);
    set<str> pubs;
    for (const asm16::LineP& l : lines) {
        const auto& d = l->directive;
        if (d && d->kind == "PUBLIC")
            for (const str& x : py::split(d->rest, S(","))) {
                str t = x.strip();
                if (!t.empty()) pubs.add(t);
            }
    }
    std::vector<Place> places;                               // ([C name, old offset, bytes of its own])
    int64_t off = 0;
    std::optional<str> seg;
    for (const asm16::LineP& l : lines) {
        const auto& d = l->directive;
        if (d && d->kind == "SEGMENT") {
            seg = d->name;
            off = 0;
            continue;
        }
        if (d && d->kind == "LABEL" && d->name && pubs.contains(*d->name)) {
            Place p;
            p.c = d->name->startswith(S("_")) ? sl(*d->name, 1, d->name->size()) : *d->name;
            p.seg = seg;
            p.off = off;
            places.push_back(p);
        } else if (d && (d->kind == "DB" || d->kind == "DW" || d->kind == "DD")) {
            auto m = py::re::match(S(R"(\s*(\d+)\s+DUP)"), d->rest, py::re::I);
            int64_t unit = d->kind == "DB" ? 1 : d->kind == "DW" ? 2 : 4;
            int64_t n = (m ? to_int(m->group(1)) : asm16::split_ops(d->rest).size()) * unit;
            if (!places.empty() && places.back().seg == seg) places.back().own += n;
            off += n;
        }
    }
    auto old_size = [&](const str& base, const str& dcl, const str& n) -> int64_t {
        std::optional<int64_t> s = pre_ansi::size_of(base, dcl, n, 2, asm16::Layouts());
        return (s && *s) ? *s : 2;
    };
    DataDefs R;
    list<str>& out = R.defs;
    dict<str, str>& aliases = R.aliases;
    set<str> done;
    dict<str, str>& one_place = R.one_place;                 // (scalars of one place under two names: one variable,
    for (size_t i = 0; i < places.size(); i++) {             //  of the type that is a pointer if one is)
        const str& c = places[i].c;
        std::vector<const Place*> same;
        for (const Place& p : places)
            if (same_at(p, places[i])) same.push_back(&p);
        auto v = ct.var(c);
        if (same.size() < 2 || done.contains(c) || !v || std::get<1>(*v).contains(S("["))) continue;
        std::vector<std::pair<str, std::tuple<str, str>>> typed;
        for (const Place* p : same) {
            auto pv = ct.var(p->c);
            if (pv && !std::get<1>(*pv).contains(S("["))) typed.push_back({p->c, *pv});
        }
        if (typed.size() < 2) continue;
        str canon = typed.back().first;
        for (const auto& nw : typed)
            if (std::get<1>(nw.second).contains(S("*"))) {
                canon = nw.first;
                break;
            }
        for (const auto& nw : typed)
            if (nw.first != canon) {
                one_place[nw.first] = canon;
                done.add(nw.first);
            }
    }
    for (size_t i = 0; i < places.size(); i++) {
        const Place& pl = places[i];
        const str& c = pl.c;
        if (done.contains(c)) continue;
        auto v = ct.var(c);
        std::optional<std::tuple<str, str>> nv;
        if (i + 1 < places.size()) nv = ct.var(places[i + 1].c);
        bool same_next = false;
        if (v && nv) {
            str a = py::re::sub(S(R"(\s)"), S(""), std::get<0>(*nv)), b = py::re::sub(S(R"(\s)"), S(""), std::get<0>(*v));
            same_next = a == b && std::get<1>(*nv).contains(S("*")) == py::re::sub(S(R"(\[\s*\])"), S(""), std::get<1>(*v)).contains(S("*"));
        }
        if (v && py::re::search(S(R"(\[\s*\])"), std::get<1>(*v)) && (pl.own == 0 || same_next)) {   // (an array that is the names
            const str& base = std::get<0>(*v);                                                         //  after it: the old linker
            const str& dcl = std::get<1>(*v);                                                          //  gave them one place)
            str el = py::re::sub(S(R"(\[\s*\])"), S(""), dcl).strip();
            int64_t es = old_size(base, el, c);
            str elt = py::re::sub(S(R"(\s)"), S(""), base + el.replace(c, S("")));
            struct Nxt {
                str name;
                int kind;       // 0: the same array by another name; 1: ("&", k) an array that is the rest of it; 2: one of its elements
                int64_t k;
            };
            std::vector<Nxt> nxt;
            std::optional<int64_t> end;
            for (size_t j = i + 1; j < places.size(); j++) {
                const Place& p2 = places[j];
                auto v2 = ct.var(p2.c);
                if (p2.seg != pl.seg) break;
                std::optional<str> d2;
                if (v2) d2 = py::re::sub(S(R"(\s)"), S(""), std::get<0>(*v2) + py::re::sub(S(R"(\[\s*\])"), S(""), std::get<1>(*v2)).replace(p2.c, S("")));
                int64_t rel = p2.off - pl.off;
                bool arr2 = v2 && py::re::search(S(R"(\[\s*\])"), std::get<1>(*v2));
                if (d2 && *d2 == elt && arr2 && rel == 0) {
                    nxt.push_back({p2.c, 0, 0});            // (the same array, by another name)
                } else if (d2 && *d2 == elt && v2 && py::mod(rel, es) == 0 && arr2) {
                    nxt.push_back({p2.c, 1, py::floordiv(rel, es)});    // (an array that is the rest of it, from there)
                } else if (d2 && *d2 == elt && v2 && py::mod(rel, es) == 0) {
                    nxt.push_back({p2.c, 2, py::floordiv(rel, es)});    // (one of its elements)
                } else {
                    end = p2.off;
                    break;
                }
            }
            int64_t extent;
            if (end) {
                extent = *end - pl.off;
            } else {
                int64_t s = 0;
                for (size_t j = i; j < std::min(places.size(), i + nxt.size() + 1); j++) s += places[j].own;
                extent = std::max(es, s);
            }
            int64_t maxk = 0;
            for (const Nxt& n : nxt)
                if (n.kind != 0) maxk = std::max(maxk, n.k);
            int64_t count = std::max<int64_t>(std::max<int64_t>(1, py::floordiv(extent, es)), maxk + 1);
            out.append(F("%s %s;", base, dcl.replace(S("[]"), F("[%d]", count)).strip()));
            for (const Nxt& n : nxt) {
                aliases[n.name] = n.kind == 0 ? c : n.kind == 1 ? F("(&%s[%d])", c, n.k) : F("%s[%d]", c, n.k);
                done.add(n.name);
            }
            continue;
        }
        if (v) {
            const str& base = std::get<0>(*v);
            str dcl = std::get<1>(*v);
            int64_t unit = old_size(base, py::re::sub(S(R"(\[\s*\])"), S(""), dcl), c);
            dcl = py::re::sub(S(R"(\[\s*\])"), F("[%d]", std::max<int64_t>(1, py::floordiv(pl.own, std::max<int64_t>(1, unit)))), dcl);
            out.append(F("%s %s;", base, dcl.strip()));
        } else {
            out.append(F("char %s[%d];\t\t/* (no C declaration says its type) */", c, std::max<int64_t>(pl.own, 1)));
        }
    }
    for (const Place& p : places)
        if (p.own) R.sizes[p.c] = p.own;
    return R;
}

// ---------------------------------------------------------------- asm_modules
namespace {
bool same_rule(const pre_ansi::DeclRule& a, const pre_ansi::DeclRule& b) {
    return !a.fn && !b.fn && a.pattern == b.pattern && a.repl == b.repl;
}
}  // namespace

// the program's own assembly language, ported as assembly language (port/asm16) and assembled; its data
// alone as C definitions. -> ([objects to link], {name: its C declaration}, [notes])
std::tuple<list<str>, dict<str, str>, list<str>>
asm_modules(pre_ansi::Port& port, const str& out, const dict<str, str>& res, const dict<str, str>& asm_, const dict<str, str>& paths,
            const Roles& roles, const RuntimeProfile& profile, const Opt& opt, const Compilers& compilers) {
    list<str> texts;
    for (const auto& kv : res.items())
        if (std::get<0>(kv).endswith(S(".c")) || std::get<0>(kv).endswith(S(".h"))) texts.append(std::get<1>(kv));
    list<str> origs;
    for (const auto& kv : port.originals.items()) origs.append(std::get<1>(kv));
    asm16::CTypes ct(texts, origs);
    list<str> blanked;
    for (const str& t : texts) blanked.append(blank(t));
    set<str> calls_set;
    for (const str& c : py::re::findall(S(R"(\b([A-Za-z_]\w*)\s*\()"), join(S("\n"), blanked))) calls_set.add(c);
    list<str> calls;
    for (const str& c : calls_set) calls.append(c);
    list<str> blanked_o;
    for (const str& t : origs) blanked_o.append(blank(t));
    set<str> far;
    for (const str& f : py::re::findall(S(R"(\bfar\b[^;(){}]*?\b(\w+)\s*\()"), join(S("\n"), blanked_o))) far.add(f);
    std::optional<str> old_asm = opt(S("--asm16"));
    if (!old_asm || old_asm->empty()) {                      // ((settings.get("port") or {}).get("asm16"))
        old_asm = std::nullopt;
        try {
            py::V s = read_json(py::os::path::join(HERE, S(".."), S(".."), S("settings.json")));
            const py::V& conf = s.get("port");
            if (conf.is_dict() && conf.get("asm16").is_str() && !conf.get("asm16").str().empty()) old_asm = str(conf.get("asm16").str());
        } catch (const py::OSError&) {
        } catch (const py::ValueError&) {
        }
    }
    std::optional<str> ml;
    if (compilers.contains(S("ml64")) && compilers.at(S("ml64")).find) ml = find(*compilers.at(S("ml64")).find);
    else ml = find(list<str>());
    list<str> objs, notes;
    dict<str, str> protos;
    dict<str, asm16::CSig> cfuncs = asm16::c_functions(origs);
    dict<str, str> declared = c_declared(origs);
    dict<str, str> runtime_mods;
    for (const auto& kv : asm_.items())
        if (std::get<0>(roles.at(std::get<0>(kv))) == "runtime") runtime_mods[std::get<0>(kv)] = std::get<1>(kv);
    dict<str, list<str>> tails = runtime_tails(runtime_mods);
    dict<str, str> asm2;
    for (const auto& kv : asm_.items()) {
        const str& n = std::get<0>(kv);
        asm2[n] = std::get<0>(roles.at(n)) == "code" ? inline_tails(std::get<1>(kv), tails, notes, n) : std::get<1>(kv);
    }
    std::map<str, set<str>> routines, externs;               // (each module's public routines: the others call them)
    for (const auto& kv : asm2.items()) {
        const str& n = std::get<0>(kv);
        if (std::get<0>(roles.at(n)) == "code") {
            asm16::Port m0(asm16::module(n, std::get<1>(kv)), ct, calls);
            set<str> r, e;
            for (const str& p : m0.public_)
                if (m0.labels.contains(p)) r.add(p);
            for (const auto& ek : m0.extrn.items()) e.add(std::get<0>(ek));
            routines[n] = r;
            externs[n] = e;
        }
    }
    std::vector<std::pair<str, str>> order;                  // (the data first: the names it places)
    for (const auto& kv : asm2.items()) order.push_back({std::get<0>(kv), std::get<1>(kv)});
    std::stable_sort(order.begin(), order.end(), [&](const auto& a, const auto& b) {
        bool da = std::get<0>(roles.at(a.first)) != "data", db = std::get<0>(roles.at(b.first)) != "data";
        if (da != db) return !da && db;
        return a.first < b.first;
    });
    for (const auto& nt : order) {
        const str& n = nt.first;
        const str& t = nt.second;
        str role = std::get<0>(roles.at(n));
        if (role == "data") {
            std::optional<str> msg = asmdata::messages(t);
            if (msg) {                                       // (the C library's numbered messages: a table of C)
                port.asm_data.append({n, *msg});
                notes.append(F("%s: the numbered messages (__NMSG_TEXT, __FMSG_TEXT): a table in data.c", n));
                continue;
            }
            std::optional<str> typed = asmdata::typed(t, ct);
            if (typed) {                                     // (tables with addresses in them: C, initialized)
                port.asm_data.append({n, *typed});
                notes.append(F("%s: tables with addresses in them - C definitions with initializers, in data.c", n));
                continue;
            }
            DataDefs dd = asm_data_defs(t, ct);
            for (const str& d : dd.defs) port.asm_data.append({n, d});
            for (const auto& kv : dd.one_place.items()) {    // (one variable: its name and type everywhere)
                const str& al = std::get<0>(kv);
                const str& canon = std::get<1>(kv);
                auto v = ct.var(canon);
                if (!v) throw py::TypeError("'NoneType' object is not subscriptable");
                str typ = (std::get<0>(*v) + S(" ") + replace1(std::get<1>(*v), canon, S(""))).strip();
                port.renames[al] = canon;
                pre_ansi::DeclRule rule(S(R"((?m)^(\s*extern\s+)[^;(=]*?\b)") + esc(canon) + S(R"(\s*;)"),
                                        S(R"(\g<1>)") + typ + (typ.endswith(S("*")) ? S("") : S(" ")) + canon + S(";"));
                bool have = false;
                for (const auto& r : port.decl_rules)
                    if (same_rule(r, rule)) have = true;
                if (!have) port.decl_rules.append(rule);
                notes.append(F("%s: %s and %s are one place - one variable, %s", n, al, canon, typ));
            }
            dict<str, int64_t> sizes = port.asm_sizes.copy();    // (dict(getattr(port, "asm_sizes", {}), **sizes))
            for (const auto& kv : dd.sizes.items()) sizes[std::get<0>(kv)] = std::get<1>(kv);
            port.asm_sizes = sizes;
            for (const auto& kv : dd.aliases.items()) {      // (names of one place: one variable)
                const str& al = std::get<0>(kv);
                const str& where = std::get<1>(kv);
                auto om = py::re::search(S(R"([A-Za-z_]\w*)"), where);
                if (!om) throw py::AttributeError("'NoneType' object has no attribute 'group'");
                str owner = om->group(0);
                std::optional<str> od;
                for (const auto& d : port.asm_data)
                    if (py::re::search(S(R"(\b)") + owner + S(R"(\s*\[)"), std::get<1>(d))) {
                        od = std::get<1>(d);
                        break;
                    }
                port.aliases[al] = {where, od ? *od : S("")};
            }
            notes.append(F("%s: data alone - its variables defined in data.c with the C program's types", n));
            continue;
        }
        if (role != "code") continue;
        list<str> others;
        {
            set<str> u;
            for (const auto& rk : routines)
                if (rk.first != n)
                    for (const str& r : rk.second) u.add(r);
            for (const str& r : u) others.append(r);
        }
        asm16::Port m(asm16::module(n, t), ct, calls, set<str>(), std::optional<dict<str, asm16::CSig>>(cfuncs), others, profile.c_underscore);
        for (const auto& ek : externs)
            if (ek.first != n)
                for (const str& e : ek.second) m.used_outside.add(e);
        m.param_types = dict<str, list<std::tuple<int64_t, std::optional<str>>>>();
        for (const auto& pc : m.cname.items()) m.param_types[std::get<0>(pc)] = call_arg_types(std::get<1>(pc), texts, origs, ct);
        m.aliases = alias_places(port.aliases, ct);
        m.data_map = data_places(paths.at(n));
        m.far.clear();
        for (const auto& pc : m.cname.items())
            if (far.contains(std::get<1>(pc))) m.far.insert(std::optional<str>(std::get<0>(pc)));
        set<str> lib;
        for (const str& s : profile.libc) lib.add(s);
        for (const char* s : {"strcmp", "strlen", "strcpy", "strcat", "memcpy", "memset", "memcmp"}) lib.add(S(s));
        for (const auto& pc : m.cname.items()) {
            const str& p = std::get<0>(pc);
            const str& c = std::get<1>(pc);
            if (lib.contains(c)) {                           // (the program's own routine of a C library name: its
                m.cname[p] = c + S("_");                     //  own kept, under a name of its own, in the C too)
                port.renames[c] = c + S("_");
                notes.append(F("%s: its own %s kept (not the C library's), as %s_", n, c, c));
            }
        }
        if (old_asm && py::os::path::isfile(*old_asm)) m.offsets = asm16::listing_offsets(paths.at(n), *old_asm);
        str text;
        try {
            m.run();
            text = m.emit();
        } catch (const asm16::Asm16Error& e) {
            notes.append(F("**%s: not ported - %s**", n, str(std::string(e.what()))));
            continue;
        }
        str base = std::get<0>(py::os::path::splitext(n)) + S("64");
        write_text(py::os::path::join(out, base + S(".asm")), text, "cp1252");
        for (const auto& pc : m.cname.items()) {
            const str& p = std::get<0>(pc);
            const str& c = std::get<1>(pc);
            std::optional<asm16::State> ex = m.exit_of(p);
            const asm16::K* k = nullptr;
            if (ex) {
                auto it = ex->r.find(S("ax"));
                if (it != ex->r.end()) k = &it->second;
            }
            str ret = (k && k->wide() && k->kind != "frame") ? S("char *") : S("int16_t");
            ret = declared.get(c, ret);                      // (the C program's own declaration says)
            protos[c] = F("%s%s%s();", ret, ret.endswith(S("*")) ? S("") : S(" "), c);
        }
        if (!ml) {
            notes.append(F("**%s: ported to %s.asm, but no ml64 to assemble it**", n, base));
            continue;
        }
        py::subprocess::RunOptions ro;
        ro.cwd = out;
        py::subprocess::CompletedProcess p = py::subprocess::run(
            list<str>{*ml, S("/nologo"), S("/c"), S("/Cp"), S("/Fo") + base + S(".obj"), base + S(".asm")}, ro);
        if (p.returncode) {
            str all = (p.stdout_ ? *p.stdout_ : S("")) + (p.stderr_ ? *p.stderr_ : S(""));
            notes.append(F("**%s: %s.asm does not assemble: %s**", n, base, all.size() > 400 ? sl(all, all.size() - 400, all.size()) : all));
            continue;
        }
        objs.append(base + S(".obj"));
        notes.append(F("%s: ported as assembly language to %s.asm (%d routines the C program calls)", n, base, m.cname.size()));
        if (!m.unreached.empty()) {                          // (code no path was found to: kept as comments - said)
            list<str> first;
            for (int64_t i = 0; i < std::min<int64_t>(12, m.unreached.size()); i++) first.append(str(std::to_string(m.unreached[i])));
            notes.append(F("**%s: %d lines no path reaches were left as comments (lines %s) - look at them**", n, m.unreached.size(),
                           join(S(", "), first)));
        }
    }
    return {objs, protos, notes};
}

// ---------------------------------------------------------------- the passes
// the records the program takes as bytes kept in their old layout, their pointers two-byte handles
// (port/oldlayout.py); -> how many lines it changed
int64_t oldlayout_pass(pre_ansi::Port& port, const str& out, const dict<str, str>& res, const Compilers& compilers,
                       dict<std::tuple<str, str>, str>* wanted) {
    std::optional<str> cc;
    if (compilers.contains(S("clang"))) {
        const Compiler& spec = compilers.at(S("clang"));
        cc = find(need_list(spec.find, "find"));
    }
    if (!cc) return 0;
    std::vector<str> fnames;
    for (const auto& kv : res.items())
        if (std::get<0>(kv).endswith(S(".c"))) fnames.push_back(std::get<0>(kv));
    std::sort(fnames.begin(), fnames.end());
    list<str> files = to_list(fnames);
    oldlayout::PTR_PARAMS = dict<str, list<bool>>();         // (the old-style functions kept: their pointer parameters)
    for (const auto& kv : port.fns.items()) {
        const pre_ansi::FnP& f = std::get<1>(kv);
        if (!f->loose) continue;
        list<bool> ptrs;
        for (const auto& pp : f->params) ptrs.append(static_cast<bool>(py::re::search(S(R"(\*|\[)"), std::get<1>(pp))));
        oldlayout::PTR_PARAMS[f->name] = ptrs;
    }
    auto [new_, olds, found, refused] = oldlayout::run(*cc, out, files);
    if (olds.empty()) return 0;
    int64_t n = 0;
    dict<str, str> texts = new_.copy();
    std::vector<str> hs;
    for (const auto& kv : res.items())
        if (std::get<0>(kv).endswith(S(".h")) || std::get<0>(kv).endswith(S(".c"))) hs.push_back(std::get<0>(kv));
    std::sort(hs.begin(), hs.end());
    for (const str& h : hs) {                                // (the records' definitions, wherever they are)
        str t = oldlayout::declare(texts.get(h, res.at(h)), olds, found);
        if (h.endswith(S(".c"))) t = oldlayout::null_reads(t);   // (a NULL read through: the null segment, as on DOS)
        if (t != res.at(h)) texts[h] = t;
    }
    for (const auto& kv : texts.items()) {
        const str& f = std::get<0>(kv);
        list<str> a_ = py::split(res.at(f), S("\n")), b_ = py::split(std::get<1>(kv), S("\n"));
        if (a_.size() != b_.size()) continue;
        for (int64_t i = 0; i < a_.size(); i++) {
            if (a_[i] != b_[i]) {                            // (its rule: the line as it was before the rules)
                std::optional<std::tuple<str, int64_t>> o;
                if (port.line_origin.contains(f)) o = port.line_origin.at(f).get(i + 1);
                port.line_fixes[o ? std::make_tuple(f, std::get<0>(*o), std::get<1>(*o)) : std::make_tuple(f, a_[i], int64_t(-1))] = b_[i];
                if (wanted) (*wanted)[std::make_tuple(f, b_[i])] = b_[i];
                n += 1;
            }
        }
    }
    std::vector<str> ov;
    for (const str& o : olds) ov.push_back(o);
    std::sort(ov.begin(), ov.end());
    port.mended[S("a record the program keeps as bytes (writes, reads, takes as char *): its old layout, its pointers two-byte handles (dos_ph, dos_hp)")]
        .append(join(S(", "), to_list(ov)));
    for (const auto& r : refused) {
        port.mended[F("**a pointer member of a record kept old, taken by %s: not converted - look at it**", std::get<2>(r) ? *std::get<2>(r) : S("None"))]
            .append(F("%s@%d", std::get<0>(r), std::get<1>(r)));
    }
    return n;
}

// arith16 over the port's C (clang reads it): each line it changed kept as a rule; -> how many casts
int64_t arith16_pass(pre_ansi::Port& port, const str& out, const dict<str, str>& res, const Compilers& compilers) {
    std::optional<str> cc = find(need_list(compilers.at(S("clang")).find, "find"));
    if (!cc) {
        port.mended[S("the 16-bit arithmetic NOT checked (no clang to read the port): comparisons of unsigned and signed 16-bit values may differ")]
            .append(S("-"));
        return 0;
    }
    std::vector<str> fnames;
    for (const auto& kv : res.items())
        if (std::get<0>(kv).endswith(S(".c"))) fnames.push_back(std::get<0>(kv));
    std::sort(fnames.begin(), fnames.end());
    dict<str, str> new_ = arith16::run(*cc, out, to_list(fnames));
    int64_t n = 0;
    for (const auto& kv : new_.items()) {
        const str& f = std::get<0>(kv);
        list<str> a_ = py::split(res.at(f), S("\n")), b_ = py::split(std::get<1>(kv), S("\n"));
        int64_t m = std::min(a_.size(), b_.size());         // (zip)
        for (int64_t i = 0; i < m; i++) {
            if (a_[i] != b_[i]) {                            // (its rule: the line as it was before the rules)
                std::optional<std::tuple<str, int64_t>> o;
                if (port.line_origin.contains(f)) o = port.line_origin.at(f).get(i + 1);
                port.line_fixes[o ? std::make_tuple(f, std::get<0>(*o), std::get<1>(*o)) : std::make_tuple(f, a_[i], int64_t(-1))] = b_[i];
                n += b_[i].count(S("_t)")) - a_[i].count(S("_t)"));
                port.mended[S("the 8086's 16-bit arithmetic kept (an unsigned met by an int was unsigned; a sum was cut to 16 bits before it was compared or made a long): a cast")]
                    .append(f);
            }
        }
    }
    return n;
}

// json.dumps(value, sort_keys=True) of a parsed JSON value (", " and ": ", ensure_ascii)
static void dumps_sorted(std::string& o, const py::V& v) {
    if (v.is_none()) o += "null";
    else if (v.is_bool()) o += v.as_int() ? "true" : "false";
    else if (v.is_int()) o += std::to_string(v.as_int());
    else if (v.is_float()) o += py::to_str(v.as_float()).utf8();
    else if (v.is_str()) py::json::put(o, str(v.str()));
    else if (v.is_seq()) {
        o += "[";
        bool first = true;
        for (const py::V& x : v.seq()) {
            if (!first) o += ", ";
            first = false;
            dumps_sorted(o, x);
        }
        o += "]";
    } else if (v.is_dict()) {
        std::vector<std::pair<std::string, const py::V*>> items;
        for (const auto& kv : v.items()) items.push_back({kv.first.str(), &kv.second});
        std::sort(items.begin(), items.end(), [](const auto& a, const auto& b) { return str(a.first) < str(b.first); });
        o += "{";
        bool first = true;
        for (const auto& kv : items) {
            if (!first) o += ", ";
            first = false;
            py::json::put(o, str(kv.first));
            o += ": ";
            dumps_sorted(o, *kv.second);
        }
        o += "}";
    }
}

// os.stat(path).st_mtime, as Python computes it (seconds + nanoseconds * 1e-9)
static double st_mtime(const str& path) {
    std::error_code ec;
    auto t = fs::last_write_time(fs::u8path(path.utf8()), ec);
    if (ec) throw py::FileNotFoundError("[WinError 2] The system cannot find the file specified: " + py::repr(path).utf8());
    int64_t ticks = std::chrono::duration_cast<std::chrono::nanoseconds>(t.time_since_epoch()).count() / 100;   // (100 ns since 1601)
    int64_t sec = ticks / 10000000 - 11644473600LL;
    int64_t nsec = (ticks % 10000000) * 100;
    return static_cast<double>(sec) + static_cast<double>(nsec) * 1e-9;
}

// everything a sanitized build and its runs depend on: the port's files, the cases and their inputs, the
// other passes' ports the cases run, the compiler
str sanitize_key(const str& out, const list<str>& srcs, const str& cases, const str& cc) {
    (void)srcs;
    py::hashlib::Hash h = py::hashlib::sha256(py::bytes(py::os::path::abspath(cc).utf8()));
    std::vector<str> names;
    for (const str& n : py::os::listdir(out)) names.push_back(n);
    std::sort(names.begin(), names.end());
    for (const str& n : names) {
        str f = py::os::path::join(out, n);
        if (py::os::path::isfile(f) && (n.endswith(S(".c")) || n.endswith(S(".h")) || n.endswith(S(".asm"))))
            h.update(py::bytes(n.utf8() + std::string(1, '\0') + read_all(f)));
    }
    try {
        py::V cfg = read_json(cases);
        std::string dumped;
        dumps_sorted(dumped, cfg);
        h.update(py::bytes(dumped));
        if (!cfg.is_dict()) {                                // (cfg.get(...) of a list, a string, a number)
            const char* kind = cfg.is_list() || cfg.is_tuple() ? "list" : cfg.is_str() ? "str" : cfg.is_bool() ? "bool" : cfg.is_int() ? "int"
                               : cfg.is_float() ? "float" : "NoneType";
            throw py::AttributeError(std::string("'") + kind + "' object has no attribute 'get'");
        }
        str base = py::os::path::dirname(py::os::path::abspath(cases));
        auto at = [&](const str& p_) { return py::os::path::isabs(p_) ? p_ : py::os::path::join(base, p_); };
        const py::V& inputs = cfg.get("inputs");
        if (inputs.is_dict()) {
            std::vector<std::pair<std::string, const py::V*>> items;
            for (const auto& kv : inputs.items()) items.push_back({kv.first.str(), &kv.second});
            std::sort(items.begin(), items.end(), [](const auto& a, const auto& b) { return str(a.first) < str(b.first); });
            for (const auto& kv : items)
                if (py::os::path::isfile(at(str(kv.second->str())))) h.update(py::bytes(read_all(at(str(kv.second->str())))));
        }
        const py::V& programs = cfg.get("programs");
        if (programs.is_dict()) {
            std::vector<std::pair<std::string, const py::V*>> items;
            for (const auto& kv : programs.items()) items.push_back({kv.first.str(), &kv.second});
            std::sort(items.begin(), items.end(), [](const auto& a, const auto& b) { return str(a.first) < str(b.first); });
            for (const auto& kv : items) {
                const py::V& pv = kv.second->get("port");
                str p = pv.is_str() ? str(pv.str()) : S("");
                if (py::os::path::isfile(at(p))) {
                    str ap = at(kv.second->at("port").is_str() ? str(kv.second->at("port").str()) : S(""));
                    h.update(py::bytes(("(" + py::repr(str(kv.first)).utf8() + ", " + std::to_string(py::os::path::getsize(ap)) + ", " +
                                        py::to_str(st_mtime(ap)).utf8() + ")")));
                }
            }
        }
    } catch (const py::OSError&) {
        h.update(py::bytes(std::string("?")));
    } catch (const py::ValueError&) {
        h.update(py::bytes(std::string("?")));
    }
    return h.hexdigest();
}

// the port built with AddressSanitizer and run on the cases until it reports nothing; -> the rounds' notes
list<str> sanitize_loop(pre_ansi::Port& port, const str& out, const std::function<dict<str, str>()>& write, const list<str>& srcs,
                        const Compilers& compilers, const str& comp, const str& cases, int64_t rounds) {
    std::vector<str> names;
    if (comp != "msvc") names.push_back(comp);
    names.push_back(S("clang"));
    names.push_back(S("gcc"));
    std::optional<str> cc;
    str name;
    Compiler spec;
    bool found = false;
    for (const str& nm : names) {
        std::optional<Compiler> sp;
        if (compilers.contains(nm)) sp = compilers.at(nm);
        std::optional<str> c = sp ? find(need_list(sp->find, "find")) : std::nullopt;
        if (c && !c->empty() && (nm != "gcc" || false)) {   // (MinGW's gcc has no sanitizer runtime: os.name == "nt")
            name = nm;
            cc = c;
            spec = *sp;
            found = true;
            break;
        }
    }
    if (!found) return list<str>{S("no compiler with AddressSanitizer found: the cases were not run sanitized")};
    std::optional<str> rt = sanitize::runtime_dir(*cc);
    str d = py::os::path::join(out, S(".sanitize"));
    py::os::makedirs(d, 0777, true);
    str exe = py::os::path::join(d, S("program.exe"));
    list<str> notes;
    write();
    str key = sanitize_key(out, srcs, cases, *cc);           // (all it would build and run: as the last time -
    str mark = py::os::path::join(out, S(".sanitized.json"));   //  what it found then, not run again)
    try {
        py::V last = read_json(mark);
        if (last.get("key").is_str() && str(last.get("key").str()) == key) {
            list<str> r = strs_of(last.get("notes"));
            r.append(S("(the port and its cases as the last run: not run again)"));
            return r;
        }
    } catch (const py::OSError&) {
    } catch (const py::ValueError&) {
    }
    bool clean = false;
    for (int64_t r = 0; r < rounds; r++) {
        base::parallel::wait_for_memory(S("sanitized build"));   // (the sanitizer's builds and runs want memory)
        write();
        list<str> tpl0;
        for (const str& a : need_list(spec.build, "build"))
            if (a != "-O1" && a != "/O1") tpl0.append(a);
        list<str> tpl;
        if (tpl0.size() > 0) tpl.append(tpl0[0]);
        tpl.append(S("-g"));
        tpl.append(S("-fsanitize=address"));
        if (name == "clang") tpl.append(S("-fsanitize=array-bounds"));
        for (int64_t i = 1; i < tpl0.size(); i++) tpl.append(tpl0[i]);
        Process p = build(spec, *cc, tpl, out, srcs, exe, py::os::path::join(d, S("obj")));
        if (p.returncode) {
            str all = p.stdout_ + p.stderr_;
            notes.append(F("the sanitized build failed: %s", all.size() > 300 ? sl(all, all.size() - 300, all.size()) : all));
            break;
        }
        dict<str, str> env;
        env[S("PATH")] = (rt ? *rt + py::os::pathsep : S("")) + py::os::environ_::get(S("PATH"), S(""));
        env[S("ASAN_OPTIONS")] = S("detect_leaks=0");
        list<std::tuple<str, str>> reports = verify::run_cases(cases, exe, env);
        list<std::tuple<str, str>> done = sanitize::mend(port, port.out, reports);
        list<str> bad;
        for (const auto& ct_ : reports)
            if (std::get<1>(ct_).contains(S("AddressSanitizer")) || std::get<1>(ct_).contains(S("runtime error: index"))) bad.append(std::get<0>(ct_));
        if (done.empty()) {
            notes.append(F("round %d: %d cases run sanitized, %s", r + 1, reports.size(),
                           bad.empty() ? S("no fault") : S("faults not mended: ") + join(S(", "), bad)));
            clean = true;
            break;
        }
        for (const auto& vw : done) notes.append(F("round %d: %s", r + 1, std::get<1>(vw)));
    }
    py::shutil::rmtree(d, true);
    if (clean) {                                             // (settled: kept, with the key of what it ran)
        try {
            std::string o = "{\"key\": ";
            py::json::put(o, sanitize_key(out, srcs, cases, *cc));
            o += ", \"notes\": [";
            bool first = true;
            for (const str& n : notes) {
                if (!first) o += ", ";
                first = false;
                py::json::put(o, n);
            }
            o += "]}";
            py::file f = py::open(mark, S("w"));
            f.write(str(o));
            f.close();
        } catch (const py::OSError&) {
        }
    }
    return notes;
}

// ---------------------------------------------------------------- the report
void report(const str& out, const str& src, const str& comp, const std::optional<str>& cc, const pre_ansi::Port& port, const Roles& roles,
            const list<Diagnostic>& diags, int64_t rounds, bool built, const str& blog, const RuntimeProfile& profile, const list<str>& sanitized,
            const list<str>& asm_notes) {
    list<str> L;
    L.append(F("# Port of `%s`", py::os::path::basename(py::os::path::normpath(src))));
    L.append(S(""));
    L.append(F("Made by CodeBreaker's port from `%s` (read only), for **%s** (`%s`).", src, comp, cc && !cc->empty() ? *cc : S("not found")));
    L.append(S(""));
    L.append(F("**%s.** %d rounds of the compiler's complaints mended.", built ? S("Built") : S("Not built"), rounds));
    L.append(S(""));
    for (const char* x : {"## Left out: the old runtime", "", "| file | why |", "|---|---|"}) L.append(S(x));
    {
        std::vector<std::pair<str, str>> lo;
        for (const auto& kv : port.left_out.items()) lo.push_back({std::get<0>(kv), std::get<1>(kv)});
        std::sort(lo.begin(), lo.end());
        for (const auto& nw : lo) L.append(F("| %s | %s |", nw.first, nw.second));
    }
    {
        std::vector<str> rn;
        for (const auto& kv : roles.items()) rn.push_back(std::get<0>(kv));
        std::sort(rn.begin(), rn.end());
        for (const str& n : rn) {
            const str& role = std::get<0>(roles.at(n));
            const list<str>& pubs = std::get<1>(roles.at(n));
            const char* why = role == "runtime" ? "assembler routines of the old runtime (%s): the C library and the platform layer stand in"
                              : role == "data"  ? "data alone (%s)"
                                                : "the program's own assembly language (%s): ported as assembly language (below)";
            str ps = join(S(", "), pubs);
            L.append(F("| %s | %s |", n, F(why, ps.empty() ? S("nothing public") : ps)));
        }
    }
    for (const char* x : {"", "## Changes", "", "| change | how many | where (first) |", "|---|---|---|"}) L.append(S(x));
    {
        std::vector<std::pair<str, list<str>>> ns;
        for (const auto& kv : port.notes.items()) ns.push_back({std::get<0>(kv), std::get<1>(kv)});
        std::stable_sort(ns.begin(), ns.end(), [](const auto& a, const auto& b) { return a.second.size() > b.second.size(); });
        for (const auto& kv : ns) {
            list<str> first;
            for (int64_t i = 0; i < std::min<int64_t>(4, kv.second.size()); i++) first.append(kv.second[i]);
            L.append(F("| %s | %d | %s |", kv.first, kv.second.size(), join(S(", "), first) + (kv.second.size() > 4 ? S(" ...") : S(""))));
        }
    }
    if (!asm_notes.empty()) {
        for (const char* x : {"", "## The program's assembly language", ""}) L.append(S(x));
        for (const str& x : asm_notes) L.append(F("- %s", x));
    }
    if (!sanitized.empty()) {
        for (const char* x : {"", "## Run under AddressSanitizer (the verify cases)", ""}) L.append(S(x));
        for (const str& s : sanitized) L.append(F("- %s", s));
    }
    for (const char* x : {"", "## Behaviour that may differ", ""}) L.append(S(x));
    for (const auto& kv : profile.notes.items()) L.append(F("- **%s**: %s", std::get<0>(kv), std::get<1>(kv)));
    L.append(F("- **int** is %d bits in the original and 32 here: arithmetic that relied on 16-bit wrap-around differs (structure members keep 16 bits); "
               "the verifier (verify.py) is what shows it.", profile.width_int));
    if (profile.char_signed) L.append(S("- **char** was signed: built with -fsigned-char where the compiler has it."));
    list<Diagnostic> left;
    for (const Diagnostic& d : diags)
        if (d.kind == "error" || d.kind == "warning") left.append(d);
    L.append(S(""));
    L.append(F("## What the compiler still says (%d)", left.size()));
    L.append(S(""));
    {
        std::vector<std::pair<str, int64_t>> cnt;           // (collections.Counter: first met first)
        for (const Diagnostic& d : left) {
            str k = py::re::sub(S(R"('[^']*')"), S("'..'"), d.text);
            bool hit = false;
            for (auto& c : cnt)
                if (c.first == k) {
                    c.second++;
                    hit = true;
                    break;
                }
            if (!hit) cnt.push_back({k, 1});
        }
        std::stable_sort(cnt.begin(), cnt.end(), [](const auto& a, const auto& b) { return a.second > b.second; });
        for (size_t i = 0; i < cnt.size() && i < 30; i++) L.append(F("- %d x %s", cnt[i].second, cnt[i].first));
    }
    if (!built && !blog.empty()) {
        for (const char* x : {"", "## Build", "", "```"}) L.append(S(x));
        L.append(blog.size() > 3000 ? sl(blog, blog.size() - 3000, blog.size()) : blog);
        L.append(S("```"));
    }
    str p = py::os::path::join(out, S("PORT.md"));
    py::file f = py::open(p, S("w"), S("utf-8"));
    f.write(join(S("\n"), L) + S("\n"));
    f.close();
}

// ---------------------------------------------------------------- main
const str DOC = S(
    "python -m codebreaker.port SOURCE_DIR OUT_DIR [--compiler gcc|clang|msvc] [--runtime NAME] [--cc PATH]\n"
    "\n"
    "SOURCE_DIR is read, never written: its C (*.c, *.h, in it or its src/) and its assembler modules (*.asm, in it or\n"
    "its asm/). OUT_DIR gets the port: the modern sources, the platform layer, a build script, the program built, and\n"
    "PORT.md - every change made and why, what the compiler still says, what could not be ported.\n"
    "\n"
    "  --cases CASES.json   the behaviour cases (port/verify): the sanitizer pass runs them\n"
    "  --asm16 ASSEMBLER    the period assembler (run under the emulator) whose listing places the assembly's tables\n"
    "  --types TYPES.json   {\"members\": {\"struct.member\": \"type\"}}: members the source declared otherwise than the\n"
    "                       program uses them (what the assembly shows) - the port's corrections, said in PORT.md\n");

// the program's main: -> the exit code, what it prints goes to `out`
int64_t main(const list<str>& argv, std::ostream& outs) {
    if (argv.size() < 2) {
        outs << DOC.utf8() << "\n";
        return 2;
    }
    str src = argv[0];
    str out = py::os::path::abspath(argv[1]);                // (the compilers run in OUT_DIR: its own path whole)
    base::parallel::throttle(std::nullopt, true);            // (the machine first; the compilers inherit it)
    memory_guard(out);                                       // (and never more of its memory than it may have)
    Opt opt = [&](const str& k) -> std::optional<str> {
        if (argv.contains(k)) return argv[argv.index(k) + 1];
        return std::nullopt;
    };
    str comp = opt(S("--compiler")).value_or(S("gcc"));
    RuntimeProfile profile = RuntimeProfile::load(py::os::path::join(py::os::path::join(HERE, S("runtimes")), opt(S("--runtime")).value_or(S("dos_c_early")) + S(".json")));
    for (const str& pj : {py::os::path::join(src, S("project.json")), py::os::path::join(py::os::path::join(src, S("src")), S("project.json"))}) {
        if (py::os::path::isfile(pj)) {                      // (the recon's build: the library's objects linked first -
            py::V doc = read_json(pj);
            std::vector<str> firsts;
            if (doc.is_dict() && doc.get("first_objects").is_list())
                for (const py::V& x : doc.get("first_objects").seq()) firsts.push_back(py::os::path::basename(str(x.str())).upper());
            if (std::find(firsts.begin(), firsts.end(), S("BINMODE.OBJ")) != firsts.end()) {   //  BINMODE.OBJ: every file binary by default)
                profile.console_text = profile.text_mode.value_or(false);
                profile.text_mode = false;
                profile.notes[S("binmode")] = S("linked with BINMODE.OBJ: files open binary by default (no \\r\\n for \\n), so here");
            }
        }
    }
    Compilers compilers = load_compilers();
    if (!compilers.contains(comp)) throw py::KeyError(py::repr(comp).utf8());
    Compiler spec = compilers.at(comp);
    std::optional<str> cc = find(need_list(spec.find, "find"), opt(S("--cc")));
    str aout = py::os::path::abspath(out), asrc = py::os::path::abspath(src);
    if (aout.startswith(asrc + py::os::sep) || aout == asrc) {
        outs << "the port is written beside the source, never into it: give another OUT_DIR\n";
        return 2;
    }
    auto [files, asm_, paths] = read_tree(src);
    dict<str, str> members;
    if (opt(S("--types")) && !opt(S("--types"))->empty()) {
        py::V types = read_json(*opt(S("--types")));
        const py::V& m = types.get("members");
        if (m.is_dict())
            for (const auto& kv : m.items()) members.set(str(kv.first.str()), str(kv.second.str()));
    }
    pre_ansi::Port port(files, pre_ansi::Profile::load(py::os::path::join(py::os::path::join(HERE, S("runtimes")), opt(S("--runtime")).value_or(S("dos_c_early")) + S(".json"))),
                        members);
    port.bss = read_bss(src);
    port.ds = read_symbols(src);
    for (const auto& kv : files.items()) {                   // (and CodeBreaker's own names: g_304a is DS 304Ah)
        for (const auto& m : py::re::finditer(S(R"(\bg_([0-9a-f]{4})\b)"), std::get<1>(kv)))
            if (!port.ds.contains(m.group(0))) port.ds[m.group(0)] = py::int_parse(m.group(1), 16);
    }
    Roles roles;
    for (const auto& kv : asm_.items()) roles[std::get<0>(kv)] = asm_role(std::get<1>(kv), profile);
    py::os::makedirs(out, 0777, true);
    for (const char* n : {"dosrt.h", "dosrt.c"}) py::shutil::copy(py::os::path::join(py::os::path::join(HERE, S("platform")), S(n)), py::os::path::join(out, S(n)));
    write_text(py::os::path::join(out, S("dosrt_config.h")),
               F("/* dosrt_config.h -- the old runtime's ways (made by CodeBreaker's port from the profile %s) */\n"
                 "#define DOSRT_TEXT_MODE %d\n#define DOSRT_CONSOLE_TEXT %d\n#define DOSRT_EXIT_STATUS %d\n",
                 profile.name, profile.text_mode.value_or(false) ? 1 : 0,
                 profile.console_text.value_or(profile.text_mode.value_or(false)) ? 1 : 0, (profile.exit_status && !*profile.exit_status) ? 0 : 1),
               "cp1252");   // (the old runtime's ways)

    std::function<dict<str, str>()> write = [&]() -> dict<str, str> {
        dict<str, str> res = port.modernize();
        for (const auto& kv : port.headers().items())
            if (std::get<1>(kv)) res[std::get<0>(kv)] = *std::get<1>(kv);
        for (const auto& kv : res.items()) write_text(py::os::path::join(out, std::get<0>(kv)), std::get<1>(kv), "latin1");
        return res;
    };

    list<str> objs, asm_notes;
    dict<str, str> res;
    bool any_asm = false;
    for (const auto& kv : roles.items())
        if (std::get<0>(std::get<1>(kv)) == "code" || std::get<0>(std::get<1>(kv)) == "data") any_asm = true;
    if (any_asm) {
        res = write();                                       // (the C as far as it goes: the types the assembly needs)
        dict<str, str> protos;
        std::tie(objs, protos, asm_notes) = asm_modules(port, out, res, asm_, paths, roles, profile, opt, compilers);
        port.asm_protos = protos;
    }
    int64_t rounds = 0;
    list<Diagnostic> diags;
    std::vector<std::pair<Compiler, std::optional<str>>> checkers;   // (then clang's eyes too: gcc does not check a call of an
    checkers.push_back({spec, cc});                                  //  old-style definition, clang does)
    if (comp != "clang" && compilers.contains(S("clang"))) {
        std::optional<str> cl = find(need_list(compilers.at(S("clang")).find, "find"));
        if (cl) checkers.push_back({compilers.at(S("clang")), cl});
    }
    for (size_t k_ = 0; k_ < checkers.size(); k_++) {
        const Compiler& spec_ = checkers[k_].first;
        const std::optional<str>& cc_ = checkers[k_].second;
        int64_t start = rounds;
        dict<str, str> seen_text;
        dict<str, list<Diagnostic>> seen_diags;              // (what each file was, and what the compiler said of it)
        double t_round = py::time::time();
        int64_t same = 0;
        std::optional<std::vector<std::tuple<str, int64_t, str>>> last_said;
        for (;;) {
            res = write();
            diags = list<Diagnostic>();
            if (!cc_) break;
            std::vector<str> cn;
            for (const auto& kv : res.items())
                if (std::get<0>(kv).endswith(S(".c"))) cn.push_back(std::get<0>(kv));
            std::sort(cn.begin(), cn.end());
            list<str> cfiles = to_list(cn);
            list<str> todo = changed_files(res, seen_text, cfiles);   // (the files whose text, or a header's they include,
            dict<str, str> keys;                                       //  changed: the others said it already - and
            for (const str& n : todo) keys[n] = check_key(spec_, *cc_, n, res);   //  what an earlier run's compiler said of the same
            for (const str& n : todo.copy()) {                         //  text, kept: not said again)
                std::optional<list<KeptDiag>> got = base::cache::value<list<KeptDiag>>(S("port-checks-cpp"), keys.at(n));
                if (got) {
                    list<Diagnostic> ds;
                    for (const KeptDiag& k : *got) ds.append(diag_of(k));
                    seen_diags[n] = ds;
                    todo.remove(n);
                }
            }
            std::string cc_str = cc_->utf8();
            list<Process> done = jobs::pool::threads(
                [&](const str& n) {
                    dict<str, str> kw;
                    kw[S("cc")] = *cc_;
                    kw[S("inc")] = S(".");
                    kw[S("src")] = n;
                    return run(spec_, args(need_list(spec_.check, "check"), kw), out, *cc_);
                },
                todo, base::parallel::workers());
            dict<str, Process> doneby;
            for (int64_t i = 0; i < todo.size(); i++) doneby[todo[i]] = done[i];
            for (const str& n : cfiles) {                    // (in the files' order, as one compiler would say them)
                if (doneby.contains(n)) {
                    seen_diags[n] = diagnostics(spec_, doneby.at(n).stdout_ + doneby.at(n).stderr_, out);
                    list<KeptDiag> kept;
                    for (const Diagnostic& d : seen_diags.at(n)) kept.append(keep_of(d));
                    base::cache::keep(S("port-checks-cpp"), keys.at(n), kept);
                }
                if (seen_diags.contains(n)) diags.extend(seen_diags.at(n));
            }
            seen_text = res.copy();
            int64_t errors = 0, warnings = 0;
            for (const Diagnostic& d : diags) (d.kind == "error" ? errors : warnings)++;
            std::cerr << F("port: round %d (%s): %d of %d files checked, %d errors, %d warnings, %.0fs\n", rounds + 1,
                           py::os::path::basename(*cc_), todo.size(), cfiles.size(), errors, warnings, py::time::time() - t_round).utf8();
            std::cerr.flush();
            t_round = py::time::time();
            if (k_ && errors == 0) break;                    // (the second compiler: its errors only)
            if (k_) {
                list<Diagnostic> only;
                for (const Diagnostic& d : diags)
                    if (d.kind == "error") only.append(d);
                diags = only;
            }
            rounds += 1;
            std::vector<std::tuple<str, int64_t, str>> said_;
            for (const Diagnostic& d : diags) said_.push_back({d.file, d.line, d.text});
            std::sort(said_.begin(), said_.end());
            same = (last_said && said_ == *last_said) ? same + 1 : 0;   // (a round that left the compiler saying all it said
            last_said = said_;                                           //  before - twice: the rules have nothing more)
            if (same >= 2) break;
            list<pre_ansi::Diag> pd;
            for (const Diagnostic& d : diags) pd.append(to_pre(d));
            if (rounds - start > 8 || !pre_ansi::mend(port, res, pd)) break;
        }
    }
    if (port.int16) {                                        // (the 8086's arithmetic, where the C of today differs)
        int64_t arith = arith16_pass(port, out, res, compilers);
        if (arith) res = write();
    }
    dict<std::tuple<str, str>, str> want;
    if (oldlayout_pass(port, out, res, compilers, &want)) {  // (records the program keeps as bytes: as they were)
        res = write();
        list<std::tuple<str, str>> lost;
        for (const auto& kv : want.items()) {
            const std::tuple<str, str>& fb = std::get<0>(kv);
            list<str> ls = py::split(res.get(std::get<0>(fb), S("")), S("\n"));
            if (!ls.contains(std::get<1>(fb))) lost.append(fb);
        }
        if (!lost.empty()) {                                 // (a line the pass mended that the next text has not)
            list<str>& m = port.mended[S("**lines the old-layout pass mended that did not reach the port - look at them**")];
            for (int64_t i = 0; i < std::min<int64_t>(40, lost.size()); i++) {
                str b = std::get<1>(lost[i]).strip();
                m.append(F("%s: %s", std::get<0>(lost[i]), b.size() > 120 ? sl(b, 0, 120) : b));
            }
            std::cerr << F("port: %d old-layout lines did not reach the port (PORT.md)\n", lost.size()).utf8();
        }
    }
    std::vector<str> csrc;
    for (const auto& kv : res.items())
        if (std::get<0>(kv).endswith(S(".c"))) csrc.push_back(std::get<0>(kv));
    std::sort(csrc.begin(), csrc.end());
    list<str> srcs = to_list(csrc);
    srcs.append(S("dosrt.c"));
    srcs.extend(objs);
    str exe = py::os::path::join(out, S("program.exe"));
    std::optional<str> cases = opt(S("--cases"));
    list<str> sanitized;
    if (cases && !cases->empty()) {                          // (what only a run shows: the sanitizer, the cases)
        sanitized = sanitize_loop(port, out, write, srcs, compilers, comp, *cases);
        res = write();
    }
    bool built = false;
    str blog;
    if (py::os::path::exists(exe)) py::os::remove(exe);      // (an old build is not this port's: a verify would run it)
    if (cc && !cc->empty()) {
        Process p = build(spec, *cc, need_list(spec.build, "build"), out, srcs, exe, py::os::path::join(out, S(".obj")));
        built = p.returncode == 0;
        blog = p.stdout_ + p.stderr_;
    }
    {
        dict<str, str> kw;
        kw[S("cc")] = py::os::path::basename((cc && !cc->empty()) ? *cc : comp);
        kw[S("inc")] = S(".");
        kw[S("out")] = py::os::path::basename(exe);
        write_text(py::os::path::join(out, S("build.sh")),
                   F("#!/bin/sh\n# the port built by %s (made by CodeBreaker's port)\ncd \"$(dirname \"$0\")\"\n%s\n", comp,
                     join(S(" "), args(need_list(spec.build, "build"), kw, srcs))),
                   "cp1252");
    }
    report(out, src, comp, cc, port, roles, diags, rounds, built, blog, profile, sanitized, asm_notes);
    int64_t errs = 0, warns = 0, cfiles_n = 0;
    for (const Diagnostic& d : diags) {
        if (d.kind == "error") errs++;
        if (d.kind == "warning") warns++;
    }
    for (const auto& kv : res.items())
        if (std::get<0>(kv).endswith(S(".c"))) cfiles_n++;
    outs << F("%s: %d files ported, %d left out (the old runtime), %d rounds; %d errors, %d warnings; %s", out, cfiles_n, port.left_out.size(), rounds,
              errs, warns, built ? S("BUILT ") + exe : S("not built"))
                .utf8()
         << "\n";
    return built ? 0 : 1;
}

}  // namespace codebreaker::port::__main__
