// Tests of cb/java/run.hpp, the Java transpiler's tool table: the declarations, the dispatch (including a tool provided from
// outside and the choices validated before it runs), and the tools that run in this tree (apimap, bugfix-ledger, debug,
// reflect-factories) on the lane's real data, with what the Python tools printed for the same inputs.
//
// DIVERGES from the Python: usage and help text are cb::cli's, not argparse's (the options, their defaults and the exit codes
// are the same); an option's bad choice is a UsageError (the app exits 2) where argparse exited 2 itself; error messages
// ("ERROR: ...", "No symbols in subsystem ...") go to stderr, results stay on stdout; a tool whose module is not in this tree
// and was not provided is an UnsupportedError naming it, where the Python imported the module and ran it; `apimap
// subsystems` lacks the Python's twelfth, java.platform (the legacy per-class map the Python folds in from
// console/platform_equivalents, which is not in this tree yet: the registry here loads the api_maps files only).
#include "cb/java/run.hpp"

#include <algorithm>
#include <set>
#include <string>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

using namespace cb;
namespace run = cb::java::run;

namespace {

namespace stdfs = std::filesystem;

struct Ran {
    int code = 0;
    std::string out, err;
};

Ran jrun(const std::vector<std::string>& args, const run::Tools& tools = {}) {
    testing::CapturedStd cap;
    Ran r;
    r.code = run::main(args, tools);
    r.out = cap.out();
    r.err = cap.err();
    return r;
}

// The lane's data is found from the checkout that holds this source tree.
struct Checkout {
    fs::ScopedEnv root{"CB_CHECKOUT_ROOT", fs::to_text(testing::data_path("../.."))};
};

}  // namespace

CB_TEST("java.run.tool_table", [] {
    const auto names = run::tool_names();
    const std::vector<std::string> expected = {
        "convert", "decompile", "scaffold", "signatures", "report", "worklist", "bugfix-ledger", "apimap", "sdk",
        "platform-config", "assets", "xact", "achievements", "project-layout", "vcxproj", "build", "debug",
        "reflect-factories", "sound-fix", "controller-nav", "render-fix", "applet-ping", "ui-mark", "keybind-mark", "scan-project"};
    CB_CHECK_EQ(names.size(), expected.size());
    for (std::size_t i = 0; i < expected.size(); ++i) CB_CHECK_EQ(std::string(names[i]), expected[i]);

    const cli::Command command = run::command();
    CB_CHECK_EQ(command.name, "java");
    CB_CHECK_EQ(command.subcommands.size(), expected.size());
    std::set<std::string> areas;
    for (const cli::Command& c : command.subcommands) {
        CB_CHECK(!c.summary.empty());
        CB_CHECK(static_cast<bool>(c.handler));
        areas.insert(c.area);
    }
    CB_CHECK_EQ(areas.size(), std::size_t{4});
});

CB_TEST("java.run.dispatch_to_a_provided_tool", [] {
    // convert is not built in: the owner of the transpiler provides it; the parse hands it every option by name
    run::Tools tools;
    cli::Args seen;
    tools.handlers["convert"] = [&](const cli::Args& a) {
        seen = a;
        return 7;
    };
    const Ran r = jrun({"convert", "--platform", "xbox360", "--project", "--cpp-std=C++11", "src", "out"}, tools);
    CB_CHECK_EQ(r.code, 7);
    CB_CHECK_EQ(seen.value("java_dir").value_or(""), "src");
    CB_CHECK_EQ(seen.value("out_dir").value_or(""), "out");
    CB_CHECK_EQ(seen.value("platform").value_or(""), "xbox360");
    CB_CHECK_EQ(seen.value("cpp-std").value_or(""), "C++11");
    CB_CHECK(seen.flag("project"));
    CB_CHECK(!seen.flag("platform-refs"));
    CB_CHECK(!seen.value("sdk").has_value());

    // defaults of the declaration
    tools.handlers["convert"] = [&](const cli::Args& a) {
        seen = a;
        return 0;
    };
    jrun({"convert", "--mc-version", "a1.2.6", "out"}, tools);
    CB_CHECK_EQ(seen.value("cpp-std").value_or(""), "C++03");
    CB_CHECK_EQ(seen.value("mc-version").value_or(""), "a1.2.6");
    CB_CHECK_EQ(seen.value("java_dir").value_or(""), "out");   // the single positional comes first: the tool decides what it is
    CB_CHECK(!seen.value("out_dir").has_value());
});

