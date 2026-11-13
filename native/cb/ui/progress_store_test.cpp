// Tests of ui::progress::store_reader: what `codebreaker progress` reads of the coverage ledger and the function store. With no store the line is
// what `python -m codebreaker progress --once` printed over the same three-module corpus (its clock aside); with results in the store the reading says
// how far the modules are, which was written last and how many results came lately.
#include <chrono>
#include <filesystem>
#include <map>
#include <memory>
#include <optional>
#include <string>

#include "cb/base/corpus.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/engine/function_store.hpp"
#include "cb/formats/ne.hpp"
#include "cb/store/functions.hpp"
#include "cb/ui/progress_store.hpp"

namespace {

namespace stdfs = std::filesystem;
namespace pg = cb::ui::progress;

struct ProgressStoreWorld {
    cb::fs::ScopedTempDir dir{"cb-progress-store"};
    cb::fs::ScopedEnv no_name{"CB_CORPUS_NAME", std::nullopt};
    cb::fs::ScopedEnv no_data{"WT_DATA_ROOT", std::nullopt};
    cb::fs::ScopedEnv no_store{"CB_FUNCTIONS_DB", std::nullopt};
    std::optional<cb::fs::ScopedEnv> root_env;
    stdfs::path corpus_dir, state;
    std::unique_ptr<cb::base::Corpus> corpus;

    ProgressStoreWorld() {
        corpus_dir = dir.path() / "corpus";
        state = dir.path() / "state";
        cb::fs::make_dirs(corpus_dir);
        root_env.emplace("CB_CORPUS_ROOT", cb::fs::to_text(corpus_dir));
        for (const char* name : {"tracert.exe", "thinkjet.drv", "ti850.drv"})
            cb::fs::write_file(corpus_dir / name, cb::fs::read_file(cb::testing::data_path(std::string("compare/testdata/versions/win95/") + name)));
        cb::base::Products products;
        products.target = "win95";
        products.target_folder = corpus_dir;
        products.checkout_root = dir.path();
        products.state = state;
        cb::base::FormatProbes probes;
        probes.lane = [](const stdfs::path& f) -> std::optional<cb::base::Lane> {
            const std::string ext = cb::text::lower(cb::fs::to_text(f.extension()));
            if (ext == ".exe") return cb::base::Lane::Pe;
            if (ext == ".drv") return cb::base::Lane::Ne;
            return std::nullopt;
        };
        probes.readable = [](const stdfs::path& f) { return f; };
        corpus = std::make_unique<cb::base::Corpus>(products, probes);
    }

    pg::ReadFn reader() const {
        return pg::store_reader(cb::coverage::ledger::services_over(*corpus, state, [] {
            return std::map<std::string, std::optional<std::string>>{{"pe", "p2"}, {"ne", "p1"}, {"le", "p3"}, {"dos", "p4"}, {"java", "p5"}};
        }));
    }

    static std::string without_clock(const std::string& line) {
        const std::size_t close = line.find(']');
        return line.substr(close + 2);
    }
};

}  // namespace

CB_TEST("ui.progress_store.no_store_yet_reads_as_the_python_printed_it", [] {
    ProgressStoreWorld w;
    pg::Snapshot s = pg::snapshot(w.reader());
    // (python -m codebreaker progress --once, the same three modules and no function store; the workers busy are the machine's)
    CB_CHECK(!pg::line(s).empty());
    CB_CHECK_EQ(s.kinds.size(), std::size_t{5});   // the kinds with a verifier profile in force
    CB_CHECK_EQ(s.kinds[0].name, "32-bit");
    CB_CHECK_EQ(s.kinds[1].name, "16-bit");
    CB_CHECK_EQ(s.kinds[0].modules, std::uint64_t{1});
    CB_CHECK_EQ(s.kinds[1].modules, std::uint64_t{2});
    CB_CHECK(s.recent.empty());
    CB_CHECK(!s.eta.has_value());
    s.workers = 0;
    CB_CHECK_EQ(ProgressStoreWorld::without_clock(pg::line(s)),
                "32-bit 0/1 modules, 0 functions (nothing yet) | 16-bit 0/2 modules, 0 functions (nothing yet) | driver 0/0 modules, 0 functions (nothing yet) | DOS 0/0 modules, 0 functions (nothing yet) | Java 0/0 modules, 0 functions (nothing yet) | idle");
});

CB_TEST("ui.progress_store.results_in_the_store_are_the_progress", [] {
    ProgressStoreWorld w;
    {
        cb::engine::FunctionStore store(cb::store::functions::default_path(w.state), cb::store::functions::databases_folder(w.state));
        const stdfs::path file = w.corpus_dir / "thinkjet.drv";
        const std::string sha = w.corpus->sha(file).value();
        store.add_profile("p1", "{}");
        store.add_module(sha, "thinkjet.drv", cb::fs::to_text(file), "ne");
        const cb::formats::ne::Module m = cb::formats::ne::Module::open_file(file);
        std::string address;
        for (const auto& segment : m.segments())
            if (!segment.is_data()) {
                address = std::format("{}:0010", segment.index);
                break;
            }
        store.add_functions(sha, std::vector<std::string>{address});
        cb::engine::Record r;
        r.addr = address;
        r.name = "f";
        r.source = "int f(void){return 1;}";
        r.sig = "sig";
        r.verdict = "equivalent";
        store.record_many(sha, "decompile", "r1", "p1", std::vector<cb::engine::Record>{r});
    }
    const pg::Snapshot s = pg::snapshot(w.reader());
    CB_CHECK(s.store.ends_with("functions.sqlite"));
    CB_CHECK_EQ(s.kinds.size(), std::size_t{5});
    const pg::KindReading& ne = s.kinds[1];
    CB_CHECK_EQ(ne.measured, std::uint64_t{1});   // thinkjet.drv's one known function has a result under the profile in force
    CB_CHECK_EQ(ne.functions, std::uint64_t{1});
    CB_CHECK_EQ(ne.classes.front().title, "VERIFIED");
    CB_CHECK_EQ(ne.classes.front().count, std::uint64_t{1});
    CB_CHECK_EQ(s.recent.size(), std::size_t{1});
    CB_CHECK_EQ(s.recent[0].module, "thinkjet.drv");
    CB_CHECK_EQ(s.recent[0].kind, "16-bit");
    CB_CHECK_EQ(s.recent[0].results, std::uint64_t{1});
    CB_CHECK(s.idle.has_value() && *s.idle < std::chrono::seconds(120));
    CB_CHECK(s.active);
    CB_CHECK(s.rate > 0.0);   // the result came within the last ten minutes
    CB_CHECK(pg::line(s).find("last: thinkjet.drv") != std::string::npos);
    CB_CHECK(pg::line(s).find("16-bit 1/2 modules, 1 functions (VERIFIED 1)") != std::string::npos);
});
