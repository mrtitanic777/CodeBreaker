#!/usr/bin/env python3
r"""l2predict.py -- corpus route-mix predictor / triage (Layer-2).

WIN95_MAP.md says which module TIER a function's route should follow: the 32-bit
C API DLLs (kernel32/user32/gdi32/...) are thin veneer -> mostly `c` route
(emit_c); MFC/C++ modules are `__thiscall` methods -> a leaf slice reachable by
emit_cpp with the bulk on the asm floor; thunk stubs mark the 16<->32 boundary.

This pass makes that quantitative WITHOUT compiling anything: it classifies every
corpus module (classify32 only -- fast), tags it with its tier, and reports the
actual route mix + the emit_cpp-reachable leaf fraction. It then uses each tier's
OWN distribution as the baseline and flags the modules that underperform their
peers -- the data-driven "look here next" list for emitter work, instead of
hand-guessed target percentages.

    python -m codebreaker.targets.win95.l2predict [--emit DIR] [--limit N]
"""
import argparse
import json
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

import pe32util as H
import classify32
import emit_cpp
import l2sweep

# Ordered name -> tier rules (first match wins). Tiers mirror WIN95_MAP.md.
_TIERS = [
    (r"^(mfc|mfco|mfcd|mfcn|mfcans|mfcuia|olepro)", "cpp",   "MFC / C++ framework (ordinal-exported __thiscall)"),
    (r"^(kernel32|gdi32|user32|advapi32|ntdll)$",   "capi",  "core Win32 C API (veneer over VxD/16-bit workers)"),
    (r"^(shell32|shlwapi|comctl32|comctl31|comdlg32|shscrap|shdocvw)", "shell", "shell / common-controls C API"),
    (r"^(ole32|oleaut32|olecli32|olesvr32|typelib|stdole)", "ole", "OLE/COM C API"),
    (r"^(winmm|msvfw32|mciole32|dciman32|ddraw|dsound)", "mm", "multimedia C API"),
    (r"^(ws2_32|wsock32|wininet|mpr|netapi32|rpcrt4)", "net", "networking C API"),
    (r".*", "other", "other 32-bit PE"),
]


def _tier(name):
    stem = re.sub(r"\.(dll|exe|drv|cpl)$", "", name.lower())
    for pat, tier, desc in _TIERS:
        if re.match(pat, stem):
            return tier, desc
    return "other", "other 32-bit PE"


def _cpp_leaf_claims(module, rows, c, base):
    """Count functions emit_cpp's LEAF modelers claim (getter/return-this/address/
    setter) -- the byte-exact-reachable __thiscall slice -- WITHOUT compiling."""
    n = 0
    for r in rows:
        s = base + r["rva"]
        ins = H.disasm(H.func_bytes(c, s, s + r["size"]), s)
        if not ins or not emit_cpp._is_thiscall(ins):
            continue
        body = emit_cpp._body(ins)
        if emit_cpp._getter(body) is not None or emit_cpp._setter(body) is not None:
            n += 1
    return n


def profile(module):
    """Classify one module; return its route counts + tier + emit_cpp leaf claims.
    named-only, matching the l2pipe sweep's function universe."""
    rows = [r for r in classify32.classify_module(module) if r["name"]]
    c = H.carve(module); base = c.pe.image_base
    tot = len(rows) or 1
    routes = {"c": 0, "cpp_leaf": 0, "thunk": 0, "asm": 0, "human": 0}
    for r in rows:
        routes[r["route"]] = routes.get(r["route"], 0) + 1
    routes["cpp_leaf"] = _cpp_leaf_claims(module, rows, c, base)
    tier, desc = _tier(module)
    return dict(module=module, tier=tier, tier_desc=desc, fns=len(rows),
                c=routes["c"], cpp_leaf=routes["cpp_leaf"], thunk=routes["thunk"],
                asm=routes["asm"], human=routes["human"],
                c_pct=100.0 * routes["c"] / tot,
                cpp_leaf_pct=100.0 * routes["cpp_leaf"] / tot,
                human_pct=100.0 * routes["human"] / tot)


# tier -> the metric that measures "reconstructable veneer" for that tier
_KEY = {"capi": "c_pct", "shell": "c_pct", "ole": "c_pct", "mm": "c_pct",
        "net": "c_pct", "other": "c_pct", "cpp": "cpp_leaf_pct"}


def predict(limit=None, emit=None):
    mods = l2sweep.pe_modules()
    if limit:
        mods = mods[:limit]
    print("L2 route-mix predictor: %d PE modules (classify-only, no compile)\n" % len(mods))
    profs = []
    for m in mods:
        try:
            profs.append(profile(m))
        except Exception as e:
            print("  %-20s ERROR %s" % (m, str(e)[:70]))
        for cc in (H._CARVE, H._NAMED, H._IMPORTS):
            cc.clear()
    # per-tier baseline (mean of the tier's key metric) + flag underperformers
    bytier = {}
    for p in profs:
        bytier.setdefault(p["tier"], []).append(p)
    print("=" * 74)
    print("%-16s %5s %6s %6s %6s %6s  tier" % ("module", "fns", "c%", "cpp%", "hum%", "flag"))
    print("-" * 74)
    flagged = []
    for tier in ("capi", "shell", "ole", "mm", "net", "cpp", "other"):
        ps = bytier.get(tier)
        if not ps:
            continue
        key = _KEY[tier]
        vals = [p[key] for p in ps]
        mean = sum(vals) / len(vals)
        var = sum((v - mean) ** 2 for v in vals) / len(vals)
        sd = var ** 0.5
        print("# tier %-6s (%s)  n=%d  mean %s=%.0f%%" % (tier, ps[0]["tier_desc"], len(ps), key, mean))
        for p in sorted(ps, key=lambda x: x[key]):
            under = len(ps) >= 3 and sd > 0 and p[key] < mean - sd    # >1σ below peers
            flag = "LOW" if under else ""
            if under:
                flagged.append(p)
            print("%-16s %5d %5.0f%% %5.0f%% %5.0f%%  %-4s %s"
                  % (p["module"], p["fns"], p["c_pct"], p["cpp_leaf_pct"], p["human_pct"], flag, tier))
    print("-" * 74)
    print("Flagged (underperform their tier -- candidate modules for emitter work): %d" % len(flagged))
    for p in flagged:
        print("  %-16s %s %.0f%% vs tier mean (key=%s)" % (p["module"], p["tier"], p[_KEY[p["tier"]]], _KEY[p["tier"]]))
    if emit:
        os.makedirs(emit, exist_ok=True)
        with open(os.path.join(emit, "_l2_route_prediction.json"), "w") as f:
            json.dump(dict(modules=profs, flagged=[p["module"] for p in flagged]), f, indent=1)
        print("\nwrote _l2_route_prediction.json")
    return profs


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--limit", type=int)
    ap.add_argument("--emit")
    a = ap.parse_args()
    predict(a.limit, a.emit)


if __name__ == "__main__":
    main()
