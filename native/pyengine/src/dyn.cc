// dyn.cc -- the dynamic mode (see dyn.hpp): each Python module written as one C++ file over py::var.
//
// The shapes, all readable as the Python they come from:
//   module names     `static py::var& name = py_module_.global("name");` -- cells, so `mod.name = x`
//                    from outside is what the code reads; the module's statements run in py_init_()
//   functions        `static py::var name(py::var a, py::var b)` at file scope, made a Python function
//                    object by `py::def("name", "a, b=", {defaults}, name)` when the def runs; a module
//                    function bound only by its def is called directly (`name(x, y)`)
//   methods          `static py::var Class__method(py::var self, ...)`, put in the class namespace
//   closures         C++ lambdas; the variables they share with their definer are py::cell
//   generators       the body yields through a py::yielder (the runtime runs it on a fiber)
//   async def        a coroutine (an async generator when it yields) on the same fibers: `await x` is
//                    y.await_(x); async for / async with await __anext__ / __aenter__ / __aexit__ through it
//   match            the cases tested in order, each pattern lowered to tests on the subject
//   classes          the body fills a namespace dict, py::make_class makes the class
//   try / with       C++ try/catch on py::error; finally and with-exit run on every way out
//                    (return / break / continue go through them)
#include "dyn.hpp"

#include <algorithm>
#include <cstdio>
#include <deque>
#include <filesystem>
#include <fstream>
#include <functional>
#include <map>
#include <set>
#include <sstream>

#include "astutil.hpp"
#include "pyfront/pyfront.hpp"
#include "islands.hpp"
#include "scopes.hpp"

namespace pyengine {

namespace fs = std::filesystem;
using namespace pyfront;

// ------------------------------------------------------------------------------------------ names
static const std::set<std::string>& cpp_reserved() {
    static const std::set<std::string> s = {
        "alignas", "alignof", "and", "and_eq", "asm", "auto", "bitand", "bitor", "bool", "case", "catch", "char", "char8_t",
        "char16_t", "char32_t", "class", "compl", "concept", "const", "consteval", "constexpr", "constinit", "const_cast",
        "continue", "co_await", "co_return", "co_yield", "decltype", "default", "delete", "do", "double", "dynamic_cast", "else",
        "enum", "explicit", "export", "extern", "false", "float", "for", "friend", "goto", "if", "inline", "int", "long",
        "mutable", "namespace", "new", "noexcept", "not", "not_eq", "nullptr", "operator", "or", "or_eq", "private", "protected",
        "public", "register", "reinterpret_cast", "requires", "return", "short", "signed", "sizeof", "static", "static_assert",
        "static_cast", "struct", "switch", "template", "this", "thread_local", "throw", "true", "try", "typedef", "typeid",
        "typename", "union", "unsigned", "using", "virtual", "void", "volatile", "wchar_t", "while", "xor", "xor_eq",
        // the C library's macros and names the runtime's headers bring
        "errno", "stdin", "stdout", "stderr", "EOF", "NULL", "assert", "offsetof", "va_arg", "va_start", "va_end", "min", "max",
        "py", "std", "main", "near", "far", "small", "interface", "IN", "OUT", "OPTIONAL", "TRUE", "FALSE", "DELETE", "ERROR",
        "environ", "unix", "linux", "_WIN32", "BUFSIZ", "FILENAME_MAX", "CHAR_BIT", "INT_MAX", "INT_MIN", "SIZE_MAX", "EXIT_SUCCESS",
        "EXIT_FAILURE", "RAND_MAX", "MAX_PATH", "INFINITY", "NAN", "HUGE_VAL", "signbit", "isnan", "isinf", "isfinite",
        "fpclassify", "setjmp", "longjmp", "y", "py_module_", "py_init_",
        // MSVC's own keywords and their old spellings (cdecl, _asm ...: synonyms of __cdecl, __asm unless /Za)
        "cdecl", "_cdecl", "_asm", "_fastcall", "_stdcall", "_thiscall", "_vectorcall", "_inline", "_forceinline", "_based",
        "_declspec", "_except", "_finally", "_try", "_leave", "_int8", "_int16", "_int32", "_int64", "_alignof", "_uuidof",
        "_ptr32", "_ptr64", "_restrict", "_sptr", "_uptr", "_unaligned", "_w64", "_pascal", "_fortran", "_syscall",
        // <cmath>, <cerrno>, <cstdio>, <climits>, <cstdint>, <cfloat> macros
        "OVERFLOW", "UNDERFLOW", "DOMAIN", "SING", "TLOSS", "PLOSS", "EPERM", "ENOENT", "ESRCH", "EINTR", "EIO", "ENXIO", "E2BIG",
        "ENOEXEC", "EBADF", "ECHILD", "EAGAIN", "ENOMEM", "EACCES", "EFAULT", "EBUSY", "EEXIST", "EXDEV", "ENODEV", "ENOTDIR",
        "EISDIR", "ENFILE", "EMFILE", "ENOTTY", "EFBIG", "ENOSPC", "ESPIPE", "EROFS", "EMLINK", "EPIPE", "EDOM", "EDEADLK",
        "ENAMETOOLONG", "ENOLCK", "ENOSYS", "ENOTEMPTY", "EINVAL", "ERANGE", "EILSEQ", "STRUNCATE", "SEEK_SET", "SEEK_CUR",
        "SEEK_END", "FOPEN_MAX", "TMP_MAX", "L_tmpnam", "WEOF", "WCHAR_MIN", "WCHAR_MAX", "CHAR_MIN", "CHAR_MAX", "SCHAR_MIN",
        "SCHAR_MAX", "UCHAR_MAX", "SHRT_MIN", "SHRT_MAX", "USHRT_MAX", "UINT_MAX", "LONG_MIN", "LONG_MAX", "ULONG_MAX", "LLONG_MIN",
        "LLONG_MAX", "ULLONG_MAX", "INT8_MIN", "INT8_MAX", "INT16_MIN", "INT16_MAX", "INT32_MIN", "INT32_MAX", "INT64_MIN",
        "INT64_MAX", "UINT8_MAX", "UINT16_MAX", "UINT32_MAX", "UINT64_MAX", "INT64_C", "UINT64_C", "INTMAX_MAX", "PTRDIFF_MIN",
        "PTRDIFF_MAX", "DBL_MAX", "DBL_MIN", "DBL_EPSILON", "FLT_MAX", "FLT_MIN", "FLT_EPSILON", "DBL_DIG", "FLT_DIG", "CLOCKS_PER_SEC",
        "MB_LEN_MAX", "_IOFBF", "_IOLBF", "_IONBF", "M_PI", "MATH_ERRNO", "FP_NAN", "FP_INFINITE", "FP_ZERO", "FP_NORMAL",
        "FP_SUBNORMAL", "HUGE_VALF", "HUGE_VALL", "LC_ALL", "LC_CTYPE", "LC_NUMERIC", "SIGINT", "SIGTERM", "SIGSEGV", "SIGABRT",
        "SIGFPE", "SIGILL", "WIN32", "_WIN64", "CONST", "VOID", "BOOL", "BYTE", "WORD", "DWORD", "UINT", "LONG", "SHORT", "CHAR",
        "INT", "FLOAT", "PVOID", "HANDLE"};
    return s;
}
static std::string ident(const std::string& n) {
    std::string o;
    for (unsigned char c : n) {
        if (isalnum(c) || c == '_') o.push_back((char)c);
        else {
            char b[8];
            snprintf(b, sizeof b, "u%02X", c);
            o += b;
        }
    }
    if (cpp_reserved().count(o)) o = "py_" + o;
    // errno names (EOPNOTSUPP, ENOTSOCK ...): <errno.h> has dozens as macros beyond the list above; the C++
    // name is only the cell's handle (the Python name is kept), so every E<CAPS> name is renamed
    else if (o.size() > 2 && o[0] == 'E' && std::all_of(o.begin() + 1, o.end(), [](char c) { return isupper((unsigned char)c) || isdigit((unsigned char)c); }))
        o = "py_" + o;
    // likewise the families <sys/stat.h> and <fcntl.h> define (S_IFDIR, S_IREAD ..., O_BINARY ...)
    else if ((o.compare(0, 3, "S_I") == 0 || o.compare(0, 2, "O_") == 0 || o == "MB_CUR_MAX") &&
             std::all_of(o.begin(), o.end(), [](char c) { return isupper((unsigned char)c) || isdigit((unsigned char)c) || c == '_'; }))
        o = "py_" + o;
    if (o.size() >= 2 && o[0] == '_' && (o[1] == '_' || isupper((unsigned char)o[1]))) o = "py" + o;   // (reserved in C++)
    return o;
}
// a direct call of a module function whose name is also one of py::'s free functions (py_object.hpp): the
// name in parentheses, so argument-dependent lookup does not bring py::'s in beside it (`(is_int)(t)`)
static std::string adl_safe(const std::string& n) {
    static const std::set<std::string> s = {
        "abs", "add", "advance_", "and_", "as_double", "as_int64", "ascii", "assert_", "attr", "bin", "builtin", "bytes_from",
        "call", "call_", "callable", "callmethod_", "chr", "class_name", "classmethod", "current_exception", "def",
        "def_generator", "delattr", "delitem", "dict", "dir", "divmod", "eq", "floordiv", "flt", "fmt", "format",
        "frozenset_from", "fstr", "getattr", "getitem", "global", "hasattr", "hash", "hash_", "hex", "id", "import",
        "import_from", "import_star", "import_top", "in", "is", "is_bool", "is_bytes", "is_dict", "is_float", "is_int", "is_list",
        "is_none", "is_not", "is_ref", "is_small_int", "is_str", "is_tuple", "is_unbound", "isinstance", "issubclass", "iter",
        "len", "len_", "list", "list_from", "lt", "make_class", "make_function", "matches", "matmul", "module", "native",
        "new_dict", "next", "next_", "not_", "not_in", "oct", "operator", "or_", "ord", "pow", "print", "print_", "property",
        "raise", "raise_from", "range", "repr", "reraise", "round", "set", "setattr", "setitem", "slice", "slice_obj", "small",
        "staticmethod", "str", "str_from", "super_", "to_vector", "truthy", "truthy_", "tuple", "type", "unpack", "vars",
        "with_enter", "yield_from", "each", "kw", "star"};
    return s.count(n) ? "(" + n + ")" : n;
}
// a line of Python as a // comment: no trailing blanks, and no trailing backslash (it would splice the
// comment's next line, the code, into it)
static std::string comment_text(std::string l) {
    while (!l.empty() && (l.back() == 13 || l.back() == 32 || l.back() == 9 || l.back() == 92)) l.pop_back();
    return l;
}
// Class.method -> Class__method
static std::string dotted_ident(std::string n) {
    std::string o;
    for (char c : n) { if (c == (char)46) o += "__"; else o.push_back(c); }
    return ident(o);
}
static std::string cstr(const std::string& s, bool bytes = false) {
    std::string o = "\"";
    bool prev_hex = false;
    for (unsigned char c : s) {
        bool hexdigit = isxdigit(c) != 0;
        if (c == '"') o += "\\\"";
        else if (c == '\\') o += "\\\\";
        else if (c == '\n') o += "\\n";
        else if (c == '\t') o += "\\t";
        else if (c == '\r') o += "\\r";
        else if (c < 0x20 || c == 0x7F || (bytes && c >= 0x80)) {
            char b[8];
            snprintf(b, sizeof b, "\\%03o", c);
            o += b;
            prev_hex = false;
            continue;
        } else {
            (void)hexdigit;
            o.push_back((char)c);
        }
        (void)prev_hex;
    }
    return o + "\"";
}
static std::string slit(const std::string& s) { return cstr(s) + "_s"; }

// ------------------------------------------------------------------------------------------ the module set
struct ModSpec {
    std::string name, file;
    bool package = false;
};
static bool find_module(const std::string& root, const std::string& dotted, ModSpec& m) {
    std::string rel = dotted;
    std::replace(rel.begin(), rel.end(), '.', '/');
    fs::path base = fs::path(root) / rel;
    if (fs::exists(base / "__init__.py")) {
        m = {dotted, (base / "__init__.py").generic_string(), true};
        return true;
    }
    fs::path f = base;
    f += ".py";
    if (fs::exists(f)) {
        m = {dotted, f.generic_string(), false};
        return true;
    }
    return false;
}
static std::string read_file(const std::string& p) {
    std::ifstream f(p, std::ios::binary);
    std::stringstream ss;
    ss << f.rdbuf();
    return ss.str();
}
static std::string resolve_relative(const std::string& mod, bool package, int level, const std::string& name) {
    std::string base = mod;
    if (!package) base = base.rfind('.') == std::string::npos ? "" : base.substr(0, base.rfind('.'));
    for (int i = 1; i < level; i++) base = base.rfind('.') == std::string::npos ? "" : base.substr(0, base.rfind('.'));
    if (name.empty()) return base;
    return base.empty() ? name : base + "." + name;
}

// ------------------------------------------------------------------------------------------ the emitter
struct Region {          // a loop, or a try/finally or a with: what return / break / continue go through
    int id = 0;
    bool is_loop = false;
    std::string else_flag;                      // a loop with an else: set when it breaks
    bool used_ret = false, used_brk = false, used_cnt = false;
};

class Emitter {
public:
    Emitter(const ModSpec& m, const std::string& src, const Module& tree, DynModule& res) : m_(m), src_(src), tree_(tree), res_(res) {
        sc_.build(tree);
        starts_ = line_starts(src);
    }
    std::string run();
    // --dyn-typed: the typed islands to write (log: qualname -> "" written, or why this module's def could not be)
    void use_islands(const IslandSet* s, std::map<std::string, std::string>* log) {
        islands_ = s;
        island_log_ = log;
    }

private:
    const ModSpec& m_;
    const std::string& src_;
    const Module& tree_;
    DynModule& res_;
    Scopes sc_;
    std::vector<size_t> starts_;
    int tmp_ = 0;
    std::string protos_, funcs_, globals_, statics_;
    std::set<std::string> global_names_;
    std::map<std::string, int> module_binds_;        // name -> binding sites at module scope
    std::set<std::string> def_only_;                 // module functions bound only by their def
    std::set<std::string> def_marks_;                // (those called directly: `name_def_` holds the function object its def made)
    void def_mark(const std::string& n) {
        if (def_marks_.insert(n).second) globals_ += "static py::Obj* " + ident(n) + "_def_ = nullptr;\n";
    }
    std::map<std::string, const FunctionDef*> module_defs_;
    std::set<std::pair<const Scope*, std::string>> cells_;
    std::set<std::string> used_fn_names_;
    const IslandSet* islands_ = nullptr;
    std::map<std::string, std::string>* island_log_ = nullptr;
    bool wrote_island_ = false;

    // the function being written
    struct Fn {
        const Scope* scope = nullptr;
        bool generator = false;
        bool async_ = false;            // async def: await allowed (a coroutine or an async generator)
        bool file_scope = true;        // a static function (false: a lambda)
        std::string class_var;          // the class being built (class bodies): its namespace var
        std::string class_cell;         // the class object for super() / __class__
        std::string self_name;          // the first parameter
        std::string qual;               // its __qualname__ (nested defs: outer.<locals>.inner)
        std::vector<Region> regions;    // innermost last (loops and protected blocks)
        int loop_depth = 0;
    };
    Fn* fn_ = nullptr;
    // private names: inside a class (its body and the functions in it) __name is _Class__name, as Python mangles
    std::vector<std::string> classes_;
    std::string mangle(const std::string& n) const {
        if (classes_.empty() || n.size() < 3 || n.compare(0, 2, "__") != 0 || (n.size() >= 4 && n.compare(n.size() - 2, 2, "__") == 0) ||
            n.find('.') != std::string::npos)
            return n;
        std::string c = classes_.back();
        size_t k = c.find_first_not_of('_');
        if (k == std::string::npos) return n;
        return "_" + c.substr(k) + n;
    }

    std::string t() { return std::to_string(++tmp_); }
    void refuse(const Node* n, const std::string& what) { res_.refused.push_back({n ? n->pos.lineno : 0, what}); }
    std::string src_line(const Node* n) {
        if (!n || n->pos.lineno <= 0 || (size_t)n->pos.lineno > starts_.size()) return "";
        size_t a = starts_[(size_t)n->pos.lineno - 1];
        size_t b = src_.find('\n', a);
        std::string l = src_.substr(a, b == std::string::npos ? std::string::npos : b - a);
        l = comment_text(l);
        size_t k = l.find_first_not_of(' ');
        return k == std::string::npos ? "" : l.substr(k);
    }

