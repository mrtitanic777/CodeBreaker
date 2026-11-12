#include "cb/engine/function_store.hpp"

#include "cb/base/sqlite.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace {

using namespace cb;
using cb::engine::Candidate;
using cb::engine::FunctionStore;
using cb::engine::Record;
using cb::verify::Class;
namespace sql = cb::base::sqlite;
namespace stdfs = std::filesystem;

struct Fixture {
    fs::ScopedTempDir tmp{"function-store"};
    stdfs::path index() const { return tmp.path() / "functions.sqlite"; }
    stdfs::path folder() const { return tmp.path() / "db"; }
    FunctionStore open(bool readonly = false) const { return FunctionStore(index(), folder(), readonly); }
};

Record rec(std::string addr, std::string name, std::string source, std::string sig, std::string verdict, std::string limits = "") {
    Record r;
    r.addr = std::move(addr);
    r.name = std::move(name);
    r.source = std::move(source);
    r.sig = std::move(sig);
    r.verdict = std::move(verdict);
    r.limits = std::move(limits);
    return r;
}

}  // namespace

CB_TEST("engine.function_store.record_and_read_candidates", [] {
    Fixture fx;
    FunctionStore store = fx.open();
    store.add_module("m1", "kernel32.dll", "C:/k32.dll", "pe");
    store.add_profile("p1", "{}");
    const std::vector<Record> rows = {rec("1:10", "_a", "int a(){}", "s1", "equivalent"),
                                      rec("1:20", "_b", "int b(){}", "s2", "divergent", ""),
                                      rec("1:30", "_c", "", "s3", "inconclusive")};
    store.record_many("m1", "decompile", "rev7", "p1", rows);

    const std::vector<Candidate> got = store.candidates("m1", "p1");
    CB_CHECK_EQ(got.size(), 3u);
    std::map<std::string, Candidate> by_addr;
    for (const Candidate& c : got) by_addr[c.addr] = c;
    CB_CHECK_EQ(by_addr.at("1:10").engine, "decompile");
    CB_CHECK_EQ(by_addr.at("1:10").rev.value(), "rev7");
    CB_CHECK_EQ(by_addr.at("1:10").verdict.value(), "equivalent");
    CB_CHECK_EQ(by_addr.at("1:10").judged_sig.value(), "s1");
    CB_CHECK_EQ(by_addr.at("1:10").source.value(), "int a(){}");
    CB_CHECK(by_addr.at("1:10").classification().cls == Class::Verified);
    CB_CHECK(by_addr.at("1:20").classification().cls == Class::Unverified);
    CB_CHECK(by_addr.at("1:30").classification().cls == Class::NotJudged);

    // under a profile nothing was judged under, the candidates are there and have no verdict
    const std::vector<Candidate> other = store.candidates("m1", "other-profile");
    CB_CHECK_EQ(other.size(), 3u);
    for (const Candidate& c : other) {
        CB_CHECK(!c.verdict.has_value());
        CB_CHECK(c.classification().cls == Class::NoSource);
    }
    // by engine
    CB_CHECK_EQ(store.candidates("m1", "p1", "decompile").size(), 3u);
    CB_CHECK_EQ(store.candidates("m1", "p1", "hand").size(), 0u);
    // a module that is not in the store has nothing
    CB_CHECK(store.candidates("zz", "p1").empty());
    CB_CHECK(store.addresses("zz").empty());
});

