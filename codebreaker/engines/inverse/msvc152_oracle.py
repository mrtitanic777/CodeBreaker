#!/usr/bin/env python3
r"""msvc152_oracle.py -- the MSVC 1.52c compile oracle (Microsoft C/C++ 8.00c, medium model).

Sibling of msc6_oracle.py. The Win95 16-bit NE corpus is a TWO-COMPILER world: a few
modules (lzexpand and its era) were built with MSC 6.0a and carry its frame -- push ds;pop
ax prologue (1e58), dec bp;dec bp;mov sp,bp epilogue. The MAJORITY -- commdlg, complinc,
the AVI/capture DLLs and most drivers -- were built with MSVC 1.52c, whose codegen lays a
visibly different frame: `mov ax,ds` prologue (8cd8) and a `lea sp,[bp-2]` epilogue. A
reconstruction of those modules can only be proven byte-exact by RECOMPILING with 1.52c;
the 6.0a oracle produces the wrong frame and never matches, however correct the body.

    cl /c /AM /G2 /Gsw /Ox   (medium model, optimized) -- emits 8cd8 + lea, the corpus frame.

Same no-fabrication contract as msc6_oracle: when unconfigured, available() is False and
compile() raises rather than guessing. Drives a compiler you own via the same config:
    config.json: dosbox_x, msc6_toolchain (the period root; this appends compilers/msvc-1.52c)

    from codebreaker.engines.inverse import msvc152_oracle as ora
    code = ora.compile("int far pascal F(int a, int b){ return a + b; }")
"""
import os, subprocess, sys, tempfile

_ENGINE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.dirname(_ENGINE))  # repo root, for engine.* imports
from codebreaker.engines.inverse.msc6_oracle import _obj_code   # OMF _TEXT extraction is identical


def _cfg():
    from codebreaker.core import config
    return config.load_config()


def _tcdir(c):
    return os.path.join(c.get("msc6_toolchain", ""), "compilers", "msvc-1.52c")


def available():
    c = _cfg()
    dbx = c.get("dosbox_x", "")
    cl = os.path.join(_tcdir(c), "BIN", "CL.EXE") if c.get("msc6_toolchain") else ""
    return bool(dbx and os.path.exists(dbx) and cl and os.path.exists(cl))


# dynamic core + surface output for speed; INCLUDE/LIB are set because CL 1.52c resolves them
# even for a headerless translation unit. Toolchain mounted C:, scratch D:.
_CONF = ("[sdl]\noutput=surface\n[cpu]\ncore=dynamic\ncputype=386\ncycles=max\n[autoexec]\n"
         "mount c %s\nmount d %s\nset PATH=C:\\BIN\nset INCLUDE=C:\\INCLUDE\nset LIB=C:\\LIB\n"
         "d:\ncall BLD.BAT\nexit\n")


def compile(csrc, flags="/AM /G2 /Gsw /Ox", timeout=120):
    """Compile one C source with MSVC 1.52c under DOSBox-X; return the _TEXT code bytes.
    Raises RuntimeError if the oracle is not configured or the compile produced no object."""
    if not available():
        raise RuntimeError("msvc152 oracle not configured (config.json: dosbox_x + msc6_toolchain)")
    return compile_batch([(csrc, flags)], timeout=timeout)[0] or _raise(flags)


def _raise(flags):
    raise RuntimeError("msvc152 oracle: compile produced no object (flags=%r)" % flags)


def compile_batch(jobs, timeout=300):
    """Compile MANY (csrc, flags) jobs in ONE DOSBox run -- the spawn cost is paid once, not
    per compile. Returns a list of code-bytes, None where a job produced no object."""
    if not available():
        raise RuntimeError("msvc152 oracle not configured")
    c = _cfg(); dbx = c["dosbox_x"]; tcdir = _tcdir(c)
    work = tempfile.mkdtemp(prefix="msvc152_")
    lines = ["@echo off", "path c:\\bin", "del *.obj"]
    for i, (csrc, flags) in enumerate(jobs):
        open(os.path.join(work, "m%d.c" % i), "w").write(csrc + "\n")
        lines.append("cl /c %s /Fom%d.obj m%d.c >nul 2>&1" % (flags, i, i))
    lines.append("echo DONE > done.txt")
    open(os.path.join(work, "BLD.BAT"), "w", newline="").write("\r\n".join(lines) + "\r\n")
    open(os.path.join(work, "r.conf"), "w").write(_CONF % (
        tcdir.replace("/", "\\"), work.replace("/", "\\")))
    subprocess.run([dbx, "-conf", os.path.join(work, "r.conf"), "-exit"],
                   stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, timeout=timeout)
    out = []
    for i in range(len(jobs)):
        obj = os.path.join(work, "m%d.obj" % i)
        try:
            out.append(_obj_code(obj) if os.path.exists(obj) else None)
        except Exception:
            out.append(None)
    return out


if __name__ == "__main__":
    if not available():
        print("msvc152 oracle: NOT configured (need config.json dosbox_x + msc6_toolchain)")
        sys.exit(1)
    # self-test: the medium-model frame is the corpus 8cd8 + lea signature
    code = compile("int far pascal F(int a, int b){ int x; x = a + b; return x; }")
    print("F add: %s (%d bytes)" % (code.hex(), len(code)))
    print("  prologue 8cd8:", code[:2].hex() == "8cd8", " lea epilogue:", "8d66fe" in code.hex())
