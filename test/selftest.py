#!/usr/bin/env python3
r"""CodeBreaker self-test -- one executable that exercises every subsystem.

Built as a single EXE (test/CodeBreakerSelfTest.exe) it does double duty:

  * RUN it  -> it checks every subsystem (foundation, format loaders, IR engine,
    disassembly, the compiler-inverse engine, the Studio pipeline, cross-version diff)
    and prints a PASS/FAIL report.
  * DROP it into CodeBreaker Studio -> it is itself a real Windows PE, so the Studio
    reverse-engineers it end-to-end (format -> structure -> disassembly -> output folder).

Nothing here needs external data or the period compilers; the one step that would (the
inverse engine's byte-exact *verify*) is reported as SKIPPED when its oracle isn't set.
"""
import os
import sys
import tempfile
import traceback

# Make the engine package importable both frozen (bundled) and from a source checkout.
if getattr(sys, "frozen", False):
    _ROOT = sys._MEIPASS
else:
    _ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if _ROOT not in sys.path:
    sys.path.insert(0, _ROOT)

RESULTS = []   # (subsystem, status, detail)   status in {"PASS","FAIL","SKIP"}


def check(name, fn):
    try:
        detail = fn()
        RESULTS.append((name, "PASS", detail or ""))
    except _Skip as s:
        RESULTS.append((name, "SKIP", str(s)))
    except Exception as e:
        RESULTS.append((name, "FAIL", "%s: %s" % (type(e).__name__, e)))
        if os.environ.get("CB_SELFTEST_TRACE"):
            traceback.print_exc()


class _Skip(Exception):
    pass


def _sample_pe():
    """A real PE to feed the format loaders / pipeline: the running executable itself."""
    return sys.executable


# ---- the subsystem checks -------------------------------------------------------------

def t_foundation():
    from engine.foundation import layout, config, cores, parallel, progress  # noqa: F401
    config.load_config()                       # must not raise
    return "layout, config, cores, parallel, progress"


def t_format_loaders():
    from engine.format_loaders import pe, ne, objextract  # noqa: F401
    P = pe.PEFile(_sample_pe())
    secs = getattr(P, "sections", [])
    if not secs:
        raise RuntimeError("PE parsed but no sections found")
    return "PE parse ok (%d sections in the test binary)" % len(secs)


def t_ir_engine():
    from engine.ir_engine import il
    # il is the IR core; confirm it exposes the lifting surface.
    if not any(hasattr(il, a) for a in ("lift", "Program", "Function", "emit_c", "decompile")):
        raise RuntimeError("il module missing its expected lifting surface")
    return "IR engine (il) import ok"


def t_disasm_engine():
    import capstone
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    n = len(list(md.disasm(b"\x55\x8b\xec\x5d\xc3", 0)))   # push ebp; mov ebp,esp; pop ebp; ret
    if n < 4:
        raise RuntimeError("capstone decoded %d/4 instructions" % n)
    return "capstone %s (x86 decode ok)" % capstone.__version__


def t_inverse_engine():
    from engine.inverse_engine import framemodel, mc2inv, mc2model, raiser16, oracle16  # noqa: F401
    ok = framemodel._self_test()               # prints "framemodel self-test: N/N ..."
    if not ok:
        raise RuntimeError("frame-allocator self-test did not reach 25/25")
    note = "frame allocator 25/25"
    if not oracle16.available():
        note += "  (byte-exact verify SKIPPED: MC1/MC2 oracle not configured)"
    return note


def t_studio_pipeline():
    from engine.studio import pipeline
    out = os.path.join(tempfile.gettempdir(), "cb_selftest_out")
    rep = pipeline.reverse(_sample_pe(), outbase=out)
    if rep.get("error"):
        raise RuntimeError(rep["error"])
    arts = rep.get("artifacts") or []
    if not rep.get("outdir") or len(arts) < 3:
        raise RuntimeError("pipeline produced no output folder / artifacts")
    return "%s -> %d artifacts in an output folder" % (rep.get("format", "?"), len(arts))


def t_studio_ui():
    from engine.studio import app  # noqa: F401  (import only; no window created)
    return "native Studio module import ok"


def t_diff():
    from engine.diff import cli  # noqa: F401
    return "cross-version diff CLI import ok"


CHECKS = [
    ("Foundation",            t_foundation),
    ("Format loaders",        t_format_loaders),
    ("IR engine",             t_ir_engine),
    ("Disassembly",           t_disasm_engine),
    ("Inverse engine",        t_inverse_engine),
    ("Studio pipeline",       t_studio_pipeline),
    ("Studio (native UI)",    t_studio_ui),
    ("Cross-version diff",    t_diff),
]


def main():
    print("=" * 64)
    print(" CodeBreaker self-test")
    print(" %s" % ("frozen EXE" if getattr(sys, "frozen", False) else "source checkout"))
    print("=" * 64)
    for name, fn in CHECKS:
        check(name, fn)
    print()
    for name, status, detail in RESULTS:
        print("  [%-4s]  %-20s %s" % (status, name, detail))
    print()
    npass = sum(1 for _, s, _ in RESULTS if s == "PASS")
    nfail = sum(1 for _, s, _ in RESULTS if s == "FAIL")
    nskip = sum(1 for _, s, _ in RESULTS if s == "SKIP")
    print("  %d passed, %d failed, %d skipped  (of %d subsystems)"
          % (npass, nfail, nskip, len(RESULTS)))
    print("=" * 64)
    if getattr(sys, "frozen", False):          # keep the console open on double-click
        try:
            input("\nPress Enter to close...")
        except EOFError:
            pass
    return 1 if nfail else 0


if __name__ == "__main__":
    sys.exit(main())
