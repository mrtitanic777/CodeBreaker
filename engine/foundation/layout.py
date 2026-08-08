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


def migrated(base=""):
    """True once this data dir uses the clean (published) layout."""
    return os.path.isdir(os.path.join(base, "src"))


# --- byte-exact asm -------------------------------------------------------
def asm_dir(base=""):
    """The byte-exact asm root. Migrated targets keep it at top-level `asm/`, EXCEPT
    where it's been pulled under the source tree at `src/asm/` (win95) — existence-based
    and per-target, so each target resolves independently during the transition."""
    if migrated(base):
        nested = os.path.join(base, "src", "asm")
        return nested if os.path.isdir(nested) else os.path.join(base, "asm")
    return os.path.join(base, "reconstructed", "asm")


# --- machine-generated [AUTO] C ------------------------------------------
def generated_dir(base=""):
    return os.path.join(base, "src", "generated") if migrated(base) else os.path.join(base, "reconstructed", "auto")


# --- hand-written C -------------------------------------------------------
def recon_dir(base=""):
    """Root of the hand-written reconstruction C."""
    return os.path.join(base, "src", "reconstructed") if migrated(base) else os.path.join(base, "reconstructed")


def recon_sub(name, base=""):
    """A named subdir of the reconstruction root (system, from95, auto_lift,
    segmaps, graftmaps, skeletons, relinked, ...)."""
    return os.path.join(recon_dir(base), name)


def written_c_globs(base=""):
    """Every hand-written-C source, across the provenance-segregated subdirs
    (top level = apps, system/ = system modules, from95/ = win98 carryover)."""
    r = recon_dir(base)
    return [os.path.join(r, "*.c"),
            os.path.join(r, "system", "*.c"),
            os.path.join(r, "from95", "*.c")]


# --- component source tree (src/TREE.map) --------------------------------
# Once a target's src/ is sorted into a real Windows component hierarchy
# (core/gdi/user/net/dev/... — see srctree.py), the machine-C and hand-C no
# longer live flat: they sit under src/{generated,reconstructed}/<component>/.
# TREE.map (module-file -> component) marks that state and is the authority for
# WHERE each file goes. Targets without a TREE.map keep the flat layout, so all
# of this is a no-op for them — the file listers fall back to the flat globs.
_TREE_SKIP = {"auto_lift", "segmaps", "graftmaps", "skeletons", "relinked",
              "decomp_cache"}          # non-source staging dirs — never source
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


def _walk_c(root):
    """Every *.c under `root`, recursively, skipping non-source staging subdirs."""
    out = []
    for dp, dns, fns in os.walk(root):
        dns[:] = [d for d in dns if d not in _TREE_SKIP]
        out += [os.path.join(dp, fn) for fn in fns if fn.endswith(".c")]
    return out


def generated_c_files(base=""):
    """Every machine-generated [AUTO] .c — recursively when the component tree is
    active, flat otherwise. Use this instead of glob(generated_dir()/*.c)."""
    root = generated_dir(base)
    if not os.path.isdir(root):
        return []
    files = _walk_c(root) if has_tree(base) else _glob.glob(os.path.join(root, "*.c"))
    return sorted(files)


def written_c_files(base=""):
    """Every hand-written .c — recursively over the component tree when active, else
    the legacy provenance-subdir globs. Use instead of expanding written_c_globs()."""
    if has_tree(base):
        return sorted(_walk_c(recon_dir(base)))
    return sorted(sum((_glob.glob(g) for g in written_c_globs(base)), []))


def module_path(filename, kind="generated", base=""):
    """The path a per-module source file resolves to, honoring the component tree.
    `filename` e.g. 'kernel32.dll.c'; `kind` 'generated' or 'reconstructed'. Falls
    back to the flat root when there's no tree or the file isn't mapped yet."""
    root = generated_dir(base) if kind == "generated" else recon_dir(base)
    comp = tree_map(base).get("%s/%s" % (kind, filename), "")
    return os.path.join(root, comp, filename) if comp else os.path.join(root, filename)


