#!/usr/bin/env python3
r"""layout.py — the single source of truth for a target's data-dir tree.

The LEGACY layout kept the whole reconstruction under one dir:
    reconstructed/asm/        byte-exact per-module .asm
    reconstructed/auto/       [AUTO] machine-C scaffolding
    reconstructed/*.c         hand-written C  (+ system/ from95/ ... subdirs)
    reconstructed/auto_lift/  (win1.0) the masm-lift staging

The PUBLISHABLE layout sorts these by KIND into a clean, browsable tree:
    asm/                          byte-exact per-module .asm
    src/generated/                [AUTO] machine-C scaffolding
    src/reconstructed/            hand-written C  (+ system/ from95/ ... subdirs)
    src/reconstructed/auto_lift/  (win1.0)

Migration is PER-TARGET and gradual: a target is "migrated" once it has a top-level
src/ dir. Un-migrated targets keep the legacy paths, so the SHARED engine serves both
during the transition (win95 migrates first, then the rest). Every tool MUST route
data-dir paths through these accessors — never hardcode "reconstructed/..." again.

Paths are relative to the current working directory, which `wt` sets to the target's
data root before invoking any tool; call the accessors at USE time, not import time.
"""
import glob as _glob
import os

# `base` is the data-root prefix. Default "" means paths are relative to the current
# working directory (which `wt` sets to the target's data root). Callers that hold an
# ABSOLUTE data root (e.g. goldens.py) pass it explicitly.


def _base(base):
    """The data-root prefix. Explicit base wins; else $WT_DATA_ROOT (so STANDALONE tools resolve
    data paths regardless of the current directory -- the class of bug where a tool run from the
    wrong cwd silently found nothing); else '' = relative to cwd, which `wt` sets to the data root.
    Backward-compatible: with WT_DATA_ROOT unset this is exactly the old cwd-relative behavior."""
    return base or os.environ.get("WT_DATA_ROOT", "")


def migrated(base=""):
    """True once this data dir uses the clean (published) layout."""
    return os.path.isdir(os.path.join(_base(base), "src"))


# --- machine-generated [AUTO] C ------------------------------------------
def generated_dir(base=""):
    base = _base(base)
    return os.path.join(base, "src", "generated") if migrated(base) else os.path.join(base, "reconstructed", "auto")


# --- hand-written C -------------------------------------------------------
def recon_dir(base=""):
    """Root of the hand-written reconstruction C."""
    base = _base(base)
    return os.path.join(base, "src", "reconstructed") if migrated(base) else os.path.join(base, "reconstructed")


_TREE_CACHE = {}                       # {abs map path: {kind/filename: component}}


def _src_root(base=""):
    return os.path.dirname(generated_dir(base))          # .../src


def has_tree(base=""):
    """True once this target's src/ has been sorted into the component tree."""
    return os.path.exists(os.path.join(_src_root(base), "TREE.map"))


def tree_map(base=""):
    """{'generated/kernel32.dll.c': 'core', 'reconstructed/gdi_seg01.c': 'gdi', ...}
    parsed from src/TREE.map, or {} if this target has no component tree. Cached."""
    path = os.path.join(_src_root(base), "TREE.map")
    if not os.path.exists(path):
        return {}
    if path not in _TREE_CACHE:
        m = {}
        try:
            with open(path, encoding="utf-8") as f:
                for line in f:
                    if line.startswith("#") or "\t" not in line:
                        continue
                    parts = line.rstrip("\n").split("\t")
                    if len(parts) >= 2:
                        m[parts[0]] = parts[1]
        except OSError:
            return {}
        _TREE_CACHE[path] = m
    return _TREE_CACHE[path]


def module_path(filename, kind="generated", base=""):
    """The path a per-module source file resolves to, honoring the component tree.
    `filename` e.g. 'kernel32.dll.c'; `kind` 'generated' or 'reconstructed'. Falls
    back to the flat root when there's no tree or the file isn't mapped yet."""
    root = generated_dir(base) if kind == "generated" else recon_dir(base)
    comp = tree_map(base).get("%s/%s" % (kind, filename), "")
    return os.path.join(root, comp, filename) if comp else os.path.join(root, filename)


# --- cross-check reference material (DDK/SDK/symbols/toolchain) ------------
def reference_dir(base=""):
    """Reference/cross-check material. Merged under docs/ where a target has done
    so (win95); still top-level `reference/` elsewhere. Existence-based (NOT tied to
    the src/ migration), so each target resolves independently."""
    d = os.path.join(base, "docs", "reference")
    return d if os.path.isdir(d) else os.path.join(base, "reference")


def reference_sub(name, base=""):
    """A named subdir/file of the reference tree (ddk, sdk, midage, reftree,
    win95ddk, toolchain, vmm_services.txt, ...)."""
    return os.path.join(reference_dir(base), name)


def runtime_dir(base=""):
    """The tools' machine RUNTIME output dir (conduct ledgers, caches, live progress).
    Merged under docs/ where a target has done so (win95: `docs/runtime/`); still top-level
    `reports/` elsewhere. Existence-based, so each target resolves independently and the
    un-migrated targets are unaffected. Distinct from `docs/reports/` (human-written
    dossiers) -- this is regenerable tool output, not documentation."""
    d = os.path.join(base, "docs", "runtime")
    return d if os.path.isdir(d) else os.path.join(base, "reports")
