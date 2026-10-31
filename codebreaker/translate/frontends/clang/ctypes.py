"""ctypes.py -- clang's type spellings ("const char *", "int (*)[4]", "geo::Vec &") as IR types, and the
sizes and layouts the source's compiler gives them.

clang's JSON AST writes an expression's or declaration's type as text (`qualType`), so the front end
reads C's declarator grammar back: specifiers, then an abstract declarator (pointers bind looser than
array and function suffixes; a parenthesised part applies last). Typedef names, struct/union/class tags
and C++ class names are resolved through the `names` callback the lowering supplies, so a type is
always concrete: `uint8_t` is u8, `Pair` is the record it names.

The data model is the target's: `Model.for_triple("x86_64-pc-windows-msvc")` is LLP64 (long is 4 bytes,
pointers 8, plain char signed, long double = double), which is what the judge's clang build runs.
"""
import re

from codebreaker.translate import ir

# ------------------------------------------------------------------------------------------ data models
class Model(object):
    """Widths the target gives the types C leaves open."""

    def __init__(self, triple, pointer, long, wchar, char_signed=True, long_double=8):
        self.triple, self.pointer, self.long, self.wchar = triple, pointer, long, wchar
        self.char_signed, self.long_double = char_signed, long_double

    @classmethod
    def for_triple(cls, triple):
        t = triple.lower()
        if 'windows' in t or 'win32' in t or 'mingw' in t:
            return cls(triple, 8 if t.startswith(('x86_64', 'aarch64')) else 4, 4, 2)
        return cls(triple, 8 if t.startswith(('x86_64', 'aarch64')) else 4,
                   8 if t.startswith(('x86_64', 'aarch64')) else 4, 4, long_double=16)

    def json(self, language, std):
        return {'triple': self.triple, 'pointer': self.pointer, 'long': self.long, 'wchar': self.wchar,
                'char_signed': self.char_signed, 'language': language, 'std': std}


_INT = {1: 'i8', 2: 'i16', 4: 'i32', 8: 'i64'}
_UINT = {1: 'u8', 2: 'u16', 4: 'u32', 8: 'u64'}
_QUALS = frozenset(('const', 'volatile', 'restrict', '__restrict', '__unaligned', '_Atomic', 'struct', 'union',
                    'enum', 'class', 'typename', '__ptr64', '__ptr32', '__w64',
                    # calling conventions: one convention on the x64 target
                    '__cdecl', '__stdcall', '__fastcall', '__thiscall', '__vectorcall', '__clrcall'))
_BUILTIN = frozenset(('void', 'char', 'short', 'int', 'long', 'float', 'double', 'signed', 'unsigned', '_Bool',
                      'bool', 'wchar_t', 'char8_t', 'char16_t', 'char32_t', '__int64', '__int32', '__int16',
                      '__int8', '__int128', '_Float16', '__fp16'))


def builtin(words, model):
    """['unsigned', 'long', 'long'] -> ir.Type, by the model."""
    w = list(words)
    uns = 'unsigned' in w
    sig = 'signed' in w
    longs = w.count('long')
    if 'void' in w:
        return ir.VOID
    if '_Bool' in w or 'bool' in w:
        return ir.BOOL
    if 'float' in w:
        return ir.Type('f32')
    if 'double' in w:
        return ir.Type('f64') if not longs or model.long_double == 8 else ir.Type('f80')
    if '__int128' in w:
        raise TypeError('__int128')
    if '_Float16' in w or '__fp16' in w:
        raise TypeError('half float')
    if 'wchar_t' in w:
        return ir.Type(_UINT[model.wchar] if model.wchar == 2 else _INT[model.wchar])
    if 'char8_t' in w:
        return ir.Type('u8')
    if 'char16_t' in w:
        return ir.Type('u16')
    if 'char32_t' in w:
        return ir.Type('u32')
    if 'char' in w:
        if uns:
            return ir.Type('u8')
        if sig:
            return ir.Type('i8')
        return ir.Type('char', signed=model.char_signed)
    if '__int8' in w:
        size = 1
    elif 'short' in w or '__int16' in w:
        size = 2
    elif longs >= 2 or '__int64' in w:
        size = 8
    elif longs == 1:
        size = model.long
    else:
        size = 4
    return ir.Type((_UINT if uns else _INT)[size])


# ------------------------------------------------------------------------------------------ the parser
_TOKEN = re.compile(r'\s*(\.\.\.|::|&&|[A-Za-z_~][\w]*|\d+|[*&()\[\],<>:])')


