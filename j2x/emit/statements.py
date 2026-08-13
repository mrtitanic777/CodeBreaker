# -*- coding: utf-8 -*-
"""
statements.py -- Module 4c: method-body / statement translation.

PASS 1 (this file): the foundation -- a per-method SYMBOL TABLE (params + own
fields + locals, each with its JType) and value/pointer ACCESS translation:
  * `this.x`            -> `this->x`           (this is a pointer in C++)
  * `<ptrVar>.x`        -> `<ptrVar>->x`       (receiver is a reference type)
  * `<valueVar>.x`      -> unchanged           (receiver is a value/struct type)
  * local declarations  -> C++ type            (`Block var1` -> `Block* var1`)

This is the #1 error class (C2227/C2228, `.` vs `->`) decided from resolved
types instead of patched afterward. It is conservative: only names KNOWN to be
pointers get rewritten; unknown receivers are left alone.

Later passes layer on: Java-API -> STL, `new`/cast translation, null-deref
guards, labeled break -> goto, class-static `X.Y` -> `X::s_Y`, the real
`this(...)` delegation inline, and dual-use value types. Bodies are therefore
PARTIALLY translated until those land -- that's the iterative 4c plan.
"""

import re

from .. import parse as javaparse, typemap, ir
from ..ledger import Category, Severity
from . import naming, ownership

# a standalone statement `recv[idx]->call(...);` (or `.` form) -- group2 = base name
_INDEX_DEREF_STMT = re.compile(
    r'^((?:this->)?(?:\w+::)*(\w+)\[[^\]]*\])\s*(?:->|\.)\s*(\w+\([^;]*\))\s*;\s*$')

