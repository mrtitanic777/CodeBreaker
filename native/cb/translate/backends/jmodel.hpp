// cb/translate/backends/jmodel.hpp -- a Java program's object model read off L2, for the back ends that write whole
// programs (the C back end's Java rules, the C++17 back end's).
//
// What every such back end has to know and Java leaves implicit: every class (nested, local, anonymous and enum-constant
// bodies included) with its C name, its supertypes, whether it needs its outer instance, what a local or anonymous class
// captures; which method a method overrides (so its erased signature is the one the root declares, as javac's bridge
// methods make it); the one abstract method of a functional interface; which classes can form a reference cycle (where
// reference counting leaks and a collector is needed).
//
//     Arena arena;  ir::Node* unit = ...;                          // a Java compilation unit, as the front end makes it
//     jmodel::Model model(std::span<ir::Node* const>(&unit, 1));
//     const jmodel::ClassInfo* c = model.info("pkg.Outer.Inner");
//     c->cname;                                                    // "Outer_Inner"
//     model.overrides(*c, method_node);                            // the root's signature, if it overrides
//     model.cycles();                                              // C names of the classes that can form a cycle
//
// Library types are named by their qualified Java name; library_supers gives their supertypes and library_methods the
// methods a program class can override, each as javac would type it (type variables erase to Object).
//
// A Model points into the trees of the units it was made from: they must outlive it. name_anonymous() is the one thing
// that changes a tree.
#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "cb/core/ordered_map.hpp"
#include "cb/translate/ir.hpp"

namespace cb::translate::backends::jmodel {

inline constexpr std::string_view kObject = "java.lang.Object";

// ---- the library --------------------------------------------------------------------------------------------------------

// A method as javac would type it: its name, its parameters and its result.
struct Signature {
    std::string name;
    std::vector<ir::TypeP> params;
    ir::TypeP result;
};

// The direct supertypes of a library type (enough for subtype tests and override roots); none for one not listed.
const std::vector<std::string>& library_supers(std::string_view qname);
// The library exception and error types, in the table's order.
const std::vector<std::string>& library_throwables();
// The one abstract method of a library functional interface, as erased; null for another type.
const Signature* library_functional(std::string_view qname);
// The methods of a library supertype a program class can override (the functional interfaces' one method last).
const std::vector<Signature>& library_methods(std::string_view qname);
// The runtime's class name for a library class (the C and C++17 runtimes share it); none for a class it has no name for.
std::optional<std::string_view> runtime_name(std::string_view qname);

// ---- the program --------------------------------------------------------------------------------------------------------

// A local a local or anonymous class reads from its enclosing method (Java copies it in).
struct Capture {
    std::string name;
    ir::TypeP type;
};

// One class of the program.
struct ClassInfo {
    ir::Node* node = nullptr;                    // its ClassDecl
    ir::ClassDecl* decl = nullptr;
    std::string qname;                           // the qualified name, or a made-up key `<cname>` for a local or anonymous class
    std::string cname;                           // its name in C
    ClassInfo* outer = nullptr;                  // the enclosing class (lexically)
    bool local = false;                          // declared in a method body
    bool anon = false;
    std::string kind;                            // class interface enum record annotation ...
    // An inner class (not static, inside a class) holds its outer instance; so does a local or anonymous class declared
    // where `this` exists (decided once the method it sits in is known).
    bool inner = false;
    bool needs_outer = false;
    std::vector<Capture> captures;
    std::optional<std::string> base;             // a qualified name: program or library
    std::vector<std::string> ifaces;
    ir::Node* enum_const = nullptr;              // an enum constant's body: the EnumConst
    std::string package;
    ir::Node* method = nullptr;                  // the MethodDecl a local or anonymous class sits in
    ir::Node* creation = nullptr;                // the `New` of an anonymous class

    // Its members, and the methods (and constructors) and fields among them; `name` selects methods by name.
    const std::vector<ir::Node*>& members() const { return decl->members; }
    std::vector<ir::Node*> methods(std::optional<std::string_view> name = std::nullopt) const;
    std::vector<ir::Node*> fields() const;
};

// A method symbol resolved to the program's declaration.
struct MethodMatch {
    ClassInfo* cls = nullptr;
    ir::Node* method = nullptr;                  // its MethodDecl
};

// The signature a method overrides, as the topmost declaration has it.
struct Override {
    std::vector<ir::TypeP> params;
    ir::TypeP result;
};

// A type with its type variables and wildcards as Object (Java's erasure, near enough: a bounded variable is Object here
// too, and its uses are cast to what javac said). An absent type is void.
ir::TypeP erase(const ir::TypeP& type);

class Model {
public:
    explicit Model(std::span<ir::Node* const> units);
    Model(const Model&) = delete;
    Model& operator=(const Model&) = delete;

    // Every class, by qualified name (or made-up key), in the order found.
    const OrderedMap<std::string, ClassInfo*>& classes() const { return classes_; }
    // Every package a unit declares.
    const std::vector<std::string>& packages() const { return packages_; }

    ClassInfo* info(std::string_view qname) const;
    ClassInfo* of_decl(const ir::Node* class_decl) const;
    ClassInfo* of_type(const ir::TypeP& type) const;

    // The direct supertypes of a class, program or library.
    std::vector<std::string> supers(std::string_view qname) const;
    // The class and every supertype above it, nearest first.
    std::vector<std::string> ancestors(std::string_view qname) const;
    bool is_sub(std::string_view a, std::string_view b) const;
    bool is_interface(std::string_view qname) const;
    bool subclassed(std::string_view qname) const;

    // The program's MethodDecl a resolved method or constructor symbol names, searching up the supertypes.
    std::optional<MethodMatch> method_decl(const ir::Sym& sym) const;
    // The root signature that method `method` (a MethodDecl) of `cls` overrides, or none when it overrides nothing (or is
    // static, private, a constructor).
    std::optional<Override> overrides(const ClassInfo& cls, const ir::Node* method) const;
    // A functional interface's one abstract method, or none.
    std::optional<Signature> functional(std::string_view qname) const;

    // Classes whose instance fields can lead back to an instance of the same class (directly or through other program
    // classes, arrays and collections): reference counting cannot free such a cycle; a collector would be needed.
    // Sorted C names.
    std::vector<std::string> cycles() const;

    // javac names an anonymous (or local) class's members as owned by '' -- this gives them the class's key, so a field or
    // method of one is found like any other's. Changes the trees.
    void name_anonymous();

private:
    ClassInfo& add(ir::Node* class_decl, ClassInfo* outer, const std::string& package, bool local = false, bool anon = false,
                   ir::Node* in_method = nullptr);
    void nested_in(ir::Node* member, ClassInfo& cls, const std::string& package);
    void find_supers(ClassInfo& cls);
    void find_captures(ClassInfo& cls);

    std::vector<std::unique_ptr<ClassInfo>> owned_;
    OrderedMap<std::string, ClassInfo*> classes_;
    std::unordered_map<const ir::Node*, ClassInfo*> by_decl_;
    std::vector<std::string> packages_;
    std::size_t anonymous_ = 0;
};

// A character as a char16_t literal in generated C++.
std::string char_literal(std::uint32_t code);

// A static field whose initialiser is a literal number, char or boolean (no class initialisation needed).
bool constant_init(const ir::FieldDecl& field);

}  // namespace cb::translate::backends::jmodel
