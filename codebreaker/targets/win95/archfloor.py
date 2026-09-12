#!/usr/bin/env python3
r"""archfloor.py -- classify the ASM-floor residue into ARCHITECTURAL floor vs genuinely
unresolved.

The fused reconstruct floors a function when no faithful C/behavioral model applies. But "floor"
conflates two very different things:

  * ARCHITECTURAL floor -- the function IS machine-level glue with NO ring-3 C equivalent:
    16<->32 flat thunks and segment transitions (load/store segment registers, far `retf`/
    `call far`, `lds/les/lfs/lgs/lss`), VxD dynamic-link service calls (`int 20h`), privileged
    ops (`hlt`, `cli`, `iret`). Flooring these to byte-exact asm is CORRECT, not a gap -- the
    semantics live in the 16-bit world or ring 0 (see docs win95/THUNKING, VMM_VXD_SERVICES,
    DEVICE_IO_VXD). The honest output is byte-exact asm + a label saying why.

  * UNRESOLVED -- a normal ring-3 function a future reconstructor could still get.

Separating them makes the reconstruction rate honest: it should be measured against the
RECONSTRUCTABLE denominator (total minus architectural floor), not inflated or deflated by glue
that was never reconstructable-as-C. This module scans a function's instructions for the
architectural markers and returns a category; it does not attempt reconstruction.

    wt --target win95 archfloor <MODULE> [--limit N]
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import pe32util as H

# strong architectural markers -> (category, why)
_SEG_REGS = {"es", "cs", "ss", "ds", "fs", "gs"}
_SEG_LOADERS = {"lds", "les", "lfs", "lgs", "lss"}
_PRIV = {"hlt", "cli", "sti", "iret", "iretd", "lgdt", "lidt", "lldt", "ltr", "clts", "invd"}


def _md():
    import capstone
    return capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)


def classify(module, rva, max_bytes=512):
    """Return (category, why, evidence) for the function at `rva`, or (None, None, None) if it
    shows no architectural-floor marker (i.e. it's a normal ring-3 function). Linear sweep from
    the entry -- marker detection, not a full CFG."""
    pe = H.carve(module).pe
    off = pe._rva_to_off(rva)
    if off is None:
        return None, None, None
    code = pe.data[off:off + max_bytes]
    md = _md()
    for ins in md.disasm(code, rva):
        m, ops = ins.mnemonic, ins.op_str
        if m == "int" and ops.strip() in ("0x20", "0x20h", "20h"):
            return "vxd-service", "INT 20h VxD dynamic-link service call", "0x%x: %s %s" % (ins.address, m, ops)
        if m == "retf" or m.endswith("call far") or m.endswith("jmp far") or "far" in m:
            return "thunk-segment", "far transfer (16:32 thunk boundary)", "0x%x: %s %s" % (ins.address, m, ops)
        if m in _SEG_LOADERS:
            return "thunk-segment", "far pointer load (%s)" % m, "0x%x: %s %s" % (ins.address, m, ops)
        if m == "mov" and any(r in _SEG_REGS for r in _seg_operands(ops)):
            return "thunk-segment", "segment-register load/store", "0x%x: %s %s" % (ins.address, m, ops)
        if m in _PRIV:
            return "privileged", "privileged instruction (%s) -- ring-0 / not ring-3 C" % m, \
                   "0x%x: %s %s" % (ins.address, m, ops)
        # NOTE: do not stop at the first `ret`. Segment/thunk glue frequently has an early
        # fast-path `ret` with the segment transition in a later branch (e.g. kernel32 sub_1ad7:
        # `or eax,eax; jne L; ret; L: mov gs,fs:[..]`), so a marker can live past the first ret.
        # Linear sweep across the bounded window finds it; markers are strong + rare enough that
        # minor bleed into an adjacent function won't false-positive a normal ring-3 function.
    return None, None, None


def _seg_operands(op_str):
    """Operands that are bare segment registers (mov gs, ax / mov ax, fs)."""
    out = []
    for tok in op_str.replace(",", " ").split():
        t = tok.strip().lower()
        if t in _SEG_REGS:
            out.append(t)
    return out


def classify_module(module, rvas):
    """Classify a list of floored RVAs; return {rva: (category, why, evidence)} for those that
    are architectural floor (omits the genuinely-unresolved ones)."""
    out = {}
    for rva in rvas:
        cat, why, ev = classify(module, rva)
        if cat:
            out[rva] = (cat, why, ev)
    return out


from codebreaker.cli.common import opt as _opt


def main():
    import collections
    from codebreaker.targets.win95 import compose
    argv = sys.argv[1:]
    if not argv or argv[0] in ("-h", "--help"):
        print(__doc__); return
    module = argv[0]
    limit = _opt(argv, "--limit"); limit = int(limit) if limit is not None else 24
    H.ensure_target()
    rs = compose.reconstruct_module(module, limit, fuse=True, behav_limit=limit)
    floored = [r.rva for r in rs if r.status == "floor"]
    tot = len(rs) or 1
    arch = classify_module(module, floored)
    cats = collections.Counter(v[0] for v in arch.values())
    recon_denom = tot - len(arch)
    readable = sum(1 for r in rs if r.status != "floor")
    print("=== %s: floor breakdown (%d functions) ===" % (module, tot))
    print("  readable (C/behavioral)       : %d" % readable)
    print("  ARCHITECTURAL floor           : %d  %s" % (len(arch), dict(cats)))
    print("  genuinely unresolved          : %d" % (len(floored) - len(arch)))
    print("  --")
    print("  reconstruction rate (raw)     : %d/%d (%.0f%%)" % (readable, tot, 100.0 * readable / tot))
    print("  rate vs RECONSTRUCTABLE       : %d/%d (%.0f%%)  <- excludes architectural glue" %
          (readable, recon_denom, 100.0 * readable / (recon_denom or 1)))
    for rva, (cat, why, ev) in sorted(arch.items()):
        print("    sub_%05x  [%s] %s  (%s)" % (rva, cat, why, ev))


if __name__ == "__main__":
    main()
