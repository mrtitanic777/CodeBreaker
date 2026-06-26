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
# new T[n] (1-D, not already value-init) -> capture type, optional *, size
_NEW_ARRAY = re.compile(r'\bnew\s+([\w:]+)\s*(\*?)\s*\[([^\]]*)\](?!\s*[\[(])')
# x instanceof Type  (x may be a var or a this->field chain)
_INSTANCEOF = re.compile(r'([A-Za-z_]\w*(?:->[A-Za-z_]\w*)*)\s+instanceof\s+([A-Za-z_]\w*)')
# receiver.member  with optional opening paren (method call)
_MEMBER = re.compile(r'\b([A-Za-z_]\w*)\.([A-Za-z_]\w*)(\s*\()?')
_CALL = re.compile(r'\b(\w+)\.(\w+)\s*\(')
# a cast `(Type)` immediately followed by an operand (word or '(') -- not a grouping
_CAST = re.compile(r'\(([A-Za-z_]\w*)\)(?=\s*[\w(])')

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
    referenced = set()
    for ln in lines:
        for m in _LABEL_JUMP.finditer(ln):
            referenced.add(m.group(2))
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
        cont_at.setdefault(close, []).append('%s_cont:;' % name)
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


def _category(jtype):
    if jtype is None:
        return None
    n = jtype.name
    if n in _LIST_TYPES:
        return 'list'
    if n in _MAP_TYPES:
        return 'map'
    if n in _SET_TYPES:
        return 'set'
    if n == 'String':
        return 'string'
    return None


def _container_call(cat, name, args_str):
    """STL replacement for a container method call `<recv>.name(args)` -- returns the text that
    REPLACES `.name(args)` (the receiver is already emitted), or None to leave it alone. Used by
    the chain walker so container methods map on FOREIGN/chained receivers too, not just simple
    in-scope names. Containers are held by value, so the result attaches with '.'/[]."""
    if cat is None:
        return None
    args = javaparse._split_top_commas(args_str) if args_str.strip() else []
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
        rep = fn(javaparse._split_top_commas(s[op + 1:close]))
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
        if jt is None or jt.is_array or resolver.is_pointer(jt) or jt.name in typemap.CONTAINERS:
            continue                                  # already a pointer/array, or a value container
        nm = re.escape(name)
        if (re.search(r'\b%s\s*=\s*(?:null|NULL)\b' % nm, text)
                or re.search(r'\b%s\s*(?:==|!=)\s*(?:null|NULL)\b' % nm, text)
                or re.search(r'\b(?:null|NULL)\s*(?:==|!=)\s*%s\b' % nm, text)
                or re.search(r'\b%s\s*=\s*new\b' % nm, text)):
            forced.add(name)
    return forced


_CHAIN_ROOT = re.compile(r'[A-Za-z_]\w*::s_\w+|[A-Za-z_]\w*')
_CHAIN_LINK = re.compile(r'\s*(->|\.)\s*([A-Za-z_]\w*)')


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


