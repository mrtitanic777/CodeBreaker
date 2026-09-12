#!/usr/bin/env python3
r"""dosbox.py -- one shared DOSBox-X runner for every DOS-hosted tool adapter.

The compiler, assembler, and linker adapters all do the same dance: mount a tool
directory + a work directory, run a .BAT, collect artifact files. That boilerplate
lived (proven) inside msc6_oracle; the backbone factors it out here so a new
DOS-hosted tool is a BAT template, not another copy of the mount/exec plumbing.

Honours the same config + no-fabrication contract as the oracle: if DOSBox-X is
not configured, run_dos() returns (False, ...) rather than pretending to run.
"""
import os, subprocess, tempfile


def dosbox_path():
    try:
        from src.core import config
        return config.load_config().get("dosbox_x", "")
    except Exception:
        return ""


def run_dos(mounts, bat_lines, artifacts, timeout=90, files=None):
    """Run a DOS batch under DOSBox-X.

      mounts    : dict drive_letter -> host_dir  (e.g. {'c': tcbin, 'd': work})
                  the LAST-listed drive is made current before the BAT runs.
      bat_lines : list[str] of DOS commands (written to BLD.BAT on the work drive).
      artifacts : list[str] filenames (on the work drive) to read back as bytes.
      files     : optional dict filename -> text/bytes to drop on the work drive first.

    Returns (ok, {name: bytes}, work_dir). ok is False (with an empty map) when
    DOSBox-X is unconfigured or the run produced none of the artifacts."""
    dbx = dosbox_path()
    if not dbx or not os.path.exists(dbx):
        return False, {}, None
    work = tempfile.mkdtemp(prefix="bb_dos_")
    mounts = dict(mounts); mounts.setdefault("d", work)     # work drive default d:
    for nm, data in (files or {}).items():
        mode = "wb" if isinstance(data, (bytes, bytearray)) else "w"
        with open(os.path.join(work, nm), mode) as fh:
            fh.write(data if isinstance(data, (bytes, bytearray)) else str(data))
    cur = list(mounts)[-1]
    with open(os.path.join(work, "BLD.BAT"), "w") as fh:
        fh.write("@echo off\r\n" + "\r\n".join(bat_lines) + "\r\necho _DONE_ > done.txt\r\n")
    conf = ["[cpu]", "cycles=20000", "[autoexec]"]
    for drv, hostdir in mounts.items():
        conf.append("mount %s %s" % (drv, hostdir.replace("/", "\\")))
    conf += ["%s:" % cur, "call BLD.BAT", "exit", ""]
    with open(os.path.join(work, "r.conf"), "w") as fh:
        fh.write("\n".join(conf))
    for a in list(artifacts) + ["done.txt"]:
        try: os.remove(os.path.join(work, a))
        except OSError: pass
    # Headless by default: SDL's dummy video driver runs DOSBox-X with NO window,
    # so batch/corpus runs don't spam hundreds of windows across the desktop.
    # Override by pre-setting SDL_VIDEODRIVER (e.g. to "" / "windib") for a real
    # window when a human wants to watch a single interactive run.
    env = dict(os.environ)
    env.setdefault("SDL_VIDEODRIVER", "dummy")
    try:
        subprocess.run([dbx, "-conf", os.path.join(work, "r.conf"), "-exit"],
                       stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL,
                       timeout=timeout, env=env)
    except Exception:
        return False, {}, work
    out = {}
    for a in artifacts:
        ap = os.path.join(work, a)
        if os.path.exists(ap):
            out[a] = open(ap, "rb").read()
    return (len(out) == len(artifacts)), out, work
