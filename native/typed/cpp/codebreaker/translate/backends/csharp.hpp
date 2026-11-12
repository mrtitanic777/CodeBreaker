// codebreaker/translate/backends/csharp.hpp -- typed C++ port of codebreaker.translate.backends.csharp.
//
// NOT YET A SHIM (W1-B27): the back end is now native/cb/translate/backends/csharp.{hpp,cpp} (typed IR, no py::). This port stays, unchanged,
// for one reason: codebreaker/translate/backends/csharp_java.* (the Java library of this back end, the next unit to be refactored) is written
// against this port's Writer and the OLD IR, and cannot compile against the new module until it becomes a cb::translate::backends::csharp::Library.
// When csharp_java is refactored this port and csharp_java's become shims over the two new modules (old IR tree -> L2 through
// cb/compat/old_l2_tree.hpp -> cb csharp::Program) and this note goes.
//
// csharp.py -- the translator's C# back end: whole programs from the IR, in C# as a person writes it.
//
// It reads any front end's units -- each says its source language (Unit.lang) -- and writes one .cs file
// per unit. Where the languages line up the output is idiomatic: properties, `using`, `lock`, generics,
// lambdas and delegates, `foreach`, `is` patterns, switch expressions, extension-method calls written on
// their receiver, operators written as operators. Where they do not, it writes the faithful form:
//
//   * from Java: a method that Java would dispatch virtually is `virtual` (and `override` where it
//     overrides); labelled break and continue are gotos to labels after and inside the loop; a switch
//     section that falls through ends in `goto case`; instance field initialisers run after the base
//     constructor, as Java runs them; a constant expression that Java lets wrap is `unchecked`; every
//     string conversion Java makes differently from .NET (doubles, floats, booleans, null) goes through
//     the small Java runtime in C# (codebreaker/translate/dotnet/runtime/JavaRt.cs), as do the library
//     calls whose meaning differs (java.util.Random, Math.round, Math.abs, List.remove(int), Map.get on
//     a missing key, String.hashCode, ...). The mapping of Java's library onto .NET's is
//     codebreaker/java/api_maps/java.csharp.json, read through the API-mapping registry;
//   * from C#: the source's own forms.
//
// What it cannot write faithfully it declines, never guesses: a method body holding something it does
// not carry becomes `throw new System.NotSupportedException("CB-GAP: ...")`, listed in the ledger with
// the reason; a declaration it cannot carry (a Java inner class that uses its outer instance, an
// anonymous class) is a ledger entry and the unit is declined.
//
//     auto prog = csharp::write(units);
//     auto files = prog->files();      // {file name: text}, the runtime's files included when used
//     prog->ledger;                    // [(where, kind, why)]
//
// Typing notes (how the Python's values are typed here):
//   * The IR is codebreaker/translate/ir.hpp; `units` is a list of Unit nodes (NodeP).
//   * csharp_java.py imports this module's Decline, ident, precedences, _strip and is_string, and csharp.py
//     imports csharp_java lazily (an import cycle): in C++ the two are headers csharp.hpp / csharp_java.hpp
//     (JavaLib is declared here, defined there), and Writer holds the JavaLib (null for a C# unit).
//   * An expression's (text, precedence) pair is Ex. The Python's `_strip` is strip; the private names
//     _PRIM, _ALIAS, _BIN_PREC, _OPERATOR_METHODS, _UNARY_METHODS, _DECL_OPERATORS, _RANGE, _LIB_CONSTS,
//     _cstr, _cchar, _float, _lib_const, _is_const, _overflows, _abrupt, _loop_breaks, _sig,
//     _initializers, _is_name, _uses, _replace, _interpolatable, _inferable stay in the .cpp.
//   * Program.inits (id(FieldDecl) -> the initialiser written, None: moved elsewhere) is a map keyed by the
//     node's address: a present key with a null node is the Python's None. The attributes csharp_java sets
//     on the Program (_java_moved, _java_prepared, _concrete) are members of Program.
//   * `_Init` (the back end's own node for a hoisted object or collection initialiser) is the struct Init.
//     Python's `_replace` reads and writes the slots of whatever node holds the old one; here it tries each
//     node class of the IR in turn.
//   * Constant folding in `_overflows` is done in int64_t (the Python's ints are unbounded): a result that
//     leaves int64 is reported as leaving the type's range and wraps as the type does; a u64 value above
//     int64's maximum is not represented (the front ends do not produce one).
//   * Python ints are int64_t.
#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <tuple>
#include <utility>
#include <vector>

