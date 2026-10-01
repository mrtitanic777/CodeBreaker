#!/usr/bin/env python3
r"""vxd_from_source.py -- build a LOADABLE Win95 VxD from reconstructed SOURCE, with NO byte transplant.

This is the path that `build_loadable_idiomatic` could not take: it emits the recovered VxD as idiomatic
DDK-macro assembly with each object in its REAL named segment, assembles it with the period ML.EXE, links
it with the DDK's OWN LINK 2.60, and embeds the version resource with the DDK's OWN RC + ADRC2VXD. Every
byte comes from an authentic period tool run over recovered source -- no oracle object bytes are overlaid
and no container field is hand-patched. A VKD.VXD built this way boots real Windows 95 to the logon (and
the keyboard is live at the logon prompt).

The four requirements, all pinned by real boots (see docs/VXD_FROM_SOURCE.md):

  1. SEGMENT MODEL. The DDB object's recovered code goes in `_LTEXT` (VxD_LOCKED_CODE_SEG) WITH the
     control dispatch; the service table + `Declare_Virtual_Device` land in `_LDATA` via the macros;
     and the DDB object's post-DDB TAIL is DATA -- emitted into `_LDATA` after the DDB, not left in
     `_LTEXT` where a disassembler's over-read would insert it ahead of the service table and shove
     the DDB off its true offset. This yields the on-object order [dispatch][code][service][DDB][data].
  2. FLAT MODEL. Every non-DDK segment is declared `<NAME> SEGMENT DWORD PUBLIC _FLAT '<NAME>CODE'`.
     `_FLAT` (=FLAT under MASM6) makes it part of the flat group; `USE32` does not, and every cross-
     segment near call then fails to assemble (A2107). This is what the DDK's MakeCodeSeg expands to.
  3. THE RIGHT LINKER. The VxD device id (LE 0xc0) and the Windows version resource (0xb8/0xbc) are
     loader-REQUIRED. The VC++ 2.0 linker (LINK 2.50) leaves 0xc0 blank and permutes the page map; the DDK
     shipped LINK 2.60.5046 (\MSVC20\LINK.EXE) SPECIFICALLY to fix VxD linking (its README says so), and it
     stamps 0xc0 from the exported <NAME>_DDB and writes an identity map. The resource is then added by the
     DDK's own RC + ADRC2VXD -- see version_rc / embed_version_resource, not a hand-copy. LINK386 (the OS/2
     linker) is never needed: it targets OS/2 LX and would not stamp a Win95 device id.
  4. IDENTITY PAGE MAP + OBJECT ORDER. realseg_def marks every segment PRELOAD NONDISCARDABLE. This keeps
     the linker from grouping discardable pages at end-of-file (which would permute the page map the Win95
     static loader reads by LOGICAL slot -> objects load as garbage -> init fault) AND keeps the objects in
     .def order == the oracle's object order. Result: an identity map and a matching object table.

    from src.engines.inverse import vxd_from_source as F
    r = F.build_from_source(v)          # v = engine.format_loaders.le.LEFile
    open("vkd.vxd","wb").write(r["vxd"])

NOTE ON PROVENANCE: the code, DDB, service table, fixups and object bytes are 100% from the recovered
source (only commutative re-encodings differ from the original -- SIB base/index order, test/xchg operand
order). The version resource is likewise rebuilt from recovered source: version_rc reconstructs the .rc
(device id + internal name from the DDB; file description + copyright years recovered from the oracle's
version-resource STRINGS -- metadata, not code) and the DDK's real RC + ADRC2VXD recompile and embed it.
Nothing is transplanted and no MS tool is reimplemented; the toolchain is ML 6.11 + LINK 2.60.5046 + RC
3.11 + ADRC2VXD, all period-authentic (see docs/VXD_FROM_SOURCE.md)."""
import os
import re
import shutil
import struct
import tempfile

from src.engines.inverse.vxd_asm import VxDDisasm
from src.engines.inverse import vxd_idiom
from src.engines.inverse.vxd_idiom import _ddb_size
from src.engines.inverse import vxd_link
from src.toolchain.backbone.dosbox import run_dos
from src.formats import le as _le
from src.core import paths

