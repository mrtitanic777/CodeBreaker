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

    from codebreaker.formats import le
    from codebreaker.engines.inverse import vxd_link
    print(vxd_link.build_and_compare(le.LEFile.from_file("vkd.vxd"), "VKD"))
"""
import os
import re
import shutil
import struct
import subprocess
import tempfile

from codebreaker.engines.inverse.vxd_asm import VxDDisasm
from codebreaker.engines.inverse import vxd_masm
from codebreaker.toolchain.backbone.dosbox import run_dos
from codebreaker.core import paths

# MS LINK 2.50 (= the VC++ 2.0 linker). Retained only for the legacy byte-exact object experiments
# (build_and_compare / build_functional). It is the WRONG linker for a loadable VxD: its /VXD leaves the
# device id (LE 0xc0) blank and permutes the page map -- the exact defects the DDK shipped LINK 2.60 to
# fix (see MSVC20\README.TXT). The from-source loadable path uses link_vxd_260() below.
_LINK = "toolchains/period/compilers/msvc-2.0/BIN/LINK.EXE"

# MS LINK 2.60.5046 -- the DDK's own VxD linker (\MSVC20\LINK.EXE), shipped specifically to fix VxD
# linking in 2.50. It stamps the device id from the exported <NAME>_DDB and writes an identity page map,
# so no post-link container patching is needed. It is a native Win32 PE (runs on modern Windows), BUT its
# /VXD self-spawn re-execs LINK by its own module path WITHOUT quoting it, so it MUST run from a
# space-free directory (the DDK's install path has spaces). _stage_link260 copies it out to one.
_LINK260_REF   = ("Microsoft Windows 95 DDK", "MSVC20")   # inside the reference SDK tree (paths.reference)
_LINK260_FILES = ("LINK.EXE", "DBI.DLL", "LINK.ERR", "IMAGEHLP.DLL")

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
    ovs = {o["idx"]: o["vsize"] for o in (D.v.objects if hasattr(D, "v") else [])}
    for (soi, soff), f in list(D.fx.items()):
        if f["kind"] != "internal" or f["target"] is None:
            continue
        tgt = (f["obj"], f["target"])
        if tgt in D.labels:
            continue
        if tgt[0] in ovs and tgt[1] >= ovs[tgt[0]]:   # target beyond the object: an absolute/linear fixup
            continue                                   #  value (0x8000.., 0xFFFF..), not object-relative
        if tgt in D.insns or tgt[1] not in covered.get(tgt[0], ()):   # insn start or a data byte
            D.labels[tgt] = "loc_%d_%04X" % tgt


def _link_body(D, oi, extra_labels=None):
    """Object body for a FUNCTIONAL (loadable, not byte-exact) rebuild: every INTERNAL fixup becomes a
    symbolic reference so LINK /VXD emits a real LE relocation (the byte-exact path raws these, losing
    the relocation -> crash on load). A near call/jmp with a self-relative fixup (E8/E9 + reloc on the
    rel32) is re-emitted as `call/jmp <label>`; any other fixup field is a 32-bit offset, emitted as
    `dd offset <label>` (the instruction split into db + dd + db). `int 20h` service bytes and any
    unlabelled/import fixup stay raw db."""
    from codebreaker.engines.inverse.vxd_masm import _db, _masm_line
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


def _stage_link260(root):
    """Copy LINK 2.60 + its DLLs into a cached SPACE-FREE dir (its /VXD self-spawn cannot handle spaces
    in its own path) and return the staged LINK.EXE path."""
    src = paths.reference(*_LINK260_REF)
    dst = os.path.join(tempfile.gettempdir(), "cb_link260")
    os.makedirs(dst, exist_ok=True)
    for f in _LINK260_FILES:
        d = os.path.join(dst, f)
        if not os.path.exists(d):
            shutil.copy(os.path.join(src, f), d)
    return os.path.join(dst, "LINK.EXE")


def link_vxd_260(obj_bytes, def_text, root, work, align=0x1000):
    """Link one COFF object into a loadable .vxd with the DDK's LINK 2.60 /VXD. Unlike _link_vxd (2.50)
    the device id (LE 0xc0) is stamped from the exported <NAME>_DDB and the page map is identity, so the
    output needs no container patching. `align` is the LE page size (/ALIGN); 0x1000 = the 4096-byte pages
    VKD ships with (LINK 2.60 defaults to 512, which is also valid and used by other shipping VxDs)."""
    link = _stage_link260(root)
    with open(os.path.join(work, "m.obj"), "wb") as f:
        f.write(obj_bytes)
    with open(os.path.join(work, "m.def"), "w") as f:
        f.write(def_text)
    env = dict(os.environ)
    env["PATH"] = os.path.dirname(link) + os.pathsep + env.get("PATH", "")   # self-spawn + DLLs
    args = [link, "/VXD", "/NOD", "/ALIGN:0x%x" % align, "/OUT:m.vxd", "/DEF:m.def", "m.obj"]
    try:
        p = subprocess.run(args, cwd=work, capture_output=True, text=True, timeout=120, env=env)
    except Exception as e:
        return None, "link260 exec failed: %s" % e
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
    from codebreaker.formats import le as _le
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
    from codebreaker.formats import le as _le
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
    incsrc = paths.reference("Microsoft Windows 95 DDK", "INC32")
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


def _stage_inc32(root):
    """Stage the DDK INC32 headers into the toolchain dir (shared by the idiomatic assemblers)."""
    import glob
    tc = vxd_masm._stage_toolchain(root)
    incdst = os.path.join(tc, "inc"); os.makedirs(incdst, exist_ok=True)
    incsrc = paths.reference("Microsoft Windows 95 DDK", "INC32")
    for f in glob.glob(incsrc + "/*.INC"):
        d = os.path.join(incdst, os.path.basename(f))
        if not os.path.exists(d):
            shutil.copy(f, d)
    return tc


def assemble_idiomatic_batch(asm_list, root, budget=200000, maxfiles=24):
    """Assemble many idiomatic DDK-macro sources to COFF in as FEW DOSBox/ML sessions as possible --
    run_dos takes a LIST of commands, so N `ml.exe` calls share one BLD.BAT. Sources are grouped so each
    session stays under `budget` chars and `maxfiles` files (a source over budget runs solo). Returns
    [(obj_bytes|None, log)] aligned with asm_list. The idiomatic form needs ML (its VMM.INC macros are
    beyond nasm), so this is the DOSBox-batching that keeps a corpus build from opening ML per module."""
    tc = _stage_inc32(root)
    results = [(None, "")] * len(asm_list)
    batches, cur, cl = [], [], 0
    for i in sorted(range(len(asm_list)), key=lambda k: len(asm_list[k])):
        sz = len(asm_list[i])
        if sz > budget:
            batches.append([i]); continue
        if cur and (cl + sz > budget or len(cur) >= maxfiles):
            batches.append(cur); cur, cl = [], 0
        cur.append(i); cl += sz
    if cur:
        batches.append(cur)
    for batch in batches:
        files, bat, arts = {}, [], []
        for j, idx in enumerate(batch):
            fn = "m%d" % j
            files[fn + ".asm"] = asm_list[idx]
            bat.append(r"c:\ml.exe -coff -c -DBLD_COFF -DIS_32 -DMASM6 -Ic:\inc -Fo%s.obj %s.asm > %s.txt"
                       % (fn, fn, fn))
            arts += [fn + ".obj", fn + ".txt"]
        tot = sum(len(asm_list[i]) for i in batch)
        _, out, work = run_dos({"c": tc}, bat, arts, files=files, timeout=min(600, 120 + tot // 300))
        for j, idx in enumerate(batch):
            log = out.get("m%d.txt" % j, b"")
            results[idx] = (out.get("m%d.obj" % j), log.decode("latin1", "replace") if log else "")
        if work:
            shutil.rmtree(work, ignore_errors=True)
    return results


def _source_code_into_container(D, v, fs_bytes, min_score=0.5):
    """Overlay the assembled-from-source CODE (from the LINK output `fs_bytes`) onto the recovered
    object bytes `D.code`, then emit through the byte-exact LE writer (vxd_le_writer.build_le), which
    reconstructs the loader-accepted container from the oracle. Returns (vxd_bytes, report).

    Why: LINK 2.50's LE container passes the loader for small VxDs (VMD) but Win95's strict LE loader
    REJECTS it for a dense one (VKD) at load time -- before any VxD code runs -- so the module faults
    with a "Windows protection error". The CODE LINK produces is faithful (only commutative re-encodings
    differ from the oracle); only the container is wrong. So keep the assembled code and drop the
    container: substitute the assembled bytes into each recovered object's D.code, then let the writer
    wrap them in the oracle's exact object table / page map / fixup records.

    Matching an assembled object to its recovered counterpart is the fragile step -- LINK reorders
    segments, and two objects can share a virtual size -- so it is NOT done by size alone (a first-match-
    by-size silently overlays the wrong object's code). Every recovered object is scored against every
    unused assembled object by the OVERLAY WALK itself (fraction of comparable bytes that already agree),
    and pairs are assigned greedily best-first: a correct pair agrees on ~all comparable bytes (the code
    is faithful), a wrong pair agrees only on noise, so the score matrix is a strong diagonal. Any object
    whose best match falls below `min_score`, or for which no assembled object is long enough to cover
    its code, ABORTS the build with a diagnostic (ValueError) rather than shipping scrambled code.

    Byte rules, per object: a fixup's 4-byte field keeps the recovered value (the loader overwrites it
    from the fixup record, and it differs from LINK's), so it is neither scored nor substituted. The DDB
    object's macro-absorbed regions -- control proc + dispatch table, exported service table, and the DDB
    itself, all regenerated by Declare_Virtual_Device / Begin_Control_Dispatch / Begin_Service_Table --
    are not present in the assembled source, so those recovered offsets are skipped in the walk and the
    assembled bytes map only the object's true code around the macro holes."""
    from codebreaker.engines.inverse import vxd_idiom, vxd_le_writer
    from codebreaker.formats import le as _le
    fs = _le.LEFile.from_buffer(fs_bytes)
    # macro-absorbed offsets (regenerated by the DDK macros; never emitted as source code), per object
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
    macro_by_obj, fx_by_obj = {}, {}
    for (o, off) in cp_skip:
        macro_by_obj.setdefault(o, set()).add(off)
    for (oi, off) in D.fx:
        fx_by_obj.setdefault(oi, set()).update(range(off, off + 4))   # whole 4-byte field is loader-written
    asm_bytes = {fo["idx"]: fs.object_bytes(fo) for fo in fs.objects}

    def walk(rec, macro, cand, fx_off):
        """Overlay `cand` (assembled true-code) onto `rec` (recovered object), skipping macro offsets in
        rec and never comparing/substituting a fixup-field byte. Returns (new_bytes, agree, comp): agree
        over comp comparable bytes = how much already matched -- the match quality that ranks/gates."""
        b = bytearray(rec); si = agree = comp = 0
        for off in range(len(b)):
            if off in macro:
                continue
            if si >= len(cand):
                break
            if off not in fx_off:
                comp += 1
                if b[off] == cand[si]:
                    agree += 1
                else:
                    b[off] = cand[si]
            si += 1
        return bytes(b), agree, comp

    # per-object context, and the full (recovered x assembled) score matrix
    meta, scores = {}, []
    for o in v.objects:
        oi = o["idx"]; rec = D.code[oi]
        macro = macro_by_obj.get(oi, set()); fx_off = fx_by_obj.get(oi, set())
        meta[oi] = (rec, macro, fx_off, len(rec) - len(macro))        # expected = bytes the asm must supply
        for aoi, cb in asm_bytes.items():
            _, agree, comp = walk(rec, macro, cb, fx_off)
            scores.append(((agree / comp) if comp else 0.0, oi, aoi, len(cb)))
    scores.sort(key=lambda s: -s[0])                                  # assign strongest pairs first
    pick, taken = {}, set()
    for score, oi, aoi, clen in scores:
        if oi not in pick and aoi not in taken:
            pick[oi] = (aoi, score, clen); taken.add(aoi)

    report = []
    for o in v.objects:
        oi = o["idx"]; rec, macro, fx_off, expected = meta[oi]
        aoi, score, clen = pick.get(oi, (None, 0.0, 0))
        if aoi is None:
            report.append({"obj": oi, "asm_obj": None, "score": 0.0, "cmp": 0,
                           "expected": expected, "asm_len": 0, "macro": len(macro), "ok": False})
            continue
        newb, agree, comp = walk(rec, macro, asm_bytes[aoi], fx_off)
        # comp==0: object is entirely macro/fixup (its bytes are loader-written) -> match on length alone
        ok = clen >= expected and (comp == 0 or score >= min_score)
        if ok:
            D.code[oi] = newb
        report.append({"obj": oi, "asm_obj": aoi, "score": round(score, 4), "cmp": comp,
                       "expected": expected, "asm_len": clen, "macro": len(macro), "ok": ok})

    bad = [r for r in report if not r["ok"]]
    if bad:
        raise ValueError(
            "from-source overlay could not confidently match every object (refusing to ship scrambled "
            "code). Weak/unmatched: "
            + "; ".join("obj%d(asm=%s score=%.3f cmp=%d need_len>=%d got=%d)"
                        % (r["obj"], r["asm_obj"], r["score"], r["cmp"], r["expected"], r["asm_len"])
                        for r in bad)
            + ". Assembled objects (idx:len): "
            + ", ".join("%d:%d" % (aoi, len(cb)) for aoi, cb in sorted(asm_bytes.items())))
    return vxd_le_writer.build_le(D, v, db["name"] if db else "DEVICE"), report


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
    from codebreaker.engines.inverse import vxd_idiom
    from codebreaker.formats import le as _le
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
    try:
        final, overlay = _source_code_into_container(D, v, vxd)   # assembled code -> loader-accepted container
    except ValueError as e:                                       # low-confidence object match -> clean failure
        return {"ok": False, "stage": "overlay", "log": str(e), "asm": asm, "link_log": llog[-300:]}
    return {"ok": True, "vxd": final, "vxd_len": len(final), "oracle_len": len(v.d), "asm": asm,
            "objects": len(_le.LEFile.from_buffer(final).objects), "link_log": llog[-300:],
            "overlay": overlay, "min_overlay_score": min((r["score"] for r in overlay), default=0.0),
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
    from codebreaker.formats import le
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
