// codebreaker/translate/backends/c_java.hpp -- typed C++ port of codebreaker.translate.backends.c_java.
//
// c_java.py -- C out for Java programs (docs/TRANSLATOR_DESIGN.md, phase 6), beside backends/c.py (which writes
// the decompiler's C, and is left as it is).
//
// A whole Java program from L2 as one C11 translation unit on the runtime codebreaker/translate/runtime/c
// (jrt.h, jrt.c), in the faithful tier of the design's table:
//
//   * a class is a struct whose first member is its superclass's struct (`B super_;`), so a pointer to it is a
//     pointer to each base; its fields follow;
//   * its virtual methods are a function table (`struct X_vt`): Object's three slots, then every slot of its
//     program bases in the same order, then its own -- so a table is a prefix of each subclass's; a call goes
//     through a small inline helper (X_m_v) that reads the table once;
//   * an interface method is found through the class's interface tables (itabs), so a class implements any
//     number of them; lambdas and method references are closure records -- a struct holding the captured
//     values (Java captures only effectively-final locals: copies) -- implementing the functional interface;
//   * a constructor is X_init_N(self, ...), run after allocation as the JVM runs it (fields zero first, so a
//     virtual call in a constructor reaches the override); X_new_N allocates and initialises;
//   * static state is globals; a class with a static initialiser runs it on first use (X_clinit);
//   * exceptions are setjmp/longjmp through a handler chain (jrt_try); a return or jump out of a try pops it;
//     locals of a function with a try are volatile (their values survive the longjmp);
//   * generics are erased; a value read through a type variable is cast (checked) to what javac said;
//   * Java's integer semantics: -fwrapv, masked shift counts, `/` and `%` by zero throw, (int) of a double
//     saturates; Java's left-to-right operand order where C's is unspecified (cpp03's rule);
//   * memory: objects are never freed while the program runs (jrt.h says why, and where a collector would go).
//
// Modern constructs C has no word for are lowered first (translate/desugar.py: for-each, switch expressions,
// switches on strings, instanceof patterns). What it cannot write raises Decline; the judge
// (translate/javajudge.py) reports that program as refused.
//
// Typing notes:
//   * The IR is codebreaker/translate/ir.hpp, the object model codebreaker/translate/backends/jmodel.hpp,
//     Decline is cpp03's. `units` is a list of the Unit nodes the front ends make (a NodeP list, as
//     desugar::run takes it).
//   * An (code, C type) pair is std::tuple<py::str, py::str> (Code).
//   * The Python's lists-as-records are structs: a virtual slot [name, params, result, implementation] is a
//     Slot, an interface method (slot name, C params, C result, Java name, erased Java params) an IMethod, a
//     lambda's capture (name, code, C type) a Cap, a SAM triple a SamInfo.
//   * Program.slots() caches its answer on the ClassInfo in the Python (`ci._slots`); the C++ keeps the cache
//     in the Program, keyed by the ClassInfo.
//   * The Python's `RUNTIME` is os.path.join(<codebreaker/translate>, 'runtime', 'c'); C++ has no __file__,
//     so the module holds the folder of c_java.py as a variable (set_here, as java/_paths does) and RUNTIME()
//     joins it.
//   * Method.lam (None, or {name: (code, C type)} plus '#this') is a std::optional<py::dict<str, Code>>.
//   * Python ints are int64_t.
#pragma once

#include <cstdint>
#include <map>
#include <optional>
#include <tuple>

#include "codebreaker/translate/backends/cpp03.hpp"
#include "codebreaker/translate/backends/jmodel.hpp"
#include "codebreaker/translate/ir.hpp"
#include "py.hpp"

namespace codebreaker::translate::backends::c_java {

namespace ir = codebreaker::translate::ir;
namespace jmodel = codebreaker::translate::backends::jmodel;

using cpp03::Decline;
using ClassInfoP = py::ref<jmodel::ClassInfo>;
using Code = std::tuple<py::str, py::str>;   // (C code, C type)

// the folder of c_java.py (default: codebreaker/translate/backends, relative), and the runtime's folder
void set_here(const py::str& here);
py::str RUNTIME();
extern const py::str CLANG;
extern const py::list<py::str> FLAGS;

// one virtual slot of a class's table: [name, C params after self, C result, implementing function]
struct Slot {
    py::str name;
    py::list<py::str> ps;
    py::str r;
    py::str impl;
};

// one method of an interface: (slot name, C params, C result, java name, erased Java params)
struct IMethod {
    py::str name;
    py::list<py::str> ps;
    py::str r;
    py::str java;
    py::list<ir::TypeP> eps;
};

// the functional interface a lambda implements: (qualified name, slot name, C params, C result, its methods)
struct SamInfo {
    py::str q;
    py::str slot;
    py::list<py::str> ps;
    py::str r;
    std::vector<IMethod> ms;
};

// what a lambda reads from outside: (name, code, C type)
struct Cap {
    py::str name;
    py::str code;
    py::str ct;
};

class Method;

class Program : public py::object {
  public:
    py::list<ir::NodeP> units;
    py::ref<jmodel::Model> model;
    std::optional<py::str> main;
    py::dict<py::str, py::str> lits;     // literal text -> slot name
    py::list<py::str> lambdas;           // closure records: lines of C
    int64_t nlambda = 0;
    py::list<py::str> vfuncs;            // the virtual-call helpers, after the tables
    py::set<py::str> overloaded;
    std::map<const void*, py::list<Slot>> slots_cache;   // ci._slots

