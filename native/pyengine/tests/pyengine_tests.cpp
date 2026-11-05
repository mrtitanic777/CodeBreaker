// pyengine_tests -- the engine's suites: each runs the whole engine on small Python modules and
// checks what it found (types, refusals) and what it wrote (L2).
//
//   pyengine_tests <suite>      scopes inference specialization closures generators variants regex files
//                               dynamic backend stubs async match
#include <cstdio>
#include <cstring>
#include <functional>
#include <map>
#include <string>

#include <filesystem>
#include <fstream>

#include "cpp17.hpp"
#include "dyn.hpp"
#include "json.hpp"
#include "lower.hpp"
#include "pyfront/pyfront.hpp"
#include "scopes.hpp"

using namespace pyengine;

static int failures = 0, checks = 0;
#define CHECK(cond, ...)                                    \
    do {                                                    \
        checks++;                                           \
        if (!(cond)) {                                      \
            failures++;                                     \
            fprintf(stderr, "FAIL %s:%d: ", __FILE__, __LINE__); \
            fprintf(stderr, __VA_ARGS__);                   \
            fprintf(stderr, "\n");                          \
        }                                                   \
    } while (0)

using Srcs = std::map<std::string, std::string>;

// the engine on {module: source}
struct Run {
    Program p;
    TranslateResult tr;
    int rounds = 0;
    Run(const Run&) = delete;
    explicit Run(const Srcs& sources, bool extended = true, const std::string& policy = "strict") {
        p.extended = extended;
        p.int_policy = policy;
        for (auto& [name, src] : sources) {
            std::string err;
            if (!p.add_module(name, name + ".py", src, &err)) fprintf(stderr, "syntax error in %s: %s\n", name.c_str(), err.c_str());
        }
        p.index();
        rounds = p.solve();
        tr = translate(p, policy);
    }
    bool ok(const std::string& q) {
        auto it = tr.lowered.find(q);
        if (it != tr.lowered.end() && it->second.md) return true;
        auto so = p.specs_of.find(q);
        if (so == p.specs_of.end() || so->second.empty()) return false;
        for (Fn* c : so->second)
            if (!tr.lowered[c->qname].md) return false;
        return true;
    }
    std::string why(const std::string& q) {
        auto it = tr.lowered.find(q);
        if (it == tr.lowered.end()) return "(not lowered)";
        return it->second.refused ? it->second.refused->issue.reason() : "";
    }
    std::string l2(const std::string& q) {
        auto it = tr.lowered.find(q);
        return it != tr.lowered.end() && it->second.md ? py::json_dump(to_json(it->second.md)) : "";
    }
    std::string unit(const std::string& m) {
        auto it = tr.modules.find(m);
        return it != tr.modules.end() && it->second.unit ? py::json_dump(to_json(it->second.unit)) : "";
    }
    T param(const std::string& q, const std::string& name) { return p.param_type(p.any_fn(q), name); }
    T ret(const std::string& q) { return p.returns.count(q) ? p.returns[q] : BOTTOM(); }
};

static bool has(const std::string& hay, const std::string& needle) { return hay.find(needle) != std::string::npos; }

