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
            cpp_std='C++03', platform=None, progress_cb=None):
    """Convert a Java source tree to C++. Returns a summary dict.

    cpp_std: target C++ standard ('C++98', 'C++03', 'C++11', 'C++14', 'C++17', 'C++20', 'C++23').
    platform: target platform id ('xbox360', 'pc_windows', 'ps3', 'wii', etc.) for stub annotations.
    progress_cb: optional callable(str) invoked with per-class progress lines."""
    ledger = ledger if ledger is not None else Ledger()
    std_version = _parse_std_version(cpp_std)
    _prog = progress_cb or (lambda _: None)
    os.makedirs(out_dir, exist_ok=True)

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

    _prog('[resolve] inheritance + MI forwards + type resolver...')
    resolve_inheritance(classes)
    compute_mi_forwards(classes)
    resolver = build_resolver(classes, value_types)
    _prog('[resolve] done (%d known classes, %d interfaces, %d enums)'
          % (len(resolver.known), len(resolver.interfaces), len(resolver.enums)))

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

    headers = sources = 0
    init_classes = []                                # concrete classes whose initStatics() does real work
    total = len(classes)
    for idx, cls in enumerate(classes):
        tag = '[%d/%d] %s' % (idx + 1, total, cls.name)
        try:
            ev_before = len(ledger)
            _write(os.path.join(out_dir, cls.name + '.h'),
                   emit_header.render_header(cls, resolver, ledger, cpp_std=std_version, platform_label=plat_label))
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
                _write(os.path.join(out_dir, cls.name + '.cpp'), cpp)
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
    _write_init_driver(out_dir, init_classes, set(c.name for c in classes))

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


def _write_init_driver(out_dir, init_classes, all_class_names):
    """Emit j2x_init.h / j2x_init.cpp -- the single ordered static-init entry point. j2x_initAll()
    calls each class's initStatics() in dependency order; Main calls it once at startup, BEFORE the
    game runs, replacing C++'s undefined cross-translation-unit static-init order."""
    callable_init = set(init_classes)
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
