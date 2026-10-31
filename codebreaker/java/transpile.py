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
import threading

from codebreaker.java import parse, types, typemap, report, rules, runtime
from codebreaker.java.emit import header as emit_header, source as emit_source
from codebreaker.java.ledger import Ledger, Category, Severity


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

# classes whose construction hands `this` to someone (a registry, a list, a listener) -- set per convert
_THIS_ESCAPES = set()
# the project of this conversion (codebreaker/java/projects), set per convert
from codebreaker.java import projects as _projects_mod  # noqa: E402
_PROJECT = _projects_mod.GENERIC


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
      - a discarded `new X(...);` -> `delete new X(...);`"""
    from codebreaker.java.emit.std_transforms import sanitize_reserved_members
    text = sanitize_reserved_members(text)
    if _GL_TO_D3D9:
        text = _PROJECT.finalize_text(text, 'xbox360')   # the project's last word (Minecraft: GL picking
        #                                                  -> a raycast), BEFORE the ns rename
        text = _convert_gl_to_d3d9(text)
    if _TRACE_THROWS:
        text = _trace_guest_throws(text)   # tag every throw with its site (throws are no-op landmines here)
    text = text.replace(chr(39) + chr(92) + 'u0000' + chr(39), chr(39) + chr(92) + '0' + chr(39))
    # `new Object()` (a bare java.lang.Object, used only as a monitor/lock) -> a JObject VALUE. Object
    # has no C++ class; the universal box default-constructs, and the sync object is never used on the port.
    text = re.sub(r'\bnew\s+Object\s*\(\s*\)', 'JObject()', text)
    # DISCARDED-NEW LEAK: a statement that is a bare `new X(...);` (allocation whose result is never stored)
    # is a no-op in Java (the temporary is GC'd immediately) but LEAKS in C++. Faithful even in the original
    # -- e.g. RubyDung.render() has Notch's stray `new Cube(0, 0);`. Rewrite to `delete new X(...);` so the
    # ctor's side effects (if any) still run and the object is freed. Skips args containing a nested `new`
    # (deleting the outer object could double-free the inner one) -- left as-is for manual review -- and
    # FIXED BUG discarded-new-escapes: a class whose constructor hands out `this` (Item registers itself
    # in Item.itemsList) is kept by someone else; deleting it left them a dangling pointer.
    def _fix_discarded_new(_m):
        indent, stmt = _m.group(1), _m.group(2)
        if re.search(r'\bnew\b', stmt[3:]):
            return _m.group(0)
        if re.match(r'new\s+([A-Za-z_]\w*)', stmt).group(1) in _THIS_ESCAPES:
            return _m.group(0)
        return '%sdelete %s  // J2X-BUGFIX discarded-new-leak' % (indent, stmt)
    return re.sub(r'(?m)^(\s*)(new\s+[A-Za-z_][\w:]*\s*\([^;]*\)\s*;)\s*$', _fix_discarded_new, text)


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
    from codebreaker.java.ir import JType as _JT
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
    from codebreaker.java import runtime
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
    from codebreaker.java.ir import JType as _JT
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
    # A STATIC method with the SAME NAME AND SIGNATURE as an inherited INSTANCE (virtual) method is C++
    # C4526 (the static "cannot override" the base virtual -> it's hidden). Java allows it (BlockFlowing's
    # private static flow(World,int,int,int) vs BlockFluid's instance flow(World,int,int,int)); rename the
    # static so it no longer collides. Guards, so we don't break legit overloads:
    #  * SIGNATURE must match -- a static helper with a DIFFERENT signature (NoiseGeneratorPerlin's static
    #    generateNoise(double) vs the base virtual generateNoise(double,double)) legally coexists in C++
    #    and does NOT trip C4526; renaming it would kill the class's real generateNoise(double,double) OVERRIDE.
    #  * the name must NOT also be an INSTANCE method of this class -- the rename is by name, so renaming
    #    would clobber that instance method (the actual override) too.
    def _sig(m):
        return (m.name, tuple(p.jtype.name for p in getattr(m, 'params', [])))
    for c in classes:
        instance_names = set(m.name for m in c.methods
                             if not m.is_ctor and not getattr(m, 'is_static', False))
        static_methods = [m for m in c.methods if not m.is_ctor and getattr(m, 'is_static', False)
                          and m.name not in instance_names]
        if not static_methods:
            continue
        inherited_sigs, cur, guard = set(), by_name.get(c.parent), set()
        while cur and cur.name not in guard:
            guard.add(cur.name)
            for m in cur.methods:
                if not m.is_ctor and not getattr(m, 'is_static', False):
                    inherited_sigs.add(_sig(m))
            cur = by_name.get(cur.parent)
        for m in static_methods:
            if _sig(m) in inherited_sigs:
                own[c.name].add(m.name)
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
    a spurious guard is at worst a benign extra null-check. Returns (getter_names, {getter: retType}).

    AMBIGUITY GUARD: getters + return types are keyed by method NAME (global), and the emit hoist types
    a `recv->NAME()` result from that map regardless of receiver. An obfuscated name (`a`, `b`) that
    returns DIFFERENT types in different classes would then mistype a call on one receiver as another's
    return -- EntityMap.a() returns List but Pathfinder.a() returns PathPoint, so `entityMap->a()` got
    typed `PathPoint*` (then `.size()` -> C2039). A name whose return type is NOT consistent across every
    class declaring it is EXCLUDED (a wrong-type hoist is a compile error, far worse than a missed guard)."""
    rt_by_name = {}
    for c in classes:
        for m in c.methods:
            if m.is_ctor or m.return_type is None:
                continue
            rt_by_name.setdefault(m.name, set()).add((m.return_type.name, m.return_type.is_array))
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
            if len(rt_by_name.get(m.name, ())) > 1:
                continue                                  # ambiguous name across classes -> don't guard globally
            body = '\n'.join(t for _ln, t in m.body)
            if _RETURN_NULL.search(body):
                getters.add(m.name)
                gtypes.setdefault(m.name, rt.name)
    return getters, gtypes


