// Tests of cb::xna::run: an XNA game as a Converted project. The assemblies are minimal PE files with a CLR header (what
// pe::is_dotnet_file asks), the decompiler and de4dot are small batch files that stand in for the real programs (ILSpy and de4dot
// are not part of the tests), and the .xnb converters are `Hooks` that answer a fixed record. What the real tools do is theirs; what
// is tested is what run does with them: which files it hands them, what it makes of what they leave, and the project it records.
//
// Hand-checked: the project's folders (original/, assets/game, assets/content, src/<assembly>, reports/), the
// decompile.json and content.json keys and order, the note line project.json carries, the `xna:` line of a failure and exit code 1.
//
// Behaviour pinned here:
//   * the reports are written with two-space indent;
//   * a failed tool is a ToolError; a missing ILSpy is a NotFoundError;
//   * a package with nothing in it to read is a UsageError.
#include "cb/xna/run.hpp"

#include <fstream>

#include <nlohmann/json.hpp>

#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"

namespace {

namespace run = cb::xna::run;
namespace stdfs = std::filesystem;
namespace converted = cb::base::converted;

// A PE32 file with `directories` data directories, the CLR one (index 14) set when `dotnet`.
cb::Bytes jrt_pe(bool dotnet) {
    cb::Bytes b(0x40 + 24 + 112 + 16 * 8, 0);
    b[0] = 'M';
    b[1] = 'Z';
    b[0x3C] = 0x40;
    const std::size_t nt = 0x40;
    b[nt] = 'P';
    b[nt + 1] = 'E';
    b[nt + 24] = 0x0B;   // PE32
    b[nt + 25] = 0x01;
    b[nt + 24 + 92] = 16;   // NumberOfRvaAndSizes
    if (dotnet) {
        const std::size_t clr = nt + 24 + 96 + 8 * 14;
        b[clr + 1] = 0x20;   // RVA 0x2000
        b[clr + 4] = 0x48;   // size 0x48
    }
    return b;
}

void jrt_write(const stdfs::path& p, const std::string& text) {
    stdfs::create_directories(p.parent_path());
    cb::fs::write_text(p, text);
}

void jrt_write_bytes(const stdfs::path& p, const cb::Bytes& b) {
    stdfs::create_directories(p.parent_path());
    cb::fs::write_file(p, b);
}

// A game on disk: Game.exe (.NET), Lib.dll (.NET), the framework's and a resource assembly (not the game's), a native dll.
void jrt_make_game(const stdfs::path& dir) {
    jrt_write_bytes(dir / "Game.exe", jrt_pe(true));
    jrt_write_bytes(dir / "Lib.dll", jrt_pe(true));
    jrt_write_bytes(dir / "System.Core.dll", jrt_pe(true));
    jrt_write_bytes(dir / "Microsoft.Xna.Framework.dll", jrt_pe(true));
    jrt_write_bytes(dir / "mscorlib.dll", jrt_pe(true));
    jrt_write_bytes(dir / "fr" / "Game.resources.dll", jrt_pe(true));
    jrt_write_bytes(dir / "native.dll", jrt_pe(false));
    jrt_write(dir / "Content" / "hero.xnb", "xnb");
    jrt_write(dir / "Content" / "Effects" / "blur.xnb", "xnb");
    jrt_write(dir / "Content" / "Shader.fxo", "fx");
    jrt_write(dir / "readme.txt", "text");
}

std::string jrt_names(const std::vector<stdfs::path>& paths, const stdfs::path& base) {
    std::string out;
    for (const stdfs::path& p : paths) out += (out.empty() ? "" : ",") + p.lexically_relative(base).generic_string();
    return out;
}

// Stand-ins for the programs, written beside a Sample.cs the fake decompiler "decompiles" to.
struct JrtTools {
    cb::fs::ScopedTempDir dir{"cbxnarun"};
    run::Tools tools;
    explicit JrtTools(bool with_de4dot) {
        jrt_write(dir.path() / "Sample.cs", "class A : B {\n    public A(int x)\n    {\n        base._002Ector(x);\n    }\n}\n");
        jrt_write(dir.path() / "fake_ilspy.cmd",
                  "@echo off\r\nmkdir \"%~5\" 2>nul\r\ncopy /y \"%~dp0Sample.cs\" \"%~5\\Game.cs\" >nul\r\nexit /b 0\r\n");
        tools.ilspycmd = dir.path() / "fake_ilspy.cmd";
        if (with_de4dot) {
            jrt_write(dir.path() / "fake_de4dot.cmd",
                      "@echo off\r\nif \"%~1\"==\"-d\" goto detect\r\ncopy /y \"%~1\" \"%~3\" >nul\r\nexit /b 0\r\n:detect\r\necho Detected Fake Obfuscator (1.0)\r\nexit /b 0\r\n");
            tools.de4dot = dir.path() / "fake_de4dot.cmd";
        }
        tools.checkout_root = dir.path() / "checkout";
        tools.state = dir.path() / "state";
        tools.home = dir.path() / "home";
    }
};

run::Hooks jrt_hooks() {
    run::Hooks h;
    h.convert_xnb = [](const stdfs::path& file, const stdfs::path&) { return std::string(R"({"kind": "texture", "name": ")") + file.stem().string() + "\"}"; };
    h.convert_effect_file = [](const stdfs::path& file, const stdfs::path&) -> std::optional<std::string> {
        if (file.extension() == ".fxo") return std::string(R"({"kind": "effect", "size": 2})");
        return std::nullopt;
    };
    return h;
}

}  // namespace

