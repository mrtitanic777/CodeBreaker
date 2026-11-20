// translate/backends/csharp_cpp17.hpp -- the C++17 back end for C# programs: the C# front end's IR written as one C++17 translation
// unit against config/translate/runtime/cpp17/csrt.hpp (docs/TRANSLATOR_DESIGN.md, "C# -> C++17").
//
//     csharp_cpp17::Output out = csharp_cpp17::write(units);        // units from frontends/csharp, lang "csharp"
//     if (out.declined.empty()) compile(out.source);                 // else out.declined says what has no form yet and where
//
// What a C# program means is carried by the runtime: a class is a struct held through std::shared_ptr (cs::Ref<T>), a struct a C++
// value, a string a UTF-8 std::string, `int` wraps (the translation is compiled with -fwrapv, and arithmetic that C# defines as
// wrapping on narrower types is written with explicit casts), shifts mask their count, `/` and `%` throw DivideByZeroException,
// a double converts to an integer as C# does (cs::to_int), `checked` traps (cs::checked_*). A property is a field and its
// accessors; a delegate is std::function; List<T>, Dictionary<K,V>, StringBuilder and the LINQ operators are in the runtime.
//
// A construct without a faithful C++ form is declined with the reason, never guessed: the whole program is then not written.
#pragma once

#include <span>
#include <string>
#include <vector>

#include "translate/ir.hpp"

namespace cb::translate::backends::csharp_cpp17 {

namespace ir = cb::translate::ir;

// What was declined: where (Class.member), the kind of construct, and why.
struct Gap {
    std::string where, kind, why;
    std::string text() const { return where + " " + kind + ": " + why; }
};

struct Output {
    std::string source;          // the C++17 text (empty when anything was declined)
    std::vector<Gap> declined;
};

// The units of one program (every file) as one translation unit; the entry point is the Main of the class named `main` (any class that
// has a static Main when empty).
Output write(std::span<ir::Node* const> units, const std::string& main = "");

}  // namespace cb::translate::backends::csharp_cpp17
