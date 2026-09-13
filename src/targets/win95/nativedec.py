#!/usr/bin/env python3
"""nativedec.py -- CodeBreaker hook to our OWN native x86 disassembler+decompiler
(native/xexmagic/win95dec), the XEXMagic-derived engine ported to Win95 x86.

Replaces the Ghidra dependency for NE (16-bit) modules: NE load -> x86 decode ->
x86->IL lift -> the reused DecompileToC back-end. Invokes the built binary the
same way CodeBreaker invoked Ghidra -- as a subprocess returning C text.

    from src.targets.win95 import nativedec
    c = nativedec.decompile_module(r"...\\WINDOWS\\SYSTEM\\lzexpand.dll")
"""
import os
import subprocess

from src.core import paths

_BIN = os.path.join(paths.repo_root(), "native", "xexmagic", "build", "bin", "win95dec.exe")


def binary_path():
    return _BIN


def available():
    return os.path.exists(_BIN)


def decompile_module(ne_path, timeout=300):
    """Decompile every function of an NE module; returns the C source text."""
    if not available():
        raise RuntimeError("win95dec not built: cmake --build native/xexmagic/build --target win95dec")
    r = subprocess.run([_BIN, ne_path, "--module", "0"],
                       capture_output=True, text=True, timeout=timeout)
    return r.stdout


def decompile_function(ne_path, entry, end, name="func", timeout=120):
    """Decompile one function spanning [entry, end) (linear addresses)."""
    if not available():
        raise RuntimeError("win95dec not built")
    r = subprocess.run([_BIN, ne_path, hex(entry), hex(end), name],
                       capture_output=True, text=True, timeout=timeout)
    return r.stdout


def disassemble_module(ne_path, timeout=300):
    """Disassemble every code segment; returns the instruction listing text."""
    if not available():
        raise RuntimeError("win95dec not built: cmake --build native/xexmagic/build --target win95dec")
    r = subprocess.run([_BIN, ne_path, "--disasm"],
                       capture_output=True, text=True, timeout=timeout)
    return r.stdout


if __name__ == "__main__":
    import sys
    if len(sys.argv) < 2:
        print("usage: nativedec.py <ne> [entry end name]")
        raise SystemExit(2)
    if len(sys.argv) >= 4:
        print(decompile_function(sys.argv[1], int(sys.argv[2], 16), int(sys.argv[3], 16),
                                 sys.argv[4] if len(sys.argv) > 4 else "func"))
    else:
        print(decompile_module(sys.argv[1]))
