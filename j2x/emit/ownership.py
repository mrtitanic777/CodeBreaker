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

_CTOR_NEW = re.compile(r'(?:this\.)?(\w+)\s*=\s*new\s+\w[\w.]*\s*(\[)?')
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
            if fld and not fld.is_static:
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
    text = '\n'.join(t for _ln, t in method.body)
    return re.search(r'\breturn\s+new\s+%s\b' % re.escape(rt.name), text) is not None


def return_cpp_type(method, resolver):
    """The C++ return type for a method, applying dual-use promotion. Shared by
    header.py and source.py so the declaration and definition always agree."""
    if method.return_type is None or method.return_type.name == 'void':
        return 'void'
    rt = resolver.return_type(method.return_type)
    if returns_pointer(method, resolver) and not rt.endswith('*') and not rt.endswith('&'):
        rt += '*'
    return rt