CB_TEST("xna.run.framework_names", [] {
    const auto p = run::framework_prefixes();
    CB_CHECK_EQ(p.size(), std::size_t{4});
    CB_CHECK_EQ(p[0], std::string_view("microsoft."));
    CB_CHECK_EQ(p[2], std::string_view("mscorlib"));
});

CB_TEST("xna.run.assemblies_are_the_games_own_the_exe_first", [] {
    cb::fs::ScopedTempDir dir("cbxnarun");
    jrt_make_game(dir.path());
    // the framework's, a satellite resource assembly and a native dll are not the game's; the .exe comes first
    CB_CHECK_EQ(jrt_names(run::assemblies(dir.path()), dir.path()), "Game.exe,Lib.dll");
    // nothing .NET: nothing
    cb::fs::ScopedTempDir empty("cbxnarun");
    jrt_write_bytes(empty.path() / "native.exe", jrt_pe(false));
    jrt_write(empty.path() / "notes.dll", "not a PE at all");
    CB_CHECK(run::assemblies(empty.path()).empty());
});

CB_TEST("xna.run.the_tools_are_found", [] {
    JrtTools t(true);
    CB_CHECK(run::find_ilspycmd(t.tools) == *t.tools.ilspycmd);   // the setting
    CB_CHECK(*run::find_de4dot(t.tools) == *t.tools.de4dot);
    // the setting names a file that is not there: the lookup goes on, and with nothing found says so
    run::Tools none;
    none.ilspycmd = t.dir.path() / "missing.exe";
    none.checkout_root = t.dir.path() / "checkout";
    none.home = t.dir.path() / "home";
    const cb::fs::ScopedEnv no_path("PATH", "");
    CB_CHECK_THROWS_AS(cb::NotFoundError, (void)run::find_ilspycmd(none));
    CB_CHECK(!run::find_de4dot(none).has_value());
    // ILSpy installed as a dotnet global tool is found in the home folder; de4dot's build beside the checkout
    jrt_write(t.dir.path() / "home" / ".dotnet" / "tools" / "ilspycmd.exe", "x");
    CB_CHECK(run::find_ilspycmd(none) == t.dir.path() / "home" / ".dotnet" / "tools" / "ilspycmd.exe");
    jrt_write(t.dir.path() / "_tools" / "de4dot" / "Release" / "net8.0" / "de4dot.exe", "x");
    CB_CHECK(*run::find_de4dot(none) == t.dir.path() / "_tools" / "de4dot" / "Release" / "net8.0" / "de4dot.exe");
});

CB_TEST("xna.run.deobfuscation", [] {
    JrtTools t(true);
    const stdfs::path asm_path = t.dir.path() / "Game.exe";
    jrt_write_bytes(asm_path, jrt_pe(true));
    CB_CHECK_EQ(run::obfuscator(asm_path, t.tools).value(), "Fake Obfuscator");
    // the cleaned copy is kept under the state folder, named by the contents' digest
    const run::Cleaned c = run::deobfuscated(asm_path, t.tools);
    CB_CHECK_EQ(c.obfuscator.value(), "Fake Obfuscator");
    CB_CHECK(c.path.string().find("deobfuscated") != std::string::npos);
    CB_CHECK(stdfs::is_regular_file(c.path));
    CB_CHECK_EQ(c.path.extension().string(), ".exe");
    CB_CHECK(c.path.stem().string().starts_with("Game-"));
    // without de4dot nothing is obfuscated: the assembly as it is
    JrtTools plain(false);
    CB_CHECK(!run::obfuscator(asm_path, plain.tools).has_value());
    const run::Cleaned same = run::deobfuscated(asm_path, plain.tools);
    CB_CHECK(same.path == asm_path);
    CB_CHECK(!same.obfuscator.has_value());
});

