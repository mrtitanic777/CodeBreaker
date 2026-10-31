"""lower.py -- a typed Python module into L2 (codebreaker/translate/ir.py), or a refusal with its reason.

Inference (infer.py) has run the whole program; this walks each function again with what it found --
every expression's type, every call's target -- and writes the L2 tree. Python's own operations that C++
does not have as such are calls owned by "py" (Sym('function', name, owner='py')): their names are the
API map's (codebreaker/java/api_maps/python.json) or the support header's core semantics (truthy, slice,
floordiv, ...). A back end writes them; nothing here knows C++.

A function is refused -- left in Python -- when anything in it has no single type, when an integer is not
shown to stay inside int64 (int_policy 'strict'; 'checked' writes overflow-checked operations instead),
when it uses a construct the front end does not lower, or when it calls a refused function.

Python's model, kept: a list, dict, set, class instance or mutated record is a reference (aliasing is
Python's); str, bytes, tuples, numbers and records never changed after they are built are values.
"""
import ast
import json
import re

from codebreaker.translate import ir
from codebreaker.translate.frontends.python import pytypes as P
from codebreaker.translate.frontends.python.infer import Issue, _str_const, _int_const

PY = 'py'
I64 = ir.Type('i64')
BOOL = ir.Type('bool')
F64 = ir.Type('f64')
VOID = ir.VOID
STR = ir.Type('class', 'py.str')
BYTES = ir.Type('class', 'py.bytes')
PURE_BUILTINS = frozenset('len str int bool float abs min max ord chr hex repr isinstance range enumerate zip reversed sorted'.split())


class Refused(Exception):
    def __init__(self, cls, detail='', line=0):
        Exception.__init__(self, '%s:%s' % (cls, detail))
        self.issue = Issue(cls, detail, line)


def pyfn(name, args, t, target=None, line=0, typeargs=None):
    return ir.Call(target=target, sym=ir.Sym('function', name, PY), args=list(args), typeargs=typeargs or [],
                   type=t, line=line)


def lit(v, t, text=None):
    return ir.Lit(value=v, text=text, type=t)


def _camel(s):
    s = re.sub(r'[^0-9A-Za-z]+', ' ', s).strip()
    return ''.join(w[:1].upper() + w[1:] for w in s.split()) or 'Record'


def _singular(w):
    if w.endswith('ies'):
        return w[:-3] + 'y'
    if w.endswith(('sses', 'xes')):
        return w[:-2]
    if w.endswith('s') and not w.endswith('ss'):
        return w[:-1]
    return w


_VERBS = ('parse_', 'read_', 'get_', 'make_', 'build_', 'load_', 'collect_', 'find_', 'scan_', 'compute_')
_GENERIC = {'out', 'r', 'd', 'e', 'x', 'h', 's', 'o', 'v', 'res', 'result', 'info', 'rec', 'item', 'entry', 'm', 'p',
            'n', 'it', 'data', 'obj', 'tmp', 'row', 'c', 'f', 't'}


class Records(object):
    """What every container site of the program is in C++: a record struct (named), a dict, a list, a set."""

    def __init__(self, prog):
        self.p = prog
        self.names = {}                  # root -> struct name
        self.owner_fn = {}               # root -> the function whose site created it
        self.module = {}                 # root -> module qname it is declared in
        used = {}
        fn_of_line = self._fn_lines()
        members = {}
        for x in prog.parent:
            members.setdefault(prog.find(x), []).append(x)
        self.home = {}                   # root -> the site that makes it (where its struct is declared)
        for s in sorted(members):
            if not prog.is_record(s):
                continue
            home = None
            for m in sorted(members[s]):                     # the site in a function that returns the record
                mod, line = prog.site_where.get(m) or ('?', 0)
                fq = fn_of_line.get(mod, {}).get(line)
                if fq is not None and self._holds(prog.returns.get(fq, P.BOTTOM), s):
                    home = m
                    break
            if home is None:
                home = min(members[s])
            self.home[s] = home
            where = prog.site_where.get(home) or ('?', 0)
            mod, line = where
            fq = fn_of_line.get(mod, {}).get(line)
            self.owner_fn[s] = fq
            self.module[s] = mod
            name = self._name(s, fq)
            k = (mod, name)
            if k in used:
                used[k] += 1
                name = '%s%d' % (name, used[k])
            else:
                used[k] = 1
            self.names[s] = name

    def _fn_lines(self):
        out = {}
        for f in self.p.fns.values():
            d = out.setdefault(f.module.name, {})
            for line in range(f.node.lineno, (f.node.end_lineno or f.node.lineno) + 1):
                d.setdefault(line, f.qname)
        # a method's lines are the method's (classes' lines were set first by the module order only for
        # functions: methods appear later in fns, so prefer the innermost: the shortest span)
        for f in sorted(self.p.fns.values(), key=lambda f: -(f.lines)):
            d = out[f.module.name]
            for line in range(f.node.lineno, (f.node.end_lineno or f.node.lineno) + 1):
                d[line] = f.qname
        return out

    def _name(self, s, fq):
        hints = [h for h in self.p.hints.get(s, []) if h.lower() not in _GENERIC and len(h) > 2]
        stem = None
        if fq is not None:
            fname = fq.rsplit('.', 1)[-1].lstrip('_')
            for v in _VERBS:
                if fname.startswith(v):
                    fname = fname[len(v):]
            stem = _camel(_singular(fname)) if fname and fname != '_init__' else None
            ret = self.p.returns.get(fq)
            flows_out = ret is not None and self._holds(ret, s)
            if flows_out and stem:
                return stem
            if stem and hints:
                return stem + _camel(_singular(hints[0]))
            if stem and self.p.hints.get(s):
                h = self.p.hints[s][0]
                return stem + _camel(h) if len(h) > 2 else stem + 'Record'
        if hints:
            return _camel(_singular(hints[0]))
        return (stem or 'Anon') + 'Record'

    def _holds(self, t, s, depth=0):
        if depth > 4:
            return False
        if t.kind == 'dict':
            return self.p.find(t.site) == s
        if t.kind in ('list', 'set'):
            return self._holds(self.p.elem[self.p.find(t.site)], s, depth + 1)
        return any(self._holds(a, s, depth + 1) for a in t.args)

    def fields(self, s):
        """[(key, T, required)]: in the order the record's own site writes them, then the others"""
        s = self.p.find(s)
        req = self.p.required.get(s) or frozenset()
        fields = self.p.fields[s]
        order = []
        home = self.home.get(s)
        node = self.p.site_node.get(home) if home is not None else None
        if node is not None:
            order = [k for k in self.p.literal_keys.get(id(node), []) if k in fields]
        order += [k for k in fields if k not in order]
        return [(k, fields[k], k in req) for k in order]

    def is_ref(self, s):
        return self.p.find(s) in self.p.mutated


class Context(object):
    """What lowering the program shares: its records, the types written so far, and the policy."""

    def __init__(self, prog, int_policy='strict'):
        self.p = prog
        self.records = Records(prog)
        self.int_policy = int_policy
        self.used_records = set()

    def qual_record(self, s):
        s = self.p.find(s)
        return '%s.%s' % (self.records.module[s], self.records.names[s])

    def ty(self, t, line=0, what='value'):
        """a pytypes.T as an ir.Type; Refused when it has none"""
        k = t.kind
        if k == 'int':
            if t.fits_i64() or self.int_policy == 'checked':
                return I64
            raise Refused('int-range', '%s in [%s, %s]' % (what, t.lo, t.hi), line)
        if k == 'bool':
            return BOOL
        if k == 'float':
            return F64
        if k == 'none':
            return VOID
        if k == 'str':
            return STR
        if k == 'bytes':
            return BYTES
        if k == 'bytearray':
            return ir.Type('class', 'py.bytearray')
        if k == 'opt':
            inner = self.ty(t.args[0], line, what)
            return ir.Type('class', 'py.optional', [inner])
        if k == 'tuple':
            return ir.Type('class', 'py.tuple', [self.ty(a, line, what) for a in t.args])
        if k in ('list', 'set', 'dict') and self.p.find(t.site) in self.p.poisoned_sites:
            raise Refused('untyped', 'a %s also given items of no single type' % k, line)
        if k in ('list', 'set'):
            s = self.p.find(t.site)
            if s in self.p.dynamic and self.p.ckind[s] != k:
                raise Refused('conflict', self.p.dynamic[s], line)
            e = self.p.elem[s]
            if e.kind == 'bottom':
                raise Refused('untyped', 'the items of an empty %s' % k, line)
            return ir.Type('class', 'py.' + k, [self.ty(e, line, k + ' item')])
        if k == 'dict':
            s = self.p.find(t.site)
            if self.p.ckind[s] != 'dict':
                raise Refused('conflict', self.p.dynamic.get(s, 'not a dict'), line)
            if self.p.is_record(s):
                for key, ft, req in self.records.fields(s):
                    if ft.kind == 'bottom':
                        raise Refused('untyped', 'record field %r' % key, line)
                    if not req and ft.kind == 'opt':
                        raise Refused('conflict', 'record field %r both optional and None' % key, line)
                self.used_records.add(s)
                return ir.Type('class', self.qual_record(s), [], ref=self.records.is_ref(s), record=True)
            kt, vt = self.p.keys[s], self.p.elem[s]
            if kt.kind == 'bottom' or vt.kind == 'bottom':
                raise Refused('untyped', 'the keys or values of an empty dict', line)
            if vt.kind == 'top':
                raise Refused('conflict', 'dict values: ' + str(vt.why), line)
            return ir.Type('class', 'py.dict', [self.ty(kt, line, 'dict key'), self.ty(vt, line, 'dict value')])
        if k == 'inst':
            if t.name.startswith('exc:'):
                return ir.Type('class', 'py.' + t.name[4:].replace('struct.error', 'StructError'))
            c = self.p.classes.get(t.name)
            if c is not None and c.exception:
                return ir.Type('class', t.name, [], exception=True)
            return ir.Type('class', t.name, [], ref=True)
        if k == 'extern' and t.name == 'file':
            return ir.Type('class', 'py.file')
        if k == 'iter':
            # an iterator (range, enumerate, zip, dict.items ...) is consumed where it is made: a loop or
            # a call that takes an iterable; kept in a name, passed or returned it has no C++ type here
            if what != 'value':
                raise Refused('syntax', 'an iterator as a %s' % what, line)
            return ir.Type('class', 'py.iter', [self.ty(t.args[0], line, 'value')])
        if k == 'top':
            why = t.why or ''
            if ':' in why and why.split(':')[0] in ('dynamic', 'stdlib', 'extern', 'syntax', 'unresolved', 'int-range',
                                                     'conflict', 'untyped', 'calls-refused', 'frontend-error'):
                c, d = why.split(':', 1)
                raise Refused(c, d, line)
            raise Refused('conflict', why, line)
        if k == 'bottom':
            raise Refused('untyped', what, line)
        raise Refused('conflict', 'a %s as a value' % k, line)


