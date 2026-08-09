#!/usr/bin/env python3
"""
CodeBreaker — a deterministic reverse-engineering workbench.

    wt --target <era> conduct <MODULE>     # drive a module to byte-exact source
    wt --target <era> ledger --carve all   # coverage ledger
    wt --target <era> <cmd> [args...]      # run any tool for an era
    wt --target <era> list                 # commands available for an era
    wt targets                             # list eras and where their tools live
    wt doctor                              # external-dependency check
    wt diff <cmd>                          # cross-version analysis (target-agnostic)
    wt recompile <cmd>                     # make the lifted C compile/link (win95 corpus)
    wt inverse <cmd>                       # compiler-inverse engine (Lattice C 2.12 / MS C 2.03)
    wt ui                                  # CodeBreaker Studio: the themed drag-and-drop UI

A single entry point over era-specific toolchains, selected by `--target`
(win1.0, win3.1, win95, win98, winnt4). The shared, target-agnostic core lives
in `engine/`; each era's quirk tools live as a submodule under
`engine/targets/<era>/`, holding only what genuinely differs for that platform.

Dispatch establishes the context a tool expects: it changes into the selected
era's data directory (so relative paths such as files/ and reconstructed/
resolve) and puts that era's tool directory plus the shared engine on the import
path, then runs the requested command. WT_DATA_ROOT / WT_TOOLS_DIR / WT_TARGET
are exported so a tool can locate its data even when its own file lives elsewhere.
"""
import json
from engine.foundation import cores
import os
import runpy
import sys

WT_DIR = os.path.dirname(os.path.abspath(__file__))     # repo root (the tool code)
# Frozen (PyInstaller) ship build: the TOOL code is bundled inside sys._MEIPASS, but the
# DATA (win95/src, files/, ...) is the user's external checkout -- resolve it from
# WT_DATA_ROOT / the working dir, not from next to the bundle. Non-frozen runs resolve the
# data alongside the tool, so the dev workflow is byte-identical.
if getattr(sys, "frozen", False) and hasattr(sys, "_MEIPASS"):
    WT_DIR = sys._MEIPASS                                # bundled tools live here
    DATA_ROOT = os.environ.get("WT_DATA_ROOT") or os.getcwd()
else:
    # Dev checkout: data trees usually sit beside the tool (monorepo), but a split layout
    # (CodeBreaker tool repo + a separate data tree) points WT_DATA_ROOT at the data. Honor
    # the env override when it actually holds the era dirs, else fall back to the tool's parent.
    _env = os.environ.get("WT_DATA_ROOT")
    if _env and any(os.path.isdir(os.path.join(_env, e)) for e in
                    ("win95", "win98", "win3.1", "win1.0", "winnt4")):
        DATA_ROOT = _env
    else:
        DATA_ROOT = os.path.dirname(WT_DIR)              # holds per-era data dirs (dev checkout)
ENGINE = os.path.join(WT_DIR, "engine")                 # shared, target-agnostic tools
                                                        # (the IR engine: il/validate_il/decompile)

# The five eras. Each era's tool code is a submodule under the core, at
# `engine/targets/<era>/`; its DATA (files/, reconstructed/, ...) is external and
# resolved under DATA_ROOT/<era> (the user's checkout / working directory).
_ERAS = ("win1.0", "win3.1", "win95", "win98", "winnt4")


def _resolve_targets():
    return {name: (os.path.join(ENGINE, "targets", name),
                   os.path.join(DATA_ROOT, name)) for name in _ERAS}


TARGETS = _resolve_targets()

CONFIG_PATH = os.path.join(WT_DIR, "config.json")
def load_config():
    for p in (CONFIG_PATH,):
        if os.path.exists(p):
            try:
                return json.load(open(p))
            except (OSError, ValueError):
                pass
    return {}


def apply_config(tools_dir):
    """Push configured external-tool paths (from config.json) into the tools that
    reference them (e.g. masmlift, cc16)."""
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


def cmd_targets():
    print("CodeBreaker targets:\n")
    for name, (tools, data) in sorted(TARGETS.items()):
        print("  %-8s %s" % (name, os.path.relpath(tools, WT_DIR).replace("\\", "/")))
    print("\nrun:  wt --target <name> <command> [args...]")


