// Tests of cb/base/config: settings.json, the defaults, the CB_<KEY> overrides, and what is built from them.
#include "cb/base/config.hpp"

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace stdfs = std::filesystem;
using cb::config::Config;

namespace {

// A checkout folder with the given settings.json text (none when empty).
struct Checkout {
    cb::fs::ScopedTempDir dir{"cbconfig"};
    explicit Checkout(std::string_view settings = {}) {
        if (!settings.empty()) cb::fs::write_text(dir.path() / "settings.json", settings);
    }
    Config load() const { return Config::load(dir.path()); }
};

}  // namespace

CB_TEST("base.config.defaults_without_a_settings_file", [] {
    const cb::fs::ScopedEnv s("CB_STATE", std::nullopt), t("CB_TARGET", std::nullopt), c("CB_CORPUS", std::nullopt);
    const Checkout co;
    const Config cfg = co.load();
    CB_CHECK_EQ(cfg.text("target"), "win95");
    CB_CHECK_EQ(cfg.text("struct_recovery"), "all");
    CB_CHECK_EQ(cfg.text("not_a_setting"), "");
    CB_CHECK(!cfg.path("openwatcom").has_value());   // no default: not set
    CB_CHECK(cfg.state() == (co.dir.path() / "state").lexically_normal());
    CB_CHECK(cfg.path("corpus") == (co.dir.path() / "../Windows95/design/corpus/files").lexically_normal());
});

CB_TEST("base.config.settings_file_values_and_kinds", [] {
    const cb::fs::ScopedEnv w("CB_WORKERS", std::nullopt), l("CB_LOW_LOAD", std::nullopt), m("CB_MIN_FREE_GB", std::nullopt),
        st("CB_STATE", std::nullopt), o("CB_OPENWATCOM", std::nullopt);
    const std::string watcom = cb::testing::rooted("WATCOM");
    const Checkout co(R"({"workers": 6, "low_load": "yes", "min_free_gb": 2.5, "state": "mystate",
                          "openwatcom": ")" + watcom + R"(", "threads": "", "a_list": [1, 2], "struct_recovery": "32"})");
    const Config cfg = co.load();
    CB_CHECK_EQ(cfg.integer("workers").value_or(-1), 6);
    CB_CHECK(cfg.truthy("low_load"));
    CB_CHECK(cfg.number("min_free_gb").value_or(0) == 2.5);
    CB_CHECK(cfg.state() == (co.dir.path() / "mystate").lexically_normal());   // a relative path: against the checkout
    CB_CHECK(cfg.path("openwatcom") == stdfs::path(watcom).lexically_normal());
    CB_CHECK_EQ(cfg.text("threads"), "");     // an empty string is "not set"
    CB_CHECK(!cfg.truthy("threads"));
    CB_CHECK_EQ(cfg.text("a_list"), "");      // not a scalar: not a setting
    CB_CHECK_EQ(cfg.text("struct_recovery"), "32");
    CB_CHECK_EQ(cfg.text("workers"), "6");
    CB_CHECK_EQ(cfg.text("low_load"), "yes");

    const cb::parallel::WorkerPolicy p = cfg.worker_policy();
    CB_CHECK_EQ(p.workers, 6);
    CB_CHECK(p.low_load);
    const cb::base::budget::Limits b = cfg.budget_limits();
    CB_CHECK(b.min_free_gb == 2.5);
    CB_CHECK(b.slot_dir == cfg.state() / "slots");
});

CB_TEST("base.config.the_environment_wins", [] {
    const Checkout co(R"({"target": "win98", "workers": 3})");
    const cb::fs::ScopedEnv t("CB_TARGET", "win1.0"), w("CB_WORKERS", "9"), e("CB_TARGET_EMPTY", std::nullopt);
    const Config cfg = co.load();
    CB_CHECK_EQ(cfg.text("target"), "win1.0");
    CB_CHECK_EQ(cfg.integer("workers").value_or(0), 9);
    {
        const cb::fs::ScopedEnv empty("CB_TARGET", "");   // an empty variable does not override
        CB_CHECK_EQ(cfg.text("target"), "win98");
    }
});

