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

from .. import typemap, ir
from ..ledger import Category, Severity
from . import naming, ownership
from .statements import (translate_body, _resolve_chains, _resolve_call_chains,
                         sub_outside_literals, _split_top_commas)

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


def _delegated_base_init(args_str, deleg, cls, resolver):
    """When a ctor delegates `this(args)`, the base class is constructed by the TARGET ctor's
    super(...). C++03 has no delegation, so the delegating ctor must carry that base init itself:
    return `Parent(<target's super args, target-params substituted by the delegation args>)`."""
    if not cls.parent:
        return None
    args = [a for a in _split_top_commas(args_str) if a]   # balanced: nested-comma calls stay one arg
    target = next((c for c in cls.constructors()
                   if c is not deleg and len(c.params) == len(args)), None)
    if target is None:
        return None
    sub = dict((p.name, args[i]) for i, p in enumerate(target.params))
    rx = re.compile(r'\b(' + '|'.join(re.escape(k) for k in sub) + r')\b') if sub else None
    for line in translate_body(target, cls, resolver):
        sm = _SUPER_CALL.match(line)
        if sm:
            sargs = sm.group(1).strip()
            if rx:
                sargs = rx.sub(lambda mm: sub[mm.group(1)], sargs)
            return '%s(%s)' % (cls.parent, sargs)
    return None


def _inline_delegation(args_str, deleg, cls, resolver, ledger):
    """Replace `this(args);` with the target constructor's body (C++03 has no ctor
    delegation), substituting the target's params with the call args. This makes the
    delegated ctor's registration actually run -- the Block blockID=0 / container-flag fix."""
    args = [a for a in _split_top_commas(args_str) if a]   # balanced: nested-comma calls stay one arg
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
        if _SUPER_CALL.match(line):                # target's super() becomes THIS ctor's base init
            continue
        out.append(rx.sub(lambda mm: sub[mm.group(1)], line) if rx else line)
    if ledger is not None:
        ledger.add(cls.name + '.cpp', Category.RULE,
                   'ctor delegation this(%s) inlined from the %d-arg constructor (registration preserved)'
                   % (args_str, len(args)), line=getattr(deleg, 'line', 0),
                   severity=Severity.LOW, source='emit', rule='ctor-delegation-inlined')
    return out


def _translate_block_lines(cls, resolver, ledger, stmt_lines, is_static):
    """Translate raw Java statement lines in cls's context. `stmt_lines` is a list of statement
    strings (no enclosing braces). Returns translated C++ lines."""
    m = ir.Method('_block', ir.JType('void'), params=[], is_static=is_static)
    m.body = ([(0, 'void _block() {')]
              + [(i + 1, s) for i, s in enumerate(stmt_lines)]
              + [(len(stmt_lines) + 1, '}')])
    return translate_body(m, cls, resolver, ledger)


def _strip_block_braces(block_lines):
    """A captured `static {`/`{` ... `}` block -> just its inner statement strings (drop the
    opening-brace line and exactly ONE trailing close brace -- not inner for/catch braces)."""
    inner = [t for _ln, t in block_lines]
    for k, t in enumerate(inner):                  # drop up to & incl. the opening `{` line
        if '{' in t:
            inner = inner[k + 1:]; break
    while inner and not inner[-1].strip():         # trailing blanks
        inner = inner[:-1]
    if inner and inner[-1].strip() == '}':         # the block's OWN close (only one)
        inner = inner[:-1]
    return inner


def _instance_init_lines(cls, resolver, ledger):
    """C++ for Java INSTANCE field initializers + instance `{ }` blocks, in declaration order.
    Java runs these at the top of every constructor that does NOT delegate via this(...). Dropping
    them was the null-deref / uninitialized-field crash class (e.g. EntityPlayer.inventory)."""
    stmts = []
    for f in cls.instance_fields():
        if f.initializer is None:
            continue
        init = f.initializer.strip()
        if f.jtype.is_array and init.startswith('{'):           # `int[]..[] x = {..}` shorthand ->
            init = 'new %s%s%s' % (f.jtype.name, '[]' * f.jtype.array_dims, init)   # new T[]..[]{..}
        stmts.append('this.%s = %s;' % (f.name, init))
    out = _translate_block_lines(cls, resolver, ledger, stmts, is_static=False) if stmts else []
    for block in cls.inst_inits:                                # instance { } blocks (rare)
        out += _translate_block_lines(cls, resolver, ledger, _strip_block_braces(block), is_static=False)
    if out and ledger is not None:
        ledger.add(cls.name + '.cpp', Category.RISK,
                   'instance field initializers / init-blocks emitted into the constructor(s) '
                   '(Java runs these per-ctor; dropping them caused null-deref/uninit fields)',
                   severity=Severity.MED, source='emit', rule='instance-init')
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

    # no own super() but delegates `this(args)`: adopt the target ctor's base init, so a base
    # with no default ctor is still constructed (C++03 delegation gap). Java inserts super()
    # in the target; here the delegating ctor must mirror it.
    if super_init is None and cls.parent:
        for line in rest:
            dm = _THIS_CALL.match(line)
            if dm:
                super_init = _delegated_base_init(dm.group(1), m, cls, resolver)
                break

    items = ([super_init] if super_init else []) + _init_list(cls, resolver)
    # Java runs instance field initializers / init-blocks at the top of every ctor that does NOT
    # delegate via this(...) (the delegated-to ctor runs them instead).
    is_delegating = any(_THIS_CALL.match(line) for line in rest)
    inst_init = [] if is_delegating else _instance_init_lines(cls, resolver, ledger)
    L = ['%s::%s(%s)' % (cls.name, cls.name, params)]
    if items:
        L.append('\t: ' + '\n\t, '.join(items))
    L.append('{')
    for line in inst_init:
        L.append('\t' + line)
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
    name = resolver.renamed_method(cls.name, m.name)   # field/method name-collision -> name_
    L = ['%s %s::%s(%s)' % (ret, cls.name, name, params), '{']
    L.extend('\t' + s for s in translate_body(m, cls, resolver, ledger))
    L.append('}')
    return L


