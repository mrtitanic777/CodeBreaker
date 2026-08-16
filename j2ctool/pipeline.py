# -*- coding: utf-8 -*-
"""
pipeline.py -- Module 5: the end-to-end converter.

  discover .java -> parse -> build resolver -> emit .h/.cpp -> verify -> report

This is the orchestrator that replaces the monolith's run_conversion(). It is
deliberately thin: every real decision lives in the modules it calls (parse,
types, emit), so this just sequences them and collects the ledger.

The verify step runs the crash-class rules (rules.py) over the *emitted* output
as an acceptance check -- a correct generator should produce ~zero findings
(arrays get `new T[n]()`, owners get dtor+disabled-copy, etc.), in contrast to
the legacy output.
"""

import os
import re

from . import parse, types, typemap, report, rules, runtime
from .emit import header as emit_header, source as emit_source
from .ledger import Ledger, Category, Severity


# Set True by convert() for platforms where a C++ `throw` is a no-op that does not unwind (xbox360),
# so _finalize_emitted prefixes every emitted throw with a location tracer (see JavaRT.h).
_TRACE_THROWS = False

# Set True by convert() for the Xbox 360: the console runs NO OpenGL, so every OpenGL call in the
# converted game is converted to the native Direct3D 9 layer (namespace d3d9). GL11:: / GLU:: are the
# OpenGL API namespaces; the registry (apimap/opengl.json + glu.json) is the authoritative GL->D3D9
# conversion table, and this pass applies it -- rewriting the call sites so the emitted code contains
# Direct3D 9, not OpenGL. (The runtime GL11/GLU names are transition aliases of d3d9, removed later.)
_GL_TO_D3D9 = False

# OpenGL API namespaces that the registry maps to the Direct3D 9 layer 'd3d9'. Sourced from the registry
# languages catalog (kind == 'api', opengl-family) at convert() time; this default covers GL11/GLU.
_GL_NAMESPACES = ('GL11', 'GLU')


def _convert_gl_to_d3d9(text):
    """Rewrite OpenGL call sites (GL11::glX, GLU::gluX, GL11::GL_CONST) in emitted game code to the
    Direct3D 9 layer namespace 'd3d9'. Registry-validated: every GL symbol converted is one the
    OpenGL->Direct3D9 table maps (see apimap/opengl.json + glu.json). Only the `Ns::` qualified form is
    touched -- `#include "GL11.h"` filenames are left intact."""
    import re as _re
    pat = _re.compile(r'\b(?:' + '|'.join(_re.escape(n) for n in _GL_NAMESPACES) + r')::')
    return pat.sub('d3d9::', text)


def _trace_guest_throws(text):
    """Prefix every LINE-LEADING `throw` statement with a separate `J2X_GUEST_THROW();` statement so a
    fired throw names its file:line. Deliberately robust-by-simplicity: it inserts a whole statement
    BEFORE the throw and never touches the throw text itself, so it cannot be fooled by a `;` inside a
    string literal (`throw new X("]);")`), a multi-line throw expression, or nested parens -- the three
    things that break any "find the statement end" approach. Only throws that begin their line are
    instrumented (the converter emits braced blocks, so essentially all throws are line-leading);
    a mid-line `if(c) throw ...;` is left alone rather than risk mangling it. `throws` in a method
    signature is excluded (the char after `throw` must be whitespace or `;`, not `s`)."""
    out = []
    for ln in text.split('\n'):
        s = ln.lstrip()
        if 'J2X_GUEST_THROW' in ln or not s.startswith('throw'):
            out.append(ln); continue
        after = s[5:6]                        # char right after `throw`
        if after not in (' ', '\t', ';'):     # 'throw <expr>' or bare 'throw;' only (not 'throws')
            out.append(ln); continue
        indent = ln[:len(ln) - len(s)]
        out.append(indent + 'J2X_GUEST_THROW(); ' + s)
    return '\n'.join(out)


def _finalize_emitted(text):
    """Global text finalizers applied to EVERY emitted .h/.cpp (so they also catch source.py-emitted
    static-data arrays and inlined ctor bodies, not just method-body lines):
      - enum-constant refs colliding with macros (EnumX::NULL -> EnumX::NULL_)
      - the invalid `'\\u0000'` C++ char literal -> the standard `'\\0'`
      - `new X(...)->m` -> `(new X(...))->m` (C++ precedence: the member binds to the call otherwise)
      - `"lit".method()` -> `JavaString("lit").method()` (a bare literal has no Java String methods)"""
    from .emit.std_transforms import sanitize_reserved_members
    from .emit.statements import _wrap_new_alloc_chains, _wrap_literal_method, _paren_shift_operands
    text = sanitize_reserved_members(text)
    if _GL_TO_D3D9:
        text = _convert_gl_to_d3d9(text)
    if _TRACE_THROWS:
        text = _trace_guest_throws(text)   # tag every throw with its site (throws are no-op landmines here)
    text = text.replace(chr(39) + chr(92) + 'u0000' + chr(39), chr(39) + chr(92) + '0' + chr(39))
    # `new Object()` (a bare java.lang.Object, used only as a monitor/lock) -> a JObject VALUE. Object
    # has no C++ class; the universal box default-constructs, and the sync object is never used on the port.
    text = re.sub(r'\bnew\s+Object\s*\(\s*\)', 'JObject()', text)
    # `arr->length` -> `arr.length`: `length` (no parens) is a field on JArray ONLY, always a value, so a
    # pointer deref of it is a cross-class member-env miss (a byte[] field reached through a typed pointer,
    # e.g. RenderEngine's var2->imageData->length). `->length(` (a method) and `->lengthX` are untouched.
    text = re.sub(r'->length\b(?!\s*\()', '.length', text)
    # `a - 3 >> 4` -> `(a - 3) >> 4`: parenthesize a shift's arithmetic operand (MSVC C4554). Behaviour-
    # preserving (Java/C++ agree additive/multiplicative binds tighter than shift); only silences the warn.
    return '\n'.join(_paren_shift_operands(_wrap_literal_method(_wrap_new_alloc_chains(ln)))
                     for ln in text.split('\n'))


def discover(java_dir):
    """All .java under java_dir, de-duplicated by basename (output is keyed by class name)."""
    paths = {}
    for dp, _dirs, fs in os.walk(java_dir):
        for f in fs:
            if f.endswith('.java') and f not in paths:
                paths[f] = os.path.join(dp, f)
    return [paths[k] for k in sorted(paths)]


