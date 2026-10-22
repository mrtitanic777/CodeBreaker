"""x360project.py -- an Xbox 360 title as a PC project on CodeBreaker's Xbox 360 runtime.

    python -m codebreaker recompile-project <default.xex> -o <folder> [--title-files <dir>]
    from codebreaker.recompile import x360project
    info = x360project.scaffold(xex_path, folder, title_files=None)

    <folder>/
      CMakeLists.txt        x360_title(<name> ...) from native/x360/x360.cmake
      recomp/               the title's XenonRecomp configuration (config.py), its switch tables
                            (switches.py, with XenonAnalyse's), and default.xex in basic form
      recomp/ppc/           XenonRecomp's C++ (one statement per guest instruction), regenerated:
                            only changed files are rewritten, so a rebuild recompiles only those
      recomp/module_info.cpp  the title described to the runtime (modules.h: g_titleModule)
      game/                 the title folder the program runs: its files (--title-files) and
                            default.xex in basic form; it is the title's game: device
      Build-<name>.bat      configures and builds with clang-cl and Ninja

The program is native/x360's runtime with the title linked in; run it as
`build/<name>.exe game` (or from game/).
"""
import filecmp
import os
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
    name = name or _name(xex_path)
    if name == "default":
        name = "title"
    recomp = os.path.join(out_dir, "recomp")
    game = os.path.join(out_dir, "game")
    for d in (recomp, game):
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
    with open(os.path.join(recomp, "recompile.log"), "w") as f:
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
x360_title(%s "${CMAKE_CURRENT_SOURCE_DIR}/recomp/ppc" "${CMAKE_CURRENT_SOURCE_DIR}/recomp/module_info.cpp")
''' % (name, runtime, name))
    vs = xenon.visual_studio()
    with open(os.path.join(out_dir, "Build-%s.bat" % name), "w", newline="\r\n") as f:
        f.write('''@echo off
rem Builds %s with clang-cl and Ninja (written by CodeBreaker). Run: build\\%s.exe game
call "%s\\VC\\Auxiliary\\Build\\vcvars64.bat" >nul 2>&1 || exit /b 1
set "LLVM=%s\\VC\\Tools\\Llvm\\x64\\bin"
cd /d "%%~dp0"
if not exist build\\CMakeCache.txt cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release "-DCMAKE_C_COMPILER=%%LLVM%%\\clang-cl.exe" "-DCMAKE_CXX_COMPILER=%%LLVM%%\\clang-cl.exe" || exit /b 1
cmake --build build || exit /b 1
''' % (name, name, vs, vs))

    with open(os.path.join(out_dir, "Play-%s.bat" % name), "w", newline="\r\n") as f:
        f.write('@echo off\nrem Plays %s (written by CodeBreaker): keyboard or controller; the log is game\\%s.log\n'
                'start "" "%%~dp0build\\%s.exe" "%%~dp0game"\n' % (name, name, name))

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