CB_TEST("java.run.choices_and_missing_tools", [] {
    run::Tools tools;
    int calls = 0;
    tools.handlers["scaffold"] = [&](const cli::Args&) {
        ++calls;
        return 0;
    };
    CB_CHECK_EQ(jrun({"scaffold", "a", "b", "--language", "rust"}, tools).code, 0);
    CB_CHECK_EQ(calls, 1);
    CB_CHECK_THROWS_AS(UsageError, jrun({"scaffold", "a", "b", "--language", "cobol"}, tools));   // refused before the handler
    CB_CHECK_EQ(calls, 1);
    CB_CHECK_THROWS_AS(UsageError, jrun({"apimap", "bogus"}));
    CB_CHECK_THROWS_AS(UsageError, jrun({"signatures", "dir", "--edition", "windows95"}, tools));
    // no handler provided and none built in
    CB_CHECK_THROWS_AS(UnsupportedError, jrun({"convert", "a", "b"}));
    CB_CHECK_THROWS_AS(UnsupportedError, jrun({"ui-mark", "dir"}));
    // misuse of the command line itself: a message on stderr, exit 2
    const Ran missing = jrun({"debug"});
    CB_CHECK_EQ(missing.code, 2);
    CB_CHECK(!missing.err.empty());
    const Ran none = jrun({});
    CB_CHECK_EQ(none.code, 2);
    CB_CHECK(none.err.find("bugfix-ledger") != std::string::npos);
    CB_CHECK(none.out.empty());
});

CB_TEST("java.run.debug_tool", [] {
    fs::ScopedTempDir tmp("jrun");
    const std::string markers = "  J2X-DEBUG marker tags present: 20 (assert, crash-hard, crash-impl, crash-soft, dbg, every, leak-free, "
                                "leak-free-impl, leak-impl, leak-n)\n";
    const Ran on = jrun({"debug", fs::to_text(tmp.path() / "dbg")});
    CB_CHECK_EQ(on.code, 0);
    CB_CHECK_EQ(on.out, "Emitted Debug.h, Debug.cpp, DEBUG.md (master toggle DEBUG_ENABLED=1; J2X-DEBUG markers).\n" + markers);
    CB_CHECK(fs::read_text(tmp.path() / "dbg" / "Debug.h").find("#define DEBUG_ENABLED 1") != std::string::npos);
    const Ran off = jrun({"debug", fs::to_text(tmp.path() / "dbg_off"), "--off"});
    CB_CHECK_EQ(off.out, "Emitted debug files with DEBUG_ENABLED=0 (opt-in off): Debug.h, Debug.cpp, DEBUG.md\n" + markers);
    CB_CHECK(fs::read_text(tmp.path() / "dbg_off" / "Debug.h").find("#define DEBUG_ENABLED 0") != std::string::npos);
});

CB_TEST("java.run.bugfix_ledger_tool", [] {
    Checkout checkout;
    fs::ScopedTempDir tmp("jrun");
    const std::string out = fs::to_text(tmp.path() / "ledger.md");
    const Ran r = jrun({"bugfix-ledger", "--repo", fs::to_text(testing::data_path("java/testdata/bugfix_tree")), "--out", out});
    CB_CHECK_EQ(r.code, 0);
    CB_CHECK_EQ(r.out, "bugfix-ledger: 12 markers / 12 distinct fixes -> " + out + "\n");
    CB_CHECK_EQ(fs::read_text(tmp.path() / "ledger.md"), fs::read_text(testing::data_path("java/testdata/bugfix_ledger.golden.md")));
});

CB_TEST("java.run.reflect_factories_tool", [] {
    fs::ScopedTempDir tmp("jrun");
    fs::copy_tree(testing::data_path("java/testdata/reflection/in"), tmp.path() / "tree");
    const std::string dir = fs::to_text(tmp.path() / "tree");
    const Ran first = jrun({"reflect-factories", dir});
    CB_CHECK_EQ(first.code, 0);
    CB_CHECK_EQ(first.out,
                "  EntityList.cpp         -> EntityList_createByReflection  (1 classes, 1 site)\n"
                "  World.cpp              -> reuses a shared factory  (1 site)\n"
                "Generated ReflectionFactory.h; 2 sites rewritten across 1 registries (1 classes).\n");
    CB_CHECK(stdfs::exists(tmp.path() / "tree" / "ReflectionFactory.h"));
    const Ran again = jrun({"reflect-factories", dir});   // already applied
    CB_CHECK_EQ(again.out, "No reflection sites to rewrite (already applied, or none present).\n");
});

