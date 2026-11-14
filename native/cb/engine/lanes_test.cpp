// Tests of the lanes' steps: a lane's reconstruct-and-judge made of facts, kept and made again only where what it was made from changed.
//
// A made-up lane (FakeKind) stands in for the real ones, : prepare reads the binary's
// words and makes one function of each, build "compiles" each function to the digits of its body (so a comment leaves its code alone)
// and refuses a function that has an x in it, judge says equivalent for an even key and divergent for an odd one and notes each call.
// testdata/lanes.golden.json is the recorded scenario: after each stage
// -- the first run, asked again, a comment added by hand, a change to a function's code and a function added by hand, and both given
// facts forgotten -- the rows, the texts, the keys, what the build said of each function, the functions judged in that stage and the
// build's [kept, made]; the C++ steps must give the same documents.
//
// Design notes:
//   * A fact is a document: the decompiled module is kept as ModuleDoc (header, order, keys, texts, sig), the live module by the
//     lanes object. A module given functions by hand is prepared again with them.
//   * pe.image keeps the linked file in the build folder and the fact names it.
//   * The judge runs on threads of this process, a function that throws is "not run".
#include "cb/engine/lanes.hpp"

#include <algorithm>
#include <map>
#include <mutex>
#include <regex>

#include <nlohmann/json.hpp>

#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"

using namespace cb;
using namespace cb::engine;
using namespace cb::engine::lanes;

namespace {

using KV = knowledge::Value;
using verify::lanes::Built;
using verify::lanes::Prepared;

void expect(bool ok, const std::string& what) {
    cb::testing::count_check();
    if (!ok) cb::testing::record_failure(__FILE__, __LINE__, what);
}

// The text of a document with every map's keys in order.
std::string canon(const KV& v) {
    switch (v.kind()) {
        case KV::Kind::Null: return "null";
        case KV::Kind::Bool: return *v.boolean() ? "true" : "false";
        case KV::Kind::Int: return std::to_string(*v.integer());
        case KV::Kind::Double: return std::to_string(*v.number());
        case KV::Kind::Text: return "\"" + *v.text() + "\"";
        case KV::Kind::List: {
            std::string out = "[";
            for (const KV& x : *v.list()) out += canon(x) + ",";
            return out + "]";
        }
        case KV::Kind::Map: {
            std::map<std::string, std::string> sorted;
            for (const auto& [k, x] : *v.map()) sorted[k] = canon(x);
            std::string out = "{";
            for (const auto& [k, x] : sorted) out += k + ":" + x + ",";
            return out + "}";
        }
    }
    return "";
}

std::string canon(const Json& j) {
    if (j.is_null()) return "null";
    if (j.is_boolean()) return j.get<bool>() ? "true" : "false";
    if (j.is_number_integer()) return std::to_string(j.get<std::int64_t>());
    if (j.is_string()) return "\"" + j.get<std::string>() + "\"";
    if (j.is_array()) {
        std::string out = "[";
        for (const Json& x : j) out += canon(x) + ",";
        return out + "]";
    }
    std::map<std::string, std::string> sorted;
    for (auto it = j.begin(); it != j.end(); ++it) sorted[it.key()] = canon(it.value());
    std::string out = "{";
    for (const auto& [k, x] : sorted) out += k + ":" + x + ",";
    return out + "}";
}

const KV& doc_of(const facts::Value& v) { return std::get<KV>(v); }

// ---- the made-up lane ------------------------------------------------------------------------------------------------------------

class FakeKind : public verify::lanes::Kind {
public:
    explicit FakeKind(std::string name = "tfake") : name_(std::move(name)) {}

