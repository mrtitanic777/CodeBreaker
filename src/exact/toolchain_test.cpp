#include "exact/toolchain.hpp"

#include <atomic>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"

namespace {

using namespace cb;
using namespace cb::exact::toolchain;
namespace stdfs = std::filesystem;
namespace emu = cb::exact::emu;
namespace modules = cb::exact::modules;

const stdfs::path& real_profiles() {
    static const stdfs::path p = testing::data_path("../config/exact/toolchains");
    return p;
}

constexpr const char* kFake = R"JSON({
  "name": "fake", "kit": "k1",
  "stage": {"files": {"TOOL.EXE": "TOOL.EXE"}, "include": "inc"},
  "psp": "0x100", "top": "0x7000",
  "env": ["PATH=C:\\"], "env_unless": {"NO87= ": "-FPi"},
  "switches": "-O", "file_switches": "MFLAGS",
  "derive": {"fp": {"regex": "-FP(\\w+)", "then": "-FP{1}", "else": "-FPc"}},
  "passes": [{"run": "TOOL.EXE", "args": "{tag} {fp}", "listing": "-Fl", "env": ["WORK={tag6}"]}],
  "output": "{tag}.OBJ",
  "patches": [{"change": 1, "file": "TOOL.EXE", "at": "0x2", "was": "4344", "now": "4546"},
              {"change": "mem", "file": "TOOL.EXE", "at": 16, "was": "00", "now": "01", "in_memory": true}],
  "variants": {"plain": [], "patched": [1], "hooked": {"patches": [1, "mem"], "hooks": ["jump"]}},
  "default_variant": "patched",
  "hooks": {"ask": {"pass": "TOOL.EXE", "at": ["0x10", "0x20"], "set": {"AX": "7"}, "header": "ASKS"},
            "jump": {"pass": "TOOL.EXE", "at": [5], "call": "test:jump"}},
  "try_switches": ["-Gs"], "segment_switch": "-Gt", "title": "a fake compiler"
})JSON";

// What the fake pass does: reads the source it was given and writes "OBJ:" + the source as the object (or fails when the
// source says FAIL); records how it was run.
class FakeRunner final : public PassRunner {
public:
    emu::dos::Result run(const PassRun& pass) const override {
        runs.push_back(pass);
        ++calls;
        emu::dos::Result r;
        const std::string tag = std::string(text::split_ws(pass.command_tail).front());
        const stdfs::path src = pass.workdir / (tag + ".C");
        const Bytes source = pass.files ? pass.files->read(src) : fs::read_file(src);
        const std::string text(as_text(source));
        r.console = to_bytes("compiling " + tag + "\r\n");
        if (text.find("FAIL") != std::string::npos) {
            r.exit = 3;
            return r;
        }
        r.exit = 0;
        const Bytes obj = to_bytes("OBJ:" + text);
        if (pass.files) pass.files->put(pass.workdir / (tag + ".OBJ"), obj);
        else fs::write_file(pass.workdir / (tag + ".OBJ"), obj);
        if (text.find("listing") != std::string::npos) {
            const Bytes lst = to_bytes("line 1\r\nline 2\r\n");
            if (pass.files) pass.files->put(pass.workdir / (tag + ".LST"), lst);
            else fs::write_file(pass.workdir / (tag + ".LST"), lst);
        }
        return r;
    }
    mutable std::vector<PassRun> runs;
    mutable std::atomic<int> calls{0};
};

struct Fixture {
    fs::ScopedTempDir dir{"toolchain"};
    fs::ScopedEnv no_kit{"CB_KIT_K1", std::nullopt};
    fs::ScopedEnv no_tag{"CB_STAGE_TAG", std::nullopt};
    Locations locations;
    FakeRunner runner;

