"""cpp03.py -- the translator's C++03 back end: method bodies, constructors and initialisers from the IR.

The transpiler (codebreaker/java) writes the declarations: headers, signatures, fields. This back end
writes every body from the typed tree, in the transpiler's conventions -- the same TypeResolver decides
pointer or value, `s_` names statics, arrays are JArray<T>, Strings JavaString (a char per byte),
concatenation JCat -- so a body fits the declarations it is written into.

It declines rather than guess: anything it does not handle raises Decline, and the transpiler writes
an explicit, ledgered gap there (J2X-GAP: the body throws UnsupportedOperationException). Whether what
it writes is right is decided by the judge, class by class (probe.py).
"""
import re

from codebreaker.java import ir as jir
from codebreaker.java import typemap
from codebreaker.java.ledger import Category, Severity
from codebreaker.java.emit import naming, ownership
from codebreaker.translate import ir

# IR primitive kinds -> the Java names the transpiler's resolver keys on
_JAVA_PRIM = {'bool': 'boolean', 'i8': 'byte', 'i16': 'short', 'u16': 'char', 'i32': 'int', 'i64': 'long',
              'f32': 'float', 'f64': 'double', 'void': 'void'}
_CPP_KEYWORDS = frozenset('''alignas alignof and and_eq asm auto bitand bitor bool break case catch char char16_t
char32_t class compl const constexpr const_cast continue decltype default delete do double dynamic_cast else enum
explicit export extern false float for friend goto if inline int long mutable namespace new noexcept not not_eq
nullptr operator or or_eq private protected public register reinterpret_cast return short signed sizeof static
static_assert static_cast struct switch template this thread_local throw true try typedef typeid typename union
unsigned using virtual void volatile wchar_t while xor xor_eq NULL min max'''.split())


class Decline(Exception):
    """This back end does not write the method; the transpiler's text path does."""


def jtype(t):
    """An IR type as the transpiler's JType (simple names, as its resolver keys on them)."""
    if t is None:
        return jir.JType('void')
    n, base = t.dims()
    if base.kind in _JAVA_PRIM:
        name = _JAVA_PRIM[base.kind]
    elif base.kind == 'class':
        name = base.simple
    elif base.kind in ('typevar', 'wildcard'):
        name = 'Object'
    elif base.kind == 'null':
        name = 'Object'
    else:
        raise Decline('type %r' % (t,))
    args = [jtype(a) for a in base.args] if base.kind == 'class' else []
    return jir.JType(name, array_dims=n, args=args)


def _split_params(params):
    """'T a, const JavaString& b' -> [('T', 'a'), ('const JavaString&', 'b')]."""
    out, depth, cur = [], 0, ''
    for ch in params:
        if ch == ',' and depth == 0:
            out.append(cur)
            cur = ''
            continue
        depth += ch == '<'
        depth -= ch == '>'
        cur += ch
    if cur.strip():
        out.append(cur)
    res = []
    for p in out:
        p = p.strip()
        m = re.match(r'(.*?)([A-Za-z_]\w*)$', p)
        res.append((m.group(1).strip(), m.group(2)))
    return res


def is_pointer(ctype):
    return ctype.rstrip().endswith('*')


def _elem(ctype):
    """'JArray<Block* >' -> 'Block*'."""
    m = re.match(r'\s*JArray<(.*)>\s*$', ctype or '')
    if not m:
        raise Decline('index into %s' % ctype)
    return m.group(1).strip()


class Context(object):
    """What the bodies of one conversion share: the transpiler's resolver and parsed classes."""

    def __init__(self, resolver, classes):
        self.resolver = resolver
        self.classes = dict((c.name, c) for c in classes)
        self.qnames = set()                              # the program's classes by qualified name (Bodies)
        self.ledger = None                               # the conversion's ledger, when it keeps one

    def is_game(self, qualified):
        """A class of the program (not the library). By qualified name: the game's own `Container` is not
        java.awt.Container."""
        if self.qnames:
            return qualified in self.qnames
        return (qualified or '').rsplit('.', 1)[-1] in self.classes

    def old_ctor(self, simple, params):
        """The transpiler's parsed constructor of `simple` taking these IR parameter types."""
        c = self.classes.get(simple)
        if c is None:
            return None
        want = [jtype(p).name + '[]' * jtype(p).array_dims for p in params]
        for m in c.constructors():
            if [x.jtype.name + '[]' * x.jtype.array_dims for x in m.params] == want:
                return m
        return None

    def old_class(self, simple):
        return self.classes.get(simple)

    def old_method(self, owner, name, params):
        """The transpiler's parsed method `owner.name(params)` (IR param types), searching up the parents."""
        seen = set()
        while owner and owner not in seen:
            seen.add(owner)
            c = self.classes.get(owner)
            if c is None:
                return None
            want = [jtype(p).name + '[]' * jtype(p).array_dims for p in params]
            for m in c.methods:
                if m.name == name and not m.is_ctor and len(m.params) == len(params):
                    got = [p.jtype.name + '[]' * p.jtype.array_dims for p in m.params]
                    if got == want:
                        return m
            for m in c.methods:                          # erased generics / raw types: by arity
                if m.name == name and not m.is_ctor and len(m.params) == len(params):
                    return m
            owner = c.parent
        return None

    def field_ctype(self, owner, name):
        jt = self.resolver.member_field_type(owner, name)
        if jt is None:
            return None
        if jt.name == 'byte' and not jt.is_array and (owner, name) in typemap.UNSIGNED_BYTE_SCALAR_FIELDS:
            return 'unsigned char'
        return self.resolver.field_type(jt)


