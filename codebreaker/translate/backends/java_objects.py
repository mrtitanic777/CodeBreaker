"""java_objects.py -- the Java back end's idiomatic tier: C structs and C++ classes as Java objects.

Chosen when the pointer analysis (java.Analysis) finds every pointer disciplined: no pointer
arithmetic, no casts between pointer types, no unions, no raw memory functions, no address of an
element or a field. Then:

  * a struct or class is a Java class (a nested static class of the program), its fields Java fields;
    a C++ class keeps its methods, constructors (base initialiser as super(...)), single inheritance,
    virtual and abstract methods, static members; operators become named methods (operator+ -> plus);
  * a pointer to a struct is a reference (NULL is null), `p->f` is `p.f`;
  * an array is a Java array, and so is a pointer to a scalar: it can only index (`*p` is `p[0]`);
  * a scalar whose address is taken is boxed in a one-element array (`int[] n = {5}`), so `&n` is `n`
    and `*p = 3` is `p[0] = 3` -- C's out-parameters and C++'s `int &` work as they did;
  * struct values keep C's copy semantics: assignment copies into the object (`a.set(b)`), passing or
    initialising by value copies (`new Point(p)`, the copy constructor), aggregates are constructors;
  * malloc(sizeof(T)) is `new T()`, malloc(n * sizeof(T)) `new T[n]`; free is the collector's;
  * function pointers are functional interfaces (`IntIntToInt cmp = Prog::compare;`).
Arithmetic, conversions, statements and C's library are the faithful tier's (java.FnWriter).
"""
import re

from codebreaker.translate import ir
from codebreaker.translate.backends import java as jb
from codebreaker.translate.backends.java import J, Refused, jname, strip, is_ptr, is_record, _kind

_OPERATORS = {'operator+': 'plus', 'operator-': 'minus', 'operator*': 'times', 'operator/': 'div',
              'operator%': 'rem', 'operator==': 'isEqual', 'operator!=': 'notEqual', 'operator<': 'lessThan',
              'operator>': 'greaterThan', 'operator<=': 'atMost', 'operator>=': 'atLeast', 'operator[]': 'at',
              'operator()': 'call', 'operator+=': 'plusAssign', 'operator-=': 'minusAssign',
              'operator*=': 'timesAssign', 'operator/=': 'divAssign', 'operator=': 'assign', 'operator!': 'not',
              'operator<<': 'shiftLeft', 'operator>>': 'shiftRight', 'operator&': 'and', 'operator|': 'or',
              'operator^': 'xor', 'operator~': 'complement', 'operator++': 'increment', 'operator--': 'decrement',
              'operator bool': 'toBool'}


def method_name(name):
    if name in _OPERATORS:
        return _OPERATORS[name]
    if name.startswith('operator'):
        raise Refused('operator %s' % name)
    return jname(name)


