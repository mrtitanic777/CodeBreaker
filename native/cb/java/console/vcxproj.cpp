#include "cb/java/console/vcxproj.hpp"

#include <algorithm>
#include <format>
#include <set>
#include <system_error>

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"
#include "cb/java/console/tree.hpp"
#include "cb/java/textutil.hpp"
#include "nlohmann/json.hpp"

namespace cb::java::console::vcxproj {

namespace stdfs = std::filesystem;

// ---- the classification rules ---------------------------------------------------------------------------------

struct FilterRules::Compiled {
    // The profile's patterns are data, so they are compiled when the rules are built -- once per profile.
    std::vector<std::pair<Regex, std::string>> rules;
};

FilterRules::FilterRules() : compiled_(std::make_shared<const Compiled>()) {}

FilterRules::FilterRules(const std::vector<FilterRule>& rules) {
    auto compiled = std::make_shared<Compiled>();
    compiled->rules.reserve(rules.size());
    for (const FilterRule& r : rules) compiled->rules.emplace_back(Regex(r.pattern), r.filter);
    compiled_ = std::move(compiled);
}

std::size_t FilterRules::size() const { return compiled_->rules.size(); }

std::string FilterRules::classify(std::string_view class_name, const std::map<std::string, std::string>& parent_of) const {
    auto by_rule = [&](std::string_view name) -> const std::string* {
        for (const auto& [regex, filter] : compiled_->rules)
            if (regex.match(name)) return &filter;
        return nullptr;
    };
    if (const std::string* f = by_rule(class_name)) return *f;
    std::set<std::string> seen;
    auto up = parent_of.find(std::string(class_name));
    while (up != parent_of.end() && !up->second.empty() && !seen.contains(up->second)) {
        const std::string& ancestor = up->second;
        seen.insert(ancestor);
        if (const std::string* f = by_rule(ancestor)) return *f;
        up = parent_of.find(ancestor);
    }
    return std::string(kFallbackFilter);
}

// ---- inheritance recovered from the emitted headers -----------------------------------------------------------

namespace {

bool is_word(char c) {
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}
bool is_space(char c) { return c == ' ' || (c >= '\t' && c <= '\r'); }

std::string_view word_at(std::string_view s, std::size_t pos) {
    std::size_t end = pos;
    while (end < s.size() && is_word(s[end])) ++end;
    return s.substr(pos, end - pos);
}

std::size_t skip_space(std::string_view s, std::size_t pos) {
    while (pos < s.size() && is_space(s[pos])) ++pos;
    return pos;
}

}  // namespace

// `class Name : [public|private|protected] Base` where `class` is the first thing on its line (an `enum class` is
// not). A base written after `virtual` is "virtual" and one written `public ::B` is "public": the declaration is read
// by its shape, as it always was, and the folder walk only follows bases that a rule names.
void add_class_bases(std::string_view text, std::map<std::string, std::string>& parent_of) {
    std::size_t from = 0;
    while (true) {
        const std::size_t at = text.find("class", from);
        if (at == std::string_view::npos) return;
        from = at + 1;
        // only whitespace between the start of the line and `class`
        std::size_t line_start = text.rfind('\n', at == 0 ? 0 : at - 1);
        line_start = (line_start == std::string_view::npos || at == 0) ? 0 : line_start + 1;
        if (!std::ranges::all_of(text.substr(line_start, at - line_start), is_space)) continue;
        std::size_t p = at + 5;
        if (p >= text.size() || !is_space(text[p])) continue;
        p = skip_space(text, p);
        const std::string_view name = word_at(text, p);
        if (name.empty()) continue;
        p = skip_space(text, p + name.size());
        if (p >= text.size() || text[p] != ':') continue;
        p = skip_space(text, p + 1);
        std::string_view base = word_at(text, p);
        if (base.empty()) continue;
        if (base == "public" || base == "private" || base == "protected") {
            const std::size_t after = p + base.size();
            if (after < text.size() && is_space(text[after])) {
                const std::size_t next = skip_space(text, after);
                if (const std::string_view named_base = word_at(text, next); !named_base.empty()) {
                    base = named_base;
                    from = next + named_base.size();
                    parent_of.emplace(std::string(name), std::string(base));
                    continue;
                }
            }
        }
        from = p + base.size();   // a declaration is not read twice
        parent_of.emplace(std::string(name), std::string(base));
    }
}

std::map<std::string, std::string> build_parent_map(const stdfs::path& proj_dir) {
    std::map<std::string, std::string> parent_of;
    std::vector<std::string> headers;
    std::error_code ec;
    for (stdfs::directory_iterator it(proj_dir, ec), end; !ec && it != end; it.increment(ec)) {
        std::error_code file_ec;
        if (it->is_regular_file(file_ec) && it->path().extension() == ".h") headers.push_back(fs::to_text(it->path().filename()));
    }
    std::sort(headers.begin(), headers.end());
    for (const std::string& name : headers)
        if (const auto text = textutil::try_read_text(proj_dir / name)) add_class_bases(*text, parent_of);
    return parent_of;
}

// ---- guids, Title ID ------------------------------------------------------------------------------------------

std::string filter_guid(std::string_view name) {
    const std::string h = crypto::to_hex(crypto::md5(as_bytes("j2ctool-filter-" + std::string(name))));
    return std::format("{{{}-{}-{}-{}-{}}}", h.substr(0, 8), h.substr(8, 4), h.substr(12, 4), h.substr(16, 4), h.substr(20, 12));
}

std::optional<std::string> normalize_title_id(std::string_view raw) {
    std::string s = text::lower(text::trim(raw));
    if (s.starts_with("0x")) s.erase(0, 2);
    if (s.empty() || s.size() > 8) return std::nullopt;
    for (char c : s)
        if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'))) return std::nullopt;
    s.insert(0, 8 - s.size(), '0');
    return text::upper(s);
}

TitleId title_id_for(const stdfs::path& proj_dir) {
    const stdfs::path config = proj_dir / "platform.config.json";
    std::error_code ec;
    if (!stdfs::is_regular_file(config, ec)) return {};
    const std::optional<std::string> body = textutil::try_read_text(config);
    Json doc;
    if (!body || !json::try_parse(*body, doc) || !doc.is_object())
        return {std::nullopt, "title id: could not read platform.config.json (not readable JSON)"};
    const auto at = doc.find("title_id");
    const bool unset = at == doc.end() || at->is_null() || (at->is_string() && at->get<std::string>().empty()) ||
                       (at->is_boolean() && !at->get<bool>()) || (at->is_number() && at->get<double>() == 0) ||
                       ((at->is_array() || at->is_object()) && at->empty());
    if (unset)
        return {std::nullopt,
                "title id: UNSET in platform.config.json -- the XEX keeps 0x00000000, so achievements cannot bind to a "
                "profile (set it, then re-run vcxproj)"};
    std::string shown;      // how the value reads in the message
    std::string digits;     // what is tried as a Title ID
    if (at->is_string()) {
        digits = at->get<std::string>();
        shown = "'" + digits + "'";
    } else {
        digits = at->dump();   // a number as it is written; anything else cannot be a Title ID
        shown = digits;
    }
    if (auto id = normalize_title_id(digits)) return {std::move(id), {}};
    return {std::nullopt, std::format("title id: {} in platform.config.json is not 1-8 hex digits -- ignored", shown)};
}

// ---- sources --------------------------------------------------------------------------------------------------

Sources enumerate_sources(const stdfs::path& proj_dir) {
    static const std::set<std::string, std::less<>> kSkipped = {"debug", "release", "release_ltcg", "profile", "fastcap",
                                                                "codeanalysis", "report", ".vs", "_decompiled", "x64",
                                                                "obj", "bin", ".git", "ipch"};
    Sources out;
    tree::WalkOptions options;
    options.skip_dir = [](std::string_view name) { return kSkipped.contains(text::lower(name)); };
    tree::walk(proj_dir, options, [&](const tree::Listing& l) {
        const stdfs::path rel_dir = l.dir.lexically_relative(proj_dir);
        std::string prefix = (rel_dir.empty() || rel_dir == ".") ? std::string() : fs::to_text(rel_dir) + "/";
        for (const std::string& f : l.files) {
            std::string rel = prefix + f;
            std::replace(rel.begin(), rel.end(), '/', '\\');
            if (f.ends_with(".cpp")) out.cpps.push_back(std::move(rel));
            else if (f.ends_with(".h")) out.headers.push_back(std::move(rel));
        }
        return true;
    });
    std::sort(out.cpps.begin(), out.cpps.end());
    std::sort(out.headers.begin(), out.headers.end());
    return out;
}

// ---- writing --------------------------------------------------------------------------------------------------

namespace {

std::string xml_escape(std::string_view v) {
    std::string out;
    for (char c : v) {
        if (c == '&') out += "&amp;";
        else if (c == '<') out += "&lt;";
        else if (c == '>') out += "&gt;";
        else out += c;
    }
    return out;
}

// The directory part of a backslash path, "" for a bare file name.
std::string dir_of(const std::string& rel) {
    const std::size_t cut = rel.rfind('\\');
    return cut == std::string::npos ? std::string() : rel.substr(0, cut);
}

std::string stem_of(const std::string& file) {
    const std::size_t dot = file.rfind('.');
    return dot == std::string::npos ? file : file.substr(0, dot);
}

std::vector<std::string> split_levels(const std::string& filter) {
    std::vector<std::string> parts;
    std::size_t from = 0;
    while (true) {
        const std::size_t cut = filter.find('\\', from);
        if (cut == std::string::npos) break;
        parts.push_back(filter.substr(from, cut - from));
        from = cut + 1;
    }
    parts.push_back(filter.substr(from));
    return parts;
}

}  // namespace

FiltersWritten write_filters(const stdfs::path& proj_dir, std::string_view proj_name, const FilterRules& rules,
                             const Sources& sources) {
    const std::map<std::string, std::string> parent_of = build_parent_map(proj_dir);   // the output's real inheritance graph
    std::map<std::string, std::string> file_filter;
    std::set<std::string> filters;   // sorted: the folder list is written in name order
    auto classify = [&](const std::string& fn) {
        std::string filter = fn.find('\\') != std::string::npos ? dir_of(fn)   // a file in a subfolder: its own folder
                                                                : rules.classify(stem_of(fn), parent_of);
        std::string level;
        for (const std::string& part : split_levels(filter)) {   // every nesting level is a folder of its own
            level += level.empty() ? part : "\\" + part;
            filters.insert(level);
        }
        file_filter[fn] = std::move(filter);
    };
    for (const std::string& fn : sources.cpps) classify(fn);
    for (const std::string& fn : sources.headers) classify(fn);

    std::string xml = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                      "<Project ToolsVersion=\"4.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">\n"
                      "  <ItemGroup>\n";
    for (const std::string& filter : filters) {
        xml += std::format("    <Filter Include=\"{}\">\n      <UniqueIdentifier>{}</UniqueIdentifier>\n    </Filter>\n",
                           xml_escape(filter), filter_guid(filter));
    }
    xml += "  </ItemGroup>\n  <ItemGroup>\n";
    for (const std::string& fn : sources.cpps)
        xml += std::format("    <ClCompile Include=\"{}\"><Filter>{}</Filter></ClCompile>\n", xml_escape(fn),
                           xml_escape(file_filter[fn]));
    xml += "  </ItemGroup>\n  <ItemGroup>\n";
    for (const std::string& fn : sources.headers)
        xml += std::format("    <ClInclude Include=\"{}\"><Filter>{}</Filter></ClInclude>\n", xml_escape(fn),
                           xml_escape(file_filter[fn]));
    xml += "  </ItemGroup>\n</Project>\n";

    FiltersWritten written;
    written.path = proj_dir / (std::string(proj_name) + ".vcxproj.filters");
    written.folders = filters.size();
    textutil::write_text(written.path, to_crlf(xml));
    return written;
}

// ---- regenerating the project file ----------------------------------------------------------------------------

namespace {

// The project file to operate on: <name>.vcxproj when a name is given, else the one .vcxproj in the folder.
stdfs::path find_vcxproj(const stdfs::path& proj_dir, const std::string& name) {
    if (!name.empty()) return proj_dir / (name + ".vcxproj");
    std::vector<std::string> found;
    std::error_code ec;
    for (stdfs::directory_iterator it(proj_dir, ec), end; !ec && it != end; it.increment(ec))
        if (it->path().extension() == ".vcxproj") found.push_back(fs::to_text(it->path().filename()));
    if (found.empty()) throw NotFoundError(proj_dir / "*.vcxproj", 0, "no .vcxproj in the project folder");
    std::sort(found.begin(), found.end());
    return proj_dir / found.front();
}

// The number of characters of a UTF-8 text (what the status line reports as its size).
std::size_t code_points(std::string_view s) {
    std::size_t n = 0;
    for (char c : s)
        if ((static_cast<unsigned char>(c) & 0xC0) != 0x80) ++n;
    return n;
}

// Every line that is not blank (only white space), joined with '\n'.
std::string without_blank_lines(std::string_view text) {
    std::string out;
    bool first = true;
    for (std::string_view line : text::split(text, '\n')) {
        if (text::trim(line).empty()) continue;
        if (!first) out += '\n';
        first = false;
        out.append(line);
    }
    return out;
}

// Remove every "<ItemGroup>...</ItemGroup>" (the bare form, from the tag to the first closing tag) that lists
// compile or include items.
std::string drop_file_item_groups(const std::string& text) {
    constexpr std::string_view kOpen = "<ItemGroup>", kClose = "</ItemGroup>";
    std::string out;
    std::size_t pos = 0;
    while (true) {
        const std::size_t open = text.find(kOpen, pos);
        const std::size_t close = open == std::string::npos ? std::string::npos : text.find(kClose, open + kOpen.size());
        if (close == std::string::npos) break;
        const std::size_t end = close + kClose.size();
        const std::string_view block = std::string_view(text).substr(open, end - open);
        out.append(text, pos, open - pos);
        if (block.find("<ClCompile") == std::string_view::npos && block.find("<ClInclude") == std::string_view::npos)
            out.append(block);
        pos = end;
    }
    out.append(text, pos, std::string::npos);
    return out;
}

// Remove every `<open_tag ...>...<close_tag>` element together with the white space before it.
std::string drop_elements(std::string text, std::string_view open_tag, std::string_view close_tag) {
    std::size_t pos = 0;
    while (true) {
        const std::size_t open = text.find(open_tag, pos);
        if (open == std::string::npos) break;
        const std::size_t close = text.find(close_tag, open + open_tag.size());
        if (close == std::string::npos) break;
        std::size_t start = open;
        while (start > 0 && is_space(text[start - 1])) --start;
        text.erase(start, close + close_tag.size() - start);
        pos = start;
    }
    return text;
}

std::string compile_group(const std::vector<std::string>& cpps, const std::optional<std::string>& stdafx_block) {
    std::string out = "  <ItemGroup>\n";
    for (const std::string& fn : cpps) {
        if (fn == "stdafx.cpp" && stdafx_block) {
            out += "    " + *stdafx_block + "\n";
        } else if (fn.find('\\') != std::string::npos) {
            // a subfolder's subsystem (xuimenu, say) carries its OWN stdafx.h, not the game's precompiled header
            out += std::format(
                "    <ClCompile Include=\"{}\">\n"
                "      <PrecompiledHeader>NotUsing</PrecompiledHeader>\n"
                "      <AdditionalIncludeDirectories>$(ProjectDir);%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>\n"
                "    </ClCompile>\n",
                fn);
        } else {
            out += std::format("    <ClCompile Include=\"{}\" />\n", fn);
        }
    }
    return out + "  </ItemGroup>\n";
}

std::string include_group(const std::vector<std::string>& headers) {
    std::string out = "  <ItemGroup>\n";
    for (const std::string& fn : headers) out += std::format("    <ClInclude Include=\"{}\" />\n", fn);
    return out + "  </ItemGroup>\n";
}

// Assets are emitted DIRECTLY into Debug\Content (= game:\Content next to the Debug .xex) by the asset, audio and
// achievement tools, so there is no Content\ at the project root. game: maps to $(OutDir), which differs per
// configuration (Debug\, Release\, Release_LTCG\ ...), so a Release or LTCG build's game:\Content (and game:\Media,
// the XUI front end's skin, fonts and strings) would otherwise be EMPTY. This target mirrors Debug's Content and
// Media into any non-Debug configuration's OutDir; /D copies only changed files (the big audio banks are not copied
// again on a rebuild) and each folder is guarded by Exists() so a project with only Content deploys cleanly.
constexpr std::string_view kDeployTarget =
    "  <Target Name=\"DeployContent\" AfterTargets=\"Build\" Condition=\"'$(Configuration)' != 'Debug'\">\n"
    "    <Exec Condition=\"Exists('$(ProjectDir)Debug\\Content')\" "
    "Command=\"xcopy /E /I /Y /Q /D &quot;$(ProjectDir)Debug\\Content&quot; &quot;$(OutDir)Content\\&quot;\" />\n"
    "    <Exec Condition=\"Exists('$(ProjectDir)Debug\\Media')\" "
    "Command=\"xcopy /E /I /Y /Q /D &quot;$(ProjectDir)Debug\\Media&quot; &quot;$(OutDir)Media\\&quot;\" />\n"
    "  </Target>\n";

// The Title ID goes into <ImageXex><TitleID>: the ImageXex task stamps it into the XEX. It MUST equal the id in the
// XLAST/SPA achievement config or unlocks never bind to a profile, so both read the ONE value in
// platform.config.json. The project template carries a single <ImageXex> block, inside the Release_LTCG
// ItemDefinitionGroup, so filling only that one would leave every other configuration building a 0x00000000 XEX.
// An ItemDefinitionGroup with no Condition applies to all configurations: emit one of those (regenerated each run,
// so repeating is harmless) AND fill any per-configuration block that exists. Returns the number of those filled.
std::size_t stamp_title_id(std::string& text, const std::string& tid) {
    text = drop_elements(std::move(text), "<ItemDefinitionGroup Label=\"J2XTitleId\">", "</ItemDefinitionGroup>");   // ours, from a previous run
    // <TitleID> + white space only + </TitleID>  ->  <TitleID>0xNNNNNNNN</TitleID>
    std::size_t filled = 0;
    std::string out;
    std::size_t pos = 0;
    while (true) {
        const std::size_t open = text.find("<TitleID>", pos);
        if (open == std::string::npos) break;
        std::size_t p = open + 9;
        while (p < text.size() && is_space(text[p])) ++p;
        // an id stamped by an earlier run (0x and eight hex digits) is replaced like an empty element, so that the next run
        // after the config changed leaves no configuration with the old one
        if (text.compare(p, 2, "0x") == 0 && p + 10 <= text.size() &&
            std::all_of(text.begin() + static_cast<std::ptrdiff_t>(p) + 2, text.begin() + static_cast<std::ptrdiff_t>(p) + 10,
                        [](char c) { return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'); })) {
            p += 10;
            while (p < text.size() && is_space(text[p])) ++p;
        }
        if (text.compare(p, 10, "</TitleID>") != 0) {
            out.append(text, pos, open + 9 - pos);
            pos = open + 9;
            continue;
        }
        out.append(text, pos, open - pos);
        out += "<TitleID>0x" + tid + "</TitleID>";
        pos = p + 10;
        ++filled;
    }
    out.append(text, pos, std::string::npos);
    text = std::move(out);

    const std::string shared = "  <ItemDefinitionGroup Label=\"J2XTitleId\">\n    <ImageXex>\n      <TitleID>0x" + tid +
                               "</TitleID>\n    </ImageXex>\n  </ItemDefinitionGroup>\n";
    const std::size_t group = text.find("<ItemDefinitionGroup");
    if (group != std::string::npos) {
        std::size_t start = group;   // in front of its indentation
        while (start > 0 && (text[start - 1] == ' ' || text[start - 1] == '\t')) --start;
        text.insert(start, shared);
    } else {
        text = text::replace_all(text, "</Project>", shared + "</Project>");
    }
    return filled;
}

}  // namespace

std::string Report::summary() const {
    std::string msg = std::format("vcxproj: {} bytes -> {} bytes; {} .cpp, {} .h listed{}", chars_before, chars_after,
                                  cpp_count, header_count, stdafx_block_found ? "" : "  (WARNING: stdafx.cpp PCH block not found)");
    if (!title_message.empty()) msg += "\n" + title_message;
    if (filters) msg += std::format("\nfilters: {} ({} subsystem folders)", fs::to_text(filters->path.filename()), filters->folders);
    return msg;
}

Report run(const stdfs::path& proj_dir, const FilterRules& rules, const Options& options) {
    Report report;
    report.vcxproj = find_vcxproj(proj_dir, options.name);
    const std::string proj_name = fs::to_text(report.vcxproj.stem());

    std::string text = textutil::read_text(report.vcxproj);
    report.chars_before = code_points(text);
    text = without_blank_lines(text);

    // stdafx.cpp's per-configuration PCH block survives the rewrite
    std::optional<std::string> stdafx_block;
    constexpr std::string_view kStdafxOpen = "<ClCompile Include=\"stdafx.cpp\">";
    if (const std::size_t open = text.find(kStdafxOpen); open != std::string::npos) {
        if (const std::size_t close = text.find("</ClCompile>", open); close != std::string::npos)
            stdafx_block = text.substr(open, close + 12 - open);
    }
    report.stdafx_block_found = stdafx_block.has_value();

    text = drop_file_item_groups(text);
    const Sources sources = enumerate_sources(proj_dir);
    report.cpp_count = sources.cpps.size();
    report.header_count = sources.headers.size();

    text = drop_elements(std::move(text), "<Target Name=\"StageContent\"", "</Target>");
    text = drop_elements(std::move(text), "<Target Name=\"DeployContent\"", "</Target>");
    text = text::replace_all(text, "</Project>",
                             compile_group(sources.cpps, stdafx_block) + include_group(sources.headers) +
                                 std::string(kDeployTarget) + "</Project>");

    report.title = title_id_for(proj_dir);
    report.title_message = report.title.note;
    if (report.title.id) {
        const std::size_t filled = stamp_title_id(text, *report.title.id);
        report.title_message = std::format("title id: 0x{} (all configurations; {} per-config block(s) filled)",
                                           *report.title.id, filled);
    }

    textutil::write_text(report.vcxproj, to_crlf(text));
    report.chars_after = code_points(text);
    if (options.write_filters) report.filters = write_filters(proj_dir, proj_name, rules, sources);
    return report;
}

}  // namespace cb::java::console::vcxproj
