"""jmodel.py -- a Java program's object model read off L2, for the back ends that write whole programs
(cpp17.Program and c_java.Program).

What every such back end has to know and Java leaves implicit: every class (nested, local, anonymous and
enum-constant bodies included) with its C name, its supertypes, whether it needs its outer instance, what
a local or anonymous class captures; which method a method overrides (so its erased signature is the one
the root declares, as javac's bridge methods make it); the one abstract method of a functional interface;
which classes can form a reference cycle (where reference counting leaks and a collector is needed).

Library types are named by their qualified Java name; LIB_SUPERS gives their supertypes and LIB_METHODS the
methods a program class can override, each as javac would type it (type variables erase to Object).
"""
from codebreaker.translate import ir

OBJECT = 'java.lang.Object'
_T = ir.Type('typevar', 'T')
_STR = ir.Type('class', 'java.lang.String')
_OBJ = ir.Type('class', OBJECT)
_I = ir.I32
_B = ir.BOOL
_V = ir.VOID

# library supertypes (enough for subtype tests and for override roots)
LIB_SUPERS = {
    'java.lang.String': ['java.lang.CharSequence', 'java.lang.Comparable'],
    'java.util.ArrayList': ['java.util.List'], 'java.util.LinkedList': ['java.util.List', 'java.util.Deque'],
    'java.util.List': ['java.util.Collection'], 'java.util.Set': ['java.util.Collection'],
    'java.util.HashSet': ['java.util.Set'], 'java.util.LinkedHashSet': ['java.util.Set'],
    'java.util.TreeSet': ['java.util.Set'], 'java.util.Collection': ['java.lang.Iterable'],
    'java.util.Deque': ['java.util.Queue'], 'java.util.Queue': ['java.util.Collection'],
    'java.util.ArrayDeque': ['java.util.Deque'],
    'java.util.HashMap': ['java.util.Map'], 'java.util.LinkedHashMap': ['java.util.HashMap'],
    'java.util.TreeMap': ['java.util.Map'],
    'java.lang.Exception': ['java.lang.Throwable'], 'java.lang.Error': ['java.lang.Throwable'],
    'java.lang.RuntimeException': ['java.lang.Exception'],
    'java.lang.IllegalArgumentException': ['java.lang.RuntimeException'],
    'java.lang.IllegalStateException': ['java.lang.RuntimeException'],
    'java.lang.ArithmeticException': ['java.lang.RuntimeException'],
    'java.lang.NullPointerException': ['java.lang.RuntimeException'],
    'java.lang.ClassCastException': ['java.lang.RuntimeException'],
    'java.lang.UnsupportedOperationException': ['java.lang.RuntimeException'],
    'java.lang.IndexOutOfBoundsException': ['java.lang.RuntimeException'],
    'java.lang.ArrayIndexOutOfBoundsException': ['java.lang.IndexOutOfBoundsException'],
    'java.lang.StringIndexOutOfBoundsException': ['java.lang.IndexOutOfBoundsException'],
    'java.lang.NumberFormatException': ['java.lang.IllegalArgumentException'],
    'java.util.NoSuchElementException': ['java.lang.RuntimeException'],
    'java.io.IOException': ['java.lang.Exception'],
    'java.lang.Integer': ['java.lang.Number', 'java.lang.Comparable'],
    'java.lang.Long': ['java.lang.Number', 'java.lang.Comparable'],
    'java.lang.Double': ['java.lang.Number', 'java.lang.Comparable'],
    'java.lang.Enum': ['java.lang.Comparable'], 'java.lang.Record': [],
    'java.util.function.UnaryOperator': ['java.util.function.Function'],
    'java.util.function.BinaryOperator': ['java.util.function.BiFunction'],
}

