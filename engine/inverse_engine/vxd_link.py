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
# to build the export file correctly.
_LINK = "binaries/toolchains/period/compilers/msvc-2.0/BIN/LINK.EXE"

# standard DDK VxD segment map (verbatim from BASE/SAMPLES/*/*.DEF -- this is what boots VMD from
# source); LINK ignores segments absent from the object, so one template serves every VxD.
_VXD_DEF_SEGS = (
    "    _LPTEXT     CLASS 'LCODE'   PRELOAD NONDISCARDABLE\r\n"
    "    _LTEXT      CLASS 'LCODE'   PRELOAD NONDISCARDABLE\r\n"
    "    _LDATA      CLASS 'LCODE'   PRELOAD NONDISCARDABLE\r\n"
    "    _TEXT       CLASS 'LCODE'   PRELOAD NONDISCARDABLE\r\n"
    "    _DATA       CLASS 'LCODE'   PRELOAD NONDISCARDABLE\r\n"
    "    CONST       CLASS 'LCODE'   PRELOAD NONDISCARDABLE\r\n"
    "    _TLS        CLASS 'LCODE'   PRELOAD NONDISCARDABLE\r\n"
    "    _BSS        CLASS 'LCODE'   PRELOAD NONDISCARDABLE\r\n"
    "    _ITEXT      CLASS 'ICODE'   DISCARDABLE\r\n"
    "    _IDATA      CLASS 'ICODE'   DISCARDABLE\r\n"
    "    _PTEXT      CLASS 'PCODE'   NONDISCARDABLE\r\n"
    "    _PDATA      CLASS 'PDATA'   NONDISCARDABLE SHARED\r\n"
    "    _STEXT      CLASS 'SCODE'   RESIDENT\r\n"
    "    _SDATA      CLASS 'SCODE'   RESIDENT\r\n"
    "    _16ICODE    CLASS '16ICODE' PRELOAD DISCARDABLE\r\n"
    "    _RCODE      CLASS 'RCODE'\r\n")


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


def _label_fixup_targets(D):
    """Give every INTERNAL fixup target a label so the functional emit can reference it: the
    disassembler leaves many cross-object targets it never reached anonymous. Skip a target that
    lands mid-instruction (the emit walk steps instruction-by-instruction and never lands there)."""
    covered = {}                                           # oi -> offsets INSIDE an instruction
    for (oi, a), row in D.insns.items():
        s = covered.setdefault(oi, set())
        for k in range(a + 1, a + row["size"]):
            s.add(k)
    for (soi, soff), f in list(D.fx.items()):
        if f["kind"] != "internal" or f["target"] is None:
            continue
        tgt = (f["obj"], f["target"])
        if tgt in D.labels:
            continue
        if tgt in D.insns or tgt[1] not in covered.get(tgt[0], ()):   # insn start or a data byte
            D.labels[tgt] = "loc_%d_%04X" % tgt


