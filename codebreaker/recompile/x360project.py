"""x360project.py -- an Xbox 360 title as a PC project on CodeBreaker's Xbox 360 runtime.

    python -m codebreaker recompile-project <default.xex> [-o <folder>] [--name N] [--title-files <dir>]
    from codebreaker.recompile import x360project
    info = x360project.scaffold(xex_path, folder, title_files=None)

The folder is the binary's project folder (codebreaker/base/converted.py: Converted/<name>/)
unless -o names another:

    <folder>/
      CMakeLists.txt        x360_title(<name> ...) from native/x360/x360.cmake
      src/                  the title's XenonRecomp configuration (config.py), its switch tables
                            (switches.py, with XenonAnalyse's), and default.xex in basic form
      src/ppc/              XenonRecomp's C++ (one statement per guest instruction), regenerated:
                            only changed files are rewritten, so a rebuild recompiles only those
      src/module_info.cpp   the title described to the runtime (modules.h: g_titleModule)
      assets/               the title folder the program runs: its files (--title-files) and
                            default.xex in basic form; it is the title's game: device
      build/                the CMake build: build/<name>.exe
      reports/recompile.log what XenonRecomp said
      Build-<name>.bat      configures and builds with clang-cl and Ninja
      Play-<name>.bat       runs it

The program is native/x360's runtime with the title linked in; run it as
`build/<name>.exe assets` (or from assets/).
"""
import filecmp
import os
import re
import shutil
import subprocess

from codebreaker.formats import xex
from codebreaker.recompile import config, discover, xenon

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))


def _name(xex_path):
    stem = os.path.splitext(os.path.basename(xex_path))[0].lower()
    return "".join(c if c.isalnum() else "_" for c in stem) or "title"


def _sync(src, dst):
    """Make dst hold src's files, rewriting only those that changed and removing the rest."""
    os.makedirs(dst, exist_ok=True)
    have = set(os.listdir(src))
    for name in have:
        a, b = os.path.join(src, name), os.path.join(dst, name)
        if not (os.path.isfile(b) and filecmp.cmp(a, b, shallow=False)):
            shutil.copy2(a, b)
    for name in os.listdir(dst):
        if name not in have:
            os.remove(os.path.join(dst, name))