_THIS_AS_VALUE = re.compile(r'\bthis\b(?!\s*(?:\.|->|\())')


def compute_this_escapes(classes):
    """The classes whose constructor (or instance initialiser) uses `this` as a value -- stores it, passes
    it -- and every class below one (its constructor runs the parent's). An object of one of these is kept
    by someone else after `new`, so it must not be freed where the `new` is a bare statement."""
    by_name = dict((c.name, c) for c in classes)
    own = set()
    for c in classes:
        parts = [t for k in c.constructors() for _ln, t in (k.body or ())]
        parts += [f.initializer for f in c.instance_fields() if f.initializer]
        for block in getattr(c, 'inst_inits', ()) or ():
            parts += [t for _ln, t in block]
        if any(_THIS_AS_VALUE.search(t) for t in parts):
            own.add(c.name)
        # a Thread is kept by its running thread: `new ThreadSleepForever(..)` starts itself in its
        # constructor (Alpha's timer hack), so deleting it frees a live thread
        if c.parent in ('Thread', 'TimerTask') or 'Runnable' in (getattr(c, 'interfaces', None) or ()):
            own.add(c.name)
    out = set()
    for c in classes:
        k = c
        while k is not None:
            if k.name in own:
                out.add(c.name)
                break
            k = by_name.get(k.parent)
    return out


def _read_text(path):
    with open(path, encoding='utf-8', errors='replace') as f:
        return f.read()


def build_resolver(classes, value_types=None):
    names = set(c.name for c in classes)
    enums = set(c.name for c in classes if c.is_enum)
    typed_enums = set(c.name for c in classes if c.is_enum and c.is_typed_enum())
    ifaces = set(c.name for c in classes if c.is_interface)
    vals = set(value_types if value_types is not None else typemap.DEFAULT_VALUE_TYPES)
    resolver = types.TypeResolver(known_classes=names, value_types=vals,
                             interface_types=ifaces, enum_types=enums,
                             typed_enum_types=typed_enums,
                             members=build_member_env(classes),
                             method_renames=build_method_renames(classes),
                             thread_like=compute_thread_like(classes))
    # whole-program no-GC caller-frees catalog for the source emitter: owned containers (Level.getCubes ->
    # new AABBs) get an RAII element-deleter at their call sites; owned scalar returns used inline as
    # arguments (getCubes(bb.expand(...))) get hoisted + RAII-deleted, but only where the receiving method
    # provably just reads the arg (retained_params / corpus_methods gate).
    from codebreaker.java.emit import ownership as _ownership
    resolver.own_catalog = types.OwnershipCatalog(
        return_methods=_ownership.owned_return_container_methods(classes, resolver),
        scalar_methods=_ownership.owned_scalar_return_methods(classes, resolver),
        retained_params=_ownership.retained_param_positions(classes),
        corpus_methods=set(m.name for c in classes for m in c.methods if not m.is_ctor))
    return resolver



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


def _defined_symbols(text):
    """Top-level symbols a translation unit DEFINES: class/struct names, the owner class of every `X::`
    method definition, and free-function names (a `<ret> name(args) {` definition). Used by the prune to
    keep an implementation file whose symbols live code links against even when NO file #includes it
    directly (e.g. FileInputStream's ctor + the free function J2X_getResourceAsStream both live in
    FileIO.cpp; Level/ImageIO reference them by name, not by including FileIO.h). Over-extraction only
    causes over-KEEPING (safe); the goal is to never miss a real link dependency."""
    import re as _re
    syms = set()
    for m in _re.finditer(r'\b(?:class|struct)\s+([A-Za-z_]\w*)', text):
        syms.add(m.group(1))
    # method/ctor/operator DEFINITION owner class: `Class::member(args) {` (has a body). Only DEFINITIONS
    # match (trailing `{`), so `std::vector v;` / `d3d9::glClear(...);` USAGES are NOT captured -- otherwise
    # every file would "provide" std/d3d9 and nothing could ever be pruned.
    for m in _re.finditer(r'\b([A-Za-z_]\w*)::[~A-Za-z_]\w*\s*\([^;{}]*\)\s*(?:const\s*)?\{', text):
        syms.add(m.group(1))
    # free-function DEFINITION at column 0: `<ret> name(args) {`. No `:` allowed in the prefix, so a method
    # definition (which has `Class::`) is NOT re-captured here as its bare method name.
    for m in _re.finditer(r'(?m)^[A-Za-z_][\w<>,\*&\s]*?\b([A-Za-z_]\w*)\s*\([^;{}]*\)\s*(?:const\s*)?\{', text):
        syms.add(m.group(1))
    return syms


