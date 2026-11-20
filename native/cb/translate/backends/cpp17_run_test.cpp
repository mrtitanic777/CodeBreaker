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

#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/process.hpp"
#include "cb/core/testing.hpp"

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
    build.argv = {"cl", "/nologo", "/std:c++17", "/EHsc", "/utf-8", "/permissive-", "/I" + cb::testing::data_path("../../config/translate/runtime/cpp17").string(),
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
