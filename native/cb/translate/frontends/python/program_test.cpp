// Tests of cb::translate::frontends::python::program (and the syntax-tree helpers it uses, pyast). The expectations were
// taken from the original module (codebreaker/translate/frontends/python/program.py) run on the same sources and
// reviewed; where the C++ answers differently the case says so in a DIVERGES block.
//
// DIVERGES: (1) a crash recorded by `solve` is the error's message ("boom"), not "RuntimeError: boom" (there is no
// exception class name to quote); (2) an import cycle that never reaches a definition resolves to Missing, where the
// Python recursed to a RecursionError; (3) a class whose bases lead back to itself no longer loops forever while
// `index` looks for the exception it derives from; (4) `foreign_base` is the text of the base expression written by
// pyast::describe_expr, which covers names, dotted names, subscripts, calls, tuples and simple constants (any other
// expression as "<Kind>"), where the Python used ast.unparse for all of them; (5) `fixed`, `raises` and `changed`, which
// nothing read or set, are gone.
#include "cb/translate/frontends/python/program.hpp"

#include <format>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace {

namespace pr = cb::translate::frontends::python::program;
namespace pt = cb::translate::frontends::python::pytypes;
namespace pa = cb::translate::frontends::python::pyast;
namespace sx = ::pyfront;
using cb::translate::frontends::python::Wide;
using pr::Ref;
using pt::Type;

