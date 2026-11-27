// base/host_tools.cpp -- see host_tools.hpp.
#include "base/host_tools.hpp"

#include "base/tool_catalog.hpp"
#include "core/fs.hpp"
#include "core/text.hpp"

namespace stdfs = std::filesystem;

namespace cb::config::host_tools {

std::optional<int> java_major(std::string_view version) {
    version = text::trim(version, "\" ");
    if (version.starts_with("1.")) version.remove_prefix(2);
    const std::size_t digits = version.find_first_not_of("0123456789");
    return text::parse_int<int>(version.substr(0, digits), 10);
}

namespace {

// The catalog's JDK (or one like it when the catalog is not there): what a JDK is, for jdk_version and newest_jdk.
tools::Tool ht_jdk() {
    tools::Tool t;
    t.id = "java";
    t.value = "bin/java.exe";
    t.needs = {"bin/java.exe", "bin/javac.exe"};
    t.version_file = "release";
    t.version_key = "JAVA_VERSION";
    return t;
}

}  // namespace

std::optional<int> jdk_version(const stdfs::path& home) {
    const tools::Check c = tools::check(ht_jdk(), home);
    return c.ok ? c.version : std::nullopt;
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

std::vector<Found> scan(const stdfs::path& checkout_root) {
    const std::vector<tools::Tool> catalog = tools::catalog_of(checkout_root);
    std::vector<Found> out;
    for (const tools::Detected& d : tools::scan(catalog))
        for (const tools::Tool& t : catalog)
            if (t.id == d.id && !t.setting.empty()) out.push_back({t.setting, d.value, d.how});
    return out;
}

}  // namespace cb::config::host_tools
