// base/config.hpp -- the application's configuration: settings.json, the defaults, and the CB_<KEY> overrides
// (docs/CPP_DESIGN.md 3.8). Loaded once; the modules below never read settings themselves -- they take plain
// values, and this is where those values come from (the state folder, the corpus's products, the worker policy...).
//
//     const cb::config::Config& c = cb::config::current();     // the checkout this program runs from
//     c.path("state")                                           // absolute; nullopt when not set
//     c.text("target")                                          // "win95"
//     c.products()                                              // what cb::base::Corpus is built from
//
// A setting's value is, first, the environment variable CB_<KEY> (upper case) when it is set and not empty; then
// settings.json's; then the work folder's; then the default. A path setting that is relative is taken against the
// checkout (in the work folder's file, against the work folder). settings.json is optional; one that is not a JSON
// object is a FormatError naming it.
//
// The work folder (setting "work_folder") is what CodeBreaker is used on: a folder of its own (e.g. a Windows 95
// repository) holding its originals, its media and its working files, with a codebreaker.json naming where they are
// (the same keys as settings.json: "target", "corpus", "reference", "win95_setup_iso", "win95_boot", ...). CodeBreaker
// names no target's paths itself.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "base/corpus.hpp"
#include "base/parallel.hpp"

namespace cb::config {

// One scalar setting as settings.json wrote it (lists and objects other than "corpora" are not settings).
using Value = std::variant<std::monostate, bool, std::int64_t, double, std::string>;

class Config {
public:
    // The configuration of the checkout at `checkout_root` (its settings.json, the defaults, the environment).
    static Config load(const std::filesystem::path& checkout_root);

    const std::filesystem::path& checkout_root() const { return root_; }
    // The data the program ships with (JSON tables, Java API maps, toolchain profiles, runtimes): <checkout>/config.
    std::filesystem::path config_root() const { return root_ / "config"; }

    // The setting as text: a string as it is, a number in decimal, a bool "true"/"false"; "" when not set.
    std::string text(std::string_view key) const;
    // A path setting: absolute (a relative one against the checkout); nullopt when not set.
    std::optional<std::filesystem::path> path(std::string_view key) const;
    // A number setting; nullopt when not set or not a number.
    std::optional<std::int64_t> integer(std::string_view key) const;
    std::optional<double> number(std::string_view key) const;
    // A switch setting: false when not set, or empty, 0, false, no, off, none (any case).
    bool truthy(std::string_view key) const;

    // The work folder (setting "work_folder"), absolute; nullopt when none is set.
    std::optional<std::filesystem::path> work_folder() const { return path("work_folder"); }
    // The work folder's codebreaker.json (whether or not it is there); nullopt without a work folder.
    std::optional<std::filesystem::path> work_file() const;

    // The state folder (setting "state"), and a path inside it whose folder is created.
    std::filesystem::path state() const;
    std::filesystem::path state(const std::filesystem::path& inside) const;

    // {name: folder} of every product's original binaries: "corpora", plus "corpus" under the "target" name when
    // "corpora" does not list it.
    std::map<std::string, std::filesystem::path> corpora() const;
    // What cb::base::Corpus is built from.
    cb::base::Products products() const;
    // settings "workers", "low_load", "min_free_gb" and the slot folder <state>/slots.
    cb::parallel::WorkerPolicy worker_policy() const;
    cb::base::budget::Limits budget_limits() const;

    // The main checkout this one belongs to: itself, or for a git worktree (whose .git is a file naming
    // <main>/.git/worktrees/<name>) the main one.
    std::filesystem::path main_checkout() const;

    // Whether `key` names a path setting.
    static bool is_path_setting(std::string_view key);

    // settings.json of the checkout (whether or not it is there).
    std::filesystem::path settings_file() const { return root_ / "settings.json"; }
    // Every setting there is to show, sorted: those settings.json holds, the ones with a default and the path settings
    // ("reference", which only the corpus reads, is shown when settings.json sets it).
    std::vector<std::string> keys() const;
    // settings.json's "corpora" as "name=folder, name=folder" (the folders as written); "" when it has none.
    std::string corpora_text() const;
    bool has_corpora() const { return !file_corpora_.empty() || !work_corpora_.empty(); }
    // A setting whose value in settings.json is an object other than "corpora" (e.g. "kits"), as "name=value, ..." in the
    // file's order; nullopt for any other setting.
    std::optional<std::string> object_text(std::string_view key) const;

private:
    Value value(std::string_view key) const;   // the environment, else settings.json, else the work folder's, else the default
    void load_work_folder();                   // the work folder's codebreaker.json into work_ and work_corpora_
    std::filesystem::path root_;
    std::map<std::string, Value, std::less<>> file_;                  // settings.json's scalar settings
    std::map<std::string, std::string, std::less<>> file_corpora_;    // settings.json's "corpora", as written
    std::map<std::string, std::string, std::less<>> file_objects_;    // the other object settings, as object_text shows them
    std::map<std::string, Value, std::less<>> work_;                  // the work folder's codebreaker.json (path settings made absolute)
    std::map<std::string, std::string, std::less<>> work_corpora_;    // its "corpora", absolute
};

// The checkout this program belongs to: CB_CHECKOUT_ROOT when set; else the first folder at or above the running
// executable that holds config/ and native/; else the first such folder at or above the working
// folder; else the working folder.
std::filesystem::path find_checkout();

// The configuration of find_checkout(), loaded on first use.
const Config& current();

}  // namespace cb::config
