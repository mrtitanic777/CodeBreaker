"""cpp17.py -- L2 into C++17 (MSVC 2022 / VS 18 toolset), for the trees the Python front end writes.

cpp03.py writes method bodies into the Java transpiler's declarations; a Python module has no such
declarations, so this back end writes the whole module: a header (records, classes, functions, the
module's names) and a source file with every body. The C++ it writes reads as a person would write the
same program: int64_t where Python had an int, value structs for records, references (py::ref) where
Python shared objects, the support header's types -- py::str, py::bytes, py::list, py::dict -- where
Python's semantics must hold (negative indices, slicing, insertion order, floor division, struct).

Python's own operations arrive as calls owned by "py": the support header's core semantics are written
here; everything else is a row of the API map (codebreaker/java/api_maps/python.json, target "cpp"),
whose target is a template ({recv}, {0}, {1}, {args}).
"""
import json
import re

from codebreaker.translate import ir

_KEYWORDS = frozenset('''alignas alignof and and_eq asm auto bitand bitor bool break case catch char char16_t char32_t
class compl const constexpr const_cast continue decltype default delete do double dynamic_cast else enum explicit
export extern false float for friend goto if inline int long mutable namespace new noexcept not not_eq nullptr
operator or or_eq private protected public register reinterpret_cast return short signed sizeof static static_assert
static_cast struct switch template this thread_local throw true try typedef typeid typename union unsigned using
virtual void volatile wchar_t while xor xor_eq NULL errno assert min max small near far stdin stdout stderr'''.split())

_PREC = {'*': 5, '/': 5, '%': 5, '+': 6, '-': 6, '<<': 7, '>>': 7, '<': 9, '<=': 9, '>': 9, '>=': 9, '==': 10, '!=': 10,
         '&': 11, '^': 12, '|': 13, '&&': 14, '||': 15}


class Decline(Exception):
    pass


def ident(name):
    return name + '_' if name in _KEYWORDS else name


_REG = None


def api(name):
    """the API map's C++ target for a Python name, or None"""
    global _REG
    if _REG is None:
        from codebreaker.java import apimap
        _REG = apimap.load()
    m = _REG.resolve(name, 'python', 'cpp', 'c++17')
    if m is None or m.name != name or m.kind == 'unsupported':
        return None
    return m.target


def cpp_string(s):
    """a C++ string literal holding the UTF-8 of `s`"""
    out = []
    for ch in s.encode('utf-8'):
        c = chr(ch)
        if c == '\\':
            out.append('\\\\')
        elif c == '"':
            out.append('\\"')
        elif c == '\n':
            out.append('\\n')
        elif c == '\t':
            out.append('\\t')
        elif c == '\r':
            out.append('\\r')
        elif 0x20 <= ch < 0x7F:
            out.append(c)
        else:
            out.append('\\%03o' % ch)
    return '"' + ''.join(out) + '"'


def cpp_bytes(s):
    """a py::bytes literal from a latin-1 string"""
    raw = s.encode('latin1')
    body = []
    for ch in raw:
        c = chr(ch)
        if c == '\\':
            body.append('\\\\')
        elif c == '"':
            body.append('\\"')
        elif 0x20 <= ch < 0x7F and c != '?':
            body.append(c)
        else:
            body.append('\\%03o' % ch)
    if 0 in raw or any(ch >= 0x80 for ch in raw):
        return 'py::bytes(%s, %d)' % ('"' + ''.join(body) + '"', len(raw))
    return 'py::bytes(%s)' % ('"' + ''.join(body) + '"')


