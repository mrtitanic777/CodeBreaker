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

from codebreaker.java import parse as javaparse, typemap, ir
from codebreaker.java.ledger import Category, Severity
from codebreaker.java.emit import naming, ownership

# a standalone statement `recv[idx]->call(...);` (or `.` form) -- group2 = base name
_INDEX_DEREF_STMT = re.compile(
    r'^((?:this->)?(?:\w+::)*(\w+)\[[^\]]*\])\s*(?:->|\.)\s*(\w+\([^;]*\))\s*;\s*$')

# string/char-literal-aware substitution + the literal pattern -- leaf text primitives live in scan.py
# so both this package and the sibling transform modules (expr.py, ...) share them without a cycle.
from codebreaker.java.emit.statements.scan import sub_outside_literals
from codebreaker.java.emit.statements.scan import _balanced_paren, _balanced_brace, _balanced_from, _balanced_bracket

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
        # Derive the element from the ARRAY type, not the scalar: `byte[]` element is unsigned char
        # (buffer policy) while a scalar `byte` is signed char -- a `new byte[a][b]` must match the
        # `JArray<JArray<unsigned char> >` field it fills (TexturePortalFX), so use the array element.
        arr = resolver.cpp_type(ir.JType(t, array_dims=1), 'local')
        if arr.startswith('JArray<') and arr.endswith('>'):
            return arr[len('JArray<'):arr.rindex('>')].strip()
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

from codebreaker.java.emit.statements.constants import _LIST_TYPES, _METHOD_RENAME, _INDEX_METHOD


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


from codebreaker.java.emit.statements.scan import _split_top_commas


from codebreaker.java.emit.statements.expr import _fix_numeric_suffixes, _fix_unicode_literals, _clarify_precedence, _rewrite_float_mod
_PURE_STR_LIT = re.compile(r'^"(?:[^"\\]|\\.)*"$')


def _has_top_level_ternary(e):
    """True if `e` contains a top-level `?` (a conditional), respecting (), [], {} and literals."""
    depth, in_str, in_chr, i, n = 0, False, False, 0, len(e)
    while i < n:
        c = e[i]
        if in_str:
            if c == '\\':
                i += 2; continue
            if c == '"':
                in_str = False
        elif in_chr:
            if c == '\\':
                i += 2; continue
            if c == "'":
                in_chr = False
        elif c == '"':
            in_str = True
        elif c == "'":
            in_chr = True
        elif c in '([{':
            depth += 1
        elif c in ')]}':
            depth -= 1
        elif depth == 0 and c == '?':
            return True
        i += 1
    return False


def _top_level_semicolon(e):
    """True if `e` has a `;` at bracket depth 0 (outside literals) -- i.e. it's a statement
    sequence / for-header, NOT a single value expression."""
    depth, in_str, in_chr, i, n = 0, False, False, 0, len(e)
    while i < n:
        c = e[i]
        if in_str:
            if c == '\\':
                i += 2; continue
            if c == '"':
                in_str = False
        elif in_chr:
            if c == '\\':
                i += 2; continue
            if c == "'":
                in_chr = False
        elif c == '"':
            in_str = True
        elif c == "'":
            in_chr = True
        elif c in '([{':
            depth += 1
        elif c in ')]}':
            depth -= 1
        elif depth == 0 and c == ';':
            return True
        i += 1
    return False


def _split_ternary(e):
    """Split a top-level `cond ? a : b` into (cond, a, b), respecting nesting and literals.
    Returns None if there's no top-level `?`."""
    depth, in_str, in_chr, i, n, q = 0, False, False, 0, len(e), -1
    while i < n:
        c = e[i]
        if in_str:
            if c == '\\':
                i += 2; continue
            if c == '"':
                in_str = False
        elif in_chr:
            if c == '\\':
                i += 2; continue
            if c == "'":
                in_chr = False
        elif c == '"':
            in_str = True
        elif c == "'":
            in_chr = True
        elif c in '([{':
            depth += 1
        elif c in ')]}':
            depth -= 1
        elif depth == 0 and c == '?':
            q = i; break
        i += 1
    if q == -1:
        return None
    depth, in_str, in_chr, i, tern = 0, False, False, q + 1, 0
    while i < n:
        c = e[i]
        if in_str:
            if c == '\\':
                i += 2; continue
            if c == '"':
                in_str = False
        elif in_chr:
            if c == '\\':
                i += 2; continue
            if c == "'":
                in_chr = False
        elif c == '"':
            in_str = True
        elif c == "'":
            in_chr = True
        elif c in '([{':
            depth += 1
        elif c in ')]}':
            depth -= 1
        elif depth == 0 and c == '?':
            tern += 1
        elif c == ':' and i + 1 < n and e[i + 1] == ':':
            i += 2; continue                              # C++ scope resolution `::`, not a ternary colon
        elif depth == 0 and c == ':':
            if tern == 0:
                return (e[:q], e[q + 1:i], e[i + 1:])
            tern -= 1
        i += 1
    return None


def _wrap_literal_method(s):
    """`"lit".method(...)` -> `JavaString("lit").method(...)`. A bare C string literal has no Java
    String methods (length/equals/charAt/...), so a direct `.` after it won't compile. Only fires on a
    literal immediately followed (past spaces) by `.<identifier>`; an already-wrapped `JavaString("x")`
    ends in `)` before the `.`, so this is idempotent. String contents are scanned, never rewritten."""
    if '"' not in s:
        return s
    out, i, n = [], 0, len(s)
    while i < n:
        c = s[i]
        if c == "'":                                     # char literal -- copy verbatim
            out.append(c); i += 1
            while i < n:
                out.append(s[i])
                if s[i] == '\\' and i + 1 < n:
                    out.append(s[i + 1]); i += 2; continue
                if s[i] == "'":
                    i += 1; break
                i += 1
            continue
        if c == '"':
            start = i; out_lit = ['"']; i += 1
            while i < n:
                out_lit.append(s[i])
                if s[i] == '\\' and i + 1 < n:
                    out_lit.append(s[i + 1]); i += 2; continue
                if s[i] == '"':
                    i += 1; break
                i += 1
            lit = ''.join(out_lit)
            j = i
            while j < n and s[j] in ' \t':
                j += 1
            if j < n and s[j] == '.' and j + 1 < n and (s[j + 1].isalpha() or s[j + 1] == '_'):
                out.append('JavaString(' + lit + ')')
            else:
                out.append(lit)
            continue
        out.append(c); i += 1
    return ''.join(out)


def _outer_parened(e):
    """If `e` is one fully-enclosing paren group `( ... )`, return the inside; else None."""
    e = e.strip()
    if len(e) >= 2 and e[0] == '(' and e[-1] == ')' and _balanced_paren(e, 0) == len(e) - 1:
        return e[1:-1]
    return None


