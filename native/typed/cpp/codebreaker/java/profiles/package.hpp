// codebreaker/java/profiles/package.hpp -- typed C++ port of codebreaker/java/profiles/__init__.py.
//
// Target profiles -- the seam that keeps the transpiler core game-agnostic.
//
// Game- and platform-specific data (class names, package roles, nullable fields/getters, the static-init
// seed, project-filter rules) lives in a *profile* rather than being hardcoded in the core logic modules.
// The core reads it through active(); swapping the active profile retargets the tool. The default is
// `generic` (empty defaults); a project's profiles register when it loads (java/projects).
//
// NOTE: this is distinct from the repo-root ``profiles/`` directory, which holds per-conversion TYPE
// override configs (e.g. ``alpha.json``). This package is the built-in TARGET-profile registry.
//
// Typing notes:
//   * Profile is the Python class: a bag of fields, every one empty by default. The constructor takes the
//     Python's keyword arguments as std::optional (nullopt = not given) in the Python's order, and copies
//     each (list(x or []), dict(...), set(...)) as the Python does.
//   * The registry holds shared Profile objects (the Python registers the object itself: a field changed
//     on it later -- minecraft's heavy_types_high -- shows in every reader). register_/get/active/use hand
//     back std::shared_ptr<Profile>.
//   * A profile's `heavy_types` is a tuple (a list<str> here), `unsigned_byte_scalar_fields` a set of the
//     strings the Python puts in it.
//   * on_change(fn): fn is `std::function<void(const Profile&)>`; it is returned for decorator-style use
//     (the Python returns fn). use() calls every one in registration order.
//   * get(name) of an unknown name asks the project layer (projects::for_profile) to load the project that
//     registers it, then KeyError if it is still unknown. `register` and `names` are register_ / names.
#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::java::profiles {

// A bag of target-specific data. Every field defaults to empty, so the generic core behaves
// game-agnostically until a profile supplies real data.
struct Profile {
    py::str name;
    py::list<py::str> static_init_order;
    py::dict<py::str, py::str> package_roles;
    py::list<py::str> heavy_types;                                // a tuple of names
    py::list<std::tuple<py::str, py::str>> vcxproj_filters;       // (basename regex, filter path)
    // Anchored regex (string) matching class BASENAMES that are DEAD on this target -- external desktop/GUI
    // types (java.awt / javax.swing / java.applet, org.w3c.dom) that cannot function on the console. The
    // pipeline drops any dead type that is ALSO unreferenced (reachability is the safety net). Precise by
    // design: explicit class names, never `\w*Listener`-style wildcards that would sweep in real game types.
    // None -> the profile prunes nothing.
    std::optional<py::str> dead_types;
    // Anchored regex (string) matching class BASENAMES of OPTIONAL platform SUBSYSTEMS -- self-contained 360
    // backend bundles (audio, networking, ...) that only matter if the game actually uses them. Pruned by the
    // SAME reachability pass as dead_types. None -> nothing optional to prune.
    std::optional<py::str> optional_types;
    py::set<py::str> nullable;
    py::set<py::str> nullable_fields;
    py::set<py::str> nullable_getters;
    py::dict<py::str, py::str> nullable_getter_return_type;
    py::set<py::str> unsigned_byte_scalar_fields;
    py::set<py::str> default_value_types;
    // heavy types whose by-value copy is a HIGH finding (an MB-scale buffer), not MED
    py::list<py::str> heavy_types_high;                           // () by default

    Profile() = default;
    explicit Profile(const py::str& name_,
                     const std::optional<py::list<py::str>>& static_init_order_ = std::nullopt,
                     const std::optional<py::dict<py::str, py::str>>& package_roles_ = std::nullopt,
                     const std::optional<py::list<py::str>>& heavy_types_ = std::nullopt,
                     const std::optional<py::list<std::tuple<py::str, py::str>>>& vcxproj_filters_ = std::nullopt,
                     const std::optional<py::set<py::str>>& nullable_ = std::nullopt,
                     const std::optional<py::set<py::str>>& nullable_fields_ = std::nullopt,
                     const std::optional<py::set<py::str>>& nullable_getters_ = std::nullopt,
                     const std::optional<py::dict<py::str, py::str>>& nullable_getter_return_type_ = std::nullopt,
                     const std::optional<py::set<py::str>>& unsigned_byte_scalar_fields_ = std::nullopt,
                     const std::optional<py::set<py::str>>& default_value_types_ = std::nullopt,
                     const std::optional<py::str>& dead_types_ = std::nullopt,
                     const std::optional<py::str>& optional_types_ = std::nullopt);

    // Every field, in the order of the fields (the era profile ports of java/projects/minecraft/profiles write their data
    // as exactly this: `Profile{name, static_init_order, ..., heavy_types_high}`).
    Profile(const py::str& name_, const py::list<py::str>& static_init_order_, const py::dict<py::str, py::str>& package_roles_,
            const py::list<py::str>& heavy_types_, const py::list<std::tuple<py::str, py::str>>& vcxproj_filters_,
            const std::optional<py::str>& dead_types_, const std::optional<py::str>& optional_types_, const py::set<py::str>& nullable_,
            const py::set<py::str>& nullable_fields_, const py::set<py::str>& nullable_getters_,
            const py::dict<py::str, py::str>& nullable_getter_return_type_, const py::set<py::str>& unsigned_byte_scalar_fields_,
            const py::set<py::str>& default_value_types_, const py::list<py::str>& heavy_types_high_)
        : name(name_), static_init_order(static_init_order_), package_roles(package_roles_), heavy_types(heavy_types_),
          vcxproj_filters(vcxproj_filters_), dead_types(dead_types_), optional_types(optional_types_), nullable(nullable_),
          nullable_fields(nullable_fields_), nullable_getters(nullable_getters_), nullable_getter_return_type(nullable_getter_return_type_),
          unsigned_byte_scalar_fields(unsigned_byte_scalar_fields_), default_value_types(default_value_types_),
          heavy_types_high(heavy_types_high_) {}
};

// Register a callback invoked as fn(profile) every time the active profile changes (and it is the caller's
// job to call it once for the current profile if they need an immediate seed). Returns `fn`.
using Observer = std::function<void(const Profile&)>;
Observer on_change(const Observer& fn);

std::shared_ptr<Profile> register_(const std::shared_ptr<Profile>& profile);
std::shared_ptr<Profile> register_(const Profile& profile);   // (a copy of it is what is registered)

// KeyError for an unknown name (after asking the project layer to load the project that registers it).
std::shared_ptr<Profile> get(const py::str& name);
// the names of every registered profile, sorted
py::list<py::str> names();

// The empty, game-agnostic baseline.
const std::shared_ptr<Profile>& GENERIC();

// The currently-active target profile.
std::shared_ptr<Profile> active();

// Switch the active target profile by name; returns it. Fires every on_change observer so the core modules
// re-derive their frozen profile globals from the new profile.
std::shared_ptr<Profile> use(const py::str& name);

}  // namespace codebreaker::java::profiles
