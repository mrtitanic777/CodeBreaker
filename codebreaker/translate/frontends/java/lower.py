"""lower.py -- javac's typed tree (TreeDump's JSON) into the translator's IR (codebreaker/translate/ir.py).

Every name is lowered from the symbol javac resolved, not from its spelling: an IDENTIFIER that javac
bound to a field becomes a Select on This (or on its class, when static), one bound to a local a Name;
a MEMBER_SELECT bound to a class is a TypeName, `x.length` on an array a Length, `X.class` a ClassLit.
Constructor calls this(...)/super(...) are CtorCall statements; the ones javac inserted (a constructor
with no explicit call, a class with no constructor) are marked implicit.

Anything not yet handled becomes ir.Unsupported(kind), never a guess.
"""
from codebreaker.translate import ir

# Java's primitives in the IR's language-neutral kinds
_PRIM = {'boolean': 'bool', 'byte': 'i8', 'short': 'i16', 'char': 'u16', 'int': 'i32', 'long': 'i64',
         'float': 'f32', 'double': 'f64', 'void': 'void'}
_LOCALS = ('LOCAL_VARIABLE', 'PARAMETER', 'EXCEPTION_PARAMETER', 'RESOURCE_VARIABLE', 'BINDING_VARIABLE')
_TYPES = ('CLASS', 'INTERFACE', 'ENUM', 'RECORD', 'ANNOTATION_TYPE', 'TYPE_PARAMETER')
_KIND = {'CLASS': 'class', 'INTERFACE': 'interface', 'ENUM': 'enum', 'RECORD': 'record',
         'ANNOTATION_TYPE': 'annotation'}


# -------------------------------------------------------------------------------------- type strings
def parse_type(s, typevars=()):
    """A javac type string ('int[][]', 'java.util.Map<K,java.util.List<V>>', '<nulltype>') -> ir.Type."""
    if s is None:
        return ir.ANY
    t, rest = _type(s.strip(), 0, frozenset(typevars))
    return t


def _type(s, i, tv):
    while i < len(s) and s[i] == ' ':
        i += 1
    if s.startswith('<nulltype>', i) or s.startswith('<nullType>', i):
        return ir.NULL, i + 10
    if s.startswith('<any>', i):
        return ir.ANY, i + 5
    if s.startswith('?', i):
        i += 1
        for word, mode in ((' extends ', 'extends'), (' super ', 'super')):
            if s.startswith(word, i):
                b, i = _type(s, i + len(word), tv)
                return ir.Type('wildcard', bound=(mode, b)), i
        return ir.Type('wildcard'), i
    j = i
    while j < len(s) and s[j] not in '<>,[]&':
        j += 1
    name = s[i:j].strip()
    if name.startswith('capture#'):
        name = 'java.lang.Object'                  # a captured wildcard: its erasure
    args = []
    if j < len(s) and s[j] == '<':
        j += 1
        while True:
            a, j = _type(s, j, tv)
            args.append(a)
            while j < len(s) and s[j] == ' ':
                j += 1
            if j < len(s) and s[j] == ',':
                j += 1
                continue
            j += 1                                 # '>'
            break
    if name in _PRIM:
        t = ir.Type(_PRIM[name]) if name != 'void' else ir.VOID
    elif name in tv or ('.' not in name and len(name) <= 2 and name[:1].isupper() and not args):
        t = ir.Type('typevar', name)
    else:
        t = ir.Type('class', name, args)
    while s.startswith('[]', j):
        t = ir.array_of(t)
        j += 2
    if j < len(s) and s[j] == '&':                 # an intersection: its first bound
        _other, j = _type(s, j + 1, tv)
    return t, j


# -------------------------------------------------------------------------------------- symbols
def sym(d):
    """TreeDump's "$sym" record -> ir.Sym."""
    if not d:
        return None
    k = d['kind']
    if k in _LOCALS:
        kind = 'param' if k == 'PARAMETER' else 'local'
    elif k == 'FIELD':
        kind = 'field'
    elif k == 'ENUM_CONSTANT':
        kind = 'enumconst'
    elif k == 'METHOD':
        kind = 'method'
    elif k == 'CONSTRUCTOR':
        kind = 'ctor'
    elif k in _TYPES:
        kind = 'typevar' if k == 'TYPE_PARAMETER' else 'class'
    elif k == 'PACKAGE':
        kind = 'package'
    else:
        kind = k.lower()
    if 'params' in d:
        return ir.Sym(kind, d['name'], d.get('owner'), d.get('static', False),
                      params=[parse_type(p) for p in d['params']], ret=parse_type(d.get('ret')),
                      varargs=d.get('varargs', False))
    name = d.get('qname') or d['name']
    return ir.Sym(kind, name, d.get('owner'), d.get('static', False), type=parse_type(d.get('type')))


