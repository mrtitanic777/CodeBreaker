// java/console/retromcp.hpp -- a RetroMCP workspace found from any folder inside it: the game's whole source, its jar, its libraries and its entry.
//
// RetroMCP (MCPHackers) sets a Minecraft version up and decompiles it into a workspace of this shape:
//
//     <root>/options.cfg                        its settings; "setup=in-20100213" names the version that was set up
//     <root>/conf/version.json                  the version's manifest: "id", the launcher's "mainClass", the libraries
//     <root>/conf/mappings.tiny ...             the mappings, patches and exceptions it applied
//     <root>/libraries/<maven path>/*.jar       the libraries the game builds against (LWJGL, JInput, the sound system), -sources jars beside them
//     <root>/libraries/natives/                 their DLLs
//     <root>/minecraft/                         the client project (minecraft_server/ for the server):
//         src/<packages>/*.java                 the source the person builds and edits -- the one that is converted
//         src_original/                         the source as decompiled, kept to diff against (never converted)
//         jars/deobfuscated.jar                 the game's classes and resources, remapped (textures, sounds: the game's Content)
//         md5/original.md5, bin/, game/         the class checksums, Eclipse's output, the game's run folder (assets)
//         .classpath .project minecraft.iml     the IDE projects (the libraries are ../libraries)
//
// find() climbs from the folder a person dropped (the root, the project, or any folder inside either) to the root, and resolves every part
// without anyone picking subfolders. Nothing is written.
#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace cb::java::console::retromcp {

struct Workspace {
    std::filesystem::path root;                        // the folder with options.cfg and conf/
    std::filesystem::path project;                     // root/minecraft (client) or root/minecraft_server
    std::string side;                                  // "client" or "server"
    std::filesystem::path java_src;                    // project/src: the source root (the packages are below it)
    std::size_t java_files = 0;                        // the .java files under java_src
    std::optional<std::filesystem::path> original_src; // project/src_original, when it is there
    std::optional<std::filesystem::path> jar;          // project/jars/deobfuscated.jar (else the project's game jar)
    std::vector<std::filesystem::path> libraries;      // root/libraries/**.jar, neither -sources nor natives jars, in path order
    std::optional<std::filesystem::path> natives;      // root/libraries/natives
    std::string version;                               // "in-20100213": options.cfg's setup=, else version.json's id
    std::string launcher_main;                         // version.json's mainClass ("org.mcphackers.launchwrapper.Launch")
    std::string entry_class;                           // the game's own entry in the source: a main(String[]) class, else an Applet
};

// The closest folder at or above `dir` (at most 8 up) that is a RetroMCP workspace's root: options.cfg with a setup= line or conf/version.json,
// and a project folder (minecraft or minecraft_server) holding Java source in src/. nullopt when there is none.
std::optional<std::filesystem::path> find_root(const std::filesystem::path& dir);

// The workspace `dropped` is in (the folder itself, or the folder of a file), with every part resolved. The project is the one the dropped
// folder is inside; dropped at the root, the client's (minecraft), else the server's. nullopt when `dropped` is not inside a workspace.
std::optional<Workspace> find(const std::filesystem::path& dropped);

// The game's entry class in a source root, as a dotted name: a class declaring `static void main(String`, else one that `extends Applet` or
// `extends JApplet` directly; the first in path order. "" when there is neither.
std::string find_entry_class(const std::filesystem::path& java_src);

}  // namespace cb::java::console::retromcp
