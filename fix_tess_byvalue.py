"""
fix_tess_byvalue.py -- fix by-value Tesselator copies.

The converter emitted Java `Tessellator var = Tessellator.instance;` as
`Tesselator var = Tesselator::getInstance();` (BY VALUE). Tesselator holds a
262144-entry m_vertices[] array (~6.3 MB), so each by-value copy blows the
Xbox 360 thread stack -> crash (IP 0x0). Java semantics are a REFERENCE; emit
`Tesselator& var = ...`.

Anchored to line-start (after indentation) so the RenderBlocks.cpp explanatory
comment (`// \`Tesselator varN = ...\``) is NOT touched.
"""
import os, re, sys

root = sys.argv[1] if len(sys.argv) > 1 else \
    r"C:\Users\mrtit\Documents\Visual Studio 2010\Projects\MinecraftAlpha\MinecraftAlpha"

# ^(indent)Tesselator (name) = Tesselator::getInstance   -> insert & after Tesselator
pat = re.compile(r'^(\s*)Tesselator (\w+) = Tesselator::getInstance')
fixed = []
for dp, _, fs in os.walk(root):
    for fn in fs:
        if not fn.endswith(".cpp"):
            continue
        p = os.path.join(dp, fn)
        lines = open(p, "r", encoding="utf-8", errors="replace").read().split("\n")
        changed = 0
        for i, ln in enumerate(lines):
            if pat.match(ln):
                lines[i] = pat.sub(r'\1Tesselator& \2 = Tesselator::getInstance', ln)
                changed += 1
        if changed:
            open(p, "w", encoding="utf-8").write("\n".join(lines))
            fixed.append((os.path.relpath(p, root), changed))

total = sum(c for _, c in fixed)
print("Fixed %d by-value Tesselator copies across %d files:" % (total, len(fixed)))
for rel, c in fixed:
    print("  %-28s %d" % (rel, c))
