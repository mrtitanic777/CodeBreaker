#include "cb/base/tidy_state.hpp"

#include <map>
#include <set>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

using namespace cb;
using namespace cb::base;
namespace stdfs = std::filesystem;
using Rule = tidy_state::Rule;

namespace {

void age(const stdfs::path& p, int hours) {
    stdfs::last_write_time(p, stdfs::file_time_type::clock::now() - std::chrono::hours(hours));
}

void make_file(const stdfs::path& p, std::size_t size) {
    fs::write_text(p, std::string(size, 'x'));
}

// A state/ and a Converted/ with one of everything the rules look at.
struct Fixture {
    fs::ScopedTempDir dir{"cbtidy"};
    tidy_state::Layout layout;
    Fixture() {
        layout.state = dir.path() / "state";
        layout.converted = dir.path() / "Converted";
        const stdfs::path& s = layout.state;
        make_file(s / "fingerprints.sqlite", 100);                                     // old cache
        make_file(s / "fingerprints.sqlite-wal", 10);
        make_file(s / "phase3" / "runs" / "killed" / "hd.img", 1000);                  // killed run: old
        age(s / "phase3" / "runs" / "killed", 30);
        make_file(s / "phase3" / "runs" / "going" / "hd.img", 500);                    // a run still going: fresh
        make_file(s / "phase3" / "walk_1" / "x", 7);                                   // runs of before
        age(s / "phase3" / "walk_1", 30);
        make_file(s / "phase3" / "walk_fresh" / "x", 7);
        make_file(s / "phase3" / "t_old", 3);
        age(s / "phase3" / "t_old", 30);
        make_file(s / "record16" / "rec_mem_a.bin", 20);                               // a memory dump
        age(s / "record16" / "rec_mem_a.bin", 30);
        make_file(s / "record16" / "rec_mem_a.txt", 20);                               // not a dump (suffix)
        age(s / "record16" / "rec_mem_a.txt", 30);
        make_file(s / "record16" / "walk" / "old" / "hd.img", 4000);                   // a kept disk, 8 days
        age(s / "record16" / "walk" / "old" / "hd.img", 8 * 24);
        make_file(s / "record16" / "walk" / "new" / "hd.img", 4000);
        make_file(s / "results.sqlite", 50);                                           // a store: kept
        make_file(s / "results.sqlite-journal", 5);
        make_file(s / "somebodys_notes.txt", 11);                                       // stray
        make_file(s / "scratch" / "a", 13);                                            // stray
        const stdfs::path& c = layout.converted;
        make_file(c / "oldprog" / "build" / "a.obj", 300);                             // not built for 3 weeks
        make_file(c / "oldprog" / "build" / "B.O", 200);
        make_file(c / "oldprog" / "build" / "prog.exe", 900);                          // the program stays
        make_file(c / "oldprog" / "build" / "sub" / "c.obj", 100);
        for (const char* f : {"a.obj", "B.O", "prog.exe"}) age(c / "oldprog" / "build" / f, 21 * 24);
        age(c / "oldprog" / "build" / "sub" / "c.obj", 21 * 24);
        make_file(c / "newprog" / "build" / "a.obj", 300);                             // built today
        make_file(c / "noprog" / "other" / "a.obj", 300);                              // not a build/ folder
    }
    std::map<std::string, std::uint64_t> bytes_by_name(const std::vector<tidy_state::Stale>& list, Rule rule) {
        std::map<std::string, std::uint64_t> out;
        for (const auto& s : list)
            if (s.rule == rule) out[s.path.filename().string()] = s.bytes;
        return out;
    }
};

}  // namespace

