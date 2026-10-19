"""cpp03.py -- method bodies from the IR as C++03, beside the Java transpiler's text path (phase 1).

The transpiler (codebreaker/java) still writes every declaration: headers, signatures, fields,
constructors. This back end writes method BODIES from the typed tree, in the transpiler's conventions
-- the same TypeResolver decides pointer or value, `s_` names statics, arrays are JArray<T>, Strings
JavaString, concatenation JCat -- so a body from either path fits the same declarations.

It declines a method rather than guess: anything it does not handle yet (a construct, a value type,
a label, `finally`) raises Decline, and the transpiler's own translation is used for that method.
Whether what it writes is right is decided by the judge, class by class (probe.py).
"""
import re

from codebreaker.java import ir as jir
from codebreaker.java import typemap
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
        for ctype, name in _split_params(ownership.params_str(old, self.r)):
            self.vars[name] = ctype
        self.parent = cls.extends.simple if cls.extends is not None else None
        self.ret = ownership.return_cpp_type(old, self.r)

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
        for s in self.m.body.stmts:
            self.stmt(s, out, '')
        return out

    def block(self, s, out, ind):
        if isinstance(s, ir.Block):
            for x in s.stmts:
                self.stmt(x, out, ind + '\t')
        else:
            self.stmt(s, out, ind + '\t')

    def stmt(self, s, out, ind):
        if isinstance(s, ir.Block):
            out.append(ind + '{')
            self.block(s, out, ind)
            out.append(ind + '}')
        elif isinstance(s, ir.Local):
            name = self.name(s.sym.name)
            if name in self.vars:
                raise Decline('shadowed local %s' % name)
            ct = self.ctype(s.type)
            self.vars[name] = ct
            if s.init is not None:
                code, it = self.expr(s.init)
                out.append(ind + '%s %s = %s;' % (ct, name, unbox(coerce(code, it, ct), it, ct)))
            else:
                out.append(ind + '%s %s;' % (ct, name))
        elif isinstance(s, ir.ExprStmt):
            out.append(ind + self.expr(s.expr)[0] + ';')
        elif isinstance(s, ir.If):
            out.append(ind + 'if (%s) {' % self.cond(s.cond))
            self.block(s.then, out, ind)
            e = s.orelse
            while isinstance(e, ir.If):
                out.append(ind + '} else if (%s) {' % self.cond(e.cond))
                self.block(e.then, out, ind)
                e = e.orelse
            if e is not None:
                out.append(ind + '} else {')
                self.block(e, out, ind)
            out.append(ind + '}')
        elif isinstance(s, ir.While):
            out.append(ind + 'while (%s) {' % self.cond(s.cond))
            self.block(s.body, out, ind)
            out.append(ind + '}')
        elif isinstance(s, ir.DoWhile):
            out.append(ind + 'do {')
            self.block(s.body, out, ind)
            out.append(ind + '} while (%s);' % self.cond(s.cond))
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
            self.block(s.body, out, ind)
            out.append(ind + '}')
            for l in s.init:                              # the loop variable's scope ends with the loop
                if isinstance(l, ir.Local):
                    self.vars.pop(self.name(l.sym.name), None)
        elif isinstance(s, ir.Return):
            if s.expr is not None:
                code, it = self.expr(s.expr)
                out.append(ind + 'return %s;' % unbox(coerce(code, it, self.ret), it, self.ret))
            else:
                out.append(ind + 'return;')
        elif isinstance(s, ir.Break):
            if s.label:
                raise Decline('labelled break')
            out.append(ind + 'break;')
        elif isinstance(s, ir.Continue):
            if s.label:
                raise Decline('labelled continue')
            out.append(ind + 'continue;')
        elif isinstance(s, ir.Throw):
            out.append(ind + 'throw %s;' % self.expr(s.expr)[0])
        elif isinstance(s, ir.Switch):
            x, xt = self.expr(s.expr)
            if 'JavaString' in xt:
                raise Decline('switch on a string')
            out.append(ind + 'switch (%s) {' % x)
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
            if s.resources or s.final is not None:
                raise Decline('finally / resources')
            # A catch that a NullPointerException reaches needs the null checks inside the try to jump to
            # it (Java throws where C++ would crash). The text path models that (_npe_to_catch); the IR
            # will in phase 2's exceptions pass. Until then such a try is the text path's.
            if any(t.kind == 'class' and t.name in _CATCHES_NPE for c in s.catches for t in c.types):
                raise Decline('catch reached by a NullPointerException')
            out.append(ind + 'try {')
            self.block(s.body, out, ind)
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
        elif isinstance(s, ir.Empty):
            out.append(ind + ';')
        else:
            raise Decline('statement %s' % type(s).__name__)

    def cond(self, e):
        return self.expr(e)[0]

    # ------------------------------------------------------------------ expressions -> (code, C++ type)
    def name(self, n):
        return n + '_' if n in _CPP_KEYWORDS else n

    def expr(self, e):
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
            if owner not in self.ctx.classes:
                if s.owner == 'java.lang.Math' and s.name == 'PI':
                    return typemap.MATH_API['PI'], 'double'
                if owner in typemap.RUNTIME_ENUM_LIKE:     # Integer.MAX_VALUE, GL11.GL_BLEND, System.out
                    return '%s::%s' % (owner, s.name), self.library_field(owner, s.name, e.type)
                raise Decline('library static %s.%s' % (owner, s.name))
            ct = self.ctx.field_ctype(owner, s.name)
            if ct is None:
                raise Decline('static field %s.%s' % (owner, s.name))
            if owner in self.r.enums:                     # an enum's statics keep their bare names
                return '%s::%s' % (owner, s.name), ct
            return '%s::%s' % (owner, naming.static_field_name(s.name)), ct
        if s.static:
            raise Decline('static field through an instance')
        if isinstance(e.obj, ir.This):
            recv, rt = 'this', self.cname + '*'
        else:
            recv, rt = self.expr(e.obj)
        base = _class_of(rt)
        ct = self.ctx.field_ctype(base or owner, s.name) or self.ctx.field_ctype(owner, s.name)
        if ct is None:
            raise Decline('field %s.%s' % (owner, s.name))
        return '%s%s%s' % (recv, '->' if is_pointer(rt) else '.', s.name), ct

    def x_Length(self, e):
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
        if e.op in ('==', '!=') and ('JavaString' in lt or 'JavaString' in rt) and 'void*' not in (lt, rt):
            raise Decline('string identity comparison')
        if e.op == '%' and e.type.kind in ('f32', 'f64'):     # Java's floating remainder: C's fmod
            return ('(float)fmod((double)(%s), (double)(%s))' if e.type.kind == 'f32' else 'fmod(%s, %s)') % (l, r),                 self.ctype(e.type)
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
        return '(%s)%s' % (ct, x), ct

    def x_InstanceOf(self, e):
        x, xt = self.expr(e.x)
        target = jtype(e.test)
        if target.is_array or not self.r.is_pointer(target):
            raise Decline('instanceof %s' % target.name)
        if xt.startswith('JObject'):
            return '(%s).is<%s>()' % (x, target.name), 'bool'
        if not is_pointer(xt):
            raise Decline('instanceof on %s' % xt)
        return '(dynamic_cast<%s*>(%s) != NULL)' % (target.name, x), 'bool'

    def x_Call(self, e):
        s = e.sym
        if s is None or s.kind != 'method' or s.varargs:
            raise Decline('call')
        owner = s.owner_simple
        if any(a.type is not None and a.type.kind == 'class' and a.type.simple in self.r.markers for a in e.args):
            raise Decline('marker argument')               # the decompiler's EmptyN bridge parameters
        if owner in typemap.BOXED_VALUE_TYPES and s.name.endswith('Value') and not s.params                 and e.target is not None and not isinstance(e.target, ir.TypeName):
            recv, rt = self.expr(e.target)                # Integer.intValue(): a boxed type is a primitive
            if not rt.startswith('JObject'):              # here, unless it is still in its JObject box
                ct = self.ctype(e.type)
                return '((%s)(%s))' % (ct, recv), ct
        if s.name == 'ordinal' and not s.params and e.target is not None:
            recv, rt = self.expr(e.target)
            if rt == 'int':                               # a constant-only enum is its ordinal
                return '(%s)' % recv, 'int'
        args = ', '.join(self.expr(a)[0] for a in e.args)
        if s.owner == 'java.lang.Object':                 # getClass/hashCode/equals on Object: the
            raise Decline('Object method %s' % s.name)    # transpiler's own lowering (j2xClassOf, ...)
        old = self.ctx.old_method(owner, s.name, s.params) if owner in self.ctx.classes else None
        if owner in self.ctx.classes:
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
            if owner not in self.ctx.classes:
                if s.owner == 'java.lang.Math':
                    fn = _MATH.get(s.name)
                    if fn is None:
                        raise Decline('Math.%s' % s.name)
                    return '%s(%s)' % (fn, args), self.ctype(e.type)
                if owner not in typemap.RUNTIME_STATIC_HOLDERS and owner not in typemap.JAVART_STATICS:
                    raise Decline('library static call %s.%s' % (owner, s.name))
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
        return '%s%s%s(%s)' % (recv, '->' if is_pointer(rt) else '.', name, args), ret

    def library_field(self, owner, name, t):
        if owner == 'System' and name in ('out', 'err'):
            return 'System::_PrintStream'                 # a value in the runtime (JavaRT.h)
        return self.ctype(t)

    def library_return(self, owner, name, ret):
        """The C++ return type of a runtime (library) method: what the runtime's own stub registry says,
        else the resolver's mapping of the Java type."""
        from codebreaker.java import runtime
        base = re.sub(r'<.*', '', self.r.base_cpp(owner))
        rec, seen = runtime.STUB_MEMBERS.get(base) or runtime.STUB_MEMBERS.get(owner), set()
        while rec is not None and id(rec) not in seen:
            seen.add(id(rec))
            t = rec['methods'].get(name)
            if t:
                return 'JavaString' if t == 'std::string' else t
            rec = runtime.STUB_MEMBERS.get(rec.get('parent'))
        if name == 'iterator':
            return 'Iterator*'
        declared = _runtime_return(owner, name)          # what the runtime's own header declares
        if declared:
            return declared
        if ret is not None and ret.kind == 'class' and ret.simple == owner and owner.endswith('Buffer'):
            return owner + '&'                            # NIO buffers chain by reference (put().put())
        if ret is None or ret.kind == 'void':
            return 'void'
        return self.ctype(ret, 'return')

    def x_New(self, e):
        if e.body is not None or e.outer is not None:
            raise Decline('anonymous or inner class')
        t = e.type
        jt = jtype(t)
        if jt.name in self.r.values:
            raise Decline('value type %s' % jt.name)
        if any(a.type is not None and a.type.kind == 'class' and a.type.simple in self.r.markers for a in e.args):
            raise Decline('marker argument')
        args = ', '.join(self.expr(a)[0] for a in e.args)
        if jt.name in self.ctx.classes:
            if self.r.extends_value_container(jt.name):
                raise Decline('new of a container subclass')
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
        if len(e.dims) != 1 or e.type.dims()[0] != 1:
            raise Decline('multi-dimensional array')
        return '%s(%s)' % (ct, self.expr(e.dims[0])[0]), ct


