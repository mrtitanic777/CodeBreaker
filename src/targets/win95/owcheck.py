#!/usr/bin/env python3
r"""owcheck.py -- compile authored C with OpenWatcom and score compilability.

Logan's hard rule: OpenWatcom (C) + MASM (asm) are the ONLY tools that may build
anything we author. This module is the C half of that verifier -- it runs the
from-source OpenWatcom compiler (wcc for 16-bit NE/DOS, wcc386 for 32-bit PE) on an
authored .c file and reports whether it compiles, plus a breakdown of the blocking
errors by Watcom code. That error taxonomy is what drives authsrc's compilability
pass (make_compilable): every category we can rewrite mechanically we do at emit
time, and owcheck measures how much of the corpus now actually builds.

    wt --target win95 owcheck --chain          # the boot -> Setup chain files
    wt --target win95 owcheck --all [--limit N] # every authored/c/*.c
    wt --target win95 owcheck <file.c> [--16|--32]

Compilation only (-c): we score that the recovered source is a legal translation
unit Watcom accepts, not that it links -- linking is owbuild's job once a module's
sources all compile.
"""
import os
import re
import sys
import subprocess

from src.core import paths, toolchain


def _ow():
    return toolchain.openwatcom_root()


def _env():
    ow = _ow()
    e = dict(os.environ)
    e["WATCOM"] = ow
    e["INCLUDE"] = os.path.join(ow, "h") + ";" + os.path.join(ow, "h", "nt")
    e["PATH"] = os.path.join(ow, "binnt") + os.pathsep + e.get("PATH", "")
    return e


# Watcom diagnostics: Error! Ennnn, Warning! Wnnn. Warnings never block a -c compile.
_DIAG = re.compile(r"\((\d+)\):\s+(Error|Warning)!\s+([EW]\d+):\s+(.*)")


def compile_c(cpath, bits=16, extra=None):
    """Compile cpath with wcc (bits=16) or wcc386 (bits=32). Returns
    {ok, errors:int, warnings:int, by_code:{code:count}, first:[lines]}."""
    ow = _ow()
    cc = os.path.join(ow, "binnt", "wcc.exe" if bits == 16 else "wcc386.exe")
    tgt = "windows" if bits == 16 else "nt"
    # per-process object file so parallel workers never clobber each other's output
    obj = os.path.join(os.environ.get("TEMP", "/tmp"), "owcheck_%d.o" % os.getpid())
    args = [cc, os.path.abspath(cpath), "-bt=" + tgt, "-q", "-fo=" + obj]
    if bits == 16:
        args[1:1] = ["-mc", "-3"]                           # compact model + 386 CPU (Win16/krnl386)
    if extra:
        args[1:1] = extra
    # hard timeout: a pathological translation unit can send wcc into an unbounded compile, which
    # would stall the whole parallel corpus sweep (one hung worker blocks as_completed forever). Cap
    # it and treat a timeout as "does not compile" so the sweep always makes progress.
    try:
        r = subprocess.run(args, capture_output=True, text=True, env=_env(),
                           cwd=os.path.dirname(os.path.abspath(cpath)) or ".", timeout=90)
    except subprocess.TimeoutExpired:
        # An unbounded compile is the OTHER face of the wcc optimizer pathology (the E1119 ICE is
        # the same bug that here just spins). Retry once with the optimizer off (-od): these units
        # then compile in well under a second. Only if -od also times out is it a genuine hang.
        if not (extra and "-od" in extra):
            return compile_c(cpath, bits, extra=list(extra or []) + ["-od"])
        return {"ok": False, "errors": 1, "warnings": 0,
                "by_code": {"ETIMEOUT": 1}, "first": ["wcc timed out (>90s, even with -od)"]}
    out = (r.stdout or "") + (r.stderr or "")
    errs = warns = 0
    by_code, first = {}, []
    for line in out.splitlines():
        m = _DIAG.search(line)
        if not m:
            continue
        code, sev = m.group(3), m.group(2)
        by_code[code] = by_code.get(code, 0) + 1
        if sev == "Error":
            errs += 1
            if len(first) < 8:
                first.append(line.strip())
        else:
            warns += 1
    # E1119 = a wcc optimizer internal-compiler-error on a specific construct. Retry once with the
    # optimizer disabled (-od): the codegen bug is avoided and the real code still compiles.
    if "E1119" in by_code and not (extra and "-od" in extra):
        return compile_c(cpath, bits, extra=list(extra or []) + ["-od"])
    return {"ok": r.returncode == 0 and errs == 0, "errors": errs,
            "warnings": warns, "by_code": by_code, "first": first}


