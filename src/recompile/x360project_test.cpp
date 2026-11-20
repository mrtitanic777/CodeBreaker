#include "recompile/x360project.hpp"

#include <chrono>
#include <map>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"

using namespace cb;
using namespace cb::recompile::x360project;
namespace stdfs = std::filesystem;

namespace {

bool has(std::string_view haystack, std::string_view needle) { return haystack.find(needle) != std::string_view::npos; }

// XenonRecomp's tool chain stood in for in-process: the "analyser" writes its second argument, the "recompiler" writes the
// configuration's ppc.new/ folder (two functions, one code file) and says two instructions are unimplemented. What it does
// is plain data a test changes.
struct Fake {
    fs::ScopedTempDir dir{"cbx360"};
    Recompiler rc;
    int analysed = 0;                                   // XenonAnalyse runs
    std::map<std::string, std::string> files = {        // what XenonRecomp writes into ppc.new/
        {"ppc_func_mapping.cpp", "{ 0x82000000, a },\r\n{ 0x82000010, b },\r\n"},
        {"ppc_recomp.0.cpp", "// code\r\n"}};
    std::string printed = "unimplemented bctr\r\nunimplemented sc\r\n";
    int exit_code = 0;
    std::string refuse;                                 // a configuration naming this file is refused (exit 4)

    Fake() {
        rc.analyse = [this](const std::vector<std::string>& args) {
            ++analysed;
            fs::write_text(fs::from_text(args.at(1)), "# analysed \r\n");
            return cb::recompile::xenon_engine::Run{0, ""};
        };
        rc.recomp = [this](const std::vector<std::string>& args) {
            const stdfs::path cfg = fs::from_text(args.at(0));
            if (!refuse.empty() && has(fs::read_text(cfg), refuse)) return cb::recompile::xenon_engine::Run{4, "cannot translate\r\n"};
            if (exit_code != 0) return cb::recompile::xenon_engine::Run{exit_code, printed};
            for (const auto& [name, text] : files) fs::write_text(cfg.parent_path() / "ppc.new" / name, text);
            return cb::recompile::xenon_engine::Run{0, printed};
        };
        rc.source = dir.path() / "XenonRecomp";
        rc.visual_studio = "C:/VS/Community";
        rc.runtime_cmake = "C:/cb/src/recompile/x360/x360.cmake";
        rc.write_basic = [](const stdfs::path& xex, const stdfs::path& out) { fs::write_file(out, fs::read_file(xex)); };
        rc.write_switch_tables = [](const stdfs::path& file, const stdfs::path&, const std::optional<std::string>& analysed) {
            fs::write_text(file, analysed ? "[analysed]\n" : "[none]\n");
            return Recompiler::SwitchTables{5, analysed ? 2u : 0u};
        };
        rc.make_config = [](const stdfs::path&, std::string_view out_dir, std::string_view tables, std::string_view file_path) {
            return "out = \"" + std::string(out_dir) + "\"\ntables = \"" + std::string(tables) + "\"\nfile = \"" + std::string(file_path) + "\"\n";
        };
        rc.savegprlr_14_address = [](const stdfs::path&) { return 0x82ABCDEFu; };
    }
    stdfs::path xex() {
        const stdfs::path p = dir.path() / "Pac-Man C.E.xex";
        fs::write_text(p, "XEX2 fake");
        return p;
    }
};

}  // namespace

CB_TEST("recompile.x360project.names", [] {
    CB_CHECK_EQ(default_name("C:/games/Pac-Man C.E.xex"), "pac_man_c_e");
    CB_CHECK_EQ(default_name("default.xex"), "default");
    CB_CHECK_EQ(default_name(""), "title");
    CB_CHECK_EQ(target_name("Pac-Man C.E."), "Pac_Man_C_E");
    CB_CHECK_EQ(target_name("pac_man_c_e"), "pac_man_c_e");
    CB_CHECK_EQ(target_name("--a--b--"), "a_b");
    CB_CHECK_EQ(target_name("default"), "title");
    CB_CHECK_EQ(target_name(""), "title");
    CB_CHECK_EQ(target_name("!!!"), "title");
});

