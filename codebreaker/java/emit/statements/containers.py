# -*- coding: utf-8 -*-
"""
statements/containers.py -- Java collection lowering: `new ArrayList/HashMap/HashSet(...)` and
their method calls -> JList/JMap/JSet + the C++ method spelling. Carries the map-get-must-not-
insert fix (see constants._INDEX_METHOD). Imports only leaves (scan/constants) + typemap/parse.
"""

import re

from codebreaker.java import typemap, parse as javaparse
from codebreaker.java.emit.statements.scan import _split_top_commas
from codebreaker.java.emit.statements.constants import _LIST_TYPES, _MAP_TYPES, _METHOD_RENAME, _INDEX_METHOD


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


def _container_element(jt):
    """Element type produced by subscripting a container with `[...]`, or None if it can't be
    determined (raw container with no tracked generic arg -- then we do NOT step in, staying safe).

    list/vector `[i]` -> the element type (args[0]); map `[k]` -> the value type (args[-1]). This is
    what lets `jlist[i].method()` know `jlist[i]` is a `T*` so the following call becomes `->` --
    the container analogue of stepping an array element (see FIXED BUG list-elem-dot-arrow)."""
    if jt is None or not getattr(jt, 'args', None):
        return None
    cat = _category(jt)
    if cat == 'list':
        return jt.args[0]
    if cat == 'map' and len(jt.args) >= 2:
        return jt.args[-1]
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


