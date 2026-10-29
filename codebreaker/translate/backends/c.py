"""c.py -- the hub's C back end: an L2 function document as C (docs/IR_UNIFICATION.md, step 2).

The decompiler's PrintL2 writes each function as L2 (`/*@L2 {...} */`, CB_L2=1). This writes it
back as C in the decompiler's own dialect -- its type names (int2, uint4, xunknown2, code), its
calling-convention words (__cdecl16far, __stdcall), hexadecimal past 9 -- so what it writes goes
through decompile/c_text.py's passes as the decompiler's C does today, and is compiled and judged
the same way. tools/l2c.py holds it to that C, character for character, over the golden corpus.

The text is laid out as PrintC lays it out: the same calls to the core's emitter (openGroup,
spaces, tagLine, ...) in the same order -- each expression through a reverse-Polish stack as
PrintLanguage::pushOp/pushAtom place it -- run through a port of its pretty printer
(EmitPrettyPrint: lines of 100, a long one broken where its groups allow). PrintC's comments (its
warnings) come with L2: the function's above it (MethodDecl.comments), the others as Comment
statements at their places.

Works on the JSON document itself (a dict), not on ir.py's classes: L2 from a binary carries
address spaces, conventions and layouts the Java front end's classes do not need.

    function(doc, moved=()) -> C text of one function          type_text(t, name='') -> a declaration
    module(doc) -> (C text, {name: assembly}) of a Module: its functions, and what stayed assembly

`moved` names the passes of decompile/c_text.py done here instead, on the tree (step 3): each is
written where the address spaces and types L2 carries say so, and c_text leaves its own out for C
written from L2. MOVED is every one there is. What they add is laid out as taking no room: c_text
adds the same to the printed C, after PrintC broke its lines.
"""

import re

# c_text's passes done on L2 (docs/IR_UNIFICATION.md step 3, L2_BINARY_TYPES.md's order)
MOVED = ("far", "near_calls", "far_from_stack", "frame_through_ss", "pieces", "segment_registers",
         "null_is_zero", "global_pointer_steps", "one_name_for_globals", "far_result")
FLAT = ("pieces", "null_is_zero")      # ... those of them c_text's 32-bit path has too (compilable32)
_now = {"moved": (), "far": set(), "marks": {}, "whole": {}, "bare": set(), "sixteen": False}
# (the function being written: its passes, its far names, and the nodes they rewrite --
#  {id(node): '_far' | '_near' | '_ss'} in a word of the text; `whole`: {id(node): shown(node)},
#  an expression written whole in another way; `bare`: those being printed as PrintC printed them)

# L2 primitive kinds -> the decompiler's names for them
_PRIM = {'i8': 'int1', 'u8': 'uint1', 'i16': 'int2', 'u16': 'uint2', 'i32': 'int4', 'u32': 'uint4',
         'i64': 'int8', 'u64': 'uint8', 'f32': 'float4', 'f64': 'float8', 'f80': 'float10',
         'bool': 'bool', 'char': 'char', 'void': 'void'}

# The core's operator tokens (printc.cc): (precedence, associative, kind). Brackets are placed by
# its rule (PrintLanguage::parentheses), not by C's grammar alone: an operand of the same
# precedence is bracketed unless it is the same associative operator -- `(a + b) - c`.
_BINARY = {',': 2, '||': 18, '&&': 22, '|': 26, '^': 30, '&': 34, '==': 38, '!=': 38,
           '<': 42, '<=': 42, '>': 42, '>=': 42, '<<': 46, '>>': 46,
           '+': 50, '-': 50, '*': 54, '/': 54, '%': 54}
_ASSOCIATIVE = {',', '|', '^', '&', '+', '*', '.', '->'}
_ASSIGN = 14
_PREFIX = 62                     # unary operators and casts
_POSTFIX = 66                    # calls, indexing, member access
_ATOM = 100

class Decline(Exception):
    """A node this back end does not write."""


# ---------------------------------------------------------------------------------------------- layout
class _Pretty:
    """The core's EmitPrettyPrint (prettyprint.cc) over its EmitNoMarkup: the same calls, the same
    queue of tokens and the same rule for where a line is broken (lines of 100 characters, an
    indent of 2, a comment's broken line filled with 3 spaces). A token can show other text than
    the room it is given: what a moved c_text pass adds, which c_text adds after PrintC's layout."""
    BEGIN, END, STRING, BREAK, INDENT, UNINDENT, COMMENT, UNCOMMENT = range(8)

    def __init__(self, width=100, indent=2, fill='   '):
        self.width, self.indent, self.fill = width, indent, fill
        self.toks = []             # [class, size, text, spaces, bump, absolute]
        self.lo = 0                # the first token not yet printed
        self.scanq = []            # tokens whose size is not yet known (groups opened, breaks)
        self.stack = []            # indentstack: the room left at each open group and indent
        self.room = width          # spaceremain
        self.left = self.right = 1
        self.needbreak = False
        self.commentmode = False
        self.out = []

    # -- the queue (EmitPrettyPrint::scan, advanceleft, print, overflow)
    def _push(self, cls, size=0, text='', spaces=0, bump=0, absolute=False):
        self.toks.append([cls, size, text, spaces, bump, absolute])
        self._scan()

    def _scan(self):
        ref = len(self.toks) - 1
        tok = self.toks[ref]
        c = tok[0]
        if c in (self.BEGIN, self.COMMENT):
            if not self.scanq:
                self.left = self.right = 1
            tok[1] = -self.right
            self.scanq.append(ref)
        elif c in (self.END, self.UNCOMMENT):
            tok[1] = 0
            if self.scanq:
                r = self.toks[self.scanq.pop()]
                r[1] += self.right
                if r[0] == self.BREAK and self.scanq:
                    self.toks[self.scanq.pop()][1] += self.right
                if not self.scanq:
                    self._advance()
        elif c == self.BREAK:
            if not self.scanq:
                self.left = self.right = 1
            else:
                r = self.toks[self.scanq[-1]]
                if r[0] == self.BREAK:
                    self.scanq.pop()
                    r[1] += self.right
            tok[1] = -self.right
            self.scanq.append(ref)
            self.right += tok[3]
        elif c in (self.INDENT, self.UNINDENT):
            tok[1] = 0
        elif self.scanq:                                 # a string
            self.right += tok[1]
            while self.right - self.left > self.room:
                self.toks[self.scanq.pop(0)][1] = 999999
                self._advance()
                if not self.scanq:
                    break

    def _advance(self):
        while self.lo < len(self.toks) and self.toks[self.lo][1] >= 0:
            tok = self.toks[self.lo]
            self._print(tok)
            if tok[0] == self.BREAK:
                self.left += tok[3]
            elif tok[0] == self.STRING:
                self.left += tok[1]
            self.lo += 1

    def _line(self):
        self.out.append('\n' + ' ' * (self.width - self.room))
        if self.commentmode and self.fill:
            self.out.append(self.fill)
            self.room -= len(self.fill)

    def _overflow(self):
        half = self.width // 2
        for i in range(len(self.stack) - 1, -1, -1):
            if self.stack[i] < half:
                self.stack[i] = half
            else:
                break
        room = self.stack[-1] if self.stack else self.width
        if room == self.room or self.commentmode and room == self.room + len(self.fill):
            return                                       # (a new line would give no more room)
        self.room = room
        self._line()

    def _print(self, tok):
        c = tok[0]
        if c == self.INDENT:
            self.stack.append(self.stack[-1] - tok[4])
        elif c in (self.BEGIN, self.COMMENT):
            self.commentmode = self.commentmode or c == self.COMMENT
            self.stack.append(self.room)
        elif c == self.UNINDENT:
            self.stack.pop()
        elif c in (self.END, self.UNCOMMENT):
            self.commentmode = self.commentmode and c != self.UNCOMMENT
            self.stack.pop()
        elif c == self.STRING:
            if tok[1] > self.room:
                self._overflow()
            self.out.append(tok[2])
            self.room -= tok[1]
        elif tok[1] > self.room:                         # a break where the line is full
            if tok[5]:
                self.room = self.width - tok[4]          # (a line at its own indent)
            else:
                val = self.stack[-1] - tok[4]
                if tok[3] <= self.room and val - self.room < 10:
                    self.out.append(' ' * tok[3])        # (breaking would gain too little)
                    self.room -= tok[3]
                    return
                self.stack[-1] = self.room = val
            self._line()
        else:
            self.out.append(' ' * tok[3])
            self.room -= tok[3]

    # -- the emitter's calls (EmitPrettyPrint's checkstart/checkstring/checkend/checkbreak)
    def _start(self):
        if self.needbreak:
            self._push(self.BREAK)
        self.needbreak = False

    def _string(self):
        if self.needbreak:
            self._push(self.BREAK)
        self.needbreak = True

    def _end(self):
        if not self.needbreak:
            self._push(self.STRING)
        self.needbreak = True

    def _break(self):
        if not self.needbreak:
            self._push(self.STRING)
        self.needbreak = False

    def print(self, text, shown=None):
        self._string()
        self._push(self.STRING, len(text), text if shown is None else shown)

    def open_group(self):
        self._start()
        self._push(self.BEGIN)

    def close_group(self):
        self._end()
        self._push(self.END)

    def open_paren(self, p):
        self.open_group()
        self._push(self.STRING, 1, p)
        self.needbreak = True

    def close_paren(self, p):
        self._string()
        self._push(self.STRING, 1, p)
        self.close_group()

    def spaces(self, n, bump=0):
        self._break()
        self._push(self.BREAK, 0, '', n, bump)

    def tag_line(self, indent=None):
        self._break()
        self._push(self.BREAK, 0, '', 999999, indent or 0, indent is not None)

    def start_indent(self):
        self._push(self.INDENT, 0, '', 0, self.indent)

    def stop_indent(self):
        self._push(self.UNINDENT)

    def start_comment(self):
        self._start()
        self._push(self.COMMENT)

    def stop_comment(self):
        self._end()
        self._push(self.UNCOMMENT)

    def text(self):
        for tok in self.toks[self.lo:]:                  # (flush)
            self._print(tok)
        self.lo = len(self.toks)
        return ''.join(self.out)