class Writer(object):
    """One method's body. `m` is the IR MethodDecl, `old` the transpiler's parsed method (for the
    parameter types its header declared)."""

    def __init__(self, ctx, cls, m, old):
        self.ctx = ctx
        self.r = ctx.resolver
        self.cls = cls                                   # ir.ClassDecl
        self.cname = cls.name
        self.m = m
        self.vars = {}                                   # name -> C++ type
        if old is not None:
            for ctype, name in _split_params(ownership.params_str(old, self.r)):
                self.vars[name] = ctype
        self.parent = cls.extends.simple if cls.extends is not None else None
        self.ret = ownership.return_cpp_type(old, self.r) if old is not None else 'void'
        self.two_phase = False                           # writing a j2xInit: `return;` is `return this;`
        self.old = old
        self.npe = 0                                     # inside a try whose catch a NullPointerException reaches
        self.cont_next = None                            # the label whose loop body is being written
        self.alias = {}                                  # a name written as something else (inlined ctors)
        self.loops = 0                                   # loops and switches around the statement
        self.fin = []                                    # enclosing finally-with-jumps: (n, loops)
        self.fin_count = 0
        self.leaks = 0                                   # _j2xleakN stack temporaries written
        self.after = []                                  # the statements after the one being written
        self.subst = {}                                  # id(expression) -> (code, C++ type) already written
        self.arg_temps = 0                               # _j2xarg_N owned temporaries hoisted
        # the no-GC ownership cleanups run in methods (as the transpiler's did), not constructors
        self.owning = old is not None and m is not None and getattr(m, 'kind', None) == 'method'
        self._owned_scalar = None

    # ------------------------------------------------------------------ types
    def ctype(self, t, context='local'):
        jt = jtype(t)
        if not jt.is_array and jt.name in self.r.values:
            raise Decline('value type %s' % jt.name)
        return self.r.cpp_type(jt, context)

    # ------------------------------------------------------------------ statements
    def body(self):
        if self.m.body is None:
            raise Decline('no body')
        out = []
        self.stmts(self.m.body.stmts, out, '')
        return out

    def stmts(self, ss, out, ind):
        for i, x in enumerate(ss):
            saved, self.after = self.after, ss[i + 1:]
            try:
                self.stmt(x, out, ind)
            finally:
                self.after = saved

    def block(self, s, out, ind):
        saved = dict(self.vars)
        if isinstance(s, ir.Block):
            self.stmts(s.stmts, out, ind + '\t')
        else:
            self.stmt(s, out, ind + '\t')
        self.vars = saved

    def loop_body(self, body, out, ind):
        """A loop's body; when the loop is labelled and continued by label, the body sits in its own block
        with `label_cont:` after it (so the goto crosses no initialisation)."""
        lab, self.cont_next = self.cont_next, None
        if lab is None:
            self.block(body, out, ind)
            return
        out.append(ind + '\t{')
        self.block(body, out, ind + '\t')
        out.append(ind + '\t}')
        out.append(ind + '\t%s_cont:;' % lab)

    def stmt(self, s, out, ind):
        loop = isinstance(s, (ir.While, ir.DoWhile, ir.For, ir.ForEach, ir.Switch))
        self.loops += loop
        try:
            self._stmt(s, out, ind)
        finally:
            self.loops -= loop

    def _stmt(self, s, out, ind):
        if isinstance(s, ir.Block):
            out.append(ind + '{')
            self.block(s, out, ind)
            out.append(ind + '}')
        elif isinstance(s, ir.Labeled):
            lab = s.label
            used = [n for n in ir.walk(s.body) if isinstance(n, (ir.Break, ir.Continue)) and n.label == lab]
            if any(isinstance(n, ir.Continue) for n in used):
                if not isinstance(s.body, (ir.While, ir.DoWhile, ir.For, ir.ForEach)):
                    raise Decline('continue to a label that is not a loop')
                self.cont_next = lab
            self.stmt(s.body, out, ind)
            if any(isinstance(n, ir.Break) for n in used):
                out.append(ind + '%s_brk:;' % lab)
        elif isinstance(s, ir.Sync):                     # as the text path: the lock is dropped, and marked
            out.append(ind + '/* J2X-REVIEW[synchronized-dropped]: was synchronized(%s) -- add a lock if this '
                       'state is touched by another thread (network/pool) */ {' % _comment(self.expr(s.lock)[0]))
            self.block(s.body, out, ind)
            out.append(ind + '}')
        elif isinstance(s, ir.Local):
            name = self.name(s.sym.name)
            if name in self.vars:
                raise Decline('shadowed local %s' % name)
            ct = self.ctype(s.type)
            self.vars[name] = ct
            if s.init is not None:
                self.review(s.init, out, ind)
                self.hoist_owned_args(s.init, out, ind)
                code, it = self.expr(s.init)
                out.append(ind + '%s %s = %s;' % (ct, name, unbox(coerce(code, it, ct), it, ct)))
                self.free_owned_return(s, ct, name, out, ind)
            else:
                out.append(ind + '%s %s;' % (ct, name))
        elif isinstance(s, ir.ExprStmt):
            self.review(s.expr, out, ind)
            self.hoist_owned_args(s.expr, out, ind)
            self.free_owned_reassign(s.expr, out, ind)
            out.append(ind + (self.kept_call(s.expr) or self.expr(s.expr)[0] + ';'))
        elif isinstance(s, ir.If):
            out.append(ind + 'if(%s) {' % self.cond(s.cond))
            self.block(s.then, out, ind)
            e = s.orelse
            while isinstance(e, ir.If):
                out.append(ind + '} else if(%s) {' % self.cond(e.cond))
                self.block(e.then, out, ind)
                e = e.orelse
            if e is not None:
                out.append(ind + '} else {')
                self.block(e, out, ind)
            out.append(ind + '}')
        elif isinstance(s, ir.While):
            out.append(ind + 'while(%s) {' % self.cond(s.cond))
            self.loop_body(s.body, out, ind)
            out.append(ind + '}')
        elif isinstance(s, ir.DoWhile):
            out.append(ind + 'do {')
            self.loop_body(s.body, out, ind)
            out.append(ind + '} while(%s);' % self.cond(s.cond))
        elif isinstance(s, ir.For):
            if len(s.init) > 1 and not all(isinstance(i, ir.Local) for i in s.init):
                raise Decline('for init')
            init = ''
            if s.init:
                if isinstance(s.init[0], ir.Local):
                    locs = s.init
                    ct = self.ctype(locs[0].type)
                    parts = []
                    for l in locs:
                        n = self.name(l.sym.name)
                        if n in self.vars:
                            raise Decline('shadowed loop variable %s' % n)
                        self.vars[n] = ct
                        parts.append('%s = %s' % (n, self.expr(l.init)[0]) if l.init is not None else n)
                    init = '%s %s' % (ct, ', '.join(parts))
                else:
                    init = ', '.join(self.expr(i.expr)[0] for i in s.init)
            cond = self.cond(s.cond) if s.cond is not None else ''
            upd = ', '.join(self.expr(u)[0] for u in s.update)
            out.append(ind + 'for (%s; %s; %s) {' % (init, cond, upd))
            self.loop_body(s.body, out, ind)
            out.append(ind + '}')
            for l in s.init:                              # the loop variable's scope ends with the loop
                if isinstance(l, ir.Local):
                    self.vars.pop(self.name(l.sym.name), None)
        elif isinstance(s, ir.Return) and self.fin:      # out through the finally
            n, _loops = self.fin[-1]
            if s.expr is not None:
                code, it = self.expr(s.expr)
                out.append(ind + 'j2xRet%d = %s;' % (n, unbox(coerce(code, it, self.ret), it, self.ret)))
            out.append(ind + 'j2xFin%d = 1; goto j2xFinally%d;' % (n, n))
        elif isinstance(s, (ir.Break, ir.Continue)) and self.fin and not s.label and self.loops == self.fin[-1][1]:
            n, _loops = self.fin[-1]
            out.append(ind + 'j2xFin%d = %d; goto j2xFinally%d;' % (n, 2 if isinstance(s, ir.Break) else 3, n))
        elif isinstance(s, ir.Return):
            if s.expr is not None:
                self.review(s.expr, out, ind)
                self.hoist_owned_args(s.expr, out, ind)
                code, it = self.expr(s.expr)
                out.append(ind + 'return %s;' % unbox(coerce(code, it, self.ret), it, self.ret))
            else:
                out.append(ind + ('return this;' if self.two_phase else 'return;'))
        elif isinstance(s, ir.Break):
            out.append(ind + ('goto %s_brk;' % s.label if s.label else 'break;'))
        elif isinstance(s, ir.Continue):
            out.append(ind + ('goto %s_cont;' % s.label if s.label else 'continue;'))
        elif isinstance(s, ir.Throw):
            out.append(ind + 'throw %s;' % self.expr(s.expr)[0])
        elif isinstance(s, ir.Switch):
            x, xt = self.expr(s.expr.x if isinstance(s.expr, ir.Paren) else s.expr)
            if 'JavaString' in xt:
                raise Decline('switch on a string')
            out.append(ind + 'switch(%s) {' % x)
            for c in s.cases:
                for lab in c.labels:
                    out.append(ind + 'case %s:' % self.expr(lab)[0])
                if c.default:
                    out.append(ind + 'default:')
                out.append(ind + '{')
                for b in c.body:
                    self.stmt(b, out, ind + '\t')
                out.append(ind + '}')
            out.append(ind + '}')
        elif isinstance(s, ir.Try):
            if s.resources:
                raise Decline('try-with-resources')
            if s.final is None:
                self.try_catch(s, out, ind)
            else:
                # finally: run on the way out by an exception (catch (...), then rethrow) and on the way out
                # normally (after the try). A return, break or continue out of the try would skip the second
                # copy, so such a try is declined until phase 2's exceptions pass.
                if _leaves([s.body] + [c.body for c in s.catches]):
                    self.finally_with_jumps(s, out, ind)
                    return
                out.append(ind + 'try {')
                self.try_catch(s, out, ind + '\t')
                out.append(ind + '} catch (...) {')
                self.block(s.final, out, ind)
                out.append(ind + '\tthrow;')
                out.append(ind + '}')
                out.append(ind + '{')
                self.block(s.final, out, ind)
                out.append(ind + '}')
        elif isinstance(s, ir.Empty):
            out.append(ind + ';')
        elif isinstance(s, ir.CtorCall):
            raise Decline('constructor call not first')
        else:
            raise Decline('statement %s' % type(s).__name__)

    def finally_with_jumps(self, s, out, ind):
        """A finally that a return, break or continue leaves through: the jump records what it was (and
        the value) and goes to the finally; after it, the recorded jump is made."""
        if self.fin:
            raise Decline('a finally with jumps inside another')
        if any(isinstance(x, (ir.Break, ir.Continue)) and x.label for b in [s.body] + [c.body for c in s.catches]
               for x in ir.walk(b)):
            raise Decline('a labelled jump out of a finally')
        self.fin_count += 1
        n = self.fin_count
        out.append(ind + '{')
        out.append(ind + '\tint j2xFin%d = 0;' % n)
        if self.ret != 'void' and not self.two_phase:
            out.append(ind + '\t%s j2xRet%d;' % (re.sub(r'^const\s+', '', self.ret).rstrip('&').strip(), n))
        self.fin.append((n, self.loops))
        try:
            out.append(ind + '\ttry {')
            self.try_catch(s, out, ind + '\t\t')
            out.append(ind + '\t} catch (...) {')
            self.block(s.final, out, ind + '\t')
            out.append(ind + '\t\tthrow;')
            out.append(ind + '\t}')
        finally:
            self.fin.pop()
        out.append(ind + '\tj2xFinally%d:;' % n)
        out.append(ind + '\t{')
        self.block(s.final, out, ind + '\t')
        out.append(ind + '\t}')
        # when the try and every catch end in a return or throw, the finally is always left by the return:
        # say so without a test, or the compiler sees a path that returns nothing (MSVC C4715)
        always = _abrupt(s.body) and all(_abrupt(c.body) for c in s.catches)
        cond = '' if always else 'if (j2xFin%d == 1) ' % n
        if self.two_phase:
            out.append(ind + '\t%sreturn this;' % cond)
        elif self.ret == 'void':
            out.append(ind + '\t%sreturn;' % cond)
        else:
            out.append(ind + '\t%sreturn j2xRet%d;' % (cond, n))
        if self.loops:
            out.append(ind + '\tif (j2xFin%d == 2) break;' % n)
            out.append(ind + '\tif (j2xFin%d == 3) continue;' % n)
        out.append(ind + '}')

    def try_catch(self, s, out, ind):
        """A try and its catches (no finally). Where a catch is one a NullPointerException reaches, every
        dereference in the try is null-checked to throw Java's exception (j2xNN), so C++ catches it where
        Java would, instead of crashing."""
        if not s.catches:
            out.append(ind + '{')
            self.block(s.body, out, ind)
            out.append(ind + '}')
            return
        npe = any(t.kind == 'class' and t.name in _CATCHES_NPE for c in s.catches for t in c.types)
        out.append(ind + 'try {')
        self.npe += npe
        try:
            self.block(s.body, out, ind)
        finally:
            self.npe -= npe
        for c in s.catches:
            if len(c.types) != 1:
                raise Decline('multi-catch')
            ct = self.ctype(c.types[0])
            name = self.name(c.sym.name)
            if name in self.vars:
                raise Decline('shadowed catch variable')
            self.vars[name] = ct
            used = any(isinstance(n, ir.Name) and n.sym.name == c.sym.name for n in ir.walk(c.body))
            out.append(ind + ('} catch (%s %s) {' % (ct, name) if used else '} catch (%s) {' % ct))   # C4101
            self.block(c.body, out, ind)
            self.vars.pop(name, None)
        out.append(ind + '}')

    def note(self, category, severity, rule, text):
        if self.ctx.ledger is not None:
            line = getattr(self.m, 'line', 0) or 0       # a static block or field initialiser has no method
            self.ctx.ledger.add(self.cname + '.cpp', category, text, line=line, severity=severity,
                                source='emit', rule=rule)

    def kept_call(self, e):
        """Two call statements written as the console needs them, or None:
          * `(new T(a)).m(b);` -- a heap temporary used once -- as a scoped stack object
            `{ T _j2xleakN(a); _j2xleakN.m(b); }` (Java's collector freed it; C++ leaked it every call:
            world-gen's `new WorldGenLakes(..).generate(..)`). Not a Thread's (it outlives the statement)
            nor start()/run();
          * `list[i].m(b);` on an array of objects -- a registry slot (Block.blocksList) can be empty --
            as `if (list[i] != NULL) { list[i]->m(b); }`.
        Inside a try that catches a NullPointerException neither: j2xNN throws there, as Java does."""
        while isinstance(e, ir.Paren):
            e = e.x
        if self.npe or not isinstance(e, ir.Call) or e.target is None or e.sym is None:
            return None
        t = e.target
        while isinstance(t, ir.Paren):
            t = t.x
        if isinstance(t, ir.New) and t.body is None and t.outer is None and t.type is not None \
                and t.type.kind == 'class' and self.ctx.is_game(t.type.name) and e.sym.name not in ('start', 'run'):
            name = jtype(t.type).name
            if name in self.r.thread_like or name in self.r.values or self.r.extends_value_container(name) \
                    or name not in self.r.known:
                return None
            call, _rt = self.expr(e)                     # the call as written, for its method and arguments
            m = re.match(r'\(?new %s\((.*?)\)\)?->(\w+)\((.*)\)$' % re.escape(name), call)
            if m is None:
                return None
            tmp = '_j2xleak%d' % self.leaks
            self.leaks += 1
            decl = '%s %s(%s);' % (name, tmp, m.group(1)) if m.group(1) else '%s %s;' % (name, tmp)
            self.note(Category.RULE, Severity.MED, 'leak-temp-stackified',
                      'leak fixed: throwaway `(new %s(...))->%s(...)` -> scoped stack object (Java GC temp '
                      'leaked every call)' % (name, m.group(2)))
            return '{ %s %s.%s(%s); }' % (decl, tmp, m.group(2), m.group(3))
        if isinstance(t, ir.Index):
            recv, rt = self.expr(t)
            if not is_pointer(rt):
                return None
            call, _rt = self.expr(e)
            self.note(Category.RISK, Severity.MED, 'null-deref-guard',
                      'null-deref guard inserted before list-indexed call (an unregistered/NULL slot would be '
                      'a virtual call through NULL -- undefined behaviour in C++)')
            return 'if (%s != NULL) { %s; }' % (recv, call)
        return None

    # -------------------------------------------------------------- no-GC ownership (Java's collector, by hand)
    def owned_scalar(self):
        """This class's owned scalar pointer members (ownership.owned_pointers)."""
        if self._owned_scalar is None:
            old_cls = self.ctx.classes.get(self.cname)
            owned = ownership.owned_pointers(old_cls, self.r) if old_cls is not None else {}
            self._owned_scalar = set(n for n, is_arr in owned.items() if not is_arr)
        return self._owned_scalar

    def free_owned_reassign(self, e, out, ind):
        """`this->m = new T(..)` (or a create*/make*/alloc*/clone* factory) on an OWNED scalar member: the
        previous object is deleted first -- Java's collector freed it; the 360 does not (Entity.bb / setPos).
        Every member is NULL in the constructor's init list, and delete of NULL does nothing."""
        while isinstance(e, ir.Paren):
            e = e.x
        if not self.owning or not isinstance(e, ir.Assign) or e.op is not None:
            return
        tgt = e.target
        if not (isinstance(tgt, ir.Select) and isinstance(tgt.obj, ir.This) and tgt.sym is not None
                and tgt.sym.name in self.owned_scalar()):
            return
        v = e.value
        while isinstance(v, (ir.Paren, ir.Cast)):
            v = v.x
        fresh = isinstance(v, ir.New) or (isinstance(v, ir.Call) and isinstance(v.target, ir.TypeName)
                                           and v.sym is not None and _FACTORY.match(v.sym.name))
        if fresh and not (out and out[-1].strip() == 'delete this->%s;' % tgt.sym.name):
            out.append(ind + 'delete this->%s;   // J2X no-gc-reassign-leak: free the previous owned %s'
                       % (tgt.sym.name, tgt.sym.name))

    def free_owned_return(self, s, ct, name, out, ind):
        """`JList<T*> x = builder(..)` where the builder hands back a container of fresh heap objects the
        caller frees (resolver.own_catalog.return_methods: Level.getCubes in Entity.move): an RAII guard
        frees the elements at scope exit -- only when every later use of x is transient (its size, or an
        element as the receiver of a call or a field read), so the delete can never dangle."""
        m = _OWNED_CONTAINER.match(ct)
        init = s.init
        while isinstance(init, ir.Paren):
            init = init.x
        if not self.owning or m is None or not isinstance(init, ir.Call) or init.sym is None \
                or init.sym.name not in self.r.own_catalog.return_methods:
            return
        if not all(_transient_use(chain) for chain in _uses(self.after, s.sym)):
            return
        out.append(ind + "J2X_ScopedFreeElems<%s<%s*> > _j2xfree_%s(%s);   // J2X no-gc-return-leak: free the "
                   "heap elements %s() handed back (caller-frees; Java GC'd them)"
                   % (m.group(1), m.group(2), name, name, init.sym.name))

    def hoist_owned_args(self, root, out, ind):
        """An owned-scalar builder used inline as an argument -- `sink.take(bb.expand(1))` -- leaks the
        temporary. It is hoisted to a local with an RAII delete, only when the receiving method is the
        program's own and provably just reads that argument position (own_catalog.retained_params)."""
        if not self.owning:
            return
        cat = self.r.own_catalog
        if not cat.scalar_methods:
            return
        for inner, outer, pos in _owned_args(root, cat):
            elem = cat.scalar_methods[inner.sym.name]
            code, ct = self.expr(inner)
            self.arg_temps += 1
            n = self.arg_temps
            out.append(ind + '%s* _j2xarg_%d = %s;' % (elem, n, code))
            out.append(ind + "J2X_ScopedDelete<%s > _j2xargd_%d(_j2xarg_%d);   // J2X no-gc-arg-leak: free the heap "
                       "%s that %s() returned (%s() only reads it)" % (elem, n, n, elem, inner.sym.name, outer.sym.name))
            self.subst[id(inner)] = ('_j2xarg_%d' % n, ct)

    def review(self, e, out, ind):
        """Reflection has no C++ equivalent -- the runtime's stubs return NULL -- so a statement using it is
        marked for a hand-written factory (J2X-REVIEW, and a HIGH ledger entry)."""
        for n in ir.walk(e):
            if isinstance(n, ir.Call) and n.sym is not None and n.sym.name in _REFLECTIVE \
                    and not self.ctx.is_game(n.sym.owner):
                out.append(ind + '// J2X-REVIEW[reflection-unsupported]: %s() has no C++ equivalent -- the '
                           'runtime stub returns NULL. Replace with a hand-written factory (a class-keyed '
                           'switch / registry), then null-check the result.' % n.sym.name)
                self.note(Category.RISK, Severity.HIGH, 'reflection-unsupported',
                          'reflection %s() has no C++ equivalent -- stub returns NULL; needs a hand-written '
                          'factory (J2X-REVIEW[reflection-unsupported])' % n.sym.name)
                return

    def cond(self, e):
        """A condition, without the parentheses Java writes around it (`if(x)`, as the text path: the console
        passes match that shape)."""
        while isinstance(e, ir.Paren):
            e = e.x
        return self.expr(e)[0]

    # ------------------------------------------------------------------ expressions -> (code, C++ type)
    def name(self, n):
        return n + '_' if n in _CPP_KEYWORDS else n

    def expr(self, e):
        if id(e) in self.subst:
            return self.subst[id(e)]
        f = getattr(self, 'x_' + type(e).__name__, None)
        if f is None:
            raise Decline('expression %s' % type(e).__name__)
        return f(e)

    def x_Paren(self, e):
        x, t = self.expr(e.x)
        return '(%s)' % x, t

    def x_Lit(self, e):
        v, k = e.value, e.type.kind
        if v is None:
            return 'NULL', 'void*'
        if k == 'bool':
            return ('true' if v else 'false'), 'bool'
        if k == 'i32':
            return (str(v) if v != -2147483648 else '(-2147483647 - 1)'), 'int'
        if k == 'i64':
            return ('%dLL' % v if v != -9223372036854775808 else '(-9223372036854775807LL - 1)'), 'long long'
        if k == 'f32':
            return _float(v, 'F'), 'float'
        if k == 'f64':
            return _float(v, ''), 'double'
        if k == 'u16':
            c = ord(v) if isinstance(v, str) else int(v)
            if c == _HOUSE:
                c = 127                                   # the font's ⌂, as in its strings
            if 32 <= c < 127 and chr(c) not in '\\\'':
                return "'%s'" % chr(c), 'unsigned char'
            if c > 255:                                   # past the runtime's 8-bit char: its value as an
                return str(c), 'int'                      # int (Java's char is 16 bits: '￿' is 65535)
            return '(unsigned char)%d' % c, 'unsigned char'
        if e.type.kind == 'class' and e.type.name == 'java.lang.String':
            return _cstring(v), 'const char*'
        raise Decline('literal %r' % (v,))

    def x_Name(self, e):
        n = self.name(e.sym.name)
        if n in self.alias:
            return self.alias[n], self.vars.get(n) or self.vars.get(self.alias[n], '')
        if n not in self.vars:
            raise Decline('unknown local %s' % n)
        return n, self.vars[n]

    def x_This(self, e):
        if e.qualifier is not None:
            raise Decline('qualified this')
        return 'this', self.cname + '*'

    def x_Select(self, e):
        s = e.sym
        owner = s.owner_simple
        if s.kind == 'enumconst':
            return '%s::%s' % (owner, naming.enum_member(s.name)), self.ctype(e.type)
        if isinstance(e.obj, ir.TypeName):
            if not self.ctx.is_game(s.owner):
                if s.owner == 'java.lang.Math' and s.name == 'PI':
                    return typemap.MATH_API['PI'], 'double'
                if owner in typemap.RUNTIME_ENUM_LIKE:     # Integer.MAX_VALUE, GL11.GL_BLEND, System.out
                    return '%s::%s' % (owner, s.name), self.library_field(owner, s.name, e.type)
                ct, spelled = _runtime_field(owner, s.name)    # a runtime class's static field, spelled
                if spelled is not None:                    # as its header has it (s_name or name)
                    return '%s::%s' % (owner, spelled), ct
                raise Decline('library static %s.%s' % (owner, s.name))
            ct = self.ctx.field_ctype(owner, s.name)
            if ct is None:
                raise Decline('static field %s.%s' % (owner, s.name))
            if owner in self.r.enums:                     # an enum's statics keep their bare names
                return '%s::%s' % (owner, s.name), ct
            return '%s::%s' % (owner, naming.static_field_name(s.name)), ct
        if s.static:
            raise Decline('static field through an instance')
        if isinstance(e.obj, (ir.This, ir.Super)):      # super.field is this object's inherited field
            recv, rt = 'this', self.cname + '*'
        else:
            recv, rt = self.expr(e.obj)
        base = _class_of(rt)
        if self.ctx.is_game(s.owner):
            ct = self.ctx.field_ctype(base or owner, s.name) or self.ctx.field_ctype(owner, s.name)
        else:
            ct, spelled = _runtime_field(owner, s.name)   # a library object's field: as the runtime has it
            if spelled is None and (_runtime_header(owner) or owner in typemap.VALUE_STDLIB):
                ct, spelled = self.ctype(e.type), s.name  # a runtime type that holds it inline
            if spelled != s.name:
                raise Decline('library field %s.%s' % (owner, s.name))
        if ct is None:
            raise Decline('field %s.%s' % (owner, s.name))
        return '%s%s%s' % (self.checked(recv, rt), '->' if is_pointer(rt) else '.', s.name), ct

    def arg(self, a):
        """An argument. A concatenation goes as a JavaString: as a JCat it converts to JavaString and to
        JObject, and a library constructor taking either is ambiguous (new File(dir + "/x"))."""
        code, ct = self.expr(a)
        return 'JavaString((%s)._s)' % code if ct == 'JCat' else code

    def unmarked(self, args):
        """The arguments without the decompiler's EmptyN marker ones, which the transpiler strips from
        declarations and calls alike."""
        def marker(a):
            t = a.type
            if isinstance(a, ir.Cast):
                t = a.type
            return t is not None and t.kind == 'class' and t.simple in self.r.markers
        return [a for a in args if not marker(a)]

    def checked(self, recv, rt):
        """A pointer receiver, null-checked when a NullPointerException here would be caught."""
        if self.npe and is_pointer(rt) and recv != 'this' and rt not in _REFLECTION:
            return 'j2xNN<NullPointerException>(%s)' % recv
        return recv

    def x_Length(self, e):
        c = e.arr
        if isinstance(c, ir.Call) and c.sym is not None and c.sym.name == 'values' and not c.args                 and isinstance(c.target, ir.TypeName) and c.sym.owner_simple in self.r.enums                 and c.sym.owner_simple not in self.r.typed_enums:
            return '%s::_J2X_COUNT' % c.sym.owner_simple, 'int'   # a constant-only enum's size
        a, _t = self.expr(e.arr)
        return '%s.length' % a, 'int'

    def x_Index(self, e):
        a, t = self.expr(e.arr)
        i, _it = self.expr(e.index)
        return '%s[%s]' % (a, i), _elem(t)

    def x_Unary(self, e):
        x, t = self.expr(e.x)
        if e.postfix:
            return '%s%s' % (x, e.op), t
        return '%s%s' % (e.op, x), (t if e.op in ('++', '--') else self.ctype(e.type))

    def x_Binary(self, e):
        l, lt = self.expr(e.left)
        r, rt = self.expr(e.right)
        if e.op in _BITS:                                 # `a >> 16 & 255` is the same in Java and C++,
            if isinstance(e.left, ir.Binary) and e.left.op != e.op:    # but MSVC warns (C4554): group it
                l = '(%s)' % l
            if isinstance(e.right, ir.Binary) and e.right.op != e.op:
                r = '(%s)' % r
        if e.op == '>>>':
            k = e.type.kind
            # static_cast, not a C cast: the transpiler's finalize pass turns `(int)(...)` into the
            # saturating float->int helper, and a shifted bit pattern must wrap, not saturate
            if k == 'i32':
                return 'static_cast<int>(static_cast<unsigned int>(%s) >> (%s))' % (l, r), 'int'
            if k == 'i64':
                return 'static_cast<long long>(static_cast<unsigned long long>(%s) >> (%s))' % (l, r), 'long long'
            raise Decline('>>> on %s' % k)
        # (a String == String compares the text, as the text path does: identity has no meaning for the
        # by-value JavaString)
        if e.op in ('==', '!=') and lt != rt and ('JObject' in lt or 'JObject' in rt) and 'void*' not in (lt, rt):
            eq = 'j2xIdentEq(%s, %s)' % (l, r)            # identity across a box, as the text path
            return (eq if e.op == '==' else '!' + eq), 'bool'
        if e.op == '%' and e.type.kind in ('f32', 'f64'):     # Java's floating remainder: C's fmod
            fmt = '(float)fmod((double)(%s), (double)(%s))' if e.type.kind == 'f32' else 'fmod(%s, %s)'
            return fmt % (l, r), self.ctype(e.type)
        return '%s %s %s' % (l, e.op, r), self.ctype(e.type)

    def x_Concat(self, e):
        parts = []
        for p in e.parts:
            x, _t = self.expr(p)
            parts.append(x)
        return '(JCat() + %s)' % ' + '.join(parts), 'JCat'

    def x_Assign(self, e):
        t, tt = self.expr(e.target)
        v, _vt = self.expr(e.value)
        if e.op is None:
            return '%s = %s' % (t, unbox(coerce(v, _vt, tt), _vt, tt)), tt
        if e.op == '>>>':
            k = e.target.type.kind
            plain = isinstance(e.target, ir.Name) or (isinstance(e.target, ir.Select)
                                                      and isinstance(e.target.obj, (ir.This, ir.TypeName)))
            if k not in ('i32', 'i64') or not plain:        # evaluated twice below: no side effects allowed
                raise Decline('>>>=')
            u = 'unsigned int' if k == 'i32' else 'unsigned long long'
            return '%s = static_cast<%s>(static_cast<%s>(%s) >> (%s))' % (t, 'int' if k == 'i32' else 'long long', u, t, v), tt
        if e.op == '%' and e.target.type.kind in ('f32', 'f64'):
            raise Decline('floating %=')
        return '%s %s= %s' % (t, e.op, v), tt

    def x_Cond(self, e):
        c = self.cond(e.cond)
        a, at = self.expr(e.then)
        b, bt = self.expr(e.orelse)
        if e.type.kind == 'null':
            return '(%s ? %s : %s)' % (c, a, b), at
        t = self.ctype(e.type)
        if at != bt:                                      # Java converts both arms to the result type
            a, b = self.arm(a, at, t), self.arm(b, bt, t)
        return '(%s ? %s : %s)' % (c, a, b), t

    def arm(self, code, ctype, t):
        if code == 'NULL':
            return null_of(t)
        if ctype == t:
            return code
        if t == 'JavaString':
            return 'JavaString((%s)._s)' % code if ctype == 'JCat' else 'JavaString(%s)' % code
        return '(%s)(%s)' % (t, code)

    def x_Cast(self, e):
        x, xt = self.expr(e.x)
        if xt.startswith('JObject') and e.type.kind == 'class' and e.type.simple in typemap.BOXED_VALUE_TYPES:
            return x, 'JObject'                           # (Integer)o stays boxed: o.intValue() is JObject's
        ct = self.ctype(e.type)
        if ct in _SATURATING:                             # Java's (int)/(long) of a float saturates; C++'s is
            return '%s(%s)' % (_SATURATING[ct], x), ct    # undefined out of range (JavaRT.h's helpers)
        if e.type.is_prim and not _SIMPLE.match(x):       # grouped
            return '(%s)(%s)' % (ct, x), ct
        return '(%s)%s' % (ct, x), ct                     # as the text path: the reflection pass reads
                                                          # `(T*)recv->newInstance()`

    def x_InstanceOf(self, e):
        x, xt = self.expr(e.x)
        target = jtype(e.test)
        if target.is_array and target.array_dims == 1 and target.name == 'String' and xt.startswith('JObject'):
            return '(%s).isStringArray()' % x, 'bool'
        if not target.is_array and target.name in _BOXED_TEST:
            return self.boxed_instanceof(x, xt, target.name)
        if target.is_array or not self.r.is_pointer(target):
            raise Decline('instanceof %s' % target.name)
        if is_pointer(xt):
            return '(dynamic_cast<%s*>(%s) != NULL)' % (target.name, x), 'bool'
        if xt.startswith('JObject'):
            return '(%s).is<%s>()' % (x, target.name), 'bool'
        raise Decline('instanceof on %s' % xt)

    def boxed_instanceof(self, x, xt, name):
        if xt.startswith('JObject') and name in _BOXED_TEST:
            return '(%s).%s()' % (x, _BOXED_TEST[name]), 'bool'
        raise Decline('instanceof %s' % name)

    def x_Call(self, e):
        s = e.sym
        if s is None or s.kind != 'method':
            raise Decline('call %s' % (s.kind if s is not None else 'unresolved'))
        owner = s.owner_simple
        e.args = self.unmarked(e.args)                    # the decompiler's EmptyN bridge arguments
        if owner in typemap.BOXED_VALUE_TYPES and s.name.endswith('Value') and not s.params \
                and e.target is not None and not isinstance(e.target, ir.TypeName):
            recv, rt = self.expr(e.target)                # Integer.intValue(): a boxed type is a primitive
            if not rt.startswith('JObject'):              # here, unless it is still in its JObject box
                ct = self.ctype(e.type)
                return '((%s)(%s))' % (ct, recv), ct
        if owner in typemap.BOXED_VALUE_TYPES and s.name == 'equals' and len(e.args) == 1                 and e.target is not None and not isinstance(e.target, ir.TypeName):
            recv, rt = self.expr(e.target)                # Integer.equals(Integer): the primitives' values
            if not rt.startswith('JObject'):
                return '(%s == %s)' % (recv, self.expr(e.args[0])[0]), 'bool'
        if s.name == 'ordinal' and not s.params and e.target is not None:
            recv, rt = self.expr(e.target)
            if rt == 'int':                               # a constant-only enum is its ordinal
                return '(%s)' % recv, 'int'
        if s.owner == 'java.lang.Object':
            return self.object_method(e)
        if s.name == 'values' and not e.args and isinstance(e.target, ir.TypeName) and owner in self.r.typed_enums:
            return '%s::values()' % owner, self.ctype(e.type)  # the enum's own table
        if s.varargs:
            args = self.varargs(e, owner)
        else:
            args = ', '.join(self.arg(a) for a in e.args)
        game = self.ctx.is_game(s.owner)
        old = self.ctx.old_method(owner, s.name, s.params) if game else None
        if game:
            if old is None:
                raise Decline('method %s.%s not found' % (owner, s.name))
            ret = ownership.return_cpp_type(old, self.r)
            name = self.r.renamed_method(owner, s.name)
        else:
            ret = self.library_return(owner, s.name, s.ret)
            name = self.name(s.name)                      # File.delete() -> delete_(), as the runtime has it
        t = e.target
        if t is None:
            return 'this->%s(%s)' % (name, args), ret
        if isinstance(t, ir.TypeName):
            if not game:
                if s.owner == 'java.lang.Math':
                    fn = _MATH.get(s.name)
                    if fn is None:
                        raise Decline('Math.%s' % s.name)
                    return '%s(%s)' % (fn, args), self.ctype(e.type)
                holder = t.type.simple if t.type is not None and t.type.kind == 'class' else owner
                if holder not in typemap.RUNTIME_HEADERS and holder not in typemap.JAVART_STATICS:
                    raise Decline('library static call %s.%s' % (holder, s.name))
                return '%s::%s(%s)' % (holder, name, args), ret   # as written: ARBVertexBufferObject::
            return '%s::%s(%s)' % (owner, name, args), ret
        if isinstance(t, ir.Super):
            if not self.parent:
                raise Decline('super call without a parent')
            return '%s::%s(%s)' % (self.parent, name, args), ret
        if isinstance(t, ir.This):
            return 'this->%s(%s)' % (name, args), ret
        recv, rt = self.expr(t)
        if rt == 'const char*':
            recv, rt = 'JavaString(%s)' % recv, 'JavaString'
        return '%s%s%s(%s)' % (self.checked(recv, rt), '->' if is_pointer(rt) else '.', name, args), ret

    def object_method(self, e):
        """java.lang.Object's own methods, as the transpiler lowers them."""
        s, t = e.sym, e.target
        if t is None or isinstance(t, ir.This):
            recv, rt = 'this', self.cname + '*'
        else:
            recv, rt = self.expr(t)
        if s.name == 'getClass' and not e.args:
            if rt.startswith('JObject'):                  # a box: the class of the object it holds
                acc = '->' if is_pointer(rt) else '.'
                return '((%s)%sti ? j2xClassOf(*(%s)%sti) : (Class*)NULL)' % (recv, acc, recv, acc), 'Class*'
            if not is_pointer(rt):
                raise Decline('getClass on %s' % rt)
            return 'j2xClassOf(typeid(*%s))' % self.checked(recv, rt), 'Class*'
        if s.name == 'equals' and len(e.args) == 1 and is_pointer(rt):
            other, ot = self.expr(e.args[0])            # Object.equals is identity
            if is_pointer(ot) and ot == rt or other == 'NULL':
                return '(%s == %s)' % (recv, other), 'bool'
            return 'j2xIdentEq(%s, %s)' % (recv, other), 'bool'
        if s.name == 'equals' and len(e.args) == 1 and rt.startswith('JObject') and not is_pointer(rt):
            return '(%s).equals(%s)' % (recv, self.expr(e.args[0])[0]), 'bool'   # the box's own equals
        if s.name == 'toString' and not e.args and is_pointer(rt):
            return '%s->toString()' % self.checked(recv, rt), 'JavaString'
        raise Decline('Object method %s' % s.name)

    def varargs(self, e, owner):
        """A varargs call's arguments: the trailing ones packed into the array the declaration takes."""
        s = e.sym
        if not self.ctx.is_game(s.owner):
            return self.library_varargs(e)
        old = self.ctx.old_method(owner, s.name, s.params)
        if old is None:
            raise Decline('method %s.%s not found' % (owner, s.name))
        last = _split_params(ownership.params_str(old, self.r))[-1][0]
        fixed = len(s.params) - 1
        args = [self.arg(a) for a in e.args]
        if len(e.args) == len(s.params) and e.args[-1].type is not None and e.args[-1].type.kind in ('array', 'null'):
            return ', '.join(args)                      # the array itself was passed
        rest = args[fixed:]
        elem = _elem(last.replace('const ', '').rstrip('&').strip())
        packed = ('(JArrayInit<%s >(), %s)' % (elem, ', '.join(rest)) if rest else 'JArray<%s >()' % elem)
        return ', '.join(args[:fixed] + [packed])

    def library_varargs(self, e):
        """A library varargs call (String.format, Arrays.asList, Constructor.newInstance): the trailing
        arguments packed into the array type the Java declaration takes, as the text path writes it."""
        s = e.sym
        last = s.params[-1] if s.params else None
        if last is None or last.kind != 'array':
            raise Decline('library varargs %s.%s' % (s.owner_simple, s.name))
        elem = _elem(self.ctype(last))
        fixed = len(s.params) - 1
        args = [self.arg(a) for a in e.args]
        if len(e.args) == len(s.params) and e.args[-1].type is not None and e.args[-1].type.kind in ('array', 'null'):
            return ', '.join(args)
        rest = args[fixed:]
        packed = ('(JArrayInit<%s >(), %s)' % (elem, ', '.join(rest)) if rest else 'JArray<%s >()' % elem)
        return ', '.join(args[:fixed] + [packed])

    def library_field(self, owner, name, t):
        if owner == 'System' and name in ('out', 'err'):
            return 'System::_PrintStream'                 # a value in the runtime (JavaRT.h)
        return self.ctype(t)

    def library_return(self, owner, name, ret):
        """The C++ return type of a runtime (library) method: what the runtime's own stub registry says,
        else the resolver's mapping of the Java type."""
        from codebreaker.java import runtime
        base = re.sub(r'<.*', '', self.r.base_cpp(owner))
        declared = _runtime_return(owner, name)          # what the runtime's own header declares, first
        if declared:
            return declared
        rec, seen = runtime.STUB_MEMBERS.get(base) or runtime.STUB_MEMBERS.get(owner), set()
        while rec is not None and id(rec) not in seen:
            seen.add(id(rec))
            t = rec['methods'].get(name)
            if t:
                return 'JavaString' if t == 'std::string' else t
            rec = runtime.STUB_MEMBERS.get(rec.get('parent'))
        if name == 'iterator':
            return 'Iterator*'
        if ret is not None and ret.kind == 'class' and ret.simple == owner and owner.endswith('Buffer'):
            return owner + '&'                            # NIO buffers chain by reference (put().put())
        if ret is None or ret.kind == 'void':
            return 'void'
        return self.ctype(ret, 'return')

    def x_ClassLit(self, e):
        t = e.of
        if t is None or t.kind != 'class':
            raise Decline('class literal of %r' % (t,))
        return 'j2xClassOf(typeid(%s))' % _CLASS_LIT_TYPE.get(t.simple, t.simple), 'Class*'

    def x_New(self, e):
        if e.body is not None or e.outer is not None:
            raise Decline('anonymous or inner class')
        t = e.type
        jt = jtype(t)
        if jt.name in self.r.values:
            raise Decline('value type %s' % jt.name)
        e.args = self.unmarked(e.args)
        args = ', '.join(self.arg(a) for a in e.args)
        if t.kind == 'class' and self.ctx.is_game(t.name):
            if self.r.extends_value_container(jt.name):   # a container subclass IS the container value
                return '(*(new %s(%s)))' % (jt.name, args), jt.name
            return 'new %s(%s)' % (jt.name, args), jt.name + '*'
        ct = self.r.cpp_type(jt, 'local')
        if jt.name in typemap.BOXED_VALUE_TYPES:          # new Integer(5): a boxed type is its primitive
            return '((%s)(%s))' % (ct, args), ct
        if is_pointer(ct):
            return 'new %s(%s)' % (ct[:-1].strip(), args), ct
        return '%s(%s)' % (ct, args), ct

    def x_NewArray(self, e):
        ct = self.ctype(e.type)
        if e.init is not None:
            if e.dims:
                raise Decline('sized initialised array')
            return '(JArrayInit<%s >(), %s)' % (_elem(ct), ', '.join(self.expr(x)[0] for x in e.init)), ct
        total = e.type.dims()[0]
        dims = [self.expr(d)[0] for d in e.dims]
        if len(dims) == 1:                                # new T[n], or jagged new T[n][]: n empty rows
            return '%s(%s)' % (ct, dims[0]), ct
        if len(dims) == total and total in (2, 3):        # new T[a][b]([c]): the runtime's nested builders
            inner = ct
            for _ in range(total):
                inner = _elem(inner)
            return 'j2x_newarr%d<%s >(%s)' % (total, inner, ', '.join(dims)), ct
        raise Decline('multi-dimensional array')