def _coalesce_forwarders(out_dir, prog):
    """Fold pure FORWARDER headers into their targets. A forwarder is a header whose only real content is a
    single `#include "target.h"` -- a redirect that exists only because the Java code imported a name whose
    C++ definition actually lives elsewhere (ByteBuffer.h -> NIOBuffers.h, Keyboard.h/Mouse.h -> Input.h,
    Iterator.h -> JIterator.h). Rewrite every `#include "forwarder.h"` across the tree to the resolved
    target and delete the forwarder. Safe and purely cosmetic: the TYPE stays defined in the target, so no
    code changes -- only fewer files. Auto-detected, no curated list. Returns the removed filenames."""
    import re as _re
    _strip = lambda s: _re.sub(r'//[^\n]*', '', _re.sub(r'/\*.*?\*/', '', s, flags=_re.S))
    inc_re = _re.compile(r'#\s*include\s+"([^"]+\.h)"')
    guard_re = _re.compile(r'^\s*#\s*(?:ifndef|define|endif|pragma\b).*$')
    onlyinc_re = _re.compile(r'^\s*#\s*include\s+"[^"]+\.h"\s*$')
    raw = {}
    for f in os.listdir(out_dir):
        if f.endswith('.h') or f.endswith('.cpp'):
            raw[f] = _read(os.path.join(out_dir, f))

    fwd = {}   # forwarder filename -> the single target header it #includes
    for f in raw:
        if not f.endswith('.h'):
            continue
        body = _strip(raw[f])
        incs = inc_re.findall(body)
        meaningful = [ln for ln in body.splitlines() if ln.strip() and not guard_re.match(ln)]
        if len(incs) == 1 and meaningful and all(onlyinc_re.match(ln) for ln in meaningful):
            fwd[f] = incs[0]
    if not fwd:
        return []

    def _resolve(h):                                  # follow forwarder chains to the real (non-fwd) target
        seen = set()
        while h in fwd and h not in seen:
            seen.add(h); h = fwd[h]
        return h
    alias = {f: _resolve(f) for f in fwd}

    for f in list(raw):
        if f in fwd:
            continue
        new = inc_re.sub(lambda m: '#include "%s"' % alias.get(m.group(1), m.group(1)), raw[f])
        if new != raw[f]:
            _write(os.path.join(out_dir, f), new)

    removed = []
    for f in sorted(fwd):
        try:
            os.remove(os.path.join(out_dir, f)); removed.append(f)
        except OSError:
            pass
    if removed:
        prog('[coalesce] folded %d forwarder header(s) into their targets: %s'
             % (len(removed), ', '.join(removed)))
    return removed


