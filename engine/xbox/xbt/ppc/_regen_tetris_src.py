# Surgical regen: re-run ONLY the PPC->C++ recompile (with the updated
# ppcrecomp.py opcode set) and overwrite just the numbered recompiled source
# files + the two generated headers in an EXISTING project. Everything else
# (hand-edited boot rex_runtime.cpp, rex_video_shim.cpp, tetris_imports.cpp,
# tetris_missing_stubs.cpp, build_all.sh, vendored kernel/GPU) is left untouched.
import os, sys, glob, re

PROJ = r"<local>\Tetris-Recompiled"
XEX  = os.path.join(PROJ, "assets", "tetris.xex")
NAME = "tetris"

import progress
progress.enable()
progress.begin("XboxTranspiler - regen recompiled source (new opcodes)")

import xex, ppcrecomp, ppcproject, parallel

x = xex.open_xex(XEX)
image_base = x.image_base or 0
entry = x.entry_point or image_base

# import thunks are skipped (host-shim layer provides them) - same as scaffold()
thunks = {}
for lib, recs in x.imports().items():
    for addr, ordv, kind in recs:
        if kind == "thunk":
            thunks[addr] = (lib, ordv)

inc = os.path.join(PROJ, "include")
src = os.path.join(PROJ, "src")

# Also skip the EABI save/restore helpers: the hand-written tetris_ppc_helpers.cpp
# provides correct versions (the recompiler mis-splits their fall-through chains
# into 1-instruction functions, corrupting caller frames). Exclude their exact
# addresses so the numbered files don't re-define them.
helper_addrs = set()
_hp = os.path.join(src, "tetris_ppc_helpers.cpp")
if os.path.exists(_hp):
    for _m in re.findall(r"sub_([0-9A-Fa-f]{8})", open(_hp, encoding="utf-8").read()):
        helper_addrs.add(int(_m, 16))
print(f"[regen] skipping {len(thunks)} import thunks + {len(helper_addrs)} EABI helpers", flush=True)
skip = set(thunks) | helper_addrs

# runtime-observed indirect-call targets to force-discover (function-discovery
# feedback loop): one hex address per line in regen_seeds.txt.
extra = set()
_sf = os.path.join(PROJ, "regen_seeds.txt")
if os.path.exists(_sf):
    for _ln in open(_sf, encoding="utf-8"):
        _ln = _ln.strip()
        if _ln.startswith("0x"):
            extra.add(int(_ln, 16))
print(f"[regen] seeding {len(extra)} runtime-observed indirect targets", flush=True)

jobs = parallel.resolve_jobs(sys.argv)
print(f"[regen] recompiling {XEX} (jobs={jobs}) ...", flush=True)
_body, mapping, agg = ppcrecomp.recompile_image(XEX, jobs=jobs, skip=skip, extra=extra)
parts, decls = agg["parts"], agg["decls"]

inc = os.path.join(PROJ, "include")
src = os.path.join(PROJ, "src")

# 1) delete the OLD numbered recompiled files (shard layout may change) so no
#    function ends up defined twice.
old = glob.glob(os.path.join(src, f"{NAME}_[0-9]*.cpp"))
for f in old:
    os.remove(f)
print(f"[regen] removed {len(old)} old numbered files", flush=True)

# 2) write the new shards
shards = ppcproject._shards(parts)
for i, group in enumerate(shards):
    text = f'#include "{NAME}.h"\n\n' + "\n".join(group)
    with open(os.path.join(src, f"{NAME}_{i:04d}.cpp"), "w", encoding="utf-8") as fh:
        fh.write(text)
print(f"[regen] wrote {len(shards)} new numbered files", flush=True)

# 3) refresh the two generated headers (declarations + runtime); harmless if
#    unchanged, and picks up any new forward decls.
with open(os.path.join(inc, "rex_ppc_min.h"), "w", encoding="utf-8") as fh:
    fh.write(ppcrecomp.runtime_header())
with open(os.path.join(inc, NAME + ".h"), "w", encoding="utf-8") as fh:
    fh.write(ppcproject._recomp_header(NAME, decls))

# 4) refresh the address->function mapping table so newly-seeded functions are
#    resolvable by indirect calls (ppc_call_indirect). chosen = all discovered
#    addrs (incl. seeds); definitions live in the numbered files / helpers /
#    imports, all declared in the header.
with open(os.path.join(src, NAME + "_init.cpp"), "w", encoding="utf-8") as fh:
    fh.write(ppcproject._init_cpp(NAME, agg["chosen"]))
print(f"[regen] refreshed mapping table ({len(agg['chosen'])} entries)", flush=True)

pct = 100.0 * agg["translated"] / max(1, agg["total"])
msg = f"regen done: {agg['funcs']:,} funcs, {len(shards)} files, {pct:.1f}% translated"
print("[regen] " + msg, flush=True)
try:
    progress.report("Recompiled source regenerated (new opcode set)",
                    [f"Functions:   {agg['funcs']:,}",
                     f"Files:       {len(shards)} numbered .cpp",
                     f"Translated:  {pct:.1f}% of {agg['total']:,} instructions",
                     "Preserved:   boot, video shim, imports, stubs, kernel"])
except Exception:
    pass
