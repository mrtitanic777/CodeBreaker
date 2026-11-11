// cb/java/console/project_scan.hpp -- from the one folder the user picks, resolve everything the conversion needs.
//
// A decompiled Minecraft tree keeps its parts in predictable places under a base folder, e.g.
//
//     <base>/jars/deobfuscated.jar          the textures/resource jar
//     <base>/game/resources/newsound/...    the .ogg music/sound tree
//     <base>/src/*.java  (or a source jar)  the Java source
//
// Hand-selecting each of those in the wizard is tedious, so given ONE folder (the base, or the src inside it) this
// finds the base and resolves the jar, the resources/music folder, the Java source and the language file -- so
// the later questionnaires (assets jar, audio resources, achievements lang) come pre-filled. The scan is
// depth-bounded and skips heavy build folders so it stays fast.
#pragma once

#include <filesystem>
#include <optional>

namespace cb::java::console::project_scan {

// Everything resolve() found. `base` is always set; the rest are empty when the folder has no such part.
struct Resolved {
    std::filesystem::path base;
    std::optional<std::filesystem::path> jar;
    std::optional<std::filesystem::path> resources;
    std::optional<std::filesystem::path> java_src;
    std::optional<std::filesystem::path> lang;
};

// Walk up from `project_dir` (at most 8 folders) and return the CLOSEST folder that looks like the decompiled base
// (directly contains jars, game or resources). Falls back to `project_dir`, made absolute.
std::filesystem::path find_base(const std::filesystem::path& project_dir);

// The textures/resource jar: deobfuscated.jar or minecraft.jar when present, else the first jar met (one under a
// "jars" folder is no more preferred than any other, as walked); never a *source*.jar.
std::optional<std::filesystem::path> find_jar(const std::filesystem::path& base);

// The audio/resource tree: a folder named "resources" that holds a newsound, sound, newmusic, music or streaming
// child -- the .ogg tree.
std::optional<std::filesystem::path> find_resources(const std::filesystem::path& base);

// The Java source ROOT (the folder the packages live under, e.g. "src" -- NOT a deep package such as
// src/net/minecraft/client). Prefers src, source or sources under the base; else the picked folder itself if it
// holds any .java below it.
std::optional<std::filesystem::path> find_java_src(const std::filesystem::path& project_dir,
                                                    const std::filesystem::path& base);

// A language file: any lang/en_US.lang, else the first *.lang under a "lang" folder.
std::optional<std::filesystem::path> find_lang(const std::filesystem::path& base);

// Resolve everything from the picked folder.
Resolved resolve(const std::filesystem::path& project_dir);

}  // namespace cb::java::console::project_scan
