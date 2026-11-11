// codebreaker/java/profiles/package.hpp -- SHIM over cb/java/profiles/package.hpp (docs/CPP_DESIGN.md 6.3); deleted
// with its last py:: consumer.
//
// The old ports reach target profiles through a process-wide registry: they register one Profile *object* and read
// (and, for heavy_types_high, change) that same object later. The new tree has no such registry -- a ProfileSet is an
// ordinary object holding values -- so the registry itself, with its active profile and its change observers, stays
// here as the legacy shape the old consumers still depend on. The Profile data type is the new module's
// (`from_cb` converts one into the old record); everything that decides what a profile contains is new code.
#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/java/profiles/package.hpp"
#include "codebreaker/java/projects/package.hpp"
#include "py.hpp"

namespace codebreaker::java::profiles {

// A bag of target-specific data. Every field defaults to empty.
struct Profile {
    py::str name;
    py::list<py::str> static_init_order;
    py::dict<py::str, py::str> package_roles;
    py::list<py::str> heavy_types;
    py::list<std::tuple<py::str, py::str>> vcxproj_filters;       // (basename regex, filter path)
    std::optional<py::str> dead_types;                            // anchored regex, or None
    std::optional<py::str> optional_types;                        // anchored regex, or None
    py::set<py::str> nullable;
    py::set<py::str> nullable_fields;
    py::set<py::str> nullable_getters;
    py::dict<py::str, py::str> nullable_getter_return_type;
    py::set<py::str> unsigned_byte_scalar_fields;
    py::set<py::str> default_value_types;
    py::list<py::str> heavy_types_high;                           // () by default
};

// The old record of a new-tree profile. (unsigned_byte_scalar_fields were (ClassName, fieldName) pairs; the old
// record holds one string per pair, "Class.field".)
inline Profile from_cb(const cb::java::profiles::Profile& p) {
    namespace cc = cb::compat;
    Profile o;
    o.name = cc::to_py(p.name);
    o.static_init_order = cc::to_py(p.static_init_order);
    o.package_roles = cc::to_py(p.package_roles);
    o.heavy_types = cc::to_py(p.heavy_types);
    for (const auto& f : p.vcxproj_filters) o.vcxproj_filters.append(std::make_tuple(cc::to_py(f.pattern), cc::to_py(f.folder)));
    o.dead_types = cc::to_py(p.dead_types);
    o.optional_types = cc::to_py(p.optional_types);
    o.nullable = cc::to_py(p.type.nullable);
    o.nullable_fields = cc::to_py(p.type.nullable_fields);
    o.nullable_getters = cc::to_py(p.type.nullable_getters);
    o.nullable_getter_return_type = cc::to_py(p.type.nullable_getter_return_type);
    for (const auto& cf : p.type.unsigned_byte_scalar_fields) o.unsigned_byte_scalar_fields.add(cc::to_py(cf.first + "." + cf.second));
    o.default_value_types = cc::to_py(p.type.default_value_types);
    o.heavy_types_high = cc::to_py(p.heavy_types_high);
    return o;
}

namespace shim_ {
// _REGISTRY, _ON_CHANGE, GENERIC and _active of the Python module, made when first asked for.
struct State {
    py::dict<py::str, std::shared_ptr<Profile>> registry;
    std::vector<std::function<void(const Profile&)>> on_change;
    std::shared_ptr<Profile> generic;
    std::shared_ptr<Profile> active;
};
inline State& state() {
    static State* s = [] {
        auto* st = new State();
        st->generic = std::make_shared<Profile>();
        st->generic->name = py::str("generic");
        st->registry.set(st->generic->name, st->generic);
        st->active = st->generic;
        return st;
    }();
    return *s;
}
}  // namespace shim_

// Register a callback invoked as fn(profile) every time the active profile changes. Returns `fn`.
using Observer = std::function<void(const Profile&)>;
inline Observer on_change(const Observer& fn) {
    shim_::state().on_change.push_back(fn);
    return fn;
}

inline std::shared_ptr<Profile> register_(const std::shared_ptr<Profile>& profile) {
    shim_::state().registry.set(profile->name, profile);
    return profile;
}
inline std::shared_ptr<Profile> register_(const Profile& profile) { return register_(std::make_shared<Profile>(profile)); }

// KeyError for an unknown name (after asking the project layer to load the project that registers it).
inline std::shared_ptr<Profile> get(const py::str& name) {
    auto& s = shim_::state();
    if (!s.registry.contains(name)) projects::for_profile(name);
    if (!s.registry.contains(name)) throw py::KeyError(py::repr(name).utf8());
    return s.registry.at(name);
}

// the names of every registered profile, sorted
inline py::list<py::str> names() {
    py::list<py::str> out;
    for (const py::str& n : py::sorted(shim_::state().registry.keys())) out.append(n);
    return out;
}

// The empty, game-agnostic baseline.
inline const std::shared_ptr<Profile>& GENERIC() { return shim_::state().generic; }

// The currently-active target profile.
inline std::shared_ptr<Profile> active() { return shim_::state().active; }

// Switch the active target profile by name; returns it. Fires every on_change observer.
inline std::shared_ptr<Profile> use(const py::str& name) {
    auto& s = shim_::state();
    s.active = get(name);
    // (an observer may register another: the loop sees it, so the bound is re-read)
    for (size_t i = 0; i < s.on_change.size(); i++) {
        Observer fn = s.on_change[i];
        fn(*s.active);
    }
    return s.active;
}

}  // namespace codebreaker::java::profiles
