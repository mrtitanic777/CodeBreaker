// java/frontend.hpp -- the Java front end: javac's typed tree of a program, lowered into the translator's IR.
//
//     Arena arena;
//     std::vector<ir::Node*> units = frontend::lower_program(arena, java_dir, tools);          // what assembly::FrontEnd gives
//     ir::Node* unit = frontend::lower_unit(arena, tree_json, "net/minecraft/src/Block.java");   // one compilation unit
//
// Step one is javac's, driven from here: `TreeDump` (codebreaker/translate/frontends/java/TreeDump.java, compiled once into the state folder
// and keyed by its source and the JDK) runs javac over the source tree -- parse and attribute, no class files -- and writes every node of every
// compilation unit with what javac resolved: each expression's type, the declaration each name, call and `new` refers to (with the overload
// chosen). A node is a JSON object: "k" its kind (javac's Tree.Kind name: METHOD_INVOCATION, IF, PLUS ...), "l" and "c" its line and column,
// "$type" the resolved type of an expression or declaration, "$sym" the element a name refers to ({"kind", "name", "owner", "static", "ret",
// "params" ...}), and its children under javac's property names ("methodSelect", "arguments", "condition", "thenStatement" ...).
//
// Step two is `lower_unit`. Every name is lowered from the symbol javac resolved, not from its spelling: an IDENTIFIER that javac bound to a field
// becomes a Select on This (or on its class, when static), one bound to a local a Name; a MEMBER_SELECT bound to a class is a TypeName, `x.length`
// on an array a Length, `X.class` a ClassLit. A constructor call this(...) / super(...) is a CtorCall statement; the ones javac inserted (a
// constructor with no explicit call, a class with no constructor) are marked implicit. A record gets what javac writes only when it lowers it
// (after the tree TreeDump sees): the canonical constructor's field assignments, an accessor per component, equals, hashCode and toString, as
// java.lang.runtime.ObjectMethods defines them, each marked GENERATED. Anything not yet handled becomes Unsupported(kind), never a guess.
#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "core/arena.hpp"
#include "core/json_fwd.hpp"
#include "translate/ir.hpp"

namespace cb::java::frontend {

// A javac type string (`int[][]`, `java.util.Map<K,java.util.List<V>>`, `<nulltype>`), as an IR type. Absent text: the unresolved type.
translate::ir::TypeP parse_type(std::optional<std::string_view> text);

// One compilation unit's TreeDump JSON as an ir::Unit made in `arena`. `path` is the unit's source path, as the dump names it. ParseError for a
// dump that is not what TreeDump writes.
translate::ir::Node* lower_unit(Arena& arena, const Json& tree, std::string_view path);

// Where javac is and what it compiles against.
struct DumpTools {
    std::filesystem::path java;                       // the JDK's java ...
    std::filesystem::path javac;                      // ... and javac
    std::filesystem::path treedump_source;            // TreeDump.java
    std::filesystem::path state;                      // the folder the compiled TreeDump is kept in (<state>/translate)
    std::vector<std::filesystem::path> classpath;     // the jars (and the stand-ins' folder) the program is attributed against
};

// The Java sources under `java_dir`, sorted.
std::vector<std::filesystem::path> java_files(const std::filesystem::path& java_dir);

// javac's typed tree of every compilation unit of `java_dir`, lowered; the units in the order of their paths. ToolError when TreeDump cannot be
// compiled or run (the first of javac's messages with it).
std::vector<translate::ir::Node*> lower_program(Arena& arena, const std::filesystem::path& java_dir, const DumpTools& tools);

}  // namespace cb::java::frontend