    explicit Program(const py::list<ir::NodeP>& units, const std::optional<py::str>& main = std::nullopt);

    // ---- names and types
    py::str cname(const py::str& qname);
    // A method's name in C: its own, or with its parameter codes when the program overloads it.
    py::str mname(const py::str& name, const py::list<ir::TypeP>& params);
    py::str ct(const ir::TypeP& t);
    // &X_class of a class or interface, program or runtime.
    py::str class_ref(const py::str& qname);
    std::tuple<py::list<py::str>, py::str, py::list<ir::TypeP>> sig(const ClassInfoP& ci, const py::ref<ir::MethodDecl>& m);
    std::optional<py::str> base_struct(const ClassInfoP& ci);
    py::str base_class_ref(const ClassInfoP& ci);
    bool is_throwable(const ClassInfoP& ci);
    py::str lit(const py::str& v);
    bool needs_clinit(const ClassInfoP& ci);

    // ---- the table of virtual slots
    // [(slot name, C params after self, C result, implementing function)] of class ci, base slots first.
    py::list<Slot> slots(const ClassInfoP& ci);
    // The implementations of toString, equals, hashCode for class ci.
    py::dict<py::str, py::str> object_slots(const ClassInfoP& ci);
    // [(slot name, C params, C result, java name, erased params)] of an interface, its super-interfaces' first.
    std::vector<IMethod> iface_methods(const py::str& qname);
    py::list<py::str> all_ifaces(const ClassInfoP& ci);
    py::str vt_name(const py::str& qname);
    // The function implementing an interface method in class ci (its own or a base's), or None.
    // (`eps` is only counted in the Python, so the count is what is passed.)
    std::optional<py::str> find_impl(const ClassInfoP& ci, const py::str& java_name, int64_t n_eps);

    // ---- the program
    py::list<ClassInfoP> ordered();
    py::str write();
    py::list<py::str> struct_(const ClassInfoP& ci);
    py::str ctor_name(const ClassInfoP& ci, const py::ref<ir::MethodDecl>& m);
    py::list<py::str> ctor_params(const ClassInfoP& ci, const py::ref<ir::MethodDecl>& m);
    py::list<py::str> new_params(const ClassInfoP& ci, const py::ref<ir::MethodDecl>& m);
    py::list<py::str> prototypes(const ClassInfoP& ci);
    py::list<py::str> definitions(const ClassInfoP& ci);
    py::list<py::str> tables(const ClassInfoP& ci);
};

// One function's C text.
class Method : public py::object {
  public:
    Program* p;
    ClassInfoP ci;
    py::ref<ir::MethodDecl> m;
    py::dict<py::str, py::str> vars;
    py::str ret = py::str("void");
    py::list<py::str> pending;
    int64_t temps = 0;
    bool order = false;
    py::dict<int64_t, Code> subst;
    std::optional<py::str> cont_next;
    py::list<std::tuple<py::str, int64_t>> tries;   // the handlers open around the statement: (name, loops)
    int64_t loops = 0;
    bool vol = false;                               // locals volatile (the function has a try)
    py::dict<py::str, py::str> captures;
    bool static_ = false;
    std::optional<py::dict<py::str, Code>> lam;     // in a closure: {name: field} of what it captured, and 'this'
    py::str self_x = py::str("self");

    Method(Program* prog, const ClassInfoP& ci, const py::ref<ir::MethodDecl>& m,
           const std::optional<py::dict<py::str, Code>>& lam = std::nullopt);