CB_TEST("engine.function_store.best_goes_by_class_then_by_the_engine_named_first", [] {
    Fixture fx;
    FunctionStore store = fx.open();
    store.add_module("m1", "a.dll", "", "pe");
    store.record_many("m1", "old", "r1", "p", std::vector<Record>{rec("1:1", "f", "int f(){}", "x", "divergent"), rec("1:2", "g", "int g(){}", "y", "equivalent")});
    store.record_many("m1", "new", "r2", "p", std::vector<Record>{rec("1:1", "f", "int f2(){}", "x2", "equivalent"), rec("1:2", "g", "int g2(){}", "y2", "equivalent"),
                                                                    rec("1:3", "h", "", "z", "equivalent")});
    // a candidate with no source counts as no source whatever its verdict says
    const std::string prefer_new[] = {"new", "old"};
    const std::string prefer_old[] = {"old", "new"};
    const auto best_new = store.best("m1", "p", prefer_new);
    CB_CHECK_EQ(best_new.size(), 3u);
    CB_CHECK(best_new.at("1:1").cls == Class::Verified);
    CB_CHECK_EQ(best_new.at("1:1").candidate.engine, "new");        // the better class wins over the engine order
    CB_CHECK_EQ(best_new.at("1:2").candidate.engine, "new");        // equal classes: the engine named first
    const auto best_old = store.best("m1", "p", prefer_old);
    CB_CHECK_EQ(best_old.at("1:1").candidate.engine, "new");
    CB_CHECK_EQ(best_old.at("1:2").candidate.engine, "old");
    CB_CHECK(best_old.at("1:3").cls == Class::NoSource);
    // an engine nobody named comes after the named ones
    const std::string only_old[] = {"old"};
    CB_CHECK_EQ(store.best("m1", "p", only_old).at("1:2").candidate.engine, "old");
    // nothing judged under another profile
    CB_CHECK(store.best("m1", "nope").empty());

    // the tally: every class a key, the functions with no candidate counted as no source
    store.add_functions("m1", std::vector<std::string>{"1:9"});
    const auto tally = store.tally("p", "m1", prefer_new);
    CB_CHECK_EQ(tally.at(Class::Verified), 2);
    CB_CHECK_EQ(tally.at(Class::NoSource), 2);   // 1:3 (no source text) and 1:9 (no candidate)
    CB_CHECK_EQ(tally.at(Class::Weak), 0);
    CB_CHECK_EQ(tally.size(), 6u);               // verified, weak, unverified, not judged, no compile, no source
    CB_CHECK(!tally.contains(Class::Identified));
});

CB_TEST("engine.function_store.consistent_verdicts_are_weak_and_say_why", [] {
    Fixture fx;
    FunctionStore store = fx.open();
    store.add_module("m", "a.dll", "", "pe");
    store.record_many("m", "e", "r", "p", std::vector<Record>{rec("1:1", "f", "x", "s", "consistent", "low-coverage,truncated"), rec("1:2", "g", "x", "s", "consistent"),
                                                              rec("1:3", "h", "x", "s", "equivalent", "stub-callee")});
    const auto best = store.best("m", "p");
    CB_CHECK(best.at("1:1").cls == Class::Weak);
    CB_CHECK(best.at("1:1").weak.contains(verify::WeakLimit::LowCoverage));
    CB_CHECK(best.at("1:1").weak.contains(verify::WeakLimit::Truncated));
    CB_CHECK_EQ(best.at("1:1").weak.size(), 2u);
    CB_CHECK(best.at("1:2").weak.contains(verify::WeakLimit::FaultOnly));   // a consistent verdict with no limit: both sides only fail alike
    CB_CHECK(best.at("1:3").cls == Class::Weak);                           // an equivalent one with a limit is weak too
    CB_CHECK(best.at("1:3").weak.contains(verify::WeakLimit::StubCallee));
});

CB_TEST("engine.function_store.older_spellings_read_as_the_words_now", [] {
    Fixture fx;
    FunctionStore store = fx.open();
    store.add_module("m", "a.dll", "", "pe");
    store.record_many("m", "e", "r", "p", std::vector<Record>{rec("1:1", "f", "x", "s", "same"), rec("1:2", "g", "x", "s", "differs")});
    // written as the words
    const auto c = store.candidates("m", "p");
    std::map<std::string, std::string> words;
    for (const Candidate& k : c) words[k.addr] = k.verdict.value();
    CB_CHECK_EQ(words.at("1:1"), "equivalent");
    CB_CHECK_EQ(words.at("1:2"), "divergent");
    // and one written by the C engine of before reads as the word too
    const stdfs::path db_file = store.database_path("m");
    {
        sql::Database db = sql::Database::open(db_file);
        db.exec("UPDATE verdict SET verdict='does not compile' WHERE candidate IN (SELECT id FROM candidate WHERE function IN (SELECT id FROM function WHERE addr='1:2'))");
    }
    std::map<std::string, std::string> again;
    for (const Candidate& k : store.candidates("m", "p")) again[k.addr] = k.verdict.value();
    CB_CHECK_EQ(again.at("1:2"), "no-compile");
    CB_CHECK(store.best("m", "p").at("1:2").cls == Class::NoCompile);
});

