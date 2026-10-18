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

import collections

from codebreaker.java import typemap


class OwnershipCatalog(collections.namedtuple(
        'OwnershipCatalog', 'return_methods scalar_methods retained_params corpus_methods')):
    """Whole-program no-GC caller-frees facts the source emitter consults:
      return_methods  -- method names that return a freshly-built OWNED container (Pattern A cleanup)
      scalar_methods  -- {name: cpp_elem_type} of methods that return a fresh heap SCALAR (Pattern B)
      retained_params -- {name: {arg positions}} the method retains -> a temp there must NOT be freed
      corpus_methods  -- every method name in the corpus (so retention of unknown callees stays unsafe)"""
    __slots__ = ()

    @classmethod
    def empty(cls):
        return cls(frozenset(), {}, {}, set())


class TypeResolver(object):
    def __init__(self, known_classes=None, value_types=None,
                 interface_types=None, enum_types=None, members=None, method_renames=None,
                 typed_enum_types=None, thread_like=None):
        # user-defined classes parsed from the corpus (everything here is a reference type)
        self.known = set(known_classes or ())
        # class names that are C++ VALUE/struct types (stack, '.' access)
        self.values = set(value_types if value_types is not None else typemap.DEFAULT_VALUE_TYPES)
        self.interfaces = set(interface_types or ())
        self.enums = set(enum_types or ())
        # TYPED enums (constants carry state/behaviour) -> emitted as real pointer-classes with a
        # static instance table, NOT int constants. Still in `enums` so `Enum.CONST` resolves to
        # `Enum::CONST` (no s_ prefix), but ownership/cpp_type treat them as pointers.
        self.typed_enums = set(typed_enum_types or ())
        # classes whose objects run asynchronously (Thread subclasses / Runnable) -> the leak
        # transform must NEVER stack-allocate them (the object must outlive the started thread).
        self.thread_like = set(thread_like or ())
        # dropped MCP EmptyN marker types -> strip `(EmptyN*)NULL` bridge args at call sites.
        self.markers = set()
        # member environment for expression-type inference (chained '.' vs '->'):
        #   {class_name: {'fields': {name: JType}, 'methods': {name: return JType}}}
        # inheritance is already flattened in (closer definitions win). Built by the
        # pipeline from the full corpus; empty in unit tests that don't need chains.
        self.members = members or {}
        # {class: set(method names colliding with a field)} -> emit those methods as `name_`
        self.method_renames = method_renames or {}
        # whole-program no-GC ownership catalog (caller-frees analysis for the source emitter). Populated
        # by pipeline.build_resolver over the whole corpus; empty in unit tests that don't set it.
        self.own_catalog = OwnershipCatalog.empty()

    def renamed_method(self, class_name, method_name):
        """The C++ emit name for a method: `name_` if it collides with a same-named field in
        class_name's hierarchy (so field and method don't clash), else name unchanged."""
        if method_name in self.method_renames.get(class_name, ()):
            return method_name + '_'
        return method_name

    # -- classification ------------------------------------------------------
    def is_primitive(self, jtype):
        return jtype.name in typemap.PRIMITIVES

    def ownership(self, jtype):
        """'value' or 'pointer'."""
        if jtype.is_array:
            return 'value'                           # Java arrays -> JArray<T> (value wrapper)
        n = jtype.name
        if n in typemap.PRIMITIVES:
            return 'value'
        if n in self.typed_enums:
            return 'pointer'                         # typed enum -> real object (singleton instances)
        if n in self.enums:
            return 'value'                           # constant-only enum -> int (value)
        if n in typemap.VALUE_STDLIB:
            return 'value'                           # std::string
        if n in typemap.CONTAINERS:
            return 'value'                           # std::vector / std::map held by value
        if n in self.values:
            return 'value'                           # Vec3D / AxisAlignedBB / ...
        # Object -> JObject (a boxed VALUE), and boxed primitives (Integer/Character/...) map to
        # C++ primitives -- all value types, not pointers.
        if n == 'Object' or n in typemap.BOXED_VALUE_TYPES:
            return 'value'
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

    def extends_value_container(self, class_name):
        """True if class_name (transitively) derives from a by-value container (JList/JMap/JSet) --
        e.g. argo's J_JsonNodeList : List, J_JsonObjectNodeList : Map. Such a class instance IS the
        container value, so `new X(...)` must be dereferenced where the base container value is wanted."""
        seen = set()
        cur = class_name
        while cur and cur not in seen:
            seen.add(cur)
            e = self.members.get(cur)
            parent = e.get('parent') if e else None
            if not parent:
                return False
            cpp = typemap.JAVA_TO_CPP.get(parent, parent)
            if cpp in ('JList', 'JSet', 'JMap'):
                return True
            cur = parent
        return False

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
        # Arrays: JArray<elem> -- a length-carrying, bounds-checked value wrapper. A
        # reference-type element is itself a pointer (JArray<Block*>); a value/primitive
        # element is stored directly (JArray<int>); a nested array is JArray<JArray<int> >.
        if jtype.is_array:
            elem_jt = jtype.element()
            # Object[] -> JArray<JObject>: a boxed element that can hold a mixed String/char/pointer
            # (Java autoboxing). Scalar Object stays void*; only the array element is boxed.
            if elem_jt.name == 'Object' and not elem_jt.is_array:
                return 'JArray<JObject >'
            # byte[] BUFFER stays unsigned char (chunk/packet/inflate data read via & 0xFF), even
            # though a SCALAR byte is signed char. This is the one place the two byte policies split.
            if elem_jt.name == 'byte' and not elem_jt.is_array:
                return 'JArray<unsigned char >'
            elem = self.cpp_type(elem_jt, 'element')
            if self.ownership(elem_jt) == 'pointer' and not elem.endswith('*'):
                elem += '*'
            return 'JArray<%s >' % elem              # trailing space keeps nested >> valid C++03

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
        if name in self.enums and name not in self.typed_enums:
            return 'int'                             # constant-only enum variable/field/param/return -> int
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
