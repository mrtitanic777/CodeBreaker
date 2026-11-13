// cb-unity: alone (test helpers named for this module)
// Tests of cb/store/migrate_state: a state folder as the Python-driven program left it (a function store whose verdicts are filed under digests of
// its own settings texts, a knowledge base of its own file) is read by this program after the migration as the Python read it.
//
// The settings texts below are the forms the Python writes: a JSON object for the 32-bit lane, the repr of a tuple for the 16-bit, DOS and cartridge
// lanes (codebreaker/verify/lanes/*.py: settings_profile, profile). The current settings are in this program's own text (verify/lanes/*.cpp: profile).
#include "cb/store/migrate_state.hpp"

#include <algorithm>
#include <format>

#include "cb/base/sqlite.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/engine/function_store.hpp"
#include "cb/knowledge/store.hpp"
#include "cb/store/functions.hpp"

namespace mg = cb::store::migrate;
namespace sql = cb::base::sqlite;
namespace stdfs = std::filesystem;
using cb::engine::FunctionStore;
using cb::engine::Record;
using cb::verify::Class;

namespace {

struct NoStateEnv {
    cb::fs::ScopedEnv functions{"CB_FUNCTIONS_DB", std::nullopt};
    cb::fs::ScopedEnv knowledge{"CB_KNOWLEDGE_DB", std::nullopt};
};

// The Python's settings texts.
const char* const kPyPe48 =
    R"({"cflags": ["-bt=nt", "-zq"], "compiler": "wcc386.exe:174080:1789307814", "confirm_inputs": [[1, 2, 3, 4]], "directed": [16, 2], )"
    R"("guided": [1, 2], "inputs": [[0, 0, 0, 0]], "lane": "pe", "max_calls": 64, "max_support": 1500, "oracle": 25, "paths": [true, 3, 4, 5, ""], "pipeline": 48, "runtime": []})";
const char* const kPyPe23 =
    R"({"cflags": ["-bt=nt", "-zq"], "compiler": "wcc386.exe:174080:1789307814", "inputs": [[0, 0, 0, 0]], "lane": "pe", "oracle": 17, "pipeline": 23})";
const char* const kPyNe23 = "('ne', 23, 25, ['-bt=windows', '-ms'], [{'args': [0, 0]}, {'args': [1, 2]}], ('guided', 3, 4), ('paths', True, 1, 2, ''))";
const char* const kPyDos93 = "('dos', 93, ['wasm:269824:1790025122', 'wlink:119808:1790043671'])";
const char* const kPyNes2 = "('nes', 2, [{'a': 0, 'x': 0, 'y': 0, 'fill': 0}, {'a': 1, 'x': 2, 'y': 3, 'fill': 1}])";
const char* const kPyGb1 = "('gb', 1, [{'a': 0, 'b': 0, 'fill': 0}, {'a': 1, 'b': 2, 'fill': 1}])";
const char* const kPyLe3 = R"({"cap": 10000, "comparer": 17, "inputs": [51, 18, 5, 11], "judge": 3, "kind": "le", "tools": ["wasm.exe:269824:1790025122"]})";
const char* const kNotPython = "win32 rev 48 compare 25 max_calls 64";   // (a text of this program's: no older writer's)

// This program's settings texts for the same kinds, as the lanes write them (the digests are of whatever text: any twelve characters do).
std::map<std::string, mg::Verifier> current() {
    return {{"pe", {"aaaaaaaaaaaa", "win32 rev 48 compare 25 max_calls 64 paths true 3 4 5 lifter.exe:9:8 inputs [0 0 0 0] confirm [1 2 3 4] cflags -bt=nt -zq compiler wcc386.exe:174080:133"}},
            {"ne", {"bbbbbbbbbbbb", "kind=ne judge=24 comparer=25 options=[-bt=windows] inputs=[0 0 | 1 2 |] guided=3,4 paths=true,1,2,stamp"}},
            {"dos", {"cccccccccccc", "dos rev 93 tools wasm:269824:999 wlink:119808:888"}},
            {"nes", {"dddddddddddd", "nes rev 2 states [{a=0, x=0, y=0, fill=0}, {a=1, x=2, y=3, fill=1}]"}},
            {"gb", {"eeeeeeeeeeee", "gb rev 1 states [{a=0, b=0, fill=0}, {a=1, b=9, fill=1}]"}}};
}

Record judged(std::string addr, std::string verdict) {
    Record r;
    r.addr = addr;
    r.name = "f_" + addr;
    r.source = "int f_" + addr + "(void) { return 0; }";
    r.sig = "sig-" + addr;
    r.verdict = std::move(verdict);
    return r;
}

// A function store with some modules judged under profiles an older writer made.
struct OlderStore {
    explicit OlderStore(const stdfs::path& state) : store(cb::store::functions::open(state)) {
        for (const char* settings : {kPyPe48, kPyPe23, kPyNe23, kPyDos93, kPyNes2, kPyGb1, kPyLe3, kNotPython}) profile(settings);
        // an older profile nothing is filed under: not reported
        store->add_profile("0000000000ff", "('zx', 2, [])");
    }

