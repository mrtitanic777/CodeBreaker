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
//         md5/original.md5, bin/, game/         the class checksums, Eclipse's output, the game's run folder
//         game/assets/indexes/<id>.json         the sounds and music the launcher downloaded: name -> hash (version.json's assetIndex)
//         game/assets/objects/<h2>/<hash>       ... their files, by hash
//         .classpath .project minecraft.iml     the IDE projects (the libraries are ../libraries)
//
// find() climbs from the folder a person dropped (the root, the project, or any folder inside either) to the root, and resolves every part
// without anyone picking subfolders. Nothing is written.
#pragma once

#include <filesystem>
#include <functional>
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
    std::optional<std::filesystem::path> asset_index;  // project/game/assets/indexes/<version.json's assetIndex id>.json (else the only one)
};

// What copy_resources did: the files copied, those fetched first (and stored in the workspace), and the ones still missing.
struct ResourceCopy {
    std::size_t copied = 0;
    std::size_t fetched = 0;
    std::size_t missing = 0;
    std::vector<std::string> failed;   // "music/calm1.ogg: <why>": a fetch that failed or whose bytes were not the index's
};

// The body at a URL (http::get); throws when there is none.
using Fetch = std::function<std::string(const std::string& url)>;

// The workspace's sounds and music under `dest` by their own names (newsound/step/grass1.ogg ...), from the asset index's objects: the
// tree the game's minecraft.net/resources/ held. Nothing when the workspace has no index.
// `fetch` set: an entry the workspace never downloaded that names its own url (RetroMCP's "custom" section: Indev's music, where its
// launcher fetches it from) and is a sound (.ogg, .wav, .mus) is fetched, checked against the index's SHA-1, and stored in the workspace's
// game/assets/objects as RetroMCP's launcher would, then copied. Without it nothing leaves the computer.
ResourceCopy copy_resources(const Workspace& workspace, const std::filesystem::path& dest, const Fetch& fetch = {});

// The closest folder at or above `dir` (at most 8 up) that is a RetroMCP workspace's root: options.cfg with a setup= line or conf/version.json,
// and a project folder (minecraft or minecraft_server) holding Java source in src/. nullopt when there is none.
std::optional<std::filesystem::path> find_root(const std::filesystem::path& dir);

// The workspace `dropped` is in (the folder itself, or the folder of a file), with every part resolved. The project is the one the dropped
// folder is inside; dropped at the root, the client's (minecraft), else the server's. nullopt when `dropped` is not inside a workspace.
std::optional<Workspace> find(const std::filesystem::path& dropped);

// The game's entry class in a source root, as a dotted name: a class declaring `static void main(String`, else one that `extends Applet` or
// `extends JApplet` directly; the first in path order. "" when there is neither.
std::string find_entry_class(const std::filesystem::path& java_src);

// A field the workspace's mappings name on a class whose source does not hold it: RetroMCP renamed the references (`x.named`) but the
// field is declared on an ancestor, still under its obfuscated name, so the decompiled source does not compile (Beta 1.6's
// EntityLiving.numTicksToChaseTarget is Entity's `aC`, the entity id).
struct FieldRepair {
    std::string owner;      // the class the mapping names it on (EntityLiving)
    std::string named;      // the mapped name the references use (numTicksToChaseTarget)
    std::string declared;   // the name the declaration still has (aC)
    std::string holder;     // the class that declares it (Entity)
};

// The field mappings of `tiny` (a mappings.tiny v2, the `side` column: "client" / "server") that the source under `java_src` breaks so:
// the mapped name is declared as a field nowhere in the source and is referenced (`.named`), and the owner or one of its ancestors
// declares the obfuscated name. A name two repairs disagree on is left alone. Sorted by owner, then name.
std::vector<FieldRepair> find_field_repairs(const std::filesystem::path& tiny, const std::filesystem::path& java_src, const std::string& side);

// Copies the .java files under `java_src` to `dest` with each repair's references (`.named`, not a call `.named(`) naming the declared
// field. Returns how many files changed.
std::size_t write_repaired_source(const std::filesystem::path& java_src, const std::filesystem::path& dest, const std::vector<FieldRepair>& repairs);

}  // namespace cb::java::console::retromcp
