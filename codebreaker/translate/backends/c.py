"""c.py -- the hub's C back end: an L2 function document as C (docs/IR_UNIFICATION.md, step 2).

The decompiler's PrintL2 writes each function as L2 (`/*@L2 {...} */`, CB_L2=1). This writes it
back as C in the decompiler's own dialect -- its type names (int2, uint4, xunknown2, code), its
calling-convention words (__cdecl16far, __stdcall), hexadecimal past 9 -- so what it writes goes
through decompile/c_text.py's passes as the decompiler's C does today, and is compiled and judged
the same way. tools/l2c.py holds it to that C, token for token, over the golden corpus.

Works on the JSON document itself (a dict), not on ir.py's classes: L2 from a binary carries
address spaces, conventions and layouts the Java front end's classes do not need.

    function(doc, moved=()) -> C text of one function          type_text(t, name='') -> a declaration

`moved` names the passes of decompile/c_text.py done here instead, on the tree (step 3): each is
written where the address spaces and types L2 carries say so, and c_text leaves its own out for C
written from L2. MOVED is every one there is.
"""

import re

# c_text's passes done on L2 (docs/IR_UNIFICATION.md step 3, L2_BINARY_TYPES.md's order)
MOVED = ("far", "near_calls", "far_from_stack")
_now = {"moved": (), "far": set(), "marks": {}}  # the function being written: its passes, its far names,
                                               # and the nodes they rewrite ({id(node): '_far' | '_near' | '_ss'})

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


# ---------------------------------------------------------------------------------------------- types
def _base(t):
    """(text before the name, text after it) for a type: `int2 *` + `p` + `[4]`."""
    k = t['kind']
    if k in _PRIM:
        return _PRIM[k], ''
    if k == 'class':
        return t.get('name') or 'undefined', ''
    if k == 'record':
        if t.get('name'):
            return t['name'], ''
        return _record(t), ''
    if k == 'ptr':
        pre, post = _base(t['to'])
        if post:                                 # pointer to an array: int2 (*p)[4]
            return pre + ' (*', ')' + post
        # (a far one too: its width is the /*@P note's) -- `int2 *`, `int2 **`
        return pre + ('*' if pre.endswith('*') else ' *'), ''
    if k == 'fnptr':
        return 'code *', ''
    if k == 'fn':
        return 'code', ''
    if k == 'array':
        pre, post = _base(t['elem'])
        return pre, '[%s]%s' % (t['count'] if 'count' in t else '', post)
    if k == 'selector':
        return 'uint2', ''
    raise Decline('type kind %r' % k)


def _record(t):
    """An unnamed structure written out (the frame the decompiler keeps at its offsets)."""
    fields = ' '.join(type_text(f['type'], f.get('name', '_f%x' % f.get('offset', 0))) + ';'
                      for f in t.get('fields', []))
    return '%sstruct { %s }' % ('_Packed ' if t.get('packed') else '', fields)


def _far_stars(pre):
    """`int2 *` -> `int2 __far *`, `int2 **` -> `int2 __far **`: the pointer is far (c_text._far)."""
    stars = len(pre) - len(pre.rstrip('*'))
    return pre[:len(pre) - stars].rstrip() + ' __far ' + '*' * stars


def is_far(t):
    """A pointer of four bytes in a 16-bit function: selector and offset."""
    return bool(t) and t.get('kind') in ('ptr', 'fnptr') and t.get('space') == 'far'


def type_text(t, name='', far=False):
    """A declaration of `name` with type `t` (`int2 *p`, `xunknown1 a [22]` -- PrintC's spacing,
    which c_text's passes read), or the type alone. `far`: the outer pointer written __far."""
    if t is None:
        return ('void ' + name).strip()
    pre, post = _base(t)
    if far and pre.endswith('*') and not post:
        pre = _far_stars(pre)
    if not name:
        return (pre + post).strip()
    sep = '' if pre.endswith('*') or pre.endswith('(') else ' '
    return pre + sep + name + (' ' + post if post.startswith('[') else post)


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
    return _expr(e)


