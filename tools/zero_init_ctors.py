# -*- coding: utf-8 -*-
"""Inject zero-init default constructors into classes that had NONE.

Java's implicit default ctor zero-inits every field; C++'s compiler-default ctor leaves primitives and
pointers as GARBAGE. The converter only synthesized a default ctor for a no-ctor class when it had
EXPLICIT field initializers -- a class with no ctor and only zero-default fields (e.g.
GuiScreen::allowUserInput) got the compiler default -> garbage -> crashes (the menu IP 0x0). header.py
and source.py are now fixed for future regens; this patches the EXISTING live tree + override headers in
place (the project patches live in place rather than regen -- cf. clean_live_cpp.py).

Type-accurate: it reuses the converter's own parser + emit.source._init_list, so the member-init list is
correct per field type (primitives/pointers -> 0/false/NULL; object members default-construct, untouched).
Injects an INLINE `ClassName() : f1(0), f2(0)... {}` after the first `public:`. Skips any class that
already declares a ctor (idempotent -- e.g. GuiScreen, already hand-fixed).

  python tools/zero_init_ctors.py --dry      # report only
  python tools/zero_init_ctors.py            # apply to live tree AND overrides/
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


def load():
    classes = []
    for path in pipeline.discover(SRC):
        try:
            classes.append(parse.parse_file(path))
        except Exception:
            pass
    pipeline.resolve_inheritance(classes)
    pipeline.compute_mi_forwards(classes)
    resolver = pipeline.build_resolver(classes)
    return classes, resolver


def has_ctor(text, cls):
    """True if the header already declares a constructor for `cls` (NOT the ~dtor)."""
    body = text
    return re.search(r'(?<![~\w])' + re.escape(cls) + r'\s*\(', body) is not None


def patch(path, cls, ctor_line):
    if not os.path.exists(path):
        return False
    with open(path, encoding='utf-8', newline='') as f:
        raw = f.read()
    nl = '\r\n' if '\r\n' in raw else '\n'
    s = raw.replace('\r\n', '\n')
    if has_ctor(s, cls):
        return False                                   # already has a ctor -> idempotent skip
    # insert after the first `public:` that follows `class <cls>`
    m = re.search(r'\bclass\s+' + re.escape(cls) + r'\b[^{]*\{', s)
    if not m:
        return False
    pub = s.find('public:', m.end())
    if pub < 0:
        return False
    ins = s.index('\n', pub) + 1
    s = s[:ins] + '\t' + ctor_line + '\n' + s[ins:]
    if not DRY:
        with open(path, 'w', encoding='utf-8', newline='') as f:
            f.write(s.replace('\n', nl))
    return True


def main():
    classes, resolver = load()
    patched, skipped, samples = 0, 0, []
    for cls in classes:
        if cls.is_interface or cls.constructors():
            continue                                   # has a real ctor -> converter already zero-inits it
        if getattr(cls, 'inst_inits', []) or any(f.initializer is not None for f in cls.instance_fields()):
            continue                                   # explicit inits -> OLD converter already synth'd a ctor
        items = source._init_list(cls, resolver)       # type-accurate zero-init member list
        if not items:
            continue                                   # no primitive/pointer fields -> compiler default is fine
        ctor_line = '%s() : %s {}' % (cls.name, ', '.join(items))
        hit = False
        for d in (LIVE, OVR):
            if patch(os.path.join(d, cls.name + '.h'), cls.name, ctor_line):
                hit = True
        if hit:
            patched += 1
            if len(samples) < 12:
                samples.append('%-22s %s' % (cls.name, ctor_line[:90]))
        else:
            skipped += 1
    print('%s: %d classes need a zero-init ctor; %d already had one (skipped).'
          % ('DRY' if DRY else 'PATCHED', patched, skipped))
    print('samples:')
    for x in samples:
        print('  ' + x)


if __name__ == '__main__':
    main()
