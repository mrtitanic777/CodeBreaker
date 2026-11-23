// Tests of the verbs (verbs.hpp): every older command belongs to a verb, `help` lists exactly the verbs, each mode routes to the command it is the
// alias of, each verb picks its steps from the kind of its input, and the verbs run the older commands through a command table.

#include "app/verbs.hpp"

#include <map>
#include <set>

#include "app/commands.hpp"
#include "core/error.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"

using namespace cb;
using namespace cb::app;
using namespace cb::app::verbs;

namespace {

using Words = std::vector<std::string>;

// A probe that knows a few made-up inputs.
Probe fake_probe() {
    static const std::map<std::string, Kind> known = {
        {"a.exe", Kind::Pe},         {"b.dll", Kind::Ne},         {"c.vxd", Kind::Le},          {"d.com", Kind::Dos},   {"g.xex", Kind::Xex},
        {"a.cab", Kind::Cab},        {"a.img", Kind::Disc},       {"a.iso", Kind::XboxDisc},    {"a.ho", Kind::HeavyIron}, {"m.gfx", Kind::Gfx},
        {"v.fon", Kind::Font},       {"S.java", Kind::JavaSource}, {"S.cs", Kind::CsSource},    {"S.py", Kind::PySource}, {"x.toml", Kind::Config},
        {"drv", Kind::DriverSource}, {"dos", Kind::DosSource},    {"csrc", Kind::CSource},      {"empty", Kind::Folder}, {"d2.vxd", Kind::Le},
        {"f.asm", Kind::Other},      {"blob.bin", Kind::Other},
    };
    return [](const std::string& path) {
        const auto it = known.find(path);
        return it == known.end() ? Kind::Missing : it->second;
    };
}

Route routed(std::string_view verb, Words args) {
    const std::optional<Route> r = route(verb, args, fake_probe());
    CB_CHECK(r.has_value());
    return r.value_or(Route{});
}

Step step(std::string command, Words args) { return Step{std::move(command), std::move(args)}; }

}  // namespace

CB_TEST("app.verbs.every_older_command_belongs_to_a_verb", [] {
    std::set<std::string_view> kept(kept_names().begin(), kept_names().end());
    for (const CommandDoc& d : command_docs()) {
        if (is_verb(d.name) || kept.contains(d.name)) {
            CB_CHECK(verb_of(d.name).empty()); // a verb or a kept command is nobody's alias
            continue;
        }
        const std::string_view verb = verb_of(d.name);
        CB_CHECK(!verb.empty());
        CB_CHECK(is_verb(verb) || verb == "cache"); // cache takes in tidy-state
        CB_CHECK(is_alias(d.name));
    }
    CB_CHECK_EQ(verb_of("decompile"), std::string_view("convert"));
    CB_CHECK_EQ(verb_of("pe-source"), std::string_view("convert"));
    CB_CHECK_EQ(verb_of("info"), std::string_view("inspect"));
    CB_CHECK_EQ(verb_of("cab"), std::string_view("extract"));
    CB_CHECK_EQ(verb_of("dos-build"), std::string_view("build"));
    CB_CHECK_EQ(verb_of("teeth"), std::string_view("check"));
    CB_CHECK_EQ(verb_of("versions"), std::string_view("compare"));
    CB_CHECK_EQ(verb_of("sweep"), std::string_view("corpus"));
    CB_CHECK_EQ(verb_of("demangle"), std::string_view("lookup"));
    CB_CHECK_EQ(verb_of("xex-edit"), std::string_view("xbox"));
    CB_CHECK_EQ(verb_of("tidy-state"), std::string_view("cache"));
    // every mode names a command of the catalog, and every command of the catalog but the verbs and the kept ones is some mode's
    for (const Mode& m : modes()) {
        CB_CHECK(find_command_doc(m.old_name) != nullptr);
        CB_CHECK(find_command_doc(m.verb) != nullptr || m.verb == "cache");
    }
    CB_CHECK_EQ(verb_names().size(), std::size_t{9});
});

