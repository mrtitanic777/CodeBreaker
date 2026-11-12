"""lower.py -- Roslyn's bound tree (CsTreeDump's JSON) into the translator's IR (codebreaker/translate/ir.py).

Declarations are lowered from the symbols the compiler gave them; bodies from Roslyn's IOperation tree,
in which every call already names its overload and lists its arguments in parameter order (defaults and
`params` arrays filled in), every implicit conversion is a node, and `foreach`, `using`, `lock`, string
interpolation, patterns and LINQ's query syntax are already what they mean. So:

  * a name is lowered from the symbol it binds to (a field read on this or on its class, a local, a
    parameter, a property, an indexer), never from its spelling;
  * implicit conversions that every target makes the same way (widening, boxing, identity) are left
    implicit, as javac's tree leaves them; explicit ones are Casts (`checked`: overflow 'trap');
    user-defined ones, and user-defined operators, are calls of their op_ methods (Call, flags
    'operator'), as are the BCL's operators that are methods (string ==, decimal); `as` is a test and a
    cast;
  * C#'s own kinds of member and type keep their shape: a property or indexer is a Property declaration
    and is read as a Select (an indexer as an Index with its sym); a struct, a record, a delegate are
    ClassDecls of that kind; an enum constant carries its number (EnumConst.value);
  * what needs a statement where C# has an expression (an object or collection initialiser, a switch
    expression on a value with effects, an `out var` declaration) is hoisted before the statement --
    only where that cannot change what runs first: never out of a branch that might not run, a loop
    condition, or past an earlier effect in the same statement.

What cannot be carried faithfully yet becomes ir.Unsupported(kind, reason), never a guess: `await`,
iterators (`yield return`), tuples and deconstruction, local functions, lifted nullable arithmetic,
pattern-matching `switch` statements, `goto case`, exception filters, events, unsafe code, `dynamic`.
"""
from codebreaker.translate import ir

LANG = 'csharp'

_PRIM = {'bool': 'bool', 'sbyte': 'i8', 'byte': 'u8', 'short': 'i16', 'ushort': 'u16', 'int': 'i32', 'uint': 'u32',
         'long': 'i64', 'ulong': 'u64', 'float': 'f32', 'double': 'f64'}
_BINARY = {'Add': '+', 'Subtract': '-', 'Multiply': '*', 'Divide': '/', 'Remainder': '%', 'LeftShift': '<<',
           'RightShift': '>>', 'UnsignedRightShift': '>>>', 'And': '&', 'Or': '|', 'ExclusiveOr': '^',
           'ConditionalAnd': '&&', 'ConditionalOr': '||', 'Equals': '==', 'NotEquals': '!=', 'LessThan': '<',
           'LessThanOrEqual': '<=', 'GreaterThan': '>', 'GreaterThanOrEqual': '>=', 'ObjectValueEquals': '==',
           'ObjectValueNotEquals': '!='}
_UNARY = {'Minus': '-', 'Plus': '+', 'Not': '!', 'BitwiseNegation': '~'}
_REF = {'None': '', 'Ref': 'ref', 'Out': 'out', 'In': 'in', 'RefReadOnly': 'in', 'RefReadOnlyParameter': 'in'}
STRING = ir.Type('class', 'System.String')
OBJECT = ir.Type('class', 'System.Object')


# -------------------------------------------------------------------------------------- types
def ty(d):
    """A CsTreeDump type record -> ir.Type."""
    if d is None:
        return ir.ANY
    k = d['k']
    if k == 'prim':
        n = d['n']
        if n == 'void':
            return ir.VOID
        if n == 'char':
            return ir.Type('u16', enc='utf16')
        if n in _PRIM:
            return ir.Type(_PRIM[n])
        return ir.Type('class', 'System.IntPtr' if n == 'nint' else 'System.UIntPtr')
    if k == 'array':
        t = ir.Type('array', elem=ty(d['elem']), **({'rank': d['rank']} if d.get('rank', 1) != 1 else {}))
        return t
    if k == 'tv':
        return ir.Type('typevar', d['n'])
    if k == 'named':
        info = {'nullable': True} if d.get('nullable') else {}
        return ir.Type('class', d['n'], [ty(a) for a in d.get('args', [])], **info)
    if k == 'ptr':
        return ir.pointer_to(ty(d['elem']))
    if k == 'dynamic':
        return ir.Type('dynamic')
    return ir.ANY


def is_string(t):
    return t is not None and t.kind == 'class' and t.name == 'System.String'


def tkind(d):
    """The TypeKind of a type record (Class, Struct, Enum, Interface, Delegate), or None."""
    return (d or {}).get('tk')


# -------------------------------------------------------------------------------------- symbols
def sym(d):
    """A CsTreeDump symbol record -> ir.Sym."""
    if not d:
        return None
    k = d['kind']
    flags = []
    for key, flag in (('ext', 'extension'), ('indexer', 'indexer'), ('readonly', 'readonly'), ('initonly', 'initonly'),
                      ('virtual', 'virtual'), ('abstract', 'abstract'), ('override', 'override'),
                      ('sealed', 'sealed'), ('implicit', 'implicit'), ('const', 'const')):
        if d.get(key):
            flags.append(flag)
    owner = d.get('owner')
    if k == 'Method':
        mk = d.get('mk')
        kind = 'ctor' if mk == 'Constructor' else 'method'
        if mk in ('UserDefinedOperator', 'Conversion', 'BuiltinOperator'):
            flags.append('operator')
        if mk == 'LocalFunction':
            flags.append('local')
        if mk == 'StaticConstructor':
            flags.append('static_init')
        if d.get('name') == 'Invoke' and tkind(d.get('ownerType')) == 'Delegate':
            flags.append('delegate')
        refs = [_REF.get(r, '') for r in d.get('refs', [])]
        return ir.Sym(kind, d['name'], owner, d.get('static', False), params=[ty(p) for p in d.get('params', [])],
                      ret=ty(d.get('ret')), varargs=d.get('varargs', False), refs=refs if any(refs) else (),
                      flags=flags)
    if k == 'Field':
        kind = 'enumconst' if tkind(d.get('ownerType')) == 'Enum' and d.get('const') else 'field'
        return ir.Sym(kind, d['name'], owner, d.get('static', False) or d.get('const', False), type=ty(d.get('type')),
                      flags=flags)
    if k == 'Property':
        return ir.Sym('property', d['name'], owner, d.get('static', False), type=ty(d.get('type')),
                      params=[ty(p) for p in d.get('params', [])], flags=flags)
    if k == 'Event':
        return ir.Sym('event', d['name'], owner, d.get('static', False), type=ty(d.get('type')), flags=flags)
    if k in ('Local', 'RangeVariable', 'Discard'):
        return ir.Sym('local', d['name'], owner, type=ty(d.get('type')),
                      flags=flags + (['ref'] if d.get('ref') else []))
    if k == 'Parameter':
        r = _REF.get(d.get('ref', 'None'), '')
        return ir.Sym('param', d['name'], owner, type=ty(d.get('type')), refs=(r,) if r else (), flags=flags)
    if k == 'NamedType':
        return ir.Sym('class', d.get('qname') or d['name'], owner, type=ty(d.get('type')), flags=flags)
    if k == 'TypeParameter':
        return ir.Sym('typevar', d['name'], owner, type=ty(d.get('type')))
    if k == 'Namespace':
        return ir.Sym('package', d['name'], owner)
    if k == 'Label':
        return ir.Sym('label', d['name'])
    return ir.Sym(k.lower(), d['name'], owner, flags=flags)


