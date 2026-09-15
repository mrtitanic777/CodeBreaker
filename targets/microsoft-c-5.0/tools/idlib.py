#!/usr/bin/env python3
"""idlib.py -- Phase 4 origin triage: identify CRT/library functions in the MS C 5.0
binaries by FLIRT-style masked matching against the shipped OMF libraries.

Reuses CodeBreaker's libmatch primitives (fixup-masked library-function signatures) and
slide-searches them into our unpacked images. A hit NAMES that function and marks it
[ID] toolchain -- not a reconstruction target.

    python idlib.py <unpacked.exe> [--min N]     # default min sig length 10

Writes work/<name>/identified.json and prints a summary.
"""
import sys, os, glob, json, struct
CB = r"C:\Users\mrtit\Documents\GitHub\CodeBreaker"
sys.path.insert(0, CB)
from src.engines.inverse.libmatch import iter_modules, masked_eq
from src.engines.inverse.omf import records, rd_index

# MS C 5.0 CRT code segments are named _TEXT (Lattice used PROG/CODE/BASE, which
# CodeBreaker's own module_functions hard-codes). Reimplement accepting _TEXT.
CODE_SEGS = ("_TEXT", "CODE", "PROG", "BASE")

def module_functions(data, mstart):
    lnames = [""]; segs = []; pubs = {}
    image = bytearray(); mask = bytearray(); cur = None
    for off, rt, body in records(data, mstart):
        if rt == 0x80 and off != mstart: break
        if rt == 0x96:
            j = 0
            while j < len(body):
                n = body[j]; lnames.append(body[j+1:j+1+n].decode('latin1')); j += 1+n
        elif rt in (0x98, 0x99):
            si, j = rd_index(body, 3); segs.append(lnames[si] if si < len(lnames) else str(si))
        elif rt in (0xa0, 0xa1):
            seg, j = rd_index(body, 0); do = struct.unpack_from("<H", body, j)[0]; j += 2; pl = body[j:]
            sn = segs[seg-1] if 0 < seg <= len(segs) else ""
            if sn in CODE_SEGS:
                end = do + len(pl)
                if end > len(image):
                    image.extend(b"\0"*(end-len(image))); mask.extend(b"\0"*(end-len(mask)))
                image[do:end] = pl; cur = do
            else:
                cur = None
        elif rt in (0x9c, 0x9d):
            if cur is None: continue
            j = 0
            while j < len(body):
                b0 = body[j]
                if not (b0 & 0x80): j += 1; continue
                locat = (b0 << 8) | body[j+1]; j += 2; dro = locat & 0x3ff; loc = (locat >> 10) & 0xf
                fd = body[j]; j += 1
                if not (fd & 0x80):
                    fb = (fd >> 4) & 7
                    if fb in (0, 1, 2): _, j = rd_index(body, j)
                if not (fd & 0x08): _, j = rd_index(body, j)
                if not ((fd >> 2) & 1): j += 2
                w = {0:1,1:2,2:2,3:4,4:1,5:2,9:4}.get(loc, 2)
                for k in range(w):
                    if cur+dro+k < len(mask): mask[cur+dro+k] = 1
        elif rt in (0x90, 0x91):
            g, j = rd_index(body, 0); s, j = rd_index(body, j)
            if s == 0: j += 2
            while j < len(body):
                n = body[j]; nm = body[j+1:j+1+n].decode('latin1'); j += 1+n
                po = struct.unpack_from("<H", body, j)[0]; j += 2; _, j = rd_index(body, j)
                pubs[nm] = po
        elif rt in (0x8a, 0x8b):
            break
    out = []
    if pubs:
        items = sorted(pubs.items(), key=lambda x: x[1])
        for i, (nm, po) in enumerate(items):
            end = items[i+1][1] if i+1 < len(items) else len(image)
            if end > po: out.append((nm, bytes(image[po:end]), bytes(mask[po:end])))
    return out

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
EXTRACT = os.path.join(ROOT, "extracted")

def all_libs():
    pats = ["INCLIBSM/*.LIB","INCLIBSM/LIB/*.LIB","LIB_M_C/*.LIB","LIB_L/*.LIB"]
    out = []
    for p in pats: out += glob.glob(os.path.join(EXTRACT, p))
    return sorted(set(out))

def load_sigs(minlen):
    sigs = []  # (name, code, mask, libbase)
    seen = set()
    for lib in all_libs():
        data = open(lib, "rb").read()
        base = os.path.basename(lib)
        try:
            for ms in iter_modules(data):
                for nm, code, mask in module_functions(data, ms):
                    if len(code) < minlen: continue
                    if code[0] not in (0x55,0x53,0x56,0x57,0x8b,0xb8,0xe9,0x2e,0x9a,0xff,0xf3): continue
                    key = (nm, bytes(code))
                    if key in seen: continue
                    seen.add(key)
                    sigs.append((nm, code, mask, base))
        except Exception as e:
            pass
    return sigs

def load_img(path):
    d = open(path, "rb").read()
    hdr = struct.unpack_from("<H", d, 8)[0] * 16
    return d[hdr:]

def match(img, sigs):
    hits = {}  # off -> (name, len, nfix, lib)
    for nm, code, mask, lib in sigs:
        L = len(code); c0 = code[0]
        pos = img.find(bytes([c0]))
        while pos != -1 and pos + L <= len(img):
            if masked_eq(code, mask, img[pos:pos+L]):
                if pos not in hits or L > hits[pos][1]:
                    hits[pos] = (nm, L, sum(mask), lib)
            pos = img.find(bytes([c0]), pos + 1)
    return hits

if __name__ == "__main__":
    path = sys.argv[1]
    minlen = int(sys.argv[sys.argv.index("--min")+1]) if "--min" in sys.argv else 10
    name = os.path.splitext(os.path.basename(path))[0]
    sigs = load_sigs(minlen)
    img = load_img(path)
    hits = match(img, sigs)
    covered = sum(h[1] for h in hits.values())
    outdir = os.path.join(ROOT, "work", name); os.makedirs(outdir, exist_ok=True)
    rec = {hex(o): {"name": h[0], "len": h[1], "fixups": h[2], "lib": h[3]}
           for o, h in sorted(hits.items())}
    json.dump(rec, open(os.path.join(outdir, "identified.json"), "w"), indent=0)
    # cross-ref carved functions
    fp = os.path.join(outdir, "functions.json")
    carved = json.load(open(fp)) if os.path.exists(fp) else []
    cset = {f["off"] for f in carved}
    id_at_fn = sum(1 for o in hits if o in cset)
    print(f"{name}: {len(sigs)} lib sigs -> {len(hits)} identified functions, "
          f"{covered} bytes ({100*covered/len(img):.0f}% of image)")
    if carved:
        print(f"  {id_at_fn}/{len(carved)} carved functions are library code "
              f"({100*id_at_fn/len(carved):.0f}%)")
    from collections import Counter
    libc = Counter(h[3] for h in hits.values())
    print("  by lib:", dict(libc.most_common(8)))
    for o in list(sorted(hits))[:12]:
        h = hits[o]; print(f"    {o:#07x} {h[0]:<16} {h[1]:>4}b  {h[3]}")
