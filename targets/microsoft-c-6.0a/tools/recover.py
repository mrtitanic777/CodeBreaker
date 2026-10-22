#!/usr/bin/env python3
"""Phase-4 baseline: attempt msc6dec.invert() on authored functions of a pass.
Counts clean inversions and dumps samples. Usage: python recover.py <exe> <origin.json> <out.c>"""
import sys, os, json
sys.path.insert(0, os.environ.get('CB_ROOT', r'C:\Users\mrtit\Documents\GitHub\CodeBreaker'))
from src.engines.inverse import msc6dec

def u16(b, o): return b[o] | (b[o+1] << 8)

exe, oj, outp = sys.argv[1:4]
d = open(exe, 'rb').read(); h = u16(d, 8)*16
img = d[h: u16(d,4)*512 - ((512-u16(d,2)) if u16(d,2) else 0)]
funcs = [f for f in json.load(open(oj))['functions'] if f.get('origin') == 'authored']
ok = 0; samples = []; declined = 0; errors = 0
from collections import Counter
reasons = Counter()
for f in sorted(funcs, key=lambda x: x['size']):
    s, e = f['start'], f['end']
    if e - s < 4 or e - s > 2000: continue
    code = bytes(img[s:e])
    try:
        res = msc6dec.invert(code)
        c, reason = res if isinstance(res, tuple) else (res, "")
    except Exception as ex:
        errors += 1; reasons[f"exc:{type(ex).__name__}"] += 1; continue
    if c and c.strip():
        ok += 1
        if len(samples) < 15:
            samples.append(f"/* f{s:05x} size={e-s} */\n{c}\n")
    else:
        declined += 1; reasons[(reason or "?").split(":")[0][:20]] += 1
print(f"{os.path.basename(exe)}: authored={len(funcs)}  invert_ok={ok}  declined={declined}  errors={errors}")
print("  top decline reasons:", dict(reasons.most_common(6)))
open(outp, 'w', newline='\n').write("\n".join(samples))
