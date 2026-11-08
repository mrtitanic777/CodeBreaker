// codebreaker/translate/frontends/clang/lower.hpp -- typed C++ port of codebreaker.translate.frontends.clang.lower.
//
// lower.py -- clang's typed AST (-ast-dump=json) into the translator's IR (codebreaker/translate/ir.py).
//
// What clang resolved is what the IR says: every implicit conversion is an explicit Cast (marked
// implicit), every name is the declaration clang bound it to, overloads and template instantiations are
// the ones clang chose. Only the declarations of the program's own files are lowered (not those of the
// headers it includes); a call into a library keeps the header that declared it as the symbol's owner.
//
// C's model in the IR:
//   * a translation unit is one Unit: its structs/unions/classes (ClassDecl, with their byte layout for
//     C/C++ records), its functions (MethodDecl kind "function"), its globals (FieldDecl, sym kind
//     "global"); C enum constants are int globals (mods const), a C++ `enum class` a ClassDecl kind "enum";
//   * pointers are 'ptr' types; `*p` and `&x` are Unary "*" and "&"; `p->f` is Select on `*p`;
//   * a C++ reference is a 'ptr' with ref: its reads are Unary "*", a binding Unary "&";
//   * `this` is a pointer (This typed ptr to the class), so `this->x` is Select on `*this`;
//   * a string literal's value is its bytes, one character per byte (latin-1); its type char[N];
//   * sizeof/alignof are Lit values by the target's layout, `text` keeping the spelling;
//   * an aggregate initialiser is NewArray (arrays) or New with memory "stack" and the field values in
//     order (structs); missing trailing elements are zero, as in C;
//   * a static local is a Local with mods ["static"].
// Anything not handled becomes ir.Unsupported(kind), never a guess.
//
// Typing notes (how the Python's values are typed here):
//   * A node of clang's JSON AST is a Dict (py::dict<py::str, Val>) of Vals -- the ir.hpp Val is exactly what
//     JSON holds (None, bool, int, float, str, a list, a dict). The Python writes the position of each
//     node into it (`_f`, `_l`, `_c` and `_rb`, a tuple there, a list of three Vals here) and the C++ does
//     the same: a Dict is shared, not copied, so the declaration index (`decls`, `parent`, `records`)
//     and the tree see one node. load_tree() reads a dump's text into a Dict with py::json_parse.
//   * A Python dict that maps an id or a name to something else is a py::dict of that type: decls and
//     parent (id -> node), qnames (id -> key), records (key -> node), typedefs (name -> (kind, id or
//     text)), enum_values (id -> int), scoped_enums (id -> qualified name), labels (id -> name or None),
//     layout_cache (key -> 'record' Type or None). `sigs` (id(MethodDecl) -> (name, type text)) is a
//     std::map keyed by the node's address.
//   * Python's `dict(line=..., col=...)` of self.pos(n) is the struct Pos; `**self.pos(n)` sets a node's
//     line and col.
//   * A Python private name keeps its meaning with a trailing underscore: _esc_value -> esc_value_,
//     _wide_value -> wide_value_, _char_text -> char_text_, _wrap -> wrap_, _Scope -> Scope, and a method
//     _index -> index_, _name -> name_, _decl -> decl_ ... ; the methods whose names are C++ keywords
//     (const, enum, new, switch, sizeof) are const_, enum_, new_, switch_, sizeof_. The tables _LOC_KEYS,
//     _OPS, _DROP_CASTS, _WRAPPERS and _CASTS stay in the .cpp.
//   * self.stmt(n) answers a statement, or a list of statements (a declaration of several variables):
//     a NodeOrList. self.const(n) answers an int or None. The Python's `std` argument is std_ (an
//     optional str); `own` is an optional list of paths.
//   * A node id that is absent is looked up as "None" (the Python indexes dicts by None there); no clang
//     dump has a declaration without an id.
//   * The Python's recursion limit (1000 frames) is kept only where a mutated or hostile dump makes the Python
//     recurse for ever: a typedef that names itself raises RecursionError after 40 nested name_ calls (the
//     Python takes about 250 levels of the parse -> name_ -> parse chain to reach its limit; a C++ level costs
//     a good deal more stack than a Python frame, and 40 typedefs in a chain is far past any real program).
//     Deep but finite trees are lowered here where the Python would stop at 1000 frames.
//   * Python ints are int64_t here: a constant past int64 (an unsigned 64-bit literal's value, a shift that
//     leaves it) raises OverflowError instead of growing, and `int(a / b)` divides doubles as the Python
//     divides ints only while both fit a double's 53 bits.
//   * Python's `except (TypeError, KeyError, ValueError, IndexError)` is a catch of py::TypeError,
//     py::KeyError, py::ValueError and py::IndexError (and the classes derived from them). The text of
//     such an error, which Unsupported.raw keeps, is the Python's message.
#pragma once

#include <map>
#include <optional>
#include <tuple>
#include <variant>

#include "codebreaker/translate/frontends/clang/ctypes.hpp"
#include "codebreaker/translate/ir.hpp"
#include "py.hpp"