class _Flat:
    """The same calls, written on one line (an expression or a type on its own): the text shown,
    and beside it the text the layout gives room to (`raw`: as PrintC printed it)."""

    def __init__(self):
        self.out, self.raw = [], []

    def print(self, text, shown=None):
        self.out.append(text if shown is None else shown)
        self.raw.append(text)

    def open_paren(self, p):
        self.out.append(p)
        self.raw.append(p)

    close_paren = open_paren

    def spaces(self, n, bump=0):
        self.out.append(' ' * n)
        self.raw.append(' ' * n)

    def open_group(self):
        pass

    close_group = start_indent = stop_indent = start_comment = stop_comment = open_group

    def tag_line(self, indent=None):
        self.out.append('\n')
        self.raw.append('\n')

    def text(self):
        return ''.join(self.out)


def _flat(put, *args):
    em = _Flat()
    put(em, *args)
    return em.text()


# ---------------------------------------------------------------------------------------------- the core's operators
class _Op:
    """An OpToken (printc.cc): what it prints, its operands, precedence, kind, and the spacing
    around it (and the extra indent of a line broken there)."""
    __slots__ = ('print1', 'print2', 'stage', 'prec', 'assoc', 'kind', 'spacing', 'bump')

    def __init__(self, print1, print2, stage, prec, assoc, kind, spacing=0, bump=0):
        self.print1, self.print2, self.stage, self.prec = print1, print2, stage, prec
        self.assoc, self.kind, self.spacing, self.bump = assoc, kind, spacing, bump


_HIDDEN = _Op('', '', 1, 70, False, 'hiddenfunction')
_MEMBER = {'.': _Op('.', '', 2, 66, True, 'binary'), '->': _Op('->', '', 2, 66, True, 'binary')}
_SUBSCRIPT = _Op('[', ']', 2, 66, False, 'postsurround')
_CALL = _Op('(', ')', 2, 66, False, 'postsurround', 0, 10)
_UNARY = {op: _Op(op, '', 1, 62, False, 'unary_prefix') for op in ('~', '!', '-', '+', '&', '*')}
_CAST = _Op('(', ')', 2, 62, False, 'presurround')
_BINOPS = {op: _Op(op, '', 2, p, op in _ASSOCIATIVE, 'binary', 0 if op == ',' else 1) for op, p in _BINARY.items()}
_ASSIGNS = {}
_TYPE_SPACE = _Op('', '', 2, 10, False, 'space', 1)
_TYPE_NOSPACE = _Op('', '', 2, 10, False, 'space', 0)
_PTR = _Op('*', '', 1, 62, False, 'unary_prefix')
_ARRAY = _Op('[', ']', 2, 66, False, 'postsurround', 1)
_WHOLE = _Op('', '', 1, _ATOM, False, 'whole')           # (an expression placed as one token: _Stack.whole)


def _assign_op(op):
    word = (op or '') + '='
    if word not in _ASSIGNS:
        _ASSIGNS[word] = _Op(word, '', 2, _ASSIGN, False, 'binary', 1, 5)
    return _ASSIGNS[word]


