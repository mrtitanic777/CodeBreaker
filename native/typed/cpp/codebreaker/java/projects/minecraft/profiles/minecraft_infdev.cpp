// codebreaker/java/projects/minecraft/profiles/minecraft_infdev.cpp -- typed C++ port of
// codebreaker.java.projects.minecraft.profiles.minecraft_infdev (see the header).

#include "codebreaker/java/projects/minecraft/profiles/minecraft_infdev.hpp"

#include "codebreaker/java/projects/minecraft/profiles/minecraft_base.hpp"

namespace codebreaker::java::projects::minecraft::profiles::minecraft_infdev {

namespace _base = codebreaker::java::projects::minecraft::profiles::minecraft_base;
namespace _canon = codebreaker::java::projects::minecraft::profiles::minecraft_xbox360;

const py::dict<py::str, py::str>& PACKAGE_ROLES() {
    static const py::dict<py::str, py::str> roles = [] {
        py::dict<py::str, py::str> d = _base::SHARED_PACKAGE_ROLES.copy();
        d.set(py::str(R"rx(com.mojang.minecraft)rx"), py::str(R"rx(Top-level game shell, main loop and input.)rx"));
        d.set(py::str(R"rx(com.mojang.minecraft.level)rx"), py::str(R"rx(The infinite world model, chunk streaming, generation, save/load.)rx"));
        d.set(py::str(R"rx(com.mojang.minecraft.level.tile)rx"), py::str(R"rx(Block/Tile definitions and per-block behaviour.)rx"));
        d.set(py::str(R"rx(com.mojang.minecraft.character)rx"), py::str(R"rx(Player and mob entities.)rx"));
        d.set(py::str(R"rx(com.mojang.minecraft.phys)rx"), py::str(R"rx(Collision primitives (AABB).)rx"));
        return d;
    }();
    return roles;
}

const py::list<py::str> STATIC_INIT_ORDER = {
    py::str(R"rx(Material)rx"),
    py::str(R"rx(Block)rx"),
    py::str(R"rx(Item)rx"),
};

// Registry arrays with NULL slots.
const py::set<py::str> NULLABLE = {
    py::str(R"rx(blocksList)rx"),
    py::str(R"rx(itemsList)rx"),
};

const py::set<py::str> NULLABLE_FIELDS = {
    py::str(R"rx(level)rx"),
    py::str(R"rx(levelRenderer)rx"),
    py::str(R"rx(particleEngine)rx"),
    py::str(R"rx(player)rx"),
    py::str(R"rx(textures)rx"),
};

// Chunk lookups can miss (unloaded/ungenerated chunk) -> NULL.
const py::set<py::str> NULLABLE_GETTERS = {
    py::str(R"rx(getChunk)rx"),
    py::str(R"rx(getChunkFromBlockCoords)rx"),
    py::str(R"rx(getChunkFromChunkCoords)rx"),
};

const py::dict<py::str, py::str> NULLABLE_GETTER_RETURN_TYPE = {
    {py::str(R"rx(getChunk)rx"), py::str(R"rx(Chunk)rx")},
    {py::str(R"rx(getChunkFromChunkCoords)rx"), py::str(R"rx(Chunk)rx")},
    {py::str(R"rx(getChunkFromBlockCoords)rx"), py::str(R"rx(Chunk)rx")},
};

const Profile& PROFILE() {
    static const Profile p = Profile{
        py::str(R"rx(minecraft_infdev)rx"),
        STATIC_INIT_ORDER.copy(),
        PACKAGE_ROLES().copy(),
        _base::SHARED_HEAVY_TYPES.copy(),  // tuple(heavy_types)
        _base::build_filters(),
        _base::SHARED_DEAD_TYPES,
        std::nullopt,                      // optional_types
        py::set<py::str>::from(NULLABLE),
        py::set<py::str>::from(NULLABLE_FIELDS),
        py::set<py::str>::from(NULLABLE_GETTERS),
        NULLABLE_GETTER_RETURN_TYPE.copy(),
        py::set<py::str>(),                // unsigned_byte_scalar_fields=set()
        py::set<py::str>(),                // default_value_types=set()
        py::list<py::str>(),               // heavy_types_high = ()
    };
    return p;
}

}  // namespace codebreaker::java::projects::minecraft::profiles::minecraft_infdev