class Writer(object):
    """C++ text for the L2 of one module."""

    def __init__(self, module, records=None, ref_classes=None, ns=None):
        self.module = module                 # the module's qualified name (namespace)
        self.records = records or {}         # record qualname -> is_ref
        self.ns = ns or module.replace('.', '::')
        self.this_class = None
        self.locals = set()

    # ------------------------------------------------------------------------------------ names, types
    def qual(self, q):
        """a qualified Python name as C++, relative to this module"""
        mod, _, name = q.rpartition('.')
        if mod == self.module:
            return ident(name)
        if not mod:
            return ident(name)
        return '::' + '::'.join(ident(x) for x in q.split('.'))

    def ctype(self, t, param=False):
        k = t.kind
        if k == 'i64':
            return 'int64_t'
        if k == 'bool':
            return 'bool'
        if k == 'f64':
            return 'double'
        if k == 'void':
            return 'void'
        if k == 'class':
            n = t.name
            if n == 'py.optional':
                inner = t.args[0]
                if inner.kind == 'class' and inner.info.get('ref'):
                    return self.ctype(inner)
                return 'std::optional<%s>' % self.ctype(inner)
            if n == 'py.tuple':
                return 'std::tuple<%s>' % ', '.join(self.ctype(a) for a in t.args)
            if n == 'py.iter':
                raise Decline('an iterator as a declared type')
            if n in ('py.list', 'py.set', 'py.dict'):
                return 'py::%s<%s>' % (n[3:], ', '.join(self.ctype(a) for a in t.args))
            if n.startswith('py.'):
                return 'py::' + n[3:]
            if t.info.get('record'):
                base = self.qual(n)
                return 'py::ref<%s>' % base if t.info.get('ref') else base
            if t.info.get('exception'):
                return self.qual(n)
            return 'py::ref<%s>' % self.qual(n)
        raise Decline('type %r' % (t,))

    def param_type(self, t, assigned):
        c = self.ctype(t)
        if not assigned and t.kind == 'class' and (t.name in ('py.str', 'py.bytes', 'py.bytearray', 'py.tuple') or
                                                   (t.info.get('record') and not t.info.get('ref'))):
            return 'const %s&' % c
        return c

    def zero(self, t):
        k = t.kind
        if k == 'i64':
            return '0'
        if k == 'bool':
            return 'false'
        if k == 'f64':
            return '0.0'
        return None

    def none_of(self, t):
        if t.kind == 'class' and t.name == 'py.optional':
            inner = t.args[0]
            if inner.kind == 'class' and inner.info.get('ref'):
                return 'nullptr'
            return 'std::nullopt'
        if t.kind == 'class' and t.info.get('ref'):
            return 'nullptr'
        raise Decline('None as %r' % (t,))

    def is_nullable_ref(self, t):
        if t.kind == 'class' and t.name == 'py.optional':
            t = t.args[0]
        return t.kind == 'class' and bool(t.info.get('ref'))

    # ------------------------------------------------------------------------------------ statements
    def body(self, block, ind=1, ctor_init=None):
        out = []
        stmts = list(block.stmts)
        for s in stmts:
            self.stmt(s, out, ind)
        return out

    def stmt(self, s, out, ind):
        pad = '    ' * ind
        k = type(s).__name__
        if k == 'Block':
            out.append(pad + '{')
            for x in s.stmts:
                self.stmt(x, out, ind + 1)
            out.append(pad + '}')
        elif k == 'Local':
            self.locals.add(s.sym.name)
            t = s.type
            const = 'const ' if 'const' in (s.mods or []) else ''
            if s.init is None:
                z = self.zero(t)
                if z is None and t.kind == 'class' and t.name == 'py.optional':
                    out.append('%s%s%s %s;' % (pad, const, self.ctype(t), ident(s.sym.name)))
                elif z is None:
                    out.append('%s%s%s %s;' % (pad, const, self.ctype(t), ident(s.sym.name)))
                else:
                    out.append('%s%s%s %s = %s;' % (pad, const, self.ctype(t), ident(s.sym.name), z))
            else:
                init = self.expr(s.init, want=t)
                if isinstance(s.init, ir.Call) and s.init.sym.owner == 'py' and (
                        s.init.sym.name == 'empty' or (s.init.sym.name in ('list', 'set', 'dict') and not s.init.args)
                        or (s.init.sym.name == 'record' and init.endswith('{}') and not s.init.type.info.get('ref'))):
                    out.append('%s%s%s %s;' % (pad, const, self.ctype(t), ident(s.sym.name)))
                else:
                    out.append('%s%s%s %s = %s;' % (pad, const, self.ctype(t), ident(s.sym.name), init))
        elif k == 'ExprStmt':
            e = s.expr
            if isinstance(e, ir.Assign) and isinstance(e.target, ir.Call) and e.target.sym.owner == 'py' \
                    and e.target.sym.name in ('bind', 'tie'):
                names = [ident(a.sym.name) for a in e.target.args]
                for n in names:
                    self.locals.add(n)
                if e.target.sym.name == 'bind':
                    out.append('%sauto [%s] = %s;' % (pad, ', '.join(names), self.expr(e.value)))
                else:
                    out.append('%sstd::tie(%s) = %s;' % (pad, ', '.join(names), self.expr(e.value)))
                return
            out.append(pad + self.expr(e) + ';')
        elif k == 'If':
            out.append('%sif (%s) {' % (pad, self.expr(s.cond)))
            self.inner(s.then, out, ind + 1)
            orelse = s.orelse
            while orelse is not None:
                if isinstance(orelse, ir.If):
                    out.append('%s} else if (%s) {' % (pad, self.expr(orelse.cond)))
                    self.inner(orelse.then, out, ind + 1)
                    orelse = orelse.orelse
                else:
                    out.append(pad + '} else {')
                    self.inner(orelse, out, ind + 1)
                    orelse = None
            out.append(pad + '}')
        elif k == 'While':
            out.append('%swhile (%s) {' % (pad, self.expr(s.cond)))
            self.inner(s.body, out, ind + 1)
            out.append(pad + '}')
        elif k == 'For':
            init = s.init[0]
            ini = '%s %s = %s' % (self.ctype(init.type), ident(init.sym.name), self.expr(init.init))
            self.locals.add(init.sym.name)
            upd = ', '.join(self.expr(u) for u in s.update)
            out.append('%sfor (%s; %s; %s) {' % (pad, ini, self.expr(s.cond), upd))
            self.inner(s.body, out, ind + 1)
            out.append(pad + '}')
        elif k == 'ForEach':
            v = s.var
            if 'bind' in (v.mods or []):
                names = v.sym.name.split(',')
                for n in names:
                    self.locals.add(n)
                head = 'const auto& [%s]' % ', '.join(ident(n) for n in names)
            else:
                self.locals.add(v.sym.name)
                head = '%s %s' % (self.loop_var_type(v.type), ident(v.sym.name))
            out.append('%sfor (%s : %s) {' % (pad, head, self.iterable(s.iterable)))
            self.inner(s.body, out, ind + 1)
            out.append(pad + '}')
        elif k == 'Break':
            out.append(pad + 'break;')
        elif k == 'Continue':
            out.append(pad + 'continue;')
        elif k == 'Return':
            if s.expr is None:
                out.append(pad + 'return;')
            else:
                out.append(pad + 'return %s;' % self.expr(s.expr, want=self.ret))
        elif k == 'Throw':
            if s.expr is None:
                out.append(pad + 'throw;')
            else:
                out.append(pad + 'throw %s;' % self.expr(s.expr))
        elif k == 'Try':
            out.append(pad + 'try {')
            self.inner(s.body, out, ind + 1)
            for c in s.catches:
                for i, t in enumerate(c.types):
                    out.append('%s} catch (const %s&) {' % (pad, self.exc_type(t)))
                    self.inner(c.body, out, ind + 1)
            out.append(pad + '}')
        elif k == 'Comment':
            for line in s.text.strip().splitlines() or ['']:
                out.append(pad + '// ' + line.rstrip())
        elif k == 'Empty':
            pass
        else:
            raise Decline('statement ' + k)

    def loop_var_type(self, t):
        c = self.ctype(t)
        if t.kind in ('i64', 'bool', 'f64'):
            return c
        return 'const %s&' % c

    def exc_type(self, t):
        if t.name.startswith('py.'):
            return 'py::' + t.name[3:]
        return self.qual(t.name)

    def inner(self, b, out, ind):
        if isinstance(b, ir.Block):
            for x in b.stmts:
                self.stmt(x, out, ind)
        else:
            self.stmt(b, out, ind)

    def iterable(self, e):
        if isinstance(e, ir.Call) and e.sym.owner == 'py':
            n = e.sym.name
            if n == 'enumerate':
                args = [self.expr(a) for a in e.args]
                return 'py::enumerate(%s)' % ', '.join(args)
            if n in ('zip', 'reversed'):
                return 'py::%s(%s)' % (n, ', '.join(self.expr(a) for a in e.args))
            if n in ('dict.items', 'dict.keys', 'dict.values'):
                return '%s.%s()' % (self.operand(e.target), n.split('.')[1])
        return self.operand(e) if isinstance(e, ir.Lit) else self.expr(e)

    # ------------------------------------------------------------------------------------ expressions
    def operand(self, e):
        if isinstance(e, ir.Lit) and isinstance(e.value, str) and e.type is not None and e.type.name == 'py.str':
            return 'py::str(%s)' % cpp_string(e.value)
        s = self.expr(e)
        if isinstance(e, (ir.Binary, ir.Cond, ir.Assign, ir.Concat)) or (isinstance(e, ir.Unary) and not e.postfix):
            return '(%s)' % s
        return s

    def sub(self, e, parent_prec, right=False):
        s = self.expr(e)
        if isinstance(e, ir.Paren):
            return s
        if isinstance(e, ir.Binary):
            p = _PREC.get(e.op, 16)
            if p > parent_prec or (right and p == parent_prec):
                return '(%s)' % s
        elif isinstance(e, (ir.Cond, ir.Assign)):
            return '(%s)' % s
        return s

    def expr(self, e, want=None):
        m = getattr(self, 'x_' + type(e).__name__, None)
        if m is None:
            raise Decline('expression ' + type(e).__name__)
        if want is not None and isinstance(e, ir.Lit) and e.value is None:
            return self.none_of(want)
        return m(e)

    def x_Paren(self, e):
        return '(%s)' % self.expr(e.x)

    def x_Lit(self, e):
        v = e.value
        t = e.type
        if v is None:
            if t is None or t.kind == 'void':
                return 'py::none'
            return self.none_of(t)
        if isinstance(v, bool):
            return 'true' if v else 'false'
        if isinstance(v, int):
            if e.text and re.fullmatch(r'0[xX][0-9a-fA-F_]+', e.text):
                s = e.text.replace('_', '')
                txt = '0x' + s[2:].upper() if len(s) > 2 else s
            elif e.text and re.fullmatch(r'0[oO][0-7_]+', e.text):
                txt = '0' + e.text[2:].replace('_', '')
            elif e.text and re.fullmatch(r'0[bB][01_]+', e.text):
                txt = '0b' + e.text[2:].replace('_', '')
            else:
                txt = str(v)
            if v > 2 ** 31 - 1 or v < -2 ** 31:
                if v == -2 ** 63:
                    return 'INT64_MIN'
                return 'INT64_C(%s)' % txt
            return txt
        if isinstance(v, float):
            r = repr(v)
            if r in ('inf', '-inf', 'nan'):
                return {'inf': 'INFINITY', '-inf': '-INFINITY', 'nan': 'NAN'}[r]
            return r if ('.' in r or 'e' in r) else r + '.0'
        if isinstance(v, str):
            if t is not None and t.kind == 'class' and t.name in ('py.bytes', 'py.bytearray'):
                return cpp_bytes(v)
            return 'py::str(%s)' % cpp_string(v) if not self.bare_ok else cpp_string(v)
        raise Decline('literal %r' % (v,))

    bare_ok = True

    def str_arg(self, e):
        """a str argument that can stay a bare literal (the support header takes const char* there)"""
        if isinstance(e, ir.Lit) and isinstance(e.value, str) and e.type is not None and e.type.name == 'py.str':
            return cpp_string(e.value)
        return self.expr(e)

    def x_Name(self, e):
        return ident(e.sym.name)

    def x_TypeName(self, e):
        return self.qual(e.type.name)

    def x_This(self, e):
        return 'self()'

    def x_Select(self, e):
        s = e.sym
        if s.kind == 'global':
            return self.qual(s.owner + '.' + s.name)
        if isinstance(e.obj, ir.TypeName):
            return '%s::%s' % (self.qual(e.obj.type.name), ident(s.name))
        obj = e.obj
        if isinstance(obj, ir.This):
            if s.name in self.locals:
                return 'this->' + ident(s.name)
            return ident(s.name)
        ot = obj.type
        arrow = '->' if (ot is not None and ot.kind == 'class' and (ot.info.get('ref') or
                                                                    (ot.name == 'py.optional' and ot.args and ot.args[0].info.get('ref')))) else '.'
        return '%s%s%s' % (self.operand(obj), arrow, ident(s.name))

    def x_Index(self, e):
        return '%s[%s]' % (self.operand(e.arr), self.expr(e.index))

    def x_Unary(self, e):
        if e.postfix:
            return self.operand(e.x) + e.op
        x = self.expr(e.x)
        if not isinstance(e.x, ir.Paren) and not re.fullmatch(r'[\w:.<>]*(\(.*\)|\[.*\])?|-?\d+', x):
            x = '(%s)' % x
        if e.op == '-' and x.startswith('-'):
            x = '(%s)' % x
        return e.op + x

    def x_Binary(self, e):
        p = _PREC.get(e.op, 16)
        return '%s %s %s' % (self.sub(e.left, p), e.op, self.sub(e.right, p, right=True))

    def x_Concat(self, e):
        parts = []
        for i, x in enumerate(e.parts):
            s = self.expr(x)
            if i == 0 and isinstance(x, ir.Lit):
                s = 'py::str(%s)' % cpp_string(x.value)
            elif isinstance(x, ir.Lit) and isinstance(x.value, str):
                s = cpp_string(x.value)
            elif isinstance(x, (ir.Binary, ir.Cond)):
                s = '(%s)' % s
            parts.append(s)
        return ' + '.join(parts)

    def x_Assign(self, e):
        t = self.expr(e.target)
        v = self.expr(e.value, want=e.target.type)
        if isinstance(e.target, ir.Index) and e.target.arr.type is not None and e.target.arr.type.name == 'py.dict':
            return '%s.set(%s, %s)' % (self.operand(e.target.arr), self.expr(e.target.index), v)
        if e.op:
            return '%s %s= %s' % (t, e.op, v)
        return '%s = %s' % (t, v)

    def x_Cond(self, e):
        a = self.expr(e.then, want=e.type)
        b = self.expr(e.orelse, want=e.type)
        t = e.type
        if t is not None and t.kind == 'class' and t.name == 'py.optional' and not self.is_nullable_ref(t):
            ct = self.ctype(t)                           # both arms of one type: the optional
            if not (isinstance(e.then, ir.Lit) and e.then.value is None) and e.then.type != t:
                a = '%s(%s)' % (ct, a)
            if not (isinstance(e.orelse, ir.Lit) and e.orelse.value is None) and e.orelse.type != t:
                b = '%s(%s)' % (ct, b)
        elif t is not None and t.kind == 'class' and t.name == 'py.str':
            if isinstance(e.then, ir.Lit) and isinstance(e.then.value, str):
                a = 'py::str(%s)' % cpp_string(e.then.value)
        else:
            if isinstance(e.orelse, ir.Cond):
                b = '(%s)' % b
        if isinstance(e.orelse, ir.Cond):
            b = self.expr(e.orelse, want=e.type)
        return '%s ? %s : %s' % (self.sub(e.cond, 15), self.sub(e.then, 15) if not a.startswith('std::optional') else a, b)

    def x_Cast(self, e):
        return 'static_cast<%s>(%s)' % (self.ctype(e.type), self.expr(e.x))

    def x_New(self, e):
        s = e.sym
        args = ', '.join(self.arg(a) for a in e.args)
        if s.owner == 'py':
            return 'py::%s(%s)' % (s.name, args)
        if e.memory == 'stack' or (e.type is not None and e.type.info.get('exception')):
            return '%s(%s)' % (self.qual(s.owner), args)       # an exception: a value, thrown as one
        return 'std::make_shared<%s>(%s)' % (self.qual(s.owner), args)

    def arg(self, a):
        return self.expr(a)

    def x_Lambda(self, e):
        params = ', '.join('%s %s' % (self.param_type(p.type, False), ident(p.sym.name)) for p in e.params)
        return '[&](%s) { return %s; }' % (params, self.expr(e.body))

    def x_Call(self, e):
        s = e.sym
        if s.owner == 'py':
            return self.py_call(e)
        args = ', '.join(self.arg(a) for a in e.args)
        if s.kind == 'ctor' and 'base' in (e.typeargs or []):
            return '%s(%s)' % (self.qual(s.owner), args)          # (moved into the initializer list)
        if e.target is None:
            if s.kind == 'function':
                return '%s(%s)' % (self.qual(s.owner + '.' + s.name), args)
            return '%s(%s)' % (ident(s.name), args)
        if isinstance(e.target, ir.TypeName):
            return '%s::%s(%s)' % (self.qual(e.target.type.name), ident(s.name), args)
        t = e.target
        if isinstance(t, ir.This):
            return '%s(%s)' % (ident(s.name), args)
        return '%s->%s(%s)' % (self.operand(t), ident(s.name), args)

    # -- Python's operations
    def py_call(self, e):
        n = e.sym.name
        a = e.args
        X = self.expr
        if n == 'truthy':
            return self.truthy(a[0])
        if n in ('is_none', 'is_not_none'):
            x = a[0]
            neg = n == 'is_not_none'
            t = x.type
            if t is not None and t.kind == 'class' and t.name == 'py.optional' and not self.is_nullable_ref(t):
                return ('%s.has_value()' if neg else '!%s.has_value()') % self.operand(x)
            if t is not None and t.kind == 'void':
                return 'true' if not neg else 'false'
            if self.is_nullable_ref(t) if t is not None else False:
                return '%s %s nullptr' % (self.operand(x), '!=' if neg else '==')
            if isinstance(x, ir.Select) and x.type is not None and x.type.name == 'py.optional':
                return ('%s.has_value()' if neg else '!%s.has_value()') % self.operand(x)
            return 'false' if not neg else 'true'
        if n == 'unwrap':
            x = a[0]
            if self.is_nullable_ref(x.type):
                return X(x)
            return '*' + self.operand(x)
        if n == 'opt_value':
            return '%s.value()' % self.operand(a[0])
        if n == 'value_or':
            return '%s.value_or(%s)' % (self.operand(a[0]), X(a[1], want=e.type))
        if n == 'empty':
            return '%s{}' % self.ctype(e.type)
        if n == 'tuple':
            items = ', '.join(self.typed_item(x, t) for x, t in zip(a, e.type.args))
            if any(isinstance(x, ir.Lit) for x in a):           # (a literal's C++ type is not the tuple's)
                return '%s{%s}' % (self.ctype(e.type), items)
            return 'std::make_tuple(%s)' % items
        if n == 'in':
            return 'py::in(%s, %s)' % (self.expr(a[0]), self.expr(a[1]))
        if n in ('any', 'all'):
            return 'py::%s_of(%s)' % (n, self.expr(a[0]))
        if n == 'get':
            x = a[0]
            if isinstance(x, ir.Call) and x.sym.owner == 'py' and x.sym.name in ('struct.unpack_from', 'struct.unpack') \
                    and len(x.type.args) == 1:
                return self.struct_unpack(x, one=True)
            return 'std::get<%d>(%s)' % (e.typeargs[0], X(x))
        if n in ('list', 'set'):
            ct = self.ctype(e.type)
            return '%s{%s}' % (ct, ', '.join(self.typed_item(x, e.type.args[0]) for x in a))
        if n == 'dict':
            ct = self.ctype(e.type)
            return '%s{%s}' % (ct, ', '.join('{%s, %s}' % (self.typed_item(p.args[0], e.type.args[0]),
                                                          self.typed_item(p.args[1], e.type.args[1])) for p in a))
        if n == 'record':
            rt = e.type
            name = self.qual(rt.name)
            vals = ', '.join(self.typed_item(x, x.type) if not (isinstance(x, ir.Lit) and x.value is None) else '{}'
                             for x in a)
            if all(isinstance(x, ir.Lit) and x.value is None for x in a):
                vals = ''                                  # nothing given yet (filled by the statements after)
            if rt.info.get('ref'):
                return 'std::make_shared<%s>(%s{%s})' % (name, name, vals)
            return '%s{%s}' % (name, vals)
        if n == 'slice':
            obj = self.operand(e.target)
            lo = 'py::none' if isinstance(a[0], ir.Lit) and a[0].value is None else X(a[0])
            hi = 'py::none' if isinstance(a[1], ir.Lit) and a[1].value is None else X(a[1])
            if len(a) > 2:
                return '%s.slice(%s, %s, %s)' % (obj, lo, hi, X(a[2]))
            if hi == 'py::none':
                return '%s.slice_from(%s)' % (obj, lo)
            if lo == 'py::none':
                return '%s.slice_to(%s)' % (obj, hi)
            return '%s.slice(%s, %s)' % (obj, lo, hi)
        if n in ('floordiv', 'mod', 'shl', 'shr', 'ipow', 'truediv', 'checked_add', 'checked_sub', 'checked_mul',
                 'checked_shl', 'checked_pow', 'checked_neg', 'pow'):
            return 'py::%s(%s)' % (n, ', '.join(X(x) for x in a))
        if n == 'concat':
            if e.type is not None and e.type.kind == 'class' and e.type.name == 'py.tuple':
                return 'std::tuple_cat(%s, %s)' % (X(a[0]), X(a[1]))
            return '%s + %s' % (self.operand(a[0]), self.operand(a[1]))
        if n == 'repeat':
            first = cpp_bytes(a[0].value) if isinstance(a[0], ir.Lit) and a[0].type.name == 'py.bytes' else self.operand(a[0])
            return 'py::repeat(%s, %s)' % (first, X(a[1]))
        if n == 'percent':
            return 'py::percent(%s)' % ', '.join([self.str_arg(a[0])] + [X(x) for x in a[1:]])
        if n == 'format':
            return 'py::format(%s, %s)' % (X(a[0]), cpp_string(a[1].value))
        if n == 'str':
            return 'py::to_str(%s)' % X(a[0])
        if n == 'repr':
            return 'py::repr(%s)' % X(a[0])
        if n in ('or', 'and'):
            return 'py::%s_(%s, %s)' % (n, X(a[0], want=e.type), X(a[1], want=e.type))
        if n in ('or_none', 'and_none'):
            return 'py::or_none(%s, %s)' % (X(a[0]), X(a[1], want=e.type))
        if n == 'contains':
            return '%s.contains(%s)' % (self.operand(e.target), X(a[0]))
        if n == 'dict.at':
            return '%s.at(%s)' % (self.operand(e.target), X(a[0]))
        if n.startswith('struct.'):
            return self.struct_call(e)
        if n == 'sorted':
            key = 'nullptr' if isinstance(a[1], ir.Lit) else X(a[1])
            rev = X(a[2]) if len(a) > 2 else 'false'
            if key == 'nullptr' and rev == 'false':
                return 'py::sorted(%s)' % X(a[0])
            return 'py::sorted(%s, %s, %s)' % (X(a[0]), key, rev)
        if n == 'list.sort':
            key = 'nullptr' if isinstance(a[0], ir.Lit) else X(a[0])
            rev = X(a[1]) if len(a) > 1 else 'false'
            if key == 'nullptr' and rev == 'false':
                return '%s.sort()' % self.operand(e.target)
            return '%s.sort(%s, %s)' % (self.operand(e.target), key, rev)
        if n == 'print':
            return 'py::print(%s)' % ', '.join(X(x) for x in a if not (isinstance(x, ir.Call) and x.sym.name == 'kw'))
        if n == 'enumerate':
            return 'py::enumerate(%s)' % ', '.join(X(x) for x in a)
        if n in ('builtins.list', 'builtins.set', 'builtins.dict') and not a:
            return '%s{}' % self.ctype(e.type)
        tpl = api(n)
        if tpl is None:
            raise Decline('no C++ for %s' % n)
        return self.template(tpl, e)

    def typed_item(self, x, t):
        if isinstance(x, ir.Lit) and x.value is None:
            return self.none_of(t)
        if isinstance(x, ir.Lit) and isinstance(x.value, str) and t is not None and t.name == 'py.str':
            return 'py::str(%s)' % cpp_string(x.value)
        return self.expr(x)

    _BARE = frozenset(('bytes.decode', 'str.encode', 'builtins.format', 'int.from_bytes', 'int.to_bytes'))     # (they take const char*)

    def template(self, tpl, e):
        args = [(self.str_arg(x) if e.sym.name in self._BARE else self.typed_item(x, x.type))
                if not (isinstance(x, ir.Call) and x.sym.name == 'kw') else self.expr(x.args[0]) for x in e.args]
        recv = self.operand(e.target) if e.target is not None else ''
        out = tpl.replace('{recv}', recv).replace('{,args}', ''.join(', ' + x for x in args)).replace('{args}', ', '.join(args))
        if '{T}' in out:
            out = out.replace('{T}', self.ctype(e.type) if e.type.kind != 'void' else 'void')
        for i, x in enumerate(args):
            out = out.replace('{%d}' % i, x)
        if re.search(r'\{\d\}', out):
            out = re.sub(r', \{\d\}', '', out)
            out = re.sub(r'\{\d\}', '', out)
        return out

    def struct_types(self, t):
        return ', '.join(self.ctype(a) for a in t.args)

    def struct_unpack(self, x, one=False):
        fmt = x.typeargs[0]
        a = x.args
        buf = self.expr(a[1])
        off = (', ' + self.expr(a[2])) if len(a) > 2 else ''
        fn = 'unpack_from' if x.sym.name == 'struct.unpack_from' else 'unpack'
        if one:
            return 'py::%s1<%s>(%s, %s%s)' % (fn, self.struct_types(x.type), cpp_string(fmt), buf, off)
        return 'py::%s<%s>(%s, %s%s)' % (fn, self.struct_types(x.type), cpp_string(fmt), buf, off)

    def struct_call(self, e):
        n = e.sym.name
        if n in ('struct.unpack_from', 'struct.unpack'):
            return self.struct_unpack(e)
        fmt = e.typeargs[0]
        a = [self.expr(x) for x in e.args[1:]]
        if n == 'struct.pack':
            return 'py::pack(%s)' % ', '.join([cpp_string(fmt)] + a)
        if n == 'struct.pack_into':
            return 'py::pack_into(%s)' % ', '.join([cpp_string(fmt)] + a)
        if n == 'struct.calcsize':
            return str(e.type and 0)
        raise Decline(n)

    def truthy(self, x):
        t = x.type
        s = self.operand(x)
        if t is None:
            raise Decline('truth of an untyped value')
        if t.kind == 'bool':
            return self.expr(x)
        if t.kind == 'i64':
            return '%s != 0' % s
        if t.kind == 'f64':
            return '%s != 0.0' % s
        if t.kind == 'class':
            if t.name == 'py.optional':
                inner = t.args[0]
                if self.is_nullable_ref(t):
                    return '%s != nullptr' % s
                if inner.kind == 'i64':
                    return '%s.value_or(0) != 0' % s
                if inner.kind == 'bool':
                    return '%s.value_or(false)' % s
                return '(%s.has_value() && !%s->empty())' % (s, s)
            if t.info.get('ref'):
                return '%s != nullptr' % s
            if t.info.get('record'):
                return 'true'
            if t.name == 'py.tuple':
                return 'true' if t.args else 'false'
            return '!%s.empty()' % s
        if t.kind == 'void':
            return 'false'
        raise Decline('truth of %r' % (t,))


