"""desugar.py -- modern constructs lowered into the IR's core, for back ends that cannot say them directly.

The IR keeps the source's intent (a for-each stays a ForEach, a switch expression a SwitchExpr, a pattern
an InstanceOf with its binding) and a back end that has the construct writes it as it is: C++17's
range-for. One that does not -- the C++03 back end, the C back end -- runs this pass first, and sees
only core statements:

  * for-each over an array      { T[] a = xs; for (int i = 0; i < a.length; i++) { T x = a[i]; ... } }
  * for-each over an Iterable   for (Iterator<T> it = xs.iterator(); it.hasNext(); ) { T x = it.next(); ... }
  * a switch expression, where it is a whole statement's value (a local's initialiser, a return, the
    right side of an assignment, an expression statement): a labelled switch statement assigning a
    temporary, each `yield v` as `tmp = v; break label;`
  * a switch on a String (statement or expression), every arm leaving it: an if/else chain on equals()
  * `x instanceof T v` where x is a plain name or field: v declared before the statement, the test as
    `x instanceof T && (v = (T) x) != null`

What it cannot lower it leaves as it was, and the back end declines that method as before (an honest gap,
never a guess). `run(units, keep)` rewrites in place; `keep` names the forms the back end writes itself.
"""
from codebreaker.translate import ir

FORMS = ('foreach', 'switchexpr', 'stringswitch', 'pattern')


def run(units, keep=()):
    """Lower the modern forms in every method of `units` (in place). -> how many were lowered."""
    d = _Desugar(set(FORMS) - set(keep))
    for u in units:
        for c in u.types:
            d.cls(c)
    return d.count