CB_TEST("engine.function_store.a_changed_sig_drops_the_verdicts_under_other_profiles", [] {
    Fixture fx;
    FunctionStore store = fx.open();
    store.add_module("m", "a.dll", "", "pe");
    store.record("m", "e", "r1", "p1", rec("1:1", "f", "src", "sig-a", "equivalent"));
    store.record("m", "e", "r1", "p2", rec("1:1", "f", "src", "sig-a", "divergent"));
    CB_CHECK_EQ(store.candidates("m", "p1").front().verdict.value(), "equivalent");
    CB_CHECK_EQ(store.candidates("m", "p2").front().verdict.value(), "divergent");
    // the same sig again under p2: p1's verdict stays
    store.record("m", "e", "r2", "p2", rec("1:1", "f", "src", "sig-a", "equivalent"));
    CB_CHECK_EQ(store.candidates("m", "p1").front().verdict.value(), "equivalent");
    CB_CHECK_EQ(store.candidates("m", "p1").front().rev.value(), "r2");   // the candidate is one row: its rev moved on
    // the sig changed (the source is another): only the profile it was judged under now says anything
    store.record("m", "e", "r3", "p2", rec("1:1", "f", "src2", "sig-b", "equivalent"));
    CB_CHECK(!store.candidates("m", "p1").front().verdict.has_value());
    CB_CHECK_EQ(store.candidates("m", "p2").front().verdict.value(), "equivalent");
    // judged(): the verdicts that are about the candidate as it is now
    CB_CHECK_EQ(store.judged("m", "p2", "e").size(), 1u);
    CB_CHECK(store.judged("m", "p1", "e").empty());

    // stored(): each candidate of a function with each of its verdicts, newest first
    const auto stored = store.stored("m", "1:1");
    CB_CHECK_EQ(stored.size(), 1u);
    CB_CHECK_EQ(stored[0].engine, "e");
    CB_CHECK_EQ(stored[0].source, "src2");
    CB_CHECK_EQ(stored[0].verdicts.size(), 1u);
    CB_CHECK_EQ(stored[0].verdicts[0].profile, "p2");
    CB_CHECK(!stored[0].verdicts[0].carried);
    CB_CHECK(store.stored("m", "9:9").empty());
});

