// translate/backends/rust_java.hpp -- Rust out for Java programs, beside backends/c_java (the C writer this one follows: no classes of its own to
// lean on, one runtime).
//
// A whole Java program from L2 as one Rust source file on the runtime config/translate/runtime/rust/jrt.rs, which the program includes by path
// (`#[path = "runtime/jrt.rs"] mod jrt;`):
//
//   * a reference is `Option<&'static T>` (jrt.rs says why: objects are made once and not freed while the program runs), so values are Copy
//     and `this` is `self`; a Java class is a trait (`Ref<dyn Animal>` is a variable of type Animal), its fields a data struct
//     (`AnimalData`, in `Cell`s), and each class that can be made a concrete struct (`AnimalObj`) holding the data of every class it extends;
//   * an instance method is a trait method; its body is an associated function of the class's data struct taking `this`, which is what
//     `super.m()` calls and what the concrete classes' trait impls call, so overriding and virtual calls are the trait's;
//   * a class with nothing but static members is a unit struct with those as associated functions and consts (`Hello::main`);
//   * a static field that is a constant is an associated const; any other is a `jstatic!` made on first use;
//   * Java's integer arithmetic is `wrapping_*`, `/` and `%` by anything but a nonzero literal go through `jdiv`/`jrem` (they throw
//     ArithmeticException), `as` is Java's cast; a String is `Str` (UTF-16, nullable, Copy);
//   * an exception is an unwind carrying a Throwable (jrt.rs `throw`, `try_catch`); a `finally` runs after the body's closure ended, so
//     it may use what the body used.
//
// What it does not write is a `Decline` (translate/backends/cpp03.hpp's): generic classes and the library classes the runtime has no word
// for, lambdas, inner and anonymous classes, enums, records, switches that fall through, a break or continue out of a try. The judge
// (translate_file) reports such a program as refused.
//
//     Arena arena;  std::vector<ir::Node*> units = ...;                 // Unit nodes, as the Java front end makes them
//     std::string rs = rust_java::program(arena, units, "Main");        // the program's Rust text (units are lowered in place)
//     rust_java::Built b = rust_java::build(arena, units, out_dir);     // ... and compiled with rustc
//
// The modules: rust_java.cpp (the entry points and the build), rust_java_names.cpp (types, names, literals), rust_java_program.cpp (the classes),
// rust_java_stmt.cpp (statements), rust_java_expr.cpp (expressions), rust_java_call.cpp (calls, `new`, the library). rust_java_internal.hpp is what
// they share.
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

namespace cb::translate::backends::rust_java {

namespace ir = cb::translate::ir;

// The program is one the Rust back end does not write (the reason says why). It is cpp03's: the back ends share what they decline.
using cpp03::Decline;

// The folder of the Rust runtime (jrt.rs) in the checkout this program belongs to.
std::filesystem::path default_runtime_dir();

// rustc: CB_RUSTC, else the PATH, else ~/.cargo/bin; nullopt when there is none.
std::optional<std::filesystem::path> find_rustc();

// The flags the build gives rustc.
inline constexpr std::string_view kFlags[] = {"--edition", "2021", "-O", "-C", "debuginfo=0", "-A", "warnings"};

// The Rust text of the whole program `units` (Unit nodes of one Java program). The modern forms Rust has no word for are lowered first, in
// the units' own arena. `main` names the class whose main runs (its qualified or simple name); empty: the only class with a main. Throws Decline.
std::string program(Arena& arena, std::span<ir::Node* const> units, const std::string& main = {});

struct BuildOptions {
    std::filesystem::path runtime_dir;               // empty: default_runtime_dir()
    std::optional<std::filesystem::path> compiler;   // empty: find_rustc()
    std::string main;                                // the main class, as program() takes it
    std::chrono::seconds timeout{600};
};

// What building a program gave: the executable, or why not (a decline, or the compiler's first error).
struct Built {
    std::optional<std::filesystem::path> exe;
    std::string why;
};

// Write the program as Rust under `out` (program.rs, with runtime/jrt.rs beside it) and compile it into out/program.exe. A decline is a
// Built with no exe.
Built build(Arena& arena, std::span<ir::Node* const> units, const std::filesystem::path& out, const BuildOptions& options = {});

}  // namespace cb::translate::backends::rust_java