def _link_body(D, oi, extra_labels=None):
    """Object body for a FUNCTIONAL (loadable, not byte-exact) rebuild: every INTERNAL fixup becomes a
    symbolic reference so LINK /VXD emits a real LE relocation (the byte-exact path raws these, losing
    the relocation -> crash on load). A near call/jmp with a self-relative fixup (E8/E9 + reloc on the
    rel32) is re-emitted as `call/jmp <label>`; any other fixup field is a 32-bit offset, emitted as
    `dd offset <label>` (the instruction split into db + dd + db). `int 20h` service bytes and any
    unlabelled/import fixup stay raw db."""
    from engine.inverse_engine.vxd_masm import _db, _masm_line
    extra_labels = extra_labels or {}
    code = D.code[oi]
    n = len(code)
    L = []
    a = 0

    def _lbl(f):
        return D.labels.get((f["obj"], f["target"]))

    def _reloc(k):                                         # an internal fixup at k with a known label
        f = D.fx.get((oi, k))
        return f and f["kind"] == "internal" and _lbl(f)

    while a < n:
        if a in extra_labels:
            L.append("public %s" % extra_labels[a])
            L.append("%s label byte" % extra_labels[a])
        if D.labels.get((oi, a)):
            L.append("%s:" % D.labels[(oi, a)])
        row = D.insns.get((oi, a))
        if row is None:                                    # data
            if _reloc(a):
                L.append("        dd  offset %s" % _lbl(D.fx[(oi, a)]))
                a += 4
                continue
            b = a                                          # raw bytes to the next label / fixup / insn
            while b < n and (oi, b) not in D.insns and not (b > a and D.labels.get((oi, b))) \
                    and not (b > a and (oi, b) in D.fx) and not (b > a and b in extra_labels):
                b += 1
            for i in range(a, b, 16):
                L.append(_db(code[i:min(i + 16, b)]))
            a = b
            continue
        size = row["size"]
        op = code[a]
        # near call/jmp whose rel32 carries the (self-relative) fixup -> symbolic branch
        if size == 5 and op in (0xE8, 0xE9) and _reloc(a + 1):
            f = D.fx[(oi, a + 1)]
            L.append("        %-6s %s" % ("call" if op == 0xE8 else "jmp", _lbl(f)))
            a += size
            continue
        fixes = sorted(k for k in range(a, a + size) if (oi, k) in D.fx)
        if fixes and not row["vxdcall"] and all(_reloc(k) for k in fixes):
            prev = a                                       # 32-bit-offset fixups: split around fields
            for k in fixes:
                if k > prev:
                    L.append(_db(code[prev:k]))
                L.append("        dd  offset %s" % _lbl(D.fx[(oi, k)]))
                prev = k + 4
            if prev < a + size:
                L.append(_db(code[prev:a + size]))
        elif row["vxdcall"] or fixes or row["fx"]:
            L.append(_db(row["raw"]))                      # int 20h service, or a fixup we can't symbolize
        else:
            line = _masm_line(D, oi, row)
            L.append(line if line is not None else _db(row["raw"]))
        a += size
    return L


def emit_source_and_def(D, vxdname, force_raw=None, functional=False):
    """Emit (asm_text, def_text) for the whole VxD. One segment per object in object order; the
    DDB object gets a public VXD_DDB label exported @1. force_raw is an optional {oi: set(offsets)}
    of instructions to byte-preserve (the encoding mirrors vxd_masm found), for byte-exact pages.
    functional=True instead emits internal fixups symbolically (via _link_body) so the linked VxD
    carries real relocations -- loadable, though its container is not byte-exact."""
    db = D.ddb
    objs = list(D.v.objects if hasattr(D, "v") else D.objs.values())
    if functional:
        _label_fixup_targets(D)                          # every internal fixup target gets a label
    asm = [".386p"]
    deflines = ["VXD %s" % vxdname, "DESCRIPTION '%s'" % vxdname, "", "SEGMENTS"]
    for o in objs:
        oi = o["idx"]
        seg = "SEG%02d" % oi
        cls = "LCODE%02d" % oi                           # 'CODE' -> LINK marks it executable;
        asm.append("%s segment dword public use32 '%s'" % (seg, cls))  # zero-pad sorts in order
        extra = {db["rva"]: "VXD_DDB"} if (db and db["obj"] == oi) else {}
        if functional:
            body = _link_body(D, oi, extra_labels=extra)
        else:
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


def build_functional(v, vxdname, root="."):
    """Build a LOADABLE .vxd (relocations present, container NOT byte-exact): emit internal fixups
    symbolically, assemble, link with LINK /VXD, and report the LE fixup count vs the oracle. A
    rebuilt-internal count near the oracle's is the proof the relocations survived. Returns
    {ok, vxd, oracle_internal, rebuilt_internal, ...}."""
    D = VxDDisasm(v).run()
    D.v = v
    root = os.path.abspath(root)
    asm, deftext = emit_source_and_def(D, vxdname, functional=True)
    obj, alog, work = _assemble_coff(asm, root)
    if not obj:
        return {"ok": False, "stage": "assemble", "log": alog[-1500:], "asm": asm}
    vxd, llog = _link_vxd(obj, deftext, root, work)
    if not vxd:
        return {"ok": False, "stage": "link", "log": llog[-1500:], "asm": asm, "def": deftext}
    from engine.format_loaders import le as _le
    tmp = os.path.join(work, "func.vxd")
    open(tmp, "wb").write(vxd)
    mine = _le.LEFile.from_file(tmp)
    ofx, mfx = v.fixups(), mine.fixups()
    oint = sum(1 for f in ofx.values() if f["kind"] == "internal")
    mint = sum(1 for f in mfx.values() if f["kind"] == "internal")
    return {"ok": True, "vxd": vxd, "vxd_len": len(vxd), "oracle_len": len(v.d),
            "oracle_fixups": len(ofx), "oracle_internal": oint,
            "rebuilt_fixups": len(mfx), "rebuilt_internal": mint, "link_log": llog[-400:]}


