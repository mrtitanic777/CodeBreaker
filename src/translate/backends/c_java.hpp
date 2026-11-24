// translate/backends/c_java.hpp -- C out for Java programs (docs/TRANSLATOR_DESIGN.md, phase 6), beside backends/c (which writes
// the decompiler's C).
//
// A whole Java program from L2 as C11 on the runtime codebreaker/translate/runtime/c (jrt.h, jrt.c), in the faithful tier of the
// design's table. The files mirror the Java program's: for each top-level class Foo (in the folder of its package) a Foo.h, with its
// struct, function table and the declarations the other files call, and a Foo.c, with its code and static data; the classes nested in
// Foo (and its local and anonymous ones) stay with it. jprogram.h holds what all of them share (a typedef for each class, the string
// literals more than one file uses), jprogram.c the definitions of those, jmain.c the entry point. Each .c is compiled on its own.
//
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
//     std::vector<c_java::File> c = c_java::files(arena, units);      // the program's files (units are lowered in place)
//     c_java::Built b = c_java::build(arena, units, out_dir);         // ... written, compiled side by side and linked with the runtime
//
// The modules: c_java.cpp (the entry points, the build), c_java_names.cpp (C names, types and literals), c_java_program.cpp (the program's
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
#include <vector>

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

// One file of the program: its path under the program's folder (with `/`) and its text.
struct File {
    std::string path;
    std::string text;
};

// The files of the whole program `units` (Unit nodes of one Java program). The modern forms C has no word for are lowered
// first, in the units' own arena (`arena`: the one they were made in). `main` is the class whose main runs when the environment
// names none. Throws Decline.
std::vector<File> files(Arena& arena, std::span<ir::Node* const> units, const std::optional<std::string>& main = std::nullopt);

// The same files as one text, each under a line `// ==== path ====` (the goldens' form).
std::string program(Arena& arena, std::span<ir::Node* const> units, const std::optional<std::string>& main = std::nullopt);

struct BuildOptions {
    std::filesystem::path runtime_dir;               // empty: default_runtime_dir()
    std::optional<std::filesystem::path> compiler;   // empty: kClang if it exists, else gcc
    std::chrono::seconds timeout{600};
    int jobs = 0;                                    // compilers at a time; 0: one for each core, at most 8
};

// What building a program gave: the executable, or why not (a decline, or the compiler's first error).
struct Built {
    std::optional<std::filesystem::path> exe;
    std::string why;
    std::filesystem::path sources;                   // the folder the files were written to (out/src)
    std::vector<std::filesystem::path> files;        // the files, relative to `sources`
};

// Write the program's files under `out` (out/src), compile each .c on its own with clang (else the host's gcc), up to `jobs` at a
// time, together with the runtime's, and link them into out/program.exe. A decline is a Built with no exe.
Built build(Arena& arena, std::span<ir::Node* const> units, const std::filesystem::path& out, const BuildOptions& options = {});

// Hand a built program over: its files and the runtime's under `out`, with a BUILD.txt and a CMakeLists.txt that build them. Returns what
// it wrote, relative to `out`.
std::vector<std::filesystem::path> install(const Built& built, const std::string& name, const std::filesystem::path& out,
                                           const std::filesystem::path& runtime_dir = {});

}  // namespace cb::translate::backends::c_java