def tokens(s):
    out, i = [], 0
    s = re.sub(r'__attribute__\(\((?:[^()]|\([^()]*\))*\)\)', ' ', s)     # (cdecl, aligned, ...: not a type)
    s = re.sub(r'\(unnamed (struct|union|enum|class) at [^)]*\)', lambda m: '__unnamed_' + _anon_key(m.group(0)), s)
    s = re.sub(r'\(anonymous (struct|union|enum|class) at [^)]*\)', lambda m: '__unnamed_' + _anon_key(m.group(0)), s)
    s = re.sub(r'\((?:unnamed|anonymous) at [^)]*\)', lambda m: 'struct __unnamed_' + _anon_key(m.group(0)), s)
    s = re.sub(r'\(lambda at [^)]*\)', '__lambda', s)
    while i < len(s):
        m = _TOKEN.match(s, i)
        if not m:
            if s[i:].strip() == '':
                break
            raise TypeError('cannot read type %r at %r' % (s, s[i:]))
        out.append(m.group(1))
        i = m.end()
    return out


def _anon_key(text):
    """'(unnamed struct at C:\\x\\t.c:3:9)' -> '3_9' (the line and column clang named it by)."""
    m = re.search(r':(\d+):(\d+)\)$', text)
    return '%s_%s' % (m.group(1), m.group(2)) if m else '0_0'


def anon_key(line, col):
    return '%s_%s' % (line, col)


class Parser(object):
    """parse(text) -> ir.Type. `names(name, args, tag)` resolves a typedef / record / class name: it returns
    an ir.Type (a typedef's meaning, or Type('class', qname, args)), or None if unknown."""

    def __init__(self, model, names):
        self.model = model
        self.names = names
        self.cache = {}

    def parse(self, text):
        if text in self.cache:
            return self.cache[text]
        saved = (getattr(self, 't', None), getattr(self, 'i', 0))     # (a typedef's meaning is parsed mid-parse)
        self.t = tokens(text)
        self.i = 0
        try:
            t = self.type_()
            if self.i != len(self.t):
                raise TypeError('trailing %r in type %r' % (self.t[self.i:], text))
        finally:
            self.t, self.i = saved
        self.cache[text] = t
        return t

    def peek(self, k=0):
        j = self.i + k
        return self.t[j] if j < len(self.t) else None

    def take(self, want=None):
        tok = self.peek()
        if want is not None and tok != want:
            raise TypeError('expected %r, got %r' % (want, tok))
        self.i += 1
        return tok

    def type_(self):
        base = self.specifiers()
        return self.declarator(base)

    def specifiers(self):
        words, tag, named = [], None, None
        while True:
            tok = self.peek()
            if tok is None:
                break
            if tok in ('struct', 'union', 'enum', 'class'):
                tag = tok
                self.i += 1
                continue
            if tok in _QUALS:
                self.i += 1
                continue
            if tok in _BUILTIN and named is None and tag is None:
                words.append(tok)
                self.i += 1
                continue
            if (tok[0].isalpha() or tok[0] in '_~' or tok == '::') and not words and named is None:
                named = self.qualified_name()
                continue
            break
        if words:
            return builtin(words, self.model)
        if named is None:
            raise TypeError('no type in %r' % (self.t,))
        name, args = named
        if tag == 'enum':
            got = self.names(name, args, 'enum')
            return got if got is not None else ir.I32
        got = self.names(name, args, tag)
        if got is None:
            raise TypeError('unknown type name %r' % name)
        return got

    def qualified_name(self):
        parts, args = [], []
        if self.peek() == '::':
            self.i += 1
        while True:
            parts.append(self.take())
            if self.peek() == '<':
                self.i += 1
                args = self.template_args()
            if self.peek() == '::':
                self.i += 1
                if parts and args:                       # (an argument list in the middle: kept on its part)
                    parts[-1] = parts[-1] + '<%s>' % ', '.join(repr(a) for a in args)
                    args = []
                continue
            break
        return '::'.join(parts), args

    def template_args(self):
        args = []
        while self.peek() not in ('>', None):
            if self.peek().isdigit() or self.peek() in ('true', 'false'):
                args.append(ir.Type('class', self.take()))      # a non-type argument: kept as its spelling
            else:
                args.append(self.type_())
            if self.peek() == ',':
                self.i += 1
        self.take('>')
        return args

    def declarator(self, t):
        ops = []
        while self.peek() in ('*', '&', '&&') or self.peek() in _QUALS:
            tok = self.take()
            if tok in ('*', '&', '&&'):
                ops.append(tok)
        for op in ops:
            t = ir.Type('ptr', elem=t, space='flat', **({'ref': True} if op != '*' else {}))
        inner = None
        if self.peek() == '(' and (self.peek(1) in ('*', '&', '&&', '(') or
                                   (self.peek(1) in _QUALS and self.peek(2) in ('*', '&', '&&'))):
            self.i += 1
            depth, start = 1, self.i
            while depth:
                tok = self.take()
                depth += tok == '('
                depth -= tok == ')'
            inner = self.t[start:self.i - 1]
        t = self.suffixes(t)
        if inner is not None:
            saved = (self.t, self.i)
            self.t, self.i = inner, 0
            t = self.declarator(t)
            self.t, self.i = saved
        return t

    def suffixes(self, t):
        sufs = []
        while self.peek() in ('[', '('):
            if self.take() == '[':
                n = None
                if self.peek() != ']':
                    n = int(self.take())
                self.take(']')
                sufs.append(('a', n))
            else:
                params, variadic = [], False
                while self.peek() != ')':
                    if self.peek() == '...':
                        self.i += 1
                        variadic = True
                    else:
                        params.append(self.type_())
                    if self.peek() == ',':
                        self.i += 1
                self.take(')')
                while self.peek() in ('const', 'volatile', 'noexcept', '&', '&&', '__attribute__'):
                    self.i += 1
                if len(params) == 1 and params[0] == ir.VOID:
                    params = []
                sufs.append(('f', params, variadic))
        for s in reversed(sufs):
            if s[0] == 'a':
                t = ir.Type('array', elem=t, **({'count': s[1]} if s[1] is not None else {}))
            else:
                t = fn_type(t, s[1], s[2])
        return t