def _to_jcat(expr):
    """If `expr` is a Java string concatenation (top-level '+' with at least one PURE
    string-literal operand), wrap it in the JCat builder; else return None. The pure-literal
    test avoids mis-wrapping numeric `+` like `"abc".length() + 5` (literal-then-method)."""
    e = expr.strip()
    if '"' not in e or '+' not in e:
        return None
    # Not a single value expression -> never wrap. A top-level ';' means a for-header/statement
    # sequence; a top-level simple '=' means an embedded assignment. Wrapping either produced the
    # `for((JCat() + var = ...` corruption.
    if _top_level_semicolon(e) or _assign_pos(e) != -1:
        return None
    # A top-level ternary hides the `+`s inside its branches. Recurse into each branch and JCat-lower
    # the ones that are concatenations (a number-first branch like `x + " s"` won't compile otherwise);
    # leave the condition alone. Strip a branch's enclosing parens so nested `(a ? b : c)` also recurses.
    if _has_top_level_ternary(e):
        t = _split_ternary(e)
        if not t:
            return None
        cond, a, b = t

        def _branch(x):
            x = x.strip()
            r = _to_jcat(x)
            if r:
                return r
            inner = _outer_parened(x)
            if inner is not None:
                ri = _to_jcat(inner.strip())
                if ri:
                    return '(' + ri + ')'
            return None
        na, nb = _branch(a), _branch(b)
        if not na and not nb:
            return None
        # Type-unify the arms: when ONE arm became a JCat and the other is a bare string literal, the
        # ternary has no common type (`cond ? "" : (JCat()+..)` -> JCat can't convert to const char*).
        # Wrap the literal as JavaString (JCat converts to JavaString) so both arms agree (ScreenShotHelper).
        fa, fb = na or a.strip(), nb or b.strip()
        if nb and not na and _PURE_STR_LIT.match(a.strip()):
            fa = 'JavaString(%s)' % a.strip()
        if na and not nb and _PURE_STR_LIT.match(b.strip()):
            fb = 'JavaString(%s)' % b.strip()
        return cond + '? ' + fa + ' : ' + fb
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


def _deref_container_subclass_new(s, resolver):
    """`new X(...)` where X derives from a by-value container (JList/JMap/JSet) -> `(*(new X(...)))`.
    Such an instance IS the container value (argo's J_JsonNodeList : List / J_JsonObjectNodeList : Map),
    so a bare `new X` (a pointer) can't be returned/passed where the base container VALUE is expected;
    the deref slices to the base container (the heap object is a dead-path leak)."""
    out, i, n = [], 0, len(s)
    while True:
        m = re.compile(r'\bnew\s+([A-Za-z_]\w*)\s*\(').search(s, i)
        if not m:
            out.append(s[i:]); break
        cls = m.group(1)
        if not resolver.extends_value_container(cls):
            out.append(s[i:m.end()]); i = m.end(); continue
        close = _balanced_paren(s, m.end() - 1)
        if close == -1:
            out.append(s[i:m.end()]); i = m.end(); continue
        out.append(s[i:m.start()] + '(*(' + s[m.start():close + 1] + '))')
        i = close + 1
    return ''.join(out)


_TERM = r'(?:[A-Za-z_]\w*(?:\s*(?:->|\.|::)\s*[A-Za-z_]\w*|\s*\([^()]*\)|\s*\[[^\]]*\])*|\d+)'
_SHIFT_ARITH = re.compile(
    r'(?<![>\w])'                                        # not >>> and not mid-identifier
    r'(' + _TERM + r'(?:\s*[-+*/%]\s*' + _TERM + r')+)'  # LEFT operand = term (arith term)+
    r'(\s*(?:>>|<<))(?![=<>])')                          # the shift op (not >>=, <<=, >>>)


def _paren_shift_operands(s):
    """`a - 3 >> 4` / `a * 121 >> 15` -> `(a - 3) >> 4` / `(a * 121) >> 15`. Java and C++ agree that
    additive/multiplicative binds tighter than shift, so this is a NO-OP on behaviour -- it only adds the
    clarifying parens MSVC's C4554 asks for (a shift with an unparenthesized arithmetic operand)."""
    if '>>' not in s and '<<' not in s:
        return s
    return _SHIFT_ARITH.sub(lambda m: '(%s)%s' % (m.group(1), m.group(2)), s)


def _fix_indexed_add(s):
    """`recv.push_back(a, b)` -> `recv.add(a, b)`. std::vector::push_back takes ONE arg, so a 2-arg
    push_back is a mis-renamed Java `list.add(index, elem)`; JList::add(int, const T&) is the insert."""
    out, i, n = [], 0, len(s)
    while True:
        j = s.find('.push_back(', i)
        if j == -1:
            out.append(s[i:]); break
        open_idx = j + len('.push_back(') - 1
        close = _balanced_paren(s, open_idx)
        if close == -1:
            out.append(s[i:]); break
        inner = s[open_idx + 1:close]
        if len(_split_top_commas(inner)) == 2:
            out.append(s[i:j] + '.add(' + inner + ')')
        else:
            out.append(s[i:close + 1])
        i = close + 1
    return ''.join(out)






def _split_top_ternary(expr):
    """If `expr` is a top-level `cond ? a : b`, return (cond, a, b) (all stripped); else None.
    Respects nesting in (), [], {} and nested `?:` pairs. Ternary is right-associative, so we
    split at the FIRST top-level '?' and the ':' that matches it."""
    depth = 0
    q = -1
    for i, c in enumerate(expr):
        if c in '([{':
            depth += 1
        elif c in ')]}':
            depth -= 1
        elif depth == 0 and c == '?':
            q = i
            break
    if q < 0:
        return None
    depth = 0
    pending = 0                                    # nested '?' awaiting their ':'
    for j in range(q + 1, len(expr)):
        c = expr[j]
        if c in '([{':
            depth += 1
        elif c in ')]}':
            depth -= 1
        elif depth == 0 and c == '?':
            pending += 1
        elif depth == 0 and c == ':':
            if pending == 0:
                return (expr[:q].strip(), expr[q + 1:j].strip(), expr[j + 1:].strip())
            pending -= 1
    return None


def _unwrap_parens(expr):
    """Strip redundant outermost parenthesis layers ((a) -> a), leaving the meaning unchanged."""
    expr = expr.strip()
    while expr.startswith('(') and _balanced_paren(expr, 0) == len(expr) - 1:
        expr = expr[1:-1].strip()
    return expr


def _apply_cast_to_arms(cast, expr):
    """Push a pointer-cast lexeme (`(Base*)`) into a conditional so each arm is cast individually.
    C++03 cannot type `cond ? new A() : new B()` when A/B are unrelated pointer types, but
    `cond ? (Base*)(new A()) : (Base*)(new B())` is well-formed (each arm converts to Base*)."""
    tern = _split_top_ternary(_unwrap_parens(expr))
    if tern is None:
        expr = expr.strip()
        return '%s%s' % (cast, expr if expr.startswith('(') else '(%s)' % expr)
    cond, a, b = tern
    return '(%s ? %s : %s)' % (cond, _apply_cast_to_arms(cast, a), _apply_cast_to_arms(cast, b))


_PTR_CAST_CALL = re.compile(r'(\((?:const\s+)?[A-Za-z_]\w*\s*\*\))\s*\(')


