#include "cb/base/corpus.hpp"

#include <windows.h>

#include <algorithm>
#include <tuple>

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/text.hpp"
#include "cb/core/win.hpp"

namespace cb::base {

namespace stdfs = std::filesystem;

namespace {

constexpr std::string_view kEnvName = "CB_CORPUS_NAME";   // selects a product for a whole run; use() sets it for workers

// The extensions a bare module name is tried with.
constexpr std::string_view kBareNameExtensions[] = {".dll", ".exe", ".drv", ".ocx", ".cpl", ".vxd",
                                                    ".386", ".sys", ".com", ".acm", ".ax"};

void set_env(std::string_view name, const std::optional<std::string>& value) {
    const std::wstring wname = win::to_wide(name);
    if (value)
        SetEnvironmentVariableW(wname.c_str(), win::to_wide(*value).c_str());
    else
        SetEnvironmentVariableW(wname.c_str(), nullptr);
}

std::optional<std::string> non_empty_env(std::string_view name) {
    auto v = fs::get_env(name);
    if (v && v->empty()) return std::nullopt;
    return v;
}

bool is_dir(const stdfs::path& p) {
    std::error_code ec;
    return stdfs::is_directory(p, ec);
}

// The sort key that puts the copy the OS actually loads first: WINDOWS\SYSTEM, then WINDOWS, then anything else;
// ties broken by shorter path, then by name.
struct Rank {
    int tier;
    std::size_t depth;
    std::string lowered;
    auto operator<=>(const Rank&) const = default;
};

Rank rank_of(const stdfs::path& path) {
    const std::string lowered = text::lower(fs::to_text(path));
    const auto parts = text::split(lowered, '/');
    const bool system = std::find(parts.begin(), parts.end(), "system") != parts.end();
    const bool windows = std::find(parts.begin(), parts.end(), "windows") != parts.end();
    return {system ? 0 : (windows ? 1 : 2), parts.size(), lowered};
}

void sort_best_first(std::vector<stdfs::path>& paths) {
    std::vector<std::pair<Rank, stdfs::path>> keyed;
    keyed.reserve(paths.size());
    for (stdfs::path& p : paths) keyed.emplace_back(rank_of(p), std::move(p));
    std::sort(keyed.begin(), keyed.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
    for (std::size_t i = 0; i < keyed.size(); ++i) paths[i] = std::move(keyed[i].second);
}

// The folder text normalised for "is this path inside that folder": absolute, lexically clean, '/' separators,
// lowercase (Windows names are case-insensitive).
std::string normal_text(const stdfs::path& p) {
    std::error_code ec;
    return text::lower(fs::to_text(stdfs::absolute(p, ec).lexically_normal()));
}

}  // namespace

std::string_view lane_name(Lane lane) {
    switch (lane) {
        case Lane::Pe: return "pe";
        case Lane::Ne: return "ne";
        case Lane::Le: return "le";
        case Lane::Dos: return "dos";
    }
    return "";
}

std::optional<Lane> lane_from_name(std::string_view name) {
    for (Lane l : {Lane::Pe, Lane::Ne, Lane::Le, Lane::Dos})
        if (lane_name(l) == name) return l;
    return std::nullopt;
}

Corpus::Corpus(Products products, FormatProbes probes) : products_(std::move(products)), probes_(std::move(probes)) {}

// ---- the products ---------------------------------------------------------------------------------------

// {name: folder} of every product's original binaries: the listed ones, and the target (its default folder) when the
// list does not name it.
std::map<std::string, stdfs::path> Corpus::product_folders() const {
    std::map<std::string, stdfs::path> out = products_.listed;
    out.try_emplace(products_.target, products_.target_folder);
    return out;
}

std::vector<std::string> Corpus::names() const {
    std::vector<std::string> out;
    for (const auto& entry : product_folders()) out.push_back(entry.first);
    return out;
}

std::string Corpus::current() const {
    {
        std::lock_guard lock(mutex_);
        if (selected_ && !selected_->empty()) return *selected_;
    }
    if (auto env = non_empty_env(kEnvName)) return *env;
    return products_.target;
}

void Corpus::select(const std::optional<std::string>& name) {
    std::lock_guard lock(mutex_);
    selected_ = name;
    set_env(kEnvName, name);
}

std::string Corpus::use(const std::optional<std::string>& name) {
    const std::string before = current();
    if (name && !product_folders().contains(*name)) {
        std::string known;
        for (const std::string& n : names()) known += (known.empty() ? "" : ", ") + n;
        throw UsageError("no corpus '" + *name + "' (the configuration lists: " + known + ")");
    }
    select(name);
    return before;
}

Corpus::Selected::Selected(Corpus& corpus, const std::optional<std::string>& name) : corpus_(corpus) {
    {
        std::lock_guard lock(corpus_.mutex_);
        was_selected_ = corpus_.selected_;
    }
    was_env_ = fs::get_env(kEnvName);
    corpus_.use(name);
}

Corpus::Selected::~Selected() {
    corpus_.select(was_selected_);                    // (what was selected before is known to be valid)
    if (was_env_) set_env(kEnvName, was_env_);        // (a selection made outside, by the environment, stands again)
}

std::vector<std::string> Corpus::take_option(std::vector<std::string> args) {
    const auto at = std::find(args.begin(), args.end(), "--corpus");
    if (at == args.end()) return args;
    if (at + 1 == args.end()) {
        std::string known;
        for (const std::string& n : names()) known += (known.empty() ? "" : ", ") + n;
        throw UsageError("--corpus needs a name: " + known);
    }
    use(*(at + 1));
    args.erase(at, at + 2);
    return args;
}

// ---- where things are -----------------------------------------------------------------------------------

stdfs::path Corpus::root(const std::optional<std::string>& name_) const {
    std::string name = name_ ? *name_ : std::string();
    if (name.empty()) {
        std::lock_guard lock(mutex_);
        if (selected_) name = *selected_;
    }
    if (name.empty()) name = non_empty_env(kEnvName).value_or("");
    if (!name.empty() && (products_.listed.contains(name) || name != products_.target)) {
        const auto all = product_folders();
        if (const auto it = all.find(name); it != all.end()) return it->second;
        std::string known;
        for (const std::string& n : names()) known += (known.empty() ? "" : ", ") + n;
        throw UsageError("no corpus '" + name + "' (the configuration lists: " + known + ")");
    }
    if (auto env = non_empty_env("CB_CORPUS_ROOT"); env && is_dir(fs::from_text(*env))) return fs::from_text(*env);
    const stdfs::path sibling =
        products_.checkout_root.parent_path() / "Windows95" / "design" / "corpus" / "files";
    if (is_dir(sibling)) return sibling;
    if (auto env = non_empty_env("WT_DATA_ROOT"); env && is_dir(fs::from_text(*env))) return fs::from_text(*env);
    return sibling;   // a sensible path to report when nothing exists
}

stdfs::path Corpus::design_dir() const {
    std::string name;
    {
        std::lock_guard lock(mutex_);
        if (selected_) name = *selected_;
    }
    if (name.empty()) name = non_empty_env(kEnvName).value_or("");
    if (!name.empty() && name != products_.target) return products_.state / "products" / name / "design";
    return root().parent_path().parent_path();
}

stdfs::path Corpus::reference(const std::vector<std::string>& parts) const {
    stdfs::path out = products_.reference && !products_.reference->empty() ? *products_.reference
                                                                           : design_dir() / "reference";
    for (const std::string& part : parts) out /= part;
    return out;
}

std::optional<std::string> Corpus::owner(const stdfs::path& path) const {
    const std::string wanted = normal_text(path);
    std::optional<std::pair<std::string, std::size_t>> best;
    for (const std::string& name : names()) {
        const std::string folder = normal_text(root(name));
        if (wanted.starts_with(folder + "/") && (!best || folder.size() > best->second)) best = {name, folder.size()};
    }
    return best ? std::optional<std::string>(best->first) : std::nullopt;
}

// ---- finding a module -----------------------------------------------------------------------------------

std::optional<Lane> Corpus::lane(const stdfs::path& path) const {
    const std::string key = fs::to_text(path);
    {
        std::lock_guard lock(mutex_);
        if (const auto it = lanes_.find(key); it != lanes_.end()) return it->second;
    }
    const std::optional<Lane> found = probes_.lane ? probes_.lane(path) : std::nullopt;
    std::lock_guard lock(mutex_);
    lanes_.emplace(key, found);
    return found;
}

// Windows 1.x keeps its system modules (KERNEL, GDI, USER, the drivers) inside one boot image, a .BIN with an .OVL
// beside it, not in files of their own. Each is written out once as an ordinary executable under the state folder and
// listed as <module>.exe, so a module of such a product is found, read and reconstructed like any other.
void Corpus::unpack_boot_images(Index& index) const {
    if (!probes_.boot_image_modules) return;
    std::vector<std::string> images;
    for (const auto& entry : index)
        if (entry.first.ends_with(".bin") && index.contains(entry.first.substr(0, entry.first.size() - 4) + ".ovl"))
            images.push_back(entry.first);
    std::sort(images.begin(), images.end());
    for (const std::string& name : images) {
        const std::string stem = name.substr(0, name.size() - 4);
        const stdfs::path image = index.at(name).front();
        const stdfs::path overlay = index.at(stem + ".ovl").front();
        if (lane(image) != Lane::Ne) continue;
        const stdfs::path out = products_.state / "unpacked" / (stem + "-" + std::to_string(fs::file_size(image)));
        std::vector<BootModule> modules;
        try {
            modules = probes_.boot_image_modules(image, overlay);
        } catch (const FormatError& e) {
            log::debug(std::string("corpus: not a boot image: ") + e.what());
            continue;
        } catch (const IoError& e) {
            log::debug(std::string("corpus: boot image unreadable: ") + e.what());
            continue;
        }
        fs::make_dirs(out);
        for (const BootModule& module : modules) {
            const std::string file_name = text::lower(module.name) + ".exe";
            const stdfs::path path = out / file_name;
            std::error_code ec;
            if (!stdfs::is_regular_file(path, ec)) fs::write_file(path, module.executable);
            std::vector<stdfs::path>& have = index[file_name];   // (MSDOS.EXE on the disk is a 1-byte stub)
            const bool any_executable = std::any_of(have.begin(), have.end(), [&](const stdfs::path& p) { return lane(p).has_value(); });
            if (!any_executable) have.insert(have.begin(), path);
        }
    }
}

std::shared_ptr<const Corpus::Index> Corpus::index(bool refresh) const {
    const stdfs::path folder = root();
    const std::string key = fs::to_text(folder);
    std::lock_guard walking(index_mutex_);
    if (!refresh) {
        std::lock_guard lock(mutex_);
        if (const auto it = indexes_.find(key); it != indexes_.end()) return it->second;
    }
    auto built = std::make_shared<Index>();
    std::error_code ec;
    stdfs::recursive_directory_iterator it(folder, stdfs::directory_options::skip_permission_denied, ec);
    for (const stdfs::recursive_directory_iterator end; !ec && it != end; it.increment(ec)) {
        std::error_code entry_ec;
        if (it->is_directory(entry_ec)) continue;
        (*built)[text::lower(it->path().filename().string())].push_back(it->path());
    }
    for (auto& entry : *built) sort_best_first(entry.second);
    unpack_boot_images(*built);
    std::lock_guard lock(mutex_);
    indexes_[key] = built;
    return built;
}

std::vector<stdfs::path> Corpus::candidates(std::string_view name_) const {
    const std::string name = text::lower(fs::from_text(name_).filename().string());
    const auto idx = index();
    if (const auto it = idx->find(name); it != idx->end()) return it->second;
    std::vector<stdfs::path> out;
    if (name.find('.') == std::string::npos) {
        for (std::string_view ext : kBareNameExtensions)
            if (const auto it = idx->find(name + std::string(ext)); it != idx->end())
                out.insert(out.end(), it->second.begin(), it->second.end());
        sort_best_first(out);
    }
    return out;
}

stdfs::path Corpus::readable(const stdfs::path& path) const {
    return probes_.readable ? probes_.readable(path) : path;
}

std::optional<stdfs::path> Corpus::retail(std::string_view name) const {
    std::error_code ec;
    const stdfs::path as_path = fs::from_text(name);
    if (stdfs::is_regular_file(as_path, ec)) return readable(as_path);
    const auto found = candidates(name);
    if (found.empty()) return std::nullopt;
    return readable(found.front());
}

stdfs::path Corpus::resolve(std::string_view name) const {
    if (auto found = retail(name)) return *found;
    throw NotFoundError(fs::from_text(name), ERROR_FILE_NOT_FOUND, "no such module in the corpus");
}

std::vector<stdfs::path> Corpus::modules(std::optional<Lane> only, const std::vector<std::string>& extensions) const {
    std::vector<std::pair<std::pair<std::uint64_t, std::string>, stdfs::path>> keyed;
    for (const auto& entry : *index()) {
        const bool wanted = std::any_of(extensions.begin(), extensions.end(),
                                        [&](const std::string& e) { return entry.first.ends_with(e); });
        if (!wanted) continue;
        const stdfs::path& best = entry.second.front();
        if (only && lane(best) != only) continue;
        keyed.push_back({{fs::file_size(best), text::lower(fs::to_text(best))}, best});
    }
    std::sort(keyed.begin(), keyed.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
    std::vector<stdfs::path> out;
    out.reserve(keyed.size());
    for (auto& k : keyed) out.push_back(std::move(k.second));
    return out;
}

std::vector<Corpus::Executable> Corpus::executables() const {
    std::vector<Executable> out;
    for (const auto& entry : *index())
        for (const stdfs::path& p : entry.second)
            if (const auto kind = lane(p)) out.push_back({p, *kind});
    std::sort(out.begin(), out.end(), [](const Executable& a, const Executable& b) {
        return std::make_tuple(fs::to_text(a.path), a.lane) < std::make_tuple(fs::to_text(b.path), b.lane);
    });
    return out;
}

std::optional<std::string> Corpus::sha(const stdfs::path& path) const {
    std::error_code ec;
    const auto size = stdfs::file_size(path, ec);
    if (ec) return std::nullopt;
    const auto written = stdfs::last_write_time(path, ec);
    if (ec) return std::nullopt;
    const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(written.time_since_epoch()).count();
    const std::string key = fs::to_text(path) + '\n' + std::to_string(seconds) + '\n' + std::to_string(size);
    {
        std::lock_guard lock(mutex_);
        if (const auto it = shas_.find(key); it != shas_.end()) return it->second;
    }
    std::string digest;
    try {
        const fs::MappedFile file(path);
        digest = crypto::to_hex(crypto::sha256(file.view()));
    } catch (const IoError&) {
        return std::nullopt;   // a file that cannot be read has no digest
    }
    std::lock_guard lock(mutex_);
    shas_.emplace(key, digest);
    return digest;
}

}  // namespace cb::base