# the one abstract method of each library functional interface: (name, params, result), as erased
SAMS = {
    'java.lang.Runnable': ('run', [], _V),
    'java.util.function.Function': ('apply', [_T], _T),
    'java.util.function.BiFunction': ('apply', [_T, _T], _T),
    'java.util.function.UnaryOperator': ('apply', [_T], _T),
    'java.util.function.BinaryOperator': ('apply', [_T, _T], _T),
    'java.util.function.Supplier': ('get', [], _T),
    'java.util.function.Consumer': ('accept', [_T], _V),
    'java.util.function.BiConsumer': ('accept', [_T, _T], _V),
    'java.util.function.Predicate': ('test', [_T], _B),
    'java.util.function.BiPredicate': ('test', [_T, _T], _B),
    'java.util.function.IntBinaryOperator': ('applyAsInt', [_I, _I], _I),
    'java.util.function.IntUnaryOperator': ('applyAsInt', [_I], _I),
    'java.util.function.IntPredicate': ('test', [_I], _B),
    'java.util.function.IntFunction': ('apply', [_I], _T),
    'java.util.function.IntConsumer': ('accept', [_I], _V),
    'java.util.function.IntSupplier': ('getAsInt', [], _I),
    'java.util.function.ToIntFunction': ('applyAsInt', [_T], _I),
    'java.util.function.ToLongFunction': ('applyAsLong', [_T], ir.Type('i64')),
    'java.util.function.ToDoubleFunction': ('applyAsDouble', [_T], ir.Type('f64')),
    'java.util.function.IntToDoubleFunction': ('applyAsDouble', [_I], ir.Type('f64')),
    'java.util.function.IntToLongFunction': ('applyAsLong', [_I], ir.Type('i64')),
    'java.util.function.DoubleUnaryOperator': ('applyAsDouble', [ir.Type('f64')], ir.Type('f64')),
    'java.util.function.DoubleBinaryOperator': ('applyAsDouble', [ir.Type('f64'), ir.Type('f64')], ir.Type('f64')),
    'java.util.function.LongBinaryOperator': ('applyAsLong', [ir.Type('i64'), ir.Type('i64')], ir.Type('i64')),
    'java.util.function.LongUnaryOperator': ('applyAsLong', [ir.Type('i64')], ir.Type('i64')),
    'java.util.Comparator': ('compare', [_T, _T], _I),
}

# methods of library supertypes a program class can override: {type: [(name, params, result)]}
LIB_METHODS = {
    OBJECT: [('toString', [], _STR), ('equals', [_OBJ], _B), ('hashCode', [], _I)],
    'java.lang.Comparable': [('compareTo', [_T], _I)],
    'java.lang.Iterable': [('iterator', [], ir.Type('class', 'java.util.Iterator')), ('forEach', [_T], _V)],
    'java.util.Iterator': [('hasNext', [], _B), ('next', [], _T), ('remove', [], _V)],
    'java.lang.Throwable': [('getMessage', [], _STR)],
    'java.lang.CharSequence': [('length', [], _I), ('charAt', [_I], ir.Type('u16'))],
}
for _q, (_n, _p, _r) in SAMS.items():
    LIB_METHODS.setdefault(_q, []).append((_n, _p, _r))


class ClassInfo(object):
    """One class of the program."""

    def __init__(self, decl, qname, cname, outer=None, local=False, anon=False):
        self.decl = decl
        self.qname = qname
        self.cname = cname
        self.outer = outer                    # the enclosing ClassInfo (lexically)
        self.local = local                    # declared in a method body
        self.anon = anon
        self.kind = decl.kind
        static = 'static' in (decl.mods or []) or decl.kind in ('interface', 'enum', 'record', 'annotation')
        # an inner class (not static, inside a class) holds its outer instance; so does a local or
        # anonymous class declared where `this` exists (set by Model once it knows the method)
        self.inner = outer is not None and not static and not local and not anon and outer.kind != 'interface'
        self.captures = []                    # [(name, Type)]: the locals a local/anonymous class reads
        self.needs_outer = self.inner
        self.base = None                      # a qualified name: program or library
        self.ifaces = []
        self.enum_const = None                # an enum constant's body: the EnumConst

    @property
    def members(self):
        return self.decl.members

    def methods(self, name=None):
        return [m for m in self.decl.members if isinstance(m, ir.MethodDecl) and m.kind in ('method', 'ctor')
                and (name is None or m.name == name)]

    def fields(self):
        return [m for m in self.decl.members if isinstance(m, ir.FieldDecl)]

    def __repr__(self):
        return '<class %s>' % self.cname


