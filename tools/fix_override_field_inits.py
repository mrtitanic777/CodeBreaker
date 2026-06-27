# -*- coding: utf-8 -*-
"""Restore Java FIELD INITIALIZERS dropped by the hand-written overrides.

Java runs `Type field = <init>;` declarations implicitly at the top of every (non-delegating)
constructor, BEFORE the ctor body. The overrides translated only the ctor BODY, so every such
initializer was lost -> the field stayed at its zero-init (NULL for pointers) -> null-deref crashes
(e.g. TexturePackList.defaultTexturePack, Minecraft.timer/entityRenderer/...). The converter already
emits these correctly for generated classes via emit.source._instance_init_lines; this reuses that
to inject the SAME lines into the override ctors.

Per override ctor, a field's init is injected only when: the field has a Java initializer, the ctor
zero-inits it in its member-init list (NULL/0/false/0.0/0LL -> a real ctor, not a this(...) delegate),
and the field is not already assigned in the file (so no double-new / leak). Includes for `new Type(`
referenced by injected lines are added if missing. Idempotent.

  python tools/fix_override_field_inits.py --dry        # report only
  python tools/fix_override_field_inits.py              # apply to overrides/ AND the live tree
"""
import os
import re
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)
from j2x import parse, pipeline            # noqa: E402
from j2x.emit import source                # noqa: E402

SRC = "C:/Users/mrtit/Desktop/MinecraftAlpha/minecraft/src"
OVR = os.path.join(ROOT, 'overrides')
LIVE = os.path.normpath(os.path.join(ROOT, '..', 'MinecraftAlpha'))
ZERO = {'NULL', '0', 'false', '0.0', '0.0F', '0.0f', '0L', '0LL', "'\\0'"}
# RHS values that the member-init list's zero-init ALREADY produces -> no need to inject (would be
# redundant noise). Everything else (new X(), sized JArray, true, non-zero literals, factory calls)
# is a real Java initializer that the override dropped and must be restored.
_EMPTY_CONTAINER = re.compile(r'^(std::(map|set|vector)<.*>|JList<.*>)\(\)$')


def _rhs(stmt):
    m = re.match(r'\s*this->\w+\s*=\s*(.+?);\s*$', stmt)
    return m.group(1).strip() if m else ''


def _trivial(stmt):
    r = _rhs(stmt)
    return r in ZERO or bool(_EMPTY_CONTAINER.match(r))


def load():
    classes = []
    for path in pipeline.discover(SRC):
        try:
            classes.append(parse.parse_file(path))     # parse_file -> one JClass
        except Exception:
            pass
    pipeline.resolve_inheritance(classes)
    pipeline.compute_mi_forwards(classes)
    resolver = pipeline.build_resolver(classes)
    return {c.name: c for c in classes}, resolver


def init_lines_for(cls, resolver):
    """{field_name: 'this->field = ...;'} for every Java field initializer of cls."""
    out = {}
    for ln in source._instance_init_lines(cls, resolver, None):
        m = re.match(r'\s*this->(\w+)\s*=', ln)
        if m:
            out[m.group(1)] = ln.strip()
    return out


def member_init_trivial(ctor_head):
    """field names the ctor's member-init list initializes to a TRIVIAL value (NULL/0/false/'' default
    or a bare literal -- captured only when the value has no nested parens, so a real `field(new X(..))`
    is deliberately NOT matched -> never re-injected -> no double-new). These are the fields a real
    (non-delegating) ctor zero-inits and that a dropped Java initializer should fill in."""
    out = set()
    for m in re.finditer(r'[:,]\s*(\w+)\(([^()]*)\)', ctor_head):
        v = m.group(2).strip()
        if v == '' or v in ZERO:
            out.add(m.group(1))
    return out


def _ctor_plan(s, cls, inits):
    """Return [(body_index, [(field, stmt), ...]), ...] -- per real ctor, the non-trivial Java field
    initializers it dropped (listed in member-init, not assigned in the body anywhere)."""
    assigned = set(re.findall(r'this->(\w+)\s*=', s))
    plans = []
    for m in re.finditer(r'\b' + re.escape(cls) + r'::' + re.escape(cls) + r'\s*\([^;{]*?\)\s*((?::[^{]*?)?)\{', s):
        listed = member_init_trivial(m.group(0))
        want = [(f, st) for f, st in inits.items()
                if f in listed and f not in assigned and not _trivial(st) and st not in s]
        if want:
            plans.append((m.end(), want))
    return plans


def inject(path, inits):
    if not os.path.exists(path):
        return []
    with open(path, encoding='utf-8', newline='') as f:
        raw = f.read()
    nl = '\r\n' if '\r\n' in raw else '\n'
    s = raw.replace('\r\n', '\n')
    cls = os.path.basename(path)[:-4]
    plans = _ctor_plan(s, cls, inits)
    if not plans:
        return []
    added, includes = [], set()
    for body_at, want in sorted(plans, key=lambda p: -p[0]):    # last ctor first: indices stay valid
        block = ''.join('\t%s\n' % st for _, st in want)
        s = s[:body_at] + '\n' + block + s[body_at:]
        added += [f for f, _ in want]
        for _, st in want:
            for t in re.findall(r'\bnew\s+(\w+)\s*[(\[]', st):
                includes.add(t)
    for t in sorted(includes):                                  # ensure includes for `new Type(`
        inc = '#include "%s.h"' % t
        if inc not in s:
            i = s.find('#include'); j = s.index('\n', i) + 1
            s = s[:j] + inc + '\n' + s[j:]
    with open(path, 'w', encoding='utf-8', newline='') as f:
        f.write(s.replace('\n', nl))
    return sorted(set(added))


def main():
    dry = '--dry' in sys.argv
    by_name, resolver = load()
    ovr_classes = [fn[:-4] for fn in os.listdir(OVR) if fn.endswith('.cpp')]
    total = 0
    for cls in sorted(ovr_classes):
        c = by_name.get(cls)
        if not c:
            continue
        inits = init_lines_for(c, resolver)
        if not inits:
            continue
        ovr_path = os.path.join(OVR, cls + '.cpp')
        with open(ovr_path, encoding='utf-8', newline='') as f:
            body = f.read().replace('\r\n', '\n')
        dropped = sorted(set(f for _, want in _ctor_plan(body, cls, inits) for f, _ in want))
        if not dropped:
            continue
        total += 1
        print('%s: %d dropped -> %s' % (cls, len(dropped), ', '.join(dropped)))
        if not dry:
            for tgt in (OVR, LIVE):
                inject(os.path.join(tgt, cls + '.cpp'), inits)
    print('\n%d override(s) with dropped field initializers%s.' % (total, '' if dry else ' -- injected into overrides/ + live'))


if __name__ == '__main__':
    main()
