"""python -m codebreaker.port SOURCE_DIR OUT_DIR [--compiler gcc|clang|msvc] [--runtime NAME] [--cc PATH]

SOURCE_DIR is read, never written: its C (*.c, *.h, in it or its src/) and its assembler modules (*.asm, in it or
its asm/). OUT_DIR gets the port: the modern sources, the platform layer, a build script, the program built, and
PORT.md - every change made and why, what the compiler still says, what could not be ported.
"""
import collections
import glob
import json
import os
import re
import shutil
import subprocess
import sys

from codebreaker.port import early_c

HERE = os.path.dirname(os.path.abspath(__file__))


def load_compilers():
    c = json.load(open(os.path.join(HERE, "compilers.json")))
    try:                                                    # (the user's own, from settings.json)
        s = json.load(open(os.path.join(HERE, "..", "..", "settings.json")))
        for k, v in s.get("port", {}).get("compilers", {}).items():
            c.setdefault(k, {}).update(v)
    except (OSError, ValueError):
        pass
    return c


def find(names, given=None):
    for c in ([given] if given else []) + names:
        p = shutil.which(c) or next(iter(sorted(glob.glob(c), reverse=True)), None)
        if p:
            return p
    return None


def env_for(cc):
    e = dict(os.environ)                                    # (a compiler's own DLLs beside it, on the path)
    e["PATH"] = os.path.dirname(cc) + os.pathsep + e.get("PATH", "")
    return e


def run(spec, argv, cwd, cc):
    """a compiler's command, after its setup script (MSVC's vcvars) in the same shell"""
    setup = find(spec.get("setup", [])) if spec.get("setup") else None
    if setup:
        argv = ["cmd", "/d", "/c", "call", setup, ">nul", "&&"] + argv
    return subprocess.run(argv, capture_output=True, text=True, env=env_for(cc), cwd=cwd, errors="replace")


def args(tpl, **kw):
    out = []
    for a in tpl:
        if a == "{srcs}":
            out += kw["srcs"]
        else:
            out.append(a.format(**{k: v for k, v in kw.items() if k != "srcs"}))
    return out


def diagnostics(spec, text, folder):
    out = []
    for l in text.splitlines():
        m = re.match(spec["diag"], l.strip())
        if m:
            f = os.path.basename(m.group("file"))
            out.append({"file": f, "line": int(m.group("line")), "col": int(m.group("col") or 0),
                        "kind": m.group("kind"), "text": m.group("text")})
    return out


def read_tree(src):
    files, asm = {}, {}
    for d in (src, os.path.join(src, "src")):
        for p in sorted(glob.glob(os.path.join(d, "*.[ch]"))):
            files[os.path.basename(p)] = open(p, encoding="latin1").read()
    for d in (src, os.path.join(src, "asm")):
        for p in sorted(glob.glob(os.path.join(d, "*.asm"))):
            asm[os.path.basename(p)] = open(p, encoding="latin1").read()
    return files, asm


def asm_role(text, profile):
    """what an assembler module of the program is: the old runtime's (each name it makes public is the C library's
    or the platform layer's, or the runtime's own), data alone, or code of the program to be ported"""
    pubs = set()
    for l in text.splitlines():
        m = re.match(r"\s*PUBLIC\s+(.+)", l, re.I)
        if m:
            pubs |= {x.strip() for x in m.group(1).split(",") if x.strip()}
    procs = re.findall(r"(?im)^\s*(\w+)\s+PROC\b", text)
    known = set(profile["libc"]) | set(profile["platform"]["calls"]) | set(profile["runtime_defines"])
    if not procs:
        return "data", sorted(pubs)
    own = [p for p in pubs if re.match(r"_[A-Za-z]", p) and p[1:] not in known]   # (C names: _x is C's x;
    return ("code" if own else "runtime"), sorted(pubs)                          #  __x and cret the runtime's)


