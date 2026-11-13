// Compile-only check of the shim at the old header path of source/pe_writer: the old API over the new module. Nothing here runs; it is in
// cb_shim_check so the shim stays honest until its last consumer is refactored.
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

}  // namespace shim_check
