// Tests of cb/recompile/xenon. The module runs programs (cmake, XenonRecomp, clang-cl, the PowerPC binutils, vswhere),
// so the tests hand it an in-process stand-in through Config::runner: it plays those programs, logs what it was asked
// to run and in what environment, and writes the files they would write. What is checked is what the module does with
// them: the arguments and environment it passes, how it reads their output, and what it leaves on disk. (One test runs
// the real cmd.exe on a real batch file, the only program of the module that is always there.)

#include "cb/recompile/xenon.hpp"

#include <chrono>
#include <fstream>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

using namespace cb;
using namespace cb::recompile::xenon;
using cb::process::ProcessResult;
using cb::process::ProcessSpec;
namespace stdfs = std::filesystem;

namespace {

bool has(std::string_view text, std::string_view needle) { return text.find(needle) != std::string_view::npos; }

std::string lower_stem(const std::string& program) {
    std::string stem = stdfs::path(program).stem().string();
    return text::lower(stem);
}

ProcessResult result(int code, std::string out = {}, std::string err = {}) {
    ProcessResult r;
    r.exit_code = static_cast<std::uint32_t>(code);
    r.out = to_bytes(out);
    r.err = to_bytes(err);
    return r;
}

std::string text_of(const std::string& path) { return fs::read_text(fs::from_text(path)); }

// The programs, in process. Each answers by the name of the program it is asked to run.
struct FakeTools {
    std::vector<std::string> log;      // one line per run: the program's name, its arguments joined by '|', what matters of the environment
    std::string cmake_fail;            // "configure" or "build": cmake fails at that step
    std::string vswhere_answer;        // what vswhere prints ('|' is a new line)
    bool cc_fail = false;              // clang-cl fails

    std::string joined() const { return text::join(log, "\n"); }
    void clear() { log.clear(); }

    ProcessResult operator()(const ProcessSpec& spec) {
        const std::string& program = spec.argv.at(0);
        const std::string name = lower_stem(program);
        const std::vector<std::string> a(spec.argv.begin() + 1, spec.argv.end());
        const std::string fakevar = spec.env.contains("FAKEVAR") ? spec.env.at("FAKEVAR") : "-";
        const std::string cwd_name = spec.cwd ? spec.cwd->filename().string() : "-";

        if (name == "cmd") return run_cmd(a);
        if (name == "vswhere") {
            log.push_back("vswhere " + text::join(a, "|"));
            return result(0, text::replace_all(vswhere_answer, "|", "\n"));
        }
        if (name == "cmake") {
            log.push_back("cmake " + text::join(a, "|") + " FAKEVAR=" + fakevar);
            const bool configure = !a.empty() && a[0] == "-S";
            const bool building = !a.empty() && a[0] == "--build";
            if ((configure && cmake_fail == "configure") || (building && cmake_fail == "build")) {
                return result(1, "CMake Error: " + cmake_fail + " failed " + std::string(3200, 'x') + "\n", "stderr of the failing cmake\n");
            }
            if (configure) {
                for (std::size_t i = 0; i + 1 < a.size(); ++i) {
                    if (a[i] == "-B") fs::write_text(fs::from_text(a[i + 1]) / "CMakeCache.txt", "cache\n");
                }
                return result(0, "-- Configuring done\n");
            }
            if (building) {
                // "building" makes the programs new (the world put stand-in files there)
                const stdfs::path b = fs::from_text(a.at(1));
                for (const stdfs::path& p : {b / "XenonRecomp" / "XenonRecomp.exe", b / "XenonAnalyse" / "XenonAnalyse.exe"}) {
                    if (stdfs::exists(p)) stdfs::last_write_time(p, stdfs::file_time_type::clock::now());
                }
                return result(0, "[1/1] Linking\n");
            }
            return result(0);
        }
        if (name == "xenonrecomp") return run_xenonrecomp(a, spec, cwd_name);
        if (name == "clang-cl") {
            log.push_back("clang-cl " + text::join(a, "|") + " FAKEVAR=" + fakevar + " CWD=" + cwd_name);
            if (cc_fail) return result(2, "cc stdout\n", "cc stderr: error\n");
            int sources = 0;
            for (const std::string& s : a) {
                if (s.empty() || s[0] == '/' || s[0] == '-') continue;
                if (!stdfs::exists(s)) return result(3, "", "missing source " + s + "\n");
                ++sources;
            }
            return result(0, "compiled " + std::to_string(sources) + " sources\n");
        }
        if (name == "ppc_tests") {
            const stdfs::path m = *spec.cwd / "mismatch.txt";
            return result(0, stdfs::exists(m) ? fs::read_text(m) : std::string(), "ppc_tests stderr\n");
        }
        if (name == "powerpc-none-elf-as") {
            log.push_back("powerpc-none-elf-as " + text::join(a, "|"));
            const std::string text = text_of(a.back());
            std::string o;
            for (const std::string& s : a) {
                if (s.rfind("-o", 0) == 0) o = s.substr(2);
            }
            if (has(text, "BAD")) return result(1, "", "  Error: bad instruction in " + stdfs::path(a.back()).filename().string() + "  \n");
            fs::write_text(fs::from_text(o), text);
            return result(0);
        }
        if (name == "powerpc-none-elf-objdump") {
            return result(0, "\nfile format elf32-powerpc\n\nDisassembly of section .text:\n" + text_of(a.back()));
        }
        return result(99, "", "fake: unknown program " + program + "\n");
    }

private:
    // cmd /d /c <the batch file the module wrote>: what it holds is `call "<vcvars64.bat>" >nul && set`
    ProcessResult run_cmd(const std::vector<std::string>& a) {
        log.push_back("cmd " + text::join(a, "|"));
        const std::string script = text_of(a.back());
        const auto first = script.find("call \"");
        if (first == std::string::npos) return result(9, "", "no call in the script\n");
        const auto close = script.find('"', first + 6);
        const std::string bat = text_of(script.substr(first + 6, close - first - 6));
        if (has(bat, "exit /b 1")) return result(1);
        std::string out = "Path=C:\\Windows\r\n=C:=C:\\hidden\r\n";
        for (const std::string_view line : text::lines(bat)) {
            if (line.starts_with("set ")) out += std::string(line.substr(4)) + "\r\n";
        }
        return result(0, out);
    }