    Fixture() {
        locations.profiles = dir.path() / "profiles";
        locations.state = dir.path() / "state";
        locations.checkout_root = dir.path();
        locations.kits["k1"] = "kits/k1";   // relative: taken against the checkout
        fs::make_dirs(locations.profiles);
        fs::write_text(locations.profiles / "fake.json", kFake);
        Bytes exe(32, 0);
        exe[0] = 'M';
        exe[1] = 'Z';
        exe[8] = 2;   // two paragraphs of header: the load image begins at 32
        std::string body = "ABCDEFGH";
        body.resize(32, '\0');   // (the in-memory patch's place, offset 16, is in the image)
        exe.insert(exe.end(), body.begin(), body.end());
        fs::make_dirs(dir.path() / "kits" / "k1" / "inc" / "SYS");
        fs::write_file(dir.path() / "kits" / "k1" / "TOOL.EXE", exe);
        fs::write_text(dir.path() / "kits" / "k1" / "inc" / "stdio.h", "int printf();\n");
        fs::write_text(dir.path() / "kits" / "k1" / "inc" / "SYS" / "types.h", "typedef int t;\n");
    }
    Toolchain make(const std::string& variant = "", const Context* with = nullptr) {
        Context c = with ? *with : Context{};
        c.locations = &locations;
        c.runner = &runner;
        return Toolchain(read_profile(locations, "fake"), variant.empty() ? std::nullopt : std::optional(variant), c);
    }
};

}  // namespace

CB_TEST("exact.toolchain.the_profiles_of_the_checkout_are_read", [] {
    Locations real;
    real.profiles = real_profiles();
    const auto names = real.profile_names();
    CB_CHECK(std::is_sorted(names.begin(), names.end()));
    CB_CHECK(std::find(names.begin(), names.end(), "msc300") != names.end());
    CB_CHECK(std::find(names.begin(), names.end(), "masm100") != names.end());
    for (const std::string& n : names) CB_CHECK_EQ(read_profile(real, n).name, n);   // every profile reads, and names itself

    const Profile msc = read_profile(real, "msc300");
    CB_CHECK_EQ(msc.kit.value(), "msc300");
    CB_CHECK_EQ(msc.psp.value(), 0x100);
    CB_CHECK_EQ(msc.switches.value(), "-Oat");
    CB_CHECK_EQ(msc.file_switches.value(), "MFLAGS");
    CB_CHECK_EQ(msc.passes.size(), 4u);
    CB_CHECK_EQ(msc.stage.files.size(), 5u);
    CB_CHECK(msc.derive.contains("fp"));
    CB_CHECK_EQ(msc.derive.at("fp").then, "-FP{1}");
    CB_CHECK_EQ(msc.strings.at("probe_switch"), "-Gs");   // a string entry other modules ask for by key
    CB_CHECK(!msc.try_switches.empty());
    CB_CHECK_EQ(msc.digest.size(), 40u);

    const Profile masm = read_profile(real, "masm100");
    CB_CHECK_EQ(masm.top.value(), 0x7000);
    CB_CHECK(masm.source_eof);
    CB_CHECK_EQ(masm.source_ext, ".ASM");
    CB_CHECK_EQ(masm.passes[0].args, "{tag},{tag},NUL,NUL;");
    CB_CHECK_THROWS_AS(NotFoundError, (void)read_profile(real, "nothing_of_that_name"));
});

