"""csharp_java.py -- the C# back end's knowledge of Java: Java's library and Java's semantics, in C#.

A Java unit (Unit.lang 'java', from the javac front end) is written by backends/csharp.py's Writer with
this as its library. It maps Java's types onto .NET's (java.lang.String -> string, java.util.List ->
List<T>, java.util.function.Function -> Func<T, R>, ...), Java's library calls onto .NET's through the
API-mapping registry (codebreaker/java/api_maps/java.csharp.json: a template per call; JavaRt, the
small Java runtime in C#, where .NET's call means something else), and Java's semantics where C#'s
differ:

  * dispatch: a Java instance method is virtual unless final, private or static -- `virtual`, and
    `override` where it overrides a class of these units or Object (toString -> ToString, equals ->
    Equals, hashCode -> GetHashCode); `main(String[])` is C#'s `Main`;
  * construction order: Java runs instance field initialisers and instance blocks after the base
    constructor, C# before it; where the base is a class of these units (whose constructor could see
    them) they are moved into each constructor after its base call. Static initialisers and static
    blocks keep Java's one textual order, in a static constructor;
  * text: every value Java turns into text differently (doubles, floats, booleans, null, collections)
    goes through JavaRt.Str;
  * HashMap and HashSet iterate in hash order, Dictionary and HashSet<T> in another: iterating one is
    declined (TreeMap/TreeSet map to the sorted collections, whose order is Java's);
  * Java's boxed types map to .NET's values (Integer -> int): a boxed value that is null is declined.

What it cannot carry it declines (csharp.Decline), with the reason.
"""
from codebreaker.translate import ir
from codebreaker.translate.backends.csharp import (Decline, ident, P_PRIMARY, P_EQ, P_UNARY,
                                                   _strip, is_string)

_TYPES = {
    'java.lang.String': 'string', 'java.lang.Object': 'object', 'java.lang.Integer': 'int', 'java.lang.Long': 'long',
    'java.lang.Short': 'short', 'java.lang.Byte': 'sbyte', 'java.lang.Character': 'char', 'java.lang.Boolean': 'bool',
    'java.lang.Double': 'double', 'java.lang.Float': 'float', 'java.lang.Number': 'object',
    'java.lang.CharSequence': 'string', 'java.lang.StringBuilder': 'System.Text.StringBuilder',
    'java.lang.StringBuffer': 'System.Text.StringBuilder', 'java.lang.Math': 'Math', 'java.lang.StrictMath': 'Math',
    'java.lang.System': 'Console', 'java.lang.Throwable': 'Exception', 'java.lang.Exception': 'Exception',
    'java.lang.RuntimeException': 'Exception', 'java.lang.Error': 'Exception',
    'java.lang.ArithmeticException': 'ArithmeticException', 'java.lang.NullPointerException': 'NullReferenceException',
    'java.lang.ArrayIndexOutOfBoundsException': 'IndexOutOfRangeException',
    'java.lang.IndexOutOfBoundsException': 'ArgumentOutOfRangeException',
    'java.lang.StringIndexOutOfBoundsException': 'ArgumentOutOfRangeException',
    'java.lang.IllegalArgumentException': 'ArgumentException', 'java.lang.IllegalStateException': 'InvalidOperationException',
    'java.lang.UnsupportedOperationException': 'NotSupportedException', 'java.lang.NumberFormatException': 'FormatException',
    'java.lang.ClassCastException': 'InvalidCastException', 'java.lang.NegativeArraySizeException': 'OverflowException',
    'java.util.NoSuchElementException': 'InvalidOperationException',
    'java.lang.Comparable': 'IComparable', 'java.lang.Iterable': 'IEnumerable', 'java.lang.AutoCloseable': 'IDisposable',
    'java.io.Closeable': 'IDisposable', 'java.lang.Runnable': 'Action',
    'java.util.Random': 'JavaRandom',
}
_GENERIC = {
    'java.util.List': 'List', 'java.util.ArrayList': 'List', 'java.util.LinkedList': 'List', 'java.util.Vector': 'List',
    'java.util.Collection': 'ICollection', 'java.util.AbstractList': 'List',
    'java.util.Map': 'IDictionary', 'java.util.HashMap': 'Dictionary', 'java.util.LinkedHashMap': 'Dictionary',
    'java.util.Hashtable': 'Dictionary', 'java.util.TreeMap': 'SortedDictionary', 'java.util.SortedMap': 'IDictionary',
    'java.util.Set': 'ISet', 'java.util.HashSet': 'HashSet', 'java.util.LinkedHashSet': 'HashSet',
    'java.util.TreeSet': 'SortedSet', 'java.util.SortedSet': 'ISet',
    'java.util.Map.Entry': 'KeyValuePair',
}
_ARITY = {'List': 1, 'Dictionary': 2, 'SortedDictionary': 2, 'HashSet': 1, 'SortedSet': 1, 'KeyValuePair': 2,
          'IDictionary': 2, 'ISet': 1, 'ICollection': 1}
