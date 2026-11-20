// decompile/jar.hpp -- a Minecraft version decompiled to Java source, natively.
//
// The preparation RetroMCP used to do (download the version, remap its classes with its mappings, fix them up, patch the source) is CodeBreaker's
// own (java/projects/minecraft/prep), and the decompiler is CodeBreaker's Fernflower (java/decompiler): no Java and no RetroMCP. The workspace is
// RetroMCP's layout (<workspace>/minecraft/src, <workspace>/minecraft_server/src ...), so what read it before reads it still.
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

#include "core/bytes.hpp"
#include "core/error.hpp"

namespace cb::decompile::jar {

// Why a decompile could not be done: a version the list does not know, a download or a step that failed, or a run that produced no source. The
// message says which and is meant to be shown to the user.
class DecompileError : public Error {
public:
    using Error::Error;
    CB_ERROR_BOILERPLATE(DecompileError)
};

// What the caller hands in (empty = not set).
struct Settings {
    std::filesystem::path java;           // settings.json `java`: a java executable to prefer (find_java; the translator's front end needs a JDK)
    std::filesystem::path gl_constants;   // the GL constants' names (config/java/projects/minecraft/gl_constants.json); none: not named
};

// Called with each progress line (the commands run, the backend's own output, the summary).
using OutputFn = std::function<void(std::string_view line)>;

// Locate a `java` executable. Order: `explicit_java`, settings.java, $JAVA_HOME/bin/java.exe, PATH. A candidate
// counts only if the file exists. nullopt when there is none.
std::optional<std::filesystem::path> find_java(const Settings& settings,
                                               const std::optional<std::filesystem::path>& explicit_java = std::nullopt);

// Locate the decompiled source directory under `workspace`: the directory named `src` holding the most `.java`
// files (the first found, in name order, when two tie); nullopt when no `src` holds any.
std::optional<std::filesystem::path> find_src(const std::filesystem::path& workspace);

struct DecompileOptions {
    Settings settings;
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
