"""paths.py -- resolve the msc6 engine's external inputs from CodeBreaker config.json.

The byte-exact MSC-6.0a reconstruction engine (merged from the standalone w95recon DLL
tool) needs: the period toolchain (MSC 6.0a / MASM 5.1 / LINK 5.20+5.60 / Win3.1 SDK),
DOSBox-X, and the shipped-DLL corpus. CodeBreaker ships no binaries -- all three are
external and configured in config.json:
    dosbox_x         -- the DOS host
    msc6_toolchain   -- the version-keyed toolchain root (has MANIFEST.json + toolchain.py)
    msc6_data        -- the Win95 data checkout root (files/WINDOWS/SYSTEM/*.dll = oracles)
"""
import os
from src.core import config

_cfg = config.load_config()

DOSBOX = _cfg.get("dosbox_x", "")
TOOLCHAIN_ROOT = _cfg.get("msc6_toolchain", "")
DATA_ROOT = _cfg.get("msc6_data", "")
SYS_DIR = os.path.join(DATA_ROOT, "files", "WINDOWS", "SYSTEM") if DATA_ROOT else ""


def toolchain_manifest():
    """Path to the version-keyed toolchain MANIFEST.json (resolver reads it)."""
    return os.path.join(TOOLCHAIN_ROOT, "MANIFEST.json")


def missing():
    """Return a list of unconfigured/absent inputs (for `wt msc6 doctor`)."""
    m = []
    if not DOSBOX or not os.path.exists(DOSBOX):
        m.append("dosbox_x")
    if not TOOLCHAIN_ROOT or not os.path.exists(toolchain_manifest()):
        m.append("msc6_toolchain (MANIFEST.json)")
    if not SYS_DIR or not os.path.isdir(SYS_DIR):
        m.append("msc6_data (files/WINDOWS/SYSTEM)")
    return m