    static std::map<std::string, std::string> parse_toml(const std::string& toml) {
        std::map<std::string, std::string> m;
        for (const std::string_view raw : text::lines(toml)) {
            const auto eq = raw.find('=');
            if (eq == std::string_view::npos) continue;
            std::string k(text::trim(raw.substr(0, eq))), v(text::trim(raw.substr(eq + 1)));
            if (v.size() >= 2 && v.front() == '"' && v.back() == '"') v = v.substr(1, v.size() - 2);
            m[k] = v;
        }
        return m;
    }

    ProcessResult run_xenonrecomp(const std::vector<std::string>& a, const ProcessSpec& spec, const std::string& cwd_name) {
        log.push_back("xenonrecomp " + text::join(a, "|") + " CWD=" + cwd_name);
        if (!a.empty() && a[0].ends_with(".toml")) {
            stdfs::path cfg = fs::from_text(a[0]);
            if (cfg.is_relative()) cfg = *spec.cwd / cfg;
            auto m = parse_toml(fs::read_text(cfg));
            const stdfs::path out = cfg.parent_path() / fs::from_text(m["out_directory_path"]);
            fs::make_dirs(out);
            std::string o = "Recompiling functions 1/3\nnote: cwd=" + cwd_name + " arg=" + a[0] + " arg2=" +
                            (a.size() > 1 ? stdfs::path(a[1]).filename().string() : std::string("-")) + "\nRecompiling functions 3/3\n";
            if (!m["fake_files"].empty()) {
                for (const std::string_view f : text::split(m["fake_files"], ',')) {
                    fs::write_text(out / std::string(f), "// fake " + std::string(f) + " tag=" + m["fake_tag"] + "\n");
                }
            }
            const int n = m.contains("fake_warn") ? std::stoi(m["fake_warn"]) : 0;
            for (int i = 1; i <= n; ++i) o += "warning: line " + std::to_string(i) + " of the recompile\n";
            return result(m.contains("fake_exit") ? std::stoi(m["fake_exit"]) : 0, o, m["fake_stderr"].empty() ? "" : m["fake_stderr"] + "\n");
        }
        // XenonRecomp <objs> <out>: the .dis files recompiled
        const stdfs::path objs = fs::from_text(a.at(0)), out = fs::from_text(a.at(1));
        std::vector<stdfs::path> dis;
        for (const auto& e : stdfs::directory_iterator(objs)) {
            if (e.path().extension() == ".dis") dis.push_back(e.path());
        }
        std::sort(dis.begin(), dis.end());
        std::string o = "dirmode " + std::to_string(dis.size()) + " files\n";
        for (const auto& d : dis) {
            const std::string text = fs::read_text(d), stem = d.stem().string();
            if (has(text, "NOIN")) o += "warning: has unimplemented thing\n";
            else if (has(text, "UNIMPL")) o += "warning: instruction 'foo' has unimplemented encoding in " + stem + " at 0x100000\n";
            else fs::write_text(out / (stem + ".cpp"), "// " + stem + "\n");
        }
        return result(0, o + "other line without the marker\n");
    }
};

// A scratch world: a checkout with a XenonRecomp source tree (and stand-in files where its programs would be), a
// Visual Studio with a clang-cl and a vcvars64.bat, a folder of binutils, and the fake programs to run.
struct World {
    fs::ScopedTempDir dir{"cb-xenon"};
    stdfs::path root = dir.path() / "r";
    stdfs::path state = dir.path() / "state";
    stdfs::path vs = dir.path() / "vs 1";   // (a space in the path: the quoting is part of what is tested)
    stdfs::path binutils = dir.path() / "bu";
    FakeTools tools;

