# -*- coding: utf-8 -*-
"""Synthesize constructors for OVERRIDE classes that have NONE but need Java field initializers.

The converter synthesizes a default ctor (zero-init member list + Java field initializers) for any
generated no-ctor class. OVERRIDE classes are used verbatim, so they DON'T get that synthesis -- and a
hand-written override that has no ctor and dropped Java's `= new X()` field initializers leaves those
pointers garbage (SoundManager::soundPoolSounds, SoundPool::rand, ClippingHelper::*Buffer,
MCHashTable::slots ...) -> NULL/garbage deref crashes. fix_override_field_inits can't help (no ctor to
inject into); zero_init_ctors skips them (they have explicit initializers).

This emits the SAME ctor the converter would (source._init_list + source._instance_init_lines) into the
override .cpp (definition) + .h (declaration), and adds includes for any `new Type(` it references.
Skips classes that already declare a ctor (idempotent -- GuiScreen/SoundManager already hand-fixed).
Applies to both overrides/ and the live tree.

  python tools/synth_override_ctors.py --dry
  python tools/synth_override_ctors.py
"""
import os, re, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)
from j2x import parse, pipeline               # noqa: E402
from j2x.emit import source                   # noqa: E402

SRC  = "C:/Users/mrtit/Desktop/MinecraftAlpha/minecraft/src"
OVR  = os.path.join(ROOT, 'overrides')
LIVE = os.path.normpath(os.path.join(ROOT, '..', 'MinecraftAlpha'))
DRY  = '--dry' in sys.argv

# Body init lines that are REDUNDANT (value members already default-construct) and RISKY (the converter's
# assumed container type may not match the override's hand-written field) -> drop them. Keep real inits:
# `new X()`, factory calls, sized arrays JArray(N), non-zero/true literals.
_EMPTY = re.compile(r'^(std::(map|set|vector|list)<.*>|JList<.*>|JArray<.*>)\(\)$')
_ZERO  = {'NULL', '0', 'false', '0.0', '0.0F', '0.0f', '0L', '0LL', "'\\0'"}
def _trivial(stmt):
    m = re.match(r'\s*this->\w+\s*=\s*(.+?);\s*$', stmt)
    r = m.group(1).strip() if m else ''
    return r in _ZERO or bool(_EMPTY.match(r))


def load():
    classes = []
    for p in pipeline.discover(SRC):
        try: classes.append(parse.parse_file(p))
        except Exception: pass
    pipeline.resolve_inheritance(classes); pipeline.compute_mi_forwards(classes)
    return classes, pipeline.build_resolver(classes)


def has_ctor_h(text, cls):
    # a ctor DECLARATION is a member line that starts with `ClassName(` (not `new ClassName(`,
    # not a method `ClassName foo(`, not the `~ClassName()` dtor).
    return re.search(r'(?m)^\s*' + re.escape(cls) + r'\s*\(', text) is not None


def has_ctor_cpp(text, cls):
    return re.search(re.escape(cls) + r'::' + re.escape(cls) + r'\s*\(', text) is not None


def patch_header(path, cls):
    with open(path, encoding='utf-8', newline='') as f: raw = f.read()
    nl = '\r\n' if '\r\n' in raw else '\n'; s = raw.replace('\r\n', '\n')
    if has_ctor_h(s, cls): return False
    m = re.search(r'\bclass\s+' + re.escape(cls) + r'\b[^{]*\{', s)
    if not m: return False
    pub = s.find('public:', m.end())
    if pub < 0: return False
    ins = s.index('\n', pub) + 1
    s = s[:ins] + '\t%s();   // synthesized: Java field initializers (override had no ctor)\n' % cls + s[ins:]
    if not DRY:
        with open(path, 'w', encoding='utf-8', newline='') as f: f.write(s.replace('\n', nl))
    return True


def patch_source(path, cls, ctor_def, inc_types):
    with open(path, encoding='utf-8', newline='') as f: raw = f.read()
    nl = '\r\n' if '\r\n' in raw else '\n'; s = raw.replace('\r\n', '\n')
    if has_ctor_cpp(s, cls): return False
    m = re.search(r'\b' + re.escape(cls) + r'::~' + re.escape(cls) + r'\s*\(\s*\)', s)  # before the dtor
    if not m:
        return False
    at = s.rfind('\n', 0, m.start())                       # start of the dtor's line
    at = s.rfind('\n', 0, at) + 1 if at > 0 else m.start() # keep a blank line above
    s = s[:m.start()] + ctor_def + '\n\n' + s[m.start():]
    for t in sorted(inc_types):                            # ensure includes for `new Type(`
        inc = '#include "%s.h"' % t
        if inc not in s:
            i = s.find('#include'); j = s.index('\n', i) + 1
            s = s[:j] + inc + '\n' + s[j:]
    if not DRY:
        with open(path, 'w', encoding='utf-8', newline='') as f: f.write(s.replace('\n', nl))
    return True


def main():
    classes, resolver = load()
    ovr = {os.path.basename(f)[:-2] for f in os.listdir(OVR) if f.endswith('.h')} \
        | {os.path.basename(f)[:-4] for f in os.listdir(OVR) if f.endswith('.cpp')}
    done = []
    for c in classes:
        if c.name not in ovr or c.constructors():
            continue
        items = source._init_list(c, resolver)
        inits = [l for l in source._instance_init_lines(c, resolver, None) if not _trivial(l)]
        if not items and not inits:
            continue
        L = ['%s::%s()' % (c.name, c.name)]
        if items: L.append('\t: ' + '\n\t, '.join(items))
        L += ['{'] + ['\t' + l for l in inits] + ['}']
        ctor_def = '\n'.join(L)
        inc = set(re.findall(r'\bnew\s+(\w+)\s*[(\[]', ctor_def))
        inc |= set(re.findall(r'typeid\((\w+)\)', ctor_def))          # Class(typeid(EntitySpider)) etc.
        hit = False
        for d in (OVR, LIVE):
            hp, cp = os.path.join(d, c.name + '.h'), os.path.join(d, c.name + '.cpp')
            if not (os.path.exists(hp) and os.path.exists(cp)):
                continue
            # Skip if a ctor exists in EITHER file (e.g. an inline .h ctor already added) -- adding a
            # .cpp definition for an inline-declared ctor would be a double definition.
            htext = open(hp, encoding='utf-8').read(); ctext = open(cp, encoding='utf-8').read()
            if has_ctor_h(htext, c.name) or has_ctor_cpp(ctext, c.name):
                continue
            ph = patch_header(hp, c.name)
            pc = patch_source(cp, c.name, ctor_def, inc)
            hit = hit or ph or pc
        if hit:
            done.append(c.name)
    print('%s %d override classes: %s' % ('DRY' if DRY else 'PATCHED', len(done), ', '.join(done)))


if __name__ == '__main__':
    main()
