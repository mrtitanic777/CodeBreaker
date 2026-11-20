// Tests of the tools behind the window's Tools page (tools.hpp, tools_run.hpp, settings_edit.hpp): every command of the table is some tool's command (or is
// named as left out), every tool is a drop and at most one choice with no typed arguments, the developer tools are hidden unless asked for, the command
// line a form builds, the result folder a run reports, the work folder's modules, the running of a child.
#include "app/tools.hpp"

#include <algorithm>
#include <cstdio>
#include <set>

#include "app/commands.hpp"
#include "app/settings_edit.hpp"
#include "app/tools_run.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"

namespace cb::app::tools {
namespace {

// A check that says which tool or command it was about when it fails.
bool tt_ok(bool ok, const std::string& what) {
    if (!ok) std::fprintf(stderr, "  (about: %s)\n", what.c_str());
    return ok;
}

CB_TEST("app.tools.every_command_is_some_tools_command_or_is_left_out", [] {
    std::set<std::string> left;
    for (const LeftOut& l : left_out()) {
        left.insert(l.name);
        CB_CHECK(!l.reason.empty());
    }
    std::set<std::string> commands;
    for (const Tool& t : all()) commands.insert(std::string(t.command()));
    for (const CommandDoc& doc : command_docs()) {
        const std::string name(doc.name);
        CB_CHECK(tt_ok(commands.contains(name) != left.contains(name), name));   // a tool's, or left out with the reason: never both, never neither
    }
    for (const std::string& c : commands) CB_CHECK(tt_ok(find_command_doc(c) != nullptr, c));   // (no tool runs a command the table does not have)
});

CB_TEST("app.tools.every_tool_is_one_drop_and_at_most_one_choice_with_nothing_typed", [] {
    std::set<std::string> ids, titles;
    for (const Tool& t : all()) {
        CB_CHECK(ids.insert(t.id).second && titles.insert(t.title).second);   // every id and every title is its own
        CB_CHECK(!t.title.empty() && t.description.size() > 20 && t.description.back() == '.');
        CB_CHECK(std::find(groups().begin(), groups().end(), t.group) != groups().end());
        CB_CHECK(tt_ok(t.developer == is_developer_group(t.group), t.id));        // the developer tools are exactly the developer groups'
        CB_CHECK(tt_ok(t.inputs.size() <= 2, t.id));
        CB_CHECK(tt_ok(t.inputs.size() < 2 || t.group == "Compare", t.id));       // two inputs only to compare two things
        CB_CHECK(tt_ok(t.choices.empty() == t.choice_label.empty(), t.id));
        CB_CHECK(tt_ok(t.choices.empty() || t.choices.size() >= 2, t.id));        // a choice is between buttons
        CB_CHECK(tt_ok(!t.argv.empty(), t.id));
        CB_CHECK(t.title.find("--") == std::string::npos && t.description.find("--") == std::string::npos);   // no command-line words in the words
        // every placeholder of the command line is one the form fills
        for (const std::string& w : t.argv) {
            for (std::size_t at = w.find('{'); at != std::string::npos; at = w.find('{', at + 1)) {
                const std::string key = w.substr(at + 1, w.find('}', at) - at - 1);
                const bool input2 = key.back() == '2';
                CB_CHECK(tt_ok(key == "choice" ? !t.choices.empty() : t.inputs.size() >= (input2 ? 2u : 1u), t.id + ": " + key));
            }
        }
    }
    // the everyday groups, in their order, then the developer ones
    CB_CHECK(std::vector<std::string>(groups().begin(), groups().begin() + 5) == std::vector<std::string>({"Convert", "Look inside", "Extract", "Build", "Compare"}));
    CB_CHECK(!is_developer_group("Convert") && is_developer_group("Collection"));
    // the list is in the order of its groups
    std::size_t last = 0;
    for (const Tool& t : all()) {
        const std::size_t g = static_cast<std::size_t>(std::find(groups().begin(), groups().end(), t.group) - groups().begin());
        CB_CHECK(g >= last);
        last = g;
    }
});

CB_TEST("app.tools.the_spec_parsers_read_inputs_and_choices", [] {
    const std::vector<Input> in = parse_inputs("F Older build; E Game: an .xex, a package; M Module; X nonsense; D Source folder");
    CB_CHECK_EQ(in.size(), 4u);
    CB_CHECK(in[0].kind == InputKind::File && in[0].label == "Older build" && in[0].accepts.empty());
    CB_CHECK(in[1].kind == InputKind::Either && in[1].label == "Game" && in[1].accepts == "an .xex, a package");
    CB_CHECK(in[2].kind == InputKind::Module && in[3].kind == InputKind::Folder);
    std::vector<Choice> choices;
    CB_CHECK_EQ(parse_choices("Platform=PC:pc|Xbox 360:xbox360", choices), "Platform");
    CB_CHECK(choices.size() == 2 && choices[1].label == "Xbox 360" && choices[1].value == "xbox360");
    CB_CHECK_EQ(parse_choices("Do=Only look:--dry|Tidy up:", choices), "Do");
    CB_CHECK(choices.size() == 2 && choices[1].value.empty());
    CB_CHECK(parse_choices("", choices).empty() && choices.empty());
});

CB_TEST("app.tools.java_to_cpp_is_one_drop_one_choice_and_run", [] {
    const Tool* t = find("java-port");
    CB_CHECK(t != nullptr);
    if (t == nullptr) return;
    CB_CHECK_EQ(t->title, "Java to C++");
    CB_CHECK(t->group == "Convert" && !t->developer);
    CB_CHECK(t->inputs.size() == 1 && t->inputs[0].kind == InputKind::Either);
    CB_CHECK(t->choice_label == "Platform" && t->choices.size() == 2 && t->choices[0].label == "PC" && t->choices[1].label == "Xbox 360");
    Values v = empty_values(*t);
    CB_CHECK_EQ(missing(*t, v).value_or(""), "Java program");   // Run waits for the drop
    v.inputs[0] = "C:/Games/a1.2.6.jar";
    CB_CHECK(!missing(*t, v));
    CB_CHECK(build_argv(*t, v) == std::vector<std::string>({"java", "port", "C:/Games/a1.2.6.jar", "--platform", "pc"}));
    v.choice = 1;
    CB_CHECK(build_argv(*t, v) == std::vector<std::string>({"java", "port", "C:/Games/a1.2.6.jar", "--platform", "xbox360"}));
});

CB_TEST("app.tools.the_command_line_is_filled_from_the_drop_and_the_choice", [] {
    {   // output beside the input
        const Tool* t = find("xbox-disc");
        Values v = empty_values(*t);
        v.inputs[0] = "C:/Games/Halo.iso";
        CB_CHECK(build_argv(*t, v) == std::vector<std::string>({"xbox-disc", "unpack", "C:/Games/Halo.iso", "C:/Games/Halo"}));
    }
    {   // two inputs
        const Tool* t = find("compare");
        Values v = empty_values(*t);
        v.inputs[0] = "old.dll";
        CB_CHECK_EQ(missing(*t, v).value_or(""), "Newer build");
        v.inputs[1] = "new.dll";
        CB_CHECK(build_argv(*t, v) == std::vector<std::string>({"compare", "old.dll", "new.dll"}));
    }
    {   // a choice whose value is nothing leaves nothing
        const Tool* t = find("tidy-state");
        Values v = empty_values(*t);
        CB_CHECK(build_argv(*t, v) == std::vector<std::string>({"tidy-state", "--dry"}));
        v.choice = 1;
        CB_CHECK(build_argv(*t, v) == std::vector<std::string>({"tidy-state"}));
    }
    {   // an option whose value is empty goes with it
        Tool t;
        t.argv = {"convert", "{1}", "--to", "{choice}"};
        t.inputs = {{InputKind::File, "Program", ""}};
        Values v = empty_values(t);
        v.inputs[0] = "a.exe";
        CB_CHECK(build_argv(t, v) == std::vector<std::string>({"convert", "a.exe"}));
    }
    {   // a tool with nothing to drop runs at once
        const Tool* t = find("scoreboard");
        CB_CHECK(!missing(*t, empty_values(*t)) && build_argv(*t, empty_values(*t)) == std::vector<std::string>({"scoreboard"}));
    }
    // a drop goes where it fits
    CB_CHECK(accepts({InputKind::File, "x", ""}, false) && !accepts({InputKind::File, "x", ""}, true));
    CB_CHECK(accepts({InputKind::Folder, "x", ""}, true) && !accepts({InputKind::Folder, "x", ""}, false));
    CB_CHECK(accepts({InputKind::Either, "x", ""}, true) && accepts({InputKind::Either, "x", ""}, false));
    CB_CHECK(!accepts({InputKind::Module, "x", ""}, false));
});

CB_TEST("app.tools.the_developer_tools_are_hidden_unless_asked_for", [] {
    const std::vector<const Tool*> everyday = search("", false), every = search("", true);
    CB_CHECK(every.size() == all().size() && everyday.size() < every.size());
    for (const Tool* t : everyday) CB_CHECK(tt_ok(!t->developer, t->id));
    for (const std::string id : {"sweep", "scoreboard", "place", "judge-cs-cpp", "exact", "module", "cache", "regress"}) {
        const Tool* t = find(id);
        CB_CHECK(tt_ok(t != nullptr && t->developer, id));
    }
    // search: the title, the command and the description, every word, any case
    const auto ids = [](const std::vector<const Tool*>& found) {
        std::set<std::string> out;
        for (const Tool* t : found) out.insert(t->id);
        return out;
    };
    CB_CHECK(ids(search("java", false)).contains("java-port"));
    CB_CHECK(ids(search("CABINET", false)).contains("cab"));
    CB_CHECK(!ids(search("scoreboard", false)).contains("scoreboard") && ids(search("scoreboard", true)).contains("scoreboard"));
    CB_CHECK(search("no such words here", true).empty());
});

CB_TEST("app.tools.the_result_folder_is_the_output_line_else_the_last_path_that_is_there", [] {
    fs::ScopedTempDir dir{"cb_tools_result"};
    const std::filesystem::path out = dir.path() / "game-converted";
    fs::make_dirs(out);
    fs::write_text(out / "Game.sln", "x");
    CB_CHECK(result_folder("wrote things\nOutput: " + fs::to_text(out) + "\n") == out);
    CB_CHECK(result_folder("  report: " + fs::to_text(out / "Game.sln") + "\n100%  Done\n") == out);           // a file: its folder
    CB_CHECK(result_folder("extracted 3 files to " + fs::to_text(out) + ".\n") == out);                        // (the sentence's full stop is not the path's)
    CB_CHECK(result_folder("Output: C:/no/such/folder\n").empty());
    CB_CHECK(result_folder("nothing here\n").empty());
});

CB_TEST("app.tools.the_work_folders_modules_are_its_program_files", [] {
    fs::ScopedTempDir dir{"cb_tools_modules"};
    fs::make_dirs(dir.path() / "SYSTEM");
    fs::write_text(dir.path() / "SYSTEM" / "LZ32.DLL", "x");
    fs::write_text(dir.path() / "kernel32.dll", "x");
    fs::write_text(dir.path() / "readme.txt", "x");
    fs::write_text(dir.path() / "SYSTEM" / "lz32.dll.bak", "x");
    const std::vector<std::string> modules = modules_in(dir.path());
    CB_CHECK(modules == std::vector<std::string>({"kernel32.dll", "LZ32.DLL"}));
    CB_CHECK(modules_in(dir.path() / "missing").empty());
    const std::vector<std::string> many = {"gdi.exe", "kernel32.dll", "krnl386.exe", "lz32.dll", "user.exe"};
    CB_CHECK(filter_modules(many, "k", 8) == std::vector<std::string>({"kernel32.dll", "krnl386.exe"}));
    CB_CHECK(filter_modules(many, "EXE", 8) == std::vector<std::string>({"gdi.exe", "krnl386.exe", "user.exe"}));   // (holds it, no name starts with it)
    CB_CHECK(filter_modules(many, "l", 8).front() == "lz32.dll");                                                   // starting with it first
    CB_CHECK_EQ(filter_modules(many, "", 2).size(), 2u);
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
    // Show developer tools: off at first, kept beside the theme
    CB_CHECK(!settings::remembered_developer_tools());
    settings::remember_developer_tools(true);
    CB_CHECK(settings::remembered_developer_tools() && settings::remembered_theme() == settings::Theme::Light);
    settings::remember_developer_tools(false);
    CB_CHECK(!settings::remembered_developer_tools());
});

}  // namespace
}  // namespace cb::app::tools
