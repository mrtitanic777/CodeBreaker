// codebreaker/translate/backends/jmodel.hpp -- typed C++ port of codebreaker.translate.backends.jmodel.
//
// jmodel.py -- a Java program's object model read off L2, for the back ends that write whole programs
// (c_java.Program; the C++17 back end's Java rules have it in C++, native/l2/src/cpp17_jmodel.cc).
//
// What every such back end has to know and Java leaves implicit: every class (nested, local, anonymous and
// enum-constant bodies included) with its C name, its supertypes, whether it needs its outer instance, what
// a local or anonymous class captures; which method a method overrides (so its erased signature is the one
// the root declares, as javac's bridge methods make it); the one abstract method of a functional interface;
// which classes can form a reference cycle (where reference counting leaks and a collector is needed).
//
// Library types are named by their qualified Java name; LIB_SUPERS gives their supertypes and LIB_METHODS the
// methods a program class can override, each as javac would type it (type variables erase to Object).
//
// Typing notes:
//   * The IR is codebreaker/translate/ir.hpp. A ClassInfo holds the real ClassDecl, MethodDecl, EnumConst and
//     New nodes; the attributes the Python sets after construction (package, method, new) are members of
//     ClassInfo, null / None until set. `new` is a C++ keyword: ClassInfo::new_.
//   * A qualified name is a py::str. Where the Python reads one off an IR slot that may be None (a class
//     Type's name, a Sym's owner, a MethodDecl's name) and uses it as a lookup key, an unset one reads as ''
//     (no class has the key '' and no library table has it, so every lookup answers as for None).
//   * (name, params, result) triples are std::tuple<py::str, py::list<TypeP>, TypeP> (Sig).
//   * Python ints are int64_t.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "py.hpp"
#include "codebreaker/translate/ir.hpp"

namespace codebreaker::translate::backends::jmodel {

namespace ir = codebreaker::translate::ir;

using TypeP = ir::TypeP;
// (name, params, result) of a method as javac would type it
using Sig = std::tuple<py::str, py::list<TypeP>, TypeP>;

extern const py::str OBJECT;

// library supertypes (enough for subtype tests and for override roots)
extern const py::dict<py::str, py::list<py::str>> LIB_SUPERS;
// the one abstract method of each library functional interface: (name, params, result), as erased
extern const py::dict<py::str, Sig> SAMS;
// methods of library supertypes a program class can override: {type: [(name, params, result)]}
extern const py::dict<py::str, py::list<Sig>> LIB_METHODS;

// One class of the program.
struct ClassInfo : py::object {
    py::ref<ir::ClassDecl> decl;
    py::str qname;
    py::str cname;
    py::ref<ClassInfo> outer;                    // the enclosing ClassInfo (lexically)
    bool local;                                  // declared in a method body
    bool anon;
    std::optional<py::str> kind;
    // an inner class (not static, inside a class) holds its outer instance; so does a local or
    // anonymous class declared where `this` exists (set by Model once it knows the method)
    bool inner;
    py::list<std::tuple<py::str, TypeP>> captures;   // the locals a local/anonymous class reads
    bool needs_outer;
    std::optional<py::str> base;                 // a qualified name: program or library
    py::list<py::str> ifaces;
    py::ref<ir::EnumConst> enum_const;           // an enum constant's body: the EnumConst
    // set by Model after construction
    std::optional<py::str> package;
    py::ref<ir::MethodDecl> method;              // the method a local/anonymous class sits in
    py::ref<ir::New> new_;                       // the `new` expression of an anonymous class

    ClassInfo(const py::ref<ir::ClassDecl>& decl, const py::str& qname, const py::str& cname,
              const py::ref<ClassInfo>& outer = nullptr, bool local = false, bool anon = false);

