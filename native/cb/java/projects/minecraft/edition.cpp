#include "cb/java/projects/minecraft/edition.hpp"

#include <format>

#include "cb/core/error.hpp"
#include "cb/core/text.hpp"

namespace cb::java::projects::minecraft::edition {

std::string_view edition_name(Edition e) {
    switch (e) {
        case Edition::RubyDung: return "rubydung";
        case Edition::Indev: return "indev";
        case Edition::Infdev: return "infdev";
        case Edition::Alpha: return "alpha";
        case Edition::Beta: return "beta";
        case Edition::Release: return "release";
    }
    CB_ASSERT(false, "unknown Edition");
    return {};
}

std::string_view profile_name(Edition e) {
    switch (e) {
        case Edition::RubyDung: return "minecraft_rubydung";
        case Edition::Indev: return "minecraft_indev";
        case Edition::Infdev: return "minecraft_infdev";
        case Edition::Alpha: return "minecraft_alpha";
        case Edition::Beta: return "minecraft_beta";
        case Edition::Release: return "minecraft_release";
    }
    CB_ASSERT(false, "unknown Edition");
    return {};
}

std::optional<Edition> parse_edition(std::string_view name) {
    for (Edition e : kEditions) {
        if (name == edition_name(e) || name == profile_name(e)) return e;
    }
    return std::nullopt;
}

namespace {

bool ver_is_digit(char c) { return c >= '0' && c <= '9'; }

// "^\d+\.\d": digits, a dot, a digit (1.2.5).
bool ver_is_release(std::string_view v) {
    std::size_t i = 0;
    while (i < v.size() && ver_is_digit(v[i])) ++i;
    return i > 0 && i + 1 < v.size() && v[i] == '.' && ver_is_digit(v[i + 1]);
}

// "^c0?\.": c. or c0. (classic builds, c0.30_01c)
bool ver_is_classic(std::string_view v) {
    if (v.size() < 2 || (v[0] != 'c' && v[0] != 'C')) return false;
    std::string_view rest = v.substr(1);
    if (rest.starts_with('0')) rest.remove_prefix(1);
    return rest.starts_with('.');
}

bool ver_is_separator(char c) { return c == '-' || c == '_'; }

// `prefix` followed by '-' or '_' (case-insensitively), e.g. "rd-" / "inf_".
bool ver_has_tag(std::string_view v, std::string_view prefix) {
    return v.size() > prefix.size() && text::istarts_with(v, prefix) && ver_is_separator(v[prefix.size()]);
}

}  // namespace

// The rules are tried in order and the first match wins, so the more specific prefixes come first: `inf-` before
// `in`, the era letters before the bare release-digit rule.
std::optional<Edition> edition_for_version(std::string_view version_id) {
    const std::string_view v = text::trim(version_id);
    if (v.empty()) return std::nullopt;
    if (ver_has_tag(v, "rd")) return Edition::RubyDung;               // rd-132211 (RubyDung)
    if (ver_is_classic(v)) return Edition::RubyDung;             // c0.30_01c (classic ~ pre-classic handling)
    if (text::istarts_with(v, "classic")) return Edition::RubyDung;
    if (ver_has_tag(v, "inf")) return Edition::Infdev;                // inf-20100618
    if (text::istarts_with(v, "infdev")) return Edition::Infdev;
    if (ver_has_tag(v, "in")) return Edition::Indev;                  // in-20100223
    if (text::istarts_with(v, "indev")) return Edition::Indev;
    if (text::istarts_with(v, "a")) return Edition::Alpha;        // a1.2.6
    if (text::istarts_with(v, "b")) return Edition::Beta;         // b1.6.6
    if (ver_is_release(v)) return Edition::Release;              // 1.2.5
    return std::nullopt;
}

void CorpusIndex::add(std::string_view name, const std::optional<std::string>& package) {
    if (!name.empty()) names.emplace(name);
    if (package && !package->empty()) packages.insert(*package);
}

CorpusIndex CorpusIndex::of(std::span<const ir::JClass> classes) {
    CorpusIndex out;
    for (const ir::JClass& c : classes) out.add(c.name, c.package);
    return out;
}

namespace {

bool corpus_has_any(const std::set<std::string, std::less<>>& names, std::initializer_list<std::string_view> wanted) {
    for (std::string_view w : wanted) {
        if (names.contains(w)) return true;
    }
    return false;
}

// The package `prefix` itself or any package below it.
bool corpus_has_package(const std::set<std::string, std::less<>>& packages, std::string_view prefix) {
    for (const std::string& p : packages) {
        if (p == prefix || (p.size() > prefix.size() && p.starts_with(prefix) && p[prefix.size()] == '.')) return true;
    }
    return false;
}

}  // namespace

std::optional<Edition> detect_edition(const CorpusIndex& corpus) {
    const auto& names = corpus.names;
    const auto& pkgs = corpus.packages;
    if (names.empty()) return std::nullopt;

    // Pre-classic: the package (or main class) is unmistakable.
    if (corpus_has_package(pkgs, "com.mojang.rubydung") || names.contains("RubyDung")) return Edition::RubyDung;

    bool has_chunk = names.contains("Chunk");
    for (auto it = names.lower_bound("ChunkProvider"); !has_chunk && it != names.end() && it->starts_with("ChunkProvider");
         ++it) {
        has_chunk = true;
    }

    // Release: dedicated-server package split or block-state classes.
    if (corpus_has_package(pkgs, "net.minecraft.server") ||
        corpus_has_any(names, {"IBlockState", "BlockState", "WorldProviderEnd"})) {
        return Edition::Release;
    }
    // Beta-ONLY classes. WorldChunkManager / ChunkProviderGenerate are NOT markers: Alpha 1.2.x already has both
    // (biomes arrived there, as MobSpawnerBase), so they misfiled Alpha corpora as beta. These are Beta additions:
    // beds/cake/note/dispenser/dye (b1.2-1.3), McRegion RegionFile (b1.3), stats and achievements (b1.5), the
    // BiomeGenBase rename (b1.8), Anvil (carried into release).
    if (corpus_has_any(names, {"BiomeGenBase", "RegionFile", "AnvilChunkLoader", "BlockBed", "BlockCake", "BlockNote",
                        "BlockDispenser", "ItemDye", "StatList", "Achievement"})) {
        return Edition::Beta;
    }
    // Chunk-streamed but pre-biome worlds: infdev (early game-package layout) vs alpha (flat src).
    if (has_chunk) {
        if (corpus_has_package(pkgs, "net.minecraft.game") && !corpus_has_package(pkgs, "net.minecraft.src")) return Edition::Infdev;
        return Edition::Alpha;
    }
    // Finite-world era (no chunk streaming). Real Indev = net.minecraft.game.level.World + LevelLoader.
    if (corpus_has_package(pkgs, "net.minecraft.game.level") || names.contains("LevelLoader")) return Edition::Indev;
    if ((names.contains("World") || names.contains("Level")) && corpus_has_any(names, {"Block", "Material", "Tile"})) {
        return Edition::Indev;
    }
    return std::nullopt;
}

std::optional<std::string> resolve(std::string_view explicit_edition, std::string_view mc_version,
                                   const CorpusIndex* corpus) {
    // 1. Explicit override.
    if (!explicit_edition.empty()) {
        if (const auto e = parse_edition(explicit_edition)) return std::string(profile_name(*e));
        std::set<std::string_view> sorted;   // the usage text lists the choices sorted
        for (Edition e : kEditions) sorted.insert(edition_name(e));
        throw UsageError(std::format("unknown edition '{}' (choices: {})", explicit_edition, text::join(sorted, ", ")));
    }
    // 2. Version-id mapping.
    if (const auto e = edition_for_version(mc_version)) return std::string(profile_name(*e));
    // 3. Source auto-detection.
    if (corpus) {
        if (const auto e = detect_edition(*corpus)) return std::string(profile_name(*e));
    }
    // 4. Nothing conclusive -> the caller keeps the default profile (a strict no-op).
    return std::nullopt;
}

}  // namespace cb::java::projects::minecraft::edition
