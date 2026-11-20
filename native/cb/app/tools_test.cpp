// Tests of the tools behind the window's Tools page (tools.hpp, tools_run.hpp, settings_edit.hpp): every command of the table is a tool with a title, a group and a
// description (or is named as left out), the argument-spec and usage parsers, search, the validation of a form, the command line it builds, the running of a child.
#include "cb/app/tools.hpp"

#include <algorithm>
#include <set>

#include "cb/app/commands.hpp"
#include "cb/app/settings_edit.hpp"
#include "cb/app/tools_run.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace cb::app::tools {
namespace {

const Arg* tt_arg(const Tool& t, const std::string& label) {
    for (const Arg& a : t.args)
        if (a.label == label) return &a;
    return nullptr;
}

CB_TEST("app.tools.every_command_is_a_tool_with_a_title_a_group_and_a_description_or_is_left_out", [] {
    std::set<std::string> left;
    for (const LeftOut& l : left_out()) {
        left.insert(l.name);
        CB_CHECK(!l.reason.empty());
    }
    std::set<std::string> titles;
    for (const CommandDoc& doc : command_docs()) {
        const std::string name(doc.name);
        const Tool* t = find(name);
        if (left.contains(name)) {
            CB_CHECK(t == nullptr);
            continue;
        }
        CB_CHECK(t != nullptr);
        if (t == nullptr) continue;
        CB_CHECK(!t->title.empty() && !t->group.empty() && t->description.size() > 20);
        CB_CHECK(std::find(groups().begin(), groups().end(), t->group) != groups().end());
        CB_CHECK(t->description.back() == '.');                 // one plain sentence
        CB_CHECK(t->title.find("--") == std::string::npos && t->description.find("--") == std::string::npos || name == "java");   // no command-line words in the words
        CB_CHECK(titles.insert(t->title).second);                // every title is its own
        CB_CHECK_EQ(t->usage, std::string(doc.synopsis));
    }
    CB_CHECK_EQ(all().size() + left.size(), command_docs().size());
    // the names the person asked for
    CB_CHECK_EQ(find("pe-source")->title, "Windows program");
    CB_CHECK_EQ(find("ne-source")->title, "Windows 3.x program");
    CB_CHECK_EQ(find("dos-source")->title, "DOS program");
    CB_CHECK_EQ(find("convert")->title, "Convert to source");
    CB_CHECK_EQ(find("info")->title, "What is this file?");
    CB_CHECK_EQ(find("analyze")->title, "Analyze a program");
    CB_CHECK_EQ(find("names")->title, "Function names");
    CB_CHECK_EQ(find("decompile")->title, "Decompile to C");
    CB_CHECK_EQ(find("judge-cs-cpp")->title, "C# to C++");
    CB_CHECK_EQ(find("judge-py-cpp")->title, "Python to C++");
    CB_CHECK_EQ(find("judge-c-java")->title, "C to Java");
});

CB_TEST("app.tools.the_list_is_in_the_order_of_its_groups", [] {
    std::size_t last = 0;
    for (const Tool& t : all()) {
        const std::size_t g = static_cast<std::size_t>(std::find(groups().begin(), groups().end(), t.group) - groups().begin());
        CB_CHECK(g >= last);
        last = g;
    }
    for (const std::string& g : groups())
        CB_CHECK(std::any_of(all().begin(), all().end(), [&](const Tool& t) { return t.group == g; }));   // no empty group
});

CB_TEST("app.tools.an_argument_spec_gives_the_fields_of_a_form", [] {
    const std::vector<Arg> a = parse_spec("F Program; ?F Stub_file --stub; ?B Check_it --check; C Action=list|extract; ?*T Names; ?W More");
    CB_CHECK(a.size() == 6);
    CB_CHECK(a[0].kind == ArgKind::File && a[0].required && a[0].label == "Program" && a[0].flag.empty());
    CB_CHECK(a[1].kind == ArgKind::File && !a[1].required && a[1].flag == "--stub" && a[1].label == "Stub file");
    CB_CHECK(a[2].kind == ArgKind::Toggle && a[2].flag == "--check" && !a[2].required);
    CB_CHECK(a[3].kind == ArgKind::Choice && a[3].required && a[3].choices == std::vector<std::string>({"list", "extract"}));
    CB_CHECK(a[4].list && a[4].kind == ArgKind::Text && !a[4].required);
    CB_CHECK(a[5].kind == ArgKind::Words);
    CB_CHECK(parse_spec("").empty());
    CB_CHECK(parse_spec(";; Z nonsense ;").empty());   // what it cannot read is left out
});

CB_TEST("app.tools.a_usage_line_gives_the_same_fields", [] {
    const std::vector<Arg> a = parse_usage("pe-source <program> [--stub F]");
    CB_CHECK(a.size() == 2);
    CB_CHECK(a[0].kind == ArgKind::File && a[0].required);
    CB_CHECK(a[1].flag == "--stub" && a[1].kind == ArgKind::File && !a[1].required);
    const std::vector<Arg> m = parse_usage("module <name> [--no-verify]");
    CB_CHECK(m.size() == 2 && m[0].kind == ArgKind::Text && m[1].kind == ArgKind::Toggle && m[1].flag == "--no-verify");
    const std::vector<Arg> c = parse_usage("media list|extract <image>");
    CB_CHECK(c.size() == 2 && c[0].kind == ArgKind::Choice && c[0].choices.size() == 2 && c[1].kind == ArgKind::File);
    const std::vector<Arg> l = parse_usage("driver-services [FILE ...]");
    CB_CHECK(!l.empty());
    const std::vector<Arg> d = parse_usage("dos-build <folder> [--against F]");
    CB_CHECK(d[0].kind == ArgKind::Folder);
    CB_CHECK(parse_usage("scoreboard").empty());
    CB_CHECK(parse_usage("cache [--stats|--prune|--clear]").size() == 3);   // (three toggles)
    // the tools that take their spec from the usage line have one
    CB_CHECK(!find("pe-source")->args.empty());
    CB_CHECK(find("pe-source")->args[0].required && find("pe-source")->args[0].kind == ArgKind::File);
});

CB_TEST("app.tools.search_matches_the_title_the_command_name_and_the_description", [] {
    const auto names = [](const std::vector<const Tool*>& v) {
        std::set<std::string> s;
        for (const Tool* t : v) s.insert(t->name);
        return s;
    };
    CB_CHECK(search("").size() == all().size());
    CB_CHECK(names(search("pe-source")).contains("pe-source"));                  // the command name
    CB_CHECK(names(search("Windows program")).contains("pe-source"));            // the title, any case
    CB_CHECK(names(search("windows PROGRAM")).contains("pe-source"));
    CB_CHECK(names(search("32-bit windows program back")).contains("pe-source"));              // the description
    CB_CHECK(names(search("rebuilds prove")).contains("pe-source"));             // every word, in any order
    CB_CHECK(names(search("python")).contains("judge-py-cpp"));
    CB_CHECK(search("no such tool anywhere").empty());
    CB_CHECK(matches(*find("info"), "what is this"));
});

CB_TEST("app.tools.run_waits_for_the_required_fields_and_builds_the_command_line", [] {
    const Tool& t = *find("pe-source");
    Values v = empty_values(t);
    CB_CHECK_EQ(*missing(t, v), "Program");
    v.text[0] = "   ";
    CB_CHECK(missing(t, v).has_value());   // blanks are not a value
    v.text[0] = "C:/Games/PINBALL.EXE";
    CB_CHECK(!missing(t, v).has_value());
    CB_CHECK(build_argv(t, v) == std::vector<std::string>({"pe-source", "C:/Games/PINBALL.EXE"}));
    v.text[1] = "stub.exe";
    CB_CHECK(build_argv(t, v) == std::vector<std::string>({"pe-source", "C:/Games/PINBALL.EXE", "--stub", "stub.exe"}));
    // toggles, choices, lists and words
    const Tool& j = *find("judge-cs-cpp");
    Values w = empty_values(j);
    CB_CHECK(!missing(j, w).has_value());   // (everything is optional: judges the corpus)
    w.text[0] = "gcc";
    w.text[1] = "1";
    w.text[4] = "A.cs; B.cs";
    CB_CHECK(build_argv(j, w) == std::vector<std::string>({"judge-cs-cpp", "--cxx", "gcc", "--keep", "A.cs", "B.cs"}));
    const Tool& x = *find("xex");
    Values y = empty_values(x);
    y.text[0] = "info";
    y.text[1] = "game.xex";
    y.text[2] = "--verbose  extra";
    CB_CHECK(build_argv(x, y) == std::vector<std::string>({"xex", "info", "game.xex", "--verbose", "extra"}));
    // a required choice starts on its first value
    CB_CHECK_EQ(empty_values(*find("media")).text[0], "list");
    CB_CHECK(!missing(*find("scoreboard"), empty_values(*find("scoreboard"))).has_value());
    CB_CHECK(build_argv(*find("scoreboard"), empty_values(*find("scoreboard"))) == std::vector<std::string>({"scoreboard"}));
});

CB_TEST("app.tools.a_percentage_at_the_start_of_a_line_is_the_progress", [] {
    CB_CHECK_EQ(percent_of_line("  35%  Translating: 6 of 6 functions"), 35);
    CB_CHECK_EQ(percent_of_line("100%  Done"), 100);
    CB_CHECK_EQ(percent_of_line("  0%  Identifying the file"), 0);
    CB_CHECK_EQ(percent_of_line("about 50% done"), -1);
    CB_CHECK_EQ(percent_of_line("150%"), -1);
    CB_CHECK_EQ(percent_of_line(""), -1);
});

#ifdef _WIN32
CB_TEST("app.tools.a_run_streams_the_output_reports_percentages_and_ends_with_the_exit_code", [] {
    Run run({"/c", "echo  40%  half&& echo done"}, "cmd");
    RunState s = run.state();
    for (int i = 0; i < 200 && s.running; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        s = run.state();
    }
    CB_CHECK(!s.running);
    CB_CHECK_EQ(s.exit_code, 0);
    CB_CHECK(s.output.find("half") != std::string::npos && s.output.find("done") != std::string::npos);
    CB_CHECK_EQ(s.percent, 100);   // (a run that reported percentages and succeeded ends at 100)
    CB_CHECK(!s.stopped);
});

CB_TEST("app.tools.stop_ends_the_child_it_started", [] {
    Run run({"/c", "ping -n 30 127.0.0.1 >nul"}, "cmd");
    CB_CHECK(run.state().running);
    run.stop();
    RunState s = run.state();
    for (int i = 0; i < 200 && s.running; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        s = run.state();
    }
    CB_CHECK(!s.running && s.stopped);
});

CB_TEST("app.tools.a_program_that_cannot_start_says_why", [] {
    Run run({}, "no-such-program-anywhere.exe");
    const RunState s = run.state();
    CB_CHECK(!s.running && !s.failure.empty() && s.exit_code != 0);
});
#endif

CB_TEST("app.tools.settings_are_rows_a_form_can_edit_and_the_theme_is_remembered", [] {
    fs::ScopedTempDir dir{"cb_settings_edit"};
    fs::write_text(dir.path() / "settings.json", "{\"masm\": \"tools/masm\", \"low_load\": true, \"workers\": 3}");
    const config::Config config = config::Config::load(dir.path());
    const std::vector<settings::Row> rows = settings::rows(config);
    CB_CHECK(!rows.empty());
    const auto row = [&](const std::string& key) -> const settings::Row* {
        for (const settings::Row& r : rows)
            if (r.key == key) return &r;
        return nullptr;
    };
    CB_CHECK(row("msvc152") != nullptr && row("msvc152")->kind == settings::Kind::Path && !row("msvc152")->present);   // (named but not there)
    CB_CHECK(row("low_load") != nullptr && row("low_load")->kind == settings::Kind::Toggle);
    CB_CHECK_EQ(row("msvc152")->label, "Visual C++ 1.52");   // (plain words; the key stays the settings.json key)
    CB_CHECK(!row("low_load")->hint.empty());
    CB_CHECK_EQ(settings::plain_label("some_new_key"), "Some new key");
    CB_CHECK(settings::is_everyday("converted") && settings::is_everyday("low_load") && !settings::is_everyday("msvc152"));
    {   // what this computer can convert: one Ready line first, then a row with its setting for each tool not found
        const std::vector<settings::Row> some = {{"java", "C:\\jdk", settings::Kind::Path, true, true, "", ""}, {"ilspycmd", "", settings::Kind::Path, false, false, "", ""}};
        const std::vector<settings::Capability> can = settings::capabilities(some);
        CB_CHECK(can.size() == 3 && can[0].ready && can[0].label == "Ready");
        CB_CHECK(can[0].note.find("Java programs") != std::string::npos);
        CB_CHECK(!can[1].ready && can[1].key == "ilspycmd" && !can[1].folder);
        CB_CHECK(!can[2].ready && can[2].key == "xbox360_sdk" && can[2].folder);   // (no row at all: missing too)
    }
    CB_CHECK_EQ(settings::write(config, "low_load", "false", settings::Kind::Toggle), "");
    CB_CHECK_EQ(settings::write(config, "masm", "C:/tools/masm", settings::Kind::Path), "");
    const config::Config again = config::Config::load(dir.path());
    CB_CHECK(!again.truthy("low_load"));
    CB_CHECK_EQ(again.text("masm"), "C:/tools/masm");
    CB_CHECK_EQ(again.text("workers"), "3");   // (what was there stays)
    // the theme
    const std::string file = fs::to_text(dir.path() / "app.json");
    fs::ScopedEnv named("CB_APP_SETTINGS", std::string_view(file));
    CB_CHECK(settings::remembered_theme() == settings::Theme::System);
    settings::remember_theme(settings::Theme::Dark);
    CB_CHECK(settings::remembered_theme() == settings::Theme::Dark);
    settings::remember_theme(settings::Theme::Light);
    CB_CHECK(settings::remembered_theme() == settings::Theme::Light);
});

}  // namespace
}  // namespace cb::app::tools
