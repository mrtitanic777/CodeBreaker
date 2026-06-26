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


def build_resolver(classes, value_types=None):
    names = set(c.name for c in classes)
    enums = set(c.name for c in classes if c.is_enum)
    ifaces = set(c.name for c in classes if c.is_interface)
    vals = set(value_types if value_types is not None else typemap.DEFAULT_VALUE_TYPES)
    return types.TypeResolver(known_classes=names, value_types=vals,
                             interface_types=ifaces, enum_types=enums,
                             members=build_member_env(classes))


def convert(java_dir, out_dir, value_types=None, ledger=None, write_report=True):
    """Convert a Java source tree to C++03. Returns a summary dict."""
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
    resolver = build_resolver(classes, value_types)

    for fname, content in runtime.FILES.items():     # runtime-support headers (Random.h, ...)
        _write(os.path.join(out_dir, fname), content)

    headers = sources = 0
    for cls in classes:
        try:
            h = emit_header.render_header(cls, resolver, ledger)
            _write(os.path.join(out_dir, cls.name + '.h'), h)
            headers += 1
            cpp = emit_source.render_source(cls, resolver, ledger)
            if cpp is not None:
                _write(os.path.join(out_dir, cls.name + '.cpp'), cpp)
                sources += 1
        except Exception as e:                       # noqa: BLE001
            ledger.add(cls.name, Category.ERROR, 'emit failed: %s' % e,
                       severity=Severity.HIGH, source='pipeline', rule='emit-error')

    # verify: run the crash-class rules over the emitted output (acceptance check)
    rule_findings = rules.scan_rules(ledger, out_dir)

    rep = None
    if write_report:
        rep = report.write_reports(ledger, out_dir,
                                   meta={'output_dir': os.path.abspath(out_dir), 'profile': 'rewrite (j2x)'})
    return {
        'parsed': len(classes), 'headers': headers, 'sources': sources,
        'rule_findings': rule_findings, 'events': len(ledger),
        'high': ledger.totals_by_severity().get(Severity.HIGH, 0),
        'report': rep,
    }


def _write(path, text):
    with open(path, 'w', encoding='utf-8') as f:
        f.write(text)