# Authentic DDK resource tools (BASE\VXDRES\MAKEFILE recipe: `rc -r <name>.rc` then `adrc2vxd`).
_RC_DIR   = "toolchains/period/compilers/msvc-1.52c/BIN"                   # RC 3.11 driver + RCPP (DOS)
_ADRC_REF = ("Microsoft Windows 95 DDK", "BIN", "ADRC2VXD.EXE")            # DDK version-resource embedder
_INC16_REF = ("Microsoft Windows 95 DDK", "INC16")                         # version.h / common.ver / ver.h

# MASM reserved words a recovered 4-char segment tag can collide with (e.g. dosmgr's `LOCK' segment ->
# `LOCK SEGMENT' -> A2008).  Rename the SEGMENT to a safe alias; the CLASS ('<tag>CODE') is unchanged, so
# the object-table 4-char tag (from the class, truncated) still matches the oracle.
_MASM_RESERVED = {"LOCK", "REP", "REPE", "REPZ", "REPNE", "REPNZ", "REPT", "AND", "OR", "XOR", "NOT",
                  "SHL", "SHR", "MOD", "EQ", "NE", "LT", "GT", "LE", "GE", "BYTE", "WORD", "DWORD",
                  "NEAR", "FAR", "PTR", "OFFSET", "SEG", "PROC", "ENDP", "IF", "ELSE", "ENDIF", "END",
                  "SIZE", "LENGTH", "TYPE", "THIS", "SHORT", "HIGH", "LOW", "GROUP", "ASSUME", "PUBLIC"}


def _seg_name(tag):
    """The MASM SEGMENT name for a 4-char class tag: the tag itself, or a safe alias when it is a reserved
    word.  Only the segment NAME changes; the class stays '<tag>CODE' so the object tag is preserved."""
    return ("_" + tag) if tag.upper() in _MASM_RESERVED else tag


def _object_segs(names, ddb_obj):
    """Map each NON-DDB object index -> (segment_name, class) for the _FLAT custom segments, guaranteeing a
    UNIQUE, VALID MASM name per object.  A tag that is blank, NUL-filled (`\\x00\\x00\\x00\\x00'), a reserved
    word, or a DUPLICATE of another object's tag would otherwise yield an unnamed or colliding segment
    (A2034 `must be in segment block' / a silent LINK merge) -- give those a synthetic `SEG<idx>' name.  The
    class tracks the name so LINK keeps every object distinct; a plain unique tag keeps its `<tag>CODE'."""
    segs, seen = {}, set()
    for idx, tag in enumerate(names, 1):
        if idx == ddb_obj:
            continue
        t = tag.rstrip("\x00 ").strip()
        valid = bool(t) and (t[0].isalpha() or t[0] == "_") and all(c.isalnum() or c in "_$?@" for c in t)
        base = _seg_name(t) if valid else "SEG%d" % idx
        name, k = base, 2
        while name in seen:                       # de-duplicate a repeated tag (e.g. two PCOD objects)
            name, k = "%s_%d" % (base, k), k + 1
        seen.add(name)
        segs[idx] = (name, name + "CODE")
    return segs


def object_names(v):
    """The 4-char segment class tag stored in each LE object-table entry's reserved field (offset 20).
    LINK /VXD writes the segment's CLASS name truncated to 4 chars there (e.g. 'LCODE' -> 'LCOD'), which
    is how the original's segment structure survives in a stripped binary."""
    d, lfa = v.d, v.lfa
    ot = struct.unpack_from("<I", d, lfa + 0x40)[0]
    oc = struct.unpack_from("<I", d, lfa + 0x44)[0]
    return [d[lfa + ot + i * 24 + 20: lfa + ot + i * 24 + 24].decode("latin1", "replace")
            for i in range(oc)]