class _Stack:
    """PrintLanguage's reverse-Polish stack: an expression pushed operator first, then each operand,
    as PrintC pushes it -- each operator a group of the emitter's, bracketed where
    PrintLanguage::parentheses says, its symbols placed as PrintLanguage::emitOp places them."""

    def __init__(self, em):
        self.em = em
        self.ops = []                                    # [op, operands placed, bracketed]

    def op(self, tok, paren=None):
        if not self.ops:
            paren = False
        else:
            self._emit(self.ops[-1])
            if paren is None:
                paren = self._parens(tok)
        if paren:
            self.em.open_paren('(')
        else:
            self.em.open_group()
        self.ops.append([tok, 0, paren])

    def atom(self, text, shown=None, blank=False):
        if self.ops:
            self._emit(self.ops[-1])
        if not blank:
            self.em.print(text, shown)
        while self.ops:
            top = self.ops[-1]
            top[1] += 1
            if top[1] != top[0].stage:
                break
            self._emit(top)
            if top[2]:
                self.em.close_paren(')')
            else:
                self.em.close_group()
            self.ops.pop()

    def whole(self, tok, raw, shown, paren=None):
        """An expression whose outermost operator is `tok` placed as one token: bracketed where
        that operator would be, given the room of its text as printed (`raw`), showing `shown`
        -- what a moved c_text pass writes in place of it, after the layout."""
        if tok is None:
            self.atom(raw, shown)
            return
        if not self.ops:
            paren = False
        else:
            self._emit(self.ops[-1])
            if paren is None:
                paren = self._parens(tok)
        if paren:
            self.em.open_paren('(')
        else:
            self.em.open_group()
        self.ops.append([_WHOLE, 0, paren])
        self.atom(raw, shown)

    def _parens(self, op2):
        """PrintLanguage::parentheses"""
        tok, stage = self.ops[-1][0], self.ops[-1][1]
        k = tok.kind
        if k == 'hiddenfunction':
            if stage == 0 and len(self.ops) > 1:
                prev = self.ops[-2][0]
                if prev.kind not in ('binary', 'unary_prefix') or prev.prec < op2.prec:
                    return False
            return True
        if k == 'postsurround' and stage == 1 or k == 'presurround' and stage == 0:
            return False                                 # (inside the surround)
        if tok.prec != op2.prec:
            return tok.prec > op2.prec
        if k in ('space', 'binary'):
            if tok.assoc and tok is op2:
                return False
            return not (op2.kind == 'postsurround' and stage == 0)
        if k in ('unary_prefix', 'presurround'):
            return op2.kind not in ('unary_prefix', 'presurround')
        return op2.kind not in ('postsurround', 'binary')

    def _emit(self, entry):
        """PrintLanguage::emitOp: the operator's symbols due before operand `entry[1]`."""
        tok, v, em = entry[0], entry[1], self.em
        k = tok.kind
        if k == 'binary' and v == 1:
            em.spaces(tok.spacing, tok.bump)
            em.print(tok.print1)
            em.spaces(tok.spacing, tok.bump)
        elif k == 'unary_prefix' and v == 0:
            em.print(tok.print1)
            em.spaces(tok.spacing, tok.bump)
        elif k == 'postsurround' and v == 1:
            em.spaces(tok.spacing, tok.bump)
            em.open_paren(tok.print1)
            em.spaces(0, tok.bump)
        elif k == 'postsurround' and v == 2:
            em.close_paren(tok.print2)
        elif k == 'presurround' and v == 0:
            em.open_paren(tok.print1)
        elif k == 'presurround' and v == 1:
            em.close_paren(tok.print2)
            em.spaces(tok.spacing, tok.bump)
        elif k == 'space' and v == 1:
            em.spaces(tok.spacing, tok.bump)


# ---------------------------------------------------------------------------------------------- types
def _stack(t):
    """(the modifiers of a type, outermost first: 'ptr' or ('array', count); its base type's name):
    PrintC::buildTypeStack, down to the first type with a name."""
    mods = []
    while True:
        if t is None:
            return mods, 'void'
        k = t['kind']
        if k in _PRIM:
            return mods, _PRIM[k]
        if k == 'class':
            return mods, t.get('name') or 'undefined'
        if k == 'record':
            return mods, t.get('name') or _record(t)
        if k == 'ptr':
            mods.append('ptr')
            t = t['to']
        elif k == 'fnptr':
            return mods + ['ptr'], 'code'
        elif k == 'fn':
            return mods, 'code'
        elif k == 'array':
            mods.append(('array', t['count'] if 'count' in t else ''))
            t = t['elem']
        elif k == 'selector':
            return mods, 'uint2'
        else:
            raise Decline('type kind %r' % k)


def _record(t):
    """An unnamed structure written out (the frame the decompiler keeps at its offsets)."""
    fields = ' '.join(type_text(f['type'], f.get('name', '_f%x' % f.get('offset', 0))) + ';'
                      for f in t.get('fields', []))
    return '%sstruct { %s }' % ('_Packed ' if t.get('packed') else '', fields)


def is_far(t):
    """A pointer of four bytes in a 16-bit function: selector and offset."""
    return bool(t) and t.get('kind') in ('ptr', 'fnptr') and t.get('space') == 'far'


def _push_type(st, t, name=None, far=False, word=None, shown_name=None):
    """A type with its name (a declaration) or without (a cast, a return type), on the stack:
    PrintC::pushTypeStart, the name or a blank, pushTypeEnd. `far`: the outer pointer written
    __far (c_text._far: `int2 __far *p`); `word`: another word there (` __near`); `shown_name`:
    what is shown for the name (`unaff_SS = codebreaker_ss()`)."""
    mods, base = _stack(t)
    st.op(_TYPE_NOSPACE if name is None and not mods else _TYPE_SPACE)
    word = ' __far' if far else word
    shown = base + word if word and mods and all(m == 'ptr' for m in mods) else None
    st.atom(base, shown)
    for m in reversed(mods):
        st.op(_PTR if m == 'ptr' else _ARRAY)
    if name is None:
        st.atom('', blank=True)
    else:
        st.atom(name, shown_name)
    for m in mods:
        if m != 'ptr':
            st.atom(str(m[1]))


def _put_type(em, t, name=None, far=False, shown_name=None):
    st = _Stack(em)
    _push_type(st, t, name, far, shown_name=shown_name)


def type_text(t, name='', far=False):
    """A declaration of `name` with type `t` (`int2 *p`, `xunknown1 a [22]` -- PrintC's spacing,
    which c_text's passes read), or the type alone. `far`: the outer pointer written __far."""
    if t is None:
        return ('void ' + name).strip()
    return _flat(_put_type, t, name or None, far)


# ---------------------------------------------------------------------------------------------- expressions
def _natural_base(v):
    """10 or 16: the base a number reads most naturally in (the core's
    PrintLanguage::mostNaturalBase -- a run of 0s or 9s at its end in decimal against a run of
    0s or Fs in hexadecimal: 500 and 1000 are decimal, 0x400 and 0xfff hexadecimal)."""
    dec, tmp = 0, v
    setdig = tmp % 10
    if setdig in (0, 9):
        dec, tmp = 1, tmp // 10
        while tmp and tmp % 10 == setdig:
            dec, tmp = dec + 1, tmp // 10
    if dec == 0:
        return 16
    if dec == 1 and (tmp > 1 or setdig == 9) or dec == 2 and tmp > 10 or dec in (3, 4) and tmp > 100 \
            or dec > 4 and tmp > 1000:
        return 16
    hexes, tmp = 0, v
    setdig = tmp & 0xF
    if setdig in (0, 0xF):
        hexes, tmp = 1, tmp >> 4
        while tmp and tmp & 0xF == setdig:
            hexes, tmp = hexes + 1, tmp >> 4
    return 10 if dec > hexes else 16


def _int(v):
    """An integer as the decompiler prints it (PrintC::push_integer): up to 10 in decimal, past it
    in the base it reads most naturally in."""
    if isinstance(v, bool):
        return 'true' if v else 'false'
    sign, m = ('-', -v) if v < 0 else ('', v)
    if m <= 10 or _natural_base(m) == 10:
        return sign + str(m)
    return sign + '0x%x' % m


def _char(v):
    if v == 0:
        return "'\\0'"
    special = {7: '\\a', 8: '\\b', 9: '\\t', 10: '\\n', 11: '\\v', 12: '\\f', 13: '\\r', 39: "\\'", 92: '\\\\'}
    if v in special:
        return "'%s'" % special[v]
    if 32 <= v < 127:
        return "'%s'" % chr(v)
    return "'\\x%02x'" % (v & 0xFF)


def _lit(e):
    if e.get('text'):
        return e['text']                         # as the decompiler spelled it
    v = e.get('value')
    t = e.get('type') or {}
    if isinstance(v, str):
        return '"%s"' % v.replace('\\', '\\\\').replace('"', '\\"')
    if isinstance(v, float):
        return repr(v)
    if t.get('kind') == 'char' and isinstance(v, int):
        # past 0x7f a byte is no character (PrintC::pushCharConstant): a signed one is a number
        return _char(v) if 0 <= v < 0x80 else _int(v - 0x100 if v >= 0x80 else v)
    if t.get('kind') in ('ptr', 'fnptr') and isinstance(v, int) and v >= 0:
        return '0x%x' % v                        # an address is always hexadecimal
    if v is None:
        raise Decline('literal without a value')
    return _int(v)


