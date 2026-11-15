// Tests of build/masm over fakes of what it drives: the native executive (a stand-in for MASM that writes an object made
// from the source), the DOSBox runner, and warm sessions. They cover what is written to DOS and in what order, the fall back
// from the native run to DOSBox, the cache (what is kept, what is not, what a hit costs), batching and the warm pool, the
// environment's options, the selftest and the command. (The real emulator and the real MASM are run by the old executive's
// tests and the exe gate: cb_tests holds no executive of its own.)
//
// What an old harness established, and these tests keep: the products' parts (object,
// listing, log) as the tool made them, a part the tool did not make is absent, the cache serves a repeated source without a
// run, an assembly that made no object is not kept, and the selftest's account and verdict.
//
// Notes:
//   * The results of assemble_many() are in the order the sources were given, and a stem given twice is a UsageError.
//   * No work folder is left behind: the cold boot's folder is deleted when the call returns (the products are already read
//     into the result), and `work` is empty.
//   * A .LST or .LOG keeps the CRLF line ends the tool wrote -- as
//     build/native_dos already does.
//   * The cold boot runs through the DosRunner (the exe's DOSBox-X driver) and no longer polls a DONE.FLG sentinel of its own;
//     whether the emulator exits by itself or is stopped once the products exist is the runner's.
//   * The cache holds the object and the listing, in the layout
//     of schema 2 of kind "masm".
//   * `masm` lists the object's segments without the "# work dir" line (there is none); its selftest names the functions as
//     `name=size`.
#include "cb/build/masm.hpp"

#include <atomic>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace {

using namespace cb;
using namespace cb::build;
namespace stdfs = std::filesystem;
namespace dsess = cb::build::dos_sessions;
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

// A stand-in for the DOSBox runner: for each `MASM S.ASM, S.OBJ, ...` command it makes the same products the executive does.
class FakeRunner : public DosRunner {
public:
    mutable std::vector<DosRunSpec> specs;
    mutable std::mutex lock;
    DosRunResult run(const DosRunSpec& spec) const override {
        {
            std::lock_guard g(lock);
            specs.push_back(spec);
        }
        DosRunResult r;
        r.ok = true;
        for (const std::string& command : spec.commands) {
            const std::string stem = command.substr(5, command.find(".ASM,") - 5);
            const DosFile& file = spec.files.at(stem + ".ASM");
            const std::string source = std::get<std::string>(file);
            if (source.find("SILENT") == std::string::npos) r.made[text::upper(stem) + ".OBJ"] = object_of(source);
            r.made[text::upper(stem) + ".LST"] = to_bytes(text::upper(source));
            if (source.find("NOLOG") == std::string::npos) r.made[text::upper(stem) + ".LOG"] = to_bytes("log of " + stem);
        }
        return r;
    }
};

// A warm session that assembles like the others, and counts its batches.
class FakeSession : public dsess::Session {
public:
    explicit FakeSession(std::atomic<int>& batches, bool dies = false) : batches_(batches), dies_(dies) {}
    dsess::Outcome assemble_many(std::span<const dsess::Source> items, std::optional<std::chrono::milliseconds>,
                                 const std::function<bool()>&) override {
        ++batches_;
        if (dies_) return dsess::Dead{};
        dsess::Finished done;
        for (const dsess::Source& item : items) {
            dsess::Product p;
            if (item.text.find("SILENT") == std::string::npos) p.obj = object_of(item.text);
            p.lst = text::upper(item.text);
            p.log = "warm log of " + item.stem;
            done.results.emplace_back(item.stem, std::move(p));
        }
        done.work = "session-work";
        return done;
    }
    bool alive() override { return alive_; }
    void close() override { alive_ = false; }
    bool alive_ = true;

private:
    std::atomic<int>& batches_;
    bool dies_;
};

struct Rig {
    fs::ScopedTempDir dir{"masm"};
    FakeDos dos;
    FakeRunner runner;
    masm::Tools tools;
    masm::Options options;
    std::unique_ptr<base::cache::Cache> cache;
    std::function<std::unique_ptr<dsess::WorkerPool>(const stdfs::path&)> make_pool;
    std::atomic<int> sessions_made{0}, batches{0};
    bool sessions_die = false;