#include "codebreaker/translate/ir.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::translate::backends::csharp {

namespace ir = codebreaker::translate::ir;
using ir::NodeP;
using ir::SymP;
using ir::TypeP;

class JavaLib;
class Writer;

// the folder of csharp.py (default: where this source file is); RUNTIME() is dotnet/runtime beside backends
void set_here(const py::str& here);
py::str RUNTIME();

extern const py::set<py::str> KEYWORDS;

// precedence (C# 12 spec, highest first)
constexpr int64_t P_PRIMARY = 16, P_UNARY = 15, P_MUL = 14, P_ADD = 13, P_SHIFT = 12, P_REL = 11, P_EQ = 10, P_AND = 9,
                  P_XOR = 8, P_OR = 7, P_LAND = 6, P_LOR = 5, P_COALESCE = 4, P_COND = 3, P_ASSIGN = 2;

// This construct is not written; the method (or unit) becomes a ledgered gap.
struct Decline : py::Exception {
    py::str kind;
    py::str why;
    explicit Decline(const py::str& kind, const std::optional<py::str>& why_ = std::nullopt)
        : py::Exception(why_ && !why_->empty() ? *why_ : kind), kind(kind), why(why_ && !why_->empty() ? *why_ : kind) {}
    const char* type() const override { return "Decline"; }
};

py::str ident(const py::str& name);
bool is_string(const TypeP& t);
// the expression with its Paren wrappers taken off
NodeP strip(const NodeP& e);

// an expression's text and the precedence of its outermost operator
using Ex = std::pair<py::str, int64_t>;

// -------------------------------------------------------------------------------------- the program
// The units to write, what they declare, and what came of writing them.
class Program : public py::object {
  public:
    py::list<NodeP> units;
    py::dict<py::str, py::ref<ir::ClassDecl>> classes;
    py::list<std::tuple<py::str, py::str, py::str>> ledger;   // (where, kind, why)
    py::set<py::str> runtime;
    std::map<const ir::Node*, NodeP> inits;       // id(FieldDecl) -> the initialiser written (null: moved elsewhere)

    // what csharp_java keeps on the Program
    std::set<const ir::Node*> java_moved;                         // FieldDecls whose initialiser moved
    std::map<std::string, py::list<NodeP>> java_prologue;     // class qname -> statements run after the base constructor
    bool java_prepared = false;
    std::map<std::string, py::str> concrete;                  // (owner, name) of a local or field -> class made where declared

    explicit Program(const py::list<NodeP>& units);

    // {file name: text}, the runtime's files included when used
    py::dict<py::str, py::str> files();
    // writes the files into out_dir; the paths
    py::list<py::str> write(const py::str& out_dir);

    // the class hierarchy, for virtual/override from a source whose methods are virtual by default
    py::ref<ir::ClassDecl> base_of(const py::ref<ir::ClassDecl>& c) const;
    // Does method m of class c override one of a class it extends (in these units)?
    bool overrides(const py::ref<ir::ClassDecl>& c, const py::ref<ir::MethodDecl>& m) const;

  private:
    void index_(const NodeP& c);
    std::optional<py::dict<py::str, py::str>> files_;
};

// -> Program (call .files() or .write(out_dir))
py::ref<Program> write(const py::list<NodeP>& units);

// -------------------------------------------------------------------------------------- the writer
class Writer {
  public:
    using ClassP = py::ref<ir::ClassDecl>;
    using MethodP = py::ref<ir::MethodDecl>;

    Program& prog;
    py::ref<ir::Unit> u;
    py::str lang;
    std::shared_ptr<JavaLib> java;            // the Java library, for a Java unit (null for C#)
    py::set<py::str> usings;
    ClassP cls;
    MethodP method;
    std::map<std::string, std::pair<bool, bool>> labels;       // a Java label -> its uses (break, continue)
    std::map<const ir::Node*, py::str> loop_labels;                // id(loop) -> its label
    py::set<py::str> locals;
    std::vector<py::set<py::str>> block_names;                 // the locals each enclosing block declares (all of the block)
    std::vector<py::dict<py::str, py::str>> scopes;            // a local's name -> the name it is written with

