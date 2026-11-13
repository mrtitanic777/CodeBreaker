// cb-unity: alone (test helpers named for this module)
// Tests of cb/source/vxd/corpus and loadable_corpus: what is a driver of a folder, the resumable results files, and a run of the whole-driver build over a
// folder of the real drivers of formats/testdata (SPLITTER.VXD, SRAMMTD.VXD). The expected records are the Python modules' on the same files.
#include "cb/source/vxd/corpus.hpp"

#include <nlohmann/json.hpp>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/detect.hpp"
#include "cb/source/vxd/loadable_corpus.hpp"

namespace vx = cb::source::vxd;
namespace cp = cb::source::vxd::corpus;
namespace lc = cb::source::vxd::loadable_corpus;

namespace {

std::filesystem::path corpus_testdata(const char* name) { return cb::testing::data_path(std::string("formats/testdata/") + name); }

// A folder with the drivers a corpus has under their names, and what is not a driver.
void corpus_make_folder(const std::filesystem::path& folder) {
    cb::fs::make_dirs(folder);
    std::filesystem::copy_file(corpus_testdata("splitter.vxd"), folder / "SPLITTER.VXD");
    std::filesystem::copy_file(corpus_testdata("srammtd.vxd"), folder / "srammtd.386");   // a driver under another extension
    std::filesystem::copy_file(corpus_testdata("lzexpand.dll"), folder / "lzexpand.dll");   // not a driver
    std::filesystem::copy_file(corpus_testdata("find.exe"), folder / "find.pdr");           // not a driver, whatever the name
    cb::fs::write_text(folder / "notes.txt", "nothing");
}

struct CorpusFixture {
    cb::fs::ScopedEnv no_name{"CB_CORPUS_NAME", std::nullopt};
    cb::fs::ScopedEnv root;
    cb::fs::ScopedEnv no_data{"WT_DATA_ROOT", std::nullopt};
    std::unique_ptr<cb::base::Corpus> corpus;
    explicit CorpusFixture(const std::filesystem::path& folder) : root("CB_CORPUS_ROOT", cb::fs::to_text(folder)) {
        cb::base::Products products;
        products.target = "win95";
        products.target_folder = folder;
        products.checkout_root = folder.parent_path() / "CodeBreaker";
        products.state = folder.parent_path() / "state";
        cb::base::FormatProbes probes;
        probes.lane = [](const std::filesystem::path& p) -> std::optional<cb::base::Lane> {
            const auto lane = cb::formats::detect::lane(p);
            if (!lane) return std::nullopt;
            switch (*lane) {
            case cb::formats::detect::Lane::Pe: return cb::base::Lane::Pe;
            case cb::formats::detect::Lane::Ne: return cb::base::Lane::Ne;
            case cb::formats::detect::Lane::Le: return cb::base::Lane::Le;
            case cb::formats::detect::Lane::Dos: return cb::base::Lane::Dos;
            }
            return std::nullopt;
        };
        corpus = std::make_unique<cb::base::Corpus>(products, probes);
    }
};
nlohmann::ordered_json corpus_load(const std::filesystem::path& file) { return nlohmann::ordered_json::parse(cb::fs::read_text(file)); }

}  // namespace

CB_TEST("source.vxd.corpus.the_drivers_of_a_folder", [] {
    cb::fs::ScopedTempDir dir("corpustest");
    corpus_make_folder(dir.path() / "drivers");
    const std::vector<std::filesystem::path> files = cp::driver_files(dir.path() / "drivers");
    std::vector<std::string> names;
    for (const auto& f : files) names.push_back(cb::fs::to_text(f.filename()));
    CB_CHECK(names == (std::vector<std::string>{"SPLITTER.VXD", "srammtd.386"}));   // sorted by path; a file that holds no driver is left out
    CB_CHECK(cp::driver_files(dir.path() / "nothing-here").empty());
});

CB_TEST("source.vxd.corpus.redo_names", [] {
    CB_CHECK(cp::redo_set("") .empty());
    CB_CHECK(cp::redo_set("vkd,vmm") == (std::set<std::string>{"vkd", "vmm"}));
    CB_CHECK(cp::redo_set("a") == (std::set<std::string>{"a"}));
});

