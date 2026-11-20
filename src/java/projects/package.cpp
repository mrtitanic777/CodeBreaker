// cb-unity: alone (helpers named for what they do here)
#include "java/projects/package.hpp"

#include <algorithm>
#include <format>

#include <nlohmann/json.hpp>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/json.hpp"
#include "core/text.hpp"

namespace cb::java::projects {

// -- the generic project: every hook does nothing -----------------------------------------------------------------------

std::optional<std::string> Project::profile_for(std::string_view edition, std::string_view mc_version, std::span<const ir::JClass>) const {
    if (!edition.empty() || !mc_version.empty()) {
        throw UsageError(std::format("the {} project takes no edition or version (edition '{}', mc_version '{}')", name, edition, mc_version));
    }
    return std::nullopt;
}

Patched Project::ir_patches(Arena&, const std::vector<translate::ir::Node*>&, std::optional<std::string_view>) { return {}; }

std::string Project::finalize_text(std::string text, std::optional<std::string_view>) const { return text; }

void Project::after_emit(const std::filesystem::path&, std::optional<std::string_view>, const Patched&, const Progress&) {}

void Project::after_construction(const std::filesystem::path&, std::optional<std::string_view>, const Progress&) {}

void Project::reports(const std::filesystem::path&, const std::filesystem::path&, std::span<const std::string>, const Progress&) {}

// -- manifests ------------------------------------------------------------------------------------------------------------

namespace {

std::vector<std::string> string_list(const Json& manifest, const char* key) {
    std::vector<std::string> out;
    const auto it = manifest.find(key);
    if (it == manifest.end() || !it->is_array()) return out;
    for (const Json& item : *it) {
        if (item.is_string()) out.push_back(item.get<std::string>());
    }
    return out;
}

std::string string_field(const Json& manifest, const char* key, std::string fallback) {
    const auto it = manifest.find(key);
    return it != manifest.end() && it->is_string() ? it->get<std::string>() : std::move(fallback);
}

Manifest read_manifest(const std::filesystem::path& file, std::string folder_name) {
    Json doc;
    try {
        doc = json::load(file);
    } catch (const Error& e) {
        throw FormatError("project manifest", 0, std::format("{}: {}", fs::to_text(file), e.what()));
    }
    if (!doc.is_object()) throw FormatError("project manifest", 0, fs::to_text(file) + ": not a JSON object");
    Manifest m;
    m.name = string_field(doc, "name", std::move(folder_name));
    m.description = string_field(doc, "description", "");
    m.packages = string_list(doc, "packages");
    m.classes = string_list(doc, "classes");
    m.options = string_list(doc, "options");
    m.profile_prefixes = string_list(doc, "profile_prefixes");
    return m;
}

std::string choices(const std::vector<Manifest>& manifests) {
    std::string out(kGeneric);
    for (const Manifest& m : manifests) out += ", " + m.name;
    return out;
}

}  // namespace

bool matches(const Manifest& manifest, std::span<const ir::JClass> classes) {
    for (const ir::JClass& c : classes) {
        const std::string_view package = c.package ? std::string_view(*c.package) : std::string_view();
        const bool by_package = std::any_of(manifest.packages.begin(), manifest.packages.end(),
                                            [&](const std::string& p) { return package.starts_with(p); });
        const bool by_name = std::find(manifest.classes.begin(), manifest.classes.end(), c.name) != manifest.classes.end();
        if (by_package || by_name) return true;
    }
    return false;
}

std::set<std::string, std::less<>> ChooseOptions::given() const {
    std::set<std::string, std::less<>> out;
    if (!edition.empty()) out.insert("edition");
    if (!mc_version.empty()) out.insert("mc_version");
    return out;
}

// -- the set ------------------------------------------------------------------------------------------------------------------

ProjectSet::ProjectSet(const std::filesystem::path& projects_dir) {
    if (!std::filesystem::is_directory(projects_dir)) return;
    std::vector<std::filesystem::path> folders;
    for (const std::filesystem::directory_entry& e : std::filesystem::directory_iterator(projects_dir)) {
        if (std::filesystem::is_regular_file(e.path() / "manifest.json")) folders.push_back(e.path());
    }
    std::sort(folders.begin(), folders.end(), [](const std::filesystem::path& a, const std::filesystem::path& b) {
        return fs::to_text(a.filename()) < fs::to_text(b.filename());
    });
    for (const std::filesystem::path& folder : folders) {
        manifests_.push_back(read_manifest(folder / "manifest.json", fs::to_text(folder.filename())));
    }
}

void ProjectSet::add(std::string name, std::unique_ptr<Project> project) {
    project->name = name;
    registered_[std::move(name)] = std::move(project);
}

Project& ProjectSet::project(std::string_view name) {
    if (name.empty() || name == kGeneric) return generic_;
    const bool known = std::any_of(manifests_.begin(), manifests_.end(), [&](const Manifest& m) { return m.name == name; });
    if (!known) throw UsageError(std::format("unknown project '{}' (choices: {})", name, choices(manifests_)));
    const auto it = registered_.find(name);
    if (it == registered_.end()) throw UnsupportedError("project", 0, std::format("project '{}' is described by a manifest but not part of this build", name));
    return *it->second;
}

std::string ProjectSet::choose(std::span<const ir::JClass> classes, std::string_view requested, const ChooseOptions& options) const {
    if (!requested.empty()) {
        const bool known = requested == kGeneric ||
                           std::any_of(manifests_.begin(), manifests_.end(), [&](const Manifest& m) { return m.name == requested; });
        if (!known) throw UsageError(std::format("unknown project '{}' (choices: {})", requested, choices(manifests_)));
        return std::string(requested);
    }
    const std::set<std::string, std::less<>> given = options.given();
    if (!given.empty()) {
        for (const Manifest& m : manifests_) {
            // every given option is one the project claims
            const bool claims_all = std::all_of(given.begin(), given.end(), [&](const std::string& g) {
                return std::find(m.options.begin(), m.options.end(), g) != m.options.end();
            });
            if (claims_all) return m.name;
        }
    }
    for (const Manifest& m : manifests_) {
        if (matches(m, classes)) return m.name;
    }
    return std::string(kGeneric);
}

std::optional<std::string> ProjectSet::for_profile(std::string_view profile_name) const {
    for (const Manifest& m : manifests_) {
        const bool hit = std::any_of(m.profile_prefixes.begin(), m.profile_prefixes.end(),
                                     [&](const std::string& prefix) { return profile_name.starts_with(prefix); });
        if (hit) return m.name;
    }
    return std::nullopt;
}

}  // namespace cb::java::projects
