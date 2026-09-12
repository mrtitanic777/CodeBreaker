#!/usr/bin/env python3
r"""paths.py -- the ONE canonical location/path resolver for CodeBreaker.

Before this module, every file computed the repo root by counting `os.path.dirname()` levels
from its own `__file__` -- and the count differed by how deeply the file was nested (2 levels in
foundation, 3 in sdk/inverse_engine, 4 in recompile95/tools). That made moving any file a silent
breakage risk. Everything that needs a root, the binaries corpus, a target's tool dir, or
config.json now imports this instead, so only THIS file knows the layout:

    from src.core import paths
    paths.repo_root()            # the CodeBreaker checkout (or the PyInstaller bundle when frozen)
    paths.package_root()         # the `engine/` package dir
    paths.data_root()            # the active data corpus (WT_DATA_ROOT); the tool holds no corpus copy
    paths.target_dir("win95")    # engine/targets/win95  (a per-era tool dir)
    paths.config_candidates()    # where config.json / CodeBreaker.config.json may live
    paths.data_root()            # the active data corpus (WT_DATA_ROOT / era dirs / cwd)
    paths.design_root()          # the Win95 'design' arena (docs + corpus + reference SDK/DDK)
    paths.reference("WIN32SDK")  # a path inside the period reference SDK/DDK trees

Computed once, here, from this file's own known location (engine/foundation/paths.py). Frozen
(PyInstaller) builds are handled: the bundled code lives in sys._MEIPASS.
"""
import os
import sys

_FROZEN = getattr(sys, "frozen", False) and hasattr(sys, "_MEIPASS")
_FOUND = os.path.dirname(os.path.abspath(__file__))     # engine/foundation
_ENGINE = os.path.dirname(_FOUND)                        # engine/


def package_root():
    """The `engine/` package directory (inside the PyInstaller bundle when frozen)."""
    return sys._MEIPASS if _FROZEN else _ENGINE          # noqa: SLF001 (documented PyInstaller attr)


def repo_root():
    """The CodeBreaker checkout root (where config.json / wt.py live). When frozen, the bundle."""
    return sys._MEIPASS if _FROZEN else os.path.dirname(_ENGINE)


def engine_dir(*parts):
    """A path under the engine package, e.g. engine_dir('targets', 'win95')."""
    return os.path.join(package_root(), *parts)


def target_dir(era):
    """A per-era tool directory: engine/targets/<era>."""
    return engine_dir("targets", era)


def binaries(*parts):
    """A path under the git-ignored data home: <repo>/binaries/<...>."""
    return os.path.join(repo_root(), "binaries", *parts)


def config_candidates():
    """Ordered paths where the external-tool config may live (first existing one wins)."""
    return [os.path.join(repo_root(), "config.json"),
            os.path.join(repo_root(), "CodeBreaker.config.json"),
            os.path.join(os.environ.get("WT_TOOLS_DIR", ""), "CodeBreaker.config.json")]


_ERAS = ("win95", "win98", "win3.1", "win1.0")


def data_root():
    """The active data corpus root. Mirrors wt.py: honor WT_DATA_ROOT when it actually holds era
    dirs (or when frozen), else the checkout's parent (the dev monorepo layout)."""
    env = os.environ.get("WT_DATA_ROOT")
    if _FROZEN:
        return env or os.getcwd()
    if env and any(os.path.isdir(os.path.join(env, e)) for e in _ERAS):
        return env
    if env and os.path.isdir(env):
        return env
    return os.path.dirname(repo_root())


def design_root():
    """The Win95 source-tree 'design' arena -- home of the architecture docs, the corpus, and the
    period reference SDK/DDK trees (Microsoft Windows 95 DDK, WIN32SDK). Honors WT_REFERENCE_ROOT;
    else derives it from the data corpus (the corpus lives at <design>/corpus, so the design arena
    is its parent); else falls back to the data root itself."""
    env = os.environ.get("WT_REFERENCE_ROOT")
    if env and os.path.isdir(env):
        return env
    dr = data_root()
    parent = os.path.dirname(dr)
    if os.path.basename(dr).lower() == "corpus" and os.path.isdir(parent):
        return parent
    return dr


def reference(*parts):
    """Resolve a path inside the period reference SDK/DDK trees (the DDK / WIN32SDK that were moved
    out of the tool into the Win95 'design' arena). Tries <design>/reference, then <design> itself,
    then the legacy in-tool binaries/media location; returns the first that exists, else the primary
    <design>/reference candidate (so callers get a sensible path to report when nothing is found)."""
    d = design_root()
    first = None
    for r in (os.path.join(d, "reference"), d, binaries("media")):
        p = os.path.join(r, *parts)
        if first is None:
            first = p
        if os.path.exists(p):
            return p
    return first
