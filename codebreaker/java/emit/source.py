# -*- coding: utf-8 -*-
"""
source.py -- render a class's .cpp from IR + a TypeResolver.

This is where the console-crash classes are prevented BY CONSTRUCTION:
  * static/array allocations get value-init `new T[n]()`  (Java zero-fills; C++ doesn't)
  * every constructor zero-inits its instance fields in the member-init list
    (no uninitialized members)
  * a class that allocates heap in a constructor gets a destructor that frees it
    (Rule-of-Three leak fix)
  * `this(...)` constructor delegation is inlined by the back end, never silently dropped (the
    Block blockID=0 / BlockContainer flag bugs)

Every body, constructor and initialiser is written from the typed IR by the translator's back end
(codebreaker/translate/backends/cpp03.py); what it declines is an explicit, ledgered J2X-GAP.
"""

import re

from codebreaker.java import typemap
from codebreaker.java.ledger import Category, Severity
from codebreaker.java.emit import naming, ownership
from codebreaker.java.emit.std_transforms import apply_std_transforms

def _banner(cls_name, cpp_std=None, platform_label=None):
    std_label = cpp_std.label if cpp_std else 'C++03'
    plat = platform_label or 'unknown'
    return '// Auto-generated from %s.java\n// Target: %s / %s' % (cls_name, plat, std_label)
