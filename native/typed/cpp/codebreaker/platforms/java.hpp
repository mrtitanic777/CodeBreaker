// codebreaker/platforms/java.hpp -- typed C++ of codebreaker.platforms.java
//
// Java: a folder of source (or a .jar), translated to C++ (codebreaker/java) and judged by
// running both under the JVM.
//
// Typing notes:
//   * The platforms package has no typed port of its own (the dos/vxd/win16/msx ports hold their PLATFORM as
//     plain records too). PLATFORM() is a record of the values the Python's six slots hold: a Ref is
//     (module, name, leading arguments), a Cpu its language id plus fields. A slot field the Python leaves
//     out (None) is an empty std::optional. Built on first use.
//   * Java's Cpu has no bits / endian / registers / stack, and its decoder is the name of one of
//     CodeBreaker's own modules (a str), not a capstone tuple.
//   * _recognise() is the Python's: a .jar, or a folder / .java file the java lane (typed
//     codebreaker::verify::lanes::java::is_module) says is a module.
#pragma once

#include <cstdint>
#include <optional>

#include "py.hpp"

namespace codebreaker::platforms::java {

// platforms.Ref: a function of another module, named here and imported when first used
struct Ref {
    py::str module;
    py::str name;
    py::list<py::str> args;
};

struct Cpu {
    py::str language;                       // "" (Java has no SLEIGH language)
    std::optional<py::str> decoder;         // a module of CodeBreaker's own
};

struct Convention {
    py::list<py::str> names;
    py::str order;
    py::str pops;
};

struct Loader {
    py::str container;
};

struct Emulator {
    py::str engine;
};

struct Compiler {
    py::str language, toolchain;
    Ref tool;
};

struct Packager {
    Ref tools;
};

struct Platform {
    py::str name, title, family, source, lane;
    Cpu cpu;
    Loader loader;
    Convention convention;
    Emulator emulator;
    Compiler compiler;
    Packager packager;
};

bool _recognise(const py::str& path);

// Platform.recognises: _recognise, with OSError and ValueError meaning "no"
bool recognises(const py::str& path);

const Platform& PLATFORM();

}  // namespace codebreaker::platforms::java
