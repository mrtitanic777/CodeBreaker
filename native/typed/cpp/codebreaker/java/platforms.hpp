// codebreaker/java/platforms.hpp -- SHIM over cb/java/platforms.hpp (docs/CPP_DESIGN.md 6.3); deleted with its last
// py:: consumer. (The private _STDAFX* tables of the port had no consumer and are not carried.)
#pragma once

#include <optional>

#include "cb/compat/py_bridge.hpp"
#include "cb/java/platforms.hpp"

namespace codebreaker::java::platforms {

struct Subsystem {
    py::str sdk;
    py::str libs;
    py::str apis;
};

struct Platform {
    py::str label;
    py::str description;
    std::optional<py::str> sdk_dir;
    bool big_endian = false;
    bool null_page_protected = false;
    py::str null_deref_msg;
    py::dict<py::str, Subsystem> subsystems;
};

namespace shim_ {
inline py::dict<py::str, Platform> build() {
    namespace cbp = cb::java::platforms;
    py::dict<py::str, Platform> out;
    for (const cbp::Platform& p : cbp::platforms()) {
        Platform q{cb::compat::to_py(p.label), cb::compat::to_py(p.description), std::nullopt, p.big_endian,
                   p.null_page_protected, cb::compat::to_py(p.null_deref_msg), {}};
        if (p.sdk_dir) q.sdk_dir = cb::compat::to_py(*p.sdk_dir);
        for (const cb::java::stubs::SubsystemInfo& s : cb::java::stubs::subsystems()) {
            const cbp::SubsystemApi& api = p.api(s.id);
            q.subsystems.set(cb::compat::to_py(s.key),
                             Subsystem{cb::compat::to_py(api.sdk), cb::compat::to_py(api.libs), cb::compat::to_py(api.apis)});
        }
        out.set(cb::compat::to_py(p.key), std::move(q));
    }
    return out;
}
}  // namespace shim_

inline const py::dict<py::str, Platform> PLATFORMS = shim_::build();

inline std::optional<Platform> get_platform(const py::str& platform_id) { return PLATFORMS.get(platform_id); }

inline py::str get_stub_comment(const py::str& platform_id, const py::str& subsystem_id) {
    return cb::compat::to_py(cb::java::platforms::stub_comment(cb::compat::from_py(platform_id), cb::compat::from_py(subsystem_id)));
}

inline py::str generate_stdafx(const py::str& platform_id, const py::str& cpp_std_label = py::str("C++03")) {
    return cb::compat::to_py(cb::java::platforms::generate_stdafx(cb::compat::from_py(platform_id), cb::compat::from_py(cpp_std_label)));
}

}  // namespace codebreaker::java::platforms