    // ---------------------------------------------------------------- analysis
    void analyze() {
        // bindings at module scope (not inside functions or classes), and globals written in functions
        std::function<void(const std::vector<Stmt*>&)> walk_block;
        auto bind_target = [&](const Expr* e, auto& self) -> void {
            if (!e) return;
            if (auto* n = as<Name>(e)) module_binds_[n->id]++;
            else if (auto* tu = as<Tuple>(e)) for (auto* x : tu->elts) self(x, self);
            else if (auto* li = as<List>(e)) for (auto* x : li->elts) self(x, self);
            else if (auto* st = as<Starred>(e)) self(st->value, self);
        };
        walk_block = [&](const std::vector<Stmt*>& body) {
            for (const Stmt* s : body) {
                switch (s->kind) {
                case StmtKind::FunctionDef: case StmtKind::AsyncFunctionDef: {
                    auto* f = cast<FunctionDef>(s);
                    module_binds_[f->name]++;
                    if (f->decorator_list.empty() && s->kind == StmtKind::FunctionDef && &body == &tree_.body) module_defs_[f->name] = f;
                    break;
                }
                case StmtKind::ClassDef: module_binds_[cast<ClassDef>(s)->name]++; break;
                case StmtKind::Assign: for (auto* x : cast<Assign>(s)->targets) bind_target(x, bind_target); break;
                case StmtKind::AugAssign: bind_target(cast<AugAssign>(s)->target, bind_target); break;
                case StmtKind::AnnAssign: if (cast<AnnAssign>(s)->value) bind_target(cast<AnnAssign>(s)->target, bind_target); break;
                case StmtKind::For: case StmtKind::AsyncFor: bind_target(cast<For>(s)->target, bind_target); break;
                case StmtKind::With: case StmtKind::AsyncWith:
                    for (auto* it : cast<With>(s)->items) bind_target(it->optional_vars, bind_target);
                    break;
                case StmtKind::Import: for (auto* a : cast<Import>(s)->names) module_binds_[a->has_asname ? a->asname : a->name.substr(0, a->name.find('.'))]++; break;
                case StmtKind::ImportFrom: for (auto* a : cast<ImportFrom>(s)->names) module_binds_[a->has_asname ? a->asname : a->name]++; break;
                case StmtKind::Delete: for (auto* x : cast<Delete>(s)->targets) bind_target(x, bind_target); break;
                case StmtKind::Try: case StmtKind::TryStar:
                    for (auto* h : cast<Try>(s)->handlers) if (h->has_name) module_binds_[h->name]++;
                    break;
                case StmtKind::Match:
                    for (auto* c : cast<Match>(s)->cases) {
                        std::vector<std::string> names;
                        pattern_names(c->pattern, names);
                        for (auto& n : names) module_binds_[n]++;
                    }
                    break;
                default: break;
                }
                if (s->kind != StmtKind::FunctionDef && s->kind != StmtKind::AsyncFunctionDef && s->kind != StmtKind::ClassDef)
                    for (auto* l : stmt_lists(s)) walk_block(*l);
            }
        };
        walk_block(tree_.body);
        // walrus at module level, and `global x` written in functions
        for (auto& sp : sc_.all()) {
            const Scope* s = sp.get();
            if (s->kind == Scope::Module) continue;
            for (auto& g : s->globals) module_binds_[g] += 2;
        }
        for (auto& [n, f] : module_defs_) {
            bool gen = false;
            for (const AnyNode& x : walk(AnyNode(static_cast<const Stmt*>(f)))) {
                if (auto* e = x.expr(); e && (e->kind == ExprKind::Yield || e->kind == ExprKind::YieldFrom)) {
                    // (a yield of a nested function is that function's)
                    gen = true;
                    break;
                }
            }
            if (gen) gen = is_generator(f);
            // (a decorated def binds what the decorator gave back, not the function: called through its name)
            if (module_binds_[n] == 1 && !gen && !f->args->vararg && !f->args->kwarg && f->args->kwonlyargs.empty() &&
                f->decorator_list.empty())
                def_only_.insert(n);
        }
        // cells: what a nested function, lambda or generator expression shares with its definer
        for (auto& sp : sc_.all()) {
            const Scope* s = sp.get();
            bool closure = s->kind == Scope::Function || s->kind == Scope::Lambda || (s->kind == Scope::Comprehension && s->name == "<genexpr>");
            if (!closure) continue;
            for (auto& f : s->free) {
                NameRes r = sc_.resolve_in(s, f);
                if (r.kind == NameRes::Free && r.owner) cells_.insert({r.owner, f});
            }
        }
    }
    // the names a match pattern binds
    static void pattern_names(const Pattern* p, std::vector<std::string>& out) {
        if (!p) return;
        switch (p->kind) {
        case PatternKind::MatchSequence: for (auto* x : cast<MatchSequence>(p)->patterns) pattern_names(x, out); break;
        case PatternKind::MatchMapping: {
            auto* m = cast<MatchMapping>(p);
            for (auto* x : m->patterns) pattern_names(x, out);
            if (m->has_rest) out.push_back(m->rest);
            break;
        }
        case PatternKind::MatchClass: {
            auto* m = cast<MatchClass>(p);
            for (auto* x : m->patterns) pattern_names(x, out);
            for (auto* x : m->kwd_patterns) pattern_names(x, out);
            break;
        }
        case PatternKind::MatchStar: if (cast<MatchStar>(p)->has_name) out.push_back(cast<MatchStar>(p)->name); break;
        case PatternKind::MatchAs: {
            auto* m = cast<MatchAs>(p);
            pattern_names(m->pattern, out);
            if (m->has_name) out.push_back(m->name);
            break;
        }
        case PatternKind::MatchOr: for (auto* x : cast<MatchOr>(p)->patterns) pattern_names(x, out); break;
        default: break;
        }
    }
    bool is_generator(const FunctionDef* f) {
        std::function<bool(const std::vector<Stmt*>&)> in_body;
        std::function<bool(const Expr*)> in_expr = [&](const Expr* e) -> bool {
            if (!e) return false;
            if (e->kind == ExprKind::Yield || e->kind == ExprKind::YieldFrom) return true;
            if (e->kind == ExprKind::Lambda || e->kind == ExprKind::GeneratorExp || e->kind == ExprKind::ListComp ||
                e->kind == ExprKind::SetComp || e->kind == ExprKind::DictComp)
                return false;
            std::vector<AnyNode> kids;
            child_nodes(AnyNode(e), kids);
            for (auto& k : kids)
                if (k.expr() && in_expr(k.expr())) return true;
                else if (k.cat == AnyNode::Keyword && in_expr(static_cast<const keyword*>(k.p)->value)) return true;
            return false;
        };
        std::function<bool(const Stmt*)> in_stmt = [&](const Stmt* s) -> bool {
            if (s->kind == StmtKind::FunctionDef || s->kind == StmtKind::AsyncFunctionDef || s->kind == StmtKind::ClassDef) return false;
            std::vector<AnyNode> kids;
            child_nodes(AnyNode(s), kids);
            for (auto& k : kids) {
                if (k.stmt() && in_stmt(k.stmt())) return true;
                if (k.expr() && in_expr(k.expr())) return true;
                if (k.cat == AnyNode::WithItem && in_expr(static_cast<const withitem*>(k.p)->context_expr)) return true;
                if (k.cat == AnyNode::Handler)
                    for (auto* b : static_cast<const ExceptHandler*>(k.p)->body) if (in_stmt(b)) return true;
                if (k.cat == AnyNode::MatchCase) {
                    auto* mc = static_cast<const match_case*>(k.p);
                    if (in_expr(mc->guard)) return true;
                    for (auto* b : mc->body) if (in_stmt(b)) return true;
                }
            }
            return false;
        };
        for (auto* s : f->body) if (in_stmt(s)) return true;
        return false;
    }
    bool is_cell(const Scope* s, const std::string& n) const { return cells_.count({s, n}) != 0; }

    // the C++ name a module-level name is held in
    std::string gref(const std::string& n) {
        std::string c = module_defs_.count(n) ? ident(n) + "_fn" : ident(n);
        if (!global_names_.count(n)) {
            global_names_.insert(n);
            globals_ += "static py::var& " + c + " = py_module_.global(" + cstr(n) + ");\n";
        }
        return c;
    }

    // ---------------------------------------------------------------- expressions
    std::string name_load(const Name* n) {
        const Scope* where = sc_.enclosing(n);
        NameRes r = sc_.resolve(n);
        const std::string& id = n->id;
        // the module's own attributes the loader sets
        if (r.kind == NameRes::Builtin && (id == "__name__" || id == "__file__" || id == "__doc__" || id == "__package__" ||
                                           id == "__path__" || id == "__spec__" || id == "__loader__"))
            return gref(id);
        if (where && where->kind == Scope::Class && r.kind != NameRes::Free) {
            // a class body reads its namespace first
            std::string fb = r.kind == NameRes::Global || (r.kind == NameRes::Local && where->binds(id)) ? global_or_builtin(id) : global_or_builtin(id);
            if (where->binds(id)) return "py::class_name(" + fn_->class_var + ", " + slit(mangle(id)) + ", " + fb + ")";
            return fb;
        }
        switch (r.kind) {
        case NameRes::Local:
        case NameRes::Free: return local_name(id);
        case NameRes::Global:
            // a module name that is also a builtin (capstone's `range = xrange` under Python 2 only): the builtin
            // while the module has not bound it, as Python's lookup falls through
            if (Scopes::is_builtin(id)) return "py::or_builtin(" + gref(id) + ", " + cstr(id) + ")";
            return gref(id);
        case NameRes::Builtin: return builtin_value(id);
        default:
            // a name the module never binds: maybe made at run time (from x import *, globals())
            if (extra_builtin(id)) return "py::or_builtin(" + gref(id) + ", " + cstr(id) + ")";
            return gref(id);
        }
    }
    // builtins the scope analysis does not list (it is shared with the typed path, whose output must not change):
    // read as a module global that falls back to the builtin, as Python's lookup does
    static bool extra_builtin(const std::string& id) {
        static const std::set<std::string> more = {
            "ConnectionResetError", "ConnectionRefusedError", "ConnectionAbortedError", "BaseExceptionGroup", "ExceptionGroup",
            "FutureWarning", "PendingDeprecationWarning", "ResourceWarning", "SyntaxWarning", "ImportWarning", "EncodingWarning",
            "UnicodeWarning", "BytesWarning", "ReferenceError", "SystemError", "FloatingPointError", "WindowsError", "aiter", "anext"};
        return more.count(id) != 0;
    }
    std::string global_or_builtin(const std::string& id) {
        if (extra_builtin(id)) return "py::or_builtin(" + gref(id) + ", " + cstr(id) + ")";
        if (sc_.module()->binds(id) || !Scopes::is_builtin(id)) return gref(id);
        return builtin_value(id);
    }
    std::string local_name(const std::string& id) {
        if (id == "__class__" && fn_ && !fn_->class_cell.empty()) return fn_->class_cell;
        return ident(id);
    }
    std::string builtin_value(const std::string& id) {
        static const std::map<std::string, std::string> types = {
            {"str", "str"}, {"int", "int_"}, {"float", "float_"}, {"bool", "bool_"}, {"list", "list"}, {"dict", "dict"}, {"tuple", "tuple"},
            {"set", "set"}, {"frozenset", "frozenset"}, {"bytes", "bytes"}, {"bytearray", "bytearray"}, {"object", "object"}, {"type", "type"},
            {"range", "range"}, {"slice", "slice"}, {"property", "property"}, {"staticmethod", "staticmethod"}, {"classmethod", "classmethod"},
            {"complex", "complex"}};
        auto it = types.find(id);
        if (it != types.end()) return "py::t::" + it->second;
        static const std::set<std::string> excs = {
            "BaseException", "Exception", "SystemExit", "KeyboardInterrupt", "GeneratorExit", "StopIteration", "ArithmeticError",
            "ZeroDivisionError", "OverflowError", "AssertionError", "AttributeError", "EOFError", "ImportError", "ModuleNotFoundError",
            "LookupError", "IndexError", "KeyError", "MemoryError", "NameError", "UnboundLocalError", "OSError", "FileNotFoundError",
            "FileExistsError", "PermissionError", "IsADirectoryError", "NotADirectoryError", "TimeoutError", "RuntimeError",
            "NotImplementedError", "RecursionError", "SyntaxError", "TypeError", "ValueError", "UnicodeError", "UnicodeDecodeError",
            "UnicodeEncodeError", "Warning", "UserWarning", "DeprecationWarning", "RuntimeWarning", "ConnectionError",
            "BrokenPipeError", "InterruptedError", "ChildProcessError", "ProcessLookupError", "BlockingIOError", "BufferError",
            "StopAsyncIteration"};
        if (excs.count(id)) return "py::t::" + id;
        if (id == "IOError" || id == "EnvironmentError") return "py::t::OSError";
        if (id == "NotImplemented") return "py::NotImplemented";
        if (id == "Ellipsis") return "py::Ellipsis";
        return "py::builtin(" + cstr(id) + ")";
    }

    std::string constant(const Constant* c) {
        const ConstValue& v = c->value;
        switch (v.kind) {
        case ConstValue::Kind::None: return "py::None";
        case ConstValue::Kind::True: return "py::var(true)";
        case ConstValue::Kind::False: return "py::var(false)";
        case ConstValue::Kind::Ellipsis: return "py::Ellipsis";
        case ConstValue::Kind::Int:
            if (v.fits_i64) {
                if (v.i64 > 2147483647LL || v.i64 < -2147483647LL) return "py::var(" + std::to_string(v.i64) + "LL)";
                return "py::var(" + std::to_string(v.i64) + ")";
            }
            return "py::int_lit_(" + cstr(v.text) + ")";
        case ConstValue::Kind::Float: {
            char b[64];
            snprintf(b, sizeof b, "%.17g", v.f);
            std::string s = b;
            if (std::isinf(v.f)) return "py::var(HUGE_VAL)";
            if (s.find_first_of(".en") == std::string::npos) s += ".0";
            return "py::var(" + s + ")";
        }
        case ConstValue::Kind::Complex: refuse(c, "complex literal"); return "py::None";
        case ConstValue::Kind::Str: return slit(v.text);
        case ConstValue::Kind::Bytes: return cstr(v.text, true) + "_b";
        }
        return "py::None";
    }
    // a constant as a bare C++ value where a var converts (x + 1)
    std::string operand(const Expr* e) {
        if (auto* c = as<Constant>(e)) {
            if (c->value.kind == ConstValue::Kind::Int && c->value.fits_i64 && c->value.i64 <= 2147483647LL && c->value.i64 >= -2147483647LL)
                return std::to_string(c->value.i64);
        }
        return expr(e);
    }
    static bool simple(const Expr* e) {
        return e->kind == ExprKind::Name || e->kind == ExprKind::Constant ||
               (e->kind == ExprKind::Attribute && cast<Attribute>(e)->value->kind == ExprKind::Name);
    }

