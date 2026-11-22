// translate/backends/csharp_java.hpp -- the C# back end's knowledge of Java: Java's library and Java's semantics, in C#.
//
// A Java unit (Unit::lang "java", from the javac front end) is written by the C# back end (csharp.hpp) with this as its
// library. It maps Java's types onto .NET's (java.lang.String -> string, java.util.List -> List<T>, java.util.function.Function ->
// Func<T, R>, ...), Java's library calls onto .NET's through the API-mapping registry (api_maps/java.csharp.json: a template per
// call; JavaRt, the small Java runtime in C#, where .NET's call means something else), and Java's semantics where C#'s differ:
//
//   * dispatch: a Java instance method is virtual unless final, private or static -- `virtual`, and `override` where it overrides a
//     class of these units or Object (toString -> ToString, equals -> Equals, hashCode -> GetHashCode); `main(String[])` is C#'s
//     `Main`; an abstract class that leaves an interface's method to its subclasses declares it abstract (C# requires it);
//   * construction order: Java runs instance field initialisers and instance blocks after the base constructor, C# before it;
//     where the base is a class of these units (whose constructor could see them) they are moved into each constructor after its
//     base call. Static initialisers and static blocks keep Java's one textual order, in a static constructor;
//   * text: every value Java turns into text differently (doubles, floats, booleans, null, collections) goes through JavaRt.Str;
//   * HashMap and HashSet iterate in hash order, Dictionary and HashSet<T> in another: iterating one is declined (TreeMap and
//     TreeSet map to the sorted collections, whose order is Java's);
//   * Java's boxed types map to .NET's values (Integer -> int): a boxed value that is null is declined.
//
// What it cannot carry it declines (csharp::Decline), with the reason; the C# back end turns that into a ledgered gap.
//
//     const apimap::Registry registry = apimap::Registry::load(api_maps_dir);
//     csharp::Options options;
//     options.libraries["java"] = csharp_java::factory(registry);
//     csharp::Program program(units, options);
//
// A library holds what it learns of one program (which fields it moved into constructors, the class a local's value was made
// of), so one is made per Program (the factory does).
#pragma once

#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "java/apimap.hpp"
#include "translate/backends/csharp.hpp"

namespace cb::translate::backends::csharp_java {

namespace cs = cb::translate::backends::csharp;
namespace ir = cb::translate::ir;

// Java's library and semantics, for the C# back end. The registry must outlive it.
class JavaLibrary final : public cs::Library {
public:
    explicit JavaLibrary(const cb::java::apimap::Registry& registry) : registry_(registry) {}

    // An abstract Java class that leaves an interface's method to its subclasses declares it abstract in C#.
    void prepare(cs::Program& program) override;

    // ---- types
    std::optional<std::string> type_name(cs::Context& w, const ir::TypeP& type) override;
    bool keep_interface(const ir::TypeP& type) override;

    // ---- declarations
    void check_class(cs::Context& w, ir::ClassDecl& c) override;
    std::vector<std::string> class_mods(cs::Context& w, ir::ClassDecl& c) override;
    std::vector<std::string> field_mods(cs::Context& w, ir::FieldDecl& f, ir::ClassDecl& c) override;
    std::vector<std::string> method_mods(cs::Context& w, ir::MethodDecl& m, ir::ClassDecl& c) override;
    std::string method_name(cs::Context& w, ir::MethodDecl& m, ir::ClassDecl& c) override;
    std::string member_name(cs::Context& w, const ir::Sym& s) override;
    // Java's initialisation order: instance initialisers after the base constructor (moved into the constructors, where the base
    // is one of these units' classes), statics in one textual order (in a static constructor, when there is a static block).
    std::vector<ir::Node*> arrange_members(cs::Context& w, ir::ClassDecl& c, std::vector<ir::Node*> members) override;
    bool ctor_needed(cs::Context& w, ir::ClassDecl& c, ir::MethodDecl& m) override;
    std::vector<ir::Node*> ctor_prologue(cs::Context& w, ir::ClassDecl& c, ir::MethodDecl& m, ir::CtorCall* call) override;

    // ---- statements and expressions
    std::optional<cs::Ex> select(cs::Context& w, ir::Node* e) override;
    std::optional<cs::Ex> call(cs::Context& w, ir::Node* e, bool statement) override;
    std::optional<std::string> stmt_call(cs::Context& w, ir::Node* e) override;
    std::optional<std::string> concat_part(cs::Context& w, ir::Node* part) override;
    std::optional<cs::Ex> new_object(cs::Context& w, ir::Node* e) override;
    std::optional<cs::Ex> binary(cs::Context& w, ir::Node* e) override;
    std::optional<std::string> coerce(cs::Context& w, ir::Node* e, const ir::TypeP& type) override;
    std::optional<std::string> iterable(cs::Context& w, ir::Node* e) override;
    void record(cs::Context& w, const ir::SymP& sym, ir::Node* init) override;
    void check_lambda(cs::Context& w, ir::Lambda& lambda, const ir::TypeP& type) override;
    // `Type::method`, and any reference into Java's library, as the lambda it stands for.
    ir::Node* method_ref(cs::Context& w, ir::Node* e) override;
    // A Java record is written as a sealed class: javac's members for it (its accessors, equals, hashCode, toString) are in the tree.
    std::optional<std::string> class_keyword(const ir::ClassDecl& c) override;
    std::optional<std::string> this_text(cs::Context& w, ir::Node* e) override;
    std::optional<std::string> name_text(cs::Context& w, ir::Node* e) override;
    std::optional<cs::Ex> update(cs::Context& w, ir::Node* e) override;
    bool local_class(const ir::Node* decl) const override { return hoist_of_.contains(decl); }
    std::vector<std::string> extra_members(cs::Context& w, ir::ClassDecl& c) override;
    bool enum_as_class(const ir::ClassDecl& c) const override { return c.qname && object_enums_.contains(*c.qname); }
    std::optional<std::string> enum_case(const ir::TypeP& scrutinee, const std::string& name) const override;

private:
    // A box that can be null. Java's Integer is C#'s int; the variables, fields, parameters and results that may hold a null (found by
    // following nulls through the program) are `int?`. Reading one where a number is wanted goes through JLib.Unbox, which throws as
    // Java does; the places that keep the null (a test against null, text, another such variable) read it as it is.
    void prepare_boxed_nulls(cs::Program& program, const std::vector<ir::Node*>& classes);
    bool may_be_null(ir::Node* e) const;
    std::string kept_null(cs::Context& w, ir::Node* e);       // the expression as a `T?`, its null kept
    std::optional<std::string> unboxed_read(cs::Context& w, ir::Node* e);
    std::unordered_set<const ir::Node*> null_reads_;          // the reads that can give a null
    std::unordered_set<const ir::Node*> read_as_is_;          // those being written with their null kept (or assigned to)

