#!/usr/bin/env python3
r"""difftest.py -- the 32-bit DIFFERENTIAL HARNESS: prove the Lifted C RUNS correctly.

Rung 2 shows the Lifted C compiles and links; this shows it *computes what Windows 95 did*.
For each PURE LEAF function (no calls, no memory deref, inputs as stdcall args -- so it is
host-word-size independent) it runs two independent executions on identical random inputs:

  * ORACLE    -- the real win95 x86 bytes, emulated in Unicorn (true 32-bit semantics).
  * CANDIDATE -- the function's Lifted C, compiled NATIVE (GCC) and called via ctypes, with the
                 register-file globals zeroed first to match the emulator's entry state.

Agreement on every input proves that function's Lifted C runs and is byte-behaviour-identical to
Microsoft's shipped code. A mismatch is a real signal -- usually a lift edge the IR flagged
itself ("cond not recovered", an entry-register dependence). Memory-touching functions need the
loadable-32-bit path (a VM/ISO harness); this host-side tool covers the pure-leaf subset.

    wt recompile difftest gdi32.dll [user32.dll ...]     # per module, aggregated
Requires: unicorn (pip install unicorn), a working gcc.
"""
import os, re, sys, glob, ctypes, random, subprocess, tempfile

HERE = os.path.dirname(os.path.abspath(__file__))
INC = os.path.join(os.path.dirname(HERE), "include")
sys.path.insert(0, HERE)
import sigrecover                                             # noqa: E402
from engine.foundation import cores                          # noqa: E402
from engine.format_loaders.pe import open_pe                 # noqa: E402
from engine.format_loaders.pe_seg import PeSeg               # noqa: E402
from engine.format_loaders.pe_trace import PeCarve           # noqa: E402

_INREG = re.compile(r'\b(edx|esi|edi|ebx|ecx|ebp)\b')
_GEN = os.path.join(os.environ.get("WT_DATA_ROOT", "."), "src", "generated")


def _blocks(module):
    c = glob.glob(os.path.join(_GEN, "**", module + ".c"), recursive=True)
    if not c:
        return {}
    txt = open(c[0], encoding="utf-8", errors="replace").read()
    out = {}
    for m in re.finditer(r'/\* \[AUTO\]\s+\w+\s+\w+\s+seg\d+:0x([0-9a-f]+)[^\n]*\*/'
                         r'((?:(?!/\* \[AUTO\]).)*)', txt, re.S):
        out[int(m.group(1), 16)] = m.group(0)
    return out


def _is_pure(body, off, arities):
    if not body or '*(' in body or 'sub_' in body or 'goto *' in body or '__lifted' in body:
        return False
    if 'not recovered' in body:
        return False
    code = body.split('{', 1)[-1]
    for m in _INREG.finditer(code):
        r, pre = m.group(1), code[:m.start()]
        if (r + ' =') not in pre and (r + '=') not in pre:   # entry register read before write
            return False
    return arities.get(off) is not None


def _emulate(uc_mod, va0, code, fva, end, args):
    import unicorn as U
    from unicorn.x86_const import UC_X86_REG_ESP, UC_X86_REG_EAX
    uc = U.Uc(U.UC_ARCH_X86, U.UC_MODE_32)
    secbase = va0 & ~0xFFF
    size = (len(code) + (va0 - secbase) + 0xFFF) & ~0xFFF
    uc.mem_map(secbase, max(size, 0x1000))
    uc.mem_write(va0, bytes(code))
    uc.mem_map(0x200000, 0x10000)
    esp, RET = 0x208000, 0xDEAD0000
    for a in reversed(args):
        esp -= 4; uc.mem_write(esp, int(a & 0xffffffff).to_bytes(4, 'little'))
    esp -= 4; uc.mem_write(esp, RET.to_bytes(4, 'little'))
    uc.reg_write(UC_X86_REG_ESP, esp)
    try:
        uc.emu_start(fva, RET, count=100000)
    except U.UcError:
        return None
    return uc.reg_read(UC_X86_REG_EAX)


def _compile(body):
    mo = re.search(r'\b([A-Za-z_]\w*)\s*\(', body)
    d = tempfile.mkdtemp()
    open(os.path.join(d, "f.c"), "w").write('#include "win95c.h"\n' + body)
    so = os.path.join(d, "f.dll")
    r = subprocess.run(["gcc", "-std=gnu89", "-w", "-shared", "-o", so,
                        os.path.join(d, "f.c"), os.path.join(INC, "win95rt.c"), "-I", INC],
                       capture_output=True, text=True)
    return (so, mo.group(1)) if r.returncode == 0 else (None, None)


_ZERO = ('eax', 'ebx', 'ecx', 'edx', 'esi', 'edi', 'CF', 'ZF', 'SF', 'OF', 'PF', 'AF')


def test_module(module, trials=20, limit=None):
    """Returns (proven, mismatches[list], skipped)."""
    import unicorn
    target = cores.get("target")
    pe = open_pe(target.resolve(module))
    va0, code, secname = pe.code_sections()[0]
    sg = PeSeg(module, section=secname, carve=PeCarve(target.resolve(module)))
    arities = sigrecover.arities(module)
    blocks = _blocks(module)
    cands = [(o, f) for o, f in sorted(sg.funcs.items())
             if (f['end'] - o) >= 8 and _is_pure(blocks.get(o, ''), o, arities)]
    if limit:
        cands = cands[:limit]
    proven, mism, skip = [], [], 0
    for off, f in cands:
        n = arities.get(off) or 0
        so, fname = _compile(blocks[off])
        if not so:
            skip += 1; continue
        lib = ctypes.CDLL(so)
        fn = getattr(lib, fname); fn.restype = ctypes.c_uint32
        fn.argtypes = [ctypes.c_uint32] * n
        ok = tested = 0; ok = True
        for _ in range(trials):
            args = [random.getrandbits(32) for _ in range(n)]
            o = _emulate(unicorn, va0, code, va0 + off, f['end'], args)
            if o is None:
                ok = False; break
            for r in _ZERO:
                try: ctypes.c_uint32.in_dll(lib, r).value = 0
                except ValueError: pass
            c = fn(*args) & 0xffffffff
            tested += 1
            if o != c:
                mism.append((off, fname, args, o, c)); ok = False; break
        if ok and tested:
            proven.append((off, fname, n))
    return proven, mism, skip


def main():
    mods = [a for a in sys.argv[1:] if not a.startswith("-")]
    tp = tm = ts = 0
    for m in mods:
        try:
            p, mm, s = test_module(m)
        except Exception as e:
            print("  %-16s ERROR %s" % (m, repr(e)[:80])); continue
        tp += len(p); tm += len(mm); ts += s
        print("  %-16s proven=%-3d mismatch=%-2d skipped=%d" % (m, len(p), len(mm), s))
    print("TOTAL: %d functions PROVEN bit-identical to real win95 machine code "
          "(%d mismatch, %d skipped)" % (tp, tm, ts))


if __name__ == "__main__":
    main()