def drop_synthetic_artifacts(classes, ledger=None):
    """Drop MCP decompiler artifacts that exist only to satisfy Java access rules and would emit
    noise / a false ctor-stub / an uninitialized-object trap in C++:
      * empty MARKER classes (`class Empty1 {}`) -- no fields, methods, parent or interfaces;
      * synthetic BRIDGE constructors `Cls(EmptyN var1) { this(); }` -- a ctor taking exactly one
        marker-typed param (these delegate to the real private ctor and are never meaningfully called).
    EmptyChunk (a real Chunk subclass) and NoiseGenerator (an empty-in-source abstract BASE class)
    are NOT markers, so they are untouched. Returns the kept classes."""
    # names used as a base -> never droppable (NoiseGenerator is `class NoiseGenerator {}` but is
    # extended by NoiseGeneratorOctaves/Perlin).
    used_as_base = set()
    for c in classes:
        if c.parent:
            used_as_base.add(c.parent)
        used_as_base.update(c.interfaces)
    # sole-parameter constructor types -> the bridge-ctor signal
    bridge_param_types = set(m.params[0].jtype.name for c in classes for m in c.methods
                             if m.is_ctor and len(m.params) == 1)
    # a marker is EMPTY (no members/parent/interfaces), never a base, AND used only as a bridge param
    markers = set(c.name for c in classes
                  if not c.is_enum and not c.is_interface
                  and not c.fields and not c.methods and not c.parent and not c.interfaces
                  and c.name not in used_as_base and c.name in bridge_param_types)
    if not markers:
        return classes, set()
    dropped_ctors = 0
    for c in classes:
        kept = []
        for m in c.methods:
            if m.is_ctor and len(m.params) == 1 and m.params[0].jtype.name in markers:
                dropped_ctors += 1
                continue                                  # synthetic bridge ctor -> drop
            kept.append(m)
        c.methods = kept
    result = [c for c in classes if c.name not in markers]
    if ledger is not None:
        ledger.add('(pipeline)', Category.PORTED,
                   'dropped %d empty marker class(es) [%s] + %d synthetic bridge constructor(s)'
                   % (len(markers), ', '.join(sorted(markers)), dropped_ctors),
                   source='pipeline', rule='drop-synthetic')
    return result, markers


def resolve_inheritance(classes):
    """Populate each class's inherited_fields by walking its parent chain across the
    full class set. Instance fields only; private parent fields are skipped (a subclass
    can't reference them); own/closer fields shadow farther ones."""
    by_name = dict((c.name, c) for c in classes)
    for cls in classes:
        seen = set(f.name for f in cls.fields)
        inherited = []
        parent = cls.parent
        guard = set([cls.name])
        while parent and parent in by_name and parent not in guard:
            guard.add(parent)
            pc = by_name[parent]
            for f in pc.fields:
                if not f.is_static and f.access != 'private' and f.name not in seen:
                    inherited.append(f)
                    seen.add(f.name)
            parent = pc.parent
        cls.inherited_fields = inherited


def compute_mi_forwards(classes):
    """MULTIPLE-INHERITANCE diamond fix. Java lets ONE method satisfy both an interface's
    declaration and a base class's concrete impl (e.g. TileEntityChest: TileEntity declares a
    concrete onInventoryChanged(), IInventory declares it abstractly). In C++ those are distinct
    vtable slots: the interface's pure-virtual stays unimplemented -> the class is abstract AND
    `obj->onInventoryChanged()` is ambiguous. Fix: synthesize, in the derived class, a forwarding
    override `R m(args){ return Base::m(args); }` that unifies both slots.

    Attaches `cls.mi_forwards = [(Method, base_class_name), ...]`. Method is the interface's
    declaration (signature source); base_class_name is the ancestor whose concrete impl to call."""
    by_name = dict((c.name, c) for c in classes)

    def iface_methods(cls):                          # all methods from implemented interfaces (transitive)
        out, stack, seen = {}, list(cls.interfaces), set()
        while stack:
            nm = stack.pop()
            if nm in seen:
                continue
            seen.add(nm)
            ic = by_name.get(nm)
            if ic is None:
                continue
            for m in ic.methods:
                if not m.is_ctor:
                    out.setdefault(m.name, m)
            stack.extend(ic.interfaces)
            if ic.parent:
                stack.append(ic.parent)
        return out

    for cls in classes:
        cls.mi_forwards = []
        if cls.is_interface or cls.is_enum or not cls.interfaces:
            continue
        own = set(m.name for m in cls.methods if not m.is_ctor)
        # nearest concrete impl from the (non-interface) base-class chain -> (class, arity)
        base_concrete, cur, guard = {}, by_name.get(cls.parent), set()
        while cur and cur.name not in guard:
            guard.add(cur.name)
            if not cur.is_interface:
                for m in cur.methods:
                    if not m.is_ctor and not m.is_abstract:
                        base_concrete.setdefault(m.name, (cur.name, m.arity))
            cur = by_name.get(cur.parent)
        for name, im in iface_methods(cls).items():
            if name in own or name not in base_concrete:
                continue                             # C overrides it, or no inherited concrete impl
            base_cls, base_arity = base_concrete[name]
            if base_arity != im.arity:               # same name, different signature -> not the diamond
                continue
            cls.mi_forwards.append((im, base_cls))


class _StubMethod(object):
    """Lightweight method record for a runtime-stub class, shaped like a parsed Method so the
    member-env flatten can treat stub and corpus classes uniformly."""
    is_ctor = False
    is_static = False
    def __init__(self, name, return_type):
        self.name = name
        self.return_type = return_type


class _StubClass(object):
    """Lightweight class record for a runtime-stub class (see runtime.STUB_MEMBERS). Only the
    attributes the member-env flatten reads (name/parent/fields/methods) are provided."""
    is_enum = False
    is_interface = False
    def __init__(self, name, parent, methods):
        self.name = name
        self.parent = parent
        self.fields = []
        self.methods = methods


def _stub_return_jtype(cpp):
    """A JType for a runtime-stub method's C++ return spelling, ONLY when it is unambiguous enough
    to drive chain inference: a single reference-type pointer (`File*` -> File) or `std::string`
    (-> String, a value). Everything else (void*, JArray<...>, primitives) returns None -- the
    chain resolver then conservatively leaves `.` untouched rather than guess."""
    from .ir import JType as _JT
    cpp = cpp.strip()
    if cpp == 'std::string':
        return _JT('String')
    m = re.match(r'^([A-Za-z_]\w*)\s*\*$', cpp)
    if m and m.group(1) != 'void':
        return _JT(m.group(1))
    return None