def _name(e):
    sym = e.get('sym') or {}
    return sym.get('name') or e.get('name') or '?'


def _token(e):
    """(precedence, operator, kind) of an expression's outermost token; kind is the core's
    OpToken type: binary, unary_prefix, presurround (a cast), postsurround (call, index), atom."""
    n = e['node']
    if n == 'Binary':
        return _BINARY[e['op']], e['op'], 'binary'
    if n == 'Assign':
        return _ASSIGN, (e.get('op') or '') + '=', 'binary'
    if n == 'Unary':
        return (_POSTFIX, e['op'], 'postsurround') if e.get('postfix') else (_PREFIX, e['op'], 'unary_prefix')
    if n == 'Cast':
        return _PREFIX, 'cast', 'presurround'
    if n in ('Call', 'Index'):
        return _POSTFIX, n, 'postsurround'
    if n == 'Select':
        return _POSTFIX, '->' if _arrow(e) else '.', 'binary'
    if n == 'Cond':
        return 16, '?', 'binary'
    return _ATOM, None, 'atom'


def _bracket(parent, child, stage):
    """Does `child`, the operand at `stage` (0 = first printed) of `parent`, need brackets?
    (PrintLanguage::parentheses)"""
    pp, pop, pkind = parent
    cp, cop, ckind = _token(child)
    if ckind == 'atom':
        return False
    if pkind == 'binary':
        if pp != cp:
            return pp > cp
        if pop in _ASSOCIATIVE and pop == cop:
            return False
        return not (ckind == 'postsurround' and stage == 0)
    if pkind in ('unary_prefix', 'presurround'):
        if pp != cp:
            return pp > cp
        return ckind not in ('unary_prefix', 'presurround')
    if pkind == 'postsurround':
        if stage == 1:
            return False                         # inside the brackets: an argument, an index
        if pp != cp:
            return pp > cp
        return ckind not in ('postsurround', 'binary')
    return False


def _arrow(e):
    obj = e['obj']
    return obj.get('node') == 'Unary' and obj.get('op') == '*' and not obj.get('postfix')


def expr(e):
    """C text of an expression."""
    return _flat(_put, e)


def _sub(parent, child, stage):
    """An operand's text as it is written there, in brackets where it takes them."""
    return '(%s)' % expr(child) if _bracket(parent, child, stage) else expr(child)


_WORDS = ('xunknown2', 'int2', 'uint2', 'undefined2')


def _near_call(e):
    """`(*(code *)*(xunknown2 *)0x18)` -- a call through a word, `call word ptr [18h]` -- is near:
    into the function's own code segment. `code *` is far in the model the 16-bit C is built in,
    and a word made into one gets segment 0 (c_text._near_calls: MSMOUS31's Disable took a GP so).
    Marks the cast to be written `code __near *`."""
    if 'near_calls' not in _now['moved'] or e.get('node') != 'Unary' or e.get('op') != '*' or e.get('postfix'):
        return
    cast = e['x']
    if cast.get('node') != 'Cast' or (cast.get('type') or {}).get('kind') != 'fnptr':
        return
    word = cast['x']
    if word.get('node') == 'Unary' and word.get('op') == '*' and not word.get('postfix') and \
            word['x'].get('node') == 'Cast' and type_text(word['x']['type']) in tuple(w + ' *' for w in _WORDS):
        _now['marks'][id(cast)] = '_near'


def _leading_cast(e):
    """The cast a printed expression starts with (`(T *)x + 2`), as ctree.Source.leading finds it:
    following the first operand down while nothing is printed before it."""
    while True:
        n = e.get('node')
        if n == 'Cast':
            return e
        tok = _token(e)
        first = {'Binary': 'left', 'Assign': 'target', 'Cond': 'cond', 'Call': 'target', 'Index': 'arr',
                 'Select': 'obj'}.get(n)
        if n == 'Unary' and e.get('postfix'):
            first = 'x'
        if not first or not e.get(first):
            return None
        child = e[first]
        if n == 'Select' and child.get('node') == 'Unary' and child.get('op') == '*' and not child.get('postfix'):
            child = child['x']                           # (`p->f`: p comes first)
        if _bracket(tok, child, 0):
            return None                                  # (a bracket comes first)
        e = child


def _data_address(e):
    """A constant address in the data segment, a number of one word but 0: a far pointer set to
    one is DS and it, written MK_FP(codebreaker_ds(), n) (c_text.data_address: LZEXPAND's LZDONE)."""
    if e.get('node') != 'Lit':
        return False
    m = re.fullmatch(r"(0x[0-9a-fA-F]+|\d+)[uUlL]*", _lit(e))     # (as it is written: no character)
    return bool(m) and 0 < int(m.group(1), 0) <= 0xFFFF


def _assigned_name(target):
    """`x` of `x = ...`, `*x = ...`, `**x = ...`"""
    while target.get('node') == 'Unary' and target.get('op') == '*' and not target.get('postfix'):
        target = target['x']
    return (target.get('sym') or {}).get('name') if target.get('node') == 'Name' else None


def _put(em, e):
    """An expression, the root of its statement, onto the emitter."""
    st = _Stack(em)
    _push(st, e)
    if st.ops:
        raise Decline('expression left unfinished')


def _operands(e, tok, kids):
    """Marks the casts of calls through a word among the bracketed operands (as they are written)."""
    for stage, child in enumerate(kids):
        if child is not None and _bracket(tok, child, stage):
            _near_call(child)


def _op_of(e):
    """The core's operator token an expression is printed with on the outside (None: an atom)."""
    n = e['node']
    if n == 'Binary':
        return _BINOPS[e['op']]
    if n == 'Assign':
        return _assign_op(e.get('op'))
    if n == 'Unary':
        return _UNARY.get(e['op'])
    if n == 'Select':
        return _MEMBER['->' if _arrow(e) else '.']
    return {'Cast': _CAST, 'Call': _CALL, 'Index': _SUBSCRIPT, 'Paren': _HIDDEN}.get(n)


def _raw(e):
    """An expression's text as PrintC printed it, the moved passes' rewrites of it and in it left out."""
    _now['bare'].add(id(e))
    try:
        em = _Flat()
        _put(em, e)
        return ''.join(em.raw)
    finally:
        _now['bare'].discard(id(e))


def _rewrite(e, shown):
    """A moved pass writes `shown(e)` in place of expression `e` (the first pass to claim it)."""
    _now['whole'].setdefault(id(e), shown)


