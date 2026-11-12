// codebreaker/translate/backends/java.hpp -- typed C++ port of codebreaker.translate.backends.java.
//
// NOT YET A SHIM (W1-B26): the back end is now native/cb/translate/backends/java.{hpp,cpp} and java_fn.cpp (typed IR, no py::). This
// port stays, as it was, for one reason: codebreaker/translate/backends/java_objects.* (the idiomatic tier, the next unit) extends this
// port's Faithful and FnWriter and java_libc.* (the library mapping) calls into them, over the OLD IR; they cannot compile against the
// new module until they are refactored into a cb::translate::backends::java::TierWriter and LibraryCalls. When they are, this port
// becomes a shim over the new module (old IR tree -> L2 through cb/compat/old_l2_tree.hpp -> java::program) and this note goes.
// The Python's fixes of this unit are made here too (this port is what the exe runs): the DEL character constant, character
// literals Java spells differently, and two lowerings that evaluated a sub-expression twice.
//
// java.py -- the translator's Java back end: a C or C++ program (L2 from the clang front end) as Java 17.
//
// docs/TRANSLATOR_DESIGN.md, phase 5. The output comes in the design's two tiers, chosen per program by a
// pointer analysis (`analyse`):
//
//   * idiomatic -- when every pointer is disciplined (it only ever names a whole struct, or an array
//     it indexes; no pointer arithmetic, no casts between pointer types, no unions, no address of a
//     scalar): structs become classes, struct pointers references, arrays Java arrays, malloc `new`, free
//     nothing. C++ classes are this tier's: methods, constructors, single inheritance, virtual calls.
//   * faithful -- otherwise: C's memory is emulated, as Emscripten and CheerpJ do. Everything whose
//     bytes the program can reach (arrays, structs, any variable whose address is taken, the heap) lives
//     in one linear memory (`crt.Mem`, a little-endian byte array); a pointer is an int address into it;
//     a struct is its address, its fields at offsets named in a small class per struct (`Pair.b`), laid
//     out as the source's compiler lays them out (ClassDecl.layout), so unions, punning, pointer
//     arithmetic and sizeof all mean what they meant. Variables whose address is never taken stay
//     ordinary Java locals and fields.
//
// Both tiers share C's arithmetic on Java's types: a char is a byte, unsigned short a char, unsigned int
// an int and unsigned long long a long, with the unsigned operations (division, comparison, widening,
// `>>`) written out (`Integer.divideUnsigned`, `x & 0xFF`, `>>>`). C's library is `crt.LibC` (printf with
// C's own formatting, the string and memory functions, malloc/free on the emulated heap, MSVC's rand),
// statically imported, so calls read as they did in C.
//
// What the back end does not write it refuses, with the reason (`Refused`): a goto, a varargs function
// definition, setjmp, a library function the runtime does not have. Nothing is dropped silently.
//
//     prog = java::program(units, "Hello")       // -> Program: .source (Java text), .tier, .notes
//     java::RUNTIME()                             // the folder holding crt/*.java
//
// Typing notes (how the Python's loosely typed parts are typed here):
//   * The IR is codebreaker/translate/ir.hpp; `units` is the list of Unit nodes the front ends make (a NodeP
//     list). A body slot (NodeOrList) that holds a bare list of statements is what the Python's isinstance
//     chains fail on: stmt() refuses it ("statement list"), can_complete() takes it as completing.
//   * The Python's classes are classes here, with their methods virtual where java_objects overrides them
//     (Faithful / Objects, FnWriter / ObjWriter). Python's __init__ calls overridable methods; C++ cannot
//     dispatch from a constructor, so the constructors only set the fields and init() does the rest --
//     make_faithful() / java_objects::make_objects() run both.
//   * A (java params, java return) signature is a Sig; a variable key ('g', name) / ('l', owner, name)
//     is a str (key_g / key_l); the layout fields (a list of {name, offset, type} dicts) are LayoutFields.
//   * `_Code` -- a Java expression already written, standing in the tree for a compound assignment's
//     operand or a function-pointer call's target -- is CodeNode (an ir::Expr).
//   * The Python's `RUNTIME` is os.path.join(<codebreaker/translate/backends>, 'java_runtime'); C++ has no
//     __file__, so the module holds the folder of java.py as a variable (set_here, as c_java does).
//   * Python ints are int64_t; a literal past int64 cannot occur in an L2 document read here.
#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <tuple>
#include <vector>

