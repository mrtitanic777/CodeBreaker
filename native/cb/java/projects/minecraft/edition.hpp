// cb/java/projects/minecraft/edition.hpp -- edition resolution: pick the per-era target profile for a conversion.
//
// An "edition" names one of the six Minecraft eras; each maps to a registered target profile (rubydung ->
// minecraft_rubydung, ...). resolve() chooses one by precedence:
//
//     explicit --edition flag   (user override, highest)
//       -> version-id mapping    (when --mc-version names a version)
//       -> source auto-detection (heuristic over the parsed corpus)
//       -> nothing               (nothing conclusive -> the caller keeps the default profile, a no-op)
//
// Answering "nothing" on the uncertain path is deliberate: the conversion then never switches profile, so the
// default (mixture) profile and today's exact output are preserved.
//
// The module is pure and read-only: no SDK, no network, no decompile. Auto-detection runs on the already-parsed
// corpus, so it costs nothing extra.
#pragma once

#include <array>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>

#include "cb/java/ir.hpp"

namespace cb::java::projects::minecraft::edition {

enum class Edition { RubyDung, Indev, Infdev, Alpha, Beta, Release };

inline constexpr std::array<Edition, 6> kEditions = {Edition::RubyDung, Edition::Indev, Edition::Infdev,
                                                      Edition::Alpha,    Edition::Beta,  Edition::Release};

// The short name an edition has on the command line ("alpha"), and the target profile registered for it
// ("minecraft_alpha").
std::string_view edition_name(Edition e);
std::string_view profile_name(Edition e);

// An edition from its short name ("alpha") or its profile name ("minecraft_alpha"); nullopt for anything else.
std::optional<Edition> parse_edition(std::string_view name);

// The edition a decompiler version id (a1.2.6, b1.6.6, c0.30_01c, in-20100223, inf-20100618, 1.2.5, rd-132211 ...)
// belongs to, or nullopt if the id is not recognised.
std::optional<Edition> edition_for_version(std::string_view version_id);

// What the auto-detection reads of a parsed corpus: the class names and the packages that appear in it.
struct CorpusIndex {
    std::set<std::string, std::less<>> names;
    std::set<std::string, std::less<>> packages;

    void add(std::string_view name, const std::optional<std::string>& package);
    static CorpusIndex of(std::span<const ir::JClass> classes);
};

// A best-effort edition guess from the parsed corpus, or nullopt when nothing is conclusive.
//
// Conservative on purpose: it would rather answer nullopt (keep the default profile) than mis-detect. The decisive
// axis is CHUNK STREAMING, not the world class name -- the real Indev decompile names its finite world class `World`
// (in net.minecraft.game.level) and has NO Chunk/ChunkProvider at all, so "World present" alone does NOT mean Alpha.
// Markers:
//   * com.mojang.rubydung package / RubyDung class                               -> rubydung (pre-classic)
//   * net.minecraft.server split / block-state classes                          -> release
//   * BiomeGenBase / RegionFile / AnvilChunkLoader / beds, cake, dye, stats ...  -> beta
//   * has Chunk/ChunkProvider (streamed world), no biome markers:
//         net.minecraft.game.* layout -> infdev ; flat net.minecraft.src -> alpha
//   * NO chunk streaming (finite world), net.minecraft.game.level / LevelLoader -> indev
std::optional<Edition> detect_edition(const CorpusIndex& corpus);

// The target-profile name to activate, or nullopt to keep the current default.
//
// Precedence: explicit edition > version-id mapping > source auto-detection > nothing. `explicit_edition` may be a
// short edition name ("alpha") or an already-qualified profile name ("minecraft_alpha"); both resolve. An explicit
// value that names no edition is a UsageError (it is surfaced, not silently ignored).
std::optional<std::string> resolve(std::string_view explicit_edition = {}, std::string_view mc_version = {},
                                   const CorpusIndex* corpus = nullptr);

}  // namespace cb::java::projects::minecraft::edition
