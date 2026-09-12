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

from src.engines.inverse.vxd_asm import VxDDisasm
from src.engines.inverse import vxd_masm

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


def emit_tree(v, name, outdir, max_ins=6000):
    """Write the multi-file source tree for one VxD; returns the list of files written."""
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
            body, _ = vxd_masm.object_body(D, oi, extra_labels=extra, crange=(lo, hi))
            head = ["; %s -- %s object %d segment %s%s" % (
                fn, name, oi, seg, "  (part %d/%d)" % (i + 1, len(ranges)) if len(ranges) > 1 else ""),
                ".386p", "%s segment dword public use32 '%s'" % (seg, cls), ""]
            tail = ["%s ends" % seg, "        end", ""]
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