CB_TEST("app.verbs.help_lists_exactly_the_new_verbs", [] {
    CommandTable table;
    const std::string usage = table.usage();
    const std::size_t also = usage.find("\nAlso:\n");
    CB_CHECK(also != std::string::npos);
    const auto names_of = [](std::string_view block) {
        std::vector<std::string> names;
        for (std::string_view line : text::lines(block)) {
            if (!line.starts_with("    ") || line.starts_with("     ")) continue; // a row begins at four spaces; a continuation is further in
            const std::string_view rest = line.substr(4);
            names.emplace_back(rest.substr(0, rest.find(' ')));
        }
        return names;
    };
    const std::vector<std::string> verbs = names_of(std::string_view(usage).substr(0, also));
    CB_CHECK(verbs == (Words{"convert", "inspect", "extract", "build", "check", "compare", "corpus", "lookup", "xbox"}));
    const std::vector<std::string> rest = names_of(std::string_view(usage).substr(also));
    CB_CHECK(rest == (Words{"settings", "cache", "regress", "serve", "mcp", "app", "disassemble", "assemble", "compile", "link"}));
    for (const CommandDoc& d : command_docs())
        if (is_alias(d.name)) CB_CHECK(usage.find("\n    " + std::string(d.name) + " ") == std::string::npos); // no older name is listed
});

CB_TEST("app.verbs.each_verb_says_what_it_does_and_ends_with_its_old_names", [] {
    for (const std::string_view verb : verb_names()) {
        const std::string h = help(verb);
        CB_CHECK(h.starts_with("usage: codebreaker " + std::string(verb)));
        if (verb != "compare") CB_CHECK(h.find("options:") != std::string::npos || h.find("what to do:") != std::string::npos);
        const std::size_t old = h.find("Old names");
        CB_CHECK(old != std::string::npos);
        for (const Mode& m : modes())
            if (m.verb == verb && !is_verb(m.old_name)) CB_CHECK(h.find(std::string(m.old_name), old) != std::string::npos);
    }
    CB_CHECK(help("tidy-state").empty());
    testing::CapturedStd cap;
    CommandTable table;
    CB_CHECK_EQ(table.run({"convert", "--help"}), kExitOk);
    CB_CHECK_EQ(table.run({"help", "extract"}), kExitOk);
    CB_CHECK(cap.out().find("usage: codebreaker convert") != std::string::npos);
    CB_CHECK(cap.out().find("usage: codebreaker extract") != std::string::npos);
    CB_CHECK(cap.out().find("--asm") != std::string::npos);
});

CB_TEST("app.verbs.every_mode_routes_to_the_command_it_is_the_alias_of", [] {
    for (const Mode& m : modes()) {
        if (m.flag.empty()) continue;
        const Kind kind = m.kind.value_or(Kind::Missing);
        const Probe probe = [kind](const std::string&) { return kind; };
        const std::optional<Route> r = route(m.verb, Words{std::string(m.flag), "x"}, probe);
        CB_CHECK(r.has_value());
        if (!r) continue;
        CB_CHECK_EQ(r->front().command, std::string(m.old_name));
    }
});

CB_TEST("app.verbs.convert_picks_the_program_kind_for_assembly", [] {
    CB_CHECK(routed("convert", {"a.exe"}) == Route{step("convert", {"a.exe"})});
    CB_CHECK(routed("convert", {"a.exe", "--to", "cpp17", "--out", "o"}) == Route{step("convert", {"a.exe", "--to", "cpp17", "--out", "o"})});
    CB_CHECK(routed("convert", {"a.exe", "--asm"}) == Route{step("pe-source", {"a.exe"})});
    CB_CHECK(routed("convert", {"--asm", "b.dll"}) == Route{step("ne-source", {"b.dll"})});
    CB_CHECK(routed("convert", {"c.vxd", "--asm"}) == Route{step("le-source", {"c.vxd"})});
    CB_CHECK(routed("convert", {"d.com", "--asm"}) == Route{step("dos-source", {"d.com"})});
    CB_CHECK(routed("convert", {"c.vxd", "--driver"}) == Route{step("driver-source", {"c.vxd"})});
    CB_CHECK(routed("convert", {"a.exe", "--decompile", "--out", "f.c"}) == Route{step("decompile", {"a.exe", "--out", "f.c"})});
    CB_CHECK_THROWS_AS(UsageError, (void)routed("convert", {"nothing", "--asm"}));
    // --check: a game is recompiled and compiled; a program is converted and then judged; a translation is proved by the pipeline itself
    CB_CHECK(routed("convert", {"g.xex", "--check"}) == Route{step("recompile", {"g.xex", "--all", "--check"})});
    CB_CHECK(routed("convert", {"a.exe", "--check"}) == (Route{step("convert", {"a.exe"}), step("measure", {"a.exe"})}));
    CB_CHECK(routed("convert", {"S.cs", "--to", "cpp17", "--check"}) == Route{step("convert", {"S.cs", "--to", "cpp17"})});
    CB_CHECK(routed("convert", {"a.exe", "--asm", "--check"}) == Route{step("pe-source", {"a.exe"})});
    CB_CHECK(routed("convert", {"g.xex", "--project", "--check"}) == Route{step("recompile-project", {"g.xex", "--check"})});
});