class _Desugar(object):
    def __init__(self, forms):
        self.forms = forms
        self.count = 0
        self.n = 0
        self.owner = ''

    def fresh(self, stem):
        self.n += 1
        return 'j2x_%s%d' % (stem, self.n)

    # ---- declarations
    def cls(self, c):
        for m in c.members:
            if isinstance(m, ir.ClassDecl):
                self.cls(m)
            elif isinstance(m, ir.MethodDecl) and m.body is not None:
                self.owner = (c.qname or c.name or '') + '.' + (m.name or '')
                self.block(m.body)
                self.nested(m.body)
            elif isinstance(m, ir.EnumConst) and m.body is not None:
                self.cls(m.body)

    def nested(self, root):
        """The bodies of the lambdas and anonymous classes inside `root`'s expressions."""
        for n in list(ir.walk(root)):
            if isinstance(n, ir.Lambda) and isinstance(n.body, ir.Block):
                self.block(n.body)
            elif isinstance(n, ir.New) and n.body is not None:
                self.cls(n.body)

    # ---- statements: each list is rewritten, a statement may become several
    def block(self, b):
        if isinstance(b, ir.Block):
            b.stmts = self.stmts(b.stmts)

    def stmts(self, ss):
        out = []
        declared = {}
        for s in ss:
            out.extend(self.stmt(s, declared))
            if isinstance(s, ir.Local) and s.sym is not None:
                declared[s.sym.name] = s.type
        return out

    def sub(self, s):
        """A nested statement (a loop's or an if's body): one statement back."""
        if s is None:
            return None
        got = self.stmt(s, {})
        if len(got) == 1:
            return got[0]
        return ir.Block(stmts=got, line=s.line, col=s.col)

    def stmt(self, s, declared):
        pre = []
        if 'pattern' in self.forms:
            pre = self.patterns(s, declared)
        out = pre + self._stmt(s)
        return out

    def _stmt(self, s):
        p = dict(line=s.line, col=s.col)
        if isinstance(s, ir.Block):
            self.block(s)
            return [s]
        if isinstance(s, ir.If):
            s.then, s.orelse = self.sub(s.then), self.sub(s.orelse)
            return [s]
        if isinstance(s, (ir.While, ir.DoWhile)):
            s.body = self.sub(s.body)
            return [s]
        if isinstance(s, ir.For):
            s.body = self.sub(s.body)
            return [s]
        if isinstance(s, ir.Labeled):
            if isinstance(s.body, ir.ForEach) and 'foreach' in self.forms:
                got = self.foreach(s.body)
                if got is not None:                        # the label goes on the loop, not its set-up
                    pre, loop = got
                    s.body = loop
                    return [ir.Block(stmts=pre + [s], **p)] if pre else [s]
            s.body = self.sub(s.body)
            return [s]
        if isinstance(s, ir.ForEach):
            s.body = self.sub(s.body)
            if 'foreach' in self.forms:
                got = self.foreach(s)
                if got is not None:
                    pre, loop = got
                    return [ir.Block(stmts=pre + [loop], **p)] if pre else [loop]
            return [s]
        if isinstance(s, ir.Switch):
            for c in s.cases:
                c.body = self.stmts(c.body)
            if 'stringswitch' in self.forms and _is_string(s.expr.type):
                got = self.string_switch(s)
                if got is not None:
                    return got
            return [s]
        if isinstance(s, ir.Try):
            self.block(s.body)
            for c in s.catches:
                self.block(c.body)
            if s.final is not None:
                self.block(s.final)
            return [s]
        if isinstance(s, ir.Sync):
            self.block(s.body)
            return [s]
        if isinstance(s, ir.LocalClass):
            self.cls(s.decl)
            return [s]
        if 'switchexpr' in self.forms:
            got = self.switch_expr_stmt(s)
            if got is not None:
                return got
        return [s]

    # ---- for-each
    def foreach(self, s):
        """(set-up statements, the loop) for a ForEach, or None."""
        p = dict(line=s.line, col=s.col)
        it = s.iterable
        t = it.type
        var = s.var
        body = s.body if isinstance(s.body, ir.Block) else ir.Block(stmts=[s.body], **p)
        if t is not None and t.kind == 'array':
            arr = ir.Sym('local', self.fresh('a'), self.owner, type=t)
            idx = ir.Sym('local', self.fresh('i'), self.owner, type=ir.I32)
            a = lambda: ir.Name(sym=arr, type=t, **p)
            i = lambda: ir.Name(sym=idx, type=ir.I32, **p)
            elem = ir.Local(sym=var.sym, type=var.type, init=ir.Index(arr=a(), index=i(), type=t.elem, **p),
                            mods=var.mods, line=var.line, col=var.col)
            loop = ir.For(init=[ir.Local(sym=idx, type=ir.I32, init=ir.Lit(value=0, type=ir.I32, **p), mods=[], **p)],
                          cond=ir.Binary(op='<', left=i(), right=ir.Length(arr=a(), type=ir.I32, **p), type=ir.BOOL, **p),
                          update=[ir.Unary(op='++', x=i(), postfix=True, type=ir.I32, **p)],
                          body=ir.Block(stmts=[elem] + list(body.stmts), **p), **p)
            self.count += 1
            return [ir.Local(sym=arr, type=t, init=it, mods=[], **p)], loop
        if t is not None and t.kind == 'class':
            elem_t = t.args[0] if t.args else ir.Type('class', 'java.lang.Object')
            if elem_t.kind in ('wildcard', 'typevar'):
                elem_t = (elem_t.bound[1] if elem_t.kind == 'wildcard' and elem_t.bound and elem_t.bound[0] == 'extends'
                          else ir.Type('class', 'java.lang.Object'))
            it_t = ir.Type('class', 'java.util.Iterator', [elem_t])
            itsym = ir.Sym('local', self.fresh('it'), self.owner, type=it_t)
            n = lambda: ir.Name(sym=itsym, type=it_t, **p)
            iterator = ir.Call(target=it, sym=ir.Sym('method', 'iterator', t.name, False, params=[], ret=it_t),
                               args=[], typeargs=[], type=it_t, **p)
            has = ir.Call(target=n(), sym=ir.Sym('method', 'hasNext', 'java.util.Iterator', False, params=[], ret=ir.BOOL),
                          args=[], typeargs=[], type=ir.BOOL, **p)
            nxt = ir.Call(target=n(), sym=ir.Sym('method', 'next', 'java.util.Iterator', False, params=[],
                                                 ret=ir.Type('typevar', 'E')),
                          args=[], typeargs=[], type=elem_t, **p)
            elem = ir.Local(sym=var.sym, type=var.type, init=nxt, mods=var.mods, line=var.line, col=var.col)
            loop = ir.For(init=[ir.Local(sym=itsym, type=it_t, init=iterator, mods=[], **p)], cond=has, update=[],
                          body=ir.Block(stmts=[elem] + list(body.stmts), **p), **p)
            self.count += 1
            return [], loop
        return None

    # ---- switch expressions
    def switch_expr_stmt(self, s):
        p = dict(line=s.line, col=s.col)
        if isinstance(s, ir.Local) and isinstance(_strip(s.init), ir.SwitchExpr):
            sw = _strip(s.init)
            decl = ir.Local(sym=s.sym, type=s.type, init=None, mods=s.mods, **p)
            target = ir.Name(sym=s.sym, type=s.type, **p)
            return [decl] + self.switch_into(sw, target)
        if isinstance(s, ir.Return) and isinstance(_strip(s.expr), ir.SwitchExpr):
            sw = _strip(s.expr)
            tmp = ir.Sym('local', self.fresh('sw'), self.owner, type=sw.type)
            decl = ir.Local(sym=tmp, type=sw.type, init=None, mods=[], **p)
            got = self.switch_into(sw, ir.Name(sym=tmp, type=sw.type, **p))
            return [ir.Block(stmts=[decl] + got + [ir.Return(expr=ir.Name(sym=tmp, type=sw.type, **p), **p)], **p)]
        if isinstance(s, ir.ExprStmt) and isinstance(s.expr, ir.Assign) and s.expr.op is None \
                and isinstance(_strip(s.expr.value), ir.SwitchExpr) and _plain(s.expr.target):
            return self.switch_into(_strip(s.expr.value), s.expr.target)
        return self.switch_expr_inside(s)

    def switch_expr_inside(self, s):
        """A switch expression deeper in a statement's expression (`list.add(switch ...)`): taken out into a
        temporary before the statement, when that cannot change what happens -- it is the only one, nothing
        else in the expression has an effect but the calls and assignments it is an operand of (they run after
        it anyway), and the switch writes nothing the rest reads."""
        root = s.expr if isinstance(s, (ir.ExprStmt, ir.Return)) else (s.init if isinstance(s, ir.Local) else None)
        if root is None:
            return None
        found = [n for n in _own_nodes(root) if isinstance(n, ir.SwitchExpr)]
        if len(found) != 1 or found[0] is root:
            return None
        sw = found[0]
        chain = _ancestors(root, sw)
        if chain is None:
            return None
        from codebreaker.translate.backends import cpp03
        others = [n for n in _siblings(chain, sw)]
        if any(cpp03._effects(n) for n in others):
            return None
        if cpp03._written(sw) & set().union(*[cpp03._read(n) for n in others] or [set()]):
            return None
        p = dict(line=s.line, col=s.col)
        tmp = ir.Sym('local', self.fresh('sw'), self.owner, type=sw.type)
        decl = ir.Local(sym=tmp, type=sw.type, init=None, mods=[], **p)
        got = self.switch_into(sw, ir.Name(sym=tmp, type=sw.type, **p))
        _replace(s, {id(sw): ir.Name(sym=tmp, type=sw.type, **p)})
        return [ir.Block(stmts=[decl] + got + [s], **p)] if not isinstance(s, ir.Local) else [decl] + got + [s]

    def switch_into(self, sw, target):
        """The statements of switch expression `sw` assigning its value to `target` (a Name or plain Select)."""
        p = dict(line=sw.line, col=sw.col)
        label = self.fresh('sw')
        cases = []
        for c in sw.cases:
            body = self.stmts([self.yields(b, target, label) for b in c.body])
            cases.append(ir.Case(labels=c.labels, default=c.default, body=body, line=c.line, col=c.col))
        st = ir.Switch(expr=sw.expr, cases=cases, **p)
        self.count += 1
        if 'stringswitch' in self.forms and _is_string(sw.expr.type):
            got = self.string_switch(st, label)
            if got is not None:
                return got
        return [ir.Labeled(label=label, body=st, **p)]

    def yields(self, s, target, label):
        """`s` with each Yield of this switch expression (not of one nested in it) as an assignment and a
        labelled break."""
        p = dict(line=s.line, col=s.col)
        if isinstance(s, ir.Yield):
            return ir.Block(stmts=[ir.ExprStmt(expr=ir.Assign(op=None, target=_copy_target(target), value=s.expr,
                                                              overflow=None, type=target.type, **p), **p),
                                   ir.Break(label=label, **p)], **p)
        for f in s.fields:
            v = getattr(s, f)
            if isinstance(v, ir.Stmt) and not isinstance(v, ir.Case):
                setattr(s, f, self.yields(v, target, label))
            elif isinstance(v, list):
                setattr(s, f, [self.yields(x, target, label) if isinstance(x, ir.Stmt) else x for x in v])
        return s

    # ---- a switch on a String
    def string_switch(self, s, label=None):
        """An if/else chain for a switch on a String whose every arm leaves it (break, return, throw,
        yield-break), or None."""
        p = dict(line=s.line, col=s.col)
        for k, c in enumerate(s.cases):
            if (c.default and c.labels) or (k < len(s.cases) - 1 and (not c.body or not _leaves_arm(c.body[-1]))):
                return None                                # a fall-through, or labels sharing the default
        label = label or self.fresh('sw')
        sel = ir.Sym('local', self.fresh('s'), self.owner, type=s.expr.type)
        eqs = ir.Sym('method', 'equals', 'java.lang.String', False, params=[ir.Type('class', 'java.lang.Object')],
                     ret=ir.BOOL)
        default, arms = None, []
        for c in s.cases:
            body = ir.Block(stmts=[_label_breaks(b, label) for b in c.body], line=c.line, col=c.col)
            if c.default:
                default = body
                continue
            cond = None
            for lab in c.labels:
                test = ir.Call(target=lab, sym=eqs, args=[ir.Name(sym=sel, type=s.expr.type, **p)], typeargs=[],
                               type=ir.BOOL, **p)
                cond = test if cond is None else ir.Binary(op='||', left=cond, right=test, type=ir.BOOL, **p)
            arms.append((cond, body))
        chain = default
        for cond, body in reversed(arms):
            chain = ir.If(cond=cond, then=body, orelse=chain, **p)
        if chain is None:
            chain = ir.Empty(**p)
        self.count += 1
        decl = ir.Local(sym=sel, type=s.expr.type, init=s.expr, mods=[], **p)
        return [ir.Block(stmts=[decl, ir.Labeled(label=label, body=ir.Block(stmts=[chain], **p), **p)], **p)]

    # ---- instanceof patterns
    def patterns(self, s, declared):
        """Declarations of the pattern variables of statement `s`'s own expressions (not those of nested
        statements, handled at their own level), each pattern rewritten to assign its variable."""
        exprs = []
        if isinstance(s, (ir.If, ir.While)):
            exprs = [s.cond]
        elif isinstance(s, ir.DoWhile):
            exprs = []
        elif isinstance(s, (ir.ExprStmt, ir.Return)):
            exprs = [s.expr]
        elif isinstance(s, ir.Local):
            exprs = [s.init]
        out, swap = [], {}
        for root in exprs:
            if root is None:
                continue
            for n in _own_nodes(root):
                if not (isinstance(n, ir.InstanceOf) and n.binding is not None and _plain(n.x)):
                    continue
                b = n.binding
                p = dict(line=n.line, col=n.col)
                if declared.get(b.sym.name) is None:
                    out.append(ir.Local(sym=b.sym, type=b.type, init=None, mods=[], line=b.line, col=b.col))
                    declared[b.sym.name] = b.type
                elif declared[b.sym.name] != b.type:
                    continue                               # another type under the same name: left as it is
                test = ir.InstanceOf(x=n.x, test=n.test, binding=None, type=ir.BOOL, **p)
                assign = ir.Assign(op=None, target=ir.Name(sym=b.sym, type=b.type, **p),
                                   value=ir.Cast(x=_copy_target(n.x), type=b.type, **p), overflow=None, type=b.type, **p)
                bind = ir.Binary(op='!=', left=ir.Paren(x=assign, type=b.type, **p),
                                 right=ir.Lit(value=None, type=ir.NULL, **p), type=ir.BOOL, **p)
                swap[id(n)] = ir.Paren(x=ir.Binary(op='&&', left=test, right=bind, type=ir.BOOL, **p), type=ir.BOOL, **p)
                self.count += 1
        if swap:
            _replace(s, swap)
        return out