def erase(t):
    """A type with its type variables and wildcards as Object (Java's erasure, near enough: a bounded
    variable is Object here too, and its uses are cast to what javac said)."""
    if t is None:
        return ir.VOID
    if t.kind in ('typevar', 'wildcard', 'any'):
        return _OBJ
    if t.kind == 'array':
        return ir.Type('array', elem=erase(t.elem))
    if t.kind == 'class' and t.args:
        return ir.Type('class', t.name)
    return t


class Model(object):
    def __init__(self, units, package_names=True):
        self.units = units
        self.classes = {}                     # qname (or a made-up key) -> ClassInfo
        self.by_decl = {}
        self._anon = 0
        self.packages = set(u.package for u in units if u.package)
        for u in units:
            for c in u.types:
                self._add(c, None, u.package)
        for ci in list(self.classes.values()):
            self._supers(ci)
        for ci in list(self.classes.values()):
            if ci.local or ci.anon:
                self._capture(ci)

    # ---- the classes
    def _cname(self, qname, package):
        q = qname
        if package and q.startswith(package + '.'):
            q = q[len(package) + 1:]
        return q.replace('.', '_').replace('$', '_')

    def _add(self, c, outer, package, local=False, anon=False, method=None):
        qname = c.qname or ''
        if anon or not qname or local:
            self._anon += 1
            base = outer.cname if outer is not None else 'Local'
            cname = '%s_%s%d' % (base, 'anon' if anon else (c.name or 'local'), self._anon)
            key = qname if (qname and not anon) else '<%s>' % cname
        else:
            cname = self._cname(qname, package)
            key = qname
        ci = ClassInfo(c, key, cname, outer, local, anon)
        ci.package = package
        ci.method = method                    # the method a local/anonymous class sits in
        if (local or anon) and method is not None and outer is not None:
            ci.needs_outer = not (method.sym is not None and method.sym.static) and method.kind != 'static_init'
        self.classes[key] = ci
        self.by_decl[id(c)] = ci
        for m in c.members:
            if isinstance(m, ir.ClassDecl):
                self._add(m, ci, package)
            elif isinstance(m, ir.EnumConst) and m.body is not None:
                sub = self._add(m.body, ci, package, anon=True)
                sub.enum_const = m
                sub.needs_outer = False
                sub.base = ci.qname
            elif isinstance(m, (ir.MethodDecl, ir.FieldDecl)):
                self._nested_in(m, ci, package)
        return ci

    def _nested_in(self, m, ci, package):
        """Local and anonymous classes inside a member's code."""
        method = m if isinstance(m, ir.MethodDecl) else None
        root = m.body if isinstance(m, ir.MethodDecl) else m.init
        if root is None:
            return
        stack = [root]
        while stack:
            n = stack.pop()
            if isinstance(n, ir.LocalClass):
                self._add(n.decl, ci, package, local=True, method=method)
                continue
            if isinstance(n, ir.New) and n.body is not None:
                for a in (n.args or []):
                    stack.append(a)
                if n.outer is not None:
                    stack.append(n.outer)
                sub = self._add(n.body, ci, package, anon=True, method=method)
                sub.new = n
                if method is None:                         # a field initialiser: an instance one has `this`
                    sub.needs_outer = not (m.sym is not None and m.sym.static)
                continue
            stack.extend(n.children())

    def _supers(self, ci):
        d = ci.decl
        if ci.anon and getattr(ci, 'new', None) is not None:
            t = d.extends if d.extends is not None else ci.new.type
            target = t.name if t is not None and t.kind == 'class' else OBJECT
            tci = self.classes.get(target)
            if (tci is not None and tci.kind == 'interface') or (tci is None and target in SAMS) \
                    or (tci is None and target in LIB_METHODS and target != OBJECT and target != 'java.lang.Throwable'):
                ci.base, ci.ifaces = OBJECT, [target]
            else:
                ci.base = target
            return
        if ci.enum_const is not None:
            return
        if d.kind == 'enum':
            ci.base = 'java.lang.Enum'
        elif d.kind == 'record':
            ci.base = OBJECT
        elif d.extends is not None and d.extends.kind == 'class':
            ci.base = d.extends.name
        else:
            ci.base = OBJECT if d.kind != 'interface' else None
        ci.ifaces = [t.name for t in (d.implements or []) if t.kind == 'class']
        if d.kind == 'interface' and d.extends is not None:
            ci.ifaces.insert(0, d.extends.name)

    def _capture(self, ci):
        """The locals of the enclosing method a local or anonymous class reads (Java copies them in)."""
        declared = set()
        used = []
        for n in ir.walk(ci.decl):
            if isinstance(n, (ir.Local, ir.Param)) and n.sym is not None:
                declared.add(id(n.sym)) if False else declared.add((n.sym.name, n.line, n.col))
        names_declared = set(x[0] for x in declared)
        for n in ir.walk(ci.decl):
            if isinstance(n, ir.Name) and n.sym is not None and n.sym.name not in names_declared:
                if n.sym.name not in [u[0] for u in used]:
                    used.append((n.sym.name, n.type))
        ci.captures = used

    def name_anonymous(self):
        """javac names an anonymous (or local) class's members as owned by '' -- give them the class's key,
        so a field or method of one is found like any other's."""
        for ci in self.classes.values():
            if not (ci.anon or ci.local):
                continue
            stack = list(ci.decl.members)
            while stack:
                n = stack.pop()
                if isinstance(n, ir.ClassDecl) and n is not ci.decl:
                    continue                                   # its own class does its members
                s = getattr(n, 'sym', None)
                if isinstance(s, ir.Sym) and s.owner == '' and s.kind in ('field', 'method', 'ctor'):
                    s.owner = ci.qname
                t = getattr(n, 'type', None)
                if isinstance(n, ir.This) and isinstance(t, ir.Type) and t.kind == 'class' and not t.name:
                    n.type = ir.Type('class', ci.qname)
                if isinstance(n, ir.MethodDecl) and n.body is not None:
                    stack.append(n.body)
                elif isinstance(n, ir.FieldDecl) and n.init is not None:
                    stack.append(n.init)
                elif isinstance(n, ir.New) and n.body is not None:
                    stack.extend(n.args or [])
                    if n.outer is not None:
                        stack.append(n.outer)
                else:
                    stack.extend(n.children())

    # ---- lookups
    def info(self, qname):
        return self.classes.get(qname) if qname else None

    def of_type(self, t):
        return self.classes.get(t.name) if t is not None and t.kind == 'class' else None

    def supers(self, qname):
        """The direct supertypes of a class, program or library."""
        ci = self.classes.get(qname)
        if ci is not None:
            return ([ci.base] if ci.base else []) + list(ci.ifaces)
        return list(LIB_SUPERS.get(qname, [])) + ([OBJECT] if qname != OBJECT else [])

    def ancestors(self, qname):
        out, todo = [], [qname]
        while todo:
            q = todo.pop(0)
            if q in out:
                continue
            out.append(q)
            todo.extend(self.supers(q))
        return out

    def is_sub(self, a, b):
        return b == OBJECT or b in self.ancestors(a)

    def is_interface(self, qname):
        ci = self.classes.get(qname)
        if ci is not None:
            return ci.kind == 'interface'
        return qname in SAMS or qname in ('java.lang.Comparable', 'java.lang.Iterable', 'java.util.Iterator',
                                          'java.lang.CharSequence', 'java.util.List', 'java.util.Map',
                                          'java.util.Set', 'java.util.Collection')

    def subclassed(self, qname):
        return any(ci.base == qname for ci in self.classes.values())

    def method_decl(self, sym):
        """The program's MethodDecl a resolved method symbol names, searching up the supertypes, or None."""
        if sym is None:
            return None, None
        for q in self.ancestors(sym.owner):
            ci = self.classes.get(q)
            if ci is None:
                continue
            for m in ci.methods(sym.name if sym.kind == 'method' else '<init>'):
                if len(m.params) == len(sym.params) and all(erase(p.type) == erase(t) for p, t in zip(m.params, sym.params)):
                    return ci, m
            if sym.kind == 'ctor':
                break
        return None, None

    def overrides(self, ci, m):
        """The root signature method `m` of class `ci` overrides: (params, result) as the topmost declaration
        has them, or None when it overrides nothing (or is static, private, a constructor)."""
        if m.kind != 'method' or (m.sym is not None and m.sym.static) or 'private' in (m.mods or []):
            return None
        found = None
        seen = set()
        todo = list(self.supers(ci.qname))
        while todo:
            q = todo.pop(0)
            if q in seen:
                continue
            seen.add(q)
            sci = self.classes.get(q)
            if sci is not None:
                for sm in sci.methods(m.name):
                    if sm.kind == 'method' and len(sm.params) == len(m.params) and \
                            all(_matches(sp.type, p.type) for sp, p in zip(sm.params, m.params)) \
                            and not (sm.sym is not None and sm.sym.static) and 'private' not in (sm.mods or []):
                        found = ([sp.type for sp in sm.params], sm.ret)
            else:
                for name, ps, r in LIB_METHODS.get(q, []):
                    if name == m.name and len(ps) == len(m.params) and \
                            all(_matches(sp, p.type) for sp, p in zip(ps, m.params)):
                        found = (list(ps), r)
            todo.extend(self.supers(q))
        return found

    def sam(self, qname):
        """(name, params, result) of a functional interface's one abstract method, or None."""
        if qname in SAMS:
            return SAMS[qname]
        ci = self.classes.get(qname)
        if ci is None or ci.kind != 'interface':
            return None
        abstract = []
        for q in self.ancestors(qname):
            sci = self.classes.get(q)
            if sci is None:
                if q in SAMS:
                    abstract.append(SAMS[q])
                continue
            if sci.kind != 'interface':
                continue
            for m in sci.methods():
                if m.body is None and not (m.sym is not None and m.sym.static) and \
                        m.name not in ('toString', 'equals', 'hashCode'):
                    if not any(a[0] == m.name and len(a[1]) == len(m.params) for a in abstract):
                        abstract.append((m.name, [p.type for p in m.params], m.ret))
        return abstract[0] if len(abstract) == 1 else None

    def cycles(self):
        """Classes whose instance fields can lead back to an instance of the same class (directly or
        through other program classes, arrays and collections): reference counting cannot free such a
        cycle; a collector (Boehm) would be needed. -> sorted C names"""
        graph = {}
        for ci in self.classes.values():
            out = set()
            for f in ci.fields():
                if f.sym is None or f.sym.static:
                    continue
                for q in _refs(f.type):
                    if q in self.classes:
                        out.add(q)
                    elif q in ('java.lang.Object', 'java.util.List', 'java.util.ArrayList', 'java.util.Map',
                               'java.util.HashMap', 'java.util.Set', 'java.util.HashSet', 'java.util.function.Function',
                               'java.util.function.Supplier', 'java.lang.Runnable'):
                        out.add('*')                       # anything at all: it may hold this class again
            if ci.needs_outer and ci.outer is not None:
                out.add(ci.outer.qname)
            graph[ci.qname] = out
        result = []
        for q in graph:
            seen, todo = set(), list(graph[q])
            hit = False
            while todo:
                x = todo.pop()
                if x == q or x == '*':
                    hit = True
                    break
                if x in seen:
                    continue
                seen.add(x)
                todo.extend(graph.get(x, ()))
                for sub, ci in self.classes.items():       # a field of type X may hold a subclass of X
                    if ci.base == x and sub not in seen:
                        todo.append(sub)
            if hit:
                result.append(self.classes[q].cname)
        return sorted(result)


def _matches(super_t, t):
    """A parameter of an overridden method (super_t, as declared) and of the overriding one (t)."""
    if super_t is None or t is None:
        return False
    if super_t.kind in ('typevar', 'wildcard'):
        return t.is_ref
    return erase(super_t) == erase(t)


def _refs(t):
    """The class names a field type can hold a reference to."""
    out = []
    if t is None:
        return out
    if t.kind == 'array':
        return _refs(t.elem)
    if t.kind == 'class':
        out.append(t.name)
        for a in t.args:
            out += _refs(a)
    if t.kind in ('typevar', 'wildcard'):
        out.append(OBJECT)
    return out
