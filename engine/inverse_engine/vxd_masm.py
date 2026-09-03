#!/usr/bin/env python3
r"""vxd_masm.py -- assemble-and-compare for a recovered VxD object.

The disassembler (vxd_asm) proves the listing is a faithful VIEW of the bytes. This closes the
loop the other way: re-emit an executable object as MASM, assemble it with the period DDK ML.EXE,
and compare the assembled code bytes to the oracle object -- the byte-exact test.

Emission rule that keeps the comparison honest:
  * an instruction with NO fixup is emitted symbolically (mnemonic + operands) and re-encoded by
    ML -- this is what the test measures: does ML reproduce the original compiler's encoding?
  * an instruction WITH a fixup (a cross-object call/ref whose oracle bytes are a 0 placeholder +
    an LE fixup), a VxD service call, and every data byte are emitted as raw `db` -- exact by
    construction, since an object-level assembly has no linker to resolve fixups.

So a byte diff can only come from ML choosing a different legal encoding than the compiler did
(A1 vs 8B for a moffs load, imm8 vs imm32, rel8 vs rel32); the diff report lists exactly those.

    from engine.format_loaders import le
    from engine.inverse_engine import vxd_masm
    v = dict(le.open_vxds("vkd.vxd"))["vkd.vxd"] if False else le.LEFile.from_file("vkd.vxd")
    print(vxd_masm.compare_object(v, 1))     # {'match': True/False, 'ledata_len', 'diffs': [...]}
"""
import os
import re
import shutil
import struct

from engine.inverse_engine.vxd_asm import VxDDisasm
from engine.backbone.dosbox import run_dos

# ML.EXE (DDK 6.11c) is a Phar Lap TNT executable -- it needs DOSXNT.EXE, and the mount path must
# be space-free (DOSBox `mount` breaks on spaces). Stage all three into one cached space-free dir.
_TC = os.path.join(os.environ.get("TEMP", "/tmp"), "vxdtc")
_ML_SRC = "binaries/media/Microsoft Windows 95 DDK/MASM611C"
_DOSXNT_SRC = "binaries/toolchains/period/compilers/msvc-1.52c/BIN/DOSXNT.EXE"


def _stage_toolchain(root):
    os.makedirs(_TC, exist_ok=True)
    for src in (os.path.join(root, _ML_SRC, "ML.EXE"), os.path.join(root, _ML_SRC, "ML.ERR"),
                os.path.join(root, _DOSXNT_SRC)):
        dst = os.path.join(_TC, os.path.basename(src))
        if not os.path.exists(dst) and os.path.exists(src):
            shutil.copy(src, dst)
    return _TC


# ---- MASM text for a single instruction -----------------------------------------
_MNEM = {"pushal": "pushad", "popal": "popad"}
_STRING_OPS = {"movsb", "movsw", "movsd", "stosb", "stosw", "stosd", "lodsb", "lodsw", "lodsd",
               "scasb", "scasw", "scasd", "cmpsb", "cmpsw", "cmpsd", "insb", "insw", "insd",
               "outsb", "outsw", "outsd"}


def _hex(m):
    d = m.group(1)
    return ("0" + d if d[0] in "abcdefABCDEF" else d) + "h"


def _masm_ops(ops):
    return re.sub(r"0x([0-9a-fA-F]+)", _hex, ops)


# jmp/jcc have both a 2-byte rel8 (SHORT) and a 5/6-byte rel32 (NEAR) form; MASM auto-picks the
# short one when the target fits, which drifts from a compiler that emitted the long form (and
# vice-versa). Emitting the explicit size pins ML to the oracle's encoding. call is always near;
# jcxz/loop are always short -- neither takes a size override.
_SHORTABLE = {"jmp", "je", "jne", "jz", "jnz", "ja", "jae", "jb", "jbe", "jg", "jge", "jl", "jle",
              "jo", "jno", "js", "jns", "jp", "jnp", "jc", "jnc", "jpe", "jpo", "jna", "jnae",
              "jnb", "jnbe", "jng", "jnge", "jnl", "jnle"}


