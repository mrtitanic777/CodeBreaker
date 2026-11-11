#include "cb/core/cli.hpp"

#include "cb/core/testing.hpp"

using namespace cb;
using namespace cb::cli;

namespace {

// A small command table in the shape the application will have.
Cli make_cli(int* last_exit = nullptr) {
    std::vector<Command> commands;
    commands.push_back(Command{
        "decompile", "decompile one function or a whole module",
        {Positional{"module", "corpus name or path"},
         Option{"--function", "-f", "name or hex address", Arg::Value, "NAME"},
         Option{"--judge", "", "run the equivalence judge", Arg::Flag},
         Option{"--verbose", "-v", "more output (repeatable)", Arg::Flag},
         Option{"--quiet", "-q", "less output", Arg::Flag},
         Option{"--jobs", "-j", "workers", Arg::Value, "N", "4"},
         Option{"--include", "-I", "include directory (repeatable)", Arg::Value, "DIR"}},
        [last_exit](const Args& a) {
            if (last_exit != nullptr) *last_exit = 0;
            return a.flag("judge") ? 1 : 0;
        },
        {}, "Decompile"});
    commands.push_back(Command{"hex", "dump bytes",
                               {Positional{"file", "input"}, Positional{"extra", "more files", false, true}},
                               [](const Args&) { return 0; }, {}, "Inspect"});
    commands.push_back(Command{"xbox-disc", "Xbox disc tools", {}, nullptr,
                               {Command{"list", "list the files", {Positional{"iso", "the disc image"}},
                                        [](const Args&) { return 7; }}},
                               "Inspect"});
    return Cli("codebreaker", std::move(commands));
}

std::vector<std::string> argv_of(std::initializer_list<const char*> a) { return {a.begin(), a.end()}; }

}  // namespace

CB_TEST("core.cli.positionals_options_and_flags", [] {
    Cli cli = make_cli();
    auto p = cli.parse(argv_of({"decompile", "lz32.dll", "--function", "main", "--judge"}));
    CB_CHECK_EQ(p.command->name, "decompile");
    CB_CHECK_EQ(p.args.required("module"), "lz32.dll");
    CB_CHECK_EQ(*p.args.value("function"), "main");
    CB_CHECK(p.args.flag("judge"));
    CB_CHECK(!p.args.flag("quiet"));
    CB_CHECK_EQ(p.args.value_or("function", "x"), "main");
});

CB_TEST("core.cli.value_forms", [] {
    Cli cli = make_cli();
    CB_CHECK_EQ(*cli.parse(argv_of({"decompile", "m", "--function=a b"})).args.value("function"), "a b");
    CB_CHECK_EQ(*cli.parse(argv_of({"decompile", "m", "-f", "x"})).args.value("function"), "x");
    CB_CHECK_EQ(*cli.parse(argv_of({"decompile", "m", "-fx"})).args.value("function"), "x");
    CB_CHECK_EQ(*cli.parse(argv_of({"decompile", "m", "-f=x"})).args.value("function"), "x");
    // options before the positional work too
    auto p = cli.parse(argv_of({"decompile", "--judge", "m"}));
    CB_CHECK_EQ(p.args.required("module"), "m");
});

CB_TEST("core.cli.clusters_repeats_defaults", [] {
    Cli cli = make_cli();
    auto p = cli.parse(argv_of({"decompile", "m", "-vvq", "-I", "a", "-Ib", "--include", "c"}));
    CB_CHECK_EQ(p.args.count("verbose"), 2u);
    CB_CHECK(p.args.flag("quiet"));
    auto inc = p.args.values("include");
    CB_CHECK_EQ(inc.size(), 3u);
    CB_CHECK_EQ(inc[1], "b");
    CB_CHECK_EQ(*p.args.integer<int>("jobs"), 4);           // the default
    auto q = cli.parse(argv_of({"decompile", "m", "-j", "0x10"}));
    CB_CHECK_EQ(*q.args.integer<int>("jobs"), 16);
    CB_CHECK(!q.args.value("function").has_value());
    CB_CHECK(!q.args.integer<int>("function").has_value());
    // a cluster ending in a value option: -vj8
    auto r = cli.parse(argv_of({"decompile", "m", "-vj8"}));
    CB_CHECK_EQ(*r.args.integer<int>("jobs"), 8);
    CB_CHECK_EQ(r.args.count("verbose"), 1u);
});

CB_TEST("core.cli.double_dash_dash_and_negative_numbers", [] {
    Cli cli = make_cli();
    auto p = cli.parse(argv_of({"hex", "--", "-weird-file", "--also"}));
    CB_CHECK_EQ(p.args.required("file"), "-weird-file");
    CB_CHECK_EQ(p.args.values("extra").size(), 1u);
    auto stdin_dash = cli.parse(argv_of({"hex", "-"}));
    CB_CHECK_EQ(stdin_dash.args.required("file"), "-");   // a lone dash is a positional
    auto neg = cli.parse(argv_of({"hex", "-5"}));
    CB_CHECK_EQ(neg.args.required("file"), "-5");          // no option claims -5
});

CB_TEST("core.cli.variadic", [] {
    Cli cli = make_cli();
    auto p = cli.parse(argv_of({"hex", "a", "b", "c", "d"}));
    CB_CHECK_EQ(p.args.required("file"), "a");
    CB_CHECK_EQ(p.args.values("extra").size(), 3u);
    CB_CHECK_EQ(p.args.values("extra")[2], "d");
    CB_CHECK_EQ(cli.parse(argv_of({"hex", "a"})).args.values("extra").size(), 0u);
});