    void profile(const char* settings) { store->add_profile(digest_of(settings), settings); }

    // module `sha` judged under the digest of `settings`
    void judge(const std::string& sha, const char* settings, const std::vector<std::pair<std::string, std::string>>& verdicts) {
        const std::string digest = digest_of(settings);
        store->add_module(sha, sha + ".dll", "", "pe");
        std::vector<std::string> addrs;
        std::vector<Record> records;
        for (const auto& [addr, verdict] : verdicts) {
            addrs.push_back(addr);
            records.push_back(judged(addr, verdict));
        }
        store->add_functions(sha, addrs);
        store->record_many(sha, "decompile", "r1", digest, records);
        std::map<std::string, std::string> built;
        for (const std::string& a : addrs) built[a] = "sig-" + a;
        store->set_built(sha, digest, built);
    }

    static std::string digest_of(const char* settings) { return std::format("{:012x}", std::hash<std::string>{}(settings) & 0xffffffffffffull); }

    std::unique_ptr<FunctionStore> store;
};

mg::Services services_for(const stdfs::path& state) { return mg::Services{.state_dir = state, .verifier_profiles = [] { return current(); }}; }

const mg::ProfileMove* move_of(const mg::Report& report, const std::string& digest) {
    for (const mg::ProfileMove& m : report.verdicts.profiles)
        if (m.from == digest) return &m;
    return nullptr;
}

std::int64_t verified_under(FunctionStore& store, const std::string& sha, const std::string& profile) {
    return store.tally(profile, sha).at(Class::Verified);
}

}  // namespace