#include "codebreaker/translate/ir.hpp"
#include "py.hpp"

namespace codebreaker::translate::backends::java {

namespace ir = codebreaker::translate::ir;

using ir::NodeP;
using ir::SymP;
using ir::TypeP;
using Lines = py::list<py::str>;
using ClassP = py::ref<ir::ClassDecl>;
using MethodP = py::ref<ir::MethodDecl>;
using FieldP = py::ref<ir::FieldDecl>;

// the folder of java.py (default: codebreaker/translate/backends, relative), and the runtime's folder
void set_here(const py::str& here);
py::str RUNTIME();

const py::dict<py::str, py::str>& JPRIM();
const py::dict<py::str, int64_t>& _BITS();
const py::dict<py::str, int64_t>& _JBITS();
const py::dict<py::str, py::str>& _ACCESS();
const py::set<py::str>& _KEYWORDS();

// Java operator precedence (higher binds tighter)
constexpr int64_t P_ATOM = 17, P_POST = 16, P_UNARY = 15, P_MUL = 13, P_ADD = 12, P_SHIFT = 11, P_REL = 10, P_EQ = 9,
                  P_BAND = 8, P_XOR = 7, P_BOR = 6, P_AND = 5, P_OR = 4, P_COND = 3, P_ASSIGN = 2;
int64_t _BINPREC(const py::str& op);

// The back end does not write this program (the reason says why).
struct Refused : py::Exception {
    explicit Refused(const py::str& m) : py::Exception(m) {}
    explicit Refused(const char* m) : py::Exception(m) {}
    const char* type() const override { return "Refused"; }
};

// A Java expression: its text, its precedence and its Java type ('boolean' for a C truth value).
struct J {
    py::str code;
    int64_t prec = 0;
    py::str jt;
    bool fresh = false;

    J() {}
    J(const py::str& code, int64_t prec, const py::str& jt, bool fresh = false) : code(code), prec(prec), jt(jt), fresh(fresh) {}
    py::str at(int64_t prec) const;
    py::str repr() const;
};

py::str jname(const std::optional<py::str>& name);
// A type's kind with plain char resolved by its sign.
py::str _kind(const TypeP& t);
bool is_int(const TypeP& t);
bool is_unsigned(const TypeP& t);
bool is_float(const TypeP& t);
bool is_ptr(const TypeP& t);
bool is_record(const TypeP& t);
// A type's canonical spelling -- the same the clang front end names records and template instances by
// (ClassDecl.qname): 'Box<f64>', 'geo.Vec', 'char*'.
py::str spell(const TypeP& t);
NodeP strip(const NodeP& e);
// A Java string literal for a C string held one character per byte.
py::str java_string(const py::str& s);
// A Java string literal for a wide (UTF-16) C string.
py::str java_wstring(const py::str& s);
// A C string literal (char or wide) as a Java string literal.
py::str string_lit(const py::ref<ir::Lit>& lit);
bool is_wide(const py::ref<ir::Lit>& lit);
// An integer constant as a Java literal of type jt (the value reduced to jt's bits).
J int_lit(int64_t v, const py::str& jt);
J float_lit(double v, const py::str& jt);
// C's null pointer constant: 0, NULL ((void *) 0), nullptr.
bool _null(const NodeP& e);
bool _from_alloc(const NodeP& e);

// ============================================================================================ analysis
// The pointer analysis that picks the tier, and the facts both tiers need.
class Analysis {
  public:
    py::list<NodeP> units;
    py::list<py::str> why;             // what keeps the program from the idiomatic tier
    py::set<py::str> addressed;        // sym names whose address is taken
    bool cpp_classes = false;
    std::set<const void*> exempt;