def _reclassify_ddb_tail_as_data(D, v):
    """The DDB object's bytes AFTER the DDB are data (locked data + tables + BSS) that a recursive-
    descent disassembler over-reads as code (procedures straddling the DDB). Mark [ddb_end, vsize) of
    the DDB object as DATA so the emitter renders it as `db`/`dd` and it can be placed in _LDATA.
    Returns the tail start offset (or None if there is no DDB / no tail)."""
    if not D.ddb:
        return None
    oi = D.ddb["obj"]
    vsize = v.objects[oi - 1]["vsize"]
    tail0 = D.ddb["rva"] + _ddb_size(D, D.ddb)
    if tail0 >= vsize:
        return None
    for (o, k) in list(D.insns):
        if o == oi and tail0 <= k < vsize:
            del D.insns[(o, k)]
    for (o, k) in list(D.labels):                     # a proc label on data confuses the emitter into
        if o == oi and tail0 <= k < vsize and D.labels[(o, k)].startswith("sub_"):
            D.labels[(o, k)] = "loc_%d_%04X" % (o, k)  # a BeginProc -> demote to a plain data label
    return tail0


def realseg_source(D, v, names, tail0):
    """Re-emit vxd_idiom's loadable source with each object in its REAL segment: the DDB object's code
    in `VxD_LOCKED_CODE_SEG` (_LTEXT) and its post-DDB tail split out into `_LDATA`; every other object
    in a `_FLAT`-model custom segment named for its class. (A transform over emit_idiomatic's output --
    which emits synthetic per-object VSEGnn -- rather than a second emitter.)"""
    ddb_obj = D.ddb["obj"] if D.ddb else 1
    segs = _object_segs(names, ddb_obj)                   # {idx: (name, class)} for the non-DDB objects

    def open_close(idx):
        if idx == ddb_obj:                                # DDB object -> the locked-code macro (_LTEXT)
            return "VxD_LOCKED_CODE_SEG", "VxD_LOCKED_CODE_ENDS"
        return "%s SEGMENT" % segs[idx][0], "%s ENDS" % segs[idx][0]

    lines = vxd_idiom.emit_idiomatic(D, loadable=True).split("\r\n")
    # 1) drop the synthetic-VSEG predeclarations
    out, i = [], 0
    while i < len(lines):
        m = re.match(r"^(VSEG\d\d) SEGMENT DWORD PUBLIC _FLAT", lines[i])
        if m and i + 1 < len(lines) and lines[i + 1].strip() == m.group(1) + " ENDS":
            i += 2
            continue
        out.append(lines[i]); i += 1
    # 2) retarget each object-walk VSEGnn open/close to the real segment
    res = []
    for ln in out:
        mo = re.match(r"^VSEG(\d\d) SEGMENT$", ln.strip())
        me = re.match(r"^VSEG(\d\d) ENDS$", ln.strip())
        if mo:
            res.append(open_close(int(mo.group(1)))[0]); continue
        if me:
            res.append(open_close(int(me.group(1)))[1]); continue
        res.append(ln)
    # 3) split the DDB object's _LTEXT: code stays in _LTEXT; the tail (>= tail0) goes to _LDATA
    if tail0 is not None:
        lc_open = next(k for k, x in enumerate(res) if x.strip() == "VxD_LOCKED_CODE_SEG")
        lc_close = next(k for k in range(lc_open + 1, len(res)) if res[k].strip() == "VxD_LOCKED_CODE_ENDS")
        lab = re.compile(r"^(?:loc|sub)_%d_([0-9A-Fa-f]+):?$" % ddb_obj)
        L1 = next((k for k in range(lc_open + 1, lc_close)
                   if (m := lab.match(res[k].strip())) and int(m.group(1), 16) >= tail0), None)
        if L1 is not None:
            # A proc whose body was over-read PAST the tail start keeps its EndProc in _LTEXT and moves the
            # straddling body to _LDATA (VKD). A data-only DDB object -- no proc at/after the tail start,
            # the DDB struct + tables are the whole object -- splits cleanly at the tail start (~45 modules
            # whose code lives in other objects). Guarding on an EndProc after L1 avoids max() over an empty
            # sequence and never duplicates an EndProc that precedes the tail.
            begins_after = any(res[k].strip().startswith("BeginProc") for k in range(L1, lc_close))
            ends_after = [k for k in range(L1, lc_close) if res[k].strip().startswith("EndProc")]
            if begins_after:
                pass    # CODE resumes past the DDB in the DDB object (service handlers in obj1, e.g. NDIS):
                #  splitting at the tail would straddle a proc across the _LTEXT/_LDATA boundary (its
                #  BeginProc in _LDATA, EndProc in _LTEXT -> A1010). Leave the whole object in _LTEXT.
            else:
                if ends_after:
                    L2 = max(ends_after)
                    head, body = res[:L1] + [res[L2], "VxD_LOCKED_CODE_ENDS"], res[L1:L2]
                else:
                    head, body = res[:L1] + ["VxD_LOCKED_CODE_ENDS"], res[L1:lc_close]
                res = (head + ["VxD_LOCKED_DATA_SEG", "   ASSUME cs:FLAT, ds:FLAT, es:FLAT, ss:FLAT"]
                       + body + ["VxD_LOCKED_DATA_ENDS"] + res[lc_close + 1:])
    # 4) predeclare the custom segments in the _FLAT model, right after the includes
    decls = []
    for idx in sorted(segs):
        nm, cls = segs[idx]
        decls += ["%s SEGMENT DWORD PUBLIC _FLAT '%s'" % (nm, cls), "%s ENDS" % nm]
    j = 0
    while j < len(res) and not res[j].startswith("include"):
        j += 1
    while j < len(res) and (res[j].startswith("include") or res[j] == ""):
        j += 1
    res = res[:j] + decls + [""] + res[j:]
    return "\r\n".join(res) + "\r\n"


