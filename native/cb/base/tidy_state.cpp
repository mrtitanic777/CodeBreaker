#include "cb/base/tidy_state.hpp"

#include <algorithm>
#include <format>
#include <map>

#include "cb/base/runs.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/text.hpp"

namespace cb::base::tidy_state {

namespace stdfs = std::filesystem;

namespace {

constexpr std::chrono::hours kDay{24};
constexpr std::chrono::hours kKeptDiskAge = 7 * kDay;
constexpr std::chrono::hours kBuildAge = 14 * kDay;
constexpr std::size_t kPartsShown = 25;

// Run tools whose runs/ folders hold the copies of killed boot runs.
constexpr std::string_view kRunTools[] = {"phase3", "vxdboot", "record16"};

// The fingerprints' own file of before they were in the cache.
constexpr std::string_view kOldCaches[] = {"fingerprints.sqlite"};

// Folders and files of the runs of before run folders were: {folder under state/, name prefix, name suffix}.
struct OldRunPattern {
    std::string_view folder, prefix, suffix;
};
constexpr OldRunPattern kOldRunPatterns[] = {
    {"phase3", "walk_", ""},         {"phase3", "setup16_", ""},         {"phase3", "t_", ""},
    {"vxdboot", "run_", ""},         {"record16", "rec_mem_", ".bin"},   {"record16", "rec_hit_", ".bin"},
};

std::string gb_text(std::uint64_t n) {
    if (n >= (1ull << 30)) return std::format("{:.2f} GB", static_cast<double>(n) / static_cast<double>(1ull << 30));
    return std::format("{:.1f} MB", static_cast<double>(n) / static_cast<double>(1ull << 20));
}

std::vector<stdfs::path> children(const stdfs::path& dir) {
    std::vector<stdfs::path> out;
    std::error_code ec;
    for (const auto& entry : stdfs::directory_iterator(dir, ec)) out.push_back(entry.path());
    std::sort(out.begin(), out.end());
    return out;
}

// The children of `dir` named <prefix>*<suffix> (like a glob; a name starting with '.' is not matched by '*').
std::vector<stdfs::path> glob_children(const stdfs::path& dir, std::string_view prefix, std::string_view suffix = "") {
    std::vector<stdfs::path> out;
    for (const stdfs::path& p : children(dir)) {
        const std::string name = p.filename().string();
        if (name.size() < prefix.size() + suffix.size()) continue;
        if (!name.starts_with(prefix) || !name.ends_with(suffix)) continue;
        if (prefix.empty() && name.starts_with('.')) continue;
        out.push_back(p);
    }
    return out;
}

bool older_than(const stdfs::path& p, Clock::time_point now, std::chrono::hours age) {
    std::error_code ec;
    const auto written = stdfs::last_write_time(p, ec);
    if (ec) return false;
    return now - std::chrono::clock_cast<Clock>(written) > age;
}

bool is_known(std::string_view name, const std::set<std::string>& names) {
    for (std::string_view suffix : {"-wal", "-shm", "-journal"})
        if (name.ends_with(suffix)) {
            name.remove_suffix(suffix.size());
            break;
        }
    if (names.contains(std::string(name))) return true;
    return std::find(std::begin(kOldCaches), std::end(kOldCaches), name) != std::end(kOldCaches);
}

bool delete_path(const stdfs::path& p) {
    try {
        (void)fs::remove_tree(p);
    } catch (const IoError&) {
        // left in place: the report says it was not removed
    }
    std::error_code ec;
    return !stdfs::exists(p, ec);
}

}  // namespace

std::string_view name(Rule rule) {
    switch (rule) {
        case Rule::OldCaches: return "old caches";
        case Rule::Runs: return "runs";
        case Rule::KeptDisks: return "kept disks";
        case Rule::Builds: return "builds";
        case Rule::Stray: return "stray";
    }
    return "";
}

const std::set<std::string>& known_names() {
    static const std::set<std::string> names = {
        "analyze", "assembly_probes.json", "bootfloppy", "build", "build_cache", "cache", "coverage_corpus.json", "cxx",
        "db", "dll", "dos_source", "driver_build", "driver_judge", "driver_kit", "driver_source", "exact",
        "functions.sqlite", "golden", "goto_report", "idiomatic", "invert", "java", "l2c", "le_source", "match",
        "match16", "mcp", "mini", "module_names16.json", "msc6", "msvc152", "native-dos-mem", "ne_source", "oss_bench",
        "pe_source", "period", "phase3", "products", "reach", "record16", "repair", "results.sqlite", "service",
        "sleigh_verify.txt", "sleigh_verify_ppc.txt", "slots", "source", "struct_report", "teeth", "translate",
        "ui_jobs", "unpacked", "vxd", "vxd_ow", "vxdboot", "win95", "work", "xenonrecomp", "xna",
    };
    return names;
}

std::uint64_t size_of(const stdfs::path& p) {
    std::error_code ec;
    if (stdfs::is_regular_file(p, ec)) {
        const auto n = stdfs::file_size(p, ec);
        return ec ? 0 : n;
    }
    std::uint64_t total = 0;
    stdfs::recursive_directory_iterator it(p, stdfs::directory_options::skip_permission_denied, ec);
    for (const stdfs::recursive_directory_iterator end; !ec && it != end; it.increment(ec)) {
        std::error_code entry_ec;
        if (!it->is_regular_file(entry_ec)) continue;
        const auto n = it->file_size(entry_ec);
        if (!entry_ec) total += n;
    }
    return total;
}

std::vector<stdfs::path> build_folders(const Layout& layout) {
    std::vector<stdfs::path> out;
    for (const stdfs::path& program : children(layout.converted)) {
        const stdfs::path build = program / "build";
        std::error_code ec;
        if (stdfs::is_directory(build, ec)) out.push_back(build);
    }
    return out;
}

std::vector<Stale> stale(const Layout& layout, Clock::time_point now) {
    std::vector<Stale> out;
    const auto add = [&](const stdfs::path& p, Rule rule) { out.push_back({p, size_of(p), rule}); };
    std::error_code ec;

    for (std::string_view name : kOldCaches)
        for (std::string_view suffix : {"", "-wal", "-shm"}) {
            const stdfs::path p = layout.state / (std::string(name) + std::string(suffix));
            if (stdfs::exists(p, ec)) add(p, Rule::OldCaches);
        }
    for (std::string_view tool : kRunTools)
        for (const stdfs::path& p : children(layout.state / std::string(tool) / "runs"))
            if (older_than(p, now, std::chrono::duration_cast<std::chrono::hours>(runs::kKilledRunAge))) add(p, Rule::Runs);
    for (const OldRunPattern& pattern : kOldRunPatterns)
        for (const stdfs::path& p : glob_children(layout.state / std::string(pattern.folder), pattern.prefix, pattern.suffix))
            if (older_than(p, now, kDay)) add(p, Rule::Runs);
    for (const stdfs::path& walk : children(layout.state / "record16" / "walk")) {
        const stdfs::path disk = walk / "hd.img";
        if (stdfs::exists(disk, ec) && older_than(disk, now, kKeptDiskAge)) add(disk, Rule::KeptDisks);
    }
    for (const stdfs::path& build : build_folders(layout)) {
        std::vector<stdfs::path> objects;
        Clock::time_point latest{};
        bool any = false;
        stdfs::recursive_directory_iterator it(build, stdfs::directory_options::skip_permission_denied, ec);
        for (const stdfs::recursive_directory_iterator end; !ec && it != end; it.increment(ec)) {
            std::error_code entry_ec;
            if (!it->is_regular_file(entry_ec)) continue;
            const auto written = it->last_write_time(entry_ec);
            if (entry_ec) continue;
            const auto at = std::chrono::clock_cast<Clock>(written);
            if (!any || at > latest) latest = at;
            any = true;
            const std::string lower = text::lower(it->path().filename().string());
            if (lower.ends_with(".obj") || lower.ends_with(".o")) objects.push_back(it->path());
        }
        if (!objects.empty() && now - latest > kBuildAge)
            for (const stdfs::path& p : objects) add(p, Rule::Builds);
    }
    return out;
}

std::vector<Part> parts(const Layout& layout) {
    std::vector<Part> out;
    for (const stdfs::path& p : children(layout.state)) out.push_back({p, size_of(p)});
    std::stable_sort(out.begin(), out.end(), [](const Part& a, const Part& b) { return a.bytes > b.bytes; });
    return out;
}

std::vector<Part> stray(const Layout& layout) {
    std::vector<Part> out;
    for (const stdfs::path& p : children(layout.state))
        if (!is_known(p.filename().string(), known_names())) out.push_back({p, size_of(p)});
    return out;
}

int run(const Layout& layout, const Options& options, const CacheHooks& cache) {
    fs::make_dirs(layout.state);
    const std::vector<Part> before = parts(layout);
    std::uint64_t total = 0;
    for (const Part& p : before) total += p.bytes;
    cb::out("state/ ({}): {}\n", fs::to_text(layout.state), gb_text(total));
    for (std::size_t i = 0; i < before.size() && i < kPartsShown; ++i)
        cb::out("  {:<28} {:>10}\n", before[i].path.filename().string(), gb_text(before[i].bytes));
    const std::vector<stdfs::path> builds = build_folders(layout);
    if (!builds.empty()) {
        std::uint64_t build_bytes = 0;
        for (const stdfs::path& b : builds) build_bytes += size_of(b);
        cb::out("Converted/*/build/ ({}): {}\n", builds.size(), gb_text(build_bytes));
    }
    std::vector<Stale> gone;
    for (Stale& s : stale(layout))
        if (!(options.keep_builds && s.rule == Rule::Builds)) gone.push_back(std::move(s));
    const std::vector<Part> strays = stray(layout);
    if (!strays.empty()) {
        cb::out("not written by CodeBreaker ({}):\n",
                options.remove_stray && !options.dry ? "removed" : "kept; --stray removes them");
        for (const Part& p : strays) cb::out("  {:<28} {:>10}\n", p.path.filename().string(), gb_text(p.bytes));
        if (options.remove_stray)
            for (const Part& p : strays) gone.push_back({p.path, p.bytes, Rule::Stray});
    }
    std::map<std::string, std::pair<std::size_t, std::uint64_t>> by_rule;   // sorted by the rule's name, as printed
    for (const Stale& s : gone) {
        if (!options.dry && !delete_path(s.path)) continue;
        auto& [count, bytes] = by_rule[std::string(name(s.rule))];
        ++count;
        bytes += s.bytes;
    }
    for (const auto& [rule_name, tally] : by_rule)
        cb::out("{} {:<12} {:>5} item(s)  {:>10}\n", options.dry ? "would remove" : "removed", rule_name, tally.first,
                gb_text(tally.second));
    if (!options.dry) {
        if (cache.prune)
            for (const CachePruned& pruned : cache.prune())
                cb::out("removed cache {}: old generation(s) {}\n", pruned.kind, text::join(pruned.generations, ", "));
        std::uint64_t after = 0;
        for (const Part& p : parts(layout)) after += p.bytes;
        cb::out("state/ now {}", gb_text(after));
        if (cache.max_bytes) cb::out(" (the cache's cap: {}, setting cache_max_gb)", gb_text(cache.max_bytes()));
        cb::out("\n");
    }
    return 0;
}

}  // namespace cb::base::tidy_state
