"""xenon.py -- XenonRecomp, the Xbox 360 recompiler CodeBreaker's projects are generated with.

    from codebreaker.recompile import xenon
    xenon.build()                      # native/xenonrecomp -> build/XenonRecomp/XenonRecomp.exe
    xenon.recompile(config_toml)       # runs it on a project's configuration (config.py writes one)
    xenon.instruction_tests()          # Xenia's PowerPC tests through it: {"tested", "skipped", "mismatches"}

native/xenonrecomp is a fork of hedge-dev's XenonRecomp (MIT) with the changes the Xbox 360
system-software ports needed (VMX128 compares that set CR6, the import library index, symbol
suffixes for several modules in one program, MMIO stores, the shared indirect-call table). It
turns each guest instruction into one C++ statement on a PPCContext; the runtime
(native/x360) supplies the kernel, XAM, GPU, audio and input.

Built with clang-cl from Visual Studio (setting `visual_studio`) and Ninja. The instruction tests
assemble Xenia's tests (native/xenonrecomp/ppc_tests: inputs and the outputs measured on the
console) with PowerPC binutils that know VMX128 (setting `ppc_binutils`), recompile them, build
them and run them: every printed line is a mismatch.
"""
import glob
import os
import shutil
import subprocess

from codebreaker.base import settings

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
SOURCE = os.path.join(ROOT, "native", "xenonrecomp")
BUILD = os.path.join(SOURCE, "build")
RECOMP = os.path.join(BUILD, "XenonRecomp", "XenonRecomp.exe")
ANALYSE = os.path.join(BUILD, "XenonAnalyse", "XenonAnalyse.exe")
TESTS = os.path.join(SOURCE, "ppc_tests")
# a host-side helper, and a test that needs a switch-table configuration test mode has not
SKIP_TESTS = ("ppc_testing_native_thunks", "seq_jumptable_constants")


def _run(cmd, **kw):
    return subprocess.run(cmd, capture_output=True, text=True, **kw)


def visual_studio():
    """The Visual Studio install that has clang-cl."""
    vs = settings.get("visual_studio")
    if vs:
        return vs
    where = os.path.join(os.environ.get("ProgramFiles(x86)", r"C:\Program Files (x86)"),
                         "Microsoft Visual Studio", "Installer", "vswhere.exe")
    if os.path.isfile(where):
        out = _run([where, "-latest", "-prerelease", "-products", "*", "-requires",
                    "Microsoft.VisualStudio.Component.VC.Tools.x86.x64", "-property",
                    "installationPath"]).stdout.strip()
        if out:
            return out.splitlines()[0]
    raise RuntimeError("no Visual Studio with the C++ x64 tools found (setting visual_studio)")


def clang_cl():
    path = os.path.join(visual_studio(), "VC", "Tools", "Llvm", "x64", "bin", "clang-cl.exe")
    if not os.path.isfile(path):
        raise RuntimeError("clang-cl is not installed: add \"C++ Clang Compiler for Windows\" in the "
                           "Visual Studio Installer")
    return path


_env = None


def msvc_env():
    """The environment vcvars64.bat sets up (the linker, the SDK's headers and libraries)."""
    global _env
    if _env is None:
        bat = os.path.join(visual_studio(), "VC", "Auxiliary", "Build", "vcvars64.bat")
        out = _run(f'cmd /c ""{bat}" >nul && set"', shell=True).stdout
        _env = dict(line.split("=", 1) for line in out.splitlines() if "=" in line)
    return _env


def _newest_source():
    newest = 0.0
    for sub in ("XenonRecomp", "XenonUtils", "XenonAnalyse", "CMakeLists.txt"):
        path = os.path.join(SOURCE, sub)
        files = [path] if os.path.isfile(path) else glob.glob(os.path.join(path, "**", "*"), recursive=True)
        for f in files:
            if os.path.isfile(f):
                newest = max(newest, os.path.getmtime(f))
    return newest


