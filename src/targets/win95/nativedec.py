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
import tempfile

from src.core import paths

try:
    from src.engines.symbol import demangle as _demangle
except Exception:                       # symbol engine optional; sigs just skipped
    _demangle = None

_BIN = os.path.join(paths.repo_root(), "native", "xexmagic", "build", "bin", "win95dec.exe")


def binary_path():
    return _BIN


def available():
    return os.path.exists(_BIN)


def _dump_names(ne_path, timeout):
    """(addr_hex, name) for every function win95dec discovers."""
    r = subprocess.run([_BIN, ne_path, "--names"], capture_output=True, text=True, timeout=timeout)
    out = []
    for line in r.stdout.splitlines():
        if "\t" in line:
            addr, name = line.split("\t", 1)
            out.append((addr, name))
    return out


def _build_sigs(names):
    """Demangle mangled function names into win95dec --sigs records. A mangled C++
    name is a complete prototype, so this types functions with no SDK. Returns the
    sig-file text (addr<TAB>display<TAB>ret<TAB>arg[|name]...), or "" if none."""
    if _demangle is None:
        return ""
    lines = []
    for addr, name in names:
        if not name.startswith("?"):
            continue
        s = _demangle.demangle_sig(name)
        if not s or s.get("kind") != "func":
            continue
        args = []
        if s.get("this_type"):
            args.append(s["this_type"] + "|this")   # implicit `this` = first arg (a1)
        args += s.get("args", [])
        rec = [addr, s["name"], s.get("ret") or ""] + args
        lines.append("\t".join(rec))
    return "\n".join(lines) + ("\n" if lines else "")


def decompile_module(ne_path, timeout=300, sigs=True):
    """Decompile every function of an NE module; returns the C source text.

    When the module has mangled C++ exports (MFC/OLE/C++ DLLs) and `sigs` is on,
    a first --names pass is demangled into prototypes (names, return + arg types,
    `this`) fed back via --sigs -- SDK-free typing straight from the symbols."""
    if not available():
        raise RuntimeError("win95dec not built: cmake --build native/xexmagic/build --target win95dec")
    sig_text = _build_sigs(_dump_names(ne_path, timeout)) if sigs else ""
    if not sig_text:
        r = subprocess.run([_BIN, ne_path, "--module", "0"],
                           capture_output=True, text=True, timeout=timeout)
        return r.stdout
    sf = tempfile.NamedTemporaryFile("w", suffix=".sigs", delete=False, encoding="utf-8", newline="\n")
    try:
        sf.write(sig_text); sf.close()
        r = subprocess.run([_BIN, ne_path, "--module", "0", "--sigs", sf.name],
                           capture_output=True, text=True, timeout=timeout)
        return r.stdout
    finally:
        try:
            os.unlink(sf.name)
        except OSError:
            pass


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
