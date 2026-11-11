// Tests of the .NET toolchain: discovery on a made-up installation (every version rule), then -- when the machine has
// the SDK -- real compiles and runs of small C# programs.
#include "cb/translate/dotnet/package.hpp"

#include <fstream>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace {

// std::string::contains is C++23; this answers for strings and for sets and maps alike
template <class Haystack, class Needle>
bool contains(const Haystack& haystack, const Needle& needle) {
    if constexpr (requires { haystack.find(needle) != Haystack::npos; })
        return haystack.find(needle) != Haystack::npos;
    else
        return haystack.contains(needle);
}
namespace dn = cb::translate::dotnet;
namespace stdfs = std::filesystem;

void touch(const stdfs::path& p, const std::string& text = "x") {
    stdfs::create_directories(p.parent_path());
    std::ofstream(p, std::ios::binary) << text;
}

// A made-up installation: SDKs 8.0.100 and 9.0.317 (Roslyn only in the newer), the targeting pack 9.0.0 with net9.0
// reference assemblies, the runtime 9.0.5 (and an 8.x one that is not it).
stdfs::path make_install(const stdfs::path& root) {
    touch(root / "dotnet.exe");
    touch(root / "sdk" / "8.0.100" / "dotnet.dll");
    touch(root / "sdk" / "9.0.317" / "Roslyn" / "bincore" / "csc.dll");
    touch(root / "packs" / "Microsoft.NETCore.App.Ref" / "9.0.0" / "ref" / "net9.0" / "System.Runtime.dll");
    touch(root / "packs" / "Microsoft.NETCore.App.Ref" / "9.0.0" / "ref" / "net9.0" / "System.Console.dll");
    touch(root / "packs" / "Microsoft.NETCore.App.Ref" / "9.0.0" / "ref" / "net9.0" / "notes.txt");
    stdfs::create_directories(root / "shared" / "Microsoft.NETCore.App" / "8.0.11");
    stdfs::create_directories(root / "shared" / "Microsoft.NETCore.App" / "9.0.5");
    stdfs::create_directories(root / "shared" / "Microsoft.NETCore.App" / "9.0.2");
    return root / "dotnet.exe";
}

// The SDK of this machine, if it has a usable one.
std::optional<dn::Sdk> machine_sdk() {
    auto sdk = dn::Sdk::find();
    if (sdk && sdk->available()) return sdk;
    return std::nullopt;
}
}  // namespace

CB_TEST("translate.dotnet.version_key", [] {
    using V = std::vector<std::uint64_t>;
    CB_CHECK(dn::version_key("8.0.100") == (V{8, 0, 100}));
    CB_CHECK(dn::version_key("9.0.0-preview.1") == (V{9, 0, 0}));
    CB_CHECK(dn::version_key("10") == (V{10}));
    CB_CHECK(dn::version_key("a.b.c") == (V{0, 0, 0}));
    CB_CHECK(dn::version_key("1.2.3.4") == (V{1, 2, 3}));   // only the first three parts
    CB_CHECK(dn::version_key("1..2") == (V{1, 0, 2}));
    CB_CHECK(dn::version_key("8.0.100-rc.1.2") == (V{8, 0, 100}));
    CB_CHECK(dn::version_key("10.0.1+build") == (V{10, 0, 0}));   // "1+build" is not a number
    CB_CHECK(dn::version_key("9.10.0") > dn::version_key("9.9.9"));   // numbers, not text
    CB_CHECK(dn::version_key("10") < dn::version_key("10.0.0"));      // a shorter key is the smaller
    CB_CHECK(dn::version_key("99999999999999999999999.0")[0] == UINT64_MAX);
});

