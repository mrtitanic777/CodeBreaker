import os

ROOT = r"C:\Users\mrtit\Documents\Visual Studio 2010\Projects\MinecraftAlpha"
targets = [
    os.path.join(ROOT, "MinecraftAlpha", "Main.cpp"),
    os.path.join(ROOT, "MinecraftAlpha", "Textures.cpp"),
    os.path.join(ROOT, "ConversionTool", "output_alpha", "Main.cpp"),
    os.path.join(ROOT, "ConversionTool", "output_alpha", "Textures.cpp"),
]

# file contains the literal characters:  Content\\char.png  (two backslashes)
OLD = r"Content\\char.png"      # raw -> Content + two backslashes + char.png
NEW = r"Content\\mob\\char.png"

for t in targets:
    if not os.path.exists(t):
        print("  MISSING", t); continue
    c = open(t, encoding="utf-8", errors="replace").read()
    n = c.count(OLD)
    if n:
        c = c.replace(OLD, NEW)
        open(t, "w", encoding="utf-8", newline="").write(c)
    print("  %-46s matches=%d %s" % (os.path.relpath(t, ROOT), n, "PATCHED" if n else ""))
