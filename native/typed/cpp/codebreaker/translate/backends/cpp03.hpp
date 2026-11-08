// codebreaker/translate/backends/cpp03.hpp -- typed C++ port of codebreaker.translate.backends.cpp03.
//
// cpp03.py -- the translator's C++03 back end: method bodies, constructors and initialisers from the IR.
//
// The transpiler (codebreaker/java) writes the declarations: headers, signatures, fields. This back end
// writes every body from the typed tree, in the transpiler's conventions -- the same TypeResolver decides
// pointer or value, `s_` names statics, arrays are JArray<T>, Strings JavaString (a char per byte),
// concatenation JCat -- so a body fits the declarations it is written into.
//
// It declines rather than guess: anything it does not handle raises Decline, and the transpiler writes
// an explicit, ledgered gap there (J2X-GAP: the body throws UnsupportedOperationException). Whether what
// it writes is right is decided by the judge, class by class (probe.py).
//
// The second half of the module is the C++17 dialect (Writer17, Bodies17): the same Writer with the C++17
// forms where the language has them.
//
// Typing notes (how the Python's values are typed here):
//   * An expression written is the Python's (code, C++ type) tuple: Code = std::tuple<py::str, py::str>.
//   * The Python makes node-kind dispatch by name (`getattr(self, 'x_' + type(e).__name__)`); here Writer::expr
//     tests the node's class (dynamic_cast) and calls the x_<Kind> member. Writer17 adds x_SwitchExpr and
//     overrides _stmt / x_Binary: those are virtual.
//   * `Writer = Writer17` (the module attribute swapped while a Bodies17 call runs, the `_with_writer17` wrapper) is
//     a flag: new_writer() makes a Writer17 while a Writer17Scope is alive.
//   * The Python's Bodies class is the interface the transpiler's source emitter calls on it
//     (codebreaker::java::emit::source::Bodies); Bodies derives from it, so the emitter takes either back end.
//     The constructor's result is the emitter's CtorParts (kind, args, body lines). `declined` and `reasons`
//     are the Python's Counters: py::dict<str, int>.
//   * A class's `.name`, a Sym's `.owner` and such, which are None until set, are std::optional; where the Python
//     formats one with %s a None reads 'None' (py::percent does that for an optional), and a dict lookup by an
//     unset name finds nothing.
//   * The IR slots a statement body holds as a bare list of statements (ir::NodeOrList) are not something the Java
//     front end makes; where the Python fails on one (Decline('statement list')), so does this.
//   * java_units() of the Python runs javac (frontends.java.dump) and lowers each unit's JSON; the part with a typed
//     port is the lowering, so java_units takes the dumps ({path: tree}) the Python's dump returns.
//   * The runtime reads (_runtime_header, _runtime_return, _runtime_field, library_return) use
//     codebreaker::java::runtime as the Python does.
//   * Python ints are int64_t.
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <tuple>
#include <vector>

#include "codebreaker/java/emit/source.hpp"
#include "codebreaker/java/ir.hpp"
#include "codebreaker/java/ledger.hpp"
#include "codebreaker/java/types.hpp"
#include "codebreaker/translate/ir.hpp"
#include "py.hpp"

namespace codebreaker::translate::backends::cpp03 {

namespace ir = codebreaker::translate::ir;
namespace jir = codebreaker::java::ir;
namespace jtypes = codebreaker::java::types;
namespace jsource = codebreaker::java::emit::source;
using JTypeRef = py::ref<jir::JType>;
using ClassRef = py::ref<jir::JClass>;
using MethodRef = py::ref<jir::Method>;
using Code = std::tuple<py::str, py::str>;      // (code, C++ type)

// This back end does not write the method; the transpiler's text path does.
struct Decline : py::Exception {
    explicit Decline(const py::str& m) : py::Exception(m) {}
    explicit Decline(const char* m) : py::Exception(m) {}
    const char* type() const override { return "Decline"; }
};

// An IR type as the transpiler's JType (simple names, as its resolver keys on them).
JTypeRef jtype(const ir::TypeP& t);

// 'T a, const JavaString& b' -> [('T', 'a'), ('const JavaString&', 'b')].
py::list<std::tuple<py::str, py::str>> _split_params(const py::str& params);

bool is_pointer(const py::str& ctype);

// 'JArray<Block* >' -> 'Block*'.
py::str _elem(const py::str& ctype);

// What the bodies of one conversion share: the transpiler's resolver and parsed classes.
class Context : public py::object {
  public:
    py::ref<jtypes::TypeResolver> resolver;
    py::dict<py::str, ClassRef> classes;
    py::set<py::str> qnames;                 // the program's classes by qualified name (Bodies)
    py::ref<codebreaker::java::ledger::Ledger> ledger;   // the conversion's ledger, when it keeps one (null: None)
    py::dict<int64_t, int64_t> char_map;     // char code -> the 8-bit code the program draws (project)

