"""registry.py -- the active target, and the few modules a target is allowed to replace.

Most of CodeBreaker is the same whatever is being reconstructed. What differs per target -- where
its binaries are, how a module name becomes a path, which of its modules matter most -- lives in
one profile module, `codebreaker/targets/<name>/profile.py`. Shared code asks for it here:

    from codebreaker.base import registry
    target = registry.get("target")        # resolved on first use, so import order does not matter
    target.resolve("kernel32")

    registry.select("win95")               # done once, by the command line or a test

`get` also hands out a small set of shared modules by short name ("trace", "le", ...). Those
names are an indirection kept so a target can supply its own version of one; none does today.
"""
import importlib
import os
import sys

_SHARED = {
    "trace": "codebreaker.disasm.trace",
    "le": "codebreaker.formats.lefile",
    "parallel": "codebreaker.base.parallel",
    "progressui": "codebreaker.ui.progress_window",
}
DEFAULT_TARGET = "win95"

_active = None
_cache = {}


def select(name):
    """Make `name` (e.g. "win95") the active target."""
    global _active
    _active = name
    _cache.clear()


def active():
    """The active target's name; CB_TARGET, else the default, when none was selected."""
    return _active or os.environ.get("CB_TARGET") or DEFAULT_TARGET


def target_dir(name=None):
    here = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    return os.path.join(here, "targets", name or active())


def _load(name):
    if name == "target":
        return importlib.import_module("codebreaker.targets.%s.profile" % active())
    override = "codebreaker.targets.%s.%s" % (active(), name)
    if os.path.isfile(os.path.join(target_dir(), name + ".py")):
        return importlib.import_module(override)
    if name in _SHARED:
        return importlib.import_module(_SHARED[name])
    raise ImportError("registry.get(%r): not a target module and not a shared one" % name)


class _Lazy(object):
    """Stands in for a module until something is first asked of it."""

    def __init__(self, name):
        object.__setattr__(self, "_name", name)

    def _mod(self):
        key = (active(), self._name)
        if key not in _cache:
            _cache[key] = _load(self._name)
        return _cache[key]

    def __getattr__(self, attr):
        return getattr(self._mod(), attr)

    def __setattr__(self, attr, value):
        setattr(self._mod(), attr, value)

    def __repr__(self):
        return "<registry %s for %s>" % (self._name, active())


def get(name):
    return _Lazy(name)


# -- names the carried-over code still calls ---------------------------------------------------
def set_target(tool_dir):
    """Older spelling of select(): takes a tool directory (or several) and uses its name."""
    if not tool_dir:
        return select(None)
    d = tool_dir if isinstance(tool_dir, str) else tool_dir[0]
    select(os.path.basename(os.path.normpath(d)))


def ensure_target(tool_dir):
    if _active is None:
        set_target(tool_dir)


def active_target():
    return target_dir() if (_active or os.environ.get("CB_TARGET")) else None
