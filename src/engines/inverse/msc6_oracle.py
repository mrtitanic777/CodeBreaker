#!/usr/bin/env python3
r"""msc6_oracle.py -- the MSC 6.0a compile oracle for a Compiler-Inverse model of
Microsoft C 6.00a (medium model / optimized), the compiler that built the Win95 16-bit
NE DLLs.

Parallel to oracle16.py (which drives MC1/MC2 = MS C 2.03). Where that inverts an
UNoptimized small-model compiler, this one targets OPTIMIZED medium-model MSC 6.0a
(`cl /c /AM /G2 /Gsw /Ox` and per-DLL variants). The inverse proves a reconstruction
byte-exact by RECOMPILING it with the real compiler under DOSBox-X and diffing the code
bytes (relocations masked upstream).

CodeBreaker ships no compiler. This drives one you own, via the msc6 engine's config:
    config.json: dosbox_x, msc6_toolchain (has compilers/msc-6.0a/BIN/CL.EXE)
When unconfigured, available() is False and mc6_compile() raises rather than guessing --
the same no-fabrication contract as oracle16.

    from src.engines.inverse import msc6_oracle as ora
    code = ora.mc6_compile("int far pascal WEP(int a){ return 1; }", flags="/AM /G2 /Gsw /Os")
"""
import os, struct, subprocess, sys, tempfile, time

_ENGINE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.dirname(_ENGINE))  # repo root, for engine.* imports


def _cfg():
    from src.core import config
    return config.load_config()


def available():
    c = _cfg()
    dbx = c.get("dosbox_x", "")
    tc = c.get("msc6_toolchain", "")
    cl = os.path.join(tc, "compilers", "msc-6.0a", "BIN", "CL.EXE") if tc else ""
    return bool(dbx and os.path.exists(dbx) and cl and os.path.exists(cl))


def _obj_code(objpath):
    """Extract the _TEXT code bytes from an OMF .obj (LEDATA records)."""
    d = open(objpath, "rb").read()
    p, c = 0, b""
    while p < len(d):
        rec = d[p]; ln = struct.unpack_from("<H", d, p + 1)[0]
        body = d[p + 3:p + 3 + ln - 1]; p += 3 + ln
        if rec in (0xA0, 0xA1):                       # LEDATA / LIDATA
            i = 0; si = body[i]; i += 1
            if si >= 0x80:
                i += 1
            i += 2; c += body[i:]
    return c


def mc6_compile(csrc, flags="/AM /G2 /Gsw /Ox", timeout=90):
    """Compile one C source with MSC 6.0a under DOSBox-X; return the _TEXT code bytes.
    Raises RuntimeError if the oracle is not configured or the compile fails."""
    if not available():
        raise RuntimeError("msc6 oracle not configured (config.json: dosbox_x + msc6_toolchain)")
    c = _cfg()
    dbx = c["dosbox_x"]
    tcdir = os.path.join(c["msc6_toolchain"], "compilers", "msc-6.0a")
    work = tempfile.mkdtemp(prefix="msc6ora_")
    open(os.path.join(work, "m.c"), "w").write(csrc + "\n")
    open(os.path.join(work, "BLD.BAT"), "w").write(
        "@echo off\r\npath c:\\bin\r\ndel m.obj\r\ncl /c %s /Fom.obj m.c >nul\r\n"
        "echo DONE > done.txt\r\n" % flags)
    open(os.path.join(work, "r.conf"), "w").write(_CONF % (
        tcdir.replace("/", "\\"), work.replace("/", "\\")))
    for f in ("m.obj", "done.txt"):
        try: os.remove(os.path.join(work, f))
        except OSError: pass
    subprocess.run([dbx, "-conf", os.path.join(work, "r.conf"), "-exit"],
                   stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, timeout=timeout)
    obj = os.path.join(work, "m.obj")
    if not os.path.exists(obj):
        raise RuntimeError("msc6 oracle: compile produced no object (flags=%r)" % flags)
    return _obj_code(obj)


# dynamic recompiler core + surface output = faster emulation and init than the default
_CONF = ("[sdl]\noutput=surface\n[cpu]\ncore=dynamic\ncputype=386\ncycles=max\n[autoexec]\n"
         "mount c %s\nmount d %s\nset PATH=C:\\BIN\nd:\ncall BLD.BAT\nexit\n")


def mc6_compile_batch(jobs, timeout=300):
    """Compile MANY (csrc, flags) jobs in ONE DOSBox run -- the ~1.8s spawn is paid once,
    not per compile (it is 58% of a single compile). Returns a list of code-bytes, with
    None where a job produced no object. This is the primary DOSBox-cost reduction."""
    if not available():
        raise RuntimeError("msc6 oracle not configured")
    c = _cfg(); dbx = c["dosbox_x"]
    tcdir = os.path.join(c["msc6_toolchain"], "compilers", "msc-6.0a")
    work = tempfile.mkdtemp(prefix="msc6batch_")
    lines = ["@echo off", "path c:\\bin", "del *.obj"]
    for i, (csrc, flags) in enumerate(jobs):
        open(os.path.join(work, "m%d.c" % i), "w").write(csrc + "\n")
        lines.append("cl /c %s /Fom%d.obj m%d.c >nul" % (flags, i, i))
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
        print("msc6 oracle: NOT configured (need config.json dosbox_x + msc6_toolchain)")
        sys.exit(1)
    # self-test: WEP round-trips to the known byte-exact forms
    for flags, src, label in [
        ("/AM /G2 /Gsw /Os", "int far pascal WEP(a) int a; { return 1; }", "VER WEP (/Os)"),
        ("/AM /G2 /Gsw /Ox", "int far pascal WEP(a) int a; { int i; }", "LZEXPAND WEP (/Ox)"),
    ]:
        code = mc6_compile(src, flags)
        print("%-22s %s  (%d bytes)" % (label, code.hex(), len(code)))
