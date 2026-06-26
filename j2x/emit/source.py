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

from .. import typemap
from ..ledger import Category, Severity
from . import naming, ownership
from .statements import translate_body, _resolve_chains, _resolve_call_chains, sub_outside_literals

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
def _static_init(f, cpp_type, ledger, cls, resolver, own_statics):
    init = (f.initializer or '').strip()
    if not init:
        return _default_value(cpp_type)              # forward-declared static -> NULL/0/default
    m = _NEW_ARRAY.match(init)
    if m:                                            # new T[n] -> JArray<elem>(n) (bounds-checked)
        n = m.group(1)
        if ledger is not None:
            ledger.add(cls.name + '.cpp', Category.RISK,
                       'uninit-array fixed: %s(%s) value-initialized + length-tracked (Java zero-fills)'
                       % (cpp_type, n), line=f.line, severity=Severity.MED, source='emit',
                       rule='uninit-array-fixed')
        return '%s(%s)' % (cpp_type, n)
    cm = re.match(r'^new\s+([A-Za-z_]\w*)', init)     # `new ArrayList()` -> value container (default)
    if cm and cm.group(1) in typemap.CONTAINERS:
        return None                                  # default-construct the value container (empty)
    return _static_ref(init, resolver, own_statics, cls.name)  # other initializer: translate refs


def _static_ref(expr, resolver, own_statics=(), current=None):
    """Translate references inside a static initializer expression (these don't go through the
    statement translator): Class.field -> Class::s_field, Class.method(...) -> Class::method(...),
    runtime-holder/enum member -> Holder::member (no s_), a bare own/inherited static -> s_name,
    Java null -> NULL, AND the chained/call-result pointer derefs (Block::s_stone->blockID)."""
    holders = typemap.RUNTIME_STATIC_HOLDERS | typemap.JAVART_STATICS | set(resolver.enums)

    def repl(m):
        cls, mem, paren = m.group(1), m.group(2), m.group(3)
        if cls in holders:
            return '%s::%s%s' % (cls, mem, paren or '')          # holder/enum/method: no s_
        if cls in resolver.known:
            if paren:
                return '%s::%s%s' % (cls, mem, paren)            # static method call
            return '%s::%s' % (cls, naming.static_field_name(mem))   # static field -> s_
        return m.group(0)
    expr = re.sub(r'\b([A-Z]\w*)\.(\w+)(\s*\()?', repl, expr)
    # bare reference to this class's own/inherited static -> s_-prefixed (e.g. an initializer
    # calling setStepSound(soundStoneFootstep) where soundStoneFootstep is a static of the class)
    for nm in own_statics:
        expr = sub_outside_literals(re.compile(r'(?<![\w.:])' + re.escape(nm) + r'\b'),
                                    naming.static_field_name(nm), expr)
    expr = re.sub(r'\bnull\b', 'NULL', expr)
    # static-field / call-result chain derefs (Block::s_stone.blockID -> Block::s_stone->blockID),
    # reusing the statement translator's type-tracking walker (no locals, current class as `this`).
    expr = _resolve_chains(expr, {}, resolver, current)
    expr = _resolve_call_chains(expr, {}, resolver, current)
    return expr


def _array_elem_cpp(jtype, resolver):
    """The C++ element spelling for a T[] field, i.e. the JArray<...> template argument."""
    elem_jt = jtype.element()
    elem = resolver.cpp_type(elem_jt, 'element')
    if resolver.ownership(elem_jt) == 'pointer' and not elem.endswith('*'):
        elem += '*'
    return elem


