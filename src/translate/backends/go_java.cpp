// translate/backends/go_java.cpp -- the entry points of the Go back end for Java: the program's Go text, and building it (see go_java.hpp).

#include "translate/backends/go_java.hpp"

#include "core/fs.hpp"
#include "core/process.hpp"
#include "core/text.hpp"
#include "java/paths.hpp"
#include "translate/backends/go_java_internal.hpp"

namespace cb::translate::backends::go_java {

namespace stdfs = std::filesystem;

stdfs::path default_runtime_dir() { return java::JavaData::locate().root().parent_path() / "translate" / "runtime" / "go"; }

std::optional<stdfs::path> find_go() {
    const auto is_file = [](const stdfs::path& p) {
        std::error_code ec;
        return stdfs::is_regular_file(p, ec);
    };
#ifdef _WIN32
    constexpr char kSeparator = ';';
    const char* exe = "go.exe";
#else
    constexpr char kSeparator = ':';
    const char* exe = "go";
#endif
    if (const auto env = fs::get_env("CB_GO"); env && is_file(fs::from_text(*env))) return fs::from_text(*env);
    if (const auto path_var = fs::get_env("PATH")) {
        for (const std::string_view dir : text::split(*path_var, kSeparator)) {
            if (!dir.empty() && is_file(fs::from_text(dir) / exe)) return fs::from_text(dir) / exe;
        }
    }
    for (const char* home : {"C:/Program Files/Go/bin", "/usr/local/go/bin", "/usr/lib/go/bin"}) {
        if (is_file(fs::from_text(home) / exe)) return fs::from_text(home) / exe;
    }
    return std::nullopt;
}

std::string program(Arena& arena, std::span<ir::Node* const> units, const std::string& main) {
    detail::Program prog(arena, units, main);
    return prog.write();
}

namespace {

// The first error line of the compiler's log (Go's are `file:line:col: message`).
std::string first_error(const std::string& log) {
    for (const std::string_view line : text::lines(log)) {
        if (line.find(".go:") != std::string_view::npos) return std::string(line);
    }
    return log.substr(0, 300);
}

}  // namespace

Built build(Arena& arena, std::span<ir::Node* const> units, const stdfs::path& out, const BuildOptions& options) {
    std::string go_text;
    try {
        go_text = program(arena, units, options.main);
    } catch (const Decline& declined) {   // a program the back end does not write is an answer, not a failure
        return {std::nullopt, std::string("Decline: ") + declined.what()};
    }
    const stdfs::path runtime = options.runtime_dir.empty() ? default_runtime_dir() : options.runtime_dir;
    const stdfs::path exe = out / "program.exe";
    fs::make_dirs(out / "jrt");
    fs::write_text(out / "program.go", go_text);
    fs::write_text(out / "go.mod", "module javaprog\n\ngo 1.21\n");
    for (const stdfs::directory_entry& e : stdfs::directory_iterator(runtime)) {
        if (e.path().extension() == ".go") fs::write_text(out / "jrt" / e.path().filename(), fs::read_text(e.path()));
    }
    // (CB_GO_KEEP names a folder to leave a copy of the Go text and the compiler's log in, for reading what was written)
    const auto keep = fs::get_env("CB_GO_KEEP");
    if (keep) {
        fs::make_dirs(fs::from_text(*keep));
        fs::write_text(fs::from_text(*keep) / "program.go", go_text);
    }
    const std::optional<stdfs::path> go = options.compiler ? options.compiler : find_go();
    if (!go) return {std::nullopt, "does not compile: there is no go"};
    const auto run_go = [&](std::vector<std::string> args, process::ProcessResult& ran) -> std::string {
        process::ProcessSpec spec;
        spec.argv.push_back(fs::to_text(*go));
        for (std::string& a : args) spec.argv.push_back(std::move(a));
        spec.cwd = out;
        spec.env["GOFLAGS"] = "-mod=mod";
        spec.env["GOPROXY"] = "off";
        spec.env["GOTOOLCHAIN"] = "local";
        spec.env["CGO_ENABLED"] = "0";
        spec.err = process::Capture::Merge;
        spec.timeout = std::chrono::duration_cast<std::chrono::milliseconds>(options.timeout);
        try {
            ran = process::run(spec);
        } catch (const ToolError& e) {   // no compiler to run
            return std::string("does not compile: ") + e.what();
        }
        if (ran.timed_out) return "does not compile: go did not finish";
        return {};
    };
    process::ProcessResult formatted;   // (a tidy source file for whoever reads it; a failure here is no matter)
    (void)run_go({"fmt", "./..."}, formatted);
    process::ProcessResult built;
    if (const std::string err = run_go({"build", "-o", fs::to_text(exe), "."}, built); !err.empty()) return {std::nullopt, err};
    if (built.exit_code != 0) {
        const std::string log = text::sanitize_utf8(as_text(built.out));
        if (keep) fs::write_text(fs::from_text(*keep) / "go.log", log);
        return {std::nullopt, "does not compile: " + first_error(log).substr(0, 400)};
    }
    return {exe, ""};
}

}  // namespace cb::translate::backends::go_java
