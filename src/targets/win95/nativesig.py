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
import hashlib
import os
import pickle
import re
import subprocess
import tempfile

from src.targets.win95 import nativedec

_HEX = re.compile(r"0x[0-9a-fA-F]+")
_db32 = None
_db32_built = False
_db16 = None
_db16_built = False

# CRT / MFC static libraries carry the toolchain floor. The 16-bit DB is built from
# every OMF C/C++-runtime and MFC lib we have -- across compilers (MS MSC/MSVC and
# OpenWatcom), memory models (S/M/C/L / clib[cshlm]), math packages, and the Windows
# runtime -- so a function is recognized whichever toolchain built it. The 32-bit DB
# adds the i386 COFF C-runtime (msvc-2.0 LIBC/LIBCMT etc.) to the shipped-CRT-DLL
# signatures. Debug variants are excluded (not shipped in the retail corpus).
#   MS/MFC:   *LIBC* / *DLLC* / *AFXC*
#   Watcom:   clib* (C runtime), plib* (C++ runtime), math*
_CRT_LIB_RE = re.compile(r"(LIBC|DLLC|AFXC)|^(CLIB|PLIB|MATH)", re.I)

# Search roots for the 16-bit OMF DB. Watcom is restricted to its 16-bit release tree
# (rel/lib286); its 32-bit libs are ALSO OMF but must not enter the 16-bit DB (they'd
# be masked with 16-bit call rules). MS 16-bit OMF lives under toolchains/.
_OMF_ROOTS = [
    ("toolchains",),
    ("experiments", "premise-proof", "open-watcom-v2", "rel", "lib286"),
]
_COFF_ROOTS = [("toolchains",)]


def _is_debug_lib(fn):
    stem = os.path.splitext(fn)[0].upper()
    return stem.endswith("D")                        # e.g. *CRD / *CWD / *CEWD


def _discover_libs(want_omf):
    """Unique CRT/MFC libs of the requested container (OMF vs COFF), retail only,
    across the roots for that container. Deduped by (name, size)."""
    from src.core import paths
    roots = _OMF_ROOTS if want_omf else _COFF_ROOTS
    seen, out = set(), []
    for parts in roots:
        base = os.path.join(paths.repo_root(), *parts)
        if not os.path.isdir(base):
            continue
        for dp, _, fs in os.walk(base):
            for fn in fs:
                if not fn.lower().endswith(".lib") or not _CRT_LIB_RE.search(fn):
                    continue
                if _is_debug_lib(fn):
                    continue
                p = os.path.join(dp, fn)
                try:
                    with open(p, "rb") as f:
                        is_omf = f.read(1) == b"\xf0"
                    sz = os.path.getsize(p)
                except OSError:
                    continue
                if is_omf != want_omf:
                    continue
                key = (fn.lower(), sz)
                if key in seen:
                    continue
                seen.add(key)
                out.append(p)
    return sorted(out)


def _cache_path(tag, libs):
    h = hashlib.sha1()
    for p in libs:
        try:
            h.update(("%s:%d;" % (os.path.basename(p), os.path.getsize(p))).encode())
        except OSError:
            pass
    return os.path.join(tempfile.gettempdir(), "cb_crtsig_%s_%s.pkl" % (tag, h.hexdigest()[:16]))


def _load_cache(path):
    try:
        with open(path, "rb") as f:
            return pickle.load(f)
    except Exception:
        return None


def _save_cache(path, db):
    try:
        with open(path, "wb") as f:
            pickle.dump(db, f, pickle.HIGHEST_PROTOCOL)
    except Exception:
        pass


def _corpus_root():
    from src.core import corpus
    return corpus.root()


def _find(root, name):
    for dp, _, fs in os.walk(root):
        for fn in fs:
            if fn.lower() == name:
                return os.path.join(dp, fn)
    return None


def crt_db32():
    """32-bit CRT signature DB: shipped Win95 CRT DLLs (crtdll/msvcrt*) + the i386
    COFF C-runtime static libs (msvc-2.0 LIBC/LIBCMT, ...). Cached to disk."""
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
        dlls = [p for p in (_find(root, n) for n in
                            ("crtdll.dll", "msvcrt20.dll", "msvcrt40.dll", "msvcrt.dll")) if p]
        coff_libs = _discover_libs(want_omf=False)
        cache = _cache_path("db32", dlls + coff_libs)
        cached = _load_cache(cache)
        if cached is not None:
            _db32 = cached
            return _db32
        for p in dlls:
            try:
                for sig, nm in crtsig.build_from_pe(p).items():
                    _db32.setdefault(sig, nm)
            except Exception:
                continue
        for p in coff_libs:                          # i386 COFF (non-i386 auto-skipped)
            try:
                for sig, nm in crtsig.build(p).items():
                    _db32.setdefault(sig, nm)
            except Exception:
                continue
        _save_cache(cache, _db32)
    except Exception:
        _db32 = {}
    return _db32


def crt_db16():
    """16-bit CRT/MFC signature DB, built from every retail OMF C-runtime and MFC lib
    under toolchains/ (all models, math packages, and the Windows DLL runtime). Cached
    to disk -- the merged set is large to parse."""
    global _db16, _db16_built
    if _db16_built:
        return _db16
    _db16_built = True
    _db16 = {}
    try:
        from src.engines.symbol import crtsig
        libs = _discover_libs(want_omf=True)
        cache = _cache_path("db16", libs)
        cached = _load_cache(cache)
        if cached is not None:
            _db16 = cached
            return _db16
        for p in libs:
            try:
                for sig, nm in crtsig.build_omf(p).items():
                    _db16.setdefault(sig, nm)
            except Exception:
                continue
        _save_cache(cache, _db16)
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
    """Yield (addr, size, raw_bytes, [(reloc_off,width),...], [insn_text,...]) from an
    engine's --sigdump."""
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
        relocs = []
        if len(f) > 3 and f[3]:
            for pair in f[3].split(","):
                o, _, w = pair.partition(":")
                try:
                    relocs.append((int(o), int(w)))
                except ValueError:
                    pass
        insns = f[4].split("|") if len(f) > 4 and f[4] else []
        yield addr, size, raw, relocs, insns


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
    for addr, size, raw, relocs, insns in _sigdump(path, timeout):
        if db and len(raw) >= crtsig.MIN_SIG:
            if bits == 16:
                sig = crtsig.mask_prefix16(raw, relocs)
            else:
                # 32-bit path masks each reloc's 4-byte span (relative offsets) + E8/E9
                sig = crtsig._pe_masked_prefix(raw, set(o for o, _w in relocs), 0, 0, crtsig.SIG_LEN)
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