CB_TEST("exact.toolchain.a_profile_that_is_not_one_is_refused", [] {
    CB_CHECK_THROWS_AS(ParseError, (void)parse_profile("{not json"));
    CB_CHECK_THROWS_AS(FormatError, (void)parse_profile("[1, 2]"));
    CB_CHECK_THROWS_AS(FormatError, (void)parse_profile(R"({"kit": "x"})"));   // no name
    CB_CHECK_THROWS_AS(FormatError, (void)parse_profile(R"({"name": "x", "passes": [{"args": "a"}]})"));
    CB_CHECK_THROWS_AS(FormatError, (void)parse_profile(R"({"name": "x", "patches": [{"change": 1, "file": "F", "at": 0, "was": "zz", "now": "00"}]})"));
    CB_CHECK_THROWS_AS(FormatError, (void)parse_profile(R"({"name": "x", "hooks": {"h": {"at": [1]}}})"));
    const Profile minimal = parse_profile(R"({"name": "x"})");
    CB_CHECK_EQ(minimal.psp.value(), 0x100);   // absent: 0x100
    CB_CHECK(!parse_profile(R"({"name": "x", "psp": null})").psp.has_value());   // a null is none
    CB_CHECK(!parse_profile(R"({"name": "x", "top": 0})").top.has_value());
    CB_CHECK_EQ(minimal.emulator, "dos");
    CB_CHECK_EQ(minimal.source_ext, ".C");
});

CB_TEST("exact.toolchain.the_digest_ignores_a_listing_switch_and_nothing_else", [] {
    const std::string a = R"({"name": "x", "passes": [{"run": "P.EXE", "args": "{tag}"}]})";
    const std::string b = R"({"name": "x", "passes": [{"run": "P.EXE", "args": "{tag}", "listing": "-Fl"}]})";
    const std::string c = R"({"name": "x", "passes": [{"run": "P.EXE", "args": "{tag} -x"}]})";
    const std::string reordered = R"({"passes": [{"args": "{tag}", "run": "P.EXE"}], "name": "x"})";
    CB_CHECK_EQ(parse_profile(a).digest, parse_profile(b).digest);
    CB_CHECK_EQ(parse_profile(a).digest, parse_profile(reordered).digest);
    CB_CHECK(parse_profile(a).digest != parse_profile(c).digest);
});

CB_TEST("exact.toolchain.kits_are_found_by_setting_environment_or_name", [] {
    CB_WINDOWS_ONLY("it keeps a drive-lettered path as the kit's own");
    Fixture f;
    CB_CHECK(fs::same_path(f.locations.kit("k1"), f.dir.path() / "kits" / "k1"));   // relative: against the checkout
    CB_CHECK_THROWS_AS(UsageError, (void)f.locations.kit("k2"));
    {
        fs::ScopedEnv env("CB_KIT_K2", fs::to_text(f.dir.path() / "elsewhere"));
        CB_CHECK(fs::same_path(f.locations.kit("k2"), f.dir.path() / "elsewhere"));
    }
    // "KIT:path" is a file of that kit, a bare path one of the profile's kit, else it is taken from `base`
    CB_CHECK(fs::same_path(f.locations.kit_path(std::nullopt, "k1:disk1/X.EXE"), f.dir.path() / "kits" / "k1" / "disk1" / "X.EXE"));
    CB_CHECK(fs::same_path(f.locations.kit_path("k1", "disk1/X.EXE"), f.dir.path() / "kits" / "k1" / "disk1" / "X.EXE"));
    CB_CHECK(fs::same_path(f.locations.kit_path(std::nullopt, "a/../b.c", f.dir.path()), f.dir.path() / "b.c"));
    CB_CHECK(fs::same_path(f.locations.kit_path(std::nullopt, "c:/x.c", "C:/base"), "C:/x.c"));   // one letter is a drive
});

CB_TEST("exact.toolchain.a_checkouts_locations_come_from_its_settings", [] {
    fs::ScopedTempDir dir("toolchain-settings");
    fs::ScopedEnv tag("CB_STAGE_TAG", "w3");
    fs::write_text(dir.path() / "settings.json", R"({"state": "work", "kits": {"msc": "C:/kits/msc", "rel": "kits/rel"}, "other": 1})");
    const Locations l = Locations::of_checkout(dir.path());
    CB_CHECK(fs::same_path(l.state, dir.path() / "work"));
    CB_CHECK_EQ(l.kits.size(), 2u);
    CB_CHECK_EQ(l.stage_tag, "w3");
    CB_CHECK(fs::same_path(l.profiles, dir.path() / "config" / "exact" / "toolchains"));
    // no settings file: the defaults
    fs::ScopedTempDir bare("toolchain-bare");
    CB_CHECK(fs::same_path(Locations::of_checkout(bare.path()).state, bare.path() / "state"));
});

