#include "cb/java/console/project_layout.hpp"

#include <array>
#include <format>
#include <system_error>

#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"
#include "cb/java/textutil.hpp"

namespace cb::java::console::project_layout {

namespace stdfs = std::filesystem;

namespace {

// The configurations of an Xbox 360 project, in the order Visual Studio lists them.
constexpr std::array<std::string_view, 6> kConfigs = {"CodeAnalysis", "Debug", "Profile_FastCap",
                                                       "Profile",      "Release_LTCG", "Release"};

// Things that stay at the solution root (never moved down into the project subfolder).
bool stays_at_root(std::string_view lower_name) {
    return lower_name.ends_with(".sln") || lower_name.ends_with(".sdf") || lower_name.ends_with(".suo");
}

bool is_dir(const stdfs::path& p) {
    std::error_code ec;
    return stdfs::is_directory(p, ec);
}

std::string guid_text(const std::string& hex_digest, bool upper) {
    const std::string h = upper ? text::upper(hex_digest) : hex_digest;
    return std::format("{{{}-{}-{}-{}-{}}}", h.substr(0, 8), h.substr(8, 4), h.substr(12, 4), h.substr(16, 4), h.substr(20, 12));
}

}  // namespace

std::string project_guid(std::string_view name) {
    return guid_text(crypto::to_hex(crypto::md5(as_bytes("j2ctool-project-" + std::string(name)))), true);
}

std::optional<std::string> existing_guid(const stdfs::path& vcxproj) {
    const std::optional<std::string> body = textutil::try_read_text(vcxproj);
    if (!body) return std::nullopt;
    std::string_view text = *body;
    if (text.starts_with("\xEF\xBB\xBF")) text.remove_prefix(3);   // a BOM, as Visual Studio writes
    constexpr std::string_view kOpen = "<ProjectGuid>", kClose = "</ProjectGuid>";
    for (std::size_t at = text.find(kOpen); at != std::string_view::npos; at = text.find(kOpen, at + 1)) {
        const std::size_t start = at + kOpen.size();
        if (start >= text.size() || text[start] != '{') continue;
        const std::size_t brace = text.find('}', start);
        if (brace == std::string_view::npos) continue;
        if (text.substr(brace + 1, kClose.size()) == kClose) return std::string(text.substr(start, brace + 1 - start));
    }
    return std::nullopt;
}

std::string solution_text(std::string_view name, std::string_view guid) {
    std::string out = std::format(
        "\n"
        "Microsoft Visual Studio Solution File, Format Version 11.00\n"
        "# Visual Studio 2010\n"
        "Project(\"{{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}}\") = \"{}\", \"{}\\{}.vcxproj\", \"{}\"\n"
        "EndProject\n"
        "Global\n"
        "\tGlobalSection(SolutionConfigurationPlatforms) = preSolution\n",
        name, name, name, guid);
    for (std::string_view c : {"CodeAnalysis", "Debug", "Profile_FastCap", "Profile", "Release_LTCG", "Release"})
        out += std::format("\t\t{}|Xbox 360 = {}|Xbox 360\n", c, c);
    out += "\tEndGlobalSection\n\tGlobalSection(ProjectConfigurationPlatforms) = postSolution\n";
    for (std::string_view c : kConfigs)
        for (std::string_view kind : {"ActiveCfg", "Build.0"})
            out += std::format("\t\t{}.{}|Xbox 360.{} = {}|Xbox 360\n", guid, c, kind, c);
    out += "\tEndGlobalSection\n\tGlobalSection(SolutionProperties) = preSolution\n\t\tHideSolutionNode = FALSE\n"
           "\tEndGlobalSection\nEndGlobal\n";
    return out;
}

stdfs::path write_solution(const stdfs::path& root, std::string_view name, std::string_view guid) {
    const stdfs::path path = root / (std::string(name) + ".sln");
    textutil::write_text(path, to_crlf(solution_text(name, guid)));
    return path;
}

bool has_flat_sources(const stdfs::path& dir) {
    std::error_code ec;
    for (stdfs::directory_iterator it(dir, ec), end; !ec && it != end; it.increment(ec))
        if (fs::to_text(it->path().filename()).ends_with(".cpp")) return true;
    return false;
}

std::size_t move_flat_into(const stdfs::path& root, const stdfs::path& proj_dir, std::string_view name) {
    fs::make_dirs(proj_dir);
    std::vector<stdfs::path> entries;
    std::error_code ec;
    for (stdfs::directory_iterator it(root, ec), end; !ec && it != end; it.increment(ec)) entries.push_back(it->path());
    std::size_t moved = 0;
    for (const stdfs::path& src : entries) {
        const std::string entry = fs::to_text(src.filename());
        if (entry == name) continue;                                  // the project subfolder
        if (stays_at_root(text::lower(entry))) continue;
        const stdfs::path dst = proj_dir / src.filename();
        if (fs::same_path(src, proj_dir)) continue;
        if (stdfs::exists(dst, ec)) {
            if (is_dir(dst)) stdfs::remove_all(dst, ec);
            else stdfs::remove(dst, ec);
        }
        stdfs::rename(src, dst, ec);
        if (ec) {   // locked (an open log, say): left at the root
            ec.clear();
            continue;
        }
        ++moved;
    }
    return moved;
}

stdfs::path template_path(const JavaData& data) { return data.path({"console", "templates", "xbox360.vcxproj.in"}); }

Layout layout(const stdfs::path& root_in, const stdfs::path& vcxproj_template, const vcxproj::FilterRules& rules,
              const Options& options) {
    std::error_code ec;
    Layout out;
    out.root = stdfs::absolute(root_in, ec).lexically_normal();
    if (!out.root.has_filename() && out.root != out.root.root_path()) out.root = out.root.parent_path();
    out.name = options.name.empty() ? fs::to_text(out.root.filename()) : options.name;
    out.proj_dir = out.root / out.name;

    if (!has_flat_sources(out.proj_dir) && has_flat_sources(out.root)) out.moved = move_flat_into(out.root, out.proj_dir, out.name);
    fs::make_dirs(out.proj_dir);

    out.project_file = out.proj_dir / (out.name + ".vcxproj");
    out.guid = existing_guid(out.project_file).value_or(project_guid(out.name));
    if (!stdfs::is_regular_file(out.project_file, ec)) {
        const std::string tmpl = textutil::read_text(vcxproj_template);
        textutil::write_text(out.project_file, to_crlf(text::replace_all(tmpl, "{{GUID}}", out.guid)));
    }
    if (options.regenerate) {
        vcxproj::Options run_options;
        run_options.name = out.name;
        out.refreshed = vcxproj::run(out.proj_dir, rules, run_options);   // the ClCompile/ClInclude lists and the filters
    }
    out.sln = write_solution(out.root, out.name, out.guid);
    return out;
}

}  // namespace cb::java::console::project_layout