# java.lang.Math as C: the transpiler's table, but each function at Java's own precision (double),
# where the transpiler has the float forms (floorf, powf, ...) for the 360's float pipeline
_MATH = dict(typemap.MATH_API_EMIT, floor='floor', ceil='ceil', pow='pow', exp='exp', log='log')
_MATH.pop('PI', None)


_BITS = frozenset(('<<', '>>', '&', '|', '^'))
_SATURATING = {'int': 'j2xToInt', 'long long': 'j2xToLong'}
_HOUSE = 0x2302                                      # U+2302, Minecraft's font tile 127
_SIMPLE = re.compile(r'^[A-Za-z_][\w]*(?:(?:->|\.)[A-Za-z_]\w*)*$')   # a name or member chain: no grouping
_REFLECTION = frozenset(('Class*', 'Constructor*'))   # left as written: the reflection pass rewrites them
_REFLECTIVE = frozenset(('newInstance', 'forName', 'getConstructor', 'getDeclaredConstructor'))
_BOXED_TEST = {'String': 'isString', 'Integer': 'isInteger', 'Character': 'isCharacter', 'Boolean': 'isBoolean'}
# X.class for a boxed or built-in type: the C++ type it is (as the transpiler's statement layer has it)
_CLASS_LIT_TYPE = {'Byte': 'signed char', 'Short': 'short', 'Integer': 'int', 'Long': 'long long',
                   'Float': 'float', 'Double': 'double', 'Character': 'char', 'Boolean': 'bool',
                   'String': 'JavaString', 'Object': 'JObject'}


