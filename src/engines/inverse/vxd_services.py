#!/usr/bin/env python3
r"""vxd_services.py -- resolve VxD service ordinals to their DDK names.

A VxD service call is `int 20h` + a DWORD whose high word is the callee's device id and whose low
word is the service ORDINAL -- the service's position in that device's service table. The Win95
DDK headers declare those tables in order:

    Begin_Service_Table VMM, VMM
    VMM_Service  Get_VMM_Version, LOCAL     ; ordinal 0
    VMM_Service  Get_Cur_VM_Handle          ; ordinal 1
    ...

so parsing every `Begin_Service_Table <DEV> ... <DEV>_Service <name>` run recovers a
{device: [name-by-ordinal]} map. That turns `VMMCall VMM.Service_003Fh` into
`VMMCall Get_System_Time` and names each driver's own exported service handlers.

    from src.engines.inverse import vxd_services
    S = vxd_services.load()          # autodiscovers the DDK under binaries/
    S.name("VMM", 0x3f)              # -> "Get_System_Time"
"""
import os
import re
import glob
import functools
from src.core import paths

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(HERE))

def _candidates():
    dr = paths.data_root()
    return [
        paths.reference("Microsoft Windows 95 DDK", "INC32"),   # the moved reference SDK/DDK tree
        # the sibling Win95 source repo (data_root is the checkouts' parent), tried directly so the
        # bounded fallback below is not needed in the normal layout
        os.path.join(dr, "Windows95", "design", "reference", "Microsoft Windows 95 DDK", "INC32"),
        dr + "/docs/reference/win95ddk/INC32",
        dr + "/docs/reference/loganxp/win95ddk/INC32",
    ]


# directories never worth descending into when searching for the DDK headers
_PRUNE = {".git", ".hg", ".svn", "node_modules", "__pycache__", ".venv", "venv",
          "open-watcom-v2", "binaries", "corpus"}


def _find_inc_dir():
    for p in _candidates():
        if p and os.path.isdir(p):
            return p
    # Bounded fallback. The old code did glob("**", recursive=True) over design_root(),
    # which -- because data_root()/design_root() default to the PARENT of the checkout --
    # crawled every sibling repo on disk and appeared to hang. Walk a shallow depth only,
    # pruning VCS/build/corpus dirs, so a missing config degrades gracefully instead of
    # scanning the whole tree.
    root = paths.design_root()
    if not root or not os.path.isdir(root):
        return None
    base = root.rstrip(os.sep).count(os.sep)
    for dirpath, dirs, files in os.walk(root):
        if dirpath.count(os.sep) - base >= 4:      # cap depth
            dirs[:] = []
            continue
        dirs[:] = [d for d in dirs if d not in _PRUNE and not d.startswith(".")]
        if os.path.basename(dirpath).upper() == "INC32" and "VMM.INC" in files:
            return dirpath
    return None


@functools.lru_cache(maxsize=8)
def _parse(inc_dir):
    tables = {}
    for fp in glob.glob(os.path.join(inc_dir, "*.INC")):
        try:
            txt = open(fp, encoding="latin1").read()
        except OSError:
            continue
        for m in re.finditer(r"Begin_Service_Table\s+(\w+)(.*?)(?:End_Service_Table|\Z)",
                             txt, re.S | re.I):
            dev = m.group(1).upper()
            names = re.findall(r"^\s*" + re.escape(dev) + r"_Service\s+(\w+)",
                              m.group(2), re.M | re.I)
            if names and dev not in tables:                # first (canonical) table wins
                tables[dev] = names
    return tables


class Services:
    def __init__(s, tables):
        s.tables = tables

    def names(s, device):
        return s.tables.get((device or "").upper())

    def name(s, device, ordinal):
        """Service name for (device, ordinal), or None. The low 15 bits are the table index; the
        top bit is a call-convention flag on some references, so try the masked ordinal too."""
        names = s.names(device)
        if not names or ordinal is None:
            return None
        for o in (ordinal, ordinal & 0x7FFF):
            if 0 <= o < len(names):
                return names[o]
        return None


def load(inc_dir=None):
    inc_dir = inc_dir or _find_inc_dir()
    return Services(_parse(inc_dir) if inc_dir else {})


if __name__ == "__main__":
    S = load()
    print("service tables:", {k: len(v) for k, v in sorted(S.tables.items())})
    for dev, ordn in (("VMM", 0x3f), ("VMM", 0), ("VKD", 0), ("VPICD", 0)):
        print("  %-8s [0x%02x] = %s" % (dev, ordn, S.name(dev, ordn)))
