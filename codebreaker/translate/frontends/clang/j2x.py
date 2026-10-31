"""j2x.py -- C++ written by the Java lane (codebreaker/java: the C++03 back end and its runtime,
platform/canonical) read back as the Java it came from: the library half of the Java -> C++ -> Java round
trip (docs/TRANSLATOR_DESIGN.md, phase 5 exit check).

The clang front end lowers that C++ like any other; what it cannot know is that `JavaString`, `JArray<T>`,
`JCat() + "x" + n`, `System::out.println(...)` and `j2xToInt(d)` are the Java runtime written in C++. This
pass maps them back onto Java's own library in the IR -- the shapes the javac front end gives
(java.lang.String, arrays, Concat, java.io.PrintStream calls, numeric casts) -- so the Java back end
writes Java again:

    JavaString / JCat / std::string     -> java.lang.String          JArray<T>            -> T[]
    JavaString(x)._s, JavaString(x)     -> x                         JCat() + a + b       -> Concat(a, b)
    arr[i] (JArray::operator[])         -> Index                     arr.length / ->length -> Length
    System::out / System::err           -> System.out / System.err   println/print        -> PrintStream's
    j2xToInt / j2xToLong (saturating)   -> Java's (int) / (long)     javaMathSqrt & co    -> Math.sqrt ...
    j2xUshr(v, n)                       -> v >>> n                   s_name (a static)     -> name
    initStatics()                       -> the class's static initialiser
and the sequencing temporaries the C++ back end adds to keep Java's left-to-right order
(`int j2xo1 = 0; ... f((j2xo1 = a, j2xo2 = b, ...))`) are folded back into their declarations.
What it does not know stays as it is, and the back end refuses it with the name.
"""
from codebreaker.translate import ir

STRING = ir.Type('class', 'java.lang.String')
SYSTEM = ir.Type('class', 'java.lang.System')
PRINTSTREAM = ir.Type('class', 'java.io.PrintStream')
MATH = ir.Type('class', 'java.lang.Math')
_STRINGS = ('JavaString', 'JCat', 'std.string', 'std.basic_string', 'StringBuilder')
_MATH = {'javaMathSqrt': 'sqrt', 'javaMathRound': 'round', 'javaMathSignum': 'signum', 'javaMathRandom': 'random'}


def is_j2x(unit):
    """Does this unit come from the Java lane (does it use the Java runtime in C++)?"""
    for n in ir.walk(unit):
        t = getattr(n, 'type', None)
        if isinstance(t, ir.Type) and t.kind == 'class' and t.name in ('JavaString', 'JArray', 'JCat'):
            return True
    return False


def lift_type(t):
    if t is None or not isinstance(t, ir.Type):
        return t
    k = t.kind
    if k == 'class':
        if t.name in _STRINGS:
            return STRING
        if t.name == 'JArray' and t.args:
            return ir.Type('array', elem=lift_type(t.args[0]))
        if t.name == 'System._PrintStream':
            return PRINTSTREAM
        return ir.Type('class', t.name, [lift_type(a) for a in t.args])
    if k == 'ptr':
        e = lift_type(t.elem)
        if e is not None and e.kind in ('array',) or e == STRING:
            return e                                     # (a pointer to an array or String: the reference)
        if t.elem is not None and t.elem.kind in ('char', 'i8') and False:
            return STRING
        return ir.Type('ptr', elem=e, **t.info)
    if k == 'array':
        if t.elem is not None and t.elem.kind in ('char',):
            return STRING                                 # a string literal's char[N]
        return ir.Type('array', elem=lift_type(t.elem), **t.info)
    return t


def _strip(e):
    while isinstance(e, ir.Paren):
        e = e.x
    return e