def _render_statics(cls, resolver, ledger):
    L = []
    # own + inherited static field names, for prefixing bare references in initializers
    own_statics = (set(f.name for f in cls.static_fields())
                   | resolver.members.get(cls.name, {}).get('statics', set()))
    for f in cls.static_fields():
        name = naming.field_name(f)
        ft = resolver.field_type(f.jtype)
        agg = ownership.aggregate_array_body(f)
        if agg is not None:                              # new T[]{a,b,c} -> file-static data + JArray
            elem = _array_elem_cpp(f.jtype, resolver)
            items = [_static_ref(e.strip(), resolver, own_statics, cls.name) for e in agg.split(',') if e.strip()]
            data = '_%s_%s_data' % (cls.name, name)
            L.append('static %s %s[] = { %s };' % (elem, data, ', '.join(items)))
            L.append('%s %s::%s(%s, %d);' % (ft, cls.name, name, data, len(items)))
            if ledger is not None:
                ledger.add(cls.name + '.cpp', Category.RISK,
                           'static array new T[]{...} -> JArray over a file-static data array '
                           '(C++03 has no braced new[]; length tracked)',
                           line=f.line, severity=Severity.LOW, source='emit', rule='static-aggregate-array')
            continue
        init = _static_init(f, ft, ledger, cls, resolver, own_statics)
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
    params = ownership.params_str(m, resolver)
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
    params = ownership.params_str(m, resolver)
    L = ['%s %s::%s(%s)' % (ret, cls.name, m.name, params), '{']
    L.extend('\t' + s for s in translate_body(m, cls, resolver, ledger))
    L.append('}')
    return L


# ---------------------------------------------------------------------------
def _referenced_types(cls, resolver, body_text):
    """All known user-type names this class references (for full #includes in the .cpp)."""
    names = set()

    def add(jt):
        if jt:
            names.add(jt.name)
            for a in jt.args:
                add(a)

    if cls.parent:
        names.add(cls.parent)
    names.update(cls.interfaces)
    for f in list(cls.fields) + list(getattr(cls, 'inherited_fields', ())):
        add(f.jtype)
    for m in cls.methods:
        add(m.return_type)
        for p in m.params:
            add(p.jtype)
    names |= set(re.findall(r'\b([A-Z]\w*)\b', body_text))     # new X / X:: / casts / locals
    # transitive (one level): a .cpp that holds a T reaches T's field and method-return
    # types through chains (`var->tField->m()`, `T::s_x->m()`), so it needs their FULL
    # definitions too -- not just T's. Driven by the resolved member environment, so this
    # follows exactly the chains the statement translator now emits.
    for n in list(names):
        env = resolver.members.get(n)
        if env:
            for jt in env['fields'].values():
                add(jt)
            for jt in env['methods'].values():
                add(jt)
    includable = set(resolver.known) | typemap.RUNTIME_HEADERS
    return sorted((names & includable) - {cls.name})


def render_source(cls, resolver, ledger=None):
    """Returns the .cpp text, or None for interfaces/enums (header-only)."""
    if cls.is_interface or cls.is_enum:
        return None

    body = []
    statics = _render_statics(cls, resolver, ledger)
    if statics:
        body += statics + ['']
    for m in cls.constructors():
        body += _render_ctor(cls, m, resolver, ledger) + ['']
    # synthesized zero-initializing default ctor for a value type that lacks one (so it can be
    # held by value as a field); matches the declaration emitted by header.py
    if (cls.name in resolver.values and cls.constructors()
            and not any(c.arity == 0 for c in cls.constructors())):
        items = _init_list(cls, resolver)
        L2 = ['%s::%s()' % (cls.name, cls.name)]
        if items:
            L2.append('\t: ' + '\n\t, '.join(items))
        L2 += ['{', '}']
        body += L2 + ['']
    body += _render_dtor(cls, ownership.owned_pointers(cls, resolver), ledger) + ['']
    for m in cls.methods:
        if m.is_ctor or m.is_abstract:
            continue
        body += _render_method(cls, m, resolver, ledger) + ['']

    # a .cpp needs the FULL definition of every user type it touches (member access,
    # new/cast/static), not just the forward decls in its own header. Collect from the
    # class's signatures (parent, fields incl. inherited, params, returns) AND capitalized
    # tokens in the body -- the signatures catch types reached only through lowercase vars
    # (e.g. entity->posX, where the type 'Entity' is never spelled in the body).
    used = _referenced_types(cls, resolver, '\n'.join(body))
    L = ['#include "stdafx.h"', '#include "JavaRT.h"', '', _BANNER % cls.name, '',
         '#include "%s.h"' % cls.name]
    L += ['#include "%s.h"' % n for n in used]
    L += ['']
    L += body

    if ledger is not None:
        ledger.add(cls.name + '.cpp', Category.PORTED, 'generated source for %s' % cls.name,
                   source='emit', rule='source')
    return '\n'.join(L).rstrip() + '\n'
