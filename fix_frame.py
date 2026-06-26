p = r"C:\Users\mrtit\Documents\Visual Studio 2010\Projects\MinecraftAlpha\MinecraftAlpha\Minecraft.cpp"
lines = open(p, "r", encoding="utf-8", errors="replace").read().split("\n")
bs_n = chr(92) + "n"   # literal backslash + n (the C escape \n)
out = []
i = 0
fixed = 0
while i < len(lines):
    ln = lines[i]
    if 'DbgPrintF("[FRAME]' in ln and not ln.rstrip().endswith('}') and i + 1 < len(lines) and lines[i+1].lstrip().startswith('", _f)'):
        out.append(ln + bs_n + lines[i+1].lstrip())
        i += 2
        fixed += 1
        continue
    out.append(ln)
    i += 1
open(p, "w", encoding="utf-8").write("\n".join(out))
print("fixed %d split [FRAME] markers" % fixed)
for l in open(p, encoding="utf-8").read().split("\n"):
    if "[FRAME]" in l:
        print(repr(l))
