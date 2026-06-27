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

from . import parse, types, typemap, report, rules, runtime
from .emit import header as emit_header, source as emit_source
from .ledger import Ledger, Category, Severity


def discover(java_dir):
    """All .java under java_dir, de-duplicated by basename (output is keyed by class name)."""
    paths = {}
    for dp, _dirs, fs in os.walk(java_dir):
        for f in fs:
            if f.endswith('.java') and f not in paths:
                paths[f] = os.path.join(dp, f)
    return [paths[k] for k in sorted(paths)]


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


def build_member_env(classes):
    """Flatten each class's fields (static + instance) and method return types over its
    parent chain into {class: {'fields': {name: JType}, 'methods': {name: JType}}}.
    Closer (more-derived) definitions win. Feeds expression-type inference so chained
    `recv.field.member` / `Class::s_field.member` get '.' vs '->' right by construction."""
    by_name = dict((c.name, c) for c in classes)
    env = {}
    for cls in classes:
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
        env[cls.name] = {'fields': fields, 'methods': methods, 'statics': statics}
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


def build_resolver(classes, value_types=None):
    names = set(c.name for c in classes)
    enums = set(c.name for c in classes if c.is_enum)
    ifaces = set(c.name for c in classes if c.is_interface)
    vals = set(value_types if value_types is not None else typemap.DEFAULT_VALUE_TYPES)
    return types.TypeResolver(known_classes=names, value_types=vals,
                             interface_types=ifaces, enum_types=enums,
                             members=build_member_env(classes),
                             method_renames=build_method_renames(classes))


def _override_path(overrides_dir, name):
    p = os.path.join(overrides_dir, name) if overrides_dir else None
    return p if (p and os.path.isfile(p)) else None


def convert(java_dir, out_dir, value_types=None, ledger=None, write_report=True, overrides_dir=None):
    """Convert a Java source tree to C++03. Returns a summary dict.

    overrides_dir: optional directory of handwritten <Class>.h / <Class>.cpp files. When a
    file there matches a class, it is used verbatim INSTEAD of generated output (and logged
    as OVERRIDDEN) -- the escape hatch for the genuinely-hard cases (Object dispatch, recipe
    autoboxing, raytrace dual-use) that are correct-by-hand rather than by construction."""
    ledger = ledger or Ledger()
    os.makedirs(out_dir, exist_ok=True)

    classes = []
    for path in discover(java_dir):
        try:
            classes.append(parse.parse_file(path))
        except Exception as e:                       # noqa: BLE001 - one bad file shouldn't abort
            ledger.add(os.path.basename(path), Category.ERROR, 'parse failed: %s' % e,
                       severity=Severity.HIGH, source='pipeline', rule='parse-error')

    resolve_inheritance(classes)
    compute_mi_forwards(classes)
    resolver = build_resolver(classes, value_types)

    for fname, content in runtime.FILES.items():     # runtime-support headers (Random.h, ...)
        _write(os.path.join(out_dir, fname), content)

    headers = sources = overrides = 0
    init_classes = []                                # concrete classes whose initStatics() does real work
    for cls in classes:
        try:
            ov_h = _override_path(overrides_dir, cls.name + '.h')
            if ov_h:
                _write(os.path.join(out_dir, cls.name + '.h'), _read(ov_h))
                overrides += 1
                ledger.add(cls.name + '.h', Category.OVERRIDDEN, 'handwritten header override used',
                           source='pipeline', rule='override')
            else:
                _write(os.path.join(out_dir, cls.name + '.h'),
                       emit_header.render_header(cls, resolver, ledger))
            headers += 1

            ov_c = _override_path(overrides_dir, cls.name + '.cpp')
            if ov_c:
                _write(os.path.join(out_dir, cls.name + '.cpp'), _read(ov_c))
                sources += 1
                overrides += 1
                ledger.add(cls.name + '.cpp', Category.OVERRIDDEN, 'handwritten source override used',
                           source='pipeline', rule='override')
            else:
                cpp = emit_source.render_source(cls, resolver, ledger)
                if cpp is not None:
                    _write(os.path.join(out_dir, cls.name + '.cpp'), cpp)
                    sources += 1
                    if getattr(cls, 'has_init_statics', False):
                        init_classes.append(cls.name)    # generated class with deferred static init
        except Exception as e:                       # noqa: BLE001
            ledger.add(cls.name, Category.ERROR, 'emit failed: %s' % e,
                       severity=Severity.HIGH, source='pipeline', rule='emit-error')

    # ORDERED STATIC-INIT DRIVER: a single j2x_initAll() that calls each class's initStatics() in
    # DEPENDENCY order (C++ gives no cross-TU static-init order -> the static-init-order crash class).
    # Seeded with the alpha dependency order; refactored OVERRIDE registry classes expose initStatics()
    # too (declared in their overrides/*.h). Everything else runs after, in a stable (sorted) order.
    _write_init_driver(out_dir, init_classes, set(c.name for c in classes))

    # verify: run the crash-class rules over the emitted output (acceptance check)
    rule_findings = rules.scan_rules(ledger, out_dir)

    rep = None
    if write_report:
        rep = report.write_reports(ledger, out_dir,
                                   meta={'output_dir': os.path.abspath(out_dir), 'profile': 'rewrite (j2x)'})
    return {
        'parsed': len(classes), 'headers': headers, 'sources': sources,
        'overrides': overrides,
        'rule_findings': rule_findings, 'events': len(ledger),
        'high': ledger.totals_by_severity().get(Severity.HIGH, 0),
        'report': rep,
    }


