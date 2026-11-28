// dotnet/decompiler/csharp/l2_body.hpp -- a method body as the C# builders read it in stage 14 of docs/DOTNET_DECOMPILER_PLAN.md: the
// ILAst after the transform pipeline lowered into L2 (translate/frontends/cil), each node linked to the instruction it was lowered from.
// For the instruction kinds they read from L2 (ExpressionBuilder::translate_from_l2, StatementBuilder::convert_from_l2) the builders
// read the instruction's node, its facts (instruction_facts.hpp), instead of the instruction; the instructions stay what the C# syntax
// tree is annotated with, what an operand is translated through and what a pattern of the builders is matched against, and the ILAst
// is the fallback for every kind not read from L2 yet (ILAST_TO_L2.md section 4). The ILAst is not lifted again from the tree: the
// stage 13 round trip proves the lifted ILAst the same as the one lowered (translate.cil.round_trip), so the builders keep the one
// the transforms made, and the body is held once.
//
//     L2Lane lane;                                                   // one per decompiler: what its bodies share
//     std::unique_ptr<L2Body> l2 = L2Body::make(*function, lane);    // null: the lowering refused it, and the builders read the ILAst alone
//     StatementBuilder builder(..., function, ..., l2.get());
//
// What the builders change in the ILAst (a variable's kind, type or name, a variable registered, an instruction replaced by a load or
// an address, a store inserted) they report here, and the tree changes with it: its symbols are restated, its derived state (the use
// counts the builders read, ir::TreeIndex) is computed again when next asked. The index and the map live as long as the body; the tree
// as long as its lane (the C# syntax tree's annotations point at its nodes, as they point at the ILAst's instructions, which live as long
// as the decompiler's ILArena).
//
// Not upstream code: ILSpy's builders read the ILAst only.
#pragma once

#include <array>
#include <chrono>
#include <cstddef>
#include <memory>

#include "core/arena.hpp"
#include "translate/frontends/cil/cil.hpp"
#include "translate/ir_index.hpp"

namespace cb::dotnet::decompiler::il {
class Block;
class ILFunction;
class ILInstruction;
class ILVariable;
}  // namespace cb::dotnet::decompiler::il

namespace cb::dotnet::decompiler::csharp {

namespace ir = cb::translate::ir;

// What the bodies of one decompiler share: the arena their trees live in, and the lowering, whose types, members and entities stand for
// the type system's objects and are made once for all the methods that use them. It must not outlive the type system.
class L2Lane {
public:
    L2Lane() = default;
    L2Lane(const L2Lane&) = delete;
    L2Lane& operator=(const L2Lane&) = delete;

    cb::Arena arena;
    translate::frontends::cil::Lowering lowering{arena};
    // The bodies made, the ones the lowering refused, and how many instructions the builders translated from their L2 node and how many
    // through the ILAst (the fallback), over all of them.
    std::size_t bodies = 0;
    std::size_t refused = 0;
    std::size_t from_l2 = 0;
    std::size_t from_ilast = 0;
    // The instructions translated through the ILAst, by opcode (il::OpCode's value): what is left to port.
    std::array<std::size_t, 256> from_ilast_by_op{};
    // The time the bodies took in the transform pipeline, in the lowering, and in the builders (the decompiler measures the first and
    // the last).
    std::chrono::steady_clock::duration transforms_time{};
    std::chrono::steady_clock::duration lowering_time{};
    std::chrono::steady_clock::duration builders_time{};
};

class L2Body {
public:
    // The function lowered (in the lane), its nodes linked to its instructions; null where the lowering refuses an ILAst state L2 cannot
    // hold.
    static std::unique_ptr<L2Body> make(il::ILFunction& function, L2Lane& lane);
    L2Body(const L2Body&) = delete;
    L2Body& operator=(const L2Body&) = delete;

    // The function lowered: what the builders translate.
    il::ILFunction* function() const { return function_; }
    const ir::Node* root() const { return root_; }

    // The node an instruction was lowered to (null for one made later, a Nop the lowering dropped ...), and back.
    const ir::Node* node(const il::ILInstruction* inst) const { return map_.node(inst); }
    il::ILInstruction* instruction(const ir::Node* node) const { return map_.instruction(node); }
    il::ILVariable* variable(const ir::Sym* sym) const { return map_.variable(sym); }
    const ir::SymP* sym(const il::ILVariable* variable) const { return map_.sym(variable); }
    // The tree's derived state: parents, uses, jumps.
    ir::TreeIndex& index() { return index_; }

    // The builders' rewrites of the ILAst, made on the tree too.
    void variable_changed(il::ILVariable* variable);                        // its kind, type or name
    void variable_registered(il::ILFunction* function, il::ILVariable* variable);
    void replaced(il::ILInstruction* old, il::ILInstruction* now);           // after old->replace_with(now): `now` a load or an address
    void inserted(il::Block* block, int at);                                // after block->instructions.insert(at, store)

    // Counts an instruction the builders translated from its L2 node, or through the ILAst.
    void count(bool read_from_l2, const il::ILInstruction& inst);

private:
    explicit L2Body(L2Lane& lane) : lane_(lane) {}
    // The nodes of an instruction made by the builders (a load, a store, an address of a variable), linked to it.
    ir::Node* lower_new(il::ILInstruction* inst);

    L2Lane& lane_;
    translate::frontends::cil::LiftMap map_;
    ir::Node* root_ = nullptr;
    il::ILFunction* function_ = nullptr;
    ir::TreeIndex index_{nullptr};
};

}  // namespace cb::dotnet::decompiler::csharp
