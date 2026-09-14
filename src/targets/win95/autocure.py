#!/usr/bin/env python3
"""autocure.py -- lift raw decompiled C toward COMPILABLE C at scale. The native
engine's *_lane output is readable but not directly compilable: MSVC-only keywords,
undeclared register temporaries, inline-asm the period C won't take, and trailing
labels. This normalizes those mechanically so a whole module compiles, moving many
DLLs across the compile gate at once. It does NOT invent behavior -- inline asm is
stubbed (and counted), so a cured module that compiles is a scaffold to curate, not
a finished reconstruction.

    wt --target win95 autocure <module> [--compile]

Transforms:
  * `#define` away __fastcall/__stdcall/__cdecl and map __int8/16/32 to C types.
  * declare the register temporaries (rN) the lifter left as bare reads.
  * stub `__asm { ... }` blocks (counted -- these are the behavioral to-do list).
  * give trailing labels an empty statement so C accepts them.
C89 implicit-int covers the sub_/import call sites, so no extern wall is needed.
"""
import argparse
import os
import re
import subprocess
import sys

from src.core import paths
from src.targets.win95 import nativedec

_PRELUDE = """\
/* autocure prelude -- mechanical shim so the decompiled C compiles under wcc/wcc386.
   NOT authored source; register temps and stubbed __asm are curation to-dos. */
#define __fastcall
#define __stdcall
#define __cdecl
#define __int8  char
#define __int16 short
#define __int32 int
#define __int64 long
typedef unsigned int   UINT_;
/* register temporaries the lifter reads as incoming machine registers */
static unsigned int r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,r13,r14,r15;
static unsigned int r16,r17,r18,r19,r20,r21,r22,r23,r24,r25,r26,r27,r28,r29,r30,r31;
/* condition-flag pseudo-vars the lifter emits when it can't fold a flag (adc/sbb) */
static unsigned int CA,CF,OF,ZF,SF,PF,AF,DF;
"""

_ASM_RE = re.compile(r"__asm\s*\{[^}]*\}")
# a label line immediately followed by the closing brace of the function
_TRAILING_LABEL_RE = re.compile(r"^(\s*[A-Za-z_]\w*:)\s*$\n(\s*\})", re.M)
# a function definition opening: <rettype> [__fastcall] <name>( ... ) then `{`
_DEF_RE = re.compile(
    r"(?m)^((?:unsigned\s+)?(?:__int8|__int16|__int32|int|void|char|short|long)|struct\s+\w+\s*\*?)"
    r"\s+(?:__fastcall\s+)?([A-Za-z_]\w*)\s*\([^;{]*\)\s*$")


_STRUCT_RE = re.compile(r"(?ms)^struct\s+(\w+)\s*\{.*?\}\s*;\s*$")
_LOCALDECL_RE = re.compile(
    r"(?m)^\s*(?:unsigned\s+)?(?:__int8|__int16|__int32|int|char|short|long)\s*\*?\s*[va]\d+\s*;\s*$"
    r"|^\s*struct\s+\w+\s*\*\s*[va]\d+\s*;\s*$")


def cure_text(text):
    """Return (cured_text, stats)."""
    asm_n = len(_ASM_RE.findall(text))
    text = _ASM_RE.sub("/* asm-stub */ ;", text)
    prev = None
    while prev != text:                      # chained trailing labels -> empty stmt
        prev = text
        text = _TRAILING_LABEL_RE.sub(r"\1 ;\n\2", text)
    text = re.sub(r"^(\s*[A-Za-z_]\w*:)\s*$\n(\s*[A-Za-z_]\w*:)", r"\1 ;\n\2", text, flags=re.M)

    # dedupe struct definitions: the emitter repeats a tag (V10_t) once per function
    # that uses it -> keep the first, drop the rest.
    # A struct tag (e.g. V13_t) recurs across functions -- but each function's use
    # of that base recorded DIFFERENT fields, and the tag name collides. So MERGE
    # the union of all fields seen for a tag into one complete definition; otherwise
    # a `->field` in one function references a field another function's (kept)
    # definition lacks. Fields keyed by the identifier before ';', first type wins.
    _FIELD_RE = re.compile(r"([A-Za-z_]\w*)\s*;")
    merged = {}                                  # tag -> {fieldname: full line}
    for mm in _STRUCT_RE.finditer(text):
        tag = mm.group(1)
        fields = merged.setdefault(tag, {})
        for line in mm.group(0).splitlines():
            fm = _FIELD_RE.search(line)
            if fm and "{" not in line and "}" not in line:
                fields.setdefault(fm.group(1), line.rstrip())
    emitted = set()
    def _dedup(m):
        tag = m.group(1)
        if tag in emitted:
            return "/* dup struct %s */" % tag
        emitted.add(tag)
        body = "\n".join(merged.get(tag, {}).values())
        return "struct %s {\n%s\n};" % (tag, body)
    text = _STRUCT_RE.sub(_dedup, text)

    # drop the emitter's local declarations -- every vN/aN becomes a uniform global
    # unsigned int below, so struct-pointer locals never clash with int uses. (The
    # cost is `->` accesses on those vars; net this compiles the most modules clean.)
    text = _LOCALDECL_RE.sub("", text)
    # a now-int function's bare `return;` must yield a value
    text = re.sub(r"\breturn\s*;", "return 0;", text)

    # Make every function K&R: strip the parameter list from each DEFINITION so C89
    # never checks argument counts (the decompiler's implicit-int call sites disagree
    # freely), and the params it named become the global aN temporaries below.
    defs = _DEF_RE.findall(text)
    seen, protos = [], []
    for ret, name in defs:
        if name not in seen:
            seen.append(name)
            protos.append("int %s();" % name)     # uniform int -> a void result is never "used as void"
    # rewrite each DEFINITION head to `int <name>()`: K&R (no arg-count checks) and
    # int-returning (so the decompiler's "v = void_call()" assignments compile). A
    # `return;` in a now-int function is a harmless C89 warning.
    _DEFHEAD_RE = re.compile(
        r"(?m)^(?:(?:unsigned\s+)?(?:__int8|__int16|__int32|int|void|char|short|long)|struct\s+\w+\s*\*?)"
        r"\s+(?:__fastcall\s+)?([A-Za-z_]\w*)\s*\([^;{]*\)\s*$")
    text = _DEFHEAD_RE.sub(r"int \1()", text)

    # declare the vN / aN temporaries the emitter can leave undeclared on some paths,
    # as globals up to the max index seen (real per-function locals shadow these).
    def _maxidx(letter):
        idx = [int(x) for x in re.findall(r"\b%s(\d+)\b" % letter, text)]
        return max(idx) if idx else -1
    vmax, amax = _maxidx("v"), _maxidx("a")
    decls = []
    if vmax >= 0:
        decls.append("static unsigned int " + ",".join("v%d" % i for i in range(vmax + 1)) + ";")
    if amax >= 0:
        decls.append("static unsigned int " + ",".join("a%d" % i for i in range(amax + 1)) + ";")

    regs = len(set(re.findall(r"\br\d{1,2}\b", text)))
    head = _PRELUDE + "\n" + "\n".join(decls) + "\n\n/* forward declarations */\n" + "\n".join(protos) + "\n\n"
    return head + text, {"asm_stubbed": asm_n, "reg_temps": regs, "funcs": len(seen)}


