#!/usr/bin/env python3
r"""triage.py -- origin-triage front-end: identify each function's ORIGIN before reconstruction.

Reconstruction effort (and the coverage metric) should apply ONLY to app/OS-specific code. A lot
of what a module contains was never authored for Windows 95: CRT functions and compiler-generated
helpers (memcpy, __allmul, __ftol, __chkstk, SEH thunks...) ship with / are emitted by the
compiler. This classifies each carved function by origin and reports the corrected denominator:

    CRT / compiler   -- identified by signature against the period MSVC CRT libs (crtsig).
                        NAMED + attributed to the toolchain; NOT a reconstruction target.
    app/OS-specific  -- the rest (classify32 route c/human/asm). THE reconstruction target.

so reconstruction coverage is reported over app-specific functions, not over compiler code.

    wt --target win95 triage <MODULE>
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import pe32util as H
import classify32
from src.engines.symbol import crtsig

# period MSVC static code libraries (CRT + compiler helpers). Import libs carry no code.
_MSVC = r"binaries\toolchains\archive-stash\Microsoft Visual C++ 2.0 (2.00.4244)\MSVC20\LIB"
_STATIC_LIBS = ("LIBC.LIB", "LIBCMT.LIB", "LARGEINT.LIB", "OLDNAMES.LIB")
_DB = None


def _crt_db():
    global _DB
    if _DB is None:
        _DB = {}
        # repo root = .../CodeBreaker (this file is at src/targets/win95/triage.py)
        repo = os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
        for lib in _STATIC_LIBS:
            p = os.path.join(repo, _MSVC, lib)
            if os.path.exists(p):
                try:
                    for sig, nm in crtsig.build(p).items():
                        _DB.setdefault(sig, nm)
                except Exception:
                    pass
        # Win95-EXACT CRT signatures from the shipped crtdll.dll export table (same compiler as
        # the statically-linked CRT copies in the system DLLs -> much higher match).
        try:
            for sig, nm in crtsig.build_from_pe("crtdll.dll").items():
                _DB.setdefault(sig, nm)
        except Exception:
            pass
    return _DB


def triage_module(module):
    rows = classify32.classify_module(module)
    crt = crtsig.match_module(_crt_db(), module)              # {rva: crt/compiler name}
    out = {"module": module, "n": len(rows), "crt": [], "app": []}
    out["crt_bytes"] = out["app_bytes"] = 0
    for r in rows:
        rva, sz = r["rva"], r.get("size", 0) or 0
        if rva in crt:
            out["crt"].append((rva, crt[rva], sz)); out["crt_bytes"] += sz
        else:
            out["app"].append((rva, r.get("route"), sz)); out["app_bytes"] += sz
    return out


def _pct(a, b):
    return 100.0 * a / b if b else 0.0


from src.cli.common import opt as _opt


def main():
    argv = sys.argv[1:]
    if not argv or argv[0] in ("-h", "--help"):
        print(__doc__); return
    from src.core import config
    config.apply_config()
    module = argv[0]
    t = triage_module(module)
    code_b = t["crt_bytes"] + t["app_bytes"]
    print("=== triage %s : %d functions, %d code bytes ===" % (module, t["n"], code_b))
    print("  CRT / compiler (identified, NOT reconstruction targets): %d funcs, %d B (%.1f%%)" %
          (len(t["crt"]), t["crt_bytes"], _pct(t["crt_bytes"], code_b)))
    names = sorted({n for _r, n, _s in t["crt"]})
    if names:
        print("     e.g. %s" % ", ".join(names[:16]))
    print("  app/OS-specific (THE reconstruction target): %d funcs, %d B (%.1f%%)" %
          (len(t["app"]), t["app_bytes"], _pct(t["app_bytes"], code_b)))
    import collections
    rc = collections.Counter(rt for _r, rt, _s in t["app"])
    print("     by authored form: %s" % dict(rc))
    if "--recon" in argv:
        # reconstruction coverage over the APP-SPECIFIC denominator (the correct base)
        from src.engines.behavior import recon
        base = H.image_base(module)
        lim = int(_opt(argv, "--recon") or 30)
        app = [r for r in t["app"] if r[1] in ("c", "human")][:lim]
        rec = 0
        for rva, _rt, _sz in app:
            try:
                if recon.reconstruct_one(module, base + rva).get("method"):
                    rec += 1
            except Exception:
                pass
        print("  reconstruction coverage over app-specific C (%d sampled): %d (%.0f%%)" %
              (len(app), rec, _pct(rec, len(app))))


if __name__ == "__main__":
    main()
