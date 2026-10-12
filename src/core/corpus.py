#!/usr/bin/env python3
r"""corpus.py -- the ONE resolver for the retail binary corpus.

Finding "the retail kernel32.dll" used to be re-implemented per tool, and the copies disagreed:
one took the first file an os.walk happened to reach, another preferred WINDOWS\SYSTEM, and two
fell back to a hardcoded absolute path. The corpus holds several copies of many modules (the
installed one, the setup one, per-driver variants), so two tools could build against one binary
and verify against another. Everything resolves here now:

    from src.core import corpus
    corpus.root()                  # the corpus directory
    corpus.retail("kernel32")      # THE retail binary for a module name (installed copy first)
    corpus.candidates("lzexpand.dll")   # every copy, best first
    corpus.lane(path)              # 'pe' | 'ne' | 'le' | 'dos' | None, from the file header
    corpus.modules("ne")           # one path per module name in a lane, smallest first

The directory walk happens once per process and is cached.
"""
import os
import struct

from src.core import paths

_EXTS = (".dll", ".exe", ".drv", ".ocx", ".cpl", ".vxd", ".386", ".sys", ".com", ".acm", ".ax")
_index_cache = {}        # root -> {basename_lower: [paths, best first]}
_lane_cache = {}         # path -> lane


def root():
    """The retail corpus directory. CB_CORPUS_ROOT overrides; otherwise the sibling Windows95
    repo's design/corpus/files, then WT_DATA_ROOT when it is a directory."""
    env = os.environ.get("CB_CORPUS_ROOT")
    if env and os.path.isdir(env):
        return env
    sib = os.path.join(os.path.dirname(paths.repo_root()), "Windows95", "design", "corpus", "files")
    if os.path.isdir(sib):
        return sib
    env = os.environ.get("WT_DATA_ROOT")
    if env and os.path.isdir(env):
        return env
    return sib                                   # a sensible path to report when nothing exists


def design_dir():
    """The Win95 'design' arena that owns the corpus (…/Windows95/design)."""
    return os.path.dirname(os.path.dirname(root()))


def lane(path):
    """Executable class of a file by its header: 'pe', 'ne', 'le' (VxD), 'dos', or None when it
    is not an MZ executable."""
    if path in _lane_cache:
        return _lane_cache[path]
    out = None
    try:
        with open(path, "rb") as f:
            d = f.read(0x40)
            if len(d) >= 0x40 and d[:2] == b"MZ":
                f.seek(struct.unpack("<I", d[0x3c:0x40])[0])
                sig = f.read(2)
                out = {b"PE": "pe", b"NE": "ne", b"LE": "le", b"LX": "le"}.get(sig, "dos")
    except OSError:
        out = None
    _lane_cache[path] = out
    return out


def _rank(path):
    """Sort key putting the copy the OS actually loads first: WINDOWS\\SYSTEM, then WINDOWS, then
    anything else; ties broken by shorter path, then name, so the choice is deterministic."""
    parts = [p.lower() for p in path.replace("\\", "/").split("/")]
    tier = 0 if "system" in parts else (1 if "windows" in parts else 2)
    return (tier, len(parts), path.lower())


def index(refresh=False):
    """{basename_lower: [paths, best first]} for the whole corpus (walked once, cached)."""
    r = root()
    if refresh or r not in _index_cache:
        idx = {}
        for dp, _dirs, files in os.walk(r):
            for fn in files:
                idx.setdefault(fn.lower(), []).append(os.path.join(dp, fn))
        for name in idx:
            idx[name].sort(key=_rank)
        _index_cache[r] = idx
    return _index_cache[r]


def candidates(name):
    """Every corpus copy of a module, best first. A bare name tries the usual extensions."""
    name = os.path.basename(name).lower()
    idx = index()
    if name in idx:
        return list(idx[name])
    if "." not in name:
        out = []
        for ext in _EXTS:
            out += idx.get(name + ext, [])
        return sorted(out, key=_rank)
    return []


def retail(name):
    """THE retail binary for a module name, or None. An existing path is returned unchanged, so
    callers can pass either a name or a path."""
    if os.path.isfile(name):
        return name
    c = candidates(name)
    return c[0] if c else None


def modules(want_lane=None, exts=(".dll", ".exe", ".drv", ".ocx", ".cpl")):
    """One path per module name (its retail() copy), optionally restricted to a lane; smallest
    file first so a sweep gives early feedback."""
    out = []
    for name, cands in index().items():
        if not name.endswith(tuple(exts)):
            continue
        p = cands[0]
        if want_lane is None or lane(p) == want_lane:
            out.append(p)
    return sorted(out, key=lambda p: (os.path.getsize(p), p.lower()))