# ---------------------------------------------------------------------------------------------- modules
def header_path(module):
    return module.replace('.', '/') + '.hpp'


def source_path(module):
    return module.replace('.', '/') + '.cpp'


def write_unit(unit, banner=None):
    """An ir.Unit of the Python front end -> (header text, source text, {function: (first, last) source
    line}) -- the lines say where each function's body is in the source, so a compiler's error can be
    pinned on it."""
    mod = unit.package
    w = Writer(mod)
    ns = '::'.join(ident(x) for x in mod.split('.'))
    records = [t for t in unit.types if t.kind == 'record']
    classes = [t for t in unit.types if t.kind == 'class']
    modules = [t for t in unit.types if t.kind == 'module']
    classes = _bases_first(classes)
    records = _records_ordered(records, w)
    H = ['// %s -- C++ from %s by CodeBreaker\'s Python front end.' % (header_path(mod), mod),
         '#pragma once', '', '#include "py.hpp"']
    for dep in unit.imports:
        H.append('#include "%s"' % header_path(dep))
    H += ['', 'namespace %s {' % ns, '']
    for c in classes:
        H.append('class %s;' % ident(c.name))
    for r in records:
        H.append('struct %s;' % ident(r.name))
    if classes or records:
        H.append('')
    for r in records:
        H += _record_decl(w, r) + ['']
    S = ['// %s -- C++ from %s by CodeBreaker\'s Python front end.' % (source_path(mod), mod), '',
         '#include "%s"' % header_path(mod), '', 'namespace %s {' % ns, '']
    spans = {}
    for c in classes:
        H += _class_decl(w, c, classes) + ['']
    for m in modules:
        notes = [x for x in m.members if isinstance(x, ir.Comment)]
        for x in m.members:
            if isinstance(x, ir.FieldDecl):
                const = 'const ' if 'const' in x.mods else ''
                H.append('extern %s%s %s;' % (const, w.ctype(x.type), ident(x.sym.name)))
                S.append('%s%s %s = %s;' % (const, w.ctype(x.type), ident(x.sym.name), w.expr(x.init, want=x.type)))
        if any(isinstance(x, ir.FieldDecl) for x in m.members):
            H.append('')
            S.append('')
        for x in m.members:
            if isinstance(x, ir.MethodDecl):
                H.append(_signature(w, x, None, decl=True) + ';')
        if notes:
            H.append('')
            H.append('// Left in Python (the front end refused them):')
            for x in notes:
                H.append('//   ' + x.text.replace('not translated: ', ''))
    H += ['', '}  // namespace %s' % ns, '']
    for c in classes:
        for x in c.members:
            if isinstance(x, ir.MethodDecl):
                start = len(S) + 1
                S += _definition(w, x, c)
                spans[x.sym.owner + '.' + x.name] = (start, len(S))
                S.append('')
    for m in modules:
        for x in m.members:
            if isinstance(x, ir.MethodDecl):
                start = len(S) + 1
                S += _definition(w, x, None)
                spans[mod + '.' + x.name] = (start, len(S))
                S.append('')
    S += ['}  // namespace %s' % ns, '']
    return '\n'.join(H), '\n'.join(S), spans