class Lift(object):
    def __init__(self):
        self.renamed = {}            # (owner, s_name) -> name

    def names(self, u):
        """A static field s_x is x again (the C++ back end's prefix), where no field x exists."""
        for c in [t for t in u.types if isinstance(t, ir.ClassDecl)]:
            names = set(m.sym.name for m in c.members if isinstance(m, ir.FieldDecl) and m.sym)
            for m in c.members:
                if isinstance(m, ir.FieldDecl) and m.sym and 'static' in (m.mods or []) and \
                        m.sym.name.startswith('s_') and m.sym.name[2:] not in names:
                    self.renamed[(c.qname, m.sym.name)] = m.sym.name[2:]

    def unit(self, u):
        u.types = [self.node(t) for t in u.types]
        for c in [t for t in u.types if isinstance(t, ir.ClassDecl)]:
            members = []
            for m in c.members:
                if isinstance(m, ir.MethodDecl) and m.kind == 'dtor' and (m.body is None or not m.body.stmts):
                    continue                              # (Java has no destructor; an empty one says nothing)
                if isinstance(m, ir.MethodDecl) and m.name == 'initStatics' and 'static' in (m.mods or []):
                    m.kind = 'static_init'
                members.append(m)
            c.members = members
        return u

    def sym(self, s):
        if s is None:
            return None
        name = self.renamed.get((s.owner, s.name), s.name)
        return ir.Sym(s.kind, name, s.owner, s.static, lift_type(s.type), [lift_type(p) for p in s.params],
                      lift_type(s.ret), s.varargs)

    def node(self, n):
        if isinstance(n, list):
            return [self.node(x) for x in n]
        if not isinstance(n, ir.Node):
            return n
        if isinstance(n, ir.Block):
            n.stmts = self.fold_temps([self.node(s) for s in n.stmts])
            return n
        for f in n.slots():
            v = getattr(n, f)
            if isinstance(v, (ir.Node, list)):
                setattr(n, f, self.node(v))
            elif isinstance(v, ir.Type):
                setattr(n, f, lift_type(v))
            elif isinstance(v, ir.Sym):
                setattr(n, f, self.sym(v))
        m = getattr(self, 'x_' + type(n).__name__, None)
        return m(n) if m is not None else n

    # ---- expressions (children already lifted)
    def x_Lit(self, e):
        if isinstance(e.value, str):
            e.type = STRING
        return e

    def x_Cast(self, e):
        x = _strip(e.x)
        if e.type == STRING or (isinstance(x, ir.Lit) and isinstance(x.value, str)) or \
                (x.type == STRING and e.type is not None and e.type.kind == 'ptr'):
            return x                                     # (a string's decay or conversion: the string)
        if e.type is not None and e.type.kind == 'array' and x.type is not None and x.type.kind == 'array':
            return x
        return e

    def x_Select(self, e):
        obj = _strip(e.obj)
        if e.sym is not None and e.sym.name == '_s' and (obj.type == STRING or isinstance(obj, ir.Concat)):
            return obj
        if e.sym is not None and e.sym.name == 'length' and obj.type is not None and obj.type.kind == 'array':
            return ir.Length(arr=obj, type=ir.I32, line=e.line, col=e.col)
        if isinstance(obj, ir.Unary) and obj.op == '*' and obj.x.type is not None and obj.x.type.kind == 'array' \
                and e.sym is not None and e.sym.name == 'length':
            return ir.Length(arr=obj.x, type=ir.I32, line=e.line, col=e.col)
        return e

    def x_Name(self, e):
        s = e.sym
        if s is not None and s.kind == 'global' and s.owner == 'System' and s.name in ('out', 'err'):
            return ir.Select(obj=ir.TypeName(type=SYSTEM), sym=ir.Sym('field', s.name, 'java.lang.System', True,
                                                                      PRINTSTREAM), type=PRINTSTREAM,
                             line=e.line, col=e.col)
        return e

    def x_Unary(self, e):
        if e.op == '*' and e.x.type is not None and (e.x.type.kind == 'array' or e.x.type == STRING):
            return e.x                                   # (*arr / *str: the reference itself)
        if e.op == '&' and e.type is not None and e.type.info.get('ref') and e.x.type is not None and \
                (e.x.type.kind == 'array' or e.x.type == STRING):
            return e.x
        return e

    def x_New(self, e):
        t = e.type
        base = t.elem if t is not None and t.kind == 'ptr' else t
        owner = e.sym.owner if e.sym is not None else None
        if owner in _STRINGS or base == STRING:
            if not e.args:
                return ir.Lit(value='', type=STRING, line=e.line, col=e.col)
            if len(e.args) == 1:
                a = _strip(_value(e.args[0]))
                if a.type == STRING or isinstance(a, (ir.Concat, ir.Lit)):
                    if owner == 'JCat':
                        return ir.Concat(parts=[], type=STRING, line=e.line, col=e.col)
                    return a
        if owner == 'JCat' and not e.args:
            return ir.Concat(parts=[], type=STRING, line=e.line, col=e.col)
        if owner and owner.startswith('JArray') or (base is not None and base.kind == 'array'):
            at = base if base is not None and base.kind == 'array' else lift_type(ir.Type('class', 'JArray'))
            if len(e.args or []) == 1 and e.args[0].type is not None and e.args[0].type.kind in ('i32', 'i64', 'u32', 'u64'):
                return ir.NewArray(elem=at.elem, dims=[e.args[0]], init=None, type=at, line=e.line, col=e.col)
        return e

    def x_Call(self, e):
        s = e.sym
        name = s.name if s is not None else None
        args = [_value(a) for a in e.args or []]        # (the runtime's helpers take const T&: the value)
        target = _strip(e.target) if e.target is not None else None
        if name == 'operator+' and (s.owner == 'JCat' or isinstance(target, ir.Concat) or
                                    (target is not None and target.type == STRING and s.owner in (None, 'JCat'))):
            t = _strip(e.target)
            parts = list(t.parts) if isinstance(t, ir.Concat) else [t]
            a = _strip(args[0]) if args else None
            if isinstance(a, ir.Unary) and a.op == '&':
                a = _strip(a.x)
            parts.append(a)
            return ir.Concat(parts=parts, type=STRING, line=e.line, col=e.col)
        if name == 'operator[]' and e.target is not None and e.target.type is not None and e.target.type.kind == 'array':
            return ir.Index(arr=e.target, index=args[0], type=e.type, line=e.line, col=e.col)
        if e.target is None and name in ('j2xToInt', 'j2xToLong'):
            return ir.Cast(x=args[0], type=ir.Type('i32' if name == 'j2xToInt' else 'i64'), line=e.line, col=e.col)
        if e.target is None and name == 'j2xUshr':
            return ir.Binary(op='>>>', left=args[0], right=args[1], type=e.type, line=e.line, col=e.col)
        if e.target is None and name in _MATH:
            return ir.Call(target=ir.TypeName(type=MATH), sym=ir.Sym('method', _MATH[name], 'java.lang.Math', True,
                                                                     params=s.params, ret=s.ret),
                           args=args, typeargs=[], type=e.type, line=e.line, col=e.col)
        if name in ('println', 'print') and e.target is not None and e.target.type == PRINTSTREAM:
            if args:
                a = _strip(args[0])
                if isinstance(a, ir.Unary) and a.op == '&':
                    a = a.x
                args = [a]
            return ir.Call(target=e.target, sym=ir.Sym('method', name, 'java.io.PrintStream', False,
                                                       params=[a.type for a in args], ret=ir.VOID),
                           args=args, typeargs=[], type=ir.VOID, line=e.line, col=e.col)
        return e

    def x_Binary(self, e):
        if e.op == ',':
            return e
        return e

    # ---- the sequencing temporaries
    def fold_temps(self, stmts):
        """`int j2xo1 = 0; ...; S((j2xo1 = a, j2xo2 = b, X))` -> `int j2xo1 = a; ...; S(X)`."""
        out = list(stmts)
        i = 0
        while i < len(out):
            s = out[i]
            temps = {}
            j = i
            while j < len(out) and isinstance(out[j], ir.Local) and out[j].sym is not None and \
                    out[j].sym.name.startswith('j2xo'):
                temps[out[j].sym.name] = j
                j += 1
            if temps and j < len(out):
                host = out[j]
                comma = _first_comma(host)
                if comma is not None:
                    parts = _commas(comma[0])
                    assigns, last = parts[:-1], parts[-1]
                    if all(isinstance(a, ir.Assign) and a.op is None and isinstance(_strip(a.target), ir.Name) and
                           _strip(a.target).sym.name in temps for a in assigns) and \
                            len(assigns) == len(temps):
                        order = []
                        for a in assigns:
                            loc = out[temps[_strip(a.target).sym.name]]
                            loc.init = a.value
                            order.append(loc)
                        out[i:j] = order
                        comma[1](last)
                        i += len(order)
                        continue
            i += 1
        return out


