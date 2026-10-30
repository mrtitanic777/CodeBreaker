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
         "null_is_zero", "global_pointer_steps", "one_name_for_globals", "far_result",
         "model_word", "joined_arguments", "far_arguments", "void_result", "byte_result", "export", "loadds",
         "selector_structs", "struct_gaps", "own_segment_calls", "open_arguments", "interrupts", "far_addresses",
         "far_casts", "entry_sp", "stack_arguments", "far_locals", "far_concat_casts", "buffer_fields", "far_reads",
         "near_to_far", "stack_segment_stores", "callee_pops", "caller_frame_slots", "selector_imports",
         "segment_values", "code_segment_reads", "names_used", "heading_facts", "int_result", "register_arguments",
         "code_pointers", "compare_as_numbers", "keep_frame", "globals_through", "global_widths", "same_cast",
         "fit_calls", "struct_fields", "wide_address", "homed_arguments", "uint8_result")
# ... those of them that need what the module's facts say of the function (its model in the
# description, what its instructions read and start with): done only when `function` is given
# them, as c_text gives them when it makes the module (c_text._write16); the C the decompiler is
# run for at first (decompiler.c_from_l2) is written without them
IN_MODULE = ("model_word", "joined_arguments", "far_arguments", "void_result", "byte_result", "export", "loadds",
             "selector_structs", "struct_gaps", "own_segment_calls", "open_arguments", "interrupts", "far_addresses",
             "far_casts", "entry_sp", "stack_arguments", "far_locals", "far_concat_casts", "buffer_fields",
             "far_reads", "near_to_far", "stack_segment_stores", "callee_pops", "caller_frame_slots",
             "selector_imports", "segment_values", "code_segment_reads", "names_used", "heading_facts",
             "int_result", "register_arguments", "code_pointers", "compare_as_numbers", "keep_frame",
             "globals_through", "global_widths", "same_cast", "fit_calls", "struct_fields", "wide_address",
             "homed_arguments", "uint8_result")
# ... those of them the 32-bit path has (compilable32)
FLAT = ("pieces", "null_is_zero", "struct_gaps", "int_result", "register_arguments", "code_pointers",
        "compare_as_numbers", "keep_frame", "globals_through", "global_widths", "same_cast", "fit_calls",
        "struct_fields")
# ... those the 16-bit path has not (the 32-bit path's own, and the PowerPC path's)
FLAT_ONLY = ("int_result", "register_arguments", "code_pointers", "compare_as_numbers", "keep_frame",
             "globals_through", "global_widths", "same_cast", "fit_calls", "struct_fields",
             "wide_address", "homed_arguments", "uint8_result")
# ... and those of the PowerPC path (c_text.compilable_powerpc; the facts' "lane": "powerpc")
POWERPC = ("wide_address",)
_now = {"moved": (), "far": set(), "marks": {}, "whole": {}, "bare": set(), "sixteen": False, "facts": {}, "doc": None,
        "locals": [], "drop": set(), "old_style": None, "room": set(), "gone": set(), "far_pointers": {},
        "gap_base": {}, "decls": {}, "seen": set(), "seeing": False, "raw": 0, "lines": set(),
        "at": {}, "top": None, "virtual": {}, "ren": {}, "locals_text": None,
        "owners": [], "owner_of": {}, "shown": {}, "ss_casts": set(), "far_read": set(),
        "before": {}, "after": {}, "stmt_at": {}, "pops": False, "declare_first": [], "seg_ptrs": [], "frame": set(), "alias": {}, "lane": None}
# (the function being written: its passes, its far names, and the nodes they rewrite --
#  {id(node): '_far' | '_near' | '_ss'} in a word of the text; `whole`: {id(node): shown(node)},
#  an expression written whole in another way; `bare`: those being printed as PrintC printed them;
#  `locals`: the function's Local statements; `drop`: the names of those a pass leaves undeclared)

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
    if _now['seeing'] and _now['owners']:
        _now['owner_of'][id(em)] = _now['owners'][-1]   # (written inside that rewrite: _prefix)
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

    def atom(self, text, shown=None, blank=False, node=None):
        if self.ops:
            self._emit(self.ops[-1])
        if node is not None and _now['seeing'] and not _now['raw'] and isinstance(self.em, _Flat):
            _now['at'][id(node)] = (self.em, len(self.em.out))       # (where it is written: _prefix)
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

    def whole(self, tok, raw, shown, paren=None, node=None):
        """An expression whose outermost operator is `tok` placed as one token: bracketed where
        that operator would be, given the room of its text as printed (`raw`), showing `shown`
        -- what a moved c_text pass writes in place of it, after the layout."""
        if tok is None:
            self.atom(raw, shown, node=node)
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
        self.atom(raw, shown, node=node)

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


def _push_type(st, t, name=None, far=False, word=None, shown_name=None, shown_base=None):
    """A type with its name (a declaration) or without (a cast, a return type), on the stack:
    PrintC::pushTypeStart, the name or a blank, pushTypeEnd. `far`: the outer pointer written
    __far (c_text._far: `int2 __far *p`); `word`: another word there (` __near`); `shown_name`:
    what is shown for the name (`unaff_SS = codebreaker_ss()`); `shown_base`: ... for the base type
    (a result of `void` written `xunknown2`)."""
    mods, base = _stack(t)
    st.op(_TYPE_NOSPACE if name is None and not mods else _TYPE_SPACE)
    word = ' __far' if far else word
    shown = (shown_base or base) + word if word and mods and all(m == 'ptr' for m in mods) else shown_base
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


def _put_type(em, t, name=None, far=False, shown_name=None, shown_base=None):
    st = _Stack(em)
    _push_type(st, t, name, far, shown_name=shown_name, shown_base=shown_base)


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
    _now['raw'] += 1
    try:
        em = _Flat()
        _put(em, e)
        return ''.join(em.raw)
    finally:
        _now['bare'].discard(id(e))
        _now['raw'] -= 1


def _rewrite(e, shown, room=False):
    """A moved pass writes `shown(e)` in place of expression `e` (the first pass to claim it).
    `room`: given no room of its own, where the text PrintC printed there was laid out over
    lines and what is written in its place is not (an interrupt helper's call: given the room of
    the call it stands for, or of its own text, its `;` was left on a line of its own)."""
    if id(e) not in _now['whole']:
        _now['whole'][id(e)] = shown
        if room:
            _now['room'].add(id(e))


def _push(st, e, paren=None):
    """One expression on the stack, as PrintC pushes it: the operator, then its operands in the
    order they are printed."""
    n = e['node']
    if _now['seeing'] and not _now['raw']:
        _now['seen'].add(id(e))                          # (shown in the text: _seen)
    rewrite = _now['whole'].get(id(e))
    if rewrite is not None and id(e) not in _now['bare']:
        # (a moved pass's text in its place: given the room of the text PrintC printed, as
        # c_text rewrites the printed C after its layout)
        seeing = _now['seeing'] and not _now['raw']
        if seeing:
            _now['owners'].append(id(e))
        try:
            shown = rewrite(e)
        finally:
            if seeing:
                _now['owners'].pop()
        if seeing:
            _now['shown'][id(e)] = shown
        st.whole(_op_of(e), '' if id(e) in _now['room'] else _raw(e), shown, paren, e)
        return
    if n == 'Lit':
        st.atom(_lit(e))
        return
    if n == 'Name':
        name = _name(e)
        if name in _now['frame'] and (e.get('sym') or {}).get('kind') != 'field':
            # (a local of the frame structure, written after the lines are laid out: _keep_frame)
            st.atom(name, '((int4)&_f + (int4)sizeof(_f))' if name == _ENTRY_SP else '_f.' + name, node=e)
            return
        if name in _now['alias']:                        # (a global at this function's own width: _global_now)
            st.atom(name, _now['alias'][name], node=e)
            return
        st.atom(name, _mk_fp_text('codebreaker_ss()', e, shown=name) if _now['marks'].get(id(e)) == '_ss' else None,
                node=e)
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
        pops = 'code_callee_pops' if _pops(_cast_type(e)) != _cast_type(e) else None   # (_callee_pops)
        if mark == '_near':                              # (`code __near *`: c_text._near_calls)
            _push_type(st, e['type'], word=' __near', shown_base=pops)
        else:
            _push_type(st, e['type'], far=mark in ('_far', '_ds'), shown_base=pops)
        if mark == '_ds':                                # (a data address: DS and it -- _data_address;
            n = _lit(e['x'])                             #  added after the lines are laid out, as c_text does)
            st.atom(n, _mk_fp_text('codebreaker_ds()', e['x'], shown=n))
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
    if id(s) in _now['gone']:
        return                                   # (a pass took the statement away)
    _inserted(em, s, 'before')
    if n == 'ExprStmt':
        if _now['seeing']:
            _now['seen'].add(id(s))
            _now['lines'].add(id(s))                     # (a statement on a line of its own)
        _statement(em, s['expr'])
        _inserted(em, s, 'after')
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
                _inserted(em, c, 'before')
                em.tag_line()
                em.print('default')
                em.print(':')
            else:
                for k, label in enumerate(_stmts(c.get('labels'))):
                    if k == len(_stmts(c.get('labels'))) - 1:
                        _inserted(em, c, 'before')       # (the case its statements are of, to C's grammar)
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
    elif n == 'MachineFragment' and s.get('reason') == 'inline':
        # an _asm block of Microsoft C (decompile/asm_blocks.py): its instructions as the inline
        # assembler takes them, C's names in its operands, labels at the left
        out.append(pad + '_asm {')
        for ln in _stmts(s.get('asm')):
            out.append(ln if ln.endswith(':') else pad + '  ' + ln)
        out.append(pad + '}')
    else:
        raise Decline('statement %s' % n)


def _inserted(em, s, where):
    """The statements a pass puts before or after statement `s` (or before a case's last label),
    each on a line of its own -- and, laying the text out for _seen, where `s` is written."""
    if where == 'before' and _now['seeing'] and isinstance(em, _Flat):
        _now['stmt_at'][id(s)] = len(em.out)
    for text in _now[where].get(id(s), ()):
        em.tag_line()
        em.print('', text)


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
    for e, _parent in nodes:
        # (also behind a cast, `(int4)*(int2 *)(...)`: GDI's sub_35_0669, sub_35_0722 and
        # sub_36_0000, which read their frame through DS while c_text took it for a product)
        if e['node'] != 'Unary' or e.get('op') != '*' or e.get('postfix') or e['x'].get('node') != 'Cast':
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
        _now['ss_casts'].add(id(cast))
        # (` __far` where _far_reads found the place it reads far: the stack's)
        _rewrite(cast, lambda c, t=ty.group(1), s=sum_: "(%s)%s" % (
            _pops('%s%s *' % (t, ' __far' if id(c) in _now['far_read'] else '')), _mk_fp_text('codebreaker_ss()', s, ',')))


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
        if _now['lane'] == 'powerpc':                   # (big-endian: from the end of its storage)
            _rewrite(e, lambda s, off=m.group(1), ty=table.get(int(m.group(2)), other):
                     "PIECE(%s, %s, %s)" % (_sub(_token(s), s['obj'], 0), off, ty))
            continue
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


# ---------------------------------------------------------------------------------------------- what the module says
# The passes after the pointer ones read what the module's facts say of the function: its model in
# the description, how far its instructions read through an argument, which arguments they load as
# far pointers, how they start (c_text._write16 works these out from the original's code and hands
# them over: `function(..., facts=)`).
_JOINED = re.compile(r"_(a\d+|param_\d+)")
_WIDE = ('xunknown4', 'int4', 'uint4', 'undefined4', 'float4')
_BYTES = ('bool', 'char', 'int1', 'uint1', 'xunknown1', 'undefined1', 'undefined')


def _heading(doc):
    """What c_text's passes after the pointer ones saw of the function's heading, as words: (the
    result's type, the convention's words, the words put before the name, the name) -- the model
    word where the decompiler left the convention out (`__stdcall16near`, or `__far`/`__near`:
    a head with no model compiles with OpenWatcom's own, its arguments in registers), and
    `__export` / `__loadds` where its instructions start so."""
    ctx = _now['facts']
    ret = _ret(doc)
    result = type_text(ret)
    mods = list(doc.get('mods') or ())
    words = []
    head0 = ' '.join([result] + mods)
    if 'model_word' in _now['moved'] and ctx.get('distance') and '16near' not in head0 and '16far' not in head0:
        said = set(head0.replace('(', ' ').replace(')', ' ').replace('*', ' ').split())
        words.append(ctx.get('model') if ctx.get('model') and not said & {'__stdcall', '__cdecl', '__pascal'}
                     else '__' + ctx['distance'])
    far_model = any('__stdcall16far' in w or '__cdecl16far' in w for w in mods + words) or (mods + words)[-1:] == ['__far']
    if far_model and 'export' in _now['moved'] and ctx.get('export') and not _lost(doc):
        words.append('__export')
    if 'loadds' in _now['moved'] and ctx.get('loadds') and not _lost(doc):
        words.append('__loadds')
    return result, mods, words, far_model


_FASTCALL = re.compile(r"__fastcall\w+")


def _lost(doc):
    """A heading c_text's parser lost the name of: `int2 __fastcall16far f(void)` (it reads
    `__fastcall` as Microsoft's word and the rest as another name). There c_text never wrote
    __export, __loadds or a byte result made a word -- KRNL386's sub_2_7805, sub_2_79DB and
    sub_2_7BA8 load DGROUP and are not __loadds -- and neither does this, until that change is
    tried on its own (with the fastcall pragma, untried)."""
    return not doc.get('params') and any(_FASTCALL.fullmatch(m) for m in doc.get('mods') or ())


def _ret(doc):
    ft = (doc.get('sym') or {}).get('type') or {}
    return doc.get('ret') or (ft.get('result') or {}).get('type') or {'kind': 'void'}


def _result_shown(ret):
    """What is shown for the result's base type: `xunknown2` for `void` (it sets no result, but
    its callers may read AX: c_text's void -> xunknown2), `int2`/`uint2` for a byte (returned in
    AX all the same: Windows' BOOL is an int, and callers test the whole of AX)."""
    mods, base = _stack(ret)
    if base == 'void' and 'int_result' in _now['moved']:
        return 'int'                                     # (32-bit: in EAX -- c_text's void -> int)
    if base == 'void' and 'uint8_result' in _now['moved']:
        # (PowerPC: every function leaves something in r3 and callers are free to use it -- the
        # module notes it returns nothing, the facts' "novalue")
        _now['facts']['novalue'] = True
        return 'uint8'

    if base == 'void' and 'void_result' in _now['moved']:
        return 'xunknown2'
    if not mods and base in _BYTES and 'byte_result' in _now['moved'] and not _lost(_now['doc'] or {}):
        return 'int2' if base in ('char', 'int1') else 'uint2'
    return None


def flags(doc, facts):
    """The function's flags in the module (the module's sets, c_text._compilable16): near (near is the default and
    is not printed), wide (a result of four bytes, or a far pointer), novalue (it sets no result),
    exported_prologue -- as its heading is written (`facts` as `function` is given them)."""
    was = _now['moved'], _now['facts'], _now['far'], _now['doc']
    _now['moved'], _now['facts'], _now['far'], _now['doc'] = \
        tuple(p for p in MOVED if p not in FLAT_ONLY), facts or {}, set(), doc      # (16-bit)
    try:
        ret = _ret(doc)
        result, mods, words, far_model = _heading(doc)
        far_result = is_far(ret)
        shown = _result_shown(ret)
        out = set()
        first = (shown or result).split()[0] if (shown or result) else ''
        if _stack(ret)[1] == 'void' and first == 'xunknown2':
            out.add('novalue')
        if far_result or (facts or {}).get('far_return'):
            out.add('wide')
        if not far_model:
            out.add('near')
        elif (facts or {}).get('export'):
            out.add('exported_prologue')
        written = type_text(ret, far=far_result)
        if shown:
            written = shown + written[len(_stack(ret)[1]):]
        before = ' '.join([written] + mods + words)
        if first in _WIDE or '*' in before and '__far' in before:
            out.add('wide')
        return out
    finally:
        _now['moved'], _now['facts'], _now['far'], _now['doc'] = was


def _joined_arguments(nodes, _arrays):
    """`_a1` is the decompiler's "a1 and the argument after it, as one four-byte value" (a far
    pointer or a long passed as two words): the four bytes from a1, `(*(uint4 *)&a1)` -- read
    through, a far pointer, `*(*(int2 __far * *)&a1)`, a byte where the original reads one through
    it (USER's sub_42_038E; c_text._joined_arguments)."""
    ctx = _now['facts']
    result, mods, words, _far_model = _heading(_now['doc'])
    far_fn = 'far' in ' '.join([result] + mods + words + [_now['doc']['name']])
    widths = (ctx.get('widths') or {}).get(far_fn) or {}
    for e, p in nodes:
        m = _JOINED.fullmatch(_name(e)) if e['node'] == 'Name' else None
        if not m:
            continue
        if p is not None and p['node'] == 'Unary' and p.get('op') == '*' and not p.get('postfix') and p['x'] is e:
            _rewrite(p, lambda _x, a=m.group(1): "*(*(%s __far * *)&%s)" % ("char" if widths.get(a) == 1 else "int2", a))
        else:
            _rewrite(e, lambda _x, a=m.group(1): "(*(uint4 *)&%s)" % a)


def _far_arguments(nodes, _arrays):
    """An argument the original loads as a far pointer (`les bx, [bp+6]`), the word after it its
    segment, is one where it is read or written through: `*(T *)a1` is `*(T __far
    *)CODEBREAKER_FAR(a2, a1)` (c_text._far_arguments). Arguments lie on the stack lowest first,
    the first 6 bytes above BP in a far function and 4 in a near one -- as c_text told them, by
    `far` anywhere in the heading as written then."""
    doc, loads = _now['doc'], _now['facts'].get('loads')
    params = doc.get('params') or []
    if not loads or not params:
        return
    result, mods, words, _far_model = _heading(doc)
    texts = [type_text(p.get('type'), _param_name(p), _param_name(p) in _now['far']) for p in params]
    near = 'far' not in ' '.join([result] + mods + words + [doc['name']] + texts)
    at, where = (4 if near else 6), {}
    for prm in texts:
        ws = prm.split()
        name = ws[-1].lstrip('*')
        wide = '*' in prm and '__far' in prm or ws[0] in _WIDE
        where[at] = (name, ws[0], wide)
        at += 4 if wide else 2
    segment_of = {}
    for off in sorted(loads):
        if off not in where or off + 2 not in where:
            continue
        (name, kind, wide), (segment, kind2, wide2) = where[off], where[off + 2]
        if wide or wide2 or kind not in _WORDS[:3] or kind2 not in _WORDS[:3]:
            continue
        segment_of.setdefault(name, segment)
    if not segment_of:
        return
    for e, p in nodes:
        if e['node'] != 'Unary' or e.get('op') != '*' or e.get('postfix') or e['x'].get('node') != 'Cast':
            continue
        cast = e['x']
        ty = re.fullmatch(r"([A-Za-z_]\w*) \*", type_text(cast.get('type')))
        if not ty or _now['marks'].get(id(cast)) or id(cast) in _now['whole']:
            continue
        v = cast['x']['x'] if cast['x'].get('node') == 'Paren' else cast['x']
        if v.get('node') == 'Name' and _name(v) in segment_of:
            name, rest = _name(v), ''
        elif v.get('node') == 'Binary' and v.get('op') == '+' and v['left'].get('node') == 'Name' and \
                _name(v['left']) in segment_of and not _bracket(_token(v), v['left'], 0):
            name, inner = _name(v['left']), expr(v)
            if '(' in inner or ')' in inner or not inner.startswith(name + ' + '):
                continue
            rest = ' + ' + inner.split('+', 1)[1].strip()
        else:
            continue
        _rewrite(e, lambda _x, t=ty.group(1), s=segment_of[name], n=name, r=rest:
                 "*(%s __far *)CODEBREAKER_FAR(%s, %s%s)" % (t, s, n, r))