def main(argv):
    if len(argv) < 2:
        print(__doc__)
        return 2
    src, out = argv[0], argv[1]
    opt = lambda k, d=None: argv[argv.index(k) + 1] if k in argv else d
    comp = opt("--compiler", "gcc")
    profile = json.load(open(os.path.join(HERE, "runtimes", opt("--runtime", "dos_c_early") + ".json")))
    compilers = load_compilers()
    spec = compilers[comp]
    cc = find(spec["find"], opt("--cc"))
    if os.path.abspath(out).startswith(os.path.abspath(src) + os.sep) or os.path.abspath(out) == os.path.abspath(src):
        print("the port is written beside the source, never into it: give another OUT_DIR")
        return 2
    files, asm = read_tree(src)
    port = early_c.Port(files, profile)
    roles = {n: asm_role(t, profile) for n, t in asm.items()}
    os.makedirs(out, exist_ok=True)
    for n in ("dosrt.h", "dosrt.c"):
        shutil.copy(os.path.join(HERE, "platform", n), os.path.join(out, n))

    def write():
        res = port.modernize()
        for n, t in port.headers().items():
            if t is not None:
                res[n] = t
        for n, t in res.items():
            open(os.path.join(out, n), "w", encoding="latin1", newline="\n").write(t)
        return res

    rounds, diags = 0, []
    while True:
        res = write()
        diags = []
        if cc is None:
            break
        for n in sorted(res):
            if n.endswith(".c"):
                p = run(spec, args(spec["check"], cc=cc, inc=".", src=n), out, cc)
                diags += diagnostics(spec, p.stdout + p.stderr, out)
        rounds += 1
        if rounds > 8 or not early_c.mend(port, res, diags):
            break
    srcs = sorted(n for n in res if n.endswith(".c")) + ["dosrt.c"]
    exe = os.path.join(out, "program.exe" if os.name == "nt" else "program")
    cases = opt("--cases")
    sanitized = []
    if cases:                                               # (what only a run shows: the sanitizer, the cases)
        sanitized = sanitize_loop(port, out, write, srcs, compilers, comp, cases)
        res = write()
    built, blog = False, ""
    if cc:
        p = run(spec, args(spec["build"], cc=cc, inc=".", out=exe, srcs=srcs), out, cc)
        built, blog = p.returncode == 0, p.stdout + p.stderr
    with open(os.path.join(out, "build.sh"), "w", newline="\n") as f:
        f.write("#!/bin/sh\n# the port built by %s (made by CodeBreaker's port)\ncd \"$(dirname \"$0\")\"\n%s\n" %
                (comp, " ".join(args(spec["build"], cc=os.path.basename(cc or comp), inc=".", out=os.path.basename(exe),
                                     srcs=srcs))))
    report(out, src, comp, cc, port, roles, diags, rounds, built, blog, profile, sanitized)
    errs = sum(1 for d in diags if d["kind"] == "error")
    print("%s: %d files ported, %d left out (the old runtime), %d rounds; %d errors, %d warnings; %s" % (
        out, len([n for n in res if n.endswith(".c")]), len(port.left_out), rounds, errs,
        sum(1 for d in diags if d["kind"] == "warning"), "BUILT " + exe if built else "not built"))
    return 0 if built else 1


