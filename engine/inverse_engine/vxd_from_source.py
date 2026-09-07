#!/usr/bin/env python3
r"""vxd_from_source.py -- build a LOADABLE Win95 VxD from reconstructed SOURCE, with NO byte transplant.

This is the path that `build_loadable_idiomatic` could not take: it emits the recovered VxD as idiomatic
DDK-macro assembly with each object in its REAL named segment, assembles it with the period ML.EXE,
links it with LINK /VXD, and applies the two container fixups the Win95 loader requires that a modern
LINK omits. Unlike `build_loadable_idiomatic`, it does NOT overlay the oracle's object bytes -- the
container that comes out is the linker's own, corrected only where a *substitute* linker diverges from
Microsoft's LINK386. A VKD.VXD built this way boots real Windows 95 to the logon.

The four requirements, all pinned by real boots (see docs/VXD_FROM_SOURCE.md):

  1. SEGMENT MODEL. The DDB object's recovered code goes in `_LTEXT` (VxD_LOCKED_CODE_SEG) WITH the
     control dispatch; the service table + `Declare_Virtual_Device` land in `_LDATA` via the macros;
     and the DDB object's post-DDB TAIL is DATA -- emitted into `_LDATA` after the DDB, not left in
     `_LTEXT` where a disassembler's over-read would insert it ahead of the service table and shove
     the DDB off its true offset. This yields the on-object order [dispatch][code][service][DDB][data].
  2. FLAT MODEL. Every non-DDK segment is declared `<NAME> SEGMENT DWORD PUBLIC _FLAT '<NAME>CODE'`.
     `_FLAT` (=FLAT under MASM6) makes it part of the flat group; `USE32` does not, and every cross-
     segment near call then fails to assemble (A2107). This is what the DDK's MakeCodeSeg expands to.
  3. CONTAINER FIELDS. The VxD device id (LE header 0xc0) and the Windows version resource
     (0xb8/0xbc) are loader-REQUIRED and LINK /VXD leaves them blank; supply them post-link.
  4. IDENTITY PAGE MAP. Mark every segment PRELOAD NONDISCARDABLE so the linker does not group
     discardable pages at end-of-file (which permutes the page map). The Win95 static VxD loader reads
     object pages by LOGICAL slot and ignores a permuted map, so a permutation loads objects as garbage
     and faults during init. NONDISCARDABLE -> pages in logical order -> identity map, and as a bonus
     the object order matches too.

    from engine.inverse_engine import vxd_from_source as F
    r = F.build_from_source(v)          # v = engine.format_loaders.le.LEFile
    open("vkd.vxd","wb").write(r["vxd"])

NOTE ON PROVENANCE: the code, DDB, service table, fixups and object bytes are 100% from the recovered
source (only commutative re-encodings differ from the original -- SIB base/index order, test/xchg
operand order). The ONE non-source input is the Windows version resource in requirement 3, copied from
the oracle: it is a version-info RESOURCE (strings), not code, and LINK386 would have generated it from
a .rc the disassembly cannot recover. Generating it from the DDB metadata instead is the open TODO that
would make the pipeline entirely oracle-free.
"""
import os
import re
import struct
import subprocess

from engine.inverse_engine.vxd_asm import VxDDisasm
from engine.inverse_engine import vxd_idiom
from engine.inverse_engine.vxd_idiom import _ddb_size
from engine.inverse_engine import vxd_link
from engine.format_loaders import le as _le


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
    STD = {names[ddb_obj - 1]: "VxD_LOCKED_CODE"}          # DDB object -> the locked-code macro (_LTEXT)
    custom = [n for n in names if n not in STD]

    def open_close(idx):
        nm = names[idx - 1]
        if nm in STD:
            return STD[nm] + "_SEG", STD[nm] + "_ENDS"
        return "%s SEGMENT" % nm, "%s ENDS" % nm

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
    if tail0 is not None and names[ddb_obj - 1] in STD:
        lc_open = next(k for k, x in enumerate(res) if x.strip() == "VxD_LOCKED_CODE_SEG")
        lc_close = next(k for k in range(lc_open + 1, len(res)) if res[k].strip() == "VxD_LOCKED_CODE_ENDS")
        lab = re.compile(r"^(?:loc|sub)_%d_([0-9A-Fa-f]+):?$" % ddb_obj)
        L1 = next((k for k in range(lc_open + 1, lc_close)
                   if (m := lab.match(res[k].strip())) and int(m.group(1), 16) >= tail0), None)
        if L1 is not None:
            L2 = max(k for k in range(lc_open + 1, lc_close) if res[k].strip().startswith("EndProc"))
            res = (res[:L1] + [res[L2], "VxD_LOCKED_CODE_ENDS", "VxD_LOCKED_DATA_SEG",
                               "   ASSUME cs:FLAT, ds:FLAT, es:FLAT, ss:FLAT"]
                   + res[L1:L2] + ["VxD_LOCKED_DATA_ENDS"] + res[lc_close + 1:])
    # 4) predeclare the custom segments in the _FLAT model, right after the includes
    decls = []
    for nm in custom:
        decls += ["%s SEGMENT DWORD PUBLIC _FLAT '%sCODE'" % (nm, nm), "%s ENDS" % nm]
    j = 0
    while j < len(res) and not res[j].startswith("include"):
        j += 1
    while j < len(res) and (res[j].startswith("include") or res[j] == ""):
        j += 1
    res = res[:j] + decls + [""] + res[j:]
    return "\r\n".join(res) + "\r\n"