CB_TEST("store.migrate_state.verdicts_follow_the_settings_that_are_the_same", [] {
    NoStateEnv env;
    cb::fs::ScopedTempDir dir("cbmigrate");
    const stdfs::path state = dir.path() / "state";
    {
        OlderStore older(state);
        older.judge("mod1", kPyPe48, {{"1000", "equivalent"}, {"1010", "equivalent"}, {"1020", "divergent"}});
        older.judge("mod2", kPyPe48, {{"2000", "equivalent"}});
        older.judge("mod1", kPyPe23, {{"1000", "divergent"}});               // an older judge's word on the same function
        older.judge("mod3", kPyNe23, {{"3000", "equivalent"}});
        older.judge("mod4", kPyDos93, {{"4000", "equivalent"}, {"4010", "equivalent"}});
        older.judge("mod5", kPyNes2, {{"5000", "equivalent"}});
        older.judge("mod6", kPyGb1, {{"6000", "equivalent"}});
        older.judge("mod7", kPyLe3, {{"7000", "equivalent"}});
    }

    // dry: says what it would do, writes nothing
    const mg::Report dry = mg::migrate(services_for(state), {.dry = true});
    CB_CHECK(dry.dry);
    CB_CHECK(dry.verdicts.store_found);
    const mg::ProfileMove* pe = move_of(dry, OlderStore::digest_of(kPyPe48));
    CB_CHECK(pe != nullptr);
    CB_CHECK_EQ(pe->to, "aaaaaaaaaaaa");
    CB_CHECK_EQ(pe->modules, 2);
    CB_CHECK_EQ(pe->found, 4);
    CB_CHECK_EQ(pe->carried, 4);
    CB_CHECK_EQ(pe->built, 4);
    {
        auto store = cb::store::functions::open(state, std::nullopt, true);
        CB_CHECK_EQ(verified_under(*store, "mod1", "aaaaaaaaaaaa"), 0);
        CB_CHECK(store->summaries().at("mod1").profiles.size() == 2);   // the two older ones
    }

    // for real
    const mg::Report done = mg::migrate(services_for(state), {});
    CB_CHECK(!done.dry);
    {
        auto store = cb::store::functions::open(state, std::nullopt, true);
        // what the Python saw under its digest, this program sees under its own
        CB_CHECK_EQ(verified_under(*store, "mod1", "aaaaaaaaaaaa"), verified_under(*store, "mod1", OlderStore::digest_of(kPyPe48)));
        CB_CHECK_EQ(verified_under(*store, "mod1", "aaaaaaaaaaaa"), 2);
        CB_CHECK_EQ(verified_under(*store, "mod2", "aaaaaaaaaaaa"), 1);
        CB_CHECK_EQ(store->tally("aaaaaaaaaaaa", "mod1").at(Class::Unverified), 1);
        CB_CHECK(store->built("mod1", "aaaaaaaaaaaa").size() == 3);
        // the dos tool's size is the same, its time is not a difference; the cartridge states are the same numbers
        CB_CHECK_EQ(verified_under(*store, "mod4", "cccccccccccc"), 2);
        CB_CHECK_EQ(verified_under(*store, "mod5", "dddddddddddd"), 1);
        // not the same verifier: the 16-bit judge is of another revision, the older 32-bit judge too, a cartridge with other inputs, a driver with no
        // verifier here
        CB_CHECK_EQ(verified_under(*store, "mod3", "bbbbbbbbbbbb"), 0);
        CB_CHECK_EQ(verified_under(*store, "mod6", "eeeeeeeeeeee"), 0);
        CB_CHECK_EQ(store->tally("aaaaaaaaaaaa", "mod1").at(Class::Verified) + 0, 2);   // (the older judge's divergent did not come over)
        // the older digests are still where they were
        CB_CHECK_EQ(verified_under(*store, "mod4", OlderStore::digest_of(kPyDos93)), 2);
        // the summary lists the new digest and the profile the module was last judged under
        const cb::engine::Summary summary = store->summaries().at("mod1");
        CB_CHECK(std::find(summary.profiles.begin(), summary.profiles.end(), "aaaaaaaaaaaa") != summary.profiles.end());
        // the index knows the settings of the new digest
        sql::Database index = sql::Database::open_readonly(store->path());
        CB_CHECK_EQ(std::get<std::string>(index.query("SELECT settings FROM profile WHERE digest='aaaaaaaaaaaa'")[0][0]), current().at("pe").settings);
    }
    const mg::ProfileMove* old_pe = move_of(done, OlderStore::digest_of(kPyPe23));
    CB_CHECK(old_pe != nullptr && old_pe->to.empty());
    CB_CHECK(old_pe->why_not.find("judge revision 23, now 48") != std::string::npos);
    const mg::ProfileMove* ne = move_of(done, OlderStore::digest_of(kPyNe23));
    CB_CHECK(ne != nullptr && ne->to.empty());
    CB_CHECK(ne->why_not.find("judge revision 23, now 24") != std::string::npos);
    const mg::ProfileMove* gb = move_of(done, OlderStore::digest_of(kPyGb1));
    CB_CHECK(gb != nullptr && gb->to.empty());
    CB_CHECK(gb->why_not.find("inputs") != std::string::npos);
    const mg::ProfileMove* le = move_of(done, OlderStore::digest_of(kPyLe3));
    CB_CHECK(le != nullptr && le->to.empty());
    CB_CHECK(le->why_not.find("no verifier for le") != std::string::npos);
    CB_CHECK(move_of(done, "0000000000ff") == nullptr);                            // nothing filed under it
    CB_CHECK(move_of(done, OlderStore::digest_of(kNotPython)) == nullptr);         // not an older writer's text

    // again: nothing left to carry, nothing changed
    const mg::Report again = mg::migrate(services_for(state), {});
    const mg::ProfileMove* pe_again = move_of(again, OlderStore::digest_of(kPyPe48));
    CB_CHECK(pe_again != nullptr);
    CB_CHECK_EQ(pe_again->carried, 0);
    CB_CHECK_EQ(pe_again->present, 4);
    CB_CHECK_EQ(again.verdicts.modules_changed, 0);
});