    std::string binop_s(OpKind op, const std::string& a, const std::string& b) {
        switch (op) {
        case OpKind::Add: return "(" + a + " + " + b + ")";
        case OpKind::Sub: return "(" + a + " - " + b + ")";
        case OpKind::Mult: return "(" + a + " * " + b + ")";
        case OpKind::Div: return "(" + a + " / " + b + ")";
        case OpKind::Mod: return "(" + a + " % " + b + ")";
        case OpKind::LShift: return "(" + a + " << " + b + ")";
        case OpKind::RShift: return "(" + a + " >> " + b + ")";
        case OpKind::BitOr: return "(" + a + " | " + b + ")";
        case OpKind::BitXor: return "(" + a + " ^ " + b + ")";
        case OpKind::BitAnd: return "(" + a + " & " + b + ")";
        case OpKind::FloorDiv: return "py::floordiv(" + a + ", " + b + ")";
        case OpKind::Pow: return "py::pow(" + a + ", " + b + ")";
        case OpKind::MatMult: return "py::matmul(" + a + ", " + b + ")";
        }
        return a;
    }
    // an expression whose evaluation may do something (a call, a yield, an assignment expression): where two
    // such meet as a C++ call's arguments or an operator's operands, their order is set explicitly
    static bool effects(const Expr* e) {
        if (!e) return false;
        switch (e->kind) {
        case ExprKind::Call: case ExprKind::Yield: case ExprKind::YieldFrom: case ExprKind::Await: case ExprKind::NamedExpr: return true;
        case ExprKind::Lambda: case ExprKind::Constant: case ExprKind::Name: return false;
        default: break;
        }
        std::vector<AnyNode> kids;
        child_nodes(AnyNode(e), kids);
        for (auto& k : kids) {
            if (k.expr() && effects(k.expr())) return true;
            if (k.cat == AnyNode::Keyword && effects(static_cast<const keyword*>(k.p)->value)) return true;
            if (k.cat == AnyNode::Comprehension) {
                auto* g = static_cast<const pyfront::comprehension*>(k.p);
                if (effects(g->iter) || effects(g->target)) return true;
                for (auto* f : g->ifs) if (effects(f)) return true;
            }
        }
        return false;
    }
    bool ordered_args(const Call* c) {
        int n = 0;
        for (auto* a : c->args) n += effects(a);
        for (auto* k : c->keywords) n += effects(k->value);
        return n >= 2;
    }
    std::string binop(const BinOp* b) {
        std::string l = expr(b->left), r = operand(b->right);
        if (effects(b->left) && effects(b->right))
            return "[&]() -> py::var { py::var py_l = " + l + "; return " + binop_s(b->op, "py_l", r) + "; }()";
        return binop_s(b->op, l, r);
    }
    std::string compare_one(CmpOpKind op, const std::string& a_in, const std::string& b, const Expr* rexpr) {
        std::string a = a_in == "py::None" ? "py::None_" : a_in;   // (None is x: the var, which has is_none)
        switch (op) {
        case CmpOpKind::Eq: return "(" + a + " == " + b + ")";
        case CmpOpKind::NotEq: return "(" + a + " != " + b + ")";
        case CmpOpKind::Lt: return "(" + a + " < " + b + ")";
        case CmpOpKind::LtE: return "(" + a + " <= " + b + ")";
        case CmpOpKind::Gt: return "(" + a + " > " + b + ")";
        case CmpOpKind::GtE: return "(" + a + " >= " + b + ")";
        case CmpOpKind::Is:
            if (rexpr && is_none_const(rexpr)) return a + ".is_none()";
            return "py::is(" + a + ", " + b + ")";
        case CmpOpKind::IsNot:
            if (rexpr && is_none_const(rexpr)) return "!" + a + ".is_none()";
            return "!py::is(" + a + ", " + b + ")";
        case CmpOpKind::In: return "py::in(" + a + ", " + b + ")";
        case CmpOpKind::NotIn: return "!py::in(" + a + ", " + b + ")";
        }
        return a;
    }
    // a comparison as a C++ bool expression
    std::string compare(const Compare* c) {
        if (c->ops.size() == 1) {
            const Expr* r = c->comparators[0];
            std::string a = expr(c->left);
            std::string b = (c->ops[0] == CmpOpKind::Is || c->ops[0] == CmpOpKind::IsNot) && is_none_const(r) ? "" : operand(r);
            if (effects(c->left) && effects(r))
                return "[&]() -> bool { py::var py_l = " + a + "; return " + compare_one(c->ops[0], "py_l", b, r) + "; }()";
            return compare_one(c->ops[0], a, b, r);
        }
        // a < b < c: each middle operand evaluated once
        std::string o = "[&]() -> bool { py::var py_l = " + expr(c->left) + "; ";
        for (size_t i = 0; i < c->ops.size(); i++) {
            o += "py::var py_r" + std::to_string(i) + " = " + expr(c->comparators[i]) + "; ";
            o += "if (!" + compare_one(c->ops[i], i == 0 ? "py_l" : "py_r" + std::to_string(i - 1), "py_r" + std::to_string(i), c->comparators[i]) + ") return false; ";
        }
        return o + "return true; }()";
    }
    // a comparison as a value: what the rich comparison returned (an __eq__ / __lt__ may return anything --
    // z3's expressions), a < b < c the first false one's or the last's; is / in are bools
    static int rich_op(CmpOpKind op) {
        switch (op) {
        case CmpOpKind::Lt: return 0;
        case CmpOpKind::LtE: return 1;
        case CmpOpKind::Eq: return 2;
        case CmpOpKind::NotEq: return 3;
        case CmpOpKind::Gt: return 4;
        case CmpOpKind::GtE: return 5;
        default: return -1;
        }
    }
    std::string compare_value(const Compare* c) {
        bool rich = false;
        for (auto op : c->ops) rich = rich || rich_op(op) >= 0;
        if (!rich) return "py::var(" + compare(c) + ")";
        auto one = [&](size_t i, const std::string& a, const std::string& b) {
            int op = rich_op(c->ops[i]);
            if (op < 0) return "py::var(" + compare_one(c->ops[i], a, b, c->comparators[i]) + ")";
            return "py::compare_value_(" + a + ", " + b + ", " + std::to_string(op) + ")";
        };
        if (c->ops.size() == 1) {
            const Expr* r = c->comparators[0];
            std::string a = expr(c->left), b = operand(r);
            if (effects(c->left) && effects(r))
                return "[&]() -> py::var { py::var py_l = " + a + "; return " + one(0, "py_l", b) + "; }()";
            return one(0, a, b);
        }
        std::string o = "[&]() -> py::var { py::var py_l = " + expr(c->left) + ", py_v; ";
        for (size_t i = 0; i < c->ops.size(); i++) {
            o += "py::var py_r" + std::to_string(i) + " = " + expr(c->comparators[i]) + "; ";
            o += "py_v = " + one(i, i == 0 ? "py_l" : "py_r" + std::to_string(i - 1), "py_r" + std::to_string(i)) + "; ";
            o += "if (!py::truthy_(py_v)) return py_v; ";
        }
        return o + "return py_v; }()";
    }
    // a condition: C++ bool
    std::string cond(const Expr* e) {
        if (auto* c = as<Compare>(e)) return compare(c);
        if (auto* u = as<UnaryOp>(e); u && u->op == UnaryOpKind::Not) return "!" + paren(cond(u->operand));
        if (auto* b = as<BoolOp>(e)) {
            std::string o;
            for (size_t i = 0; i < b->values.size(); i++) {
                if (i) o += b->op == BoolOpKind::And ? " && " : " || ";
                o += paren(cond(b->values[i]));
            }
            return o;
        }
        if (auto* c = as<Constant>(e)) {
            if (c->value.kind == ConstValue::Kind::True) return "true";
            if (c->value.kind == ConstValue::Kind::False) return "false";
        }
        return "py::truthy_(" + expr(e) + ")";
    }
    // s in parentheses unless it already is one parenthesised whole (or has no space: one term)
    static std::string paren(const std::string& s) {
        if (s.empty() || s.find(' ') == std::string::npos) return s;
        if (s.front() == '(' && s.back() == ')') {
            int depth = 0;
            bool in_str = false;
            for (size_t i = 0; i < s.size(); i++) {
                char c = s[i];
                if (in_str) {
                    if (c == '\\') i++;
                    else if (c == '"') in_str = false;
                    continue;
                }
                if (c == '"') in_str = true;
                else if (c == '(') depth++;
                else if (c == ')' && --depth == 0 && i + 1 < s.size()) return "(" + s + ")";
            }
            return s;
        }
        return "(" + s + ")";
    }

    std::string call_args(const Call* c, size_t skip = 0) {
        std::string o;
        for (size_t i = skip; i < c->args.size(); i++) {
            const Expr* a = c->args[i];
            if (!o.empty()) o += ", ";
            if (auto* st = as<Starred>(a)) o += "py::star(" + expr(st->value) + ")";
            else o += expr(a);
        }
        for (auto* k : c->keywords) {
            if (!o.empty()) o += ", ";
            if (!k->has_arg) o += "py::starstar(" + expr(k->value) + ")";
            else o += "py::kw(" + cstr(k->arg) + ", " + expr(k->value) + ")";
        }
        return o;
    }
    bool plain_args(const Call* c) {
        if (!c->keywords.empty()) return false;
        for (auto* a : c->args) if (a->kind == ExprKind::Starred) return false;
        return true;
    }
    bool name_is_builtin(const Expr* f, const char* name) {
        auto* n = as<Name>(f);
        if (!n || n->id != name) return false;
        return sc_.resolve(n).kind == NameRes::Builtin;
    }

    std::string call(const Call* c) {
        // super(): the class the method is in, and its first argument
        if (name_is_builtin(c->func, "super") && c->args.empty() && c->keywords.empty()) {
            if (!fn_ || fn_->class_cell.empty() || fn_->self_name.empty()) {
                refuse(c, "super() outside a method");
                return "py::None";
            }
            return "py::super_(" + fn_->class_cell + ", " + ident(fn_->self_name) + ")";
        }
        if (auto* n = as<Name>(c->func)) {
            NameRes r = sc_.resolve(n);
            const Scope* where = sc_.enclosing(n);
            bool in_class = where && where->kind == Scope::Class && where->binds(n->id);
            if (r.kind == NameRes::Builtin && !in_class && !ordered_args(c)) {
                std::string b = builtin_call(n->id, c);
                if (!b.empty()) return b;
            }
            if (r.kind == NameRes::Global && def_only_.count(n->id) && plain_args(c) && !ordered_args(c) && !in_class && !getenv("PYENGINE_NO_DIRECT")) {
                const FunctionDef* f = module_defs_[n->id];
                size_t np = f->args->posonlyargs.size() + f->args->args.size();
                if (c->args.size() == np) {
                    // directly while the module's name still holds the function its def made; else through the
                    // name, as Python calls it (monkeypatch.setattr(module, "f", ...) from outside is seen)
                    std::string o = adl_safe(ident(n->id)) + "(";
                    for (size_t i = 0; i < c->args.size(); i++) o += (i ? ", " : "") + expr(c->args[i]);
                    std::string cellv = gref(n->id);
                    def_mark(n->id);
                    return "(" + cellv + ".ref() == " + ident(n->id) + "_def_ ? " + o + ") : " + cellv + "(" + call_args(c) + "))";
                }
            }
        }
        if (auto* a = as<Attribute>(c->func)) {
            std::string obj = expr(a->value);
            // ''.join(x for x in ...), xs.extend(...), s.update(...): the generator consumed at once, as a list
            static const std::set<std::string> eager = {"join", "extend", "update", "union", "intersection", "difference", "issubset", "issuperset"};
            if (eager.count(a->attr) && c->args.size() == 1 && c->keywords.empty() && c->args[0]->kind == ExprKind::GeneratorExp) {
                auto* g = cast<GeneratorExp>(c->args[0]);
                return obj + ".call(" + cstr(mangle(a->attr)) + ", " + comprehension_list(g->elt, g->generators, c->args[0]) + ")";
            }
            std::string args = call_args(c);
            if (ordered_args(c)) return obj + ".call(" + cstr(mangle(a->attr)) + ", {" + args + "})";
            return obj + ".call(" + cstr(mangle(a->attr)) + (args.empty() ? "" : ", " + args) + ")";
        }
        std::string f = expr(c->func);
        if (ordered_args(c)) return paren(f) + "({" + call_args(c) + "})";
        return paren(f) + "(" + call_args(c) + ")";
    }
    std::string builtin_call(const std::string& id, const Call* c) {
        if (!plain_args(c)) {
            if (id == "print") return "py::print(" + call_args(c) + ")";
            return "";
        }
        size_t n = c->args.size();
        auto a = [&](size_t i) { return expr(c->args[i]); };
        static const std::set<std::string> one = {"len", "repr", "hash", "id", "abs", "chr", "ord", "hex", "oct", "bin", "callable", "iter", "ascii"};
        if (one.count(id) && n == 1) return "py::" + id + "(" + a(0) + ")";
        if (id == "str" && n == 1) return "py::str(" + a(0) + ")";
        if (id == "type" && n == 1) return "py::type(" + a(0) + ")";
        if (id == "isinstance" && n == 2) return "py::isinstance(" + a(0) + ", " + a(1) + ")";
        if (id == "issubclass" && n == 2) return "py::issubclass(" + a(0) + ", " + a(1) + ")";
        if (id == "getattr" && (n == 2 || n == 3)) return "py::getattr(" + a(0) + ", " + a(1) + (n == 3 ? ", " + a(2) : "") + ")";
        if (id == "hasattr" && n == 2) return "py::hasattr(" + a(0) + ", " + a(1) + ")";
        if (id == "setattr" && n == 3) return "(py::setattr(" + a(0) + ", " + a(1) + ", " + a(2) + "), py::None_)";
        if (id == "next" && (n == 1 || n == 2)) return "py::next(" + a(0) + (n == 2 ? ", " + a(1) : "") + ")";
        if (id == "divmod" && n == 2) return "py::divmod(" + a(0) + ", " + a(1) + ")";
        if (id == "round" && (n == 1 || n == 2)) return "py::round(" + a(0) + (n == 2 ? ", " + a(1) : "") + ")";
        if (id == "format" && (n == 1 || n == 2)) return "py::format(" + a(0) + (n == 2 ? ", " + a(1) : "") + ")";
        if (id == "range" && n >= 1 && n <= 3) {
            if (n == 1) return "py::range(" + a(0) + ")";
            return "py::range(" + a(0) + ", " + a(1) + (n == 3 ? ", " + a(2) : "") + ")";
        }
        if (id == "print") return "py::print(" + call_args(c) + ")";
        if (id == "globals" && n == 0) return "py::globals_of(py_module_)";
        if (id == "locals" && n == 0) {
            // locals(): the module's dict at module level (and in a comprehension there), the namespace in a
            // class body, else a dict of the function's bound names at the call (as CPython's snapshot)
            const Scope* s = fn_ ? fn_->scope : nullptr;
            while (s && s->kind == Scope::Comprehension && s->parent) s = s->parent;
            if (!s || s->kind == Scope::Module) return "py::globals_of(py_module_)";
            if (s->kind == Scope::Class && fn_ && !fn_->class_var.empty()) return fn_->class_var;
            std::set<std::string> names(s->params.begin(), s->params.end());
            for (auto& b : s->bound) if (s->binds(b)) names.insert(b);
            for (auto& f : s->free) if (f != "__class__") names.insert(f);
            std::string o = "[&]() -> py::var { py::var py_d = py::dict(); ";
            for (auto& nm : names) o += "if (!" + ident(nm) + ".is_unbound()) py_d.setitem(" + cstr(nm) + "_s, " + ident(nm) + "); ";
            return o + "return py_d; }()";
        }
        if ((id == "any" || id == "all") && n == 1) {
            if (auto* g = as<GeneratorExp>(c->args[0])) return any_all(g, id == "any");
        }
        // list(...), sorted(...), sum(...) of a generator expression: consumed at once
        static const std::set<std::string> eager = {"list", "tuple", "set", "frozenset", "sorted", "sum", "min", "max", "dict"};
        if (eager.count(id) && n >= 1 && as<GeneratorExp>(c->args[0])) {
            std::string o = builtin_value(id) + "(" + comprehension_list(as<GeneratorExp>(c->args[0])->elt, as<GeneratorExp>(c->args[0])->generators, c->args[0]);
            for (size_t i = 1; i < n; i++) o += ", " + a(i);
            return o + ")";
        }
        return "";
    }

    std::string attribute(const Attribute* a) { return expr(a->value) + ".attr(" + cstr(mangle(a->attr)) + ")"; }

    std::string subscript_key(const Expr* s) {
        if (auto* sl = as<Slice>(s))
            return "py::slice_obj(" + (sl->lower ? expr(sl->lower) : "py::None") + ", " + (sl->upper ? expr(sl->upper) : "py::None") + ", " +
                   (sl->step ? expr(sl->step) : "py::None") + ")";
        if (auto* tu = as<Tuple>(s)) {
            bool has_slice = false;
            for (auto* x : tu->elts) if (x->kind == ExprKind::Slice) has_slice = true;
            if (has_slice) {
                std::string o = "py::tuple({";
                for (size_t i = 0; i < tu->elts.size(); i++) o += (i ? ", " : "") + subscript_key(tu->elts[i]);
                return o + "})";
            }
        }
        return operand(s);
    }
    std::string subscript(const Subscript* s) {
        if (auto* sl = as<Slice>(s->slice)) {
            return expr(s->value) + ".slice(" + (sl->lower ? expr(sl->lower) : "py::None") + ", " + (sl->upper ? expr(sl->upper) : "py::None") +
                   (sl->step ? ", " + expr(sl->step) : "") + ")";
        }
        return expr(s->value) + "[" + subscript_key(s->slice) + "]";
    }

    std::string display(const std::vector<Expr*>& elts, const char* fn, const char* disp) {
        bool star = false;
        for (auto* e : elts) if (e->kind == ExprKind::Starred) star = true;
        std::string o;
        if (!star) {
            o = std::string(fn) + "({";
            for (size_t i = 0; i < elts.size(); i++) o += (i ? ", " : "") + expr(elts[i]);
            return o + "})";
        }
        std::string items, idx;
        for (size_t i = 0; i < elts.size(); i++) {
            const Expr* e = elts[i];
            items += (i ? ", " : "");
            if (auto* st = as<Starred>(e)) {
                items += expr(st->value);
                idx += (idx.empty() ? "" : ", ") + std::to_string(i);
            } else items += expr(e);
        }
        return std::string(disp) + "({" + items + "}, {" + idx + "})";
    }
    std::string dict_display(const Dict* d) {
        bool spread = false;
        for (auto* k : d->keys) if (!k) spread = true;
        if (!spread) {
            if (d->keys.empty()) return "py::dict()";
            std::string o = "py::dict({";
            for (size_t i = 0; i < d->keys.size(); i++) o += (i ? ", " : "") + std::string("{") + expr(d->keys[i]) + ", " + expr(d->values[i]) + "}";
            return o + "})";
        }
        std::string o = "[&]() -> py::var { py::var py_d = py::dict(); ";
        for (size_t i = 0; i < d->keys.size(); i++) {
            if (!d->keys[i]) o += "py::dict_update_(py_d, " + expr(d->values[i]) + "); ";
            else o += "py_d.setitem(" + expr(d->keys[i]) + ", " + expr(d->values[i]) + "); ";
        }
        return o + "return py_d; }()";
    }

