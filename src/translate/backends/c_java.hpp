// translate/backends/c_java.hpp -- C out for Java programs (docs/TRANSLATOR_DESIGN.md, phase 6), beside backends/c (which writes
// the decompiler's C).
//
// A whole Java program from L2 as one C11 translation unit on the runtime codebreaker/translate/runtime/c (jrt.h, jrt.c), in the
// faithful tier of the design's table:
//
//   * a class is a struct whose first member is its superclass's struct (`B super_;`), so a pointer to it is a pointer to each
//     base; its fields follow;
//   * its virtual methods are a function table (`struct X_vt`): Object's three slots, then every slot of its program bases in the
//     same order, then its own -- so a table is a prefix of each subclass's; a call goes through a small inline helper (X_m_v)
//     that reads the table once;
//   * an interface method is found through the class's interface tables (itabs), so a class implements any number of them;
//     lambdas and method references are closure records -- a struct holding the captured values (Java captures only
//     effectively-final locals: copies) -- implementing the functional interface;
//   * a constructor is X_init_N(self, ...), run after allocation as the JVM runs it (fields zero first, so a virtual call in a
//     constructor reaches the override); X_new_N allocates and initialises;
//   * static state is globals; a class with a static initialiser runs it on first use (X_clinit);
//   * exceptions are setjmp/longjmp through a handler chain (jrt_try); a return or jump out of a try pops it; locals of a function
//     with a try are volatile (their values survive the longjmp);
//   * generics are erased; a value read through a type variable is cast (checked) to what javac said;
//   * Java's integer semantics: -fwrapv, masked shift counts, `/` and `%` by zero throw, (int) of a double saturates; Java's
//     left-to-right operand order where C's is unspecified (cpp03's rule);
//   * memory: objects are never freed while the program runs (jrt.h says why, and where a collector would go).
//
// Modern constructs C has no word for are lowered first (translate/desugar: for-each, switch expressions, switches on strings,
// instanceof patterns). What it cannot write is a `Decline`; the judge (translate/javajudge) reports that program as refused.
//
//     Arena arena;  std::vector<ir::Node*> units = ...;               // Unit nodes, as the Java front end makes them
//     std::string c = c_java::program(arena, units);                  // the program's C text (units are lowered in place)
//     c_java::Built b = c_java::build(arena, units, out_dir);         // ... and compiled against the runtime
//
// The modules: c_java.cpp (the entry points), c_java_names.cpp (C names, types and literals), c_java_program.cpp (the program's
// classes: structs, tables, prototypes), c_java_method.cpp (members: constructors, initialisers), c_java_stmt.cpp (statements),
// c_java_expr.cpp and c_java_call.cpp (expressions, calls and `new`), c_java_closure.cpp (lambdas and method references).
// c_java_internal.hpp is what they share.
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

namespace cb::translate::backends::c_java {

namespace ir = cb::translate::ir;

// The program is one the C back end does not write (the reason says why). It is cpp03's: the back ends share what they decline.
using cpp03::Decline;

// The C compiler the build uses when it is installed (Visual Studio's clang), else the host's gcc, and the flags it is given.
inline constexpr std::string_view kClang = R"(C:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\Llvm\x64\bin\clang.exe)";
inline constexpr std::string_view kFlags[] = {"-std=c11", "-fwrapv", "-O1", "-w"};

// The folder of the C runtime (jrt.h, jrt.c) in the checkout this program belongs to. NotFoundError when there is none.
std::filesystem::path default_runtime_dir();

// The C text of the whole program `units` (Unit nodes of one Java program). The modern forms C has no word for are lowered
// first, in the units' own arena (`arena`: the one they were made in). `main` is the class whose main runs when the environment
// names none. Throws Decline.
std::string program(Arena& arena, std::span<ir::Node* const> units, const std::optional<std::string>& main = std::nullopt);

struct BuildOptions {
    std::filesystem::path runtime_dir;               // empty: default_runtime_dir()
    std::optional<std::filesystem::path> compiler;   // empty: kClang if it exists, else gcc
    std::chrono::seconds timeout{600};
};

// What building a program gave: the executable, or why not (a decline, or the compiler's first error).
struct Built {
    std::optional<std::filesystem::path> exe;
    std::string why;
};

// Write the program as C under `out` (program.c) and compile it with clang (else the host's gcc) against the runtime into
// out/program.exe. A decline is a Built with no exe.
Built build(Arena& arena, std::span<ir::Node* const> units, const std::filesystem::path& out, const BuildOptions& options = {});

}  // namespace cb::translate::backends::c_java
