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
from .std_transforms import apply_std_transforms
from .statements import (translate_body, _resolve_chains, _resolve_call_chains,
                         sub_outside_literals, _split_top_commas, _fix_numeric_suffixes,
                         _rewrite_array_aggregates, _MATH, distribute_cast_into_ternaries,
                         _balanced_paren)

def _banner(cls_name, cpp_std=None, platform_label=None):
    std_label = cpp_std.label if cpp_std else 'C++03'
    plat = platform_label or 'unknown'
    return '// Auto-generated from %s.java\n// Target: %s / %s' % (cls_name, plat, std_label)
_NEW_ARRAY = re.compile(r'^new\s+[\w.]+\s*\[([^\]]+)\]\s*$')   # size may be any expression
# Multi-dimensional aggregate `new T[][]{...}` / `new T[][][]{...}` (2+ empty `[]` dims then `{`).
# aggregate_array_body() intentionally handles only the 1-D `new T[]{...}` file-static-data form;
# nested aggregates can't be a flat C array, so they build a nested JArrayInit instead (C++03 has no
# braced new[][]). One `[]` is NOT matched here -- that stays the ownership.aggregate_array_body path.
_NEW_ARRAY_AGG_MULTI = re.compile(r'^new\s+[\w.]+\s*(?:\[\s*\]\s*){2,}\{', re.S)
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
        # the size expression is not a bare literal in general (e.g. `new int[EnumOptions.values().length]`)
        # -> translate its references too, so `EnumOptions.values()` becomes `EnumOptions::values()`.
        n = _static_ref(m.group(1), resolver, own_statics, cls.name)
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
    # Every runtime-support class (RUNTIME_HEADERS) is a legitimate `::` static holder here, matching
    # statements.py's static_set -- so Logger.getLogger / NumberFormat.getIntegerInstance / Locale.US
    # / Charset.forName in a static initializer lower to `::` just as they do in method bodies.
    holders = typemap.RUNTIME_HEADERS | typemap.JAVART_STATICS | set(resolver.enums)

    # static-field/array initializers don't go through the statement translator, so apply the two
    # transforms they still need: Java numeric-literal suffixes (`2.0D`->`2.0`, `7L`->`7LL` -- MSVC
    # rejects the `D` suffix -> "bad suffix on number") and primitive-type casts (`(byte)2` ->
    # `(signed char)2`; `byte`/`boolean` are not C++ types).
    expr = _fix_numeric_suffixes(expr)
    expr = re.sub(r'\(\s*(byte|boolean|short|int|long|float|double|char)\s*\)',
                  lambda mm: '(%s)' % typemap.JAVA_TO_CPP.get(mm.group(1), mm.group(1)), expr)
    # Math.<fn> -> C math (sqrtf, floorf, javaMathRound, ...) -- same map the statement translator uses,
    # since a static initializer (e.g. NoiseGenerator2 = 0.5 * (Math.sqrt(3.0) - 1.0)) skips that pass.
    expr = _MATH.sub(lambda mm: typemap.MATH_API_EMIT.get(mm.group(1), mm.group(0)), expr)
    # reference-type cast -> pointer cast: `(BlockGrass)(new BlockGrass(2))` -> `(BlockGrass*)(...)`,
    # `(Achievement)NULL` -> `(Achievement*)NULL`. Statement bodies get this via emit.statements'
    # `_cast`, but static initializers bypass the statement translator, so apply it here too. Only
    # POINTER types gain the star -- value types (JavaString/JObject, `(String)NULL` -> "") are left
    # exactly as-is so their conversion-operator behaviour is unchanged.
    def _refcast(mm):
        t = mm.group(1)
        return '(%s*)' % t if resolver.is_pointer(ir.JType(t)) else mm.group(0)
    expr = re.sub(r'\(([A-Z]\w*)\)(?=\s*[\w("])', _refcast, expr)
    # distribute a pointer-cast over a conditional's arms (see emit.statements) -- a static
    # initializer may hold `(Base)(cond ? new A() : new B())` too.
    expr = distribute_cast_into_ternaries(expr)

    def repl(m):
        cls, mem, paren = m.group(1), m.group(2), m.group(3)
        if mem == 'class' and not paren:                         # Java `X.class` literal (any X) ->
            from .statements import _CLASS_LIT_TYPE               # wrapper/String -> its C++ type (typeid)
            return 'j2xClassOf(typeid(%s))' % _CLASS_LIT_TYPE.get(cls, cls)   # STABLE Class* (no dangling temp)
        if cls in holders:
            return '%s::%s%s' % (cls, mem, paren or '')          # holder/enum/method: no s_
        if cls in resolver.known:
            if paren:
                return '%s::%s%s' % (cls, mem, paren)            # static method call
            return '%s::%s' % (cls, naming.static_field_name(mem))   # static field -> s_
        return m.group(0)
    expr = re.sub(r'\b([A-Z]\w*)\.(\w+)(\s*\()?', repl, expr)
    from .statements import fix_enum_access
    expr = fix_enum_access(expr, resolver)           # const/typed enum ordinal()/values() (MappingHelper tables)
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
    # own + inherited static field names, for prefixing bare references in initializers
    own_statics = (set(f.name for f in cls.static_fields())
                   | resolver.members.get(cls.name, {}).get('statics', set()))
    for f in cls.static_fields():
        name = naming.field_name(f)
        ft = resolver.field_type(f.jtype)
        init_raw = (f.initializer or '').strip()
        if _NEW_ARRAY_AGG_MULTI.match(init_raw):         # new T[][]{...} -> nested JArrayInit (C++03)
            # Translate references first (numeric leaves are untouched), then build the nested
            # JArrayInit comma-expression the runtime converts to JArray<JArray<T> >. Deferred into
            # initStatics() like any value-constructing init -- it allocates, so it isn't a constant.
            value = _rewrite_array_aggregates(
                _static_ref(init_raw, resolver, own_statics, cls.name), resolver)
            defs.append('%s %s::%s;' % (ft, cls.name, name))          # zero/default now (order-safe)
            inits.append('%s::%s = %s;' % (cls.name, name, value))    # nested-array value in initStatics()
            if ledger is not None:
                ledger.add(cls.name + '.cpp', Category.RISK,
                           'multi-dim static array new T[][]{...} -> nested JArrayInit '
                           '(C++03 has no braced new[][]; each level length-tracked)',
                           line=f.line, severity=Severity.LOW, source='emit', rule='static-aggregate-array-nd')
            continue
        agg = ownership.aggregate_array_body(f)
        if agg is not None:                              # new T[]{a,b,c} -> file-static data + JArray
            elem = _array_elem_cpp(f.jtype, resolver)
            items = [_static_ref(e.strip(), resolver, own_statics, cls.name) for e in agg.split(',') if e.strip()]
            data = '_%s_%s_data' % (cls.name, name)
            defs.append('static %s %s[] = { %s };' % (elem, data, ', '.join(items)))
            defs.append('%s %s::%s(%s, %d);' % (ft, cls.name, name, data, len(items)))
            if ledger is not None:
                ledger.add(cls.name + '.cpp', Category.RISK,
                           'static array new T[]{...} -> JArray over a file-static data array '
                           '(C++03 has no braced new[]; length tracked)',
                           line=f.line, severity=Severity.LOW, source='emit', rule='static-aggregate-array')
            continue
        init = _static_init(f, ft, ledger, cls, resolver, own_statics)
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


