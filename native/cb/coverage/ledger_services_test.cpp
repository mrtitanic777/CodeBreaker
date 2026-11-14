// Tests of coverage::ledger::services_over: the `coverage` command built over the corpus and the function store of a state folder, against the
// recorded output of `codebreaker coverage ...` for the same three-module corpus and no function store yet (testdata/commands/*.txt; the folders
// are <STATE> and <CORPUS>). The verifier settings (the tools of this machine) are given to the services as the digests the recording shows.
#include <filesystem>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "cb/base/corpus.hpp"
#include "cb/coverage/ledger.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/store/functions.hpp"

namespace {

namespace stdfs = std::filesystem;

struct CoverageCommandWorld {
    cb::fs::ScopedTempDir dir{"cb-coverage-command"};
    cb::fs::ScopedEnv no_name{"CB_CORPUS_NAME", std::nullopt};
    cb::fs::ScopedEnv no_data{"WT_DATA_ROOT", std::nullopt};
    cb::fs::ScopedEnv no_store{"CB_FUNCTIONS_DB", std::nullopt};
    std::optional<cb::fs::ScopedEnv> root_env;
    stdfs::path corpus_dir, state;
    std::unique_ptr<cb::base::Corpus> corpus;

    CoverageCommandWorld() {
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

    cb::coverage::ledger::Services services() const {
        return cb::coverage::ledger::services_over(*corpus, state, [] {
            return std::map<std::string, std::optional<std::string>>{
                {"pe", "1d3e92132994"},
                {"ne", "856575d18ee9"}};
        });
    }

    // What the command prints and returns as the golden files have it: '/' in paths, `exit=N` last.
    std::string run(std::vector<std::string> args) {
        cb::testing::CapturedStd cap;
        cb::cli::Cli cli("codebreaker", {cb::coverage::ledger::command(services())});
        args.insert(args.begin(), "coverage");
        const int code = cli.run(args);
        std::string out = cap.out();
        for (char& c : out)
            if (c == '\\') c = '/';
        return out + "exit=" + std::to_string(code) + "\n";
    }

    std::string golden(const char* name) const {
        std::string text = cb::fs::read_text(cb::testing::data_path(std::string("coverage/testdata/commands/") + name + ".txt"));
        text = cb::text::replace_all(text, "<STATE>", slashed(state));
        return cb::text::replace_all(text, "<CORPUS>", slashed(corpus_dir));
    }

    static std::string slashed(const stdfs::path& p) { return cb::fs::to_text(p); }
};

}  // namespace

CB_TEST("coverage.services.are_the_state_folders_store", [] {
    CoverageCommandWorld w;
    const cb::coverage::ledger::Services s = w.services();
    CB_CHECK(s.corpus == w.corpus.get());
    CB_CHECK(s.state_dir == w.state);
    CB_CHECK(s.store_index == cb::store::functions::default_path(w.state));
    CB_CHECK(s.store_folder == cb::store::functions::databases_folder(w.state));
    CB_CHECK(s.verifier_profiles != nullptr);
});

CB_TEST("coverage.services.the_listing_matches_the_recording", [] {
    CoverageCommandWorld w;
    CB_CHECK_EQ(w.run({"--all"}), w.golden("all"));
    CB_CHECK_EQ(w.run({}), w.golden("default"));
});

CB_TEST("coverage.services.one_module_and_an_unknown_one", [] {
    CoverageCommandWorld w;
    CB_CHECK_EQ(w.run({"--module", "thinkjet.drv"}), w.golden("module_thinkjet"));
    CB_CHECK_EQ(w.run({"--module", "nothing"}), w.golden("module_unknown"));
});
