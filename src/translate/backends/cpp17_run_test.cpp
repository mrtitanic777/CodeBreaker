// The Python front end's semantics snippet, as C++17 that runs: the checked-in text the back end wrote for it (testdata/cpp17/
// semantics.golden.{hpp,cpp}) is built with the host's MSVC against the runtime in config/translate/runtime/cpp17, every run_* function is
// called, and each answer is compared with what CPython gave for the same function (semantics.expected.txt, made by running the
// snippet under CPython: one "name<TAB>json" line per function; an exception is {"error": its class}). Without `cl` on PATH (a build run from a plain
// shell) the test says so and passes.
//
// cb-unity: alone
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "core/fs.hpp"
#include "core/json.hpp"
#include "core/process.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"

namespace {

bool host_compiler_present() {
    try {
        cb::process::ProcessSpec spec;
        spec.argv = {"cl", "/nologo", "/?"};
        spec.timeout = std::chrono::seconds(60);
        return cb::process::run(spec).exit_code == 0;
    } catch (const std::exception&) {
        return false;
    }
}

std::vector<std::pair<std::string, cb::Json>> expected_answers(const std::filesystem::path& file) {
    std::vector<std::pair<std::string, cb::Json>> out;
    const std::string text = cb::fs::read_text(file);
    std::size_t at = 0;
    while (at < text.size()) {
        std::size_t end = text.find('\n', at);
        if (end == std::string::npos) end = text.size();
        const std::string line = text.substr(at, end - at);
        at = end + 1;
        const std::size_t tab = line.find('\t');
        if (tab == std::string::npos) continue;
        out.emplace_back(line.substr(0, tab), cb::json::parse(line.substr(tab + 1)));
    }
    return out;
}

}  // namespace

CB_TEST("translate.cpp17.the_python_semantics_snippet_runs_as_cpython_runs_it", [] {
    if (!host_compiler_present()) {
        std::cerr << "cpp17 run test: no cl on PATH; skipped\n";
        return;
    }
    const std::filesystem::path data = cb::testing::data_path("translate/testdata/cpp17");
    const auto expected = expected_answers(data / "semantics.expected.txt");
    CB_CHECK_EQ(expected.size(), 15u);

    cb::fs::ScopedTempDir work("cb_cpp17_run");
    const std::filesystem::path dir = work.path();   // NOLINT
    cb::fs::write_text(dir / "snip.hpp", cb::fs::read_text(data / "semantics.golden.hpp"));
    std::string main_text = cb::fs::read_text(data / "semantics.golden.cpp");
    main_text += "\n#include \"py_json.hpp\"\n#include <cstdio>\nusing namespace snip;\nint main() {\n";
    for (const auto& [name, answer] : expected) {
        main_text += "    std::printf(\"%s\\n\", py::json::attempt([&](std::string& v) { py::json::put(v, " + name + "()); }).c_str());\n";
    }
    main_text += "    return 0;\n}\n";
    cb::fs::write_text(dir / "snip.cpp", main_text);

    cb::process::ProcessSpec build;
    build.argv = {"cl", "/nologo", "/std:c++17", "/EHsc", "/utf-8", "/permissive-", "/I" + cb::testing::data_path("../config/translate/runtime/cpp17").string(),
                  "/I" + dir.string(), "/Fe" + (dir / "snip.exe").string(), "/Fo" + dir.string() + "\\", "snip.cpp"};
    build.cwd = dir;
    build.err = cb::process::Capture::Merge;
    build.timeout = std::chrono::minutes(10);
    const cb::process::ProcessResult built = cb::process::run(build);
    CB_CHECK(built.exit_code == 0);
    if (built.exit_code != 0) {
        std::cerr << std::string(reinterpret_cast<const char*>(built.out.data()), std::min<std::size_t>(built.out.size(), 3000)) << "\n";
        return;
    }

    cb::process::ProcessSpec run;
    run.argv = {(dir / "snip.exe").string()};
    run.timeout = std::chrono::minutes(2);
    const cb::process::ProcessResult ran = cb::process::run(run);
    CB_CHECK(ran.exit_code == 0);
    const std::string out(reinterpret_cast<const char*>(ran.out.data()), ran.out.size());   // (UTF-8 bytes, kept as they are)
    std::vector<std::string> lines;
    std::size_t at = 0;
    while (at < out.size()) {
        std::size_t end = out.find('\n', at);
        if (end == std::string::npos) end = out.size();
        std::string line = out.substr(at, end - at);
        at = end + 1;
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (!line.empty()) lines.push_back(std::move(line));
    }
    CB_CHECK_EQ(lines.size(), expected.size());
    for (std::size_t i = 0; i < lines.size() && i < expected.size(); ++i) {
        cb::Json got;
        CB_CHECK(cb::json::try_parse(lines[i], got));
        if (got != expected[i].second)
            cb::testing::record_failure(__FILE__, __LINE__, expected[i].first + ": CPython gave " + expected[i].second.dump() + ", the C++ gave " + lines[i]);
    }
});

