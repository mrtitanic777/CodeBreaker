#!/usr/bin/env python3
r"""namestore.py -- the corpus symbol store: set a name once, it propagates everywhere.

XEXMagic keeps a per-program .xdb ({functions,vars,labels}) loaded on start and saved on every
rename, and because its renderer looks names up by address at every reference, one rename shows at
all sites. CodeBreaker adds the cross-MODULE layer XEXMagic lacks: body_ledger gives each unique
function body a structural identity (body_id) shared across every module it is linked into. So a name
keyed by body_id propagates to ALL modules that contain that body -- not just one program.

Store (design/namestore.json):
    { "by_body":   { "<body_id>": "Name", ... },          # propagates corpus-wide (shared identity)
      "by_module": { "<module>": { "<addr>": "Name" } } }  # a module-local override for a unique body

The emitters (dosrecon/authsrc/nerecon) call resolve(module, addr, body_id, default) before naming a
function; a curated name (by_body first, then by_module) wins over the engine's inferred name, so any
name -- hand-curated or engine-derived -- is applied once and rendered everywhere that body appears.

    wt --target win95 namestore --stats
    wt --target win95 namestore --set-body <body_id> <Name>       # propagate to all modules
    wt --target win95 namestore --set <module> <addr> <Name>      # one module only
    wt --target win95 namestore --list [substr]
    wt --target win95 namestore --bodyid <module-path> <addr>     # look up a body_id
"""
import os
import re
import sys
import json

from src.core import paths


def _win95_design():
    dr = paths.design_root()
    if os.path.isdir(os.path.join(dr, "corpus")):
        return dr
    sib = os.path.join(os.path.dirname(paths.repo_root()), "Windows95", "design")
    return sib if os.path.isdir(os.path.join(sib, "corpus")) else dr


def _path():
    return os.path.join(_win95_design(), "namestore.json")


def load():
    p = _path()
    if os.path.isfile(p):
        try:
            d = json.load(open(p, encoding="utf-8"))
        except Exception:                                    # noqa: BLE001
            d = {}
    else:
        d = {}
    d.setdefault("by_body", {})
    d.setdefault("by_module", {})
    return d


def save(d):
    os.makedirs(os.path.dirname(_path()), exist_ok=True)
    json.dump(d, open(_path(), "w", encoding="utf-8"), indent=1, sort_keys=True)


# in-process cache so the emitters don't re-read the file per function
_CACHE = None


def _store():
    global _CACHE
    if _CACHE is None:
        _CACHE = load()
    return _CACHE


def resolve(module, addr, body_id=None, default=None):
    """The curated name for a function: by_body (cross-module) wins, then by_module, else default."""
    s = _store()
    if body_id and body_id in s["by_body"]:
        return s["by_body"][body_id]
    m = s["by_module"].get((module or "").lower())
    if m:
        key = addr if isinstance(addr, str) else ("0x%X" % addr)
        if key in m:
            return m[key]
        if isinstance(addr, int) and ("%X" % addr) in m:
            return m["%X" % addr]
    return default


def set_body(body_id, name):
    d = load(); d["by_body"][body_id] = name; save(d)
    global _CACHE; _CACHE = None


def set_local(module, addr, name):
    d = load()
    m = d["by_module"].setdefault(module.lower(), {})
    m[addr if isinstance(addr, str) else ("0x%X" % addr)] = name
    save(d)
    global _CACHE; _CACHE = None


# --- body_id resolution (reuse origintriage's structural hash) -------------------------------
def bodyid_map(path):
    """{addr(int-or-normalised) -> body_id(16hex)} for a module, via origintriage.triage_module.
    Cached under design/bodyid/<module>.json (triage is a full disasm; cache so emitters are cheap)."""
    cache = os.path.join(_win95_design(), "bodyid", os.path.basename(path).lower() + ".json")
    if os.path.isfile(cache) and os.path.getmtime(cache) >= os.path.getmtime(path):
        try:
            return {int(k): v for k, v in json.load(open(cache)).items()}
        except Exception:                                    # noqa: BLE001
            pass
    from src.targets.win95 import origintriage
    lane, funcs = origintriage.triage_module(path)
    out = {}
    for f in (funcs or []):
        a = f.get("addr")
        if a is not None:
            out[int(a)] = f["sh"][:16]
    os.makedirs(os.path.dirname(cache), exist_ok=True)
    json.dump({str(k): v for k, v in out.items()}, open(cache, "w"))
    return out


def seed_from_ledger(min_modules=2):
    """Lever 2: seed by_body from body_ledger.csv -- for each SHARED body (in >= min_modules), take a
    meaningful example_name as the canonical name so it propagates to every module with that body.
    Skips placeholder names (sub_/data_/helper_/thunk_) and bodies already curated. Returns count."""
    import csv
    led = os.path.join(_win95_design(), "body_ledger.csv")
    if not os.path.isfile(led):
        return 0
    d = load()
    n = 0
    for r in csv.DictReader(open(led, encoding="utf-8", errors="replace")):
        try:
            inm = int(r.get("in_modules") or 0)
        except ValueError:
            continue
        nm = (r.get("example_name") or "").strip()
        bid = r.get("body_id")
        if (inm >= min_modules and nm and bid and bid not in d["by_body"]
                and not nm.lower().startswith(("sub_", "data_", "seg_", "helper_", "thunk_", "loc_"))):
            d["by_body"][bid] = nm
            n += 1
    save(d)
    global _CACHE; _CACHE = None
    return n


def main(argv):
    if not argv:
        print(__doc__); return 0
    a = argv[0]
    if a == "--stats":
        s = load()
        print("namestore: %d body-keyed names (propagate corpus-wide), %d module-local names in %d modules"
              % (len(s["by_body"]), sum(len(v) for v in s["by_module"].values()), len(s["by_module"])))
        print("  -> %s" % _path())
    elif a == "--seed-ledger":
        mm = int(argv[1]) if len(argv) > 1 else 2
        n = seed_from_ledger(mm)
        print("seeded %d shared-body names from body_ledger (in >= %d modules) -> propagate corpus-wide" % (n, mm))
    elif a == "--set-body" and len(argv) >= 3:
        set_body(argv[1], argv[2]); print("set by_body[%s] = %s (propagates to every module with this body)" % (argv[1], argv[2]))
    elif a == "--set" and len(argv) >= 4:
        set_local(argv[1], argv[2], argv[3]); print("set by_module[%s][%s] = %s" % (argv[1], argv[2], argv[3]))
    elif a == "--list":
        s = load(); sub = argv[1] if len(argv) > 1 else ""
        for bid, nm in sorted(s["by_body"].items()):
            if sub in nm or sub in bid:
                print("  body %s -> %s" % (bid, nm))
        for mod, mm in sorted(s["by_module"].items()):
            for k, nm in sorted(mm.items()):
                if sub in nm or sub in mod:
                    print("  %s:%s -> %s" % (mod, k, nm))
    elif a == "--bodyid" and len(argv) >= 3:
        m = bodyid_map(argv[1])
        addr = int(argv[2], 16) if argv[2].lower().startswith("0x") else int(argv[2], 16)
        print("body_id(%s @ %s) = %s" % (os.path.basename(argv[1]), argv[2], m.get(addr, "<not found>")))
    else:
        print(__doc__)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
