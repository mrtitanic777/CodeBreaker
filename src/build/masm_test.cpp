// Tests of build/masm over a fake of what it drives: the native executive (a stand-in for MASM that writes an object made
// from the source). They cover what is written to DOS, the cache (what is kept, what is not, what a hit costs), how a refusal
// of the executive reaches the caller, the tools, the selftest and the command. (The real MASM is run by the executive's own
// tests and the exe gate: cb_tests holds no executive of its own here.)
//
// Notes:
//   * The results of assemble_many() are in the order the sources were given, and a stem given twice is a UsageError.
//   * No work folder is left behind: the run's folder is deleted when the call returns (the products are already read into
//     the result), and `work` is empty.
//   * A .LST or .LOG keeps the CRLF line ends the tool wrote -- as build/native_dos already does.
//   * The cache holds the object and the listing, in the layout of schema 2 of kind "masm".
//   * `masm` lists the object's segments without the "# work dir" line (there is none); its selftest names the functions as
//     `name=size`.
#include "build/masm.hpp"

#include <atomic>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"

namespace {

using namespace cb;
using namespace cb::build;
namespace stdfs = std::filesystem;
namespace nd = cb::build::native_dos;

// What a source turns into: a reversed copy of its bytes, unless it says SILENT (no object); NOLOG leaves no log.
Bytes object_of(std::string_view source) { return Bytes(source.rbegin(), source.rend()); }

// A stand-in for MASM on the native executive: `A.ASM,A.OBJ,A.LST,NUL;` reads A.ASM from the work folder and writes A.OBJ
// (the source reversed), A.LST (upper-cased) and A.LOG.
class FakeDos : public nd::DosExecutive {
public:
    bool available_ = true;
    bool refuse = false;
    mutable std::atomic<int> runs{0};
    mutable std::vector<std::string> command_lines;

    bool available() const override { return available_; }
    nd::Exit run(const stdfs::path&, const stdfs::path& work, std::string_view cmdline) const override {
        if (refuse) throw EmulationError("the guest faulted");
        ++runs;
        command_lines.emplace_back(cmdline);
        const std::string name(cmdline.substr(0, cmdline.find(".ASM,")));
        const std::string source = std::string(as_text(fs::read_file(work / (name + ".ASM"))));
        if (source.find("SILENT") == std::string::npos) fs::write_file(work / (name + ".OBJ"), object_of(source));
        fs::write_text(work / (name + ".LST"), text::upper(source));
        if (source.find("NOLOG") == std::string::npos) fs::write_text(work / (name + ".LOG"), "log of " + name);
        return {0, {}};
    }
    nd::MemoryResult run_dpmi(const nd::MemoryRun&) const override { return {}; }
};

struct Rig {
    fs::ScopedTempDir dir{"masm"};
    FakeDos dos;
    masm::Tools tools;
    std::unique_ptr<base::cache::Cache> cache;

    explicit Rig(bool with_cache = false) {
        fs::write_text(dir.path() / "masm" / "BIN" / "MASM.EXE", "MZ-not-really");
        fs::write_text(dir.path() / "masm" / "INC" / "X.INC", "include-1");
        tools.masm = dir.path() / "masm";
        if (with_cache) {
            base::cache::Config config;
            config.state_dir = dir.path() / "cache-state";
            config.shared_dir = dir.path() / "cache-shared";
            cache = std::make_unique<base::cache::Cache>(config);
        }
    }
    masm::Context context() { return masm::Context{tools, dos, cache.get()}; }
};

std::vector<nd::Source> sources(std::initializer_list<std::pair<const char*, const char*>> items) {
    std::vector<nd::Source> out;
    for (const auto& [stem, text] : items) out.push_back({stem, text});
    return out;
}

std::string text_of(const std::optional<Bytes>& b) { return b ? std::string(as_text(*b)) : "<none>"; }

}  // namespace

// ---- one source --------------------------------------------------------------------------------------------------------

CB_TEST("build.masm.assemble_runs_the_native_executive_and_reads_back_the_products", [] {
    Rig rig;
    masm::Assembler assembler(rig.context());
    const nd::Product p = assembler.assemble("mov ax,1\r\nint 21h\n");
    CB_CHECK_EQ(rig.dos.runs.load(), 1);
    CB_CHECK_EQ(rig.dos.command_lines.at(0), "t.ASM,t.OBJ,t.LST,NUL;");
    CB_CHECK(p.obj == object_of("mov ax,1\r\nint 21h\r\n"));   // (the source reached DOS as clean CRLF text)
    CB_CHECK_EQ(p.lst.value_or("<none>"), std::string("MOV AX,1\r\nINT 21H\r\n"));
    CB_CHECK_EQ(p.log.value_or("<none>"), std::string("log of t"));
});

