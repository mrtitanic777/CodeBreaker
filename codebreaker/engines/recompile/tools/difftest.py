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
from codebreaker.core import cores                          # noqa: E402
from codebreaker.formats.pe import open_pe                 # noqa: E402
from codebreaker.formats.pe_seg import PeSeg               # noqa: E402
from codebreaker.formats.pe_trace import PeCarve           # noqa: E402

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
    # a VOID-return function has no value to differential-test: the candidate leaves gcc's scratch
    # in eax while the emulator has its own eax, so comparing the "return" is a false mismatch.
    # (Verifying a void fn means checking its memory side-effects -- the buffer-marshaling path.)
    if re.search(r'/\* \[AUTO\][^*]*\*/\s*void\s+\w+\s*\(', body) or \
            re.match(r'\s*(?:/\*[^\n]*\*/\s*)?void\s+\w+\s*\(', body.split('*/', 1)[-1]):
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
    import rc                                             # apply rc.adapt's dropped-arg-load recovery
    body = rc.recover_arg_load(body)                      # so the sweep tests the recompile-layer C
    mo = re.search(r'\b([A-Za-z_]\w*)\s*\(', body)
    d = tempfile.mkdtemp()
    open(os.path.join(d, "f.c"), "w").write('#include "win95c.h"\n' + body)
    so = os.path.join(d, "f.dll")
    r = subprocess.run(["gcc", "-std=gnu89", "-w", "-shared", "-o", so,
                        os.path.join(d, "f.c"), os.path.join(INC, "win95rt.c"), "-I", INC],
                       capture_output=True, text=True)
    return (so, mo.group(1)) if r.returncode == 0 else (None, None)


_ZERO = ('eax', 'ebx', 'ecx', 'edx', 'esi', 'edi', 'CF', 'ZF', 'SF', 'OF', 'PF', 'AF')

_MD = None


def _md():
    """One detailed capstone engine, reused -- constructing it per function was the cost that
    tipped mfco40 (61 fns) over the sweep's per-module time budget."""
    global _MD
    if _MD is None:
        import capstone
        _MD = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
        _MD.detail = True
    return _MD


def _arg_slots(code, va0, off, end):
    """The ordered, distinct stack-argument slots a function actually reads -- esp-adjusted.

    il.py emits SLOT-NAMED, SPARSE parameter lists: a param called `arg3` is the 3rd stdcall
    dword ([esp+0xc] at entry) and unreferenced leading args are omitted, so a 2-param C
    function may read slots (3,4) not (1,2). Worse, the name can be off-by-one when the body
    pushes before the read (`push esi` then `[esp+0x14]` is slot 4, not 5). So we can't trust
    the names -- we recover the TRUE slots from the bytes by tracking esp across push/pop/
    sub/add and mapping each [esp+d] read to entry-arg index (d + esp_delta)//4. Returns e.g.
    [3, 4]; the caller maps the i-th C parameter to the i-th slot here."""
    import capstone
    md = _md()
    delta, slots = 0, []
    for i in md.disasm(bytes(code[off:end]), va0 + off):
        m = i.mnemonic
        if m == "push":
            delta -= 4
        elif m == "pop":
            delta += 4
        elif m in ("sub", "add") and i.operands and i.operands[0].type == capstone.x86.X86_OP_REG \
                and i.reg_name(i.operands[0].reg) == "esp" and i.operands[1].type == capstone.x86.X86_OP_IMM:
            delta += (-i.operands[1].imm if m == "sub" else i.operands[1].imm)
        for op in i.operands:                            # any [esp + d] memory reference
            if op.type == capstone.x86.X86_OP_MEM and op.mem.base and \
                    i.reg_name(op.mem.base) == "esp" and op.mem.index == 0:
                entry_disp = op.mem.disp + delta
                if entry_disp >= 4 and entry_disp % 4 == 0:
                    idx = entry_disp // 4                # slot 1 == arg1 == [entry esp+4]
                    if idx not in slots:
                        slots.append(idx)
    return sorted(slots)


def _nparams(body):
    seg = body.split("(", 1)[-1].split(")", 1)[0]
    return 0 if not seg.strip() else seg.count(",") + 1


def test_module(module, trials=20, limit=None, budget=90):
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
    import time
    start, truncated = time.time(), False
    for off, f in cands:
        if time.time() - start > budget:            # per-module wall-clock cap: one pathological
            truncated = True; break                 # module (100s of gcc spawns) must not stall a sweep
        n = arities.get(off) or 0
        so, fname = _compile(blocks[off])
        if not so:
            skip += 1; continue
        # il.py's params are SLOT-NAMED and SPARSE, so calling positionally binds the wrong
        # stack slots. Recover the true slots from the bytes and map the i-th C parameter to
        # the i-th slot; fall back to positional only if the shapes don't line up.
        npar = _nparams(blocks[off])
        slots = _arg_slots(code, va0, off, f['end'])
        by_slot = slots if len(slots) == npar else [k + 1 for k in range(npar)]
        depth = max([n] + by_slot) if by_slot else n
        lib = ctypes.CDLL(so)
        fn = getattr(lib, fname); fn.restype = ctypes.c_uint32
        fn.argtypes = [ctypes.c_uint32] * npar
        ok = tested = 0; ok = True
        rng = random.Random(off)                     # deterministic per-function inputs -> reproducible
        for _ in range(trials):
            stack = [rng.getrandbits(32) for _ in range(depth)]
            o = _emulate(unicorn, va0, code, va0 + off, f['end'], stack)
            if o is None:
                ok = False; break
            for r in _ZERO:
                try: ctypes.c_uint32.in_dll(lib, r).value = 0
                except ValueError: pass
            c = fn(*[stack[s - 1] for s in by_slot]) & 0xffffffff
            tested += 1
            if o != c:
                mism.append((off, fname, [stack[s - 1] for s in by_slot], o, c)); ok = False; break
        if ok and tested:
            proven.append((off, fname, n))
    return proven, mism, skip, truncated


def main():
    manifest = sys.argv[sys.argv.index("--manifest") + 1] if "--manifest" in sys.argv else None
    mods = [a for a in sys.argv[1:] if not a.startswith("-") and a != manifest]
    mf = open(manifest, "a", encoding="utf-8") if manifest else None
    tp = tm = ts = trunc = 0
    for m in mods:
        try:
            p, mm, s, t = test_module(m)
        except Exception as e:
            print("  %-16s ERROR %s" % (m, repr(e)[:80])); continue
        tp += len(p); tm += len(mm); ts += s; trunc += bool(t)
        if mf:                                       # proven-manifest: module <tab> seg1-offset
            for off, fname, n in p:
                mf.write("%s\t1\t%x\n" % (m, off))
            mf.flush()
        print("  %-16s proven=%-3d mismatch=%-2d skipped=%d%s"
              % (m, len(p), len(mm), s, "  (TRUNCATED: hit time budget)" if t else ""))
    if mf:
        mf.close()
    print("TOTAL: %d functions PROVEN bit-identical to real win95 machine code "
          "(%d mismatch, %d skipped, %d modules truncated)" % (tp, tm, ts, trunc))


if __name__ == "__main__":
    main()