    World() {
        fs::write_text(vs / "VC" / "Tools" / "Llvm" / "x64" / "bin" / "clang-cl.exe", "");
        fs::write_text(vs / "VC" / "Auxiliary" / "Build" / "vcvars64.bat", "@echo off\r\nset FAKEVAR=fromvcvars\r\nset FAKEEQ=a=b=c\r\n");
        fs::write_text(binutils / "powerpc-none-elf-as.exe", "");
        fs::write_text(binutils / "powerpc-none-elf-objdump.exe", "");
        const stdfs::path src = source();
        fs::write_text(src / "XenonRecomp" / "a.cpp", "a");
        fs::write_text(src / "XenonRecomp" / "sub" / "b.cpp", "b");
        fs::write_text(src / "XenonRecomp" / ".hidden" / "c.cpp", "c");
        fs::write_text(src / "XenonUtils" / "ppc_context.h", "h");
        fs::write_text(src / "XenonAnalyse" / "x.cpp", "x");
        fs::write_text(src / "CMakeLists.txt", "cm");
        // the programs of an old build: older than the sources
        for (const char* name : {"XenonRecomp", "XenonAnalyse"}) {
            const stdfs::path exe = src / "build" / name / (std::string(name) + ".exe");
            fs::write_text(exe, "");
            stdfs::last_write_time(exe, stdfs::file_time_type::clock::now() - std::chrono::hours(2));
        }
    }

    stdfs::path source() const { return root / "native" / "xenonrecomp"; }

    Config config() {
        Config c;
        c.checkout_root = root;
        c.state_dir = state;
        c.visual_studio = vs;
        c.ppc_binutils = binutils;
        c.build_jobs = 3;
        c.runner = [this](const ProcessSpec& spec) { return tools(spec); };
        return c;
    }
};

// make `p` newer than everything else in the world
void touch_newer(const stdfs::path& p) { stdfs::last_write_time(p, stdfs::file_time_type::clock::now() + std::chrono::hours(1)); }

// a configuration `work/<name>` for the fake XenonRecomp (the lines after the first three say what it should do)
stdfs::path write_config(const World& w, std::string_view extra, std::string_view name = "cfg.toml") {
    const stdfs::path cfg = w.dir.path() / "work" / name;
    fs::write_text(cfg, "[main]\nfile_path = \"x.xex\"\nout_directory_path = \"out/ppc\"\n" + std::string(extra));
    return cfg;
}

}  // namespace

