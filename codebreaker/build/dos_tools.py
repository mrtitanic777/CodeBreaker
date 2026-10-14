"""dos_tools.py -- run DOS programs (assemblers, compilers, linkers of the 1980s) in DOSBox-X.

    from codebreaker.build import dos_tools
    dos_tools.emulator()                                       DOSBox-X (settings: dosbox_x), or ''
    ok, made, work = dos_tools.run({"c": tools}, ["masm t.asm;"], ["T.OBJ"], files={"T.ASM": text})
    r = dos_tools.masm(text)                                   {"obj", "lst", "log", "work"} from MASM

`run` mounts each folder as a drive (the work folder is d: unless named; the last drive listed
is the current one), writes the commands to BLD.BAT, runs it and reads the named files back.
`ok` is False -- and nothing is pretended -- when the emulator is not set up or a file was not
made. The emulator runs without a window.

`masm` assembles one source with the Microsoft Macro Assembler of the folder settings "masm"
names (it holds BIN/MASM.EXE, INC, LIB), or the one given -- build/masm.py runs it.
"""
import os
import subprocess
import tempfile


def emulator():
    from codebreaker.base import settings
    path = settings.get("dosbox_x") or ""
    return path if path and os.path.isfile(path) else ""


def run(mounts, bat_lines, artifacts, timeout=90, files=None):
    """-> (every artifact was made, {name: bytes}, the work folder or None)"""
    dbx = emulator()
    if not dbx:
        return False, {}, None
    work = tempfile.mkdtemp(prefix="cb_dos_")
    mounts = dict(mounts)
    mounts.setdefault("d", work)
    for name, data in (files or {}).items():
        with open(os.path.join(work, name), "wb" if isinstance(data, (bytes, bytearray)) else "w") as f:
            f.write(data if isinstance(data, (bytes, bytearray)) else str(data))
    with open(os.path.join(work, "BLD.BAT"), "w") as f:
        f.write("@echo off\r\n" + "\r\n".join(bat_lines) + "\r\necho _DONE_ > done.txt\r\n")
    conf = ["[cpu]", "cycles=20000", "[autoexec]"]
    conf += ["mount %s %s" % (drive, folder.replace("/", "\\")) for drive, folder in mounts.items()]
    conf += ["%s:" % list(mounts)[-1], "call BLD.BAT", "exit", ""]
    with open(os.path.join(work, "r.conf"), "w") as f:
        f.write("\n".join(conf))
    for name in list(artifacts) + ["done.txt"]:
        try:
            os.remove(os.path.join(work, name))
        except OSError:
            pass
    env = dict(os.environ)
    env.setdefault("SDL_VIDEODRIVER", "dummy")
    try:
        subprocess.run([dbx, "-conf", os.path.join(work, "r.conf"), "-exit"], stdout=subprocess.DEVNULL,
                       stderr=subprocess.DEVNULL, timeout=timeout, env=env)
    except (OSError, subprocess.TimeoutExpired):
        return False, {}, work
    made = {}
    for name in artifacts:
        path = os.path.join(work, name)
        if os.path.exists(path):
            with open(path, "rb") as f:
                made[name] = f.read()
    return len(made) == len(artifacts), made, work


def masm_root():
    from codebreaker.base import settings
    root = settings.get("masm") or ""
    return root if root and os.path.isfile(os.path.join(root, "BIN", "MASM.EXE")) else ""


def masm(source, stem="t", timeout=180, root=None):
    """Assemble `source` (build/masm.py). -> {"obj": bytes, "lst": text, "log": text, "work": folder};
    a part the assembler did not make is missing."""
    from codebreaker.build import masm as assembler
    return assembler.assemble(source, stem=stem, timeout=timeout, masmroot=root or None)
