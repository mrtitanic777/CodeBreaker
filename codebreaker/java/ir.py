# -*- coding: utf-8 -*-
"""
ir.py -- the Intermediate Representation: what a Java source file becomes.

This is the CONTRACT between the front-end (`parse/`) and the back-end
(`types`, `emit`). Everything downstream depends on these shapes and nothing
else, so the parser can be reworked without touching emit as long as it keeps
producing valid IR -- and emit can be reworked without touching the parser.

The IR is deliberately dumb: it holds *structure*, not decisions. No C++ types,
no value/pointer calls, no translation. Those belong to `types` and `emit`.
Method bodies are kept as raw source lines (with their original line numbers, so
emit can attribute ledger events back to the Java) until the statement
translator in `emit/` consumes them.
"""

# Access levels, lowest-visibility last. 'package' = Java default (no modifier).
PUBLIC, PROTECTED, PACKAGE, PRIVATE = 'public', 'protected', 'package', 'private'



class JType(object):
    """A Java type reference exactly as written at a use site.

    Examples:
        int                 -> JType('int')
        int[][]             -> JType('int', array_dims=2)
        List<ItemStack>     -> JType('List', args=[JType('ItemStack')])
        String              -> JType('String')
    """
    __slots__ = ('name', 'array_dims', 'args')

    def __init__(self, name, array_dims=0, args=None):
        self.name = name
        self.array_dims = int(array_dims)
        self.args = list(args) if args else []

    @property
    def is_array(self):
        return self.array_dims > 0

    @property
    def is_generic(self):
        return bool(self.args)

    def element(self):
        """The element type of an array (one dimension peeled); self if scalar."""
        if not self.array_dims:
            return self
        return JType(self.name, self.array_dims - 1, self.args)

    def __eq__(self, other):
        return (isinstance(other, JType) and self.name == other.name
                and self.array_dims == other.array_dims and self.args == other.args)

    def __ne__(self, other):
        return not self.__eq__(other)

    def __hash__(self):
        return hash((self.name, self.array_dims, tuple(self.args)))

    def __repr__(self):
        s = self.name
        if self.args:
            s += '<%s>' % ', '.join(repr(a) for a in self.args)
        return s + '[]' * self.array_dims


class Field(object):
    __slots__ = ('name', 'jtype', 'access', 'is_static', 'is_final', 'initializer', 'line')

    def __init__(self, name, jtype, access=PACKAGE, is_static=False, is_final=False,
                 initializer=None, line=0):
        self.name = name
        self.jtype = jtype                  # JType
        self.access = access
        self.is_static = is_static
        self.is_final = is_final
        self.initializer = initializer      # raw Java expression string, or None
        self.line = line

    def __repr__(self):
        mods = ('static ' if self.is_static else '') + ('final ' if self.is_final else '')
        return '<Field %s%s %s>' % (mods, self.jtype, self.name)


class Param(object):
    __slots__ = ('name', 'jtype', 'is_final', 'by_ref')

    def __init__(self, name, jtype, is_final=False):
        self.name = name
        self.jtype = jtype                  # JType
        self.is_final = is_final
        # Set by pipeline.compute_collection_out_params: a Java collection param the method MUTATES, so the
        # emitter must pass it by C++ reference (JList<T>&) -- a by-value copy loses the callee's writes.
        self.by_ref = False

    def __repr__(self):
        return '<Param %s %s>' % (self.jtype, self.name)


class Method(object):
    __slots__ = ('name', 'return_type', 'params', 'access', 'is_static',
                 'is_abstract', 'is_final', 'is_ctor', 'body', 'line')

    def __init__(self, name, return_type=None, params=None, access=PACKAGE,
                 is_static=False, is_abstract=False, is_final=False, is_ctor=False,
                 body=None, line=0):
        self.name = name
        self.return_type = return_type      # JType, or None for constructors / void handled as JType('void')
        self.params = list(params) if params else []
        self.access = access
        self.is_static = is_static
        self.is_abstract = is_abstract
        self.is_final = is_final
        self.is_ctor = is_ctor
        self.body = list(body) if body else []   # list of (line_no, raw_source_line)
        self.line = line

    @property
    def arity(self):
        return len(self.params)

    @property
    def param_types(self):
        return [p.jtype for p in self.params]

    def signature(self):
        """A name+arity+param-type key, for overload-aware lookups."""
        return (self.name, tuple(repr(p.jtype) for p in self.params))

    def __repr__(self):
        kind = 'ctor' if self.is_ctor else (self.return_type or 'void')
        return '<Method %s %s(%d)>' % (kind, self.name, self.arity)