CB_TEST("exact.toolchain.a_sources_first_line_gives_its_own_switches", [] {
    Fixture f;
    const Toolchain tc = f.make();
    CB_CHECK_EQ(tc.switches_of(as_bytes(std::string_view("/* MFLAGS: -Ox -Gs */ int f;"))), "-Ox -Gs");
    CB_CHECK_EQ(tc.switches_of(as_bytes(std::string_view("int f;"))), "-O");                 // the profile's
    CB_CHECK_EQ(tc.switches_of(as_bytes(std::string_view("int f;")), "-Od"), "-Od");           // the caller's
    CB_CHECK_EQ(tc.switches_of(as_bytes(std::string_view("int f; /* MFLAGS: -x */"))), "-O");   // only the first line counts
    // an assembler's comes behind a semicolon
    const Profile asm_profile = parse_profile(R"({"name": "a", "switches": "/Z", "file_switches": "AFLAGS", "file_switches_comment": ";"})");
    Context c;
    c.locations = &f.locations;
    const Toolchain a(asm_profile, std::nullopt, c);
    CB_CHECK_EQ(a.switches_of(as_bytes(std::string_view("; AFLAGS: /ML\r\nmov ax,1"))), "/ML");
    CB_CHECK_EQ(a.switches_of(as_bytes(std::string_view("mov ax,1"))), "/Z");
});

CB_TEST("exact.toolchain.command_line_values_are_derived_from_the_switches", [] {
    Fixture f;
    const Toolchain tc = f.make();
    auto v = tc.values("-O -FPi", "T00042");
    CB_CHECK_EQ(v.at("tag"), "T00042");
    CB_CHECK_EQ(v.at("tag6"), "T00042");
    CB_CHECK_EQ(v.at("switches"), "-O -FPi");
    CB_CHECK_EQ(v.at("fp"), "-FPi");   // {1} is the regex's first group
    CB_CHECK_EQ(tc.values("-O", "T1").at("fp"), "-FPc");   // the else
    CB_CHECK_EQ(tc.values("-O", "T000123456").at("tag6"), "T00012");

    // the real compiler's: -Ot / -Os, and the optional switches
    Locations real;
    real.profiles = real_profiles();
    Context c;
    c.locations = &real;
    const Toolchain msc(read_profile(real, "msc300"), std::nullopt, c);
    const auto m = msc.values("-Oat -Gs", "T00001");
    CB_CHECK_EQ(m.at("ot"), "-Ot");
    CB_CHECK_EQ(m.at("gs"), " -Gs");
    CB_CHECK_EQ(m.at("oa"), " -Oa");
    CB_CHECK_EQ(msc.values("-Ox", "T00001").at("ot"), "-Os");
    CB_CHECK_EQ(msc.values("-Ox", "T00001").at("gs"), "");
});

CB_TEST("exact.toolchain.the_environment_leaves_out_what_the_switches_forbid", [] {
    Fixture f;
    const Toolchain tc = f.make();
    const Bytes with = tc.environment("-O");
    CB_CHECK_EQ(std::string(as_text(with)), std::string("PATH=C:\\\0NO87= \0", 16));
    CB_CHECK_EQ(std::string(as_text(tc.environment("-O -FPi"))), std::string("PATH=C:\\\0", 9));
});

