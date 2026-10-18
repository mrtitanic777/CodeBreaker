"""Goto-free structuring (docs/CORE_ROADMAP.md, Track 1 item D), measured on the golden corpus.

    python tools/goto_report.py [NAME ...]

Runs the decompiler as it is now on each golden module's frozen description (tools/golden.py)
and counts, in its C and in the frozen C: `goto` statements, lines, gotos per 1,000 lines; and
what each goto jumps to -- a `return` (or a short tail ending in one), a place earlier in the
function (a loop the structurer could not make one), or somewhere further on.

The report is written to state/goto_report.txt; each module's output to state/gotos/.
"""
import collections
import json
import os
import re
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)

from codebreaker.decompile import decompiler                 # noqa: E402

GOLDEN = os.path.join(ROOT, "state", "golden")
OUT = os.path.join(ROOT, "state", "gotos")
GOTO = re.compile(r"\bgoto (\w+);")
LABEL = re.compile(r"^\s*(\w+):\s*$")


def kinds(body):
    """Counter of what each goto in one function's C jumps to"""
    lines = body.split("\n")
    labels = {m.group(1): i for i, l in enumerate(lines) for m in [LABEL.match(l)] if m}
    out = collections.Counter()
    for i, line in enumerate(lines):
        for m in GOTO.finditer(line):
            j = labels.get(m.group(1))
            if j is None:
                out["other"] += 1
                continue
            tail = []
            for x in lines[j + 1:j + 6]:
                x = x.strip()
                if LABEL.match(x):
                    continue
                tail.append(x)
                if x.startswith("return") or x == "}":
                    break
            if tail and tail[0].startswith("return"):
                out["to a return"] += 1
            elif any(x.startswith("return") for x in tail) and len([x for x in tail if x != "}"]) <= 3:
                out["to a short tail ending in return"] += 1
            elif j < i:
                out["backward"] += 1
            else:
                out["forward"] += 1
    return out


def measure(bodies):
    gotos = sum(len(GOTO.findall(b)) for b in bodies)
    lines = sum(b.count("\n") + 1 for b in bodies)
    k = collections.Counter()
    for b in bodies:
        k.update(kinds(b))
    return gotos, lines, k


def run(name, desc):
    os.makedirs(OUT, exist_ok=True)
    path = os.path.join(OUT, name + ".txt")
    env = dict(os.environ, CB_LANGUAGES=decompiler._languages(),
               CB_THREADS=os.environ.get("CB_THREADS", "4"), CB_MEMORY_MB=os.environ.get("CB_MEMORY_MB", "4000"))
    with open(path, "w", encoding="utf-8", errors="replace") as f:
        subprocess.run([decompiler._exe(), desc], stdout=f, stderr=subprocess.DEVNULL, env=env)
    text = open(path, encoding="utf-8", errors="replace").read()
    return [b.partition("*/\n")[2] for b in text.split("/*@@ ")[1:]]


def main(names):
    names = names or sorted(os.listdir(GOLDEN))
    lines = []
    tot = [0, 0, 0, 0, collections.Counter(), collections.Counter()]
    for name in names:
        gpath = os.path.join(GOLDEN, name, "golden.json")
        if not os.path.exists(gpath):
            continue
        member = next(iter(json.load(open(gpath, encoding="utf-8")).values()))
        g0, l0, k0 = measure(list(member["functions"].values()))
        g1, l1, k1 = measure(run(name, member["description"]))
        tot[0] += g0; tot[1] += l0; tot[2] += g1; tot[3] += l1; tot[4].update(k0); tot[5].update(k1)
        lines.append("%-9s gotos %5d -> %5d   per 1,000 lines %5.1f -> %5.1f" %
                     (name, g0, g1, g0 * 1000.0 / max(l0, 1), g1 * 1000.0 / max(l1, 1)))
        print(lines[-1], flush=True)
    lines.append("%-9s gotos %5d -> %5d   per 1,000 lines %5.1f -> %5.1f" %
                 ("TOTAL", tot[0], tot[2], tot[0] * 1000.0 / max(tot[1], 1), tot[2] * 1000.0 / max(tot[3], 1)))
    lines.append("")
    lines.append("by target (before -> after):")
    for k in ("to a return", "to a short tail ending in return", "backward", "forward", "other"):
        lines.append("  %-34s %5d -> %5d" % (k, tot[4][k], tot[5][k]))
    report = "\n".join(lines) + "\n"
    with open(os.path.join(ROOT, "state", "goto_report.txt"), "w", encoding="utf-8") as f:
        f.write(report)
    print(report)


if __name__ == "__main__":
    main(sys.argv[1:])