    explicit Analysis(const py::list<NodeP>& units) : units(units) {}
    static const py::set<py::str>& RAW();
    Analysis& run();
    void visit(const NodeP& n);
};

struct Program {
    py::str name;
    py::str source;
    py::str tier;
    py::list<py::str> notes;
    std::optional<py::str> refused;

    Program(const py::str& name, const py::str& source, const py::str& tier, const py::list<py::str>& notes,
            const std::optional<py::str>& refused = std::nullopt)
        : name(name), source(source), tier(tier), notes(notes), refused(refused) {}
};

// L2 units of one C/C++ program -> Program (Java source of class `name`). Raises Refused.
// tier: 'auto' (idiomatic where the analysis allows it, else faithful), 'idiomatic' or 'faithful'.
Program program(const py::list<NodeP>& units, const py::str& name, const py::str& tier = py::str("auto"));

// ============================================================================================ faithful
// A C variable as the Java side keeps it: `java` its Java name; `mem` True when it lives in the
// emulated memory (then `java` names an int holding its address).
struct Var {
    py::str java;
    bool mem = false;
    TypeP type;
    py::str kind;

    Var() {}
    Var(const py::str& java, bool mem, const TypeP& type, const py::str& kind) : java(java), mem(mem), type(type), kind(kind) {}
};

// (java param types, java return type) of a function
struct Sig {
    std::vector<py::str> params;
    py::str ret;
    bool operator==(const Sig& o) const { return params == o.params && ret == o.ret; }
};

// one field of a record Type's layout: {name, offset, type}
struct LayoutField {
    py::str name;
    int64_t offset = 0;
    TypeP type;
};
std::vector<LayoutField> layout_fields(const TypeP& lay);
int64_t layout_size(const TypeP& lay);

// the parameter types and the result type of an 'fn' Type (info['params'][i]['type'], info['result'].get('type'))
std::vector<TypeP> fn_params(const TypeP& fn);
TypeP fn_result(const TypeP& fn);

class FnWriter;

// The faithful tier: C memory emulated in crt.Mem.
class Faithful {
  public:
    py::list<NodeP> units;
    py::str name;
    std::shared_ptr<Analysis> a;
    py::list<py::str> notes;
    int64_t ptr_size = 8;
    py::dict<py::str, ClassP> records;           // qname -> ClassDecl (with layout)
    py::dict<py::str, py::str> layout_names;     // qname -> Java class name of its offsets
    py::dict<py::str, MethodP> functions;        // name -> MethodDecl
    py::dict<py::str, Var> globals;              // name -> Var
    py::list<FieldP> global_decls;               // FieldDecl in order
    std::vector<std::pair<Sig, py::str>> fn_ifaces;   // signature key -> interface name
    py::dict<py::str, py::str> fn_consts;        // function name -> constant naming its address
    py::set<py::str> used_names;
    py::list<py::str> gaps;                      // functions written as an explicit gap: 'name (reason)'
    py::dict<py::str, py::list<MethodP>> overloads;   // name -> [MethodDecl] where C++ overloads one name
    py::set<py::str> addr_taken;
    py::list<py::str> fields_out;
    py::list<py::str> static_init;

    Faithful(const py::list<NodeP>& units, const py::str& name, const std::shared_ptr<Analysis>& analysis);
    virtual ~Faithful() {}
    // the rest of Python's __init__ (it calls overridable methods)
    void init();

    virtual void _check_records();
    virtual void _names();
    void _addressed();
    py::list<MethodP> all_functions();
    MethodP function_for(const SymP& s);
    static py::str key(const SymP& s);

    // ---- types
    virtual py::str jt(const TypeP& t);
    TypeP layout(const TypeP& t);
    py::str rec_name(const TypeP& t);
    int64_t size(const TypeP& t);
    int64_t align(const TypeP& t);
    // The Mem accessor suffix for a scalar type: Int, Byte, Ptr, ...
    py::str access(const TypeP& t);
    py::str size_code(const TypeP& t);
    LayoutField field_off(const TypeP& rec_t, const py::str& fname);