_SUBSYS = ("targets", "diff", "recompile95", "studio", "inverse_engine", "goldens", "__pycache__")


def _engine_commands():
    """command name -> script path, across the engine's cluster subpackages."""
    out = {}
    if os.path.isdir(ENGINE):
        for c in sorted(os.listdir(ENGINE)):
            d = os.path.join(ENGINE, c)
            if c in _SUBSYS or not os.path.isdir(d):
                continue
            for f in sorted(os.listdir(d)):
                if f.endswith(".py") and not f.startswith("_") and f != "cores.py":
                    out.setdefault(f[:-3], os.path.join(d, f))
    return out


def cmd_list(tools_dir, target):
    cmds = set(_engine_commands())
    if os.path.isdir(tools_dir):
        cmds |= {f[:-3] for f in os.listdir(tools_dir)
                 if f.endswith(".py") and not f.startswith("_")}
    if not cmds:
        print("no tools for %s: %s" % (target, tools_dir)); return
    print("commands for --target %s  (%s + shared engine):\n"
          % (target, os.path.relpath(tools_dir, WT_DIR).replace("\\", "/")))
    for c in sorted(cmds):
        print("  %s" % c)


def cmd_doctor():
    cfg = load_config()
    print("CodeBreaker doctor\n")
    def chk(label, ok, detail=""):
        print("  [%s] %-12s %s" % ("OK" if ok else "--", label, detail))
    for name, (tools, data) in sorted(TARGETS.items()):
        chk(name, os.path.isdir(tools),
            "%s%s" % (os.path.relpath(tools, DATA_ROOT),
                      "" if os.path.isdir(data) else "  (DATA MISSING: %s)" % data))
    print()
    chk("DOSBox-X", os.path.exists(cfg.get("dosbox_x", "")), cfg.get("dosbox_x", "(unset)"))
    chk("MASM", os.path.exists(os.path.join(cfg.get("masm", ""), "MASM.EXE")),
        cfg.get("masm", "(unset)"))
    chk("CPU", True, "%d cores" % (os.cpu_count() or 1))


def cmd_recompile(rest):
    """Dispatch a recompile95 tool in the win95 data context."""
    rc_dir = os.path.join(ENGINE, "recompile95", "tools")
    if not rest or rest[0] == "list":
        print("recompile commands  (engine/recompile95/tools):\n")
        for f in sorted(os.listdir(rc_dir)) if os.path.isdir(rc_dir) else []:
            if f.endswith(".py") and not f.startswith("_"):
                print("  %s" % f[:-3])
        return
    cmd, cmd_args = rest[0], rest[1:]
    script = os.path.join(rc_dir, cmd + ".py")
    if not os.path.exists(script):
        print("error: no recompile command %r (try: wt recompile list)" % cmd); sys.exit(2)
    data_dir = os.path.join(DATA_ROOT, "win95")             # recompile95 works on the win95 corpus
    os.environ["WT_TARGET"] = "win95"
    os.environ["WT_DATA_ROOT"] = data_dir
    if os.path.isdir(data_dir):
        os.chdir(data_dir)
    sys.path.insert(0, WT_DIR)                               # the engine package
    cores.set_target(os.path.join(ENGINE, "targets", "win95"))
    sys.argv = [os.path.basename(script)] + cmd_args
    runpy.run_path(script, run_name="__main__")


def cmd_inverse(rest):
    """Dispatch an inverse-engine tool. The compiler-inverse engine (Lattice C 2.12 /
    MS C 2.03) is target-agnostic -- it models a period compiler, not a Windows era -- so it
    takes no --target. Its oracle (MC1/MC2 under DOSBox-X) is configured in config.json."""
    iv_dir = os.path.join(ENGINE, "inverse_engine")
    tools = {}
    if os.path.isdir(iv_dir):
        for f in sorted(os.listdir(iv_dir)):
            if f.endswith(".py") and not f.startswith("_") and f != "oracle16.py":
                tools[f[:-3]] = os.path.join(iv_dir, f)
    if not rest or rest[0] == "list":
        print("inverse-engine commands  (engine/inverse_engine):\n")
        for name in sorted(tools):
            print("  %s" % name)
        return
    cmd, cmd_args = rest[0], rest[1:]
    script = tools.get(cmd)
    if not script:
        print("error: no inverse command %r (try: wt inverse list)" % cmd); sys.exit(2)
    sys.path.insert(0, WT_DIR)                               # the engine package
    sys.argv = [os.path.basename(script)] + cmd_args
    runpy.run_path(script, run_name="__main__")