CB_TEST("exact.toolchain.a_variant_chooses_the_patches_and_the_hooks", [] {
    Fixture f;
    CB_CHECK_EQ(f.make().variant(), "patched");   // the default
    CB_CHECK_EQ(f.make().patches().size(), 1u);
    CB_CHECK_EQ(f.make("plain").patches().size(), 0u);
    const Toolchain hooked = f.make("hooked");
    CB_CHECK_EQ(hooked.patches().size(), 2u);
    CB_CHECK(f.make("plain").key() != f.make("patched").key());   // another variant, another stage and cache
    CB_CHECK_THROWS_AS(UsageError, (void)f.make("nonesuch"));
    CB_CHECK_EQ(f.make().profile("segment_switch").value(), "-Gt");
    CB_CHECK(!f.make().profile("psp-or-so").has_value());
});

CB_TEST("exact.toolchain.the_stage_holds_the_tools_files_patched_for_the_variant", [] {
    Fixture f;
    const Toolchain tc = f.make();   // variant "patched"
    const stdfs::path stage = tc.stage();
    CB_CHECK(stdfs::exists(stage / ".staged"));
    const Bytes exe = fs::read_file(stage / "TOOL.EXE");
    CB_CHECK_EQ(exe[34], 'E');   // 'CD' at 2 past the 32-byte header, now 'EF'
    CB_CHECK_EQ(exe[35], 'F');
    CB_CHECK_EQ(exe[32], 'A');
    CB_CHECK_EQ(exe[48], 0);   // the in-memory patch is not made in the file
    // the include folder, with its folders, in upper case
    CB_CHECK(stdfs::exists(stage / "INCLUDE" / "STDIO.H"));
    CB_CHECK(stdfs::exists(stage / "INCLUDE" / "SYS" / "TYPES.H"));
    // staged once: a second call returns the same folder and rewrites nothing
    fs::write_text(stage / "TOOL.EXE", "touched");
    CB_CHECK(fs::same_path(tc.stage(), stage));
    CB_CHECK_EQ(fs::read_text(stage / "TOOL.EXE"), "touched");
    // another variant has its own stage
    CB_CHECK(!fs::same_path(f.make("plain").stage(), stage));
    CB_CHECK_EQ(fs::read_file(f.make("plain").stage() / "TOOL.EXE")[34], 'C');
});

CB_TEST("exact.toolchain.a_kit_that_is_not_this_compilers_is_refused", [] {
    Fixture f;
    Bytes other(40, 0);
    other[0] = 'M';
    other[1] = 'Z';
    other[8] = 2;
    fs::write_file(f.dir.path() / "kits" / "k1" / "TOOL.EXE", other);   // 'CD' is not at 34
    const Toolchain tc = f.make();
    CB_CHECK_THROWS_AS(FormatError, (void)tc.stage());
    CB_CHECK(!stdfs::exists(f.locations.state / "exact" / "stage" / ("fake-" + tc.key()) / ".staged"));   // and no stage is marked done
});

CB_TEST("exact.toolchain.a_compile_runs_the_passes_and_gives_the_object", [] {
    Fixture f;
    const Toolchain tc = f.make("plain");
    const std::string source = "int main() { return 0; }\r\n";
    const auto r = tc.compile(as_bytes(std::string_view(source)), {});
    CB_CHECK(r.object.has_value());
    // the source reached the pass with DOS line ends, and the object is the pass's
    CB_CHECK_EQ(std::string(as_text(*r.object)), "OBJ:int main() { return 0; }\r\n");
    CB_CHECK(r.console.starts_with("compiling T"));
    CB_CHECK(r.console.find('\r') == std::string::npos);
    CB_CHECK_EQ(f.runner.calls.load(), 1);
    const PassRun& run = f.runner.runs[0];
    CB_CHECK(run.exe.filename() == "TOOL.EXE");
    CB_CHECK_EQ(run.psp.value(), 0x100);
    CB_CHECK_EQ(run.top.value(), 0x7000);
    CB_CHECK(run.fast);   // nothing instrumented
    CB_CHECK(run.files != nullptr);
    // "{tag} {fp}": the tag and the derived value, the environment with the pass's own string
    CB_CHECK(run.command_tail.ends_with(" -FPc"));
    CB_CHECK(run.command_tail.starts_with("T"));
    const std::string env(as_text(run.environment));
    CB_CHECK(env.find("PATH=C:\\") == 0);
    CB_CHECK(env.find(std::string("WORK=") + run.command_tail.substr(0, 6)) != std::string::npos);
});