def _push(st, e, paren=None):
    """One expression on the stack, as PrintC pushes it: the operator, then its operands in the
    order they are printed."""
    n = e['node']
    rewrite = _now['whole'].get(id(e))
    if rewrite is not None and id(e) not in _now['bare']:
        # (a moved pass's text in its place: given the room of the text PrintC printed, as
        # c_text rewrites the printed C after its layout)
        st.whole(_op_of(e), _raw(e), rewrite(e), paren)
        return
    if n == 'Lit':
        st.atom(_lit(e))
        return
    if n == 'Name':
        name = _name(e)
        st.atom(name, 'MK_FP(codebreaker_ss(), %s)' % name if _now['marks'].get(id(e)) == '_ss' else None)
        return
    if n == 'Paren':                                     # a hidden function's operand, bracketed
        _near_call(e['x'])
        st.op(_HIDDEN, paren)
        _push(st, e['x'], True)
        return
    tok = _token(e)
    if n == 'Cast':
        _operands(e, tok, (None, e['x']))
        mark = _now['marks'].get(id(e))
        st.op(_CAST, paren)
        if mark == '_near':                              # (`code __near *`: c_text._near_calls)
            _push_type(st, e['type'], word=' __near')
        else:
            _push_type(st, e['type'], far=mark in ('_far', '_ds'))
        if mark == '_ds':                                # (a data address: DS and it -- _data_address;
            n = _lit(e['x'])                             #  added after the lines are laid out, as c_text does)
            st.atom(n, 'MK_FP(codebreaker_ds(), %s)' % n)
            return
        _push(st, e['x'])
        return
    if n == 'Unary':
        if e.get('postfix') or e['op'] not in _UNARY:
            raise Decline('unary %s' % e['op'])
        _operands(e, tok, (e['x'],))
        st.op(_UNARY[e['op']], paren)
        _push(st, e['x'])
        return
    if n == 'Binary':
        _operands(e, tok, (e['left'], e['right']))
        st.op(_BINOPS[e['op']], paren)
        _push(st, e['left'])
        _push(st, e['right'])
        return
    if n == 'Assign':
        if 'far' in _now['moved'] and not e.get('op') and _assigned_name(e['target']) in _now['far']:
            cast = _leading_cast(e['value'])             # `p = (T *)x` -> `p = (T __far *)x` (c_text._far)
            if cast is not None and (cast.get('type') or {}).get('kind') in ('ptr', 'fnptr'):
                _now['marks'][id(cast)] = '_ds' if _data_address(cast['x']) else '_far'
        _operands(e, tok, (e['target'], e['value']))
        st.op(_assign_op(e.get('op')), paren)
        _push(st, e['target'])
        _push(st, e['value'])
        return
    if n == 'Call':
        target = e.get('target')
        args = e.get('args') or []
        _operands(e, tok, (target,))
        st.op(_CALL, paren)
        if target:
            _push(st, target)
        else:
            st.atom((e.get('sym') or {}).get('name', '?'))
        if not args:
            st.atom('', blank=True)                      # f(): nothing between the brackets
        for _ in args[1:]:
            st.op(_BINOPS[','])                          # (the arguments as PrintC joins them)
        for a in args:
            _push(st, a)
        return
    if n == 'Index':
        _operands(e, tok, (e['arr'], e['index']))
        st.op(_SUBSCRIPT, paren)
        _push(st, e['arr'])
        _push(st, e['index'])
        return
    if n == 'Select':
        obj = e['obj']['x'] if _arrow(e) else e['obj']
        _operands(e, tok, (obj,))
        st.op(_MEMBER['->' if _arrow(e) else '.'], paren)
        _push(st, obj)
        st.atom(_name(e))
        return
    raise Decline('expression %s' % n)


# ---------------------------------------------------------------------------------------------- conditions
def _joined(e):
    """A condition the structure joined (PrintL2 marks one so: both sides in Paren)."""
    return e.get('node') == 'Binary' and e.get('op') in ('&&', '||') and \
        e['left'].get('node') == 'Paren' and e['right'].get('node') == 'Paren'


def _cond(em, e, comma):
    """A branch's condition as PrintC writes it (emitBlockCondition, opCbranch): the test of one
    block in brackets of its own -- or, in a loop's comma list (`comma`: comma_separate), each
    statement of the block and the test, `a, b`, the test in none; a condition the structure
    joined bracketed whole, its right side in brackets and in comma-list mode."""
    if _joined(e):
        em.open_paren('(')
        _cond(em, e['left']['x'], comma)
        em.spaces(1)
        em.print(e['op'])
        em.spaces(1)
        em.open_paren('(')
        _cond(em, e['right']['x'], True)
        em.close_paren(')')
        em.close_paren(')')
        return
    if not comma:                                        # (only the branch: opCbranch's brackets)
        em.open_paren('(')
        _put(em, e)
        em.close_paren(')')
        return
    items = _listed(e)
    for i, x in enumerate(items):
        if i:
            em.print(',')
            em.spaces(1)
        em.open_group()                                  # (each a statement: emitStatement)
        if i == len(items) - 1:
            em.open_group()                              # (the test: opCbranch, unbracketed)
            _put(em, x)
            em.close_group()
        else:
            _put(em, x)
        em.close_group()


def condition(e, inner=False):
    """A branch's condition as PrintC brackets it (emitBlockCondition, opCbranch): each condition
    in brackets of its own, but not one in a comma list (a loop's) or on the right of a joining
    && / ||, which the join brackets instead -- `if ((a) && (b))`, `if (((a) && (b)) && ((c || (d))))`."""
    return _flat(_cond, e, inner)


def _listed(e):
    """A condition block's statements and its test, joined by PrintL2 with `,` (PrintC writes them
    `a, b`: statements, not the comma operator, which it writes `a,b`)."""
    if e.get('node') == 'Binary' and e.get('op') == ',':
        return _listed(e['left']) + [e['right']]
    return [e]


# ---------------------------------------------------------------------------------------------- statements
def _true(e):
    return e.get('node') == 'Lit' and e.get('value') is True


def _stmts(s):
    if s is None:
        return []
    return s if isinstance(s, list) else [s]


def _block(em, stmts):
    for s in _stmts(stmts):
        _stmt(em, s)


def _braced(em, stmts):
    """`{` on the line, the statements indented, `}` on a line of its own (openBraceIndent,
    closeBraceIndent)."""
    em.spaces(1)
    em.start_indent()
    em.print('{')
    _block(em, stmts)
    em.stop_indent()
    em.tag_line()
    em.print('}')


def _comment(em, text, indent):
    """PrintLanguage::emitLineComment: `/* text */` on a line at `indent`, its words as the
    core splits them, so a long one is broken as PrintC breaks it."""
    em.tag_line(indent)
    em.start_comment()
    em.print('/* ')
    for m in re.finditer(r"[ \t]+|\n|\r|\{@[^}]*\}?|[^\s]+", text):
        w = m.group(0)
        if w[0] in ' \t':
            em.spaces(len(w))
        elif w == '\n':
            em.tag_line()
        elif w != '\r':
            em.print(w)
    em.print(' */')
    em.stop_comment()


def _jump(em, s):
    """PrintC::emitGotoStatement: `goto x;`, `break;`, `continue;` as one statement."""
    em.open_group()
    n = s['node']
    if n == 'Goto':
        em.print('goto')
        em.spaces(1)
        em.print(s['label'])
    elif n in ('Break', 'Continue'):
        em.print(n.lower())
    else:
        raise Decline('jump %s' % n)
    em.print(';')
    em.close_group()


def _while_true(em):
    em.open_paren('(')
    em.spaces(1)
    em.print('true')
    em.spaces(1)
    em.close_paren(')')


def _if(em, s, merged=False):
    """PrintC::emitBlockIf: `if (c) goto x;` braceless where the then-part is one statement, not a
    block; an else holding one if and nothing else merged into `else if`."""
    if merged:
        em.spaces(1)
    else:
        em.tag_line()
    em.print('if')
    em.spaces(1)
    _cond(em, s['cond'], False)
    if isinstance(s.get('then'), dict):
        em.spaces(1)
        _jump(em, s['then'])
        return
    _braced(em, s.get('then'))
    orelse = s.get('orelse')
    if orelse is None:
        return
    em.tag_line()
    em.print('else')
    if isinstance(orelse, list) and len(orelse) == 1 and orelse[0]['node'] == 'If':
        _if(em, orelse[0], True)
        return
    _braced(em, orelse)                                  # (an else with nothing in it is still written)


