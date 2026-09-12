#!/usr/bin/env python3
"""
le_seg.py — a `trace.Seg`-shaped view of ONE LE executable object.

The VxD stack (every .VXD/.386, and VMM32's bundled core) is 32-bit flat code —
the SAME machine model as the PE engines. So an LE object, once its bytes are
extracted (le.py), reconstructs byte-exact through the EXACT SAME 32-bit pipeline
we already built for PE: capstone rows + `asmrecon32` (MASM 5.10A, .386/USE32).
This adapter is the bridge — it gives `asmrecon32.reconcile*` the shape it wants:

  .code / .rows / .funcs / .export_names / .mask / .va / .base

Function starts come from prologue scan (55 8B EC) + direct-call targets + the
object start (LE has no export table like PE). Fixups (the mask) are not wired
yet — a first proof emits literals, exactly as KERNEL32 (0 relocs) already did.
"""
import capstone
from codebreaker.core import cores

target = cores.get('target')
_di_le = cores.get('le')            # per-era LE parser (resolved on first use)
from codebreaker.formats.pe_seg import disasm_whole32

_MD = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
_MD.detail = True


class LeSeg:
    def __init__(self, module, obj_index=None, le=None):
        self.module = module
        if le is not None:                           # pre-parsed (e.g. W3-embedded LE)
            self.path = module
            self.le = le
        else:
            self.path = target.resolve(module)
            self.le = _di_le.LEFile(self.path)
        objs = self.le.objects()
        if obj_index is None:                        # default: first executable
            obj_index = next((i for i, o in enumerate(objs, 1) if o["exec"]), 1)
        self.obj_index = obj_index
        ob = objs[obj_index - 1]
        self.obj = ob
        self.code = self.le.object_bytes(ob)
        self.va = 0                                  # LE objects relocate to 0
        self.base = 0
        self.name = "obj%d" % obj_index
        self.rows = disasm_whole32(self.code)
        self.off2idx = {o: i for i, (o, _) in enumerate(self.rows)}
        self.mask = set()                            # LE fixups: TODO (emit literals)
        self.export_names = {}
        self.fixups = {}
        self.funcs = self._carve()

    def _carve(self):
        code = self.code
        n = len(code)
        starts = {0}
        insn = {ins.address: ins for ins in _MD.disasm(bytes(code), 0)}
        for a, ins in insn.items():                  # direct-call targets
            if ins.mnemonic == "call" and ins.operands and \
                    ins.operands[0].type == capstone.x86.X86_OP_IMM:
                t = ins.operands[0].imm
                if 0 <= t < n:
                    starts.add(t)
        for i in range(n - 3):                        # push ebp; mov ebp,esp
            if code[i] == 0x55 and code[i + 1] == 0x8B and code[i + 2] == 0xEC:
                starts.add(i)
        starts = {s for s in starts if s in self.off2idx}
        order = sorted(starts)
        out = {}
        for i, s in enumerate(order):
            e = order[i + 1] if i + 1 < len(order) else n
            out[s] = {"start": s, "end": e, "name": None}
        return out


def _demo():
    import sys
    mod = sys.argv[1] if len(sys.argv) > 1 else "adaptec.vxd"
    sg = LeSeg(mod)
    print("%s %s: %d bytes, %d rows, %d funcs (object %d of %d)"
          % (mod, sg.name, len(sg.code), len(sg.rows), len(sg.funcs),
             sg.obj_index, len(sg.le.objects())))
    small = sorted((f["end"] - f["start"], s) for s, f in sg.funcs.items()
                   if 16 <= f["end"] - f["start"] <= 120)
    for sz, s in small[:6]:
        print("   +%#06x  %3d B" % (s, sz))


if __name__ == "__main__":
    _demo()
