#include "decompile/jar.hpp"

#include <algorithm>
#include <format>
#include <system_error>

#include "core/fs.hpp"
#include "core/log.hpp"
#include "core/error.hpp"
#include "core/regex.hpp"
#include "core/text.hpp"
#include "java/projects/minecraft/prep/prepare.hpp"

namespace cb::decompile::jar {

namespace stdfs = std::filesystem;

namespace {

bool is_java_file(const stdfs::directory_entry& e, std::error_code& ec) {
    return e.is_regular_file(ec) && e.path().extension() == ".java";
}

// How many .java files lie under `dir`, at any depth. Unreadable directories count nothing.
std::size_t count_java_files(const stdfs::path& dir) {
    std::size_t n = 0;
    std::error_code ec;
    for (stdfs::recursive_directory_iterator it(dir, stdfs::directory_options::skip_permission_denied, ec), end;
         !ec && it != end; it.increment(ec)) {
        std::error_code entry_ec;
        if (is_java_file(*it, entry_ec)) ++n;
    }
    return n;
}

// The directories named `src` at or under `dir`, parents before children, siblings in name order.
void collect_src_dirs(const stdfs::path& dir, std::vector<stdfs::path>& out) {
    if (dir.filename() == "src") out.push_back(dir);
    std::vector<stdfs::path> children;
    std::error_code ec;
    for (stdfs::directory_iterator it(dir, stdfs::directory_options::skip_permission_denied, ec), end;
         !ec && it != end; it.increment(ec)) {
        std::error_code entry_ec;
        if (it->is_directory(entry_ec) && !it->is_symlink(entry_ec)) children.push_back(it->path());
    }
    std::sort(children.begin(), children.end());
    for (const stdfs::path& child : children) collect_src_dirs(child, out);
}

// `name` on PATH: the first directory holding name + one of PATHEXT's extensions. Does not look in the current
// directory (a program of that name there is not what a user asking for `java` means).
std::optional<stdfs::path> which(std::string_view name) {
    const std::string path_var = fs::get_env("PATH").value_or("");
    const std::string ext_var = fs::get_env("PATHEXT").value_or(".COM;.EXE;.BAT;.CMD");
    for (std::string_view dir : text::split(path_var, ';')) {
        dir = text::trim(dir, "\" ");
        if (dir.empty()) continue;
        for (std::string_view ext : text::split(ext_var, ';')) {
            if (ext.empty()) continue;
            stdfs::path candidate = fs::from_text(dir) / fs::from_text(std::string(name) + std::string(ext));
            std::error_code ec;
            if (stdfs::is_regular_file(candidate, ec)) return candidate;
        }
    }
    return std::nullopt;
}

// The mangled array parameter `Block[] block;4` / `byte[] [B4`: type, junk name, the trailing digits the body's
// `varN` uses. Followed by `,` or `)` (checked without consuming it).
const Regex& mangled_param() {
    static const Regex re(R"re((\w+(?:\[\])+)\s+([^\s,()]*[;\[][^\s,()]*?)(\d+)(?=\s*[,)]))re");
    return re;
}

// Remove every `/* $VF was: ... */` comment (the text between has no `*`).
std::string strip_vf_comments(std::string_view source) {
    constexpr std::string_view kOpen = "/* $VF was: ";
    std::string out;
    std::size_t at = 0;
    for (;;) {
        std::size_t open = source.find(kOpen, at);
        if (open == std::string_view::npos) break;
        std::size_t star = source.find('*', open + kOpen.size());
        if (star == std::string_view::npos || star + 1 >= source.size() || source[star + 1] != '/') {
            out.append(source.substr(at, open + kOpen.size() - at));   // not a comment this repair knows: keep it
            at = open + kOpen.size();
            continue;
        }
        out.append(source.substr(at, open - at));
        at = star + 2;
    }
    out.append(source.substr(at));
    return out;
}

}  // namespace

std::optional<stdfs::path> find_java(const Settings& settings, const std::optional<stdfs::path>& explicit_java) {
    auto is_file = [](const stdfs::path& p) {
        std::error_code ec;
        return !p.empty() && stdfs::is_regular_file(p, ec);
    };
    const stdfs::path& wanted = (explicit_java && !explicit_java->empty()) ? *explicit_java : settings.java;
    if (is_file(wanted)) return wanted;
    if (auto home = fs::get_env("JAVA_HOME"); home && !home->empty()) {
        stdfs::path candidate = fs::from_text(*home) / "bin" / "java.exe";
        if (is_file(candidate)) return candidate;
    }
    return which("java");
}

std::optional<stdfs::path> find_src(const stdfs::path& workspace) {
    std::vector<stdfs::path> candidates;
    collect_src_dirs(workspace, candidates);
    std::optional<stdfs::path> best;
    std::size_t best_count = 0;
    for (const stdfs::path& dir : candidates) {
        const std::size_t n = count_java_files(dir);
        if (n > best_count) {
            best = dir;
            best_count = n;
        }
    }
    return best;
}

stdfs::path decompile(std::string_view version, const stdfs::path& workspace_in, const DecompileOptions& options) {
    const stdfs::path workspace = stdfs::absolute(workspace_in).lexically_normal();
    fs::make_dirs(workspace);
    auto say = [&](const std::string& line) {
        if (options.on_output) options.on_output(line);
    };
    namespace prep = java::minecraft::prep;
    prep::PrepareOptions po;
    po.setup = options.setup;
    po.gl_constants = options.settings.gl_constants;
    po.on_output = [&](const std::string& line) { say(line); };
    prep::PrepareResult r;
    try {
        r = prep::prepare(std::string(version), workspace, po);
    } catch (const UsageError&) {   // (setup: the version is not in the list)
        throw DecompileError(std::format("Minecraft version '{}' was not found in the list of versions. "
                                         "Check the version id (e.g. a1.2.6, b1.6.6, c0.30-c-1900).", version));
    } catch (const std::exception& e) {
        throw DecompileError(std::format("Preparing Minecraft {} failed: {}", version, e.what()));
    }
    for (const std::string& m : r.messages) {
        if (m.find("Patching failed!") != std::string::npos) say("[decompile] " + m);
    }

    std::optional<stdfs::path> src = find_src(workspace);
    if (!src) throw DecompileError("Decompile produced no Java source under " + fs::to_text(workspace) + ".");
    const std::size_t fixed = repair_array_params(*src);
    say(std::format("[decompile] done: {} .java files at {}", count_java_files(*src), fs::to_text(*src)));
    if (fixed) say(std::format("[decompile] repaired {} mangled array parameter name(s)", fixed));
    return *src;
}

std::string repair_array_params_text(std::string_view source, std::size_t& renamed) {
    renamed = 0;
    std::string out = mangled_param().replace_all(source, [&](const RegexMatch& m) {
        ++renamed;
        return std::format("{} var{}", m.group(1), m.group(3));
    });
    return renamed ? strip_vf_comments(out) : out;
}

std::size_t repair_array_params(const stdfs::path& src) {
    std::size_t count = 0;
    std::error_code ec;
    for (stdfs::recursive_directory_iterator it(src, stdfs::directory_options::skip_permission_denied, ec), end;
         !ec && it != end; it.increment(ec)) {
        std::error_code entry_ec;
        if (!is_java_file(*it, entry_ec)) continue;
        const std::string source = fs::read_text(it->path());
        std::size_t n = 0;
        const std::string repaired = repair_array_params_text(source, n);
        if (n) {
            fs::write_text(it->path(), repaired);
            count += n;
        }
    }
    return count;
}

}  // namespace cb::decompile::jar