_OWNED_CONTAINER = re.compile(r'^(JList|JSet)\s*<\s*([A-Za-z_][\w:]*)\s*\*\s*>$')
_FACTORY = re.compile(r'^(?:create|make|alloc|clone)[A-Za-z]')


def _with_parents(node, chain=()):
    """(node, its ancestors nearest first) for every node under `node`."""
    yield node, chain
    for c in node.children():
        for x in _with_parents(c, (node,) + chain):
            yield x


def _uses(stmts, sym):
    """The ancestor chains of every reference to the local `sym` in `stmts`."""
    for s in stmts:
        for n, chain in _with_parents(s):
            if isinstance(n, ir.Name) and n.sym is not None and n.sym.name == sym.name:   # by name: a
                                                                    # lowered use has its own Symbol
                yield chain


def _transient_use(chain):
    """A container use that lets no element outlive it: x.size() / x.isEmpty(), or x.get(i) used only as
    the receiver of a call or a field read (through casts and parentheses)."""
    if not chain or not isinstance(chain[0], ir.Call) or chain[0].sym is None:
        return False
    name = chain[0].sym.name
    if name in ('size', 'isEmpty'):
        return True
    if name != 'get':
        return False
    k = 1
    while k < len(chain) and isinstance(chain[k], (ir.Paren, ir.Cast)):
        k += 1
    if k >= len(chain):
        return False
    up, below = chain[k], chain[k - 1]
    return (isinstance(up, ir.Call) and up.target is below) or (isinstance(up, ir.Select) and up.obj is below)