def _bases_first(classes):
    by = dict((c.qname, c) for c in classes)
    out, seen = [], set()

    def add(c):
        if c.qname in seen:
            return
        seen.add(c.qname)
        if c.extends is not None and c.extends.name in by:
            add(by[c.extends.name])
        out.append(c)
    for c in classes:
        add(c)
    return out


def _records_ordered(records, w):
    """a value record holding another value record needs that one complete first"""
    by = dict((r.qname, r) for r in records)
    out, seen = [], set()

    def deps(r):
        d = []
        for f in r.members:
            if isinstance(f, ir.FieldDecl):
                _value_records(f.type, d)
        return [x for x in d if x in by]

    def add(r, stack=()):
        if r.qname in seen or r.qname in stack:
            return
        for d in deps(r):
            add(by[d], stack + (r.qname,))
        seen.add(r.qname)
        out.append(r)
    for r in records:
        add(r)
    return out


def _value_records(t, out):
    if t.kind == 'class':
        if t.info.get('record') and not t.info.get('ref'):
            out.append(t.name)
        if t.name in ('py.optional', 'py.tuple'):
            for a in t.args:
                _value_records(a, out)


def _record_decl(w, r):
    out = ['struct %s {' % ident(r.name)]
    for f in r.members:
        if isinstance(f, ir.Comment):
            out.append('    // ' + f.text)
            continue
        t = f.type
        if 'optional' in f.mods and not (t.kind == 'class' and t.name == 'py.optional'):
            ct = w.ctype(ir.Type('class', 'py.optional', [t]))
        else:
            ct = w.ctype(t)
        z = w.zero(t) if 'optional' not in f.mods else None
        out.append('    %s %s%s;' % (ct, ident(f.sym.name), (' = ' + z) if z else ''))
    out.append('};')
    # its keys and values as the Python dict had them (an absent optional key is no key): for repr, JSON, tests
    out.append('template <class F> void for_each_field(const %s& r, F&& f) {' % ident(r.name))
    for fd in r.members:
        if isinstance(fd, ir.FieldDecl):
            n = ident(fd.sym.name)
            if 'optional' in fd.mods:
                out.append('    if (r.%s) f("%s", *r.%s);' % (n, fd.sym.name, n))
            else:
                out.append('    f("%s", r.%s);' % (fd.sym.name, n))
    out.append('}')
    return out