CB_TEST("xna.run.decompile_runs_ilspy_and_reports_its_failure", [] {
    JrtTools t(false);
    const stdfs::path out = t.dir.path() / "out";
    run::decompile(t.dir.path() / "Game.exe", out, t.tools);
    CB_CHECK(stdfs::is_regular_file(out / "Game.cs"));
    // a decompiler that fails: ToolError with its exit code
    jrt_write(t.dir.path() / "failing.cmd", "@echo off\r\necho boom 1>&2\r\nexit /b 3\r\n");
    t.tools.ilspycmd = t.dir.path() / "failing.cmd";
    try {
        run::decompile(t.dir.path() / "Game.exe", t.dir.path() / "out2", t.tools);
        CB_CHECK(false);
    } catch (const cb::ToolError& e) {
        CB_CHECK_EQ(e.tool_exit_code(), std::uint32_t{3});
        CB_CHECK(e.output().find("boom") != std::string::npos);
    }
});

CB_TEST("xna.run.convert_a_game_folder", [] {
    JrtTools t(true);
    cb::fs::ScopedTempDir game("cbxnagame");
    jrt_make_game(game.path());
    run::Environment env{.tools = t.tools, .store = converted::Store(t.dir.path() / "Converted")};
    const run::Converted done = run::convert(game.path(), env, {.name = "block-world"}, jrt_hooks());
    const stdfs::path root = done.project.root();
    CB_CHECK_EQ(root.filename().string(), "block-world");
    // the game's files as it ships them, and one project per assembly of its own (the fix-ups ran over the C#)
    CB_CHECK(stdfs::is_regular_file(done.project.assets() / "game" / "Game.exe"));
    CB_CHECK(stdfs::is_regular_file(done.project.assets() / "game" / "Content" / "hero.xnb"));
    CB_CHECK_EQ(done.report.assemblies.size(), std::size_t{2});
    CB_CHECK_EQ(done.report.assemblies[0].project, "Game");
    CB_CHECK_EQ(done.report.assemblies[0].assembly, "Game.exe");
    CB_CHECK_EQ(done.report.assemblies[0].deobfuscated.value(), "Fake Obfuscator");   // de4dot found one
    CB_CHECK_EQ(done.report.assemblies[0].constructors, std::size_t{2});   // the base call counts twice (fixups)
    CB_CHECK_EQ(done.report.assemblies[1].project, "Lib");
    const std::string cs = cb::fs::read_text(done.project.src() / "Game" / "Game.cs");
    CB_CHECK(cs.find("public A(int x) : base(x)") != std::string::npos);
    // the reports
    const cb::Json decompile = cb::json::load(done.project.reports() / "decompile.json");
    CB_CHECK_EQ(decompile.at("assemblies").size(), std::size_t{2});
    CB_CHECK_EQ(decompile.at("assemblies")[0].at("assembly").get<std::string>(), "Game.exe");
    CB_CHECK_EQ(decompile.at("assemblies")[0].at("constructors").get<int>(), 2);
    CB_CHECK_EQ(decompile.at("assemblies")[0].at("ref casts").get<int>(), 0);
    CB_CHECK(decompile.at("assemblies")[0].at("left").is_array());
    const cb::Json content = cb::json::load(done.project.reports() / "content.json");
    // every .xnb through the hook, a non-.xnb file only when it is an effect; each record says its folder (in walk order)
    CB_CHECK_EQ(content.size(), std::size_t{3});
    CB_CHECK_EQ(content[0].at("folder").get<std::string>(), "Content");   // a folder's own files first, in name order: hero, Shader
    CB_CHECK_EQ(content[0].at("kind").get<std::string>(), "texture");
    CB_CHECK_EQ(content[1].at("kind").get<std::string>(), "effect");
    CB_CHECK_EQ(content[2].at("folder").get<std::string>(), "Content/Effects");   // then the folders under it
    CB_CHECK_EQ(done.kinds.at("texture"), std::size_t{2});
    CB_CHECK_EQ(done.kinds.at("effect"), std::size_t{1});
    // the project's record
    const converted::Manifest m = done.project.manifest();
    CB_CHECK_EQ(m.steps.size(), std::size_t{1});
    CB_CHECK_EQ(m.steps[0].command, "xna");
    CB_CHECK(m.steps[0].notes.find("C# of Game, Lib (ILSpy; 0 ref casts and 4 constructor calls rewritten, 0 left as they were: reports/decompile.json); content: 1 effect, 2 texture") !=
             std::string::npos);
});