def is_ref_type(t):
    return t is not None and t.kind == 'class' and t.info.get('ref')


def is_opt(t):
    return t is not None and t.kind == 'class' and t.name == 'py.optional'


class FnLower(object):
    """One function of the program into an ir.MethodDecl."""

    def __init__(self, ctx, f, ev):
        self.ctx = ctx
        self.p = ctx.p
        self.f = f
        self.ev = ev
        self.src = f.module.source
        self.pre = []                    # statements an expression needs before its statement
        self.tmp = 0
        self.decl_at = {}                # id(stmt) -> [names declared just before it]
        self.decl_init = {}              # id(Assign stmt) -> name declared by it
        self.decl_tuple = {}             # id(Assign stmt) -> names an unpacking declares
        self.local_types = {}
        self.declared = set()
        self.loopvars = {}
        self.calls = set()               # qualnames of the program's functions it calls
        self.classes_made = set()
        self.globals_used = set()
        self.records_used = set()

    # -- types and positions
    def T(self, node):
        t = self.ev.types.get(id(node))
        if t is None:
            raise Refused('untyped', 'no type reached %s' % type(node).__name__.lower(), getattr(node, 'lineno', 0))
        return t

    def ty(self, t, node=None, what='value'):
        return self.ctx.ty(t, getattr(node, 'lineno', 0), what)

    def R(self, node):
        return self.ev.res.get(id(node))

    def line(self, n):
        return getattr(n, 'lineno', 0)

    # -- the function
    def lower(self):
        f = self.f
        for i in self.ev.issues:
            raise Refused(i.cls, i.detail, i.line)
        if f.qname in self.p.crashes:
            raise Refused('frontend-error', self.p.crashes[f.qname], f.node.lineno)
        params = []
        skip = 1 if f.kind in ('method', 'property') else 0
        if f.kind == 'classmethod':
            raise Refused('dynamic', 'classmethod', f.node.lineno)
        for name in f.params[skip:] + f.kwonly:
            t = self.p.param_type(f, name)
            if t.kind == 'bottom':
                got = self.p.param_poison.get((f.qname, name))
                if got is not None:
                    raise Refused('untyped', 'parameter %s (its callers pass values of no type: %s in %s)'
                                  % (name, got[1].split(':')[0] + ':' + got[1].split(':', 1)[-1][:40], got[0].rsplit('.', 1)[-1]),
                                  f.node.lineno)
                raise Refused('untyped', 'parameter %s (no annotation, no call site)' % name, f.node.lineno)
            it = self.ty(t, f.node, 'parameter ' + name)
            self.local_types[name] = it
            self.declared.add(name)
            mods = []
            d = f.defaults.get(name)
            if isinstance(d, ast.Constant):
                v = d.value
                mods.append('default=' + json.dumps({'bytes': v.decode('latin1')} if isinstance(v, bytes) else v))
            params.append(ir.Param(sym=ir.Sym('param', name, f.qname, type=it), type=it, mods=mods))
        ret_t = self.p.returns.get(f.qname, P.NONE)
        if ret_t.kind == 'bottom':
            ret_t = P.NONE
        if f.name == '__init__':
            ret = VOID
        else:
            ret = self.ty(ret_t, f.node, 'return value')
        self.ret = ret
        self.plan_locals()
        body = self.block(self.body_stmts(f.node.body))
        kind = 'function' if f.cls is None else ('ctor' if f.name == '__init__' else 'method')
        mods = []
        if f.kind == 'static':
            mods.append('static')
        if f.kind == 'property':
            mods.append('property')
        sym = ir.Sym('function' if f.cls is None else ('ctor' if kind == 'ctor' else 'method'), f.name,
                     f.cls.qname if f.cls else f.module.name, f.kind == 'static', params=[p.type for p in params], ret=ret)
        doc = ast.get_docstring(f.node)
        m = ir.MethodDecl(sym=sym, name=f.name, kind=kind, params=params, ret=ret, body=body, mods=mods, throws=[],
                          typeparams=[], comments=[doc] if doc else [], line=f.node.lineno)
        return m

    def body_stmts(self, stmts):
        if stmts and isinstance(stmts[0], ast.Expr) and isinstance(stmts[0].value, ast.Constant) \
                and isinstance(stmts[0].value.value, str):
            return stmts[1:]
        return stmts

    # -- where each local is declared
    def plan_locals(self):
        """A local's C++ type is the join of everything stored to it. It is declared at its first
        assignment when that is a statement of the innermost block holding every use, else just before the
        statement of that block that first touches it. A loop's variable used only inside the loop is
        declared by the loop itself (and one never read, like `_`, by each loop)."""
        f = self.f
        occ = {}                                             # name -> [(path, stmt)]; path: ((list id, i, stmt), ...)
        stores = {}
        loads = set()

        def visit(stmts, path):
            for i, st in enumerate(stmts):
                here = path + ((id(stmts), i, st),)
                for n in self._own_nodes(st):
                    if isinstance(n, ast.Name) and n.id in self.ev.locals and n.id not in self.declared:
                        occ.setdefault(n.id, []).append((here, st))
                        if isinstance(n.ctx, ast.Store):
                            stores.setdefault(n.id, []).append(n)
                        else:
                            loads.add(n.id)
                for field in ('body', 'orelse', 'finalbody'):
                    sub = getattr(st, field, None)
                    if isinstance(sub, list) and sub and isinstance(sub[0], ast.stmt):
                        visit(sub, here)
                for h in getattr(st, 'handlers', []) or []:
                    visit(h.body, here)
        visit(f.node.body, ())
        for name, uses in occ.items():
            ts = [self.ev.types.get(id(n)) for n in stores.get(name, [])]
            ts = [t for t in ts if t is not None]
            if not ts:
                continue                                     # only read where no store reached: not lowered
            j = ts[0]
            for t in ts[1:]:
                j = self.p.join(j, t)
            self.local_types[name] = self.ty(j, stores[name][0], 'local ' + name)
            loop_targets = [st for _p, st in uses if isinstance(st, ast.For) and name in self._target_names(st.target)]
            if name not in loads and stores[name] and all(
                    any(n is x for st in loop_targets for x in ast.walk(st.target)) for n in stores[name]):
                self.loopvars[name] = True                   # never read: each loop declares its own
                continue
            paths = [tuple((lid, i) for lid, i, _s in u[0]) for u in uses]
            L = 0
            while all(len(p_) > L for p_ in paths) and len(set(p_[L] for p_ in paths)) == 1:
                L += 1
            first_path = uses[0][0]
            if all(len(p_) > L for p_ in paths) and len(set(p_[L][0] for p_ in paths)) == 1:
                anchor = first_path[L][2]                    # siblings in one block: the first of them
            else:
                anchor = first_path[L - 1][2]                # the statement whose blocks hold them all
            if isinstance(anchor, ast.For) and name in self._target_names(anchor.target) and \
                    all(any(x is u[1] or x is anchor for x in ast.walk(anchor)) for u in uses):
                continue                                     # the loop declares it
            first_stmt = uses[0][1]
            if anchor is first_stmt and isinstance(first_stmt, ast.Assign) and len(first_stmt.targets) == 1 \
                    and isinstance(first_stmt.targets[0], ast.Name) and first_stmt.targets[0].id == name \
                    and not any(isinstance(n, ast.Name) and n.id == name for n in ast.walk(first_stmt.value)):
                self.decl_init[id(first_stmt)] = name
            elif anchor is first_stmt and isinstance(first_stmt, ast.Assign) and len(first_stmt.targets) == 1 \
                    and isinstance(first_stmt.targets[0], ast.Tuple) and name in self._target_names(first_stmt.targets[0]) \
                    and not any(isinstance(n, ast.Name) and n.id == name for n in ast.walk(first_stmt.value)):
                self.decl_tuple.setdefault(id(first_stmt), set()).add(name)
            else:
                self.decl_at.setdefault(id(anchor), []).append(name)

    def _target_names(self, t):
        if isinstance(t, ast.Name):
            return [t.id]
        if isinstance(t, (ast.Tuple, ast.List)):
            out = []
            for e in t.elts:
                out += self._target_names(e)
            return out
        return []

    def _index_lists(self):
        self._lists = {}
        for n in ast.walk(self.f.node):
            for field in ('body', 'orelse', 'finalbody'):
                sub = getattr(n, field, None)
                if isinstance(sub, list):
                    self._lists[id(sub)] = sub

    def _own_nodes(self, st):
        """the nodes of a statement outside its nested statement blocks"""
        out = []
        todo = [st]
        while todo:
            n = todo.pop()
            out.append(n)
            for field, v in ast.iter_fields(n):
                if n is st and field in ('body', 'orelse', 'finalbody', 'handlers'):
                    continue
                if isinstance(v, ast.AST):
                    todo.append(v)
                elif isinstance(v, list):
                    todo.extend(x for x in v if isinstance(x, ast.AST))
        return out

    # -- statements
    def block(self, stmts):
        out = []
        for st in stmts:
            for name in self.decl_at.get(id(st), ()):
                t = self.local_types[name]
                out.append(ir.Local(sym=ir.Sym('local', name, self.f.qname, type=t), type=t, init=None, mods=[]))
                self.declared.add(name)
            self.pre = []
            got = self.stmt(st)
            out.extend(self.pre)
            self.pre = []
            out.extend(got)
        return ir.Block(stmts=out)

    def stmt(self, st):
        m = getattr(self, 's_' + type(st).__name__, None)
        if m is None:
            raise Refused('syntax', type(st).__name__.lower(), self.line(st))
        r = m(st)
        return r if isinstance(r, list) else [r]

    def s_Pass(self, st):
        return []

    def s_Expr(self, st):
        if isinstance(st.value, ast.Constant):
            return [ir.Comment(text=str(st.value.value))] if isinstance(st.value.value, str) else []
        return ir.ExprStmt(expr=self.expr(st.value), line=self.line(st))

    def s_Assign(self, st):
        if len(st.targets) != 1:
            raise Refused('syntax', 'chained assignment', self.line(st))
        tgt = st.targets[0]
        if isinstance(st.value, (ast.ListComp, ast.SetComp, ast.DictComp)) and isinstance(tgt, ast.Name) \
                and not any(isinstance(n, ast.Name) and n.id == tgt.id for n in ast.walk(st.value)):
            return self.comp_into(st, tgt, st.value)
        if self.is_record_literal(st.value) and isinstance(tgt, ast.Name) and id(st) in self.decl_init:
            return self.record_builder(st, tgt)
        value = self.expr(st.value)
        return self.store(tgt, value, st)

    def s_AnnAssign(self, st):
        if st.value is None:
            return []
        return self.store(st.target, self.expr(st.value), st)

    def s_AugAssign(self, st):
        t = self.T(st)
        target = self.target_expr(st.target)
        value = self.expr(st.value)
        cur = self.expr_of_target(st.target, target)
        new = self.binop(st, st.op, cur, value, self.T(st.target) if id(st.target) in self.ev.types else t,
                         self.T(st.value), t)
        if isinstance(new, ir.Binary) and new.op in ('+', '-', '*', '&', '|', '^', '<<', '>>', '/', '%') \
                and new.left is cur:
            return ir.ExprStmt(expr=ir.Assign(op=new.op, target=target, value=value, type=new.type), line=self.line(st))
        if isinstance(new, ir.Concat) and new.parts and new.parts[0] is cur:
            return ir.ExprStmt(expr=ir.Assign(op='+', target=target, value=ir.Concat(parts=new.parts[1:], type=new.type)
                                              if len(new.parts) > 2 else new.parts[1], type=new.type), line=self.line(st))
        return ir.ExprStmt(expr=ir.Assign(op=None, target=target, value=new, type=new.type), line=self.line(st))

    def expr_of_target(self, tgt, lowered):
        """the target read as a value (for x op= y)"""
        if isinstance(tgt, ast.Name):
            return ir.Name(sym=lowered.sym, type=lowered.type)
        return lowered

    def store(self, tgt, value, st):
        line = self.line(st)
        if isinstance(tgt, ast.Name):
            name = tgt.id
            t = self.local_types.get(name)
            if t is None:
                raise Refused('untyped', 'local ' + name, line)
            value = self.coerce(value, t)
            if id(st) in self.decl_init and name not in self.declared:
                self.declared.add(name)
                return ir.Local(sym=ir.Sym('local', name, self.f.qname, type=t), type=t, init=value, mods=[], line=line)
            return ir.ExprStmt(expr=ir.Assign(op=None, target=ir.Name(sym=ir.Sym('local', name, self.f.qname, type=t), type=t),
                                              value=value, type=t), line=line)
        if isinstance(tgt, (ast.Tuple, ast.List)):
            return self.unpack(tgt, value, st)
        target = self.target_expr(tgt)
        value = self.coerce(value, target.type)
        return ir.ExprStmt(expr=ir.Assign(op=None, target=target, value=value, type=target.type), line=line)

    def unpack(self, tgt, value, st):
        line = self.line(st)
        names = []
        for e in tgt.elts:
            if not isinstance(e, ast.Name):
                raise Refused('syntax', 'unpacking into %s' % type(e).__name__, line)
            names.append(e.id)
        tts = value.type.args if value.type.kind == 'class' and value.type.name == 'py.tuple' else None
        if tts is None or len(tts) != len(names):
            raise Refused('conflict', 'unpacking a non-tuple', line)
        if len(names) == 1:                                  # `x, = t`: its one item
            n = names[0]
            item = self.coerce(pyfn('get', [value], tts[0], typeargs=[0]), self.local_types.get(n))
            if n in self.decl_tuple.get(id(st), ()) and n not in self.declared:
                self.declared.add(n)
                t = self.local_types[n]
                return ir.Local(sym=ir.Sym('local', n, self.f.qname, type=t), type=t, init=item, mods=[], line=line)
            return self.store(tgt.elts[0], item, st)
        fresh = self.decl_tuple.get(id(st), set())
        if all(n in fresh and n not in self.declared and self.local_types.get(n) == et for n, et in zip(names, tts)):
            for n in names:
                self.declared.add(n)
            return ir.ExprStmt(expr=ir.Assign(op=None, target=pyfn('bind', [self.name_expr(n) for n in names], value.type),
                                              value=value, type=value.type), line=line)
        out = []
        for n in names:
            if n not in self.declared:
                t = self.local_types[n]
                out.append(ir.Local(sym=ir.Sym('local', n, self.f.qname, type=t), type=t, init=None, mods=[]))
                self.declared.add(n)
        out.append(ir.ExprStmt(expr=ir.Assign(op=None, target=pyfn('tie', [self.name_expr(n) for n in names], value.type),
                                              value=value, type=value.type), line=line))
        return out

    def name_expr(self, name):
        t = self.local_types[name]
        return ir.Name(sym=ir.Sym('local', name, self.f.qname, type=t), type=t)

    def target_expr(self, tgt):
        line = self.line(tgt)
        if isinstance(tgt, ast.Name):
            if tgt.id not in self.local_types:
                raise Refused('untyped', 'local ' + tgt.id, line)
            if tgt.id not in self.declared:
                t = self.local_types[tgt.id]
                self.pre.append(ir.Local(sym=ir.Sym('local', tgt.id, self.f.qname, type=t), type=t, init=None, mods=[]))
                self.declared.add(tgt.id)
            return self.name_expr(tgt.id)
        if isinstance(tgt, ast.Attribute):
            r = self.R(tgt)
            if not r or r[0] != 'field':
                raise Refused('dynamic', 'attribute store', line)
            obj = self.expr(tgt.value)
            ft = self.ctx.ty(self.p.attr_type(r[1], r[2]), line, 'attribute ' + r[2])
            return ir.Select(obj=obj, sym=ir.Sym('field', r[2], r[1], type=ft), type=ft, line=line)
        if isinstance(tgt, ast.Subscript):
            r = self.R(tgt)
            obj = self.expr(tgt.value)
            if r and r[0] == 'dict_store' and r[1] is not None and obj.type.info.get('record'):
                return self.field_ref(obj, r[1], tgt.value, line, store=True)
            if isinstance(tgt.slice, ast.Slice):
                raise Refused('syntax', 'slice assignment', line)
            idx = self.expr(tgt.slice)
            et = self.elem_type(obj.type, line)
            return ir.Index(arr=obj, index=idx, type=et, line=line)
        raise Refused('syntax', 'assignment target %s' % type(tgt).__name__, line)

    def elem_type(self, t, line):
        if t.kind == 'class' and t.name in ('py.list', 'py.set') and t.args:
            return t.args[0]
        if t.kind == 'class' and t.name == 'py.dict':
            return t.args[1]
        if t.kind == 'class' and t.name in ('py.bytes', 'py.bytearray'):
            return I64
        if t.kind == 'class' and t.name == 'py.str':
            return STR
        raise Refused('conflict', 'item of %r' % (t,), line)

    def s_Return(self, st):
        if st.value is None:
            return ir.Return(expr=None if self.ret == VOID else lit(None, self.ret), line=self.line(st))
        if isinstance(st.value, (ast.ListComp, ast.SetComp, ast.DictComp)):
            name = self.new_tmp('result')
            t = self.ty(self.T(st.value), st.value)
            out = [ir.Local(sym=ir.Sym('local', name, self.f.qname, type=t), type=t, init=self.empty(t), mods=[])]
            self.local_types[name] = t
            self.declared.add(name)
            out += self.comp_loops(st.value, name, t)
            out.append(ir.Return(expr=self.name_expr(name), line=self.line(st)))
            return out
        if self.ret == VOID:
            e = self.expr(st.value)
            if isinstance(st.value, ast.Constant) and st.value.value is None:
                return ir.Return(expr=None, line=self.line(st))
            return [ir.ExprStmt(expr=e), ir.Return(expr=None, line=self.line(st))]
        return ir.Return(expr=self.coerce(self.expr(st.value), self.ret), line=self.line(st))

    def s_Raise(self, st):
        if st.exc is None:
            return ir.Throw(expr=None, line=self.line(st))
        e = self.expr(st.exc)
        return ir.Throw(expr=e, line=self.line(st))

    def s_Assert(self, st):
        cond = self.cond(st.test)
        args = [self.expr(st.msg)] if st.msg is not None else []
        if args and args[0].type != STR:
            args = [pyfn('str', args, STR)]
        exc = ir.New(sym=ir.Sym('ctor', 'AssertionError', PY), args=args, type=ir.Type('class', 'py.AssertionError'))
        return ir.If(cond=ir.Unary(op='!', x=ir.Paren(x=cond, type=BOOL), postfix=False, type=BOOL),
                     then=ir.Block(stmts=[ir.Throw(expr=exc)]), orelse=None, line=self.line(st))

    def s_If(self, st):
        cond = self.cond(st.test)
        pre = self.pre
        self.pre = []
        then = self.block(st.body)
        orelse = None
        if st.orelse:
            if len(st.orelse) == 1 and isinstance(st.orelse[0], ast.If) and not self.decl_at.get(id(st.orelse[0])):
                saved = self.pre
                self.pre = []
                inner = self.s_If(st.orelse[0])
                if self.pre:
                    orelse = ir.Block(stmts=self.pre + [inner])
                else:
                    orelse = inner
                self.pre = saved
            else:
                orelse = self.block(st.orelse)
        self.pre = pre
        return ir.If(cond=cond, then=then, orelse=orelse, line=self.line(st))

    def s_While(self, st):
        saved = self.pre
        self.pre = []
        cond = self.cond(st.test)
        if self.pre:
            raise Refused('syntax', 'a while condition that needs statements', self.line(st))
        self.pre = saved
        body = self.block(st.body)
        return ir.While(cond=cond, body=body, line=self.line(st))

    def s_Break(self, st):
        return ir.Break(label=None, line=self.line(st))

    def s_Continue(self, st):
        return ir.Continue(label=None, line=self.line(st))

    def s_For(self, st):
        line = self.line(st)
        it = st.iter
        r = self.R(it)
        # `for x in range(...)`: a counted loop
        if r == ('builtin', 'range') and isinstance(st.target, ast.Name):
            return self.range_loop(st)
        iterable = self.expr(it)
        tgt = st.target
        names = [tgt.id] if isinstance(tgt, ast.Name) else [e.id for e in getattr(tgt, 'elts', []) if isinstance(e, ast.Name)]
        if not isinstance(tgt, ast.Name) and len(names) != len(getattr(tgt, 'elts', [])):
            raise Refused('syntax', 'loop target', line)
        inner_only = all(self.loop_local(st, n) for n in names)
        elem_t = self.ty(self.T(tgt), tgt, 'loop item')
        if inner_only:
            for n in names:
                self.declared.add(n)
            if isinstance(tgt, ast.Name):
                var = ir.Local(sym=ir.Sym('local', tgt.id, self.f.qname, type=self.local_types[tgt.id]),
                               type=self.local_types[tgt.id], init=None, mods=[])
            else:
                var = ir.Local(sym=ir.Sym('local', ','.join(names), self.f.qname, type=elem_t), type=elem_t, init=None,
                               mods=['bind'])
            body = self.block(st.body)
            return ir.ForEach(var=var, iterable=iterable, body=body, line=line)
        tmpn = self.new_tmp('item')
        self.local_types[tmpn] = elem_t
        var = ir.Local(sym=ir.Sym('local', tmpn, self.f.qname, type=elem_t), type=elem_t, init=None, mods=[])
        pre = []
        for n in names:
            if n not in self.declared:
                t = self.local_types[n]
                pre.append(ir.Local(sym=ir.Sym('local', n, self.f.qname, type=t), type=t, init=None, mods=[]))
                self.declared.add(n)
        first = self.store(tgt, self.name_expr(tmpn), st) if isinstance(tgt, ast.Name) else \
            self.unpack(tgt, self.name_expr(tmpn), st)
        first = first if isinstance(first, list) else [first]
        body = self.block(st.body)
        body.stmts = first + body.stmts
        return pre + [ir.ForEach(var=var, iterable=iterable, body=body, line=line)]

    def loop_local(self, st, name):
        """the loop variable is used only inside the loop (C++ can declare it in the loop's header)"""
        if self.loopvars.get(name):
            return True
        inside = set(id(n) for n in ast.walk(st))
        for n in ast.walk(self.f.node):
            if isinstance(n, ast.Name) and n.id == name and id(n) not in inside:
                return False
        for n in ast.walk(st):                       # nor stored again inside, nor in a nested loop's target
            if isinstance(n, ast.Name) and n.id == name and isinstance(n.ctx, ast.Store) and n is not st.target \
                    and not (isinstance(st.target, ast.Tuple) and n in st.target.elts):
                return False
        return name not in self.declared or self.loopvars.get(name)

    def range_loop(self, st):
        line = self.line(st)
        call = st.iter
        args = [self.expr(a) for a in call.args]
        step = _int_const(call.args[2]) if len(call.args) == 3 else 1
        if len(args) == 1:
            start, stop = lit(0, I64), args[0]
        else:
            start, stop = args[0], args[1]
        name = st.target.id
        it_t = self.local_types.get(name, I64)
        inner = self.loop_local(st, name)
        stmts = []
        if not self.pure(call.args[-1] if len(call.args) < 3 else call.args[1], st):
            sname = self.new_tmp('stop')
            self.local_types[sname] = I64
            stmts.append(ir.Local(sym=ir.Sym('local', sname, self.f.qname, type=I64), type=I64, init=stop, mods=['const']))
            stop = self.name_expr(sname)
        if inner:
            var = name
            self.declared.add(name)
        else:
            var = self.new_tmp(name)
            self.local_types[var] = I64
            if name not in self.declared:
                stmts.append(ir.Local(sym=ir.Sym('local', name, self.f.qname, type=it_t), type=it_t, init=None, mods=[]))
                self.declared.add(name)
        v = ir.Name(sym=ir.Sym('local', var, self.f.qname, type=I64), type=I64)
        init = [ir.Local(sym=ir.Sym('local', var, self.f.qname, type=I64), type=I64, init=start, mods=[])]
        cond = ir.Binary(op='<' if step > 0 else '>', left=v, right=stop, type=BOOL)
        upd = ir.Unary(op='++', x=v, postfix=False, type=I64) if step == 1 else (
            ir.Unary(op='--', x=v, postfix=False, type=I64) if step == -1 else
            ir.Assign(op='+', target=v, value=lit(step, I64), type=I64))
        body = self.block(st.body)
        if not inner:
            body.stmts.insert(0, ir.ExprStmt(expr=ir.Assign(op=None, target=self.name_expr(name), value=v, type=I64)))
        stmts.append(ir.For(init=init, cond=cond, update=[upd], body=body, line=line))
        return stmts

    def pure(self, node, loop):
        """a range bound C++ may evaluate on every pass: a constant, or a local the loop does not store"""
        if isinstance(node, ast.Constant):
            return True
        if isinstance(node, ast.Name) and node.id in self.local_types:
            for n in ast.walk(loop):
                if isinstance(n, ast.Name) and n.id == node.id and isinstance(n.ctx, ast.Store):
                    return False
            return True
        return False

    def s_Try(self, st):
        body = self.block(st.body)
        catches = []
        for h in st.handlers:
            if h.type is None:
                types = [ir.Type('class', 'py.Exception')]
            else:
                names = h.type.elts if isinstance(h.type, ast.Tuple) else [h.type]
                types = []
                for n in names:
                    if isinstance(n, ast.Name) and self.ev.lookup_class(n):
                        types.append(ir.Type('class', self.ev.lookup_class(n), [], ref=False))
                    elif isinstance(n, ast.Name):
                        types.append(ir.Type('class', 'py.' + n.id))
                    else:
                        types.append(ir.Type('class', 'py.StructError' if ast.unparse(n) == 'struct.error' else 'py.' + n.attr))
            catches.append(ir.Catch(sym=None, types=types, body=self.block(h.body), line=self.line(h)))
        return ir.Try(resources=[], body=body, catches=catches, final=None, line=self.line(st))

    def s_With(self, st):
        out = []
        for item in st.items:
            v = self.expr(item.context_expr)
            if item.optional_vars is not None:
                out += self.as_list(self.store(item.optional_vars, v, st))
            else:
                out.append(ir.ExprStmt(expr=v))
        b = self.block(st.body)
        return [ir.Block(stmts=out + b.stmts)]

    def as_list(self, x):
        return x if isinstance(x, list) else [x]

    def s_Import(self, st):
        return []

    s_ImportFrom = s_Import

    # -- comprehensions
    def new_tmp(self, base):
        self.tmp += 1
        name = '%s_%d' % (base, self.tmp) if (base in self.local_types or base in self.ev.locals or self.tmp > 1) else base
        while name in self.local_types or name in self.ev.locals:
            self.tmp += 1
            name = '%s_%d' % (base, self.tmp)
        return name

    def empty(self, t):
        return pyfn('empty', [], t)

    def comp_into(self, st, tgt, comp):
        name = tgt.id
        t = self.local_types[name]
        out = []
        if name not in self.declared:
            out.append(ir.Local(sym=ir.Sym('local', name, self.f.qname, type=t), type=t, init=self.empty(t), mods=[]))
            self.declared.add(name)
        else:
            out.append(ir.ExprStmt(expr=ir.Assign(op=None, target=self.name_expr(name), value=self.empty(t), type=t)))
        out += self.comp_loops(comp, name, t)
        return out

    def comp_loops(self, comp, into, t):
        """the loops of a comprehension, adding each item to the local `into`"""
        acc = self.name_expr(into)

        def add():
            if isinstance(comp, ast.DictComp):
                k = self.expr(comp.key)
                v = self.coerce(self.expr(comp.value), t.args[1])
                return ir.ExprStmt(expr=ir.Assign(op=None, target=ir.Index(arr=acc, index=k, type=t.args[1]), value=v,
                                                  type=t.args[1]))
            e = self.coerce(self.expr(comp.elt), t.args[0])
            return ir.ExprStmt(expr=pyfn('list.append' if t.name == 'py.list' else 'set.add', [e], VOID, target=acc))
        return self.gen_loops(comp.generators, add)

    def gen_loops(self, gens, innermost):
        g = gens[0]
        saved = self.pre
        self.pre = []
        r = self.R(g.iter)
        iterable = self.expr(g.iter) if r != ('builtin', 'range') else None
        tgt = g.target
        names = [tgt.id] if isinstance(tgt, ast.Name) else [e.id for e in getattr(tgt, 'elts', []) if isinstance(e, ast.Name)]
        if not isinstance(tgt, ast.Name) and len(names) != len(getattr(tgt, 'elts', [])):
            raise Refused('syntax', 'comprehension target', self.line(tgt))
        elem_t = self.ty(self.T(tgt), tgt, 'comprehension item')
        for n in names:
            if n in self.local_types and n in self.declared and self.local_types[n] != elem_t:
                raise Refused('syntax', 'comprehension variable %s shadows a local of another type' % n, self.line(tgt))
        if isinstance(tgt, ast.Name):
            self.local_types[tgt.id] = elem_t
            var = ir.Local(sym=ir.Sym('local', tgt.id, self.f.qname, type=elem_t), type=elem_t, init=None, mods=[])
        else:
            for e, et in zip(tgt.elts, elem_t.args):
                self.local_types[e.id] = et
            var = ir.Local(sym=ir.Sym('local', ','.join(names), self.f.qname, type=elem_t), type=elem_t, init=None,
                           mods=['bind'])
        body_stmts = []
        if len(gens) > 1:
            body_stmts += self.gen_loops(gens[1:], innermost)
        else:
            self.pre = []
            st = innermost()
            body_stmts += self.pre + [st]
            self.pre = []
        for cond in reversed(g.ifs):
            self.pre = []
            c = self.cond(cond)
            if self.pre:
                raise Refused('syntax', 'comprehension condition needing statements', self.line(cond))
            body_stmts = [ir.If(cond=c, then=ir.Block(stmts=body_stmts), orelse=None)]
        loop = None
        if r == ('builtin', 'range') and isinstance(tgt, ast.Name):
            args = [self.expr(a) for a in g.iter.args]
            start, stop = (lit(0, I64), args[0]) if len(args) == 1 else (args[0], args[1])
            step = _int_const(g.iter.args[2]) if len(args) == 3 else 1
            v = self.name_expr(tgt.id)
            loop = ir.For(init=[ir.Local(sym=v.sym, type=I64, init=start, mods=[])],
                          cond=ir.Binary(op='<' if step > 0 else '>', left=v, right=stop, type=BOOL),
                          update=[ir.Unary(op='++', x=v, postfix=False, type=I64) if step == 1 else
                                  ir.Assign(op='+', target=v, value=lit(step, I64), type=I64)],
                          body=ir.Block(stmts=body_stmts))
        else:
            loop = ir.ForEach(var=var, iterable=iterable, body=ir.Block(stmts=body_stmts))
        out = self.pre + [loop]
        self.pre = saved
        return out

    def hoist_comp(self, node, kind):
        """a comprehension inside an expression: built into a temporary before the statement"""
        t = self.ty(self.T(node), node)
        name = self.new_tmp({'list': 'items', 'set': 'items', 'dict': 'table'}.get(kind, 'items'))
        self.local_types[name] = t
        self.declared.add(name)
        stmts = [ir.Local(sym=ir.Sym('local', name, self.f.qname, type=t), type=t, init=self.empty(t), mods=[])]
        stmts += self.comp_loops(node, name, t)
        self.pre.extend(stmts)
        return self.name_expr(name)

    def gen_as_list(self, gen):
        """a generator expression consumed whole (sum, join, sorted, list, set, min, max): its items as a list"""
        et = self.ty(self.T(gen.elt), gen.elt, 'generator item')
        t = ir.Type('class', 'py.list', [et])
        name = self.new_tmp('items')
        self.local_types[name] = t
        self.declared.add(name)
        acc = self.name_expr(name)
        stmts = [ir.Local(sym=acc.sym, type=t, init=self.empty(t), mods=[])]

        def add():
            return ir.ExprStmt(expr=pyfn('list.append', [self.coerce(self.expr(gen.elt), et)], VOID, target=acc))
        stmts += self.gen_loops(gen.generators, add)
        self.pre.extend(stmts)
        return acc

    def any_all(self, node, which):
        """any(gen) / all(gen): a loop that stops at the first item deciding it (Python's short-circuit)"""
        gen = node.args[0]
        if not isinstance(gen, ast.GeneratorExp):
            seq = self.expr(gen)
            return pyfn(which, [seq], BOOL)
        name = self.new_tmp('found' if which == 'any' else 'every')
        self.local_types[name] = BOOL
        self.declared.add(name)
        acc = self.name_expr(name)
        stmts = [ir.Local(sym=acc.sym, type=BOOL, init=lit(which == 'all', BOOL), mods=[])]

        def decide():
            c = self.cond(gen.elt)
            if which == 'all':
                c = ir.Unary(op='!', x=ir.Paren(x=c, type=BOOL), postfix=False, type=BOOL)
            return ir.If(cond=c, then=ir.Block(stmts=[
                ir.ExprStmt(expr=ir.Assign(op=None, target=acc, value=lit(which == 'any', BOOL), type=BOOL)),
                ir.Break(label=None)]), orelse=None)
        loops = self.gen_loops(gen.generators, decide)
        if len(gen.generators) > 1:
            raise Refused('syntax', '%s over nested loops (break leaves only the inner one)' % which, self.line(node))
        stmts += loops
        self.pre.extend(stmts)
        return acc

    # -- records
    def is_record_literal(self, node):
        t = self.ev.types.get(id(node))
        return t is not None and t.kind == 'dict' and self.p.is_record(t.site) and \
            (isinstance(node, ast.Dict) or (isinstance(node, ast.Call) and isinstance(node.func, ast.Name) and node.func.id == 'dict'))

    def record_builder(self, st, tgt):
        """`h = {...}` (then `h["k"] = v` ...): the record declared, then filled as Python fills it"""
        name = tgt.id
        t = self.local_types[name]
        value = self.expr(st.value)
        self.declared.add(name)
        return ir.Local(sym=ir.Sym('local', name, self.f.qname, type=t), type=t, init=value, mods=[], line=self.line(st))

    def record_value(self, node):
        """a dict display / dict(...) call that is a record: its fields in the struct's order"""
        t = self.T(node)
        rt = self.ty(t, node)
        s = self.p.find(t.site)
        fields = self.ctx.records.fields(s)
        given = []                                           # (key, lowered) in source order
        if isinstance(node, ast.Dict):
            for k, v in zip(node.keys, node.values):
                if k is None:
                    src = self.expr(v)
                    given += self.spread_fields(src, v)
                else:
                    given.append((_str_const(k), self.expr(v)))
        else:
            for a in node.args:
                src = self.expr(a)
                given += self.spread_fields(src, a)
            for kw in node.keywords:
                if kw.arg is None:
                    src = self.expr(kw.value)
                    given += self.spread_fields(src, kw.value)
                else:
                    given.append((kw.arg, self.expr(kw.value)))
        by_key = dict(given)
        order = [k for k, _ in given]
        field_order = [k for k, _ft, _r in fields]
        impure = [k for k, e in given if self.effects(e)]
        if order != [k for k in field_order if k in by_key] and len(impure) > 1:
            for k in order:                                  # keep Python's evaluation order
                e = by_key[k]
                if self.effects(e):
                    n = self.new_tmp(k)
                    self.local_types[n] = e.type
                    self.declared.add(n)
                    self.pre.append(ir.Local(sym=ir.Sym('local', n, self.f.qname, type=e.type), type=e.type, init=e, mods=[]))
                    by_key[k] = self.name_expr(n)
        args = []
        for k, ft, req in fields:
            want = self.ty(ft, node, 'field ' + k)
            if not req:
                want = ir.Type('class', 'py.optional', [want]) if not is_opt(want) else want
            if k in by_key:
                args.append(self.coerce(by_key[k], want))
            else:
                args.append(lit(None, want))
        return pyfn('record', args, rt, line=self.line(node), typeargs=[k for k, _f, _r in fields])

    def spread_fields(self, src, node):
        if not src.type.info.get('record'):
            raise Refused('syntax', '** of a dict', self.line(node))
        st = self.T(node)
        out = []
        for k, ft, req in self.ctx.records.fields(st.site):
            out.append((k, self.field_ref(src, k, node, self.line(node), raw=True)))
        return out

    def field_ref(self, obj, key, node, line, store=False, raw=False):
        t = self.T(node)
        s = self.p.find(t.site)
        info = dict((k, (ft, req)) for k, ft, req in self.ctx.records.fields(s))
        if key not in info:
            raise Refused('conflict', 'no field %r' % key, line)
        ft, req = info[key]
        ftt = self.ty(ft, node, 'field ' + key)
        if not req and (raw or store):
            ftt = ir.Type('class', 'py.optional', [ftt]) if not is_opt(ftt) else ftt
        sel = ir.Select(obj=obj, sym=ir.Sym('field', key, obj.type.name, type=ftt), type=ftt, line=line)
        if not req and not raw and not store:
            return pyfn('opt_value', [sel], ftt)                 # an absent key raises (KeyError in Python)
        return sel

    # -- expressions
    def cond(self, node):
        """an expression as a C++ condition (Python's truth)"""
        self.in_cond = node
        e = self.expr(node)
        return self.truthy(e)

    in_cond = None

    def truthy(self, e):
        if e.type == BOOL:
            return e
        return pyfn('truthy', [e], BOOL)

    def coerce(self, e, want):
        """a value where a `want` is expected: an optional unwrapped or wrapped, None typed"""
        if want is None or e.type == want:
            return e
        if isinstance(e, ir.Lit) and e.value is None:
            return lit(None, want)
        if is_opt(e.type) and not is_opt(want):
            if e.type.args[0] == want:
                return pyfn('unwrap', [e], want)
        if is_opt(want) and e.type == want.args[0]:
            return e
        if want.kind == 'class' and e.type.kind == 'class' and want.info.get('ref') and e.type.info.get('ref'):
            return e                                         # a subclass where its base is expected
        if want == I64 and e.type == BOOL:
            return ir.Cast(x=e, type=I64)
        if want.kind == 'class' and want.name == 'py.tuple' and e.type.kind == 'class' and e.type.name == 'py.tuple':
            return e
        if want.kind == 'class' and want.name in ('py.bytes', 'py.bytearray') and e.type.kind == 'class' \
                and e.type.name in ('py.bytes', 'py.bytearray'):
            return e
        return e

    def expr(self, node):
        m = getattr(self, 'e_' + type(node).__name__, None)
        if m is None:
            raise Refused('syntax', type(node).__name__.lower(), self.line(node))
        return m(node)

    def e_Constant(self, n):
        v = n.value
        t = self.T(n)
        if v is None:
            return lit(None, VOID)
        if isinstance(v, bool):
            return lit(v, BOOL)
        if isinstance(v, int):
            if not t.fits_i64():
                raise Refused('int-range', 'literal %d' % v, n.lineno)
            text = ast.get_source_segment(self.src, n)
            return lit(v, I64, text if text and re.fullmatch(r'[0-9a-fA-FxXoObB_]+', text) else None)
        if isinstance(v, float):
            return lit(v, F64, ast.get_source_segment(self.src, n))
        if isinstance(v, str):
            return lit(v, STR)
        if isinstance(v, bytes):
            return lit(v.decode('latin1'), BYTES)
        raise Refused('syntax', 'constant', n.lineno)

    def e_Name(self, n):
        r = self.R(n)
        t = self.T(n)
        if r is None or r[0] == 'local':
            name = n.id
            if self.f.kind in ('method', 'property') and name == self.f.params[0]:
                return ir.This(qualifier=None, type=ir.Type('class', self.f.cls.qname, [], ref=True), line=n.lineno)
            if name not in self.local_types:
                raise Refused('untyped', 'local ' + name, n.lineno)
            e = self.name_expr(name)
            flow = self.ty(t, n)
            return self.narrowed(e, flow)
        if r[0] == 'global':
            self.globals_used.add((r[1], r[2]))
            gt = self.ty(self.p.global_type(r[1], r[2]), n, 'global ' + r[2])
            e = ir.Select(obj=ir.TypeName(type=ir.Type('class', r[1])), sym=ir.Sym('global', r[2], r[1], True, type=gt),
                          type=gt, line=n.lineno)
            return e
        raise Refused('dynamic', '%s %s as a value' % (r[0], n.id), n.lineno)

    def narrowed(self, e, flow):
        if is_opt(e.type) and not is_opt(flow):
            if flow == VOID:
                return e
            return pyfn('unwrap', [e], flow)
        return e

    def e_Attribute(self, n):
        r = self.R(n)
        if r is None:
            raise Refused('dynamic', 'attribute .%s' % n.attr, n.lineno)
        if r[0] == 'field':
            obj = self.expr(n.value)
            ft = self.ctx.ty(self.p.attr_type(r[1], r[2]), n.lineno, 'attribute ' + r[2])
            return ir.Select(obj=obj, sym=ir.Sym('field', r[2], r[1], type=ft), type=ft, line=n.lineno)
        if r[0] == 'property':
            obj = self.expr(n.value)
            self.calls.add(r[1])
            t = self.ty(self.T(n), n)
            f = self.p.fns[r[1]]
            return ir.Call(target=obj, sym=ir.Sym('method', f.name, f.cls.qname, ret=t), args=[], typeargs=[], type=t,
                           line=n.lineno)
        if r[0] == 'global':
            self.globals_used.add((r[1], r[2]))
            gt = self.ty(self.p.global_type(r[1], r[2]), n, 'global ' + r[2])
            return ir.Select(obj=ir.TypeName(type=ir.Type('class', r[1])), sym=ir.Sym('global', r[2], r[1], True, type=gt),
                             type=gt, line=n.lineno)
        if r[0] == 'classattr':
            t = self.ty(self.p.class_attr_type(r[1], r[2]), n, 'class attribute ' + r[2])
            return ir.Select(obj=ir.TypeName(type=ir.Type('class', r[1])), sym=ir.Sym('field', r[2], r[1], True, type=t),
                             type=t, line=n.lineno)
        if r[0] == 'stdattr':
            t = self.ty(self.T(n), n)
            return pyfn(r[1], [], t, line=n.lineno)
        raise Refused('dynamic', 'attribute %s as a value' % n.attr, n.lineno)

    def e_Subscript(self, n):
        r = self.R(n) or ('?',)
        t = self.ty(self.T(n), n)
        obj = self.expr(n.value)
        if r[0] == 'slice':
            sl = n.slice
            lo = self.expr(sl.lower) if sl.lower is not None else lit(None, VOID)
            hi = self.expr(sl.upper) if sl.upper is not None else lit(None, VOID)
            args = [lo, hi]
            if sl.step is not None:
                args.append(self.expr(sl.step))
            return pyfn('slice', args, t, target=obj, line=n.lineno)
        if r[0] == 'tuple_slice':
            parts = obj.type.args[slice(r[1], r[2])]
            idxs = list(range(len(obj.type.args)))[slice(r[1], r[2])]
            if self.effects(obj):
                raise Refused('syntax', 'slice of a computed tuple', n.lineno)
            return pyfn('tuple', [pyfn('get', [obj], a, typeargs=[i]) for a, i in zip(parts, idxs)], t)
        if r[0] == 'tuple_index':
            if r[1] is None:
                raise Refused('syntax', 'computed tuple index', n.lineno)
            i = r[1] % len(obj.type.args)
            return pyfn('get', [obj], t, typeargs=[i], line=n.lineno)
        if r[0] == 'field_get':
            return self.field_ref(obj, r[1], n.value, n.lineno)
        idx = self.expr(n.slice)
        if r[0] == 'dict_get':
            return pyfn('dict.at', [idx], t, target=obj, line=n.lineno)
        return ir.Index(arr=obj, index=idx, type=t, line=n.lineno)

    def e_BinOp(self, n):
        a = self.expr(n.left)
        b = self.expr(n.right)
        return self.binop(n, n.op, a, b, self.T(n.left), self.T(n.right), self.T(n))

    _OPS = {ast.Add: '+', ast.Sub: '-', ast.Mult: '*', ast.Div: '/', ast.FloorDiv: '//', ast.Mod: '%',
            ast.LShift: '<<', ast.RShift: '>>', ast.BitAnd: '&', ast.BitOr: '|', ast.BitXor: '^', ast.Pow: '**'}

    def binop(self, n, op, a, b, ta, tb, tr):
        line = self.line(n)
        o = self._OPS[type(op)]
        rt = self.ty(tr, n)
        a = self.coerce(a, a.type)
        if is_opt(a.type) or is_opt(b.type):
            raise Refused('conflict', 'arithmetic on a value that may be None', line)
        ka, kb = ta.kind, tb.kind
        if ka == 'bool':
            ka, ta, a = 'int', P.INT(0, 1), ir.Cast(x=a, type=I64)
        if kb == 'bool':
            kb, tb, b = 'int', P.INT(0, 1), ir.Cast(x=b, type=I64)
        if ka == 'int' and kb == 'int':
            return self.int_op(n, o, a, b, ta, tb, tr, rt)
        if rt == F64:
            if o == '/':
                return pyfn('truediv', [a, b], F64, line=line)
            if o in ('//', '%', '**'):
                return pyfn({'//': 'floordiv', '%': 'mod', '**': 'pow'}[o], [a, b], F64, line=line)
            return ir.Binary(op=o, left=self.as_double(a), right=b, type=F64, line=line)
        if ka == 'str' and o == '+':
            parts = (a.parts if isinstance(a, ir.Concat) else [a]) + (b.parts if isinstance(b, ir.Concat) else [b])
            return ir.Concat(parts=parts, type=STR, line=line)
        if ka == 'str' and o == '%':
            vals = b.args if isinstance(b, ir.Call) and b.sym.name == 'tuple' and b.sym.owner == PY else [b]
            return pyfn('percent', [a] + vals, STR, line=line)
        if o == '*' and ka in ('str', 'bytes', 'list'):
            return pyfn('repeat', [a, b], rt, line=line)
        if o == '+':
            return pyfn('concat', [a, b], rt, line=line)
        if ka == kb == 'set':
            return pyfn('set' + o, [a, b], rt, line=line)
        raise Refused('conflict', '%s %s %s' % (ka, o, kb), line)

    def as_double(self, e):
        if e.type == I64:
            return ir.Cast(x=e, type=F64)
        return e

    def int_op(self, n, o, a, b, ta, tb, tr, rt):
        """an integer operation: C++'s own operator where the intervals show it agrees with Python's,
        else the support header's Python-exact one (and with int_policy 'checked', an overflow-checked one
        where the result is not shown to fit)"""
        line = self.line(n)
        checked = self.ctx.int_policy == 'checked' and not tr.fits_i64()
        if o in ('+', '-', '*'):
            if checked:
                return pyfn('checked_' + {'+': 'add', '-': 'sub', '*': 'mul'}[o], [a, b], I64, line=line)
            return ir.Binary(op=o, left=a, right=b, type=I64, line=line)
        if o in ('&', '|', '^'):
            return ir.Binary(op=o, left=a, right=b, type=I64, line=line)
        if o in ('//', '%'):
            if ta.nonneg() and tb.positive():
                return ir.Binary(op='/' if o == '//' else '%', left=a, right=b, type=I64, line=line)
            return pyfn('floordiv' if o == '//' else 'mod', [a, b], I64, line=line)
        if o == '<<':
            if not checked and ta.nonneg() and tb.nonneg() and tb.hi is not None and tb.hi < 63:
                return ir.Binary(op='<<', left=a, right=b, type=I64, line=line)
            return pyfn('checked_shl' if checked else 'shl', [a, b], I64, line=line)
        if o == '>>':
            if ta.nonneg() and tb.nonneg() and tb.hi is not None and tb.hi < 63:
                return ir.Binary(op='>>', left=a, right=b, type=I64, line=line)
            return pyfn('shr', [a, b], I64, line=line)
        if o == '**':
            return pyfn('checked_pow' if checked else 'ipow', [a, b], I64, line=line)
        if o == '/':
            return pyfn('truediv', [a, b], F64, line=line)
        raise Refused('conflict', 'int %s int' % o, line)

    def e_UnaryOp(self, n):
        x = self.expr(n.operand)
        if isinstance(n.op, ast.Not):
            return ir.Unary(op='!', x=self.paren(self.truthy(x)), postfix=False, type=BOOL, line=n.lineno)
        t = self.ty(self.T(n), n)
        if x.type == BOOL:
            x = ir.Cast(x=x, type=I64)
        if isinstance(n.op, ast.USub):
            if self.ctx.int_policy == 'checked' and t == I64 and not self.T(n).fits_i64():
                return pyfn('checked_neg', [x], I64)
            return ir.Unary(op='-', x=self.paren(x), postfix=False, type=t, line=n.lineno)
        if isinstance(n.op, ast.UAdd):
            return x
        if isinstance(n.op, ast.Invert):
            return ir.Unary(op='~', x=self.paren(x), postfix=False, type=t, line=n.lineno)
        raise Refused('syntax', 'unary', n.lineno)

    def paren(self, x):
        if isinstance(x, (ir.Binary, ir.Cond, ir.Assign)):
            return ir.Paren(x=x, type=x.type)
        return x

    def e_BoolOp(self, n):
        t = self.T(n)
        vals = n.values
        op = '&&' if isinstance(n.op, ast.And) else '||'
        r = self.R(n)
        if r and r[0] == 'mixedbool' and self.in_cond is not n:
            raise Refused('conflict', 'value of and/or over %s' % r[1], n.lineno)
        if t.kind == 'bool':
            saved = self.pre
            parts = []
            for i, v in enumerate(vals):
                self.pre = []
                c = self.cond(v)
                if self.pre and i > 0:
                    raise Refused('syntax', 'a lazy operand that needs statements', n.lineno)
                saved = saved + self.pre
                parts.append(c)
            self.pre = saved
            e = parts[0]
            for c in parts[1:]:
                e = ir.Binary(op=op, left=self.paren_bool(e, op), right=self.paren_bool(c, op), type=BOOL, line=n.lineno)
            return e
        # a value: `a or b` is a if a is true, else b (b only evaluated then)
        rt = self.ty(t, n)
        if len(vals) != 2:
            raise Refused('syntax', 'value of a chained and/or', n.lineno)
        a = self.expr(vals[0])
        saved = self.pre
        self.pre = []
        b = self.expr(vals[1])
        if self.pre or self.effects(b):
            raise Refused('syntax', 'value of and/or with a right side that has effects', n.lineno)
        self.pre = saved
        if is_opt(a.type) and not is_opt(rt):
            return pyfn('or_none' if op == '||' else 'and_none', [a, self.coerce(b, rt)], rt, line=n.lineno)
        return pyfn('or' if op == '||' else 'and', [self.coerce(a, rt), self.coerce(b, rt)], rt, line=n.lineno)

    def paren_bool(self, e, op):
        if isinstance(e, ir.Binary) and e.op in ('&&', '||') and e.op != op:
            return ir.Paren(x=e, type=BOOL)
        return e

    def e_Compare(self, n):
        left_node = n.left
        a = self.expr(left_node)
        parts = []
        for i, (op, c) in enumerate(zip(n.ops, n.comparators)):
            b = self.expr(c)
            if i < len(n.ops) - 1 and self.effects(b):
                raise Refused('syntax', 'chained comparison with a computed middle', n.lineno)
            parts.append(self.compare(n, op, a, b, left_node, c))
            a, left_node = b, c
        e = parts[0]
        for p_ in parts[1:]:
            e = ir.Binary(op='&&', left=e, right=p_, type=BOOL, line=n.lineno)
        return e

    def compare(self, n, op, a, b, an, bn):
        line = self.line(n)
        if isinstance(op, (ast.Is, ast.IsNot)) or (isinstance(op, (ast.Eq, ast.NotEq)) and
                                                   (isinstance(bn, ast.Constant) and bn.value is None)):
            pos = isinstance(op, (ast.Is, ast.Eq))
            x = a if not (isinstance(an, ast.Constant) and an.value is None) else b
            return pyfn('is_none' if pos else 'is_not_none', [x], BOOL, line=line)
        if isinstance(op, (ast.In, ast.NotIn)):
            neg = isinstance(op, ast.NotIn)
            if isinstance(bn, ast.Tuple) and not self.effects(a):
                parts = [ir.Binary(op='!=' if neg else '==', left=a, right=self.coerce(x, a.type), type=BOOL)
                         for x in b.args] if isinstance(b, ir.Call) and b.sym.name == 'tuple' else None
                if parts:
                    e = parts[0]
                    for x in parts[1:]:
                        e = ir.Binary(op='&&' if neg else '||', left=e, right=x, type=BOOL)
                    return ir.Paren(x=e, type=BOOL) if len(parts) > 1 else e
            bt = self.T(bn)
            if bt.kind == 'dict' and self.p.is_record(bt.site):
                key = _str_const(an)
                fields = dict((k, req) for k, _t, req in self.ctx.records.fields(bt.site))
                if key not in fields:
                    return lit(neg, BOOL)
                if fields[key]:
                    if self.effects(b):
                        raise Refused('syntax', 'membership in a computed record', line)
                    return lit(not neg, BOOL)
                sel = ir.Select(obj=b, sym=ir.Sym('field', key, b.type.name), type=ir.Type('class', 'py.optional', []))
                e = pyfn('is_not_none' if not neg else 'is_none', [sel], BOOL)
                return e
            e = pyfn('contains', [a], BOOL, target=b, line=line)
            return ir.Unary(op='!', x=e, postfix=False, type=BOOL) if neg else e
        o = {ast.Eq: '==', ast.NotEq: '!=', ast.Lt: '<', ast.LtE: '<=', ast.Gt: '>', ast.GtE: '>='}[type(op)]
        if is_opt(a.type) or is_opt(b.type):
            if o in ('==', '!='):
                return ir.Binary(op=o, left=a, right=b, type=BOOL, line=line)
            raise Refused('conflict', 'ordering a value that may be None', line)
        if a.type == BOOL and b.type == I64:
            a = ir.Cast(x=a, type=I64)
        if b.type == BOOL and a.type == I64:
            b = ir.Cast(x=b, type=I64)
        if {a.type.kind, b.type.kind} == {'i64', 'f64'}:
            a, b = self.as_double(a), self.as_double(b)
        return ir.Binary(op=o, left=a, right=b, type=BOOL, line=line)

    def e_IfExp(self, n):
        t = self.ty(self.T(n), n)
        c = self.cond(n.test)
        saved = self.pre
        self.pre = []
        a = self.coerce(self.expr(n.body), t)
        b = self.coerce(self.expr(n.orelse), t)
        if self.pre:
            raise Refused('syntax', 'a conditional branch that needs statements', n.lineno)
        self.pre = saved
        return ir.Cond(cond=c, then=a, orelse=b, type=t, line=n.lineno)

    def e_JoinedStr(self, n):
        parts = []
        for v in n.values:
            if isinstance(v, ast.Constant):
                if v.value:
                    parts.append(lit(v.value, STR))
                continue
            x = self.expr(v.value)
            if v.format_spec is not None:
                spec = ''.join(c.value for c in v.format_spec.values)
                parts.append(pyfn('format', [x, lit(spec, STR)], STR))
            elif v.conversion == 114:
                parts.append(pyfn('repr', [x], STR))
            elif x.type == STR:
                parts.append(x)
            else:
                parts.append(pyfn('str', [x], STR))
        if not parts:
            return lit('', STR)
        if len(parts) == 1:
            return parts[0] if parts[0].type == STR else pyfn('str', parts, STR)
        return ir.Concat(parts=parts, type=STR, line=n.lineno)

    def e_Tuple(self, n):
        t = self.ty(self.T(n), n)
        return pyfn('tuple', [self.expr(e) for e in n.elts], t, line=n.lineno)

    def e_List(self, n):
        t = self.ty(self.T(n), n)
        return pyfn('list', [self.coerce(self.expr(e), t.args[0]) for e in n.elts], t, line=n.lineno)

    def e_Set(self, n):
        t = self.ty(self.T(n), n)
        return pyfn('set', [self.coerce(self.expr(e), t.args[0]) for e in n.elts], t, line=n.lineno)

    def e_Dict(self, n):
        t = self.T(n)
        if self.p.is_record(t.site):
            return self.record_value(n)
        rt = self.ty(t, n)
        args = []
        for k, v in zip(n.keys, n.values):
            if k is None:
                raise Refused('syntax', '** in a dict', n.lineno)
            args.append(pyfn('tuple', [self.coerce(self.expr(k), rt.args[0]), self.coerce(self.expr(v), rt.args[1])],
                             ir.Type('class', 'py.tuple', rt.args)))
        return pyfn('dict', args, rt, line=n.lineno)

    def e_ListComp(self, n):
        return self.hoist_comp(n, 'list')

    def e_SetComp(self, n):
        return self.hoist_comp(n, 'set')

    def e_DictComp(self, n):
        return self.hoist_comp(n, 'dict')

    def e_GeneratorExp(self, n):
        raise Refused('syntax', 'generator expression as a value', n.lineno)

    def e_Lambda(self, n):
        raise Refused('dynamic', 'lambda', n.lineno)

    def lambda_expr(self, lam, ptypes):
        params = []
        for a, t in zip(lam.args.args, ptypes):
            self.local_types[a.arg] = t
            params.append(ir.Param(sym=ir.Sym('param', a.arg, self.f.qname, type=t), type=t, mods=[]))
        saved = self.pre
        self.pre = []
        body = self.expr(lam.body)
        if self.pre:
            raise Refused('syntax', 'a lambda that needs statements', lam.lineno)
        self.pre = saved
        return ir.Lambda(params=params, body=body, type=body.type)

    def effects(self, e):
        """whether evaluating `e` can do anything but read (a call, an assignment)"""
        for x in ir.walk(e):
            if isinstance(x, (ir.Assign, ir.New)):
                return True
            if isinstance(x, ir.Call):
                if x.sym.owner == PY and x.sym.name in ('tuple', 'get', 'unwrap', 'truthy', 'is_none', 'is_not_none',
                                                        'opt_value', 'list', 'str', 'empty', 'record', 'len'):
                    continue
                return True
            if isinstance(x, ir.Index):
                return True                          # can raise
        return False

    # -- calls
    def e_Call(self, n):
        r = self.R(n)
        line = n.lineno
        if r is None:
            raise Refused('dynamic', 'call', line)
        kind = r[0]
        t = self.T(n)
        if kind in ('fn', 'method', 'super', 'basecall'):
            f = self.p.fns[r[1]]
            self.calls.add(f.qname)
            args = self.call_args(n, f, bound=kind != 'fn' or f.kind == 'method')
            rt = self.call_ret(f, n)
            if kind == 'fn':
                if f.cls is not None and f.kind == 'static':
                    return ir.Call(target=ir.TypeName(type=ir.Type('class', f.cls.qname)),
                                   sym=ir.Sym('method', f.name, f.cls.qname, True, ret=rt), args=args, typeargs=[], type=rt,
                                   line=line)
                return ir.Call(target=None, sym=ir.Sym('function', f.name, f.module.name, True, ret=rt), args=args,
                               typeargs=[], type=rt, line=line)
            if kind in ('super', 'basecall'):
                if f.name == '__init__':
                    return ir.Call(target=None, sym=ir.Sym('ctor', '__init__', f.cls.qname), args=args, typeargs=['base'],
                                   type=VOID, line=line)
                return ir.Call(target=ir.TypeName(type=ir.Type('class', f.cls.qname)),
                               sym=ir.Sym('method', f.name, f.cls.qname, ret=rt), args=args, typeargs=['base'], type=rt,
                               line=line)
            recv = self.expr(n.func.value) if isinstance(n.func, ast.Attribute) else None
            if recv is not None and isinstance(recv, ir.Call) and recv.sym.owner == PY and recv.sym.name == 'unwrap':
                pass
            return ir.Call(target=recv, sym=ir.Sym('method', f.name, f.cls.qname, ret=rt), args=args, typeargs=[],
                           type=rt, line=line)
        if kind == 'ctor':
            cq = r[1]
            c = self.p.classes[cq]
            init = self.p.find_method(cq, '__init__')
            args = self.call_args(n, init, bound=True) if init else []
            if init:
                self.calls.add(init.qname)
            self.classes_made.add(cq)
            if c.exception:
                if not init:
                    args = [self.expr(a) for a in n.args]
                    if args and args[0].type != STR:
                        args = [pyfn('str', args, STR)]
                ct = ir.Type('class', cq, [], exception=True)
                return ir.New(sym=ir.Sym('ctor', c.name, cq), args=args, type=ct, memory='stack', line=line)
            ct = ir.Type('class', cq, [], ref=True)
            return ir.New(sym=ir.Sym('ctor', c.name, cq), args=args, type=ct, memory='gc', line=line)
        if kind == 'exc':
            args = [self.expr(a) for a in n.args]
            if args and args[0].type != STR:
                args = [pyfn('str', args, STR)]
            name = r[1].replace('struct.error', 'StructError')
            return ir.New(sym=ir.Sym('ctor', name, PY), args=args, type=ir.Type('class', 'py.' + name), line=line)
        if kind == 'builtin':
            return self.builtin_call(n, r[1], t)
        if kind == 'bmethod':
            return self.method_call(n, r[1], t)
        if kind == 'std':
            return self.std_call(n, r, t)
        if kind == 'isinstance':
            for a in n.args:
                if self.effects(self.expr(a)):
                    raise Refused('syntax', 'isinstance of a computed value', line)
            return lit(bool(r[1]), BOOL)
        raise Refused('dynamic', 'call (%s)' % kind, line)

    def call_ret(self, f, n):
        t = self.T(n)
        if f.name == '__init__':
            return VOID
        return self.ty(t, n) if t.kind != 'none' else VOID

    def call_args(self, n, f, bound):
        names = f.bound if bound or f.kind not in ('method', 'property') else f.params
        if f.kind == 'static':
            names = f.params
        given = {}
        if isinstance(n.func, ast.Attribute) and self.R(n) and self.R(n)[0] == 'basecall':
            pos = n.args[1:]
        else:
            pos = n.args
        for name, a in zip(names, pos):
            given[name] = a
        for k in n.keywords:
            given[k.arg] = k.value
        out = []
        for name in list(names) + f.kwonly:
            pt = self.ty(self.p.param_type(f, name), n, 'parameter ' + name)
            if name in given:
                out.append(self.coerce(self.expr(given[name]), pt))
            elif name in f.defaults:
                d = f.defaults[name]
                if not isinstance(d, ast.Constant):
                    raise Refused('syntax', 'a computed default of %s' % name, n.lineno)
                out.append(self.coerce(self.default_lit(d), pt))
            else:
                raise Refused('conflict', 'missing argument ' + name, n.lineno)
        return out

    def default_lit(self, d):
        v = d.value
        if v is None:
            return lit(None, VOID)
        if isinstance(v, bool):
            return lit(v, BOOL)
        if isinstance(v, int):
            return lit(v, I64)
        if isinstance(v, str):
            return lit(v, STR)
        if isinstance(v, bytes):
            return lit(v.decode('latin1'), BYTES)
        if isinstance(v, float):
            return lit(v, F64)
        raise Refused('syntax', 'default', 0)

    def builtin_call(self, n, name, t):
        line = n.lineno
        rt = self.ty(t, n) if t.kind != 'none' else VOID
        if name in ('any', 'all'):
            return self.any_all(n, name)
        args = []
        for a in n.args:
            if isinstance(a, ast.GeneratorExp):
                args.append(self.gen_as_list(a))
            else:
                args.append(self.expr(a))
        if name in ('sorted', 'list.sort'):
            key = next((k.value for k in n.keywords if k.arg == 'key'), None)
            rev = next((k.value for k in n.keywords if k.arg == 'reverse'), None)
            extra = []
            if key is not None:
                et = rt.args[0]
                if not isinstance(key, ast.Lambda):
                    raise Refused('dynamic', 'key function', line)
                extra.append(self.lambda_expr(key, [et]))
            else:
                extra.append(lit(None, VOID))
            if rev is not None:
                extra.append(self.truthy(self.expr(rev)))
            return pyfn('sorted', args + extra, rt, line=line)
        if name == 'dict':
            if self.is_record_literal(n):
                return self.record_value(n)
            raise Refused('syntax', 'dict() call', line)
        if name in ('min', 'max', 'min_iter', 'max_iter'):
            if 'default' in [k.arg for k in n.keywords]:
                raise Refused('syntax', '%s(default=)' % name, line)
            args = [self.coerce(a, rt) for a in args]
        if name == 'print':
            for k in n.keywords:
                args.append(pyfn('kw', [self.expr(k.value)], VOID, typeargs=[k.arg]))
        elif n.keywords and name not in ('enumerate',):
            raise Refused('syntax', '%s keywords' % name, line)
        if name == 'enumerate' and n.keywords:
            args.append(self.expr(n.keywords[0].value))
        return pyfn(name if name in ('sorted', 'enumerate', 'print', 'zip', 'reversed') else 'builtins.' + name, args, rt, line=line)

    def method_call(self, n, name, t):
        line = n.lineno
        rt = self.ty(t, n) if t.kind != 'none' else VOID
        recv = self.expr(n.func.value)
        if name in ('record.get',):
            key = _str_const(n.args[0])
            st = self.T(n.func.value)
            fields = dict((k, (ft, req)) for k, ft, req in self.ctx.records.fields(st.site))
            if key not in fields:
                return self.coerce(self.expr(n.args[1]), rt) if len(n.args) > 1 else lit(None, rt)
            ft, req = fields[key]
            sel = self.field_ref(recv, key, n.func.value, line, raw=True)
            if req:
                return self.coerce(sel, rt)
            if len(n.args) < 2 or (isinstance(n.args[1], ast.Constant) and n.args[1].value is None):
                return sel                                   # .get(k) of an optional field: the optional itself
            d = self.coerce(self.expr(n.args[1]), rt) if len(n.args) > 1 else lit(None, rt)
            return pyfn('value_or', [sel, d], rt, line=line)
        if name == 'list.sort':
            return self.builtin_call_sort(n, recv)
        args = []
        for a in n.args:
            if isinstance(a, ast.GeneratorExp):
                args.append(self.gen_as_list(a))
            else:
                args.append(self.expr(a))
        for k in n.keywords:
            args.append(pyfn('kw', [self.expr(k.value)], VOID, typeargs=[k.arg]))
        if name in ('list.append', 'list.insert', 'set.add') and recv.type.kind == 'class' and recv.type.args:
            args[-1] = self.coerce(args[-1], recv.type.args[0])
        if name in ('bytes.decode', 'str.encode'):
            enc = _str_const(n.args[0]) if n.args else next((_str_const(k.value) for k in n.keywords if k.arg == 'encoding'), 'utf-8')
            errs = _str_const(n.args[1]) if len(n.args) > 1 else next((_str_const(k.value) for k in n.keywords if k.arg == 'errors'), 'strict')
            enc = {'latin-1': 'latin1', 'iso-8859-1': 'latin1', 'utf8': 'utf-8'}.get(enc.lower(), enc.lower())
            return pyfn(name, [lit(enc, STR), lit(errs, STR)], rt, target=recv, line=line)
        return pyfn(name, args, rt, target=recv, line=line)

    def builtin_call_sort(self, n, recv):
        key = next((k.value for k in n.keywords if k.arg == 'key'), None)
        rev = next((k.value for k in n.keywords if k.arg == 'reverse'), None)
        args = []
        et = recv.type.args[0]
        if key is not None:
            if not isinstance(key, ast.Lambda):
                raise Refused('dynamic', 'key function', n.lineno)
            args.append(self.lambda_expr(key, [et]))
        else:
            args.append(lit(None, VOID))
        if rev is not None:
            args.append(self.truthy(self.expr(rev)))
        return pyfn('list.sort', args, VOID, target=recv, line=n.lineno)

    def std_call(self, n, r, t):
        line = n.lineno
        rt = self.ty(t, n) if t.kind != 'none' else VOID
        name = r[1]
        args = [self.expr(a) for a in n.args]
        if name.startswith('struct.'):
            fmt = r[2]
            if name in ('struct.unpack', 'struct.unpack_from'):
                return pyfn(name, args, rt, line=line, typeargs=[fmt])
            return pyfn(name, args, rt, line=line, typeargs=[fmt])
        if name == 'int.from_bytes':
            order = _str_const(n.args[1]) if len(n.args) > 1 else \
                _str_const(next((k.value for k in n.keywords if k.arg == 'byteorder'), None)) or 'big'
            signed = next((k.value.value for k in n.keywords if k.arg == 'signed'), False)
            return pyfn(name, [args[0], lit(order, STR), lit(bool(signed), BOOL)], rt, line=line)
        if n.keywords:
            raise Refused('syntax', name + ' keywords', line)
        return pyfn(name, args, rt, line=line)


