#include "dotnet/decompiler/documentation/xml_doc_loader.hpp"

#include <algorithm>
#include <cstdlib>
#include <map>
#include <mutex>
#include <system_error>
#include <tuple>

#ifdef _WIN32
#include <windows.h>
#endif

#include "dotnet/decompiler/metadata/universal_assembly_resolver.hpp"
#include "dotnet/decompiler/util/dotnet_path.hpp"
#include "dotnet/decompiler/util/platform.hpp"

namespace cb::dotnet::decompiler::documentation {

namespace {

namespace stdfs = std::filesystem;
namespace path = util::path;

// An environment variable's value ("" when it is not set).
std::string get_environment_variable(const char* name) {
    std::string result;
#ifdef _WIN32
    char* value = nullptr;
    std::size_t length = 0;
    if (_dupenv_s(&value, &length, name) == 0 && value != nullptr) result = value;
    std::free(value);
#else
    if (const char* value = std::getenv(name)) result = value;
#endif
    return result;
}

struct Culture {
    std::string name;             // "en-US"
    std::string two_letter_name;  // "en"
};

// The user interface culture: what CultureInfo.CurrentUICulture is for ilspycmd on this machine.
Culture current_ui_culture() {
    Culture culture;
#ifdef _WIN32
    wchar_t buffer[LOCALE_NAME_MAX_LENGTH] = {};
    if (LCIDToLocaleName(MAKELCID(GetUserDefaultUILanguage(), SORT_DEFAULT), buffer, LOCALE_NAME_MAX_LENGTH, 0) > 0) {
        for (const wchar_t* p = buffer; *p != 0; ++p) culture.name.push_back(*p < 0x80 ? static_cast<char>(*p) : '?');
    }
#else
    std::string lang = get_environment_variable("LANG");
    const std::size_t cut = lang.find_first_of(".@");
    if (cut != std::string::npos) lang = lang.substr(0, cut);
    for (char& c : lang)
        if (c == '_') c = '-';
    if (lang != "C" && lang != "POSIX") culture.name = lang;
#endif
    const std::size_t dash = culture.name.find('-');
    culture.two_letter_name = dash == std::string::npos ? culture.name : culture.name.substr(0, dash);
    return culture;
}

bool file_exists(const std::string& file_name) {
    std::error_code ec;
    return stdfs::is_regular_file(path::from_text(file_name), ec);
}

std::string get_localized_name(const std::string& file_name, const std::string& language) {
    return path::combine(path::get_directory_name(file_name).value_or(std::string()), language, path::get_file_name(file_name));
}

// the .NET Framework's places for documentation
std::string reference_assemblies_path() {
    std::string program_files = get_environment_variable("ProgramFiles(x86)");
    if (program_files.empty()) program_files = get_environment_variable("ProgramFiles");
    return path::combine(program_files, "Reference Assemblies\\Microsoft\\Framework");
}

std::string framework_path() { return path::combine(get_environment_variable("windir"), "Microsoft.NET\\Framework"); }

std::optional<std::string> find_xml_documentation(const std::string& assembly_file_name, metadata::TargetRuntime runtime) {
    const std::string reference_assemblies = reference_assemblies_path();
    const std::string framework = framework_path();
    auto look = [&](const std::string& directory) { return xml_doc_loader::lookup_localized_xml_doc(path::combine(directory, assembly_file_name)); };
    auto first_of = [&](const std::vector<std::string>& directories) -> std::optional<std::string> {
        for (const std::string& directory : directories)
            if (auto found = look(directory)) return found;
        return std::nullopt;
    };
    switch (runtime) {
    case metadata::TargetRuntime::Net_1_0: return look(path::combine(framework, "v1.0.3705"));
    case metadata::TargetRuntime::Net_1_1: return look(path::combine(framework, "v1.1.4322"));
    case metadata::TargetRuntime::Net_2_0:
        return first_of({path::combine(reference_assemblies, "v3.5"), path::combine(reference_assemblies, ".NETFramework\\v3.5\\Profile\\Client"),
                         path::combine(reference_assemblies, "v3.0"), path::combine(framework, "v2.0.50727")});
    case metadata::TargetRuntime::Net_4_0:
    default: {
        std::vector<std::string> directories;
        for (const char* version : {"v4.8.1", "v4.8", "v4.7.2", "v4.7.1", "v4.7", "v4.6.2", "v4.6.1", "v4.6", "v4.5.2", "v4.5.1", "v4.5", "v4.0"})
            directories.push_back(path::combine(reference_assemblies, std::string(".NETFramework\\") + version));
        directories.push_back(path::combine(framework, "v4.0.30319"));
        return first_of(directories);
    }
    }
}

// ---- version numbers of the reference packs ---------------------------------------------------------------------------------------

using Version3 = std::tuple<int, int, int>;

Version3 parse_version_prefix(const std::string& version) {
    if (version.empty()) return {-1, -1, -1};
    const std::size_t dash = version.find('-');
    const std::string core = dash != std::string::npos ? version.substr(0, dash) : version;
    std::vector<std::string> parts;
    std::size_t start = 0;
    for (std::size_t i = 0; i <= core.size(); ++i) {
        if (i == core.size() || core[i] == '.') {
            parts.push_back(core.substr(start, i - start));
            start = i + 1;
        }
    }
    auto part = [&](std::size_t i) {
        if (i >= parts.size() || parts[i].empty() || parts[i].size() > 9) return 0;
        int n = 0;
        for (char c : parts[i]) {
            if (c < '0' || c > '9') return 0;
            n = n * 10 + (c - '0');
        }
        return n;
    };
    return {part(0), part(1), part(2)};
}

std::pair<int, int> parse_tfm(const std::string& tfm) {
    if (tfm.size() < 3 || util::compare_ordinal_ignore_case(std::string_view(tfm).substr(0, 3), "net") != 0) return {-1, -1};
    const std::string rest = tfm.substr(3);
    const std::size_t dot = rest.find('.');
    if (dot == std::string::npos) return {-1, -1};
    auto parse_int = [](const std::string& s, int& value) {
        if (s.empty() || s.size() > 9) return false;
        value = 0;
        for (char c : s) {
            if (c < '0' || c > '9') return false;
            value = value * 10 + (c - '0');
        }
        return true;
    };
    int major, minor;
    if (!parse_int(rest.substr(0, dot), major)) return {-1, -1};
    if (!parse_int(rest.substr(dot + 1), minor)) return {-1, -1};
    return {major, minor};
}

// The names of the subdirectories of a directory, in the order the file system lists them (nothing for one that cannot be listed).
std::vector<stdfs::path> get_directories(const stdfs::path& directory) {
    std::vector<stdfs::path> result;
    std::error_code ec;
    for (stdfs::directory_iterator it(directory, ec), end; !ec && it != end; it.increment(ec)) {
        std::error_code type_ec;
        if (it->is_directory(type_ec)) result.push_back(it->path());
    }
    return result;
}

std::string file_name_of(const stdfs::path& p) { return path::get_file_name(path::native_text(p)); }

// Picks the ref-pack version directory most appropriate for the running runtime: an exact version match wins; otherwise the newest pack sharing the
// runtime's major.minor, then the newest pack of the same major, then the newest pack present. Pre-release suffixes are ignored when comparing.
std::optional<stdfs::path> select_ref_pack_version_directory(const stdfs::path& pack_root, const std::string& runtime_version) {
    const stdfs::path exact = pack_root / path::from_text(runtime_version);
    std::error_code ec;
    if (stdfs::is_directory(exact, ec)) return exact;
    const Version3 rt = parse_version_prefix(runtime_version);
    std::optional<stdfs::path> best;
    Version3 best_ver{-1, -1, -1};
    int best_rank = -1;
    for (const stdfs::path& dir : get_directories(pack_root)) {
        const Version3 v = parse_version_prefix(file_name_of(dir));
        const int rank = (std::get<0>(v) == std::get<0>(rt) && std::get<1>(v) == std::get<1>(rt)) ? 2 : (std::get<0>(v) == std::get<0>(rt) ? 1 : 0);
        if (rank > best_rank || (rank == best_rank && v > best_ver)) {
            best = dir;
            best_ver = v;
            best_rank = rank;
        }
    }
    return best;
}

// Aggregates several XmlDocumentationProvider instances behind one XmlDocumentationProvider surface. Used because in modern .NET an entity's
// metadata-token-bearing assembly (System.Private.CoreLib.dll) is different from the assembly whose XML contains its docs (System.Runtime.xml): we
// have to probe every ref-pack XML for the same id string until one matches. First non-empty answer wins; id strings are unique across the pack.
class AggregatingXmlDocumentationProvider final : public XmlDocumentationProvider {
public:
    explicit AggregatingXmlDocumentationProvider(std::vector<std::shared_ptr<XmlDocumentationProvider>> providers) : providers_(std::move(providers)) {}

