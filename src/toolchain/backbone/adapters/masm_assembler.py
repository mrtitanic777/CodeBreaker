#!/usr/bin/env python3
r"""MASM assembler adapters (5.1 / 6.13) behind the backbone Assembler interface.
Resolves the real MASM.EXE through the msc6 toolchain manifest and assembles one
.asm -> .obj under the shared DOSBox runner. available() is honest: False when the
manifest marks the version a gap or DOSBox-X is unconfigured.
"""
import os
from src.toolchain.backbone.base import Assembler, ToolResult
from src.toolchain.backbone.dosbox import run_dos, dosbox_path


class _MASM(Assembler):
    _ver = None

    def _exe(self):
        try:
            from src.engines.msc6 import toolchain
            return toolchain.resolve("assembler", self._ver)
        except Exception:
            return None

    def available(self) -> bool:
        e = self._exe()
        return bool(e and os.path.exists(e) and dosbox_path())

    def score(self, **h):
        return 3 if h.get("bits") == 16 else 0

    def assemble(self, source: str, *, flags=None, **kw) -> ToolResult:
        exe = self._exe()
        if not exe or not os.path.exists(exe):
            return ToolResult(False, stderr="MASM %s not resolved" % self._ver)
        bindir = os.path.dirname(exe); base = os.path.basename(exe)
        f = flags or ""
        ok, out, _ = run_dos(
            {"c": bindir}, ["path c:\\", "%s %s m.asm,m.obj,,; >nul" % (base, f)],
            ["m.obj"], files={"m.asm": source + "\r\n"}, timeout=kw.get("timeout", 90))
        if not ok:
            return ToolResult(False, stderr="assemble produced no object (MASM %s)" % self._ver)
        return ToolResult(True, output=out["m.obj"], meta={"assembler": self._ver})


class MASM51(_MASM):
    name = "masm-5.1"; version = "Microsoft MASM 5.1"; _ver = "masm-5.1"


class MASM613(_MASM):
    name = "masm-6.13"; version = "Microsoft MASM 6.13"; _ver = "masm-6.13"


def register(backbone):
    backbone.register(MASM51(), default=True)      # 5.1 matches the Win95-era build
    backbone.register(MASM613())
