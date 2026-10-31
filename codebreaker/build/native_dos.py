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
    unset / "auto"   native for the tools PROVEN byte-identical to DOSBox on the sample (MASM), with
                     DOSBox as the automatic fallback when the native run cannot complete
    "1" / "on"       force the native runner (no DOSBox)
    "0" / "off"      force DOSBox (the native runner is not used)
MASM is proven byte-identical (tests/test_dos.py::test_native_masm_matches_dosbox and the gate in
state/work/dos-runner/gate.py); only MASM is defaulted on. The DPMI passes (the Win95 DDK's Phar
Lap TNT ML.EXE, and Microsoft C's protected-mode P3 code generator) run under a DPMI host, not
plain real mode -- they keep using DOSBox until a DPMI host is built (see the module notes / report).

THE NATIVE/C++ SPLIT:
    The hot loop -- the millions of 8086 instructions MASM executes -- already runs inside Unicorn's
    native C engine. With fast=True the executive adds NO per-instruction or per-block Python hook;
    only the INT 21h service runs in Python, and that fires a few thousand times per assemble, not
    per instruction. So the emulation hot path is native already; a C++ rewrite of the INT 21h shim
    would save little and is not warranted. The native tracer work in native/tracer is a separate
    lane (the behaviour oracle), not needed here.

THE DPMI PASSES (deferred, with a plan):
    The executive above is real-mode only. Two period tools do NOT run in plain real mode:
      * the Win95 DDK's ML.EXE (the VxD/driver assembler) is a Phar Lap TNT bound program -- it
        switches to 32-bit protected mode through a DPMI host at start-up; and
      * Microsoft C's P3 code generator (c2.exe / the later CL back-end passes) runs protected via
        a DPMI host too ("Invalid DPMI return" is what a real-mode run gives).
    Those keep using DOSBox (native_dos.enabled() returns False for them under "auto"). Bringing
    them onto our emulator needs a DPMI host beside the real-mode executive:
      1. INT 2Fh AX=1687h -- report a DPMI host and hand back the mode-switch entry (AX=0, the real
         -> protected entry in ES:DI, the host-data paragraph count).
      2. The mode-switch entry: build a tiny protected context -- a flat/para GDT with the LDT
         selectors DPMI clients expect (CS/DS/the PSP and environment selectors), switch Unicorn to
         UC_MODE_32, and run the client's protected entry.
      3. INT 31h DPMI services the tools actually use: 0000/0001 allocate/free LDT descriptors,
         0006/0007/0008 get/set segment base and limit, 0100/0101 allocate/free DOS (real-mode)
         memory with both a selector and a segment, 0200-0205 real-mode interrupt vectors, 0300/0301
         simulate a real-mode interrupt/far call (this is how a protected client reaches the INT 21h
         file API -- route it back through the existing real-mode int21()), 0500/0501 memory-info
         and allocate, 0e00/0e01 the 80x87 state. Unicorn gives GDT/LDT and mode switching, so this
         is service plumbing on top of the executive we already have, not a new CPU.
      4. Load a Phar Lap TNT / bound (.exp-style) image: parse its extender header, lay the 32-bit
         segments out, and start the extended entry in protected mode.
    Scope: a focused module (dpmi.py) of a few hundred lines reusing exact/emu/dos.py's int21().
    Gate, as here: byte-identical .OBJ from the DDK ML and from CL's P3 pass versus DOSBox. Until
    it lands, _PROVEN stays {"masm"} and the DPMI tools fall back to DOSBox automatically.
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


_PROVEN = {"masm"}                               # tools proven byte-identical to DOSBox on the sample


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
