// cb/verify/measure.cpp -- see measure.hpp.

#include "cb/verify/measure.hpp"

#include <algorithm>
#include <format>

#include "cb/core/error.hpp"
#include "cb/core/log.hpp"
#include "cb/core/ordered_map.hpp"
#include "cb/core/text.hpp"

namespace cb::verify::measure {

namespace {

// Counts by key, kept in the order first seen so that equal counts list the first seen first.
class Tally {
public:
    void add(const std::string& key) { ++counts_[key]; }
    std::vector<Cause> most_common(std::size_t limit = static_cast<std::size_t>(-1)) const {
        std::vector<Cause> out;
        for (const auto& [reason, count] : counts_)
            out.push_back({reason, count});
        std::stable_sort(out.begin(), out.end(), [](const Cause& a, const Cause& b) { return a.count > b.count; });
        if (out.size() > limit)
            out.resize(limit);
        return out;
    }

private:
    OrderedMap<std::string, std::size_t> counts_;
};

// The first `n` bytes of text, not ending inside a UTF-8 sequence.
std::string_view prefix_bytes(std::string_view s, std::size_t n) {
    if (s.size() <= n)
        return s;
    while (n > 0 && (static_cast<unsigned char>(s[n]) & 0xC0) == 0x80)
        --n;
    return s.substr(0, n);
}

} // namespace

Summary summarise(std::span<const Row> rows) {
    Tally why, bad;
    for (const Row& r : rows) {
        if (r.verdict == Verdict::Divergent)
            why.add(r.reason.empty() ? "(no reason given)" : r.reason);
        if (r.verdict == Verdict::NoCompile)
            bad.add(std::string(prefix_bytes(r.reason, kCauseKeyLength)));
    }
    return {why.most_common(), bad.most_common(kCompileCausesShown)};
}

std::string render(const Summary& summary) {
    std::string out;
    if (!summary.unverified.empty()) {
        std::vector<std::string> parts;
        for (const Cause& c : summary.unverified)
            parts.push_back(std::format("{} {}", c.reason, c.count));
        out += "unverified by cause: " + text::join(parts, ", ") + "\n";
    }
    if (!summary.no_compile.empty()) {
        std::vector<std::string> parts;
        for (const Cause& c : summary.no_compile)
            parts.push_back(std::format("{} ({})", c.reason, c.count));
        out += "does not compile: " + text::join(parts, "; ") + "\n";
    }
    return out;
}

int run(const Services& services, std::string_view module, Engine engine) {
    std::optional<std::filesystem::path> path = services.find_module(module);
    if (!path)
        throw NotFoundError(std::filesystem::path(module), 2, "no such module in the corpus");
    const std::vector<Row> rows = services.judge(*path, engine);
    out_text(render(summarise(rows)));
    return kExitOk;
}

cli::Command command(Services services) {
    return cli::Command{
        "measure",
        "reconstruct and judge one module; writes nothing to the tree",
        {cli::Positional{"module", "a corpus name, or a folder (a Java program)"},
         cli::Option{"--no-engine", "", "run the old direct path instead of the engine", cli::Arg::Flag},
         cli::Option{"--engine", "", "run the engine (the default; kept for scripts)", cli::Arg::Flag}},
        [services = std::move(services)](const cli::Args& args) {
            Engine engine = Engine::Default;
            if (args.flag("engine"))
                engine = Engine::On;
            if (args.flag("no-engine"))
                engine = Engine::Off; // (given with --engine, the old path wins)
            return run(services, args.required("module"), engine);
        },
        {},
        "",
        "Reconstructs and judges the module, then prints why functions are unverified and what did not compile. Only "
        "what changed since the last measure is made again. This is the number to watch while changing the decompiler "
        "or the verifier."};
}

} // namespace cb::verify::measure