# Dependency order for static initialization (the static-init-order crash class). Seeded from the
# old port's proven Main.cpp order: enums are header-only const-int (no runtime init, omitted);
# Material before Block (block ctors read Material statics); Item + CraftingManager after Block
# (items/recipes read block statics); then the class registries. Anything not listed runs after,
# in a stable sorted order. To fix a missed dependency found on hardware, edit THIS list.
_STATIC_INIT_ORDER = ['MathHelper', 'Material', 'Block', 'Item', 'CraftingManager',
                      'EntityList', 'TileEntity', 'MobSpawnerBase', 'OsMap', 'Packet']
# OVERRIDE classes refactored to expose initStatics() (they're verbatim, so not in init_classes,
# but their overrides/*.h declares initStatics() and overrides/*.cpp defines it). The driver only
# calls names listed here; add each override as it's refactored to the initStatics() pattern.
# PENDING: Block/EntityList/TileEntity/MobSpawnerBase/OsMap/Packet still use eager/file-local static
# init -- until they're refactored they keep self-initializing (and are NOT driven in order yet).
_INIT_OVERRIDES = set()   # TODO(step1): refactor the 6 registry overrides, then list them here


def _write_init_driver(out_dir, init_classes, all_class_names):
    """Emit j2x_init.h / j2x_init.cpp -- the single ordered static-init entry point. j2x_initAll()
    calls each class's initStatics() in dependency order; Main calls it once at startup, BEFORE the
    game runs, replacing C++'s undefined cross-translation-unit static-init order."""
    callable_init = set(init_classes) | (_INIT_OVERRIDES & all_class_names)
    ordered = [c for c in _STATIC_INIT_ORDER if c in callable_init]
    ordered += sorted(c for c in callable_init if c not in _STATIC_INIT_ORDER)

    _write(os.path.join(out_dir, 'j2x_init.h'),
           '#ifndef J2X_INIT_H\n#define J2X_INIT_H\n'
           '// Runs every class\'s static initializers (Java static{} + static field inits) in\n'
           '// dependency order. CALL ONCE at startup, before any game code. Generated by j2x.\n'
           'void j2x_initAll();\n#endif // J2X_INIT_H\n')

    lines = ['#include "stdafx.h"', '#include "JavaRT.h"', '#include "j2x_init.h"', '']
    lines += ['#include "%s.h"' % c for c in ordered]
    lines += ['', 'void j2x_initAll()', '{']
    lines += ['\t%s::initStatics();' % c for c in ordered]
    lines += ['}', '']
    _write(os.path.join(out_dir, 'j2x_init.cpp'), '\n'.join(lines))


def _write(path, text):
    with open(path, 'w', encoding='utf-8') as f:
        f.write(text)


def _read(path):
    with open(path, 'r', encoding='utf-8') as f:
        return f.read()
