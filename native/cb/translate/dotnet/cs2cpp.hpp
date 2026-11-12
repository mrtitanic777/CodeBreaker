// cb/translate/dotnet/cs2cpp.hpp -- C# into the C++03 back end: the C# kit's L2 written by backends/cpp03.
//
// The C++03 back end (the Xbox 360 path) is L2-driven for bodies only. Three things about it are still Java's, and this module is what a
// C# source has to cross to reach it (docs/TRANSLATOR_DESIGN.md, wave 3: "checks the IR is not Java-shaped"):
//
//   1. its declarations come from Java source text: the converter (cb/java) parses .java files for classes, fields and signatures, and
//      asks the back end only for bodies. So a C# program is given a Java declaration skeleton, written here from its L2 (signatures, no
//      bodies), and the converter's front-end call is handed the C# kit's L2 units instead of javac's;
//   2. its runtime is Java's library in C++ (JavaString, JList, java.lang.Math, System.out): C#'s library calls are rebound to the Java
//      calls of the same meaning (Console.WriteLine -> System.out.println, List<T> -> java.util.ArrayList, s.Length -> s.length(), ...),
//      and C#'s text conversions are written out where Java's differ (a bool is "True"/"False" in C#: a conditional);
//   3. it knows no properties: a C# property becomes a field and get/set methods, and every read and write of it a call (C#'s own
//      extension lowered into the core, as the design asks).
//
// What has no faithful form on that path is refused with the reason, never guessed: structs (value semantics), records, generics of the
// program's own, delegates and lambdas (C++03), unsigned integers (Java's runtime has none), nested classes (the converter's body lookup is
// by top-level class), using, lock, goto, checked arithmetic, doubles turned into text (C#'s format is not Java's), and any BCL call not
// in the table.
//
//     cb::Arena arena;                                              // the units were made in it
//     auto done = cs2cpp::convert(arena, units, work, converter);   // units from the Roslyn front end
//     // done.cpp_dir, done.refused, done.stats
//
// The pass rewrites the units in place, in the arena they live in: the nodes it makes (a lowered property, a counted loop, an `equals`
// call) go into that arena and are linked into the units' own trees, never into another's. The units are the caller's, and so is the
// arena; both must outlive the conversion.
//
// The modules: cs2cpp.cpp (the tables, type and symbol mapping, the program), cs2cpp_decl.cpp (classes, members, properties),
// cs2cpp_stmt.cpp (statements), cs2cpp_expr.cpp (expressions and library calls), cs2cpp_skeleton.cpp (the Java text, the conversion).
#pragma once

#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "cb/core/arena.hpp"
#include "cb/core/error.hpp"
#include "cb/core/ordered_map.hpp"
#include "cb/java/transpile.hpp"
#include "cb/translate/ir.hpp"

namespace cb::translate::dotnet::cs2cpp {

namespace ir = cb::translate::ir;

// A construct with no faithful form on the C++03 path. `category` names the kind (`unsigned`, `generic`, `library` ...), `why` says what.
// Caught where one class or one member is isolated from the rest of the program.
class Refuse : public Error {
public:
    Refuse(std::string category, std::string why) : Error(why), category_(std::move(category)), why_(std::move(why)) {}
    const std::string& category() const { return category_; }
    const std::string& why() const { return why_; }
    CB_ERROR_BOILERPLATE(Refuse)

private:
    std::string category_;
    std::string why_;
};

// What was refused: where (a class, or Class.member; none when the source gives no name), the kind, and why.
struct Refusal {
    std::optional<std::string> where;
    std::string kind;
    std::string why;
};

// A property's accessors: the getter, the setter (null for a get-only one) and whether it is static.
struct Accessors {
    ir::SymP getter;
    ir::SymP setter;
    bool is_static = false;
};

// Rebind C# L2 to Java's library and lower C#'s properties into methods, in place.
class ToJava {
public:
    ToJava(Arena& arena, std::span<ir::Node* const> units);

    // Refuses what has no form (a class or a member at a time), lowers the rest; the refusals, in the order met.
    std::vector<Refusal> run();

private:
    using PropKey = std::pair<std::string, std::string>;   // (owner, name)

    // ---- types and symbols (cs2cpp.cpp)
    ir::TypeP ty(const ir::TypeP& t, bool boxed = false);
    ir::SymP sym(const ir::SymP& s);

    // ---- the program (cs2cpp.cpp) and its classes (cs2cpp_decl.cpp)
    void check_class(const ir::Node& c);
    void collect_props(const ir::ClassDecl& c);
    // A property's accessors, looked up through the classes it is inherited from.
    std::optional<Accessors> prop(const std::optional<std::string>& owner, const std::string& name) const;
    void klass(ir::Node& c);
    std::vector<std::string> mods(const std::vector<std::string>& mods) const;
    std::vector<ir::Node*> member(ir::Node* m, const ir::ClassDecl& c);
    std::vector<ir::Node*> lower_prop(ir::Node* p, const ir::ClassDecl& c);
    ir::Node* method(ir::Node* m, const ir::ClassDecl& c);

