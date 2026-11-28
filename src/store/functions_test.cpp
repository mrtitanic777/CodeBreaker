// cb-unity: alone (test helpers named for this module)
// Tests of cb/store/functions (where the function store is and how it is opened) and cb/store/versions_ledger (what `codebreaker versions` asks of
// the corpus, the store and the body store), on a store made in a temporary folder.
//
// Design notes: the verdict vocabulary (ORDER, TITLE, WEAK, IDENTIFIED, SCORED, classify) is verify/verdicts's and is
// not repeated here; `latest` answers classes, not their words;
// the state folder is an argument (nothing is read from settings), and opening a store for writing makes its folders (asking for the path alone makes nothing).
#include "store/functions.hpp"

#include "base/corpus.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "store/bodies.hpp"
#include "compare/versions_ledger.hpp"

namespace fn = cb::store::functions;
namespace ve = cb::compare::versions;
using cb::store::FunctionStore;
using cb::store::Record;
using cb::store::Class;
namespace stdfs = std::filesystem;

namespace {

struct NoEnv {
    cb::fs::ScopedEnv unset{"CB_FUNCTIONS_DB", std::nullopt};
};

Record record_of(std::string addr, std::string name, std::string source, std::string verdict) {
    Record r;
    r.addr = std::move(addr);
    r.name = std::move(name);
    r.source = std::move(source);
    r.sig = "sig-" + r.addr;
    r.verdict = std::move(verdict);
    return r;
}

}  // namespace

CB_TEST("store.functions.the_store_of_the_state_folder_keeps_its_databases_in_db", [] {
    NoEnv env;
    cb::fs::ScopedTempDir dir("cbstorefunctions");
    const stdfs::path state = dir.path() / "state";
    CB_CHECK(fn::default_path(state) == state / "functions.sqlite");
    CB_CHECK(fn::databases_folder(state) == state / "db");
    // a path of its own: beside it, db for functions.sqlite (in any case), <name>.db for any other name
    CB_CHECK(fn::databases_folder(state, dir.path() / "mine" / "functions.sqlite") == dir.path() / "mine" / "db");
    CB_CHECK(fn::databases_folder(state, dir.path() / "mine" / "FUNCTIONS.SQLITE") == dir.path() / "mine" / "db");
    CB_CHECK(fn::databases_folder(state, dir.path() / "x.sqlite") == dir.path() / "x.db");
    CB_CHECK(fn::databases_folder(state, dir.path() / "x.sqlite", true) == dir.path() / "x.db");
    CB_CHECK(fn::databases_folder(state, dir.path() / "a.b.sqlite", true) == dir.path() / "a.b.db");
    // told it is not one of its own, the path is ignored
    CB_CHECK(fn::databases_folder(state, dir.path() / "x.sqlite", false) == state / "db");
    // an empty path is no path
    CB_CHECK(fn::databases_folder(state, stdfs::path()) == state / "db");
});

CB_TEST("store.functions.the_environment_names_a_store_of_its_own", [] {
    cb::fs::ScopedTempDir dir("cbstorefunctions");
    const stdfs::path state = dir.path() / "state";
    const stdfs::path own = dir.path() / "elsewhere" / "scores.sqlite";
    cb::fs::ScopedEnv env("CB_FUNCTIONS_DB", cb::fs::to_text(own));
    CB_CHECK(fn::default_path(state) == own);
    CB_CHECK(fn::databases_folder(state) == dir.path() / "elsewhere" / "scores.db");
    cb::fs::ScopedEnv empty("CB_FUNCTIONS_DB", std::string_view(""));   // set but empty is not set
    CB_CHECK(fn::default_path(state) == state / "functions.sqlite");
    CB_CHECK(fn::databases_folder(state) == state / "db");
});

CB_TEST("store.functions.a_store_that_is_not_there_reads_as_empty", [] {
    NoEnv env;
    cb::fs::ScopedTempDir dir("cbstorefunctions");
    const stdfs::path state = dir.path() / "state";
    CB_CHECK(fn::open(state, std::nullopt, /*readonly=*/true) == nullptr);
    CB_CHECK(fn::overview(state).empty());
    CB_CHECK(fn::latest(state, "no-such-module").empty());
    CB_CHECK(!stdfs::exists(state));   // asking made nothing
});