# (bytes of the decompiler's scalar type names)
SIZEOF = {"int1": 1, "uint1": 1, "xunknown1": 1, "char": 1, "bool": 1, "undefined": 1, "undefined1": 1,
          "int2": 2, "uint2": 2, "xunknown2": 2, "undefined2": 2,
          "int4": 4, "uint4": 4, "xunknown4": 4, "undefined4": 4, "float4": 4,
          "int8": 8, "uint8": 8, "xunknown8": 8, "undefined8": 8, "float8": 8, "float10": 10}
_RAM = re.compile(r"([a-z]+)Ram([0-9a-f]{8})")


def seg_layout(fields):
    """the fields of a selector's structure, {offset: type}, as a declaration's body; None when
    they overlap"""
    body, at = [], 0
    for off in sorted(fields):
        if off < at:
            return None
        if off > at:
            body.append("uint1 _pad_%x[%d];" % (at, off - at))
        body.append("%s f_%x;" % (fields[off], off))
        at = off + SIZEOF[fields[off]]
    return body


def _shape(e):
    """What C's grammar makes of an expression as it is written now (the moved passes' rewrites
    so far in it): 'identifier', 'pointer' (`*x`, `&x`), 'field' (`x.f`, `x->f`), 'number', or
    'other' -- as c_text's passes asked tree-sitter of the text."""
    shown = _now['whole'].get(id(e))
    if shown is not None:
        t = shown(e).strip()
        if re.fullmatch(r"[A-Za-z_]\w*", t):
            return 'identifier'
        return 'pointer' if t[:1] in '*&' else 'other'
    n = e.get('node')
    if n == 'Name':
        return 'identifier' if _now['marks'].get(id(e)) != '_ss' else 'other'
    if n == 'Unary' and not e.get('postfix') and e.get('op') in ('*', '&'):
        return 'pointer'
    if n == 'Select':
        return 'field'
    if n == 'Lit':
        return 'number' if re.fullmatch(r"0x[0-9a-fA-F]+|\d+|0[0-7]*", _lit(e)) else 'other'
    return 'other'


def _unary_as_read(u, op, parent):
    """Is `u` the prefix operator `op` as c_text's parser read the text? Not right behind a cast to
    a type with no `*`: the grammar does not know the decompiler's type names, and reads
    `(int2)&x` as a bitwise and, `(int2)*p` as a product (ctree.misread)."""
    if u is None or u.get('node') != 'Unary' or u.get('op') != op or u.get('postfix') or id(u) in _now['whole']:
        return False
    up = parent.get(id(u))
    if not (up is not None and up.get('node') == 'Cast' and up.get('x') is u and
            '*' not in type_text(up.get('type')) and id(up) not in _now['whole']):
        return True
    # (`(T)&x` is read as a cast where what follows it binds tighter than a bitwise and -- `(int2)&p->f
    # + 2` -- and as `(T) & x` where it does not: `f((int2)&p->f)`; `(T)*p` is a product wherever)
    return op == '&' and _BINARY.get(_token_after(up, parent), 0) > _BINARY['&']


def _token_after(e, parent):
    """The operator printed right after expression `e` ('' for one that ends its statement, `)` for
    a bracket closing): what the grammar sees next."""
    while True:
        p = parent.get(id(e))
        if p is None:
            return ''
        n = p['node']
        if n == 'Paren':
            return ')'
        tok = _token(p)
        if n == 'Binary':
            if p['left'] is e:
                return ')' if _bracket(tok, e, 0) else p['op']
            if _bracket(tok, e, 1):
                return ')'
        elif n == 'Assign':
            if p['target'] is e:
                return '='
            if _bracket(tok, e, 1):
                return ')'
        elif n in ('Cast', 'Unary'):
            if _bracket(tok, e, 1 if n == 'Cast' else 0):
                return ')'
        elif n == 'Call':
            if p.get('target') is e:
                return '('
            args = p.get('args') or []
            return ')' if args and args[-1] is e else ','
        elif n == 'Index':
            return '[' if p['arr'] is e else ']'
        elif n == 'Select':
            return '.'
        elif n == 'Cond':
            return '?' if p.get('cond') is e else ':' if p.get('then') is e else ''
        else:
            return ''
        e = p


def _calls_in(e):
    """Is a call written in `e` as it is now (a moved pass's rewrite counts by its text)?"""
    for x, _p in _walk(e):
        shown = _now['whole'].get(id(x))
        if shown is not None:
            if re.search(r"\w\s*\(", shown(x)):
                return True
            continue
        if x.get('node') == 'Call':
            return True
    return False


def _selector_structs(nodes, _arrays):
    """A segment reached through a selector at constant offsets -- `*MK_FP(xVar2,(int2 *)0x3e)`,
    `*MK_FP(*(xunknown2 *)(a1 + 0x18),(int2 *)0x40)`: a structure that segment holds (a task's
    or a module's database) -- read as one: `SEG_PTR(seg_f_xVar2, xVar2)->f_3e` (SEG_PTR: the
    segment's start as a far pointer), its fields at exactly those offsets with exactly those
    types. Per function, for a selector that is a variable or a plain read (no call in it), used
    at two offsets or more, each always with one type and none overlapping (c_text's
    _selector_structs, which worked on the text). The structures go to the module's facts
    ("selector_decls"); a global selector's (`xRam10000002`, the same segment in every function)
    is the module's, `seg_xRam10000002`: its fields go to "selector_shared" {name: {function:
    fields}}, declared once (c_text._shared_selector_structs)."""
    facts = _now['facts']
    if not facts.get('selector_structs'):
        return
    decls = facts.setdefault('selector_decls', [])
    shared = facts.setdefault('selector_shared', {})
    fname = _now['doc'].get('name') or ''
    parent = {id(e): p for e, p in nodes}
    groups = {}                                          # selector text -> [(node, offset, type)]
    for e, p in nodes:
        parts = _mk_fp(e)
        if not parts or id(e) in _now['whole']:
            continue
        sel_e, off_e = parts
        if off_e.get('node') != 'Cast' or id(off_e) in _now['whole'] or _now['marks'].get(id(off_e)):
            continue
        ty = type_text(off_e.get('type')).replace(' ', '')
        val = off_e['x']
        if not ty.endswith('*') or ty.count('*') != 1 or _shape(val) != 'number' or id(val) in _now['whole']:
            continue
        try:
            off = int(_lit(val), 0)
        except ValueError:
            continue
        if _shape(sel_e) not in ('identifier', 'pointer', 'field') or _calls_in(sel_e):
            continue
        up, inside = p, False
        while up is not None:
            if _mk_fp(up):
                inside = True                            # (one inside another's arguments: a far call
                break                                    # through a far pointer -- left as it is)
            up = parent.get(id(up))
        if inside:
            continue
        sel = ' '.join(expr(sel_e).split())
        groups.setdefault(sel, []).append((e, p, off, ty[:-1]))
    made = 0
    for sel, uses in groups.items():
        fields = {}
        for _e, _p, off, ty in uses:
            if fields.get(off, ty) != ty:
                fields = None
                break
            fields[off] = ty
        if not fields or len(fields) < 2 or any(SIZEOF.get(t) is None for t in fields.values()):
            continue
        body = seg_layout(fields)
        if body is None:
            continue                                     # (overlapping: not one layout)
        made += 1
        if _RAM.fullmatch(sel):
            name = 'seg_' + sel                          # (the module's)
            shared.setdefault(name, {})[fname] = fields
        else:
            tag = sel if re.fullmatch(r"[A-Za-z_]\w*", sel) else 'sel%d' % made
            name = 'seg_%s_%s' % (re.sub(r"\W", "_", fname), tag)   # (not s_..: the core's own structures)
            decls.append('typedef _Packed struct %s { %s } %s;' % (name, ' '.join(body), name))
        # (a global selector's structure where the module has the function's own: its name)
        name = (facts.get('selector_renames') or {}).get(name, name)
        for e, p, off, _ty in uses:
            _now['seg_ptrs'].append((id(p) if _unary_as_read(p, '*', parent) else id(e), name, e['args'][0]))
            if _unary_as_read(p, '*', parent):
                _rewrite(p, lambda _x, n=name, s=e['args'][0], o=off: 'SEG_PTR(%s, %s)->f_%x' % (n, expr(s), o))
            else:
                _rewrite(e, lambda _x, n=name, s=e['args'][0], o=off: '(&SEG_PTR(%s, %s)->f_%x)' % (n, expr(s), o))


_GAP_FIELD = re.compile(r"field_0x([0-9a-f]+)")


def _pointer_distances(doc):
    """{name: "far" | "near"} of the function's pointer parameters and locals, by how each is
    declared (`T __far *p` far, `T *p` near; c_text._pointer_distances)."""
    out = {}
    decls = [(_param_name(p), p.get('type')) for p in doc.get('params') or []] + \
        [((v.get('sym') or {}).get('name'), v.get('type')) for v in _now['locals']]
    for n, t in decls:
        m = re.fullmatch(r"(?<![\w*])[A-Za-z_]\w*\s+(__far\s*)?\*+\s*(\w+)(?: \[\d*\])?", type_text(t, n, n in _now['far']))
        if m:
            out.setdefault(m.group(2), 'far' if m.group(1) else 'near')
    return out


def _struct_gaps(nodes, _arrays):
    """`&p->field_0x1` -- the core's name for an offset of a recovered structure that holds no
    field of it: the bytes at that offset, `((uint1 *)(p) + 0x1)`, and `p->field_0x1` the byte
    there; with `.` the same from the structure's own address. The byte pointer keeps the base
    variable's distance (`uint1 __far *` for a far one); where the base is no variable of known
    distance the offset is left as it is (c_text._struct_gaps). 32-bit: there is one distance, and
    every such offset is so (compilable32's _struct_gaps)."""
    flat = not _now['sixteen']
    if not flat and not _now['facts'].get('selector_structs'):
        return
    distance = None
    parent = {id(e): p for e, p in nodes}
    for e, p in nodes:
        mm = _GAP_FIELD.fullmatch(_name(e)) if e['node'] == 'Select' else None
        if not mm or id(e) in _now['whole']:
            continue
        if distance is None:
            distance = _pointer_distances(_now['doc']) if not flat else {}
        arrow = _arrow(e)
        base = e['obj']['x'] if arrow else e['obj']
        arg = _gap_base(e, base, False)
        root = re.match(r"\(*(\w+)", arg)
        d = distance.get(root.group(1)) if root else None
        cast = re.match(r"\(+\s*\w+\s*(__far\s*)?\*\s*\)", arg)    # (a cast says its own distance)
        if cast:
            d = 'far' if cast.group(1) else 'near'
        if flat:
            d = 'near'
        if d is None:
            continue
        ptr = 'uint1 __far *' if d == 'far' else 'uint1 *'
        dot = not arrow
        if _unary_as_read(p, '&', parent):
            _rewrite(p, lambda _x, b=base, pt=ptr, k=mm.group(1), dot=dot, e=e:
                     '(%s + 0x%s)' % (_gap_cast(pt, _gap_base(e, b, dot)), k))
            if not dot and id(p) in _now['whole']:
                _now['gap_base'][id(p)] = base           # (a sum from the base, as the text reads)
        else:
            _rewrite(e, lambda _x, b=base, pt=ptr, k=mm.group(1), dot=dot, e=e:
                     '(*(%s + 0x%s))' % (_gap_cast(pt, _gap_base(e, b, dot)), k))


def _gap_cast(pt, base):
    """`(pt)(base)`, the byte pointer a gap's offset is counted from -- `(uint1 __far *)(uint1 *)(base)`
    where _near_to_far finds a number in the brackets, as c_text's did in the text this wrote."""
    inner = '(%s)' % base
    if pt.endswith('__far *') and 'near_to_far' in _now['moved'] and _near_number(inner):
        return '(%s)(%s *)%s' % (pt, pt[:-len(' __far *')], inner)
    return '(%s)%s' % (pt, inner)


def _gap_base(e, base, dot):
    """The base of a gap's field as the text had it (in brackets where it was printed so), and
    `&(...)` of it for `.`."""
    arg = expr(base)
    if _bracket(_token(e), base, 0):
        arg = '(%s)' % arg
    return '&(%s)' % arg if dot else arg


def _mk_fp(e):
    """(selector, offset) of `MK_FP(s,o)` -- the core's segmented address printed whole -- else None."""
    if e.get('node') != 'Call' or len(e.get('args') or ()) != 2:
        return None
    t = e.get('target') or {}
    return tuple(e['args']) if t.get('node') == 'Name' and _name(t) == 'MK_FP' else None


def _own_segment_calls(nodes, _arrays):
    """`(*MK_FP(0x1000,*(xunknown2 *)0x18))()` -- a near call through a word, `call word ptr
    [18h]`, as the decompiler prints it: the segment is the function's own code segment, which it
    takes as the made-up selector the description gave that segment (describe.selector: 1000h,
    1008h, ...). Made a far call to that constant, it faulted (MSMOUS31's Disable, at Setup's
    restart): a near call, `((*(code __near *)*(xunknown2 *)0x18))()`, as the original makes. And
    any other MK_FP of that selector -- a read of the code segment's own bytes, `mov ax, cs:[si]`
    (a table beside the code: KEYBOARD's GetKeyNameText) -- is through CS as it runs,
    `MK_FP(codebreaker_cs(),...)`: no selector of that number exists (c_text._own_segment_calls)."""
    own = _now['facts'].get('selector')
    if own is None:
        return
    spelled = '0x%x' % own

    def ours(lit_):
        return lit_.get('node') == 'Lit' and re.fullmatch(r"0x[0-9a-fA-F]+|\d+", _lit(lit_)) is not None and \
            int(_lit(lit_), 0) == own
    for e, _p in nodes:
        if e['node'] != 'Call':
            continue
        fn = e.get('target') or {}
        while fn.get('node') == 'Paren':
            fn = fn['x']
        if fn.get('node') != 'Unary' or fn.get('op') not in ('*', '&') or fn.get('postfix'):
            continue
        target = fn['x']
        while target.get('node') == 'Paren':
            target = target['x']
        parts = _mk_fp(target)
        if fn['op'] == '*' and parts and ours(parts[0]):
            _rewrite(fn, lambda _f, off=parts[1]: "(*(%s)%s)" % (_pops('code __near *'), expr(off)))
    for e, _p in nodes:
        parts = _mk_fp(e)
        if parts and parts[0].get('node') == 'Lit' and _lit(parts[0]) == spelled:
            _rewrite(parts[0], lambda _x: 'codebreaker_cs()')


# Software interrupts. The decompiler gives `INT n` as a call to int<n> (languages/x86-16.cspec):
# its arguments are the registers set before it, in the order AX BX CX DX SI DI DS ES, and what it
# hands back besides AX is read as int_<register> (int_CF, int_BX, ...). In the C, a call of int21
# setting AX, CX and DX is int21_ax_cx_dx(ax, cx, dx), a helper that puts them in those registers,
# takes the interrupt (an assembly routine, codebreaker_int21, the build writes) and returns DX:AX;
# int_<register> is the register as the interrupt left it (c_text.interrupt_helpers declares
# them). A register the code did not set before the interrupt (the decompiler passes it on as
# in_BX, unaff_DS ...) is not an argument: it keeps the value it has -- DS the caller's data segment.
INTERRUPT_REGISTERS = ('ax', 'bx', 'cx', 'dx', 'si', 'di', 'ds', 'es')
_INT_CALL = re.compile(r"int([0-9a-f]{2})")
_INT_RESULT = re.compile(r"int_E?([A-Z][A-Z])(?:_[0-9a-f]+)?")


def _int_call(e):
    """The vector of a call of int<n>, else None."""
    t = e.get('target') if e.get('node') == 'Call' else None
    m = _INT_CALL.fullmatch(_name(t)) if t and t.get('node') == 'Name' else None
    return int(m.group(1), 16) if m else None


def _interrupts(nodes, _arrays):
    """Each call of int<n> -- not one inside another's arguments -- as its helper with the
    registers set for it (`int21_ax_cx_dx(0x3d00,a,b)`), and each interrupt result as the register
    the helper leaves it in (`int_EBX_12` is int_BX; its local goes: c_text._interrupts). The
    interrupts taken, (vector, registers), go to the module's facts ("interrupts"), for the helpers
    the header declares."""
    used = _now['facts'].setdefault('interrupts', set())
    parent = {id(e): p for e, p in nodes}
    for e, p in nodes:
        vector = _int_call(e)
        if vector is None:
            continue
        up, inside = p, False
        while up is not None:
            if _int_call(up) is not None:
                inside = True                            # (one inside another's arguments: as it was)
                break
            up = parent.get(id(up))
        if inside:
            continue
        args = (e.get('args') or [])[:len(INTERRUPT_REGISTERS)]
        regs = tuple(r for r, a in zip(INTERRUPT_REGISTERS, args) if not _passed_on(expr(a), r))
        used.add((vector, regs))
        _rewrite(e, lambda c, v=vector, regs=regs, args=args:
                 "int%02x_%s(%s)" % (v, '_'.join(regs) if regs else 'none',
                                     ','.join(expr(a) for r, a in zip(INTERRUPT_REGISTERS, args) if r in regs)),
                 room=True)
    for e, _p in nodes:
        if e['node'] == 'Name' and (e.get('sym') or {}).get('kind') != 'field':
            m = _INT_RESULT.fullmatch(_name(e))
            if m:
                _rewrite(e, lambda _x, r=m.group(1): 'int_' + r)
    for v in _now['locals']:
        n = (v.get('sym') or {}).get('name') or ''
        if _INT_RESULT.fullmatch(n):
            _now['drop'].add(n)                          # (no variable of its own: the register)


_ENTRY_SP = 'register0x00000010'                       # (the decompiler's SP as the function was entered)
_STACK_PLACE = re.compile(r"stack0x([0-9a-f]{4})")
_OWN_FRAME = re.compile(r"\w*Stack_([0-9a-f]+)")


