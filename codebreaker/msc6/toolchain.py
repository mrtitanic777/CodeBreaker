#!/usr/bin/env python3
"""toolchain.py -- resolver over MANIFEST.json.

The reconstruction pipeline locates every tool through this module instead of
hardcoding paths, so a layout change is a one-line manifest edit, never a
codebase-wide hunt.  It NEVER invents a path: an absent/`present:false` tool
raises ToolchainGap with the manifest's explanation, so the caller can surface
the gap for hand review rather than silently guessing (per RECONSTRUCTION_SPEC).

CLI:
  toolchain.py list                     # every role/version and whether present
  toolchain.py linker 5.20              # -> absolute path to LINK.EXE
  toolchain.py compiler msc-6.0a        # -> absolute path to CL.EXE
  toolchain.py assembler masm-5.1
  toolchain.py rc 3.1
  toolchain.py lib win31-kit mdllcew
  toolchain.py check                    # report every gap
"""
import json, os, sys

# Resolve the version-keyed toolchain root from CodeBreaker config.json (msc6_toolchain).
# Fall back to this engine dir when run outside the package (config unavailable).
try:
    from codebreaker.msc6 import paths as _paths
    _HERE = _paths.TOOLCHAIN_ROOT or os.path.dirname(os.path.abspath(__file__))
    _MANIFEST = _paths.toolchain_manifest() if _paths.TOOLCHAIN_ROOT else \
        os.path.join(_HERE, "toolchain_manifest.json")
except Exception:
    _HERE = os.path.dirname(os.path.abspath(__file__))
    _MANIFEST = os.path.join(_HERE, "toolchain_manifest.json")


class ToolchainGap(Exception):
    pass


def load():
    return json.load(open(_MANIFEST))


def _abs(relpath):
    return os.path.normpath(os.path.join(_HERE, relpath))


# role -> the manifest key that holds the primary executable/artifact path
_PRIMARY = {"compiler": "cl", "assembler": "exe", "linker": "exe", "rc": "exe"}


def resolve(role, version, artifact=None):
    """Return the absolute path to a tool. `artifact` overrides which manifest
    key to return (e.g. resolve('lib','win31-kit','mdllcew'))."""
    m = load()
    if role not in m:
        raise ToolchainGap("unknown role %r (roles: %s)" % (role, ", ".join(k for k in m if isinstance(m[k], dict))))
    table = m[role]
    if version not in table:
        raise ToolchainGap("%s %r not in manifest (have: %s)" % (role, version, ", ".join(table)))
    entry = table[version]
    if entry.get("present") is False:
        raise ToolchainGap("%s %s is a KNOWN GAP: %s" % (role, version, entry.get("gap", "not present")))
    key = artifact or _PRIMARY.get(role)
    if key is None or key not in entry:
        # fall back to the directory if no specific artifact key
        if "dir" in entry:
            return _abs(entry["dir"])
        raise ToolchainGap("%s %s has no %r path in manifest" % (role, version, key))
    return _abs(entry[key])


def available(role, version):
    m = load()
    return role in m and version in m[role] and m[role][version].get("present") is not False


def gaps():
    return load().get("gaps", [])


def _cli():
    if len(sys.argv) < 2:
        print(__doc__); return 2
    cmd = sys.argv[1]
    m = load()
    if cmd == "list":
        for role in ("compiler", "assembler", "linker", "rc", "lib"):
            if role not in m:
                continue
            print("%s:" % role)
            for ver, e in m[role].items():
                flag = "GAP " if e.get("present") is False else "ok  "
                print("  [%s] %-14s %s" % (flag, ver, e.get("role", e.get("note", ""))[:70]))
        return 0
    if cmd == "check":
        g = gaps()
        print("gaps (%d):" % len(g))
        for x in g:
            print("  - " + x)
        return 0
    # resolve
    if len(sys.argv) < 3:
        print("usage: toolchain.py <role> <version> [artifact]"); return 2
    role, ver = sys.argv[1], sys.argv[2]
    artifact = sys.argv[3] if len(sys.argv) > 3 else None
    try:
        print(resolve(role, ver, artifact))
        return 0
    except ToolchainGap as e:
        print("GAP: %s" % e, file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(_cli())