CB_TEST("exact.toolchain.headers_and_include_files_are_put_where_the_compiler_looks", [] {
    Fixture f;
    const Toolchain tc = f.make("plain");
    fs::make_dirs(f.dir.path() / "incs");
    fs::write_text(f.dir.path() / "incs" / "a.h", "int a;\n");
    modules::CompileOptions options;
    options.headers["tree.h"] = to_bytes("struct tree;\n");
    options.include = {fs::to_text(f.dir.path() / "incs")};
    options.switches = "-Ox";
    bool seen_header = false, seen_include = false;
    // the fake pass cannot see them; the MemFS it is given can (it holds the run's files)
    struct Looking final : PassRunner {
        FakeRunner* inner;
        bool* header;
        bool* include;
        emu::dos::Result run(const PassRun& pass) const override {
            *header = pass.files->is_file(pass.workdir / "TREE.H") && pass.files->read(pass.workdir / "TREE.H") == to_bytes("struct tree;\r\n");
            *include = pass.files->is_file(pass.workdir / "INCX" / "A.H");
            return inner->run(pass);
        }
    } looking;
    looking.inner = &f.runner;
    looking.header = &seen_header;
    looking.include = &seen_include;
    Context c;
    c.locations = &f.locations;
    c.runner = &looking;
    const Toolchain with_looking(read_profile(f.locations, "fake"), "plain", c);
    const auto r = with_looking.compile(as_bytes(std::string_view("int x;")), options);
    CB_CHECK(r.object.has_value());
    CB_CHECK(seen_header);
    CB_CHECK(seen_include);
});

CB_TEST("exact.toolchain.a_pass_that_fails_ends_the_compile", [] {
    Fixture f;
    const Toolchain tc = f.make("plain");
    const auto r = tc.compile(as_bytes(std::string_view("FAIL")), {});
    CB_CHECK(!r.object.has_value());
    CB_CHECK(r.console.find("(the pass that stopped: TOOL.EXE)") != std::string::npos);
});

CB_TEST("exact.toolchain.what_a_sources_head_answers_instruments_the_pass", [] {
    Fixture f;
    f.runner.runs.clear();
    f.make("plain").compile(as_bytes(std::string_view("/* ASKS: 3, 1 */ int f;")), {});
    CB_CHECK_EQ(f.runner.runs.size(), 1u);
    const Instrument& inst = f.runner.runs[0].instrument;
    CB_CHECK_EQ(inst.counted.size(), 1u);
    CB_CHECK_EQ(inst.counted[0].at.size(), 2u);
    CB_CHECK_EQ(inst.counted[0].at[1], 0x20);
    CB_CHECK(inst.counted[0].counts == (std::set<std::int64_t>{1, 3}));
    CB_CHECK_EQ(inst.counted[0].registers.at("AX"), 7);
    CB_CHECK(!f.runner.runs[0].fast);   // a pass with an instrument is not the fast run
    // the answers, as the toolchain reads them
    const auto answers = f.make("plain").answers_of(as_bytes(std::string_view("/* ASKS: 3, 1 */ int f;")));
    CB_CHECK(answers.at("ask") == (std::vector<std::int64_t>{1, 3}));
    CB_CHECK(f.make("plain").answers_of(as_bytes(std::string_view("int f;")) ).empty());
    // the same text further than 400 bytes in is not a head
    CB_CHECK(f.make("plain").answers_of(as_bytes(std::string(500, ' ') + "/* ASKS: 1 */")).empty());
});