def _class_decl(w, c, classes):
    by = dict((x.qname, x) for x in classes)
    base = c.extends.name if c.extends is not None and c.extends.name in by else None
    exc = next((m.split(':', 1)[1] for m in c.mods if m.startswith('exception:')), None)
    if base:
        head = 'class %s : public %s {' % (ident(c.name), w.qual(base))
    elif exc:
        head = 'class %s : public py::%s {' % (ident(c.name), exc)
    else:
        head = 'class %s : public py::object {' % ident(c.name)
    out = [head, 'public:']
    for f in c.members:
        if isinstance(f, ir.FieldDecl):
            static = 'static ' if 'static' in f.mods else ''
            const = 'constexpr ' if 'const' in f.mods and f.type.kind in ('i64', 'bool', 'f64') else ''
            if static and not const:
                static = 'inline static const '
            init = (' = ' + w.expr(f.init, want=f.type)) if f.init is not None else ''
            if not init and not static:
                z = w.zero(f.type)
                init = (' = ' + z) if z else ''
            out.append('    %s%s%s %s%s;' % (static, const, w.ctype(f.type), ident(f.sym.name), init))
    methods = [m for m in c.members if isinstance(m, ir.MethodDecl)]
    has_ctor = any(m.kind == 'ctor' for m in methods)
    if not has_ctor and base and _has_ctor(by, base):
        out.append('    using %s::%s;' % (w.qual(base), ident(by[base].name)))
    if exc and not has_ctor:
        out.append('    using py::%s::%s;' % (exc, exc))
    if methods or any(isinstance(f, ir.FieldDecl) for f in c.members):
        out.append('')
    for m in methods:
        out.append('    ' + _signature(w, m, c, decl=True, classes=by) + ';')
    if any('self()' in line for m in methods for line in _definition(w, m, c)):
        out.append('    py::ref<%s> self() { return std::static_pointer_cast<%s>(shared_from_this()); }'
                   % (ident(c.name), ident(c.name)))
    notes = [x for x in c.members if isinstance(x, ir.Comment)]
    if notes:
        out.append('')
        out.append('    // Left in Python (the front end refused them):')
        for x in notes:
            out.append('    //   ' + x.text.replace('not translated: ', ''))
    out.append('};')
    return out


