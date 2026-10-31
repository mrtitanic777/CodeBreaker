// ir.hpp -- the L2 tree the lowering builds: codebreaker/translate/ir.py's nodes as one plain
// struct (only the slots the Python front end fills), written as L2 JSON exactly as l2.to_json
// writes them (keys in ir.py's slot order, None and empty non-child lists left out), so the C++17
// back end reads the same documents the Python front end produced.
#pragma once
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "value.hpp"

namespace pyengine {

struct IType;
using TyP = std::shared_ptr<const IType>;

// ir.Type (kind, name, args, and the info flags the lowering asks: ref, record, exception)
struct IType {
    std::string kind;
    std::string name;
    std::vector<TyP> args;
    bool ref = false, record = false, exception = false, has_ref = false;
    bool is_class(const char* n) const { return kind == "class" && name == n; }
};
bool ty_eq(const TyP& a, const TyP& b);          // ir.Type.__eq__ (info included)
std::string ty_repr(const TyP& t);
TyP prim(const char* kind);
TyP cls(const std::string& name, std::vector<TyP> args = {});
TyP cls_ref(const std::string& name, bool ref);   // Type('class', name, [], ref=...)
TyP cls_flag(const std::string& name, const char* flag);   // ref / record / exception = True
TyP record_ty(const std::string& name, bool ref);

struct ISym;
using SymP = std::shared_ptr<const ISym>;
struct ISym {
    std::string kind, name;
    std::optional<std::string> owner;
    bool stat = false;
    TyP type;
    std::vector<TyP> params;
    TyP ret;
};
SymP sym(const std::string& kind, const std::string& name, std::optional<std::string> owner, bool stat = false, TyP type = nullptr,
         std::vector<TyP> params = {}, TyP ret = nullptr);

struct IR;
using IRP = std::shared_ptr<IR>;
struct IR {
    std::string k;                       // the ir class name
    int line = 0;
    TyP type;
    SymP sym;
    std::optional<std::string> op;       // Unary / Binary / Assign
    IRP target, x, left, right, cond, then, orelse, body, init, iterable, var, arr, index, obj, value, expr, final_;
    std::vector<IRP> args, stmts, parts, params, update, catches, members, inits, resources;
    std::vector<TyP> types;              // Catch
    std::vector<py::V> typeargs;         // Call
    bool has_value = false;
    py::V lit_value;                     // Lit
    std::optional<std::string> text;     // Lit, Comment
    std::vector<std::string> mods, comments;
    std::optional<std::string> memory;   // New
    bool postfix = false;                // Unary
    // declarations
    std::string name, qname, dkind;      // ClassDecl / MethodDecl (dkind: its `kind` slot)
    TyP ret, extends;
    std::vector<std::string> imports;
    std::optional<std::string> path, package;
    // (new) where a node kind the old front end never made needs a free-form slot
    std::vector<std::pair<std::string, py::V>> extra;
};

IRP node(const char* k, int line = 0);
IRP mk_lit(py::V v, TyP t, std::optional<std::string> text = std::nullopt);
IRP mk_lit_none(TyP t);
IRP mk_pyfn(const std::string& name, std::vector<IRP> args, TyP t, IRP target = nullptr, int line = 0, std::vector<py::V> typeargs = {});
IRP mk_name(SymP s, TyP t);

// ir.walk: every node under (and including) n, depth first, in source order (child slots only)
void ir_walk(const IRP& n, const std::function<void(const IRP&)>& f);
std::vector<IRP> ir_children(const IRP& n);

py::V to_json(const IRP& n);
py::V type_json(const TyP& t);
py::V sym_json(const SymP& s);

}  // namespace pyengine
