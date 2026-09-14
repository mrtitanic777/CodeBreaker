#!/usr/bin/env python3
"""nativedec.py -- CodeBreaker hook to our OWN native x86 disassembler+decompiler,
CodeBreaker's native engine (XEXMagic core + our x86 front-end).

One engine, four loaders, so a single call handles whichever class a module is:
  - NE (16-bit)  -> win95dec     (NE load -> x86 decode -> lift[16] -> DecompileToC)
  - PE (32-bit)  -> win95pedec   (PE load -> x86 decode -> lift[32] -> DecompileToC)
  - LE (VxD/386) -> win95ledec   (LE load -> x86 decode -> lift[32,VxD] -> DecompileToC)
  - DOS/MZ       -> win95mzdec   (MZ load -> x86 decode -> lift[16] -> DecompileToC)
The format is detected from the file header and the right binary is invoked, the
same way CodeBreaker invoked Ghidra -- as a subprocess returning C text.

    from src.targets.win95 import nativedec
    c = nativedec.decompile_module(r"...\\WINDOWS\\SYSTEM\\lzexpand.dll")  # NE
    c = nativedec.decompile_module(r"...\\WINDOWS\\SYSTEM\\kernel32.dll")  # PE
    c = nativedec.decompile_module(r"...\\WINDOWS\\SYSTEM\\vmm32.vxd")     # LE
"""
import os
import struct
import subprocess
import tempfile

from src.core import paths

try:
    from src.engines.symbol import demangle as _demangle
except Exception:                       # symbol engine optional; sigs just skipped
    _demangle = None

_BINDIR = os.path.join(paths.repo_root(), "native", "decompiler", "build", "bin")
_BIN = os.path.join(_BINDIR, "win95dec.exe")          # NE engine (back-compat alias)
_PEBIN = os.path.join(_BINDIR, "win95pedec.exe")      # PE engine
_LEBIN = os.path.join(_BINDIR, "win95ledec.exe")      # LE (VxD) engine
_MZBIN = os.path.join(_BINDIR, "win95mzdec.exe")      # DOS/MZ engine


def binary_path():
    return _BIN


def available():
    return os.path.exists(_BIN)


def detect_format(path):
    """'PE'/'NE'/'LE'/'MZ' from the MZ + new-header signature, or None. A file whose
    new-header signature is none of NE/PE/LE/LX (or whose e_lfanew is unusable) is a
    plain DOS/MZ executable -> 'MZ'."""
    try:
        with open(path, "rb") as f:
            d = f.read(0x40)
            if len(d) < 0x40 or d[:2] != b"MZ":
                return None
            e = struct.unpack("<I", d[0x3C:0x40])[0]
            sig = b""
            if 0 < e < 0x10000000:
                with open(path, "rb") as g:
                    g.seek(e)
                    sig = g.read(2)
            if sig == b"PE":
                return "PE"
            if sig == b"NE":
                return "NE"
            if sig == b"LE" or sig == b"LX":
                return "LE"
            return "MZ"                       # MZ with no (recognized) new header = DOS exe
    except OSError:
        return None
    return None


def _engine(path):
    """(exe, module_args, disasm_args, names_args) for this module's format.
    NE's module mode takes a trailing '0'; PE/LE's do not."""
    fmt = detect_format(path)
    if fmt == "PE":
        if not os.path.exists(_PEBIN):
            raise RuntimeError("win95pedec not built: cmake --build native/decompiler/build --target win95pedec")
        return _PEBIN, ["--module"], ["--disasm"], ["--names"]
    if fmt == "LE":
        if not os.path.exists(_LEBIN):
            raise RuntimeError("win95ledec not built: cmake --build native/decompiler/build --target win95ledec")
        return _LEBIN, ["--module"], ["--disasm"], ["--names"]
    if fmt == "MZ":
        if not os.path.exists(_MZBIN):
            raise RuntimeError("win95mzdec not built: cmake --build native/decompiler/build --target win95mzdec")
        return _MZBIN, ["--module"], ["--disasm"], ["--names"]
    if not os.path.exists(_BIN):
        raise RuntimeError("win95dec not built: cmake --build native/decompiler/build --target win95dec")
    return _BIN, ["--module", "0"], ["--disasm"], ["--names"]


def _dump_names(ne_path, timeout):
    """(addr_hex, name) for every function the engine discovers."""
    exe, _mod, _dis, names_args = _engine(ne_path)
    r = subprocess.run([exe, ne_path] + names_args, capture_output=True, text=True, timeout=timeout)
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


def _wrap_raw_mz(path):
    """Wrap a headerless real-mode blob (no MZ/NE/PE/LE header -- e.g. a .COM-style
    or overlay binary that begins directly with 16-bit code) in a synthetic MZ so the
    DOS/MZ 16-bit lifter can load it as a flat code image at CS:IP=0:0. Returns the
    path to a temp .exe, or None if the file already has a real header."""
    if detect_format(path) is not None:
        return None
    blob = open(path, "rb").read()
    if len(blob) < 8 or len(blob) > 0x10000:      # must fit one real-mode segment
        return None
    hdr = bytearray(64)
    hdr[0:2] = b"MZ"
    total = 64 + len(blob)
    struct.pack_into("<H", hdr, 0x02, total % 512)     # bytes in last page
    struct.pack_into("<H", hdr, 0x04, (total + 511) // 512)  # pages
    struct.pack_into("<H", hdr, 0x08, 4)               # header paragraphs (64/16)
    struct.pack_into("<H", hdr, 0x0C, 0xFFFF)          # maxalloc
    struct.pack_into("<H", hdr, 0x10, 0xFFFE)          # sp
    struct.pack_into("<H", hdr, 0x18, 0x40)            # reloc-table offset
    struct.pack_into("<I", hdr, 0x3C, 0)               # e_lfanew=0 -> no new header
    tf = tempfile.NamedTemporaryFile("wb", suffix=".exe", delete=False)
    tf.write(bytes(hdr) + blob); tf.close()
    return tf.name


def decompile_module(ne_path, timeout=300, sigs=True):
    """Decompile every function of a module (NE or PE, auto-detected); returns the
    C source text.

    When the module has mangled C++ exports (MFC/OLE/C++ DLLs) and `sigs` is on,
    a first --names pass is demangled into prototypes (names, return + arg types,
    `this`) fed back via --sigs -- SDK-free typing straight from the symbols."""
    # Headerless real-mode blob: load it via a synthetic MZ wrapper on the MZ engine.
    if detect_format(ne_path) is None:
        wrapped = _wrap_raw_mz(ne_path)
        if wrapped:
            try:
                r = subprocess.run([_MZBIN, wrapped, "--module"],
                                   capture_output=True, text=True, timeout=timeout)
                return r.stdout
            finally:
                try:
                    os.unlink(wrapped)
                except OSError:
                    pass
    exe, mod_args, _dis, _names = _engine(ne_path)
    sig_text = _build_sigs(_dump_names(ne_path, timeout)) if sigs else ""
    if not sig_text:
        r = subprocess.run([exe, ne_path] + mod_args,
                           capture_output=True, text=True, timeout=timeout)
        return r.stdout
    sf = tempfile.NamedTemporaryFile("w", suffix=".sigs", delete=False, encoding="utf-8", newline="\n")
    try:
        sf.write(sig_text); sf.close()
        r = subprocess.run([exe, ne_path] + mod_args + ["--sigs", sf.name],
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
    """Disassemble every code segment/section (NE or PE); returns the listing text."""
    exe, _mod, dis_args, _names = _engine(ne_path)
    r = subprocess.run([exe, ne_path] + dis_args,
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