def _statement(em, e, word=None):
    """PrintC::emitStatement: one expression as a statement (a group of its own), then `;`."""
    em.tag_line()
    em.open_group()
    if word:
        em.print(word)
        if e is not None:
            em.spaces(1)
    if e is not None:
        _put(em, e)
    em.close_group()
    em.print(';')


def _stmt(em, s):
    n = s['node']
    if n == 'Local':
        return                                   # declared at the top, as the decompiler does
    if n == 'ExprStmt':
        _statement(em, s['expr'])
    elif n == 'Return':
        _statement(em, s.get('expr'), 'return')
    elif n == 'If':
        _if(em, s)
    elif n == 'While':
        em.tag_line()
        em.print('while')
        if _true(s['cond']):
            _while_true(em)                      # PrintC writes the endless one `while( true )`
        else:
            em.spaces(1)
            em.open_paren('(')
            _cond(em, s['cond'], True)
            em.close_paren(')')
        _braced(em, s.get('body'))
    elif n == 'DoWhile':
        em.tag_line()
        em.print('do')
        _braced(em, s.get('body'))
        em.spaces(1)
        em.print('while')
        if _true(s['cond']):
            _while_true(em)
        else:
            em.spaces(1)
            _cond(em, s['cond'], False)
        em.print(';')
    elif n == 'For':
        em.tag_line()
        em.print('for')
        em.spaces(1)
        em.open_paren('(')
        for i, x in enumerate(_stmts(s.get('init'))):
            if i:
                em.print(';')
                em.spaces(1)
            em.open_group()
            _put(em, x['expr'] if x.get('node') == 'ExprStmt' else x)
            em.close_group()
        em.print(';')
        em.spaces(1)
        if s.get('cond'):
            _cond(em, s['cond'], True)
        em.print(';')
        em.spaces(1)
        for i, x in enumerate(_stmts(s.get('update'))):
            if i:
                em.print(',')
            em.open_group()
            _put(em, x['expr'] if x.get('node') == 'ExprStmt' else x)
            em.close_group()
        em.close_paren(')')
        _braced(em, s.get('body'))
    elif n == 'Switch':
        em.tag_line()
        em.print('switch')
        em.open_paren('(')
        _put(em, s['expr'])
        em.close_paren(')')
        em.spaces(1)
        em.print('{')
        for c in _stmts(s.get('cases')):
            if c.get('default'):
                em.tag_line()
                em.print('default')
                em.print(':')
            else:
                for label in _stmts(c.get('labels')):
                    em.tag_line()
                    em.print('case')
                    em.spaces(1)
                    _put(em, label)
                    em.print(':')
            em.start_indent()
            _block(em, c.get('body'))
            em.stop_indent()
        em.tag_line()
        em.print('}')
    elif n in ('Goto', 'Break', 'Continue'):
        em.tag_line()
        _jump(em, s)
    elif n == 'Labeled':
        em.tag_line(0)
        em.print(s['label'])
        em.print(':')
        _block(em, s.get('body'))
    elif n == 'Block':
        _block(em, s.get('stmts'))
    elif n == 'Comment':
        _comment(em, s['text'], 20)              # (PrintC's line_commentindent)
    elif n == 'Empty':
        pass
    else:
        raise Decline('statement %s' % n)


def _locals(stmts, found):
    """Every Local of the body, in order, wherever it sits (the decompiler declares them all first)."""
    for s in _stmts(stmts):
        if not isinstance(s, dict):
            continue
        if s.get('node') == 'Local':
            found.append(s)
            continue
        for key in ('then', 'orelse', 'body', 'stmts', 'cases'):
            if key in s:
                _locals(s[key], found)
    return found


# ---------------------------------------------------------------------------------------------- far from the frame
_FRAME = re.compile(r"\w*Stack_[0-9a-f]+")


def _sets(stmts, found):
    """(variable, right side) of each `v = ...;` statement of a line of its own, wherever it sits."""
    for s in _stmts(stmts):
        if not isinstance(s, dict):
            continue
        n = s.get('node')
        if n == 'ExprStmt':
            a = s['expr']
            if a.get('node') == 'Assign' and not a.get('op') and a['target'].get('node') == 'Name':
                found.append((_name(a['target']), a['value']))
            continue
        if n == 'If' and isinstance(s.get('then'), dict):
            _sets(s.get('orelse'), found)                # (`if (c) goto x;`: one line with its if)
            continue
        for key in ('then', 'orelse', 'body', 'stmts'):
            if key in s:
                _sets(s[key], found)
        for c in _stmts(s.get('cases')):
            _sets(c.get('body'), found)
    return found


def _origin(e):
    """(the variable a pointer is set from, whether its address was taken): `x`, `&x`, `(T *)x`,
    `(T *)&x`, each `+ n` -- or (None, False). As the C reads: a bracketed operand is none of these."""
    if e.get('node') == 'Binary' and e['op'] == '+' and not _bracket(_token(e), e['left'], 0) and \
            re.fullmatch(r"[\w()]+", _sub(_token(e), e['right'], 1)):
        e = e['left']
    if e.get('node') == 'Cast' and re.fullmatch(r"[\w ]+\*", type_text(e['type'])) and not _bracket(_token(e), e['x'], 1):
        e = e['x']
    amp = False
    if e.get('node') == 'Unary' and e['op'] == '&' and not e.get('postfix') and not _bracket(_token(e), e['x'], 1):
        e, amp = e['x'], True
    return (_name(e), amp) if e.get('node') == 'Name' else (None, False)


def _far_from_stack(body, locs):
    """A far pointer set from a near one into the frame: the original made it with SS (`push ss;
    pop es; rep movsw` -- USER's WM_WINDOWPOSCHANGED copies each WINDOWPOS into its frame so). C
    makes a far pointer of a near one with DS, and the bytes went to the frame's offset in the data
    segment, over whatever lay there (c_text._far_from_stack). Marks each such right side to be
    written `MK_FP(codebreaker_ss(), p)`."""
    sets = _sets(body, [])
    # (an address in the frame: an array local, or & of a local -- not the value a local holds)
    arrays = _frame_arrays(locs)
    far = _now['far']

    def in_frame(found, stack):
        name, amp = found
        return name is not None and bool((amp and _FRAME.fullmatch(name)) or name in arrays or
                                         (not amp and name in stack))
    stack = set()
    for _ in range(3):                                   # (pointers set from those, in turn)
        for v, rhs in sets:
            if v not in far and in_frame(_origin(rhs), stack):
                stack.add(v)
    # (one set from anything else too is not known to be in the frame -- nor one set from it)
    dropped = True
    while dropped:
        dropped = False
        for v in sorted(stack):
            if any(not in_frame(_origin(rhs), stack) for w, rhs in sets if w == v):
                stack.discard(v)
                dropped = True
    for v, rhs in sets:
        if v in far and rhs.get('node') == 'Name' and _name(rhs) in stack:
            _now['marks'][id(rhs)] = '_ss'


def _frame_arrays(locs):
    """The names of the buffers in the frame: array locals (`xunknown1 axStack_4c [52];`)."""
    return {(v.get('sym') or {}).get('name') for v in locs
            if (v.get('type') or {}).get('kind') == 'array' and 'count' in v['type']
            and v['type']['elem'].get('kind') != 'array' and _FRAME.fullmatch((v.get('sym') or {}).get('name') or '')}