CB_TEST("store.functions.the_overview_and_the_latest_classes_are_read_from_the_store", [] {
    NoEnv env;
    cb::fs::ScopedTempDir dir("cbstorefunctions");
    const stdfs::path state = dir.path() / "state";
    {
        const auto store = fn::open(state);   // for writing: the files and folders are made
        CB_CHECK(store != nullptr);
        store->add_module("sha-one", "alpha.dll", "C:/alpha.dll", "pe");
        store->add_profile("p", "{}");
        store->add_functions("sha-one", std::vector<std::string>{"10", "20", "30"});
        const std::vector<Record> rows = {record_of("10", "_a", "int a(){}", "equivalent"), record_of("20", "_b", "int b(){}", "divergent")};
        store->record_many("sha-one", "decompile", "r1", "p", rows);
    }
    CB_CHECK(stdfs::is_regular_file(state / "functions.sqlite"));
    CB_CHECK(stdfs::is_directory(state / "db"));
    const auto glance = fn::overview(state);
    CB_CHECK_EQ(glance.size(), 1U);
    CB_CHECK_EQ(glance[0].name, "alpha.dll");
    CB_CHECK_EQ(glance[0].kind, "pe");
    CB_CHECK_EQ(glance[0].functions, 3);
    CB_CHECK_EQ(glance[0].latest.at(Class::Verified), 1);
    CB_CHECK_EQ(glance[0].latest.at(Class::Unverified), 1);
    CB_CHECK_EQ(glance[0].latest.at(Class::NoSource), 1);   // 30 was never made anything of
    const auto classes = fn::latest(state, "sha-one");
    CB_CHECK_EQ(classes.size(), 2U);
    CB_CHECK(classes.at("10") == Class::Verified);
    CB_CHECK(classes.at("20") == Class::Unverified);
    CB_CHECK(fn::latest(state, "another").empty());
});

CB_TEST("store.functions.a_store_of_its_own_is_found_by_its_path", [] {
    NoEnv env;
    cb::fs::ScopedTempDir dir("cbstorefunctions");
    const stdfs::path state = dir.path() / "state";
    const stdfs::path own = dir.path() / "scratch" / "mine.sqlite";
    {
        const auto store = fn::open(state, own);
        store->add_module("m", "only.dll", "", "pe");
    }
    CB_CHECK(stdfs::is_regular_file(own));
    CB_CHECK(!stdfs::exists(state / "functions.sqlite"));   // the state's store is another
    CB_CHECK_EQ(fn::overview(state, own).size(), 1U);
    CB_CHECK(fn::overview(state).empty());
    CB_CHECK(fn::open(state, own, true) != nullptr);
    CB_CHECK(fn::open(state, dir.path() / "none.sqlite", true) == nullptr);
});

// ---- the ledger of `codebreaker versions` -------------------------------------------------------------------------------------------

namespace {

// The body store as a map in memory.
class MapBodies : public cb::store::BodyStore {
public:
    std::map<std::string, std::map<std::string, cb::store::Body>> by_module;
    std::vector<std::tuple<std::string, std::string, std::string>> named;

