// census.cc -- what a Python tree uses: the standard library and outside libraries it imports and
// the attributes it reads off them, the builtins it calls, the methods it calls on values, and the
// dynamic features the dynamic mode has to carry (getattr, *args, with, generators, classes ...).
// `pyengine --census <root> [--census-out FILE]` writes it as Markdown (see census.hpp).
#include "census.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <map>
#include <set>
#include <sstream>

#include "astutil.hpp"
#include "pyfront/pyfront.hpp"

namespace pyengine {

namespace fs = std::filesystem;
using namespace pyfront;

// sys.stdlib_module_names (3.14), the top-level names
static const std::set<std::string>& stdlib_names() {
    static const std::set<std::string> s = {
        "__future__", "_thread", "abc", "argparse", "array", "ast", "asyncio", "atexit", "base64",
        "bdb", "binascii", "bisect", "builtins", "bz2", "calendar", "cmath", "cmd", "code", "codecs",
        "collections", "colorsys", "compileall", "concurrent", "configparser", "contextlib",
        "contextvars", "copy", "copyreg", "cProfile", "csv", "ctypes", "curses", "dataclasses",
        "datetime", "dbm", "decimal", "difflib", "dis", "doctest", "email", "encodings", "enum",
        "errno", "faulthandler", "filecmp", "fileinput", "fnmatch", "fractions", "ftplib",
        "functools", "gc", "getopt", "getpass", "gettext", "glob", "graphlib", "gzip", "hashlib",
        "heapq", "hmac", "html", "http", "imaplib", "importlib", "inspect", "io", "ipaddress",
        "itertools", "json", "keyword", "linecache", "locale", "logging", "lzma", "mailbox",
        "marshal", "math", "mimetypes", "mmap", "msvcrt", "multiprocessing", "netrc", "numbers",
        "operator", "optparse", "os", "pathlib", "pdb", "pickle", "pickletools", "pkgutil",
        "platform", "plistlib", "poplib", "posixpath", "ntpath", "pprint", "profile", "pstats",
        "py_compile", "pyclbr", "queue", "quopri", "random", "re", "reprlib", "resource",
        "runpy", "sched", "secrets", "select", "selectors", "shelve", "shlex", "shutil", "signal",
        "site", "smtplib", "socket", "socketserver", "sqlite3", "ssl", "stat", "statistics",
        "string", "stringprep", "struct", "subprocess", "symtable", "sys", "sysconfig", "tabnanny",
        "tarfile", "tempfile", "textwrap", "threading", "time", "timeit", "tkinter", "token",
        "tokenize", "tomllib", "trace", "traceback", "tracemalloc", "turtle", "types", "typing",
        "unicodedata", "unittest", "urllib", "uuid", "venv", "warnings", "wave", "weakref",
        "webbrowser", "winreg", "winsound", "wsgiref", "xml", "xmlrpc", "zipapp", "zipfile",
        "zipimport", "zlib", "zoneinfo", "_winapi", "nt", "posix", "opcode", "sre_parse",
        "sre_compile", "sre_constants", "_ctypes", "lib2to3", "distutils", "imp"};
    return s;
}

static const std::set<std::string>& builtin_names() {
    static const std::set<std::string> s = {
        "abs", "aiter", "all", "anext", "any", "ascii", "bin", "bool", "breakpoint", "bytearray",
        "bytes", "callable", "chr", "classmethod", "compile", "complex", "delattr", "dict", "dir",
        "divmod", "enumerate", "eval", "exec", "filter", "float", "format", "frozenset", "getattr",
        "globals", "hasattr", "hash", "help", "hex", "id", "input", "int", "isinstance",
        "issubclass", "iter", "len", "list", "locals", "map", "max", "memoryview", "min", "next",
        "object", "oct", "open", "ord", "pow", "print", "property", "range", "repr", "reversed",
        "round", "set", "setattr", "slice", "sorted", "staticmethod", "str", "sum", "super",
        "tuple", "type", "vars", "zip", "__import__", "NotImplemented", "Ellipsis", "__name__",
        "__file__", "__doc__", "exit", "quit"};
    return s;
}

struct Census {
    // module -> files importing it / references to it
    std::map<std::string, std::set<std::string>> mod_files;
    std::map<std::string, int> mod_refs;
    std::map<std::string, int> api;               // "os.path.join" -> uses
    std::map<std::string, int> builtins;
    std::map<std::string, int> methods;           // .name( on a value that is not a module
    std::map<std::string, int> features;
    std::map<std::string, int> decorators;
    std::map<std::string, int> pkg_lines;
    std::map<std::string, std::string> mod_kind;  // stdlib | third-party | internal
    int files = 0, lines = 0;
};

static std::string dotted(const Expr* e) {
    if (auto* n = as<Name>(e)) return n->id;
    if (auto* a = as<Attribute>(e)) {
        std::string b = dotted(a->value);
        return b.empty() ? "" : b + "." + a->attr;
    }
    return "";
}

static std::set<std::string> g_local_tops;   // the trees' own module names (tests/cases.py: cases)

static std::string kind_of(const std::string& top, const std::string& internal_top) {
    if (top == internal_top || top.empty()) return "internal";
    if (stdlib_names().count(top)) return "stdlib";
    if (g_local_tops.count(top)) return "internal";
    return "third-party";
}

static void census_file(Census& c, const std::string& path, const std::string& rel, const std::string& internal_top) {
    std::ifstream f(path, std::ios::binary);
    std::stringstream ss;
    ss << f.rdbuf();
    std::string src = ss.str();
    auto r = pyfront::parse(src, path);
    if (!r) return;
    int nlines = (int)std::count(src.begin(), src.end(), '\n');
    c.files++;
    c.lines += nlines;
    std::string pkg = rel;
    {
        auto p = rel.find('/');
        if (p != std::string::npos) {
            auto q = rel.find('/', p + 1);
            pkg = q == std::string::npos ? rel.substr(0, p) : rel.substr(0, q);
        }
    }
    c.pkg_lines[pkg] += nlines;

    // names bound to modules (anywhere in the file: imports inside functions included)
    std::map<std::string, std::string> alias;   // local name -> dotted module (or module.attr for from-imports)
    std::set<std::string> is_module;            // local names that are modules
    const Module& m = *r.module;
    for (const Stmt* s : m.body) {
        for (const AnyNode& n : walk(AnyNode(s))) {
            auto* st = n.stmt();
            if (!st) continue;
            if (auto* im = as<Import>(st)) {
                for (auto* a : im->names) {
                    std::string top = a->name.substr(0, a->name.find('.'));
                    std::string k = kind_of(top, internal_top);
                    c.mod_kind[a->name] = k;
                    c.mod_files[a->name].insert(rel);
                    if (a->has_asname) { alias[a->asname] = a->name; is_module.insert(a->asname); }
                    else { alias[top] = top; is_module.insert(top); }
                }
            } else if (auto* im = as<ImportFrom>(st)) {
                std::string mod = im->level ? std::string(im->level, '.') + im->module : im->module;
                std::string top = im->level ? "" : mod.substr(0, mod.find('.'));
                std::string k = kind_of(top, internal_top);
                for (auto* a : im->names) {
                    std::string full = mod + "." + a->name;
                    std::string local = a->has_asname ? a->asname : a->name;
                    if (k == "internal") continue;
                    c.mod_kind[mod] = k;
                    c.mod_files[mod].insert(rel);
                    alias[local] = full;
                    // `from os import path` / `from concurrent import futures`: a module
                    if (stdlib_names().count(a->name) || (mod == "os" && a->name == "path") ||
                        (mod == "xml.etree" || mod == "concurrent" || mod == "ctypes" && a->name == "wintypes"))
                        is_module.insert(local);
                }
            }
        }
    }

    for (const Stmt* s : m.body) {
        for (const AnyNode& n : walk(AnyNode(s))) {
            if (auto* e = n.expr()) {
                switch (e->kind) {
                case ExprKind::Attribute: {
                    auto* a = cast<Attribute>(e);
                    // the longest dotted chain rooted at a module name: count only at its top
                    std::string d = dotted(a);
                    if (!d.empty()) {
                        std::string root = d.substr(0, d.find('.'));
                        auto it = alias.find(root);
                        if (it != alias.end() && is_module.count(root)) {
                            std::string full = it->second + d.substr(root.size());
                            // count the chain once: os.path.join is one use, not os.path too
                            c.api[full]++;
                            c.mod_refs[it->second]++;
                        }
                    }
                    break;
                }
                case ExprKind::Name: {
                    auto* nm = cast<Name>(e);
                    if (nm->ctx != ExprCtx::Load) break;
                    auto it = alias.find(nm->id);
                    if (it != alias.end() && !is_module.count(nm->id)) {
                        c.api[it->second]++;
                        c.mod_refs[it->second.substr(0, it->second.rfind('.'))]++;
                    }
                    if (builtin_names().count(nm->id)) c.builtins[nm->id]++;
                    break;
                }
                case ExprKind::Call: {
                    auto* call = cast<Call>(e);
                    if (auto* a = as<Attribute>(call->func)) {
                        std::string d = dotted(a->value);
                        std::string root = d.substr(0, d.find('.'));
                        if (!(alias.count(root) && is_module.count(root))) c.methods[a->attr]++;
                    }
                    if (auto* nm = as<Name>(call->func)) {
                        static const std::set<std::string> dyn = {"getattr", "setattr", "hasattr", "delattr", "id", "next", "iter",
                                                                  "type", "vars", "globals", "locals", "exec", "eval", "compile",
                                                                  "super", "isinstance", "callable", "__import__"};
                        if (dyn.count(nm->id)) c.features["call " + nm->id + "()"]++;
                    }
                    for (auto* x : call->args) if (x->kind == ExprKind::Starred) c.features["call with *args"]++;
                    for (auto* k : call->keywords) {
                        if (!k->has_arg) c.features["call with **kwargs"]++;
                        else c.features["call with keyword"]++;
                    }
                    break;
                }
                case ExprKind::Lambda: c.features["lambda"]++; break;
                case ExprKind::ListComp: c.features["list comprehension"]++; break;
                case ExprKind::SetComp: c.features["set comprehension"]++; break;
                case ExprKind::DictComp: c.features["dict comprehension"]++; break;
                case ExprKind::GeneratorExp: c.features["generator expression"]++; break;
                case ExprKind::Yield: c.features["yield"]++; break;
                case ExprKind::YieldFrom: c.features["yield from"]++; break;
                case ExprKind::Await: c.features["await"]++; break;
                case ExprKind::JoinedStr: c.features["f-string"]++; break;
                case ExprKind::NamedExpr: c.features["walrus :="]++; break;
                case ExprKind::Slice: c.features["slice"]++; break;
                case ExprKind::Starred: if (cast<Starred>(e)->ctx == ExprCtx::Store) c.features["starred assignment"]++; break;
                case ExprKind::BinOp:
                    if (cast<BinOp>(e)->op == OpKind::Mod && is_str_const(cast<BinOp>(e)->left)) c.features["% formatting"]++;
                    if (cast<BinOp>(e)->op == OpKind::Pow) c.features["**"]++;
                    break;
                case ExprKind::Compare:
                    if (cast<Compare>(e)->ops.size() > 1) c.features["chained comparison"]++;
                    break;
                case ExprKind::BoolOp: c.features["and/or"]++; break;
                default: break;
                }
            } else if (auto* st = n.stmt()) {
                switch (st->kind) {
                case StmtKind::FunctionDef:
                case StmtKind::AsyncFunctionDef: {
                    auto* fd = cast<FunctionDef>(st);
                    c.features[st->kind == StmtKind::AsyncFunctionDef ? "async def" : "def"]++;
                    if (fd->args->vararg) c.features["def with *args"]++;
                    if (fd->args->kwarg) c.features["def with **kwargs"]++;
                    if (!fd->args->kwonlyargs.empty()) c.features["def with keyword-only"]++;
                    if (!fd->args->defaults.empty()) c.features["def with defaults"]++;
                    for (auto* d : fd->decorator_list) {
                        const Expr* de = d;
                        if (auto* call = as<Call>(de)) de = call->func;
                        std::string nm = dotted(de);
                        c.decorators[nm.empty() ? "(expression)" : nm]++;
                    }
                    break;
                }
                case StmtKind::ClassDef: {
                    auto* cd = cast<ClassDef>(st);
                    c.features["class"]++;
                    if (!cd->bases.empty()) c.features["class with bases"]++;
                    for (auto* k : cd->keywords) c.features["class keyword " + k->arg]++;
                    for (auto* d : cd->decorator_list) {
                        const Expr* de = d;
                        if (auto* call = as<Call>(de)) de = call->func;
                        std::string nm = dotted(de);
                        c.decorators["class @" + (nm.empty() ? "(expression)" : nm)]++;
                    }
                    break;
                }
                case StmtKind::With: c.features["with"]++; break;
                case StmtKind::Try: {
                    auto* t = cast<Try>(st);
                    c.features["try"]++;
                    if (!t->finalbody.empty()) c.features["try/finally"]++;
                    if (!t->orelse.empty()) c.features["try/else"]++;
                    for (auto* h : t->handlers) {
                        if (h->has_name) c.features["except ... as"]++;
                        if (!h->type) c.features["bare except"]++;
                    }
                    break;
                }
                case StmtKind::Raise: c.features[cast<Raise>(st)->cause ? "raise from" : "raise"]++; break;
                case StmtKind::Global: c.features["global"]++; break;
                case StmtKind::Nonlocal: c.features["nonlocal"]++; break;
                case StmtKind::Match: c.features["match"]++; break;
                case StmtKind::Delete: c.features["del"]++; break;
                case StmtKind::Assert: c.features["assert"]++; break;
                case StmtKind::For: if (!cast<For>(st)->orelse.empty()) c.features["for/else"]++; break;
                case StmtKind::While: if (!cast<While>(st)->orelse.empty()) c.features["while/else"]++; break;
                case StmtKind::AnnAssign: c.features["annotated assignment"]++; break;
                default: break;
                }
            }
        }
    }
}

template <class M> static std::vector<std::pair<std::string, int>> by_count(const M& m) {
    std::vector<std::pair<std::string, int>> v;
    for (auto& [k, n] : m) v.push_back({k, (int)n});
    std::stable_sort(v.begin(), v.end(), [](auto& a, auto& b) { return a.second > b.second; });
    return v;
}

std::string run_census(const std::vector<std::string>& roots, const std::string& internal_top) {
    Census c;
    g_local_tops.clear();
    for (const std::string& root : roots) {
        g_local_tops.insert(fs::absolute(root).filename().string());
        for (auto& e : fs::recursive_directory_iterator(root)) {
            if (e.is_regular_file() && e.path().extension() == ".py") g_local_tops.insert(e.path().stem().string());
            if (e.is_directory()) g_local_tops.insert(e.path().filename().string());
        }
    }
    for (const std::string& root : roots) {
        fs::path base = fs::absolute(root).parent_path();
        std::vector<fs::path> files;
        for (auto& e : fs::recursive_directory_iterator(root))
            if (e.is_regular_file() && e.path().extension() == ".py") files.push_back(fs::absolute(e.path()));
        std::sort(files.begin(), files.end());
        for (auto& p : files) {
            std::string rel = fs::relative(p, base).generic_string();
            census_file(c, p.string(), rel, internal_top);
        }
    }
    std::ostringstream o;
    o << "# Census: what CodeBreaker's Python uses\n\n";
    o << "Written by `pyengine --census` (native/pyengine/src/census.cc) over ";
    for (size_t i = 0; i < roots.size(); i++) o << (i ? ", " : "") << "`" << fs::path(roots[i]).filename().string() << "/`";
    o << ": " << c.files << " files, " << c.lines << " lines.\n\n";

    for (const char* kind : {"stdlib", "third-party"}) {
        o << "## " << (std::string(kind) == "stdlib" ? "Standard library" : "Outside libraries") << " modules\n\n";
        o << "| module | files importing | references |\n|---|---|---|\n";
        std::vector<std::pair<std::string, int>> rows;
        for (auto& [mod, k] : c.mod_kind)
            if (k == kind) rows.push_back({mod, (int)c.mod_files[mod].size()});
        std::stable_sort(rows.begin(), rows.end(), [](auto& a, auto& b) { return a.second > b.second; });
        for (auto& [mod, n] : rows) o << "| " << mod << " | " << n << " | " << c.mod_refs[mod] << " |\n";
        o << "\n";
    }
    o << "## Library API used (module attribute or imported name, uses)\n\n";
    {
        // grouped by top module
        std::map<std::string, std::vector<std::pair<std::string, int>>> grp;
        for (auto& [k, n] : by_count(c.api)) {
            std::string top = k.substr(0, k.find('.'));
            if (top.empty() || top[0] == '.') continue;
            std::string kind = kind_of(top, internal_top);
            if (kind == "internal") continue;
            grp[top].push_back({k, n});
        }
        std::vector<std::pair<std::string, int>> tops;
        for (auto& [t, v] : grp) {
            int s = 0;
            for (auto& x : v) s += x.second;
            tops.push_back({t, s});
        }
        std::stable_sort(tops.begin(), tops.end(), [](auto& a, auto& b) { return a.second > b.second; });
        for (auto& [t, s] : tops) {
            o << "- **" << t << "** (" << s << "): ";
            int i = 0;
            for (auto& [k, n] : grp[t]) o << (i++ ? ", " : "") << k.substr(t.size() + (k.size() > t.size() ? 1 : 0)) << " " << n;
            o << "\n";
        }
        o << "\n";
    }
    o << "## Builtins called or read\n\n";
    {
        int i = 0;
        for (auto& [k, n] : by_count(c.builtins)) o << (i++ ? ", " : "") << k << " " << n;
        o << "\n\n";
    }
    o << "## Methods called on values (top 200)\n\n";
    {
        int i = 0;
        for (auto& [k, n] : by_count(c.methods)) {
            if (i >= 200) break;
            o << (i++ ? ", " : "") << k << " " << n;
        }
        o << "\n\n";
    }
    o << "## Language features\n\n| feature | uses |\n|---|---|\n";
    for (auto& [k, n] : by_count(c.features)) o << "| " << k << " | " << n << " |\n";
    o << "\n## Decorators\n\n";
    {
        int i = 0;
        for (auto& [k, n] : by_count(c.decorators)) o << (i++ ? ", " : "") << k << " " << n;
        o << "\n\n";
    }
    o << "## Lines by package\n\n| package | lines |\n|---|---|\n";
    for (auto& [k, n] : by_count(c.pkg_lines)) o << "| " << k << " | " << n << " |\n";
    return o.str();
}

}  // namespace pyengine