CB_TEST("engine.function_store.the_summary_follows_every_write", [] {
    Fixture fx;
    FunctionStore store = fx.open();
    store.add_module("m1", "b.dll", "C:/b.dll", "ne");
    store.add_module("m2", "a.dll", "C:/a.dll", "pe");
    store.add_functions("m1", std::vector<std::string>{"1:1", "1:2", "1:3", "1:4"});
    auto sums = store.summaries();
    CB_CHECK_EQ(sums.at("m1").functions, 4);
    CB_CHECK_EQ(sums.at("m1").judged, 0);
    CB_CHECK(!sums.at("m1").last.has_value());

    store.record_many("m1", "e", "r", "pA", std::vector<Record>{rec("1:1", "f", "x", "s", "equivalent"), rec("1:2", "g", "x", "s", "divergent")});
    store.set_identified("m1", {{"1:3", {"_memcpy", "libc"}}});
    sums = store.summaries();
    CB_CHECK_EQ(sums.at("m1").judged, 2);
    CB_CHECK_EQ(sums.at("m1").identified, 1);
    CB_CHECK(sums.at("m1").last.has_value());
    CB_CHECK_EQ(sums.at("m1").last_profile.value(), "pA");
    CB_CHECK_EQ(sums.at("m1").profiles.size(), 1u);
    CB_CHECK_EQ(sums.at("m1").latest.at(Class::Verified), 1);
    CB_CHECK_EQ(sums.at("m1").latest.at(Class::Unverified), 1);

    store.record_many("m1", "e", "r", "pB", std::vector<Record>{rec("1:1", "f", "x", "s", "equivalent")});
    CB_CHECK_EQ(store.summaries().at("m1").profiles.size(), 2u);
    CB_CHECK_EQ(store.summaries().at("m1").last_profile.value(), "pB");

    // the index: modules by name, with a profile those judged under it, paths, kinds, recent
    const auto mods = store.modules();
    CB_CHECK_EQ(mods.size(), 2u);
    CB_CHECK_EQ(mods[0].name, "a.dll");   // by name
    CB_CHECK_EQ(store.modules("pB").size(), 1u);
    CB_CHECK_EQ(store.modules("pB")[0].sha, "m1");
    CB_CHECK(store.modules("nobody").empty());
    CB_CHECK_EQ(store.module_rows("ne").size(), 1u);
    CB_CHECK_EQ(store.module_rows("ne")[0].path, "C:/b.dll");
    CB_CHECK_EQ(store.module_path("b.dll").value(), "C:/b.dll");
    CB_CHECK(!store.module_path("c.dll").has_value());
    CB_CHECK_EQ(store.latest_profile("ne").value(), "pB");
    CB_CHECK(!store.latest_profile("pe").has_value());   // nothing judged there
    const auto recent = store.recent(5);
    CB_CHECK_EQ(recent.size(), 1u);
    CB_CHECK_EQ(recent[0].name, "b.dll");
    CB_CHECK_EQ(recent[0].judged, 3);
    CB_CHECK_EQ(store.verdicts_since(0), 3);
    CB_CHECK_EQ(store.verdicts_since(1e12), 0);

    // overview: the latest verdicts by class, the functions with none counted as no source
    const auto overview = store.overview();
    CB_CHECK_EQ(overview.size(), 2u);
    CB_CHECK_EQ(overview[0].name, "a.dll");
    CB_CHECK_EQ(overview[0].functions, 0);
    CB_CHECK_EQ(overview[1].name, "b.dll");
    CB_CHECK_EQ(overview[1].functions, 4);
    CB_CHECK_EQ(overview[1].identified, 1);
    CB_CHECK_EQ(overview[1].latest.at(Class::Verified), 1);
    CB_CHECK_EQ(overview[1].latest.at(Class::Unverified), 1);
    CB_CHECK_EQ(overview[1].latest.at(Class::NoSource), 2);

    // latest(): by each function's latest verdict under any profile
    const auto latest = store.latest("m1");
    CB_CHECK_EQ(latest.size(), 2u);
    CB_CHECK(latest.at("1:1") == Class::Verified);
    CB_CHECK(latest.at("1:2") == Class::Unverified);
});

CB_TEST("engine.function_store.scores_count_runtime_library_functions_on_their_own", [] {
    Fixture fx;
    FunctionStore store = fx.open();
    store.add_module("m", "a.dll", "", "pe");
    store.add_functions("m", std::vector<std::string>{"1:1", "1:2", "1:3", "1:4"});
    store.record_many("m", "e", "r", "p", std::vector<Record>{rec("1:1", "f", "x", "s", "equivalent"), rec("1:2", "g", "x", "s", "divergent")});
    store.set_identified("m", {{"1:1", {"_f", "libc"}}, {"1:2", {"_g", "libc"}}, {"1:3", {"_h", "libc"}}});
    const auto scores = store.scores("p", "m");
    CB_CHECK_EQ(scores.size(), 7u);
    CB_CHECK_EQ(scores.at(Class::Verified), 1);       // recognised and verified: stays verified
    CB_CHECK_EQ(scores.at(Class::Identified), 2);     // 1:2 (diverges) and 1:3 (nothing): by name, linked, not written
    CB_CHECK_EQ(scores.at(Class::NoSource), 1);
    const auto known = store.identified("m");
    CB_CHECK_EQ(known.size(), 3u);
    CB_CHECK_EQ(known.at("1:3").second.value(), "libc");
    store.set_identified("m", {});                     // the list is replaced
    CB_CHECK(store.identified("m").empty());
    CB_CHECK_EQ(store.scores("p", "m").at(Class::Identified), 0);
});

