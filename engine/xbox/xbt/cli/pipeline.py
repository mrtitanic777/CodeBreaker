"""Pipeline runner — chain XboxTranspiler commands into scripted workflows.

A pipeline is a plain-text script: one command per line (the same commands you
run by hand), with `# comments`, `set VAR=value` assignments, `{VAR}`
substitution, and a leading `-` to ignore a step's failure. It lets you capture
a repeatable job — "extract this GoD, carve the FATX, verify against the DAT,
repackage" — as one file and run it with variables.

    set IMG = hdd.bin
    set OUT = ./out
    fatx recover {IMG} {OUT}
    -datcheck verify {OUT}/default.xex xbox360.dat     # '-' = keep going if it fails

Commands run through the same dispatcher (the frozen EXE re-invokes itself; the
script re-invokes the CLI entry point), so every builtin/plugin is available.
"""

import os
import re
import subprocess
import sys

DESCRIPTION = "run a pipeline script: chain XboxTranspiler commands with variables"

_VAR = re.compile(r"\{(\w+)\}")


def _split(line):
    """Shell-lite tokenizer: whitespace split honoring double quotes."""
    out, cur, q = [], [], False
    for c in line:
        if c == '"':
            q = not q
        elif c.isspace() and not q:
            if cur:
                out.append("".join(cur))
                cur = []
        else:
            cur.append(c)
    if cur:
        out.append("".join(cur))
    return out


def parse_script(text, variables=None):
    """Return (steps, variables). steps = [(argv, ignore_errors)]."""
    variables = dict(variables or {})
    steps = []
    for raw in text.splitlines():
        line = raw.split("#", 1)[0].strip()
        if not line:
            continue
        ignore = line.startswith("-")
        if ignore:
            line = line[1:].strip()
        if line.lower().startswith("set "):
            k, _, v = line[4:].partition("=")
            variables[k.strip()] = v.strip()
            continue
        resolved = _VAR.sub(lambda m: variables.get(m.group(1), m.group(0)), line)
        steps.append((_split(resolved), ignore))
    return steps, variables


def _dispatch(argv):
    """Command line that runs one XboxTranspiler command."""
    if getattr(sys, "frozen", False):
        return [sys.executable] + argv
    entry = os.path.join(os.path.dirname(os.path.abspath(__file__)), "xboxtranspiler.py")
    return [sys.executable, entry] + argv


def run(steps, echo=True):
    """Run steps in order; stop on the first failure unless it is `ignore`.
    Returns (ok_count, failed_step_or_None)."""
    ok = 0
    for argv, ignore in steps:
        if echo:
            print("$ %s" % " ".join(argv))
        r = subprocess.run(_dispatch(argv))
        if r.returncode != 0 and not ignore:
            return ok, (argv, r.returncode)
        ok += 1
    return ok, None


def main():
    a = sys.argv[1:]
    if not a or a[0] in ("-h", "--help"):
        print("usage: pipeline <script.txt> [--set K=V ...] [--dry]\n"
              "  --dry   print the resolved commands without running them")
        return
    variables = {}
    i = 0
    while i < len(a):
        if a[i] == "--set" and i + 1 < len(a):
            k, _, v = a[i + 1].partition("=")
            variables[k] = v
            i += 2
        else:
            i += 1
    steps, variables = parse_script(open(a[0], encoding="utf-8").read(), variables)
    if "--dry" in a:
        for argv, ignore in steps:
            print(("-" if ignore else " ") + " " + " ".join(argv))
        print("(%d step(s); variables: %s)" % (len(steps), variables))
        return
    ok, failed = run(steps)
    if failed:
        print("\nFAILED at step %d: %s (exit %d)" % (ok + 1, " ".join(failed[0]), failed[1]))
        sys.exit(1)
    print("\npipeline complete: %d step(s) ok" % ok)


if __name__ == "__main__":
    main()