def _consume_index(s, i, out, cur_jt, is_ptr, resolver):
    """While cur_jt is an array and a balanced [index] follows, consume it and step into the
    element type (so `arr[i].member` types `arr[i]` as the element)."""
    while cur_jt is not None and cur_jt.is_array and i < len(s) and s[i] == '[':
        close = _balanced_bracket(s, i)
        if close < 0:
            break
        out.append(s[i:close + 1])
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
    out = []
    i, n = 0, len(s)
    while i < n:
        prev = out[-1][-1] if (out and out[-1]) else ''
        at_boundary = (prev == '') or (not prev.isalnum() and prev not in '_>.:')
        m = _CHAIN_ROOT.match(s, i) if at_boundary else None
        if not m:
            out.append(s[i]); i += 1; continue
        tok = m.group(0)
        rt = _chain_root(tok, scope, resolver, current)
        out.append(tok)
        i = m.end()
        if rt is None:
            continue
        cur_jt, is_ptr = rt
        i, cur_jt, is_ptr = _consume_index(s, i, out, cur_jt, is_ptr, resolver)
        while cur_jt is not None:                      # walk the chain links
            lm = _CHAIN_LINK.match(s, i)
            if not lm:
                break
            sep, name, after = lm.group(1), lm.group(2), lm.end()
            k = after
            while k < n and s[k] == ' ':
                k += 1
            is_call = k < n and s[k] == '('
            args = None
            seg_end = after
            if is_call:
                close = _balanced_paren(s, k)
                if close < 0:
                    break
                args = _resolve_chains(s[k + 1:close], scope, resolver, current)
                seg_end = close + 1
                crepl = _container_call(_category(cur_jt), name, args)
                if crepl is not None:                 # STL container method on a chained receiver
                    out.append(crepl)
                    i = seg_end
                    break                             # result type (raw element) not tracked
                name = resolver.renamed_method(cur_jt.name, name)   # field/method collision -> name_
            out.append(('->' if is_ptr else sep) + name)   # upgrade only when proven pointer
            if is_call:
                out.append('(' + args + ')')
            i = seg_end
            cur_jt = (resolver.member_method_type(cur_jt.name, name) if is_call
                      else resolver.member_field_type(cur_jt.name, name))
            if cur_jt is None:
                break                                 # type unknown -> stop tracking this chain
            is_ptr = resolver.is_pointer(cur_jt)
            i, cur_jt, is_ptr = _consume_index(s, i, out, cur_jt, is_ptr, resolver)
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
            #   (e.g. the source already wrote `(new X(...)).start()`)
            extra_close = j < len(s) and s[j] == ')'
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
    # 0. `synchronized(lock) {` -> `{`  (no JVM monitors on the single-threaded console port;
    #    drop the lock, keep the guarded block)
    s = re.sub(r'\bsynchronized\s*\([^){]*\)\s*', '', s)
    # 1. local declaration: rewrite the leading Java type to its C++ spelling. A value-type
    #    local inferred to be a nullable reference (ptr_locals) gets a trailing '*'.
    m = _DECL.match(s)
    if m and _decl_base(m.group(1)) not in _KEYWORDS and m.group(2) not in _KEYWORDS:
        cpp = resolver.cpp_type(javaparse.parse_type(m.group(1)), 'local')
        if m.group(2) in ptr_locals and not cpp.endswith('*'):
            cpp += '*'
        s = cpp + s[m.end(1):]
        # a value-container local initialized with `new ArrayList()` -> just default-construct
        # (Java collections are by-value std::vector/map/set; dropping avoids an element-type
        # mismatch between the typed LHS and a raw `new ArrayList()`).
        cm = re.match(r'^(.*\b%s)\s*=\s*new\s+([A-Z]\w*)(?:<[^;{}]*>)?\s*\([^;{}]*\)\s*;\s*$'
                      % re.escape(m.group(2)), s)
        if cm and javaparse.parse_type(cm.group(2)).name in typemap.CONTAINERS:
            s = cm.group(1) + ';'
    # 1b. any other `new <Container>(...)` (field/element assignment, for-init, ...) -> a
    #     value temporary, since collections are held by value
    def _new_container(mm):
        jt = javaparse.parse_type(mm.group(1) + (mm.group(2) or ''))
        return resolver.cpp_type(jt, 'local') + '()' if jt.name in typemap.CONTAINERS else mm.group(0)
    s = re.sub(r'\bnew\s+([A-Z]\w*)(<[^;{}]*?>)?\s*\([^;{}]*?\)', _new_container, s)
    # 2. this.  -> this->
    s = re.sub(r'\bthis\.', 'this->', s)
    # 3. <pointerName>.  -> <pointerName>->
    for name in ptr_names:
        s = re.sub(r'\b' + re.escape(name) + r'\.', name + '->', s)
    # 3b. index-result is a pointer: <refArray>[i].member -> <refArray>[i]->member
    s = re.sub(r'\b(\w+)(\[[^\]]*\])\.',
               lambda mm: ('%s%s->' % (mm.group(1), mm.group(2)))
               if _is_nullable_array(mm.group(1), scope, resolver) else mm.group(0), s)
    # 4. Math.fn -> C math (sqrtf, sinf, ...)
    s = _MATH.sub(lambda mm: typemap.MATH_API.get(mm.group(1), mm.group(0)), s)
    # 5. KnownClass.member / Enum.CONST -> :: static access
    def _static(mm):
        recv, mem, paren = mm.group(1), mm.group(2), mm.group(3)
        if recv not in static_set:
            return mm.group(0)
        if mem == 'class' and not paren:             # Java class literal X.class -> stub (no RTTI port)
            return '((void*)0)'
        if paren:                                  # static method call
            return '%s::%s%s' % (recv, resolver.renamed_method(recv, mem), paren)
        if recv in enum_set:                       # enum constant (no s_ prefix)
            return '%s::%s' % (recv, mem)
        return '%s::%s' % (recv, naming.static_field_name(mem))   # static field
    s = _MEMBER.sub(_static, s)
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
    # 6b. cast-result access: ((Type)x).m -> ((Type*)x)->m  (a cast to a reference type yields a pointer)
    s = re.sub(r'\(\(([A-Za-z_]\w*)\)([^()]+)\)\s*\.',
               lambda mm: ('((%s*)%s)->' % (mm.group(1), mm.group(2)))
               if resolver.is_pointer(ir.JType(mm.group(1))) else mm.group(0), s)
    # 6c. chained member access: receiver sub-expression that resolves to a pointer (static
    #     reference field / pointer field of a typed receiver) -> '.' becomes '->'
    s = _resolve_chains(s, scope, resolver, current)
    # 6d. method/ctor RESULT chains: call(...).m -> call(...)->m / (new T(...))->m
    s = _resolve_call_chains(s, scope, resolver, current)
    # 7. cast translation: reference type -> pointer cast ((Block)x -> (Block*)x);
    #    primitive whose C++ spelling differs -> the C++ spelling ((byte)x -> (unsigned char)x)
    def _cast(m):
        t = m.group(1)
        if resolver.is_pointer(ir.JType(t)):
            return '(%s*)' % t
        cpp = typemap.JAVA_TO_CPP.get(t)
        if cpp and cpp != t and t in typemap.PRIMITIVES:
            return '(%s)' % cpp
        return m.group(0)
    s = _CAST.sub(_cast, s)
    # 8. Java null literal -> NULL
    s = re.sub(r'\bnull\b', 'NULL', s)
    # 9. new T[n] -> JArray<elem>(n) (length-carrying, bounds-checked, value-initialized)
    def _new_arr(m):
        cpp = resolver.cpp_type(ir.JType(m.group(1), array_dims=1), 'local')   # JArray<elem>
        size = m.group(3).strip()
        return '%s(%s)' % (cpp, size) if size else '%s()' % cpp
    s = _NEW_ARRAY.sub(_new_arr, s)
    # 10. super.method(...) -> Parent::method(...)  (super(...) ctor calls handled in source.py)
    if parent:
        s = re.sub(r'\bsuper\.(\w+)', parent + r'::\1', s)
    # 11. x instanceof Type -> dynamic_cast<Type*>(x) != NULL  (RTTI / -GR is on)
    s = _INSTANCEOF.sub(lambda m: '(dynamic_cast<%s*>(%s) != NULL)' % (m.group(2), m.group(1)), s)
    # 12. Java string concatenation in a return -> ostringstream (C++ can't '+' a literal
    #     with non-strings; ostringstream::operator<< handles every operand type)
    rm = re.match(r'^return\s+(.+);\s*$', s)
    if rm:
        chain = _maybe_concat(rm.group(1).strip())
        if chain:
            s = '{ std::ostringstream _oss; _oss << %s; return _oss.str(); }' % chain
    return s