CB_TEST("recompile.xenon.locations_come_from_the_checkout", [] {
    const Xenon x(Config{.checkout_root = "C:/co", .state_dir = "C:/co/state"});
    CB_CHECK_EQ(fs::to_text(x.source_dir()), "C:/co/native/xenonrecomp");
    CB_CHECK_EQ(fs::to_text(x.build_dir()), "C:/co/native/xenonrecomp/build");
    CB_CHECK_EQ(fs::to_text(x.recomp_exe()), "C:/co/native/xenonrecomp/build/XenonRecomp/XenonRecomp.exe");
    CB_CHECK_EQ(fs::to_text(x.analyse_exe()), "C:/co/native/xenonrecomp/build/XenonAnalyse/XenonAnalyse.exe");
    CB_CHECK_EQ(fs::to_text(x.tests_dir()), "C:/co/native/xenonrecomp/ppc_tests");
    CB_CHECK_EQ(fs::to_text(x.context_header()), "C:/co/native/xenonrecomp/XenonUtils/ppc_context.h");
    CB_CHECK_EQ(skipped_instruction_tests().size(), 2u);
    CB_CHECK_EQ(skipped_instruction_tests()[0], "ppc_testing_native_thunks");
    CB_CHECK_EQ(skipped_instruction_tests()[1], "seq_jumptable_constants");
});

CB_TEST("recompile.xenon.visual_studio_by_setting_by_vswhere_or_not_at_all", [] {
    World w;
    // the setting wins
    CB_CHECK(Xenon(w.config()).visual_studio() == w.vs);
    // else the newest vswhere finds: its first line
    Config c = w.config();
    c.visual_studio.reset();
    const stdfs::path pf = w.dir.path() / "pf";
    fs::write_text(pf / "Microsoft Visual Studio" / "Installer" / "vswhere.exe", "");
    const fs::ScopedEnv pf_env("ProgramFiles(x86)", fs::to_text(pf));
    w.tools.vswhere_answer = fs::to_text(w.vs) + "|" + fs::to_text(w.dir.path() / "vs2");
    CB_CHECK(Xenon(c).visual_studio() == w.vs);
    CB_CHECK(has(w.tools.joined(), "vswhere -latest|-prerelease|-products|*|-requires|Microsoft.VisualStudio.Component.VC.Tools.x86.x64|-property|installationPath"));
    // vswhere finds none
    w.tools.vswhere_answer = "";
    CB_CHECK_THROWS_AS(ToolError, (void)Xenon(c).visual_studio());
    // there is no vswhere
    const fs::ScopedEnv elsewhere("ProgramFiles(x86)", fs::to_text(w.dir.path() / "nowhere"));
    CB_CHECK_THROWS_AS(ToolError, (void)Xenon(c).visual_studio());
});

CB_TEST("recompile.xenon.clang_cl_of_the_install", [] {
    World w;
    CB_CHECK(Xenon(w.config()).clang_cl() == w.vs / "VC" / "Tools" / "Llvm" / "x64" / "bin" / "clang-cl.exe");
    stdfs::remove(w.vs / "VC" / "Tools" / "Llvm" / "x64" / "bin" / "clang-cl.exe");
    CB_CHECK_THROWS_AS(ToolError, (void)Xenon(w.config()).clang_cl());
});

CB_TEST("recompile.xenon.msvc_env_is_what_vcvars_sets_and_is_read_once", [] {
    World w;
    Xenon x(w.config());
    const Environment& env = x.msvc_env();
    CB_CHECK_EQ(env.at("FAKEVAR"), "fromvcvars");
    CB_CHECK_EQ(env.at("FAKEEQ"), "a=b=c");      // a value with '=' in it is kept whole
    CB_CHECK_EQ(env.at("Path"), "C:\\Windows");
    CB_CHECK(!env.contains(""));                 // (the hidden per-drive entries start with '=')
    // the batch file the module wrote puts the quoted path in a call and asks for the environment as UTF-8
    const std::string script = fs::read_text(w.state / "xenonrecomp" / "msvc_env.cmd");
    CB_CHECK(has(script, "chcp 65001 >nul"));
    CB_CHECK(has(script, "call \"" + (w.vs / "VC" / "Auxiliary" / "Build" / "vcvars64.bat").string() + "\" >nul && set"));
    // read once: the batch file may go and the answer is the same object, with no second run
    stdfs::remove(w.vs / "VC" / "Auxiliary" / "Build" / "vcvars64.bat");
    w.tools.clear();
    CB_CHECK(&x.msvc_env() == &env);
    CB_CHECK(w.tools.log.empty());
    // a vcvars that fails gives no environment: an error, not an empty environment
    World bad;
    fs::write_text(bad.vs / "VC" / "Auxiliary" / "Build" / "vcvars64.bat", "@echo off\r\nexit /b 1\r\n");
    Xenon y(bad.config());
    CB_CHECK_THROWS_AS(ToolError, (void)y.msvc_env());
});

