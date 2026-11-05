"""lower.py -- clang's typed AST (-ast-dump=json) into the translator's IR (codebreaker/translate/ir.py).

What clang resolved is what the IR says: every implicit conversion is an explicit Cast (marked
implicit), every name is the declaration clang bound it to, overloads and template instantiations are
the ones clang chose. Only the declarations of the program's own files are lowered (not those of the
headers it includes); a call into a library keeps the header that declared it as the symbol's owner.

C's model in the IR:
  * a translation unit is one Unit: its structs/unions/classes (ClassDecl, with their byte layout for
    C/C++ records), its functions (MethodDecl kind "function"), its globals (FieldDecl, sym kind
    "global"); C enum constants are int globals (mods const), a C++ `enum class` a ClassDecl kind "enum";
  * pointers are 'ptr' types; `*p` and `&x` are Unary "*" and "&"; `p->f` is Select on `*p`;
  * a C++ reference is a 'ptr' with ref: its reads are Unary "*", a binding Unary "&";
  * `this` is a pointer (This typed ptr to the class), so `this->x` is Select on `*this`;
  * a string literal's value is its bytes, one character per byte (latin-1); its type char[N];
  * sizeof/alignof are Lit values by the target's layout, `text` keeping the spelling;
  * an aggregate initialiser is NewArray (arrays) or New with memory "stack" and the field values in
    order (structs); missing trailing elements are zero, as in C;
  * a static local is a Local with mods ["static"].
Anything not handled becomes ir.Unsupported(kind), never a guess.
"""
import os

from codebreaker.translate import ir
from codebreaker.translate.frontends.clang import ctypes

_LOC_KEYS = ('spellingLoc', 'expansionLoc')
_OPS = {'+', '-', '*', '/', '%', '<<', '>>', '&', '|', '^', '&&', '||', '==', '!=', '<', '<=', '>', '>=', ','}
_DROP_CASTS = frozenset(('LValueToRValue', 'NoOp', 'FunctionToPointerDecay', 'LValueToRValueBitCast',
                         'AtomicToNonAtomic', 'NonAtomicToAtomic'))
_WRAPPERS = frozenset(('ExprWithCleanups', 'MaterializeTemporaryExpr', 'CXXBindTemporaryExpr', 'FullExpr',
                       'SubstNonTypeTemplateParmExpr', 'CXXDefaultInitExpr', 'ConstantExpr'))
_CASTS = frozenset(('CStyleCastExpr', 'CXXStaticCastExpr', 'CXXReinterpretCastExpr', 'CXXConstCastExpr',
                    'CXXFunctionalCastExpr', 'CXXDynamicCastExpr', 'BuiltinBitCastExpr'))


class Refused(Exception):
    """The unit holds something the front end does not lower at all."""


def _esc_value(s):
    """A C string literal as clang spells it ("\"a\\n\"", u8"..", L"..") -> its bytes, latin-1."""
    prefix = s[:s.index('"')]
    if prefix in ('U',):
        raise TypeError('UTF-32 string literal')
    body = s[s.index('"') + 1:s.rindex('"')]
    out = bytearray()
    i = 0
    if prefix in ('L', 'u'):                      # a wide literal: its UTF-16 code units, one character each
        return _wide_value(body)
    raw = body.encode('utf-8', 'surrogateescape')
    while i < len(raw):
        c = raw[i]
        if c != 0x5c:
            out.append(c)
            i += 1
            continue
        i += 1
        e = chr(raw[i])
        simple = {'n': 10, 't': 9, 'r': 13, '0': 0, 'a': 7, 'b': 8, 'f': 12, 'v': 11, '\\': 92, "'": 39, '"': 34, '?': 63,
                  'e': 27}
        if e == 'x':
            j = i + 1
            while j < len(raw) and chr(raw[j]) in '0123456789abcdefABCDEF':
                j += 1
            out.append(int(raw[i + 1:j], 16) & 0xFF)
            i = j
        elif e in '01234567':
            j = i
            while j < len(raw) and j < i + 3 and chr(raw[j]) in '01234567':
                j += 1
            out.append(int(raw[i:j], 8) & 0xFF)
            i = j
        elif e in simple:
            out.append(simple[e])
            i += 1
        else:
            out.append(raw[i])
            i += 1
    return out.decode('latin-1')


def _wide_value(body):
    simple = {'n': '\n', 't': '\t', 'r': '\r', '0': '\0', '\\': '\\', "'": "'", '"': '"', 'a': '\a', 'b': '\b',
              'f': '\f', 'v': '\v', '?': '?'}
    out, i = [], 0
    while i < len(body):
        c = body[i]
        if c != '\\':
            out.append(c)
            i += 1
            continue
        e = body[i + 1]
        if e in 'xuU':
            j = i + 2
            while j < len(body) and body[j] in '0123456789abcdefABCDEF' and (e == 'x' or j < i + 2 + (4 if e == 'u' else 8)):
                j += 1
            out.append(chr(int(body[i + 2:j], 16) & 0xFFFF))
            i = j
        elif e in '01234567':
            j = i + 1
            while j < len(body) and j < i + 4 and body[j] in '01234567':
                j += 1
            out.append(chr(int(body[i + 1:j], 8)))
            i = j
        else:
            out.append(simple.get(e, e))
            i += 2
    return ''.join(out)


def _char_text(v):
    if 32 <= v < 127 and chr(v) not in "'\\":
        return "'%s'" % chr(v)
    return {10: "'\\n'", 9: "'\\t'", 13: "'\\r'", 0: "'\\0'", 39: "'\\''", 92: "'\\\\'"}.get(v)


def spell(t):
    """A canonical spelling of a type: the key template instances and records are found by."""
    if t is None:
        return '?'
    k = t.kind
    if k == 'class':
        return t.name + ('<%s>' % ', '.join(spell(a) for a in t.args) if t.args else '')
    if k == 'ptr':
        return spell(t.elem) + ('&' if t.info.get('ref') else '*')
    if k == 'array':
        return '%s[%s]' % (spell(t.elem), t.info.get('count', ''))
    if k == 'fnptr':
        return spell(t.info['fn']) + '*'
    if k == 'fn':
        return '%s(%s)' % (spell(t.info['result'].get('type')), ','.join(spell(p['type']) for p in t.info['params']))
    if k == 'char':
        return 'char'
    return k


