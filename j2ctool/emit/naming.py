# -*- coding: utf-8 -*-
"""
naming.py -- identifier conventions for emitted C++.

Clean, CONSISTENT rules (the legacy converter mixed `s_` statics with the odd
bare exception like `blocksList`; we normalize):
  * static fields  -> `s_` prefix
  * instance fields -> bare (decompiled locals are var1/var2, so no collision)
  * leading underscores stripped to avoid reserved `s__name` / `__name`
"""

import re


def static_field_name(name):
    return 's_' + name.lstrip('_')


def instance_field_name(name):
    return name


def field_name(field):
    return static_field_name(field.name) if field.is_static else instance_field_name(field.name)


def guard_macro(class_name):
    """'Vec3D' -> 'VEC3D_H'."""
    return re.sub(r'[^A-Za-z0-9]', '_', class_name).upper() + '_H'


# Enum-constant names that collide with C/C++ macros or keywords (Windows/CRT/xnamath define these).
# A member literally named NULL/TRUE/FALSE expands to a constant -> "C2059 syntax error 'constant'".
# We suffix `_` at BOTH the declaration and the reference so `EnumX::NULL` becomes `EnumX::NULL_`.
_RESERVED_MEMBER = {
    'TRUE', 'FALSE', 'NULL', 'NAN', 'INFINITY', 'ERROR', 'DELETE', 'IN', 'OUT',
    'OPTIONAL', 'CONST', 'VOID', 'NEAR', 'FAR', 'THIS', 'INTERFACE', 'DOMAIN',
    'this', 'new', 'delete', 'class', 'template', 'operator', 'default', 'private',
    'public', 'protected', 'register', 'export', 'try', 'throw', 'namespace',
    'true', 'false', 'nullptr', 'min', 'max',
}


def enum_member(name):
    """Sanitize an enum-constant identifier that collides with a C/C++ macro or keyword.
    Idempotent and applied consistently at declaration and reference sites."""
    return name + '_' if name in _RESERVED_MEMBER else name