    // ---- the file
    virtual py::str write();
    Lines function_or_gap(const MethodP& f, const std::function<Lines()>& write, const py::str& indent = py::str("    "),
                          const std::optional<py::str>& owner = std::nullopt);
    virtual std::optional<py::str> gap_signature(const MethodP& f, const std::optional<py::str>& owner);
    Lines layout_class(const ClassP& c);
    virtual Lines entry();

    // ---- globals
    virtual void declare_global(const FieldP& g);
    std::optional<py::str> const_value(const NodeP& e, const TypeP& t);
    virtual void init_global(const FieldP& g);

    // ---- functions
    virtual Sig fn_sig(const SymP& s);
    virtual Sig fn_sig_of_type(const TypeP& ft);
    virtual py::str iface(const Sig& sig);
    py::str fn_const(const py::str& name);
};

std::shared_ptr<Faithful> make_faithful(const py::list<NodeP>& units, const py::str& name, const std::shared_ptr<Analysis>& a);

// ============================================================================================ functions
struct Place {
    py::str java;
    bool mem = false;
    TypeP type;
    int64_t prec = P_ATOM;

    Place() {}
    Place(const py::str& java, bool mem, const TypeP& type, int64_t prec = P_ATOM) : java(java), mem(mem), type(type), prec(prec) {}
};

// A Java expression already written, standing in the tree for a compound assignment's operand.
struct CodeNode final : ir::Kind<CodeNode, ir::Expr> {
    static constexpr const char* NAME = "_Code";
    J j;
    template <class F> void each_slot(F&&) {}
};

bool _is_int_text(const py::str& code);
bool _balanced_call(const py::str& text);
py::str _promoted(const py::str& jt);
py::str _promoted2(const py::str& a, const py::str& b);
TypeP _promote_type(const TypeP& t);

// One function's body in the faithful tier.
class FnWriter {
  public:
    Faithful* p;
    MethodP fn;
    py::dict<py::str, Var> vars;               // local name -> Var
    int64_t frame = 0;                         // frame bytes so far
    bool has_frame = false;
    Lines lines;
    int64_t tmp = 0;
    py::set<py::str> declared;
    std::map<const void*, py::str> goto_map;   // id(Goto) -> 'break L' / 'continue L'
    std::set<const void*> labels_done;         // id(Labeled) placed by structure()
    bool ret_struct = false;

    FnWriter(Faithful* prog, const MethodP& fn);
    virtual ~FnWriter() {}

    // ---- frame
    py::str alloc(const TypeP& t, std::optional<int64_t> size = std::nullopt, std::optional<int64_t> align = std::nullopt);
    py::str temp(const TypeP& t);
    py::str fresh(const py::str& base);

    // ---- the function
    virtual Lines write();

    // ---- statements
    py::list<NodeP> structure(const py::list<NodeP>& stmts);
    bool loose_jump(const NodeP& s);
    void block_stmts(const ir::Nodes& stmts, Lines& out, int64_t ind);
    void block_stmts(const py::list<NodeP>& stmts, Lines& out, int64_t ind);
    bool abrupt(const NodeP& s);
    bool can_complete(const NodeP& s);
    bool can_complete(const ir::NodeOrList& s);
    bool const_true(const NodeP& c);
    bool has_break(const NodeP& body);
    bool has_break(const ir::NodeOrList& body);
    Lines sub(const ir::NodeOrList& s);
    Lines sub(const NodeP& s);
    void stmt(const NodeP& s, Lines& out);
    void stmt(const ir::NodeOrList& s, Lines& out);
    py::str loop_cond(const NodeP& c);
    void for_(const py::ref<ir::For>& s, Lines& out);
    void switch_(const py::ref<ir::Switch>& s, Lines& out);
    virtual void local(const py::ref<ir::Local>& s, Lines& out);
    virtual void static_local(const py::ref<ir::Local>& s, const py::str& name);
    py::str zero(const py::str& jt);
    Lines init_place(const py::str& addr, const TypeP& t, const NodeP& init, bool fresh = false);
    std::optional<py::str> small_lit(const NodeP& x, const TypeP& et);
    // The scalar leaves of a (nested) array initialiser when it covers whole rows -> ([exprs], scalar type)
    std::optional<std::pair<py::list<NodeP>, TypeP>> flat_leaves(const py::list<NodeP>& items, const TypeP& et);
    TypeP _scalar(TypeP t);
    Lines wrap_call(const py::str& head, const py::list<py::str>& vals, const py::str& tail, int64_t width = 100);
    py::str offset(const py::str& addr, int64_t off);
    py::str field_addr(const py::str& addr, const py::str& ln, const LayoutField& f);
    virtual void ret(const py::ref<ir::Return>& s, Lines& out);
    virtual void expr_stmt(const NodeP& e, Lines& out);
    bool java_statement(const py::str& text);
    bool statement_expr(const NodeP& e);
    py::str expr_text(const NodeP& e);