namespace codebreaker::translate::frontends::clang::lower {

namespace ir = codebreaker::translate::ir;
namespace ctypes = codebreaker::translate::frontends::clang::ctypes;
using ir::Dict;
using ir::NodeOrList;
using ir::NodeP;
using ir::SymP;
using ir::TypeP;
using ir::Val;

// The unit holds something the front end does not lower at all.
struct Refused : py::Exception {
    Refused() : py::Exception() {}
    explicit Refused(const std::string& m) : py::Exception(m) {}
    const char* type() const override { return "Refused"; }
};

// RecursionError: a typedef that names itself is parsed for ever (the Python stops at its recursion limit).
struct RecursionError : py::RuntimeError {
    RecursionError() : py::RuntimeError("maximum recursion depth exceeded") {}
    const char* type() const override { return "RecursionError"; }
};

// A dump's text (the JSON `clang -Xclang -ast-dump=json` writes) as a node Dict. ValueError for text that
// is not JSON. (The Python reads it with json.loads; clang.dump does that.)
Dict load_tree(const py::str& text);

// A C string literal as clang spells it ("\"a\\n\"", u8"..", L"..") -> its bytes, latin-1.
py::str esc_value_(const Val& s);

py::str wide_value_(const py::str& body);

std::optional<py::str> char_text_(int64_t v);

// A canonical spelling of a type: the key template instances and records are found by.
py::str spell(const TypeP& t);

// An integer constant reduced to its type's width, as C does.
int64_t wrap_(int64_t v, const TypeP& t);

// self.pos(n): dict(line=, col=)
struct Pos {
    int64_t line = 0;
    int64_t col = 0;
};

// A function's names in use, its symbols by declaration id.
class Scope {
  public:
    explicit Scope(const std::optional<py::str>& owner);

    py::str fresh(const py::str& base);

    std::optional<py::str> owner;
    py::dict<py::str, SymP> syms;
    py::set<py::str> used;
    int64_t n = 0;
};

class Lowering {
  public:
    Lowering(const Dict& tree, const py::str& main_path, const std::optional<py::list<py::str>>& own = std::nullopt,
             const py::str& language = py::str("c"), const std::optional<py::str>& std_ = std::nullopt,
             const py::str& triple = py::str("x86_64-pc-windows-msvc"));
    Lowering(const Lowering&) = delete;
    Lowering& operator=(const Lowering&) = delete;

    // ------------------------------------------------------------------------------------- indexing
    // One pre-order walk: each node's file/line/column (clang writes them as deltas), and every
    // declaration by id, with its parent and qualified name.
    void index_();
    py::str record_key_(const Dict& n, const std::vector<py::str>& ns);
    int64_t enum_const_(const std::optional<Dict>& e);

    // ---------------------------------------------------------------------------------------- types
    // The parser's callback: a typedef / record / class / enum name -> ir.Type (null for None).
    TypeP name_(const py::str& name, const py::list<TypeP>& args, const std::optional<py::str>& tag);
    std::optional<Dict> find_record_(const py::str& key);
    std::optional<std::tuple<py::str, std::optional<py::str>>> find_typedef_(const py::str& name);
    bool find_enum_(const py::str& name);
    TypeP enum_type_(const py::str& eid);
    TypeP record_type_(const std::optional<Dict>& n);
    TypeP ty(const Dict& n, const char* key = "type");
    // sizeof's view of a class type: its record, or an int's for a C++ enum class.
    TypeP layout_of_(const TypeP& t);
    // The 'record' Type of a C/C++ struct type (for sizeof and ClassDecl.layout).
    TypeP record_layout_(const TypeP& t);
    bool polymorphic_(const Dict& rec);
    py::list<Dict> bases_(const Dict& rec);

    // ---------------------------------------------------------------------------------------- helpers
    Pos pos(const Dict& n);
    bool is_own_(const Val& f);
    py::ref<ir::Unsupported> unsupported(const std::optional<py::str>& kind, const std::optional<Dict>& n,
                                         const std::optional<py::str>& raw = std::nullopt);
    // A C integer constant expression's value, or None.
    std::optional<int64_t> const_(const std::optional<Dict>& n);

    // ---------------------------------------------------------------------------------------- the unit
    py::ref<ir::Unit> unit();
    void decls_(const py::list<Dict>& nodes, bool top = false);
    void decl_(const Dict& n);
    // A library symbol's owner: its C++ namespace ('std', 'System') where it has one, else the header
    // that declares it ('stdio.h').
    std::optional<py::str> lib_owner_(const Dict& node);
    // The record a VarDecl is a static data member of (in its class or defined outside it), or None.
    std::optional<Dict> member_owner_(const Dict& n);
    py::ref<ir::MethodDecl> find_method_(const py::ref<ir::ClassDecl>& cls, const Dict& n);
    std::optional<Dict> body_(const Dict& n);

    // ---- records
    py::ref<ir::ClassDecl> record(const Dict& n);
    void enum_(const Dict& n);
    py::ref<ir::FieldDecl> global_(const Dict& n);
    TypeP complete_array_(const TypeP& t, const NodeP& init);