    // ---- members
    py::list<py::str> define();
    py::list<py::str> ctor_call(const py::ref<ir::CtorCall>& call, const py::str& ind);
    // X_new for a class with no constructor of its own (an anonymous class): the super's constructor with
    // the arguments, then the field initialisers.
    py::list<py::str> anon_new();
    py::list<py::str> finit();
    py::list<py::str> clinit();
    py::str constant(const py::ref<ir::FieldDecl>& f);

    // ---- statements
    void stmts(const py::list<ir::NodeP>& ss, const py::list<py::str>& out, const py::str& ind);
    void block(const ir::NodeP& s, const py::list<py::str>& out, const py::str& ind);
    void loop_body(const ir::NodeP& body, const py::list<py::str>& out, const py::str& ind);
    void stmt(const ir::NodeP& s, const py::list<py::str>& out, const py::str& ind);
    py::str cond(const ir::NodeP& e);
    py::str decl(const py::str& ct, const py::str& name, const std::optional<py::str>& init = std::nullopt);
    // jrt_pop for each handler opened since `depth` (innermost first).
    py::str pops(int64_t depth);
    void _stmt(const ir::NodeP& s, const py::list<py::str>& out, const py::str& ind);
    py::str case_label(const ir::NodeP& lab, const ClassInfoP& enum_);
    void switch_(const py::ref<ir::Switch>& s, const py::list<py::str>& out, const py::str& ind);
    void try_(const py::ref<ir::Try>& s, const py::list<py::str>& out, const py::str& ind);
    void try_catch(const py::ref<ir::Try>& s, const py::list<py::str>& out, const py::str& ind);

    // ---- expressions -> (code, C type)
    Code expr(const ir::NodeP& e);
    py::str sequenced(const ir::NodeP& e);
    bool is_sub(const py::str& src, const py::str& dst);
    py::str conv(const py::str& code, const py::str& src, const py::str& dst);
    Code this_ref();
    Code outer_of(const py::str& qname);
    Code receiver(const ir::NodeP& obj, const ir::SymP& sym);
    Code unboxed(const py::str& code, const py::str& ct, const ir::TypeP& t);
    py::str as_string(const ir::NodeP& x);
    Code x_Paren(const py::ref<ir::Paren>& e);
    Code x_Lit(const py::ref<ir::Lit>& e);
    Code x_Name(const py::ref<ir::Name>& e);
    Code x_This(const py::ref<ir::This>& e);
    Code x_Select(const py::ref<ir::Select>& e);
    Code x_Length(const py::ref<ir::Length>& e);
    Code x_Index(const py::ref<ir::Index>& e);
    Code x_Unary(const py::ref<ir::Unary>& e);
    Code x_Binary(const py::ref<ir::Binary>& e);
    Code x_Concat(const py::ref<ir::Concat>& e);
    Code x_Assign(const py::ref<ir::Assign>& e);
    Code x_Cond(const py::ref<ir::Cond>& e);
    Code x_Cast(const py::ref<ir::Cast>& e);
    Code x_InstanceOf(const py::ref<ir::InstanceOf>& e);
    py::str args(const ir::Nodes& args, const py::list<py::str>& params, const ir::SymP& sym, bool lib = false);
    Code lib_call(const py::ref<ir::Call>& e);
    Code x_Call(const py::ref<ir::Call>& e);
    // The topmost class whose table first has the slot (its _v helper is the one written).
    ClassInfoP slot_owner(const ClassInfoP& ci, const py::str& name);
    Code x_New(const py::ref<ir::New>& e);
    Code new_class(const ClassInfoP& ci, const py::ref<ir::New>& e);
    Code x_NewArray(const py::ref<ir::NewArray>& e);
    Code x_SwitchExpr(const py::ref<ir::SwitchExpr>& e);
    Code x_ClassLit(const py::ref<ir::ClassLit>& e);
    Code x_TypeName(const py::ref<ir::TypeName>& e);

    // ---- closures
    Code closure(const ir::NodeP& e);
    SamInfo sam(const ir::TypeP& t);
    Code x_Lambda(const py::ref<ir::Lambda>& e);
    // A method reference as the lambda it stands for, written as one.
    Code x_MethodRef(const py::ref<ir::MethodRef>& e);
    static py::list<ir::NodeP> ref_args(const py::list<ir::NodeP>& names, const py::list<ir::TypeP>& ptypes);
};

py::str program(const py::list<ir::NodeP>& units, const std::optional<py::str>& main = std::nullopt);

// Write the program as C and compile it with clang (else the host's gcc) against the runtime.
// -> (exe or None, why)
std::tuple<std::optional<py::str>, py::str> build(const py::list<ir::NodeP>& units, const py::str& out);

}  // namespace codebreaker::translate::backends::c_java
