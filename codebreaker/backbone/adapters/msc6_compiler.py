#!/usr/bin/env python3
r"""MSC 6.0a compiler adapter -- wraps engine.inverse_engine.msc6_oracle behind the
backbone Compiler interface. This is the compiler that built Win95's 16-bit DLLs;
registering it here lets the reconstruction pipeline (and the inverse verifier) call
`backbone.compiler().compile(src, flags=...)` instead of importing the oracle directly,
so a second compiler (MSVC 1.52c, MSC 7.0) is a sibling adapter, not a call-site rewrite.
"""
from codebreaker.backbone.base import Compiler, ToolResult


class MSC6Compiler(Compiler):
    name = "msc-6.0a"
    version = "Microsoft C 6.00a (16-bit, medium model)"

    def __init__(self):
        self._ora = None
        try:
            from codebreaker.inverse_engine import msc6_oracle as ora
            self._ora = ora
        except Exception:
            self._ora = None

    def available(self) -> bool:
        if self._ora is None:
            return False
        # the oracle is usable only when config.json points at dosbox_x + toolchain
        try:
            return bool(self._ora._cfg().get("dosbox_x")) if hasattr(self._ora, "_cfg") \
                else True
        except Exception:
            return True

    def score(self, **h):
        # prefer this adapter for 16-bit x86 medium-model C targets
        s = 0
        if h.get("bits") == 16:
            s += 2
        if h.get("compiler") in ("msc-6.0a", "msc6", "cl6"):
            s += 5
        return s

    def compile(self, source: str, *, flags=None, **kw) -> ToolResult:
        if self._ora is None:
            return ToolResult(False, stderr="msc6_oracle import failed")
        f = flags or "/AM /G2 /Gsw /Ox"
        try:
            text = self._ora.mc6_compile(source, f, **{k: v for k, v in kw.items()
                                                       if k in ("timeout",)})
            return ToolResult(True, text=text, meta={"flags": f, "bytes": len(text)})
        except Exception as e:
            return ToolResult(False, stderr=str(e), meta={"flags": f})


def register(backbone):
    backbone.register(MSC6Compiler(), default=True)