    // A class Java declares in a method (a local or anonymous one), as C# has none: a member of the class around it, with a field
    // `cap_x` for each local it reads, given to each constructor. One written for a library functional interface is handed over as
    // a delegate to its one method.
    struct Hoisted {
        ir::ClassDecl* decl = nullptr;
        std::vector<std::pair<std::string, ir::TypeP>> captures;
        std::vector<ir::SymP> fields;
        std::string delegate_method;
        ir::TypeP delegate_type;
    };
    void prepare_local_classes(cs::Program& program, std::vector<ir::Node*>& classes);
    void find_local_captures();
    void add_capture_fields(cs::Program& program);
    const Hoisted* hoisted(const ir::ClassDecl& c) const;
    bool has_captures(const ir::ClassDecl& c) const;
    // An inner class that reaches its enclosing instance holds it in a field `outer_`, given to each constructor first.
    void prepare_inner_classes(cs::Program& program, const std::vector<ir::Node*>& classes);
    // The path from the class being written to the instance of `qname` it is inside ("outer_", "outer_.outer_"): empty when it is `this`.
    std::string outer_path(cs::Context& w, const std::string& qname) const;
    bool needs_outer(const ir::ClassDecl& c) const { return c.qname && inner_.contains(*c.qname); }
    std::optional<cs::Ex> new_inner(cs::Context& w, ir::Node* e, const ir::ClassDecl& c);

    // A call into java.util.stream, Optional, Comparator's combinators and the few statics a record's members use: these go to
    // JavaStreams.cs, which keeps Java's method names. Nothing for any other call.
    std::optional<cs::Ex> stream_call(cs::Context& w, ir::Node* e);
    // Declines a call that changes a list Java would refuse to change (Arrays.asList's size, Stream.toList's anything).
    void check_list_change(const ir::Call& call) const;
    // What the list an expression holds cannot do: empty, "fixed" or "immutable".
    std::string_view list_limit(ir::Node* e) const;
    // The mapping row of a library call, and the row's C# extras (null when the registry has no row).
    struct Row {
        std::optional<cb::java::apimap::Mapping> mapping;
        const cb::java::apimap::TargetMap* raw = nullptr;
    };
    Row lookup(const ir::Call& call) const;
    // The class the object a name holds was made of (a Map that is a TreeMap), else its declared type's.
    std::optional<std::string> concrete(ir::Node* e) const;
    // The template of a row filled with the call's target and arguments.
    std::string fill(cs::Context& w, const std::string& tmpl, const ir::Call& call);
    // `a` as Java turns it into text (String.valueOf).
    std::string string_of(cs::Context& w, ir::Node* a);
    bool closeable(cs::Context& w, const ir::ClassDecl& c) const;
    bool is_const(const ir::FieldDecl& f) const;
    std::vector<std::string> access(const std::vector<std::string>& mods, const ir::ClassDecl& c) const;
    bool overrides(cs::Context& w, const ir::ClassDecl& c, const ir::MethodDecl& m) const;

    const cb::java::apimap::Registry& registry_;
    std::unordered_set<const ir::FieldDecl*> moved_;                      // fields whose initialiser moved into a constructor or the static one
    std::unordered_map<std::string, std::vector<ir::Node*>> prologue_;    // class -> the statements its constructors run after the base call
    std::map<std::pair<std::string, std::string>, std::string> concrete_;   // (owner, name) -> the class the value held was made of
    std::unordered_map<std::string, ir::SymP> inner_;                     // an inner class that needs its outer instance -> its `outer_` field
    std::unordered_map<std::string, Hoisted> hoisted_;                    // a class declared in a method, by its qualified name here
    std::unordered_map<const ir::Node*, ir::ClassDecl*> hoist_of_;        // the tree's ClassDecl -> the class written for it
    std::unordered_map<const ir::Node*, std::string> refused_;            // the tree's ClassDecl -> why it has no class
    std::unordered_map<const ir::Node*, std::string> capture_names_;      // a Name that reads a captured local -> the field
    std::unordered_map<const ir::Node*, std::set<std::string>> via_field_;   // a `new` -> the captures it passes from the class's own fields
    std::unordered_map<std::string, std::vector<std::string>> object_enums_;   // an enum written as a class -> its constants, in order
    std::unordered_map<const ir::Node*, std::pair<std::string, int>> enum_made_;   // the `new` of a constant -> its name and ordinal
    std::unordered_set<const ir::Node*> before_base_;                     // a `new` in this(...) or super(...): its captures are parameters there
};

// The factory the C# back end's Options take for "java".
cs::LibraryFactory factory(const cb::java::apimap::Registry& registry);

}  // namespace cb::translate::backends::csharp_java