CB_TEST("base.tidy_state.stale_lists_each_rule_and_only_what_is_old", [] {
    Fixture f;
    const auto list = tidy_state::stale(f.layout);
    using Map = std::map<std::string, std::uint64_t>;
    CB_CHECK(f.bytes_by_name(list, Rule::OldCaches) == (Map{{"fingerprints.sqlite", 100}, {"fingerprints.sqlite-wal", 10}}));
    CB_CHECK(f.bytes_by_name(list, Rule::Runs) ==
             (Map{{"killed", 1000}, {"walk_1", 7}, {"t_old", 3}, {"rec_mem_a.bin", 20}}));
    CB_CHECK(f.bytes_by_name(list, Rule::KeptDisks) == (Map{{"hd.img", 4000}}));
    CB_CHECK(f.bytes_by_name(list, Rule::Builds) == (Map{{"a.obj", 300}, {"B.O", 200}, {"c.obj", 100}}));
    CB_CHECK(f.bytes_by_name(list, Rule::Stray).empty());   // stray is reported by stray(), never by stale()
    // measured from a later `now`, a fresh run is old too
    const auto later = tidy_state::stale(f.layout, tidy_state::Clock::now() + std::chrono::hours(24 * 30));
    CB_CHECK(f.bytes_by_name(later, Rule::Runs).contains("going"));
    CB_CHECK(f.bytes_by_name(later, Rule::Runs).contains("walk_fresh"));
});

CB_TEST("base.tidy_state.a_build_with_a_recent_file_is_left_alone", [] {
    Fixture f;
    // one file of the build touched today: the build is not stale, whatever its objects' ages
    make_file(f.layout.converted / "oldprog" / "build" / "log.txt", 1);
    const auto list = tidy_state::stale(f.layout);
    CB_CHECK(f.bytes_by_name(list, Rule::Builds).empty());
});

CB_TEST("base.tidy_state.stray_is_what_no_part_writes", [] {
    Fixture f;
    std::set<std::string> names;
    for (const auto& p : tidy_state::stray(f.layout)) names.insert(p.path.filename().string());
    // known folders, a store's -journal and the old caches are not stray
    CB_CHECK(names == (std::set<std::string>{"somebodys_notes.txt", "scratch"}));
});

CB_TEST("base.tidy_state.parts_are_sorted_biggest_first_and_sized_recursively", [] {
    Fixture f;
    const auto parts = tidy_state::parts(f.layout);
    CB_CHECK(!parts.empty());
    CB_CHECK_EQ(parts.front().path.filename().string(), "record16");   // 4000 + 4000 + 20 + 20
    CB_CHECK_EQ(parts.front().bytes, 8040u);
    for (std::size_t i = 1; i < parts.size(); ++i) CB_CHECK(parts[i - 1].bytes >= parts[i].bytes);
    CB_CHECK_EQ(tidy_state::size_of(f.layout.state / "scratch"), 13u);
    CB_CHECK_EQ(tidy_state::size_of(f.layout.state / "scratch" / "a"), 13u);
    CB_CHECK_EQ(tidy_state::size_of(f.layout.state / "no such thing"), 0u);
});

CB_TEST("base.tidy_state.dry_run_reports_and_removes_nothing", [] {
    Fixture f;
    testing::CapturedStd cap;
    tidy_state::Options o;
    o.dry = true;
    o.remove_stray = true;
    CB_CHECK_EQ(tidy_state::run(f.layout, o), 0);
    const std::string out = cap.out();
    CB_CHECK(out.find("would remove builds") != std::string::npos);
    CB_CHECK(out.find("would remove old caches") != std::string::npos);
    CB_CHECK(out.find("would remove stray") != std::string::npos);
    CB_CHECK(out.find("not written by CodeBreaker (kept; --stray removes them):") != std::string::npos);
    CB_CHECK(stdfs::exists(f.layout.state / "fingerprints.sqlite"));
    CB_CHECK(stdfs::exists(f.layout.state / "somebodys_notes.txt"));
    CB_CHECK(stdfs::exists(f.layout.converted / "oldprog" / "build" / "a.obj"));
    CB_CHECK(out.find("state/ now") == std::string::npos);   // a dry run does not report an after
});