def _owned_args(root, cat):
    """(inner call, outer call, argument position) for each owned-scalar builder call passed directly as an
    argument of a call to a method of the program that does not retain that position -- and evaluated
    unconditionally (not under ?:, && or ||)."""
    for n, chain in _with_parents(root):
        if not (isinstance(n, ir.Call) and n.sym is not None and n.sym.name in cat.scalar_methods):
            continue
        if not chain or not isinstance(chain[0], ir.Call) or chain[0].sym is None:
            continue
        outer = chain[0]
        pos = next((i for i, a in enumerate(outer.args or []) if a is n), None)
        if pos is None:
            continue
        if outer.sym.name not in cat.corpus_methods or pos in cat.retained_params.get(outer.sym.name, set()):
            continue
        if any(isinstance(a, ir.Cond) or (isinstance(a, ir.Binary) and a.op in ('&&', '||')) or
               isinstance(a, ir.Lambda) for a in chain):
            continue
        yield n, outer, pos


def _comment(text):
    return text.replace('*/', '* /')


def _abrupt(s):
    """Whether statement `s` never completes normally (Java's rule, for return and throw): it ends in a
    return or throw on every path."""
    if isinstance(s, (ir.Return, ir.Throw)):
        return True
    if isinstance(s, ir.Block):
        return bool(s.stmts) and _abrupt(s.stmts[-1])
    if isinstance(s, ir.If):
        return s.orelse is not None and _abrupt(s.then) and _abrupt(s.orelse)
    if isinstance(s, ir.Try):
        return _abrupt(s.body) and all(_abrupt(c.body) for c in s.catches)
    return False


