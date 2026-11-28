// translate/frontends/cil/cil.hpp -- the CIL lane: ILSpy's structured ILAst (dotnet/decompiler/il, after the transform pipeline) lowered
// into the translator's IR losslessly, and lifted back.
//
//     Arena arena;
//     cil::Lowering lowering(arena);
//     ir::Node* decl = lowering.function(*function);       // an ir::MethodDecl (a method, a local function) or an ir::Lambda
//     il::ILFunction* again = cil::Lifting().function(decl); // the same ILAst, made in the thread's current il::ILArena
//
// The design is work/ilspy-port/l2/ILAST_TO_L2.md (reviewed: ILAST_TO_L2_REVIEW.md). What it decides:
//
//   * one L2 node per ILAst instruction, in the same shape and child order (a few helper nodes: an ExprStmt around an expression in a
//     statement place, a Yield around a switch expression's arm, the Name of a variable, the Labeled/Block pair of a container's block);
//     no restructuring, so stage 14's builders, which decide on the ILAst's shape, see what they see today;
//   * a fact other languages share gets a typed slot of ir.hpp; a CIL-only residue an `Info` key (`cil.*`); a CIL-only operation an
//     `Intrinsic` (`cil.*`, `cs.dynamic.*`);
//   * type-system objects are entities (ClrEntity: the IType, IMethod, IField, IMember or IParameter itself) on the Type or Sym made from
//     them, with the name, owner and signature other lanes read filled too; one TypeP per IType, one SymP per member and per variable;
//   * what the ILAst derives (parents, use lists, incoming edge and leave counts, entry points, a block's label) is not stored: the
//     lifting rebuilds the tree and the ILAst recomputes it. A block's label is its `IL_xxxx`, made unique per function (`_2`, `_3`);
//     a Goto names the block, a Break the entry block of the container it leaves.
//
// Entities are in-process handles here: `ClrEntity::key()` is a stable text of the object, but no `ir::EntityScope` resolves it yet,
// so a lowered tree written as a document cannot be read back with its entities (stage 13's document round trip is what is left).
//
// Ported concepts from ILSpy (ICSharpCode.Decompiler.IL) are named as there; this lane itself has no upstream counterpart.
#pragma once

#include <cstddef>
#include <memory>
#include <string>

#include "translate/frontends/cil/flat_map.hpp"
#include "translate/ir.hpp"

namespace cb::dotnet::decompiler::il {
class ILFunction;
class ILInstruction;
class ILVariable;
}
namespace cb::dotnet::decompiler::typesystem {
class IType;
class IMethod;
class IField;
class IMember;
class IParameter;
}  // namespace cb::dotnet::decompiler::typesystem

namespace cb::translate::frontends::cil {

// A CLR type-system object: an IType (kind Type), IMethod (Method), IField (Field), IProperty or IEvent (Property, Event: an IMember),
// IParameter (Parameter). It holds the object only; the object belongs to the decompiler's type system (or to the ILArena of the
// function it was made for), which must outlive the tree.
class ClrEntity final : public ir::Entity {
public:
    ClrEntity(ir::EntityKind kind, const void* object) : kind_(kind), object_(object) {}
    std::string_view family() const override { return "clr"; }
    ir::EntityKind kind() const override { return kind_; }
    // "T:<reflection name>", "M:<declaring type>#<token>[<type arguments>]", "F:..", "P:..", "E:.." (a member without a token: its
    // declaring type, name and signature), "A:<owner>/<name>".
    std::string key() const override;
    const void* object() const { return object_; }

private:
    ir::EntityKind kind_;
    const void* object_;
};

// The object behind an entity, a type or a symbol made by the lowering; null for another one (or none).
const dotnet::decompiler::typesystem::IType* clr_type(const ir::EntityP& entity);
const dotnet::decompiler::typesystem::IType* clr_type(const ir::TypeP& type);
const dotnet::decompiler::typesystem::IMethod* clr_method(const ir::SymP& sym);
const dotnet::decompiler::typesystem::IMethod* clr_method(const ir::EntityP& entity);
const dotnet::decompiler::typesystem::IField* clr_field(const ir::SymP& sym);
const dotnet::decompiler::typesystem::IField* clr_field(const ir::EntityP& entity);
const dotnet::decompiler::typesystem::IMember* clr_member(const ir::SymP& sym);
const dotnet::decompiler::typesystem::IParameter* clr_parameter(const ir::SymP& sym);

class LiftMap;

struct LoweringStats {
    std::size_t functions = 0;            // ILFunctions lowered (a method and its lambdas and local functions)
    std::size_t instructions = 0;         // ILAst instructions lowered (each one node)
    std::size_t nodes = 0;                // L2 nodes made (the instructions and the helper nodes)
    std::size_t labels_uniquified = 0;    // blocks whose IL_xxxx another block of the function had already
};

// Lowers functions into one arena (or, with the arena reset between them, one function at a time). The types, the members' symbols and
// the entities it makes are kept and shared by every function it lowers: they stand for objects of the type system, which must outlive the
// Lowering. What lives only as long as one function's ILArena (its variables, a member a transform made there) is forgotten at the next
// function.
class Lowering {
public:
    explicit Lowering(Arena& arena);
    ~Lowering();
    Lowering(const Lowering&) = delete;
    Lowering& operator=(const Lowering&) = delete;

