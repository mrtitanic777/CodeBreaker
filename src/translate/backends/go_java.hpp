// translate/backends/go_java.hpp -- Go out for Java programs, beside backends/rust_java (the Rust writer this one follows) and backends/c_java.
//
// A whole Java program from L2 as one Go source file on the runtime config/translate/runtime/go (package jrt, which the program imports as
// `javaprog/jrt`; the build writes a go.mod beside it). Go has a collector, interfaces and panics, so the program reads close to the Java:
//
//   * Java's int, long, short, byte, char, float and double are int32, int64, int16, int8, uint16, float32 and float64, whose arithmetic wraps
//     as Java's does (a division by zero is a panic the runtime turns into ArithmeticException); a shift masks its count as Java's does;
//   * a String is `jrt.String` (UTF-16, nullable), Object and the boxed numbers are `any`, an array is a slice, null is nil;
//   * a class is an interface (`Animal`, what a variable of that type holds), a struct (`AnimalObj`, with the struct of its superclass
//     embedded and a `self` field holding the object as its interface, so a call inside a base class method reaches the override) and a
//     constructor function (`NewAnimal`); a Java interface is a Go interface; an instance method is a method of the struct whose receiver
//     is `this`; a static member is a package-level function or variable named `Class_member`;
//   * an exception is a panic carrying a `jrt.Throwable`; try/catch/finally run their bodies as closures under `jrt.Try`, with a flow
//     variable carrying a return, break or continue out of them;
//   * a for loop, while loop, switch and labeled jump are Go's own.
//
// What it does not write is a `Decline` (translate/backends/cpp03.hpp's): the library classes the runtime has no word for, records,
// lambdas, anonymous and inner classes, an evaluation order Go leaves open. The judge (translate_file) reports such a program as refused.
//
//     Arena arena;  std::vector<ir::Node*> units = ...;                 // Unit nodes, as the Java front end makes them
//     std::string go = go_java::program(arena, units, "Main");          // the program's Go text (units are lowered in place)
//     go_java::Built b = go_java::build(arena, units, out_dir);         // ... and compiled with go build
//
// The modules: go_java.cpp (the entry points and the build), go_java_program.cpp (types, names, classes), go_java_stmt.cpp (statements),
// go_java_expr.cpp (expressions, calls, the library). go_java_internal.hpp is what they share.
#pragma once

#include <chrono>
#include <filesystem>
#include <optional>
#include <span>
#include <string>
#include <string_view>

#include "core/arena.hpp"
#include "translate/backends/cpp03.hpp"
#include "translate/ir.hpp"

namespace cb::translate::backends::go_java {

namespace ir = cb::translate::ir;

// The program is one the Go back end does not write (the reason says why). It is cpp03's: the back ends share what they decline.
using cpp03::Decline;

// The folder of the Go runtime (jrt*.go) in the checkout this program belongs to.
std::filesystem::path default_runtime_dir();

// go: CB_GO, else the PATH, else C:/Program Files/Go/bin or /usr/local/go/bin; nullopt when there is none.
std::optional<std::filesystem::path> find_go();

// The Go text of the whole program `units` (Unit nodes of one Java program). The modern forms Go has no word for are lowered first, in the
// units' own arena. `main` names the class whose main runs (its qualified or simple name); empty: the only class with a main. Throws Decline.
std::string program(Arena& arena, std::span<ir::Node* const> units, const std::string& main = {});

struct BuildOptions {
    std::filesystem::path runtime_dir;               // empty: default_runtime_dir()
    std::optional<std::filesystem::path> compiler;   // empty: find_go()
    std::string main;                                // the main class, as program() takes it
    std::chrono::seconds timeout{600};
};

// What building a program gave: the executable, or why not (a decline, or the compiler's first error).
struct Built {
    std::optional<std::filesystem::path> exe;
    std::string why;
};

// Write the program under `out` (program.go, go.mod and jrt/ beside it) and build it into out/program.exe. A decline is a Built with no exe.
Built build(Arena& arena, std::span<ir::Node* const> units, const std::filesystem::path& out, const BuildOptions& options = {});

}  // namespace cb::translate::backends::go_java