def _value(c, t=None):
    """A CsTreeDump constant record -> a Python value (as ir.Lit keeps it)."""
    if c is None:
        return None
    tt, v = c['t'], c.get('v')
    if tt == 'null':
        return None
    if tt == 'bool':
        return bool(v)
    if tt == 'string':
        return v
    if tt == 'char':
        return chr(v)
    if tt in ('float', 'double'):
        return float({'Infinity': 'inf', '-Infinity': '-inf', '∞': 'inf', '-∞': '-inf'}.get(v, v))
    if tt == 'decimal':
        raise _Refuse('decimal', 'a decimal literal: no IR number carries base-10 decimals')
    return int(v)


class _Refuse(Exception):
    def __init__(self, kind, why):
        Exception.__init__(self, why)
        self.kind = kind
        self.why = why


# -------------------------------------------------------------------------------------- the lowering
class Lowering(object):
    def __init__(self, path=None):
        self.path = path
        self.cls = None                    # the ClassDecl being lowered
        self.pending = None                # statements hoisted before the statement being lowered
        self.guard = 0                     # > 0: inside a branch that might not run, a loop condition
        self.effects = 0                   # effects lowered so far in this statement
        self.temps = 0
        self.refused = []                  # (line, kind, why): what became Unsupported
        self.rename = {}                   # a local's name -> the name it is given (bindings joined in one expression)

    def pos(self, n):
        return dict(line=n.get('l', 0), col=n.get('c', 0))

    def renamed(self, s):
        if s is not None and s.kind == 'local' and s.name in self.rename:
            s = ir.Sym(s.kind, self.rename[s.name], s.owner, s.static, s.type, s.params, s.ret, s.varargs, s.refs, s.flags)
        return s

    def unsupported(self, n, kind, why, t=None):
        self.refused.append((n.get('l', 0) if n else 0, kind, why))
        p = self.pos(n) if n else dict(line=0, col=0)
        return ir.Unsupported(kind=kind, raw=why, type=t or (ty(n.get('$type')) if n else ir.ANY), **p)

    # ---- declarations
    def unit(self, n):
        types = []
        for t in n.get('types', []):
            types.append(self.decl(t, None))
        package = ''
        if types and types[0].qname and '.' in types[0].qname:
            package = types[0].qname.rsplit('.', 1)[0]
        if n.get('main') is not None:
            types.append(self.top_level(n))
        imports = [u['name'] for u in n.get('usings', []) if u['name'] and not u.get('alias')]
        return ir.Unit(path=self.path, package=package, imports=imports, types=types, lang=LANG, **self.pos(n))

    def top_level(self, n):
        """Top-level statements: the compiler's Program.Main, as a class."""
        ms = n.get('mainSym') or {}
        owner = ms.get('owner') or 'Program'
        c = ir.ClassDecl(name=owner.rsplit('.', 1)[-1], qname=owner, kind='class', mods=['static', 'partial'],
                         typeparams=[], extends=None, implements=[], members=[], outer=None, tparams=[])
        self.cls = c
        body = self.method_body(n['main'])
        s = ir.Sym('method', 'Main', owner, True, params=[], ret=ty(ms.get('ret')) if ms else ir.VOID)
        params = []
        for pn, pt in zip(ms.get('pnames', []), ms.get('params', [])):
            params.append(ir.Param(sym=ir.Sym('param', pn, owner + '.Main', type=ty(pt)), type=ty(pt), mods=[]))
        c.members.append(ir.MethodDecl(sym=s, name='Main', kind='method', params=params, ret=s.ret, body=body,
                                       mods=['static'], throws=[], typeparams=[], tparams=[]))
        return c

    def tparams(self, tps):
        out = []
        for tp in tps or []:
            flags = [f for f in ('class', 'struct', 'new') if tp.get(f)]
            out.append(ir.TypeParam(name=tp['name'], bounds=[ty(b) for b in tp.get('bounds', [])], flags=flags,
                                    variance=None if tp.get('variance') in (None, 'None') else tp['variance']))
        return out

    def decl(self, n, outer):
        s = n.get('sym') or {}
        kind = n['kind']
        qname = s.get('qname') or n['name']
        base = n.get('base')
        extends = ty(base) if base and base.get('n') not in ('System.Object', 'System.ValueType', 'System.Enum') \
            else None
        if kind == 'enum' and n.get('underlying') and n['underlying'].get('n') != 'int':
            extends = ty(n['underlying'])                     # an enum's integer type, where not int
        c = ir.ClassDecl(name=n['name'], qname=qname, kind=kind, mods=list(n.get('mods', [])),
                         typeparams=[tp['name'] for tp in n.get('typeparams', [])],
                         extends=extends, implements=[ty(i) for i in n.get('interfaces', [])
                                                      if i.get('n') != 'System.IEquatable' or kind not in ('record', 'record struct')],
                         members=[], outer=outer, tparams=self.tparams(n.get('typeparams')), **self.pos(n))
        prev, self.cls = self.cls, c
        try:
            if kind == 'delegate':
                inv = sym(n.get('invoke'))
                params = [ir.Param(sym=ir.Sym('param', pn, qname + '.Invoke', type=ty(pt)), type=ty(pt),
                                   mods=[r for r in [_REF.get(rk, '')] if r])
                          for pn, pt, rk in zip(n['invoke'].get('pnames', []), n['invoke'].get('params', []),
                                                n['invoke'].get('refs', []))]
                c.members.append(ir.MethodDecl(sym=inv, name='Invoke', kind='method', params=params, ret=inv.ret,
                                               body=None, mods=['public', 'abstract'], throws=[], typeparams=[],
                                               tparams=[], **self.pos(n)))
                return c
            if n.get('params') is not None and kind in ('record', 'record struct', 'class', 'struct'):
                self.primary(c, n)
            for m in n.get('members', []):
                for x in self.member(m, c):
                    c.members.append(x)
            if kind == 'class' and 'static' not in c.mods and not any(
                    isinstance(x, ir.MethodDecl) and x.kind == 'ctor' for x in c.members):
                # the compiler's default constructor (as javac's, marked IMPLICIT)
                csym = ir.Sym('ctor', '.ctor', qname, params=[], ret=ir.VOID)
                body = ir.Block(stmts=[ir.CtorCall(kind='super:implicit', sym=None, args=[], target=None, **self.pos(n))],
                                **self.pos(n))
                c.members.append(ir.MethodDecl(sym=csym, name='.ctor', kind='ctor', params=[], ret=ir.VOID, body=body,
                                               mods=['public' if 'abstract' not in c.mods else 'protected', 'IMPLICIT'],
                                               throws=[], typeparams=[], tparams=[], **self.pos(n)))
        finally:
            self.cls = prev
        return c

    def primary(self, c, n):
        """A record's (or class's) primary constructor: its parameters, and for a record the public
        properties the compiler makes of them (mods 'positional')."""
        implicit = dict((x['name'], x) for x in (n.get('sym') or {}).get('implicit', []) if x['kind'] == 'Property') \
            if c.kind in ('record', 'record struct') else {}
        params = []
        for p in n['params']:
            ps = sym(p['sym'])
            params.append(ir.Param(sym=ps, type=ps.type, mods=list(p.get('mods', [])), **self.pos(p)))
            if ps.name in implicit:
                psym = sym(implicit[ps.name])
                acc = [ir.MethodDecl(sym=None, name='get', kind='get', params=[], ret=ps.type, body=None, mods=[],
                                     throws=[], typeparams=[], tparams=[]),
                       ir.MethodDecl(sym=None, name='init' if c.kind == 'record' else 'set',
                                     kind='init' if c.kind == 'record' else 'set', params=[], ret=ir.VOID, body=None,
                                     mods=[], throws=[], typeparams=[], tparams=[])]
                c.members.append(ir.Property(sym=psym, type=psym.type, mods=['public', 'positional'], params=[],
                                             accessors=acc, init=None, **self.pos(p)))
        stmts = []
        if n.get('baseCall') is not None:
            stmts.append(self.ctor_init(n['baseCall']))
        csym = ir.Sym('ctor', '.ctor', c.qname, params=[p.type for p in params], ret=ir.VOID)
        c.members.append(ir.MethodDecl(sym=csym, name='.ctor', kind='ctor', params=params, ret=ir.VOID,
                                       body=ir.Block(stmts=stmts, **self.pos(n)), mods=['public', 'primary', 'IMPLICIT'],
                                       throws=[], typeparams=[], tparams=[], **self.pos(n)))

    def params(self, ps):
        out = []
        for p in ps or []:
            s = sym(p['sym'])
            out.append(ir.Param(sym=s, type=s.type if s else ir.ANY, mods=list(p.get('mods', [])), **self.pos(p)))
        return out

    def member(self, m, c):
        k = m['k']
        if k == 'type':
            return [self.decl(m, c.qname)]
        if k == 'field':
            out = []
            for v in m.get('vars', []):
                s = sym(v['sym'])
                init = self.expr_in_decl(v['init']) if v.get('init') is not None else None
                out.append(ir.FieldDecl(sym=s, type=s.type, init=init, mods=list(m.get('mods', [])), **self.pos(v)))
            return out
        if k in ('property', 'indexer'):
            s = sym(m['sym'])
            accessors = []
            if m.get('expr') is not None:                       # int X => expr;
                body = self.method_body(m['expr'])
                accessors.append(ir.MethodDecl(sym=None, name='get', kind='get', params=[], ret=s.type, body=body,
                                               mods=[], throws=[], typeparams=[], tparams=[], **self.pos(m)))
            for a in m.get('accessors', []):
                body = self.method_body(a['body']) if a.get('body') is not None else None
                accessors.append(ir.MethodDecl(sym=None, name=a['kind'], kind=a['kind'], params=[],
                                               ret=s.type if a['kind'] == 'get' else ir.VOID, body=body,
                                               mods=list(a.get('mods', [])), throws=[], typeparams=[], tparams=[],
                                               **self.pos(a)))
            init = self.expr_in_decl(m['init']) if m.get('init') is not None else None
            return [ir.Property(sym=s, type=s.type, mods=list(m.get('mods', [])), params=self.params(m.get('params')),
                                accessors=accessors, init=init, **self.pos(m))]
        if k in ('method', 'operator', 'conversion'):
            s = sym(m['sym'])
            body = self.method_body(m['body']) if m.get('body') is not None else None
            mods = list(m.get('mods', []))
            if k != 'method':
                mods.append('operator')
            name = s.name if s else '?'
            return [ir.MethodDecl(sym=s, name=name, kind='method', params=self.params(m.get('params')),
                                  ret=s.ret if s else ir.VOID, body=body, mods=mods, throws=[],
                                  typeparams=[tp['name'] for tp in m.get('typeparams', [])],
                                  tparams=self.tparams(m.get('typeparams')), **self.pos(m))]
        if k == 'ctor':
            s = sym(m['sym'])
            static = s is not None and 'static_init' in s.flags
            body = self.ctor_body(m.get('body'), m)
            return [ir.MethodDecl(sym=s, name='<clinit>' if static else '.ctor', kind='static_init' if static else 'ctor',
                                  params=self.params(m.get('params')), ret=ir.VOID, body=body,
                                  mods=list(m.get('mods', [])), throws=[], typeparams=[], tparams=[], **self.pos(m))]
        if k == 'enummember':
            s = sym(m['sym'])
            v = m.get('value')
            return [ir.EnumConst(sym=s, args=[], body=None, ctor=None, value=int(v['v']) if v else None, **self.pos(m))]
        why = {'dtor': 'a finalizer: no target runs code when the collector frees an object',
               'eventfield': 'an event', 'event': 'an event'}.get(k, 'not lowered yet')
        return [self.unsupported(m, k, why, ir.ANY)]

    # ---- bodies
    def method_body(self, op):
        """An IMethodBodyOperation / IBlockOperation -> Block."""
        if op is None:
            return None
        k = op['k']
        if k in ('MethodBody', 'MethodBodyOperation', 'ConstructorBody', 'ConstructorBodyOperation'):
            b = op.get('blockBody') or op.get('expressionBody')
            return self.method_body(b) if b else ir.Block(stmts=[], **self.pos(op))
        if k == 'Block':
            return self.block(op)
        return ir.Block(stmts=self.stmts([op]), **self.pos(op))

    def ctor_body(self, op, m):
        stmts = []
        if op is not None and op.get('initializer') is not None:
            stmts.append(self.ctor_init(op['initializer']))
        b = None
        if op is not None:
            b = op.get('blockBody') or op.get('expressionBody')
        if b is not None:
            inner = self.method_body(b)
            stmts += inner.stmts
        return ir.Block(stmts=stmts, **self.pos(op or m))

    def ctor_init(self, st):
        """`: base(...)` / `: this(...)` (or the compiler's own base()) -> CtorCall."""
        inv = st.get('operation') if st['k'] == 'ExpressionStatement' else st
        if inv is None or inv['k'] != 'Invocation':
            return ir.ExprStmt(expr=self.unsupported(st, 'ctor-init', 'a constructor initialiser of another shape'),
                               **self.pos(st))
        s = sym(inv['targetMethod'])
        kind = 'this' if self.cls is not None and s.owner == self.cls.qname else 'super'
        if inv.get('implicit'):
            kind += ':implicit'
        self.pending, self.effects = [], 0
        try:
            args = self.args(inv, s)
        except _Refuse as e:
            self.pending = None
            return ir.ExprStmt(expr=self.unsupported(st, e.kind, e.why, ir.ANY), **self.pos(st))
        pre, self.pending = self.pending, None
        cc = ir.CtorCall(kind=kind, sym=s, args=args, target=None, **self.pos(st))
        if pre:
            return ir.ExprStmt(expr=self.unsupported(st, 'ctor-init', 'a constructor initialiser needing a statement'),
                               **self.pos(st))
        return cc

    def block(self, op):
        return ir.Block(stmts=self.stmts(op.get('operations', [])), **self.pos(op))

    def stmts(self, ops):
        out = []
        ops = list(ops)
        for i, op in enumerate(ops):
            if op['k'] == 'UsingDeclaration':
                # `using var x = ...;` holds to the end of the block: a scoped resource around the rest
                res = self.stmt(op['declarationGroup'])
                body = ir.Block(stmts=self.stmts(ops[i + 1:]), **self.pos(op))
                out.append(ir.Try(resources=[r for r in res if isinstance(r, ir.Local)], body=body, catches=[],
                                  final=None, **self.pos(op)))
                if any(not isinstance(r, ir.Local) for r in res):
                    out.append(ir.ExprStmt(expr=self.unsupported(op, 'using', 'a using declaration with a hoisted part'),
                                           **self.pos(op)))
                return out
            out.extend(self.stmt(op))
        return out

    def one(self, op):
        """A statement in a place that holds one (a loop's or an if's body)."""
        ss = self.stmt(op)
        if len(ss) == 1:
            return ss[0]
        return ir.Block(stmts=ss, **self.pos(op))

    def stmt(self, op):
        """One operation in statement position -> [Stmt] (hoisted statements first)."""
        saved = (self.pending, self.effects, self.guard)
        self.pending, self.effects, self.guard = [], 0, 0
        try:
            s = self._stmt(op)
        except _Refuse as e:
            s = [ir.ExprStmt(expr=self.unsupported(op, e.kind, e.why, ir.ANY), **self.pos(op))]
        pre = self.pending
        self.pending, self.effects, self.guard = saved
        if not isinstance(s, list):
            s = [s]
        return pre + s

    def _stmt(self, op):
        k = op['k']
        p = self.pos(op)
        if k == 'Block':
            return self.block(op)
        if k == 'VariableDeclarationGroup':
            out = []
            for decl in op.get('declarations', []):
                for d in decl.get('declarators', []):
                    s = sym(d['symbol'])
                    init = d.get('initializer') or decl.get('initializer')
                    v = self.expr(init['value']) if init is not None else None
                    out.append(ir.Local(sym=s, type=s.type, init=v, mods=['const'] if 'const' in s.flags else [],
                                        **self.pos(d)))
            return out
        if k == 'ExpressionStatement':
            e = op['operation']
            if e['k'] == 'Throw':
                return ir.Throw(expr=self.expr(e['exception']) if e.get('exception') else None, **p)
            if e['k'] == 'SimpleAssignment' and e['target']['k'] == 'Discard':
                return ir.ExprStmt(expr=self.expr(e['value']), **p)
            return ir.ExprStmt(expr=self.expr(e), **p)
        if k == 'Throw':
            return ir.Throw(expr=self.expr(op['exception']) if op.get('exception') else None, **p)
        if k == 'Rethrow':
            return ir.Throw(expr=None, **p)
        if k == 'Conditional':
            return ir.If(cond=self.expr(op['condition']), then=self.one(op['whenTrue']),
                         orelse=self.one(op['whenFalse']) if op.get('whenFalse') else None, **p)
        if k == 'Loop':
            return self.loop(op, p)
        if k == 'Branch':
            bk = op.get('branchKind')
            if bk == 'Break':
                return ir.Break(label=None, **p)
            if bk == 'Continue':
                return ir.Continue(label=None, **p)
            if bk == 'GoTo':
                tgt = (op.get('target') or {}).get('name', '')
                if tgt.startswith('case ') or tgt == 'default:' or tgt.startswith('default'):
                    raise _Refuse('goto-case', '`goto case`: a jump between switch sections')
                return ir.Goto(label=tgt, **p)
            raise _Refuse('branch', 'a %s branch' % bk)
        if k == 'Labeled':
            inner = self.one(op['operation']) if op.get('operation') else ir.Empty(**p)
            return ir.Labeled(label=(op.get('label') or {}).get('name'), body=inner, **p)
        if k == 'Return':
            rk = op.get('returnKind', 'Return')
            if rk != 'Return':
                raise _Refuse('iterator', '`yield return` / `yield break`: an iterator (a state machine)')
            return ir.Return(expr=self.expr(op['returnedValue']) if op.get('returnedValue') else None, **p)
        if k == 'YieldReturn' or k == 'YieldBreak':
            raise _Refuse('iterator', '`yield return` / `yield break`: an iterator (a state machine)')
        if k == 'Switch':
            return self.switch(op, p)
        if k == 'Try':
            return self.try_(op, p)
        if k == 'Using':
            res = op['resources']
            resources = self.stmt(res) if res['k'] == 'VariableDeclarationGroup' else \
                [ir.ExprStmt(expr=self.expr(res), **self.pos(res))]
            return ir.Try(resources=resources, body=self.as_block(op['body']), catches=[], final=None, **p)
        if k == 'Lock':
            return ir.Sync(lock=self.expr(op['lockedValue']), body=self.as_block(op['body']), **p)
        if k == 'Empty':
            return ir.Empty(**p)
        if k == 'LocalFunction':
            raise _Refuse('local-function', 'a local function')
        if k == 'UsingDeclaration':
            raise _Refuse('using', 'a using declaration out of a block')
        raise _Refuse(k, 'the statement %s' % k)

    def as_block(self, op):
        s = self.one(op)
        return s if isinstance(s, ir.Block) else ir.Block(stmts=[s], **self.pos(op))

    def loop(self, op, p):
        lk = op.get('loopKind')
        if lk == 'While':
            self.guard += 1
            cond = self.expr(op['condition']) if op.get('condition') else ir.Lit(value=True, type=ir.BOOL, **p)
            self.guard -= 1
            if op.get('conditionIsUntil'):
                cond = ir.Unary(op='!', x=cond, postfix=False, type=ir.BOOL, **p)
            body = self.one(op['body'])
            if op.get('conditionIsTop', True):
                return ir.While(cond=cond, body=body, **p)
            return ir.DoWhile(body=body, cond=cond, **p)
        if lk == 'For':
            init = []
            for b in op.get('before', []):
                init.extend(self.stmt(b))
            self.guard += 1
            cond = self.expr(op['condition']) if op.get('condition') else None
            update = []
            for u in op.get('atLoopBottom', []):
                e = u['operation'] if u['k'] == 'ExpressionStatement' else u
                update.append(self.expr(e))
            self.guard -= 1
            return ir.For(init=init, cond=cond, update=update, body=self.one(op['body']), **p)
        if lk == 'ForEach':
            v = op.get('loopControlVariable')
            if v is None or v['k'] not in ('VariableDeclarator', 'LocalReference'):
                raise _Refuse('foreach', 'a foreach that deconstructs its element')
            s = sym(v.get('symbol') or v.get('local'))
            coll = op['collection']
            while coll['k'] == 'Conversion' and coll.get('implicit'):     # to IEnumerable: the compiler's own
                coll = coll['operand']
            var = ir.Local(sym=s, type=s.type, init=None, mods=[], **self.pos(v))
            return ir.ForEach(var=var, iterable=self.expr(coll), body=self.one(op['body']), **p)
        raise _Refuse('loop', 'a %s loop' % lk)

    def switch(self, op, p):
        cases = []
        for c in op.get('cases', []):
            labels, default = [], False
            for cl in c.get('clauses', []):
                ck = cl.get('caseKind') or cl['k']
                if ck in ('Default', 'DefaultCaseClause'):
                    default = True
                elif ck in ('SingleValue', 'SingleValueCaseClause'):
                    labels.append(self.expr(cl['value']))
                elif ck in ('Pattern', 'PatternCaseClause') and (cl.get('pattern') or {}).get('k') == 'ConstantPattern' \
                        and cl.get('guard') is None:
                    labels.append(self.expr(cl['pattern']['value']))
                else:
                    raise _Refuse('switch-pattern', 'a switch section matched by a pattern or a `when` guard')
            body = []
            for b in c.get('body', []):
                body.extend(self.stmt(b))
            cases.append(ir.Case(labels=labels, default=default, body=body, **self.pos(c)))
        return ir.Switch(expr=self.expr(op['value']), cases=cases, **p)

    def try_(self, op, p):
        catches = []
        for c in op.get('catches', []):
            if c.get('filter') is not None:
                raise _Refuse('catch-filter', 'an exception filter (`catch ... when`)')
            d = c.get('exceptionDeclarationOrExpression')
            s = sym(d.get('symbol')) if d is not None and d['k'] == 'VariableDeclarator' else None
            catches.append(ir.Catch(sym=s, types=[ty(c.get('exceptionType'))], body=self.block(c['handler']),
                                    **self.pos(c)))
        return ir.Try(resources=[], body=self.block(op['body']), catches=catches,
                      final=self.block(op['finally']) if op.get('finally') else None, **p)

    # ---- expressions
    def expr_in_decl(self, op):
        """A field's or property's initialiser: an expression with no statement to hoist into."""
        saved = (self.pending, self.effects, self.guard)
        self.pending, self.effects, self.guard = None, 0, 0
        try:
            return self.expr(op)
        except _Refuse as e:
            return self.unsupported(op, e.kind, e.why)
        finally:
            self.pending, self.effects, self.guard = saved

    def hoist(self, n, stmts, needs_order=True):
        """Put statements before the statement being lowered, where that cannot change what runs first."""
        if self.pending is None:
            raise _Refuse('hoist', 'an initialiser needing a statement, in a declaration')
        if self.guard:
            raise _Refuse('hoist', 'an initialiser needing a statement, in a branch that might not run')
        if needs_order and self.effects:
            raise _Refuse('hoist', 'an initialiser needing a statement, after an effect in the same statement')
        self.pending.extend(stmts)

    def temp(self, t, init, n):
        self.temps += 1
        s = ir.Sym('local', '__t%d' % self.temps, None, type=t)
        self.hoist(n, [ir.Local(sym=s, type=t, init=init, mods=[], **self.pos(n))])
        return ir.Name(sym=s, type=t, **self.pos(n))

    def expr(self, n):
        try:
            return self._expr(n)
        except _Refuse as e:
            if self.pending is None:
                return self.unsupported(n, e.kind, e.why)
            raise

    def _expr(self, n):
        k = n['k']
        p = self.pos(n)
        t = ty(n.get('$type'))
        if k == 'Literal':
            return ir.Lit(value=_value(n.get('$const'), t), type=t if n.get('$type') else ir.NULL, **p)
        if k == 'NameOf':
            return ir.Lit(value=_value(n.get('$const')), type=STRING, **p)
        if k == 'LocalReference':
            s = self.renamed(sym(n['local']))
            if n.get('isDeclaration'):                            # `out var x`
                self.hoist(n, [ir.Local(sym=s, type=s.type, init=None, mods=[], **p)], needs_order=False)
            return ir.Name(sym=s, type=t, **p)
        if k == 'ParameterReference':
            return ir.Name(sym=sym(n['parameter']), type=t, **p)
        if k == 'FieldReference':
            if n['field'].get('const') and not n['field'].get('source') and n.get('$const') is not None:
                return ir.Lit(value=_value(n['$const']), type=t, **p)      # a library constant (int.MaxValue): its value
            s = sym(n['field'])
            return ir.Select(obj=self.receiver(n, s), sym=s, type=t, **p)
        if k == 'PropertyReference':
            s = sym(n['property'])
            obj = self.receiver(n, s)
            if s.owner == 'System.Array' and s.name == 'Length':
                return ir.Length(arr=obj, type=t, **p)
            if 'indexer' in s.flags:
                args = self.args(n, s)
                if len(args) != 1:
                    raise _Refuse('indexer', 'an indexer of %d arguments' % len(args))
                return ir.Index(arr=obj, index=args[0], sym=s, type=t, **p)
            return ir.Select(obj=obj, sym=s, type=t, **p)
        if k == 'EventReference':
            raise _Refuse('event', 'an event')
        if k == 'Invocation':
            return self.call(n, t, p)
        if k == 'ObjectCreation':
            return self.new(n, t, p)
        if k == 'TypeParameterObjectCreation':
            raise _Refuse('new-T', '`new T()` of a type parameter')
        if k == 'AnonymousObjectCreation':
            raise _Refuse('anonymous-type', 'an anonymous type')
        if k == 'ArrayCreation':
            return self.new_array(n, t, p)
        if k == 'ArrayElementReference':
            arr = self.expr(n['arrayReference'])
            idx = [self.expr(i) for i in n.get('indices', [])]
            return ir.Index(arr=arr, index=idx[0] if len(idx) == 1 else idx, type=t, **p)
        if k == 'Binary':
            return self.binary(n, t, p)
        if k == 'Unary':
            if n.get('operatorMethod') is not None:
                s = sym(n['operatorMethod'])
                return self.op_call(n, s, [self.expr(n['operand'])], t, p)
            op = _UNARY.get(n.get('operatorKind'))
            if op is None or n.get('isLifted'):
                raise _Refuse('unary', 'the unary operator %s%s' % (n.get('operatorKind'),
                                                                    ' (lifted)' if n.get('isLifted') else ''))
            return ir.Unary(op=op, x=self.expr(n['operand']), postfix=False, type=t,
                            overflow='trap' if n.get('isChecked') else None, **p)
        if k in ('Increment', 'Decrement'):
            if n.get('operatorMethod') is not None or n.get('isLifted'):
                raise _Refuse('increment', 'a user-defined or lifted ++/--')
            self.effects += 1
            return ir.Unary(op='++' if k == 'Increment' else '--', x=self.expr(n['target']),
                            postfix=bool(n.get('isPostfix')), type=t,
                            overflow='trap' if n.get('isChecked') else None, **p)
        if k == 'SimpleAssignment':
            if n['target']['k'] in ('Tuple', 'Discard') or n.get('isRef'):
                raise _Refuse('deconstruction', 'a deconstructing, discarding or ref assignment')
            target = self.expr(n['target'])
            value = self.expr(n['value'])
            self.effects += 1
            return ir.Assign(op=None, target=target, value=value, type=t, **p)
        if k == 'DeconstructionAssignment':
            raise _Refuse('deconstruction', 'a deconstruction')
        if k == 'CompoundAssignment':
            if n.get('operatorMethod') is not None and not is_string(t):
                raise _Refuse('compound', 'a compound assignment through a user-defined operator')
            if n.get('isLifted'):
                raise _Refuse('lifted', 'lifted nullable arithmetic')
            op = _BINARY.get(n.get('operatorKind'))
            if op is None:
                raise _Refuse('compound', 'the compound operator %s' % n.get('operatorKind'))
            target = self.expr(n['target'])
            value = self.expr(n['value'])
            self.effects += 1
            return ir.Assign(op=op, target=target, value=value, type=t,
                             overflow='trap' if n.get('isChecked') else None, **p)
        if k == 'CoalesceAssignment':
            target = self.expr(n['target'])
            self.guard += 1
            value = self.expr(n['value'])
            self.guard -= 1
            self.effects += 1
            return ir.Assign(op='??', target=target, value=value, type=t, **p)
        if k == 'Conditional':
            cond = self.expr(n['condition'])
            self.guard += 1
            a, b = self.expr(n['whenTrue']), self.expr(n['whenFalse'])
            self.guard -= 1
            return ir.Cond(cond=cond, then=a, orelse=b, type=t, **p)
        if k == 'Coalesce':
            left = self.expr(n['value'])
            self.guard += 1
            right = self.expr(n['whenNull'])
            self.guard -= 1
            return ir.Binary(op='??', left=left, right=right, type=t, **p)
        if k == 'ConditionalAccess':
            return self.cond_access(n, t, p)
        if k == 'Conversion':
            return self.conversion(n, t, p)
        if k == 'IsType':
            x = ir.InstanceOf(x=self.expr(n['valueOperand']), test=ty(n.get('typeOperand')), binding=None,
                              type=ir.BOOL, **p)
            return ir.Unary(op='!', x=x, postfix=False, type=ir.BOOL, **p) if n.get('isNegated') else x
        if k == 'IsPattern':
            v = self.expr(n['value'])
            return self.pattern(n['pattern'], v, p)
        if k == 'SwitchExpression':
            return self.switch_expr(n, t, p)
        if k == 'DelegateCreation':
            target = n['target']
            if target['k'] == 'AnonymousFunction':
                return self.lam(target, t, p)
            if target['k'] == 'MethodReference':
                s = sym(target['method'])
                inst = target.get('instance')
                obj = self.expr(inst) if inst is not None and not inst.get('implicit') else \
                    (ir.TypeName(type=ir.Type('class', s.owner), **p) if s.static else ir.This(qualifier=None, type=ir.Type('class', s.owner), **p))
                return ir.MethodRef(target=obj, sym=s, mode='invoke', type=t, **p)
            raise _Refuse('delegate', 'a delegate made from %s' % target['k'])
        if k == 'AnonymousFunction':
            return self.lam(n, t, p)
        if k == 'InstanceReference':
            if n.get('referenceKind') == 'ImplicitReceiver':
                if getattr(self, 'receiver_now', None) is None:
                    raise _Refuse('initializer', 'an initialiser receiver outside its creation')
                return self.receiver_now
            return ir.This(qualifier=None, type=t, **p)
        if k == 'ConditionalAccessInstance':
            if getattr(self, 'cond_recv', None) is None:
                raise _Refuse('null-conditional', 'a `?.` receiver outside its access')
            return self.cond_recv
        if k == 'TypeOf':
            return ir.ClassLit(of=ty(n.get('typeOperand')), type=t, **p)
        if k == 'DefaultValue':
            return self.default(n, t, p)
        if k == 'InterpolatedString':
            return self.interpolated(n, t, p)
        if k == 'Throw':
            raise _Refuse('throw-expression', 'a throw expression')
        if k == 'Await':
            raise _Refuse('await', '`await`: an asynchronous method (a state machine)')
        if k == 'DeclarationExpression' and n.get('expression', {}).get('k') == 'LocalReference':
            return self.expr(n['expression'])                     # out T x
        if k in ('Tuple', 'DeclarationExpression'):
            raise _Refuse('tuple', 'a tuple')
        if k == 'Discard':
            raise _Refuse('discard', 'a discard')
        if k in ('SizeOf', 'AddressOf', 'PointerIndirectionReference'):
            raise _Refuse('unsafe', 'unsafe code')
        if k.startswith('Dynamic'):
            raise _Refuse('dynamic', '`dynamic`')
        if k == 'Invalid':
            raise _Refuse('invalid', 'code the compiler could not bind')
        if k == 'TranslatedQuery':                                # query syntax: the calls it means
            return self.expr(n['operation'])
        if k == 'Parenthesized':
            return ir.Paren(x=self.expr(n['operand']), type=t, **p)
        raise _Refuse(k, 'the operation %s' % k)

    def receiver(self, n, s):
        """The object a member is read on: This (implicit or written), its class for a static, the
        receiver of an object initialiser, or an expression."""
        inst = n.get('instance')
        p = self.pos(n)
        if inst is None or s.static:
            return ir.TypeName(type=ir.Type('class', s.owner), **p)
        if inst['k'] == 'InstanceReference':
            if inst.get('referenceKind') == 'ImplicitReceiver':
                return self._expr(inst)
            it = ty(inst.get('$type'))
            if self.cls is not None and it.kind == 'class' and it.name != self.cls.qname and not inst.get('implicit'):
                return ir.Super(qualifier=None, type=it, **p)        # base.X
            return ir.This(qualifier=None, type=it, **p)
        return self.expr(inst)

    def args(self, n, s):
        """A call's arguments in parameter order, as written: a `params` array spread back out, defaults the
        compiler filled in kept (every target then sees the value)."""
        items = list(n.get('arguments', []))
        order = [a.get('parameter', {}).get('ordinal', i) if a.get('parameter') else i for i, a in enumerate(items)]
        lowered = []                                              # each argument lowered once, as written
        for a in items:
            v = a['value']
            if a.get('argumentKind') == 'ParamArray' and v['k'] == 'ArrayCreation' and v.get('implicit'):
                init = v.get('initializer')
                lowered.append([self.expr(e) for e in (init or {}).get('elementValues', [])])
                continue
            x = self.expr(v)
            if a.get('argumentKind') == 'DefaultValue' and a.get('parameter'):
                pt = ty(a['parameter'].get('type'))
                if x.type != pt and pt.kind != 'any':             # the default as the parameter's type, so an
                    x = ir.Cast(x=x, type=pt, **self.pos(n))      # overload is chosen as the compiler chose it
            lowered.append([x])
        if order != sorted(order):                                # named arguments out of order: only where no effect moves
            if any(_effects(v) for vs in lowered for v in vs):
                raise _Refuse('named-args', 'named arguments out of parameter order, with effects')
            lowered = [vs for _o, vs in sorted(zip(order, lowered), key=lambda z: z[0])]
        return [v for vs in lowered for v in vs]

    def call(self, n, t, p):
        s = sym(n['targetMethod'])
        inst = n.get('instance')
        if inst is None:
            target = ir.TypeName(type=ir.Type('class', s.owner), **p) if s.static else None
        elif inst['k'] == 'InstanceReference' and inst.get('implicit') and inst.get('referenceKind') != 'ImplicitReceiver':
            target = None                                           # own method, implicit this
        else:
            target = self.receiver(n, s)
        args = self.args(n, s)
        self.effects += 1
        typeargs = [ty(a) for a in n['targetMethod'].get('typeargs', [])]
        if typeargs and not n.get('typeArgsWritten'):          # the compiler inferred them: a back end may too
            s = ir.Sym(s.kind, s.name, s.owner, s.static, s.type, s.params, s.ret, s.varargs, s.refs,
                       tuple(s.flags) + ('inferred',))
        return ir.Call(target=target, sym=s, args=args, typeargs=typeargs, type=t, **p)

    def op_call(self, n, s, args, t, p):
        """A user-defined operator or conversion, or a BCL operator that is a method: a static call."""
        flags = tuple(f for f in s.flags if f != 'operator') + ('operator',)
        s = ir.Sym(s.kind, s.name, s.owner, True, s.type, s.params, s.ret, s.varargs, s.refs, flags)
        return ir.Call(target=ir.TypeName(type=ir.Type('class', s.owner), **p), sym=s, args=args, typeargs=[], type=t, **p)

    def new(self, n, t, p):
        s = sym(n.get('constructor')) if n.get('constructor') else ir.Sym('ctor', '.ctor', t.name if t.kind == 'class' else None,
                                                                          params=[], ret=ir.VOID)
        args = self.args(n, s) if n.get('constructor') else []
        self.effects += 1
        e = ir.New(sym=s, args=args, body=None, outer=None, memory='gc', type=t, **p)
        init = n.get('initializer')
        if init is None or not init.get('initializers'):
            return e
        # an object or collection initialiser: a temporary, then each member set or Add, before the statement
        effects = self.effects
        self.effects -= 1
        tmp = self.temp(t, e, n)
        self.effects = effects
        prev, self.receiver_now = getattr(self, 'receiver_now', None), tmp
        stmts = []
        try:
            for i in init['initializers']:
                saved = (self.pending, self.effects)
                self.pending, self.effects = stmts, 0          # each member set or Add: a statement of its own
                try:
                    stmts.append(ir.ExprStmt(expr=self.expr(i), **self.pos(i)))
                finally:
                    self.pending, self.effects = saved
        finally:
            self.receiver_now = prev
        self.pending.extend(stmts)
        return tmp

    def new_array(self, n, t, p):
        _d, elem = t.dims() if t.kind == 'array' else (0, ir.ANY)
        if t.kind == 'array' and t.info.get('rank', 1) != 1:
            elem = t.elem
        init = n.get('initializer')
        dims = [] if init is not None else [self.expr(d) for d in n.get('dimensionSizes', [])]
        items = None
        if init is not None:
            if t.info.get('rank', 1) != 1:
                raise _Refuse('array', 'a rectangular array initialiser')
            items = [self.expr(e) for e in init.get('elementValues', [])]
            if not items:
                dims = [ir.Lit(value=0, type=ir.I32, **p)]
        return ir.NewArray(elem=elem, dims=dims, init=items, type=t, **p)

    def binary(self, n, t, p):
        if n.get('operatorMethod') is not None:
            s = sym(n['operatorMethod'])
            left = self.expr(n['leftOperand'])
            if n.get('operatorKind') in ('ConditionalAnd', 'ConditionalOr'):
                raise _Refuse('operator', 'a user-defined && or ||')
            right = self.expr(n['rightOperand'])
            if s.owner == 'System.String' and s.name == 'op_Addition' or (is_string(t) and n.get('operatorKind') == 'Add'):
                return _concat(left, right, t, p)
            return self.op_call(n, s, [left, right], t, p)
        op = _BINARY.get(n.get('operatorKind'))
        if op is None:
            raise _Refuse('binary', 'the operator %s' % n.get('operatorKind'))
        left = self.expr(n['leftOperand'])
        if op in ('&&', '||'):
            self.guard += 1
        right = self.expr(n['rightOperand'])
        if op in ('&&', '||'):
            self.guard -= 1
        if n.get('isLifted') and not (op in ('==', '!=') and (_is_null(left) or _is_null(right))):
            raise _Refuse('lifted', 'lifted nullable arithmetic (%s on a nullable value)' % op)
        if op == '+' and is_string(t):
            return _concat(left, right, t, p)
        return ir.Binary(op=op, left=left, right=right, type=t, overflow='trap' if n.get('isChecked') else None, **p)

    def conversion(self, n, t, p):
        c = n.get('conversion') or {}
        operand = n['operand']
        if c.get('userDefined') and c.get('method'):
            s = sym(c['method'])
            return self.op_call(n, s, [self.expr(operand)], t, p)
        if n.get('isTryCast'):                                    # x as T
            x = self.expr(operand)
            if not _simple(x):
                raise _Refuse('as', '`as` on an expression with effects')
            return ir.Cond(cond=ir.InstanceOf(x=x, test=t, binding=None, type=ir.BOOL, **p),
                           then=ir.Cast(x=x, type=t, **p), orelse=ir.Lit(value=None, type=ir.NULL, **p), type=t, **p)
        x = self.expr(operand)
        if n.get('implicit'):                                     # the compiler's, not written: left implicit
            if not c.get('identity') and isinstance(x, ir.Lit) and x.value is None:
                x.type = t if t.kind != 'any' else x.type             # null as a typed null
            return x
        return ir.Cast(x=x, type=t, overflow='trap' if n.get('isChecked') else None, **p)

    def cond_access(self, n, t, p):
        """a?.b: when `a` can be read twice (a name, this, a field of one), a test and the access."""
        recv = self.expr(n['operation'])
        if not _simple(recv):
            raise _Refuse('null-conditional', '`?.` on an expression with effects')
        prev, self.cond_recv = getattr(self, 'cond_recv', None), recv
        self.guard += 1
        try:
            inner = self.expr(n['whenNotNull'])
        finally:
            self.guard -= 1
            self.cond_recv = prev
        test = ir.Binary(op='!=', left=recv, right=ir.Lit(value=None, type=ir.NULL, **p), type=ir.BOOL, **p)
        return ir.Cond(cond=test, then=inner, orelse=ir.Lit(value=None, type=ir.NULL, **p), type=t, **p)

    def default(self, n, t, p):
        if t.kind in ('i8', 'i16', 'i32', 'i64', 'u8', 'u16', 'u32', 'u64'):
            return ir.Lit(value=chr(0) if t.info.get('enc') else 0, type=t, **p)
        if t.kind in ('f32', 'f64'):
            return ir.Lit(value=0.0, type=t, **p)
        if t.kind == 'bool':
            return ir.Lit(value=False, type=t, **p)
        if n.get('$const') is not None and n['$const']['t'] == 'null':
            return ir.Lit(value=None, type=t, **p)
        raise _Refuse('default', '`default` of %r' % (t,))

    def interpolated(self, n, t, p):
        parts = []
        for part in n.get('parts', []):
            if part['k'] == 'InterpolatedStringText':
                parts.append(ir.Lit(value=_value(part['text'].get('$const')), type=STRING, **self.pos(part)))
            elif part['k'] == 'Interpolation':
                if part.get('alignment') is not None or part.get('formatString') is not None:
                    raise _Refuse('interpolation-format', 'an interpolation with an alignment or a format string')
                parts.append(self.expr(part['expression']))
            else:
                raise _Refuse('interpolation', 'an interpolated part %s' % part['k'])
        if not parts:
            return ir.Lit(value='', type=STRING, **p)
        if len(parts) == 1 and is_string(parts[0].type):
            parts.insert(0, ir.Lit(value='', type=STRING, **p))
        return ir.Concat(parts=parts, type=STRING, **p)

    def lam(self, n, t, p):
        ms = n.get('symbol') or {}
        owner = (ms.get('owner') or '') + '.lambda'
        params = [ir.Param(sym=ir.Sym('param', pn, owner, type=ty(pt)), type=ty(pt), mods=[])
                  for pn, pt in zip(ms.get('pnames', []), ms.get('params', []))]
        saved = (self.pending, self.effects, self.guard)
        self.pending, self.effects, self.guard = None, 0, 0
        try:
            body = self.method_body(n['body'])
        finally:
            self.pending, self.effects, self.guard = saved
        if isinstance(body, ir.Block) and len(body.stmts) == 1 and isinstance(body.stmts[0], ir.Return) \
                and body.stmts[0].expr is not None and n['body'].get('implicit', True) and _implicit_body(n['body']):
            body = body.stmts[0].expr                             # x => x + 1
        return ir.Lambda(params=params, body=body, type=t, **p)

    # ---- patterns
    def pattern(self, pat, v, p):
        k = pat['k']
        if k == 'ConstantPattern':
            c = self.expr(pat['value'])
            if _is_null(c):
                return ir.Binary(op='==', left=v, right=c, type=ir.BOOL, **p)
            if is_string(v.type) or is_string(c.type):
                s = ir.Sym('method', 'op_Equality', 'System.String', True, params=[STRING, STRING], ret=ir.BOOL,
                           flags=('operator',))
                return ir.Call(target=ir.TypeName(type=STRING, **p), sym=s, args=[v, c], typeargs=[], type=ir.BOOL, **p)
            return ir.Binary(op='==', left=v, right=c, type=ir.BOOL, **p)
        if k == 'DeclarationPattern':
            mt = ty(pat.get('matchedType') or pat.get('narrowedType'))
            ds = pat.get('declaredSymbol')
            if pat.get('matchesNull'):
                raise _Refuse('var-pattern', 'a `var` pattern')
            binding = ir.Local(sym=self.renamed(sym(ds)), type=mt, init=None, mods=[], **p) if ds else None
            return ir.InstanceOf(x=v, test=mt, binding=binding, type=ir.BOOL, **p)
        if k == 'TypePattern':
            return ir.InstanceOf(x=v, test=ty(pat.get('matchedType')), binding=None, type=ir.BOOL, **p)
        if k == 'RelationalPattern':
            op = _BINARY.get(pat.get('operatorKind'))
            if op is None:
                raise _Refuse('pattern', 'the relational operator %s' % pat.get('operatorKind'))
            return ir.Binary(op=op, left=v, right=self.expr(pat['value']), type=ir.BOOL, **p)
        if k == 'NegatedPattern':
            return ir.Unary(op='!', x=self.pattern(pat['pattern'], v, p), postfix=False, type=ir.BOOL, **p)
        if k == 'BinaryPattern':
            if not _simple(v):
                raise _Refuse('pattern', 'an and/or pattern on an expression with effects')
            op = '&&' if pat.get('operatorKind') == 'And' else '||'
            return ir.Binary(op=op, left=self.pattern(pat['leftPattern'], v, p),
                             right=self.pattern(pat['rightPattern'], v, p), type=ir.BOOL, **p)
        if k == 'DiscardPattern':
            return ir.Lit(value=True, type=ir.BOOL, **p)
        if k == 'RecursivePattern':
            if pat.get('deconstructionSubpatterns'):
                raise _Refuse('pattern', 'a positional (deconstructing) pattern')
            mt = ty(pat.get('matchedType'))
            ds = pat.get('declaredSymbol')
            subs = pat.get('propertySubpatterns') or []
            if subs and not ds and not _simple(v):
                raise _Refuse('pattern', 'a property pattern on an expression with effects')
            test = None
            if mt.kind != 'any' and (mt != v.type):
                test = ir.InstanceOf(x=v, test=mt, binding=ir.Local(sym=self.renamed(sym(ds)), type=mt, init=None, mods=[], **p) if ds else None,
                                     type=ir.BOOL, **p)
                obj = ir.Name(sym=self.renamed(sym(ds)), type=mt, **p) if ds else ir.Cast(x=v, type=mt, **p)
            else:
                test = ir.Binary(op='!=', left=v, right=ir.Lit(value=None, type=ir.NULL, **p), type=ir.BOOL, **p) \
                    if v.type.kind == 'class' else None
                obj = v
            for sp in subs:
                m = sp.get('member') or {}
                if m.get('k') != 'PropertyReference' and m.get('k') != 'FieldReference':
                    raise _Refuse('pattern', 'a nested property pattern')
                ms = sym(m.get('property') or m.get('field'))
                val = ir.Select(obj=obj, sym=ms, type=ms.type, **p)
                one = self.pattern(sp['pattern'], val, p)
                test = one if test is None else ir.Binary(op='&&', left=test, right=one, type=ir.BOOL, **p)
            return test if test is not None else ir.Lit(value=True, type=ir.BOOL, **p)
        raise _Refuse('pattern', 'the pattern %s' % k)

    def switch_expr(self, n, t, p):
        v = self.expr(n['value'])
        arms = n.get('arms', [])
        if all(a['pattern']['k'] in ('ConstantPattern', 'DiscardPattern') and a.get('guard') is None for a in arms) \
                and arms and arms[-1]['pattern']['k'] == 'DiscardPattern' \
                and not any(_is_null(self.expr(a['pattern']['value'])) for a in arms if a['pattern']['k'] == 'ConstantPattern'):
            cases = []
            self.guard += 1
            for a in arms:
                default = a['pattern']['k'] == 'DiscardPattern'
                labels = [] if default else [self.expr(a['pattern']['value'])]
                cases.append(ir.Case(labels=labels, default=default,
                                     body=[ir.Yield(expr=self.expr(a['value']), **self.pos(a))], **self.pos(a)))
            self.guard -= 1
            return ir.SwitchExpr(expr=v, cases=cases, type=t, **p)
        if not _simple(v):
            v = self.temp(v.type, v, n)
        if not arms or arms[-1]['pattern']['k'] != 'DiscardPattern' or arms[-1].get('guard') is not None:
            raise _Refuse('switch-expression', 'a switch expression with no `_` arm (it throws when none matches)')
        # the arms become one expression, so a name two arms both bind is given a number in the later ones
        bound, renames = set(), []
        for i, a in enumerate(arms):
            r = {}
            for x in _bindings(a['pattern']):
                if x in bound:
                    r[x] = '%s%d' % (x, i + 1)
                bound.add(x)
            renames.append(r)
        self.guard += 1
        saved = self.rename
        try:
            self.rename = dict(saved, **renames[-1])
            out = self.expr(arms[-1]['value'])
            for a, r in reversed(list(zip(arms[:-1], renames[:-1]))):
                self.rename = dict(saved, **r)
                test = self.pattern(a['pattern'], v, self.pos(a))
                if a.get('guard') is not None:
                    test = ir.Binary(op='&&', left=test, right=self.expr(a['guard']), type=ir.BOOL, **self.pos(a))
                out = ir.Cond(cond=test, then=self.expr(a['value']), orelse=out, type=t, **self.pos(a))
        finally:
            self.guard -= 1
            self.rename = saved
        return out