CB_TEST("engine.function_store.texts_built_and_the_about_note", [] {
    Fixture fx;
    FunctionStore store = fx.open();
    store.add_module("m", "a.dll", "", "pe");
    CB_CHECK_EQ(store.get_text("m", "e", "c"), "");
    store.put_text("m", "e", "c", "int main(void) {}");
    store.put_text("m", "e", "c", "int main(int a) {}");   // replaced
    store.put_text("m", "e", "asm", "ret");
    CB_CHECK_EQ(store.get_text("m", "e", "c"), "int main(int a) {}");
    CB_CHECK_EQ(store.get_text("m", "e", "asm"), "ret");
    CB_CHECK_EQ(store.get_text("m", "x", "c"), "");

    store.set_built("m", "p", {{"1:1", "sig1"}, {"1:2", "sig2"}});
    store.set_built("m", "p", {{"1:1", "sig1b"}});   // one addr replaced, the other left
    const auto built = store.built("m", "p");
    CB_CHECK_EQ(built.size(), 2u);
    CB_CHECK_EQ(built.at("1:1").value(), "sig1b");
    CB_CHECK(store.built("m", "q").empty());

    CB_CHECK(!store.about("note").has_value());
    store.set_about("note", "hello");
    store.set_about("note", "again");
    CB_CHECK_EQ(store.about("note").value(), "again");
});

CB_TEST("engine.function_store.variants_are_compared_by_their_bytes", [] {
    Fixture fx;
    FunctionStore store = fx.open();
    store.add_module("m", "a.dll", "", "pe");
    const std::int64_t v1 = store.record_variant("m", "1:1", "msc6", "int f(){}", 12, 40, "different", "-O1");
    const std::int64_t v2 = store.record_variant("m", "1:1", "msc6", "int f(){ }", 0, 40, "identical", "-O2", v1, "equivalent");
    const std::int64_t v3 = store.record_variant("m", "1:2", "msc6", "int g(){}", 4, 40, "same size");
    store.record_variant("m", "1:3", "msc6", "int h(){}", std::nullopt, std::nullopt, "no original extent");
    store.record_variant("m", "1:2", "watcom", "int g(){}", 2, 30, "identical", "", std::nullopt, "divergent");
    CB_CHECK(v1 != v2 && v2 != v3);
    // the same function, compiler and knobs again: the same variant, updated
    CB_CHECK_EQ(store.record_variant("m", "1:1", "msc6", "int f(){;}", 6, 40, "different", "-O1"), v1);

    const auto all = store.variants("m");
    CB_CHECK_EQ(all.size(), 5u);
    const auto msc6 = store.variants("m", "msc6");
    CB_CHECK_EQ(msc6.size(), 4u);
    bool saw_parent = false;
    for (const auto& v : msc6) {
        if (v.knobs == "-O2") {
            saw_parent = v.parent == v1;
            CB_CHECK_EQ(v.behaves.value(), "equivalent");
            CB_CHECK_EQ(v.match.value(), "identical");
            CB_CHECK_EQ(v.distance.value(), 0);
        }
        if (v.addr == "1:3") CB_CHECK(!v.distance.has_value());
    }
    CB_CHECK(saw_parent);

    // best: one that behaves (or was not judged) first, then the fewest bytes apart
    const auto best = store.best_variant("m", "msc6");
    CB_CHECK_EQ(best.size(), 3u);
    CB_CHECK_EQ(best.at("1:1").knobs, "-O2");
    CB_CHECK_EQ(best.at("1:2").match.value(), "same size");
    CB_CHECK_EQ(store.best_variant("m", "watcom").at("1:2").behaves.value(), "divergent");
    const auto tally = store.match_tally("m", "msc6");
    CB_CHECK_EQ(tally.functions, 3);
    CB_CHECK_EQ(tally.identical, 1);
    CB_CHECK_EQ(tally.same_size, 1);
    CB_CHECK_EQ(tally.other, 1);
});

