// codebreaker/translate/backends/java_objects.hpp -- typed C++ port of codebreaker.translate.backends.java_objects.
//
// java_objects.py -- the Java back end's idiomatic tier: C structs and C++ classes as Java objects.
//
// Chosen when the pointer analysis (java.Analysis) finds every pointer disciplined: no pointer
// arithmetic, no casts between pointer types, no unions, no raw memory functions, no address of an
// element or a field. Then:
//
//   * a struct or class is a Java class (a nested static class of the program), its fields Java fields;
//     a C++ class keeps its methods, constructors (base initialiser as super(...)), single inheritance,
//     virtual and abstract methods, static members; operators become named methods (operator+ -> plus);
//   * a pointer to a struct is a reference (NULL is null), `p->f` is `p.f`;
//   * an array is a Java array, and so is a pointer to a scalar: it can only index (`*p` is `p[0]`);
//   * a scalar whose address is taken is boxed in a one-element array (`int[] n = {5}`), so `&n` is `n`
//     and `*p = 3` is `p[0] = 3` -- C's out-parameters and C++'s `int &` work as they did;
//   * struct values keep C's copy semantics: assignment copies into the object (`a.set(b)`), passing or
//     initialising by value copies (`new Point(p)`, the copy constructor), aggregates are constructors;
//   * malloc(sizeof(T)) is `new T()`, malloc(n * sizeof(T)) `new T[n]`; free is the collector's;
//   * function pointers are functional interfaces (`IntIntToInt cmp = Prog::compare;`).
// Arithmetic, conversions, statements and C's library are the faithful tier's (java.FnWriter).
//
// Typing notes: Objects is Faithful's subclass and ObjWriter FnWriter's (java.hpp), overriding the same
// methods the Python overrides. The Python's `default` method is `default_` (C++ keyword). Python's
// __init__ chain calls overridable methods, so a program is made with make_objects() (see java.hpp).
#pragma once

#include "codebreaker/translate/backends/java.hpp"

namespace codebreaker::translate::backends::java_objects {

namespace jb = codebreaker::translate::backends::java;
namespace ir = codebreaker::translate::ir;

using jb::ClassP;
using jb::FieldP;
using jb::J;
using jb::Lines;
using jb::MethodP;
using jb::NodeP;
using jb::Place;
using jb::SymP;
using jb::TypeP;
using jb::Var;

// operator+ -> plus, ...; Refused for an operator the table does not name
py::str method_name(const py::str& name);

// The idiomatic tier's program writer.
class Objects : public jb::Faithful {
  public:
    py::set<py::str> ctors_used;               // qnames whose aggregate constructor is used
    py::set<py::str> copied;                   // qnames whose copy constructor is used
    py::set<py::str> assigned;                 // qnames whose set() is used
    bool arrays_filled = false;
    py::dict<py::str, py::str> cls_names;
    py::set<py::str> boxed;

    using jb::Faithful::Faithful;

    void _check_records() override;
    void _names() override;

    // ---- types
    py::str cls(const TypeP& t);
    // sizeof(t) where the source's layout gives it (C structs, scalars), else None.
    std::optional<int64_t> size_hint(const TypeP& t);
    bool is_enum(const TypeP& t);
    py::str jt(const TypeP& t) override;
    jb::Sig fn_sig(const SymP& s) override;
    jb::Sig fn_sig_of_type(const TypeP& ft) override;
    py::str iface(const jb::Sig& sig) override;
    // A fresh value of type t: what C's zero-initialised storage holds, as Java.
    py::str default_(const TypeP& t);

    // ---- the file
    py::str write() override;
    std::optional<py::str> gap_signature(const MethodP& f, const std::optional<py::str>& owner) override;
    Lines entry() override;

    // ---- globals
    void declare_global(const FieldP& g) override;
    void init_global(const FieldP& g) override;