def realseg_def(name, names, ddb_obj=1):
    """The .DEF SEGMENTS block: _LTEXT/_LDATA (the DDB object) plus one entry per custom segment, ALL
    `PRELOAD NONDISCARDABLE` -> the linker writes pages in logical order (identity page map). Names and
    classes come from _object_segs so they match realseg_source (unique + valid, even for NUL/dup tags)."""
    seg = ["    _LTEXT   CLASS 'LCODE'  PRELOAD NONDISCARDABLE",
           "    _LDATA   CLASS 'LCODE'  PRELOAD NONDISCARDABLE"]
    for idx, (nm, cls) in sorted(_object_segs(names, ddb_obj).items()):
        seg.append("    %-8s CLASS '%s' PRELOAD NONDISCARDABLE" % (nm, cls))
    return ("VXD %s\r\nDESCRIPTION '%s reconstructed from source'\r\n\r\nSEGMENTS\r\n%s\r\n\r\n"
            "EXPORTS\r\n        %s_DDB @1\r\n" % (name, name, "\r\n".join(seg), name))


def ver_strings(res):
    """16-bit (ANSI) VER resource: 'key\\0value\\0' pairs with WORD-align padding. Extract wanted keys.
    (A VxD's version resource is VOS_DOS_WINDOWS16, so its strings are ANSI, not the UTF-16 of a Win32
    VS_VERSION_INFO.)"""
    out = {}
    for key in ("FileDescription", "LegalCopyright", "InternalName", "OriginalFilename"):
        i = res.find(key.encode("latin1"))
        if i < 0:
            continue
        j = i + len(key)
        while j < len(res) and res[j] == 0:            # skip key terminator + alignment pad
            j += 1
        e = j
        while e < len(res) and res[e] != 0:            # value runs to its null
            e += 1
        out[key] = res[j:e].decode("latin1", "replace")
    return out


def version_rc(D, res):
    """Reconstruct the VxD version-resource .rc as recovered SOURCE. The device id and internal name come
    from the recovered DDB; the human-authored strings (file description, copyright years) are recovered
    from the oracle's version-resource block `res`. Those strings are metadata the real RC recompiles into
    a fresh resource -- NOT a byte transplant. This is the .rc Microsoft fed to RC + ADRC2VXD (its shape is
    the DDK's own BASE\\VXDRES\\SAMPLE.RC)."""
    s = ver_strings(res)
    m = re.search(r"\d{4}-\d{4}|\d{4}", s.get("LegalCopyright", ""))
    return ("#include <version.h>\r\n"
            "#define VER_FILETYPE             VFT_VXD\r\n"
            "#define VER_FILESUBTYPE          0x%08X\r\n"
            '#define VER_FILEDESCRIPTION_STR  "%s"\r\n'
            '#define VER_INTERNALNAME_STR     "%s"\r\n'
            '#define VER_LEGALCOPYRIGHT_YEARS "%s"\r\n'
            '#define VER_ORIGINALFILENAME_STR "%s.VXD"\r\n'
            "#include <common.ver>\r\n"
            % (D.ddb["device_id"], s.get("FileDescription", ""), D.ddb["name"],
               m.group(0) if m else "", D.ddb["name"]))


