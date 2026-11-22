#include "verify/sweep.hpp"

#include <chrono>
#include <format>
#include <unordered_map>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/text.hpp"

namespace cb::verify::sweep {

namespace {

std::string totals_text(const std::map<Class, std::int64_t>& totals) {
    std::string parts;
    for (const Class c : kClassScored) {
        const auto it = totals.find(c);
        if (it == totals.end() || it->second == 0)
            continue;
        if (!parts.empty())
            parts += ", ";
        parts += std::format("{} {}", title(c), it->second);
    }
    return parts;
}

std::string file_name_lower(const std::filesystem::path& p) { return text::lower(p.filename().generic_string()); }

void say(const std::function<void(std::string_view)>& to, const std::string& line) {
    if (to)
        to(line);
}

}  // namespace

std::vector<cli::Param> params() {
    return {
        cli::Option{"--only", "", "these modules: a comma-separated list, or @file with one name per line", cli::Arg::Value, "NAMES"},
        cli::Option{"--16", "", "the 16-bit modules"},
        cli::Option{"--drivers", "", "the drivers"},
        cli::Option{"--dos", "", "real-mode programs"},
        cli::Option{"--all", "", "every kind of module"},
        cli::Option{"--status", "", "what has been measured; no work"},
        cli::Option{"--again", "", "measure modules again even if already done"},
    };
}

std::set<std::string> wanted_names(std::string_view only) {
    std::set<std::string> out;
    const auto add = [&](std::string_view name) {
        name = text::trim(name);
        if (!name.empty())
            out.insert(text::lower(name));
    };
    if (only.starts_with('@')) {
        const std::string listing = fs::read_text(std::string(only.substr(1)));
        for (const std::string_view line : text::lines(listing))
            add(line);
    } else {
        for (const std::string_view name : text::split(only, ','))
            add(name);
    }
    return out;
}

Options options_of(const cli::Args& args) {
    Options o;
    // --all: every kind; else the first of --16, --drivers, --dos that is given; else the 32-bit modules.
    if (args.flag("all"))
        o.kinds = {"pe", "ne", "le", "dos"};
    else if (args.flag("16"))
        o.kinds = {"ne"};
    else if (args.flag("drivers"))
        o.kinds = {"le"};
    else if (args.flag("dos"))
        o.kinds = {"dos"};
    if (const std::optional<std::string> only = args.value("only"))
        o.only = wanted_names(*only);
    o.status_only = args.flag("status");
    o.again = args.flag("again");
    return o;
}

std::vector<std::pair<std::filesystem::path, std::string>> status(const Services& services, const std::vector<std::filesystem::path>& paths) {
    const reconstruct::Reconstructor& rec = services.reconstruct;
    const auto digests = rec.digests();
    std::unordered_map<std::string, std::string> stored;   // module hash -> the digest it has results under; lookup only
    for (const auto& [kind, digest] : digests)
        if (digest)
            for (const engine::ModuleRow& m : services.store.modules(*digest))
                stored[m.sha] = *digest;
    std::vector<std::pair<std::filesystem::path, std::string>> done;
    std::map<Class, std::int64_t> totals;
    for (const std::filesystem::path& path : paths) {
        const std::string sha = rec.services().content_sha ? rec.services().content_sha(path).value_or("") : std::string();
        const auto has = stored.find(sha);
        const std::optional<std::string> kind = rec.kind_name(path);
        const auto current = kind ? digests.find(*kind) : digests.end();
        if (has == stored.end() || current == digests.end() || !current->second || *current->second != has->second)
            continue;
        done.emplace_back(path, sha);
        for (const auto& [c, n] : services.store.scores(has->second, sha))
            totals[c] += n;
    }
    std::int64_t functions = 0;
    for (const auto& [_, n] : totals)
        functions += n;
    say(services.out, std::format("{} of {} modules measured under the current verifier settings; {} functions: {}", done.size(), paths.size(),
                                  functions, totals_text(totals)));
    return done;
}

int run(const Options& options, const Services& services) {
    if (services.tools_missing)
        if (const std::optional<std::string> why = services.tools_missing()) {
            say(services.err, *why);
            return 1;
        }
    std::vector<std::filesystem::path> paths;
    for (const std::string& kind : options.kinds) {
        std::vector<std::filesystem::path> of_kind = services.modules(kind);
        paths.insert(paths.end(), of_kind.begin(), of_kind.end());
    }
    std::set<std::string> listed;   // (a DOS program with a Windows or driver image is listed by both lanes: it is one module)
    std::erase_if(paths, [&](const std::filesystem::path& p) { return !listed.insert(p.generic_string()).second; });
    if (options.only)
        std::erase_if(paths, [&](const std::filesystem::path& p) { return !options.only->contains(file_name_lower(p)); });

    const auto done = status(services, paths);
    if (options.status_only)
        return 0;
    std::set<std::string> is_done;   // by path text
    for (const auto& [path, _] : done)
        is_done.insert(path.generic_string());

    int failed = 0;
    for (std::size_t i = 0; i < paths.size(); ++i) {
        const std::filesystem::path& path = paths[i];
        const std::string name = file_name_lower(path);
        const std::optional<reconstruct::Identity> id = services.reconstruct.try_identity(path);
        if (!id) {
            ++failed;
            say(services.out, std::format("[{}/{}] {} FAILED: no way to reconstruct this file", i + 1, paths.size(), name));
            continue;
        }
        if (!options.again && is_done.contains(path.generic_string())) {
            const auto judged = services.store.best(id->sha, id->profile);
            bool all = true;
            const bool driver = services.reconstruct.kind_name(path) == std::optional<std::string>("le");
            for (const std::string& a : services.store.addresses(id->sha))
                if (!judged.contains(a) && (!driver || a.find(':') != std::string::npos)) {   // (see Ledger::counts)
                    all = false;
                    break;
                }
            if (all)
                continue;
        }
        const auto started = std::chrono::steady_clock::now();
        try {
            (void)services.reconstruct.run(path);
        } catch (const std::exception& e) {   // isolation boundary: one module must not end the sweep
            ++failed;
            say(services.out, std::format("[{}/{}] {} FAILED: {}", i + 1, paths.size(), name, std::string(e.what()).substr(0, 120)));
            continue;
        }
        const double seconds = std::chrono::duration<double>(std::chrono::steady_clock::now() - started).count();
        say(services.out, std::format("[{}/{}] {}: {}  ({:.0f}s)", i + 1, paths.size(), name,
                                      totals_text(services.store.scores(id->profile, id->sha)), seconds));
    }
    (void)status(services, paths);
    return failed ? 1 : 0;
}

}  // namespace cb::verify::sweep