// ------------------------------------------------------------------------------------ scopes
static void t_scopes() {
    const char* src = R"PY(
import os
from json import dumps as dj
G = 1
def outer(a, b=G):
    x = a
    total = 0
    def inner(k):
        nonlocal total
        total += k
        return x + k + G
    class K:
        y = x
        def m(self):
            return y
    squares = [i * i for i in range(a) if i != x]
    if (n := len(squares)) > 2:
        pass
    lam = lambda q: q + x
    return inner(b)
def uses_global():
    global G
    G = 2
    return os.sep
)PY";
    auto r = pyfront::parse(src, "s.py");
    CHECK(r.module != nullptr, "parse");
    Scopes sc;
    sc.build(*r.module);
    const Scope* mod = sc.module();
    CHECK(mod->bound.count("outer") && mod->bound.count("G") && mod->imports.count("os") && mod->imports.count("dj"), "module bindings");
    const A::FunctionDef* outer = nullptr;
    const A::FunctionDef* ug = nullptr;
    for (auto* s : r.module->body)
        if (s->kind == pyfront::StmtKind::FunctionDef) {
            auto* f = pyfront::cast<pyfront::FunctionDef>(s);
            (f->name == "outer" ? outer : ug) = f;
        }
    const Scope* so = sc.scope_of(outer);
    CHECK(so && so->kind == Scope::Function, "outer is a function scope");
    CHECK(so->params.count("a") && so->params.count("b"), "parameters");
    CHECK(so->bound.count("x") && so->bound.count("inner") && so->bound.count("K") && so->bound.count("n"), "outer's locals (walrus binds here)");
    CHECK(!so->bound.count("i"), "a comprehension's target is its own");
    CHECK(so->cells.count("x") && so->cells.count("total"), "captured names are cells");
    const Scope* si = nullptr;
    const Scope* sk = nullptr;
    const Scope* sm = nullptr;
    const Scope* sl = nullptr;
    const Scope* scomp = nullptr;
    for (auto& s : sc.all()) {
        if (s->name == "inner") si = s.get();
        if (s->name == "K") sk = s.get();
        if (s->name == "m") sm = s.get();
        if (s->kind == Scope::Lambda) sl = s.get();
        if (s->kind == Scope::Comprehension) scomp = s.get();
    }
    CHECK(si && si->free.count("x") && si->free.count("total") && si->nonlocals.count("total"), "inner's free names");
    CHECK(si && !si->free.count("G"), "a module name is global, not free");
    CHECK(sk && sk->kind == Scope::Class && sk->free.count("x"), "a class body reads the function's x");
    // m reads y: the class scope is skipped, so y is not K's -- unresolved here (Python: NameError)
    CHECK(sm && sc.resolve_in(sm, "y").kind == NameRes::Unresolved, "class scopes are skipped by methods");
    CHECK(sl && sl->free.count("x") && sl->params.count("q"), "the lambda captures x");
    CHECK(scomp && scomp->bound.count("i") && scomp->free.count("x"), "the comprehension binds i and reads x");
    CHECK(sc.resolve_in(si, "G").kind == NameRes::Global, "G from inner: global");
    CHECK(sc.resolve_in(si, "len").kind == NameRes::Builtin, "len: builtin");
    const Scope* sug = sc.scope_of(ug);
    CHECK(sug && sug->globals.count("G") && sc.resolve_in(sug, "G").kind == NameRes::Global, "global statement");
    CHECK(sc.resolve_in(so, "x").kind == NameRes::Local, "x in outer: local");
}