def build_member_env(classes):
    """Flatten each class's fields (static + instance) and method return types over its
    parent chain into {class: {'fields': {name: JType}, 'methods': {name: JType}}}.
    Closer (more-derived) definitions win. Feeds expression-type inference so chained
    `recv.field.member` / `Class::s_field.member` get '.' vs '->' right by construction."""
    by_name = dict((c.name, c) for c in classes)
    # Runtime-stub classes (URL/File/Runtime/Applet/Node/...) aren't in the parsed corpus, but their
    # method result types must resolve chains through them (`getDocumentBase()->getHost()`, a corpus
    # class extending Applet inheriting getDocumentBase, etc.). Adopt them as class-like records so the
    # SAME flatten handles corpus->stub inheritance and stub->stub bases uniformly. Corpus names win.
    from . import runtime
    stub_classes = []
    for name, rec in runtime.STUB_MEMBERS.items():
        if name in by_name:
            continue
        methods = []
        for mname, cpp in rec['methods'].items():
            jt = _stub_return_jtype(cpp)
            if jt is not None:
                methods.append(_StubMethod(mname, jt))
        stub = _StubClass(name, rec.get('parent'), methods)
        by_name[name] = stub
        stub_classes.append(stub)
    env = {}
    for cls in list(classes) + stub_classes:
        fields, methods, statics = {}, {}, set()
        cur, guard = cls, set()
        while cur and cur.name not in guard:         # most-derived first; don't overwrite
            guard.add(cur.name)
            for f in cur.fields:
                fields.setdefault(f.name, f.jtype)
                if f.is_static:
                    statics.add(f.name)              # own + inherited statics (for bare s_ prefix)
            for m in cur.methods:
                if not m.is_ctor and m.return_type is not None:
                    methods.setdefault(m.name, m.return_type)
            cur = by_name.get(cur.parent)
        env[cls.name] = {'fields': fields, 'methods': methods, 'statics': statics, 'parent': cls.parent}
    # Runtime types that aren't in the parsed corpus but whose chained methods must resolve
    # `.`->`->` (StringBuilder.append() returns the builder). Return types drive chain inference.
    from .ir import JType as _JT
    env['StringBuilder'] = {'fields': {}, 'statics': set(), 'methods': {
        'append': _JT('StringBuilder'), 'insert': _JT('StringBuilder'),
        'deleteCharAt': _JT('StringBuilder'), 'reverse': _JT('StringBuilder'),
        'toString': _JT('String'), 'charAt': _JT('char'), 'length': _JT('int')}}
    return env


def build_method_renames(classes):
    """{class: set(method names that COLLIDE with a same-named field, over the parent chain)}.
    Java allows a field and method with the same name (e.g. synthetic accessors like
    NetworkManager.isRunning); C++ does not, so emit renames the method to `name_` everywhere
    (declaration + all call sites). Flattened over inheritance so a subclass call to an
    inherited colliding method renames too."""
    by_name = dict((c.name, c) for c in classes)
    own = {}
    for c in classes:
        fields = set(f.name for f in c.fields)
        own[c.name] = set(m.name for m in c.methods if not m.is_ctor and m.name in fields)
    renames = {}
    for c in classes:
        s, cur, guard = set(), c, set()
        while cur and cur.name not in guard:
            guard.add(cur.name)
            s |= own.get(cur.name, set())
            cur = by_name.get(cur.parent)
        renames[c.name] = s
    return renames


def compute_thread_like(classes):
    """Names of classes that are (transitively) a Thread subclass or Runnable -- i.e. an
    object whose `.start()`/`.run()` runs ASYNCHRONOUSLY and must OUTLIVE the call site. These must
    NEVER be stack-allocated by the leak transform (the object outliving the thread is a use-after-free).
    Seeded with the external roots Thread/Runnable (not in the parsed corpus)."""
    by_name = dict((c.name, c) for c in classes)
    thread_like = set()
    for cls in classes:
        cur, guard = cls, set()
        while cur and cur.name not in guard:
            guard.add(cur.name)
            if cur.parent in ('Thread', 'Runnable') or 'Runnable' in cur.interfaces or 'Thread' in cur.interfaces:
                thread_like.add(cls.name)
                break
            cur = by_name.get(cur.parent)
    return thread_like


_RETURN_NULL = re.compile(r'\breturn\s+null\s*;')

# Java collection mutators. Calling one of these ON a collection PARAM mutates the CALLER's collection
# (Java passes the reference), so that param is an out / in-out param. The converter models collections as
# value structs (JList<T>), so a by-value param silently COPIES -- the callee's add()/remove()/put() are
# lost. That is the getCollidingBoundingBoxes "player falls through the world" class of bug.
_COLL_MUTATORS = (
    'add', 'addAll', 'addElement', 'addFirst', 'addLast', 'insertElementAt',
    'set', 'setElementAt', 'replaceAll', 'sort',
    'remove', 'removeAll', 'removeElement', 'removeElementAt', 'removeAllElements',
    'removeFirst', 'removeLast', 'retainAll', 'clear',
    'put', 'putAll', 'putIfAbsent', 'merge', 'replace',
    'push', 'pop', 'offer', 'poll',
)


def _iter_call_args(body, name, arity):
    """Yield (arg_index, arg_text) for every call to `name(...)` in `body` (raw Java) whose top-level
    argument count == arity. Paren-depth aware so nested calls don't split the arg list. Used to see when a
    method FORWARDS one of its params into another method's out-param slot."""
    pat = re.compile(r'\b' + re.escape(name) + r'\s*\(')
    for mt in pat.finditer(body):
        k = mt.end() - 1                       # index of the opening '('
        depth = 0
        args, cur = [], []
        n = len(body)
        while k < n:
            ch = body[k]
            if ch == '(':
                depth += 1
                if depth == 1:
                    k += 1
                    continue
            elif ch == ')':
                depth -= 1
                if depth == 0:
                    args.append(''.join(cur))
                    break
            if depth == 1 and ch == ',':
                args.append(''.join(cur))
                cur = []
                k += 1
                continue
            if depth >= 1:
                cur.append(ch)
            k += 1
        args = [a.strip() for a in args]
        if len(args) == 1 and args[0] == '':
            args = []                          # zero-arg call
        if len(args) == arity:
            for idx, a in enumerate(args):
                yield idx, a