    void set_bodies(const cb::store::ModuleRef& module, const std::map<std::string, cb::store::Body>& by_key) override { by_module[module.sha] = by_key; }
    std::map<std::string, cb::store::Body> bodies(std::string_view module) override {
        const auto found = by_module.find(std::string(module));
        return found == by_module.end() ? std::map<std::string, cb::store::Body>{} : found->second;
    }
    void name_body(std::string_view digest, std::string_view name, std::string_view source) override {
        named.emplace_back(std::string(digest), std::string(name), std::string(source));
    }
    std::map<std::string, cb::store::SharedName> names_from_bodies(std::string_view) override { return {}; }
    cb::store::BodyCounts shared_bodies() override { return {}; }
};

struct LedgerWorld {
    NoEnv env;
    cb::fs::ScopedTempDir dir{"cbstoreledger"};
    cb::fs::ScopedEnv no_name{"CB_CORPUS_NAME", std::nullopt};
    std::optional<cb::fs::ScopedEnv> root_env;
    std::unique_ptr<cb::base::Corpus> corpus;
    std::unique_ptr<FunctionStore> store;
    stdfs::path old_build, new_build;
    LedgerWorld() {
        cb::fs::make_dirs(dir.path() / "corpus" / "win95");
        cb::fs::make_dirs(dir.path() / "corpus" / "win98");
        old_build = dir.path() / "corpus" / "win95" / "tool.exe";
        new_build = dir.path() / "corpus" / "win98" / "tool.exe";
        cb::fs::write_file(old_build, cb::to_bytes("old build bytes"));
        cb::fs::write_file(new_build, cb::to_bytes("new build bytes"));
        cb::base::Products products;
        products.target = "win95";
        products.target_folder = dir.path() / "corpus" / "win95";
        products.listed = {{"win95", dir.path() / "corpus" / "win95"}, {"win98", dir.path() / "corpus" / "win98"}};
        products.checkout_root = dir.path() / "CodeBreaker";
        products.state = dir.path() / "state";
        corpus = std::make_unique<cb::base::Corpus>(products);
        store = fn::open(dir.path() / "state");
    }
    cb::compare::VersionsLedger::Services services(cb::store::BodyStore* bodies = nullptr, bool with_identity = true) {
        cb::compare::VersionsLedger::Services s{*corpus, *store};
        if (with_identity)
            s.identity = [this](const stdfs::path& p) -> std::optional<ve::Ledger::Identity> {
                return ve::Ledger::Identity{corpus->sha(p).value_or(""), "profile-1"};
            };
        s.bodies = bodies;
        return s;
    }
};

}  // namespace

CB_TEST("store.versions_ledger.the_corpus_says_what_a_file_is", [] {
    LedgerWorld w;
    cb::compare::VersionsLedger ledger(w.services());
    const auto sha = ledger.sha(w.old_build);
    CB_CHECK(sha && sha->size() == 64);
    CB_CHECK(ledger.sha(w.old_build) == w.corpus->sha(w.old_build));
    CB_CHECK(ledger.sha(w.old_build) != ledger.sha(w.new_build));
    CB_CHECK(!ledger.sha(w.dir.path() / "missing.exe"));
    CB_CHECK(ledger.owner(w.old_build) == std::optional<std::string>("win95"));
    CB_CHECK(ledger.owner(w.new_build) == std::optional<std::string>("win98"));
    CB_CHECK(!ledger.owner(w.dir.path() / "elsewhere.exe"));
});

CB_TEST("store.versions_ledger.without_the_verifiers_settings_nothing_has_an_identity", [] {
    LedgerWorld w;
    cb::compare::VersionsLedger without(w.services(nullptr, /*with_identity=*/false));
    CB_CHECK(!without.identity(w.old_build));
    cb::compare::VersionsLedger with(w.services());
    const auto id = with.identity(w.old_build);
    CB_CHECK(id && id->sha == w.corpus->sha(w.old_build) && id->profile == "profile-1");
});

CB_TEST("store.versions_ledger.the_best_candidate_comes_with_the_class_its_verdict_makes_it", [] {
    LedgerWorld w;
    cb::compare::VersionsLedger ledger(w.services());
    const auto id = *ledger.identity(w.old_build);
    w.store->add_module(id.sha, "tool.exe", cb::fs::to_text(w.old_build), "pe");
    w.store->add_profile(id.profile, "{}");
    const std::vector<Record> rows = {record_of("1000", "_main", "int main(void) { return 0; }", "equivalent"),
                                      record_of("1010", "_helper", "int helper(void) { return 1; }", "divergent"),
                                      record_of("1020", "_none", "", "no-source")};
    w.store->record_many(id.sha, "core", "r9", id.profile, rows);
    const auto best = ledger.best(id);
    CB_CHECK_EQ(best.size(), 3U);
    CB_CHECK_EQ(best.at("1000").klass, "verified");
    CB_CHECK(best.at("1000").engine == std::optional<std::string>("core"));
    CB_CHECK(best.at("1000").name == std::optional<std::string>("_main"));
    CB_CHECK_EQ(best.at("1000").source, "int main(void) { return 0; }");
    CB_CHECK_EQ(best.at("1010").klass, "unverified");
    CB_CHECK_EQ(best.at("1020").source, "");
    // the same module under another profile has nothing
    CB_CHECK(ledger.best({id.sha, "profile-2"}).empty());
    CB_CHECK(ledger.best({"another-module", id.profile}).empty());
});

