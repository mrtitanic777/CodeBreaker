// codebreaker/translate/frontends/csharp/lower.hpp -- typed C++ port of codebreaker.translate.frontends.csharp.lower.
//
// lower.py -- Roslyn's bound tree (CsTreeDump's JSON) into the translator's IR (codebreaker/translate/ir.py).
//
// Declarations are lowered from the symbols the compiler gave them; bodies from Roslyn's IOperation tree,
// in which every call already names its overload and lists its arguments in parameter order (defaults and
// `params` arrays filled in), every implicit conversion is a node, and `foreach`, `using`, `lock`, string
// interpolation, patterns and LINQ's query syntax are already what they mean. So:
//
//   * a name is lowered from the symbol it binds to (a field read on this or on its class, a local, a
//     parameter, a property, an indexer), never from its spelling;
//   * implicit conversions that every target makes the same way (widening, boxing, identity) are left
//     implicit, as javac's tree leaves them; explicit ones are Casts (`checked`: overflow 'trap');
//     user-defined ones, and user-defined operators, are calls of their op_ methods (Call, flags
//     'operator'), as are the BCL's operators that are methods (string ==, decimal); `as` is a test and a
//     cast;
//   * C#'s own kinds of member and type keep their shape: a property or indexer is a Property declaration
//     and is read as a Select (an indexer as an Index with its sym); a struct, a record, a delegate are
//     ClassDecls of that kind; an enum constant carries its number (EnumConst.value);
//   * what needs a statement where C# has an expression (an object or collection initialiser, a switch
//     expression on a value with effects, an `out var` declaration) is hoisted before the statement --
//     only where that cannot change what runs first: never out of a branch that might not run, a loop
//     condition, or past an earlier effect in the same statement.
//
// What cannot be carried faithfully yet becomes ir.Unsupported(kind, reason), never a guess: `await`,
// iterators (`yield return`), tuples and deconstruction, local functions, lifted nullable arithmetic,
// pattern-matching `switch` statements, `goto case`, exception filters, events, unsafe code, `dynamic`.
//
// Typing notes (how the Python's values are typed here):
//   * A node of CsTreeDump's JSON is a Dict (py::dict<py::str, Val>) of Vals -- the ir.hpp Val is exactly what
//     JSON holds (None, bool, int, float, str, a list, a dict). A type record, a symbol record and a constant
//     record are Vals too (None, or the dict). load_tree() reads a dump's text into a Dict. Where the Python
//     reads a child with `n['x']` / `n.get('x')` the C++ does the same on the Dict: a missing key is a
//     KeyError, a child that is None where a node is subscripted a TypeError, where `.get` is called on it an
//     AttributeError.
//   * `dict(line=..., col=...)` of self.pos(n) is the struct Pos; `**self.pos(n)` sets a node's line and col.
//   * A Python private name keeps its meaning with a trailing underscore: _PRIM, _BINARY, _UNARY, _REF,
//     _PREC stay in the .cpp; _value, _concat, _bindings, _is_null, _simple, _effects, _implicit_body, _prec
//     are value_, concat_, bindings_, is_null_, simple_, effects_, implicit_body_, prec_; the method `new`
//     is new_ (a C++ keyword), `try_` and `default` are try_ and default_ (as the Python's try_ and default
//     are).
//   * _Refuse (the exception of a construct that cannot be carried, caught inside the Lowering) is Refuse. One
//     that is not caught (a constructor initialiser's argument that refuses) leaves lower() as the Python
//     leaves it, as a Refuse.
//   * Lowering.receiver_now and Lowering.cond_recv (the Python makes them with setattr/getattr) are plain
//     members, null for None. `self.pending` is an optional list, shared by reference as the Python's list is.
//   * Lit.value / the slots that hold what the JSON holds are typed as in ir.hpp. A constant too large for
//     int64 (a ulong above 2**63-1, in the dump a string) raises OverflowError where the Python keeps a big
//     int.
//   * Python ints are int64_t here.
#pragma once