    const std::string& name() const override { return name_; }
    verify::lanes::Profile profile(const std::filesystem::path&) const override {
        const std::lock_guard<std::mutex> lock(mutex);
        return {digest, "{}"};
    }
    std::unique_ptr<Prepared> prepare(const std::filesystem::path& module, const std::filesystem::path&) const override {
        {
            const std::lock_guard<std::mutex> lock(mutex);
            ++prepares;
        }
        const std::string words_text = fs::read_text(std::filesystem::is_directory(module) ? module / "words.txt" : module);
        std::vector<std::string> words;
        std::string word;
        for (char c : words_text + " ") {
            if (c == ' ' || c == '\n') {
                if (!word.empty()) words.push_back(word);
                word.clear();
            } else {
                word += c;
            }
        }
        auto p = std::make_unique<Prepared>();
        for (std::size_t i = 0; i < words.size(); ++i) {
            const std::string n = "f" + std::to_string(i);
            p->order.push_back(n);
            p->texts[n] = "int " + n + "(void) { return " + words[i] + "; }";
            p->key[n] = std::format("{:04x}", i);
        }
        for (const std::string& n : p->order) p->all_keys.push_back(p->key[n]);
        std::sort(p->all_keys.begin(), p->all_keys.end());
        p->all_keys.push_back("ffff");   // one the decompiler gave nothing for
        p->header = "typedef int int4;";
        p->sig = "whole:";
        for (std::size_t i = 0; i < words.size(); ++i) p->sig += (i ? " " : "") + words[i];
        return p;
    }
    Built build(const Prepared& p, const std::filesystem::path&, const std::filesystem::path& work) const override {
        {
            const std::lock_guard<std::mutex> lock(mutex);
            ++builds;
        }
        std::filesystem::create_directories(work);
        fs::write_text(work / "m.dll", "built");
        std::map<std::string, std::string> digits;
        Built b;
        for (const std::string& n : p.order) {
            const std::string& t = p.texts.at(n);
            std::string body = t.substr(t.find('{') + 1);
            body = body.substr(0, body.find("/*"));
            digits[n];   // (a function with no digit in it has the empty digest: it is in the build's objects all the same)
            for (char c : body)
                if (c >= '0' && c <= '9') digits[n] += c;
            if (t.find('x') == std::string::npos) b.items.push_back(n);
            else b.missing[n] = "it does not compile";
        }
        {
            const std::lock_guard<std::mutex> lock(mutex);
            objects[fs::to_text(work)] = digits;
        }
        const auto key = p.key;
        b.judge = [this, key, work](const std::string& n) {
            if (!std::filesystem::is_regular_file(work / "m.dll")) throw InternalError("the build's files are gone");
            const std::string& k = key.at(n);
            {
                const std::lock_guard<std::mutex> lock(mutex);
                judged.push_back(k);
            }
            if (n == crash_on) throw FormatError("engine fact", 0, "the judge fell over");
            verify::Judgement j;
            j.verdict = std::stoi(k, nullptr, 16) % 2 == 0 ? verify::Verdict::Equivalent : verify::Verdict::Divergent;
            j.limits = {"coverage=100"};
            return j;
        };
        return b;
    }

    std::vector<std::string> take_judged() const {
        const std::lock_guard<std::mutex> lock(mutex);
        std::vector<std::string> out = judged;
        std::sort(out.begin(), out.end());
        judged.clear();
        return out;
    }
    std::map<std::string, std::string> digits_of(const std::filesystem::path& work) const {
        const std::lock_guard<std::mutex> lock(mutex);
        const auto it = objects.find(fs::to_text(work));
        return it == objects.end() ? std::map<std::string, std::string>{} : it->second;
    }

    std::string name_;
    mutable std::mutex mutex;
    mutable int prepares = 0, builds = 0;
    mutable std::vector<std::string> judged;
    mutable std::map<std::string, std::map<std::string, std::string>> objects;   // work folder -> {name: digits of its body}
    std::string digest = "p1";
    std::string crash_on;
};

// A world to ask the lane's steps in: a binary, a folder of databases, the steps, the engine and the lanes object.
struct World {
    fs::ScopedTempDir dir{"lanes"};
    std::filesystem::path binary;
    facts::Databases databases;
    Steps steps;
    knowledge::ReadDigests digests;
    Engine engine;
    Lanes ls;
    std::shared_ptr<FakeKind> kind;

    explicit World(const std::string& words, std::shared_ptr<FakeKind> k = nullptr, bool per_function = true)
        : binary(dir.path() / "thing.bin"),
          databases(facts::Config{dir.path() / "db", 1 * facts::kGiB}),
          digests([](const knowledge::Read&) { return std::string(knowledge::kAbsent); }),
          engine(steps, databases, digests, 4),
          ls(steps, engine, [](const std::filesystem::path&) { return std::optional<std::string>("tfake"); }),
          kind(k ? std::move(k) : std::make_shared<FakeKind>()) {
        fs::write_text(binary, words);
        declare(per_function);
    }