_ORDERED = ('java.util.TreeMap', 'java.util.LinkedHashMap', 'java.util.TreeSet', 'java.util.LinkedHashSet')
# functional interfaces -> delegates: (C# generic, how the interface's own type arguments become it)
_FUNCTIONS = {
    'java.util.function.Function': ('Func', lambda a: a), 'java.util.function.BiFunction': ('Func', lambda a: a),
    'java.util.function.Supplier': ('Func', lambda a: a), 'java.util.function.Consumer': ('Action', lambda a: a),
    'java.util.function.BiConsumer': ('Action', lambda a: a),
    'java.util.function.Predicate': ('Func', lambda a: a + ['bool']),
    'java.util.function.BiPredicate': ('Func', lambda a: a + ['bool']),
    'java.util.function.UnaryOperator': ('Func', lambda a: [a[0], a[0]]),
    'java.util.function.BinaryOperator': ('Func', lambda a: [a[0], a[0], a[0]]),
    'java.util.function.IntBinaryOperator': ('Func', lambda a: ['int', 'int', 'int']),
    'java.util.function.IntUnaryOperator': ('Func', lambda a: ['int', 'int']),
    'java.util.function.IntPredicate': ('Func', lambda a: ['int', 'bool']),
    'java.util.function.IntFunction': ('Func', lambda a: ['int'] + a),
    'java.util.function.ToIntFunction': ('Func', lambda a: a + ['int']),
    'java.util.function.IntSupplier': ('Func', lambda a: ['int']),
    'java.util.function.DoubleUnaryOperator': ('Func', lambda a: ['double', 'double']),
    'java.util.Comparator': ('Comparison', lambda a: a),
}
# a library class's family: the classes that answer a call the same way (the map's keys use the family)
_FAMILY = {
    'java.lang.String': 'String', 'java.lang.StringBuilder': 'StringBuilder', 'java.lang.StringBuffer': 'StringBuilder',
    'java.lang.AbstractStringBuilder': 'StringBuilder', 'java.lang.Object': 'Object', 'java.lang.Math': 'Math',
    'java.lang.StrictMath': 'Math', 'java.lang.Integer': 'Integer', 'java.lang.Long': 'Long', 'java.lang.Double': 'Double',
    'java.lang.Float': 'Float', 'java.lang.Boolean': 'Boolean', 'java.lang.Character': 'Character',
    'java.lang.System': 'System', 'java.lang.Thread': 'Thread', 'java.io.PrintStream': 'PrintStream',
    'java.util.Arrays': 'Arrays', 'java.util.Collections': 'Collections', 'java.util.Random': 'Random',
    'java.lang.Throwable': 'Throwable', 'java.lang.Exception': 'Throwable', 'java.lang.RuntimeException': 'Throwable',
    'java.util.Map.Entry': 'Entry', 'java.lang.Number': 'Integer', 'java.lang.CharSequence': 'String',
    'java.lang.Enum': 'Enum',
}
for _n in ('List', 'ArrayList', 'LinkedList', 'Vector', 'Collection', 'AbstractList', 'AbstractCollection', 'Iterable',
           'AbstractSequentialList'):
    _FAMILY['java.util.' + _n] = 'List'
_FAMILY['java.lang.Iterable'] = 'List'
for _n in ('Map', 'HashMap', 'LinkedHashMap', 'TreeMap', 'AbstractMap', 'SortedMap', 'Hashtable', 'NavigableMap'):
    _FAMILY['java.util.' + _n] = 'Map'
