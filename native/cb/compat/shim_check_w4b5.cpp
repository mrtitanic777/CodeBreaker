// Compile-only check of the shims at the old header paths of recompile/title and source/{ne_writer, le_writer, msx_writer}: the old API over
// the new modules. Nothing here runs; it is in cb_shim_check so the shims stay honest until their last consumer is refactored.
#include "codebreaker/recompile/title.hpp"
#include "codebreaker/source/le_writer.hpp"
#include "codebreaker/source/msx_writer.hpp"
#include "codebreaker/source/ne_writer.hpp"

namespace shim_check {

inline void touch_w4b5() {
    namespace title = codebreaker::recompile::title;
    namespace ne = codebreaker::source::ne_writer;
    namespace le = codebreaker::source::le_writer;
    namespace msx = codebreaker::source::msx_writer;
    (void)&title::locate;
    const std::optional<title::Title> found = title::locate(py::str("x"));
    if (found) {
        title::Title t = *found;
        (void)t.read_xex();
        (void)t.unpack(py::str("out"));
    }
    (void)ne::reconstruct(py::str("x.dll"), py::str("out"));
    (void)ne::reconstruct(py::str("x.dll"), py::str("out"), false);
    (void)ne::main(py::list<py::str>{py::str("x.dll")});
    (void)le::reconstruct(py::str("x.vxd"), py::str("out"));
    (void)le::main(py::list<py::str>{py::str("x.vxd")});
    (void)msx::reconstruct(py::str("x.rom"), py::str("out"));
    (void)msx::main(py::list<py::str>{py::str("x.rom")});
    (void)msx::PORTS;
    (void)msx::NAMES;
}

}  // namespace shim_check
