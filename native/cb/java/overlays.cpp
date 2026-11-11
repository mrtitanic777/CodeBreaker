#include "cb/java/overlays.hpp"

#include <algorithm>
#include <format>

#include "cb/core/fs.hpp"

namespace cb::java::overlays {

namespace stdfs = std::filesystem;

std::string project_key(const stdfs::path& out_dir) {
    if (out_dir.empty()) return {};
    stdfs::path p = out_dir.lexically_normal();
    if (p.filename().empty()) p = p.parent_path();   // "a/b/" names b
    if (p.filename().empty() || p.filename() == ".") {
        std::error_code ec;
        p = stdfs::absolute(out_dir, ec).lexically_normal();
        if (p.filename().empty()) p = p.parent_path();
    }
    return fs::to_text(p.filename());
}

std::optional<stdfs::path> resolve_overlay_dir(const stdfs::path& out_dir, const stdfs::path& overlays_root,
                                               const std::optional<stdfs::path>& explicit_dir) {
    if (explicit_dir && !explicit_dir->empty()) return explicit_dir;
    const std::string key = project_key(out_dir);
    if (key.empty()) return std::nullopt;
    return overlays_root / fs::from_text(key);
}

std::vector<std::string> apply_overlays(const stdfs::path& out_dir, const std::optional<stdfs::path>& overlay_dir,
                                        const Progress& progress) {
    auto say = [&](const std::string& line) {
        if (progress) progress(line);
    };
    std::vector<std::string> applied;
    std::error_code ec;
    if (!overlay_dir || !stdfs::is_directory(*overlay_dir, ec)) return applied;

    std::vector<stdfs::path> sources;
    for (const auto& entry : stdfs::recursive_directory_iterator(*overlay_dir)) {
        if (entry.is_regular_file() && entry.path().filename() != "MANIFEST.md") sources.push_back(entry.path());
    }
    std::sort(sources.begin(), sources.end());

    std::size_t added = 0;
    for (const stdfs::path& src : sources) {
        const stdfs::path rel = src.lexically_relative(*overlay_dir);
        const stdfs::path dst = out_dir / rel;
        const bool existed = stdfs::exists(dst, ec);
        fs::make_dirs(dst.parent_path());
        fs::write_file(dst, fs::read_file(src));
        const std::string rel_text = fs::to_text(rel);
        applied.push_back(rel_text);
        if (!existed) ++added;
        say(std::format("[overlay] {} {}", existed ? "replaced" : "ADDED   ", rel_text));
    }
    if (!applied.empty()) {
        say(std::format("[overlay] applied {} project overlay file(s) ({} new) from {}", applied.size(), added,
                        fs::to_text(*overlay_dir)));
    }
    return applied;
}

}  // namespace cb::java::overlays
