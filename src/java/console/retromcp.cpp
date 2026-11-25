// java/console/retromcp.cpp -- see retromcp.hpp.
#include "java/console/retromcp.hpp"

#include <algorithm>
#include <array>
#include <system_error>

#include <nlohmann/json.hpp>

#include "core/fs.hpp"
#include "core/text.hpp"

namespace cb::java::console::retromcp {

namespace stdfs = std::filesystem;

namespace {

constexpr int kMostLevelsUp = 8;
constexpr std::array<std::string_view, 2> kProjects = {"minecraft", "minecraft_server"};   // the client's first

stdfs::path rm_absolute_dir(const stdfs::path& p) {
    std::error_code ec;
    stdfs::path out = stdfs::absolute(p, ec).lexically_normal();
    if (!out.has_filename() && out != out.root_path()) out = out.parent_path();
    return out;
}

bool rm_is_dir(const stdfs::path& p) {
    std::error_code ec;
    return stdfs::is_directory(p, ec);
}

bool rm_is_file(const stdfs::path& p) {
    std::error_code ec;
    return stdfs::is_regular_file(p, ec);
}

// The child of `dir` named `name` in any case (the folder as it is spelled on disk); nullopt when there is none.
std::optional<stdfs::path> rm_child(const stdfs::path& dir, std::string_view name) {
    std::error_code ec;
    for (stdfs::directory_iterator it(dir, ec), end; !ec && it != end; it.increment(ec))
        if (text::lower(fs::to_text(it->path().filename())) == name) return it->path();
    return std::nullopt;
}

// Every .java file under `dir`, in path order (case-insensitive, '/' separators, so the order does not depend on the file system).
std::vector<stdfs::path> rm_java_files(const stdfs::path& dir) {
    std::vector<std::pair<std::string, stdfs::path>> keyed;
    std::error_code ec;
    for (stdfs::recursive_directory_iterator it(dir, ec), end; !ec && it != end; it.increment(ec)) {
        if (!it->is_regular_file(ec) || text::lower(fs::to_text(it->path().extension())) != ".java") continue;
        keyed.emplace_back(text::lower(it->path().lexically_relative(dir).generic_string()), it->path());
    }
    std::sort(keyed.begin(), keyed.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
    std::vector<stdfs::path> out;
    out.reserve(keyed.size());
    for (auto& [key, path] : keyed) out.push_back(std::move(path));
    return out;
}

bool rm_has_java(const stdfs::path& dir) {
    std::error_code ec;
    for (stdfs::recursive_directory_iterator it(dir, ec), end; !ec && it != end; it.increment(ec))
        if (it->is_regular_file(ec) && text::lower(fs::to_text(it->path().extension())) == ".java") return true;
    return false;
}

// options.cfg's setup= value ("" when the file or the line is missing).
std::string rm_setup_of(const stdfs::path& root) {
    const stdfs::path cfg = root / "options.cfg";
    if (!rm_is_file(cfg)) return {};
    try {
        const std::string cfg_text = fs::read_text(cfg);
        for (std::string_view line : text::split(cfg_text, '\n')) {
            line = text::trim(line, " \t\r");
            if (line.starts_with("setup=")) return std::string(text::trim(line.substr(6), " \t"));
        }
    } catch (const std::exception&) {
    }
    return {};
}

// A project folder of the root that holds Java source in src/.
std::optional<stdfs::path> rm_project(const stdfs::path& root, std::string_view name) {
    const std::optional<stdfs::path> dir = rm_child(root, name);
    if (!dir || !rm_is_dir(*dir)) return std::nullopt;
    const std::optional<stdfs::path> src = rm_child(*dir, "src");
    if (!src || !rm_has_java(*src)) return std::nullopt;
    return dir;
}

bool rm_is_root(const stdfs::path& dir) {
    const bool marked = !rm_setup_of(dir).empty() || rm_is_file(dir / "conf" / "version.json");
    if (!marked) return false;
    for (std::string_view name : kProjects)
        if (rm_project(dir, name)) return true;
    return false;
}

// The game's jar in the project: jars/deobfuscated.jar, else jars/minecraft.jar, else the first jar of jars/ that is not a source jar.
std::optional<stdfs::path> rm_jar(const stdfs::path& project) {
    const std::optional<stdfs::path> jars = rm_child(project, "jars");
    if (!jars) return std::nullopt;
    for (std::string_view name : {"deobfuscated.jar", "minecraft.jar", "minecraft_server.jar"})
        if (const auto jar = rm_child(*jars, name); jar && rm_is_file(*jar)) return jar;
    std::vector<stdfs::path> found;
    std::error_code ec;
    for (stdfs::directory_iterator it(*jars, ec), end; !ec && it != end; it.increment(ec)) {
        const std::string low = text::lower(fs::to_text(it->path().filename()));
        if (it->is_regular_file(ec) && low.ends_with(".jar") && low.find("source") == std::string::npos) found.push_back(it->path());
    }
    std::sort(found.begin(), found.end());
    if (found.empty()) return std::nullopt;
    return found.front();
}

// Every library jar under root/libraries: not a -sources jar, not a natives jar (those are unpacked into natives/), in path order.
std::vector<stdfs::path> rm_libraries(const stdfs::path& libraries) {
    std::vector<std::pair<std::string, stdfs::path>> keyed;
    std::error_code ec;
    for (stdfs::recursive_directory_iterator it(libraries, ec), end; !ec && it != end; it.increment(ec)) {
        if (it->is_directory(ec) && text::lower(fs::to_text(it->path().filename())) == "natives") {
            it.disable_recursion_pending();
            continue;
        }
        const std::string low = text::lower(fs::to_text(it->path().filename()));
        if (!it->is_regular_file(ec) || !low.ends_with(".jar")) continue;
        if (low.ends_with("-sources.jar") || low.find("-natives-") != std::string::npos) continue;
        keyed.emplace_back(text::lower(it->path().lexically_relative(libraries).generic_string()), it->path());
    }
    std::sort(keyed.begin(), keyed.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
    std::vector<stdfs::path> out;
    for (auto& [key, path] : keyed) out.push_back(std::move(path));
    return out;
}

// The dotted class name of a source file under the source root.
std::string rm_class_name(const stdfs::path& file, const stdfs::path& java_src) {
    stdfs::path rel = file.lexically_relative(java_src);
    rel.replace_extension();
    std::string name = rel.generic_string();
    std::replace(name.begin(), name.end(), '/', '.');
    return name;
}

}  // namespace

std::string find_entry_class(const stdfs::path& java_src) {
    std::string applet;
    for (const stdfs::path& file : rm_java_files(java_src)) {
        std::string body;
        try {
            body = fs::read_text(file);
        } catch (const std::exception&) {
            continue;
        }
        if (body.find("static void main(String") != std::string::npos) return rm_class_name(file, java_src);
        if (applet.empty() && (body.find(" extends Applet") != std::string::npos || body.find(" extends JApplet") != std::string::npos ||
                               body.find(" extends java.applet.Applet") != std::string::npos))
            applet = rm_class_name(file, java_src);
    }
    return applet;
}

std::optional<stdfs::path> find_root(const stdfs::path& dir_in) {
    stdfs::path dir = rm_absolute_dir(dir_in);
    if (!rm_is_dir(dir)) dir = dir.parent_path();
    for (int up = 0; up <= kMostLevelsUp; ++up) {
        if (rm_is_root(dir)) return dir;
        stdfs::path parent = dir.parent_path();
        if (parent == dir || parent.empty()) break;
        dir = std::move(parent);
    }
    return std::nullopt;
}

std::optional<Workspace> find(const stdfs::path& dropped) {
    const std::optional<stdfs::path> root = find_root(dropped);
    if (!root) return std::nullopt;
    Workspace w;
    w.root = *root;

    // the project the dropped folder is in; at the root (or in conf/, libraries/ ...), the client's, else the server's
    const stdfs::path at = rm_absolute_dir(dropped);
    const stdfs::path rel = at.lexically_relative(w.root);
    const std::string first = rel.empty() || rel == "." ? std::string() : text::lower(fs::to_text(*rel.begin()));
    std::optional<stdfs::path> project;
    for (std::string_view name : kProjects)
        if (first == name) project = rm_project(w.root, name);
    for (std::string_view name : kProjects)
        if (!project) project = rm_project(w.root, name);
    if (!project) return std::nullopt;
    w.project = *project;
    w.side = text::lower(fs::to_text(w.project.filename())) == "minecraft_server" ? "server" : "client";

    w.java_src = *rm_child(w.project, "src");
    w.java_files = rm_java_files(w.java_src).size();
    if (const auto original = rm_child(w.project, "src_original"); original && rm_is_dir(*original)) w.original_src = original;
    w.jar = rm_jar(w.project);
    if (const auto libraries = rm_child(w.root, "libraries"); libraries && rm_is_dir(*libraries)) {
        w.libraries = rm_libraries(*libraries);
        if (const auto natives = rm_child(*libraries, "natives"); natives && rm_is_dir(*natives)) w.natives = natives;
    }

    w.version = rm_setup_of(w.root);
    const stdfs::path manifest = w.root / "conf" / "version.json";
    if (rm_is_file(manifest)) {
        try {
            const nlohmann::json j = nlohmann::json::parse(fs::read_text(manifest));
            if (w.version.empty() && j.contains("id") && j["id"].is_string()) w.version = j["id"].get<std::string>();
            if (j.contains("mainClass") && j["mainClass"].is_string()) w.launcher_main = j["mainClass"].get<std::string>();
        } catch (const std::exception&) {   // a manifest that does not parse: the version and the launcher stay unknown
        }
    }
    w.entry_class = find_entry_class(w.java_src);

    // the asset index: version.json's assetIndex id under the project's game/assets/indexes, else the one index there is
    std::string index_id;
    if (rm_is_file(manifest)) {
        try {
            const nlohmann::json j = nlohmann::json::parse(fs::read_text(manifest));
            if (j.contains("assetIndex") && j["assetIndex"].is_object() && j["assetIndex"].contains("id") && j["assetIndex"]["id"].is_string())
                index_id = j["assetIndex"]["id"].get<std::string>();
        } catch (const std::exception&) {
        }
    }
    const stdfs::path indexes = w.project / "game" / "assets" / "indexes";
    if (!index_id.empty() && rm_is_file(indexes / (index_id + ".json"))) {
        w.asset_index = indexes / (index_id + ".json");
    } else {
        std::vector<stdfs::path> found;
        std::error_code ec;
        for (stdfs::directory_iterator it(indexes, ec), end; !ec && it != end; it.increment(ec))
            if (it->is_regular_file(ec) && text::lower(fs::to_text(it->path().extension())) == ".json") found.push_back(it->path());
        if (found.size() == 1) w.asset_index = found.front();
    }
    return w;
}

ResourceCopy copy_resources(const Workspace& workspace, const stdfs::path& dest) {
    ResourceCopy done;
    if (!workspace.asset_index) return done;
    nlohmann::json index;
    try {
        index = nlohmann::json::parse(fs::read_text(*workspace.asset_index));
    } catch (const std::exception&) {
        return done;
    }
    const stdfs::path objects = workspace.asset_index->parent_path().parent_path() / "objects";
    // "objects" is the index's own list; RetroMCP's indexes keep what it adds (the music) under "custom"
    for (const char* section : {"objects", "custom"}) {
        if (!index.contains(section) || !index[section].is_object()) continue;
        for (const auto& [name, entry] : index[section].items()) {
            if (!entry.is_object() || !entry.contains("hash") || !entry["hash"].is_string()) continue;
            const std::string hash = entry["hash"].get<std::string>();
            if (hash.size() < 3 || name.empty() || name.find("..") != std::string::npos || name.front() == '/') continue;   // (never outside dest)
            const stdfs::path from = objects / hash.substr(0, 2) / hash;
            if (!rm_is_file(from)) {
                ++done.missing;
                continue;
            }
            const stdfs::path to = dest / fs::from_text(name);
            std::error_code ec;
            stdfs::create_directories(to.parent_path(), ec);
            stdfs::copy_file(from, to, stdfs::copy_options::overwrite_existing, ec);
            if (!ec) ++done.copied;
        }
    }
    return done;
}

}  // namespace cb::java::console::retromcp