    explicit Rig(bool with_cache = false) {
        fs::write_text(dir.path() / "masm" / "BIN" / "MASM.EXE", "MZ-not-really");
        fs::write_text(dir.path() / "masm" / "INC" / "X.INC", "include-1");
        fs::write_text(dir.path() / "dosbox.exe", "dosbox");
        tools.masm = dir.path() / "masm";
        tools.dosbox_x = dir.path() / "dosbox.exe";
        options.warm_pool = false;
        if (with_cache) {
            base::cache::Config config;
            config.state_dir = dir.path() / "cache-state";
            config.shared_dir = dir.path() / "cache-shared";
            cache = std::make_unique<base::cache::Cache>(config);
        }
    }
    masm::Context context() {
        make_pool = [this](const stdfs::path&) {
            return std::make_unique<dsess::WorkerPool>(
                [this]() -> std::shared_ptr<dsess::Session> { ++sessions_made; return std::make_shared<FakeSession>(batches, sessions_die); }, options.pool_max);
        };
        return masm::Context{tools, options, dos, runner, cache.get(), make_pool};
    }
};

std::vector<dsess::Source> sources(std::initializer_list<std::pair<const char*, const char*>> items) {
    std::vector<dsess::Source> out;
    for (const auto& [stem, text] : items) out.push_back({stem, text});
    return out;
}

std::string text_of(const std::optional<Bytes>& b) { return b ? std::string(as_text(*b)) : "<none>"; }

}  // namespace

// ---- one source --------------------------------------------------------------------------------------------------------

CB_TEST("build.masm.assemble_runs_the_native_executive_and_reads_back_the_products", [] {
    Rig rig;
    masm::Assembler assembler(rig.context());
    const dsess::Product p = assembler.assemble("mov ax,1\r\nint 21h\n");
    CB_CHECK_EQ(rig.dos.runs.load(), 1);
    CB_CHECK_EQ(rig.dos.command_lines.at(0), "t.ASM,t.OBJ,t.LST,NUL;");
    CB_CHECK(p.obj == object_of("mov ax,1\r\nint 21h\r\n"));   // (the source reached DOS as clean CRLF text)
    CB_CHECK_EQ(p.lst.value_or("<none>"), std::string("MOV AX,1\r\nINT 21H\r\n"));
    CB_CHECK_EQ(p.log.value_or("<none>"), std::string("log of t"));
    CB_CHECK(rig.runner.specs.empty());   // DOSBox was not needed
});

CB_TEST("build.masm.a_part_the_assembler_did_not_make_is_absent", [] {
    Rig rig;
    masm::Assembler assembler(rig.context());
    const dsess::Product p = assembler.assemble("SILENT NOLOG", "x");
    CB_CHECK(!p.obj.has_value());
    CB_CHECK(p.lst.has_value());
    CB_CHECK_EQ(p.log.value_or("<none>"), std::string());   // no .LOG: what MASM printed to the console (nothing, here)
});