CB_TEST("recompile.x360project.sync_rewrites_only_what_changed", [] {
    fs::ScopedTempDir dir{"cbsync"};
    const stdfs::path src = dir.path() / "src", dst = dir.path() / "dst";
    fs::write_text(src / "same.cpp", "same");
    fs::write_text(src / "changed.cpp", "new");
    fs::write_text(src / "added.cpp", "added");
    fs::write_text(dst / "same.cpp", "same");
    fs::write_text(dst / "changed.cpp", "old");
    fs::write_text(dst / "stale.cpp", "stale");
    const auto old_time = stdfs::file_time_type::clock::now() - std::chrono::hours(48);
    cb::fs::set_last_write_time(dst / "same.cpp", old_time);
    sync_folder(src, dst);
    CB_CHECK_EQ(fs::read_text(dst / "changed.cpp"), "new");
    CB_CHECK_EQ(fs::read_text(dst / "added.cpp"), "added");
    CB_CHECK(!stdfs::exists(dst / "stale.cpp"));
    CB_CHECK(cb::fs::last_write_time(dst / "same.cpp") == old_time);   // not rewritten: a rebuild does not recompile it
    // into a folder that is not there yet
    sync_folder(src, dir.path() / "made");
    CB_CHECK(stdfs::exists(dir.path() / "made" / "same.cpp"));
});

CB_TEST("recompile.x360project.scaffold_writes_the_project", [] {
    Fake fake;
    const stdfs::path title = fake.dir.path() / "title files";
    fs::write_text(title / "media" / "a.bin", "a");
    fs::write_text(title / "readme.txt", "hello");
    fs::write_text(title / "DEFAULT.XEX", "must not replace the basic one");
    const stdfs::path out = fake.dir.path() / "project";
    const ScaffoldInfo info = scaffold(fake.rc, fake.xex(), out, {}, title);
    CB_CHECK_EQ(info.name, "pac_man_c_e");
    CB_CHECK(info.dir == out);
    CB_CHECK_EQ(info.functions, 2u);
    CB_CHECK_EQ(info.switches, 7u);          // found and added
    CB_CHECK_EQ(info.unimplemented, 2u);
    CB_CHECK_EQ(fake.analysed, 1);         // XenonAnalyse once, for the title

    const stdfs::path src = out / "src";
    CB_CHECK_EQ(fs::read_text(src / "default.xex"), "XEX2 fake");
    CB_CHECK_EQ(fs::read_text(src / "switch_tables.toml"), "[analysed]\n");   // the analyser's output was handed over
    CB_CHECK_EQ(fs::read_text(src / "pac_man_c_e.toml"), "out = \"ppc.new\"\ntables = \"switch_tables.toml\"\nfile = \"default.xex\"\n");
    CB_CHECK(stdfs::exists(src / "ppc" / "ppc_func_mapping.cpp"));
    CB_CHECK(stdfs::exists(src / "ppc" / "ppc_recomp.0.cpp"));
    CB_CHECK(!stdfs::exists(src / "ppc.new"));
    CB_CHECK(has(fs::read_text(out / "reports" / "recompile.log"), "unimplemented bctr"));

    const std::string info_cpp = fs::read_text(src / "module_info.cpp");
    CB_CHECK(has(info_cpp, "    0x82ABCDEF, // __savegprlr_14\n};"));
    CB_CHECK(has(info_cpp, "extern const modules::RecompiledModule g_titleModule = {\n    \"default.xex\","));
    CB_CHECK_EQ(fs::read_text(out / "CMakeLists.txt"),
                "# Written by CodeBreaker (cb/recompile/x360project.cpp).\ncmake_minimum_required(VERSION 3.20)\nproject(pac_man_c_e CXX)\n"
                "include(\"C:/cb/src/recompile/x360/x360.cmake\")\n"
                "x360_title(pac_man_c_e \"${CMAKE_CURRENT_SOURCE_DIR}/src/ppc\" \"${CMAKE_CURRENT_SOURCE_DIR}/src/module_info.cpp\")\n");
    const std::string build_bat = fs::read_text(out / "Build-pac_man_c_e.bat");
    CB_CHECK(has(build_bat, "rem Builds pac_man_c_e with clang-cl and Ninja (written by CodeBreaker). Run: build\\pac_man_c_e.exe assets\r\n"));
    CB_CHECK(has(build_bat, "call \"C:\\VS\\Community\\VC\\Auxiliary\\Build\\vcvars64.bat\" >nul 2>&1 || exit /b 1\r\n"));
    CB_CHECK(has(build_bat, "set \"LLVM=C:\\VS\\Community\\VC\\Tools\\Llvm\\x64\\bin\"\r\n"));
    CB_CHECK(has(build_bat, "if defined CB_BUILD_JOBS (cmake --build build -j %CB_BUILD_JOBS% || exit /b 1)"));
    CB_CHECK(has(fs::read_text(out / "Play-pac_man_c_e.bat"), "start \"\" \"%~dp0build\\pac_man_c_e.exe\" \"%~dp0assets\"\r\n"));

    // the title folder: its files (a folder too), the program in basic form, never the one the title came with
    CB_CHECK_EQ(fs::read_text(out / "assets" / "media" / "a.bin"), "a");
    CB_CHECK_EQ(fs::read_text(out / "assets" / "readme.txt"), "hello");
    CB_CHECK_EQ(fs::read_text(out / "assets" / "default.xex"), "XEX2 fake");
});