    void declare(bool per_function) {
        Setup setup;
        setup.kind = kind;
        setup.platform = "tfake";
        setup.traits.per_function = per_function;
        setup.traits.identifies_library = false;
        const auto k = kind;
        setup.traits.object_digests = [k](const std::filesystem::path& work) { return k->digits_of(work); };
        ls.declare(std::move(setup));
    }

    // The steps over the same databases, as a new process has them (what the database holds is all it has).
    std::shared_ptr<facts::Database> db() { return engine.open(binary); }
};

const Json& golden() {
    static const Json doc = cb::json::load(cb::testing::data_path("engine/testdata/lanes.golden.json"));
    return doc;
}

std::size_t made_of(Engine& e, facts::Database& db, const std::string& kind, bool made) {
    for (const auto& c : e.last_run(db))
        if (c.kind == kind) return made ? c.made : c.kept;
    return 0;
}

facts::Value text_value(std::string s) { return facts::Value(std::move(s)); }

// What a step gave, as a document of text.
std::string got(World& w, const std::shared_ptr<facts::Database>& db, const std::string& kind, const std::string& key = "") {
    return canon(doc_of(w.engine.get(db, kind, key)));
}

}  // namespace

// ---- the recorded scenario ----------------------------------------------------------------------------------------------------------

CB_TEST("engine.lanes.the_steps_give_what_the_recorded_steps_gave_stage_by_stage", [] {
    World w("2 7 4 9");
    const auto db = w.db();
    const Json& stages = golden().at("stages");
    const std::vector<std::string> keys = {"0000", "0001", "0002", "0003"};
    const Json& f0002 = golden().at("function_0002");
    for (std::size_t i = 0; i < stages.size(); ++i) {
        const std::string name = stages.at(i).at("stage").get<std::string>();
        if (name == "comment") w.engine.put(*db, "lane.tfake.function", "0002", facts::Value(KV(KV::List{KV(f0002.at(0).get<std::string>()), KV(f0002.at(1).get<std::string>() + " /* by hand */")})));
        if (name == "code") {
            std::string text = f0002.at(1).get<std::string>();
            text.replace(text.find('4'), 1, "6");
            w.engine.put(*db, "lane.tfake.function", "0002", facts::Value(KV(KV::List{KV(f0002.at(0).get<std::string>()), KV(text)})));
            w.engine.put(*db, "lane.tfake.function", "0003", facts::Value(KV(KV::List{KV("f3"), KV("int f3(void) { return x; }")})));
        }
        if (name == "forgotten") {
            w.engine.forget(*db, "lane.tfake.function", "0002");
            w.engine.forget(*db, "lane.tfake.function", "0003");
        }
        const Json& want = stages.at(i);
        const std::string rows = got(w, db, "lane.tfake.rows");
        expect(rows == canon(want.at("rows")), std::format("stage {}: the rows\n  got  {}\n  want {}", name, rows, canon(want.at("rows"))));
        const auto build = std::pair{made_of(w.engine, *db, "lane.tfake.build", false), made_of(w.engine, *db, "lane.tfake.build", true)};
        expect(build.first == want.at("build").at(0).get<std::size_t>() && build.second == want.at("build").at(1).get<std::size_t>(),
               std::format("stage {}: the build kept/made {}/{}", name, build.first, build.second));
        expect(w.kind->take_judged() == want.at("judged").get<std::vector<std::string>>(), "stage " + name + ": the functions judged");
        expect(got(w, db, "lane.tfake.texts") == canon(want.at("texts")), "stage " + name + ": the texts");
        expect(got(w, db, "lane.tfake.keys") == canon(want.at("keys")), "stage " + name + ": the keys");
        for (const std::string& k : keys)
            expect(got(w, db, "lane.tfake.built", k) == canon(want.at("built").at(k)), std::format("stage {}: built {}", name, k));
    }
    // a module given functions by hand is prepared again with them (the first preparation serves the module as the lane made it)
    CB_CHECK_EQ(w.kind->prepares, 3);   // the first run; the comment; the change to a function's code (and one added) -- the rest from the first
});

