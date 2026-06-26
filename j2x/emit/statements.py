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
from . import naming

# a standalone statement `recv[idx]->call(...);` (or `.` form) -- group2 = base name
_INDEX_DEREF_STMT = re.compile(
    r'^((?:this->)?(?:\w+::)*(\w+)\[[^\]]*\])\s*(?:->|\.)\s*(\w+\([^;]*\))\s*;\s*$')

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
# receiver.member  with optional opening paren (method call)
_MEMBER = re.compile(r'\b([A-Za-z_]\w*)\.([A-Za-z_]\w*)(\s*\()?')
_CALL = re.compile(r'\b(\w+)\.(\w+)\s*\(')
# a cast `(Type)` immediately followed by an operand (word or '(') -- not a grouping
_CAST = re.compile(r'\(([A-Za-z_]\w*)\)(?=\s*[\w(])')

# (receiver-category, java method) -> C++ method, args preserved
_METHOD_RENAME = {
    ('container', 'add'): 'push_back',
    ('container', 'isEmpty'): 'empty',
    ('string', 'isEmpty'): 'empty',
}
# (category) -> java accessor that becomes recv[arg]
_INDEX_METHOD = {'container': 'get', 'string': 'charAt'}


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


def _category(jtype):
    if jtype is None:
        return None
    if jtype.name in typemap.CONTAINERS:
        return 'container'
    if jtype.name == 'String':
        return 'string'
    return None


def _translate_calls(s, scope):
    """Map container/String methods to STL, using each receiver's resolved type."""
    # method renames (args preserved): recv.add(...) -> recv.push_back(...), isEmpty -> empty
    def repl(m):
        recv, meth = m.group(1), m.group(2)
        new = _METHOD_RENAME.get((_category(scope.get(recv)), meth))
        return '%s.%s(' % (recv, new) if new else m.group(0)
    s = _CALL.sub(repl, s)
    # index accessors: vector.get(i) / String.charAt(i) -> recv[i]
    for recv, jt in scope.items():
        meth = _INDEX_METHOD.get(_category(jt))
        if not meth:
            continue
        token = '%s.%s(' % (recv, meth)
        while True:
            idx = s.find(token)
            if idx < 0:
                break
            op = idx + len(token) - 1                 # position of '('
            close = _balanced_paren(s, op)
            if close < 0:
                break
            s = s[:idx] + '%s[%s]' % (recv, s[op + 1:close]) + s[close + 1:]
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
    """name -> JType for params, the class's own fields, and locals declared in the body."""
    scope = {}
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


def _translate_line(s, ptr_names, static_set, enum_set, resolver, scope):
    # 1. local declaration: rewrite the leading Java type to its C++ spelling
    m = _DECL.match(s)
    if m and _decl_base(m.group(1)) not in _KEYWORDS and m.group(2) not in _KEYWORDS:
        cpp = resolver.cpp_type(javaparse.parse_type(m.group(1)), 'local')
        s = cpp + s[m.end(1):]
    # 2. this.  -> this->
    s = re.sub(r'\bthis\.', 'this->', s)
    # 3. <pointerName>.  -> <pointerName>->
    for name in ptr_names:
        s = re.sub(r'\b' + re.escape(name) + r'\.', name + '->', s)
    # 4. Math.fn -> C math (sqrtf, sinf, ...)
    s = _MATH.sub(lambda mm: typemap.MATH_API.get(mm.group(1), mm.group(0)), s)
    # 5. KnownClass.member / Enum.CONST -> :: static access
    def _static(mm):
        recv, mem, paren = mm.group(1), mm.group(2), mm.group(3)
        if recv not in static_set:
            return mm.group(0)
        if paren:                                  # static method call
            return '%s::%s%s' % (recv, mem, paren)
        if recv in enum_set:                       # enum constant (no s_ prefix)
            return '%s::%s' % (recv, mem)
        return '%s::%s' % (recv, naming.static_field_name(mem))   # static field
    s = _MEMBER.sub(_static, s)
    # 6. container/String methods -> STL (.add->.push_back, .get(i)->[i], .charAt(i)->[i], isEmpty->empty)
    s = _translate_calls(s, scope)
    # 7. cast to a reference type -> pointer cast: (Block)x -> (Block*)x
    s = _CAST.sub(lambda m: '(%s*)' % m.group(1)
                  if resolver.is_pointer(ir.JType(m.group(1))) else m.group(0), s)
    # 8. Java null literal -> NULL
    s = re.sub(r'\bnull\b', 'NULL', s)
    # 9. new T[n] -> new T[n]() (value-init like Java; reference elements become T*)
    def _new_arr(m):
        t = m.group(1)
        star = m.group(2) or ('*' if resolver.is_pointer(ir.JType(t)) else '')
        return 'new %s%s[%s]()' % (resolver.base_cpp(t), star, m.group(3))
    s = _NEW_ARRAY.sub(_new_arr, s)
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
    ptr_names = sorted((n for n, jt in scope.items() if resolver.is_pointer(jt)),
                       key=len, reverse=True)   # longer names first (avoid prefix overlap)
    static_set = set(resolver.known) | set(resolver.enums) | {cls.name}
    enum_set = set(resolver.enums)
    out = []
    for raw in inner_lines(method.body):
        s = raw.strip()
        if not s or s.startswith('//') or s.startswith('*') or s.startswith('/*'):
            if s:
                out.append(s)
            continue
        line = _translate_line(s, ptr_names, static_set, enum_set, resolver, scope)
        line, guarded = _guard_null_deref(line, scope, resolver)
        if guarded and ledger is not None:
            ledger.add(cls.name + '.cpp', Category.RISK,
                       'null-deref guard inserted before list-indexed call (an unregistered/NULL '
                       'slot would be a virtual call through NULL -> IP 0x0 crash)',
                       line=method.line, severity=Severity.MED, source='emit', rule='null-deref-guard')
        out.append(line)
    return out
