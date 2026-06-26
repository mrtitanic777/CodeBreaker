import re, os, glob, sys

SHELL = r"C:\Users\mrtit\Documents\Visual Studio 2010\Projects\MinecraftAlpha\MinecraftAlpha"
CONTENT = os.path.join(SHELL, "Release_LTCG", "Content")

refs = set()
for f in glob.glob(os.path.join(SHELL, "*.cpp")):
    txt = open(f, encoding="utf-8", errors="replace").read()
    for m in re.findall(r'"(game:[^"]*)"', txt):
        refs.add(m)

def to_rel(g):
    # strip the "game:\Content" prefix (source has doubled backslashes)
    p = g
    for pref in ("game:\\\\Content", "game:\\Content"):
        if p.startswith(pref):
            p = p[len(pref):]
            break
    p = p.replace("\\\\", "/").replace("\\", "/").lstrip("/")
    return p

present = set()
for dp, _, fs in os.walk(CONTENT):
    if "Content.new" in dp:
        continue
    for fn in fs:
        rel = os.path.relpath(os.path.join(dp, fn), CONTENT).replace("\\", "/")
        present.add(rel)

print("=== referenced paths: %d | files present: %d ===\n" % (len(refs), len(present)))

broken = []
missing = []
for g in sorted(refs):
    rel = to_rel(g)
    if not rel or rel in (".", ""):
        continue
    if "PORT" in g or "*" in g:
        broken.append(g); continue
    if rel not in present:
        missing.append(rel)

print("=== BROKEN refs (converter artifacts in the path string) ===")
for b in broken: print("   ", b)
print("\n=== MISSING (referenced, not at that path) -> where the basename actually is ===")
for m in missing:
    base = os.path.basename(m)
    locs = sorted(p for p in present if os.path.basename(p) == base)
    print("   %-28s  actual: %s" % (m, locs if locs else "*** NOWHERE ***"))

# orphans: present pngs never referenced (informational)
reffiles = set(to_rel(g) for g in refs)
print("\n=== ORPHAN png/asset files present but never referenced in code ===")
for p in sorted(present):
    if p.endswith(".txt"): continue
    if p not in reffiles:
        # also accept basename-only match (in case ref uses a different dir)
        print("   ", p)
