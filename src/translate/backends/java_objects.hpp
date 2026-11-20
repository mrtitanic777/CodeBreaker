// translate/backends/java_objects.hpp -- the Java back end's idiomatic tier: C structs and C++ classes as Java objects.
//
// Chosen when the pointer analysis (java::Analysis) finds every pointer disciplined: no pointer arithmetic, no casts between pointer
// types, no unions, no raw memory functions, no address of an element or a field. Then:
//
//   * a struct or class is a Java class (a nested static class of the program), its fields Java fields; a C++ class keeps its
//     methods, constructors (base initialiser as super(...)), single inheritance, virtual and abstract methods, static members;
//     operators become named methods (operator+ -> plus);
//   * a pointer to a struct is a reference (NULL is null), `p->f` is `p.f`;
//   * an array is a Java array, and so is a pointer to a scalar: it can only index (`*p` is `p[0]`);
//   * a scalar whose address is taken is boxed in a one-element array (`int[] n = {5}`), so `&n` is `n` and `*p = 3` is `p[0] = 3` --
//     C's out-parameters and C++'s `int &` work as they did;
//   * struct values keep C's copy semantics: assignment copies into the object (`a.set(b)`), passing or initialising by value copies
//     (`new Point(p)`, the copy constructor), aggregates are constructors;
//   * malloc(sizeof(T)) is `new T()`, malloc(n * sizeof(T)) `new T[n]`; free is the collector's, `delete` runs a destructor;
//   * function pointers are functional interfaces (`IntIntToInt cmp = Prog::compare;`).
// Arithmetic, conversions, statements and C's library are the faithful tier's (java::FnWriter).
//
// What it cannot write it refuses (java::Refused): the program when the fault is the program's (a union, a bit-field, a local with
// a destructor that must run at the end of its scope), the one function or method as a gap when it is not (a method that returns a
// reference to a scalar, a library call the runtime does not have).
//
//     java_objects::factory(&library)        // the TierFactory java::Options takes as `idiomatic`
//     java::program(units, "Prog", {.idiomatic = java_objects::factory(&library), .library = &library});
#pragma once

#include <string>
#include <string_view>

#include "translate/backends/java.hpp"

namespace cb::translate::backends::java_objects {

namespace jb = cb::translate::backends::java;

// The Java method an operator is (`operator+` -> `plus`), and any other name as a Java identifier. A C++ operator the table does
// not name is refused.
std::string method_name(std::string_view name);

// The idiomatic tier for java::Options. `library` is how C library calls are written (java_libc), and must outlive the programs
// the factory's writers write; without one every library call is refused.
jb::TierFactory factory(const jb::LibraryCalls* library);

}  // namespace cb::translate::backends::java_objects