    // ---- classes
    Lines class_decl(const ClassP& c);
    Lines copy_fields(const py::list<FieldP>& fields, const py::str& dst, const py::str& src, bool fresh);
};

std::shared_ptr<Objects> make_objects(const py::list<NodeP>& units, const py::str& name, const std::shared_ptr<jb::Analysis>& a);

// A function, method or constructor in the idiomatic tier.
class ObjWriter : public jb::FnWriter {
  public:
    Objects* po;
    ClassP cls;

    ObjWriter(Objects* prog, const MethodP& fn, const ClassP& cls = nullptr);

    // ---- functions
    Lines write() override;
    py::str default_of(const TypeP& t);
    std::pair<py::list<py::str>, Lines> params(const MethodP& f);
    Lines write_member();
    // `this->f = 0` (or null, false) for a field with no initialiser of its own.
    bool zero_init(const NodeP& s, const ClassP& c);

    // ---- statements
    void local(const py::ref<ir::Local>& s, Lines& out) override;
    // An initialiser of type t as a Java expression (a fresh object or array where t is one).
    J init_value(const TypeP& t, const NodeP& init);
    J aggregate(const TypeP& t, const py::list<NodeP>& args);
    J copy_of(const J& v, const TypeP& t);
    void static_local(const py::ref<ir::Local>& s, const py::str& name) override;
    void ret(const py::ref<ir::Return>& s, Lines& out) override;
    void expr_stmt(const NodeP& e, Lines& out) override;
    bool has_destroy(const TypeP& t);

    // ---- places
    jb::Var var(const SymP& s) override;
    Place place(const NodeP& e) override;
    py::str field(const py::ref<ir::Select>& e);
    J load(const Place& pl) override;
    J store(const Place& pl, const J& val) override;
    J addr(const NodeP& e) override;

    // ---- values
    J truth(const J& j, const TypeP& t) override;
    J x_Concat(const py::ref<ir::Concat>& e) override;
    J x_Length(const py::ref<ir::Length>& e) override;
    J x_Lit(const py::ref<ir::Lit>& e) override;
    J x_Name(const py::ref<ir::Name>& e) override;
    J x_Select(const py::ref<ir::Select>& e) override;
    J x_This(const py::ref<ir::This>& e) override;
    J x_AddressOf(const py::ref<ir::AddressOf>& e) override;
    J x_Unary(const py::ref<ir::Unary>& e) override;
    J incdec(const py::ref<ir::Unary>& e, bool as_statement = false) override;
    J compare(const py::ref<ir::Binary>& e, const py::str& op) override;
    J x_Assign(const py::ref<ir::Assign>& e) override;
    J x_Cast(const py::ref<ir::Cast>& e) override;
    J conv(const J& j, const TypeP& s, const TypeP& t) override;
    // malloc/calloc(...) converted to T*: a new object or array of T.
    J allocation(const py::ref<ir::Call>& call, const TypeP& t);
    bool is_size(const NodeP& e, const TypeP& el);
    // The element count an allocation size expresses ('n * sizeof(T)', 'sizeof(T)', a byte count of chars),
    // as Java int text; or None.
    std::optional<py::str> count(const NodeP& e, const TypeP& el);
    J x_Call(const py::ref<ir::Call>& e) override;
    py::list<py::str> call_args(const py::list<NodeP>& args, const SymP& sym, const std::optional<std::vector<TypeP>>& params = std::nullopt);
    J method_call(const py::ref<ir::Call>& e);
    MethodP find_method(const SymP& s);
    J x_New(const py::ref<ir::New>& e) override;
    J construct(const TypeP& t, const py::ref<ir::New>& e);
    J x_NewArray(const py::ref<ir::NewArray>& e) override;
    J x_Cond(const py::ref<ir::Cond>& e) override;
    J lib_call(const py::ref<ir::Call>& e, const py::str& name, const py::list<NodeP>& args) override;
};

}  // namespace codebreaker::translate::backends::java_objects
