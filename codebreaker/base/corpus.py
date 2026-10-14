#!/usr/bin/env python3
r"""corpus.py -- the ONE resolver for the retail binary corpus.

Finding "the retail kernel32.dll" used to be re-implemented per tool, and the copies disagreed:
one took the first file an os.walk happened to reach, another preferred WINDOWS\SYSTEM, and two
fell back to a hardcoded absolute path. The corpus holds several copies of many modules (the
installed one, the setup one, per-driver variants), so two tools could build against one binary
and verify against another. Everything resolves here now:

    from codebreaker.base import corpus
    corpus.root()                  # the corpus directory
    corpus.retail("kernel32")      # THE retail binary for a module name (installed copy first)
    corpus.candidates("lzexpand.dll")   # every copy, best first
    corpus.lane(path)              # 'pe' | 'ne' | 'le' | 'dos' | None, from the file header
    corpus.modules("ne")           # one path per module name in a lane, smallest first

There is more than one product. settings.json lists them under "corpora" ({name: folder}); one
is selected at a time and everything above answers for that one. Nothing selected means the
product settings.json names as "target", found exactly as it always was.

    corpus.names()                 # every product there is a folder for
    corpus.current()               # the selected one's name
    corpus.use("win98")            # select one (for this process and the workers it starts)
    with corpus.selected("win3.1"): corpus.retail("user.exe")       # for a moment
    corpus.root("win98")           # a product's folder without selecting it

The environment variable CB_CORPUS_NAME selects one for a whole run, and so does the global
option `--corpus NAME` (take_option). Each product's directory walk happens once per process
and is cached.
"""
import contextlib
import hashlib as _hashlib
import os

from codebreaker.base import settings as paths

_EXTS = (".dll", ".exe", ".drv", ".ocx", ".cpl", ".vxd", ".386", ".sys", ".com", ".acm", ".ax")
_index_cache = {}        # root -> {basename_lower: [paths, best first]}
_lane_cache = {}         # path -> lane


ENV = "CB_CORPUS_NAME"       # selects a product for a whole run; use() sets it for workers
_selected = None


def names():
    """Every product that has a folder of original binaries, by name."""
    return sorted(paths.corpora())


def current():
    """The selected product's name."""
    return _selected or os.environ.get(ENV) or paths.get("target")


def use(name):
    """Select a product: every function here then answers for it, in this process and in the
    worker processes it starts. None goes back to the default. -> the name selected before"""
    global _selected
    before = current()
    if name is not None and name not in paths.corpora():
        raise ValueError("no corpus %r (settings.json lists: %s)" % (name, ", ".join(names())))
    _selected = name
    if name is None:
        os.environ.pop(ENV, None)
    else:
        os.environ[ENV] = name
    return before


@contextlib.contextmanager
def selected(name):
    """`with corpus.selected("win98"): ...` -- another product for the length of a block."""
    was_var, was_env = _selected, os.environ.get(ENV)
    use(name)
    try:
        yield
    finally:
        use(was_var)
        if was_env is not None:
            os.environ[ENV] = was_env


def take_option(argv):
    """Act on a `--corpus NAME` in a command line and return the line without it."""
    argv = list(argv)
    if "--corpus" in argv:
        i = argv.index("--corpus")
        if i + 1 >= len(argv):
            raise SystemExit("--corpus needs a name: %s" % ", ".join(names()))
        use(argv[i + 1])
        del argv[i:i + 2]
    return argv


def root(name=None):
    """The folder of original binaries: of the product `name`, else of the selected one. For
    the default product that is CB_CORPUS_ROOT when set; otherwise the sibling Windows95
    repo's design/corpus/files, then WT_DATA_ROOT when it is a directory."""
    name = name or _selected or os.environ.get(ENV)
    listed = paths._load().get("corpora") or {}
    if name and (name in listed or name != paths.get("target")):
        try:
            return paths.corpora()[name]
        except KeyError:
            raise ValueError("no corpus %r (settings.json lists: %s)" % (name, ", ".join(names())))
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
    """The Win95 'design' arena that owns the corpus (…/Windows95/design). A product other than
    the default has no such arena beside its binaries: what is derived for it is kept in the
    state folder, under products/<name>/design."""
    name = _selected or os.environ.get(ENV)
    if name and name != paths.get("target"):
        return os.path.join(paths.get("state"), "products", name, "design")
    return os.path.dirname(os.path.dirname(root()))


