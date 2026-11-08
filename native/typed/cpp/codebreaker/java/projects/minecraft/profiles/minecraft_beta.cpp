// codebreaker/java/projects/minecraft/profiles/minecraft_beta.cpp -- typed C++ port of
// codebreaker.java.projects.minecraft.profiles.minecraft_beta (see the header).

#include "codebreaker/java/projects/minecraft/profiles/minecraft_beta.hpp"

#include "codebreaker/java/projects/minecraft/profiles/minecraft_base.hpp"
#include "codebreaker/java/projects/minecraft/profiles/minecraft_xbox360.hpp"

namespace codebreaker::java::projects::minecraft::profiles::minecraft_beta {

namespace _base = codebreaker::java::projects::minecraft::profiles::minecraft_base;
namespace _canon = codebreaker::java::projects::minecraft::profiles::minecraft_xbox360;

const Profile& PROFILE() {
    static const Profile p = Profile{
        py::str(R"rx(minecraft_beta)rx"),
        _canon::STATIC_INIT_ORDER.copy(),  // list(_canon.STATIC_INIT_ORDER)
        _canon::PACKAGE_ROLES.copy(),      // dict(_canon.PACKAGE_ROLES)
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

}  // namespace codebreaker::java::projects::minecraft::profiles::minecraft_beta