// The Java runtime's cycle collector (JRT_CYCLES in jrt.hpp), built with g++ as the Java programs are: rings of objects, directly and
// through a list, a map and an array, are freed; a ring a local or a static holds stays; an object a method is running on stays.
CB_TEST("translate.cpp17.the_cycle_collector_frees_rings_and_nothing_in_use", [] {
    try {
        cb::process::ProcessSpec probe;
        probe.argv = {"g++", "--version"};
        probe.timeout = std::chrono::seconds(30);
        if (cb::process::run(probe).exit_code != 0) return;
    } catch (const std::exception&) {
        std::cerr << "cycle collector test: no g++ on PATH; skipped\n";
        return;
    }
    cb::fs::ScopedTempDir work("cb_jrt_gc");
    const std::string exe = cb::fs::to_text(work.path() / "gc_check.exe");
    cb::process::ProcessSpec build;
    build.argv = {"g++", "-std=c++17", "-fwrapv", "-w", "-O1", "-I" + cb::fs::to_text(cb::testing::data_path("../config/translate/runtime/cpp17")), "-o", exe,
                  cb::fs::to_text(cb::testing::data_path("translate/testdata/cpp17/gc_check.cpp"))};
    build.timeout = std::chrono::seconds(300);
    CB_CHECK(cb::process::run(build).exit_code == 0);
    cb::process::ProcessSpec run;
    run.argv = {exe};
    run.timeout = std::chrono::seconds(120);
    const auto ran = cb::process::run(run);
    CB_CHECK(ran.exit_code == 0);
});

// Each Java program's translation, built with g++ and run: what it prints is what the JVM printed for the same program (<name>.jvm.txt,
// recorded from Java 21). A golden alone only says the text is unchanged; these say the text works.
namespace {
void prints_what_the_jvm_printed(const std::string& name) {
    try {
        cb::process::ProcessSpec probe;
        probe.argv = {"g++", "--version"};
        probe.timeout = std::chrono::seconds(30);
        if (cb::process::run(probe).exit_code != 0) return;
    } catch (const std::exception&) {
        return;   // (no g++ here)
    }
    const std::filesystem::path data = cb::testing::data_path("translate/testdata/cpp17");
    cb::fs::ScopedTempDir work("cb_cpp17_jvm");
    const std::string exe = cb::fs::to_text(work.path() / (name + ".exe"));
    cb::process::ProcessSpec build;
    build.argv = {"g++", "-std=c++17", "-fwrapv", "-w", "-O0", "-I" + cb::fs::to_text(cb::testing::data_path("../config/translate/runtime/cpp17")), "-x", "c++", "-o", exe,
                  cb::fs::to_text(data / (name + ".golden.cpp"))};
    build.timeout = std::chrono::seconds(300);
    const auto built = cb::process::run(build);
    CB_CHECK(built.exit_code == 0);
    if (built.exit_code != 0) return;
    cb::process::ProcessSpec run;
    run.argv = {exe};
    run.timeout = std::chrono::seconds(60);
    const auto ran = cb::process::run(run);
    CB_CHECK(ran.exit_code == 0);
    const std::string got = cb::text::replace_all(std::string(ran.out.begin(), ran.out.end()), "\r", "");
    CB_CHECK_EQ(got, cb::text::replace_all(cb::fs::read_text(data / (name + ".jvm.txt")), "\r", ""));
}
}  // namespace