CB_TEST("recompile.x360project.the_other_modules_of_a_title_are_translated_and_listed", [] {
    Fake fake;
    const stdfs::path title = fake.dir.path() / "title files";
    fs::write_text(title / "bin" / "Tier0_360.dll", "XEX2 program");
    fs::write_text(title / "bin" / "data.xex", "XEX2 resources");        // a module with no program in it
    fs::write_text(title / "bin" / "notes.dll", "not an xex at all");    // not an XEX
    fs::write_text(title / "DEFAULT.XEX", "must not replace the basic one");
    fake.rc.has_native_code = [](const stdfs::path& basic) { return fs::read_text(basic) == "XEX2 program"; };
    // the title's own configuration has no suffix; a module's names its symbols
    fake.rc.make_config = [](const stdfs::path&, std::string_view out_dir, std::string_view tables, std::string_view file_path) {
        return "[main]\nout = \"" + std::string(out_dir) + "\"\ntables = \"" + std::string(tables) + "\"\nfile = \"" + std::string(file_path) + "\"\n";
    };
    const stdfs::path out = fake.dir.path() / "project";
    const ScaffoldInfo info = scaffold(fake.rc, fake.xex(), out, {}, title);
    CB_CHECK_EQ(info.modules, 1u);
    CB_CHECK_EQ(info.functions, 2u);   // the title's own

    const stdfs::path module = out / "src" / "modules" / "bin_tier0_360_dll";
    CB_CHECK(stdfs::exists(module / "ppc" / "ppc_func_mapping.cpp"));
    CB_CHECK(!stdfs::exists(out / "src" / "modules" / "bin_data_xex"));
    CB_CHECK(!stdfs::exists(out / "src" / "modules" / "bin_notes_dll"));
    const std::string config = fs::read_text(module / "bin_tier0_360_dll.toml");
    CB_CHECK(has(config, "[main]\nsymbol_suffix = \"_bin_tier0_360_dll\"\nsuffix_function_names = true\n"));
    CB_CHECK(has(config, "file = \"image.xex\""));
    CB_CHECK(!has(fs::read_text(out / "src" / "pac_man_c_e.toml"), "symbol_suffix"));

    const std::string module_cpp = fs::read_text(module / "module_info.cpp");
    CB_CHECK(has(module_cpp, "extern PPCFuncMapping PPCFuncMappings_bin_tier0_360_dll[];"));
    CB_CHECK(has(module_cpp, "extern const modules::RecompiledModule g_module_bin_tier0_360_dll = {\n    \"Tier0_360.dll\","));
    CB_CHECK(has(module_cpp, "    PPCFuncMappings_bin_tier0_360_dll,\n    0x82ABCDEF, // __savegprlr_14\n    \"bin/Tier0_360.dll\",\n    nullptr,\n};"));
    const std::string title_cpp = fs::read_text(out / "src" / "module_info.cpp");
    CB_CHECK(has(title_cpp, "extern const modules::RecompiledModule g_module_bin_tier0_360_dll;\n"));
    CB_CHECK(has(title_cpp, "kExtraModules[] = { &g_module_bin_tier0_360_dll, nullptr };"));
    CB_CHECK(has(title_cpp, "    0x82ABCDEF, // __savegprlr_14\n    nullptr,\n    kExtraModules,\n};"));

    // the program the runtime loads is the module in basic form; the others are as they were
    CB_CHECK_EQ(fs::read_text(out / "assets" / "bin" / "Tier0_360.dll"), "XEX2 program");
    CB_CHECK_EQ(fs::read_text(out / "assets" / "bin" / "data.xex"), "XEX2 resources");
    CB_CHECK(has(fs::read_text(out / "reports" / "modules.txt"), "module bin/Tier0_360.dll: 2 functions, 7 switch tables\n"));
    CB_CHECK(has(fs::read_text(out / "reports" / "modules.txt"), "module bin/data.xex: not a PowerPC program"));
});

