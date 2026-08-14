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
            sdk_override=None, emit_debug=True):
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
        _ref_dir0 = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))),
                                 'docs', 'platform', 'reference')
        emitted = _pref.emit_references(out_dir, _ref_dir0)
        _prog('[platform] emitted %d hand-written platform references (replacing stubs): %s'
              % (len(emitted), ', '.join(sorted(emitted))))

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

    # verify: run the crash-class rules over the emitted output (acceptance check)
    _prog('[rules] scanning crash-class rules on emitted output...')
    rule_findings = rules.scan_rules(ledger, out_dir, platform=platform)
    _prog('[rules] %d crash-class findings' % rule_findings)

    rep = None
    if write_report:
        _prog('[report] writing conversion report...')
        rep = report.write_reports(ledger, out_dir,
                                   meta={'output_dir': os.path.abspath(out_dir), 'profile': 'rewrite (j2x)'})
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
    _ref_dir = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))),
                            'docs', 'platform', 'reference')
    pc_path, _pc = platform_coverage.write_report(
        out_dir, reference_dir=(_ref_dir if os.path.isdir(_ref_dir) else None),
        runtime_files=set(rt_files.keys()))
    _prog('[platform] coverage report written to %s' % pc_path)

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
_STATIC_INIT_ORDER = ['MathHelper', 'Material', 'Block', 'Item', 'CraftingManager',
                      'EntityList', 'TileEntity', 'MobSpawnerBase', 'OsMap', 'Packet']


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
           '// dependency order. CALL ONCE at startup, before any game code. Generated by j2x.\n'
           'void j2x_initAll();\n#endif // J2X_INIT_H\n')

    lines = ['#include "stdafx.h"', '#include "JavaRT.h"', '#include "j2x_init.h"', '']
    lines += ['#include "%s.h"' % c for c in ordered]
    # J2X-BUGFIX 2026-08-13 init-trace: emit a per-class boot trace so a crash INSIDE j2x_initAll (a class
    # whose initStatics/ctor derefs a not-yet-initialized static) is pinpointed -- the last [INIT] line
    # logged names the offender. One-shot at boot (sparse -> safe per the DbgPrintF rule); gate J2X_INIT_TRACE
    # to 0 to strip it from a release build. Uses raw DbgPrint (surfaces once main() is running).
    lines += ['',
              'extern "C" unsigned long __cdecl DbgPrint(const char* Format, ...);',
              '#ifndef J2X_INIT_TRACE',
              '#define J2X_INIT_TRACE 1',
              '#endif',
              '', 'void j2x_initAll()', '{']
    for c in ordered:
        lines.append('#if J2X_INIT_TRACE')
        lines.append('\tDbgPrint("[INIT] %s\\n");' % c)
        lines.append('#endif')
        lines.append('\t%s::initStatics();' % c)
    lines += ['}', '']
    _write(os.path.join(out_dir, 'j2x_init.cpp'), '\n'.join(lines))


def _write(path, text):
    with open(path, 'w', encoding='utf-8') as f:
        f.write(text)


def _read(path):
    with open(path, 'r', encoding='utf-8') as f:
        return f.read()
