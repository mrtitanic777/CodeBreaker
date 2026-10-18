# -*- coding: utf-8 -*-
"""
statements/expr.py -- expression & numeric-literal rewriting (no cross-module state):
  * Java numeric-literal suffixes -> C++ (2.0D->2.0, 7L->7LL)      (_fix_numeric_suffixes)
  * unicode/char escapes -> portable literals                     (_fix_unicode_literals)
  * operator-precedence reparenthesization (C4554 shift/bitwise)  (_clarify_precedence / _reparen_expr)
  * `a % b` on floats -> fmodf(a, b)                              (_rewrite_float_mod)
Leaf module: only `re` + its own patterns; imported by the statements package.
"""

import re

from codebreaker.java.emit.statements.scan import sub_outside_literals


_FLOAT_LIT =r'\d+\.\d*[FfDd]?|\.\d+[FfDd]?|\d+[FfDd]'
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


# ---------------------------------------------------------------------------------------------------
# (int)x / (long long)x -> j2xToInt(x) / j2xToLong(x): Java's float/double -> int/long conversion
# saturates (NaN -> 0) where C++'s is undefined out of range (JavaRT.h). Applied to whole emitted class
# files (transpile._finalize_emitted), outside string and char literals; a cast whose operand cannot be
# read is left as it was.
_INT_CAST = re.compile(r'\((int|long long)\)')
_CAST_TYPES = ('int', 'long long', 'float', 'double', 'char', 'short', 'bool', 'signed char', 'unsigned char',
               'unsigned int', 'long')
_NUMBER = re.compile(r'(?:\d+\.?\d*|\.\d+)(?:[eE][+-]?\d+)?[FfLlDdUu]*|0[xX][0-9a-fA-F]+[LlUu]*')
_WORD = re.compile(r'[A-Za-z_]\w*(?:\s*::\s*[A-Za-z_]\w*)*')
_HELPER = {'int': 'j2xToInt', 'long long': 'j2xToLong'}


def _close(s, i, open_c, close_c):
    depth, n = 0, len(s)
    while i < n:
        c = s[i]
        if c in '"\'':
            j = i + 1
            while j < n and s[j] != c:
                j += 2 if s[j] == '\\' else 1
            i = j + 1
            continue
        if c == open_c:
            depth += 1
        elif c == close_c:
            depth -= 1
            if depth == 0:
                return i
        i += 1
    return -1


def _cast_operand_end(s, i):
    """End index of the unary operand starting at s[i] (a cast binds to one), or -1."""
    n = len(s)
    while i < n and s[i] == ' ':
        i += 1
    if i >= n:
        return -1
    if s[i] in '-+!~' and not s.startswith('--', i) and not s.startswith('++', i):
        return _cast_operand_end(s, i + 1)
    if s[i] == '(':
        close = _close(s, i, '(', ')')
        if close < 0:
            return -1
        if s[i + 1:close].strip() in _CAST_TYPES or s[i + 1:close].strip().endswith('*'):
            return _cast_operand_end(s, close + 1)                  # a cast of a cast: (int)(float)x
        j = close + 1
    else:
        m = _NUMBER.match(s, i) if s[i].isdigit() or s[i] == '.' else _WORD.match(s, i)
        if not m:
            return -1
        j = m.end()
    while j < n:                                                    # postfix: call, index, member access
        if s[j] == '(':
            k = _close(s, j, '(', ')')
        elif s[j] == '[':
            k = _close(s, j, '[', ']')
        elif s.startswith('->', j) or (s[j] == '.' and j + 1 < n and (s[j + 1].isalpha() or s[j + 1] == '_')):
            m = _WORD.match(s, j + (2 if s[j] == '-' else 1))
            k = m.end() - 1 if m else -1
        else:
            break
        if k < 0:
            return -1
        j = k + 1
    if s.startswith('++', j) or s.startswith('--', j):
        j += 2
    return j


def java_int_casts(text):
    out, i, n = [], 0, len(text)
    while i < n:
        c = text[i]
        if c in '"\'':                                              # literals verbatim
            j = i + 1
            while j < n and text[j] != c and text[j] != '\n':
                j += 2 if text[j] == '\\' else 1
            out.append(text[i:j + 1])
            i = j + 1
            continue
        if text.startswith('//', i):                                # comments verbatim
            j = text.find('\n', i)
            j = n if j < 0 else j
            out.append(text[i:j])
            i = j
            continue
        m = _INT_CAST.match(text, i) if c == '(' else None
        if m:
            end = _cast_operand_end(text, m.end())
            if end > m.end():
                operand = text[m.end():end].strip()
                out.append('%s(%s)' % (_HELPER[m.group(1)], java_int_casts(operand)))
                i = end
                continue
        out.append(c)
        i += 1
    return ''.join(out)

