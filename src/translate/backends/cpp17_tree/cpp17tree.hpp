// translate/backends/cpp17_tree/cpp17tree.hpp -- the translator's C++17 back end on the typed tree (docs/ENGINE_UNIFICATION.md item
// 1): ONE back end for every front end, reading translate::ir. A port line by line of the L2 core library's C++17 back end
// (translate/l2/src/cpp17*, itself a port of the Python reference): the same names, the same order of work, the same text. What
// moved is the tree it reads: the nodes are translate::ir's, read by the names their documents give their slots (l2::slot), the
// types and symbols ir's structs; the state a writer keeps of a node is keyed on its ir::Node*.
//
// What a source language means is a rule set apart, chosen by the unit's language (Unit.lang):
//
//   'python'  py.hpp semantics (config/translate/runtime/cpp17/py.hpp): int64_t where the front end proved the range, floor
//             division and modulo signs, UTF-8 str and bytes, dict insertion order, value records and shared objects (python.cpp);
//   'java'    jrt.hpp semantics (translate/runtime/cpp17/jrt.hpp): wrapping ints, masked shifts, truncating division that throws
//             on zero, saturating double-to-int, UTF-16 strings, two-phase construction where a constructor's virtual call could
//             reach an override (java.cpp, over the Java object model in jmodel.cpp);
//   others    decline until their rules are written.
//
//     cpp17tree::write_unit(unit, api_root)  -> (header, source, {function: (first, last)})  a Python module
//     cpp17tree::program(units, main)        -> the whole program's C++ text                  a Java program
//
// Both raise cpp17tree::Decline for what they cannot write. The nodes a writer makes (a field's initialiser as an assignment, a
// literal ...) are made in the arena of the call (Made).
#pragma once
#include <cstdint>
#include <cstring>
#include <map>
#include <memory>
#include <set>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "core/arena.hpp"
#include "translate/ir.hpp"

#if defined(_MSC_VER)
#pragma warning(push, 3)   // (the L2 core library's headers: compiled under its own flags)
#endif
#include "translate/l2/src/pyre.hpp"
#include "translate/l2/src/pystr.hpp"
#include "translate/l2/src/value.hpp"
#if defined(_MSC_VER)
#pragma warning(pop)
#endif

