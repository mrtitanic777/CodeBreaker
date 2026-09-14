#!/usr/bin/env python3
"""nativesig.py -- cross-lane CRT / compiler-helper identification for the native
decompiler, so origin-triage can separate the toolchain floor from authored code in
ALL four lanes (NE / PE / LE / MZ), not just the PE lane the symbol-engine matchers
were written for.

The symbol engine's crtsig (FLIRT-style reloc-masked byte-prefix DB) and sigscan
(structural x86-shape predicates) only carve 32-bit PE. The native engine is the one
component that decodes every Win95 executable class, so we bridge through it: each
engine's `--sigdump` emits, per discovered function,

    <addrHex>\t<size>\t<raw prefix bytes hex>\t<insn|insn|...>

and this module applies BOTH matchers to that:

  * crtsig byte-prefix -- precise, names the exact CRT function. Two DBs, chosen by
    the module's bitness: PE/LE match the 32-bit DB built from the shipped Win95 CRT
    DLLs (crtsig.build_from_pe); NE/MZ match the 16-bit DB built from the period MSC
    6.0a / MSVC 1.5x OMF model libraries (crtsig.build_omf), with 16-bit call masking.
  * sigscan structural predicates (crt shapes: byteswap / memcpy / memset / stack
    cookie) -- generic x86, so they fire on every lane.

    from src.targets.win95 import nativesig
    hits = nativesig.identify_module(path)   # {addr:int -> (name, source)}
"""
import os
import re
import subprocess

from src.targets.win95 import nativedec

_HEX = re.compile(r"0x[0-9a-fA-F]+")
_db32 = None
_db32_built = False
_db16 = None
_db16_built = False

# 16-bit CRT model libraries (OMF). Win95's 16-bit modules were built across memory
# models and MSC/MSVC versions, so the DB merges several; a masked prefix is
# model-independent for the shared leaf helpers we care about.
_LIB16 = [
    ("compilers", "msc-6.0a", "LIB", "SLIBCR.LIB"),
    ("compilers", "msc-6.0a", "LIB", "MLIBCR.LIB"),
    ("compilers", "msc-6.0a", "LIB", "CLIBCR.LIB"),
    ("compilers", "msc-6.0a", "LIB", "LLIBCR.LIB"),
    ("compilers", "msvc-1.52c", "LIB", "SLIBCE.LIB"),
    ("compilers", "msvc-1.52c", "LIB", "MLIBCE.LIB"),
    ("compilers", "msvc-1.52c", "LIB", "LLIBCE.LIB"),
]


def _corpus_root():
    c = os.environ.get("WT_DATA_ROOT")
    if c and os.path.isdir(c):
        return c
    return r"C:\Users\mrtit\Documents\GitHub\Windows95\design\corpus\files"


def _find(root, name):
    for dp, _, fs in os.walk(root):
        for fn in fs:
            if fn.lower() == name:
                return os.path.join(dp, fn)
    return None


def crt_db32():
    """Build (once) the 32-bit CRT signature DB from the shipped Win95 CRT DLLs.
    Empty dict if none are present or the symbol engine can't load."""
    global _db32, _db32_built
    if _db32_built:
        return _db32
    _db32_built = True
    _db32 = {}
    try:
        from src.engines.symbol import crtsig
        from src.core import config
        try:
            config.apply_config()
        except Exception:
            pass
        root = _corpus_root()
        for name in ("crtdll.dll", "msvcrt20.dll", "msvcrt40.dll", "msvcrt.dll"):
            p = _find(root, name)
            if not p:
                continue
            try:
                for sig, nm in crtsig.build_from_pe(p).items():
                    _db32.setdefault(sig, nm)
            except Exception:
                continue
    except Exception:
        _db32 = {}
    return _db32