for _n in ('Set', 'HashSet', 'LinkedHashSet', 'TreeSet', 'AbstractSet', 'SortedSet', 'NavigableSet'):
    _FAMILY['java.util.' + _n] = 'Set'
for _q in _FUNCTIONS:
    _FAMILY[_q] = _q.rsplit('.', 1)[-1]
_FAMILY['java.lang.Runnable'] = 'Runnable'

_FIELDS = {
    ('java.lang.Integer', 'MAX_VALUE'): 'int.MaxValue', ('java.lang.Integer', 'MIN_VALUE'): 'int.MinValue',
    ('java.lang.Long', 'MAX_VALUE'): 'long.MaxValue', ('java.lang.Long', 'MIN_VALUE'): 'long.MinValue',
    ('java.lang.Short', 'MAX_VALUE'): 'short.MaxValue', ('java.lang.Short', 'MIN_VALUE'): 'short.MinValue',
    ('java.lang.Byte', 'MAX_VALUE'): 'sbyte.MaxValue', ('java.lang.Byte', 'MIN_VALUE'): 'sbyte.MinValue',
    ('java.lang.Character', 'MAX_VALUE'): 'char.MaxValue', ('java.lang.Character', 'MIN_VALUE'): 'char.MinValue',
    ('java.lang.Double', 'MAX_VALUE'): 'double.MaxValue', ('java.lang.Double', 'MIN_VALUE'): 'double.Epsilon',
    ('java.lang.Double', 'POSITIVE_INFINITY'): 'double.PositiveInfinity',
    ('java.lang.Double', 'NEGATIVE_INFINITY'): 'double.NegativeInfinity', ('java.lang.Double', 'NaN'): 'double.NaN',
    ('java.lang.Float', 'MAX_VALUE'): 'float.MaxValue', ('java.lang.Float', 'MIN_VALUE'): 'float.Epsilon',
    ('java.lang.Float', 'NaN'): 'float.NaN', ('java.lang.Math', 'PI'): 'Math.PI', ('java.lang.Math', 'E'): 'Math.E',
}
_OBJECT_METHODS = {('toString', 0): 'ToString', ('equals', 1): 'Equals', ('hashCode', 0): 'GetHashCode'}
_PARAM_KIND = {'i32': 'int', 'i64': 'long', 'i16': 'int', 'i8': 'int', 'u16': 'int', 'f32': 'float', 'f64': 'double',
               'bool': 'boolean'}

_REGISTRY = None


def registry():
    global _REGISTRY
    if _REGISTRY is None:
        from codebreaker.java import apimap
        _REGISTRY = apimap.load()
    return _REGISTRY


def _qual(t):
    return t.name if t is not None and t.kind == 'class' else None


