#!/usr/bin/env python3
"""Empirical register-input detector. Runs a function's ORIGINAL bytes in unicorn and varies each
GP register (holding stack args fixed); a register whose value changes the output is a genuine
INPUT. Everything else can be deregisterized to a local. Answers milestone-2's key question:
what fraction of functions have ZERO real register inputs (=> fully localizable)?
Run from the win95 data root. Usage: reg_inputs.py MODULE.DLL [...]"""
import os, sys, glob, random, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from codebreaker.core import cores
cores.set_target(paths.target_dir("win95"))
from codebreaker.formats.pe import open_pe
from codebreaker.formats.pe_seg import PeSeg
from codebreaker.formats.pe_trace import PeCarve
from codebreaker.core import paths

GP = ["eax", "ecx", "edx", "ebx", "esi", "edi", "ebp"]
MARK = 0x5A5A5A5A

def _emul(va0, code, fva, end, args, regvals):
    import unicorn as U
    from unicorn.x86_const import (UC_X86_REG_ESP, UC_X86_REG_EAX, UC_X86_REG_ECX, UC_X86_REG_EDX,
        UC_X86_REG_EBX, UC_X86_REG_ESI, UC_X86_REG_EDI, UC_X86_REG_EBP)
    RMAP = {"eax":UC_X86_REG_EAX,"ecx":UC_X86_REG_ECX,"edx":UC_X86_REG_EDX,"ebx":UC_X86_REG_EBX,
            "esi":UC_X86_REG_ESI,"edi":UC_X86_REG_EDI,"ebp":UC_X86_REG_EBP}
    uc = U.Uc(U.UC_ARCH_X86, U.UC_MODE_32)
    secbase = va0 & ~0xFFF
    size = (len(code) + (va0 - secbase) + 0xFFF) & ~0xFFF
    uc.mem_map(secbase, max(size, 0x1000)); uc.mem_write(va0, bytes(code))
    uc.mem_map(0x200000, 0x10000)
    # Pointer-input detection: a register holding a pointer the fn DEREFERENCES at entry would fault
    # every run -- including the all-zero baseline -- giving ran=0 and a false "no register inputs"
    # (the class of bug that deregisterized an edi buffer-pointer to an uninit local=0 -> NULL). Map
    # the low page (deref of 0 reads zero) and the MARK page (deref of MARK reads a valid scratch
    # pointer, so a pointer input shifts eax observably instead of crashing; double-deref stays mapped).
    for _b in (0x0, 0x5A5A0000):
        try:
            uc.mem_map(_b, 0x20000)
        except U.UcError:
            pass
    uc.mem_write(0x5A5A0000, b"\x00\x00\x20\x00" * (0x20000 // 4))   # each u32 == 0x00200000 (mapped)
    esp, RET = 0x208000, 0xDEAD0000
    for a in reversed(args):
        esp -= 4; uc.mem_write(esp, int(a & 0xffffffff).to_bytes(4, "little"))
    esp -= 4; uc.mem_write(esp, RET.to_bytes(4, "little"))
    uc.reg_write(UC_X86_REG_ESP, esp)
    for r in GP:
        uc.reg_write(RMAP[r], regvals.get(r, 0))
    try:
        uc.emu_start(fva, RET, count=50000)
    except U.UcError:
        return None
    return uc.reg_read(UC_X86_REG_EAX)

def detect(va0, code, fva, end, depth=8, trials=6):
    inputs = set()
    ran = 0
    for t in range(trials):
        rng = random.Random(fva * 131 + t)
        args = [rng.getrandbits(32) for _ in range(depth)]
        base = _emul(va0, code, fva, end, args, {})
        if base is None:
            continue
        ran += 1
        allset = _emul(va0, code, fva, end, args, {r: MARK for r in GP})
        if allset == base:
            continue                       # no register affects output this trial
        for r in GP:                       # something does -> find which
            if r in inputs:
                continue
            v = _emul(va0, code, fva, end, args, {r: MARK})
            if v != base:
                inputs.add(r)
    return inputs, ran

def main():
    hist = collections.Counter()
    per_reg = collections.Counter()
    n = n0 = nrun = 0
    for m in [a for a in sys.argv[1:] if not a.startswith("-")]:
        target = cores.get("target")
        pe = open_pe(target.resolve(m))
        va0, code, secname = pe.code_sections()[0]
        sg = PeSeg(m, section=secname, carve=PeCarve(target.resolve(m)))
        cnt = 0
        for off, f in sorted(sg.funcs.items()):
            sz = f["end"] - off
            if sz < 6 or sz > 400:         # skip tiny/huge; keep it fast + reliable
                continue
            inp, ran = detect(va0, code, va0 + off, f["end"])
            if ran == 0:
                continue
            n += 1; nrun += 1; cnt += 1
            if not inp:
                n0 += 1
            hist[len(inp)] += 1
            for r in inp:
                per_reg[r] += 1
            if cnt >= 120:                 # cap per module for speed
                break
        print("  %-16s tested=%d" % (m, cnt))
    print("\nfunctions run: %d" % n)
    print("  ZERO register inputs (fully localizable): %d (%d%%)" % (n0, 100*n0//max(1,n)))
    print("  by #inputs:", dict(sorted(hist.items())))
    print("  which regs are inputs (count):", dict(per_reg.most_common()))

if __name__ == "__main__":
    main()