# string / char literals -- substitutions that rename identifiers must skip these (renaming a
# word inside "stone" would corrupt sound/texture resource names, etc.)
_STR_LITERAL = re.compile(r'"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'')


def sub_outside_literals(rx, repl, s):
    """re.sub(rx, repl, s) applied ONLY to the parts of s outside string/char literals."""
    out, last = [], 0
    for m in _STR_LITERAL.finditer(s):
        out.append(rx.sub(repl, s[last:m.start()]))
        out.append(m.group(0))
        last = m.end()
    out.append(rx.sub(repl, s[last:]))
    return ''.join(out)

# Java keywords that must never be mistaken for a local-variable TYPE.
_KEYWORDS = {
    'return', 'if', 'else', 'while', 'for', 'do', 'switch', 'case', 'break',
    'continue', 'throw', 'throws', 'new', 'this', 'super', 'synchronized',
    'try', 'catch', 'finally', 'instanceof', 'assert', 'default', 'void',
}

# `[final] Type name =|;`  -- a local declaration (Type may have generics / [] dims)
_DECL = re.compile(
    r'^(?:final\s+)?([A-Za-z_][\w.]*(?:<[^>]*>)?(?:\s*\[\s*\])*)\s+([A-Za-z_]\w*)\s*(=|;)')

_MATH = re.compile(r'\bMath\.([A-Za-z_]\w*)')
# new T[n] (1-D, not already value-init, NOT an aggregate `new T[]{...}`) -> type, optional *, size
_NEW_ARRAY = re.compile(r'\bnew\s+([\w:]+)\s*(\*?)\s*\[([^\]]*)\](?!\s*[\[({])')
# new T[]{a, b, c} aggregate -- captured separately and built via JArrayInit
_NEW_ARRAY_AGG = re.compile(r'\bnew\s+([\w:]+)\s*((?:\*?\s*\[\s*\]\s*)+)\{')   # T[]...[]{ (any dims)
# multi-dim SIZED new (not an aggregate `{`): new T[a][b] and new T[a][b][c]
_NEW_ARRAY_3D = re.compile(r'\bnew\s+([\w:]+)\s*\*?\s*\[([^\]]*)\]\s*\[([^\]]*)\]\s*\[([^\]]*)\](?!\s*[\[{])')
_NEW_ARRAY_2D = re.compile(r'\bnew\s+([\w:]+)\s*\*?\s*\[([^\]]*)\]\s*\[([^\]]*)\](?!\s*[\[{])')


def _rewrite_multidim_new(s, resolver):
    """`new T[a][b]` -> j2x_newarr2<elem>(a,b); `new T[a][b][c]` -> j2x_newarr3<elem>(a,b,c). elem is
    the scalar element C++ type (int / Chunk*). Sized only -- aggregate `new T[][]{...}` is separate."""
    def elem(t):
        return resolver.cpp_type(ir.JType(t), 'local')
    s = _NEW_ARRAY_3D.sub(lambda m: 'j2x_newarr3<%s >(%s, %s, %s)'
                          % (elem(m.group(1)), m.group(2).strip(), m.group(3).strip(), m.group(4).strip()), s)
    s = _NEW_ARRAY_2D.sub(lambda m: 'j2x_newarr2<%s >(%s, %s)'
                          % (elem(m.group(1)), m.group(2).strip(), m.group(3).strip()), s)
    return s
# x instanceof Type  -- x may be a var, a this->field chain, an [index], or a method CALL
_INSTANCEOF = re.compile(
    r'([A-Za-z_]\w*(?:\s*(?:->|\.|::)\s*[A-Za-z_]\w*|\s*\([^()]*\)|\s*\[[^\]]*\])*)'
    r'\s+instanceof\s+([A-Za-z_]\w*(?:\s*\[\s*\])?)')      # type may be an array (String[])
# receiver.member  with optional opening paren (method call)
_MEMBER = re.compile(r'\b([A-Za-z_]\w*)\.([A-Za-z_]\w*)(\s*\()?')
_CALL = re.compile(r'\b(\w+)\.(\w+)\s*\(')
# a cast `(Type)` immediately followed by an operand (word or '(') -- not a grouping
_CAST = re.compile(r'\(([A-Za-z_]\w*)\)(?=\s*[\w("\'\-+~!])')   # cast target may be a var, (, literal, or unary -/+/~/! (e.g. (byte)-1)

# receiver categories
_LIST_TYPES = {'ArrayList', 'List', 'LinkedList'}
_MAP_TYPES = {'HashMap', 'Map', 'TreeMap'}
_SET_TYPES = {'HashSet', 'Set'}

# simple renames (args preserved): (category, java method) -> C++ method
_METHOD_RENAME = {
    ('list', 'add'): 'push_back', ('list', 'isEmpty'): 'empty',
    ('map', 'isEmpty'): 'empty', ('map', 'containsKey'): 'count', ('map', 'remove'): 'erase',
    ('set', 'add'): 'insert', ('set', 'isEmpty'): 'empty', ('set', 'contains'): 'count', ('set', 'remove'): 'erase',
    ('string', 'isEmpty'): 'empty',
}
# accessor that becomes recv[arg]: category -> java method
_INDEX_METHOD = {'list': 'get', 'map': 'get', 'string': 'charAt'}


def _split_top_plus(expr):
    """Split on top-level binary '+' (respecting (), [], {}, string/char literals;
    not splitting on ++ or +=)."""
    parts, depth, buf, i, n = [], 0, [], 0, len(expr)
    in_str = in_chr = False
    while i < n:
        c = expr[i]
        if in_str:
            buf.append(c)
            if c == '\\' and i + 1 < n:
                buf.append(expr[i + 1]); i += 2; continue
            if c == '"':
                in_str = False
            i += 1; continue
        if in_chr:
            buf.append(c)
            if c == '\\' and i + 1 < n:
                buf.append(expr[i + 1]); i += 2; continue
            if c == "'":
                in_chr = False
            i += 1; continue
        if c == '"':
            in_str = True; buf.append(c); i += 1; continue
        if c == "'":
            in_chr = True; buf.append(c); i += 1; continue
        if c in '([{':
            depth += 1; buf.append(c); i += 1; continue
        if c in ')]}':
            depth -= 1; buf.append(c); i += 1; continue
        if c == '+' and depth == 0:
            nxt = expr[i + 1] if i + 1 < n else ''
            prv = expr[i - 1] if i else ''
            if nxt in '+=' or prv == '+':            # ++ or += -- not a concat
                buf.append(c); i += 1; continue
            parts.append(''.join(buf)); buf = []; i += 1; continue
        buf.append(c); i += 1
    if ''.join(buf).strip():
        parts.append(''.join(buf))
    return [p.strip() for p in parts]


def _maybe_concat(expr):
    """If expr is a Java string concatenation (top-level '+' with a string literal
    operand), return the ostringstream `<<` chain; else None."""
    if '"' not in expr or '+' not in expr:
        return None
    parts = _split_top_plus(expr)
    if len(parts) < 2 or not any(p.startswith('"') for p in parts):
        return None
    return ' << '.join(parts)


def _split_top_commas(expr):
    """Split on top-level commas (respecting (), [], {}, and string/char literals). '<>'
    is NOT tracked -- it would mis-balance on '->' and comparisons; call-arg generics with
    commas don't occur in this emitted code."""
    parts, depth, buf, i, n = [], 0, [], 0, len(expr)
    while i < n:
        c = expr[i]
        if c == '"' or c == "'":
            q = c; buf.append(c); i += 1
            while i < n:
                buf.append(expr[i])
                if expr[i] == '\\' and i + 1 < n:
                    buf.append(expr[i + 1]); i += 2; continue
                if expr[i] == q:
                    i += 1; break
                i += 1
            continue
        if c in '([{':
            depth += 1
        elif c in ')]}':
            depth -= 1
        elif c == ',' and depth == 0:
            parts.append(''.join(buf)); buf = []; i += 1; continue
        buf.append(c); i += 1
    if buf:
        parts.append(''.join(buf))
    return [p.strip() for p in parts]


_FLOAT_LIT = r'\d+\.\d*[FfDd]?|\.\d+[FfDd]?|\d+[FfDd]'
_MOD_TERM = r'(?:\([^()]*\)|[A-Za-z_]\w*(?:(?:->|\.)\w+|\[[^\]\[]*\])*|\d+\.?\d*[FfDd]?|\.\d+[FfDd]?)'
_MOD_PAT = re.compile(r'(%s)\s*%%\s*(%s)' % (_MOD_TERM, _MOD_TERM))
_FLOAT_RE = re.compile(r'^(?:%s)$' % _FLOAT_LIT)


# Java numeric literal with a type suffix. Decimal only (the negative lookbehind for a word char
# before the number keeps us out of hex like 0x1D / 0xABCDL and out of identifiers like var2D).
_NUM_SUFFIX = re.compile(
    r'(?<![\w.])(\d+\.?\d*(?:[eE][+-]?\d+)?|\.\d+(?:[eE][+-]?\d+)?)([DdLl])(?![\w.])')


def _fix_numeric_suffixes(s):
    """Normalize Java numeric-literal suffixes to valid C++03 (MSVC/XDK rejects what g++ tolerated):
      D/d (double) -> stripped (and `.0` added if it would otherwise read as an integer),
      L/l (Java long, 64-bit) -> LL.
      F/f (float) is left alone -- already valid C++. Applied OUTSIDE string/char literals only."""
    def rep(m):
        num, suf = m.group(1), m.group(2)
        if suf in 'Ll':
            return num + 'LL'
        # D/d: a double literal. Keep it double-typed: add .0 if there's no '.' and no exponent.
        if '.' not in num and 'e' not in num and 'E' not in num:
            return num + '.0'
        return num
    return sub_outside_literals(_NUM_SUFFIX, rep, s)


def _fix_unicode_literals(s):
    """Make Unicode char/string escapes code-page-safe for MSVC (execution charset cp1252) -> no C4566:
      * A Java char literal '\\uXXXX' is a 16-bit integer; emit it as 0xXXXX so a mask like
        `x & '\\uffff'` becomes `x & 0xffff` -- exact value, no execution-charset encoding needed.
      * U+2302 (the '⌂' font glyph) is NOT representable in cp1252 (-> C4566 + a mangled byte).
        Emit its CP437 byte 0x7F, which is ALSO the correct Minecraft font tile: as '\\x7f' (char)
        or \\x7f (inside a string). Other accented chars in the font string are <=0xFF and cp1252-
        representable (no warning) -- their CP437-vs-cp1252 tile offset is a separate font-correctness
        item, not a warning."""
    # NB: the source carries U+2302 as the 6-char ASCII escape ⌂ (verified), so match that
    # exact text. Do the 2302 -> 7F rewrite BEFORE the generic char-literal rule so the char form
    # '⌂' becomes the CP437 byte 0x7F (font tile) and NOT its raw value 0x2302.
    s = s.replace("'\\u2302'", "'\\x7f'")     # char-literal form of the ⌂ glyph -> CP437 byte 0x7F
    s = s.replace("\\u2302", "\\x7f")          # ... and inside string literals
    s = re.sub(r"'\\u([0-9a-fA-F]{4})'", lambda m: '0x' + m.group(1), s)   # other char literals -> int value
    return s


# --- C4554: clarify shift/bitwise precedence by adding parentheses --------------------------------
# MSVC warns (C4554) on `a << b + c`, `x >> 8 & 255`, `p ^ q <= r` etc. -- legal and identical in
# precedence between Java and C++ (so never a real bug here), but the compiler wants parentheses.
# We RE-PARSE the expression with C precedence (the SAME the compiler uses) and re-emit it with the
# minimum parentheses needed for correctness PLUS a clarifying pair wherever a shift/bitwise operator
# meets a different-precedence operator. Because the parse tree uses real C precedence, the re-emitted
# expression is provably equivalent. Anything we can't cleanly tokenize (casts, ternary, templates,
# calls-with-operators-as-args we don't recognize) makes the parser BAIL -> original text unchanged.
_PREC = {'||': 1, '&&': 2, '|': 3, '^': 4, '&': 5, '==': 6, '!=': 6,
         '<': 7, '<=': 7, '>': 7, '>=': 7, '<<': 8, '>>': 8,
         '+': 9, '-': 9, '*': 10, '/': 10, '%': 10}
_WARN_OPS = frozenset(('<<', '>>', '&', '|', '^'))
_BINOP_RE = re.compile(r'(<<|>>|<=|>=|==|!=|&&|\|\||[+\-*/%&^|<>])')


class _Bail(Exception):
    pass


def _match_bracket(s, i, op, cl):
    """Index of the bracket matching s[i]==op, or raise _Bail."""
    depth = 0
    n = len(s)
    while i < n:
        if s[i] == op:
            depth += 1
        elif s[i] == cl:
            depth -= 1
            if depth == 0:
                return i
        i += 1
    raise _Bail()


def _scan_atom(s, i):
    """Scan one operand (optional prefix unary/!/~/-/+, a primary, then ./->/::/[]/() postfix).
    Returns (text, next_index). Raises _Bail on anything unexpected (casts, `new`, etc.)."""
    n = len(s)
    start = i
    while i < n and s[i] in '!~-+':            # prefix unary (binary +/- never reach here)
        i += 1
    if i >= n:
        raise _Bail()
    c = s[i]
    if c == '(':                                # parenthesized primary  (a cast `(int)x` -> Bail later)
        i = _match_bracket(s, i, '(', ')') + 1
    elif c.isdigit():
        while i < n and (s[i].isalnum() or s[i] in '.x'):
            i += 1
    elif c.isalpha() or c == '_':
        while i < n and (s[i].isalnum() or s[i] == '_'):
            i += 1
    else:
        raise _Bail()
    while i < n:                                # postfix chain
        if s[i] == '.':
            i += 1
            while i < n and (s[i].isalnum() or s[i] == '_'):
                i += 1
        elif s[i:i + 2] in ('->', '::'):
            i += 2
            while i < n and (s[i].isalnum() or s[i] == '_'):
                i += 1
        elif s[i] == '[':
            i = _match_bracket(s, i, '[', ']') + 1
        elif s[i] == '(':
            i = _match_bracket(s, i, '(', ')') + 1
        else:
            break
    return s[start:i], i


def _reparen_expr(expr):
    """Re-parenthesize a standalone expression to clarify shift/bitwise precedence. Returns the
    original text unchanged if there is nothing to clarify or if it can't be parsed safely."""
    s = expr.strip()
    if not s or not any(w in s for w in ('<<', '>>', '^', '&', '|')):
        return expr
    try:
        toks, i, n = [], 0, len(s)
        expect_atom = True
        while i < n:
            if s[i].isspace():
                i += 1
                continue
            if expect_atom:
                atom, i = _scan_atom(s, i)
                toks.append(('a', atom))
                expect_atom = False
            else:
                m = _BINOP_RE.match(s, i)
                if not m or m.group(1) not in _PREC:
                    raise _Bail()
                toks.append(('o', m.group(1)))
                i = m.end()
                expect_atom = True
        if not toks or toks[-1][0] != 'a' or len(toks) < 3:
            return expr
        pos = [0]

        def parse(min_prec):
            node = toks[pos[0]]; pos[0] += 1          # ('a', text)
            while pos[0] < len(toks):
                op = toks[pos[0]][1]
                p = _PREC[op]
                if p < min_prec:
                    break
                pos[0] += 1
                node = ('b', op, p, node, parse(p + 1))   # left-assoc
            return node

        tree = parse(1)
        if pos[0] != len(toks):
            return expr

        def emit(node, parent_p, parent_op, is_right):
            if node[0] == 'a':
                return _reparen_atom(node[1].strip())
            _, op, p, l, r = node
            txt = '%s %s %s' % (emit(l, p, op, False), op, emit(r, p, op, True))
            if parent_op is None:
                return txt
            need = (p < parent_p) or (p == parent_p and is_right)         # correctness
            if (op in _WARN_OPS or parent_op in _WARN_OPS) and p != parent_p:
                need = True                                               # C4554 clarity
            return '(%s)' % txt if need else txt

        out = emit(tree, 0, None, False)
        return expr.replace(s, out, 1) if out != s else expr
    except _Bail:
        return expr


def _reparen_atom(text):
    """If an operand is a pure parenthesized group (optionally with a leading unary), recurse into it
    so shifts/bitwise nested inside parens get clarified too -- `(var3 + var4 >> 1 << 24)`."""
    t = text.strip()
    m = re.match(r'^([!~\-+]*)(.*)$', t, re.S)
    pre, rest = m.group(1), m.group(2)
    if rest.startswith('(') and rest.endswith(')'):
        depth = 0
        for k, ch in enumerate(rest):
            if ch == '(':
                depth += 1
            elif ch == ')':
                depth -= 1
                if depth == 0:
                    if k == len(rest) - 1:          # the opening '(' matches the FINAL ')': a pure group
                        return pre + '(' + _reparen_expr(rest[1:-1]) + ')'
                    return text                     # e.g. a cast `(int)x` or `(a).b` -> leave alone
    return text


def _clarify_precedence(s):
    """Apply _reparen_expr to the expression part of a return / if-while-condition / assignment."""
    m = re.match(r'^(\s*return\s+)(.+?)(;\s*)$', s)
    if m:
        return m.group(1) + _reparen_expr(m.group(2)) + m.group(3)
    m = re.match(r'^(\s*(?:\}\s*)?(?:else\s+)?(?:if|while)\s*\()(.+)(\)\s*\{?\s*)$', s)
    if m and m.group(2).count('(') == m.group(2).count(')'):
        return m.group(1) + _reparen_expr(m.group(2)) + m.group(3)
    idx = s.find(' = ')                            # plain assignment (spaces rule out == <= >= += ...)
    if idx >= 0:
        body = s[idx + 3:].rstrip()
        if body.endswith(';') and ' = ' not in body:
            return s[:idx + 3] + _reparen_expr(body[:-1]) + ';'
    return s


def _rewrite_float_mod(s):
    """Java `a % b` where one operand is a float literal -> fmodf(a, b). C++ '%' is integer-only;
    Java's '%' is also float remainder. The float-literal guard keeps integer modulo untouched."""
    def rep(m):
        a, b = m.group(1), m.group(2)
        if _FLOAT_RE.match(a.strip()) or _FLOAT_RE.match(b.strip()):
            return 'fmodf(%s, %s)' % (a, b)
        return m.group(0)
    return _MOD_PAT.sub(rep, s)


_PURE_STR_LIT = re.compile(r'^"(?:[^"\\]|\\.)*"$')


def _to_jcat(expr):
    """If `expr` is a Java string concatenation (top-level '+' with at least one PURE
    string-literal operand), wrap it in the JCat builder; else return None. The pure-literal
    test avoids mis-wrapping numeric `+` like `"abc".length() + 5` (literal-then-method)."""
    e = expr.strip()
    if '"' not in e or '+' not in e:
        return None
    parts = _split_top_plus(e)
    if len(parts) < 2:
        return None
    if not any(_PURE_STR_LIT.match(p.strip()) for p in parts):
        return None
    return '(JCat() + ' + ' + '.join(p.strip() for p in parts) + ')'


def _rewrite_concats(s):
    """Wrap Java string concatenations with the JCat builder, anywhere they appear as a
    value: inside every (...) argument group (per comma-separated arg) and recursively in
    nested groups. Statement-level RHS/return concats are handled by the caller. Literals
    are copied verbatim so embedded '+'/',' inside strings are never touched."""
    out, i, n = [], 0, len(s)
    while i < n:
        c = s[i]
        if c == '"' or c == "'":
            q = c; out.append(c); i += 1
            while i < n:
                out.append(s[i])
                if s[i] == '\\' and i + 1 < n:
                    out.append(s[i + 1]); i += 2; continue
                if s[i] == q:
                    i += 1; break
                i += 1
            continue
        if c == '(':
            close = _balanced_paren(s, i)
            if close == -1:
                out.append(c); i += 1; continue
            inner = _rewrite_concats(s[i + 1:close])          # recurse into nested groups
            new_parts = [(_to_jcat(p) or p) for p in _split_top_commas(inner)]
            out.append('(' + ','.join(new_parts) + ')')
            i = close + 1
            continue
        out.append(c); i += 1
    return ''.join(out)


def _balanced_paren(s, open_idx):
    depth = 0
    for i in range(open_idx, len(s)):
        if s[i] == '(':
            depth += 1
        elif s[i] == ')':
            depth -= 1
            if depth == 0:
                return i
    return -1


def _balanced_brace(s, open_idx):
    depth = 0
    for i in range(open_idx, len(s)):
        if s[i] == '{':
            depth += 1
        elif s[i] == '}':
            depth -= 1
            if depth == 0:
                return i
    return -1


_PTR_CAST_HEAD = re.compile(r'^\(\s*([A-Za-z_]\w*)\s*(\*?)\s*\)')


def _deref_cast_results(s, resolver):
    """`((Type*)EXPR).member` / `((Type)EXPR).member` (Type a reference) -> `...)->member`.
    A cast to a pointer yields a pointer, so the following access must be '->'. Handles a
    balanced EXPR (calls, [index]) -- broader than the old no-paren-operand regex."""
    n = len(s)
    match, st = {}, []
    for i, c in enumerate(s):
        if c == '(':
            st.append(i)
        elif c == ')' and st:
            match[st.pop()] = i
    repl = set()
    for op in match:
        cl = match[op]
        content = s[op + 1:cl]
        m = _PTR_CAST_HEAD.match(content)
        if not m or m.end() >= len(content):           # must be a cast WITH an operand after it
            continue
        is_ptr = bool(m.group(2)) or resolver.is_pointer(ir.JType(m.group(1)))
        if not is_ptr:
            continue
        j = cl + 1
        while j < n and s[j] == ' ':
            j += 1
        if j < n and s[j] == '.' and j + 1 < n and (s[j + 1].isalpha() or s[j + 1] == '_'):
            repl.add(j)
    if not repl:
        return s
    return ''.join('->' if k in repl else ch for k, ch in enumerate(s))


def _array_type(base, dims):
    """JArray nested `dims` deep over `base` (dims=1 -> JArray<base>)."""
    t = base
    for _ in range(dims):
        t = 'JArray<%s >' % t
    return t


def _build_agg(body, dims, base):
    """Build a nested JArrayInit for an N-dim array aggregate. `body` = contents between the
    outermost braces (no enclosing braces); `dims` = remaining dimensions; `base` = element type."""
    parts = [p.strip() for p in _split_top_commas(body)]
    if dims <= 1:
        return ('(JArrayInit<%s >(), %s)' % (base, ', '.join(parts))) if parts else ('JArray<%s >()' % base)
    et = _array_type(base, dims - 1)                    # element of THIS level = (dims-1)-dim array
    if not parts:
        return 'JArray<%s >()' % et
    rows = []
    for p in parts:
        if p.startswith('{') and p.endswith('}'):
            rows.append(_build_agg(p[1:-1], dims - 1, base))   # recurse into the row braces
        else:
            rows.append(p)                             # non-brace row (e.g. NULL) -- leave verbatim
    return '(JArrayInit<%s >(), %s)' % (et, ', '.join(rows))


def _rewrite_array_aggregates(s, resolver):
    """`new T[]...[]{...}` (any dimensionality) -> nested JArrayInit (C++03 has no heap-array init
    list). Object elements box to JObject; the JArrayInit comma-builders convert to nested JArray."""
    while True:
        m = _NEW_ARRAY_AGG.search(s)
        if not m:
            return s
        brace = m.end() - 1                            # the '{'
        close = _balanced_brace(s, brace)
        if close < 0:
            return s
        dims = m.group(2).count('[')                   # number of [] dimension brackets
        # Object[] elements are boxed (JObject) -- a mixed String/char/pointer array (Java autobox).
        # byte[] is an unsigned-char BUFFER (matches types.cpp_type / _array_elem_cpp), NOT signed char.
        if m.group(1) == 'Object':
            base = 'JObject'
        elif m.group(1) == 'byte':
            base = 'unsigned char'
        else:
            base = resolver.cpp_type(ir.JType(m.group(1)), 'local')
        rep = _build_agg(s[brace + 1:close].strip(), dims, base)
        s = s[:m.start()] + rep + s[close + 1:]


_LABEL_DEF = re.compile(r'^(\w+):\s*(.*)$')
_LABEL_JUMP = re.compile(r'\b(break|continue)\s+(\w+)\s*;')
_LOOP_START = re.compile(r'^(for|while|do)\b')


def _loop_close(lines, loop_idx):
    """Index of the line holding the `}` that closes the loop starting at loop_idx."""
    depth, started = 0, False
    for i in range(loop_idx, len(lines)):
        depth += lines[i].count('{') - lines[i].count('}')
        if '{' in lines[i]:
            started = True
        if started and depth <= 0:
            return i
    return None


def _translate_labels(lines):
    """Labeled break/continue -> goto. `break L` jumps past the L-loop; `continue L`
    jumps to a target at the END of the L-loop body, so a `for` runs its increment
    on fall-through (no skipped-increment infinite loop -- the shore-water freeze)."""
    # Track break- vs continue-references SEPARATELY so we emit ONLY the target(s) that are
    # actually jumped to. Emitting an unused `L_cont:`/`L_brk:` triggers C4102 (unreferenced label).
    cont_refs, brk_refs = set(), set()
    for ln in lines:
        for m in _LABEL_JUMP.finditer(ln):
            (cont_refs if m.group(1) == 'continue' else brk_refs).add(m.group(2))
    referenced = cont_refs | brk_refs
    if not referenced:
        return lines

    def_strip, cont_at, brk_at = {}, {}, {}
    for i, ln in enumerate(lines):
        m = _LABEL_DEF.match(ln)
        if not m or m.group(1) not in referenced:
            continue
        name = m.group(1)
        loop_idx = (i if _LOOP_START.match(m.group(2))
                    else i + 1 if i + 1 < len(lines) and _LOOP_START.match(lines[i + 1]) else None)
        if loop_idx is None:
            continue
        close = _loop_close(lines, loop_idx)
        if close is None:
            continue
        def_strip[i] = name
        if name in cont_refs:
            cont_at.setdefault(close, []).append('%s_cont:;' % name)
        if name in brk_refs:
            brk_at.setdefault(close, []).append('%s_brk:;' % name)
    if not def_strip:
        return lines

    def jump(m):
        return 'goto %s_%s;' % (m.group(2), 'brk' if m.group(1) == 'break' else 'cont')
    rewritten = [_LABEL_JUMP.sub(jump, ln) for ln in lines]

    out = []
    for i, ln in enumerate(rewritten):
        if i in def_strip:
            ln = re.sub(r'^\s*%s:\s*' % re.escape(def_strip[i]), '', ln)
            if not ln.strip():
                continue
        for c in cont_at.get(i, []):           # continue-target: end of loop body (before `}`)
            out.append(c)
        out.append(ln)
        for b in brk_at.get(i, []):            # break-target: just after the loop
            out.append(b)
    return out


def _balanced_from(s, i):
    """s[i] must be '('. Return (inner, index_after_close) for the balanced paren group."""
    assert s[i] == '('
    depth, j = 0, i
    while j < len(s):
        if s[j] == '(':
            depth += 1
        elif s[j] == ')':
            depth -= 1
            if depth == 0:
                return s[i + 1:j], j + 1
        j += 1
    return None, len(s)


def _parse_throwaway_new(stmt):
    """If `stmt` (a full translated statement, no trailing ';') is exactly a used-once heap temporary
    `(new T(ctorargs))->method(callargs)` whose result is DISCARDED, return (T, ctorargs, method,
    callargs); else None. Balanced-paren aware so nested calls in the args don't fool it."""
    s = stmt.strip()
    if not s.startswith('(new '):
        return None
    m = re.match(r'\(new\s+([A-Za-z_]\w*)\s*\(', s)
    if not m:
        return None
    typ = m.group(1)
    ctorargs, k = _balanced_from(s, m.end() - 1)          # '(' of the ctor args
    if ctorargs is None or k >= len(s) or s[k] != ')':    # the outer ')' closing `(new ...)`
        return None
    rest = s[k + 1:]
    mm = re.match(r'->\s*([A-Za-z_]\w*)\s*\(', rest)       # ->method(
    if not mm:
        return None
    method = mm.group(1)
    callargs, k2 = _balanced_from(rest, mm.end() - 1)
    if callargs is None or rest[k2:].strip() != '':       # nothing may follow (result is discarded)
        return None
    return typ, ctorargs.strip(), method, callargs.strip()


def _rewrite_leak_temporaries(lines, resolver, ledger, cls):
    """AUTO-FIX the dominant [memleak] pattern: a used-once heap temporary
    `(new T(a))->m(b);` (statement, result discarded) leaks T every call (Java GC'd it; C++ does not
    -- world-gen `(new WorldGenX())->generate(...)` leaked on every populate). Rewrite to a scoped
    STACK object `{ T _j2xleakN(a); _j2xleakN.m(b); }` that auto-destructs. Only for known user
    classes (stack-constructible). Matches the hand [memleak] fixes in ChunkProviderGenerate.

    SAFETY: never stackify an object that ESCAPES the call -- a Thread/Runnable started via
    `.start()`/`.run()` runs asynchronously and MUST outlive the statement (stack-allocating it is a
    use-after-free, not a leak fix). Those are left as heap (a benign leak beats a crash)."""
    out, n = [], 0
    for ln in lines:
        stripped = ln.strip()
        if stripped.endswith(';') and stripped.startswith('(new '):
            parsed = _parse_throwaway_new(stripped[:-1])
            if (parsed and parsed[0] in resolver.known
                    and parsed[0] not in resolver.thread_like       # async object: must outlive the call
                    and parsed[2] not in ('start', 'run')):         # thread-launch methods -> escapes
                typ, ctorargs, method, callargs = parsed
                indent = ln[:len(ln) - len(ln.lstrip())]
                tmp = '_j2xleak%d' % n
                n += 1
                decl = '%s %s(%s);' % (typ, tmp, ctorargs) if ctorargs else '%s %s;' % (typ, tmp)
                out.append('%s{ %s %s.%s(%s); }' % (indent, decl, tmp, method, callargs))
                if ledger is not None:
                    ledger.add(cls.name + '.cpp', Category.RULE,
                               'leak fixed: throwaway `(new %s(...))->%s(...)` -> scoped stack object '
                               '(Java GC temp leaked every call)' % (typ, method),
                               severity=Severity.MED, source='emit', rule='leak-temp-stackified')
                continue
        out.append(ln)
    return out


# Java reflection with NO C++ equivalent -- the runtime stubs return NULL, so a silent translation is
# a latent NULL-deref/hang (the TileEntitySign sign-placement deadlock: newInstance() -> NULL tile
# entity -> null write on the 360 -> freeze). These MUST be surfaced, never silently stubbed.
_REFLECTION_CALL = re.compile(r'(?<![\w])(newInstance|forName|getConstructor|getDeclaredConstructor)\s*\(')


def _flag_reflection(lines, ledger, cls):
    """Prepend a greppable `J2X-REVIEW[reflection-unsupported]` tag above any statement using Java
    reflection (newInstance/forName/getConstructor). The stubs return NULL, so a human must supply a
    hand-written factory. `grep -rn J2X-REVIEW` after a full conversion lists every one."""
    out = []
    for ln in lines:
        code = ln.split('//', 1)[0]
        m = _REFLECTION_CALL.search(code)
        if m and 'J2X-REVIEW' not in ln:
            indent = ln[:len(ln) - len(ln.lstrip())]
            out.append('%s// J2X-REVIEW[reflection-unsupported]: %s() has no C++ equivalent -- the '
                       'runtime stub returns NULL. Replace with a hand-written factory (a class-keyed '
                       'switch / registry), then null-check the result.' % (indent, m.group(1)))
            if ledger is not None:
                ledger.add(cls.name + '.cpp', Category.RISK,
                           'reflection %s() has no C++ equivalent -- stub returns NULL; needs a '
                           'hand-written factory (J2X-REVIEW[reflection-unsupported])' % m.group(1),
                           severity=Severity.HIGH, source='emit', rule='reflection-unsupported')
        out.append(ln)
    return out


_CATCH_PTR = re.compile(r'\bcatch\s*\(\s*([\w:]+)\s*\*\s*([A-Za-z_]\w*)\s*\)')
_CATCH_VAL = re.compile(r'\bcatch\s*\(\s*([\w:]+)\s+([A-Za-z_]\w*)\s*\)')


def _drop_unused_catch_vars(lines):
    """`catch (Type* e) { ... }` where `e` is never referenced -> `catch (Type*)`. The decompiled
    Java is full of empty/rethrow-free handlers whose exception var is dead -> C4101. A name that
    occurs exactly once in the whole body is only the catch declaration itself, so it is safe to drop
    (decompiled locals are uniquely numbered per method, so no shadowing ambiguity)."""
    counts = {}
    for tok in re.findall(r'[A-Za-z_]\w*', '\n'.join(lines)):
        counts[tok] = counts.get(tok, 0) + 1
    def fix(ln):
        ln = _CATCH_PTR.sub(lambda m: 'catch (%s*)' % m.group(1) if counts.get(m.group(2), 0) <= 1 else m.group(0), ln)
        ln = _CATCH_VAL.sub(lambda m: 'catch (%s)' % m.group(1) if counts.get(m.group(2), 0) <= 1 else m.group(0), ln)
        return ln
    return [fix(ln) for ln in lines]


def _category(jtype):
    if jtype is None:
        return None
    n = jtype.name
    if n in _LIST_TYPES:
        return 'list'
    if n in _MAP_TYPES:
        return 'map'
    # sets now map to JSet<T> (a std::set subclass with the native Java API: add/addAll/removeAll/
    # contains/isEmpty return the Java types), so DON'T remap set method calls -- let them bind to
    # JSet's own methods (remapping add->insert would break `if(set.add(v))`, since insert returns a pair).
    if n == 'String':
        return 'string'
    return None


def _rewrite_new_containers(s, resolver):
    """Rewrite every `new <Container>(...)` in `s` to a by-value STL temporary, balanced-paren aware.
    Empty args -> default construct `CppType()`; a single collection arg (Java copy/range ctor) is
    PRESERVED as `CppType(arg)` so the elements are actually copied (dropping it silently emptied the
    container -- the recipe/texture-pack data-loss bug). Non-container `new` is left untouched."""
    out, i, n = [], 0, len(s)
    pat = re.compile(r'\bnew\s+([A-Z]\w*)(<[^;{}]*?>)?\s*\(')
    while i < n:
        m = pat.search(s, i)
        if not m:
            out.append(s[i:]); break
        jt = javaparse.parse_type(m.group(1) + (m.group(2) or ''))
        if jt.name not in typemap.CONTAINERS:
            out.append(s[i:m.end()]); i = m.end(); continue      # not a container -> leave `new X(`
        popen = m.end() - 1                                      # index of the '('
        depth, j = 0, popen
        while j < n:                                            # balanced close paren
            if s[j] == '(':
                depth += 1
            elif s[j] == ')':
                depth -= 1
                if depth == 0:
                    break
            j += 1
        inner = s[popen + 1:j].strip()
        cpp = resolver.cpp_type(jt, 'local')
        out.append(s[i:m.start()])
        out.append('%s(%s)' % (cpp, inner) if inner else '%s()' % cpp)
        i = j + 1
    return ''.join(out)


def _container_call(cat, name, args_str):
    """STL replacement for a container method call `<recv>.name(args)` -- returns the text that
    REPLACES `.name(args)` (the receiver is already emitted), or None to leave it alone. Used by
    the chain walker so container methods map on FOREIGN/chained receivers too, not just simple
    in-scope names. Containers are held by value, so the result attaches with '.'/[]."""
    if cat is None:
        return None
    args = _split_top_commas(args_str) if args_str.strip() else []
    rename = _METHOD_RENAME.get((cat, name))
    if rename:
        return '.%s(%s)' % (rename, args_str)               # add->push_back, remove->erase, ...
    if name == _INDEX_METHOD.get(cat) and args:
        return '[%s]' % args[0]                             # list/map get / String charAt -> [i]
    if cat == 'map' and name == 'put' and len(args) == 2:
        return '[%s] = %s' % (args[0], args[1])             # map.put(k, v) -> [k] = v
    return None


def _balanced_call_sub(s, recv, meth, fn):
    """Rewrite every `recv.meth(args)` via fn(arg_list) -> replacement (None = leave)."""
    token = '%s.%s(' % (recv, meth)
    while True:
        idx = s.find(token)
        if idx < 0:
            return s
        op = idx + len(token) - 1
        close = _balanced_paren(s, op)
        if close < 0:
            return s
        rep = fn(_split_top_commas(s[op + 1:close]))
        if rep is None:
            return s                                  # unexpected arity -> leave (and stop)
        s = s[:idx] + rep + s[close + 1:]


def _translate_calls(s, scope):
    """Map list/map/set/String methods to STL, using each receiver's resolved type."""
    # 1. simple method renames (args preserved)
    def repl(m):
        recv, meth = m.group(1), m.group(2)
        new = _METHOD_RENAME.get((_category(scope.get(recv)), meth))
        return '%s.%s(' % (recv, new) if new else m.group(0)
    s = _CALL.sub(repl, s)
    # 2. per-receiver rewrites that need the args
    for recv, jt in scope.items():
        cat = _category(jt)
        if not cat:
            continue
        idx_m = _INDEX_METHOD.get(cat)
        if idx_m:                                     # list/map .get(k) / String.charAt(i) -> recv[k]
            s = _balanced_call_sub(s, recv, idx_m,
                                   lambda a, r=recv: ('%s[%s]' % (r, a[0])) if a else None)
        if cat == 'map':                              # map.put(k, v) -> recv[k] = v
            s = _balanced_call_sub(s, recv, 'put',
                                   lambda a, r=recv: ('%s[%s] = %s' % (r, a[0], a[1])) if len(a) == 2 else None)
        if cat == 'string':
            s = _balanced_call_sub(s, recv, 'equals',
                                   lambda a, r=recv: ('(%s == %s)' % (r, a[0])) if a else None)
            s = _balanced_call_sub(s, recv, 'substring',   # Java end-exclusive -> substr(pos, len)
                                   lambda a, r=recv: ('%s.substr(%s, (%s) - (%s))' % (r, a[0], a[1], a[0])) if len(a) == 2
                                   else ('%s.substr(%s)' % (r, a[0])) if len(a) == 1 else None)
    return s


def inner_lines(body):
    """Statement lines between the outermost braces of a parsed method body."""
    text = '\n'.join(t for _ln, t in body)
    i = text.find('{')
    j = text.rfind('}')
    if i != -1 and j != -1 and j > i:
        text = text[i + 1:j]
    return [ln.rstrip() for ln in text.split('\n')]


def _decl_base(type_str):
    """The bare type name (drop generics, arrays) for keyword checking."""
    return type_str.split('[')[0].split('<')[0].strip()


def build_scope(cls, method):
    """name -> JType for params, the class's own + inherited fields, and locals declared in the body."""
    scope = {}
    for f in getattr(cls, 'inherited_fields', ()):   # ancestors first; own fields override on shadow
        scope[f.name] = f.jtype
    for f in cls.fields:
        scope[f.name] = f.jtype
    for p in method.params:
        scope[p.name] = p.jtype
    for _ln, text in method.body:
        m = _DECL.match(text.strip())
        if m and _decl_base(m.group(1)) not in _KEYWORDS and m.group(2) not in _KEYWORDS:
            try:
                scope[m.group(2)] = javaparse.parse_type(m.group(1))
            except Exception:                       # noqa: BLE001 - never let a parse hiccup abort emit
                pass
    return scope


def _local_names(method):
    """Names of locals DECLARED in the body (not params/fields)."""
    names = set()
    for _ln, text in method.body:
        m = _DECL.match(text.strip())
        if m and _decl_base(m.group(1)) not in _KEYWORDS and m.group(2) not in _KEYWORDS:
            names.add(m.group(2))
    return names


def _pointer_locals(method, scope, resolver):
    """Value-type LOCALS used as nullable references must be C++ pointers -- a C++ value
    type cannot hold null, so a Java local assigned/compared null (or `new`-d) is really a
    reference. Sound and signature-safe: only locals (params would change the signature)."""
    forced = set()
    locals_ = _local_names(method)
    param_names = set(p.name for p in method.params)
    text = '\n'.join(t for _ln, t in method.body)
    for name in locals_:
        if name in param_names:
            continue
        jt = scope.get(name)
        if (jt is None or jt.is_array or resolver.is_pointer(jt)
                or jt.name in typemap.CONTAINERS or jt.name in typemap.VALUE_STDLIB
                or jt.name == 'Object'):
            continue            # already a pointer/array, a value container, String, or JObject
            # (String/JObject hold null themselves and absorb `new T()` via their ctors)
        nm = re.escape(name)
        if (re.search(r'\b%s\s*=\s*(?:null|NULL)\b' % nm, text)
                or re.search(r'\b%s\s*(?:==|!=)\s*(?:null|NULL)\b' % nm, text)
                or re.search(r'\b(?:null|NULL)\s*(?:==|!=)\s*%s\b' % nm, text)
                or re.search(r'\b%s\s*=\s*new\b' % nm, text)):
            forced.add(name)
    return forced


# Java raw Map/HashMap types -> our raw map is std::map<JObject, JObject>, so indexing one yields a
# JObject value (see typemap.RAW_CONTAINER_DEFAULT). Used to detect JObject operands for casts/instanceof.
_RAW_MAPS = {'HashMap', 'Map', 'TreeMap', 'LinkedHashMap', 'Hashtable', 'SortedMap', 'MCHashTable'}
# boxed wrappers whose cast on a JObject is a no-op (JObject's operator int()/char() + .intValue()/
# .charValue() unbox at the point of use), so `(Integer)obj`/`(Character)obj` -> just `(obj)`.
_BOXED_WRAPPERS = {'Integer', 'Character', 'Boolean', 'Byte', 'Short', 'Long', 'Double', 'Float'}
_OPERAND_ROOT = re.compile(r'^([A-Za-z_]\w*)\s*((?:\[[^\]]*\])*)$')


def _is_balanced(s):
    depth = 0
    for c in s:
        if c in '([':
            depth += 1
        elif c in ')]':
            depth -= 1
            if depth < 0:
                return False
    return depth == 0


def _expr_object_kind(expr, scope):
    """'object' if `expr` is a JObject-typed VALUE -- a bare Object variable, a fully-indexed Object
    array element (var[i] / var[i][j]), or a raw-Map index (map[k], value is JObject). Else None.
    Conservative: only identifier-rooted exprs with simple [idx] groups (no nested brackets)."""
    o = expr.strip()
    o = re.sub(r'\s*(?:\+\+|--)\s*$', '', o)                 # x++ / x-- -> x
    while o.startswith('(') and o.endswith(')') and _is_balanced(o[1:-1]):
        o = o[1:-1].strip()                                  # strip balanced wrapping parens
    m = _OPERAND_ROOT.match(o)
    if not m:
        return None
    base, idx = m.group(1), m.group(2)
    jt = scope.get(base)
    if jt is None:
        return None
    ndim = idx.count('[')
    if jt.is_array:
        if (jt.array_dims - ndim) == 0 and jt.name == 'Object':
            return 'object'                                 # Object[..] fully indexed -> element JObject
        return None
    if ndim == 1 and jt.name in _RAW_MAPS:
        return 'object'                                     # raw map value -> JObject
    if ndim == 0 and jt.name == 'Object':
        return 'object'                                     # bare Object variable
    return None


def _read_primary(s, i):
    """Read a primary operand expression starting at s[i] (just past a cast's ')'). Handles a
    parenthesized group, or an identifier with trailing ->/. member access, [idx] groups, and ++/--.
    Returns (expr, end_index); expr is '' if nothing readable."""
    n = len(s)
    while i < n and s[i].isspace():
        i += 1
    start = i
    if i < n and s[i] == '(':                               # balanced parenthesized operand
        depth = 0
        while i < n:
            if s[i] == '(':
                depth += 1
            elif s[i] == ')':
                depth -= 1
                if depth == 0:
                    i += 1
                    break
            i += 1
        return s[start:i], i
    if i < n and (s[i].isalpha() or s[i] == '_'):
        while i < n and (s[i].isalnum() or s[i] == '_'):
            i += 1
        while i < n:
            if s[i] == '[':                                 # [ balanced ] index
                depth = 0
                while i < n:
                    if s[i] == '[':
                        depth += 1
                    elif s[i] == ']':
                        depth -= 1
                        if depth == 0:
                            i += 1
                            break
                    i += 1
            elif s[i:i + 2] == '->' or s[i] == '.':         # member access (NOT a call -- stop at '(')
                i += 2 if s[i:i + 2] == '->' else 1
                while i < n and s[i].isspace():
                    i += 1
                if i < n and s[i] == '(':                    # `.method(` -> not part of a plain operand
                    break
                while i < n and (s[i].isalnum() or s[i] == '_'):
                    i += 1
            else:
                break
        if s[i:i + 2] in ('++', '--'):
            i += 2
        return s[start:i], i
    return '', start


_CAST_HEAD = re.compile(r'\(\s*([A-Za-z_]\w*)\s*(\[\s*\])?\s*\)')


def _rewrite_jobject_casts(s, scope):
    """Lower casts whose operand is a JObject (Object var / Object[] element / raw-map value), which
    the generic _cast pass + JObject's conversion operators can't always handle:
      (String)obj      -> (obj).str()            (avoids the operator JavaString()/T*-char* ambiguity)
      (String[])obj    -> (obj).strArray()       ((String[]) is not a _CAST-handled spelling)
      (String[])arr    -> arr                     (redundant array cast on an already-array expr)
      (Integer|Character|...)obj -> (obj)         (drop boxed wrapper; operator int()/char() + the
                                                   trailing .intValue()/.charValue() unbox at use)
    Other casts ((SomeClass)obj -> (SomeClass*)obj, (int)/(char) -> via operators) are left to _cast."""
    out = []
    i, n = 0, len(s)
    while i < n:
        m = _CAST_HEAD.match(s, i)
        if not m:
            out.append(s[i])
            i += 1
            continue
        typ, is_arr = m.group(1), bool(m.group(2))
        operand, end = _read_primary(s, m.end())
        if not operand:
            out.append(s[i])
            i += 1
            continue
        kind = _expr_object_kind(operand, scope)
        rewritten = None
        if is_arr:
            if typ == 'String':
                rewritten = ('(%s).strArray()' % operand) if kind else operand
        elif kind:
            if typ == 'String':
                rewritten = '(%s).str()' % operand
            elif typ in _BOXED_WRAPPERS:
                rewritten = '(%s)' % operand
        if rewritten is None:
            out.append(s[i])
            i += 1
        else:
            out.append(_rewrite_jobject_casts(rewritten, scope))   # recurse (nested casts in operand)
            i = end
    return ''.join(out)


_CHAIN_ROOT = re.compile(r'[A-Za-z_]\w*::s_\w+|[A-Za-z_]\w*')
_CHAIN_LINK = re.compile(r'\s*(->|\.)\s*([A-Za-z_]\w*)')
_NEW_ROOT = re.compile(r'new\s+([A-Za-z_]\w*)\s*\(')   # `new Type(` scalar ctor (not `new T[`)
_PAREN_NEW_ROOT = re.compile(r'\(\s*new\s+([A-Za-z_]\w*)\s*\(')   # `(new Type(` already-parenthesized


def _balanced_bracket(s, open_idx):
    depth = 0
    for i in range(open_idx, len(s)):
        if s[i] == '[':
            depth += 1
        elif s[i] == ']':
            depth -= 1
            if depth == 0:
                return i
    return -1


def _chain_root(tok, scope, resolver, current):
    """(JType, is_pointer) for a chain-root token, or None if it isn't a typed root. Roots:
    `this`, a `Class::s_field` static field, a bare emitted static `s_field` (own/inherited),
    or an in-scope variable/field."""
    if tok == 'this':
        return (ir.JType(current), True)
    sm = re.match(r'^([A-Za-z_]\w*)::s_(\w+)$', tok)
    if sm:
        jt = resolver.member_field_type(sm.group(1), sm.group(2))
        return (jt, resolver.is_pointer(jt)) if jt is not None else None
    if tok.startswith('s_'):                          # bare emitted static (s_ prefix)
        jt = resolver.member_field_type(current, tok[2:])
        if jt is not None:
            return (jt, resolver.is_pointer(jt))
    jt = scope.get(tok)
    return (jt, resolver.is_pointer(jt)) if jt is not None else None


def _consume_index(s, i, out, cur_jt, is_ptr, resolver, scope, current):
    """While cur_jt is an array and a balanced [index] follows, consume it and step into the
    element type (so `arr[i].member` types `arr[i]` as the element). The index EXPRESSION is
    itself chain-resolved (it may contain its own pointer chains, e.g. arr[base->idx])."""
    while cur_jt is not None and cur_jt.is_array and i < len(s) and s[i] == '[':
        close = _balanced_bracket(s, i)
        if close < 0:
            break
        out.append('[' + _resolve_chains(s[i + 1:close], scope, resolver, current) + ']')
        i = close + 1
        cur_jt = cur_jt.element()
        is_ptr = resolver.is_pointer(cur_jt)
    return i, cur_jt, is_ptr


def _resolve_chains(s, scope, resolver, current):
    """Deep expression-type inference for chained member access: walk a chain from a typed
    root (this / scope var / static field), tracking each sub-expression's JType via the
    member environment (following field/method links AND [index] into array elements), and
    upgrade a '.' to '->' wherever the receiver-so-far is a pointer -- to ANY depth (e.g.
    this->mc->options->keyBindInventory->keyCode, or var5->mainInventory[i]->writeToNBT()).
    Safe: only ever turns '.' into '->' when the receiver is PROVEN a pointer, never the
    reverse, and stops when a link's type is unknown. Recurses into call args."""
    out, n = [], len(s)

    def walk_links(i, cur_jt, is_ptr):
        """Consume `.`/`->` field/method links from i, tracking type; returns the new i."""
        while cur_jt is not None:
            lm = _CHAIN_LINK.match(s, i)
            if not lm:
                break
            sep, name, after = lm.group(1), lm.group(2), lm.end()
            k = after
            while k < n and s[k] == ' ':
                k += 1
            is_call = k < n and s[k] == '('
            args, seg_end = None, after
            if is_call:
                close = _balanced_paren(s, k)
                if close < 0:
                    break
                args = _resolve_chains(s[k + 1:close], scope, resolver, current)
                seg_end = close + 1
                crepl = _container_call(_category(cur_jt), name, args)
                if crepl is not None:                 # STL container method on a chained receiver
                    out.append(crepl)
                    return seg_end                    # result type (raw element) not tracked
                name = resolver.renamed_method(cur_jt.name, name)   # field/method collision -> name_
            out.append(('->' if is_ptr else sep) + name)   # upgrade only when proven pointer
            if is_call:
                out.append('(' + args + ')')
            i = seg_end
            cur_jt = (resolver.member_method_type(cur_jt.name, name) if is_call
                      else resolver.member_field_type(cur_jt.name, name))
            if cur_jt is None:
                break
            is_ptr = resolver.is_pointer(cur_jt)
            i, cur_jt, is_ptr = _consume_index(s, i, out, cur_jt, is_ptr, resolver, scope, current)
        return i

    i = 0
    while i < n:
        prev = out[-1][-1] if (out and out[-1]) else ''
        at_boundary = (prev == '') or (not prev.isalnum() and prev not in '_>.:')
        # a `new Type(args)` root (reference type) -> wrap `(new Type(args))` and walk as a
        # pointer (fluent builders: new BlockX(...).setHardness(a).setResistance(b)...)
        # `(new Type(args))` already-parenthesized root -> walk the fluent chain as a pointer
        pn = _PAREN_NEW_ROOT.match(s, i) if at_boundary else None
        if pn and pn.group(1) not in typemap.CONTAINERS and not resolver.is_value(ir.JType(pn.group(1))):
            inner = _balanced_paren(s, pn.end() - 1)
            j = inner + 1
            while inner >= 0 and j < n and s[j] == ' ':
                j += 1
            if inner >= 0 and j < n and s[j] == ')':           # the wrapping ')'
                args = _resolve_chains(s[pn.end():inner], scope, resolver, current)
                out.append('(new %s(%s))' % (pn.group(1), args))
                cur_jt, is_ptr = ir.JType(pn.group(1)), True
                k, cur_jt, is_ptr = _consume_index(s, j + 1, out, cur_jt, is_ptr, resolver, scope, current)
                i = walk_links(k, cur_jt, is_ptr)
                continue
        nw = _NEW_ROOT.match(s, i) if at_boundary else None
        if nw and nw.group(1) not in typemap.CONTAINERS and not resolver.is_value(ir.JType(nw.group(1))):
            close = _balanced_paren(s, nw.end() - 1)
            j = close + 1
            while close >= 0 and j < n and s[j] == ' ':
                j += 1
            # only engage when a chain link actually follows (fluent builder); leave a standalone
            # or already-parenthesized `new X()` to _resolve_call_chains / verbatim
            if close >= 0 and j < n and (s[j] == '.' or s[j:j + 2] == '->'):
                args = _resolve_chains(s[nw.end():close], scope, resolver, current)
                out.append('(new %s(%s))' % (nw.group(1), args))
                cur_jt, is_ptr = ir.JType(nw.group(1)), True
                k, cur_jt, is_ptr = _consume_index(s, close + 1, out, cur_jt, is_ptr, resolver, scope, current)
                i = walk_links(k, cur_jt, is_ptr)
                continue
        m = _CHAIN_ROOT.match(s, i) if at_boundary else None
        if not m:
            out.append(s[i]); i += 1; continue
        out.append(m.group(0))
        rt = _chain_root(m.group(0), scope, resolver, current)
        i = m.end()
        if rt is None:
            continue
        cur_jt, is_ptr = rt
        i, cur_jt, is_ptr = _consume_index(s, i, out, cur_jt, is_ptr, resolver, scope, current)
        i = walk_links(i, cur_jt, is_ptr)
    return ''.join(out)


_CALL_HEAD = re.compile(r'(\bnew\s+(\w+)|(\w+)(->|::)(\w+))\s*\(')


def _resolve_call_chains(s, scope, resolver, current):
    """Method/constructor RESULT chains: a call that yields a pointer, then `.member`.
        new Type(...).m          -> (new Type(...))->m     (new always yields a pointer)
        recv->method(...).m      -> recv->method(...)->m   (method returns a pointer)
        Class::method(...).m     -> Class::method(...)->m   (static method returns a pointer)
    Receiver/return types come from the member environment; conservative (only when known
    to be a pointer). Re-scans after each rewrite since indices shift."""
    for _ in range(8):
        changed = False
        for m in _CALL_HEAD.finditer(s):
            op = m.end() - 1                          # index of the '(' (pattern ends with it)
            close = _balanced_paren(s, op)
            if close < 0:
                continue
            j = close + 1
            while j < len(s) and s[j] == ' ':
                j += 1
            # the member-access dot may be directly after the call, or after a wrapping ')'
            #   (e.g. the source already wrote `(new X(...)).start()`). BUT a ')' here is only a
            #   genuine wrapper if the call head is itself preceded by the matching '(' -- otherwise
            #   the ')' closes an OUTER call (this call is its last arg) and the '.member' belongs to
            #   that outer result, not this one (e.g. outer(inner(x)).size()).
            extra_close = j < len(s) and s[j] == ')'
            if extra_close:
                p = m.start() - 1
                while p >= 0 and s[p] == ' ':
                    p -= 1
                if p < 0 or s[p] != '(':
                    extra_close = False
            dot = j + 1 if extra_close else j
            while dot < len(s) and s[dot] == ' ':
                dot += 1
            if dot >= len(s) or s[dot] != '.':
                continue                              # not a result-member access
            if m.group(2) is not None:               # new Type(...) always yields a pointer
                ptr = True
            else:
                recv, sep, meth = m.group(3), m.group(4), m.group(5)
                owner = (recv if sep == '::' else
                         current if recv == 'this' else
                         (scope[recv].name if recv in scope else None))
                if owner is None:
                    continue
                rt = resolver.member_method_type(owner, meth)
                ptr = rt is not None and resolver.is_pointer(rt)
            if not ptr:
                continue
            if m.group(2) is not None and not extra_close:
                s = s[:m.start()] + '(' + s[m.start():close + 1] + ')->' + s[dot + 1:]
            else:                                     # already parenthesized, or a method result
                head_end = j + 1 if extra_close else close + 1
                s = s[:head_end] + '->' + s[dot + 1:]
            changed = True
            break
        if not changed:
            break
    return s


def _translate_line(s, ptr_names, static_set, enum_set, resolver, scope, parent, current,
                    own_statics, ptr_locals):
    # 0. `synchronized(lock) {` -> keep the block, drop the JVM monitor -- BUT tag it. The port is
    #    NOT fully single-threaded (network reader/writer threads, the mesher pool), so a dropped
    #    monitor on cross-thread state is a race (the hand-fixed sendQueueLock reader/writer race).
    #    J2X-REVIEW[synchronized-dropped] makes every one greppable so a human confirms it's not shared.
    def _sync(mm):
        return '/* J2X-REVIEW[synchronized-dropped]: was synchronized(%s) -- add a lock if this state '\
               'is touched by another thread (network/pool) */ ' % mm.group(1).strip()
    s = re.sub(r'\bsynchronized\s*\(([^){]*)\)\s*', _sync, s)
    # 0a2. `finally {` -> `catch (...) {} {`: C++ has no `finally`. The catch-all makes the `try` valid
    #      whether or not it already had a `catch`, and the finally body becomes a trailing scope block
    #      that runs on the normal path (guest exceptions are disabled on the 360, so this is faithful).
    s = re.sub(r'\bfinally\s*\{', 'catch (...) {} {', s)
    # 0e. Java Iterator idiom -> typed helpers returning Iterator* (JavaRT.h): `coll.iterator()` and
    #     `map.values()/keySet().iterator()`. Run BEFORE the container/chain passes (the `.values()/
    #     .iterator()` calls must still be intact). RECV is a field/index chain, not a method call.
    _iter_recv = r'([A-Za-z_]\w*(?:(?:->|\.)\w+(?!\s*\()|\[[^\]]*\])*)'
    s = re.sub(_iter_recv + r'\.values\(\)\s*\.\s*iterator\(\)', r'j2xValues(\1)', s)
    s = re.sub(_iter_recv + r'\.keySet\(\)\s*\.\s*iterator\(\)', r'j2xKeys(\1)', s)
    s = re.sub(_iter_recv + r'\.iterator\(\)', r'j2xIter(\1)', s)
    # 0g. Java method named like a C++ KEYWORD (File.delete(), StringBuilder.delete(), ...) -> `name_`
    #     at the call site (`x->delete()` is the delete-operator = syntax error). Runtime stubs and the
    #     user-class decls provide the `name_` form (build_method_renames + the File/StringBuilder stubs).
    s = re.sub(r'(->|\.)\s*(delete|register|and|or|xor|not|operator|template|typename|namespace|using|friend|explicit|mutable)\s*\(',
               r'\1\2_(', s)
    # 0h. a call passing a DROPPED marker as its only arg -- `new X((Empty2)null)` -- was the synthetic
    #     bridge-ctor call; the bridge ctor + marker class are gone, so strip the arg -> `new X()`
    #     (invokes the real no-arg ctor). Marker set comes from pipeline.drop_synthetic_artifacts.
    if getattr(resolver, 'markers', None):
        _mk = '|'.join(re.escape(m) for m in resolver.markers)
        s = re.sub(r'\(\s*\(\s*(?:%s)\s*\)\s*null\s*\)' % _mk, '()', s)
    # 0b. Java numeric-literal suffixes -> valid C++03: `0.0D`->`0.0`, `5D`->`5.0`, `7L`->`7LL`
    #     (D/d is invalid in MSVC/XDK; L must be 64-bit; F stays). Outside string/char literals.
    s = _fix_numeric_suffixes(s)
    # 0c. Unicode char/string escapes -> code-page-safe forms (no C4566): '￿' -> 0xffff,
    #     the ⌂ font glyph -> its CP437 byte. Operates ON literals, so before identifier rewrites.
    s = _fix_unicode_literals(s)
    # 0f. for-init ARRAY declaration: `for(int[] var3 = ...` -- the line-anchored _DECL below misses
    #     it, and a raw `int[] var` is invalid C++ (MSVC parses `[...]` as a lambda -> C3493 cascade).
    #     Convert the array TYPE in a for-init to JArray<...>.
    def _forinit_arr(mm):
        base = _decl_base(mm.group(1))
        if base in _KEYWORDS:
            return mm.group(0)
        return 'for (%s %s ' % (resolver.cpp_type(javaparse.parse_type(mm.group(1)), 'local'), mm.group(2))
    s = re.sub(r'\bfor\s*\(\s*(?:final\s+)?([A-Za-z_][\w.]*(?:<[^>]*>)?(?:\s*\[\s*\])+)\s+([A-Za-z_]\w*)\s+',
               _forinit_arr, s)
    # 1. local declaration: rewrite the leading Java type to its C++ spelling. A value-type
    #    local inferred to be a nullable reference (ptr_locals) gets a trailing '*'.
    m = _DECL.match(s)
    if m and _decl_base(m.group(1)) not in _KEYWORDS and m.group(2) not in _KEYWORDS:
        cpp = resolver.cpp_type(javaparse.parse_type(m.group(1)), 'local')
        if m.group(2) in ptr_locals and not cpp.endswith('*'):
            cpp += '*'
        s = cpp + s[m.end(1):]
        # a value-container local initialized with EMPTY-arg `new ArrayList()` -> just
        # default-construct (Java collections are by-value std::vector/map/set; dropping the raw
        # `new ArrayList()` avoids an element-type mismatch with the typed LHS). A NON-empty
        # `new ArrayList(src)` (collection-copy ctor) is preserved by 1b below.
        cm = re.match(r'^(.*\b%s)\s*=\s*new\s+([A-Z]\w*)(?:<[^;{}]*>)?\s*\(\s*\)\s*;\s*$'
                      % re.escape(m.group(2)), s)
        if cm and javaparse.parse_type(cm.group(2)).name in typemap.CONTAINERS:
            s = cm.group(1) + ';'
    # 1b. any other `new <Container>(...)` -> a value temporary (collections are held by value).
    #     A single collection argument (`new ArrayList(src)`, `new TreeSet(map.keySet())`) is the
    #     Java copy/range ctor -> PRESERVE it as `CppType(src)` (a copy) rather than silently
    #     dropping it to an empty container. Balanced-paren aware so nested calls in the arg
    #     (`map.keySet()`) don't leave a dangling `)`.
    s = _rewrite_new_containers(s, resolver)
    # 1c. leftover `boolean` type keyword not caught as a leading decl (for-init, multi-decl) ->
    #     bool. Java-only word, so a literal-safe global map is safe (won't touch "boolean" text).
    s = sub_outside_literals(re.compile(r'\bboolean\b'), 'bool', s)
    # 1d. a method called directly on a string literal: "abc".indexOf(x) -> the literal is a
    #     const char[] in C++ (no methods); wrap it so the Java String API resolves.
    s = re.sub(r'("(?:\\.|[^"\\])*")(\s*\.\s*[A-Za-z_]\w*\s*\()', r'JavaString(\1)\2', s)
    # 2. this.  -> this->
    s = re.sub(r'\bthis\.', 'this->', s)
    # 3. <pointerName>.  -> <pointerName>->
    for name in ptr_names:
        s = re.sub(r'\b' + re.escape(name) + r'\.', name + '->', s)
    # 3b. index-result is a pointer: <refArray>[i].member -> <refArray>[i]->member
    s = re.sub(r'\b(\w+)(\[[^\]]*\])\.',
               lambda mm: ('%s%s->' % (mm.group(1), mm.group(2)))
               if _is_nullable_array(mm.group(1), scope, resolver) else mm.group(0), s)
    # 3c. getClass() -> j2xClassOf(typeid(...)): a STABLE Class* (same registry singleton as the
    #     `X.class` literal, so `a->getClass() == B.class` is correct pointer identity, and it can be
    #     stored/passed as Class* without dangling). `recv->getClass()` -> typeid(*(recv)) (dynamic
    #     type via the vtable); `recv.getClass()` (value) -> typeid(recv); bare -> typeid(*this).
    s = re.sub(r'([A-Za-z_]\w*(?:(?:->|\.)\w+|\[[^\]]*\])*)\s*->\s*getClass\s*\(\s*\)',
               r'j2xClassOf(typeid(*(\1)))', s)
    s = re.sub(r'([A-Za-z_]\w*(?:(?:->|\.)\w+|\[[^\]]*\])*)\s*\.\s*getClass\s*\(\s*\)',
               r'j2xClassOf(typeid(\1))', s)
    s = re.sub(r'(?<![\w.>])getClass\s*\(\s*\)', 'j2xClassOf(typeid(*this))', s)
    # 4. Math.fn -> C math (sqrtf, sinf, ...)
    s = _MATH.sub(lambda mm: typemap.MATH_API.get(mm.group(1), mm.group(0)), s)
    # 5. KnownClass.member / Enum.CONST -> :: static access
    def _static(mm):
        recv, mem, paren = mm.group(1), mm.group(2), mm.group(3)
        if recv not in static_set:
            return mm.group(0)
        if mem == 'class' and not paren:             # Java class literal X.class -> STABLE Class*
            return 'j2xClassOf(typeid(%s))' % recv   # program-lifetime singleton (never a dangling temp)
        if paren:                                  # static method call
            return '%s::%s%s' % (recv, resolver.renamed_method(recv, mem), paren)
        if recv in enum_set:                       # enum constant (no s_ prefix)
            return '%s::%s' % (recv, mem)
        return '%s::%s' % (recv, naming.static_field_name(mem))   # static field
    s = _MEMBER.sub(_static, s)
    # 5a2. j2xClassOf(typeid(...)) is a Class* (from `.class` / getClass above); a member access
    #      immediately after it must use `->`, not `.` (e.g. `X.class.getResource()` ->
    #      `j2xClassOf(typeid(X))->getResource()`). Balanced over ONE nested paren (typeid(*(recv))).
    s = re.sub(r'(j2xClassOf\(typeid\((?:[^()]|\([^()]*\))*\)\))\s*\.', r'\1->', s)
    # 5b. bare reference to one of THIS class's static fields -> s_-prefixed name. Java lets
    #     a class name its own statics unqualified; the C++ static def is `s_name`. Skip any
    #     occurrence that's already qualified (`X::`, `x.`), already prefixed (`s_name`), or
    #     INSIDE a string/char literal (renaming a word in "stone" would corrupt resource names).
    for nm in own_statics:
        s = sub_outside_literals(re.compile(r'(?<![\w.:])' + re.escape(nm) + r'\b'),
                                 naming.static_field_name(nm), s)
    # 5c. bare/this call to one of THIS class's name-collided methods -> name_ (the method was
    #     renamed because a field shares its name). Skips qualified/`.`/string-literal forms.
    for nm in resolver.method_renames.get(current, ()):
        s = sub_outside_literals(re.compile(r'(?<![\w.:>])' + re.escape(nm) + r'(\s*\()'),
                                 nm + r'_\1', s)
    # 6. container/String methods -> STL (.add->.push_back, .get(i)->[i], .charAt(i)->[i], isEmpty->empty)
    s = _translate_calls(s, scope)
    # 6b. cast-result access: ((Type*)EXPR).m -> ((Type*)EXPR)->m  (a pointer cast yields a pointer);
    #     handles `*`-casts and balanced EXPRs (calls / [index]).
    s = _deref_cast_results(s, resolver)
    # 6c. chained member access: receiver sub-expression that resolves to a pointer (static
    #     reference field / pointer field of a typed receiver) -> '.' becomes '->'
    s = _resolve_chains(s, scope, resolver, current)
    # 6d. method/ctor RESULT chains: call(...).m -> call(...)->m / (new T(...))->m
    s = _resolve_call_chains(s, scope, resolver, current)
    # 6e. JObject-operand casts (Object var / Object[] element / raw-map value): lower BEFORE the
    #     generic cast pass -- (String)obj -> (obj).str(), (String[])obj -> (obj).strArray(),
    #     (Integer/Character/...)obj -> (obj) (JObject operators/unbox-methods do the rest).
    s = _rewrite_jobject_casts(s, scope)
    # 7. cast translation: reference type -> pointer cast ((Block)x -> (Block*)x);
    #    primitive whose C++ spelling differs -> the C++ spelling ((byte)x -> (unsigned char)x)
    def _cast(m):
        t = m.group(1)
        if resolver.is_pointer(ir.JType(t)):
            return '(%s*)' % t
        cpp = typemap.JAVA_TO_CPP.get(t)
        # primitive whose spelling differs (byte->unsigned char) OR a value class whose C++
        # name differs (String->JavaString; (String)NULL -> (JavaString)NULL, which the
        # const char* ctor turns into ""). Skip containers (no valid value-cast spelling).
        if cpp and cpp != t and t not in typemap.CONTAINERS:
            return '(%s)' % cpp
        return m.group(0)
    s = _CAST.sub(_cast, s)
    # 7b. `catch (Type name)` -> `catch (Type* name)` for a reference type: exceptions are thrown
    #     as `new X(...)` (pointers), so the handler binds a pointer. (Exceptions are off on the
    #     console; this just makes the dead handler compile and keeps the caught var usable.)
    def _catch(m):
        t = m.group(1)
        return 'catch (%s* %s)' % (t, m.group(2)) if resolver.is_pointer(ir.JType(t)) else m.group(0)
    s = re.sub(r'\bcatch\s*\(\s*([A-Za-z_]\w*)\s+([A-Za-z_]\w*)\s*\)', _catch, s)
    # 7c. `new Object()` (bare Object, the Java lock-monitor idiom) -> a default JObject
    #     (Object is JObject now). Identity is all that's used (locks are stripped anyway).
    s = re.sub(r'\bnew\s+Object\s*\(\s*\)', 'JObject()', s)
    # 8. Java null literal -> NULL
    s = re.sub(r'\bnull\b', 'NULL', s)
    # 9pre. new T[]{a, b, c} aggregate -> JArrayInit builder (before the sized-new pass)
    if '[]' in s and '{' in s:
        s = _rewrite_array_aggregates(s, resolver)
    # 9b. multi-dim sized new: new T[a][b] / new T[a][b][c] -> j2x_newarr2/3<elem>(...) (nested JArray)
    s = _rewrite_multidim_new(s, resolver)
    # 9. new T[n] -> JArray<elem>(n) (length-carrying, bounds-checked, value-initialized)
    def _new_arr(m):
        cpp = resolver.cpp_type(ir.JType(m.group(1), array_dims=1), 'local')   # JArray<elem>
        size = m.group(3).strip()
        return '%s(%s)' % (cpp, size) if size else '%s()' % cpp
    s = _NEW_ARRAY.sub(_new_arr, s)
    # 10. super.method(...) -> Parent::method(...)  (super(...) ctor calls handled in source.py)
    if parent:
        s = re.sub(r'\bsuper\.(\w+)', parent + r'::\1', s)
    # 11. x instanceof Type -> dynamic_cast<Type*>(x) != NULL  (RTTI / -GR is on). When the
    #     operand is an Object (void*), RTTI is impossible -- fall back to a non-null check.
    _BOXED_IS = {'Character': 'isCharacter', 'String': 'isString',
                 'Integer': 'isInteger', 'Boolean': 'isBoolean'}

    def _instanceof(m):
        lhs, typ = m.group(1).strip(), m.group(2).strip()
        is_arr = typ.endswith(']')
        base_typ = re.sub(r'\s*\[\s*\]$', '', typ).strip()
        # JObject operand? -- a bare Object var, an Object[] element, or a raw-map value.
        is_obj = _expr_object_kind(lhs, scope) is not None
        if not is_obj:                           # also accept a simple Object-typed scope var
            jt = scope.get(lhs)
            is_obj = jt is not None and (jt.name == 'Object'
                                         or resolver.cpp_type(jt, 'local').replace(' ', '') == 'JObject')
        if is_arr:                               # `obj instanceof String[]` -> JObject StringArray kind
            if is_obj or base_typ == 'String':
                return '((%s).isStringArray())' % lhs
            return '((%s) != NULL)' % lhs
        if base_typ in _BOXED_IS and is_obj:     # instanceof Character/String/Integer/Boolean on a box
            return '((%s).%s())' % (lhs, _BOXED_IS[base_typ])
        if is_obj:                               # Object (JObject) instanceof <Class> -> RTTI kind check
            return '((%s).is<%s>())' % (lhs, base_typ)
        if resolver.cpp_type(ir.JType(base_typ), 'local').replace(' ', '').startswith('void*'):
            return '((%s) != NULL)' % lhs        # can't dynamic_cast a void* -- only null-test
        return '(dynamic_cast<%s*>(%s) != NULL)' % (base_typ, lhs)
    s = _INSTANCEOF.sub(_instanceof, s)
    # 11b. Java '%' on floating-point operands -> fmodf (C++ '%' is integer-only). Fire only when
    #      one side is a float LITERAL, so we know it's float math (e.g. `counter % 4.0F`).
    if '%' in s:
        s = _rewrite_float_mod(s)
    # 12. Java string concatenation (C++ can't '+' a string literal with an int/float/object).
    #     Rewrite `"x" + n + obj` -> `(JCat() + "x" + n + obj)`, which streams each operand and
    #     converts to JavaString/std::string in the surrounding value context.
    # 12a. inside argument groups: out.println("a" + n), new Exception("a" + n), foo("a"+b)
    if '"' in s and '+' in s:
        s = _rewrite_concats(s)
    # 12b. return <concat>;
    rm = re.match(r'^(\s*return\s+)(.+)(;\s*)$', s)
    if rm:
        w = _to_jcat(rm.group(2))
        if w:
            s = rm.group(1) + w + rm.group(3)
    else:
        # 12c. assignment / declaration-init RHS: LHS = <concat>;
        eq = _assign_pos(s)
        if eq != -1:
            tm = re.match(r'^(.*?)(;\s*)$', s[eq + 1:])
            if tm:
                w = _to_jcat(tm.group(1))
                if w:
                    s = s[:eq + 1] + ' ' + w + tm.group(2)
    return s


def _assign_pos(s):
    """Index of the top-level simple-assignment '=' in a statement, or -1. Skips '==',
    '<=', '>=', '!=', and compound assigns (+=, -=, ...); ignores '=' inside (), [], {},
    and string/char literals. '<>' is not tracked (would mis-balance on '->')."""
    depth, i, n = 0, 0, len(s)
    while i < n:
        c = s[i]
        if c == '"' or c == "'":
            q = c; i += 1
            while i < n:
                if s[i] == '\\' and i + 1 < n:
                    i += 2; continue
                if s[i] == q:
                    i += 1; break
                i += 1
            continue
        if c in '([{':
            depth += 1
        elif c in ')]}':
            depth -= 1
        elif c == '=' and depth == 0:
            prv = s[i - 1] if i else ''
            nxt = s[i + 1] if i + 1 < n else ''
            if nxt != '=' and prv not in '=<>!+-*/%&|^~':
                return i
        i += 1
    return -1


def _is_nullable_array(base, scope, resolver):
    """True if `base[...]` indexes a pointer-element array (registry list or a
    scope array of a reference type) -- i.e. a slot can be NULL."""
    stripped = base[2:] if base.startswith('s_') else base
    if stripped in typemap.KNOWN_NULLABLE:
        return True
    jt = scope.get(base) or scope.get(stripped)
    return bool(jt and jt.is_array and resolver.is_pointer(jt.element()))


# standalone `getter(args)->call(args);`
_GETTER_DEREF_STMT = re.compile(
    r'^((?:this->)?(?:\w+::)*(\w+)\([^)]*\))\s*->\s*(\w+\([^;]*\))\s*;\s*$')

# `this->field->call(args);` as a standalone stmt
_FIELD_DEREF_STMT = re.compile(
    r'^((?:this->)(\w+))\s*->\s*(\w+\([^;]*\))\s*;\s*$')

# General nullable-getter deref scanner: matches receiver->GETTER(args) where GETTER
# is in KNOWN_NULLABLE_GETTERS. Receiver can be this->, var->, this->field[idx]->, etc.
# Rebuilt via refresh_nullable_getters() after the pipeline DERIVES additional nullable getters
# from the corpus (so a getter that can `return null` on ANY corpus, not just the curated Alpha
# set, gets emit-time guarded -- not merely flagged).
def refresh_nullable_getters():
    """Rebuild the getter-deref scanner from the CURRENT typemap.KNOWN_NULLABLE_GETTERS. Call after
    the pipeline unions derived nullable getters into that set, so emit-time guarding covers them."""
    global _GETTER_NAMES_ALT, _NULLABLE_GETTER_CALL
    _GETTER_NAMES_ALT = '|'.join(re.escape(g) for g in
        sorted(typemap.KNOWN_NULLABLE_GETTERS, key=len, reverse=True))
    _NULLABLE_GETTER_CALL = re.compile(
        r'((?:this->)?(?:\w+(?:\[[^\]]*\])?\s*->\s*)*)'
        r'(' + _GETTER_NAMES_ALT + r')\s*\(')


refresh_nullable_getters()

# A function/method CALL token (identifier immediately followed by `(`), excluding control keywords.
# Used to keep the getter-hoist SIDE-EFFECT SAFE: never hoist a getter across a preceding call.
_PRECEDING_CALL = re.compile(
    r'(?<![\w>])(?!(?:if|while|for|switch|return|sizeof|catch|new)\b)[A-Za-z_]\w*\s*\(')


def _default_return_val(rt, resolver):
    if rt is None or rt.name == 'void':
        return None
    if rt.is_array:
        return '%s()' % resolver.cpp_type(rt, 'local')
    if rt.name == 'String':
        return 'JavaString()'
    if rt.name in ('boolean',):
        return 'false'
    if rt.name in typemap.PRIMITIVES:
        return '0'
    return 'NULL'


def _nullable_getter_ctype(getter_name):
    """C++ pointer type for a pointer-returning nullable getter (from typemap), so the null-guard
    hoist can declare a typed temp in C++03 -- never `auto`. None if the getter has no known pointer
    return type (then the caller skips hoisting rather than emit `auto`)."""
    t = typemap.NULLABLE_GETTER_RETURN_TYPE.get(getter_name)
    return (t + '*') if t else None


def _hoist_nullable_getter(s, return_type, resolver):
    """If s contains a KNOWN_NULLABLE_GETTER(args)->deref not caught by the standalone
    patterns, hoist the getter into a temp variable and null-guard the dereference."""
    for m in _NULLABLE_GETTER_CALL.finditer(s):
        gn = m.group(2)
        open_idx = m.end() - 1
        close = _balanced_paren(s, open_idx)
        if close < 0:
            continue
        j = close + 1
        while j < len(s) and s[j] == ' ':
            j += 1
        if j + 1 >= len(s) or s[j:j + 2] != '->':
            continue

        # SIDE-EFFECT SAFETY (the RenderPig-class invariant): never hoist the getter ACROSS a
        # preceding call in the same statement. Java evaluates left-to-right; moving the getter to
        # the statement head would reorder it before an earlier (possibly side-effecting) call. Only
        # hoist when nothing callable precedes it (the common `return getX()->..`/`T v = getX()->..`).
        if _PRECEDING_CALL.search(s[:m.start()]):
            continue

        # Need the getter's concrete pointer type to declare the temp in C++03 (no `auto`). If it's
        # unknown, don't hoist -- leave the expression for other guards rather than emit C++11.
        ctype = _nullable_getter_ctype(gn)
        if ctype is None:
            continue

        recv_expr = s[m.start():close + 1]
        tmp = '_j2x_%s' % gn.replace('get', '', 1)[:20]
        tmp = tmp[0].lower() + tmp[1:]
        new_line = s[:m.start()] + tmp + s[close + 1:]

        stripped = s.strip()
        if stripped.startswith('return '):
            dflt = _default_return_val(return_type, resolver)
            if dflt is None:
                return '%s %s = %s; if (%s == NULL) return; %s' % (
                    ctype, tmp, recv_expr, tmp, new_line.strip())
            return '%s %s = %s; if (%s == NULL) return %s; %s' % (
                ctype, tmp, recv_expr, tmp, dflt, new_line.strip())

        dm = _DECL.match(new_line.strip())
        if dm and dm.group(3) == '=' and _decl_base(dm.group(1)) not in _KEYWORDS:
            cpp_type = dm.group(1)
            var_name = dm.group(2)
            assign_rest = new_line.strip()[dm.end(3):].lstrip()
            return ('%s %s = 0; %s %s = %s; if (%s != NULL) { %s = %s }'
                    % (cpp_type, var_name, ctype, tmp, recv_expr, tmp, var_name, assign_rest))

        return '{ %s %s = %s; if (%s != NULL) { %s } }' % (
            ctype, tmp, recv_expr, tmp, new_line.strip())
    return None


def _guard_null_deref(s, scope, resolver, return_type=None):
    """Wrap NULL-dereference-risk patterns in a NULL check; return (line, guarded, rule_id)."""
    # (1) list-indexed deref: blocksList[id]->method()
    m = _INDEX_DEREF_STMT.match(s)
    if m:
        recv, base, call = m.group(1), m.group(2), m.group(3)
        if _is_nullable_array(base, scope, resolver):
            return 'if (%s != NULL) { %s->%s; }' % (recv, recv, call), True, 'null-deref-guard'

    # (2) getter()->deref: getChunkFromChunkCoords(...)->method()
    m2 = _GETTER_DEREF_STMT.match(s)
    if m2:
        recv_expr, getter_name, call = m2.group(1), m2.group(2), m2.group(3)
        if getter_name in typemap.KNOWN_NULLABLE_GETTERS:
            ctype = _nullable_getter_ctype(getter_name)
            if ctype is not None:
                tmp = '_j2x_%s' % getter_name.replace('get', '', 1)[:20]
                tmp = tmp[0].lower() + tmp[1:]
                return ('{ %s %s = %s; if (%s != NULL) { %s->%s; } }'
                        % (ctype, tmp, recv_expr, tmp, tmp, call)), True, 'null-deref-getter-guard'
            # Unknown pointer type -> C++03-safe double-eval (no temp). These getters are pure lookups.
            return ('if (%s != NULL) { %s->%s; }'
                    % (recv_expr, recv_expr, call)), True, 'null-deref-getter-guard'

    # (3) General: nullable getter()->deref embedded in a return / assignment / expression.
    result = _hoist_nullable_getter(s, return_type, resolver)
    if result is not None:
        return result, True, 'null-deref-getter-guard'

    # (4) nullable field deref: this->worldObj->method()
    # NOT auto-guarded at emit time -- the working game shows these are overwhelmingly
    # false positives (the field is always set when the method runs). The post-hoc scanner
    # rule (null-deref-field) still flags them for manual review.

    return s, False, None


def translate_body(method, cls, resolver, ledger=None):
    """Return translated C++ body lines (no indentation; caller indents)."""
    scope = build_scope(cls, method)
    ptr_locals = _pointer_locals(method, scope, resolver)   # value locals used as nullable refs
    ptr_params = ownership.nullable_value_params(method, resolver)   # value-struct params used nullably
    # catch (Type name) of a reference type becomes catch (Type* name) -- exceptions are thrown as
    # `new X` -- so the handler var is a pointer and its member access must use '->'.
    _body_text = '\n'.join(t for _ln, t in method.body)
    ptr_catch = set(m.group(2) for m in re.finditer(r'\bcatch\s*\(\s*([A-Za-z_]\w*)\s+([A-Za-z_]\w*)\s*\)', _body_text)
                    if resolver.is_pointer(ir.JType(m.group(1))))
    ptr_names = sorted((set(n for n, jt in scope.items() if resolver.is_pointer(jt))
                        | ptr_locals | ptr_params | ptr_catch),
                       key=len, reverse=True)   # longer names first (avoid prefix overlap)
    static_set = (set(resolver.known) | set(resolver.enums) | {cls.name}
                  | typemap.RUNTIME_HEADERS | typemap.JAVART_STATICS)
    enum_set = set(resolver.enums) | typemap.RUNTIME_ENUM_LIKE
    # own statics, plus inherited statics from the member environment (so a bare reference to
    # a parent's static field also gets the s_ prefix, e.g. BlockStep using Block.stairSingle)
    own_statics = set(f.name for f in cls.static_fields())
    own_statics |= resolver.members.get(cls.name, {}).get('statics', set())
    out = []
    for raw in inner_lines(method.body):
        s = raw.strip()
        if not s or s.startswith('//') or s.startswith('*') or s.startswith('/*'):
            if s:
                out.append(s)
            continue
        line = _translate_line(s, ptr_names, static_set, enum_set, resolver, scope,
                               cls.parent, cls.name, own_statics, ptr_locals)
        line, guarded, guard_rule = _guard_null_deref(line, scope, resolver, method.return_type)
        if guarded and ledger is not None:
            _GUARD_MSGS = {
                'null-deref-guard':
                    'null-deref guard inserted before list-indexed call (an unregistered/NULL '
                    'slot would be a virtual call through NULL -- undefined behaviour in C++)',
                'null-deref-getter-guard':
                    'null-deref guard inserted: getter can return NULL -- dereference without '
                    'check is undefined behaviour in C++ (Java would throw NPE)',
                'null-deref-field-guard':
                    'null-deref guard inserted: nullable field dereference -- undefined '
                    'behaviour in C++ if field is NULL (Java would throw NPE)',
            }
            ledger.add(cls.name + '.cpp', Category.RISK,
                       _GUARD_MSGS.get(guard_rule, 'null-deref guard inserted'),
                       line=method.line, severity=Severity.MED, source='emit', rule=guard_rule)
        out.append(line)

    # a String return is by value (std::string); `return null` would construct a string
    # from a NULL char* (undefined behavior / crash) -- return an empty string instead.
    rt = method.return_type
    if rt is not None and rt.name == 'String' and not rt.is_array:
        out = [re.sub(r'\breturn\s+NULL\s*;', 'return std::string();', l) for l in out]
    # an array return is by value (JArray<T>); `return null` -> an empty JArray (no implicit
    # NULL->JArray conversion exists), matching the value-container model.
    if rt is not None and rt.is_array:
        empty = '%s()' % resolver.cpp_type(rt, 'local')
        out = [re.sub(r'\breturn\s+NULL\s*;', 'return %s;' % empty, l) for l in out]

    before = out
    out = _translate_labels(out)                  # labeled break/continue -> goto (body-level)
    if ledger is not None and len(out) != len(before):
        ledger.add(cls.name + '.cpp', Category.WARNING,
                   'labeled break/continue translated to goto (verify the loop still terminates)',
                   line=method.line, severity=Severity.MED, source='emit', rule='label-goto')
    out = _rewrite_leak_temporaries(out, resolver, ledger, cls)   # (new T())->m() throwaway -> scoped stack obj
    out = _flag_reflection(out, ledger, cls)      # tag newInstance/forName/getConstructor for review
    out = [_clarify_precedence(l) for l in out]   # C4554: parenthesize shift/bitwise sub-expressions
    out = _drop_unused_catch_vars(out)            # `catch (Type* e){}` with e unused -> `catch (Type*)` (C4101)
    return out
