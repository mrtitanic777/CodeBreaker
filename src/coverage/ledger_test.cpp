#include "coverage/ledger.hpp"

#include <algorithm>

#include <nlohmann/json.hpp>

#include "core/fs.hpp"
#include "core/json.hpp"
#include "core/testing.hpp"
#include "formats/ne.hpp"
#include "formats/pe.hpp"

namespace {

using namespace cb;
using namespace cb::coverage::ledger;
using store::Record;
using store::Class;
namespace stdfs = std::filesystem;

// Three real Windows 95 modules (copied from compare/testdata): a 32-bit program and two 16-bit drivers.
constexpr const char* kPe = "tracert.exe";
constexpr const char* kNeA = "thinkjet.drv";
constexpr const char* kNeB = "ti850.drv";

struct CodeFacts {
    std::int64_t first_start = 0;   // where the first function starts
    std::int64_t length = 0;        // how long the code section or segment is
    std::string prefix;             // "1:" for a segment; "" for a 32-bit address
    std::string address(std::int64_t offset) const {
        return prefix + std::format("{:x}", first_start + offset);
    }
    std::string ne_address(std::int64_t offset) const { return prefix + std::format("{:04x}", offset); }
};

struct World {
    fs::ScopedTempDir dir{"ledger"};
    // nothing in the environment may pick another product or another corpus folder for us
    fs::ScopedEnv no_name{"CB_CORPUS_NAME", std::nullopt};
    fs::ScopedEnv no_data{"WT_DATA_ROOT", std::nullopt};
    std::optional<fs::ScopedEnv> root_env;
    stdfs::path corpus_dir, state, index, folder;
    base::Products products;
    std::unique_ptr<base::Corpus> corpus;
    std::unique_ptr<store::FunctionStore> store;
    std::string sha_pe, sha_a, sha_b;
    CodeFacts pe, ne_a, ne_b;

    // the probes say what the corpus files are by their extension
    static base::FormatProbes probes() {
        base::FormatProbes p;
        p.lane = [](const stdfs::path& f) -> std::optional<base::Lane> {
            const std::string ext = text::lower(fs::to_text(f.extension()));
            if (ext == ".exe") return base::Lane::Pe;
            if (ext == ".drv") return base::Lane::Ne;
            return std::nullopt;
        };
        p.readable = [](const stdfs::path& f) { return f; };
        return p;
    }

    static CodeFacts ne_facts(const stdfs::path& file) {
        const formats::ne::Module m = formats::ne::Module::open_file(file);
        for (const auto& s : m.segments())
            if (!s.is_data()) return {0, s.length, std::format("{}:", s.index)};
        throw InternalError("no code segment");
    }

    World() {
        corpus_dir = dir.path() / "corpus";
        state = dir.path() / "state";
        index = dir.path() / "functions.sqlite";
        folder = dir.path() / "db";
        fs::make_dirs(corpus_dir);
        root_env.emplace("CB_CORPUS_ROOT", fs::to_text(corpus_dir));
        for (const char* name : {kPe, kNeA, kNeB})
            fs::write_file(corpus_dir / name, fs::read_file(testing::data_path(std::string("compare/testdata/versions/win95/") + name)));
        products.target = "win95";
        products.target_folder = corpus_dir;
        products.checkout_root = dir.path();
        products.state = state;
        corpus = std::make_unique<base::Corpus>(products, probes());
        store = std::make_unique<store::FunctionStore>(index, folder);
        sha_pe = corpus->sha(corpus_dir / kPe).value();
        sha_a = corpus->sha(corpus_dir / kNeA).value();
        sha_b = corpus->sha(corpus_dir / kNeB).value();
        const formats::pe::Image image = formats::pe::Image::open(corpus_dir / kPe);
        const auto sections = image.code_sections();
        pe = {static_cast<std::int64_t>(sections.front().va), static_cast<std::int64_t>(image.section_data(*sections.front().section).size()), ""};
        ne_a = ne_facts(corpus_dir / kNeA);
        ne_b = ne_facts(corpus_dir / kNeB);
    }

