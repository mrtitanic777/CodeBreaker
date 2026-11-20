// Tests of cb/compare/versions.hpp on real Windows 95 and Windows 98 builds of the same modules (testdata/versions/): a PE (TRACERT.EXE, 9 KB
// against 20 KB), two printer drivers (NE) and a DOS driver (DBLBUFF.SYS).
//
// The goldens in testdata/versions/golden/ are the documents the original (Python) comparison wrote for the same pairs; what this code writes
// agrees with them in every pair, every level and score, every listing and every opcode (50 comparisons of 18 module pairs of the old harness
// were compared document against document, and the six kept here were read through). The sequence matcher and the digest are checked against
// difflib's and hashlib's answers, written out below.
//
// DIVERGES from the Python module:
//  - the document's `reconstructions`, `sha` and `corpus` come from a Ledger the caller gives (the Python opened the global store and corpus);
//  - an error is an Error of the vocabulary (UnsupportedError for a kind that is not compared, UsageError for a bad command line), not a
//    ValueError or SystemExit;
//  - the Python's `argv.index(opt)+1` raised IndexError for an option given last; the command parser says the option needs a value.

#include "compare/versions.hpp"

#include <map>
#include <string>

#include <nlohmann/json.hpp>

#include "base/corpus.hpp"
#include "compare/versions_internal.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"
#include "formats/detect.hpp"

namespace v = cb::compare::versions;
namespace det = cb::compare::versions::detail;
using std::string;

namespace {

std::filesystem::path data(const string& name) { return cb::testing::data_path("compare/testdata/versions/" + name); }
nlohmann::json golden(const string& name) {
    return nlohmann::json::parse(cb::fs::read_text(cb::testing::data_path("compare/testdata/versions/golden/" + name)));
}

// the document as the goldens have it: no timing, no paths or hashes of the machine it ran on
nlohmann::json scrubbed(const string& json) {
    nlohmann::json d = nlohmann::json::parse(json);
    d.erase("seconds");
    d.erase("reconstructions");
    for (const char* side : {"from", "to"})
        for (const char* k : {"path", "sha", "corpus"}) d[side].erase(k);
    return d;
}

// which top-level keys of two documents differ (and the first pair that does), for the failure message
string differing(const nlohmann::json& got, const nlohmann::json& want) {
    string out;
    for (auto it = want.begin(); it != want.end(); ++it) {
        if (!got.contains(it.key())) { out += it.key() + " (missing) "; continue; }
        if (got[it.key()] == it.value()) continue;
        out += it.key() + " ";
        if (it.value().is_array())
            for (std::size_t i = 0; i < std::max(got[it.key()].size(), it.value().size()); ++i)
                if (i >= got[it.key()].size() || i >= it.value().size() || got[it.key()][i] != it.value()[i]) {
                    out += "[" + std::to_string(i) + "] got " + (i < got[it.key()].size() ? got[it.key()][i].dump() : "-") + " want " + (i < it.value().size() ? it.value()[i].dump() : "-") + " ";
                    break;
                }
        else out += "got " + got[it.key()].dump() + " want " + it.value().dump() + " ";
    }
    return out;
}

// a ledger that remembers what it was asked and answers from tables
class FakeLedger : public v::Ledger {
public:
    std::map<string, v::Ledger::Known> known;           // for the old build
    std::map<string, string> bodies_of_new;              // {address: digest}
    std::map<string, string> named;                      // digest -> name given
    std::vector<string> name_sources;
    struct Filed {
        Identity identity;
        string module;
        std::vector<string> addresses;
        string rev;
        std::vector<Carried> rows;
    };
    std::vector<Filed> filed;
    bool identified = true;

    std::optional<string> sha(const std::filesystem::path& m) override { return m.parent_path().filename() == "win98" ? "new-sha-0123456789abcdef" : "old-sha-0123456789abcdef"; }
    std::optional<string> owner(const std::filesystem::path& m) override { return m.parent_path().filename().string(); }
    std::optional<Identity> identity(const std::filesystem::path& m) override {
        if (!identified) return std::nullopt;
        return Identity{*sha(m), m.parent_path().filename() == "win98" ? "profile-new" : "profile-old"};
    }
    std::map<string, Known> best(const Identity&) override { return known; }
    std::map<string, string> bodies(const string&) override { return bodies_of_new; }
    void name_body(const string& digest, const string& name, const string& source) override {
        named[digest] = name;
        name_sources.push_back(source);
    }
    void file_carried(const Identity& id, const string& module_name, const std::filesystem::path&, v::Kind, std::span<const string> addresses,
                      const string& rev, std::span<const Carried> rows) override {
        filed.push_back({id, module_name, std::vector<string>(addresses.begin(), addresses.end()), rev, std::vector<Carried>(rows.begin(), rows.end())});
    }
};

}  // namespace