    std::string fstring(const JoinedStr* j) {
        std::string o = "py::fstr({";
        bool first = true;
        for (auto* v : j->values) {
            if (!first) o += ", ";
            first = false;
            if (auto* c = as<Constant>(v)) o += slit(c->value.text);
            else if (auto* f = as<FormattedValue>(v)) {
                std::string conv = f->conversion < 0 ? "0" : std::string("'") + (char)f->conversion + "'";
                std::string spec = f->format_spec ? expr(f->format_spec) : "py::None";
                o += "py::fmt(" + expr(f->value) + ", " + conv + ", " + spec + ")";
            }
        }
        return o + "})";
    }

    // the comprehension's loops and filters around `inner` (a statement), inside an immediately-run lambda
    std::string comp_loops(const std::vector<pyfront::comprehension*>& gens, const std::string& inner, const std::string& first_iter) {
        std::string o;
        int depth = 0;
        for (size_t i = 0; i < gens.size(); i++) {
            const pyfront::comprehension* g = gens[i];
            std::string it = i == 0 ? first_iter : expr(g->iter);
            std::string v = "py_it" + t();
            if (it.find("[&]") != std::string::npos) {   // (MSVC: a lambda in a range-for's range inside a lambda miscompiles)
                o += "{ py::var " + v + "r = " + it + "; ";
                it = v + "r";
                depth++;
            }
            if (g->is_async) {   // [x async for x in xs]: __anext__() awaited (inside an async def)
                if (!fn_ || !fn_->async_) refuse(g->target, "async comprehension outside an async function");
                o += "{ py::var " + v + "a = py::aiter_(" + it + "); for (;;) { py::var " + v + "; if (!y.async_next(" + v + "a, " + v + ")) break; " +
                     assign_target(g->target, v) + " ";
                depth += 2;
            } else {
                o += "for (py::var " + v + " : py::each(" + it + ")) { " + assign_target(g->target, v) + " ";
                depth++;
            }
            for (auto* f : g->ifs) {
                o += "if (" + cond(f) + ") { ";
                depth++;
            }
        }
        o += inner;
        for (int i = 0; i < depth; i++) o += " }";
        return o;
    }
    std::string comp_locals(const AnyNode& node) {
        const Scope* s = sc_.scope_of(node.p);
        std::string o;
        if (!s) return o;
        for (auto& n : s->bound) {
            if (is_cell(s, n)) o += "py::cell " + ident(n) + "_cell; py::var& " + ident(n) + " = *" + ident(n) + "_cell; ";
            else o += "py::var " + ident(n) + "; ";
        }
        return o;
    }
    std::string comprehension_list(const Expr* elt, const std::vector<pyfront::comprehension*>& gens, const Expr* node) {
        std::string first = expr(gens[0]->iter);
        std::string o = "[&](py::var py_src) -> py::var { " + comp_locals(AnyNode(node)) + "py::var py_out = py::list(); ";
        o += comp_loops(gens, "py_out.call(\"append\", " + expr(elt) + ");", "py_src");
        return o + " return py_out; }(" + first + ")";
    }
    std::string any_all(const GeneratorExp* g, bool any) {
        std::string first = expr(g->generators[0]->iter);
        std::string o = "[&](py::var py_src) -> py::var { " + comp_locals(AnyNode(static_cast<const Expr*>(g)));
        std::string test = any ? "if (py::truthy_(" + expr(g->elt) + ")) return py::var(true);" : "if (!py::truthy_(" + expr(g->elt) + ")) return py::var(false);";
        o += comp_loops(g->generators, test, "py_src");
        return o + std::string(" return py::var(") + (any ? "false" : "true") + "); }(" + first + ")";
    }
    std::string comprehension(const Expr* e) {
        if (auto* l = as<ListComp>(e)) return comprehension_list(l->elt, l->generators, e);
        if (auto* s = as<SetComp>(e)) {
            std::string first = expr(s->generators[0]->iter);
            std::string o = "[&](py::var py_src) -> py::var { " + comp_locals(AnyNode(e)) + "py::var py_out = py::set(); ";
            o += comp_loops(s->generators, "py_out.call(\"add\", " + expr(s->elt) + ");", "py_src");
            return o + " return py_out; }(" + first + ")";
        }
        if (auto* d = as<DictComp>(e)) {
            std::string first = expr(d->generators[0]->iter);
            std::string o = "[&](py::var py_src) -> py::var { " + comp_locals(AnyNode(e)) + "py::var py_out = py::dict(); ";
            o += comp_loops(d->generators, "{ py::var py_k = " + expr(d->key) + "; py_out.setitem(py_k, " + expr(d->value) + "); }", "py_src");
            return o + " return py_out; }(" + first + ")";
        }
        // a generator expression: a generator over the first iterable, evaluated now
        auto* g = cast<GeneratorExp>(e);
        const Scope* s = sc_.scope_of(e);
        std::string caps = captures(s);
        std::string first = expr(g->generators[0]->iter);
        Fn* outer = fn_;
        Fn inner;
        inner.scope = s;
        inner.generator = true;
        inner.file_scope = false;
        if (outer) { inner.class_cell = outer->class_cell; inner.self_name = outer->self_name; }
        fn_ = &inner;
        std::string body = free_decls(s) + comp_locals(AnyNode(e));
        body += comp_loops(g->generators, "y(" + expr(g->elt) + ");", "py_src");
        fn_ = outer;
        return "py::def_generator(\"<genexpr>\", \"py_src\", {}, [" + caps + "](py::yielder& y, py::var py_src) -> py::var { " + body +
               " return py::None; })(" + first + ")";
    }

    // the captures of a closure: the cells of the names it shares
    std::string captures(const Scope* s) {
        std::string o;
        if (!s) return o;
        for (auto& f : s->free) {
            if (f == "__class__") continue;
            o += (o.empty() ? "" : ", ") + ident(f) + "_cell";
        }
        if (fn_ && !fn_->class_cell.empty() && !fn_->file_scope) {
            // (a lambda inside a method of a class made in a function: the class cell)
        }
        return o;
    }
    std::string free_decls(const Scope* s) {
        std::string o;
        if (!s) return o;
        for (auto& f : s->free) {
            if (f == "__class__") continue;
            o += "py::var& " + ident(f) + " = *" + ident(f) + "_cell; ";
        }
        return o;
    }

    std::string lambda(const Lambda* l) {
        const Scope* s = sc_.scope_of(l);
        std::string sig, params, defaults;
        const Scope* saved_scope = cur_param_scope_;   // (a lambda in a default: the outer signature goes on after it)
        cur_param_scope_ = s;
        signature(l->args, sig, params, defaults);
        cur_param_scope_ = saved_scope;
        Fn* outer = fn_;
        Fn inner;
        inner.scope = s;
        inner.file_scope = false;
        if (outer) { inner.class_cell = outer->class_cell; inner.self_name = outer->self_name; }
        fn_ = &inner;
        std::string body = free_decls(s) + param_cells(s, l->args) + "return " + expr(l->body) + ";";
        fn_ = outer;
        std::string lq = (outer && !outer->qual.empty() ? outer->qual + ".<locals>." : std::string()) + "<lambda>";
        return "py::def(\"<lambda>\", " + cstr(sig) + ", {" + defaults + "}, [" + captures(s) + "](" + params + ") -> py::var { " + body + " }, " + cstr(lq) + ", " + cstr(m_.name) + ")";
    }

    std::string expr(const Expr* e) {
        switch (e->kind) {
        case ExprKind::Name: return name_load(cast<Name>(e));
        case ExprKind::Constant: return constant(cast<Constant>(e));
        case ExprKind::Attribute: return attribute(cast<Attribute>(e));
        case ExprKind::Subscript: return subscript(cast<Subscript>(e));
        case ExprKind::Call: return call(cast<Call>(e));
        case ExprKind::BinOp: return binop(cast<BinOp>(e));
        case ExprKind::UnaryOp: {
            auto* u = cast<UnaryOp>(e);
            switch (u->op) {
            case UnaryOpKind::Not: return "py::var(!" + paren(cond(u->operand)) + ")";
            case UnaryOpKind::USub:
                if (auto* c = as<Constant>(u->operand); c && (c->value.kind == ConstValue::Kind::Int || c->value.kind == ConstValue::Kind::Float)) {
                    if (c->value.kind == ConstValue::Kind::Int && !c->value.fits_i64) return "py::int_lit_(\"-" + c->value.text + "\")";
                    std::string v = constant(c);
                    return "(-" + v + ")";
                }
                return "(-" + expr(u->operand) + ")";
            case UnaryOpKind::UAdd: return "(+" + expr(u->operand) + ")";
            case UnaryOpKind::Invert: return "(~" + expr(u->operand) + ")";
            }
            return "";
        }
        case ExprKind::BoolOp: {
            auto* b = cast<BoolOp>(e);
            std::string o = expr(b->values.back());
            for (size_t i = b->values.size() - 1; i-- > 0;) {
                const Expr* rhs = b->values[i + 1];
                std::string r = i + 2 == b->values.size() ? o : o;
                // (only a name or a constant is read without a lambda: an attribute may raise or run a property --
                // isinstance(x, T) and x.label reads label only of a T)
                bool lazy = !(rhs->kind == ExprKind::Name || rhs->kind == ExprKind::Constant) || i + 2 < b->values.size();
                std::string rr = lazy ? "[&] { return py::var(" + r + "); }" : r;
                o = std::string(b->op == BoolOpKind::And ? "py::and_(" : "py::or_(") + expr(b->values[i]) + ", " + rr + ")";
            }
            return o;
        }
        case ExprKind::Compare: return compare_value(cast<Compare>(e));
        case ExprKind::IfExp: {
            auto* i = cast<IfExp>(e);
            return "(" + cond(i->test) + " ? py::var(" + expr(i->body) + ") : py::var(" + expr(i->orelse) + "))";
        }
        case ExprKind::List: return display(cast<List>(e)->elts, "py::list", "py::list_display");
        case ExprKind::Tuple: return display(cast<Tuple>(e)->elts, "py::tuple", "py::tuple_display");
        case ExprKind::Set: return display(cast<Set>(e)->elts, "py::set", "py::set_display");
        case ExprKind::Dict: return dict_display(cast<Dict>(e));
        case ExprKind::ListComp: case ExprKind::SetComp: case ExprKind::DictComp: case ExprKind::GeneratorExp: return comprehension(e);
        case ExprKind::Lambda: return lambda(cast<Lambda>(e));
        case ExprKind::JoinedStr: return fstring(cast<JoinedStr>(e));
        case ExprKind::FormattedValue: {
            auto* f = cast<FormattedValue>(e);
            return "py::fmt(" + expr(f->value) + ", 0, py::None)";
        }
        case ExprKind::NamedExpr: {
            auto* n = cast<NamedExpr>(e);
            return "(" + store_name_expr(cast<Name>(n->target)) + " = " + expr(n->value) + ")";
        }
        case ExprKind::Yield: {
            auto* y = cast<Yield>(e);
            if (!fn_ || !fn_->generator) { refuse(e, "yield outside a generator"); return "py::None"; }
            return "y(" + (y->value ? expr(y->value) : std::string("py::None")) + ")";
        }
        case ExprKind::YieldFrom: {
            if (!fn_ || !fn_->generator) { refuse(e, "yield from outside a generator"); return "py::None"; }
            return "y.yield_from(" + expr(cast<YieldFrom>(e)->value) + ")";
        }
        case ExprKind::Await:
            if (!fn_ || !fn_->async_) { refuse(e, "await outside an async function"); return "py::None"; }
            return "y.await_(" + expr(cast<Await>(e)->value) + ")";
        case ExprKind::Starred: return "py::star(" + expr(cast<Starred>(e)->value) + ")";
        case ExprKind::Slice: return subscript_key(e);
        default: refuse(e, std::string("expression ") + name_of(e->kind)); return "py::None";
        }
    }

    // ---------------------------------------------------------------- targets
    std::string store_name_expr(const Name* n) {
        const Scope* where = sc_.enclosing(n);
        NameRes r = sc_.resolve(n);
        if (where && where->kind == Scope::Class && r.kind != NameRes::Free && !where->globals.count(n->id))
            return "py::at(" + fn_->class_var + ", " + slit(mangle(n->id)) + ")";
        if (r.kind == NameRes::Global) return gref(n->id);
        return local_name(n->id);
    }
    // `target = <value>`; value is a C++ expression (evaluated once by the caller)
    std::string assign_target(const Expr* tgt, const std::string& v) {
        switch (tgt->kind) {
        case ExprKind::Name: return store_name_expr(cast<Name>(tgt)) + " = " + v + ";";
        case ExprKind::Attribute: {
            auto* a = cast<Attribute>(tgt);
            return expr(a->value) + ".setattr(" + cstr(mangle(a->attr)) + ", " + v + ");";
        }
        case ExprKind::Subscript: {
            auto* s = cast<Subscript>(tgt);
            return expr(s->value) + ".setitem(" + subscript_key(s->slice) + ", " + v + ");";
        }
        case ExprKind::Tuple: case ExprKind::List: {
            const auto& elts = tgt->kind == ExprKind::Tuple ? cast<Tuple>(tgt)->elts : cast<List>(tgt)->elts;
            int star = -1;
            for (size_t i = 0; i < elts.size(); i++) if (elts[i]->kind == ExprKind::Starred) star = (int)i;
            std::string u = "py_u" + t();
            std::string o = "{ auto " + u + " = ";
            if (star < 0) o += "py::unpack(" + v + ", " + std::to_string(elts.size()) + "); ";
            else o += "py::unpack_star(" + v + ", " + std::to_string(star) + ", " + std::to_string(elts.size() - (size_t)star - 1) + "); ";
            for (size_t i = 0; i < elts.size(); i++) {
                const Expr* x = elts[i];
                if (auto* st = as<Starred>(x)) x = st->value;
                o += assign_target(x, u + "[" + std::to_string(i) + "]") + " ";
            }
            return o + "}";
        }
        case ExprKind::Starred: return assign_target(cast<Starred>(tgt)->value, v);
        default: refuse(tgt, "assignment target"); return ";";
        }
    }

    // ---------------------------------------------------------------- functions
    void signature(const arguments* a, std::string& sig, std::string& params, std::string& defaults) {
        sig.clear();
        params.clear();
        defaults.clear();
        auto add_param = [&](const std::string& n) { params += (params.empty() ? "" : ", ") + std::string("py::var ") + ident(n) + param_suffix(n); };
        size_t npos = a->posonlyargs.size() + a->args.size();
        size_t first_default = npos - a->defaults.size();
        size_t k = 0;
        auto pos_param = [&](const arg* p) {
            sig += (sig.empty() ? "" : ", ") + p->arg + (k >= first_default ? "=" : "");
            if (k >= first_default) defaults += (defaults.empty() ? "" : ", ") + expr(a->defaults[k - first_default]);
            add_param(p->arg);
            k++;
        };
        for (auto* p : a->posonlyargs) pos_param(p);
        if (!a->posonlyargs.empty()) sig += ", /";
        for (auto* p : a->args) pos_param(p);
        if (a->vararg) {
            sig += (sig.empty() ? "" : ", ") + std::string("*") + a->vararg->arg;
            add_param(a->vararg->arg);
        } else if (!a->kwonlyargs.empty()) sig += (sig.empty() ? "" : ", ") + std::string("*");
        for (size_t i = 0; i < a->kwonlyargs.size(); i++) {
            bool d = a->kw_defaults[i] != nullptr;
            sig += ", " + a->kwonlyargs[i]->arg + (d ? "=" : "");
            if (d) defaults += (defaults.empty() ? "" : ", ") + expr(a->kw_defaults[i]);
            add_param(a->kwonlyargs[i]->arg);
        }
        if (a->kwarg) {
            sig += (sig.empty() ? "" : ", ") + std::string("**") + a->kwarg->arg;
            add_param(a->kwarg->arg);
        }
    }
    const Scope* cur_param_scope_ = nullptr;
    std::string param_suffix(const std::string& n) { return cur_param_scope_ && is_cell(cur_param_scope_, n) ? "_in" : ""; }
    std::string param_cells(const Scope* s, const arguments* a) {
        std::string o;
        auto one = [&](const arg* p) {
            if (p && is_cell(s, p->arg)) o += "py::cell " + ident(p->arg) + "_cell; py::var& " + ident(p->arg) + " = (*" + ident(p->arg) + "_cell = " + ident(p->arg) + "_in); ";
        };
        for (auto* p : a->posonlyargs) one(p);
        for (auto* p : a->args) one(p);
        one(a->vararg);
        for (auto* p : a->kwonlyargs) one(p);
        one(a->kwarg);
        return o;
    }
    std::string locals_decl(const Scope* s, const std::string& ind) {
        std::string vars, cells;
        for (auto& n : s->bound) {
            if (s->params.count(n) || s->globals.count(n) || s->nonlocals.count(n)) continue;
            if (is_cell(s, n)) cells += ind + "py::cell " + ident(n) + "_cell;\n" + ind + "py::var& " + ident(n) + " = *" + ident(n) + "_cell;\n";
            else vars += (vars.empty() ? "" : ", ") + ident(n);
        }
        std::string o;
        if (!vars.empty()) o += ind + "py::var " + vars + ";\n";
        return o + cells;
    }