# -------------------------------------------------------------------------------------- helpers
def _concat(left, right, t, p):
    parts = []
    for x in (left, right):
        if isinstance(x, ir.Concat):
            parts.extend(x.parts)
        else:
            parts.append(x)
    return ir.Concat(parts=parts, type=t, **p)


def _bindings(pat):
    """The names a pattern declares."""
    out = []

    def walk(n):
        if isinstance(n, dict):
            if n.get('k') in ('DeclarationPattern', 'RecursivePattern') and n.get('declaredSymbol'):
                out.append(n['declaredSymbol']['name'])
            for v in n.values():
                walk(v)
        elif isinstance(n, list):
            for v in n:
                walk(v)
    walk(pat)
    return out


def _is_null(e):
    return isinstance(e, ir.Lit) and e.value is None and (e.type is None or e.type.kind in ('null', 'class', 'any', 'typevar'))


def _simple(e):
    """An expression that can be read twice: a name, this, a literal, a field of one."""
    if isinstance(e, (ir.Name, ir.This, ir.Lit, ir.TypeName)):
        return True
    if isinstance(e, ir.Select) and e.sym is not None and e.sym.kind == 'field':
        return _simple(e.obj)
    return False


def _effects(e):
    return any(isinstance(x, (ir.Call, ir.New, ir.Assign)) or (isinstance(x, ir.Unary) and x.op in ('++', '--'))
               for x in ir.walk(e))


