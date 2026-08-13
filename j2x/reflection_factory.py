# -*- coding: utf-8 -*-
r"""
reflection_factory.py -- turn Java reflection registries into real C++ factories.

Java code registers a class table and then instantiates by id via reflection:

    addMapping(EntityPig.class, "Pig", 90);              // register
    (Entity) cls.getConstructor(World.class).newInstance(world);   // reflect-construct

C++ has no `newInstance`, so the converter stubs it to NULL (the classic TileEntity-load /
Packet-parse / entity-spawn crash). But the registration table lists EVERY concrete class, so the
factory is fully recoverable: this pass reads the `addMapping(j2xClassOf(typeid(X)), …)` /
`addIdClassMapping(…, j2xClassOf(typeid(X)))` table out of the emitted .cpp, generates a
`<Registry>_createByReflection(Class* c[, Arg* a0 …])` that switches the Class* to a real
`new X(args)`, and rewrites the reflection call site to invoke it. NULL stub -> working factory.

Runs as a post-emit pass over the converted tree (also `j2x reflect-factories <dir>`), and is wired
into `convert()` so every port gets it automatically.
"""

import os
import re

# a receiver lvalue: var, var->field, this->field, var[i], var[i]->field, ...
_RECV = r'((?:this\s*->\s*)?[A-Za-z_]\w*(?:\s*->\s*[A-Za-z_]\w*|\s*\[[^\]]*\])*)'

# a reflection instantiation with a ctor arg (EntityList / SpawnerAnimals / MapStorage):
#   (Entity*)recv->getConstructor((JArrayInit<Class* >(), j2xClassOf(typeid(World)))).newInstance((JArrayInit<JObject >(), var1))
_WORLD_ARG = re.compile(
    r'\(\s*([A-Za-z_]\w*)\s*\*\s*\)\s*' + _RECV + r'\s*->\s*getConstructor\s*\(\s*'
    r'\(\s*JArrayInit<[^>]*>\(\)((?:\s*,\s*Class\(typeid\(\s*\w+\s*\)\))+)\s*\)\s*\)'
    r'\s*\.\s*newInstance\s*\(\s*\(\s*JArrayInit<[^>]*>\(\)\s*,\s*(.+?)\)\s*\)')

# a plain no-arg reflection instantiation (TileEntity / Packet / BlockSign):  (TileEntity*)recv->newInstance()
_PLAIN = re.compile(r'\(\s*([A-Za-z_]\w*)\s*\*\s*\)\s*' + _RECV + r'\s*->\s*newInstance\s*\(\s*\)')

# X.class -> Class(typeid(X)); the dynamic getClass() form Class(typeid(*(var))) is excluded by \w+
_TYPEID = re.compile(r'Class\(typeid\(\s*(\w+)\s*\)\)')
# registration call lines that carry the concrete class table
_REG_LINE = re.compile(r'\b(?:addMapping|addIdClassMapping|register\w*|addClassMapping)\s*\(')


def _split_top(s):
    """Split a comma-separated arg blob at top-level commas (respecting nested parens/angles)."""
    out, depth, cur = [], 0, ''
    for ch in s:
        if ch in '(<':
            depth += 1
        elif ch in ')>':
            depth -= 1
        if ch == ',' and depth == 0:
            out.append(cur.strip()); cur = ''
        else:
            cur += ch
    if cur.strip():
        out.append(cur.strip())
    return out


def _concrete_classes(text, arg_types):
    """Concrete classes to switch over: every typeid(X) on a registration line, minus the ctor arg
    types (e.g. World) and dynamic `typeid(*(...))` lookups (which the \\w+ pattern already excludes)."""
    seen, out = set(), []
    for line in text.split('\n'):
        if not _REG_LINE.search(line):
            continue
        for m in _TYPEID.finditer(line):
            x = m.group(1)
            if x in arg_types or x in seen:
                continue
            seen.add(x); out.append(x)
    return out


def _strip_resolved_comments(text):
    """Drop `// J2X-REVIEW[reflection-unsupported]` lines whose next code line now calls a generated
    factory -- the concern is resolved, so the greppable tag should go."""
    lines = text.split('\n')
    out = []
    for i, ln in enumerate(lines):
        if 'J2X-REVIEW[reflection-unsupported]' in ln:
            nxt = lines[i + 1] if i + 1 < len(lines) else ''
            if '_createByReflection(' in nxt:
                continue
        out.append(ln)
    return '\n'.join(out)


