// codebreaker/java/projects/minecraft/profiles/minecraft_base.cpp -- typed C++ port of
// codebreaker.java.projects.minecraft.profiles.minecraft_base. See the header for the module's notes.
#include "codebreaker/java/projects/minecraft/profiles/minecraft_base.hpp"

#include "codebreaker/java/projects/minecraft/profiles/minecraft_xbox360.hpp"

// The constants below are copied from minecraft_xbox360's, which are built when its translation unit
// starts. Python's `import` guarantees that order; C++ does not across units, so this unit's
// initialisation is put in a later CRT section (the default user initialisers run first, in .CRT$XCU).
#ifdef _MSC_VER
#pragma warning(disable : 4075)
#pragma init_seg(".CRT$XCV")
#endif

namespace codebreaker::java::projects::minecraft::profiles::minecraft_base {

namespace xbox = codebreaker::java::projects::minecraft::profiles::minecraft_xbox360;

const py::str SHARED_DEAD_TYPES = xbox::DEAD_TYPES;
const py::list<py::str> SHARED_HEAVY_TYPES = xbox::HEAVY_TYPES;

// (list(...) is a copy: editions must not change the xbox360 table through this one)
const py::list<std::tuple<py::str, py::str>> FULL_FILTERS =
    py::list<std::tuple<py::str, py::str>>::from(xbox::VCXPROJ_FILTERS);

const py::dict<py::str, py::str> SHARED_PACKAGE_ROLES = {
    {py::str("util"),
     xbox::PACKAGE_ROLES.get(py::str("util"), py::str("Standalone utility helpers (math, noise, etc.)."))},
};

py::list<std::tuple<py::str, py::str>> build_filters(const py::list<std::tuple<py::str, py::str>>& era_rules) {
    return py::list<std::tuple<py::str, py::str>>::from(era_rules) + FULL_FILTERS;
}

py::list<std::tuple<py::str, py::str>> build_filters() {
    return build_filters(py::list<std::tuple<py::str, py::str>>());
}

}  // namespace codebreaker::java::projects::minecraft::profiles::minecraft_base
