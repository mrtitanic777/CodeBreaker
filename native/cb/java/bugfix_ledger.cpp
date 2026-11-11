#include "cb/java/bugfix_ledger.hpp"

#include <algorithm>
#include <format>
#include <map>
#include <set>

#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"
#include "cb/java/textutil.hpp"

namespace cb::java::bugfix_ledger {

namespace {

namespace stdfs = std::filesystem;

constexpr std::string_view kMarkerTokens[] = {"FIXED BUG", "J2X-BUGFIX"};

// Words that follow the marker token in META references to the marker system itself (a help/docstring line that
// says "grep for the J2X-BUGFIX marker") -- these are NOT real fix slugs, so they are skipped.
bool is_non_slug(std::string_view slug) { return slug == "marker" || slug == "markers"; }

// Source extensions worth scanning (skip binaries, images, generated caches).
bool is_scanned_extension(std::string_view name) {
    for (std::string_view ext : {".h", ".hpp", ".cpp", ".cc", ".c", ".py", ".md"}) {
        if (name.ends_with(ext)) return true;
    }
    return false;
}

// The scanner's own files: they contain the marker words as prose ABOUT markers, so they are excluded from the walk.
bool is_scanner_itself(std::string_view name) {
    return name == "bugfix_ledger.py" || name == "bugfix_ledger.cpp" || name == "bugfix_ledger.hpp";
}

bool is_slug_start(char c) { return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9'); }
bool is_slug_char(char c) { return is_slug_start(c) || c == '-'; }

struct Marker {
    std::size_t begin = 0;         // where the token starts
    std::size_t slug_begin = 0;
    std::size_t slug_end = 0;
};

// The first `(FIXED BUG|J2X-BUGFIX)\s+<slug>` in the line, <slug> being [A-Za-z0-9][A-Za-z0-9-]*.
std::optional<Marker> find_marker(std::string_view line) {
    std::optional<Marker> best;
    for (std::string_view token : kMarkerTokens) {
        for (std::size_t at = line.find(token); at != std::string_view::npos; at = line.find(token, at + 1)) {
            if (best && at >= best->begin) break;
            std::size_t i = at + token.size();
            const std::size_t spaces = i;
            while (i < line.size() && text::is_space(line[i])) ++i;
            if (i == spaces || i >= line.size() || !is_slug_start(line[i])) continue;
            Marker m{at, i, i};
            while (m.slug_end < line.size() && is_slug_char(line[m.slug_end])) ++m.slug_end;
            best = m;
            break;
        }
    }
    return best;
}

// The text without any trailing characters from chars.
std::string_view rstrip_chars(std::string_view s, std::string_view chars) {
    while (!s.empty() && chars.find(s.back()) != std::string_view::npos) s.remove_suffix(1);
    return s;
}

}  // namespace

// Strip code/comment punctuation to recover the human explanation on a marker line.
std::string clean_comment(std::string_view line) {
    std::string_view s = text::trim(line);
    std::string_view tail = s;
    if (auto m = find_marker(s)) {
        std::size_t i = m->slug_end;
        while (i < s.size() && (s[i] == ':' || text::is_space(s[i]))) ++i;
        tail = s.substr(i);
    }
    tail = text::trim(tail);
    tail = text::trim(rstrip_chars(tail, "*/"));
    return std::string(tail);
}

std::vector<MarkerEntry> scan_file(const std::filesystem::path& path) {
    std::vector<MarkerEntry> entries;
    const auto content = textutil::try_read_text(path);
    if (!content) return entries;
    const std::vector<std::string_view> lines = text::split(*content, '\n');
    // a text ending in "\n" has an empty last piece that is not a line
    const std::size_t count = !lines.empty() && lines.back().empty() ? lines.size() - 1 : lines.size();
    for (std::size_t i = 0; i < count; ++i) {
        const auto m = find_marker(lines[i]);
        if (!m) continue;
        const std::string slug(lines[i].substr(m->slug_begin, m->slug_end - m->slug_begin));
        if (is_non_slug(slug)) continue;   // a meta reference to the marker system, not a fix slug
        std::string desc = clean_comment(lines[i]);
        // If the marker line has no trailing prose, borrow the next comment line.
        if (desc.empty() && i + 1 < count) {
            std::string_view next = text::trim(lines[i + 1]);
            while (!next.empty() && (next.front() == '/' || next.front() == '*')) next.remove_prefix(1);
            next = text::trim(next);
            if (!next.empty() && !find_marker(next)) desc = std::string(text::trim(rstrip_chars(next, "*/")));
        }
        MarkerEntry e;
        e.slug = slug;
        e.line = static_cast<std::uint32_t>(i + 1);
        e.description = std::move(desc);
        entries.push_back(std::move(e));
    }
    return entries;
}

namespace {

struct LayerDef {
    Layer layer;
    const char* folder;   // under the scanned root; empty = the root itself
    const char* title;
};
constexpr LayerDef kLayers[] = {
    {Layer::Canonical, "platform/canonical", "Canonical headers (real java.* replacements)"},
    {Layer::Reference, "platform/reference", "Platform references (native 360/D3D/XInput/XACT layer)"},
    {Layer::Companion, "platform/companions", "Companions (bolt-on platform helpers)"},
    {Layer::Generator, "", "Generator / emitter / runtime / toolchain (the converter itself)"},
};

void walk(const stdfs::path& dir, const stdfs::path& repo_root, Layer layer, bool prune_data_dirs,
          std::vector<MarkerEntry>& out) {
    std::vector<stdfs::path> files, dirs;
    for (const auto& entry : stdfs::directory_iterator(dir)) {
        (entry.is_directory() ? dirs : files).push_back(entry.path());
    }
    std::sort(files.begin(), files.end());
    std::sort(dirs.begin(), dirs.end());
    for (const stdfs::path& file : files) {
        const std::string name = fs::to_text(file.filename());
        if (!is_scanned_extension(name) || is_scanner_itself(name)) continue;
        const std::string rel = fs::to_text(file.lexically_relative(repo_root));
        for (MarkerEntry& e : scan_file(file)) {
            e.layer = layer;
            e.file = rel;
            out.push_back(std::move(e));
        }
    }
    for (const stdfs::path& sub : dirs) {
        const std::string name = fs::to_text(sub.filename());
        if (name.find("__pycache__") != std::string::npos) continue;
        // the generator layer is the folder itself; platform/ and api_maps/ are scanned as their own layers
        if (prune_data_dirs && (name == "platform" || name == "api_maps")) continue;
        walk(sub, repo_root, layer, false, out);
    }
}

}  // namespace

std::vector<MarkerEntry> collect(const stdfs::path& repo_root) {
    std::vector<MarkerEntry> out;
    for (const LayerDef& def : kLayers) {
        const stdfs::path root = repo_root / fs::from_text(def.folder);
        std::error_code ec;
        if (!stdfs::is_directory(root, ec)) continue;
        walk(root, repo_root, def.layer, def.layer == Layer::Generator, out);
    }
    return out;
}

std::string render(std::span<const MarkerEntry> entries) {
    std::set<std::string> distinct;
    for (const MarkerEntry& e : entries) distinct.insert(e.slug);

    std::vector<std::string> lines;
    lines.push_back("# Bug-fix ledger");
    lines.push_back("");
    lines.push_back(
        "_Auto-generated by `bugfix_ledger` from the live `FIXED BUG` / `J2X-BUGFIX` markers in the converter's "
        "authored sources. Do not edit by hand -- fix the marker in-code and re-run the converter (or "
        "`codebreaker java bugfix-ledger`) to regenerate._");
    lines.push_back("");
    lines.push_back(
        "Every game-breaking fix carries a greppable in-code marker (standing rule). This ledger is the index of those "
        "markers so a fix can never be silently reverted by a reconvert without it showing up here as missing.");
    lines.push_back("");
    lines.push_back(std::format("**{} markers across {} distinct fixes.**", entries.size(), distinct.size()));
    lines.push_back("");
    for (const LayerDef& def : kLayers) {
        std::vector<const MarkerEntry*> rows;
        for (const MarkerEntry& e : entries) {
            if (e.layer == def.layer) rows.push_back(&e);
        }
        if (rows.empty()) continue;
        lines.push_back(std::format("## {}", def.title));
        lines.push_back("");
        lines.push_back("| Fix (slug) | Location | What it fixes |");
        lines.push_back("| --- | --- | --- |");
        std::stable_sort(rows.begin(), rows.end(), [](const MarkerEntry* a, const MarkerEntry* b) {
            if (a->file != b->file) return a->file < b->file;
            return a->line < b->line;
        });
        for (const MarkerEntry* e : rows) {
            std::string desc = text::replace_all(e->description, "|", "\\|");
            constexpr std::size_t kMaxColumns = 220;
            if (text::display_width(desc) > kMaxColumns) desc = std::string(text::utf8_prefix(desc, kMaxColumns - 3)) + "...";
            lines.push_back(std::format("| `{}` | {}:{} | {} |", e->slug, e->file, e->line, desc));
        }
        lines.push_back("");
    }
    return text::join(lines, "\n") + "\n";
}

std::vector<MarkerEntry> run(const stdfs::path& repo_root, const stdfs::path& out_path) {
    std::vector<MarkerEntry> entries = collect(repo_root);
    fs::write_text(out_path, render(entries));
    return entries;
}

}  // namespace cb::java::bugfix_ledger