    // ---- places
    virtual Var var(const SymP& s);
    virtual Place place(const NodeP& e);
    bool pure_place(const NodeP& e);
    bool pure(const NodeP& e);
    virtual J addr(const NodeP& e);
    virtual J load(const Place& pl);
    virtual J store(const Place& pl, const J& val);

    // ---- values
    J cond(const NodeP& e);
    virtual J truth(const J& j, const TypeP& t);
    J as_value(const J& j, const TypeP& t);
    J value(const NodeP& e);
    virtual J x_Paren(const py::ref<ir::Paren>& e);
    virtual J x_Unsupported(const py::ref<ir::Unsupported>& e);
    virtual J x_Lit(const py::ref<ir::Lit>& e);
    virtual J x_Name(const py::ref<ir::Name>& e);
    virtual J x_Select(const py::ref<ir::Select>& e);
    virtual J x_Index(const py::ref<ir::Index>& e);
    virtual J x_TypeName(const py::ref<ir::TypeName>& e);
    virtual J x_AddressOf(const py::ref<ir::AddressOf>& e);
    virtual J x_Unary(const py::ref<ir::Unary>& e);
    J narrow(const J& j, const py::str& jt);
    virtual J incdec(const py::ref<ir::Unary>& e, bool as_statement = false);
    virtual J x_Binary(const py::ref<ir::Binary>& e);
    J ptr_add(const J& base, const J& idx, const TypeP& it, const TypeP& pt, const py::str& op);
    virtual J compare(const py::ref<ir::Binary>& e, const py::str& op);
    virtual J x_Assign(const py::ref<ir::Assign>& e);
    J compound(const Place& pl, const py::ref<ir::Assign>& e);
    J x__Code(const py::ref<CodeNode>& e);
    virtual J x_Cond(const py::ref<ir::Cond>& e);
    virtual J x_Cast(const py::ref<ir::Cast>& e);
    virtual J conv(const J& j, const TypeP& s, const TypeP& t);
    J widen(const J& j, const py::str& sk, const py::str& to);
    J retype(const J& j, const py::str& tj);
    virtual J x_Call(const py::ref<ir::Call>& e);
    Lines args(const py::list<NodeP>& args, const std::vector<TypeP>& params, const TypeP& ret);
    J own_call(const MethodP& f, const py::str& jn, const py::list<NodeP>& args, const py::ref<ir::Call>& e);
    virtual J lib_call(const py::ref<ir::Call>& e, const py::str& name, const py::list<NodeP>& args);
    virtual J x_New(const py::ref<ir::New>& e);
    py::str seq_lines(const Lines& lines, const py::str& result);
    virtual J x_NewArray(const py::ref<ir::NewArray>& e);
    // (idiomatic tier only: the faithful tier has no such expressions)
    virtual J x_Concat(const py::ref<ir::Concat>& e);
    virtual J x_Length(const py::ref<ir::Length>& e);
    virtual J x_This(const py::ref<ir::This>& e);
};

}  // namespace codebreaker::translate::backends::java