def _is_nullable_array(base, scope, resolver):
    """True if `base[...]` indexes a pointer-element array (registry list or a
    scope array of a reference type) -- i.e. a slot can be NULL."""
    stripped = base[2:] if base.startswith('s_') else base
    if stripped in typemap.KNOWN_NULLABLE:
        return True
    jt = scope.get(base) or scope.get(stripped)
    return bool(jt and jt.is_array and resolver.is_pointer(jt.element()))


def _guard_null_deref(s, scope, resolver):
    """Wrap a standalone list-indexed deref in a NULL check; return (line, guarded)."""
    m = _INDEX_DEREF_STMT.match(s)
    if not m:
        return s, False
    recv, base, call = m.group(1), m.group(2), m.group(3)
    if not _is_nullable_array(base, scope, resolver):
        return s, False
    return 'if (%s != NULL) { %s->%s; }' % (recv, recv, call), True


def translate_body(method, cls, resolver, ledger=None):
    """Return translated C++ body lines (no indentation; caller indents)."""
    scope = build_scope(cls, method)
    ptr_locals = _pointer_locals(method, scope, resolver)   # value locals used as nullable refs
    ptr_params = ownership.nullable_value_params(method, resolver)   # value-struct params used nullably
    ptr_names = sorted((set(n for n, jt in scope.items() if resolver.is_pointer(jt))
                        | ptr_locals | ptr_params),
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
        line, guarded = _guard_null_deref(line, scope, resolver)
        if guarded and ledger is not None:
            ledger.add(cls.name + '.cpp', Category.RISK,
                       'null-deref guard inserted before list-indexed call (an unregistered/NULL '
                       'slot would be a virtual call through NULL -> IP 0x0 crash)',
                       line=method.line, severity=Severity.MED, source='emit', rule='null-deref-guard')
        out.append(line)

    # a String return is by value (std::string); `return null` would construct a string
    # from a NULL char* (undefined behavior / crash) -- return an empty string instead.
    rt = method.return_type
    if rt is not None and rt.name == 'String' and not rt.is_array:
        out = [re.sub(r'\breturn\s+NULL\s*;', 'return std::string();', l) for l in out]

    before = out
    out = _translate_labels(out)                  # labeled break/continue -> goto (body-level)
    if ledger is not None and len(out) != len(before):
        ledger.add(cls.name + '.cpp', Category.WARNING,
                   'labeled break/continue translated to goto (verify the loop still terminates)',
                   line=method.line, severity=Severity.MED, source='emit', rule='label-goto')
    return out
