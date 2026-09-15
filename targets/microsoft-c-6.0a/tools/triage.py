#!/usr/bin/env python3
"""Phase-3 origin triage: match a pass image against a set of period .LIBs, tag every
carved function as library (named) or authored. Reuses CodeBreaker's libmatch internals.

Usage: python triage.py <unpacked.exe> <functions.json> <out.json> <lib1> [lib2 ...]
"""
import sys, os, json
sys.path.insert(0, os.environ.get('CB_ROOT', r'C:\Users\mrtit\Documents\GitHub\CodeBreaker'))
from src.engines.inverse.libmatch import iter_modules, module_functions, masked_eq

def u16(b, o): return b[o] | (b[o+1] << 8)

def main():
    exe, fj, outp = sys.argv[1:4]; libs = sys.argv[4:]
    d = open(exe, 'rb').read(); h = u16(d, 8)*16
    e_cp, e_cblp = u16(d, 4), u16(d, 2)
    img = d[h:e_cp*512 - ((512-e_cblp) if e_cblp else 0)]
    funcs = json.load(open(fj))['functions']
    starts = {f['start'] for f in funcs}

    # collect (name, code, mask) from all libs; index first byte -> occurrences in img
    hits = {}   # img_off -> (libname, funcname, length, nfixups)
    for lib in libs:
        ln = os.path.basename(lib)
        data = open(lib, 'rb').read()
        libfns = []
        for ms in iter_modules(data):
            for nm, code, mask in module_functions(data, ms):
                if len(code) >= 8 and code[0] in (0x55,0x53,0x56,0x57,0x8b,0xb8,0xe9,0x2e,0x1e,0x06):
                    libfns.append((nm, code, mask))
        for nm, code, mask in libfns:
            L = len(code); c0 = code[0]; pos = img.find(bytes([c0]))
            while pos != -1 and pos+L <= len(img):
                if masked_eq(code, mask, img[pos:pos+L]):
                    if pos not in hits or L > hits[pos][2]:
                        hits[pos] = (ln, nm, L, sum(mask))
                pos = img.find(bytes([c0]), pos+1)

    # tag carved functions
    out = []; nlib = 0
    for f in funcs:
        s = f['start']
        if s in hits:
            ln, nm, L, nf = hits[s]
            out.append({**f, 'origin': 'library', 'lib': ln, 'name': nm}); nlib += 1
        else:
            out.append({**f, 'origin': 'authored'})
    covered = bytearray(len(img))
    for pos, (ln, nm, L, nf) in hits.items():
        for k in range(L): covered[pos+k] = 1
    json.dump({'functions': out, 'lib_hits': len(hits)}, open(outp, 'w'), indent=1)
    print(f"{os.path.basename(exe)}: {nlib}/{len(funcs)} funcs = library (named), "
          f"{len(funcs)-nlib} authored; {sum(covered)} img bytes "
          f"({sum(covered)*100//len(img)}%) identified as library code "
          f"[{len(hits)} placements from {len(libs)} libs]")

if __name__ == '__main__':
    main()