def compute_collection_out_params(classes, resolver):
    """WHOLE-PROGRAM pass: mark every collection-typed value param that its method mutates (an out-param)
    with Param.by_ref, so the emitter passes it by C++ reference instead of copying it. Three signals,
    unioned to a fixpoint:
      1. DIRECT      -- the body calls a mutator on the param (`var6.add(...)`).
      2. OVERRIDE    -- virtual override signatures MUST match: if any member of an override family has
                        position i out, all do (else an override that only forwards to super -- e.g.
                        BlockStairs.getCollidingBoundingBoxes -- wouldn't match Block's `&`).
      3. FORWARDING  -- the param is passed as an argument into another method's out-param slot
                        (World.getEntitiesWithinAABBForEntity -> Chunk fills the same list).
    Read-only collection params are LEFT by value (a safe copy) -- only mutated ones flip to `&`, which
    keeps the change minimal and avoids binding rvalue arguments to a non-const reference.
    Returns the number of params marked."""
    def is_coll_value_param(p):
        return (p.jtype is not None and not p.jtype.is_array
                and p.jtype.name in typemap.CONTAINERS)

    methods = [(c, m) for c in classes for m in c.methods]
    mut_re = re.compile(r'\.\s*(?:' + '|'.join(_COLL_MUTATORS) + r')\s*\(')

    # inheritance closure (parent chain + interfaces) -> relate override-family members
    by_name = dict((c.name, c) for c in classes)

    def ancestors(cname):
        seen, stack = set(), [cname]
        while stack:
            cur = by_name.get(stack.pop())
            if not cur:
                continue
            for par in ([cur.parent] if cur.parent else []) + list(cur.interfaces or []):
                if par and par not in seen:
                    seen.add(par)
                    stack.append(par)
        return seen

    anc = dict((c.name, ancestors(c.name)) for c in classes)

    def related(n1, n2):
        return n1 == n2 or n2 in anc.get(n1, ()) or n1 in anc.get(n2, ())

    out_pos = {}                               # id(method) -> set of out positions
    body_cache = {}
    for c, m in methods:
        body_cache[id(m)] = '\n'.join(t for _l, t in m.body)

    # (1) seed: direct mutation of a collection param in the body
    for c, m in methods:
        s = out_pos.setdefault(id(m), set())
        body = body_cache[id(m)]
        for i, p in enumerate(m.params):
            if not is_coll_value_param(p):
                continue
            if re.search(r'\b' + re.escape(p.name) + r'\b' + mut_re.pattern, body):
                s.add(i)

    by_key = {}                                # (name, arity) -> [(cls, method)]
    for c, m in methods:
        by_key.setdefault((m.name, m.arity), []).append((c, m))

    # (2)+(3) propagate to a fixpoint
    changed = True
    while changed:
        changed = False
        # override-family union
        for (name, arity), group in by_key.items():
            if len(group) < 2:
                continue
            for i in range(arity):
                holders = [(c, m) for (c, m) in group
                           if i < len(m.params) and is_coll_value_param(m.params[i])]
                if len(holders) < 2:
                    continue
                hot = [(c, m) for (c, m) in holders if i in out_pos.get(id(m), ())]
                for (c1, m1) in hot:
                    for (c2, m2) in holders:
                        if related(c1.name, c2.name) and i not in out_pos.get(id(m2), ()):
                            out_pos.setdefault(id(m2), set()).add(i)
                            changed = True
        # forwarding: param passed into a callee's out slot
        name_out = {}                          # (name, arity) -> set of out positions (unioned)
        for c, m in methods:
            for i in out_pos.get(id(m), ()):
                name_out.setdefault((m.name, m.arity), set()).add(i)
        for c, m in methods:
            todo = [i for i, p in enumerate(m.params)
                    if is_coll_value_param(p) and i not in out_pos.get(id(m), ())]
            if not todo:
                continue
            body = body_cache[id(m)]
            pname_at = dict((m.params[i].name, i) for i in todo)
            for (callname, callarity), outposs in name_out.items():
                if callname == m.name and callarity == m.arity:
                    pass                       # self/override forwarding still valid (super.same(...))
                for argi, argtext in _iter_call_args(body, callname, callarity):
                    if argi in outposs and argtext in pname_at:
                        i = pname_at[argtext]
                        if i not in out_pos.get(id(m), ()):
                            out_pos.setdefault(id(m), set()).add(i)
                            changed = True

    # finalize -> Param.by_ref
    count = 0
    for c, m in methods:
        for i in out_pos.get(id(m), ()):
            if i < len(m.params) and not m.params[i].by_ref:
                m.params[i].by_ref = True
                count += 1
    return count


def derive_nullability(classes, resolver):
    """DERIVE nullable getters from the corpus (retires reliance on the Alpha-curated whitelist):
    a method returning a REFERENCE type whose body can `return null;` can hand back NULL, so
    `getX()->y` on it is a NULL-deref crash on the 360 and must be guarded -- on ANY corpus, not just
    Alpha. High-precision (explicit `return null` + pointer return only), so over-guarding is bounded;
    a spurious guard is at worst a benign extra null-check. Returns (getter_names, {getter: retType})."""
    getters, gtypes = set(), {}
    for c in classes:
        for m in c.methods:
            rt = m.return_type
            if m.is_ctor or rt is None or rt.is_array:
                continue
            if rt.name in typemap.PRIMITIVES or rt.name == 'String' or rt.name in typemap.CONTAINERS:
                continue                                  # value return can't form getter()->deref
            if not resolver.is_pointer(rt):
                continue
            body = '\n'.join(t for _ln, t in m.body)
            if _RETURN_NULL.search(body):
                getters.add(m.name)
                gtypes.setdefault(m.name, rt.name)
    return getters, gtypes


def build_resolver(classes, value_types=None):
    names = set(c.name for c in classes)
    enums = set(c.name for c in classes if c.is_enum)
    typed_enums = set(c.name for c in classes if c.is_enum and c.is_typed_enum())
    ifaces = set(c.name for c in classes if c.is_interface)
    vals = set(value_types if value_types is not None else typemap.DEFAULT_VALUE_TYPES)
    return types.TypeResolver(known_classes=names, value_types=vals,
                             interface_types=ifaces, enum_types=enums,
                             typed_enum_types=typed_enums,
                             members=build_member_env(classes),
                             method_renames=build_method_renames(classes),
                             thread_like=compute_thread_like(classes))



class CppStandard:
    """Lightweight descriptor for the target C++ standard revision."""
    def __init__(self, year):
        self.year = year
        self.label = 'C++%02d' % (year % 100)
    @property
    def has_nullptr(self):
        return self.year >= 2011
    @property
    def has_auto(self):
        return self.year >= 2011
    @property
    def has_range_for(self):
        return self.year >= 2011
    @property
    def has_override(self):
        return self.year >= 2011
    @property
    def has_initializer_list(self):
        return self.year >= 2011
    @property
    def has_enum_class(self):
        return self.year >= 2011
    @property
    def has_constexpr(self):
        return self.year >= 2011
    @property
    def has_structured_bindings(self):
        return self.year >= 2017
    @property
    def has_optional(self):
        return self.year >= 2017
    @property
    def has_string_view(self):
        return self.year >= 2017


_STD_MAP = {
    'C++98': 1998, 'C++03': 2003, 'C++11': 2011, 'C++14': 2014,
    'C++17': 2017, 'C++20': 2020, 'C++23': 2023,
}

def _parse_std_version(label):
    return CppStandard(_STD_MAP.get(label, 2003))