class Objects(jb.Faithful):
    """The idiomatic tier's program writer."""

    def _check_records(self):
        for c in self.records.values():
            if c.kind == 'union':
                raise Refused('union %s' % c.name)
            if c.implements:
                raise Refused('%s has more than one base class' % c.name)
            for m in c.members or []:
                if 'bitfield' in (m.mods or []):
                    raise Refused('bit-fields in %s' % c.name)
                if isinstance(m, ir.FieldDecl) and m.sym is not None and m.sym.name.startswith('<'):
                    raise Refused('a member the front end did not lower in %s' % c.name)
        self.ctors_used = set()            # qnames whose aggregate constructor is used
        self.copied = set()                # qnames whose copy constructor is used
        self.assigned = set()              # qnames whose set() is used
        self.arrays_filled = False

    def _names(self):
        taken = set(jname(n) for n in self.functions) | set(jname(f.sym.name) for f in self.global_decls)
        taken.add(self.name)
        self.cls_names = {}
        for qn, c in sorted(self.records.items(), key=lambda kv: kv[1].line):
            base = c.name.replace('__unnamed_', 'Anon')
            if '<' in qn:                     # a template instance: Stack<i32, 8> -> Stack_int_8
                args = qn[qn.index('<') + 1:-1]
                args = re.sub(r'\b(%s)\b' % '|'.join(jb.JPRIM), lambda m: jb.JPRIM[m.group(1)], args)
                base += '_' + re.sub(r'\W+', '_', args).strip('_')
            base = jname(base)
            n = base
            while n in taken:
                n += '_'
            taken.add(n)
            self.cls_names[qn] = n
        self.layout_names = self.cls_names
        self.used_names = taken

    # ------------------------------------------------------------------------------------- types
    def cls(self, t):
        qn = self.rec_name(t)
        if qn.startswith('java.') and qn not in self.cls_names:
            return qn.rsplit('.', 1)[-1]                 # Java's own library (a program read back from Java)
        if qn not in self.cls_names:
            if qn.startswith('std.'):
                raise Refused('the C++ standard library (%s) is not carried yet' % qn.replace('.', '::'))
            raise Refused('no class for %r' % (t,))
        return self.cls_names[qn]

    def size_hint(self, t):
        """sizeof(t) where the source's layout gives it (C structs, scalars), else None."""
        try:
            return self.size(t)
        except Refused:
            return None

    def is_enum(self, t):
        c = self.records.get(self.rec_name(t)) if t is not None and t.kind == 'class' else None
        return c is not None and c.kind == 'enum'

    def jt(self, t):
        k = _kind(t)
        if k in jb.JPRIM:
            return jb.JPRIM[k]
        if k == 'void':
            return 'void'
        if k in ('class', 'record'):
            if self.is_enum(t):
                return 'int'
            return self.cls(t)
        if k == 'array':
            return self.jt(t.elem) + '[]'
        if k == 'ptr':
            e = t.elem
            if is_record(e):
                return self.cls(e)
            if e is None or e.kind == 'void':
                raise Refused('a void pointer')
            if e.kind == 'fn':
                return self.iface(self.fn_sig_of_type(e))
            return self.jt(e) + '[]'
        if k == 'fnptr':
            return self.iface(self.fn_sig_of_type(t))
        if k == 'null':
            return 'Object'
        raise Refused('type %r' % (t,))

    def fn_sig(self, s):
        params = tuple(self.jt(p) for p in s.params)
        ret = self.jt(s.ret) if s.ret is not None else 'void'
        return params, ret

    def fn_sig_of_type(self, ft):
        f = ft.info['fn'] if ft.kind == 'fnptr' else ft
        params = tuple(self.jt(p['type']) for p in f.info['params'])
        rt = f.info['result'].get('type')
        return params, (self.jt(rt) if rt is not None else 'void')

    def iface(self, sig):
        if sig not in self.fn_ifaces:
            params, ret = sig

            def word(p):
                p = p.replace('[]', 'Array')
                return p[:1].upper() + p[1:]
            n = ''.join(word(p) for p in params) + 'To' + word(ret)
            self.fn_ifaces[sig] = n
        return self.fn_ifaces[sig]

    def default(self, t):
        """A fresh value of type t: what C's zero-initialised storage holds, as Java."""
        k = _kind(t)
        if k in ('class', 'record'):
            return 'new %s()' % self.cls(t)
        if k == 'array' and 'count' not in t.info:
            return 'null'                                # (an array reference -- Java's own, read back -- not storage)
        if k == 'array':
            dims, base = [], t
            while base.kind == 'array':
                if 'count' not in base.info:
                    raise Refused('an array of unknown size')
                dims.append(base.info['count'])
                base = base.elem
            if is_record(base):
                if len(dims) != 1:
                    raise Refused('a multi-dimensional array of structs')
                self.arrays_filled = True
                return 'filled(new %s[%d], %s::new)' % (self.cls(base), dims[0], self.cls(base))
            return 'new %s%s' % (self.jt(base), ''.join('[%d]' % d for d in dims))
        if k in ('ptr', 'fnptr', 'null'):
            return 'null'
        return {'boolean': 'false', 'float': '0.0f', 'double': '0.0', 'long': '0L'}.get(jb.JPRIM.get(k), '0')

    # ------------------------------------------------------------------------------------- the file
    def write(self):
        self.fields_out = []
        self.static_init = []
        self.boxed = set()
        for g in self.global_decls:
            self.declare_global(g)
        # (written until what the classes must offer -- aggregate, copy and assignment constructors -- is
        # stable: a method written later can ask it of a class written earlier)
        seen = None
        for _round in range(4):
            self.gaps = []
            funcs = [self.function_or_gap(f, lambda f=f: ObjWriter(self, f).write()) for f in self.all_functions()]
            classes = []
            for qn in sorted(self.records, key=lambda q: (self.records[q].line, q)):
                classes += self.class_decl(self.records[qn])
            now = (len(self.ctors_used), len(self.copied), len(self.assigned), self.arrays_filled)
            if now == seen:
                break
            seen = now
        out = ['// %s.java -- translated from %s by CodeBreaker (idiomatic tier: structs as classes, pointers as '
               'references).' % (self.name, 'C++' if self.a.cpp_classes else 'C'),
               'import crt.*;', '', 'import static crt.LibC.*;', '', 'public final class %s {' % self.name]
        out += classes
        for sig, name in self.fn_ifaces.items():
            params, ret = sig
            ps = ', '.join('%s a%d' % (p, i) for i, p in enumerate(params))
            out += ['', '    interface %s {' % name, '        %s call(%s);' % (ret, ps), '    }']
        if self.fields_out:
            out.append('')
            out += ['    ' + f for f in self.fields_out]
        if self.static_init:
            out += ['', '    static {'] + ['        ' + s for s in self.static_init] + ['    }']
        for f in funcs:
            out += [''] + f
        out += self.entry()
        out.append('}')
        self.notes += ['gap: ' + g for g in self.gaps]
        return '\n'.join(out) + '\n'

    def gap_signature(self, f, owner):
        try:
            params, ret = self.fn_sig(f.sym)
            if f.ret is not None and f.ret.kind == 'ptr' and f.ret.info.get('ref') and not is_record(f.ret.elem):
                ret = self.jt(f.ret.elem)
        except Refused:
            return None
        ps = ', '.join('%s %s' % (t, jname(p.sym.name)) for t, p in zip(params, f.params))
        if owner is None:
            return 'static %s %s(%s)' % (ret, jname(f.name), ps)
        if f.kind != 'method':
            return None
        return '%s%s %s(%s)' % ('static ' if 'static' in (f.mods or []) else '', ret, method_name(f.name), ps)

    def entry(self):
        m = self.functions.get('main')
        if m is None:
            # a program read back from Java: its class with `static void main(String[])`
            for qn, c in self.records.items():
                for x in c.members:
                    if isinstance(x, ir.MethodDecl) and x.name == 'main' and 'static' in (x.mods or []) and \
                            len(x.params) == 1 and x.params[0].type.kind == 'array':
                        return ['', '    public static void main(String[] args) {',
                                '        %s.main(args);' % self.cls_names[qn], '    }']
            raise Refused('no main function')
        out = ['', '    public static void main(String[] args) {']
        call = 'main()'
        if len(m.params) >= 2:
            call = 'main(args.length + 1, argv(System.getProperty("c.argv0", "%s"), args))' % self.name
        if m.ret is None or m.ret.kind == 'void':
            out += ['        %s;' % call, '        exit(0);']
        else:
            out.append('        exit(%s);' % call)
        out.append('    }')
        return out

    # ------------------------------------------------------------------------------------- globals
    def declare_global(self, g):
        s = g.sym
        name = jname(s.name)
        if 'const' in (g.mods or []) and any(m.startswith('enum:') for m in g.mods or []):
            self.globals[s.name] = jb.Var(name, False, ir.I32, 'enum')
            self.fields_out.append('static final int %s = %s;' % (name, g.init.value))
            return
        if s.name in self.globals and self.globals[s.name].kind != 'enum':
            return
        t = g.type
        boxed = ('g', s.name) in self.addr_taken and not is_record(t) and t.kind != 'array'
        v = jb.Var(name, boxed, t, 'global')
        self.globals[s.name] = v
        w = ObjWriter(self, None)
        if boxed:
            init = w.init_value(t, g.init) if g.init is not None else J(self.default(t), jb.P_ATOM, self.jt(t))
            self.fields_out.append('static %s[] %s = {%s};' % (self.jt(t), name, init.code))
            return
        init = w.init_value(t, g.init) if g.init is not None else J(self.default(t), jb.P_ATOM, self.jt(t))
        final = 'final ' if t.kind in ('array', 'class', 'record') else ''
        self.fields_out.append('static %s%s %s = %s;' % (final, self.jt(t), name, init.code))

    def init_global(self, g):
        pass

    # ------------------------------------------------------------------------------------- classes
    def class_decl(self, c):
        n = self.cls_names[c.qname]
        if c.kind == 'enum':                                 # a C++ enum class: its constants
            consts = ['static final int %s = %s;' % (jname(f.sym.name), f.init.value) for f in c.members]
            return ['', '    /** enum class %s */' % c.name, '    static final class %s {' % n] + \
                   ['        ' + x for x in consts] + ['    }']
        t = ir.Type('class', c.qname)
        fields = [m for m in c.members if isinstance(m, ir.FieldDecl)]
        inst = [f for f in fields if 'static' not in (f.mods or [])]
        methods = [m for m in c.members if isinstance(m, ir.MethodDecl)]
        mods = 'static '
        abstract = 'abstract' in (c.mods or []) or any('pure' in (m.mods or []) for m in methods)
        if abstract:
            mods += 'abstract '
        ext = ''
        base = None
        if c.extends is not None:
            base = self.records.get(self.rec_name(c.extends))
            ext = ' extends %s' % self.cls(c.extends)
        what = {'struct': 'struct', 'class': 'class', 'union': 'union'}.get(c.kind, c.kind)
        title = c.qname.replace('.', '::').replace('__unnamed_', '(anonymous) ')
        out = ['', '    /** %s %s */' % (what, title), '    %sclass %s%s {' % (mods, n, ext)]
        body = []
        for f in fields:
            ft = f.type
            st = 'static ' if 'static' in (f.mods or []) else ''
            name = jname(f.sym.name)
            if f.init is not None:
                w = ObjWriter(self, None, cls=c)
                init = ' = ' + w.init_value(ft, f.init).code
            elif ft.kind in ('array', 'class', 'record'):
                init = ' = ' + self.default(ft)
            else:
                init = ''
            if st and 'const' in (f.mods or []) and f.init is not None:
                st = 'static final '
            body.append('%s%s %s%s;' % (st, self.jt(ft), name, init))
        ctors = [m for m in methods if m.kind == 'ctor']
        has_dtor = [m for m in methods if m.kind == 'dtor']
        user_copy = any(len(m.params) == 1 and is_ptr(m.params[0].type) and is_record(m.params[0].type.elem) and
                        self.rec_name(m.params[0].type.elem) == c.qname for m in ctors)
        # a no-argument constructor where Java would not make one: beside an aggregate or a copy constructor
        need_noarg = not ctors and (c.qname in self.ctors_used and inst or c.qname in self.copied or
                                    c.qname in self.assigned)
        if need_noarg:
            body.append('')
            body.append('%s() {' % n)
            body.append('}')
        if c.qname in self.ctors_used and inst:
            ps = ', '.join('%s %s' % (self.jt(f.type), jname(f.sym.name)) for f in inst)
            body += ['', '%s(%s) {' % (n, ps)]
            body += ['    this.%s = %s;' % (jname(f.sym.name), jname(f.sym.name)) for f in inst]
            body.append('}')
        if (c.qname in self.copied or c.qname in self.assigned) and not user_copy:
            body += ['', '/** C\'s copy of a %s value. */' % what, '%s(%s o) {' % (n, n)]
            if base is not None:
                body.append('    super(o);')
            body += ['    ' + x for x in self.copy_fields(inst, 'this', 'o', fresh=True)]
            body.append('}')
        if c.qname in self.assigned and not any(m.name == 'operator=' for m in methods):
            body += ['', '/** C\'s assignment of a %s value: copies o into this one. */' % what,
                     '%s set(%s o) {' % (n, n)]
            if base is not None:
                body.append('    super.set(o);')
            body += ['    ' + x for x in self.copy_fields(inst, 'this', 'o', fresh=False)]
            body += ['    return this;', '}']
        for m in methods:
            if m.body is None and '<' in c.qname and 'pure' not in (m.mods or []):
                continue                 # (a template's member this instance never uses: clang made no body)
            if m.kind == 'dtor':
                if m.body is None or (not m.body.stmts and 'virtual' not in (m.mods or [])):
                    continue
            body += [''] + self.function_or_gap(m, lambda m=m: ObjWriter(self, m, cls=c).write_member(), indent='',
                                                owner=c.name)
        out += ['        ' + x if x else x for x in body]
        out.append('    }')
        return out

    def copy_fields(self, fields, dst, src, fresh):
        out = []
        for f in fields:
            name = jname(f.sym.name)
            t = f.type
            if is_record(t):
                out.append('%s.%s = new %s(%s.%s);' % (dst, name, self.cls(t), src, name) if fresh else
                           '%s.%s.set(%s.%s);' % (dst, name, src, name))
                self.copied.add(self.rec_name(t))
                if not fresh:
                    self.assigned.add(self.rec_name(t))
            elif t.kind == 'array':
                base = t
                depth = 0
                while base.kind == 'array':
                    base = base.elem
                    depth += 1
                if is_record(base):
                    self.copied.add(self.rec_name(base))
                    if fresh:
                        out.append('%s.%s = new %s[%s.%s.length];' % (dst, name, self.cls(base), src, name))
                        out.append('for (int i = 0; i < %s.length; i++) %s.%s[i] = new %s(%s.%s[i]);' % (
                            src + '.' + name, dst, name, self.cls(base), src, name))
                    else:
                        self.assigned.add(self.rec_name(base))
                        out.append('for (int i = 0; i < %s.length; i++) %s.%s[i].set(%s.%s[i]);' % (
                            src + '.' + name, dst, name, src, name))
                elif depth == 1:
                    out.append('%s.%s = %s.%s.clone();' % (dst, name, src, name) if fresh else
                               'System.arraycopy(%s.%s, 0, %s.%s, 0, %s.%s.length);' % (src, name, dst, name, dst, name))
                else:
                    out.append('%s.%s = deepCopy(%s.%s);' % (dst, name, src, name) if fresh else
                               'copyInto(%s.%s, %s.%s);' % (dst, name, src, name))
            else:
                out.append('%s.%s = %s.%s;' % (dst, name, src, name))
        return out


