#include "cb/base/runs.hpp"

#include "cb/core/regex.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

using namespace cb;
using namespace cb::base;
namespace stdfs = std::filesystem;

CB_TEST("base.runs.start_makes_a_named_folder_under_the_tools_runs", [] {
    fs::ScopedTempDir state("cbruns");
    const stdfs::path a = runs::start(state.path(), "phase3", "my tag/here");
    CB_CHECK(stdfs::is_directory(a));
    CB_CHECK_EQ(a.parent_path().filename().string(), "runs");
    CB_CHECK_EQ(a.parent_path().parent_path().filename().string(), "phase3");
    const std::string name = a.filename().string();
    CB_CHECK(name.starts_with("my-tag-here_"));
    // <tag>_<yyyymmdd-hhmmss>_<pid>
    static const Regex shape(R"(my-tag-here_\d{8}-\d{6}_\d+)");
    CB_CHECK(shape.full_match(name).has_value());
    // the same name asked for twice in a second is not shared
    const stdfs::path b = runs::start(state.path(), "phase3", "my tag/here");
    CB_CHECK(a != b);
    CB_CHECK(stdfs::is_directory(b));
});

CB_TEST("base.runs.the_tag_is_cleaned_and_cut", [] {
    fs::ScopedTempDir state("cbruns");
    CB_CHECK(runs::start(state.path(), "t", "").filename().string().starts_with("run_"));
    CB_CHECK(runs::start(state.path(), "t", "///").filename().string().starts_with("-_"));
    CB_CHECK(runs::start(state.path(), "t", "a  b\\c").filename().string().starts_with("a-b-c_"));
    CB_CHECK(runs::start(state.path(), "t", "v1.2-x").filename().string().starts_with("v1.2-x_"));
    const std::string long_tag(100, 'q');
    const std::string name = runs::start(state.path(), "t", long_tag).filename().string();
    CB_CHECK(name.starts_with(std::string(40, 'q') + "_"));
});

CB_TEST("base.runs.folder_is_deleted_at_scope_end_even_by_an_exception", [] {
    fs::ScopedTempDir state("cbruns");
    stdfs::path kept;
    {
        runs::Folder work(state.path(), "phase3", "ok");
        kept = work.path();
        fs::write_text(kept / "disk.img", "bytes");
        CB_CHECK(stdfs::exists(kept / "disk.img"));
    }
    CB_CHECK(!stdfs::exists(kept));
    try {
        runs::Folder work(state.path(), "phase3", "boom");
        kept = work.path();
        throw InternalError("the run failed");
    } catch (const InternalError&) {
    }
    CB_CHECK(!stdfs::exists(kept));
});

CB_TEST("base.runs.sweep_removes_only_old_run_folders", [] {
    fs::ScopedTempDir state("cbruns");
    const stdfs::path runs_dir = state.path() / "vxdboot" / "runs";
    fs::make_dirs(runs_dir / "old_run");
    fs::make_dirs(runs_dir / "fresh_run");
    fs::write_text(runs_dir / "old_run" / "copy.img", "x");
    fs::write_text(runs_dir / "stray.txt", "a file, not a run folder");
    stdfs::last_write_time(runs_dir / "old_run", stdfs::file_time_type::clock::now() - std::chrono::hours(30));
    stdfs::last_write_time(runs_dir / "stray.txt", stdfs::file_time_type::clock::now() - std::chrono::hours(30));
    const auto gone = runs::sweep(state.path(), "vxdboot");
    CB_CHECK_EQ(gone.size(), 1u);
    CB_CHECK_EQ(gone[0].filename().string(), "old_run");
    CB_CHECK(!stdfs::exists(runs_dir / "old_run"));
    CB_CHECK(stdfs::exists(runs_dir / "fresh_run"));
    CB_CHECK(stdfs::exists(runs_dir / "stray.txt"));
    // an age of zero takes the fresh one too
    CB_CHECK_EQ(runs::sweep(state.path(), "vxdboot", std::chrono::seconds(-1)).size(), 1u);
});

CB_TEST("base.runs.start_sweeps_the_killed_runs_first", [] {
    fs::ScopedTempDir state("cbruns");
    const stdfs::path old_run = state.path() / "phase3" / "runs" / "killed_run";
    fs::make_dirs(old_run);
    stdfs::last_write_time(old_run, stdfs::file_time_type::clock::now() - std::chrono::hours(48));
    const stdfs::path fresh = runs::start(state.path(), "phase3");
    CB_CHECK(!stdfs::exists(old_run));
    CB_CHECK(stdfs::exists(fresh));
});

CB_TEST("base.runs.results_folder_is_made_and_named_by_the_tool", [] {
    fs::ScopedTempDir state("cbruns");
    const stdfs::path shots = runs::results(state.path(), "phase3");
    CB_CHECK(stdfs::is_directory(shots));
    CB_CHECK(shots == state.path() / "phase3" / "shots");
    CB_CHECK_EQ(runs::results(state.path(), "phase3", "logs").filename().string(), "logs");
});

CB_TEST("base.runs.remove_reports_whether_the_folder_is_gone", [] {
    fs::ScopedTempDir state("cbruns");
    CB_CHECK(runs::remove(state.path() / "never_existed", 1));
    const stdfs::path folder = state.path() / "f";
    fs::make_dirs(folder / "sub");
    fs::write_text(folder / "sub" / "a", "a");
    CB_CHECK(runs::remove(folder));
    CB_CHECK(!stdfs::exists(folder));
});
