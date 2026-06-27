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
            print('=== %s.cpp ===' % cls.name)
            for n, i in miss:
                print('   %-24s = %s' % (n, i))
    print('\nTOTAL dropped allocation-initializers across overrides:', total)

if __name__ == '__main__':
    main()