# Body init lines that a value member already produces by default-construction (empty container) or that
# a zero member-init already covers -- redundant, and risky to re-emit into a hand-written override whose
# field type may differ. Dropped when back-filling an override ctor.
_REDUNDANT_INIT = re.compile(r'^(std::(map|set|vector|list)<.*>|JList<.*>|JArray<.*>)\(\)$')
_ZERO_RHS = {'NULL', '0', 'false', '0.0', '0.0F', '0.0f', '0L', '0LL', "'\\0'"}


def _init_is_redundant(stmt):
    m = re.match(r'\s*this->\w+\s*=\s*(.+?);\s*$', stmt)
    r = m.group(1).strip() if m else ''
    return r in _ZERO_RHS or bool(_REDUNDANT_INIT.match(r))


def synth_default_ctor(cls, resolver):
    """Return (decl, definition, referenced_type_names) for a synthesized zero-init + Java-field-init
    default ctor, or None if the class needs none. Used to BACK-FILL OVERRIDE classes that have no ctor
    and therefore dropped Java's implicit field zero-init + initializers (the SoundManager/SoundPool
    crash class). Mirrors the ctor source.py already synthesizes for generated no-ctor classes."""
    items = _init_list(cls, resolver)
    inits = [l for l in _instance_init_lines(cls, resolver, None) if not _init_is_redundant(l)]
    if not items and not inits:
        return None
    L = ['%s::%s()' % (cls.name, cls.name)]
    if items:
        L.append('\t: ' + '\n\t, '.join(items))
    L += ['{'] + ['\t' + l for l in inits] + ['}']
    defn = '\n'.join(L)
    refs = (set(re.findall(r'\bnew\s+(\w+)\s*[(\[]', defn))
            | set(re.findall(r'typeid\((\w+)\)', defn))
            | set(re.findall(r'\b([A-Za-z_]\w*)::', defn)))   # static/factory calls (GLAllocation::, Block::)
    return ('%s();' % cls.name, defn, refs)


