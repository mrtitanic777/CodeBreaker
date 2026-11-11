// cb/source/vxd/services.cpp -- VxD service tables from the DDK includes (see services.hpp).
#include "cb/source/vxd/services.hpp"

#include <algorithm>
#include <array>
#include <set>

#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"

namespace cb::source::vxd {
namespace {

using std::string;
using std::string_view;

// A letter of a name: ASCII letters, digits and underscore, and any byte of a non-ASCII letter (the text is UTF-8).
bool is_word(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' ||
           static_cast<unsigned char>(c) >= 0x80;
}

// The position of `needle` in `hay` from `from`, letters compared without regard to case.
size_t ifind(string_view hay, string_view needle, size_t from) {
    if (needle.empty() || hay.size() < needle.size()) return string_view::npos;
    for (size_t i = from; i + needle.size() <= hay.size(); ++i) {
        if (text::iequals(hay.substr(i, needle.size()), needle)) return i;
    }
    return string_view::npos;
}

// The service names of a table's body: every line `   <DEV>_Service  name` (the device's name in the line without
// regard to case).
std::vector<string> service_names(string_view body, string_view device) {
    const string marker = string(device) + "_Service";
    std::vector<string> names;
    size_t line_start = 0;
    while (line_start <= body.size()) {
        size_t eol = body.find('\n', line_start);
        if (eol == string_view::npos) eol = body.size();
        string_view line = text::ltrim(body.substr(line_start, eol - line_start));
        if (line.size() > marker.size() && text::iequals(line.substr(0, marker.size()), marker) && text::is_space(line[marker.size()])) {
            line = text::ltrim(line.substr(marker.size()));
            size_t n = 0;
            while (n < line.size() && is_word(line[n])) ++n;
            if (n > 0) names.emplace_back(line.substr(0, n));
        }
        line_start = eol + 1;
    }
    return names;
}

}  // namespace

void ServiceTables::parse_into(ServiceTables& into, std::string_view inc_text) {
    constexpr string_view kBegin = "Begin_Service_Table";
    constexpr string_view kEnd = "End_Service_Table";
    size_t at = 0;
    while ((at = ifind(inc_text, kBegin, at)) != string_view::npos) {
        size_t p = at + kBegin.size();
        const size_t gap = p;
        while (p < inc_text.size() && text::is_space(inc_text[p])) ++p;
        const size_t dev_start = p;
        while (p < inc_text.size() && is_word(inc_text[p])) ++p;
        if (p == gap || p == dev_start) {   // not followed by a device name: not a table
            at = at + kBegin.size();
            continue;
        }
        const string device = text::upper(inc_text.substr(dev_start, p - dev_start));
        size_t end = ifind(inc_text, kEnd, p);
        const string_view body = inc_text.substr(p, (end == string_view::npos ? inc_text.size() : end) - p);
        at = end == string_view::npos ? inc_text.size() : end + kEnd.size();
        std::vector<string> names = service_names(body, device);
        if (!names.empty() && !into.tables_.contains(device)) into.tables_.try_emplace(device, std::move(names));   // the first (canonical) table wins
    }
}

ServiceTables ServiceTables::load(const std::filesystem::path& inc_folder) {
    std::vector<std::filesystem::path> files;
    std::error_code ec;
    for (const auto& entry : std::filesystem::directory_iterator(inc_folder, ec)) {
        if (entry.is_regular_file() && text::iends_with(fs::to_text(entry.path().filename()), ".inc")) files.push_back(entry.path());
    }
    std::sort(files.begin(), files.end(), [](const auto& a, const auto& b) {
        return text::lower(fs::to_text(a.filename())) < text::lower(fs::to_text(b.filename()));
    });
    ServiceTables tables;
    for (const auto& file : files) {
        string text;
        try {
            text = text::from_latin1(fs::read_file(file));   // the DDK's text is Latin-1
        } catch (const IoError&) {   // a file that cannot be read has no tables
            continue;
        }
        text = text::replace_all(text, "\r\n", "\n");
        std::replace(text.begin(), text.end(), '\r', '\n');
        parse_into(tables, text);
    }
    return tables;
}

std::optional<std::filesystem::path> ServiceTables::find_inc_folder(const DdkFolders& ddk,
                                                                    const std::optional<std::filesystem::path>& design_dir) {
    for (const auto& candidate : ddk.inc) {
        std::error_code ec;
        if (!candidate.empty() && std::filesystem::is_directory(candidate, ec)) return candidate;
    }
    // Bounded fallback. A search of every folder under the design arena (which, by default, is the parent of the
    // checkout) crawled every sibling repository on disk. Walk a shallow depth only, pruning version-control, build and
    // corpus folders, so a missing configuration degrades gracefully instead of scanning the whole tree.
    if (!design_dir || design_dir->empty()) return std::nullopt;
    std::error_code ec;
    if (!std::filesystem::is_directory(*design_dir, ec)) return std::nullopt;
    static const std::set<string> prune = {".git", ".hg", ".svn", "node_modules", "__pycache__", ".venv", "venv",
                                           "open-watcom-v2", "binaries", "corpus"};
    constexpr int kMaxDepth = 4;
    std::vector<std::pair<std::filesystem::path, int>> pending = {{*design_dir, 0}};
    while (!pending.empty()) {   // depth-first, a folder's own files before its sub-folders
        const auto [dir, depth] = pending.back();
        pending.pop_back();
        if (depth >= kMaxDepth) continue;
        if (text::iequals(fs::to_text(dir.filename()), "INC32") && std::filesystem::exists(dir / "VMM.INC", ec)) return dir;
        std::vector<std::filesystem::path> children;
        for (const auto& entry : std::filesystem::directory_iterator(dir, ec)) {
            const string name = fs::to_text(entry.path().filename());
            if (entry.is_directory(ec) && !prune.contains(name) && !name.starts_with('.')) children.push_back(entry.path());
        }
        std::sort(children.begin(), children.end(), std::greater<>());   // popped in order
        for (const auto& child : children) pending.emplace_back(child, depth + 1);
    }
    return std::nullopt;
}

ServiceTables ServiceTables::standard(const DdkFolders& ddk, const std::optional<std::filesystem::path>& design_dir) {
    const auto folder = find_inc_folder(ddk, design_dir);
    return folder ? load(*folder) : ServiceTables();
}

const std::vector<std::string>* ServiceTables::names(std::string_view device) const {
    return tables_.find(text::upper(device));
}

std::optional<std::string> ServiceTables::name(std::string_view device, std::uint32_t ordinal) const {
    const std::vector<string>* list = names(device);
    if (list == nullptr || list->empty()) return std::nullopt;
    for (const std::uint32_t index : {ordinal, ordinal & 0x7FFFu}) {
        if (index < list->size()) return (*list)[index];
    }
    return std::nullopt;
}

}  // namespace cb::source::vxd
