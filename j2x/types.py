# -*- coding: utf-8 -*-
"""
types.py -- Module 3: resolve a Java type to its C++03 form + OWNERSHIP.

Ownership is the decision that fixes the converter's #1 bug class (value vs
pointer -> `.` vs `->`, C2227/C2228). It is decided ONCE, here, from the type --
not patched afterward by regex. `emit` asks this resolver and emits correct code
the first time.

  ownership(jtype) -> 'value'   stack object, accessed with '.'   (int, Vec3D, std::string)
                   -> 'pointer' heap object, accessed with '->'   (Block*, World*, Entity*)

cpp_type(jtype, context) produces the C++ spelling for a field / param / return /
local / generic-or-array element, applying the proven array and string rules.
"""

from . import typemap


class TypeResolver(object):
    def __init__(self, known_classes=None, value_types=None,
                 interface_types=None, enum_types=None, members=None):
        # user-defined classes parsed from the corpus (everything here is a reference type)
        self.known = set(known_classes or ())
        # class names that are C++ VALUE/struct types (stack, '.' access)
        self.values = set(value_types if value_types is not None else typemap.DEFAULT_VALUE_TYPES)
        self.interfaces = set(interface_types or ())
        self.enums = set(enum_types or ())
        # member environment for expression-type inference (chained '.' vs '->'):
        #   {class_name: {'fields': {name: JType}, 'methods': {name: return JType}}}
        # inheritance is already flattened in (closer definitions win). Built by the
        # pipeline from the full corpus; empty in unit tests that don't need chains.
        self.members = members or {}

    # -- classification ------------------------------------------------------
    def is_primitive(self, jtype):
        return jtype.name in typemap.PRIMITIVES

    def ownership(self, jtype):
        """'value' or 'pointer'."""
        if jtype.is_array:
            return 'pointer'                         # Java arrays -> heap buffers (T* / T**)
        n = jtype.name
        if n in typemap.PRIMITIVES:
            return 'value'
        if n in typemap.VALUE_STDLIB:
            return 'value'                           # std::string
        if n in typemap.CONTAINERS:
            return 'value'                           # std::vector / std::map held by value
        if n in self.values:
            return 'value'                           # Vec3D / AxisAlignedBB / ...
        # everything else -- user classes, interfaces, enums, unknown Capitalized -- is a reference
        if (n in self.known or n in self.interfaces or n in self.enums
                or (n[:1].isupper())):
            return 'pointer'
        return 'value'                               # stray lowercase / generic param

    def is_pointer(self, jtype):
        return self.ownership(jtype) == 'pointer'

    def is_value(self, jtype):
        return self.ownership(jtype) == 'value'

    # -- member environment (for chained '.' vs '->' inference) ---------------
    def member_field_type(self, class_name, field_name):
        """JType of class_name's field (own or inherited), or None if unknown."""
        e = self.members.get(class_name)
        return e['fields'].get(field_name) if e else None

    def member_method_type(self, class_name, method_name):
        """Return JType of class_name's method (own or inherited), or None if unknown."""
        e = self.members.get(class_name)
        return e['methods'].get(method_name) if e else None

    def port_note(self, jtype):
        """A PORT message if this type is mapped lossily, else None (for emit's ledger)."""
        return typemap.PORT_NOTE_TYPES.get(jtype.name)

    # -- C++ spelling --------------------------------------------------------
    def base_cpp(self, name):
        return typemap.JAVA_TO_CPP.get(name, name)

    def cpp_type(self, jtype, context='field'):
        """C++03 type for a use site.

        context: 'field' | 'param' | 'return' | 'local' | 'element'
        'element' = inside an array or generic; the caller adds any pointer star.
        """
        # Arrays: a buffer pointer over the element type. A reference-type element is
        # itself a pointer (heap object, nullable), so a ref array is T** -- its slots
        # hold pointers and `arr[i]` is a T* you call through. A value/primitive element
        # is contiguous, so a value array is T*.
        if jtype.is_array:
            elem = jtype.element()
            inner = self.cpp_type(elem, 'element') + '*'
            if self.is_pointer(elem) and not inner.endswith('**'):
                inner += '*'
            return inner

        # Generics: std::vector<Elem(*)>, std::map<...>, etc.
        if jtype.is_generic:
            outer = self.base_cpp(jtype.name)
            args = []
            for a in jtype.args:
                ac = self.cpp_type(a, 'element')
                if self.is_pointer(a) and not ac.endswith('*'):
                    ac += '*'
                args.append(ac)
            return '%s<%s>' % (outer, ', '.join(args))

        name = jtype.name
        # String is passed as const ref (cheap), but RETURNED by value: methods often
        # build a new string (concat), and returning that as const& would dangle. Uses
        # JavaString (a std::string subclass with Java's methods) so s.equals()/s.trim()/...
        # compile on the parameter too.
        if name == 'String' and context == 'param':
            return 'const JavaString&'

        cpp = self.base_cpp(name)
        # raw (non-generic) container -> give it a concrete element type so it's valid C++
        if cpp in typemap.RAW_CONTAINER_DEFAULT:
            cpp += typemap.RAW_CONTAINER_DEFAULT[cpp]
        # Add the ownership pointer for reference types -- except in 'element'
        # context, where the array/generic caller is responsible for the star.
        if context != 'element' and self.ownership(jtype) == 'pointer' and not cpp.endswith('*'):
            cpp += '*'
        return cpp

    # convenience wrappers mirroring the call sites emit will use
    def field_type(self, jtype):
        return self.cpp_type(jtype, 'field')

    def param_type(self, jtype):
        return self.cpp_type(jtype, 'param')

    def return_type(self, jtype):
        return self.cpp_type(jtype, 'return')
