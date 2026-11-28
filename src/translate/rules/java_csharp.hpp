// translate/rules/java_csharp.hpp -- the rule set java of the C# back end (docs/JAVA_TO_CSHARP_L2.md, step 3): Java units restated as
// C#-shaped L2, which the back end's plain lowering (dotnet/decompiler/csharp/l2_syntax_builder) writes.
//
//     Arena arena;
//     const apimap::Registry registry = apimap::Registry::load(api_maps_dir);
//     java_csharp::Result r = java_csharp::lower(arena, units, registry);   // copies: the units are not changed
//     // r.units: lang "java", every type and member .NET's; r.runtime: the runtime files (JavaRt ...) the units use
//
// What it carries over from Java (the knowledge translate/backends/csharp_java holds as text, here as L2):
//   * types: java.lang.String is System.String, java.util.List<T> System.Collections.Generic.List<T>, a functional interface the
//     delegate System.Func<...> / System.Action<...>, a box the number it holds, char a UTF-16 char;
//   * dispatch: a Java instance method is `virtual` unless final, private or static, `override` where it overrides a class of the
//     units or Object's (toString -> ToString ...), `main(String[])` is `Main`;
//   * construction order: instance initialisers after the base constructor, statics in one static constructor in Java's order;
//   * the library: each call through the API-mapping registry, whose C# templates are read into L2 here (`{t}` the target, `{0}` an
//     argument ...), the rest by rules (text conversions through JavaRt.Str, Java's reference comparison of strings and boxes, ...);
//   * statements: labelled break and continue as goto to labels after and inside the loop, try-with-resources with Java's suppressed
//     exceptions, a finally that jumps written after its try.
//
// What it cannot carry it declines: a member whose body holds such a construct gets a body of one Unsupported node (the back end writes
// the gap and its ledger line), a class one Unsupported member in place of the class. Not yet carried (each declined, by name): classes
// declared in a method or anonymous, inner classes that reach their outer instance, lambdas of the program's own interfaces, enums of
// objects, java.util.stream, boxes that may hold a null, the reflection classes.
#pragma once

#include <set>
#include <span>
#include <string>
#include <vector>

#include "core/arena.hpp"
#include "java/apimap.hpp"
#include "translate/ir.hpp"

namespace cb::translate::rules::java_csharp {

struct Result {
    std::vector<ir::Node*> units;   // in the arena given
    std::set<std::string> runtime;  // the runtime files the units use, by name ("JavaRt", "JavaCollections" ...)
};

// The Java units (Unit::lang "java") restated for the C# back end; units of any other language are passed through as they are. The
// registry must outlive the call.
Result lower(Arena& arena, std::span<ir::Node* const> units, const cb::java::apimap::Registry& registry);

}  // namespace cb::translate::rules::java_csharp
