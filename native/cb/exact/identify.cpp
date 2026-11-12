#include "cb/exact/identify.hpp"

#include <algorithm>
#include <format>
#include <random>
#include <set>

#include <nlohmann/json.hpp>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/log.hpp"
#include "cb/core/text.hpp"

namespace cb::exact::identify {

namespace {

// Put a project's switches back when the scoring ends, however it ends.
class IdentifySwitches {
public:
    IdentifySwitches(const ledger::Project& project, const Services& services) : set_(services.set_switches), old_(project.switches()) {}
    ~IdentifySwitches() { set_(old_); }
    IdentifySwitches(const IdentifySwitches&) = delete;
    IdentifySwitches& operator=(const IdentifySwitches&) = delete;
    const std::optional<std::string>& old() const { return old_; }

private:
    std::function<void(const std::optional<std::string>&)> set_;
    std::optional<std::string> old_;
};

// "NAME[:VARIANT][/SWITCHES]"
Candidate identify_parse(std::string_view t) {
    std::string_view name_variant = t, switches;
    if (const std::size_t slash = t.find('/'); slash != std::string_view::npos) {
        name_variant = t.substr(0, slash);
        switches = t.substr(slash + 1);
    }
    std::string_view name = name_variant, variant;
    if (const std::size_t colon = name_variant.find(':'); colon != std::string_view::npos) {
        name = name_variant.substr(0, colon);
        variant = name_variant.substr(colon + 1);
    }
    return {std::string(name), variant.empty() ? std::nullopt : std::optional(std::string(variant)),
            switches.empty() ? std::nullopt : std::optional(std::string(switches))};
}

std::string identify_extension(const std::filesystem::path& p) { return text::lower(fs::to_text(p.extension())); }

}  // namespace

std::vector<Candidate> candidates(const ledger::Project& project, const toolchain::Locations& locations, std::span<const std::string> tries) {
    std::vector<Candidate> out;
    if (!tries.empty()) {
        for (const std::string& t : tries) out.push_back(identify_parse(t));
        return out;
    }
    std::set<std::string> extensions;
    for (const ledger::SourceFile& s : project.sources()) extensions.insert(identify_extension(s.path));
    for (const std::string& n : locations.profile_names()) {
        const toolchain::Profile p = toolchain::read_profile(locations, n);
        // the extension its sources have: the profile's, else by its name
        std::string ext = ".c";
        if (n.find("pascal") != std::string::npos) ext = ".pas";
        else if (n.find("plm") != std::string::npos) ext = ".plm";
        if (const auto given = p.strings.find("source_ext"); given != p.strings.end() && !given->second.empty()) ext = given->second;
        if (!extensions.contains(text::lower(ext))) continue;
        out.push_back({n, std::nullopt, std::nullopt});   // (the default variant; --try names others)
        for (const std::string& sw : p.try_switches) out.push_back({n, std::nullopt, sw});
    }
    return out;
}

Score score(const ledger::Project& project, const Candidate& candidate, std::span<const ledger::SourceFile> files, const Services& services) {
    std::shared_ptr<const ledger::Toolchain> tc;
    try {
        tc = services.load(candidate.toolchain, candidate.variant);
    } catch (const Error& e) {   // a toolchain this machine has no kit for: its score says so
        return {-1, e.what()};
    }
    const SymbolTable symbols = project.symbols();
    const Checker check = services.check ? services.check : Checker([](const ledger::Project& p, const ledger::Toolchain& t,
                                                                         const ledger::SourceFile& f, const SymbolTable& syms,
                                                                         SymbolTable& learnt) {
        return ledger::check_file(p, t, f.path, f.at, syms, learnt);
    });
    CB_ASSERT(services.set_switches != nullptr, "identify needs a way to set the project's switches");
    const IdentifySwitches saved(project, services);
    services.set_switches(candidate.switches ? candidate.switches : saved.old());
    std::int64_t same = 0, total = 0;
    for (const ledger::SourceFile& f : files) {
        SymbolTable learnt;
        for (const ledger::Row& r : check(project, *tc, f, symbols, learnt)) {
            ++total;
            same += r.verdict == ledger::Verdict::Identical ? 1 : 0;
        }
    }
    return {same, std::format("{} of {}", same, total)};
}

std::vector<ledger::SourceFile> sample(std::vector<ledger::SourceFile> files, std::size_t n) {
    std::stable_sort(files.begin(), files.end(), [](const ledger::SourceFile& a, const ledger::SourceFile& b) {
        const std::int64_t x = a.at.value_or(-1), y = b.at.value_or(-1);   // a file without an address first
        return x != y ? x < y : fs::to_text(a.path) < fs::to_text(b.path);
    });
    std::mt19937_64 random(0);   // (a fixed seed: the same project is always asked the same question)
    for (std::size_t i = files.size(); i > 1; --i) std::swap(files[i - 1], files[std::uniform_int_distribution<std::size_t>(0, i - 1)(random)]);
    if (files.size() > n) files.resize(n);
    return files;
}

int command(const ledger::Project& project, const std::vector<std::string>& args, const Services& services) {
    const auto report = [&](const std::string& line) {
        if (services.say) services.say(line);
        else out("{}\n", line);
    };
    std::vector<std::string> tries;
    std::size_t n = 12;
    bool write = false;
    for (std::size_t k = 0; k < args.size(); ++k) {
        if (args[k] == "--try" && k + 1 < args.size()) tries.push_back(args[k + 1]);
        if (args[k] == "--sample") {
            if (k + 1 >= args.size()) throw UsageError("--sample needs a number");
            const auto count = text::parse_int<std::size_t>(args[k + 1], 10);
            if (!count) throw UsageError("--sample needs a number, not '" + args[k + 1] + "'");
            n = *count;
        }
        if (args[k] == "--write") write = true;
    }
    const std::vector<ledger::SourceFile> files = sample(project.sources(), n);

    std::optional<std::pair<std::int64_t, Candidate>> best;
    for (const Candidate& c : candidates(project, services.locations, tries)) {
        const Score s = score(project, c, files, services);
        report(std::format("   {:<10} {:<8} {:<8} {}", c.toolchain, c.variant.value_or(""), c.switches.value_or(""), s.text));
        if (!best || s.same > best->first) best = std::make_pair(s.same, c);
    }
    if (!best || best->first <= 0) {
        report("no toolchain makes any of the sample's functions the program's");
        return 1;
    }
    const Candidate& b = best->second;
    report(std::format("the best: {}{}{}", b.toolchain, b.variant && !b.variant->empty() ? ":" + *b.variant : "",
                       b.switches && !b.switches->empty() ? " " + *b.switches : ""));
    if (write) {
        const std::filesystem::path p = project.dir() / "project.json";
        Json j = json::load(p);
        j["toolchain"] = b.toolchain;
        if (b.variant && !b.variant->empty()) j["variant"] = *b.variant;
        else j.erase("variant");
        if (b.switches && !b.switches->empty()) j["switches"] = *b.switches;
        json::save(p, j, json::Style::Pretty);
        report("written to " + fs::to_text(p));
    }
    return 0;
}

}  // namespace cb::exact::identify