def distribute_cast_into_ternaries(s):
    """`(Base*)(cond ? A : B)` -> `(cond ? (Base*)(A) : (Base*)(B))`, recursively for nested/chained
    conditionals. A no-op unless a pointer-cast directly wraps a parenthesized top-level conditional,
    so ordinary casts are left byte-for-byte unchanged."""
    if '?' not in s or '*)' not in s:
        return s
    out = []
    i = 0
    n = len(s)
    while i < n:
        m = _PTR_CAST_CALL.match(s, i)
        if m:
            open_idx = m.end() - 1                  # index of the '(' opening the cast operand
            close = _balanced_paren(s, open_idx)
            if close != -1:
                inner = s[open_idx + 1:close]
                if _split_top_ternary(_unwrap_parens(inner)) is not None:
                    out.append(_apply_cast_to_arms(m.group(1), inner))
                    i = close + 1
                    continue
        out.append(s[i])
        i += 1
    return ''.join(out)


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
        # Skip a CALL / INDEX / TEMPLATE argument paren -- `a(`, `f()(`, `x]( `, `JArray<int>(` -- whose
        # content may START with a cast arg (`a((Entity)null, var1)`). Its trailing `.member` belongs to
        # the CALL RESULT, not the cast, so treating it as `((cast)...).member` wrongly derefs the result
        # (entityMap->a((Entity*)NULL,var1).size() -> ...->size() on a List value). A genuine cast-GROUPING
        # paren is never preceded by an identifier/`]`/`)`/`>`.
        if op > 0 and (s[op - 1].isalnum() or s[op - 1] in '_])>'):
            continue
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


from codebreaker.java.emit.statements.controlflow import _wrap_switch_cases, _translate_labels




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


from codebreaker.java.emit.statements.containers import _category, _container_element, _rewrite_new_containers, _container_call
def _member_expr_start(s, idx):
    """Walk backwards from idx over a leading member-access chain (`this->`, `a.b->c.`) and return the
    index where the FULL receiver expression begins. Used to place a wrapping cast BEFORE the whole
    receiver, not after a `->`/`.` qualifier."""
    j = idx
    while j > 0:
        if s[j - 1] == '.':
            k = j - 1
        elif j >= 2 and s[j - 2:j] == '->':
            k = j - 2
        else:
            break
        i = k
        while i > 0 and (s[i - 1].isalnum() or s[i - 1] == '_'):
            i -= 1
        if i == k:                                    # no identifier before the . / -> : stop
            break
        j = i
    return j


def _balanced_call_sub(s, recv, meth, fn, standalone_only=False, pre=''):
    """Rewrite every `recv.meth(args)` via fn(arg_list) -> replacement (None = leave). With
    standalone_only, skip a `recv` that is a member of something else (`this.recv`) or the suffix of a
    longer identifier -- needed for rewrites that PARENTHESIZE the result (equals -> `(a == b)`), where
    `this.username.equals(x)` would otherwise mangle to `this.(username == x)`. Index rewrites (get->[k])
    don't parenthesize, so they stay off it and correctly rewrite `this->map.get(k)` -> `this->map[k]`.
    With `pre` (e.g. a cast), the prefix is inserted BEFORE the whole receiver expression -- walking back
    over any `this->`/`a.` qualifier -- so `this->name.charAt(i)` -> `(unsigned char)this->name[i]`, not
    the broken `this->(unsigned char)name[i]` (a cast can't sit after `->`)."""
    token = '%s.%s(' % (recv, meth)
    start = 0
    while True:
        idx = s.find(token, start)
        if idx < 0:
            return s
        prev = s[idx - 1] if idx > 0 else ''
        # A hit inside a LONGER identifier (`name.charAt(` within `username.charAt(`) is a different
        # receiver -- never rewrite it as `recv`: with `pre` that spliced the cast mid-identifier
        # (`user(unsigned char)name[i]`). Always skipped; `standalone_only` also skips member receivers.
        if prev == '_' or prev.isalnum() or (standalone_only and (prev == '.' or prev == '>')):
            start = idx + 1
            continue
        op = idx + len(token) - 1
        close = _balanced_paren(s, op)
        if close < 0:
            return s
        rep = fn(_split_top_commas(s[op + 1:close]))
        if rep is None:
            return s                                  # unexpected arity -> leave (and stop)
        if pre:                                       # wrap the FULL receiver expr (incl. this->/a. qualifier) in `pre`
            qstart = _member_expr_start(s, idx)
            s = s[:qstart] + pre + s[qstart:idx] + rep + s[close + 1:]
        else:
            s = s[:idx] + rep + s[close + 1:]
        start = 0                                     # indices shifted -- rescan from the front


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
            # FIXED BUG section-sign-signed-char: Java `char` is UNSIGNED and String.charAt() promotes to an
            # unsigned value in comparisons, but C++ std::string::operator[] returns SIGNED char -- so
            # `str.charAt(i) == 167` (the § colour code, 0xA7) is `-89 == 167` = never true, and §-coloured
            # text renders white. Cast the STRING subscript to unsigned char so high-byte chars (§, extended
            # Latin) compare correctly; list/map .get() keep the plain subscript. Was a per-project overlay
            # (FontRenderer.cpp); migrated here so every convert gets it.
            _cast = '(unsigned char)' if cat == 'string' else ''
            s = _balanced_call_sub(s, recv, idx_m,
                                   lambda a, r=recv: ('%s[%s]' % (r, a[0])) if a else None,
                                   pre=_cast)
        if cat == 'map':                              # map.put(k, v) -> recv[k] = v
            s = _balanced_call_sub(s, recv, 'put',
                                   lambda a, r=recv: ('%s[%s] = %s' % (r, a[0], a[1])) if len(a) == 2 else None)
        if cat == 'string':
            s = _balanced_call_sub(s, recv, 'equals',
                                   lambda a, r=recv: ('(%s == %s)' % (r, a[0])) if a else None,
                                   standalone_only=True)   # parenthesizes -> must not match this.<field>.equals
            s = _balanced_call_sub(s, recv, 'substring',   # Java end-exclusive -> substr(pos, len)
                                   lambda a, r=recv: ('%s.substr(%s, (%s) - (%s))' % (r, a[0], a[1], a[0])) if len(a) == 2
                                   else ('%s.substr(%s)' % (r, a[0])) if len(a) == 1 else None)
    return s


def _rewrite_noequals_identity(s, scope, resolver):
    """`recv.equals(arg)` / `recv->equals(arg)` where recv is a scope var of a CORPUS reference type
    that declares NO equals() -> `(recv == arg)` (Java falls back to Object.equals = reference identity).
    Prevents a call to a non-existent member (EntityMapSlot -> C2039 'equals is not a member').

    build_member_env FLATTENS inheritance, so a class that inherits OR overrides equals keeps its call;
    String/container equals is lowered by _translate_calls; stub types (not in resolver.known) define
    their own equals and are left alone."""
    if 'equals(' not in s:
        return s
    for name, jt in list(scope.items()):
        if jt is None or getattr(jt, 'is_array', False) or not resolver.is_pointer(jt):
            continue
        cn = jt.name
        if cn not in resolver.known:                       # only corpus classes
            continue
        if resolver.member_method_type(cn, 'equals') is not None:
            continue                                       # a real (own/inherited) equals -> keep the call
        for sep in ('->', '.'):
            token = '%s%sequals(' % (name, sep)
            i = 0
            while True:
                p = s.find(token, i)
                if p < 0:
                    break
                prev = s[p - 1] if p > 0 else ''
                if prev.isalnum() or prev == '_' or prev in '.>':   # a suffix / member of something else
                    i = p + len(token); continue
                op = p + len(token) - 1
                close = _balanced_paren(s, op)
                if close < 0:
                    break
                repl = '(%s == %s)' % (name, s[op + 1:close].strip())
                s = s[:p] + repl + s[close + 1:]
                i = p + len(repl)
    return s