# ---------------------------------------------------------------------------------------------- the passes on the tree
def _walk(x, parent=None):
    """(node, its parent) for every node under `x`, in the order they are printed."""
    if isinstance(x, list):
        for y in x:
            yield from _walk(y, parent)
    elif isinstance(x, dict) and 'node' in x:
        yield x, parent
        for k, v in x.items():
            if isinstance(v, (dict, list)) and k not in ('type', 'sym', 'ret'):
                yield from _walk(v, x)


_C_TYPES = {'char', 'short', 'int', 'long', 'float', 'double', 'void', 'signed', 'unsigned', 'bool'}


def _terms(e):
    """The terms of a sum written without brackets (`a + b + c`), in order."""
    if e.get('node') != 'Binary' or e.get('op') != '+':
        return [e]
    tok = _token(e)
    return [x for k, side in enumerate((e['left'], e['right']))
            for x in ([side] if _bracket(tok, side, k) else _terms(side))]


def _frame_through_ss(nodes, arrays):
    """A buffer in the frame reached through its address as a number -- `*(xunknown2 *)((int2)
    axStack_a8 + iVar1)`, `mov ss:[bx], ax` (KEYBOARD's NEWTABLEEX): a near pointer made of a
    number is read through DS, so the cast is written with SS, `(xunknown2 *)MK_FP(
    codebreaker_ss(),(int2)axStack_a8 + iVar1)` (c_text._frame_through_ss)."""
    def in_frame(n):
        """`(int2)axStack_a8` or `(int2)&iStack_6`: a frame address as a number"""
        if n.get('node') != 'Cast' or type_text(n.get('type')) not in ('int2', 'uint2'):
            return False
        v = n['x']
        if v.get('node') == 'Unary' and v.get('op') == '&' and not v.get('postfix'):
            return v['x'].get('node') == 'Name' and _FRAME.fullmatch(_name(v['x'])) is not None
        return v.get('node') == 'Name' and _name(v) in arrays
    for e, parent in nodes:
        if e['node'] != 'Unary' or e.get('op') != '*' or e.get('postfix') or e['x'].get('node') != 'Cast':
            continue
        if parent is not None and parent.get('node') == 'Cast' and \
                re.fullmatch(r"\w+", type_text(parent.get('type'))) and type_text(parent.get('type')) not in _C_TYPES:
            # (`(int4)*(int2 *)(...)`: a name of a type not C's own, so the C's parser reads
            # `(int4) * ...` as a product -- c_text never saw a dereference there, and left it as
            # the core printed it: GDI's sub_35_0669 reads its frame so, through DS)
            continue
        cast = e['x']
        ty = re.fullmatch(r"([\w ]+?) ?\*", type_text(cast.get('type')))
        sum_ = cast['x']['x'] if cast['x'].get('node') == 'Paren' else cast['x']
        if ty is None or sum_.get('node') != 'Binary' or sum_.get('op') != '+':
            continue
        # (the sum as the C reads it: `a + b + c` is (a + b) + c whatever the tree's grouping,
        # so of three terms or more only the last is a side of its own)
        terms = _terms(sum_)
        if not (in_frame(terms[-1]) or len(terms) == 2 and in_frame(terms[0])):
            continue
        _rewrite(cast, lambda c, t=ty.group(1), s=sum_: "(%s *)MK_FP(codebreaker_ss(),%s)" % (t, expr(s)))


_PIECE = re.compile(r"_(\d+)_(\d+)_")
_ORDERING = ('<', '>', '<=', '>=')


def _pieces(nodes, _arrays):
    """`x._2_2_` is the decompiler's "2 bytes of x from byte 2": written as the bytes at that
    place, `(*(uint2 *)((char *)&x + 2))` -- unsigned, but signed as one side of an ordering
    comparison, where the core printed its signed compare with no cast (USER's sub_10_2F28;
    c_text._pieces). A piece of a piece: the inner one inside."""
    parent = {id(e): p for e, p in nodes}
    for e, p in nodes:
        m = _PIECE.fullmatch(_name(e)) if e['node'] == 'Select' and not _arrow(e) else None
        if not m:
            continue
        while p is not None and p['node'] == 'Paren':
            p = parent.get(id(p))
        if not _now['sixteen']:                          # (32-bit: by its size alone, c_text.compilable32)
            table, other = _BY_SIZE, 'uint4'
        else:
            table = _SIGNED if p is not None and p['node'] == 'Binary' and p['op'] in _ORDERING else _SIZED
            other = 'uint2'
        _rewrite(e, lambda s, off=m.group(1), ty=table.get(int(m.group(2)), other):
                 "(*(%s *)((char *)&%s + %s))" % (ty, _sub(_token(s), s['obj'], 0), off))


_SIZED = {1: 'uint1', 2: 'uint2', 4: 'uint4'}
_SIGNED = {1: 'int1', 2: 'int2', 4: 'int4'}
_BY_SIZE = {1: 'uint1', 2: 'uint2', 4: 'uint4', 8: 'uint8'}

def _null_is_zero(nodes, _arrays):
    """`x == (int4 *)0x0` is `x == 0` whatever x is; with the cast it does not compile where x is
    a number (c_text._null_is_zero)."""
    for e, _p in nodes:
        if e['node'] == 'Binary' and e['op'] in ('==', '!='):
            r = e['right']
            if r.get('node') == 'Cast' and (r.get('type') or {}).get('kind') in ('ptr', 'fnptr') and \
                    r['x'].get('node') == 'Lit' and _lit(r['x']) == '0x0':
                _rewrite(r, lambda _r: '0')


_POINTER_GLOBAL = re.compile(r"p+[a-z]*Ram[0-9a-f]{8}")


def _last_name(e):
    """The name an expression's text ends with (`x - pRam00001234`: pRam00001234), and what is
    printed just before it ('*' of `*p`, ')' of `(T)p`, else ' '); or (None, None)."""
    before = ' '
    while True:
        n = e.get('node')
        if n == 'Name':
            return e, before
        tok = _token(e)
        if n == 'Binary':
            kid, before = e['right'], ' '
        elif n == 'Assign':
            kid, before = e['value'], ' '
        elif n == 'Unary' and not e.get('postfix'):
            kid, before = e['x'], e['op']
        elif n == 'Cast':
            kid, before = e['x'], ')'
        else:
            return None, None
        if _bracket(tok, kid, 1 if n != 'Unary' else 0):
            return None, None                            # (a bracket comes last)
        e = kid


def _global_pointer_steps(nodes, _arrays):
    """`pRam00001234 + 2` -- a step from a global pointer: in bytes, as the core counts it, so
    written `(uint1 *)pRam00001234 + 2` (c_text._global_pointer_steps, which rewrites a name of
    the kind followed by ` + ` where nothing but a blank or an operator other than `*` is before it)."""
    for e, _p in nodes:
        if e['node'] == 'Select' and id(e) in _now['whole'] and _PIECE.fullmatch(_name(e)):
            # (a piece is written `(char *)&x + 2`: x, too, is followed by ` + `)
            obj = e['obj']
            if _bracket(_token(e), obj, 0):
                continue
            last, before = (obj, '&') if obj.get('node') == 'Name' else _last_name(obj)
        elif e['node'] != 'Binary' or e['op'] != '+' or _bracket(_token(e), e['left'], 0):
            continue
        else:
            last, before = _last_name(e['left'])
        if last is not None and before not in ('*', ')') and _POINTER_GLOBAL.fullmatch(_name(last)):
            _rewrite(last, lambda x: '(uint1 *)' + _name(x))


_SECOND_NAME = re.compile(r"_[a-z]+Ram[0-9a-f]{8}")


