#!/usr/bin/env python3
r"""vxd_tree.py -- emit a VxD as a proper multi-file MASM source tree.

Real Win95 VxD source was never one monolithic file: it was organised per segment (_LTEXT locked
code, _ITEXT init, _PTEXT pageable, message tables, data) and split further by function. This emits
that structure -- one directory per driver, one file per segment, large segments split at function
boundaries into numbered files -- reusing the disassembler's DDK-recovered names (services, control
handlers, and the 12 core VxDs' internal names). Each file carries a header noting the segment
class and, honestly, that the ORIGINAL file/function names are not present in a stripped LE binary,
so files are named by segment/role rather than fabricated Microsoft filenames.

    from src.formats import le
    from src.engines.inverse import vxd_tree
    vxd_tree.emit_tree(le.LEFile.from_file("vkd.vxd"), "VKD", "recovered/vxd_src")
"""
import os
import re
import subprocess
import tempfile

from src.engines.inverse.vxd_asm import VxDDisasm
from src.engines.inverse import vxd_masm

_ERRLINE = re.compile(r":(\d+):\s*(?:error|fatal)", re.I)          # nasm / gas style
_ERRLINE_WASM = re.compile(r"\((\d+)\):\s*Error", re.I)            # wasm: file(NN): Error!


def _nasm_probe(nasm):
    """(rc, err_text, errline_re) probe using `nasm --masm -f bin`."""
    def go(path):
        r = subprocess.run([nasm, "--masm", "-f", "bin", path, "-o", os.devnull],
                           capture_output=True, text=True)
        return r.returncode, r.stderr, _ERRLINE
    return go


def _wasm_probe(wasm):
    """(rc, err_text, errline_re) probe using OpenWatcom `wasm -zcm=masm`."""
    def go(path):
        o = tempfile.NamedTemporaryFile(suffix=".obj", delete=False); o.close()
        try:
            r = subprocess.run([wasm, "-zcm=masm", "-q", "-fo=" + o.name, path],
                               capture_output=True, text=True)
        finally:
            try: os.unlink(o.name)
            except OSError: pass
        return r.returncode, r.stdout + r.stderr, _ERRLINE_WASM
    return go


def _converge_buildable(D, oi, lo, hi, extra_labels, head, tail, probe, maxit=20):
    """Emit body, assemble via `probe` (a nasm/wasm assembler callable), byte-preserve
    (force_raw -> db) every instruction the assembler rejects, and repeat until it
    assembles clean. db == the exact bytes, so this only trades an un-assemblable
    disassembly (data walked as code, a mnemonic/operand the assembler won't spell) for
    its raw bytes -- byte-exact preserved and now buildable. Returns the body lines."""
    force = set()
    body = None
    for _ in range(maxit):
        body, linemap = vxd_masm.object_body(D, oi, force_raw=force, extra_labels=extra_labels,
                                             base_line=len(head), crange=(lo, hi))
        with tempfile.NamedTemporaryFile("w", suffix=".asm", delete=False, newline="") as tf:
            tf.write("\r\n".join(head + body + tail))
            path = tf.name
        try:
            rc, errtext, errre = probe(path)
        finally:
            try: os.unlink(path)
            except OSError: pass
        if rc == 0:
            return body
        lines = {int(m.group(1)) for m in errre.finditer(errtext)}
        new = {linemap[L] for L in lines if L in linemap} - force
        if not new:
            return body            # nothing further we can pin -- leave as is
        force |= new
    return body

# LE object flags -> DDK segment class / name (inferred; the class strings are the DDK's own).
def seg_class(flags):
    if not (flags & 0x04):                                 # not executable -> data
        return ("IDATA", "_IDATA") if (flags & 0x10) else ("LDATA", "_LDATA")
    if flags & 0x8000:                                     # IOPL -> message-hook table
        return ("MCODE", "_LMSGTABLE")
    if flags & 0x10:                                       # discardable -> init code
        return ("ICODE", "_ITEXT")
    if flags & 0x40:                                       # preload+locked
        return ("LCODE", "_LTEXT")
    return ("PCODE", "_PTEXT")                              # pageable


def emit_tree(v, name, outdir, max_ins=6000, probe_nasm=None, probe_wasm=None):
    """Write the multi-file source tree for one VxD; returns the list of files written.
    With probe_nasm (a `nasm --masm` binary) or probe_wasm (an OpenWatcom `wasm` binary),
    each file is converged to assemble clean under that assembler by byte-preserving (db)
    the instructions it rejects."""
    probe = _nasm_probe(probe_nasm) if probe_nasm else (_wasm_probe(probe_wasm) if probe_wasm else None)
    D = VxDDisasm(v).run()
    D.v = v
    d = os.path.join(outdir, name.lower())
    os.makedirs(d, exist_ok=True)
    db = D.ddb
    written = []
    index = ["; %s -- recovered VxD source tree (multi-file, by segment + function)" % name, ";"]
    if db:
        index += ["; Declare_Virtual_Device %s, %d, %d, %s_Control, 0%04Xh" % (
            name, db["major"], db["minor"], name, db["device_id"]),
            "; DDK version 0%03Xh   init_order 0%08Xh" % (db["version"], db["init_order"]), ";"]
    index += ["; NOTE: a stripped LE binary carries no original file or (mostly) function names;",
              "; files are named by segment class and role. Exported service names, control-dispatch",
              "; handler names, and -- for the 12 core VxDs with a debug .SYM -- internal function",
              "; names are the real DDK names. Other internals are sub_<offset>.", "", "; files:"]
    for o in v.objects:
        oi = o["idx"]
        cls, seg = seg_class(o["flags"])
        offs = sorted(a for (oo, a) in D.insns if oo == oi)
        ranges = [(0, len(D.code[oi]))] if len(offs) <= max_ins else vxd_masm.split_ranges(D, oi, max_ins)
        for i, (lo, hi) in enumerate(ranges):
            suffix = "" if len(ranges) == 1 else "_%02d" % (i + 1)
            fn = "%s_%s_%02d%s.asm" % (name.lower(), cls.lower(), oi, suffix)
            extra = {db["rva"]: "VXD_DDB"} if (db and db["obj"] == oi and lo <= db["rva"] < hi) else {}
            head = ["; %s -- %s object %d segment %s%s" % (
                fn, name, oi, seg, "  (part %d/%d)" % (i + 1, len(ranges)) if len(ranges) > 1 else ""),
                ".386p", "%s segment dword public use32 '%s'" % (seg, cls), ""]
            tail = ["%s ends" % seg, "        end", ""]
            if probe:
                body = _converge_buildable(D, oi, lo, hi, extra, head, tail, probe)
            else:
                body, _ = vxd_masm.object_body(D, oi, extra_labels=extra, crange=(lo, hi))
            open(os.path.join(d, fn), "w").write("\r\n".join(head + body + tail))
            written.append(fn)
            index.append(";   %-28s object %d %s [0x%x-0x%x)" % (fn, oi, seg, lo, hi))
    open(os.path.join(d, "%s.txt" % name.lower()), "w").write("\r\n".join(index) + "\r\n")
    return written


if __name__ == "__main__":
    import sys
    from src.formats import le
    path = sys.argv[1]
    outdir = sys.argv[2] if len(sys.argv) > 2 else "recovered/vxd_src"
    for nm, v in le.open_vxds(path):
        files = emit_tree(v, nm.split(".")[0].upper(), outdir)
        print("%-14s -> %d files in %s/%s" % (nm, len(files), outdir, nm.split(".")[0].lower()))
