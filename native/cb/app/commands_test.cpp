// Tests of the command table: the catalog, `--corpus`, dispatch to raw and declared commands, the fallback, help,
// and the mapping of errors to exit codes.

#include "cb/app/commands.hpp"

#include <set>

#include "cb/core/error.hpp"
#include "cb/core/testing.hpp"

using namespace cb;
using namespace cb::app;

namespace {

using Words = std::vector<std::string>;

Words words(std::span<const std::string> s) {
    return Words(s.begin(), s.end());
}

} // namespace

CB_TEST("app.commands.catalog_lists_every_command", [] {
    // the 72 commands of the Python __main__.COMMANDS table (its 70 documented ones, score16 and exact)
    const char* expected[] = {"module",
                              "scoreboard",
                              "place",
                              "sweep",
                              "measure",
                              "match",
                              "score16",
                              "repair",
                              "teeth",
                              "invert",
                              "exact",
                              "vxd",
                              "ne-asm",
                              "build16",
                              "flat-thunk",
                              "decompile",
                              "names",
                              "identify",
                              "demangle",
                              "media",
                              "dos-functions",
                              "dos-interrupts",
                              "analyze",
                              "driver-services",
                              "driver-source",
                              "dos-source",
                              "dos-build",
                              "pe-source",
                              "ne-source",
                              "le-source",
                              "origin",
                              "boot-floppy",
                              "driver-build",
                              "driver-judge",
                              "build",
                              "resources",
                              "cab",
                              "disc",
                              "versions",
                              "fonts",
                              "info",
                              "xex",
                              "coverage",
                              "reach",
                              "mcp",
                              "serve",
                              "ui",
                              "progress",
                              "settings",
                              "cache",
                              "facts",
                              "knowledge",
                              "tidy-state",
                              "recompile",
                              "recompile-project",
                              "xenon-recompile",
                              "xdk-match",
                              "xex-imports",
                              "xex-edit",
                              "ppc-calls",
                              "heavyiron",
                              "gfx",
                              "classes",
                              "system-ui",
                              "crypto",
                              "java",
                              "xna",
                              "xbox-disc",
                              "xex-basic",
                              "xenon-config",
                              "switch-tables",
                              "corpus-executables"};
    std::set<std::string> have;
    for (const CommandDoc& d : command_docs()) {
        CB_CHECK(have.insert(std::string(d.name)).second); // no name twice
        CB_CHECK(!d.synopsis.empty());
        CB_CHECK(d.synopsis.starts_with(d.name)); // a synopsis begins with the command's name
    }
    CB_CHECK_EQ(have.size(), std::size_t{72});
    for (const char* n : expected)
        CB_CHECK(have.count(n) == 1);
    const CommandDoc* d = find_command_doc("flat-thunk");
    CB_CHECK(d != nullptr);
    CB_CHECK(d->synopsis == "flat-thunk <module> [--recover DIR] [--build DIR] [--ow-build DIR]");
    CB_CHECK(find_command_doc("no-such-command") == nullptr);
});

CB_TEST("app.commands.corpus_option", [] {
    Words a = {"decompile", "lz32.dll", "--corpus", "other", "--out", "x"};
    CorpusOption o = take_corpus_option(a);
    CB_CHECK(o.corpus == std::optional<std::string>("other"));
    CB_CHECK(o.rest == (Words{"decompile", "lz32.dll", "--out", "x"}));
    // before the command works too
    o = take_corpus_option(Words{"--corpus", "x", "info", "a.dll"});
    CB_CHECK(o.corpus == std::optional<std::string>("x") && o.rest == (Words{"info", "a.dll"}));
    // none given: the line is as it was
    o = take_corpus_option(Words{"info"});
    CB_CHECK(!o.corpus && o.rest == (Words{"info"}));
    // only the first is taken
    o = take_corpus_option(Words{"--corpus", "a", "--corpus", "b"});
    CB_CHECK(o.corpus == std::optional<std::string>("a") && o.rest == (Words{"--corpus", "b"}));
    // no name: a usage error that lists the known corpora
    Words known = {"win95", "x360"};
    try {
        (void)take_corpus_option(Words{"info", "--corpus"}, known);
        CB_CHECK(false);
    } catch (const UsageError& e) {
        CB_CHECK(std::string(e.what()).find("win95, x360") != std::string::npos);
    }
});

CB_TEST("app.commands.dispatch_raw", [] {
    std::optional<Words> seen;
    std::string corpus;
    Environment env;
    env.use_corpus = [&](const std::string& n) { corpus = n; };
    CommandTable table(env);
    table.set("info", [&](std::span<const std::string> args) {
        seen = words(args);
        return 7;
    });
    CB_CHECK(table.has_runner("info"));
    CB_CHECK(!table.has_runner("decompile"));
    CB_CHECK_EQ(table.run({"--corpus", "win95", "info", "a.dll", "--json"}), 7);
    CB_CHECK(seen && *seen == (Words{"a.dll", "--json"})); // the command's own words, no name, no --corpus
    CB_CHECK_EQ(corpus, "win95");
    // a later registration replaces the earlier
    table.set("info", [](std::span<const std::string>) { return 9; });
    CB_CHECK_EQ(table.run({"info"}), 9);
});