// ------------------------------------------------------------------------------------ inference
static void t_inference() {
    Run r(Srcs{{"pkg.m", R"PY(
import struct

def u16(data, p):
    return struct.unpack_from('<H', data, p)[0]

def header(data: bytes):
    if len(data) < 4:
        return None
    return {'magic': u16(data, 0), 'count': u16(data, 2)}

def total(data: bytes):
    h = header(data)
    if h is None:
        return 0
    return h['magic'] + h['count']

def grow(n):
    x = 1
    while True:
        x = x * n
    return x

def caller():
    return grow(3)
)PY"}});
    CHECK(r.ok("pkg.m.u16"), "u16: %s", r.why("pkg.m.u16").c_str());
    T p = r.param("pkg.m.u16", "data");
    CHECK(p.kind == TK::Bytes, "u16's data is bytes from its caller (%s)", p.repr().c_str());
    T rt = r.ret("pkg.m.u16");
    CHECK(rt.kind == TK::Int && rt.fits_i64() && rt.lo() && *rt.lo() == Big(0) && *rt.hi() == Big(65535), "a u16's interval (%s)", rt.repr().c_str());
    CHECK(r.ok("pkg.m.header") && r.ok("pkg.m.total"), "header / total: %s %s", r.why("pkg.m.header").c_str(), r.why("pkg.m.total").c_str());
    CHECK(has(r.unit("pkg.m"), "\"kind\": \"record\""), "the dict with constant keys is a record");
    CHECK(has(r.l2("pkg.m.total"), "is_none"), "h is None narrows the optional");
    CHECK(!r.ok("pkg.m.grow") && has(r.why("pkg.m.grow"), "int-range"), "an int that grows without bound: %s", r.why("pkg.m.grow").c_str());
    // the old rules and the new agree on what the old rules translate
    Run o(Srcs{{"pkg.m", R"PY(
import struct
def u16(data, p):
    return struct.unpack_from('<H', data, p)[0]
def first(data: bytes):
    return u16(data, 0)
)PY"}}, false);
    Run n(Srcs{{"pkg.m", R"PY(
import struct
def u16(data, p):
    return struct.unpack_from('<H', data, p)[0]
def first(data: bytes):
    return u16(data, 0)
)PY"}}, true);
    CHECK(o.l2("pkg.m.first") == n.l2("pkg.m.first") && !o.l2("pkg.m.first").empty(), "old and new rules write the same L2");
    // checked ints: what strict refuses, checked writes with overflow-checked operations
    Run c(Srcs{{"pkg.m", "def mul(a: int, b: int):\n    return a * b\n"}}, true, "checked");
    CHECK(c.ok("pkg.m.mul") && has(c.l2("pkg.m.mul"), "checked_mul"), "checked policy: %s", c.why("pkg.m.mul").c_str());
    // demand typing: a parameter no caller types takes what its uses demand
    Run d(Srcs{{"pkg.m", R"PY(
import os
def ext(path):
    return os.path.splitext(path)[1]
def is_text(name):
    return name.endswith('.txt')
)PY"}});
    CHECK(d.ok("pkg.m.ext") && d.param("pkg.m.ext", "path").kind == TK::Str, "path demanded as a str: %s", d.why("pkg.m.ext").c_str());
    CHECK(d.ok("pkg.m.is_text"), "name demanded as a str: %s", d.why("pkg.m.is_text").c_str());
}

// ------------------------------------------------------------------------------------ specialization
static void t_specialization() {
    Run r(Srcs{{"pkg.m", R"PY(
def size_of(x):
    return len(x) + 1

def both():
    return size_of('abc') + size_of(b'ab')

def only_str():
    return size_of('z')
)PY"}});
    CHECK(r.p.spec_candidates.count("pkg.m.size_of"), "callers disagree on x: a candidate");
    auto so = r.p.specs_of.find("pkg.m.size_of");
    CHECK(so != r.p.specs_of.end() && so->second.size() == 2, "one copy per argument kind (%zu)", so == r.p.specs_of.end() ? 0 : so->second.size());
    CHECK(r.ok("pkg.m.size_of"), "every copy translates");
    CHECK(r.ok("pkg.m.both") && r.ok("pkg.m.only_str"), "the callers translate: %s / %s", r.why("pkg.m.both").c_str(), r.why("pkg.m.only_str").c_str());
    std::string u = r.unit("pkg.m");
    size_t first = u.find("\"name\": \"size_of\", \"kind\": \"function\"");
    CHECK(first != std::string::npos && u.find("\"name\": \"size_of\", \"kind\": \"function\"", first + 1) != std::string::npos,
          "two overloads named size_of in the unit");
    CHECK(has(u, "py.bytes") && has(u, "py.str"), "one for str, one for bytes");
    // the old rules refuse it
    Run o(Srcs{{"pkg.m", "def size_of(x):\n    return len(x) + 1\ndef both():\n    return size_of('abc') + size_of(b'ab')\n"}}, false);
    CHECK(!o.ok("pkg.m.size_of"), "the old rules refuse the clash");
}

// ------------------------------------------------------------------------------------ closures
static void t_closures() {
    Run r(Srcs{{"pkg.m", R"PY(
def scaled(items, k):
    factor = 3
    def scale(v):
        return v * factor + k
    out = []
    for v in items:
        out.append(scale(v))
    return out

def feed():
    return scaled([1, 2, 3], 2)

def escapes(k):
    def f(v):
        return v + k
    return f

def use_escapes():
    return escapes(1)
)PY"}});
    CHECK(r.ok("pkg.m.scaled"), "a closure over factor and k: %s", r.why("pkg.m.scaled").c_str());
    std::string l = r.l2("pkg.m.scaled");
    CHECK(has(l, "\"node\": \"Lambda\"") && has(l, "\"closure\"") && has(l, "py.fn"), "a capturing lambda in a py.fn local");
    CHECK(has(l, "\"name\": \"factor\", \"owner\": \"pkg.m.scaled\""), "the captured name is the definer's");
    size_t decl = l.find("\"name\": \"factor\""), lam = l.find("\"node\": \"Lambda\"");
    CHECK(decl < lam, "factor is declared before the lambda that captures it");
    CHECK(!r.ok("pkg.m.escapes") && has(r.why("pkg.m.escapes"), "nested function used as a value"), "an escaping closure is refused: %s",
          r.why("pkg.m.escapes").c_str());
    Run o(Srcs{{"pkg.m", "def f(k: int):\n    def g(v):\n        return v + k\n    return g(1)\n"}}, false);
    CHECK(has(o.why("pkg.m.f"), "nested function"), "the old rules refuse nested functions");
}

// ------------------------------------------------------------------------------------ generators
static void t_generators() {
    Run r(Srcs{{"pkg.m", R"PY(
def evens(limit: int):
    i = 0
    while i < 10:
        if i % 2 == 0:
            yield i
        i += 1

def words(text: str):
    for w in text.split(','):
        yield w.strip()

def use():
    out = []
    for e in evens(4):
        out.append(e)
    return out, list(words('a, b'))
)PY"}});
    CHECK(r.ok("pkg.m.evens") && r.ok("pkg.m.words") && r.ok("pkg.m.use"), "generators and their consumer: %s / %s / %s",
          r.why("pkg.m.evens").c_str(), r.why("pkg.m.words").c_str(), r.why("pkg.m.use").c_str());
    std::string u = r.unit("pkg.m");
    CHECK(has(u, "\"name\": \"EvensGenerator\"") && has(u, "py.generator") && has(u, "\"name\": \"next\""), "a generator class with next()");
    CHECK(has(u, "\"node\": \"Goto\", \"label\": \"resume_1\"") && has(u, "\"node\": \"Labeled\", \"label\": \"resume_1\""), "resumes after the yield");
    CHECK(has(u, "\"name\": \"index_1\""), "a loop a resume enters keeps its index in a field");
    CHECK(has(r.l2("pkg.m.evens"), "py.iter"), "the function makes an iterator");
    T rt = r.ret("pkg.m.words");
    CHECK(rt.kind == TK::Iter && rt.args()[0].kind == TK::Str, "words yields str (%s)", rt.repr().c_str());
    Run o(Srcs{{"pkg.m", "def g(n: int):\n    yield n\n"}}, false);
    CHECK(has(o.why("pkg.m.g"), "generator"), "the old rules refuse generators");
}

// ------------------------------------------------------------------------------------ variants
static void t_variants() {
    Run r(Srcs{{"pkg.m", R"PY(
def label(flag: bool):
    v = 3 if flag else 'three'
    if isinstance(v, int):
        return v + 1
    return len(v)

def show(flag: bool):
    v = 3 if flag else 'three'
    return str(v)

def bad(flag: bool):
    v = 3 if flag else 'three'
    return v + 1
)PY"}});
    CHECK(r.ok("pkg.m.label") && r.ok("pkg.m.show"), "variants narrowed / printed: %s / %s", r.why("pkg.m.label").c_str(), r.why("pkg.m.show").c_str());
    std::string l = r.l2("pkg.m.label");
    CHECK(has(l, "py.variant") && has(l, "variant.is") && has(l, "variant.get"), "tagged by name: variant.is / variant.get");
    CHECK(has(l, "\"typeargs\": [\"int\"]"), "the int tag is named");
    CHECK(has(r.l2("pkg.m.show"), "variant.of"), "each alternative enters as variant.of");
    CHECK(!r.ok("pkg.m.bad"), "arithmetic on an untested variant is refused: %s", r.why("pkg.m.bad").c_str());
    Run o(Srcs{{"pkg.m", "def show(flag: bool):\n    v = 3 if flag else 'three'\n    return str(v)\n"}}, false);
    CHECK(has(o.why("pkg.m.show"), "mixed"), "the old rules refuse the mix: %s", o.why("pkg.m.show").c_str());
}

// ------------------------------------------------------------------------------------ regex
static void t_regex() {
    ReInfo a = re_analyze(R"((\d+)-(\w+)?)", false);
    CHECK(a.ok && a.groups == 2 && !a.optional[1] && a.optional[2], "(\\d+)-(\\w+)? : group 2 may not take part");
    ReInfo b = re_analyze(R"((?P<k>a)|(?P<v>b))", false);
    CHECK(b.ok && b.groups == 2 && b.optional[1] && b.optional[2] && b.names["v"] == 2, "alternatives are optional, names known");
    ReInfo c = re_analyze(R"((?:x(y))*z([a)]+)(?=q(r)))", false);
    CHECK(c.ok && c.groups == 3 && c.optional[1] && !c.optional[2] && !c.optional[3], "quantified non-capturing group; class with ); lookahead");
    ReInfo d = re_analyze("(a) # comment (b)\n(c)", true);
    CHECK(d.ok && d.groups == 2, "verbose: comments are not groups (%d)", d.groups);
    Run r(Srcs{{"pkg.m", R"PY(
import re
_RX = re.compile(r'(\w+)=(\d+)?')

def key(line: str):
    m = re.match(r'(\w+)=(\w+)', line)
    if not m:
        return ''
    return m.group(1).upper()

def num(line: str):
    m = _RX.match(line)
    if m is None:
        return None
    return m.group(2)

def pairs(text: str):
    return re.findall(r'(\w)(\d)', text)

def clean(text: str):
    return re.sub(r'\s+', ' ', text, flags=re.I)
)PY"}});
    CHECK(r.ok("pkg.m.key"), "re.match / group: %s", r.why("pkg.m.key").c_str());
    CHECK(has(r.l2("pkg.m.key"), "\"typeargs\": [\"(\\\\w+)=(\\\\w+)\"]"), "the call carries its pattern");
    CHECK(r.ok("pkg.m.num"), "a compiled pattern's match: %s", r.why("pkg.m.num").c_str());
    T nr = r.ret("pkg.m.num");
    CHECK(nr.kind == TK::Opt && nr.args()[0].kind == TK::Str, "an optional group is Optional[str] (%s)", nr.repr().c_str());
    CHECK(r.ok("pkg.m.pairs") && has(r.unit("pkg.m"), "py.tuple"), "findall with two groups: tuples: %s", r.why("pkg.m.pairs").c_str());
    CHECK(r.ok("pkg.m.clean") && has(r.l2("pkg.m.clean"), "\"kw\""), "re.sub with flags=: %s", r.why("pkg.m.clean").c_str());
    Run o(Srcs{{"pkg.m", "import re\ndef key(line: str):\n    return re.escape(line)\n"}}, false);
    CHECK(has(o.why("pkg.m.key"), "stdlib:re.escape"), "the old rules refuse re");
}

// ------------------------------------------------------------------------------------ files
static void t_files() {
    Run r(Srcs{{"pkg.m", R"PY(
import os

def count_lines(path: str):
    n = 0
    with open(path, encoding='utf-8') as f:
        for line in f:
            if line.strip():
                n += 1
    return n

def save(path: str, text: str):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, 'w', encoding='utf-8') as f:
        f.write(text)

def raw(path: str):
    with open(path, 'rb') as f:
        return f.read()

def home():
    return os.environ.get('HOME', '')
)PY"}});
    CHECK(r.ok("pkg.m.count_lines"), "text read, line iteration: %s", r.why("pkg.m.count_lines").c_str());
    CHECK(r.ok("pkg.m.save") && has(r.l2("pkg.m.save"), "\"open\"") && has(r.l2("pkg.m.save"), "file.write"), "text write: %s",
          r.why("pkg.m.save").c_str());
    CHECK(r.ok("pkg.m.raw"), "binary read (the old row): %s", r.why("pkg.m.raw").c_str());
    CHECK(r.ok("pkg.m.home") && r.ret("pkg.m.home").kind == TK::Str, "os.environ.get with a str default: %s", r.why("pkg.m.home").c_str());
    Run o(Srcs{{"pkg.m", "def w(path: str, t: str):\n    with open(path, 'w') as f:\n        f.write(t)\n"}}, false);
    CHECK(has(o.why("pkg.m.w"), "open"), "the old rules refuse write mode: %s", o.why("pkg.m.w").c_str());
}

// getattr with a constant name; hand-written type hints; module names whose value cannot be written
static void t_dynamic() {
    Run r(Srcs{{"pkg.m", R"PY(
class Box:
    LIMIT = 4
    def __init__(self, n: int):
        self.n = n
    def later(self):
        self.cache = 1

def size(b: Box) -> int:
    return getattr(b, 'n', 0)

def limit(b: Box) -> int:
    return getattr(b, 'LIMIT')

def missing(b: Box) -> int:
    return getattr(b, 'nothing', 7)

def maybe(b: Box) -> int:
    return getattr(b, 'cache', 0)

def use():
    b = Box(3)
    return size(b) + limit(b) + missing(b) + maybe(b)
)PY"}});
    CHECK(r.ok("pkg.m.size") && has(r.l2("pkg.m.size"), "\"Select\""), "set at the top of __init__: the attribute: %s", r.why("pkg.m.size").c_str());
    CHECK(r.ok("pkg.m.limit"), "a class attribute: %s", r.why("pkg.m.limit").c_str());
    CHECK(r.ok("pkg.m.missing") && has(r.l2("pkg.m.missing"), "\"value\": 7"), "never there: the default: %s", r.why("pkg.m.missing").c_str());
    CHECK(!r.ok("pkg.m.maybe") && has(r.why("pkg.m.maybe"), "getattr"), "set only in another method: refused: %s", r.why("pkg.m.maybe").c_str());
    Run o(Srcs{{"pkg.m", "class A:\n    def __init__(self):\n        self.n = 1\n\ndef f(a: A) -> int:\n    return getattr(a, 'n', 0)\n"}}, false);
    CHECK(!o.ok("pkg.m.f"), "the old rules refuse getattr");

    // a hint types a parameter no caller or use pins down
    Srcs hs{{"pkg.m", "def entry(n, name):\n    return n.upper() + name\n"}};
    Run plain(hs);
    CHECK(!plain.ok("pkg.m.entry"), "no hint: refused: %s", plain.why("pkg.m.entry").c_str());
    {
        Program p;
        std::string err;
        p.add_module("pkg.m", "pkg.m.py", hs["pkg.m"], &err);
        p.index();
        p.type_hints["pkg.m.entry"]["n"] = "str";
        p.type_hints["pkg.m.entry"]["name"] = "str";
        p.solve();
        TranslateResult tr = translate(p, "strict");
        auto it = tr.lowered.find("pkg.m.entry");
        CHECK(it != tr.lowered.end() && it->second.md, "hinted: translated: %s",
              it != tr.lowered.end() && it->second.refused ? it->second.refused->issue.reason().c_str() : "");
        T t = p.param_type(p.any_fn("pkg.m.entry"), "n");
        CHECK(t.kind == TK::Str, "the hint is the parameter's type (%s)", t.repr().c_str());
    }

    // a module name whose value calls a refused function is not written, and neither is its reader
    Run g(Srcs{{"pkg.m", "import json\n\ndef load():\n    return json.loads('{}')\n\nTABLE = load()\n\ndef read() -> int:\n    return len(TABLE)\n"}});
    CHECK(!g.ok("pkg.m.read"), "a reader of a refused name: refused: %s", g.why("pkg.m.read").c_str());
    CHECK(!has(g.unit("pkg.m"), "\"name\": \"TABLE\", \"owner\""), "the refused name is not in the unit");
}

// what the extensions carry, written by the C++17 back end (native/l2's l2cpp17)
static void t_backend() {
    Run r(Srcs{{"pkg.m", R"PY(
import re, os, hashlib

def word(s: str) -> str:
    m = re.search(r'(\w+)', s)
    return m.group(1) if m else ''

def where(p: str) -> str:
    return os.path.abspath(p)

def digest(b: bytes) -> str:
    return hashlib.md5(b).hexdigest()

def first_line(p: str) -> str:
    with open(p, encoding='utf-8') as f:
        return f.readline()

def run():
    return word('a b') + where('x') + digest(b'') + first_line('x')
)PY"}});
    for (auto q : {"pkg.m.word", "pkg.m.where", "pkg.m.digest", "pkg.m.first_line"}) CHECK(r.ok(q), "%s: %s", q, r.why(q).c_str());
    py::V doc = py::json_parse(r.unit("pkg.m"));
    doc.set("lang", std::string("python"));
    std::string src;
    try {
        src = cpp17::write_unit(doc, "")[1].str();
    } catch (const std::exception& e) {
        CHECK(false, "the back end declines: %s", e.what());
    }
    CHECK(has(src, "py::re::search(") && has(src, "group(1)"), "re over py::re");
    CHECK(has(src, "py::os::path::abspath(") && has(src, "py::hashlib::md5("), "os.path / hashlib rows");
    CHECK(has(src, "py::open(p, py::str(\"r\"), ") && has(src, "utf-8") && has(src, ".readline_text()"), "open with a keyword in its slot, text reads: %s", src.c_str());
}

// MonkeyType-style .pyi stubs read as parameter hints (--stubs): applied where nothing else types the
// parameter, inference wins where it does, a type the engine cannot model is ignored with a reason
static void t_stubs() {
    Srcs ss{{"pkg.m", R"PY(
class Box:
    pass

def entry(n, name):
    return n.upper() + name

def typed(x):
    return x + 1

def wild(v):
    return v

def boxed(b):
    return b

def use():
    return typed(3)
)PY"}};
    Program p;
    std::string err;
    p.add_module("pkg.m", "pkg.m.py", ss["pkg.m"], &err);
    p.index();
    p.load_stub("pkg.m",
                "from typing import Any, Union\n"
                "from pkg.m import Box\n"
                "def entry(n: str, name: <class 'Union'>[str]) -> str: ...\n"
                "def typed(x: str) -> int: ...\n"
                "def wild(v: Mystery) -> Any: ...\n"
                "def boxed(b: Box) -> Box: ...\n");
    CHECK(p.stub_files == 1 && p.stub_hints.count("pkg.m.entry"), "the stub is read");
    p.solve();
    auto note = [&](const char* fn, const char* param) -> const Program::StubNote* {
        for (auto& n : p.stub_notes)
            if (n.fn == fn && n.param == param) return &n;
        return nullptr;
    };
    auto* a = note("pkg.m.entry", "n");
    CHECK(a && a->status == "applied", "an untyped parameter takes the stub's type (%s)", a ? a->status.c_str() : "no note");
    CHECK(p.param_type(p.any_fn("pkg.m.entry"), "n").kind == TK::Str, "the stub type is the parameter's type");
    auto* u = note("pkg.m.entry", "name");
    CHECK(u && u->status == "applied", "<class 'Union'>[str] is read as Union[str] (%s)", u ? u->status.c_str() : "no note");
    auto* c = note("pkg.m.typed", "x");
    CHECK(c && c->status == "conflict" && p.param_type(p.any_fn("pkg.m.typed"), "x").kind == TK::Int, "inference wins and the conflict is reported");
    auto* w = note("pkg.m.wild", "v");
    CHECK(w && w->status == "ignored" && !w->detail.empty(), "a type the engine cannot model is ignored with a reason");
    auto* b = note("pkg.m.boxed", "b");
    CHECK(b && b->status == "applied" && p.param_type(p.any_fn("pkg.m.boxed"), "b").kind == TK::Inst, "a class named through a stub import (%s)", b ? b->detail.c_str() : "no note");
}

// the dynamic mode on one module: its C++ and its refusals (the module written under the test's folder)
static DynModule dyn_one(const std::string& name, const std::string& src) {
    namespace fs = std::filesystem;
    fs::path root = fs::current_path() / "dyn_tests" / name;
    fs::create_directories(root);
    {
        std::ofstream f(root / (name + ".py"), std::ios::binary);
        f << src;
    }
    DynOptions o;
    o.root = root.string();
    o.out = (root / "out").string();
    o.modules = {name};
    o.pyrt = (root / "pyrt").string();
    std::vector<DynModule> mods;
    run_dynamic(o, mods);
    for (auto& m : mods)
        if (m.name == name) {
            DynModule r = m;   // (cpp: the C++ itself, read back from where it was written)
            std::ifstream f(fs::path(o.out) / m.cpp, std::ios::binary);
            r.cpp.assign(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>());
            return r;
        }
    return DynModule{};
}
static std::string refusals(const DynModule& m) {
    std::string o;
    for (auto& r : m.refused) o += std::to_string(r.line) + ": " + r.what + "; ";
    return o + m.error;
}

// async def / await / async for / async with / async comprehensions: coroutines on the generators' fibers
static void t_async() {
    DynModule m = dyn_one("am", R"PY(
import asyncio

async def leaf(x):
    return x * 2

async def mid(x):
    a = await leaf(x)
    return a + await leaf(a)

async def agen(n):
    for i in range(n):
        await asyncio.sleep(0)
        yield i

async def collect():
    out = []
    async for v in agen(3):
        if v == 5:
            break
        out.append(v)
    else:
        out.append('done')
    return out + [w async for w in agen(2) if w]

class Lock:
    async def __aenter__(self):
        return self
    async def __aexit__(self, *exc):
        return False

async def guarded(lock):
    async with lock as held, Lock():
        return await mid(1)

def plain():
    async def inner():
        return 1
    return inner
)PY");
    CHECK(m.refused.empty() && m.error.empty(), "async code is translated: %s", refusals(m).c_str());
    CHECK(m.cpp.find("py::def_coroutine(\"leaf\"") != std::string::npos, "async def: a coroutine function");
    CHECK(m.cpp.find("py::def_async_generator(\"agen\"") != std::string::npos, "async def with yield: an async generator function");
    CHECK(m.cpp.find("y.await_(leaf(") != std::string::npos || m.cpp.find("y.await_(") != std::string::npos, "await: y.await_");
    CHECK(m.cpp.find("py::aiter_(") != std::string::npos && m.cpp.find("y.async_next(") != std::string::npos, "async for: __aiter__ / __anext__");
    CHECK(m.cpp.find("py::awith_exit_of(") != std::string::npos && m.cpp.find("py::awith_enter(") != std::string::npos &&
              m.cpp.find("py::with_exit_call(") != std::string::npos, "async with: __aenter__ / __aexit__ awaited");
    CHECK(m.cpp.find("py::def_coroutine(\"inner\"") != std::string::npos, "a nested async def is a coroutine closure");
    DynModule bad = dyn_one("am2", "def f(x):\n    return await x\n");
    CHECK(bad.refused.size() == 1 && bad.refused[0].what.find("await outside") != std::string::npos, "await outside an async def is refused: %s",
          refusals(bad).c_str());
}

// match statements: value, singleton, capture, wildcard, sequence (with a star), mapping (with **rest),
// class (positional and keyword), or-patterns, as-patterns and guards
static void t_match() {
    DynModule m = dyn_one("mm", R"PY(
class Point:
    __match_args__ = ('x', 'y')
    def __init__(self, x, y):
        self.x, self.y = x, y

def what(v):
    match v:
        case 0 | 1 as small:
            return 'small %d' % small
        case None:
            return 'none'
        case [first, *rest] if rest:
            return ('seq', first, rest)
        case {'kind': 'pt', **others}:
            return ('map', others)
        case Point(0, y=yy):
            return ('on y', yy)
        case Point(x, y):
            return ('pt', x, y)
        case str() | bytes():
            return 'text'
        case _:
            return 'other'

def loop(xs):
    out = []
    for x in xs:
        match x:
            case 'stop':
                break
            case _:
                out.append(x)
    return out

mode = 'a'
match mode:
    case 'a':
        chosen = 1
    case other:
        chosen = other
)PY");
    CHECK(m.refused.empty() && m.error.empty(), "match is translated: %s", refusals(m).c_str());
    CHECK(m.cpp.find("py::match_sequence_(") != std::string::npos, "a sequence pattern");
    CHECK(m.cpp.find("py::match_mapping_(") != std::string::npos, "a mapping pattern");
    CHECK(m.cpp.find("py::match_class_(") != std::string::npos, "a class pattern");
    CHECK(m.cpp.find("NotImplementedError") == std::string::npos, "nothing left unlowered");
}

int main(int argc, char** argv) {
    std::map<std::string, std::function<void()>> suites = {
        {"async", t_async},       {"match", t_match},
        {"dynamic", t_dynamic},   {"backend", t_backend}, {"stubs", t_stubs},
        {"scopes", t_scopes},     {"inference", t_inference}, {"specialization", t_specialization}, {"closures", t_closures},
        {"generators", t_generators}, {"variants", t_variants}, {"regex", t_regex}, {"files", t_files}};
    std::vector<std::string> run;
    for (int i = 1; i < argc; i++) run.push_back(argv[i]);
    if (run.empty())
        for (auto& [k, _] : suites) run.push_back(k);
    for (auto& name : run) {
        auto it = suites.find(name);
        if (it == suites.end()) {
            fprintf(stderr, "no suite %s\n", name.c_str());
            return 2;
        }
        int before = failures, cb = checks;
        it->second();
        printf("%-15s %d checks, %d failed\n", name.c_str(), checks - cb, failures - before);
    }
    return failures ? 1 : 0;
}
