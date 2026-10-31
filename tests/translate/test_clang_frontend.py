# -*- coding: utf-8 -*-
"""The C and C++ front end (codebreaker/translate/frontends/clang): clang's typed AST lowered into L2.

The type-spelling parser and the layouts need nothing installed; the lowering needs clang (skipped
without it)."""
import pytest

from codebreaker.translate import ir, l2
from codebreaker.translate.frontends import clang
from codebreaker.translate.frontends.clang import ctypes

needs_clang = pytest.mark.skipif(not clang.clang(), reason='needs clang')
LLP64 = ctypes.Model.for_triple('x86_64-pc-windows-msvc')


def _parser(names=None):
    return ctypes.Parser(LLP64, lambda name, args, tag: (names or {}).get(name))


# ------------------------------------------------------------------------------------------ type spellings
def test_declarators():
    p = _parser({'Pair': ir.Type('class', 'Pair')})
    assert p.parse('unsigned long long') == ir.Type('u64')
    assert p.parse('long') == ir.Type('i32')                   # LLP64: long is 4 bytes
    assert p.parse('char').info['signed'] is True
    t = p.parse('int (*)[4]')                                  # a pointer to an array of 4 ints
    assert t.kind == 'ptr' and t.elem.kind == 'array' and t.elem.info['count'] == 4
    t = p.parse('int *[4]')                                    # an array of 4 pointers
    assert t.kind == 'array' and t.elem.kind == 'ptr'
    t = ctypes.normalise(p.parse('void (*)(int, ...)'))
    assert t.kind == 'fnptr' and t.info['fn'].info['variadic'] and len(t.info['fn'].info['params']) == 1
    t = p.parse('const struct Pair *const')
    assert t.kind == 'ptr' and t.elem == ir.Type('class', 'Pair')
    t = p.parse('const Pair &')
    assert t.kind == 'ptr' and t.info.get('ref')
    assert p.parse('int (void)').info['params'] == []


def test_layouts_follow_the_target():
    sizes = ctypes.Layouts(LLP64, lambda t: None)
    rec, align = sizes.lay_out('S', [('c', ir.Type('char', signed=True)), ('d', ir.Type('f64')),
                                     ('s', ir.Type('i16'))])
    assert [f['offset'] for f in rec.info['fields']] == [0, 8, 16] and rec.info['size'] == 24 and align == 8
    u, _ = sizes.lay_out('U', [('i', ir.I32), ('b', ir.Type('array', elem=ir.Type('u8'), count=6))], union=True)
    assert u.info['size'] == 8 and all(f['offset'] == 0 for f in u.info['fields'])
    assert sizes.size(ir.Type('ptr', elem=ir.I32, space='flat')) == 8


# ------------------------------------------------------------------------------------------ C
C_SOURCE = r'''
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
typedef struct { char tag; double value; } Item;
enum Color { RED, GREEN = 5, BLUE };
union Bits { uint32_t u; float f; };
static const uint8_t table[6] = { 1, 2, 3 };
int counter;
static int sum(int n, ...) {
    va_list ap; int s = 0;
    va_start(ap, n);
    for (int i = 0; i < n; i++) s += va_arg(ap, int);
    va_end(ap);
    return s;
}
int main(void) {
    Item it = { 'x', 2.5 };
    Item *p = &it;
    unsigned short us = 65535;
    char msg[] = "hi\n";
    int k = 0;
    us++;
    switch (BLUE) { case RED: k = 1; break; case BLUE: case GREEN: k = 2; default: k += 3; }
again:
    if (k < 10) { k += 4; goto again; }
    printf("%c %f %d %u %s %d %d\n", p->tag, it.value, (int)sizeof(Item), us, msg, k, sum(2, table[0], table[5]));
    return 0;
}
'''


@pytest.fixture(scope='module')
def c_unit(tmp_path_factory):
    d = tmp_path_factory.mktemp('clang_c')
    f = d / 'prog.c'
    f.write_text(C_SOURCE, encoding='utf-8')
    return clang.lower_file(str(f))


