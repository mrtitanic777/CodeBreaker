// translate/backends/c_java.cpp -- the entry points of the C back end for Java: the program's C text, and building it (see c_java.hpp).

#include "translate/backends/c_java.hpp"

#include "core/fs.hpp"
#include "core/process.hpp"
#include "core/text.hpp"
#include "java/paths.hpp"
#include "translate/backends/c_java_internal.hpp"

namespace cb::translate::backends::c_java {

namespace stdfs = std::filesystem;

stdfs::path default_runtime_dir() {
    // the Java lane's folder is codebreaker/java; the C runtime is beside it, under translate
    return java::JavaData::locate().root().parent_path() / "translate" / "runtime" / "c";
}

std::string program(Arena& arena, std::span<ir::Node* const> units, const std::optional<std::string>& main) {
    detail::Program prog(arena, units, main);
    std::string text = prog.write();
    if (text.find("jrt_Character_getType_C(") == std::string::npos) return text;
    // Java's Unicode categories are a large table: only a program that asks for them carries it
    const std::string table = fs::read_text(default_runtime_dir().parent_path() / "java_char_types.inc");
    const std::string include = "#include \"jrt.h\"\n";
    const std::size_t at = text.find(include);
    CB_ASSERT(at != std::string::npos, "the program includes its runtime");
    text.insert(at + include.size(),
                "static const uint16_t jrt_char_types[] = {\n" + table + "};\n"
                "static int32_t jrt_Character_getType_C(char16_t c) {\n"
                "\tsize_t low = 0, high = sizeof(jrt_char_types) / sizeof(jrt_char_types[0]) / 2;\n"
                "\twhile (high - low > 1) {\n"
                "\t\tconst size_t middle = low + (high - low) / 2;\n"
                "\t\tif (jrt_char_types[middle * 2] <= c) low = middle; else high = middle;\n"
                "\t}\n"
                "\treturn jrt_char_types[low * 2 + 1];\n"
                "}\n");
    return text;
}

Built build(Arena& arena, std::span<ir::Node* const> units, const stdfs::path& out, const BuildOptions& options) {
    std::string c_text;
    try {
        c_text = program(arena, units);
    } catch (const Decline& declined) {   // a program the back end does not write is an answer, not a failure
        return {std::nullopt, std::string("Decline: ") + declined.what()};
    }
    const stdfs::path runtime = options.runtime_dir.empty() ? default_runtime_dir() : options.runtime_dir;
    const stdfs::path src = out / "program.c";
    const stdfs::path exe = out / "program.exe";
    fs::write_text(src, c_text);
    const stdfs::path clang = fs::from_text(std::string(kClang));
    const std::string compiler = options.compiler ? fs::to_text(*options.compiler) : (stdfs::is_regular_file(clang) ? std::string(kClang) : "gcc");
    process::ProcessSpec spec;
    spec.argv.push_back(compiler);
    for (const std::string_view flag : kFlags) spec.argv.emplace_back(flag);
    for (const std::string& arg : {std::string("-I"), fs::to_text(runtime), fs::to_text(src), fs::to_text(runtime / "jrt.c"),
                                   std::string("-o"), fs::to_text(exe)})
        spec.argv.push_back(arg);
    spec.err = process::Capture::Merge;
    spec.timeout = options.timeout;
    process::ProcessResult ran;
    try {
        ran = process::run(spec);
    } catch (const ToolError& e) {   // no compiler to run
        return {std::nullopt, std::string("does not compile: ") + e.what()};
    }
    if (ran.timed_out) return {std::nullopt, "does not compile: the C compiler did not finish"};
    if (ran.exit_code != 0) {
        const std::string log = text::sanitize_utf8(as_text(ran.out));
        std::string first = log.substr(0, 300);
        for (const std::string_view line : text::lines(log)) {
            if (line.find("error") != std::string_view::npos) {
                first = std::string(line);
                break;
            }
        }
        return {std::nullopt, "does not compile: " + first.substr(0, 400)};
    }
    return {exe, ""};
}

}  // namespace cb::translate::backends::c_java
