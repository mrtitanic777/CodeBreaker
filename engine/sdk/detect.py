#!/usr/bin/env python3
r"""detect.py -- toolchain auto-detection from an ingested SDK/DDK (Phase 3).

Given the tool executables an Inventory found, identify each one's role+version by
reading the version banner embedded in the binary, and map it to a backbone adapter
NAME (so the reconstruction can then drive it through the API backbone). Honest by
construction: a tool whose version can't be read is reported role='?'/adapter=None
for hand review -- never mapped to an arbitrary version.

    from engine.sdk import ingest, detect
    inv = ingest.scan(sdk_root)
    for t in detect.identify_tools(inv):
        print(t)          # {'path':.., 'role':'compiler', 'version':'6.00', 'adapter':'msc-6.0a'}
"""
import os, re

# banner signature -> (role, backbone adapter name). Probed against the exe's ASCII.
_SIGS = [
    (re.compile(rb"Microsoft \(R\) C.{0,40}?Version 6\.0", re.S), ("compiler", "msc-6.0a")),
    (re.compile(rb"Microsoft \(R\) C.{0,40}?Version 8\.0", re.S), ("compiler", "msvc-1.52c")),
    (re.compile(rb"Microsoft \(R\) C.{0,40}?Version 9\.0", re.S), ("compiler", "msvc-2.0")),
    (re.compile(rb"Macro Assembler.{0,40}?Version 5\.1", re.S), ("assembler", "masm-5.1")),
    (re.compile(rb"Macro Assembler.{0,40}?Version 6\.1", re.S), ("assembler", "masm-6.13")),
    (re.compile(rb"Segmented.{0,40}?Linker.{0,40}?Version 5\.20", re.S), ("linker", "link-5.20")),
    (re.compile(rb"Linker.{0,40}?Version 5\.60", re.S), ("linker", "link-5.60")),
    (re.compile(rb"Linker.{0,40}?Version 5\.20", re.S), ("linker", "link-5.20")),
]

# fallback role from the executable base name when no banner matches
_ROLE_BY_NAME = {"cl.exe": "compiler", "link.exe": "linker", "masm.exe": "assembler",
                 "ml.exe": "assembler", "rc.exe": "rc", "lib.exe": "lib",
                 "dosxnt.exe": "support"}


def identify_binary(path):
    """Read a tool binary and identify (role, version-text, adapter-name)."""
    try:
        data = open(path, "rb").read()
    except OSError:
        return {"path": path, "role": "?", "version": None, "adapter": None}
    for rx, (role, adapter) in _SIGS:
        m = rx.search(data)
        if m:
            vt = re.search(rb"Version [\d.]+", m.group(0))
            return {"path": path, "role": role,
                    "version": vt.group(0).decode() if vt else None, "adapter": adapter}
    role = _ROLE_BY_NAME.get(os.path.basename(path).lower(), "?")
    return {"path": path, "role": role, "version": None, "adapter": None}


def identify_tools(inventory):
    return [identify_binary(p) for p in inventory.tools]


def plan(inventory):
    """A toolchain PLAN: role -> chosen backbone adapter name (or None + reason).
    Cross-checks against what the backbone actually has registered/available."""
    ids = identify_tools(inventory)
    chosen = {}
    for t in ids:
        if t["adapter"] and t["role"] not in chosen:
            chosen[t["role"]] = t["adapter"]
    # verify each choice is a live backbone adapter
    report = {}
    try:
        from engine.backbone import BACKBONE as bb
        for role, adapter in chosen.items():
            try:
                a = bb.get(role, adapter)
                report[role] = {"adapter": adapter, "available": a.available()}
            except Exception as e:
                report[role] = {"adapter": adapter, "available": False, "note": str(e)}
    except Exception as e:
        for role, adapter in chosen.items():
            report[role] = {"adapter": adapter, "available": None, "note": "backbone: %s" % e}
    # surface tools we could not identify
    report["_unidentified"] = [t["path"] for t in ids if not t["adapter"]]
    return report


if __name__ == "__main__":
    import sys
    from engine.sdk import ingest
    from engine.sdk import w95paths
    root = sys.argv[1] if len(sys.argv) > 1 else w95paths.toolchain_root()
    inv = ingest.scan(root)
    print("tools found:", len(inv.tools))
    for t in identify_tools(inv)[:20]:
        print("  %-11s %-10s %s" % (t["role"], t["adapter"] or "-", os.path.basename(t["path"])))
    print("plan:", plan(inv))
