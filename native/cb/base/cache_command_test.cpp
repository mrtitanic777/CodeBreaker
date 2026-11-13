// Tests of cb/base/cache_command: `codebreaker cache`, against what the Python command printed for an empty state folder (cache_max_gb 1.5),
// and on a cache with something in it.
#include "cb/base/cache_command.hpp"

#include <memory>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace {

namespace stdfs = std::filesystem;
namespace cachecmd = cb::base::cache;

cachecmd::CommandEnvironment environment_in(const stdfs::path& dir, std::uint64_t max_bytes) {
    cachecmd::CommandEnvironment env;
    env.cache.state_dir = dir / "cache";
    env.cache.shared_dir = dir / "build_cache";
    env.cache.max_bytes = max_bytes;
    return env;
}

int run_command(const cachecmd::CommandEnvironment& env, std::vector<std::string> args, std::string& out) {
    cb::testing::CapturedStd cap;
    cb::cli::Cli cli("codebreaker", {cachecmd::command(env)});
    args.insert(args.begin(), "cache");
    const int code = cli.run(args);
    out = cap.out();
    return code;
}

}  // namespace

CB_TEST("base.cache_command.an_empty_cache_reads_as_the_python_printed_it", [] {
    cb::fs::ScopedTempDir dir("cbcachecmd");
    const cachecmd::CommandEnvironment env = environment_in(dir.path(), static_cast<std::uint64_t>(1.5 * static_cast<double>(cachecmd::kGiB)));
    // (python -m codebreaker cache [--stats | --prune | --clear] with CB_STATE an empty folder and CB_CACHE_MAX_GB=1.5)
    const std::string listing =
        "kind                   size    results  generations (* = of now)\n"
        "all                  0.0 MB   (cap 1.50 GB: setting cache_max_gb)\n";
    std::string out;
    CB_CHECK_EQ(run_command(env, {}, out), 0);
    CB_CHECK_EQ(out, listing);
    CB_CHECK_EQ(run_command(env, {"--stats"}, out), 0);
    CB_CHECK_EQ(out, listing);
    CB_CHECK_EQ(run_command(env, {"--prune"}, out), 0);
    CB_CHECK_EQ(out, listing);
    CB_CHECK_EQ(run_command(env, {"--clear"}, out), 0);
    CB_CHECK_EQ(out, "cache cleared: 0.0 MB freed\n");
});

CB_TEST("base.cache_command.lists_prunes_and_clears", [] {
    cb::fs::ScopedTempDir dir("cbcachecmd");
    cachecmd::CommandEnvironment env = environment_in(dir.path(), 2 * cachecmd::kGiB);
    env.declare_kinds = [](cachecmd::Cache& c) {
        c.declare_kind("checks", {.generation = "rev 2"});
    };
    {
        cachecmd::Cache now(env.cache);
        now.declare_kind("checks", {.generation = "rev 2"});
        now.put("checks", "b", cb::Bytes(1000, 2));
        now.put("checks", "c", cb::Bytes(1000, 3));
    }
    // a generation of older code, as an older run left it (using a kind drops the others, so it is put there by hand)
    for (const auto& entry : stdfs::directory_iterator(env.cache.state_dir / "checks")) {
        if (entry.path().extension() == ".cbc") {
            stdfs::copy_file(entry.path(), entry.path().parent_path() / "g0old.cbc");
            break;
        }
    }
    std::string out;
    CB_CHECK_EQ(run_command(env, {"--stats"}, out), 0);
    CB_CHECK(out.starts_with("kind                   size    results  generations (* = of now)\n"));
    CB_CHECK(out.find("checks") != std::string::npos);
    CB_CHECK(out.find('*') != std::string::npos);                      // the generation of now is marked
    CB_CHECK(out.ends_with("(cap 2.00 GB: setting cache_max_gb)\n"));

    CB_CHECK_EQ(run_command(env, {"--prune"}, out), 0);                // the old generation goes, and then the listing
    CB_CHECK(out.starts_with("  checks           dropped old generation(s) g"));
    CB_CHECK(out.find("\nkind ") != std::string::npos);

    CB_CHECK_EQ(run_command(env, {"--clear"}, out), 0);
    CB_CHECK(out.starts_with("cache cleared: "));
    CB_CHECK_EQ(run_command(env, {"--stats"}, out), 0);
    CB_CHECK(out.find("checks") == std::string::npos);
});

CB_TEST("base.cache_command.a_misused_command_line_is_a_usage_error", [] {
    cb::fs::ScopedTempDir dir("cbcachecmd");
    const cachecmd::CommandEnvironment env = environment_in(dir.path(), cachecmd::kGiB);
    cb::testing::CapturedStd cap;
    cb::cli::Cli cli("codebreaker", {cachecmd::command(env)});
    CB_CHECK_EQ(cli.run({"cache", "--bogus"}), cb::kExitUsage);       // (the Python printed its usage and exited 2)
});

CB_TEST("base.cache_command.settings_give_the_folders_and_caps", [] {
    cb::fs::ScopedTempDir dir("cbcachecmd");
    cb::fs::write_text(dir.path() / "settings.json", R"({"state": "mystate", "cache_max_gb": 3, "build_cache_max_gb": 0.5})");
    const cb::fs::ScopedEnv a("CB_STATE", std::nullopt), b("CB_CACHE_MAX_GB", std::nullopt), c("CB_BUILD_CACHE", std::nullopt),
        d("CB_BUILD_CACHE_MAX_GB", std::nullopt), e("CB_NO_CACHE", std::nullopt);
    const cachecmd::Config config = cachecmd::settings_of(cb::config::Config::load(dir.path()));
    CB_CHECK(config.state_dir == (dir.path() / "mystate" / "cache").lexically_normal());
    CB_CHECK(config.shared_dir == (dir.path() / "state" / "build_cache").lexically_normal() ||
             config.shared_dir == dir.path() / "state" / "build_cache");
    CB_CHECK_EQ(config.max_bytes, 3 * cachecmd::kGiB);
    CB_CHECK_EQ(config.shared_max_bytes, cachecmd::kGiB / 2);
    CB_CHECK(!config.disabled);
});