def _assemble_idiomatic(asm_text, root):
    """Assemble idiomatic DDK-macro source to COFF -- needs the DDK INC32 headers on the include path
    (the raw-segment build in _assemble_coff does not include them)."""
    import glob
    tc = vxd_masm._stage_toolchain(root)
    incdst = os.path.join(tc, "inc"); os.makedirs(incdst, exist_ok=True)
    incsrc = os.path.join(root, "binaries", "media", "Microsoft Windows 95 DDK", "INC32")
    for f in glob.glob(incsrc + "/*.INC"):
        d = os.path.join(incdst, os.path.basename(f))
        if not os.path.exists(d):
            shutil.copy(f, d)
    _, out, work = run_dos({"c": tc},
                           [r"c:\ml.exe -coff -c -DBLD_COFF -DIS_32 -DMASM6 -Ic:\inc -Fom.obj m.asm > a.txt"],
                           ["m.obj", "a.txt"], files={"m.asm": asm_text}, timeout=300)
    log = open(os.path.join(work, "a.txt"), errors="replace").read() if work and os.path.exists(
        os.path.join(work, "a.txt")) else ""
    return out.get("m.obj"), log, work


def _source_code_into_container(D, v, fs_bytes):
    """Overlay the assembled-from-source CODE (from the LINK output `fs_bytes`) onto the recovered
    object bytes `D.code`, then emit through the byte-exact LE writer (vxd_le_writer.build_le), which
    reconstructs the loader-accepted container from the oracle.

    Why: LINK 2.50's LE container passes the loader for small VxDs (VMD) but Win95's strict LE loader
    REJECTS it for a dense one (VKD) at load time -- before any VxD code runs -- so the module faults
    with a "Windows protection error". The CODE it links is faithful (only commutative re-encodings
    differ from the oracle); only the container is wrong. So we keep the assembled code and drop the
    container: for each recovered object, substitute the assembled bytes into D.code, then let the
    byte-exact writer wrap them in the oracle's exact object table / page map / fixup records.

    A fixup's 4-byte field keeps the recovered value (the loader overwrites it from the fixup record).
    The DDB object's macro-absorbed regions (control proc + dispatch table, exported service table, and
    the DDB itself -- all regenerated by Declare_Virtual_Device/Begin_Control_Dispatch/Begin_Service_
    Table) keep the recovered bytes; only its true code takes the assembled bytes. Objects are matched
    to their assembled counterpart by virtual size (LINK may reorder), so it is layout-independent."""
    from engine.inverse_engine import vxd_idiom, vxd_le_writer
    from engine.format_loaders import le as _le
    fs = _le.LEFile.from_buffer(fs_bytes)
    emittable = {(o, a) for (o, a) in D.insns}
    handlers, cp_skip = vxd_idiom.control_dispatch(D, emittable)
    db = D.ddb
    if db:
        cp_skip = set(cp_skip) | {(db["obj"], k)
                                  for k in range(db["rva"], db["rva"] + vxd_idiom._ddb_size(D, db))}
    svc_labels, svc_loc, svc_extra = vxd_idiom.service_table(D, emittable - cp_skip)
    if svc_loc:
        so, sp, sc = svc_loc
        cp_skip = cp_skip | {(so, k) for k in range(sp, sp + 4 * sc)}
    ddb_obj = db["obj"] if db else 1
    macro_off = {off for (o, off) in cp_skip if o == ddb_obj}
    fxbytes = {(oi, off + j) for (oi, off) in D.fx for j in range(4)}
    used = set()

    def take(vsize):                                        # next unused assembled object of this size
        for fo in fs.objects:
            if fo["idx"] not in used and fo["vsize"] == vsize:
                used.add(fo["idx"]); return fs.object_bytes(fo)
        return b""
    for k in range(1, len(v.objects) + 1):
        b = bytearray(D.code[k])
        if k != ddb_obj:
            src = take(v.objects[k - 1]["vsize"])
            for i in range(min(len(b), len(src))):
                if b[i] != src[i] and (k, i) not in fxbytes:
                    b[i] = src[i]
        else:                                              # DDB object: its code minus the macro regions
            src = take(len(b) - len(macro_off)); si = 0
            for off in range(len(b)):
                if off in macro_off:
                    continue                               # keep recovered byte (macro regenerates it)
                if si < len(src):
                    if b[off] != src[si] and (k, off) not in fxbytes:
                        b[off] = src[si]
                    si += 1
        D.code[k] = bytes(b)
    return vxd_le_writer.build_le(D, v, db["name"] if db else "DEVICE")


