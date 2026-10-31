"""infer.py -- what each value of a Python program is, found without running it.

An abstract interpreter over the Python AST. Each function is run on types instead of values, flow by
flow: a branch's condition narrows what it tests (`x is None`, `p < len(data)`, `isinstance`), a loop is
run to a fixpoint (integer bounds widened over thresholds, then narrowed back), and a successful access
bounds the index that made it (`data[p]` returning means p is inside data). What a function learns about
others -- the arguments it passes, what it stores into attributes and containers -- becomes program facts
(program.Program), and the program is run round after round until no fact changes.

An integer's interval is what lets it be int64_t in C++. Where none can be shown, its expressions carry
an unbounded interval and the lowering refuses them (int-range). Counters get one more rule: a variable a
loop only ever increases by a bounded step, in a loop whose number of passes is bounded (a range, a walk
over a sequence -- bounded by the address space --, a while loop over a variable it strictly increases
toward a bound), is bounded by its start plus steps times passes.

Everything the interpreter cannot type is an Issue on the node: (class, detail). The classes:

    untyped      a parameter no annotation or call site gives a type
    conflict     values of two kinds meet in one place (no one C++ type)
    int-range    an integer not shown to stay inside int64
    dynamic      a dynamic feature (getattr, *args, a generator, set iteration order, ...)
    stdlib       a standard library call not mapped (detail: which)
    extern       a library outside the program and the standard library
    syntax       a construct the front end does not lower (with, finally, nested def, ...)
    unresolved   a name not found
"""
import ast

from codebreaker.translate.frontends.python import pytypes as P
from codebreaker.translate.frontends.python.program import EXCEPTIONS

BUILTIN_FUNCS = frozenset('''len range enumerate zip isinstance int str bytes bytearray bool float min max abs
sorted reversed sum any all list dict set tuple print open ord chr hex oct bin repr divmod round pow format
super iter next map filter getattr setattr hasattr type id hash vars globals locals callable issubclass
object frozenset delattr exec eval compile input memoryview staticmethod classmethod property'''.split())
DYNAMIC_BUILTINS = frozenset('getattr setattr hasattr delattr type id hash vars globals locals callable issubclass '
                             'exec eval compile iter next memoryview input'.split())
MAX_LOOP = 10


def _has_top(t, depth=0):
    if t.kind == 'top':
        return True
    return depth < 6 and any(_has_top(a, depth + 1) for a in t.args)


class Issue(object):
    __slots__ = ('cls', 'detail', 'line')

    def __init__(self, cls, detail, line):
        self.cls, self.detail, self.line = cls, detail, line

    @property
    def reason(self):
        return self.cls + (':' + self.detail if self.detail else '')

    def __repr__(self):
        return 'Issue(%s at %s)' % (self.reason, self.line)


def _str_const(n):
    return n.value if isinstance(n, ast.Constant) and isinstance(n.value, str) else None


def _int_const(n):
    if isinstance(n, ast.Constant) and isinstance(n.value, int) and not isinstance(n.value, bool):
        return n.value
    if isinstance(n, ast.UnaryOp) and isinstance(n.op, ast.USub):
        v = _int_const(n.operand)
        return -v if v is not None else None
    return None


def assigned_names(fn_node):
    """Every name a function binds (Python's rule: a name assigned anywhere in the body is local)."""
    out = set()
    todo = list(fn_node.body)
    while todo:
        n = todo.pop()
        if isinstance(n, (ast.FunctionDef, ast.AsyncFunctionDef, ast.ClassDef)):
            out.add(n.name)
            continue
        if isinstance(n, ast.Lambda):
            continue
        if isinstance(n, ast.Name) and isinstance(n.ctx, (ast.Store, ast.Del)):
            out.add(n.id)
        elif isinstance(n, (ast.Import, ast.ImportFrom)):
            for a in n.names:
                out.add((a.asname or a.name).split('.')[0])
        elif isinstance(n, ast.ExceptHandler) and n.name:
            out.add(n.name)
        if isinstance(n, (ast.ListComp, ast.SetComp, ast.DictComp, ast.GeneratorExp)):
            continue                                          # their targets are their own scope
        todo.extend(ast.iter_child_nodes(n))
    return out