    // the __doc__ CPython (3.13+) keeps: the string with its tabs expanded, the first line's leading spaces and the
    // other lines' common indent removed; blank lines and the trailing newline stay (inspect.cleandoc does more)
    static bool doc_value(const std::vector<Stmt*>& body, std::string& out) {
        if (body.empty()) return false;
        auto* e = as<ExprStmt>(body[0]);
        std::string s;
        if (!e || !is_str_const(e->value, &s)) return false;
        std::vector<std::string> lines;
        size_t st = 0;
        for (size_t i = 0; i <= s.size(); i++)
            if (i == s.size() || s[i] == '\n') {
                std::string l = s.substr(st, i - st), x;
                for (char c : l) {
                    if (c == '\t') x.append(8 - x.size() % 8, ' ');
                    else x += c;
                }
                lines.push_back(x);
                st = i + 1;
            }
        size_t margin = std::string::npos;
        for (size_t i = 1; i < lines.size(); i++) {
            size_t k = lines[i].find_first_not_of(' ');
            if (k != std::string::npos) margin = std::min(margin, k);
        }
        out.clear();
        for (size_t i = 0; i < lines.size(); i++) {
            std::string l = lines[i];
            if (i == 0) l.erase(0, std::min(l.size(), l.find_first_not_of(' ') == std::string::npos ? l.size() : l.find_first_not_of(' ')));
            else {
                size_t k = l.find_first_not_of(' ');
                l.erase(0, std::min(k == std::string::npos ? l.size() : k, margin == std::string::npos ? l.size() : margin));
            }
            out += (i ? "\n" : "") + l;
        }
        return true;
    }
    // a def's body as C++ statements; `decl` gets the C++ head
    std::string function_body(const FunctionDef* f, const Scope* s, bool generator, const std::string& ind) {
        std::string o;
        std::string doc;
        if (docstring(f->body, doc)) {
            std::istringstream ds(doc);
            std::string line;
            while (std::getline(ds, line)) o += ind + "// " + comment_text(line) + "\n";
        }
        if (!fn_->file_scope) {
            std::string fd = free_decls(s);
            if (!fd.empty()) o += ind + fd + "\n";
        }
        std::string pc = param_cells(s, f->args);
        if (!pc.empty()) o += ind + pc + "\n";
        o += locals_decl(s, ind);
        size_t start = 0;
        if (!f->body.empty() && f->body[0]->kind == StmtKind::Expr && is_str_const(cast<ExprStmt>(f->body[0])->value)) start = 1;
        for (size_t i = start; i < f->body.size(); i++) o += stmt(f->body[i], ind);
        if (f->body.empty() || !ends_with_return(f->body)) o += ind + "return py::None;\n";
        (void)generator;
        return o;
    }
    static bool ends_with_return(const std::vector<Stmt*>& b) {
        if (b.empty()) return false;
        const Stmt* s = b.back();
        return s->kind == StmtKind::Return || s->kind == StmtKind::Raise;
    }

    // the function object for a def (and, at file scope, the C++ function written out)
    std::string make_def(const FunctionDef* f, const std::string& cpp_name, const std::string& qualname, const std::string& ind,
                         const std::string& class_cell, bool method) {
        const Scope* s = sc_.scope_of(f);
        bool is_async = f->kind == StmtKind::AsyncFunctionDef;
        bool yields = is_generator(f);
        bool gen = yields || is_async;   // (a coroutine runs on a fiber as a generator does)
        res_.functions++;
        std::string sig, params, defaults;
        Fn* outer = fn_;
        // defaults are evaluated where the def is
        const Scope* saved_scope = cur_param_scope_;   // (a lambda in a default: the outer signature goes on after it)
        cur_param_scope_ = s;
        signature(f->args, sig, params, defaults);
        cur_param_scope_ = saved_scope;
        Fn inner;
        inner.scope = s;
        inner.generator = gen;
        inner.async_ = is_async;
        inner.qual = qualname;
        inner.file_scope = cpp_name != "";
        inner.class_cell = method ? class_cell : (outer ? outer->class_cell : "");
        if (method || (outer && !outer->self_name.empty())) {
            if (method) {
                if (!f->args->posonlyargs.empty()) inner.self_name = f->args->posonlyargs[0]->arg;
                else if (!f->args->args.empty()) inner.self_name = f->args->args[0]->arg;
            } else inner.self_name = outer->self_name;
        }
        std::string yparam = gen ? std::string("py::yielder& y") + (params.empty() ? "" : ", ") : "";
        std::string head = "py::var " + std::string(cpp_name) + "(" + yparam + params + ")";
        fn_ = &inner;
        std::string body = function_body(f, s, gen, inner.file_scope ? "    " : ind + "    ");
        fn_ = outer;
        std::string maker = is_async ? (yields ? "py::def_async_generator" : "py::def_coroutine") : gen ? "py::def_generator" : "py::def";
        std::string tail = ", " + cstr(qualname) + ", " + cstr(m_.name) + ")";
        // the annotations, as their source text (what `from __future__ import annotations` keeps): __annotations__
        // and inspect.signature give them
        std::string anns;
        auto ann = [&](const std::string& name, const Expr* e) {
            if (!e) return;
            std::string text;
            if (!source_segment(src_, starts_, e->pos, text)) return;
            anns += (anns.empty() ? "" : ", ") + cstr(name) + ", " + cstr(text);
        };
        for (auto* p : f->args->posonlyargs) ann(p->arg, p->annotation);
        for (auto* p : f->args->args) ann(p->arg, p->annotation);
        if (f->args->vararg) ann(f->args->vararg->arg, f->args->vararg->annotation);
        for (auto* p : f->args->kwonlyargs) ann(p->arg, p->annotation);
        if (f->args->kwarg) ann(f->args->kwarg->arg, f->args->kwarg->annotation);
        ann("return", f->returns);
        std::string pre = anns.empty() ? "" : "py::annotate_(", post = anns.empty() ? "" : ", {" + anns + "})";
        std::string fdoc;
        if (doc_value(f->body, fdoc)) {   // (its __doc__)
            pre = "py::doc_(" + pre;
            post += ", " + cstr(fdoc) + ")";
        }
        if (inner.file_scope && !method && !gen && !is_async && islands_) {
            if (const Island* is = island_for(f, qualname)) {
                write_island(*is, f, s, cpp_name, params, head, body);
                return pre + maker + "(" + cstr(f->name) + ", " + cstr(sig) + ", {" + defaults + "}, " + cpp_name + tail + post;
            }
        }
        if (inner.file_scope) {
            protos_ += "static " + head + ";\n";
            std::string comment = "// " + src_line(f) + "\n";
            funcs_ += comment + "static " + head + " {\n" + body + "}\n\n";
            return pre + maker + "(" + cstr(f->name) + ", " + cstr(sig) + ", {" + defaults + "}, " + cpp_name + tail + post;
        }
        // a closure
        std::string caps = captures(s);
        return pre + maker + "(" + cstr(f->name) + ", " + cstr(sig) + ", {" + defaults + "}, [" + caps + "](" + yparam + params + ") -> py::var {\n" +
               body + ind + "}" + tail + post;
    }

    // ---------------------------------------------------------------- typed islands (--dyn-typed)
    // A module function the typed path proved (islands.hpp) is written twice: its dynamic body as usual under
    // another name, and under its own name a wrapper with the same signature that checks and unboxes the
    // arguments, calls the typed body (islands.cpp, through native/pyengine/islands/pyisl.hpp) and boxes the
    // result -- or calls the dynamic body for any other argument and whenever the typed body raises.
    const Island* island_for(const FunctionDef* f, const std::string& qualname) {
        if (qualname != f->name) return nullptr;
        std::string q = m_.name + "." + f->name;
        auto it = islands_->emit.find(q);
        if (it == islands_->emit.end()) return nullptr;
        const Island& is = it->second;
        auto no = [&](const std::string& why) -> const Island* {
            (*island_log_)[q] = why;
            return nullptr;
        };
        if (!def_only_.count(f->name)) return no("the module binds " + f->name + " more than once");
        const arguments* a = f->args;
        if (a->vararg || a->kwarg || !a->kwonlyargs.empty()) return no("*args, **kwargs or keyword-only parameters");
        std::vector<std::string> names;
        for (auto* p : a->posonlyargs) names.push_back(p->arg);
        for (auto* p : a->args) names.push_back(p->arg);
        if (names.size() != is.params.size()) return no("the typed signature has " + std::to_string(is.params.size()) + " parameters");
        for (size_t i = 0; i < names.size(); i++)
            if (names[i] != is.params[i].name) return no("parameter " + names[i] + " is " + is.params[i].name + " in the typed signature");
        return &is;
    }
    static std::string int_bound(int64_t v) {
        if (v == INT64_MIN) return "INT64_MIN";
        if (v == INT64_MAX) return "INT64_MAX";
        return std::to_string(v) + "LL";
    }
    void write_island(const Island& is, const FunctionDef* f, const Scope* s, const std::string& cpp_name, const std::string& params,
                      const std::string& head, const std::string& body) {
        std::string dyn_name = unique_fn(cpp_name + "_dyn");
        std::string dyn_head = "py::var " + dyn_name + "(" + params + ")";
        std::string comment = "// " + src_line(f) + "\n";
        protos_ += "static " + dyn_head + ";\n";
        protos_ += "static " + head + ";\n";
        funcs_ += comment + "static " + dyn_head + " {\n" + body + "}\n\n";
        // the wrapper
        std::string decls, checks, args, fwd;
        std::vector<std::string> pnames;
        for (auto* p : f->args->posonlyargs) pnames.push_back(p->arg);
        for (auto* p : f->args->args) pnames.push_back(p->arg);
        for (size_t i = 0; i < is.params.size(); i++) {
            const IslandParam& p = is.params[i];
            std::string v = ident(pnames[i]) + (is_cell(s, pnames[i]) ? "_in" : "");
            std::string u = "isl_" + ident(pnames[i]);
            fwd += (fwd.empty() ? "" : ", ") + v;
            std::string c;
            if (p.kind == "int") {
                decls += "    int64_t " + u + ";\n";
                c = "pyisl::int_in(" + v + ", " + int_bound(p.lo) + ", " + int_bound(p.hi) + ", " + u + ")";
                args += u;
            } else if (p.kind == "bool") {
                decls += "    bool " + u + ";\n";
                c = "pyisl::boolean(" + v + ", " + u + ")";
                args += u;
            } else if (p.kind == "float") {
                decls += "    double " + u + ";\n";
                c = "pyisl::real(" + v + ", " + u + ")";
                args += u;
            } else if (p.kind == "str" || p.kind == "bytes") {
                decls += "    const std::string* " + u + ";\n";
                c = "(" + u + " = pyisl::" + p.kind + "_arg(" + v + "))";
                args += "*" + u;
            } else {
                decls += "    pyisl::val " + u + ";\n";
                c = "pyisl::value(" + v + ", " + cstr(p.spec) + ", " + u + ")";
                args += u;
            }
            checks += " && " + c;
            args += ", ";
        }
        std::string o = "// typed island: " + is.qual + " runs as the typed path's C++ (islands.cpp) when its arguments are the\n"
                        "// values the proof took; anything else, or an exception, runs the dynamic body above\n";
        o += "static " + head + " {\n" + decls;
        o += "    if (pyisl::on()" + checks + ") {\n";
        o += "        pyisl::val isl_result;\n";
        o += "        if (pyisl::island::" + is.ns + "::" + is.cpp + "(" + args + "isl_result)) return pyisl::box(isl_result);\n";
        o += "    }\n";
        o += "    return " + dyn_name + "(" + fwd + ");\n}\n\n";
        funcs_ += o;
        (*island_log_)[is.qual] = "";
        wrote_island_ = true;
    }

    std::string decorate(const std::vector<Expr*>& decos, const std::string& v) {
        std::string o = v;
        for (size_t i = decos.size(); i-- > 0;) {
            const Expr* d = decos[i];
            if (name_is_builtin(d, "property")) o = "py::property(" + o + ")";
            else if (name_is_builtin(d, "staticmethod")) o = "py::staticmethod(" + o + ")";
            else if (name_is_builtin(d, "classmethod")) o = "py::classmethod(" + o + ")";
            else o = paren(expr(d)) + "(" + o + ")";
        }
        return o;
    }

    // ---------------------------------------------------------------- classes
    std::string class_def(const ClassDef* c, const std::string& ind, const std::string& prefix) {
        const Scope* s = sc_.scope_of(c);
        std::string ns = "py_ns" + t();
        std::string qual = prefix.empty() ? c->name : prefix + "." + c->name;
        bool file_level = !fn_ || fn_->file_scope || fn_->scope == nullptr || fn_->scope->kind == Scope::Module || fn_->scope->kind == Scope::Class;
        if (fn_ && fn_->scope && fn_->scope->kind != Scope::Module && fn_->scope->kind != Scope::Class) file_level = false;
        std::string cell_name;
        if (file_level) {
            cell_name = dotted_ident(qual) + "__class";
            std::replace(cell_name.begin(), cell_name.end(), '.', '_');
            statics_ += "static py::var " + cell_name + ";   // class " + qual + " (for super() and __class__)\n";
        } else {
            cell_name = "py_cls" + t() + "_cell";
        }
        std::string o = ind + "// " + src_line(c) + "\n" + ind + "{\n";
        std::string in = ind + "    ";
        if (!file_level) o = ind + "// " + src_line(c) + "\n" + ind + "py::var " + cell_name + ";\n" + ind + "{\n";
        o += in + "py::var " + ns + " = py::new_dict();\n";
        o += in + ns + ".setitem(\"__module__\"_s, " + slit(m_.name) + ");\n";
        o += in + ns + ".setitem(\"__qualname__\"_s, " + slit(qual) + ");\n";
        Fn* outer = fn_;
        Fn body;
        body.scope = s;
        body.file_scope = outer ? outer->file_scope : true;
        body.class_var = ns;
        body.class_cell = cell_name;
        body.qual = qual;
        body.self_name = "";
        // a class body inside a function: its defs are closures
        bool in_function = outer && outer->scope && outer->scope->kind != Scope::Module && outer->scope->kind != Scope::Class;
        fn_ = &body;
        classes_.push_back(c->name);
        std::string annotations;
        size_t start = 0;
        std::string doc;
        if (doc_value(c->body, doc)) {
            o += in + ns + ".setitem(\"__doc__\"_s, " + slit(doc) + ");\n";
            start = 1;
        }
        for (size_t i = start; i < c->body.size(); i++) {
            const Stmt* st = c->body[i];
            if (st->kind == StmtKind::FunctionDef || st->kind == StmtKind::AsyncFunctionDef) {
                auto* f = cast<FunctionDef>(st);
                std::string cpp = in_function ? "" : unique_fn(dotted_ident(qual + "." + f->name));
                std::string v = make_def(f, cpp, qual + "." + f->name, in, cell_name, true);
                v = decorate(f->decorator_list, v);
                o += in + ns + ".setitem(" + slit(mangle(f->name)) + ", " + v + ");\n";
            } else if (st->kind == StmtKind::ClassDef) {
                auto* cc = cast<ClassDef>(st);
                std::string inner_cls = class_def(cc, in, qual);
                o += inner_cls;
            } else if (st->kind == StmtKind::AnnAssign) {
                auto* a = cast<AnnAssign>(st);
                if (auto* n = as<Name>(a->target)) {
                    std::string ann_text;
                    source_segment(src_, starts_, a->annotation->pos, ann_text);
                    if (annotations.empty()) {
                        annotations = "py_ann" + t();
                        o += in + "py::var " + annotations + " = py::dict();\n" + in + ns + ".setitem(\"__annotations__\"_s, " + annotations + ");\n";
                    }
                    o += in + annotations + ".setitem(" + slit(n->id) + ", " + slit(ann_text) + ");\n";
                    if (a->value) o += in + ns + ".setitem(" + slit(n->id) + ", " + expr(a->value) + ");\n";
                } else if (a->value) o += stmt(st, in);
            } else {
                o += stmt(st, in);
            }
        }
        fn_ = outer;
        classes_.pop_back();
        std::string bases;
        for (auto* b : c->bases) bases += (bases.empty() ? "" : ", ") + expr(b);
        std::string kws;
        for (auto* k : c->keywords) kws += (kws.empty() ? "" : ", ") + std::string("{") + cstr(k->arg) + ", " + expr(k->value) + "}";
        std::string mk = "py::make_class(" + cstr(c->name) + ", " + cstr(qual) + ", {" + bases + "}, " + ns + ", {" + kws + "}, " + cstr(m_.name) + ")";
        o += in + cell_name + " = " + decorate(c->decorator_list, mk) + ";\n";
        // bind the class name where the class statement is
        Name tmp;
        (void)tmp;
        o += in + class_name_store(c, prefix) + " = " + cell_name + ";\n";
        o += ind + "}\n";
        return o;
    }
    std::string class_name_store(const ClassDef* c, const std::string& prefix) {
        // the scope the class statement is in
        const Scope* where = nullptr;
        const Scope* cs = sc_.scope_of(c);
        if (cs) where = cs->parent;
        if (!where || where->kind == Scope::Module) return gref(c->name);
        if (where->kind == Scope::Class) return "py::at(" + fn_->class_var + ", " + slit(mangle(c->name)) + ")";
        (void)prefix;
        if (where->globals.count(c->name)) return gref(c->name);
        return local_name(c->name);
    }
    std::string unique_fn(std::string n) {
        std::replace(n.begin(), n.end(), '.', '_');
        // Class.method -> Class__method
        std::string base = n;
        while (used_fn_names_.count(n) || global_names_.count(n)) n = base + "_" + t();
        used_fn_names_.insert(n);
        return n;
    }

