// Compile-only check of the shims at the old header paths of formats/xbox/god (the native reader's device and the GDF-based operations of
// main()) and java/probe (the settings and corpus, the compile-and-run entry points): what reaches old ports that cannot be linked into
// cb_tests. Nothing here runs; it is in cb_shim_check, so the shims stay honest until their last consumer is refactored.
#include "codebreaker/formats/xbox/god.hpp"
#include "codebreaker/java/probe.hpp"

namespace shim_check_w1b16 {

inline void touch() {
    namespace god = codebreaker::formats::xbox::god;
    god::SvodReader reader(py::str("00004000/CAFEF00D.data"));
    (void)reader.paths;
    (void)reader.read_at(0, 16);
    (void)god::_template();
    god::main(py::list<py::str>{py::str("info"), py::str("x")});
    god::main(py::list<py::str>{py::str("iso2god"), py::str("x.iso"), py::str("out")});
    god::main(py::list<py::str>{py::str("xex2god"), py::str("default.xex"), py::str("out")});
    god::main(py::list<py::str>{py::str("god2iso"), py::str("x"), py::str("out.iso")});

    namespace probe = codebreaker::java::probe;
    (void)probe::libraries();
    (void)probe::shims(py::str("work"), py::str("javac"));
    (void)probe::stage_sources(py::str("src"), py::str("staged"));
    (void)probe::library(py::str("g++"), py::str("cpp"), py::str("work"));
    (void)probe::build_native(py::str("g++"), py::str("cpp"), py::str("probe.cpp"), py::str("probe.exe"), py::str("libj2x.a"));
    const probe::Todo todo = probe::plan(py::str("staged"), py::str("cpp"), py::list<py::str>{py::str("Foo")});
    const probe::Compiled compiled = probe::compile_java(py::str("staged"), todo, py::str("work"), py::str("javac"));
    py::ref<codebreaker::java::ir::JClass> cls;
    (void)probe::run(py::str("staged"), py::str("cpp"), cls, py::str("work"), py::str("g++"), py::str("javac"), py::str("java"), std::nullopt, compiled);
}

}  // namespace shim_check_w1b16