    ir::Nodes members() const;
    py::list<py::ref<ir::MethodDecl>> methods(const std::optional<py::str>& name = std::nullopt) const;
    py::list<py::ref<ir::FieldDecl>> fields() const;
    py::str repr() const;
};

// A type with its type variables and wildcards as Object (Java's erasure, near enough: a bounded
// variable is Object here too, and its uses are cast to what javac said).
TypeP erase(const TypeP& t);

struct Model : py::object {
    py::list<py::ref<ir::Unit>> units;
    py::dict<py::str, py::ref<ClassInfo>> classes;     // qname (or a made-up key) -> ClassInfo
    py::dict<int64_t, py::ref<ClassInfo>> by_decl;     // id(ClassDecl) -> ClassInfo (the node's address)
    int64_t _anon = 0;
    py::set<py::str> packages;

    explicit Model(const py::list<py::ref<ir::Unit>>& units, bool package_names = true);

    // ---- the classes
    py::str _cname(const py::str& qname, const std::optional<py::str>& package) const;
    py::ref<ClassInfo> _add(const py::ref<ir::ClassDecl>& c, const py::ref<ClassInfo>& outer,
                            const std::optional<py::str>& package, bool local = false, bool anon = false,
                            const py::ref<ir::MethodDecl>& method = nullptr);
    // Local and anonymous classes inside a member's code.
    void _nested_in(const ir::NodeP& m, const py::ref<ClassInfo>& ci, const std::optional<py::str>& package);
    void _supers(const py::ref<ClassInfo>& ci);
    // The locals of the enclosing method a local or anonymous class reads (Java copies them in).
    void _capture(const py::ref<ClassInfo>& ci);
    // javac names an anonymous (or local) class's members as owned by '' -- give them the class's key,
    // so a field or method of one is found like any other's.
    void name_anonymous();

    // ---- lookups
    py::ref<ClassInfo> info(const py::str& qname) const;
    py::ref<ClassInfo> of_type(const TypeP& t) const;
    // The direct supertypes of a class, program or library.
    py::list<py::str> supers(const py::str& qname) const;
    py::list<py::str> ancestors(const py::str& qname) const;
    bool is_sub(const py::str& a, const py::str& b) const;
    bool is_interface(const py::str& qname) const;
    bool subclassed(const py::str& qname) const;
    // The program's MethodDecl a resolved method symbol names, searching up the supertypes, or (None, None).
    std::tuple<py::ref<ClassInfo>, py::ref<ir::MethodDecl>> method_decl(const ir::SymP& sym) const;
    // The root signature method `m` of class `ci` overrides: (params, result) as the topmost declaration
    // has them, or None when it overrides nothing (or is static, private, a constructor).
    std::optional<std::tuple<py::list<TypeP>, TypeP>> overrides(const py::ref<ClassInfo>& ci,
                                                               const py::ref<ir::MethodDecl>& m) const;
    // (name, params, result) of a functional interface's one abstract method, or None.
    std::optional<Sig> sam(const py::str& qname) const;
    // Classes whose instance fields can lead back to an instance of the same class (directly or
    // through other program classes, arrays and collections): reference counting cannot free such a
    // cycle; a collector (Boehm) would be needed. -> sorted C names
    py::list<py::str> cycles() const;
};

// A parameter of an overridden method (super_t, as declared) and of the overriding one (t).
bool _matches(const TypeP& super_t, const TypeP& t);
// The class names a field type can hold a reference to.
py::list<py::str> _refs(const TypeP& t);

// ---------------------------------------------------------------------------------------------- the runtimes' names
// library classes -> the runtime's class names (C++17's jrt.hpp: native/l2/src/cpp17_java.cc has the
// same table; c_java's C runtime), in this order (the first library class of a runtime name is the one
// it stands for)
extern const py::dict<py::str, py::str> LIB;

// A Java char as a char16_t literal. (The Python takes a one-character str or a number.)
py::str char_literal(const py::str& c);
py::str char_literal(int64_t c);

// A static field whose initialiser is a literal number, char or boolean (no class initialisation needed).
bool constant_init(const py::ref<ir::FieldDecl>& f);

}  // namespace codebreaker::translate::backends::jmodel