# ---------------------------------------------------------------------------------------------- helpers
def _own_nodes(root):
    """The nodes of an expression, not entering lambdas, switch expressions or anonymous classes (their
    statements are another level's)."""
    stack = [root]
    while stack:
        n = stack.pop()
        yield n
        if isinstance(n, (ir.Lambda, ir.SwitchExpr, ir.ClassDecl)):
            continue
        stack.extend(reversed(list(n.children())))


def _ancestors(root, target):
    """The nodes from root down to target's parent, or None when target is not under root."""
    if root is target:
        return []
    for c in root.children():
        if isinstance(c, (ir.Lambda, ir.ClassDecl)):
            continue
        got = _ancestors(c, target)
        if got is not None:
            return [root] + got
    return None


def _siblings(chain, target):
    """Every operand of the chain's nodes that is not on the path to target (each evaluated whole)."""
    path = set(id(n) for n in chain) | {id(target)}
    for n in chain:
        for c in n.children():
            if id(c) not in path:
                yield c


def _replace(node, swap):
    """Replace, in place under `node`, each child whose id is in `swap` by its replacement."""
    for f in node.fields:
        v = getattr(node, f)
        if isinstance(v, ir.Node):
            if id(v) in swap:
                setattr(node, f, swap[id(v)])
            else:
                _replace(v, swap)
        elif isinstance(v, list):
            for i, x in enumerate(v):
                if isinstance(x, ir.Node):
                    if id(x) in swap:
                        v[i] = swap[id(x)]
                    else:
                        _replace(x, swap)