CB_TEST("store.versions_ledger.carried_reconstructions_are_filed_to_be_judged_in_the_new_build", [] {
    LedgerWorld w;
    cb::compare::VersionsLedger ledger(w.services());
    const auto id = *ledger.identity(w.new_build);
    const std::vector<std::string> addresses = {"2000", "2010", "2020"};
    const std::vector<ve::Ledger::Carried> rows = {
        {"2000", std::optional<std::string>("_main"), "int main(void) { return 0; }", "carried: identical to tool.exe 1000 (verified there); not judged here"},
        {"2020", std::nullopt, "int g(void) { return 2; }", "carried: identical to tool.exe 1020 (weak there); not judged here"}};
    ledger.file_carried(id, "tool.exe", w.new_build, ve::Kind::Pe, addresses, "0123456789ab", rows);

    // the module is in the store, with every address; the filed candidates are there under the engine, with no verdict that counts
    const auto modules = w.store->module_rows();
    CB_CHECK_EQ(modules.size(), 1U);
    CB_CHECK_EQ(modules[0].sha, id.sha);
    CB_CHECK_EQ(modules[0].name, "tool.exe");
    CB_CHECK_EQ(modules[0].kind, "pe");
    CB_CHECK(modules[0].path == cb::fs::to_text(w.new_build));
    CB_CHECK_EQ(w.store->addresses(id.sha).size(), 3U);
    const auto candidates = w.store->candidates(id.sha, id.profile, "carried");
    CB_CHECK_EQ(candidates.size(), 2U);
    for (const auto& c : candidates) {
        CB_CHECK_EQ(c.engine, "carried");
        CB_CHECK(c.rev == std::optional<std::string>("0123456789ab"));
        CB_CHECK(c.verdict == std::optional<std::string>("inconclusive"));
        CB_CHECK(c.reason.starts_with("carried: identical to tool.exe"));
        CB_CHECK(c.sig && c.sig->size() == 40);   // the SHA-1 of the source
    }
    // not judged here: the best of each is "not judged", never verified
    const auto best = ledger.best(id);
    CB_CHECK_EQ(best.at("2000").klass, "not-judged");
    CB_CHECK_EQ(best.at("2000").name.value_or(""), "_main");
    CB_CHECK_EQ(best.at("2000").source, "int main(void) { return 0; }");
    // the SHA-1 is of the source: the same source, the same sig
    CB_CHECK(candidates[0].sig != candidates[1].sig);
});

CB_TEST("store.versions_ledger.bodies_and_their_names_go_through_the_body_store", [] {
    LedgerWorld w;
    {
        // no body store: no bodies, and naming does nothing
        cb::compare::VersionsLedger ledger(w.services());
        CB_CHECK(ledger.bodies("any").empty());
        ledger.name_body("digest", "name", "why");
    }
    MapBodies bodies;
    bodies.by_module["sha-new"] = {{"3000", {"d1", std::nullopt, std::nullopt}}, {"3010", {"d2", std::string("Named"), std::string("export")}}};
    cb::compare::VersionsLedger ledger(w.services(&bodies));
    const auto got = ledger.bodies("sha-new");
    CB_CHECK_EQ(got.size(), 2U);
    CB_CHECK_EQ(got.at("3000"), "d1");
    CB_CHECK_EQ(got.at("3010"), "d2");
    CB_CHECK(ledger.bodies("sha-unknown").empty());
    CB_CHECK(ledger.bodies("").empty());
    ledger.name_body("d1", "Carried", "the same function in tool.exe of win95");
    CB_CHECK_EQ(bodies.named.size(), 1U);
    CB_CHECK(bodies.named[0] == std::make_tuple(std::string("d1"), std::string("Carried"), std::string("the same function in tool.exe of win95")));
});

