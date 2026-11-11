#include "cb/java/console/assets.hpp"

#include <algorithm>
#include <format>
#include <system_error>
#include <unordered_set>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"
#include "cb/java/console/png_rgba.hpp"
#include "cb/java/console/tree.hpp"
#include "cb/java/textutil.hpp"

namespace cb::java::console::assets {

namespace stdfs = std::filesystem;

namespace {

constexpr std::array<std::string_view, 6> kCodeExtensions = {".java", ".cpp", ".h", ".hpp", ".cc", ".cxx"};

bool has_extension(std::string_view name, const auto& extensions) {
    const std::string low = text::lower(name);
    return std::any_of(extensions.begin(), extensions.end(), [&](std::string_view e) { return low.ends_with(e); });
}

// \w, ., / and - : what a resource path is made of.
bool is_path_char(char c) {
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '.' || c == '/' || c == '-';
}

std::string forward_slashes(const stdfs::path& p) { return fs::to_text(p); }

}  // namespace

bool is_asset_name(std::string_view name) { return has_extension(name, kAssetExtensions); }

std::vector<std::string> list_jar_assets(const ZipArchive& jar) {
    std::set<std::string> names;
    for (const ZipArchive::Entry& e : jar.entries())
        if (!e.name.ends_with('/') && is_asset_name(e.name)) names.insert(e.name);
    return {names.begin(), names.end()};
}

// ---- what the code loads --------------------------------------------------------------------------------------

std::string strip_comments(std::string_view text) {
    // block comments first, then line comments (a "//" inside a block comment goes with it)
    std::string no_blocks;
    no_blocks.reserve(text.size());
    std::size_t pos = 0;
    while (pos < text.size()) {
        const std::size_t open = text.find("/*", pos);
        const std::size_t close = open == std::string_view::npos ? std::string_view::npos : text.find("*/", open + 2);
        if (close == std::string_view::npos) break;   // an unterminated comment is not one
        no_blocks.append(text.substr(pos, open - pos));
        pos = close + 2;
    }
    no_blocks.append(text.substr(pos));
    std::string out;
    out.reserve(no_blocks.size());
    pos = 0;
    while (pos < no_blocks.size()) {
        const std::size_t open = no_blocks.find("//", pos);
        if (open == std::string::npos) break;
        out.append(no_blocks, pos, open - pos);
        const std::size_t eol = no_blocks.find('\n', open);
        if (eol == std::string::npos) {
            pos = no_blocks.size();
            break;
        }
        pos = eol;   // the newline stays
    }
    out.append(no_blocks, pos, std::string::npos);
    return out;
}

namespace {

// Every string literal in the text of the form "/<path chars>.<ext>" with a shipped extension in lower case, as the path
// between the quotes. Scans left to right; a quote that does not open such a literal is passed over, so the quote that
// closes an ordinary string can open the next one.
template <class Fn>
void for_each_resource_literal(std::string_view text, Fn&& fn) {
    std::size_t i = 0;
    while (i < text.size()) {
        const std::size_t open = text.find('"', i);
        if (open == std::string_view::npos) return;
        std::size_t end = open + 1;
        if (end < text.size() && text[end] == '/') {
            while (end < text.size() && is_path_char(text[end])) ++end;
            if (end < text.size() && text[end] == '"') {
                const std::string_view path = text.substr(open + 1, end - open - 1);
                // "/" + at least one character + "." + the extension, in lower case
                const bool shipped = std::any_of(kAssetExtensions.begin(), kAssetExtensions.end(), [&](std::string_view ext) {
                    return path.size() >= 2 + ext.size() && path.ends_with(ext);
                });
                if (shipped) {
                    fn(path);
                    i = end + 1;
                    continue;
                }
            }
        }
        i = open + 1;
    }
}

}  // namespace

ResourceRefs code_resource_refs(const stdfs::path& code_dir) {
    std::map<std::string, std::set<std::string>> refs;
    for (const stdfs::path& file : tree::files_below(code_dir)) {
        const std::string name = fs::to_text(file.filename());
        if (!has_extension(name, kCodeExtensions)) continue;
        const std::optional<std::string> body = textutil::try_read_text(file);
        if (!body) continue;
        for_each_resource_literal(strip_comments(*body), [&](std::string_view path) {
            const std::size_t first = path.find_first_not_of('/');
            refs[std::string(first == std::string_view::npos ? std::string_view() : path.substr(first))].insert(name);
        });
    }
    ResourceRefs out;
    for (auto& [path, files] : refs) out[path].assign(files.begin(), files.end());
    return out;
}

// ---- the console bundle ---------------------------------------------------------------------------------------

std::vector<std::string> list_console_assets(const stdfs::path& bundle_dir) {
    std::vector<std::string> out;
    if (bundle_dir.empty()) return out;
    for (const stdfs::path& file : tree::files_below(bundle_dir)) out.push_back(forward_slashes(file.lexically_relative(bundle_dir)));
    std::sort(out.begin(), out.end());
    return out;
}

std::vector<std::string> place_console_assets(const stdfs::path& bundle_dir, const stdfs::path& content_dir) {
    std::vector<std::string> placed = list_console_assets(bundle_dir);
    for (const std::string& rel : placed) fs::write_file(content_dir / fs::from_text(rel), fs::read_file(bundle_dir / fs::from_text(rel)));
    return placed;
}

// ---- extracting -----------------------------------------------------------------------------------------------

namespace {

// content_dir / name, provided the name stays inside content_dir (a jar is untrusted input: "../x" must not escape).
stdfs::path destination(const stdfs::path& content_dir, const std::string& name) {
    const stdfs::path rel = fs::from_text(name).lexically_normal();
    if (rel.empty() || rel.is_absolute() || rel.has_root_name() || *rel.begin() == "..")
        throw FormatError("jar", 0, std::format("entry '{}' would be written outside the Content folder", name));
    return content_dir / rel;
}

}  // namespace

Extracted extract(const ZipArchive& jar, const stdfs::path& content_dir, const ExtractOptions& options) {
    Extracted out;
    for (const ZipArchive::Entry& e : jar.entries()) {
        if (e.name.ends_with('/') || !is_asset_name(e.name)) continue;
        if (options.only && !options.only->contains(e.name)) continue;
        Bytes data = jar.read(e);
        if (options.normalize_png && text::iends_with(e.name, ".png")) {
            png::Converted c = png::normalize_to_rgba(data);
            if (c.changed) {
                data = std::move(c.png);
                out.normalized.push_back(e.name);
            }
        }
        fs::write_file(destination(content_dir, e.name), data);
        out.written.push_back(e.name);
    }
    std::sort(out.written.begin(), out.written.end());
    return out;
}

// ---- auditing -------------------------------------------------------------------------------------------------

Coverage coverage(const ZipArchive& jar, const stdfs::path& code_dir, const stdfs::path& bundle_dir) {
    Coverage cov;
    cov.needed = code_resource_refs(code_dir);
    std::set<std::string> supply;   // the jar and the console bundle
    for (const std::string& n : list_jar_assets(jar)) supply.insert(n);
    for (const std::string& n : list_console_assets(bundle_dir)) supply.insert(n);
    cov.supply = supply.size();
    std::unordered_set<std::string> supply_lower;   // lookup only
    for (const std::string& n : supply) supply_lower.insert(text::lower(n));
    std::set<std::string> referenced;
    for (const auto& [path, files] : cov.needed) {
        if (supply_lower.contains(text::lower(path))) cov.present.push_back(path);   // case drift is tolerated
        else cov.missing.push_back(path);
        referenced.insert(path);
    }
    std::set<std::string> present_lower;
    for (const std::string& p : cov.present) present_lower.insert(text::lower(p));
    for (const std::string& n : supply)
        if (!referenced.contains(n) && !present_lower.contains(text::lower(n))) cov.extra.push_back(n);
    return cov;
}

std::string render_coverage(const Coverage& cov, const std::vector<std::string>& converted) {
    std::string out =
        "# TEXTURE_COVERAGE.md -- shipped-asset audit\n\n"
        "Resource paths the game code loads vs assets present in the jar. A **MISSING** path is one the code opens but "
        "the jar does not contain -> at runtime the loader gets a NULL image and crashes. Everything here is copied into "
        "`Content\\` with its exact jar path so the loader needs no changes.\n\n";
    out += std::format("- code needs: {}   jar provides: {}   **present: {}   MISSING: {}**   (jar extras: {})\n\n",
                       cov.needed.size(), cov.supply, cov.present.size(), cov.missing.size(), cov.extra.size());
    if (!cov.missing.empty()) {
        out += "## MISSING -- code loads these but the jar lacks them (runtime crash)\n\n| Resource path | Loaded by |\n|---|---|\n";
        for (const std::string& p : cov.missing) {
            const std::vector<std::string>& files = cov.needed.at(p);
            const std::vector<std::string> shown(files.begin(), files.begin() + std::min<std::size_t>(4, files.size()));
            out += std::format("| `{}` | {} |\n", p, text::join(shown, ", "));
        }
        out += "\n";
    } else {
        out += "> No missing textures: the jar covers every resource path the code loads.\n\n";
    }
    if (!converted.empty()) {
        out +=
            "## PNGs normalized to RGBA (the 360 render fix)\n\n"
            "These were palettized/grayscale in the jar and would misrender as garbage colors if copied raw (the 360 "
            "texture loader expects 32-bit RGBA). They were decoded to RGBA8888 (honoring palette transparency) on "
            "extraction:\n\n";
        for (const std::string& p : converted) out += std::format("- `{}`\n", p);
        out += "\n";
    }
    if (!cov.extra.empty()) {
        out += "## Jar extras (shipped anyway, not referenced by code)\n\n";
        for (const std::string& p : cov.extra) out += std::format("- `{}`\n", p);
        out += "\n";
    }
    return out;
}

RunResult run(const stdfs::path& jar_path, const stdfs::path& content_dir, const RunOptions& options) {
    const ZipArchive jar = ZipArchive::open(jar_path);
    RunResult result;
    result.content_dir = content_dir;
    if (options.code_dir) result.coverage = coverage(jar, *options.code_dir, options.bundle_dir);

    ExtractOptions extraction;
    extraction.normalize_png = options.normalize_png;
    if (options.only_needed && result.coverage) {
        // the jar's own spelling of every path the code names (it may name it in another case)
        std::set<std::string> wanted_lower;
        for (const std::string& p : result.coverage->present) wanted_lower.insert(text::lower(p));
        std::set<std::string> only;
        for (const std::string& n : list_jar_assets(jar))
            if (wanted_lower.contains(text::lower(n))) only.insert(n);
        extraction.only = std::move(only);
    }
    Extracted extracted = extract(jar, content_dir, extraction);
    result.written = std::move(extracted.written);
    const std::vector<std::string> in_jar_order = extracted.normalized;   // the report lists them as the jar has them
    result.converted = in_jar_order;
    std::sort(result.converted.begin(), result.converted.end());
    // the console-only assets ship from the converter's bundle, not the jar
    result.console = place_console_assets(options.bundle_dir, content_dir);

    if (result.coverage) {
        stdfs::path parent = content_dir;
        if (!parent.has_filename()) parent = parent.parent_path();   // "Content/" -> "Content"
        parent = parent.parent_path();
        const stdfs::path report = (parent.empty() ? stdfs::path(".") : parent) / "TEXTURE_COVERAGE.md";
        textutil::write_text(report, render_coverage(*result.coverage, in_jar_order));
        result.report = report;
    }
    return result;
}

}  // namespace cb::java::console::assets