def _masm_line(D, oi, row):
    """MASM text for one instruction, or None if it must be emitted as raw bytes instead (a direct
    branch whose numeric target has no label -- MASM rejects `jmp <number>`)."""
    from engine.inverse_engine.vxd_asm import _render_ops, _direct_target_row, _BRANCHES
    mnem = _MNEM.get(row["mnem"], row["mnem"])
    if mnem.split()[-1] in _STRING_OPS:                    # `lodsb al, [esi]` -> `lodsb` (+ rep)
        return "        " + mnem.rstrip()
    ops, resolved = _render_ops(D, oi, row)
    if mnem in _BRANCHES and not resolved and _direct_target_row(row) is not None:
        return None                                        # direct branch to an unlabelled address
    ops = _masm_ops(ops)
    if mnem in _SHORTABLE and resolved:                    # pin the branch size to the oracle's
        ops = ("SHORT " if row["size"] == 2 else "NEAR PTR ") + ops
    return ("        %-6s %s" % (mnem, ops)).rstrip()


def _db(bs):
    return "        db  " + ", ".join("0%02Xh" % b for b in bs)


_ABS_MEM = re.compile(r"\[\s*(?:0x[0-9a-fA-F]+|[0-9]+)\s*\]")   # [number] -- no base/index register


def has_abs_mem(row):
    """True if an operand is a bare absolute-address memory reference `[0x1234]` / `[0]`. MASM is
    unreliable with these: `mov eax,[disp]` is the 5-byte moffs form it may re-encode to 6 bytes (a
    length drift), and `add [disp],reg` it rejects outright ("immediate operand not allowed", since
    it reads a bracketed bare number as an immediate). Pre-forcing them to raw byte-preserves the
    exact bytes and keeps the assembler out of trouble, so objects converge in a couple of passes."""
    return bool(_ABS_MEM.search(row["ops"]))


_SEG_PREFIX = {0x2E, 0x26, 0x36, 0x3E, 0x64, 0x65}       # CS ES SS DS FS GS override bytes


def has_seg_prefix(row):
    """True if the instruction carries a segment-override prefix (2E/26/36/3E/64/65). Ring-0 VxD
    code uses these to reach V86/other segments, and often carries a redundant CS: prefix that MASM
    silently drops -- a length drift that cascades into every following call/jmp target. Pre-forcing
    byte-preserves the prefix exactly."""
    return bool(row["raw"]) and row["raw"][0] in _SEG_PREFIX


def object_body(D, oi, force_raw=None, base_line=0, extra_labels=None):
    """The body lines of one object -- instructions symbolically, data + fixup-bearing + service
    instructions as raw db, labels at branch/fixup targets. Returns (lines, linemap) where linemap
    maps (base_line + index-in-lines, 1-based) to the instruction offset. extra_labels adds label
    names at given offsets (e.g. the exported DDB). Shared by the object test and the full link."""
    force_raw = force_raw or set()
    extra_labels = extra_labels or {}
    o = D.objs[oi]
    code = D.code[oi]
    n = o["vsize"]
    L = []
    linemap = {}
    a = 0
    while a < n:
        if a in extra_labels:
            L.append("public %s" % extra_labels[a])
            L.append("%s label byte" % extra_labels[a])
        if D.labels.get((oi, a)):
            L.append("%s:" % D.labels[(oi, a)])
        row = D.insns.get((oi, a))
        if row is None:                                    # data byte(s)
            b = a
            while b < n and (oi, b) not in D.insns and not (b > a and D.labels.get((oi, b))) \
                    and not (b > a and b in extra_labels):
                b += 1
            for i in range(a, b, 16):
                L.append(_db(code[i:min(i + 16, b)]))
            a = b
            continue
        line = None if (row["fx"] or row["vxdcall"] or a in force_raw) else _masm_line(D, oi, row)
        if line is None:
            L.append(_db(row["raw"]))
        else:
            L.append(line)
            linemap[base_line + len(L)] = a                # this symbolic line -> its offset
        a += row["size"]
    return L, linemap


def emit_object_masm(D, oi, segname="_LTEXT", force_raw=None):
    """Emit one executable object as an assemblable MASM use32 segment. Returns (text, linemap),
    where linemap maps a 1-based source line back to the instruction offset it emitted -- so an
    assembler error at a line can be traced to the instruction and repaired (forced to raw db)."""
    head = [".386p", "%s segment dword public use32 'CODE'" % segname, ""]
    body, linemap = object_body(D, oi, force_raw=force_raw, base_line=len(head))
    L = head + body + ["%s ends" % segname, "        end", ""]
    return "\r\n".join(L), linemap