class JavaLib(object):
    lang = 'java'

    def __init__(self, w):
        self.w = w
        self.prog = w.prog
        if not hasattr(self.prog, '_java_moved'):
            self.prog._java_moved = {}

    # ---- types
    def type_name(self, t):
        q = t.name
        if q in self.prog.classes:
            return None
        if q in _FUNCTIONS:
            gen, fn = _FUNCTIONS[q]
            args = fn([self.w.ctype(a) for a in t.args])
            if gen == 'Action' and not args:
                return 'Action'
            self.w.usings.add('System')
            return '%s<%s>' % (gen, ', '.join(args))
        if q in _GENERIC:
            gen = _GENERIC[q]
            self.w.usings.add('System.Collections.Generic')
            n = _ARITY[gen]
            args = [self.w.ctype(a) for a in t.args] or ['object'] * n     # a raw type: of Object
            if len(args) != n:
                raise Decline('type', 'the type %r' % (t,))
            return '%s<%s>' % (gen, ', '.join(args))
        if q in _TYPES:
            name = _TYPES[q]
            if name == 'JavaRandom':
                self.prog.runtime.add('JavaRt')
            if name in ('IComparable', 'IEnumerable') and t.args:
                if name == 'IEnumerable':
                    self.w.usings.add('System.Collections.Generic')
                return '%s<%s>' % (name, ', '.join(self.w.ctype(a) for a in t.args))
            if '.' not in name and name[:1].isupper():
                self.w.usings.add('System')
            return name
        if q.startswith('java.') or q.startswith('javax.'):
            raise Decline('library', 'the Java library class %s has no C# mapping' % q)
        return None

    def keep_interface(self, t):
        q = _qual(t)
        if q in ('java.io.Serializable', 'java.lang.Cloneable', 'java.util.RandomAccess'):
            return False
        return True

    def exception(self, t):
        if t is None:
            return None
        return self.w.ctype(t)

    # ---- declarations
    def check_class(self, c):
        if c.kind == 'class' and c.outer and 'static' not in (c.mods or []):
            outer = self.prog.classes.get(c.outer)
            if outer is not None and outer.kind == 'class':
                for n in ir.walk(c):
                    if isinstance(n, ir.This) and n.type is not None and n.type.name == outer.qname:
                        raise Decline('inner-class', 'an inner class that uses its outer instance (%s)' % c.name)
                    if isinstance(n, ir.Call) and n.target is None and n.sym is not None and n.sym.owner == outer.qname:
                        raise Decline('inner-class', 'an inner class that calls its outer instance (%s)' % c.name)
        if c.kind == 'enum':
            for m in c.members:
                if isinstance(m, ir.FieldDecl) or (isinstance(m, ir.MethodDecl) and 'IMPLICIT' not in (m.mods or []))                         or (isinstance(m, ir.EnumConst) and (m.args or m.body is not None)):
                    raise Decline('enum', 'a Java enum with fields, methods or constructor arguments (an enum of objects)')

    def class_mods(self, c):
        out = []
        for m in c.mods or []:
            if m in ('public', 'protected', 'private', 'abstract'):
                out.append(m)
            elif m == 'final' and c.kind == 'class':
                out.append('sealed')
        if c.outer is None and 'public' not in out:
            out.insert(0, 'internal')
        return out

    def _access(self, mods, c):
        acc = [m for m in mods if m in ('public', 'protected', 'private')]
        if acc == ['private'] and c.outer:
            acc = ['internal']                                # Java's private reaches the whole top-level class
        if not acc and c.kind != 'interface':
            acc = ['internal']
        if acc == ['protected']:
            acc = ['protected', 'internal']                   # Java's protected is package-visible too
        return acc

    def field_mods(self, f, c):
        mods = f.mods or []
        if c.kind == 'interface':
            return ['public', 'const'] if self._const(f) else ['public', 'static', 'readonly']
        out = self._access(mods, c)
        if 'static' in mods and 'final' in mods and self._const(f):
            return out + ['const']
        if 'static' in mods:
            out.append('static')
        if 'final' in mods and self.prog.inits.get(id(f), f.init) is not None and not self.prog._java_moved.get(id(f)):
            out.append('readonly')
        if 'volatile' in mods:
            out.append('volatile')
        return out

    def _const(self, f):
        init = _strip(f.init) if f.init is not None else None
        return isinstance(init, ir.Lit) and init.value is not None and (f.type.is_prim or is_string(f.type))

    def method_mods(self, m, c):
        mods = m.mods or []
        if c.kind == 'interface':
            out = []
            if 'static' in mods:
                out.append('static')
            return out
        out = self._access(mods, c)
        if m.kind == 'ctor':
            return out
        if 'static' in mods:
            return out + ['static']
        if 'abstract' in mods:
            return out + (['abstract', 'override'][:2 if self.overrides(c, m) else 1])
        if self.overrides(c, m):
            out.append('override')
            if 'final' in mods:
                out.append('sealed')
            return out
        if 'private' in mods or 'final' in mods or 'final' in (c.mods or []):
            return out
        return out + ['virtual']

    def overrides(self, c, m):
        if (m.name, len(m.params or [])) in _OBJECT_METHODS:
            return True
        return self.prog.overrides(c, m)

    def method_name(self, m, c):
        n = len(m.params or [])
        if (m.name, n) in _OBJECT_METHODS:
            return _OBJECT_METHODS[(m.name, n)]
        if m.name == 'main' and 'static' in (m.mods or []) and n == 1:
            return 'Main'
        if m.name == 'compareTo' and n == 1 and any(_qual(i) == 'java.lang.Comparable' for i in c.implements or []):
            return 'CompareTo'
        if m.name == 'close' and n == 0 and self._closeable(c):
            return 'Dispose'
        if m.name == c.name:
            return ident(m.name) + '_'                        # (C# forbids a member named as its class)
        return ident(m.name)

    def _closeable(self, c):
        seen = set()
        while c is not None and c.qname not in seen:
            seen.add(c.qname)
            if any(_qual(i) in ('java.lang.AutoCloseable', 'java.io.Closeable') for i in c.implements or []):
                return True
            c = self.prog.base_of(c)
        return False

    def member_name(self, s):
        if s.kind in ('method',):
            n = len(s.params)
            if (s.name, n) in _OBJECT_METHODS:
                return _OBJECT_METHODS[(s.name, n)]
            c = self.prog.classes.get(s.owner)
            if c is not None:
                if s.name == 'compareTo' and n == 1:
                    return 'CompareTo'
                if s.name == 'close' and n == 0 and self._closeable(c):
                    return 'Dispose'
                if s.name == c.name:
                    return ident(s.name) + '_'
                if s.name == 'main' and s.static and n == 1:
                    return 'Main'
        return ident(s.name)

    def arrange_members(self, c, members):
        """Java's initialisation order: instance initialisers after the base constructor (moved into the
        constructors, where the base is one of these units' classes), statics in one textual order."""
        out = []
        statics, has_block = [], False
        moves = []
        base_is_ours = c.extends is not None and c.extends.kind == 'class' and c.extends.name in self.prog.classes
        for m in members:
            if isinstance(m, ir.MethodDecl) and m.kind == 'static_init':
                has_block = True
                statics.append(('block', m))
                continue
            if isinstance(m, ir.FieldDecl) and 'static' in (m.mods or []) and m.init is not None and not self._const(m):
                statics.append(('field', m))
            if isinstance(m, ir.FieldDecl) and 'static' not in (m.mods or []) and m.init is not None and base_is_ours:
                moves.append(('field', m))
            if isinstance(m, ir.MethodDecl) and m.kind == 'init':
                moves.append(('block', m))
            out.append(m)
        if has_block:
            body = []
            for kind, m in statics:
                if kind == 'field':
                    self.prog.inits[id(m)] = None
                    self.prog._java_moved[id(m)] = True
                    body.append(ir.ExprStmt(expr=ir.Assign(op=None, target=ir.Select(obj=ir.TypeName(type=ir.Type('class', c.qname)),
                                                                                     sym=m.sym, type=m.type),
                                                           value=m.init, type=m.type)))
                else:
                    body += list(m.body.stmts)
            out.append(ir.MethodDecl(sym=None, name='<clinit>', kind='cctor', params=[], ret=ir.VOID,
                                     body=ir.Block(stmts=body), mods=['static'], throws=[], typeparams=[]))
        if moves:
            pro = []
            for kind, m in moves:
                if kind == 'field':
                    self.prog.inits[id(m)] = None
                    self.prog._java_moved[id(m)] = True
                    pro.append(ir.ExprStmt(expr=ir.Assign(op=None, target=ir.Select(obj=ir.This(qualifier=None, type=ir.Type('class', c.qname)),
                                                                                    sym=m.sym, type=m.type),
                                                          value=m.init, type=m.type)))
                else:
                    pro += list(m.body.stmts)
            self.prog._java_moved[('prologue', c.qname)] = pro
        return out

    def ctor_needed(self, c, m):
        return bool(self.prog._java_moved.get(('prologue', c.qname)))

    def ctor_prologue(self, c, m, call):
        if call is not None and call.kind == 'this':
            return []
        return list(self.prog._java_moved.get(('prologue', c.qname), []))

    # ---- expressions
    def select(self, e):
        s = e.sym
        if s is None or s.kind not in ('field',):
            return None
        key = (s.owner, s.name)
        if key in _FIELDS:
            self.w.usings.add('System')
            return _FIELDS[key], P_PRIMARY
        if s.owner == 'java.lang.System' and s.name in ('out', 'err'):
            self.w.usings.add('System')
            return ('Console.Out' if s.name == 'out' else 'Console.Error'), P_PRIMARY
        if s.owner and (s.owner.startswith('java.') or s.owner.startswith('javax.')):
            raise Decline('library', 'the Java library field %s.%s has no C# mapping' % (s.owner, s.name))
        return None

    def _lookup(self, e):
        s = e.sym
        fam = _FAMILY.get(s.owner)
        if fam is None:
            return None, None
        n = len(e.args or [])
        keys = []
        if s.params:
            p0 = s.params[0]
            pk = _PARAM_KIND.get(p0.kind) or (p0.simple if p0.kind == 'class' else
                                               ('char[]' if p0.kind == 'array' and p0.elem.kind == 'u16' else 'Object'))
            if p0.kind == 'typevar':
                pk = 'Object'
            keys.append('%s.%s(%s)' % (fam, s.name, pk))
        keys += ['%s.%s/%d' % (fam, s.name, n), '%s.%s' % (fam, s.name)]
        reg = registry()
        for k in keys:
            m = reg.resolve(k, 'java', 'csharp')
            if m is not None and m.name == k:
                rec = reg._sym.get(('java', k))
                return m, (rec['maps']['csharp'] if rec else {})
        return None, fam

    def call(self, e, statement=False):
        s = e.sym
        if s is None:
            return None
        c = self.prog.classes.get(s.owner)
        if c is not None and c.kind == 'enum' and s.static and s.name in ('values', 'valueOf'):
            self.w.usings.add('System')
            tn = self.w.ctype(ir.Type('class', c.qname))
            if s.name == 'values':
                return 'Enum.GetValues<%s>()' % tn, P_PRIMARY
            return 'Enum.Parse<%s>(%s)' % (tn, self.w.expr(e.args[0])[0]), P_PRIMARY
        if c is not None or not s.owner:
            return None
        m, raw = self._lookup(e)
        if m is None:
            if (s.name, len(e.args or [])) in _OBJECT_METHODS and e.target is not None:
                tt = e.target.type
                if tt is not None and tt.kind == 'class' and tt.name in self.prog.classes:
                    return None                                   # a class of ours: its own override
            if s.owner.startswith('java.') or s.owner.startswith('javax.'):
                raise Decline('library', 'the Java library call %s.%s/%d has no C# mapping' % (s.owner, s.name, len(e.args or [])))
            return None
        if m.kind == 'unsupported':
            raise Decline('library', '%s.%s: %s' % (s.owner, s.name, m.note or 'no faithful C# form'))
        if raw.get('ordered') and e.target is not None and self.concrete(e.target) not in _ORDERED:
            raise Decline('hash-order', 'iterating a %s: Java visits it in hash order, .NET in another'
                          % (self.concrete(e.target) or '?'))
        tmpl = raw.get('stmt') if statement and raw.get('stmt') else m.target
        if not tmpl:
            if raw.get('stmt'):
                raise Decline('library', '%s.%s used for its value' % (s.owner, s.name))
            return None
        if 'JavaRt' in tmpl or 'JavaRt' in (raw.get('target') or '') and '{s' in tmpl:
            self.prog.runtime.add('JavaRt')
        text = self.fill(tmpl, e)
        prec = {'eq': P_EQ, 'unary': P_UNARY}.get(raw.get('prec'), P_PRIMARY)
        return text, prec

    def stmt_call(self, e):
        got = self.call(e, statement=True)
        return got[0] if got is not None else None

    def fill(self, tmpl, e):
        import re
        w = self.w
        args = e.args or []

        def sub(mo):
            k = mo.group(1)
            if k == 't':
                return w.wrap(e.target, P_PRIMARY) if e.target is not None else 'this'
            if k == 'out':
                w.usings.add('System')
                t = e.target
                if isinstance(t, ir.Select) and t.sym is not None and t.sym.name == 'err':
                    return 'Console.Error'
                return 'Console'
            if k == '*':
                return ', '.join(w.expr(a)[0] for a in args)
            if k.startswith('s'):
                return self.string_of(args[int(k[1:])])
            if k.endswith('p'):
                return w.wrap(args[int(k[:-1])], P_PRIMARY)
            i = int(k)
            pt = e.sym.params[i] if i < len(e.sym.params) else None
            return w.coerce(args[i], pt) if pt is not None and pt.kind != 'typevar' else w.expr(args[i])[0]
        out = re.sub(r'\{(t|out|\*|s\d+|\d+p?)\}', sub, tmpl)
        if 'JavaRt' in out:
            self.prog.runtime.add('JavaRt')
        if 'StringComparison' in out or 'Convert.' in out or 'Math.' in out or 'Array.' in out \
                or 'Environment.' in out or 'Console' in out:
            w.usings.add('System')
        return out

    def string_of(self, a):
        """a as Java turns it into text (String.valueOf)."""
        t = a.type
        e = _strip(a)
        if isinstance(e, ir.Lit) and isinstance(e.value, str):
            return self.w.expr(a)[0]
        if t is not None and t.kind in ('i8', 'i16', 'i32', 'i64', 'u16') or isinstance(e, ir.Concat):
            return self.w.expr(a)[0]
        if t is not None and t.kind == 'array' and t.elem.kind == 'u16':
            return 'new string(' + self.w.expr(a)[0] + ')'
        if is_string(t) and _non_null(e):
            return self.w.expr(a)[0]
        self.prog.runtime.add('JavaRt')
        return 'JavaRt.Str(' + self.w.expr(a)[0] + ')'

    def concat_part(self, p):
        t = p.type
        e = _strip(p)
        if isinstance(e, ir.Lit) and isinstance(e.value, str):
            return None
        if t is not None and t.kind in ('i8', 'i16', 'i32', 'i64', 'u16'):
            return None
        if is_string(t) and _non_null(e):
            return None
        return self.string_of(p)

    def new(self, e):
        t = e.type
        q = _qual(t)
        if q is None or q in self.prog.classes:
            return None
        tn = self.w.ctype(t)
        args = e.args or []
        if q in _GENERIC:
            if len(args) == 1 and args[0].type is not None and args[0].type.is_prim:
                return 'new %s(%s)' % (tn, self.w.expr(args[0])[0]), P_PRIMARY       # a capacity
            return 'new %s(%s)' % (tn, ', '.join(self.w.expr(a)[0] for a in args)), P_PRIMARY
        if q in ('java.lang.StringBuilder', 'java.lang.StringBuffer'):
            self.w.usings.add('System.Text')
            if args and is_string(args[0].type):
                return 'new StringBuilder(%s)' % self.string_of(args[0]), P_PRIMARY
            return 'new StringBuilder(%s)' % ', '.join(self.w.expr(a)[0] for a in args), P_PRIMARY
        if q == 'java.util.Random':
            self.prog.runtime.add('JavaRt')
            return 'new JavaRandom(%s)' % ', '.join(self.w.expr(a)[0] for a in args), P_PRIMARY
        if q in _TYPES and tn.endswith('Exception'):
            if len(args) > 1 or (args and not is_string(args[0].type)):
                raise Decline('exception', 'an exception made with a cause')
            return 'new %s(%s)' % (tn, ', '.join(self.w.expr(a)[0] for a in args)), P_PRIMARY
        if q == 'java.lang.Object':
            return 'new object()', P_PRIMARY
        raise Decline('library', 'new %s: no C# mapping' % q)

    def binary(self, e):
        if e.op in ('==', '!=') and (is_string(e.left.type) and is_string(e.right.type)):
            l, r = _strip(e.left), _strip(e.right)
            if not (isinstance(l, ir.Lit) and l.value is None) and not (isinstance(r, ir.Lit) and r.value is None):
                text = '(object)' + self.w.wrap(e.left, P_UNARY) + ' ' + e.op + ' (object)' + self.w.wrap(e.right, P_UNARY)
                return text, P_EQ                               # Java compares the references
        return None

    def coerce(self, e, t):
        if t is not None and t.kind == 'class' and t.name in ('java.lang.Integer', 'java.lang.Long', 'java.lang.Double',
                                                              'java.lang.Float', 'java.lang.Boolean', 'java.lang.Character',
                                                              'java.lang.Short', 'java.lang.Byte'):
            x = _strip(e)
            if isinstance(x, ir.Lit) and x.value is None:
                raise Decline('boxed-null', 'a null %s: Java boxes it, the translation keeps it a value' % t.simple)
        return None

    # what a name holds: the class of the object made where it is declared (a Map that is a TreeMap)
    def record(self, sym, init):
        x = _strip(init) if init is not None else None
        if sym is not None and isinstance(x, ir.New) and x.type is not None and x.type.kind == 'class':
            self.prog.__dict__.setdefault('_concrete', {})[(sym.owner, sym.name)] = x.type.name

    def concrete(self, e):
        x = _strip(e)
        known = self.prog.__dict__.get('_concrete', {})
        if isinstance(x, (ir.Name, ir.Select)) and x.sym is not None and (x.sym.owner, x.sym.name) in known:
            return known[(x.sym.owner, x.sym.name)]
        return x.type.name if x.type is not None and x.type.kind == 'class' else None

    def arg(self, a, pt, text):
        return text

    def iterable(self, e):
        x = _strip(e)
        if isinstance(x, ir.Call):
            got = self.call(x)
            if got is not None:
                return got[0]
        t = e.type
        if t is not None and t.kind == 'class' and _FAMILY.get(t.name) in ('Set', 'Map') and self.concrete(e) not in _ORDERED:
            raise Decline('hash-order', 'iterating a %s: Java visits it in hash order, .NET in another' % self.concrete(e))
        return None

    def check_lambda(self, e):
        q = _qual(e.type)
        if q is not None and q not in _FUNCTIONS and q != 'java.lang.Runnable' and q not in self.prog.classes:
            raise Decline('lambda', 'a lambda of the interface %s (no C# delegate for it)' % q)
        if q is not None and q in self.prog.classes:
            raise Decline('lambda', 'a lambda of an interface declared in the program (C# lambdas make delegates only)')


