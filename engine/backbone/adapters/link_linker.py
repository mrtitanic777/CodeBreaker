#!/usr/bin/env python3
r"""LINK linker adapters (5.20 / 5.60) behind the backbone Linker interface.

5.20 emits the 0xFFFF chained relocations of the shipped Win95 DLLs; 5.60.339 emits
direct-offset relocations and needs DOSXNT.EXE alongside it. Both are real, present
tools resolved via the msc6 manifest; this adapter runs one through the shared DOSBox
runner given a set of objects, a .DEF, and libraries (or a ready-made response file).
"""
import os
from engine.backbone.base import Linker, ToolResult
from engine.backbone.dosbox import run_dos, dosbox_path


class _LINK(Linker):
    _ver = None

    def _exe(self):
        try:
            from engine.msc6 import toolchain
            return toolchain.resolve("linker", self._ver)
        except Exception:
            return None

    def available(self) -> bool:
        e = self._exe()
        return bool(e and os.path.exists(e) and dosbox_path())

    def score(self, **h):
        # 5.20 reproduces the shipped chained relocs; prefer it for byte-exact NE work
        want = h.get("reloc")
        if want == "chained":
            return 5 if self._ver == "5.20" else 0
        if want == "direct":
            return 5 if self._ver == "5.60" else 0
        return 2 if self._ver == "5.20" else 1

    def link(self, objs, *, defs=None, libs=None, opts=None, response=None, **kw) -> ToolResult:
        """objs: list[(name,bytes)]. defs: (name,text). libs: list[(name,bytes)].
        response: optional ready-made .lnk text (overrides the generated command)."""
        exe = self._exe()
        if not exe or not os.path.exists(exe):
            return ToolResult(False, stderr="LINK %s not resolved" % self._ver)
        bindir = os.path.dirname(exe); base = os.path.basename(exe)
        files = {}
        for nm, data in (objs or []):
            files[nm] = data
        for nm, data in (libs or []):
            files[nm] = data
        defname = None
        if defs:
            defname, deftext = defs; files[defname] = deftext
        objlist = " ".join(nm for nm, _ in (objs or []))
        liblist = "+".join(nm for nm, _ in (libs or [])) if libs else ""
        outname = kw.get("out", "out.dll")
        if response is not None:
            files["lz.lnk"] = response
            cmd = "%s @lz.lnk >nul" % base
        else:                                            # obj,exe,map,libs,def / opts
            o = (opts or "/NOPACKCODE")
            cmd = "%s %s %s,%s,nul,%s,%s %s >nul" % (
                base, o, objlist, outname, liblist, defname or "", "")
        ok, out, _ = run_dos({"c": bindir}, ["path c:\\", cmd], [outname],
                             files=files, timeout=kw.get("timeout", 120))
        if not ok:
            return ToolResult(False, stderr="link produced no image (LINK %s)" % self._ver)
        return ToolResult(True, output=out[outname], meta={"linker": self._ver, "out": outname})


class LINK520(_LINK):
    name = "link-5.20"; version = "Microsoft LINK 5.20 (chained relocs)"; _ver = "5.20"


class LINK560(_LINK):
    name = "link-5.60"; version = "Microsoft LINK 5.60.339 (direct relocs)"; _ver = "5.60"


def register(backbone):
    backbone.register(LINK520(), default=True)     # chained relocs = the shipped form
    backbone.register(LINK560())