    // ---- statements (cs2cpp_stmt.cpp)
    ir::Node* block(ir::Node* b);
    ir::Node* stmt(ir::Node* s);
    ir::Node* stmt_slot(const ir::Suite& suite);
    // foreach over an array or a List: a counted loop (the C++03 back end writes no for-each; decompiled Java never has one). Over a
    // List it is the same while the body does not change the list, where C# would throw.
    ir::Node* foreach_(ir::Node* s);
    // switch on a string: the C++03 back end switches on integers only (decompiled Java switches on the hash). As tests in order: each
    // C# section ends in a jump, so none falls into the next.
    ir::Node* string_switch(ir::Node* s);
    ir::Node* set_prop(ir::Node* e);
    ir::Node* set_index(ir::Node* e);

    // ---- expressions (cs2cpp_expr.cpp)
    ir::Node* expr(ir::Node* e);
    ir::Node* typed(ir::Node* e);
    ir::Node* call(ir::Node* target, ir::SymP sym, std::vector<ir::Node*> args, ir::TypeP type);
    // p as C# turns it into text, written in terms Java's concatenation gives the same.
    ir::Node* text(ir::Node* p);
    ir::Node* write_select(ir::Node* e);
    ir::Node* write_index(ir::Node* e);
    ir::Node* write_new(ir::Node* e);
    ir::Node* write_call(ir::Node* e);
    ir::Node* write_console(const ir::Sym& s, const std::vector<ir::Node*>& args);
    ir::Node* write_string_call(const ir::Node& e, const ir::Call& c, const ir::Sym& s, const std::vector<ir::Node*>& args);
    ir::Node* write_list_call(const ir::Node& e, const ir::Call& c, const ir::Sym& s, const std::vector<ir::Node*>& args);

    // a node of kind K made in the arena, typed as given, placed where `from` is
    template <class K> ir::Node* make(K kind, ir::TypeP type = {}, const ir::Node* from = nullptr) {
        return ir::make(arena_, std::move(kind), std::move(type), from != nullptr ? from->line : 0, from != nullptr ? from->col : 0);
    }

    Arena& arena_;
    std::vector<ir::Node*> units_;
    OrderedMap<std::string, ir::Node*> classes_;                  // the program's classes by qualified name (a refused one stays)
    std::map<PropKey, Accessors> props_;                          // (owner, name) -> accessors
    std::map<PropKey, ir::SymP> auto_;                            // (owner, name) -> an auto-property's field
    std::vector<Refusal> refused_;
    int loops_ = 0;
};

// ---------------------------------------------------------------------------------------------- Java text
// A type as Java writes it (`int`, `String`, `ArrayList<String>`, `int[]`), and ` false` / ` 0` / ` null` / `` for a default return.
std::string java_type(const ir::TypeP& type);
std::string default_value(const ir::TypeP& type);
// The Java declarations of a class (no bodies: the converter takes those from L2).
std::string skeleton(const ir::Node& cls);

// ---------------------------------------------------------------------------------------------- the conversion
// What the converter answers for the skeleton and the units: its summary and, for the C++03 back end it ran, why bodies were declined
// (the full reason and the first-word group, each with its count) -- `has_back_end` false when none ran.
struct Converted {
    cb::java::transpile::Summary stats;
    bool has_back_end = false;
    OrderedMap<std::string, std::size_t> reasons;
    OrderedMap<std::string, std::size_t> declined;
};
// Runs the Java converter over the skeleton folder, with the C# units as its front end's answer (the units are lowered to Java's
// library already). Given: the skeleton folder, the output folder, the units.
using Converter = std::function<Converted(const std::filesystem::path& skeleton_dir, const std::filesystem::path& out_dir,
                                          std::span<ir::Node* const> units)>;

struct Conversion {
    std::filesystem::path cpp_dir;
    std::vector<Refusal> refused;
    cb::java::transpile::Summary stats;
};
// C# L2 units -> the C++03 back end's program in work/cpp.
Conversion convert(Arena& arena, std::span<ir::Node* const> units, const std::filesystem::path& work, const Converter& converter);

// The C++03 runtime's System.out is a no-op (a console has no standard output); for the judge on the host, a PrintStream that writes, with
// Java's text for each kind of value. True when JavaRT.h had the no-op to replace.
bool host_print(const std::filesystem::path& cpp_dir);

}  // namespace cb::translate::dotnet::cs2cpp
