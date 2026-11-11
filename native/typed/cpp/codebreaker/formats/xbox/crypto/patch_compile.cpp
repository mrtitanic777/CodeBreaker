// codebreaker/formats/xbox/crypto/patch_compile.cpp -- typed C++ port of codebreaker.formats.xbox.crypto.patch_compile.

#include "codebreaker/formats/xbox/crypto/patch_compile.hpp"

#include "codebreaker/formats/xbox/crypto/build_lib.hpp"

namespace codebreaker::formats::xbox::crypto::patch_compile {

using namespace build_lib;

const py::str PATCH_DIR("Patches");

int64_t main() {
    assemble_rgl_flash(py::str("Patches/RGL/17559-dev/RGLoader-dev.S"), py::str("Output/NonZero/RGL_flash.bin"));
    assemble_rgl_hdd(py::str("Patches/RGL/17559-dev/RGLoader-dev.S"), py::str("Output/NonZero/RGL_hdd.bin"));

    assemble_rgl_vfuses_flash(py::str("Patches/RGL/17559-dev/RGLoader-dev.S"), py::str("Output/Zero/VRGL_flash.bin"));
    assemble_rgl_vfuses_hdd(py::str("Patches/RGL/17559-dev/RGLoader-dev.S"), py::str("Output/Zero/VRGL_hdd.bin"));

    // assemble syscall zero payload
    assemble_devkit_patch(py::str("Patches/SC0.S"), py::str("Output/Compiled/Patches/SC0_dev.bin"));
    assemble_devkit_patch(py::str("Patches/SC0.S"), py::str("Output/Compiled/Patches/SpoofySC0_dev.bin"), py::list<py::str>{py::str("SPOOFY")});

    return 0;
}

}  // namespace codebreaker::formats::xbox::crypto::patch_compile