def prepare(prog):
    """Before writing: an abstract Java class that leaves an interface's method to its subclasses declares
    it abstract in C# (C# requires the class to say so)."""
    if getattr(prog, '_java_prepared', False):
        return
    prog._java_prepared = True
    for c in list(prog.classes.values()):
        if c.kind != 'class' or 'abstract' not in (c.mods or []):
            continue
        have = set()
        k, seen = c, set()
        while k is not None and k.qname not in seen:
            seen.add(k.qname)
            have |= set((m.name, len(m.params or [])) for m in k.members if isinstance(m, ir.MethodDecl))
            k = prog.base_of(k)
        for i in _interfaces(prog, c):
            for m in i.members:
                if isinstance(m, ir.MethodDecl) and m.body is None and 'static' not in (m.mods or []) \
                        and (m.name, len(m.params or [])) not in have:
                    c.members.append(ir.MethodDecl(sym=m.sym, name=m.name, kind='method', params=m.params, ret=m.ret,
                                                   body=None, mods=['public', 'abstract'], throws=[], typeparams=m.typeparams,
                                                   line=m.line, col=m.col))
                    have.add((m.name, len(m.params or [])))


def _interfaces(prog, c):
    out, todo, seen = [], list(c.implements or []), set()
    while todo:
        t = todo.pop()
        i = prog.classes.get(t.name) if t is not None and t.kind == 'class' else None
        if i is None or i.qname in seen:
            continue
        seen.add(i.qname)
        out.append(i)
        todo += list(i.implements or []) + ([i.extends] if i.extends is not None else [])
    return out


def _non_null(e):
    """Is this string expression never null? (a literal, a concatenation, a library call that gives text)"""
    if isinstance(e, (ir.Concat,)) or (isinstance(e, ir.Lit) and isinstance(e.value, str)):
        return True
    if isinstance(e, ir.Call) and e.sym is not None and e.sym.owner in ('java.lang.String', 'java.lang.StringBuilder',
                                                                         'java.lang.Integer', 'java.lang.Long',
                                                                         'java.lang.Double', 'java.lang.Float'):
        return e.sym.name in ('toString', 'substring', 'trim', 'toUpperCase', 'toLowerCase', 'replace', 'valueOf',
                              'concat', 'repeat', 'join', 'strip', 'intern')
    return False
