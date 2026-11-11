// codebreaker/java/stubs.hpp -- SHIM over cb/java/stubs.hpp (docs/CPP_DESIGN.md 6.3); deleted with its last
// py:: consumer. (The old SUBSYSTEMS table had no consumer but classify_stub and is not carried.)
#pragma once

#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/java/stubs.hpp"

namespace codebreaker::java::stubs {

// One subsystem's record (the Python dict with the keys label, description, sdk_hint, members).
struct Subsystem {
    py::str label;
    py::str description;
    py::str sdk_hint;
    py::set<py::str> members;
};

// Return (subsystem_id, subsystem_info) for a stub class name, or ('unknown', None).
inline std::tuple<py::str, std::optional<Subsystem>> classify_stub(const py::str& class_name) {
    const cb::java::stubs::SubsystemInfo* info = cb::java::stubs::classify_stub(cb::compat::from_py(class_name));
    if (info == nullptr) return {py::str("unknown"), std::nullopt};
    Subsystem s{cb::compat::to_py(info->label), cb::compat::to_py(info->description), cb::compat::to_py(info->sdk_hint), {}};
    for (std::string_view m : info->members) s.members.add(cb::compat::to_py(m));
    return {cb::compat::to_py(info->key), std::move(s)};
}

}  // namespace codebreaker::java::stubs