def crt_db16():
    """Build (once) the 16-bit CRT signature DB from the period OMF model libraries.
    Empty dict if none are present."""
    global _db16, _db16_built
    if _db16_built:
        return _db16
    _db16_built = True
    _db16 = {}
    try:
        from src.engines.symbol import crtsig
        from src.core import paths
        root = paths.repo_root()
        for parts in _LIB16:
            p = os.path.join(root, "toolchains", "period", *parts)
            if not os.path.exists(p):
                continue
            try:
                for sig, nm in crtsig.build_omf(p).items():
                    _db16.setdefault(sig, nm)
            except Exception:
                continue
    except Exception:
        _db16 = {}
    return _db16


class _Fn:
    """The minimal view sigscan.SIGNATURES predicates read, built from a --sigdump
    record instead of a PeCarve."""
    def __init__(self, size, prefix_bytes, insn_texts):
        self.size = size
        self.bytes = prefix_bytes
        self.mnem = []
        self.imm = set()
        for t in insn_texts:
            if not t:
                continue
            sp = t.split(" ", 1)
            mn = sp[0]
            op = sp[1] if len(sp) > 1 else ""
            self.mnem.append((mn, op))
            for h in _HEX.findall(op):
                try:
                    self.imm.add(int(h, 16) & 0xFFFFFFFF)
                except ValueError:
                    pass

    def has(self, mnem):
        return any(m == mnem for m, _ in self.mnem)


def _sigscan_name(fn):
    """Best sigscan structural name for a function view, or None."""
    from src.engines.symbol import sigscan
    best = None
    for name, conf, pred in sigscan.SIGNATURES:
        try:
            if pred(fn) and (best is None or conf > best[1]):
                best = (name, conf)
        except Exception:
            pass
    return best[0] if best else None


def _sigdump(path, timeout):
    """Yield (addr, size, raw_bytes, [insn_text,...]) from an engine's --sigdump."""
    exe, _mod, _dis, _names = nativedec._engine(path)
    r = subprocess.run([exe, path, "--sigdump"], capture_output=True, text=True, timeout=timeout)
    for line in r.stdout.splitlines():
        f = line.split("\t")
        if len(f) < 3:
            continue
        try:
            addr = int(f[0], 16)
            size = int(f[1])
        except ValueError:
            continue
        raw = bytes.fromhex(f[2]) if f[2] else b""
        insns = f[3].split("|") if len(f) > 3 and f[3] else []
        yield addr, size, raw, insns


def identify_module(path, timeout=300):
    """Identify CRT/compiler functions in a module. Returns {addr:int -> (name, src)}
    where src is 'crtsig' (byte-prefix, precise) or 'sigscan' (structural shape).

    The byte-prefix matcher is chosen by the module's bitness: PE/LE use the 32-bit
    CRT DLL DB with 32-bit call masking; NE/MZ use the 16-bit OMF-lib DB with 16-bit
    call masking. sigscan structural shapes run on every lane."""
    from src.engines.symbol import crtsig
    fmt = nativedec.detect_format(path)
    if fmt in ("NE", "MZ"):
        db, bits = crt_db16(), 16
    else:
        db, bits = crt_db32(), 32
    out = {}
    for addr, size, raw, insns in _sigdump(path, timeout):
        if db and len(raw) >= crtsig.MIN_SIG:
            if bits == 16:
                sig = crtsig.mask_prefix16(raw)
            else:
                sig = crtsig._pe_masked_prefix(raw, set(), 0, 0, crtsig.SIG_LEN)
            nm = db.get(sig) if sig else None
            if nm:
                out[addr] = (nm, "crtsig")
                continue
        # sigscan structural (all lanes)
        nm = _sigscan_name(_Fn(size, raw, insns))
        if nm:
            out[addr] = (nm, "sigscan")
    return out


if __name__ == "__main__":
    import sys
    if len(sys.argv) < 2:
        print("usage: nativesig.py <module>"); raise SystemExit(2)
    from src.core import config
    try:
        config.apply_config()
    except Exception:
        pass
    hits = identify_module(sys.argv[1])
    for a in sorted(hits):
        nm, src = hits[a]
        print("  sub_%06X  %-24s (%s)" % (a, nm, src))
    print("# %d CRT/compiler functions identified" % len(hits))
