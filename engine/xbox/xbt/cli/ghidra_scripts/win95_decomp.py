# -*- coding: utf-8 -*-
# win95_decomp.py — Ghidra headless post-script: decompile every function in the
# loaded Win95 module and write each one's C to an output directory.
# @category Win95
#
#   analyzeHeadless <proj> win95 -import files/<mod> -overwrite \
#       -scriptPath ghidra_scripts -postScript win95_decomp.py <outdir>
#
# Ghidra loads Win95's NE modules on x86:LE:16:Real Mode (its 16-bit decompiler
# is the one thing Hex-Rays cannot offer) and PE modules natively. The output is
# a GUESS — invented variable names, and on segmented 16-bit code it can get
# far-pointer arithmetic and control flow wrong. So it is a HYPOTHESIS SOURCE
# (like wow16 / winevdm): read it, confirm against the retail bytes before any
# of it reaches the reconstruction. Runs under Ghidra's Jython (2.7).
import os

from ghidra.app.decompiler import DecompInterface
from ghidra.util.task import ConsoleTaskMonitor


def run():
    args = getScriptArgs()
    outdir = args[0] if args else "."
    if not os.path.isdir(outdir):
        os.makedirs(outdir)

    prog = currentProgram
    fm = prog.getFunctionManager()

    # Promote every cleanly-disassembled symbol to a function so the decompiler
    # covers internals, not just the entry points the loader named.
    st = prog.getSymbolTable()
    made = 0
    for sym in st.getAllSymbols(False):
        a = sym.getAddress()
        if fm.getFunctionAt(a) is not None:
            continue
        if prog.getListing().getInstructionAt(a) is None:
            continue
        try:
            if createFunction(a, sym.getName()) is not None:
                made += 1
        except Exception:
            pass

    ifc = DecompInterface()
    ifc.openProgram(prog)
    mon = ConsoleTaskMonitor()

    n = 0
    for f in fm.getFunctions(True):
        try:
            res = ifc.decompileFunction(f, 60, mon)
        except Exception:
            continue
        if not res.decompileCompleted():
            continue
        ep = f.getEntryPoint()
        nm = f.getName()
        # sanitize BOTH name and address: the 16-bit entry point is "seg:off",
        # and a ':' in a Windows filename opens an alternate data stream (so the
        # '.c' ends up hidden in an ADS and the file looks extension-less).
        stem = "%s_%s" % (nm, ep)
        safe = "".join(ch if (ch.isalnum() or ch in "._") else "_" for ch in stem)
        path = os.path.join(outdir, safe + ".c")
        try:
            fh = open(path, "w")
            fh.write("/* GHIDRA DECOMPILE (HYPOTHESIS - confirm vs retail bytes) "
                     "%s @%s */\n" % (nm, ep))
            fh.write(res.getDecompiledFunction().getC())
            fh.close()
            n += 1
        except Exception:
            pass
    print("win95_decomp: created %d functions, decompiled %d -> %s"
          % (made, n, outdir))


run()