def convert(java_dir, out_dir, value_types=None, ledger=None, write_report=True,
            cpp_std='C++03', platform=None, progress_cb=None, platform_refs=False,
            sdk_override=None, emit_debug=True, overlay_dir=None):
    """Convert a Java source tree to C++. Returns a summary dict.

    cpp_std: target C++ standard ('C++98', 'C++03', 'C++11', 'C++14', 'C++17', 'C++20', 'C++23').
    platform: target platform id ('xbox360', 'pc_windows', 'ps3', 'wii', etc.) for stub annotations.
    platform_refs: if True, emit the real hand-written platform layer from docs/platform/reference/
                   (GL->D3D9 emulator, audio, input, socket) instead of the no-op stubs.
    sdk_override: explicit SDK root; else auto-detected. A console platform without a located SDK is
                  REFUSED (the converter ships no SDK headers/signatures).
    progress_cb: optional callable(str) invoked with per-class progress lines."""
    ledger = ledger if ledger is not None else Ledger()
    std_version = _parse_std_version(cpp_std)
    _prog = progress_cb or (lambda _: None)
    os.makedirs(out_dir, exist_ok=True)
    # The 360 target ships a COMPLETE, self-contained platform layer by default (entry point, GL->D3D9
    # emulator, shaders, textures, audio, input, file IO) -- the user never hand-writes/copies it. Opt
    # out is still possible by passing platform_refs=False explicitly for a non-xbox target.
    if platform == 'xbox360':
        platform_refs = True

    # On the 360 emulator a C++ `throw` is a no-op that doesn't unwind -> tag every emitted throw with
    # its site (JavaRT.h's J2X_GUEST_THROW) so a fired landmine names its file:line in the debug log.
    global _TRACE_THROWS, _GL_TO_D3D9
    _TRACE_THROWS = (platform == 'xbox360')
    # The 360 runs NO OpenGL: convert every GL call in the emitted game to the Direct3D 9 layer (d3d9).
    _GL_TO_D3D9 = (platform == 'xbox360')

    # SDK GATE: refuse platform-specific conversion for a console target with no SDK on the machine.
    from . import sdk as _sdk
    sdk_info = None
    if _sdk.requires_sdk(platform):
        sdk_info = _sdk.require(platform, override=sdk_override)   # raises SdkNotFound -> caller reports
        _prog('[sdk] %s located at %s (via %s)' % (sdk_info.name, sdk_info.root, sdk_info.source))

    _prog('[config] target C++ standard: %s (year %d)' % (std_version.label, std_version.year))
    _prog('[parse] discovering .java files...')
    java_paths = discover(java_dir)
    _prog('[parse] found %d .java files' % len(java_paths))

    classes = []
    for i, path in enumerate(java_paths):
        try:
            classes.append(parse.parse_file(path))
        except Exception as e:                       # noqa: BLE001 - one bad file shouldn't abort
            _prog('[parse] FAIL %s: %s' % (os.path.basename(path), e))
            ledger.add(os.path.basename(path), Category.ERROR, 'parse failed: %s' % e,
                       severity=Severity.HIGH, source='pipeline', rule='parse-error')
    _prog('[parse] parsed %d classes OK' % len(classes))

    before = len(classes)
    classes, markers = drop_synthetic_artifacts(classes, ledger)
    if len(classes) != before:
        _prog('[parse] dropped %d empty marker class(es) + synthetic bridge ctors' % (before - len(classes)))

    _prog('[resolve] inheritance + MI forwards + type resolver...')
    resolve_inheritance(classes)
    compute_mi_forwards(classes)
    resolver = build_resolver(classes, value_types)
    resolver.markers = markers                            # dropped EmptyN markers -> strip their call-site args
    _prog('[resolve] done (%d known classes, %d interfaces, %d enums)'
          % (len(resolver.known), len(resolver.interfaces), len(resolver.enums)))

    # Java collection out-params -> C++ references. A method that MUTATES a List/Map/Set param writes the
    # caller's collection; modelled as a value struct it would copy instead, silently dropping the writes
    # (the getCollidingBoundingBoxes collision-dead bug). Mark them so the emitter passes them by reference.
    _coll_ref = compute_collection_out_params(classes, resolver)
    _prog('[resolve] %d collection out-param(s) promoted to by-reference' % _coll_ref)

    # DERIVE nullable getters from the corpus and union into the guarded set for THIS conversion, so
    # emit-time null-guarding + the null-deref rule cover getters that can `return null` on any corpus
    # (not just the Alpha whitelist). Snapshot/restore keeps the module globals clean across calls.
    from .emit import statements as _statements
    _base_getters = set(typemap.KNOWN_NULLABLE_GETTERS)
    _base_gtypes = dict(typemap.NULLABLE_GETTER_RETURN_TYPE)
    d_getters, d_gtypes = derive_nullability(classes, resolver)
    typemap.KNOWN_NULLABLE_GETTERS |= d_getters
    for _k, _v in d_gtypes.items():
        typemap.NULLABLE_GETTER_RETURN_TYPE.setdefault(_k, _v)
    _statements.refresh_nullable_getters()
    _prog('[nullability] derived %d additional nullable getter(s) from the corpus'
          % len(d_getters - _base_getters))

    plat_label = None
    if platform:
        from .platforms import get_platform
        plat_info = get_platform(platform)
        plat_label = plat_info['label'] if plat_info else platform
        _prog('[config] target platform: %s' % plat_label)

    rt_files = runtime.generate_files(cpp_std=std_version, platform_id=platform)
    _prog('[runtime] writing %d support headers...' % len(rt_files))
    for fname, content in rt_files.items():
        _write(os.path.join(out_dir, fname), content)

    # optionally REPLACE the platform stubs with the real hand-written implementations (GL->D3D9
    # emulator, XACT audio, XInput, XNet socket, ...) from docs/platform/reference/. The port stops
    # being blind: it ships the proven Alpha platform layer; PLATFORM_COVERAGE.md reports the delta.
    if platform_refs:
        from . import platform_refs as _pref
        from ._paths import data_path as _data_path
        _ref_dir0 = _data_path('docs', 'platform', 'reference')
        # DETECTION-DRIVEN (no game hardcoding): tell the platform emitter which classes the game
        # defines itself + its main class, so it skips any platform file that collides with a
        # game-defined class and generates a generic entry point instead of the Beta-specific one.
        _game_classes = set(c.name for c in classes)
        _main_class = _pref.detect_main_class(classes)
        emitted = _pref.emit_references(out_dir, _ref_dir0,
                                        game_classes=_game_classes, main_class=_main_class)
        _prog('[platform] emitted %d platform references (agnostic layer, replacing stubs): %s'
              % (len(emitted), ', '.join(sorted(emitted))))
        _skipped = getattr(_pref.emit_references, 'last_skipped', [])
        if _skipped:
            _prog('[platform] skipped %d platform file(s) the game defines itself (its converted class '
                  'wins): %s' % (len(_skipped), ', '.join(_skipped)))
        if _main_class:
            _prog('[platform] generic entry point -> %s::main (detected)' % _main_class)
        # INTEGRATION GUARD: never let a reference silently fail to emit again (the GLU flat-world bug --
        # a .reference.cpp with no matching .reference.h). Audit for orphans + VERIFY every reference's stub
        # was really replaced in the output; surface any gap LOUDLY.
        _audit = _pref.audit_references(_ref_dir0)
        _expect = sorted(b for b in _audit['complete']
                         if b not in _pref.REFERENCE_SKIP and b not in _game_classes)
        _vfail = _pref.verify_integrated(out_dir, _expect)
        if _audit['orphans']:
            _prog('[platform] !! ORPHAN REFERENCE(S) NOT INTEGRATED (have .reference.cpp but NO .reference.h '
                  '-> game keeps the no-op stub): %s  --> add the matching .reference.h'
                  % ', '.join(_audit['orphans']))
        if _vfail:
            _prog('[platform] !! REFERENCE(S) FAILED TO REPLACE THEIR STUB in the output: %s'
                  % ', '.join(_vfail))
        if not _audit['orphans'] and not _vfail:
            _prog('[platform] integration verified: all %d reference(s) replaced their stub' % len(_expect))

    headers = sources = 0
    init_classes = []                                # concrete classes whose initStatics() does real work
    total = len(classes)
    for idx, cls in enumerate(classes):
        tag = '[%d/%d] %s' % (idx + 1, total, cls.name)
        try:
            ev_before = len(ledger)
            _write(os.path.join(out_dir, cls.name + '.h'),
                   _finalize_emitted(emit_header.render_header(cls, resolver, ledger, cpp_std=std_version, platform_label=plat_label)))
            new_evs = ledger.events[ev_before:]
            if new_evs:
                for ev in new_evs:
                    if ev.category != Category.PORTED:
                        _prog('%s .h %s: %s' % (tag, ev.category, ev.message))
            else:
                _prog('%s .h generated' % tag)
            headers += 1

            ev_before = len(ledger)
            cpp = emit_source.render_source(cls, resolver, ledger, cpp_std=std_version, platform_label=plat_label)
            if cpp is not None:
                _write(os.path.join(out_dir, cls.name + '.cpp'), _finalize_emitted(cpp))
                sources += 1
                if getattr(cls, 'has_init_statics', False):
                    init_classes.append(cls.name)    # generated class with deferred static init
            new_evs = ledger.events[ev_before:]
            notable = [ev for ev in new_evs if ev.category != Category.PORTED]
            if notable:
                for ev in notable:
                    _prog('%s .cpp %s: %s' % (tag, ev.category, ev.message))
            else:
                _prog('%s .cpp generated (clean)' % tag)
        except Exception as e:                       # noqa: BLE001
            _prog('%s ERROR: %s' % (tag, e))
            ledger.add(cls.name, Category.ERROR, 'emit failed: %s' % e,
                       severity=Severity.HIGH, source='pipeline', rule='emit-error')

    # ORDERED STATIC-INIT DRIVER: a single j2x_initAll() that calls each class's initStatics() in
    # DEPENDENCY order (C++ gives no cross-TU static-init order -> the static-init-order crash class).
    _write_init_driver(out_dir, init_classes, classes)

    # reflection factories: Java class-registry reflection (getConstructor/newInstance -> NULL stub,
    # the entity-spawn / tile-load / packet-parse crash) becomes a real generated factory switch built
    # from the registration table, and reuse sites (BlockSign/SpawnerAnimals) call it via a shared
    # header. Runs before the rules scan so the reflection-unsupported findings drop.
    from . import reflection_factory
    _rf = reflection_factory.run(out_dir)
    if _rf['total_sites']:
        _prog('[reflect] %d registries -> factories (%d classes), %d sites rewritten (%d reused)'
              % (len(_rf['registries']), _rf['total_classes'], _rf['total_sites'], len(_rf['reused'])))

    # PROJECT OVERLAYS: re-apply game/version-specific hand-fixes to GENERATED files -- leak `delete`s in
    # specific methods, the scheduled-tick value-comparing std::set, UI tweaks (OSK/FPS/tooltip), input
    # remaps, and hand-written net files (Socket.cpp) -- that do NOT generalize into an emitter rule and
    # would otherwise be WIPED by a reconversion. Runs LAST over the generated tree (after emission +
    # references + reflection) so overlays are the final say; ADDED .cpp are compiled because vcxproj
    # enumerates out_dir afterward. See j2ctool/overlays.py. When an overlay turns out to be a general
    # pattern, migrate it into the emitter/runtime/reference and drop the overlay.
    from . import overlays as _overlays
    _ov_dir = _overlays.resolve_overlay_dir(out_dir, java_dir, explicit=overlay_dir)
    _applied = _overlays.apply_overlays(out_dir, _ov_dir, prog=_prog)
    if _applied:
        _prog('[overlay] %d project overlay file(s) re-applied from %s' % (len(_applied), _ov_dir))

    # KEYBIND MARKING (console): the game drives F1/F2/F3/F5/F8/F11/ESC/hotbar/... off physical keyboard
    # keys that DON'T EXIST on the console. Tag every hardcoded key-press site with a greppable
    # J2X-KEYBIND marker + KEYBIND_INDEX.md so each can be deliberately routed to a controller button or a
    # menu toggle -- instead of discovering them one at a time in-game (the Show-FPS/F3 case).
    if platform == 'xbox360':
        from . import keybind_mark as _kbm
        _kb = _kbm.run(out_dir)
        _prog('[keybind] tagged %d keyboard key-press site(s) -> KEYBIND_INDEX.md (route to controller/menu)'
              % len(_kb['sites']))

    # verify: run the crash-class rules over the emitted output (acceptance check)
    _prog('[rules] scanning crash-class rules on emitted output...')
    rule_findings = rules.scan_rules(ledger, out_dir, platform=platform)
    _prog('[rules] %d crash-class findings' % rule_findings)

    rep = None
    if write_report:
        _prog('[report] writing conversion report...')
        rep = report.write_reports(ledger, out_dir,
                                   meta={'output_dir': os.path.abspath(out_dir), 'profile': 'rewrite (j2ctool)'})
        _prog('[report] written to %s' % rep)

    # asset manifest: what textures/sounds/music/lang files must ship in the content folder
    from . import content
    cm = content.write_content_md(java_dir, out_dir)
    _prog('[content] asset manifest written to %s' % cm)

    # achievement conversion: map the program's achievements to the console achievement system
    # (title config + networked profile write) -- only if the program actually defines any.
    from . import achievements as _ach
    _found = _ach.scan_achievements(out_dir)
    if _found:
        _ach.run(out_dir, out_dir)
        _prog('[achievements] %d found -> ACHIEVEMENTS.md + achievements.xlast + Achievements.h/.cpp' % len(_found))

    # the single debug file: one master switch (DEBUG_ENABLED) gating every DBG_* macro, so all
    # debug instrumentation across the codebase is toggled from one place. Concise + rate-limited.
    # Opt-in: the UI/CLI decides whether the port gets debug support at all.
    if emit_debug:
        from . import debug
        debug.write_debug(out_dir)
        _prog('[debug] emitted Debug.h/.cpp + DEBUG.md (master toggle DEBUG_ENABLED; J2X-DEBUG markers)')
    else:
        _prog('[debug] debug support NOT emitted (opt-in disabled)')

    # platform-layer coverage: which video/input/audio/fileIO/network/memory symbols the game uses,
    # classified REUSABLE (Alpha reference) / STUB / MISSING, with an implementation-choice menu.
    from . import platform_coverage
    from ._paths import data_path as _data_path
    _ref_dir = _data_path('docs', 'platform', 'reference')
    pc_path, _pc = platform_coverage.write_report(
        out_dir, reference_dir=(_ref_dir if os.path.isdir(_ref_dir) else None),
        runtime_files=set(rt_files.keys()))
    _prog('[platform] coverage report written to %s' % pc_path)

    # API-mapping coverage: which external Java/library symbols the source references, resolved
    # against the API-mapping registry for the target language+dialect. APIMAP.md is the enumerable
    # gap list -- what the engine can already translate vs. what still needs a mapping row/shim.
    try:
        from . import apimap as _apimap
        _dialect = (std_version.label or '').lower() or None
        _syms = _apimap.extract_symbols_from_dir(java_dir)
        _md = _apimap.render_coverage_md(_syms, 'java', 'cpp', _dialect)
        _write(os.path.join(out_dir, 'APIMAP.md'), _md)
        _amcov = _apimap.load().coverage(_syms, 'java', 'cpp', _dialect)
        _prog('[apimap] %d symbols: %d mapped, %d unsupported, %d unknown -> APIMAP.md'
              % (len(_syms), len(_amcov['mapped']), len(_amcov['unsupported']), len(_amcov['unmapped'])))
    except Exception as _e:
        _prog('[apimap] skipped: %s' % _e)

    # Regenerate the bug-fix ledger from the live FIXED BUG / J2X-BUGFIX markers in the
    # converter's authored sources (source of truth is the CONVERTER repo, not the tree).
    # Keeps BUGFIX_LEDGER.md perfectly in sync so no game-breaking fix is ever forgotten.
    try:
        from . import bugfix_ledger
        _repo_root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
        _bf = bugfix_ledger.run(_repo_root)
        _prog('[bugfix-ledger] %d markers / %d distinct fixes -> BUGFIX_LEDGER.md'
              % (len(_bf), len(set(e['slug'] for e in _bf))))
    except Exception as _e:
        _prog('[bugfix-ledger] skipped: %s' % _e)

    # restore the base nullable-getter globals (derived set was scoped to this conversion only)
    typemap.KNOWN_NULLABLE_GETTERS.clear(); typemap.KNOWN_NULLABLE_GETTERS.update(_base_getters)
    typemap.NULLABLE_GETTER_RETURN_TYPE.clear(); typemap.NULLABLE_GETTER_RETURN_TYPE.update(_base_gtypes)
    _statements.refresh_nullable_getters()

    cats = ledger.totals_by_category()
    sevs = ledger.totals_by_severity()
    _prog('[done] %d events: %d PORTED, %d GUESSED, %d STUBBED, %d RISK, %d HIGH-sev'
          % (len(ledger), cats.get(Category.PORTED, 0), cats.get(Category.GUESSED, 0),
             cats.get(Category.STUBBED, 0), cats.get(Category.RISK, 0),
             sevs.get(Severity.HIGH, 0)))
    return {
        'parsed': len(classes), 'headers': headers, 'sources': sources,
        'rule_findings': rule_findings, 'events': len(ledger),
        'high': sevs.get(Severity.HIGH, 0),
        'report': rep,
        'ledger': ledger,
        'cpp_std': std_version.label,
        'platform': platform or 'none',
    }


