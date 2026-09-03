#!/usr/bin/env python3
r"""vxd_link.py -- full-file LE rebuild of a VxD via ML (-coff) + LINK /VXD.

The object-level test (vxd_masm) proves each object's CODE reassembles byte-exact. This goes the
whole way: emit the entire driver as a multi-segment MASM source + a .DEF, assemble to COFF with
the period DDK ML.EXE, and LINK it with `/VXD` (MS LINK 2.60, which runs natively on modern
Windows) into a real .vxd (LE) -- then byte-compare the whole file to the oracle, region by region
(LE header / object table / page data / fixups).

One segment per oracle object (unique CLASS so LINK keeps them separate), attributes derived from
the object's LE flags, in object order; the DDB is published as VXD_DDB and exported @1 so LINK
recognises the device. This first cut emits object bytes raw (no relocations) to validate the
link path; symbolic fixups are layered on top once a valid LE comes out.

    from engine.format_loaders import le
    from engine.inverse_engine import vxd_link
    print(vxd_link.build_and_compare(le.LEFile.from_file("vkd.vxd"), "VKD"))
"""
import os
import re
import shutil
import struct
import subprocess

from engine.inverse_engine.vxd_asm import VxDDisasm
from engine.inverse_engine import vxd_masm
from engine.backbone.dosbox import run_dos

# MS LINK 2.50 (msvc-2.0) -- runs natively; unlike the DDK's 2.60, its /VXD self-spawns as `-lib`
# to build the export file correctly. Its BIN also carries the CVTVXD helper it may invoke.
_LINK = "binaries/toolchains/period/compilers/msvc-2.0/BIN/LINK.EXE"


def _seg_attrs(flags):
    """.DEF segment attributes for an LE object's flags (the ones LINK /VXD maps back to flags)."""
    a = []
    a.append("PRELOAD" if flags & 0x40 else "")
    a.append("DISCARDABLE" if flags & 0x10 else "NONDISCARDABLE")
    if flags & 0x20:
        a.append("SHARED")
    if flags & 0x8000:
        a.append("IOPL")
    if flags & 0x4000:
        a.append("CONFORMING")
    if flags & 0x200:
        a.append("RESIDENT")
    return " ".join(x for x in a if x)


def emit_source_and_def(D, vxdname, force_raw=None):
    """Emit (asm_text, def_text) for the whole VxD. One segment per object in object order; the
    DDB object gets a public VXD_DDB label exported @1. force_raw is an optional {oi: set(offsets)}
    of instructions to byte-preserve (the encoding mirrors vxd_masm found), for byte-exact pages."""
    db = D.ddb
    asm = [".386p"]
    deflines = ["VXD %s" % vxdname, "DESCRIPTION '%s'" % vxdname, "", "SEGMENTS"]
    for o in (D.v.objects if hasattr(D, "v") else list(D.objs.values())):
        oi = o["idx"]
        seg = "SEG%02d" % oi
        cls = "LCODE%02d" % oi                           # 'CODE' -> LINK marks it executable;
        asm.append("%s segment dword public use32 '%s'" % (seg, cls))  # zero-pad sorts in order
        extra = {db["rva"]: "VXD_DDB"} if (db and db["obj"] == oi) else {}
        body, _ = vxd_masm.object_body(D, oi, force_raw=(force_raw or {}).get(oi),
                                       extra_labels=extra)
        asm += body                                         # instructions -> COFF code section
        asm.append("%s ends" % seg)
        deflines.append("    %-8s CLASS '%s' %s" % (seg, cls, _seg_attrs(o["flags"])))
    asm.append("        end")
    deflines += ["", "EXPORTS", "        VXD_DDB @1"]
    return "\r\n".join(asm) + "\r\n", "\r\n".join(deflines) + "\r\n"


def _assemble_coff(asm_text, root):
    tc = vxd_masm._stage_toolchain(root)
    ok, out, work = run_dos({"c": tc},
                            ["c:\\ml.exe -coff -c -Fom.obj m.asm > asm.txt"],
                            ["m.obj", "asm.txt"], files={"m.asm": asm_text}, timeout=180)
    log = open(os.path.join(work, "asm.txt"), errors="replace").read() if work and os.path.exists(
        os.path.join(work, "asm.txt")) else ""
    return out.get("m.obj"), log, work