class Lowering(object):
    def __init__(self, tree, main_path, own=None, language='c', std=None, triple='x86_64-pc-windows-msvc'):
        self.tree = tree
        self.main = os.path.normcase(os.path.abspath(main_path))
        self.own = set(os.path.normcase(os.path.abspath(p)) for p in (own or []))
        self.own.add(self.main)
        self.auto_own = own is None
        self.files = set()
        self.own_dirs = set()
        self.cpp = language in ('c++', 'cpp', 'cxx')
        self.language = 'c++' if self.cpp else 'c'
        self.std = std
        self.model = ctypes.Model.for_triple(triple)
        self.unit_name = os.path.splitext(os.path.basename(main_path))[0]
        self.decls = {}            # id -> decl node
        self.parent = {}           # id -> the enclosing decl node (record, function, namespace)
        self.qnames = {}           # record id -> qualified key
        self.records = {}          # key -> record node (the complete definition)
        self.typedefs = {}         # name -> ('record', id) | ('type', text)
        self.enum_values = {}      # EnumConstantDecl id -> int
        self.scoped_enums = {}     # EnumDecl id -> qualified name (C++ enum class)
        self.labels = {}           # LabelDecl id -> name
        self.layout_cache = {}
        self.sigs = {}             # id(MethodDecl) -> (name, type text): an out-of-line body finds its declaration
        self.types = ctypes.Parser(self.model, self._name)
        self.sizes = ctypes.Layouts(self.model, self._layout_of)
        self.notes = []
        self._index()

    # ------------------------------------------------------------------------------------- indexing
    def _index(self):
        """One pre-order walk: each node's file/line/column (clang writes them as deltas), and every
        declaration by id, with its parent and qualified name."""
        state = {'file': None, 'line': 0}

        def upd(loc):
            if not loc:
                return None
            if any(k in loc for k in _LOC_KEYS):
                got = None
                for k in _LOC_KEYS:
                    if k in loc:
                        got = upd(loc[k]) or got
                return got
            if 'file' in loc:
                state['file'] = loc['file']
                self.files.add(loc['file'])
            if 'line' in loc:
                state['line'] = loc['line']
            return (state['file'], state['line'], loc.get('col', 0))

        stack = [(self.tree, None, ())]
        pending = []
        while stack:
            n, parent, ns = stack.pop()
            if not isinstance(n, dict) or not n:              # ({}: an absent child, as in a for's parts)
                continue
            here = upd(n.get('loc'))
            r = n.get('range') or {}
            rb = upd(r.get('begin'))
            upd(r.get('end'))
            if rb:
                n['_rb'] = rb
            if here:
                n['_f'], n['_l'], n['_c'] = here
            elif parent is not None and '_f' in parent:
                n['_f'], n['_l'], n['_c'] = parent['_f'], parent.get('_l', 0), parent.get('_c', 0)
            kind = n.get('kind', '')
            child_ns = ns
            if kind == 'LabelStmt' and n.get('declId'):
                self.labels[n['declId']] = n.get('name')
            nid = n.get('id')
            if nid and kind.endswith('Decl') or kind == 'ClassTemplateSpecializationDecl':
                self.decls[nid] = n
                if parent is not None and parent.get('kind', '').endswith('Decl'):
                    self.parent[nid] = parent
                if kind == 'NamespaceDecl':
                    child_ns = ns + (n.get('name') or '(anonymous)',)
                elif kind in ('RecordDecl', 'CXXRecordDecl', 'ClassTemplateSpecializationDecl'):
                    if parent is not None and parent.get('kind') in ('CXXRecordDecl', 'ClassTemplateSpecializationDecl') \
                            and n.get('name') == parent.get('name') and not n.get('completeDefinition'):
                        pass                                       # the injected class name
                    elif kind == 'ClassTemplateSpecializationDecl':
                        # (its key spells its arguments, which may name records indexed later: after the walk)
                        pending.append((n, ns))
                        child_ns = ns + (n.get('name'),)
                    else:
                        key = self._record_key(n, ns)
                        if key is not None:
                            self.qnames[nid] = key
                            if n.get('completeDefinition') or key not in self.records:
                                self.records[key] = n
                            if not n.get('name') and n.get('_rb'):     # (clang names it by where it begins too)
                                alias = '.'.join(ns + ('__unnamed_' + ctypes.anon_key(n['_rb'][1], n['_rb'][2]),))
                                self.records.setdefault(alias, n)
                            child_ns = ns + (key.rsplit('.', 1)[-1],) if key else ns
                elif kind == 'TypedefDecl' or kind == 'TypeAliasDecl':
                    inner = (n.get('inner') or [{}])[0]
                    while inner.get('kind') == 'ElaboratedType' and inner.get('inner'):
                        inner = inner['inner'][0]
                    name = '.'.join(ns + (n['name'],))
                    if inner.get('kind') in ('RecordType', 'EnumType') and inner.get('decl'):
                        self.typedefs[name] = ('record' if inner['kind'] == 'RecordType' else 'enum', inner['decl']['id'])
                    else:
                        self.typedefs[name] = ('type', n['type'].get('qualType'))
                elif kind == 'EnumDecl':
                    val = -1
                    for c in n.get('inner', []):
                        if c.get('kind') == 'EnumConstantDecl':
                            init = (c.get('inner') or [None])[0]
                            val = self._enum_const(init) if init else val + 1
                            self.enum_values[c['id']] = val
                    if n.get('scopedEnumTag'):
                        self.scoped_enums[nid] = '.'.join(ns + (n['name'],))
                elif kind == 'LabelDecl':
                    self.labels[nid] = n.get('name')
            for c in reversed((n.get('inner', []) or []) + (n.get('array_filler') or [])):
                if isinstance(c, dict):
                    stack.append((c, n, child_ns))
            # template arguments of a specialisation sit in inner too: nothing else to walk
        for _pass in (1, 2):                          # (twice: an argument may be another instance)
            self.types.cache.clear()
            for n, ns in pending:
                key = self._record_key(n, ns)
                self.qnames[n['id']] = key
                if n.get('completeDefinition') or key not in self.records:
                    self.records[key] = n
        self.types.cache.clear()
        if self.auto_own:                       # the program's own headers: those beside its source
            here = os.path.dirname(self.main)
            for f in self.files:
                p = os.path.normcase(os.path.abspath(f))
                if os.path.dirname(p) == here:
                    self.own.add(p)
        self.own_dirs = set(os.path.dirname(p) for p in self.own)

    def _record_key(self, n, ns):
        name = n.get('name')
        if not name:
            loc = n
            return '.'.join(ns + ('__unnamed_' + ctypes.anon_key(loc.get('_l', 0), loc.get('_c', 0)),))
        key = '.'.join(ns + (name,))
        if n.get('kind') == 'ClassTemplateSpecializationDecl':
            args = []
            for a in n.get('inner', []):
                if a.get('kind') == 'TemplateArgument':
                    if 'type' in a:
                        try:
                            args.append(self.types.parse(a['type']['qualType']))
                        except (TypeError, KeyError, ValueError, IndexError):
                            args.append(ir.Type('class', a['type']['qualType']))
                    elif 'value' in a:
                        args.append(ir.Type('class', str(a['value'])))
            key += '<%s>' % ', '.join(spell(x) for x in args)
        return key

    def _enum_const(self, e):
        v = self.const(e)
        return v if v is not None else 0

    # ---------------------------------------------------------------------------------------- types
    def _name(self, name, args, tag):
        """The parser's callback: a typedef / record / class / enum name -> ir.Type."""
        dotted = name.replace('::', '.')
        if dotted == '__builtin_va_list':                       # MSVC's va_list: a char pointer
            return ir.Type('ptr', elem=ir.Type('char', signed=self.model.char_signed), space='flat')
        if dotted.startswith('std.') and dotted not in self.records:
            return ir.Type('class', dotted, args)               # a library class: named, not laid out
        key = dotted + ('<%s>' % ', '.join(spell(a) for a in args) if args else '')
        rec = self._find_record(key)
        if rec is not None and tag != 'enum':
            return self._record_type(rec)
        td = self._find_typedef(dotted)
        if td is not None:
            k, v = td
            if k == 'record':
                n = self.decls.get(v)
                return self._record_type(n) if n is not None else None
            if k == 'enum':
                return self._enum_type(v)
            return self.types.parse(v)
        for nid, qn in self.scoped_enums.items():
            if qn == dotted or qn.endswith('.' + dotted):
                return ir.I32                   # (an enum class is its int: its constants stay named)
        if tag == 'enum' or self._find_enum(dotted):
            return ir.I32
        if dotted.startswith('__unnamed_'):
            return None
        return None

    def _find_record(self, key):
        if key in self.records:
            return self.records[key]
        memo = self.__dict__.setdefault('_tail_memo', {})
        if key in memo and memo[key][0] == len(self.records):
            return memo[key][1]
        tail = '.' + key
        hits = sorted((k for k in self.records if k.endswith(tail)), key=len)
        got = self.records[hits[0]] if hits else None
        memo[key] = (len(self.records), got)
        return got

    def _find_typedef(self, name):
        if name in self.typedefs:
            return self.typedefs[name]
        hits = [k for k in self.typedefs if k.endswith('.' + name)]
        return self.typedefs[hits[0]] if hits else None

    def _find_enum(self, name):
        names = self.__dict__.get('_enum_names')
        if names is None:
            names = self._enum_names = set(n.get('name') for n in self.decls.values() if n.get('kind') == 'EnumDecl')
        return name.rsplit('.', 1)[-1] in names

    def _enum_type(self, eid):
        if eid in self.scoped_enums:
            return ir.I32
        return ir.I32

    def _record_type(self, n):
        key = self.qnames.get(n['id'])
        if key is None:
            return None
        full = self.records.get(key, n)
        if full.get('kind') == 'ClassTemplateSpecializationDecl':
            base, _, rest = key.partition('<')
            args = []
            for a in full.get('inner', []):
                if a.get('kind') == 'TemplateArgument':
                    try:
                        args.append(self.types.parse(a['type']['qualType']) if 'type' in a
                                    else ir.Type('class', str(a.get('value'))))
                    except (TypeError, KeyError, ValueError, IndexError):
                        args.append(ir.Type('class', a['type']['qualType']))
            return ir.Type('class', base, args)
        return ir.Type('class', key)

    def ty(self, n, key='type'):
        t = n.get(key) or {}
        text = t.get('desugaredQualType') if 'typeAliasDeclId' in t and False else t.get('qualType')
        if text is None:
            return ir.ANY
        try:
            return ctypes.normalise(self.types.parse(text))
        except (TypeError, KeyError, ValueError, IndexError):
            try:
                if t.get('desugaredQualType'):
                    return ctypes.normalise(self.types.parse(t['desugaredQualType']))
            except (TypeError, KeyError, ValueError, IndexError):
                pass
            return ir.Type('class', '<unread:%s>' % text)

    def _layout_of(self, t):
        """sizeof's view of a class type: its record, or an int's for a C++ enum class."""
        name = spell(t)
        if any(qn == name or qn.endswith('.' + name) for qn in self.scoped_enums.values()):
            return ir.Type('record', name, size=4, fields=[{'name': 'value', 'offset': 0, 'type': ir.I32}], role='struct')
        return self._record_layout(t)

    def _record_layout(self, t):
        """The 'record' Type of a C/C++ struct type (for sizeof and ClassDecl.layout)."""
        key = spell(t)
        if key in self.layout_cache:
            return self.layout_cache[key]
        rec = self._find_record(key)
        if rec is None:
            return None
        self.layout_cache[key] = None                      # (a cycle through a value field cannot be)
        fields, union = [], rec.get('tagUsed') == 'union'
        for c in rec.get('inner', []):
            if c.get('kind') == 'FieldDecl':
                if c.get('isBitfield'):
                    return None
                fields.append((c.get('name') or '', self.ty(c)))
            elif c.get('kind') in ('CXXRecordDecl', 'RecordDecl') and c.get('completeDefinition') and \
                    not c.get('name') and False:
                pass
        if self._polymorphic(rec) or self._bases(rec):
            return None
        try:
            r, _a = self.sizes.lay_out(key, fields, union)
        except TypeError:
            return None                                  # (a field whose size is not known here: no layout)
        self.layout_cache[key] = r
        return r

    def _polymorphic(self, rec):
        dd = rec.get('definitionData') or {}
        return bool(dd.get('isPolymorphic') or dd.get('isAbstract'))

    def _bases(self, rec):
        return rec.get('bases') or []

    # ---------------------------------------------------------------------------------------- helpers
    def pos(self, n):
        return dict(line=n.get('_l', 0) if n.get('_f') and self._is_own(n.get('_f')) else 0, col=n.get('_c', 0))

    def _is_own(self, f):
        if f is None:
            return False
        p = os.path.normcase(os.path.abspath(f))
        return p in self.own

    def unsupported(self, kind, n, raw=None):
        return ir.Unsupported(kind=kind, raw=raw, type=self.ty(n) if n is not None else ir.ANY,
                              **(self.pos(n) if n is not None else {}))

    def const(self, n):
        """A C integer constant expression's value, or None."""
        if n is None:
            return None
        k = n.get('kind')
        if 'value' in n and k in ('ConstantExpr', 'IntegerLiteral'):
            try:
                return int(n['value'])
            except ValueError:
                pass
        if k == 'CharacterLiteral':
            return int(n['value'])
        if k == 'CXXBoolLiteralExpr':
            return int(bool(n['value']))
        if k in ('ConstantExpr', 'ParenExpr', 'ImplicitCastExpr', 'CStyleCastExpr', 'CXXStaticCastExpr',
                 'CXXFunctionalCastExpr', 'SubstNonTypeTemplateParmExpr'):
            v = self.const(n['inner'][-1]) if n.get('inner') else None
            if v is not None and k != 'ParenExpr' and k != 'ConstantExpr':
                t = self.ty(n)
                v = _wrap(v, t)
            return v
        if k == 'DeclRefExpr':
            d = n.get('referencedDecl') or {}
            if d.get('kind') == 'EnumConstantDecl':
                return self.enum_values.get(d['id'])
            dn = self.decls.get(d.get('id'))
            if dn is not None and dn.get('kind') == 'VarDecl' and dn.get('constexpr') and dn.get('inner'):
                return self.const(dn['inner'][0])
            return None
        if k == 'UnaryOperator':
            v = self.const(n['inner'][0])
            if v is None:
                return None
            op = n['opcode']
            return {'-': -v, '+': v, '~': ~v, '!': int(not v)}.get(op)
        if k == 'BinaryOperator':
            a, b = self.const(n['inner'][0]), self.const(n['inner'][1])
            if a is None or b is None:
                return None
            op = n['opcode']
            try:
                v = {'+': a + b, '-': a - b, '*': a * b, '<<': a << b, '>>': a >> b, '&': a & b, '|': a | b,
                     '^': a ^ b, '==': int(a == b), '!=': int(a != b), '<': int(a < b), '>': int(a > b),
                     '<=': int(a <= b), '>=': int(a >= b), '&&': int(bool(a and b)), '||': int(bool(a or b))}.get(op)
                if op == '/':
                    v = int(a / b)
                elif op == '%':
                    v = a - int(a / b) * b
            except (ZeroDivisionError, ValueError):
                return None
            return _wrap(v, self.ty(n)) if v is not None else None
        if k == 'UnaryExprOrTypeTraitExpr':
            lit = self.sizeof(n)
            return lit.value if isinstance(lit, ir.Lit) else None
        if k == 'ConditionalOperator':
            c = self.const(n['inner'][0])
            return None if c is None else self.const(n['inner'][1 if c else 2])
        return None

    # ---------------------------------------------------------------------------------------- the unit
    def unit(self):
        self.out = []
        self.classes = {}          # record key -> ClassDecl
        self.funcs = {}            # function id -> MethodDecl
        self.methods = {}          # method id -> MethodDecl (inside a ClassDecl)
        self._decls(self.tree.get('inner', []), top=True)
        target = self.model.json(self.language, self.std or '')
        return ir.Unit(path=self.main, package='', imports=[], types=self.out, target=target)

    def _decls(self, nodes, top=False):
        for n in nodes:
            k = n.get('kind')
            if k in ('NamespaceDecl', 'LinkageSpecDecl', 'ExternCContextDecl'):
                self._decls(n.get('inner', []))
                continue
            if not self._is_own(n.get('_f')) or n.get('isImplicit'):
                continue
            try:
                self._decl(n)
            except Refused:
                raise
            except (TypeError, KeyError, ValueError, IndexError) as e:
                self.out.append(ir.MethodDecl(sym=None, name=n.get('name'), kind='function', params=[], ret=ir.VOID,
                                              body=ir.Block(stmts=[ir.ExprStmt(expr=self.unsupported('decl:' + k, n, str(e)))]),
                                              mods=['unsupported'], throws=[], typeparams=[], **self.pos(n)))

    def _decl(self, n):
        k = n.get('kind')
        if k in ('RecordDecl', 'CXXRecordDecl'):
            if n.get('completeDefinition'):
                self.record(n)
        elif k == 'ClassTemplateDecl':
            for c in n.get('inner', []):
                if c.get('kind') == 'ClassTemplateSpecializationDecl' and c.get('completeDefinition'):
                    self.record(self.records.get(self.qnames.get(c['id']), c))
        elif k == 'ClassTemplateSpecializationDecl':
            if n.get('completeDefinition'):
                self.record(n)
        elif k == 'FunctionTemplateDecl':
            for c in n.get('inner', []):
                if c.get('kind') in ('FunctionDecl', 'CXXMethodDecl') and c.get('TemplateInstantiationPattern'):
                    self._decl(c)
        elif k == 'FunctionDecl':
            if self._body(n) is not None:
                m = self.function(n)
                self.out.append(m)
        elif k in ('CXXMethodDecl', 'CXXConstructorDecl', 'CXXDestructorDecl', 'CXXConversionDecl'):
            if self._body(n) is not None:                       # a member defined outside its class
                owner = self.decls.get(n.get('parentDeclContextId')) or self.parent.get(n['id'])
                m = self.method(n, owner)
                cls = self.classes.get(self.qnames.get((owner or {}).get('id')))
                prev = self._find_method(cls, n) if cls is not None else None
                if prev is not None:
                    # (static, virtual, override are said in the class, not at the definition outside it)
                    m.mods = list(prev.mods or []) + [x for x in m.mods or [] if x not in (prev.mods or [])]
                    if 'static' in m.mods and m.sym is not None:
                        m.sym.static = True
                    cls.members[cls.members.index(prev)] = m
                elif cls is not None:
                    cls.members.append(m)
                else:
                    self.out.append(m)
        elif k == 'VarDecl':
            if n.get('storageClass') == 'extern' and not n.get('inner'):
                return
            owner = self._member_owner(n)
            if owner is not None:                          # `int Shape::count = 0;`: the class's static member
                cls = self.classes.get(self.qnames.get(owner['id']))
                init = [c for c in n.get('inner', []) if not c.get('kind', '').endswith('Attr')]
                for m in (cls.members if cls is not None else []):
                    if isinstance(m, ir.FieldDecl) and m.sym.name == n['name'] and init:
                        m.init = self.expr(init[-1])
                return
            self.out.append(self.global_(n))
        elif k == 'EnumDecl':
            self.enum(n)
        elif k in ('TypedefDecl', 'TypeAliasDecl', 'EmptyDecl', 'StaticAssertDecl', 'UsingDirectiveDecl',
                   'UsingDecl', 'FunctionDecl', 'TypeAliasTemplateDecl', 'VarTemplateDecl', 'UsingShadowDecl'):
            pass
        else:
            self.out.append(ir.MethodDecl(sym=None, name=n.get('name'), kind='function', params=[], ret=ir.VOID,
                                          body=ir.Block(stmts=[ir.ExprStmt(expr=self.unsupported('decl:' + k, n))]),
                                          mods=['unsupported'], throws=[], typeparams=[], **self.pos(n)))

    def _lib_owner(self, node):
        """A library symbol's owner: its C++ namespace ('std', 'System') where it has one, else the header
        that declares it ('stdio.h')."""
        ns, cur = [], self.parent.get(node.get('id'))
        while cur is not None:
            if cur.get('kind') == 'NamespaceDecl':
                ns.append(cur.get('name') or '(anonymous)')
            cur = self.parent.get(cur.get('id'))
        if ns:
            return '.'.join(reversed(ns))
        f = node.get('_f')
        return os.path.basename(f) if f else None

    def _member_owner(self, n):
        """The record a VarDecl is a static data member of (in its class or defined outside it), or None."""
        par = self.parent.get(n.get('id'))
        if par is not None and par.get('kind') in ('CXXRecordDecl', 'ClassTemplateSpecializationDecl'):
            return par
        pd = self.decls.get(n.get('parentDeclContextId'))
        if pd is not None and pd.get('kind') in ('CXXRecordDecl', 'ClassTemplateSpecializationDecl'):
            return pd
        return None

    def _find_method(self, cls, n):
        sig = (n.get('name'), (n.get('type') or {}).get('qualType'))
        for m in cls.members:
            if isinstance(m, ir.MethodDecl) and self.sigs.get(id(m)) == sig:
                return m
        return None

    def _body(self, n):
        for c in n.get('inner', []):
            if c.get('kind') in ('CompoundStmt', 'CXXTryStmt'):
                return c
        return None

    # ---- records
    def record(self, n):
        key = self.qnames.get(n['id'])
        if key is None or key in self.classes:
            return self.classes.get(key)
        t = self._record_type(n)
        tag = n.get('tagUsed', 'struct')
        kind = 'union' if tag == 'union' else ('class' if tag == 'class' else 'struct')
        bases = [self.types.parse(b['type']['qualType']) for b in self._bases(n)]
        mods = []
        if self._polymorphic(n):
            mods.append('polymorphic')
        if (n.get('definitionData') or {}).get('isAbstract'):
            mods.append('abstract')
        c = ir.ClassDecl(name=key.rsplit('.', 1)[-1].split('<')[0] if '<' not in key.rsplit('.', 1)[-1] else
                         key.rsplit('.', 1)[-1].split('<')[0],
                         qname=spell(t), kind=kind, mods=mods, typeparams=[],
                         extends=bases[0] if bases else None, implements=bases[1:], members=[], outer=None,
                         layout=self._record_layout(t), **self.pos(n))
        self.classes[key] = c
        self.out.append(c)
        access = 'private' if tag == 'class' else 'public'
        for m in n.get('inner', []):
            mk = m.get('kind')
            if mk == 'AccessSpecDecl':
                access = m.get('access', access)
                continue
            if m.get('isImplicit'):
                continue
            if mk == 'FieldDecl':
                ft = self.ty(m)
                fmods = [access]
                if m.get('isBitfield'):
                    fmods.append('bitfield')
                init = None
                for x in m.get('inner', []):
                    if x.get('kind') not in ('ConstantExpr',) or not m.get('isBitfield'):
                        if not x.get('kind', '').endswith('Attr'):
                            init = self.expr(x)
                c.members.append(ir.FieldDecl(sym=ir.Sym('field', m.get('name') or '', c.qname, False, ft), type=ft,
                                              init=init, mods=fmods, **self.pos(m)))
            elif mk == 'VarDecl':                                 # a static data member
                ft = self.ty(m)
                init = self.expr(m['inner'][0]) if m.get('inner') and not m['inner'][0].get('kind', '').endswith('Attr') else None
                c.members.append(ir.FieldDecl(sym=ir.Sym('field', m['name'], c.qname, True, ft), type=ft, init=init,
                                              mods=[access, 'static'] + (['const'] if 'const' in m['type']['qualType'] else []),
                                              **self.pos(m)))
            elif mk in ('CXXMethodDecl', 'CXXConstructorDecl', 'CXXDestructorDecl', 'CXXConversionDecl'):
                if m.get('explicitlyDefaulted') or m.get('explicitlyDeleted'):
                    continue
                md = self.method(m, n, access)
                c.members.append(md)
            elif mk in ('CXXRecordDecl', 'RecordDecl') and m.get('completeDefinition'):
                inner = self.record(m)
                if inner is not None:
                    inner.outer = c.qname
            elif mk == 'EnumDecl':
                self.enum(m)
            elif mk == 'FunctionTemplateDecl':
                for x in m.get('inner', []):
                    if x.get('kind') in ('CXXMethodDecl',) and x.get('TemplateInstantiationPattern'):
                        c.members.append(self.method(x, n, access))
            elif mk == 'FriendDecl' or mk.endswith('Attr') or mk in ('TemplateArgument', 'TypedefDecl', 'TypeAliasDecl',
                                                                     'StaticAssertDecl', 'UsingDecl'):
                continue
            else:
                c.members.append(ir.FieldDecl(sym=ir.Sym('field', '<%s>' % mk, c.qname), type=ir.ANY,
                                              init=self.unsupported('member:' + mk, m), mods=['unsupported']))
        return c

    def enum(self, n):
        consts = [c for c in n.get('inner', []) if c.get('kind') == 'EnumConstantDecl']
        if n['id'] in self.scoped_enums:
            qn = self.scoped_enums[n['id']]
            c = ir.ClassDecl(name=n['name'], qname=qn, kind='enum', mods=['scoped'], typeparams=[], extends=None,
                             implements=[], members=[], outer=None, **self.pos(n))
            for e in consts:
                c.members.append(ir.FieldDecl(sym=ir.Sym('field', e['name'], qn, True, ir.I32), type=ir.I32,
                                              init=ir.Lit(value=self.enum_values[e['id']], type=ir.I32),
                                              mods=['public', 'static', 'const'], **self.pos(e)))
            self.out.append(c)
            return
        for e in consts:                                     # a C enum's constants are ints in the global scope
            self.out.append(ir.FieldDecl(sym=ir.Sym('global', e['name'], self.unit_name, True, ir.I32), type=ir.I32,
                                         init=ir.Lit(value=self.enum_values[e['id']], type=ir.I32),
                                         mods=['static', 'const', 'enum:' + (n.get('name') or '')], **self.pos(e)))

    def global_(self, n):
        t = self.ty(n)
        init = None
        for c in n.get('inner', []):
            if not c.get('kind', '').endswith('Attr'):
                init = self.expr(c)
        t = self._complete_array(t, init)
        mods = ['static'] + (['internal'] if n.get('storageClass') == 'static' else []) + \
            (['const'] if (n.get('type') or {}).get('qualType', '').startswith('const ') else [])
        return ir.FieldDecl(sym=ir.Sym('global', n['name'], self.unit_name, True, t), type=t, init=init, mods=mods,
                            **self.pos(n))

    def _complete_array(self, t, init):
        if t.kind == 'array' and 'count' not in t.info and init is not None and init.type is not None \
                and init.type.kind == 'array' and 'count' in init.type.info:
            return init.type
        return t

    # ---- functions
    def _sym_of_function(self, d, owner=None, static=True):
        """A FunctionDecl / CXXMethodDecl node (or a referencedDecl stub) -> ir.Sym."""
        node = self.decls.get(d.get('id'), d)
        ft = self.ty(node)
        params, ret, var = [], ir.VOID, False
        if ft.kind == 'fn':
            params = [p['type'] for p in ft.info['params']]
            ret = ft.info['result'].get('type') or ir.VOID
            var = bool(ft.info.get('variadic'))
        k = node.get('kind', d.get('kind'))
        if k in ('CXXMethodDecl', 'CXXConversionDecl', 'CXXDestructorDecl', 'CXXConstructorDecl'):
            par = self.decls.get(node.get('parentDeclContextId')) or self.parent.get(node.get('id'))
            qn = spell(self._record_type(par)) if par is not None and self._record_type(par) is not None else owner
            kind = 'ctor' if k == 'CXXConstructorDecl' else 'method'
            return ir.Sym(kind, node.get('name') or d.get('name'), qn, bool(node.get('storageClass') == 'static'),
                          params=params, ret=ret, varargs=var)
        f = node.get('_f')
        if self._is_own(f):
            own = self.unit_name
        else:
            own = self._lib_owner(node)
        return ir.Sym('function', node.get('name') or d.get('name'), own, True, params=params, ret=ret, varargs=var)

    def _params(self, n, scope):
        out = []
        for p in n.get('inner', []):
            if p.get('kind') == 'ParmVarDecl':
                t = self.ty(p)
                name = self._local_name(p, scope) if p.get('name') else scope.fresh('arg')
                s = ir.Sym('param', name, scope.owner, False, t)
                scope.syms[p['id']] = s
                out.append(ir.Param(sym=s, type=t, mods=[], **self.pos(p)))
        return out

    def _local_name(self, d, scope):
        name = d.get('name') or 'tmp'
        if name in scope.used:
            i = 2
            while '%s_%d' % (name, i) in scope.used:
                i += 1
            name = '%s_%d' % (name, i)
        scope.used.add(name)
        return name

    def function(self, n):
        scope = _Scope(n.get('name'))
        s = self._sym_of_function(n)
        params = self._params(n, scope)
        self.scope = scope
        body = self.stmt(self._body(n))
        mods = []
        if n.get('storageClass') == 'static':
            mods.append('internal')
        if n.get('inline'):
            mods.append('inline')
        m = ir.MethodDecl(sym=s, name=n['name'], kind='function', params=params, ret=s.ret, body=body, mods=mods,
                          throws=[], typeparams=[], **self.pos(n))
        return m

    def method(self, n, owner, access='public'):
        scope = _Scope(n.get('name'))
        s = self._sym_of_function(n)
        params = self._params(n, scope)
        self.scope = scope
        k = n['kind']
        kind = {'CXXConstructorDecl': 'ctor', 'CXXDestructorDecl': 'dtor'}.get(k, 'method')
        mods = [access]
        for flag in ('virtual', 'pure', 'inline'):
            if n.get(flag):
                mods.append(flag)
        if n.get('storageClass') == 'static':
            mods.append('static')
        qt = (n.get('type') or {}).get('qualType', '')
        if qt.endswith(' const') or ') const' in qt:
            mods.append('const')
        if any(c.get('kind') == 'OverrideAttr' for c in n.get('inner', [])):
            mods.append('override')
        body = None
        b = self._body(n)
        if b is not None:
            pre = []
            for c in n.get('inner', []):
                if c.get('kind') == 'CXXCtorInitializer':
                    pre.extend(self.ctor_init(c, owner))
            body = self.stmt(b)
            if pre:
                body.stmts = pre + body.stmts
        m = ir.MethodDecl(sym=s, name=n.get('name'), kind=kind, params=params, ret=s.ret if kind == 'method' else ir.VOID,
                          body=body, mods=mods, throws=[], typeparams=[], **self.pos(n))
        self.sigs[id(m)] = (n.get('name'), qt)
        return m

    def ctor_init(self, c, owner):
        p = self.pos(c)
        val = c['inner'][0] if c.get('inner') else None
        if 'baseInit' in c:
            bt = self.types.parse(c['baseInit']['qualType'])
            e = self.expr(val) if val is not None else None
            args = e.args if isinstance(e, ir.New) else ([e] if e is not None else [])
            sym = e.sym if isinstance(e, ir.New) else ir.Sym('ctor', bt.name, spell(bt))
            return [ir.CtorCall(kind='super', sym=sym, args=args or [], target=None, **p)]
        if 'delegatingInit' in c or 'delegateInit' in c:
            e = self.expr(val)
            return [ir.CtorCall(kind='this', sym=e.sym if isinstance(e, ir.New) else None,
                                args=e.args if isinstance(e, ir.New) else [], target=None, **p)]
        f = c.get('anyInit') or {}
        if not f or val is None:
            return []
        ot = self._record_type(owner)
        ft = self.types.parse(f['type']['qualType'])
        this = ir.This(qualifier=None, type=ir.Type('ptr', elem=ot, space='flat'), **p)
        target = ir.Select(obj=ir.Unary(op='*', x=this, postfix=False, type=ot, **p),
                           sym=ir.Sym('field', f['name'], spell(ot), False, ft), type=ft, **p)
        if val.get('kind') == 'ImplicitValueInitExpr' or (val.get('kind') == 'CXXConstructExpr' and not val.get('inner')
                                                           and ft.kind != 'class'):
            v = self.zero(ft, p)
        else:
            v = self.expr(val)
        if ft.kind == 'ptr' and ft.info.get('ref'):
            v = ir.Unary(op='&', x=v, postfix=False, type=ft, **p)
        return [ir.ExprStmt(expr=ir.Assign(op=None, target=target, value=v, type=ft, **p), **p)]

    # ---------------------------------------------------------------------------------------- statements
    def stmt(self, n):
        k = n.get('kind')
        p = self.pos(n)
        inner = n.get('inner') or []
        if k == 'CompoundStmt':
            out = []
            for c in inner:
                s = self.stmt(c)
                out.extend(s if isinstance(s, list) else [s])
            return ir.Block(stmts=out, **p)
        if k == 'DeclStmt':
            out = []
            for d in inner:
                if d.get('kind') == 'VarDecl':
                    out.append(self.local(d))
                elif d.get('kind') in ('RecordDecl', 'CXXRecordDecl'):
                    if d.get('completeDefinition'):
                        self.record(d)
                elif d.get('kind') == 'EnumDecl':
                    self.enum(d)
                elif d.get('kind') in ('TypedefDecl', 'TypeAliasDecl', 'StaticAssertDecl', 'UsingDecl',
                                       'UsingDirectiveDecl', 'FunctionDecl'):        # (a prototype in a body)
                    pass
                else:
                    out.append(ir.ExprStmt(expr=self.unsupported('local:' + d.get('kind', ''), d), **p))
            return out if len(out) != 1 else out[0]
        if k == 'NullStmt':
            return ir.Empty(**p)
        if k == 'ReturnStmt':
            e = self.expr(inner[0]) if inner else None
            return ir.Return(expr=e, **p)
        if k == 'IfStmt':
            i = 0
            pre = []
            if n.get('hasInit'):
                pre.append(self.stmt(inner[i]))
                i += 1
            if n.get('hasVar'):
                pre.append(self.stmt(inner[i]))
                i += 1
            cond = self.expr(inner[i])
            then = self.stmt(inner[i + 1])
            orelse = self.stmt(inner[i + 2]) if n.get('hasElse') and len(inner) > i + 2 else None
            s = ir.If(cond=cond, then=self._as_stmt(then), orelse=self._as_stmt(orelse) if orelse is not None else None, **p)
            return ir.Block(stmts=self._flat(pre) + [s], **p) if pre else s
        if k == 'WhileStmt':
            i = 1 if n.get('hasVar') else 0
            if i:
                return ir.ExprStmt(expr=self.unsupported('while-declaration', n), **p)
            return ir.While(cond=self.expr(inner[0]), body=self._as_stmt(self.stmt(inner[1])), **p)
        if k == 'DoStmt':
            return ir.DoWhile(body=self._as_stmt(self.stmt(inner[0])), cond=self.expr(inner[1]), **p)
        if k == 'ForStmt':
            init, var, cond, inc, body = [x if x.get('kind') else None for x in (inner + [{}] * 5)[:5]]
            if var:
                return ir.ExprStmt(expr=self.unsupported('for-declaration', n), **p)
            inits = []
            if init:
                if init.get('kind') == 'DeclStmt':
                    s = self.stmt(init)
                    inits = s if isinstance(s, list) else [s]
                else:
                    inits = [ir.ExprStmt(expr=e, **p) for e in self._commas(self.expr(init))]
            update = self._commas(self.expr(inc)) if inc else []
            return ir.For(init=inits, cond=self.expr(cond) if cond else None, update=update,
                          body=self._as_stmt(self.stmt(body)), **p)
        if k == 'SwitchStmt':
            i = int(bool(n.get('hasInit'))) + int(bool(n.get('hasVar')))
            if i:
                return ir.ExprStmt(expr=self.unsupported('switch-declaration', n), **p)
            return self.switch(n, inner[0], inner[1], p)
        if k == 'BreakStmt':
            return ir.Break(label=None, **p)
        if k == 'ContinueStmt':
            return ir.Continue(label=None, **p)
        if k == 'GotoStmt':
            return ir.Goto(label=self.labels.get(n.get('targetLabelDeclId'), 'L'), **p)
        if k == 'LabelStmt':
            return ir.Labeled(label=n.get('name'), body=self._as_stmt(self.stmt(inner[0])) if inner else ir.Empty(**p), **p)
        if k in ('CaseStmt', 'DefaultStmt'):
            return ir.ExprStmt(expr=self.unsupported('case-outside-switch-body', n), **p)
        if k == 'AttributedStmt':
            return self.stmt(inner[-1])
        if k == 'CXXTryStmt':
            body = self.stmt(inner[0])
            catches = []
            for c in inner[1:]:
                ci = c.get('inner') or []
                var = ci[0] if ci and ci[0].get('kind') == 'VarDecl' else None
                blk = ci[-1]
                if var is not None:
                    t = self.ty(var)
                    s = ir.Sym('local', self._local_name(var, self.scope), self.scope.owner, False, t)
                    self.scope.syms[var['id']] = s
                    catches.append(ir.Catch(sym=s, types=[t], body=self.stmt(blk), **self.pos(c)))
                else:
                    catches.append(ir.Catch(sym=None, types=[], body=self.stmt(blk), **self.pos(c)))
            return ir.Try(resources=[], body=body, catches=catches, final=None, **p)
        if k == 'CXXForRangeStmt':
            return self.range_for(n, p)
        if k == 'CXXThrowExpr':
            return ir.Throw(expr=self.expr(inner[0]) if inner else None, **p)
        if k in ('ExprWithCleanups',) and inner and inner[0].get('kind') == 'CXXThrowExpr':
            return self.stmt(inner[0])
        if k and (k.endswith('Expr') or k.endswith('Operator') or k == 'ExprWithCleanups'):
            return ir.ExprStmt(expr=self.expr(n), **p)
        return ir.ExprStmt(expr=self.unsupported('stmt:' + str(k), n), **p)

    def _flat(self, xs):
        out = []
        for x in xs:
            out.extend(x if isinstance(x, list) else [x])
        return out

    def _as_stmt(self, s):
        if isinstance(s, list):
            return ir.Block(stmts=s)
        return s

    def _commas(self, e):
        if isinstance(e, ir.Binary) and e.op == ',':
            return self._commas(e.left) + self._commas(e.right)
        return [e]

    def switch(self, n, cond_n, body_n, p):
        cases = []
        items = body_n.get('inner', []) if body_n.get('kind') == 'CompoundStmt' else [body_n]
        pre = []
        for it in items:
            cur = it
            labels, default = [], False
            while cur.get('kind') in ('CaseStmt', 'DefaultStmt'):
                if cur['kind'] == 'CaseStmt':
                    if cur.get('inner') and len(cur['inner']) > 2 and cur['inner'][1].get('kind') != 'NullStmt' \
                            and False:
                        pass
                    v = self.const(cur['inner'][0])
                    lab = self.expr(cur['inner'][0])
                    if v is not None:
                        lab = ir.Lit(value=v, type=self.ty(cur['inner'][0]), **self.pos(cur))
                    labels.append(lab)
                    cur = cur['inner'][-1] if len(cur.get('inner', [])) > 1 else {'kind': 'NullStmt'}
                else:
                    default = True
                    cur = cur['inner'][-1] if cur.get('inner') else {'kind': 'NullStmt'}
            if labels or default or it.get('kind') in ('CaseStmt', 'DefaultStmt'):
                body = self.stmt(cur)
                cases.append(ir.Case(labels=labels, default=default,
                                     body=[] if isinstance(body, ir.Empty) else self._flat([body]), **self.pos(it)))
            elif cases:
                s = self.stmt(it)
                cases[-1].body.extend(s if isinstance(s, list) else [s])
            else:
                s = self.stmt(it)
                pre.extend(s if isinstance(s, list) else [s])
        if self._case_inside(body_n):
            return ir.ExprStmt(expr=self.unsupported('case-label-nested-in-a-statement', n), **p)
        sw = ir.Switch(expr=self.expr(cond_n), cases=cases, **p)
        if [x for x in pre if not isinstance(x, (ir.Local, ir.Empty))]:
            return ir.ExprStmt(expr=self.unsupported('code-before-first-case', n), **p)
        return ir.Block(stmts=pre + [sw], **p) if pre else sw

    def _case_inside(self, body_n):
        """A case label below a statement of the switch body (Duff's device): not a structured switch."""
        top = body_n.get('inner', []) if body_n.get('kind') == 'CompoundStmt' else [body_n]
        def has(x, depth):
            if not isinstance(x, dict):
                return False
            if x.get('kind') in ('CaseStmt', 'DefaultStmt') and depth > 0:
                return True
            if x.get('kind') == 'SwitchStmt':
                return False
            nxt = depth + (0 if x.get('kind') in ('CaseStmt', 'DefaultStmt') else 1)
            return any(has(c, nxt) for c in x.get('inner', []) or [])
        return any(has(c, 0) for c in top)

    def range_for(self, n, p):
        """for (T x : array) -> ForEach over the array; anything else declines."""
        inner = n.get('inner') or []
        var = None
        rng = None
        for c in inner:
            if c.get('kind') == 'DeclStmt':
                for d in c.get('inner', []):
                    if d.get('kind') == 'VarDecl' and d.get('name') == '__range1' and d.get('inner'):
                        rng = d['inner'][0]
                    elif d.get('kind') == 'VarDecl' and not (d.get('name') or '').startswith('__'):
                        var = d
        body = inner[-1]
        if var is None or rng is None:
            return ir.ExprStmt(expr=self.unsupported('range-for', n), **p)
        it = self.expr(rng)
        if it.type.kind != 'array':
            return ir.ExprStmt(expr=self.unsupported('range-for:' + spell(it.type), n), **p)
        t = self.ty(var)
        s = ir.Sym('local', self._local_name(var, self.scope), self.scope.owner, False, t)
        self.scope.syms[var['id']] = s
        loc = ir.Local(sym=s, type=t, init=None, mods=[], **self.pos(var))
        return ir.ForEach(var=loc, iterable=it, body=self._as_stmt(self.stmt(body)), **p)

    def local(self, d):
        t = self.ty(d)
        name = self._local_name(d, self.scope)
        s = ir.Sym('local', name, self.scope.owner, False, t)
        self.scope.syms[d['id']] = s
        init = None
        for c in d.get('inner', []):
            if not c.get('kind', '').endswith('Attr'):
                init = self.expr(c)
                if t.kind == 'ptr' and t.info.get('ref'):
                    init = ir.Unary(op='&', x=init, postfix=False, type=t, **self.pos(c))
        t2 = self._complete_array(t, init)
        if t2 is not t:
            t = t2
            s.type = t
        mods = ['static'] if d.get('storageClass') == 'static' else []
        if d.get('storageClass') == 'extern':
            mods = ['extern']
        return ir.Local(sym=s, type=t, init=init, mods=mods, **self.pos(d))

    # ---------------------------------------------------------------------------------------- expressions
    def zero(self, t, p):
        if t.kind == 'class' or t.kind == 'record':
            return ir.New(sym=ir.Sym('ctor', '<aggregate>', spell(t)), args=[], body=None, outer=None, memory='stack',
                          type=t, **p)
        if t.kind == 'array':
            return ir.NewArray(elem=t.elem, dims=[], init=[], type=t, **p)
        if t.kind in ('ptr', 'fnptr'):
            return ir.Lit(value=None, type=t, **p)
        if t.kind in ('f32', 'f64', 'f80'):
            return ir.Lit(value=0.0, type=t, **p)
        if t.kind == 'bool':
            return ir.Lit(value=False, type=t, **p)
        return ir.Lit(value=0, type=t, **p)

    def expr(self, n):
        try:
            return self._expr(n)
        except (TypeError, KeyError, ValueError, IndexError) as e:
            return self.unsupported('expr:%s' % n.get('kind'), n, str(e)[:200])

    def _expr(self, n):
        k = n.get('kind')
        p = self.pos(n)
        inner = n.get('inner') or []
        t = self.ty(n)
        if k == 'IntegerLiteral':
            return ir.Lit(value=int(n['value']), type=t, **p)
        if k == 'CharacterLiteral':
            v = int(n['value'])
            txt = _char_text(v)
            return ir.Lit(value=v, type=t, **dict(p, **({'text': txt} if txt else {})))
        if k == 'FloatingLiteral':
            return ir.Lit(value=float(n['value']), type=t, **p)
        if k == 'StringLiteral':
            v = _esc_value(n['value'])
            return ir.Lit(value=v, type=t, **p)
        if k == 'CXXBoolLiteralExpr':
            return ir.Lit(value=bool(n['value']), type=t, **p)
        if k in ('CXXNullPtrLiteralExpr', 'GNUNullExpr'):
            return ir.Lit(value=None, type=ir.NULL, **p)
        if k == 'PredefinedExpr':
            return self.expr(inner[0]) if inner else self.unsupported(k, n)
        if k == 'ParenExpr':
            return ir.Paren(x=self.expr(inner[0]), type=t, **p)
        if k in _WRAPPERS:
            if k == 'CXXDefaultInitExpr' and not inner:
                return self.unsupported(k, n)
            if k == 'SubstNonTypeTemplateParmExpr':          # [the parameter, the value it stands for]
                return self.expr(inner[-1])
            return self.expr(inner[0])
        if k == 'CXXDefaultArgExpr':
            return self.default_arg(n, p)
        if k == 'ImplicitCastExpr':
            return self.implicit_cast(n, inner[0], t, p)
        if k in _CASTS:
            x = inner[0] if inner else None
            if k == 'CXXFunctionalCastExpr' and x is not None and x.get('kind') in ('CXXConstructExpr', 'InitListExpr'):
                return self.expr(x)
            if n.get('castKind') == 'ToVoid':
                return ir.Cast(x=self.expr(x), type=ir.VOID, **p)
            if n.get('castKind') in ('ConstructorConversion', 'NoOp') and t.kind == 'class':
                return self.expr(x)
            return ir.Cast(x=self.expr(x), type=t, **p)
        if k == 'DeclRefExpr':
            return self.decl_ref(n, t, p)
        if k == 'MemberExpr':
            return self.member(n, t, p)
        if k == 'ArraySubscriptExpr':
            a, b = self.expr(inner[0]), self.expr(inner[1])
            if a.type is not None and a.type.kind not in ('ptr', 'array') and b.type is not None and \
                    b.type.kind in ('ptr', 'array'):
                a, b = b, a
            return ir.Index(arr=a, index=b, type=t, **p)
        if k == 'UnaryOperator':
            return self.unary(n, t, p)
        if k == 'BinaryOperator':
            op = n['opcode']
            l, r = self.expr(inner[0]), self.expr(inner[1])
            if op == '=':
                return ir.Assign(op=None, target=l, value=r, type=t, **p)
            if op in _OPS:
                return ir.Binary(op=op, left=l, right=r, type=t, **p)
            return self.unsupported('binary:' + op, n)
        if k == 'CompoundAssignOperator':
            op = n['opcode'][:-1]
            return ir.Assign(op=op, target=self.expr(inner[0]), value=self.expr(inner[1]), type=t, **p)
        if k == 'ConditionalOperator':
            return ir.Cond(cond=self.expr(inner[0]), then=self.expr(inner[1]), orelse=self.expr(inner[2]), type=t, **p)
        if k == 'CallExpr':
            return self.call(n, t, p)
        if k == 'CXXMemberCallExpr':
            return self.member_call(n, t, p)
        if k == 'CXXOperatorCallExpr':
            return self.operator_call(n, t, p)
        if k == 'UnaryExprOrTypeTraitExpr':
            return self.sizeof(n)
        if k == 'InitListExpr':
            return self.init_list(n, t, p)
        if k == 'ImplicitValueInitExpr':
            return self.zero(t, p)
        if k == 'CompoundLiteralExpr':
            return self.expr(inner[0])
        if k in ('CXXConstructExpr', 'CXXTemporaryObjectExpr'):
            return self.construct(n, t, p)
        if k == 'CXXNewExpr':
            return self.new(n, t, p)
        if k == 'CXXDeleteExpr':
            return ir.Unary(op='delete[]' if n.get('isArray') else 'delete', x=self.expr(inner[0]), postfix=False,
                            type=ir.VOID, **p)
        if k == 'CXXThisExpr':
            return ir.This(qualifier=None, type=t, **p)
        if k == 'CXXScalarValueInitExpr':
            return self.zero(t, p)
        if k == 'VAArgExpr':                     # va_arg(ap, T): the builtin, typed T
            return ir.Call(target=None, sym=ir.Sym('function', '__builtin_va_arg', None, True,
                                                   params=[self.ty(inner[0])], ret=t),
                           args=[self.expr(inner[0])], typeargs=[], type=t, **p)
        if k == 'CXXThrowExpr':
            return self.unsupported('throw-as-expression', n)
        return self.unsupported(k, n)

    def implicit_cast(self, n, x_n, t, p):
        ck = n.get('castKind')
        if ck in _DROP_CASTS:
            x = self.expr(x_n)
            if ck == 'NoOp' and x.type is not None and x.type.kind == 'ptr' and t.kind == 'ptr':
                return x
            return x
        if ck in ('UncheckedDerivedToBase', 'DerivedToBase'):
            x = self.expr(x_n)
            return ir.Cast(x=x, type=t, implicit=True, **p)
        if ck in ('ConstructorConversion', 'UserDefinedConversion'):
            return self.expr(x_n)
        x = self.expr(x_n)
        if ck == 'BuiltinFnToFnPtr':
            return x
        return ir.Cast(x=x, type=t, implicit=True, **p)

    def default_arg(self, n, p):
        d = self.decls.get((n.get('param') or {}).get('id')) if isinstance(n.get('param'), dict) else None
        if d is not None and d.get('inner'):
            return self.expr(d['inner'][-1])
        return self.unsupported('default-argument', n)

    def decl_ref(self, n, t, p):
        d = n.get('referencedDecl') or {}
        dk = d.get('kind')
        did = d.get('id')
        if dk in ('VarDecl', 'ParmVarDecl', 'BindingDecl'):
            s = self.scope.syms.get(did) if getattr(self, 'scope', None) else None
            if s is None:
                node = self.decls.get(did) or d
                par = self._member_owner(node) if node is not d else self.parent.get(did)
                if par is not None and par.get('kind') in ('CXXRecordDecl', 'ClassTemplateSpecializationDecl'):
                    rt = self._record_type(par)
                    st = self.ty(node)
                    return ir.Select(obj=ir.TypeName(type=rt, **p), sym=ir.Sym('field', node['name'], spell(rt), True, st),
                                     type=st, **p)
                st = self.ty(node)
                f = node.get('_f')
                s = ir.Sym('global', d.get('name'), self.unit_name if self._is_own(f) else
                           self._lib_owner(node), True, st)
            e = ir.Name(sym=s, type=s.type if s.type is not None else t, **p)
            if s.type is not None and s.type.kind == 'ptr' and s.type.info.get('ref'):
                return ir.Unary(op='*', x=e, postfix=False, type=s.type.elem, **p)
            return e
        if dk in ('FunctionDecl', 'CXXMethodDecl'):
            s = self._sym_of_function(d)
            return ir.AddressOf(sym=s, type=ctypes.decay(self.ty(self.decls.get(did, d))), **p)
        if dk == 'EnumConstantDecl':
            par = self.parent.get(did)
            v = self.enum_values.get(did)
            if par is not None and par['id'] in self.scoped_enums:
                qn = self.scoped_enums[par['id']]
                return ir.Select(obj=ir.TypeName(type=ir.Type('class', qn), **p),
                                 sym=ir.Sym('field', d['name'], qn, True, ir.I32), type=ir.I32, **p)
            node = self.decls.get(did) or {}
            if not self._is_own(node.get('_f')):
                return ir.Lit(value=v, text=d['name'], type=ir.I32, **p)
            return ir.Name(sym=ir.Sym('global', d['name'], self.unit_name, True, ir.I32), type=ir.I32, **p)
        if dk == 'NonTypeTemplateParmDecl':
            return self.unsupported('template-parameter', n)
        return self.unsupported('ref:' + str(dk), n)

    def member(self, n, t, p):
        base_n = n['inner'][0]
        base = self.expr(base_n)
        md = self.decls.get(n.get('referencedMemberDecl'))
        name = n.get('name')
        if md is not None and md.get('kind') in ('CXXMethodDecl',):
            return self.unsupported('bound-member-function', n)
        if md is not None and md.get('kind') == 'VarDecl':          # a static member through an object
            par = self.parent.get(md['id'])
            rt = self._record_type(par)
            return ir.Select(obj=ir.TypeName(type=rt, **p), sym=ir.Sym('field', name, spell(rt), True, t), type=t, **p)
        obj = base
        if n.get('isArrow'):
            bt = base.type
            el = bt.elem if bt is not None and bt.kind == 'ptr' else ir.ANY
            obj = ir.Unary(op='*', x=base, postfix=False, type=el, **p)
        ot = obj.type
        owner = spell(ot) if ot is not None else None
        if md is not None:
            par = self.parent.get(md['id'])
            if par is not None and self._record_type(par) is not None:
                owner = spell(self._record_type(par))
        ft = t
        if md is not None:
            ft = self.ty(md)
        e = ir.Select(obj=obj, sym=ir.Sym('field', name, owner, False, ft), type=ft, **p)
        if ft.kind == 'ptr' and ft.info.get('ref'):
            return ir.Unary(op='*', x=e, postfix=False, type=ft.elem, **p)
        if md is None or ft != t:
            e.type = t if t.kind != 'any' else ft
        return e

    def unary(self, n, t, p):
        op = n['opcode']
        x_n = n['inner'][0]
        if op == '&':
            # &function: a function's address
            inner = x_n
            while inner.get('kind') in ('ParenExpr',):
                inner = inner['inner'][0]
            if inner.get('kind') == 'DeclRefExpr' and (inner.get('referencedDecl') or {}).get('kind') == 'FunctionDecl':
                return self.expr(inner)
        x = self.expr(x_n)
        if op == '__extension__':
            return x
        if op in ('++', '--'):
            return ir.Unary(op=op, x=x, postfix=bool(n.get('isPostfix')), type=t, **p)
        if op in ('-', '+', '!', '~', '*', '&'):
            if op == '*' and x.type is not None and x.type.kind == 'fnptr':
                return x                                         # (*fp)(...) is fp(...)
            return ir.Unary(op=op, x=x, postfix=False, type=t, **p)
        return self.unsupported('unary:' + op, n)

    def bind_refs(self, args, params):
        """An argument given to a reference parameter: its binding made explicit (Unary '&'), as for a
        pointer, so every back end sees which variables a call can change."""
        out = []
        for i, a in enumerate(args):
            pt = params[i] if i < len(params) else None
            if pt is not None and pt.kind == 'ptr' and pt.info.get('ref') and not (
                    isinstance(a, ir.Unary) and a.op == '&') and not (a.type is not None and a.type.kind == 'ptr'
                                                                     and a.type.info.get('ref')):
                a = ir.Unary(op='&', x=a, postfix=False, type=pt, line=a.line, col=a.col)
            out.append(a)
        return out

    def call(self, n, t, p):
        inner = n['inner']
        callee = inner[0]
        c = callee
        while c.get('kind') in ('ImplicitCastExpr', 'ParenExpr') and c.get('inner'):
            if c.get('kind') == 'ImplicitCastExpr' and c.get('castKind') not in ('FunctionToPointerDecay', 'BuiltinFnToFnPtr', 'NoOp'):
                break
            c = c['inner'][0]
        args = [self.expr(a) for a in inner[1:]]
        if c.get('kind') == 'DeclRefExpr' and (c.get('referencedDecl') or {}).get('kind') in ('FunctionDecl', 'CXXMethodDecl'):
            d = c['referencedDecl']
            s = self._sym_of_function(d)
            args = self.bind_refs(args, s.params)
            node = self.decls.get(d['id'], {})
            if node.get('kind') == 'CXXMethodDecl':        # a static member function
                par = self.decls.get(node.get('parentDeclContextId')) or self.parent.get(node['id'])
                return ir.Call(target=ir.TypeName(type=self._record_type(par), **p), sym=s, args=args, typeargs=[],
                               type=t, **p)
            return ir.Call(target=None, sym=s, args=args, typeargs=[], type=t, **p)
        if c.get('kind') == 'MemberExpr':
            return self.member_call(dict(n, inner=[c] + inner[1:]), t, p)
        fn = self.expr(callee)
        return ir.Call(target=fn, sym=None, args=args, typeargs=[], type=t, **p)

    def member_call(self, n, t, p):
        inner = n['inner']
        me = inner[0]
        while me.get('kind') == 'ParenExpr':
            me = me['inner'][0]
        if me.get('kind') != 'MemberExpr':
            return self.unsupported('member-call:' + str(me.get('kind')), n)
        md = self.decls.get(me.get('referencedMemberDecl')) or {'id': me.get('referencedMemberDecl'), 'name': me.get('name'),
                                                                   'kind': 'CXXMethodDecl'}
        base = self.expr(me['inner'][0])
        obj = base
        if me.get('isArrow'):
            obj = ir.Unary(op='*', x=base, postfix=False, type=base.type.elem if base.type.kind == 'ptr' else ir.ANY, **p)
        s = self._sym_of_function(md)
        args = self.bind_refs([self.expr(a) for a in inner[1:]], s.params)
        if md.get('kind') == 'CXXConversionDecl':
            return ir.Cast(x=obj, type=t, **p)
        return ir.Call(target=obj, sym=s, args=args, typeargs=[], type=t, **p)

    def operator_call(self, n, t, p):
        inner = n['inner']
        c = inner[0]
        while c.get('kind') in ('ImplicitCastExpr', 'ParenExpr') and c.get('inner'):
            c = c['inner'][0]
        d = c.get('referencedDecl') or {}
        node = self.decls.get(d.get('id'), d)
        args = [self.expr(a) for a in inner[1:]]
        name = d.get('name', '')
        if node.get('isImplicit') or node.get('explicitlyDefaulted'):
            if name == 'operator=' and len(args) == 2:
                return ir.Assign(op=None, target=args[0], value=args[1], type=t, **p)
        s = self._sym_of_function(d)
        if node.get('kind') == 'CXXMethodDecl':
            return ir.Call(target=args[0], sym=s, args=self.bind_refs(args[1:], s.params), typeargs=[], type=t, **p)
        return ir.Call(target=None, sym=s, args=self.bind_refs(args, s.params), typeargs=[], type=t, **p)

    def construct(self, n, t, p):
        args = [self.expr(a) for a in (n.get('inner') or [])]
        ctor_t = (n.get('ctorType') or {}).get('qualType', '')
        cls = self._find_record(spell(t)) if t.kind == 'class' else None
        ctor = self._find_ctor(cls, ctor_t) if cls is not None else None
        copying = len(args) == 1 and args[0].type is not None and spell(args[0].type) == spell(t)
        if copying and (ctor is None or ctor.get('isImplicit') or ctor.get('explicitlyDefaulted')):
            return args[0]                                   # a copy or a move: the value itself
        if t.kind != 'class':
            return args[0] if args else self.zero(t, p)
        if ctor is None or ctor.get('isImplicit') or ctor.get('explicitlyDefaulted'):
            if not args:
                return ir.New(sym=ir.Sym('ctor', '<aggregate>', spell(t)), args=[], body=None, outer=None,
                              memory='stack', type=t, **p)
        sym = self._sym_of_function(ctor) if ctor is not None else ir.Sym('ctor', t.name, spell(t))
        return ir.New(sym=sym, args=self.bind_refs(args, sym.params), body=None, outer=None, memory='stack', type=t, **p)

    def _find_ctor(self, cls, ctor_t):
        for m in cls.get('inner', []):
            if m.get('kind') == 'CXXConstructorDecl' and (m.get('type') or {}).get('qualType') == ctor_t:
                return m
        for m in cls.get('inner', []):
            if m.get('kind') == 'FunctionTemplateDecl':
                for x in m.get('inner', []):
                    if x.get('kind') == 'CXXConstructorDecl' and (x.get('type') or {}).get('qualType') == ctor_t:
                        return x
        return None

    def new(self, n, t, p):
        inner = n.get('inner') or []
        el = t.elem if t.kind == 'ptr' else ir.ANY
        if n.get('isArray'):
            size = None
            init = None
            for c in inner:
                if c.get('kind') in ('InitListExpr',):
                    init = [self.expr(x) for x in c.get('inner', [])]
                elif size is None and c.get('kind') != 'CXXConstructExpr':
                    size = self.expr(c)
            return ir.NewArray(elem=el, dims=[size] if size is not None else [], init=init, type=t, **p)
        if inner:
            e = self.expr(inner[-1])
            if isinstance(e, ir.New):
                e.memory = 'manual'
                e.type = t
                return e
            return ir.New(sym=ir.Sym('ctor', '<value>', spell(el)), args=[e], body=None, outer=None, memory='manual',
                          type=t, **p)
        return ir.New(sym=ir.Sym('ctor', '<value>', spell(el)), args=[], body=None, outer=None, memory='manual', type=t, **p)

    def sizeof(self, n):
        p = self.pos(n)
        name = n.get('name', 'sizeof')
        if 'argType' in n:
            at = ctypes.normalise(self.types.parse(n['argType']['qualType']))
            text = '%s(%s)' % (name, n['argType']['qualType'])
        else:
            arg = n['inner'][0]
            at = self.ty(arg)
            text = '%s %s' % (name, 'expr')
        if name not in ('sizeof', 'alignof', '_Alignof', '__alignof'):
            return self.unsupported('trait:' + name, n)
        v = self.sizes.size(at) if name == 'sizeof' else self.sizes.align(at)
        return ir.Lit(value=v, text=text, type=self.ty(n), **p)

    def init_list(self, n, t, p):
        inner = n.get('inner') or []
        filler = None
        if 'array_filler' in n:                  # clang writes the filler first, then the elements given
            filler, inner = n['array_filler'][0], n['array_filler'][1:]
        if t.kind == 'array':
            items = [self.expr(x) for x in inner]
            if filler is not None and filler.get('kind') != 'ImplicitValueInitExpr' and 'count' in t.info:
                fill = self.expr(filler)
                items += [fill] * (t.info['count'] - len(items))
            # a string literal initialising a char array
            if len(items) == 1 and isinstance(items[0], ir.Lit) and isinstance(items[0].value, str) and \
                    t.elem.kind in ('char', 'i8', 'u8'):
                return items[0]
            return ir.NewArray(elem=t.elem, dims=[], init=items, type=t, **p)
        if t.kind == 'class':
            if 'field' in n:                                      # a union: the member it initialises
                f = n['field']
                return ir.New(sym=ir.Sym('ctor', '<aggregate:%s>' % f.get('name'), spell(t)),
                              args=[self.expr(x) for x in inner], body=None, outer=None, memory='stack', type=t, **p)
            return ir.New(sym=ir.Sym('ctor', '<aggregate>', spell(t)), args=[self.expr(x) for x in inner], body=None,
                          outer=None, memory='stack', type=t, **p)
        if len(inner) == 1:                                       # int x = {5};
            return self.expr(inner[0])
        if not inner:
            return self.zero(t, p)
        return self.unsupported('init-list:' + spell(t), n)


def _wrap(v, t):
    """An integer constant reduced to its type's width, as C does."""
    bits = {'i8': 8, 'u8': 8, 'char': 8, 'i16': 16, 'u16': 16, 'i32': 32, 'u32': 32, 'i64': 64, 'u64': 64, 'bool': 1}
    if t is None or t.kind not in bits:
        return v
    if t.kind == 'bool':
        return int(bool(v))
    b = bits[t.kind]
    v &= (1 << b) - 1
    if t.kind.startswith('i') or (t.kind == 'char' and t.info.get('signed', True)):
        if v >= 1 << (b - 1):
            v -= 1 << b
    return v


class _Scope(object):
    def __init__(self, owner):
        self.owner = owner
        self.syms = {}
        self.used = set()
        self.n = 0

    def fresh(self, base):
        self.n += 1
        name = '%s%d' % (base, self.n)
        self.used.add(name)
        return name
