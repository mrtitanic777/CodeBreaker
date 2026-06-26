# -*- coding: utf-8 -*-
"""
ownership.py -- which instance fields a class owns on the heap.

Shared by header.py (to declare the Rule-of-Three destructor + disabled copy)
and source.py (to define the destructor). A field is "owned heap" iff it is a
POINTER field (per the resolver) that a constructor assigns via `new`. Value-type
and static fields are never owned here -- deleting those would be wrong.

This is the heap-corruption defense: an owning class with a destructor but a
default copy constructor double-frees when copied (the classic 0xC0000018). We
detect owners here so emit can free them once AND forbid copying.
"""

import re

from .. import typemap

_CTOR_NEW = re.compile(r'(?:this\.)?(\w+)\s*=\s*new\s+\w[\w.]*\s*(\[)?')
# `new T[]{a, b, c}` -- a static array with an aggregate (brace) initializer
_AGG_ARRAY = re.compile(r'^new\s+[\w.]+\s*\[\s*\]\s*\{(.*)\}\s*$', re.S)


def aggregate_array_body(field):
    """For a static field initialized as `new T[]{...}`, the raw brace contents; else None.
    C++03 cannot `new T[]{...}`, so emit (header + source) declare a fixed C array instead."""
    init = (field.initializer or '').strip()
    m = _AGG_ARRAY.match(init)
    return m.group(1) if m else None
# broad: `field = new` OR `field[i] = new` (array-element ownership)
_CTOR_NEW_BROAD = re.compile(r'(?:this(?:->|\.))?(\w+)\s*(?:\[[^\]]*\])?\s*=\s*new\b')


def owned_pointers(cls, resolver):
    """{field_name: is_array} for pointer instance fields new'd DIRECTLY (`field = new`).
    This is what the destructor can safely `delete` -- element ownership (`field[i] = new`)
    is intentionally excluded (freeing it needs a sized loop)."""
    owned = {}
    for m in cls.constructors():
        text = '\n'.join(t for _ln, t in m.body)
        for fm in _CTOR_NEW.finditer(text):
            name, is_arr = fm.group(1), bool(fm.group(2))
            fld = cls.field(name)
            if fld and not fld.is_static and resolver.is_pointer(fld.jtype):
                owned[name] = is_arr or owned.get(name, False)
    return owned


def owns_heap(cls, resolver):
    """True if a constructor allocates into any instance field directly (`field = new`)
    or into an element of one (`field[i] = new`). Either way a by-value copy would
    double-free, so copying must be forbidden -- broader than owned_pointers()."""
    for m in cls.constructors():
        text = '\n'.join(t for _ln, t in m.body)
        for fm in _CTOR_NEW_BROAD.finditer(text):
            fld = cls.field(fm.group(1))
            # only real POINTER fields own heap; arrays are non-owning JArray<T> values now
            if fld and not fld.is_static and resolver.is_pointer(fld.jtype):
                return True
    return False


def returns_pointer(method, resolver):
    """Whether the method's return should be a C++ pointer.

    Reference types are always pointers. A VALUE type (e.g. Vec3D) is promoted to
    a pointer when the body `return new <ReturnType>(...)` -- the dual-use case
    where a struct is stored by value as a field but heap-allocated on return.
    """
    rt = method.return_type
    if rt is None or rt.name == 'void':
        return False
    if resolver.is_pointer(rt):
        return True
    # std::string (and other value-stdlib types) is NEVER a pointer: it has dedicated
    # by-value handling (return-by-value, `return null` -> empty string). Promoting it
    # both reintroduces the dangling-pointer risk and breaks virtual-override return
    # unification (base `return null` -> string*, derived `return "x"` -> string).
    if rt.name in typemap.VALUE_STDLIB:
        return False
    text = '\n'.join(t for _ln, t in method.body)
    # value-type return promoted to pointer if the body new's it in a return (incl. a
    # ternary) OR returns null -- both impossible for a by-value return
    if re.search(r'\breturn\b[^;]*\bnew\s+%s\b' % re.escape(rt.name), text):
        return True
    if re.search(r'\breturn\s+(?:null|NULL)\b', text):
        return True
    return False


def return_cpp_type(method, resolver):
    """The C++ return type for a method, applying dual-use promotion. Shared by
    header.py and source.py so the declaration and definition always agree."""
    if method.return_type is None or method.return_type.name == 'void':
        return 'void'
    rt = resolver.return_type(method.return_type)
    if returns_pointer(method, resolver) and not rt.endswith('*') and not rt.endswith('&'):
        rt += '*'
    return rt