def _value(a):
    """An argument bound to a reference parameter (Unary '&' typed ref): the value it binds."""
    x = _strip(a)
    if isinstance(x, ir.Unary) and x.op == '&' and x.type is not None and x.type.info.get('ref'):
        return x.x
    return a


def _commas(e):
    e = _strip(e)
    if isinstance(e, ir.Binary) and e.op == ',':
        return _commas(e.left) + _commas(e.right)
    return [e]


def _first_comma(stmt):
    """The first comma expression in a statement, with a function that puts something in its place."""
    found = []

    def visit(node, setter):
        if found:
            return
        if isinstance(node, ir.Paren):
            visit(node.x, setter)
            return
        if isinstance(node, ir.Binary) and node.op == ',':
            found.append((node, setter))
            return
        if isinstance(node, ir.Node):
            for f in node.fields:
                v = getattr(node, f)
                if isinstance(v, ir.Node):
                    visit(v, lambda x, n=node, f=f: setattr(n, f, x))
                elif isinstance(v, list):
                    for k, x in enumerate(v):
                        if isinstance(x, ir.Node):
                            visit(x, lambda y, lst=v, k=k: lst.__setitem__(k, y))
    visit(stmt, None)
    return found[0] if found else None


def lift(units):
    """The Java lane's C++ (its units) read back as Java's library, in place."""
    lf = Lift()
    for u in units:
        lf.names(u)
    for u in units:
        lf.unit(u)
    return units
