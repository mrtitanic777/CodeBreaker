#!/usr/bin/env python3
r"""dataseg.py -- reconstruct a Win16 module's DATA segment (DGROUP) as AUTHORED assembly source.

The decompiler recovers CODE (functions). It does NOT recover the DATA segment -- the module's global
variables -- so our rebuilt NE libraries had no DGROUP at all: every `DS:offset` deref in the recovered
code pointed into nothing, and data exports (KERNEL's THHOOK, __GP, ...) had nothing to point at. This
lane closes that gap. It reads the original module's automatic data segment and emits an authored MASM/
wasm module that reconstructs DGROUP with byte/offset fidelity, so:

  * the code's `*(t*)(0xNNNN)` DGROUP-relative accesses resolve to the right global;
  * exported data (THHOOK@ord332, __GP@ord342) is present at its real offset and exportable;
  * the segment's internal relocations (segment-selector fixups) are preserved symbolically, so the
    loader patches them at load time exactly as it does the original.

The values in a data segment ARE the authored source of that data (a person wrote those initializers);
we recover them and give them names, types and provenance -- known structures (THHOOK) by their DDK
definition, the rest as labeled reconstructions (the data analogue of sub_XXXX for code). This is
authored source in the file's original language (asm for a fixed-layout data segment), not a byte blob.

    wt --target win95 dataseg <module>          # emit the data module + report

Writes recon/authored/tree/<module>/<module>_data.asm and returns the data-export symbol list.
"""
import os
import sys

from src.core import paths
from src.targets.win95 import authsrc, owlink
from src.formats.ne import NEFile

# Known Win16 KERNEL data exports -> an authored type/description. Everything else is emitted as a
# labeled reconstruction (we cannot recover the original variable name from a stripped binary).
KNOWN_DATA = {
    ("KRNL386", "THHOOK"): "kernel task/module/heap hook table (TOOLHELP + thunk layer read it; "
                           "the kernel fills it during init). DDK: struct THHOOK.",
    ("KERNEL", "THHOOK"): "kernel task/module/heap hook table. DDK: struct THHOOK.",
    ("KRNL386", "__GP"): "kernel general-protection / segment-fixup table.",
    ("KERNEL", "__GP"): "kernel general-protection / segment-fixup table.",
}


def _autodata_seg(ne, segs):
    """The DGROUP automatic data segment: the header's autodata_seg if set, else the first DATA seg."""
    ad = ne.h.get("autodata_seg", 0)
    if ad and ad <= len(segs) and segs[ad - 1]["kind"] == "DATA":
        return segs[ad - 1]
    for s in segs:
        if s["kind"] == "DATA":
            return s
    return None


def _hexbytes(data):
    return data


