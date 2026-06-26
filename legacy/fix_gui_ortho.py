import os, re, glob

HW = r"C:\Users\mrtit\Documents\Visual Studio 2010\Projects\MinecraftAlpha\ConversionTool\handwritten_alpha"

# Minecraft's GUI glOrtho(0,w,h,0,zn,zf) -> D3D LH must be Y-FLIPPED (b=h+0.5, t=0.5,
# +0.5 half-pixel) and the view Z translate must be POSITIVE (LH visible range is [+zn,+zf]).
# The converter/agents emitted the GL form (b=0,t=h, translate -N) -> all GUI geometry
# clipped behind the near plane -> blank. Fix every such ortho + its negative Z translate.
ortho_re = re.compile(
    r'D3DXMatrixOrthoOffCenterLH\(&_ortho, \(float\)\(0\.0\), '
    r'\(float\)\(\(double\)([^)]+)\), \(float\)\(0\.0\), '
    r'\(float\)\(\(double\)([^)]+)\), (\(float\)\([0-9.]+\)), (\(float\)\([0-9.]+\))\)'
)
ortho_repl = (r'D3DXMatrixOrthoOffCenterLH(&_ortho, (float)(0.5), '
              r'(float)((double)\1 + 0.5), (float)((double)\2 + 0.5), (float)(0.5), \3, \4)')

# GUI Z translates (verified GUI-only per file): -2000 (zn=1000 screens) and -200 (loading, zn=100)
trans_re = re.compile(r'(D3DXMatrixTranslation\(&_t, 0\.0F, 0\.0F, )-(2000\.0F|200\.0F)\)')

total_o = total_t = 0
for f in glob.glob(os.path.join(HW, "*.cpp")):
    c = open(f, encoding="utf-8", errors="replace").read()
    c2, no = ortho_re.subn(ortho_repl, c)
    c2, nt = trans_re.subn(r'\g<1>\2)', c2)
    if c2 != c:
        open(f, "w", encoding="utf-8").write(c2)
        print("  %-28s ortho=%d ztrans-flip=%d" % (os.path.basename(f), no, nt))
    total_o += no; total_t += nt
print("TOTAL: ortho fixed=%d, z-translate flipped=%d" % (total_o, total_t))
