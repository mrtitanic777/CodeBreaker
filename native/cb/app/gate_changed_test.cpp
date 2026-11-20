// Tests of --changed's rules (gate_changed.hpp): what a change selects, over a small tree and over text.
#include "cb/app/gate_changed.hpp"

#include <algorithm>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace cb::app::gate_changed {
namespace {

// A toy tree: app -> ui? no: ui includes app; app includes engine, decompile; engine includes decompile; decompile includes formats (and back: formats
// includes build, build includes decompile, a cycle like the real one); xna includes decompile; its test includes engine.
Graph toy() {
    return graph_of({
        {"/r/native/cb/app/commands.cpp", "#include \"cb/engine/a.hpp\"\n#include \"cb/decompile/a.hpp\"\n"},
        {"/r/native/cb/ui/serve.cpp", "#include \"cb/app/commands.hpp\"\n"},
        {"/r/native/cb/engine/a.cpp", "#include \"cb/decompile/a.hpp\"\n"},
        {"/r/native/cb/decompile/a.cpp", "#include \"cb/formats/a.hpp\"\n#include \"cb/core/a.hpp\"\n"},
        {"/r/native/cb/formats/a.cpp", "#include \"cb/build/a.hpp\"\n"},
        {"/r/native/cb/build/a.cpp", "#include \"cb/decompile/a.hpp\"\n"},
        {"/r/native/cb/xna/a.cpp", "#include \"cb/decompile/a.hpp\"\n"},
        {"/r/native/cb/xna/a_test.cpp", "#include \"cb/engine/a.hpp\"\n"},
        {"/r/native/cb/core/a.cpp", ""},
    });
}

CommandAreas toy_map() { return parse_areas("# comment\ninfo: formats\nxna: xna\nserve: ui\nconvert: app:convert\n--help: core\n"); }

bool runs(const std::vector<std::string>& changed, const std::string& name, const std::string& command, const std::vector<std::string>& extra = {}) {
    return case_selected(select(changed), toy(), toy_map(), name, command, extra);
}

CB_TEST("app.gate_changed.the_include_graph_reaches_through_cycles_and_a_test_only_counts_for_unit_tests", [] {
    const Graph g = toy();
    CB_CHECK(g.code.at("xna").contains("decompile") && !g.code.at("xna").contains("engine"));   // the test's include is not the code's
    CB_CHECK(g.all.at("xna").contains("engine"));
    const std::set<std::string> down = reach_down(g.code, {"formats"});
    CB_CHECK(down.contains("build") && down.contains("decompile") && down.contains("core") && !down.contains("xna") && !down.contains("app"));
    const std::set<std::string> up = reach_up(g.all, {"formats"});
    CB_CHECK(up.contains("decompile") && up.contains("build") && up.contains("engine") && up.contains("app") && up.contains("ui") && up.contains("xna"));
    CB_CHECK(!up.contains("core"));
});

CB_TEST("app.gate_changed.a_window_file_runs_the_app_s_tests_and_no_command_case", [] {
    const Selection s = select({"native/cb/app/window_win.cpp", "native/cb/app/tools_run.cpp"});
    CB_CHECK(!s.everything && s.areas == std::set<std::string>({"app:window"}));
    const std::set<std::string> units = unit_areas(s, toy());
    CB_CHECK(units == std::set<std::string>({"app", "ui"}));   // ui includes app
    CB_CHECK(!runs({"native/cb/app/window_win.cpp"}, "info-pe", "info"));
    CB_CHECK(!runs({"native/cb/app/window_win.cpp"}, "serve-x", "serve"));
    CB_CHECK(!runs({"native/cb/app/window_win.cpp"}, "convert-x", "convert"));
});

CB_TEST("app.gate_changed.an_area_runs_the_cases_whose_commands_reach_it_and_the_unit_tests_of_what_includes_it", [] {
    // xna is reached by the xna command alone
    CB_CHECK(runs({"native/cb/xna/a.cpp"}, "xna-1", "xna"));
    CB_CHECK(!runs({"native/cb/xna/a.cpp"}, "info-pe", "info"));
    CB_CHECK(!runs({"native/cb/xna/a.cpp"}, "serve-x", "serve"));
    // decompile is reached by info (formats -> build -> decompile), by serve (ui -> app -> decompile), not by --help (core)
    CB_CHECK(runs({"native/cb/decompile/a.cpp"}, "info-pe", "info"));
    CB_CHECK(runs({"native/cb/decompile/a.cpp"}, "serve-x", "serve"));
    CB_CHECK(!runs({"native/cb/decompile/a.cpp"}, "help", "--help"));
    // a command nobody described always runs once something changed; an @areas on the case describes it
    CB_CHECK(runs({"native/cb/xna/a.cpp"}, "mystery", "mystery"));
    CB_CHECK(!runs({"native/cb/xna/a.cpp"}, "mystery", "mystery", {"core"}));
    CB_CHECK(runs({"native/cb/xna/a.cpp"}, "mystery", "mystery", {"xna"}));
    // the convert pipeline
    CB_CHECK(runs({"native/cb/app/convert.cpp"}, "convert-x", "convert"));
    CB_CHECK(!runs({"native/cb/app/convert.cpp"}, "info-pe", "info"));
    CB_CHECK(runs({"native/cb/decompile/a.cpp"}, "convert-x", "convert"));   // (convert is app code: it reaches what app includes)
    const std::set<std::string> units = unit_areas(select({"native/cb/decompile/a.cpp"}), toy());
    CB_CHECK(units.contains("engine") && units.contains("xna") && units.contains("app") && units.contains("formats") && !units.contains("core"));
    // a test file alone: its area's unit tests, no case
    const Selection t = select({"native/cb/formats/a_test.cpp"});
    CB_CHECK(!t.everything && t.areas.empty() && unit_areas(t, toy()) == std::set<std::string>({"formats"}));
    CB_CHECK(!runs({"native/cb/formats/a_test.cpp"}, "info-pe", "info"));
});

CB_TEST("app.gate_changed.the_foundations_and_the_unknown_run_everything", [] {
    for (const char* path : {"native/cb/core/fs.cpp", "native/cb/base/cache.cpp", "native/cb/CMakeLists.txt", "native/CMakeLists.txt", "tools/gate.cmd",
                             "native/cb/app/exe_gate_main.cpp", "native/cb/app/commands_catalog.inc", "native/cb/app/native_commands.cpp",
                             "native/cb/formats/testdata/lz32.dll", "tests/exe_gate/in/x.txt", "tests/fixtures/y", "data/mystery/z.json", "native/cb/third_party/bin/a.dll",
                             "native/nasm/whatever.c", "case-vars"}) {
        const Selection s = select({path});
        CB_CHECK(s.everything);
        CB_CHECK(runs({path}, "info-pe", "info") && runs({path}, "help", "--help"));
    }
});

CB_TEST("app.gate_changed.data_is_its_area_s_code_a_golden_or_a_case_line_is_that_case", [] {
    const Selection d = select({"config/build/kept_as_asm.txt"});
    CB_CHECK(!d.everything && d.areas == std::set<std::string>({"build"}));
    CB_CHECK(runs({"config/build/kept_as_asm.txt"}, "info-pe", "info"));   // formats -> build
    const Selection g = select({"tests/exe_gate/golden/info-pe.txt", "case:info-ne"});
    CB_CHECK(!g.everything && g.areas.empty() && g.cases == std::set<std::string>({"info-pe", "info-ne"}));
    CB_CHECK(case_selected(g, toy(), toy_map(), "info-pe", "info", {}) && case_selected(g, toy(), toy_map(), "info-ne", "info", {}));
    CB_CHECK(!case_selected(g, toy(), toy_map(), "info-le", "info", {}));
    CB_CHECK(unit_areas(g, toy()).empty());
});

CB_TEST("app.gate_changed.a_record_is_the_exact_difference_of_two_trees_by_content", [] {
    fs::ScopedTempDir dir{"cb_gate_changed"};
    const std::filesystem::path r = dir.path();
    fs::make_dirs(r / "native" / "cb" / "xna");
    fs::make_dirs(r / "native" / "cb" / "app");
    fs::make_dirs(r / "config" / "build");
    fs::make_dirs(r / "tests" / "exe_gate");
    fs::write_text(r / "native" / "cb" / "xna" / "a.cpp", "int a;\n");
    fs::write_text(r / "native" / "cb" / "app" / "window_win.cpp", "int w;\n");
    fs::write_text(r / "config" / "build" / "k.txt", "k\n");
    fs::write_text(r / "tests" / "exe_gate" / "cases.txt", "info-pe: info x\n");   // not a file of the record: its lines are
    const std::map<std::string, std::string> extra = {{"case:info-pe", "aa"}, {"case-vars", "vv"}};
    const Record first = scan_tree(r, nullptr, extra);
    CB_CHECK(first.files.contains("native/cb/xna/a.cpp") && first.files.contains("config/build/k.txt") && first.files.contains("case:info-pe"));
    CB_CHECK(!first.files.contains("tests/exe_gate/cases.txt"));
    const Record round = read_record(record_text(first));
    CB_CHECK(round.files == first.files);
    CB_CHECK(changed_paths(round, scan_tree(r, &round, extra)).empty());
    // saved again with the same content: unchanged
    fs::write_text(r / "native" / "cb" / "xna" / "a.cpp", "int a;\n");
    CB_CHECK(changed_paths(round, scan_tree(r, &round, extra)).empty());
    // one file edited, one added, one case line edited
    fs::write_text(r / "native" / "cb" / "app" / "window_win.cpp", "int w2;\n");
    fs::write_text(r / "config" / "build" / "new.txt", "n\n");
    std::map<std::string, std::string> extra2 = extra;
    extra2["case:info-pe"] = "bb";
    std::vector<std::string> changed = changed_paths(round, scan_tree(r, &round, extra2));
    std::sort(changed.begin(), changed.end());
    CB_CHECK(changed == (std::vector<std::string>{"case:info-pe", "config/build/new.txt", "native/cb/app/window_win.cpp"}));
    // a file removed counts too
    fs::remove_tree(r / "native" / "cb" / "xna");
    changed = changed_paths(round, scan_tree(r, &round, extra));
    CB_CHECK(std::find(changed.begin(), changed.end(), "native/cb/xna/a.cpp") != changed.end());
    // a record of another format is no record
    CB_CHECK(read_record("something else\n").files.empty());
});

}  // namespace
}  // namespace cb::app::gate_changed