def _leaves(stmts):
    """Whether any of these statement trees returns, or breaks or continues out of them."""
    def walk(n, loops, labels):
        if isinstance(n, ir.Return):
            return True
        if isinstance(n, ir.Break):
            return (n.label not in labels) if n.label else loops == 0
        if isinstance(n, ir.Continue):
            return (n.label not in labels) if n.label else loops == 0
        if isinstance(n, (ir.Lambda, ir.ClassDecl, ir.LocalClass)):
            return False
        inner = loops + isinstance(n, (ir.While, ir.DoWhile, ir.For, ir.ForEach, ir.Switch))
        labs = labels | {n.label} if isinstance(n, ir.Labeled) else labels
        return any(walk(c, inner, labs) for c in n.children())
    return any(s is not None and walk(s, 0, frozenset()) for s in stmts)
_CATCHES_NPE = frozenset(('java.lang.Exception', 'java.lang.RuntimeException', 'java.lang.Throwable',
                          'java.lang.NullPointerException'))
_UNBOX = {'int': 'intValue', 'short': 'shortValue', 'signed char': 'byteValue', 'long long': 'longValue',
          'float': 'floatValue', 'double': 'doubleValue', 'bool': 'booleanValue', 'unsigned char': 'charValue'}


def _runtime_header(owner):
    from codebreaker.java import runtime
    return runtime.FILES.get(owner + '.h') or getattr(runtime, '_GENERATED', {}).get(owner + '.h') or ''