CB_TEST("base.config.truthy_words", [] {
    for (const char* no : {"", "0", "false", "No", "OFF", "none", " false "}) {
        const cb::fs::ScopedEnv e("CB_LOW_LOAD", std::string(no));
        const Checkout co(std::string(R"({"low_load": ")") + no + "\"}");
        CB_CHECK(!co.load().truthy("low_load"));
    }
    const cb::fs::ScopedEnv e("CB_LOW_LOAD", std::nullopt);
    CB_CHECK(Checkout(R"({"low_load": true})").load().truthy("low_load"));
    CB_CHECK(Checkout(R"({"low_load": 1})").load().truthy("low_load"));
    CB_CHECK(!Checkout(R"({"low_load": 0})").load().truthy("low_load"));
});

CB_TEST("base.config.only_path_settings_are_joined", [] {
    const cb::fs::ScopedEnv t("CB_TARGET", std::nullopt);
    const Checkout co(R"({"target": "win95"})");
    // "target" is not a path setting: a relative-looking value stays as it is
    CB_CHECK(co.load().path("target") == stdfs::path("win95"));
});

CB_TEST("base.config.corpora_and_products", [] {
    const cb::fs::ScopedEnv t("CB_TARGET", std::nullopt), c("CB_CORPUS", std::nullopt), r("CB_REFERENCE", std::nullopt),
        st("CB_STATE", std::nullopt);
    const std::string old = cb::testing::rooted("old");
    const Checkout co(R"({"corpus": "files95", "corpora": {"win98": "files98", "win1.0": ")" + old + R"(", "bad": 3},
                          "reference": "ref"})");
    const Config cfg = co.load();
    const auto corpora = cfg.corpora();
    CB_CHECK_EQ(corpora.size(), 3u);   // the target's "corpus" joins the listed two; a folder that is not text does not
    CB_CHECK(corpora.at("win95") == (co.dir.path() / "files95").lexically_normal());
    CB_CHECK(corpora.at("win98") == (co.dir.path() / "files98").lexically_normal());
    CB_CHECK(corpora.at("win1.0") == stdfs::path(old).lexically_normal());

    const cb::base::Products p = cfg.products();
    CB_CHECK_EQ(p.target, "win95");
    CB_CHECK(p.target_folder == (co.dir.path() / "files95").lexically_normal());
    CB_CHECK_EQ(p.listed.size(), 2u);
    CB_CHECK(p.checkout_root == co.dir.path());
    CB_CHECK(p.reference == (co.dir.path() / "ref").lexically_normal());
});

CB_TEST("base.config.a_listed_target_is_not_replaced_by_corpus", [] {
    const cb::fs::ScopedEnv t("CB_TARGET", std::nullopt), c("CB_CORPUS", std::nullopt);
    const Checkout co(R"({"corpus": "files95", "corpora": {"win95": "elsewhere"}})");
    CB_CHECK(co.load().corpora().at("win95") == (co.dir.path() / "elsewhere").lexically_normal());
});

CB_TEST("base.config.a_damaged_settings_file_is_a_format_error", [] {
    CB_CHECK_THROWS_AS(cb::FormatError, (void)Checkout("{ not json").load());
    CB_CHECK_THROWS_AS(cb::FormatError, (void)Checkout("[1, 2]").load());
});

CB_TEST("base.config.state_inside_makes_the_folder", [] {
    const cb::fs::ScopedEnv st("CB_STATE", std::nullopt);
    const Checkout co;
    const stdfs::path p = co.load().state("cache/kind/file.bin");
    CB_CHECK(stdfs::is_directory(p.parent_path()));
    CB_CHECK(!stdfs::exists(p));
});

CB_TEST("base.config.main_checkout_of_a_worktree", [] {
    const Checkout main_co, wt;
    CB_CHECK(main_co.load().main_checkout() == main_co.dir.path());   // no .git file: itself
    const stdfs::path gitdir = main_co.dir.path() / ".git" / "worktrees" / "agent-x";
    cb::fs::make_dirs(gitdir);
    cb::fs::write_text(wt.dir.path() / ".git", "gitdir: " + cb::fs::to_text(gitdir) + "\n");
    CB_CHECK(wt.load().main_checkout() == main_co.dir.path().lexically_normal());
    cb::fs::write_text(wt.dir.path() / ".git", "gitdir: C:/somewhere/else\n");   // not a worktree of a main checkout
    CB_CHECK(wt.load().main_checkout() == wt.dir.path());
});

CB_TEST("base.config.find_checkout_honours_the_environment", [] {
    const Checkout co;
    const cb::fs::ScopedEnv e("CB_CHECKOUT_ROOT", cb::fs::to_text(co.dir.path()));
    CB_CHECK(cb::config::find_checkout() == co.dir.path());
});