def sanitize_loop(port, out, write, srcs, compilers, comp, cases, rounds=6):
    """the port built with AddressSanitizer and run on the cases until it reports nothing; -> the rounds' notes"""
    from codebreaker.port import sanitize, verify
    for name in ([comp] if comp != "msvc" else []) + ["clang", "gcc"]:
        spec = compilers.get(name)
        cc = find(spec["find"]) if spec else None
        if cc and (name != "gcc" or os.name != "nt"):       # (MinGW's gcc has no sanitizer runtime)
            break
    else:
        return ["no compiler with AddressSanitizer found: the cases were not run sanitized"]
    rt = sanitize.runtime_dir(cc)
    d = os.path.join(out, ".sanitize")
    os.makedirs(d, exist_ok=True)
    exe = os.path.join(d, "program.exe" if os.name == "nt" else "program")
    notes = []
    for r in range(rounds):
        write()
        tpl = [a for a in spec["build"] if a not in ("-O1", "/O1")]
        tpl = tpl[:1] + ["-g", "-fsanitize=address"] + tpl[1:]
        p = run(spec, args(tpl, cc=cc, inc=".", out=exe, srcs=srcs), out, cc)
        if p.returncode:
            notes.append("the sanitized build failed: %s" % (p.stdout + p.stderr)[-300:])
            break
        env = {"PATH": (rt + os.pathsep if rt else "") + os.environ.get("PATH", ""), "ASAN_OPTIONS": "detect_leaks=0"}
        reports = verify.run_cases(cases, exe, env)
        done = sanitize.mend(port, port.out, reports)
        bad = [c for c, t in reports if "AddressSanitizer" in t]
        if not done:
            notes.append("round %d: %d cases run sanitized, %s" % (
                r + 1, len(reports), "no fault" if not bad else "faults not mended: " + ", ".join(bad)))
            break
        notes += ["round %d: %s" % (r + 1, why) for v, why in done]
    import shutil as _sh
    _sh.rmtree(d, ignore_errors=True)
    return notes


def report(out, src, comp, cc, port, roles, diags, rounds, built, blog, profile, sanitized=()):
    L = ["# Port of `%s`" % os.path.basename(os.path.normpath(src)), "",
         "Made by CodeBreaker's port from `%s` (read only), for **%s** (`%s`)." % (src, comp, cc or "not found"), "",
         "**%s.** %d rounds of the compiler's complaints mended." % ("Built" if built else "Not built", rounds), ""]
    L += ["## Left out: the old runtime", "", "| file | why |", "|---|---|"]
    L += ["| %s | %s |" % (n, w) for n, w in sorted(port.left_out.items())]
    for n, (role, pubs) in sorted(roles.items()):
        why = {"runtime": "assembler routines of the old runtime (%s): the C library and the platform layer stand in",
               "data": "data alone (%s)",
               "code": "**the program's own assembler (%s): not ported yet - the port is incomplete**"}[role]
        L.append("| %s | %s |" % (n, why % (", ".join(pubs) or "nothing public")))
    L += ["", "## Changes", "", "| change | how many | where (first) |", "|---|---|---|"]
    for k, v in sorted(port.notes.items(), key=lambda kv: -len(kv[1])):
        L.append("| %s | %d | %s |" % (k, len(v), ", ".join(v[:4]) + (" ..." if len(v) > 4 else "")))
    if sanitized:
        L += ["", "## Run under AddressSanitizer (the verify cases)", ""] + ["- %s" % s for s in sanitized]
    L += ["", "## Behaviour that may differ", ""]
    L += ["- **%s**: %s" % (k, v) for k, v in profile["notes"].items()]
    L += ["- **int** is %d bits in the original and 32 here: arithmetic that relied on 16-bit wrap-around differs "
          "(structure members keep 16 bits); the verifier (verify.py) is what shows it." % profile["widths"]["int"]]
    if profile.get("char_signed"):
        L.append("- **char** was signed: built with -fsigned-char where the compiler has it.")
    left = [d for d in diags if d["kind"] in ("error", "warning")]
    L += ["", "## What the compiler still says (%d)" % len(left), ""]
    cnt = collections.Counter(re.sub(r"'[^']*'", "'..'", d["text"]) for d in left)
    L += ["- %d x %s" % (n, t) for t, n in cnt.most_common(30)]
    if not built and blog:
        L += ["", "## Build", "", "```", blog[-3000:], "```"]
    open(os.path.join(out, "PORT.md"), "w", encoding="utf-8").write("\n".join(L) + "\n")


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
