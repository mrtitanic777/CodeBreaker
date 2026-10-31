"""cs2cpp.py -- C# into the C++03 back end: the C# kit's L2 written by backends/cpp03.py.

The C++03 back end (the Xbox 360 path) is L2-driven for bodies only. Three things about it are still
Java's, and this module is what a C# source has to cross to reach it (docs/TRANSLATOR_DESIGN.md, wave 3:
"checks the IR is not Java-shaped"):

  1. its declarations come from Java source text: the transpiler (codebreaker/java) parses .java files
     for classes, fields and signatures, and asks the back end only for bodies. So a C# program is given
     a Java declaration skeleton, written here from its L2 (signatures, no bodies), and the transpiler's
     front-end call is handed the C# kit's L2 units instead of javac's;
  2. its runtime is Java's library in C++ (JavaString, JList, java.lang.Math, System.out): C#'s library
     calls are rebound to the Java calls of the same meaning (Console.WriteLine -> System.out.println,
     List<T> -> java.util.ArrayList, s.Length -> s.length(), ...), and C#'s text conversions are
     written out where Java's differ (a bool is "True"/"False" in C#: a conditional);
  3. it knows no properties: a C# property becomes a field and get/set methods, and every read and
     write of it a call (C#'s own extension lowered into the core, as the design asks).

What has no faithful form on that path is refused with the reason, never guessed: structs (value
semantics), records, generics of the program's own, delegates and lambdas (C++03), unsigned integers
(Java's runtime has none), nested classes (the transpiler's body lookup is by top-level class), using,
lock, goto, checked arithmetic, doubles turned into text (C#'s format is not Java's), and any BCL call
not in the table.

    from codebreaker.translate.dotnet import cs2cpp
    cpp_dir, refused = cs2cpp.convert(units, work)       # units from the Roslyn front end
"""
import os
import shutil

from codebreaker.translate import ir

_PRIM_JAVA = {'bool': 'boolean', 'i8': 'byte', 'i16': 'short', 'u16': 'char', 'i32': 'int', 'i64': 'long',
              'f32': 'float', 'f64': 'double', 'void': 'void'}
_BOXED = {'bool': 'java.lang.Boolean', 'i8': 'java.lang.Byte', 'i16': 'java.lang.Short', 'u16': 'java.lang.Character',
          'i32': 'java.lang.Integer', 'i64': 'java.lang.Long', 'f32': 'java.lang.Float', 'f64': 'java.lang.Double'}
_CLASSES = {
    'System.String': 'java.lang.String', 'System.Object': 'java.lang.Object',
    'System.Collections.Generic.List': 'java.util.ArrayList',
    'System.Exception': 'java.lang.RuntimeException', 'System.InvalidOperationException': 'java.lang.IllegalStateException',
    'System.ArgumentException': 'java.lang.IllegalArgumentException',
    'System.DivideByZeroException': 'java.lang.ArithmeticException', 'System.ArithmeticException': 'java.lang.ArithmeticException',
    'System.NullReferenceException': 'java.lang.NullPointerException',
    'System.IndexOutOfRangeException': 'java.lang.ArrayIndexOutOfBoundsException',
    'System.Math': 'java.lang.Math',
}
_MATH = {'Sqrt': 'sqrt', 'Abs': 'abs', 'Max': 'max', 'Min': 'min', 'Sin': 'sin', 'Cos': 'cos', 'Floor': 'floor',
         'Pow': 'pow', 'Atan2': 'atan2', 'Tan': 'tan', 'Exp': 'exp', 'Log': 'log'}
STRING = ir.Type('class', 'java.lang.String')


class Refuse(Exception):
    def __init__(self, kind, why):
        Exception.__init__(self, why)
        self.kind, self.why = kind, why


def _m(name, owner, params, ret, static=False):
    return ir.Sym('method', name, owner, static, params=params, ret=ret)


