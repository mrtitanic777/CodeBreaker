"""twophase.py -- two-phase construction for classes whose constructor makes a virtual call.

Java dispatches a virtual call made inside a constructor to the most-derived override, even though the
subclass's own constructor has not run yet. C++ does not: while a base constructor runs, the object IS a
base, so the call reaches the base's version. Minecraft relies on the Java rule in a few hierarchies --
Block's constructor reads isOpaqueCube() into opaqueCubeLookup (every transparent block came out opaque:
wrong light and culling), Entity's calls entityInit(), World's createChunkProvider() -- so a plain C++
constructor gets them wrong.

For those classes (the set `compute` finds) every Java constructor becomes a method
`C* C::j2xInit(params)`; the C++ constructor `C::C()` only zeroes the fields. j2xInit calls the parent's
j2xInit first (Java's super(...)), then runs the field initialisers and the body, and returns `this`. It
runs on a fully built C++ object, so its virtual calls reach the override as in Java. Creating an object
is then `(new C())->j2xInit(args)` -- `rewrite_news` turns every `new C(args)` in the emitted tree (and in
the probe's harness) into that.
"""
import os
import re

_CALL = re.compile(r'(?<![\w.>:])(?:this\s*\.\s*)?([A-Za-z_]\w*)\s*\(')
_KEYWORDS = frozenset(('if', 'for', 'while', 'switch', 'catch', 'return', 'new', 'super', 'this', 'synchronized'))
_SUPER_ARGS = re.compile(r'^\s*super\s*\((.*)\)\s*;')


def _ctor_text(cls):
    parts = [t for c in cls.constructors() for _ln, t in (c.body or ())]
    parts += [f.initializer for f in cls.instance_fields() if f.initializer]
    for block in getattr(cls, 'inst_inits', ()) or ():
        parts += [t for _ln, t in block]
    return '\n'.join(parts)


def compute(classes, values=()):
    """The classes to build in two phases: each class whose constructor (or instance initialiser) calls one
    of its own instance methods that a subclass overrides (or that is abstract), and every descendant of
    such a class. Left out (with the subtree below it): a value type, and a root whose parent is another
    user class that it constructs with arguments (the parent's constructor would have to run in `C()`)."""
    by_name = dict((c.name, c) for c in classes)
    children = {}
    for c in classes:
        if c.parent in by_name:
            children.setdefault(c.parent, []).append(c.name)

    def descendants(name):
        out, todo = [], list(children.get(name, ()))
        while todo:
            n = todo.pop()
            out.append(n)
            todo.extend(children.get(n, ()))
        return out

    def instance_methods(c):
        return set(m.name for m in c.methods if not m.is_ctor and not m.is_static)

    roots = set()
    for c in classes:
        if c.is_interface or c.is_enum or not c.constructors() and not c.instance_fields():
            continue
        called = set(n for n in _CALL.findall(_ctor_text(c)) if n not in _KEYWORDS)
        if not called:
            continue
        own = set()
        k = c
        while k is not None:                                  # its own and inherited instance methods
            own |= instance_methods(k)
            k = by_name.get(k.parent)
        abstract = set(m.name for m in c.methods if m.is_abstract)
        overridden = set()
        for d in descendants(c.name):
            overridden |= instance_methods(by_name[d])
        if (called & own) & (overridden | abstract):
            roots.add(c.name)

    picked = set()
    for r in roots:
        picked.add(r)
        picked.update(descendants(r))

    def drop(name):
        picked.discard(name)
        for d in descendants(name):
            picked.discard(d)

    changed = True
    while changed:
        changed = False
        for name in sorted(picked):
            c = by_name[name]
            if name in values:
                drop(name); changed = True; break
            if c.parent in by_name and c.parent not in picked:
                with_args = any(_SUPER_ARGS.match(t) and _SUPER_ARGS.match(t).group(1).strip()
                                for k in c.constructors() for _ln, t in (k.body or ()))
                if with_args:
                    drop(name); changed = True; break
    return picked


def _close(text, i):
    """Index just past the parenthesis that closes the one opened before `i`, or -1."""
    depth, q = 1, None
    while i < len(text):
        ch = text[i]
        if q:
            if ch == '\\':
                i += 1
            elif ch == q:
                q = None
        elif ch in '"\'':
            q = ch
        elif ch == '(':
            depth += 1
        elif ch == ')':
            depth -= 1
            if depth == 0:
                return i + 1
        i += 1
    return -1


def rewrite_news(text, names):
    """`new C(args)` -> `(new C())->j2xInit(args)` for C in names. -> (text, how many). Leaves alone a
    `new C()` already in that form and `new C[n]` (an array)."""
    if not names or 'new ' not in text:
        return text, 0
    rx = re.compile(r'\bnew\s+(%s)\s*\(' % '|'.join(re.escape(n) for n in sorted(names, key=len, reverse=True)))
    out, pos, n = [], 0, 0
    for m in rx.finditer(text):
        if m.start() < pos:
            continue
        end = _close(text, m.end())
        if end < 0:
            continue
        args = text[m.end():end - 1]
        if not args.strip() and text.startswith(')->j2xInit(', end):
            continue
        inner, k = rewrite_news(args, names)
        out.append(text[pos:m.start()])
        out.append('(new %s())->j2xInit(%s)' % (m.group(1), inner))
        pos = end
        n += 1 + k
    out.append(text[pos:])
    return ''.join(out), n


def rewrite_stack(text, names):
    """A stack object `C tmp(args);` / `C tmp;` (the leak-temporary rewrite makes these) ->
    `C tmp; tmp.j2xInit(args);`. -> (text, how many)."""
    if not names:
        return text, 0
    alt = '|'.join(re.escape(n) for n in sorted(names, key=len, reverse=True))
    rx = re.compile(r'(?<![\w:<*&])(%s)\s+(_j2xleak\d+)\s*(?:\((.*?)\))?\s*;' % alt)
    count = [0]

    def sub(m):
        count[0] += 1
        return '%s %s; %s.j2xInit(%s);' % (m.group(1), m.group(2), m.group(2), m.group(3) or '')
    return rx.sub(sub, text), count[0]


def run(out_dir, names):
    """The post-pass over the emitted tree: every `new C(...)` of a two-phase class. -> {'sites', 'files'}"""
    sites, files = 0, []
    if not names:
        return {'sites': 0, 'files': []}
    for fn in sorted(os.listdir(out_dir)):
        if not fn.endswith(('.cpp', '.h')):
            continue
        path = os.path.join(out_dir, fn)
        with open(path, encoding='utf-8', errors='replace', newline='') as f:
            text = f.read()
        new, n = rewrite_news(text, names)
        new, k = rewrite_stack(new, names)
        if n or k:
            with open(path, 'w', encoding='utf-8', newline='') as f:
                f.write(new)
            sites += n + k
            files.append(fn)
    return {'sites': sites, 'files': files}