CB_TEST("exact.toolchain.a_variants_hooks_and_patches_in_memory_are_described_to_the_runner", [] {
    Fixture f;
    Toolchains toolchains(f.locations, &f.runner);
    int asked = 0;
    toolchains.register_hook("test:jump", [&](const WordReader&, std::int64_t, std::int64_t, std::int64_t) -> std::optional<std::int64_t> {
        ++asked;
        return 0x42;
    });
    const auto tc = toolchains.load("fake", "hooked");
    f.runner.runs.clear();
    (void)tc->compile(as_bytes(std::string_view("int f;")), {});
    const Instrument& inst = f.runner.runs.at(0).instrument;
    CB_CHECK_EQ(inst.calls.size(), 1u);
    CB_CHECK_EQ(inst.calls[0].at[0], 5);
    CB_CHECK_EQ(inst.calls[0].fn({}, 0, 0, 0).value(), 0x42);
    CB_CHECK_EQ(asked, 1);
    CB_CHECK_EQ(inst.memory.size(), 1u);
    CB_CHECK_EQ(inst.memory[0].at, 16);
    CB_CHECK(inst.memory[0].now == Bytes{1});
    // a hook the profile names but nobody registered
    Toolchains bare(f.locations, &f.runner);
    CB_CHECK_THROWS_AS(UsageError, (void)bare.load("fake", "hooked")->compile(as_bytes(std::string_view("int f;")), {}));
});

CB_TEST("exact.toolchain.a_compile_is_kept_and_not_run_again", [] {
    Fixture f;
    base::cache::Config config;
    config.state_dir = f.dir.path() / "cache";
    config.shared_dir = f.dir.path() / "shared";
    base::cache::Cache cache(config);
    cache.declare_kind(kCacheKind, {base::cache::Scope::State, "1"});
    Context c;
    c.cache = &cache;
    const Toolchain tc = f.make("plain", &c);
    const std::string source = "int f(void) { return 7; }\n";
    const auto first = tc.compile(as_bytes(std::string_view(source)), {});
    CB_CHECK_EQ(f.runner.calls.load(), 1);
    const auto again = tc.compile(as_bytes(std::string_view(source)), {});
    CB_CHECK_EQ(f.runner.calls.load(), 1);   // kept
    CB_CHECK(again.object == first.object);
    CB_CHECK_EQ(again.console, first.console);
    // another source, other switches, or another variant are other compiles
    (void)tc.compile(as_bytes(std::string_view("int g;")), {});
    CB_CHECK_EQ(f.runner.calls.load(), 2);
    modules::CompileOptions options;
    options.switches = "-Ox";
    (void)tc.compile(as_bytes(std::string_view(source)), options);
    CB_CHECK_EQ(f.runner.calls.load(), 3);
    (void)f.make("patched", &c).compile(as_bytes(std::string_view(source)), {});
    CB_CHECK_EQ(f.runner.calls.load(), 4);
    // a line end that differs makes no other compile (the pass is always given DOS ones)
    (void)tc.compile(as_bytes(std::string_view("int f(void) { return 7; }\r\n")), {});
    CB_CHECK_EQ(f.runner.calls.load(), 4);
    // not kept when the caller says so
    (void)tc.compile_full(as_bytes(std::string_view(source)), {}, false, /*cache*/ false);
    CB_CHECK_EQ(f.runner.calls.load(), 5);
    CB_CHECK(tc.compile_key(as_bytes(std::string_view(source)), {}) != tc.compile_key(as_bytes(std::string_view(source)), options));
});