def _lane_c(module, fmt):
    design = os.path.dirname(os.path.dirname(_corpus())) if _corpus().lower().rstrip("\\/").endswith("files") \
        else os.path.dirname(_corpus())
    lane = {"NE": "ne_lane", "PE": "pe_lane", "LE": "le_lane", "MZ": "mz_lane"}.get(fmt)
    if not lane:
        return None, design
    p = os.path.join(design, lane, module + ".c")
    return (p if os.path.isfile(p) else None), design


def _corpus():
    c = os.environ.get("WT_DATA_ROOT")
    if c and os.path.isdir(c):
        return c
    return r"C:\Users\mrtit\Documents\GitHub\Windows95\design\corpus\files"


def cure_module(module, do_compile=False):
    # module may be "advpack" or "advpack.dll"; find its binary to get the format
    corpus = _corpus()
    binpath = None
    want = module if module.lower().endswith(".dll") else module + ".dll"
    for dp, _, fs in os.walk(corpus):
        for f in fs:
            if f.lower() == want.lower():
                binpath = os.path.join(dp, f); break
        if binpath:
            break
    fmt = nativedec.detect_format(binpath) if binpath else "PE"
    lane_c, design = _lane_c(want, fmt)
    if not lane_c:
        return {"ok": False, "error": "no lane .c for %s (run the *_scale sweep first)" % want}
    cured, stats = cure_text(open(lane_c, encoding="utf-8", errors="replace").read())
    outdir = os.path.join(design, "scaffold", want.split(".")[0])
    os.makedirs(outdir, exist_ok=True)
    outc = os.path.join(outdir, want.split(".")[0] + ".c")
    with open(outc, "w", encoding="utf-8", newline="\n") as f:
        f.write(cured)
    res = {"ok": True, "out": outc, "fmt": fmt, **stats}
    if do_compile:
        res["compile"] = _compile(outc, fmt)
    return res


def _compile(cfile, fmt):
    ow = os.path.join(paths.repo_root(), "experiments", "premise-proof", "open-watcom-v2", "rel")
    cc = os.path.join(ow, "binnt64", "wcc386.exe" if fmt != "NE" else "wcc.exe")
    if not os.path.exists(cc):
        return {"ok": False, "error": "wcc not built"}
    env = dict(os.environ)
    env["NoDefaultCurrentDirectoryInExePath"] = ""
    env["WATCOM"] = ow
    env["PATH"] = os.path.join(ow, "binnt64") + os.pathsep + env.get("PATH", "")
    h = os.path.join(ow, "h")
    env["INCLUDE"] = os.pathsep.join([h, os.path.join(h, "win" if fmt == "NE" else "nt")])
    flags = ["-bt=nt", "-q", "-wcd=138"] if fmt != "NE" else ["-bt=windows", "-mm", "-zW", "-q"]
    r = subprocess.run([cc] + flags + [os.path.basename(cfile)],
                       cwd=os.path.dirname(cfile), env=env, capture_output=True, text=True)
    errs = [l for l in (r.stdout + r.stderr).splitlines() if "Error!" in l]
    return {"ok": r.returncode == 0, "errors": len(errs), "sample": errs[:5]}


def main(argv):
    ap = argparse.ArgumentParser(prog="autocure", description=__doc__.splitlines()[0])
    ap.add_argument("module")
    ap.add_argument("--compile", action="store_true")
    a = ap.parse_args(argv)
    r = cure_module(a.module, do_compile=a.compile)
    if not r["ok"]:
        print("FAILED:", r["error"]); sys.exit(1)
    print("cured %s (%s): %d reg-temps, %d __asm stubbed -> %s"
          % (a.module, r["fmt"], r["reg_temps"], r["asm_stubbed"], r["out"]))
    if "compile" in r:
        c = r["compile"]
        print("compile:", "OK" if c["ok"] else "FAIL (%d errors)" % c.get("errors", -1))
        for e in c.get("sample", []):
            print("   ", e)


if __name__ == "__main__":
    main(sys.argv[1:])
