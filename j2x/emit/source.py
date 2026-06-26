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
from . import naming, ownership
from .statements import translate_body

_BANNER = '// Auto-generated from %s.java\n// Target: Xbox 360 / C++03 / DirectX 9'
_NEW_ARRAY = re.compile(r'^new\s+[\w.]+\s*\[([^\]]+)\]\s*$')   # size may be any expression
_THIS_CALL = re.compile(r'\bthis\s*\(([^;]*)\)\s*;')
_SUPER_CALL = re.compile(r'^super\s*\(([^;]*)\)\s*;')
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


def _inline_delegation(args_str, deleg, cls, resolver, ledger):
    """Replace `this(args);` with the target constructor's body (C++03 has no ctor
    delegation), substituting the target's params with the call args. This makes the
    delegated ctor's registration actually run -- the Block blockID=0 / container-flag fix."""
    args = [a.strip() for a in args_str.split(',') if a.strip()]
    target = next((c for c in cls.constructors()
                   if c is not deleg and len(c.params) == len(args)), None)
    if target is None:
        if ledger is not None:
            ledger.add(cls.name + '.cpp', Category.STUBBED,
                       'constructor delegation this(%s): no target ctor with %d args found -- inline manually'
                       % (args_str, len(args)), line=getattr(deleg, 'line', 0),
                       severity=Severity.HIGH, source='emit', rule='ctor-delegation')
        return ['// PORT: delegation this(%s) -- target ctor not found; inline manually' % args_str,
                'this(%s);' % args_str]
    sub = dict((p.name, args[i]) for i, p in enumerate(target.params))
    rx = re.compile(r'\b(' + '|'.join(re.escape(k) for k in sub) + r')\b') if sub else None
    out = ['// inlined this(%s) -- C++03 lacks ctor delegation' % args_str]
    for line in translate_body(target, cls, resolver):
        if _THIS_CALL.match(line):                 # chained delegation in target (rare) -- skip
            continue
        out.append(rx.sub(lambda mm: sub[mm.group(1)], line) if rx else line)
    if ledger is not None:
        ledger.add(cls.name + '.cpp', Category.RULE,
                   'ctor delegation this(%s) inlined from the %d-arg constructor (registration preserved)'
                   % (args_str, len(args)), line=getattr(deleg, 'line', 0),
                   severity=Severity.LOW, source='emit', rule='ctor-delegation-inlined')
    return out


def _render_ctor(cls, m, resolver, ledger):
    params = ', '.join('%s %s' % (resolver.param_type(p.jtype), p.name) for p in m.params)
    body = translate_body(m, cls, resolver, ledger)

    # extract a leading super(args) into the C++ base-class init (C++ has no super() statement)
    super_init, rest = None, []
    for line in body:
        sm = _SUPER_CALL.match(line)
        if sm and super_init is None and cls.parent:
            super_init = '%s(%s)' % (cls.parent, sm.group(1).strip())
            continue
        rest.append(line)

    items = ([super_init] if super_init else []) + _init_list(cls, resolver)
    L = ['%s::%s(%s)' % (cls.name, cls.name, params)]
    if items:
        L.append('\t: ' + '\n\t, '.join(items))
    L.append('{')
    for line in rest:
        dm = _THIS_CALL.match(line)
        if dm:
            L.extend('\t' + il for il in _inline_delegation(dm.group(1), m, cls, resolver, ledger))
        else:
            L.append('\t' + line)
    L.append('}')
    return L


# ---------------------------------------------------------------------------
# destructor (Rule of Three: free heap allocated in constructors)
# ---------------------------------------------------------------------------
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
    ret = ownership.return_cpp_type(m, resolver)
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

    body = []
    statics = _render_statics(cls, resolver, ledger)
    if statics:
        body += statics + ['']
    for m in cls.constructors():
        body += _render_ctor(cls, m, resolver, ledger) + ['']
    body += _render_dtor(cls, ownership.owned_pointers(cls, resolver), ledger) + ['']
    for m in cls.methods:
        if m.is_ctor or m.is_abstract:
            continue
        body += _render_method(cls, m, resolver, ledger) + ['']

    # body-driven includes: a .cpp needs the FULL definition of every user type it uses
    # (calls/new/cast/static access), not just the forward decls in its own header.
    used = sorted((set(re.findall(r'\b([A-Z]\w*)\b', '\n'.join(body))) & set(resolver.known))
                  - {cls.name} - set(cls.interfaces))
    L = ['#include "stdafx.h"', '', _BANNER % cls.name, '', '#include "%s.h"' % cls.name]
    L += ['#include "%s.h"' % n for n in used]
    L += ['']
    L += body

    if ledger is not None:
        ledger.add(cls.name + '.cpp', Category.PORTED, 'generated source for %s' % cls.name,
                   source='emit', rule='source')
    return '\n'.join(L).rstrip() + '\n'