constexpr const char* kProgramTestBase = R"PY("""base docs"""
import os
import os.path
import json as js
import mystery.lib as ml
from . import util
from .util import helper, MISSING, Cyc
from os import path
from typing import Optional, List
from ..outer import thing
from .sub import deep

LIMIT = 10
a, b = 1, 2
VERSION: str = "1"
declared: int
obj.attr = 5

class Error(ValueError):
    pass

class Base(object):
    kind = 'base'
    size: int = 4
    unset: int
    def __init__(self, x: int, y=None):
        self.x = x
        self.y = y
    @property
    def double(self):
        return self.x * 2
    @staticmethod
    def make(n: int):
        return Base(n)
    @classmethod
    def build(cls, n):
        return cls(n)

class Child(Base, Error):
    def set(self, z):
        self.z = z
        other.w = 1

class Other(threading.Thread, mystery.Thing, Generic[T], metaclass=Meta):
    pass

def first(data, offset, n=None, *rest, key=None, **extra):
    """doc"""
    return data[offset]

def second(a, b, c=1):
    if a is None:
        return 1
    if not b:
        return 2
    x = a or b
    return struct.unpack_from('<H', a, b)

def third(p, q, r):
    y = 1 if p else 2
    while r:
        pass
    assert q
    return p == None

async def runner():
    pass

if TYPE_CHECKING:
    def conditional():
        pass
try:
    import fastlib
except ImportError:
    def fallback():
        pass
setup()
)PY";

constexpr const char* kProgramTestUtil = R"PY(
from .base import Base, helper_missing
def helper():
    return 1
class Cyc:
    pass
)PY";

constexpr const char* kProgramTestDeep = R"PY(
from .. import util
from ... import top
from .. import base as b2
import pkg.util
)PY";

constexpr const char* kProgramTestInit = R"PY("""package"""
from .base import Base
)PY";

void program_test_load(pr::Program& p) {
    p.add_module("pkg.base", kProgramTestBase);
    p.add_module("pkg.util", kProgramTestUtil);
    p.add_module("pkg.sub.deep", kProgramTestDeep);
    p.add_module("pkg", kProgramTestInit, "x/__init__.py");
    p.index();
}

std::string program_test_ref(const std::optional<Ref>& r) {
    if (!r) return "none";
    std::string word;
    switch (r->kind) {
        case Ref::Kind::Function: word = "fn"; break;
        case Ref::Kind::Class: word = "class"; break;
        case Ref::Kind::Global: word = "global"; break;
        case Ref::Kind::Module: word = "module"; break;
        case Ref::Kind::StdModule: word = "stdmod"; break;
        case Ref::Kind::Extern: word = "extern"; break;
        case Ref::Kind::StdObject: word = "stdobj"; break;
        case Ref::Kind::Missing: word = "missing"; break;
        case Ref::Kind::Member: word = "obj"; break;
    }
    return word + ":" + r->name + (r->member.empty() ? "" : ":" + r->member);
}

std::string program_test_join(const std::vector<std::string>& items) { return cb::text::join(items, ","); }
std::string program_test_join(const pr::StringSet& items) { return cb::text::join(items, ","); }

std::string program_test_resolve(pr::Program& p, const std::string& module, const std::string& name) {
    return program_test_ref(p.resolve_global(*p.modules.at(module), name));
}

const sx::Expr* program_test_assigned(const pr::Module& m, std::size_t index) {
    return sx::as<sx::Assign>(m.tree->body.at(index))->value;
}

Type program_test_int(std::int64_t lo, std::int64_t hi) { return pt::int_range(Wide(lo), Wide(hi)); }

}  // namespace

CB_TEST("translate.program.module_symbols", [] {
    pr::Program p;
    program_test_load(p);
    const pr::Module& base = *p.modules.at("pkg.base");
    CB_CHECK(base.has_docstring());
    CB_CHECK(!base.is_package);
    CB_CHECK(!p.modules.at("pkg.util")->has_docstring());
    CB_CHECK(p.modules.at("pkg")->is_package);
    CB_CHECK(p.modules.at("pkg")->has_docstring());
    // modules keep the order they were added in
    std::vector<std::string> names;
    for (const auto& [name, module] : p.modules) names.push_back(name);
    CB_CHECK_EQ(program_test_join(names), std::string("pkg.base,pkg.util,pkg.sub.deep,pkg"));

    std::vector<std::string> functions;
    for (const auto& [name, fn] : base.functions) functions.push_back(name);
    // functions defined under `if` and `try` are indexed too
    CB_CHECK_EQ(program_test_join(functions), std::string("first,second,third,runner,conditional,fallback"));
    std::vector<std::string> classes;
    for (const auto& [name, cls] : base.classes) classes.push_back(name);
    CB_CHECK_EQ(program_test_join(classes), std::string("Error,Base,Child,Other"));

    // module-level names; an unpacking and an annotation with no value have no single value
    std::vector<std::string> assigns;
    for (const auto& [name, values] : base.assigns) assigns.push_back(name + (values.at(0) != nullptr ? "=" : "?"));
    CB_CHECK_EQ(program_test_join(assigns), std::string("LIMIT=,a?,b?,VERSION=,declared?"));

    // what is left over: the `if`, the `try`, and the call (not the docstring, the imports, the defs or `obj.attr = 5`)
    std::vector<std::string> kinds;
    for (const sx::Stmt* s : base.statements) kinds.push_back(sx::name_of(s->kind));
    CB_CHECK_EQ(program_test_join(kinds), std::string("If,Try,Expr"));

    std::vector<std::string> imports;
    for (const auto& [local, ref] : base.imports) imports.push_back(local + "=" + program_test_ref(ref));
    CB_CHECK_EQ(program_test_join(imports),
                std::string("os=stdmod:os,js=stdmod:json,ml=extern:mystery.lib,util=module:pkg.util,"
                            "helper=obj:pkg.util:helper,MISSING=obj:pkg.util:MISSING,Cyc=obj:pkg.util:Cyc,"
                            "path=extern:os.path,Optional=extern:typing.Optional,List=extern:typing.List,"
                            "thing=extern:.outer.thing,deep=module:pkg.sub.deep,fastlib=extern:fastlib"));
    // a relative import that climbs: `from .. import util` from pkg.sub.deep is pkg.util; `from ... import top` leaves the program
    CB_CHECK_EQ(program_test_ref(p.modules.at("pkg.sub.deep")->imports.at("util")), std::string("module:pkg.util"));
    CB_CHECK_EQ(program_test_ref(p.modules.at("pkg.sub.deep")->imports.at("top")), std::string("extern:top"));
    CB_CHECK_EQ(program_test_ref(p.modules.at("pkg.sub.deep")->imports.at("b2")), std::string("module:pkg.base"));
    CB_CHECK_EQ(program_test_ref(p.modules.at("pkg.sub.deep")->imports.at("pkg")), std::string("module:pkg"));
});

CB_TEST("translate.program.classes", [] {
    pr::Program p;
    program_test_load(p);
    const auto& error = *p.classes.at("pkg.base.Error");
    CB_CHECK_EQ(error.exception.value_or("-"), std::string("ValueError"));
    CB_CHECK_EQ(program_test_join(error.subclasses), std::string("pkg.base.Child"));
    const auto& base = *p.classes.at("pkg.base.Base");
    CB_CHECK(base.bases.empty());   // `object` is no base worth recording
    CB_CHECK(!base.foreign_base);
    std::vector<std::string> methods;
    for (const auto& [name, fn] : base.methods) methods.push_back(name);
    CB_CHECK_EQ(program_test_join(methods), std::string("__init__,double,make,build"));
    std::vector<std::string> attrs;
    for (const auto& [name, value] : base.class_attrs) attrs.push_back(name);
    CB_CHECK_EQ(program_test_join(attrs), std::string("kind,size"));   // `unset: int` has no value
    const auto& child = *p.classes.at("pkg.base.Child");
    CB_CHECK_EQ(program_test_join(child.bases), std::string("pkg.base.Base,pkg.base.Error"));
    // the exception is found through the first base only, which is not one
    CB_CHECK(!child.exception);
    CB_CHECK_EQ(program_test_join(p.mro("pkg.base.Child")), std::string("pkg.base.Child,pkg.base.Base,pkg.base.Error"));
    CB_CHECK_EQ(program_test_join(p.descendants("pkg.base.Base")), std::string("pkg.base.Child"));
    CB_CHECK_EQ(p.common_base("pkg.base.Child", "pkg.base.Base").value_or("-"), std::string("pkg.base.Base"));
    CB_CHECK(!p.common_base("pkg.base.Error", "pkg.base.Base"));
    // a library base is named by its text: the last one that is not the program's wins
    const auto& other = *p.classes.at("pkg.base.Other");
    CB_CHECK_EQ(other.foreign_base.value_or("-"), std::string("Generic[T]"));
    CB_CHECK(other.bases.empty());
    CB_CHECK_EQ(p.classes.at("pkg.util.Cyc")->qname, std::string("pkg.util.Cyc"));
});

CB_TEST("translate.program.functions", [] {
    pr::Program p;
    program_test_load(p);
    const auto describe_fn = [&](const std::string& q) {
        const pr::Fn& f = *p.fns.at(q);
        std::vector<std::string> annotations, defaults;
        for (const auto& [name, node] : f.annotations) annotations.push_back(name);
        for (const auto& [name, node] : f.defaults) defaults.push_back(name);
        return std::format("{} [{}] kw[{}] ann[{}] def[{}] *{} **{} lines={} none[{}] idx[{}] bound[{}] deco[{}]",
                           pr::fn_kind_word(f.kind), program_test_join(f.params), program_test_join(f.kwonly),
                           program_test_join(annotations), program_test_join(defaults), f.varargs.value_or("-"),
                           f.kwargs.value_or("-"), f.lines, program_test_join(f.none_aware),
                           program_test_join(p.index_params.at(q)), program_test_join(f.bound()),
                           program_test_join(f.decorators));
    };
    CB_CHECK_EQ(describe_fn("pkg.base.Base.__init__"),
                std::string("method [self,x,y] kw[] ann[x] def[y] *- **- lines=3 none[y] idx[] bound[x,y] deco[]"));
    CB_CHECK_EQ(describe_fn("pkg.base.Base.double"),
                std::string("property [self] kw[] ann[] def[] *- **- lines=2 none[] idx[] bound[] deco[property]"));
    CB_CHECK_EQ(describe_fn("pkg.base.Base.make"),
                std::string("static [n] kw[] ann[n] def[] *- **- lines=2 none[] idx[] bound[n] deco[staticmethod]"));
    CB_CHECK_EQ(describe_fn("pkg.base.Base.build"),
                std::string("classmethod [cls,n] kw[] ann[] def[] *- **- lines=2 none[] idx[] bound[n] deco[classmethod]"));
    CB_CHECK_EQ(describe_fn("pkg.base.Child.set"),
                std::string("method [self,z] kw[] ann[] def[] *- **- lines=3 none[] idx[] bound[z] deco[]"));
    // a default of None, a keyword-only default, a first-statement subscript by a parameter
    CB_CHECK_EQ(describe_fn("pkg.base.first"),
                std::string("function [data,offset,n] kw[key] ann[] def[n,key] *rest **extra lines=3 none[key,n] idx[offset] "
                            "bound[data,offset,n] deco[]"));
    // tests of a parameter against None, of its truth, `or`
    CB_CHECK_EQ(describe_fn("pkg.base.second"),
                std::string("function [a,b,c] kw[] ann[] def[c] *- **- lines=7 none[a,b] idx[] bound[a,b,c] deco[]"));
    // a conditional expression, a loop test, an assert, a comparison with None
    CB_CHECK_EQ(describe_fn("pkg.base.third"),
                std::string("function [p,q,r] kw[] ann[] def[] *- **- lines=6 none[p,q,r] idx[] bound[p,q,r] deco[]"));
    CB_CHECK_EQ(describe_fn("pkg.base.runner"),
                std::string("async [] kw[] ann[] def[] *- **- lines=2 none[] idx[] bound[] deco[]"));
    CB_CHECK_EQ(describe_fn("pkg.base.conditional"),
                std::string("function [] kw[] ann[] def[] *- **- lines=2 none[] idx[] bound[] deco[]"));
    CB_CHECK(p.fns.at("pkg.base.Base.make")->cls == p.classes.at("pkg.base.Base").get());
    CB_CHECK(p.fns.at("pkg.base.first")->cls == nullptr);
    CB_CHECK(p.fns.at("pkg.util.helper")->module == p.modules.at("pkg.util").get());
});

CB_TEST("translate.program.resolve", [] {
    pr::Program p;
    program_test_load(p);
    const auto r = [&](const std::string& module, const std::string& name) { return program_test_resolve(p, module, name); };
    CB_CHECK_EQ(r("pkg.base", "first"), std::string("fn:pkg.base.first"));
    CB_CHECK_EQ(r("pkg.base", "Base"), std::string("class:pkg.base.Base"));
    CB_CHECK_EQ(r("pkg.base", "LIMIT"), std::string("global:pkg.base:LIMIT"));
    CB_CHECK_EQ(r("pkg.base", "a"), std::string("global:pkg.base:a"));
    CB_CHECK_EQ(r("pkg.base", "declared"), std::string("global:pkg.base:declared"));
    CB_CHECK_EQ(r("pkg.base", "util"), std::string("module:pkg.util"));
    // an object imported from a program module resolves through it
    CB_CHECK_EQ(r("pkg.base", "helper"), std::string("fn:pkg.util.helper"));
    CB_CHECK_EQ(r("pkg.base", "Cyc"), std::string("class:pkg.util.Cyc"));
    CB_CHECK_EQ(r("pkg.base", "MISSING"), std::string("missing:pkg.util:MISSING"));
    CB_CHECK_EQ(r("pkg.base", "path"), std::string("stdobj:os.path"));
    CB_CHECK_EQ(r("pkg.base", "js"), std::string("stdmod:json"));
    CB_CHECK_EQ(r("pkg.base", "os"), std::string("stdmod:os"));
    CB_CHECK_EQ(r("pkg.base", "ml"), std::string("extern:mystery.lib"));
    CB_CHECK_EQ(r("pkg.base", "thing"), std::string("extern:.outer.thing"));
    CB_CHECK_EQ(r("pkg.base", "deep"), std::string("module:pkg.sub.deep"));
    CB_CHECK_EQ(r("pkg.base", "fallback"), std::string("fn:pkg.base.fallback"));
    CB_CHECK_EQ(r("pkg.base", "nothing"), std::string("none"));
    CB_CHECK_EQ(r("pkg.base", "setup"), std::string("none"));
    CB_CHECK_EQ(r("pkg.sub.deep", "b2"), std::string("module:pkg.base"));
    // pkg.util and pkg.base import each other; the names resolve through the cycle to their definitions
    CB_CHECK_EQ(r("pkg.util", "Base"), std::string("class:pkg.base.Base"));
    CB_CHECK_EQ(r("pkg.util", "helper_missing"), std::string("missing:pkg.base:helper_missing"));
    // dotted expressions: `util.helper`, `pkg.util.helper` is not (pkg is imported as the package, which has no `util` name)
    pr::Module& base = *p.modules.at("pkg.base");
    sx::ParseResult parsed = sx::parse("util.helper\nutil.Cyc\nutil.nothing\nos.path\nBase\n");
    std::vector<std::string> results;
    for (const sx::Stmt* s : parsed.module->body) {
        results.push_back(program_test_ref(p.resolve_expr_name(base, sx::as<sx::ExprStmt>(s)->value)));
    }
    CB_CHECK_EQ(program_test_join(results),
                std::string("fn:pkg.util.helper,class:pkg.util.Cyc,none,none,class:pkg.base.Base"));
});

CB_TEST("translate.program.import_cycle", [] {
    // a imports x from b, b imports x from a: neither defines it
    pr::Program p;
    p.add_module("a", "from b import x\n");
    p.add_module("b", "from a import x\n");
    p.index();
    CB_CHECK(p.resolve_global(*p.modules.at("a"), "x")->kind == Ref::Kind::Missing);
    CB_CHECK(p.resolve_global(*p.modules.at("b"), "x")->kind == Ref::Kind::Missing);
});

CB_TEST("translate.program.relative_imports", [] {
    pr::Program p;
    p.add_module("top.a.b.c", "from . import x\nfrom .. import y\nfrom ... import z\nfrom .... import w\nfrom ..q import v\n");
    p.add_module("top.a.b.x", "");
    p.add_module("top.a.y", "");
    p.add_module("top.z", "");
    p.add_module("top.a.b", "from . import x\nfrom .c import t\n", "top/a/b/__init__.py");
    p.add_module("top.a.b.c.t", "");
    p.index();
    const pr::Module& c = *p.modules.at("top.a.b.c");
    CB_CHECK_EQ(program_test_ref(c.imports.at("x")), std::string("module:top.a.b.x"));
    CB_CHECK_EQ(program_test_ref(c.imports.at("y")), std::string("module:top.a.y"));
    CB_CHECK_EQ(program_test_ref(c.imports.at("z")), std::string("module:top.z"));
    // past the top of the package the base is empty: the full name is just the imported name
    CB_CHECK_EQ(program_test_ref(c.imports.at("w")), std::string("extern:w"));
    // `from ..q import v` names top.a.q.v, which the program does not have
    CB_CHECK_EQ(program_test_ref(c.imports.at("v")), std::string("extern:top.a.q.v"));
    // in a package's __init__ one level is the package itself
    const pr::Module& init = *p.modules.at("top.a.b");
    CB_CHECK_EQ(program_test_ref(init.imports.at("x")), std::string("module:top.a.b.x"));
    CB_CHECK_EQ(program_test_ref(init.imports.at("t")), std::string("module:top.a.b.c.t"));
});

CB_TEST("translate.program.attributes", [] {
    pr::Program p;
    program_test_load(p);
    CB_CHECK_EQ(program_test_join(p.assigned_attrs("pkg.base.Base")), std::string("x,y"));
    CB_CHECK_EQ(program_test_join(p.assigned_attrs("pkg.base.Child")), std::string("z"));   // `other.w` is not self
    CB_CHECK_EQ(program_test_join(p.assigned_attrs_any("pkg.base.Base")), std::string("x,y,z"));
    CB_CHECK_EQ(p.attr_owner("pkg.base.Child", "x").value_or("-"), std::string("pkg.base.Base"));
    CB_CHECK_EQ(p.attr_owner("pkg.base.Child", "z").value_or("-"), std::string("pkg.base.Child"));
    CB_CHECK_EQ(p.attr_owner("pkg.base.Base", "kind").value_or("-"), std::string("pkg.base.Base"));
    CB_CHECK(!p.attr_owner("pkg.base.Child", "nope"));
    CB_CHECK(!p.attr_owner("not.a.Class", "x"));
    CB_CHECK_EQ(pt::describe(*p.attr_type("pkg.base.Child", "x")), std::string("bottom"));
    // set only by a subclass: no one type fits a Base
    CB_CHECK_EQ(pt::describe(*p.attr_type("pkg.base.Base", "z")), std::string("top(attribute z only set by a subclass)"));
    CB_CHECK(!p.attr_type("pkg.base.Child", "nope"));
    CB_CHECK(!p.attr_type("pkg.base.Base", "w"));
    // a fact stored, a class-level value joined in, a poisoned attribute
    p.attrs["pkg.base.Base"].insert_or_assign("x", program_test_int(0, 5));
    p.class_attr_types[{"pkg.base.Base", "kind"}] = pt::str();
    CB_CHECK_EQ(pt::describe(*p.attr_type("pkg.base.Child", "x")), std::string("int[0..5]"));
    CB_CHECK_EQ(pt::describe(*p.attr_type("pkg.base.Child", "kind")), std::string("str"));
    CB_CHECK_EQ(pt::describe(p.class_attr_type("pkg.base.Base", "kind")), std::string("str"));
    CB_CHECK_EQ(pt::describe(p.class_attr_type("pkg.base.Base", "size")), std::string("bottom"));
    p.poisoned[{"pkg.base.Base", "x"}] = "pkg.base.Base.reset";
    CB_CHECK_EQ(pt::describe(*p.attr_type("pkg.base.Child", "x")),
                std::string("top(untyped:attribute x is also stored a value of no single type (by reset))"));
    // methods are found along the bases
    CB_CHECK(p.find_method("pkg.base.Child", "double") == p.fns.at("pkg.base.Base.double").get());
    CB_CHECK(p.find_method("pkg.base.Child", "set") == p.fns.at("pkg.base.Child.set").get());
    CB_CHECK(p.find_method("pkg.base.Child", "nope") == nullptr);
});

CB_TEST("translate.program.annotations", [] {
    pr::Program p;
    p.add_module("m", R"PY(
from typing import Optional, List, Dict, Union, Tuple
class K:
    pass
def f(a: int, b: str, c: Optional[int], d: List[int], e: Dict[str, int], f1: 'K', g: K, h: int | None, i: Tuple[int, str], j: Tuple[int, ...], k: Union[int, str], l: 'List[str]', m: 'not valid(', n: bytes, o: float, p: bool, q: list, r: set[int], s: Sequence[bytes], t: Mapping[str], u: foo.K, v: Optional[List[int]], w: int | str, x: None, y: 'int', z: typing.List[bytes]):
    pass
)PY");
    p.index();
    const pr::Fn& f = *p.fns.at("m.f");
    const auto text = [&](const std::string& param) {
        const Type t = p.annotation_type(f, param);
        std::string out = pt::describe(t);
        if (t.site) {
            const pr::Site& s = p.root(*t.site);
            out += std::format(" elem={} keys={} dyn={}", pt::describe(s.elem), pt::describe(s.keys), s.dynamic.value_or("-"));
        }
        return out;
    };
    // (the sites are numbered in the order the annotations are first asked for)
    CB_CHECK_EQ(text("a"), std::string("int[None..None]"));
    CB_CHECK_EQ(text("b"), std::string("str"));
    CB_CHECK_EQ(text("c"), std::string("opt[int[None..None]]"));
    CB_CHECK_EQ(text("d"), std::string("list#1 elem=int[None..None] keys=bottom dyn=-"));
    CB_CHECK_EQ(text("e"), std::string("dict#2 elem=int[None..None] keys=str dyn=annotated as a dict"));
    CB_CHECK_EQ(text("f1"), std::string("inst m.K"));   // a string annotation is the expression it holds
    CB_CHECK_EQ(text("g"), std::string("inst m.K"));
    CB_CHECK_EQ(text("h"), std::string("opt[int[None..None]]"));
    CB_CHECK_EQ(text("i"), std::string("tuple[int[None..None], str]"));
    CB_CHECK_EQ(text("j"), std::string("top(variable-length tuple annotation)"));
    CB_CHECK_EQ(text("k"), std::string("top(int and str mixed)"));
    CB_CHECK_EQ(text("l"), std::string("list#3 elem=str keys=bottom dyn=-"));
    CB_CHECK_EQ(text("m"), std::string("bottom"));   // a string that is not an expression has no type
    CB_CHECK_EQ(text("n"), std::string("bytes"));
    CB_CHECK_EQ(text("o"), std::string("float"));
    CB_CHECK_EQ(text("p"), std::string("bool"));
    CB_CHECK_EQ(text("q"), std::string("list#4 elem=bottom keys=bottom dyn=-"));
    CB_CHECK_EQ(text("r"), std::string("set#5 elem=int[None..None] keys=bottom dyn=-"));
    CB_CHECK_EQ(text("s"), std::string("list#6 elem=bytes keys=bottom dyn=-"));
    CB_CHECK_EQ(text("t"), std::string("dict#7 elem=bottom keys=bottom dyn=annotated as a dict"));
    CB_CHECK_EQ(text("u"), std::string("bottom"));
    CB_CHECK_EQ(pt::describe(p.annotation_type(f, "v")), std::string("opt[list#8]"));
    CB_CHECK_EQ(pt::describe(p.root(*pt::strip_optional(p.annotation_type(f, "v")).site).elem), std::string("int[None..None]"));
    CB_CHECK_EQ(text("w"), std::string("top(int and str mixed)"));
    CB_CHECK_EQ(text("x"), std::string("none"));
    CB_CHECK_EQ(text("y"), std::string("int[None..None]"));
    CB_CHECK_EQ(text("z"), std::string("list#9 elem=bytes keys=bottom dyn=-"));
    // asking again gives the same type (and no new site)
    CB_CHECK_EQ(text("d"), std::string("list#1 elem=int[None..None] keys=bottom dyn=-"));
    CB_CHECK_EQ(p.site_count(), std::size_t{9});
    CB_CHECK_EQ(p.site(1).where->module, std::string("m"));
    CB_CHECK_EQ(p.site(1).where->line, 5u);
    // no annotation, no type
    pr::Program q;
    q.add_module("m", "def g(a):\n    pass\n");
    q.index();
    CB_CHECK_EQ(pt::describe(q.annotation_type(*q.fns.at("m.g"), "a")), std::string("bottom"));
});

CB_TEST("translate.program.param_type", [] {
    pr::Program p;
    p.add_module("m", "def f(a: int, b=1, c: str = 'x', d=None):\n    pass\n");
    p.index();
    const pr::Fn& f = *p.fns.at("m.f");
    // the annotation of an int is the fallback: a call's argument narrows it
    CB_CHECK_EQ(pt::describe(p.param_type(f, "a")), std::string("int[None..None]"));
    p.params["m.f"].insert_or_assign("a", program_test_int(0, 9));
    CB_CHECK_EQ(pt::describe(p.param_type(f, "a")), std::string("int[0..9]"));
    // any other annotation wins over the facts
    p.params["m.f"].insert_or_assign("c", pt::int_range());
    CB_CHECK_EQ(pt::describe(p.param_type(f, "c")), std::string("str"));
    // the default's type is joined in
    p.params["m.f"].insert_or_assign("b", program_test_int(5, 6));
    p.default_types[{"m.f", "b"}] = program_test_int(1, 1);
    CB_CHECK_EQ(pt::describe(p.param_type(f, "b")), std::string("int[1..6]"));
    CB_CHECK_EQ(pt::describe(p.param_type(f, "d")), std::string("bottom"));
    // a parameter that is neither annotated nor passed has no type yet
    CB_CHECK_EQ(pt::describe(p.global_type("m", "nothing")), std::string("bottom"));
    p.globals["m"].insert_or_assign("LIMIT", program_test_int(10, 10));
    CB_CHECK_EQ(pt::describe(p.global_type("m", "LIMIT")), std::string("int[10..10]"));
    p.returns["m.f"] = pt::str();
    CB_CHECK_EQ(pt::describe(p.ret("m.f")), std::string("str"));
    CB_CHECK_EQ(pt::describe(p.ret("m.other")), std::string("bottom"));
});

CB_TEST("translate.program.join", [] {
    pr::Program p;
    p.add_module("m", "def f():\n    pass\nclass K:\n    pass\nclass L(K):\n    pass\nclass M:\n    pass\n");
    p.index();
    const auto J = [&](const Type& a, const Type& b) { return pt::describe(p.join(a, b)); };
    CB_CHECK_EQ(J(program_test_int(0, 5), program_test_int(3, 10)), std::string("int[0..10]"));
    CB_CHECK_EQ(J(pt::none(), program_test_int(0, 1)), std::string("opt[int[0..1]]"));
    CB_CHECK_EQ(J(pt::optional_of(program_test_int(0, 1)), program_test_int(5, 6)), std::string("opt[int[0..6]]"));
    CB_CHECK_EQ(J(pt::optional_of(pt::str()), pt::none()), std::string("opt[str]"));
    CB_CHECK_EQ(J(pt::str(), pt::int_range()), std::string("top(str and int mixed)"));
    CB_CHECK_EQ(J(pt::bytes(), pt::bytearray()), std::string("top(bytes and bytearray mixed)"));
    CB_CHECK_EQ(J(pt::bytes_of(Wide(4)), pt::bytes_of(Wide(8))), std::string("bytes(<=8)"));
    CB_CHECK_EQ(J(pt::bytes_of(Wide(4)), pt::bytes()), std::string("bytes"));
    CB_CHECK_EQ(J(pt::tuple_of({program_test_int(0, 1), pt::str()}), pt::tuple_of({program_test_int(5, 6), pt::str()})),
                std::string("tuple[int[0..6], str]"));
    CB_CHECK_EQ(J(pt::tuple_of({pt::str()}), pt::tuple_of({pt::str(), pt::str()})),
                std::string("top(tuples of 1 and 2 items mixed)"));
    CB_CHECK_EQ(J(pt::tuple_of({pt::str()}), pt::tuple_of({pt::int_range()})), std::string("top(str and int mixed)"));
    CB_CHECK_EQ(J(pt::bottom(), pt::str()), std::string("str"));
    CB_CHECK_EQ(J(pt::str(), pt::bottom()), std::string("str"));
    CB_CHECK_EQ(J(pt::top("x"), pt::str()), std::string("top(x)"));
    CB_CHECK_EQ(J(pt::str(), pt::top("y")), std::string("top(y)"));
    CB_CHECK_EQ(J(pt::boolean(), pt::floating()), std::string("top(bool and float mixed)"));
    const auto inst = [](const char* name) {
        Type t;
        t.kind = pt::Kind::Inst;
        t.name = name;
        return t;
    };
    CB_CHECK_EQ(J(inst("m.K"), inst("m.K")), std::string("inst m.K"));
    CB_CHECK_EQ(J(inst("m.L"), inst("m.K")), std::string("inst m.K"));   // the common base
    CB_CHECK_EQ(J(inst("m.K"), inst("m.M")), std::string("top(instances of m.K and m.M mixed)"));
    Type list;
    list.kind = pt::Kind::List;
    list.site = p.new_site(p.fns.at("m.f")->node, pr::Container::List);
    CB_CHECK_EQ(J(inst("m.K"), list), std::string("top(K and list mixed)"));
    // the join of two containers is one container: their sites are one class from then on
    Type other = list;
    other.site = p.new_site(p.classes.at("m.K")->node, pr::Container::List);
    CB_CHECK_NE(*list.site, *other.site);
    CB_CHECK_EQ(J(list, other), std::string("list#1"));
    CB_CHECK_EQ(p.find(*other.site), *list.site);
});

CB_TEST("translate.program.containers", [] {
    pr::Program p;
    pr::Module& m = p.add_module("m", "a = []\nb = {}\nc = []\nd = set()\ne = {}\nf = []\ng = []\nh = []\ni = []\nj = {}\nk = []\n");
    p.index();
    pr::SiteId S[11];
    const char* names = "abcdefghijk";
    for (int n = 0; n < 11; ++n) {
        const auto kind = names[n] == 'b' || names[n] == 'e' || names[n] == 'j' ? pr::Container::Dict
                          : names[n] == 'd'                                      ? pr::Container::Set
                                                                                 : pr::Container::List;
        S[n] = p.new_site(program_test_assigned(m, static_cast<std::size_t>(n)), kind, pr::Origin{"m", static_cast<std::uint32_t>(n + 1)});
    }
    CB_CHECK_EQ(S[0], 1u);
    CB_CHECK_EQ(S[10], 11u);
    CB_CHECK_EQ(p.new_site(program_test_assigned(m, 0), pr::Container::List), S[0]);   // the same node is the same site
    CB_CHECK_EQ(p.site_count(), std::size_t{11});

    p.grow(pr::Table::Elem, S[0], program_test_int(0, 5));
    p.grow(pr::Table::Elem, S[0], program_test_int(3, 9));
    CB_CHECK_EQ(pt::describe(p.site(S[0]).elem), std::string("int[0..9]"));
    p.grow(pr::Table::Elem, S[2], pt::str());
    // union: the lower site is the root; the elements join (here into a refusal), a kind mismatch makes it dynamic
    CB_CHECK_EQ(p.union_sites(S[0], S[2]), S[0]);
    CB_CHECK_EQ(pt::describe(p.site(S[0]).elem), std::string("top(int and str mixed)"));
    CB_CHECK_EQ(p.find(S[2]), S[0]);
    CB_CHECK_EQ(p.union_sites(S[2], S[0]), S[0]);   // already one
    CB_CHECK_EQ(p.union_sites(S[0], S[1]), S[0]);
    CB_CHECK_EQ(p.site(S[0]).dynamic.value_or("-"), std::string("a list and a dict flow together"));
    CB_CHECK_EQ(p.find(S[1]), S[0]);

    // a record: fields in the order first set, the join of what each is given; the keys every creation sets
    p.grow_field(S[4], "x", program_test_int(1, 1));
    p.grow_field(S[4], "y", pt::str());
    p.grow_field(S[4], "x", program_test_int(7, 7));
    std::vector<std::string> fields;
    for (const auto& [key, type] : p.site(S[4]).fields) fields.push_back(key + ":" + pt::describe(type));
    CB_CHECK_EQ(program_test_join(fields), std::string("x:int[1..7],y:str"));
    CB_CHECK_EQ(pt::describe(p.site(S[4]).elem), std::string("top(int and str mixed)"));
    CB_CHECK_EQ(pt::describe(p.site(S[4]).keys), std::string("str"));
    CB_CHECK(!p.site(S[4]).required);
    p.created(S[4], {"x", "y"});
    CB_CHECK_EQ(program_test_join(*p.site(S[4]).required), std::string("x,y"));
    p.created(S[4], {"x"});
    CB_CHECK_EQ(program_test_join(*p.site(S[4]).required), std::string("x"));
    CB_CHECK(p.is_record(S[4]));
    CB_CHECK(p.is_record(S[9]));
    p.make_dynamic(S[4], "because");
    CB_CHECK(!p.is_record(S[4]));
    p.make_dynamic(S[4], "again");
    CB_CHECK_EQ(p.site(S[4]).dynamic.value_or("-"), std::string("because"));   // the first reason stays
    CB_CHECK(!p.is_record(S[0]));   // a list is not a record

    // names the source gave a container, merged with the sites they flow into
    p.hint(S[5], "x");
    p.hint(S[5], "y");
    p.hint(S[5], "x");
    CB_CHECK_EQ(program_test_join(p.site(S[5]).hints), std::string("x,y"));
    p.hint(S[6], "z");
    p.union_sites(S[5], S[6]);
    CB_CHECK_EQ(program_test_join(p.site(S[5]).hints), std::string("x,y,z"));
    CB_CHECK(p.site(S[6]).hints.empty());

    // an item of no type poisons the site (the first reason stays; an empty reason is "?")
    p.grow(pr::Table::Elem, S[7], pt::top("boom"));
    CB_CHECK_EQ(p.site(S[7]).poisoned.value_or("-"), std::string("boom"));
    CB_CHECK_EQ(pt::describe(p.site(S[7]).elem), std::string("bottom"));
    p.grow(pr::Table::Elem, S[7], pt::top("second"));
    CB_CHECK_EQ(p.site(S[7]).poisoned.value_or("-"), std::string("boom"));
    p.grow(pr::Table::Elem, S[8], pt::top(""));
    CB_CHECK_EQ(p.site(S[8]).poisoned.value_or("-"), std::string("?"));

    // nothing is recorded while a loop runs toward its fixpoint
    {
        pr::Program::SuspendScope quiet(p);
        CB_CHECK(p.suspended());
        p.grow(pr::Table::Elem, S[10], pt::str());
        p.grow_field(S[9], "q", pt::str());
        p.created(S[9], {"q"});
    }
    CB_CHECK(!p.suspended());
    CB_CHECK_EQ(pt::describe(p.site(S[10]).elem), std::string("bottom"));
    CB_CHECK_EQ(p.site(S[9]).fields.size(), std::size_t{1});   // the field is known, its type is not
    CB_CHECK_EQ(pt::describe(p.site(S[9]).fields.at("q")), std::string("bottom"));
    CB_CHECK(!p.site(S[9]).required);
    p.mark_mutated(S[10]);
    CB_CHECK(p.site(S[10]).mutated);

    // canon names a container by its root
    Type c;
    c.kind = pt::Kind::List;
    c.site = S[2];
    CB_CHECK_EQ(pt::describe(p.canon(c)), std::string("list#1"));
    CB_CHECK_EQ(pt::describe(p.canon(pt::tuple_of({c, pt::str()}))), std::string("tuple[list#1, str]"));
    CB_CHECK_EQ(pt::describe(p.canon(pt::str())), std::string("str"));
});

CB_TEST("translate.program.settle", [] {
    pr::Program p;
    const auto settle = [&](const Type& a, const Type& b) { return pt::describe(p.settle(a, b)); };
    // the first rounds take the new fact as it is
    p.round = 2;
    CB_CHECK_EQ(settle(program_test_int(0, 1), program_test_int(0, 2)), std::string("int[0..2]"));
    // later a bound that keeps growing moves out to a threshold
    p.round = 5;
    CB_CHECK_EQ(settle(program_test_int(0, 1), program_test_int(0, 2)), std::string("int[0..255]"));
    CB_CHECK_EQ(settle(program_test_int(0, 1), program_test_int(0, 1)), std::string("int[0..1]"));
    CB_CHECK_EQ(settle(program_test_int(0, 300), program_test_int(0, 301)), std::string("int[0..65535]"));
    CB_CHECK_EQ(settle(program_test_int(-1, 1), program_test_int(-2, 1)), std::string("int[-255..1]"));
    CB_CHECK_EQ(settle(pt::tuple_of({program_test_int(0, 1), pt::str()}), pt::tuple_of({program_test_int(0, 2), pt::str()})),
                std::string("tuple[int[0..255], str]"));
    CB_CHECK_EQ(settle(pt::optional_of(program_test_int(0, 1)), pt::optional_of(program_test_int(0, 2))),
                std::string("opt[int[0..255]]"));
    Type a = program_test_int(0, 1);
    Type b = program_test_int(0, 2);
    for (int i = 0; i < 8; ++i) {
        a = pt::tuple_of({a});
        b = pt::tuple_of({b});
    }
    // a type that keeps nesting deeper is given up
    CB_CHECK_EQ(settle(a, b), std::string("tuple[tuple[tuple[tuple[tuple[tuple[tuple[top(a type that keeps growing)]]]]]]]"));
    CB_CHECK_EQ(settle(pt::str(), pt::int_range()), std::string("int[None..None]"));
});

namespace {

// The inference's part of `solve`, reduced to what decides its rounds: f gives g's parameter a type, g returns it, h returns
// what g returns, k fails.
class ProgramTestEvaluator : public pr::Evaluator {
public:
    std::vector<std::string> runs;

    void run_module(pr::Program& p, pr::Module& m) override {
        runs.push_back("module " + m.name);
        if (m.name == "m" && !p.globals["m"].contains("LIMIT")) {
            p.globals["m"].insert_or_assign("LIMIT", program_test_int(10, 10));
            p.touch(pr::FactKey::globals("m"));
        }
    }
    void run_function(pr::Program& p, pr::Fn& f) override {
        runs.push_back(f.qname);
        if (f.qname == "m.f") {
            p.read(pr::FactKey::returns("m.g"));
            join_into(p, p.params["m.g"], "x", program_test_int(1, 1), pr::FactKey::params("m.g"));
        } else if (f.qname == "m.g") {
            const Type t = p.param_type(f, "x");
            join_return(p, "m.g", t);
        } else if (f.qname == "m.h") {
            join_return(p, "m.h", p.ret("m.g"));
        } else if (f.qname == "m.k") {
            throw std::runtime_error("boom");
        }
    }

private:
    static void join_into(pr::Program& p, cb::OrderedMap<std::string, Type>& table, const std::string& key, const Type& t,
                          const pr::FactKey& fact) {
        const Type* existing = table.find(key);
        const Type old = existing != nullptr ? *existing : pt::bottom();
        Type joined = p.join(old, t);
        if (joined != old) {
            table.insert_or_assign(key, joined);
            p.touch(fact);
        }
    }
    static void join_return(pr::Program& p, const std::string& fn, const Type& t) {
        const auto it = p.returns.find(fn);
        const Type old = it != p.returns.end() ? it->second : pt::bottom();
        Type joined = p.join(old, t);
        if (joined != old) {
            p.returns[fn] = joined;
            p.touch(pr::FactKey::returns(fn));
        }
    }
};

}  // namespace

CB_TEST("translate.program.solve", [] {
    pr::Program p;
    p.add_module("m", "LIMIT = 10\ndef f(): pass\ndef g(x): pass\ndef h(): pass\ndef k(): pass\n");
    p.add_module("n", "def other(): pass\n");
    p.index();
    ProgramTestEvaluator ev;
    std::vector<std::string> log;
    const std::size_t rounds = p.solve(ev, 40, [&](const std::string& line) { log.push_back(line); });
    CB_CHECK_EQ(rounds, std::size_t{2});
    CB_CHECK_EQ(program_test_join(log), std::string("round 0: ran 5, 4 facts changed,round 1: ran 3, 0 facts changed"));
    // the first round runs everything; the second only what read a fact the first changed (a function that crashed is out)
    CB_CHECK_EQ(program_test_join(ev.runs),
                std::string("module m,module n,m.f,m.g,m.h,m.k,n.other,m.f,m.g,m.h"));
    CB_CHECK_EQ(pt::describe(p.params.at("m.g").at("x")), std::string("int[1..1]"));
    CB_CHECK_EQ(pt::describe(p.returns.at("m.g")), std::string("int[1..1]"));
    CB_CHECK_EQ(pt::describe(p.returns.at("m.h")), std::string("int[1..1]"));
    CB_CHECK_EQ(pt::describe(p.globals.at("m").at("LIMIT")), std::string("int[10..10]"));
    CB_CHECK_EQ(p.crashes.size(), std::size_t{1});
    CB_CHECK_EQ(p.crashes.at("m.k"), std::string("boom"));
    // the statement lists of every function are known after a solve
    const pr::Fn& f = *p.fns.at("m.f");
    CB_CHECK(p.block_of(f.node->body.front()) == &f.node->body);
    // a run that never settles stops at the round limit
    class Churn : public pr::Evaluator {
    public:
        void run_module(pr::Program&, pr::Module&) override {}
        void run_function(pr::Program& q, pr::Fn&) override { q.touch(pr::FactKey::class_attrs()); q.read(pr::FactKey::class_attrs()); }
    } churn;
    pr::Program q;
    q.add_module("c", "def f(): pass\n");
    q.index();
    CB_CHECK_EQ(q.solve(churn, 5), std::size_t{5});
    // an error of a module's top level is not swallowed
    class Fail : public pr::Evaluator {
    public:
        void run_module(pr::Program&, pr::Module&) override { throw std::runtime_error("module failed"); }
        void run_function(pr::Program&, pr::Fn&) override {}
    } fail;
    CB_CHECK_THROWS_AS(std::runtime_error, q.solve(fail));
});

CB_TEST("translate.program.add_tree", [] {
    cb::fs::ScopedTempDir dir("cb-program");
    const auto root = dir.path() / "pkg";
    cb::fs::write_text(root / "__init__.py", "\"\"\"package\"\"\"\n");
    cb::fs::write_text(root / "a.py", "def a(): pass\n");
    cb::fs::write_text(root / "notes.txt", "not python\n");
    cb::fs::write_text(root / "__pycache__" / "x.py", "def x(): pass\n");
    cb::fs::write_text(root / "sub" / "__init__.py", "");
    cb::fs::write_text(root / "sub" / "b.py", "import pkg.a\n");
    cb::fs::write_text(root / "sub" / "bad.py", "def (:\n");
    cb::fs::write_text(root / "sub" / "deeper" / "c.py", "class C: pass\n");
    pr::Program p;
    const auto skipped = p.add_tree(root, "pkg");
    std::vector<std::string> names;
    for (const auto& [name, module] : p.modules) names.push_back(name);
    // a folder's files first, then its subfolders, each in name order; __pycache__ and non-Python files are not modules
    CB_CHECK_EQ(program_test_join(names), std::string("pkg,pkg.a,pkg.sub,pkg.sub.b,pkg.sub.deeper.c"));
    CB_CHECK(p.modules.at("pkg")->is_package);
    CB_CHECK(!p.modules.at("pkg.a")->is_package);
    CB_CHECK(p.modules.at("pkg")->has_docstring());
    CB_CHECK(p.modules.at("pkg.a")->path == root / "a.py");
    // a file that does not parse is skipped and reported
    CB_CHECK_EQ(skipped.size(), std::size_t{1});
    CB_CHECK_EQ(skipped.at(0).filename().string(), std::string("bad.py"));
    p.index();
    CB_CHECK(p.fns.contains("pkg.a.a"));
    CB_CHECK(p.classes.contains("pkg.sub.deeper.c.C"));
    CB_CHECK_EQ(program_test_ref(p.modules.at("pkg.sub.b")->imports.at("pkg")), std::string("module:pkg"));
    // a folder that is not there
    pr::Program q;
    CB_CHECK_THROWS_AS(cb::NotFoundError, q.add_tree(dir.path() / "absent", "x"));
});

CB_TEST("translate.program.errors", [] {
    pr::Program p;
    // a module that does not parse: the position is in the error
    try {
        p.add_module("bad", "x = 1\ndef f(:\n    pass\n");
        CB_CHECK(false);
    } catch (const cb::ParseError& e) {
        CB_CHECK_EQ(e.file(), std::string("bad"));
        CB_CHECK_EQ(e.line(), 2u);
    }
    CB_CHECK(p.modules.empty());
    CB_CHECK_THROWS_AS(cb::ParseError, p.add_module("bad", "x ="));
    cb::fs::ScopedTempDir dir("cb-program");
    CB_CHECK_THROWS_AS(cb::NotFoundError, p.add_file("gone", dir.path() / "gone.py"));
    cb::fs::write_text(dir.path() / "bad.py", "class (:\n");
    CB_CHECK_THROWS_AS(cb::ParseError, p.add_file("bad", dir.path() / "bad.py"));
    // a module added again replaces the one of that name, keeping its place
    p.add_module("one", "A = 1\n");
    p.add_module("two", "B = 1\n");
    p.add_module("one", "C = 1\n");
    CB_CHECK_EQ(p.modules.size(), std::size_t{2});
    CB_CHECK(p.modules.at("one")->assigns.empty());   // not yet indexed
    p.index();
    CB_CHECK(p.modules.at("one")->assigns.contains("C"));
    CB_CHECK_EQ(p.modules.entries().front().first, std::string("one"));
    // classes whose bases lead back to themselves are handled
    pr::Program loop;
    loop.add_module("m", "class A(B): pass\nclass B(A): pass\n");
    CB_CHECK_NOTHROW(loop.index());
    CB_CHECK(!loop.classes.at("m.A")->exception);
});

CB_TEST("translate.program.blocks_and_scopes", [] {
    pr::Program p;
    p.add_module("m", R"PY(
def fn(a, b, *rest, k=1, **kw):
    x = a[b](c)[d]
    y = [q[i] for q in z]
    f = lambda t: t[0]
    for i in range(3):
        if i:
            w = g(i)
        else:
            w = h[i]
    try:
        import os.path as osp, sys
        from m import n as nn
    except ValueError as err:
        del gone
    with open(p) as fh, other():
        (u, v), *rest2 = pair
    def inner(zz):
        inside = 1
    class Klass:
        attr = 2
    if (walrus := call()):
        pass
    return x[1:2]
)PY");
    p.index();
    pr::Fn& fn = *p.fns.at("m.fn");
    p.map_blocks(*fn.node);
    const auto accesses = [&](const sx::Stmt* s) {
        std::vector<std::string> out;
        for (const sx::Expr* e : p.access_nodes(s)) out.push_back(pa::describe_expr(e));
        return program_test_join(out);
    };
    const auto& body = fn.node->body;
    // the subscripts and calls of a statement, innermost first, outside lambdas and comprehensions
    CB_CHECK_EQ(accesses(body[0]), std::string("a[b],a[b](c),a[b](c)[d]"));
    CB_CHECK_EQ(accesses(body[1]), std::string(""));
    CB_CHECK_EQ(accesses(body[2]), std::string(""));
    CB_CHECK_EQ(accesses(body[3]), std::string("range(3),g(i),h[i]"));
    CB_CHECK_EQ(accesses(body[5]), std::string("open(p),other()"));
    CB_CHECK_EQ(accesses(body[8]), std::string("call()"));
    CB_CHECK_EQ(accesses(body[9]), std::string("x[1:2]"));
    CB_CHECK(&p.access_nodes(body[0]) == &p.access_nodes(body[0]));   // cached

    // every name the function binds, its parameters (but not *rest or **kw) among them
    CB_CHECK_EQ(program_test_join(p.fn_locals(fn)),
                std::string("Klass,a,b,err,f,fh,gone,i,inner,k,nn,osp,rest2,sys,u,v,w,walrus,x,y"));

    // the statement list each statement is in
    const auto* loop = sx::cast<sx::For>(body[3]);
    CB_CHECK(p.block_of(body[3]) == &body);
    CB_CHECK(p.block_of(loop->body[0]) == &loop->body);
    const auto* cond = sx::as<sx::If>(loop->body[0]);
    CB_CHECK(p.block_of(cond->body[0]) == &cond->body);
    CB_CHECK(p.block_of(cond->orelse[0]) == &cond->orelse);
    const auto* attempt = sx::cast<sx::Try>(body[4]);
    CB_CHECK(p.block_of(attempt->body[0]) == &attempt->body);
    CB_CHECK(p.block_of(attempt->handlers[0]->body[0]) == &attempt->handlers[0]->body);
    CB_CHECK(p.block_of(sx::cast<sx::With>(body[5])->body[0]) == &sx::cast<sx::With>(body[5])->body);
    CB_CHECK(p.block_of(sx::as<sx::ClassDef>(body[7])->body[0]) == &sx::as<sx::ClassDef>(body[7])->body);
    CB_CHECK(p.block_of(nullptr) == nullptr);
});

CB_TEST("translate.pyast.walk_and_text", [] {
    sx::ParseResult parsed = sx::parse("def f(a: int = 1, *r, k, **w):\n    return g(a)[0] + 'x'\n");
    const auto* def = sx::cast<sx::FunctionDef>(parsed.module->body[0]);
    // ast.walk: breadth first, the node first; the arguments node before the body
    std::vector<std::string> kinds;
    for (const pa::AnyNode& n : pa::walk(def)) {
        if (const sx::Stmt* s = n.stmt()) kinds.push_back(sx::name_of(s->kind));
        else if (const sx::Expr* e = n.expr()) kinds.push_back(sx::name_of(e->kind));
        else kinds.push_back("helper");
    }
    CB_CHECK_EQ(program_test_join(kinds),
                std::string("FunctionDef,helper,Return,helper,helper,helper,helper,Constant,BinOp,Name,Subscript,Constant,"
                            "Call,Constant,Name,Name"));
    CB_CHECK(pa::walk(pa::AnyNode()).empty());
    // names, dotted names, subscripts, calls and constants as a message shows them; the rest by kind
    sx::ParseResult texts = sx::parse("a.b.c\nGeneric[T, U]\nf(x, k=1, **kw)\n(1, 'two')\n(1,)\nx + y\n'it''s'\nNone\n...\n3.5\n");
    std::vector<std::string> out;
    for (const sx::Stmt* s : texts.module->body) out.push_back(pa::describe_expr(sx::as<sx::ExprStmt>(s)->value));
    CB_CHECK_EQ(program_test_join(out), std::string("a.b.c,Generic[T, U],f(x, k=1, **kw),(1, 'two'),(1,),<BinOp>,'its',None,...,3.5"));
    CB_CHECK_EQ(pa::describe_expr(nullptr), std::string("None"));
    // constants
    sx::ParseResult consts = sx::parse("'s'\nNone\n...\n1\n");
    const auto expr_at = [&](std::size_t i) { return sx::as<sx::ExprStmt>(consts.module->body[i])->value; };
    CB_CHECK_EQ(pa::string_constant(expr_at(0)).value_or("-"), std::string("s"));
    CB_CHECK(!pa::string_constant(expr_at(3)));
    CB_CHECK(pa::is_none_constant(expr_at(1)));
    CB_CHECK(!pa::is_none_constant(expr_at(0)));
    CB_CHECK(pa::is_ellipsis(expr_at(2)));
    CB_CHECK(!pa::is_ellipsis(nullptr));
});
