# -*- coding: utf-8 -*-
"""rewrite.py -- small tools for passes that change the IR in place (docs/TRANSLATOR_DESIGN.md, passes).

A pass matches a shape in the tree -- by node kinds, symbols and types, never by the text a back end
would print -- and rebuilds it from the nodes it matched plus a few new ones. Language-neutral: what a
pass means belongs to the pass (a project's fix, a platform's), not here.
"""
from codebreaker.translate import ir


def strip(e):
    """The expression under any grouping parentheses."""
    while isinstance(e, ir.Paren):
        e = e.x
    return e


def methods(units):
    """(class, method) for every method, constructor and initialiser block of the units, nested classes
    included."""
    def classes(c):
        yield c
        for m in c.members:
            if isinstance(m, ir.ClassDecl):
                for x in classes(m):
                    yield x
    for u in units:
        for c in u.types:
            for k in classes(c):
                for m in k.members:
                    if isinstance(m, ir.MethodDecl) and m.body is not None:
                        yield k, m


def stmt_lists(node):
    """Every statement list under `node` that statements sit in, in order: a block's, a case's."""
    for n in ir.walk(node):
        if isinstance(n, ir.Block):
            yield n.stmts
        elif isinstance(n, ir.Case):
            yield n.body


def replace(node, fn):
    """Rebuild `node`'s subtree bottom-up: fn(child) returns a replacement or None (keep it). The node
    itself is not replaced (its parent holds it)."""
    for f in node.fields:
        v = getattr(node, f)
        if isinstance(v, ir.Node):
            replace(v, fn)
            r = fn(v)
            if r is not None:
                setattr(node, f, r)
        elif isinstance(v, list):
            for i, x in enumerate(v):
                if isinstance(x, ir.Node):
                    replace(x, fn)
                    r = fn(x)
                    if r is not None:
                        v[i] = r
    return node


def mentions(node, sym):
    """Does `node` refer to the local or parameter `sym` (by name: each use carries its own Sym)?"""
    return any(isinstance(n, ir.Name) and n.sym is not None and n.sym.name == sym.name for n in ir.walk(node))


def is_name(e, sym):
    e = strip(e)
    return isinstance(e, ir.Name) and e.sym is not None and e.sym.name == sym.name


def is_call(e, name, on_field=None):
    """A call of method `name`; with on_field, on `this.<on_field>`."""
    e = strip(e)
    if not (isinstance(e, ir.Call) and e.sym is not None and e.sym.name == name):
        return False
    if on_field is None:
        return True
    t = strip(e.target) if e.target is not None else None
    return isinstance(t, ir.Select) and isinstance(t.obj, ir.This) and t.sym is not None and t.sym.name == on_field


def is_null_test(e, sym, op='!='):
    """`sym != null` (or ==)."""
    e = strip(e)
    return (isinstance(e, ir.Binary) and e.op == op and is_name(e.left, sym)
            and isinstance(strip(e.right), ir.Lit) and strip(e.right).value is None)


# -- building
def lit(value, type_):
    return ir.Lit(value=value, type=type_)


def null():
    return ir.Lit(value=None, type=ir.NULL)


def true():
    return ir.Lit(value=True, type=ir.BOOL)


def name(sym, type_=None):
    return ir.Name(sym=sym, type=type_ if type_ is not None else sym.type)


def binary(op, left, right, type_):
    return ir.Binary(op=op, left=left, right=right, type=type_)


def if_(cond, then, orelse=None):
    return ir.If(cond=cond, then=then if isinstance(then, ir.Block) else ir.Block(stmts=[then]), orelse=orelse)