CB_TEST("store.migrate_state.a_verdict_this_program_reached_is_never_replaced", [] {
    NoStateEnv env;
    cb::fs::ScopedTempDir dir("cbmigrate");
    const stdfs::path state = dir.path() / "state";
    {
        OlderStore older(state);
        older.judge("mod1", kPyPe48, {{"1000", "equivalent"}, {"1010", "equivalent"}});
        // this program judged 1000 itself, and differently
        std::vector<Record> mine = {judged("1000", "divergent")};
        older.store->record_many("mod1", "decompile", "r1", "aaaaaaaaaaaa", mine);
    }
    const mg::Report report = mg::migrate(services_for(state), {});
    const mg::ProfileMove* pe = move_of(report, OlderStore::digest_of(kPyPe48));
    CB_CHECK(pe != nullptr);
    CB_CHECK_EQ(pe->carried, 1);   // only 1010
    CB_CHECK_EQ(pe->present, 1);
    auto store = cb::store::functions::open(state, std::nullopt, true);
    const auto best = store->best("mod1", "aaaaaaaaaaaa");
    CB_CHECK_EQ(best.at("1000").cls, Class::Unverified);
    CB_CHECK_EQ(best.at("1010").cls, Class::Verified);
});

CB_TEST("store.migrate_state.without_verifier_settings_nothing_is_carried_and_nothing_fails", [] {
    NoStateEnv env;
    cb::fs::ScopedTempDir dir("cbmigrate");
    const stdfs::path state = dir.path() / "state";
    {
        OlderStore older(state);
        older.judge("mod1", kPyPe48, {{"1000", "equivalent"}});
    }
    mg::Services none{.state_dir = state, .verifier_profiles = {}};
    const mg::Report report = mg::migrate(none, {});
    const mg::ProfileMove* pe = move_of(report, OlderStore::digest_of(kPyPe48));
    CB_CHECK(pe != nullptr && pe->to.empty());
    CB_CHECK_EQ(report.verdicts.modules_changed, 0);
    // and no state folder at all
    const mg::Report empty = mg::migrate(mg::Services{.state_dir = dir.path() / "nowhere"}, {});
    CB_CHECK(!empty.verdicts.store_found);
    CB_CHECK(!empty.knowledge.found);
    CB_CHECK(!stdfs::exists(dir.path() / "nowhere"));
});