CB_TEST("compare.versions.digest_is_blake2b_of_eight_bytes", [] {
    // hashlib.blake2b(data, digest_size=8).hexdigest()
    CB_CHECK_EQ(det::hex_of(det::blake2b8("")), string("e4a6a0577479b2b4"));
    CB_CHECK_EQ(det::hex_of(det::blake2b8("abc")), string("d8bb14d833d59559"));
    CB_CHECK_EQ(det::hex_of(det::blake2b8(string(128, 'a'))), string("f06643fe9c7e18da"));   // one full block
    CB_CHECK_EQ(det::hex_of(det::blake2b8(string(129, 'a'))), string("e462535bb0c5a299"));   // one past it
    CB_CHECK_EQ(det::hex_of(det::blake2b8("line1\nline2\nline3")), string("4b63d7ad58103fa4"));
});

CB_TEST("compare.versions.sequence_matcher_is_difflibs", [] {
    struct Case {
        std::vector<int> a, b;
        std::vector<std::array<int, 3>> blocks;
        std::vector<std::array<int, 5>> ops;   // tag (0 equal 1 replace 2 delete 3 insert), i1, i2, j1, j2
        double ratio, quick, real_quick;
    };
    const Case cases[] = {
        {{1, 2, 3, 4, 5}, {1, 2, 9, 4, 5}, {{0, 0, 2}, {3, 3, 2}, {5, 5, 0}}, {{0, 0, 2, 0, 2}, {1, 2, 3, 2, 3}, {0, 3, 5, 3, 5}}, 0.8, 0.8, 1.0},
        {{1, 2, 3}, {4, 5, 6}, {{3, 3, 0}}, {{1, 0, 3, 0, 3}}, 0.0, 0.0, 1.0},
        {{}, {1, 2}, {{0, 2, 0}}, {{3, 0, 0, 0, 2}}, 0.0, 0.0, 0.0},
        {{1, 1, 2, 2, 3}, {2, 2, 1, 1, 3, 3}, {{0, 2, 2}, {4, 4, 1}, {5, 6, 0}},
         {{3, 0, 0, 0, 2}, {0, 0, 2, 2, 4}, {2, 2, 4, 4, 4}, {0, 4, 5, 4, 5}, {3, 5, 5, 5, 6}}, 0.545455, 0.909091, 0.909091},
        {{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, {0, 1, 2, 7, 8, 9, 3, 4, 5, 6}, {{0, 0, 3}, {3, 6, 4}, {10, 10, 0}},
         {{0, 0, 3, 0, 3}, {3, 3, 3, 3, 6}, {0, 3, 7, 6, 10}, {2, 7, 10, 10, 10}}, 0.7, 1.0, 1.0}};
    for (const Case& c : cases) {
        det::SeqMatcher m(c.a, c.b);
        const auto blocks = m.matching_blocks();
        CB_CHECK_EQ(blocks.size(), c.blocks.size());
        for (std::size_t i = 0; i < std::min(blocks.size(), c.blocks.size()); ++i)
            CB_CHECK(blocks[i].i == c.blocks[i][0] && blocks[i].j == c.blocks[i][1] && blocks[i].k == c.blocks[i][2]);
        const auto ops = m.opcodes();
        CB_CHECK_EQ(ops.size(), c.ops.size());
        for (std::size_t i = 0; i < std::min(ops.size(), c.ops.size()); ++i)
            CB_CHECK(static_cast<int>(ops[i].tag) == c.ops[i][0] && ops[i].i1 == c.ops[i][1] && ops[i].i2 == c.ops[i][2] && ops[i].j1 == c.ops[i][3] &&
                     ops[i].j2 == c.ops[i][4]);
        CB_CHECK(std::abs(m.ratio() - c.ratio) < 1e-6);
        CB_CHECK(std::abs(m.quick_ratio() - c.quick) < 1e-6);
        CB_CHECK(std::abs(m.real_quick_ratio() - c.real_quick) < 1e-6);
    }
    // the interner numbers strings densely, the same string the same
    det::Interner in;
    CB_CHECK_EQ(in.of("a"), 0);
    CB_CHECK_EQ(in.of("b"), 1);
    CB_CHECK_EQ(in.of("a"), 0);
    CB_CHECK(in.all({"b", "c", "a"}) == (std::vector<int>{1, 2, 0}));
});

CB_TEST("compare.versions.names_of_levels_and_hows", [] {
    for (v::Level l : v::kLevels) CB_CHECK(v::level_from_name(v::level_name(l)) == l);
    CB_CHECK(!v::level_from_name("loose"));
    CB_CHECK_EQ(string(v::how_name(v::How::Likeness)), string("likeness"));
    CB_CHECK_EQ(string(v::kind_name(v::Kind::Dos)), string("dos"));
    CB_CHECK_EQ(string(v::status_name(v::ModuleStatus::SameFile)), string("same file"));
});

CB_TEST("compare.versions.load_reads_each_kind", [] {
    const v::Side pe = v::load(data("win95/tracert.exe"));
    CB_CHECK(pe.kind() == v::Kind::Pe);
    CB_CHECK_EQ(pe.name(), string("tracert.exe"));
    CB_CHECK_EQ(pe.count(), 29u);
    CB_CHECK(pe.size() > 2000);
    CB_CHECK(pe.code() >= pe.size());
    const v::Function& first = pe.functions().front();
    CB_CHECK(first.count() > 0 && first.rows.size() == first.count() && first.text.size() == first.count());
    CB_CHECK_EQ(first.ids.bytes.size(), 16u);   // eight bytes of digest, in hex
    CB_CHECK(pe.find(first.key) == &first);
    CB_CHECK(!pe.find("nonesuch"));
    const v::Side ne = v::load(data("win95/ti850.drv"));
    CB_CHECK(ne.kind() == v::Kind::Ne);
    CB_CHECK(ne.functions().front().key.find(':') != string::npos);   // "segment:offset"
    bool named = false;
    for (const v::Function& f : ne.functions()) named = named || f.named;
    CB_CHECK(named);   // a driver exports its entry points
    const v::Side dos = v::load(data("win95/dblbuff.sys"));
    CB_CHECK(dos.kind() == v::Kind::Dos);
    CB_CHECK(dos.count() > 0);
    // the callers of a function are the functions that call it, by index
    std::size_t called = 0;
    for (std::size_t i = 0; i < pe.count(); ++i) called += pe.callers_of(i).size();
    CB_CHECK(called > 0);
    for (std::size_t i = 0; i < pe.count(); ++i)
        for (std::size_t c : pe.callers_of(i)) {
            bool calls = false;
            for (const v::Call& call : pe.function(c).calls) calls = calls || (call.to == v::Call::To::Function && call.key == pe.function(i).key);
            CB_CHECK(calls);
        }
});

CB_TEST("compare.versions.a_file_that_is_not_compared_is_unsupported", [] {
    cb::fs::ScopedTempDir dir("versions");
    cb::fs::write_text(dir.path() / "notes.txt", "not a program");
    CB_CHECK_THROWS_AS(cb::UnsupportedError, v::load(dir.path() / "notes.txt"));
    CB_CHECK_THROWS_AS(cb::UnsupportedError, v::load(dir.path() / "missing.exe"));
    // a PE cut off in its headers is damaged
    cb::Bytes cut = cb::fs::read_file(data("win95/tracert.exe"));
    cut.resize(0x90);
    cb::fs::write_file(dir.path() / "cut.exe", cut);
    CB_CHECK_THROWS_AS(cb::Error, v::load(dir.path() / "cut.exe"));
});

CB_TEST("compare.versions.a_build_is_identical_to_itself", [] {
    for (const char* name : {"tracert.exe", "ti850.drv", "dblbuff.sys"}) {
        const v::Comparison c = v::compare(data(string("win95/") + name), data(string("win95/") + name));
        const v::Counts n = c.counts();
        CB_CHECK_EQ(n.identical, static_cast<std::int64_t>(c.from().count()));
        CB_CHECK_EQ(n.changed + n.added + n.removed, 0);
        CB_CHECK_EQ(n.identical_bytes, n.identical);
        for (const v::Pair& p : c.pairs()) CB_CHECK(p.identical && p.score == 1.0 && p.level == v::Level::Bytes);
        const v::CodeBytes code = c.code();
        CB_CHECK_EQ(code.identical, code.to_bytes);
    }
});

CB_TEST("compare.versions.windows_95_against_windows_98_is_what_the_original_wrote", [] {
    for (const char* name : {"tracert.exe", "ti850.drv", "thinkjet.drv"}) {
        const v::Comparison c = v::compare(data(string("win95/") + name), data(string("win98/") + name));
        CB_CHECK_EQ(differing(scrubbed(c.json(false)), golden(string(name) + ".compare.json")), string());
    }
    // the DOS driver against itself (its golden is that comparison)
    CB_CHECK_EQ(differing(scrubbed(v::compare(data("win95/dblbuff.sys"), data("win95/dblbuff.sys")).json(false)), golden("dblbuff.sys.compare.json")), string());
    // with the listings: each changed function's instructions side by side, and the opcodes between them
    const v::Comparison c = v::compare(data("win95/ti850.drv"), data("win98/ti850.drv"));
    CB_CHECK(scrubbed(c.json(true)) == golden("ti850.drv.listings.json"));
    // the pairs say how they were found
    const v::Comparison t = v::compare(data("win95/tracert.exe"), data("win98/tracert.exe"));
    CB_CHECK(t.counts().changed > 0 && t.counts().added > 0 && t.counts().removed > 0);
    bool by_calls = false, by_bytes = false;
    for (const v::Pair& p : t.pairs()) {
        by_calls = by_calls || p.how == v::How::Calls;
        by_bytes = by_bytes || p.how == v::How::Bytes;
        CB_CHECK(p.score >= 0 && p.score <= 1);
        CB_CHECK(p.identical || p.score < 1);
    }
    CB_CHECK(by_bytes);
    CB_CHECK(!t.report().empty() && t.report().find("tracert.exe: 29 functions -> tracert.exe") == 0);
    CB_CHECK(t.report().find("(") != string::npos);
});

CB_TEST("compare.versions.the_least_likeness_keeps_looser_pairs_out", [] {
    const v::Comparison loose = v::compare(data("win95/tracert.exe"), data("win98/tracert.exe"), 0.3);
    const v::Comparison strict = v::compare(data("win95/tracert.exe"), data("win98/tracert.exe"), 0.95);
    CB_CHECK(loose.pairs().size() >= strict.pairs().size());
    CB_CHECK_EQ(loose.pairs().size() + loose.removed().size(), strict.pairs().size() + strict.removed().size());   // every old function is paired or removed
});

CB_TEST("compare.versions.likeness_of_two_functions", [] {
    const v::Side a = v::load(data("win95/tracert.exe")), b = v::load(data("win98/tracert.exe"));
    const v::Function& f = a.functions().front();
    CB_CHECK_EQ(v::likeness(f, f), 1.0);
    double lowest = 1.0;
    for (const v::Function& g : b.functions()) {
        const double l = v::likeness(f, g);
        CB_CHECK(l >= 0.0 && l <= 1.0);
        lowest = std::min(lowest, l);
    }
    CB_CHECK(lowest < 1.0);
});

CB_TEST("compare.versions.functions_shared_by_programs", [] {
    const std::filesystem::path paths[] = {data("win95/ti850.drv"), data("win98/ti850.drv"), data("win95/thinkjet.drv")};
    const std::vector<v::Shared> found = v::shared(paths, v::Level::Text, 4);
    const nlohmann::json want = golden("shared_text_4.json");
    CB_CHECK_EQ(found.size(), want.size());
    for (std::size_t i = 0; i < std::min<std::size_t>(found.size(), want.size()); ++i) {
        CB_CHECK_EQ(static_cast<std::int64_t>(found[i].instructions), want[i][0].get<std::int64_t>());
        nlohmann::json where = nlohmann::json::object();
        for (const auto& [name, keys] : found[i].where) where[name] = keys;
        CB_CHECK(where == want[i][1]);
    }
    // sorted largest first, and only what two programs or more have
    for (std::size_t i = 1; i < found.size(); ++i) CB_CHECK(found[i - 1].instructions >= found[i].instructions);
    for (const v::Shared& s : found) CB_CHECK(s.where.size() >= 2);
    // one program alone shares nothing; a high bar leaves nothing
    CB_CHECK(v::shared(std::span<const std::filesystem::path>(paths, 1)).empty());
    CB_CHECK(v::shared(paths, v::Level::Text, 100000).empty());
});

CB_TEST("compare.versions.names_and_reconstructions_carry_across_builds", [] {
    const v::Comparison c = v::compare(data("win95/tracert.exe"), data("win98/tracert.exe"));
    FakeLedger ledger;
    // the old build has a verified reconstruction of every function it has an address for, a weak one, one unverified, one with no source
    std::vector<string> identical;
    for (const v::Pair& p : c.pairs())
        if (p.identical) identical.push_back(p.from);
    CB_CHECK(identical.size() >= 4);
    ledger.known[identical[0]] = {"verified", "core", "main", "int main() { return 0; }"};
    ledger.known[identical[1]] = {"weak", std::nullopt, std::nullopt, "int g() { return 1; }"};
    ledger.known[identical[2]] = {"unverified", "core", "h", "int h() { return 2; }"};
    ledger.known[identical[3]] = {"verified", "core", "k", ""};
    string changed_from;
    for (const v::Pair& p : c.pairs())
        if (!p.identical) {
            changed_from = p.from;
            break;
        }
    ledger.known[changed_from] = {"verified", "core", "c", "int c() { return 3; }"};   // changed in the new build: does not carry
    const auto found = c.reconstructions(ledger);
    CB_CHECK_EQ(found.size(), 2u);
    CB_CHECK_EQ(found[0].klass, string("verified"));
    CB_CHECK_EQ(found[0].from, identical[0]);
    CB_CHECK(found[0].engine && *found[0].engine == "core");
    CB_CHECK(!found[1].engine && !found[1].name);
    // the document lists them
    const nlohmann::json doc = nlohmann::json::parse(c.json(false, &ledger));
    CB_CHECK_EQ(doc["reconstructions"].size(), 2u);
    CB_CHECK_EQ(doc["from"]["corpus"].get<string>(), string("win95"));
    CB_CHECK_EQ(doc["to"]["sha"].get<string>(), string("new-sha-0123456789abcdef"));
    // filing them for the new build
    CB_CHECK_EQ(c.carry(ledger), 2u);
    CB_CHECK_EQ(ledger.filed.size(), 1u);
    const auto& f = ledger.filed[0];
    CB_CHECK_EQ(f.identity.sha, string("new-sha-0123456789abcdef"));
    CB_CHECK_EQ(f.identity.profile, string("profile-new"));
    CB_CHECK_EQ(f.rev, string("old-sha-0123"));
    CB_CHECK_EQ(f.module, string("tracert.exe"));
    CB_CHECK_EQ(f.addresses.size(), c.to().count());
    CB_CHECK_EQ(f.rows.size(), 2u);
    CB_CHECK(f.rows[0].reason.find("carried: identical to tracert.exe ") == 0 && f.rows[0].reason.find("(verified there); not judged here") != string::npos);
    CB_CHECK(f.rows[1].reason.find("(weak there)") != string::npos);
    // a name the old build has for a function the new one calls sub_...: carried onto identical bodies the store knows
    const auto names = c.names();
    ledger.bodies_of_new.clear();
    CB_CHECK(!c.carry_names(ledger));   // no bodies recorded for the new build
    for (const v::NameRow& n : names) ledger.bodies_of_new[n.to] = "digest-" + n.to;
    if (!names.empty()) {
        const auto n = c.carry_names(ledger);
        CB_CHECK(n.has_value());
        std::size_t onto = 0;
        for (const v::NameRow& r : names) onto += r.identical;
        CB_CHECK_EQ(*n, onto);
        for (const string& s : ledger.name_sources) CB_CHECK(s == "the same function in tracert.exe of win95");
    }
    // a build the store has no identity for carries nothing
    ledger.identified = false;
    CB_CHECK(c.reconstructions(ledger).empty());
    CB_CHECK_EQ(c.carry(ledger), 0u);
});

namespace {
// Two products on disk made of the modules above: the old one's and the new one's.
struct TwoProducts {
    cb::fs::ScopedTempDir dir{"versions"};
    std::filesystem::path old_dir, new_dir;
    cb::fs::ScopedEnv no_name{"CB_CORPUS_NAME", std::nullopt};

    void put(const std::filesystem::path& where, const string& file, const string& name) {
        cb::fs::write_file(where / name, cb::fs::read_file(data(file)));
    }
    TwoProducts() {
        old_dir = dir.path() / "old";
        new_dir = dir.path() / "new";
        put(old_dir, "win95/tracert.exe", "tracert.exe");
        put(new_dir, "win98/tracert.exe", "tracert.exe");          // compared
        put(old_dir, "win95/ti850.drv", "ti850.drv");
        put(new_dir, "win95/ti850.drv", "ti850.drv");              // the same file
        put(new_dir, "win98/thinkjet.drv", "thinkjet.drv");        // a new module
        put(old_dir, "win95/tracert.exe", "damaged.exe");
        cb::Bytes cut = cb::fs::read_file(data("win98/tracert.exe"));
        cut.resize(0x240);                                          // headers and a little code: a PE that cannot be read
        cb::fs::write_file(new_dir / "damaged.exe", cut);
    }
    cb::base::Products products() const {
        cb::base::Products p;
        p.target = "old";
        p.target_folder = old_dir;
        p.listed["old"] = old_dir;
        p.listed["new"] = new_dir;
        p.checkout_root = dir.path() / "checkout";
        p.state = dir.path() / "state";
        return p;
    }
    static cb::base::FormatProbes probes() {
        cb::base::FormatProbes f;
        f.lane = [](const std::filesystem::path& p) -> std::optional<cb::base::Lane> {
            const auto lane = cb::formats::detect::lane(p);
            if (!lane) return std::nullopt;
            return static_cast<cb::base::Lane>(static_cast<int>(*lane));
        };
        return f;
    }
};
}  // namespace

CB_TEST("compare.versions.a_whole_product", [] {
    TwoProducts t;
    cb::base::Corpus corpus(t.products(), TwoProducts::probes());
    std::vector<string> told;
    const v::Product p = v::product(corpus, "old", "new", 0, 1, 0.6, [&](const string& line) { told.push_back(line); });
    CB_CHECK_EQ(p.modules.size(), 4u);
    std::map<string, v::ModuleRow> by_name;
    for (const v::ModuleRow& r : p.modules) by_name[r.name] = r;
    CB_CHECK(by_name["tracert.exe"].status == v::ModuleStatus::Compared);
    CB_CHECK(by_name["tracert.exe"].functions && *by_name["tracert.exe"].identical + *by_name["tracert.exe"].changed + *by_name["tracert.exe"].added == *by_name["tracert.exe"].functions);
    CB_CHECK(by_name["ti850.drv"].status == v::ModuleStatus::SameFile);
    CB_CHECK_EQ(*by_name["ti850.drv"].identical, *by_name["ti850.drv"].functions);
    CB_CHECK_EQ(*by_name["ti850.drv"].changed, 0);
    CB_CHECK(by_name["thinkjet.drv"].status == v::ModuleStatus::NewModule);
    CB_CHECK_EQ(*by_name["thinkjet.drv"].added, *by_name["thinkjet.drv"].functions);
    CB_CHECK(by_name["damaged.exe"].status == v::ModuleStatus::Failed);
    CB_CHECK(by_name["damaged.exe"].error && !by_name["damaged.exe"].error->empty());
    // sorted by name; the totals add the rows up, keyed in the order first met
    CB_CHECK(p.modules[0].name == "damaged.exe" && p.modules[3].name == "tracert.exe");
    CB_CHECK_EQ(p.total("modules"), 4);
    CB_CHECK_EQ(p.total("compared"), 1);
    CB_CHECK_EQ(p.total("failed"), 1);
    CB_CHECK_EQ(p.total("functions"), static_cast<std::int64_t>(*by_name["tracert.exe"].functions + *by_name["ti850.drv"].functions + *by_name["thinkjet.drv"].functions));
    CB_CHECK_EQ(p.totals.front().first, string("modules"));
    CB_CHECK_EQ(told.size(), 4u);
    CB_CHECK(told[0].find("damaged.exe") != string::npos && told[0].find("FAILED") != string::npos);
    // a sample takes modules evenly spread; more than there are takes all
    const v::Product one = v::product(corpus, "old", "new", 1, 1, 0.6, [](const string&) {});
    CB_CHECK_EQ(one.modules.size(), 1u);
    CB_CHECK_EQ(v::product(corpus, "old", "new", 99, 2, 0.6, [](const string&) {}).modules.size(), 4u);
    // the document
    const nlohmann::json doc = nlohmann::json::parse(v::product_json(p, 1.5));
    CB_CHECK_EQ(doc["totals"]["seconds"].get<double>(), 1.5);
    CB_CHECK_EQ(doc["modules"].size(), 4u);
    CB_CHECK_EQ(doc["modules"][0]["status"].get<string>(), string("failed"));
    CB_CHECK(!doc["modules"][0].contains("functions"));
});

CB_TEST("compare.versions.the_command", [] {
    TwoProducts t;
    cb::base::Corpus corpus(t.products(), TwoProducts::probes());
    FakeLedger ledger;
    const cb::cli::Cli cli("codebreaker", {v::command(corpus, ledger)});
    const string a = (t.old_dir / "tracert.exe").string(), b = (t.new_dir / "tracert.exe").string();
    {   // two files
        cb::testing::CapturedStd cap;
        CB_CHECK_EQ(cli.run({"versions", a, b}), 0);
        const string out = cap.out();
        CB_CHECK(out.starts_with("tracert.exe: 29 functions -> tracert.exe: "));
        CB_CHECK(out.find("names carried to the new build: ") != string::npos);
    }
    {   // one module in two products, the document written
        cb::testing::CapturedStd cap;
        const string json_path = (t.dir.path() / "doc.json").string();
        CB_CHECK_EQ(cli.run({"versions", "tracert.exe", "--from", "old", "--to", "new", "--json", json_path}), 0);
        CB_CHECK(cap.out().find("wrote " + json_path) != string::npos);
        const nlohmann::json doc = nlohmann::json::parse(cb::fs::read_text(json_path));
        CB_CHECK(doc.contains("listings") && doc.contains("pairs") && doc["counts"]["identical"].is_number());
    }
    {   // a whole product
        cb::testing::CapturedStd cap;
        CB_CHECK_EQ(cli.run({"versions", "--all", "--from", "old", "--to", "new", "--jobs", "1"}), 1);   // one module failed
        CB_CHECK(cap.out().find("4 modules (1 compared, 1 the same file, 1 new, 1 failed)") != string::npos);
    }
    {   // the functions programs share
        cb::testing::CapturedStd cap;
        const string p1 = (t.old_dir / "ti850.drv").string(), p2 = (t.new_dir / "thinkjet.drv").string();
        CB_CHECK_EQ(cli.run({"versions", "--shared", p1, p2, "--level", "shape"}), 0);
        CB_CHECK(cap.out().find("functions in two programs or more (shape)") != string::npos);
        CB_CHECK(cap.out().find("largest (instructions: program@keys):") != string::npos);
    }
    {   // nothing to compare: the usage, and the code of a command line that was wrong
        cb::testing::CapturedStd cap;
        CB_CHECK_EQ(cli.run({"versions"}), 2);
        CB_CHECK(cap.out().find("codebreaker versions <module> --from CORPUS --to CORPUS") != string::npos);
    }
    {
        cb::testing::CapturedStd cap;
        CB_CHECK_THROWS_AS(cb::UsageError, cli.run({"versions", "--shared", a}));                       // needs two programs
        CB_CHECK_THROWS_AS(cb::UsageError, cli.run({"versions", "--shared", a, b, "--level", "bogus"}));
        CB_CHECK_THROWS_AS(cb::UsageError, cli.run({"versions", "nosuch.dll", "--from", "old", "--to", "new"}));
        CB_CHECK_THROWS_AS(cb::UsageError, cli.run({"versions", a, b, "--least", "x"}));
        CB_CHECK_THROWS_AS(cb::UsageError, cli.run({"versions", a, b, "--sample", "x", "--all", "--from", "old", "--to", "new"}));
    }
    {   // carrying: reconstructions and names are filed for the new build
        cb::testing::CapturedStd cap;
        const v::Comparison c = v::compare(a, b);
        for (const v::Pair& p : c.pairs())
            if (p.identical) ledger.known[p.from] = {"verified", "core", std::nullopt, "int f() { return 0; }"};
        CB_CHECK_EQ(cli.run({"versions", a, b, "--carry"}), 0);
        CB_CHECK(cap.out().find("filed ") != string::npos && cap.out().find("reconstructions for tracert.exe in the store") != string::npos);
        CB_CHECK(cap.out().find("none -- its bodies are not recorded there yet") != string::npos);
        CB_CHECK_EQ(ledger.filed.size(), 1u);
    }
});
