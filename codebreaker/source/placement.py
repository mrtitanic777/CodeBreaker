"""placement.py -- where a module's source goes in the source tree.

    python -m codebreaker place <module> ...      where each would go, and why
    python -m codebreaker place --all             every module of the corpus (nothing is written)

    from codebreaker.source import placement
    placement.resolve("comctl32.dll", path)      ("shell/comctl32", "known")
    placement.folder("comctl32.dll", path)       the folder itself (created on demand)

There is one source tree per product: the folder that holds the corpus's `design` folder. A
module's source lives in <tree>/<component>/<module>/. Where a module already has a folder
anywhere in the tree, that is where it goes -- someone put it there. Otherwise the component is
worked out, most certain first:

    known      the module is named in the product's table (the system's core modules)
    kind       what the file is settles it: a virtual device driver, a real-mode program
    installed  the folder it is installed in settles it (I/O subsystem drivers, COMMAND)
    suffix     its extension does (.acm, .cpl, .scr, .drv)
    prefix     its name starts like a family the table knows (mci*, ras*, ole*, ...)
    unsorted   nothing above applies

The tables are per product, in placement.json beside this file.
"""
import json
import os
import re

from codebreaker.base import corpus

_SKIP = {"design", "build", ".git", "tools", "inc"}
_tables = None
_existing = {}


def tree_root():
    """The source tree of the selected product."""
    return os.path.dirname(corpus.design_dir())


def _table():
    global _tables
    if _tables is None:
        with open(os.path.join(os.path.dirname(os.path.abspath(__file__)), "placement.json"), encoding="utf-8") as f:
            _tables = json.load(f)
    return _tables.get(corpus.current()) or {"known": {}, "prefix": [], "suffix": {}, "installed": {}, "kind": {}}


def existing(root=None):
    """{module stem: folder relative to the tree} for every module folder already there."""
    root = root or tree_root()
    if root not in _existing:
        found = {}
        for top in sorted(os.listdir(root)) if os.path.isdir(root) else ():
            base = os.path.join(root, top)
            if top in _SKIP or top.startswith(".") or not os.path.isdir(base):
                continue
            for name in sorted(os.listdir(base)):
                if os.path.isdir(os.path.join(base, name)):
                    found.setdefault(name.lower(), "%s/%s" % (top, name))
        _existing[root] = found
    return _existing[root]


def resolve(module, path=""):
    """-> (folder relative to the tree, how it was decided)."""
    name = os.path.basename(module).lower()
    stem = re.sub(r"\.[^.]+$", "", name)
    here = existing().get(stem)
    if here:
        return here, "already there"
    t = _table()
    if stem in t["known"]:
        return t["known"][stem], "known"
    kind = corpus.lane(path) if path else None
    if kind in t["kind"]:
        return "%s/%s" % (t["kind"][kind], stem), "kind"
    where = (path or "").replace("\\", "/").lower()
    for part, component in t["installed"].items():
        if part in where:
            return "%s/%s" % (component, stem), "installed"
    ext = os.path.splitext(name)[1]
    if ext in t["suffix"]:
        return "%s/%s" % (t["suffix"][ext], stem), "suffix"
    for prefix, component in t["prefix"]:
        if stem.startswith(prefix):
            return "%s/%s" % (component, stem), "prefix"
    return "unsorted/%s" % stem, "unsorted"


def folder(module, path="", create=True):
    """The folder a module's source goes in."""
    rel, _how = resolve(module, path)
    d = os.path.join(tree_root(), *rel.split("/"))
    if create:
        os.makedirs(d, exist_ok=True)
    return d


def main(argv):
    """python -m codebreaker place <module> ... | --all

    Say where each module's source goes in the source tree, and how that was decided. Nothing
    is written."""
    names = [a for a in argv if not a.startswith("--")]
    if "--all" in argv:
        from codebreaker.formats import dos
        paths = [p for k in ("pe", "ne") for p in corpus.modules(k)] + corpus.modules("le", exts=(".vxd", ".386")) + \
            list(dos.programs())
    else:
        paths = [corpus.retail(n) or n for n in names]
    if not paths:
        print(main.__doc__)
        return 2
    how_many = {}
    for p in paths:
        rel, how = resolve(os.path.basename(p), p if os.path.isfile(p) else "")
        how_many[how] = how_many.get(how, 0) + 1
        if "--all" not in argv or "--list" in argv:
            print("%-16s %-32s %s" % (os.path.basename(p).lower(), rel, how))
    print("%d modules: %s" % (len(paths), ", ".join("%s %d" % kv for kv in sorted(how_many.items(), key=lambda kv: -kv[1]))))
    return 0