def reference(*parts):
    """A path inside the period SDK and DDK folders (settings "reference", else the folder of
    that name in the design arena): reference("Microsoft Windows 95 DDK", "INC32")."""
    return os.path.join(paths.get("reference") or os.path.join(design_dir(), "reference"), *parts)


def owner(path):
    """The name of the product whose folder holds `path` (the deepest one), or None."""
    path = os.path.normcase(os.path.abspath(path))
    best = None
    for name in names():
        r = os.path.normcase(os.path.abspath(root(name)))
        if path.startswith(r + os.sep) and (best is None or len(r) > len(best[1])):
            best = (name, r)
    return best[0] if best else None


def lane(path):
    """Executable class of a file by its header: 'pe', 'ne', 'le' (VxD), 'dos', or None when it
    is not an MZ executable."""
    if path in _lane_cache:
        return _lane_cache[path]
    from codebreaker.formats import detect
    out = detect.lane(path)
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
        _unpack_boot_images(idx)
        _index_cache[r] = idx
    return _index_cache[r]


def _unpack_boot_images(idx):
    """Windows 1.x keeps its system modules (KERNEL, GDI, USER, the drivers) inside one boot
    image, a .BIN with an .OVL beside it, not in files of their own. Each is written out once
    as an ordinary executable under the state folder and listed as <module>.exe, so a module
    of such a product is found, read and reconstructed like any other."""
    for name in [n for n in idx if n.endswith(".bin") and n[:-4] + ".ovl" in idx]:
        image, overlay = idx[name][0], idx[name[:-4] + ".ovl"][0]
        if lane(image) != "ne":
            continue
        from codebreaker.formats import ne
        out = os.path.join(paths.get("state"), "unpacked", "%s-%d" % (name[:-4], os.path.getsize(image)))
        try:
            modules = ne.open_boot_image(image, overlay)
        except (OSError, ValueError):
            continue
        os.makedirs(out, exist_ok=True)
        for module, nef in modules.items():
            path = os.path.join(out, module.lower() + ".exe")
            if not os.path.isfile(path):
                with open(path, "wb") as f:
                    f.write(ne.standalone(nef))
            have = idx.setdefault(module.lower() + ".exe", [])
            if not any(lane(p) for p in have):               # (MSDOS.EXE on the disk is a 1-byte stub)
                have.insert(0, path)


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


def resolve(name):
    """The file a module is named by: a path as it stands, otherwise the corpus's copy of that
    module name."""
    if os.path.isfile(name):
        return name
    found = retail(name)
    if not found:
        raise ValueError("no module %r in the corpus" % name)
    return found


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


def executables():
    """[(path, kind)] -- every file of the selected product that is an executable, whatever it
    is called (fonts, drivers, overlays): kind is 'pe' | 'ne' | 'le' | 'dos'. Every copy."""
    out = []
    for cands in index().values():
        for p in cands:
            kind = lane(p)
            if kind:
                out.append((p, kind))
    return sorted(out)


_SHA_MEMO = {}


def sha(path):
    """sha256 of a module file, memoized by (path, mtime, size) so a stable corpus binary is hashed
    once per process."""
    try:
        st = os.stat(path)
    except OSError:
        return None
    key = (path, int(st.st_mtime), st.st_size)
    s = _SHA_MEMO.get(key)
    if s is None:
        h = _hashlib.sha256()
        try:
            with open(path, "rb") as f:
                for chunk in iter(lambda: f.read(1 << 20), b""):
                    h.update(chunk)
        except OSError:
            return None
        s = h.hexdigest()
        _SHA_MEMO[key] = s
    return s
