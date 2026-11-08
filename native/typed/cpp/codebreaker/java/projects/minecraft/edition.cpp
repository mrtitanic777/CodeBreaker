// codebreaker/java/projects/minecraft/edition.cpp -- typed C++ port of codebreaker.java.projects.minecraft.edition
// (see the header).

#include "codebreaker/java/projects/minecraft/edition.hpp"

namespace codebreaker::java::projects::minecraft::edition {

const py::dict<py::str, py::str> EDITIONS = {
    {py::str("rubydung"), py::str("minecraft_rubydung")},
    {py::str("indev"), py::str("minecraft_indev")},
    {py::str("infdev"), py::str("minecraft_infdev")},
    {py::str("alpha"), py::str("minecraft_alpha")},
    {py::str("beta"), py::str("minecraft_beta")},
    {py::str("release"), py::str("minecraft_release")},
};

const py::list<std::tuple<py::re::Pattern, py::str>> _VERSION_RULES = {
    {py::re::compile(py::str("^rd[-_]"), py::re::I), py::str("rubydung")},     // rd-132211 (RubyDung)
    {py::re::compile(py::str("^c0?\\."), py::re::I), py::str("rubydung")},     // c0.30_01c (classic ~ pre-classic handling)
    {py::re::compile(py::str("^classic"), py::re::I), py::str("rubydung")},
    {py::re::compile(py::str("^inf[-_]"), py::re::I), py::str("infdev")},      // inf-20100618
    {py::re::compile(py::str("^infdev"), py::re::I), py::str("infdev")},
    {py::re::compile(py::str("^in[-_]"), py::re::I), py::str("indev")},        // in-20100223
    {py::re::compile(py::str("^indev"), py::re::I), py::str("indev")},
    {py::re::compile(py::str("^a"), py::re::I), py::str("alpha")},             // a1.2.6
    {py::re::compile(py::str("^b"), py::re::I), py::str("beta")},              // b1.6.6
    {py::re::compile(py::str("^\\d+\\.\\d")), py::str("release")},             // 1.2.5
};

std::optional<py::str> edition_for_version(const std::optional<py::str>& version_id) {
    if (!version_id || !py::truthy(*version_id)) {
        return std::nullopt;
    }
    py::str v = version_id->strip();
    for (const auto& rule : _VERSION_RULES) {
        if (std::get<0>(rule).match(v)) {
            return std::get<1>(rule);
        }
    }
    return std::nullopt;
}

std::tuple<py::set<py::str>, py::set<py::str>> _corpus_sets(const std::optional<py::list<ClassInfo>>& classes) {
    py::set<py::str> names;
    py::set<py::str> pkgs;
    if (classes) {                                    // classes or ()
        for (const ClassInfo& c : *classes) {
            if (c.name && py::truthy(*c.name)) {
                names.add(*c.name);
            }
            if (c.package && py::truthy(*c.package)) {
                pkgs.add(*c.package);
            }
        }
    }
    return {names, pkgs};
}

std::optional<py::str> detect_edition(const std::optional<py::list<ClassInfo>>& classes) {
    auto [names, pkgs] = _corpus_sets(classes);
    if (!py::truthy(names)) {
        return std::nullopt;
    }

    auto has_pkg = [&](const py::str& prefix) {
        for (const py::str& p : pkgs) {
            if (p == prefix || p.startswith(prefix + ".")) {
                return true;
            }
        }
        return false;
    };
    auto any_name = [&](const py::set<py::str>& markers) {   // bool(markers & names)
        return py::truthy(markers & names);
    };

    // Pre-classic: the package (or main class) is unmistakable.
    if (has_pkg("com.mojang.rubydung") || names.contains("RubyDung")) {
        return py::str("rubydung");
    }

    bool has_chunk = names.contains("Chunk");
    if (!has_chunk) {
        for (const py::str& n : names) {
            if (n.startswith("ChunkProvider")) {
                has_chunk = true;
                break;
            }
        }
    }
    // Beta-ONLY classes. WorldChunkManager / ChunkProviderGenerate are NOT markers: Alpha 1.2.x already has
    // both (biomes arrived there, as MobSpawnerBase), so they misfiled Alpha corpora as beta. These are
    // Beta additions: beds/cake/note/dispenser/dye (b1.2-1.3), McRegion RegionFile (b1.3), stats and
    // achievements (b1.5), the BiomeGenBase rename (b1.8), Anvil (carried into release).
    const py::set<py::str> beta_markers = {"BiomeGenBase", "RegionFile", "AnvilChunkLoader", "BlockBed",
                                           "BlockCake",    "BlockNote",  "BlockDispenser",   "ItemDye",
                                           "StatList",     "Achievement"};
    const py::set<py::str> release_markers = {"IBlockState", "BlockState", "WorldProviderEnd"};

    // Release: dedicated-server package split or block-state classes.
    if (has_pkg("net.minecraft.server") || any_name(release_markers)) {
        return py::str("release");
    }
    // Beta: biome / region-file / anvil world storage.
    if (any_name(beta_markers)) {
        return py::str("beta");
    }
    // Chunk-streamed but pre-biome worlds: infdev (early game-package layout) vs alpha (flat src).
    if (has_chunk) {
        if (has_pkg("net.minecraft.game") && !has_pkg("net.minecraft.src")) {
            return py::str("infdev");
        }
        return py::str("alpha");
    }
    // Finite-world era (no chunk streaming). Real Indev = net.minecraft.game.level.World + LevelLoader.
    if (has_pkg("net.minecraft.game.level") || names.contains("LevelLoader")) {
        return py::str("indev");
    }
    if ((names.contains("World") || names.contains("Level")) &&
        any_name(py::set<py::str>{"Block", "Material", "Tile"})) {
        return py::str("indev");
    }

    return std::nullopt;
}

std::optional<py::str> profile_name(const std::optional<py::str>& edition) {
    if (!edition || !py::truthy(*edition)) {
        return std::nullopt;
    }
    return EDITIONS.get(*edition);
}

std::optional<py::str> resolve(const std::optional<py::str>& explicit_, const std::optional<py::str>& mc_version,
                               const std::optional<py::list<ClassInfo>>& classes) {
    // 1. Explicit override.
    if (explicit_ && py::truthy(*explicit_)) {
        if (EDITIONS.contains(*explicit_)) {
            return EDITIONS.at(*explicit_);
        }
        for (const py::str& qualified : EDITIONS.values()) {
            if (qualified == *explicit_) {
                return explicit_;
            }
        }
        // Unknown explicit value: surface it rather than silently ignoring.
        throw py::ValueError(py::percent(py::str("unknown edition %r (choices: %s)"), *explicit_,
                                         py::join(py::str(", "), py::sorted(EDITIONS.keys()))));
    }

    // 2. Version-id mapping.
    std::optional<py::str> ed = edition_for_version(mc_version);
    if (ed) {
        return EDITIONS.at(*ed);
    }

    // 3. Source auto-detection.
    ed = detect_edition(classes);
    if (ed) {
        return EDITIONS.at(*ed);
    }

    // 4. Nothing conclusive -> caller keeps the default profile (a strict no-op).
    return std::nullopt;
}

}  // namespace codebreaker::java::projects::minecraft::edition