def fn_type(ret, params, variadic=False, names=None):
    info = dict(conv='cdecl', params=[dict({'type': p}, **({'name': names[i]} if names and names[i] else {}))
                                      for i, p in enumerate(params)],
                result=({'type': ret, 'at': 'none'}))
    if variadic:
        info['variadic'] = True
    return ir.Type('fn', **info)


def decay(t):
    """A function type as a pointer to it; an array type as a pointer to its element."""
    if t.kind == 'fn':
        return ir.Type('fnptr', space='flat', fn=t)
    if t.kind == 'array':
        return ir.Type('ptr', elem=t.elem, space='flat')
    return t


def fnptr_of(t):
    """A pointer to a function type -> the 'fnptr' kind the IR uses for it."""
    if t.kind == 'ptr' and t.elem is not None and t.elem.kind == 'fn' and not t.info.get('ref'):
        return ir.Type('fnptr', space='flat', fn=t.elem)
    return t


def normalise(t):
    """Every pointer to a function as an 'fnptr', all the way down."""
    if t is None:
        return t
    if t.kind == 'ptr':
        e = normalise(t.elem)
        if e.kind == 'fn' and not t.info.get('ref'):
            return ir.Type('fnptr', space='flat', fn=e)
        return ir.Type('ptr', elem=e, **t.info)
    if t.kind == 'array':
        return ir.Type('array', elem=normalise(t.elem), **t.info)
    if t.kind == 'fn':
        info = dict(t.info)
        info['params'] = [dict(p, type=normalise(p['type'])) for p in info.get('params', [])]
        info['result'] = dict(info['result'], type=normalise(info['result'].get('type')))
        return ir.Type('fn', **info)
    if t.kind == 'class' and t.args:
        return ir.Type('class', t.name, [normalise(a) for a in t.args])
    return t


# ------------------------------------------------------------------------------------------ sizes
_PRIM_SIZE = {'bool': 1, 'i8': 1, 'u8': 1, 'char': 1, 'i16': 2, 'u16': 2, 'i32': 4, 'u32': 4, 'i64': 8,
              'u64': 8, 'f32': 4, 'f64': 8, 'f80': 16}


class Layouts(object):
    """sizeof / alignof by the model; records through `record(name)` -> ir 'record' Type (or None)."""

    def __init__(self, model, record):
        self.model = model
        self.record = record

    def size(self, t):
        return self.size_align(t)[0]

    def align(self, t):
        return self.size_align(t)[1]

    def size_align(self, t):
        k = t.kind
        if k in _PRIM_SIZE:
            s = _PRIM_SIZE[k]
            return s, s
        if k in ('ptr', 'fnptr'):
            return self.model.pointer, self.model.pointer
        if k == 'array':
            s, a = self.size_align(t.elem)
            return s * t.info.get('count', 0), a
        if k == 'class':
            r = self.record(t)
            if r is None:
                raise TypeError('no layout for %r' % (t,))
            return r.info['size'], self.record_align(r)
        if k == 'record':
            return t.info['size'], self.record_align(t)
        if k == 'void':
            return 1, 1
        raise TypeError('no size for %r' % (t,))

    def record_align(self, r):
        """A record's alignment: its widest field's (L2 keeps offsets and size, not the alignment)."""
        if 'align' in r.info:
            return r.info['align']
        return max([self.align(f['type']) for f in r.info.get('fields', [])] or [1])

    def lay_out(self, name, fields, union=False):
        """fields [(name, Type)] -> ('record' Type, align): C's rules -- each field at the next multiple
        of its alignment, the whole padded to the largest alignment."""
        off, big, out = 0, 1, []
        for fname, ft in fields:
            s, a = self.size_align(ft)
            big = max(big, a)
            if union:
                out.append({'name': fname, 'offset': 0, 'type': ft})
                off = max(off, s)
            else:
                off = (off + a - 1) // a * a
                out.append({'name': fname, 'offset': off, 'type': ft})
                off += s
        size = (off + big - 1) // big * big
        return ir.Type('record', name, size=size, fields=out, role='union' if union else 'struct', packed=False), big