def _named(unit, cls, name):
    for t in unit.types:
        if isinstance(t, cls) and (getattr(t, 'name', None) == name or (getattr(t, 'sym', None) and t.sym.name == name)):
            return t
    raise AssertionError('%s %s not lowered' % (cls.__name__, name))


@needs_clang
def test_c_declarations(c_unit):
    u = c_unit
    assert u.target['pointer'] == 8 and u.target['long'] == 4 and u.target['language'] == 'c'
    item = [t for t in u.types if isinstance(t, ir.ClassDecl) and t.kind == 'struct'][0]
    assert item.layout.info['size'] == 16 and [f['offset'] for f in item.layout.info['fields']] == [0, 8]
    bits = _named(u, ir.ClassDecl, 'Bits')
    assert bits.kind == 'union' and bits.layout.info['size'] == 4
    blue = _named(u, ir.FieldDecl, 'BLUE')                      # a C enum constant: an int global, its value
    assert blue.init.value == 6 and 'const' in blue.mods
    table = _named(u, ir.FieldDecl, 'table')
    assert table.type.kind == 'array' and table.type.elem == ir.Type('u8') and len(table.init.init) == 3
    s = _named(u, ir.MethodDecl, 'sum')
    assert s.kind == 'function' and s.sym.varargs and 'internal' in s.mods


@needs_clang
def test_c_bodies(c_unit):
    main = _named(c_unit, ir.MethodDecl, 'main')
    nodes = list(ir.walk(main))
    assert not ir.unsupported(main)
    # p->tag is a field of *p; &it an address-of
    assert any(isinstance(n, ir.Select) and isinstance(n.obj, ir.Unary) and n.obj.op == '*' and n.sym.name == 'tag'
               for n in nodes)
    assert any(isinstance(n, ir.Unary) and n.op == '&' and isinstance(n.x, ir.Name) and n.x.sym.name == 'it'
               for n in nodes)
    # C's conversions are explicit: us++ stays u16; the char 'x' is converted for the struct field
    assert any(isinstance(n, ir.Cast) and n.implicit for n in nodes)
    # sizeof is the target's value, its spelling kept
    sz = [n for n in nodes if isinstance(n, ir.Lit) and (n.text or '').startswith('sizeof')]
    assert sz and sz[0].value == 16
    # a string literal's value is its bytes
    lits = [n.value for n in nodes if isinstance(n, ir.Lit) and isinstance(n.value, str)]
    assert 'hi\n' in lits
    # the switch's cases are flattened, a case label a constant
    sw = [n for n in nodes if isinstance(n, ir.Switch)][0]
    assert [len(c.labels) for c in sw.cases] == [1, 2, 0] and sw.cases[2].default
    assert sw.cases[1].labels[0].value == 6
    # goto and its label
    assert any(isinstance(n, ir.Goto) and n.label == 'again' for n in nodes)
    assert any(isinstance(n, ir.Labeled) and n.label == 'again' for n in nodes)
    # varargs: va_arg is the builtin, typed
    body = list(ir.walk(_named(c_unit, ir.MethodDecl, 'sum')))
    assert any(isinstance(n, ir.Call) and n.sym is not None and n.sym.name == '__builtin_va_arg' and n.type == ir.I32
               for n in body)


@needs_clang
def test_c_unit_is_l2(c_unit):
    doc = l2.to_json(c_unit)
    try:
        l2.validate(doc)
    except ImportError:
        pytest.skip('jsonschema not installed')
    back = l2.from_json(doc)
    assert l2.to_json(back) == doc                     # (the new fields -- target, layout, implicit -- survive)