_NEW_ARG_TYPE = re.compile(r'^\s*new\s+([A-Za-z_]\w*)')
_CAST_ARG_TYPE = re.compile(r'^\s*\(\s*([A-Za-z_]\w*)\s*\)\s*\S')
_FACTORY_ARG_TYPE = re.compile(r'^\s*([A-Za-z_]\w*)\s*::\s*[A-Za-z_]\w*\s*\(')


def _infer_arg_type(expr):
    """Static type of a delegation argument expression, when obvious: `new X(...)` -> X, `(X)e` -> X,
    `X::factory(...)` -> X (a static factory almost always returns its own type, e.g.
    Vec3D::createVectorHelper). None otherwise -- resolved via the caller's param map, or left unscored."""
    m = _NEW_ARG_TYPE.match(expr)
    if m:
        return m.group(1)
    m = _CAST_ARG_TYPE.match(expr)
    if m:
        return m.group(1)
    m = _FACTORY_ARG_TYPE.match(expr)
    if m:
        return m.group(1)
    return None


def _paren_arg(a):
    """Parenthesize a delegation arg before it is substituted into a ctor body, so a compound arg
    keeps its precedence: substituting `(Block*)var1` into `var1.blockID` must yield
    `((Block*)var1)->blockID`, not `(Block*)var1->blockID` (a cast of the whole field access)."""
    a = a.strip()
    if re.match(r'^[A-Za-z_][\w.]*$', a) or re.match(r'^-?\d[\w.]*$', a) or (a.startswith('(') and a.endswith(')')):
        return a                                       # bare name/field, number, or already parenthesized
    return '(' + a + ')'


def _ctor_delegates(c):
    """True if constructor `c` itself starts by delegating via `this(...)`."""
    for _ln, t in c.body:
        if re.match(r'\s*this\s*\(', t):
            return True
    return False


def _select_deleg_target(args, deleg, cls):
    """Pick the constructor a `this(args)` delegates to. Matching by ARG COUNT alone is ambiguous when
    a class has several same-arity ctors (Achievement's Block/Item/ItemStack 6-arg trio) -- it can pick
    another DELEGATING ctor and recurse (`new ItemStack(new ItemStack(...))`) or lose the base init.
    Score candidates by param-type match against the args (`new X`/`(X)` args + the delegating ctor's
    own param types), tie-breaking toward the terminal (non-delegating) ctor."""
    cands = [c for c in cls.constructors() if c is not deleg and len(c.params) == len(args)]
    if len(cands) <= 1:
        return cands[0] if cands else None
    pmap = dict((p.name, p.jtype.name) for p in deleg.params)
    arg_types = []
    for a in args:
        t = _infer_arg_type(a)
        if t is None:
            t = pmap.get(a.strip())
        arg_types.append(t)

    def score(c):
        matched = sum(1 for i, p in enumerate(c.params) if arg_types[i] and p.jtype.name == arg_types[i])
        return (matched, not _ctor_delegates(c))
    return max(cands, key=score)