def _rewrite_array_null_assign(s, scope, resolver):
    """`arr = NULL;` where arr is an array-typed field/local -> `arr = JArray<T>();`. A JArray is a
    VALUE and can't hold NULL; Java `arr = null` is an empty array (matches the return-null-array rule)."""
    m = re.match(r'^(\s*)(this->[A-Za-z_]\w*|[A-Za-z_]\w*)\s*=\s*(?:NULL|null|0)\s*;(\s*)$', s)
    if not m:
        return s
    lhs = m.group(2)
    name = lhs[6:] if lhs.startswith('this->') else lhs
    jt = scope.get(name)
    if jt is not None and getattr(jt, 'is_array', False):
        return '%s%s = %s();%s' % (m.group(1), lhs, resolver.cpp_type(jt, 'local'), m.group(3))
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
        # for-init object declarations (`for(MCHashEntry var3 = this.slots[i]; ...)`) escape the
        # line-anchored _DECL above, so their loop var never enters scope -> stays a value with `.`
        # access and no null-compare. Capture it so pointer classes get `*` + `->` like any local.
        for fm in re.finditer(r'\bfor\s*\(\s*(?:final\s+)?([A-Za-z_][\w.]*)\s+([A-Za-z_]\w*)\s*=', text):
            if _decl_base(fm.group(1)) in _KEYWORDS or fm.group(2) in _KEYWORDS or fm.group(2) in scope:
                continue
            try:
                scope[fm.group(2)] = javaparse.parse_type(fm.group(1))
            except Exception:                       # noqa: BLE001
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


def _local_used_nullably(name, text):
    """True if local `name` is assigned/compared null or `new`-d in the method text."""
    nm = re.escape(name)
    return bool(re.search(r'\b%s\s*=\s*(?:null|NULL)\b' % nm, text)
                or re.search(r'\b%s\s*(?:==|!=)\s*(?:null|NULL)\b' % nm, text)
                or re.search(r'\b(?:null|NULL)\s*(?:==|!=)\s*%s\b' % nm, text)
                or re.search(r'\b%s\s*=\s*new\b' % nm, text))


def _boxed_nullable_locals(method, scope, resolver):
    """Boxed-wrapper LOCALS (Integer/Byte/Short/Float/...) used nullably must become JObject, NOT
    a pointer-to-primitive. A raw `int*`/`signed char*` can't hold the boxed value the Java code
    assigns it (`x = (Integer)map.get(k)` yields a JObject) and has no .intValue()/.byteValue()
    unbox. JObject holds null, boxes every primitive, unboxes via the value methods, and converts
    to the primitive in arithmetic -- so a nullable boxed local round-trips cleanly as a JObject
    VALUE (no `->`)."""
    boxed = set()
    param_names = set(p.name for p in method.params)
    text = '\n'.join(t for _ln, t in method.body)
    for name in _local_names(method):
        if name in param_names:
            continue
        jt = scope.get(name)
        if jt is None or jt.is_array or jt.name not in typemap.BOXED_VALUE_TYPES:
            continue
        # nullable use OR an unbox call (`var.intValue()` / `.byteValue()` / ...) both mean the local
        # holds a BOXED value (a collection/iterator result, an Object cast) -> it must be a JObject,
        # not the raw primitive (`int var = iter->next()` + `var.intValue()` don't compile).
        if (_local_used_nullably(name, text)
                or re.search(r'(?<![\w.>])' + re.escape(name) +
                             r'\s*\.\s*(?:intValue|byteValue|shortValue|longValue|floatValue|doubleValue|'
                             r'booleanValue|charValue)\s*\(', text)):
            boxed.add(name)
    return boxed


def _pointer_locals(method, scope, resolver):
    """Value-type LOCALS used as nullable references must be C++ pointers -- a C++ value
    type cannot hold null, so a Java local assigned/compared null (or `new`-d) is really a
    reference. Sound and signature-safe: only locals (params would change the signature).
    (Boxed wrappers are handled by _boxed_nullable_locals -> JObject, not by a pointer.)"""
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
                or jt.name in typemap.BOXED_VALUE_TYPES
                or jt.name == 'Object'):
            continue            # already a pointer/array, a value container, String, JObject, or a
            # boxed wrapper (String/JObject hold null themselves and absorb `new T()`; boxed
            # wrappers are retyped to JObject by _boxed_nullable_locals)
        if _local_used_nullably(name, text):
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


def _expr_object_kind(expr, scope, resolver=None):
    """'object' if `expr` is a JObject-typed VALUE -- a bare Object variable, a fully-indexed Object
    array element (var[i] / var[i][j]), a raw-Map index (map[k]), or a raw-List/Map element read
    (list.get(i)/list.remove(i)/map.get(k), incl. `var->field.get/remove(...)` when `resolver` is given).
    Else None. Conservative: only identifier-rooted exprs with simple [idx] groups (no nested brackets)."""
    o = expr.strip()
    o = re.sub(r'\s*(?:\+\+|--)\s*$', '', o)                 # x++ / x-- -> x
    while o.startswith('(') and o.endswith(')') and _is_balanced(o[1:-1]):
        o = o[1:-1].strip()                                  # strip balanced wrapping parens
    o = re.sub(r'^this\s*(?:->|\.)\s*', '', o)               # this->field / this.field -> field (either form; some cast passes run pre-arrow-rewrite)
    # raw-Map read via `.get(k)`: map.get() is NOT rewritten to map[k] (operator[] READ would insert a
    # null entry -- see FIXED BUG map-get-inserts), so recognize the `.get()` form too. Its value is a
    # JObject, exactly like the old map[k], so `(int[])map.get(k)` / `(Integer)map.get(k)` unbox here.
    mg = re.match(r'^([A-Za-z_]\w*)\s*\.\s*get\s*\(.*\)$', o, re.S)
    if mg:
        jt2 = scope.get(mg.group(1))
        if jt2 is not None and not getattr(jt2, 'is_array', False) and jt2.name in _RAW_MAPS:
            return 'object'                                  # raw map value via .get() -> JObject
    # raw-List element read via .get(i) / .remove(i) (JList::remove returns the removed element). A raw
    # List is JList<JObject>, so the element is a JObject -- `(int[])list.remove(i)` unboxes here (Light's
    # lighting-update queue). Only RAW lists (no generic element type); a typed List<T> already yields T.
    lg = re.match(r'^([A-Za-z_]\w*)\s*\.\s*(?:get|remove)\s*\(.*\)$', o, re.S)
    if lg:
        jt3 = scope.get(lg.group(1))
        if (jt3 is not None and not getattr(jt3, 'is_array', False)
                and jt3.name in _LIST_TYPES and not getattr(jt3, 'args', None)):
            return 'object'                                  # raw list element via get()/remove() -> JObject

    def _raw_list_or_map(jt):
        return (jt is not None and not getattr(jt, 'is_array', False)
                and not getattr(jt, 'args', None) and (jt.name in _LIST_TYPES or jt.name in _RAW_MAPS))

    # `var->field.get/remove(k)` (a raw list/map FIELD of a typed receiver, e.g. Light:253's
    # var22->lightingUpdateList.remove(i)). Needs the resolver to type the field.
    if resolver is not None:
        fm = re.match(r'^([A-Za-z_]\w*)\s*(?:->|\.)\s*([A-Za-z_]\w*)\s*\.\s*(?:get|remove)\s*\(.*\)$', o, re.S)
        if fm:
            vjt = scope.get(fm.group(1))
            if vjt is not None and _raw_list_or_map(resolver.member_field_type(vjt.name, fm.group(2))):
                return 'object'
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
    if ndim == 1 and jt.name in _LIST_TYPES and not getattr(jt, 'args', None):
        return 'object'                                     # RAW list element (JList<JObject>) -> JObject
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
            elif s[i:i + 2] == '->' or s[i] == '.':         # member access or `.method(args)` call
                i += 2 if s[i:i + 2] == '->' else 1
                while i < n and s[i].isspace():
                    i += 1
                while i < n and (s[i].isalnum() or s[i] == '_'):
                    i += 1
                if i < n and s[i] == '(':                    # include the balanced call: `map.get(k)` is
                    depth = 0                                # one operand (needed so a cast on a raw-Map
                    while i < n:                             # .get() value unboxes -- see map-get-inserts).
                        if s[i] == '(':
                            depth += 1
                        elif s[i] == ')':
                            depth -= 1
                            if depth == 0:
                                i += 1
                                break
                        i += 1
            else:
                break
        if s[i:i + 2] in ('++', '--'):
            i += 2
        return s[start:i], i
    return '', start