CB_TEST("recompile.xenon.msvc_env_through_the_real_cmd", [] {
    // no stand-in: the real cmd.exe runs the real batch file (in a folder with a space) and its `set` is parsed
    World w;
    fs::write_text(w.vs / "VC" / "Auxiliary" / "Build" / "vcvars64.bat", "@echo off\r\nset CB_XENON_TEST=from the batch file\r\nset CB_XENON_EQ=a=b\r\n");
    Config c = w.config();
    c.runner = nullptr;
    Xenon x(c);
    const Environment& env = x.msvc_env();
    CB_CHECK_EQ(env.at("CB_XENON_TEST"), "from the batch file");
    CB_CHECK_EQ(env.at("CB_XENON_EQ"), "a=b");
    CB_CHECK(env.contains("PATH") || env.contains("Path"));    // the rest of the environment comes through
    CB_CHECK(env.contains("SystemRoot") || env.contains("SYSTEMROOT") || env.contains("windir"));
});

CB_TEST("recompile.xenon.build_configures_and_builds_once_per_change", [] {
    World w;
    Xenon x(w.config());
    CB_CHECK(x.build() == x.recomp_exe());
    std::string log = w.tools.joined();
    const std::string src = x.source_dir().string(), bld = x.build_dir().string();
    CB_CHECK(has(log, "cmake -S|" + src + "|-B|" + bld + "|-G|Ninja|-DCMAKE_BUILD_TYPE=Release|-DCMAKE_C_COMPILER="));
    CB_CHECK(has(log, "FAKEVAR=fromvcvars"));        // run in the vcvars environment
    CB_CHECK(has(log, "cmake --build|" + bld + "|-j|3"));

    // up to date: nothing runs
    w.tools.clear();
    x.build();
    CB_CHECK(w.tools.log.empty());

    // a source newer than the programs: built again, without configuring again
    touch_newer(x.source_dir() / "XenonRecomp" / "sub" / "b.cpp");
    x.build();
    log = w.tools.joined();
    CB_CHECK(has(log, "cmake --build|"));
    CB_CHECK(!has(log, "cmake -S|"));
    stdfs::last_write_time(x.source_dir() / "XenonRecomp" / "sub" / "b.cpp", stdfs::file_time_type::clock::now() - std::chrono::hours(1));

    // a hidden file is not a source
    w.tools.clear();
    touch_newer(x.source_dir() / "XenonRecomp" / ".hidden" / "c.cpp");
    x.build();
    CB_CHECK(w.tools.log.empty());
});

CB_TEST("recompile.xenon.build_runs_when_the_programs_are_missing", [] {
    World w;
    Xenon x(w.config());
    stdfs::remove(x.recomp_exe());
    x.build();
    CB_CHECK(has(w.tools.joined(), "cmake -S|"));       // configured and built: there was nothing to be up to date
    CB_CHECK(has(w.tools.joined(), "cmake --build|"));
});

CB_TEST("recompile.xenon.build_forced_reconfigures_and_failures_are_tool_errors", [] {
    World w;
    Xenon x(w.config());
    x.build();
    w.tools.clear();
    x.build(true);
    CB_CHECK(has(w.tools.joined(), "cmake -S|"));
    w.tools.cmake_fail = "configure";
    try {
        x.build(true);
        CB_CHECK(false);
    } catch (const ToolError& e) {
        CB_CHECK_EQ(e.tool(), "cmake");
        CB_CHECK_EQ(e.tool_exit_code(), 1u);
        CB_CHECK(has(e.what(), "configuring XenonRecomp failed"));
        CB_CHECK(e.output().size() <= 3000);                  // the end of a long output
        CB_CHECK(has(e.output(), "stderr of the failing cmake"));
    }
    w.tools.cmake_fail = "build";
    touch_newer(x.source_dir() / "CMakeLists.txt");
    try {
        x.build();
        CB_CHECK(false);
    } catch (const ToolError& e) {
        CB_CHECK(has(e.what(), "building XenonRecomp failed"));
    }
});

