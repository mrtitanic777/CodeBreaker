#include "cb/java/console/project_scan.hpp"

#include <array>
#include <system_error>

#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"
#include "cb/java/console/tree.hpp"

namespace cb::java::console::project_scan {

namespace stdfs = std::filesystem;

namespace {

// Heavy build and tool folders the scan never enters.
bool is_skipped_dir(std::string_view name) {
    static constexpr std::array<std::string_view, 12> kSkipped = {
        ".git", ".svn", "node_modules", "bin", "obj", "debug", "release", "release_ltcg", "ipch", ".vs", "x64", "x86"};
    const std::string low = text::lower(name);
    for (std::string_view s : kSkipped)
        if (low == s) return true;
    return false;
}

tree::WalkOptions scan_options(std::size_t max_depth) { return {.max_depth = max_depth, .skip_dir = is_skipped_dir}; }

// A folder name compared the way file names are on this system: case-insensitively.
bool named(const stdfs::path& dir, std::string_view lower_name) { return text::lower(fs::to_text(dir.filename())) == lower_name; }

// The folder as an absolute path with "." and ".." resolved and no trailing separator.
stdfs::path absolute_dir(const stdfs::path& p) {
    std::error_code ec;
    stdfs::path out = stdfs::absolute(p, ec).lexically_normal();
    if (!out.has_filename() && out != out.root_path()) out = out.parent_path();
    return out;
}

}  // namespace

stdfs::path find_base(const stdfs::path& project_dir) {
    static constexpr std::array<std::string_view, 3> kBaseMarkers = {"jars", "game", "resources"};
    std::error_code ec;
    stdfs::path dir = absolute_dir(project_dir);
    for (int up = 0; up < 8; ++up) {
        for (stdfs::directory_iterator it(dir, ec), end; !ec && it != end; it.increment(ec)) {
            const std::string name = text::lower(fs::to_text(it->path().filename()));
            for (std::string_view marker : kBaseMarkers)
                if (name == marker) return dir;   // the nearest match wins (do not overshoot upward)
        }
        ec.clear();
        stdfs::path parent = dir.parent_path();
        if (parent == dir || parent.empty()) break;
        dir = std::move(parent);
    }
    return absolute_dir(project_dir);
}

std::optional<stdfs::path> find_jar(const stdfs::path& base) {
    std::optional<stdfs::path> found, fallback;
    tree::walk(base, scan_options(5), [&](const tree::Listing& l) {
        for (const std::string& f : l.files) {
            const std::string low = text::lower(f);
            if (!low.ends_with(".jar") || low.find("source") != std::string::npos) continue;
            if (low == "deobfuscated.jar" || low == "minecraft.jar") {
                found = l.dir / f;
                return false;
            }
            if (!fallback) fallback = l.dir / f;
        }
        return true;
    });
    return found ? found : fallback;
}

std::optional<stdfs::path> find_resources(const stdfs::path& base) {
    std::optional<stdfs::path> found;
    tree::walk(base, scan_options(5), [&](const tree::Listing& l) {
        if (!named(l.dir, "resources")) return true;
        for (const std::string& d : l.subdirs) {
            const std::string low = text::lower(d);
            if (low == "newsound" || low == "sound" || low == "newmusic" || low == "music" || low == "streaming") {
                found = l.dir;
                return false;
            }
        }
        return true;
    });
    return found;
}

std::optional<stdfs::path> find_java_src(const stdfs::path& project_dir, const stdfs::path& base) {
    auto has_java = [](const stdfs::path& dir) {
        bool any = false;
        tree::walk(dir, scan_options(6), [&](const tree::Listing& l) {
            for (const std::string& f : l.files)
                if (f.ends_with(".java")) any = true;
            return !any;
        });
        return any;
    };
    for (const char* name : {"src", "source", "sources"})   // the package root, not a deep package
        if (has_java(base / name)) return base / name;
    if (has_java(project_dir)) return absolute_dir(project_dir);
    return std::nullopt;
}

std::optional<stdfs::path> find_lang(const stdfs::path& base) {
    std::optional<stdfs::path> found, first;
    tree::walk(base, scan_options(6), [&](const tree::Listing& l) {
        if (!named(l.dir, "lang")) return true;
        for (const std::string& f : l.files) {
            const std::string low = text::lower(f);
            if (!low.ends_with(".lang")) continue;
            if (low == "en_us.lang") {
                found = l.dir / f;
                return false;
            }
            if (!first) first = l.dir / f;
        }
        return true;
    });
    return found ? found : first;
}

Resolved resolve(const stdfs::path& project_dir) {
    Resolved r;
    r.base = find_base(project_dir);
    r.jar = find_jar(r.base);
    r.resources = find_resources(r.base);
    r.java_src = find_java_src(project_dir, r.base);
    r.lang = find_lang(r.base);
    return r;
}

}  // namespace cb::java::console::project_scan
