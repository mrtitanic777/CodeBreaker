#!/usr/bin/env python3
"""Audit handwritten overrides for DROPPED field initializers.

Recurring crash class: a hand-authored override/<Class>.cpp omits a Java field initializer that the
converter itself emits correctly (e.g. GuiMainMenu.minecraftLogo, RenderGlobal.allRenderLists,
WorldRenderer.skipRenderPass). C++ does NOT zero-init like Java, so the field is left empty/garbage ->
null-deref / IP 0x0 at runtime.

This flags the high-signal subset: ALLOCATION initializers (`new X()`, `new T[]{...}`, `{...}`) whose
field is never assigned via `this->FIELD =` anywhere in the override .cpp. Those are the ones that cause
NULL/empty-array crashes. Run from the ConversionTool dir:  python tools/audit_override_inits.py
"""
import sys, os, re
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from j2x import parse, pipeline

JAVA_DIR = r'C:/Users/mrtit/Desktop/MinecraftAlpha/minecraft/src'
OVR_DIR  = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), 'overrides')
LIVE_DIR = r'C:/Users/mrtit/Documents/Visual Studio 2010/Projects/MinecraftAlpha/MinecraftAlpha'
# Java container methods that the converter RENAMES (add->push_back, get->[], etc.) -- ignore in the
# static{}-block call check or they false-positive (e.g. Session's registeredBlocksList.add -> push_back).
_CONTAINER_METHODS = {'add','get','put','remove','size','contains','clear','keySet','values','iterator',
                      'isEmpty','set','addAll','indexOf','toString','length','charAt','put','containsKey',
                      'static','if','for','while','switch','return','new','equals','hashCode'}

def main():
    classes = []
    for p in pipeline.discover(JAVA_DIR):
        try:
            classes.append(parse.parse_file(p))
        except Exception:
            pass
    byname = {c.name: c for c in classes}

    total = 0
    for fn in sorted(os.listdir(OVR_DIR)):
        if not fn.endswith('.cpp'):
            continue
        cls = byname.get(fn[:-4])
        if not cls:
            continue
        ov = open(os.path.join(OVR_DIR, fn), encoding='utf-8', errors='replace').read()
        miss = []
        for f in cls.instance_fields():
            init = (f.initializer or '').strip()
            if not init:
                continue
            # crash-class only: object/array ALLOCATIONS (NULL/empty if dropped)
            if 'new ' not in init and not init.startswith('{'):
                continue
            # FOCUS on Java ARRAY fields (T[]). These map to JArray<T>, which DEFAULT-constructs to
            # length 0 -> indexing returns a NULL/dummy element -> deref crash. Value-type containers
            # (ArrayList->JList, HashMap->JMap) auto-construct empty and are NOT a crash risk; skip them.
            if not f.jtype.is_array:
                continue
            # assigned in the override BODY? (this->FIELD = ...)  then it's fine. (The converter emits
            # array inits in the body, not the init-list, so this is the right signal for arrays.)
            if re.search(r'this->' + re.escape(f.name) + r'\s*=[^=]', ov):
                continue
            miss.append((f.name, init[:60]))
        if miss:
            total += len(miss)
            print('=== %s.cpp  (dropped array initializers) ===' % cls.name)
            for n, i in miss:
                print('   %-24s = %s' % (n, i))

    # --- check 2: dropped Java static{} BLOCK bodies (stale overrides) ---
    # An override that predates a converter feature can omit a `static { ... }` block the converter
    # now emits into initStatics(). MobSpawnerBase's `static { generateBiomeLookup(); }` was dropped ->
    # all-NULL biome table -> IP 0x0 at first render. Flag any override whose initStatics() is missing a
    # method called by the class's Java static{} block.
    for fn in sorted(os.listdir(OVR_DIR)):
        if not fn.endswith('.cpp'):
            continue
        cls = byname.get(fn[:-4])
        if not cls or not getattr(cls, 'static_inits', None):
            continue
        ov = open(os.path.join(OVR_DIR, fn), encoding='utf-8', errors='replace').read()
        for block in cls.static_inits:
            txt = '\n'.join(t for (_, t) in block) if block and isinstance(block[0], tuple) else str(block)
            for call in set(re.findall(r'\b([A-Za-z_]\w*)\s*\(', txt)):
                if call in ('static', 'if', 'for', 'while', 'switch', 'return', 'new'):
                    continue
                if call not in ov:
                    total += 1
                    print('=== %s.cpp  (dropped static{} block call: %s()) ===' % (cls.name, call))

    # --- check 3: WHOLE LIVE TREE -- every class with a Java static{} block, confirm the live .cpp
    # (override OR generated) still contains the block's custom-method calls. Catches a stale generated
    # file too (not just overrides). Container methods are renamed by the converter, so they're ignored.
    for c in classes:
        si = getattr(c, 'static_inits', None)
        if not si:
            continue
        live_cpp = os.path.join(LIVE_DIR, c.name + '.cpp')
        if not os.path.exists(live_cpp):
            continue
        src = open(live_cpp, encoding='utf-8', errors='replace').read()
        calls = set()
        for block in si:
            txt = '\n'.join(t for (_, t) in block) if block and isinstance(block[0], tuple) else str(block)
            calls |= set(re.findall(r'\b([A-Za-z_]\w*)\s*\(', txt))
        for call in calls - _CONTAINER_METHODS:
            if call not in src:
                total += 1
                print('=== LIVE %s.cpp  (static{} block call missing: %s()) ===' % (c.name, call))

    print('\nTOTAL dropped converter-content (override array/static-blocks + live static-blocks):', total)

if __name__ == '__main__':
    main()