CB_TEST("engine.lanes.judged_is_the_rows_as_records", [] {
    World w("2 7 4 9");
    const Judged j = w.ls.judged(w.binary);
    CB_CHECK_EQ(j.profile, "p1");
    CB_CHECK_EQ(j.header, "typedef int int4;");
    CB_CHECK_EQ(j.rows.size(), std::size_t{5});
    CB_CHECK_EQ(j.rows[0].addr, "ffff");
    CB_CHECK(j.rows[0].verdict == verify::Verdict::NoSource);
    CB_CHECK(!j.rows[0].source.has_value());
    CB_CHECK_EQ(j.rows[0].problem.value_or(""), "none");
    CB_CHECK(j.rows[1].verdict == verify::Verdict::Equivalent);
    CB_CHECK_EQ(j.rows[1].name, "f0");
    CB_CHECK_EQ(j.rows[1].source.value_or(""), "int f0(void) { return 2; }");
    CB_CHECK_EQ(j.rows[1].limits, "coverage=100");
    CB_CHECK(j.rows[2].verdict == verify::Verdict::Divergent);
    CB_CHECK_EQ(j.built.size(), std::size_t{4});
    CB_CHECK(!j.made.empty());
    CB_CHECK(w.ls.kinds() == (std::vector<std::string>{"tfake"}));
    CB_CHECK(w.ls.on(w.binary));
});

CB_TEST("engine.lanes.a_new_process_asks_again_and_nothing_is_prepared_built_or_judged", [] {
    {
        World w("2 7 4 9");
        got(w, w.db(), "lane.tfake.rows");
        // (a second engine on the same databases, with its own lanes object: the facts are all it has)
        Steps steps2;
        knowledge::ReadDigests digests2([](const knowledge::Read&) { return std::string(knowledge::kAbsent); });
        Engine engine2(steps2, w.databases, digests2, 4);
        Lanes ls2(steps2, engine2, [](const std::filesystem::path&) { return std::optional<std::string>("tfake"); });
        auto kind2 = std::make_shared<FakeKind>();
        Setup setup;
        setup.kind = kind2;
        setup.platform = "tfake";
        setup.traits.per_function = true;
        ls2.declare(setup);
        const Judged again = ls2.judged(w.binary);
        CB_CHECK_EQ(again.rows.size(), std::size_t{5});
        CB_CHECK_EQ(kind2->prepares, 0);
        CB_CHECK_EQ(kind2->builds, 0);
        CB_CHECK(kind2->take_judged().empty());
        // the profile changed and the build's files gone: made again there, every function judged again
        kind2->digest = "p2";
        std::filesystem::remove_all(facts::Databases::work_folder(*engine2.open(w.binary), "lane"));
        const Judged third = ls2.judged(w.binary);
        CB_CHECK_EQ(third.profile, "p2");
        CB_CHECK_EQ(kind2->prepares, 1);   // (the module was not live in this object: prepared again)
        CB_CHECK_EQ(kind2->builds, 1);
        CB_CHECK(kind2->take_judged() == (std::vector<std::string>{"0000", "0001", "0002", "0003"}));
        CB_CHECK(third.rows[1].verdict == verify::Verdict::Equivalent);
    }
});

// ---- what the steps do on the way ----------------------------------------------------------------------------------------------------

CB_TEST("engine.lanes.a_good_function_that_reaches_a_stand_in_gets_stub_callee", [] {
    World w("2 7 f3() x");   // f2 (even key, good) calls f3, which does not compile
    const Judged j = w.ls.judged(w.binary);
    std::map<std::string, JudgedRow> by;
    for (const JudgedRow& r : j.rows) by[r.addr] = r;
    CB_CHECK(by.at("0002").verdict == verify::Verdict::Equivalent);
    CB_CHECK_EQ(by.at("0002").limits, "coverage=100,stub-callee");
    CB_CHECK_EQ(by.at("0000").limits, "coverage=100");
});

CB_TEST("engine.lanes.a_judge_that_falls_over_is_not_run_and_the_others_go_on", [] {
    auto kind = std::make_shared<FakeKind>();
    kind->crash_on = "f1";
    World w("2 7 4 9", kind);
    const Judged j = w.ls.judged(w.binary);
    std::map<std::string, JudgedRow> by;
    for (const JudgedRow& r : j.rows) by[r.addr] = r;
    CB_CHECK(by.at("0001").verdict == verify::not_run().verdict);
    CB_CHECK(by.at("0000").verdict == verify::Verdict::Equivalent);
    CB_CHECK(by.at("0003").verdict == verify::Verdict::Divergent);
});

