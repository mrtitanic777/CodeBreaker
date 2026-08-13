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
from .std_transforms import apply_std_transforms

def _banner(cls_name, cpp_std=None, platform_label=None):
    std_label = cpp_std.label if cpp_std else 'C++03'
    plat = platform_label or 'unknown'
    return '// Auto-generated from %s.java\n// Target: %s / %s' % (cls_name, plat, std_label)


def _std_header_for(cpp_base):
    # Object -> JObject (the universal boxed value type) is a name-changing mapping, so it lands here
    # rather than as a user X.h. JObject lives in the runtime prelude; a header that takes/returns it
    # BY VALUE (e.g. `equals(JObject)`) needs the full definition, so pull in JavaRT.h. Without this the
    # header has neither an include nor a forward-decl for JObject and fails to compile standalone (the
    # only reason it ever worked was the .cpp happening to #include JavaRT.h before the class header).
    if 'JObject' in cpp_base:
        return '"JavaRT.h"'
    if 'std::string' in cpp_base:
        return '<string>'
    if 'std::vector' in cpp_base:
        return '<vector>'
    if 'std::map' in cpp_base:
        return '<map>'
    if 'std::set' in cpp_base:
        return '<set>'
    return None


def _base_type_spelling(parent):
    """C++ base-class spelling for a Java class that EXTENDS a container (e.g. `extends ArrayList`).
    ArrayList/List -> JList<void* > (JList.h is always included); HashMap/Map -> std::map<JObject,
    JObject>; HashSet/Set/TreeSet -> std::set<void* >. Non-container parents are returned unchanged."""
    if parent not in typemap.CONTAINERS:
        return parent
    base = typemap.JAVA_TO_CPP.get(parent, parent)
    return base + typemap.RAW_CONTAINER_DEFAULT.get(base, '')


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
        if cls.parent in typemap.CONTAINERS:          # class extends ArrayList/HashMap/... -> STL base
            h = _std_header_for(_base_type_spelling(cls.parent))
            if h:
                sys_inc.add(h)
        else:
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
    for im, _base in getattr(cls, 'mi_forwards', []):   # synthesized forwarding overrides
        if im.return_type:
            note(im.return_type)
        for p in im.params:
            note(p.jtype)

    inc.discard(cls.name)
    fwd.discard(cls.name)
    fwd -= inc                                    # if we #include it, don't also forward-declare
    return sys_inc, inc, fwd


def _method_decl(cls, m, resolver):
    params = ownership.params_str(m, resolver)
    if m.is_ctor:
        return '%s(%s);' % (cls.name, params)
    ret = ownership.return_cpp_type(m, resolver)
    prefix = 'static ' if m.is_static else 'virtual '
    suffix = ' = 0' if m.is_abstract else ''
    name = resolver.renamed_method(cls.name, m.name)   # field/method name-collision -> name_
    return '%s%s %s(%s)%s;' % (prefix, ret, name, params, suffix)


def _field_type(cls, f, resolver):
    """C++ type for an instance field. Scalar Java `byte` -> `signed char` (Java-faithful; resolver
    handles it) and `byte[]` -> unsigned-char buffer -- see the signed-byte policy in typemap. The
    only override is the (empty by default) UNSIGNED_BYTE_SCALAR_FIELDS escape hatch."""
    if (f.jtype.name == 'byte' and not f.jtype.is_array
            and (cls.name, f.name) in typemap.UNSIGNED_BYTE_SCALAR_FIELDS):
        return 'unsigned char'
    return resolver.field_type(f.jtype)


def _render_typed_enum_header(cls, resolver, ledger, cpp_std, platform_label):
    """A TYPED enum (constants carry ctor args / the enum has instance fields or methods) becomes a
    real class: instance fields + ctor + methods are preserved, and each constant is a singleton
    `static EnumX* NAME` built in initStatics(). All statics use BARE names (no s_), because a typed
    enum stays in the holder set so `EnumX.NAME` resolves to `EnumX::NAME` (see types.py)."""
    guard = naming.guard_macro(cls.name)
    L = ['#ifndef %s' % guard, '#define %s' % guard, '',
         _banner(cls.name, cpp_std, platform_label), '',
         '#include "JavaString.h"', '#include "JArray.h"', '#include "JList.h"', '#include "JSet.h"']
    sys_inc, inc, fwd = _collect_dependencies(cls, resolver)
    for h in sorted(sys_inc):
        L.append('#include %s' % h)
    for h in sorted(inc):
        L.append('#include "%s.h"' % h)
    for name in sorted(fwd):
        L.append('class %s;' % name)
    L += ['', 'class %s {' % cls.name, 'public:']
    for m in cls.constructors():
        L.append('\t' + _method_decl(cls, m, resolver))
    L.append('\tvirtual ~%s();' % cls.name)
    L.append('')
    L.append('\tint __ordinal;   // Java enum ordinal (0-based, declaration order)')
    for f in cls.instance_fields():
        L.append('\t%s %s;' % (_field_type(cls, f, resolver), f.name))   # bare instance name
    L.append('')
    for ec in cls.enum_consts:                                # the singleton constants
        L.append('\tstatic %s* %s;' % (cls.name, ec.name))
    for f in cls.static_fields():                             # real enum statics -> BARE name (holder access)
        L.append('\tstatic %s %s;' % (resolver.field_type(f.jtype), f.name))
    L.append('')
    for m in cls.methods:
        if not m.is_ctor:
            L.append('\t' + _method_decl(cls, m, resolver))
    L.append('\tint ordinal();')
    L.append('\tstatic JArray<%s* >& values();' % cls.name)
    L.append('\tstatic void initStatics();')
    L += ['};', '', '#endif // %s' % guard]
    if ledger is not None:
        ledger.add(cls.name + '.h', Category.PORTED,
                   'typed enum %s -> class with %d singleton constants + preserved fields/methods'
                   % (cls.name, len(cls.enum_consts)), source='emit', rule='typed-enum')
    return apply_std_transforms('\n'.join(L), cpp_std)