namespace cb::translate::cpp17tree {

using NodeP = ir::Node*;
using TypeP = ir::TypeP;
using SymP = ir::SymP;
using py::V;
using Strs = std::vector<std::string>;

struct Decline : std::runtime_error {
    explicit Decline(const std::string& m) : std::runtime_error(m) {}
    std::string where;    // the function (its Sym's owner.name) being written, when it is one
};

// ---------------------------------------------------------------------------------------------- entry points
V write_unit(NodeP unit, const std::string& api_root);
// the C++ name write_unit gives a Python name (a function, a part of a module's name): a keyword or a C runtime macro gets "_"
std::string cpp_name(const std::string& python_name);
std::string program(const std::vector<NodeP>& units, const V& main);

// The source language's rules for a unit's language: 'python' or 'java' (a unit without one is Java's, as L2 from the Java front
// end has none); others decline.
enum class Lang { Python, Java };
Lang semantics(const V& lang);

// The arena the nodes a writer makes go in: the call's (entry.cpp sets it for the call, on its thread).
struct Made {
    Arena* was;
    explicit Made(Arena& a);
    ~Made();
};
Arena& arena();

// ---------------------------------------------------------------------------------------------- the tree
inline bool is(const ir::Node* n, const char* k) { return n != nullptr && ir::kind_name(*n) == k; }
NodeP node(NodeP n, const char* slot);                                // the node in a slot (a body of one statement too)
const std::vector<NodeP>& nodes(NodeP n, const char* slot);           // a list of nodes ([] when empty)
TypeP type(NodeP n, const char* slot = "type");
std::vector<TypeP> types(NodeP n, const char* slot);
SymP sym(NodeP n, const char* slot = "sym");
V raw(NodeP n, const char* slot);                                     // a slot that is no node, type or symbol, as its value
void set_type(NodeP n, const TypeP& t, const char* slot = "type");
void put(NodeP n, const char* slot, NodeP child);                     // a node slot set
void put_sym(NodeP n, const SymP& s, const char* slot = "sym");
NodeP make_node(const char* kind);                                    // a new node of the kind, every slot empty (in arena())
bool has_mod(NodeP n, const std::string& m);                          // m in (n.mods or [])
std::vector<std::string> mods(NodeP n);
std::string str(const V& v);                                          // a str's text, else ''
std::vector<NodeP> children(NodeP n);                                 // ir.Node.children()
std::vector<NodeP> tree_walk(NodeP n);                                     // ir.walk: depth first, in order

// types (ir.Type)
std::string tkind(const TypeP& t);                                    // the kind as L2 spells it ('' for none)
std::string tname(const TypeP& t);                                    // a class's, type variable's or record's name
const std::vector<TypeP>& targs(const TypeP& t);                      // a class's type arguments
TypeP telem(const TypeP& t);                                          // an array's element (a pointer's target)
bool tinfo(const TypeP& t, const char* key);                          // what the type says of itself (ref, record ...), truthy
const ir::WildcardBound* tbound(const TypeP& t);                      // a wildcard's bound, or none
bool type_eq(const TypeP& a, const TypeP& b);                         // ir.Type.__eq__ (None == None)
TypeP mk_type(const std::string& kind, const std::string& name = std::string(), std::vector<TypeP> args = {},
              TypeP elem = TypeP());
bool is_prim(const TypeP& t);
bool is_ref_type(const TypeP& t);
std::string type_repr(const TypeP& t);                               // repr(ir.Type)

// symbols
std::string sname(const SymP& s);
std::string sowner(const SymP& s);   // '' for None
std::string skind(const SymP& s);
bool sstatic(const SymP& s);

// ---------------------------------------------------------------------------------------------- the emitter
// The C++17 forms, the same whatever the source: a block, the if / else-if chain, while, the range-for head, a structured
// binding, std::make_shared, std::optional, an immediately invoked lambda. Each language's writer says how a level is indented
// (levels are the indentation text: Python's four spaces, Java's tab), how a nested body is written (nested; loop_nested for a
// loop's body) and how a condition is written (cond_text).
struct Emitter {
    std::string unit;                          // one level of indentation
    explicit Emitter(const std::string& u) : unit(u) {}
    virtual ~Emitter() {}
    std::string deeper(const std::string& level) const { return level + unit; }
    virtual void nested(const NodeP& body, Strs& out, const std::string& level) = 0;
    virtual void loop_nested(const NodeP& body, Strs& out, const std::string& level) { nested(body, out, level); }
    virtual std::string cond_text(const NodeP& e) = 0;

    void emit_block(NodeP s, Strs& out, const std::string& level);
    void emit_if(NodeP s, Strs& out, const std::string& level);
    void emit_while(NodeP s, Strs& out, const std::string& level);
    static std::string range_for(const std::string& head, const std::string& range) {
        return "for (" + head + " : " + range + ") {";
    }
    static std::string binding(const Strs& names, const std::string& qual = "auto");
    static std::string make_shared(const std::string& cls, const std::string& args) {
        return "std::make_shared<" + cls + ">(" + args + ")";
    }
    static std::string optional_of(const std::string& inner) { return "std::optional<" + inner + ">"; }
    static constexpr const char* NULLOPT = "std::nullopt";
};

// text helpers
std::string join(const Strs& v, const std::string& sep);
std::string fmt_int(int64_t v);
bool starts(const std::string& s, const std::string& p);
bool ends(const std::string& s, const std::string& p);
std::string float_text(double v, const std::string& suffix);   // cpp03._float (Decline when not finite)
bool lit_float(NodeP lit, double& out);                         // a Lit's value as a float (int, float, or the text L2 gives
                                                                // inf/nan), false if none

}  // namespace cb::translate::cpp17tree