CB_TEST("build.masm.when_the_native_run_cannot_complete_dosbox_takes_over", [] {
    Rig rig;
    rig.dos.refuse = true;
    masm::Assembler assembler(rig.context());
    const dsess::Product p = assembler.assemble("abc", "stem");
    CB_CHECK_EQ(text_of(p.obj), std::string("cba"));
    CB_CHECK_EQ(p.lst.value_or("<none>"), std::string("ABC"));
    CB_CHECK_EQ(p.log.value_or("<none>"), std::string("log of stem"));
    // what DOSBox was given: the assembler's folder as C:, a work folder as D:, one MASM line per source, the environment
    // MASM reads, a 386 on the dynamic core, DOS 6.22
    CB_CHECK_EQ(rig.runner.specs.size(), std::size_t{1});
    const DosRunSpec& spec = rig.runner.specs.at(0);
    CB_CHECK_EQ(spec.mounts.size(), std::size_t{2});
    CB_CHECK_EQ(spec.mounts[0].drive, std::string("c"));
    CB_CHECK(spec.mounts[0].folder == rig.tools.masm);
    CB_CHECK_EQ(spec.mounts[1].drive, std::string("d"));
    CB_CHECK_EQ(spec.commands.size(), std::size_t{1});
    CB_CHECK_EQ(spec.commands[0], std::string("MASM stem.ASM, stem.OBJ, stem.LST, NUL; > stem.LOG 2>&1"));
    CB_CHECK_EQ(std::get<std::string>(spec.files.at("stem.ASM")), std::string("abc"));
    CB_CHECK_EQ(spec.variables.at("PATH"), std::string("C:\\BIN"));
    CB_CHECK_EQ(spec.variables.at("INCLUDE"), std::string("C:\\INC"));
    CB_CHECK_EQ(spec.variables.at("LIB"), std::string("C:\\LIB"));
    CB_CHECK_EQ(spec.variables.at("TMP"), std::string("D:\\"));
    CB_CHECK_EQ(spec.cpu.at("core"), std::string("dynamic"));
    CB_CHECK_EQ(spec.cpu.at("cputype"), std::string("386"));
    CB_CHECK_EQ(spec.cpu.at("cycles"), std::string("max"));
    CB_CHECK_EQ(spec.dos.at("ver"), std::string("6.22"));
    CB_CHECK_EQ(spec.timeout.count(), 180);
    // the core is the option's
    rig.options.dosbox_core = "normal";
    masm::Assembler other(rig.context());
    (void)other.assemble("x");
    CB_CHECK_EQ(rig.runner.specs.back().cpu.at("core"), std::string("normal"));
});

CB_TEST("build.masm.without_a_native_executive_dosbox_is_the_way", [] {
    Rig rig;
    rig.dos.available_ = false;   // (unicorn.dll was not handed over)
    masm::Assembler assembler(rig.context());
    CB_CHECK_EQ(text_of(assembler.assemble("ab").obj), std::string("ba"));
    CB_CHECK_EQ(rig.dos.runs.load(), 0);
    CB_CHECK_EQ(rig.runner.specs.size(), std::size_t{1});
});

CB_TEST("build.masm.what_the_assembler_needs_is_named_when_it_is_missing", [] {
    Rig rig;
    rig.dos.refuse = true;
    {   // no emulator set up
        rig.tools.dosbox_x = rig.dir.path() / "no-dosbox.exe";
        masm::Assembler assembler(rig.context());
        CB_CHECK_THROWS_AS(ToolError, (void)assembler.assemble("x"));
    }
    rig.tools.dosbox_x = rig.dir.path() / "dosbox.exe";
    {   // CB_NO_DOSBOX
        rig.tools.dosbox_forbidden = true;
        masm::Assembler assembler(rig.context());
        try {
            (void)assembler.assemble("x");
            CB_CHECK(false);
        } catch (const ToolError& e) {
            CB_CHECK(std::string(e.what()).find("CB_NO_DOSBOX") != std::string::npos);
        }
    }
    rig.tools.dosbox_forbidden = false;
    {   // no MASM.EXE under the folder
        masm::Assembler assembler(rig.context());
        CB_CHECK_THROWS_AS(NotFoundError, (void)assembler.assemble("x", "t", std::chrono::seconds(5), rig.dir.path() / "empty"));
        // a folder given for the call replaces the default one
        fs::write_text(rig.dir.path() / "masm5" / "BIN" / "MASM.EXE", "MZ");
        (void)assembler.assemble("x", "t", std::chrono::seconds(5), rig.dir.path() / "masm5");
        CB_CHECK(rig.runner.specs.back().mounts[0].folder == rig.dir.path() / "masm5");
    }
});

// ---- many sources, the cache ---------------------------------------------------------------------------------------------

