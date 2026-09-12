#!/usr/bin/env python3
r"""frontier.py — a COMPLEXITY CENSUS of the reconstruction, to map the byte-exact-C frontier.

matchbatch measures byte-match on the trivial tier (identity/const). But "how much of Windows
can we recompile byte-exact" needs the shape of the whole corpus first: what fraction of
functions is trivial (done), a wrapper (one call), straight-line (no control flow), conditional
(branches), or a loop. Each class is a distinct rung of raiser work, so this census turns the
frontier from a vibe into numbers — and says exactly where the next increment pays off.

    wt --target win95 frontier            # census over 32-bit (PE) modules
    wt --target win95 frontier --all      # every module (NE/PE/LE)

Classification is on the lifted [AUTO] C body (structure, not semantics):
  trivial     body reduces to `return arg/const`               (matchbatch already: ~99%)
  wrapper     one call then return  (call rel32 is oracle-masked -> next-easiest real code)
  straight    >1 stmt, NO goto/if/while  (sequence of ops)
  conditional has if/goto but no loop
  loop        has while/for/loop
  raw/stub    contains __asm (un-lifted opcode) — not clean C yet
"""
import glob
import os
import re
import sys

from codebreaker.core import layout

FN_RE = re.compile(
    r"/\* \[AUTO\] \w+\s+\w+ seg\d+:0x[0-9a-f]+\s+\w+ \*/\n"
    r"[A-Za-z_][\w ]*\**\w+\([^)]*\)\n\{(.*?)\n\}", re.S)


def classify_body(body):
    if "__asm(" in body:
        return "raw/stub"
    if re.search(r"\b(while|for)\b", body):
        return "loop"
    if re.search(r"\b(if|goto)\b", body):
        return "conditional"
    # no control flow below. drop pure local-declaration lines (u32 v1, v2;).
    stmts = [s.strip() for s in body.strip().split("\n")
             if s.strip() and not re.fullmatch(r"u\d+ [\w, ]+;", s.strip())]
    ncall = len(re.findall(r"\bsub_[0-9a-f]+\(", body)) + body.count("(fnptr)")
    if len(stmts) == 1 and re.fullmatch(r"return (arg\d+|-?\d+|0x[0-9a-f]+);", stmts[0]):
        return "trivial"
    if ncall >= 1 and stmts and stmts[-1].startswith("return"):
        return "wrapper"          # call(s) then return; call rel32 is oracle-masked
    return "straight"


def _modules(all_fmts):
    gen = glob.glob(os.path.join(layout.generated_dir(), "**", "*.c"), recursive=True)
    if not all_fmts:
        gen = [g for g in gen if re.search(r"\.(dll|exe|cpl|ocx)\.c$", g)]
    return gen


def main():
    a = sys.argv[1:]
    all_fmts = "--all" in a
    from collections import Counter
    cats = Counter()
    per_mod_tot = 0
    files = _modules(all_fmts)
    print("scanning %d module C files (%s)..." % (len(files), "all formats" if all_fmts else "PE/32-bit only"),
          flush=True)
    for f in files:
        txt = open(f, encoding="utf-8", errors="replace").read()
        for m in FN_RE.finditer(txt):
            cats[classify_body(m.group(1))] += 1
            per_mod_tot += 1
    tot = sum(cats.values())
    print("\n" + "=" * 58)
    print("FRONTIER COMPLEXITY CENSUS  (%d functions)" % tot)
    print("=" * 58)
    order = ["trivial", "wrapper", "straight", "conditional", "loop", "raw/stub"]
    note = {
        "trivial": "matchbatch ~99% (DONE)",
        "wrapper": "call masked -> next raiser increment",
        "straight": "translate lifted stmts -> C",
        "conditional": "needs control-flow structuring",
        "loop": "needs loop recovery",
        "raw/stub": "un-lifted opcodes (not clean C)",
    }
    for c in order:
        n = cats.get(c, 0)
        print("  %-12s %8d  %5.1f%%   %s" % (c, n, 100 * n / max(1, tot), note[c]))
    print("-" * 58)
    done = cats.get("trivial", 0)
    reachable = done + cats.get("wrapper", 0) + cats.get("straight", 0)
    print("  byte-match DONE (trivial):        %5.1f%%" % (100 * done / max(1, tot)))
    print("  NO-control-flow reachable soon:   %5.1f%%  (trivial+wrapper+straight)"
          % (100 * reachable / max(1, tot)))
    print("  needs structuring (cond+loop):    %5.1f%%"
          % (100 * (cats.get("conditional", 0) + cats.get("loop", 0)) / max(1, tot)))
    print("=" * 58)


if __name__ == "__main__":
    main()
