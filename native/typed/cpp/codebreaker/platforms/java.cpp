// codebreaker/platforms/java.cpp -- typed C++ of codebreaker.platforms.java (see java.hpp).

#include "codebreaker/platforms/java.hpp"

#include "codebreaker/verify/lanes/java.hpp"
#include "py_os.hpp"

namespace codebreaker::platforms::java {

bool _recognise(const py::str& path) {
    if (path.lower().endswith(py::str(".jar"))) return true;
    if (py::os::path::isdir(path) || path.lower().endswith(py::str(".java"))) {
        return verify::lanes::java::is_module(path);
    }
    return false;
}

bool recognises(const py::str& path) {
    try {
        return _recognise(path);
    } catch (const py::OSError&) {
        return false;
    } catch (const py::ValueError&) {
        return false;
    }
}

static Platform make_platform() {
    Platform p;
    p.name = py::str("java");
    p.title = py::str("Java");
    p.family = py::str("bytecode");
    p.source = py::str("java -> c++");
    p.lane = py::str("codebreaker.verify.lanes.java");
    p.cpu.language = py::str("");
    p.cpu.decoder = py::str("codebreaker.decompile.jar");
    p.loader.container = py::str("source folder, jar");
    p.convention.names = py::list<py::str>{py::str("the JVM's")};
    p.convention.order = py::str("c");
    p.convention.pops = py::str("callee");
    p.emulator.engine = py::str("the JVM");
    p.compiler.language = py::str("c++");
    p.compiler.toolchain = py::str("the host C++ compiler (host_cxx, else g++)");
    p.compiler.tool = Ref{py::str("codebreaker.verify.lanes.java"), py::str("compiler"), py::list<py::str>()};
    p.packager.tools = Ref{py::str("codebreaker.verify.lanes.java"), py::str("jdk"), py::list<py::str>()};
    return p;
}

const Platform& PLATFORM() {
    static const Platform p = make_platform();
    return p;
}

}  // namespace codebreaker::platforms::java
