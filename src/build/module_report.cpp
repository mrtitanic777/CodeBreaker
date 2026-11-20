// build/module_report.cpp -- what a build found out, as text.

#include <algorithm>
#include <format>

#include "build/module.hpp"
#include "core/fs.hpp"
#include "core/text.hpp"

namespace cb::build::module {

Built::Built(std::filesystem::path original_, std::optional<std::string> kind_of)
    : original(std::move(original_)), name(text::lower(original.filename().string())), kind(std::move(kind_of)) {}

namespace {

std::string first_of(const std::vector<std::string>& names, std::size_t n) {
    std::vector<std::string> head(names.begin(), names.begin() + static_cast<std::ptrdiff_t>(std::min(n, names.size())));
    return text::join(head, ", ") + (names.size() > n ? " ..." : "");
}

}  // namespace

std::string Built::report() const {
    if (carried_report) return *carried_report;
    if (!linked()) return std::format("{}: NOT BUILT: {}", name, error);
    const std::uint64_t size = fs::file_size(*image), was = fs::file_size(original);
    std::vector<std::string> out;
    out.push_back(std::format("{}: linked, {} bytes (the original is {}: {:.0f}%)", name, size, was,
                              100.0 * static_cast<double>(size) / static_cast<double>(std::max<std::uint64_t>(1, was))));
    out.push_back(std::format("  functions   {} in the module, {} with reconstructed C, {} compiled, {} rejected by the compiler (stand-ins)", functions,
                              with_text, with_text - rejected.size(), rejected.size()));
    out.push_back(std::format("              {} called but not reconstructed (stand-ins), {} left unresolved", absent.size(), undefined.size()));
    if (!data_error.empty()) {
        out.push_back("  data        NOT CARRIED: " + data_error);
    } else if (data) {
        out.push_back(std::format("  data        {} bytes as C, {} pointers written as pointers, {} left as numbers, {} addresses used by the code not placed",
                                  data->size, data->pointers, data->unresolved.size(), lost_addresses));
    }
    if (!code_data.empty()) {
        std::vector<std::string> parts;
        for (const CodeDataSegment& s : code_data) parts.push_back(std::format("{} (for {})", s.segment, first_of(s.readers, 4)));
        out.push_back("  code data   the original's bytes the C reads through CS, in its segment: " + text::join(parts, ", "));
    }
    if (!disputes.empty()) {
        std::vector<std::string> parts;
        for (std::size_t i = 0; i < std::min<std::size_t>(12, disputes.size()); ++i) parts.push_back(std::format("{} (callers: {})", disputes[i].function, disputes[i].who));
        out.push_back(std::format("  conventions {} functions whose callers contradict how they are described (who removes the arguments): {}", disputes.size(),
                                  text::join(parts, ", ")));
    }
    if (contract_error && !contract_error->empty()) out.push_back("  contracts   NOT CHECKED: " + *contract_error);
    if (gate_error && !gate_error->empty()) out.push_back("  gate        NOT CHECKED for " + *gate_error);
    if (!gated.empty()) {
        std::vector<std::pair<std::string, std::string>> items(gated.begin(), gated.end());
        std::sort(items.begin(), items.end());
        std::string lines;
        for (std::size_t i = 0; i < std::min<std::size_t>(12, items.size()); ++i) lines += std::format("\n              {}: {}", items[i].first, items[i].second.substr(0, 140));
        out.push_back(std::format("  gated       {} functions' C differs from the original (rebuilt from their instructions){}", gated.size(), lines));
    }
    if (!hidden.empty()) {
        std::vector<std::string> parts;
        for (std::size_t i = 0; i < std::min<std::size_t>(16, hidden.size()); ++i) parts.push_back(std::format("{}:{:04X}", hidden[i].seg, hidden[i].off));
        out.push_back(std::format("  HIDDEN      {} functions begin inside another (an entry sequence after its end) and are not described: {}", hidden.size(),
                                  text::join(parts, " ")));
    }
    out.push_back("  entry       " + entry);
    out.push_back(std::string("  format      ") + (format_same ? "same as the original" : "DIFFERENT from the original"));
    {
        std::string missing;
        for (std::size_t i = 0; i < std::min<std::size_t>(12, exports_missing.size()); ++i) {
            const definitions::MissingExport& m = exports_missing[i];
            missing += std::format("\n              missing: {} @{} -- {}", m.what.has_name() ? *m.what.name : std::string("(no name)"), m.what.ordinal, m.why);
        }
        out.push_back(std::format("  exports     {} of {} with the original's ordinal and name ({} of them point at stand-ins){}", exports_same, exports_total,
                                  exports_on_stand_ins, missing));
    }
    {
        std::string all = "all are the original's";
        if (!imports_extra.empty()) {
            std::vector<std::string> parts;
            for (std::size_t i = 0; i < std::min<std::size_t>(8, imports_extra.size()); ++i) parts.push_back(imports_extra[i].module + "." + imports_extra[i].text());
            all = "NOT in the original: " + text::join(parts, ", ");
        }
        out.push_back(std::format("  imports     {} (the original has {}); {}", imports_built, imports_original, all));
    }
    out.push_back("  resources   " + (!resources_line.empty() ? resources_line : std::string("the original has none")));
    if (!ran.empty()) {
        const auto returned = std::count_if(ran.begin(), ran.end(), [](const RanExport& r) { return r.rebuilt == "ret"; });
        const auto same_end = std::count_if(ran.begin(), ran.end(), [](const RanExport& r) { return r.same; });
        out.push_back(std::format("  runs        {} exported functions called with zero arguments: {} returned, {} did what the original does (same end, same result)",
                                  ran.size(), returned, same_end));
    }
    return text::join(out, "\n");
}

}  // namespace cb::build::module