def _delegated_base_init(args_str, deleg, cls, resolver):
    """When a ctor delegates `this(args)`, the base class is constructed by the TARGET ctor's
    super(...). C++03 has no delegation, so the delegating ctor must carry that base init itself:
    return `Parent(<target's super args, target-params substituted by the delegation args>)`."""
    if not cls.parent:
        return None
    args = [a for a in _split_top_commas(args_str) if a]   # balanced: nested-comma calls stay one arg
    target = _select_deleg_target(args, deleg, cls)
    if target is None:
        return None
    sub = dict((p.name, _paren_arg(args[i])) for i, p in enumerate(target.params))
    # Substitute the target's PARAM references, but never an identifier used as a MEMBER
    # (`this->pos`, `obj.pos`, `Cls::pos`) -- a field that shares a param's name must keep its
    # name. Without the lookbehind, `this->pos = pos;` became `this->(arg) = (arg);` (the botched
    # Vertex ctor). The leading (?<![\w.>:]) excludes a preceding word char, '.', '>' (from '->')
    # and ':' (from '::'); the trailing \b keeps whole-word matching.
    rx = re.compile(r'(?<![\w.>:])(' + '|'.join(re.escape(k) for k in sub) + r')\b') if sub else None
    for line in translate_body(target, cls, resolver):
        sm = _SUPER_CALL.match(line)
        if sm:
            sargs = sm.group(1).strip()
            if rx:
                sargs = rx.sub(lambda mm: sub[mm.group(1)], sargs)
            return '%s(%s)' % (cls.parent, sargs)
    return None


def _inline_delegation(args_str, deleg, cls, resolver, ledger, _depth=0):
    """Replace `this(args);` with the target constructor's body (C++03 has no ctor
    delegation), substituting the target's params with the call args. This makes the
    delegated ctor's registration actually run -- the Block blockID=0 / container-flag fix.
    Chained delegations (target body is itself a this() call) are recursively inlined."""
    if _depth > 8:
        return ['// PORT: delegation this(%s) -- recursion limit reached; inline manually' % args_str]
    args = [a for a in _split_top_commas(args_str) if a]   # balanced: nested-comma calls stay one arg
    target = _select_deleg_target(args, deleg, cls)
    if target is None:
        if ledger is not None:
            ledger.add(cls.name + '.cpp', Category.STUBBED,
                       'constructor delegation this(%s): no target ctor with %d args found -- inline manually'
                       % (args_str, len(args)), line=getattr(deleg, 'line', 0),
                       severity=Severity.HIGH, source='emit', rule='ctor-delegation')
        return ['// PORT: delegation this(%s) -- target ctor not found; inline manually' % args_str,
                'this(%s);' % args_str]
    sub = dict((p.name, _paren_arg(args[i])) for i, p in enumerate(target.params))
    # Substitute the target's PARAM references, but never an identifier used as a MEMBER
    # (`this->pos`, `obj.pos`, `Cls::pos`) -- a field that shares a param's name must keep its
    # name. Without the lookbehind, `this->pos = pos;` became `this->(arg) = (arg);` (the botched
    # Vertex ctor). The leading (?<![\w.>:]) excludes a preceding word char, '.', '>' (from '->')
    # and ':' (from '::'); the trailing \b keeps whole-word matching.
    rx = re.compile(r'(?<![\w.>:])(' + '|'.join(re.escape(k) for k in sub) + r')\b') if sub else None
    out = ['// inlined this(%s) -- C++03 lacks ctor delegation' % args_str]
    for line in translate_body(target, cls, resolver):
        cm = _THIS_CALL.match(line)
        if cm:
            # Recursively inline chained delegation with substituted args
            chained_args = rx.sub(lambda mm: sub[mm.group(1)], cm.group(1)) if rx else cm.group(1)
            out.extend(_inline_delegation(chained_args, target, cls, resolver, ledger, _depth + 1))
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
_OWNED_LOCAL_DECL = re.compile(r'^(\s*)(JList|JSet)\s*<\s*([A-Za-z_][\w:]*)\s*\*\s*>\s+([A-Za-z_]\w*)\s*=\s*(.+);\s*$')
_CALL_NAME = re.compile(r'([A-Za-z_]\w*)\s*\(')