class Eval(object):
    """One function run on types. With `record`, every node's type and resolution is kept for lowering."""

    def __init__(self, prog, fn, record=False):
        self.p = prog
        self.fn = fn
        self.m = fn.module if fn is not None else None
        self.record = record
        self.quiet = 0
        self.types = {}
        self.res = {}
        self.seen = {}
        self.issues = []
        self.ret = P.BOTTOM
        self.returns_value = False
        self.loops = []
        self.locals = set()
        self.local_imports = {}
        self.cls = fn.cls if fn is not None else None
        self.builder = None
        self.loop_memo = {}

    # ------------------------------------------------------------------------------------- bookkeeping
    def note(self, node, t, res=None):
        k = id(node)
        old = self.seen.get(k)
        self.seen[k] = t if old is None else self.p.join(old, t)
        if self.record and not self.quiet:
            old = self.types.get(k)
            self.types[k] = t if old is None else self.p.join(old, t)
            if res is not None:
                self.res[k] = res
        return t

    def issue(self, node, cls, detail=''):
        if self.record and not self.quiet:
            self.issues.append(Issue(cls, detail, getattr(node, 'lineno', 0)))
        return P.top('%s:%s' % (cls, detail))

    @property
    def live(self):
        """facts are contributed only from a pass whose loop heads are final"""
        return not self.quiet

    def contribute_param(self, fq, name, t):
        # a value with no single type is the caller's refusal, not the callee's: it is not passed on
        # (but remembered: a parameter only such callers reach is untyped because of them)
        if self.live and t.kind == 'top' and (fq, name) not in self.p.param_poison:
            self.p.param_poison[(fq, name)] = (self.fn.qname if self.fn else '?', t.why or '')
        if not self.live or t.kind in ('bottom', 'top') or (t.kind in ('tuple', 'opt') and _has_top(t)):
            return
        f = self.p.fns[fq]
        ps = self.p.params.setdefault(fq, {})
        old = ps.get(name, P.BOTTOM)
        ann = f.annotations.get(name)
        if ann is not None:
            at = self.p.annotation_type(f, name)
            if at.kind == 'int' and t.kind == 'int':     # the annotation's kind, the call sites' range
                new = self.p.join(old, t) if old.kind != 'bottom' else t
            else:
                new = at
        else:
            new = self.p.join(old, t)
        if self.p.round > 3 and new.kind == 'int' and old.kind == 'int':
            new = P.widen_int(old, new)
        if new != old:
            ps[name] = new
            self.p.touch(('p', fq))

    def arg_fit(self, node, f, name, t):
        """an argument: it fits int64, and it is not None where the callee does not expect None"""
        t = self.fit(node, t, 'argument ' + name)
        if t.kind == 'opt' and name not in getattr(f, 'none_aware', ()):
            self.issue(node, 'conflict', 'passes a value that may be None as %s of %s' % (name, f.name))
            return t.args[0]
        if t.kind == 'none' and name not in getattr(f, 'none_aware', ()):
            self.issue(node, 'conflict', 'passes None as %s of %s' % (name, f.name))
            return P.BOTTOM
        return t

    def fit(self, node, t, what):
        """A value leaving this function (an argument, an attribute, an item): an int must be shown to fit
        int64 here -- the issue is this function's -- and what arrives is within int64."""
        if t.kind == 'int' and not t.fits_i64():
            if self.p.int_policy == 'strict':
                self.issue(node, 'int-range', '%s in [%s, %s]' % (what, t.lo, t.hi))
            return P.meet(t, P.I64_MIN, P.I64_MAX)
        if t.kind in ('tuple', 'opt') and any(a.kind == 'int' and not a.fits_i64() for a in t.args):
            return P.T(t.kind, [self.fit(node, a, what) for a in t.args])
        return t

    def contribute_attr(self, cq, attr, t):
        if self.live and (t.kind == 'top' or (t.kind in ('tuple', 'opt') and _has_top(t))):
            owner = self.p.attr_owner(cq, attr) or cq
            if (owner, attr) not in self.p.poisoned:
                self.p.poisoned[(owner, attr)] = self.fn.qname if self.fn else '?'
                self.p.touch(('a', owner))
            return
        if not self.live or t.kind in ('bottom', 'top') or (t.kind in ('tuple', 'opt') and _has_top(t)):
            return
        owner = self.p.attr_owner(cq, attr) or cq
        a = self.p.attrs.setdefault(owner, {})
        old = a.get(attr, P.BOTTOM)
        new = self.p.join(old, t)
        if self.p.round > 3 and new.kind == 'int' and old.kind == 'int':
            new = P.widen_int(old, new)
        if new != old:
            a[attr] = new
            self.p.touch(('a', owner))

    # ------------------------------------------------------------------------------------- the function
    def run(self):
        f = self.fn
        node = f.node
        self.locals = self.p.fn_locals(f)
        state = {}
        if f.varargs or f.kwargs:
            self.issue(node, 'dynamic', '*args' if f.varargs else '**kwargs')
        if f.kind == 'async':
            self.issue(node, 'dynamic', 'async')
        for d in f.decorators:
            if d not in ('staticmethod', 'property'):
                self.issue(node, 'dynamic', 'decorator ' + d)
        for i, name in enumerate(f.params + f.kwonly):
            if i == 0 and f.kind in ('method', 'property'):
                t = P.T('inst', name=f.cls.qname)
            elif i == 0 and f.kind == 'classmethod':
                t = P.T('cls', name=f.cls.qname)
            else:
                t = self.p.param_type(f, name)
            state[name] = t
        for st in ast.walk(node):
            if isinstance(st, (ast.Yield, ast.YieldFrom)):
                self.issue(st, 'dynamic', 'generator')
                break
        out = self.block(node.body, state)
        if out is not None and self.returns_value:
            self.ret = self.p.join(self.ret, P.NONE)
        if not self.returns_value:
            self.ret = P.NONE
        if self.ret.kind == 'int' and not self.ret.fits_i64():
            self.ret = self.fit(node, self.ret, 'return value')
        if self.live:
            fq = f.qname
            old = self.p.returns.get(fq, P.BOTTOM)
            new = self.p.join(old, self.ret)
            if self.p.round > 3 and new.kind == 'int' and old.kind == 'int':
                new = P.widen_int(old, new)
            if new != old:
                self.p.returns[fq] = new
                self.p.touch(('r', fq))
        return self

    # ------------------------------------------------------------------------------------- statements
    def block(self, stmts, state):
        for st in stmts:
            if state is None:
                return None
            state = self.stmt(st, state)
        return state

    def stmt(self, st, state):
        m = getattr(self, 's_' + type(st).__name__, None)
        if m is None:
            self.issue(st, 'syntax', type(st).__name__.lower())
            return state
        return m(st, state)

    def s_Pass(self, st, state):
        return state

    def s_Expr(self, st, state):
        if isinstance(st.value, ast.Constant):
            return state
        self.ev(st.value, state)
        return self.after_access(st.value, state)

    def s_Assign(self, st, state):
        if len(st.targets) == 1 and isinstance(st.targets[0], ast.Name) and isinstance(st.value, (ast.Dict, ast.Call)):
            self.builder = id(st.value)                       # its creation keys are counted by builder_keys
        t = self.ev(st.value, state)
        state = self.after_access(st.value, state)
        for tgt in st.targets:
            state = self.assign(tgt, t, state, st.value)
        if len(st.targets) == 1 and isinstance(st.targets[0], ast.Name) and isinstance(st.value, (ast.Dict, ast.Call)):
            self.builder_keys(st, t)
        return state

    def s_AnnAssign(self, st, state):
        if st.value is None:
            return state
        t = self.ev(st.value, state)
        if isinstance(st.target, ast.Name):
            at = self.p.ann_to_type(self.m, st.annotation, st.annotation)
            if at.kind not in ('bottom', 'top', 'int'):
                t = at if t.kind in ('list', 'dict', 'set') and at.kind == t.kind and self.p.join(at, t) else at
        return self.assign(st.target, t, state, st.value)

    def s_AugAssign(self, st, state):
        cur = self.ev(st.target, state, aug=True)
        val = self.ev(st.value, state)
        t = self.binop(st, st.op, cur, val, st.target, st.value)
        self.note(st, t)
        return self.assign(st.target, t, state, None, aug=True)

    def s_Return(self, st, state):
        if st.value is not None:
            t = self.ev(st.value, state)
            self.returns_value = True
            if self.live:
                self.ret = self.p.join(self.ret, t)
        elif self.live:
            self.ret = self.p.join(self.ret, P.NONE)
        return None

    def s_Raise(self, st, state):
        if st.exc is not None:
            t = self.ev(st.exc, state)
            if t.kind not in ('inst', 'cls', 'bottom', 'top') and not (t.kind == 'extern'):
                self.issue(st, 'conflict', 'raise of a %s' % t.kind)
        if st.cause is not None:
            self.issue(st, 'syntax', 'raise from')
        return None

    def s_Assert(self, st, state):
        self.ev(st.test, state)
        if st.msg is not None:
            self.ev(st.msg, state)
        return self.refine(st.test, state, True)

    def s_Global(self, st, state):
        self.issue(st, 'dynamic', 'global')
        return state

    def s_Nonlocal(self, st, state):
        self.issue(st, 'dynamic', 'nonlocal')
        return state

    def s_Delete(self, st, state):
        self.issue(st, 'dynamic', 'del')
        return state

    def s_FunctionDef(self, st, state):
        self.issue(st, 'syntax', 'nested function')
        return state

    s_AsyncFunctionDef = s_FunctionDef

    def s_ClassDef(self, st, state):
        self.issue(st, 'syntax', 'nested class')
        return state

    def s_Import(self, st, state):
        for a in st.names:
            local = a.asname or a.name.split('.')[0]
            self.local_imports[local] = self.p._module_ref(a.name if a.asname else a.name.split('.')[0])
            state = dict(state)
            state.pop(local, None)
        return state

    def s_ImportFrom(self, st, state):
        base = self.p._from_base(self.m, st)
        for a in st.names:
            local = a.asname or a.name
            full = base + '.' + a.name if base else a.name
            if full in self.p.modules:
                self.local_imports[local] = ('module', full)
            elif base in self.p.modules:
                r = self.p.resolve_global(self.p.modules[base], a.name)
                self.local_imports[local] = r if r else ('missing', full)
            else:
                top = full.split('.')[0]
                from codebreaker.translate.frontends.python.program import STDLIB
                self.local_imports[local] = ('stdobj', full) if top in STDLIB else ('extern', full)
            state = dict(state)
            state.pop(local, None)
        return state

    def s_If(self, st, state):
        self.ev(st.test, state)
        state = self.after_access(st.test, state)
        a = self.block(st.body, self.refine(st.test, state, True))
        b = self.block(st.orelse, self.refine(st.test, state, False))
        return self.join_states(a, b)

    def s_With(self, st, state):
        for item in st.items:
            ce = item.context_expr
            t = self.ev(ce, state)
            if not (t.kind == 'extern' and t.name == 'file'):
                self.issue(st, 'syntax', 'with')
            if item.optional_vars is not None:
                state = self.assign(item.optional_vars, t, state, None)
        return self.block(st.body, state)

    def s_Try(self, st, state):
        if st.finalbody:
            self.issue(st, 'syntax', 'finally')
        if st.orelse:
            self.issue(st, 'syntax', 'try-else')
        body = self.block(st.body, dict(state))
        # a handler can be entered from anywhere in the body: its state is what the body had at entry
        # joined with every state along the body (approximated by entry joined with the end of the body)
        entry = self.join_states(dict(state), body) if body is not None else dict(state)
        outs = [body]
        for h in st.handlers:
            hs = dict(entry)
            if h.type is not None:
                names = h.type.elts if isinstance(h.type, ast.Tuple) else [h.type]
                for n in names:
                    if not (isinstance(n, ast.Name) and (n.id in EXCEPTIONS or self.lookup_class(n) is not None)) \
                            and not self._std_exception(n):
                        self.issue(h, 'dynamic', 'except ' + ast.unparse(n))
                if h.name:
                    self.issue(h, 'syntax', 'except as')
                    hs[h.name] = P.T('extern', name='exception')
            outs.append(self.block(h.body, hs))
        if st.orelse:
            outs[0] = self.block(st.orelse, outs[0]) if outs[0] is not None else None
        res = None
        for o in outs:
            res = self.join_states(res, o)
        return res

    def _std_exception(self, n):
        return isinstance(n, ast.Attribute) and isinstance(n.value, ast.Name) and \
            (n.value.id, n.attr) in (('struct', 'error'), ('subprocess', 'TimeoutExpired'),
                                     ('subprocess', 'CalledProcessError'), ('json', 'JSONDecodeError'),
                                     ('zlib', 'error'))

    def s_Break(self, st, state):
        if self.loops:
            self.loops[-1][0].append(state)
        return None

    def s_Continue(self, st, state):
        if self.loops:
            self.loops[-1][1].append(state)
        return None

    def s_While(self, st, state):
        if st.orelse:
            self.issue(st, 'syntax', 'while-else')

        def body(head):
            self.ev(st.test, head)
            h2 = self.after_access(st.test, head)
            return self.block(st.body, self.refine(st.test, h2, True))
        return self.loop(st, state, body, lambda head: self.refine(st.test, head, False),
                         bound=lambda head, init: self.while_bound(st, head, init))

    def s_For(self, st, state):
        if st.orelse:
            self.issue(st, 'syntax', 'for-else')
        it = self.ev(st.iter, state)
        elem, n = self.iteration(st.iter, it, state)
        state = self.after_access(st.iter, state)

        def body(head):
            h = self.assign(st.target, elem, dict(head), None)
            return self.block(st.body, h)
        return self.loop(st, state, body, lambda head: head, bound=lambda head, init: n)

    s_AsyncFor = s_For

    # ------------------------------------------------------------------------------------- loops
    def loop(self, st, init, body, exit_refine, bound):
        """Run a loop body to a fixpoint; the last pass (with final heads) records and contributes.
        Inside an enclosing loop's fixpoint (quiet), a loop met again with the same state at entry gives
        the same state at exit: that is remembered rather than run again."""
        memo_key = None
        if self.quiet:
            memo_key = (id(st), tuple(sorted((k, self.p.canon(v)) for k, v in init.items())))
            got = self.loop_memo.get(memo_key)
            if got is not None:
                return dict(got)
        head = dict(init)
        self.quiet += 1
        self.p.suspend += 1
        try:
            head = self.fixpoint(st, init, head, body, bound)
        finally:
            self.quiet -= 1
            self.p.suspend -= 1
        self.loops.append(([], []))
        body(head)                                           # the pass that counts
        brk, cont = self.loops.pop()
        out = exit_refine(head)
        for b in brk:
            out = self.join_states(out, b)
        if memo_key is not None:
            self.loop_memo[memo_key] = dict(out)
        return out

    def fixpoint(self, st, init, head, body, bound):
        widened = False
        for i in range(MAX_LOOP):
            self.loops.append(([], []))
            back = body(head)
            brk, cont = self.loops.pop()
            for c in cont:
                back = self.join_states(back, c)
            new = self.join_states(init, back) if back is not None else dict(init)
            new = self.merge_head(head, new)
            if i >= 1:
                new = self.widen_states(head, new)
                widened = True
            if self.same_state(new, head):
                break
            if i == MAX_LOOP - 1:                            # it does not settle: what still moves is unknown
                for k, v in new.items():
                    if k in head and self.p.canon(head[k]) != self.p.canon(v):
                        new[k] = P.INT() if v.kind == 'int' else P.top('a value a loop keeps changing')
            head = new
        if widened:                                          # narrowing: a decreasing pass
            self.loops.append(([], []))
            back = body(head)
            brk, cont = self.loops.pop()
            for c in cont:
                back = self.join_states(back, c)
            new = self.join_states(init, back) if back is not None else dict(init)
            new = self.merge_head(head, new)
            head = dict((k, self.narrow(head.get(k), v)) for k, v in new.items())
            head = self.counter_rule(st, head, init, bound)
        return head

    def merge_head(self, head, new):
        for k in head:
            if k not in new:
                new[k] = head[k]
        return new

    def narrow(self, old, new):
        if old is None or new is None or old.kind != 'int' or new.kind != 'int':
            return new
        lo = new.lo if old.lo is None else old.lo
        hi = new.hi if old.hi is None else old.hi
        if new.lo is not None and (old.lo is None):
            lo = new.lo
        if new.hi is not None and (old.hi is None):
            hi = new.hi
        return P.INT(lo, hi)

    def widen_states(self, old, new):
        out = dict(new)
        for k, v in new.items():
            o = old.get(k)
            if o is not None and o.kind == 'int' and v.kind == 'int' and o != v:
                out[k] = P.widen_int(o, v)
        return out

    def same_state(self, a, b):
        if a.keys() != b.keys():
            return False
        for k in a:
            if self.p.canon(a[k]) != self.p.canon(b[k]):
                return False
        return True

    def join_states(self, a, b):
        if a is None:
            return None if b is None else dict(b)
        if b is None:
            return dict(a)
        out = dict(a)
        for k, v in b.items():
            out[k] = self.p.join(out[k], v) if k in out else v
        return out

    def counter_rule(self, st, head, init, bound):
        """A variable left unbounded that the loop only increases by bounded steps, in a loop of bounded
        passes, is bounded by its start plus the steps times the passes."""
        loose = [k for k, v in head.items() if v.kind == 'int' and (v.hi is None or v.lo is None)]
        if not loose:
            return head
        n = bound(head, init)
        if n is None:
            return head
        head = dict(head)
        for k in loose:
            step = self.step_bound(st.body, k)
            v0 = init.get(k)
            if step is None or v0 is None or v0.kind != 'int':
                continue
            up, down = step
            lo = v0.lo if down == 0 else (None if v0.lo is None else v0.lo - down * n)
            hi = v0.hi if up == 0 else (None if v0.hi is None else v0.hi + up * n)
            cur = head[k]
            head[k] = P.INT(cur.lo if cur.lo is not None else lo, cur.hi if cur.hi is not None else hi)
        return head

    def step_bound(self, stmts, var):
        """(most it can increase, most it can decrease) in one pass over stmts -- None if `var` is set
        any other way or a step is not bounded."""
        up = down = 0
        for st in stmts:
            r = self._step(st, var)
            if r is None:
                return None
            up, down = up + r[0], down + r[1]
        return up, down

    def _step(self, st, var):
        if isinstance(st, ast.AugAssign) and isinstance(st.target, ast.Name) and st.target.id == var:
            t = self.seen.get(id(st.value))
            if t is None or t.kind != 'int' or t.lo is None or t.hi is None:
                return None
            if isinstance(st.op, ast.Add):
                return max(t.hi, 0), max(-t.lo, 0)
            if isinstance(st.op, ast.Sub):
                return max(-t.lo, 0), max(t.hi, 0)
            return None
        if isinstance(st, (ast.For, ast.While)):
            inner = self.step_bound(st.body, var)
            if inner is None:
                return None
            if inner == (0, 0):
                return 0, 0
            n = None
            if isinstance(st, ast.For):
                n = self.range_count(st.iter)
            if n is None:
                return None
            return inner[0] * n, inner[1] * n
        for n in ast.walk(st):
            if isinstance(n, ast.Name) and n.id == var and isinstance(n.ctx, ast.Store):
                return None
            if isinstance(n, ast.AugAssign) and isinstance(n.target, ast.Name) and n.target.id == var:
                break
        else:
            return 0, 0
        if isinstance(st, ast.If):
            a = self.step_bound(st.body, var)
            b = self.step_bound(st.orelse, var)
            if a is None or b is None:
                return None
            return max(a[0], b[0]), max(a[1], b[1])
        if isinstance(st, ast.Try) and not st.finalbody:
            parts = [self.step_bound(st.body, var)] + [self.step_bound(h.body, var) for h in st.handlers]
            if any(x is None for x in parts):
                return None
            return sum(x[0] for x in parts), sum(x[1] for x in parts)
        return None

    def range_count(self, it):
        """passes of `for x in <it>`, or None"""
        t = self.seen.get(id(it))
        if isinstance(it, ast.Call) and isinstance(it.func, ast.Name) and it.func.id == 'range':
            args = [self.seen.get(id(a)) for a in it.args]
            if any(a is None or a.kind != 'int' for a in args):
                return None
            if len(args) == 1:
                return None if args[0].hi is None else max(args[0].hi, 0)
            if len(args) >= 2:
                step = _int_const(it.args[2]) if len(it.args) == 3 else 1
                if step is None or step == 0:
                    return None
                a, b = args[0], args[1]
                if step > 0:
                    if b.hi is None or a.lo is None:
                        return None
                    return max((b.hi - a.lo + step - 1) // step, 0)
                if a.hi is None or b.lo is None:
                    return None
                return max((a.hi - b.lo - step - 1) // (-step), 0)
        if t is not None and t.kind in ('list', 'tuple', 'str', 'bytes', 'bytearray', 'dict', 'set'):
            return P.MEM
        if isinstance(it, ast.Call) and isinstance(it.func, (ast.Name, ast.Attribute)):
            name = it.func.id if isinstance(it.func, ast.Name) else it.func.attr
            if name in ('enumerate', 'zip', 'reversed', 'sorted', 'items', 'keys', 'values'):
                return P.MEM
        return None

    def while_bound(self, st, head, init):
        """passes of `while v < X` (v strictly increased by every pass), or None"""
        t = st.test
        if isinstance(t, ast.BoolOp) and isinstance(t.op, ast.And):
            for v in t.values:
                r = self._cmp_bound(v, st, head, init)
                if r is not None:
                    return r
            return None
        return self._cmp_bound(t, st, head, init)

    def _cmp_bound(self, t, st, head, init):
        if not (isinstance(t, ast.Compare) and len(t.ops) == 1 and isinstance(t.left, ast.Name)):
            return None
        var = t.left.id
        op = t.ops[0]
        if not isinstance(op, (ast.Lt, ast.LtE)):
            return None
        d = self.min_step(st.body, var)
        if not d:
            return None
        if d == float('inf'):
            return 1
        x = self.seen.get(id(t.comparators[0]))
        v0 = init.get(var)
        if x is None or x.kind != 'int' or x.hi is None or v0 is None or v0.kind != 'int' or v0.lo is None:
            return None
        return max((x.hi - v0.lo) // d + 2, 0)

    def min_step(self, stmts, var):
        """the least `var` grows along every path back to the loop's head (0: some path may not grow it;
        inf: no path loops back)"""
        r = self._paths(stmts, var)
        if r is None:
            return 0
        return min(r)

    def _paths(self, stmts, var):
        """(least growth of the paths falling off the end, least growth of the paths reaching a continue),
        inf where there are none; None when some path changes `var` other than by a non-negative step.
        Paths ending in break, return or raise do not loop back and are left out."""
        INF = float('inf')
        acc, cont = 0, INF
        for st in stmts:
            if isinstance(st, (ast.Break, ast.Return, ast.Raise)):
                return INF, cont
            if isinstance(st, ast.Continue):
                return INF, min(cont, acc)
            if isinstance(st, ast.AugAssign) and isinstance(st.target, ast.Name) and st.target.id == var:
                t = self.seen.get(id(st.value))
                if not isinstance(st.op, ast.Add) or t is None or t.kind != 'int' or t.lo is None or t.lo < 0:
                    return None
                acc += t.lo
            elif isinstance(st, ast.If):
                a = self._paths(st.body, var)
                b = self._paths(st.orelse, var)
                if a is None or b is None:
                    return None
                cont = min(cont, acc + a[1], acc + b[1])
                fall = min(a[0], b[0])
                if fall == INF:
                    return INF, cont
                acc += fall
            elif not self._grows_only(st, var):
                return None
        return acc, cont

    def _grows_only(self, st, var):
        """`st` changes `var` (if at all) only by non-negative steps (a nested loop, a try)"""
        aug = set(id(n.target) for n in ast.walk(st) if isinstance(n, ast.AugAssign))
        for n in ast.walk(st):
            if isinstance(n, ast.Name) and n.id == var and isinstance(n.ctx, ast.Store) and id(n) not in aug:
                return False
            if isinstance(n, ast.AugAssign) and isinstance(n.target, ast.Name) and n.target.id == var:
                t = self.seen.get(id(n.value))
                if not isinstance(n.op, ast.Add) or t is None or t.kind != 'int' or t.lo is None or t.lo < 0:
                    return False
            if isinstance(n, ast.Continue) and not isinstance(st, (ast.For, ast.While)):
                return False
        return True

    def iteration(self, node, t, state):
        """(element type, pass bound) of iterating a value"""
        k = t.kind
        if k == 'bottom':
            return P.BOTTOM, None
        if k == 'top':
            return t, None
        if k in ('list', 'set'):
            if k == 'set':
                self.issue(node, 'dynamic', 'set iteration order')
            return self.p.elem[self.p.find(t.site)], P.MEM
        if k == 'dict':
            s = self.p.find(t.site)
            self.p.make_dynamic(s, 'iterated')
            return self.p.keys[s], P.MEM
        if k == 'tuple':
            e = P.BOTTOM
            for a in t.args:
                e = self.p.join(e, a)
            if e.kind == 'top':
                return self.issue(node, 'conflict', 'iterating a tuple of mixed items'), None
            return e, len(t.args)
        if k == 'str':
            return P.STR, P.MEM
        if k in ('bytes', 'bytearray'):
            return P.u(8), P.MEM if t.hi is None else t.hi
        if k == 'extern' and t.name and t.name.startswith('iter:'):
            return self.p.iters.get(id(node), P.BOTTOM), P.MEM
        if k == 'iter':
            return t.args[0], t.lo
        return self.issue(node, 'conflict', 'iterating a %s' % k), None

    # ------------------------------------------------------------------------------------- assignment
    def assign(self, tgt, t, state, value_node, aug=False):
        if isinstance(tgt, ast.Name):
            state = dict(state)
            state[tgt.id] = t
            self.note(tgt, t, ('local',))
            if t.kind in ('list', 'dict', 'set'):
                self.p.hint(t.site, tgt.id)
            return state
        if isinstance(tgt, ast.Attribute):
            ot = self.ev(tgt.value, state)
            if ot.kind == 'inst':
                t = self.fit(tgt, t, 'attribute ' + tgt.attr)
                self.contribute_attr(ot.name, tgt.attr, t)
                self.note(tgt, t, ('field', ot.name, tgt.attr))
                if t.kind in ('list', 'dict', 'set'):
                    self.p.hint(t.site, tgt.attr)
            elif ot.kind in ('bottom',):
                pass
            elif ot.kind == 'top':
                self.note(tgt, ot)
            else:
                self.note(tgt, self.issue(tgt, 'dynamic', 'attribute store on a %s' % ot.kind))
            return state
        if isinstance(tgt, ast.Subscript):
            ot = self.ev(tgt.value, state)
            if isinstance(tgt.slice, ast.Slice):
                self.ev_slice_parts(tgt.slice, state)
                if ot.kind == 'list' and t.kind == 'list':
                    self.p.grow(self.p.elem, ot.site, self.p.elem[self.p.find(t.site)])
                elif ot.kind == 'bytearray' and t.kind in ('bytes', 'bytearray'):
                    pass
                elif ot.kind != 'bottom':
                    self.issue(tgt, 'syntax', 'slice assignment to a %s' % ot.kind)
                self.note(tgt, t, ('slice_store', ot.kind))
                return state
            kt = self.ev(tgt.slice, state)
            if ot.kind == 'list':
                self.p.grow(self.p.elem, ot.site, t)
                self.need_int_index(tgt.slice, kt)
                self.note(tgt, t, ('index_store', 'list'))
            elif ot.kind == 'bytearray':
                if t.kind != 'int' and t.kind != 'bottom':
                    self.issue(tgt, 'conflict', 'bytearray item of %s' % t.kind)
                self.note(tgt, P.u(8), ('index_store', 'bytearray'))
            elif ot.kind == 'dict':
                key = _str_const(tgt.slice)
                s = self.p.find(ot.site)
                t = self.fit(tgt, t, 'field %s' % key)
                if key is not None:
                    self.p.grow_field(s, key, t)
                    if (aug or id(tgt) not in self.p.builder_targets) and self.live:
                        self.p.mark_mutated(s)
                else:
                    self.p.make_dynamic(s, 'a key that is not a constant string')
                    self.p.grow(self.p.keys, s, kt)
                    self.p.grow(self.p.elem, s, t)
                self.note(tgt, t, ('dict_store', key))
            elif ot.kind in ('bottom', 'top'):
                pass
            else:
                self.issue(tgt, 'conflict', 'item store on a %s' % ot.kind)
            return state
        if isinstance(tgt, (ast.Tuple, ast.List)):
            if any(isinstance(e, ast.Starred) for e in tgt.elts):
                self.issue(tgt, 'syntax', 'starred unpacking')
                return state
            if t.kind == 'tuple':
                if len(t.args) != len(tgt.elts):
                    self.issue(tgt, 'conflict', 'unpacking %d items into %d' % (len(t.args), len(tgt.elts)))
                    return state
                for e, et in zip(tgt.elts, t.args):
                    state = self.assign(e, et, state, None)
                self.note(tgt, t, ('unpack',))
                return state
            if t.kind in ('bottom',):
                for e in tgt.elts:
                    state = self.assign(e, P.BOTTOM, state, None)
                return state
            if t.kind == 'top':
                for e in tgt.elts:
                    state = self.assign(e, t, state, None)
                return state
            self.issue(tgt, 'conflict', 'unpacking a %s (its length is not known to the type)' % t.kind)
            for e in tgt.elts:
                state = self.assign(e, P.top('unpack'), state, None)
            return state
        self.issue(tgt, 'syntax', 'assignment to %s' % type(tgt).__name__)
        return state

    def builder_keys(self, st, t):
        """A record made empty (or partly) and filled by the statements right after it: the keys those
        statements set are set by every creation. (Called with the Assign; scans its block.)"""
        if t.kind != 'dict':
            return
        name = st.targets[0].id
        block = self.p.block_of(st)
        if block is None:
            return
        keys = set(self.p.literal_keys.get(id(st.value), ()))
        i = block.index(st) + 1
        while i < len(block):
            s2 = block[i]
            if isinstance(s2, ast.Assign) and len(s2.targets) == 1 and isinstance(s2.targets[0], ast.Subscript) \
                    and isinstance(s2.targets[0].value, ast.Name) and s2.targets[0].value.id == name \
                    and _str_const(s2.targets[0].slice) is not None \
                    and self._reads_fields_only(s2.value, name):
                keys.add(_str_const(s2.targets[0].slice))
                self.p.builder_targets.add(id(s2.targets[0]))
            elif any(isinstance(n, ast.Name) and n.id == name for n in ast.walk(s2)):
                break
            i += 1
        if self.live:
            self.p.created(t.site, keys)

    def _reads_fields_only(self, value, name):
        """`value` uses the record `name` only to read its fields by constant key"""
        ok = set()
        for n in ast.walk(value):
            if isinstance(n, ast.Subscript) and isinstance(n.value, ast.Name) and n.value.id == name \
                    and _str_const(n.slice) is not None and isinstance(n.ctx, ast.Load):
                ok.add(id(n.value))
        return all(id(n) in ok for n in ast.walk(value) if isinstance(n, ast.Name) and n.id == name)

    # ------------------------------------------------------------------------------------- refinement
    def refine(self, test, state, truth):
        """The state after `test` came out `truth`."""
        if state is None:
            return None
        if isinstance(test, ast.UnaryOp) and isinstance(test.op, ast.Not):
            return self.refine(test.operand, state, not truth)
        if isinstance(test, ast.BoolOp):
            if isinstance(test.op, ast.And) == truth:          # and/True, or/False: every part holds
                for v in test.values:
                    state = self.refine(v, state, truth)
                return state
            outs = None                                        # one of them failed: join each case
            s = state
            for v in test.values:
                outs = self.join_states(outs, self.refine(v, s, truth))
                s = self.refine(v, s, not truth)
            return outs
        if isinstance(test, ast.Name) and test.id in state:
            t = state[test.id]
            if t.kind == 'opt' and truth:
                state = dict(state)
                state[test.id] = t.args[0]
            elif t.kind == 'opt' and not truth and t.args[0].kind == 'inst':
                state = dict(state)
                state[test.id] = P.NONE
            elif t.kind == 'int':
                state = dict(state)
                if not truth:
                    state[test.id] = P.meet(t, 0, 0)
            return state
        if isinstance(test, ast.Call) and isinstance(test.func, ast.Name) and test.func.id == 'isinstance' \
                and len(test.args) == 2 and isinstance(test.args[0], ast.Name):
            return state
        if isinstance(test, ast.Compare) and len(test.ops) >= 1:
            left = test.left
            for op, right in zip(test.ops, test.comparators):
                state = self.refine_cmp(left, op, right, state, truth if len(test.ops) == 1 else truth)
                if not truth and len(test.ops) > 1:
                    return state if len(test.ops) == 1 else state
                left = right
            return state
        return state

    def refine_cmp(self, left, op, right, state, truth):
        if isinstance(op, (ast.Is, ast.IsNot)) and isinstance(right, ast.Constant) and right.value is None \
                and isinstance(left, ast.Name) and left.id in state:
            is_none = isinstance(op, ast.Is) == truth
            t = state[left.id]
            state = dict(state)
            state[left.id] = P.NONE if is_none else P.strip_opt(t)
            return state
        if isinstance(op, (ast.Eq, ast.NotEq)) and isinstance(right, ast.Constant) and right.value is None \
                and isinstance(left, ast.Name) and left.id in state:
            return self.refine_cmp(left, ast.Is() if isinstance(op, ast.Eq) else ast.IsNot(), right, state, truth)
        ops = {ast.Lt: '<', ast.LtE: '<=', ast.Gt: '>', ast.GtE: '>=', ast.Eq: '==', ast.NotEq: '!='}
        o = ops.get(type(op))
        if o is None:
            return state
        if not truth:
            o = {'<': '>=', '<=': '>', '>': '<=', '>=': '<', '==': '!=', '!=': '=='}[o]
        lt = self.seen.get(id(left))
        rt = self.seen.get(id(right))
        if isinstance(left, ast.Name) and left.id in state and state[left.id].kind == 'int' and rt is not None and rt.kind == 'int':
            state = dict(state)
            state[left.id] = self._bound(state[left.id], o, rt)
        if isinstance(right, ast.Name) and right.id in state and state[right.id].kind == 'int' and lt is not None and lt.kind == 'int':
            flip = {'<': '>', '<=': '>=', '>': '<', '>=': '<=', '==': '==', '!=': '!='}[o]
            state = dict(state)
            state[right.id] = self._bound(state[right.id], flip, lt)
        return state

    def _bound(self, v, o, other):
        if o == '<':
            return P.meet(v, hi=None if other.hi is None else other.hi - 1)
        if o == '<=':
            return P.meet(v, hi=other.hi)
        if o == '>':
            return P.meet(v, lo=None if other.lo is None else other.lo + 1)
        if o == '>=':
            return P.meet(v, lo=other.lo)
        if o == '==':
            return P.meet(v, other.lo, other.hi)
        return v

    def after_access(self, node, state):
        """A successful index bounds the variable that indexed: data[p] returned, so p is inside data."""
        if state is None:
            return None
        for n in self.p.access_nodes(node):
            if isinstance(n, ast.Subscript) and isinstance(n.slice, ast.Name) and n.slice.id in state:
                st = self.seen.get(id(n.value))
                if st is not None and st.kind in ('bytes', 'bytearray', 'list', 'str', 'tuple'):
                    v = state[n.slice.id]
                    if v.kind == 'int':
                        top = P.MEM if st.kind not in ('bytes', 'bytearray') or st.hi is None else st.hi
                        state = dict(state)
                        state[n.slice.id] = P.meet(v, -top, top - 1)
            elif isinstance(n, ast.Call):
                fq = self.res_fn(n)
                if fq is not None:
                    idx = self.p.index_params.get(fq, ())
                    f = self.p.fns[fq]
                    names = f.bound
                    for a, pname in zip(n.args, names):
                        if pname in idx and isinstance(a, ast.Name) and a.id in state and state[a.id].kind == 'int':
                            state = dict(state)
                            state[a.id] = P.meet(state[a.id], -P.MEM, P.MEM)
                elif isinstance(n.func, ast.Attribute) and n.func.attr in ('unpack_from', 'pack_into') \
                        and len(n.args) >= 3 and isinstance(n.args[2], ast.Name) and n.args[2].id in state \
                        and state[n.args[2].id].kind == 'int':
                    state = dict(state)
                    state[n.args[2].id] = P.meet(state[n.args[2].id], -P.MEM, P.MEM)
        return state

    def res_fn(self, call):
        r = self.res.get(id(call)) if self.record else None
        if r is None:
            r = self._res_quiet.get(id(call)) if hasattr(self, '_res_quiet') else None
        if r and r[0] in ('fn', 'method'):
            return r[1]
        return None

    # ------------------------------------------------------------------------------------- expressions
    def ev(self, node, state, aug=False):
        m = getattr(self, 'e_' + type(node).__name__, None)
        if m is None:
            t = self.issue(node, 'syntax', type(node).__name__.lower())
            return self.note(node, t)
        r = m(node, state)
        if isinstance(r, tuple):
            t, res = r
        else:
            t, res = r, None
        if res is not None and isinstance(node, ast.Call) and res[0] in ('fn', 'method'):
            if not hasattr(self, '_res_quiet'):
                self._res_quiet = {}
            self._res_quiet[id(node)] = res
        return self.note(node, t, res)

    def e_Constant(self, n, state):
        v = n.value
        if v is None:
            return P.NONE
        if isinstance(v, bool):
            return P.BOOL
        if isinstance(v, int):
            return P.const_int(v)
        if isinstance(v, float):
            return P.FLOAT
        if isinstance(v, str):
            return P.STR
        if isinstance(v, bytes):
            return P.bytes_of(len(v))
        return self.issue(n, 'syntax', 'constant %s' % type(v).__name__)

    def e_JoinedStr(self, n, state):
        for v in n.values:
            if isinstance(v, ast.FormattedValue):
                t = self.ev(v.value, state)
                if v.format_spec is not None:
                    spec = ''.join(x.value for x in v.format_spec.values if isinstance(x, ast.Constant)) \
                        if all(isinstance(x, ast.Constant) for x in v.format_spec.values) else None
                    if spec is None:
                        self.issue(v, 'dynamic', 'computed format spec')
                    else:
                        self.check_format(v, t, spec)
                elif v.conversion not in (-1, 114, 115):
                    self.issue(v, 'syntax', 'f-string conversion !%s' % chr(v.conversion))
                else:
                    self.check_str(v, t, v.conversion == 114)
        return P.STR

    def check_str(self, node, t, as_repr):
        k = t.kind
        if k in ('int', 'str', 'bool', 'none', 'bytes', 'bytearray', 'bottom', 'top'):
            return
        if k == 'float' and not as_repr:
            return
        if k == 'float':
            return
        if k == 'opt' and t.args[0].kind in ('int', 'str', 'bool'):
            return
        if k in ('tuple', 'list') and as_repr:
            return
        self.issue(node, 'syntax', 'str() of a %s' % k)

    def check_format(self, node, t, spec):
        import re
        if not re.fullmatch(r'(?:.?[<>^])?[+\- ]?#?0?\d*,?(?:\.\d+)?[bdoxXcsfeEg%]?', spec):
            self.issue(node, 'syntax', 'format spec %r' % spec)
            return
        if t.kind in ('int', 'bool') and spec[-1:] in 'fFeEg%':
            return
        if t.kind not in ('int', 'str', 'float', 'bool', 'bottom', 'top'):
            self.issue(node, 'syntax', 'format of a %s' % t.kind)

    def e_FormattedValue(self, n, state):
        return self.ev(n.value, state)

    def e_Name(self, n, state):
        name = n.id
        if name in state:
            return state[name], ('local',)
        if name in self.locals:
            return P.BOTTOM, ('local',)                      # read before any assignment reaches here
        r = self.lookup(name)
        if r is None:
            return self.issue(n, 'unresolved', name)
        return self.ref_type(n, r), r

    def lookup(self, name):
        if name in self.local_imports:
            return self.local_imports[name]
        if self.m is not None:
            r = self.p.resolve_global(self.m, name)
            if r is not None:
                return r
        if name in BUILTIN_FUNCS:
            return ('builtin', name)
        if name in EXCEPTIONS:
            return ('exc', name)
        if name in ('True', 'False', 'None', '__name__', '__file__', '__doc__'):
            return ('const', name)
        return None

    def lookup_class(self, n):
        if isinstance(n, ast.Name):
            r = self.lookup(n.id)
            if r and r[0] == 'class':
                return r[1]
        return None

    def ref_type(self, n, r):
        k = r[0]
        if k == 'global':
            return self.p.global_type(r[1], r[2])
        if k == 'fn':
            return P.T('fn', name=r[1])
        if k == 'class':
            return P.T('cls', name=r[1])
        if k == 'module':
            return P.T('module', name=r[1])
        if k == 'stdmod':
            return P.T('module', name='std:' + r[1])
        if k == 'stdobj':
            return P.T('extern', name='std:' + r[1])
        if k == 'builtin':
            return P.T('fn', name='builtin:' + r[1])
        if k == 'exc':
            return P.T('cls', name='exc:' + r[1])
        if k == 'const':
            if r[1] == '__doc__':
                return P.STR if ast.get_docstring(self.m.tree) is not None else P.NONE
            if r[1] == '__file__' or r[1] == '__name__':
                return self.issue(n, 'dynamic', r[1])
            return P.BOTTOM
        if k == 'extern':
            return self.issue(n, 'extern', r[1].split('.')[0])
        if k == 'missing':
            return self.issue(n, 'unresolved', r[1])
        return self.issue(n, 'unresolved', str(r))

    def e_Attribute(self, n, state):
        ot = self.ev(n.value, state)
        k = ot.kind
        if k in ('bottom', 'top'):
            return ot
        if k == 'inst':
            cq = ot.name
            f = self.p.find_method(cq, n.attr)
            if f is not None:
                if f.kind == 'property':
                    return self.p.ret(f.qname), ('property', f.qname)
                return P.T('fn', name='bound:' + f.qname), ('bound', f.qname)
            t = self.p.attr_type(cq, n.attr)
            if t is None:
                return self.issue(n, 'unresolved', '%s.%s' % (cq.rsplit('.', 1)[-1], n.attr))
            return t, ('field', self.p.attr_owner(cq, n.attr) or cq, n.attr)
        if k == 'module':
            if ot.name.startswith('std:'):
                full = ot.name[4:] + '.' + n.attr
                t = STD_ATTRS.get(full)
                if t is not None:
                    return t, ('stdattr', full)
                return P.T('extern', name='std:' + full), ('stdattr', full)
            mod = self.p.modules[ot.name]
            r = self.p.resolve_global(mod, n.attr)
            if r is None:
                full = ot.name + '.' + n.attr
                if full in self.p.modules:
                    return P.T('module', name=full), ('module', full)
                return self.issue(n, 'unresolved', full)
            return self.ref_type(n, r), r
        if k == 'cls':
            if ot.name.startswith('exc:'):
                return self.issue(n, 'dynamic', 'exception class attribute')
            c = self.p.classes[ot.name]
            f = self.p.find_method(ot.name, n.attr)
            if f is not None:
                if f.kind == 'static':
                    return P.T('fn', name=f.qname), ('fn', f.qname)
                return self.issue(n, 'dynamic', 'unbound method')
            for cq in self.p.mro(ot.name):
                if n.attr in self.p.classes[cq].class_attrs:
                    return self.p.class_attr_type(cq, n.attr), ('classattr', cq, n.attr)
            return self.issue(n, 'unresolved', '%s.%s' % (c.name, n.attr))
        if k == 'extern':
            if ot.name.startswith('std:'):
                full = ot.name[4:] + '.' + n.attr
                t = STD_ATTRS.get(full)
                if t is not None:
                    return t, ('stdattr', full)
                return P.T('extern', name='std:' + full), ('stdattr', full)
            return ot
        if k in ('str', 'bytes', 'bytearray', 'list', 'dict', 'set', 'int', 'float', 'tuple'):
            return P.T('fn', name='bmethod:%s.%s' % (k, n.attr)), ('bmethod', k + '.' + n.attr)
        if k == 'opt':
            return self.issue(n, 'conflict', 'attribute .%s of a value that may be None' % n.attr)
        return self.issue(n, 'conflict', 'attribute .%s of a %s' % (n.attr, k))

    def e_Subscript(self, n, state):
        ot = self.ev(n.value, state)
        k = ot.kind
        if isinstance(n.slice, ast.Slice):
            lo, hi, step = self.ev_slice_parts(n.slice, state)
            for part, t in ((n.slice.lower, lo), (n.slice.upper, hi), (n.slice.step, step)):
                if part is not None and t.kind not in ('int', 'bottom', 'top', 'none'):
                    self.issue(part, 'conflict', 'slice bound of %s' % t.kind)
            if n.slice.step is not None:
                sv = _int_const(n.slice.step)
                if sv is None or sv == 0:
                    self.issue(n, 'syntax', 'slice with a computed step')
            if k in ('bytes', 'bytearray'):
                cap = self.slice_cap(n.slice, ot.hi)
                return P.T(k, hi=cap), ('slice', k)
            if k in ('str',):
                return P.STR, ('slice', k)
            if k == 'list':
                s = self.p.new_site(n, 'list', self.where(n))
                self.p.grow(self.p.elem, s, self.p.elem[self.p.find(ot.site)])
                return P.T('list', site=s), ('slice', 'list')
            if k == 'tuple':
                a, b = _int_const(n.slice.lower), _int_const(n.slice.upper)
                if n.slice.step is None and (n.slice.lower is None or a is not None) and (n.slice.upper is None or b is not None):
                    return P.tuple_of(ot.args[slice(a, b)]), ('tuple_slice', a, b)
                return self.issue(n, 'conflict', 'computed slice of a tuple')
            if k in ('bottom', 'top'):
                return ot
            return self.issue(n, 'conflict', 'slice of a %s' % k)
        it = self.ev(n.slice, state)
        if k in ('bytes', 'bytearray'):
            self.need_int_index(n.slice, it)
            return P.u(8), ('index', k)
        if k == 'str':
            self.need_int_index(n.slice, it)
            return P.STR, ('index', k)
        if k == 'list':
            self.need_int_index(n.slice, it)
            return self.p.elem[self.p.find(ot.site)], ('index', k)
        if k == 'tuple':
            i = _int_const(n.slice)
            if i is None:
                e = P.BOTTOM
                for a in ot.args:
                    e = self.p.join(e, a)
                if e.kind == 'top':
                    return self.issue(n, 'conflict', 'computed index into a tuple of mixed items')
                self.need_int_index(n.slice, it)
                return e, ('tuple_index', None)
            if not -len(ot.args) <= i < len(ot.args):
                return self.issue(n, 'conflict', 'tuple index %d out of range' % i)
            return ot.args[i], ('tuple_index', i)
        if k == 'dict':
            s = self.p.find(ot.site)
            key = _str_const(n.slice)
            if key is not None and s not in self.p.dynamic:
                return self.p.fields[s].get(key, P.BOTTOM), ('field_get', key)
            if key is None:
                self.p.make_dynamic(s, 'a key that is not a constant string')
            self.p.grow(self.p.keys, s, it)
            return self.p.elem[s], ('dict_get', None)
        if k in ('bottom', 'top'):
            return ot
        if k == 'extern' and ot.name == 'std:os.environ':
            return self.issue(n, 'stdlib', 'os.environ')
        if k == 'opt':
            return self.issue(n, 'conflict', 'subscript of a value that may be None')
        return self.issue(n, 'conflict', 'subscript of a %s' % k)

    def slice_cap(self, sl, cap):
        """the most items a slice can hold: d[a:a+4] four, d[:8] eight, d[2:6] four"""
        lo, hi = sl.lower, sl.upper
        if sl.step is not None:
            return cap
        if hi is not None and _int_const(hi) is not None and _int_const(hi) >= 0:
            if lo is None:
                return _int_const(hi) if cap is None else min(cap, _int_const(hi))
            if _int_const(lo) is not None and _int_const(lo) >= 0:
                return max(_int_const(hi) - _int_const(lo), 0)
        if isinstance(hi, ast.BinOp) and isinstance(hi.op, ast.Add) and lo is not None \
                and ast.dump(hi.left) == ast.dump(lo) and _int_const(hi.right) is not None and _int_const(hi.right) >= 0:
            return _int_const(hi.right)
        return cap

    def ev_slice_parts(self, sl, state):
        out = []
        for part in (sl.lower, sl.upper, sl.step):
            out.append(self.ev(part, state) if part is not None else P.NONE)
        return out

    def need_int_index(self, node, t):
        if t.kind not in ('int', 'bottom', 'top', 'bool'):
            self.issue(node, 'conflict', 'index of %s' % t.kind)

    def where(self, n):
        return (self.m.name if self.m else '?', getattr(n, 'lineno', 0))

    # -- operators
    def e_BinOp(self, n, state):
        a = self.ev(n.left, state)
        b = self.ev(n.right, state)
        return self.binop(n, n.op, a, b, n.left, n.right)

    def binop(self, n, op, a, b, ln, rn):
        if a.kind == 'bottom' or b.kind == 'bottom':
            return P.BOTTOM
        if a.kind == 'top':
            return a
        if b.kind == 'top':
            return b
        ka, kb = a.kind, b.kind
        if ka == 'bool':
            ka, a = 'int', P.INT(0, 1)
        if kb == 'bool':
            kb, b = 'int', P.INT(0, 1)
        if ka == 'int' and kb == 'int':
            if isinstance(op, ast.Add):
                return P.iadd(a, b)
            if isinstance(op, ast.Sub):
                return P.isub(a, b)
            if isinstance(op, ast.Mult):
                return P.imul(a, b)
            if isinstance(op, ast.FloorDiv):
                return P.ifloordiv(a, b)
            if isinstance(op, ast.Mod):
                return P.imod(a, b)
            if isinstance(op, ast.LShift):
                return P.ishl(a, b)
            if isinstance(op, ast.RShift):
                return P.ishr(a, b)
            if isinstance(op, ast.BitAnd):
                return P.iand(a, b)
            if isinstance(op, ast.BitOr):
                return P.ior(a, b)
            if isinstance(op, ast.BitXor):
                return P.ixor(a, b)
            if isinstance(op, ast.Pow):
                if b.lo is not None and b.lo >= 0:
                    return P.ipow(a, b)
                return self.issue(n, 'conflict', 'int ** a possibly negative int (a float)')
            if isinstance(op, ast.Div):
                self.exact_float(n, a)
                self.exact_float(n, b)
                return P.FLOAT
        if ka in ('int', 'float') and kb in ('int', 'float'):
            if ka == 'int':
                self.exact_float(n, a)
            if kb == 'int':
                self.exact_float(n, b)
            if isinstance(op, (ast.Add, ast.Sub, ast.Mult, ast.Div, ast.FloorDiv, ast.Mod, ast.Pow)):
                return P.FLOAT
        if ka == kb and ka in ('str', 'bytes', 'bytearray') and isinstance(op, ast.Add):
            if ka == 'str':
                return P.STR
            return P.T(ka, hi=None if a.hi is None or b.hi is None else a.hi + b.hi)
        if ka == 'bytearray' and kb == 'bytes' and isinstance(op, ast.Add):
            return P.T('bytearray')
        if ka == 'bytes' and kb == 'bytearray' and isinstance(op, ast.Add):
            return P.T('bytes')
        if ka in ('str', 'bytes') and kb == 'int' and isinstance(op, ast.Mult):
            return P.T(ka) if ka == 'bytes' else P.STR
        if ka == 'str' and isinstance(op, ast.Mod):
            self.check_percent(n, ln, rn, b)
            return P.STR
        if ka == 'bytes' and isinstance(op, ast.Mod):
            return self.issue(n, 'syntax', 'bytes %-formatting')
        if ka == 'list' and kb == 'list' and isinstance(op, ast.Add):
            s = self.p.new_site(n, 'list', self.where(n))
            self.p.grow(self.p.elem, s, self.p.elem[self.p.find(a.site)])
            self.p.grow(self.p.elem, s, self.p.elem[self.p.find(b.site)])
            return P.T('list', site=s)
        if ka == 'list' and kb == 'int' and isinstance(op, ast.Mult):
            s = self.p.new_site(n, 'list', self.where(n))
            self.p.grow(self.p.elem, s, self.p.elem[self.p.find(a.site)])
            return P.T('list', site=s)
        if ka == 'tuple' and kb == 'tuple' and isinstance(op, ast.Add):
            return P.tuple_of(a.args + b.args)
        if ka == 'set' and kb == 'set' and isinstance(op, (ast.BitOr, ast.BitAnd, ast.Sub, ast.BitXor)):
            s = self.p.new_site(n, 'set', self.where(n))
            self.p.grow(self.p.elem, s, self.p.elem[self.p.find(a.site)])
            self.p.grow(self.p.elem, s, self.p.elem[self.p.find(b.site)])
            return P.T('set', site=s)
        return self.issue(n, 'conflict', '%s %s %s' % (ka, type(op).__name__, kb))

    def exact_float(self, n, t):
        """an int meeting a float: exact in a double only within 2**53"""
        if t.kind == 'int' and not (t.lo is not None and t.hi is not None and -2 ** 53 <= t.lo and t.hi <= 2 ** 53):
            self.issue(n, 'int-range', 'an int beyond 2**53 meets a float')

    def check_percent(self, n, ln, rn, b):
        fmt = _str_const(ln)
        if fmt is None:
            self.issue(n, 'dynamic', '% with a computed format')
            return
        import re
        specs = re.findall(r'%([-+ #0]*)(\*|\d+)?(?:\.(\*|\d+))?([a-zA-Z%])', fmt)
        for flags, width, prec, conv in specs:
            if conv not in 'dixXosrcfeEgG%' or width == '*' or prec == '*':
                self.issue(n, 'syntax', 'percent conversion %%%s' % conv)
        if '%(' in fmt:
            self.issue(n, 'syntax', 'percent with a mapping')
        nvals = len([s for s in specs if s[3] != '%'])
        items = b.args if b.kind == 'tuple' else [b]
        if b.kind == 'tuple' and len(items) != nvals:
            self.issue(n, 'conflict', '%% with %d values for %d conversions' % (len(items), nvals))
        for t in items:
            if t.kind in ('list', 'dict', 'set', 'inst', 'opt') and not (t.kind == 'opt' and t.args[0].kind in ('int', 'str')):
                self.issue(n, 'syntax', '%% of a %s' % t.kind)

    def e_UnaryOp(self, n, state):
        t = self.ev(n.operand, state)
        if isinstance(n.op, ast.Not):
            return P.BOOL
        if t.kind in ('bottom', 'top'):
            return t
        if t.kind == 'bool':
            t = P.INT(0, 1)
        if t.kind == 'int':
            if isinstance(n.op, ast.USub):
                return P.ineg(t)
            if isinstance(n.op, ast.UAdd):
                return t
            if isinstance(n.op, ast.Invert):
                return P.iinvert(t)
        if t.kind == 'float' and isinstance(n.op, (ast.USub, ast.UAdd)):
            return P.FLOAT
        return self.issue(n, 'conflict', '%s of %s' % (type(n.op).__name__, t.kind))

    def e_BoolOp(self, n, state):
        ts = []
        s = state
        for v in n.values:
            t = self.ev(v, s)
            ts.append(t)
            s = self.refine(v, s, isinstance(n.op, ast.And))
        # the value is one of the operands (Python): in a test only truth matters, so bool;
        # as a value, all operands must be of one type
        j = P.BOTTOM
        for t in ts:
            j = self.p.join(j, t)
        if all(t.kind in ('bool', 'bottom') for t in ts):
            return P.BOOL
        if isinstance(n.op, ast.Or) and len(ts) == 2 and ts[0].kind == 'opt':
            j2 = self.p.join(P.strip_opt(ts[0]), ts[1])          # `x or default`: None never comes out
            if j2.kind != 'top':
                return j2
        if j.kind == 'top':
            return P.BOOL, ('mixedbool', j.why)
        return j

    def e_Compare(self, n, state):
        left = self.ev(n.left, state)
        for op, c in zip(n.ops, n.comparators):
            right = self.ev(c, state)
            self.check_compare(n, op, left, right)
            left = right
        return P.BOOL

    def check_compare(self, n, op, a, b):
        if a.kind in ('bottom', 'top') or b.kind in ('bottom', 'top'):
            return
        if isinstance(op, (ast.Is, ast.IsNot)):
            if not (b.kind == 'none' or a.kind == 'none'):
                self.issue(n, 'dynamic', 'identity comparison of %s' % a.kind)
            return
        if isinstance(op, (ast.In, ast.NotIn)):
            if b.kind in ('list', 'set'):
                e = self.p.elem[self.p.find(b.site)]
                self.comparable(n, a, e)
            elif b.kind == 'dict':
                s = self.p.find(b.site)
                if not (_str_const(n.left) is not None and s not in self.p.dynamic):
                    if _str_const(n.left) is None:
                        self.p.make_dynamic(s, 'membership of a computed key')
            elif b.kind == 'tuple':
                for x in b.args:
                    self.comparable(n, a, x)
            elif b.kind in ('str', 'bytes', 'bytearray'):
                if not (a.kind == b.kind or (a.kind == 'int' and b.kind != 'str') or {a.kind, b.kind} <= {'bytes', 'bytearray'}):
                    self.issue(n, 'conflict', '%s in %s' % (a.kind, b.kind))
            else:
                self.issue(n, 'conflict', 'membership in a %s' % b.kind)
            return
        if isinstance(op, (ast.Eq, ast.NotEq)):
            self.comparable(n, a, b, eq=True)
            return
        self.comparable(n, a, b, order=True)

    def comparable(self, n, a, b, eq=False, order=False):
        ka, kb = a.kind, b.kind
        if 'bottom' in (ka, kb) or 'top' in (ka, kb):
            return
        if ka == 'bool':
            ka = 'int'
        if kb == 'bool':
            kb = 'int'
        if ka == kb and ka in ('int', 'str', 'float', 'bytes', 'bytearray', 'none'):
            return
        if {ka, kb} == {'bytes', 'bytearray'}:
            return
        if {ka, kb} == {'int', 'float'}:
            self.exact_float(n, a if ka == 'int' else b)
            return
        if ka == 'tuple' and kb == 'tuple' and len(a.args) == len(b.args):
            for x, y in zip(a.args, b.args):
                self.comparable(n, x, y, eq, order)
            return
        if eq and (ka == 'opt' or kb == 'opt'):
            ia, ib = P.strip_opt(a), P.strip_opt(b)
            if ib.kind == 'none' or ia.kind == 'none':
                return
            self.comparable(n, ia, ib, eq, order)
            return
        if eq and ka == kb == 'list':
            self.comparable(n, self.p.elem[self.p.find(a.site)], self.p.elem[self.p.find(b.site)], eq)
            return
        if eq and ka == kb == 'inst':
            self.issue(n, 'dynamic', 'equality of instances')
            return
        if eq:
            self.issue(n, 'conflict', 'comparing %s with %s (always unequal in Python)' % (ka, kb))
            return
        self.issue(n, 'conflict', 'ordering %s against %s' % (ka, kb))

    def e_IfExp(self, n, state):
        self.ev(n.test, state)
        a = self.ev(n.body, self.refine(n.test, state, True))
        b = self.ev(n.orelse, self.refine(n.test, state, False))
        j = self.p.join(a, b)
        if j.kind == 'top':
            return self.issue(n, 'conflict', j.why)
        return j

    def e_NamedExpr(self, n, state):
        self.ev(n.value, state)
        return self.issue(n, 'syntax', 'walrus')

    def e_Lambda(self, n, state):
        return self.issue(n, 'dynamic', 'first-class lambda')

    def e_Starred(self, n, state):
        self.ev(n.value, state)
        return self.issue(n, 'syntax', 'starred')

    def e_Await(self, n, state):
        return self.issue(n, 'dynamic', 'await')

    def e_Yield(self, n, state):
        if n.value is not None:
            self.ev(n.value, state)
        return P.BOTTOM

    e_YieldFrom = e_Yield

    # -- displays
    def e_Tuple(self, n, state):
        ts = [self.ev(e, state) for e in n.elts]
        if any(isinstance(e, ast.Starred) for e in n.elts):
            return self.issue(n, 'syntax', 'starred in a tuple')
        return P.tuple_of(ts)

    def e_List(self, n, state):
        s = self.p.new_site(n, 'list', self.where(n))
        for e in n.elts:
            t = self.ev(e, state)
            self.p.grow(self.p.elem, s, t)
        return P.T('list', site=s)

    def e_Set(self, n, state):
        s = self.p.new_site(n, 'set', self.where(n))
        for e in n.elts:
            self.p.grow(self.p.elem, s, self.ev(e, state))
        return P.T('set', site=s)

    def e_Dict(self, n, state):
        s = self.p.new_site(n, 'dict', self.where(n))
        keys = []
        for k, v in zip(n.keys, n.values):
            vt = self.ev(v, state)
            if k is None:                                     # {**other}
                self.spread(s, vt, n)
                continue
            kt = self.ev(k, state)
            key = _str_const(k)
            if key is not None:
                self.p.grow_field(s, key, self.fit(v, vt, 'field ' + key))
                keys.append(key)
            else:
                self.p.make_dynamic(s, 'a key that is not a constant string')
                self.p.grow(self.p.keys, s, kt)
                self.p.grow(self.p.elem, s, vt)
        if self.live:
            self.p.literal_keys[id(n)] = keys + list(self.p.spread_keys.get(s, ()))
            if self.builder != id(n):
                self.p.created(s, self.p.literal_keys[id(n)])
        return P.T('dict', site=s)

    def spread(self, s, vt, n):
        if vt.kind != 'dict':
            if vt.kind not in ('bottom', 'top'):
                self.issue(n, 'conflict', '** of a %s' % vt.kind)
            return
        src = self.p.find(vt.site)
        if src in self.p.dynamic:
            self.p.make_dynamic(s, 'filled from a dict')
            self.p.grow(self.p.keys, s, self.p.keys[src])
            self.p.grow(self.p.elem, s, self.p.elem[src])
            return
        for key, t in list(self.p.fields[src].items()):
            self.p.grow_field(s, key, t)
        req = self.p.required[src] or frozenset()
        self.p.spread_keys[s] = req
        self.p.spread_from.setdefault(s, set()).add(src)

    def comp_scope(self, gens, state):
        st = dict(state)
        for g in gens:
            if g.is_async:
                self.issue(g, 'dynamic', 'async comprehension')
            it = self.ev(g.iter, st)
            elem, _n = self.iteration(g.iter, it, st)
            st = self.assign(g.target, elem, st, None)
            for cond in g.ifs:
                self.ev(cond, st)
                st = self.refine(cond, st, True)
        return st

    def e_ListComp(self, n, state):
        st = self.comp_scope(n.generators, state)
        s = self.p.new_site(n, 'list', self.where(n))
        self.p.grow(self.p.elem, s, self.ev(n.elt, st))
        return P.T('list', site=s)

    def e_SetComp(self, n, state):
        st = self.comp_scope(n.generators, state)
        s = self.p.new_site(n, 'set', self.where(n))
        self.p.grow(self.p.elem, s, self.ev(n.elt, st))
        return P.T('set', site=s)

    def e_DictComp(self, n, state):
        st = self.comp_scope(n.generators, state)
        s = self.p.new_site(n, 'dict', self.where(n))
        self.p.make_dynamic(s, 'a comprehension')
        self.p.grow(self.p.keys, s, self.ev(n.key, st))
        self.p.grow(self.p.elem, s, self.ev(n.value, st))
        return P.T('dict', site=s)

    def e_GeneratorExp(self, n, state):
        st = self.comp_scope(n.generators, state)
        e = self.ev(n.elt, st)
        return P.T('iter', (e,), lo=P.MEM)

    # -- calls
    def e_Call(self, n, state):
        if isinstance(n.func, ast.Attribute):
            recv = self.ev(n.func.value, state)
        else:
            recv = None
        ft = self.ev(n.func, state) if recv is None else None
        for a in n.args:
            if isinstance(a, ast.Starred):
                self.ev(a.value, state)
                return self.issue(n, 'dynamic', '*args call')
        for k in n.keywords:
            if k.arg is None and not (isinstance(n.func, ast.Name) and n.func.id == 'dict'):
                self.ev(k.value, state)
                return self.issue(n, 'dynamic', '**kwargs call')
        lazy = (ast.Lambda, ast.GeneratorExp)
        args = [self.ev(a, state) if not isinstance(a, lazy) else None for a in n.args]
        kw = dict((k.arg, self.ev(k.value, state) if not isinstance(k.value, lazy) else None) for k in n.keywords)
        if recv is not None:
            return self.method_call(n, recv, n.func.attr, args, kw, state)
        return self.call_value(n, ft, args, kw, state)

    def call_value(self, n, ft, args, kw, state):
        k = ft.kind
        if k in ('bottom', 'top'):
            return ft
        if k == 'fn':
            name = ft.name
            if name.startswith('builtin:'):
                return self.builtin(n, name[8:], args, kw, state)
            if name.startswith('bound:'):
                return self.call_fn(n, self.p.fns[name[6:]], args, kw, bound=True)
            if name not in self.p.fns:
                return self.issue(n, 'dynamic', 'a method kept as a value (%s)' % name.split(':', 1)[-1])
            return self.call_fn(n, self.p.fns[name], args, kw)
        if k == 'cls':
            if ft.name.startswith('exc:'):
                for a in args:
                    if a is not None and a.kind not in ('str', 'bottom', 'top', 'int'):
                        self.issue(n, 'syntax', '%s with a %s' % (ft.name[4:], a.kind))
                if len(args) > 1:
                    self.issue(n, 'syntax', 'exception with several arguments')
                return P.T('inst', name=ft.name), ('exc', ft.name[4:])
            return self.construct(n, ft.name, args, kw)
        if k == 'extern':
            if ft.name.startswith('std:'):
                return self.std_call(n, ft.name[4:], args, kw, state)
            return self.issue(n, 'extern', ft.name.split('.')[0])
        return self.issue(n, 'conflict', 'calling a %s' % k)

    def construct(self, n, cq, args, kw):
        c = self.p.classes[cq]
        if c.foreign_base:
            return self.issue(n, 'extern', 'class derived from %s' % c.foreign_base)
        init = self.p.find_method(cq, '__init__')
        if init is not None:
            self.bind_args(n, init, args, kw, bound=True)
        elif c.exception and len(args) <= 1 and not kw:
            if args and args[0] is not None and args[0].kind not in ('str', 'bottom', 'top'):
                self.issue(n, 'syntax', '%s with a %s' % (c.name, args[0].kind))
        elif args or kw:
            self.issue(n, 'conflict', 'arguments to a class with no __init__')
        return P.T('inst', name=cq), ('ctor', cq)

    def call_fn(self, n, f, args, kw, bound=False):
        if f.kind == 'property':
            return self.issue(n, 'conflict', 'calling a property')
        self.bind_args(n, f, args, kw, bound=bound)
        targets = [f]
        if bound and f.cls is not None:
            for d in self.p.descendants(f.cls.qname):
                o = self.p.classes[d].methods.get(f.name)
                if o is not None:
                    self.bind_args(n, o, args, kw, bound=True)
                    targets.append(o)
        r = P.BOTTOM
        for t in targets:
            r = self.p.join(r, self.p.ret(t.qname))
        if r.kind == 'top':                                  # its result has no type: that is the callee's refusal
            r = P.top('calls-refused:%s' % f.name)
        return r, ('method' if bound else 'fn', f.qname)

    def bind_args(self, n, f, args, kw, bound=False):
        names = f.bound if (bound or f.kind not in ('method', 'property', 'classmethod')) else f.params
        if f.kind in ('method', 'property') and not bound:
            names = f.params
        if f.varargs or f.kwargs:
            return
        if len(args) > len(names):
            self.issue(n, 'conflict', 'too many arguments to %s' % f.name)
            return
        given = set()
        for name, t in zip(names, args):
            given.add(name)
            if t is not None:
                self.contribute_param(f.qname, name, self.arg_fit(n, f, name, t))
            else:
                self.issue(n, 'dynamic', 'lambda or generator passed to %s' % f.name)
        for k, t in kw.items():
            if k not in names and k not in f.kwonly:
                self.issue(n, 'conflict', 'no parameter %s of %s' % (k, f.name))
                continue
            given.add(k)
            if t is not None:
                self.contribute_param(f.qname, k, self.arg_fit(n, f, k, t))
        for name in list(names) + f.kwonly:
            if name not in given and name not in f.defaults:
                self.issue(n, 'conflict', 'missing argument %s to %s' % (name, f.name))

    def method_call(self, n, recv, attr, args, kw, state):
        k = recv.kind
        if k in ('bottom', 'top'):
            return recv
        if k == 'inst':
            if recv.name.startswith('exc:'):
                return self.issue(n, 'dynamic', 'method of an exception')
            f = self.p.find_method(recv.name, attr)
            if f is None:
                t = self.p.attr_type(recv.name, attr)
                if t is not None:
                    return self.issue(n, 'dynamic', 'calling an attribute (%s)' % attr)
                return self.issue(n, 'unresolved', '%s.%s()' % (recv.name.rsplit('.', 1)[-1], attr))
            if f.kind == 'static':
                return self.call_fn(n, f, args, kw)
            return self.call_fn(n, f, args, kw, bound=True)
        if k == 'module':
            if recv.name.startswith('std:'):
                return self.std_call(n, recv.name[4:] + '.' + attr, args, kw, state)
            mod = self.p.modules[recv.name]
            r = self.p.resolve_global(mod, attr)
            if r is None:
                return self.issue(n, 'unresolved', recv.name + '.' + attr)
            ft = self.ref_type(n.func, r)
            return self.call_value(n, ft, args, kw, state)
        if k == 'cls':
            if recv.name.startswith('exc:'):
                return self.issue(n, 'dynamic', 'exception class method')
            f = self.p.find_method(recv.name, attr)
            if f is not None and f.kind == 'static':
                return self.call_fn(n, f, args, kw)
            if f is not None and f.kind == 'method' and args and args[0] is not None and args[0].kind == 'inst' \
                    and recv.name in self.p.mro(args[0].name) and isinstance(n.args[0], ast.Name) \
                    and self.fn is not None and self.fn.params and n.args[0].id == self.fn.params[0]:
                # Base.method(self, ...): the base's own method, not a virtual call
                if attr == '__init__' and not (self.fn.name == '__init__' and self._first_stmt(n)):
                    return self.issue(n, 'syntax', 'base __init__ called other than first in __init__')
                self.bind_args(n, f, args[1:], kw, bound=True)
                return self.p.ret(f.qname), ('basecall', f.qname)
            return self.issue(n, 'dynamic', '%s.%s through the class' % (recv.name.rsplit('.', 1)[-1], attr))
        if k == 'extern':
            if recv.name.startswith('std:'):
                return self.std_call(n, recv.name[4:] + '.' + attr, args, kw, state)
            if recv.name == 'file':
                return self.file_method(n, attr, args, kw)
            if recv.name == 'super':
                return self.super_call(n, attr, args, kw)
            return self.issue(n, 'extern', recv.name.split('.')[0])
        if k == 'opt':
            return self.issue(n, 'conflict', '.%s() of a value that may be None' % attr)
        from codebreaker.translate.frontends.python import methods
        return methods.call(self, n, recv, attr, args, kw, state)

    def _first_stmt(self, call):
        body = [s for s in self.fn.node.body if not (isinstance(s, ast.Expr) and isinstance(s.value, ast.Constant))]
        return bool(body) and isinstance(body[0], ast.Expr) and body[0].value is call

    def super_call(self, n, attr, args, kw):
        if self.cls is None or not self.cls.bases:
            return self.issue(n, 'dynamic', 'super() without a base of the program')
        base = self.cls.bases[0]
        f = self.p.find_method(base, attr)
        if f is None:
            return self.issue(n, 'unresolved', 'super().%s' % attr)
        self.bind_args(n, f, args, kw, bound=True)
        return self.p.ret(f.qname), ('super', f.qname)

    def file_method(self, n, attr, args, kw):
        if attr == 'read' and not args:
            return P.BYTES, ('bmethod', 'file.read')
        return self.issue(n, 'stdlib', 'file.' + attr)

    def builtin(self, n, name, args, kw, state):
        from codebreaker.translate.frontends.python import methods
        return methods.builtin(self, n, name, args, kw, state)

    def std_call(self, n, name, args, kw, state):
        from codebreaker.translate.frontends.python import methods
        return methods.std_call(self, n, name, args, kw, state)

    def lambda_type(self, lam, params, state):
        """the type a key function returns for these parameter types"""
        if not isinstance(lam, ast.Lambda):
            return self.ev(lam, state)
        a = lam.args
        if a.vararg or a.kwarg or a.kwonlyargs or len(a.args) != len(params):
            return self.issue(lam, 'dynamic', 'lambda signature')
        st = dict(state)
        for p_, t in zip(a.args, params):
            st[p_.arg] = t
            self.note(p_, t)
        t = self.ev(lam.body, st)
        self.note(lam, t)
        return t

    def gen_elem(self, g, state):
        """element type of a generator expression argument"""
        st = self.comp_scope(g.generators, state)
        return self.ev(g.elt, st), st


class ModuleEval(Eval):
    """A module's top level: the types of its names, its classes' class-level values and its functions'
    parameter defaults (each evaluated where Python evaluates it, once, at import)."""

    def safe_ev(self, v):
        try:
            return self.ev(v, {})
        except RecursionError:
            return P.top('frontend-error:recursion')
        except Exception as e:                               # a front end bug: that name has no type
            return P.top('frontend-error:%s: %s' % (type(e).__name__, e))

    def __init__(self, prog, module, record=False):
        Eval.__init__(self, prog, None, record)
        self.m = module

    def run(self):
        m = self.m
        g = self.p.globals.setdefault(m.name, {})
        for name, values in m.assigns.items():
            t = P.BOTTOM
            for v in values:
                t = self.p.join(t, self.safe_ev(v) if v is not None else P.top('module-level unpacking'))
            old = g.get(name, P.BOTTOM)
            new = self.p.join(old, t)
            if new != old:
                g[name] = new
                self.p.touch(('g', m.name))
        for c in m.classes.values():
            for name, v in c.class_attrs.items():
                t = self.safe_ev(v)
                key = (c.qname, name)
                old = self.p.class_attr_types.get(key, P.BOTTOM)
                new = self.p.join(old, t)
                if new != old:
                    self.p.class_attr_types[key] = new
                    self.p.touch(('ca',))
        fns = list(m.functions.values()) + [f for c in m.classes.values() for f in c.methods.values()]
        for f in fns:
            for name, d in f.defaults.items():
                if isinstance(d, (ast.List, ast.Dict, ast.Set)) or (isinstance(d, ast.Call)):
                    t = P.top('mutable default')
                else:
                    t = self.safe_ev(d)
                key = (f.qname, name)
                old = self.p.default_types.get(key, P.BOTTOM)
                new = self.p.join(old, t)
                if new != old:
                    self.p.default_types[key] = new
                    self.p.touch(('d',))
        return self


STD_ATTRS = {
    'sys.maxsize': P.const_int(2 ** 63 - 1),
    'os.sep': P.STR,
    'os.linesep': P.STR,
    'os.pathsep': P.STR,
    'math.pi': P.FLOAT,
    'string.hexdigits': P.STR,
    'string.digits': P.STR,
    'string.ascii_letters': P.STR,
    'string.printable': P.STR,
    'struct.error': P.T('cls', name='exc:struct.error'),
}