CB_TEST("translate.dotnet.newest_version_dir", [] {
    cb::fs::ScopedTempDir dir("cb-dotnet");
    const auto v = dir.path() / "ver";
    for (const char* n : {"8.0.100", "9.0.0", "10.0.1", "9.0.0-preview.1", "abc", "7", "10.0.1-rc.1", "9.0.0-rc", "11",
                          "9.10.0", "9.9.9", "x9.0.0"})
        stdfs::create_directories(v / n);
    touch(v / "12.file");   // a file counts: the original listed whatever was there
    CB_CHECK_EQ(dn::newest_version_dir(v).value(), std::string("12.file"));
    CB_CHECK_EQ(dn::newest_version_dir(v, "9.").value(), std::string("9.10.0"));
    CB_CHECK_EQ(dn::newest_version_dir(v, "8.0").value(), std::string("8.0.100"));
    CB_CHECK_EQ(dn::newest_version_dir(v, "10").value(), std::string("10.0.1"));   // the first of equals stays
    CB_CHECK(!dn::newest_version_dir(v, "x"));            // names must start with a digit
    CB_CHECK(!dn::newest_version_dir(v, "net"));
    CB_CHECK(!dn::newest_version_dir(dir.path() / "nope"));
    CB_CHECK(!dn::newest_version_dir(v / "12.file"));      // not a folder
});

CB_TEST("translate.dotnet.discovery_on_an_installation", [] {
    cb::fs::ScopedTempDir dir("cb-dotnet");
    const auto sdk = dn::Sdk::at(make_install(dir.path() / "dotnet"));
    CB_CHECK_EQ(sdk.sdk_dir().value().filename().generic_string(), std::string("9.0.317"));
    CB_CHECK_EQ(sdk.roslyn_dir().value().filename().generic_string(), std::string("bincore"));
    CB_CHECK_EQ(sdk.reference_dir().value().filename().generic_string(), std::string("net9.0"));
    CB_CHECK_EQ(sdk.runtime().value().major, std::string("9"));
    CB_CHECK_EQ(sdk.runtime().value().version, std::string("9.0.5"));   // the newest installed of the major
    CB_CHECK(sdk.available());
    CB_CHECK(cb::fs::same_path(sdk.home(), dir.path() / "dotnet"));
});

CB_TEST("translate.dotnet.discovery_gaps", [] {
    cb::fs::ScopedTempDir dir("cb-dotnet");
    // only a dotnet.exe: nothing to compile with
    touch(dir.path() / "bare" / "dotnet.exe");
    const auto bare = dn::Sdk::at(dir.path() / "bare" / "dotnet.exe");
    CB_CHECK(!bare.sdk_dir() && !bare.roslyn_dir() && !bare.reference_dir() && !bare.runtime());
    CB_CHECK(!bare.available());
    CB_CHECK_THROWS_AS(cb::ToolError, (void)bare.compile({.sources = {"a.cs"}, .out_dll = dir.path() / "a.dll"}));

    // a targeting pack with no runtime installed: the runtime is the pack's own version
    touch(dir.path() / "norun" / "dotnet.exe");
    touch(dir.path() / "norun" / "sdk" / "8.0.100" / "Roslyn" / "bincore" / "csc.dll");
    touch(dir.path() / "norun" / "packs" / "Microsoft.NETCore.App.Ref" / "8.0.0" / "ref" / "net8.0" / "System.Runtime.dll");
    const auto norun = dn::Sdk::at(dir.path() / "norun" / "dotnet.exe");
    CB_CHECK_EQ(norun.runtime().value().version, std::string("8.0.0"));

    // an SDK folder without Roslyn is not a compiler; several target frameworks: the newest
    touch(dir.path() / "noroslyn" / "dotnet.exe");
    touch(dir.path() / "noroslyn" / "sdk" / "9.0.100" / "dotnet.dll");
    for (const char* tfm : {"net8.0", "net10.0", "netstandard2.1", "net9.0"})
        touch(dir.path() / "noroslyn" / "packs" / "Microsoft.NETCore.App.Ref" / "10.0.0" / "ref" / tfm / "x.dll");
    const auto noroslyn = dn::Sdk::at(dir.path() / "noroslyn" / "dotnet.exe");
    CB_CHECK(!noroslyn.roslyn_dir());
    CB_CHECK_EQ(noroslyn.reference_dir().value().filename().generic_string(), std::string("net10.0"));
    CB_CHECK(!noroslyn.available());
});