CB_TEST("build.masm.a_part_the_assembler_did_not_make_is_absent", [] {
    Rig rig;
    masm::Assembler assembler(rig.context());
    const nd::Product p = assembler.assemble("SILENT NOLOG", "x");
    CB_CHECK(!p.obj.has_value());
    CB_CHECK(p.lst.has_value());
    CB_CHECK_EQ(p.log.value_or("<none>"), std::string());   // no .LOG: what MASM printed to the console (nothing, here)
});

CB_TEST("build.masm.a_run_the_executive_cannot_complete_fails_with_its_reason", [] {
    Rig rig;
    rig.dos.refuse = true;
    masm::Assembler assembler(rig.context());
    try {
        (void)assembler.assemble("abc", "stem");
        CB_CHECK(false);
    } catch (const nd::NativeDosError& e) {
        CB_CHECK(std::string(e.what()).find("the guest faulted") != std::string::npos);
    }
    CB_CHECK_THROWS_AS(nd::NativeDosError, (void)assembler.assemble_many(sources({{"F0", "x"}})));
});

CB_TEST("build.masm.without_an_executive_the_run_is_refused_plainly", [] {
    Rig rig;
    rig.dos.available_ = false;   // (unicorn.dll was not handed over)
    masm::Assembler assembler(rig.context());
    CB_CHECK_THROWS_AS(nd::NativeDosError, (void)assembler.assemble("ab"));
    CB_CHECK_THROWS_AS(nd::NativeDosError, (void)assembler.assemble_many(sources({{"F0", "ab"}})));
    CB_CHECK_EQ(rig.dos.runs.load(), 0);
});

CB_TEST("build.masm.what_the_assembler_needs_is_named_when_it_is_missing", [] {
    Rig rig;
    masm::Assembler assembler(rig.context());
    // no MASM.EXE under the folder
    CB_CHECK_THROWS_AS(NotFoundError, (void)assembler.assemble("x", "t", rig.dir.path() / "empty"));
    // a folder given for the call replaces the default one
    fs::write_text(rig.dir.path() / "masm5" / "BIN" / "MASM.EXE", "MZ");
    CB_CHECK_EQ(text_of(assembler.assemble("ab", "t", rig.dir.path() / "masm5").obj), std::string("ba"));
});

// ---- many sources, the cache ---------------------------------------------------------------------------------------------

CB_TEST("build.masm.assemble_many_serves_a_kept_source_without_a_run", [] {
    Rig rig(true);
    masm::Assembler assembler(rig.context());
    const auto items = sources({{"F0", "one"}, {"F1", "two"}, {"F2", "three"}});
    const nd::Finished first = assembler.assemble_many(items);
    CB_CHECK_EQ(first.results.size(), std::size_t{3});
    CB_CHECK_EQ(first.results[0].first, std::string("F0"));
    CB_CHECK_EQ(text_of(first.results[1].second.obj), std::string("owt"));
    CB_CHECK_EQ(text_of(first.results[2].second.obj), std::string("eerht"));
    CB_CHECK_EQ(rig.dos.runs.load(), 3);
    // all kept: nothing runs, the products come back as they were (the listing; a kept assembly has an empty log)
    const nd::Finished second = assembler.assemble_many(items);
    CB_CHECK_EQ(rig.dos.runs.load(), 3);
    CB_CHECK_EQ(second.results.size(), std::size_t{3});
    for (std::size_t i = 0; i < 3; ++i) {
        CB_CHECK_EQ(second.results[i].first, first.results[i].first);
        CB_CHECK(second.results[i].second.obj == first.results[i].second.obj);
        CB_CHECK(second.results[i].second.lst == first.results[i].second.lst);
        CB_CHECK_EQ(second.results[i].second.log.value_or("<none>"), std::string());
    }
    // a new source among kept ones: only it runs, and the results keep the order given
    const nd::Finished third = assembler.assemble_many(sources({{"F0", "one"}, {"F9", "new"}, {"F2", "three"}}));
    CB_CHECK_EQ(rig.dos.runs.load(), 4);
    CB_CHECK_EQ(third.results[1].first, std::string("F9"));
    CB_CHECK_EQ(text_of(third.results[1].second.obj), std::string("wen"));
    CB_CHECK_EQ(text_of(third.results[0].second.obj), std::string("eno"));
});