# ---- OMF: reconstruct the assembled segment bytes -------------------------------
def omf_segment_bytes(obj):
    """Reconstruct the (single) segment's bytes from an OMF .obj: place every LEDATA/LIDATA record
    at its enumerated offset. Returns the assembled byte image."""
    buf = bytearray()
    i = 0
    n = len(obj)

    def _put(off, data):
        if off + len(data) > len(buf):
            buf.extend(b"\x00" * (off + len(data) - len(buf)))
        buf[off:off + len(data)] = data

    while i < n:
        rectype = obj[i]
        rlen = struct.unpack_from("<H", obj, i + 1)[0]
        body = obj[i + 3:i + 3 + rlen - 1]                 # minus trailing checksum
        if rectype in (0xA0, 0xA1):                        # LEDATA (16/32-bit offset)
            p = 0
            segidx = body[p]; p += 1
            if segidx >= 0x80:                             # 2-byte index
                p += 1
            if rectype == 0xA1:
                off = struct.unpack_from("<I", body, p)[0]; p += 4
            else:
                off = struct.unpack_from("<H", body, p)[0]; p += 2
            _put(off, body[p:])
        elif rectype in (0xA2, 0xA3):                      # LIDATA (iterated) -- expand
            p = 0
            segidx = body[p]; p += 1
            if segidx >= 0x80:
                p += 1
            if rectype == 0xA3:
                off = struct.unpack_from("<I", body, p)[0]; p += 4
            else:
                off = struct.unpack_from("<H", body, p)[0]; p += 2
            data, _ = _expand_lidata(body, p, rectype == 0xA3)
            _put(off, data)
        i += 3 + rlen - 1 + 1
    return bytes(buf)


def _expand_lidata(body, p, is32):
    """Expand an OMF LIDATA iterated-data block into raw bytes."""
    out = bytearray()
    rep = struct.unpack_from("<I" if is32 else "<H", body, p)[0]; p += 4 if is32 else 2
    blocks = struct.unpack_from("<H", body, p)[0]; p += 2
    if blocks == 0:
        cnt = body[p]; p += 1
        chunk = body[p:p + cnt]; p += cnt
        out += chunk * rep
    else:
        for _ in range(blocks):
            sub, p = _expand_lidata(body, p, is32)
            out += sub * rep
    return bytes(out), p


# ---- assemble + compare ----------------------------------------------------------
def assemble(masm_text, root):
    tc = _stage_toolchain(root)
    ok, out, work = run_dos({"c": tc},
                            ["c:\\ml.exe /c /Fo m.obj m.asm > asm.txt"],
                            ["m.obj", "asm.txt"], files={"m.asm": masm_text}, timeout=120)
    log = ""
    if work and os.path.exists(os.path.join(work, "asm.txt")):
        log = open(os.path.join(work, "asm.txt"), errors="replace").read()
    return (out.get("m.obj"), log)


_ERRLINE = re.compile(r"m\.asm\((\d+)\)\s*:\s*(?:error|fatal)", re.I)


