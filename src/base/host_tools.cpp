// base/host_tools.cpp -- see host_tools.hpp.
#include "base/host_tools.hpp"

#include <algorithm>
#include <chrono>

#include "core/fs.hpp"
#include "core/process.hpp"
#include "core/text.hpp"

namespace stdfs = std::filesystem;

namespace cb::config::host_tools {

namespace {

bool is_file(const stdfs::path& p) {
    std::error_code ec;
    return !p.empty() && stdfs::is_regular_file(p, ec);
}

bool is_dir(const stdfs::path& p) {
    std::error_code ec;
    return !p.empty() && stdfs::is_directory(p, ec);
}

std::optional<stdfs::path> env_path(std::string_view name) {
    const std::optional<std::string> v = fs::get_env(name);
    if (!v || text::trim(*v).empty()) return std::nullopt;
    return fs::from_text(std::string(text::trim(*v, "\" ")));
}

// The folders on PATH, as written.
std::vector<stdfs::path> path_dirs() {
    std::vector<stdfs::path> out;
    const std::string path_var = fs::get_env("PATH").value_or("");   // (kept: the pieces are views of it)
    for (std::string_view dir : text::split(path_var, ';')) {
        dir = text::trim(dir, "\" ");
        if (!dir.empty()) out.push_back(fs::from_text(std::string(dir)));
    }
    return out;
}

std::optional<stdfs::path> on_path(std::string_view program) {
    for (const stdfs::path& dir : path_dirs())
        if (is_file(dir / program)) return dir / program;
    return std::nullopt;
}

std::vector<stdfs::path> program_files() {
    std::vector<stdfs::path> out;
    for (const char* name : {"ProgramFiles", "ProgramW6432", "ProgramFiles(x86)"})
        if (const auto p = env_path(name); p && std::find(out.begin(), out.end(), *p) == out.end()) out.push_back(*p);
    if (out.empty()) out.push_back("C:/Program Files");
    return out;
}

std::vector<stdfs::path> children(const stdfs::path& dir) {
    std::vector<stdfs::path> out;
    std::error_code ec;
    if (!is_dir(dir)) return out;
    for (stdfs::directory_iterator it(dir, ec), end; !ec && it != end; it.increment(ec))
        if (it->is_directory(ec)) out.push_back(it->path());
    return out;
}

std::string as_value(const stdfs::path& p) { return fs::to_text(p.lexically_normal()); }

// ---- each tool ---------------------------------------------------------------------------------------------------------------------------

std::optional<Found> find_java() {
    std::vector<stdfs::path> homes;
    std::vector<std::string> hows;
    const auto add = [&](const stdfs::path& home, const std::string& how) {
        if (std::find(homes.begin(), homes.end(), home) == homes.end()) {
            homes.push_back(home);
            hows.push_back(how);
        }
    };
    if (const auto home = env_path("JAVA_HOME")) add(*home, "JAVA_HOME");
    for (const stdfs::path& pf : program_files())
        for (const char* vendor : {"Java", "Eclipse Adoptium", "Microsoft", "Zulu", "BellSoft", "Amazon Corretto", "Semeru", "OpenJDK", "AdoptOpenJDK"})
            for (const stdfs::path& home : children(pf / vendor)) add(home, fs::to_text(pf / vendor));
    if (const auto user = env_path("USERPROFILE")) {
        for (const stdfs::path& home : children(*user / ".jdks")) add(home, "~/.jdks");
    }
    for (const stdfs::path& dir : path_dirs())
        if (is_file(dir / "javac.exe")) add(dir.parent_path(), "PATH");
    const std::optional<stdfs::path> best = newest_jdk(homes);
    if (!best) return std::nullopt;
    const std::size_t at = static_cast<std::size_t>(std::find(homes.begin(), homes.end(), *best) - homes.begin());
    return Found{"java", as_value(*best / "bin" / "java.exe"), hows[at]};
}

std::optional<stdfs::path> visual_studio() {
    for (const stdfs::path& pf : program_files()) {
        const stdfs::path vswhere = pf / "Microsoft Visual Studio" / "Installer" / "vswhere.exe";
        if (!is_file(vswhere)) continue;
        try {
            const process::ProcessResult r = process::run({.argv = {fs::to_text(vswhere), "-latest", "-prerelease", "-products", "*", "-requires",
                                                                    "Microsoft.VisualStudio.Component.VC.Tools.x86.x64", "-property", "installationPath", "-utf8"},
                                                           .timeout = std::chrono::seconds(20)});
            const std::string out(text::trim(std::string_view(reinterpret_cast<const char*>(r.out.data()), r.out.size())));
            const std::string first(text::trim(out.substr(0, out.find('\n'))));
            if (r.ok() && !first.empty() && is_dir(fs::from_text(first))) return fs::from_text(first);
        } catch (const std::exception&) {
        }
    }
    return std::nullopt;
}

std::optional<Found> find_cmake(const std::optional<stdfs::path>& vs) {
    if (const auto p = on_path("cmake.exe")) return Found{"cmake", as_value(*p), "PATH"};
    if (vs) {
        const stdfs::path bundled = *vs / "Common7" / "IDE" / "CommonExtensions" / "Microsoft" / "CMake" / "CMake" / "bin" / "cmake.exe";
        if (is_file(bundled)) return Found{"cmake", as_value(bundled), "Visual Studio"};
    }
    for (const stdfs::path& pf : program_files())
        if (is_file(pf / "CMake" / "bin" / "cmake.exe")) return Found{"cmake", as_value(pf / "CMake" / "bin" / "cmake.exe"), fs::to_text(pf / "CMake")};
    return std::nullopt;
}

std::optional<Found> find_ilspy() {
    if (const auto user = env_path("USERPROFILE"); user && is_file(*user / ".dotnet" / "tools" / "ilspycmd.exe"))
        return Found{"ilspycmd", as_value(*user / ".dotnet" / "tools" / "ilspycmd.exe"), "~/.dotnet/tools"};
    if (const auto p = on_path("ilspycmd.exe")) return Found{"ilspycmd", as_value(*p), "PATH"};
    return std::nullopt;
}

std::optional<Found> find_xbox360_sdk() {
    if (const auto p = env_path("XEDK"); p && is_dir(*p / "bin")) return Found{"xbox360_sdk", as_value(*p), "XEDK"};
    for (const stdfs::path& pf : program_files())
        if (is_dir(pf / "Microsoft Xbox 360 SDK" / "bin")) return Found{"xbox360_sdk", as_value(pf / "Microsoft Xbox 360 SDK"), fs::to_text(pf)};
    return std::nullopt;
}

std::optional<Found> find_openwatcom() {
    const auto has = [](const stdfs::path& p) { return is_file(p / "binnt" / "wcc386.exe") || is_file(p / "binnt64" / "wcc386.exe"); };
    if (const auto p = env_path("WATCOM"); p && has(*p)) return Found{"openwatcom", as_value(*p), "WATCOM"};
    for (const char* dir : {"C:/WATCOM", "C:/OpenWatcom", "C:/OW"})
        if (has(dir)) return Found{"openwatcom", as_value(dir), dir};
    return std::nullopt;
}

}  // namespace

std::optional<int> java_major(std::string_view version) {
    version = text::trim(version, "\" ");
    if (version.starts_with("1.")) version.remove_prefix(2);
    const std::size_t digits = version.find_first_not_of("0123456789");
    return text::parse_int<int>(version.substr(0, digits), 10);
}

std::optional<int> jdk_version(const stdfs::path& home) {
    if (!is_file(home / "bin" / "javac.exe") || !is_file(home / "bin" / "java.exe")) return std::nullopt;
    std::string release;
    try {
        release = fs::read_text(home / "release");
    } catch (const std::exception&) {
        return std::nullopt;
    }
    for (std::string_view line : text::lines(release)) {
        line = text::trim(line);
        if (line.starts_with("JAVA_VERSION=")) return java_major(line.substr(13));
    }
    return std::nullopt;
}

std::optional<stdfs::path> newest_jdk(const std::vector<stdfs::path>& homes, int least) {
    std::optional<stdfs::path> best;
    int best_version = 0;
    for (const stdfs::path& home : homes) {
        const std::optional<int> v = jdk_version(home);
        if (!v || *v < least || *v <= best_version) continue;
        best = home;
        best_version = *v;
    }
    return best;
}

std::vector<Found> scan() {
    std::vector<Found> out;
    if (auto f = find_java()) out.push_back(std::move(*f));
    const std::optional<stdfs::path> vs = visual_studio();
    if (vs) {
        out.push_back({"visual_studio", as_value(*vs), "vswhere"});
        const stdfs::path msbuild = *vs / "MSBuild" / "Current" / "Bin" / "MSBuild.exe";
        if (is_file(msbuild)) out.push_back({"msbuild", as_value(msbuild), "Visual Studio"});
    }
    if (auto f = find_cmake(vs)) out.push_back(std::move(*f));
    if (auto f = find_ilspy()) out.push_back(std::move(*f));
    if (auto f = find_xbox360_sdk()) out.push_back(std::move(*f));
    if (auto f = find_openwatcom()) out.push_back(std::move(*f));
    return out;
}

}  // namespace cb::config::host_tools