def scaffold(xex_path, out_dir, name=None, title_files=None):
    """-> {"name", "dir", "functions", "switches", "unimplemented"}."""
    # the program's, target's and scripts' name: letters, digits and underscores ("Pac-Man C.E." -> Pac_Man_C_E)
    name = "_".join(filter(None, re.split(r"[^A-Za-z0-9]+", name or _name(xex_path)))) or "title"
    out_dir = os.path.abspath(out_dir)  # XenonRecomp runs inside it and is given paths into it
    if name == "default":
        name = "title"
    recomp = os.path.join(out_dir, "src")
    game = os.path.join(out_dir, "assets")
    reports = os.path.join(out_dir, "reports")
    for d in (recomp, game, reports):
        os.makedirs(d, exist_ok=True)
    basic = os.path.join(recomp, "default.xex")
    xex.basic(xex_path, basic)

    # switch tables: switches.py's, with XenonAnalyse's for any bctr it leaves unresolved
    xenon.build()
    analysed = os.path.join(recomp, "switch_tables.analysed.toml")
    try:                       # optional: switches.py finds nearly all of them by itself
        subprocess.run([xenon.ANALYSE, basic, analysed], capture_output=True)
    except OSError as e:
        print("  (XenonAnalyse did not run: %s; switch tables from switches.py alone)" % e)
    with_analysed = open(analysed).read() if os.path.isfile(analysed) else None
    found, added = config.write_switch_tables(os.path.join(recomp, "switch_tables.toml"), basic, with_analysed)

    # the configuration, then XenonRecomp into a fresh folder, synced into recomp/ppc
    cfg = os.path.join(recomp, name + ".toml")
    with open(cfg, "w", newline="\n") as f:
        f.write(config.make(basic, out_dir="ppc.new", switch_tables="switch_tables.toml", file_path="default.xex"))
    fresh = os.path.join(recomp, "ppc.new")
    shutil.rmtree(fresh, ignore_errors=True)
    os.makedirs(fresh)
    xenon.build()
    r = subprocess.run([xenon.RECOMP, cfg, os.path.join(xenon.SOURCE, "XenonUtils", "ppc_context.h")],
                       capture_output=True, text=True, cwd=recomp)
    log = (r.stdout or "") + (r.stderr or "")
    with open(os.path.join(reports, "recompile.log"), "w") as f:
        f.write(log)
    if r.returncode or not os.path.isfile(os.path.join(fresh, "ppc_func_mapping.cpp")):
        raise RuntimeError("XenonRecomp failed (%s):\n%s" % (r.returncode, log[-3000:]))
    _sync(fresh, os.path.join(recomp, "ppc"))
    shutil.rmtree(fresh, ignore_errors=True)

    helpers = config.helpers(discover._image(basic))
    with open(os.path.join(recomp, "module_info.cpp"), "w", newline="\n") as f:
        f.write('''// The recompiled title described to the runtime (modules.h), built with its own generated ppc_config.h.
// Written by CodeBreaker (codebreaker/recompile/x360project.py).
#include "ppc_config.h"
#include "ppc_context.h"

#include "modules.h"

extern PPCFuncMapping PPCFuncMappings[];

extern const modules::RecompiledModule g_titleModule = {
    "default.xex",
    uint32_t(PPC_IMAGE_BASE),
    uint32_t(PPC_IMAGE_SIZE),
    uint32_t(PPC_CODE_BASE),
    uint32_t(PPC_CODE_SIZE),
    PPCFuncMappings,
    0x%08X, // __savegprlr_14
};
''' % helpers.get("savegprlr_14_address", 0))
    runtime = os.path.join(ROOT, "native", "x360", "x360.cmake").replace("\\", "/")
    with open(os.path.join(out_dir, "CMakeLists.txt"), "w", newline="\n") as f:
        f.write('''# Written by CodeBreaker (codebreaker/recompile/x360project.py).
cmake_minimum_required(VERSION 3.20)
project(%s CXX)
include("%s")
x360_title(%s "${CMAKE_CURRENT_SOURCE_DIR}/src/ppc" "${CMAKE_CURRENT_SOURCE_DIR}/src/module_info.cpp")
''' % (name, runtime, name))
    vs = xenon.visual_studio()
    with open(os.path.join(out_dir, "Build-%s.bat" % name), "w", newline="\r\n") as f:
        f.write('''@echo off
rem Builds %s with clang-cl and Ninja (written by CodeBreaker). Run: build\\%s.exe assets
call "%s\\VC\\Auxiliary\\Build\\vcvars64.bat" >nul 2>&1 || exit /b 1
set "LLVM=%s\\VC\\Tools\\Llvm\\x64\\bin"
cd /d "%%~dp0"
if not exist build\\CMakeCache.txt cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release "-DCMAKE_C_COMPILER=%%LLVM%%\\clang-cl.exe" "-DCMAKE_CXX_COMPILER=%%LLVM%%\\clang-cl.exe" || exit /b 1
if defined CB_BUILD_JOBS (cmake --build build -j %%CB_BUILD_JOBS%% || exit /b 1) else (cmake --build build || exit /b 1)
''' % (name, name, vs, vs))

    with open(os.path.join(out_dir, "Play-%s.bat" % name), "w", newline="\r\n") as f:
        f.write('@echo off\nrem Plays %s (written by CodeBreaker): keyboard or controller; the log is assets\\%s.log\n'
                'start "" "%%~dp0build\\%s.exe" "%%~dp0assets"\n' % (name, name, name))

    # the title folder: its files, and default.xex in basic form
    if title_files:
        for entry in os.listdir(title_files):
            src = os.path.join(title_files, entry)
            dst = os.path.join(game, entry)
            if entry.lower() == "default.xex":
                continue
            if os.path.isdir(src):
                shutil.copytree(src, dst, dirs_exist_ok=True)
            else:
                shutil.copy2(src, dst)
    shutil.copy2(basic, os.path.join(game, "default.xex"))

    mapping = open(os.path.join(recomp, "ppc", "ppc_func_mapping.cpp")).read()
    return {"name": name, "dir": out_dir, "functions": mapping.count("{ 0x"),
            "switches": found + added, "unimplemented": log.count("unimplemented")}


def build(out_dir, name, jobs=4):
    """Runs Build-<name>.bat with at most `jobs` compiles at once. -> (ok, the last lines of its output)"""
    env = dict(os.environ, CB_BUILD_JOBS=str(jobs))
    r = subprocess.run(["cmd", "/c", os.path.join(out_dir, "Build-%s.bat" % name)], cwd=out_dir, env=env,
                       capture_output=True, text=True, errors="replace")
    out = (r.stdout + r.stderr).splitlines()
    with open(os.path.join(out_dir, "reports", "build.log"), "w", encoding="utf-8") as f:
        f.write("\n".join(out))
    errors = [line for line in out if " error" in line or "error:" in line]
    return r.returncode == 0, (errors or out)[-12:]


def check(out_dir, name, seconds=60):
    """Runs the built title headless for `seconds` and reads its log.
    -> {"frames", "unimplemented": {import: calls}, "fatal": message or "", "log"}"""
    game = os.path.join(out_dir, "assets")
    exe = os.path.join(out_dir, "build", "%s.exe" % name)
    log = os.path.join(game, "%s.log" % name)
    if os.path.exists(log):
        os.remove(log)
    env = dict(os.environ, CB_HEADLESS="1", CB_WATCHDOG="20", CB_CONTINUE_UNIMPLEMENTED="1")
    proc = subprocess.Popen([exe, game], cwd=game, env=env, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    try:
        proc.wait(timeout=seconds)
    except subprocess.TimeoutExpired:
        proc.kill()
        proc.wait()
    text = open(log, encoding="utf-8", errors="replace").read() if os.path.exists(log) else ""
    frames = [int(m) for m in re.findall(r"GPU: frame (\d+)", text)]
    missing = {}
    for m in re.findall(r"UNIMPLEMENTED IMPORT (\w+)", text):
        missing[m] = missing.get(m, 0) + 1
    fatal = re.findall(r"FATAL: (.*)", text)
    stall = re.findall(r"no frame presented for \d+ s after frame \d+", text)
    return {"frames": max(frames) if frames else 0, "unimplemented": missing,
            "fatal": fatal[0] if fatal else (stall[0] if stall else ""),
            "exit": proc.returncode, "log": log}
