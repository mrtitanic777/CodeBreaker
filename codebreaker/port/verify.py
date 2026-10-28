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
    "programs": {"mc2": {"original": "MC2.EXE", "port": "../../mc2/modern/program.exe"}}
                                                            another program of the same set (a compiler's next
                                                            pass): a case names it, "program": "mc2"
    a case's "ignore": ["*.Q"]                              files not compared (what the original left
                                                            unwritten - its memory as it was - and the next pass
                                                            does not read)
"""
import fnmatch
import difflib
import functools
import re
import json
import os
import shutil
import subprocess
import sys
import tempfile


@functools.lru_cache(maxsize=None)
def _emulator_key():
    """what the emulator is: its sources (a change to it runs the originals again)"""
    import hashlib
    from codebreaker.exact.emu import dos
    h = hashlib.sha256()
    d = os.path.dirname(os.path.abspath(dos.__file__))
    for n in sorted(os.listdir(d)):
        if n.endswith(".py"):
            h.update(n.encode() + open(os.path.join(d, n), "rb").read())
    return h.hexdigest()


@functools.lru_cache(maxsize=None)
def _file_hash(path, mtime, size):
    import hashlib
    return hashlib.sha256(open(path, "rb").read()).hexdigest()


def _original_key(exe, folder, cmd, stdin):
    """the original's run, by everything it depends on: the program, the emulator, the command, the input and
    every file it is given"""
    import hashlib
    h = hashlib.sha256()
    st = os.stat(exe)
    h.update(_file_hash(os.path.abspath(exe), st.st_mtime, st.st_size).encode())
    h.update(_emulator_key().encode())
    h.update(repr((cmd, stdin)).encode("latin1", "replace"))
    for n in sorted(os.listdir(folder)):
        f = os.path.join(folder, n)
        if os.path.isfile(f):
            h.update(n.encode("latin1", "replace") + b"\0" + open(f, "rb").read() + b"\0")
    return h.hexdigest()


def run_original(cfg, exe, folder, cmd, stdin):
    """the original run under the emulator - once: what it showed and left, by what it was given, kept (it is the
    same every time); -> (console, exit)"""
    from codebreaker.base import cache
    key = _original_key(exe, folder, cmd, stdin)
    got = cache.value("port-originals", key)
    if got is not None:
        try:
            for n in os.listdir(folder):                    # (the folder as the run left it)
                if os.path.isfile(os.path.join(folder, n)):
                    os.remove(os.path.join(folder, n))
            for n, data in got["files"].items():
                with open(os.path.join(folder, n), "wb") as f:
                    f.write(data)
            return got["console"], got["exit"]
        except (OSError, KeyError):
            pass
    from codebreaker.exact.emu import dos
    from codebreaker.base import parallel
    parallel.wait_for_memory("verify")
    try:
        r = dos.run(exe, folder, cmdline=(" " + cmd) if cmd else "", stdin=stdin.encode("latin1"), fast=True)
        console, code = r.console, r.exit
    except RuntimeError as e:                               # (a run that does not end: what it showed, and that)
        console, code = str(e).encode("latin1", "replace"), "did not end"
    try:
        files = {n: open(os.path.join(folder, n), "rb").read()
                 for n in os.listdir(folder) if os.path.isfile(os.path.join(folder, n))}
        cache.keep("port-originals", key, {"console": console, "exit": code, "files": files})
    except OSError:
        pass
    return console, code


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
    def one(case, fb, out):
        for n in case.get("files", []):
            shutil.copy(inputs[n], os.path.join(fb, n))
        for n, text in case.get("make", {}).items():
            open(os.path.join(fb, n), "wb").write(text.encode("latin1"))
        prog = cfg.get("programs", {}).get(case.get("program"))
        if prog:                                            # (another pass: its own port)
            exe2 = at(prog["port"])
            if not os.path.isfile(exe2):
                return
        try:
            c, x = run_port(cfg, at(prog["port"]) if prog else exe, fb, case.get("cmd", ""), case.get("stdin", ""), e)
        except subprocess.TimeoutExpired as ex:             # (what it said before it was stopped: the sanitizer's
            c = (ex.output or b"")[-200000:] + b"\n(did not end)"   #  reports among it)
        out.append((case["name"], c.decode("latin1")))
        for c2 in case.get("then", []):
            one(c2, fb, out)

    def top(case):
        o = []
        fb = tempfile.mkdtemp(prefix="cbs_")
        try:
            one(case, fb, o)
        finally:
            shutil.rmtree(fb, ignore_errors=True)
        return o
    from codebreaker.jobs import pool
    out = []
    for o in pool.threads(top, cfg["cases"]):                   # (the cases at once: the machine's budget)
        out += o
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
    from codebreaker.base import parallel
    parallel.throttle(below_normal=True)
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

    def one(case, fa, fb, rows, detail):
        for n in case.get("files", []):
            for f in (fa, fb):
                shutil.copy(inputs[n], os.path.join(f, n))
        for n, text in case.get("make", {}).items():        # (small inputs written out: a response file)
            for f in (fa, fb):
                open(os.path.join(f, n), "wb").write(text.encode("latin1"))
        cmd, stdin = case.get("cmd", ""), case.get("stdin", "")
        prog = cfg.get("programs", {}).get(case.get("program"))
        oa, pb = (at(prog["original"]), at(prog["port"])) if prog else (orig, port)
        ca, xa = run_original(cfg, oa, fa, cmd, stdin)
        try:
            cb, xb = run_port(cfg, pb, fb, cmd, stdin, cfg.get("env", {}))
        except subprocess.TimeoutExpired:                   # (a port that does not end: so recorded)
            cb, xb = b"(did not end)", "did not end"
        A, B = files_of(fa), files_of(fb)
        vol = [re.compile(v.encode("latin1")) for v in cfg.get("volatile", [])]
        if vol:                                             # (what depends on the machine - free memory - masked
            mask = lambda b: functools.reduce(lambda x, rx: rx.sub(b"<varies>", x), vol, b)   #  alike in both)
            ca, cb = mask(ca), mask(cb)
            A = {n: (mask(v) if n.endswith((".LST", ".MAP", ".TXT")) else v) for n, v in A.items()}
            B = {n: (mask(v) if n.endswith((".LST", ".MAP", ".TXT")) else v) for n, v in B.items()}
        bad = []
        if ca != cb:
            bad.append("console")
            detail.append("### %s: console\n\n```diff\n%s\n```\n" % (case["name"], "\n".join(
                difflib.unified_diff(show(ca), show(cb), "original", "port", lineterm="", n=2))))
        if (xa or 0) != (xb or 0):
            bad.append("exit %s / %s" % (xa, xb))
        for n in sorted(set(A) | set(B)):
            if any(fnmatch.fnmatch(n, g.upper()) for g in case.get("ignore", [])):
                continue                                    # (not compared: the case says so)
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
            one(c, fa, fb, rows, detail)

    def top(case):                                          # (a case and those run after it in its folders)
        r, d = [], []
        fa, fb = tempfile.mkdtemp(prefix="cbv_o_"), tempfile.mkdtemp(prefix="cbv_p_")
        try:
            one(case, fa, fb, r, d)
        finally:
            shutil.rmtree(fa, ignore_errors=True)
            shutil.rmtree(fb, ignore_errors=True)
        return r, d
    from codebreaker.jobs import pool
    for r, d in pool.threads(top, cfg["cases"]):                # (the cases at once, the machine's budget of
        rows += r                                               #  them; reported in their order)
        detail += d
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