class ToJava(object):
    """Rebind C# L2 to Java's library and lower C#'s properties into methods, in place."""

    def __init__(self, units):
        self.units = units
        self.classes = {}
        for u in units:
            for c in u.types:
                if isinstance(c, ir.ClassDecl):
                    self.classes[c.qname] = c
        self.props = {}                                    # (owner, name) -> (getter Sym, setter Sym or None, static)
        self.auto = {}                                     # (owner, name) -> an auto-property's field
        self.refused = []

    # ---- types
    def ty(self, t, boxed=False):
        if t is None:
            return None
        k = t.kind
        if k in ('u8', 'u32', 'u64'):
            raise Refuse('unsigned', 'an unsigned integer (%s): Java and its C++ runtime have none' % k)
        if k in _PRIM_JAVA:
            return ir.Type('class', _BOXED[k]) if boxed and k != 'void' else ir.Type(k)
        if k == 'array':
            if t.info.get('rank', 1) != 1:
                raise Refuse('array', 'a rectangular array')
            return ir.Type('array', elem=self.ty(t.elem))
        if k == 'class':
            if t.name.startswith('java.'):                       # (already Java's)
                return t
            if t.name == 'System.Nullable':
                raise Refuse('nullable', 'a nullable value type')
            if t.name in self.classes:
                c = self.classes[t.name]
                if c.kind not in ('class', 'interface'):
                    raise Refuse(c.kind, 'a C# %s: no faithful form on the C++03 path' % c.kind)
                return ir.Type('class', t.name)
            if t.name in _CLASSES:
                return ir.Type('class', _CLASSES[t.name], [self.ty(a, boxed=True) for a in t.args])
            raise Refuse('library', 'the .NET type %s has no mapping onto Java\'s runtime' % t.name)
        if k in ('null', 'any'):
            return t
        if k == 'typevar':
            raise Refuse('generic', 'a type parameter (%s)' % t.name)
        raise Refuse('type', 'the type %r' % (t,))

    def sym(self, s):
        if s is None:
            return None
        owner = s.owner
        if owner in _CLASSES:
            owner = _CLASSES[owner]
        params = [self.ty(p) for p in s.params]
        return ir.Sym(s.kind, s.name, owner, s.static, self.ty(s.type) if s.type is not None else None, params,
                      self.ty(s.ret) if s.ret is not None else None, s.varargs)

    # ---- the program
    def run(self):
        for u in self.units:
            kept = []
            for c in u.types:
                try:
                    self.check_class(c)
                    kept.append(c)
                except Refuse as r:
                    self.refused.append((c.name, r.kind, r.why))
            u.types = kept
        for c in self.classes.values():
            try:
                self.collect_props(c)
            except Refuse as r:
                self.refused.append((c.name, r.kind, r.why))
        for u in self.units:
            for c in u.types:
                try:
                    self.klass(c)
                except Refuse as r:
                    self.refused.append((c.name, r.kind, r.why))
            u.lang = 'java'
        return self.refused

    def check_class(self, c):
        if c.kind not in ('class', 'interface'):
            raise Refuse(c.kind, 'a C# %s: no faithful form on the C++03 path' % c.kind)
        if c.typeparams:
            raise Refuse('generic', 'a generic class (%s)' % c.name)
        for m in c.members:
            if isinstance(m, ir.ClassDecl):
                raise Refuse('nested', 'a nested class (%s.%s): the transpiler finds bodies by top-level class'
                             % (c.name, m.name))
            if isinstance(m, ir.Unsupported):
                raise Refuse(m.kind, m.raw or m.kind)

    def collect_props(self, c):
        for m in c.members:
            if isinstance(m, ir.Property):
                if m.params:
                    raise Refuse('indexer', 'an indexer')
                t = self.ty(m.type)
                static = m.sym.static
                g = _m('get' + m.sym.name, c.qname, [], t, static)
                writable = any(a.kind in ('set', 'init') for a in m.accessors or [])
                s = _m('set' + m.sym.name, c.qname, [t], ir.VOID, static) if writable else None
                self.props[(c.qname, m.sym.name)] = (g, s, static)
                if all(a.body is None for a in m.accessors or []) and 'abstract' not in (m.mods or [])                         and c.kind != 'interface':
                    self.auto[(c.qname, m.sym.name)] = ir.Sym('field', '_' + m.sym.name, c.qname, static, type=t)

    def prop(self, owner, name):
        """A property's accessors, looked up through the classes it is inherited from."""
        seen = set()
        while owner and owner not in seen:
            seen.add(owner)
            if (owner, name) in self.props:
                return self.props[(owner, name)]
            c = self.classes.get(owner)
            if c is None:
                break
            owner = c.extends.name if c.extends is not None else None
            for i in (c.implements or []):
                if (i.name, name) in self.props:
                    return self.props[(i.name, name)]
        return None

    def klass(self, c):
        c.mods = [m for m in c.mods if m in ('public', 'abstract')] + (['final'] if 'sealed' in c.mods else [])
        if c.extends is not None:
            c.extends = self.ty(c.extends)
        c.implements = [self.ty(i) for i in c.implements or []]
        out = []
        for m in c.members:
            try:
                out += self.member(m, c)
            except Refuse as r:
                self.refused.append(('%s.%s' % (c.name, getattr(m, 'name', None) or getattr(m.sym, 'name', '?')),
                                     r.kind, r.why))
                if isinstance(m, ir.MethodDecl) and m.body is not None:
                    m.body = ir.Block(stmts=[ir.ExprStmt(expr=ir.Unsupported(kind=r.kind, raw=r.why, type=ir.ANY))])
                    out.append(m)
        c.members = out

    def mods(self, mods):
        out = []
        for m in mods or []:
            if m in ('public', 'private', 'protected', 'static', 'abstract'):
                out.append(m)
            elif m == 'const':
                out += ['static', 'final']
            elif m == 'sealed':
                out.append('final')
        return out

    def member(self, m, c):
        if isinstance(m, ir.FieldDecl):
            m.sym = self.sym(m.sym)
            m.type = self.ty(m.type)
            m.mods = self.mods(m.mods)
            if m.init is not None:
                m.init = self.expr(m.init)
            return [m]
        if isinstance(m, ir.Property):
            return self.lower_prop(m, c)
        if isinstance(m, ir.MethodDecl):
            return [self.method(m, c)]
        if isinstance(m, ir.EnumConst):
            raise Refuse('enum', 'an enum')
        raise Refuse('member', 'a member %s' % type(m).__name__)

    def lower_prop(self, p, c):
        if (c.qname, p.sym.name) not in self.props:
            raise Refuse('property', 'the property %s.%s (its type has no form here)' % (c.name, p.sym.name))
        g, s, static = self.props[(c.qname, p.sym.name)]
        t = g.ret
        out = []
        mods = self.mods([x for x in p.mods or [] if x not in ('override', 'virtual')])
        abstract = 'abstract' in (p.mods or []) or c.kind == 'interface'
        auto = not abstract and all(a.body is None for a in p.accessors or [])
        field = None
        if auto:
            fs = ir.Sym('field', '_' + p.sym.name, c.qname, static, type=t)
            field = fs
            out.append(ir.FieldDecl(sym=fs, type=t, init=self.expr(p.init) if p.init is not None else None,
                                    mods=['private'] + (['static'] if static else []), line=p.line, col=p.col))
        for a in p.accessors or []:
            sym = g if a.kind == 'get' else s
            if sym is None:
                continue
            amods = [x for x in mods if x not in ('public', 'private', 'protected')]
            amods = self.mods(a.mods) + amods if a.mods else mods
            if abstract:
                body = None
                amods = [x for x in amods if x != 'abstract'] + (['abstract'] if c.kind != 'interface' else [])
            elif a.body is None:                                        # the compiler's own accessor
                fsel = ir.Select(obj=ir.TypeName(type=ir.Type('class', c.qname)) if static else
                                 ir.This(qualifier=None, type=ir.Type('class', c.qname)), sym=field, type=t)
                if a.kind == 'get':
                    body = ir.Block(stmts=[ir.Return(expr=fsel)])
                else:
                    v = ir.Name(sym=ir.Sym('param', 'value', c.qname + '.set' + p.sym.name, type=t), type=t)
                    body = ir.Block(stmts=[ir.ExprStmt(expr=ir.Assign(op=None, target=fsel, value=v, type=t))])
            else:
                body = self.block(a.body)
            params = [] if a.kind == 'get' else [ir.Param(sym=ir.Sym('param', 'value', c.qname + '.set' + p.sym.name, type=t),
                                                          type=t, mods=[])]
            out.append(ir.MethodDecl(sym=sym, name=sym.name, kind='method', params=params, ret=sym.ret, body=body,
                                     mods=amods, throws=[], typeparams=[], line=a.line or p.line, col=a.col or p.col))
        return out

    def method(self, m, c):
        name = {'ToString': 'toString', 'Equals': 'equals', 'GetHashCode': 'hashCode'}.get(m.name, m.name)
        if m.kind == 'method' and m.name == 'Main' and 'static' in (m.mods or []):
            name = 'main'
            if not m.params:
                st = ir.Type('array', elem=STRING)
                m.params = [ir.Param(sym=ir.Sym('param', 'args', c.qname + '.main', type=st), type=st, mods=[])]
        if 'operator' in (m.mods or []):
            raise Refuse('operator', 'an operator')
        m.name = name
        m.sym = self.sym(m.sym) if m.sym is not None else None
        if m.sym is not None:
            m.sym.name = name
            if name == 'main':
                m.sym.params = (ir.Type('array', elem=STRING),)
        for p in m.params or []:
            if p.sym is not None and p.sym.refs and any(p.sym.refs):
                raise Refuse('ref', 'a ref or out parameter')
            p.type = self.ty(p.type)
            p.sym = self.sym(p.sym)
        m.ret = self.ty(m.ret)
        m.mods = self.mods(m.mods) if m.kind != 'ctor' else [x for x in self.mods(m.mods) if x != 'static'] + \
            (['IMPLICIT'] if 'IMPLICIT' in (m.mods or []) else [])
        if c.kind == 'interface':
            m.mods = [x for x in m.mods if x != 'abstract']
        if m.body is not None:
            m.body = self.block(m.body)
        return m

    # ---- statements
    def block(self, b):
        return ir.Block(stmts=[self.stmt(s) for s in b.stmts], line=b.line, col=b.col)

    def stmt(self, s):
        k = type(s).__name__
        if isinstance(s, ir.Block):
            return self.block(s)
        if k == 'Local':
            s.sym, s.type = self.sym(s.sym), self.ty(s.type)
            if s.init is not None:
                s.init = self.expr(s.init)
            return s
        if k == 'ExprStmt':
            e = s.expr
            if isinstance(e, ir.Assign) and isinstance(e.target, ir.Select) and e.target.sym is not None \
                    and e.target.sym.kind == 'property':
                return ir.ExprStmt(expr=self.set_prop(e), line=s.line, col=s.col)
            if isinstance(e, ir.Unary) and e.op in ('++', '--') and isinstance(e.x, ir.Select) and e.x.sym is not None \
                    and e.x.sym.kind == 'property':
                one = ir.Lit(value=1, type=ir.I32)
                return ir.ExprStmt(expr=self.set_prop(ir.Assign(op=e.op[0], target=e.x, value=one, type=e.type)),
                                   line=s.line, col=s.col)
            if isinstance(e, ir.Assign) and isinstance(e.target, ir.Index) and e.target.sym is not None and e.op is None:
                return ir.ExprStmt(expr=self.set_index(e), line=s.line, col=s.col)
            s.expr = self.expr(e)
            return s
        if k == 'If':
            s.cond, s.then = self.expr(s.cond), self.stmt(s.then)
            s.orelse = self.stmt(s.orelse) if s.orelse is not None else None
            return s
        if k == 'While':
            s.cond, s.body = self.expr(s.cond), self.stmt(s.body)
            return s
        if k == 'DoWhile':
            s.body, s.cond = self.stmt(s.body), self.expr(s.cond)
            return s
        if k == 'For':
            s.init = [self.stmt(x) for x in s.init or []]
            s.cond = self.expr(s.cond) if s.cond is not None else None
            s.update = [self.expr(x) for x in s.update or []]
            s.body = self.stmt(s.body)
            return s
        if k == 'ForEach':
            return self.foreach(s)
        if k == 'Switch' and s.expr.type is not None and s.expr.type.kind == 'class' and s.expr.type.name == 'System.String':
            return self.string_switch(s)
        if k == 'Switch':
            s.expr = self.expr(s.expr)
            for c in s.cases or []:
                c.labels = [self.expr(x) for x in c.labels or []]
                c.body = [self.stmt(x) for x in c.body or []]
            return s
        if k in ('Break', 'Continue', 'Empty', 'Comment'):
            if k in ('Break', 'Continue') and s.label:
                raise Refuse('label', 'a labelled jump')
            return s
        if k == 'Return':
            s.expr = self.expr(s.expr) if s.expr is not None else None
            return s
        if k == 'Throw':
            if s.expr is None:
                raise Refuse('rethrow', '`throw;`')
            s.expr = self.expr(s.expr)
            return s
        if k == 'Try':
            if s.resources:
                raise Refuse('using', 'using (IDisposable): the C++03 runtime has no Dispose to call')
            s.body = self.block(s.body)
            for c in s.catches or []:
                c.sym = self.sym(c.sym)
                c.types = [self.ty(t) for t in c.types or []]
                c.body = self.block(c.body)
            s.final = self.block(s.final) if s.final is not None else None
            return s
        if k == 'CtorCall':
            s.sym = self.sym(s.sym)
            s.args = [self.expr(a) for a in s.args or []]
            return s
        raise Refuse(k, 'the statement %s on the C++03 path' % k)

    def foreach(self, s):
        """foreach over an array or a List: a counted loop (the C++03 back end writes no for-each; decompiled
        Java never has one). Over a List it is the same while the body does not change the list, where C#
        would throw."""
        var = self.stmt(s.var)
        coll = self.expr(s.iterable)
        if not isinstance(coll, (ir.Name, ir.Select, ir.This)) or isinstance(coll, ir.Select) and not isinstance(
                coll.obj, (ir.Name, ir.This, ir.TypeName)):
            raise Refuse('foreach', 'foreach over an expression with effects')
        self.loops = getattr(self, 'loops', 0) + 1
        i = ir.Sym('local', '_i%d' % self.loops, var.sym.owner, type=ir.I32)
        iv = ir.Name(sym=i, type=ir.I32)
        ct = coll.type
        if ct is not None and ct.kind == 'array':
            n = ir.Length(arr=coll, type=ir.I32)
            item = ir.Index(arr=coll, index=iv, type=var.type)
        elif ct is not None and ct.kind == 'class' and ct.name == 'java.util.ArrayList':
            n = ir.Call(target=coll, sym=_m('size', 'java.util.ArrayList', [], ir.I32), args=[], typeargs=[], type=ir.I32)
            get = ir.Call(target=coll, sym=_m('get', 'java.util.ArrayList', [ir.I32], ir.Type('typevar', 'E')), args=[iv],
                          typeargs=[], type=var.type)
            item = get
        elif ct is not None and ct.kind == 'class' and ct.name == 'java.lang.String':
            n = ir.Call(target=coll, sym=_m('length', 'java.lang.String', [], ir.I32), args=[], typeargs=[], type=ir.I32)
            item = ir.Call(target=coll, sym=_m('charAt', 'java.lang.String', [ir.I32], ir.Type('u16')), args=[iv],
                           typeargs=[], type=ir.Type('u16'))
        else:
            raise Refuse('foreach', 'foreach over a %r' % (ct,))
        var.init = item
        body = self.stmt(s.body)
        stmts = [var] + (body.stmts if isinstance(body, ir.Block) else [body])
        return ir.For(init=[ir.Local(sym=i, type=ir.I32, init=ir.Lit(value=0, type=ir.I32), mods=[])],
                      cond=ir.Binary(op='<', left=iv, right=n, type=ir.BOOL),
                      update=[ir.Unary(op='++', x=iv, postfix=False, type=ir.I32)],
                      body=ir.Block(stmts=stmts), line=s.line, col=s.col)

    def string_switch(self, s):
        """switch on a string: the C++03 back end switches on integers only (decompiled Java switches on the
        hash). As tests in order: each C# section ends in a jump, so none falls into the next."""
        v = self.expr(s.expr)
        if not isinstance(v, (ir.Name, ir.This)) and not (isinstance(v, ir.Select) and isinstance(v.obj, (ir.Name, ir.This, ir.TypeName))):
            raise Refuse('switch', 'a switch on a string expression with effects')
        chain, default = [], None
        pend = []
        for c in s.cases or []:
            pend += c.labels or []
            if not c.body:
                if c.default:
                    raise Refuse('switch', 'a default label sharing a section')
                continue
            body = list(c.body)
            if body and isinstance(body[-1], ir.Break) and not body[-1].label:
                body = body[:-1]
            for x in body:
                for n in ir.walk(x):
                    if isinstance(n, (ir.Break, ir.Continue)):
                        raise Refuse('switch', 'a break or continue inside a string switch section')
            body = [self.stmt(x) for x in body]
            if c.default:
                default = body
            else:
                tests = None
                for lab in pend:
                    eq = self.call(v, _m('equals', 'java.lang.String', [ir.Type('class', 'java.lang.Object')], ir.BOOL),
                                   [self.expr(lab)], ir.BOOL)
                    tests = eq if tests is None else ir.Binary(op='||', left=tests, right=eq, type=ir.BOOL)
                chain.append((tests, body))
            pend = []
        out = ir.Block(stmts=default) if default is not None else None
        for test, body in reversed(chain):
            out = ir.If(cond=test, then=ir.Block(stmts=body), orelse=out, line=s.line, col=s.col)
        return out if out is not None else ir.Empty()

    def set_prop(self, e):
        t = e.target
        acc = self.prop(t.sym.owner, t.sym.name)
        field = self.auto.get((t.sym.owner, t.sym.name))
        if acc is not None and acc[1] is None and field is not None and e.op is None:
            # a get-only auto-property set in its constructor: its field
            ft = field.type
            return ir.Assign(op=None, target=ir.Select(obj=self.expr(t.obj), sym=field, type=ft), value=self.expr(e.value),
                             type=ft)
        if acc is None or acc[1] is None:
            raise Refuse('property', 'setting %s.%s' % (t.sym.owner, t.sym.name))
        g, s, static = acc
        obj = self.expr(t.obj)
        value = self.expr(e.value)
        if e.op is not None:
            if not isinstance(obj, (ir.This, ir.TypeName, ir.Name)):
                raise Refuse('property', 'a compound assignment to a property of an expression')
            cur = ir.Call(target=obj, sym=g, args=[], typeargs=[], type=g.ret)
            value = ir.Binary(op=e.op, left=cur, right=value, type=g.ret)
            if g.ret is not None and g.ret.kind in ('i8', 'i16', 'u16'):
                value = ir.Cast(x=value, type=g.ret)
        return ir.Call(target=obj, sym=s, args=[value], typeargs=[], type=ir.VOID)

    def set_index(self, e):
        t = e.target
        owner = t.sym.owner
        if owner == 'System.Collections.Generic.List':
            s = _m('set', 'java.util.ArrayList', [ir.I32, ir.Type('typevar', 'E')], ir.Type('typevar', 'E'))
            return ir.Call(target=self.expr(t.arr), sym=s, args=[self.expr(t.index), self.expr(e.value)], typeargs=[],
                           type=self.ty(t.type))
        raise Refuse('indexer', 'an indexer of %s' % owner)

    # ---- expressions
    def expr(self, e):
        k = type(e).__name__
        f = getattr(self, 'x_' + k, None)
        if f is None:
            raise Refuse(k, 'the expression %s on the C++03 path' % k)
        return f(e)

    def _typed(self, e):
        if getattr(e, 'type', None) is not None:
            e.type = self.ty(e.type)
        return e

    def x_Lit(self, e):
        return self._typed(e)

    def x_Paren(self, e):
        e.x = self.expr(e.x)
        return self._typed(e)

    def x_Name(self, e):
        e.sym = self.sym(e.sym)
        return self._typed(e)

    def x_This(self, e):
        return self._typed(e)

    def x_Super(self, e):
        return self._typed(e)

    def x_TypeName(self, e):
        return self._typed(e)

    def x_Select(self, e):
        s = e.sym
        if s.kind == 'property':
            if s.owner == 'System.String' and s.name == 'Length':
                return self.call(self.expr(e.obj), _m('length', 'java.lang.String', [], ir.I32), [], ir.I32)
            if s.owner == 'System.Collections.Generic.List' and s.name == 'Count':
                return self.call(self.expr(e.obj), _m('size', 'java.util.ArrayList', [], ir.I32), [], ir.I32)
            if s.owner == 'System.Exception' and s.name == 'Message':
                raise Refuse('runtime', "an exception's Message: the C++03 runtime's exceptions keep no message "
                             "(RuntimeException(...) {})")
            acc = self.prop(s.owner, s.name)
            if acc is None:
                raise Refuse('library', 'the .NET property %s.%s' % (s.owner, s.name))
            g = acc[0]
            obj = self.expr(e.obj)
            return ir.Call(target=None if isinstance(obj, ir.This) else obj, sym=g, args=[], typeargs=[], type=g.ret,
                           line=e.line, col=e.col)
        if s.owner not in self.classes:
            raise Refuse('library', 'the .NET field %s.%s' % (s.owner, s.name))
        e.obj = self.expr(e.obj)
        e.sym = self.sym(s)
        return self._typed(e)

    def call(self, target, sym, args, t):
        return ir.Call(target=target, sym=sym, args=args, typeargs=[], type=t)

    def x_Length(self, e):
        e.arr = self.expr(e.arr)
        return self._typed(e)

    def x_Index(self, e):
        if e.sym is not None:
            if e.sym.owner == 'System.String':
                return self.call(self.expr(e.arr), _m('charAt', 'java.lang.String', [ir.I32], ir.Type('u16')),
                                 [self.expr(e.index)], ir.Type('u16'))
            if e.sym.owner == 'System.Collections.Generic.List':
                t = self.ty(e.type)
                return self.call(self.expr(e.arr), _m('get', 'java.util.ArrayList', [ir.I32], ir.Type('typevar', 'E')),
                                 [self.expr(e.index)], t)
            raise Refuse('indexer', 'an indexer of %s' % e.sym.owner)
        if isinstance(e.index, list):
            raise Refuse('array', 'a rectangular array')
        e.arr, e.index = self.expr(e.arr), self.expr(e.index)
        return self._typed(e)

    def x_Unary(self, e):
        if e.overflow == 'trap':
            raise Refuse('checked', 'checked arithmetic')
        if e.op in ('++', '--') and isinstance(e.x, ir.Select) and e.x.sym is not None and e.x.sym.kind == 'property':
            raise Refuse('property', '++/-- on a property inside an expression')
        e.x = self.expr(e.x)
        return self._typed(e)

    def x_Binary(self, e):
        if e.overflow == 'trap':
            raise Refuse('checked', 'checked arithmetic')
        if e.op == '??':
            raise Refuse('coalesce', '??')
        e.left, e.right = self.expr(e.left), self.expr(e.right)
        return self._typed(e)

    def x_Assign(self, e):
        if isinstance(e.target, ir.Select) and e.target.sym is not None and e.target.sym.kind == 'property':
            raise Refuse('property', 'a property assigned inside an expression')
        if e.op == '??':
            raise Refuse('coalesce', '??=')
        e.target, e.value = self.expr(e.target), self.expr(e.value)
        return self._typed(e)

    def x_Cond(self, e):
        e.cond, e.then, e.orelse = self.expr(e.cond), self.expr(e.then), self.expr(e.orelse)
        return self._typed(e)

    def x_Cast(self, e):
        if e.overflow == 'trap':
            raise Refuse('checked', 'a checked conversion')
        e.x = self.expr(e.x)
        return self._typed(e)

    def x_InstanceOf(self, e):
        if e.binding is not None:
            raise Refuse('pattern', 'a type pattern that declares a name')
        e.x = self.expr(e.x)
        e.test = self.ty(e.test)
        return self._typed(e)

    def x_New(self, e):
        t = self.ty(e.type)
        e.sym = self.sym(e.sym)
        if e.sym is not None and e.type.name in _CLASSES:
            e.sym.owner = _CLASSES[e.type.name]
        e.args = [self.expr(a) for a in e.args or []]
        e.type = t
        return e

    def x_NewArray(self, e):
        e.elem = self.ty(e.elem)
        e.dims = [self.expr(d) for d in e.dims or []]
        e.init = [self.expr(x) for x in e.init] if e.init is not None else None
        return self._typed(e)

    def x_Concat(self, e):
        e.parts = [self.text(p) for p in e.parts]
        return self._typed(e)

    def text(self, p):
        """p as C# turns it into text, written in terms Java's concatenation gives the same."""
        t = p.type
        if t is not None and t.kind == 'bool':
            return ir.Cond(cond=self.expr(p), then=ir.Lit(value='True', type=STRING),
                           orelse=ir.Lit(value='False', type=STRING), type=STRING)
        if t is not None and t.kind in ('f32', 'f64'):
            raise Refuse('double-text', 'a %s turned into text: C#\'s format is not Java\'s' % t.kind)
        if t is not None and t.kind == 'class' and t.name not in ('System.String',):
            if t.name in self.classes:
                ts = _m('toString', t.name, [], STRING)
                return ir.Call(target=self.expr(p), sym=ts, args=[], typeargs=[], type=STRING)
            raise Refuse('object-text', 'a %s turned into text' % t.name)
        return self.expr(p)

    def x_Call(self, e):
        s = e.sym
        o = s.owner
        args = e.args or []
        if o == 'System.Console' and s.name in ('WriteLine', 'Write'):
            out = ir.Select(obj=ir.TypeName(type=ir.Type('class', 'java.lang.System')),
                            sym=ir.Sym('field', 'out', 'java.lang.System', True, type=ir.Type('class', 'java.io.PrintStream')),
                            type=ir.Type('class', 'java.io.PrintStream'))
            name = 'println' if s.name == 'WriteLine' else 'print'
            if not args:
                return self.call(out, _m(name, 'java.io.PrintStream', [], ir.VOID), [], ir.VOID)
            if len(args) != 1:
                raise Refuse('library', 'Console.%s with a format' % s.name)
            a = self.text(args[0])
            pt = a.type if a.type is not None and a.type.kind in ('i32', 'i64', 'u16') else STRING
            return self.call(out, _m(name, 'java.io.PrintStream', [pt], ir.VOID), [a], ir.VOID)
        if o == 'System.Math' and s.name in _MATH:
            return ir.Call(target=ir.TypeName(type=ir.Type('class', 'java.lang.Math')),
                           sym=_m(_MATH[s.name], 'java.lang.Math', [self.ty(p) for p in s.params], self.ty(s.ret), True),
                           args=[self.expr(a) for a in args], typeargs=[], type=self.ty(e.type))
        if o == 'System.String':
            if 'operator' in (s.flags or ()) and s.name in ('op_Equality', 'op_Inequality'):
                l, r = args
                if any(isinstance(x, ir.Lit) and x.value is None for x in (l, r)):
                    return ir.Binary(op='==' if s.name == 'op_Equality' else '!=', left=self.expr(l), right=self.expr(r),
                                     type=ir.BOOL)
                eq = self.call(self.expr(l), _m('equals', 'java.lang.String', [ir.Type('class', 'java.lang.Object')], ir.BOOL),
                               [self.expr(r)], ir.BOOL)
                return eq if s.name == 'op_Equality' else ir.Unary(op='!', x=eq, postfix=False, type=ir.BOOL)
            table = {('ToUpper', 0): 'toUpperCase', ('ToLower', 0): 'toLowerCase', ('Contains', 1): 'contains',
                     ('Substring', 1): 'substring', ('Equals', 1): 'equals', ('Trim', 0): 'trim'}
            name = table.get((s.name, len(args)))
            if name is None and s.name == 'Substring' and len(args) == 2 and isinstance(args[0], (ir.Lit, ir.Name)):
                end = ir.Binary(op='+', left=self.expr(args[0]), right=self.expr(args[1]), type=ir.I32)
                return self.call(self.expr(e.target), _m('substring', 'java.lang.String', [ir.I32, ir.I32], STRING),
                                 [self.expr(args[0]), end], STRING)
            if name is None:
                raise Refuse('library', 'string.%s/%d' % (s.name, len(args)))
            return self.call(self.expr(e.target), _m(name, 'java.lang.String', [self.ty(p) for p in s.params],
                                                     self.ty(s.ret)), [self.expr(a) for a in args], self.ty(e.type))
        if o == 'System.Collections.Generic.List':
            el = ir.Type('typevar', 'E')
            table = {('Add', 1): ('add', [el], ir.BOOL), ('Contains', 1): ('contains', [ir.Type('class', 'java.lang.Object')], ir.BOOL),
                     ('Clear', 0): ('clear', [], ir.VOID), ('Insert', 2): ('add', [ir.I32, el], ir.VOID),
                     ('RemoveAt', 1): ('remove', [ir.I32], el), ('IndexOf', 1): ('indexOf', [ir.Type('class', 'java.lang.Object')], ir.I32)}
            got = table.get((s.name, len(args)))
            if got is None:
                raise Refuse('library', 'List.%s/%d' % (s.name, len(args)))
            name, params, ret = got
            return self.call(self.expr(e.target), _m(name, 'java.util.ArrayList', params, ret),
                             [self.expr(a) for a in args], self.ty(e.type) if e.type is not None and e.type.kind != 'void' else ir.VOID)
        if o == 'System.Object' and s.name == 'ToString' and not args:
            return self.call(self.expr(e.target), _m('toString', 'java.lang.Object', [], STRING), [], STRING)
        if o not in self.classes:
            raise Refuse('library', 'the .NET call %s.%s' % (o, s.name))
        if 'operator' in (s.flags or ()) or 'extension' in (s.flags or ()) or 'delegate' in (s.flags or ()):
            raise Refuse('call', 'an operator, extension or delegate call')
        e.sym = self.sym(s)
        e.sym.name = {'ToString': 'toString', 'Equals': 'equals', 'GetHashCode': 'hashCode'}.get(s.name, s.name)
        if e.target is not None:
            e.target = self.expr(e.target)
        e.args = [self.expr(a) for a in args]
        return self._typed(e)

    def x_Lambda(self, e):
        raise Refuse('lambda', 'a lambda: the C++03 back end has none')

    def x_MethodRef(self, e):
        raise Refuse('delegate', 'a method group as a delegate')

    def x_Unsupported(self, e):
        raise Refuse(e.kind, e.raw or e.kind)


