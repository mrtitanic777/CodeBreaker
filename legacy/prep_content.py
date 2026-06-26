"""
prep_content.py — normalize every PNG in a Content folder so the Xbox 360 D3DX loader
(D3DXCreateTextureFromFile) can decode it.

360 D3DX rejects PNGs that are INDEXED (PLTE/palette) or carry certain ancillary chunks
(observed: pHYs, sRGB, iCCP). Working textures only had IHDR+gAMA+tEXt+IDAT. This script
re-encodes each PNG as a clean 8-bit RGBA image with NO ancillary chunks (just IHDR+IDAT
+IEND), which loads reliably.

Re-run this after ANY redeploy of the Content folder.

Usage:  python prep_content.py "<path-to-Content>"
        (defaults to the Debug\Content the emulator loads)
"""
import os, sys, struct
from PIL import Image

DEFAULT = r"C:\Users\mrtit\Documents\Visual Studio 2010\Projects\MinecraftAlpha\MinecraftAlpha\Debug\Content"
root = sys.argv[1] if len(sys.argv) > 1 else DEFAULT

CT = {0: "Gray", 2: "RGB", 3: "INDEXED", 4: "GrayA", 6: "RGBA"}

# Path-mismatch copies: the game loads these subpaths but the asset ships elsewhere.
# (dest relative path, [candidate source relative paths]). Copied if dest is missing.
COPIES = [
    ("misc/shadow.png",     ["shadow.png"]),
    ("misc/water.png",      ["water.png"]),
    ("mob/saddle.png",      ["item/saddle.png"]),
    ("mob/char.png",        ["char.png"]),
    ("gui/background.png",  ["background.png"]),
    ("title/black.png",     ["black.png"]),
]

def ensure_copies(root):
    import shutil
    made = []
    for dest, srcs in COPIES:
        dpath = os.path.join(root, dest.replace("/", os.sep))
        if os.path.exists(dpath):
            continue
        for s in srcs:
            spath = os.path.join(root, s.replace("/", os.sep))
            if os.path.exists(spath):
                os.makedirs(os.path.dirname(dpath), exist_ok=True)
                shutil.copy2(spath, dpath)
                made.append((dest, s))
                break
    return made

def png_chunks(path):
    b = open(path, "rb").read()
    i, out = 8, []
    while i < len(b) - 8 and len(out) < 20:
        ln = struct.unpack(">I", b[i:i+4])[0]
        typ = b[i+4:i+8].decode("latin1")
        out.append(typ)
        if typ == "IEND":
            break
        i += 12 + ln
    return out

BAD = {"pHYs", "sRGB", "iCCP", "PLTE", "gAMA", "bKGD", "cHRM", "tRNS", "tEXt", "zTXt", "iTXt"}

made_copies = ensure_copies(root)  # create path-mismatch copies BEFORE normalizing (so they get cleaned too)
fixed, clean, total, locked = [], 0, 0, []
for dp, _, fs in os.walk(root):
    if "Content.new" in dp:
        continue
    for fn in fs:
        if not fn.lower().endswith(".png"):
            continue
        total += 1
        p = os.path.join(dp, fn)
        rel = os.path.relpath(p, root).replace("\\", "/")
        with open(p, "rb") as f:
            head = f.read(26)
        ct = head[25] if head[:8] == b"\x89PNG\r\n\x1a\n" else -1
        before = set(png_chunks(p))
        needs = (ct != 6) or bool(before & BAD)
        if not needs:
            clean += 1
            continue
        im = Image.open(p).convert("RGBA")
        im.info = {}                       # drop dpi/gamma/srgb/icc so PIL emits no pHYs/sRGB/gAMA/iCCP
        try:
            try:
                os.chmod(p, 0o666)         # clear read-only if set
            except OSError:
                pass
            # write to a temp then replace, so a locked target fails cleanly without truncating
            tmp = p + ".tmp"
            im.save(tmp, "PNG", optimize=False)   # -> clean IHDR + IDAT + IEND, RGBA
            os.replace(tmp, p)
        except (PermissionError, OSError) as e:
            try:
                if os.path.exists(tmp): os.remove(tmp)
            except OSError:
                pass
            locked.append((rel, str(e)))
            continue
        after = set(png_chunks(p))
        fixed.append((rel, CT.get(ct, "?"), sorted(before & BAD), sorted(after - {"IHDR","IDAT","IEND"})))

print("Content: %s" % root)
for dest, src in made_copies:
    print("  copied %s  <- %s" % (dest, src))
print("PNGs: %d total | %d already clean | %d re-encoded\n" % (total, clean, len(fixed)))
for rel, oldct, badbefore, leftover in fixed:
    note = (" was %s" % oldct) if oldct != "RGBA" else ""
    strip = (" stripped %s" % ",".join(badbefore)) if badbefore else ""
    warn = ("  !! still has %s" % ",".join(leftover)) if leftover else ""
    print("  fixed %-30s%s%s%s" % (rel, note, strip, warn))
if locked:
    print("\n!! %d file(s) LOCKED/read-only (close the emulator + any image viewer, then re-run):" % len(locked))
    for rel, e in locked:
        print("   LOCKED %s" % rel)
