#!/usr/bin/env python3
"""
sigscan.py — recognize common library / CRT helpers by code shape.

`symdb` answers "what is the prototype of `memcpy`"; it cannot tell you that an
unnamed `sub_40966d` *is* a byteswap. This is the FLIRT-style step: match a
carved function against a small, high-precision table of structural signatures
and hand back a name. It complements `names`/`symdb` (which name by export /
ordinal / SDK) with naming by *behavior*, so leaf helpers a stripped binary
never named become readable.

A signature is a predicate over a `Fn` view (start/end/size/mnemonics/imm/bytes).
The table is conservative — it fires only on unambiguous shapes, so a hit is
trustworthy and precision is preferred over recall. Rows are independent; add
freely. Input is a `pe_trace.PeCarve` (32-bit PE); the shapes are generic x86.

    python sigscan.py foo.exe          # recognized helpers (needs a --target ctx via wt)
    python sigscan.py --selftest
"""
import sys


class Fn:
    """A carved function as the signatures see it."""
    def __init__(self, carve, start):
        f = carve.funcs[start]
        self.start, self.end = start, f["end"]
        self.size = self.end - start
        self.mnem = []
        self.imm = set()
        base = data = None
        for b, dd, _n in carve.pe.code_sections():
            if b <= start < b + len(dd):
                base, data = b, dd
                break
        self.bytes = data[start - base:self.end - base] if data else b""
        va = start
        while va < self.end:
            rec = carve.insn.get(va)
            if rec is None:
                va += 1
                continue
            mn, op, size, ins, _t = rec
            self.mnem.append((mn, op))
            for o in ins.operands:
                if getattr(o, "type", None) == 2:          # X86_OP_IMM
                    self.imm.add(o.imm & 0xFFFFFFFF)
            va += size

    def has(self, mnem):
        return any(m == mnem for m, _ in self.mnem)


# -- signature predicates (generic x86 / MSVC-CRT shapes) ---------------------
def _byteswap_uint64(fn):
    return fn.size < 0x30 and sum(1 for m, _ in fn.mnem if m == "bswap") >= 2

def _byteswap_ulong(fn):
    if fn.size >= 0x30:
        return False
    if any(m == "bswap" for m, _ in fn.mnem):
        return True
    return (0xFF00 in fn.imm and 0xFF0000 in fn.imm
            and any(m == "shl" and "0x10" in o for m, o in fn.mnem))

def _byteswap_ushort(fn):
    if fn.size >= 0x20 or 0xFF0000 in fn.imm:
        return False
    return (any(m == "shr" and o.endswith(" 8") for m, o in fn.mnem)
            and any(m == "mov" and ("ch," in o or "cl," in o) for m, o in fn.mnem))

def _security_check_cookie(fn):
    if fn.size >= 0x1C or fn.has("call"):
        return False
    cmp_ecx = any(m == "cmp" and o.startswith("ecx,") and "[" in o for m, o in fn.mnem)
    return cmp_ecx and fn.has("jne") and fn.has("ret")

def _rep(fn, kind):
    return any(m in ("rep " + kind + "d", "rep " + kind + "b")
               or (m == "rep" and kind in o) for m, o in fn.mnem)

def _memcpy(fn):
    return _rep(fn, "movs") and fn.size < 0x50 and not fn.has("call")

def _memset(fn):
    return _rep(fn, "stos") and fn.size < 0x50 and not fn.has("call")


SIGNATURES = [
    ("_byteswap_uint64", 0.95, _byteswap_uint64),
    ("_byteswap_ulong",  0.90, _byteswap_ulong),
    ("_byteswap_ushort", 0.80, _byteswap_ushort),
    ("__security_check_cookie", 0.85, _security_check_cookie),
    ("memcpy", 0.60, _memcpy),
    ("memset", 0.60, _memset),
]


def identify(carve, start):
    """Best (name, confidence) for the function at `start`, or None."""
    fn = Fn(carve, start)
    best = None
    for name, conf, pred in SIGNATURES:
        try:
            if pred(fn) and (best is None or conf > best[1]):
                best = (name, conf)
        except Exception:
            pass
    return best


def identify_all(carve):
    """{func_start: (name, confidence)} for every recognized helper."""
    out = {}
    for s in carve.funcs:
        r = identify(carve, s)
        if r:
            out[s] = r
    return out


def _selftest():
    import capstone
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True

    def carve_for(code, start=0x401000):
        class _C:
            class pe:
                @staticmethod
                def code_sections():
                    return [(start, code, ".text")]
            imports = {}
            insn = {}
            funcs = {start: {"start": start, "end": start + len(code), "name": None}}
        c = _C()
        for ins in md.disasm(code, start):
            c.insn[ins.address] = (ins.mnemonic, ins.op_str, ins.size, ins, None)
        return c

    assert identify(carve_for(b"\x0f\xc8\xc3"), 0x401000)[0] == "_byteswap_ulong"
    assert identify(carve_for(b"\x0f\xc8\x0f\xc9\xc3"), 0x401000)[0] == "_byteswap_uint64"
    print("[OK] sigscan byteswap32/64")
    return True


def main():
    argv = sys.argv[1:]
    if not argv or argv[0] in ("-h", "--help"):
        sys.exit(__doc__)
    if argv[0] == "--selftest":
        sys.exit(0 if _selftest() else 1)
    from codebreaker.format_loaders.pe_trace import PeCarve
    c = PeCarve(argv[0])
    ided = identify_all(c)
    for s in sorted(ided):
        name, conf = ided[s]
        print("  %#010x  %-26s (%.2f)" % (s, name, conf))
    print("# %d helpers recognized of %d functions" % (len(ided), len(c.funcs)),
          file=sys.stderr)


if __name__ == "__main__":
    main()
