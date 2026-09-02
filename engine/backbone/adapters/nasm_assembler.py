#!/usr/bin/env python3
r"""NASM `--masm` assembler adapter behind the backbone Assembler interface.

A native, cross-platform, DOSBox-free assembler that emits MASM-matching bytes -- the
vendored nasm-masm fork's `--masm` mode (e.g. `xor eax,eax` -> 33 c0, MASM's encoding,
not NASM's 31 c0). It complements the vintage MASM.EXE-under-DOSBox adapters and serves
as the native fallback when the DOS toolchain is unconfigured.

available() is honest: False unless the built binary is present. Build the vendored
fork (`tools/nasm-masm`, `sh autogen.sh && ./configure && make`, or `nmake -f
Mkfiles/msvc.mak`) and point config.json "nasm" at the resulting `nasm` executable.
"""
import os
import subprocess
import tempfile
from engine.backbone.base import Assembler, ToolResult


def _nasm():
    try:
        from engine.foundation import config
        return config.load_config().get("nasm", "")
    except Exception:
        return ""


class NASMMasm(Assembler):
    name = "nasm-masm"
    version = "NASM 3.02 (--masm)"

    def available(self) -> bool:
        e = _nasm()
        return bool(e and os.path.exists(e))

    def score(self, **h):
        # native + cross-platform (no DOSBox); a strong fallback across bit widths
        return 2 if h.get("bits") in (16, 32, None) else 0

    def assemble(self, source: str, *, flags=None, bits=32, fmt=None, **kw) -> ToolResult:
        exe = _nasm()
        if not exe or not os.path.exists(exe):
            return ToolResult(False, stderr="nasm-masm not configured (config.json 'nasm')")
        of = fmt or ("obj" if bits == 16 else "win32")
        with tempfile.TemporaryDirectory(prefix="nasmmasm_") as d:
            src = os.path.join(d, "m.asm")
            obj = os.path.join(d, "m.obj")
            open(src, "w", encoding="utf-8").write(source + "\n")
            cmd = [exe, "--masm", "-f", of]
            if flags:
                cmd += flags.split() if isinstance(flags, str) else list(flags)
            cmd += ["-o", obj, src]
            try:
                p = subprocess.run(cmd, capture_output=True, timeout=kw.get("timeout", 60))
            except Exception as e:                       # invocation failure -> honest, no fabrication
                return ToolResult(False, stderr="nasm-masm invoke failed: %s" % e)
            if p.returncode != 0 or not os.path.exists(obj):
                return ToolResult(False, stderr=(p.stderr or b"").decode("utf-8", "replace")
                                  or "nasm-masm produced no object")
            return ToolResult(True, output=open(obj, "rb").read(),
                              meta={"assembler": "nasm-masm", "format": of})


def register(backbone):
    backbone.register(NASMMasm())