CB_TEST("app.commands.dispatch_declared", [] {
    CommandTable table;
    std::string got_module;
    bool got_flag = false;
    table.set(
        cli::Command{"measure",
                     "reconstruct and judge one module",
                     {cli::Positional{"module", "corpus name"}, cli::Option{"--no-engine", "", "", cli::Arg::Flag}},
                     [&](const cli::Args& a) {
                         got_module = a.required("module");
                         got_flag = a.flag("no-engine");
                         return 0;
                     },
                     {},
                     "",
                     ""});
    CB_CHECK_EQ(table.run({"measure", "lz32.dll", "--no-engine"}), 0);
    CB_CHECK_EQ(got_module, "lz32.dll");
    CB_CHECK(got_flag);
    testing::CapturedStd cap;
    CB_CHECK_EQ(table.run({"measure"}), kExitUsage); // the declared command's own usage check: a module is needed
    CB_CHECK(!cap.err().empty());
    CB_CHECK_EQ(table.run({"measure", "--help"}), kExitOk);
    CB_CHECK(cap.out().find("module") != std::string::npos);
});

CB_TEST("app.commands.help_and_unknown", [] {
    CommandTable table;
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(table.run({"help"}), kExitOk);
        CB_CHECK_EQ(table.run({"--help"}), kExitOk);
        CB_CHECK_EQ(table.run({"-h"}), kExitOk);
        const std::string out = cap.out();
        CB_CHECK(out.find("usage: codebreaker <command>") != std::string::npos);
        CB_CHECK(out.find("decompile <name> [--out FILE]") != std::string::npos);
        CB_CHECK(out.find("decompile any module (32-bit, 16-bit, PowerPC) to C; nothing is built") !=
                 std::string::npos);
        CB_CHECK(out.find("--corpus NAME") != std::string::npos);
        CB_CHECK(cap.err().empty()); // help is a result: stdout
    }
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(table.run({}), kExitUsage); // no command and not in a terminal: usage, on stderr
        CB_CHECK_EQ(table.run({"frobnicate"}), kExitUsage);
        const std::string err = cap.err();
        CB_CHECK(err.find("unknown command 'frobnicate'") != std::string::npos);
        CB_CHECK(err.find("usage: codebreaker") != std::string::npos);
        CB_CHECK(cap.out().empty());
    }
});

CB_TEST("app.commands.usage_layout", [] {
    CommandTable table;
    const std::string u = table.usage();
    // a short synopsis has its summary on the same line at column 36; a long one has it on the lines below
    CB_CHECK(u.find("    ui [module]                     CodeBreaker in the command prompt") != std::string::npos);
    const std::size_t at = u.find("    flat-thunk <module> [--recover DIR] [--build DIR] [--ow-build DIR]\n");
    CB_CHECK(at != std::string::npos);
    CB_CHECK(u.find("\n" + std::string(36, ' ') + "a 32-to-16-bit thunk DLL", at) != std::string::npos);
    // no line is longer than the wrap width (the synopsis alone excepted)
    for (std::string_view line : text::lines(u))
        if (line.find("    flat-thunk") != 0)
            CB_CHECK(line.size() <= 120);
});

CB_TEST("app.commands.terminal_default", [] {
    std::optional<Words> seen;
    bool available = true;
    Environment env;
    env.terminal_interface_available = [&] { return available; };
    CommandTable table(env);
    table.set("ui", [&](std::span<const std::string> args) {
        seen = words(args);
        return 0;
    });
    CB_CHECK_EQ(table.run({}), 0); // no command, in a terminal with the interface: it starts
    CB_CHECK(seen && seen->empty());
    seen.reset();
    available = false;
    testing::CapturedStd cap;
    CB_CHECK_EQ(table.run({}), kExitUsage); // without it: usage
    CB_CHECK(!seen);
});

CB_TEST("app.commands.fallback_and_unavailable", [] {
    CommandTable table;
    // a listed command with no runner is not available...
    CB_CHECK_THROWS_AS(UsageError, table.run({"decompile", "x"}));
    testing::CapturedStd cap;
    CB_CHECK_EQ(table.main({"decompile", "x"}), kExitUsage); // ...and `main` reports it
    CB_CHECK(cap.err().find("not available in this build") != std::string::npos);
    // ...until there is a fallback, which is handed the whole line, name first
    std::optional<Words> seen;
    table.set_fallback([&](std::span<const std::string> args) {
        seen = words(args);
        return 5;
    });
    CB_CHECK(table.has_runner("decompile"));
    CB_CHECK_EQ(table.run({"decompile", "x", "--out", "y"}), 5);
    CB_CHECK(seen && *seen == (Words{"decompile", "x", "--out", "y"}));
    CB_CHECK(!table.has_runner("frobnicate")); // not a command: nothing to hand it to
});

CB_TEST("app.commands.main_maps_errors", [] {
    CommandTable table;
    table.set("info", [](std::span<const std::string>) -> int { throw NotFoundError("x.dll", 2); });
    table.set("cab", [](std::span<const std::string>) -> int { throw UsageError("bad option"); });
    table.set("settings", [](std::span<const std::string>) -> int { throw InternalError("invariant"); });
    table.set("cache", [](std::span<const std::string>) -> int { throw std::runtime_error("something else"); });
    testing::CapturedStd cap;
    CB_CHECK_EQ(table.main({"info"}), kExitNotFound);
    CB_CHECK_EQ(table.main({"cab"}), kExitUsage);
    CB_CHECK_EQ(table.main({"settings"}), kExitInternal);
    CB_CHECK_EQ(table.main({"cache"}), kExitInternal); // not a cb::Error: a bug
    const std::string err = cap.err();
    CB_CHECK(err.find("bad option") != std::string::npos);
    CB_CHECK(err.find("something else") != std::string::npos);
});

CB_TEST("app.commands.set_rejects_undocumented", [] {
    CommandTable table;
    CB_CHECK_THROWS_AS(InternalError, table.set("not-a-command", [](std::span<const std::string>) { return 0; }));
});