CB_TEST("translate.dotnet.find_uses_cb_dotnet", [] {
    cb::fs::ScopedTempDir dir("cb-dotnet");
    const auto exe = make_install(dir.path() / "dotnet");
    {
        cb::fs::ScopedEnv env("CB_DOTNET", cb::fs::to_text(exe));
        const auto sdk = dn::Sdk::find();
        CB_CHECK(sdk.has_value());
        CB_CHECK(cb::fs::same_path(sdk->exe(), exe));
        CB_CHECK(sdk->available());
    }
    // a CB_DOTNET that is not a file is ignored: discovery falls on to PATH and the standard folder
    {
        cb::fs::ScopedEnv env("CB_DOTNET", cb::fs::to_text(dir.path() / "missing" / "dotnet.exe"));
        const auto sdk = dn::Sdk::find();
        CB_CHECK(!sdk || !cb::fs::same_path(sdk->exe(), dir.path() / "missing" / "dotnet.exe"));
    }
    {
        cb::fs::ScopedEnv env("CB_DOTNET", cb::fs::to_text(dir.path()));   // a folder
        const auto sdk = dn::Sdk::find();
        CB_CHECK(!sdk || !cb::fs::same_path(sdk->exe(), dir.path()));
    }
    // found on PATH
    {
        cb::fs::ScopedEnv unset("CB_DOTNET", std::nullopt);
        cb::fs::ScopedEnv path("PATH", cb::fs::to_text(dir.path() / "nothing") + ";" + cb::fs::to_text(dir.path() / "dotnet"));
        const auto sdk = dn::Sdk::find();
        CB_CHECK(sdk.has_value());
        CB_CHECK(cb::fs::same_path(sdk->exe(), dir.path() / "dotnet" / "dotnet.exe"));
    }
});

CB_TEST("translate.dotnet.runtimeconfig", [] {
    cb::fs::ScopedTempDir dir("cb-dotnet");
    const auto sdk = dn::Sdk::at(make_install(dir.path() / "dotnet"));
    stdfs::create_directories(dir.path() / "out");
    const auto written = sdk.write_runtimeconfig(dir.path() / "out" / "a.dll");
    CB_CHECK_EQ(written.filename().generic_string(), std::string("a.runtimeconfig.json"));
    CB_CHECK_EQ(cb::fs::read_text(written),
                std::string("{\"runtimeOptions\": {\"tfm\": \"net9.0\", \"framework\": {\"name\": \"Microsoft.NETCore.App\", "
                            "\"version\": \"9.0.5\"}, \"rollForward\": \"LatestPatch\"}}\n"));
    // (the compiler made the folder; a runtimeconfig for a program of a folder that is gone makes it again)
    CB_CHECK(stdfs::is_regular_file(sdk.write_runtimeconfig(dir.path() / "nodir" / "b.dll")));
    // with no runtime there is nothing to name
    touch(dir.path() / "bare" / "dotnet.exe");
    CB_CHECK_THROWS_AS(cb::ToolError, (void)dn::Sdk::at(dir.path() / "bare" / "dotnet.exe").write_runtimeconfig(dir.path() / "c.dll"));
});

// ---- the machine's own SDK -------------------------------------------------------------------------------------

namespace {
// Compile one source text into <dir>/<name>.dll; the source is kept at <dir>/<name>.cs.
dn::Compiled build(const dn::Sdk& sdk, const stdfs::path& dir, const std::string& name, const std::string& source,
                   dn::CompileOptions options = {}) {
    touch(dir / (name + ".cs"), source);
    options.sources.push_back(dir / (name + ".cs"));
    options.out_dll = dir / (name + ".dll");
    return sdk.compile(options);
}
}  // namespace