def _entry_sp(nodes, _arrays):
    """The decompiler's name for SP as the function was entered, used as a value (code that keeps
    the stack pointer, or makes an address on the stack from it): codebreaker_entry_sp(). A place
    on the stack it names by that (`&stack0xffec`: entry SP - 0x14, above the function's own
    frame, the caller's) is that address -- unless it is one of the function's own locals
    (xStack_4: entry SP - 4) read through its address, `*(T *)(&stack0xfffa + 2)`: then it is
    that local, wherever the compiler keeps it (SUWIN's 2:819E called through the segment it kept
    in [bp-4], read as a place nothing had set: c_text._entry_sp)."""
    own = {}                                             # entry SP less -> (type, name)
    for v in _now['locals']:
        n = (v.get('sym') or {}).get('name') or ''
        m = _OWN_FRAME.fullmatch(n)
        t = type_text(v.get('type'))
        if m and n not in _now['drop'] and re.fullmatch(r"\w+", t):
            own[int(m.group(1), 16)] = (t, n)

    def place(u):
        """(name, plus) of `&stack0xNNNN`, else None"""
        if u.get('node') == 'Unary' and u.get('op') == '&' and not u.get('postfix') and u['x'].get('node') == 'Name':
            m = _STACK_PLACE.fullmatch(_name(u['x']))
            return int(m.group(1), 16) if m else None
        return None
    taken = set()
    if own:
        for e, _p in nodes:
            if e['node'] != 'Unary' or e.get('op') != '*' or e.get('postfix') or id(e) in _now['whole']:
                continue
            cast = e['x']
            if cast.get('node') != 'Cast' or id(cast) in _now['whole']:
                continue
            ty = re.fullmatch(r"(\w+) \*", type_text(cast.get('type')))
            v = cast['x']
            base, plus = place(v), 0
            if base is None and v.get('node') == 'Binary' and v.get('op') == '+' and \
                    v['right'].get('node') == 'Lit' and re.fullmatch(r"\d+|0x[0-9a-f]+", _lit(v['right'])):
                base, plus = place(v['left']), int(_lit(v['right']), 0)
                v = v['left']
            if not ty or base is None or id(v) in _now['whole']:
                continue
            hit = own.get(0x10000 - base - plus)
            if hit:
                taken.add(id(v))
                _now['virtual'][id(e)] = (hit[1], 'plain' if hit[0] == ty.group(1) else 'address')
                _rewrite(e, lambda _x, h=hit, t=ty.group(1):
                         _local_text(h[1]) if h[0] == t else '*(%s)&%s' % (_pops(t + ' *'), _local_text(h[1])))
    for e, _p in nodes:
        if e['node'] == 'Name' and _name(e) == _ENTRY_SP:
            _rewrite(e, lambda _x: 'codebreaker_entry_sp()')
        base = place(e)
        if base is not None and id(e) not in taken:
            _rewrite(e, lambda _x, b=base: '((uint1 *)codebreaker_entry_sp() + (int2)0x%04x)' % b)


# (a far pointer's high word as c_text's _far_addresses read it in the text: `(T)((uint4)p >> 0x10)`,
# the piece p._2_2_, or that piece as the word at &p + 2)
_HIGH = (r"(?:\(\s*\w+\s*\)\s*\(\s*\(\s*uint4\s*\)\s*(\w+)\s*>>\s*0x10\s*\)|(\w+)\._2_2_|"
         r"\(?\s*\*\s*\(\s*u?int2\s*\*\s*\)\s*\(\s*\(\s*char\s*\*\s*\)\s*&\s*(\w+)\s*\+\s*2\s*\)\s*\)?)")
_LOW = re.compile(r"\s*\(\s*u?int2\s*\)\s*(?:\(\s*void\s*\*\s*\)\s*)?(\w+)\s*")
_FAR_DECL = re.compile(r"__far\s*\*\s*(\w+)\s*[;,)]")


def _high_of(text):
    m = re.fullmatch(_HIGH, text)
    return (m.group(1) or m.group(2) or m.group(3)) if m else None


def _strip(n):
    while n.get('node') == 'Paren':
        n = n['x']
    return n


def _leftmost(n):
    """what an offset counts from: itself, or the first term of a sum or difference"""
    while True:
        n = _strip(n)
        if n.get('node') == 'Binary' and n.get('op') in ('+', '-'):
            n = n['left']
        else:
            return n


def _pointer_cast(n):
    """T of a cast to `T *`, else None"""
    if n.get('node') != 'Cast' or id(n) in _now['whole']:
        return None
    ty = type_text(n.get('type')).strip()
    return ty[:-1].strip() if ty.endswith('*') and ty.count('*') == 1 else None


def _far_target(off):
    """(T, the offset written) of a segmented address to `off` as a far pointer (_far_addresses):
    `(T *)x` a T at x; `(T *)x + n` a T at the sum; `p + n`, p a T *, a T as the pointer is;
    any other a byte."""
    first = _leftmost(off)
    if _pointer_cast(off):
        target, offset = _pointer_cast(off), off['x']
    elif _pointer_cast(first):
        target, offset = _pointer_cast(first), off
    else:
        # (the pointer by its name as written then: a far pointer from the frame -- `_ss`, written
        # MK_FP(codebreaker_ss(), p) -- by its own; an interrupt's result, whose local is gone, is none)
        n = _name(first) if first.get('node') == 'Name' and _now['marks'].get(id(first)) == '_ss' else \
            expr(first) if _shape(first) == 'identifier' else None
        target, offset = (_now['far_pointers'].get(n, 'uint1') if n not in _now['drop'] else 'uint1'), off
    return ('uint1' if target in ('void', 'code') else target), offset


def _written(e, under):
    """`e` as written as the operand of the cast `under` (in brackets where it takes them)."""
    return _sub(_token(under), e, 1) if under is not None else expr(e)


def _mk_fp_text(seg, off, comma=', ', shown=None):
    """MK_FP(seg, off) -- the stack's or the data segment's, written by a pass -- as c_text's
    _far_addresses made it: `((T __far *)MK_FP(seg, off))`, where it is moved (else as it was)."""
    if 'far_addresses' not in _now['moved']:
        return 'MK_FP(%s%s%s)' % (seg, comma, shown or expr(off))
    t, o = _far_target(off)
    return '((%s)MK_FP(%s, %s))' % (_pops(t + ' __far *'), seg, shown or _written(o, off if o is not off else None))


def _far_addresses(nodes, _arrays):
    """An address made of a segment the decompiler could not fold to a constant and an offset
    (`MK_FP(seg,(T *)off)`, its printing) as a far pointer to T: `((T __far *)MK_FP(seg, off))`.
    An offset with no cast is a pointer's own arithmetic (`MK_FP(seg,p + 2)`, p an `int2 *`: the
    word two words on) -- the place is a T as the pointer is; only an offset that is no pointer at
    all is taken a byte at a time. A far pointer's own halves put back together --
    `MK_FP((int2)((uint4)p >> 0x10),(T *)p + 2)`, or the high half kept in a variable first -- is
    the pointer's own arithmetic, `((T __far *)((T __far *)p + 2))`; a far pointer passed as its
    two halves, low then high (`f((int2)(void *)p,p._2_2_)`), is the pointer passed; and a
    variable that held the high half and is read nowhere else goes (c_text._far_addresses)."""
    parent = {id(e): p for e, p in nodes}
    params = [(_param_name(p), p.get('type')) for p in _now['doc'].get('params') or []]
    locs = [((v.get('sym') or {}).get('name'), v.get('type')) for v in _now['locals']
            if (v.get('sym') or {}).get('name') not in _now['drop']]
    far_names = set()
    for n, t in params + locs:
        far_names.update(_FAR_DECL.findall(type_text(t, n, n in _now['far']) + ';'))
    pointers = _now['far_pointers']
    high_vars, high_sets = {}, []
    for e, _p in nodes:
        if e['node'] == 'ExprStmt' and e['expr'].get('node') == 'Assign' and not e['expr'].get('op') and \
                e['expr']['target'].get('node') == 'Name':
            hp = _high_of(expr(e['expr']['value']))
            if hp is not None:
                high_sets.append((_name(e['expr']['target']), e))
                if hp in far_names:
                    high_vars[_name(e['expr']['target'])] = hp
    consumed = []                                        # (the high halves' names the rewrites take in)

    pointer_cast, leftmost = _pointer_cast, _leftmost

    def own_halves(seg, off):
        s = expr(seg).strip()
        p = _high_of(s) or high_vars.get(s)
        if p not in far_names:
            return None
        base = off
        for _ in range(12):                              # (through casts, brackets, a sum's first term)
            if id(base) in _now['whole']:
                if id(base) not in _now['gap_base']:
                    break
                base = _now['gap_base'][id(base)]        # (`((uint1 *)(p) + 0x1a)`: through its base)
            elif base.get('node') == 'Paren':
                base = base['x']
            elif base.get('node') == 'Cast':
                base = base['x']
            elif base.get('node') == 'Binary' and base.get('op') in ('+', '-'):
                base = base['left']
            else:
                break
        if base.get('node') == 'Name' and _shape(base) == 'identifier' and _name(base) == p:
            if s in high_vars and seg.get('node') == 'Name':
                consumed.append(seg)
            return p
        return None

    def halves_text(off, p, target):
        sum_ = _written(off['x'], off) if pointer_cast(off) else expr(off)
        # (the low half counted in bytes -- `(int2)(void *)p + 2` -- is the pointer moved on 2 bytes)
        # (`(void __far *)`: _far_casts, after it in c_text, has made that cast far by now)
        sum_ = re.sub(r"\(\s*u?int2\s*\)\s*(?:\(\s*void\s*(?:__far\s*)?\*\s*\)\s*)?%s\b" % re.escape(p),
                      "(uint1 __far *)%s" % p, sum_)
        sum_ = re.sub(r"\(\s*((?:(?!__far)[\w ])+?)\s*\*\s*\)\s*%s\b" % re.escape(p), r"(\1 __far *)%s" % p, sum_)
        if not re.search(r"__far \*\)\s*%s\b" % re.escape(p), sum_):
            sum_ = re.sub(r"\b%s\b" % re.escape(p), "(uint1 __far *)%s" % p, sum_, count=1)
        return "((%s __far *)(%s))" % ("uint1" if target in ("void", "code") else target, sum_)

    for e, p in nodes:
        parts = _mk_fp(e)
        if not parts or id(e) in _now['whole']:
            continue
        seg, off = parts
        hp = own_halves(seg, off)
        if hp is not None:
            target = pointer_cast(off) or pointer_cast(leftmost(off)) or 'uint1'
            _rewrite(e, lambda _x, off=off, hp=hp, t=target: halves_text(off, hp, t), room=True)
            continue
        target, offset = _far_target(off)
        up = p
        if up is not None and up.get('node') == 'Unary' and up.get('op') == '*' and not up.get('postfix'):
            call = parent.get(id(up))
            if call is not None and call.get('node') == 'Call' and call.get('target') is up:
                target = 'code'                          # (*MK_FP(seg, off))(...): a call to code there
        _rewrite(e, lambda _x, s=seg, o=offset, t=target, off=off:
                 '((%s)MK_FP(%s, %s))' % (_pops(t + ' __far *'), expr(s), _written(o, off if o is not off else None)),
                 room=True)
    # a far pointer passed whole, as its two halves
    merged = set()
    for e, _p in nodes:
        if e['node'] != 'Call' or id(e) in _now['whole'] or (e.get('target') or {}).get('node') != 'Name':
            continue
        up, inside = parent.get(id(e)), False
        while up is not None:
            if id(up) in merged:
                inside = True                            # (inside one already written so: as it is)
                break
            up = parent.get(id(up))
        args = e.get('args') or []
        if inside or len(args) < 2:
            continue
        new, k, changed = [], 0, False
        while k < len(args):
            lo = _LOW.fullmatch(expr(args[k])) if k + 1 < len(args) else None
            hi_text = expr(args[k + 1]).strip() if lo else ''
            hp = (_high_of(hi_text) or high_vars.get(hi_text)) if lo else None
            if lo and lo.group(1) in far_names and hp == lo.group(1):
                new.append((None, (' ' if k else '') + lo.group(1)))
                if hi_text in high_vars and args[k + 1].get('node') == 'Name':
                    consumed.append(args[k + 1])
                k += 2
                changed = True
            else:
                new.append((args[k], None))
                k += 1
        if changed:
            merged.add(id(e))
            _rewrite(e, lambda c, new=new: '%s(%s)' % (_name(c['target']), ','.join(
                t if a is None else expr(a) for a, t in new)))
    # (a far pointer's high half no longer used: gone)
    uses = {}
    for e, _p in nodes:
        if e['node'] == 'Name':
            uses[_name(e)] = uses.get(_name(e), 0) + 1
    for x in consumed:
        uses[_name(x)] -= 1
    for v in high_vars:
        if uses.get(v, 0) == 1 and v in dict(locs):
            for n, s in high_sets:
                if n == v:
                    _now['gone'].add(id(s))
            t = dict(locs).get(v)
            if t is not None and re.fullmatch(r"\w+", type_text(t)):
                _now['drop'].add(v)


def _first_name(e):
    """The name an expression's text starts with (`p` of `p->f`, `p[1]`, `p + 2`), else None --
    nothing printed before it."""
    while True:
        shown = _now['whole'].get(id(e))
        if shown is not None:
            m = re.match(r"([A-Za-z_]\w*)\b", shown(e))
            return m.group(1) if m else None
        n = e.get('node')
        if n == 'Name':
            return _name(e) if _now['marks'].get(id(e)) != '_ss' else None
        first = {'Binary': 'left', 'Assign': 'target', 'Call': 'target', 'Index': 'arr', 'Select': 'obj'}.get(n)
        if n == 'Unary' and e.get('postfix'):
            first = 'x'
        if not first or not e.get(first):
            return None
        child = e[first]
        if n == 'Select' and _arrow(e):
            child = child['x']
        if _bracket(_token(e), child, 0):
            return None
        e = child


def _ends_with_semicolon(e, parent):
    """Is a `;` written right after expression `e`: a statement of its own, or a for's start?"""
    p = parent.get(id(e))
    if p is not None and p.get('node') == 'ExprStmt':
        pp = parent.get(id(p))
        return not (pp is not None and pp.get('node') == 'For' and any(x is p for x in _stmts(pp.get('update'))))
    return p is not None and p.get('node') == 'For' and any(x is e for x in _stmts(p.get('init')))


def _far_casts(nodes, _arrays):
    """A far pointer read as another type stays far: `*(int2 *)p`, p a `void __far *`, is
    `*(int2 __far *)p` -- the near cast keeps only its offset, and reads it in DS. And a far
    pointer's own halves put back as it: `CONCAT22(<p's high word>,p)` is `(int4)p`; `<p's low
    word> = <p's offset> + k` -- the pointer moved on in its segment, as a far pointer's
    arithmetic is -- is `p = (uint1 __far *)p + k` (c_text._far_casts)."""
    names = set()
    params = [(_param_name(p), p.get('type')) for p in _now['doc'].get('params') or []]
    locs = [((v.get('sym') or {}).get('name'), v.get('type')) for v in _now['locals']
            if (v.get('sym') or {}).get('name') not in _now['drop']]
    for n, t in params + locs:
        names.update(_FAR_DECL.findall(type_text(t, n, n in _now['far']) + ';'))
    if not names:
        return
    parent = {id(e): p for e, p in nodes}
    for e, _p in nodes:
        if e['node'] != 'Cast' or id(e) in _now['whole'] or _now['marks'].get(id(e)):
            continue
        if re.fullmatch(r"(?:(?!__far)[\w ])+?\s*\*", type_text(e.get('type'))) and \
                not _bracket(_token(e), e['x'], 1) and _first_name(e['x']) in names:
            _now['marks'][id(e)] = '_far'
    for e, _p in nodes:
        if e['node'] == 'Call' and id(e) not in _now['whole'] and _name(e.get('target') or {}) == 'CONCAT22' and \
                len(e.get('args') or ()) == 2:
            hi, lo = (expr(a) for a in e['args'])
            for p in names:
                q = re.escape(p)
                high = (r"(?:\(\s*\*\s*\(\s*u?int2\s*(?:__far\s*)?\*\s*\)\s*\(\s*\(\s*char\s*(?:__far\s*)?\*\s*\)\s*&\s*%s\s*\+\s*2\s*\)\s*\)"
                        r"|\(\s*u?int2\s*\)\s*\(\s*\(\s*uint4\s*\)\s*%s\s*>>\s*0x10\s*\)|%s\._2_2_)" % (q, q, q))
                if re.fullmatch(r"\s*%s\s*" % high, hi) and re.fullmatch(r"\s*(?:\(\s*void\s*__far\s*\*\s*\)\s*)?%s\s*" % q, lo):
                    _rewrite(e, lambda _x, p=p: '(int4)%s' % p)
                    break
        if e['node'] == 'Assign' and not e.get('op') and id(e) not in _now['whole'] and _ends_with_semicolon(e, parent):
            target, value = expr(e['target']), expr(e['value'])
            for p in names:
                q = re.escape(p)
                low = r"\(\s*\*\s*\(\s*u?int2\s*(?:__far\s*)?\*\s*\)\s*\(\s*\(\s*char\s*(?:__far\s*)?\*\s*\)\s*&\s*%s\s*\+\s*0\s*\)\s*\)" % q
                off = re.fullmatch(r"\(\s*void\s*(?:__far\s*)?\*\s*\)\s*\(\s*\(\s*u?int2\s*\)\s*(?:\(\s*void\s*__far\s*\*\s*\)\s*)?%s"
                                   r"\s*([+-])\s*(0x[0-9a-fA-F]+|\d+)\s*\)" % q, value)
                if off and re.fullmatch(low, target):
                    _rewrite(e, lambda _x, p=p, o=off: '%s = (uint1 __far *)%s %s %s' % (p, p, o.group(1), o.group(2)))
                    break


_IN_STACK = re.compile(r"in_stack_([0-9a-f]{8})")
_STACK_PIECE = re.compile(r"_?([a-z]+)Stack([0-9a-f]{4})")


def _stack_arguments(nodes, _arrays):
    """Words of the caller's stack the function reads beyond its declared parameters: the
    decompiler declares them as locals (in_stack_0000000c -- which in C would hold nothing) or
    names a piece of them (_uStack000e). Each is that place: the stack segment, at the entry SP
    plus its offset, `codebreaker_stack(T, 0xc)` (c_text._stack_arguments)."""
    places = {}
    for v in _now['locals']:
        n = (v.get('sym') or {}).get('name') or ''
        m = _IN_STACK.fullmatch(n)
        if m and n not in _now['drop'] and int(m.group(1), 16) < 0x10000 and not _stack(v.get('type'))[0] \
                and n not in places:
            places[n] = (type_text(v.get('type')), int(m.group(1), 16))
            _now['drop'].add(n)
    declared = {(v.get('sym') or {}).get('name') for v in _now['locals']} - _now['drop']
    for e, _p in nodes:
        if e['node'] != 'Name' or (e.get('sym') or {}).get('kind') == 'field':
            continue
        n = _name(e)
        if n in places:
            _rewrite(e, lambda _x, p=places[n]: 'codebreaker_stack(%s, 0x%x)' % p)
            continue
        m = _STACK_PIECE.fullmatch(n)
        if m and n not in declared:
            ty = {'u': 'uint2', 'i': 'int2', 'c': 'char', 'b': 'uint1', 'x': 'xunknown2'}.get(m.group(1)[:1], 'uint2')
            _rewrite(e, lambda _x, t=ty, o=int(m.group(2), 16): 'codebreaker_stack(%s, 0x%x)' % (t, o))


def _seen():
    """The ids of the nodes the function's text shows as it is written now (each moved pass's
    rewrites in it, a statement taken away left out): what c_text's passes found in the text."""
    _now['seen'], _now['seeing'], _now['at'], _now['lines'] = set(), True, {}, set()
    _now['owners'], _now['owner_of'], _now['shown'], _now['stmt_at'] = [], {}, {}, {}
    _now['top'] = _Flat()
    try:
        _function(_now['doc'], _now['locals'], _now['top'])
        return _now['seen']
    finally:
        _now['seeing'] = False


def _cast_type(cast):
    """A cast's type as it is written now (`T __far *` where a pass marked it far)."""
    mark = _now['marks'].get(id(cast))
    if mark == '_near':
        return _flat(lambda em: _push_type(_Stack(em), cast['type'], word=' __near'))
    return type_text(cast.get('type'), far=mark in ('_far', '_ds'))


