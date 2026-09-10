#!/usr/bin/env python3
"""
split_bundle.py — Carve the concatenated NE modules out of WIN100.BIN
(the Windows 1.0 boot image) and report each module's full API surface:
its exports and, crucially, what it imports from the other modules.

This reconstructs the Windows 1.0 module dependency graph.
"""
import struct, sys, os, re
from codebreaker.format_loaders.ne import NEFile, bits, SEG_FLAGS

def find_ne_modules(data):
    """Locate every embedded NE header that looks structurally valid."""
    mods = []
    for i in range(0x100, len(data)-2):
        if data[i:i+2] != b"NE":
            continue
        try:
            nseg   = struct.unpack_from("<H", data, i+0x1c)[0]
            segtab = struct.unpack_from("<H", data, i+0x22)[0]
            resnm  = struct.unpack_from("<H", data, i+0x26)[0]
            if not (0 < nseg < 64 and 0x20 <= segtab < 0x100 and 0 < resnm < 0x400):
                continue
            p = i + resnm; ln = data[p]
            if not (0 < ln < 32):
                continue
            name = data[p+1:p+1+ln].decode("latin1")
            if not name.isascii() or not name[0].isalpha():
                continue
            mods.append((i, name))
        except Exception:
            pass
    return mods

def analyze(path, out_dir=None):
    data = open(path, "rb").read()
    mods = find_ne_modules(data)
    print(f"{os.path.basename(path)}: {len(mods)} embedded NE modules\n")
    offsets = [m[0] for m in mods] + [len(data)]

    summary = []
    for idx, (off, name) in enumerate(mods):
        ne = NEFile.from_buffer(data, off, path)
        res = ne.resident_names()
        nonres = ne.nonresident_names()
        exports = [(n,o) for n,o in res[1:]]
        # non-resident exports are addressed from file start; skip if bogus
        mrefs = ne.module_refs()
        segs = ne.segments()
        extent = offsets[idx+1] - off
        summary.append(dict(name=name, off=off, extent=extent, n_segs=ne.h["n_segs"],
                            flags=ne.h["flags"], exports=exports, imports=mrefs,
                            desc=nonres[0][0] if nonres else ""))
        if out_dir:
            os.makedirs(out_dir, exist_ok=True)
            open(os.path.join(out_dir, f"{name}.ne"), "wb").write(data[off:offsets[idx+1]])

    # Pretty report
    for m in summary:
        kind = "DLL" if m["flags"] & 0x8000 else "PROGRAM"
        print("="*70)
        print(f"{m['name']:10} @0x{m['off']:06x}  ~{m['extent']:>6} bytes  "
              f"{m['n_segs']:>2} segs  [{kind}]")
        if m["imports"]:
            print(f"   imports: {', '.join(m['imports'])}")
        print(f"   exports {len(m['exports'])}:")
        line = "     "
        for n,o in sorted(m["exports"], key=lambda x:x[1]):
            tok = f"{n}@{o}"
            if len(line)+len(tok) > 68:
                print(line); line="     "
            line += tok + "  "
        if line.strip(): print(line)
    return summary

if __name__ == "__main__":
    path = sys.argv[1] if len(sys.argv) > 1 else "files/WF/WIN100.BIN"
    out  = sys.argv[2] if len(sys.argv) > 2 else "modules"
    analyze(path, out)
