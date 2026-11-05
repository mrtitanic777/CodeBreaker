"""native_dos.py -- run the period real-mode DOS tools on CodeBreaker's OWN emulator, in-process,
instead of launching DOSBox-X.

DOSBox-X boots a whole emulated PC in an external process to run one 16-bit DOS program: it is
slow (~1.5-2 s of boot per assemble), serial, heavyweight and an external dependency. CodeBreaker
already has a real-mode DOS executive on Unicorn -- codebreaker/exact/emu/dos.py: a PSP, the MCB
memory chain, program load and INT 20h/21h (the file & process API over a shim filesystem that
maps a working directory, the console, EXEC, version, time), all in-process with no disk image.
This wraps that executive in the shape build/masm.py wants, so the optional period byte-match can
re-assemble with the authentic MASM.EXE on our own CPU: ~20x faster, headless, cacheable, parallel,
no external process.

    native_dos.available()                      Unicorn is importable
    native_dos.enabled()                        the native runner is on (see the CB_NATIVE_DOS switch)
    native_dos.assemble(src, stem, masmroot)    -> {obj, lst, log, work}   (one source; mirrors masm.assemble)
    native_dos.assemble_many(items, masmroot)   -> {stem: {obj, lst, log}} (mirrors the warm DOSBox worker)

THE SWITCH -- CB_NATIVE_DOS:
    unset / "auto"   native for the tools PROVEN byte-identical to DOSBox (_PROVEN: MASM and the DDK's
                     TNT ML), with DOSBox as the automatic fallback when the native run cannot complete
    "1" / "on"       force the native runner (no DOSBox)
    "0" / "off"      force DOSBox (the native runner is not used)
MASM is proven byte-identical (tests/test_dos.py::test_native_masm_matches_dosbox). The Win95 DDK's
Phar Lap TNT ML.EXE ("ml") is proven byte-identical on real Win95 VxD sources, OMF and COFF
(test_native_tnt_ml_matches_dosbox and the gate of state/work/mcb-arena/gate.py; a COFF header's
TimeDateStamp is the run's clock -- ours a fixed date, DOSBox's the host's -- and is the one field
that differs, as it does between two DOSBox runs).

THE NATIVE/C++ SPLIT:
    The hot loop -- the millions of 8086 instructions MASM executes -- runs inside Unicorn's native C
    engine. The executive itself (load, the MCB arena, INT 20h/21h, EXEC, the DPMI host) is C++ too:
    native/dosexec (cbdos.dll), behind CB_NATIVE_DOSEXEC (exact/emu/native_dosexec.py; docs/NATIVE.md),
    gated byte-identical to the Python executive on MASM, TNT ML and the period compilers.

THE DPMI TOOL (TNT ML.EXE):
    The Win95 DDK's ML.EXE (the VxD/driver assembler) is a Phar Lap TNT bound program: its real-mode
    stub walks the DOS MCB chain, loads DOSXNT.EXE as an overlay (INT 21h 4B03h), and DOSXNT enters
    32-bit protected mode through a DPMI host, copies itself above 1 MB, and from then on raw-switches
    back to real mode (INT 31h 0306h) for every DOS call its 32-bit application makes through its own
    protected-mode INT 21h handler. (MSC 6.0a is plain real mode and the VC++ 1.52c passes are Win32,
    so ML is the one DPMI tool in our toolchains.) What that needed:
      * exact/emu/dos.py: a real in-RAM MCB arena (INT 21h 48h/49h/4Ah on it, 52h list of lists, 58h
        allocation strategy), 4B03h overlay load, 50h/51h/55h PSPs, 67h, 2Bh/2Dh, INT 15h (no
        extended memory by BIOS), and the program's own path after the environment strings;
      * exact/emu/dpmi.py: raw mode switches, 0305h, protected-mode interrupt dispatch to the
        client's 0205h handlers, 000Dh, 0002h, 0604h, 0702h/0703h, 0800h, 0900h-0902h, the PIC bases
        in 0400h, and descriptor reads from the tables themselves (D/B bits).
    ml() below runs it entirely in memory (a MemFS: nothing written to the disk).
"""
import os
import shutil
import tempfile


class NativeDosError(RuntimeError):
    """The native runner could not complete a run (unimplemented service, CPU fault, limit): the
    caller falls back to DOSBox. A clean run that merely reports tool errors is NOT this."""


def available():
    try:
        import unicorn  # noqa: F401
    except Exception:
        return False
    return True


def enabled(tool="masm"):
    """Whether the native runner should be used for `tool`. See the CB_NATIVE_DOS switch above."""
    mode = str(os.environ.get("CB_NATIVE_DOS", "auto")).lower()
    if mode in ("0", "off", "no", "false"):
        return False
    if not available():
        return False
    if mode in ("1", "on", "yes", "true"):
        return True
    return tool in _PROVEN                       # "auto": only the proven tools


_PROVEN = {"masm", "ml"}                         # tools proven byte-identical to DOSBox (MASM; the DDK TNT ML)


def _run_tool(exe, work, cmdline, fast=True):
    """Run a real-mode DOS program under the executive. -> (exit code, console bytes). Raises
    NativeDosError if the executive could not complete the run."""
    from codebreaker.exact.emu import dos
    try:
        r = dos.run(exe, work, cmdline, fast=fast)
    except Exception as e:                       # unimplemented INT, CPU fault, block/time limit
        raise NativeDosError("%s: %s" % (type(e).__name__, e))
    return r.exit, r.console


def _stem_path(work, stem, ext):
    """work/<stem><ext>, making any sub-folder the stem names (callers use flat stems, but
    'asm/lead00'-style stems are handled too)."""
    parts = stem.replace("\\", "/").split("/")
    d = os.path.join(work, *parts[:-1]) if len(parts) > 1 else work
    os.makedirs(d, exist_ok=True)
    return os.path.join(d, parts[-1] + ext)


