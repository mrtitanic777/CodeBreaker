#include "java/paths.hpp"

#include <string>

#include "core/error.hpp"
#include "core/fs.hpp"

namespace cb::java {

namespace stdfs = std::filesystem;

JavaData JavaData::locate() {
    if (auto env = fs::get_env("CB_CHECKOUT_ROOT"); env && !env->empty()) {
        stdfs::path root = fs::from_text(*env) / "config" / "java";
        std::error_code ec;
        if (stdfs::is_directory(root, ec)) return JavaData(root);
    }
    std::error_code ec;
    stdfs::path dir = stdfs::current_path(ec);
    while (!ec && !dir.empty()) {
        stdfs::path candidate = dir / "config" / "java";
        if (stdfs::is_directory(candidate, ec)) return JavaData(candidate);
        stdfs::path parent = dir.parent_path();
        if (parent == dir) break;
        dir = parent;
    }
    throw NotFoundError(stdfs::path("config/java"), 0, "the Java lane's data folder was not found");
}

stdfs::path JavaData::path(std::initializer_list<std::string_view> parts) const {
    stdfs::path out = root_;
    for (std::string_view part : parts) out /= fs::from_text(part);
    return out;
}

}  // namespace cb::java