# Multi-dimensional aggregate `new T[][]{...}` / `new T[][][]{...}` (2+ empty `[]` dims then `{`).
# aggregate_array_body() intentionally handles only the 1-D `new T[]{...}` file-static-data form;
# nested aggregates can't be a flat C array, so they build a nested JArrayInit instead (C++03 has no
# braced new[][]). One `[]` is NOT matched here -- that stays the ownership.aggregate_array_body path.
_NEW_ARRAY_AGG_MULTI = re.compile(r'^new\s+[\w.]+\s*(?:\[\s*\]\s*){2,}\{', re.S)
# zero-initialised in every constructor's member-init list, as Java zeroes every field. FIXED BUG
# byte-fields-uninitialised: 'signed char' (Java byte) was missing when byte stopped being plain char, so
# every byte field -- packet types, yaw/pitch, NBT tags -- held garbage until assigned (the behavioural
# judge: Packet24MobSpawn.type -64, Packet34EntityTeleport.yaw -72 on a fresh packet).
_PRIMITIVE_CPP = {'int', 'short', 'long long', 'char', 'unsigned char', 'signed char',
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
def _array_elem_cpp(jtype, resolver):
    """The C++ element spelling for a T[] field, i.e. the JArray<...> template argument. Must MATCH
    types.cpp_type's array handling exactly, or the aggregate data array (`static elem _data[]`) and
    the JArray<elem> field type disagree -- e.g. byte[] is JArray<unsigned char> (a buffer) even though
    a SCALAR byte is signed char, so the element here must be unsigned char too."""
    elem_jt = jtype.element()
    if elem_jt.name == 'byte' and not elem_jt.is_array:      # byte[] buffer -> unsigned char (see types.py)
        return 'unsigned char'
    elem = resolver.cpp_type(elem_jt, 'element')
    if resolver.ownership(elem_jt) == 'pointer' and not elem.endswith('*'):
        elem += '*'
    return elem


# J2X-BUGFIX 2026-08-13 static-init-order: a FUNCTION-CALL static field initializer (e.g. StatList's
# `s_mineBlockStatArray = func_25153_a(...)`) leaked out as an eager file-scope global ctor. Global ctors
# run in UNDEFINED cross-TU order BEFORE main()/j2x_initAll(), so func_25153_a() -> Block::s_waterStill->
# blockID derefed a NULL Block static -> silent crash before main() (black screen, no boot markers). Fix:
# also defer lowercase-initial function-call initializers into the ordered initStatics(). See below.
def _is_deferrable_static_init(init, ft):
    """True if a static initializer must run AFTER zero-init, in CONTROLLED order -- so that cross-
    translation-unit global-ctor order (which C++ leaves UNDEFINED) can't crash the boot before main().
    Deferred inits move into the ordered initStatics(); the field def is zero-initialized. Compile-time
    constants and `const` members (can't be reassigned) stay eager.

    Order-sensitive when the initializer:
      - constructs an object (`new`) or reads another class's statics (`::` / `->`); OR
      - CALLS a free/static function -- a lowercase-initial identifier (incl. obfuscated `func_NNNNN`)
        immediately before `(`. Such a call runs arbitrary code at static-init time that may touch other
        classes' not-yet-constructed statics. The real bug this catches: `StatList::s_mineBlockStatArray
        = func_25153_a("stat.mineBlock", 16777216)` -- func_25153_a() indexes Block::s_blocksList, whose
        TU may not have run its global ctors yet -> null deref BEFORE main() (silent black screen).
    Constructor calls (PascalCase type, e.g. `JArray<T>(N)`, `JavaString(...)`) only allocate and are
    order-independent, so they stay eager."""
    head = ft.split('<')[0]
    if 'const' in head.split():
        return False
    if re.search(r'\bnew\b|::|->', init):
        return True
    # a call to a lowercase-initial function name (not a PascalCase constructor) -> defer
    return bool(re.search(r'(^|[^\w:.])[a-z]\w*\s*\(', init))


def _render_statics(cls, resolver, ledger):
    """Returns (def_lines, init_lines): the static field DEFINITIONS (zero-init for deferrable ones)
    and the DEFERRED initializer statements that initStatics() must run in dependency order."""
    defs, inits = [], []
    for f in cls.static_fields():
        name = naming.field_name(f)
        ft = resolver.field_type(f.jtype)
        init_raw = (f.initializer or '').strip()
        tree = TREE_BODIES.field_init(cls.name, f.name) if TREE_BODIES is not None and init_raw else None
        items = TREE_BODIES.array_items(cls.name, f.name) \
            if TREE_BODIES is not None and ownership.aggregate_array_body(f) is not None else None
        if init_raw and tree is None and items is None:
            defs.append('%s %s::%s;' % (ft, cls.name, name))
            inits += _gap(cls, ledger, 'static field %s.%s' % (cls.name, f.name), getattr(f, 'line', 0))
            continue
        if _NEW_ARRAY_AGG_MULTI.match(init_raw):         # new T[][]{...} -> nested JArrayInit (C++03)
            # the nested JArrayInit comma-expression the runtime converts to JArray<JArray<T> >. Deferred
            # into initStatics() like any value-constructing init -- it allocates, so it isn't a constant.
            defs.append('%s %s::%s;' % (ft, cls.name, name))          # zero/default now (order-safe)
            inits.append('%s::%s = %s;' % (cls.name, name, tree))     # nested-array value in initStatics()
            if ledger is not None:
                ledger.add(cls.name + '.cpp', Category.RISK,
                           'multi-dim static array new T[][]{...} -> nested JArrayInit '
                           '(C++03 has no braced new[][]; each level length-tracked)',
                           line=f.line, severity=Severity.LOW, source='emit', rule='static-aggregate-array-nd')
            continue
        if items is not None:                            # new T[]{a,b,c} -> file-static data + JArray
            elem = _array_elem_cpp(f.jtype, resolver)
            data = '_%s_%s_data' % (cls.name, name)
            defs.append('static %s %s[] = { %s };' % (elem, data, ', '.join(items)))
            defs.append('%s %s::%s(%s, %d);' % (ft, cls.name, name, data, len(items)))
            if ledger is not None:
                ledger.add(cls.name + '.cpp', Category.RISK,
                           'static array new T[]{...} -> JArray over a file-static data array '
                           '(C++03 has no braced new[]; length tracked)',
                           line=f.line, severity=Severity.LOW, source='emit', rule='static-aggregate-array')
            continue
        init = tree if init_raw else _default_value(ft)  # forward-declared static -> NULL/0/default
        if init is None:
            defs.append('%s %s::%s;' % (ft, cls.name, name))
        elif _is_deferrable_static_init(init, ft):
            defs.append('%s %s::%s;' % (ft, cls.name, name))          # zero-init now (order-safe)
            inits.append('%s::%s = %s;' % (cls.name, name, init))     # initialize in ordered initStatics()
        else:
            defs.append('%s %s::%s = %s;' % (ft, cls.name, name, init))   # compile-time const: eager
    return defs, inits


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



# The translator's back end (codebreaker.translate.backends.cpp03.Bodies), set by transpile for the
# conversion: every body, constructor and initialiser is written from the typed IR.
TREE_BODIES = None


def _gap(cls, ledger, what, line=0):
    """What the IR's back end could not write: said in the code and the ledger, and thrown if it runs
    -- never a guess."""
    if ledger is not None:
        ledger.add(cls.name + '.cpp', Category.STUBBED,
                   'untranslated %s: the back end declined it -- the body throws UnsupportedOperationException '
                   '(J2X-GAP)' % what, line=line or 0, severity=Severity.HIGH, source='emit', rule='untranslated')
    return ['// J2X-GAP: %s was not translated (see the ledger)' % what,
            'throw new UnsupportedOperationException();']


def _instance_init_lines(cls, resolver, ledger):
    """C++ for Java INSTANCE field initializers + instance `{ }` blocks, in declaration order.
    Java runs these at the top of every constructor that does NOT delegate via this(...). Dropping
    them was the null-deref / uninitialized-field crash class (e.g. EntityPlayer.inventory)."""
    tree = TREE_BODIES.instance_inits(cls.name) if TREE_BODIES is not None else None
    if tree is not None:
        return tree
    if not any(f.initializer is not None for f in cls.instance_fields()) and not cls.inst_inits:
        return []
    return _gap(cls, ledger, 'instance initialisers of %s' % cls.name)


def _render_ctor(cls, m, resolver, ledger):
    params = ownership.params_str(m, resolver)
    tree = TREE_BODIES.ctor(cls.name, m) if TREE_BODIES is not None else None
    if tree is not None:
        _kind, args, body = tree
        items = (['%s(%s)' % (cls.parent, args)] if args is not None and cls.parent else []) + _init_list(cls, resolver)
        body = _instance_init_lines(cls, resolver, ledger) + body
    else:
        items = _init_list(cls, resolver)
        body = _gap(cls, ledger, 'constructor %s(%s)' % (cls.name, params), getattr(m, 'line', 0))
    L = ['%s::%s(%s)' % (cls.name, cls.name, params)]
    if items:
        L.append('\t: ' + '\n\t, '.join(items))
    L.append('{')
    L += ['\t' + line for line in body]
    L.append('}')
    return L


def _render_two_phase(cls, resolver, ledger):
    """A two-phase class (see twophase.py): `C::C()` zeroes the fields, and each Java constructor becomes
    `C* C::j2xInit(params)` -- the parent's j2xInit (Java's super(...)), the instance initialisers, the
    body, `return this`. this(args) is a call to the other j2xInit, which does the parent and initialisers."""
    parent_two = cls.parent in resolver.two_phase
    items = _init_list(cls, resolver)
    L = ['%s::%s()' % (cls.name, cls.name)]
    if items:
        L.append('\t: ' + '\n\t, '.join(items))
    L += ['{', '}', '']
    ctors = cls.constructors() or [None]
    for m in ctors:
        params = ownership.params_str(m, resolver) if m is not None else ''
        tree = TREE_BODIES.ctor(cls.name, m, True) if TREE_BODIES is not None else None
        L += ['%s* %s::j2xInit(%s)' % (cls.name, cls.name, params), '{']
        if tree is not None:
            kind, args, body = tree
            head = []
            if kind != 'this' and parent_two:
                head.append('%s::j2xInit(%s);' % (cls.parent, args or ''))   # super(...), or Java's implicit one
            inits = [] if kind == 'this' else _instance_init_lines(cls, resolver, ledger)
            L += ['\t' + x for x in head + inits + body]
            L += ['\treturn this;', '}', '']
        else:
            L += ['\t' + x for x in _gap(cls, ledger, 'constructor %s(%s)' % (cls.name, params),
                                         getattr(m, 'line', 0))]
            L += ['}', '']
    if ledger is not None:
        ledger.add(cls.name + '.cpp', Category.RULE,
                   'two-phase construction: constructors are j2xInit(...) so their virtual calls reach the '
                   'override, as in Java', severity=Severity.LOW, source='emit', rule='two-phase-ctor')
    return L[:-1]


# ---------------------------------------------------------------------------
# destructor (Rule of Three: free heap allocated in constructors)
# ---------------------------------------------------------------------------
def _render_dtor(cls, resolver, ledger):
    owned = ownership.owned_pointers(cls, resolver)
    containers = ownership.owned_containers(cls, resolver)
    L = ['%s::~%s()' % (cls.name, cls.name), '{']
    # container ELEMENTS first (an element may reference a scalar-owned sibling), then scalar pointers
    for name in sorted(containers):
        L.append('\tfor (int _i = 0; _i < %s%s; ++_i) { delete %s[_i]; }' % (name, containers[name], name))
    for name in sorted(owned):
        L.append('\tdelete%s %s;' % ('[]' if owned[name] else '', name))
        L.append('\t%s = NULL;' % name)
    L.append('}')
    freed = sorted(list(owned) + list(containers))
    if freed and ledger is not None:
        ledger.add(cls.name + '.cpp', Category.RISK,
                   'Rule-of-Three: synthesized destructor frees %d heap-owning field(s): %s'
                   % (len(freed), ', '.join(freed)),
                   severity=Severity.MED, source='emit', rule='rule-of-three-dtor')
    return L


# ---------------------------------------------------------------------------
# methods
# ---------------------------------------------------------------------------
# a local declaration `JList<T*> name = <rhs>;` (the JSet form too) -- candidate to receive an owned-return guard
def _render_method(cls, m, resolver, ledger):
    ret = ownership.return_cpp_type(m, resolver)
    params = ownership.params_str(m, resolver)
    name = resolver.renamed_method(cls.name, m.name)   # field/method name-collision -> name_
    L = ['%s %s::%s(%s)' % (ret, cls.name, name, params), '{']
    tree = TREE_BODIES.body(cls.name, m) if TREE_BODIES is not None else None
    body = list(tree) if tree is not None else _gap(cls, ledger, 'method %s' % m.name, getattr(m, 'line', 0))
    L.extend('\t' + s for s in body)
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
    if 'j2xClassOf' in body_text or 'typeid' in body_text:     # `.class` / getClass() -> needs Class.h
        names.add('Class')
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


def _render_typed_enum_source(cls, resolver, ledger, cpp_std, platform_label):
    """Emit the .cpp for a typed enum: static const/instance defs, the ctor(s)/dtor/methods via the
    normal renderers, and an initStatics() that builds the singleton instance table (in ctor-body
    dependency order via j2x_initAll), assigns ordinals, and fills values()."""
    body = []
    # static definitions: the singleton constants + real enum statics (zero-init) + values backing
    for ec in cls.enum_consts:
        body.append('%s* %s::%s;' % (cls.name, cls.name, naming.enum_member(ec.name)))
    for f in cls.static_fields():
        body.append('%s %s::%s;' % (resolver.field_type(f.jtype), cls.name, f.name))
    body.append('JArray<%s* > %s::_j2x_values;' % (cls.name, cls.name))
    body.append('')

    for m in cls.constructors():
        body += _render_ctor(cls, m, resolver, ledger) + ['']
    body += _render_dtor(cls, resolver, ledger) + ['']
    for m in cls.methods:
        if not m.is_ctor and not m.is_abstract:
            body += _render_method(cls, m, resolver, ledger) + ['']
    body += ['int %s::ordinal()' % cls.name, '{', '\treturn this->__ordinal;', '}', '']
    body += ['JArray<%s* >& %s::values()' % (cls.name, cls.name), '{', '\treturn _j2x_values;', '}', '']

    # initStatics(): construct each singleton, set its ordinal, run real static inits, fill values().
    init = []
    for i, ec in enumerate(cls.enum_consts):
        args = TREE_BODIES.enum_args(cls.name, ec.name) if TREE_BODIES is not None else None
        if args is None:
            init += _gap(cls, ledger, 'enum constant %s.%s' % (cls.name, ec.name))
            continue
        nm = naming.enum_member(ec.name)
        init.append('%s = new %s(%s);' % (nm, cls.name, ', '.join(args)))
        init.append('%s->__ordinal = %d;' % (nm, i))
    for f in cls.static_fields():
        if f.initializer:
            value = TREE_BODIES.field_init(cls.name, f.name) if TREE_BODIES is not None else None
            if value is None:
                init += _gap(cls, ledger, 'static field %s.%s' % (cls.name, f.name))
            else:
                init.append('%s = %s;' % (f.name, value))
    init += _static_block_lines(cls, ledger)
    init.append('_j2x_values = JArray<%s* >(%d);' % (cls.name, len(cls.enum_consts)))
    for i, ec in enumerate(cls.enum_consts):
        init.append('_j2x_values[%d] = %s;' % (i, naming.enum_member(ec.name)))
    cls.has_init_statics = True                              # always: the constant table must be built
    body += ['void %s::initStatics()' % cls.name, '{']
    body += ['\t' + l for l in init] + ['}', '']

    used = _referenced_types(cls, resolver, '\n'.join(body))
    L = ['#include "stdafx.h"', '#include "JavaRT.h"', '', _banner(cls.name, cpp_std, platform_label), '',
         '#include "%s.h"' % cls.name]
    L += ['#include "%s.h"' % n for n in used]
    L += [''] + body
    if ledger is not None:
        ledger.add(cls.name + '.cpp', Category.PORTED,
                   'typed enum source: %d singletons built in initStatics()' % len(cls.enum_consts),
                   source='emit', rule='typed-enum')
    return apply_std_transforms('\n'.join(L).rstrip() + '\n', cpp_std)


# J2X-BUGFIX 2026-08-13 static-init-cycle: Java initializes a class LAZILY on first static access, so a
# self-singleton getter called mid-init (StatList::func_25157_c -> CraftingManager::getInstance(), whose
# instance is built in a LATER-ordered initStatics()) returns NULL and crashes. A linear j2x_initAll can't
# reproduce lazy init for a true cycle. Emulate it: a `X::s_foo = new X(...)` singleton makes its pure
# getter (`return s_foo;`) build on demand, and initStatics() becomes idempotent (the driver may also call
# it). Detects the singleton field from the deferred init lines.
def _detect_self_singleton(clsname, static_init_lines):
    pat = re.compile(r'^%s::(\w+)\s*=\s*new\s+%s\b' % (re.escape(clsname), re.escape(clsname)))
    for l in static_init_lines:
        m = pat.match(l.strip())
        if m:
            return m.group(1)
    return None


def _inject_lazy_singleton(mlines, clsname, field):
    """If a rendered method is a PURE getter that returns the singleton field, prepend a lazy-init guard so
    first access builds it (Java's <clinit>-on-static-access). Only touches single-statement `return field;`
    bodies -- never a method that does other work."""
    try:
        open_i = mlines.index('{')
    except ValueError:
        return mlines
    stmts = [i for i in range(open_i + 1, len(mlines)) if mlines[i].strip() not in ('', '}')]
    if len(stmts) != 1:
        return mlines
    i = stmts[0]
    if mlines[i].strip() in ('return %s;' % field, 'return %s::%s;' % (clsname, field)):
        indent = mlines[i][:len(mlines[i]) - len(mlines[i].lstrip())]
        mlines[i:i] = ['%sif (%s == NULL) initStatics();   // J2X: lazy singleton (Java <clinit> on static access)'
                       % (indent, field)]
    return mlines


def _static_block_lines(cls, ledger):
    """The class's static {} blocks, in source order."""
    static_inits = getattr(cls, 'static_inits', [])
    if not static_inits:
        return []
    tree = TREE_BODIES.static_blocks(cls.name) if TREE_BODIES is not None else None
    if tree is None or len(tree) != len(static_inits):
        return _gap(cls, ledger, 'static {} blocks of %s' % cls.name)
    return [line for lines in tree for line in lines]


def render_source(cls, resolver, ledger=None, cpp_std=None, platform_label=None):
    """Returns the .cpp text, or None for interfaces / constant-only enums (header-only)."""
    if cls.is_enum and cls.name in resolver.typed_enums:
        return _render_typed_enum_source(cls, resolver, ledger, cpp_std, platform_label)
    if cls.is_interface or cls.is_enum:
        return None

    body = []
    static_defs, static_init_lines = _render_statics(cls, resolver, ledger)
    singleton_field = _detect_self_singleton(cls.name, static_init_lines)   # lazy-init emulation, see helper
    if static_defs:
        body += static_defs + ['']
    two_phase = cls.name in getattr(resolver, 'two_phase', ())
    if two_phase:
        body += _render_two_phase(cls, resolver, ledger) + ['']
    else:
        for m in cls.constructors():
            body += _render_ctor(cls, m, resolver, ledger) + ['']
    # synthesized zero-initializing default ctor for a value type that lacks one (so it can be
    # held by value as a field); matches the declaration emitted by header.py
    if two_phase:
        pass
    elif (cls.name in resolver.values and cls.constructors()
            and not any(c.arity == 0 for c in cls.constructors())):
        items = _init_list(cls, resolver)
        L2 = ['%s::%s()' % (cls.name, cls.name)]
        if items:
            L2.append('\t: ' + '\n\t, '.join(items))
        L2 += ['{', '}']
        body += L2 + ['']
    # A class with NO explicit ctor relies on Java's implicit default ctor, which zero-inits ALL fields
    # and runs any initializers/init-blocks. C++'s implicit ctor leaves primitives/pointers as garbage
    # (the uninitialized-field crash class), so synthesize a zero-init default ctor whenever the class
    # has ANY instance fields (or init-blocks). Matches the declaration emitted by header.py.
    elif (not cls.constructors()
          and (cls.instance_fields() or cls.inst_inits)):
        items = _init_list(cls, resolver)
        inits = _instance_init_lines(cls, resolver, ledger)
        L2 = ['%s::%s()' % (cls.name, cls.name)]
        if items:
            L2.append('\t: ' + '\n\t, '.join(items))
        L2 += ['{'] + ['\t' + l for l in inits] + ['}']
        body += L2 + ['']
    body += _render_dtor(cls, resolver, ledger) + ['']
    for m in cls.methods:
        if m.is_ctor or m.is_abstract:
            continue
        mlines = _render_method(cls, m, resolver, ledger)
        if singleton_field:
            mlines = _inject_lazy_singleton(mlines, cls.name, singleton_field)
        body += mlines + ['']
    # An abstract method CALLED from a constructor (Entity's ctor calls the abstract entityInit()) needs
    # an out-of-line definition: from the base ctor the call resolves NON-virtually to THIS class's version,
    # so a pure `= 0` with no body is an unresolved external. A pure virtual is allowed to have a body;
    # emit an empty one (subclasses still override it). Java would dispatch to the override here, but C++
    # calls the base during base-construction -- the standard virtual-call-in-ctor divergence.
    ctor_text = '\n'.join(t for c in cls.constructors() for _ln, t in c.body)
    for m in cls.methods:
        if not m.is_abstract:
            continue
        if re.search(r'(?<![\w.>])(?:this\s*(?:\.|->)\s*)?%s\s*\(' % re.escape(m.name), ctor_text):
            rt = ownership.return_cpp_type(m, resolver)
            sig = '%s %s::%s(%s)' % (rt, cls.name, m.name, ownership.params_str(m, resolver))
            if rt == 'void':
                body += [sig, '{', '}', '']
            else:
                dv = _default_value(rt)
                body += [sig, '{', '\treturn %s;' % (dv if dv is not None else rt + '()'), '}', '']
            if ledger is not None:
                ledger.add(cls.name + '.cpp', Category.RISK,
                           'abstract %s() is called from a constructor -- emitted an empty base definition '
                           '(C++ calls the base, not the override, during base construction)' % m.name,
                           line=getattr(m, 'line', 0), severity=Severity.MED, source='emit',
                           rule='abstract-called-in-ctor')
    for im, base in getattr(cls, 'mi_forwards', []):
        body += _render_mi_forward(cls, im, base, resolver, ledger) + ['']

    # ORDERED STATIC INIT: deferred static field initializers + Java static{} blocks are run by a
    # single `void <Class>::initStatics()`, which the generated driver (j2x_init.cpp / j2x_initAll())
    # calls in dependency order -- NOT by a file-local trigger (whose cross-translation-unit order is
    # undefined: the static-init-order crash class -- Block initializing before Material -> null vtable).
    # Static field DEFS above are zero-initialized; initStatics() assigns the real values, in order.
    init_body = list(static_init_lines)                  # deferred static FIELD initializers first
    init_body += _static_block_lines(cls, ledger)        # then the Java static{} block bodies
    # ALWAYS emit initStatics() (empty when nothing to defer) so the driver can call it on any class
    # without a header/source mismatch. header.py declares it for every concrete class to match.
    # cls.has_init_statics lets the pipeline driver call only the classes that actually defer work.
    cls.has_init_statics = bool(init_body)
    body += ['void %s::initStatics()' % cls.name, '{']
    if singleton_field and init_body:      # idempotent: a lazy getter may have already built the singleton
        body += ['\tif (%s != NULL) return;   // J2X: idempotent (may be lazily inited via a singleton getter)'
                 % singleton_field]
    body += ['\t' + l for l in init_body] + ['}', '']
    if init_body and ledger is not None:
        ledger.add(cls.name + '.cpp', Category.RISK,
                   'static field inits + static{} blocks deferred into initStatics(), run in '
                   'dependency order by j2x_initAll() (kills the static-init-order crash class)',
                   severity=Severity.MED, source='emit', rule='static-init-ordered')

    # a .cpp needs the FULL definition of every user type it touches (member access,
    # new/cast/static), not just the forward decls in its own header. Collect from the
    # class's signatures (parent, fields incl. inherited, params, returns) AND capitalized
    # tokens in the body -- the signatures catch types reached only through lowercase vars
    # (e.g. entity->posX, where the type 'Entity' is never spelled in the body).
    used = _referenced_types(cls, resolver, '\n'.join(body))
    L = ['#include "stdafx.h"', '#include "JavaRT.h"', '', _banner(cls.name, cpp_std, platform_label), '',
         '#include "%s.h"' % cls.name]
    L += ['#include "%s.h"' % n for n in used]
    L += ['']
    L += body

    if ledger is not None:
        ledger.add(cls.name + '.cpp', Category.PORTED, 'generated source for %s' % cls.name,
                   source='emit', rule='source')
    return apply_std_transforms('\n'.join(L).rstrip() + '\n', cpp_std)