    // ---------------------------------------------------------------- statements
    std::string stmt(const Stmt* s, const std::string& ind);
    std::string block(const std::vector<Stmt*>& b, const std::string& ind) {
        std::string o;
        for (auto* s : b) o += stmt(s, ind);
        if (o.empty()) o = ind + ";\n";
        return o;
    }
    bool has_escape(const std::vector<Stmt*>& body, bool loops_too);
    std::string try_stmt(const Try* t, const std::string& ind);
    std::string with_stmt(const With* w, size_t item, const std::string& ind);
    std::string async_with_stmt(const With* w, size_t item, const std::string& ind);
    std::string match_stmt(const Match* m, const std::string& ind);
    std::string pattern(const Pattern* p, const std::string& subj, const std::string& ok, const std::string& ind);
    std::string escape(const char* kind, const std::string& value, const std::string& ind);
    std::string escape_after(const Region& r, const std::string& ind);
    std::string plain_try(const Try* t, const std::string& ind);
    std::string import_stmt(const Stmt* s, const std::string& ind);
    std::string local_or_global(const std::string& n) {
        const Scope* where = fn_ ? fn_->scope : nullptr;
        if (!where || where->kind == Scope::Module || where->globals.count(n)) return gref(n);
        if (where->kind == Scope::Class) return "py::at(" + fn_->class_var + ", " + slit(mangle(n)) + ")";
        return local_name(n);
    }
    std::string bind_name(const std::string& n) {
        // a name bound by an import, a def or a class in the current scope
        const Scope* where = fn_ ? fn_->scope : nullptr;
        if (!where || where->kind == Scope::Module || where->globals.count(n)) return gref(n);
        if (where->kind == Scope::Class) return "py::at(" + fn_->class_var + ", " + slit(mangle(n)) + ")";
        return local_name(n);
    }

public:
    std::string module_name() const { return m_.name; }
};

bool Emitter::has_escape(const std::vector<Stmt*>& body, bool loops_too) {
    std::function<bool(const Stmt*, bool)> walk_s = [&](const Stmt* s, bool in_loop) -> bool {
        switch (s->kind) {
        case StmtKind::Return: return true;
        case StmtKind::Break: case StmtKind::Continue: return !in_loop && loops_too;
        case StmtKind::FunctionDef: case StmtKind::AsyncFunctionDef: case StmtKind::ClassDef: return false;
        default: break;
        }
        bool loop = s->kind == StmtKind::For || s->kind == StmtKind::While || s->kind == StmtKind::AsyncFor;
        for (auto* l : stmt_lists(s))
            for (auto* x : *l) {
                // a loop's own body: break / continue are its own (its orelse's are not)
                bool inner_loop = in_loop || (loop && l == &(s->kind == StmtKind::While ? cast<While>(s)->body : cast<For>(s)->body));
                if (walk_s(x, inner_loop)) return true;
            }
        return false;
    };
    for (auto* s : body)
        if (walk_s(s, false)) return true;
    return false;
}

// return / break / continue inside protected blocks: through each region's cleanup
std::string Emitter::escape(const char* kind, const std::string& value, const std::string& ind) {
    std::string k = kind;
    // the innermost protected region before the target (a loop for break/continue, the function for return)
    for (size_t i = fn_->regions.size(); i-- > 0;) {
        Region& r = fn_->regions[i];
        if (r.is_loop) {
            if (k == "return") continue;
            // break / continue reach their loop: nothing protected in between
            if (k == "break" && !r.else_flag.empty()) return ind + "{ " + r.else_flag + " = true; break; }\n";
            return ind + k + ";\n";
        }
        std::string id = std::to_string(r.id);
        std::string o = ind + "{ ";
        if (k == "return") { o += "py_ret" + id + " = " + value + "; py_exit" + id + " = 1; "; r.used_ret = true; }
        else { o += "py_exit" + id + " = " + (k == "break" ? "2" : "3") + "; "; (k == "break" ? r.used_brk : r.used_cnt) = true; }
        return o + "goto py_fin" + id + "; }\n";
    }
    if (k == "return") return ind + "return " + value + ";\n";
    return ind + k + ";\n";
}

std::string Emitter::stmt(const Stmt* s, const std::string& ind) {
    switch (s->kind) {
    case StmtKind::Expr: {
        auto* e = cast<ExprStmt>(s)->value;
        if (is_str_const(e)) return "";   // (a string statement: a comment in Python)
        return ind + expr(e) + ";\n";
    }
    case StmtKind::Pass: return "";
    case StmtKind::Return: {
        auto* r = cast<Return>(s);
        std::string v = r->value ? expr(r->value) : "py::None";
        if (fn_ && fn_->generator) v = r->value ? v : "py::None";
        return escape("return", v, ind);
    }
    case StmtKind::Assign: {
        auto* a = cast<Assign>(s);
        if (a->targets.size() == 1 && a->targets[0]->kind == ExprKind::Name) return ind + assign_target(a->targets[0], expr(a->value)) + "\n";
        if (a->targets.size() == 1 && (a->targets[0]->kind == ExprKind::Attribute || a->targets[0]->kind == ExprKind::Subscript) && simple(a->value))
            return ind + assign_target(a->targets[0], expr(a->value)) + "\n";
        std::string v = "py_v" + t();
        std::string o = ind + "{\n" + ind + "    py::var " + v + " = " + expr(a->value) + ";\n";
        for (auto* tg : a->targets) o += ind + "    " + assign_target(tg, v) + "\n";
        return o + ind + "}\n";
    }
    case StmtKind::AugAssign: {
        auto* a = cast<AugAssign>(s);
        std::string v = operand(a->value);
        static const std::map<OpKind, std::string> ops = {{OpKind::Add, "+="}, {OpKind::Sub, "-="}, {OpKind::Mult, "*="}, {OpKind::Div, "/="},
                                                          {OpKind::Mod, "%="}, {OpKind::BitOr, "|="}, {OpKind::BitAnd, "&="}, {OpKind::BitXor, "^="},
                                                          {OpKind::LShift, "<<="}, {OpKind::RShift, ">>="}};
        auto it = ops.find(a->op);
        if (auto* n = as<Name>(a->target)) {
            std::string tg = store_name_expr(n);
            std::string cur = expr(a->target);
            if (it != ops.end() && tg == cur) return ind + tg + " " + it->second + " " + v + ";\n";
            if (it != ops.end()) return ind + "{ py::var py_cur = " + cur + "; py_cur " + it->second + " " + v + "; " + tg + " = py_cur; }\n";
            return ind + tg + " = " + binop_s(a->op, cur, v) + ";\n";
        }
        if (auto* at = as<Attribute>(a->target)) {
            std::string o = "py_o" + t();
            std::string op = it != ops.end() ? "py_c " + it->second + " " + v + ";" : "py_c = " + binop_s(a->op, "py_c", v) + ";";
            return ind + "{ py::var " + o + " = " + expr(at->value) + "; py::var py_c = " + o + ".getattr(" + cstr(mangle(at->attr)) + "); " + op + " " + o +
                   ".setattr(" + cstr(mangle(at->attr)) + ", py_c); }\n";
        }
        if (auto* sb = as<Subscript>(a->target)) {
            std::string o = "py_o" + t();
            std::string op = it != ops.end() ? "py_c " + it->second + " " + v + ";" : "py_c = " + binop_s(a->op, "py_c", v) + ";";
            return ind + "{ py::var " + o + " = " + expr(sb->value) + "; py::var py_k = " + subscript_key(sb->slice) + "; py::var py_c = " + o +
                   ".getitem(py_k); " + op + " " + o + ".setitem(py_k, py_c); }\n";
        }
        refuse(s, "augmented assignment target");
        return "";
    }
    case StmtKind::AnnAssign: {
        auto* a = cast<AnnAssign>(s);
        if (!a->value) return "";
        return ind + assign_target(a->target, expr(a->value)) + "\n";
    }
    case StmtKind::If: {
        auto* i = cast<If>(s);
        std::string o = ind + "if (" + cond(i->test) + ") {\n" + block(i->body, ind + "    ") + ind + "}";
        const If* cur = i;
        while (cur->orelse.size() == 1 && cur->orelse[0]->kind == StmtKind::If) {
            cur = cast<If>(cur->orelse[0]);
            o += " else if (" + cond(cur->test) + ") {\n" + block(cur->body, ind + "    ") + ind + "}";
        }
        if (!cur->orelse.empty()) o += " else {\n" + block(cur->orelse, ind + "    ") + ind + "}";
        return o + "\n";
    }
    case StmtKind::While: {
        auto* w = cast<While>(s);
        std::string o;
        std::string flag;
        if (!w->orelse.empty()) {
            flag = "py_broke" + t();
            o += ind + "bool " + flag + " = false;\n";
        }
        Region lr;
        lr.is_loop = true;
        lr.else_flag = flag;
        fn_->regions.push_back(lr);
        std::string body = block(w->body, ind + "    ");
        fn_->regions.pop_back();
        o += ind + "while (" + cond(w->test) + ") {\n" + body + ind + "}\n";
        if (!flag.empty()) o += ind + "if (!" + flag + ") {\n" + block(w->orelse, ind + "    ") + ind + "}\n";
        return o;
    }
    case StmtKind::For: {
        auto* f = cast<For>(s);
        std::string o;
        std::string flag;
        if (!f->orelse.empty()) {
            flag = "py_broke" + t();
            o += ind + "bool " + flag + " = false;\n";
        }
        std::string v = "py_it" + t();
        Region lr;
        lr.is_loop = true;
        lr.else_flag = flag;
        fn_->regions.push_back(lr);
        std::string body = block(f->body, ind + "    ");
        fn_->regions.pop_back();
        o += ind + "for (py::var " + v + " : py::each(" + expr(f->iter) + ")) {\n" + ind + "    " + assign_target(f->target, v) + "\n" + body + ind + "}\n";
        if (!flag.empty()) o += ind + "if (!" + flag + ") {\n" + block(f->orelse, ind + "    ") + ind + "}\n";
        return o;
    }
    case StmtKind::Break: return escape("break", "", ind);
    case StmtKind::Continue: return escape("continue", "", ind);
    case StmtKind::Raise: {
        auto* r = cast<Raise>(s);
        if (!r->exc) return ind + "py::reraise();\n";
        if (r->cause) return ind + "py::raise_from(" + expr(r->exc) + ", " + expr(r->cause) + ");\n";
        return ind + "py::raise(" + expr(r->exc) + ");\n";
    }
    case StmtKind::Assert: {
        auto* a = cast<Assert>(s);
        return ind + "if (!" + paren(cond(a->test)) + ") py::assert_(false" + (a->msg ? ", " + expr(a->msg) : std::string()) + ");\n";
    }
    case StmtKind::Delete: {
        std::string o;
        for (auto* tg : cast<Delete>(s)->targets) {
            if (auto* n = as<Name>(tg)) {
                const Scope* where = sc_.enclosing(n);
                if (where && where->kind == Scope::Class) o += ind + fn_->class_var + ".delitem(" + slit(n->id) + ");\n";
                else o += ind + store_name_expr(n) + " = py::var();\n";
            } else if (auto* at = as<Attribute>(tg)) o += ind + expr(at->value) + ".delattr(" + cstr(mangle(at->attr)) + ");\n";
            else if (auto* sb = as<Subscript>(tg)) o += ind + expr(sb->value) + ".delitem(" + subscript_key(sb->slice) + ");\n";
            else if (auto* tu = as<Tuple>(tg)) {
                Delete d2;
                d2.targets = tu->elts;
                o += stmt(&d2, ind);
            }
        }
        return o;
    }
    case StmtKind::Global: case StmtKind::Nonlocal: return "";
    case StmtKind::Import: case StmtKind::ImportFrom: return import_stmt(s, ind);
    case StmtKind::FunctionDef: case StmtKind::AsyncFunctionDef: {
        auto* f = cast<FunctionDef>(s);
        bool at_module = !fn_ || !fn_->scope || fn_->scope->kind == Scope::Module;
        std::string target = bind_name(f->name);
        std::string v;
        if (at_module) {
            std::string cpp = ident(f->name);
            if (used_fn_names_.count(cpp) || module_binds_[f->name] > 1) cpp = unique_fn(cpp);
            else used_fn_names_.insert(cpp);
            // a module function the module calls directly keeps the plain name
            if (!def_only_.count(f->name) && cpp == ident(f->name)) cpp = unique_fn(ident(f->name) + "_impl");
            v = make_def(f, cpp, f->name, ind, "", false);
            if (def_only_.count(f->name)) {
                def_mark(f->name);
                return ind + "// " + src_line(f) + "\n" + ind + target + " = " + decorate(f->decorator_list, v) + ";\n" + ind +
                       ident(f->name) + "_def_ = " + target + ".ref();\n";
            }
        } else {
            std::string qual = (fn_ && !fn_->qual.empty() ? fn_->qual + ".<locals>." : std::string()) + f->name;
            v = make_def(f, "", qual, ind, "", false);
        }
        return ind + "// " + src_line(f) + "\n" + ind + target + " = " + decorate(f->decorator_list, v) + ";\n";
    }
    case StmtKind::ClassDef: return class_def(cast<ClassDef>(s), ind, fn_ && fn_->scope && fn_->scope->kind != Scope::Module && !fn_->qual.empty() && fn_->class_var.empty() ? fn_->qual + ".<locals>" : "");
    case StmtKind::Try: return try_stmt(cast<Try>(s), ind);
    case StmtKind::TryStar: refuse(s, "except*"); return "";
    case StmtKind::With: return with_stmt(cast<With>(s), 0, ind);
    case StmtKind::AsyncWith:
        if (!fn_ || !fn_->async_) { refuse(s, "async with outside an async function"); return ""; }
        return async_with_stmt(cast<With>(s), 0, ind);
    case StmtKind::AsyncFor: {
        if (!fn_ || !fn_->async_) { refuse(s, "async for outside an async function"); return ""; }
        auto* f = cast<For>(s);
        std::string o;
        std::string flag;
        if (!f->orelse.empty()) {
            flag = "py_broke" + t();
            o += ind + "bool " + flag + " = false;\n";
        }
        std::string v = "py_it" + t();
        Region lr;
        lr.is_loop = true;
        lr.else_flag = flag;
        fn_->regions.push_back(lr);
        std::string body = block(f->body, ind + "        ");
        fn_->regions.pop_back();
        // async for: __anext__() awaited until StopAsyncIteration
        o += ind + "{   // async for " + src_line(f) + "\n";
        o += ind + "    py::var " + v + "a = py::aiter_(" + expr(f->iter) + ");\n";
        o += ind + "    for (;;) {\n" + ind + "        py::var " + v + ";\n";
        o += ind + "        if (!y.async_next(" + v + "a, " + v + ")) break;\n";
        o += ind + "        " + assign_target(f->target, v) + "\n" + body + ind + "    }\n" + ind + "}\n";
        if (!flag.empty()) o += ind + "if (!" + flag + ") {\n" + block(f->orelse, ind + "    ") + ind + "}\n";
        return o;
    }
    case StmtKind::Match: return match_stmt(cast<Match>(s), ind);
    case StmtKind::TypeAlias: refuse(s, "type alias statement"); return "";
    default: refuse(s, std::string("statement ") + name_of(s->kind)); return "";
    }
}

static std::string replace_all(std::string s, const std::string& a, const std::string& b) {
    size_t p = 0;
    while ((p = s.find(a, p)) != std::string::npos) { s.replace(p, a.size(), b); p += b.size(); }
    return s;
}

std::string Emitter::escape_after(const Region& r, const std::string& in) {
    std::string id = std::to_string(r.id);
    std::string o;
    if (r.used_ret) o += in + "if (py_exit" + id + " == 1)\n" + escape("return", "py_ret" + id, in + "    ");
    if (r.used_brk) o += in + "if (py_exit" + id + " == 2)\n" + escape("break", "", in + "    ");
    if (r.used_cnt) o += in + "if (py_exit" + id + " == 3)\n" + escape("continue", "", in + "    ");
    return o;
}

// try / except / else (no finally)
std::string Emitter::plain_try(const Try* tr, const std::string& ind) {
    std::string sid = t();
    std::string body_ind = ind + "    ";
    std::string o;
    std::string okflag;
    if (!tr->orelse.empty()) {
        okflag = "py_ok" + sid;
        o += ind + "bool " + okflag + " = false;\n";
    }
    o += ind + "try {\n" + block(tr->body, body_ind);
    if (!okflag.empty()) o += body_ind + okflag + " = true;\n";
    o += ind + "}";
    if (!tr->handlers.empty()) {
        std::string e = "py_e" + sid;
        o += " catch (py::error& " + e + ") {\n";
        o += body_ind + "py::handling_ py_h" + sid + "(" + e + ");\n";
        bool first = true, bare = false;
        for (auto* h : tr->handlers) {
            std::string hb = body_ind + "    ";
            std::string bind = h->has_name ? hb + local_or_global(h->name) + " = " + e + ".value;\n" : "";
            std::string unbind = h->has_name ? hb + local_or_global(h->name) + " = py::var();\n" : "";
            std::string hbody = bind + block(h->body, hb) + (ends_with_return(h->body) ? "" : unbind);
            if (!h->type) {
                o += body_ind + (first ? "{\n" : "else {\n") + hbody + body_ind + "}\n";
                bare = true;
                break;
            }
            o += body_ind + (first ? "if" : "else if") + std::string(" (") + e + ".matches(" + expr(h->type) + ")) {\n" + hbody + body_ind + "}\n";
            first = false;
        }
        if (!bare) o += body_ind + "else throw;\n";
        o += ind + "}";
    } else {
        o += " catch (...) {\n" + body_ind + "throw;\n" + ind + "}";
    }
    o += "\n";
    if (!okflag.empty()) o += ind + "if (" + okflag + ") {\n" + block(tr->orelse, body_ind) + ind + "}\n";
    return o;
}

std::string Emitter::try_stmt(const Try* tr, const std::string& ind) {
    if (tr->finalbody.empty()) return plain_try(tr, ind);
    int id = ++tmp_;
    std::string sid = std::to_string(id);
    std::string in = ind + "    ";
    Region r;
    r.id = id;
    fn_->regions.push_back(r);
    std::string inner;
    if (tr->handlers.empty() && tr->orelse.empty()) inner = block(tr->body, in + "    ");
    else inner = plain_try(tr, in + "    ");
    Region done = fn_->regions.back();
    fn_->regions.pop_back();
    std::string o = ind + "{\n";
    o += in + "py::var py_ret" + sid + "; int py_exit" + sid + " = 0; std::exception_ptr py_exc" + sid + ";\n";
    o += in + "try {\n" + inner + in + "} catch (...) {\n" + in + "    py_exc" + sid + " = std::current_exception();\n" + in + "}\n";
    o += in + "py_fin" + sid + ":   // finally\n";
    o += block(tr->finalbody, in);
    o += in + "if (py_exc" + sid + ") std::rethrow_exception(py_exc" + sid + ");\n";
    o += escape_after(done, in);
    o += ind + "}\n";
    return o;
}

std::string Emitter::with_stmt(const With* w, size_t item, const std::string& ind) {
    if (item >= w->items.size()) return block(w->body, ind);
    const withitem* wi = w->items[item];
    int id = ++tmp_;
    std::string sid = std::to_string(id);
    std::string in = ind + "    ";
    std::string o = ind + "{   // with " + (item == 0 ? src_line(w) : std::string("...")) + "\n";
    o += in + "py::var py_mgr" + sid + " = " + expr(wi->context_expr) + ";\n";
    o += in + "py::var py_exitfn" + sid + " = py::with_exit_of(py_mgr" + sid + ");\n";
    if (wi->optional_vars) o += in + assign_target(wi->optional_vars, "py::with_enter(py_mgr" + sid + ")") + "\n";
    else o += in + "py::with_enter(py_mgr" + sid + ");\n";
    o += in + "py::var py_ret" + sid + "; int py_exit" + sid + " = 0; bool py_exited" + sid + " = false;\n";
    Region r;
    r.id = id;
    fn_->regions.push_back(r);
    std::string body = with_stmt(w, item + 1, in + "    ");
    Region done = fn_->regions.back();
    fn_->regions.pop_back();
    o += in + "try {\n" + body + in + "} catch (py::error& e" + sid + ") {\n";
    o += in + "    py_exited" + sid + " = true;\n";
    o += in + "    py::handling_ py_h" + sid + "(e" + sid + ");\n";
    o += in + "    if (!py::with_exit_exc(py_exitfn" + sid + ", e" + sid + ")) throw;\n";
    o += in + "}\n";
    o += in + "py_fin" + sid + ":\n";
    o += in + "if (!py_exited" + sid + ") py_exitfn" + sid + "(py::None, py::None, py::None);\n";
    o += escape_after(done, in);
    o += ind + "}\n";
    return o;
}

// async with: __aenter__() and __aexit__(...) awaited. The exception is taken out of its catch block
// before __aexit__ is awaited (the coroutine's fiber may switch away while it waits).
std::string Emitter::async_with_stmt(const With* w, size_t item, const std::string& ind) {
    if (item >= w->items.size()) return block(w->body, ind);
    const withitem* wi = w->items[item];
    int id = ++tmp_;
    std::string sid = std::to_string(id);
    std::string in = ind + "    ";
    std::string o = ind + "{   // async with " + (item == 0 ? src_line(w) : std::string("...")) + "\n";
    o += in + "py::var py_mgr" + sid + " = " + expr(wi->context_expr) + ";\n";
    o += in + "py::var py_exitfn" + sid + " = py::awith_exit_of(py_mgr" + sid + ");\n";
    if (wi->optional_vars) o += in + assign_target(wi->optional_vars, "y.await_(py::awith_enter(py_mgr" + sid + "))") + "\n";
    else o += in + "y.await_(py::awith_enter(py_mgr" + sid + "));\n";
    o += in + "py::var py_ret" + sid + "; int py_exit" + sid + " = 0; bool py_exited" + sid + " = false; py::error py_err" + sid + "{py::None};\n";
    Region r;
    r.id = id;
    fn_->regions.push_back(r);
    std::string body = async_with_stmt(w, item + 1, in + "    ");
    Region done = fn_->regions.back();
    fn_->regions.pop_back();
    o += in + "try {\n" + body + in + "} catch (py::error& e" + sid + ") {\n";
    o += in + "    py_exited" + sid + " = true;\n";
    o += in + "    py_err" + sid + " = e" + sid + ";\n";
    o += in + "}\n";
    o += in + "if (py_exited" + sid + ") {\n";
    o += in + "    py::handling_ py_h" + sid + "(py_err" + sid + ");\n";
    o += in + "    if (!py::truthy_(y.await_(py::with_exit_call(py_exitfn" + sid + ", py_err" + sid + ")))) throw py_err" + sid + ";\n";
    o += in + "}\n";
    o += in + "py_fin" + sid + ":\n";
    o += in + "if (!py_exited" + sid + ") y.await_(py_exitfn" + sid + "(py::None, py::None, py::None));\n";
    o += escape_after(done, in);
    o += ind + "}\n";
    return o;
}

// match: each case's pattern tested on the subject in order; the first that matches (and whose guard
// holds) runs. Names a pattern captures are bound as it matches (as CPython's are).
std::string Emitter::match_stmt(const Match* m, const std::string& ind) {
    std::string sid = t();
    std::string subj = "py_subj" + sid, done = "py_matched" + sid;
    std::string in = ind + "    ";
    std::string o = ind + "{   // " + src_line(m) + "\n";
    o += in + "py::var " + subj + " = " + expr(m->subject) + ";\n";
    o += in + "bool " + done + " = false;\n";
    for (auto* c : m->cases) {
        std::string ok = "py_ok" + t();
        std::string cin = in + "    ";
        std::string casesrc = src_line(c->pattern);
        o += in + "if (!" + done + ") {   // " + (casesrc.empty() ? std::string("case") : casesrc) + "\n";
        o += cin + "bool " + ok + " = true;\n";
        o += pattern(c->pattern, subj, ok, cin);
        std::string test = ok + (c->guard ? " && " + paren(cond(c->guard)) : std::string());
        o += cin + "if (" + test + ") {\n" + cin + "    " + done + " = true;\n" + block(c->body, cin + "    ") + cin + "}\n";
        o += in + "}\n";
    }
    return o + ind + "}\n";
}

// statements that leave `ok` true when `subj` matches p (binding its captures), false when not
std::string Emitter::pattern(const Pattern* p, const std::string& subj, const std::string& ok, const std::string& ind) {
    std::string in = ind + "    ";
    switch (p->kind) {
    case PatternKind::MatchValue:
        return ind + "if (!(" + subj + " == " + operand(cast<MatchValue>(p)->value) + ")) " + ok + " = false;\n";
    case PatternKind::MatchSingleton: {
        auto* s = cast<MatchSingleton>(p);
        std::string v = s->value.kind == ConstValue::Kind::None ? "py::None" : s->value.kind == ConstValue::Kind::True ? "py::var(true)" : "py::var(false)";
        return ind + "if (!py::is(" + subj + ", " + v + ")) " + ok + " = false;\n";
    }
    case PatternKind::MatchAs: {
        auto* a = cast<MatchAs>(p);
        std::string o;
        if (a->pattern) o += pattern(a->pattern, subj, ok, ind);
        if (a->has_name) o += ind + "if (" + ok + ") " + local_or_global(a->name) + " = " + subj + ";\n";
        return o;
    }
    case PatternKind::MatchOr: {
        auto* mo = cast<MatchOr>(p);
        std::string any = "py_any" + t();
        std::string o = ind + "bool " + any + " = false;\n";
        for (auto* alt : mo->patterns) {
            std::string ok2 = "py_ok" + t();
            o += ind + "if (!" + any + ") {\n" + in + "bool " + ok2 + " = true;\n" + pattern(alt, subj, ok2, in) + in + "if (" + ok2 + ") " + any + " = true;\n" + ind + "}\n";
        }
        return o + ind + "if (!" + any + ") " + ok + " = false;\n";
    }
    case PatternKind::MatchSequence: {
        auto* sq = cast<MatchSequence>(p);
        size_t star = sq->patterns.size();
        for (size_t i = 0; i < sq->patterns.size(); i++)
            if (sq->patterns[i]->kind == PatternKind::MatchStar) star = i;
        bool has_star = star < sq->patterns.size();
        size_t before = has_star ? star : sq->patterns.size(), after = has_star ? sq->patterns.size() - star - 1 : 0;
        std::string items = "py_seq" + t();
        // the items as a list ([before..., [starred], after...]), or None: not a sequence of that length
        std::string o = ind + "py::var " + items + " = py::match_sequence_(" + subj + ", " + std::to_string(before) + ", " + std::to_string(after) + ", " +
                        (has_star ? "true" : "false") + ");\n";
        o += ind + "if (" + items + ".is_none()) " + ok + " = false;\n";
        for (size_t i = 0; i < sq->patterns.size(); i++) {
            const Pattern* sub = sq->patterns[i];
            std::string item = items + ".getitem(py::var(" + std::to_string(i) + "))";
            if (sub->kind == PatternKind::MatchStar) {
                auto* st = cast<MatchStar>(sub);
                if (st->has_name) o += ind + "if (" + ok + ") " + local_or_global(st->name) + " = " + item + ";\n";
                continue;
            }
            std::string iv = "py_el" + t();
            o += ind + "if (" + ok + ") {\n" + in + "py::var " + iv + " = " + item + ";\n" + pattern(sub, iv, ok, in) + ind + "}\n";
        }
        return o;
    }
    case PatternKind::MatchMapping: {
        auto* mm = cast<MatchMapping>(p);
        std::string vals = "py_map" + t();
        std::string keys;
        for (auto* k : mm->keys) keys += (keys.empty() ? "" : ", ") + expr(k);
        // the values of the keys in order (and the rest as a dict), or None: not a mapping holding them all
        std::string o = ind + "py::var " + vals + " = py::match_mapping_(" + subj + ", py::tuple({" + keys + "}), " + (mm->has_rest ? "true" : "false") + ");\n";
        o += ind + "if (" + vals + ".is_none()) " + ok + " = false;\n";
        for (size_t i = 0; i < mm->patterns.size(); i++) {
            std::string iv = "py_mv" + t();
            o += ind + "if (" + ok + ") {\n" + in + "py::var " + iv + " = " + vals + ".getitem(py::var(" + std::to_string(i) + "));\n" +
                 pattern(mm->patterns[i], iv, ok, in) + ind + "}\n";
        }
        if (mm->has_rest) o += ind + "if (" + ok + ") " + local_or_global(mm->rest) + " = " + vals + ".getitem(py::var(" + std::to_string(mm->patterns.size()) + "));\n";
        return o;
    }
    case PatternKind::MatchClass: {
        auto* mc = cast<MatchClass>(p);
        std::string vals = "py_cls" + t();
        std::string kw;
        for (auto& k : mc->kwd_attrs) kw += (kw.empty() ? "" : ", ") + slit(k);
        // the positional (by __match_args__) and keyword attributes' values in order, or None: no match
        std::string o = ind + "py::var " + vals + " = py::match_class_(" + subj + ", " + expr(mc->cls) + ", " + std::to_string(mc->patterns.size()) +
                        ", py::tuple({" + kw + "}));\n";
        o += ind + "if (" + vals + ".is_none()) " + ok + " = false;\n";
        size_t n = 0;
        auto sub = [&](const Pattern* sp) {
            std::string iv = "py_ca" + t();
            o += ind + "if (" + ok + ") {\n" + in + "py::var " + iv + " = " + vals + ".getitem(py::var(" + std::to_string(n++) + "));\n" + pattern(sp, iv, ok, in) +
                 ind + "}\n";
        };
        for (auto* sp : mc->patterns) sub(sp);
        for (auto* sp : mc->kwd_patterns) sub(sp);
        return o;
    }
    case PatternKind::MatchStar: return "";   // (only inside a sequence pattern)
    }
    return "";
}

std::string Emitter::import_stmt(const Stmt* s, const std::string& ind) {
    std::string o;
    if (auto* im = as<Import>(s)) {
        for (auto* a : im->names) {
            if (a->has_asname) o += ind + bind_name(a->asname) + " = py::import(" + cstr(a->name) + ");\n";
            else {
                std::string top = a->name.substr(0, a->name.find('.'));
                o += ind + bind_name(top) + " = py::import" + (a->name.find('.') != std::string::npos ? "_top" : "") + "(" + cstr(a->name) + ");\n";
            }
        }
        return o;
    }
    auto* im = cast<ImportFrom>(s);
    if (im->module == "__future__") return "";
    std::string mod = im->level ? resolve_relative(m_.name, m_.package, im->level, im->module) : im->module;
    std::string mv = "py::import(" + cstr(mod) + ")";
    if (im->names.size() == 1 && im->names[0]->name == "*") {
        if (fn_ && fn_->scope && fn_->scope->kind != Scope::Module) { refuse(s, "import * in a function"); return ""; }
        return ind + "py::import_star(" + mv + ", py_module_);\n";
    }
    if (im->names.size() == 1) {
        auto* a = im->names[0];
        return ind + bind_name(a->has_asname ? a->asname : a->name) + " = py::import_from(" + mv + ", " + cstr(a->name) + ");\n";
    }
    std::string m = "py_m" + t();
    o += ind + "{\n" + ind + "    py::var " + m + " = " + mv + ";\n";
    for (auto* a : im->names) o += ind + "    " + bind_name(a->has_asname ? a->asname : a->name) + " = py::import_from(" + m + ", " + cstr(a->name) + ");\n";
    return o + ind + "}\n";
}

std::string Emitter::run() {
    analyze();
    Fn top;
    top.scope = sc_.module();
    top.file_scope = true;
    fn_ = &top;
    // every name the module binds is a cell of the module
    for (auto& n : sc_.module()->bound) gref(n);
    std::string body;
    size_t start = 0;
    std::string doc;
    if (doc_value(tree_.body, doc)) {
        body += "    " + gref("__doc__") + " = " + slit(doc) + ";\n";
        start = 1;
    }
    for (size_t i = start; i < tree_.body.size(); i++) body += stmt(tree_.body[i], "    ");
    fn_ = nullptr;
    std::string dotted = m_.name;
    std::replace(dotted.begin(), dotted.end(), (char)46, (char)95);
    std::string ns = "pymod_" + ident(dotted);
    std::replace(ns.begin(), ns.end(), '.', '_');
    std::string o;
    o += "// " + m_.name + " -- " + fs::path(m_.file).filename().string() + " translated by pyengine --dynamic (C++ over py::var, native/pyrt).\n";
    o += "// Each function keeps its Python name; the module's statements are py_init_ at the end.\n";
    o += "#include \"py_object.hpp\"\n";
    if (wrote_island_) o += "#include \"pyisl.hpp\"     // the typed islands' boundary (--dyn-typed)\n#include \"islands.hpp\"\n";
    o += "\n";
    o += "namespace " + ns + " {\n\n";
    o += "static void py_init_();\n";
    o += "static py::module_def py_module_(" + cstr(m_.name) + ", py_init_, " + (m_.package ? "true" : "false") + ", " +
         cstr(fs::absolute(m_.file).make_preferred().string()) + ");\n\n";
    o += "// module names\n" + globals_ + "\n";
    if (!statics_.empty()) o += statics_ + "\n";
    if (!protos_.empty()) o += protos_ + "\n";
    o += funcs_;
    o += "// the module's statements, run on first import\n";
    o += "static void py_init_() {\n" + body + "}\n\n";
    o += "}  // namespace " + ns + "\n";
    return o;
}

// ------------------------------------------------------------------------------------------ the run
static void write_if_changed(const fs::path& dst, const std::string& text) {
    std::string old = fs::exists(dst) ? read_file(dst.string()) : "";
    if (old == text) return;
    fs::create_directories(dst.parent_path());
    std::ofstream f(dst, std::ios::binary);
    f << text;
}

int run_dynamic(const DynOptions& o, std::vector<DynModule>& out) {
    std::vector<ModSpec> todo;
    std::set<std::string> seen;
    std::deque<std::string> queue;
    std::map<std::string, std::string> explicit_files;
    std::set<std::string> aliased;   // `alias = path` modules: a dotted one is a module of a package the runtime has in C++
    for (auto& spec : o.modules) {
        auto eq = spec.find('=');
        if (eq != std::string::npos) {
            std::string name = spec.substr(0, eq), file = spec.substr(eq + 1);
            fs::path p = fs::path(file).is_absolute() ? fs::path(file) : fs::path(o.root) / file;
            explicit_files[name] = p.generic_string();
            queue.push_back(name);
        } else queue.push_back(spec);
    }
    // the standard modules whose Python is CPython's own (native/pyrt/stdlib.txt), translated from its Lib
    // folder with the program: `name` one module, `name.*` a package and every module under it
    std::set<std::string> from_stdlib;
    for (auto& [lib_dir, list_name] : std::vector<std::pair<std::string, std::string>>{{o.stdlib_dir, "stdlib.txt"}, {o.site_dir, "site.txt"}}) {
        if (lib_dir.empty()) continue;   // (site.txt: outside packages taken the same way from site-packages: unicorn, capstone)
        std::string pyrt_dir = o.pyrt.empty() ? (fs::path(o.root) / "native" / "pyrt").string() : o.pyrt;
        std::string list = read_file((fs::path(pyrt_dir) / list_name).string());
        size_t st = 0;
        while (st < list.size()) {
            size_t e = list.find('\n', st);
            if (e == std::string::npos) e = list.size();
            std::string ln = list.substr(st, e - st);
            st = e + 1;
            if (auto h = ln.find('#'); h != std::string::npos) ln.resize(h);
            while (!ln.empty() && isspace((unsigned char)ln.back())) ln.pop_back();
            while (!ln.empty() && isspace((unsigned char)ln.front())) ln.erase(ln.begin());
            if (ln.empty()) continue;
            std::vector<std::string> names;
            if (auto eq = ln.find('='); eq != std::string::npos) {
                // `alias = path`: a module of Lib under another name (_pycollections = collections/__init__.py: the
                // Python half of a module the runtime has in C++)
                std::string alias = ln.substr(0, eq), path = ln.substr(eq + 1);
                while (!alias.empty() && isspace((unsigned char)alias.back())) alias.pop_back();
                while (!path.empty() && isspace((unsigned char)path.front())) path.erase(path.begin());
                fs::path f = fs::path(lib_dir) / path;
                if (!fs::exists(f)) {
                    fprintf(stderr, "stdlib.txt: %s is not in %s\n", path.c_str(), lib_dir.c_str());
                    continue;
                }
                explicit_files[alias] = f.generic_string();
                aliased.insert(alias);
                from_stdlib.insert(alias);
                queue.push_back(alias);
                continue;
            }
            if (ln.size() > 2 && ln.compare(ln.size() - 2, 2, ".*") == 0) {
                std::string pkg = ln.substr(0, ln.size() - 2), rel = pkg;
                std::replace(rel.begin(), rel.end(), '.', '/');
                fs::path base = fs::path(lib_dir) / rel;
                std::error_code ec;
                for (auto it = fs::recursive_directory_iterator(base, ec); !ec && it != fs::recursive_directory_iterator(); it.increment(ec)) {
                    if (!it->is_regular_file() || it->path().extension() != ".py") continue;
                    std::string r = fs::relative(it->path(), lib_dir).generic_string();
                    r.resize(r.size() - 3);
                    if (r.size() >= 9 && r.compare(r.size() - 9, 9, "/__init__") == 0) r.resize(r.size() - 9);
                    if (r.find("/test") != std::string::npos) continue;
                    std::replace(r.begin(), r.end(), '/', '.');
                    names.push_back(r);
                }
                std::sort(names.begin(), names.end());
            } else names.push_back(ln);
            for (auto& n : names) {
                ModSpec m;
                if (!find_module(lib_dir, n, m)) {
                    fprintf(stderr, "stdlib.txt: %s is not in %s\n", n.c_str(), lib_dir.c_str());
                    continue;
                }
                explicit_files[n] = m.file;
                from_stdlib.insert(n);
                queue.push_back(n);
            }
        }
    }
    auto excluded = [&](const std::string& n) {
        for (auto& x : o.exclude) if (n == x || n.rfind(x + ".", 0) == 0) return true;
        return false;
    };
    while (!queue.empty()) {
        std::string name = queue.front();
        queue.pop_front();
        if (seen.count(name) || excluded(name)) continue;
        seen.insert(name);
        ModSpec m;
        if (explicit_files.count(name)) m = {name, explicit_files[name], fs::path(explicit_files[name]).filename() == "__init__.py"};
        else if (!find_module(o.root, name, m)) {
            // a parent package of a standard module (xml for xml.dom.minidom): CPython's too
            std::string top = name.substr(0, name.find('.'));
            bool std_parent = false;
            for (auto& s : from_stdlib) if (s.substr(0, s.find('.')) == top) std_parent = true;
            if (!std_parent || !find_module(o.stdlib_dir, name, m)) continue;
            from_stdlib.insert(name);
        }
        // (its packages too -- but not an alias's: re._constants = re/_constants.py is a module of pyrt's own re)
        if (!aliased.count(name))
            for (size_t p = name.find('.'); p != std::string::npos; p = name.find('.', p + 1)) queue.push_back(name.substr(0, p));
        todo.push_back(m);
        if (!o.closure) continue;
        std::string src = read_file(m.file);
        auto r = pyfront::parse(src, m.file);
        if (!r) continue;
        // imports at module level only (top_only), or anywhere (a function's own imports too)
        std::vector<AnyNode> nodes;
        for (const Stmt* s : r.module->body) {
            if (o.top_only) {
                std::function<void(const Stmt*)> add = [&](const Stmt* x) {
                    nodes.push_back(AnyNode(x));
                    if (x->kind == StmtKind::FunctionDef || x->kind == StmtKind::AsyncFunctionDef || x->kind == StmtKind::ClassDef) return;
                    for (auto* l : stmt_lists(x)) for (auto* y : *l) add(y);
                };
                add(s);
            } else
                for (const AnyNode& n : walk(AnyNode(s))) nodes.push_back(n);
        }
        bool own_package = false;
        for (auto& p : o.full_closure) if (m.name == p || m.name.rfind(p + ".", 0) == 0) own_package = true;
        if (own_package && o.top_only) {
            nodes.clear();
            for (const Stmt* s : r.module->body) for (const AnyNode& n : walk(AnyNode(s))) nodes.push_back(n);
        }
        for (const AnyNode& n : nodes) {
                auto* st = n.stmt();
                if (!st) continue;
                ModSpec x;
                if (auto* im = as<Import>(st)) {
                    for (auto* a : im->names)
                        if (find_module(o.root, a->name, x)) queue.push_back(a->name);
                } else if (auto* im = as<ImportFrom>(st)) {
                    std::string mod = im->level ? resolve_relative(m.name, m.package, im->level, im->module) : im->module;
                    if (find_module(o.root, mod, x)) queue.push_back(mod);
                    for (auto* a : im->names)
                        if (find_module(o.root, mod + "." + a->name, x)) queue.push_back(mod + "." + a->name);
                }
            }
    }
    fs::create_directories(o.out);
    // --dyn-typed: the typed path's proven functions, as typed islands (islands.hpp)
    IslandSet islands;
    std::map<std::string, std::string> island_log;
    bool islands_on = !o.typed_dir.empty();
    if (islands_on) {
        std::string err;
        if (!load_islands(o.typed_dir, o.islands_file, islands, err)) {
            fprintf(stderr, "--dyn-typed: %s\n", err.c_str());
            return 1;
        }
    }
    std::string cmake_srcs;
    for (auto& m : todo) {
        DynModule res;
        res.name = m.name;
        res.file = m.file;
        res.package = m.package;
        std::string src = read_file(m.file);
        res.lines = (int)std::count(src.begin(), src.end(), '\n');
        auto r = pyfront::parse(src, m.file);
        if (!r) {
            res.error = "syntax error: " + r.error->msg;
            out.push_back(res);
            continue;
        }
        Emitter em(m, src, *r.module, res);
        if (islands_on) em.use_islands(&islands, &island_log);
        std::string cpp = em.run();
        std::string rel = m.name;
        std::replace(rel.begin(), rel.end(), '.', '/');
        if (m.package) rel += "/__init__";
        if (from_stdlib.count(m.name)) rel = "_stdlib/" + rel;
        rel += ".cpp";
        write_if_changed(fs::path(o.out) / rel, cpp);
        res.cpp = rel;
        cmake_srcs += "  " + rel + "\n";
        out.push_back(res);
    }
    if (!o.main_module.empty()) {
        std::string main = "// the program: python -m " + o.main_module + ", translated (pyengine --dynamic)\n#include \"py_object.hpp\"\n\n"
                           "int main(int argc, char** argv) { return py::run_main(" + cstr(o.main_module) + ", argc, argv); }\n";
        write_if_changed(fs::path(o.out) / "main.cpp", main);
        cmake_srcs += "  main.cpp\n";
    }
    std::string pyrt = fs::absolute(o.pyrt.empty() ? fs::path(o.root) / "native" / "pyrt" : fs::path(o.pyrt)).lexically_normal().generic_string();
    std::string cm = "# the translated program: every module's C++ and main.cpp, over native/pyrt (written by pyengine --dynamic)\n"
                     "cmake_minimum_required(VERSION 3.20)\nproject(pydyn LANGUAGES C CXX)\nset(CMAKE_CXX_STANDARD 17)\n"
                     "if(NOT CMAKE_BUILD_TYPE)\n  set(CMAKE_BUILD_TYPE Release)\nendif()\n"
                     "if(MSVC)\n  add_compile_options(/W1 /permissive- /utf-8 /EHsc /bigobj)\nendif()\n"
                     "add_subdirectory(" + pyrt + " pyrt)\n"
                     "add_executable(" + o.exe + "\n" + cmake_srcs + ")\ntarget_link_libraries(" + o.exe + " PRIVATE pyrt)\n"
                     "# a whole tree is hundreds of modules: unity batches over one precompiled py_object.hpp (each module is\n"
                     "# its own namespace, so batching them is safe); -DPYDYN_UNITY=OFF builds one object per module\n"
                     "option(PYDYN_UNITY \"unity batches\" ON)\nset(PYDYN_BATCH 24 CACHE STRING \"modules per unity batch\")\n"
                     "target_precompile_headers(" + o.exe + " PRIVATE <py_object.hpp>)\n"
                     "if(PYDYN_UNITY)\n  set_target_properties(" + o.exe + " PROPERTIES UNITY_BUILD ON UNITY_BUILD_BATCH_SIZE ${PYDYN_BATCH})\nendif()\n"
                     "if(MSVC)\n  target_link_options(" + o.exe + " PRIVATE /STACK:268435456)\nendif()\n";
    if (islands_on) {
        // only the islands a module's def was written as (a module not in this program, or a def of another shape, is not one)
        for (auto it = islands.emit.begin(); it != islands.emit.end();) {
            auto lg = island_log.find(it->first);
            if (lg != island_log.end() && lg->second.empty()) { ++it; continue; }
            islands.not_islands[it->first] = lg == island_log.end() ? "its module is not in this program" : "the dynamic def: " + lg->second;
            it = islands.emit.erase(it);
        }
        fs::path td = fs::path(o.out) / "_typed";
        write_if_changed(td / "islands.hpp", islands_header(islands));
        write_if_changed(td / "islands.cpp", islands_glue(islands));
        write_if_changed(fs::path(o.out) / "islands.json", islands_report(islands));
        write_if_changed(fs::path(o.out) / "islands.funcs", islands_funcs(islands));
        auto native = [&](const char* rel) { return fs::absolute(fs::path(o.root) / rel).lexically_normal().generic_string(); };
        std::string typed_cpp = (fs::path(islands.typed_dir) / "cpp").generic_string();
        std::set<std::string> typed_mods;
        for (auto& [q, is] : islands.emit) typed_mods.insert(is.modules.begin(), is.modules.end());
        std::string typed_srcs;
        for (auto& m : typed_mods) {
            std::string rel = m;
            std::replace(rel.begin(), rel.end(), '.', '/');
            typed_srcs += "  " + typed_cpp + "/" + rel + ".cpp\n";
        }
        cm += "# typed islands (pyengine --dyn-typed): the typed path's C++ (l2cpp17 over py.hpp) of the modules the islands\n"
              "# reach, in a library of its own, its namespace py renamed pyt (the dynamic runtime's is py); _typed/islands.cpp\n"
              "# is the boundary the wrappers in the modules call (native/pyengine/islands)\n"
              "add_library(pyisl_typed STATIC\n" + typed_srcs + "  _typed/islands.cpp)\n"
              "set_target_properties(pyisl_typed PROPERTIES CXX_STANDARD 17)\n"
              "target_compile_definitions(pyisl_typed PRIVATE py=pyt)\n"
              "target_include_directories(pyisl_typed PRIVATE " + typed_cpp + " " + native("native/ported/include") + " " + native("native/l2/src") +
              " " + native("native/pyengine/islands") + " ${CMAKE_CURRENT_SOURCE_DIR}/_typed)\n"
              "target_precompile_headers(pyisl_typed PRIVATE <py.hpp>)\n"
              "if(PYDYN_UNITY)\n  set_target_properties(pyisl_typed PROPERTIES UNITY_BUILD ON UNITY_BUILD_BATCH_SIZE ${PYDYN_BATCH})\nendif()\n"
              "target_include_directories(" + o.exe + " PRIVATE " + native("native/pyengine/islands") + " ${CMAKE_CURRENT_SOURCE_DIR}/_typed)\n"
              "target_link_libraries(" + o.exe + " PRIVATE pyisl_typed)\n";
        fprintf(stderr, "typed islands: %zu written, %zu held back by the judge's verdicts, %zu translated functions not islands\n",
                islands.emit.size(), islands.held.size(), islands.not_islands.size());
    }
    write_if_changed(fs::path(o.out) / "CMakeLists.txt", cm);
    std::string j = "{\n";
    for (size_t i = 0; i < out.size(); i++) {
        auto& m = out[i];
        j += "  " + cstr(m.name) + ": {\"file\": " + cstr(m.file) + ", \"lines\": " + std::to_string(m.lines) + ", \"functions\": " +
             std::to_string(m.functions) + ", \"error\": " + cstr(m.error) + ", \"refused\": [";
        for (size_t k = 0; k < m.refused.size(); k++)
            j += (k ? ", " : "") + std::string("[") + std::to_string(m.refused[k].line) + ", " + cstr(m.refused[k].what) + "]";
        j += "]}" + std::string(i + 1 < out.size() ? "," : "") + "\n";
    }
    j += "}\n";
    write_if_changed(fs::path(o.out) / "dynamic.json", j);
    return 0;
}

}  // namespace pyengine
