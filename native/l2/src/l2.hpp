// l2.hpp -- the L2 tree in C++: codebreaker/translate/ir.py's node model (every node kind and slot,
// the C#, clang and Python kits' optional ones too: Unit.lang and .target, ClassDecl.tparams and
// .layout, TypeParam, Property ...), read from and written to L2 JSON exactly as l2.from_json and
// l2.to_json do, and validated as l2.validate does against l2.schema.json.
//
//     l2::NodeP n = l2::from_json(doc);      // doc: the parsed JSON (py::V)
//     py::V back = l2::to_json(*n);          // what l2.to_json(l2.from_json(doc)) gives
//     std::string why; l2::Schema s(schema_json); s.valid(doc, &why);
//
// The back ends sit on this: a Node has its kind (Node::kind, the ir class name), its place (line,
// col, origin) and its slots by name (slot("cond"), the order ir.py gives them); a slot holds
// another Node, a list of them, a Type, a Sym, a list of Types, or a raw value (a string, a number, a
// dict such as origin or Unit.target). The C back end itself reads the JSON document directly, as
// backends/c.py does (L2 from a binary carries more than ir.py's classes need).
#pragma once
#include <memory>
#include <string>
#include <vector>

#include "value.hpp"

namespace l2 {

struct Node;
struct Type;
struct Sym;
using NodeP = std::shared_ptr<Node>;
using TypeP = std::shared_ptr<Type>;
using SymP = std::shared_ptr<Sym>;

// one slot's value
struct Item {
    enum Kind { Raw, NodeK, TypeK, SymK, Nodes, Types } kind = Raw;
    py::V raw;                     // Raw (None: the slot is empty)
    NodeP node;
    TypeP type;                    // (null: None)
    SymP sym;                      // (null: None)
    std::vector<NodeP> nodes;
    std::vector<TypeP> types;      // (an entry may be null: None)
    bool empty() const { return kind == Raw && raw.is_none(); }
};

// a type field ({name, offset, type, pad} of a record; {name, type, at} of an fn's parameter): the
// field's dict as it came, and its type read as a Type
struct TypedEntry {
    py::V dict;
    TypeP type;
};

struct Type {
    std::string kind;               // ir.PRIMS, void, null, any, dynamic, class, typevar, wildcard,
                                    // array, ptr, selector, fn, fnptr, record
    py::V name;                     // class, typevar, record
    std::vector<TypeP> args;        // class
    TypeP elem;                     // array; ptr's `to`
    // info (only the keys the type had, as Type(**info) keeps them)
    py::V info = py::V::dict();     // raw values by key: signed enc nullable count rank space seg ref of
                                    // conv distance pops loadds preserves variadic size role packed
    bool has_bound = false;         // wildcard: a bound given (its word, and its type)
    py::V bound;
    TypeP bound_type;
    std::vector<TypedEntry> params; // fn
    py::V result;                   // fn: the result's dict
    TypeP result_type;
    bool result_has_type = false;
    TypeP fn;                       // fnptr
    std::vector<TypedEntry> fields; // record
};

struct Sym {
    py::V kind, name, owner, stat = py::V(false), varargs = py::V(false);
    TypeP type, ret;
    std::vector<TypeP> params;
    std::vector<py::V> refs, flags;
};

struct NodeClass {
    const char* name;
    std::vector<std::string> slots;     // every slot, in ir.py's order (an Expr's `type` first)
    std::vector<std::string> fields;    // the child slots walk() follows
    bool expr, stmt;
    int index(const std::string& slot) const;
    bool is_field(const std::string& slot) const;
};

const NodeClass* node_class(const std::string& name);    // nullptr: no such node
const std::vector<NodeClass>& node_classes();

struct Node {
    const NodeClass* cls = nullptr;
    py::V line = py::V(0), col = py::V(0), origin;
    std::vector<Item> slots;             // by cls->slots
    const std::string kind() const { return cls->name; }
    Item& slot(const std::string& name);
    const Item* find(const std::string& name) const;
};

TypeP type_from(const py::V& d);
py::V type_json(const TypeP& t);
SymP sym_from(const py::V& d);
py::V sym_json(const SymP& s);
NodeP from_json(const py::V& d);          // KeyError / TypeError (py::PyError) where l2.from_json raises
py::V to_json(const Node& n);
py::V roundtrip(const py::V& d);          // to_json(from_json(d))

// l2.schema.json (draft 2020-12, the keywords it uses)
class Schema {
public:
    explicit Schema(const py::V& schema);
    bool valid(const py::V& doc, std::string* why = nullptr) const;

private:
    py::V root_;
    bool check(const py::V& s, const py::V& v, std::string& why, const std::string& at) const;
    const py::V& resolve(const std::string& ref) const;
};

}  // namespace l2
