// codebreaker/decompile/xbox_effect.hpp -- COMPAT SHIM (docs/CPP_DESIGN.md 6.3). The module now lives in
// native/cb/decompile/xbox_effect.{hpp,cpp}; this header keeps the one function the old layer calls (to_fx: an Xbox 360
// effect as .fx text, raising xenos_hlsl::Unsupported for what is not translated yet). Conversion and forwarding only;
// deleted with its last py:: consumer. (The Python module's private helpers -- _type_name, _literal, _array_selection ... --
// had no other caller and are the new module's own now.)
#pragma once

#include "cb/compat/py_bridge.hpp"
#include "cb/decompile/xbox_effect.hpp"
#include "codebreaker/decompile/xenos_hlsl.hpp"
#include "py.hpp"

namespace codebreaker::decompile::xbox_effect {

inline py::str to_fx(const py::bytes& data, const py::str& name = py::str("Effect")) {
    namespace nx = ::cb::decompile::xenos_hlsl;
    try {
        const ::cb::Bytes bytes = ::cb::compat::from_py(data);
        return ::cb::compat::to_py(::cb::decompile::xbox_effect::to_fx(bytes, name.utf8()));
    } catch (const nx::Unsupported& e) {
        throw xenos_hlsl::Unsupported(py::percent("%s shader %s, slot %d: %s", ::cb::compat::to_py(nx::stage_name(e.stage())),
                                                  ::cb::compat::to_py(e.shader()), static_cast<int64_t>(e.slot()),
                                                  ::cb::compat::to_py(e.what_failed())));
    } catch (const ::cb::Error& e) {
        ::cb::compat::rethrow_as_py(e);
    }
}

}  // namespace codebreaker::decompile::xbox_effect