def process(text, registry):
    """Rewrite reflection sites in `text` (one .cpp, `registry` = its class name) to a generated
    factory. Returns (new_text, sites_rewritten, factory_or_None)."""
    world = list(_WORLD_ARG.finditer(text))
    plain = list(_PLAIN.finditer(text))
    if not world and not plain:
        return _strip_resolved_comments(text), 0, None

    if world:                                            # World-arg registry (entities)
        m0 = world[0]
        ret_type = m0.group(1)
        arg_types = _TYPEID.findall(m0.group(3))         # e.g. ['World']
        arg_vals_sample = _split_top(m0.group(4))
        n_args = min(len(arg_types), len(arg_vals_sample))
        arg_types = arg_types[:n_args]
    else:                                                # no-arg registry (tile entities / packets)
        ret_type = plain[0].group(1)
        arg_types = []

    concrete = _concrete_classes(text, set(arg_types))
    if not concrete:
        return text, 0, None

    # ---- generate the factory ----
    params = ', '.join(['Class* c'] + ['%s* a%d' % (t, i) for i, t in enumerate(arg_types)])
    callargs = ', '.join('a%d' % i for i in range(len(arg_types)))
    fname = '%s_createByReflection' % registry
    L = ['', '// ==== j2x-generated reflection factory (replaces newInstance/getConstructor) ====',
         '// Built from the class registration table; returns a real `new` instead of the NULL stub.',
         '%s* %s(%s)' % (ret_type, fname, params), '{', '\tif (!c) return NULL;']
    for x in concrete:
        L.append('\tif (*c == Class(typeid(%s))) return new %s(%s);' % (x, x, callargs))
    L += ['\treturn NULL;', '}', '']
    factory = '\n'.join(L)

    # ---- rewrite the call sites ----
    n = [0]

    def _repl_world(m):
        n[0] += 1
        recv = m.group(2)
        vals = _split_top(m.group(4))[:len(arg_types)]
        casts = ', '.join('(%s*)%s' % (t, v) for t, v in zip(arg_types, vals))
        return '%s(%s%s)' % (fname, recv, (', ' + casts) if casts else '')

    def _repl_plain(m):
        n[0] += 1
        return '%s(%s)' % (fname, m.group(2))

    text = _WORLD_ARG.sub(_repl_world, text)
    text = _PLAIN.sub(_repl_plain, text)

    # ---- insert the factory after the include block (before the first method uses it) ----
    lines = text.split('\n')
    last_inc = 0
    for i, ln in enumerate(lines):
        if ln.lstrip().startswith('#include'):
            last_inc = i
    lines.insert(last_inc + 1, factory)
    return _strip_resolved_comments('\n'.join(lines)), n[0], {'registry': registry, 'ret': ret_type,
                                                              'args': arg_types, 'classes': concrete}


def _find_factory(factories, arg_types, cast):
    """Pick the factory whose ctor-arg signature matches this site and whose return type is `cast`
    (or a base of it -- e.g. EntityList's Entity* factory serving an (EntityLiving*) site)."""
    same = [fa for fa in factories if fa['args'] == arg_types]
    for fa in same:
        if fa['ret'] == cast or cast.startswith(fa['ret']):
            return fa
    return same[0] if len(same) == 1 else None            # unique arg-sig (e.g. the sole World-arg factory)


def _reuse(text, factories):
    """Rewrite reflection sites in a NON-registry file using the shared factories. Returns (text, n)."""
    n = [0]

    def rep_world(m):
        cast, recv = m.group(1), m.group(2)
        arg_types = _TYPEID.findall(m.group(3))
        vals = _split_top(m.group(4))[:len(arg_types)]
        fa = _find_factory(factories, arg_types, cast)
        if not fa:
            return m.group(0)
        n[0] += 1
        casts = ', '.join('(%s*)%s' % (t, v) for t, v in zip(arg_types, vals))
        return '(%s*)%s(%s%s)' % (cast, fa['name'], recv, (', ' + casts) if casts else '')

    def rep_plain(m):
        cast, recv = m.group(1), m.group(2)
        fa = _find_factory(factories, [], cast)
        if not fa:
            return m.group(0)
        n[0] += 1
        return '(%s*)%s(%s)' % (cast, fa['name'], recv)

    text = _WORLD_ARG.sub(rep_world, text)
    text = _PLAIN.sub(rep_plain, text)
    return _strip_resolved_comments(text), n[0]


