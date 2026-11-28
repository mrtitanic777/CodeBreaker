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
#include <map>
#include <optional>
#include <source_location>
#include <string>
#include <utility>
#include <vector>

#include "core/arena.hpp"
#include "dotnet/decompiler/csharp/instruction_facts.hpp"
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
class L2Body;

// A delegate construction (DelegateConstruction::match_delegate_construction): the method, the object it binds, the delegate type, and
// whether the method pointer is a virtual one (ldvirtftn).
struct DelegateConstructionMatch {
    const typesystem::IMethod* method;
    il::ILInstruction* target;
    const typesystem::IType* delegate_type;
    bool is_virtual;

    bool operator==(const DelegateConstructionMatch&) const = default;
};

// What the C# transforms read of a variable, from its symbol and the uses ir::TreeIndex counts (L2Lane::variable).
struct VariableFacts {
    il::VariableKind kind;
    const typesystem::IType* type;
    std::optional<std::string> name;
    bool uses_initial_value;
    bool initial_value_is_initialized;
    bool is_ref_read_only;
    int load_count;
    int store_count;   // with the initial value where it is used, as ILVariable::store_count
    int address_count;
    il::BlockContainer* capture_scope;
    std::optional<int> index;
    il::StackType stack_type;

    bool is_single_definition() const { return store_count == 1 && address_count == 0; }

    bool operator==(const VariableFacts&) const = default;
};

// What they read of a container: the jumps into its entry block (its own entry counted), whether it is a function's body.
struct ContainerFacts {
    int entry_incoming;
    bool is_function_body;

    bool operator==(const ContainerFacts&) const = default;
};

class L2Lane {
public:
    L2Lane();
    ~L2Lane();
    L2Lane(const L2Lane&) = delete;
    L2Lane& operator=(const L2Lane&) = delete;

    // The bodies the C# transforms read, after the builders: each kept for the lane's life, found by the function it is the body of.
    void keep(std::unique_ptr<L2Body> body);
    L2Body* body_of(const il::ILInstruction* inst) const;   // the body of the function the instruction is in; null for none kept

    // The facts the C# transforms read, from L2 where the object's body is kept (else from the ILAst). With CB_DN_L2_CHECK set, each is
    // read from the ILAst too and a difference counted in `check_differences` (the proof that the transforms read the same).
    VariableFacts variable(const il::ILVariable& v);
    ContainerFacts container(const il::BlockContainer& c);
    il::ILFunctionKind function_kind(const il::ILFunction& f);
    il::BlockContainer* function_body(const il::ILFunction& f);
    std::vector<il::ILVariable*> captured_variables(const il::ILFunction& f);   // in the order the function's set has them
    il::BlockContainer* enclosing_container(const il::ILInstruction& inst);       // the closest container above it (BlockContainer::find_closest_container of its parent)
    bool hides_variable_with_name(const il::ILFunction& f, const std::string& name);   // a variable or local function of it or a function around it has the name
    // A variable a C# transform registers in a function, given a symbol in its body as the builders' are; one it changes (its kind),
    // restated.
    void variable_registered(il::ILFunction* function, il::ILVariable* v);
    void variable_changed(il::ILVariable* v);
    // What an instruction is (decided from its node, as the lifting decides it): a call (call, callvirt, newobj), a user-defined or a
    // dynamic compound assignment; a call's method, a match's deconstruct method; the variables a function has, in its order; the
    // variables an instruction or its operands name (each node naming a variable; in no particular order).
    bool is_call_instruction(const il::ILInstruction& inst);
    bool is_user_defined_compound_assign(const il::ILInstruction& inst);
    bool is_dynamic_compound_assign(const il::ILInstruction& inst);
    const typesystem::IMethod* call_method(const il::CallInstruction& call);
    const typesystem::IMethod* match_method(const il::MatchInstruction& match);
    std::vector<il::ILVariable*> function_variables(const il::ILFunction& f);
    std::vector<il::ILVariable*> variables_named(il::ILInstruction& inst);
    // An address-taking of a variable used once as the `this` of a non-static call (PatternStatementTransform): the call's container
    // chain from the call up to the first container, or nothing when the variable's one address is not such a `this`.
    std::optional<std::vector<il::BlockContainer*>> single_this_call_containers(const il::ILVariable& v);
    // The same facts read from the ILAst.
    static VariableFacts ilast_variable(const il::ILVariable& v);
    static ContainerFacts ilast_container(const il::BlockContainer& c);
    static bool ilast_hides_variable_with_name(const il::ILFunction& f, const std::string& name);
    std::size_t checks = 0;
    std::size_t check_differences = 0;
    std::map<std::uint32_t, std::size_t> check_differences_at;   // by the line of l2_body.cpp that read the fact
    std::size_t facts_from_l2 = 0;
    std::size_t facts_from_ilast = 0;

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
    std::array<std::size_t, 256> without_node_by_op{};   // ... of them, the ones with no node (made after the lowering)
    // The time the bodies took in the transform pipeline, in the lowering, and in the builders (the decompiler measures the first and
    // the last).
    std::chrono::steady_clock::duration transforms_time{};
    std::chrono::steady_clock::duration lowering_time{};
    std::chrono::steady_clock::duration builders_time{};