CB_TEST("source.vxd.corpus.a_recorded_module_is_skipped_and_redo_does_it_again", [] {
    cb::fs::ScopedTempDir dir("corpustest");
    corpus_make_folder(dir.path() / "drivers");
    const vx::DdkNames names;
    const vx::masm::Toolchain no_tools;   // nothing to assemble with: a module that is done again says so
    const CorpusFixture world(dir.path() / "drivers");
    const cp::Environment env{names, no_tools, *world.corpus, dir.path() / "default.json"};
    const std::string results = cb::fs::to_text(dir.path() / "results.json");
    // the file has splitter (done) and srammtd (an error, which is done again)
    cb::fs::write_text(dir.path() / "results.json",
                       R"({"splitter.vxd": {"byte_exact": true, "n_obj": 1, "n_exact": 1, "objects": [], "symbolic": 5, "forced_raw": 0, "raw_fallback_objs": 0, "seconds": 0.1},
                           "srammtd.vxd": {"error": "boom"}})");
    {
        cb::testing::CapturedStd cap;
        CB_CHECK_EQ(cp::main(env, std::vector<std::string>{cb::fs::to_text(dir.path() / "drivers"), results, "--workers", "2"}), cb::kExitOk);
        const std::string said = cap.out();
        CB_CHECK(said.find("== 1 modules to process, 2 workers ==") != std::string::npos);   // (not splitter)
        CB_CHECK(said.find("== corpus: ") != std::string::npos);
    }
    const nlohmann::ordered_json after = corpus_load(dir.path() / "results.json");
    CB_CHECK(after["splitter.vxd"]["seconds"] == 0.1);   // untouched
    CB_CHECK(after.contains("srammtd.vxd") && !after["srammtd.vxd"].contains("boom"));
    // --redo splitter.vxd does it again too
    {
        cb::testing::CapturedStd cap;
        CB_CHECK_EQ(cp::main(env, std::vector<std::string>{cb::fs::to_text(dir.path() / "drivers"), results, "--redo", "splitter.vxd"}), cb::kExitOk);
        CB_CHECK(cap.out().find("== 1 modules to process, 4 workers ==") != std::string::npos);
    }
});

CB_TEST("source.vxd.loadable_corpus.splitter_and_srammtd_build", [] {
    cb::fs::ScopedTempDir dir("corpustest");
    corpus_make_folder(dir.path() / "drivers");
    const vx::DdkNames names;
    const vx::masm::Toolchain no_tools;
    const vx::link::Environment build{names, no_tools, [](cb::ByteView, std::string_view, const std::filesystem::path&) { return vx::link::Linked{}; }};
    const CorpusFixture world(dir.path() / "drivers");
    const lc::Environment env{build, *world.corpus, dir.path() / "loadable.json"};
    std::string said;
    {
        cb::testing::CapturedStd cap;
        CB_CHECK_EQ(lc::main(env, std::vector<std::string>{}), cb::kExitOk);   // the corpus's folder, the default results file
        said = cap.out();
    }
    CB_CHECK(said.find("== 2 modules to build, 4 workers ==") != std::string::npos);
    CB_CHECK(said.find("== loadable corpus: 2/2 BUILD | 2 device-id-correct | 2 identity-map") != std::string::npos);
    const nlohmann::ordered_json results = corpus_load(dir.path() / "loadable.json");
    CB_CHECK_EQ(results.size(), std::size_t{2});
    // the Python module's record of splitter
    const nlohmann::ordered_json& splitter = results["splitter"];
    CB_CHECK(splitter["ok"] == true);
    CB_CHECK(splitter["stage"].is_null());
    CB_CHECK(splitter["objects"] == 1);
    CB_CHECK(splitter["device_id"] == 0x0498);
    CB_CHECK(splitter["device_id_ok"] == true);
    CB_CHECK(splitter["identity"] == true);
    CB_CHECK(splitter["resource_len"] == 512);
    CB_CHECK(splitter["vxd_len"] == 2596);
    CB_CHECK(results["srammtd"]["ok"] == true);
    // asked again, it skips both
    {
        cb::testing::CapturedStd cap;
        CB_CHECK_EQ(lc::main(env, std::vector<std::string>{}), cb::kExitOk);
        CB_CHECK(cap.out().find("== 0 modules to build") != std::string::npos);
    }
});