def _prune_unreferenced(out_dir, game_classes, prog, overlay_files=None):
    """Full-product prune -- automatic dead-subsystem cut, no curated library list. ROOTS are the converted
    GAME CORPUS + the entry/build spine + any file marked `J2X-ALWAYS-LINK`. A file is KEPT if either
    (1) it is #include-reachable from a root, OR (2) it DEFINES a symbol that live code references by name
    (the cross-named-implementation case: FileInputStream's ctor / J2X_getResourceAsStream are defined in
    FileIO.cpp but declared in kept headers -- no #include reaches FileIO.cpp, yet the linker needs it).
    Everything else -- whole platform bundles (audio, net) and unused java.* stubs the game never touches --
    is a dead island and is dropped.

    Soundness: rule (1) keeps a file's own #includes (closure -> nothing kept dangles an include); rule (2)
    keeps any translation unit that supplies a symbol live code links against (-> no unresolved externals).
    The residual is link-only side effects with no name reference at all (HeapTrack's operator new), handled
    by the J2X-ALWAYS-LINK marker. Returns the list of dropped filenames.

    `overlay_files` -- relpaths a project OVERLAY just wrote (see overlays.py). These are roots by
    DEFINITION: a human put the file there deliberately, which outranks include-reachability. Without this
    every overlay-ADDED file was silently deleted on every convert -- the 360 boot/config glue (Kernel.cpp,
    KernelConfig.h, ControllerConfig.h) and the whole UDP scaffold, which UDP_MIGRATION.md says nothing
    includes yet BY DESIGN -- defeating the "ADDED .cpp are compiled" guarantee overlays.py makes."""
    import re as _re
    files = [f for f in os.listdir(out_dir) if f.endswith('.h') or f.endswith('.cpp')]
    fileset = set(files)
    _base = lambda f: f[:-2] if f.endswith('.h') else f[:-4]
    inc_re = _re.compile(r'#\s*include\s+"([^"]+\.h)"')
    _strip = lambda s: _re.sub(r'//[^\n]*', '', _re.sub(r'/\*.*?\*/', '', s, flags=_re.S))
    incs, raw, code, provides = {}, {}, {}, {}
    for f in files:
        try:
            with open(os.path.join(out_dir, f), encoding='utf-8', errors='replace') as _fh:
                raw[f] = _fh.read()
        except Exception:
            raw[f] = ''
        incs[f] = set(inc_re.findall(raw[f]))
        code[f] = _strip(raw[f])
        provides[f] = _defined_symbols(code[f])

    # Build-critical spine + entry glue: always roots. These are reachable from the game anyway (it
    # #includes JavaRT etc.); listing them is insurance so a scan gap can never drop the foundation.
    FOUNDATION = ('stdafx', 'JavaRT', 'JavaString', 'JArray', 'JList', 'JMap', 'JSet', 'JObject', 'JCat',
                  'Common', 'j2x_init', 'j2x_rtti', 'Main')
    roots = set(game_classes) | set(FOUNDATION)
    for f in files:                                   # link-critical files self-declare (HeapTrack, ...)
        if 'J2X-ALWAYS-LINK' in raw[f]:
            roots.add(_base(f))
    # Overlay-written files are roots by definition -- a deliberate hand-placed file outranks the include
    # graph. (Only the basename matters: the prune works on the flat out_dir listing.)
    for rel in (overlay_files or ()):
        fn = os.path.basename(rel)
        if fn.endswith('.h') or fn.endswith('.cpp'):
            roots.add(_base(fn))

    reachable = set()

    def _walk(seed_files):
        stack = list(seed_files)
        while stack:
            f = stack.pop()
            if f in reachable or f not in fileset:
                continue
            reachable.add(f)
            if f.endswith('.h'):                       # a reached .h drags its .cpp partner (same TU)
                cpp = _base(f) + '.cpp'
                if cpp in fileset and cpp not in reachable:
                    stack.append(cpp)
            for inc in incs.get(f, ()):
                if inc in fileset and inc not in reachable:
                    stack.append(inc)

    _walk([f for f in files if _base(f) in roots])
    # Symbol-defense fixpoint: keep any file that DEFINES a name referenced by already-reachable CODE
    # (catches cross-named implementation files the #include graph misses), then re-walk their includes.
    changed = True
    while changed:
        changed = False
        ref_idents = set()
        for f in reachable:
            ref_idents.update(_re.findall(r'\b[A-Za-z_]\w*\b', code[f]))
        for f in files:
            if f in reachable:
                continue
            if provides[f] & ref_idents:
                _walk([f]); changed = True

    dropped = []
    for f in sorted(files):
        if f in reachable or _base(f) in roots:
            continue
        try:
            os.remove(os.path.join(out_dir, f)); dropped.append(f)
        except OSError:
            pass
    if dropped:
        prog('[prune] cut %d file(s) in dead subsystems no live #include reaches (%d subsystem(s)): %s'
             % (len(dropped), len(set(_base(f) for f in dropped)),
                ', '.join(sorted(set(_base(f) for f in dropped)))))
    return dropped


def convert(java_dir, out_dir, value_types=None, ledger=None, write_report=True,
            cpp_std='C++03', platform=None, progress_cb=None, platform_refs=False,
            sdk_override=None, emit_debug=True, overlay_dir=None,
            edition=None, mc_version=None):
    """Convert a Java source tree to C++ (see _convert). The per-conversion module state -- the active
    edition profile and the derived nullable-getter sets -- is snapshotted here and restored in a
    `finally`, so a conversion that RAISES midway can't leak its profile into the next in-process
    convert, which would make output depend on earlier runs.

    That state is module-level (the active profile reseeds typemap/rules/vcxproj/signatures; the
    nullable-getter and `->length` sets are per corpus), so conversions in one process run ONE AT A
    TIME: a second thread waits here. Conversions that should run side by side run in separate
    processes -- which is how CodeBreaker runs every job (jobs/pool, the console's subprocesses)."""
    with _ONE_AT_A_TIME:
        return _convert_restoring(java_dir, out_dir, value_types, ledger, write_report, cpp_std, platform,
                                  progress_cb, platform_refs, sdk_override, emit_debug, overlay_dir,
                                  edition, mc_version)


_ONE_AT_A_TIME = threading.Lock()


def _convert_restoring(java_dir, out_dir, value_types, ledger, write_report, cpp_std, platform, progress_cb,
                       platform_refs, sdk_override, emit_debug, overlay_dir, edition, mc_version):
    from codebreaker.java import profiles as _profiles
    _prev_profile = _profiles.active().name
    _base_getters = set(typemap.KNOWN_NULLABLE_GETTERS)
    _base_gtypes = dict(typemap.NULLABLE_GETTER_RETURN_TYPE)
    try:
        return _convert(java_dir, out_dir, value_types=value_types, ledger=ledger,
                        write_report=write_report, cpp_std=cpp_std, platform=platform,
                        progress_cb=progress_cb, platform_refs=platform_refs,
                        sdk_override=sdk_override, emit_debug=emit_debug, overlay_dir=overlay_dir,
                        edition=edition, mc_version=mc_version)
    finally:
        typemap.KNOWN_NULLABLE_GETTERS.clear(); typemap.KNOWN_NULLABLE_GETTERS.update(_base_getters)
        typemap.NULLABLE_GETTER_RETURN_TYPE.clear(); typemap.NULLABLE_GETTER_RETURN_TYPE.update(_base_gtypes)
        from codebreaker.java.emit import source as _emit_source
        _emit_source.TREE_BODIES = None                # phase 1's IR bodies never outlive their conversion
        global _PROJECT
        _PROJECT = _projects_mod.GENERIC              # nor does its project
        if _profiles.active().name != _prev_profile:
            _profiles.use(_prev_profile)


