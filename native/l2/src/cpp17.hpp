// cpp17.hpp -- the translator's C++17 back end (docs/TRANSLATOR_DESIGN.md): ONE back end for every
// front end, on the L2 core (l2.hpp). C++17 is written by one emitter; what a source language means is
// a rule set apart, chosen by the unit's language (Unit.lang):
//
//   'python'  py.hpp semantics (native/ported/include/py.hpp): int64_t where the front end proved the
//             range, floor division and modulo signs (py::floordiv, py::mod), UTF-8 str and bytes,
//             dict insertion order, value records and shared objects (cpp17_python.cc);
//   'java'    jrt.hpp semantics (translate/runtime/cpp17/jrt.hpp): wrapping ints, masked shifts,
//             truncating division that throws on zero, saturating double-to-int, UTF-16 strings,
//             two-phase construction where a constructor's virtual call could reach an override
//             (cpp17_java.cc, over the Java object model jmodel in cpp17_jmodel.cc);
//   'csharp', 'c', 'cpp'  have a place and decline until their rules are written.
//
// The two runtime headers stay: they are the libraries the generated code links against.
//
//     cpp17::write_unit(unit_doc, api_root)  -> (header, source, {function: (first, last)})  a module
//     cpp17::program(unit_docs, main)        -> the whole program's C++ text                  a program
//
// Both raise cpp17::Decline for what they cannot write (the shim raises the Python Decline).
#pragma once
#include <cstring>
#include <map>
#include <memory>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

#include "l2.hpp"
#include "pyre.hpp"
#include "pystr.hpp"
#include "value.hpp"

namespace cpp17 {

using l2::NodeP;
using l2::SymP;
using l2::TypeP;
using py::V;
using Strs = std::vector<std::string>;

struct Decline : std::runtime_error {
    explicit Decline(const std::string& m) : std::runtime_error(m) {}
    std::string where;    // the function (its Sym's owner.name) being written, when it is one
};

// ---------------------------------------------------------------------------------------------- entry points
V write_unit(const V& unit_doc, const std::string& api_root);
std::string program(const V& unit_docs, const V& main);

// The source language's rules for a unit's language: 'python' or 'java' (a unit without one is Java's,
// as L2 from the Java front end has none); others decline.
enum class Lang { Python, Java };
Lang semantics(const V& lang);

// ---------------------------------------------------------------------------------------------- the tree
inline bool is(const l2::Node* n, const char* k) { return n && std::strcmp(n->cls->name, k) == 0; }
inline bool is(const NodeP& n, const char* k) { return is(n.get(), k); }
const l2::Item* item(const NodeP& n, const char* slot);
NodeP node(const NodeP& n, const char* slot);
const std::vector<NodeP>& nodes(const NodeP& n, const char* slot);   // a list of nodes ([] when empty)
TypeP type(const NodeP& n, const char* slot = "type");
std::vector<TypeP> types(const NodeP& n, const char* slot);
SymP sym(const NodeP& n, const char* slot = "sym");
const V& raw(const NodeP& n, const char* slot);
void set_type(const NodeP& n, const TypeP& t, const char* slot = "type");
bool has_mod(const NodeP& n, const std::string& m);                   // m in (n.mods or [])
std::vector<std::string> mods(const NodeP& n);
std::string str(const V& v);                                          // a str's text, else ''
std::vector<NodeP> children(const NodeP& n);                          // ir.Node.children()
std::vector<NodeP> walk(const NodeP& n);                              // ir.walk: depth first, in order

// types (ir.Type)
std::string tkind(const TypeP& t);
std::string tname(const TypeP& t);
bool tinfo(const TypeP& t, const char* key);                          // t.info.get(key) truthy
bool type_eq(const TypeP& a, const TypeP& b);                         // ir.Type.__eq__ (None == None)
TypeP mk_type(const std::string& kind, const std::string& name = std::string(), std::vector<TypeP> args = {},
              TypeP elem = nullptr);
bool is_prim(const TypeP& t);
bool is_ref_type(const TypeP& t);
std::string type_repr(const TypeP& t);                               // repr(ir.Type)

// symbols
std::string sname(const SymP& s);
std::string sowner(const SymP& s);   // '' for None
std::string skind(const SymP& s);
bool sstatic(const SymP& s);

// ---------------------------------------------------------------------------------------------- the emitter
// The C++17 forms, the same whatever the source: a block, the if / else-if chain, while, the range-for
// head, a structured binding, std::make_shared, std::optional, an immediately invoked lambda. Each
// language's writer says how a level is indented (levels are the indentation text: Python's four
// spaces, Java's tab), how a nested body is written (nested; loop_nested for a loop's body) and how a
// condition is written (cond_text).
struct Emitter {
    std::string unit;                          // one level of indentation
    explicit Emitter(const std::string& u) : unit(u) {}
    virtual ~Emitter() {}
    std::string deeper(const std::string& level) const { return level + unit; }
    virtual void nested(const NodeP& body, Strs& out, const std::string& level) = 0;
    virtual void loop_nested(const NodeP& body, Strs& out, const std::string& level) { nested(body, out, level); }
    virtual std::string cond_text(const NodeP& e) = 0;

    void emit_block(const NodeP& s, Strs& out, const std::string& level);
    void emit_if(const NodeP& s, Strs& out, const std::string& level);
    void emit_while(const NodeP& s, Strs& out, const std::string& level);
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
bool lit_float(const NodeP& lit, double& out);                  // a Lit's value as a float (int, float, or
                                                                // the text L2 gives inf/nan), false if none

}  // namespace cpp17