    Context(const py::ref<jtypes::TypeResolver>& resolver, const py::list<ClassRef>& classes);

    // A class of the program (not the library). By qualified name: the game's own `Container` is not
    // java.awt.Container.
    bool is_game(const std::optional<py::str>& qualified) const;
    // The transpiler's parsed constructor of `simple` taking these IR parameter types (null: none).
    MethodRef old_ctor(const py::str& simple, const py::list<ir::TypeP>& params) const;
    ClassRef old_class(const py::str& simple) const;
    // The transpiler's parsed method `owner.name(params)` (IR param types), searching up the parents.
    MethodRef old_method(const py::str& owner, const py::str& name, const py::list<ir::TypeP>& params) const;
    std::optional<py::str> field_ctype(const py::str& owner, const py::str& name) const;
};

// One method's body. `m` is the IR MethodDecl, `old` the transpiler's parsed method (for the
// parameter types its header declared).
class Writer {
  public:
    py::ref<Context> ctx;
    py::ref<jtypes::TypeResolver> r;
    py::ref<ir::ClassDecl> cls;
    py::str cname;
    py::ref<ir::MethodDecl> m;
    py::dict<py::str, py::str> vars;             // name -> C++ type
    std::optional<py::str> parent;
    py::str ret;
    bool two_phase = false;                      // writing a j2xInit: `return;` is `return this;`
    MethodRef old;
    int64_t npe = 0;                             // inside a try whose catch a NullPointerException reaches
    std::optional<py::str> cont_next;            // the label whose loop body is being written
    py::dict<py::str, py::str> alias;            // a name written as something else (inlined ctors)
    int64_t loops = 0;                           // loops and switches around the statement
    py::list<std::tuple<int64_t, int64_t>> fin;  // enclosing finally-with-jumps: (n, loops)
    int64_t fin_count = 0;
    int64_t leaks = 0;                           // _j2xleakN stack temporaries written
    py::list<ir::NodeP> after;                   // the statements after the one being written
    py::dict<int64_t, Code> subst;               // id(expression) -> (code, C++ type) already written
    py::list<py::str> pending;                   // declarations the current statement needs first
    bool order = false;                          // in a statement: keep Java's operand order
    int64_t temps = 0;
    int64_t arg_temps = 0;                       // _j2xarg_N owned temporaries hoisted
    bool owning = false;
    std::optional<py::set<py::str>> _owned_scalar;

    Writer(const py::ref<Context>& ctx, const py::ref<ir::ClassDecl>& cls, const py::ref<ir::MethodDecl>& m,
           const MethodRef& old);
    virtual ~Writer() = default;

    // ---- types
    py::str ctype(const ir::TypeP& t, const py::str& context = py::str("local"));

    // ---- statements
    py::list<py::str> body();
    void stmts(const py::list<ir::NodeP>& ss, const py::list<py::str>& out, const py::str& ind);
    void block(const ir::NodeP& s, const py::list<py::str>& out, const py::str& ind);
    void block(const ir::NodeOrList& s, const py::list<py::str>& out, const py::str& ind);
    // A loop's body; when the loop is labelled and continued by label, the body sits in its own block
    // with `label_cont:` after it (so the goto crosses no initialisation).
    void loop_body(const ir::NodeOrList& body, const py::list<py::str>& out, const py::str& ind);
    void loop_body(const ir::NodeP& body, const py::list<py::str>& out, const py::str& ind);
    void stmt(const ir::NodeP& s, const py::list<py::str>& out, const py::str& ind);
    void stmt(const ir::NodeOrList& s, const py::list<py::str>& out, const py::str& ind);
    virtual void _stmt(const ir::NodeP& s, const py::list<py::str>& out, const py::str& ind);
    // A finally that a return, break or continue leaves through: the jump records what it was (and
    // the value) and goes to the finally; after it, the recorded jump is made.
    void finally_with_jumps(const py::ref<ir::Try>& s, const py::list<py::str>& out, const py::str& ind);
    // A try and its catches (no finally). Where a catch is one a NullPointerException reaches, every
    // dereference in the try is null-checked to throw Java's exception (j2xNN), so C++ catches it where
    // Java would, instead of crashing.
    void try_catch(const py::ref<ir::Try>& s, const py::list<py::str>& out, const py::str& ind);
    void note(const py::str& category, const py::str& severity, const py::str& rule, const py::str& text);
    // Two call statements written as the console needs them, or None (see the Python).
    std::optional<py::str> kept_call(const ir::NodeP& e);