def _runtime_return(owner, name):
    """The return type `owner.h` in the runtime declares for method `name`, or None."""
    rx = re.compile(r'^\s*(?:(?:inline|virtual|static)\s+)*([A-Za-z_][\w:<>,* &]*?)\s*\b%s\s*\(' % re.escape(name))
    for line in _runtime_header(owner).splitlines():
        m = rx.match(line)
        if m and m.group(1).strip() not in ('return', 'else', 'new', 'delete'):
            t = m.group(1).replace('virtual ', '').replace('inline ', '').replace('static ', '').strip()
            return 'JavaString' if t == 'std::string' else t
    return None


def _runtime_field(owner, name):
    """(C++ type, spelling) of field `name` as the runtime's `owner.h` declares it -- `s_name` or `name`
    (Locale::US, DataFlavor::s_stringFlavor) -- or (None, None)."""
    text = _runtime_header(owner)
    for spelled in ('s_' + name, name):
        for line in text.splitlines():
            if not re.search(r'\b%s\b' % re.escape(spelled), line) or '(' in line.split(spelled)[0]:
                continue
            m = re.match(r'^\s*(?:static\s+)?(?:const\s+)?([A-Za-z_][\w:<>* &]*?)\s+[A-Za-z_]', line)
            if m:
                return m.group(1).strip(), spelled
    return None, None

def unbox(code, ctype, target):
    """A boxed JObject where a primitive is wanted: its explicit accessor (the implicit conversions are
    ambiguous for short, byte and bool)."""
    t = re.sub(r'^const\s+', '', target or '').rstrip('&').strip()
    if ctype and ctype.startswith('JObject') and t in _UNBOX:
        return '(%s).%s()' % (code, _UNBOX[t])
    return code


def null_of(ctype):
    """Java's null as a value of C++ type `ctype`: NULL for a pointer; a value type's own null (the
    null state of JavaString/JList/JMap/JSet, an empty JArray, a null JObject)."""
    t = re.sub(r'^const\s+', '', ctype or '').rstrip('&').strip()
    if not t or t.endswith('*') or t in ('void*',):
        return 'NULL'
    if re.match(r'(JList|JMap|JSet)\b', t):
        return '%s(0LL)' % t
    return '%s()' % t


def coerce(code, ctype, target):
    """`code` (of C++ type `ctype`) where a `target` is wanted: Java's null becomes the target's null."""
    if code == 'NULL' and target and not is_pointer(target):
        return null_of(target)
    return code


def _class_of(ctype):
    m = re.match(r'\s*([A-Za-z_]\w*)\s*\*\s*$', ctype or '')
    return m.group(1) if m else None


def _float(v, suffix):
    if v != v or v in (float('inf'), float('-inf')):
        raise Decline('non-finite literal')
    s = repr(float(v))
    if 'e' in s or 'E' in s:
        mant, exp = s.lower().split('e')
        if '.' not in mant:
            mant += '.0'
        s = '%se%s' % (mant, exp)
    elif '.' not in s:
        s += '.0'
    return s + suffix


def _cstring(v):
    out = []
    for ch in v:
        o = ord(ch)
        if ch == '\\':
            out.append('\\\\')
        elif ch == '"':
            out.append('\\"')
        elif ch == '\n':
            out.append('\\n')
        elif ch == '\r':
            out.append('\\r')
        elif ch == '\t':
            out.append('\\t')
        elif o < 32 or 127 <= o <= 255:                  # the runtime's String is a char per byte (Latin-1),
            out.append('\\%03o' % o)                      # as the judge's inputs are: § is one byte, 0xA7
        elif o == _HOUSE:
            out.append('\\177')                           # the font's ⌂ tile: its CP437 byte
        elif o > 255:
            out.append('?')                               # outside the 8-bit runtime, as Latin-1 encodes it
        else:
            out.append(ch)
    return '"%s"' % ''.join(out)


def ctor_parts(ctx, cls, m, old, two_phase):
    """A constructor from the IR: (super-arguments or None, body lines). Ordinary classes: super(args)
    is the base initialiser, so its arguments come back separately; this(args) is declined (C++03 has no
    delegation: the transpiler inlines the other constructor). Two-phase classes: super(args) is
    Parent::j2xInit(args), this(args) is C::j2xInit(args), the instance initialisers are the caller's."""
    if ir.unsupported(m):
        raise Decline('unsupported construct')
    w = Writer(ctx, cls, m, old)
    stmts = list(m.body.stmts) if m.body is not None else []
    first = stmts[0] if stmts and isinstance(stmts[0], ir.CtorCall) else None
    if first is not None:
        stmts = stmts[1:]
    args = None
    if first is not None and not first.kind.endswith(':implicit'):
        if first.target is not None:
            raise Decline('qualified super call')
        args = ', '.join(w.expr(a)[0] for a in first.args)
    out = []
    if two_phase:
        w.two_phase = True
        w.ret = cls.name + '*'
        if first is not None and first.kind.startswith('this'):
            out.append('%s::j2xInit(%s);' % (cls.name, args or ''))
            args = None
    elif first is not None and first.kind.startswith('this'):
        # C++03 has no delegation: the other constructor's base initialiser comes here, with its
        # parameters replaced by this call's arguments, and its body runs first, in a block of its own
        kind, args = _delegated_super(ctx, cls, w, first, 0)
        out += _delegated_body(ctx, cls, w, first, 0)
        for s in stmts:
            w.stmt(s, out, '')
        return kind, args, out
    for s in stmts:
        w.stmt(s, out, '')
    return (first.kind.split(':')[0] if first is not None else None), args, out


def _delegate_target(ctx, cls, call):
    """The constructor this(...) calls: (IR constructor, the transpiler's parsed one)."""
    if call.sym is None:
        raise Decline('unresolved this(...)')
    want = [jtype(p).name + '[]' * jtype(p).array_dims for p in call.sym.params]
    for m in cls.members:
        if isinstance(m, ir.MethodDecl) and m.kind == 'ctor' and len(m.params) == len(want):
            if [jtype(p.type).name + '[]' * jtype(p.type).array_dims for p in m.params] == want:
                old = ctx.old_ctor(cls.name, [p.type for p in m.params])
                if old is None:
                    raise Decline('delegate target not parsed')
                return m, old
    raise Decline('delegate target not found')


def _split_ctor(m):
    stmts = list(m.body.stmts) if m.body is not None else []
    first = stmts[0] if stmts and isinstance(stmts[0], ir.CtorCall) else None
    return first, (stmts[1:] if first is not None else stmts)


def _delegated_super(ctx, cls, w, call, depth):
    """(kind, arguments) of the base initialiser the delegation chain ends in, written with each target's
    parameters replaced by the (parenthesised) arguments it was called with."""
    if depth > 8:
        raise Decline('delegation too deep')
    target, old = _delegate_target(ctx, cls, call)
    tw = Writer(ctx, cls, target, old)
    for p, a in zip(target.params, call.args):
        tw.alias[tw.name(p.sym.name)] = '(%s)' % w.expr(a)[0]
    first, _rest = _split_ctor(target)
    if first is not None and first.kind.startswith('this'):
        return _delegated_super(ctx, cls, tw, first, depth + 1)
    if first is not None and not first.kind.endswith(':implicit'):
        if first.target is not None:
            raise Decline('qualified super call')
        return 'super', ', '.join(tw.expr(a)[0] for a in first.args)
    return None, None