CB_TEST("app.verbs.inspect_says_what_it_is_or_asks_one_question", [] {
    CB_CHECK(routed("inspect", {"a.exe"}) == Route{step("info", {"a.exe"})});
    CB_CHECK(routed("inspect", {"g.xex"}) == Route{step("info", {"g.xex"})});
    CB_CHECK(routed("inspect", {"empty"}) == Route{step("reach", {"empty"})}); // a folder: what each program in it is
    CB_CHECK(routed("inspect", {"a.exe", "--names"}) == Route{step("names", {"a.exe"})});
    CB_CHECK(routed("inspect", {"d.com", "--functions"}) == Route{step("dos-functions", {"d.com"})});
    CB_CHECK(routed("inspect", {"a.exe", "--functions"}) == Route{step("names", {"a.exe"})});
    CB_CHECK(routed("inspect", {"g.xex", "--imports"}) == Route{step("xex-imports", {"g.xex"})});
    CB_CHECK(routed("inspect", {"a.exe", "--analyze", "--what", "calls"}) == Route{step("analyze", {"a.exe", "--what", "calls"})});
    CB_CHECK(!route("inspect", Words{}, fake_probe()).has_value());
});

CB_TEST("app.verbs.extract_looks_at_the_container", [] {
    CB_CHECK(routed("extract", {"a.cab", "out"}) == Route{step("cab", {"extract", "a.cab", "out"})});
    CB_CHECK(routed("extract", {"a.cab", "--list"}) == Route{step("cab", {"list", "a.cab"})});
    CB_CHECK(routed("extract", {"a.img", "out"}) == Route{step("media", {"extract", "a.img", "out"})});
    CB_CHECK(routed("extract", {"a.img", "--list"}) == Route{step("media", {"list", "a.img"})});
    CB_CHECK(routed("extract", {"a.iso", "--list"}) == Route{step("xbox-disc", {"files", "a.iso"})});
    CB_CHECK(routed("extract", {"a.iso", "out"}) == Route{step("xbox-disc", {"unpack", "a.iso", "out"})});
    CB_CHECK(routed("extract", {"a.ho", "out"}) == Route{step("heavyiron", {"extract", "a.ho", "out"})});
    CB_CHECK(routed("extract", {"m.gfx"}) == Route{step("gfx", {"images", "m.gfx"})});
    CB_CHECK(routed("extract", {"v.fon"}) == Route{step("fonts", {"v.fon"})});
    CB_CHECK(routed("extract", {"g.xex", "b.bin"}) == Route{step("xex", {"extract", "g.xex", "b.bin"})});
    CB_CHECK(routed("extract", {"--basic", "g.xex", "o.xex"}) == Route{step("xex-basic", {"g.xex", "o.xex"})});
    CB_CHECK(routed("extract", {"--system-ui", "u.bin", "out"}) == Route{step("system-ui", {"u.bin", "out"})});
    CB_CHECK(routed("extract", {"--cab", "x.dat", "out"}) == Route{step("cab", {"extract", "x.dat", "out"})}); // a kind named: no looking
    CB_CHECK_THROWS_AS(UsageError, (void)routed("extract", {"blob.bin", "out"}));
    CB_CHECK(!route("extract", Words{}, fake_probe()).has_value());
});