CB_TEST("recompile.xenon.recompile_runs_in_the_configurations_folder", [] {
    World w;
    Xenon x(w.config());
    const stdfs::path cfg = write_config(w, "fake_warn = 2\nfake_files = a.cpp\n");
    const std::string out = x.recompile(cfg);
    CB_CHECK(has(out, "Recompiling functions 1/3"));                // its output, whole
    CB_CHECK(has(out, "note: cwd=work "));                          // run in the configuration's folder
    CB_CHECK(has(out, "arg2=ppc_context.h"));                       // given the context header
    CB_CHECK(has(out, "warning: line 2 of the recompile"));
    CB_CHECK(stdfs::exists(w.dir.path() / "work" / "out" / "ppc" / "a.cpp"));
    // a failure carries the end of its output
    const stdfs::path bad = write_config(w, "fake_exit = 3\nfake_stderr = it broke\n", "bad.toml");
    try {
        (void)x.recompile(bad);
        CB_CHECK(false);
    } catch (const ToolError& e) {
        CB_CHECK_EQ(e.tool(), "XenonRecomp");
        CB_CHECK_EQ(e.tool_exit_code(), 3u);
        CB_CHECK(has(e.output(), "it broke"));
    }
});

CB_TEST("recompile.xenon.recompile_synced_changes_only_what_changed", [] {
    World w;
    Xenon x(w.config());
    const stdfs::path out = w.dir.path() / "work" / "out" / "ppc";
    const stdfs::path cfg = write_config(w, "fake_tag = 1\nfake_files = a.cpp,b.cpp\nfake_warn = 2\n");
    std::vector<std::string> shown;
    const auto report = [&](std::string_view line) { shown.emplace_back(line); };

    // the first run writes both files; the temporary configuration and folder are gone afterwards
    SyncResult r = x.recompile_synced(cfg, report);
    CB_CHECK_EQ(r.generated, 2u);
    CB_CHECK_EQ(r.changed, 2u);
    CB_CHECK(r.stale.empty());
    CB_CHECK_EQ(r.log.size(), 3u);                                  // its note and the two warnings, not the progress lines
    CB_CHECK(has(r.log[0], "note: cwd=work "));
    CB_CHECK(has(r.log[0], "arg=_recompile_tmp.toml"));             // run on the temporary copy, by its name
    CB_CHECK(!stdfs::exists(w.dir.path() / "work" / "_recompile_tmp.toml"));
    CB_CHECK_EQ(fs::read_text(out / "a.cpp"), "// fake a.cpp tag=1\n");
    CB_CHECK_EQ(shown.size(), 4u);                                  // the three lines and the summary
    CB_CHECK_EQ(shown.back(), "2 files generated, 2 changed, 0 stale removed; 3 warning lines");
    std::size_t entries = 0;
    for (const auto& e : stdfs::directory_iterator(out.parent_path())) {
        (void)e;
        ++entries;
    }
    CB_CHECK_EQ(entries, 1u);                                       // only ppc/: the scratch folder is gone

    // the same output again: nothing changed, and the files keep their times
    stdfs::last_write_time(out / "a.cpp", stdfs::file_time_type::clock::now() - std::chrono::hours(5));
    const auto before = stdfs::last_write_time(out / "a.cpp");
    r = x.recompile_synced(cfg);
    CB_CHECK_EQ(r.generated, 2u);
    CB_CHECK_EQ(r.changed, 0u);
    CB_CHECK(stdfs::last_write_time(out / "a.cpp") == before);

    // every file's content changes
    const stdfs::path cfg2 = write_config(w, "fake_tag = 2\nfake_files = a.cpp,b.cpp\n");
    r = x.recompile_synced(cfg2);
    CB_CHECK_EQ(r.changed, 2u);
    CB_CHECK_EQ(fs::read_text(out / "b.cpp"), "// fake b.cpp tag=2\n");

    // a recompile that makes fewer files removes the ones it no longer makes
    fs::write_text(out / "zz_old.cpp", "x");
    const stdfs::path cfg3 = write_config(w, "fake_tag = 2\nfake_files = a.cpp\n");
    shown.clear();
    r = x.recompile_synced(cfg3, report);
    CB_CHECK_EQ(r.generated, 1u);
    CB_CHECK_EQ(r.stale.size(), 2u);
    CB_CHECK_EQ(r.stale[0], "b.cpp");
    CB_CHECK_EQ(r.stale[1], "zz_old.cpp");                          // sorted
    CB_CHECK(!stdfs::exists(out / "b.cpp") && !stdfs::exists(out / "zz_old.cpp"));
    CB_CHECK_EQ(shown.back(), "1 files generated, 0 changed, 2 stale removed: b.cpp zz_old.cpp; 1 warning lines");
});