# Dependency order for static initialization (the static-init-order crash class). Seeded from the
# old port's proven Main.cpp order: enums are header-only const-int (no runtime init, omitted);
# Material before Block (block ctors read Material statics); Item + CraftingManager after Block
# (items/recipes read block statics); then the class registries. Anything not listed runs after,
# in a stable sorted order. To fix a missed dependency found on hardware, edit THIS list.
from . import profiles as _profiles
_STATIC_INIT_ORDER = list(_profiles.active().static_init_order)


def compute_static_init_order(classes, callable_init):
    """Order classes' initStatics() so a class runs AFTER every class whose statics it reads.
    Two dependency sources, combined:
      * SEED (`_STATIC_INIT_ORDER`): proven, ctor-MEDIATED deps (Block ctor reads Material statics --
        invisible to a text scan because Material is referenced inside BlockStone's ctor, not Block's
        static init). Kept as a PRIORITY so the known order is preserved.
      * DERIVED: DIRECT reads -- a class whose static field initializers / static{} blocks reference
        another initable class's `Name.`/`Name::` statics. These become HARD ordering constraints and
        catch registries the seed doesn't list (new Beta classes: Achievement, StatList, typed enums
        reading Material, ...). Correct by construction rather than by hand-editing the seed.
    Stable + deterministic; cycles are broken by seed/alpha priority (logged by the caller)."""
    by_name = dict((c.name, c) for c in classes)

    def _ctor_and_inst_text(cls):
        """Text that runs when an object of `cls` is constructed: its ctor bodies + instance-field
        initializers + instance `{ }` blocks (all execute during construction)."""
        t = [ln for m in cls.constructors() for _ln, ln in m.body]
        t += [f.initializer for f in cls.fields if not f.is_static and f.initializer]
        for blk in getattr(cls, 'inst_inits', []):
            t += [ln for _ln, ln in blk]
        return t

    # J2X-BUGFIX 2026-08-13 static-init-order-ctor: a class whose initStatics() CONSTRUCTS objects
    # (`new X`) runs X's ctor chain -- and those ctors read other classes' statics. That dependency is
    # invisible if you only scan the class's own static-init text: BiomeGenBase::initStatics() does
    # `new BiomeGenRainforest()`, whose (inherited BiomeGenBase) ctor reads Block::s_grass -> BiomeGenBase
    # must init AFTER Block, but its static-init text names Block zero times. So FOLLOW `new X` transitively
    # into X's ctor + instance inits + ancestor ctors, collecting their static refs as real deps.
    def direct_refs(c):
        txt = [f.initializer for f in c.fields if f.is_static and f.initializer]
        for blk in getattr(c, 'static_inits', []):
            txt.extend(t for _ln, t in blk)
        for ec in getattr(c, 'enum_consts', []):        # typed-enum constant args build in initStatics()
            txt.extend(ec.args)                          # (e.g. EnumCreatureType reads Material.air)
        joined = ' '.join(t for t in txt if t)
        refs = set(re.findall(r'\b([A-Z]\w*)\b', joined))
        # transitively follow `new X` into constructed classes' ctor/instance/ancestor bodies
        todo = list(re.findall(r'\bnew\s+([A-Z]\w*)', joined))
        seen = set()
        while todo:
            x = todo.pop()
            if x in seen:
                continue
            seen.add(x)
            anc = by_name.get(x)
            while anc is not None and anc.name not in ('',):     # walk x + its superclass chain
                bj = ' '.join(t for t in _ctor_and_inst_text(anc) if t)
                refs |= set(re.findall(r'\b([A-Z]\w*)\b', bj))
                todo.extend(re.findall(r'\bnew\s+([A-Z]\w*)', bj))
                nxt = by_name.get(anc.parent)
                if nxt is None or nxt.name in seen:
                    break
                anc = nxt
        return (refs & callable_init) - {c.name}        # only OTHER initable classes are real deps

    deps = dict((n, direct_refs(by_name[n])) for n in callable_init if n in by_name)

    # J2X-BUGFIX 2026-08-13 static-init-order-seed: the SEED must be a HARD ordering chain, not just a
    # tiebreaker priority. CraftingManager::initStatics() is only `s_instance = new CraftingManager()`, so
    # direct_refs sees no deps (the real Item::/Block:: reads are buried in the ctor -> RecipesTools::
    # addRecipes) -> it floated to position 2, ran before Item/Block, and derefed NULL Item statics ->
    # crash in j2x_initAll (black screen). Chain consecutive seed members as real deps so those ctor-
    # mediated orderings hold: each seeded class present in the graph must run after the previous one.
    seed_present = [n for n in _STATIC_INIT_ORDER if n in deps]
    for _i in range(1, len(seed_present)):
        deps[seed_present[_i]] = deps[seed_present[_i]] | {seed_present[_i - 1]}

    prio = dict((name, i) for i, name in enumerate(_STATIC_INIT_ORDER))
    order_key = lambda n: (prio.get(n, len(_STATIC_INIT_ORDER)), n)

    ordered, placed, remaining = [], set(), set(deps)
    while remaining:
        ready = [n for n in remaining if deps[n] <= placed]
        if not ready:                                   # dependency cycle -> break by priority
            ready = list(remaining)
        pick = min(ready, key=order_key)
        ordered.append(pick)
        placed.add(pick)
        remaining.discard(pick)
    # any callable class not present in `classes` (shouldn't happen) -> append stably
    ordered += sorted(c for c in callable_init if c not in placed)
    return ordered


