#include "cb/java/console/sdk.hpp"

#include <format>
#include <system_error>

#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"
#include "cb/java/console/tree.hpp"
#include "cb/java/textutil.hpp"

namespace cb::java::console::sdk {

namespace stdfs = std::filesystem;

std::optional<Console> console_of(std::string_view platform) {
    if (platform == "xbox360") return Console::Xbox360;
    if (platform == "ps3") return Console::Ps3;
    if (platform == "wii") return Console::Wii;
    return std::nullopt;
}

std::string_view platform_name(Console c) {
    switch (c) {
        case Console::Xbox360: return "xbox360";
        case Console::Ps3: return "ps3";
        case Console::Wii: return "wii";
    }
    return {};
}

const SdkSpec& spec_of(Console c) {
    static const SdkSpec kXbox360 = {
        "Xbox 360 SDK (XDK)",
        {"XEDK"},
        {R"(C:\Program Files (x86)\Microsoft Xbox 360 SDK)", R"(C:\Program Files\Microsoft Xbox 360 SDK)", R"(C:\XDK)"},
        {stdfs::path("include") / "xbox", "include"},
        {"xtl.h"},
        "xbox360_sdk",
    };
    static const SdkSpec kPs3 = {
        "PlayStation 3 SDK (Cell)",
        {"SCE_PS3_ROOT", "CELL_SDK"},
        {R"(C:\usr\local\cell)", R"(C:\ProgramData\SCE\PS3)"},
        {stdfs::path("target") / "ppu" / "include", "include"},
        {"cell", "sys"},   // directories/headers present in the Cell SDK
        "",
    };
    static const SdkSpec kWii = {
        "Wii SDK (Revolution)",
        {"REVOLUTION_SDK_ROOT", "RVL_ROOT"},
        {R"(C:\projects\RVL)", R"(C:\RVL_SDK)"},
        {"include"},
        {"revolution.h"},
        "",
    };
    switch (c) {
        case Console::Xbox360: return kXbox360;
        case Console::Ps3: return kPs3;
        case Console::Wii: return kWii;
    }
    return kXbox360;
}

bool requires_sdk(std::string_view platform) { return console_of(platform).has_value(); }

namespace {

bool is_dir(const stdfs::path& p) {
    std::error_code ec;
    return stdfs::is_directory(p, ec);
}

bool path_exists(const stdfs::path& p) {
    std::error_code ec;
    return stdfs::exists(p, ec);
}

// A path as the person who typed it would write it on this machine.
std::string shown(const stdfs::path& p) { return p.u8string(); }

bool is_word(char c) {
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

}  // namespace

std::optional<std::vector<stdfs::path>> valid_root(const SdkSpec& spec, const stdfs::path& root) {
    if (root.empty() || !is_dir(root)) return std::nullopt;
    std::vector<stdfs::path> include_dirs;
    for (const stdfs::path& sub : spec.include_subdirs)
        if (is_dir(root / sub)) include_dirs.push_back(root / sub);
    if (include_dirs.empty()) include_dirs.push_back(root);   // some SDKs point the variable straight at the include folder
    std::vector<stdfs::path> searched = include_dirs;
    searched.push_back(root);
    for (const std::string& probe : spec.probe_headers)
        for (const stdfs::path& d : searched)
            if (path_exists(d / probe)) return include_dirs;
    return std::nullopt;
}

SdkInfo::SdkInfo(std::string platform, std::string name, stdfs::path root, std::vector<stdfs::path> include_dirs,
                 Source source, std::string source_detail)
    : platform_(std::move(platform)),
      name_(std::move(name)),
      root_(std::move(root)),
      include_dirs_(std::move(include_dirs)),
      source_(source),
      source_detail_(std::move(source_detail)) {}

std::string SdkInfo::source_text() const {
    switch (source_) {
        case Source::Ungated: return "ungated";
        case Source::Override: return "override";
        case Source::Setting: return "setting:" + source_detail_;
        case Source::Environment: return "env:" + source_detail_;
        case Source::DefaultPath: return "path";
    }
    return {};
}

const std::string& SdkInfo::all_header_text() {
    if (header_text_) return *header_text_;
    std::string blob;
    for (const stdfs::path& dir : include_dirs_) {
        for (const stdfs::path& file : tree::files_below(dir)) {
            const std::string ext = text::lower(fs::to_text(file.extension()));
            if (ext != ".h" && ext != ".hpp" && ext != ".hxx") continue;
            if (const auto text = textutil::try_read_text(file)) {
                if (!blob.empty()) blob += '\n';
                blob += *text;
            }
        }
    }
    header_text_ = std::move(blob);
    return *header_text_;
}

bool SdkInfo::has_symbol(std::string_view symbol) {
    if (symbol.empty()) return false;
    const std::string& blob = all_header_text();
    for (std::size_t at = blob.find(symbol); at != std::string::npos; at = blob.find(symbol, at + 1)) {
        // a whole identifier: no word character directly before or after (the symbol's own edges are word characters)
        const bool before = at > 0 && is_word(symbol.front()) && is_word(blob[at - 1]);
        const std::size_t after_at = at + symbol.size();
        const bool after = after_at < blob.size() && is_word(symbol.back()) && is_word(blob[after_at]);
        if (!before && !after) return true;
    }
    return false;
}

std::optional<SdkInfo> detect(std::string_view platform, const SdkHints& hints) {
    const std::optional<Console> console = console_of(platform);
    if (!console) return SdkInfo(std::string(platform), "no SDK required", {}, {}, SdkInfo::Source::Ungated);
    const SdkSpec& spec = spec_of(*console);
    const std::string platform_text(platform);
    if (hints.override_root && !hints.override_root->empty()) {
        if (auto inc = valid_root(spec, *hints.override_root))
            return SdkInfo(platform_text, spec.name, *hints.override_root, std::move(*inc), SdkInfo::Source::Override);
        return std::nullopt;   // an explicit path that is not an SDK is not silently replaced by another one
    }
    if (hints.configured_root && !spec.setting_key.empty()) {
        if (auto inc = valid_root(spec, *hints.configured_root))
            return SdkInfo(platform_text, spec.name, *hints.configured_root, std::move(*inc), SdkInfo::Source::Setting,
                           spec.setting_key);
    }
    for (const std::string& var : spec.env_vars) {
        const std::optional<std::string> value = fs::get_env(var);
        if (!value || value->empty()) continue;
        const stdfs::path root = fs::from_text(*value);
        if (auto inc = valid_root(spec, root))
            return SdkInfo(platform_text, spec.name, root, std::move(*inc), SdkInfo::Source::Environment, var);
    }
    for (const stdfs::path& root : spec.default_paths)
        if (auto inc = valid_root(spec, root))
            return SdkInfo(platform_text, spec.name, root, std::move(*inc), SdkInfo::Source::DefaultPath);
    return std::nullopt;
}

namespace {

std::string env_list(const SdkSpec& spec, std::string_view separator) {
    std::string out;
    for (const std::string& e : spec.env_vars) {
        if (!out.empty()) out += separator;
        out += "%" + e + "%";
    }
    return out;
}

std::string path_list(const SdkSpec& spec) {
    std::string out;
    for (const stdfs::path& p : spec.default_paths) {
        if (!out.empty()) out += ", ";
        out += shown(p);
    }
    return out;
}

}  // namespace

SdkInfo require(std::string_view platform, const SdkHints& hints) {
    if (auto info = detect(platform, hints)) return std::move(*info);
    const SdkSpec& spec = spec_of(*console_of(platform));   // detect() found no SDK, so the platform is a console
    std::string vars;
    for (std::size_t i = 0; i < spec.env_vars.size(); ++i) vars += (i ? " or %" : "%") + spec.env_vars[i] + "%";
    throw SdkNotFoundError(std::format(
        "No {} found -- cannot convert/emit for platform '{}'.\n"
        "  The converter never ships SDK headers or signatures; it pulls them from your installed SDK.\n"
        "  Provide it one of these ways:\n"
        "    * set the environment variable {} to the SDK root, or\n"
        "    * pass --sdk <path-to-sdk-root>, or\n"
        "    * install it to one of: {}\n"
        "  (Ungated targets still work: use --platform none for a portable conversion with no {} code.)",
        spec.name, platform, vars, path_list(spec), spec.name));
}

std::string describe(std::string_view platform, const SdkHints& hints) {
    const std::optional<Console> console = console_of(platform);
    if (!console) {
        // the ungated platforms are known ones; anything else is not a platform at all
        if (platform.empty() || platform == "none" || platform == "pc_windows" || platform == "pc" || platform == "windows")
            return std::format("platform '{}': no SDK required (ungated).", platform);
        return std::format("platform '{}': unknown.", platform);
    }
    const SdkSpec& spec = spec_of(*console);
    const std::optional<SdkInfo> info = detect(platform, hints);
    if (!info) {
        std::string slash_list;
        for (const std::string& e : spec.env_vars) slash_list += (slash_list.empty() ? "" : " / ") + e;
        return std::format("platform '{}': {} NOT found (checked {} + {}). Set {} or pass --sdk.", platform, spec.name,
                           env_list(spec, ", "), path_list(spec), slash_list);
    }
    return std::format("platform '{}': {} found at {} (via {}).", platform, info->name(), shown(info->root()),
                       info->source_text());
}

}  // namespace cb::java::console::sdk