    Services services() const {
        Services s;
        s.corpus = corpus.get();
        s.state_dir = state;
        s.verifier_profiles = [] {
            return std::map<std::string, std::optional<std::string>>{{"pe", std::nullopt}, {"ne", "p1"}};
        };
        s.now_text = [] { return std::string("2026-11-12 10:00:00"); };
        s.store_index = index;
        s.store_folder = folder;
        return s;
    }

    static Record rec(std::string addr, std::string source, std::string verdict, std::string limits = "", std::string reason = "") {
        Record r;
        r.addr = std::move(addr);
        r.name = "f" + r.addr;
        r.source = std::move(source);
        r.sig = "sig" + r.addr;
        r.verdict = std::move(verdict);
        r.limits = std::move(limits);
        r.reason = std::move(reason);
        return r;
    }

    // thinkjet: two functions, one verified and one weak; ti850: one divergent and one nothing was made of; tracert: one
    // verified under the profile the store last used (p2); and a module in the store the corpus does not have.
    void fill() {
        store->add_profile("p1", "{}");
        store->add_profile("p2", "{}");
        store->add_module(sha_a, kNeA, fs::to_text(corpus_dir / kNeA), "ne");
        const std::vector<std::string> a = {ne_a.ne_address(0x10), ne_a.ne_address(0x50)};
        store->add_functions(sha_a, a);
        store->record_many(sha_a, "decompile", "r1", "p1",
                           std::vector<Record>{rec(a[0], "int f(void){return 1;}", "equivalent"),
                                               rec(a[1], "int g(void){return 2;}", "consistent", "low-coverage")});
        store->add_module(sha_b, kNeB, fs::to_text(corpus_dir / kNeB), "ne");
        const std::vector<std::string> b = {ne_b.ne_address(0x10), ne_b.ne_address(0x20)};
        store->add_functions(sha_b, b);
        store->record_many(sha_b, "decompile", "r1", "p1",
                           std::vector<Record>{rec(b[0], "int h(void){return 3;}", "divergent", "", "the two return different values")});
        store->add_module(sha_pe, kPe, fs::to_text(corpus_dir / kPe), "pe");
        const std::vector<std::string> p = {pe.address(0x10)};
        store->add_functions(sha_pe, p);
        store->record_many(sha_pe, "decompile", "r1", "p2", std::vector<Record>{rec(p[0], "int main(void){}", "equivalent")});
        store->add_module("gone-sha", "gone.dll", "C:/nowhere/gone.dll", "ne");
        store->add_functions("gone-sha", std::vector<std::string>{"1:0000"});
    }
};

const Row& row_of(const Report& r, const std::string& name) {
    for (const Row& row : r.modules)
        if (row.name == name) return row;
    throw InternalError("no row " + name);
}

}  // namespace

CB_TEST("coverage.ledger.extents_reach_to_the_next_function_or_the_end_of_the_section", [] {
    World w;
    const std::string a = w.pe.address(0x10), b = w.pe.address(0x40);
    const auto e = extents(w.corpus_dir / kPe, "pe", {b, a});   // given out of order
    CB_CHECK_EQ(e.size(), 2u);
    CB_CHECK_EQ(e.at(a), 0x30);
    CB_CHECK_EQ(e.at(b), w.pe.length - 0x40);

    const auto n = extents(w.corpus_dir / kNeA, "ne", {w.ne_a.ne_address(0x50), w.ne_a.ne_address(0x10)});
    CB_CHECK_EQ(n.at(w.ne_a.ne_address(0x10)), 0x40);
    CB_CHECK_EQ(n.at(w.ne_a.ne_address(0x50)), w.ne_a.length - 0x50);
});

CB_TEST("coverage.ledger.extents_leave_out_what_does_not_lie_in_the_code", [] {
    World w;
    const std::string inside = w.ne_a.ne_address(0x10);
    const auto e = extents(w.corpus_dir / kNeA, "ne",
                           {inside, "not-an-address", "9:0010", w.ne_a.ne_address(w.ne_a.length + 5), "1:zz"});
    // one address that parses and lies in the code: it reaches to the segment's end
    CB_CHECK_EQ(e.size(), 1u);
    CB_CHECK_EQ(e.at(inside), w.ne_a.length - 0x10);
    // a 32-bit address outside every code section, and one that is not hexadecimal
    CB_CHECK(extents(w.corpus_dir / kPe, "pe", {"1", "zzzz"}).empty());
    // a file that is not there, a file that is not that kind, a kind that has no extents
    CB_CHECK(extents(w.dir.path() / "missing.exe", "pe", {"1000"}).empty());
    CB_CHECK(extents(w.corpus_dir / kNeA, "pe", {"10"}).empty());
    CB_CHECK(extents(w.corpus_dir / kPe, "ne", {"1:0010"}).empty());
    CB_CHECK(extents(w.corpus_dir / kPe, "dos", {"10"}).empty());
});