CB_TEST("recompile.x360project.a_module_that_fails_to_translate_is_left_out_not_fatal", [] {
    Fake fake;
    const stdfs::path title = fake.dir.path() / "title files";
    fs::write_text(title / "engine.dll", "XEX2 program");
    fake.rc.has_native_code = [](const stdfs::path&) { return true; };
    // the recompiler works for the title (a config naming default.xex) and fails for the module (image.xex)
    fake.refuse = "image.xex";
    const ScaffoldInfo info = scaffold(fake.rc, fake.xex(), fake.dir.path() / "project", {}, title);
    CB_CHECK_EQ(info.modules, 0u);
    CB_CHECK_EQ(info.functions, 2u);
    CB_CHECK(!stdfs::exists(fake.dir.path() / "project" / "src" / "modules" / "engine_dll"));
    CB_CHECK(has(fs::read_text(fake.dir.path() / "project" / "reports" / "modules.txt"), "module engine.dll: not translated"));
});

CB_TEST("recompile.x360project.a_second_scaffold_rewrites_only_the_changed_code", [] {
    Fake fake;
    const stdfs::path out = fake.dir.path() / "project";
    (void)scaffold(fake.rc, fake.xex(), out, "My Title");
    const stdfs::path kept = out / "src" / "ppc" / "ppc_recomp.0.cpp";
    const auto old_time = stdfs::file_time_type::clock::now() - std::chrono::hours(24);
    cb::fs::set_last_write_time(kept, old_time);
    // the recompiler now writes an extra file and gives the same one as before
    fake.files["ppc_recomp.1.cpp"] = "// more\r\n";
    fake.printed.clear();
    const ScaffoldInfo again = scaffold(fake.rc, fake.xex(), out, "My Title");
    CB_CHECK_EQ(again.name, "My_Title");
    CB_CHECK_EQ(again.unimplemented, 0u);
    CB_CHECK(cb::fs::last_write_time(kept) == old_time);
    CB_CHECK(stdfs::exists(out / "src" / "ppc" / "ppc_recomp.1.cpp"));
});

CB_TEST("recompile.x360project.default_is_not_a_target_name", [] {
    Fake fake;
    fs::write_text(fake.dir.path() / "default.xex", "XEX2");
    CB_CHECK_EQ(scaffold(fake.rc, fake.dir.path() / "default.xex", fake.dir.path() / "p", {}).name, "title");
});

CB_TEST("recompile.x360project.a_failed_recompile_is_a_tool_error_with_its_log", [] {
    Fake fake;
    fake.printed = "cannot read config\r\n";
    fake.exit_code = 3;
    try {
        (void)scaffold(fake.rc, fake.xex(), fake.dir.path() / "project");
        CB_CHECK(false);
    } catch (const ToolError& e) {
        CB_CHECK_EQ(e.tool_exit_code(), 3u);
        CB_CHECK(has(e.output(), "cannot read config"));
    }
    CB_CHECK(has(fs::read_text(fake.dir.path() / "project" / "reports" / "recompile.log"), "cannot read config"));
    // exit 0 but nothing written is a failure too
    fake.exit_code = 0;
    fake.files.clear();
    CB_CHECK_THROWS_AS(ToolError, (void)scaffold(fake.rc, fake.xex(), fake.dir.path() / "project"));
});

