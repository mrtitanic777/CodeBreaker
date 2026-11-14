// Tests of cb/base/tidy_state_command: `codebreaker tidy-state` over a state folder with stale parts and a cache the earlier implementation
// (and ours) filled.
#include "cb/base/tidy_state_command.hpp"

#include "cb/base/sqlite.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace {

namespace stdfs = std::filesystem;
namespace tidy = cb::base::tidy_state;

void age_of(const stdfs::path& p, int hours) {
    stdfs::last_write_time(p, stdfs::file_time_type::clock::now() - std::chrono::hours(hours));
}

// state/cache/<kind>/<generation>.sqlite as the earlier implementation writes it.
void legacy_cache_file(const stdfs::path& file, int rows) {
    using cb::base::sqlite::Value;
    auto db = cb::base::sqlite::Database::open(file);
    db.exec("PRAGMA auto_vacuum=INCREMENTAL");
    db.query("PRAGMA journal_mode=WAL");
    db.exec("CREATE TABLE IF NOT EXISTS entry(k TEXT PRIMARY KEY, v BLOB, used INTEGER)");
    for (int i = 0; i < rows; ++i)
        db.exec("INSERT INTO entry(k, v, used) VALUES(?,?,?)",
                {Value("k" + std::to_string(i)), Value(cb::Bytes(100, 7)), Value(std::int64_t{1000 + i})});
}

struct Setup {
    cb::fs::ScopedTempDir dir{"cbtidycmd"};
    tidy::CommandEnvironment env;
    Setup() {
        env.layout.state = dir.path() / "state";
        env.layout.converted = dir.path() / "Converted";
        env.cache.cache.state_dir = env.layout.state / "cache";
        env.cache.cache.shared_dir = dir.path() / "build_cache";
        env.cache.cache.max_bytes = cb::base::cache::kGiB;
        cb::fs::write_text(env.layout.state / "fingerprints.sqlite", std::string(2000, 'x'));       // an old cache
        cb::fs::write_text(env.layout.state / "scratch.txt", "somebody's notes");                    // stray
        cb::fs::write_text(env.layout.state / "phase3" / "runs" / "killed" / "hd.img", std::string(900, 'x'));
        age_of(env.layout.state / "phase3" / "runs" / "killed", 48);
        legacy_cache_file(env.layout.state / "cache" / "decompiled" / "gaaaa.sqlite", 3);
        legacy_cache_file(env.layout.state / "cache" / "decompiled" / "gbbbb.sqlite", 3);
        age_of(env.layout.state / "cache" / "decompiled" / "gaaaa.sqlite", 300);
    }
    int run(std::vector<std::string> args, std::string& out) {
        cb::testing::CapturedStd cap;
        cb::cli::Cli cli("codebreaker", {tidy::command(env)});
        args.insert(args.begin(), "tidy-state");
        const int code = cli.run(args);
        out = cap.out();
        return code;
    }
};

}  // namespace

CB_TEST("base.tidy_state_command.dry_reports_and_removes_nothing", [] {
    Setup s;
    std::string out;
    CB_CHECK_EQ(s.run({"--dry"}, out), 0);
    CB_CHECK(out.starts_with("state/ ("));
    CB_CHECK(out.find("would remove old caches") != std::string::npos);
    CB_CHECK(out.find("would remove runs") != std::string::npos);
    CB_CHECK(out.find("scratch.txt") != std::string::npos);                           // listed, kept
    CB_CHECK(out.find("kept; --stray removes them") != std::string::npos);
    CB_CHECK(out.find("state/ now") == std::string::npos);                            // (a dry run does not look again)
    CB_CHECK(stdfs::exists(s.env.layout.state / "fingerprints.sqlite"));
    CB_CHECK(stdfs::exists(s.env.layout.state / "phase3" / "runs" / "killed"));
    CB_CHECK(stdfs::exists(s.env.layout.state / "cache" / "decompiled" / "gaaaa.sqlite"));
});

CB_TEST("base.tidy_state_command.removes_the_stale_and_prunes_the_cache", [] {
    Setup s;
    std::string out;
    CB_CHECK_EQ(s.run({}, out), 0);
    CB_CHECK(out.find("removed old caches") != std::string::npos);
    CB_CHECK(out.find("removed runs") != std::string::npos);
    CB_CHECK(out.find("removed cache decompiled: old generation(s) gaaaa.sqlite") != std::string::npos);
    CB_CHECK(out.find("(the cache's cap: 1.00 GB, setting cache_max_gb)") != std::string::npos);
    CB_CHECK(!stdfs::exists(s.env.layout.state / "fingerprints.sqlite"));
    CB_CHECK(!stdfs::exists(s.env.layout.state / "phase3" / "runs" / "killed"));
    CB_CHECK(!stdfs::exists(s.env.layout.state / "cache" / "decompiled" / "gaaaa.sqlite"));
    CB_CHECK(stdfs::exists(s.env.layout.state / "cache" / "decompiled" / "gbbbb.sqlite"));   // the earlier implementation's newest stays, intact
    CB_CHECK(stdfs::exists(s.env.layout.state / "scratch.txt"));                              // stray stays without --stray
});

CB_TEST("base.tidy_state_command.stray_is_removed_only_when_asked", [] {
    Setup s;
    std::string out;
    CB_CHECK_EQ(s.run({"--stray"}, out), 0);
    CB_CHECK(out.find("removed stray") != std::string::npos);
    CB_CHECK(!stdfs::exists(s.env.layout.state / "scratch.txt"));
});

CB_TEST("base.tidy_state_command.the_environment_comes_from_the_settings", [] {
    cb::fs::ScopedTempDir dir("cbtidycmd");
    cb::fs::write_text(dir.path() / "settings.json", R"({"state": "mystate", "converted": "elsewhere", "cache_max_gb": 3})");
    const cb::fs::ScopedEnv a("CB_STATE", std::nullopt), b("CB_CONVERTED", std::nullopt), c("CB_CACHE_MAX_GB", std::nullopt);
    const tidy::CommandEnvironment env = tidy::environment_of(cb::config::Config::load(dir.path()));
    CB_CHECK(env.layout.state == (dir.path() / "mystate").lexically_normal() || env.layout.state == dir.path() / "mystate");
    CB_CHECK(env.layout.converted == (dir.path() / "elsewhere").lexically_normal() || env.layout.converted == dir.path() / "elsewhere");
    CB_CHECK_EQ(env.cache.cache.max_bytes, 3 * cb::base::cache::kGiB);
    CB_CHECK(env.cache.cache.state_dir.filename() == "cache");
});