    // ---- no-GC ownership (Java's collector, by hand)
    const py::set<py::str>& owned_scalar();
    void free_owned_reassign(const ir::NodeP& e, const py::list<py::str>& out, const py::str& ind);
    void free_owned_return(const py::ref<ir::Local>& s, const py::str& ct, const py::str& name,
                           const py::list<py::str>& out, const py::str& ind);
    void hoist_owned_args(const ir::NodeP& root, const py::list<py::str>& out, const py::str& ind);
    // Reflection has no C++ equivalent -- the runtime's stubs return NULL -- so a statement using it is
    // marked for a hand-written factory (J2X-REVIEW, and a HIGH ledger entry).
    void review(const ir::NodeP& e, const py::list<py::str>& out, const py::str& ind);
    // A condition, without the parentheses Java writes around it (`if(x)`, as the text path: the console
    // passes match that shape).
    py::str cond(const ir::NodeP& e);

    // ---- expressions -> (code, C++ type)
    py::str name(const py::str& n) const;
    Code expr(const ir::NodeP& e);
    Code expr_nl(const ir::NodeOrList& e);
    // Java evaluates operands left to right; C++ leaves the order of a call's arguments, of a binary
    // operator's operands and of an assignment's sides unspecified (g++ takes arguments right to left). When
    // the order can change the result, each operand is evaluated into a temporary first, in Java's order,
    // with the comma operator. Returns that sequence, or ''.
    py::str sequenced(const ir::NodeP& e);
    Code x_Paren(const py::ref<ir::Paren>& e);
    Code x_Lit(const py::ref<ir::Lit>& e);
    Code x_Name(const py::ref<ir::Name>& e);
    Code x_This(const py::ref<ir::This>& e);
    Code x_Select(const py::ref<ir::Select>& e);
    // An argument. A concatenation goes as a JavaString: as a JCat it converts to JavaString and to
    // JObject, and a library constructor taking either is ambiguous (new File(dir + "/x")).
    py::str arg(const ir::NodeP& a);
    // The arguments without the decompiler's EmptyN marker ones, which the transpiler strips from
    // declarations and calls alike.
    py::list<ir::NodeP> unmarked(const ir::Nodes& args);
    py::str checked(const py::str& recv, const py::str& rt);
    Code x_Length(const py::ref<ir::Length>& e);
    Code x_Index(const py::ref<ir::Index>& e);
    Code x_Unary(const py::ref<ir::Unary>& e);
    virtual Code x_Binary(const py::ref<ir::Binary>& e);
    Code x_Concat(const py::ref<ir::Concat>& e);
    Code x_Assign(const py::ref<ir::Assign>& e);
    Code x_Cond(const py::ref<ir::Cond>& e);
    py::str arm(const py::str& code, const py::str& ctype, const py::str& t);
    Code x_Cast(const py::ref<ir::Cast>& e);
    Code x_InstanceOf(const py::ref<ir::InstanceOf>& e);
    Code boxed_instanceof(const py::str& x, const py::str& xt, const py::str& name);
    Code x_Call(const py::ref<ir::Call>& e);
    // java.lang.Object's own methods, as the transpiler lowers them.
    Code object_method(const py::ref<ir::Call>& e);
    // A varargs call's arguments: the trailing ones packed into the array the declaration takes.
    py::str varargs(const py::ref<ir::Call>& e, const py::str& owner);
    // A library varargs call (String.format, Arrays.asList, Constructor.newInstance): the trailing
    // arguments packed into the array type the Java declaration takes, as the text path writes it.
    py::str library_varargs(const py::ref<ir::Call>& e);
    py::str library_field(const py::str& owner, const py::str& name, const ir::TypeP& t);
    // The C++ return type of a runtime (library) method: what the runtime's own stub registry says,
    // else the resolver's mapping of the Java type.
    py::str library_return(const py::str& owner, const py::str& name, const ir::TypeP& ret);
    Code x_ClassLit(const py::ref<ir::ClassLit>& e);
    Code x_New(const py::ref<ir::New>& e);
    Code x_NewArray(const py::ref<ir::NewArray>& e);