def _leading_cast_now(e):
    """The cast the text of `e` starts with as it is written now (none past a moved pass's rewrite)."""
    while True:
        if id(e) in _now['whole']:
            return None
        n = e.get('node')
        if n == 'Cast':
            return e
        first = {'Binary': 'left', 'Assign': 'target', 'Cond': 'cond', 'Call': 'target', 'Index': 'arr',
                 'Select': 'obj'}.get(n)
        if n == 'Unary' and e.get('postfix'):
            first = 'x'
        if not first or not e.get(first):
            return None
        child = e[first]
        if n == 'Select' and _arrow(e):
            child = child['x']
        if _bracket(_token(e), child, 0):
            return None
        e = child


def _bare_name(e, name=None):
    """Is `e` a variable's name written as it is (`name`, where given)?"""
    return e.get('node') == 'Name' and (e.get('sym') or {}).get('kind') != 'field' and \
        id(e) not in _now['whole'] and _now['marks'].get(id(e)) != '_ss' and (name is None or _name(e) == name)


_WIDE_LOCAL = ('xunknown4', 'uint4', 'int4', 'undefined4')


def _far_locals(nodes, _arrays):
    """A four-byte local the code reads through (`*x`) holds a far pointer: declared as one, to
    what the cast it is set from names (`x = (T *)CONCAT22(seg,off)`: `T __far *x`), and those
    casts made far -- a near one keeps the offset alone, and C gives it DS (USER's sub_43_032F
    copied from its own data segment instead of the block GlobalLock locked). `_p` beside a near
    pointer `p` -- the decompiler's name for the four bytes whose low half is p -- is declared a far
    pointer, its CONCAT22 casts far, and p read as its offset where nothing sets p. A pointer
    local set from another typed otherwise is cast to its own type (c_text._far_locals)."""
    seen = _seen()
    parent = {id(e): p for e, p in nodes}
    shown = [(e, p) for e, p in nodes if id(e) in seen]
    decls = _now['decls']
    locs = [v for v in _now['locals'] if (v.get('sym') or {}).get('name') not in _now['drop']]

    def decl_text(v):
        n = (v.get('sym') or {}).get('name') or ''
        if n in decls:
            return decls[n]
        if _segment_register(n, v.get('type')):
            return None                                  # (`T unaff_SS = codebreaker_ss()`)
        return type_text(v.get('type'), n, n in _now['far'])

    def sets(name, concat):
        """[(cast, T)] of `name = (T *)...` (`concat`: of `name = (T *)CONCAT22(...)` alone)"""
        out = []
        for e, _p in shown:
            if e['node'] != 'Assign' or e.get('op') or id(e) in _now['whole'] or not _bare_name(e['target'], name):
                continue
            cast = _leading_cast_now(e['value'])
            m = re.fullmatch(r"([\w ]+?) \*", _cast_type(cast)) if cast is not None else None
            v = cast['x'] if m else None
            if v is not None and (not concat or v.get('node') == 'Call' and id(v) not in _now['whole'] and
                                  _bare_name(v.get('target') or {}, 'CONCAT22')):
                out.append((cast, m.group(1)))
        return out

    def far_casts(found):
        for cast, to in found:
            if _now['marks'].get(id(cast)) in ('_far', '_ds'):
                _rewrite(cast, lambda c, to=to: '(%s)%s' % (_pops(to + ' __far *'), _written(c['x'], c)))
            else:
                _now['marks'][id(cast)] = '_far'
    for v in locs:
        n = (v.get('sym') or {}).get('name') or ''
        if decl_text(v) not in tuple('%s %s' % (w, n) for w in _WIDE_LOCAL):
            continue
        # (`*x` as written: not the `*` L2 reads `x->f` with)
        through = any(_bare_name(e, n) and _unary_parent(e, parent, '*') and not _arrow_of(parent.get(id(e)), parent)
                      for e, p in shown)
        if not through:
            continue
        found = sets(n, False)
        decls[n] = '%s __far *%s' % (found[0][1] if found else 'xunknown1', n)
        far_casts(found)
    # `_p` beside `p`: the four bytes whose low half is the near pointer p
    declared = {(v.get('sym') or {}).get('name') for v in locs}
    for v in locs:
        n = (v.get('sym') or {}).get('name') or ''
        d = decl_text(v)
        m = re.fullmatch(r"([\w ]+?) \*(\w+)", d or '')
        whole = '_' + n
        if not m or m.group(2) != n or whole in declared or \
                not any(_bare_name(e, whole) for e, _p in shown):
            continue
        to = m.group(1)
        assigned = any(_bare_name(e, n) and p is not None and (
            p.get('node') == 'Unary' and p.get('op') == '&' and not p.get('postfix') and id(p) not in _now['whole'] or
            p.get('node') == 'Index' and p['arr'] is e and id(p) not in _now['whole'] and
            _unary_parent(p, parent, '&')) for e, p in shown) or \
            any(e['node'] == 'Assign' and not e.get('op') and id(e) not in _now['whole'] and _bare_name(e['target'], n)
                for e, _p in shown)
        decls[n] = (d + ';\n  ' if assigned else '') + '%s __far *%s' % (to, whole)
        far_casts(sets(whole, True))
        if not assigned:
            for e, _p in shown:
                if _bare_name(e, n):
                    _rewrite(e, lambda _x, to=to, w=whole: '((%s)(uint2)(uint4)%s)' % (_pops(to + ' *'), w))
    # one pointer local set from another the decompiler typed otherwise: the value, in its type
    types = {}
    for v in locs:
        n = (v.get('sym') or {}).get('name') or ''
        for line in (decl_text(v) or '').split(';\n  '):
            m = re.fullmatch(r"([\w ]+?[\w ]*\*+)\s*(\w+)", line)
            if m:
                types[m.group(2)] = m.group(1).strip()
    for e, p in shown:
        if e['node'] != 'ExprStmt' or id(e) not in _now['lines']:
            continue
        a = e['expr']
        if a.get('node') != 'Assign' or a.get('op') or id(a) in _now['whole']:
            continue
        left, right = a['target'], a['value']
        if _bare_name(left) and _bare_name(right) and _name(left) in types and _name(right) in types and \
                types[_name(left)] != types[_name(right)]:
            _rewrite(right, lambda x, t=types[_name(left)]: '(%s)%s' % (t, _name(x)))


def _pops(ty):
    """A cast's type, `code *`, as _callee_pops writes it in a function whose calls through pointers
    remove nothing themselves: `code_callee_pops *` (`__far`, `__near` kept)."""
    m = re.fullmatch(r"code (__far |__near )?\*", ty) if _now['pops'] else None
    return "code_callee_pops %s*" % (m.group(1) or "") if m else ty


def _callee_pops(nodes, _arrays):
    """A function whose calls through pointers remove nothing themselves -- the callee does (the
    arguments are listed in stack order, so __stdcall lays them out as the original pushed them):
    each cast to `code *` written `code_callee_pops *`, the passes' casts too, as c_text wrote them
    over the text (the module's facts: "callee_pops"; c_text._callee_pops)."""
    if _now['facts'].get('callee_pops'):
        _now['pops'] = True


# A helper that works in its caller's frame -------------------------------------------------------------
# It makes no frame of its own and reaches places through BP as its caller left it -- KEYBOARD's
# sub_9_0A62, `xor ax, ax / mov [bp-4], ax / mov [bp-6], ax / ret`, which clears two of
# OEMTOANSI's locals. The decompiler writes `*(xunknown2 *)(unaff_BP + -4) = 0`, a BP the C never
# has, and the caller's locals stay unset. Each such place becomes a parameter -- a far pointer,
# the frame being in SS -- and each call passes the address of the caller's own local there:
# [bp-4] of a frame BP set up is entry SP - 6, the caller's xStack_6. The module decides which
# helpers and callers (caller_frame_plan, from what each function's writing found: "frame"); the
# functions it names are written again with what it decided ("frame_slots", "frame_calls").
_CALLER_SLOT = re.compile(r"\*\((\w+) \*\)\(unaff_BP \+ -(0x[0-9a-f]+|\d+)\)")
_CDECL16 = re.compile(r"__cdecl16(?:near|far)")
_CALLER_LOCAL = re.compile(r"(?m)^\s+([\w ]+?\**) ?(\w*Stack_([0-9a-f]+));$")
_CALLER_ARRAY = re.compile(r"(?m)^\s+[\w ]+?\**\s*\w*Stack_[0-9a-f]+ \[\d+\];")


def _named_functions(nodes):
    """{name: (times named, times called with no arguments)} of the names the function's text
    writes that a call could be of: each Name, each call by name alone."""
    out = {}
    for e, _p in nodes:
        if e['node'] == 'Name' and (e.get('sym') or {}).get('kind') != 'field':
            n = out.setdefault(_name(e), [0, 0])
            n[0] += 1
        if e['node'] == 'Call' and not e.get('args'):
            t = e.get('target')
            name = _name(t) if t is not None and t.get('node') == 'Name' else \
                (e.get('sym') or {}).get('name') if t is None else None
            if name:
                n = out.setdefault(name, [0, 0])
                n[1] += 1
                if t is None:
                    n[0] += 1
    return {k: tuple(v) for k, v in out.items()}


def _caller_frame_slots(nodes, _arrays):
    """What the module decides a helper working in its caller's frame from (above): the function's
    places through BP (`frame_slots` where it is such a helper), its locals of the frame as they
    are declared, whether it uses BP itself, the functions it names -- facts["frame"]; and, given
    what the module decided, the helper's places as its parameters (its heading in the older style,
    `f(frame_4,frame_6)` then `T __far *frame_4;`, BP's local gone) or each plain call of the
    helper in a caller given the addresses of the caller's locals there, a local it does not
    declare declared first (c_text._caller_frame_slots, which did it on the module's texts)."""
    facts = _now['facts']
    doc = _now['doc']
    locs = [v for v in _now['locals'] if (v.get('sym') or {}).get('name') not in _now['drop']]
    seen = _seen()
    # (the locals' declarations as they are written: _Locals.region)
    region = _now['locals_text'] if _now['locals_text'] is not None else \
        ''.join('\n  %s;' % _decl_now(v) for v in locs) + ('\n  ' if locs else '')
    own = {int(m.group(3), 16): (m.group(1), m.group(2)) for m in _CALLER_LOCAL.finditer(region)}
    bp = 'unaff_BP' in region or any(e['node'] == 'Name' and _name(e) == 'unaff_BP' for e, _p in nodes if id(e) in seen)
    report = {'locals': own, 'arrays': bool(_CALLER_ARRAY.search(region)), 'bp': bp,
              'named': _named_functions([(e, p) for e, p in nodes if id(e) in seen]), 'region': region}
    # (a helper: `__cdecl16near f(void)`, `int2 unaff_BP;`, every use of it a place below BP)
    _result, mods, words, _far_model = _heading(doc)
    heads = mods + words
    slots, places = None, []
    if heads and _CDECL16.fullmatch(heads[-1]) and not doc.get('params') and \
            not ((doc.get('sym') or {}).get('type') or {}).get('variadic') and \
            re.search(r"(?m)^\s+int2 unaff_BP;$", region):
        uses = sum(1 for e, _p in nodes if id(e) in seen and e['node'] == 'Name' and _name(e) == 'unaff_BP')
        for e, _p in nodes:
            if id(e) in seen and e['node'] == 'Unary' and e.get('op') == '*' and not e.get('postfix'):
                m = _CALLER_SLOT.fullmatch(expr(e))
                if m:
                    places.append((e, m.group(1), int(m.group(2), 0)))
        found = {}
        for _e, ty, less in places:
            found.setdefault(less, ty)
        if found and uses == len(places) and all(t in SIZEOF for t in found.values()):
            slots = found
    report['slots'] = slots
    facts['frame'] = report
    given = facts.get('frame_slots')
    if given and slots:
        _now['old_style'] = [('frame_%x' % less, '%s __far *frame_%x' % (given[less], less)) for less in sorted(given)]
        for e, ty, less in places:
            _rewrite(e, lambda _x, t=ty, k=less: '*frame_%x' % k if t == given[k] else '*(%s __far *)frame_%x' % (t, k))
        if _now['locals_text'] is not None:
            _now['locals_text'] = re.sub(r"(?m)^\s+int2 unaff_BP;\n", "", _now['locals_text'], count=1)
        else:
            _now['drop'].add('unaff_BP')
    calls = facts.get('frame_calls') or {}
    if calls:
        for e, _p in nodes:
            if e['node'] != 'Call' or e.get('args') or id(e) in _now['whole']:
                continue
            t = e.get('target')
            name = _name(t) if t is not None and t.get('node') == 'Name' else \
                (e.get('sym') or {}).get('name') if t is None else None
            if name in calls:
                _rewrite(e, lambda _x, n=name: '%s(%s)' % (n, calls[n]))
        _now['declare_first'] = list(facts.get('frame_declare') or ())


_SELECTOR_VALUE = re.compile(r"0x[dD]0[0-9a-fA-F]{2}")


def _selector_imports(nodes, _arrays):
    """An import's value used bare -- `mov es, __0040H`, `test ax, __WINFLAGS`: the decompiler
    shows the made-up number the description gave the import (describe: its selector_imports,
    the module's facts "selector_values" {number: (import, "segment" or "offset")}). It is the
    import, `FP_SEG(&__0040H)` (the loader fills in its selector) or `FP_OFF(&__WINFLAGS)`; the
    imports so used are found for the module's header (facts["selector_imports"]; c_text's
    _compilable16 did it on the module's texts)."""
    values = _now['facts'].get('selector_values')
    if not values:
        return
    seen = _seen()
    used = _now['facts'].setdefault('selector_imports', {})
    for e, _p in nodes:
        if e['node'] != 'Lit' or id(e) not in seen or id(e) in _now['whole']:
            continue
        spelled = _lit(e)
        if _SELECTOR_VALUE.fullmatch(spelled) and int(spelled, 16) in values:
            imp, how = values[int(spelled, 16)]
            used[imp] = how
            _rewrite(e, lambda _x, s="%s(&%s)" % ("FP_SEG" if how == "segment" else "FP_OFF", imp): s)


_FAR_OFFSET = re.compile(r"(-?)0x([0-9a-f]+)")


def _segment_values(nodes, _arrays):
    """A segment of the module's own as a value -- its selector passed on, pushed, left in AX:
    the decompiler shows the made-up selector the description gave it. Where the function's own
    code holds a loader fix-up for that segment's selector (a number like 1000h is as often a
    plain constant), the module says what it is (facts "segment_values" {spelled: C}): the
    automatic data segment is `codebreaker_ds()`; a code segment, the selector of a far function
    in it (the loader fixes that one up); another data segment, by the bytes the build carries
    for it. And a far pointer passed whole -- its offset the argument before its selector: a
    function's place ("segment_places" {(segment, offset): function}), written by the function's
    name (USER's sub_1_3514 passes the callback 1:3452 to an enumeration as `0x3452, <segment
    1>`: a number, it would still go where the function was if that one moved). c_text's
    _compilable16 did both on the module's texts."""
    values = _now['facts'].get('segment_values')
    if not values:
        return
    seen = _seen()
    for e, _p in nodes:
        if e['node'] == 'Lit' and id(e) in seen and id(e) not in _now['whole'] and _lit(e) in values:
            _rewrite(e, lambda _x, s=values[_lit(e)]: s)
    by_expr = {s: sg for s, sg in (_now['facts'].get('segment_of') or {}).items()}
    places = _now['facts'].get('segment_places') or {}
    for e, _p in nodes:
        args = [x for x in e.get('args') or [] if id(x) in seen] if e['node'] == 'Call' else []
        for a, b in zip(args, args[1:]):
            sg = by_expr.get(expr(b).strip())
            if sg is None or id(a) in _now['whole'] or not (
                    a['node'] == 'Lit' or a['node'] == 'Unary' and a.get('op') == '-' and not a.get('postfix')):
                continue
            lit_ = _FAR_OFFSET.fullmatch(expr(a).replace(' ', ''))
            if not lit_:
                continue
            fn = places.get((sg, (-int(lit_.group(2), 16) if lit_.group(1) else int(lit_.group(2), 16)) & 0xFFFF))
            if fn:
                _rewrite(a, lambda _x, f=fn: "((uint2)(uint4)(void __far *)&%s)" % f)


# Data kept in a code segment, read through CS ------------------------------------------------------
# KEYBOARD's GetKeyNameText looks a key up in tables beside its own code (9:0261, 9:02E9 ...) and
# reads them through CS: `MK_FP(unaff_CS, pcVar11)` with pcVar11 set to 0x261. The rebuilt code
# segment is laid out by the compiler, so CS:0261 is not the table any more. The source such a
# module was written from named the table, in the code segment: so does this. Writing each
# function finds the numbers that may be places in its code segment's data (_code_segment_reads:
# facts["code_reads"]); the module lays the data out (c_text._code_segment_data) and writes each
# place found as the member it is (facts "code_places" {the number's node: its C}).
_CS_WORD = re.compile(r"\s*(?:x?unknown2|u?int2|undefined2)\s+__far\s*\*\s*$")
_CS_NUMBER = re.compile(r"(0x[0-9a-fA-F]+|\d+)[uUlL]*")


