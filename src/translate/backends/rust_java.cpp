// translate/backends/rust_java.cpp -- the entry points of the Rust back end for Java: the program's Rust text, and building it (see rust_java.hpp).

#include "translate/backends/rust_java.hpp"

#include "core/fs.hpp"
#include "core/process.hpp"
#include "core/text.hpp"
#include "java/paths.hpp"
#include "translate/backends/rust_java_internal.hpp"

namespace cb::translate::backends::rust_java {

namespace stdfs = std::filesystem;

stdfs::path default_runtime_dir() {
    // the Java lane's folder is codebreaker/java; the Rust runtime is beside it, under translate
    return java::JavaData::locate().root().parent_path() / "translate" / "runtime" / "rust";
}

std::optional<stdfs::path> find_rustc() {
    const auto is_file = [](const stdfs::path& p) {
        std::error_code ec;
        return stdfs::is_regular_file(p, ec);
    };
#ifdef _WIN32
    constexpr char kSeparator = ';';
    const char* exe = "rustc.exe";
    const char* home_var = "USERPROFILE";
#else
    constexpr char kSeparator = ':';
    const char* exe = "rustc";
    const char* home_var = "HOME";
#endif
    if (const auto env = fs::get_env("CB_RUSTC"); env && is_file(fs::from_text(*env))) return fs::from_text(*env);
    if (const auto path_var = fs::get_env("PATH")) {
        for (const std::string_view dir : text::split(*path_var, kSeparator)) {
            if (!dir.empty() && is_file(fs::from_text(dir) / exe)) return fs::from_text(dir) / exe;
        }
    }
    if (const auto home = fs::get_env(home_var)) {
        const stdfs::path cargo = fs::from_text(*home) / ".cargo" / "bin" / exe;
        if (is_file(cargo)) return cargo;
    }
    return std::nullopt;
}

std::string program(Arena& arena, std::span<ir::Node* const> units, const std::string& main) {
    detail::Program prog(arena, units, main);
    return prog.write();
}

Built build(Arena& arena, std::span<ir::Node* const> units, const stdfs::path& out, const BuildOptions& options) {
    std::string rust_text;
    try {
        rust_text = program(arena, units, options.main);
    } catch (const Decline& declined) {   // a program the back end does not write is an answer, not a failure
        return {std::nullopt, std::string("Decline: ") + declined.what()};
    }
    const stdfs::path runtime = options.runtime_dir.empty() ? default_runtime_dir() : options.runtime_dir;
    const stdfs::path src = out / "program.rs";
    const stdfs::path exe = out / "program.exe";
    fs::make_dirs(out / "runtime");
    fs::write_text(src, rust_text);
    fs::write_text(out / "runtime" / "jrt.rs", fs::read_text(runtime / "jrt.rs"));
    // (CB_RUST_KEEP names a folder to leave a copy of the Rust text and the compiler's log in, for reading what was written)
    const auto keep = fs::get_env("CB_RUST_KEEP");
    if (keep) {
        fs::make_dirs(fs::from_text(*keep));
        fs::write_text(fs::from_text(*keep) / "program.rs", rust_text);
    }
    const std::optional<stdfs::path> rustc = options.compiler ? options.compiler : find_rustc();
    if (!rustc) return {std::nullopt, "does not compile: there is no rustc"};
    process::ProcessSpec spec;
    spec.argv.push_back(fs::to_text(*rustc));
    for (const std::string_view flag : kFlags) spec.argv.emplace_back(flag);
    for (const std::string& arg : {fs::to_text(src), std::string("-o"), fs::to_text(exe)}) spec.argv.push_back(arg);
    spec.err = process::Capture::Merge;
    spec.timeout = options.timeout;
    process::ProcessResult ran;
    try {
        ran = process::run(spec);
    } catch (const ToolError& e) {   // no compiler to run
        return {std::nullopt, std::string("does not compile: ") + e.what()};
    }
    if (ran.timed_out) return {std::nullopt, "does not compile: rustc did not finish"};
    if (ran.exit_code != 0) {
        const std::string log = text::sanitize_utf8(as_text(ran.out));
        if (keep) fs::write_text(fs::from_text(*keep) / "rustc.log", log);
        std::string first = log.substr(0, 300);
        for (const std::string_view line : text::lines(log)) {
            if (line.starts_with("error")) {
                first = std::string(line);
                break;
            }
        }
        return {std::nullopt, "does not compile: " + first.substr(0, 400)};
    }
    return {exe, ""};
}

}  // namespace cb::translate::backends::rust_java