CB_TEST("base.tidy_state.run_removes_the_stale_and_keeps_the_rest", [] {
    Fixture f;
    testing::CapturedStd cap;
    CB_CHECK_EQ(tidy_state::run(f.layout, {}), 0);
    const std::string out = cap.out();
    CB_CHECK(out.find("removed builds") != std::string::npos);
    CB_CHECK(out.find("Converted/*/build/ (2):") != std::string::npos);   // oldprog and newprog
    CB_CHECK(!stdfs::exists(f.layout.state / "fingerprints.sqlite"));
    CB_CHECK(!stdfs::exists(f.layout.state / "phase3" / "runs" / "killed"));
    CB_CHECK(stdfs::exists(f.layout.state / "phase3" / "runs" / "going"));
    CB_CHECK(!stdfs::exists(f.layout.state / "phase3" / "walk_1"));
    CB_CHECK(stdfs::exists(f.layout.state / "phase3" / "walk_fresh"));
    CB_CHECK(stdfs::exists(f.layout.state / "record16" / "rec_mem_a.txt"));
    CB_CHECK(!stdfs::exists(f.layout.state / "record16" / "walk" / "old" / "hd.img"));
    CB_CHECK(stdfs::exists(f.layout.state / "record16" / "walk" / "new" / "hd.img"));
    CB_CHECK(!stdfs::exists(f.layout.converted / "oldprog" / "build" / "a.obj"));
    CB_CHECK(!stdfs::exists(f.layout.converted / "oldprog" / "build" / "sub" / "c.obj"));
    CB_CHECK(stdfs::exists(f.layout.converted / "oldprog" / "build" / "prog.exe"));
    CB_CHECK(stdfs::exists(f.layout.converted / "newprog" / "build" / "a.obj"));
    CB_CHECK(stdfs::exists(f.layout.state / "results.sqlite"));
    // stray things are listed and kept
    CB_CHECK(stdfs::exists(f.layout.state / "somebodys_notes.txt"));
    CB_CHECK(out.find("kept; --stray removes them") != std::string::npos);
    CB_CHECK(out.find("state/ now ") != std::string::npos);
});

CB_TEST("base.tidy_state.stray_and_keep_builds_options", [] {
    Fixture f;
    tidy_state::Options o;
    o.remove_stray = true;
    o.keep_builds = true;
    testing::CapturedStd cap;
    tidy_state::run(f.layout, o);
    CB_CHECK(!stdfs::exists(f.layout.state / "somebodys_notes.txt"));
    CB_CHECK(!stdfs::exists(f.layout.state / "scratch"));
    CB_CHECK(stdfs::exists(f.layout.converted / "oldprog" / "build" / "a.obj"));   // --keep-builds
    CB_CHECK(cap.out().find("removed stray") != std::string::npos);
    CB_CHECK(cap.out().find("removed builds") == std::string::npos);
});

CB_TEST("base.tidy_state.the_cache_is_pruned_through_its_hooks", [] {
    Fixture f;
    tidy_state::CacheHooks hooks;
    hooks.prune = [] { return std::vector<tidy_state::CachePruned>{{"objects", {"g1", "g2"}}}; };
    hooks.max_bytes = [] { return std::uint64_t(3) << 30; };
    testing::CapturedStd cap;
    tidy_state::run(f.layout, {}, hooks);
    const std::string out = cap.out();
    CB_CHECK(out.find("removed cache objects: old generation(s) g1, g2\n") != std::string::npos);
    CB_CHECK(out.find("(the cache's cap: 3.00 GB, setting cache_max_gb)") != std::string::npos);
});

CB_TEST("base.tidy_state.an_empty_state_folder_is_made_and_reported", [] {
    fs::ScopedTempDir dir("cbtidy");
    tidy_state::Layout layout{dir.path() / "state", dir.path() / "Converted"};
    testing::CapturedStd cap;
    CB_CHECK_EQ(tidy_state::run(layout, {}), 0);
    CB_CHECK(stdfs::is_directory(layout.state));
    CB_CHECK(cap.out().find("0.0 MB") != std::string::npos);
});