def _code_segment_reads(nodes, _arrays):
    """Where the function's C reads its own code segment, and the numbers each read's offset may
    come from (c_text's _cs_reads and _cs_places, done on the tree as the text is written now):
    in each offset read through CS, a number added to things that are not pointers is the place
    (`(uVar6 & 0xff) * 2 + 0x1c7`, the table, indexed); a variable is followed to what is put in it
    (`pcVar11 = (char *)0x261`), and so on; a number added to a pointer is a step from the place
    (`psVar13 + 1`). A variable that is not a pointer and is also set from arithmetic (`uStack_c =
    (a5 + -1) * 2`) is an index: its numbers are not places (MINIKBD's `uStack_c = 0`). What is
    read through CS and then used as an offset makes the place it was read from a table of places:
    a word read so ("pointers") is written as the place it holds; anything else cannot be (->
    unsafe). Whether each number is in the segment's data is the module's to say: every one is
    reported, by its node. Given "code_places", those numbers are written as the module says."""
    given = _now['facts'].get('code_places')
    if given:
        for k, text in given.items():
            _rewrite(nodes[k][0], lambda _x, t=text: t)
        return
    seen = _seen()
    if 'codebreaker_cs()' not in ''.join(_now['top'].out):
        return
    parent = {id(e): p for e, p in nodes}
    index_of = {id(e): k for k, (e, _p) in enumerate(nodes)}
    shown = [e for e, _p in nodes if id(e) in seen]
    types, sets, cs = {}, {}, set()
    for v in _now['locals']:
        n = (v.get('sym') or {}).get('name') or ''
        if n in _now['drop']:
            continue
        left, eq, init = _decl_now(v).partition(' = ')
        types[n] = 'p *' if '*' in left else 'v'
        if init.strip() == 'codebreaker_cs()':
            cs.add(n)
        elif eq:                                         # (`unaff_SS = codebreaker_ss()`: a call)
            sets.setdefault(n, []).append({'node': 'Call', 'target': {'node': 'Name', 'sym': {'name': init}}, 'args': []})

    def ident(n):
        """the name a variable is written as (a pass may write another: KRNL386's int_DI), or None"""
        if n.get('node') != 'Name' or (n.get('sym') or {}).get('kind') == 'field':
            return None
        text = expr(n) if id(n) in _now['whole'] or _now['marks'].get(id(n)) else _local_text(_name(n))
        return text if re.fullmatch(r"[A-Za-z_]\w*", text) else None
    for e in shown:
        if e['node'] == 'Assign' and not e.get('op') and ident(e['target']):
            sets.setdefault(ident(e['target']), []).append(e['value'])

    def opaque(n):
        """`(T)-x`, `(T)*p`: read by c_text's parser as an operation on the cast and x"""
        x = n.get('x') if n.get('node') == 'Cast' and id(n) not in _now['whole'] else None
        return x is not None and x.get('node') == 'Unary' and not x.get('postfix') and \
            x.get('op') in ('*', '&', '-', '+') and not _unary_as_read(x, x['op'], parent)

    def bare(n):
        while n is not None and n.get('node') in ('Paren', 'Cast') and id(n) not in _now['whole'] and not opaque(n):
            n = n.get('x')
        return n

    def is_cs(n):
        n = bare(n)
        return n is not None and (expr(n).strip() == 'codebreaker_cs()' or
                                  ident(n) in cs)

    def printed(n):
        """is `n` written in the text, or a part of it (a pass may write what is under a cast)?"""
        return any(id(x) in seen for x, _p in _walk(n))

    def mk_fp(n):
        n = bare(n)
        if n is None or n.get('node') != 'Call' or len(n.get('args') or ()) != 2:
            return None
        t = n.get('target')
        fn = _name(t) if t is not None and t.get('node') == 'Name' else (n.get('sym') or {}).get('name')
        return n['args'] if fn == 'MK_FP' and all(printed(a) for a in n['args']) else None

    reads, far = [], []
    for e, _p in nodes:
        if e['node'] != 'Call':
            continue
        t = e.get('target')
        fn = _name(t) if t is not None and t.get('node') == 'Name' else (e.get('sym') or {}).get('name') if t is None else None
        args = [a for a in e.get('args') or () if printed(a)]
        if fn == 'MK_FP' and len(args) == 2 and is_cs(args[0]):
            # (what is read there called: a table of the code's own addresses)
            up, called = e, False
            while parent.get(id(up)) is not None:
                p = parent[id(up)]
                if p.get('node') == 'Call' and p.get('target') is up:
                    called = True
                up = p
            reads.append((args[1], 'call' if called else ''))
        elif fn != 'MK_FP':
            for a, b in zip(args, args[1:]):
                if is_cs(b) and not is_cs(a):
                    far.append(a)
    for at, struct, sel in _now['seg_ptrs']:
        # (`SEG_PTR(seg_f_unaff_CS, unaff_CS)`, _selector_structs' text: a call whose second
        # argument is CS, the structure's name before it, as c_text's parser read it)
        if at in seen and is_cs(sel):
            far.append({'node': 'Name', 'sym': {'kind': 'local', 'name': struct}})
    if not reads and not far:
        return
    places, events, unknown, unsafe, visited = {}, [], [], [], set()

    def pointer(n):
        return '*' in types.get(ident(n) or '', '')

    def through_cs(t):
        """(a word or not, the offset) when `t` is a value read through CS"""
        if t.get('node') == 'Unary' and t.get('op') == '*' and not t.get('postfix') and id(t) not in _now['whole'] \
                and _star_as_read(t, parent) and not _arrow_of(t, parent):
            arg = t['x']
            while arg.get('node') == 'Paren':
                arg = arg['x']
            m = re.match(r"\(*\s*\(([^()]*\*\s*)\)", expr(arg))
            word = bool(m and _CS_WORD.match(m.group(1)))
        elif t.get('node') == 'Select' and id(t) not in _now['whole']:
            arg, word = (t['obj']['x'] if _arrow(t) else t['obj']), False
        else:
            return None
        args = mk_fp(arg)
        if args and ('codebreaker_cs()' in expr(args[0]) or ident(args[0]) in cs):
            return word, args[1]
        return None

    def from_cs(t, depth):
        got = through_cs(t)
        if got is None:
            return
        word, e = got
        if word:
            for k_v in follow(e, None, depth + 1):
                events.append(('pointers', k_v))
        else:
            unsafe.append(expr(t)[:60])

    def is_index(name):
        if '*' in types.get(name, '*'):
            return False
        for rhs in sets.get(name, ()):
            rb = bare(rhs)
            if rb is None or rb.get('node') in ('Lit', 'Name') or through_cs(rb):
                continue
            if rb.get('node') == 'Binary' and rb.get('op') in ('+', '-'):
                continue
            return True
        return False

    def terms(n, sign=1):
        n = bare(n)
        if n is None:
            return []
        if n.get('node') == 'Binary' and n.get('op') in ('+', '-') and id(n) not in _now['whole']:
            return terms(n['left'], sign) + terms(n['right'], sign if n['op'] == '+' else -sign)
        if n.get('node') == 'Unary' and n.get('op') == '-' and not n.get('postfix') and id(n) not in _now['whole']:
            return terms(n['x'], -sign)
        return [(n, sign)]

    def node_terms(e):
        out = []
        for t, sign in terms(e):
            if t.get('node') == 'Unary' and t.get('op') == '&' and not t.get('postfix') and id(t) not in _now['whole']:
                arg = bare(t['x'])
                if arg is not None and arg.get('node') == 'Select':          # (&p->f: a place in p)
                    t = bare(arg['obj']['x'] if _arrow(arg) else arg['obj'])
                    if t is None:
                        continue
            out.append((t, sign))
        return out

    def number(t):
        m = _CS_NUMBER.fullmatch(expr(t)) if t.get('node') == 'Lit' and id(t) in seen else None
        return int(m.group(1), 0) if m else None

    def follow(e, self_name=None, depth=0):
        """the numbers an offset `e` may come from, each (node, value)"""
        if depth > 12:
            return set()
        found = set()
        ts = node_terms(e)
        numbers = []
        for t, s in ts:
            v = number(t)
            if v is not None and (s > 0 and v > 0 or (v == 0 and len(ts) == 1 and
                                                      (self_name is None or '*' in types.get(self_name, '')))):
                numbers.append((t, v))
        names = [t for t, _s in ts if ident(t)]
        steps = any(pointer(t) for t in names) or (self_name is not None and self_name in [ident(t) for t in names])
        for t, v in numbers:
            if not steps:
                places[index_of[id(t)]] = v
                found.add((index_of[id(t)], v))
        for t, _s in ts:
            from_cs(t, depth)
        if numbers and not steps:
            return found
        for t in names:
            name = ident(t)
            if name == self_name or name in visited:
                continue
            visited.add(name)
            if name not in sets:
                unknown.append(name)
            if is_index(name):
                continue
            for rhs in sets.get(name, ()):
                found |= follow(rhs, name, depth + 1)
        return found
    each = []
    for e, use in reads:
        visited.clear()
        got = follow(e)
        if use == 'call':
            events += [('call', k_v) for k_v in sorted(got)]
        each.append(sorted(got))
    for e in far:
        visited.clear()
        each.append(sorted(follow(e)))
    _now['facts']['code_reads'] = {'reads': len(reads) + len(far), 'places': places, 'events': events,
                                   'unknown': unknown, 'each': each, 'unsafe': unsafe}