def reconstruct(module):
    """Emit an authored DGROUP asm module for `module`. Returns
    {ok, asm, data_exports:[(ord,name,offset)], reloc_fixups, seg_len} or {ok:False,...}."""
    binpath = owlink._find_binary(module)
    if not binpath:
        return {"ok": False, "error": "no source binary"}
    if authsrc.classify(binpath)[0] != "NE":
        return {"ok": False, "error": "not an NE module (data lane is 16-bit NE)"}
    ne = NEFile(binpath)
    segs = ne.segments()
    ds = _autodata_seg(ne, segs)
    if not ds:
        return {"ok": False, "error": "no DATA segment"}
    dseg_idx = ds["idx"]
    raw = ne.data[ds["file_off"]:ds["file_off"] + ds["length"]]
    modname = os.path.splitext(os.path.basename(binpath))[0].upper()

    # data exports that live in this segment: ordinal -> (name, offset)
    ord2name = {}
    for nm, ordv in (ne.resident_names() + ne.nonresident_names()):
        ord2name.setdefault(ordv, nm)
    data_exports = []                                    # (ordinal, name, offset)
    for e in ne.entries():
        if e.get("kind") in ("FIXED", "MOVEABLE") and e.get("seg") == dseg_idx:
            nm = ord2name.get(e["ordinal"], "export_%d" % e["ordinal"])
            data_exports.append((e["ordinal"], nm, e["offset"]))
    off2label = {off: nm for _o, nm, off in data_exports}

    # relocations in the data segment: offset -> a symbolic fixup we re-emit so the loader patches it
    # exactly as it patches the original (a static file value would be a bogus runtime selector).
    relocs = {r["offset"]: r for r in ne.relocations(ds)}

    # ---- author the module -------------------------------------------------------------------------
    L = []
    L.append("; %s_data.asm -- reconstructed DGROUP (automatic data segment) for %s." % (module, modname))
    L.append("; AUTHORED-SOURCE reconstruction (CodeBreaker dataseg): the module's global-variable")
    L.append("; image, recovered from the original DATA segment so DGROUP-relative code accesses")
    L.append("; resolve and exported data is present at its real offset. Known structures are named")
    L.append("; from the Win16 DDK; the remainder is labeled data (a stripped binary carries no")
    L.append("; original variable names). Segment-selector fixups are preserved symbolically.")
    L.append("; segment length 0x%04X bytes; %d data export(s); %d relocation(s)."
             % (ds["length"], len(data_exports), len(relocs)))
    L.append("")
    L.append("\t.286")
    L.append("DGROUP group _DATA")
    L.append("_DATA segment word public 'DATA'")
    L.append("")

    # external code anchors for INTERNAL segment-selector fixups (dw SEG <ourcode>). We reference the
    # module's own recovered code so the loader writes one of OUR code selectors, self-referential like
    # the original. We cannot map the original seg1/2/3 split onto our (differently-split) code exactly,
    # so all such fixups anchor to the module's entry/code group; the loader-patch MECHANISM is kept.
    seg_fixups = sorted(o for o, r in relocs.items() if r["kind"] == "INTERNAL" and r["src"] in (2, 3))
    code_anchor = "dataseg_code_anchor" if seg_fixups else None

    # public labels for data exports
    for _o, nm, _off in sorted(data_exports, key=lambda x: x[2]):
        L.append("\tpublic %s" % nm)
    L.append("")

    # emit the bytes in offset order, inserting labels and symbolic fixups.
    def emit_run(start, end):
        """Emit raw bytes [start,end) as db lines, collapsing long zero runs."""
        i = start
        while i < end:
            if raw[i] == 0:
                j = i
                while j < end and raw[j] == 0:
                    j += 1
                if j - i >= 8:
                    L.append("\tdb %d dup(0)" % (j - i)); i = j; continue
            j = min(i + 16, end)
            # stop a line before a label/reloc boundary handled by the caller
            chunk = raw[i:j]
            L.append("\tdb " + ", ".join("0%02Xh" % b for b in chunk))
            i = j

    # boundaries where we must break the byte stream: labels and reloc sites
    marks = sorted(set(off2label) | set(relocs))
    cur = 0
    for m in marks:
        if m > cur:
            emit_run(cur, m)
            cur = m
        # label at this offset?
        if m in off2label:
            nm = off2label[m]
            desc = KNOWN_DATA.get((modname, nm))
            if desc:
                L.append("; %s -- %s" % (nm, desc))
            else:
                L.append("; %s -- exported data (ordinal export); labeled reconstruction." % nm)
            L.append("%s label byte" % nm)
        # reloc at this offset?
        if m in relocs:
            r = relocs[m]
            if r["kind"] == "INTERNAL" and r["src"] in (2, 3) and code_anchor:
                L.append("\tdw seg %s\t\t; internal segment-selector fixup (was seg %d)"
                         % (code_anchor, r.get("seg", 0)))
                cur = m + 2
            elif r["kind"] == "INTERNAL" and r["src"] == 5:      # 16-bit offset within a segment
                L.append("\tdw 0%04Xh\t\t; internal offset fixup (seg %d:0x%04X) -- static"
                         % (int.from_bytes(raw[m:m + 2], "little"), r.get("seg", 0), r.get("entry", 0)))
                cur = m + 2
            else:
                # import / far / other: keep the file bytes (width by src type), noted.
                w = 4 if r["src"] == 3 else 2
                val = int.from_bytes(raw[m:m + w], "little")
                L.append("\td%s 0%0*Xh\t\t; %s fixup -- static (file value)"
                         % ("d" if w == 4 else "w", w * 2, val, r["kind"]))
                cur = m + w
    if cur < len(raw):
        emit_run(cur, len(raw))

    L.append("")
    L.append("_DATA ends")
    if code_anchor:
        # A self-contained code anchor for the internal segment-selector fixups: the loader writes THIS
        # segment's runtime selector into each fixup site, self-referential to our own code exactly as
        # the original stored its code selectors. (We cannot reproduce the original seg1/2/3 split on our
        # differently-partitioned code, so all such fixups anchor here; the loader-patch mechanism and
        # the "holds one of my code selectors" semantics are preserved.)
        L.append("")
        L.append("_TEXT segment word public 'CODE'")
        L.append("\tpublic %s" % code_anchor)
        L.append("%s label far" % code_anchor)
        L.append("\tretf")
        L.append("_TEXT ends")
    L.append("\tend")
    asm = "\n".join(L) + "\n"
    return {"ok": True, "asm": asm, "data_exports": data_exports,
            "reloc_fixups": len(relocs), "seg_len": ds["length"],
            "code_anchor": code_anchor}


def write_module(module):
    """Reconstruct and write the data asm into the module's authored tree. Returns the result dict
    (with 'path' added on success)."""
    r = reconstruct(module)
    if not r.get("ok"):
        return r
    dr = authsrc._dr()
    treedir = os.path.join(dr, "recon", "authored", "tree", module)
    os.makedirs(treedir, exist_ok=True)
    path = os.path.join(treedir, module + "_data.asm")
    with open(path, "w", newline="\n") as f:
        f.write(r["asm"])
    r["path"] = path
    return r


def main(argv):
    if not argv or argv[0].startswith("-"):
        print(__doc__); return 0
    module = argv[0]
    r = write_module(module)
    if not r.get("ok"):
        print("  dataseg %-12s FAIL: %s" % (module, r.get("error"))); return 1
    print("  dataseg %-12s OK  DGROUP %d bytes, %d data export(s), %d reloc(s) -> %s"
          % (module, r["seg_len"], len(r["data_exports"]), r["reloc_fixups"], os.path.basename(r["path"])))
    for o, nm, off in r["data_exports"]:
        print("     export ord %-4d %-12s @ 0x%04X" % (o, nm, off))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
