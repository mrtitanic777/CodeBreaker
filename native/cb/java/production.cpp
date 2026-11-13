#include "cb/java/production.hpp"

#include "cb/core/fs.hpp"
#include "cb/decompile/jar.hpp"
#include "cb/java/probe.hpp"

namespace cb::java::production {

namespace stdfs = std::filesystem;

std::optional<stdfs::path> find_java(const cb::config::Config& config) {
    decompile::jar::Settings s;
    s.retromcp_jar = config.path("retromcp").value_or(stdfs::path{});
    s.java = config.path("java").value_or(stdfs::path{});
    return decompile::jar::find_java(s);
}

frontend::DumpTools dump_tools(const cb::config::Config& config, const JavaData& data) {
    frontend::DumpTools tools;
    if (const std::optional<stdfs::path> java = find_java(config)) {
        tools.java = *java;
        const stdfs::path sibling = java->parent_path() / "javac.exe";
        std::error_code ec;
        tools.javac = stdfs::is_regular_file(sibling, ec) ? sibling : stdfs::path("javac");
    }
    tools.treedump_source = config.checkout_root() / "codebreaker" / "translate" / "frontends" / "java" / "TreeDump.java";
    tools.state = config.state();
    // the libraries the game is attributed against (setting java_libs: LWJGL, the sound system), and the stand-ins for the classes no library
    // download has, ahead of them
    probe::Toolchain toolchain;
    toolchain.javac = tools.javac;
    toolchain.java = tools.java;
    const std::string libs = config.text("java_libs");
    if (!libs.empty()) toolchain.libraries = probe::list_libraries(fs::from_text(libs));
    toolchain.shim_sources = data.path({"probe_shims"});
    if (!toolchain.libraries.empty()) {
        if (const auto shims = probe::compile_shims(config.state() / "translate", toolchain)) tools.classpath.push_back(*shims);
    }
    for (const stdfs::path& jar : toolchain.libraries) tools.classpath.push_back(jar);
    return tools;
}

std::unique_ptr<assembly::Assembly> make_assembly(const cb::config::Config& config, const JavaData& data) {
    return std::make_unique<assembly::Assembly>(data, [config, data](Arena& arena, const stdfs::path& java_dir) {
        return frontend::lower_program(arena, java_dir, dump_tools(config, data));
    });
}

}  // namespace cb::java::production