def module_out_path(filename, kind="generated", base=""):
    """Like module_path(), but for WRITING — creates the component dir first."""
    p = module_path(filename, kind, base)
    os.makedirs(os.path.dirname(p), exist_ok=True)
    return p


# --- asm mirrored into the same component tree ---------------------------
# asm/ is a DIRECTORY per module (asm/<mod>/*.asm). When the src component tree is
# active it mirrors the same shape: asm/<component>/<mod>/ (dev/ nests one deeper).
# Resolution/enumeration is walk-based, so it's oblivious to nesting DEPTH — a module
# dir is simply any dir that directly holds .asm. Only the MOVER needs TREE.map.
_ASM_COMP_CACHE = {}


_ASM_DIRS_CACHE = {}


def asm_module_dirs(base=""):
    """Absolute path of every module's asm dir — the dirs that directly contain .asm.
    Works flat (asm/<mod>) or nested (asm/<comp>/<mod>, asm/<comp>/<sub>/<mod>).

    CACHED per asm-root: this walks the whole asm tree (224k files for win95), and it's
    on the hot path — linkfile._asm_dir calls it for EVERY module built, so an uncached
    walk turned a 1191-module install-tree build into hours of pure directory walking.
    asm/ does not change mid-run, so the walk is done once per process per root."""
    root = asm_dir(base)
    key = os.path.abspath(root)
    cached = _ASM_DIRS_CACHE.get(key)
    if cached is not None:
        return cached
    out = []
    if os.path.isdir(root):
        for dp, dns, fns in os.walk(root):
            dns[:] = [d for d in dns if d not in _TREE_SKIP]
            if any(f.endswith(".asm") for f in fns):
                out.append(dp)
        out = sorted(out)
    _ASM_DIRS_CACHE[key] = out
    return out


def asm_dir_index(base=""):
    """{module-dir basename (lower): abspath} — resolve a module to its asm dir at
    whatever depth the component tree put it. Cheap: derived from the cached walk."""
    return {os.path.basename(d).lower(): d for d in asm_module_dirs(base)}


def asm_component_map(base=""):
    """{asm-dir name AND stem (lower): component} derived from the generated TREE.map,
    so a module dir named by full filename (kernel32.dll) OR stem (krnl386) resolves."""
    root = _src_root(base)
    if root not in _ASM_COMP_CACHE:
        m = {}
        for k, comp in tree_map(base).items():
            if not k.startswith("generated/"):
                continue
            b = k[len("generated/"):]
            b = b[:-2] if b.endswith(".c") else b        # kernel32.dll.c -> kernel32.dll
            m[b.lower()] = comp
            m[b.rsplit(".", 1)[0].lower()] = comp         # + stem: kernel32
        _ASM_COMP_CACHE[root] = m
    return _ASM_COMP_CACHE[root]


def _asm_comp_for(name, base=""):
    cm = asm_component_map(base)
    return cm.get(name.lower()) or cm.get(name.rsplit(".", 1)[0].lower(), "")


def asm_module_dir(name, base=""):
    """Resolve an EXISTING module's asm dir (any name form / any depth); fall back to
    its component-tree path if not present yet."""
    idx = asm_dir_index(base)
    d = idx.get(name.lower()) or idx.get(name.rsplit(".", 1)[0].lower())
    if d:
        return d
    comp = _asm_comp_for(name, base)
    return os.path.join(asm_dir(base), comp, name) if comp else os.path.join(asm_dir(base), name)


def asm_module_out_dir(name, base=""):
    """Where to WRITE a module's asm — component dir when the tree is active. Creates
    it. Prefers an already-existing dir for `name` so re-conducts don't fork a copy."""
    idx = asm_dir_index(base)
    d = idx.get(name.lower()) or idx.get(name.rsplit(".", 1)[0].lower())
    if not d:
        comp = _asm_comp_for(name, base)
        d = os.path.join(asm_dir(base), comp, name) if comp else os.path.join(asm_dir(base), name)
    os.makedirs(d, exist_ok=True)
    return d


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