#include <optional>
#include <tuple>

#include "codebreaker/translate/ir.hpp"
#include "py.hpp"

namespace codebreaker::translate::frontends::csharp::lower {

namespace ir = codebreaker::translate::ir;
using ir::Dict;
using ir::NodeP;
using ir::SymP;
using ir::TypeP;
using ir::Val;

extern const py::str LANG;
extern const TypeP STRING;
extern const TypeP OBJECT;

// -------------------------------------------------------------------------------------- JSON
// A dump's text (one file's CsTreeDump JSON) as a node Dict. ValueError for text that is not JSON.
Dict load_tree(const py::str& text);

// -------------------------------------------------------------------------------------- types
// A CsTreeDump type record -> ir.Type (None: ANY).
TypeP ty(const Val& d);

bool is_string(const TypeP& t);

// The TypeKind of a type record (Class, Struct, Enum, Interface, Delegate), or None.
Val tkind(const Val& d);

// -------------------------------------------------------------------------------------- symbols
// A CsTreeDump symbol record -> ir.Sym (None for None or an empty record).
SymP sym(const Val& d);

// A CsTreeDump constant record -> a Python value (as ir.Lit keeps it).
Val value_(const Val& c);

// -------------------------------------------------------------------------------------- the lowering
// A construct that cannot be carried (the Python's _Refuse): its kind and why.
struct Refuse {
    std::optional<py::str> kind;
    py::str why;
};

// self.pos(n): dict(line=, col=)
struct Pos {
    int64_t line = 0;
    int64_t col = 0;
};

class Lowering {
  public:
    std::optional<py::str> path;
    py::ref<ir::ClassDecl> cls;                                          // the ClassDecl being lowered
    std::optional<py::list<NodeP>> pending;                              // statements hoisted before the statement being lowered
    int64_t guard = 0;                                                   // > 0: inside a branch that might not run, a loop condition
    int64_t effects = 0;                                                 // effects lowered so far in this statement
    int64_t temps = 0;
    py::list<std::tuple<int64_t, std::optional<py::str>, py::str>> refused;   // (line, kind, why): what became Unsupported
    py::dict<py::str, py::str> rename;                                   // a local's name -> the name it is given
    NodeP receiver_now;
    NodeP cond_recv;

    explicit Lowering(const std::optional<py::str>& path = std::nullopt) : path(path) {}

    Pos pos(const Dict& n) const;
    SymP renamed(const SymP& s) const;
    py::ref<ir::Unsupported> unsupported(const Dict& n, const std::optional<py::str>& kind, const py::str& why,
                                         const TypeP& t = nullptr);

    // ---- declarations
    py::ref<ir::Unit> unit(const Dict& n);
    // Top-level statements: the compiler's Program.Main, as a class.
    py::ref<ir::ClassDecl> top_level(const Dict& n);
    py::list<NodeP> tparams(const Val& tps);
    py::ref<ir::ClassDecl> decl(const Dict& n, const std::optional<py::str>& outer);
    // A record's (or class's) primary constructor: its parameters, and for a record the public
    // properties the compiler makes of them (mods 'positional').
    void primary(const py::ref<ir::ClassDecl>& c, const Dict& n);
    py::list<NodeP> params(const Val& ps);
    py::list<NodeP> member(const Dict& m, const py::ref<ir::ClassDecl>& c);

    // ---- bodies
    // An IMethodBodyOperation / IBlockOperation -> Block (null for None).
    py::ref<ir::Block> method_body(const Val& op);
    py::ref<ir::Block> ctor_body(const Val& op, const Dict& m);
    // `: base(...)` / `: this(...)` (or the compiler's own base()) -> CtorCall.
    NodeP ctor_init(const Dict& st);
    py::ref<ir::Block> block(const Dict& op);
    py::list<NodeP> stmts(const py::list<Val>& ops);
    // A statement in a place that holds one (a loop's or an if's body).
    NodeP one(const Dict& op);
    // One operation in statement position -> [Stmt] (hoisted statements first).
    py::list<NodeP> stmt(const Dict& op);
    py::list<NodeP> stmt_(const Dict& op);
    py::ref<ir::Block> as_block(const Dict& op);
    py::list<NodeP> loop(const Dict& op, const Pos& p);
    py::ref<ir::Switch> switch_(const Dict& op, const Pos& p);
    py::ref<ir::Try> try_(const Dict& op, const Pos& p);