# -------------------------------------------------------------------------------------- the lowering
class Lowering(object):
    def __init__(self, path=None):
        self.path = path

    def pos(self, n):
        return dict(line=n.get('l', 0), col=n.get('c', 0))

    def ty(self, n):
        return parse_type(n.get('$type'))

    # ---- declarations
    def unit(self, n):
        pkg = self.qualified(n.get('packageName')) if n.get('packageName') else ''
        imports = [self.qualified(i.get('qualifiedIdentifier')) + ('.*' if False else '') for i in n.get('imports', [])]
        types = [self.decl(t, None) for t in n.get('typeDecls', []) if t['k'] in _KIND]
        return ir.Unit(path=self.path, package=pkg, imports=imports, types=types, **self.pos(n))

    def qualified(self, n):
        if n is None:
            return ''
        if n['k'] == 'IDENTIFIER':
            return n['name']
        if n['k'] == 'MEMBER_SELECT':
            return self.qualified(n['expression']) + '.' + n['identifier']
        return ''

    def mods(self, n):
        return list((n or {}).get('flags', []))

    def decl(self, n, outer):
        s = sym(n.get('$sym'))
        c = ir.ClassDecl(name=n.get('simpleName'), qname=s.name if s else n.get('simpleName'), kind=_KIND[n['k']],
                         mods=self.mods(n.get('modifiers')),
                         typeparams=[tp.get('name') for tp in n.get('typeParameters', [])],
                         extends=self.ty(n['extendsClause']) if n.get('extendsClause') else None,
                         implements=[self.ty(i) for i in n.get('implementsClause', [])],
                         members=[], outer=outer, **self.pos(n))
        for m in n.get('members', []):
            c.members.append(self.member(m, c, n))
        if c.kind == 'record':
            record_members(c)
        return c

    def member(self, m, cls, raw_cls):
        k = m['k']
        if k in _KIND:
            return self.decl(m, cls.qname)
        if k == 'VARIABLE':
            s = sym(m.get('$sym'))
            if s and s.kind == 'enumconst':
                init = m.get('initializer') or {}
                return ir.EnumConst(sym=s, args=[self.expr(a) for a in init.get('arguments', [])],
                                    body=self.decl(init['classBody'], cls.qname) if init.get('classBody') else None,
                                    ctor=sym(init.get('$sym')), **self.pos(m))
            return ir.FieldDecl(sym=s, type=self.ty(m), init=self.expr(m['initializer']) if m.get('initializer') else None,
                                mods=self.mods(m.get('modifiers')), **self.pos(m))
        if k == 'METHOD':
            return self.method(m, raw_cls)
        if k == 'BLOCK':                                   # an initialiser block
            return ir.MethodDecl(sym=None, name='<clinit>' if m.get('static') else '<init-block>',
                                 kind='static_init' if m.get('static') else 'init', params=[], ret=ir.VOID,
                                 body=self.block(m), mods=[], throws=[], typeparams=[], **self.pos(m))
        return ir.Unsupported(kind=k, raw=None, type=ir.ANY, **self.pos(m))

    def method(self, m, raw_cls):
        s = sym(m.get('$sym'))
        ctor = m.get('name') == '<init>'
        params = [ir.Param(sym=sym(p.get('$sym')), type=self.ty(p), mods=self.mods(p.get('modifiers')), **self.pos(p))
                  for p in m.get('parameters', [])]
        body = self.block(m['body'], ctor_body=ctor) if m.get('body') else None
        mods = self.mods(m.get('modifiers'))
        # javac adds a default constructor to a class that declares none, at the class's own position
        if ctor and m.get('l') == raw_cls.get('l') and m.get('c') == raw_cls.get('c'):
            mods = mods + ['IMPLICIT']
        return ir.MethodDecl(sym=s, name=m.get('name'), kind='ctor' if ctor else 'method', params=params,
                             ret=s.ret if s and s.ret else ir.VOID, body=body, mods=mods,
                             throws=[self.ty(t) for t in m.get('throws', [])],
                             typeparams=[tp.get('name') for tp in m.get('typeParameters', [])], **self.pos(m))

    # ---- statements
    def block(self, n, ctor_body=False):
        stmts = []
        for st in n.get('statements', []):
            s = self.stmt(st)
            if ctor_body and isinstance(s, ir.CtorCall) and st.get('l') == n.get('l') and st.get('c') == n.get('c'):
                s.kind = s.kind + ':implicit'              # javac's own super() at the body's position
            stmts.append(s)
        return ir.Block(stmts=stmts, **self.pos(n))

    def stmt(self, n):
        k = n['k']
        p = self.pos(n)
        if k == 'BLOCK':
            return self.block(n)
        if k == 'VARIABLE':
            return ir.Local(sym=sym(n.get('$sym')), type=self.ty(n),
                            init=self.expr(n['initializer']) if n.get('initializer') else None,
                            mods=self.mods(n.get('modifiers')), **p)
        if k == 'EXPRESSION_STATEMENT':
            e = n['expression']
            sel = e.get('methodSelect') if e['k'] == 'METHOD_INVOCATION' else None
            if sel and sel['k'] == 'IDENTIFIER' and sel['name'] in ('this', 'super'):
                return ir.CtorCall(kind=sel['name'], sym=sym(e.get('$sym')), args=[self.expr(a) for a in e.get('arguments', [])],
                                   target=None, **p)
            if sel and sel['k'] == 'MEMBER_SELECT' and sel['identifier'] in ('this', 'super') \
                    and (sel.get('$sym') or {}).get('kind') == 'CONSTRUCTOR':
                return ir.CtorCall(kind=sel['identifier'], sym=sym(e.get('$sym')),
                                   args=[self.expr(a) for a in e.get('arguments', [])], target=self.expr(sel['expression']), **p)
            return ir.ExprStmt(expr=self.expr(e), **p)
        if k == 'IF':
            return ir.If(cond=self.expr(n['condition']), then=self.stmt(n['thenStatement']),
                         orelse=self.stmt(n['elseStatement']) if n.get('elseStatement') else None, **p)
        if k == 'WHILE_LOOP':
            return ir.While(cond=self.expr(n['condition']), body=self.stmt(n['statement']), **p)
        if k == 'DO_WHILE_LOOP':
            return ir.DoWhile(body=self.stmt(n['statement']), cond=self.expr(n['condition']), **p)
        if k == 'FOR_LOOP':
            return ir.For(init=[self.stmt(s) for s in n.get('initializer', [])],
                          cond=self.expr(n['condition']) if n.get('condition') else None,
                          update=[self.expr(s['expression']) for s in n.get('update', [])],
                          body=self.stmt(n['statement']), **p)
        if k == 'ENHANCED_FOR_LOOP':
            v = n['variable']
            var = ir.Local(sym=sym(v.get('$sym')), type=self.ty(v), init=None, mods=self.mods(v.get('modifiers')), **self.pos(v))
            return ir.ForEach(var=var, iterable=self.expr(n['expression']), body=self.stmt(n['statement']), **p)
        if k == 'SWITCH':
            return ir.Switch(expr=self.expr(n['expression']), cases=[self.case(c) for c in n.get('cases', [])], **p)
        if k == 'BREAK':
            return ir.Break(label=n.get('label'), **p)
        if k == 'CONTINUE':
            return ir.Continue(label=n.get('label'), **p)
        if k == 'RETURN':
            return ir.Return(expr=self.expr(n['expression']) if n.get('expression') else None, **p)
        if k == 'THROW':
            return ir.Throw(expr=self.expr(n['expression']), **p)
        if k == 'TRY':
            catches = []
            for c in n.get('catches', []):
                v = c['parameter']
                tt = v.get('type') or {}
                types = [self.ty(a) for a in tt.get('typeAlternatives', [])] if tt.get('k') == 'UNION_TYPE' else [self.ty(v)]
                catches.append(ir.Catch(sym=sym(v.get('$sym')), types=types, body=self.block(c['block']), **self.pos(c)))
            return ir.Try(resources=[self.stmt(r) if r['k'] == 'VARIABLE' else ir.ExprStmt(expr=self.expr(r), **self.pos(r))
                                     for r in n.get('resources', [])],
                          body=self.block(n['block']), catches=catches,
                          final=self.block(n['finallyBlock']) if n.get('finallyBlock') else None, **p)
        if k == 'LABELED_STATEMENT':
            return ir.Labeled(label=n.get('label'), body=self.stmt(n['statement']), **p)
        if k == 'SYNCHRONIZED':
            return ir.Sync(lock=self.expr(n['expression']), body=self.block(n['block']), **p)
        if k == 'EMPTY_STATEMENT':
            return ir.Empty(**p)
        if k in _KIND:
            return ir.LocalClass(decl=self.decl(n, None), **p)
        if k == 'ASSERT':
            return ir.Assert(cond=self.expr(n['condition']), detail=self.expr(n['detail']) if n.get('detail') else None, **p)
        if k == 'YIELD':
            return ir.Yield(expr=self.expr(n['value']), **p)
        return ir.ExprStmt(expr=ir.Unsupported(kind=k, raw=None, type=ir.ANY, **p), **p)

    def case(self, c, in_expr=False):
        labels, default = [], False
        for lab in c.get('labels', []):
            if lab['k'] == 'DEFAULT_CASE_LABEL':
                default = True
            elif lab['k'] == 'CONSTANT_CASE_LABEL':
                labels.append(self.expr(lab['constantExpression']))
            else:
                labels.append(ir.Unsupported(kind=lab['k'], raw=None, type=ir.ANY, **self.pos(lab)))
        if not c.get('labels'):                                  # older javac: expression(s) or none
            exprs = c.get('expressions') or ([c['expression']] if c.get('expression') else [])
            labels = [self.expr(e) for e in exprs]
            default = not exprs
        if c.get('caseKind') == 'RULE':                          # an arrow arm: a value, a block or a throw
            b = c.get('body')
            if b is None:
                body = []
            elif b['k'] in ('BLOCK', 'THROW', 'EXPRESSION_STATEMENT'):
                body = [self.stmt(b)]
            elif in_expr:
                body = [ir.Yield(expr=self.expr(b), **self.pos(b))]
            else:
                body = [ir.ExprStmt(expr=self.expr(b), **self.pos(b))]
            if not in_expr:
                body.append(ir.Break(label=None, **self.pos(c)))  # an arrow arm does not fall through
        else:
            body = [self.stmt(s) for s in c.get('statements', [])]
        return ir.Case(labels=labels, default=default, body=body, **self.pos(c))

    # ---- expressions
    def expr(self, n):
        k = n['k']
        p = self.pos(n)
        t = self.ty(n)
        if k.endswith('_LITERAL'):
            return self.literal(n, t, p)
        if k == 'PARENTHESIZED':
            return ir.Paren(x=self.expr(n['expression']), type=t, **p)
        if k == 'IDENTIFIER':
            return self.identifier(n, t, p)
        if k == 'MEMBER_SELECT':
            return self.member_select(n, t, p)
        if k == 'METHOD_INVOCATION':
            return self.call(n, t, p)
        if k == 'NEW_CLASS':
            body = self.decl(dict(n['classBody'], k='CLASS'), None) if n.get('classBody') else None
            if body is not None and body.extends is None and n.get('identifier'):
                body.extends = self.ty(n['identifier'])      # what an anonymous class extends or implements
            return ir.New(sym=sym(n.get('$sym')), args=[self.expr(a) for a in n.get('arguments', [])], body=body,
                          outer=self.expr(n['enclosingExpression']) if n.get('enclosingExpression') else None, type=t, **p)
        if k == 'NEW_ARRAY':
            _n, elem = t.dims() if t.kind == 'array' else (0, ir.ANY)
            return ir.NewArray(elem=elem, dims=[self.expr(d) for d in n.get('dimensions', [])],
                               init=[self.expr(e) for e in n['initializers']] if n.get('initializers') is not None else None,
                               type=t, **p)
        if k == 'ARRAY_ACCESS':
            return ir.Index(arr=self.expr(n['expression']), index=self.expr(n['index']), type=t, **p)
        if k in ir.OPERATORS:
            l, r = self.expr(n['leftOperand']), self.expr(n['rightOperand'])
            if k == 'PLUS' and t.kind == 'class' and t.name == 'java.lang.String':
                parts = []
                for x in (l, r):
                    if isinstance(x, ir.Concat):
                        parts.extend(x.parts)
                    else:
                        parts.append(x)
                return ir.Concat(parts=parts, type=t, **p)
            return ir.Binary(op=ir.OPERATORS[k], left=l, right=r, type=t, **p)
        if k in ir.UNARY:
            op, post = ir.UNARY[k]
            return ir.Unary(op=op, x=self.expr(n['expression']), postfix=post, type=t, **p)
        if k == 'ASSIGNMENT':
            return ir.Assign(op=None, target=self.expr(n['variable']), value=self.expr(n['expression']), type=t, **p)
        if k.endswith('_ASSIGNMENT'):
            op = ir.OPERATORS.get(k[:-len('_ASSIGNMENT')])
            if op:
                return ir.Assign(op=op, target=self.expr(n['variable']), value=self.expr(n['expression']), type=t, **p)
        if k == 'CONDITIONAL_EXPRESSION':
            return ir.Cond(cond=self.expr(n['condition']), then=self.expr(n['trueExpression']),
                           orelse=self.expr(n['falseExpression']), type=t, **p)
        if k == 'TYPE_CAST':
            return ir.Cast(x=self.expr(n['expression']), type=t, **p)
        if k == 'INSTANCE_OF':
            pat = n.get('pattern') or {}
            test = n.get('type') or pat.get('variable') or pat.get('var', {})
            binding = None
            if pat.get('k') == 'BINDING_PATTERN':          # `x instanceof T v`: v is bound where it matched
                v = pat.get('variable') or pat['var']
                binding = ir.Local(sym=sym(v.get('$sym')), type=self.ty(v), init=None,
                                   mods=self.mods(v.get('modifiers')), **self.pos(v))
                test = test or v
            elif pat:                                      # a record pattern (deconstruction): not yet
                return ir.Unsupported(kind='INSTANCE_OF:' + pat.get('k', '?'), raw=None, type=t, **p)
            return ir.InstanceOf(x=self.expr(n['expression']), test=self.ty(test) if test else ir.ANY, binding=binding,
                                 type=t, **p)
        if k == 'SWITCH_EXPRESSION':
            return ir.SwitchExpr(expr=self.expr(n['expression']), cases=[self.case(c, True) for c in n.get('cases', [])],
                                 type=t, **p)
        if k == 'LAMBDA_EXPRESSION':
            params = [ir.Param(sym=sym(v.get('$sym')), type=self.ty(v), mods=[], **self.pos(v)) for v in n.get('parameters', [])]
            b = n['body']
            body = self.block(b) if b['k'] == 'BLOCK' else self.expr(b)
            return ir.Lambda(params=params, body=body, type=t, **p)
        if k == 'MEMBER_REFERENCE':
            return ir.MethodRef(target=self.expr(n['qualifierExpression']), sym=sym(n.get('$sym')),
                                mode='new' if n.get('mode') == 'NEW' else 'invoke', type=t, **p)
        return ir.Unsupported(kind=k, raw=None, type=t, **p)

    def literal(self, n, t, p):
        v = n.get('value')
        k = n['k']
        if k == 'NULL_LITERAL':
            v = None
        elif k in ('FLOAT_LITERAL', 'DOUBLE_LITERAL'):
            v = float(v)
        elif k in ('INT_LITERAL', 'LONG_LITERAL'):
            v = int(v)
        elif k == 'BOOLEAN_LITERAL':
            v = bool(v)
        return ir.Lit(value=v, type=t, **p)

    def identifier(self, n, t, p):
        name = n.get('name')
        if name == 'this':
            return ir.This(qualifier=None, type=t, **p)
        if name == 'super':
            return ir.Super(qualifier=None, type=t, **p)
        s = sym(n.get('$sym'))
        if s is None:
            return ir.Unsupported(kind='IDENTIFIER?', raw=name, type=t, **p)
        if s.kind in ('local', 'param'):
            return ir.Name(sym=s, type=t, **p)
        if s.kind in ('field', 'enumconst'):                     # a bare field name: on this, or on its class
            owner = ir.Type('class', s.owner)
            obj = ir.TypeName(type=owner, **p) if s.static else ir.This(qualifier=None, type=owner, **p)
            return ir.Select(obj=obj, sym=s, type=t, **p)
        if s.kind in ('class', 'typevar', 'package'):
            return ir.TypeName(type=t if t.kind != 'any' else ir.Type('class', s.name), **p)
        return ir.Unsupported(kind='IDENTIFIER:' + s.kind, raw=name, type=t, **p)

    def member_select(self, n, t, p):
        ident = n['identifier']
        s = sym(n.get('$sym'))
        inner = n['expression']
        if ident == 'class':
            return ir.ClassLit(of=parse_type(inner.get('$type')), type=t, **p)
        if ident == 'length' and s is not None and s.owner == 'Array':
            return ir.Length(arr=self.expr(inner), type=t, **p)
        if ident == 'this':
            return ir.This(qualifier=parse_type(inner.get('$type')), type=t, **p)
        if ident == 'super':
            return ir.Super(qualifier=parse_type(inner.get('$type')), type=t, **p)
        if s is None:
            return ir.Unsupported(kind='MEMBER_SELECT?', raw=ident, type=t, **p)
        if s.kind in ('class', 'package', 'typevar'):
            return ir.TypeName(type=t if t.kind != 'any' else ir.Type('class', s.name), **p)
        if s.kind in ('field', 'enumconst'):
            return ir.Select(obj=self.expr(inner), sym=s, type=t, **p)
        return ir.Unsupported(kind='MEMBER_SELECT:' + s.kind, raw=ident, type=t, **p)

    def call(self, n, t, p):
        s = sym(n.get('$sym'))
        sel = n['methodSelect']
        args = [self.expr(a) for a in n.get('arguments', [])]
        if sel['k'] == 'IDENTIFIER':
            if s is not None and s.kind == 'method' and s.static:
                target = ir.TypeName(type=ir.Type('class', s.owner), **self.pos(sel))
            else:
                target = None                                     # this class's own method, implicit this
        elif sel['k'] == 'MEMBER_SELECT':
            target = self.expr(sel['expression'])
        else:
            return ir.Unsupported(kind='METHOD_INVOCATION:' + sel['k'], raw=None, type=t, **p)
        return ir.Call(target=target, sym=s, args=args, typeargs=[], type=t, **p)