# ---------------------------------------------------------------------------------------------- Java text
def _jtype(t):
    if t is None:
        return 'void'
    if t.kind in _PRIM_JAVA:
        return _PRIM_JAVA[t.kind]
    if t.kind == 'array':
        return _jtype(t.elem) + '[]'
    if t.kind == 'class':
        n = t.name.rsplit('.', 1)[-1]
        if t.args:
            n += '<' + ', '.join(_jtype(a) for a in t.args) + '>'
        return n
    if t.kind == 'typevar':
        return t.name
    return 'Object'


def _default(t):
    if t is None or t.kind == 'void':
        return ''
    if t.kind == 'bool':
        return ' false'
    if t.kind in _PRIM_JAVA:
        return ' 0'
    return ' null'


def skeleton(c):
    """The Java declarations of a class (no bodies: the transpiler takes those from L2)."""
    pkg = c.qname[:-len(c.name)].rstrip('.') if c.qname.endswith(c.name) else ''
    lines = []
    if pkg:
        lines.append('package %s;' % pkg)
    lines += ['import java.util.ArrayList;', '']
    head = ' '.join(c.mods + ['interface' if c.kind == 'interface' else 'class', c.name])
    if c.extends is not None:
        head += ' extends ' + _jtype(c.extends)
    if c.implements:
        head += (' extends ' if c.kind == 'interface' else ' implements ') + ', '.join(_jtype(i) for i in c.implements)
    lines.append(head + ' {')
    for m in c.members:
        if isinstance(m, ir.FieldDecl):
            lines.append('\t%s %s %s;' % (' '.join(m.mods), _jtype(m.type), m.sym.name))
        elif isinstance(m, ir.MethodDecl):
            ps = ', '.join('%s %s' % (_jtype(p.type), p.sym.name) for p in m.params or [])
            mods = [x for x in m.mods if x != 'IMPLICIT']
            if m.kind == 'ctor':
                if 'IMPLICIT' in (m.mods or []):
                    continue
                lines.append('\t%s %s(%s) {\n\t}' % (' '.join(mods), c.name, ps))
            elif m.kind == 'static_init':
                lines.append('\tstatic {\n\t}')
            elif m.body is None:
                lines.append('\t%s %s %s(%s);' % (' '.join(mods), _jtype(m.ret), m.name, ps))
            else:
                ret = _default(m.ret)
                lines.append('\t%s %s %s(%s) {\n\t\treturn%s;\n\t}' % (' '.join(mods), _jtype(m.ret), m.name, ps, ret))
    lines.append('}')
    return '\n'.join(l.replace('\t ', '\t') for l in lines) + '\n'