def _write_init_driver(out_dir, init_classes, classes):
    """Emit j2x_init.h / j2x_init.cpp -- the single ordered static-init entry point. j2x_initAll()
    calls each class's initStatics() in dependency order; Main calls it once at startup, BEFORE the
    game runs, replacing C++'s undefined cross-translation-unit static-init order."""
    callable_init = set(init_classes)
    ordered = compute_static_init_order(classes, callable_init)

    _write(os.path.join(out_dir, 'j2x_init.h'),
           '#ifndef J2X_INIT_H\n#define J2X_INIT_H\n'
           '// Runs every class\'s static initializers (Java static{} + static field inits) in\n'
           '// dependency order. CALL ONCE at startup, before any game code. Generated by j2ctool.\n'
           'void j2x_initAll();\n#endif // J2X_INIT_H\n')

    # J2X-BUGFIX 2026-08-13 instanceof-isa: C++ typeid is EXACT; Java instanceof is IS-A. is<T>() (JObject)
    # therefore missed every subclass (x instanceof Block was false for BlockOreStorage) -- silently
    # breaking recipes/entities/GUI. Emit a class-ancestry registry (each class -> its base + interfaces)
    # that is<T>() walks. Separate TU (all headers) so a bad include can't break the init driver.
    names = set(c.name for c in classes)
    edges = []
    for c in classes:
        if getattr(c, 'parent', None):
            b = c.parent.split('<')[0].strip()
            if b in names and b != c.name:
                edges.append((c.name, b))
        for iface in getattr(c, 'interfaces', []):
            b = iface.split('<')[0].strip()
            if b in names and b != c.name:
                edges.append((c.name, b))
    _write(os.path.join(out_dir, 'j2x_rtti.h'),
           '#ifndef J2X_RTTI_H\n#define J2X_RTTI_H\n'
           '// Registers each class\'s base/interface ancestry so JObject::is<T>() (instanceof) matches\n'
           '// subclasses, not just the exact dynamic type. Call ONCE, before any instanceof runs.\n'
           'void j2xRegisterRtti();\n'
           'void j2xPrepopulateClassReg();   // pre-fill _j2xClassReg at BOOT (thread-safe RTTI under MP net threads)\n'
           '#endif // J2X_RTTI_H\n')
    rtti = ['#include "stdafx.h"', '#include "JavaRT.h"', '#include "j2x_rtti.h"', '#include "Class.h"', '']
    rtti += ['#include "%s.h"' % n for n in sorted(names)]
    rtti += ['', 'void j2xRegisterRtti()', '{']
    rtti += ['\tj2xRegisterBase(typeid(%s).name(), typeid(%s).name());' % (d, b) for d, b in edges]
    rtti += ['}', '']
    # FIXED BUG classreg-multithread-race: pre-materialise EVERY class into _j2xClassReg AT BOOT (before the MP
    # network reader/writer threads start), so j2xClassOf never lazily push_backs the shared registry during play.
    # That keeps it READ-ONLY under concurrency -> no reader-thread/main-thread use-after-free (the non-deterministic
    # MP heap-corruption hang). Alpha's runtime had no such registry, which is why Alpha MP never corrupted.
    rtti += ['void j2xPrepopulateClassReg()', '{']
    rtti += ['\tj2xClassOf(typeid(%s));' % n for n in sorted(names)]
    rtti += ['}', '']
    _write(os.path.join(out_dir, 'j2x_rtti.cpp'), '\n'.join(rtti))

    lines = ['#include "stdafx.h"', '#include "JavaRT.h"', '#include "j2x_init.h"', '#include "j2x_rtti.h"', '']
    lines += ['#include "%s.h"' % c for c in ordered]
    # J2X-BUGFIX 2026-08-13 init-trace: emit a per-class boot trace so a crash INSIDE j2x_initAll (a class
    # whose initStatics/ctor derefs a not-yet-initialized static) is pinpointed -- the last [INIT] line
    # logged names the offender. One-shot at boot (sparse -> safe per the DbgPrintF rule); gate J2X_INIT_TRACE
    # to 0 to strip it from a release build. Uses raw DbgPrint (surfaces once main() is running).
    lines += ['',
              'extern "C" unsigned long __cdecl DbgPrint(const char* Format, ...);',
              '#ifndef J2X_INIT_TRACE',
              '#define J2X_INIT_TRACE 0   // default OFF (floods boot log). Set 1 to pinpoint a static-init crash.',
              '#endif',
              '', 'void j2x_initAll()', '{',
              '\tj2xRegisterRtti();   // instanceof ancestry -- MUST precede any is<T>() (recipe/entity init)',
              '\tj2xPrepopulateClassReg();   // pre-fill the class registry at BOOT so RTTI is read-only under MP net threads']
    for c in ordered:
        lines.append('#if J2X_INIT_TRACE')
        lines.append('\tDbgPrint("[INIT] %s\\n");' % c)
        lines.append('#endif')
        lines.append('\t%s::initStatics();' % c)
    lines += ['}', '']
    _write(os.path.join(out_dir, 'j2x_init.cpp'), '\n'.join(lines))


from .textutil import read_text as _read_text, write_text as _write_text


def _write(path, text):
    _write_text(path, text)


def _read(path):
    # strict decode + raise on missing (matches the original pipeline behavior)
    return _read_text(path, errors='strict', missing_ok=False)