def build_loadable_idiomatic(v, name=None, root="."):
    """Build a LOADABLE LE .vxd from the *idiomatic DDK-macro* source and the real toolchain:
    vxd_idiom.emit_idiomatic(loadable=True) -> ML (-coff) -> LINK /VXD -> (byte-exact container writer).
    Unlike build_functional (raw per-object segments, which hit A2107 far-call errors on cross-object
    references), the DDK segment macros set up the flat model, so cross-object near calls assemble;
    loadable mode routes every internal fixup symbolically so LINK writes real LE relocations. LINK's
    own LE container loads for small VxDs (VMD ran to the desktop with a working mouse) but Win95's LE
    loader rejects it for a dense one (VKD faults before init), so the assembled code is re-wrapped in
    the loader-accepted container via _source_code_into_container -- with which a from-source VKD boots
    the real Win95 VMM to the logon. Returns {ok, vxd, objects, rebuilt_internal, oracle_internal, ...}."""
    from engine.inverse_engine import vxd_idiom
    from engine.format_loaders import le as _le
    D = VxDDisasm(v).run(); D.v = v
    name = name or (D.ddb and D.ddb["name"])
    root = os.path.abspath(root)
    asm = vxd_idiom.emit_idiomatic(D, loadable=True)
    obj, alog, work = _assemble_idiomatic(asm, root)
    if not obj:
        return {"ok": False, "stage": "assemble", "log": alog[-1500:], "asm": asm}
    perobj = ""                                             # one .DEF entry per object (unique class) so
    for o in v.objects:                                    # LINK keeps each object separate instead of
        sn, cl = vxd_idiom.obj_seg(o["idx"], o["flags"])   # merging by class. Fixed PRELOAD NONDISCARDABLE
        perobj += "    %-11s CLASS '%s' PRELOAD NONDISCARDABLE\r\n" % (sn, cl)   # (real flags come from the
    #                                                       byte-exact writer, not LINK): the DISCARDABLE
    #  attribute makes LINK reorder objects (discardable to the end), which desyncs the code overlay.
    deftext = ("VXD %s\r\nDESCRIPTION '%s reconstructed from source'\r\n\r\nSEGMENTS\r\n%s%s\r\n"
               "EXPORTS\r\n        %s_DDB @1\r\n" % (name, name, _VXD_DEF_SEGS, perobj, name))
    vxd, llog = _link_vxd(obj, deftext, root, work)
    if not vxd:
        return {"ok": False, "stage": "link", "log": llog[-1500:], "asm": asm, "def": deftext}
    link_mine = _le.LEFile.from_buffer(vxd); mfx = link_mine.fixups(); ofx = v.fixups()
    final = _source_code_into_container(D, v, vxd)          # assembled code -> loader-accepted container
    return {"ok": True, "vxd": final, "vxd_len": len(final), "oracle_len": len(v.d), "asm": asm,
            "objects": len(_le.LEFile.from_buffer(final).objects), "link_log": llog[-300:],
            "rebuilt_internal": sum(1 for f in mfx.values() if f["kind"] == "internal"),
            "oracle_internal": sum(1 for f in ofx.values() if f["kind"] == "internal")}


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