# ---------------------------------------------------------------------------------------------- the conversion
def convert(units, work):
    """C# L2 units -> the C++03 back end's program in work/cpp. -> (cpp_dir, [refusals], {'tree_bodies': ...})"""
    from codebreaker.java import transpile
    from codebreaker.translate.backends import cpp03
    tj = ToJava(units)
    refused = tj.run()
    src = os.path.join(work, 'java-skeleton')
    out = os.path.join(work, 'cpp')
    for d in (src, out):
        shutil.rmtree(d, ignore_errors=True)
        os.makedirs(d)
    for u in units:
        for c in u.types:
            with open(os.path.join(src, c.name + '.java'), 'w', encoding='utf-8') as f:
                f.write(skeleton(c))
    saved, saved_bodies = cpp03.java_units, cpp03.Bodies
    seen = []

    class Bodies(saved_bodies):                                  # (kept, for what it declined and why)
        def __init__(self, *a, **k):
            saved_bodies.__init__(self, *a, **k)
            seen.append(self)
    cpp03.java_units = lambda java_dir, classpath=(): units        # the C# kit's L2, not javac's
    cpp03.Bodies = Bodies
    try:
        stats = transpile.convert(src, out, platform=None, write_report=False)
    finally:
        cpp03.java_units, cpp03.Bodies = saved, saved_bodies
    tree = seen[0] if seen else None
    for why, n in (tree.reasons.items() if tree is not None else []):
        refused.append(('C++03 back end', 'declined', '%d bodies: %s' % (n, why)))
    for why, n in (tree.declined.items() if tree is not None else []):
        if why == 'not found' or not tree.reasons:
            refused.append(('C++03 back end', 'declined', '%d bodies: %s' % (n, why)))
    _host_print(out)
    return out, refused, stats