    Writer(Program& prog, const py::ref<ir::Unit>& unit);

    // ---- locals: Java's scope of a local begins at its declaration, C#'s is its whole block, so a name an
    // inner block declares before an outer one declares it is given a number
    py::str declare(const py::str& name, bool nested = false);
    py::str lookup(const py::str& name);
    void push();
    void pop();
    void note(const py::str& kind, const py::str& why);

    // ---- types
    py::str ctype(const TypeP& t);
    py::str class_name(const TypeP& t);
    // A type's name as written in this unit: relative to its namespace, with a using for it.
    py::str short_(const py::str& qname);
    py::str namespace_of(const ClassP& c);

    // ---- declarations
    py::str unit();
    py::list<py::str> class_decl(const ClassP& c, const py::str& ind);
    py::list<py::str> class_mods(const ClassP& c);
    py::list<py::str> member(const NodeP& m, const ClassP& c, const py::str& ind);
    py::list<py::str> field(const py::ref<ir::FieldDecl>& f, const ClassP& c, const py::str& ind);
    py::list<py::str> prop(const py::ref<ir::Property>& p, const ClassP& c, const py::str& ind);
    py::str params_text(const ir::Nodes& params, bool varargs = false);
    py::list<py::str> method_decl(const MethodP& m, const ClassP& c, const py::str& ind);
    py::str ret_expr(const NodeP& e);
    py::list<py::str> method_body(const MethodP& m, const py::str& ind);
    py::list<py::str> body_lines(const NodeP& body, const py::str& ind, const MethodP& m);
    py::list<py::str> gap(const std::optional<py::str>& head, const Decline& d, const py::str& ind);

    // ---- statements
    void block_into(const NodeP& b, py::list<py::str>& out, const py::str& ind);
    void sub(const ir::NodeOrList& s, py::list<py::str>& out, const py::str& ind);
    void stmt(const NodeP& s, py::list<py::str>& out, const py::str& ind);
    py::str cond(const NodeP& e);
    py::str case_label(const NodeP& lab, const NodeP& scrutinee);

    // ---- expressions: each gives (text, precedence)
    Ex expr(const NodeP& e, bool statement = false);
    py::str wrap(const NodeP& e, int64_t prec, bool right = false);
    // e as a value of type t: a char from an int, as Java converts a constant implicitly.
    py::str coerce(const NodeP& e, const TypeP& t);
    py::str member_name(const SymP& s);
    bool shadowed(const py::str& name);
    py::str args_text(const ir::Nodes& args, const SymP& s);

  // (the rest are the Python's helper methods)
    py::list<py::str> class_decl_(const ClassP& c, const py::str& ind);
    bool lib_interface(const TypeP& t);
    py::str tparams_text(const py::list<py::str>& names, const ir::Nodes& tparams);
    py::str where_text(const NodeP& tp);
    void loop_body(const NodeP& loop, const ir::NodeOrList& body, py::list<py::str>& out, const py::str& ind);
    void for_(const py::ref<ir::For>& s, py::list<py::str>& out, const py::str& ind);
    std::pair<bool, bool> label_uses(const ir::NodeOrList& body, const py::str& label);
    void catch_(const py::ref<ir::Catch>& c, const TypeP& t, py::list<py::str>& out, const py::str& ind);
    void using_(const py::list<NodeP>& res, const NodeP& body, py::list<py::str>& out, const py::str& ind);
    void s_Switch(const py::ref<ir::Switch>& s, py::list<py::str>& out, const py::str& ind);
    void s_Try(const py::ref<ir::Try>& s, py::list<py::str>& out, const py::str& ind);
    void s_Local(const py::ref<ir::Local>& s, py::list<py::str>& out, const py::str& ind);
    Ex dispatch_(const NodeP& e);
    py::str interpolate(const py::list<NodeP>& parts);
    std::optional<Ex> null_conditional(const py::ref<ir::Cond>& e);
};

}  // namespace codebreaker::translate::backends::csharp