def _has_ctor(by, q):
    c = by.get(q)
    while c is not None:
        if any(isinstance(m, ir.MethodDecl) and m.kind == 'ctor' for m in c.members):
            return True
        c = by.get(c.extends.name) if c.extends is not None else None
    return False


def _assigned(m):
    out = set()
    for x in ir.walk(m):
        if isinstance(x, ir.Assign) and isinstance(x.target, ir.Name):
            out.add(x.target.sym.name)
    return out


def _default(w, p):
    """a parameter's default (a constant), as C++, or None"""
    for mod in p.mods or []:
        if mod.startswith('default='):
            v = json.loads(mod[len('default='):])
            if v is None:
                return w.none_of(p.type) if p.type.kind == 'class' and p.type.name == 'py.optional' else None
            if isinstance(v, bool):
                return 'true' if v else 'false'
            if isinstance(v, (int, float)):
                return w.expr(ir.Lit(value=v, type=p.type))
            if isinstance(v, dict) and 'bytes' in v:
                return cpp_bytes(v['bytes'])
            if isinstance(v, str):
                return 'py::str(%s)' % cpp_string(v)
    return None


def _signature(w, m, c, decl=False, classes=None):
    assigned = _assigned(m)
    parts = []
    for p in m.params:
        d = _default(w, p) if decl else None
        parts.append('%s %s%s' % (w.param_type(p.type, p.sym.name in assigned), ident(p.sym.name),
                                  (' = ' + d) if d else ''))
    params = ', '.join(parts)
    if m.kind == 'ctor':
        name = ident(c.name)
        if decl:
            return ('explicit ' if len(m.params) == 1 else '') + '%s(%s)' % (name, params)
        return '%s::%s(%s)' % (name, name, params)
    ret = w.ctype(m.ret)
    if c is None:
        return '%s %s(%s)' % (ret, ident(m.name), params)
    if not decl:
        return '%s %s::%s(%s)' % (ret, ident(c.name), ident(m.name), params)
    static = 'static ' if 'static' in (m.mods or []) else ''
    virt = ''
    override = ''
    if classes is not None and not static:
        if _base_has(classes, c, m.name):
            override = ' override'
        elif _sub_has(classes, c, m.name):
            virt = 'virtual '
    return '%s%s%s %s(%s)%s' % (static, virt, ret, ident(m.name), params, override)