def _asm_error_lines(cpath, bits):
    """1-based line numbers wcc reports an assembler error (E1156) on -- the lines whose inline asm
    the assembler cannot build."""
    ow = _ow()
    cc = os.path.join(ow, "binnt", "wcc.exe" if bits == 16 else "wcc386.exe")
    args = [cc, os.path.abspath(cpath), "-bt=" + ("windows" if bits == 16 else "nt"), "-q",
            "-fo=" + os.path.join(os.environ.get("TEMP", "/tmp"), "owcheck_%d.o" % os.getpid())]
    if bits == 16:
        args[1:1] = ["-mc", "-3"]
    try:
        r = subprocess.run(args, capture_output=True, text=True, env=_env(),
                           cwd=os.path.dirname(os.path.abspath(cpath)) or ".", timeout=90)
    except subprocess.TimeoutExpired:
        return set()
    out = (r.stdout or "") + (r.stderr or "")
    lns = set()
    for m in re.finditer(r"\((\d+)\):\s+Error!\s+E1156", out):
        lns.add(int(m.group(1)))
    return lns


def neutralize_unassemblable(cpath, bits, max_iter=12):
    """Resolve `__asm{}` blocks the assembler rejects (E1156) by falling back to the instruction's RAW
    BYTES via `db`, never by dropping the instruction. The decoder emits each inline-asm line with a
    `/*=<hexbytes>*/` marker; when wcc cannot spell the mnemonic (an undocumented/rare opcode like
    `salc`, or a form the assembler lacks), rewrite `__asm { <mnem> } /*=BYTES*/` to
    `__asm { db BYTES } /* was: <mnem> */`. The CPU executes the exact bytes either way, so behaviour
    is preserved for real instructions AND faithful for any data that slipped through data-typing.
    Only if the bytes are unavailable is the line commented out as a last resort. Returns (result, n).
    """
    total = 0
    for _ in range(max_iter):
        lns = _asm_error_lines(cpath, bits)
        if not lns:
            break
        lines = open(cpath, encoding="utf-8", errors="replace").read().split("\n")
        changed = False
        for ln in lns:
            k = ln - 1
            if not (0 <= k < len(lines) and "__asm {" in lines[k]):
                continue
            indent = lines[k][:len(lines[k]) - len(lines[k].lstrip())]
            mk = re.search(r"/\*=([0-9A-Fa-fhx,]+)\*/", lines[k])
            mnem = re.search(r"__asm \{([^}]*)\}", lines[k])
            if mk:
                asmtxt = (mnem.group(1).strip() if mnem else "")
                lines[k] = "%s__asm { db %s }   /* was: %s */" % (indent, mk.group(1), asmtxt)
            else:
                lines[k] = "%s/* unassemblable, no bytes: %s */" % (indent, lines[k].strip())
            total += 1; changed = True
        if not changed:
            break
        open(cpath, "w", encoding="utf-8", errors="replace").write("\n".join(lines))
    return compile_c(cpath, bits), total


def _bits_for(cpath):
    """16-bit for the DOS/Win16 classes (their origin), 32-bit for PE. We key off the
    authored file's origin binary format via classify when we can, else default 16."""
    from src.targets.win95 import authsrc
    base = os.path.splitext(os.path.basename(cpath))[0]
    for name, rel, _n in authsrc._CHAIN:
        if name.split(".")[0].lower() == base.lower():
            p = os.path.join(authsrc._dr(), rel)
            if os.path.isfile(p):
                return 32 if authsrc.classify(p) == "PE" else 16
    return 16


def main(argv):
    files, limit, forced = [], None, None
    dr = None
    i = 0
    while i < len(argv):
        a = argv[i]
        if a == "--chain":
            from src.targets.win95 import authsrc
            dr = authsrc._dr()
            for name, _rel, _n in authsrc._CHAIN:
                base = name.split(".")[0].lower()
                cp = os.path.join(dr, "recon", "authored", "c", base + ".c")
                if os.path.isfile(cp):
                    files.append(cp)
        elif a == "--all":
            from src.targets.win95 import authsrc
            cdir = os.path.join(authsrc._dr(), "recon", "authored", "c")
            files = [os.path.join(cdir, f) for f in sorted(os.listdir(cdir)) if f.endswith(".c")]
        elif a == "--limit" and i + 1 < len(argv):
            limit = int(argv[i + 1]); i += 1
        elif a in ("--16", "--32"):
            forced = 16 if a == "--16" else 32
        elif not a.startswith("-"):
            files.append(a)
        i += 1
    if not files:
        print(__doc__); return 0
    if limit:
        files = files[:limit]

    ok_n = 0
    agg = {}
    for cp in files:
        bits = forced or _bits_for(cp)
        r = compile_c(cp, bits)
        if r["ok"]:
            ok_n += 1
            print("  OK   %-16s (%d-bit)  %d warnings" % (os.path.basename(cp), bits, r["warnings"]))
        else:
            top = sorted(r["by_code"].items(), key=lambda x: -x[1])
            top = ", ".join("%s x%d" % (c, n) for c, n in top if c.startswith("E"))[:70]
            print("  FAIL %-16s (%d-bit)  %d errors: %s" % (os.path.basename(cp), bits, r["errors"], top))
            for c, n in r["by_code"].items():
                if c.startswith("E"):
                    agg[c] = agg.get(c, 0) + n
    print("\ncompilable (OpenWatcom -c): %d / %d" % (ok_n, len(files)))
    if agg:
        print("top blocking errors across the set:")
        for c, n in sorted(agg.items(), key=lambda x: -x[1])[:12]:
            print("   %s  x%d" % (c, n))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