CB_TEST("core.cli.usage_errors", [] {
    Cli cli = make_cli();
    CB_CHECK_THROWS_AS(UsageError, cli.parse(argv_of({})));
    CB_CHECK_THROWS_AS(UsageError, cli.parse(argv_of({"nonsense"})));
    CB_CHECK_THROWS_AS(UsageError, cli.parse(argv_of({"decompile"})));                       // missing <module>
    CB_CHECK_THROWS_AS(UsageError, cli.parse(argv_of({"decompile", "m", "--nope"})));
    CB_CHECK_THROWS_AS(UsageError, cli.parse(argv_of({"decompile", "m", "-z"})));
    CB_CHECK_THROWS_AS(UsageError, cli.parse(argv_of({"decompile", "m", "--function"})));    // needs a value
    CB_CHECK_THROWS_AS(UsageError, cli.parse(argv_of({"decompile", "m", "-f"})));
    CB_CHECK_THROWS_AS(UsageError, cli.parse(argv_of({"decompile", "m", "--judge=yes"})));   // a flag takes none
    CB_CHECK_THROWS_AS(UsageError, cli.parse(argv_of({"decompile", "m", "extra"})));         // one positional only
    CB_CHECK_THROWS_AS(UsageError, cli.parse(argv_of({"decompile", "m", "-j", "ten"})).args.integer<int>("jobs"));
    try {
        (void)cli.parse(argv_of({"decompil", "m"}));
    } catch (const UsageError& e) {
        CB_CHECK(std::string(e.what()).find("did you mean 'decompile'") != std::string::npos);
    }
});

CB_TEST("core.cli.subcommands", [] {
    Cli cli = make_cli();
    auto p = cli.parse(argv_of({"xbox-disc", "list", "game.iso"}));
    CB_CHECK_EQ(p.command->name, "list");
    CB_CHECK_EQ(p.path, "xbox-disc list");
    CB_CHECK_EQ(p.args.required("iso"), "game.iso");
    CB_CHECK(cli.parse(argv_of({"xbox-disc"})).help);                   // a group without a subcommand shows its help
    CB_CHECK_THROWS_AS(UsageError, cli.parse(argv_of({"xbox-disc", "burn"})));
    CB_CHECK(cli.find({"xbox-disc", "list"}) != nullptr);
    CB_CHECK(cli.find({"xbox-disc", "burn"}) == nullptr);
    testing::CapturedStd captured;
    CB_CHECK_EQ(cli.run(argv_of({"xbox-disc", "list", "a.iso"})), 7);   // the handler's code is returned
});

CB_TEST("core.cli.run_returns_codes", [] {
    Cli cli = make_cli();
    testing::CapturedStd captured;
    CB_CHECK_EQ(cli.run(argv_of({"decompile", "m"})), 0);
    CB_CHECK_EQ(cli.run(argv_of({"decompile", "m", "--judge"})), 1);   // handler says "failed"
    CB_CHECK_EQ(cli.run(argv_of({"decompile"})), kExitUsage);           // usage error: message on stderr, code 2
    CB_CHECK_EQ(cli.run(argv_of({"decompile", "--help"})), kExitOk);
    CB_CHECK_EQ(cli.run(argv_of({"help"})), kExitOk);
    CB_CHECK_EQ(cli.run(argv_of({"help", "decompile"})), kExitOk);
    CB_CHECK_EQ(cli.run(argv_of({"help", "xbox-disc", "list"})), kExitOk);
    CB_CHECK_EQ(cli.run(argv_of({"help", "bogus"})), kExitUsage);
    CB_CHECK(captured.out().find("usage: codebreaker decompile [options] <module>") != std::string::npos);
    CB_CHECK(captured.out().find("Decompile:\n  decompile") != std::string::npos);
    CB_CHECK(captured.err().find("missing argument <module>") != std::string::npos);   // errors go to stderr, not stdout
    CB_CHECK(captured.out().find("missing argument") == std::string::npos);
    CB_CHECK(captured.err().find("unknown command 'bogus'") != std::string::npos);
});

CB_TEST("core.cli.help_text", [] {
    Cli cli = make_cli();
    const Command& c = *cli.find({"decompile"});
    const std::string h = cli.help(c, "decompile");
    CB_CHECK(h.starts_with("usage: codebreaker decompile [options] <module>\n"));
    CB_CHECK(h.find("decompile one function or a whole module") != std::string::npos);
    CB_CHECK(h.find("arguments:\n  module") != std::string::npos);
    CB_CHECK(h.find("-f, --function NAME") != std::string::npos);
    CB_CHECK(h.find("--judge") != std::string::npos);
    CB_CHECK(h.find("(default: 4)") != std::string::npos);
    CB_CHECK(h.find("-h, --help") != std::string::npos);
    // the right column lines up
    const auto col = [&](const std::string& left) { return h.find(left) - h.rfind('\n', h.find(left)); };
    CB_CHECK_EQ(col("--judge ") + 0, col("--quiet ") + 0);
    const std::string o = cli.overview();
    CB_CHECK(o.find("Decompile:\n  decompile") != std::string::npos);
    CB_CHECK(o.find("Inspect:\n  hex") != std::string::npos);
    CB_CHECK(o.find("xbox-disc") != std::string::npos);
    const std::string g = cli.help(*cli.find({"xbox-disc"}), "xbox-disc");
    CB_CHECK(g.find("commands:\n  list") != std::string::npos);
    CB_CHECK(g.find("<command>") != std::string::npos);
    const std::string v = cli.help(*cli.find({"hex"}), "hex");
    CB_CHECK(v.find("<file> [<extra>...]") != std::string::npos);
});