CB_TEST("app.verbs.build_looks_at_the_folder", [] {
    CB_CHECK(routed("build", {"drv", "--out", "o"}) == Route{step("driver-build", {"drv", "--out", "o"})});
    CB_CHECK(routed("build", {"dos", "--against", "c.com"}) == Route{step("dos-build", {"dos", "--against", "c.com"})});
    CB_CHECK(routed("build", {"csrc"}) == Route{step("build16", {"csrc"})});
    CB_CHECK(routed("build", {"x.toml"}) == Route{step("xenon-recompile", {"x.toml"})});
    CB_CHECK(routed("build", {"f.asm"}) == Route{step("driver-build", {"f.asm"})});
    CB_CHECK(routed("build", {"a.exe", "--out", "o"}) == Route{step("build", {"a.exe", "--out", "o"})}); // a module: as before
    CB_CHECK(routed("build", {"lz32.dll"}) == Route{step("build", {"lz32.dll"})});
    CB_CHECK(routed("build", {"--module", "lz32.dll"}) == Route{step("build", {"lz32.dll"})});
    CB_CHECK(routed("build", {"--disc", "tree", "o.iso"}) == Route{step("disc", {"tree", "o.iso"})});
    CB_CHECK(routed("build", {"--floppy"}) == Route{step("boot-floppy", {})});
    CB_CHECK(routed("build", {"--resources", "lz32.dll"}) == Route{step("resources", {"lz32.dll"})});
    CB_CHECK_THROWS_AS(UsageError, (void)routed("build", {"empty"}));
    CB_CHECK(!route("build", Words{}, fake_probe()).has_value());
});

CB_TEST("app.verbs.check_picks_the_judge_from_the_input", [] {
    CB_CHECK(routed("check", {"a.exe"}) == Route{step("measure", {"a.exe"})});
    CB_CHECK(routed("check", {"lz32.dll"}) == Route{step("measure", {"lz32.dll"})});
    CB_CHECK(routed("check", {"a.exe", "--bytes", "--try"}) == Route{step("match", {"a.exe", "--try"})});
    CB_CHECK(routed("check", {"a.exe", "--teeth", "--rounds", "1"}) == Route{step("teeth", {"a.exe", "--rounds", "1"})});
    CB_CHECK(routed("check", {"S.java"}) == Route{step("judge-c-java", {"roundtrip", "S.java"})});
    CB_CHECK(routed("check", {"S.cs"}) == Route{step("judge-cs-cpp", {"S.cs"})});
    CB_CHECK(routed("check", {"S.py"}) == Route{step("judge-py-cpp", {"S.py"})});
    CB_CHECK(routed("check", {"c.vxd", "d2.vxd"}) == Route{step("driver-judge", {"c.vxd", "d2.vxd"})}); // two drivers: both run and compared
    CB_CHECK(routed("check", {"c.vxd"}) == Route{step("measure", {"c.vxd"})});
    CB_CHECK(routed("check", {"--exact", "lz32.dll"}) == Route{step("exact", {"lz32.dll"})});
    CB_CHECK(!route("check", Words{}, fake_probe()).has_value());
});

CB_TEST("app.verbs.compare_corpus_lookup_xbox", [] {
    CB_CHECK(routed("compare", {"a.exe", "b.dll", "--json", "j"}) == Route{step("versions", {"a.exe", "b.dll", "--json", "j"})});
    CB_CHECK(routed("compare", {"lz32.dll", "--from", "x", "--to", "y"}) == Route{step("versions", {"lz32.dll", "--from", "x", "--to", "y"})});
    CB_CHECK(!route("compare", Words{}, fake_probe()).has_value());

    CB_CHECK(routed("corpus", {}) == Route{step("coverage", {})}); // the whole collection, at a glance
    CB_CHECK(routed("corpus", {"run", "--resume"}) == Route{step("sweep", {"--resume"})});
    CB_CHECK(routed("corpus", {"progress", "--once"}) == Route{step("progress", {"--once"})});
    CB_CHECK(routed("corpus", {"scoreboard"}) == Route{step("scoreboard", {})});
    CB_CHECK(routed("corpus", {"place", "--all"}) == Route{step("place", {"--all"})});
    CB_CHECK(routed("corpus", {"coverage", "--module", "lz32.dll"}) == Route{step("coverage", {"--module", "lz32.dll"})});
    CB_CHECK(routed("corpus", {"list"}) == Route{step("corpus-executables", {})});
    CB_CHECK_THROWS_AS(UsageError, (void)routed("corpus", {"frobnicate"}));

    CB_CHECK(routed("lookup", {"?foo@@YAXH@Z"}) == Route{step("demangle", {"?foo@@YAXH@Z"})});
    CB_CHECK(routed("lookup", {"21", "09"}) == Route{step("dos-interrupts", {"21", "09"})});
    CB_CHECK(routed("lookup", {"0x10"}) == Route{step("dos-interrupts", {"0x10"})});
    CB_CHECK(routed("lookup", {"c.vxd"}) == Route{step("driver-services", {"c.vxd"})});
    CB_CHECK(routed("lookup", {"win95"}) == Route{step("knowledge", {"win95"})});
    CB_CHECK(routed("lookup", {}) == Route{step("knowledge", {})});
    CB_CHECK(routed("lookup", {"--demangle", "foo"}) == Route{step("demangle", {"foo"})});

    CB_CHECK(routed("xbox", {"edit", "privileges", "g.xex"}) == Route{step("xex-edit", {"privileges", "g.xex"})});
    CB_CHECK(routed("xbox", {"xex", "info", "g.xex"}) == Route{step("xex", {"info", "g.xex"})});
    CB_CHECK(routed("xbox", {"crypto", "keys"}) == Route{step("crypto", {"keys"})});
    CB_CHECK(routed("xbox", {"config", "b.xex", "o.toml"}) == Route{step("xenon-config", {"b.xex", "o.toml"})});
    CB_CHECK(routed("xbox", {"match", "g.xex"}) == Route{step("xdk-match", {"g.xex"})});
    CB_CHECK(!route("xbox", Words{}, fake_probe()).has_value());
    CB_CHECK(routed("cache", {"--tidy", "--dry"}) == Route{step("tidy-state", {"--dry"})});
    CB_CHECK(routed("cache", {"--stats"}) == Route{step("cache", {"--stats"})});
});