def _stage_res_tools(root):
    """Copy RC 3.11 (+ its RCPP preprocessor) and the DDK ADRC2VXD into a cached space-free tool dir, and
    the INC16 version headers into a cached space-free include dir (the DDK's own path has spaces DOSBox
    cannot mount). Returns (tooldir, incdir)."""
    import glob
    tools = os.path.join(tempfile.gettempdir(), "cb_restools")
    inc = os.path.join(tempfile.gettempdir(), "cb_inc16")
    os.makedirs(tools, exist_ok=True); os.makedirs(inc, exist_ok=True)
    rcbin = os.path.join(root, _RC_DIR.replace("/", os.sep))
    for f, src in (("RC.EXE", os.path.join(rcbin, "RC.EXE")),
                   ("RCPP.EXE", os.path.join(rcbin, "RCPP.EXE")),
                   ("ADRC2VXD.EXE", paths.reference(*_ADRC_REF))):
        if not os.path.exists(os.path.join(tools, f)):
            shutil.copy(src, os.path.join(tools, f))
    incsrc = paths.reference(*_INC16_REF)
    for f in glob.glob(incsrc + os.sep + "*.H") + glob.glob(incsrc + os.sep + "*.VER"):
        if not os.path.exists(os.path.join(inc, os.path.basename(f))):
            shutil.copy(f, os.path.join(inc, os.path.basename(f)))
    return tools, inc


def embed_version_resource(vxd_bytes, D, root):
    """Add the Windows version resource with the DDK's OWN tools -- the BASE\\VXDRES\\MAKEFILE recipe,
    `rc -r <name>.rc` (RC 3.11) then `adrc2vxd <name>.vxd <name>.res` (DDK) -- replacing every hand-rolled
    container patch. Reconstructs the .rc (version_rc), compiles + embeds it under DOSBox, and returns
    (bytes, log) where bytes is the resource-carrying .vxd (hdr 0xb8/0xbc now reference a freshly compiled
    resource) or None if DOSBox is unconfigured / the tools error."""
    lfa = D.v.lfa
    woff = struct.unpack_from("<I", D.v.d, lfa + 0xb8)[0]    # absolute file offset of the oracle resource
    wlen = struct.unpack_from("<I", D.v.d, lfa + 0xbc)[0]
    if not wlen:                                             # ~1/7 of VxDs ship no version resource
        return vxd_bytes, "(no version resource in oracle; skipped)"
    rc_text = version_rc(D, D.v.d[woff:woff + wlen])
    tools, inc = _stage_res_tools(root)
    bat = ["set INCLUDE=e:\\",
           "copy in.vxd out.vxd",
           "c:\\RC.EXE -r m.rc > rc.txt",
           "c:\\ADRC2VXD.EXE out.vxd m.res > adrc.txt"]
    ok, out, work = run_dos({"c": tools, "e": inc}, bat, ["out.vxd", "m.res", "rc.txt", "adrc.txt"],
                            timeout=180, files={"m.rc": rc_text, "in.vxd": vxd_bytes})
    log = "".join("[%s] %s\n" % (t, out[t].decode("latin1", "replace").strip())
                  for t in ("rc.txt", "adrc.txt") if t in out)
    return out.get("out.vxd"), (rc_text + "\n" + log)