CB_TEST("xna.run.without_content_and_run_again", [] {
    JrtTools t(false);
    cb::fs::ScopedTempDir game("cbxnagame");
    jrt_make_game(game.path());
    run::Environment env{.tools = t.tools, .store = converted::Store(t.dir.path() / "Converted")};
    const run::Converted done = run::convert(game.path() / "Game.exe", env, {.content = false}, {});   // the .exe: its folder is the game
    CB_CHECK(done.kinds.empty());
    CB_CHECK(!stdfs::exists(done.project.reports() / "content.json"));
    CB_CHECK(stdfs::is_regular_file(done.project.assets() / "game" / "Content" / "hero.xnb"));
    CB_CHECK(done.report.assemblies[0].constructors == 2);
    CB_CHECK(done.project.manifest().steps[0].notes.find("content: not converted") != std::string::npos);
    // the C# an earlier run decompiled is not decompiled again (an edit to it stays), and the fix-ups find nothing left to do
    cb::fs::write_text(done.project.src() / "Game" / "Game.cs", "// edited\n");
    const run::Converted again = run::convert(game.path() / "Game.exe", env, {.content = false}, {});
    CB_CHECK_EQ(cb::fs::read_text(again.project.src() / "Game" / "Game.cs"), "// edited\n");
    CB_CHECK(again.project.root() == done.project.root());
    CB_CHECK_EQ(again.project.manifest().steps.size(), std::size_t{1});   // the same command replaces its step
});

CB_TEST("xna.run.what_cannot_be_converted_is_said", [] {
    JrtTools t(false);
    run::Environment env{.tools = t.tools, .store = converted::Store(t.dir.path() / "Converted")};
    // a folder with no .NET assembly in it
    cb::fs::ScopedTempDir nothing("cbxnagame");
    jrt_write(nothing.path() / "readme.txt", "x");
    CB_CHECK_THROWS_AS(cb::UsageError, (void)run::convert(nothing.path(), env, {}, {}));
    // content wanted, converters not given
    cb::fs::ScopedTempDir game("cbxnagame");
    jrt_make_game(game.path());
    CB_CHECK_THROWS_AS(cb::UnsupportedError, (void)run::convert(game.path(), env, {}, {}));
});

CB_TEST("xna.run.main_prints_the_report", [] {
    JrtTools t(false);
    cb::fs::ScopedTempDir game("cbxnagame");
    jrt_make_game(game.path());
    run::Environment env{.tools = t.tools, .store = converted::Store(t.dir.path() / "Converted")};
    std::string text;
    int code = -1;
    {
        cb::testing::CapturedStd cap;
        const std::vector<std::string> args = {game.path().string(), "--name", "demo"};
        code = run::main(args, env, jrt_hooks());
        text = cap.out();
    }
    CB_CHECK_EQ(code, 0);
    CB_CHECK(text.find("  Game                         C#: 0 ref casts, 2 constructors rewritten, 0 left\n") != std::string::npos);
    CB_CHECK(text.find("  content: 1 effect, 2 texture\n") != std::string::npos);
    CB_CHECK(text.find("-> ") != std::string::npos);
    // no arguments: the usage, a failure; -h: the usage; port: handed on
    {
        cb::testing::CapturedStd cap;
        CB_CHECK_EQ(run::main({}, env, {}), 2);
        CB_CHECK(cap.out().find("codebreaker xna <package") != std::string::npos);
    }
    {
        cb::testing::CapturedStd cap;
        const std::vector<std::string> args = {"-h"};
        CB_CHECK_EQ(run::main(args, env, {}), 0);
    }
    std::vector<std::string> seen;
    const std::vector<std::string> port_args = {"port", "demo", "--x"};
    CB_CHECK_EQ(run::main(port_args, env, {}, [&](std::span<const std::string> a) { seen.assign(a.begin(), a.end()); return 7; }), 7);
    CB_CHECK_EQ(seen.size(), std::size_t{2});
    CB_CHECK_EQ(seen[0], "demo");
    // a failure: `xna: <why>` and exit code 1
    cb::fs::ScopedTempDir nothing("cbxnagame");
    {
        cb::testing::CapturedStd cap;
        const std::vector<std::string> args = {nothing.path().string()};
        CB_CHECK_EQ(run::main(args, env, {}), 1);
        CB_CHECK(cap.out().find("xna: ") != std::string::npos);
        CB_CHECK(cap.out().find("no .NET assembly in it") != std::string::npos);
    }
});
