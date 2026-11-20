// cb-unity: alone (helpers named for what they do here)
#include "port/tool_report.hpp"

#include <algorithm>
#include <format>

#include "core/fs.hpp"
#include "core/regex.hpp"
#include "core/text.hpp"

namespace cb::port::tool {

namespace {

// A complaint with what is in quotes made alike, so that "'x' undeclared" counts once for every x.
std::string complaint_shape(std::string_view text) {
    static const Regex quoted(R"('[^']*')");
    return quoted.replace_all(text, "'..'");
}

std::string last_folder_name(const std::filesystem::path& p) {
    std::filesystem::path n = p.lexically_normal();
    if (!n.has_filename()) n = n.parent_path();
    return fs::to_text(n.filename());
}

}  // namespace

std::string render_report(const ReportInput& in) {
    std::vector<std::string> L;
    L.push_back(std::format("# Port of `{}`", last_folder_name(in.source)));
    L.push_back("");
    L.push_back(std::format("Made by CodeBreaker's port from `{}` (read only), for **{}** (`{}`).", fs::to_text(in.source), in.compiler,
                            in.compiler_path ? fs::to_text(*in.compiler_path) : "not found"));
    L.push_back("");
    L.push_back(std::format("**{}.** {} rounds of the compiler's complaints mended.", in.built ? "Built" : "Not built", in.rounds));
    L.push_back("");

    L.insert(L.end(), {"## Left out: the old runtime", "", "| file | why |", "|---|---|"});
    std::vector<std::pair<std::string, std::string>> left(in.left_out.begin(), in.left_out.end());
    std::sort(left.begin(), left.end());
    for (const auto& [n, w] : left) L.push_back(std::format("| {} | {} |", n, w));
    for (const auto& [n, role] : in.roles) {   // (a map: by name)
        const std::string pubs = role.publics.empty() ? "nothing public" : text::join(role.publics, ", ");
        std::string why;
        switch (role.kind) {
            case AsmKind::Runtime: why = std::format("assembler routines of the old runtime ({}): the C library and the platform layer stand in", pubs); break;
            case AsmKind::Data: why = std::format("data alone ({})", pubs); break;
            case AsmKind::Code: why = std::format("the program's own assembly language ({}): ported as assembly language (below)", pubs); break;
        }
        L.push_back(std::format("| {} | {} |", n, why));
    }

    L.insert(L.end(), {"", "## Changes", "", "| change | how many | where (first) |", "|---|---|---|"});
    std::vector<std::pair<std::string, std::vector<std::string>>> changes(in.notes.begin(), in.notes.end());
    std::stable_sort(changes.begin(), changes.end(), [](const auto& a, const auto& b) {
        if (a.second.size() != b.second.size()) return a.second.size() > b.second.size();
        return false;
    });
    // (the order of the notes as they came, among equals: sorted by count, largest first)
    for (const auto& [k, v] : changes) {
        std::vector<std::string> first(v.begin(), v.begin() + static_cast<std::ptrdiff_t>(std::min<std::size_t>(4, v.size())));
        L.push_back(std::format("| {} | {} | {}{} |", k, v.size(), text::join(first, ", "), v.size() > 4 ? " ..." : ""));
    }
    if (!in.assembler_notes.empty()) {
        L.insert(L.end(), {"", "## The program's assembly language", ""});
        for (const std::string& x : in.assembler_notes) L.push_back("- " + x);
    }
    if (!in.sanitizer_notes.empty()) {
        L.insert(L.end(), {"", "## Run under AddressSanitizer (the verify cases)", ""});
        for (const std::string& s : in.sanitizer_notes) L.push_back("- " + s);
    }
    L.insert(L.end(), {"", "## Behaviour that may differ", ""});
    for (const auto& [k, v] : in.behaviour) L.push_back(std::format("- **{}**: {}", k, v));
    L.push_back(std::format("- **int** is {} bits in the original and 32 here: arithmetic that relied on 16-bit wrap-around differs "
                            "(structure members keep 16 bits); the verifier (verify.py) is what shows it.", in.int_bits));
    if (in.char_signed) L.push_back("- **char** was signed: built with -fsigned-char where the compiler has it.");

    std::vector<const Diagnostic*> left_say;
    for (const Diagnostic& d : in.diagnostics)
        if (d.kind == "error" || d.kind == "warning") left_say.push_back(&d);
    L.insert(L.end(), {"", std::format("## What the compiler still says ({})", left_say.size()), ""});
    // each shape of complaint and how often, the most frequent first (equal counts in the order first met)
    std::vector<std::pair<std::string, std::size_t>> counts;
    for (const Diagnostic* d : left_say) {
        const std::string shape = complaint_shape(d->text);
        const auto it = std::find_if(counts.begin(), counts.end(), [&](const auto& c) { return c.first == shape; });
        if (it == counts.end()) counts.emplace_back(shape, 1);
        else ++it->second;
    }
    std::stable_sort(counts.begin(), counts.end(), [](const auto& a, const auto& b) { return a.second > b.second; });
    for (std::size_t i = 0; i < std::min<std::size_t>(30, counts.size()); ++i) L.push_back(std::format("- {} x {}", counts[i].second, counts[i].first));
    if (!in.built && !in.build_log.empty()) {
        const std::string tail = in.build_log.size() > 3000 ? in.build_log.substr(in.build_log.size() - 3000) : in.build_log;
        L.insert(L.end(), {"", "## Build", "", "```", tail, "```"});
    }
    return text::join(L, "\n") + "\n";
}

void write_report(const std::filesystem::path& out, const ReportInput& input) { fs::write_text(out / "PORT.md", render_report(input)); }

}  // namespace cb::port::tool