def render_header(cls, resolver, ledger=None, cpp_std=None, platform_label=None):
    guard = naming.guard_macro(cls.name)

    if cls.is_enum and cls.name in resolver.typed_enums:
        return _render_typed_enum_header(cls, resolver, ledger, cpp_std, platform_label)

    # Constant-only enums: emit the constants as scoped static const ints so `EnumOS::linux` resolves;
    # the enum TYPE itself maps to int (see types.py), so variables/switches are plain ints.
    if cls.is_enum:
        L = ['#ifndef %s' % guard, '#define %s' % guard, '', _banner(cls.name, cpp_std, platform_label), '',
             'struct %s {' % cls.name]
        for i, ec in enumerate(cls.enum_consts):
            L.append('\tstatic const int %s = %d;' % (ec.name, i))
        L += ['};', '', '#endif // %s' % guard]
        if ledger is not None:
            ledger.add(cls.name + '.h', Category.PORTED,
                       'enum %s -> struct of static const int constants' % cls.name,
                       source='emit', rule='enum')
        return apply_std_transforms('\n'.join(L), cpp_std)

    L = ['#ifndef %s' % guard, '#define %s' % guard, '',
         _banner(cls.name, cpp_std, platform_label), '']

    # String -> JavaString and T[] -> JArray<T>; a header that mentions either in a field/
    # param/return needs the full definition. Cheap and idempotent to always include.
    L.append('#include "JavaString.h"')
    L.append('#include "JArray.h"')
    L.append('#include "JList.h"')
    L.append('#include "JSet.h"')

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
        bases.append('public %s' % _base_type_spelling(cls.parent))
    bases += ['public %s' % i for i in cls.interfaces]
    decl = 'class %s' % cls.name + (' : ' + ', '.join(bases) if bases else '')
    L.append(decl + ' {')
    L.append('public:')

    # constructors + destructor
    for m in cls.constructors():
        L.append('\t' + _method_decl(cls, m, resolver))
    # a value-type class held by value as a field must be default-constructible; if it only
    # has parameterized ctors, synthesize a (zero-initializing) default ctor
    if (cls.name in resolver.values and cls.constructors()
            and not any(c.arity == 0 for c in cls.constructors())):
        L.append('\t%s();' % cls.name)
    # A class with NO explicit ctor relies on Java's implicit default ctor, which zero-inits ALL fields
    # (and runs any field initializers / init-blocks). C++'s compiler-default ctor leaves primitives and
    # pointers as GARBAGE -- the uninitialized-field crash class (e.g. GuiScreen had no ctor, so
    # allowUserInput was garbage -> the menu deref'd a NULL player -> IP 0x0). So synthesize a zero-init
    # default ctor whenever such a class has ANY instance fields (or init-blocks). source.py emits the
    # matching definition via _init_list (primitives/pointers -> 0; objects default-construct).
    elif (not cls.constructors()
          and (cls.instance_fields() or getattr(cls, 'inst_inits', []))):
        L.append('\t%s();' % cls.name)
    if not cls.is_interface:
        L.append('\tvirtual ~%s();' % cls.name)

    # fields (instance, then static)
    inst = cls.instance_fields()
    stat = cls.static_fields()
    if inst or stat:
        L.append('')
    for f in inst:
        L.append('\t%s %s;' % (_field_type(cls, f, resolver), naming.field_name(f)))
    for f in stat:                                   # arrays are JArray<T> now -> normal member decl
        L.append('\tstatic %s %s;' % (resolver.field_type(f.jtype), naming.field_name(f)))

    # methods (skip ctors, already emitted)
    methods = [m for m in cls.methods if not m.is_ctor]
    if methods:
        L.append('')
    for m in methods:
        L.append('\t' + _method_decl(cls, m, resolver))

    # Ordered static init: deferred static field initializers + Java static{} blocks run here,
    # called in dependency order by the generated driver j2x_initAll() (see source.py / j2x_init.cpp).
    # Always declared (empty when nothing to defer) so the driver can call it on any concrete class.
    if not cls.is_interface:
        L.append('')
        L.append('\tstatic void initStatics();')

    # MI-diamond unification: forwarding overrides so an interface's pure-virtual that a base
    # class already implements concretely is satisfied in THIS class (see compute_mi_forwards).
    mi = getattr(cls, 'mi_forwards', [])
    if mi:
        L.append('')
        for im, _base in mi:                          # concrete override (NOT '= 0' like the interface)
            L.append('\tvirtual %s %s(%s);' % (ownership.return_cpp_type(im, resolver),
                                               im.name, ownership.params_str(im, resolver)))

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
    return apply_std_transforms('\n'.join(L), cpp_std)


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
