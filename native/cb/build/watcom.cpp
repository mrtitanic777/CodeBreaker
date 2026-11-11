#include "cb/build/watcom.hpp"

#include <algorithm>

#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/text.hpp"

namespace cb::build::watcom {

namespace stdfs = std::filesystem;

namespace {

bool is_word(char c) { return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_'; }

// `identifier@digits`, the decoration of a __stdcall name.
bool is_stdcall_name(std::string_view s) {
    const std::size_t at = s.find('@');
    if (at == std::string_view::npos || !text::is_identifier(s.substr(0, at))) return false;
    const std::string_view digits = s.substr(at + 1);
    return !digits.empty() && std::all_of(digits.begin(), digits.end(), [](char c) { return c >= '0' && c <= '9'; });
}

// `?` followed by word characters, `@`, `$` and `?`: a C++ mangled name.
bool is_cxx_name(std::string_view s) {
    if (s.size() < 2 || s[0] != '?') return false;
    return std::all_of(s.begin() + 1, s.end(), [](char c) { return is_word(c) || c == '@' || c == '$' || c == '?'; });
}

// The file name without its last extension, upper-cased: the DLL as the original names it.
std::string module_of(std::string_view dll) {
    const std::size_t dot = dll.rfind('.');
    return text::upper(dot == std::string_view::npos || dot == 0 ? dll : dll.substr(0, dot));
}

}  // namespace

stdfs::path scratch_dir(std::span<const std::string_view> parts) {
    stdfs::path p = stdfs::temp_directory_path() / "codebreaker" / "bodyverify";
    for (const std::string_view part : parts) p /= std::string(part);
    return p;
}

OpenWatcom OpenWatcom::from_settings(std::string_view settings_root) {
    const std::optional<std::string> env = fs::get_env("CB_OPENWATCOM");
    return OpenWatcom(fs::from_text(env && !env->empty() ? std::string_view(*env) : settings_root));
}

stdfs::path OpenWatcom::tool(std::string_view name) const {
    std::string file(name);
    if (!text::iends_with(file, ".exe")) file += ".exe";
    const stdfs::path p = root_ / "binnt64" / file;
    std::error_code ec;
    return !root_.empty() && stdfs::exists(p, ec) ? p : stdfs::path();
}

std::vector<stdfs::path> OpenWatcom::include_dirs(Target target) const {
    const stdfs::path h = root_ / "h";
    return {h, h / (target == Target::Win16 ? "win" : "nt")};
}

stdfs::path OpenWatcom::lib_dir(Target target) const {
    return target == Target::Win16 ? root_ / "lib286" / "win" : root_ / "lib386" / "nt";
}

std::map<std::string, std::string> OpenWatcom::environment(Target target) const {
    std::string include;
    for (const stdfs::path& dir : include_dirs(target)) {
        if (!include.empty()) include += ';';
        include += fs::to_text(dir);
    }
    const std::string bin = fs::to_text(root_ / "binnt64");
    std::map<std::string, std::string> env;
    env["NoDefaultCurrentDirectoryInExePath"] = "";   // OpenWatcom runs its own tools from the current directory
    env["WATCOM"] = fs::to_text(root_);
    env["INCLUDE"] = include;
    env["WLINK_LNK"] = fs::to_text(root_ / "binnt64" / "wlink.lnk");
    env["PATH"] = bin + ";" + fs::get_env("PATH").value_or("");
    return env;
}

std::shared_ptr<const ImportMap> ImportMaps::of(const stdfs::path& module) {
    std::lock_guard<std::mutex> hold(lock_);
    if (const auto it = maps_.find(module); it != maps_.end()) return it->second;
    auto map = std::make_shared<ImportMap>();
    std::vector<ImportedName> imported;
    try {
        imported = reader_(module);
    } catch (const std::exception& e) {
        // Not a readable PE: no imports. This is an isolation boundary: a module that cannot be read must not stop
        // the build of the others; the empty map is the answer, and callers see no import for any name.
        log::debug("imports of " + fs::to_text(module) + ": " + e.what());
    }
    for (const ImportedName& i : imported) {
        std::string c;
        if (text::is_identifier(i.name)) {
            c = i.name;
        } else if (is_stdcall_name(i.name) || is_cxx_name(i.name)) {
            c = text::sanitize_identifier(i.name);
        } else {
            continue;   // an import by ordinal ("#12") or a name no C identifier can stand for
        }
        map->try_emplace(c, module_of(i.dll));   // the first DLL wins
        if (c != i.name) export_names_[c] = i.name;
    }
    maps_.emplace(module, map);
    return map;
}

std::string ImportMaps::export_name(const std::string& c_name) const {
    std::lock_guard<std::mutex> hold(lock_);
    const auto it = export_names_.find(c_name);
    return it == export_names_.end() ? c_name : it->second;
}

std::string import_lines(std::span<const std::string> externs, std::span<const std::string> wanted, const ImportMap& imports,
                         const ImportMaps& names, const std::unordered_map<std::string, NumberedImport>* numbered) {
    std::vector<std::string_view> sorted(wanted.begin(), wanted.end());
    std::sort(sorted.begin(), sorted.end());
    sorted.erase(std::unique(sorted.begin(), sorted.end()), sorted.end());
    std::string out;
    for (const std::string_view n : sorted) {
        for (const std::string& sym : externs) {
            // the decoration adds ONE underscore: `__lread@12` is `_lread`, not `lread`
            std::string_view bare = sym;
            if (bare.starts_with('_')) bare.remove_prefix(1);
            bare = bare.substr(0, bare.find('@'));
            if (bare != n) continue;
            const std::string key(n);
            if (numbered != nullptr) {
                if (const auto num = numbered->find(key); num != numbered->end()) {
                    out += "import '" + sym + "' '" + num->second.dll + "'." + std::to_string(num->second.number) + "\n";
                    break;
                }
            }
            if (const auto dll = imports.find(key); dll != imports.end()) {
                out += "import '" + sym + "' '" + dll->second + "'.'" + names.export_name(key) + "'\n";
            }
            break;
        }
    }
    return out;
}

}  // namespace cb::build::watcom
