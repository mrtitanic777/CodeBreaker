"""Goto-free structuring (docs/CORE_ROADMAP.md, Track 1 item D), measured on the golden corpus.

    python tools/goto_report.py [NAME ...] [--classes]

Runs the decompiler as it is now on each golden module's frozen description (tools/golden.py)
and counts, in its C and in the frozen C: `goto` statements, lines, gotos per 1,000 lines; and
what each goto jumps to -- a `return` (or a short tail ending in one), a place earlier in the
function (a loop the structurer could not make one), or somewhere further on.

With --classes, each goto of the new C is also put in a class by where its label stands
(docs/STRUCTURING_PLAN.md): out of a loop, into a loop (a second entry), a loop made by a goto,
out of nested blocks, into another block -- besides the tails above. The functions of each
class are listed in state/goto_report/classes/<class>.txt (the regression set of each phase).

The report is written to state/goto_report/goto_report.txt; each module's output to
state/goto_report/modules/.
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
HOME = os.path.join(ROOT, "state", "goto_report")
OUT = os.path.join(HOME, "modules")
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


LOOPS = ("while_statement", "do_statement", "for_statement")
CLASSES = ("to a return", "to a short tail ending in return", "out of a loop", "into a loop (second entry)",
           "loop made by a goto", "out of nested blocks", "into another block", "unparsed")


def classes(body):
    """[(class, label)] of each goto in one function's C, by where its label stands (CLASSES)"""
    from codebreaker.decompile import ctree
    tails = kinds(body)
    try:
        src = ctree.Source(body)
    except Exception:
        return [("unparsed", "")] * len(GOTO.findall(body))
    labels = {}
    for n in src.find("labeled_statement"):
        lab = n.child_by_field_name("label")
        if lab is not None:
            labels[src.at(lab)] = n
    out = []
    lines = body.split("\n")
    for g in src.find("goto_statement"):
        lab = g.child_by_field_name("label")
        name = src.at(lab) if lab is not None else ""
        t = labels.get(name)
        if t is None:
            out.append(("unparsed", name))
            continue
        # the tail classes, as kinds() sees them (a return right after the label, or a short tail)
        j = src.data[:t.start_byte].count(b"\n")
        after = [x.strip() for x in lines[j + 1:j + 6] if x.strip() and not LABEL.match(x.strip())]
        if after and after[0].startswith("return"):
            out.append(("to a return", name))
            continue
        if any(x.startswith("return") for x in after[:4]) and len([x for x in after[:4] if x != "}"]) <= 3 and \
                after[:4].index(next(x for x in after[:4] if x.startswith("return"))) < 3:
            out.append(("to a short tail ending in return", name))
            continue
        gl = [a for a in src.ancestors(g, *LOOPS)]
        tl = [a for a in src.ancestors(t, *LOOPS)]
        gids = {(a.start_byte, a.end_byte) for a in gl}
        tids = {(a.start_byte, a.end_byte) for a in tl}
        if tids - gids:
            out.append(("into a loop (second entry)", name))
        elif gids - tids:
            out.append(("out of a loop", name))
        elif t.start_byte < g.start_byte:
            out.append(("loop made by a goto", name))
        else:
            # the label's block holds the goto (it jumps out of blocks it is nested in), or not
            blk = t.parent
            if blk is not None and blk.start_byte <= g.start_byte and g.end_byte <= blk.end_byte:
                out.append(("out of nested blocks", name))
            else:
                out.append(("into another block", name))
    return out


def measure(bodies):
    gotos = sum(len(GOTO.findall(b)) for b in bodies)
    lines = sum(b.count("\n") + 1 for b in bodies)
    k = collections.Counter()
    for b in bodies:
        k.update(kinds(b))
    return gotos, lines, k


def class_counts(bodies, names, module):
    """Counter of the classes of the gotos in the bodies; {class: [module:function]}"""
    k, where = collections.Counter(), collections.defaultdict(list)
    for fn, b in zip(names, bodies):
        for c, _lab in classes(b):
            k[c] += 1
            if not where[c] or where[c][-1] != "%s:%s" % (module, fn):
                where[c].append("%s:%s" % (module, fn))
    return k, where


def run(name, desc):
    os.makedirs(OUT, exist_ok=True)
    path = os.path.join(OUT, name + ".txt")
    env = dict(os.environ, CB_LANGUAGES=decompiler._languages(),
               CB_THREADS=os.environ.get("CB_THREADS", "4"), CB_MEMORY_MB=os.environ.get("CB_MEMORY_MB", "4000"))
    with open(path, "w", encoding="utf-8", errors="replace") as f:
        subprocess.run([decompiler._exe(), desc], stdout=f, stderr=subprocess.DEVNULL, env=env)
    text = open(path, encoding="utf-8", errors="replace").read()
    parts = text.split("/*@@ ")[1:]
    run.names = [p.split()[0] for p in parts]
    return [b.partition("*/\n")[2] for b in parts]


def main(names):
    want_classes = "--classes" in names
    names = [n for n in names if n != "--classes"]
    names = names or sorted(os.listdir(GOLDEN))
    cls, cls_where = collections.Counter(), collections.defaultdict(list)
    lines = []
    tot = [0, 0, 0, 0, collections.Counter(), collections.Counter()]
    for name in names:
        gpath = os.path.join(GOLDEN, name, "golden.json")
        if not os.path.exists(gpath):
            continue
        member = next(iter(json.load(open(gpath, encoding="utf-8")).values()))
        g0, l0, k0 = measure(list(member["functions"].values()))
        bodies = run(name, member["description"])
        g1, l1, k1 = measure(bodies)
        if want_classes and name != "COMMAND":                # (hand-written assembly: its C is not the deliverable)
            kc, wc = class_counts(bodies, run.names, name)
            cls.update(kc)
            for c, fns in wc.items():
                cls_where[c] += fns
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
    if want_classes:
        lines.append("")
        lines.append("by class, outside COMMAND.COM (docs/STRUCTURING_PLAN.md):")
        for c in CLASSES:
            lines.append("  %-34s %5d" % (c, cls[c]))
        d = os.path.join(HOME, "classes")
        os.makedirs(d, exist_ok=True)
        for c in CLASSES:
            with open(os.path.join(d, re.sub(r"\W+", "_", c).strip("_") + ".txt"), "w", encoding="utf-8") as f:
                f.write("\n".join(cls_where.get(c, [])) + "\n")
    report = "\n".join(lines) + "\n"
    os.makedirs(HOME, exist_ok=True)
    with open(os.path.join(HOME, "goto_report.txt"), "w", encoding="utf-8") as f:
        f.write(report)
    print(report)


if __name__ == "__main__":
    main(sys.argv[1:])