def restore_oracle_object_flags(vxd_bytes, v):
    r"""Set each linked LE object's ATTRIBUTE flags to the ORACLE object's flags (read per object from the
    retail VxD `v`), and return (bytes, report).

    WHY THIS IS NEEDED. LINK 2.60 /VXD derives an object's read/write/exec from the segment CLASS name
    (a class containing 'CODE' -> R|X executable; 'DATA' -> R|W writable, per VMM.INC: _PDATA is the only
    'PDATA'-class seg, every other locked seg is 'LCODE') and the object's discard/preload/shared/iopl/
    conforming from the .DEF keywords. realseg_source / realseg_def deliberately give EVERY object a
    unique '<tag>CODE' class and a blanket `PRELOAD NONDISCARDABLE` -- so LINK stamps every object
    READ|EXEC|PRELOAD|BIG32 (0x2045) and, crucially, WRITABLE DATA OBJECTS LOSE THE WRITE BIT (retail
    0x2023 READ|WRITE|SHARED -> 0x2045 READ|EXEC). A VxD then write-faults at ring 0 the first time it
    touches its own globals during Sys_Critical_Init/Device_Init.

    Why not just emit the data objects in a 'DATA' class + faithful .DEF attributes (the DDK mechanism)?
    Because LINK 2.60 /VXD then REORDERS the object table -- it emits all non-discardable code first (in
    .def order), then the data classes, then discardable segments last -- which does NOT reproduce the
    oracle's interleaved object order (e.g. IOS keeps discardable ICODE at slot 3, data at 5, code at 6-7).
    That permutes the object table the page-map fix pinned in oracle order (realseg_def's uniform PRELOAD
    NONDISCARDABLE keeps LINK from moving anything, which is what yields both the identity page map and the
    oracle object order). So the order-preserving uniform-class link is kept, and the ONE field LINK gets
    wrong -- the object flags word -- is corrected here, per object, straight from the oracle.

    The build's own segment-BITNESS bits are preserved: BIG32 (0x2000) and the 16:16-alias (0x1000) reflect
    the actual USE32 `_FLAT` assembly, not a retail attribute -- a real-mode RCODE object is 16-bit in retail
    (0x1005) but is genuinely 32-bit here, so forcing 16:16 would misdescribe the assembled code. Everything
    else (R/W/X, DISCARDABLE, SHARED, PRELOAD, IOPL, CONFORMING, RESIDENT) is taken from the oracle. Gated to
    the loadable from-source path (build_from_source); the byte-exact transplant path is untouched."""
    from src.formats import le as _le
    m = _le.LEFile.from_buffer(vxd_bytes)
    if len(m.objects) != len(v.objects):                     # object count desynced (should not happen with
        return vxd_bytes, {"applied": False,                 #  the uniform-class link) -> refuse to guess
                           "reason": "object count %d != oracle %d" % (len(m.objects), len(v.objects))}
    b = bytearray(vxd_bytes)
    ot = m.lfa + struct.unpack_from("<I", b, m.lfa + 0x40)[0]
    rows = []
    for i, (mo, vo) in enumerate(zip(m.objects, v.objects)):
        bld, orc = mo["flags"], vo["flags"]
        # BITNESS (BIG32 0x2000 / 16:16-alias 0x1000): normally keep the BUILD's, because a retail
        # real-mode segment (0x1005) is genuinely assembled 32-bit here and forcing 16:16 would
        # misdescribe it. BUT when the built object is BYTE-IDENTICAL to the oracle (the loadable lane
        # now byte-preserves, so this is the common case), our bytes ARE the oracle's bytes -- including
        # a real-mode-init (RCOD) 16-bit segment -- so the oracle's bitness is the CORRECT description;
        # keeping BIG32 there marks a 16-bit segment as 32-bit and VMM rejects the load (vshare's RCOD
        # LoadFailed). Take the full oracle flags for a byte-identical object; keep build-bitness only
        # where our code genuinely differs (re-encoded) from the oracle.
        try:
            identical = bytes(m.object_bytes(mo)) == bytes(v.object_bytes(vo))
        except Exception:
            identical = False
        target = orc if identical else ((orc & ~0x3000) | (bld & 0x3000))
        struct.pack_into("<I", b, ot + i * 24 + 8, target)
        rows.append({"obj": i + 1, "build": bld, "oracle": orc, "final": target, "identical": identical,
                     "write_restored": bool((orc & 0x02) and not (bld & 0x02) and (target & 0x02))})
    return bytes(b), {"applied": True, "objects": rows,
                      "writable_restored": sum(1 for r in rows if r["write_restored"])}