class EnumConst(object):
    __slots__ = ('name', 'args')

    def __init__(self, name, args=None):
        self.name = name
        self.args = list(args) if args else []   # raw Java argument expression strings

    def __repr__(self):
        return '<EnumConst %s(%d)>' % (self.name, len(self.args))


class JClass(object):
    """A parsed Java top-level type."""
    __slots__ = ('name', 'package', 'parent', 'interfaces', 'is_interface',
                 'is_abstract', 'is_enum', 'fields', 'methods', 'enum_consts',
                 'source_path', 'inherited_fields', 'mi_forwards',
                 'static_inits', 'inst_inits', 'has_init_statics')

    def __init__(self, name, package=None, parent=None, interfaces=None,
                 is_interface=False, is_abstract=False, is_enum=False,
                 source_path=None):
        self.name = name
        self.package = package
        self.parent = parent                # str (extends), or None
        self.interfaces = list(interfaces) if interfaces else []
        self.is_interface = is_interface
        self.is_abstract = is_abstract
        self.is_enum = is_enum
        self.fields = []
        self.methods = []
        self.enum_consts = []
        # raw Java bodies of `static { ... }` and instance `{ ... }` initializer blocks, in
        # source order. Static blocks run once at class load; instance blocks run in each ctor.
        self.static_inits = []
        self.inst_inits = []
        self.source_path = source_path
        # instance fields inherited from ancestors (populated by the pipeline from the
        # full class set); used for value/pointer access of inherited fields in bodies.
        self.inherited_fields = []
        # synthesized MI-diamond forwarding overrides (populated by pipeline.compute_mi_forwards):
        # [(interface_Method, base_class_name)] -- an interface pure-virtual a base implements.
        self.mi_forwards = []
        # set by emit.source.render_source: True if initStatics() has a non-empty body (deferred
        # static field inits or static{} blocks) -> the pipeline adds it to the ordered init driver.
        self.has_init_statics = False

    # -- queries used by types/emit -----------------------------------------
    def field(self, name):
        for f in self.fields:
            if f.name == name:
                return f
        return None

    def instance_fields(self):
        return [f for f in self.fields if not f.is_static]

    def static_fields(self):
        return [f for f in self.fields if f.is_static]

    def constructors(self):
        return [m for m in self.methods if m.is_ctor]

    def method(self, name):
        """First method by that name (use signature() for overload precision)."""
        for m in self.methods:
            if m.name == name:
                return m
        return None

    def owns_heap_field(self):
        """True if any field initializer allocates (a Rule-of-Three signal at the IR level)."""
        return any(f.initializer and 'new ' in f.initializer for f in self.fields)

    def is_typed_enum(self):
        """A Java 5 'typed' enum -- constants carry constructor args and/or the enum declares
        instance state/behaviour (EnumToolMaterial, EnumArt, EnumCreatureType, ...). These must
        become real objects (a class with a static instance table), NOT bare int constants.
        A constant-only enum (EnumMobType: `everything, mobs, players;`) stays int constants."""
        if not self.is_enum:
            return False
        return (any(ec.args for ec in self.enum_consts)
                or bool(self.instance_fields())
                or any(not m.is_ctor for m in self.methods))

    def __repr__(self):
        kind = 'interface' if self.is_interface else ('enum' if self.is_enum else 'class')
        ext = (' extends %s' % self.parent) if self.parent else ''
        return '<JClass %s %s%s f=%d m=%d>' % (
            kind, self.name, ext, len(self.fields), len(self.methods))
