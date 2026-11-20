// java/port.hpp -- `codebreaker java port <input> [--platform pc|xbox360]`: a whole Java program to a C++ project from one Run.
//
// One input, whatever a person has at hand:
//     a Java source folder           (.java files under it)                 converted as it is
//     a decompiled game folder       (jars/, src/, resources/ ... under it) its source, jar, sounds and language file found by project_scan
//     a source .jar                  (.java entries)                        unpacked, then converted
//     a game .jar                    (classes only)                         decompiled first (its Minecraft version read from the file name)
// and one choice: PC (OpenGL/SDL, a CMake project) or Xbox 360 (a Visual Studio solution with the platform config, Content, the XACT audio banks and
// the achievements). The steps are chained by the `port` tool (java/tools.cpp); this module is the part that decides, with no side effect but the
// unpacking of a source jar, so the decisions are tested on their own (port_test.cpp).
//
//     const port::Plan p = port::plan("C:/Games/a1.2.6.jar", port::Platform::Xbox360);
//     // p.kind == ClassJar, p.mc_version == "a1.2.6", p.root == "C:/Games/a1.2.6-converted", p.project == p.root / "a1_2_6"
#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace cb::java::port {

enum class Platform { Pc, Xbox360 };

// "pc", "pc_opengl", "xbox360", "xbox 360", "360" (any case); nullopt for anything else.
std::optional<Platform> parse_platform(std::string_view text);
// The transpiler's platform id: "pc_opengl" or "xbox360".
std::string_view platform_id(Platform platform);
std::string_view platform_title(Platform platform);   // "PC" or "Xbox 360"

enum class SourceKind { JavaTree, GameFolder, SourceJar, ClassJar, Unknown };
std::string_view kind_title(SourceKind kind);

struct Plan {
    SourceKind kind = SourceKind::Unknown;
    Platform platform = Platform::Pc;
    std::filesystem::path input;                       // absolute
    std::optional<std::filesystem::path> java_src;     // the source root, when the input already has one
    std::optional<std::filesystem::path> jar;          // the game's jar: its textures and resources become Content
    std::optional<std::filesystem::path> resources;    // the .ogg sound tree
    std::optional<std::filesystem::path> lang;         // a language file (achievement names)
    std::string mc_version;                            // ClassJar: the Minecraft version to decompile ("" when the name does not say)
    std::string name;                                  // the project's name: an identifier made from the input's name
    std::filesystem::path root;                        // the solution folder: <input's folder>/<input's stem>-converted
    std::filesystem::path project;                     // root / name: the converted source and its project
    std::vector<std::string> steps;                    // what the run will do, in order, in plain words
    std::string problem;                               // not empty: the run cannot start, and why
};

// The plan for `input`. `out_root` replaces the default solution folder.
Plan plan(const std::filesystem::path& input, Platform platform, const std::optional<std::filesystem::path>& out_root = std::nullopt);

// A Minecraft version id in a jar's file stem ("a1.2.6", "minecraft-b1.7.3", "c0.30_01c", "inf-20100618", "1.2.5"); "" when there is none.
std::string version_from_name(std::string_view stem);

// An identifier for a project from a file or folder name: letters, digits and '_' (others become '_'), never starting with a digit.
std::string project_name(std::string_view stem);

// How many entries of the jar are Java source (.java) and how many are classes.
struct JarCounts {
    std::size_t java = 0, classes = 0;
};
JarCounts count_jar(const std::filesystem::path& jar);

// Writes every .java entry of the jar under `dest` (its package folders kept); returns how many.
std::size_t extract_java_sources(const std::filesystem::path& jar, const std::filesystem::path& dest);

}  // namespace cb::java::port
