// codebreaker/java/projects/minecraft/profiles/minecraft_rubydung.cpp -- typed C++ port of
// codebreaker.java.projects.minecraft.profiles.minecraft_rubydung (see the header).

#include "codebreaker/java/projects/minecraft/profiles/minecraft_rubydung.hpp"

#include "codebreaker/java/projects/minecraft/profiles/minecraft_base.hpp"

namespace codebreaker::java::projects::minecraft::profiles::minecraft_rubydung {

namespace _base = codebreaker::java::projects::minecraft::profiles::minecraft_base;
namespace _canon = codebreaker::java::projects::minecraft::profiles::minecraft_xbox360;

const py::dict<py::str, py::str>& PACKAGE_ROLES() {
    static const py::dict<py::str, py::str> roles = [] {
        py::dict<py::str, py::str> d = _base::SHARED_PACKAGE_ROLES.copy();
        d.set(py::str(R"rx(com.mojang.rubydung)rx"), py::str(R"rx(Top-level pre-classic game shell, main loop and input.)rx"));
        d.set(py::str(R"rx(com.mojang.rubydung.level)rx"), py::str(R"rx(The flat Level model, tiles, level renderer and frustum.)rx"));
        d.set(py::str(R"rx(com.mojang.rubydung.character)rx"), py::str(R"rx(Player and mob (Zombie) entities.)rx"));
        d.set(py::str(R"rx(com.mojang.rubydung.phys)rx"), py::str(R"rx(Collision primitives (AABB).)rx"));
        return d;
    }();
    return roles;
}

// Tile holds the static tile instances (Tile.rock/grass/...) and the Tile.tiles[] registry, so its
// statics must init first (they are read when the Level/renderer builds).
const py::list<py::str> STATIC_INIT_ORDER = {
    py::str(R"rx(Tile)rx"),
};

// Tile.tiles[] has NULL slots for unregistered tile ids -- deref of an unregistered slot is a virtual
// call on NULL (IP 0x0 on the console).
const py::set<py::str> NULLABLE = {
    py::str(R"rx(tiles)rx"),
};

// Fields legitimately NULL at points in the lifecycle (set after ctor / cleared on shutdown).
const py::set<py::str> NULLABLE_FIELDS = {
    py::str(R"rx(level)rx"),
    py::str(R"rx(levelRenderer)rx"),
    py::str(R"rx(particleEngine)rx"),
    py::str(R"rx(player)rx"),
};

const py::set<py::str> NULLABLE_GETTERS;

const py::dict<py::str, py::str> NULLABLE_GETTER_RETURN_TYPE;

const Profile& PROFILE() {
    static const Profile p = Profile{
        py::str(R"rx(minecraft_rubydung)rx"),
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

}  // namespace codebreaker::java::projects::minecraft::profiles::minecraft_rubydung
