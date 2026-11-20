// translate/frontends/clang/j2x.hpp -- C++ written by the Java lane read back as the Java it came from: the library half of the
// Java -> C++ -> Java round trip (docs/TRANSLATOR_DESIGN.md, phase 5 exit check).
//
// The clang front end lowers that C++ like any other; what it cannot know is that `JavaString`, `JArray<T>`, `JCat() + "x" + n`,
// `System::out.println(...)` and `j2xToInt(d)` are the Java runtime written in C++. This pass maps them back onto Java's own library in
// the IR -- the shapes the javac front end gives (java.lang.String, arrays, Concat, java.io.PrintStream calls, numeric casts) -- so the
// Java back end writes Java again:
//
//     JavaString / JCat / std::string     -> java.lang.String          JArray<T>            -> T[]
//     JavaString(x)._s, JavaString(x)     -> x                         JCat() + a + b       -> Concat(a, b)
//     arr[i] (JArray::operator[])         -> Index                     arr.length / ->length -> Length
//     System::out / System::err           -> System.out / System.err   println / print      -> PrintStream's
//     j2xToInt / j2xToLong (saturating)   -> Java's (int) / (long)     javaMathSqrt & co    -> Math.sqrt ...
//     j2xUshr(v, n)                       -> v >>> n                   s_name (a static)     -> name
//     initStatics()                       -> the class's static initialiser
//
// and the sequencing temporaries the C++ back end adds to keep Java's left-to-right order (`int j2xo1 = 0; ... f((j2xo1 = a, j2xo2 = b,
// ...))`) are folded back into their declarations. What it does not know stays as it is, and the Java back end refuses it by name.
//
//     j2x::lift(arena, units);                                    // in place; new nodes are made in `arena`
//     j2x::bind(host);                                            // host.lift_java_runtime of the native round-trip judge
#pragma once

#include <span>

#include "core/arena.hpp"
#include "translate/ir.hpp"
#include "translate/judge_native_jvm.hpp"

namespace cb::translate::frontends::clang::j2x {

// A type as the Java it came from: the runtime's string classes are java.lang.String, JArray<T> and a string literal's char array are
// arrays or strings, the runtime's PrintStream is java.io.PrintStream. An absent type stays absent.
ir::TypeP lift_type(const ir::TypeP& type);

// The Java lane's C++ (its units, from the clang front end) read back as Java's library, in place.
void lift(Arena& arena, std::span<ir::Node* const> units);

// Make `lift` the host's `lift_java_runtime`, the step the native round-trip judge needs before the Java back end.
void bind(judge_native_jvm::Host& host);

}  // namespace cb::translate::frontends::clang::j2x