def _delegated_body(ctx, cls, w, call, depth):
    """The delegated-to constructor's body, in a block: its parameters as locals holding the arguments."""
    if depth > 8:
        raise Decline('delegation too deep')
    target, old = _delegate_target(ctx, cls, call)
    tw = Writer(ctx, cls, target, old)
    lines = ['{']
    for p, a in zip(target.params, call.args):
        name = tw.name(p.sym.name)
        ct = tw.vars[name]
        temp = 'j2xD%d_%s' % (depth, name)
        code, at = w.expr(a)
        local = re.sub(r'^const\s+', '', ct).rstrip('&').strip()
        lines.append('\t%s %s = %s;' % (local, temp, unbox(coerce(code, at, local), at, local)))
        tw.vars[temp] = local
        tw.alias[name] = temp
    first, rest = _split_ctor(target)
    if first is not None and first.kind.startswith('this'):
        lines += ['\t' + x for x in _delegated_body(ctx, cls, tw, first, depth + 1)]
    for st in rest:
        tw.stmt(st, lines, '\t')
    lines.append('}')
    return lines


def field_init(ctx, cls, f):
    """A field initialiser's C++ expression, converted to the field's type."""
    if f.init is None or ir.unsupported(f):
        raise Decline('field initialiser')
    w = Writer(ctx, cls, None, None)
    code, ct = w.expr(f.init)
    owner = cls.name
    target = ctx.field_ctype(owner, f.sym.name)
    if target is None:
        raise Decline('field %s.%s' % (owner, f.sym.name))
    return unbox(coerce(code, ct, target), ct, target)


def block_lines(ctx, cls, m):
    """An initialiser block's statements (static {} or instance {})."""
    if ir.unsupported(m):
        raise Decline('unsupported construct')
    w = Writer(ctx, cls, None, None)
    out = []
    for s in m.body.stmts:
        w.stmt(s, out, '')
    return out


def method_body(ctx, cls, m, old):
    """The C++ body lines of IR method `m`, or raise Decline."""
    if ir.unsupported(m):
        raise Decline('unsupported construct')
    return Writer(ctx, cls, m, old).body()


class Bodies(object):
    """The tree's method bodies for one conversion, looked up by the transpiler as it writes each method
    (emit/source.py). Counts what it wrote and why it declined the rest."""

    def __init__(self, ctx, units):
        import collections
        self.ctx = ctx
        self.decls = {}
        for u in units:
            for c in u.types:
                self.decls.setdefault(c.name, c)
        ctx.qnames = set(c.qname for c in self.decls.values() if c.qname)
        self.written = 0
        self.written_ctors = 0
        self.declined = collections.Counter()
        self.reasons = collections.Counter()              # the full reason, for the work list

    def _find(self, cls_name, old):
        c = self.decls.get(cls_name)
        if c is None:
            return None, None
        want = [p.jtype.name + '[]' * p.jtype.array_dims for p in old.params]
        for m in c.members:
            if isinstance(m, ir.MethodDecl) and m.kind == 'method' and m.name == old.name \
                    and len(m.params) == len(old.params):
                try:
                    got = [jtype(p.type).name + '[]' * jtype(p.type).array_dims for p in m.params]
                except Decline:
                    continue
                if got == want:
                    return c, m
        return c, None

    def _ctor(self, cls_name, old):
        c = self.decls.get(cls_name)
        if c is None:
            return None, None
        ctors = [m for m in c.members if isinstance(m, ir.MethodDecl) and m.kind == 'ctor']
        if old is None:                                   # the implicit default constructor
            return c, next((m for m in ctors if not m.params), None)
        want = [p.jtype.name + '[]' * p.jtype.array_dims for p in old.params]
        for m in ctors:
            if len(m.params) != len(old.params):
                continue
            try:
                got = [jtype(p.type).name + '[]' * jtype(p.type).array_dims for p in m.params]
            except Decline:
                continue
            if got == want:
                return c, m
        return c, None

    def ctor(self, cls_name, old, two_phase=False):
        """(kind 'super'/'this'/None, its arguments, body lines) for the transpiler's constructor `old`,
        or None."""
        c, m = self._ctor(cls_name, old)
        if m is None:
            self.declined['ctor not found'] += 1
            return None
        try:
            parts = ctor_parts(self.ctx, c, m, old, two_phase)
        except Decline as e:
            self.declined['ctor ' + (str(e).split(' ')[0] if str(e) else '')] += 1
            self.reasons['ctor: ' + str(e)] += 1
            return None
        self.written_ctors += 1
        return parts

    def field_init(self, cls_name, name):
        """The C++ initialiser expression of field `name`, or None."""
        c = self.decls.get(cls_name)
        f = next((x for x in c.members if isinstance(x, ir.FieldDecl) and x.sym is not None
                  and x.sym.name == name), None) if c is not None else None
        if f is None or f.init is None:
            return None
        try:
            return field_init(self.ctx, c, f)
        except Decline as e:
            self.reasons['field init: ' + str(e)] += 1
            return None

    def enum_args(self, cls_name, const):
        """The C++ arguments of enum constant `const`'s constructor call, or None."""
        c = self.decls.get(cls_name)
        e = next((x for x in c.members if isinstance(x, ir.EnumConst) and x.sym is not None
                  and x.sym.name == const), None) if c is not None else None
        if e is None or e.body is not None:
            return None
        try:
            w = Writer(self.ctx, c, None, None)
            return [w.arg(a) for a in w.unmarked(e.args or [])]
        except Decline as ex:
            self.reasons['enum constant: ' + str(ex)] += 1
            return None

    def array_items(self, cls_name, name):
        """The C++ elements of static field `name`'s `new T[]{...}` initialiser, each as the element type,
        or None."""
        c = self.decls.get(cls_name)
        f = next((x for x in c.members if isinstance(x, ir.FieldDecl) and x.sym is not None
                  and x.sym.name == name), None) if c is not None else None
        init = f.init if f is not None else None
        if not isinstance(init, ir.NewArray) or init.init is None or init.dims:
            return None
        try:
            w = Writer(self.ctx, c, None, None)
            elem = _elem(self.ctx.field_ctype(cls_name, name) or w.ctype(init.type))
            out = []
            for x in init.init:
                code, ct = w.expr(x)
                out.append(unbox(coerce(code, ct, elem), ct, elem))
            return out
        except Decline as ex:
            self.reasons['array items: ' + str(ex)] += 1
            return None

    def instance_inits(self, cls_name):
        """`this->f = init;` for each instance field initialiser and the instance {} blocks, in source
        order -- or None (the transpiler's own, then)."""
        c = self.decls.get(cls_name)
        if c is None:
            return None
        out = []
        try:
            for x in c.members:
                if isinstance(x, ir.FieldDecl) and x.init is not None and x.sym is not None and not x.sym.static:
                    if x.type is not None and x.type.kind == 'array' and isinstance(x.init, ir.Lit) \
                            and x.init.value is None:
                        continue                          # an array field = null: JArray starts empty
                    out.append('this->%s = %s;' % (x.sym.name, field_init(self.ctx, c, x)))
                elif isinstance(x, ir.MethodDecl) and x.kind == 'init':
                    out += block_lines(self.ctx, c, x)
        except Decline as e:
            self.reasons['instance init: ' + str(e)] += 1
            return None
        return out

    def static_blocks(self, cls_name):
        """Each static {} block's lines, in source order, or None."""
        c = self.decls.get(cls_name)
        if c is None:
            return None
        try:
            return [block_lines(self.ctx, c, x) for x in c.members
                    if isinstance(x, ir.MethodDecl) and x.kind == 'static_init']
        except Decline as e:
            self.reasons['static block: ' + str(e)] += 1
            return None

    def body(self, cls_name, old):
        """Body lines for the transpiler's method `old` of class `cls_name`, or None (use its own)."""
        c, m = self._find(cls_name, old)
        if m is None:
            self.declined['not found'] += 1
            return None
        try:
            lines = method_body(self.ctx, c, m, old)
        except Decline as e:
            self.declined[str(e).split(' ')[0] if str(e) else 'declined'] += 1
            self.reasons[str(e)] += 1
            return None
        self.written += 1
        return lines


def java_units(java_dir, classpath=()):
    """Run the Java front end over java_dir and lower every unit. -> [ir.Unit]"""
    import shutil
    import tempfile
    from codebreaker.translate.frontends import java as fj
    from codebreaker.translate.frontends.java import lower as L
    out = tempfile.mkdtemp(prefix='cb-treedump-')
    try:
        units = fj.dump(java_dir, out, classpath=classpath)
        return [L.lower(t, p) for p, t in sorted(units.items())]
    finally:
        shutil.rmtree(out, ignore_errors=True)