def _host_print(cpp_dir):
    """The C++03 runtime's System.out is a no-op (a console has no standard output); for the judge on the
    host, a PrintStream that writes, with Java's text for each kind of value."""
    p = os.path.join(cpp_dir, 'JavaRT.h')
    with open(p, encoding='utf-8') as f:
        text = f.read()
    old = '''	struct _PrintStream {
		template<class T> void println(const T&) {}
		void println() {}
		template<class T> void print(const T&) {}
	};
	static _PrintStream out;
	static _PrintStream err;'''
    new = '''	struct _PrintStream {                     // (the judge's host PrintStream: CodeBreaker's C# kit)
		FILE* f;
		explicit _PrintStream(FILE* to) : f(to) {}
		void put(const std::string& s) { fwrite(s.data(), 1, s.size(), f); }
		template<class T> void print(const T& x) { std::ostringstream o; o << x; put(o.str()); }
		void print(bool b) { put(b ? "true" : "false"); }
		void print(char c) { put(std::string(1, c)); }
		void print(double d) { put(j2xNumberText(d, false)); }
		void print(float d) { put(j2xNumberText(d, true)); }
		template<class T> void println(const T& x) { print(x); put("\\n"); }
		void println() { put("\\n"); }
	};
	static _PrintStream out(stdout);
	static _PrintStream err(stderr);'''
    if old in text:
        text = text.replace(old, new)
        if '#include <sstream>' not in text:
            text = '#include <sstream>\n#include <cstdio>\n' + text
        with open(p, 'w', encoding='utf-8') as f:
            f.write(text)
        return True
    return False