CB_TEST("app.verbs.run_the_older_commands_through_a_table", [] {
    std::vector<std::pair<std::string, Words>> ran;
    CommandTable table;
    for (const char* name : {"convert", "build", "cache", "info", "cab", "tidy-state", "versions", "sweep", "measure"})
        table.set(name, [&ran, n = std::string(name)](std::span<const std::string> args) {
            ran.emplace_back(n, Words(args.begin(), args.end()));
            return n == "measure" ? 3 : 0;
        });
    register_verbs(table, fake_probe());

    CB_CHECK_EQ(table.run({"convert", "a.exe", "--out", "o"}), 0); // the verb's default is the command it kept
    CB_CHECK(ran.back() == (std::pair<std::string, Words>{"convert", {"a.exe", "--out", "o"}}));
    CB_CHECK_EQ(table.run({"convert"}), 0); // alone: the command's own usage, as before
    CB_CHECK(ran.back() == (std::pair<std::string, Words>{"convert", {}}));
    CB_CHECK_EQ(table.run({"build", "a.exe", "--out", "o"}), 0);
    CB_CHECK(ran.back() == (std::pair<std::string, Words>{"build", {"a.exe", "--out", "o"}}));
    CB_CHECK_EQ(table.run({"inspect", "a.exe"}), 0);
    CB_CHECK(ran.back() == (std::pair<std::string, Words>{"info", {"a.exe"}}));
    CB_CHECK_EQ(table.run({"extract", "a.cab", "out"}), 0);
    CB_CHECK(ran.back() == (std::pair<std::string, Words>{"cab", {"extract", "a.cab", "out"}}));
    CB_CHECK_EQ(table.run({"cache", "--tidy", "--dry"}), 0);
    CB_CHECK(ran.back() == (std::pair<std::string, Words>{"tidy-state", {"--dry"}}));
    CB_CHECK_EQ(table.run({"cache", "--stats"}), 0);
    CB_CHECK(ran.back() == (std::pair<std::string, Words>{"cache", {"--stats"}}));
    CB_CHECK_EQ(table.run({"compare", "a.exe", "b.dll"}), 0);
    CB_CHECK(ran.back() == (std::pair<std::string, Words>{"versions", {"a.exe", "b.dll"}}));
    CB_CHECK_EQ(table.run({"corpus", "run"}), 0);
    CB_CHECK(ran.back() == (std::pair<std::string, Words>{"sweep", {}}));
    // the older names still run, as they were
    CB_CHECK_EQ(table.run({"tidy-state", "--dry"}), 0);
    CB_CHECK(ran.back() == (std::pair<std::string, Words>{"tidy-state", {"--dry"}}));
    // a two-step route stops at the first step that fails
    ran.clear();
    CB_CHECK_EQ(table.run({"convert", "a.exe", "--check"}), 3);
    CB_CHECK_EQ(ran.size(), std::size_t{2}); // convert, then measure (which says 3)
    // no input: the verb's help, and a usage error
    testing::CapturedStd cap;
    CB_CHECK_EQ(table.run({"inspect"}), kExitUsage);
    CB_CHECK(cap.err().find("usage: codebreaker inspect") != std::string::npos);
});