def _elements_not_retained(var, text):
    """True iff every use of `var` after its declaration is transient -- a container-level read
    (`var.size()` / `var.length`) or an element used ONLY as a call receiver (`var[i]->m(...)`). Any other
    use (element stored, whole container returned/passed) means an element may outlive the guard, so we
    must NOT free -- return False and leave the leak (still flagged) rather than risk a dangling delete."""
    v = re.escape(var)
    for mo in re.finditer(r'\b%s\b' % v, text):
        after = text[mo.end():]
        if re.match(r'\s*\.\s*(?:size\s*\(\s*\)|length\b)', after):
            continue
        if re.match(r'\s*\[[^\]]*\]\s*->', after):
            continue
        return False
    return True


def _free_owned_returns(lines, resolver):
    """Post-pass: after a `JList<T*> x = ...builder(...);` where `builder` hands back an owned container
    (resolver.own_catalog.return_methods), drop an RAII `J2X_ScopedFreeElems` guard so the caller frees the
    elements at scope exit -- the no-GC caller-frees contract (Level.getCubes -> new AABBs in Entity.move).
    Only when the elements are used transiently, so the delete can never dangle."""
    owned = resolver.own_catalog.return_methods
    if not owned:
        return lines
    out = []
    for idx, ln in enumerate(lines):
        out.append(ln)
        dm = _OWNED_LOCAL_DECL.match(ln)
        if not dm:
            continue
        indent, kind, elem, var, rhs = dm.groups()
        hit = sorted(set(_CALL_NAME.findall(rhs)) & owned)
        if not hit:
            continue
        if not _elements_not_retained(var, '\n'.join(lines[idx + 1:])):
            continue
        out.append("%sJ2X_ScopedFreeElems<%s<%s*> > _j2xfree_%s(%s);   // J2X no-gc-return-leak: free the "
                   "heap elements %s() handed back (caller-frees; Java GC'd them)"
                   % (indent, kind, elem, var, var, '/'.join(hit)))
    return out


_RECV_PREFIX = re.compile(r'(?:[A-Za-z_]\w*\s*(?:->|\.|::)\s*)+$')   # `this->bb->` before a method name


def _enclosing_call(line, inner_start):
    """(outer_method_name, arg_position) of the call whose argument list DIRECTLY contains inner_start,
    or None if inner_start isn't inside a call's parens. Scans left, tracking paren depth."""
    depth, commas, i = 0, 0, inner_start - 1
    while i >= 0:
        ch = line[i]
        if ch == ')':
            depth += 1
        elif ch == '(':
            if depth == 0:
                mm = re.search(r'([A-Za-z_]\w*)\s*$', line[:i])
                return (mm.group(1), commas) if mm else None
            depth -= 1
        elif ch == ',' and depth == 0:
            commas += 1
        i -= 1
    return None


def _hoist_owned_scalar_args(lines, resolver):
    """Post-pass (Pattern B): an owned-scalar builder call used INLINE as an argument -- `getCubes(bb->
    expand(...))` -- leaks the temp. Hoist it to a local + RAII delete, but ONLY when the receiving method
    is in-corpus and provably just reads that argument position (never retains it). Anything uncertain is
    left untouched (the leak stays flagged) rather than risk a dangling free."""
    cat = resolver.own_catalog
    owned = cat.scalar_methods
    if not owned:
        return lines
    corpus, retained = cat.corpus_methods, cat.retained_params
    owned_re = re.compile(r'(?<![\w])(%s)\s*\(' % '|'.join(re.escape(n) for n in owned))
    out, counter = [], [0]
    for ln in lines:
        indent = re.match(r'\s*', ln).group(0)
        pre = []
        for _ in range(8):                                        # bounded: at most a few owned temps / line
            mm = owned_re.search(ln)
            if not mm:
                break
            nm = mm.group(1)
            close = _balanced_paren(ln, ln.index('(', mm.start()))
            args_end = close + 1 if close >= 0 else -1
            rp = _RECV_PREFIX.search(ln[:mm.start()])
            expr_start = rp.start() if rp else mm.start()
            enc = _enclosing_call(ln, expr_start)
            if args_end < 0 or not enc:
                break                                             # captured (not an arg) / malformed -> stop
            outer, pos = enc
            if outer not in corpus or pos in retained.get(outer, set()):
                break                                             # receiver may retain the arg -> unsafe
            counter[0] += 1
            tmp, elem, expr = '_j2xarg_%d' % counter[0], owned[nm], ln[expr_start:args_end]
            pre.append('%s%s* %s = %s;' % (indent, elem, tmp, expr))
            pre.append("%sJ2X_ScopedDelete<%s > _j2xargd_%d(%s);   // J2X no-gc-arg-leak: free the heap %s "
                       "that %s() returned (%s() only reads it)" % (indent, elem, counter[0], tmp, elem, nm, outer))
            ln = ln[:expr_start] + tmp + ln[args_end:]
        out.extend(pre)
        out.append(ln)
    return out