namespace {

// The Python's knowledge base, in the shape its file has (tag J: JSON, S: text, P: a pickle).
void make_older_knowledge(const stdfs::path& file) {
    sql::Database db = sql::Database::open(file);
    for (const char* ddl : {"CREATE TABLE knowledge(platform TEXT NOT NULL, kind TEXT NOT NULL, key TEXT NOT NULL, seq INTEGER, hash TEXT NOT NULL, tag TEXT NOT NULL, "
                            "value BLOB, source TEXT, origin TEXT NOT NULL, made REAL, PRIMARY KEY (platform, kind, key))",
                            "CREATE TABLE loaded(platform TEXT NOT NULL, kind TEXT NOT NULL, stamp TEXT, source TEXT, made REAL, PRIMARY KEY (platform, kind))",
                            "CREATE TABLE module(sha TEXT PRIMARY KEY, name TEXT, platform TEXT)",
                            "CREATE TABLE body(module TEXT NOT NULL, addr TEXT NOT NULL, hash TEXT NOT NULL, name TEXT, source TEXT, PRIMARY KEY (module, addr))",
                            "CREATE TABLE body_name(hash TEXT PRIMARY KEY, name TEXT, source TEXT, ts REAL)",
                            "CREATE TABLE carried(what TEXT PRIMARY KEY, mark TEXT)"})
        db.exec(ddl);
    const auto row = [&](const char* platform, const char* kind, const char* key, const char* tag, const cb::Bytes& value, const char* origin) {
        db.exec("INSERT INTO knowledge(platform, kind, key, seq, hash, tag, value, source, origin, made) VALUES(?,?,?,NULL,'h',?,?,'older',?,1.0)",
                {std::string(platform), std::string(kind), std::string(key), std::string(tag), value, std::string(origin)});
    };
    const auto bytes = [](std::string_view s) { return cb::Bytes(s.begin(), s.end()); };
    row("win32", "argument_bytes.code", "USER32.FindWindowA", "J", bytes("8"), "learned");
    row("win32", "argument_bytes.code", "USER32.GetFoo", "J", bytes("null"), "learned");          // an export whose code says nothing
    row("win32", "argument_bytes.code", "KERNEL32.Held", "J", bytes("4"), "learned");              // this program found it too
    row("win32", "argument_bytes.code", "KERNEL32.Pickled", "P", bytes("\x80\x04K\x04."), "learned");
    row("win16", "argument_bytes", "win95:0d9b19e201|USER.430", "J", bytes("8"), "learned");
    row("win32", "prototype", "GetDlgItem", "J", bytes("{\"name\":\"GetDlgItem\"}"), "derived");
    row("win32", "struct", "RECT", "P", bytes("\x80\x04K\x01."), "derived");
    row("windows", "ordinal_names", "kernel32.1", "J", bytes("\"A\""), "authored");
    db.exec("INSERT INTO module VALUES('modA','a.dll','pe'),('modB','b.dll','pe'),('modEmpty','e.dll','pe')");
    db.exec("INSERT INTO body VALUES('modA','1000','d1','_one','export'),('modA','1010','d2',NULL,NULL),('modB','2000','d1',NULL,NULL),('modB','2010','d3','_three','debug')");
    db.exec("INSERT INTO body_name VALUES('d3','three','named by hand',1.0),('d9','nine','named by hand',2.0)");
    db.exec("INSERT INTO carried VALUES('function store x','0:0:4')");
}

}  // namespace

