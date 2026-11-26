// dotnet/decompiler/metadata/universal_assembly_resolver.cpp -- finding referenced assemblies and an assembly's target framework.
#include "dotnet/decompiler/metadata/universal_assembly_resolver.hpp"

#include <algorithm>
#include <cstdlib>
#include <regex>
#include <system_error>

#include "core/fs.hpp"
#include "core/json.hpp"
#include "dotnet/decompiler/metadata/metadata_extensions.hpp"
#include "dotnet/decompiler/srm_extensions.hpp"
#include "dotnet/decompiler/typesystem/known_attributes.hpp"

#include <nlohmann/json.hpp>

namespace cb::dotnet::decompiler::metadata {
namespace fs = std::filesystem;
namespace {

// ---- small helpers -------------------------------------------------------------------------------------------------------

int compare(const AssemblyVersion& a, const AssemblyVersion& b) {   // System.Version.CompareTo (an undefined part is -1)
    const int x[4] = {a.major, a.minor, a.build, a.revision}, y[4] = {b.major, b.minor, b.build, b.revision};
    for (int i = 0; i < 4; ++i)
        if (x[i] != y[i]) return x[i] < y[i] ? -1 : 1;
    return 0;
}

const AssemblyVersion kZeroVersion{0, 0, 0, 0};

std::optional<AssemblyVersion> try_parse_version(std::string_view text) {
    try {
        return AssemblyVersion::parse(text);
    } catch (const UsageError&) {
        return std::nullopt;
    }
}

std::string to_upper_ascii(std::string_view s) {
    std::string r(s);
    for (char& c : r)
        if (c >= 'a' && c <= 'z') c = static_cast<char>(c - 'a' + 'A');
    return r;
}

std::string to_lower_ascii(std::string_view s) {
    std::string r(s);
    for (char& c : r)
        if (c >= 'A' && c <= 'Z') c = static_cast<char>(c - 'A' + 'a');
    return r;
}

std::string_view trim_text(std::string_view s) {
    while (!s.empty() && (s.front() == ' ' || s.front() == '\t' || s.front() == '\r' || s.front() == '\n')) s.remove_prefix(1);
    while (!s.empty() && (s.back() == ' ' || s.back() == '\t' || s.back() == '\r' || s.back() == '\n')) s.remove_suffix(1);
    return s;
}

std::vector<std::string_view> split_text(std::string_view s, char sep) {
    std::vector<std::string_view> parts;
    std::size_t start = 0;
    for (;;) {
        const std::size_t at = s.find(sep, start);
        parts.push_back(s.substr(start, at == std::string_view::npos ? std::string_view::npos : at - start));
        if (at == std::string_view::npos) return parts;
        start = at + 1;
    }
}

// An environment variable as a path (nullopt when unset or empty).
std::optional<fs::path> env_path(const char* name) {
    const std::optional<std::string> v = cb::fs::get_env(name);
    if (!v || v->empty()) return std::nullopt;
    return fs::u8path(*v);
}

bool file_exists(const fs::path& p) {
    std::error_code ec;
    return fs::is_regular_file(p, ec);
}

bool directory_exists(const fs::path& p) {
    std::error_code ec;
    return fs::is_directory(p, ec);
}

std::vector<fs::path> subdirectories(const fs::path& p) {
    std::vector<fs::path> result;
    std::error_code ec;
    for (fs::directory_iterator it(p, ec), end; !ec && it != end; it.increment(ec))
        if (it->is_directory(ec)) result.push_back(it->path());
    return result;
}

bool contains_dll(const fs::path& p) {
    std::error_code ec;
    for (fs::recursive_directory_iterator it(p, fs::directory_options::skip_permission_denied, ec), end; !ec && it != end; it.increment(ec))
        if (it->is_regular_file(ec) && to_lower_ascii(it->path().extension().string()) == ".dll") return true;
    return false;
}

std::string hex_lower(const Bytes& bytes) {
    static constexpr char kHex[] = "0123456789abcdef";
    std::string s;
    for (const std::uint8_t b : bytes) {
        s.push_back(kHex[b >> 4]);
        s.push_back(kHex[b & 15]);
    }
    return s;
}

std::string two_part(const srm::Version& v) { return std::to_string(v.major) + "." + std::to_string(v.minor); }

// DotNetCorePathFinder.ConvertToVersion: a folder named as a version (anything after a '-' dropped).
std::optional<AssemblyVersion> convert_to_version(const fs::path& directory) {
    std::string short_name = directory.filename().string();
    const std::size_t dash = short_name.find('-');
    if (dash != std::string::npos && dash > 0) short_name.erase(dash);
    return try_parse_version(short_name);
}

// GetClosestVersionFolder: the lowest version folder at or above `version` that has any .dll; else the version itself.
std::string get_closest_version_folder(const fs::path& base_path, const AssemblyVersion& version) {
    std::vector<std::pair<AssemblyVersion, fs::path>> found;
    for (const fs::path& d : subdirectories(base_path))
        if (const std::optional<AssemblyVersion> v = convert_to_version(d)) found.emplace_back(*v, d);
    std::stable_sort(found.begin(), found.end(), [](const auto& a, const auto& b) { return compare(a.first, b.first) < 0; });
    for (const auto& [v, d] : found)
        if (compare(v, version) >= 0 && contains_dll(d)) return d.filename().string();
    return version.to_string();
}

// FindClosestVersionDirectory (UniversalAssemblyResolver)
std::string find_closest_version_directory(const fs::path& base_path, const std::optional<AssemblyVersion>& version) {
    std::vector<std::pair<AssemblyVersion, fs::path>> found;
    for (const fs::path& d : subdirectories(base_path))
        if (const std::optional<AssemblyVersion> v = convert_to_version(d)) found.emplace_back(*v, d);
    std::stable_sort(found.begin(), found.end(), [](const auto& a, const auto& b) { return compare(a.first, b.first) > 0; });
    std::optional<std::string> path;
    for (const auto& [v, d] : found)
        if (!path || !version || compare(v, *version) >= 0) path = d.filename().string();
    if (path) return *path;
    if (version) return version->to_string();
    return ".";
}

bool is_zero_or_all_ones(const std::optional<AssemblyVersion>& version) {
    if (!version) return true;
    const AssemblyVersion& v = *version;
    return (v.major == 0 && v.minor == 0 && v.build == 0 && v.revision == 0) || (v.major == 65535 && v.minor == 65535 && v.build == 65535 && v.revision == 65535);
}

bool is_special_version_or_retargetable(const IAssemblyReference& reference) {
    return is_zero_or_all_ones(reference.version()) || reference.is_retargetable();
}

std::optional<fs::path> search_directory(const IAssemblyReference& name, const fs::path& directory) {
    const std::vector<std::string> extensions = name.is_windows_runtime() ? std::vector<std::string>{".winmd", ".dll"} : std::vector<std::string>{".dll", ".exe"};
    for (const std::string& extension : extensions) {
        const fs::path file = directory / fs::u8path(name.name() + extension);
        if (file_exists(file)) return file;
    }
    return std::nullopt;
}

std::optional<fs::path> search_directories(const IAssemblyReference& name, const std::vector<fs::path>& directories) {
    for (const fs::path& directory : directories) {
        if (directory.empty()) continue;
        if (std::optional<fs::path> file = search_directory(name, directory)) return file;
    }
    return std::nullopt;
}

// The directory of the .NET runtime ILSpy would run on: the newest shared Microsoft.NETCore.App of the dotnet on PATH.
std::optional<fs::path> runtime_directory() {
    static const std::optional<fs::path> dir = []() -> std::optional<fs::path> {
        const std::optional<fs::path> dotnet = DotNetCorePathFinder::find_dot_net_exe_directory();
        if (!dotnet) return std::nullopt;
        std::optional<std::pair<AssemblyVersion, fs::path>> best;
        for (const fs::path& d : subdirectories(*dotnet / "shared" / "Microsoft.NETCore.App"))
            if (const std::optional<AssemblyVersion> v = convert_to_version(d); v && (!best || compare(*v, best->first) > 0)) best.emplace(*v, d);
        if (!best) return std::nullopt;
        return best->second;
    }();
    return dir;
}

}  // namespace

// ---- target frameworks ---------------------------------------------------------------------------------------------------

std::pair<TargetFrameworkIdentifier, AssemblyVersion> parse_target_framework(std::string_view target_framework) {
    if (target_framework.empty()) return {TargetFrameworkIdentifier::NETFramework, kZeroVersion};
    const std::vector<std::string_view> tokens = split_text(target_framework, ',');
    TargetFrameworkIdentifier identifier = TargetFrameworkIdentifier::NETFramework;
    const std::string first = to_upper_ascii(trim_text(tokens[0]));
    if (first == ".NETCOREAPP") identifier = TargetFrameworkIdentifier::NETCoreApp;
    else if (first == ".NETSTANDARD") identifier = TargetFrameworkIdentifier::NETStandard;
    else if (first == "SILVERLIGHT") identifier = TargetFrameworkIdentifier::Silverlight;
    std::optional<AssemblyVersion> version;
    for (std::size_t i = 1; i < tokens.size(); ++i) {
        const std::vector<std::string_view> pair = split_text(trim_text(tokens[i]), '=');
        if (pair.size() != 2) continue;
        if (to_upper_ascii(trim_text(pair[0])) != "VERSION") continue;
        std::string_view version_string = pair[1];
        while (!version_string.empty() && (version_string.front() == 'v' || version_string.front() == 'V' || version_string.front() == ' ' || version_string.front() == '\t'))
            version_string.remove_prefix(1);
        version = try_parse_version(version_string);
        if (version) version = AssemblyVersion{version->major, version->minor, version->build < 0 ? 0 : version->build, -1};
        // .NET 5 and later still say ".NETCOREAPP"
        if (version && version->major >= 5 && identifier == TargetFrameworkIdentifier::NETCoreApp) identifier = TargetFrameworkIdentifier::NET;
    }
    return {identifier, version.value_or(kZeroVersion)};
}

namespace {

// System.Runtime / System.Private.CoreLib versions: 4.1.0 .NET Core 1.x, 4.2.0 2.0, 4.2.1 2.1/3.0, 4.2.2 3.1, 5.0.0+ .NET 5+
std::optional<std::string> get_dot_net_core_version(const srm::Version& v) {
    if (v.major == 4 && v.minor == 1 && v.build == 0) return "1.1";
    if (v.major == 4 && v.minor == 2 && v.build == 0) return "2.0";
    if (v.major == 4 && v.minor == 2 && v.build == 1) return "3.0";
    if (v.major == 4 && v.minor == 2 && v.build == 2) return "3.1";
    if (v.major >= 5) return two_part(v);
    return std::nullopt;
}

// DotNetCorePathFinderExtensions.PathPattern, with numbered groups: (type, version) of each alternative
const std::regex& path_pattern() {
    static const std::regex r(
        R"((?:Reference Assemblies[/\\]Microsoft[/\\]Framework[/\\](.NETFramework)[/\\]v([^/\\]+)[/\\]))"
        R"(|(?:(Microsoft\.NET)[/\\]assembly[/\\]GAC_(?:MSIL|32|64)[/\\]))"
        R"(|(?:(Microsoft\.NET)[/\\]Framework(?:64)?[/\\]([^/\\]+)[/\\]))"
        R"(|(?:NuGetFallbackFolder[/\\]([^/\\]+)\\([^/\\]+)(?:[/\\].*)?[/\\]ref[/\\]))"
        R"(|(?:shared[/\\]([^/\\]+)\\([^/\\]+)(?:[/\\].*)?[/\\]))"
        R"(|(?:packs[/\\]([^/\\]+)\\([^/\\]+)\\ref(?:[/\\].*)?[/\\]))",
        std::regex::icase | std::regex::ECMAScript | std::regex::optimize);
    return r;
}

const std::regex& ref_path_pattern() {
    static const std::regex r(R"((Reference Assemblies[/\\]Microsoft[/\\]Framework[/\\](.NETFramework)[/\\]v([^/\\]+)[/\\]))"
                              R"(|(NuGetFallbackFolder[/\\]([^/\\]+)\\([^/\\]+)([/\\].*)?[/\\]ref[/\\]))"
                              R"(|(packs[/\\]([^/\\]+)\\([^/\\]+)\\ref([/\\].*)?[/\\]))",
                              std::regex::icase | std::regex::ECMAScript | std::regex::optimize);
    return r;
}

std::string version_prefix_3(std::string version) {
    while (!version.empty() && version.front() == 'v') version.erase(0, 1);
    return version.substr(0, std::min<std::size_t>(3, version.size()));
}

bool contains_ignore_case(std::string_view haystack, std::string_view needle) { return to_lower_ascii(haystack).find(to_lower_ascii(needle)) != std::string::npos; }

}  // namespace

std::string detect_target_framework_id(const MetadataFile& assembly) { return detect_target_framework_id(assembly.metadata(), assembly.file_name().string()); }

std::string detect_target_framework_id(const srm::MetadataReader& metadata, const std::optional<std::string>& assembly_path) {
    static const std::string kTargetFrameworkAttributeName = "System.Runtime.Versioning.TargetFrameworkAttribute";
    for (const srm::CustomAttributeHandle h : metadata.get_custom_attributes(srm::AssemblyDefinitionHandle(1))) {
        try {
            const srm::CustomAttribute attribute = metadata.get_custom_attribute(h);
            if (get_full_type_name(get_attribute_type(attribute, metadata), metadata).reflection_name() != kTargetFrameworkAttributeName) continue;
            srm::BlobReader blob_reader = metadata.get_blob_reader(attribute.value());
            if (blob_reader.read_uint16() == 0x0001) {
                std::string value = blob_reader.read_serialized_string().value_or(std::string());
                std::erase(value, ' ');
                return value;
            }
        } catch (const srm::BadImageFormatError&) {
            // malformed attributes are ignored
        }
    }
    if (metadata.is_assembly()) {
        const srm::AssemblyDefinition assembly_definition = metadata.get_assembly_definition();
        const std::string_view name = metadata.get_string(assembly_definition.name());
        if (name == "mscorlib") return ".NETFramework,Version=v" + two_part(assembly_definition.version());
        if (name == "netstandard") return ".NETStandard,Version=v" + two_part(assembly_definition.version());
        if (name == "System.Runtime" || name == "System.Private.CoreLib")
            if (const std::optional<std::string> version = get_dot_net_core_version(assembly_definition.version())) return ".NETCoreApp,Version=v" + *version;
    }
    for (const srm::AssemblyReferenceHandle h : metadata.assembly_references()) {
        try {
            const srm::AssemblyReference r = metadata.get_assembly_reference(h);
            if (r.public_key_or_token().is_nil()) continue;
            const std::string_view name = metadata.get_string(r.name());
            if (name == "mscorlib") return ".NETFramework,Version=v" + two_part(r.version());
            if (name == "System.Runtime" || name == "System.Private.CoreLib")
                if (const std::optional<std::string> version = get_dot_net_core_version(r.version())) return ".NETCoreApp,Version=v" + *version;
        } catch (const srm::BadImageFormatError&) {
            // malformed references are ignored
        }
    }
    // netstandard on its own: .NET Core and Framework assemblies can reference it too
    for (const srm::AssemblyReferenceHandle h : metadata.assembly_references()) {
        try {
            const srm::AssemblyReference r = metadata.get_assembly_reference(h);
            if (r.public_key_or_token().is_nil() || metadata.get_string(r.name()) != "netstandard") continue;
            return ".NETStandard,Version=v" + two_part(r.version());
        } catch (const srm::BadImageFormatError&) {
        }
    }
    // the path, as a fallback (reference assemblies)
    if (assembly_path) {
        std::smatch m;
        if (std::regex_search(*assembly_path, m, path_pattern())) {
            static constexpr int kTypeGroups[] = {1, 3, 4, 6, 8, 10};
            static constexpr int kVersionGroups[] = {2, 0, 5, 7, 9, 11};
            std::string type, version;
            for (int i = 0; i < 6; ++i)
                if (m[kTypeGroups[i]].matched) {
                    type = m[kTypeGroups[i]].str();
                    if (kVersionGroups[i] != 0) version = m[kVersionGroups[i]].str();
                    break;
                }
            if (version.empty()) version = metadata.metadata_version();
            if (version.empty()) version = "4.0";
            while (!version.empty() && version.front() == 'v') version.erase(0, 1);
            if (type == "Microsoft.NET" || type == ".NETFramework") return ".NETFramework,Version=v" + version.substr(0, std::min<std::size_t>(3, version.size()));
            if (contains_ignore_case(type, "netcore")) return ".NETCoreApp,Version=v" + version;
            if (contains_ignore_case(type, "netstandard")) return ".NETStandard,Version=v" + version;
        } else {
            std::string version = metadata.metadata_version();
            if (version.empty()) version = "4.0";
            return ".NETFramework,Version=v" + version_prefix_3(version);
        }
    }
    return std::string();
}

bool is_reference_assembly(const MetadataFile& assembly) {
    const srm::MetadataReader& metadata = assembly.metadata();
    if (typesystem::has_known_attribute(metadata.get_custom_attributes(srm::AssemblyDefinitionHandle(1)), metadata, typesystem::KnownAttribute::ReferenceAssembly))
        return true;
    const std::string path = assembly.file_name().string();
    return std::regex_search(path, ref_path_pattern());
}

std::string detect_runtime_pack(const MetadataFile& assembly) {
    const srm::MetadataReader& metadata = assembly.metadata();
    for (const srm::AssemblyReferenceHandle r : metadata.assembly_references()) {
        const srm::AssemblyReference reference = metadata.get_assembly_reference(r);
        if (reference.public_key_or_token().is_nil()) continue;
        const std::string_view name = metadata.get_string(reference.name());
        if (name == "WindowsBase" || name == "PresentationFramework" || name == "PresentationCore") return "Microsoft.WindowsDesktop.App";
        // (upstream's TODO: ASP.NET Core)
    }
    return "Microsoft.NETCore.App";
}

// ---- DotNetCorePathFinder ------------------------------------------------------------------------------------------------

namespace {
const std::vector<std::string>& runtime_packs() {
    static const std::vector<std::string> packs = {"Microsoft.NETCore.App", "Microsoft.WindowsDesktop.App", "Microsoft.AspNetCore.App", "Microsoft.AspNetCore.All"};
    return packs;
}
}  // namespace

DotNetCorePathFinder::DotNetCorePathFinder(TargetFrameworkIdentifier target_framework, AssemblyVersion target_framework_version, std::string preferred_runtime_pack)
    : target_framework_version_(target_framework_version), dotnet_base_path_(find_dot_net_exe_directory()), preferred_runtime_pack_(std::move(preferred_runtime_pack)) {
    // .NET Standard 2.1 is implemented by .NET Core 3.0 and later
    if (target_framework == TargetFrameworkIdentifier::NETStandard && target_framework_version.major == 2 && target_framework_version.minor == 1)
        target_framework_version_ = AssemblyVersion{3, 0, 0, -1};
}

DotNetCorePathFinder::DotNetCorePathFinder(const fs::path& parent_assembly_file_name, const std::string& target_framework_id_string,
                                           std::string preferred_runtime_pack, TargetFrameworkIdentifier target_framework, AssemblyVersion target_framework_version)
    : DotNetCorePathFinder(target_framework, target_framework_version, std::move(preferred_runtime_pack)) {
    const std::string assembly_name = parent_assembly_file_name.stem().string();
    const fs::path base_path = parent_assembly_file_name.parent_path();
    search_paths_.push_back(base_path);
    const fs::path deps_json_file_name = base_path / (assembly_name + ".deps.json");
    if (!file_exists(deps_json_file_name)) return;   // (upstream notes "could not be found" in its load info)
    packages_ = load_package_infos(deps_json_file_name, target_framework_id_string);
    std::vector<fs::path> lookup_paths;
    if (std::optional<fs::path> p = env_path("NUGET_PACKAGES")) lookup_paths.push_back(*p);
    if (std::optional<fs::path> home = env_path("USERPROFILE")) lookup_paths.push_back(*home / ".nuget" / "packages");
    for (const fs::path& path : lookup_paths) {
        for (const PackageInfo& p : packages_) {
            for (const std::string& item : p.runtime_components) {
                const fs::path item_path = fs::u8path(item).parent_path();
                const fs::path full_path = fs::u8path(to_lower_ascii((path / fs::u8path(p.name) / fs::u8path(p.version) / item_path).u8string()));
                if (directory_exists(full_path)) package_base_paths_.push_back(full_path);
            }
        }
    }
}

void DotNetCorePathFinder::remove_search_directory(const fs::path& path) {
    const auto it = std::find(search_paths_.begin(), search_paths_.end(), path);
    if (it != search_paths_.end()) search_paths_.erase(it);
}

std::optional<fs::path> DotNetCorePathFinder::try_resolve_dot_net_core(const IAssemblyReference& name) const {
    const auto try_dir = [&](const fs::path& base_path) -> std::optional<fs::path> {
        const fs::path dll = base_path / fs::u8path(name.name() + ".dll");
        if (file_exists(dll)) return dll;
        const fs::path exe = base_path / fs::u8path(name.name() + ".exe");
        if (file_exists(exe)) return exe;
        return std::nullopt;
    };
    for (const fs::path& base_path : search_paths_)
        if (std::optional<fs::path> f = try_dir(base_path)) return f;
    for (const fs::path& base_path : package_base_paths_)
        if (std::optional<fs::path> f = try_dir(base_path)) return f;
    return try_resolve_dot_net_core_shared(name);
}

fs::path DotNetCorePathFinder::get_reference_assembly_path(std::string_view target_framework) const {
    const auto [tfi, version] = parse_target_framework(target_framework);
    std::string identifier, identifier_ext;
    switch (tfi) {
    case TargetFrameworkIdentifier::NETCoreApp:
        identifier = "Microsoft.NETCore.App";
        identifier_ext = "netcoreapp" + std::to_string(version.major) + "." + std::to_string(version.minor);
        break;
    case TargetFrameworkIdentifier::NETStandard:
        identifier = "NETStandard.Library";
        identifier_ext = "netstandard" + std::to_string(version.major) + "." + std::to_string(version.minor);
        break;
    case TargetFrameworkIdentifier::NET:
        identifier = "Microsoft.NETCore.App";
        identifier_ext = "net" + std::to_string(version.major) + "." + std::to_string(version.minor);
        break;
    default: throw UsageError("get_reference_assembly_path: a framework without reference packs");
    }
    const fs::path base_path = dotnet_base_path_.value_or(fs::path()) / "packs" / (identifier + ".Ref");
    return base_path / get_closest_version_folder(base_path, version) / "ref" / identifier_ext;
}

std::vector<DotNetCorePathFinder::PackageInfo> DotNetCorePathFinder::load_package_infos(const fs::path& deps_json_file_name, const std::string& target_framework) {
    const Json dependencies = json::load(deps_json_file_name);
    std::vector<PackageInfo> result;
    if (!dependencies.is_object() || !dependencies.contains("targets") || !dependencies.contains("libraries")) return result;
    const Json& targets = dependencies["targets"];
    if (!targets.is_object() || !targets.contains(target_framework) || !targets[target_framework].is_object()) return result;
    const Json& runtime_infos = targets[target_framework];
    const Json& libraries = dependencies["libraries"];
    if (!libraries.is_object()) return result;
    const auto as_string = [](const Json& o, const char* key) -> std::string {
        if (!o.is_object() || !o.contains(key) || !o[key].is_string()) return {};
        return o[key].get<std::string>();
    };
    for (auto it = libraries.begin(); it != libraries.end(); ++it) {
        PackageInfo info;
        const std::vector<std::string_view> parts = split_text(it.key(), '/');
        info.name = std::string(parts[0]);
        info.version = parts.size() > 1 ? std::string(parts[1]) : std::string("<UNKNOWN>");
        info.type = as_string(it.value(), "type");
        info.path = as_string(it.value(), "path");
        if (runtime_infos.contains(it.key())) {
            const Json& library_runtime = runtime_infos[it.key()];
            if (library_runtime.is_object() && library_runtime.contains("runtime") && library_runtime["runtime"].is_object())
                for (auto c = library_runtime["runtime"].begin(); c != library_runtime["runtime"].end(); ++c) info.runtime_components.push_back(c.key());
        }
        result.push_back(std::move(info));
    }
    return result;
}

std::optional<fs::path> DotNetCorePathFinder::try_resolve_dot_net_core_shared(const IAssemblyReference& name, std::string* runtime_pack) const {
    if (!dotnet_base_path_) return std::nullopt;
    std::vector<std::string> packs;
    if (!preferred_runtime_pack_.empty()) packs.push_back(preferred_runtime_pack_);
    packs.insert(packs.end(), runtime_packs().begin(), runtime_packs().end());
    for (const std::string& pack : packs) {
        const fs::path base_path = *dotnet_base_path_ / "shared" / pack;
        if (!directory_exists(base_path)) continue;
        const std::string closest_version = get_closest_version_folder(base_path, target_framework_version_);
        for (const char* extension : {".dll", ".exe"}) {
            const fs::path file = base_path / closest_version / fs::u8path(name.name() + extension);
            if (file_exists(file)) {
                if (runtime_pack != nullptr) *runtime_pack = pack;
                return file;
            }
        }
    }
    return std::nullopt;
}

std::optional<fs::path> DotNetCorePathFinder::find_dot_net_exe_directory() {
    const std::optional<std::string> path = cb::fs::get_env("PATH");
    if (!path) return std::nullopt;
#ifdef _WIN32
    constexpr char kSeparator = ';';
    const char* exe_name = "dotnet.exe";
#else
    constexpr char kSeparator = ':';
    const char* exe_name = "dotnet";
#endif
    for (const std::string_view item : split_text(*path, kSeparator)) {
        if (item.empty()) continue;
        const fs::path file_name = fs::u8path(std::string(item)) / exe_name;
        if (file_exists(file_name)) return file_name.parent_path();
    }
    return std::nullopt;
}

// ---- UniversalAssemblyResolver -------------------------------------------------------------------------------------------

UniversalAssemblyResolver::UniversalAssemblyResolver(std::optional<fs::path> main_assembly_file_name, bool throw_on_error, std::optional<std::string> target_framework,
                                                     std::optional<std::string> runtime_pack)
    : throw_on_error_(throw_on_error), main_assembly_file_name_(std::move(main_assembly_file_name)), target_framework_(target_framework.value_or(std::string())),
      runtime_pack_(runtime_pack.value_or("Microsoft.NETCore.App")) {
    std::tie(target_framework_identifier_, target_framework_version_) = parse_target_framework(target_framework_);
    if (main_assembly_file_name_) {
        base_directory_ = main_assembly_file_name_->parent_path();
        if (base_directory_->empty()) base_directory_ = fs::current_path();
        add_search_directory(*base_directory_);
    }
}

void UniversalAssemblyResolver::add_search_directory(const fs::path& directory) {
    directories_.push_back(directory);
    if (dot_net_core_path_finder_) dot_net_core_path_finder_->add_search_directory(directory);
}

void UniversalAssemblyResolver::remove_search_directory(const fs::path& directory) {
    const auto it = std::find(directories_.begin(), directories_.end(), directory);
    if (it != directories_.end()) directories_.erase(it);
    if (dot_net_core_path_finder_) dot_net_core_path_finder_->remove_search_directory(directory);
}

DotNetCorePathFinder& UniversalAssemblyResolver::dot_net_core_path_finder() {
    if (!dot_net_core_path_finder_) {
        if (!main_assembly_file_name_)
            dot_net_core_path_finder_ = std::make_unique<DotNetCorePathFinder>(target_framework_identifier_, target_framework_version_, runtime_pack_);
        else
            dot_net_core_path_finder_ = std::make_unique<DotNetCorePathFinder>(*main_assembly_file_name_, target_framework_, runtime_pack_,
                                                                               target_framework_identifier_, target_framework_version_);
        for (const fs::path& directory : directories_) dot_net_core_path_finder_->add_search_directory(directory);
    }
    return *dot_net_core_path_finder_;
}

std::shared_ptr<MetadataFile> UniversalAssemblyResolver::resolve(const IAssemblyReference& name) {
    return create_pe_file_from_file_name(find_assembly_file(name), name.full_name());
}

std::shared_ptr<MetadataFile> UniversalAssemblyResolver::resolve_module(const MetadataFile& main_module, std::string_view module_name) {
    const fs::path module_file_name = main_module.file_name().parent_path() / fs::u8path(std::string(module_name));
    return create_pe_file_from_file_name(module_file_name, std::string(module_name));
}

std::shared_ptr<MetadataFile> UniversalAssemblyResolver::create_pe_file_from_file_name(const std::optional<fs::path>& file_name, const std::string& what) {
    if (!file_name) {
        if (throw_on_error_) throw ResolutionError("Failed to resolve assembly: '" + what + "'");
        return nullptr;
    }
    try {
        return PEFile::open(*file_name);
    } catch (const FormatError& ex) {   // (BadImageFormatException)
        if (throw_on_error_) throw ResolutionError("Failed to resolve assembly: '" + what + "': " + ex.what());
    } catch (const IoError& ex) {
        if (throw_on_error_) throw ResolutionError("Failed to resolve assembly: '" + what + "': " + ex.what());
    }
    return nullptr;
}

bool UniversalAssemblyResolver::is_shared_assembly(const IAssemblyReference& reference, std::string* runtime_pack) {
    return dot_net_core_path_finder().try_resolve_dot_net_core_shared(reference, runtime_pack).has_value();
}

std::optional<fs::path> UniversalAssemblyResolver::find_assembly_file(const IAssemblyReference& name) { return find_assembly_file_core(name); }

std::optional<fs::path> UniversalAssemblyResolver::find_assembly_file_core(const IAssemblyReference& name) {
    if (name.is_windows_runtime()) return find_windows_metadata_file(name);
    switch (target_framework_identifier_) {
    case TargetFrameworkIdentifier::NET:
    case TargetFrameworkIdentifier::NETCoreApp:
    case TargetFrameworkIdentifier::NETStandard:
        if (!is_zero_or_all_ones(target_framework_version_))
            if (std::optional<fs::path> file = dot_net_core_path_finder().try_resolve_dot_net_core(name)) return file;
        break;
    case TargetFrameworkIdentifier::Silverlight:
        if (!is_zero_or_all_ones(target_framework_version_))
            if (std::optional<fs::path> file = resolve_silverlight(name, target_framework_version_)) return file;
        break;
    default: break;
    }
    return resolve_internal(name);
}

std::optional<fs::path> UniversalAssemblyResolver::find_windows_metadata_file(const IAssemblyReference& name) const {
#ifndef _WIN32
    return std::nullopt;   // (only on Windows)
#else
    const auto in_system_directory = [&]() -> std::optional<fs::path> {
        const std::optional<fs::path> windir = env_path("windir");
        if (!windir) return std::nullopt;
        const fs::path file = *windir / "System32" / "WinMetadata" / fs::u8path(name.name() + ".winmd");
        if (file_exists(file)) return file;
        return std::nullopt;
    };
    const std::optional<fs::path> program_files_x86 = env_path("ProgramFiles(x86)");
    if (!program_files_x86) return in_system_directory();
    fs::path base_path = *program_files_x86 / "Windows Kits" / "10" / "References";
    if (!directory_exists(base_path)) return in_system_directory();
    const std::vector<fs::path> versions = subdirectories(base_path);
    if (versions.empty()) return in_system_directory();
    base_path = versions.back() / fs::u8path(name.name());
    if (!directory_exists(base_path)) return in_system_directory();
    base_path = base_path / fs::u8path(find_closest_version_directory(base_path, name.version()));
    if (!directory_exists(base_path)) return in_system_directory();
    const fs::path file = base_path / fs::u8path(name.name() + ".winmd");
    if (!file_exists(file)) return in_system_directory();
    return file;
#endif
}

std::optional<fs::path> UniversalAssemblyResolver::resolve_silverlight(const IAssemblyReference& name, const AssemblyVersion& version) const {
    std::vector<fs::path> target_framework_search_paths;
    if (std::optional<fs::path> p = env_path("ProgramFiles")) target_framework_search_paths.push_back(*p / "Microsoft Silverlight");
    if (std::optional<fs::path> p = env_path("ProgramFiles(x86)")) target_framework_search_paths.push_back(*p / "Microsoft Silverlight");
    for (const fs::path& base_directory : target_framework_search_paths) {
        if (!directory_exists(base_directory)) continue;
        const fs::path version_directory = base_directory / fs::u8path(find_closest_version_directory(base_directory, version));
        if (std::optional<fs::path> file = search_directory(name, version_directory)) return file;
    }
    return std::nullopt;
}

std::optional<fs::path> UniversalAssemblyResolver::resolve_internal(const IAssemblyReference& name) {
    if (std::optional<fs::path> assembly = search_directories(name, directories_)) return assembly;
    // (ILSpy on .NET: the 64-bit .NET Framework 4 directory, else the runtime's own)
    std::vector<fs::path> framework_dirs;
    if (const std::optional<fs::path> windir = env_path("windir")) framework_dirs.push_back(*windir / "Microsoft.NET" / "Framework64" / "v4.0.30319");
    else if (const std::optional<fs::path> runtime = runtime_directory()) framework_dirs.push_back(*runtime);
    if (is_special_version_or_retargetable(name))
        if (std::optional<fs::path> assembly = search_directories(name, framework_dirs)) return assembly;
    if (name.name() == "mscorlib")
        if (std::optional<fs::path> assembly = get_corlib(name)) return assembly;
    if (std::optional<fs::path> assembly = get_assembly_in_gac(name)) return assembly;
    // an assembly of a framework before 4.0 falls back to the 4.0 one; Microsoft.Build.Framework 15.0 does not
    if (!name.version() || compare(*name.version(), AssemblyVersion{4, 0, 0, 0}) <= 0)
        if (std::optional<fs::path> assembly = search_directories(name, framework_dirs)) return assembly;
    // hosts without a .NET Framework have no GAC: the runtime's shared framework is the last resort, whatever the version
    if (const std::optional<fs::path> runtime = runtime_directory())
        if (std::optional<fs::path> assembly = search_directory(name, *runtime)) return assembly;
    if (throw_on_error_) throw ResolutionError("Failed to resolve assembly: '" + name.full_name() + "'");
    return std::nullopt;
}

std::optional<fs::path> UniversalAssemblyResolver::get_corlib(const IAssemblyReference& reference) const {
    // (ILSpy on .NET never takes its own corlib for mscorlib)
    const std::optional<Bytes> token = reference.public_key_token();
    if (!token) return std::nullopt;
    const std::optional<AssemblyVersion> version = reference.version();
    if (!version) return std::nullopt;
    const std::optional<fs::path> path = get_mscorlib_base_path(*version, hex_lower(*token));
    if (!path) return std::nullopt;
    const fs::path file = *path / "mscorlib.dll";
    if (file_exists(file)) return file;
    return std::nullopt;
}

std::optional<fs::path> UniversalAssemblyResolver::get_mscorlib_base_path(const AssemblyVersion& version, const std::string& public_key_token) const {
    const auto not_supported = [&]() -> std::optional<fs::path> {
        if (throw_on_error_) throw ResolutionError("Version not supported: " + version.to_string());
        return std::nullopt;
    };
    if (public_key_token == "969db8053d3322ac") {
        // the .NET Compact Framework
        const std::optional<fs::path> program_files = env_path("ProgramFiles(x86)") ? env_path("ProgramFiles(x86)") : env_path("ProgramFiles");
        if (program_files) {
            const fs::path cf_base_path = *program_files / "Microsoft.NET" / "SDK" / "CompactFramework" /
                                          ("v" + std::to_string(version.major) + "." + std::to_string(version.minor)) / "WindowsCE";
            if (directory_exists(cf_base_path)) return cf_base_path;
        }
    } else {
        const std::optional<fs::path> windir = env_path("windir");
        const fs::path root_path = windir.value_or(fs::path()) / "Microsoft.NET";
        std::optional<std::string> folder;
        switch (version.major) {
        case 1: folder = (version.revision >> 16) == 3300 ? "v1.0.3705" : "v1.1.4322"; break;   // (Version.MajorRevision)
        case 2: folder = "v2.0.50727"; break;
        case 4: folder = "v4.0.30319"; break;
        default: return not_supported();
        }
        for (const char* framework : {"Framework", "Framework64"}) {
            const fs::path base_path = root_path / framework / *folder;
            if (directory_exists(base_path)) return base_path;
        }
    }
    return not_supported();
}

std::vector<fs::path> UniversalAssemblyResolver::get_gac_paths() {
    std::vector<fs::path> paths;
    const std::optional<fs::path> windir = env_path("windir");
    if (!windir) return paths;
    paths.push_back(*windir / "assembly");
    paths.push_back(*windir / "Microsoft.NET" / "assembly");
    return paths;
}

std::optional<fs::path> UniversalAssemblyResolver::get_assembly_in_gac(const IAssemblyReference& reference) {
    const std::optional<Bytes> token = reference.public_key_token();
    if (!token || token->empty()) return std::nullopt;
    static const std::vector<fs::path> gac_paths = get_gac_paths();
    static constexpr const char* kGacs[] = {"GAC_MSIL", "GAC_32", "GAC_64", "GAC"};
    static constexpr const char* kPrefixes[] = {"", "v4.0_"};   // (upstream picks the prefix by the GAC path's index)
    const std::optional<AssemblyVersion> version = reference.version();
    for (std::size_t i = 0; i < gac_paths.size() && i < 2; ++i) {
        for (const char* gac_name : kGacs) {
            const std::string gac_folder = std::string(kPrefixes[i]) + (version ? version->to_string() : std::string()) + "__" + hex_lower(*token);
            const fs::path file = gac_paths[i] / gac_name / fs::u8path(reference.name()) / gac_folder / fs::u8path(reference.name() + ".dll");
            if (file_exists(file)) return file;
        }
    }
    return std::nullopt;
}

}  // namespace cb::dotnet::decompiler::metadata