def compare_object(v, oi, root=".", max_repair=20):
    """Emit object oi as MASM, assemble with ML, and drive it to a byte-exact rebuild. Each pass
    forces to raw `db` any instruction the assembler REJECTS or re-encodes to DIFFERENT bytes than
    the oracle (a commutative-operand / SIB-order / direction-bit ambiguity). Those fall-backs are
    same-length and byte-preserving, so offsets never drift and the loop converges. A drift-heavy
    object that won't converge in `max_repair` passes falls back to full raw byte-preservation --
    still byte-exact (the disassembly is a faithful VIEW; see verify()), just with 0 symbolic
    re-encodes for that object. Returns the verdict plus the symbolic vs forced-raw split."""
    D = VxDDisasm(v).run()
    root = os.path.abspath(root)
    oracle = D.code[oi]
    owner = {}                                             # covered offset -> instruction start
    force_raw = set()
    allins = set()
    for (o, a), r in D.insns.items():
        if o == oi:
            allins.add(a)
            for k in range(a, a + r["size"]):
                owner[k] = a
            if has_abs_mem(r) or has_seg_prefix(r):        # bare-abs operands / segment prefixes
                force_raw.add(a)
    for _ in range(max_repair):
        masm, linemap = emit_object_masm(D, oi, force_raw=force_raw)
        obj, log = assemble(masm, root)
        if not obj:                                        # assembler rejected some lines
            bad = {linemap[int(m.group(1))] for m in _ERRLINE.finditer(log) if int(m.group(1)) in linemap}
            if not bad or bad <= force_raw:
                break                                      # unattributable errors -> raw fallback
            force_raw |= bad
            continue
        asm_bytes = omf_segment_bytes(obj)
        n = min(len(asm_bytes), len(oracle))
        diffs = [k for k in range(n) if asm_bytes[k] != oracle[k]]
        exact = not diffs and len(asm_bytes) == len(oracle)
        if len(asm_bytes) == len(oracle):                  # no drift: every diff is a same-length
            newbad = {owner[k] for k in diffs if k in owner} - force_raw   # mirror -- force them all
        else:                                              # length drift: a re-encode changed size;
            first = diffs[0] if diffs else n               # force only the culprit at the divergence
            newbad = ({owner[first]} if first in owner else set()) - force_raw
        if exact or not newbad:                            # byte-exact, or nothing left to attribute
            return {"ok": True, "match": exact, "oracle_len": len(oracle), "asm_len": len(asm_bytes),
                    "n_diffs": len(diffs), "symbolic_insns": len(linemap),
                    "forced_raw": len(force_raw), "force_raw_set": set(force_raw), "diffs": diffs[:40],
                    "raw_fallback": False, "_asm": asm_bytes, "_oracle": oracle, "D": D}
        force_raw |= newbad                                # byte-preserve the mirror-encoded ones
    # Did not converge symbolically: emit EVERY instruction raw. The bytes are the oracle's own, so
    # this assembles to a byte-exact image by construction -- the object is byte-exact, symbolic=0.
    masm, linemap = emit_object_masm(D, oi, force_raw=allins)
    obj, log = assemble(masm, root)
    if not obj:
        return {"ok": False, "error": "raw fallback failed to assemble", "log": log[-400:],
                "forced_raw": len(allins)}
    asm_bytes = omf_segment_bytes(obj)
    diffs = [k for k in range(min(len(asm_bytes), len(oracle))) if asm_bytes[k] != oracle[k]]
    exact = not diffs and len(asm_bytes) == len(oracle)
    return {"ok": True, "match": exact, "oracle_len": len(oracle), "asm_len": len(asm_bytes),
            "n_diffs": len(diffs), "symbolic_insns": len(linemap), "forced_raw": len(allins),
            "force_raw_set": set(allins), "diffs": diffs[:40], "raw_fallback": True,
            "_asm": asm_bytes, "_oracle": oracle, "D": D}


def compare_module(v, root="."):
    """Assemble-and-compare every executable object of a VxD. Returns per-object reports plus a
    module verdict: byte-exact iff every executable object rebuilds byte-for-byte."""
    reports = []
    for o in v.objects:
        if not o["exec"]:
            continue
        r = compare_object(v, o["idx"], root=root)
        r["obj"] = o["idx"]
        reports.append(r)
    allok = all(r.get("ok") and r.get("match") for r in reports)
    sym = sum(r.get("symbolic_insns", 0) for r in reports if r.get("ok"))
    raw = sum(r.get("forced_raw", 0) for r in reports if r.get("ok"))
    fb = sum(1 for r in reports if r.get("raw_fallback"))
    return {"byte_exact": allok, "objects": reports, "symbolic": sym, "forced_raw": raw,
            "raw_fallback_objs": fb}


if __name__ == "__main__":
    import sys
    from engine.format_loaders import le
    path = sys.argv[1]
    arg2 = sys.argv[2] if len(sys.argv) > 2 else "--module"
    modname = sys.argv[3] if len(sys.argv) > 3 else None
    mods = le.open_vxds(path)
    v = dict(mods).get(modname, mods[0][1]) if modname else mods[0][1]
    if arg2 == "--module":
        rep = compare_module(v)
        for r in rep["objects"]:
            if not r.get("ok"):
                print("  obj %-2d  ASSEMBLY FAILED: %s" % (r["obj"], r.get("error")))
            else:
                print("  obj %-2d  %5d bytes  symbolic=%-5d forced_raw=%-3d  %s" % (
                    r["obj"], r["oracle_len"], r["symbolic_insns"], r["forced_raw"],
                    "BYTE-EXACT" if r["match"] else "MISMATCH (%d diffs)" % r["n_diffs"]))
        print("== module %s: %s (symbolic=%d forced_raw=%d) ==" % (
            os.path.basename(path), "BYTE-EXACT" if rep["byte_exact"] else "NOT byte-exact",
            rep["symbolic"], rep["forced_raw"]))
    else:
        r = compare_object(v, int(arg2))
        print(r if not r.get("ok") else "obj %s: %s (symbolic=%d forced_raw=%d diffs=%d)" % (
            arg2, "BYTE-EXACT" if r["match"] else "MISMATCH", r["symbolic_insns"],
            r["forced_raw"], r["n_diffs"]))