_STRING = re.compile(r'"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'|/\*.*?\*/', re.S)
_IDENT = re.compile(r"(?<![\w.>])[A-Za-z_]\w*")
_CALL_IN = re.compile(r"(?<![\w.>])([A-Za-z_]\w*)\s*\(")
_THROUGH_IN = re.compile(r"\(\*\s*([a-z]+Ram[0-9a-f]{8})\s*\)\s*\(")


def _names_used(nodes, _arrays):
    """What the module's header is written from, of the function as it is written now (c_text's
    _compilable16 read it off every function's text): the globals it names by their address
    (`iRam10080012`: "globals", as (prefix, address)), those it calls through ("called_through"),
    the functions and helpers it calls ("called"), the other data segments it names
    (codebreaker_segment_<n>: "segments") and whether it makes a far pointer with MK_FP -- from
    the names and calls of the tree, and those in the text the passes wrote ->
    facts["names"]."""
    seen = _seen()
    names, called, through = set(), set(), set()
    plain = lambda x: id(x) in seen and id(x) not in _now['whole']
    for e, _p in nodes:
        if e['node'] == 'Name' and (e.get('sym') or {}).get('kind') != 'field' and plain(e) \
                and not _now['marks'].get(id(e)):
            names.add(_local_text(_name(e)))
        elif e['node'] == 'Call':
            t = e.get('target')
            if t is None:
                if plain(e):
                    n = (e.get('sym') or {}).get('name')
                    names.add(n)
                    called.add(n)
            elif t.get('node') == 'Name' and id(t) in seen:
                called.add(_local_text(_name(t)) if plain(t) else expr(t))
            else:
                while t.get('node') == 'Paren' and plain(t):
                    t = t['x']
                if t.get('node') == 'Unary' and t.get('op') == '*' and not t.get('postfix') and plain(t) and \
                        t['x'].get('node') == 'Name' and id(t['x']) in seen:
                    through.add(_name(t['x']) if plain(t['x']) else expr(t['x']))
    wrote = list(_now['shown'].values()) + [t for k in ('before', 'after') for ts in _now[k].values() for t in ts]
    wrote += ['%s %s;' % d for d in _now['declare_first']] + [_now['locals_text'] or '']
    for text in wrote:                                   # (what the passes wrote: its names, its calls)
        plain = _STRING.sub('""', text)
        names |= set(_IDENT.findall(plain))
        called |= set(_CALL_IN.findall(plain))
        through |= set(_THROUGH_IN.findall(plain))
    for v in _now['locals']:                             # (a declaration a pass wrote: `= codebreaker_ss()`)
        if (v.get('sym') or {}).get('name') not in _now['drop']:
            plain = _decl_now(v)
            names |= set(_IDENT.findall(plain))
            called |= set(_CALL_IN.findall(plain))
    _now['facts']['names'] = {
        'globals': {m.groups() for m in (_RAM.fullmatch(n or '') for n in names) if m},
        'called_through': {n for n in through if _RAM.fullmatch(n)},
        'called': called, 'mk_fp': 'MK_FP' in called,
        'segments': {int(m.group(1)) for m in (re.fullmatch(r"codebreaker_segment_(\d+)", n or '') for n in names) if m}}


# Prototypes where the calls agree ------------------------------------------------------------------
# The source had prototypes, and with none Microsoft C gives other code for a call (it keeps what it
# passes in a register variable, and removes the arguments itself before a frame's `leave`). Each
# function is written in the older style; one every call to which passes as many words in each
# place as its definition takes there is declared with its parameters, the others with the list
# left open (the decompiler recovers each call on its own evidence). Writing each function finds
# its heading, its variables' types and its calls (_heading_facts: facts["heading"]); the module
# decides (prototypes).
_DEFINITION = re.compile(r"^([^\n#{};]*?\b(\w+))\(([^)]*)\)\n((?:[^{;\n]+;\n)*)\{", re.M)
_WIDE_TYPES = re.compile(r"\b(u?int4|xunknown4|long|unkbyte3|unkint3|unkuint3|int3|float4)\b")
_VARIABLE = re.compile(r"^\s+([A-Za-z_][\w ]*?[\w*])\s*\**\s*(\w+)(\s*\[[^\]]*\])?\s*;\s*$")
_CALLED = re.compile(r"\b(\w+)\s*\(")


def arguments_at(text, open_paren):
    """The arguments of the call whose `(` is at `open_paren`. -> ([argument text], index after `)`)"""
    depth, args, start, i = 0, [], open_paren + 1, open_paren
    while i < len(text):
        ch = text[i]
        if ch == "(":
            depth += 1
        elif ch == ")":
            depth -= 1
            if depth == 0:
                if text[start:i].strip():
                    args.append(text[start:i])
                return args, i + 1
        elif ch == "," and depth == 1:
            args.append(text[start:i])
            start = i + 1
        i += 1
    return args, len(text)


def words(ty):
    """the words a value of C type `ty` takes on the stack -- 1, 2 -- or None"""
    ty = " ".join(ty.split())
    if "*" in ty:
        return 2 if "__far" in ty else 1
    if _WIDE_TYPES.search(ty):
        return 2
    if re.search(r"\b(u?int2|xunknown2|int|unsigned|short|code|u?int1|char|xunknown1|bool)\b", ty):
        return 1
    return None


def parameters(text):
    """(the heading before the parameter list, [(name, type)]) of a definition as it is written in
    the older style (`f(a1,a2)` then `int2 a1;` ...), or None. (A list in the brackets is taken
    as names, each of the type `int`, as c_text read it.)"""
    m = _DEFINITION.search(text)
    if not m:
        return None
    decl = {}
    for d in m.group(4).strip().splitlines():
        d = d.strip().rstrip(";")
        nm = re.findall(r"\w+", d)
        if nm:
            decl[nm[-1]] = d[:d.rindex(nm[-1])].strip()
    names = [p.strip() for p in m.group(3).split(",") if p.strip()]
    head = []                                        # (the head may run over lines: `int2 __stdcall16far` / `name(`)
    for line in reversed(text[:m.end(2)].split("\n")):
        s = line.strip()
        if head and (not s or s.endswith((";", "}", "*/")) or s.startswith(("#", "extern ", "/*"))):
            break
        head.insert(0, s)
    return " ".join(head), [(p, decl.get(p, "int")) for p in names]


def argument_words(arg, types):
    """the words an argument as written takes on the stack, as far as it can be told, else None"""
    a = arg.strip()
    while a.startswith("(") and arguments_at(a, 0)[1] == len(a):
        a = a[1:-1].strip()                          # (the whole in brackets)
    if re.fullmatch(r"-?(0x[0-9a-fA-F]+|\d+)", a):
        v = int(a, 0)
        return 1 if -0x8000 <= v <= 0xFFFF else 2
    if re.fullmatch(r"-?(0x[0-9a-fA-F]+|\d+)[lL]", a):
        return 2
    cast = re.match(r"\(\s*([A-Za-z_][\w ]*\**)\s*\)", a)
    if cast and arguments_at(a, 0)[1] == cast.end():
        return words(cast.group(1))
    if re.fullmatch(r"[A-Za-z_]\w*", a):
        if a in ("unaff_SS", "unaff_DS", "unaff_ES", "unaff_CS"):
            return 1
        return words(types[a]) if a in types else None
    if a.startswith("&") or a.startswith('"'):
        return 1                                     # (a near address: the medium model's data is near)
    return None


def _heading_facts(nodes, _arrays):
    """The function's heading as it is written (its words before the brackets; its parameters and
    their types as its definition in the older style gives them), the types of its variables, and
    each call it makes with its arguments as they are written (of the tree's calls, and of those
    in the text the passes wrote) -> facts["heading"], for prototypes."""
    doc = _now['doc']
    seen = _seen()
    sig = _flat(_signature, doc)
    old = _now['old_style']
    # (as _function lays it out: in the older style `{` follows the declarations; else a blank
    # line is between -- which c_text's pattern for a definition did not take)
    definition = sig + '\n' + (''.join('%s;\n' % d for _n, d in old) if old is not None else '\n') + '{'
    params = parameters(definition)
    locs = [v for v in _now['locals'] if (v.get('sym') or {}).get('name') not in _now['drop']]
    region = ''.join('\n  %s %s;' % d for d in reversed(_now['declare_first'])) + (
        _now['locals_text'] if _now['locals_text'] is not None else ''.join('\n  %s;' % _decl_now(v) for v in locs))
    types = dict(params[1]) if params else {}
    for line in region.splitlines():
        mm = _VARIABLE.match(line)
        if mm and mm.group(1) not in ("return", "goto"):
            star = line.count("*")
            types[mm.group(2)] = mm.group(1) + (" *" * star if star else "") + (" *" if mm.group(3) else "")
    calls = []

    def from_text(text):
        for mm in _CALLED.finditer(text):
            calls.append((mm.group(1), arguments_at(text, mm.end() - 1)[0]))
    for e, _p in nodes:
        if e['node'] == 'Call' and id(e) in seen and id(e) not in _now['whole']:
            from_text(expr(e))
    for text in list(_now['shown'].values()) + [t for k in ('before', 'after') for ts in _now[k].values() for t in ts]:
        from_text(text)
    head = ' '.join(sig.split('(', 1)[0].split())
    _now['facts']['heading'] = {'params': params, 'types': types, 'calls': calls,
                                'declaration': head + '();', 'fastcall': '__fastcall16near' in head}


def prototypes(headings, order):
    """{name: a declaration with its parameters} for each function every call to which, in all of
    the module, passes as many words in each place as its definition takes there (`headings`
    {function: facts["heading"]}) -- each parameter declared with the type its definition in the
    older style gives it, as it is (`short`, not the `int` it is promoted to: both Microsoft C 8
    and OpenWatcom take it so). A function a call disagrees with keeps the open declaration."""
    params = {n: (headings.get(n) or {}).get('params') for n in order}
    want = {n: [words(t) for _p, t in ps[1]] for n, ps in params.items() if ps and "..." not in ps[0]}
    ok = {n: all(w is not None for w in ws) for n, ws in want.items()}
    for caller in order:
        h = headings.get(caller) or {}
        types = h.get('types') or {}
        for callee, args in h.get('calls') or ():
            if not ok.get(callee):
                continue
            if len(args) != len(want[callee]) or \
                    any(argument_words(a, types) != w for a, w in zip(args, want[callee])):
                ok[callee] = False
            elif "__stdcall" in params[callee][0] and \
                    any(re.search(r"\b(unaff_[SD]S|codebreaker_[sd]s\(\))|>>\s*0x10", a) for a in args):
                ok[callee] = False                   # (a far pointer passed as halves: build/msc6.py's
                                                     #  pascal_order makes it one argument -- not two words)
    out = {}
    for n, good in ok.items():
        if not good:
            continue
        head, ps = params[n]
        listed = ", ".join("%s %s" % (t, p) for p, t in ps)
        out[n] = " ".join(head.split()) + "(" + (listed or "void") + ");"
    return out


def segment_plan(functions, near, where, sites, code, data_segment, other_data, first_selector):
    """Which functions' C names a segment of the module's own by its made-up selector, and what it
    is there (_segment_values) -- `functions` [(address, name)] in address order, `near` the near
    ones, `where` {address: (segment, offset)}, `sites` {segment: [(offset, segment whose selector
    the loader puts there)]}, `code` the code segments. -> {function: its facts}"""
    own = {}
    for va, n in functions:
        sg = (where.get(va) or (None,))[0]
        if sg is not None and n not in near:
            own.setdefault(sg, n)
    sel_expr = {}
    for sg in set(code) | ({data_segment} if data_segment else set()):
        made_up = first_selector + 8 * (sg - 1)
        if sg == data_segment:
            sel_expr[made_up] = "codebreaker_ds()"
        elif sg in own:
            sel_expr[made_up] = "((uint2)((uint4)(void __far *)&%s >> 16))" % own[sg]
    for sg in other_data:
        sel_expr[first_selector + 8 * (sg - 1)] = "((uint2)((uint4)(void __far *)codebreaker_segment_%d >> 16))" % sg
    if not sel_expr:
        return {}
    starts = {}
    for va, n in functions:
        if where.get(va):
            starts.setdefault(where[va][0], []).append(where[va][1])
    for sg in starts:
        starts[sg].sort()
    at_place = {where[va]: n for va, n in functions if where.get(va)}
    plan = {}
    for va, name in functions:
        if not where.get(va):
            continue
        sg, off = where[va]
        later = [o for o in starts[sg] if o > off]
        end = later[0] if later else 0x10000
        wanted = {first_selector + 8 * (t - 1) for (o, t) in sites.get(sg, ()) if off <= o < end}
        here = {v: e for v, e in sel_expr.items() if v in wanted}
        if here:
            plan[name] = {"segment_values": {"0x%x" % v: e for v, e in here.items()},
                          "segment_of": {e: (v - first_selector) // 8 + 1 for v, e in here.items()},
                          "segment_places": {p: n for p, n in at_place.items()
                                             if p[0] in {(v - first_selector) // 8 + 1 for v in here}}}
    return plan


def caller_frame_plan(reports, frames):
    """The module's decision for the helpers that work in their callers' frames (above), from what
    writing each function found (`reports` {function: facts["frame"]}, in the module's order;
    `frames` {function: does it set up BP as its frame}) -- only where every place is below BP,
    every caller keeps a frame of its own, and every use of the helper is a plain call of it.
    -> {function: the facts it is written again with}."""
    plan = {}
    region = {n: r['region'] for n, r in reports.items() if r}     # (with the declarations put first)
    for name, rep in reports.items():
        slots = (rep or {}).get('slots')
        if not slots:
            continue
        callers = {n: r for n, r in reports.items() if n != name and r and name in r['named']}
        if not callers or any(not frames.get(n) or r['bp'] or r['named'][name][0] != r['named'][name][1]
                              for n, r in callers.items()):
            continue
        found, ok = {}, True
        for n, r in callers.items():
            for less, ty in slots.items():
                got = r['locals'].get(less + 2)
                width = 2 if got and "*" in got[0] else SIZEOF.get(got[0].strip()) if got else None
                if got is None and r['arrays'] or got is not None and width != SIZEOF[ty]:
                    ok = False
                found[(n, less)] = got[1] if got else None
        if not ok:
            continue
        plan.setdefault(name, {})['frame_slots'] = dict(slots)
        for n in callers:
            args = []
            for less in sorted(slots):
                local = found[(n, less)] or "xStack_%x" % (less + 2)      # (a place the caller's C never reads)
                if not re.search(r"(?m)^\s+[\w ]+?\**\s*%s;$" % local, region[n]):
                    plan.setdefault(n, {}).setdefault('frame_declare', []).append((slots[less], local))
                    region[n] = "\n  %s %s;" % (slots[less], local) + region[n]
                # (the far address as the back end writes one: _far_addresses)
                args.append("(%s __far *)((uint1 __far *)MK_FP(codebreaker_ss(), &%s))" % (slots[less], local))
            plan.setdefault(n, {}).setdefault('frame_calls', {})[name] = ",".join(args)
    return plan


def _far_concat_casts(nodes, _arrays):
    """`(T *)CONCAT22(seg,off)` wherever it stands is `(T __far *)CONCAT22(seg,off)`: four bytes, a
    segment and an offset, cast to a near pointer keep the offset alone and C gives it DS (SETUPX's
    sub_31_002A passed four far pointers on so; c_text._far_concat_casts)."""
    for e, _p in nodes:
        if e['node'] != 'Cast' or id(e) in _now['whole'] or _now['marks'].get(id(e)):
            continue
        v = e['x']
        if v.get('node') == 'Call' and id(v) not in _now['whole'] and _bare_name(v.get('target') or {}, 'CONCAT22') \
                and re.fullmatch(r"([\w ]+?) \*", type_text(e.get('type'))):
            _now['marks'][id(e)] = '_far'


def _local_text(name):
    """A local's name as it is written now: what _buffer_fields made it (`a[1]`, a buffer's field,
    `codebreaker_frame.a`), else itself."""
    made = _now['ren'].get(name)
    return made() if made else name


def _decl_now(v):
    """A local's declaration as _function writes it now (without its `;`)."""
    n = (v.get('sym') or {}).get('name') or ''
    if n in _now['decls']:
        return _now['decls'][n]
    return _flat(_put_type, v.get('type'), n, n in _now['far'], _segment_register(n, v.get('type')))


def _prefix(e):
    """The text written before node `e` (its own text not), as _seen found it -- in the function's
    text, or, inside a moved pass's rewrite, in the part of it `e` is written in; None if unseen."""
    at = _now['at'].get(id(e))
    if at is None:
        return None
    em, k = at
    pre = ''.join(em.out[:k])
    owner = _now['owner_of'].get(id(em))
    if em is _now['top'] or owner is None:
        return pre
    # (inside a rewrite: its text, where this part of it stands, after what is before the rewrite)
    whole, part = _now['shown'].get(owner, ''), ''.join(em.out)
    if re.fullmatch(r"[A-Za-z_]\w*", part):
        m = re.search(r"(?<![\w.])%s\b" % re.escape(part), whole)
        k = m.start() if m else -1
    else:
        k = whole.find(part)
    up = _prefix_of_id(owner)
    return pre if k < 0 or up is None else up + whole[:k] + pre


def _prefix_of_id(i):
    for e in _now['nodes_by_id'].get(i, ()):
        return _prefix(e)
    return None


# (a 16-bit local of the frame, as c_text read its declaration: type, width, stars, name, its offset, its count)
_LOCAL16 = re.compile(r"(?m)^  (\w+?)(\d)?(\s*\*+\s*|\s+)(\w*Stack_([0-9a-f]+))( \[(\d+)\])?;\s*$")
_ELEM = {"1": 1, "2": 2, "4": 4}


def _elem16(typ):
    """Bytes in one of a 16-bit local's type, as written: `int2`, `uint1`, `char`, `code *`."""
    typ = (typ or "").strip()
    if typ.endswith("*"):
        return 2
    return _ELEM.get(typ[-1:]) or (1 if typ in ("char", "uchar", "byte") else 2)


def _resized(line, count):
    """An array's declaration with another element count: `  char a [4];` -> `  char a [6];`"""
    left, _sep, right = line.rpartition("[")
    return "%s[%d]%s" % (left, count, right[right.index("]") + 1:]) if _sep and "]" in right else line


class _Locals:
    """The function's locals as its text is written now (one _seen layout): their declarations'
    text (`region`, as c_text read the lines after the function's `{`), and every place one is
    written -- its name, or a moved pass's text naming it (_entry_sp) -- as c_text's parser read
    each: what the text there starts with, its address taken, passed on; a local renamed (its
    uses written otherwise: `_now['ren']`)."""

    def __init__(self, nodes, locs):
        seen = _seen()
        self.parent = {id(e): p for e, p in nodes}
        _now['nodes_by_id'] = {id(e): (e,) for e, _p in nodes}
        self.text = ''.join(_now['top'].out)
        self.region = _now['locals_text'] if _now['locals_text'] is not None else \
            ''.join('\n  %s;' % _decl_now(v) for v in locs) + ('\n  ' if locs else '')
        names = {(v.get('sym') or {}).get('name') for v in locs}
        self.refs = [(e, _name(e), 'plain') for e, _p in nodes if id(e) in seen and _bare_name(e) and _name(e) in names]
        self.refs += [(e, n, form) for e, _p in nodes for n, form in [_now['virtual'].get(id(e), (None, None))]
                      if n is not None and id(e) in seen]

    @staticmethod
    def written(n):
        """the name the text of local `n` starts with now"""
        m = re.match(r"[A-Za-z_]\w*", _local_text(n))
        return m.group(0) if m else None

    def occurs(self, name):
        return [(e, form) for e, n, form in self.refs if self.written(n) == name]

    def addressed(self, name):
        parent = self.parent
        for e, form in self.occurs(name):
            if _local_text(name) != name:
                continue
            if form == 'address':
                return True
            if _unary_parent(e, parent, '&'):
                return True
            up = parent.get(id(e))
            if up is not None and up.get('node') == 'Index' and up['arr'] is e and id(up) not in _now['whole'] and \
                    _unary_parent(up, parent, '&'):
                return True
            if up is not None and id(up) in _now['whole']:
                # (written inside a moved pass's rewrite: `(*(uint2 *)((char *)&x + 2))`, a piece of x)
                pre = _prefix(e)
                if pre and re.search(r"(?<!&)&\s*\Z", pre):
                    return True
        return False

    def passed(self, name):
        for e, form in self.occurs(name):
            pre = _prefix(e) if form == 'plain' else None
            if pre and re.search(r"[(,]\s*(?:\(\w+\s*\*?\))?\s*&?\s*\Z", pre):
                return True
        return False

    def rename(self, name, made):
        _now['ren'][name] = made
        for e, n, form in self.refs:
            if n == name and form == 'plain' and id(e) not in _now['whole'] and e['node'] == 'Name':
                _rewrite(e, lambda _x, n=n: _local_text(n))


def _statement_keys(body):
    """{id(node): the statement C's grammar has it in} -- the statement whose line a pass puts
    another before, as c_text found it, climbing from a node to a child of a block or a label: an
    `else if`'s is its first if's; one in a case's statements (no block of their own) is that case,
    written from its last label."""
    keys = {}

    def under(x, k):
        for n, _p in _walk(x):
            keys.setdefault(id(n), k)

    def block(stmts, case=None):
        for s in _stmts(stmts):
            if isinstance(s, dict):
                stmt(s, case if case is not None else s, case)

    def stmt(s, k, case=None):
        keys[id(s)] = k
        n = s.get('node')
        if n == 'If':
            under(s.get('cond'), k)
            then = s.get('then')
            if isinstance(then, dict):
                under(then, k)
            else:
                block(then)
            orelse = s.get('orelse')
            if isinstance(orelse, list) and len(orelse) == 1 and orelse[0].get('node') == 'If':
                stmt(orelse[0], k)
            else:
                block(orelse)
        elif n in ('While', 'DoWhile'):
            under(s.get('cond'), k)
            block(s.get('body'))
        elif n == 'For':
            for part in ('init', 'cond', 'update'):
                under(s.get(part), k)
            block(s.get('body'))
        elif n == 'Switch':
            under(s.get('expr'), k)
            for c in _stmts(s.get('cases')):
                under(c.get('labels'), c)
                block(c.get('body'), c)
        elif n == 'Labeled':
            body = [x for x in _stmts(s.get('body')) if isinstance(x, dict)]
            for i, x in enumerate(body):
                stmt(x, x if not i or case is None else case, case)
        elif n == 'Block':
            block(s.get('stmts'), case)
        else:
            under(s, k)
    block(body)
    return keys


def _calls_with(text, arr):
    """Does `text` hold a call whose arguments name `arr` (or codebreaker_frame.arr)?"""
    name = re.compile(r"(?<![\w.])%s\b|codebreaker_frame\.%s\b" % (re.escape(arr), re.escape(arr)))
    for m in re.finditer(r"[A-Za-z_]\w*\s*\(", text):
        depth, k = 0, m.end() - 1
        for j in range(k, len(text)):
            depth += {'(': 1, ')': -1}.get(text[j], 0)
            if depth == 0:
                if name.search(text, k + 1, j):
                    return True
                break
    return False


def _stack_segment_stores(nodes, _arrays):
    """The original stores SS into its frame (`mov [bp-0Ch], ss`) as the segment half of a far
    pointer it builds there to a local -- WNDCLASS.lpszClassName, CBT_CREATEWND.lpcs -- and the
    decompiler drops that store (a copy of the incoming SS, taken for a register saved). Put back
    where the buffer holding that place has it: SS written there just before the first statement
    with a call the buffer is passed to. Where no buffer holds it, the words from the one whose
    address is passed on are one packed record with SS in its place, written with the offset
    (USER's CreateWindow builds CBT_CREATEWND so: c_text._stack_segment_stores, _far_record).
    The places are the module's facts ("ss_stores": c_text._ss_store_places)."""
    places = _now['facts'].get('ss_stores')
    if not places:
        return
    locs = [v for v in _now['locals'] if (v.get('sym') or {}).get('name') not in _now['drop']]
    lv = _Locals(nodes, locs)
    seen = _now['seen']
    region = lv.region
    keys, at_of = _statement_keys(_now['doc'].get('body')), dict(_now['stmt_at'])
    arrays = {}
    for line in region.split('\n'):
        m = re.fullmatch(r"  +(.+?)\b(\w*Stack_([0-9a-f]+)) \[(\d+)\];[ \t]*", line)
        if m and m.group(1).count('(') == 0 and not m.group(1).strip().endswith('struct {'):
            arrays.setdefault(m.group(2), (int(m.group(3), 16), int(m.group(4)), m.group(1).strip()))
    ss = 'unaff_SS' if re.search(r"\bunaff_SS\b", lv.text) else 'codebreaker_ss()'
    made = set()
    for place in sorted(set(places)):
        for arr, (ao, count, typ) in arrays.items():
            elem = 2 if '*' in typ else _ELEM.get(typ.strip()[-1:], 2)
            if not ao > place >= ao - count * elem:
                continue
            at = ao - place
            store = "(*(int2 *)((uint1 *)%s + 0x%x)) = %s;" % (_local_text(arr), at, ss)
            if store in lv.text or store in made:
                break
            best = None
            for e, _p in nodes:
                k = keys.get(id(e))
                if k is None or id(k) not in at_of or id(e) not in seen:
                    continue
                shown = _now['shown'].get(id(e))
                if shown is not None:
                    hit = _calls_with(shown, arr)
                elif e['node'] == 'Call':
                    hit = any(_calls_with('f(%s)' % expr(a), arr) for a in e.get('args') or ())
                else:
                    continue
                if hit and (best is None or at_of[id(k)] < at_of[id(best)]):
                    best = k
            if best is not None:
                _now['before'].setdefault(id(best), []).append(store)
                made.add(store)
            break
        else:
            region = _far_record(lv, region, place, ss, nodes)
    if region != lv.region:
        _now['locals_text'] = region


def _far_record(lv, region, place, ss, nodes):
    """The far pointer is not in a buffer but in a record of single words the decompiler split
    into separate locals (USER's CreateWindow builds CBT_CREATEWND {lpcs = ss:&args,
    hwndInsertAfter} at [bp-26h] and passes its address to the CBT hook): kept as one packed
    record, from the word whose address is passed on, through SS, through the words after it with
    no gap; SS written with the offset (c_text._far_record). -> the locals' text"""
    def scalar(off):
        for mm in re.finditer(r"(?m)^  ([\w ]*?\w(?:\s*\*+)?)\s*\b(\w*Stack_%x);[ \t]*$" % off, region):
            typ = mm.group(1)
            size = 2 if '*' in typ else _ELEM.get(typ.strip()[-1:], 0)
            return (mm.group(0), mm.group(2), size) if size == 2 else None
        return None
    first = scalar(place + 2)
    if not first or not lv.addressed(first[1]):
        return region
    words = [first, None]
    off = place - 2
    while scalar(off):
        words.append(scalar(off))
        off -= 2
    rec = "codebreaker_far_%x" % (place + 2)
    fields = ["    int2 codebreaker_ss;" if w is None else "  " + w[0].strip() for w in words]
    for w in words:
        if w:
            region = region.replace(w[0] + "\n", "", 1)
    region = "\n" + "  _Packed struct {\n%s\n  } %s;\n" % ("\n".join(fields), rec) + \
        (region[1:] if region.startswith("\n") else region)
    for w in words:
        if w:
            lv.rename(w[1], lambda r=rec, n=w[1]: "%s.%s" % (r, n))
    for e, _p in nodes:
        if e['node'] == 'ExprStmt' and id(e) in _now['lines'] and e['expr'].get('node') == 'Assign' and \
                not e['expr'].get('op') and _bare_name(e['expr']['target'], first[1]) or \
                e['node'] == 'ExprStmt' and id(e) in _now['lines'] and e['expr'].get('node') == 'Assign' and \
                not e['expr'].get('op') and id(e['expr']['target']) in _now['whole'] and \
                e['expr']['target'].get('node') == 'Name' and _name(e['expr']['target']) == first[1]:
            _now['after'].setdefault(id(e), []).append("%s.codebreaker_ss = %s;" % (rec, ss))
            break
    return region


def _buffer_fields(nodes, _arrays):
    """A local the function never sets, lying past the end of an array local whose address it
    passes on, is a field of that buffer: the callee fills the buffer, the function reads the field
    (the analysis sized the array to reach the next thing it saw used, and made that a variable of
    its own). It is read where it is, `(*(T *)((uint1 *)a + 0x4))`, and the buffer made to reach
    it. Before that, locals of one width right below one whose address is passed on are one array
    of them (a RECT copied into four words). A buffer whose address is passed on is laid out as
    the original's frame had it, up to the next buffer passed on or the saved BP; two of them or
    more are kept in the frame's order and distances, one packed structure, codebreaker_frame
    (USER's sub_3_1C4E has GetObject write a 50-byte LOGFONT into an 18-byte buffer, running on
    into the next one). c_text._buffer_fields, which did it on the text: the declarations are
    rewritten as that text, the locals' uses on the tree."""
    locs = [v for v in _now['locals'] if (v.get('sym') or {}).get('name') not in _now['drop']]
    if not locs:
        return
    lv = _Locals(nodes, locs)
    before = lv.region
    region = before                                      # (the locals' text, as c_text read it)
    occurs, addressed, passed, rename = lv.occurs, lv.addressed, lv.passed, lv.rename

    def replace(old, new):
        nonlocal region
        region = region.replace(old, new, 1)
    decl = {}
    for mm in _LOCAL16.finditer(region):
        typ, width, stars, name, off, _a, count = mm.groups()
        stars = (stars or "").strip()
        elem = 2 if stars else _ELEM.get(width or "", 2) if width else _elem16(typ or "")
        size = int(count) * elem if count else elem
        decl[name] = (int(off, 16), size, bool(count), mm.group(0), (typ or "") + (width or "") + (" " + stars if stars else ""))
    by_off = {v[0]: n for n, v in decl.items() if not v[2]}
    for name, (off, size, is_array, line, typ) in sorted(decl.items(), key=lambda kv: -kv[1][0]):
        if is_array or name not in decl or not addressed(name):
            continue
        run, at = [name], off - size
        while at in by_off and decl.get(by_off[at], (0, 0))[1] == size and by_off[at] not in run \
                and decl[by_off[at]][4] == typ and not addressed(by_off[at]):
            run.append(by_off[at])
            at -= size
        if len(run) < 2:
            continue
        arr = "a" + name
        replace(line, "  %s %s [%d];" % (typ or "int2", arr, len(run)))
        for k, n in enumerate(run):
            if k:
                replace(decl[n][3], "")
            rename(n, lambda a=arr, k=k: "%s[%d]" % (_local_text(a), k))
            decl.pop(n, None)
        decl[arr] = (off, size * len(run), True, "  %s %s [%d];" % (typ or "int2", arr, len(run)), typ)
    arrays = {n: v for n, v in decl.items() if v[2] and passed(n)}
    if not arrays:
        if region != before:
            _now['locals_text'] = region
        return
    for owner, (ao, asz, _i, aline, _atyp) in list(arrays.items()):
        others = [v[0] for k, v in arrays.items() if k != owner and v[0] < ao]
        need = ao - max(others + [2])
        if asz < need <= 0x400:
            newline = _resized(aline, (need + _elem16(_atyp) - 1) // _elem16(_atyp))
            replace(aline, newline)
            arrays[owner] = (ao, need, True, newline, _atyp)
    for name, (off, size, is_array, line, typ) in sorted(decl.items()):
        if name in arrays or not occurs(name):
            continue
        owner = next((a for a, (ao, asz, _i, _l, _t) in arrays.items() if ao > off >= ao - asz), None)
        if owner is None:
            continue
        ao, asz, _i, aline, _atyp = arrays[owner]
        at = ao - off                                    # bytes into the buffer
        replace(line, "")
        place = lambda t=typ or "int2", o=owner, at=at: "(%s)((uint1 *)%s + 0x%x)" % (_pops(t + ' *'), _local_text(o), at)
        rename(name, (lambda p=place: "(%s)" % p()) if is_array else (lambda p=place: "(*%s)" % p()))
        need = at + (size if is_array else _elem16(typ))
        if need > asz:
            newline = _resized(aline, (need + _elem16(_atyp) - 1) // _elem16(_atyp))
            replace(aline, newline)
            arrays[owner] = (ao, need, True, newline, _atyp)
    if len(arrays) >= 2:                                 # (c_text._frame_block)
        order = sorted(arrays.items(), key=lambda kv: -kv[1][0])          # (lowest address first)
        fields, at = [], None
        for k, (name, (off, size, _i, line, _typ)) in enumerate(order):
            if at is not None and at > off:
                fields.append("    uint1 codebreaker_gap%d [%d];" % (k, at - off))
            elif at is not None and at < off:
                fields = None                            # (overlapping: left alone)
                break
            fields.append("  " + line.strip())
            at = off - size
        if fields is not None:
            for name, (off, size, _i, line, _typ) in order:
                replace(line, "")
            region = "\n" + "  _Packed struct {\n%s\n  } codebreaker_frame;\n" % "\n".join(fields) + \
                (region[1:] if region.startswith("\n") else region)
            for name in arrays:                          # (the fields' own declarations keep their names)
                rename(name, lambda n=name: "codebreaker_frame.%s" % n)
    region += "\n"                                       # (the statements' line starts after it)
    while "\n\n\n" in region:
        region = region.replace("\n\n\n", "\n\n")
    _now['locals_text'] = region[:-1]


_FAR_PLACE = re.compile(r"\(\(\w+ __far \*\)(?:MK_FP\(.*\)|\(.*\))\)", re.S)


def _far_place(x):
    """Is `x` written as one far cast in brackets -- `((U __far *)MK_FP(s, o))`, `((U __far *)(p +
    2))` (_far_addresses' texts), `((U __far *)p)`?"""
    shown = _now['whole'].get(id(x))
    if shown is not None:
        return bool(_FAR_PLACE.fullmatch(shown(x).strip()))
    if x.get('node') == 'Paren':
        inner = x['x']
        return inner.get('node') == 'Cast' and id(inner) not in _now['whole'] and \
            re.fullmatch(r"\w+ __far \*", _cast_type(inner)) is not None
    return False


def _far_reads(nodes, _arrays):
    """`*(T *)((U __far *)MK_FP(s, o))` -- a far place read (or written) through a near cast: the
    segment dropped, DS:o in its stead (a RECT read out of the caller's segment, read out of the
    module's own). What is reached through a far pointer is reached far: `*(T __far *)(...)`
    (also behind a cast, `(int4)*(T *)(...)`, which c_text's parser read as a product; KRNL386's
    INITTASK stores &param_1 at ss:[0Ch] through `(xunknown1 **)`, written `xunknown1 * __far*`
    as c_text wrote it; c_text._far_reads)."""
    parent = {id(e): p for e, p in nodes}
    for e, p in nodes:
        if e['node'] != 'Cast' or _now['marks'].get(id(e)) or not _unary_parent(e, parent, '*') or \
                _arrow_of(p, parent) or not _star_as_read(p, parent):
            continue
        if id(e) in _now['ss_casts']:
            # (a frame buffer through its address, written with SS: _frame_through_ss)
            if 'far_addresses' in _now['moved']:
                _now['far_read'].add(id(e))
            continue
        if id(e) in _now['whole']:
            continue
        ty = _cast_type(e)
        near = re.fullmatch(r"(\w+(?: \w+)?(?: ?\*)*?) ?\*", ty)
        if not near or '__far' in ty or not _far_place(e['x']):
            continue
        if near.group(1) + ' *' == ty:
            _now['marks'][id(e)] = '_far'
        else:
            k = len(near.group(1))
            _rewrite(e, lambda c, t=ty[:k] + ' __far' + ty[k:]: '(%s)%s' % (_pops(t), _written(c['x'], c)))


_WIDE_DECL = re.compile(r"(?m)^[ \t]*(int4|uint4|xunknown4|undefined4|float4) (\w+)(?: \[\d*\])?(?: = [^;\n]*)?;")


def _wide_names():
    """The function's four-byte variables (and arrays of them) as their declarations are written
    now: the locals, and the arguments where they are declared apart (the older style)."""
    if _now['locals_text'] is not None:
        text = _now['locals_text']
    else:
        text = ''.join('\n  %s;' % _decl_now(v) for v in _now['locals']
                       if (v.get('sym') or {}).get('name') not in _now['drop'])
    text += ''.join('\n%s;' % d for _n, d in _now['old_style'] or ())
    return {m.group(2) for m in _WIDE_DECL.finditer(text)}


def _near_number(inner):
    """Is `inner`, in brackets, no far value -- MK_FP, a far cast, CONCAT, an address, a four-byte
    variable, a four-byte type, a null pointer (_near_to_far)?"""
    if "__far" in inner or "MK_FP" in inner or "CONCAT" in inner or "&" in inner \
            or re.search(r"\b(?:int4|uint4|xunknown4|undefined4)\b", inner) or re.fullmatch(r"\(\s*0\s*\)", inner):
        return False
    return not any(re.search(r"\b%s\b" % re.escape(w), inner) for w in _wide_names())


def _parenthesized(t):
    """Is `t` one expression in brackets, `(a + b)` (not `(a) + (b)`, nor a cast `(T)x`)?"""
    t = t.strip()
    if not t.startswith('(') or not t.endswith(')'):
        return False
    depth = 0
    for k, ch in enumerate(t):
        depth += {'(': 1, ')': -1}.get(ch, 0)
        if depth == 0:
            return k == len(t) - 1
    return False


def _near_to_far(nodes, _arrays):
    """`(T __far *)(n)` of a number -- an offset in the data segment, as the original reads it
    through DS: C makes a far pointer of a number with segment 0. Through a near pointer it gets
    DS: `(T __far *)(T *)(n)`. Not where the value is far already (MK_FP, a far cast, CONCAT, an
    address), nor a four-byte variable's, nor a null pointer (c_text._near_to_far)."""
    for e, _p in nodes:
        if e['node'] != 'Cast' or id(e) in _now['whole']:
            continue
        far = re.fullmatch(r"(\w+(?: \w+)?) __far \*", _cast_type(e))
        if not far:
            continue
        inner = _written(e['x'], e)
        if not _parenthesized(inner) or not _near_number(inner):
            continue
        _rewrite(e, lambda c, t=far.group(1): '(%s)(%s)%s' % (_pops(_cast_type(c)), _pops(t + ' *'), _written(c['x'], c)))


def _star_as_read(u, parent):
    """Is the `*` of `u` an indirection as c_text's parser read the text (ctree's unary, and
    _behind_cast)? Behind casts to types with no `*` -- `(int4)*p` -- the grammar reads a product
    of the casts and p, which it takes for one; after a product or a quotient (`a * (int4)*p`,
    left to right: `(a * (int4)) * p`) only behind a single cast, and never after a prefix
    operator (`-(int4)*p`: `(-(int4)) * p`)."""
    chain, t = 0, u
    while True:
        up = parent.get(id(t))
        if up is not None and up.get('node') == 'Cast' and up.get('x') is t and id(up) not in _now['whole'] and \
                '*' not in _cast_type(up):
            chain, t = chain + 1, up
        else:
            break
    if not chain:
        return True
    p = parent.get(id(t))
    if p is not None and p.get('node') == 'Unary' and not p.get('postfix') and id(p) not in _now['whole']:
        return False
    if p is not None and p.get('node') == 'Binary' and p.get('op') in ('*', '/', '%') and p.get('right') is t and \
            not _bracket(_token(p), t, 1):
        return chain == 1
    return True


def _arrow_of(u, parent):
    """Is `u` the `*` of a `p->f` (L2's Select of *p, written `->`)?"""
    s = parent.get(id(u))
    return s is not None and s.get('node') == 'Select' and s.get('obj') is u and _arrow(s)


def _unary_parent(e, parent, op):
    p = parent.get(id(e))
    return p is not None and p.get('node') == 'Unary' and p.get('op') == op and not p.get('postfix') and \
        id(p) not in _now['whole']


def _passed_on(arg, reg):
    """Is `arg` the register `reg` itself as the function had it -- not set for the interrupt?"""
    r = reg.upper()
    return arg.strip() in ('in_' + r, 'in_E' + r, 'unaff_' + r, 'unaff_E' + r)


def _code_pointers(nodes, _arrays):
    """`p = sub_401000;` -- a function's address stored in a pointer the core typed loosely: `p =
    (code *)sub_401000;`, where the assignment ends its statement (compilable32's _code_pointers;
    the module's functions are the facts' "functions")."""
    functions = _now['facts'].get('functions') or ()
    if not functions:
        return
    parent = {id(e): p for e, p in nodes}
    for e, _p in nodes:
        if e['node'] == 'Assign' and not e.get('op') and e['value'].get('node') == 'Name' and \
                id(e['value']) not in _now['whole'] and _name(e['value']) in functions and _ends_statement(e, parent):
            _rewrite(e['value'], lambda x: '(code *)' + _name(x))


_POINTER_LOCAL = re.compile(r"[A-Za-z_][\w ]*?\*+\s*(\w+)(?: \[\d+\])?")
_COMPARISONS = ("<=", ">=", "<", ">", "==", "!=")


def _compare_as_numbers(nodes, _arrays):
    """The core compares a pointer with a plain number freely (`if (pcVar2 < (uint4)a1)`); C does
    not allow it. Where exactly one side is a pointer variable -- a local or a parameter declared
    one -- or a cast to a pointer type, its value is compared as a number, `(uint4)pcVar2`; the
    other side a variable, or one cast to int4 / uint4 (compilable32's _compare_as_numbers)."""
    doc = _now['doc']
    ptrs = set()
    for v in _now['locals']:
        n = (v.get('sym') or {}).get('name')
        if n not in _now['drop']:
            m = _POINTER_LOCAL.fullmatch(_decl_now(v))
            if m:
                ptrs.add(m.group(1))
    for p in doc.get('params') or []:
        if re.search(r"\*+\s*%s$" % re.escape(_param_name(p)), type_text(p.get('type'), _param_name(p))):
            ptrs.add(_param_name(p))
    cast_ptr = object()

    def operand(n):
        """the name a side is a pointer by (a variable; '' when cast to a number; cast_ptr for a cast
        to a pointer type), None when it is no such side"""
        if id(n) in _now['whole']:
            return None
        if n.get('node') == 'Cast' and _cast_type(n) in ('int4', 'uint4'):
            v = n['x']
            return '' if v.get('node') != 'Cast' and operand(v) is not None else None
        if n.get('node') == 'Cast' and _cast_type(n).replace(' ', '').endswith('*'):
            return cast_ptr
        if n.get('node') == 'Name' and (n.get('sym') or {}).get('kind') != 'field':
            return _name(n)
        return None
    seen = _seen()
    for e, _p in nodes:
        if e['node'] != 'Binary' or e.get('op') not in _COMPARISONS or id(e) in _now['whole'] or id(e) not in seen:
            continue
        sides = [e['left'], e['right']]
        names = [operand(s) for s in sides]
        if None in names or (cast_ptr in names and '' not in names):
            continue
        ptr = [x is cast_ptr or x in ptrs for x in names]
        if ptr[0] != ptr[1]:
            for s, is_ptr in zip(sides, ptr):
                if is_ptr:
                    _rewrite(s, lambda x: '(uint4)' + _raw_now(x))


_FRAME_LOCAL = re.compile(r"(?m)^  ([A-Za-z_][\w ]*?(?:\s*\*+\s*|\s))([a-z]+Stack_([0-9a-f]+))(?: \[(\d+)\])?;[ \t]*$")


def _keep_frame(nodes, _arrays):
    """Keep a function's stack locals where the original had them, relative to one another
    (compilable32's _keep_frame). The original often builds a small structure in its frame and
    passes its address (`mov [ebp-18h], hwnd / mov [ebp-14h], id / lea edx, [ebp-18h] / push
    edx`); the core recovers the fields as separate locals, which a C compiler places as it
    likes, and the callee reading through the pointer finds something else. When a frame
    local's address can escape -- one is an array, its address is taken (`&x`, in the text the
    passes wrote too), or the function reads the stack pointer it was entered with -- the locals
    are declared as the fields of one frame structure `_f` at their original offsets (filler
    between, the rest of the frame up to the return address after the last), every use is
    `_f.x`, and the entry SP is the end of `_f`. A type of unknown size (the facts'
    "struct_sizes": the structures recovered so far in the module) or locals that overlap leave
    the function as it is. As c_text did it on the text: the first declaration's line becomes
    the structure's, the others' lines are left empty, and the names are written after the
    lines are laid out."""
    locs = [v for v in _now['locals'] if (v.get('sym') or {}).get('name') not in _now['drop']]
    if not locs:
        return
    lv = _Locals(nodes, locs)
    region = lv.region
    found = list(_FRAME_LOCAL.finditer(region))
    if not found:
        return
    names = {m.group(2) for m in found}
    seen = _now['seen']
    esp = any(e['node'] == 'Name' and _name(e) == _ENTRY_SP and id(e) in seen for e, _p in nodes) or \
        any(_ENTRY_SP in t for t in _now['shown'].values())
    parent = {id(e): p for e, p in nodes}

    def addressed(e):
        if _unary_parent(e, parent, '&'):
            return True
        up = parent.get(id(e))
        if up is not None and id(up) in _now['whole']:
            pre = _prefix(e)                             # (inside a pass's rewrite: `(char *)&x + 2`)
            return bool(pre and re.search(r"&\s*\Z", pre))
        return False
    if not (esp or any(m.group(4) for m in found) or
            any(e['node'] == 'Name' and _name(e) in names and id(e) in seen and addressed(e) for e, _p in nodes)):
        return
    sizes = _now['facts'].get('struct_sizes') or {}
    items = []
    for m in found:
        ty = " ".join(m.group(1).split())
        size = 4 if ty.endswith("*") else SIZEOF.get(ty) or sizes.get(ty)
        if size is None:
            return                                       # (a type whose size is not known)
        count = int(m.group(4)) if m.group(4) else 1
        items.append((int(m.group(3), 16), size * count, ty, m.group(2), m.group(4)))
    items.sort(reverse=True)                             # (largest offset = lowest address, first)
    fields, pos = [], None
    for off, size, ty, name, arr in items:
        addr = -off
        if pos is not None:
            gap = addr - pos
            if gap < 0:
                return                                   # (overlapping locals: left as they are)
            if gap:
                fields.append("uint1 _gap_%x[%d];" % (off, gap))
        fields.append("%s %s%s;" % (ty, name, "[%s]" % arr if arr else ""))
        pos = addr + size
    if pos is not None and pos < 0:
        # (the frame runs on to the return address: a callee handed the last local's address may
        # fill a whole structure there -- a RECT the core saw two fields of)
        fields.append("uint1 _rest[%d];" % -pos)
    decl = "  struct { %s } _f;   /* locals at their offsets in the original's frame */" % " ".join(fields)
    first = [True]

    def strip(_m):
        if first[0]:
            first[0] = False
            return decl
        return ""
    _now['locals_text'] = _FRAME_LOCAL.sub(strip, region)
    _now['frame'] = names | ({_ENTRY_SP} if esp else set())


# ---------------------------------------------------------------------------------------------- the 32-bit module's globals
# What compilable32 decides over the whole module -- which globals are called through, read through
# as pointers, used at another width, compared as numbers -- is decided from every function's tree
# before any is written (module_names32), and each function written with its part of it (the facts'
# "called_through", "aliases", "pointer_globals", "takes").
_THROUGH = {"i": "int4", "u": "uint4", "x": "xunknown4"}
_C_PRIMITIVE = ("char", "short", "int", "long", "unsigned", "signed", "double", "float", "void", "bool")


def module_names32(doc):
    """What a 32-bit function's tree names, as its text will: {'names': every name it writes
    (its variables, globals and the functions it calls), 'calls': the functions it calls by name,
    'called_through': the globals it calls through, `(*xRam0040c000)(...)`}."""
    names, calls, through, arity = set(), set(), set(), []
    for e, _p in _walk(doc.get('body')):
        if e['node'] == 'Name':
            names.add(_name(e))
        elif e['node'] == 'Call':
            t = e.get('target')
            n = None
            if t is None:
                n = (e.get('sym') or {}).get('name')
                if n:
                    names.add(n)
                    calls.add(n)
            elif t.get('node') == 'Name':
                n = _name(t)
                calls.add(n)
            elif t.get('node') == 'Select':
                n = _name(t)                             # (`p->name(...)` counts as a call of name)
            elif t.get('node') == 'Unary' and t.get('op') == '*' and not t.get('postfix') and \
                    t['x'].get('node') == 'Name' and _RAM.fullmatch(_name(t['x'])):
                through.add(_name(t['x']))
            if n:
                arity.append((n, len(e.get('args') or ())))
    return {'names': names, 'calls': calls, 'called_through': through, 'arity': arity}


def type_names(doc):
    """The names of the decompiler's own types a function's tree uses (`unkbyte10`, `uint3`,
    `s_f_p`): every nominal type in it -- its result, parameters, locals and casts -- for the
    module's header to declare (compilable32's odd-size typedefs). Only the types written: a
    variable's own type in the tree is not."""
    out = set()
    todo = [doc.get('ret')] + [p.get('type') for p in doc.get('params') or []]
    todo += [e.get('type') for e, _p in _walk(doc.get('body')) if e['node'] in ('Local', 'Cast')]
    while todo:
        t = todo.pop()
        if not isinstance(t, dict):
            continue
        if t.get('kind') == 'class' and isinstance(t.get('name'), str):
            out.add(t['name'])
        todo += [t.get('to'), t.get('elem')]
        fn = t.get('fn') if t.get('kind') == 'fnptr' else None
        if isinstance(fn, dict):
            todo += [p.get('type') for p in fn.get('params') or []] + [(fn.get('result') or {}).get('type')]
    return out


def _read_through(u, parent):
    """Is the `*` of `u` a pointer expression as c_text's parser read the text -- not after a
    cast to a type it did not know with no `*` (`(int4)*p`, which it read as a product)?"""
    up = parent.get(id(u))
    if up is not None and up.get('node') == 'Cast' and up.get('x') is u and id(up) not in _now['whole']:
        ty = _cast_type(up)
        return '*' in ty or ty.split()[-1] in _C_PRIMITIVE
    return True


def _globals_through(nodes, _arrays):
    """A global the decompiler named as a number but reads through (`*iRam..`, `iRam..[n]`) is read
    through as a pointer THERE -- `*(int4 *)iRam..`, `((int4 *)iRam..)[n]` -- and stays a number
    everywhere else, where it is added to and compared as one (compilable32's `through`). Not one
    the module calls through: that one holds a function pointer (the facts' "called_through")."""
    called = _now['facts'].get('called_through')
    if called is None:
        return
    parent = {id(e): p for e, p in nodes}
    for e, p in nodes:
        if e['node'] != 'Name' or id(e) in _now['whole'] or p is None or id(p) in _now['whole']:
            continue
        nm = _name(e)
        if not _RAM.fullmatch(nm) or nm in called or nm[0] not in _THROUGH:
            continue
        kind = _THROUGH[nm[0]]
        if p.get('node') == 'Unary' and p.get('op') == '*' and not p.get('postfix') and _read_through(p, parent):
            _rewrite(e, lambda x, k=kind: '(%s *)%s' % (k, _global_now(_name(x))))
        elif p.get('node') == 'Index' and p.get('arr') is e:
            _rewrite(e, lambda x, k=kind: '((%s *)%s)' % (k, _global_now(_name(x))))


def _global_now(name):
    """A global's name as the function writes it: at its own width, where it uses the place at one
    width other than the module's (the facts' "aliases": `iRam0040c000_2`)."""
    return _now['alias'].get(name, name)


def _same_cast(nodes, _arrays):
    """A pointer-typed global compared with a number the decompiler cast to an integer
    (`(uint4)pcVar3 != pcRam004094b0`) is compared as the same integer, whatever it points to
    (compilable32's `same_cast`; the facts' "pointer_globals")."""
    pointers = _now['facts'].get('pointer_globals')
    if not pointers:
        return
    for e, _p in nodes:
        if e['node'] != 'Binary' or e.get('op') not in _COMPARISONS or id(e) in _now['whole']:
            continue
        for g, other in ((e['right'], e['left']), (e['left'], e['right'])):
            if g.get('node') != 'Name' or id(g) in _now['whole'] or _name(g) not in pointers:
                continue
            ty = value = None
            if id(other) in _now['whole']:
                m = re.fullmatch(r"\(\s*(\w+)\s*\)(.*)", expr(other), re.S)
                if m:
                    ty, value = m.group(1), m.group(2)
            elif other.get('node') == 'Cast':
                ty, value = _cast_type(other), expr(other['x'])
            if ty and re.fullmatch(r"u?int\d", ty) and re.fullmatch(r"[\w.>\-]+", value):
                _rewrite(g, lambda x, t=ty: '(%s)%s' % (t, _name(x)))
                break


def _fit_calls(nodes, _arrays):
    """A call that passes another number of arguments than the function is defined with is made
    through a pointer with the argument list left open, `((int4 (*)())f)(...)`: the decompiler
    recovers each call on the evidence at that call, and a call the compiler refused lost the
    whole calling function (compilable32's _fit_calls; the facts' "takes": {function: the
    arguments its definition takes}, of those not __cdecl)."""
    takes = _now['facts'].get('takes')
    if not takes:
        return
    for e, _p in nodes:
        if e['node'] != 'Call' or id(e) in _now['whole']:
            continue
        t = e.get('target')
        if t is not None and t.get('node') == 'Name' and id(t) not in _now['whole'] and _name(t) in takes and \
                len(e.get('args') or ()) != takes[_name(t)]:
            _rewrite(t, lambda x: '((int4 (*)())%s)' % _name(x))


def parameter_names(doc):
    """The names of a function's parameters as its heading writes them, as struct_text read them
    from the text: none where a declaration has brackets in it (a pointer to a function)."""
    decls = [type_text(p.get('type'), _param_name(p)) for p in doc.get('params') or []]
    if ((doc.get('sym') or {}).get('type') or {}).get('variadic'):
        decls.append('...')
    if any('(' in d or ')' in d for d in decls):
        return []
    return [d.strip().split()[-1].lstrip('*') for d in decls]


_ACCESS = r"\((\w+) ?(\*+)\)(?:\(%s \+ (0x[0-9a-fA-F]+|\d+)\)|%s)"


def _struct_access(u, params):
    """(parameter, base type, stars, offset) of `*(T *)(param + N)` or `*(T *)param` -- the
    deref `u` of one of `params` at a constant offset, as it is written -- else None."""
    if u.get('node') != 'Unary' or u.get('op') != '*' or u.get('postfix') or u['x'].get('node') != 'Cast':
        return None
    inner = u['x']['x']
    if inner.get('node') == 'Name':
        p = _name(inner)
    elif inner.get('node') == 'Binary' and inner.get('op') == '+' and inner['left'].get('node') == 'Name' and \
            inner['right'].get('node') == 'Lit':
        p = _name(inner['left'])
    else:
        return None
    if p not in params:
        return None
    m = re.fullmatch(_ACCESS % (re.escape(p), re.escape(p)), expr(u['x']))
    if not m:
        return None
    return p, m.group(1), m.group(2), int(m.group(3), 0) if m.group(3) else 0


def struct_accesses(doc, param):
    """[(base type, stars, offset)] of every access through `param` at a constant offset in a
    function's tree (struct_text.plan counts the types each offset is read as)."""
    return [a[1:] for a in (_struct_access(e, (param,)) for e, _p in _walk(doc.get('body'))) if a]


def _struct_fields(nodes, _arrays):
    """An access through a parameter known to point at a structure, at a constant offset and of
    the type the module reads that field as, is the field: `*(int4 *)(a1 + 0x48)` is
    `((struct_7 *)a1)->f_48` (struct_text.apply did it on the texts; the module's plan is the
    facts' "struct_fields": {param: (structure, {offset: (type, size)})}; the structures written
    are reported, "structs_used")."""
    planned = _now['facts'].get('struct_fields')
    if not planned:
        return
    from codebreaker.decompile import struct_text
    used = _now['facts'].setdefault('structs_used', set())
    for e, _p in nodes:
        if id(e) in _now['whole']:
            continue
        a = _struct_access(e, planned)
        if not a:
            continue
        param, base, stars, off = a
        sname, fields = planned[param]
        k, _size = struct_text.kind(base, stars)
        if k is None or fields.get(off, (None,))[0] != k:
            continue
        used.add(sname)
        _rewrite(e, lambda _e, s=sname, p=param, o=off: "((%s *)%s)->f_%x" % (s, p, o))


# ---------------------------------------------------------------------------------------------- PowerPC
_WIDE_ADDRESS = re.compile(r"0xffffffff([89a-f][0-9a-f]{7})")
_HOME = re.compile(r"_?([a-z]+)Stack(000000[0-9a-f]{2})")


def _wide_address(nodes, _arrays):
    """An address the decompiler shows widened to 64 bits (PowerPC's registers are 64 bits wide,
    its addresses 32), `0xffffffff82000000`, is the 32-bit address it is, `0x82000000`
    (compilable_powerpc's _WIDE_ADDRESS)."""
    for e, _p in nodes:
        if e['node'] == 'Lit' and id(e) not in _now['whole']:
            m = _WIDE_ADDRESS.fullmatch(_lit(e))
            if m:
                _rewrite(e, lambda _e, a=m.group(1): '0x' + a)


def _homed_arguments(nodes, _arrays):
    """A caller leaves room on the stack for each of the eight register arguments (eight bytes
    each, from 0x10 above the stack pointer at entry), and a function that needs an argument's
    address stores it there. The decompiler shows that slot as a stack variable of the caller's
    frame, `xStack00000010`; it is the argument, and is written as the argument -- its own
    declaration gone, and the store of the argument into its own slot, `a1 = a1;`, with it
    (compilable_powerpc's _homed_arguments)."""
    doc = _now['doc']
    names = [type_text(p.get('type'), _param_name(p)).split()[-1].lstrip('*') for p in doc.get('params') or []]

    def slot(name):
        m = _HOME.fullmatch(name)
        if not m:
            return None
        off = int(m.group(2), 16)
        k = (off - 0x10) // 8
        return names[k] if 0 <= k < len(names) and off % 8 in (0, 4) else None
    homes = {}
    for v in _now['locals']:
        n = (v.get('sym') or {}).get('name') or ''
        if slot(n):
            _now['drop'].add(n)
    for e, _p in nodes:
        if e['node'] == 'Name' and slot(_name(e)):
            homes[_name(e)] = slot(_name(e))
    _now['alias'].update(homes)                          # (written after the lines are laid out)
    for e, _p in nodes:                                  # the store into its own slot: `a1 = a1;`
        x = e.get('expr') if e['node'] == 'ExprStmt' else None
        if x is not None and x.get('node') == 'Assign' and not x.get('op') and \
                x['target'].get('node') == 'Name' and x['value'].get('node') == 'Name' and \
                id(x) not in _now['whole'] and \
                _now['alias'].get(_name(x['target']), _name(x['target'])) == \
                _now['alias'].get(_name(x['value']), _name(x['value'])):
            _now['gone'].add(id(e))


def takes32(signature):
    """How many arguments a 32-bit function's definition takes, as compilable32 counted them in
    its heading (`signature`: the heading as written) -- None for one that is __cdecl."""
    head = signature
    if "__cdecl" in head or "(" not in head:
        return None
    inside = head[head.index("(") + 1:head.rindex(")")].strip() if ")" in head else ""
    return 0 if inside in ("", "void") else inside.count(",") + 1


def _raw_now(x):
    """`x` as it is written now, its own rewrite aside (a rewrite of it written around it)"""
    _now['bare'].add(id(x))
    try:
        return expr(x)
    finally:
        _now['bare'].discard(id(x))


def _ends_statement(e, parent):
    """Is `e` the end of its statement -- the statement's expression, or the value of an
    assignment that is (`a = b = f;`)?"""
    up = parent.get(id(e))
    while up is not None and up.get('node') == 'Assign' and up['value'] is e:
        e, up = up, parent.get(id(up))
    return up is not None and up.get('node') == 'ExprStmt' and up['expr'] is e


# (each: c_text's pass, done on the tree -- in the order c_text runs them)
_PASSES = (("wide_address", _wide_address), ("frame_through_ss", _frame_through_ss), ("pieces", _pieces), ("joined_arguments", _joined_arguments),
           ("null_is_zero", _null_is_zero), ("global_pointer_steps", _global_pointer_steps),
           ("far_arguments", _far_arguments), ("one_name_for_globals", _one_name_for_globals),
           ("selector_structs", _selector_structs), ("struct_gaps", _struct_gaps),
           ("own_segment_calls", _own_segment_calls), ("interrupts", _interrupts),
           ("far_addresses", _far_addresses), ("far_casts", _far_casts), ("entry_sp", _entry_sp),
           ("stack_arguments", _stack_arguments), ("far_locals", _far_locals),
           ("far_concat_casts", _far_concat_casts), ("buffer_fields", _buffer_fields),
           ("far_reads", _far_reads),
           ("near_to_far", _near_to_far),
           ("stack_segment_stores", _stack_segment_stores), ("callee_pops", _callee_pops),
           ("caller_frame_slots", _caller_frame_slots), ("selector_imports", _selector_imports),
           ("segment_values", _segment_values), ("code_segment_reads", _code_segment_reads),
           ("names_used", _names_used), ("heading_facts", _heading_facts), ("code_pointers", _code_pointers),
           ("compare_as_numbers", _compare_as_numbers), ("keep_frame", _keep_frame),
           ("globals_through", _globals_through), ("same_cast", _same_cast), ("fit_calls", _fit_calls),
           ("struct_fields", _struct_fields), ("homed_arguments", _homed_arguments))


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
    ret = _ret(doc)
    far = 'far_result' in _now['moved'] and is_far(ret)
    if far and _stack(ret)[0] != ['ptr']:
        raise Decline('a far result of a pointer to a pointer')      # (c_text._far_result writes `T *__far *`)
    # (a far pointer: `T __far *`, c_text._far_result; `void` and a byte as _result_shown says)
    _put_type(em, ret, far=far, shown_base=_result_shown(ret))
    em.close_group()
    em.spaces(1)
    for m in doc.get('mods') or ():
        if 'register_arguments' in _now['moved'] and m == _register_arguments(doc):
            # (OpenWatcom has no word for it: left out, as c_text took it out of the heading, its
            # place given no room -- a #pragma in the header says where the arguments are)
            em.print(m + ' ', '')
            continue
        em.print(m)
        em.spaces(1)
    em.open_group()
    # (the model word, __export, __loadds: before the name, given no room -- c_text put them in
    # after PrintC's layout)
    words = _heading(doc)[2] if _now['facts'] else []
    em.print(doc['name'], ' '.join(words + [doc['name']]) if words else None)
    em.spaces(0, 10)
    em.open_paren('(')
    em.spaces(0, 10)
    params = doc.get('params') or []
    old = _now['old_style'] if _now['doc'] is doc else None
    if old is not None:
        # (the older style: the names alone here, their declarations below -- _old_style)
        if old:
            em.print(','.join(n for n, _d in old))
        params, ft = [], {}
    elif not params:
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


def _far_pointers(locs):
    """{name: T} of the function's own pointers, `T *name;` (or `T __far *name;`): what an offset
    through one points at (_far_addresses)."""
    out = {}
    for v in locs:
        n, t = (v.get('sym') or {}).get('name'), v.get('type')
        m = re.fullmatch(r"([A-Za-z_]\w*(?: [A-Za-z_]\w*)?)(?: __far)? \*(\w+)", type_text(t, n, n in _now['far']))
        if m:
            out[m.group(2)] = m.group(1)
    return out


def _old_style(doc):
    """The definition in the older style, `f(a,b)` then `int2 a;` `int2 b;` before its `{`
    (c_text._open_arguments): written with its argument types in the brackets a definition also
    fixes how every later call must look, and the decompiler recovers each call on its own
    evidence (a caller that passes three words where the definition takes a far pointer and a
    word passes the same bytes, not the same C). -> [(name, declaration)], or None where it stays
    as it is: an argument whose declaration has brackets in it (a pointer to a function), `...`,
    a heading c_text's parser lost (_lost)."""
    ft = (doc.get('sym') or {}).get('type') or {}
    if ft.get('variadic') or _lost(doc):
        return None
    if any(_FASTCALL.fullmatch(m) for m in doc.get('mods') or ()) and _stack(_ret(doc))[0]:
        return None                                      # (`T * __fastcall16near f(...)`: lost too)
    out = []
    for p in doc.get('params') or []:
        decl = type_text(p.get('type'), _param_name(p), _param_name(p) in _now['far'])
        m = re.search(r"([A-Za-z_]\w*)\s*(?:\[\d*\])?$", decl)
        if '(' in decl or ')' in decl or not m:
            return None
        out.append((m.group(1), decl))
    return out


def _register_arguments(doc):
    """`__thiscall` (the first argument in ECX) or `__fastcall` (the first two in ECX and EDX): a
    32-bit function's convention the module's compiler has no word for (c_text's compilable32 took
    it out of the heading, the function in its in_ecx or in_edx) -- reported as the facts'
    "registers", "ecx" or "edx"; else None."""
    mods = doc.get('mods') or ()
    for word, reg in (('__thiscall', 'ecx'), ('__fastcall', 'edx')):
        if word in mods:
            _now['facts']['registers'] = reg
            return word
    return None


def signature(doc):
    """`int2 __cdecl16far sub_1_0000(void)`"""
    return _flat(_signature, doc)


def function(doc, moved=(), far=None, facts=None):
    """C text of one function document (a MethodDecl), with the c_text passes `moved` done here.
    `far`: the names of its four-byte pointers where the decompiler said them apart from the types
    (its pointers' sizes, which c_text went by: a variable can be four bytes wide and typed as a
    near pointer); else they are read off the types. `facts`: what the module says of it, for the
    passes IN_MODULE (left out without them) -- distance and model (its description line), widths
    ({far: {a<n>: bytes}}: c_text._far_argument_widths), loads (c_text._far_loads), export and
    loadds (how its instructions start)."""
    if doc.get('node') != 'MethodDecl':
        raise Decline('document %s' % doc.get('node'))
    sixteen = 'segment' in (doc.get('origin') or {})
    locs = _locals(doc.get('body'), [])
    if facts is None:
        moved = [p for p in moved if p not in IN_MODULE]
    _now['facts'], _now['doc'] = (facts if facts is not None else {}), doc
    _now['lane'] = (facts or {}).get('lane')
    flat = POWERPC if _now['lane'] == 'powerpc' else FLAT
    _now['moved'] = tuple(p for p in moved if p not in FLAT_ONLY) if sixteen else tuple(p for p in moved if p in flat)
    _now['sixteen'] = sixteen
    _now['locals'], _now['drop'] = locs, set()
    # the far pointers: the parameters and locals of four bytes
    _now['far'] = (set(far) if far is not None else
                   {n for n, t in [(_param_name(p), p.get('type')) for p in doc.get('params') or []] +
                    [((v.get('sym') or {}).get('name'), v.get('type')) for v in locs] if is_far(t)}) \
        if 'far' in _now['moved'] else set()
    _now['old_style'] = _old_style(doc) if 'open_arguments' in _now['moved'] else None
    _now['far_pointers'] = _far_pointers(locs) if 'far_addresses' in _now['moved'] else {}
    _now['alias'] = dict(_now['facts'].get('aliases') or {}) if 'global_widths' in _now['moved'] else {}
    try:
        if 'far_from_stack' in _now['moved'] and _now['far']:
            _far_from_stack(doc.get('body'), locs)
        nodes, arrays = list(_walk(doc.get('body'))), _frame_arrays(locs)
        for name, rule in _PASSES:
            if name in _now['moved']:
                rule(nodes, arrays)
        if not sixteen and facts is not None:
            # (the heading as it is written, its words one space apart: the module declares the
            # function with it -- compilable32)
            em = _Pretty()
            em.open_group()
            em.tag_line()
            _signature(em, doc)
            em.close_group()
            facts['declared'] = ' '.join(em.text().split())
        return _function(doc, locs)
    finally:
        _now['moved'], _now['far'], _now['marks'], _now['whole'], _now['bare'] = (), set(), {}, {}, set()
        _now['facts'], _now['doc'], _now['locals'], _now['drop'] = {}, None, [], set()
        _now['old_style'], _now['room'], _now['gone'], _now['far_pointers'] = None, set(), set(), {}
        _now['gap_base'], _now['decls'], _now['seen'], _now['lines'] = {}, {}, set(), set()
        _now['at'], _now['top'], _now['virtual'], _now['ren'], _now['locals_text'] = {}, None, {}, {}, None
        _now['ss_casts'], _now['far_read'] = set(), set()
        _now['before'], _now['after'], _now['stmt_at'], _now['pops'] = {}, {}, {}, False
        _now["declare_first"], _now["seg_ptrs"], _now["frame"], _now["alias"] = [], [], set(), {}
        _now["lane"] = None


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


def _function(doc, locs, em=None):
    """PrintC::docFunction: the comments above it, the declaration, `{` a line below, the locals,
    the statements, `}`. (`em`: another emitter -- _seen's, on one line.) The module's C has
    no comments above a function (the facts' "without_notes": c_text took the core's notes off
    each text)."""
    notes = (doc.get('comments') or ()) if em is not None or not _now['facts'].get('without_notes') else ()
    em = em or _Pretty()
    em.open_group()                                      # (the function)
    for text in notes:
        _comment(em, text, 0)
    if notes:
        em.tag_line()
    em.tag_line()
    _signature(em, doc)
    for _n, decl in _now['old_style'] or ():
        em.tag_line(0)
        em.print(decl + ';')
    if _now['old_style'] is None:                     # (the older style: its `{` on the next line)
        em.tag_line()
    em.tag_line()
    em.start_indent()
    em.print('{')
    for ty, name in reversed(_now['declare_first']):
        # (a local a pass declares before the others, each put first: _caller_frame_slots)
        em.tag_line()
        em.print('', '%s %s;' % (ty, name))
    if _now['locals_text'] is not None:
        # (the locals as a pass rewrote their declarations' text, blank line after them included:
        # _buffer_fields, which c_text did on the text of them)
        em.print('', _now['locals_text'])
        locs = ()
    for loc in locs:
        name = (loc.get('sym') or {}).get('name', '')
        if name in _now['drop']:
            continue                                     # (a pass took its declaration's line away)
        em.tag_line()
        em.open_group()
        if name in _now['decls']:                        # (a pass declared it otherwise: _far_locals)
            em.print(type_text(loc.get('type'), name, name in _now['far']), _now['decls'][name])
        else:
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