CB_TEST("coverage.ledger.the_corpus_modules_by_name_with_their_code_size_remembered", [] {
    World w;
    coverage::ledger::Ledger ledger(w.services(), nullptr);
    const auto mods = ledger.corpus_modules();
    CB_CHECK_EQ(mods.size(), 3u);
    CB_CHECK_EQ(mods[0].name, kNeA);   // thinkjet.drv < ti850.drv < tracert.exe
    CB_CHECK_EQ(mods[1].name, kNeB);
    CB_CHECK_EQ(mods[2].name, kPe);
    CB_CHECK_EQ(mods[0].kind, "ne");
    CB_CHECK_EQ(mods[2].kind, "pe");
    CB_CHECK_EQ(mods[2].size, static_cast<std::int64_t>(fs::file_size(w.corpus_dir / kPe)));
    CB_CHECK(mods[2].code_bytes > 0);

    // the sizes are in the memo; a changed remembered size shows that an unchanged file is served from it ...
    const stdfs::path memo = w.state / "coverage_corpus.json";
    CB_CHECK(stdfs::exists(memo));
    Json j = json::load(memo);
    const std::string key = mods[2].path;
    CB_CHECK(j.contains(key));
    j[key][2] = 123456;
    json::save(memo, j, json::Style::Compact);
    CB_CHECK_EQ(ledger.corpus_modules()[2].code_bytes, 123456);
    // ... and --refresh reads the file again
    CB_CHECK_EQ(ledger.corpus_modules(/*refresh*/ true)[2].code_bytes, mods[2].code_bytes);
    // a damaged memo is read as none
    fs::write_text(memo, "{not json");
    CB_CHECK_EQ(ledger.corpus_modules()[2].code_bytes, mods[2].code_bytes);
});

CB_TEST("coverage.ledger.counts_come_from_the_store_by_the_best_candidates_class", [] {
    World w;
    w.fill();
    coverage::ledger::Ledger ledger(w.services(), w.store.get());
    const Report r = ledger.measure();
    CB_CHECK_EQ(r.modules.size(), 4u);   // the three corpus modules and the one only the store has

    const Row& a = row_of(r, kNeA);
    CB_CHECK(a.in_store);
    CB_CHECK_EQ(a.sha.value(), w.sha_a);
    CB_CHECK_EQ(a.functions, 2);
    CB_CHECK_EQ(a.decompiled, 2);
    CB_CHECK_EQ(a.compiled, 2);
    CB_CHECK_EQ(a.judged, 2);
    CB_CHECK(a.measured);
    CB_CHECK_EQ(a.classes.at(Class::Verified), 1);
    CB_CHECK_EQ(a.classes.at(Class::Weak), 1);
    CB_CHECK_EQ(a.bytes_known, w.ne_a.length - 0x10);
    CB_CHECK_EQ(a.bytes_verified, 0x40);
    CB_CHECK_EQ(a.bytes_weak, w.ne_a.length - 0x50);
    CB_CHECK(a.code_bytes > 0);
    CB_CHECK_EQ(std::string(state_of(a)), "measured");

    // two functions, one of which nothing was made of: no source, so the module is only part measured
    const Row& b = row_of(r, kNeB);
    CB_CHECK_EQ(b.functions, 2);
    CB_CHECK_EQ(b.decompiled, 1);
    CB_CHECK_EQ(b.compiled, 1);
    CB_CHECK_EQ(b.judged, 1);
    CB_CHECK(!b.measured);
    CB_CHECK_EQ(b.classes.at(Class::Unverified), 1);
    CB_CHECK_EQ(b.classes.at(Class::NoSource), 1);
    CB_CHECK_EQ(b.bytes_verified, 0);
    CB_CHECK_EQ(std::string(state_of(b)), "part measured");

    // the 32-bit program was judged under p2, the profile the store last used for that kind (the verifier had none)
    CB_CHECK_EQ(r.profiles.at("pe").value(), "p2");
    CB_CHECK_EQ(row_of(r, kPe).classes.at(Class::Verified), 1);

    // a module only the store has: its kind is the store's, and it has no size
    const Row& gone = row_of(r, "gone.dll");
    CB_CHECK(gone.in_store);
    CB_CHECK_EQ(gone.kind, "ne");
    CB_CHECK_EQ(gone.size, 0);
    CB_CHECK_EQ(gone.functions, 1);
    CB_CHECK_EQ(gone.classes.at(Class::NoSource), 1);
    CB_CHECK_EQ(gone.bytes_known, 0);

    // the kinds and the total add the rows up
    CB_CHECK_EQ(r.kinds.size(), kKinds.size());
    CB_CHECK_EQ(r.kinds[0].name, "32-bit");
    CB_CHECK_EQ(r.kinds[0].modules, 1);
    CB_CHECK_EQ(r.kinds[1].modules, 3);
    CB_CHECK_EQ(r.kinds[1].functions, 5);
    CB_CHECK_EQ(r.total.modules, 4);
    CB_CHECK_EQ(r.total.in_store, 4);
    CB_CHECK_EQ(r.total.functions, 6);
    CB_CHECK_EQ(r.total.measured, 2);
    CB_CHECK_EQ(r.total.classes.at(Class::Verified), 2);
    CB_CHECK_EQ(r.generated, "2026-11-12 10:00:00");
    CB_CHECK_EQ(r.corpus, fs::to_text(w.corpus_dir));
});

