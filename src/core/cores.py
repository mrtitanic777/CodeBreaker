"""cores — the per-target override registry (replaces sys.path shadowing).

A handful of engine modules have per-era variants (the "quirk cores"). A shared
engine tool cannot `from src.engines.recon import cc16` and still get win1.0's
cc16 — explicit imports always resolve to the default. So those modules are
obtained through this registry instead:

    from src.core import cores
    cc16 = cores.get("cc16")          # active target's override, else the default

The active target is set once, by `wt`, before a command is dispatched:

    cores.set_target("engine/targets/win1.0")

Resolution is explicit and centralized (one place, testable) — not import-order
luck. A module with no active-target override falls back to its engine default;
one with neither raises a clear error.
"""
import importlib
import importlib.util
import os
import sys

# DI-critical modules -> their engine-default cluster (None = target-only: no default,
# it exists only in a target). `target` is THE injected dependency (the per-era profile).
_DEFAULT = {
    "target": None,
    "trace": "disasm", "segdiff": "engines.recon", "parallel": "core",
    "cc16": "engines.recon", "masmlift": "engines.recon", "cfg": "ir",
    "idord": "engines.symbol", "matchbatch": "engines.match", "asmrecon": "engines.recon",
    "refscan": "toolchain.reference", "asmrecon32": "engines.recon", "le": None,
}

_active_dir = None
_cache = {}


def set_target(tool_dir):
    """Point the registry at an active target's tool directory (or None)."""
    global _active_dir
    _active_dir = tool_dir
    _cache.clear()
    if tool_dir and tool_dir not in sys.path:
        sys.path.insert(0, tool_dir)     # so an override's own local siblings import


def _load(name):
    if _active_dir:
        p = os.path.join(_active_dir, name + ".py")
        if os.path.exists(p):
            spec = importlib.util.spec_from_file_location("cb_target_%s" % name, p)
            mod = importlib.util.module_from_spec(spec)
            spec.loader.exec_module(mod)
            return mod
    cluster = _DEFAULT.get(name)
    if cluster is None and name not in _DEFAULT:
        cluster = _find_default(name)
    if cluster:
        return importlib.import_module("src.%s.%s" % (cluster, name))
    raise ImportError("cores.get(%r): no target override and no engine default" % name)


class _Proxy:
    """Lazy module handle: resolution is deferred until first attribute use, so a
    module can be imported before a target is set — the target only has to be set
    by the time the dependency is actually used (wt sets it before dispatch)."""
    __slots__ = ("_cb_name", "_cb_mod")

    def __init__(self, name):
        object.__setattr__(self, "_cb_name", name)
        object.__setattr__(self, "_cb_mod", None)

    def _cb_resolve(self):
        m = object.__getattribute__(self, "_cb_mod")
        if m is None:
            m = _load(object.__getattribute__(self, "_cb_name"))
            object.__setattr__(self, "_cb_mod", m)
        return m

    def __getattr__(self, a):
        return getattr(self._cb_resolve(), a)

    def __setattr__(self, a, v):
        setattr(self._cb_resolve(), a, v)

    def __call__(self, *a, **k):
        return self._cb_resolve()(*a, **k)


def get(name):
    """A lazy handle to `name`: the active target's override if it has one, else the
    engine default. Resolution happens on first use, not here."""
    if name not in _cache:
        _cache[name] = _Proxy(name)
    return _cache[name]


def _find_default(name):
    """Best-effort: locate a not-pre-registered module in a cluster, returning its DOTTED path
    (e.g. 'engines.recon'). Walks the top-level groups and one level into engines/ and toolchain/."""
    from src.core import paths
    root = paths.package_root()                                         # src/
    for d in sorted(os.listdir(root)):
        if os.path.isfile(os.path.join(root, d, name + ".py")):
            return d
    for grp in ("engines", "toolchain"):
        gdir = os.path.join(root, grp)
        if os.path.isdir(gdir):
            for sub in sorted(os.listdir(gdir)):
                if os.path.isfile(os.path.join(gdir, sub, name + ".py")):
                    return "%s.%s" % (grp, sub)
    return None
