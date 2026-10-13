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
    wt inverse <cmd>                       # compiler-inverse engine (Lattice C 2.12 / MS C 2.03)
    wt ui                                  # CodeBreaker Studio: the themed drag-and-drop UI

A single entry point over era-specific toolchains, selected by `--target`
(win1.0, win3.1, win95, win98). The shared, target-agnostic core lives
in `src/`; each era's quirk tools live as a submodule under
`src/targets/<era>/`, holding only what genuinely differs for that platform.

Dispatch establishes the context a tool expects: it changes into the selected
era's data directory (so relative paths such as files/ and reconstructed/
resolve) and puts that era's tool directory plus the shared engine on the import
path, then runs the requested command. WT_DATA_ROOT / WT_TOOLS_DIR / WT_TARGET
are exported so a tool can locate its data even when its own file lives elsewhere.
"""
import json
from src.core import cores
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
                    ("win95", "win98", "win3.1", "win1.0")):
        DATA_ROOT = _env
    else:
        DATA_ROOT = os.path.dirname(WT_DIR)              # holds per-era data dirs (dev checkout)
ENGINE = os.path.join(WT_DIR, "src")                 # shared, target-agnostic tools
                                                        # (the IR engine: il/validate_il/decompile)

# The five eras. Each era's tool code is a submodule under the core, at
# `src/targets/<era>/`; its DATA (files/, reconstructed/, ...) is external and
# resolved under DATA_ROOT/<era> (the user's checkout / working directory).
_ERAS = ("win1.0", "win3.1", "win95", "win98")


def _era_data(name):
    # A per-era data override (WT_WIN1_0_DATA / WT_WIN3_1_DATA / WT_WIN98_DATA) lets an era's
    # data live in its OWN standalone repo (e.g. GitHub/win1.0) instead of under DATA_ROOT;
    # falls back to DATA_ROOT/<era> (win95 stays under binaries/win95). NOTE: distinct from
    # WT_WIN95_ROOT, which is the reconstructed SOURCE tree (the Windows95 repo), not data.
    ov = os.environ.get("WT_%s_DATA" % name.upper().replace(".", "_"))
    return ov if ov and os.path.isdir(ov) else os.path.join(DATA_ROOT, name)


def _resolve_targets():
    return {name: (os.path.join(ENGINE, "targets", name), _era_data(name)) for name in _ERAS}


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


def _engine_commands():
    """command name -> script path, across the shared engine clusters. Post-regroup the clusters
    are the top-level analysis groups (core/formats/disasm/ir) plus one level into engines/ and
    toolchain/; targets/xbox/studio/cli have their own dispatch and are not shared commands."""
    scan = [os.path.join(ENGINE, c) for c in ("core", "formats", "disasm", "ir")]
    for grp in ("engines", "toolchain"):
        gdir = os.path.join(ENGINE, grp)
        if os.path.isdir(gdir):
            scan += [os.path.join(gdir, s) for s in sorted(os.listdir(gdir))]
    out = {}
    for d in scan:
        if not os.path.isdir(d):
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


# Script-directory subsystems: a subsystem's commands are the non-underscore .py files in a
# directory, executed as __main__. This table is the per-subsystem config; `_run_scripts` is the
# ONE dispatcher that replaced four near-identical cmd_* functions.
# Subsystems moved out of the active package (see trash/README.md). Nothing imports them.
_ARCHIVED = {
    "recompile": "the GCC compile/link lane is in trash/engines/recompile, superseded by the "
                 "OpenWatcom lane (wt --target win95 owbuild / owlink)",
}

_SCRIPT_SUBSYS = {
    "behavior":  dict(subdir=("engines", "behavior"), win95=True, behavior_env=True,
                      skip={"__init__.py", "tracer.py", "corpus.py", "perturb.py",
                            "hypothesis.py", "env.py"},
                      header="behavior-engine commands (src/engines/behavior) -- dynamic "
                             "evidence via Unicorn:\n",
                      footer="\n(trace: record a function's behavior; show: print a stored trace)"),
    "inverse":   dict(subdir=("engines", "inverse"), skip={"oracle16.py"},
                      header="inverse-engine commands  (src/engines/inverse):\n"),
    "msc6":      dict(subdir=("engines", "msc6"),
                      skip={"paths.py", "toolchain.py", "__init__.py", "bruteforce.py", "autoaxis.py"},
                      header="msc6-engine commands (src/engines/msc6) -- byte-exact MSC-6.0a "
                             "NE reconstruction:\n",
                      footer="\n(recipe: auto-detect a DLL's compiler flags; normalize: compact an "
                             "NE's DOS stub)"),
}


def _run_scripts(rest, *, subdir, skip=frozenset(), win95=False, behavior_env=False,
                 header="", footer=""):
    """Dispatch a script-directory subsystem: list its commands, or run one as __main__.
    (`engine` is importable via the installed package; the runpy'd scripts self-insert their own
    dir for sibling imports, so no repo-root sys.path hack is needed here.)"""
    directory = os.path.join(ENGINE, *subdir)
    tools = {}
    if os.path.isdir(directory):
        for f in sorted(os.listdir(directory)):
            if f.endswith(".py") and not f.startswith("_") and f not in skip:
                tools[f[:-3]] = os.path.join(directory, f)
    if not rest or rest[0] in ("list", "-h", "--help"):
        print(header)
        for name in sorted(tools):
            print("  %s" % name)
        if footer:
            print(footer)
        return
    cmd, cmd_args = rest[0], rest[1:]
    script = tools.get(cmd)
    if not script:
        print("error: unknown command %r (try: list)" % cmd); sys.exit(2)
    if win95:                                                # recompile/behavior run on the win95 corpus
        data_dir = os.path.join(DATA_ROOT, "win95")
        os.environ["WT_TARGET"] = "win95"
        os.environ["WT_DATA_ROOT"] = data_dir
        if os.path.isdir(data_dir):
            os.chdir(data_dir)
        cores.set_target(os.path.join(ENGINE, "targets", "win95"))
    if behavior_env:                                         # enable API-call stubbing if SDK configured
        try:
            from src.engines.behavior import env
            env.apply_defaults()
        except Exception:
            pass
    sys.argv = [os.path.basename(script)] + cmd_args
    runpy.run_path(script, run_name="__main__")


def usage():
    print(__doc__)
    cmd_targets()


def cmd_xbox(rest):
    """The Xbox 360 subsystem (integrated from XboxTranspiler): XEX2/PowerPC byte-exact
    reconstruction + console/format/crypto/forensics tooling. Its package sets up its own
    flat import path; data/samples/console-secrets live outside git (src/xbox/.gitignore)."""
    launcher = os.path.join(ENGINE, "xbox", "XboxTranspiler.py")
    if not os.path.exists(launcher):
        print("error: xbox subsystem not present (src/xbox)"); sys.exit(2)
    sys.path.insert(0, WT_DIR)                            # the engine package on path
    sys.argv = [launcher] + list(rest)
    runpy.run_path(launcher, run_name="__main__")


def _attach_console_if_cli(argv):
    """Hybrid ship-exe support: the packaged build is windowed (no console flash on a
    double-click). When launched with CLI arguments from a terminal, attach to the parent
    console so stdout/stderr are visible; either way, ensure the streams exist (a windowed
    build has none) so print() never crashes -- output is discarded when there is no console."""
    if not (getattr(sys, "frozen", False) and sys.platform == "win32"):
        return
    log = os.environ.get("WT_LOG")                          # diagnostics for the windowed build
    if log:
        try:
            f = open(log, "w", buffering=1); sys.stdout = f; sys.stderr = f
            return
        except Exception:
            pass
    attached = False
    if argv:
        try:
            import ctypes
            attached = bool(ctypes.windll.kernel32.AttachConsole(-1))   # ATTACH_PARENT_PROCESS
        except Exception:
            attached = False
    for _name in ("stdout", "stderr"):
        if getattr(sys, _name, None) is None:
            try:
                setattr(sys, _name, open("CONOUT$" if attached else os.devnull, "w", buffering=1))
            except Exception:
                pass


def main():
    import multiprocessing
    multiprocessing.freeze_support()
    argv = sys.argv[1:]
    _attach_console_if_cli(argv)
    if not argv:
        argv = ["ui"]                                           # double-click / no args -> the UI
    if argv[0] in ("-h", "--help", "help"):
        usage(); return
    if argv[0] == "targets":
        cmd_targets(); return
    if argv[0] == "doctor":
        cmd_doctor(); return
    if argv[0] == "serve":                                   # localhost HTTP API over the whole toolset
        from src.cli import apiserver
        apiserver.main(argv[1:]); return
    # Subsystem dispatch. The script-directory subsystems (behavior/inverse/msc6) share one
    # dispatcher via the _SCRIPT_SUBSYS table; the rest have their own import entry.
    # (No sys.path.insert(WT_DIR) anywhere now -- `engine` is importable via the installed package.)
    if argv[0] in _ARCHIVED:
        print("'%s' was archived: %s" % (argv[0], _ARCHIVED[argv[0]])); return
    if argv[0] in _SCRIPT_SUBSYS:
        _run_scripts(argv[1:], **_SCRIPT_SUBSYS[argv[0]]); return
    if argv[0] == "xbox":
        cmd_xbox(argv[1:]); return
    if argv[0] == "diff":
        # Cross-version analysis: target-agnostic, over the whole corpus.
        sys.path.insert(0, os.path.join(ENGINE, "engines", "diff"))  # diff's own local sibling modules
        from src.engines.diff import cli as diff_cli
        diff_cli.main(argv[1:]); return
    if argv[0] == "backbone":
        from src.toolchain.backbone.__main__ import main as bb_main
        sys.exit(bb_main(argv[1:]))
    if argv[0] == "sdk":
        from src.toolchain.sdk.__main__ import main as sdk_main
        sys.exit(sdk_main(argv[1:]))
    if argv[0] == "ui":
        if len(argv) > 1 and argv[1] == "decode":               # the Live Reconstruction window
            from src.studio import decoder
            decoder.main(argv[2:])
            return
        # Native WinForms UI; owner-drawn GDI fallback if pythonnet/.NET is unavailable.
        try:
            from src.studio import winforms_app
            winforms_app.main(argv[1:])
        except ImportError:
            from src.studio import app as studio
            studio.main(argv[1:])
        return

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
