# -*- coding: utf-8 -*-
"""
header.py -- render a class's .h from IR + a TypeResolver.

Decides includes vs forward declarations correctly:
  * parent / interfaces  -> #include (need the full definition to inherit)
  * value-type members / by-value params/returns -> #include (need full def)
  * reference-type (pointer) members/params/returns -> forward `class X;`
  * std-mapped types -> the right <string>/<vector>/<map>/<set> system header

Every lossy mapping (Object -> void*, NIO buffers) is recorded on the ledger as
an ANNOTATED event so the report shows it.
"""

from .. import typemap
from ..ledger import Category, Severity
from . import naming, ownership

_BANNER = '// Auto-generated from %s.java\n// Target: Xbox 360 / C++03 / DirectX 9'


def _std_header_for(cpp_base):
    if 'std::string' in cpp_base:
        return '<string>'
    if 'std::vector' in cpp_base:
        return '<vector>'
    if 'std::map' in cpp_base:
        return '<map>'
    if 'std::set' in cpp_base:
        return '<set>'
    return None


def _collect_dependencies(cls, resolver):
    """Return (system_includes, quoted_includes, forward_decls)."""
    sys_inc, inc, fwd = set(), set(), set()

    def note(jtype):
        name = jtype.name
        if name in typemap.PRIMITIVES:
            return
        cpp_base = resolver.base_cpp(name)
        if cpp_base != name:                      # std-mapped (string/vector/...)
            h = _std_header_for(cpp_base)
            if h:
                sys_inc.add(h)
            for a in jtype.args:                  # generic element types still need decls
                note(a)
            return
        # user type with its own X.h
        if resolver.is_value(jtype) and not jtype.is_array:
            inc.add(name)                         # by value -> need full definition
        else:
            fwd.add(name)                         # pointer -> forward declare
        for a in jtype.args:
            note(a)

    if cls.parent:
        inc.add(cls.parent)
    for iface in cls.interfaces:
        inc.add(iface)
    for f in cls.fields:
        note(f.jtype)
    for m in cls.methods:
        if m.return_type:
            note(m.return_type)
        for p in m.params:
            note(p.jtype)

    inc.discard(cls.name)
    fwd.discard(cls.name)
    fwd -= inc                                    # if we #include it, don't also forward-declare
    return sys_inc, inc, fwd


def _method_decl(cls, m, resolver):
    params = ', '.join('%s %s' % (resolver.param_type(p.jtype), p.name) for p in m.params)
    if m.is_ctor:
        return '%s(%s);' % (cls.name, params)
    ret = ownership.return_cpp_type(m, resolver)
    prefix = 'static ' if m.is_static else 'virtual '
    suffix = ' = 0' if m.is_abstract else ''
    return '%s%s %s(%s)%s;' % (prefix, ret, m.name, params, suffix)


def render_header(cls, resolver, ledger=None):
    guard = naming.guard_macro(cls.name)
    L = ['#ifndef %s' % guard, '#define %s' % guard, '',
         _BANNER % cls.name, '']

    # String -> JavaString and T[] -> JArray<T>; a header that mentions either in a field/
    # param/return needs the full definition. Cheap and idempotent to always include.
    L.append('#include "JavaString.h"')
    L.append('#include "JArray.h"')

    sys_inc, inc, fwd = _collect_dependencies(cls, resolver)
    for h in sorted(sys_inc):
        L.append('#include %s' % h)
    for h in sorted(inc):
        L.append('#include "%s.h"' % h)
    if sys_inc or inc:
        L.append('')
    for name in sorted(fwd):
        L.append('class %s;' % name)
    if fwd:
        L.append('')

    # class declaration
    bases = []
    if cls.parent:
        bases.append('public %s' % cls.parent)
    bases += ['public %s' % i for i in cls.interfaces]
    decl = 'class %s' % cls.name + (' : ' + ', '.join(bases) if bases else '')
    L.append(decl + ' {')
    L.append('public:')

    # constructors + destructor
    for m in cls.constructors():
        L.append('\t' + _method_decl(cls, m, resolver))
    if not cls.is_interface:
        L.append('\tvirtual ~%s();' % cls.name)

    # fields (instance, then static)
    inst = cls.instance_fields()
    stat = cls.static_fields()
    if inst or stat:
        L.append('')
    for f in inst:
        L.append('\t%s %s;' % (resolver.field_type(f.jtype), naming.field_name(f)))
    for f in stat:                                   # arrays are JArray<T> now -> normal member decl
        L.append('\tstatic %s %s;' % (resolver.field_type(f.jtype), naming.field_name(f)))

    # methods (skip ctors, already emitted)
    methods = [m for m in cls.methods if not m.is_ctor]
    if methods:
        L.append('')
    for m in methods:
        L.append('\t' + _method_decl(cls, m, resolver))

    # Rule of Three: a class that owns heap (freed in its destructor) must forbid
    # copying, else a by-value copy double-frees the pointer -> heap corruption.
    if ownership.owns_heap(cls, resolver):
        L.append('')
        L.append('private:')
        L.append('\t// owns heap; copying disabled so a copy cannot double-free (Rule of Three)')
        L.append('\t%s(const %s&);' % (cls.name, cls.name))
        L.append('\t%s& operator=(const %s&);' % (cls.name, cls.name))
        if ledger is not None:
            ledger.add(cls.name + '.h', Category.RISK,
                       'Rule-of-Three: copy ctor + assignment disabled (owns heap) to prevent '
                       'double-free heap corruption',
                       severity=Severity.MED, source='emit', rule='rule-of-three-nocopy')

    L.append('};')
    L.append('')
    L.append('#endif // %s' % guard)

    if ledger is not None:
        _report(cls, resolver, ledger)
    return '\n'.join(L)


def _report(cls, resolver, ledger):
    fname = cls.name + '.h'
    ledger.add(fname, Category.PORTED, 'generated header for %s' % cls.name,
               source='emit', rule='header')
    seen = set()
    for f in cls.fields:
        note = resolver.port_note(f.jtype)
        if note and f.jtype.name not in seen:
            seen.add(f.jtype.name)
            ledger.add(fname, Category.ANNOTATED,
                       'field type %s -> %s (%s)' % (f.jtype.name, resolver.field_type(f.jtype), note),
                       line=f.line, severity=Severity.LOW, source='emit', rule='type-note')