CB_TEST("build.masm.assemble_many_serves_a_kept_source_without_a_run", [] {
    Rig rig(true);
    masm::Assembler assembler(rig.context());
    const auto items = sources({{"F0", "one"}, {"F1", "two"}, {"F2", "three"}});
    const dsess::Finished first = assembler.assemble_many(items);
    CB_CHECK_EQ(first.results.size(), std::size_t{3});
    CB_CHECK_EQ(first.results[0].first, std::string("F0"));
    CB_CHECK_EQ(text_of(first.results[1].second.obj), std::string("owt"));
    CB_CHECK_EQ(text_of(first.results[2].second.obj), std::string("eerht"));
    CB_CHECK_EQ(rig.dos.runs.load(), 3);
    // all kept: nothing runs, the products come back as they were (the listing; a kept assembly has an empty log)
    const dsess::Finished second = assembler.assemble_many(items);
    CB_CHECK_EQ(rig.dos.runs.load(), 3);
    CB_CHECK_EQ(second.results.size(), std::size_t{3});
    for (std::size_t i = 0; i < 3; ++i) {
        CB_CHECK_EQ(second.results[i].first, first.results[i].first);
        CB_CHECK(second.results[i].second.obj == first.results[i].second.obj);
        CB_CHECK(second.results[i].second.lst == first.results[i].second.lst);
        CB_CHECK_EQ(second.results[i].second.log.value_or("<none>"), std::string());
    }
    // a new source among kept ones: only it runs, and the results keep the order given
    const dsess::Finished third = assembler.assemble_many(sources({{"F0", "one"}, {"F9", "new"}, {"F2", "three"}}));
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
    (void)assembler.assemble_many(sources({{"A", "same"}}), std::nullopt, rig.dir.path() / "copy");
    CB_CHECK_EQ(rig.dos.runs.load(), 3);
    fs::write_text(rig.dir.path() / "copy" / "BIN" / "MASM.EXE", "MZ-another-assembler");
    (void)assembler.assemble_many(sources({{"A", "same"}}), std::nullopt, rig.dir.path() / "copy");
    CB_CHECK_EQ(rig.dos.runs.load(), 4);
});