def _strip(e):
    while isinstance(e, ir.Paren):
        e = e.x
    return e


def _is_string(t):
    return t is not None and t.kind == 'class' and t.name == 'java.lang.String'


def _plain(e):
    """A name, or a field of this / of a class / of a plain name: evaluating it twice is reading it twice."""
    e = _strip(e)
    if isinstance(e, ir.Name):
        return True
    if isinstance(e, ir.Select):
        return isinstance(e.obj, (ir.This, ir.TypeName)) or _plain(e.obj)
    return False


def _copy_target(e):
    e = _strip(e)
    if isinstance(e, ir.Name):
        return ir.Name(sym=e.sym, type=e.type, line=e.line, col=e.col)
    if isinstance(e, ir.Select):
        obj = e.obj if isinstance(e.obj, (ir.This, ir.TypeName)) else _copy_target(e.obj)
        return ir.Select(obj=obj, sym=e.sym, type=e.type, line=e.line, col=e.col)
    return e


def _leaves_arm(s):
    if isinstance(s, (ir.Break, ir.Return, ir.Throw, ir.Continue)):
        return True
    if isinstance(s, ir.Block):
        return bool(s.stmts) and _leaves_arm(s.stmts[-1])
    if isinstance(s, ir.If):
        return s.orelse is not None and _leaves_arm(s.then) and _leaves_arm(s.orelse)
    return False


def _label_breaks(s, label):
    """`s` with each unlabelled break that leaves the switch (not an inner loop or switch) labelled."""
    if isinstance(s, ir.Break) and s.label is None:
        return ir.Break(label=label, line=s.line, col=s.col)
    if isinstance(s, (ir.While, ir.DoWhile, ir.For, ir.ForEach, ir.Switch)):
        return s
    for f in s.fields:
        v = getattr(s, f)
        if isinstance(v, ir.Stmt):
            setattr(s, f, _label_breaks(v, label))
        elif isinstance(v, list):
            setattr(s, f, [_label_breaks(x, label) if isinstance(x, ir.Stmt) else x for x in v])
    return s