CB_TEST("engine.lanes.a_lane_that_does_not_build_per_function_files_every_function_under_the_modules_signature", [] {
    World w("2 7 4 9", nullptr, /*per_function=*/false);
    const Judged j = w.ls.judged(w.binary);
    for (const JudgedRow& r : j.rows)
        if (r.addr != "ffff") CB_CHECK_EQ(r.sig, "whole:2 7 4 9");
    CB_CHECK(j.built.empty());   // (no per-function objects: nothing is built per function)
});

CB_TEST("engine.lanes.a_verdict_is_judged_in_a_build_not_asked_for_alone", [] {
    World w("2 7 4 9");
    const auto db = w.db();
    CB_CHECK_THROWS_AS(UsageError, w.engine.get(db, "lane.tfake.verdict", "0000"));
});

CB_TEST("engine.lanes.a_folder_module_has_a_database_named_by_what_its_lane_files_its_results_under", [] {
    auto kind = std::make_shared<FakeKind>();
    fs::ScopedTempDir dir("lanes3");
    World w("1", kind);
    const std::filesystem::path folder = dir.path() / "program";
    std::filesystem::create_directories(folder);
    fs::write_text(folder / "words.txt", "2 7");
    Setup setup;
    setup.kind = kind;
    setup.platform = "tfake";
    setup.traits.per_function = true;
    setup.traits.sha = [](const std::filesystem::path&) { return std::optional<std::string>("abc123"); };
    w.ls.declare(setup);
    const auto db = w.ls.open_module(folder);
    CB_CHECK_EQ(db->sha, "abc123");
    CB_CHECK(db->path().filename() == "abc123.sqlite");
    CB_CHECK_EQ(db->about("name").value_or(""), "program");
    const auto row = db->row("binary", "");
    CB_CHECK(row.has_value() && row->given());
    CB_CHECK_EQ(row->hash, "sha256:abc123");
    const Judged j = w.ls.judged(folder);
    CB_CHECK_EQ(j.rows.size(), std::size_t{3});   // f0, f1 and ffff
    // opened again: the binary's row is as it was
    const double made = row->made;
    CB_CHECK_EQ(w.ls.open_module(folder)->row("binary", "")->made, made);
    // a lane that says nothing of where its results go cannot open a folder
    World plain("1");
    CB_CHECK_THROWS_AS(UsageError, plain.ls.open_module(folder));
});

CB_TEST("engine.lanes.the_errors_of_declaring_and_asking", [] {
    World w("2 7");
    CB_CHECK_THROWS_AS(UsageError, w.ls.declare(Setup{}));
    CB_CHECK_THROWS_AS(UsageError, w.ls.declare_pe_image(nullptr));   // no lane "pe" yet
    Lanes none(w.steps, w.engine, [](const std::filesystem::path&) { return std::optional<std::string>(); });
    CB_CHECK(!none.on(w.binary));
    CB_CHECK_THROWS_AS(UsageError, none.judged(w.binary));
    CB_CHECK_THROWS_AS(UsageError, none.build_pe(w.binary));
});

// ---- the module's document -------------------------------------------------------------------------------------------------------