# ------------------------------------------------------------------------------------------ modules
class ModuleResult(object):
    """One module: its L2 unit, what was translated, what was refused and why."""

    def __init__(self, name):
        self.name = name
        self.unit = None
        self.translated = {}             # fn qname -> ir.MethodDecl
        self.refused = {}                # fn qname -> Issue
        self.calls = {}                  # fn qname -> set of fn qnames it calls
        self.made = {}                   # fn qname -> classes it constructs
        self.globals = {}                # fn qname -> {(module, name)}
        self.global_decls = []           # ir.FieldDecl of the module's names the translation uses
        self.lines = {}                  # fn qname -> lines


def lower_functions(prog, ctx, fns):
    """{qname: (ir.MethodDecl | Refused, FnLower)} for the given functions"""
    out = {}
    for f in fns:
        try:
            ev = prog.evaluate(f)
        except Exception as e:
            out[f.qname] = (Refused('frontend-error', '%s: %s' % (type(e).__name__, e), f.node.lineno), None)
            continue
        fl = FnLower(ctx, f, ev)
        try:
            out[f.qname] = (fl.lower(), fl)
        except Refused as r:
            out[f.qname] = (r, fl)
        except RecursionError:
            out[f.qname] = (Refused('frontend-error', 'recursion', f.node.lineno), fl)
        except Exception as e:                            # a front end bug: refused, with the error
            out[f.qname] = (Refused('frontend-error', '%s: %s' % (type(e).__name__, e), f.node.lineno), fl)
    return out


def close_refusals(prog, lowered):
    """a function that calls (or constructs through) a refused function is refused too"""
    changed = True
    while changed:
        changed = False
        for q, (m, fl) in lowered.items():
            if isinstance(m, Refused) or fl is None:
                continue
            for c in sorted(fl.calls):
                got = lowered.get(c)
                if got is None:
                    lowered[q] = (Refused('calls-refused', '%s (not in the translation)' % c.rsplit('.', 1)[-1],
                                          m.line), fl)
                    changed = True
                    break
                if isinstance(got[0], Refused):
                    lowered[q] = (Refused('calls-refused', '%s (%s)' % (c.split('.', 2)[-1], got[0].issue.reason[:60]),
                                          m.line), fl)
                    changed = True
                    break
    return lowered