def _render_mi_forward(cls, im, base, resolver, ledger):
    """A synthesized MI-diamond forwarding override: `R Cls::m(args) { [return] Base::m(args); }`
    so an interface pure-virtual already implemented by a base class is satisfied here too."""
    ret = ownership.return_cpp_type(im, resolver)
    params = ownership.params_str(im, resolver)
    call_args = ', '.join(p.name for p in im.params)
    ret_kw = '' if ret == 'void' else 'return '
    if ledger is not None:
        ledger.add(cls.name + '.cpp', Category.RULE,
                   'MI-diamond: synthesized %s() forwarding to %s:: (interface decl + base impl '
                   'unified into one vtable override)' % (im.name, base),
                   severity=Severity.LOW, source='emit', rule='mi-forward')
    return ['%s %s::%s(%s)' % (ret, cls.name, im.name, params), '{',
            '\t%s%s::%s(%s);' % (ret_kw, base, im.name, call_args), '}']


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
    # transitive CLOSURE: the statement translator emits arbitrarily deep chains
    # (`this->mc->gameSettings->keyBindInventory->keyCode`), so a .cpp needs the FULL
    # definition of every type reachable through field/method links -- not just one hop.
    # Iterate to a fixpoint over the resolved member environment.
    seen = set()
    frontier = list(names)
    while frontier:
        n = frontier.pop()
        if n in seen:
            continue
        seen.add(n)
        env = resolver.members.get(n)
        if not env:
            continue
        for jt in list(env['fields'].values()) + list(env['methods'].values()):
            add(jt)                          # may grow `names`
        frontier.extend(names - seen)
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
    # a class with instance field initializers / init-blocks but NO explicit constructor: Java's
    # implicit default ctor runs the inits, so synthesize one (C++'s implicit ctor would not).
    elif (not cls.constructors()
          and (any(f.initializer is not None for f in cls.instance_fields()) or cls.inst_inits)):
        items = _init_list(cls, resolver)
        inits = _instance_init_lines(cls, resolver, ledger)
        L2 = ['%s::%s()' % (cls.name, cls.name)]
        if items:
            L2.append('\t: ' + '\n\t, '.join(items))
        L2 += ['{'] + ['\t' + l for l in inits] + ['}']
        body += L2 + ['']
    body += _render_dtor(cls, ownership.owned_pointers(cls, resolver), ledger) + ['']
    for m in cls.methods:
        if m.is_ctor or m.is_abstract:
            continue
        body += _render_method(cls, m, resolver, ledger) + ['']
    for im, base in getattr(cls, 'mi_forwards', []):
        body += _render_mi_forward(cls, im, base, resolver, ledger) + ['']

    # static { } blocks: emit each as a static member method, then a file-local trigger that runs
    # them once at program start (Java runs them at class-load, after static field inits -- the
    # trigger sits after the static field definitions above, so order is preserved). Dropping these
    # left registries empty (EntityList/Packet/TileEntity id<->class maps) -- a crash/no-op class.
    static_inits = getattr(cls, 'static_inits', [])
    if static_inits:
        triggers = []
        for i, block in enumerate(static_inits):
            clines = _translate_block_lines(cls, resolver, ledger, _strip_block_braces(block), is_static=True)
            body += ['void %s::_j2x_clinit_%d()' % (cls.name, i), '{']
            body += ['\t' + l for l in clines] + ['}', '']
            triggers.append('%s::_j2x_clinit_%d();' % (cls.name, i))
        body += ['namespace {   // run the static initializer block(s) once at program start',
                 '\tstruct _ClinitRun_%s {' % cls.name,
                 '\t\t_ClinitRun_%s() {' % cls.name]
        body += ['\t\t\t' + t for t in triggers]
        body += ['\t\t}', '\t};', '\t_ClinitRun_%s _clinitRun_%s_instance;' % (cls.name, cls.name),
                 '}', '']
        if ledger is not None:
            ledger.add(cls.name + '.cpp', Category.RISK,
                       'static {} initializer block(s) emitted as _j2x_clinit + a run-once trigger '
                       '(dropping them left static registries empty)',
                       severity=Severity.MED, source='emit', rule='static-init-block')

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
