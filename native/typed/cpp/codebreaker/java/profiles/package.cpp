// codebreaker/java/profiles/package.cpp -- typed C++ port of codebreaker/java/profiles/__init__.py (see package.hpp).
#include "codebreaker/java/profiles/package.hpp"

#include <vector>

#include "codebreaker/java/projects/package.hpp"

namespace codebreaker::java::profiles {

using py::str;

Profile::Profile(const str& name_, const std::optional<py::list<str>>& static_init_order_,
                 const std::optional<py::dict<str, str>>& package_roles_, const std::optional<py::list<str>>& heavy_types_,
                 const std::optional<py::list<std::tuple<str, str>>>& vcxproj_filters_,
                 const std::optional<py::set<str>>& nullable_, const std::optional<py::set<str>>& nullable_fields_,
                 const std::optional<py::set<str>>& nullable_getters_,
                 const std::optional<py::dict<str, str>>& nullable_getter_return_type_,
                 const std::optional<py::set<str>>& unsigned_byte_scalar_fields_,
                 const std::optional<py::set<str>>& default_value_types_, const std::optional<str>& dead_types_,
                 const std::optional<str>& optional_types_)
    : name(name_) {
    // list(x or []), dict(x or {}), set(x or set()), tuple(x or ()): a copy of what is given, empty when not
    if (static_init_order_) static_init_order = static_init_order_->copy();
    if (package_roles_) package_roles = package_roles_->copy();
    if (heavy_types_) heavy_types = heavy_types_->copy();
    if (vcxproj_filters_) vcxproj_filters = vcxproj_filters_->copy();
    dead_types = dead_types_;
    optional_types = optional_types_;
    if (nullable_) nullable = py::set<str>::from(*nullable_);
    if (nullable_fields_) nullable_fields = py::set<str>::from(*nullable_fields_);
    if (nullable_getters_) nullable_getters = py::set<str>::from(*nullable_getters_);
    if (nullable_getter_return_type_) nullable_getter_return_type = nullable_getter_return_type_->copy();
    if (unsigned_byte_scalar_fields_) unsigned_byte_scalar_fields = py::set<str>::from(*unsigned_byte_scalar_fields_);
    if (default_value_types_) default_value_types = py::set<str>::from(*default_value_types_);
    // (heavy_types_high: () by default)
}

namespace {

// _REGISTRY, _ON_CHANGE, GENERIC and _active, made when first asked for (the Python's import time)
struct State {
    py::dict<str, std::shared_ptr<Profile>> registry;
    std::vector<Observer> on_change;
    std::shared_ptr<Profile> generic;
    std::shared_ptr<Profile> active;
};

State& state() {
    static State* s = [] {
        auto* st = new State();
        st->generic = std::make_shared<Profile>(str("generic"));
        st->registry.set(st->generic->name, st->generic);          // GENERIC = register(Profile('generic'))
        st->active = st->generic;
        return st;
    }();
    return *s;
}

}  // namespace

Observer on_change(const Observer& fn) {
    state().on_change.push_back(fn);
    return fn;
}

std::shared_ptr<Profile> register_(const std::shared_ptr<Profile>& profile) {
    state().registry.set(profile->name, profile);
    return profile;
}

std::shared_ptr<Profile> register_(const Profile& profile) { return register_(std::make_shared<Profile>(profile)); }

std::shared_ptr<Profile> get(const str& name) {
    State& s = state();
    if (!s.registry.contains(name)) {   // a project's profile: its project registers it on load
        projects::for_profile(name);
    }
    if (!s.registry.contains(name)) throw py::KeyError(py::repr(name).utf8());
    return s.registry.at(name);
}

py::list<str> names() {
    py::list<str> out;
    for (const str& n : py::sorted(state().registry.keys())) out.append(n);
    return out;
}

const std::shared_ptr<Profile>& GENERIC() { return state().generic; }

std::shared_ptr<Profile> active() { return state().active; }

std::shared_ptr<Profile> use(const str& name) {
    State& s = state();
    s.active = get(name);
    // (an observer may register another: the Python's loop over the list sees it, so the bound is re-read)
    for (size_t i = 0; i < s.on_change.size(); i++) {
        Observer fn = s.on_change[i];
        fn(*s.active);
    }
    return s.active;
}

}  // namespace codebreaker::java::profiles