def build(force=False):
    """Build XenonRecomp and XenonAnalyse (only when a source is newer than the executables)."""
    if not force and os.path.isfile(RECOMP) and os.path.isfile(ANALYSE) and \
            min(os.path.getmtime(RECOMP), os.path.getmtime(ANALYSE)) >= _newest_source():
        return RECOMP
    env = msvc_env()
    cc = clang_cl()
    if force or not os.path.isfile(os.path.join(BUILD, "CMakeCache.txt")):
        r = _run(["cmake", "-S", SOURCE, "-B", BUILD, "-G", "Ninja", "-DCMAKE_BUILD_TYPE=Release",
                  "-DCMAKE_C_COMPILER=" + cc, "-DCMAKE_CXX_COMPILER=" + cc], env=env)
        if r.returncode:
            raise RuntimeError("configuring XenonRecomp failed:\n" + (r.stdout + r.stderr)[-3000:])
    r = _run(["cmake", "--build", BUILD, "-j", str(max(1, (os.cpu_count() or 2) // 2))], env=env)
    if r.returncode:
        raise RuntimeError("building XenonRecomp failed:\n" + (r.stdout + r.stderr)[-3000:])
    return RECOMP


def recompile(config):
    """Run XenonRecomp on a configuration file. -> its output (warnings, the unimplemented)."""
    build()
    r = _run([RECOMP, config, os.path.join(SOURCE, "XenonUtils", "ppc_context.h")],
             cwd=os.path.dirname(os.path.abspath(config)))
    if r.returncode:
        raise RuntimeError("XenonRecomp failed:\n" + (r.stdout + r.stderr)[-3000:])
    return r.stdout


def _unix(p):
    return p.replace("\\", "/")


def instruction_tests(names=None, work=None, march="haswell"):
    """Xenia's PowerPC instruction tests (and ours) through XenonRecomp.
    -> {"tested": n, "skipped": [tests with instructions it does not implement],
        "mismatches": [lines the tests printed], "failed": [test names]}"""
    binutils = settings.get("ppc_binutils")
    asm = os.path.join(binutils, "powerpc-none-elf-as.exe")
    objdump = os.path.join(binutils, "powerpc-none-elf-objdump.exe")
    if not os.path.isfile(asm):
        raise RuntimeError("no PowerPC binutils at %s (setting ppc_binutils)" % binutils)
    build()
    work = work or settings.state("xenonrecomp", "ppc_tests")
    src, objs, out = os.path.join(work, "src"), os.path.join(work, "src", "bin"), os.path.join(work, "out")
    for d in (objs, out):
        shutil.rmtree(d, ignore_errors=True)
        os.makedirs(d)
    tested = []
    for s in sorted(glob.glob(os.path.join(TESTS, "*", "*.s"))):
        name = os.path.splitext(os.path.basename(s))[0]
        if (names and name not in names) or name in SKIP_TESTS:
            continue
        shutil.copy(s, src)
        o = os.path.join(objs, name + ".o")
        r = _run([asm, "-a32", "-be", "-mregnames", "-mpower7", "-maltivec", "-mvsx", "-mvmx128", "-R",
                  "-o" + _unix(o), _unix(os.path.join(src, name + ".s"))])
        if r.returncode:
            raise RuntimeError("assembling %s failed: %s" % (name, r.stderr.strip()[:300]))
        r = _run([objdump, "--adjust-vma=0x100000", "-Mpower7", "-Mvmx128", "-D", "-EB", _unix(o)])
        with open(os.path.join(objs, name + ".dis"), "w") as f:
            f.writelines(r.stdout.splitlines(True)[4:])
        tested.append(name)
    # a test of an instruction XenonRecomp does not implement is left out (and listed): a title's
    # own recompile reports any instruction it uses
    r = _run([RECOMP, objs, out])
    skipped = sorted({l.split(" in ")[1].split()[0] for l in r.stdout.splitlines() if "has unimplemented" in l})
    if skipped:
        for name in skipped:
            for ext in (".o", ".dis"):
                os.remove(os.path.join(objs, name + ext))
        shutil.rmtree(out)
        os.makedirs(out)
        _run([RECOMP, objs, out])
    exe = os.path.join(work, "ppc_tests.exe")
    cmd = [clang_cl(), "/nologo", "/std:c++20", "/O1", "/fp:strict", "-fno-strict-aliasing", "-march=" + march,
           "-Wno-everything", "/DFMT_HEADER_ONLY", "/I" + os.path.join(SOURCE, "XenonUtils"),
           "/I" + os.path.join(SOURCE, "thirdparty", "simde"),
           "/I" + os.path.join(SOURCE, "thirdparty", "fmt", "include"), "/Fe" + exe, "/Fo" + work + "\\"]
    r = _run(cmd + sorted(glob.glob(os.path.join(out, "*.cpp"))), env=msvc_env(), cwd=work)
    if r.returncode:
        raise RuntimeError("building the instruction tests failed:\n" + (r.stdout + r.stderr)[-4000:])
    r = _run([exe], cwd=work)
    lines = [l for l in r.stdout.splitlines() if l.strip()]
    return {"tested": len(tested) - len(skipped), "skipped": skipped, "mismatches": lines,
            "failed": sorted({l.split()[0] for l in lines})}