def _convert(java_dir, out_dir, value_types=None, ledger=None, write_report=True,
             cpp_std='C++03', platform=None, progress_cb=None, platform_refs=False,
             sdk_override=None, emit_debug=True, overlay_dir=None,
             edition=None, mc_version=None):
    """Convert a Java source tree to C++. Returns a summary dict.

    cpp_std: target C++ standard ('C++98', 'C++03', 'C++11', 'C++14', 'C++17', 'C++20', 'C++23').
    platform: target platform id ('xbox360', 'pc_windows', 'ps3', 'wii', etc.) for stub annotations.
    platform_refs: if True, emit the real hand-written platform layer from docs/platform/reference/
                   (GL->D3D9 emulator, audio, input, socket) instead of the no-op stubs.
    sdk_override: explicit SDK root; else auto-detected. A console platform without a located SDK is
                  REFUSED (the converter ships no SDK headers/signatures).
    edition: Minecraft era to target ('rubydung'/'indev'/'infdev'/'alpha'/'beta'/'release', or a
             'minecraft_*' profile name). None -> resolve from mc_version, else auto-detect from the
             source, else keep the default profile (a no-op that preserves today's exact behavior).
    mc_version: the decompiler version id (e.g. 'a1.2.6'), used to infer the edition when `edition`
                is not given.
    progress_cb: optional callable(str) invoked with per-class progress lines."""
    ledger = ledger if ledger is not None else Ledger()
    std_version = _parse_std_version(cpp_std)
    _prog = progress_cb or (lambda _: None)
    os.makedirs(out_dir, exist_ok=True)
    # The 360 target ships a COMPLETE, self-contained platform layer by default (entry point, GL->D3D9
    # emulator, shaders, textures, audio, input, file IO) -- the user never hand-writes/copies it. Opt
    # out is still possible by passing platform_refs=False explicitly for a non-xbox target.
    if platform in ('xbox360', 'pc_opengl'):
        platform_refs = True

    # On the 360 emulator a C++ `throw` is a no-op that doesn't unwind -> tag every emitted throw with
    # its site (JavaRT.h's J2X_GUEST_THROW) so a fired landmine names its file:line in the debug log.
    global _TRACE_THROWS, _GL_TO_D3D9
    _TRACE_THROWS = (platform == 'xbox360')
    # The 360 runs NO OpenGL: convert every GL call in the emitted game to the Direct3D 9 layer (d3d9).
    _GL_TO_D3D9 = (platform == 'xbox360')

    # SDK GATE: refuse platform-specific conversion for a console target with no SDK on the machine.
    from codebreaker.java.console import sdk as _sdk
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

    # Select the per-edition target profile for THIS conversion (explicit `edition` > `mc_version` id
    # > source auto-detection > keep default). profiles.use() reseeds the frozen profile globals in
    # typemap/rules/vcxproj/signatures/pipeline, so it MUST run before build_resolver (which reads
    # typemap.DEFAULT_VALUE_TYPES). convert() restores it in a `finally` (even on an exception), so
    # repeated in-process conversions can't drift.
    from codebreaker.java import profiles as _profiles, projects as _projects
    global _PROJECT
    _PROJECT = _projects.choose(classes, edition=edition, mc_version=mc_version)
    if _PROJECT is not _projects.GENERIC:
        _prog('[project] %s' % _PROJECT.name)
    _prev_profile = _profiles.active().name
    _ed_name = _PROJECT.profile_for(edition, mc_version=mc_version, classes=classes)
    if _ed_name and _ed_name != _profiles.active().name:
        _profiles.use(_ed_name)
        _prog('[edition] active target profile: %s' % _ed_name)
    elif edition or mc_version:
        _prog('[edition] target profile: %s' % _profiles.active().name)

    _prog('[resolve] inheritance + MI forwards + type resolver...')
    resolve_inheritance(classes)
    compute_mi_forwards(classes)
    resolver = build_resolver(classes, value_types)
    resolver.markers = markers                            # dropped EmptyN markers -> strip their call-site args
    # classes whose constructor makes a virtual call Java would dispatch to the override (Block's
    # isOpaqueCube, Entity's entityInit): built in two phases, `(new C())->j2xInit(args)` (see twophase.py)
    from codebreaker.java import twophase as _twophase
    resolver.two_phase = _twophase.compute(classes, resolver.values)
    if resolver.two_phase:
        _prog('[resolve] %d class(es) built in two phases (virtual calls in constructors)' % len(resolver.two_phase))
    _prog('[resolve] done (%d known classes, %d interfaces, %d enums)'
          % (len(resolver.known), len(resolver.interfaces), len(resolver.enums)))

    global _THIS_ESCAPES
    _THIS_ESCAPES = compute_this_escapes(classes)

    # Java collection out-params -> C++ references. A method that MUTATES a List/Map/Set param writes the
    # caller's collection; modelled as a value struct it would copy instead, silently dropping the writes
    # (the getCollidingBoundingBoxes collision-dead bug). Mark them so the emitter passes them by reference.
    _coll_ref = compute_collection_out_params(classes, resolver)
    _prog('[resolve] %d collection out-param(s) promoted to by-reference' % _coll_ref)

    # DERIVE nullable getters from the corpus and union into the guarded set for THIS conversion, so
    # emit-time null-guarding + the null-deref rule cover getters that can `return null` on any corpus
    # (not just the Alpha whitelist). convert()'s `finally` restores the module globals across calls.
    _base_getters = set(typemap.KNOWN_NULLABLE_GETTERS)
    _base_gtypes = dict(typemap.NULLABLE_GETTER_RETURN_TYPE)
    d_getters, d_gtypes = derive_nullability(classes, resolver)
    typemap.KNOWN_NULLABLE_GETTERS |= d_getters
    for _k, _v in d_gtypes.items():
        typemap.NULLABLE_GETTER_RETURN_TYPE.setdefault(_k, _v)
    _prog('[nullability] derived %d additional nullable getter(s) from the corpus'
          % len(d_getters - _base_getters))

    plat_label = None
    if platform:
        from codebreaker.java.platforms import get_platform
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
        from codebreaker.java.console import platform_refs as _pref
        from codebreaker.java._paths import data_path as _data_path
        # the platform's own layer: the 360's (Direct3D 9 / XInput / XACT) or the PC's (SDL2 / OpenGL)
        _ref_dir0 = (_data_path('platform', 'pc', 'reference') if platform == 'pc_opengl'
                     else _data_path('platform', 'reference'))
        # DETECTION-DRIVEN (no game hardcoding): tell the platform emitter which classes the game
        # defines itself + its main class, so it skips any platform file that collides with a
        # game-defined class and generates a generic entry point instead of the Beta-specific one.
        _game_classes = set(c.name for c in classes)
        _main_class = _pref.detect_main_class(classes)
        # No static main? A concrete top-level Runnable game shell (Indev's Minecraft) gets a
        # construct-and-run() entry; Alpha/Beta (abstract Minecraft + MinecraftImpl) return None here
        # and keep the Beta-specific Main.reference.cpp.
        _runnable_game = _pref.detect_runnable_game_class(classes) if not _main_class else None
        emitted = _pref.emit_references(out_dir, _ref_dir0,
                                        game_classes=_game_classes, main_class=_main_class,
                                        runnable_game=_runnable_game, platform=platform)
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
    else:
        # The runtime's Inflater/GZIPInputStream include "Inflate.h" -- portable C (puff.c, no platform
        # in it) that otherwise only arrives with the platform layer. Without a platform it comes alone.
        from codebreaker.java._paths import data_path as _data_path
        for _ext in ('.h', '.cpp'):
            _write(os.path.join(out_dir, 'Inflate' + _ext),
                   _read(_data_path('platform', 'reference', 'Inflate.reference' + _ext)))

    # THE TRANSLATOR (docs/TRANSLATOR_DESIGN.md): every method body, constructor and initialiser comes from
    # javac's typed tree through codebreaker/translate's IR and C++03 back end.
    from codebreaker.java import probe as _probe
    from codebreaker.translate.backends import cpp03 as _cpp03
    from codebreaker.verify.lanes import java as _vjava
    from codebreaker.base import settings as _settings
    _cp = list(_probe.libraries())
    _shim = _probe.shims(_settings.state('translate'), _vjava.jdk()[0]) if _cp else ''
    _ctx = _cpp03.Context(resolver, classes)
    _ctx.ledger = ledger
    _ctx.char_map = dict(_PROJECT.char_map)
    _units = _cpp03.java_units(java_dir, ([_shim] if _shim else []) + _cp)
    # the console's fixes to the game's own logic, made on the tree before it is written (by shape, so no
    # printing of the C++ hides a site): sound routed to XACT, the applet ping, the render budget and NaN
    _patched = _PROJECT.ir_patches(_units, platform)
    if std_version.year >= 2017:                     # the C++17 forms (backends/cpp17.Bodies: cpp03.Bodies17)
        from codebreaker.translate.backends import cpp17 as _cpp17
        tree = _cpp17.Bodies(_ctx, _units)
    else:                                            # modern Java lowered to what C++03 can say (desugar.py)
        from codebreaker.translate import desugar as _desugar
        _desugar.run(_units)
        tree = _cpp03.Bodies(_ctx, _units)
    emit_source.TREE_BODIES = tree
    _prog('[translate] the IR front end lowered %d classes' % len(tree.decls))

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
    from codebreaker.java import reflection_factory
    _rf = reflection_factory.run(out_dir, registrars=_PROJECT.registrars)
    if _rf['total_sites']:
        _prog('[reflect] %d registries -> factories (%d classes), %d sites rewritten (%d reused)'
              % (len(_rf['registries']), _rf['total_classes'], _rf['total_sites'], len(_rf['reused'])))

    # the project's fixes and checks on the written program (Minecraft: the console's sound, menu
    # navigation, applet ping and render fixes), before overlays so an overlay still has the last word
    _PROJECT.after_emit(out_dir, platform, _patched, _prog)

    # PROJECT OVERLAYS: re-apply game/version-specific hand-fixes to GENERATED files -- leak `delete`s in
    # specific methods, the scheduled-tick value-comparing std::set, UI tweaks (OSK/FPS/tooltip), input
    # remaps, and hand-written net files (Socket.cpp) -- that do NOT generalize into an emitter rule and
    # would otherwise be WIPED by a reconversion. Runs LAST over the generated tree (after emission +
    # references + reflection) so overlays are the final say; ADDED .cpp are compiled because vcxproj
    # enumerates out_dir afterward. See codebreaker/java/overlays.py. When an overlay turns out to be a general
    # pattern, migrate it into the emitter/runtime/reference and drop the overlay.
    from codebreaker.java import overlays as _overlays
    _ov_dir = _overlays.resolve_overlay_dir(out_dir, java_dir, explicit=overlay_dir)
    _applied = _overlays.apply_overlays(out_dir, _ov_dir, prog=_prog)
    if _applied:
        _prog('[overlay] %d project overlay file(s) re-applied from %s' % (len(_applied), _ov_dir))

    # TWO-PHASE CONSTRUCTION: every `new C(args)` of a two-phase class -- the emitted code's, the generated
    # factories', the reference layer's and the overlays' -- becomes `(new C())->j2xInit(args)`. Last of the
    # rewrites, so nothing after it makes a plain `new C(...)` again.
    _tp = _twophase.run(out_dir, resolver.two_phase)
    if _tp['sites']:
        _prog('[twophase] %d construction site(s) across %d file(s) -> (new C())->j2xInit(...)'
              % (_tp['sites'], len(_tp['files'])))
    _stale = [c for c in sorted(resolver.two_phase) if os.path.isfile(os.path.join(out_dir, c + '.cpp'))
              and ('%s::j2xInit(' % c) not in _read_text(os.path.join(out_dir, c + '.cpp'))]
    if _stale:
        _prog('[twophase] WARNING: %s has no j2xInit (an overlay replaced it?) -- it will not link' % ', '.join(_stale))

    # the project's markers over the final code (Minecraft: keyboard keys the console does not have)
    _PROJECT.after_construction(out_dir, platform, _prog)

    # FORWARDER COALESCE: fold pure redirect headers (ByteBuffer.h -> NIOBuffers.h, Keyboard.h -> Input.h)
    # into their targets and delete them -- fewer files, no shims. Runs BEFORE the prune so the prune sees
    # the rewritten (direct) include graph.
    if platform:
        _coalesce_forwarders(out_dir, _prog)

    # DEAD-SUBSYSTEM PRUNE: full-product #include-reachability from the game corpus + entry. Any platform
    # bundle or stub the game never reaches (audio, networking, unused java.* libs, dead desktop types) is
    # cut whole -- no curated list. Runs LAST, after every file exists (emission + references + overlays),
    # so it sees the final include graph. See _prune_unreferenced.
    if platform:
        _prune_unreferenced(out_dir, set(c.name for c in classes), _prog, overlay_files=_applied)

    # the PC build: one CMake executable named after the program's main class
    if platform == 'pc_opengl':
        from codebreaker.java import cmake as _cmake
        from codebreaker.java.console import platform_refs as _pref2
        _exe = _pref2.detect_main_class(classes) or 'game'
        _prog('[cmake] %s (executable %s)' % (_cmake.write_cmakelists(out_dir, _exe), _exe))

    # verify: run the crash-class rules over the emitted output (acceptance check)
    _prog('[rules] scanning crash-class rules on emitted output...')
    rule_findings = rules.scan_rules(ledger, out_dir, platform=platform)
    _prog('[rules] %d crash-class findings' % rule_findings)

    rep = None
    if write_report:
        _prog('[report] writing conversion report...')
        rep = report.write_reports(ledger, out_dir,
                                   meta={'output_dir': os.path.abspath(out_dir), 'profile': 'rewrite (CodeBreaker)'})
        _prog('[report] written to %s' % rep)

    # the single debug file: one master switch (DEBUG_ENABLED) gating every DBG_* macro, so all
    # debug instrumentation across the codebase is toggled from one place. Concise + rate-limited.
    # Opt-in: the UI/CLI decides whether the port gets debug support at all.
    if emit_debug:
        from codebreaker.java import debug
        debug.write_debug(out_dir)
        _prog('[debug] emitted Debug.h/.cpp + DEBUG.md (master toggle DEBUG_ENABLED; J2X-DEBUG markers)')
    else:
        _prog('[debug] debug support NOT emitted (opt-in disabled)')

    # the project's own reports (Minecraft: asset manifest, achievements, platform-layer coverage)
    _PROJECT.reports(java_dir, out_dir, rt_files.keys(), _prog)

    # API-mapping coverage: which external Java/library symbols the source references, resolved
    # against the API-mapping registry for the target language+dialect. APIMAP.md is the enumerable
    # gap list -- what the engine can already translate vs. what still needs a mapping row/shim.
    try:
        from codebreaker.java import apimap as _apimap
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
        from codebreaker.java import bugfix_ledger
        _bf = bugfix_ledger.run()
        _prog('[bugfix-ledger] %d markers / %d distinct fixes -> BUGFIX_LEDGER.md'
              % (len(_bf), len(set(e['slug'] for e in _bf))))
    except Exception as _e:
        _prog('[bugfix-ledger] skipped: %s' % _e)

    # (the nullable-getter globals and the target profile are restored by convert()'s `finally`)

    emit_source.TREE_BODIES = None
    _prog('[translate] %d method bodies from the IR; %d untranslated (J2X-GAP%s)'
          % (tree.written, sum(tree.declined.values()),
             ''.join(', %s %d' % kv for kv in tree.declined.most_common(8))))

    cats = ledger.totals_by_category()
    sevs = ledger.totals_by_severity()
    _prog('[done] %d events: %d PORTED, %d GUESSED, %d STUBBED, %d RISK, %d HIGH-sev'
          % (len(ledger), cats.get(Category.PORTED, 0), cats.get(Category.GUESSED, 0),
             cats.get(Category.STUBBED, 0), cats.get(Category.RISK, 0),
             sevs.get(Severity.HIGH, 0)))
    return {
        'parsed': len(classes), 'headers': headers, 'sources': sources,
        'markers': sorted(markers),             # empty decompiler marker classes, dropped on purpose
        'tree_bodies': (tree.written, dict(tree.declined)),
        'rule_findings': rule_findings, 'events': len(ledger),
        'high': sevs.get(Severity.HIGH, 0),
        'report': rep,
        'ledger': ledger,
        'cpp_std': std_version.label,
        'platform': platform or 'none',
        'profile': _ed_name or _prev_profile,   # the target profile active DURING this conversion
    }