CB_TEST("coverage.ledger.a_module_not_in_the_store_is_untouched", [] {
    World w;   // the store is empty
    coverage::ledger::Ledger ledger(w.services(), w.store.get());
    const Report r = ledger.measure();
    CB_CHECK_EQ(r.modules.size(), 3u);
    for (const Row& row : r.modules) {
        CB_CHECK(!row.in_store);
        CB_CHECK_EQ(std::string(state_of(row)), "untouched");
        CB_CHECK_EQ(row.functions, 0);
        CB_CHECK(row.code_bytes > 0);
    }
    // without a store at all it is the same
    coverage::ledger::Ledger none(w.services(), nullptr);
    CB_CHECK_EQ(none.measure().modules.size(), 3u);
});

CB_TEST("coverage.ledger.one_module_by_name_with_or_without_its_extension", [] {
    World w;
    w.fill();
    coverage::ledger::Ledger ledger(w.services(), w.store.get());
    CB_CHECK_EQ(ledger.measure("thinkjet.drv").modules.size(), 1u);
    CB_CHECK_EQ(ledger.measure("THINKJET").modules.size(), 1u);
    CB_CHECK_EQ(ledger.measure("C:/somewhere/gone.dll").modules.size(), 1u);   // only the store has it
    CB_CHECK_EQ(ledger.measure("nothing").modules.size(), 0u);
    CB_CHECK_EQ(ledger.measure("thinkjet").total.modules, 1);
});

CB_TEST("coverage.ledger.counts_follow_the_store_when_its_summary_changes", [] {
    World w;
    w.fill();
    coverage::ledger::Ledger ledger(w.services(), w.store.get());
    CB_CHECK_EQ(row_of(ledger.measure(), kNeB).classes.at(Class::Verified), 0);
    // the second function of ti850 gets judged: the module's summary changes, and so do its counts
    w.store->record_many(w.sha_b, "decompile", "r1", "p1",
                         std::vector<Record>{World::rec(w.ne_b.ne_address(0x20), "int k(void){}", "equivalent")});
    const Report r = ledger.measure();
    CB_CHECK_EQ(row_of(r, kNeB).classes.at(Class::Verified), 1);
    CB_CHECK(row_of(r, kNeB).measured);
    CB_CHECK_EQ(row_of(r, kNeB).decompiled, 2);
});