def _write_header(out_dir, factories):
    fwd = set(['Class'])
    for fa in factories:
        fwd.add(fa['ret']); fwd.update(fa['args'])
    L = ['// ReflectionFactory.h -- j2x-generated. Declares the reflection factories so any file can',
         '// construct a registered class by its Class* (replacing Java newInstance()).',
         '#ifndef J2X_REFLECTION_FACTORY_H', '#define J2X_REFLECTION_FACTORY_H', '',
         ' '.join('class %s;' % t for t in sorted(fwd))]
    seen = set()
    for fa in factories:
        params = ', '.join(['Class* c'] + ['%s* a%d' % (t, i) for i, t in enumerate(fa['args'])])
        decl = '%s* %s(%s);' % (fa['ret'], fa['name'], params)
        if decl not in seen:
            seen.add(decl); L.append(decl)
    L += ['', '#endif // J2X_REFLECTION_FACTORY_H', '']
    with open(os.path.join(out_dir, 'ReflectionFactory.h'), 'w', encoding='utf-8') as f:
        f.write('\n'.join(L))


def _add_include(text):
    if '#include "ReflectionFactory.h"' in text:
        return text
    lines = text.split('\n')
    last = 0
    for i, ln in enumerate(lines):
        if ln.lstrip().startswith('#include'):
            last = i
    lines.insert(last + 1, '#include "ReflectionFactory.h"')
    return '\n'.join(lines)


def run(out_dir):
    """Two passes: (1) generate a factory for each registry class from its registration table;
    (2) rewrite reflection sites in OTHER files to reuse those factories via ReflectionFactory.h."""
    files = [f for f in sorted(os.listdir(out_dir)) if f.endswith('.cpp')]

    def read(f):
        try:
            return open(os.path.join(out_dir, f), 'r', encoding='utf-8', errors='replace').read()
        except (IOError, OSError):
            return None

    def write(f, t):
        with open(os.path.join(out_dir, f), 'w', encoding='utf-8') as fh:
            fh.write(t)

    # PASS 1: registry factories
    results, factories = [], []
    registry_files = set()
    for f in files:
        text = read(f)
        if text is None or ('newInstance' not in text and '_createByReflection(' not in text):
            continue
        new, n, fac = process(text, f[:-4])
        if new != text:
            write(f, new)
        if n and fac:
            results.append({'file': f, 'sites': n, **fac})
            registry_files.add(f)
            factories.append({'name': '%s_createByReflection' % fac['registry'],
                              'ret': fac['ret'], 'args': fac['args']})
    # detect factories generated by a PRIOR run (so re-runs / cross-file reuse still work)
    _defre = re.compile(r'^([A-Za-z_]\w*)\*\s+(\w+_createByReflection)\s*\(([^)]*)\)\s*$', re.M)
    known = set((fa['name']) for fa in factories)
    for f in files:
        text = read(f)
        if text is None or '_createByReflection(' not in text:
            continue
        for m in _defre.finditer(text):
            if m.group(2) in known:
                continue
            params = [p.strip() for p in m.group(3).split(',')][1:]   # drop the leading Class* c
            args = [p.split('*')[0].strip() for p in params if '*' in p]
            known.add(m.group(2))
            factories.append({'name': m.group(2), 'ret': m.group(1), 'args': args})

    # PASS 2: reuse factories in other files
    reused = []
    if factories:
        _write_header(out_dir, factories)
        for f in files:
            if f in registry_files:
                continue
            text = read(f)
            if text is None or 'newInstance' not in text:
                continue
            new, n = _reuse(text, factories)
            if n:
                new = _add_include(new)
                write(f, new)
                reused.append({'file': f, 'sites': n})
    return {'registries': results, 'reused': reused,
            'total_sites': sum(r['sites'] for r in results) + sum(r['sites'] for r in reused),
            'total_classes': sum(len(r['classes']) for r in results)}