def _one_name_for_globals(nodes, _arrays):
    """`_iRam0040c000` is the decompiler's second name for the global iRam0040c000 (read at another
    width): the global (c_text._one_name_for_globals)."""
    for e, _p in nodes:
        if e['node'] == 'Name' and _SECOND_NAME.fullmatch(_name(e)):
            _rewrite(e, lambda x: _name(x)[1:])


# (each: c_text's pass, done on the tree -- in the order c_text runs them)
_PASSES = (("frame_through_ss", _frame_through_ss), ("pieces", _pieces), ("null_is_zero", _null_is_zero),
           ("global_pointer_steps", _global_pointer_steps), ("one_name_for_globals", _one_name_for_globals))


# ---------------------------------------------------------------------------------------------- functions
def _param_name(p):
    return (p.get('sym') or {}).get('name') or p.get('name', '')


def _signature(em, doc):
    """PrintC::emitFunctionDeclaration: the result's type, the convention's word where it is not
    the module's default (`__stdcall16near`), the name, the parameters."""
    sym = doc.get('sym') or {}
    ft = sym.get('type') or {}
    em.open_group()                                      # (the prototype)
    em.open_group()                                      # (its result)
    ret = doc.get('ret') or (ft.get('result') or {}).get('type') or {'kind': 'void'}
    far = 'far_result' in _now['moved'] and is_far(ret)
    if far and _stack(ret)[0] != ['ptr']:
        raise Decline('a far result of a pointer to a pointer')      # (c_text._far_result writes `T *__far *`)
    _put_type(em, ret, far=far)                          # (a far pointer: `T __far *`, c_text._far_result)
    em.close_group()
    em.spaces(1)
    for m in doc.get('mods') or ():
        em.print(m)
        em.spaces(1)
    em.open_group()
    em.print(doc['name'])
    em.spaces(0, 10)
    em.open_paren('(')
    em.spaces(0, 10)
    params = doc.get('params') or []
    if not params:
        em.print('void')
    for i, p in enumerate(params):
        if i:
            em.print(',')
        em.open_group()                                  # (each a declaration)
        _put_type(em, p.get('type'), _param_name(p), _param_name(p) in _now['far'])
        em.close_group()
    if ft.get('variadic'):
        if params:
            em.print(',')
        em.print('...')
    em.close_paren(')')
    em.close_group()
    em.close_group()


def signature(doc):
    """`int2 __cdecl16far sub_1_0000(void)`"""
    return _flat(_signature, doc)


def function(doc, moved=(), far=None):
    """C text of one function document (a MethodDecl), with the c_text passes `moved` done here.
    `far`: the names of its four-byte pointers where the decompiler said them apart from the types
    (its /*@P name 4 */ notes, which c_text goes by: a variable can be four bytes wide and typed as
    a near pointer); else they are read off the types."""
    if doc.get('node') != 'MethodDecl':
        raise Decline('document %s' % doc.get('node'))
    sixteen = 'segment' in (doc.get('origin') or {})
    locs = _locals(doc.get('body'), [])
    _now['moved'] = tuple(moved) if sixteen else tuple(p for p in moved if p in FLAT)
    _now['sixteen'] = sixteen
    # the far pointers: the parameters and locals of four bytes
    _now['far'] = (set(far) if far is not None else
                   {n for n, t in [(_param_name(p), p.get('type')) for p in doc.get('params') or []] +
                    [((v.get('sym') or {}).get('name'), v.get('type')) for v in locs] if is_far(t)}) \
        if 'far' in _now['moved'] else set()
    try:
        if 'far_from_stack' in _now['moved'] and _now['far']:
            _far_from_stack(doc.get('body'), locs)
        nodes, arrays = list(_walk(doc.get('body'))), _frame_arrays(locs)
        for name, rule in _PASSES:
            if name in _now['moved']:
                rule(nodes, arrays)
        return _function(doc, locs)
    finally:
        _now['moved'], _now['far'], _now['marks'], _now['whole'], _now['bare'] = (), set(), {}, {}, set()


_SEGMENT_REGISTER = re.compile(r"unaff_(SS|DS|CS)")


def _segment_register(name, t):
    """`unaff_SS` is the decompiler's "SS as the function was entered" (the segment of a local
    passed as a far pointer, CS of a string constant): declared with the register's value,
    `xunknown2 unaff_SS = codebreaker_ss();` (c_text._segment_registers). -> what is shown for the
    name, or None."""
    m = _SEGMENT_REGISTER.fullmatch(name)
    if 'segment_registers' not in _now['moved'] or not m or not re.fullmatch(r"\w+", type_text(t)):
        return None
    return "%s = codebreaker_%s()" % (name, m.group(1).lower())


def fragment(node):
    """A MachineFragment -- what stays assembly -- as the C of its module holds it. The 16-bit
    lane assembles such a function on its own, from its own file (build/module.py: wasm, linked at
    its place), and the C has its declaration in the header like any other's: in the function's
    place in the C, a note of where it is and why."""
    if node.get('node') != 'MachineFragment':
        raise Decline('document %s' % node.get('node'))
    o = node.get('origin') or {}
    at = ' %d:%04X' % (o['segment'], o['offset']) if 'segment' in o else ' %08X' % o['va'] if 'va' in o else ''
    held = ' (assembled with %s)' % node['within'] if node.get('within') else ''
    why = (node.get('reason') or '').replace('*/', '* /')
    return '/* %s%s: kept as its instructions%s -- %s */\n' % (node.get('name') or 'a fragment', at, held, why)


def module(doc, moved=MOVED):
    """A Module -- one L2 tree whatever each part came back as -- as (its C, {name: assembly}):
    each function written from its L2 document with the passes `moved`, each MachineFragment as
    `fragment` writes it and its text beside, for the assembler (docs/IR_UNIFICATION.md step 4).
    A function with no body in the tree (its C was the decompiler's, with no L2) is a note too."""
    if doc.get('node') != 'Module':
        raise Decline('document %s' % doc.get('node'))
    units = doc.get('units') or []
    parts, asm = [], {}
    for u in units if isinstance(units, list) else [units]:
        if u.get('node') == 'MachineFragment':
            parts.append(fragment(u))
            if u.get('asm'):
                asm[u.get('name') or len(asm)] = u['asm']
        elif u.get('node') == 'MethodDecl' and u.get('body') is not None:
            try:
                parts.append(function(u, moved))
            except Decline as e:                         # (a node this back end does not write)
                parts.append('/* %s: not written from L2 (%s) */\n' % (u.get('name') or '?', e))
        else:
            parts.append('/* %s: not in L2 (its C is the decompiler\'s printed C) */\n' % (u.get('name') or '?'))
    return '\n'.join(parts), asm


def _function(doc, locs):
    """PrintC::docFunction: the comments above it, the declaration, `{` a line below, the locals,
    the statements, `}`."""
    em = _Pretty()
    em.open_group()                                      # (the function)
    for text in doc.get('comments') or ():
        _comment(em, text, 0)
    if doc.get('comments'):
        em.tag_line()
    em.tag_line()
    _signature(em, doc)
    em.tag_line()
    em.tag_line()
    em.start_indent()
    em.print('{')
    for loc in locs:
        name = (loc.get('sym') or {}).get('name', '')
        em.tag_line()
        em.open_group()
        _put_type(em, loc.get('type'), name, name in _now['far'], _segment_register(name, loc.get('type')))
        em.close_group()
        em.print(';')
    if locs:
        em.tag_line()
    _block(em, doc.get('body'))
    em.stop_indent()
    em.tag_line()
    em.print('}')
    em.tag_line()
    em.close_group()
    return em.text()[1:]                                 # (the line it starts with: the caller's)
