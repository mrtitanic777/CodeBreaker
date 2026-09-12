"""Single source of truth for external-tool paths.

The CLI `config` / `doctor` commands and the individual toolchain modules
(cc16, masmlift, the Ghidra bridge, the Free60 xenon build_lib, ...) all resolve
tool locations through here, so no module hardcodes a machine-specific path.
Values persist in `XboxTranspiler.config.json` next to the CLI/EXE; set them with
`XboxTranspiler config <key> <path>` or edit the JSON by hand. Any key falls back
to the DEFAULTS below (discovered on the author's box) when unset.

Modules that live in a sibling package dir import this resiliently:

    import os, sys
    sys.path.insert(0, os.path.join(os.path.dirname(
        os.path.dirname(os.path.abspath(__file__))), "core"))
    import config
    DBX = config.tool("dosbox_x")
"""
import json
import os
import sys

FROZEN = getattr(sys, "frozen", False)
_HERE = os.path.dirname(os.path.abspath(__file__))              # .../xbt/core


def config_path():
    """Where XboxTranspiler.config.json lives (next to the CLI, or the EXE when
    frozen) -- kept identical to the CLI's own resolution so both agree."""
    base = (os.path.dirname(sys.executable) if FROZEN
            else os.path.join(os.path.dirname(_HERE), "cli"))  # .../xbt/cli
    return os.path.join(base, "XboxTranspiler.config.json")


CONFIG_PATH = config_path()

# Machine-specific defaults. Override any with `XboxTranspiler config <key> …`;
# `doctor` reports which of these actually resolve on this machine.
DEFAULTS = {
    "ghidra":   "",
    # Xbox 360 XDK -- the PowerPC cl.exe the permuter (--arch ppc) drives for
    # byte-exact matching decompilation.
    "xdk":      r"C:\Program Files (x86)\Microsoft Xbox 360 SDK",
    # optional PowerPC assembler for ppcrecon mnemonic verification (llvm-mc or a
    # powerpc GNU as). Empty => .long-backed byte-exact only.
    "ppc_as":   "",
    # dir holding the Free60 Xenon toolchain (xenon-as.exe / xenon-objcopy.exe),
    # used by the crypto build_lib / shadowboot helpers.
    "xenon_bin": "bin",
    # 16-bit period toolchain (runs under DOSBox-X; no NTVDM on x64).
    "dosbox_x": "",
    "msvc152c": "",
    # MASM's BIN dir (holds MASM.EXE); its parent (build16) has BIN/INC/LIB.
    "masm":     "",
    "sevenzip": r"C:\Program Files\7-Zip\7z.exe",
    "chdman": "",             # MAME chdman for CHD <-> ISO (empty => search PATH)
    "ffmpeg": "",             # ffmpeg for XMA/Bink audio+video decode (empty => PATH)
}


def load():
    """Merged config: DEFAULTS overlaid with the on-disk JSON (if present)."""
    cfg = dict(DEFAULTS)
    try:
        with open(CONFIG_PATH) as f:
            cfg.update(json.load(f))
    except (OSError, ValueError):
        pass
    return cfg


def save(cfg):
    with open(CONFIG_PATH, "w") as f:
        json.dump(cfg, f, indent=2)


def tool(key, default=None):
    """Resolved path for an external-tool key: the config value if set, else the
    DEFAULTS entry, else `default`."""
    return load().get(key, DEFAULTS.get(key, default))