class ObjWriter(jb.FnWriter):
    """A function, method or constructor in the idiomatic tier."""

    def __init__(self, prog, fn, cls=None):
        jb.FnWriter.__init__(self, prog, fn)
        self.cls = cls
        self.ret_struct = False

    # ------------------------------------------------------------------------------------- functions
    def write(self):
        f = self.fn
        s = f.sym
        params, pre = self.params(f)
        body = list(pre)
        self.block_stmts(f.body.stmts, body, 1)
        ret = self.p.jt(f.ret) if f.ret is not None else 'void'
        if ret != 'void' and self.can_complete(f.body):
            body.append('return 0;' if f.name == 'main' else 'return %s;  // (C: the function ends without a value)' % self.default_of(f.ret))
        head = '    static %s %s(%s) {' % (ret, jname(f.name), ', '.join(params))
        return [head] + ['        ' + x if x else x for x in body] + ['    }']

    def default_of(self, t):
        return self.p.default(t) if not is_record(t) else 'null'

    def params(self, f):
        params, pre = [], []
        for prm in f.params:
            name = jname(prm.sym.name)
            self.declared.add(name)
            t = prm.type
            key = ('l', prm.sym.owner, prm.sym.name)
            if key in self.p.addr_taken and not is_record(t) and t.kind != 'array':
                arg = name + 'Arg'
                params.append('%s %s' % (self.p.jt(t), arg))
                pre.append('%s[] %s = {%s};' % (self.p.jt(t), name, arg))
                self.vars[prm.sym.name] = jb.Var(name, True, t, 'param')
            else:
                params.append('%s %s' % (self.p.jt(t), name))
                self.vars[prm.sym.name] = jb.Var(name, False, t, 'param')
        return params, pre

    def write_member(self):
        m = self.fn
        c = self.cls
        n = self.p.cls_names[c.qname]
        params, pre = self.params(m)
        mods = []
        if 'static' in (m.mods or []):
            mods.append('static')
        if m.kind == 'ctor':
            body = []
            stmts = list(m.body.stmts) if m.body is not None else []
            if stmts and isinstance(stmts[0], ir.CtorCall):
                cc = stmts.pop(0)
                body.append('%s(%s);' % ('super' if cc.kind == 'super' else 'this',
                                         ', '.join(self.call_args(cc.args or [], cc.sym))))
            # a member initialised to zero before anything else: a Java field already is
            while stmts and self.zero_init(stmts[0], c):
                stmts.pop(0)
            body += pre
            self.block_stmts(stmts, body, 1)
            return ['%s(%s) {' % (n, ', '.join(params))] + ['    ' + x if x else x for x in body] + ['}']
        if m.kind == 'static_init':
            body = []
            self.block_stmts(m.body.stmts, body, 1)
            if not body:
                return []
            return ['static {'] + ['    ' + x if x else x for x in body] + ['}']
        if m.kind == 'dtor':
            body = list(pre)
            self.block_stmts(m.body.stmts, body, 1)
            base = self.p.records.get(self.p.rec_name(c.extends)) if c.extends is not None else None
            if base is not None and any(isinstance(x, ir.MethodDecl) and x.kind == 'dtor' and x.body is not None
                                        and (x.body.stmts or 'virtual' in (x.mods or [])) for x in base.members):
                body.append('super.destroy();')
            return ['/** ~%s() */' % c.name, 'void destroy() {'] + ['    ' + x if x else x for x in body] + ['}']
        ret = self.p.jt(m.ret) if m.ret is not None else 'void'
        if m.ret is not None and m.ret.kind == 'ptr' and m.ret.info.get('ref') and not is_record(m.ret.elem):
            raise Refused('%s returns a reference to a %r' % (m.name, m.ret.elem))
        if m.ret is not None and m.ret.kind == 'ptr' and m.ret.info.get('ref'):
            ret = self.p.jt(m.ret.elem)
        name = method_name(m.name)
        if m.body is None:
            if 'pure' in (m.mods or []):
                return ['abstract %s %s(%s);' % (ret, name, ', '.join(params))]
            raise Refused('%s::%s is declared but not defined' % (c.name, m.name))
        body = list(pre)
        self.block_stmts(m.body.stmts, body, 1)
        if ret != 'void' and self.can_complete(m.body):
            body.append('return %s;' % self.default_of(m.ret))
        return ['%s%s %s(%s) {' % (' '.join(mods) + ' ' if mods else '', ret, name, ', '.join(params))] + \
               ['    ' + x if x else x for x in body] + ['}']

    def zero_init(self, s, c):
        """`this->f = 0` (or null, false) for a field with no initialiser of its own."""
        if not isinstance(s, ir.ExprStmt) or not isinstance(s.expr, ir.Assign) or s.expr.op is not None:
            return False
        t = strip(s.expr.target)
        if not (isinstance(t, ir.Select) and isinstance(strip(t.obj), ir.Unary) and
                isinstance(strip(strip(t.obj).x), ir.This)):
            return False
        v = strip(s.expr.value)
        while isinstance(v, ir.Cast):
            v = strip(v.x)
        if not (isinstance(v, ir.Lit) and v.value in (0, None, False) and not isinstance(v.value, str)):
            return False
        f = [m for m in c.members if isinstance(m, ir.FieldDecl) and m.sym.name == t.sym.name]
        return bool(f) and f[0].init is None and f[0].type.kind not in ('class', 'record', 'array')

    # ------------------------------------------------------------------------------------- statements
    def local(self, s, out):
        name = jname(s.sym.name)
        if name in self.declared:
            name = self.fresh(name + '_')
        self.declared.add(name)
        t = s.type
        mods = s.mods or []
        if 'extern' in mods:
            return
        if 'static' in mods:
            self.static_local(s, name)
            return
        key = ('l', s.sym.owner, s.sym.name)
        if is_record(t) and self.has_destroy(t):
            raise Refused('%s %s has a destructor that runs at the end of its scope (RAII: try/finally not written '
                          'yet)' % (self.p.cls(t), s.sym.name))
        boxed = key in self.p.addr_taken and not is_record(t) and t.kind != 'array'
        self.vars[s.sym.name] = jb.Var(name, boxed, t, 'local')
        jt = self.p.jt(t)
        if s.init is None:
            init = self.p.default(t)
        else:
            init = self.init_value(t, s.init).code
        if boxed:
            out.append('%s[] %s = {%s};' % (jt, name, init))
        else:
            out.append('%s %s = %s;' % (jt, name, init))

    def init_value(self, t, init):
        """An initialiser of type t as a Java expression (a fresh object or array where t is one)."""
        init = strip(init)
        if isinstance(init, ir.Lit) and isinstance(init.value, str) and t.kind == 'array':
            n = t.info.get('count', len(init.value) + 1)
            return J('bytes(%s, %d)' % (jb.java_string(init.value), n), jb.P_POST, 'byte[]')
        if isinstance(init, ir.NewArray) and t.kind == 'array' and init.dims and not init.init:
            return self.x_NewArray(init)                 # new T[n]
        if isinstance(init, ir.NewArray) and t.kind == 'array':
            items =[self.init_value(t.elem, x).code for x in init.init or []]
            n = t.info.get('count', len(items))
            jt = self.p.jt(t)
            if not items:
                return J(self.p.default(t), jb.P_POST, jt)
            if len(items) < n:
                if is_record(t.elem):
                    items += ['new %s()' % self.p.cls(t.elem)] * (n - len(items))
                elif t.elem.kind == 'array':
                    items += [self.p.default(t.elem)] * (n - len(items))
                else:
                    return J('java.util.Arrays.copyOf(new %s{%s}, %d)' % (jt, ', '.join(items), n), jb.P_POST, jt)
            return J('new %s{%s}' % (jt, ', '.join(items)), jb.P_POST, jt)
        if isinstance(init, ir.New) and is_record(t) and init.sym is not None and init.sym.name.startswith('<aggregate'):
            return self.aggregate(t, init.args or [])
        if is_record(t):
            v = self.value(init)
            return v if v.fresh else self.copy_of(v, t)
        return self.conv(self.value(init), init.type, t)

    def aggregate(self, t, args):
        c = self.p.records.get(self.p.rec_name(t))
        fields = [m for m in c.members if isinstance(m, ir.FieldDecl) and 'static' not in (m.mods or [])]
        if not args:
            return J('new %s()' % self.p.cls(t), jb.P_POST, self.p.cls(t), fresh=True)
        self.p.ctors_used.add(c.qname)
        vals = []
        for i, f in enumerate(fields):
            if i < len(args):
                vals.append(self.init_value(f.type, args[i]).code)
            else:
                vals.append(self.p.default(f.type))
        return J('new %s(%s)' % (self.p.cls(t), ', '.join(vals)), jb.P_POST, self.p.cls(t), fresh=True)

    def copy_of(self, v, t):
        qn = self.p.rec_name(t)
        self.p.copied.add(qn)
        return J('new %s(%s)' % (self.p.cls(t), v.code), jb.P_POST, self.p.cls(t), fresh=True)

    def static_local(self, s, name):
        g = ir.FieldDecl(sym=ir.Sym('global', '%s_%s' % (self.fn.name, s.sym.name), None, True, s.type), type=s.type,
                         init=s.init, mods=['static'])
        if ('l', s.sym.owner, s.sym.name) in self.p.addr_taken:
            self.p.addr_taken.add(('g', g.sym.name))
        self.p.declare_global(g)
        self.vars[s.sym.name] = self.p.globals[g.sym.name]

    def ret(self, s, out):
        f = self.fn
        if s.expr is None:
            if f.ret is not None and f.ret.kind != 'void' and f.kind == 'method' or (
                    f.kind == 'function' and f.ret is not None and f.ret.kind != 'void'):
                out.append('return %s;  // (C: a return without a value)' % self.default_of(f.ret))
            else:
                out.append('return;')
            return
        if f.kind in ('ctor', 'dtor') or f.ret is None or f.ret.kind == 'void':
            self.expr_stmt(s.expr, out)
            out.append('return;')
            return
        rt = f.ret
        if rt.kind == 'ptr' and rt.info.get('ref'):
            out.append('return %s;' % self.value(strip(s.expr).x if isinstance(strip(s.expr), ir.Unary) and
                                                   strip(s.expr).op == '&' else s.expr).code)
            return
        if is_record(rt):
            e = strip(s.expr)
            v = self.value(e)
            local = isinstance(e, ir.Name) and e.sym.kind in ('local', 'param') and \
                ('l', e.sym.owner, e.sym.name) not in self.p.addr_taken
            out.append('return %s;' % (v.code if v.fresh or local else self.copy_of(v, rt).code))
            return
        out.append('return %s;' % self.conv(self.value(s.expr), s.expr.type, rt).code)

    def expr_stmt(self, e, out):
        e0 = strip(e)
        if isinstance(e0, ir.Call) and e0.target is None and e0.sym is not None and e0.sym.name == 'free' \
                and e0.sym.name not in self.p.functions:
            out.append('// free(%s): the collector frees it' % self.value(e0.args[0]).code)
            return
        if isinstance(e0, ir.Unary) and e0.op in ('delete', 'delete[]'):
            x = self.value(e0.x)
            t = e0.x.type
            if t is not None and t.kind == 'ptr' and is_record(t.elem) and self.has_destroy(t.elem) and e0.op == 'delete':
                out.append('%s.destroy();' % x.at(jb.P_POST))
            else:
                out.append('// %s %s: the collector frees it' % (e0.op, x.code))
            return
        jb.FnWriter.expr_stmt(self, e, out)

    def has_destroy(self, t):
        c = self.p.records.get(self.p.rec_name(t))
        while c is not None:
            if any(isinstance(m, ir.MethodDecl) and m.kind == 'dtor' and m.body is not None and
                   (m.body.stmts or 'virtual' in (m.mods or [])) for m in c.members):
                return True
            c = self.p.records.get(self.p.rec_name(c.extends)) if c.extends is not None else None
        return False

    # ------------------------------------------------------------------------------------- places
    def var(self, s):
        if s.kind in ('local', 'param') and s.name in self.vars:
            return self.vars[s.name]
        g = self.p.globals.get(s.name)
        if g is not None:
            return g
        raise Refused('unknown variable %s' % s.name)

    def place(self, e):
        """An lvalue -> Place whose `java` is a Java lvalue (mem is never True here)."""
        e = strip(e)
        if isinstance(e, ir.Name):
            v = self.var(e.sym)
            return jb.Place('%s[0]' % v.java if v.mem else v.java, False, v.type, prec=jb.P_POST)
        if isinstance(e, ir.Unary) and e.op == '*':
            x = self.value(e.x)
            if is_record(e.type):
                return jb.Place(x.code, False, e.type, prec=x.prec)
            return jb.Place('%s[0]' % x.at(jb.P_POST), False, e.type, prec=jb.P_POST)
        if isinstance(e, ir.Index):
            a = self.value(e.arr)
            i = self.conv(self.value(e.index), e.index.type, ir.I32)
            return jb.Place('%s[%s]' % (a.at(jb.P_POST), i.code), False, e.type, prec=jb.P_POST)
        if isinstance(e, ir.Select):
            return jb.Place(self.field(e), False, e.type, prec=jb.P_POST)
        if isinstance(e, ir.Cast):
            return self.place(e.x)
        raise Refused('an lvalue %s' % type(e).__name__)

    def field(self, e):
        name = jname(e.sym.name)
        obj = strip(e.obj)
        if isinstance(obj, ir.TypeName):
            owner = self.p.cls(obj.type)
            if self.cls is not None and self.p.cls_names.get(self.cls.qname) == owner:
                return name
            return '%s.%s' % (owner, name)
        if isinstance(obj, ir.Unary) and obj.op == '*' and isinstance(strip(obj.x), ir.This):
            return name if name not in self.declared else 'this.' + name
        o = self.value(obj)
        return '%s.%s' % (o.at(jb.P_POST), name)

    def load(self, pl):
        return J(pl.java, pl.prec, self.p.jt(pl.type))

    def store(self, pl, val):
        t = pl.type
        if is_record(t):
            self.p.assigned.add(self.p.rec_name(t))
            return J('%s.set(%s)' % (_at(pl), val.code), jb.P_POST, self.p.jt(t))
        return J('%s = %s' % (pl.java, val.at(jb.P_ASSIGN)), jb.P_ASSIGN, self.p.jt(t))

    def addr(self, e):
        e = strip(e)
        if isinstance(e, ir.Name):
            v = self.var(e.sym)
            return J(v.java, jb.P_ATOM, self.p.jt(e.type))
        if isinstance(e, ir.Unary) and e.op == '*':
            return self.value(e.x)
        if is_record(e.type) or (e.type is not None and e.type.kind == 'array'):
            return self.value(e)
        raise Refused('the address of %s' % type(e).__name__)

    # ------------------------------------------------------------------------------------- values
    def truth(self, j, t):
        if j.jt == 'boolean':
            return j
        if t is not None and t.kind in ('ptr', 'fnptr', 'null'):
            return J('%s != null' % j.at(jb.P_EQ + 1), jb.P_EQ, 'boolean')
        return jb.FnWriter.truth(self, j, t)

    def x_Concat(self, e):
        """Java string concatenation (a program read back from Java): Java's own semantics."""
        parts = [self.value(x) for x in e.parts]
        if not parts:
            return J('""', jb.P_ATOM, 'String')
        if parts[0].jt != 'String' and not (len(parts) > 1 and parts[1].jt == 'String'):
            parts.insert(0, J('""', jb.P_ATOM, 'String'))
        code = parts[0].at(jb.P_ADD) + ''.join(' + ' + p.at(jb.P_ADD + 1) for p in parts[1:])
        return J(code, jb.P_ADD, 'String')

    def x_Length(self, e):
        return J('%s.length' % self.value(e.arr).at(jb.P_POST), jb.P_POST, 'int')

    def x_Lit(self, e):
        if e.value is None and (e.type is None or e.type.kind in ('ptr', 'fnptr', 'null')):
            return J('null', jb.P_ATOM, 'Object')
        if isinstance(e.value, str) and e.type is not None and e.type.kind == 'class' and e.type.name == 'java.lang.String':
            return J(jb.java_wstring(e.value), jb.P_ATOM, 'String')
        if isinstance(e.value, str):
            if jb.is_wide(e):
                raise Refused('a wide string outside a library call')
            return J('bytes(%s)' % jb.java_string(e.value), jb.P_POST, 'byte[]')
        return jb.FnWriter.x_Lit(self, e)

    def x_Name(self, e):
        v = self.var(e.sym)
        if v.mem:
            return J('%s[0]' % v.java, jb.P_POST, self.p.jt(v.type))
        return J(v.java, jb.P_ATOM, self.p.jt(v.type))

    def x_Select(self, e):
        return J(self.field(e), jb.P_POST, self.p.jt(e.type))

    def x_Index(self, e):
        return self.load(self.place(e))

    def x_This(self, e):
        return J('this', jb.P_ATOM, self.p.cls(self.cls and ir.Type('class', self.cls.qname)))

    def x_AddressOf(self, e):
        name = e.sym.name
        if name not in self.p.functions:
            raise Refused('the address of library function %s' % name)
        return J('%s::%s' % (self.p.name, jname(name)), jb.P_POST, self.p.jt(e.type))

    def x_Unary(self, e):
        if e.op == '*':
            x = strip(e.x)
            if isinstance(x, ir.This):
                return J('this', jb.P_ATOM, self.p.jt(e.type))
            if is_record(e.type):
                return self.value(e.x)
            return self.load(self.place(e))
        if e.op == '&':
            x = strip(e.x)
            if isinstance(x, ir.AddressOf):
                return self.value(x)
            return self.addr(x)
        if e.op in ('++', '--') and is_ptr(e.type):
            raise Refused('pointer arithmetic')
        return jb.FnWriter.x_Unary(self, e)

    def incdec(self, e, as_statement=False):
        pl = self.place(e.x)
        if _kind(e.type) == 'bool':
            raise Refused('++ on a bool')
        code = (pl.java + e.op) if e.postfix else (e.op + pl.java)
        return J(code, jb.P_POST if e.postfix else jb.P_UNARY, self.p.jt(e.type))

    def compare(self, e, op):
        lt, rt = e.left.type, e.right.type
        if (lt is not None and lt.kind in ('ptr', 'fnptr', 'null')) or (rt is not None and rt.kind in ('ptr', 'fnptr', 'null')):
            l, r = self.value(e.left), self.value(e.right)
            return J('%s %s %s' % (l.at(jb.P_EQ), op, r.at(jb.P_EQ + 1)), jb.P_EQ, 'boolean')
        return jb.FnWriter.compare(self, e, op)

    def x_Assign(self, e):
        if e.op is None:
            t = e.target.type
            pl = self.place(e.target)
            if is_record(t):
                return self.store(pl, self.value(e.value))
            v = self.value(e.value)
            return self.store(pl, self.conv(v, e.value.type, t))
        return jb.FnWriter.x_Assign(self, e)

    def x_Cast(self, e):
        x = strip(e.x)
        t = e.type
        st = x.type
        if t.kind == 'void':
            return self.value(x)
        if t.kind in ('ptr', 'fnptr') and jb._null(x):
            return J('null', jb.P_ATOM, 'Object')
        if isinstance(x, ir.Call) and x.target is None and x.sym is not None and x.sym.name in ('malloc', 'calloc') \
                and x.sym.name not in self.p.functions:
            return self.allocation(x, t)
        if st is not None and st.kind == 'array' and is_ptr(t):
            return self.value(x)                         # an array is its own pointer
        if isinstance(x, ir.Lit) and x.value is None:
            return J('null', jb.P_ATOM, 'Object')
        if isinstance(x, ir.AddressOf):
            return self.value(x)
        j = self.value(x)
        return self.conv(j, st, t)

    def conv(self, j, s, t):
        if s is None or t is None:
            return j
        refs = ('ptr', 'fnptr', 'null', 'class', 'record', 'array')
        if s.kind in refs or t.kind in refs:
            if t.kind == 'bool':
                return J('%s != null' % j.at(jb.P_EQ + 1), jb.P_EQ, 'boolean')
            if (s.kind in refs) != (t.kind in refs) and t.kind != 'void':
                raise Refused('a pointer used as an integer, or the reverse')
            return j
        return jb.FnWriter.conv(self, j, s, t)

    def allocation(self, call, t):
        """malloc/calloc(...) converted to T*: a new object or array of T."""
        if t.kind != 'ptr' or t.elem is None or t.elem.kind == 'void':
            raise Refused('an allocation of no type')
        el = t.elem
        size = self.p.size_hint(el)
        args = call.args
        n = None
        if call.sym.name == 'calloc':
            n = self.count(args[0], None) if self.is_size(args[1], el) else None
        else:
            n = self.count(args[0], el)
        if n is None:
            raise Refused('an allocation whose size is not a count of %r' % (el,))
        if is_record(el):
            if n == '1':
                return J('new %s()' % self.p.cls(el), jb.P_POST, self.p.cls(el), fresh=True)
            self.p.arrays_filled = True
            return J('filled(new %s[%s], %s::new)' % (self.p.cls(el), n, self.p.cls(el)), jb.P_POST,
                     self.p.cls(el) + '[]')
        return J('new %s[%s]' % (self.p.jt(el), n) if el.kind != 'array' else
                 'new %s[%s]%s' % (self.p.jt(_base(el)), n, ''.join('[%d]' % d for d in _dims(el))),
                 jb.P_POST, self.p.jt(t))

    def is_size(self, e, el):
        e = strip(e)
        if isinstance(e, ir.Cast):
            e = strip(e.x)
        return isinstance(e, ir.Lit) and isinstance(e.value, int) and e.value == self.p.size_hint(el)

    def count(self, e, el):
        """The element count an allocation size expresses ('n * sizeof(T)', 'sizeof(T)', a byte count of
        chars), as Java int text; or None."""
        e = strip(e)
        while isinstance(e, ir.Cast):
            e = strip(e.x)
        if el is None:
            return self.conv(self.value(e), e.type, ir.I32).code
        size = self.p.size_hint(el)
        if self.is_size(e, el):
            return '1'
        if isinstance(e, ir.Binary) and e.op == '*':
            for a, b in ((e.left, e.right), (e.right, e.left)):
                if self.is_size(b, el):
                    a0 = strip(a)
                    while isinstance(a0, ir.Cast):
                        a0 = strip(a0.x)
                    return self.conv(self.value(a0), a0.type, ir.I32).code
        if size == 1:
            return self.conv(self.value(e), e.type, ir.I32).code
        return None

    def x_Call(self, e):
        args = e.args or []
        if e.sym is not None and e.sym.kind in ('method', 'ctor') and e.target is not None:
            return self.method_call(e)
        if e.sym is not None and e.target is None and e.sym.name in self.p.functions:
            f = self.p.function_for(e.sym)
            ac = self.call_args(args, f.sym, [p.type for p in f.params])
            rt = f.ret
            return J('%s(%s)' % (jname(e.sym.name), ', '.join(ac)), jb.P_POST, self.p.jt(rt) if rt else 'void',
                     fresh=rt is not None and is_record(rt))
        if e.sym is not None and e.target is None and e.sym.name in ('malloc', 'calloc'):
            raise Refused('an allocation used without a type')
        target = strip(e.target) if e.target is not None else None
        if target is not None and target.type is not None and target.type.kind in ('fnptr', 'fn'):
            fn = self.value(target)
            ft = target.type if target.type.kind == 'fnptr' else ir.Type('fnptr', space='flat', fn=target.type)
            params = [p['type'] for p in ft.info['fn'].info['params']]
            ac = [self.conv(self.value(a), a.type, params[i] if i < len(params) else a.type).code for i, a in enumerate(args)]
            return J('%s.call(%s)' % (fn.at(jb.P_POST), ', '.join(ac)), jb.P_POST, self.p.fn_sig_of_type(ft)[1])
        if e.sym is not None and e.target is None:
            return self.lib_call(e, e.sym.name, args)
        raise Refused('a call this back end cannot place')

    def call_args(self, args, sym, params=None):
        params = params if params is not None else (list(sym.params) if sym is not None else [])
        out = []
        for i, a in enumerate(args):
            pt = params[i] if i < len(params) else a.type
            if is_record(pt):
                v = self.value(a)
                out.append(v.code if v.fresh else self.copy_of(v, pt).code)
            elif pt is not None and pt.kind == 'ptr' and pt.info.get('ref'):
                aa = strip(a)
                if isinstance(aa, ir.Unary) and aa.op == '&':
                    out.append(self.addr(aa.x).code)
                else:
                    out.append(self.value(a).code)
            else:
                out.append(self.conv(self.value(a), a.type, pt).code)
        return out

    def method_call(self, e):
        s = e.sym
        target = strip(e.target)
        name = method_name(s.name)
        m = self.find_method(s)
        params = [p.type for p in m.params] if m is not None else list(s.params)
        ac = self.call_args(e.args or [], s, params)
        rt = e.type
        jt = self.p.jt(rt) if rt is not None and rt.kind != 'void' else 'void'
        fresh = rt is not None and is_record(rt) and not (m is not None and m.ret is not None and m.ret.kind == 'ptr')
        if isinstance(target, ir.TypeName):
            owner = self.p.cls(target.type)
            here = self.cls is not None and self.p.cls_names.get(self.cls.qname) == owner
            return J('%s(%s)' % (name if here else '%s.%s' % (owner, name), ', '.join(ac)), jb.P_POST, jt, fresh)
        if isinstance(target, ir.Unary) and target.op == '*' and isinstance(strip(target.x), ir.This):
            return J('%s(%s)' % (name, ', '.join(ac)), jb.P_POST, jt, fresh)
        obj = self.value(target)
        return J('%s.%s(%s)' % (obj.at(jb.P_POST), name, ', '.join(ac)), jb.P_POST, jt, fresh)

    def find_method(self, s):
        c = self.p.records.get(s.owner)
        while c is not None:
            for m in c.members:
                if isinstance(m, ir.MethodDecl) and m.name == s.name and len(m.params) == len(s.params):
                    return m
            c = self.p.records.get(self.p.rec_name(c.extends)) if c.extends is not None else None
        return None

    def x_New(self, e):
        t = e.type
        if e.memory == 'manual' and t is not None and t.kind == 'ptr':
            el = t.elem
            if is_record(el):
                return self.construct(el, e)
            if e.args:
                return J('new %s[] {%s}' % (self.p.jt(el), self.conv(self.value(e.args[0]), e.args[0].type, el).code),
                         jb.P_POST, self.p.jt(t))
            return J('new %s[1]' % self.p.jt(el), jb.P_POST, self.p.jt(t))
        if is_record(t):
            return self.construct(t, e)
        raise Refused('new (%s)' % (e.memory,))

    def construct(self, t, e):
        if e.sym is not None and e.sym.name.startswith('<aggregate'):
            return self.aggregate(t, e.args or [])
        c = self.p.records.get(self.p.rec_name(t))
        ctor = None
        if c is not None:
            for m in c.members:
                if isinstance(m, ir.MethodDecl) and m.kind == 'ctor' and len(m.params) == len(e.args or []) and \
                        [spell_t(p) for p in e.sym.params] == [spell_t(p.type) for p in m.params]:
                    ctor = m
        params = [p.type for p in ctor.params] if ctor is not None else list(e.sym.params)
        ac = self.call_args(e.args or [], e.sym, params)
        return J('new %s(%s)' % (self.p.cls(t), ', '.join(ac)), jb.P_POST, self.p.cls(t), fresh=True)

    def x_NewArray(self, e):
        t = e.type
        if t is not None and t.kind == 'ptr':
            el = t.elem
            n = self.conv(self.value(e.dims[0]), e.dims[0].type, ir.I32).code if e.dims else '0'
            if is_record(el):
                self.p.arrays_filled = True
                return J('filled(new %s[%s], %s::new)' % (self.p.cls(el), n, self.p.cls(el)), jb.P_POST, self.p.jt(t))
            if e.init:
                return J('java.util.Arrays.copyOf(new %s[] {%s}, %s)' % (
                    self.p.jt(el), ', '.join(self.conv(self.value(x), x.type, el).code for x in e.init), n),
                    jb.P_POST, self.p.jt(t))
            return J('new %s[%s]' % (self.p.jt(el), n), jb.P_POST, self.p.jt(t))
        if t is not None and t.kind == 'array':
            if e.dims and 'count' not in t.info:            # new T[n] (a Java array read back)
                n = self.conv(self.value(e.dims[0]), e.dims[0].type, ir.I32).code
                base = _base(t)
                return J('new %s[%s]%s' % (self.p.jt(base), n, '[]' * (len(_dims(t)) - 1)), jb.P_POST, self.p.jt(t))
            return self.init_value(t, e)
        raise Refused('an array value')

    def x_Cond(self, e):
        if is_record(e.type):
            c = self.cond(e.cond)
            a, b = self.value(e.then), self.value(e.orelse)
            return J('%s ? %s : %s' % (c.at(jb.P_COND + 1), a.at(jb.P_COND + 1), b.at(jb.P_COND)), jb.P_COND,
                     self.p.jt(e.type))
        return jb.FnWriter.x_Cond(self, e)

    def lib_call(self, e, name, args):
        if name.startswith('__builtin_va'):
            raise Refused('a variadic function')
        from codebreaker.translate.backends import java_libc
        return java_libc.call(self, e, name, args)


def _at(pl):
    return pl.java if pl.prec >= jb.P_POST else '(%s)' % pl.java


def _base(t):
    while t.kind == 'array':
        t = t.elem
    return t


def _dims(t):
    out = []
    while t.kind == 'array':
        out.append(t.info.get('count', 0))
        t = t.elem
    return out


def spell_t(t):
    return jb.spell(t)