# -------------------------------------------------------------------------------------- records
# What javac writes for a record only when it lowers it (after the tree TreeDump sees): the canonical
# constructor's field assignments, an accessor per component, equals, hashCode and toString -- made here as
# IR, as java.lang.runtime.ObjectMethods defines them, each marked GENERATED.
_STRING = ir.Type('class', 'java.lang.String')
_OBJECT = ir.Type('class', 'java.lang.Object')
_HASH_OWNER = {'i32': 'java.lang.Integer', 'i64': 'java.lang.Long', 'i16': 'java.lang.Short', 'i8': 'java.lang.Byte',
               'u16': 'java.lang.Character', 'bool': 'java.lang.Boolean', 'f32': 'java.lang.Float',
               'f64': 'java.lang.Double'}


def record_members(c):
    """Add a record's implicit members to ClassDecl `c` (those it does not declare itself)."""
    comps = [m for m in c.members if isinstance(m, ir.FieldDecl) and m.sym is not None and not m.sym.static]
    me = ir.Type('class', c.qname)
    pos = dict(line=c.line, col=c.col)

    def lit(v, t):
        return ir.Lit(value=v, type=t, **pos)

    def this_field(f):
        return ir.Select(obj=ir.This(qualifier=None, type=me, **pos), sym=f.sym, type=f.type, **pos)

    def static_call(owner, name, params, ret, args):
        s = ir.Sym('method', name, owner, True, params=params, ret=ret)
        return ir.Call(target=ir.TypeName(type=ir.Type('class', owner), **pos), sym=s, args=args, typeargs=[],
                       type=ret, **pos)

    def method(name, params, ret, stmts):
        s = ir.Sym('method', name, c.qname, False, params=[p.type for p in params], ret=ret)
        c.members.append(ir.MethodDecl(sym=s, name=name, kind='method', params=params, ret=ret,
                                       body=ir.Block(stmts=stmts, **pos), mods=['public', 'GENERATED'], throws=[],
                                       typeparams=[], **pos))

    def declared(name, nparams):
        return any(isinstance(m, ir.MethodDecl) and m.name == name and len(m.params or []) == nparams
                   for m in c.members)

    # the canonical constructor (javac's own, or a compact one) assigns each component from its parameter
    want = [f.type for f in comps]
    for m in c.members:
        if not (isinstance(m, ir.MethodDecl) and m.kind == 'ctor' and [p.type for p in m.params] == want):
            continue
        assigned = [x for x in ir.walk(m.body) if isinstance(x, ir.Assign) and isinstance(x.target, ir.Select)
                    and isinstance(x.target.obj, ir.This)] if m.body is not None else []
        if m.body is not None and not assigned:
            for f, p in zip(comps, m.params):
                m.body.stmts.append(ir.ExprStmt(expr=ir.Assign(op=None, target=this_field(f), overflow=None,
                                                               value=ir.Name(sym=p.sym, type=p.type, **pos),
                                                               type=f.type, **pos), **pos))
    for f in comps:                                        # x() for each component x
        if not declared(f.sym.name, 0):
            method(f.sym.name, [], f.type, [ir.Return(expr=this_field(f), **pos)])
    if not declared('toString', 0):                        # Point[x=1, y=2]
        parts = [lit(c.name + '[', _STRING)]
        for i, f in enumerate(comps):
            parts += [lit(('' if i == 0 else ', ') + f.sym.name + '=', _STRING), this_field(f)]
        parts.append(lit(']', _STRING))
        method('toString', [], _STRING, [ir.Return(expr=ir.Concat(parts=parts, type=_STRING, **pos), **pos)])
    if not declared('hashCode', 0):                        # h = 31 * h + hash(component), from 0
        h = ir.Sym('local', 'h', c.qname + '.hashCode', type=ir.I32)
        stmts = [ir.Local(sym=h, type=ir.I32, init=lit(0, ir.I32), mods=[], **pos)]
        for f in comps:
            prim = f.type.kind in _HASH_OWNER
            call = static_call(_HASH_OWNER[f.type.kind] if prim else 'java.util.Objects', 'hashCode',
                               [f.type if prim else _OBJECT], ir.I32, [this_field(f)])
            times = ir.Binary(op='*', left=lit(31, ir.I32), right=ir.Name(sym=h, type=ir.I32, **pos), type=ir.I32, **pos)
            stmts.append(ir.ExprStmt(expr=ir.Assign(op=None, target=ir.Name(sym=h, type=ir.I32, **pos), overflow=None,
                                                    value=ir.Binary(op='+', left=times, right=call, type=ir.I32, **pos),
                                                    type=ir.I32, **pos), **pos))
        stmts.append(ir.Return(expr=ir.Name(sym=h, type=ir.I32, **pos), **pos))
        method('hashCode', [], ir.I32, stmts)
    if not declared('equals', 1):                          # the same record type, every component equal
        o = ir.Sym('param', 'o', c.qname + '.equals', type=_OBJECT)
        that = ir.Sym('local', 'that', c.qname + '.equals', type=me)
        isit = ir.InstanceOf(x=ir.Name(sym=o, type=_OBJECT, **pos), test=me, binding=None, type=ir.BOOL, **pos)
        stmts = [ir.If(cond=ir.Unary(op='!', x=ir.Paren(x=isit, type=ir.BOOL, **pos), postfix=False, type=ir.BOOL, **pos),
                       then=ir.Return(expr=lit(False, ir.BOOL), **pos), orelse=None, **pos),
                 ir.Local(sym=that, type=me, init=ir.Cast(x=ir.Name(sym=o, type=_OBJECT, **pos), type=me, **pos),
                          mods=[], **pos)]
        cond = None
        for f in comps:
            mine = this_field(f)
            theirs = ir.Select(obj=ir.Name(sym=that, type=me, **pos), sym=f.sym, type=f.type, **pos)
            if f.type.kind in ('f32', 'f64'):              # Float.compare / Double.compare == 0 (NaN equals NaN)
                eq = ir.Binary(op='==', left=static_call(_HASH_OWNER[f.type.kind], 'compare', [f.type, f.type], ir.I32,
                                                         [mine, theirs]), right=lit(0, ir.I32), type=ir.BOOL, **pos)
            elif f.type.is_prim:
                eq = ir.Binary(op='==', left=mine, right=theirs, type=ir.BOOL, **pos)
            else:
                eq = static_call('java.util.Objects', 'equals', [_OBJECT, _OBJECT], ir.BOOL, [mine, theirs])
            cond = eq if cond is None else ir.Binary(op='&&', left=cond, right=eq, type=ir.BOOL, **pos)
        stmts.append(ir.Return(expr=cond if cond is not None else lit(True, ir.BOOL), **pos))
        method('equals', [ir.Param(sym=o, type=_OBJECT, mods=[], **pos)], ir.BOOL, stmts)


def lower(tree, path=None):
    """One compilation unit's TreeDump JSON -> ir.Unit."""
    return Lowering(path).unit(tree)
