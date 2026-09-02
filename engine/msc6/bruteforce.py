#!/usr/bin/env python3
"""Brute-force source search — crack a function's exact C with ZERO LLM tokens.

Because compile+diff is deterministic and free, we can enumerate the (small, bounded)
space of source forms an early-90s C function could have taken and compile them all,
keeping whichever reproduces the original bytes exactly.

The LLM writes ONE base reconstruction and marks the uncertain spots with choice
markers  {{ optA | optB | optC }}  (the options are literal C fragments).  We expand
the cross-product, batch-compile every variant in ONE DOSBox startup, diff each
against the target function, and report the winner(s).

Auto-axes (applied when asked) add common toggles without markers: a leading unused
`int i;`, loop-form rewrites, signed/unsigned index types, etc.
"""
import os
import re
import time
import itertools
import subprocess
from omf import parse as omf_parse
import verify


def expand(text, cap=512):
    """Expand  @{ a | b | c }@  choice markers into (combo_indices, source) variants.
    Marker uses @{ ... }@ (with | separators) because @ never appears in C, so content
    that ends in a brace can't collide with the closing delimiter."""
    parts = re.split(r"@\{(.*?)\}@", text, flags=re.S)
    literals = parts[0::2]
    choices = [c.split("|") for c in parts[1::2]]
    n = 1
    for c in choices:
        n *= len(c)
    if n > cap:
        raise ValueError("variant explosion: %d > cap %d (reduce markers)" % (n, cap))
    out = []
    for combo in (itertools.product(*choices) if choices else [()]):
        s = literals[0]
        for i, opt in enumerate(combo):
            s += opt + literals[i + 1]
        idx = tuple(choices[i].index(combo[i]) for i in range(len(combo)))
        out.append((idx, s))
    return out


def batch_compile(variants, workdir, cflags=verify.CFLAGS, timeout=None):
    """Compile many variants in ONE DOSBox startup (amortizes the launch across all
    variants).  variants = [(label, source_text)].  Returns {label: objpath|None}."""
    os.makedirs(workdir, exist_ok=True)
    labels = []
    lines = ["@echo off", "path c:\\bin", "set tmp=d:\\"]
    for i, (label, src) in enumerate(variants):
        mod = "v%d" % i
        open(os.path.join(workdir, mod + ".c"), "w").write(src)
        try:
            os.remove(os.path.join(workdir, mod + ".obj"))
        except OSError:
            pass
        lines.append("del %s.obj" % mod)
        lines.append("cl %s %s.c >> cl.txt" % (cflags, mod))
        labels.append((mod, label))
    lines.append("echo DONE > done.txt")
    open(os.path.join(workdir, "BLD.BAT"), "w").write("\n".join(lines) + "\n")
    open(os.path.join(workdir, "conf.conf"), "w").write(
        "[dosbox]\nmemsize=32\n[cpu]\ncputype=386\ncore=normal\ncycles=%d\n"
        "[dos]\nver=6.22\n[autoexec]\nmount c %s\nmount d %s\nd:\ncall BLD.BAT\nexit\n"
        % (verify.CYCLES, verify.TOOLCHAIN, os.path.abspath(workdir)))
    done = os.path.join(workdir, "done.txt")
    try:
        os.remove(done)
    except OSError:
        pass
    open(os.path.join(workdir, "cl.txt"), "w").close()
    timeout = timeout or (30 + 3 * len(variants))
    subprocess.Popen([verify.DOSBOX, "-conf", os.path.join(workdir, "conf.conf")],
                     stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    t0 = time.time()
    while not os.path.exists(done) and time.time() - t0 < timeout:
        time.sleep(1)
    return {label: (os.path.join(workdir, mod + ".obj")
                    if os.path.exists(os.path.join(workdir, mod + ".obj")) else None)
            for mod, label in labels}


def _func_diff(objpath, orig_seg, func_index, func_ranges):
    """Diff one function (by index in func_ranges) of a compiled segment module."""
    per = verify.diff_segment(objpath, orig_seg, func_ranges)
    name = func_ranges[func_index][2]
    return per[name][0]   # n_diffs


def crack(template, workdir, orig_seg, func_index, func_ranges, cap=512, verbose=True):
    """Expand `template` (full segment .c with {{...}} markers around the target
    function), batch-compile all variants, diff the target function, return
    (best_label, best_diffs, best_source, all_results)."""
    variants = [("v%d_%s" % (i, "-".join(map(str, idx))), src)
                for i, (idx, src) in enumerate(expand(template, cap))]
    if verbose:
        print("brute-forcing %d variants of %s ..." %
              (len(variants), func_ranges[func_index][2]))
    objs = batch_compile(variants, workdir)
    results = []
    src_by_label = {lbl: src for lbl, src in variants}
    for label, obj in objs.items():
        if not obj:
            results.append((label, 10 ** 9))
            continue
        try:
            d = _func_diff(obj, orig_seg, func_index, func_ranges)
        except Exception:
            d = 10 ** 9
        results.append((label, d))
    results.sort(key=lambda x: x[1])
    best_label, best_diffs = results[0]
    if verbose:
        exact = [l for l, d in results if d == 0]
        print("  best=%d diffs (%s)   exact variants: %d" %
              (best_diffs, best_label, len(exact)))
    return best_label, best_diffs, src_by_label[best_label], results


if __name__ == "__main__":
    # self-test: expander sanity
    t = "int f(){ {{return 1;|return 0;}} {{/*a*/|int i;}} }"
    v = expand(t)
    print("expanded %d variants:" % len(v))
    for idx, s in v:
        print("  ", idx, repr(s))