# No-GC ownership cleanup passes over the translated body, in order. Each takes (lines, resolver) and
# returns lines; Pattern B (hoist owned-scalar args) must run before Pattern A (free owned-return
# containers) so the container guard sees the rewritten call. Add a pass here to grow the family.
_OWNERSHIP_POSTPASSES = (_hoist_owned_scalar_args, _free_owned_returns)


def _free_owned_reassigns(lines, owned_scalar):
    """Reassigning an OWNED scalar pointer member leaks the previous object (Java GC'd it; the 360 does
    not). Prepend `delete this->m;` before each `this->m = new .../factory` -- safe because every member
    is NULL-initialized in the constructor's member-init list, and delete-of-NULL is a no-op. Idempotent:
    skips a reassign already preceded by that delete (a hand-written one, or the raycast template's)."""
    if not owned_scalar:
        return lines
    pat = re.compile(r'^(\s*)this->(%s)\s*=\s*(?:new\s|[A-Za-z_][\w:]*::(?:create|make|alloc|clone)[A-Za-z]\w*\s*\()'
                     % '|'.join(re.escape(n) for n in owned_scalar))
    out = []
    for ln in lines:
        m = pat.match(ln)
        if m:
            already = bool(out) and out[-1].strip() == ('delete this->%s;' % m.group(2))
            if not already:
                out.append('%sdelete this->%s;   // J2X no-gc-reassign-leak: free the previous owned %s'
                           % (m.group(1), m.group(2), m.group(2)))
        out.append(ln)
    return out


def _render_method(cls, m, resolver, ledger):
    ret = ownership.return_cpp_type(m, resolver)
    params = ownership.params_str(m, resolver)
    name = resolver.renamed_method(cls.name, m.name)   # field/method name-collision -> name_
    L = ['%s %s::%s(%s)' % (ret, cls.name, name, params), '{']
    body = list(translate_body(m, cls, resolver, ledger))
    for _pass in _OWNERSHIP_POSTPASSES:
        body = _pass(body, resolver)
    # delete-before-reassign for this class's owned SCALAR pointer members (the Entity.bb / setPos leak)
    owned_scalar = [n for n, is_arr in ownership.owned_pointers(cls, resolver).items() if not is_arr]
    body = _free_owned_reassigns(body, owned_scalar)
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


def _enum_const_arg(expr, resolver, cls_name):
    """Translate one enum-constant constructor argument (e.g. `Material.air`, `IMob.class`,
    `"Kebab"`, `16`). Reuses the static-initializer ref translator, which maps `X.class` to a stable
    j2xClassOf(typeid(X)) Class*."""
    return _static_ref(expr, resolver, (), cls_name)


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
    own_statics = set(f.name for f in cls.static_fields())
    init = []
    for i, ec in enumerate(cls.enum_consts):
        args = ', '.join(_enum_const_arg(a.strip(), resolver, cls.name) for a in ec.args)
        nm = naming.enum_member(ec.name)
        init.append('%s = new %s(%s);' % (nm, cls.name, args))
        init.append('%s->__ordinal = %d;' % (nm, i))
    for f in cls.static_fields():
        if f.initializer:
            init.append('%s = %s;' % (f.name, _static_ref(f.initializer.strip(), resolver, own_statics, cls.name)))
    for block in getattr(cls, 'static_inits', []):
        init += _translate_block_lines(cls, resolver, ledger, _strip_block_braces(block), is_static=True)
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
    static_inits = getattr(cls, 'static_inits', [])
    init_body = list(static_init_lines)                  # deferred static FIELD initializers first
    for block in static_inits:                           # then the Java static{} block bodies
        init_body += _translate_block_lines(cls, resolver, ledger, _strip_block_braces(block), is_static=True)
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
