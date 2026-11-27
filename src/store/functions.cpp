#include "store/functions.hpp"

#include "core/fs.hpp"
#include "core/text.hpp"

namespace cb::store::functions {

namespace {

bool is_set(const std::optional<std::filesystem::path>& path) { return path && !path->empty(); }

}  // namespace

std::filesystem::path default_path(const std::filesystem::path& state) {
    if (const auto env = fs::get_env("CB_FUNCTIONS_DB"); env && !env->empty()) return fs::from_text(*env);
    return state / "functions.sqlite";
}

std::filesystem::path databases_folder(const std::filesystem::path& state, const std::optional<std::filesystem::path>& path, std::optional<bool> own) {
    const auto env = fs::get_env("CB_FUNCTIONS_DB");
    const bool has_own = own.value_or(is_set(path) || (env && !env->empty()));
    if (!has_own) return state / "db";
    const std::filesystem::path index = std::filesystem::absolute(is_set(path) ? *path : default_path(state)).lexically_normal();
    if (text::lower(fs::to_text(index.filename())) == "functions.sqlite") return index.parent_path() / "db";
    return index.parent_path() / (index.stem().string() + ".db");
}

std::unique_ptr<store::FunctionStore> open(const std::filesystem::path& state, const std::optional<std::filesystem::path>& path, bool readonly) {
    const std::filesystem::path index = is_set(path) ? *path : default_path(state);
    if (readonly && !std::filesystem::is_regular_file(index)) return nullptr;
    return std::make_unique<store::FunctionStore>(index, databases_folder(state, path), readonly);
}

std::vector<store::Overview> overview(const std::filesystem::path& state, const std::optional<std::filesystem::path>& path) {
    const auto store = open(state, path, /*readonly=*/true);
    return store ? store->overview() : std::vector<store::Overview>{};
}

std::map<std::string, store::Class> latest(const std::filesystem::path& state, std::string_view module_sha, const std::optional<std::filesystem::path>& path) {
    const auto store = open(state, path, /*readonly=*/true);
    return store ? store->latest(module_sha) : std::map<std::string, store::Class>{};
}

}  // namespace cb::store::functions