CB_TEST("java.run.apimap_tool", [] {
    Checkout checkout;
    const Ran languages = jrun({"apimap", "languages"});
    CB_CHECK_EQ(languages.code, 0);
    CB_CHECK(languages.out.starts_with("  asm.x86      [language] x86-64 assembly (target for lowered hot paths).\n"
                                       "  c            [language] C (ISO).  dialects: c89, c99, c11, c17, c23\n"));
    CB_CHECK(languages.out.find("  java         [language] Java SE (source language of the Minecraft corpus).  dialects: java5, java6, "
                                "java7, java8, java11, java17, java21\n") != std::string::npos);
    CB_CHECK(languages.out.ends_with("  xbox360      [api] Xbox 360 XDK families (xboxkrnl/xam/xnet/d3d9/xact3/xinput2).\n"));

    const Ran subsystems = jrun({"apimap", "subsystems"});
    CB_CHECK(subsystems.out.starts_with("  csharp.bcl               (csharp)  C# / .NET base class library -> C++ and Java. The 'cpp' rows are demon\n"));
    CB_CHECK_EQ(std::count(subsystems.out.begin(), subsystems.out.end(), '\n'), 11);   // (the Python's twelfth is java.platform)

    const Ran list = jrun({"apimap", "list", "java.lang"});
    CB_CHECK_EQ(list.code, 0);
    CB_CHECK(list.out.starts_with("  Integer.parseInt             -> cpp=JavaString::parseInt\n"));
    CB_CHECK(list.out.ends_with("  null                         -> cpp=NULL\n"));
    CB_CHECK_EQ(std::count(list.out.begin(), list.out.end(), '\n'), 18);
    const Ran nope = jrun({"apimap", "list", "nope"});
    CB_CHECK_EQ(nope.code, 2);
    CB_CHECK_EQ(nope.err, "No symbols in subsystem 'nope'. Try: apimap subsystems\n");
    CB_CHECK_EQ(jrun({"apimap", "list"}).code, 2);

    // resolve: the dialect picks the version override
    CB_CHECK_EQ(jrun({"apimap", "resolve", "Math.sqrt", "--from", "java", "--to", "cpp"}).out,
                "Math.sqrt  (java -> cpp)\n  target : sqrt\n  kind   : direct\n  note   : <math.h> / <cmath>.\n");
    CB_CHECK_EQ(jrun({"apimap", "resolve", "Math.sqrt", "--from", "java", "--to", "cpp", "--dialect", "c++11"}).out,
                "Math.sqrt  (java -> cpp@c++11)\n  target : std::sqrt\n  kind   : direct\n  note   : C++11 <cmath> adds type-generic std::sqrt "
                "overloads.\n");
    CB_CHECK_EQ(jrun({"apimap", "resolve", "null", "--from", "java", "--to", "cpp", "--dialect", "c++11"}).out,
                "null  (java -> cpp@c++11)\n  target : nullptr\n  kind   : constant\n  note   : C++11 typed null pointer; also what "
                "std_transforms rewrites NULL to.\n");
    const Ran unmapped = jrun({"apimap", "resolve", "No.such", "--from", "java", "--to", "cpp"});
    CB_CHECK_EQ(unmapped.code, 1);
    CB_CHECK_EQ(unmapped.out, "UNMAPPED: No.such (java -> cpp)\n");
    CB_CHECK_EQ(jrun({"apimap", "resolve", "Math.sqrt"}).code, 2);   // --from and --to are needed

    // coverage writes APIMAP.md and counts
    fs::ScopedTempDir tmp("jrun");
    fs::write_text(tmp.path() / "src" / "A.java", "class A { double f(double x) { return Math.sqrt(x) + Math.nosuch(x); } }\n");
    const std::string out = fs::to_text(tmp.path() / "cov.md");
    const Ran cov = jrun({"apimap", "coverage", fs::to_text(tmp.path() / "src"), "--from", "java", "--to", "cpp", "--out", out});
    CB_CHECK_EQ(cov.code, 0);
    CB_CHECK(cov.out.starts_with("symbols=2  mapped=1  unsupported=0  unmapped=1  ->  "));
    CB_CHECK(fs::read_text(tmp.path() / "cov.md").find("Math.sqrt") != std::string::npos);
    const Ran nodir = jrun({"apimap", "coverage", fs::to_text(tmp.path() / "missing"), "--from", "java", "--to", "cpp"});
    CB_CHECK_EQ(nodir.code, 2);
    CB_CHECK(nodir.err.starts_with("ERROR: not a directory: "));
});