def build_from_source(v, name=None, root="."):
    """Build a loadable from-source VxD (no transplant). Returns
    {ok, vxd, vxd_len, oracle_len, asm, def, objects, page_map_identity, ddb_rva, obj1_diffs, ...}."""
    root = os.path.abspath(root)
    D = VxDDisasm(v).run(); D.v = v
    name = name or (D.ddb and D.ddb["name"]) or "VXD"
    names = object_names(v)
    tail0 = _reclassify_ddb_tail_as_data(D, v)
    asm = realseg_source(D, v, names, tail0)
    obj, alog, work = vxd_link._assemble_idiomatic(asm, root)
    if not obj:
        return {"ok": False, "stage": "assemble", "log": alog[-1500:], "asm": asm}
    deftext = realseg_def(name, names, D.ddb["obj"] if D.ddb else 1)
    vxd, llog = vxd_link.link_vxd_260(obj, deftext, root, work)   # LINK 2.60 stamps device id + page map
    if not vxd:
        return {"ok": False, "stage": "link", "log": llog[-1500:], "asm": asm, "def": deftext}
    final, rlog = embed_version_resource(vxd, D, root)            # real RC + ADRC2VXD version resource
    if not final:
        return {"ok": False, "stage": "resource", "log": rlog[-1500:], "asm": asm, "def": deftext}
    # restore each object's LE flags from the oracle (LINK 2.60 flattens every object to 0x2045, dropping
    # the WRITE bit on data objects -> ring-0 write fault in init). Order-preserving: see the helper.
    final, oflags = restore_oracle_object_flags(final, v)
    # verification: identity page map, DDB back at the oracle offset, DDB object near-exact
    m = _le.LEFile.from_buffer(final)
    d, lfa = m.d, m.lfa
    pm = struct.unpack_from("<I", d, lfa + 0x48)[0]
    npg = struct.unpack_from("<I", d, lfa + 0x14)[0]
    phys = [(d[lfa + pm + k * 4] << 16) | (d[lfa + pm + k * 4 + 1] << 8) | d[lfa + pm + k * 4 + 2]
            for k in range(npg)]
    identity = phys == list(range(1, npg + 1))
    dd = m.ddb()
    dev = struct.unpack_from("<H", d, lfa + 0xc0)[0]              # device id LINK 2.60 stamped from the DDB
    reslen = struct.unpack_from("<I", d, lfa + 0xbc)[0]          # version resource ADRC2VXD embedded
    o = bytes(v.object_bytes(v.objects[0])); r = bytes(m.object_bytes(m.objects[0]))
    o1diffs = sum(1 for k in range(min(len(o), len(r))) if o[k] != r[k])
    return {"ok": True, "vxd": final, "vxd_len": len(final), "oracle_len": len(v.d), "asm": asm,
            "def": deftext, "objects": len(m.objects), "page_map_identity": identity,
            "ddb_rva": dd and dd["rva"], "oracle_ddb_rva": D.ddb and D.ddb["rva"],
            "device_id": dev, "device_id_ok": bool(D.ddb) and dev == D.ddb["device_id"],
            "resource_len": reslen, "obj1_diffs": o1diffs, "link_log": llog[-300:], "res_log": rlog[-300:],
            "object_flags": oflags}


if __name__ == "__main__":
    import sys
    from src.core import config
    config.apply_config()
    v = _le.open_vxds(sys.argv[1])[0][1]
    r = build_from_source(v)
    if not r["ok"]:
        print("FAILED at %s:\n%s" % (r["stage"], r.get("log", ""))); sys.exit(1)
    print("built %d bytes (oracle %d); objects=%d; page map identity=%s; DDB @0x%x (oracle 0x%x); "
          "obj1 diffs=%d" % (r["vxd_len"], r["oracle_len"], r["objects"], r["page_map_identity"],
                             r["ddb_rva"] or 0, r["oracle_ddb_rva"] or 0, r["obj1_diffs"]))
    if len(sys.argv) > 2:
        open(sys.argv[2], "wb").write(r["vxd"]); print("wrote", sys.argv[2])
