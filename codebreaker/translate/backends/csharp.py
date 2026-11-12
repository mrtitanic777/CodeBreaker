"""csharp.py -- the translator's C# back end: whole programs from the IR, in C# as a person writes it.

It reads any front end's units -- each says its source language (Unit.lang) -- and writes one .cs file
per unit. Where the languages line up the output is idiomatic: properties, `using`, `lock`, generics,
lambdas and delegates, `foreach`, `is` patterns, switch expressions, extension-method calls written on
their receiver, operators written as operators. Where they do not, it writes the faithful form:

  * from Java: a method that Java would dispatch virtually is `virtual` (and `override` where it
    overrides); labelled break and continue are gotos to labels after and inside the loop; a switch
    section that falls through ends in `goto case`; instance field initialisers run after the base
    constructor, as Java runs them; a constant expression that Java lets wrap is `unchecked`; every
    string conversion Java makes differently from .NET (doubles, floats, booleans, null) goes through
    the small Java runtime in C# (codebreaker/translate/dotnet/runtime/JavaRt.cs), as do the library
    calls whose meaning differs (java.util.Random, Math.round, Math.abs, List.remove(int), Map.get on
    a missing key, String.hashCode, ...). The mapping of Java's library onto .NET's is
    codebreaker/java/api_maps/java.csharp.json, read through the API-mapping registry;
  * from C#: the source's own forms.

What it cannot write faithfully it declines, never guesses: a method body holding something it does
not carry becomes `throw new System.NotSupportedException("CB-GAP: ...")`, listed in the ledger with
the reason; a declaration it cannot carry (a Java inner class that uses its outer instance, an
anonymous class) is a ledger entry and the unit is declined.

    from codebreaker.translate.backends import csharp
    prog = csharp.Program(units)
    files = prog.files()            # {file name: text}, the runtime's files included when used
    prog.ledger                     # [(where, kind, why)]
"""
import os

from codebreaker.translate import ir

RUNTIME = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), 'dotnet', 'runtime')

KEYWORDS = frozenset('''abstract as base bool break byte case catch char checked class const continue decimal default
delegate do double else enum event explicit extern false finally fixed float for foreach goto if implicit in int
interface internal is lock long namespace new null object operator out override params private protected public
readonly ref return sbyte sealed short sizeof stackalloc static string struct switch this throw true try typeof uint
ulong unchecked unsafe ushort using virtual void volatile while'''.split())

_PRIM = {'bool': 'bool', 'i8': 'sbyte', 'u8': 'byte', 'i16': 'short', 'i32': 'int', 'u32': 'uint', 'i64': 'long',
         'u64': 'ulong', 'f32': 'float', 'f64': 'double', 'void': 'void', 'char': 'byte'}
_ALIAS = {'System.String': 'string', 'System.Object': 'object', 'System.Boolean': 'bool', 'System.Int32': 'int',
          'System.Int64': 'long', 'System.Double': 'double', 'System.Single': 'float', 'System.Char': 'char',
          'System.Byte': 'byte', 'System.SByte': 'sbyte', 'System.Int16': 'short', 'System.UInt16': 'ushort',
          'System.UInt32': 'uint', 'System.UInt64': 'ulong', 'System.Decimal': 'decimal', 'System.Void': 'void'}

# precedence (C# 12 spec, highest first)
P_PRIMARY, P_UNARY, P_MUL, P_ADD, P_SHIFT, P_REL, P_EQ, P_AND, P_XOR, P_OR, P_LAND, P_LOR, P_COALESCE, P_COND, \
    P_ASSIGN = 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2
_BIN_PREC = {'*': P_MUL, '/': P_MUL, '%': P_MUL, '+': P_ADD, '-': P_ADD, '<<': P_SHIFT, '>>': P_SHIFT,
             '>>>': P_SHIFT, '<': P_REL, '>': P_REL, '<=': P_REL, '>=': P_REL, '==': P_EQ, '!=': P_EQ, '&': P_AND,
             '^': P_XOR, '|': P_OR, '&&': P_LAND, '||': P_LOR, '??': P_COALESCE}
_OPERATOR_METHODS = {'op_Addition': '+', 'op_Subtraction': '-', 'op_Multiply': '*', 'op_Division': '/',
                     'op_Modulus': '%', 'op_Equality': '==', 'op_Inequality': '!=', 'op_LessThan': '<',
                     'op_GreaterThan': '>', 'op_LessThanOrEqual': '<=', 'op_GreaterThanOrEqual': '>=',
                     'op_BitwiseAnd': '&', 'op_BitwiseOr': '|', 'op_ExclusiveOr': '^', 'op_LeftShift': '<<',
                     'op_RightShift': '>>', 'op_UnsignedRightShift': '>>>'}
_UNARY_METHODS = {'op_UnaryNegation': '-', 'op_UnaryPlus': '+', 'op_LogicalNot': '!', 'op_OnesComplement': '~'}
_DECL_OPERATORS = dict(list(_OPERATOR_METHODS.items()) + list(_UNARY_METHODS.items()) +
                       [('op_Increment', '++'), ('op_Decrement', '--'), ('op_True', 'true'), ('op_False', 'false')])


class Decline(Exception):
    """This construct is not written; the method (or unit) becomes a ledgered gap."""

    def __init__(self, kind, why=None):
        Exception.__init__(self, why or kind)
        self.kind = kind
        self.why = why or kind


def ident(name):
    return '@' + name if name in KEYWORDS else name


def _cstr(s):
    out = ['"']
    for ch in s:
        o = ord(ch)
        if ch == '"':
            out.append('\\"')
        elif ch == '\\':
            out.append('\\\\')
        elif ch == '\n':
            out.append('\\n')
        elif ch == '\r':
            out.append('\\r')
        elif ch == '\t':
            out.append('\\t')
        elif ch == '\0':
            out.append('\\0')
        elif o < 0x20 or o > 0x7e:
            if o > 0xffff:
                o -= 0x10000
                out.append('\\u%04x\\u%04x' % (0xd800 + (o >> 10), 0xdc00 + (o & 0x3ff)))
            else:
                out.append('\\u%04x' % o)
        else:
            out.append(ch)
    return ''.join(out) + '"'


def _cchar(ch):
    if ch == "'":
        return "'\\''"
    if ch == '"':
        return "'\"'"
    s = _cstr(ch)[1:-1]
    return "'" + s + "'"


def _float(v, t):
    import math
    single = t is not None and t.kind == 'f32'
    if isinstance(v, str):
        v = float(v)
    if math.isnan(v):
        return 'float.NaN' if single else 'double.NaN'
    if math.isinf(v):
        return ('float' if single else 'double') + ('.PositiveInfinity' if v > 0 else '.NegativeInfinity')
    if single:
        import struct
        r = repr(struct.unpack('f', struct.pack('f', v))[0])
        # the shortest text that is still this float
        for digits in range(1, 18):
            cand = '%.*g' % (digits, v)
            if struct.unpack('f', struct.pack('f', float(cand)))[0] == struct.unpack('f', struct.pack('f', v))[0]:
                r = cand
                break
        if 'e' not in r and '.' not in r and 'inf' not in r:
            r += '.0'
        return r.replace('e', 'E') + 'f'
    r = repr(v)
    if 'e' not in r and '.' not in r:
        r += '.0'
    return r.replace('e', 'E') + ('' if '.' in r or 'e' in r else 'D')


def is_string(t):
    return t is not None and t.kind == 'class' and t.name in ('System.String', 'java.lang.String')


def _strip(e):
    while isinstance(e, ir.Paren):
        e = e.x
    return e


# library constants a constant expression may use (Java's Integer.MAX_VALUE is a field, not a literal)
_LIB_CONSTS = {('java.lang.Integer', 'MAX_VALUE'): 2 ** 31 - 1, ('java.lang.Integer', 'MIN_VALUE'): -2 ** 31,
               ('java.lang.Long', 'MAX_VALUE'): 2 ** 63 - 1, ('java.lang.Long', 'MIN_VALUE'): -2 ** 63,
               ('java.lang.Short', 'MAX_VALUE'): 32767, ('java.lang.Short', 'MIN_VALUE'): -32768,
               ('java.lang.Byte', 'MAX_VALUE'): 127, ('java.lang.Byte', 'MIN_VALUE'): -128,
               ('java.lang.Character', 'MAX_VALUE'): 65535, ('java.lang.Character', 'MIN_VALUE'): 0}


