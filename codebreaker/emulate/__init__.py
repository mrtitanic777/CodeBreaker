"""emulate -- run a piece of the original (or of a rebuild) and record what it did.

One runner per kind of code, all in this package, each returning the one run record
(emulate/run.py: Run) -- how the run ended, the result registers, the memory it wrote, the
calls and device accesses out of it in order, how the stack moved, the registers it had to
keep, which of its instructions ran:

    kind       runner (each platform's emulator.run)   the code it runs
    pe         emulator.recorded (trace_function)       a function of a 32-bit Windows module
    ne         windows16.recorded (probe)               a function of a 16-bit Windows module
    le         driver.recorded (run)                    an entry point of a virtual device driver (flat ring 0)
    powerpc    powerpc.recorded (run)                   a function of a PowerPC image
    nes ...    m6502 / z80 / sm83 / m68k .recorded      a function of a console or home-computer program

    from codebreaker import emulate
    run = emulate.runner("pe")(path, func=0x401000, args=[1, 2])      # -> emulate.run.Run

Each module's own record (trace_function's, probe's ...) is still what it was -- run.raw, and
what other tools read; `record(raw)` beside it makes the Run.

The three x86 runners are one engine (unicorn) with one set of memory hooks
(emulator._memory_hooks: what was read and written, pages given on demand, runaway runs cut
off); they differ in how the code is laid out and entered, which is what a kind is. PowerPC is
a processor unicorn does not run usefully in this mode, so powerpc.py is an interpreter of its
own, written to give the same record.

What a record means for a reconstruction -- two runs compared -- is not here: verify/lanes/<kind>.py
turns a record into a fingerprint (lane_common.fingerprint) and verify/compare.py compares two.
"""

def _runners():
    from codebreaker import platforms
    return platforms.runners()


RUNNERS = _runners()                    # each platform's runner (codebreaker/platforms)


def runner(kind):
    """The function that runs code of this kind (the kinds are verify/reconstruct.py's)."""
    import importlib
    module, name = RUNNERS[kind]
    return getattr(importlib.import_module(module), name)