CB_TEST("build.masm.the_cache_is_keyed_by_the_stem_the_source_and_the_assemblers_files", [] {
    Rig rig(true);
    masm::Assembler assembler(rig.context());
    (void)assembler.assemble_many(sources({{"A", "same"}}));
    CB_CHECK_EQ(rig.dos.runs.load(), 1);
    (void)assembler.assemble_many(sources({{"B", "same"}}));   // another stem: the object's title names it
    CB_CHECK_EQ(rig.dos.runs.load(), 2);
    (void)assembler.assemble_many(sources({{"a", "same"}}));   // (the stem is compared without regard to case)
    CB_CHECK_EQ(rig.dos.runs.load(), 2);
    fs::write_text(rig.tools.masm / "INC" / "X.INC", "include-2-longer");   // a file MASM may read changed
    (void)assembler.assemble_many(sources({{"A", "same"}}));
    CB_CHECK_EQ(rig.dos.runs.load(), 3);
    // a folder is its files' bytes, not its place: a copy of the assembler is a hit, another MASM.EXE is not
    fs::write_text(rig.dir.path() / "copy" / "BIN" / "MASM.EXE", "MZ-not-really");
    fs::write_text(rig.dir.path() / "copy" / "INC" / "X.INC", "include-2-longer");
    (void)assembler.assemble_many(sources({{"A", "same"}}), rig.dir.path() / "copy");
    CB_CHECK_EQ(rig.dos.runs.load(), 3);
    fs::write_text(rig.dir.path() / "copy" / "BIN" / "MASM.EXE", "MZ-another-assembler");
    (void)assembler.assemble_many(sources({{"A", "same"}}), rig.dir.path() / "copy");
    CB_CHECK_EQ(rig.dos.runs.load(), 4);
});

CB_TEST("build.masm.an_assembly_that_made_no_object_is_not_kept", [] {
    Rig rig(true);
    masm::Assembler assembler(rig.context());
    const auto items = sources({{"F0", "SILENT broken"}, {"F1", "fine"}});
    const nd::Finished first = assembler.assemble_many(items);
    CB_CHECK(!first.results[0].second.obj.has_value());
    CB_CHECK(first.results[1].second.obj.has_value());
    CB_CHECK_EQ(rig.dos.runs.load(), 2);
    (void)assembler.assemble_many(items);
    CB_CHECK_EQ(rig.dos.runs.load(), 3);   // the failed one ran again, the good one did not
});

CB_TEST("build.masm.without_a_cache_everything_runs_every_time", [] {
    Rig rig;
    masm::Assembler assembler(rig.context());
    const auto items = sources({{"F0", "a"}, {"F1", "b"}});
    (void)assembler.assemble_many(items);
    (void)assembler.assemble_many(items);
    CB_CHECK_EQ(rig.dos.runs.load(), 4);
});

CB_TEST("build.masm.a_stem_given_twice_is_a_usage_error", [] {
    Rig rig;
    masm::Assembler assembler(rig.context());
    CB_CHECK_THROWS_AS(UsageError, (void)assembler.assemble_many(sources({{"F0", "a"}, {"F0", "b"}})));
    CB_CHECK(assembler.assemble_many({}).results.empty());
});

// ---- the tools -------------------------------------------------------------------------------------------------------------------

CB_TEST("build.masm.tools_come_from_the_settings", [] {
    CB_WINDOWS_ONLY("it names Windows drive paths");
    fs::ScopedTempDir dir("masm");
    fs::write_text(dir.path() / "settings.json", "{\"masm\": \"M:/masm\"}");
    const auto config = config::Config::load(dir.path());
    CB_CHECK(masm::Tools::from_config(config).masm == stdfs::path("M:/masm"));
});

// ---- the functions, the selftest, the command ----------------------------------------------------------------------------------------

// MASM for a source made of the selftest's text: the object of `_addasm`, the bytes the real MASM makes.
class SelfTestDos : public FakeDos {
public:
    Bytes code = {0x55, 0x8B, 0xEC, 0x8B, 0x46, 0x04, 0x03, 0x46, 0x06, 0x5D, 0xC3};
    nd::Exit run(const stdfs::path&, const stdfs::path& work, std::string_view cmdline) const override {
        ++runs;
        const std::string name(cmdline.substr(0, cmdline.find(".ASM,")));
        fs::write_file(work / (name + ".OBJ"), formats::omf::raw_object(name, code, {{"_ADDASM", 0}}));
        fs::write_text(work / (name + ".LST"), "listing");
        return {0, {}};
    }
};