# ------------------------------------------------------------------------------------------ C++
CPP_SOURCE = r'''
#include <cstdio>
namespace geo { struct Vec { int x, y; Vec(int a, int b) : x(a), y(b) {}
    Vec operator+(const Vec &o) const { return Vec(x + o.x, y + o.y); } }; }
class Shape { public: explicit Shape(int id) : id_(id) {} virtual ~Shape() {} virtual int area() const = 0;
    int id() const { return id_; } static int made; protected: int id_; };
int Shape::made = 0;
class Rect : public Shape { int w, h; public: Rect(int i, int w_, int h_) : Shape(i), w(w_), h(h_) { ++made; }
    int area() const override { return w * h; } };
template <typename T> T biggest(T a, T b) { return a > b ? a : b; }
template <typename T, int N> struct Box { T v[N]; T first() const { return v[0]; } };
static void bump(int &r) { r += 1; }
int main() {
    geo::Vec c = geo::Vec(1, 2) + geo::Vec(3, 4);
    Shape *s = new Rect(7, 3, 4);
    int n = 5; bump(n);
    Box<double, 2> bx; bx.v[0] = 2.5;
    std::printf("%d %d %d %d %d %g\n", c.x, s->area(), n, biggest(3, 9), Shape::made, bx.first());
    delete s;
    return 0;
}
'''


@pytest.fixture(scope='module')
def cpp_unit(tmp_path_factory):
    d = tmp_path_factory.mktemp('clang_cpp')
    f = d / 'prog.cpp'
    f.write_text(CPP_SOURCE, encoding='utf-8')
    return clang.lower_file(str(f))


@needs_clang
def test_cpp_classes(cpp_unit):
    u = cpp_unit
    assert not ir.unsupported(u)
    vec = [t for t in u.types if isinstance(t, ir.ClassDecl) and t.qname == 'geo.Vec'][0]
    plus = [m for m in vec.members if isinstance(m, ir.MethodDecl) and m.name == 'operator+'][0]
    assert 'const' in plus.mods and plus.params[0].type.info.get('ref')
    rect = [t for t in u.types if isinstance(t, ir.ClassDecl) and t.name == 'Rect'][0]
    assert rect.extends == ir.Type('class', 'Shape') and rect.layout is None      # (polymorphic: no C layout)
    ctor = [m for m in rect.members if isinstance(m, ir.MethodDecl) and m.kind == 'ctor'][0]
    assert isinstance(ctor.body.stmts[0], ir.CtorCall) and ctor.body.stmts[0].kind == 'super'
    shape = [t for t in u.types if isinstance(t, ir.ClassDecl) and t.name == 'Shape'][0]
    made = [m for m in shape.members if isinstance(m, ir.FieldDecl) and m.sym.name == 'made'][0]
    assert 'static' in made.mods and made.init is not None and made.init.value == 0   # (defined outside the class)
    area = [m for m in shape.members if isinstance(m, ir.MethodDecl) and m.name == 'area'][0]
    assert 'pure' in area.mods and 'virtual' in area.mods
    box = [t for t in u.types if isinstance(t, ir.ClassDecl) and t.qname.startswith('Box<')][0]
    assert box.qname == 'Box<f64, 2>'                            # a template by its instance
    names = [t.name for t in u.types if isinstance(t, ir.MethodDecl)]
    assert names.count('biggest') == 1                           # (one instance: int)


@needs_clang
def test_cpp_bodies(cpp_unit):
    main = [t for t in cpp_unit.types if isinstance(t, ir.MethodDecl) and t.name == 'main'][0]
    nodes = list(ir.walk(main))
    # an operator is a call of the method on its left operand; a reference argument's binding is explicit
    assert any(isinstance(n, ir.Call) and n.sym is not None and n.sym.name == 'operator+' and n.target is not None
               for n in nodes)
    call = [n for n in nodes if isinstance(n, ir.Call) and n.sym is not None and n.sym.name == 'bump'][0]
    assert isinstance(call.args[0], ir.Unary) and call.args[0].op == '&'
    # new and delete; a virtual call on *s
    assert any(isinstance(n, ir.New) and n.memory == 'manual' for n in nodes)
    assert any(isinstance(n, ir.Unary) and n.op == 'delete' for n in nodes)
    assert any(isinstance(n, ir.Call) and n.sym is not None and n.sym.name == 'area' and
               isinstance(n.target, ir.Unary) and n.target.op == '*' for n in nodes)
    # inside a method, a reference parameter's reads are explicit
    bump = [t for t in cpp_unit.types if isinstance(t, ir.MethodDecl) and t.name == 'bump'][0]
    assign = [n for n in ir.walk(bump) if isinstance(n, ir.Assign)][0]
    assert isinstance(assign.target, ir.Unary) and assign.target.op == '*'