CB_TEST("store.versions_ledger.a_build_carries_its_reconstructions_to_the_next_through_the_store", [] {
    // The whole of `codebreaker versions --carry`, with the real ledger: a build of LZ32.DLL and a second build of it that differs only past its end.
    // Every function is identical, so what the store holds of the old build reaches the new one as candidates to judge.
    LedgerWorld w;
    const cb::Bytes lz32 = cb::fs::read_file(cb::testing::data_path("formats/testdata/lz32.dll"));
    cb::Bytes second = lz32;
    second.push_back(0);   // another build as far as its hash goes, the same code
    cb::fs::write_file(w.old_build, lz32);
    cb::fs::write_file(w.new_build, second);
    cb::compare::VersionsLedger ledger(w.services());
    const ve::Comparison c = ve::compare(w.old_build, w.new_build);
    CB_CHECK(c.counts().identical > 3);
    CB_CHECK_EQ(c.counts().changed, 0);

    // the old build has two functions reconstructed and verified, a third judged against, a fourth with a weak match
    const auto old_id = *ledger.identity(w.old_build);
    const auto new_id = *ledger.identity(w.new_build);
    CB_CHECK(old_id.sha != new_id.sha);
    w.store->add_module(old_id.sha, "lz32.dll", cb::fs::to_text(w.old_build), "pe");
    const std::vector<ve::Pair>& pairs = c.pairs();
    const std::vector<Record> rows = {record_of(pairs[0].from, "_first", "int first(void) { return 1; }", "equivalent"),
                                      record_of(pairs[1].from, "_second", "int second(void) { return 2; }", "equivalent"),
                                      record_of(pairs[2].from, "_third", "int third(void) { return 3; }", "divergent"),
                                      record_of(pairs[3].from, "_fourth", "int fourth(void) { return 4; }", "consistent")};
    w.store->record_many(old_id.sha, "core", "r1", old_id.profile, rows);

    const std::vector<ve::Reconstruction> found = c.reconstructions(ledger);
    CB_CHECK_EQ(found.size(), 3U);   // verified, verified and the weak one; not the divergent
    CB_CHECK_EQ(found[0].from, pairs[0].from);
    CB_CHECK_EQ(found[0].to, pairs[0].to);
    CB_CHECK_EQ(found[0].klass, "verified");
    CB_CHECK_EQ(found[2].klass, "weak");

    CB_CHECK_EQ(c.carry(ledger), 3U);
    const auto filed = w.store->candidates(new_id.sha, new_id.profile, "carried");
    CB_CHECK_EQ(filed.size(), 3U);
    CB_CHECK_EQ(w.store->addresses(new_id.sha).size(), c.to().count());   // every function of the new build is in the store
    for (const auto& cand : filed) CB_CHECK(cand.verdict == std::optional<std::string>("inconclusive"));
    // nothing was said of the new build's own results: what was carried waits to be judged
    const auto best = w.store->best(new_id.sha, new_id.profile);
    CB_CHECK_EQ(best.size(), 3U);
    for (const auto& [address, b] : best) CB_CHECK(b.cls == Class::NotJudged);

    // the names: without a body store there is nothing to carry them to; with one, the identical named functions are given to its bodies
    CB_CHECK(!c.carry_names(ledger).has_value());
    MapBodies bodies;
    bodies.by_module[new_id.sha] = {{pairs[0].to, {"digest-0", std::nullopt, std::nullopt}}};
    cb::compare::VersionsLedger with_bodies(w.services(&bodies));
    CB_CHECK(c.carry_names(with_bodies).has_value());   // (a build compared with itself has no names to carry: the same ones are on both sides)
    CB_CHECK(bodies.named.empty());
});