def usage():
    print(__doc__)
    cmd_targets()


def main():
    import multiprocessing
    multiprocessing.freeze_support()
    argv = sys.argv[1:]
    if not argv or argv[0] in ("-h", "--help", "help"):
        usage(); return
    if argv[0] == "targets":
        cmd_targets(); return
    if argv[0] == "doctor":
        cmd_doctor(); return
    if argv[0] == "diff":
        # Cross-version analysis (CodeBreaker Diff): target-agnostic, so it does not
        # take --target. Delegates to the shared engine/diff CLI over the whole corpus.
        sys.path.insert(0, WT_DIR)                       # the engine package
        sys.path.insert(0, os.path.join(ENGINE, "diff")) # diff's own local modules
        from engine.diff import cli as diff_cli
        diff_cli.main(argv[1:]); return
    if argv[0] == "recompile":
        # The recompile95 subsystem: take the IR engine's lifted C the rest of the way
        # (compile -> link). It operates on the win95 corpus, so run it in win95's data
        # context (the tools set up their own import path).
        cmd_recompile(argv[1:]); return
    if argv[0] == "inverse":
        # The compiler-inverse engine: bytes -> byte-exact C by inverting the Lattice C /
        # MS C 2.03 codegen. Target-agnostic; the oracle is set in config.json.
        cmd_inverse(argv[1:]); return
    if argv[0] == "ui":
        # CodeBreaker Studio: the native (Win32/GDI) themed drag-and-drop workbench.
        sys.path.insert(0, WT_DIR)
        from engine.studio import app as studio
        studio.main(argv[1:]); return

    # --target <name> may appear anywhere; pull it out, keep the rest verbatim.
    target = None
    rest = []
    i = 0
    while i < len(argv):
        a = argv[i]
        if a == "--target" and i + 1 < len(argv):
            target = argv[i + 1]; i += 2; continue
        if a.startswith("--target="):
            target = a.split("=", 1)[1]; i += 1; continue
        rest.append(a); i += 1

    if target is None:
        print("error: --target is required (one of: %s)\n" % ", ".join(sorted(TARGETS)))
        cmd_targets(); sys.exit(2)
    if target not in TARGETS:
        print("error: unknown target %r (one of: %s)" % (target, ", ".join(sorted(TARGETS))))
        sys.exit(2)

    tools_dir, data_dir = TARGETS[target]
    if not rest or rest[0] == "list":
        cmd_list(tools_dir, target); return

    cmd, cmd_args = rest[0], rest[1:]
    script = os.path.join(tools_dir, cmd + ".py")        # a target override wins...
    if not os.path.exists(script):
        script = _engine_commands().get(cmd)             # ...else the shared engine command
        if not script:
            print("error: no command %r for target %s (try: wt --target %s list)"
                  % (cmd, target, target))
            sys.exit(2)

    # Become the project context the tool expects.
    os.environ["WT_TARGET"] = target
    os.environ["WT_DATA_ROOT"] = data_dir
    os.environ["WT_TOOLS_DIR"] = tools_dir
    if os.path.isdir(data_dir):
        os.chdir(data_dir)                   # files/ reconstructed/ resolve here
    sys.path.insert(0, WT_DIR)               # repo root -> the `engine` package imports
    sys.path.insert(0, tools_dir)            # a target's own local modules (target.py, ...)
    cores.set_target(tools_dir)              # DI: per-era overrides for shared engine tools
    apply_config(tools_dir)
    sys.argv = [os.path.basename(script)] + cmd_args
    runpy.run_path(script, run_name="__main__")


if __name__ == "__main__":
    main()