    using XmlDocumentationProvider::get_documentation;
    std::optional<std::string> get_documentation(std::string_view key) const override {
        for (const auto& p : providers_) {
            std::optional<std::string> doc = p->get_documentation(key);
            if (doc && !doc->empty()) return doc;
        }
        return std::nullopt;
    }

private:
    std::vector<std::shared_ptr<XmlDocumentationProvider>> providers_;
};

std::vector<stdfs::path> safe_get_xml_files(const stdfs::path& directory) {
    std::vector<stdfs::path> result;
    std::error_code ec;
    for (stdfs::directory_iterator it(directory, ec), end; !ec && it != end; it.increment(ec)) {
        std::error_code type_ec;
        if (!it->is_regular_file(type_ec)) continue;
        const std::string extension = path::get_extension(file_name_of(it->path()));
        if (util::compare_ordinal_ignore_case(extension, ".xml") == 0) result.push_back(it->path());
    }
    return result;
}

std::shared_ptr<XmlDocumentationProvider> try_load_modern_ref_pack_documentation(const std::string& assembly_file_name) {
    const std::optional<stdfs::path> ref_pack_tfm_dir = xml_doc_loader::resolve_modern_ref_pack_tfm_directory(assembly_file_name);
    if (!ref_pack_tfm_dir) return nullptr;
    std::vector<std::shared_ptr<XmlDocumentationProvider>> providers;
    for (const stdfs::path& xml_path : safe_get_xml_files(*ref_pack_tfm_dir)) {
        try {
            providers.push_back(std::make_shared<XmlDocumentationProvider>(xml_path));
        } catch (...) {
            // One bad XML doesn't disable the rest - skip and continue.
            continue;
        }
    }
    if (providers.empty()) return nullptr;
    return std::make_shared<AggregatingXmlDocumentationProvider>(std::move(providers));
}

std::shared_ptr<XmlDocumentationProvider> load_mscorlib_documentation() {
    std::optional<std::string> xml_doc_file = find_xml_documentation("mscorlib.dll", metadata::TargetRuntime::Net_4_0);
    if (!xml_doc_file) xml_doc_file = find_xml_documentation("mscorlib.dll", metadata::TargetRuntime::Net_2_0);
    if (xml_doc_file) return std::make_shared<XmlDocumentationProvider>(path::from_text(*xml_doc_file));
    // On modern .NET there is no .NET Framework reference-assembly documentation; fall back to the ref pack parallel to the runtime (corlib lives in
    // dotnet/shared/Microsoft.NETCore.App/<version>/, the docs in the matching dotnet/packs/Microsoft.NETCore.App.Ref/<version>/ref/<tfm>/ directory).
    const std::optional<stdfs::path> runtime = runtime_directory();
    if (!runtime) return nullptr;
    return try_load_modern_ref_pack_documentation(path::native_text(*runtime / "System.Private.CoreLib.dll"));
}

struct CacheEntry {
    std::string file_name;
    std::shared_ptr<XmlDocumentationProvider> provider;
};
std::mutex g_cache_mutex;
std::map<const metadata::MetadataFile*, CacheEntry> g_cache;

}  // namespace

std::optional<std::filesystem::path> runtime_directory() {
    const std::optional<stdfs::path> dotnet = metadata::DotNetCorePathFinder::find_dot_net_exe_directory();
    if (!dotnet) return std::nullopt;
    const stdfs::path shared = *dotnet / "shared" / "Microsoft.NETCore.App";
    std::optional<stdfs::path> best;
    Version3 best_version{-1, -1, -1};
    for (const stdfs::path& dir : get_directories(shared)) {
        const Version3 v = parse_version_prefix(file_name_of(dir));
        if (!best || v > best_version) {
            best = dir;
            best_version = v;
        }
    }
    if (!best) return std::nullopt;
    return *best / "";
}

std::optional<std::string> get_documentation(const typesystem::IEntity& entity) {
    const typesystem::IModule* parent = entity.parent_module();
    const metadata::MetadataFile* module = parent != nullptr ? parent->metadata_file() : nullptr;
    if (module == nullptr) throw UsageError("the entity has no module");
    const std::shared_ptr<XmlDocumentationProvider> doc_provider = xml_doc_loader::load_documentation(*module);
    if (!doc_provider) return std::nullopt;
    return doc_provider->get_documentation(&entity);
}

namespace xml_doc_loader {

std::shared_ptr<XmlDocumentationProvider> mscorlib_documentation() {
    static std::once_flag once;
    static std::shared_ptr<XmlDocumentationProvider> value;
    std::call_once(once, [] { value = load_mscorlib_documentation(); });
    return value;
}

std::shared_ptr<XmlDocumentationProvider> load_documentation(const metadata::MetadataFile& module) {
    const std::lock_guard<std::mutex> lock(g_cache_mutex);
    const std::string module_file_name = path::native_text(module.file_name());
    if (const auto it = g_cache.find(&module); it != g_cache.end() && it->second.file_name == module_file_name) return it->second.provider;

    std::shared_ptr<XmlDocumentationProvider> xml_doc;
    std::optional<std::string> xml_doc_file = lookup_localized_xml_doc(module_file_name);
    if (!xml_doc_file) xml_doc_file = find_xml_documentation(path::get_file_name(module_file_name), module.get_runtime());
    if (xml_doc_file) {
        xml_doc = std::make_shared<XmlDocumentationProvider>(path::from_text(*xml_doc_file));
    } else {
        // Last resort for modern .NET (.NET 5+): runtime DLLs ship under dotnet/shared/Microsoft.NETCore.App/<version>/ with no .xml beside them. The
        // matching XML lives in the parallel ref pack under dotnet/packs/Microsoft.NETCore.App.Ref/<version>/ref/<tfm>/. Aggregate every *.xml in the
        // matching tfm folder into a single provider so type-forwarded entities (System.String docs live in System.Runtime.xml but the metadata token
        // comes from System.Private.CoreLib.dll) resolve transparently.
        xml_doc = try_load_modern_ref_pack_documentation(module_file_name);
    }
    g_cache[&module] = CacheEntry{module_file_name, xml_doc};   // cache null misses too - avoids re-scanning disk
    return xml_doc;
}

void clear_cache() {
    const std::lock_guard<std::mutex> lock(g_cache_mutex);
    g_cache.clear();
}

std::optional<std::filesystem::path> resolve_modern_ref_pack_tfm_directory(const std::string& assembly_file_name) {
    if (assembly_file_name.empty()) return std::nullopt;
    const std::optional<std::string> version_dir = path::get_directory_name(assembly_file_name);
    if (!version_dir) return std::nullopt;
    const std::optional<std::string> shared_framework = path::get_directory_name(*version_dir);
    if (!shared_framework || util::compare_ordinal_ignore_case(path::get_file_name(*shared_framework), "Microsoft.NETCore.App") != 0) return std::nullopt;
    const std::optional<std::string> shared_dir = path::get_directory_name(*shared_framework);
    if (!shared_dir || util::compare_ordinal_ignore_case(path::get_file_name(*shared_dir), "shared") != 0) return std::nullopt;
    const std::optional<std::string> dotnet_root = path::get_directory_name(*shared_dir);
    if (!dotnet_root) return std::nullopt;
    const stdfs::path pack_root = path::from_text(path::combine(*dotnet_root, "packs", "Microsoft.NETCore.App.Ref"));
    std::error_code ec;
    if (!stdfs::is_directory(pack_root, ec)) return std::nullopt;
    // The targeting (ref) pack version usually differs from the installed runtime patch version - e.g. runtime shared/.../10.0.8 against pack
    // .../10.0.0, or a relocated CI install whose only ref pack is a different feature band. Prefer an exact match, then the newest pack sharing the
    // runtime's major.minor, then the newest pack present.
    const std::optional<stdfs::path> pack_version_dir = select_ref_pack_version_directory(pack_root, path::get_file_name(*version_dir));
    if (!pack_version_dir) return std::nullopt;
    const stdfs::path ref_pack_root = *pack_version_dir / "ref";
    if (!stdfs::is_directory(ref_pack_root, ec)) return std::nullopt;
    // One tfm folder per pack version (e.g. "net10.0"). Sort by parsed (Major, Minor) - a lexicographic compare puts "net10.0" behind "net9.0" because
    // '1' < '9'. Multiple tfms only show up across pack versions, not within one.
    std::vector<stdfs::path> directories = get_directories(ref_pack_root);
    std::stable_sort(directories.begin(), directories.end(), [](const stdfs::path& a, const stdfs::path& b) { return parse_tfm(file_name_of(a)) > parse_tfm(file_name_of(b)); });
    if (directories.empty()) return std::nullopt;
    return directories.front();
}

std::optional<std::string> lookup_localized_xml_doc(const std::string& file_name) {
    if (file_name.empty()) return std::nullopt;

    const std::string xml_file_name = path::change_extension(file_name, ".xml");

    const Culture current_culture = current_ui_culture();
    const std::string localized_xml_doc_file = get_localized_name(xml_file_name, current_culture.name);
    const std::string localized_xml_doc_fallback_file = get_localized_name(xml_file_name, current_culture.two_letter_name);

    if (file_exists(localized_xml_doc_file)) return localized_xml_doc_file;
    if (file_exists(localized_xml_doc_fallback_file)) return localized_xml_doc_fallback_file;
    if (file_exists(xml_file_name)) return xml_file_name;
    if (current_culture.two_letter_name != "en") {
        const std::string english_xml_doc_file = get_localized_name(xml_file_name, "en");
        if (file_exists(english_xml_doc_file)) return english_xml_doc_file;
    }
    return std::nullopt;
}

}  // namespace xml_doc_loader

}  // namespace cb::dotnet::decompiler::documentation