CB_TEST("exact.toolchain.a_listing_is_asked_of_the_pass_that_makes_one", [] {
    Fixture f;
    const Toolchain tc = f.make("plain");
    const auto r = tc.compile_full(as_bytes(std::string_view("int f;")), {}, /*want_listing*/ true);
    CB_CHECK(r.object.has_value());
    CB_CHECK_EQ(r.listing.value(), "line 1\nline 2\n");
    CB_CHECK(f.runner.runs.back().command_tail.find(" -Fl") != std::string::npos);   // the pass's listing switch
    CB_CHECK_EQ(std::string(as_text(*r.object)), "OBJ:int f;\r\n/* listing */");   // the marker that tells it from a plain compile
    // without a pass that lists, the listing is empty
    Context c;
    c.locations = &f.locations;
    c.runner = &f.runner;
    const Profile no_listing = parse_profile(R"({"name": "n", "kit": "k1", "stage": {"files": {"TOOL.EXE": "TOOL.EXE"}},
                                               "passes": [{"run": "TOOL.EXE", "args": "{tag}"}], "output": "{tag}.OBJ"})");
    const Toolchain n(no_listing, std::nullopt, c);
    CB_CHECK_EQ(n.compile_full(as_bytes(std::string_view("x")), {}, true).listing.value(), "");
    CB_CHECK(!n.compile_full(as_bytes(std::string_view("x")), {}, false).listing.has_value());
});

CB_TEST("exact.toolchain.files_on_disk_when_memory_is_off", [] {
    Fixture f;
    Context c;
    c.memfs = false;
    const Toolchain tc = f.make("plain", &c);
    const auto r = tc.compile(as_bytes(std::string_view("int f;")), {});
    CB_CHECK(r.object.has_value());
    CB_CHECK(f.runner.runs.back().files == nullptr);
    // the compile's own files are cleaned from the stage
    std::size_t leftovers = 0;
    for (const auto& e : stdfs::directory_iterator(tc.stage()))
        if (e.path().extension() == ".OBJ" || e.path().extension() == ".C") ++leftovers;
    CB_CHECK_EQ(leftovers, 0u);
});

CB_TEST("exact.toolchain.the_toolchains_in_use_are_kept_and_plugins_are_registered_by_name", [] {
    Fixture f;
    Toolchains toolchains(f.locations, &f.runner);
    CB_CHECK(toolchains.load("fake") == toolchains.load("fake"));   // one for each (name, variant)
    CB_CHECK(toolchains.load("fake", "plain") != toolchains.load("fake"));
    CB_CHECK_THROWS_AS(NotFoundError, (void)toolchains.load("not_a_profile"));
    CB_CHECK_THROWS_AS(UsageError, (void)toolchains.load("fake", "nonesuch"));
    CB_CHECK_EQ(toolchains.profiles().size(), 1u);
    fs::write_text(f.locations.profiles / "coded.json", R"({"name": "coded", "plugin": "pascal:Pascal"})");
    CB_CHECK_THROWS_AS(UnsupportedError, (void)toolchains.load("coded"));
    int made = 0;
    toolchains.register_plugin("pascal:Pascal", [&](const Profile& p, const std::optional<std::string>& v, const Context& c) {
        ++made;
        return std::make_shared<Toolchain>(p, v, c);
    });
    CB_CHECK_EQ(toolchains.load("coded")->name(), "coded");
    (void)toolchains.load("coded");
    CB_CHECK_EQ(made, 1);
});

CB_TEST("exact.toolchain.templates_fill_names_and_braces", [] {
    const std::map<std::string, std::string> v = {{"tag", "T1"}, {"fp", "-FPi"}};
    CB_CHECK_EQ(format_named("{tag}.OBJ", v), "T1.OBJ");
    CB_CHECK_EQ(format_named("{tag},{tag},NUL;", v), "T1,T1,NUL;");
    CB_CHECK_EQ(format_named("{{literal}} {fp}", v), "{literal} -FPi");
    CB_CHECK_EQ(format_named("", v), "");
    CB_CHECK_THROWS_AS(FormatError, (void)format_named("{missing}", v));
    CB_CHECK_THROWS_AS(FormatError, (void)format_named("{tag", v));
    CB_CHECK_THROWS_AS(FormatError, (void)format_named("tag}", v));
});