CB_TEST("store.migrate_state.the_knowledge_base_comes_over_without_replacing_what_is_held", [] {
    NoStateEnv env;
    cb::fs::ScopedTempDir dir("cbmigrate");
    const stdfs::path state = dir.path() / "state";
    const stdfs::path older_file = state / "db" / "knowledge.sqlite";
    make_older_knowledge(older_file);
    const cb::Bytes before = cb::fs::read_file(older_file);
    const stdfs::path file = cb::knowledge::Store::default_path(state);
    CB_CHECK(file != older_file);
    {
        // this program has indexed modB itself, named a body of its own, and found one export's size
        cb::knowledge::Store kb(file, cb::knowledge::DataFolders::from_environment());
        kb.add_module("modB", "b.dll", "pe");
        const std::vector<cb::knowledge::BodyRow> mine = {{"2000", "dX", std::nullopt, std::nullopt}};
        kb.set_bodies("modB", mine);
        kb.name_body("d9", "mine", "named by hand");
        kb.put("win32", "argument_bytes.code", "KERNEL32.Held", cb::knowledge::Value(12), "this program's code", cb::knowledge::Origin::Learned);
    }

    // dry: counts, writes nothing
    const mg::Report dry = mg::migrate(mg::Services{.state_dir = state}, {.dry = true});
    CB_CHECK(dry.knowledge.found);
    CB_CHECK_EQ(dry.knowledge.modules, 1);          // modA
    CB_CHECK_EQ(dry.knowledge.modules_present, 1);  // modB stays as this program has it
    CB_CHECK_EQ(dry.knowledge.bodies, 2);
    CB_CHECK_EQ(dry.knowledge.names, 1);            // d3
    CB_CHECK_EQ(dry.knowledge.names_present, 1);    // d9
    CB_CHECK_EQ(dry.knowledge.learned, 2);          // FindWindowA and GetFoo (the null)
    CB_CHECK_EQ(dry.knowledge.learned_present, 1);  // KERNEL32.Held
    {
        cb::knowledge::Store kb(file, cb::knowledge::DataFolders::from_environment());
        CB_CHECK(kb.bodies("modA").empty());
        CB_CHECK(!kb.has("win32", "argument_bytes.code", "USER32.FindWindowA"));
    }

    const mg::Report done = mg::migrate(mg::Services{.state_dir = state}, {});
    CB_CHECK_EQ(done.knowledge.modules, 1);
    CB_CHECK_EQ(done.knowledge.bodies, 2);
    {
        cb::knowledge::Store kb(file, cb::knowledge::DataFolders::from_environment());
        // the bodies and their own names
        const std::vector<cb::knowledge::BodyRow> a = kb.bodies("modA");
        CB_CHECK_EQ(a.size(), std::size_t{2});
        CB_CHECK(a[0].addr == "1000" && a[0].digest == "d1" && a[0].name == std::optional<std::string>("_one") && a[0].source == std::optional<std::string>("export"));
        CB_CHECK(!a[1].name.has_value());
        // modB: as this program had it
        CB_CHECK_EQ(kb.bodies("modB").size(), std::size_t{1});
        CB_CHECK_EQ(kb.bodies("modB")[0].digest, "dX");
        // given names: carried, and this program's own stands
        CB_CHECK_EQ(kb.name("d3").value().name, "three");
        CB_CHECK_EQ(kb.name("d9").value().name, "mine");
        // a body modA names that modB has no name for: shared names read through (d1 is named _one in modA)
        // learned rows
        CB_CHECK(kb.get("win32", "argument_bytes.code", "USER32.FindWindowA") == std::optional<cb::knowledge::Value>(cb::knowledge::Value(8)));
        CB_CHECK(kb.get("win32", "argument_bytes.code", "USER32.GetFoo")->is_null());
        CB_CHECK(kb.get("win32", "argument_bytes.code", "KERNEL32.Held") == std::optional<cb::knowledge::Value>(cb::knowledge::Value(12)));
        CB_CHECK(!kb.has("win32", "argument_bytes.code", "KERNEL32.Pickled"));
        // not carried: the 16-bit table (keyed by the Python's reading), what is made again
        CB_CHECK(!kb.has("win16", "argument_bytes", "win95:0d9b19e201|USER.430"));
        CB_CHECK(!kb.has("win32", "prototype", "GetDlgItem"));
        const auto provenance = kb.provenance("win32", "argument_bytes.code", "USER32.FindWindowA");
        CB_CHECK(provenance.has_value());
    }
    bool left_win16 = false, left_authored = false, left_pickle = false;
    for (const mg::Left& l : done.knowledge.left) {
        left_win16 = left_win16 || (l.what == "learned win16 argument_bytes" && l.rows == 1);
        left_authored = left_authored || (l.what == "authored rows" && l.rows == 1);
        left_pickle = left_pickle || (l.what == "learned rows that are not JSON or text" && l.rows == 1);
    }
    CB_CHECK(left_win16 && left_authored && left_pickle);
    // the Python's file is as it was
    CB_CHECK(cb::fs::read_file(older_file) == before);

    // again: nothing more
    const mg::Report again = mg::migrate(mg::Services{.state_dir = state}, {});
    CB_CHECK_EQ(again.knowledge.modules, 0);
    CB_CHECK_EQ(again.knowledge.modules_present, 2);
    CB_CHECK_EQ(again.knowledge.names, 0);
    CB_CHECK_EQ(again.knowledge.learned, 0);
    CB_CHECK_EQ(again.knowledge.learned_present, 3);
    CB_CHECK(cb::fs::read_file(older_file) == before);
});

CB_TEST("store.migrate_state.a_dry_run_makes_no_knowledge_base_and_one_named_for_both_is_left", [] {
    NoStateEnv env;
    cb::fs::ScopedTempDir dir("cbmigrate");
    const stdfs::path state = dir.path() / "state";
    make_older_knowledge(state / "db" / "knowledge.sqlite");
    const mg::Report dry = mg::migrate(mg::Services{.state_dir = state}, {.dry = true});
    CB_CHECK_EQ(dry.knowledge.modules, 2);
    CB_CHECK_EQ(dry.knowledge.bodies, 4);
    CB_CHECK_EQ(dry.knowledge.names, 2);
    CB_CHECK_EQ(dry.knowledge.learned, 3);
    CB_CHECK(!stdfs::exists(cb::knowledge::Store::default_path(state)));
    // CB_KNOWLEDGE_DB naming the Python's file: both are one file, nothing to carry
    cb::fs::ScopedEnv same("CB_KNOWLEDGE_DB", cb::fs::to_text(state / "db" / "knowledge.sqlite"));
    const mg::Report one = mg::migrate(mg::Services{.state_dir = state}, {});
    CB_CHECK(one.knowledge.found && one.knowledge.same_file);
    CB_CHECK_EQ(one.knowledge.modules, 0);
});

