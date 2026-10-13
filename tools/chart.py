#!/usr/bin/env python3
"""chart.py -- redraw docs/architecture.png: how the parts of the workbench fit.

    python tools/chart.py [out.png]

The chart is drawn from the lists below, which name real modules; edit them when a part is
added, then run this.
"""
import os
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

# (title, what it is, the modules) for each step of the one path, left to right
PATH = [
    ("original binary", "a module of the corpus", ["base/corpus", "formats/*"]),
    ("decompile", "its functions as C\n(or assembly source)", ["decompile/describe", "decompile/decompiler", "decompile/c_text",
                                                               "decompile/struct_text"]),
    ("build", "each function compiled,\none module linked", ["build/functions", "build/module", "build/driver"]),
    ("run both", "original and rebuild,\nsame inputs", ["emulate/emulator", "emulate/windows16", "emulate/driver",
                                                      "emulate/powerpc"]),
    ("compare", "calls, result, memory,\nstack, registers", ["verify/compare", "verify/<kind>"]),
    ("record", "every function,\nevery verdict", ["store/functions"]),
    ("source tree", "the module as\nsource files", ["source/module_writer", "source/placement"]),
]
KINDS = ["32-bit Windows", "16-bit Windows", "PowerPC", "device drivers", "DOS programs"]
# what sits beside the path
SIDE = [
    ("the judge, judged", "verify/teeth: functions broken on purpose --\ndoes the judge notice?"),
    ("toward the authored source", "verify/match, repair, shape + build/period:\na period compiler, and rewrites that\nchange no meaning"),
    ("what is known about a module", "analyze/*: structures, classes, call graph\nsymbols/*: names for imports, runtime\nlibrary, services"),
    ("never twice", "base/cache: results kept by what made them\nverify/reconstruct: a verdict holds while\nthe function and what it calls are unchanged"),
]


def draw(out):
    import matplotlib
    matplotlib.use("Agg")
    import matplotlib.pyplot as plt
    from matplotlib.patches import FancyBboxPatch

    ink, soft, accent, ground = "#1d2733", "#5b6b7c", "#b4532a", "#f7f5f0"
    fig = plt.figure(figsize=(16, 8.6), dpi=130)
    ax = fig.add_axes([0, 0, 1, 1])
    ax.set_xlim(0, 16)
    ax.set_ylim(0, 8.6)
    ax.axis("off")
    fig.patch.set_facecolor(ground)
    ax.text(0.5, 8.1, "CodeBreaker", fontsize=22, fontweight="bold", color=ink)
    ax.text(0.5, 7.72, "one path for every kind of code: decompile, build, run beside the original, compare, record",
            fontsize=11.5, color=soft)

    w, h, y, gap = 1.9, 1.5, 5.15, 0.3
    x0 = 0.5
    for i, (title, what, modules) in enumerate(PATH):
        x = x0 + i * (w + gap)
        ax.add_patch(FancyBboxPatch((x, y), w, h, boxstyle="round,pad=0.02,rounding_size=0.08", fc="white", ec=ink, lw=1.4))
        ax.text(x + w / 2, y + h - 0.3, title, ha="center", fontsize=11.5, fontweight="bold", color=ink)
        ax.text(x + w / 2, y + h - 0.82, what, ha="center", va="center", fontsize=8.6, color=soft, linespacing=1.35)
        ax.text(x + w / 2, y - 0.16, "\n".join(modules), ha="center", va="top", fontsize=7.6, color=accent,
                family="monospace", linespacing=1.4)
        if i + 1 < len(PATH):
            ax.annotate("", xy=(x + w + gap - 0.03, y + h / 2), xytext=(x + w + 0.03, y + h / 2),
                        arrowprops=dict(arrowstyle="-|>", color=ink, lw=1.4))
    ax.text(x0, y + h + 0.3, "verify/reconstruct.py runs this path.  A kind of code is one file saying how its modules are "
            "prepared, built and judged:", fontsize=9.2, color=ink)
    kx = 9.3
    for k in KINDS:
        ax.text(kx, y + h + 0.3, k, fontsize=8.2, color="white", fontweight="bold",
                bbox=dict(boxstyle="round,pad=0.26", fc=ink, ec=ink))
        kx += 0.36 + 0.072 * len(k)

    sw, sh, sy = 3.55, 1.5, 1.35
    for i, (title, what) in enumerate(SIDE):
        x = x0 + i * (sw + 0.27)
        ax.add_patch(FancyBboxPatch((x, sy), sw, sh, boxstyle="round,pad=0.02,rounding_size=0.08", fc="#fffdf8", ec=soft, lw=1.0))
        ax.text(x + 0.16, sy + sh - 0.3, title, fontsize=10.2, fontweight="bold", color=ink)
        ax.text(x + 0.16, sy + sh - 0.52, what, fontsize=8.2, color=soft, va="top", family="monospace", linespacing=1.45)
    ax.text(x0, sy + sh + 0.2, "beside the path", fontsize=9.2, color=ink)
    ax.text(x0, 0.75, "jobs/pool spreads the work over processes and survives a crash   |   ui/ the local console   |   "
            "tests/run_all.py before every commit", fontsize=8.6, color=soft)
    fig.savefig(out, facecolor=ground)
    return out


if __name__ == "__main__":
    print("written:", draw(sys.argv[1] if len(sys.argv) > 1 else os.path.join(ROOT, "docs", "architecture.png")))