def _masm_exe(work, masmroot):
    exe = os.path.join(work, "MASM.EXE")
    if not os.path.exists(exe):
        src = os.path.join(masmroot, "BIN", "MASM.EXE")
        if not os.path.exists(src):
            raise NativeDosError("MASM.EXE not found under %s\\BIN" % masmroot)
        shutil.copy(src, exe)
    return exe


def _read_out(work, stem):
    out = {}
    for ext, key in ((".OBJ", "obj"), (".LST", "lst"), (".LOG", "log")):
        p = _stem_path(work, stem, ext)
        if os.path.isfile(p):
            out[key] = open(p, "rb").read() if ext == ".OBJ" else open(p, encoding="latin1").read()
    return out


def _cmdline(stem):
    # the ASM must be given with a sub-path if the stem has one; the outputs the same
    name = stem.replace("/", "\\")
    return "%s.ASM,%s.OBJ,%s.LST,NUL;" % (name, name, name)


def assemble(asm_src, stem="t", masmroot=None):
    """One source, the native runner. -> {obj, lst, log, work}. Raises NativeDosError if the
    executive could not complete (the caller then uses DOSBox)."""
    from codebreaker.base import settings
    masmroot = masmroot or settings.get("masm")
    work = tempfile.mkdtemp(prefix="masm_nat_")
    with open(_stem_path(work, stem, ".ASM"), "w", newline="\r\n") as f:
        f.write(asm_src.replace("\r\n", "\n"))        # one newline convention -> clean CRLF on disk
    exe = _masm_exe(work, masmroot)
    _exit, console = _run_tool(exe, work, _cmdline(stem))
    out = _read_out(work, stem)
    out["work"] = work
    out.setdefault("log", console.decode("latin1"))
    return out


def run_extended(tool_files, exe, cmdline, files, artifacts, env=b"PATH=C:\\\0"):
    """Run a DOS-extended (DPMI) tool -- the Phar Lap TNT ML.EXE -- on the executive with the DPMI host,
    entirely in memory: the tool's own files and the inputs are given ({name: path or bytes}), what it
    writes stays in a MemFS, nothing touches the disk. -> ({artifact: bytes} of the ones made,
    console bytes). Raises NativeDosError if the run could not complete (the caller uses DOSBox)."""
    from codebreaker.exact.emu import dos
    from codebreaker.base import settings
    work = os.path.join(settings.ROOT, "state", "native-dos-mem")     # a name only: nothing is written there
    fs = dos.MemFS()
    for name, src in tool_files.items():
        fs.put(os.path.join(work, name.upper()), fs.disk(src))       # (read once, kept for every run after)
    for name, src in files.items():                                   # text: CRLF on "disk", as a text-mode write
        if isinstance(src, str):
            src = src.replace("\r\n", "\n").replace("\n", "\r\n").encode("latin1")
        fs.put(os.path.join(work, name.upper()), src)
    try:
        r = dos.run(os.path.join(work, exe.upper()), work, cmdline, fast=True, dpmi=True, fs=fs, env=env)
    except Exception as e:
        raise NativeDosError("%s: %s" % (type(e).__name__, e))
    made = {}
    for name in artifacts:
        p = os.path.join(work, name.upper())
        if fs.isfile(p) and fs.key(p) in fs.files:
            made[name] = fs.read(p)
    return made, r.console


def ml_files(root=None):
    """The Win95 DDK's Phar Lap TNT ML.EXE, its ML.ERR and the DOSXNT.EXE extender: {name: path}."""
    from codebreaker.base import corpus, settings
    ml = corpus.reference("Microsoft Windows 95 DDK", "MASM611C")
    dx = os.path.join(settings.get("msvc152") or os.path.join(root or settings.ROOT, "toolchains", "period",
                                                              "compilers", "msvc-1.52c"), "BIN", "DOSXNT.EXE")
    out = {"ML.EXE": os.path.join(ml, "ML.EXE"), "ML.ERR": os.path.join(ml, "ML.ERR"), "DOSXNT.EXE": dx}
    missing = [p for p in out.values() if not os.path.isfile(p)]
    if missing:
        raise NativeDosError("TNT ML toolchain not found: %s" % ", ".join(missing))
    return out


def ml(args, files, artifacts, root=None):
    """TNT ML.EXE natively: ml("/c /Fom.obj m.asm", {"m.asm": text}, ["m.obj"]) -> ({artifact: bytes},
    console text). Raises NativeDosError when the run could not complete."""
    made, console = run_extended(ml_files(root), "ML.EXE", args, files, artifacts)
    return made, console.decode("latin1")


def assemble_many(items, masmroot=None):
    """[(stem, asm_src)] -> {"work": dir, "results": {stem: {obj, lst, log}}}. One MASM run per
    item, in a shared work folder (the real MASM.EXE copied in once). Raises NativeDosError the
    moment any item's run could not complete, so the caller falls the WHOLE call back to DOSBox."""
    from codebreaker.base import settings
    masmroot = masmroot or settings.get("masm")
    work = tempfile.mkdtemp(prefix="masm_nat_")
    exe = _masm_exe(work, masmroot)
    results = {}
    for stem, src in items:
        with open(_stem_path(work, stem, ".ASM"), "w", newline="\r\n") as f:
            f.write(src.replace("\r\n", "\n"))
        _exit, console = _run_tool(exe, work, _cmdline(stem))
        d = _read_out(work, stem)
        d.setdefault("log", console.decode("latin1"))
        results[stem] = d
    return {"work": work, "results": results}
