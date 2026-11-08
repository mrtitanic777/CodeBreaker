// codebreaker/java/projects/minecraft/profiles/minecraft_release.cpp -- typed C++ port of
// codebreaker.java.projects.minecraft.profiles.minecraft_release (see the header).

#include "codebreaker/java/projects/minecraft/profiles/minecraft_release.hpp"

#include "codebreaker/java/projects/minecraft/profiles/minecraft_base.hpp"

namespace codebreaker::java::projects::minecraft::profiles::minecraft_release {

namespace _base = codebreaker::java::projects::minecraft::profiles::minecraft_base;
namespace _canon = codebreaker::java::projects::minecraft::profiles::minecraft_xbox360;

const py::dict<py::str, py::str>& PACKAGE_ROLES() {
    static const py::dict<py::str, py::str> roles = [] {
        py::dict<py::str, py::str> d = _canon::PACKAGE_ROLES.copy();
        d.set(py::str(R"rx(net.minecraft.src)rx"), py::str(R"rx(Client-side game classes (release single-package layout).)rx"));
        d.set(py::str(R"rx(net.minecraft.client)rx"), py::str(R"rx(Client game shell, input and main loop.)rx"));
        d.set(py::str(R"rx(net.minecraft.server)rx"), py::str(R"rx(Integrated server: world ticking, save, and player management.)rx"));
        return d;
    }();
    return roles;
}

const Profile& PROFILE() {
    static const Profile p = Profile{
        py::str(R"rx(minecraft_release)rx"),
        _canon::STATIC_INIT_ORDER.copy(),  // list(_canon.STATIC_INIT_ORDER)
        PACKAGE_ROLES().copy(),
        _base::SHARED_HEAVY_TYPES.copy(),  // tuple(heavy_types)
        _base::build_filters(),
        _base::SHARED_DEAD_TYPES,
        std::nullopt,                      // optional_types
        py::set<py::str>::from(_canon::NULLABLE),
        py::set<py::str>::from(_canon::NULLABLE_FIELDS),
        py::set<py::str>::from(_canon::NULLABLE_GETTERS),
        _canon::NULLABLE_GETTER_RETURN_TYPE.copy(),
        py::set<py::str>(),                // unsigned_byte_scalar_fields=set()
        py::set<py::str>(),                // default_value_types=set()
        py::list<py::str>(),               // heavy_types_high = ()
    };
    return p;
}

}  // namespace codebreaker::java::projects::minecraft::profiles::minecraft_release