// The runtime is not GCC's alone: with MSVC, the program that uses the most of its library prints what the JVM printed, and the collector holds.
CB_TEST("translate.cpp17.the_java_runtime_builds_and_runs_with_msvc", [] {
    if (!host_compiler_present()) {
        std::cerr << "cpp17 MSVC test: no cl on PATH; skipped\n";
        return;
    }
    const std::filesystem::path data = cb::testing::data_path("translate/testdata/cpp17");
    cb::fs::ScopedTempDir work("cb_cpp17_msvc");
    const auto build_and_run = [&](const std::string& name, const std::filesystem::path& source) {
        const std::string exe = cb::fs::to_text(work.path() / (name + ".exe"));
        cb::process::ProcessSpec build;
        build.argv = {"cl", "/nologo", "/std:c++17", "/EHsc", "/utf-8", "/permissive-", "/bigobj", "/w", "/Od",
                      "/I" + cb::fs::to_text(cb::testing::data_path("../config/translate/runtime/cpp17")), "/Fe" + exe,
                      "/Fo" + cb::fs::to_text(work.path() / (name + ".obj")), "/TP", cb::fs::to_text(source)};
        build.cwd = work.path();
        build.err = cb::process::Capture::Merge;
        build.timeout = std::chrono::minutes(5);
        const auto built = cb::process::run(build);
        CB_CHECK(built.exit_code == 0);
        if (built.exit_code != 0) {
            std::cerr << std::string(built.out.begin(), built.out.begin() + static_cast<std::ptrdiff_t>(std::min<std::size_t>(built.out.size(), 3000))) << "\n";
            return std::string();
        }
        cb::process::ProcessSpec run;
        run.argv = {exe};
        run.timeout = std::chrono::seconds(120);
        const auto ran = cb::process::run(run);
        CB_CHECK(ran.exit_code == 0);
        return cb::text::replace_all(std::string(ran.out.begin(), ran.out.end()), "\r", "");
    };
    CB_CHECK_EQ(build_and_run("streams", data / "streams.golden.cpp"), cb::text::replace_all(cb::fs::read_text(data / "streams.jvm.txt"), "\r", ""));
    (void)build_and_run("gc_check", data / "gc_check.cpp");
});

CB_TEST("translate.cpp17.runs_as_the_jvm.bank", [] { prints_what_the_jvm_printed("bank"); });
CB_TEST("translate.cpp17.runs_as_the_jvm.calc", [] { prints_what_the_jvm_printed("calc"); });
CB_TEST("translate.cpp17.runs_as_the_jvm.generics", [] { prints_what_the_jvm_printed("generics"); });
CB_TEST("translate.cpp17.runs_as_the_jvm.mix", [] { prints_what_the_jvm_printed("mix"); });
CB_TEST("translate.cpp17.runs_as_the_jvm.names", [] { prints_what_the_jvm_printed("names"); });
CB_TEST("translate.cpp17.runs_as_the_jvm.shapes", [] { prints_what_the_jvm_printed("shapes"); });
CB_TEST("translate.cpp17.runs_as_the_jvm.streams", [] { prints_what_the_jvm_printed("streams"); });
CB_TEST("translate.cpp17.runs_as_the_jvm.t17", [] { prints_what_the_jvm_printed("t17"); });
CB_TEST("translate.cpp17.runs_as_the_jvm.textstats", [] { prints_what_the_jvm_printed("textstats"); });
CB_TEST("translate.cpp17.runs_as_the_jvm.zoo", [] { prints_what_the_jvm_printed("zoo"); });