CB_TEST("translate.dotnet.real.compile_and_run", [] {
    const auto sdk = machine_sdk();
    if (!sdk) return;   // no .NET SDK on this machine
    cb::fs::ScopedTempDir dir("cb-dotnet");
    const auto c = build(*sdk, dir.path(), "hello",
                         "using System;\nclass P { static int Main(string[] a) { Console.WriteLine(\"hello \" + string.Join(\",\", a));"
                         " string s = Console.In.ReadToEnd(); Console.WriteLine(\"stdin=\" + s.Length);"
                         " Console.Error.WriteLine(\"to stderr\"); return a.Length; } }\n");
    CB_CHECK(c.ok);
    CB_CHECK(stdfs::is_regular_file(dir.path() / "hello.runtimeconfig.json"));
    CB_CHECK(!stdfs::exists(dir.path() / "hello.rsp"));   // the response file does not stay behind
    dn::RunOptions run;
    run.args = {"x", "y z"};
    run.stdin_data = {'a', 'b', 'c'};
    const auto r = sdk->run(dir.path() / "hello.dll", run);
    CB_CHECK_EQ(r.exit_code.value(), std::uint32_t{2});
    CB_CHECK(contains(r.out, "hello x,y z"));
    CB_CHECK(contains(r.out, "stdin=3"));
    CB_CHECK(contains(r.err, "to stderr"));
});

CB_TEST("translate.dotnet.real.errors_options_and_timeout", [] {
    const auto sdk = machine_sdk();
    if (!sdk) return;
    cb::fs::ScopedTempDir dir("cb-dotnet");
    // a program that does not compile: no output, the compiler's complaints in the log
    const auto bad = build(*sdk, dir.path(), "bad", "class P { static void Main() { int x = \"no\"; undefined(); } }\n");
    CB_CHECK(!bad.ok);
    CB_CHECK(contains(bad.log, "error CS"));
    CB_CHECK(!stdfs::exists(dir.path() / "bad.dll"));
    // a library has no runtimeconfig; a nullable warning only shows with `warn`
    dn::CompileOptions lib;
    lib.target = dn::Target::Library;
    CB_CHECK(build(*sdk, dir.path(), "lib", "public class Lib { public static int Twice(int x) { return x * 2; } }\n", lib).ok);
    CB_CHECK(!stdfs::exists(dir.path() / "lib.runtimeconfig.json"));
    const std::string nullable = "#nullable enable\nusing System;\nclass P { static void Main() { string? s = null; Console.WriteLine(s.Length); } }\n";
    CB_CHECK(!contains(build(*sdk, dir.path(), "quiet", nullable).log, "warning"));
    dn::CompileOptions loud;
    loud.warn = true;
    CB_CHECK(contains(build(*sdk, dir.path(), "loud", nullable, loud).log, "CS8602"));
    // a reference, and the class that holds Main when two do
    dn::CompileOptions uses;
    uses.references = {dir.path() / "lib.dll"};
    CB_CHECK(build(*sdk, dir.path(), "uses", "class P { static void Main() { System.Console.WriteLine(Lib.Twice(21)); } }\n", uses).ok);
    CB_CHECK_EQ(sdk->run(dir.path() / "uses.dll").out, std::string("42\r\n"));
    const std::string two = "class A { static void Main() { System.Console.WriteLine(\"A\"); } }\n"
                            "class B { static void Main() { System.Console.WriteLine(\"B\"); } }\n";
    CB_CHECK(!build(*sdk, dir.path(), "two", two).ok);   // two entry points
    dn::CompileOptions pick;
    pick.main = "B";
    CB_CHECK(build(*sdk, dir.path(), "two", two, pick).ok);
    CB_CHECK_EQ(sdk->run(dir.path() / "two.dll").out, std::string("B\r\n"));
    // an exit code, UTF-8 output, a path with a space, and a program that outlives its timeout
    CB_CHECK(build(*sdk, dir.path() / "sp ace", "mixed",
                   "class P { static int Main(string[] a) { System.Console.OutputEncoding = System.Text.Encoding.UTF8; "
                   "System.Console.WriteLine(\"h\\u00e9llo \\u4e16\"); if (a.Length > 0) System.Threading.Thread.Sleep(30000); return 3; } }\n")
              .ok);
    const auto mixed = sdk->run(dir.path() / "sp ace" / "mixed.dll");
    CB_CHECK_EQ(mixed.exit_code.value(), std::uint32_t{3});
    CB_CHECK_EQ(mixed.out, std::string("h\xC3\xA9llo \xE4\xB8\x96\r\n"));
    dn::RunOptions quick;
    quick.args = {"sleep"};
    quick.timeout = std::chrono::seconds(1);
    CB_CHECK(!sdk->run(dir.path() / "sp ace" / "mixed.dll", quick).exit_code);
});