def realseg_def(name, names):
    """The .DEF SEGMENTS block: _LTEXT/_LDATA plus one entry per custom segment, in object order, ALL
    `PRELOAD NONDISCARDABLE` -> the linker writes pages in logical order (identity page map)."""
    seg = ["    _LTEXT   CLASS 'LCODE'  PRELOAD NONDISCARDABLE",
           "    _LDATA   CLASS 'LCODE'  PRELOAD NONDISCARDABLE"]
    for idx, nm in enumerate(names, 1):
        if idx == 1:                                  # the DDB object is the LCODE object (_LTEXT/_LDATA)
            continue
        seg.append("    %-8s CLASS '%sCODE' PRELOAD NONDISCARDABLE" % (nm, nm))
    return ("VXD %s\r\nDESCRIPTION '%s reconstructed from source'\r\n\r\nSEGMENTS\r\n%s\r\n\r\n"
            "EXPORTS\r\n        %s_DDB @1\r\n" % (name, name, "\r\n".join(seg), name))


def container_fixup(vxd_bytes, v, D):
    """Supply the two loader-required fields LINK /VXD leaves blank: the VxD device id (LE hdr 0xc0,
    from the DDB's device number -- source-derived) and the Windows version resource (hdr 0xb8/0xbc,
    copied from the oracle -- a resource, not code; see the module TODO). Returns patched bytes."""
    raw = bytearray(vxd_bytes)
    blfa = struct.unpack_from("<I", raw, 0x3c)[0]
    if D.ddb:
        struct.pack_into("<H", raw, blfa + 0xc0, D.ddb["device_id"] & 0xffff)   # device id (source)
    # Windows version resource: copy the oracle's block, append, point hdr 0xb8/0xbc at it.
    olfa = v.lfa
    woff = struct.unpack_from("<I", v.d, olfa + 0xb8)[0]
    wlen = struct.unpack_from("<I", v.d, olfa + 0xbc)[0]
    if woff and wlen:
        while len(raw) % 4:
            raw.append(0)
        newoff = len(raw) - blfa
        raw += v.d[olfa + woff: olfa + woff + wlen]
        struct.pack_into("<I", raw, blfa + 0xb8, newoff)
        struct.pack_into("<I", raw, blfa + 0xbc, wlen)
    return bytes(raw)


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
    deftext = realseg_def(name, names)
    vxd, llog = vxd_link._link_vxd(obj, deftext, root, work)
    if not vxd:
        return {"ok": False, "stage": "link", "log": llog[-1500:], "asm": asm, "def": deftext}
    final = container_fixup(vxd, v, D)
    # verification: identity page map, DDB back at the oracle offset, DDB object near-exact
    m = _le.LEFile.from_buffer(final)
    d, lfa = m.d, m.lfa
    pm = struct.unpack_from("<I", d, lfa + 0x48)[0]
    npg = struct.unpack_from("<I", d, lfa + 0x14)[0]
    phys = [(d[lfa + pm + k * 4] << 16) | (d[lfa + pm + k * 4 + 1] << 8) | d[lfa + pm + k * 4 + 2]
            for k in range(npg)]
    identity = phys == list(range(1, npg + 1))
    dd = m.ddb()
    o = bytes(v.object_bytes(v.objects[0])); r = bytes(m.object_bytes(m.objects[0]))
    o1diffs = sum(1 for k in range(min(len(o), len(r))) if o[k] != r[k])
    return {"ok": True, "vxd": final, "vxd_len": len(final), "oracle_len": len(v.d), "asm": asm,
            "def": deftext, "objects": len(m.objects), "page_map_identity": identity,
            "ddb_rva": dd and dd["rva"], "oracle_ddb_rva": D.ddb and D.ddb["rva"],
            "obj1_diffs": o1diffs, "link_log": llog[-300:]}


if __name__ == "__main__":
    import sys
    from engine.foundation import config
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
