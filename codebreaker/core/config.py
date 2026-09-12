#!/usr/bin/env python3
r"""config.py — shared external-tool config + plugin loader.

Lifted out of the per-target `<x>re.py` workbench dispatchers (win95re/win98re/
winnt4re.py), which `wt.py` replaced as the single entry point. The dispatchers'
only remaining job was two helpers that the TOOLS imported directly:

  * apply_config()  — push configured external-tool paths (DOSBox-X, MASM) INTO the
    tools that hardcode them (masmlift, cc16). doctor/config read the config; this is
    what makes it actually take effect. No-op when there's no config (tools keep their
    correct-for-this-checkout defaults).
  * load_plugins()  — enumerate plugins/*.py that expose a DESCRIPTION.

Moving them here lets every tool `import config` from the shared engine instead of a
per-target module, so the `<x>re.py` dispatchers can retire. Mirrors wt.py's own
load_config/apply_config (wt uses its copy for the CLI path; tools use this one when
they wire config themselves, incl. standalone runs).
"""
import json
from codebreaker.core import cores
from codebreaker.core import paths
import os


def _resolve(cfg):
    """Make config.json portable: a RELATIVE tool path is resolved against the repo root, so a
    clone anywhere just works (dosbox_x/masm/msvc152c live inside the repo). ABSOLUTE paths --
    genuinely-external tools like ghidra / 7-Zip -- pass through unchanged."""
    root = paths.repo_root()
    out = {}
    for k, v in cfg.items():
        out[k] = os.path.normpath(os.path.join(root, v)) if isinstance(v, str) and v and not os.path.isabs(v) else v
    return out


def load_config():
    """The external-tool config dict (dosbox_x / masm / ...), or {} if none set. Repo-relative
    paths are resolved to absolute against the repo root (see _resolve)."""
    cands = paths.config_candidates()
    for p in cands:
        if p and os.path.exists(p):
            try:
                return _resolve(json.load(open(p)))
            except (OSError, ValueError):
                pass
    return {}


def apply_config():
    """Push configured external-tool paths into the tools that hardcode them."""
    cfg = load_config()
    if not cfg:
        return
    try:
        masmlift = cores.get('masmlift')
        if cfg.get("dosbox_x"):
            masmlift.DBX = cfg["dosbox_x"]
        cm = cfg.get("masm")
        if cm and os.path.exists(os.path.join(cm, "MASM.EXE")):
            masmlift.MASMROOT = os.path.dirname(cm)
        elif cm and os.path.exists(os.path.join(cm, "BIN", "MASM.EXE")):
            masmlift.MASMROOT = cm
    except Exception:
        pass
    try:
        cc16 = cores.get('cc16')
        if cfg.get("dosbox_x"):
            cc16.DBX = cfg["dosbox_x"]
    except Exception:
        pass


def load_plugins(tools_dir=None):
    """[(name, path, desc)] for every plugins/*.py (under the target's tool dir)
    that defines a module-level DESCRIPTION string."""
    pdir = os.path.join(tools_dir or os.environ.get("WT_TOOLS_DIR", "") or paths.repo_root(), "plugins")
    out = []
    if not os.path.isdir(pdir):
        return out
    for fn in sorted(os.listdir(pdir)):
        if not fn.endswith(".py") or fn.startswith("_"):
            continue
        path = os.path.join(pdir, fn)
        desc = ""
        try:
            for line in open(path, encoding="utf-8"):
                if line.strip().startswith("DESCRIPTION"):
                    desc = line.split("=", 1)[1].strip().strip("'\"")
                    break
        except OSError:
            pass
        out.append((fn[:-3], path, desc or "(plugin)"))
    return out
