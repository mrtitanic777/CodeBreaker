"""CodeBreaker -- reconstruct readable, compilable source from binaries, and prove it behaves
like the original.

    base/        where things are; settings; spreading work over cores; progress
    formats/     reading executables (PE, NE, LE, CAB)
    disasm/      instructions and function boundaries
    emulate/     running original and rebuilt code in an emulator and recording what it does
    decompile/   getting C out of a function (our decompiler)
    recover/     what a function's behaviour says about it: arguments, structures, globals
    symbols/     names and prototypes from headers, exports and symbol files
    verify/      building a reconstruction and judging it against the original
    source/      writing the reconstructed module and the scoreboard
    targets/     what is specific to one thing being reconstructed (win95)
    ui/          the progress window
"""
try:
    from codebreaker.base import settings as _settings
except ModuleNotFoundError as _missing:
    if _missing.name != "codebreaker.base":
        raise
    # only codebreaker/ui is here: the terminal interface, a thin client of the native service (ui/client.py)
else:
    _settings.export_for_carried_over_code()