CB_TEST("coverage.ledger.reasons_say_why_functions_are_not_verified", [] {
    World w;
    w.fill();
    coverage::ledger::Ledger ledger(w.services(), w.store.get());
    const auto b = ledger.reasons(w.sha_b, "p1");
    CB_CHECK_EQ(b.size(), 1u);
    CB_CHECK(b[0].first == Class::Unverified);
    CB_CHECK_EQ(b[0].second[0].first, "the two return different values");
    CB_CHECK_EQ(b[0].second[0].second, 1);
    const auto a = ledger.reasons(w.sha_a, "p1");   // the weak one: why it is weak
    CB_CHECK_EQ(a.size(), 1u);
    CB_CHECK(a[0].first == Class::Weak);
    CB_CHECK_EQ(a[0].second[0].first, "under half of the original's instructions ran");
    // a long reason is cut at 70 bytes
    World w2;
    w2.fill();
    const std::string long_reason(100, 'x');
    w2.store->record_many(w2.sha_b, "decompile", "r1", "p1",
                          std::vector<Record>{World::rec(w2.ne_b.ne_address(0x20), "int k(void){}", "divergent", "", long_reason)});
    coverage::ledger::Ledger l2(w2.services(), w2.store.get());
    const auto cut = l2.reasons(w2.sha_b, "p1");
    CB_CHECK_EQ(cut[0].second.size(), 2u);
    CB_CHECK_EQ(cut[0].second[0].first.size() == 70 || cut[0].second[1].first.size() == 70, true);
});

CB_TEST("coverage.ledger.the_text_of_the_ledger", [] {
    World w;
    w.fill();
    coverage::ledger::Ledger ledger(w.services(), w.store.get());
    const Report r = ledger.measure();
    const std::vector<std::string> lines = render(r);
    CB_CHECK_EQ(lines[0], "Coverage of " + fs::to_text(w.corpus_dir));
    CB_CHECK_EQ(lines[1], "verifier settings in force: 32-bit p2, 16-bit p1");
    CB_CHECK_EQ(lines[3], "Functions, by kind of module");
    CB_CHECK(lines[4].starts_with("kind    modules  in store  measured  functions  decompiled  compiled"));
    CB_CHECK(lines[4].ends_with("DOES NOT COMPILE  NO SOURCE  verified %"));
    // the 16-bit row: 3 modules (thinkjet, ti850, and the store's gone.dll), 5 functions, 2 of them decompiled in thinkjet...
    CB_CHECK(lines[6].starts_with("16-bit"));
    const auto joined = [&] {
        std::string s;
        for (const auto& l : lines) s += l + "\n";
        return s;
    }();
    CB_CHECK(joined.find("Modules the store holds (--all lists the rest)") != std::string::npos);
    CB_CHECK(joined.find("\nthinkjet.drv  16-bit") != std::string::npos);
    CB_CHECK(joined.find("part measured") != std::string::npos);
    CB_CHECK(joined.find("not measured now") != std::string::npos);
    CB_CHECK(joined.find("gone.dll") != std::string::npos);
    // no line ends in a blank
    for (const auto& l : lines) CB_CHECK(l.empty() || l.back() != ' ');
    // --all lists modules nothing was done to: with an empty store
    World empty;
    coverage::ledger::Ledger none(empty.services(), empty.store.get());
    const auto quiet = render(none.measure());
    CB_CHECK_EQ(quiet.back(), "(none)");
    CB_CHECK(render(none.measure(), /*every*/ true).size() > quiet.size());
});

CB_TEST("coverage.ledger.percent_and_table", [] {
    CB_CHECK_EQ(percent(1, 4), " 25.0%");
    CB_CHECK_EQ(percent(1, 3), " 33.3%");
    CB_CHECK_EQ(percent(100, 100), "100.0%");
    CB_CHECK_EQ(percent(5, 0), "     -");
    const auto t = table({"name", "n"}, {{"alpha", "1"}, {"b", "200"}});
    CB_CHECK_EQ(t.size(), 3u);
    CB_CHECK_EQ(t[0], "name     n");
    CB_CHECK_EQ(t[1], "alpha    1");
    CB_CHECK_EQ(t[2], "b      200");
    CB_CHECK_EQ(table({"", "x"}, {}).front(), "  x");   // the head alone; a leading blank column keeps its width
});