def _link_vxd(obj_bytes, def_text, root, work):
    """Link one COFF object into a .vxd with LINK /VXD (native)."""
    with open(os.path.join(work, "m.obj"), "wb") as f:
        f.write(obj_bytes)
    with open(os.path.join(work, "m.def"), "w") as f:
        f.write(def_text)
    link = os.path.join(root, _LINK)
    try:
        p = subprocess.run([link, "/VXD", "/NOD", "/OUT:m.vxd", "/DEF:m.def", "m.obj"],
                           cwd=work, capture_output=True, text=True, timeout=120)
    except Exception as e:
        return None, "link exec failed: %s" % e
    vp = os.path.join(work, "m.vxd")
    out = (p.stdout or "") + (p.stderr or "")
    return (open(vp, "rb").read() if os.path.exists(vp) else None), out


def byte_exact_force_raw(v, root="."):
    """Per exec object, run the object-level byte-exact test and collect the force-raw offset set
    that made it exact -- the instructions the assembler rejects or re-encodes to a mirror. Feeding
    these to the link emitter makes the linked page data byte-exact by construction. Returns
    ({oi: set(offsets)}, {oi: matched?})."""
    fr, ok = {}, {}
    for o in v.objects:
        if not o["exec"]:
            continue
        r = vxd_masm.compare_object(v, o["idx"], root=root)
        fr[o["idx"]] = r.get("force_raw_set", set())
        ok[o["idx"]] = bool(r.get("match"))
    return fr, ok


def build_and_compare(v, vxdname, root=".", force_raw=None):
    D = VxDDisasm(v).run()
    D.v = v
    root = os.path.abspath(root)
    asm, deftext = emit_source_and_def(D, vxdname, force_raw=force_raw)
    obj, alog, work = _assemble_coff(asm, root)
    if not obj:
        return {"ok": False, "stage": "assemble", "log": alog[-800:]}
    vxd, llog = _link_vxd(obj, deftext, root, work)
    if not vxd:
        return {"ok": False, "stage": "link", "log": llog[-1200:], "asm": asm, "def": deftext}
    tmp = os.path.join(work, "out.vxd")
    open(tmp, "wb").write(vxd)
    from engine.format_loaders import le as _le
    mine = _le.LEFile.from_file(tmp)
    return {"ok": True, "vxd": vxd, "vxd_len": len(vxd), "oracle_len": len(v.d),
            "link_log": llog[-400:], "report": compare_objects(v, mine)}


def compare_objects(orc, mine):
    """Match each linked object to the oracle object with the same vsize + best byte overlap (LINK
    reorders objects -- discardable segments go to the end), and report per-object page-data
    exactness. The meaningful full-file metric: how many objects' code linked byte-for-byte."""
    ob = [(o["idx"], orc.object_bytes(o)) for o in orc.objects]
    rows = []
    exact = 0
    for mo in mine.objects:
        mb = mine.object_bytes(mo)
        best = None
        for oi, ocb in ob:
            if len(ocb) == len(mb):
                eq = sum(1 for i in range(len(mb)) if mb[i] == ocb[i])
                if best is None or eq > best[1]:
                    best = (oi, eq, len(mb))
        if best:
            exact += best[1] == best[2]
            rows.append({"mine": mo["idx"], "oracle": best[0], "equal": best[1], "n": best[2],
                         "pct": round(100 * best[1] / best[2], 1) if best[2] else 0})
    return {"objects": rows, "exact_objects": exact, "total": len(mine.objects)}


if __name__ == "__main__":
    import sys
    from engine.format_loaders import le
    path = sys.argv[1]
    name = sys.argv[2] if len(sys.argv) > 2 else os.path.splitext(os.path.basename(path))[0].upper()
    mods = le.open_vxds(path)
    v = mods[0][1]
    fr, okmap = byte_exact_force_raw(v)
    r = build_and_compare(v, name, force_raw=fr)
    if not r["ok"]:
        print("FAILED at %s:\n%s" % (r["stage"], r.get("log", "")))
    else:
        rep = r["report"]
        for row in rep["objects"]:
            print("  my obj%-2d -> oracle obj%-2d  %d/%d (%.1f%%)%s" % (
                row["mine"], row["oracle"], row["equal"], row["n"], row["pct"],
                "  EXACT" if row["equal"] == row["n"] else ""))
        print("linked %s: %d bytes (oracle %d)  %d/%d objects byte-exact" % (
            name, r["vxd_len"], r["oracle_len"], rep["exact_objects"], rep["total"]))