    // (the C++17 dialect adds the switch expression)
    virtual bool handles(const ir::NodeP& e) const;
    virtual Code dispatch(const ir::NodeP& e);
};

// ---------------------------------------------------------------------------------------------- module-level helpers
// Does evaluating `n` change anything (a call into the program, an allocation, an assignment, ++/--)?
bool _effects(const ir::NodeP& n);
// Names (locals, fields) that evaluating `n` assigns or increments.
py::set<py::str> _written(const ir::NodeP& n);
py::set<py::str> _read(const ir::NodeP& n);
bool _order_matters(const py::list<ir::NodeP>& nodes);
bool _constant(const ir::NodeP& n);
// The operands of `e` whose evaluation order Java fixes (left to right) and C++ does not.
py::list<ir::NodeP> _operands(const ir::NodeP& e);
ir::NodeP _strip_paren(ir::NodeP e);
// An initial value for a temporary of C++ type `ct` (it is assigned before it is read).
py::str _zero(const py::str& ct);
py::str _comment(const py::str& text);
// Whether statement `s` never completes normally (Java's rule, for return and throw).
bool _abrupt(const ir::NodeP& s);
// Whether any of these statement trees returns, or breaks or continues out of them.
bool _leaves(const py::list<ir::NodeP>& stmts);

// The header `owner.h` in the runtime, the return type it declares for `name`, a static field it declares.
py::str _runtime_header(const py::str& owner);
std::optional<py::str> _runtime_return(const py::str& owner, const py::str& name);
// (C++ type, spelling) of field `name` as the runtime's `owner.h` declares it -- `s_name` or `name` -- or (None, None).
std::tuple<std::optional<py::str>, std::optional<py::str>> _runtime_field(const py::str& owner, const py::str& name);

// A boxed JObject where a primitive is wanted: its explicit accessor.
py::str unbox(const py::str& code, const py::str& ctype, const py::str& target);
// Java's null as a value of C++ type `ctype`: NULL for a pointer; a value type's own null.
py::str null_of(const py::str& ctype);
// `code` (of C++ type `ctype`) where a `target` is wanted: Java's null becomes the target's null.
py::str coerce(const py::str& code, const py::str& ctype, const py::str& target);
std::optional<py::str> _class_of(const py::str& ctype);
// (v: an int, float or bool, as the front end's Lit.value holds it)
py::str _float(const ir::Val& v, const py::str& suffix);
py::str _cstring(const ir::Val& v, const py::dict<int64_t, int64_t>& char_map = py::dict<int64_t, int64_t>());

// A constructor from the IR: (super-arguments or None, body lines). Ordinary classes: super(args)
// is the base initialiser, so its arguments come back separately; this(args) is declined (C++03 has no
// delegation: the transpiler inlines the other constructor). Two-phase classes: super(args) is
// Parent::j2xInit(args), this(args) is C::j2xInit(args), the instance initialisers are the caller's.
jsource::CtorParts ctor_parts(const py::ref<Context>& ctx, const py::ref<ir::ClassDecl>& cls,
                              const py::ref<ir::MethodDecl>& m, const MethodRef& old, bool two_phase);
// A field initialiser's C++ expression, converted to the field's type.
py::str field_init(const py::ref<Context>& ctx, const py::ref<ir::ClassDecl>& cls, const py::ref<ir::FieldDecl>& f);
// An initialiser block's statements (static {} or instance {}).
py::list<py::str> block_lines(const py::ref<Context>& ctx, const py::ref<ir::ClassDecl>& cls,
                              const py::ref<ir::MethodDecl>& m);
// The C++ body lines of IR method `m`, or raise Decline.
py::list<py::str> method_body(const py::ref<Context>& ctx, const py::ref<ir::ClassDecl>& cls,
                              const py::ref<ir::MethodDecl>& m, const MethodRef& old);

// The tree's method bodies for one conversion, looked up by the transpiler as it writes each method
// (emit/source.py). Counts what it wrote and why it declined the rest.
class Bodies : public jsource::Bodies {
  public:
    py::ref<Context> ctx;
    py::dict<py::str, py::ref<ir::ClassDecl>> decls;
    int64_t written = 0;
    int64_t written_ctors = 0;
    py::dict<py::str, int64_t> declined;
    py::dict<py::str, int64_t> reasons;      // the full reason, for the work list

    Bodies(const py::ref<Context>& ctx, const py::list<ir::NodeP>& units);

    // (the class, the method) of the IR method matching the transpiler's `old` (method null: none)
    std::tuple<py::ref<ir::ClassDecl>, py::ref<ir::MethodDecl>> _find(const py::str& cls_name, const MethodRef& old);
    std::tuple<py::ref<ir::ClassDecl>, py::ref<ir::MethodDecl>> _ctor(const py::str& cls_name, const MethodRef& old);