# java.lang.Math as C: the transpiler's table, but each function at Java's own precision (double),
# where the transpiler has the float forms (floorf, powf, ...) for the 360's float pipeline
_MATH = dict(typemap.MATH_API_EMIT, floor='floor', ceil='ceil', pow='pow', exp='exp', log='log')
_MATH.pop('PI', None)


_BITS = frozenset(('<<', '>>', '&', '|', '^'))
_CATCHES_NPE = frozenset(('java.lang.Exception', 'java.lang.RuntimeException', 'java.lang.Throwable',
                          'java.lang.NullPointerException'))
_UNBOX = {'int': 'intValue', 'short': 'shortValue', 'signed char': 'byteValue', 'long long': 'longValue',
          'float': 'floatValue', 'double': 'doubleValue', 'bool': 'booleanValue', 'unsigned char': 'charValue'}


def _runtime_return(owner, name):
    """The return type `owner.h` in the runtime declares for `name` (a static holder's inline function,
    a stub class's method), or None."""
    from codebreaker.java import runtime
    text = runtime.FILES.get(owner + '.h') or getattr(runtime, '_GENERATED', {}).get(owner + '.h')
    if not text:
        return None
    m = re.search(r'(?:inline|virtual|static)\s+([A-Za-z_][\w:<>,* &]*?)\s*\b%s\s*\(' % re.escape(name), text)
    if not m:
        return None
    t = m.group(1).replace('virtual ', '').replace('inline ', '').replace('static ', '').strip()
    return 'JavaString' if t == 'std::string' else t


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
        elif o < 32 or o == 127:
            out.append('\\%03o' % o)
        elif o > 127:
            out.append(''.join('\\%03o' % b for b in ch.encode('utf-8')))
        else:
            out.append(ch)
    return '"%s"' % ''.join(out)


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
        self.written = 0
        self.declined = collections.Counter()

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
