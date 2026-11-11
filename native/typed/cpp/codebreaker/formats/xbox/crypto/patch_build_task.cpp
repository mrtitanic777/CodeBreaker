// codebreaker/formats/xbox/crypto/patch_build_task.cpp -- typed C++ port of codebreaker.formats.xbox.crypto.patch_build_task.

#include "codebreaker/formats/xbox/crypto/patch_build_task.hpp"

#include "py.hpp"
#include "codebreaker/formats/xbox/crypto/patch_checker.hpp"
#include "codebreaker/formats/xbox/crypto/patch_compile.hpp"

namespace codebreaker::formats::xbox::crypto::patch_build_task {

void main() {
    py::print(py::str("Compiling..."));
    patch_compile::main();
    // print("Patching...")
    // se_patcher.main()
    py::print(py::str("Checking..."));
    patch_checker::main();

    py::print(py::str("Done!"));
}

}  // namespace codebreaker::formats::xbox::crypto::patch_build_task