_CAST_HEAD = re.compile(r'\(\s*([A-Za-z_]\w*)\s*(\[\s*\])?\s*\)')


def _rewrite_this_ident_eq(s, scope):
    """`this == o` / `o == this` where o is an Object-typed var (a JObject boxing the arg) -> j2xIdentEq
    (Java reference identity: is o boxing `this`?). A raw `this == someJObject` won't compile -- a typed
    pointer can't be compared to a JObject, and an operator== can't bridge two pointers (C2803)."""
    if 'this' not in s or ('==' not in s and '!=' not in s):
        return s

    def _obj(ident):
        jt = scope.get(ident)
        return jt is not None and not jt.is_array and jt.name == 'Object'

    def r1(m):
        op, ident = m.group(1), m.group(2)
        if not _obj(ident):
            return m.group(0)
        call = 'j2xIdentEq(this, %s)' % ident
        return call if op == '==' else '!' + call
    s = re.sub(r'\bthis\s*(==|!=)\s*([A-Za-z_]\w*)', r1, s)

    def r2(m):
        ident, op = m.group(1), m.group(2)
        if not _obj(ident):
            return m.group(0)
        call = 'j2xIdentEq(this, %s)' % ident
        return call if op == '==' else '!' + call
    return re.sub(r'\b([A-Za-z_]\w*)\s*(==|!=)\s*this\b', r2, s)