    // ---- functions
    // A FunctionDecl / CXXMethodDecl node (or a referencedDecl stub) -> ir.Sym.
    SymP sym_of_function_(const Dict& d, const std::optional<py::str>& owner = std::nullopt, bool static_ = true);
    py::list<NodeP> params_(const Dict& n, Scope& scope);
    py::str local_name_(const Dict& d, Scope& scope);
    py::ref<ir::MethodDecl> function(const Dict& n);
    py::ref<ir::MethodDecl> method(const Dict& n, const std::optional<Dict>& owner,
                                   const py::str& access = py::str("public"));
    py::list<NodeP> ctor_init(const Dict& c, const std::optional<Dict>& owner);

    // ---------------------------------------------------------------------------------------- statements
    NodeOrList stmt(const Dict& n);
    py::list<NodeP> flat_(const py::list<NodeOrList>& xs);
    NodeP as_stmt_(const NodeOrList& s);
    py::list<NodeP> commas_(const NodeP& e);
    NodeOrList switch_(const Dict& n, const Dict& cond_n, const Dict& body_n, const Pos& p);
    // A case label below a statement of the switch body (Duff's device): not a structured switch.
    bool case_inside_(const Dict& body_n);
    // for (T x : array) -> ForEach over the array; anything else declines.
    NodeP range_for(const Dict& n, const Pos& p);
    py::ref<ir::Local> local(const Dict& d);

    // ---------------------------------------------------------------------------------------- expressions
    NodeP zero(const TypeP& t, const Pos& p);
    NodeP expr(const Dict& n);
    NodeP expr_(const Dict& n);
    NodeP implicit_cast(const Dict& n, const Dict& x_n, const TypeP& t, const Pos& p);
    NodeP default_arg(const Dict& n, const Pos& p);
    NodeP decl_ref(const Dict& n, const TypeP& t, const Pos& p);
    NodeP member(const Dict& n, const TypeP& t, const Pos& p);
    NodeP unary(const Dict& n, const TypeP& t, const Pos& p);
    // An argument given to a reference parameter: its binding made explicit (Unary '&'), as for a
    // pointer, so every back end sees which variables a call can change.
    py::list<NodeP> bind_refs(const py::list<NodeP>& args, const py::list<TypeP>& params);
    NodeP call(const Dict& n, const TypeP& t, const Pos& p);
    NodeP member_call(const Dict& n, const TypeP& t, const Pos& p);
    NodeP operator_call(const Dict& n, const TypeP& t, const Pos& p);
    NodeP construct(const Dict& n, const TypeP& t, const Pos& p);
    std::optional<Dict> find_ctor_(const Dict& cls, const Val& ctor_t);
    NodeP new_(const Dict& n, const TypeP& t, const Pos& p);
    NodeP sizeof_(const Dict& n);
    NodeP init_list(const Dict& n, const TypeP& t, const Pos& p);

    // ------------------------------------------------------------------------------------- state
    Dict tree;
    py::str main;
    py::set<py::str> own;
    bool auto_own = false;
    py::set<py::str> files;
    py::set<py::str> own_dirs;
    bool cpp = false;
    py::str language;
    std::optional<py::str> std_;
    ctypes::Model model;
    py::str unit_name;
    py::dict<py::str, Dict> decls;              // id -> decl node
    py::dict<py::str, Dict> parent;             // id -> the enclosing decl node (record, function, namespace)
    py::dict<py::str, py::str> qnames;          // record id -> qualified key
    py::dict<py::str, Dict> records;            // key -> record node (the complete definition)
    // name -> ('record' | 'enum', id) | ('type', text)
    py::dict<py::str, std::tuple<py::str, std::optional<py::str>>> typedefs;
    py::dict<py::str, int64_t> enum_values;     // EnumConstantDecl id -> int
    py::dict<py::str, py::str> scoped_enums;    // EnumDecl id -> qualified name (C++ enum class)
    py::dict<py::str, std::optional<py::str>> labels;   // LabelDecl id -> name
    py::dict<py::str, TypeP> layout_cache;
    // id(MethodDecl) -> (name, type text): an out-of-line body finds its declaration
    std::map<const ir::MethodDecl*, std::tuple<std::optional<py::str>, std::optional<py::str>>> sigs;
    ctypes::Parser types;
    ctypes::Layouts sizes;
    py::list<py::str> notes;

    // the Python makes these in unit() (and _find_record / _find_enum on first use)
    py::list<NodeP> out;
    py::dict<py::str, py::ref<ir::ClassDecl>> classes;   // record key -> ClassDecl
    std::shared_ptr<Scope> scope;                        // (self.scope: set by function() and method())
    py::dict<py::str, std::tuple<int64_t, std::optional<Dict>>> tail_memo;   // (_tail_memo)
    std::optional<py::set<py::str>> enum_names;                              // (_enum_names)
    int64_t name_depth = 0;                                                  // (how deep name_ is: the recursion limit)
};

}  // namespace codebreaker::translate::frontends::clang::lower