CB_TEST("coverage.ledger.the_json_and_the_csv", [] {
    World w;
    w.fill();
    coverage::ledger::Ledger ledger(w.services(), w.store.get());
    const Report r = ledger.measure();
    const Json j = to_json(r);
    CB_CHECK_EQ(j["generated"].get<std::string>(), "2026-11-12 10:00:00");
    CB_CHECK_EQ(j["profiles"]["ne"].get<std::string>(), "p1");
    CB_CHECK_EQ(j["total"]["functions"].get<int>(), 6);
    CB_CHECK_EQ(j["kinds"].size(), kKinds.size());
    CB_CHECK_EQ(j["modules"].size(), 4u);
    CB_CHECK_EQ(j["modules"][1]["name"].get<std::string>(), kNeA);
    CB_CHECK_EQ(j["modules"][1]["verified"].get<int>(), 1);
    CB_CHECK_EQ(j["modules"][1]["not-judged"].get<int>(), 0);
    CB_CHECK(j["modules"][1].contains("sha"));
    CB_CHECK(j["modules"][0]["in_store"].get<bool>());

    const stdfs::path csv = w.dir.path() / "ledger.csv";
    CB_CHECK_EQ(write_csv(r, csv), 4u);
    const std::string text = fs::read_text(csv);
    const auto lines = text::lines(text);
    CB_CHECK_EQ(lines.size(), 5u);
    CB_CHECK_EQ(std::string(lines[0]),
                "name,kind,state,path,size,functions,decompiled,compiled,verified,weak,unverified,not-judged,no-compile,no-source,code_bytes,"
                "bytes_known,bytes_verified,bytes_weak,judged");
    CB_CHECK(std::string(lines[2]).starts_with("thinkjet.drv,16-bit,measured,"));
    CB_CHECK(text.find("\r\n") != std::string::npos);
});

CB_TEST("coverage.ledger.the_row_of_one_module", [] {
    World w;
    w.fill();
    coverage::ledger::Ledger ledger(w.services(), w.store.get());
    const Report r = ledger.measure("ti850");
    const auto lines = describe_row(r.modules[0], r, ledger);
    CB_CHECK(lines[0].starts_with("ti850.drv  (16-bit, part measured)  "));
    CB_CHECK(lines[1].find("functions") != std::string::npos && lines[1].ends_with("share"));
    CB_CHECK(lines[2].starts_with("functions"));
    bool by_cause = false, why = false;
    for (const auto& l : lines) {
        by_cause |= l == "UNVERIFIED, by cause:";
        why |= l == "      1  the two return different values";
    }
    CB_CHECK(by_cause && why);
});

CB_TEST("coverage.ledger.the_command", [] {
    World w;
    w.fill();
    cli::Cli cli("codebreaker", {command(w.services())});
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(cli.run({"coverage"}), 0);
        CB_CHECK(cap.out().find("Functions, by kind of module") != std::string::npos);
    }
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(cli.run({"coverage", "--module", "ti850"}), 0);
        CB_CHECK(cap.out().find("UNVERIFIED, by cause:") != std::string::npos);
    }
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(cli.run({"coverage", "--module", "nothing"}), 2);
        CB_CHECK(cap.out().find("no module 'nothing' in the corpus or the store") != std::string::npos);
    }
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(cli.run({"coverage", "--json"}), 0);
        const Json j = json::parse(cap.out());
        CB_CHECK_EQ(j["total"]["modules"].get<int>(), 4);
    }
    {
        testing::CapturedStd cap;
        const std::string csv = fs::to_text(w.dir.path() / "out.csv");
        CB_CHECK_EQ(cli.run({"coverage", "--csv", csv}), 0);
        CB_CHECK(cap.out().find("wrote 4 modules to") != std::string::npos);
        CB_CHECK(stdfs::exists(w.dir.path() / "out.csv"));
    }
    // no function store yet: the corpus is listed alone
    World bare;
    Services without = bare.services();
    without.store_index = bare.dir.path() / "nothing-yet.sqlite";
    cli::Cli bare_cli("codebreaker", {command(without)});
    testing::CapturedStd cap;
    CB_CHECK_EQ(bare_cli.run({"coverage"}), 0);
    CB_CHECK(cap.out().find("there is no function store yet") != std::string::npos);
    CB_CHECK(cap.out().find("Modules") != std::string::npos);
});
