// java/projects/minecraft/prep/prepare.hpp -- a Minecraft version from nothing to its Java source, natively: what RetroMCP's setup and decompile
// tasks did (RetroMCP is retired). The workspace is RetroMCP's layout, so what read it before reads it still.
//
//     prep::PrepareOptions o;
//     o.gl_constants = config / "java/projects/minecraft/gl_constants.json";
//     o.on_output = [](const std::string& line) { show(line); };
//     const prep::PrepareResult r = prep::prepare("a1.2.6", workspace, o);   // workspace/minecraft/src, workspace/minecraft_server/src
//
// The steps, each RetroMCP's, in its order:
//   1. setup (setup.hpp): the version's jars, libraries, natives and conf/ (mappings, exceptions, access, patches) downloaded.
//   2. for each side whose jar is there (TaskDecompile, side ANY: the client, then the server; MERGED: the two in one):
//      a. the side's folder emptied;
//      b. the classes read, remapped and fixed (TaskDecompile.applyInjector: the mappings, decomposeVars, replaceCommonConstants, the GL constants
//         for the client, restoreSourceFile, fixInnerClasses, fixImplicitConstructors, guessGenerics when asked, the exceptions, the access) and
//         written to <side>/jars/deobfuscated.jar;
//      c. decompiled by CodeBreaker's Fernflower (java/decompiler) with RetroMCP's options and the version's libraries into <side>/src_original;
//      d. patched with conf/<side>.patch (patch.hpp);
//      e. the source kept: the files that are not .java dropped (unless resources are asked for), the source jar written
//         (<side>/jars/deobfuscated-source.jar), the ignored packages removed, the rest copied to <side>/src; the client's assets copied to its game
//         folder.
//   Not done, as nothing of CodeBreaker's uses them: RetroMCP's IDE projects (Eclipse, IntelliJ, VS Code) and its recompile-and-MD5 stage (that
//   needed javac, for RetroMCP's reobfuscation).
#pragma once

#include <filesystem>
#include <functional>
#include <optional>
#include <string>
#include <vector>

#include "java/projects/minecraft/prep/setup.hpp"
#include "java/projects/minecraft/prep/versions.hpp"

namespace cb::java::minecraft::prep {

struct PrepareOptions {
    Side side = Side::Any;               // RetroMCP's `side` (ANY: each side the version has)
    bool setup = true;                   // false: the workspace is set up already (decompile it again)
    bool patches = true;                 // RetroMCP's `patch`
    bool decompile_resources = false;    // `resources`: keep the files that are not .java
    bool guess_generics = false;         // `generics`
    bool strip_generics = false;         // `stripgenerics`
    bool output_src = true;              // `outputsrc`
    std::vector<std::string> ignored_packages = {"paulscode", "com/jcraft", "de/jarnbjo", "isom", "ibxm", "de/matthiasmann/twl", "javax/xml", "javax/ws",
                                                 "argo", "org/bouncycastle", "org/apache/commons", "com/google/gson", "com/google/common"};   // `ignore`
    std::filesystem::path gl_constants;  // RetroMCP's gl_constants.json (config/java/projects/minecraft); none: the GL constants are not named
    SetupOptions setup_options;
    std::function<void(const std::string& line)> on_output;
};

struct PrepareResult {
    std::filesystem::path workspace;
    std::vector<std::filesystem::path> sources;   // each side's src folder, in the order made
    std::vector<std::string> messages;            // what went wrong without stopping (a patch that failed, a class Fernflower could not write ...)
};

// The version prepared in `workspace`. Throws (core errors, std::runtime_error) for what stops it: no such version, a download that failed, a jar
// that cannot be read.
PrepareResult prepare(const std::string& version, const std::filesystem::path& workspace, const PrepareOptions& options = {});

// Step 2 for one side of a set-up workspace (`side` Client, Server or Merged); the side's src folder.
std::filesystem::path decompile_side(const std::filesystem::path& workspace, Side side, const PrepareOptions& options, std::vector<std::string>& messages);

}  // namespace cb::java::minecraft::prep