    // The function (after any part of the transform pipeline) as an L2 tree: a MethodDecl for a top-level or local function, a Lambda
    // for a delegate or an expression tree. A Lowering error (std::logic_error) names an ILAst state the L2 cannot hold. `map`, if
    // given, gets each node's instruction and each symbol's variable, as a Lifting of the tree would link them to what it makes (a
    // reader that keeps the ILAst, the C# builders, reads the tree beside it without lifting it).
    ir::Node* function(const dotnet::decompiler::il::ILFunction& function, LiftMap* map = nullptr);
    const LoweringStats& stats() const;

    // For a reader that keeps the tree current while it changes the ILAst (the C# builders, stage 14), in the function lowered last: the
    // type made from an IType; the symbol of a variable made after the lowering; a symbol stated again from its variable after the
    // variable changed (its kind, type or name: the symbol stays the same object).
    ir::TypeP type(const dotnet::decompiler::typesystem::IType* type);
    ir::SymP new_variable(const dotnet::decompiler::il::ILVariable& variable);
    // The symbol a variable of that function was lowered to; null for one it does not have.
    ir::SymP symbol_of(const dotnet::decompiler::il::ILVariable& variable) const;
    void restate(ir::Sym& sym, const dotnet::decompiler::il::ILVariable& variable);

    struct State;   // (the lowering's own; lower.cpp)

private:
    std::unique_ptr<State> s_;
};

// What a Lifting made from what (or a Lowering lowered from what), both ways: each lifted instruction's node (a Labeled for a container's block; the helper nodes a lowering
// adds, an ExprStmt or the Name a store or an address names, have no instruction of their own) and each variable's symbol. The C#
// builders read the node of the instruction they translate through it (stage 14), and a rewrite they make on the ILAst is made on the
// tree through it too. A link to a node or instruction made after the lifting is added with `link`.
class LiftMap {
public:
    const ir::Node* node(const dotnet::decompiler::il::ILInstruction* inst) const {
        const ir::Node* const* n = nodes_.find(inst);
        return n != nullptr ? *n : nullptr;
    }
    dotnet::decompiler::il::ILInstruction* instruction(const ir::Node* node) const {
        dotnet::decompiler::il::ILInstruction* const* i = instructions_.find(node);
        return i != nullptr ? *i : nullptr;
    }
    dotnet::decompiler::il::ILVariable* variable(const ir::Sym* sym) const {
        dotnet::decompiler::il::ILVariable* const* v = variables_.find(sym);
        return v != nullptr ? *v : nullptr;
    }
    const ir::SymP* sym(const dotnet::decompiler::il::ILVariable* v) const { return syms_.find(v); }

    void link(const ir::Node* node, dotnet::decompiler::il::ILInstruction* inst) {
        nodes_.get(inst).value = node;
        instructions_.get(node).value = inst;
    }
    void link(const ir::SymP& sym, dotnet::decompiler::il::ILVariable* v) {
        syms_.get(v).value = sym;
        variables_.get(sym.get()).value = v;
    }

private:
    FlatMap<const dotnet::decompiler::il::ILInstruction*, const ir::Node*> nodes_;
    FlatMap<const ir::Node*, dotnet::decompiler::il::ILInstruction*> instructions_;
    FlatMap<const ir::Sym*, dotnet::decompiler::il::ILVariable*> variables_;
    FlatMap<const dotnet::decompiler::il::ILVariable*, ir::SymP> syms_;
};

// Lifts what a Lowering made back into the ILAst, in the thread's current il::ILArena: the function is connected (its variables' load,
// store and address lists, the blocks' incoming edge counts and the containers' leave counts are current), ready for the C# builders.
// A tree that is not a lowered function is a std::logic_error naming the node.
class Lifting {
public:
    Lifting();
    ~Lifting();
    Lifting(const Lifting&) = delete;
    Lifting& operator=(const Lifting&) = delete;

    // `map`, if given, gets what was made from what.
    dotnet::decompiler::il::ILFunction* function(const ir::Node* root, LiftMap* map = nullptr);

    struct State;   // (the lifting's own; lift.cpp)

private:
    std::unique_ptr<State> s_;
};

}  // namespace cb::translate::frontends::cil
