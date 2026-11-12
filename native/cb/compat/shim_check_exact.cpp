// Compile-only check of the exact shims that call old ports which cannot be linked into cb_tests (the habits comparers
// pascal_join's `run` resolves "equivalent" names to): the shim must keep compiling until its last consumer is refactored.
#include <iostream>

#include "codebreaker/exact/data.hpp"
#include "codebreaker/exact/library.hpp"
#include "codebreaker/exact/linkwhole.hpp"
#include "codebreaker/exact/pascal_join.hpp"

namespace {
[[maybe_unused]] void use_pascal_join() {
    codebreaker::exact::pascal_join::Project project;
    (void)codebreaker::exact::pascal_join::run(project, false);
}

// the shims that call the old toolchain, DOS emulator and omf ports
[[maybe_unused]] void use_linkwhole(const codebreaker::exact::linkwhole::Project& project) {
    namespace lw = codebreaker::exact::linkwhole;
    const auto objects = lw::modules(project);
    (void)lw::listed(project, project.link());
    (void)lw::run(project, objects, std::nullopt);
    (void)lw::main(project, py::list<py::str>(), std::cout);
}

[[maybe_unused]] void use_data(const codebreaker::exact::data::Project& project, const codebreaker::exact::modules::Row& row) {
    (void)codebreaker::exact::data::draft(project, row);
    (void)codebreaker::exact::data::main(project, py::list<py::str>());
}

[[maybe_unused]] void use_library(const codebreaker::exact::library::Project& project) {
    namespace lib = codebreaker::exact::library;
    (void)lib::modules(project);
    (void)lib::lines(*lib::locate(project));
    (void)lib::main(project, py::list<py::str>());
}
}  // namespace