CB_TEST("recompile.xenon.recompile_synced_failures", [] {
    World w;
    Xenon x(w.config());
    const stdfs::path out = w.dir.path() / "work" / "out" / "ppc";
    // XenonRecomp fails: a ToolError with the end of its log and its stderr; the output folder is untouched
    {
        const stdfs::path cfg = write_config(w, "fake_files = a.cpp\nfake_exit = 7\nfake_stderr = boom\n");
        try {
            (void)x.recompile_synced(cfg);
            CB_CHECK(false);
        } catch (const ToolError& e) {
            CB_CHECK_EQ(e.tool_exit_code(), 7u);
            CB_CHECK(has(e.output(), "boom"));
        }
        CB_CHECK(!stdfs::exists(out / "a.cpp"));
        CB_CHECK(!stdfs::exists(w.dir.path() / "work" / "_recompile_tmp.toml"));
    }
    // a configuration without out_directory_path
    {
        const stdfs::path cfg = w.dir.path() / "work" / "none.toml";
        fs::write_text(cfg, "[main]\nfile_path = \"x\"\n");
        CB_CHECK_THROWS_AS(ParseError, (void)x.recompile_synced(cfg));
    }
    // a configuration that is not there
    CB_CHECK_THROWS_AS(IoError, (void)x.recompile_synced(w.dir.path() / "work" / "missing.toml"));
});

CB_TEST("recompile.xenon.out_directory_path_of_a_configuration", [] {
    CB_CHECK_EQ(out_directory_path("out_directory_path = \"ppc\"\n").value_or("-"), "ppc");
    CB_CHECK_EQ(out_directory_path("[main]\nfile_path = \"a\"\nout_directory_path=\"x/y z\"\r\n").value_or("-"), "x/y z");
    CB_CHECK_EQ(out_directory_path("out_directory_path\t=\t\"t\"").value_or("-"), "t");
    CB_CHECK_EQ(out_directory_path("out_directory_path = \"first\"\nout_directory_path = \"second\"").value_or("-"), "first");
    CB_CHECK(!out_directory_path("").has_value());
    CB_CHECK(!out_directory_path("  out_directory_path = \"indented\"").has_value());   // only at the start of a line
    CB_CHECK(!out_directory_path("# out_directory_path = \"comment\"").has_value());
    CB_CHECK(!out_directory_path("out_directory_path = ppc").has_value());              // not quoted
    CB_CHECK(!out_directory_path("out_directory_path = \"open").has_value());
});

CB_TEST("recompile.xenon.instruction_tests_assemble_recompile_build_and_run", [] {
    World w;
    Xenon x(w.config());
    const stdfs::path tests = x.tests_dir();
    fs::write_text(tests / "basic" / "add.s", "ADD\n");
    fs::write_text(tests / "basic" / "sub.s", "SUB\n");
    fs::write_text(tests / "basic" / "ppc_testing_native_thunks.s", "THUNKS\n");     // never run
    fs::write_text(tests / "vmx" / "v1.s", "UNIMPL\n");                              // XenonRecomp does not implement it
    fs::write_text(tests / "vmx" / "notes.txt", "not a test");
    const stdfs::path work = w.dir.path() / "work 2";
    fs::write_text(work / "mismatch.txt", "add rA: 1 != 2\nsub mismatch\n\n   \nadd again\n");

    InstructionTestOptions opt;
    opt.work = work;
    opt.march = "skylake";
    const InstructionTestResult r = x.instruction_tests(opt);
    CB_CHECK_EQ(r.tested, 2u);                                                       // add and sub; v1 is skipped, the thunk test never ran
    CB_CHECK(r.skipped == std::vector<std::string>{"v1"});
    CB_CHECK(r.mismatches == (std::vector<std::string>{"add rA: 1 != 2", "sub mismatch", "add again"}));   // blank lines dropped
    CB_CHECK(r.failed == (std::vector<std::string>{"add", "sub"}));                  // sorted, once each

    // what was assembled and recompiled: the listing without objdump's four header lines
    CB_CHECK_EQ(fs::read_text(work / "src" / "bin" / "add.dis"), "ADD\n");
    CB_CHECK(!stdfs::exists(work / "src" / "bin" / "v1.dis"));                       // the skipped one is taken out and recompiled without it
    CB_CHECK(stdfs::exists(work / "out" / "add.cpp") && !stdfs::exists(work / "out" / "v1.cpp"));
    const std::string log = w.tools.joined();
    CB_CHECK(has(log, "powerpc-none-elf-as -a32|-be|-mregnames|-mpower7|-maltivec|-mvsx|-mvmx128|-R|-o"));
    CB_CHECK(has(log, "clang-cl /nologo|/std:c++20|/O1|/fp:strict|-fno-strict-aliasing|-march=skylake|-Wno-everything|/DFMT_HEADER_ONLY|/I"));
    CB_CHECK(has(log, "FAKEVAR=fromvcvars CWD=work 2"));                             // built in the vcvars environment, in the work folder
    CB_CHECK(has(log, "|/Fo" + work.string() + "\\|"));                              // the objects go to the work folder

    // only the named tests
    InstructionTestOptions only;
    only.work = work;
    only.names = {"add", "nonexistent"};
    const InstructionTestResult one = x.instruction_tests(only);
    CB_CHECK_EQ(one.tested, 1u);
    CB_CHECK(one.skipped.empty());
});