CB_TEST("build.masm.an_assembly_that_made_no_object_is_not_kept", [] {
    Rig rig(true);
    masm::Assembler assembler(rig.context());
    const auto items = sources({{"F0", "SILENT broken"}, {"F1", "fine"}});
    const dsess::Finished first = assembler.assemble_many(items);
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

// ---- batches and warm sessions --------------------------------------------------------------------------------------------------

CB_TEST("build.masm.a_large_module_is_cut_into_batches_and_the_results_keep_their_order", [] {
    Rig rig;
    rig.dos.refuse = true;   // everything goes to DOSBox
    rig.options.batch = 2;
    masm::Assembler assembler(rig.context());
    std::vector<dsess::Source> items;
    for (int i = 0; i < 5; ++i) items.push_back({"F" + std::to_string(i), "src" + std::to_string(i)});
    const dsess::Finished done = assembler.assemble_many(items);
    CB_CHECK_EQ(rig.runner.specs.size(), std::size_t{3});   // 2 + 2 + 1 commands: one boot each (the warm pool is off)
    CB_CHECK_EQ(done.results.size(), std::size_t{5});
    for (int i = 0; i < 5; ++i) {
        CB_CHECK_EQ(done.results[static_cast<std::size_t>(i)].first, "F" + std::to_string(i));
        const Bytes expected = object_of("src" + std::to_string(i));
        CB_CHECK(done.results[static_cast<std::size_t>(i)].second.obj == expected);
    }
    std::vector<std::size_t> sizes;
    for (const DosRunSpec& s : rig.runner.specs) sizes.push_back(s.commands.size());
    std::sort(sizes.begin(), sizes.end());
    CB_CHECK(sizes == (std::vector<std::size_t>{1, 2, 2}));
});

CB_TEST("build.masm.warm_sessions_are_shared_between_batches_and_kept", [] {
    Rig rig;
    rig.dos.refuse = true;
    rig.options.warm_pool = true;
    rig.options.batch = 2;
    rig.options.pool_max = 1;   // one session does every batch, one after the other
    masm::Assembler assembler(rig.context());
    std::vector<dsess::Source> items;
    for (int i = 0; i < 5; ++i) items.push_back({"F" + std::to_string(i), "src" + std::to_string(i)});
    const dsess::Finished done = assembler.assemble_many(items);
    CB_CHECK_EQ(done.results.size(), std::size_t{5});
    CB_CHECK_EQ(rig.sessions_made.load(), 1);
    CB_CHECK_EQ(rig.batches.load(), 3);
    CB_CHECK(rig.runner.specs.empty());   // no DOSBox of its own
    CB_CHECK_EQ(done.results[4].second.log.value_or(""), std::string("warm log of F4"));
    // another call reuses the kept session
    (void)assembler.assemble_many(sources({{"G0", "x"}}));
    CB_CHECK_EQ(rig.sessions_made.load(), 1);
    CB_CHECK_EQ(rig.batches.load(), 4);
});

CB_TEST("build.masm.a_session_is_retired_after_its_uses_when_asked", [] {
    Rig rig;
    rig.dos.refuse = true;
    rig.options.warm_pool = true;
    rig.options.pool_max = 1;
    rig.options.worker_max_uses = 2;
    masm::Assembler assembler(rig.context());
    for (int i = 0; i < 4; ++i) (void)assembler.assemble_many(sources({{"F0", "x"}}));
    CB_CHECK_EQ(rig.batches.load(), 4);
    CB_CHECK_EQ(rig.sessions_made.load(), 2);   // retired after its second batch, a fresh one for the third
});

CB_TEST("build.masm.a_session_that_dies_is_dropped_and_the_batch_boots_its_own_dosbox", [] {
    Rig rig;
    rig.dos.refuse = true;
    rig.options.warm_pool = true;
    rig.sessions_die = true;
    masm::Assembler assembler(rig.context());
    const dsess::Finished done = assembler.assemble_many(sources({{"F0", "ab"}}));
    CB_CHECK_EQ(rig.batches.load(), 1);
    CB_CHECK_EQ(rig.runner.specs.size(), std::size_t{1});
    CB_CHECK_EQ(text_of(done.results.at(0).second.obj), std::string("ba"));
});

CB_TEST("build.masm.a_pool_that_cannot_boot_is_no_pool", [] {
    Rig rig;
    rig.dos.refuse = true;
    rig.options.warm_pool = true;
    masm::Context context = rig.context();
    context.make_pool = [](const stdfs::path&) {
        return std::make_unique<dsess::WorkerPool>([]() -> std::shared_ptr<dsess::Session> { throw ToolError("dosbox", 1, "", "no boot"); }, 1);
    };
    masm::Assembler assembler(context);
    CB_CHECK_EQ(text_of(assembler.assemble_many(sources({{"F0", "ab"}})).results.at(0).second.obj), std::string("ba"));
    CB_CHECK_EQ(rig.runner.specs.size(), std::size_t{1});
});

// ---- the options, the tools ------------------------------------------------------------------------------------------------------

CB_TEST("build.masm.options_come_from_the_environment_with_the_defaults_when_it_is_silent_or_wrong", [] {
    {
        fs::ScopedEnv a("WT_DOSBOX_CORE", std::nullopt), b("WT_POOL_MAX", std::nullopt), c("WT_NO_WARM_POOL", std::nullopt),
            d("WT_WORKER_MAX_USES", std::nullopt), e("WT_ASM_BATCH", std::nullopt);
        const masm::Options o = masm::Options::from_environment();
        CB_CHECK_EQ(o.dosbox_core, std::string("dynamic"));
        CB_CHECK_EQ(o.pool_max, std::size_t{3});
        CB_CHECK(o.warm_pool);
        CB_CHECK_EQ(o.worker_max_uses, std::size_t{0});
        CB_CHECK_EQ(o.batch, std::size_t{40});
    }
    {
        fs::ScopedEnv a("WT_DOSBOX_CORE", std::string_view("normal")), b("WT_POOL_MAX", std::string_view("6")),
            c("WT_NO_WARM_POOL", std::string_view("1")), d("WT_WORKER_MAX_USES", std::string_view("25")),
            e("WT_ASM_BATCH", std::string_view("10"));
        const masm::Options o = masm::Options::from_environment();
        CB_CHECK_EQ(o.dosbox_core, std::string("normal"));
        CB_CHECK_EQ(o.pool_max, std::size_t{6});
        CB_CHECK(!o.warm_pool);
        CB_CHECK_EQ(o.worker_max_uses, std::size_t{25});
        CB_CHECK_EQ(o.batch, std::size_t{10});
    }
    {   // below the least is the least; text that is not a number is the default
        fs::ScopedEnv b("WT_POOL_MAX", std::string_view("0")), e("WT_ASM_BATCH", std::string_view("lots")),
            c("WT_NO_WARM_POOL", std::string_view("0"));
        const masm::Options o = masm::Options::from_environment();
        CB_CHECK_EQ(o.pool_max, std::size_t{1});
        CB_CHECK_EQ(o.batch, std::size_t{40});
        CB_CHECK(o.warm_pool);
    }
});

CB_TEST("build.masm.tools_come_from_the_settings_and_the_environment", [] {
    CB_WINDOWS_ONLY("it names Windows drive paths");
    fs::ScopedTempDir dir("masm");
    fs::write_text(dir.path() / "settings.json", "{\"masm\": \"M:/masm\", \"dosbox_x\": \"D:/dosbox.exe\"}");
    const auto config = config::Config::load(dir.path());
    {
        fs::ScopedEnv none("CB_NO_DOSBOX", std::nullopt);
        const masm::Tools t = masm::Tools::from_config(config);
        CB_CHECK(t.masm == stdfs::path("M:/masm"));
        CB_CHECK(t.dosbox_x == stdfs::path("D:/dosbox.exe"));
        CB_CHECK(!t.dosbox_forbidden);
    }
    fs::ScopedEnv no("CB_NO_DOSBOX", std::string_view("1"));
    CB_CHECK(masm::Tools::from_config(config).dosbox_forbidden);
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
    masm::Context with{context.tools, context.options, dos, rig.runner, nullptr, nullptr};
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
    masm::Context good{rig.tools, rig.options, dos, rig.runner, nullptr, nullptr};
    masm::Assembler assembler(good);
    std::string report;
    CB_CHECK(masm::selftest(assembler, report));
    CB_CHECK(report.find("functions: _ADDASM=11\n") != std::string::npos);
    CB_CHECK(report.find("_addasm bytes: 558bec8b46040346065dc3\n") != std::string::npos);
    CB_CHECK(report.find("SELFTEST: PASS - MASM assembled byte-exact proc") != std::string::npos);
    SelfTestDos wrong;
    wrong.code = {0x55, 0x8B, 0xEC, 0x8B, 0x46, 0x04, 0x01, 0x46, 0x06, 0x5D, 0xC3};   // add the other way: not the bytes
    masm::Context bad{rig.tools, rig.options, wrong, rig.runner, nullptr, nullptr};
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
    const masm::Context context{rig.tools, rig.options, dos, rig.runner, nullptr, nullptr};
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
    const masm::Context context{rig.tools, rig.options, dos, rig.runner, nullptr, nullptr};
    const cli::Cli app("codebreaker", {masm::command(context)});
    testing::CapturedStd captured;
    CB_CHECK_EQ(app.run({"masm", "--selftest"}), 0);
    CB_CHECK(captured.out().find("SELFTEST: PASS") != std::string::npos);
});

CB_TEST("build.masm.the_command_lists_the_segments_of_a_file_it_assembled", [] {
    Rig rig;
    SelfTestDos dos;
    fs::write_text(rig.dir.path() / "p.asm", std::string(masm::kSelfTestSource));
    const masm::Context context{rig.tools, rig.options, dos, rig.runner, nullptr, nullptr};
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