CB_TEST("build.masm.functions_lists_the_publics_of_the_object_with_their_bytes", [] {
    Rig rig;
    SelfTestDos dos;
    masm::Context context = rig.context();
    masm::Context with{context.tools, dos, nullptr};
    masm::Assembler assembler(with);
    const auto functions = assembler.functions(masm::kSelfTestSource);
    CB_CHECK_EQ(functions.size(), std::size_t{1});
    const auto* f = functions.find("_ADDASM");
    CB_CHECK(f != nullptr);
    CB_CHECK_EQ(f->code.size(), std::size_t{11});
    CB_CHECK(f->fixup_mask.empty());
    // an assembly that made no object has no functions: its log is the message
    Rig silent;
    masm::Assembler plain(silent.context());
    try {
        (void)plain.functions("SILENT");
        CB_CHECK(false);
    } catch (const ToolError& e) {
        CB_CHECK_EQ(e.output(), std::string("log of t"));
    }
});

CB_TEST("build.masm.the_selftest_passes_on_the_bytes_masm_makes_and_fails_on_others", [] {
    Rig rig;
    SelfTestDos dos;
    masm::Context good{rig.tools, dos, nullptr};
    masm::Assembler assembler(good);
    std::string report;
    CB_CHECK(masm::selftest(assembler, report));
    CB_CHECK(report.find("functions: _ADDASM=11\n") != std::string::npos);
    CB_CHECK(report.find("_addasm bytes: 558bec8b46040346065dc3\n") != std::string::npos);
    CB_CHECK(report.find("SELFTEST: PASS - MASM assembled byte-exact proc") != std::string::npos);
    SelfTestDos wrong;
    wrong.code = {0x55, 0x8B, 0xEC, 0x8B, 0x46, 0x04, 0x01, 0x46, 0x06, 0x5D, 0xC3};   // add the other way: not the bytes
    masm::Context bad{rig.tools, wrong, nullptr};
    masm::Assembler other(bad);
    report.clear();
    CB_CHECK(!masm::selftest(other, report));
    CB_CHECK(report.find("SELFTEST: FAIL") != std::string::npos);
});

CB_TEST("build.masm.the_selftest_with_no_object_gives_the_log", [] {
    class NoObject : public FakeDos {
    public:
        nd::Exit run(const stdfs::path&, const stdfs::path& work, std::string_view cmdline) const override {
            const std::string name(cmdline.substr(0, cmdline.find(".ASM,")));
            fs::write_text(work / (name + ".LOG"), "Severity code 1: no way");
            return {0, {}};
        }
    };
    Rig rig;
    NoObject dos;
    const masm::Context context{rig.tools, dos, nullptr};
    masm::Assembler assembler(context);
    std::string report;
    CB_CHECK(!masm::selftest(assembler, report));
    CB_CHECK(report.starts_with("no object; MASM log:\nSeverity code 1: no way"));
});

CB_TEST("build.masm.the_command_declares_its_arguments", [] {
    Rig rig;
    const cli::Command c = masm::command(rig.context());
    CB_CHECK_EQ(c.name, std::string("masm"));
    const cli::Cli app("codebreaker", {c});
    const auto parsed = app.parse({"masm", "--selftest"});
    CB_CHECK(parsed.args.flag("selftest"));
    CB_CHECK(!app.parse({"masm", "f.asm"}).args.flag("selftest"));
    CB_CHECK_EQ(app.parse({"masm", "f.asm"}).args.value("asm").value_or(""), std::string("f.asm"));
});

CB_TEST("build.masm.the_command_runs_the_selftest", [] {
    Rig rig;
    SelfTestDos dos;
    const masm::Context context{rig.tools, dos, nullptr};
    const cli::Cli app("codebreaker", {masm::command(context)});
    testing::CapturedStd captured;
    CB_CHECK_EQ(app.run({"masm", "--selftest"}), 0);
    CB_CHECK(captured.out().find("SELFTEST: PASS") != std::string::npos);
});

CB_TEST("build.masm.the_command_lists_the_segments_of_a_file_it_assembled", [] {
    Rig rig;
    SelfTestDos dos;
    fs::write_text(rig.dir.path() / "p.asm", std::string(masm::kSelfTestSource));
    const masm::Context context{rig.tools, dos, nullptr};
    const cli::Cli app("codebreaker", {masm::command(context)});
    testing::CapturedStd captured;
    CB_CHECK_EQ(app.run({"masm", fs::to_text(rig.dir.path() / "p.asm")}), 0);
    const std::string out = captured.out();
    CB_CHECK(out.find("_TEXT") != std::string::npos);
    CB_CHECK(out.find("class=CODE") != std::string::npos);
    CB_CHECK(out.find("11 bytes") != std::string::npos);
    // no argument is a usage error
    CB_CHECK_THROWS_AS(UsageError, (void)app.run({"masm"}));
});
