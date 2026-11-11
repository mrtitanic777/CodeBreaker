// cb/decompile/jar.hpp -- the RetroMCP decompiler backend.
//
// CodeBreaker drives the RetroMCP CLI (native/retromcp: MIT, MCPHackers; built from that source, settings.json
// `retromcp` names another jar) as a headless backend so a Minecraft version can be decompiled straight into Java
// source and fed into the transpiler -- no manual round-trip through the RetroMCP GUI. The decompile step needs a
// Java runtime and network access (RetroMCP downloads the version + mappings on first setup). The C++ transpiler
// itself needs neither.
//
// Pipeline, run inside a workspace directory:
//     java -jar retromcp-cli.jar setup setup=<version>     # download + prepare the workspace
//     java -jar retromcp-cli.jar decompile                 # decompile -> <workspace>/minecraft_*/src
//
//     fs::path src = jar::decompile("a1.2.6", workspace, {.settings = settings, .on_output = show});
#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/error.hpp"

namespace cb::decompile::jar {

// Why a decompile could not be done: no Java, no backend jar, a version the manifest does not know, a step that
// failed, or a run that produced no source. The message says which and is meant to be shown to the user.
class DecompileError : public Error {
public:
    using Error::Error;
    CB_ERROR_BOILERPLATE(DecompileError)
};

// The two settings.json entries the backend reads, handed in by the caller (empty = not set).
struct Settings {
    std::filesystem::path retromcp_jar;   // `retromcp`: the RetroMCP CLI jar
    std::filesystem::path java;           // `java`: a java executable to prefer
};

// Called with each progress line (the commands run, the backend's own output, the summary).
using OutputFn = std::function<void(std::string_view line)>;

// Locate a `java` executable. Order: `explicit_java`, settings.java, $JAVA_HOME/bin/java.exe, PATH. A candidate
// counts only if the file exists. nullopt when there is none.
std::optional<std::filesystem::path> find_java(const Settings& settings,
                                               const std::optional<std::filesystem::path>& explicit_java = std::nullopt);

// True if both a Java runtime and the backend jar are present.
bool available(const Settings& settings);

// Locate the decompiled source directory under `workspace`: the directory named `src` holding the most `.java`
// files (the first found, in name order, when two tie); nullopt when no `src` holds any.
std::optional<std::filesystem::path> find_src(const std::filesystem::path& workspace);

// Splits a child's output into lines the way a text-mode pipe does: "\n", "\r" and "\r\n" each end a line (the
// terminator is not part of it), and a last line without one still counts. Feed it the chunks as they arrive.
class LineSplitter {
public:
    // Calls `emit` for each line completed by `chunk`.
    void feed(ByteView chunk, const std::function<void(std::string_view line)>& emit);
    // The unterminated last line, if any.
    void finish(const std::function<void(std::string_view line)>& emit);

private:
    std::string pending_;
};

struct RunResult {
    std::uint32_t exit_code = 0;
    std::vector<std::string> lines;   // everything the program wrote (stdout and stderr interleaved), as UTF-8
};

// Run `java -jar jar args...` in `cwd`: each output line goes to `on_output` as it arrives and to
// <cwd>/codebreaker-decompile.log (appended; a log that cannot be written is skipped). With `stdin_text` the text
// is fed to the program (RetroMCP's setup prompts for the version when it cannot resolve it from the argument);
// without, its input is empty. Throws ToolError when the program cannot be started.
RunResult run_java(const std::filesystem::path& java, const std::filesystem::path& jar,
                   const std::vector<std::string>& args, const std::filesystem::path& cwd,
                   const OutputFn& on_output = {}, const std::optional<std::string>& stdin_text = std::nullopt);

struct DecompileOptions {
    Settings settings;
    std::optional<std::filesystem::path> java;   // overrides settings.java and the search
    OutputFn on_output;
    bool setup = true;                           // false: skip the download/setup (re-decompile a set-up workspace)
};

// Decompile a Minecraft `version` (e.g. "a1.2.6", "c0.30_01c") into `workspace`. Returns the absolute path of the
// decompiled Java source directory, the `java_dir` input of java::transpile::convert; afterwards the mangled array
// parameters are repaired. Throws DecompileError on any failure.
std::filesystem::path decompile(std::string_view version, const std::filesystem::path& workspace,
                                const DecompileOptions& options);

// RetroMCP names an array parameter from its JVM descriptor in some versions (Indev 20100213: `Block[] block;4`,
// `byte[] [B4`), which is not Java, while Fernflower's body still calls it `var4` (`var4/* $VF was: block;4 */`).
// The parameter takes the name the body uses. Renames them in every .java under `src` (bytes are kept as they
// are; only a file that had one is rewritten) and returns how many parameters were renamed.
std::size_t repair_array_params(const std::filesystem::path& src);

// The same repair over one source text; `renamed` receives the count.
std::string repair_array_params_text(std::string_view source, std::size_t& renamed);

}  // namespace cb::decompile::jar
