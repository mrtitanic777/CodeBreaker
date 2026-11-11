// codebreaker/java/cmake.hpp -- SHIM over cb/java/cmake.hpp (docs/CPP_DESIGN.md 6.3); deleted with its last
// py:: consumer.
#pragma once

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/java/cmake.hpp"
#include "py_os.hpp"

namespace codebreaker::java::cmake {

// Write out_dir/CMakeLists.txt for an executable called `name`. -> its path
inline py::str write_cmakelists(const py::str& out_dir, const py::str& name) {
    return cb::compat::guard([&] {
        return cb::compat::to_py(cb::fs::to_text(cb::java::cmake::write_cmakelists(
            cb::fs::from_text(cb::compat::from_py(out_dir)), cb::compat::from_py(name))));
    });
}

}  // namespace codebreaker::java::cmake
