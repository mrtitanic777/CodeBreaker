"""settings.py -- where the things CodeBreaker does not contain are found on this machine.

CodeBreaker holds no binaries to reconstruct, no compiler and no results. Those live elsewhere
and are named in one file, `settings.json` at the top of the checkout:

    corpus          the folder of original binaries
    corpora         {name: folder} -- the original binaries of every product there is; the one
                    named by `target` is `corpus` unless it is listed here too
    openwatcom      the OpenWatcom release tree used to compile reconstructions
    powerpc_tools   folder holding a Microsoft C compiler and linker that target PowerPC
                    (cl.exe, link.exe); only needed to rebuild and judge PowerPC code
    powerpc_lib     that compiler's C runtime library file (its register save/restore helpers)
    period_compiler folder holding a 32-bit Microsoft C compiler of the original's time (CL.EXE),
                    for the optional byte-match check (`match`)
    decompiler_bin  folder of the native decompiler executables   (default: native/decompiler/build/bin)
    ghidra_core     folder holding cbdecomp.exe                    (default: native/ghidra_core/build/bin)
    state           where results and caches are kept             (default: state/)
    target          what is being reconstructed                   (default: win95)

A relative path is relative to the checkout. An environment variable CB_<KEY> (CB_CORPUS,
CB_OPENWATCOM, ...) overrides the file, for one-off runs.

    from codebreaker.base import settings
    settings.get("openwatcom")
    settings.state("results.sqlite")       # a path inside the state folder (created on demand)
    settings.corpora()                     # {name: folder} of every product's original binaries
"""
import json
import os
import sys


ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
FILE = os.path.join(ROOT, "settings.json")
DEFAULTS = {
    "corpus": os.path.join("..", "Windows95", "design", "corpus", "files"),
    "openwatcom": "",
    "decompiler_bin": os.path.join("native", "decompiler", "build", "bin"),
    "ghidra_core": os.path.join("native", "ghidra_core", "build", "bin"),
    "state": "state",
    "target": "win95",
}
_PATHS = ("corpus", "openwatcom", "decompiler_bin", "ghidra_core", "state", "powerpc_tools", "powerpc_lib", "period_compiler", "dosbox_x", "msc6", "xenix_c")
_loaded = None


def _load():
    global _loaded
    if _loaded is None:
        data = dict(DEFAULTS)
        try:
            with open(FILE, encoding="utf-8") as f:
                data.update(json.load(f))
        except FileNotFoundError:
            pass
        _loaded = data
    return _loaded


def get(key):
    """One setting. Paths come back absolute; "" means not set."""
    value = os.environ.get("CB_" + key.upper()) or _load().get(key, "")
    if value and key in _PATHS and not os.path.isabs(value):
        value = os.path.normpath(os.path.join(ROOT, value))
    return value


def corpora():
    """{name: folder} of every product's original binaries: the "corpora" setting, and the
    single "corpus" under the name in "target" when that name is not listed there."""
    out = {}
    for name, value in (_load().get("corpora") or {}).items():
        out[name] = value if os.path.isabs(value) else os.path.normpath(os.path.join(ROOT, value))
    out.setdefault(get("target"), get("corpus"))
    return out


def state(*parts):
    """A path inside the state folder, which is created if it is not there."""
    d = get("state")
    os.makedirs(d, exist_ok=True)
    return os.path.join(d, *parts)


def export_for_carried_over_code():
    """The modules carried over from the previous layout read their locations from environment
    variables. Fill in the ones not already set, from the settings. Goes away as each is changed
    to ask `settings` directly."""
    for env, key in (("CB_CORPUS_ROOT", "corpus"), ("CB_OPENWATCOM", "openwatcom"),
                     ("CB_DECOMPILER_BIN", "decompiler_bin")):
        if not os.environ.get(env) and get(key):
            os.environ[env] = get(key)
    os.environ.setdefault("CB_RESULTS_DB", state("results.sqlite"))


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
