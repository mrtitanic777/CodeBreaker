"""verify.py -- the original and its port run on the same inputs; what each shows and writes compared.

    python -m codebreaker.port.verify CASES.json [--report OUT.md]

The port is right when it does what the original did. The original runs under CodeBreaker's own emulator (DOS:
exact/emu/dos.py - nothing of it runs on the host); the port runs natively. Each case runs both in folders of their
own with the same input files, the same command line, the same keyboard input; then the console output (byte for
byte), the exit code and every file left in the folder are compared.

CASES.json (paths relative to it):
    {"original": "LIB.EXE", "port": "../modern/program.exe", "system": "dos",
     "inputs": {"HELLO.OBJ": "path", ...},                  the files every case may use
     "env": {"DOSRT_CORE_PAGES": "1103"},                   for the port (the platform layer's settings)
     "cases": [{"name": "make", "cmd": "NEW +HELLO;", "stdin": "", "files": ["HELLO.OBJ"],
                "then": [...more cases run in the same folders after it...]}]}
"""
import difflib
import json
import os
import shutil
import subprocess
import sys
import tempfile


def run_original(cfg, exe, folder, cmd, stdin):
    from codebreaker.exact.emu import dos
    try:
        r = dos.run(exe, folder, cmdline=(" " + cmd) if cmd else "", stdin=stdin.encode("latin1"), fast=True)
    except RuntimeError as e:                               # (a run that does not end: what it showed, and that)
        return str(e).encode("latin1", "replace"), "did not end"
    return r.console, r.exit


def run_port(cfg, exe, folder, cmd, stdin, env):
    e = dict(os.environ)
    e.update(env)
    p = subprocess.run([exe] + cmd.split(), cwd=folder, input=stdin.encode("latin1"), stdout=subprocess.PIPE,
                       stderr=subprocess.STDOUT, env=e, timeout=60)
    return p.stdout, p.returncode


def run_cases(path, exe, env=None):
    """every case of CASES.json run by exe alone (the original not run): [(case name, its console)] - what the
    sanitizer pass reads"""
    base = os.path.dirname(os.path.abspath(path))
    cfg = json.load(open(path))
    at = lambda p: p if os.path.isabs(p) else os.path.join(base, p)
    inputs = {k: at(v) for k, v in cfg.get("inputs", {}).items()}
    e = dict(cfg.get("env", {}))
    e.update(env or {})
    out = []

    def one(case, fb):
        for n in case.get("files", []):
            shutil.copy(inputs[n], os.path.join(fb, n))
        for n, text in case.get("make", {}).items():
            open(os.path.join(fb, n), "wb").write(text.encode("latin1"))
        try:
            c, x = run_port(cfg, exe, fb, case.get("cmd", ""), case.get("stdin", ""), e)
        except subprocess.TimeoutExpired:
            c = b"(did not end)"
        out.append((case["name"], c.decode("latin1")))
        for c2 in case.get("then", []):
            one(c2, fb)
    for case in cfg["cases"]:
        fb = tempfile.mkdtemp(prefix="cbs_")
        try:
            one(case, fb)
        finally:
            shutil.rmtree(fb, ignore_errors=True)
    return out


def files_of(folder):
    out = {}
    for n in sorted(os.listdir(folder)):
        p = os.path.join(folder, n)
        if os.path.isfile(p):
            out[n.upper()] = open(p, "rb").read()
    return out


def show(b):
    return b.decode("latin1").replace("\r\n", "\n").splitlines()


def main(argv):
    path = argv[0]
    base = os.path.dirname(os.path.abspath(path))
    cfg = json.load(open(path))
    at = lambda p: p if os.path.isabs(p) else os.path.join(base, p)
    orig = at(cfg["original"])
    port = os.path.abspath(argv[argv.index("--port") + 1]) if "--port" in argv else at(cfg["port"])
    if not os.path.isfile(port):
        print("the port is not built: %s" % port)
        return 2
    inputs = {k: at(v) for k, v in cfg.get("inputs", {}).items()}
    rows, detail = [], []

    def one(case, fa, fb):
        for n in case.get("files", []):
            for f in (fa, fb):
                shutil.copy(inputs[n], os.path.join(f, n))
        for n, text in case.get("make", {}).items():        # (small inputs written out: a response file)
            for f in (fa, fb):
                open(os.path.join(f, n), "wb").write(text.encode("latin1"))
        cmd, stdin = case.get("cmd", ""), case.get("stdin", "")
        ca, xa = run_original(cfg, orig, fa, cmd, stdin)
        cb, xb = run_port(cfg, port, fb, cmd, stdin, cfg.get("env", {}))
        A, B = files_of(fa), files_of(fb)
        bad = []
        if ca != cb:
            bad.append("console")
            detail.append("### %s: console\n\n```diff\n%s\n```\n" % (case["name"], "\n".join(
                difflib.unified_diff(show(ca), show(cb), "original", "port", lineterm="", n=2))))
        if (xa or 0) != (xb or 0):
            bad.append("exit %s / %s" % (xa, xb))
        for n in sorted(set(A) | set(B)):
            if A.get(n) != B.get(n):
                what = "only the original's" if n not in B else "only the port's" if n not in A else \
                       "differs (%d / %d bytes, first at %d)" % (len(A[n]), len(B[n]), next(
                           (i for i, (x, y) in enumerate(zip(A[n], B[n])) if x != y), min(len(A[n]), len(B[n]))))
                bad.append("%s %s" % (n, what))
        given = [x.upper() for x in case.get("files", []) + list(case.get("make", {}))]
        made = sorted(n for n in A if n not in given)
        rows.append((case["name"], cmd, not bad, ", ".join(bad) or "same: console%s" % (
            ", " + ", ".join(made) if made else "")))
        for c in case.get("then", []):
            one(c, fa, fb)

    for case in cfg["cases"]:
        fa, fb = tempfile.mkdtemp(prefix="cbv_o_"), tempfile.mkdtemp(prefix="cbv_p_")
        try:
            one(case, fa, fb)
        finally:
            shutil.rmtree(fa, ignore_errors=True)
            shutil.rmtree(fb, ignore_errors=True)
    same = sum(1 for r in rows if r[2])
    L = ["# Behaviour: the original against the port", "",
         "`%s` (under CodeBreaker's emulator) against `%s` (native): **%d of %d cases the same**." % (
             os.path.basename(orig), os.path.relpath(port, base), same, len(rows)), "",
         "| case | command | | what |", "|---|---|---|---|"]
    L += ["| %s | `%s` | %s | %s |" % (n, c or "(none)", "same" if ok else "**DIFFERS**", w) for n, c, ok, w in rows]
    if detail:
        L += ["", "## Differences", ""] + detail
    text = "\n".join(L) + "\n"
    rep = argv[argv.index("--report") + 1] if "--report" in argv else None
    if rep:
        open(rep, "w", encoding="utf-8").write(text)
    print(text if not rep else "%d of %d cases the same (%s)" % (same, len(rows), rep))
    return 0 if same == len(rows) else 1


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
