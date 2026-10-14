"""emulate -- run a piece of the original (or of a rebuild) and record what it did.

One runner per kind of code, all in this package, each returning a record of the same sort --
how the run ended, the result, the calls it made out of the module with their arguments, the
memory it left changed, which of its instructions ran:

    kind       runner                        the code it runs
    pe         emulator.trace_function       a function of a 32-bit Windows module
    ne         windows16.probe               a function of a 16-bit Windows module
    le         driver.run                    an entry point of a virtual device driver (flat ring 0)
    powerpc    powerpc.run                   a function of a PowerPC image

    from codebreaker import emulate
    record = emulate.runner("pe")(path, func=0x401000, args=[1, 2])

The three x86 runners are one engine (unicorn) with one set of memory hooks
(emulator._memory_hooks: what was read and written, pages given on demand, runaway runs cut
off); they differ in how the code is laid out and entered, which is what a kind is. PowerPC is
a processor unicorn does not run usefully in this mode, so powerpc.py is an interpreter of its
own, written to give the same record.

What a record means for a reconstruction -- two runs compared -- is not here: verify/<kind>.py
turns a record into a fingerprint and verify/compare.py compares two of them.
"""

RUNNERS = {"pe": ("codebreaker.emulate.emulator", "trace_function"),
           "ne": ("codebreaker.emulate.windows16", "probe"),
           "le": ("codebreaker.emulate.driver", "run"),
           "powerpc": ("codebreaker.emulate.powerpc", "run")}


def runner(kind):
    """The function that runs code of this kind (the kinds are verify/reconstruct.py's)."""
    import importlib
    module, name = RUNNERS[kind]
    return getattr(importlib.import_module(module), name)
