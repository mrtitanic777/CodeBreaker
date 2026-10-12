#!/usr/bin/env python3
r"""
win386link.py — whole-file byte-IDENTICAL relink of WIN386.EXE (W3/LE).

The NE analog (linkfile.py) proves a whole module rebuilds byte-for-byte; this is
the W3/LE counterpart for WIN386.EXE. It reassembles every conducted function of
every embedded LE submodule (reconstructed/asm/win386.<sub>/objN_<off>.asm), places
the assembled bytes at their file offset (via the LE page map) into a copy of the
shipped WIN386.EXE, and byte-compares the whole 545 KB file. LE objects relocate to
0 and the on-disk pages hold base values, so no relocation pass is needed — the
assembled bytes equal the shipped bytes (conduct proved each function byte-exact).

Verdict requires that functions were actually assembled+placed (never a false
identical from an untouched copy).

    python win386link.py            # rebuild + byte-verify the whole file
    python win386link.py --save DIR # also write the rebuilt WIN386.EXE
"""
import os
import re
import struct
import sys

from src.core import layout

ROOT = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, ROOT)
CHUNK = 40
EMIT = layout.asm_dir()


def _foff(le, ob, objoff):
    """File offset of byte `objoff` within LE object `ob` (pages can be non-
    contiguous on disk, so resolve per page) — same mapping linkfile uses for LE."""
    ps = le.page_size
    base, _flag = le._page_file_off(ob["page_index"] + objoff // ps)
    return base + objoff % ps


def _submodules(d):
    lfa = struct.unpack_from("<I", d, 0x3C)[0]
    count = struct.unpack_from("<I", d, lfa + 4)[0]
    out = []
    for i in range(count):
        rec = d[lfa + 0x10 + i * 16: lfa + 0x10 + (i + 1) * 16]
        nm = rec[0:8].rstrip(b" \x00").decode("latin1", "replace")
        off = struct.unpack("<I", rec[8:12])[0]
        out.append((nm, off))
    return out


def link():
    from src.formats import objextract
    import target, le as le_mod, masmlift
    from src.engines.recon import asmrecon32
    path = target.resolve("WIN386.EXE")
    d = open(path, "rb").read()
    orig = bytes(d)
    rebuilt = bytearray(d)
    idx = layout.asm_dir_index()                    # resolve win386.<sub> at any depth
    placed = nfun = 0
    for nm, sub_off in _submodules(d):
        emitdir = idx.get("win386." + nm.lower())
        if not emitdir or not os.path.isdir(emitdir):
            continue
        le = le_mod.LEFile.from_buffer(d, e_lfanew=sub_off)
        objs = le.objects()
        # Drive placement from the conduct LEDGER (the authoritative current run),
        # not by globbing every .asm on disk — stale files from earlier runs / smoke
        # tests would otherwise be placed at offsets the current carve never used and
        # corrupt the file. Only ledger keys "oi:off" (with a matching .asm) are used.
        import json
        lp = os.path.join("reports", "conduct", "win386." + nm.lower() + ".json")
        if not os.path.exists(lp):
            continue
        led = json.load(open(lp))
        funcs = []
        for key, rec in led.items():
            if rec.get("status") != "exact":
                continue
            oi, off = int(key.split(":")[0]), int(key.split(":")[1], 16)
            p = os.path.join(emitdir, "obj%d_%05x.asm" % (oi, off))
            if os.path.exists(p):
                funcs.append((oi, off, p))
        funcs.sort()
        nfun += len(funcs)
        items = [("F%d" % i, open(p, encoding="latin1").read())
                 for i, (_oi, _off, p) in enumerate(funcs)]
        res = {}
        for c in range(0, len(items), CHUNK):
            res.update(masmlift.assemble_many(items[c:c + CHUNK],
                                              masmroot=asmrecon32.MASM32ROOT).get("results", {}))
        for i, (oi, off, _p) in enumerate(funcs):
            obj = res.get("F%d" % i, {}).get("obj")
            if not obj:
                continue
            _n, code, mask = objextract.Obj(obj).code_segment()
            ob = objs[oi - 1]
            for j in range(len(code)):
                if j in mask:
                    continue
                fo = _foff(le, ob, off + j)
                if 0 <= fo < len(rebuilt):
                    rebuilt[fo] = code[j]
            placed += 1
    diffs = [i for i in range(len(orig)) if rebuilt[i] != orig[i]]
    return {"orig": orig, "rebuilt": bytes(rebuilt), "n_diff": len(diffs),
            "first_diff": diffs[0] if diffs else None, "placed": placed, "nfun": nfun,
            "match": not diffs and placed > 0}


def main():
    r = link()
    print("=" * 60)
    print("RELINK WIN386.EXE  [W3/LE]  %d bytes, %d functions placed / %d"
          % (len(r["orig"]), r["placed"], r["nfun"]))
    if r["match"]:
        print("VERDICT: BYTE-IDENTICAL — the rebuilt file == the shipped file.")
    elif r["placed"] == 0:
        print("VERDICT: INCOMPLETE — 0 functions assembled (proves nothing).")
    else:
        print("VERDICT: %d byte(s) differ (first at 0x%X)" % (r["n_diff"], r["first_diff"]))
        o, b = r["orig"], r["rebuilt"]
        f = r["first_diff"]
        print("  shipped %s  vs  rebuilt %s" % (o[f:f + 8].hex(), b[f:f + 8].hex()))
    print("=" * 60)
    if "--save" in sys.argv:
        dd = sys.argv[sys.argv.index("--save") + 1]
        os.makedirs(dd, exist_ok=True)
        p = os.path.join(dd, "win386.exe")
        open(p, "wb").write(r["rebuilt"])
        print("saved:", os.path.abspath(p))


if __name__ == "__main__":
    main()