    // (kind 'super'/'this'/None, its arguments, body lines) for the transpiler's constructor `old`, or None.
    std::optional<jsource::CtorParts> ctor(const py::str& cls_name, const MethodRef& old, bool two_phase = false) override;
    // The C++ initialiser expression of field `name`, or None.
    std::optional<py::str> field_init(const py::str& cls_name, const py::str& name) override;
    // The C++ arguments of enum constant `const`'s constructor call, or None.
    std::optional<py::list<py::str>> enum_args(const py::str& cls_name, const py::str& const_) override;
    // The C++ elements of static field `name`'s `new T[]{...}` initialiser, each as the element type, or None.
    std::optional<py::list<py::str>> array_items(const py::str& cls_name, const py::str& name) override;
    // `this->f = init;` for each instance field initialiser and the instance {} blocks, in source
    // order -- or None (the transpiler's own, then).
    std::optional<py::list<py::str>> instance_inits(const py::str& cls_name) override;
    // Each static {} block's lines, in source order, or None.
    std::optional<py::list<py::list<py::str>>> static_blocks(const py::str& cls_name) override;
    // Body lines for the transpiler's method `old` of class `cls_name`, or None (use its own).
    std::optional<py::list<py::str>> body(const py::str& cls_name, const MethodRef& old) override;
};

// Run the Java front end's dumps ({path: tree}, what frontends.java.dump returns) through the lowering.
// -> [ir.Unit]
py::list<ir::NodeP> java_units(const py::dict<py::str, ir::Dict>& trees);

// ============================================================================================ the C++17 dialect
// Inside the transpiler, `convert(..., cpp_std='C++17')` writes every body with this module's Writer and the
// C++17 forms where the language has them: a for-each over a container as a range-for, a switch expression as
// an immediately invoked lambda, `auto` where the initialiser spells the type, nullptr against pointers.
class Writer17 : public Writer {
  public:
    using Writer::Writer;

    void _stmt(const ir::NodeP& s, const py::list<py::str>& out, const py::str& ind) override;
    // A comparison with null: nullptr against a pointer. Against the runtime's value types (JavaString,
    // JList, JObject: their null is a state, compared with 0) the null stays what C++03 wrote.
    Code x_Binary(const py::ref<ir::Binary>& e) override;
    // The initialiser names the local's own type: `new T(..)` of the same T, or a cast to it.
    bool spelled(const py::ref<ir::Local>& s);
    // `auto x = new T(...)` / `auto x = (T*)y`: the type is written once, where it is made.
    void auto_local(const py::ref<ir::Local>& s, const py::list<py::str>& out, const py::str& ind);
    // for (T x : xs): a range-for over a JList (a std::vector, iterated in order as Java's Iterator
    // does); over an array, the counted loop (JArray has no iterators).
    void foreach(const py::ref<ir::ForEach>& s, const py::list<py::str>& out, const py::str& ind);
    // A switch expression as an immediately invoked lambda: each `yield v` is `return v`.
    Code x_SwitchExpr(const py::ref<ir::SwitchExpr>& e);

    bool handles(const ir::NodeP& e) const override;
    Code dispatch(const ir::NodeP& e) override;
};

// The NULLs the C++03 rules wrote where the target is one of the runtime's value types (a JavaString,
// JList or JObject compared with or made from null): spelled __null, what NULL is to GCC and clang, so they
// resolve as they did under C++03.
py::str _keep_null(const py::str& x);

// The tree's bodies for one conversion, written by Writer17. The modern forms C++17 has no direct
// word for (a switch on a String, instanceof patterns) are lowered first (translate/desugar.py).
class Bodies17 : public Bodies {
  public:
    Bodies17(const py::ref<Context>& ctx, const py::list<ir::NodeP>& units);

    std::optional<jsource::CtorParts> ctor(const py::str& cls_name, const MethodRef& old, bool two_phase = false) override;
    std::optional<py::str> field_init(const py::str& cls_name, const py::str& name) override;
    std::optional<py::list<py::str>> enum_args(const py::str& cls_name, const py::str& const_) override;
    std::optional<py::list<py::str>> array_items(const py::str& cls_name, const py::str& name) override;
    std::optional<py::list<py::str>> instance_inits(const py::str& cls_name) override;
    std::optional<py::list<py::list<py::str>>> static_blocks(const py::str& cls_name) override;
    std::optional<py::list<py::str>> body(const py::str& cls_name, const MethodRef& old) override;
};

}  // namespace codebreaker::translate::backends::cpp03
