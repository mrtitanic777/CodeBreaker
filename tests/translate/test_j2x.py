# -*- coding: utf-8 -*-
"""j2x (the Java runtime in C++ read back as Java's library): the types in a list are lifted like the ones in a slot."""
from codebreaker.translate import ir
from codebreaker.translate.frontends.clang import j2x

STRING = ir.Type('class', 'java.lang.String')


def test_types_in_lists_are_lifted():
    js = ir.Type('class', 'JavaString')
    cls = ir.ClassDecl(name='C', qname='C', kind='class', mods=[], members=[], implements=[js])
    meth = ir.MethodDecl(name='m', kind='method', mods=[], params=[], throws=[js], body=ir.Block(stmts=[]))
    cat = ir.Catch(sym=None, types=[js], body=ir.Block(stmts=[]))
    call = ir.Call(target=None, sym=None, args=[], typeargs=[js])
    cls.members = [meth]
    unit = ir.Unit(path='x.cpp', types=[cls])
    meth.body = ir.Block(stmts=[cat, ir.ExprStmt(expr=call)])
    j2x.lift([unit])
    assert cls.implements == [STRING]
    assert meth.throws == [STRING]
    assert cat.types == [STRING]
    assert call.typeargs == [STRING]