def _lib_const(e):
    return isinstance(e, ir.Select) and e.sym is not None and (e.sym.owner, e.sym.name) in _LIB_CONSTS


def _is_const(e):
    """A compile-time constant expression (where Java folds with wrapping and C# refuses to overflow)."""
    e = _strip(e)
    if _lib_const(e):
        return True
    if isinstance(e, ir.Lit):
        return e.value is not None and not isinstance(e.value, str) or (isinstance(e.value, str) and len(e.value) == 1)
    if isinstance(e, ir.Unary) and e.op in ('-', '+', '~'):
        return _is_const(e.x)
    if isinstance(e, ir.Binary) and e.op in _BIN_PREC and e.op not in ('&&', '||', '??'):
        return _is_const(e.left) and _is_const(e.right)
    if isinstance(e, ir.Cast):
        return e.type is not None and e.type.is_prim and _is_const(e.x)
    return False


_RANGE = {'i8': (-128, 127), 'u8': (0, 255), 'i16': (-32768, 32767), 'u16': (0, 65535),
          'i32': (-2 ** 31, 2 ** 31 - 1), 'u32': (0, 2 ** 32 - 1), 'i64': (-2 ** 63, 2 ** 63 - 1), 'u64': (0, 2 ** 64 - 1)}


def _overflows(e):
    """Does folding this constant integer expression leave its type's range anywhere along the way?"""
    bad = [False]

    def val(x):
        x = _strip(x)
        t = x.type.kind if x.type is not None else 'i32'
        if isinstance(x, ir.Lit):
            v = x.value
            return ord(v) if isinstance(v, str) else (int(v) if isinstance(v, (int, bool)) else None)
        if _lib_const(x):
            return _LIB_CONSTS[(x.sym.owner, x.sym.name)]
        if isinstance(x, ir.Unary):
            a = val(x.x)
            r = None if a is None else {'-': -a if a is not None else None, '+': a, '~': ~a}[x.op]
        elif isinstance(x, ir.Binary):
            a, b = val(x.left), val(x.right)
            if a is None or b is None:
                return None
            op = x.op
            bits = 64 if t in ('i64', 'u64') else 32
            if op in ('<<', '>>', '>>>'):
                b &= bits - 1
            r = {'+': lambda: a + b, '-': lambda: a - b, '*': lambda: a * b,
                 '/': lambda: (abs(a) // abs(b)) * (1 if (a >= 0) == (b >= 0) else -1) if b else None, '%': lambda: (abs(a) % abs(b)) * (1 if a >= 0 else -1) if b else None,
                 '<<': lambda: a << b, '>>': lambda: a >> b, '>>>': lambda: (a % (1 << bits)) >> b,
                 '&': lambda: a & b, '|': lambda: a | b, '^': lambda: a ^ b}.get(op, lambda: None)()
            if op in ('<', '>', '<=', '>=', '==', '!='):
                return None
        elif isinstance(x, ir.Cast):
            r = val(x.x)
        else:
            return None
        if r is None:
            return None
        lo, hi = _RANGE.get(t, (None, None))
        if lo is not None and not lo <= r <= hi:
            bad[0] = True
            span = hi - lo + 1
            r = (r - lo) % span + lo
        return r
    val(e)
    return bad[0]


def _abrupt(stmts):
    """Does this statement list never fall off its end?"""
    if not stmts:
        return False
    s = stmts[-1]
    if isinstance(s, (ir.Return, ir.Throw, ir.Break, ir.Continue, ir.Goto)):
        return True
    if isinstance(s, ir.Block):
        return _abrupt(s.stmts)
    if isinstance(s, ir.If) and s.orelse is not None:
        return _abrupt([s.then]) and _abrupt([s.orelse])
    if isinstance(s, ir.While) and isinstance(_strip(s.cond), ir.Lit) and _strip(s.cond).value is True:
        return not any(isinstance(x, ir.Break) and x.label is None for x in _loop_breaks(s.body))
    return False


def _loop_breaks(body):
    """The unlabelled breaks that leave this loop (not those of nested loops or switches)."""
    out = []

    def walk(n, depth):
        if isinstance(n, ir.Break) and depth == 0:
            out.append(n)
        for c in n.children():
            nested = isinstance(c, (ir.While, ir.DoWhile, ir.For, ir.ForEach, ir.Switch))
            if isinstance(c, (ir.Lambda, ir.ClassDecl)):
                continue
            walk(c, depth + (1 if nested else 0))
    walk(body, 0)
    return out


# ---------------------------------------------------------------------------------------------- the library
class CSharpLib(object):
    """A C# source's own library: names as they are."""
    lang = 'csharp'

    def __init__(self, w):
        self.w = w

    def type_name(self, t):
        return None

    def call(self, e):
        return None

    def select(self, e):
        return None

    def new(self, e):
        return None

    def concat_part(self, e):
        return None

    def string_of(self, code, t):
        return code


class Program(object):
    """The units to write, what they declare, and what came of writing them."""

    def __init__(self, units, namespace_of_default=None):
        self.units = list(units)
        self.classes = {}
        for u in self.units:
            for c in u.types:
                self._index(c)
        self.ledger = []
        self.runtime = set()
        self.inits = {}                        # id(FieldDecl) -> the initialiser written (None: moved elsewhere)
        self._files = None

    def _index(self, c):
        if not isinstance(c, ir.ClassDecl):
            return
        self.classes[c.qname] = c
        for m in c.members:
            if isinstance(m, ir.ClassDecl):
                self._index(m)

    def files(self):
        if self._files is None:
            if any((u.lang or 'java') == 'java' for u in self.units):
                from codebreaker.translate.backends import csharp_java
                csharp_java.prepare(self)
            out = {}
            for u in self.units:
                w = Writer(self, u)
                stem = os.path.splitext(os.path.basename(u.path or 'Unit'))[0] or 'Unit'
                name, k = stem, 2
                while name + '.cs' in out:      # two units of one stem were one file, the first lost
                    name, k = '%s_%d' % (stem, k), k + 1
                out[name + '.cs'] = w.unit()
            for r in sorted(self.runtime):
                with open(os.path.join(RUNTIME, r + '.cs'), encoding='utf-8') as f:
                    out[r + '.cs'] = f.read()
            self._files = out
        return self._files

    def write(self, out_dir):
        os.makedirs(out_dir, exist_ok=True)
        paths = []
        for n, text in self.files().items():
            p = os.path.join(out_dir, n)
            with open(p, 'w', encoding='utf-8', newline='\n') as f:
                f.write(text)
            paths.append(p)
        return paths

    # -- the class hierarchy, for virtual/override from a source whose methods are virtual by default
    def base_of(self, c):
        if c.extends is None or c.extends.kind != 'class':
            return None
        return self.classes.get(c.extends.name)

    def overrides(self, c, m):
        """Does method m of class c override one of a class it extends (in these units)?"""
        b = self.base_of(c)
        sig = _sig(m)
        seen = set()
        while b is not None and b.qname not in seen:
            seen.add(b.qname)
            for x in b.members:
                if isinstance(x, ir.MethodDecl) and x.kind == 'method' and x.name == m.name and _sig(x) == sig \
                        and 'static' not in x.mods and 'private' not in x.mods:
                    return True
            b = self.base_of(b)
        return False


def _sig(m):
    return tuple(repr(p.type) for p in m.params or [])


# ---------------------------------------------------------------------------------------------- the writer
class Writer(object):
    def __init__(self, prog, unit):
        self.prog = prog
        self.u = unit
        self.lang = unit.lang or 'java'
        if self.lang == 'java':
            from codebreaker.translate.backends import csharp_java
            self.lib = csharp_java.JavaLib(self)
        else:
            self.lib = CSharpLib(self)
        self.usings = set()
        self.cls = None
        self.method = None
        self.labels = {}                       # a Java label -> its loop, for break/continue as goto
        self.loop_labels = {}                  # id(loop) -> its label
        self.locals = set()
        self.block_names = []                  # the locals each enclosing block declares (all of the block)
        self.scopes = [{}]                     # a local's name -> the name it is written with

    # ---- locals: Java's scope of a local begins at its declaration, C#'s is its whole block, so a name an
    # inner block declares before an outer one declares it is given a number
    def declare(self, name, nested=False):
        self.locals.add(name)
        enclosing = self.block_names if nested else self.block_names[:-1]
        new = name
        if any(name in b for b in enclosing):
            used = set().union(*self.block_names) if self.block_names else set()
            k = 2
            while name + str(k) in used or any(name + str(k) in sc.values() for sc in self.scopes):
                k += 1
            new = name + str(k)
        self.scopes[-1][name] = new
        return ident(new)

    def lookup(self, name):
        for sc in reversed(self.scopes):
            if name in sc:
                return ident(sc[name])
        return ident(name)

    def push(self):
        self.scopes.append({})

    def pop(self):
        self.scopes.pop()

    def note(self, kind, why):
        where = (self.cls.qname if self.cls else self.u.path or '') + ('.' + self.method.name if self.method and self.method.name else '')
        self.prog.ledger.append((where, kind, why))

    # ---- types
    def ctype(self, t, ctx=None):
        if t is None:
            return 'void'
        k = t.kind
        if k == 'u16':
            return 'char' if (t.info.get('enc') or self.lang == 'java') else 'ushort'
        if k in _PRIM:
            return _PRIM[k]
        if k == 'array':
            r = t.info.get('rank', 1)
            # int[][] nests outward in C#: the element's own brackets come after
            inner, suffix = t.elem, '[' + ',' * (r - 1) + ']'
            while inner.kind == 'array':
                suffix += '[' + ',' * (inner.info.get('rank', 1) - 1) + ']'
                inner = inner.elem
            return self.ctype(inner) + suffix
        if k == 'typevar':
            return ident(t.name)
        if k == 'wildcard':
            if t.bound and t.bound[0] == 'extends':
                return self.ctype(t.bound[1])
            return 'object'
        if k == 'null':
            return 'object'
        if k == 'dynamic':
            return 'dynamic'
        if k == 'class':
            got = self.lib.type_name(t)
            if got is not None:
                return got
            return self.class_name(t)
        raise Decline('type', 'the type %r' % (t,))

    def class_name(self, t):
        name = t.name
        if name in _ALIAS and not t.args:
            return _ALIAS[name] + ('?' if t.info.get('nullable') else '')
        if name == 'System.Nullable' and len(t.args) == 1:
            return self.ctype(t.args[0]) + '?'
        simple = self.short(name)
        if t.args:
            simple += '<' + ', '.join(self.ctype(a) for a in t.args) + '>'
        if t.info.get('nullable'):
            simple += '?'
        return simple

    def short(self, qname):
        """A type's name as written in this unit: relative to its namespace, with a using for it."""
        c = self.prog.classes.get(qname)
        if c is not None:
            chain = [c]
            while chain[-1].outer and self.prog.classes.get(chain[-1].outer if isinstance(chain[-1].outer, str) else None):
                chain.append(self.prog.classes[chain[-1].outer])
            top = chain[-1]
            ns = top.qname[:-len(top.name)].rstrip('.') if top.qname.endswith(top.name) else ''
            rel = '.'.join(ident(x.name) for x in reversed(chain))
            if ns and ns != self.namespace_of(self.cls):
                self.usings.add(ns)
            # inside the class itself or its outer classes, the nested name alone
            cur = self.cls
            while cur is not None:
                if c.outer and (cur.qname == c.outer):
                    return ident(c.name)
                cur = self.prog.classes.get(cur.outer) if isinstance(cur.outer, str) else None
            return rel
        if '.' in qname:
            ns, simple = qname.rsplit('.', 1)
            if ns in self.prog.classes:                      # a nested type of another
                return self.short(ns) + '.' + ident(simple)
            self.usings.add(ns)
            return ident(simple)
        return ident(qname)

    def namespace_of(self, c):
        if c is None:
            return self.u.package or ''
        while c.outer and isinstance(c.outer, str) and c.outer in self.prog.classes:
            c = self.prog.classes[c.outer]
        return c.qname[:-len(c.name)].rstrip('.') if c.qname and c.qname.endswith(c.name) else ''

    # ---- declarations
    def unit(self):
        body = []
        groups = []
        for c in self.u.types:
            ns = self.namespace_of(c) if isinstance(c, ir.ClassDecl) else ''
            if groups and groups[-1][0] == ns:
                groups[-1][1].append(c)
            else:
                groups.append((ns, [c]))
        for ns, cs in groups:
            lines = []
            for c in cs:
                try:
                    lines += self.class_decl(c, '')
                except Decline as d:
                    self.note(d.kind, d.why)
                    lines += ['// CB-GAP: %s was not translated: %s' % (getattr(c, 'name', '?'), d.why)]
                lines.append('')
            while lines and not lines[-1]:
                lines.pop()
            if ns:
                body += ['namespace %s' % ns, '{'] + ['    ' + l if l else '' for l in lines] + ['}', '']
            else:
                body += lines + ['']
        own = set(self.namespace_of(c) for c in self.u.types if isinstance(c, ir.ClassDecl))
        usings = sorted(u for u in self.usings if u and u not in own and not any(
            self.namespace_of(c).startswith(u + '.') for c in self.u.types if isinstance(c, ir.ClassDecl)))
        head = ['// Translated by CodeBreaker from %s (%s).' % (os.path.basename(self.u.path or '?'), self.lang)]
        if self.lang == 'java':
            head.append('#nullable disable')
        head += ['using %s;' % u for u in usings]
        return '\n'.join(head + [''] + body).rstrip() + '\n'

    def class_decl(self, c, ind):
        prev, self.cls = self.cls, c
        try:
            return self._class_decl(c, ind)
        finally:
            self.cls = prev

    def class_mods(self, c):
        if self.lang == 'java':
            return self.lib.class_mods(c)
        return [m for m in c.mods if m != 'IMPLICIT']

    def _class_decl(self, c, ind):
        if c.kind == 'delegate':
            inv = next(m for m in c.members if isinstance(m, ir.MethodDecl))
            return [ind + '%sdelegate %s %s%s(%s);' % (''.join(m + ' ' for m in self.class_mods(c)), self.ctype(inv.ret),
                                                        ident(c.name), self.tparams_text(c), self.params_text(inv.params))]
        kind = {'class': 'class', 'interface': 'interface', 'enum': 'enum', 'struct': 'struct', 'record': 'record',
                'record struct': 'record struct', 'annotation': None}.get(c.kind)
        if kind is None:
            raise Decline('class-kind', 'a %s' % c.kind)
        if self.lang == 'java':
            self.lib.check_class(c)
        mods = self.class_mods(c)
        head = ind + ''.join(m + ' ' for m in mods) + kind + ' ' + ident(c.name) + self.tparams_text(c)
        primary = next((m for m in c.members if isinstance(m, ir.MethodDecl) and 'primary' in (m.mods or [])), None)
        positional = set()
        if primary is not None:
            head += '(' + self.params_text(primary.params) + ')'
            positional = set(p.sym.name for p in primary.params if p.sym)
        bases = []
        if c.kind == 'enum':
            if c.extends is not None:
                bases.append(self.ctype(c.extends))
        else:
            if c.extends is not None:
                b = self.ctype(c.extends)
                if primary is not None:
                    call = next((s for s in primary.body.stmts if isinstance(s, ir.CtorCall) and not s.kind.endswith(':implicit')), None)
                    if call is not None:
                        b += '(' + self.args_text(call.args, call.sym) + ')'
                bases.append(b)
            bases += [self.ctype(i) for i in (c.implements or []) if self.lib_interface(i)]
        if bases:
            head += ' : ' + ', '.join(bases)
        lines = [head]
        for tp in c.tparams or []:
            w = self.where_text(tp)
            if w:
                lines.append(ind + '    ' + w)
        members = [m for m in c.members if not (isinstance(m, ir.Property) and 'positional' in (m.mods or [])
                                                and m.sym and m.sym.name in positional)]
        members = [m for m in members if m is not primary]
        if primary is not None and not members:
            lines[-1] += ';'
            return lines
        lines.append(ind + '{')
        inner = ind + '    '
        if c.kind == 'enum':
            consts = [m for m in members if isinstance(m, ir.EnumConst)]
            for i, e in enumerate(consts):
                text = inner + ident(e.sym.name)
                if e.value is not None:
                    text += ' = %d' % e.value
                elif e.args or e.body is not None:
                    raise Decline('enum', 'an enum constant with constructor arguments or a body')
                lines.append(text + (',' if i < len(consts) - 1 else ''))
            members = [m for m in members if not isinstance(m, ir.EnumConst)]
            if members and self.lang != 'java':
                raise Decline('enum', 'an enum with members')
        if self.lang == 'java':
            members = self.lib.arrange_members(c, members)
        first = True
        for m in members:
            text = self.member(m, c, inner)
            if not text:
                continue
            if not first and (len(text) > 1 or isinstance(m, (ir.MethodDecl, ir.ClassDecl, ir.Property))):
                lines.append('')
            lines += text
            first = False
        lines.append(ind + '}')
        return lines

    def lib_interface(self, t):
        return True if self.lang != 'java' else self.lib.keep_interface(t)

    def tparams_text(self, d):
        names = d.typeparams or []
        if not names:
            return ''
        tps = dict((tp.name, tp) for tp in (d.tparams or []))
        out = []
        for n in names:
            tp = tps.get(n)
            v = {'Out': 'out ', 'In': 'in '}.get(tp.variance, '') if tp is not None and tp.variance else ''
            out.append(v + ident(n))
        return '<' + ', '.join(out) + '>'

    def where_text(self, tp):
        parts = []
        if 'class' in (tp.flags or []):
            parts.append('class')
        if 'struct' in (tp.flags or []):
            parts.append('struct')
        parts += [self.ctype(b) for b in tp.bounds or [] if not (b.kind == 'class' and b.name in ('java.lang.Object', 'System.Object'))]
        if 'new' in (tp.flags or []):
            parts.append('new()')
        return ('where %s : %s' % (ident(tp.name), ', '.join(parts))) if parts else ''

    def member(self, m, c, ind):
        if isinstance(m, ir.ClassDecl):
            return self.class_decl(m, ind)
        if isinstance(m, ir.FieldDecl):
            return self.field(m, c, ind)
        if isinstance(m, ir.Property):
            return self.prop(m, c, ind)
        if isinstance(m, ir.MethodDecl):
            return self.method_decl(m, c, ind)
        if isinstance(m, ir.Unsupported):
            self.note(m.kind, m.raw or m.kind)
            return [ind + '// CB-GAP: %s' % (m.raw or m.kind)]
        if isinstance(m, ir.EnumConst):
            raise Decline('enum', 'an enum constant outside an enum')
        raise Decline('member', 'a member %s' % type(m).__name__)

    def field(self, f, c, ind):
        mods = self.lib.field_mods(f, c) if self.lang == 'java' else list(f.mods or [])
        text = ind + ''.join(x + ' ' for x in mods) + self.ctype(f.type) + ' ' + ident(f.sym.name)
        init = self.prog.inits.get(id(f), f.init)
        if init is not None:
            self.method = None
            try:
                text += ' = ' + self.coerce(init, f.type)
            except Decline as d:
                self.note(d.kind, d.why)
                text += ' = default /* CB-GAP: %s */' % d.why
        return [text + ';']

    def prop(self, p, c, ind):
        mods = [m for m in p.mods or [] if m not in ('positional',)]
        name = 'this[' + self.params_text(p.params) + ']' if p.params else ident(p.sym.name)
        head = ind + ''.join(x + ' ' for x in mods) + self.ctype(p.type) + ' ' + name
        accs = p.accessors or []
        if len(accs) == 1 and accs[0].kind == 'get' and accs[0].body is not None and not accs[0].mods:
            b = accs[0].body.stmts
            if len(b) == 1 and isinstance(b[0], ir.Return) and b[0].expr is not None:
                try:
                    self.method = accs[0]
                    return [head + ' => ' + self.expr(b[0].expr)[0] + ';']
                except Decline:
                    pass
        if all(a.body is None for a in accs):
            text = head + ' { ' + ' '.join(''.join(x + ' ' for x in a.mods or []) + a.kind + ';' for a in accs) + ' }'
            if p.init is not None:
                self.method = None
                text += ' = ' + self.coerce(p.init, p.type) + ';'
            return [text]
        lines = [head, ind + '{']
        for a in accs:
            amods = ''.join(x + ' ' for x in a.mods or [])
            if a.body is None:
                lines.append(ind + '    ' + amods + a.kind + ';')
                continue
            self.method = a
            body = self.method_body(a, ind + '    ')
            st = a.body.stmts
            if len(st) == 1 and isinstance(st[0], ir.Return) and st[0].expr is not None and len(body) == 3:
                lines.append(ind + '    ' + amods + a.kind + ' => ' + body[1].strip()[len('return '):])
            elif len(st) == 1 and isinstance(st[0], ir.ExprStmt) and len(body) == 3:
                lines.append(ind + '    ' + amods + a.kind + ' => ' + body[1].strip())
            else:
                lines.append(ind + '    ' + amods + a.kind)
                lines += body
        lines.append(ind + '}')
        if p.init is not None:
            lines[-1] += ' = ' + self.coerce(p.init, p.type) + ';'
        return lines

    def params_text(self, params, varargs=False):
        out = []
        for i, p in enumerate(params or []):
            mods = [m for m in (p.mods or []) if m in ('ref', 'out', 'in', 'params', 'this', 'scoped')]
            if varargs and i == len(params) - 1 and 'params' not in mods:
                mods.append('params')
            if p.sym is not None and p.sym.refs and p.sym.refs[0] and p.sym.refs[0] not in mods:
                mods.insert(0, p.sym.refs[0])
            out.append(''.join(m + ' ' for m in mods) + self.ctype(p.type) + ' ' + ident(p.sym.name if p.sym else 'p'))
        return ', '.join(out)

    def method_decl(self, m, c, ind):
        self.method = m
        self.labels = {}
        if m.kind == 'static_init':
            if self.lang == 'java':
                return []                                     # (folded into the static constructor)
            head = ind + 'static ' + ident(c.name) + '()'
            return [head] + self.method_body(m, ind)
        if m.kind == 'init':
            return []                                         # (Java's instance blocks run in each constructor)
        if m.kind == 'cctor':                                 # Java's statics, in their one textual order
            return [ind + 'static ' + ident(c.name) + '()'] + self.method_body(m, ind)
        if m.kind == 'ctor':
            if 'IMPLICIT' in (m.mods or []) and not (self.lang == 'java' and self.lib.ctor_needed(c, m)):
                return []
            mods = self.lib.method_mods(m, c) if self.lang == 'java' else [x for x in m.mods or [] if x != 'IMPLICIT']
            head = ind + ''.join(x + ' ' for x in mods) + ident(c.name) + '(' +                 self.params_text(m.params, varargs=self.lang == 'java' and m.sym is not None and m.sym.varargs) + ')'
            stmts = list(m.body.stmts) if m.body else []
            call = stmts[0] if stmts and isinstance(stmts[0], ir.CtorCall) else None
            if call is not None:
                stmts = stmts[1:]
                if not call.kind.endswith(':implicit') and call.target is None:
                    try:
                        head += ' : %s(%s)' % ('this' if call.kind == 'this' else 'base', self.args_text(call.args, call.sym))
                    except Decline as d:
                        return self.gap(head, d, ind)
            if self.lang == 'java':
                stmts = self.lib.ctor_prologue(c, m, call) + stmts
            body = ir.Block(stmts=stmts)
            return [head] + self.body_lines(body, ind, m)
        mods = self.lib.method_mods(m, c) if self.lang == 'java' else list(m.mods or [])
        name = self.lib.method_name(m, c) if self.lang == 'java' else ident(m.name)
        if 'operator' in mods and m.name in _DECL_OPERATORS:
            mods = [x for x in mods if x != 'operator']
            head = ind + ''.join(x + ' ' for x in mods) + self.ctype(m.ret) + ' operator ' + _DECL_OPERATORS[m.name]
        elif 'operator' in mods and m.name in ('op_Implicit', 'op_Explicit'):
            mods = [x for x in mods if x != 'operator']
            head = ind + ''.join(x + ' ' for x in mods) + ('implicit' if m.name == 'op_Implicit' else 'explicit') + \
                ' operator ' + self.ctype(m.ret)
        else:
            head = ind + ''.join(x + ' ' for x in mods) + self.ctype(m.ret) + ' ' + name + self.tparams_text(m)
        head += '(' + self.params_text(m.params, varargs=self.lang == 'java' and m.sym is not None and m.sym.varargs) + ')'
        wheres = [self.where_text(tp) for tp in m.tparams or []]
        wheres = [w for w in wheres if w]
        if m.body is None:
            return [head + (' ' + ' '.join(wheres) if wheres else '') + ';']
        lines = [head] + [ind + '    ' + w for w in wheres]
        st = m.body.stmts
        if len(st) == 1 and isinstance(st[0], ir.Return) and st[0].expr is not None and self.lang == 'csharp' \
                and not wheres:
            try:
                return [head + ' => ' + self.ret_expr(st[0].expr) + ';']
            except Decline:
                pass
        return lines + self.method_body(m, ind)

    def ret_expr(self, e):
        return self.coerce(e, self.method.ret if self.method is not None else None)

    def method_body(self, m, ind):
        return self.body_lines(m.body, ind, m)

    def body_lines(self, body, ind, m):
        if ir.unsupported(body):
            u = ir.unsupported(body)[0]
            return self.gap(None, Decline(u.kind, u.raw or u.kind), ind)
        try:
            out = []
            self.locals = set(p.sym.name for p in (m.params or []) if p.sym is not None) if m is not None else set()
            self.block_names, self.scopes = [set(self.locals)], [{}]
            self.block_into(body, out, ind)
            return out
        except Decline as d:
            return self.gap(None, d, ind)

    def gap(self, head, d, ind):
        self.note(d.kind, d.why)
        lines = [head] if head else []
        return lines + [ind + '{', ind + '    throw new System.NotSupportedException(%s);' % _cstr('CB-GAP: ' + d.why),
                        ind + '}']

    # ---- statements
    def block_into(self, b, out, ind):
        out.append(ind + '{')
        stmts = b.stmts if isinstance(b, ir.Block) else [b]
        if self.lang == 'csharp':
            stmts = _initializers(stmts)
        self.block_names.append(set(x.sym.name for x in stmts if isinstance(x, ir.Local) and x.sym is not None))
        self.push()
        try:
            for s in stmts:
                self.stmt(s, out, ind + '    ')
        finally:
            self.pop()
            self.block_names.pop()
        out.append(ind + '}')

    def sub(self, s, out, ind):
        """A statement in a place that holds one (if, loop): always a block, for clarity."""
        if isinstance(s, ir.Block):
            self.block_into(s, out, ind)
        else:
            self.block_into(ir.Block(stmts=[s]), out, ind)

    def stmt(self, s, out, ind):
        k = type(s).__name__
        f = getattr(self, 's_' + k, None)
        if f is None:
            raise Decline(k, 'the statement %s' % k)
        f(s, out, ind)

    def s_Block(self, s, out, ind):
        self.block_into(s, out, ind)

    def s_Empty(self, s, out, ind):
        out.append(ind + ';')

    def s_Comment(self, s, out, ind):
        out.append(ind + '// ' + s.text)

    def s_Local(self, s, out, ind):
        mods = 'const ' if 'const' in (s.mods or []) and self.lang == 'csharp' else ''
        if s.init is None:
            name = self.declare(s.sym.name)
            out.append(ind + mods + self.ctype(s.type) + ' ' + name + ';')
            return
        init = self.coerce(s.init, s.type)
        name = self.declare(s.sym.name)
        if self.lang == 'java':
            self.lib.record(s.sym, s.init)
        tn = self.ctype(s.type)
        if isinstance(_strip(s.init), (ir.New, _Init)) and s.init.type == s.type and not mods:
            out.append(ind + 'var ' + name + ' = ' + init + ';')
        else:
            out.append(ind + mods + tn + ' ' + name + ' = ' + init + ';')

    def s_ExprStmt(self, s, out, ind):
        e = _strip(s.expr)
        if isinstance(e, ir.Unsupported):
            raise Decline(e.kind, e.raw or e.kind)
        text = self.lib.stmt_call(e) if self.lang == 'java' and isinstance(e, ir.Call) else None
        if text is None:
            text = self.expr(e, statement=True)[0]
        out.append(ind + text + ';')

    def s_If(self, s, out, ind):
        out.append(ind + 'if (' + self.cond(s.cond) + ')')
        self.sub(s.then, out, ind)
        e = s.orelse
        while e is not None:
            if isinstance(e, ir.If):
                out.append(ind + 'else if (' + self.cond(e.cond) + ')')
                self.sub(e.then, out, ind)
                e = e.orelse
            else:
                out.append(ind + 'else')
                self.sub(e, out, ind)
                e = None

    def cond(self, e):
        return self.coerce(_strip(e), ir.BOOL)           # (the parentheses an if or a loop writes anyway)

    def loop_body(self, s, body, out, ind):
        """A loop's body, with the label a Java `continue label` jumps to at its end."""
        lab = self.loop_labels.get(id(s))
        if lab is None or not self.labels.get(lab, {}).get('continue'):
            self.sub(body, out, ind)
            return
        stmts = body.stmts if isinstance(body, ir.Block) else [body]
        out.append(ind + '{')
        for x in stmts:
            self.stmt(x, out, ind + '    ')
        out.append(ind + lab + '_continue:;')
        out.append(ind + '}')

    def s_While(self, s, out, ind):
        out.append(ind + 'while (' + self.cond(s.cond) + ')')
        self.loop_body(s, s.body, out, ind)

    def s_DoWhile(self, s, out, ind):
        out.append(ind + 'do')
        self.loop_body(s, s.body, out, ind)
        out.append(ind + 'while (' + self.cond(s.cond) + ');')

    def s_For(self, s, out, ind):
        self.push()
        try:
            self._for(s, out, ind)
        finally:
            self.pop()

    def _for(self, s, out, ind):
        inits = []
        for i in s.init or []:
            if isinstance(i, ir.Local):
                v = self.coerce(i.init, i.type) if i.init is not None else None
                inits.append((self.ctype(i.type), self.declare(i.sym.name, nested=True) + (' = ' + v if v is not None else '')))
            elif isinstance(i, ir.ExprStmt):
                inits.append((None, self.expr(i.expr, statement=True)[0]))
            else:
                raise Decline('for', 'a for initialiser %s' % type(i).__name__)
        if inits and inits[0][0] is not None:
            if any(t != inits[0][0] for t, _x in inits):
                raise Decline('for', 'a for initialiser declaring locals of several types')
            init = inits[0][0] + ' ' + ', '.join(x for _t, x in inits)
        else:
            if any(t is not None for t, _x in inits):
                raise Decline('for', 'a for initialiser mixing declarations and expressions')
            init = ', '.join(x for _t, x in inits)
        cond = self.cond(s.cond) if s.cond is not None else ''
        upd = ', '.join(self.expr(u, statement=True)[0] for u in s.update or [])
        out.append(ind + 'for (%s; %s; %s)' % (init, cond, upd))
        self.loop_body(s, s.body, out, ind)

    def s_ForEach(self, s, out, ind):
        v = s.var
        coll = self.lib.iterable(s.iterable) if self.lang == 'java' else None
        if coll is None:
            coll = self.expr(s.iterable)[0]
        self.push()
        try:
            out.append(ind + 'foreach (%s %s in %s)' % (self.ctype(v.type), self.declare(v.sym.name, nested=True), coll))
            self.loop_body(s, s.body, out, ind)
        finally:
            self.pop()

    def s_Labeled(self, s, out, ind):
        lab = ident(s.label)
        body = s.body
        if isinstance(body, (ir.While, ir.DoWhile, ir.For, ir.ForEach)):
            uses = self._label_uses(body, s.label)
            self.labels[lab] = uses
            self.loop_labels[id(body)] = lab
            self.stmt(body, out, ind)
            if uses.get('break'):
                out.append(ind + lab + '_break:;')
            return
        if self.lang == 'csharp' or not self._label_uses(body, s.label).get('break'):
            out.append(ind + lab + ':')
            self.stmt(body, out, ind)
            return
        self.labels[lab] = {'break': True}
        self.stmt(body, out, ind)
        out.append(ind + lab + '_break:;')

    def _label_uses(self, body, label):
        uses = {}
        for n in ir.walk(body):
            if isinstance(n, ir.Break) and n.label == label:
                uses['break'] = True
            if isinstance(n, ir.Continue) and n.label == label:
                uses['continue'] = True
        return uses

    def s_Break(self, s, out, ind):
        if s.label:
            out.append(ind + 'goto %s_break;' % ident(s.label))
        else:
            out.append(ind + 'break;')

    def s_Continue(self, s, out, ind):
        if s.label:
            out.append(ind + 'goto %s_continue;' % ident(s.label))
        else:
            out.append(ind + 'continue;')

    def s_Goto(self, s, out, ind):
        out.append(ind + 'goto %s;' % ident(s.label))

    def s_Return(self, s, out, ind):
        if s.expr is None:
            out.append(ind + 'return;')
        else:
            out.append(ind + 'return ' + self.ret_expr(s.expr) + ';')

    def s_Throw(self, s, out, ind):
        if s.expr is None:
            out.append(ind + 'throw;')
        else:
            out.append(ind + 'throw ' + self.expr(s.expr)[0] + ';')

    def s_Sync(self, s, out, ind):
        out.append(ind + 'lock (' + self.expr(s.lock)[0] + ')')
        self.sub(s.body, out, ind)

    def s_Assert(self, s, out, ind):
        if self.lang == 'java':
            out.append(ind + '// assert %s (Java runs asserts only with -ea)' % self.expr(s.cond)[0])
            return
        self.usings.add('System.Diagnostics')
        out.append(ind + 'Debug.Assert(' + self.cond(s.cond) + ');')

    def s_LocalClass(self, s, out, ind):
        raise Decline('local-class', 'a class declared in a method')

    def s_CtorCall(self, s, out, ind):
        raise Decline('ctor-call', 'a constructor call that is not the first statement')

    def s_Switch(self, s, out, ind):
        out.append(ind + 'switch (' + self.expr(s.expr)[0] + ')')
        out.append(ind + '{')
        cases = s.cases or []
        for i, c in enumerate(cases):
            for lab in c.labels or []:
                out.append(ind + '    case ' + self.case_label(lab, s.expr) + ':')
            if c.default:
                out.append(ind + '    default:')
            if not c.body and i < len(cases) - 1:
                continue                                    # labels sharing the next section
            body = list(c.body)
            inner = ind + '        '
            for x in body:
                self.stmt(x, out, inner)
            if not _abrupt(body):
                nxt = next((cases[j] for j in range(i + 1, len(cases))), None)
                if nxt is None:
                    out.append(inner + 'break;')
                elif nxt.labels:
                    out.append(inner + 'goto case ' + self.case_label(nxt.labels[0], s.expr) + ';')
                else:
                    out.append(inner + 'goto default;')
        out.append(ind + '}')

    def case_label(self, lab, scrutinee):
        lab = _strip(lab)
        if isinstance(lab, ir.Select) and lab.sym is not None and lab.sym.kind == 'enumconst':
            return self.ctype(ir.Type('class', lab.sym.owner)) + '.' + ident(lab.sym.name)
        if isinstance(lab, ir.Lit) and isinstance(lab.value, int) and scrutinee is not None \
                and scrutinee.type is not None and scrutinee.type.kind == 'u16' and not isinstance(lab.value, bool):
            return _cchar(chr(lab.value))
        return self.expr(lab)[0]

    def s_Try(self, s, out, ind):
        res = s.resources or []
        if res and not s.catches and s.final is None:
            self.using(res, s.body, out, ind)
            return
        out.append(ind + 'try')
        if res:
            out.append(ind + '{')
            self.using(res, s.body, out, ind + '    ')
            out.append(ind + '}')
        else:
            self.block_into(s.body, out, ind)
        for c in s.catches or []:
            types = c.types or []
            if len(types) > 1:
                # a multi-catch: one clause per type, each with the same body
                for t in types:
                    self.catch(c, t, out, ind)
                continue
            self.catch(c, types[0] if types else None, out, ind)
        if s.final is not None:
            out.append(ind + 'finally')
            self.block_into(s.final, out, ind)

    def catch(self, c, t, out, ind):
        tn = self.lib.exception(t) if self.lang == 'java' else (self.ctype(t) if t is not None else None)
        if tn is None:
            out.append(ind + 'catch')
            self.block_into(c.body, out, ind)
        elif c.sym is not None:
            self.push()
            try:
                out.append(ind + 'catch (%s %s)' % (tn, self.declare(c.sym.name, nested=True)))
                self.block_into(c.body, out, ind)
            finally:
                self.pop()
        else:
            out.append(ind + 'catch (%s)' % tn)
            self.block_into(c.body, out, ind)

    def using(self, res, body, out, ind):
        r = res[0]
        if isinstance(r, ir.Local):
            init = self.coerce(r.init, r.type)
            self.push()
            tn = 'var' if isinstance(_strip(r.init), ir.New) and r.init.type == r.type else self.ctype(r.type)
            head = 'using (%s %s = %s)' % (tn, self.declare(r.sym.name, nested=True), init)
        elif isinstance(r, ir.ExprStmt):
            head = 'using (%s)' % self.expr(r.expr)[0]
        else:
            raise Decline('using', 'a resource %s' % type(r).__name__)
        out.append(ind + head)
        try:
            if len(res) > 1:
                out.append(ind + '{')
                self.using(res[1:], body, out, ind + '    ')
                out.append(ind + '}')
            else:
                self.block_into(body, out, ind)
        finally:
            if isinstance(r, ir.Local):
                self.pop()

    def s_Yield(self, s, out, ind):
        raise Decline('yield', 'a switch-expression arm with statements')

    def s_MachineFragment(self, s, out, ind):
        raise Decline('machine', 'machine code (%s)' % (s.reason or 'asm'))

    # ---- expressions: each gives (text, precedence)
    def expr(self, e, statement=False):
        k = type(e).__name__
        f = getattr(self, 'x_' + k, None)
        if f is None:
            raise Decline(k, 'the expression %s' % k)
        r = f(e)
        if not statement and not isinstance(_strip(e), (ir.Lit, ir.Paren)) and not _lib_const(_strip(e)) and e.type is not None \
                and e.type.kind in _RANGE and _is_const(e) and _overflows(e):
            return 'unchecked(' + r[0] + ')', P_PRIMARY     # a constant that wraps: C# would refuse to fold it
        return r

    def wrap(self, e, prec, right=False):
        text, p = self.expr(e)
        if p < prec or (right and p == prec):
            return '(' + text + ')'
        return text

    def coerce(self, e, t):
        """e as a value of type t: a char from an int, as Java converts a constant implicitly."""
        if t is not None and t.kind == 'u16' and e.type is not None and e.type.kind in ('i32', 'i8', 'i16', 'u8') \
                and self.ctype(t) == 'char':
            if isinstance(_strip(e), ir.Lit) and isinstance(_strip(e).value, int):
                return _cchar(chr(_strip(e).value & 0xffff))
            return '(char)' + self.wrap(e, P_UNARY)
        if t is not None and t.kind in ('i8', 'i16', 'u8') and e.type is not None and e.type.kind == 'u16' \
                and isinstance(_strip(e), ir.Lit) and self.lang == 'java':
            v = _strip(e).value
            return str(ord(v) if isinstance(v, str) else v)
        if self.lang == 'java':
            got = self.lib.coerce(e, t)
            if got is not None:
                return got
        return self.expr(e)[0]

    def x_Paren(self, e):
        return self.expr(e.x)              # (grouping is written from the operators' precedence, where needed)

    def x_Lit(self, e):
        v, t = e.value, e.type
        if v is None:
            return 'null', P_PRIMARY
        if isinstance(v, bool):
            return ('true' if v else 'false'), P_PRIMARY
        if isinstance(v, str):
            if t is not None and t.kind == 'u16' and len(v) == 1:
                if self.ctype(t) == 'ushort':
                    return str(ord(v)), P_PRIMARY
                return _cchar(v), P_PRIMARY
            return _cstr(v), P_PRIMARY
        if isinstance(v, float) or (t is not None and t.kind in ('f32', 'f64')):
            s = _float(float(v), t)
            return s, (P_UNARY if s.startswith('-') else P_PRIMARY)
        if isinstance(v, int):
            k = t.kind if t is not None else 'i32'
            if k == 'u16' and self.ctype(t) == 'char':
                return _cchar(chr(v & 0xffff)), P_PRIMARY
            suffix = {'i64': 'L', 'u32': 'u', 'u64': 'UL'}.get(k, '')
            s = str(v) + suffix
            s = {('i32', -2 ** 31): 'int.MinValue', ('i32', 2 ** 31 - 1): 'int.MaxValue',
                 ('i64', -2 ** 63): 'long.MinValue', ('i64', 2 ** 63 - 1): 'long.MaxValue',
                 ('u32', 2 ** 32 - 1): 'uint.MaxValue', ('u64', 2 ** 64 - 1): 'ulong.MaxValue'}.get((k, v), s)
            return s, (P_UNARY if s.startswith('-') else P_PRIMARY)
        raise Decline('literal', 'a literal %r' % (v,))

    def x_Name(self, e):
        return self.lookup(e.sym.name), P_PRIMARY

    def x_This(self, e):
        if e.qualifier is not None and self.cls is not None and e.qualifier.name != self.cls.qname:
            raise Decline('outer-this', 'an outer instance (Outer.this)')
        return 'this', P_PRIMARY

    def x_Super(self, e):
        return 'base', P_PRIMARY

    def x_TypeName(self, e):
        return self.ctype(e.type), P_PRIMARY

    def x_Select(self, e):
        got = self.lib.select(e)
        if got is not None:
            return got
        s = e.sym
        if isinstance(e.obj, ir.This) and e.obj.qualifier is None and not self.shadowed(s.name):
            return self.member_name(s), P_PRIMARY              # this.x, where no local hides x
        if isinstance(e.obj, ir.TypeName) and self.cls is not None and e.obj.type.name == self.cls.qname:
            return ident(s.name), P_PRIMARY                    # a static of this class
        return self.wrap(e.obj, P_PRIMARY) + '.' + self.member_name(s), P_PRIMARY

    def member_name(self, s):
        if self.lang == 'java':
            return self.lib.member_name(s)
        return ident(s.name)

    def shadowed(self, name):
        return name in self.locals

    def x_Length(self, e):
        return self.wrap(e.arr, P_PRIMARY) + '.Length', P_PRIMARY

    def x_Index(self, e):
        if e.sym is not None and self.lang == 'java':
            raise Decline('indexer', 'an indexer from Java')
        idx = e.index if isinstance(e.index, list) else [e.index]
        return self.wrap(e.arr, P_PRIMARY) + '[' + ', '.join(self.expr(i)[0] for i in idx) + ']', P_PRIMARY

    def x_Unary(self, e):
        if e.op in ('++', '--') and e.postfix:
            return self.wrap(e.x, P_PRIMARY) + e.op, P_PRIMARY
        x = self.wrap(e.x, P_UNARY)
        if e.op in ('-', '+') and x.startswith(e.op):
            x = '(' + x + ')'                                   # - -x, not --x
        text = e.op + x
        if e.overflow == 'trap':
            return 'checked(' + text + ')', P_PRIMARY
        return text, P_UNARY

    def x_Binary(self, e):
        op = e.op
        if self.lang == 'java':
            got = self.lib.binary(e)
            if got is not None:
                return got
        prec = _BIN_PREC.get(op)
        if prec is None:
            raise Decline('operator', 'the operator %s' % op)
        if op == '??':
            text = self.wrap(e.left, prec, right=True) + ' ?? ' + self.wrap(e.right, prec)
        else:
            text = self.wrap(e.left, prec) + ' ' + op + ' ' + self.wrap(e.right, prec, right=True)
        if e.overflow == 'trap':
            return 'checked(' + text + ')', P_PRIMARY
        return text, prec

    def x_Concat(self, e):
        parts = list(e.parts)
        texts = []
        for i, p in enumerate(parts):
            got = self.lib.concat_part(p) if self.lang == 'java' else None
            if got is not None:
                texts.append(got)
                continue
            texts.append(self.wrap(p, P_ADD, right=i > 0))
        if len(parts) >= 2 and not is_string(parts[0].type) and not is_string(parts[1].type) \
                and not (texts[0].startswith('JavaRt.Str(') or texts[1].startswith('JavaRt.Str(')):
            texts.insert(0, '""')
        if len(texts) == 1:
            texts.insert(0, '""')
        if self.lang == 'csharp' and all(isinstance(p, ir.Lit) and isinstance(p.value, str) or not isinstance(p, ir.Lit)
                                         for p in parts) and len(parts) > 2 and _interpolatable(parts):
            return self.interpolate(parts), P_PRIMARY
        return ' + '.join(texts), P_ADD

    def interpolate(self, parts):
        out = '$"'
        for p in parts:
            if isinstance(p, ir.Lit) and isinstance(p.value, str):
                out += _cstr(p.value)[1:-1].replace('{', '{{').replace('}', '}}')
            else:
                text = self.expr(p)[0]
                if ':' in text or '?' in text:
                    text = '(' + text + ')'
                out += '{' + text + '}'
        return out + '"'

    def x_Assign(self, e):
        target = self.wrap(e.target, P_UNARY)
        if e.op is None:
            value = self.coerce(e.value, e.target.type)
            return target + ' = ' + value, P_ASSIGN
        op = e.op + '='
        value = self.expr(e.value)[0]
        if e.overflow == 'trap':
            return 'checked(' + target + ' ' + op + ' ' + value + ')', P_PRIMARY
        return target + ' ' + op + ' ' + value, P_ASSIGN

    def x_Cond(self, e):
        got = self.null_conditional(e)
        if got is not None:
            return got
        c = self.wrap(e.cond, P_COND + 1)
        a = self.coerce(e.then, e.type) if e.type is not None else self.expr(e.then)[0]
        b = self.wrap(e.orelse, P_COND) if e.type is None else self.coerce(e.orelse, e.type)
        if ' = ' in a:
            a = '(' + a + ')'
        return c + ' ? ' + a + ' : ' + b, P_COND

    def null_conditional(self, e):
        """r != null ? r.m : null, written r?.m (C#'s own form of the test and the access)."""
        c = _strip(e.cond)
        if self.lang != 'csharp' or not (isinstance(c, ir.Binary) and c.op == '!=' and isinstance(_strip(c.right), ir.Lit)
                                         and _strip(c.right).value is None):
            return None
        if not (isinstance(_strip(e.orelse), ir.Lit) and _strip(e.orelse).value is None):
            return None
        r = self.expr(c.left)[0]
        then = self.expr(e.then)
        if then[1] == P_PRIMARY and then[0].startswith(r + '.') and not then[0].startswith(r + '.' + r):
            return r + '?.' + then[0][len(r) + 1:], P_PRIMARY
        return None

    def x_Cast(self, e):
        tn = self.ctype(e.type)
        x = self.wrap(e.x, P_UNARY)
        if x.startswith('-') or x.startswith('+'):
            x = '(' + x + ')'
        text = '(' + tn + ')' + x
        if e.overflow == 'trap':
            return 'checked(' + text + ')', P_PRIMARY
        return text, P_UNARY

    def x_InstanceOf(self, e):
        text = self.wrap(e.x, P_REL) + ' is ' + self.ctype(e.test)
        if e.binding is not None and e.binding.sym is not None:
            text += ' ' + self.declare(e.binding.sym.name)
        return text, P_REL

    def x_ClassLit(self, e):
        return 'typeof(' + self.ctype(e.of) + ')', P_PRIMARY

    def x_Lambda(self, e):
        if self.lang == 'java':
            self.lib.check_lambda(e)
        # untyped parameters: a lambda here always has a target type (an argument, a typed local, a return)
        saved = self.locals
        self.locals = set(saved)
        self.push()
        names = [self.declare(p.sym.name, nested=True) for p in e.params or []]
        ps = names[0] if len(names) == 1 else '(' + ', '.join(names) + ')'
        try:
            if isinstance(e.body, ir.Block):
                out = []
                self.block_into(e.body, out, '')
                body = '\n'.join(out)
                # keep it on lines of its own, re-indented by the caller's line
                return ps + ' =>\n' + body, P_ASSIGN
            return ps + ' => ' + self.expr(e.body)[0], P_ASSIGN
        finally:
            self.locals = saved
            self.pop()

    def x_MethodRef(self, e):
        if e.mode == 'new':
            raise Decline('method-ref', 'a constructor reference')
        if isinstance(e.target, ir.TypeName) and not e.sym.static:
            raise Decline('method-ref', 'an unbound method reference (Type::instanceMethod)')
        return self.wrap(e.target, P_PRIMARY) + '.' + self.member_name(e.sym), P_PRIMARY

    def x_SwitchExpr(self, e):
        arms = []
        for c in e.cases or []:
            if len(c.body) != 1:
                raise Decline('switch-expression', 'a switch-expression arm with statements')
            b = c.body[0]
            if isinstance(b, ir.Yield):
                val = self.coerce(b.expr, e.type)
            elif isinstance(b, ir.Throw):
                val = 'throw ' + self.expr(b.expr)[0]
            elif isinstance(b, ir.ExprStmt):
                val = self.expr(b.expr)[0]
            else:
                raise Decline('switch-expression', 'a switch-expression arm %s' % type(b).__name__)
            pats = [self.case_label(l, e.expr) for l in c.labels or []]
            if c.default:
                pats = ['_']
            arms.append(' or '.join(pats) + ' => ' + val)
        return self.wrap(e.expr, P_PRIMARY) + ' switch { ' + ', '.join(arms) + ' }', P_PRIMARY

    def x_New(self, e):
        if e.body is not None:
            raise Decline('anonymous-class', 'an anonymous class')
        if e.outer is not None:
            raise Decline('inner-class', 'an inner class made on an outer instance')
        got = self.lib.new(e)
        if got is not None:
            return got
        return 'new ' + self.ctype(e.type) + '(' + self.args_text(e.args, e.sym) + ')', P_PRIMARY

    def x_NewArray(self, e):
        t = e.type
        if e.init is not None:
            if e.dims:
                raise Decline('array', 'an array with sizes and items')
            elem = t.elem if t.kind == 'array' else e.elem
            items = ', '.join(self.coerce(x, elem) for x in e.init)
            return 'new ' + self.ctype(t) + ' { ' + items + ' }' if items else 'new ' + self.ctype(t) + ' { }', P_PRIMARY
        if t.kind == 'array' and t.info.get('rank', 1) != 1:
            return 'new ' + self.ctype(t.elem) + '[' + ', '.join(self.expr(d)[0] for d in e.dims) + ']', P_PRIMARY
        n, base = t.dims() if t.kind == 'array' else (1, e.elem)
        sizes = ''.join('[' + self.expr(d)[0] + ']' for d in e.dims)
        rest = '[]' * (n - len(e.dims))
        return 'new ' + self.ctype(base) + sizes + rest, P_PRIMARY

    def args_text(self, args, s):
        out = []
        refs = list(s.refs) if s is not None and s.refs else []
        params = list(s.params) if s is not None else []
        for i, a in enumerate(args or []):
            r = refs[i] if i < len(refs) else ''
            pt = params[i] if i < len(params) and not (s.varargs and i >= len(params) - 1) else None
            text = self.coerce(a, pt) if pt is not None else self.expr(a)[0]
            if self.lang == 'java':
                text = self.lib.arg(a, pt, text)
            out.append((r + ' ' if r and r != 'in' else '') + text)
        return ', '.join(out)

    def x_Call(self, e):
        got = self.lib.call(e)
        if got is not None:
            return got
        s = e.sym
        if s is None:
            raise Decline('call', 'a call with no resolved method')
        flags = s.flags or ()
        if 'operator' in flags:
            if s.name in _OPERATOR_METHODS and len(e.args) == 2:
                op = _OPERATOR_METHODS[s.name]
                prec = _BIN_PREC[op]
                return self.wrap(e.args[0], prec) + ' ' + op + ' ' + self.wrap(e.args[1], prec, right=True), prec
            if s.name in _UNARY_METHODS and len(e.args) == 1:
                return _UNARY_METHODS[s.name] + self.wrap(e.args[0], P_UNARY), P_UNARY
            if s.name == 'op_Implicit' and len(e.args) == 1:
                return self.expr(e.args[0])
            if s.name == 'op_Explicit' and len(e.args) == 1:
                return '(' + self.ctype(e.type) + ')' + self.wrap(e.args[0], P_UNARY), P_UNARY
        if 'delegate' in flags and e.target is not None:
            return self.wrap(e.target, P_PRIMARY) + '(' + self.args_text(e.args, s) + ')', P_PRIMARY
        targs = ''
        if e.typeargs and 'inferred' not in flags and (self.lang == 'csharp' or not _inferable(e)):
            targs = '<' + ', '.join(self.ctype(t) for t in e.typeargs) + '>'
        name = self.member_name(s) + targs
        if 'extension' in flags and e.args and isinstance(e.target, ir.TypeName):
            rest = ir.Sym(s.kind, s.name, s.owner, s.static, s.type, s.params[1:], s.ret, s.varargs,
                          s.refs[1:] if s.refs else (), s.flags)
            c = self.prog.classes.get(s.owner)
            ns = self.namespace_of(c) if c is not None else (s.owner.rsplit('.', 1)[0] if '.' in s.owner else '')
            if ns:
                self.usings.add(ns)
            return self.wrap(e.args[0], P_PRIMARY) + '.' + name + '(' + self.args_text(e.args[1:], rest) + ')', P_PRIMARY
        if e.target is None:
            return name + '(' + self.args_text(e.args, s) + ')', P_PRIMARY
        if isinstance(e.target, ir.TypeName) and self.cls is not None and e.target.type.name == self.cls.qname:
            return name + '(' + self.args_text(e.args, s) + ')', P_PRIMARY
        return self.wrap(e.target, P_PRIMARY) + '.' + name + '(' + self.args_text(e.args, s) + ')', P_PRIMARY

    def x__Init(self, e):
        new = self.x_New(e.new)[0]
        if new.endswith('()'):
            new = new[:-2]
        items = []
        for kind, a, b in e.items:
            if kind == 'set':
                items.append(self.member_name(a) + ' = ' + self.coerce(b, a.type))
            elif len(b) == 1:
                items.append(self.expr(b[0])[0])
            else:
                items.append('{ ' + ', '.join(self.expr(x)[0] for x in b) + ' }')
        return new + ' { ' + ', '.join(items) + ' }', P_PRIMARY

    def x_Unsupported(self, e):
        raise Decline(e.kind, e.raw or e.kind)


class _Init(ir.Expr):
    """(the back end's own) an object or collection initialiser: what the front end hoisted, put back"""
    __slots__ = ('new', 'items')
    fields = ()


def _initializers(stmts):
    """var t = new T(); t.A = 1; t.Add(x); ... S(t)  ->  S(new T { A = 1 }) / S(new T { x }): the statements a
    C# object or collection initialiser was hoisted into, put back as the initialiser."""
    stmts = list(stmts)
    changed = True
    while changed:
        changed = False
        for i, st in enumerate(stmts):
            if not (isinstance(st, ir.Local) and st.sym is not None and st.sym.name.startswith('__t')
                    and isinstance(st.init, ir.New) and st.init.body is None):
                continue
            items, j = [], i + 1
            while j < len(stmts) and isinstance(stmts[j], ir.ExprStmt):
                e = stmts[j].expr
                if isinstance(e, ir.Assign) and e.op is None and isinstance(e.target, ir.Select)                         and _is_name(e.target.obj, st.sym) and e.target.sym.kind in ('field', 'property')                         and not _uses(e.value, st.sym):
                    items.append(('set', e.target.sym, e.value))
                elif isinstance(e, ir.Call) and _is_name(e.target, st.sym) and e.sym is not None and e.sym.name == 'Add'                         and not any(_uses(a, st.sym) for a in e.args):
                    items.append(('add', e.sym, list(e.args)))
                else:
                    break
                j += 1
            if j >= len(stmts) or not items or len(set(k for k, _a, _b in items)) > 1:
                continue
            uses = [n for n in ir.walk(stmts[j]) if _is_name(n, st.sym)]
            if len(uses) != 1:
                continue
            init = _Init(type=st.init.type, line=st.line, col=st.col)
            init.new, init.items = st.init, items
            _replace(stmts[j], uses[0], init)
            del stmts[i:j]
            changed = True
            break
    return stmts


def _is_name(e, s):
    return isinstance(e, ir.Name) and e.sym is not None and e.sym.name == s.name and e.sym.kind == s.kind


def _uses(e, s):
    return any(_is_name(n, s) for n in ir.walk(e))


def _replace(root, old, new):
    for n in ir.walk(root):
        for f in n.slots():
            v = getattr(n, f, None)
            if v is old:
                setattr(n, f, new)
                return True
            if isinstance(v, list):
                for k, x in enumerate(v):
                    if x is old:
                        v[k] = new
                        return True
    return False


def _interpolatable(parts):
    return any(isinstance(p, ir.Lit) and isinstance(p.value, str) for p in parts) and \
        not any(isinstance(p, ir.Lit) and isinstance(p.value, str) and ('\n' in p.value) for p in parts)


def _inferable(e):
    """Does C# infer this generic call's type arguments from its arguments? (Every type argument appears
    in a parameter's type, and no argument is a lambda or null.)"""
    s = e.sym
    if not s.params:
        return False
    if any(isinstance(_strip(a), (ir.Lambda, ir.MethodRef)) or (isinstance(_strip(a), ir.Lit) and _strip(a).value is None)
           for a in e.args or []):
        return False
    return True


def write(units):
    """-> Program (call .files() or .write(out_dir))."""
    return Program(units)