CB_TEST("recompile.x360project.build_reports_errors_first", [] {
    CB_WINDOWS_ONLY("its stand-in tools are .cmd/.bat files and Windows programs");
    fs::ScopedTempDir dir{"cbbuild"};
    fs::write_text(dir.path() / "Build-t.bat", "@echo off\r\necho jobs=%CB_BUILD_JOBS%\r\necho compiling\r\necho a.cpp(3): error C2065: x\r\nexit /b 1\r\n");
    const BuildResult bad = build(dir.path(), "t", 3);
    CB_CHECK(!bad.ok);
    CB_CHECK((bad.tail == std::vector<std::string>{"a.cpp(3): error C2065: x"}));
    CB_CHECK(has(fs::read_text(dir.path() / "reports" / "build.log"), "jobs=3\ncompiling\na.cpp(3): error C2065: x"));
    std::string many = "@echo off\r\n";
    for (int i = 0; i < 20; ++i) many += "echo line " + std::to_string(i) + "\r\n";
    fs::write_text(dir.path() / "Build-u.bat", many + "exit /b 0\r\n");
    const BuildResult good = build(dir.path(), "u");
    CB_CHECK(good.ok);
    CB_CHECK_EQ(good.tail.size(), 12u);        // no errors: the last twelve lines
    CB_CHECK_EQ(good.tail.back(), "line 19");
});

CB_TEST("recompile.x360project.the_log_says_how_far_the_title_got", [] {
    const CheckResult r = read_log("start\nGPU: frame 3\nUNIMPLEMENTED IMPORT XamFoo\nGPU: frame 120\nUNIMPLEMENTED IMPORT XamBar\n"
                                   "UNIMPLEMENTED IMPORT XamFoo\nGPU: frame 7\n");
    CB_CHECK_EQ(r.frames, 120u);
    CB_CHECK_EQ(r.unimplemented.size(), 2u);
    CB_CHECK_EQ(r.unimplemented.begin()->first, "XamFoo");
    CB_CHECK_EQ(*r.unimplemented.find("XamFoo"), 2u);
    CB_CHECK_EQ(*r.unimplemented.find("XamBar"), 1u);
    CB_CHECK(r.fatal.empty());
    // a fatal message wins over a stall
    const CheckResult fatal = read_log("no frame presented for 20 s after frame 9\r\nFATAL: bad thing happened\r\nmore\r\n");
    CB_CHECK_EQ(fatal.fatal, "bad thing happened");
    const CheckResult stall = read_log("GPU: frame 9\nno frame presented for 20 s after frame 9\n");
    CB_CHECK_EQ(stall.fatal, "no frame presented for 20 s after frame 9");
    const CheckResult empty = read_log("");
    CB_CHECK(empty.frames == 0 && empty.unimplemented.empty() && empty.fatal.empty());
    CB_CHECK(read_log("no frame presented for x s after frame 9").fatal.empty());   // not that report
});

CB_TEST("recompile.x360project.check_runs_the_title_and_reads_its_log", [] {
    CB_WINDOWS_ONLY("its stand-in tools are .cmd/.bat files and Windows programs");
    fs::ScopedTempDir dir{"cbcheck"};
    fs::make_dirs(dir.path() / "assets");
    fs::make_dirs(dir.path() / "build");
    // a stale log is not mistaken for this run's
    fs::write_text(dir.path() / "assets" / "t.log", "GPU: frame 999\n");
    // the "title": a program that exits at once, writing nothing
    stdfs::copy_file(stdfs::path(fs::get_env("SystemRoot").value_or("C:\\Windows")) / "System32" / "whoami.exe", dir.path() / "build" / "t.exe");
    const CheckResult r = check(dir.path(), "t", 30);
    CB_CHECK_EQ(r.frames, 0u);
    CB_CHECK(r.exit.has_value());
    CB_CHECK(r.log == dir.path() / "assets" / "t.log");
    // a title that is not there cannot be started
    CB_CHECK_THROWS_AS(ToolError, (void)check(dir.path(), "missing", 5));
});
