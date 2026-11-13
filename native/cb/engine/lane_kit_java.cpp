// cb/engine/lane_kit_java.cpp -- the Java lane, wired (see lane_kit.hpp).
//
// A Java program is a folder of source; its classes are its functions. The lane transpiles it to C++03 with the production assembly (java/production: the
// transpiler over the lane's data, javac's typed tree of the sources as the front end) and compiles each class with the host C++ compiler (setting
// host_cxx, else g++ on the PATH); where a JDK is set up the class's methods are also run beside the original in the JVM.
#include <cstdlib>
#include <mutex>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"
#include "cb/engine/lane_kit_internal.hpp"
#include "cb/java/probe.hpp"
#include "cb/java/production.hpp"
#include "cb/verify/lanes/java.hpp"

namespace cb::engine {

namespace stdfs = std::filesystem;

namespace {

// `name` (with .exe) in a folder of the PATH.
std::optional<stdfs::path> kit_find_on_path(std::string_view name) {
    const std::string path = fs::get_env("PATH").value_or("");
    std::error_code ec;
    for (const std::string_view folder : text::split(path, ';')) {
        if (folder.empty()) continue;
        for (const std::string candidate : {std::string(name) + ".exe", std::string(name)}) {
            const stdfs::path file = fs::from_text(folder) / candidate;
            if (stdfs::is_regular_file(file, ec)) return file;
        }
    }
    return std::nullopt;
}

// The host C++ compiler: setting host_cxx when it names a file, else g++ on the PATH.
std::optional<stdfs::path> kit_host_cxx(const config::Config& config) {
    std::error_code ec;
    if (const std::optional<stdfs::path> named = config.path("host_cxx"); named && stdfs::is_regular_file(*named, ec)) return named;
    return kit_find_on_path("g++");
}

}  // namespace

std::shared_ptr<const verify::lanes::Kind> LaneKit::java_lane() {
    {
        std::lock_guard hold(impl_->lock);
        if (impl_->java_) return impl_->java_;
    }
    const config::Config& config = impl_->config;
    if (!kit_host_cxx(config)) return nullptr;   // no compiler: the lane has no profile
    const java::JavaData data(config.checkout_root() / "codebreaker" / "java");
    std::error_code ec;
    if (!stdfs::is_directory(data.root(), ec)) return nullptr;

    // What the lane is told of where things are, when it needs them (a profile, a build).
    auto tools = [config, data]() -> verify::lanes::JavaLane::Tools {
        verify::lanes::JavaLane::Tools t;
        t.cxx = kit_host_cxx(config).value_or(stdfs::path{});
        if (const std::optional<stdfs::path> java = java::production::find_java(config)) {
            const stdfs::path sibling = java->parent_path() / "javac.exe";
            std::error_code code;
            const std::optional<stdfs::path> javac = stdfs::is_regular_file(sibling, code) ? std::optional<stdfs::path>(sibling) : kit_find_on_path("javac");
            if (javac) {   // (a JDK is a java with its javac)
                t.java = *java;
                t.javac = *javac;
            }
        }
        const std::string libs = config.text("java_libs");
        if (!libs.empty()) t.libraries = java::probe::list_libraries(fs::from_text(libs));
        t.shim_sources = data.path({"probe_shims"});
        return t;
    };
    // One conversion at a time: an assembly shares its profile sets and project hooks between conversions.
    std::shared_ptr<java::assembly::Assembly> assembly = java::production::make_assembly(config, data);
    auto turn = std::make_shared<std::mutex>();
    verify::lanes::JavaLane::Convert convert = [assembly, turn](const stdfs::path& java_dir, const stdfs::path& out_dir) {
        std::lock_guard hold(*turn);
        return assembly->converter(std::string(verify::lanes::JavaLane::kCppStandard))(java_dir, out_dir);
    };
    auto lane = std::make_shared<verify::lanes::JavaLane>(std::move(tools), std::move(convert));
    std::lock_guard hold(impl_->lock);
    return impl_->java_ = std::move(lane);
}

std::string_view LaneKit::java_missing() {
    return "the Java lane needs a C++ compiler (settings host_cxx, or g++ on the PATH) and the lane's data folder (codebreaker/java); a JDK (setting java) lets it also run the classes";
}

}  // namespace cb::engine
