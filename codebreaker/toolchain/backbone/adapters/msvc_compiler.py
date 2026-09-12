#!/usr/bin/env python3
r"""MSVC (cl.exe) x86 compiler adapter -- the 32-bit-PE sibling of msc6_compiler.

Drives a Microsoft Visual C++ `cl.exe` behind the backbone Compiler interface, so
the reconstruction/verify path can compile recovered C for a flat 32-bit PE and
compare the emitted .text to the shipped bytes. It is toolchain-agnostic: the
exact cl.exe (and its INCLUDE/LIB env) come from config.json, so pointing it at,
say, a VC7.1 (Visual C++ 2003) install is configuration, not code.

config.json keys (any absolute paths):
    msvc_cl        full path to cl.exe                       (required)
    msvc_include   ';'-joined INCLUDE dirs                   (optional)
    msvc_lib       ';'-joined LIB dirs                       (optional)
    msvc_env_path  ';'-joined dirs prepended to PATH so cl finds its DLLs
                   (mspdb*.dll, msobj*.dll); optional

available() is False unless msvc_cl exists, so auto-selection never substitutes a
different compiler (the no-fabrication doctrine). compile() runs `cl /c`, returns
the OBJ in .output and its extracted .text (COFF) in .text.
"""
import os
import struct
import subprocess
import tempfile

from codebreaker.toolchain.backbone.base import Compiler, ToolResult


def _cfg():
    try:
        from codebreaker.core import config
        return config.load_config()
    except Exception:
        return {}


def _coff_text(obj: bytes) -> bytes:
    """Return the raw bytes of the .text section of a COFF object, or b''."""
    if len(obj) < 20:
        return b""
    nsec = struct.unpack_from("<H", obj, 2)[0]
    opt = struct.unpack_from("<H", obj, 16)[0]
    p = 20 + opt                                   # section headers follow the header
    for _ in range(nsec):
        hdr = obj[p:p + 40]
        if len(hdr) < 40:
            break
        name = hdr[:8].rstrip(b"\0")
        size = struct.unpack_from("<I", hdr, 16)[0]
        ptr = struct.unpack_from("<I", hdr, 20)[0]
        if name == b".text" and ptr and size:
            return obj[ptr:ptr + size]
        p += 40
    return b""


class MSVCCompiler(Compiler):
    name = "msvc-x86"
    version = "Microsoft Visual C++ (cl.exe, 32-bit x86) — path from config.json"

    def _cl(self):
        return _cfg().get("msvc_cl", "")

    def available(self) -> bool:
        cl = self._cl()
        return bool(cl) and os.path.exists(cl)

    def score(self, **h):
        s = 0
        if h.get("bits") == 32:
            s += 2
        if h.get("compiler") in ("msvc", "msvc-x86", "cl", "vc7", "vc71"):
            s += 5
        return s

    def compile(self, source: str, *, flags=None, **kw) -> ToolResult:
        cl = self._cl()
        if not cl or not os.path.exists(cl):
            return ToolResult(False, stderr="msvc_cl not configured / not found")
        cfg = _cfg()
        env = dict(os.environ)
        if cfg.get("msvc_include"):
            env["INCLUDE"] = cfg["msvc_include"]
        if cfg.get("msvc_lib"):
            env["LIB"] = cfg["msvc_lib"]
        if cfg.get("msvc_env_path"):
            env["PATH"] = cfg["msvc_env_path"] + os.pathsep + env.get("PATH", "")
        # default flags aim at matching-friendly output; caller overrides per module.
        fl = flags if flags is not None else "/c /O1 /GS- /Gd /Zl"
        with tempfile.TemporaryDirectory(prefix="msvc_") as d:
            src = os.path.join(d, "u.c")
            obj = os.path.join(d, "u.obj")
            with open(src, "w", encoding="latin1") as f:
                f.write(source)
            args = [cl] + fl.split() + ["/Fo" + obj, src]
            if "/c" not in fl.split():
                args.insert(1, "/c")
            try:
                p = subprocess.run(args, capture_output=True, text=True, env=env,
                                   cwd=d, timeout=kw.get("timeout", 120))
            except Exception as e:
                return ToolResult(False, stderr="cl invocation failed: %s" % e,
                                  meta={"flags": fl})
            if not os.path.exists(obj):
                return ToolResult(False, stdout=p.stdout, stderr=p.stderr or
                                  "cl produced no .obj", meta={"flags": fl})
            data = open(obj, "rb").read()
            return ToolResult(True, output=data, text=_coff_text(data),
                              stdout=p.stdout, stderr=p.stderr,
                              meta={"flags": fl, "obj_bytes": len(data)})


def register(backbone):
    # not default: the 16-bit msc-6.0a stays the default compiler; this is selected
    # explicitly (compiler='msvc-x86') or by detect(bits=32).
    backbone.register(MSVCCompiler(), default=False)


if __name__ == "__main__":
    import sys
    a = MSVCCompiler()
    print("msvc-x86 available:", a.available(), "cl:", a._cl() or "(unset)")
    if a.available() and len(sys.argv) > 1:
        r = a.compile(open(sys.argv[1]).read())
        print(r, "text=%dB" % len(r.text or b""))
        if not r.ok:
            print(r.stderr[:400])
