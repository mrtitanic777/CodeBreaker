#!/usr/bin/env python3
"""
pe_seg.py — a `trace.Seg`-shaped view of ONE PE code section (32-bit byte-exact).

`asmrecon.py` reconstructs a 16-bit NE function to byte-exact assembly by driving
`trace.Seg` (segment bytes + objdump rows + carved functions). `asmrecon32.py`
does the same for the 32-bit PE engines (KERNEL32/USER32/GDI32), and it needs the
same shape. This adapter provides it for a single executable section:

  .code          the section's raw bytes (index 0 == section start)
  .rows          objdump -mi386 -Mintel linear disasm, section-relative offsets
  .funcs         {start_off: {start,end,name}} carved by pe_trace.PeCarve
  .export_names  {start_off: export_name}
  .mask          section-relative byte offsets covered by base relocations
  .va            the section's virtual address (image_base + vaddr)

Offsets are section-relative throughout so the reconcile loop is identical to the
NE path — only the disassembly width (32-bit) and the assembler wrapper (.386 /
USE32) differ, and those live in asmrecon32.
"""
import capstone
from codebreaker.core import cores

target = cores.get('target')
from codebreaker.formats.pe import open_pe
from codebreaker.formats.pe_trace import PeCarve

_MD32 = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)


def disasm_whole32(code, base=0, progress_cb=None):
    """Linearly decode the blob as 32-bit code with capstone; return
    [(offset, asm_text), ...] with CONTIGUOUS coverage (undecodable bytes become
    a 1-byte '(bad)' row -> db). capstone is used instead of objdump because
    objdump's linear sweep MIS-SPLITS prefixed instructions (a segment/opsize
    prefix like `64`/`66` gets peeled onto its own row), which the byte-exact
    reconcile then cannot realign. capstone decodes each prefixed instruction as
    one unit with the correct length, so the reconcile converges cleanly.

    progress_cb(bytes_done) is called periodically so a caller can drive a
    per-section disassembly progress bar."""
    rows = []
    n = len(code)
    pos = 0
    cnt = 0
    while pos < n:
        ins = next(iter(_MD32.disasm(bytes(code[pos:pos + 15]), base + pos, count=1)),
                   None)
        if ins is None:
            rows.append((base + pos, "(bad)"))
            pos += 1
        else:
            text = ins.mnemonic if not ins.op_str else "%s %s" % (ins.mnemonic, ins.op_str)
            rows.append((base + pos, text))
            pos += ins.size
        cnt += 1
        if progress_cb and cnt % 2000 == 0:
            progress_cb(pos)
    if progress_cb:
        progress_cb(n)
    return rows


class PeSeg:
    """One PE executable section, shaped like trace.Seg for asmrecon32."""

    def __init__(self, module, section=None, carve=None, progress_cb=None):
        # carve: a pre-built PeCarve to REUSE (a multi-section PE otherwise re-runs
        # the whole capstone sweep once per section). progress_cb drives the
        # per-section disassembly bar.
        self.module = module
        self.path = target.resolve(module)
        self.pe = open_pe(self.path)
        self.base = self.pe.image_base
        secs = self.pe.code_sections()            # [(va, bytes, name)]
        if not secs:
            raise ValueError("%s: no executable section" % module)
        if section is None:                        # default: the largest .text-like
            pick = next((s for s in secs if s[2] == ".text"), None) or \
                max(secs, key=lambda s: len(s[1]))
        else:
            pick = next((s for s in secs if s[2] == section), None)
            if pick is None:
                raise ValueError("%s: no section %r" % (module, section))
        self.va, self.code, self.name = pick
        self.secva = self.va                       # image VA of section start
        self.rows = disasm_whole32(self.code, progress_cb=progress_cb)
        self.off2idx = {o: i for i, (o, _) in enumerate(self.rows)}

        # carve functions with PeCarve (capstone, export-table driven), then keep
        # those that fall inside THIS section and rebase VA -> section offset.
        # Reuse a caller-supplied carve when given (avoids re-sweeping every
        # section of a multi-section module).
        if carve is None:
            carve = PeCarve(self.path)
        lo, hi = self.va, self.va + len(self.code)
        self.funcs = {}
        self.export_names = {}
        for va, f in carve.funcs.items():
            if lo <= va < hi:
                s = va - self.va
                e = min(f["end"], hi) - self.va
                self.funcs[s] = {"start": s, "end": e, "name": f["name"]}
                if f["name"]:
                    self.export_names[s] = f["name"]
        self._carve = carve                        # keep for call resolution/annotate

        # base-relocation mask, section-relative (each HIGHLOW covers 4 bytes)
        self.mask = set()
        rvbase = self.va - self.base
        for rva in self.pe.base_relocs():
            o = rva - rvbase
            if 0 <= o < len(self.code):
                for k in range(4):
                    if o + k < len(self.code):
                        self.mask.add(o + k)

        # fixups: resolved call sites for annotate (site_off -> "DLL!name").
        # objdump/asmrecon don't need it for byte-exactness; annotate uses it.
        self.fixups = {}


def _demo():
    import sys
    mod = sys.argv[1] if len(sys.argv) > 1 else "USER32.DLL"
    sg = PeSeg(mod)
    print("%s section %s: va=%#x  %d bytes  %d rows  %d funcs  %d mask bytes"
          % (mod, sg.name, sg.va, len(sg.code), len(sg.rows), len(sg.funcs),
             len(sg.mask)))
    small = sorted((f["end"] - f["start"], s) for s, f in sg.funcs.items()
                   if 16 <= f["end"] - f["start"] <= 80)
    for sz, s in small[:8]:
        print("   %#06x  %3d B  %s" % (s, sz, sg.funcs[s]["name"] or "sub"))


if __name__ == "__main__":
    _demo()
