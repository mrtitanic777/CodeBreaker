// Compile-only check of the shims at the old header paths of source/pe_writer and source/dos_writer: the old API over the new modules.
// Nothing here runs; it is in cb_shim_check so the shims stay honest until their last consumer is refactored.
#include "codebreaker/source/dos_writer.hpp"
#include "codebreaker/source/driver_writer.hpp"
#include "codebreaker/source/pe_writer.hpp"

namespace shim_check {

inline void touch_source_pe() {
    namespace pe = codebreaker::source::pe_writer;
    (void)pe::reconstruct(py::str("x.dll"), py::str("out"));
    (void)pe::reconstruct(py::str("x.dll"), py::str("out"), false);
    (void)pe::reconstruct(py::str("x.dll"), py::str("out"), py::str("stub.bin"));
    (void)pe::main(py::list<py::str>{py::str("x.dll")});
    (void)pe::DOC;
}

inline void touch_source_driver() {
    namespace dr = codebreaker::source::driver_writer;
    const dr::Reconstruction r = dr::reconstruct(py::str("x.vxd"), py::str("out"));
    (void)dr::reconstruct(py::str("x.vxd"), py::str("out"), 1, nullptr);
    (void)r.checks;
    (void)r.same;
    (void)r.differing_objects;
    (void)r.l2;
    dr::set_capstone_dll(py::str("capstone.dll"));
    (void)dr::main(py::list<py::str>{py::str("x.vxd")});
    (void)dr::MAIN_DOC;
}

inline void touch_source_dos() {
    namespace dw = codebreaker::source::dos_writer;
    const dw::Result r = dw::reconstruct(py::str("x.exe"), py::str("out"));
    (void)r.same;
    (void)r.file_same;
    (void)r.parts;
    (void)dw::rebuild(py::str("out"));
    (void)dw::main(py::list<py::str>{py::str("x.exe")});
    (void)dw::MAIN_DOC;
}

}  // namespace shim_check
