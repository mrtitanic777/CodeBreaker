#include "cb/source/placement.hpp"

#include <algorithm>
#include <nlohmann/json.hpp>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/log.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/dos.hpp"

namespace cb::source::placement {
namespace stdfs = std::filesystem;

std::string_view how_name(How how) {
    switch (how) {
        case How::AlreadyThere: return "already there";
        case How::Known: return "known";
        case How::Kind: return "kind";
        case How::Installed: return "installed";
        case How::Suffix: return "suffix";
        case How::Prefix: return "prefix";
        case How::Unsorted: return "unsorted";
    }
    return "unsorted";
}

namespace {

// Folders of the tree that hold no module folders.
bool skipped_folder(std::string_view name) {
    return name == "design" || name == "build" || name == "tools" || name == "inc" || name.starts_with('.');
}

constexpr std::string_view kReader = "placement.json";

std::string component(const nlohmann::ordered_json& v, std::string_view part) {
    if (!v.is_string()) throw FormatError(kReader, 0, std::string(part) + ": a component must be text");
    return v.get<std::string>();
}

// {"name": "component", ...} in the file's order.
template <class Add>
void each_pair(const nlohmann::ordered_json& entry, std::string_view part, Add&& add) {
    if (!entry.is_object()) throw FormatError(kReader, 0, std::string(part) + " must be an object of strings");
    for (auto it = entry.begin(); it != entry.end(); ++it) add(it.key(), component(it.value(), part));
}

// The name without its last extension: `name.dll` -> `name`. A name whose only dot leads it (".bashrc") has nothing left.
std::string_view stem_of(std::string_view name) {
    const std::size_t dot = name.rfind('.');
    if (dot == std::string_view::npos || dot + 1 >= name.size()) return name;
    return name.substr(0, dot);
}

// The extension with its dot, as a path's extension: none for a name that only starts with dots.
std::string_view extension_of(std::string_view name) {
    const std::size_t first = name.find_first_not_of('.');
    const std::size_t dot = name.rfind('.');
    if (first == std::string_view::npos || dot == std::string_view::npos || dot < first) return {};
    return name.substr(dot);
}

std::string lower_name(std::string_view module) {
    const std::size_t slash = module.find_last_of("/\\");
    return text::lower(slash == std::string_view::npos ? module : module.substr(slash + 1));
}

}  // namespace

ProductTable load_table(const stdfs::path& file, std::string_view product) {
    const Json doc = json::load(file);
    if (!doc.is_object()) throw FormatError(kReader, 0, "the tables are not an object");
    ProductTable table;
    const auto entry = doc.find(std::string(product));
    if (entry == doc.end() || !entry->is_object()) return table;
    for (auto it = entry->begin(); it != entry->end(); ++it) {
        const std::string& part = it.key();
        const auto& value = it.value();
        if (part == "known") {
            each_pair(value, part, [&](const std::string& k, std::string c) { table.known[k] = std::move(c); });
        } else if (part == "kind") {
            each_pair(value, part, [&](const std::string& k, std::string c) { table.kind[k] = std::move(c); });
        } else if (part == "installed") {
            each_pair(value, part, [&](const std::string& k, std::string c) { table.installed.insert_or_assign(k, std::move(c)); });
        } else if (part == "suffix") {
            each_pair(value, part, [&](const std::string& k, std::string c) { table.suffix[k] = std::move(c); });
        } else if (part == "prefix") {
            if (!value.is_array()) throw FormatError(kReader, 0, "prefix must be a list of pairs");
            for (const auto& pair : value) {
                if (!pair.is_array() || pair.size() != 2) throw FormatError(kReader, 0, "a prefix must be a [prefix, component] pair");
                table.prefix.emplace_back(component(pair[0], part), component(pair[1], part));
            }
        }
    }
    return table;
}

Placement::Placement(stdfs::path tree_root, ProductTable table) : tree_root_(std::move(tree_root)), table_(std::move(table)) {}

const std::unordered_map<std::string, std::string>& Placement::existing() const {
    std::call_once(existing_once_, [this] {
        std::error_code ec;
        if (!stdfs::is_directory(tree_root_, ec)) return;
        // Names are sorted (byte order) so that the same name in two components goes to the same one every run.
        const auto folders = [](const stdfs::path& dir) {
            std::vector<std::string> names;
            std::error_code e;
            for (const auto& entry : stdfs::directory_iterator(dir, e)) {
                if (entry.is_directory(e)) names.push_back(fs::to_text(entry.path().filename()));
            }
            std::sort(names.begin(), names.end());
            return names;
        };
        for (const std::string& top : folders(tree_root_)) {
            if (skipped_folder(top)) continue;
            for (const std::string& name : folders(tree_root_ / fs::from_text(top))) existing_.try_emplace(text::lower(name), top + "/" + name);
        }
    });
    return existing_;
}

Decision Placement::resolve(std::string_view module, const stdfs::path& file, std::optional<base::Lane> lane) const {
    const std::string name = lower_name(module);
    const std::string stem(stem_of(name));
    const auto joined = [&](const std::string& comp) { return comp + "/" + stem; };

    if (const auto here = existing().find(stem); here != existing().end() && !here->second.empty()) return {here->second, How::AlreadyThere};
    if (const auto it = table_.known.find(stem); it != table_.known.end()) return {it->second, How::Known};   // already a folder, not a component
    if (lane) {
        if (const auto it = table_.kind.find(std::string(base::lane_name(*lane))); it != table_.kind.end()) return {joined(it->second), How::Kind};
    }
    std::string where = text::lower(fs::to_text(file));
    std::replace(where.begin(), where.end(), '\\', '/');
    for (const auto& [part, comp] : table_.installed) {
        if (where.find(part) != std::string::npos) return {joined(comp), How::Installed};
    }
    if (const auto it = table_.suffix.find(std::string(extension_of(name))); it != table_.suffix.end()) return {joined(it->second), How::Suffix};
    for (const auto& [prefix, comp] : table_.prefix) {
        if (stem.starts_with(prefix)) return {joined(comp), How::Prefix};
    }
    return {"unsorted/" + stem, How::Unsorted};
}

stdfs::path Placement::folder(std::string_view module, const stdfs::path& file, std::optional<base::Lane> lane, bool create) const {
    const Decision d = resolve(module, file, lane);
    stdfs::path dir = tree_root_;
    for (const std::string_view part : text::split(d.folder, '/')) dir /= fs::from_text(part);
    if (create) fs::make_dirs(dir);
    return dir;
}

namespace {

// Every real-mode program of the corpus the DOS reader accepts, one path per name, by name.
std::vector<stdfs::path> dos_programs(const base::Corpus& corpus) {
    static constexpr std::string_view kExtensions[] = {".exe", ".com", ".sys", ".dos", ".mod", ".new", ".vxd", ".386", ".bin", ".ovl"};
    const auto index = corpus.index();
    std::vector<const base::Corpus::Index::value_type*> names;   // sorted below: the output is per name, in name order
    for (const auto& entry : *index) names.push_back(&entry);
    std::sort(names.begin(), names.end(), [](const auto* a, const auto* b) { return a->first < b->first; });
    std::vector<stdfs::path> out;
    for (const auto* entry : names) {
        const bool wanted = std::any_of(std::begin(kExtensions), std::end(kExtensions), [&](std::string_view ext) { return entry->first.ends_with(ext); });
        if (!wanted || entry->second.empty()) continue;
        try {
            if (formats::dos::kind_of(fs::read_file(entry->second.front()), fs::to_text(entry->second.front().filename()))) out.push_back(entry->second.front());
        } catch (const IoError&) {
            // a file that cannot be read is not a program of the corpus
        }
    }
    return out;
}

}  // namespace

int place_command(const Placement& placement, const base::Corpus& corpus, std::span<const std::string> args) {
    std::vector<std::string> names;
    bool all = false, list = false;
    for (const std::string& a : args) {
        if (a == "--all") all = true;
        if (a == "--list") list = true;
        if (!a.starts_with("--")) names.push_back(a);
    }
    std::vector<stdfs::path> paths;
    if (all) {
        for (const base::Lane lane : {base::Lane::Pe, base::Lane::Ne}) {
            for (stdfs::path& p : corpus.modules(lane)) paths.push_back(std::move(p));
        }
        for (stdfs::path& p : corpus.modules(base::Lane::Le, {".vxd", ".386"})) paths.push_back(std::move(p));
        for (stdfs::path& p : dos_programs(corpus)) paths.push_back(std::move(p));
    } else {
        for (const std::string& n : names) paths.push_back(corpus.retail(n).value_or(fs::from_text(n)));
    }
    if (paths.empty()) {
        cb::out("codebreaker place <module> ... | --all [--list]\n\n"
                "Say where each module's source goes in the source tree, and how that was decided. Nothing\n"
                "is written.\n");
        return kExitUsage;
    }
    std::vector<std::pair<How, std::size_t>> tally;   // in the order each way was first met; ties keep it
    for (const stdfs::path& p : paths) {
        std::error_code ec;
        const bool is_file = stdfs::is_regular_file(p, ec);
        const std::string module = fs::to_text(p.filename());
        const Decision d = placement.resolve(module, is_file ? p : stdfs::path{}, is_file ? corpus.lane(p) : std::nullopt);
        const auto seen = std::find_if(tally.begin(), tally.end(), [&](const auto& t) { return t.first == d.how; });
        if (seen == tally.end()) tally.emplace_back(d.how, 1); else ++seen->second;
        if (!all || list) cb::out("{:<16} {:<32} {}\n", text::lower(module), d.folder, how_name(d.how));
    }
    std::stable_sort(tally.begin(), tally.end(), [](const auto& a, const auto& b) { return a.second > b.second; });
    std::string summary;
    for (const auto& [how, count] : tally) summary += (summary.empty() ? "" : ", ") + std::string(how_name(how)) + " " + std::to_string(count);
    cb::out("{} modules: {}\n", paths.size(), summary);
    return kExitOk;
}

}  // namespace cb::source::placement