# Dependency order for static initialization (the static-init-order crash class). Seeded from the
# old port's proven Main.cpp order: enums are header-only const-int (no runtime init, omitted);
# Material before Block (block ctors read Material statics); Item + CraftingManager after Block
# (items/recipes read block statics); then the class registries. Anything not listed runs after,
# in a stable sorted order. To fix a missed dependency found on hardware, edit THIS list.
from codebreaker.java import profiles as _profiles
_STATIC_INIT_ORDER = list(_profiles.active().static_init_order)


def _reseed_static_init_order(p):
    """Re-derive the static-init seed when the active target profile changes (profiles.on_change).
    compute_static_init_order() reads this module global, so an edition switch must retarget it."""
    global _STATIC_INIT_ORDER
    _STATIC_INIT_ORDER = list(p.static_init_order)


_profiles.on_change(_reseed_static_init_order)


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
           '// dependency order. CALL ONCE at startup, before any game code. Generated by CodeBreaker.\n'
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
    # Java value equality: each concrete class that defines equals(Object) or hashCode() (a HashMap key such as
    # ChunkPosition) -- JObject compares and orders by them (JavaRT.h j2xRegisterValue)
    valued = []
    for c in classes:
        if c.name not in names or getattr(c, 'is_interface', False) or getattr(c, 'is_abstract', False):
            continue
        ms = [m for m in c.methods if not m.is_ctor and not getattr(m, 'is_static', False)
              and not getattr(m, 'is_abstract', False)]
        eq = any(m.name == 'equals' and len(m.params) == 1 and m.params[0].jtype.name == 'Object' for m in ms)
        hc = any(m.name == 'hashCode' and not m.params for m in ms)
        if eq or hc:
            valued.append(c.name)
            rtti.append('static bool j2xEq_%s(void* s, const JObject& o) { return %s; }'
                        % (c.name, '((%s*)s)->equals(o)' % c.name if eq else 's == o.p'))
            rtti.append('static int j2xHash_%s(void* s) { return %s; }'
                        % (c.name, '((%s*)s)->hashCode()' % c.name if hc else '(int)(std::size_t)s'))
    rtti += ['', 'void j2xRegisterRtti()', '{']
    rtti += ['\tj2xRegisterBase(typeid(%s).name(), typeid(%s).name());' % (d, b) for d, b in edges]
    rtti += ['\tj2xRegisterValue(typeid(%s).name(), j2xEq_%s, j2xHash_%s);' % (n, n, n) for n in valued]
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


from codebreaker.java.textutil import read_text as _read_text, write_text as _write_text


def _write(path, text):
    _write_text(path, text)


def _read(path):
    # strict decode + raise on missing (matches the original pipeline behavior)
    return _read_text(path, errors='strict', missing_ok=False)