CB_TEST("store.migrate_state.the_command_runs_dry_and_takes_a_state_folder", [] {
    NoStateEnv env;
    cb::fs::ScopedTempDir dir("cbmigrate");
    const stdfs::path state = dir.path() / "state";
    {
        OlderStore older(state);
        older.judge("mod1", kPyPe48, {{"1000", "equivalent"}});
    }
    cb::cli::Cli cli("codebreaker", {mg::command(mg::Services{.state_dir = dir.path() / "elsewhere", .verifier_profiles = [] { return current(); }})});
    const cb::cli::Cli::Parsed parsed = cli.parse({"migrate-state", "--dry", "--state", cb::fs::to_text(state)});
    CB_CHECK(parsed.command != nullptr && !parsed.help);
    CB_CHECK_EQ(parsed.command->handler(parsed.args), cb::kExitOk);
    auto store = cb::store::functions::open(state, std::nullopt, true);
    CB_CHECK_EQ(verified_under(*store, "mod1", "aaaaaaaaaaaa"), 0);   // (dry)
    CB_CHECK_EQ(parsed.command->handler(cli.parse({"migrate-state", "--state", cb::fs::to_text(state)}).args), cb::kExitOk);
    CB_CHECK_EQ(verified_under(*store, "mod1", "aaaaaaaaaaaa"), 1);
    // the lines say what happened
    const mg::Report report = mg::migrate(services_for(state), {.dry = true});
    const std::vector<std::string> lines = mg::render(report);
    CB_CHECK(!lines.empty());
    bool names_profile = false;
    for (const std::string& line : lines) names_profile = names_profile || line.find("aaaaaaaaaaaa") != std::string::npos;
    CB_CHECK(names_profile);
});

CB_TEST("store.migrate_state.a_judge_that_explores_paths_is_not_one_that_does_not", [] {
    NoStateEnv env;
    cb::fs::ScopedTempDir dir("cbmigrate");
    const stdfs::path state = dir.path() / "state";
    {
        OlderStore older(state);
        older.judge("mod1", kPyPe48, {{"1000", "equivalent"}});
    }
    // the older verdicts were reached with the branch-guided exploration on (kPyPe48's "paths": [true, ...]); a verifier without lifters guesses inputs
    const auto settings_of_pe = [](const char* text) {
        return [text] {
            std::map<std::string, mg::Verifier> v = current();
            v.at("pe").settings = text;
            return v;
        };
    };
    mg::Services s{.state_dir = state, .verifier_profiles = settings_of_pe("win32 rev 48 compare 25 paths false 3 4 5 inputs [0 0 0 0] compiler wcc386.exe:174080:133")};
    const mg::Report report = mg::migrate(s, {});
    const mg::ProfileMove* pe = move_of(report, OlderStore::digest_of(kPyPe48));
    CB_CHECK(pe != nullptr && pe->to.empty());
    CB_CHECK(pe->why_not.find("explore") != std::string::npos);
    CB_CHECK_EQ(report.verdicts.modules_changed, 0);
    // other limits for the same exploration: not the same settings either
    s.verifier_profiles = settings_of_pe("win32 rev 48 compare 25 paths true 9 4 5 stamp inputs [0 0 0 0] compiler wcc386.exe:174080:133");
    CB_CHECK(move_of(mg::migrate(s, {}), OlderStore::digest_of(kPyPe48))->why_not.find("limits") != std::string::npos);
    // a compiler of another size
    s.verifier_profiles = settings_of_pe("win32 rev 48 compare 25 paths true 3 4 5 stamp inputs [0 0 0 0] compiler wcc386.exe:999:133");
    CB_CHECK(move_of(mg::migrate(s, {}), OlderStore::digest_of(kPyPe48))->why_not.find("tool") != std::string::npos);
});