def _base_has(by, c, name):
    b = by.get(c.extends.name) if c.extends is not None else None
    while b is not None:
        if any(isinstance(x, ir.MethodDecl) and x.name == name for x in b.members):
            return True
        b = by.get(b.extends.name) if b.extends is not None else None
    return False


def _sub_has(by, c, name):
    for d in by.values():
        k = d
        while k is not None and k.extends is not None:
            if k.extends.name == c.qname:
                if any(isinstance(x, ir.MethodDecl) and x.name == name for x in d.members):
                    return True
                break
            k = by.get(k.extends.name)
    return False


def _definition(w, m, c):
    w.locals = set(p.sym.name for p in m.params)
    w.ret = m.ret
    w.this_class = c
    stmts = list(m.body.stmts)
    init = ''
    if m.kind == 'ctor' and stmts and isinstance(stmts[0], ir.ExprStmt) and isinstance(stmts[0].expr, ir.Call) \
            and stmts[0].expr.sym.kind == 'ctor' and 'base' in (stmts[0].expr.typeargs or []):
        init = ' : ' + w.expr(stmts[0].expr)
        stmts = stmts[1:]
    out = []
    for text in m.comments or []:
        for line in text.strip().splitlines():
            out.append('// ' + line.rstrip())
    out.append(_signature(w, m, c) + init + ' {')
    body = []
    for s in stmts:
        w.stmt(s, body, 1)
    out += body
    out.append('}')
    return out
