# -*- coding: utf-8 -*-
"""
source.py -- render a class's .cpp from IR + a TypeResolver.

This is where the console-crash classes are prevented BY CONSTRUCTION:
  * static/array allocations get value-init `new T[n]()`  (Java zero-fills; C++ doesn't)
  * every constructor zero-inits its instance fields in the member-init list
    (no uninitialized members)
  * a class that allocates heap in a constructor gets a destructor that frees it
    (Rule-of-Three leak fix)
  * `this(...)` constructor delegation is detected and LOUDLY flagged, never
    silently dropped (the Block blockID=0 / BlockContainer flag bugs)

Method-body statement translation is delegated to emit/statements.py (Module 4c).
"""

import re

from ..ledger import Category, Severity
from . import naming
from .statements import translate_body

_BANNER = '// Auto-generated from %s.java\n// Target: Xbox 360 / C++03 / DirectX 9'
_NEW_ARRAY = re.compile(r'^new\s+[\w.]+\s*\[\s*(\w+)\s*\]\s*$')
_CTOR_NEW = re.compile(r'(?:this\.)?(\w+)\s*=\s*new\s+\w[\w.]*\s*(\[)?')
_THIS_CALL = re.compile(r'\bthis\s*\(([^;]*)\)\s*;')
_PRIMITIVE_CPP = {'int', 'short', 'long long', 'char', 'unsigned char',
                  'float', 'double', 'bool', 'long'}


def _default_value(cpp_type):
    """The zero/null literal for a member-init, or None for value types (default ctor)."""
    if cpp_type.endswith('*'):
        return 'NULL'
    if cpp_type in _PRIMITIVE_CPP:
        return '0'
    return None


# ---------------------------------------------------------------------------
# static field definitions
# ---------------------------------------------------------------------------
def _static_init(f, cpp_type, ledger, cls):
    init = (f.initializer or '').strip()
    if not init:
        return _default_value(cpp_type)              # forward-declared static -> NULL/0/default
    m = _NEW_ARRAY.match(init)
    if m:
        n = m.group(1)
        element = cpp_type[:-1] if cpp_type.endswith('*') else cpp_type
        if ledger is not None:
            ledger.add(cls.name + '.cpp', Category.RISK,
                       'uninit-array fixed: new %s[%s]() value-initialized (Java zero-fills)' % (element, n),
                       line=f.line, severity=Severity.MED, source='emit', rule='uninit-array-fixed')
        return 'new %s[%s]()' % (element, n)
    return init                                       # other initializer: passthrough (refined in 4c)


def _render_statics(cls, resolver, ledger):
    L = []
    for f in cls.static_fields():
        ft = resolver.field_type(f.jtype)
        init = _static_init(f, ft, ledger, cls)
        name = naming.field_name(f)
        if init is None:
            L.append('%s %s::%s;' % (ft, cls.name, name))
        else:
            L.append('%s %s::%s = %s;' % (ft, cls.name, name, init))
    return L


# ---------------------------------------------------------------------------
# constructors
# ---------------------------------------------------------------------------
def _init_list(cls, resolver):
    items = []
    for f in cls.instance_fields():
        dv = _default_value(resolver.field_type(f.jtype))
        if dv is not None:
            items.append('%s(%s)' % (naming.field_name(f), dv))
    return items


def _render_ctor(cls, m, resolver, ledger):
    params = ', '.join('%s %s' % (resolver.param_type(p.jtype), p.name) for p in m.params)
    L = ['%s::%s(%s)' % (cls.name, cls.name, params)]
    items = _init_list(cls, resolver)
    if items:
        L.append('\t: ' + '\n\t, '.join(items))
    L.append('{')
    body_text = '\n'.join(t for _l, t in m.body)
    deleg = _THIS_CALL.search(body_text)
    if deleg:
        args = deleg.group(1).strip()
        if ledger is not None:
            ledger.add(cls.name + '.cpp', Category.STUBBED,
                       'constructor delegation this(%s) must inline the target ctor body '
                       '(C++03 has no ctor delegation) -- cf. Block blockID=0 bug' % args,
                       line=m.line, severity=Severity.HIGH, source='emit', rule='ctor-delegation')
        L.append('\t// PORT: delegation this(%s) -- inline target constructor here (Module 4c)' % args)
    L.extend('\t' + s for s in translate_body(m, cls, resolver, ledger))
    L.append('}')
    return L


# ---------------------------------------------------------------------------
# destructor (Rule of Three: free heap allocated in constructors)
# ---------------------------------------------------------------------------
def _owned_pointers(cls):
    """instance fields assigned via `new` in a ctor body -> {name: is_array}."""
    owned = {}
    for m in cls.constructors():
        text = '\n'.join(t for _l, t in m.body)
        for fm in _CTOR_NEW.finditer(text):
            name, is_arr = fm.group(1), bool(fm.group(2))
            fld = cls.field(name)
            if fld and not fld.is_static:
                owned[name] = is_arr or owned.get(name, False)
    return owned


def _render_dtor(cls, owned, ledger):
    L = ['%s::~%s()' % (cls.name, cls.name), '{']
    for name in sorted(owned):
        L.append('\tdelete%s %s;' % ('[]' if owned[name] else '', name))
        L.append('\t%s = NULL;' % name)
    L.append('}')
    if owned and ledger is not None:
        ledger.add(cls.name + '.cpp', Category.RISK,
                   'Rule-of-Three: synthesized destructor frees %d heap-owning field(s): %s'
                   % (len(owned), ', '.join(sorted(owned))),
                   severity=Severity.MED, source='emit', rule='rule-of-three-dtor')
    return L


# ---------------------------------------------------------------------------
# methods
# ---------------------------------------------------------------------------
def _render_method(cls, m, resolver, ledger):
    ret = resolver.return_type(m.return_type) if m.return_type else 'void'
    params = ', '.join('%s %s' % (resolver.param_type(p.jtype), p.name) for p in m.params)
    L = ['%s %s::%s(%s)' % (ret, cls.name, m.name, params), '{']
    L.extend('\t' + s for s in translate_body(m, cls, resolver, ledger))
    L.append('}')
    return L


# ---------------------------------------------------------------------------
def render_source(cls, resolver, ledger=None):
    """Returns the .cpp text, or None for interfaces (header-only)."""
    if cls.is_interface:
        return None

    L = ['#include "stdafx.h"', '', _BANNER % cls.name, '', '#include "%s.h"' % cls.name, '']

    statics = _render_statics(cls, resolver, ledger)
    if statics:
        L += statics + ['']

    for m in cls.constructors():
        L += _render_ctor(cls, m, resolver, ledger) + ['']

    L += _render_dtor(cls, _owned_pointers(cls), ledger) + ['']

    for m in cls.methods:
        if m.is_ctor or m.is_abstract:
            continue
        L += _render_method(cls, m, resolver, ledger) + ['']

    if ledger is not None:
        ledger.add(cls.name + '.cpp', Category.PORTED, 'generated source for %s' % cls.name,
                   source='emit', rule='source')
    return '\n'.join(L).rstrip() + '\n'