    bool check_enabled() const { return check_; }

private:
    template <class T> T checked(T from_l2, T from_ilast, std::source_location where = std::source_location::current());
    std::vector<std::unique_ptr<L2Body>> kept_;
    translate::frontends::cil::FlatMap<const il::ILFunction*, L2Body*> by_function_;
    bool check_ = false;
};

// The same, with no lane (the transforms of a tree the builders made without L2): from the ILAst.
VariableFacts variable_of(L2Lane* l2, const il::ILVariable& v);
ContainerFacts container_of(L2Lane* l2, const il::BlockContainer& c);
il::ILFunctionKind function_kind_of(L2Lane* l2, const il::ILFunction& f);
il::BlockContainer* function_body_of(L2Lane* l2, const il::ILFunction& f);
std::vector<il::ILVariable*> captured_variables_of(L2Lane* l2, const il::ILFunction& f);
il::BlockContainer* enclosing_container_of(L2Lane* l2, const il::ILInstruction& inst);
bool hides_variable_with_name_of(L2Lane* l2, const il::ILFunction& f, const std::string& name);

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
    // ... and for an instruction the builders made, outside the function (a default value, a constant, the arguments of a call they
    // build), the node it is lowered to now; null where the lowering refuses it.
    const ir::Node* node_or_lower(il::ILInstruction* inst);
    il::ILInstruction* instruction(const ir::Node* node) const { return map_.instruction(node); }
    il::ILVariable* variable(const ir::Sym* sym) const { return map_.variable(sym); }
    const ir::SymP* sym(const il::ILVariable* variable) const { return map_.sym(variable); }
    // The instruction a child node stands for, while it is still where the node is: below `parent`, in the slot it was lowered from (a
    // rewrite of the ILAst the builders make without telling the body, as the deconstruction's replace_with, leaves the node naming an
    // instruction that moved). The wrappers a lowering adds (an ExprStmt, a switch arm's Yield) are looked through. Null otherwise.
    il::ILInstruction* operand(const ir::Node* child, const il::ILInstruction& parent) const;
    // A switch's facts from its node (a Switch or a SwitchExpr); nothing where a section or an operand is not where it was lowered.
    std::optional<SwitchFacts> switch_facts(const ir::Node& node, il::SwitchInstruction& inst) const;
    // The facts of an object of the body (null: one the tree does not have).
    std::optional<VariableFacts> variable_facts(const il::ILVariable& v);
    std::optional<ContainerFacts> container_facts(const il::BlockContainer& c);
    std::optional<il::ILFunctionKind> function_kind(const il::ILFunction& f) const;
    il::BlockContainer* function_body(const il::ILFunction& f) const;
    std::optional<std::vector<il::ILVariable*>> captured_variables(const il::ILFunction& f) const;
    std::optional<il::BlockContainer*> enclosing_container(const il::ILInstruction& inst);
    std::optional<bool> hides_variable_with_name(const il::ILFunction& f, const std::string& name);
    std::optional<std::vector<il::ILVariable*>> function_variables(const il::ILFunction& f) const;
    std::optional<std::vector<il::ILVariable*>> variables_named(il::ILInstruction& inst);
    std::optional<std::optional<std::vector<il::BlockContainer*>>> single_this_call_containers(const il::ILVariable& v);
    // The builders' patterns, matched on the tree (each as its ILAst matcher, which a CB_DN_L2_CHECK run compares it with): a newobj of a
    // delegate over ldftn/ldvirtftn; a tuple's construction (its elements, the Rest of a long one followed); a span-based string.Concat
    // (its operands and their types); `a && b` (if a then b else 0) and `a || b` (if a then 1 else b).
    std::optional<DelegateConstructionMatch> match_delegate_construction(il::NewObj& newobj);
    bool match_tuple_construction(il::NewObj& newobj, std::vector<il::ILInstruction*>* elements);
    bool match_span_string_concat(il::CallInstruction& call, std::vector<std::pair<il::ILInstruction*, typesystem::KnownTypeCode>>* operands);
    bool match_logic_and(il::IfInstruction& inst, il::ILInstruction** lhs, il::ILInstruction** rhs);
    bool match_logic_or(il::IfInstruction& inst, il::ILInstruction** lhs, il::ILInstruction** rhs);
    // The foreach without a dispose (StatementBuilder::transform_to_foreach_without_dispose): the block's statement `at` a store of a
    // variable, the next a while loop.
    std::optional<std::pair<il::StLoc*, il::BlockContainer*>> match_store_then_while(il::Block& block, int at);
    // A container's kind (its Region's), an instruction that is the ldc.i4 of `value` (a Lit of I4), the cast around a foreach's
    // get_Current (castclass or unbox.any: its type), from the nodes.
    il::ContainerKind container_kind(const il::BlockContainer& c);
    bool is_ldc_i4(const il::ILInstruction& inst, std::int32_t value);
    std::optional<std::pair<il::ILInstruction*, const typesystem::IType*>> cast_around(il::ILInstruction& inst);
    // A variable's facts as the builders read them (the ILAst's where the tree has no symbol for it); the values its stores store (null for a store that is no stloc: a pattern's, a handler's, a using's or a fixed's variable); whether an instruction is a
    // default.value, and of which type.
    VariableFacts variable(const il::ILVariable& v);
    std::vector<il::ILInstruction*> stored_values(const il::ILVariable& v);
    bool match_default_value(const il::ILInstruction& inst, const typesystem::IType** type);
    bool hides(const il::ILFunction& f, const std::string& name);   // ExpressionBuilder::hides_variable_with_name, from the tree
    const typesystem::IMethod* call_method(const il::CallInstruction& call);
    // The ILAst's matchers of the same (with no body: the builders read the ILAst alone).
    static std::optional<DelegateConstructionMatch> ilast_delegate_construction(il::NewObj& newobj);
    // The tree's derived state: parents, uses, jumps.
    ir::TreeIndex& index() { return index_; }

    // The builders' rewrites of the ILAst, made on the tree too.
    void variable_changed(il::ILVariable* variable);                        // its kind, type or name
    void variable_registered(il::ILFunction* function, il::ILVariable* variable);
    void replaced(il::ILInstruction* old, il::ILInstruction* now);           // after old->replace_with(now): `now` a load or an address
    void inserted(il::Block* block, int at);                                // after block->instructions.insert(at, store)
    void moved(il::ILInstruction* old, il::ILInstruction* now);              // before old->replace_with(now): `now` an instruction of the tree

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