CB_TEST("engine.lanes.a_module_document_round_trips_and_a_hand_made_text_changes_the_sig_as_recorded", [] {
    ModuleDoc d;
    d.header = "typedef int int4;";
    d.order = {"f0", "f1", "f2"};
    d.key = {{"f0", "0000"}, {"f1", "0001"}, {"f2", "0002"}};
    d.all_keys = {"0000", "0001", "0002", "ffff"};
    d.texts = {{"f0", "int f0(void) { return 2; }"}, {"f1", "int f1(void) { return 7; }"}, {"f2", "int f2(void) { return 4; }"}};
    d.sig = "whole:2 7 4 9";
    const KV v = to_value(d);
    const ModuleDoc back = module_doc_of(v);
    CB_CHECK_EQ(back.header, d.header);
    CB_CHECK(back.order == d.order);
    CB_CHECK(back.key == d.key);
    CB_CHECK(back.all_keys == d.all_keys);
    CB_CHECK(back.texts == d.texts);
    CB_CHECK_EQ(back.sig, d.sig);
    CB_CHECK_EQ(canon(to_value(back)), canon(v));
    CB_CHECK(patched(d, {}).sig == d.sig);
    const ModuleDoc p = patched(d, {{"f2", "int f2(void) { return 4; } /* by hand */"}, {"f1", "int f1(void) { return 8; }"}});
    CB_CHECK_EQ(p.sig, "469089398c183be8e44328fe2eab737258ac7c66");   // hashlib.sha1("%s\0%s" % (sig, "".join("%s\0%s\0" % kv for kv in sorted(given.items()))))
    CB_CHECK_EQ(p.texts.at("f1"), "int f1(void) { return 8; }");
    CB_CHECK_EQ(d.texts.at("f1"), "int f1(void) { return 7; }");   // the original is as it was
    CB_CHECK_THROWS_AS(FormatError, module_doc_of(KV(std::int64_t{1})));
    CB_CHECK_THROWS_AS(FormatError, module_doc_of(KV(KV::Map{{"header", KV(std::int64_t{1})}})));
});

// ---- pe.image -----------------------------------------------------------------------------------------------------------------------

CB_TEST("engine.lanes.pe_image_links_the_module_and_build_pe_puts_the_image_where_asked", [] {
    auto kind = std::make_shared<FakeKind>("pe");
    fs::ScopedTempDir dir("lanespe");
    facts::Databases databases(facts::Config{dir.path() / "db", 1 * facts::kGiB});
    Steps steps;
    knowledge::ReadDigests digests([](const knowledge::Read&) { return std::string(knowledge::kAbsent); });
    Engine engine(steps, databases, digests, 4);
    Lanes L(steps, engine, [](const std::filesystem::path&) { return std::optional<std::string>("pe"); });
    Setup setup;
    setup.kind = kind;
    setup.platform = "win32";
    setup.traits.per_function = true;
    L.declare(setup);
    int links = 0;
    L.declare_pe_image([&](const Prepared& module, const std::filesystem::path& binary, const std::filesystem::path& folder) {
        ++links;
        Linked l;
        l.name = "thing.bin";
        std::string all;
        for (const std::string& n : module.order) all += module.texts.at(n);
        l.image = Bytes(all.begin(), all.end());
        l.rejected["f1"] = "no good";
        l.report = "1 function rejected";
        CB_CHECK(std::filesystem::is_directory(folder));
        CB_CHECK(binary.filename() == "THING.BIN");
        return l;
    });
    const std::filesystem::path binary = dir.path() / "THING.BIN";
    fs::write_text(binary, "2 7");
    const PeBuild b = L.build_pe(binary, dir.path() / "out");
    CB_CHECK_EQ(b.name, "thing.bin");
    CB_CHECK_EQ(b.report, "1 function rejected");
    CB_CHECK_EQ(b.rejected.at("f1"), "no good");
    CB_CHECK(b.image.has_value());
    CB_CHECK(*b.image == dir.path() / "out" / "thing.bin");
    CB_CHECK_EQ(fs::read_text(*b.image), "int f0(void) { return 2; }int f1(void) { return 7; }");
    CB_CHECK_EQ(links, 1);
    // asked again: nothing is linked again; the build folder cleared: linked again
    L.build_pe(binary);
    CB_CHECK_EQ(links, 1);
    std::filesystem::remove_all(facts::Databases::work_folder(*engine.open(binary), "build"));
    const PeBuild again = L.build_pe(binary);
    CB_CHECK_EQ(links, 2);
    CB_CHECK(std::filesystem::is_regular_file(*again.image));
    // a function given by hand is in the link
    engine.put(*engine.open(binary), "lane.pe.function", "0001", facts::Value(KV(KV::List{KV("f1"), KV("int f1(void) { return 9; }")})));
    const PeBuild hand = L.build_pe(binary, dir.path() / "out");
    CB_CHECK_EQ(links, 3);
    CB_CHECK_EQ(fs::read_text(*hand.image), "int f0(void) { return 2; }int f1(void) { return 9; }");
    // not the lane "pe"
    Lanes other(steps, engine, [](const std::filesystem::path&) { return std::optional<std::string>("ne"); });
    CB_CHECK_THROWS_AS(UsageError, other.build_pe(binary));
});
