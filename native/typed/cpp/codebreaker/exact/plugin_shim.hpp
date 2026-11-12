// codebreaker/exact/plugin_shim.hpp -- what the compat shims of the toolchain plugins (plm86, mspascal, xenix_c) share: the new module's
// Locations and Context put together from the settings, the options of an old compile call, and the cache the old callers shared.
// Conversion and forwarding only; deleted with the last of the three.
#pragma once

#include <optional>
#include <tuple>

#include "cb/compat/legacy_settings.hpp"
#include "cb/compat/py_bridge.hpp"
#include "cb/exact/modules.hpp"
#include "cb/exact/toolchain.hpp"
#include "codebreaker/base/cache.hpp"
#include "codebreaker/exact/toolchain.hpp"
#include "py.hpp"
#include "py_settings.hpp"

namespace codebreaker::exact::plugin_shim {

namespace nt = ::cb::exact::toolchain;

// The locations of the checkout (its profiles, kits and state), made once.
inline const nt::Locations& locations() {
    static const nt::Locations l = nt::Locations::of_checkout(::cb::compat::legacy::path_of(py::settings::checkout_root()));
    return l;
}

// What a plugin of the new module is run with: no cache of its own -- the shim keeps its answers in the cache the old callers shared
// (cache kind "exact"), under the key the plugin says it is made under.
inline nt::Context context() {
    nt::Context c;
    c.locations = &locations();
    return c;
}

// The profile as the old module read it, as the new one.
inline nt::Profile new_profile(const toolchain::Profile& old) { return nt::parse_profile(old.keyed.utf8()); }

using Result = std::tuple<std::optional<py::bytes>, py::str>;

inline Result result_of(const ::cb::exact::modules::CompileResult& r) {
    return {r.object ? std::optional<py::bytes>(::cb::compat::to_py(::cb::ByteView(*r.object))) : std::nullopt, ::cb::compat::to_py(r.console)};
}

inline ::cb::exact::modules::CompileOptions options_of(const py::dict<py::str, py::bytes>* headers, const py::list<py::str>& include,
                                                        const std::optional<py::str>& switches) {
    ::cb::exact::modules::CompileOptions o;
    if (headers)
        for (const auto& kv : headers->items()) o.headers[std::get<0>(kv).utf8()] = ::cb::compat::from_py(std::get<1>(kv));
    for (const py::str& d : include) o.include.push_back(d.utf8());
    if (switches) o.switches = switches->utf8();
    return o;
}

// The answer kept under `key`, or made by `make` and kept (the Python kept it even when asked not to read it).
template <class Make>
Result kept(const py::str& key, bool read, Make&& make) {
    namespace cache = codebreaker::base::cache;
    if (read) {
        const std::optional<Result> got = cache::value<Result>(py::str("exact"), key);
        if (got) return *got;
    }
    const Result res = make();
    cache::keep<Result>(py::str("exact"), key, res);
    return res;
}

}  // namespace codebreaker::exact::plugin_shim