CB_TEST("recompile.xenon.instruction_tests_report_why_they_cannot_run", [] {
    World w;
    Xenon x(w.config());
    const stdfs::path work = w.dir.path() / "work";
    InstructionTestOptions opt;
    opt.work = work;
    // no binutils configured, or none there
    {
        Config c = w.config();
        c.ppc_binutils.reset();
        CB_CHECK_THROWS_AS(ToolError, (void)Xenon(c).instruction_tests(opt));
        c.ppc_binutils = w.dir.path() / "none";
        CB_CHECK_THROWS_AS(ToolError, (void)Xenon(c).instruction_tests(opt));
    }
    // the assembler rejects a test
    fs::write_text(x.tests_dir() / "bad" / "bad1.s", "BAD\n");
    try {
        (void)x.instruction_tests(opt);
        CB_CHECK(false);
    } catch (const ToolError& e) {
        CB_CHECK(has(e.what(), "assembling bad1 failed: Error: bad instruction in bad1.s"));
    }
    stdfs::remove_all(x.tests_dir() / "bad");
    // the build of the tests fails
    fs::write_text(x.tests_dir() / "ok" / "ok1.s", "OK\n");
    w.tools.cc_fail = true;
    try {
        (void)x.instruction_tests(opt);
        CB_CHECK(false);
    } catch (const ToolError& e) {
        CB_CHECK_EQ(e.tool_exit_code(), 2u);
        CB_CHECK(has(e.what(), "building the instruction tests failed"));
        CB_CHECK(has(e.output(), "cc stderr: error"));
    }
    w.tools.cc_fail = false;
    // no mismatch file, no options: the default work folder is under the state folder
    const InstructionTestResult plain = x.instruction_tests();
    CB_CHECK_EQ(plain.tested, 1u);
    CB_CHECK(plain.mismatches.empty() && plain.failed.empty());     // (stderr of the test program is not a result)
    CB_CHECK(stdfs::exists(w.state / "xenonrecomp" / "ppc_tests" / "out" / "ok1.cpp"));
});

CB_TEST("recompile.xenon.the_command_takes_one_configuration", [] {
    World w;
    Xenon x(w.config());
    const stdfs::path cfg = write_config(w, "fake_files = a.cpp\n");
    CB_CHECK_THROWS_AS(UsageError, xenon_recompile_command(x, std::span<const std::string>()));
    const std::vector<std::string> two = {"a", "b"};
    CB_CHECK_THROWS_AS(UsageError, xenon_recompile_command(x, two));
    testing::CapturedStd captured;
    const std::vector<std::string> one = {fs::to_text(cfg)};
    xenon_recompile_command(x, one);
    const std::string printed = captured.out();
    CB_CHECK(has(printed, "note: cwd=work "));
    CB_CHECK(has(printed, "1 files generated, 1 changed, 0 stale removed; 1 warning lines"));
});