    // ---- expressions
    // A field's or property's initialiser: an expression with no statement to hoist into.
    NodeP expr_in_decl(const Dict& op);
    // Put statements before the statement being lowered, where that cannot change what runs first.
    void hoist(const Dict& n, const py::list<NodeP>& stmts, bool needs_order = true);
    NodeP temp(const TypeP& t, const NodeP& init, const Dict& n);
    NodeP expr(const Dict& n);
    NodeP expr_(const Dict& n);
    // The object a member is read on: This (implicit or written), its class for a static, the
    // receiver of an object initialiser, or an expression.
    NodeP receiver(const Dict& n, const SymP& s);
    // A call's arguments in parameter order, as written: a `params` array spread back out, defaults the
    // compiler filled in kept (every target then sees the value).
    py::list<NodeP> args(const Dict& n, const SymP& s);
    NodeP call(const Dict& n, const TypeP& t, const Pos& p);
    // A user-defined operator or conversion, or a BCL operator that is a method: a static call.
    NodeP op_call(const Dict& n, const SymP& s, const py::list<NodeP>& args, const TypeP& t, const Pos& p);
    NodeP new_(const Dict& n, const TypeP& t, const Pos& p);
    NodeP new_array(const Dict& n, const TypeP& t, const Pos& p);
    NodeP binary(const Dict& n, const TypeP& t, const Pos& p);
    NodeP conversion(const Dict& n, const TypeP& t, const Pos& p);
    // a?.b: when `a` can be read twice (a name, this, a field of one), a test and the access.
    NodeP cond_access(const Dict& n, const TypeP& t, const Pos& p);
    NodeP default_(const Dict& n, const TypeP& t, const Pos& p);
    NodeP interpolated(const Dict& n, const TypeP& t, const Pos& p);
    NodeP lam(const Dict& n, const TypeP& t, const Pos& p);

    // ---- patterns
    NodeP pattern(const Dict& pat, const NodeP& v, const Pos& p);
    NodeP switch_expr(const Dict& n, const TypeP& t, const Pos& p);
};

// -------------------------------------------------------------------------------------- helpers
NodeP concat_(const NodeP& left, const NodeP& right, const TypeP& t, const Pos& p);

// The names a pattern declares.
py::list<py::str> bindings_(const Val& pat);

bool is_null_(const NodeP& e);

// An expression that can be read twice: a name, this, a literal, a field of one.
bool simple_(const NodeP& e);

bool effects_(const NodeP& e);

// A lambda whose body was an expression: Roslyn wraps it in an implicit block and return.
bool implicit_body_(const Dict& b);

int64_t prec_(const NodeP& e);

// Roslyn's bound tree keeps no parentheses; the source had them wherever an operand binds looser than
// its operator. Put those back as Paren nodes (as javac's tree has them), so a back end that prints the
// grouping the source wrote groups the operands as C# does.
NodeP group(const NodeP& node);

// One file's CsTreeDump JSON -> ir.Unit.
py::ref<ir::Unit> lower(const Dict& tree, const std::optional<py::str>& path = std::nullopt);

// -> (ir.Unit, [(line, kind, why)]): what became Unsupported, and why.
std::tuple<py::ref<ir::Unit>, py::list<std::tuple<int64_t, std::optional<py::str>, py::str>>> lower_with_refusals(
    const Dict& tree, const std::optional<py::str>& path = std::nullopt);

}  // namespace codebreaker::translate::frontends::csharp::lower
