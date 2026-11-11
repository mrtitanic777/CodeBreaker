// codebreaker/recompile/x360project.cpp -- typed C++ from codebreaker.recompile.x360project (see x360project.hpp).
#include "codebreaker/recompile/x360project.hpp"

#include <algorithm>
#include <cstdio>
#include <filesystem>
#include <iostream>
#include <variant>

#include "codebreaker/formats/xex.hpp"
#include "codebreaker/recompile/config.hpp"
#include "codebreaker/recompile/discover.hpp"
#include "codebreaker/recompile/xenon.hpp"
#include "py_os.hpp"
#include "py_subprocess.hpp"

namespace codebreaker::recompile::x360project {

namespace {

// ROOT as the import derives it: three folders above this module. The C++ source sits six folders below the
// checkout (native/typed/cpp/codebreaker/recompile/x360project.cpp).
py::str default_root_() {
    py::str here = py::os::path::abspath(py::str(__FILE__));
    for (int i = 0; i < 6; ++i) here = py::os::path::dirname(here);
    return here;
}

// shutil.copy2(src, dst): the file and its modification time
void copy2_(const py::str& src, const py::str& dst) {
    py::shutil::copyfile(src, dst);
    std::error_code ec;
    auto t = std::filesystem::last_write_time(py::fspath_(src), ec);
    if (!ec) std::filesystem::last_write_time(py::fspath_(dst), t, ec);
}

// shutil.copytree(src, dst, dirs_exist_ok=True)
void copytree_(const py::str& src, const py::str& dst) {
    py::os::makedirs(dst, 0777, true);
    for (const py::str& n : py::os::listdir(src)) {
        py::str a = py::os::path::join(src, n);
        py::str b = py::os::path::join(dst, n);
        if (py::os::path::isdir(a)) copytree_(a, b);
        else copy2_(a, b);
    }
}

// filecmp.cmp(a, b, shallow=False)
bool same_file_(const py::str& a, const py::str& b) {
    if (py::os::path::getsize(a) != py::os::path::getsize(b)) return false;
    py::file fa = py::open(a, py::str("rb"));
    py::bytes da = fa.read();
    fa.close();
    py::file fb = py::open(b, py::str("rb"));
    py::bytes db = fb.read();
    fb.close();
    return da == db;
}

int64_t count_(const py::str& s, const char* sub) { return s.count(py::str(sub)); }

void write_text_(const py::str& path, const py::str& text, const char* newline) {
    py::file f = py::open(path, py::str("w"), std::nullopt, std::nullopt, py::str(newline));
    f.write(text);
    f.close();
}

py::str read_text_(const py::str& path) {
    py::file f = py::open(path);
    py::str t = f.read_text();
    f.close();
    return t;
}

py::str hex8_(int64_t v) {
    char b[16];
    std::snprintf(b, sizeof b, "%08X", static_cast<unsigned>(v & 0xFFFFFFFFLL));
    return py::str(b);
}

}  // namespace

py::str ROOT;

void set_root(const py::str& root) { ROOT = root; }

namespace {
const bool root_set_ = (set_root(default_root_()), true);
}

py::str _name(const py::str& xex_path) {
    py::str stem = std::get<0>(py::os::path::splitext(py::os::path::basename(xex_path))).lower();
    std::u32string out;
    for (char32_t c : stem.u32()) out.push_back(py::str(std::u32string(1, c)).isalnum() ? c : U'_');
    py::str r{out};
    return r.empty() ? py::str("title") : r;
}

void _sync(const py::str& src, const py::str& dst) {
    py::os::makedirs(dst, 0777, true);
    py::list<py::str> have = py::os::listdir(src);
    for (const py::str& name : have) {
        py::str a = py::os::path::join(src, name);
        py::str b = py::os::path::join(dst, name);
        if (!(py::os::path::isfile(b) && same_file_(a, b))) copy2_(a, b);
    }
    for (const py::str& name : py::os::listdir(dst)) {
        if (!have.contains(name)) py::os::remove(py::os::path::join(dst, name));
    }
}

ScaffoldInfo scaffold(const py::str& xex_path, const py::str& out_dir0, const std::optional<py::str>& name0,
                      const std::optional<py::str>& title_files) {
    // the program's, target's and scripts' name: letters, digits and underscores ("Pac-Man C.E." -> Pac_Man_C_E)
    py::str wanted = (name0 && !name0->empty()) ? *name0 : _name(xex_path);
    py::list<py::str> parts;
    for (const py::str& p : py::re::split("[^A-Za-z0-9]+", wanted))
        if (!p.empty()) parts.append(p);
    py::str name = py::join(py::str("_"), parts);
    if (name.empty()) name = py::str("title");
    py::str out_dir = py::os::path::abspath(out_dir0);   // XenonRecomp runs inside it and is given paths into it
    if (name == py::str("default")) name = py::str("title");
    py::str recomp = py::os::path::join(out_dir, py::str("src"));
    py::str game = py::os::path::join(out_dir, py::str("assets"));
    py::str reports = py::os::path::join(out_dir, py::str("reports"));
    for (const py::str& d : {recomp, game, reports}) py::os::makedirs(d, 0777, true);
    py::str basic = py::os::path::join(recomp, py::str("default.xex"));
    formats::xex::basic(xex_path, basic);

    // switch tables: switches.py's, with XenonAnalyse's for any bctr it leaves unresolved
    xenon::build();
    py::str analysed = py::os::path::join(recomp, py::str("switch_tables.analysed.toml"));
    try {                       // optional: switches.py finds nearly all of them by itself
        py::subprocess::RunOptions ro;
        ro.text = false;
        py::subprocess::run(py::list<py::str>{xenon::ANALYSE, basic, analysed}, ro);
    } catch (const py::OSError& e) {
        std::cout << "  (XenonAnalyse did not run: " << e.what() << "; switch tables from switches.py alone)" << std::endl;
    }
    std::optional<py::str> with_analysed;
    if (py::os::path::isfile(analysed)) with_analysed = read_text_(analysed);
    auto [found, added] = config::write_switch_tables(py::os::path::join(recomp, py::str("switch_tables.toml")), basic, with_analysed);

    // the configuration, then XenonRecomp into a fresh folder, synced into recomp/ppc
    py::str cfg = py::os::path::join(recomp, name + py::str(".toml"));
    write_text_(cfg, config::make(basic, py::str("ppc.new"), py::str("switch_tables.toml"), std::nullopt, py::str("default.xex")), "\n");
    py::str fresh = py::os::path::join(recomp, py::str("ppc.new"));
    py::shutil::rmtree(fresh, true);
    py::os::makedirs(fresh);
    xenon::build();
    py::subprocess::RunOptions ro;
    ro.cwd = recomp;
    py::subprocess::CompletedProcess r = py::subprocess::run(
        py::list<py::str>{xenon::RECOMP, cfg, py::os::path::join(xenon::SOURCE, py::str("XenonUtils"), py::str("ppc_context.h"))}, ro);
    py::str log = (r.stdout_ ? *r.stdout_ : py::str("")) + (r.stderr_ ? *r.stderr_ : py::str(""));
    write_text_(py::os::path::join(reports, py::str("recompile.log")), log, "\r\n");
    if (r.returncode != 0 || !py::os::path::isfile(py::os::path::join(fresh, py::str("ppc_func_mapping.cpp")))) {
        throw py::RuntimeError(py::str("XenonRecomp failed (") + py::to_str(r.returncode) + py::str("):\n") + log.slice_from(-3000));
    }
    _sync(fresh, py::os::path::join(recomp, py::str("ppc")));
    py::shutil::rmtree(fresh, true);

    py::dict<py::str, int64_t> helpers = config::helpers(*discover::_image(basic));
    write_text_(py::os::path::join(recomp, py::str("module_info.cpp")),
                py::str("// The recompiled title described to the runtime (modules.h), built with its own generated ppc_config.h.\n"
                        "// Written by CodeBreaker (codebreaker/recompile/x360project.py).\n"
                        "#include \"ppc_config.h\"\n"
                        "#include \"ppc_context.h\"\n"
                        "\n"
                        "#include \"modules.h\"\n"
                        "\n"
                        "extern PPCFuncMapping PPCFuncMappings[];\n"
                        "\n"
                        "extern const modules::RecompiledModule g_titleModule = {\n"
                        "    \"default.xex\",\n"
                        "    uint32_t(PPC_IMAGE_BASE),\n"
                        "    uint32_t(PPC_IMAGE_SIZE),\n"
                        "    uint32_t(PPC_CODE_BASE),\n"
                        "    uint32_t(PPC_CODE_SIZE),\n"
                        "    PPCFuncMappings,\n"
                        "    0x") + hex8_(helpers.get(py::str("savegprlr_14_address"), 0)) + py::str(", // __savegprlr_14\n};\n"),
                "\n");
    py::str runtime = py::os::path::join(ROOT, py::str("native"), py::str("x360"), py::str("x360.cmake")).replace(py::str("\\"), py::str("/"));
    write_text_(py::os::path::join(out_dir, py::str("CMakeLists.txt")),
                py::str("# Written by CodeBreaker (codebreaker/recompile/x360project.py).\n"
                        "cmake_minimum_required(VERSION 3.20)\n"
                        "project(") + name + py::str(" CXX)\ninclude(\"") + runtime + py::str("\")\nx360_title(") + name +
                    py::str(" \"${CMAKE_CURRENT_SOURCE_DIR}/src/ppc\" \"${CMAKE_CURRENT_SOURCE_DIR}/src/module_info.cpp\")\n"),
                "\n");
    py::str vs = xenon::visual_studio();
    write_text_(py::os::path::join(out_dir, py::str("Build-") + name + py::str(".bat")),
                py::str("@echo off\n"
                        "rem Builds ") + name + py::str(" with clang-cl and Ninja (written by CodeBreaker). Run: build\\") + name +
                    py::str(".exe assets\n"
                            "call \"") + vs + py::str("\\VC\\Auxiliary\\Build\\vcvars64.bat\" >nul 2>&1 || exit /b 1\n"
                                                      "set \"LLVM=") + vs + py::str("\\VC\\Tools\\Llvm\\x64\\bin\"\n"
                                                                                    "cd /d \"%~dp0\"\n"
                                                                                    "if not exist build\\CMakeCache.txt cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release "
                                                                                    "\"-DCMAKE_C_COMPILER=%LLVM%\\clang-cl.exe\" \"-DCMAKE_CXX_COMPILER=%LLVM%\\clang-cl.exe\" || exit /b 1\n"
                                                                                    "if defined CB_BUILD_JOBS (cmake --build build -j %CB_BUILD_JOBS% || exit /b 1) else (cmake --build build || exit /b 1)\n"),
                "\r\n");

    write_text_(py::os::path::join(out_dir, py::str("Play-") + name + py::str(".bat")),
                py::str("@echo off\nrem Plays ") + name + py::str(" (written by CodeBreaker): keyboard or controller; the log is assets\\") + name +
                    py::str(".log\nstart \"\" \"%~dp0build\\") + name + py::str(".exe\" \"%~dp0assets\"\n"),
                "\r\n");

    // the title folder: its files, and default.xex in basic form
    if (title_files && !title_files->empty()) {
        for (const py::str& entry : py::os::listdir(*title_files)) {
            py::str src = py::os::path::join(*title_files, entry);
            py::str dst = py::os::path::join(game, entry);
            if (entry.lower() == py::str("default.xex")) continue;
            if (py::os::path::isdir(src)) copytree_(src, dst);
            else copy2_(src, dst);
        }
    }
    copy2_(basic, py::os::path::join(game, py::str("default.xex")));

    py::str mapping = read_text_(py::os::path::join(recomp, py::str("ppc"), py::str("ppc_func_mapping.cpp")));
    ScaffoldInfo info;
    info.name = name;
    info.dir = out_dir;
    info.functions = count_(mapping, "{ 0x");
    info.switches = found + added;
    info.unimplemented = count_(log, "unimplemented");
    return info;
}

std::tuple<bool, py::list<py::str>> build(const py::str& out_dir, const py::str& name, int64_t jobs) {
    py::dict<py::str, py::str> env = py::os::environ_::copy();
    env.set(py::str("CB_BUILD_JOBS"), py::to_str(jobs));
    py::subprocess::RunOptions ro;
    ro.cwd = out_dir;
    ro.env = env;
    ro.errors = "replace";
    py::subprocess::CompletedProcess r = py::subprocess::run(
        py::list<py::str>{py::str("cmd"), py::str("/c"), py::os::path::join(out_dir, py::str("Build-") + name + py::str(".bat"))}, ro);
    py::list<py::str> out = py::splitlines((r.stdout_ ? *r.stdout_ : py::str("")) + (r.stderr_ ? *r.stderr_ : py::str("")));
    {
        py::file f = py::open(py::os::path::join(out_dir, py::str("reports"), py::str("build.log")), py::str("w"), py::str("utf-8"));
        f.write(py::join(py::str("\n"), out));
        f.close();
    }
    py::list<py::str> errors;
    for (const py::str& line : out)
        if (line.contains(py::str(" error")) || line.contains(py::str("error:"))) errors.append(line);
    py::list<py::str> shown = errors.empty() ? out : errors;
    return std::make_tuple(r.returncode == 0, shown.slice_from(-12));
}

CheckResult check(const py::str& out_dir, const py::str& name, int64_t seconds) {
    py::str game = py::os::path::join(out_dir, py::str("assets"));
    py::str exe = py::os::path::join(out_dir, py::str("build"), name + py::str(".exe"));
    py::str log = py::os::path::join(game, name + py::str(".log"));
    if (py::os::path::exists(log)) py::os::remove(log);
    py::dict<py::str, py::str> env = py::os::environ_::copy();
    env.set(py::str("CB_HEADLESS"), py::str("1"));
    env.set(py::str("CB_WATCHDOG"), py::str("20"));
    env.set(py::str("CB_CONTINUE_UNIMPLEMENTED"), py::str("1"));
    py::subprocess::RunOptions ro;
    ro.cwd = game;
    ro.env = env;
    ro.stdout_ = py::subprocess::Stream::Devnull;
    ro.stderr_ = py::subprocess::Stream::Devnull;
    ro.timeout = seconds;
    int64_t exit_code = 0;
    try {
        exit_code = py::subprocess::run(py::list<py::str>{exe, game}, ro).returncode;
    } catch (const py::subprocess::TimeoutExpired&) {
        exit_code = 1;   // (the program is ended: Process.kill() leaves exit code 1)
    }
    py::str text;
    if (py::os::path::exists(log)) {
        py::file f = py::open(log, py::str("r"), py::str("utf-8"), py::str("replace"));
        text = f.read_text();
        f.close();
    }
    std::optional<int64_t> frames;
    for (const py::str& m : py::re::findall("GPU: frame (\\d+)", text)) {
        int64_t v = py::int_parse(m, 10);
        if (!frames || v > *frames) frames = v;
    }
    py::dict<py::str, int64_t> missing;
    for (const py::str& m : py::re::findall("UNIMPLEMENTED IMPORT (\\w+)", text)) missing.set(m, missing.get(m, 0) + 1);
    py::list<py::str> fatal = py::re::findall("FATAL: (.*)", text);
    py::list<py::str> stall = py::re::findall("no frame presented for \\d+ s after frame \\d+", text);
    CheckResult r;
    r.frames = frames ? *frames : 0;
    r.unimplemented = missing;
    r.fatal = !fatal.empty() ? fatal[0] : (!stall.empty() ? stall[0] : py::str(""));
    r.exit = exit_code;
    r.log = log;
    return r;
}

}  // namespace codebreaker::recompile::x360project