CB_TEST("engine.function_store.a_store_written_by_the_c_engine_reads_the_same", [] {
    // rows as native/engine (cbengine.dll) writes them: the same tables, the weak limits and a carried verdict among them
    Fixture fx;
    {
        FunctionStore writer = fx.open();
        writer.add_module("legacy", "old.dll", "C:/old.dll", "ne");   // the tables made
    }
    {
        sql::Database db = sql::Database::open(fx.folder() / "legacy.sqlite");
        db.exec("INSERT INTO function(module, addr, name, size) VALUES('legacy','1:1','_f',10)");
        db.exec("INSERT INTO candidate(function, engine, rev, sig, name, source, problem, ts) VALUES(1,'decompile',42,'abc','_f','int f(){}',NULL,100.5)");
        db.exec("INSERT INTO verdict(candidate, profile, sig, verdict, reason, limits, detail, ts, carried) VALUES(1,'p','abc','consistent','r','truncated','d',101.0,1)");
        db.exec("INSERT INTO text(module, engine, kind, body, ts) VALUES('legacy','decompile','c','body',1.0)");
    }
    FunctionStore store = fx.open();
    const auto cands = store.candidates("legacy", "p");
    CB_CHECK_EQ(cands.size(), 1u);
    CB_CHECK_EQ(cands[0].rev.value(), "42");   // a number in a text column reads as its digits
    CB_CHECK_EQ(cands[0].verdict.value(), "consistent");
    CB_CHECK_EQ(cands[0].limits, "truncated");
    CB_CHECK(store.best("legacy", "p").at("1:1").weak.contains(verify::WeakLimit::Truncated));
    const auto stored = store.stored("legacy", "1:1");
    CB_CHECK(stored[0].verdicts[0].carried);
    CB_CHECK_EQ(store.get_text("legacy", "decompile", "c"), "body");

    // a verdict table from before the `carried` column is brought up to date when it is opened
    {
        sql::Database db = sql::Database::open(fx.folder() / "ancient.sqlite");
        db.exec("CREATE TABLE verdict(candidate INTEGER NOT NULL, profile TEXT NOT NULL, sig TEXT, verdict TEXT, reason TEXT, limits TEXT, detail TEXT, ts REAL, PRIMARY KEY (candidate, profile))");
    }
    store.add_functions("ancient", std::vector<std::string>{"1:1"});
    store.record("ancient", "e", "r", "p", rec("1:1", "f", "x", "s", "equivalent"));
    CB_CHECK_EQ(store.candidates("ancient", "p").size(), 1u);
});

CB_TEST("engine.function_store.read_only_stores_read_and_never_write", [] {
    Fixture fx;
    {
        // nothing there: reads are empty, nothing is made
        FunctionStore none = fx.open(true);
        CB_CHECK(none.modules().empty());
        CB_CHECK(none.candidates("m", "p").empty());
        CB_CHECK(!none.module_database("m").has_value());
        CB_CHECK(none.summaries().empty());
        CB_CHECK(!stdfs::exists(fx.index()));
        CB_CHECK_THROWS_AS(IoError, none.add_module("m", "a.dll"));
        CB_CHECK_THROWS_AS(IoError, none.add_functions("m", std::vector<std::string>{"1:1"}));
        CB_CHECK(!stdfs::exists(fx.folder()));
    }
    {
        FunctionStore store = fx.open();
        store.add_module("m", "a.dll", "", "pe");
        store.record("m", "e", "r", "p", rec("1:1", "f", "x", "s", "equivalent"));
    }
    FunctionStore ro = fx.open(true);
    CB_CHECK(ro.readonly());
    CB_CHECK_EQ(ro.modules().size(), 1u);
    CB_CHECK_EQ(ro.candidates("m", "p").size(), 1u);
    CB_CHECK(ro.module_database("m").has_value());
    CB_CHECK_EQ(ro.tally("p", "m").at(Class::Verified), 1);
    CB_CHECK_THROWS_AS(IoError, ro.record("m", "e", "r", "p", rec("1:2", "g", "x", "s", "divergent")));
    CB_CHECK_THROWS_AS(IoError, ro.put_text("m", "e", "c", "x"));
    CB_CHECK_THROWS_AS(IoError, ro.set_about("a", "b"));
    CB_CHECK_EQ(ro.candidates("m", "p").size(), 1u);   // and nothing was written
    CB_CHECK(ro.candidates("other", "p").empty());     // a module with no database reads as empty
});