def _sub(parent, child, stage):
    bracketed = _bracket(parent, child, stage)
    if bracketed:
        _near_call(child)
    text = _expr(child)
    return '(%s)' % text if bracketed else text


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
    one is DS and it, so its cast stays near (c_text.data_address: LZEXPAND's LZDONE)."""
    if e.get('node') != 'Lit':
        return False
    m = re.fullmatch(r"(0x[0-9a-fA-F]+|\d+)[uUlL]*", _lit(e))     # (as it is written: no character)
    return bool(m) and 0 < int(m.group(1), 0) <= 0xFFFF


def _assigned_name(target):
    """`x` of `x = ...`, `*x = ...`, `**x = ...`"""
    while target.get('node') == 'Unary' and target.get('op') == '*' and not target.get('postfix'):
        target = target['x']
    return (target.get('sym') or {}).get('name') if target.get('node') == 'Name' else None


def _expr(e):
    n = e['node']
    if n == 'Lit':
        return _lit(e)
    if n == 'Name':
        if _now['marks'].get(id(e)) == '_ss':
            return 'MK_FP(codebreaker_ss(), %s)' % _name(e)
        return _name(e)
    if n == 'Paren':
        _near_call(e['x'])
        return '(%s)' % _expr(e['x'])
    tok = _token(e)
    if n == 'Cast':
        if _now['marks'].get(id(e)) == '_near':
            return '(code __near *)%s' % _sub(tok, e['x'], 1)
        return '(%s)%s' % (type_text(e['type'], far=_now['marks'].get(id(e)) == '_far'), _sub(tok, e['x'], 1))
    if n == 'Unary':
        op = e['op']
        if e.get('postfix'):
            return _sub(tok, e['x'], 0) + op
        return op + _sub(tok, e['x'], 1)
    if n == 'Binary':
        op = e['op']
        left, right = _sub(tok, e['left'], 0), _sub(tok, e['right'], 1)
        return '%s,%s' % (left, right) if op == ',' else '%s %s %s' % (left, op, right)   # (PrintC::comma: no space)
    if n == 'Assign':
        if 'far' in _now['moved'] and not e.get('op') and _assigned_name(e['target']) in _now['far']:
            cast = _leading_cast(e['value'])             # `p = (T *)x` -> `p = (T __far *)x` (c_text._far)
            if cast is not None and (cast.get('type') or {}).get('kind') in ('ptr', 'fnptr') and \
                    not _data_address(cast['x']):
                _now['marks'][id(cast)] = '_far'
        return '%s %s %s' % (_sub(tok, e['target'], 0), tok[1], _sub(tok, e['value'], 1))
    if n == 'Cond':
        return '%s ? %s : %s' % (_sub(tok, e['cond'], 0), _expr(e['then']), _sub(tok, e['orelse'], 1))
    if n == 'Call':
        target = e.get('target')
        head = _sub(tok, target, 0) if target else (e.get('sym') or {}).get('name', '?')
        return '%s(%s)' % (head, ','.join(_expr(a) for a in (e.get('args') or [])))
    if n == 'Index':
        return '%s[%s]' % (_sub(tok, e['arr'], 0), _expr(e['index']))
    if n == 'Select':
        if _arrow(e):
            return '%s->%s' % (_sub(tok, e['obj']['x'], 0), _name(e))
        return '%s.%s' % (_sub(tok, e['obj'], 0), _name(e))
    raise Decline('expression %s' % n)


def _joined(e):
    """A condition the structure joined (PrintL2 marks one so: both sides in Paren)."""
    return e.get('node') == 'Binary' and e.get('op') in ('&&', '||') and \
        e['left'].get('node') == 'Paren' and e['right'].get('node') == 'Paren'


def condition(e, inner=False):
    """A branch's condition as PrintC brackets it (emitBlockCondition, opCbranch): each condition
    in brackets of its own, but not one in a comma list (a loop's) or on the right of a joining
    && / ||, which the join brackets instead -- `if ((a) && (b))`, `if (((a) && (b)) && ((c || (d))))`."""
    if _joined(e):
        return '(%s %s (%s))' % (condition(e['left']['x'], inner), e['op'], condition(e['right']['x'], True))
    text = ', '.join(expr(x) for x in _listed(e))
    return text if inner else '(%s)' % text


def _listed(e):
    """A condition block's statements and its test, joined by PrintL2 with `,` (PrintC writes them
    `a, b`: statements, not the comma operator, which it writes `a,b`)."""
    if e.get('node') == 'Binary' and e.get('op') == ',':
        return _listed(e['left']) + [e['right']]
    return [e]


# ---------------------------------------------------------------------------------------------- statements
def _true(e):
    return e.get('node') == 'Lit' and e.get('value') is True


def _loop(cond):
    """A while's condition: PrintC writes the endless one `while( true )`; others in comma-list mode."""
    return '( true )' if _true(cond) else ' (%s)' % condition(cond, True)


def _stmts(s):
    if s is None:
        return []
    return s if isinstance(s, list) else [s]


def _block(out, stmts, depth):
    for s in _stmts(stmts):
        _stmt(out, s, depth)


def _if(out, s, depth, word):
    """PrintC::emitBlockIf: `if (c) goto x;` braceless where the then-part is one statement, not a
    block; an else holding one if and nothing else merged into `else if`."""
    pad = '  ' * depth
    if isinstance(s.get('then'), dict):
        line = []
        _stmt(line, s['then'], 0)
        out.append(pad + '%s %s %s' % (word, condition(s['cond']), line[0]))
        return
    out.append(pad + '%s %s {' % (word, condition(s['cond'])))
    _block(out, s.get('then'), depth + 1)
    out.append(pad + '}')
    orelse = s.get('orelse')
    if orelse is None:
        return
    if isinstance(orelse, list) and len(orelse) == 1 and orelse[0]['node'] == 'If':
        _if(out, orelse[0], depth, 'else if')
        return
    out.append(pad + 'else {')                   # (an else with nothing in it is still written)
    _block(out, orelse, depth + 1)
    out.append(pad + '}')


def _stmt(out, s, depth):
    pad = '  ' * depth
    n = s['node']
    if n == 'Local':
        return                                   # declared at the top, as the decompiler does
    if n == 'ExprStmt':
        out.append(pad + expr(s['expr']) + ';')
    elif n == 'Return':
        out.append(pad + ('return %s;' % expr(s['expr']) if s.get('expr') else 'return;'))
    elif n == 'If':
        _if(out, s, depth, 'if')
    elif n == 'While':
        out.append(pad + 'while%s {' % _loop(s['cond']))
        _block(out, s.get('body'), depth + 1)
        out.append(pad + '}')
    elif n == 'DoWhile':
        out.append(pad + 'do {')
        _block(out, s.get('body'), depth + 1)
        out.append(pad + '} while%s;' % ('( true )' if _true(s['cond']) else ' ' + condition(s['cond'])))
    elif n == 'For':
        init = '; '.join(expr(x['expr'] if x.get('node') == 'ExprStmt' else x) for x in _stmts(s.get('init')))
        update = ','.join(expr(x['expr'] if x.get('node') == 'ExprStmt' else x) for x in _stmts(s.get('update')))
        cond = condition(s['cond'], True) if s.get('cond') else ''
        out.append(pad + 'for (%s; %s; %s) {' % (init, cond, update))
        _block(out, s.get('body'), depth + 1)
        out.append(pad + '}')
    elif n == 'Switch':
        out.append(pad + 'switch(%s) {' % expr(s['expr']))
        for c in _stmts(s.get('cases')):
            for label in _stmts(c.get('labels')):
                out.append(pad + 'case %s:' % expr(label))
            if c.get('default'):
                out.append(pad + 'default:')
            _block(out, c.get('body'), depth + 1)
        out.append(pad + '}')
    elif n == 'Goto':
        out.append(pad + 'goto %s;' % s['label'])
    elif n == 'Labeled':
        out.append(s['label'] + ':')
        _block(out, s.get('body'), depth)
    elif n == 'Break':
        out.append(pad + 'break;')
    elif n == 'Continue':
        out.append(pad + 'continue;')
    elif n == 'Block':
        _block(out, s.get('stmts'), depth)
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
    arrays = {(v.get('sym') or {}).get('name') for v in locs
              if (v.get('type') or {}).get('kind') == 'array' and 'count' in v['type']
              and v['type']['elem'].get('kind') != 'array' and _FRAME.fullmatch((v.get('sym') or {}).get('name') or '')}
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


# ---------------------------------------------------------------------------------------------- functions
def _param_name(p):
    return (p.get('sym') or {}).get('name') or p.get('name', '')


def signature(doc):
    """`int2 __cdecl16far sub_1_0000(void)`"""
    sym = doc.get('sym') or {}
    ft = sym.get('type') or {}
    ret = type_text(doc.get('ret') or (ft.get('result') or {}).get('type'))
    params = [type_text(p.get('type'), _param_name(p), far=_param_name(p) in _now['far'])
              for p in doc.get('params') or []]
    if ft.get('variadic'):
        params.append('...')
    # the declaration's modifiers as the decompiler wrote them: the convention's word, where the
    # function's is not the module's default (`__stdcall16near`)
    mods = ''.join(m + ' ' for m in doc.get('mods') or ())
    return '%s %s%s(%s)' % (ret, mods, doc['name'], ','.join(params) or 'void')


def function(doc, moved=(), far=None):
    """C text of one function document (a MethodDecl), with the c_text passes `moved` done here.
    `far`: the names of its four-byte pointers where the decompiler said them apart from the types
    (its /*@P name 4 */ notes, which c_text goes by: a variable can be four bytes wide and typed as
    a near pointer); else they are read off the types."""
    if doc.get('node') != 'MethodDecl':
        raise Decline('document %s' % doc.get('node'))
    sixteen = 'segment' in (doc.get('origin') or {})
    locs = _locals(doc.get('body'), [])
    _now['moved'] = tuple(moved) if sixteen else ()
    # the far pointers: the parameters and locals of four bytes
    _now['far'] = (set(far) if far is not None else
                   {n for n, t in [(_param_name(p), p.get('type')) for p in doc.get('params') or []] +
                    [((v.get('sym') or {}).get('name'), v.get('type')) for v in locs] if is_far(t)}) \
        if 'far' in _now['moved'] else set()
    try:
        if 'far_from_stack' in _now['moved'] and _now['far']:
            _far_from_stack(doc.get('body'), locs)
        return _function(doc, locs)
    finally:
        _now['moved'], _now['far'], _now['marks'] = (), set(), {}


def _function(doc, locs):
    out = [signature(doc), '', '{']
    for loc in locs:
        name = (loc.get('sym') or {}).get('name', '')
        out.append('  ' + type_text(loc.get('type'), name, far=name in _now['far']) + ';')
    out.append('  ')
    _block(out, doc.get('body'), 1)
    out.append('}')
    return '\n'.join(out) + '\n'