def _implicit_body(b):
    """A lambda whose body was an expression: Roslyn wraps it in an implicit block and return."""
    if b.get('k') != 'Block':
        return False
    ops = b.get('operations', [])
    return b.get('implicit', False) and len(ops) == 1 and ops[0].get('implicit', False)


_PREC = {'*': 14, '/': 14, '%': 14, '+': 13, '-': 13, '<<': 12, '>>': 12, '>>>': 12, '<': 11, '>': 11, '<=': 11,
         '>=': 11, '==': 10, '!=': 10, '&': 9, '^': 8, '|': 7, '&&': 6, '||': 5, '??': 4}


def _prec(e):
    if isinstance(e, ir.Binary):
        return _PREC.get(e.op, 0)
    if isinstance(e, ir.Concat):
        return 13
    if isinstance(e, ir.Cond):
        return 3
    if isinstance(e, (ir.Assign, ir.Lambda)):
        return 2
    if isinstance(e, ir.InstanceOf):
        return 11
    if isinstance(e, (ir.Unary, ir.Cast)):
        return 15 if not (isinstance(e, ir.Unary) and e.postfix) else 16
    return 16


def group(node):
    """Roslyn's bound tree keeps no parentheses; the source had them wherever an operand binds looser than
    its operator. Put those back as Paren nodes (as javac's tree has them), so a back end that prints the
    grouping the source wrote groups the operands as C# does."""
    def wrap(child, need):
        if isinstance(child, ir.Expr) and not isinstance(child, ir.Paren) and _prec(child) < need:
            return ir.Paren(x=child, type=child.type, line=child.line, col=child.col)
        return child
    for n in list(ir.walk(node)):
        if isinstance(n, ir.Binary):
            p = _PREC.get(n.op, 0)
            n.left = wrap(n.left, p if n.op != '??' else p + 1)
            n.right = wrap(n.right, p + 1 if n.op != '??' else p)
        elif isinstance(n, ir.Concat):
            n.parts = [wrap(x, 13 if i == 0 else 14) for i, x in enumerate(n.parts)]
        elif isinstance(n, (ir.Unary, ir.Cast)):
            n.x = wrap(n.x, 15)
        elif isinstance(n, ir.InstanceOf):
            n.x = wrap(n.x, 12)
        elif isinstance(n, ir.Cond):
            n.cond = wrap(n.cond, 4)
            n.then = wrap(n.then, 3)
            n.orelse = wrap(n.orelse, 3)
        elif isinstance(n, ir.Select) and isinstance(n.obj, ir.Expr):
            n.obj = wrap(n.obj, 16)
        elif isinstance(n, ir.Call) and isinstance(n.target, ir.Expr):
            n.target = wrap(n.target, 16)
        elif isinstance(n, ir.Index):
            n.arr = wrap(n.arr, 16)
        elif isinstance(n, ir.Length):
            n.arr = wrap(n.arr, 16)
    return node


def lower(tree, path=None):
    """One file's CsTreeDump JSON -> ir.Unit."""
    return group(Lowering(path).unit(tree))


def lower_with_refusals(tree, path=None):
    """-> (ir.Unit, [(line, kind, why)]): what became Unsupported, and why."""
    L = Lowering(path)
    u = group(L.unit(tree))
    return u, L.refused
