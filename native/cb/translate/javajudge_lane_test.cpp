// Tests of the java lane as the tree judge's lane (javajudge_lane.hpp), over the transpiler as the application assembles it
// (java/assembly) -- everything native/cb has but the Java front end, which is javac's: the front end here serves the units a run
// of the Python front end made of the fixtures (testdata/javajudge/units.json, the L2 documents of `cpp03.java_units`).
//
//   * the dialect each back end asks for reaches the lane as an option (the flags, the label the transpiler is asked for), and
//     nothing else is touched: two judgements of two dialects in a row are each made in theirs;
//   * the nine fixture classes (the Python java kind's own: tests/java/fixtures/java) judged on each back end give, class by class,
//     the verdict, the gaps and the reason the Python javajudge.judge_tree gave (testdata/javajudge/verdicts.python.json, made by
//     running it). The host compiler and the JDK are needed; without them the test says so and passes nothing it did not check.

#include "cb/translate/javajudge_lane.hpp"

#include <nlohmann/json.hpp>

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <set>

#include "cb/core/arena.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/process.hpp"
#include "cb/core/testing.hpp"
#include "cb/java/assembly.hpp"
#include "cb/translate/l2.hpp"

namespace cb::translate::javajudge {
namespace {

namespace stdfs = std::filesystem;

bool jl_tool_runs(const char* tool, const char* flag) {
    try {
        return process::run({.argv = {tool, flag}}).ok();
    } catch (const ToolError&) {
        return false;
    }
}

bool jl_have_tools() { return jl_tool_runs("g++", "--version") && jl_tool_runs("javac", "-version") && jl_tool_runs("java", "-version"); }

verify::lanes::JavaLane::Tools jl_tools() {
    verify::lanes::JavaLane::Tools t;
    t.cxx = "g++";
    t.javac = "javac";
    t.java = "java";
    return t;
}

java::assembly::FrontEnd jl_front_end(const Json& units) {
    return [&units](Arena& arena, const stdfs::path&) {
        std::vector<ir::Node*> out;
        for (const Json& doc : units) out.push_back(l2::from_json(arena, doc));
        return out;
    };
}

CB_TEST("translate.javajudge_lane.a_back_end_asks_for_its_dialect", [] {
    CB_CHECK(dialect_of(Backend::Cpp03).label == "C++03" && dialect_of(Backend::Cpp03).standard == "c++03");
    CB_CHECK(dialect_of(Backend::Cpp17).label == "C++17" && dialect_of(Backend::Cpp17).standard == "c++17");
    CB_CHECK((verify::lanes::JavaLane::flags(dialect_of(Backend::Cpp17).standard) ==
              std::vector<std::string>{"-std=c++17", "-fsyntax-only", "-w"}));
});

CB_TEST("translate.javajudge_lane.the_lane_is_given_the_dialect_it_converts_in", [] {
    fs::ScopedTempDir dir("cb_jj_lane");
    fs::write_text(dir.path() / "src" / "A.java", "public class A { }\n");
    std::vector<std::string> asked;
    JavaTreeLane lane([] { return verify::lanes::JavaLane::Tools{}; }, [&](std::string_view label) {
        asked.emplace_back(label);
        return [](const stdfs::path&, const stdfs::path& out) {
            fs::write_text(out / "A.h", "class A { };\n");
            return java::transpile::Summary{};
        };
    });
    CB_CHECK((lane.prepare(dir.path() / "src", dir.path() / "w03", Backend::Cpp03) == std::vector<std::string>{"A"}));
    CB_CHECK((lane.prepare(dir.path() / "src", dir.path() / "w17", Backend::Cpp17) == std::vector<std::string>{"A"}));
    CB_CHECK((asked == std::vector<std::string>{"C++03", "C++17"}));
    CB_CHECK_EQ(lane.gaps("A"), std::size_t{0});
    CB_CHECK_THROWS_AS(UsageError, JavaTreeLane([] { return verify::lanes::JavaLane::Tools{}; }, nullptr).judge("A"));
});

// The program in `folder` (src/, units.json, verdicts.python.json) judged on each back end and compared, class by class, with the
// verdict, the gaps and the reason the Python gave.
void jl_judge_against_python(const stdfs::path& folder) {
    const Json units = json::load(folder / "units.json");
    const Json golden = json::load(folder / "verdicts.python.json");
    const stdfs::path src = folder / "src";
    java::assembly::Assembly assembly(java::JavaData(testing::data_path("../../data/java")), jl_front_end(units));
    const bool times = fs::get_env("CB_JAVAJUDGE_TIMES").has_value();
    JavaTreeLane lane(jl_tools, [&](std::string_view label) {
        verify::lanes::JavaLane::Convert convert = assembly.converter(std::string(label));
        if (!times) return convert;
        return verify::lanes::JavaLane::Convert([convert](const stdfs::path& in, const stdfs::path& out) {
            const auto t0 = std::chrono::steady_clock::now();
            auto done = convert(in, out);
            std::fprintf(stderr, "        (transpile %.2fs)\n", std::chrono::duration<double>(std::chrono::steady_clock::now() - t0).count());
            return done;
        });
    });
    for (const Backend backend : {Backend::Cpp03, Backend::Cpp17}) {
        fs::ScopedTempDir work("cb_jj_judge");
        const auto started = std::chrono::steady_clock::now();
        const auto since = [&] { return std::chrono::duration<double>(std::chrono::steady_clock::now() - started).count(); };
        TreeOptions options{.workers = 2};
        if (times) {   // (what the proof run reads: the time of each step)
            options.log = [&](std::string_view line) { std::fprintf(stderr, "%7.2fs %.*s\n", since(), static_cast<int>(line.size()), line.data()); };
        }
        const auto judged = judge_tree(lane, src, work.path(), backend, options);
        const Json& want = golden[std::string(backend_name(backend))];
        CB_CHECK_EQ(judged.size(), want.size());
        for (const auto& [name, j] : judged) {
            CB_CHECK(want.contains(name));
            if (!want.contains(name)) continue;
            const Json& w = want[name];
            CB_CHECK_EQ(std::string(word(j.verdict)), w["verdict"].get<std::string>());
            CB_CHECK_EQ(j.gaps, w["gaps"].get<std::size_t>());
            CB_CHECK_EQ(j.detail.substr(0, j.detail.find(':')), w["reason"].get<std::string>());
        }
    }
}

CB_TEST("translate.javajudge_lane.fixtures_are_judged_as_the_python_judged_them", [] {
    if (!jl_have_tools()) {
        CB_CHECK(true);   // (no host compiler or JDK here: nothing to run)
        return;
    }
    jl_judge_against_python(testing::data_path("translate/testdata/javajudge"));
});

// More programs, made the same way and kept outside the checkout (the proof run's: CB_JAVAJUDGE_EXTRA is a folder of folders, each
// with src/, units.json and verdicts.python.json).
CB_TEST("translate.javajudge_lane.more_programs_as_the_python_judged_them", [] {
    const std::optional<std::string> extra = fs::get_env("CB_JAVAJUDGE_EXTRA");
    if (!extra || !jl_have_tools()) return;
    for (const auto& entry : stdfs::directory_iterator(fs::from_text(*extra))) {
        if (entry.is_directory()) jl_judge_against_python(entry.path());
    }
});

}  // namespace
}  // namespace cb::translate::javajudge