def _rewrite_jobject_casts(s, scope, resolver=None):
    """Lower casts whose operand is a JObject (Object var / Object[] element / raw-map value), which
    the generic _cast pass + JObject's conversion operators can't always handle:
      (String)obj      -> (obj).str()            (avoids the operator JavaString()/T*-char* ambiguity)
      (String[])obj    -> (obj).strArray()       ((String[]) is not a _CAST-handled spelling)
      (int[])obj       -> (JArray<int >)(obj)     (JObject::operator JArray<T>() unbox; (int[]) is not
                                                   valid C++ and _CAST doesn't match a `[]` target)
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
        kind = _expr_object_kind(operand, scope, resolver)
        rewritten = None
        if is_arr:
            if typ == 'String':
                rewritten = ('(%s).strArray()' % operand) if kind else operand
            elif kind and resolver is not None:
                # non-String array cast of a JObject: unbox to JArray<elem> via operator JArray<T>()
                rewritten = '(%s)(%s)' % (resolver.cpp_type(ir.JType(typ, array_dims=1), 'local'), operand)
        elif kind:
            if typ == 'String':
                rewritten = '(%s).str()' % operand
            elif typ in _BOXED_WRAPPERS:
                rewritten = operand          # drop the wrapper; operand is a self-contained primary
            elif typ in typemap.CONTAINERS and resolver is not None:
                # (List)/(Map)/(Set) on a JObject (a raw-map value that holds a boxed container): unbox
                # via operator JList/JMap/JSet -- NOT dropped, else a chained `.add()`/`.get()` lands on
                # the JObject (SoundPool's ((List)map.get(k)).add(x)).
                rewritten = '(%s)(%s)' % (resolver.cpp_type(ir.JType(typ), 'local'), operand)
        if rewritten is None:
            out.append(s[i])
            i += 1
        else:
            out.append(_rewrite_jobject_casts(rewritten, scope, resolver))   # recurse (nested casts in operand)
            i = end
    return ''.join(out)


_CHAIN_ROOT = re.compile(r'[A-Za-z_]\w*::s_\w+|[A-Za-z_]\w*')
_CHAIN_LINK = re.compile(r'\s*(->|\.)\s*([A-Za-z_]\w*)')
_NEW_ROOT = re.compile(r'new\s+([A-Za-z_]\w*)\s*\(')   # `new Type(` scalar ctor (not `new T[`)
_PAREN_NEW_ROOT = re.compile(r'\(\s*new\s+([A-Za-z_]\w*)\s*\(')   # `(new Type(` already-parenthesized
_CAST_ROOT = re.compile(r'\(\s*\(\s*([A-Za-z_]\w*)\s*(\*?)\s*\)')  # `((Type*)EXPR)` / `((Type)EXPR)` root
# a method-CALL root: `Class::meth(` (static) or bare `meth(` (implicit this). The `recv->meth(` case
# is already handled by _CHAIN_ROOT + walk_links (recv is a typed root). group1=Class or None, meth=g3.
_CALL_ROOT = re.compile(r'(?:([A-Za-z_]\w*)\s*(::)\s*)?([A-Za-z_]\w*)\s*\(')




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
    """While a balanced [index] follows and cur_jt is subscriptable (an ARRAY, or a list/map
    CONTAINER whose element type is known), consume it and step into the element type -- so
    `arr[i].member` / `jlist[i].member` types the indexed value as the element (a pointer for
    reference types, so the following `.` becomes `->`). The index EXPRESSION is itself
    chain-resolved (it may contain its own pointer chains, e.g. arr[base->idx])."""
    while cur_jt is not None and i < len(s) and s[i] == '[':
        elem = cur_jt.element() if cur_jt.is_array else _container_element(cur_jt)
        if elem is None:                      # not subscriptable / element type unknown -> stop
            break
        close = _balanced_bracket(s, i)
        if close < 0:
            break
        out.append('[' + _resolve_chains(s[i + 1:close], scope, resolver, current) + ']')
        i = close + 1
        cur_jt = elem
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
        # FIXED BUG string-dot-mangled: copy string/char literals VERBATIM -- their contents must never be
        # treated as pointer-member chains. A field named `options` (a GameSettings*) made walk_links upgrade
        # the '.' inside the literal "options.title" to "options->title", so translateKey() looked up a
        # nonexistent key and the Options screen showed raw "options->title"/"options->video" labels.
        if s[i] == '"' or s[i] == "'":
            q = s[i]; j = i + 1
            while j < n:
                if s[j] == '\\':
                    j += 2; continue
                if s[j] == q:
                    j += 1; break
                j += 1
            out.append(s[i:j]); i = j; continue
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
        # `((Type*)EXPR)` cast root -> the parenthesized value is a pointer; walk the fluent chain
        # off it as a pointer so a method RESULT deeper in (`...)->getChildNodes().item(0)`) types too.
        cr = _CAST_ROOT.match(s, i) if at_boundary else None
        if cr:
            close = _balanced_paren(s, i)              # the wrapping '(' at i
            tname, star = cr.group(1), cr.group(2)
            if close >= 0 and tname != 'void' and (star or resolver.is_pointer(ir.JType(tname))):
                j = close + 1
                while j < n and s[j] == ' ':
                    j += 1
                if j < n and (s[j] == '.' or s[j:j + 2] == '->'):   # a chain link actually follows
                    inner = _resolve_chains(s[i + 1:close], scope, resolver, current)
                    out.append('(' + inner + ')')
                    cur_jt, is_ptr = ir.JType(tname), True
                    k, cur_jt, is_ptr = _consume_index(s, close + 1, out, cur_jt, is_ptr, resolver, scope, current)
                    i = walk_links(k, cur_jt, is_ptr)
                    continue
        # a METHOD-CALL root whose result is a pointer: `Class::m(...)`, `recv->m(...)`, `this->m(...)`,
        # or a bare `m(...)` (implicit this). Typing the result lets walk_links upgrade the WHOLE
        # subsequent chain -- e.g. `func(...)->texturePackList.availableTexturePacks()` (call -> field ->
        # method) or `x.entrySet().iterator().next()` (iterator() returns Iterator*, so .next() -> ->).
        cm = _CALL_ROOT.match(s, i) if at_boundary else None
        if cm:
            crecv, csep, cmeth = cm.group(1), cm.group(2), cm.group(3)
            op = cm.end() - 1
            cclose = _balanced_paren(s, op)
            if cclose >= 0:
                j = cclose + 1
                while j < n and s[j] == ' ':
                    j += 1
                if j < n and (s[j] == '.' or s[j:j + 2] == '->'):     # a member link actually follows
                    owner = (crecv if csep == '::' else
                             current if (crecv is None or crecv == 'this') else
                             (scope[crecv].name if crecv in scope else None))
                    rt = resolver.member_method_type(owner, cmeth) if owner else None
                    if rt is None and owner and (owner, cmeth) in _PTR_FACTORY_METHODS:
                        rt = ir.JType(owner)         # stub factory (Runtime::getRuntime()) returns a pointer to its type
                    if rt is not None and resolver.is_pointer(rt):
                        cargs = _resolve_chains(s[op + 1:cclose], scope, resolver, current)
                        out.append((('%s%s' % (crecv, csep)) if crecv else '') + '%s(%s)' % (cmeth, cargs))
                        k, cur_jt, is_ptr = _consume_index(s, cclose + 1, out, rt, True, resolver, scope, current)
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


# Stdlib STATIC FACTORY methods that return a POINTER (a shared singleton or a fresh object) whose
# OWNING class is a runtime STUB, not a parsed-corpus class -- so resolver.member_method_type() can't
# type the result and a following `.m()` would wrongly stay `.` instead of `->`. The canonical case is
# `Runtime.getRuntime().maxMemory()` (Runtime.h: `static Runtime* getRuntime()`). (class, method) pairs;
# the result is treated as a pointer to `class`. Extend as other stub factories surface.
_PTR_FACTORY_METHODS = frozenset({
    ('Runtime', 'getRuntime'),
})

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
                if not ptr and (owner, meth) in _PTR_FACTORY_METHODS:
                    ptr = True                       # stub factory (Runtime::getRuntime()) returns a pointer
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


# Free helpers (JavaRT.h) that RETURN Iterator* -- the java `coll.iterator()` idiom was lowered to
# these in step 0e. A `.member` directly on their result must be `->` (the result is a pointer), and
# _resolve_call_chains only matches receiver.method/new heads, not a bare `helper(...)` call.
_ITER_HELPERS = ('j2xIter', 'j2xValues', 'j2xKeys')


def _deref_iter_helpers(s):
    """`j2xIter(coll).next()` -> `j2xIter(coll)->next()` (Iterator* result). Balanced-paren scan so
    the collection arg may itself be a chain/call."""
    for fn in _ITER_HELPERS:
        start = 0
        while True:
            idx = s.find(fn + '(', start)
            if idx < 0:
                break
            if idx > 0 and (s[idx - 1].isalnum() or s[idx - 1] == '_'):
                start = idx + len(fn)                  # part of a longer identifier -- skip
                continue
            close = _balanced_paren(s, idx + len(fn))
            if close < 0:
                break
            j = close + 1
            while j < len(s) and s[j] == ' ':
                j += 1
            if j < len(s) and s[j] == '.':
                s = s[:close + 1] + '->' + s[j + 1:]
            start = close + 1
    return s


_NEW_ALLOC = re.compile(r'new\s+[A-Za-z_]\w*\s*\(')


def _wrap_new_alloc_chains(s):
    """`new X(...)->m` / `new X(...).m` parses in C++ as `new (X()->m)` -- the member access binds to
    the CALL, not the allocation. Parenthesize the allocation: `(new X(...))->m`. Idempotent (skips a
    `new` that is already wrapped as `(new ...)`)."""
    out, i, n = [], 0, len(s)
    while i < n:
        m = _NEW_ALLOC.match(s, i)
        if m:
            close = _balanced_paren(s, m.end() - 1)
            if close >= 0:
                j = close + 1
                while j < n and s[j] == ' ':
                    j += 1
                if j < n and (s[j] == '.' or s[j:j + 2] == '->'):
                    prev = out[-1] if out else ''
                    if prev != '(':                       # not already `(new ...)`
                        out.append('(' + s[i:close + 1] + ')')
                        i = close + 1
                        continue
        out.append(s[i]); i += 1
    return ''.join(out)


# `X.class` for a wrapper/String maps to the C++ type so `typeid(X)` is valid (the wrappers are
# NAMESPACES, not classes, so `typeid(Byte)` fails). Corpus classes pass through unchanged.
_CLASS_LIT_TYPE = {'Byte': 'signed char', 'Short': 'short', 'Integer': 'int', 'Long': 'long long',
                   'Float': 'float', 'Double': 'double', 'Character': 'char', 'Boolean': 'bool',
                   'String': 'JavaString', 'Object': 'JObject'}
_USHR = re.compile(r'(\w+(?:\s*(?:->|\.)\s*\w+|\s*\[[^\]]*\])*|\([^()]*\))\s*>>>(?!=)\s*(\w+|\([^()]*\))')


def _rewrite_ushr(s):
    """Java `a >>> b` (unsigned/logical right shift) -> j2xUshr(a, b). C++ has no `>>>`, and `>>` on a
    signed value is arithmetic. Handles the simple-operand forms the corpus uses (var/member/index/
    paren >>> number/var/paren); repeated to catch `a >>> b >>> c`."""
    for _ in range(4):
        new = _USHR.sub(r'j2xUshr(\1, \2)', s)
        if new == s:
            break
        s = new
    return s


def fix_enum_access(s, resolver):
    """Refine enum member access after `Enum.member` -> `Enum::member`. Shared by the statement
    translator and the static-initializer translator (source._static_ref), since enum access appears
    in both method bodies and static initializers (the MappingHelper `values().length` tables).
      const-only enum (maps to int; the member value IS its 0-based ordinal):
        `EnumX::values().length` -> `EnumX::_J2X_COUNT`
        `EnumX::MEMBER.ordinal()` -> `EnumX::MEMBER`     (ordinal() is the identity)
        `<call>().ordinal()`      -> `<call>()`           (a const-enum-returning call is int; a
                                                           typed-enum result is a pointer and the chain
                                                           resolver already made it `)->ordinal()`)
      typed enum: the singleton constant is a POINTER, so `EnumX::MEMBER.method(` -> `->method(`."""
    for e in (set(resolver.enums) - set(resolver.typed_enums)):
        s = re.sub(re.escape(e) + r'::values\(\)\s*\.\s*length\b', e + '::_J2X_COUNT', s)
        s = re.sub(r'\b' + re.escape(e) + r'::(\w+)\s*\.\s*ordinal\s*\(\s*\)', e + r'::\1', s)
    for e in set(resolver.typed_enums):
        s = re.sub(r'\b' + re.escape(e) + r'::(\w+)\s*\.\s*(\w+\s*\()', e + r'::\1->\2', s)
    s = re.sub(r'\)\s*\.\s*ordinal\s*\(\s*\)', ')', s)   # const-enum method result (int) -- ordinal is identity
    return s


def _translate_line(s, ptr_names, static_set, enum_set, resolver, scope, parent, current,
                    own_statics, ptr_locals, box_locals=frozenset()):
    # 0. `synchronized(lock) {` -> keep the block, drop the JVM monitor -- BUT tag it. The port is
    #    NOT fully single-threaded (network reader/writer threads, the mesher pool), so a dropped
    #    monitor on cross-thread state is a race (the hand-fixed sendQueueLock reader/writer race).
    #    J2X-REVIEW[synchronized-dropped] makes every one greppable so a human confirms it's not shared.
    def _sync(mm):
        return '/* J2X-REVIEW[synchronized-dropped]: was synchronized(%s) -- add a lock if this state '\
               'is touched by another thread (network/pool) */ ' % mm.group(1).strip()
    s = re.sub(r'\bsynchronized\s*\(([^){]*)\)\s*', _sync, s)
    s = _rewrite_ushr(s)                             # Java `a >>> b` (unsigned shift) -> j2xUshr(a, b)
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
    # 0d''. `this == <Object var>` (Java reference identity in equals) -> j2xIdentEq(this, var). Done
    #       early, while `this` is still bare (before `this.` -> `this->`) and before the comparison is
    #       otherwise touched.
    if 'this' in s and ('==' in s or '!=' in s):
        s = _rewrite_this_ident_eq(s, scope)
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
    # 0f'. for-init OBJECT declaration: `for(MCHashEntry var3 = this->slots[i]; ...)`. Same blind spot
    #      as arrays -- rewrite the type to its C++ spelling so a pointer class gets its `*` (the value
    #      spelling can't hold null and breaks `var3 != NULL` / `var3->nextEntry`).
    def _forinit_obj(mm):
        base = _decl_base(mm.group(1))
        if base in _KEYWORDS or mm.group(2) in _KEYWORDS:
            return mm.group(0)
        cpp = resolver.cpp_type(javaparse.parse_type(mm.group(1)), 'local')
        return 'for (%s %s =' % (cpp, mm.group(2))
    s = re.sub(r'\bfor\s*\(\s*(?:final\s+)?([A-Za-z_][\w.]*)\s+([A-Za-z_]\w*)\s*=', _forinit_obj, s)
    # 1. local declaration: rewrite the leading Java type to its C++ spelling. A value-type
    #    local inferred to be a nullable reference (ptr_locals) gets a trailing '*'.
    m = _DECL.match(s)
    if m and _decl_base(m.group(1)) not in _KEYWORDS and m.group(2) not in _KEYWORDS:
        if m.group(2) in box_locals:
            cpp = 'JObject'          # nullable boxed wrapper -> JObject (holds null + boxes/unboxes)
        else:
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
    # 1e. `new String(args)` -> `JavaString(args)`: java.lang.String is a VALUE type (JavaString), never
    #     a heap object. Its ctors (from a byte[]/char[] [+ charset], a copy, or empty) live on JavaString.
    s = sub_outside_literals(re.compile(r'\bnew\s+String\s*\('), 'JavaString(', s)
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
    s = _MATH.sub(lambda mm: typemap.MATH_API_EMIT.get(mm.group(1), mm.group(0)), s)
    # 5. KnownClass.member / Enum.CONST -> :: static access
    def _static(mm):
        recv, mem, paren = mm.group(1), mm.group(2), mm.group(3)
        if recv not in static_set:
            return mm.group(0)
        if mem == 'class' and not paren:             # Java class literal X.class -> STABLE Class*
            return 'j2xClassOf(typeid(%s))' % _CLASS_LIT_TYPE.get(recv, recv)   # Byte.class->typeid(signed char)
        if paren:                                  # static method call
            return '%s::%s%s' % (recv, resolver.renamed_method(recv, mem), paren)
        if recv in enum_set:                       # enum constant (no s_ prefix)
            return '%s::%s' % (recv, naming.enum_member(mem))
        return '%s::%s' % (recv, naming.static_field_name(mem))   # static field
    s = _MEMBER.sub(_static, s)
    s = fix_enum_access(s, resolver)                 # 5a1. const/typed enum ordinal()/values() refinements
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
    # 6d'. Iterator* free-helper results (j2xIter/j2xValues/j2xKeys from step 0e): `.next()` -> `->next()`.
    s = _deref_iter_helpers(s)
    # 6d''. `.iterator()` (the container MEMBER, e.g. on a chained `map.entrySet().iterator()`) always
    #       returns Iterator* -- so the call after it derefs with `->`, which the pointer-root chain pass
    #       misses when the receiver is a value (entrySet() -> JList). Universal: `.iterator().` -> `->`.
    if '.iterator().' in s:
        s = re.sub(r'\.iterator\(\)\s*\.(?=[A-Za-z_])', '.iterator()->', s)
    # 6f. `recv->equals(arg)` on a corpus reference type with no equals() -> `(recv == arg)` (Object
    #     identity). After the arrow passes so the pointer receiver is `recv->`. Value/String equals
    #     is handled by _translate_calls (step 6).
    s = _rewrite_noequals_identity(s, scope, resolver)
    # 6g. `arr = NULL;` on an array-typed field/local -> `arr = JArray<T>();` (a JArray value can't be NULL).
    s = _rewrite_array_null_assign(s, scope, resolver)
    # 6e. JObject-operand casts (Object var / Object[] element / raw-map value): lower BEFORE the
    #     generic cast pass -- (String)obj -> (obj).str(), (String[])obj -> (obj).strArray(),
    #     (Integer/Character/...)obj -> (obj) (JObject operators/unbox-methods do the rest).
    s = _rewrite_jobject_casts(s, scope, resolver)
    # 7. cast translation: reference type -> pointer cast ((Block)x -> (Block*)x);
    #    primitive whose C++ spelling differs -> the C++ spelling ((byte)x -> (unsigned char)x)
    def _cast(m):
        t = m.group(1)
        # Boxed-wrapper cast ((Integer)/(Byte)/(Short)/(Float)/...): DROP it. The operand is a
        # JObject (a raw-map value, an Object[] element, or an Object-returning call) whose
        # operator int()/... converts it in a primitive context, and whose trailing
        # .intValue()/.byteValue()/.floatValue()/... unbox binds directly. Lowering the wrapper to
        # a primitive spelling ((Integer)->(int)) instead produced `((int)obj).intValue()` -- an
        # unbox call on a raw primitive (no such member). Dropping is also harmless when the operand
        # is already a primitive (a redundant Java widening cast). _rewrite_jobject_casts handles the
        # simple identifier-rooted operands earlier; this catches call/static-rooted ones it can't.
        if t in typemap.BOXED_VALUE_TYPES:
            return ''
        # interface cast to a collection ((List)/(Map)/(Set)/(Collection)): DROP it. `List`/`Map` are
        # Java interface NAMES with no C++ type, and the operand is either already the value container
        # (redundant cast) or a JObject that unboxes to it via the JObject ctor added to JList/JMap/JSet.
        if t in typemap.CONTAINERS:
            return ''
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
    # 7a. distribute a pointer-cast over a conditional's arms: `(Base*)(c ? new A() : new B())` ->
    #     `(c ? (Base*)(new A()) : (Base*)(new B()))`. C++03 can't type a conditional whose arms are
    #     unrelated pointer types (Java picks the least-upper-bound); casting each arm to the target
    #     pointer makes both convertible. No-op unless a `*)` cast wraps a parenthesized conditional.
    s = distribute_cast_into_ternaries(s)
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
    # 7c2. `new Integer(x)` / `new Byte(x)` / ... (boxed-wrapper CONSTRUCTOR) -> JObject(x): the wrapper
    #      types are namespaces (Integer::valueOf, ...), not classes, so `new Byte(x)` boxes into the
    #      universal JObject (dataWatcher->addObject(id, new Byte((signed char)v)), NBT tag values, ...).
    s = re.sub(r'\bnew\s+(?:Integer|Long|Float|Double|Boolean|Byte|Short|Character)\s*\(', 'JObject(', s)
    s = s.replace(chr(39) + chr(92) + "u0000" + chr(39), chr(39) + chr(92) + "0" + chr(39))
    #      only one the corpus uses is NUL -> the standard `'\0'` escape.
    s = s.replace(chr(39) + chr(92) + "u0000" + chr(39), chr(39) + chr(92) + "0" + chr(39))
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
        is_obj = _expr_object_kind(lhs, scope, resolver) is not None
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
    # 13. `list.add(i, x)` (indexed insert) renamed to `.push_back(i, x)` by ANY pass above -- but
    #     std::vector::push_back takes ONE arg. Rewrite the 2-arg form to JList::add(int, const T&).
    if '.push_back(' in s:
        s = _fix_indexed_add(s)
    # 14. `new X(...)` where X derives from a value container -> `(*(new X(...)))` (slice to the base
    #     container value; argo's J_JsonNodeList : List / J_JsonObjectNodeList : Map).
    if 'new ' in s:
        s = _deref_container_subclass_new(s, resolver)
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


def _recv_qual_start(s, name_start):
    """Walk back from a getter name at `name_start` over any receiver qualifier chain
    (`NetworkManager::getX`, `this->obj.getX`) so the whole receiver is hoisted together,
    not left glued to the substituted temp. Returns the index the qualified receiver starts at."""
    i = name_start
    while True:
        if i >= 2 and s[i - 2:i] in ('::', '->'):
            j = i - 2
        elif i >= 1 and s[i - 1] == '.':
            j = i - 1
        else:
            break
        k = j
        while k > 0 and (s[k - 1].isalnum() or s[k - 1] == '_'):
            k -= 1
        if k == j:                                   # a connector with no identifier before it -> stop
            break
        i = k
    return i


def _hoist_nullable_getter(s, return_type, resolver):
    """If s contains a KNOWN_NULLABLE_GETTER(args)->deref not caught by the standalone
    patterns, hoist the getter into a temp variable and null-guard the dereference."""
    # A control-flow header / block opener is NOT a hoistable value statement: wrapping it in a
    # `{ tmp = ...; if (tmp) { <line> } }` block strands the trailing `{` and mangles the condition
    # (the NetworkManager `if(getReadThread(x)->isAlive()) {` corruption). Leave it un-hoisted -- the
    # raw `recv->call()` compiles and matches Java's own NPE-on-null semantics.
    head = s.lstrip()
    if s.rstrip().endswith('{') or head.startswith((
            'if(', 'if ', 'while(', 'while ', 'for(', 'for ', 'switch(', 'switch ',
            '} else', 'else ', 'else{', 'do ', 'do{')):
        return None
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

        qstart = _recv_qual_start(s, m.start())      # include NetworkManager:: / this->obj. qualifier
        recv_expr = s[qstart:close + 1]
        tmp = '_j2x_%s' % gn.replace('get', '', 1)[:20]
        tmp = tmp[0].lower() + tmp[1:]
        new_line = s[:qstart] + tmp + s[close + 1:]

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
    box_locals = _boxed_nullable_locals(method, scope, resolver)   # nullable boxed wrappers -> JObject
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
    # A param or local that shares a static field's name SHADOWS it in this method (Java scoping),
    # so a bare reference is the LOCAL, not the static -- don't s_-prefix it. (FIXED BUG
    # static-shadowed-by-param: Frustum.normalizePlane(float[][] frustum, ...) -- the param `frustum`
    # collided with the static singleton field `frustum` and every `frustum[side][0]` became
    # `s_frustum[side][0]`, indexing the Frustum* singleton -> "Frustum has no operator[]".)
    own_statics -= (set(p.name for p in method.params) | _local_names(method))
    out = []
    for raw in inner_lines(method.body):
        s = raw.strip()
        if not s or s.startswith('//') or s.startswith('*') or s.startswith('/*'):
            if s:
                out.append(s)
            continue
        line = _translate_line(s, ptr_names, static_set, enum_set, resolver, scope,
                               cls.parent, cls.name, own_statics, ptr_locals, box_locals)
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
    out = [_wrap_new_alloc_chains(l) for l in out]   # `new X()->m` -> `(new X())->m` (C++ precedence)
    out = [_clarify_precedence(l) for l in out]   # C4554: parenthesize shift/bitwise sub-expressions
    out = _drop_unused_catch_vars(out)            # `catch (Type* e){}` with e unused -> `catch (Type*)` (C4101)
    out = _wrap_switch_cases(out)                 # `{ }` per case body so decls don't cross a case label (C2360/C2361)
    return out
