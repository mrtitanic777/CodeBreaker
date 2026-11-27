// dotnet/decompiler/il/control_flow_instructions.hpp -- the ILAst's structure and control flow: functions, block containers and blocks,
// branch and leave, if, null coalescing, switch, try/catch/finally/fault, lock, using, pinned regions, yield return, await, and the
// nop and invalid instructions.
//
//     auto* container = make<BlockContainer>();
//     auto* block = make<Block>();
//     container->blocks.add(block);
//     block->instructions.add(make<Leave>(container));
//     container->write_to(output, ILAstWritingOptions{});
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL: Nop, NopKind, InvalidBranch, InvalidExpression, DebugBreak, ILFunction,
// ILFunctionKind, BlockContainer, ContainerKind, Block, BlockKind, PinnedRegion, Branch, IBranchOrLeaveInstruction, Leave,
// IfInstruction, NullCoalescingInstruction, NullCoalescingKind, SwitchInstruction, SwitchSection, TryInstruction, TryCatch,
// TryCatchHandler, TryFinally, TryFault, LockInstruction, UsingInstruction, YieldReturn, Await, from Instructions.cs and
// Instructions/SimpleInstruction.cs, ILFunction.cs, BlockContainer.cs, Block.cs, Branch.cs, Leave.cs, IfInstruction.cs,
// NullCoalescingInstruction.cs, SwitchInstruction.cs, TryInstruction.cs, LockInstruction.cs, UsingInstruction.cs, Await.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
//
// Names: upstream's readonly collection fields stay public data members (`block->instructions`, `container->blocks`,
// `function->local_functions`, `switch->sections`, `try_catch->handlers`), its readonly kinds too (`block->kind`, `nop->kind`); a
// property is a getter and setter (`container->kind()`, `function->kind()`). TryFault passes OpCode::TryFault (upstream's constructor
// passes TryFinally, which only makes its opcode wrong; the visitors here dispatch on it).
// RunTransforms of ILFunction and Block are in il/transforms/il_transform.cpp. AsyncDebugInfo
// is a small stand-in until the debug information is ported (WAITS); ReducedMethod holds the nearest type
// that exists, an IMethod.
#pragma once

#include <functional>
#include <limits>
#include <memory>
#include <optional>
#include <string>
#include <unordered_set>
#include <vector>

#include "dotnet/decompiler/il/il_instruction.hpp"
#include "dotnet/decompiler/il/il_variable.hpp"
#include "dotnet/decompiler/il/instructions_base.hpp"
#include "dotnet/decompiler/typesystem/type_provider.hpp"
#include "dotnet/decompiler/util/long_set.hpp"

namespace cb::dotnet::decompiler::typesystem {
class IParameter;
}

namespace cb::dotnet::decompiler::il {

using typesystem::IParameter;

namespace transforms {
class IILTransform;
class IBlockTransform;
class ILTransformContext;
class BlockTransformContext;
}  // namespace transforms

class Block;
class BlockContainer;
class Await;
class CallInstruction;
class TryFinally;
class ILFunction;

// ---- simple instructions

enum class NopKind : std::uint8_t { Normal, Pop };

// No operation. Takes 0 arguments and returns void.
class Nop final : public SimpleInstruction {
public:
    static constexpr OpCode opcode = OpCode::Nop;
    using ILInstruction::perform_match;

    std::string comment;  // empty: none
    NopKind kind = NopKind::Normal;

    Nop() : SimpleInstruction(OpCode::Nop) {}
    StackType result_type() const override { return StackType::Void; }
    ILInstruction* clone() const override;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;

protected:
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;
};

// Represents invalid IL. Semantically, this instruction is considered to throw some kind of exception.
class InvalidBranch final : public SimpleInstruction {
public:
    static constexpr OpCode opcode = OpCode::InvalidBranch;
    using ILInstruction::perform_match;

    std::string message;  // empty: none
    StackType expected_result_type = StackType::Void;

    InvalidBranch() : SimpleInstruction(OpCode::InvalidBranch) {}
    explicit InvalidBranch(std::string message_) : SimpleInstruction(OpCode::InvalidBranch), message(std::move(message_)) {}
    StackType result_type() const override { return expected_result_type; }
    InstructionFlags direct_flags() const override {
        return InstructionFlags::MayThrow | InstructionFlags::SideEffect | InstructionFlags::EndPointUnreachable;
    }
    ILInstruction* clone() const override;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;

protected:
    InstructionFlags compute_flags() const override { return direct_flags(); }
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;
};

// Represents invalid IL. Semantically, this instruction is considered to produce some kind of value.
class InvalidExpression final : public SimpleInstruction {
public:
    static constexpr OpCode opcode = OpCode::InvalidExpression;
    using ILInstruction::perform_match;

    std::string severity = "Error";
    std::string message;  // empty: none
    StackType expected_result_type = StackType::Unknown;

    InvalidExpression() : SimpleInstruction(OpCode::InvalidExpression) {}
    explicit InvalidExpression(std::string message_) : SimpleInstruction(OpCode::InvalidExpression), message(std::move(message_)) {}
    StackType result_type() const override { return expected_result_type; }
    InstructionFlags direct_flags() const override { return InstructionFlags::MayThrow | InstructionFlags::SideEffect; }
    ILInstruction* clone() const override;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;

protected:
    InstructionFlags compute_flags() const override { return direct_flags(); }
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;
};

// Breakpoint instruction.
class DebugBreak final : public SimpleInstruction {
public:
    static constexpr OpCode opcode = OpCode::DebugBreak;
    using ILInstruction::perform_match;

    DebugBreak() : SimpleInstruction(OpCode::DebugBreak) {}
    StackType result_type() const override { return StackType::Void; }
    InstructionFlags direct_flags() const override { return InstructionFlags::SideEffect; }
    ILInstruction* clone() const override;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;

protected:
    InstructionFlags compute_flags() const override { return InstructionFlags::SideEffect; }
};

// ---- ILFunction

enum class ILFunctionKind : std::uint8_t {
    // a "top-level" function: method, accessor, constructor, destructor or operator
    TopLevelFunction,
    // a delegate or lambda expression (introduced by the DelegateConstruction and TransformExpressionTrees steps)
    Delegate,
    // an expression tree lambda (introduced by the TransformExpressionTrees step)
    ExpressionTree,
    // a C# 7.0 local function (introduced by the LocalFunctionDecompiler step)
    LocalFunction,
};

using typesystem::GenericContext;

// The async method's debug information (upstream's DebugInfo.AsyncDebugInfo; the blob writing comes with the debug info port).
struct AsyncDebugInfo {
    int catch_handler_offset = 0;
    std::vector<Await*> awaits;
    bool awaits_is_default = true;  // upstream's Awaits.IsDefault
};

// A function (a method body, a lambda, a local function): its body and variables, and the local functions declared in it.
class ILFunction final : public ILInstruction {
public:
    static constexpr OpCode opcode = OpCode::ILFunction;
    using ILInstruction::perform_match;

    static const SlotInfo body_slot;
    static const SlotInfo local_functions_slot;

    // The method definition from metadata; nullptr for functions that were not made from metadata (expression trees).
    const IMethod* method = nullptr;
    const GenericContext generic_context;
    // The name of the function, usually the metadata name. For local functions it is the name used to declare and use the function
    // (it may not conflict with the names of local variables of ancestor functions, and AssignVariableNames may change it).
    std::string name;
    // The size of the IL code (after async/await, the size of MoveNext).
    int code_size = 0;
    int local_variable_signature_length = 0;
    // The scope a local function is declared in (nullptr for others).
    BlockContainer* declaration_scope = nullptr;
    // The variables captured by this function (filled by TransformDisplayClassUsage).
    std::unordered_set<ILVariable*> captured_variables;
    std::vector<std::string> warnings;  // of the ILReader
    // Set by YieldReturnDecompiler: the function is an iterator, and 'return' means 'yield break'.
    bool is_iterator = false;
    bool state_machine_compiled_with_mono = false;
    bool state_machine_compiled_with_legacy_visual_basic = false;
    // Set by AsyncAwaitDecompiler: the T of an async method returning Task<T>; void for Task and void (nullptr: not async).
    const IType* async_return_type = nullptr;
    // The compiler-generated MoveNext of an iterator or async function.
    const IMethod* move_next_method = nullptr;
    // For a local function: the reduced version of the function (upstream's LocalFunctionMethod, an IMethod).
    const IMethod* reduced_method = nullptr;
    AsyncDebugInfo async_debug_info;
    // For a delegate or expression tree: the expression tree type Expression<T> or T, T the delegate type matching the signature.
    const IType* delegate_type = nullptr;
    const IType* return_type = nullptr;
    std::vector<const IParameter*> parameters;
    // Candidate locations for sequence points: every offset where the stack is empty, nops, and what follows a call.
    std::optional<std::vector<int>> sequence_point_candidates;
    InstructionCollection<ILFunction> local_functions;

    ILFunction(const IMethod* method, int code_size, GenericContext generic_context, ILInstruction* body,
               ILFunctionKind kind = ILFunctionKind::TopLevelFunction);
    // Only for the TransformExpressionTrees step.
    ILFunction(const IType* return_type, std::vector<const IParameter*> parameters, GenericContext generic_context, ILInstruction* body,
               ILFunctionKind kind = ILFunctionKind::TopLevelFunction);
    ILFunction(const ILFunction&) = delete;

    ILInstruction* body() const { return body_; }
    void set_body(ILInstruction* value) {
        validate_child(value);
        set_child_instruction(body_, value, 0);
    }
    ILVariableCollection& variables() { return variables_; }
    const ILVariableCollection& variables() const { return variables_; }
    bool is_async() const { return async_return_type != nullptr; }
    ILFunctionKind kind() const { return kind_; }
    // A top-level or local function's kind may not change.
    void set_kind(ILFunctionKind value);
    // The IL offset of the chained constructor call, -1 if this is not a constructor or no chained call was found.
    int chained_constructor_call_il_offset();

    // Runs the transforms in order (il/transforms/il_transform.cpp).
    void run_transforms(const std::vector<std::unique_ptr<transforms::IILTransform>>& transforms, transforms::ILTransformContext& context);
    // Makes a variable of this function (named I_n when no name is given).
    ILVariable* register_variable(VariableKind kind, const IType* type, const std::string& name = std::string());
    // Recombines split variables by replacing all occurrences of variable2 with variable1.
    void recombine_variables(ILVariable* variable1, ILVariable* variable2);

    int get_child_count() const override { return 1 + local_functions.count(); }
    ILInstruction* get_child(int index) const override;
    void set_child(int index, ILInstruction* value) override;
    const SlotInfo& get_child_slot(int index) const override;
    ILInstruction* clone() const override;
    StackType result_type() const override;
    InstructionFlags direct_flags() const override { return InstructionFlags::MayThrow | InstructionFlags::ControlFlow; }
    bool perform_match(ILInstruction* other, patterns::Match& match) override;
    void check_invariant(ILPhase phase) override;
    bool can_inline_into_slot(int child_index, ILInstruction* expression_being_moved) override;

protected:
    // Creating a lambda may throw OutOfMemoryException; no flags of the lambda body propagate.
    InstructionFlags compute_flags() const override { return InstructionFlags::MayThrow | InstructionFlags::ControlFlow; }
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

public:
    // How many variables register_variable has named I_<n> (a blank name): the next one is I_<count>. (Not upstream's API: what the CIL
    // lane carries over when it lifts a function back from L2.)
    int helper_variable_count() const { return helper_variable_count_; }
    void set_helper_variable_count(int count) { helper_variable_count_ = count; }

private:
    util::LongSet find_unused_il_ranges() const;
    [[noreturn]] void clone_variables();

    ILInstruction* body_ = nullptr;
    ILVariableCollection variables_;
    ILFunctionKind kind_;
    int ctor_call_start_ = std::numeric_limits<int>::min();
    int helper_variable_count_ = 0;
};

// ---- BlockContainer and Block

enum class ContainerKind : std::uint8_t {
    // a normal container of control-flow blocks
    Normal,
    // a while-true loop: continue is a branch to the entry point, return/break a leave
    Loop,
    // a container whose entry point is a switch instruction: goto case is a branch, break a leave
    Switch,
    // a while loop: the entry point is a block of a single if instruction (true: jump to the loop body, false: leave); continue is a
    // branch to the entry point
    While,
    // a do-while loop: the entry point is the head of the loop body; the last block is a single if (true: jump to the head, false:
    // leave); only the last block may jump to the entry point; continue is a branch to the last block
    DoWhile,
    // a for loop: like While, the last block being the increment block (the only one that may jump to the entry point)
    For,
};

// A container of IL blocks. Each block is an extended basic block (branches may only jump to the beginning of blocks) and only
// branches within this container may reference its blocks, so seen from outside the container has one entry point (but possibly
// several exits), and so has every block within it. All blocks must perform unconditional control flow; use 'leave' to exit.
class BlockContainer final : public ILInstruction {
public:
    static constexpr OpCode opcode = OpCode::BlockContainer;
    using ILInstruction::perform_match;

    static const SlotInfo block_slot;
    InstructionCollection<Block> blocks;

    explicit BlockContainer(ContainerKind kind = ContainerKind::Normal, StackType expected_result_type = StackType::Void);
    BlockContainer(const BlockContainer&) = delete;

    ContainerKind kind() const { return kind_; }
    void set_kind(ContainerKind value) { kind_ = value; }
    StackType expected_result_type() const { return expected_result_type_; }
    void set_expected_result_type(StackType value) { expected_result_type_ = value; }
    // The number of 'leave' instructions that target this container.
    int leave_count() const { return leave_count_; }
    void set_leave_count(int value) {
        leave_count_ = value;
        invalidate_flags();
    }
    // The first block (every container must have one; transforms may rely on it being non-null).
    Block* entry_point() const { return entry_point_; }

    // Topologically sorts the blocks: the new order is returned and the container is not changed.
    std::vector<Block*> topological_sort(bool delete_unreachable_blocks = false);
    // Topologically sorts the blocks of the container.
    void sort_blocks(bool delete_unreachable_blocks = false);
    static BlockContainer* find_closest_container(ILInstruction* inst);
    static BlockContainer* find_closest_switch_container(ILInstruction* inst);
    // A block of one if instruction that leaves this container if false and branches to the body's start block if true.
    bool match_condition_block(Block* block, ILInstruction** condition, Block** body_start_block);
    bool match_increment_block(Block* block);
    // The single instruction of a container of one block with one instruction; else the block, or the container itself.
    ILInstruction* single_instruction();

    int get_child_count() const override { return blocks.count(); }
    ILInstruction* get_child(int index) const override;
    void set_child(int index, ILInstruction* value) override;
    const SlotInfo& get_child_slot(int) const override { return block_slot; }
    ILInstruction* clone() const override;
    StackType result_type() const override { return expected_result_type_; }
    InstructionFlags direct_flags() const override { return InstructionFlags::ControlFlow; }
    bool perform_match(ILInstruction* other, patterns::Match& match) override;
    void check_invariant(ILPhase phase) override;
    void instruction_collection_update_complete() override;
    bool can_inline_into_slot(int child_index, ILInstruction* expression_being_moved) override;
    bool prepare_extract(int child_index, transforms::ExtractionContext& ctx) override;

protected:
    InstructionFlags compute_flags() const override;
    void connected() override;
    void disconnected() override;
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    void set_entry_point(Block* value);

    ContainerKind kind_;
    StackType expected_result_type_;
    int leave_count_ = 0;
    Block* entry_point_ = nullptr;
};

enum class BlockKind : std::uint8_t {
    // a block of control flow: all blocks in block containers; the final instruction is a nop
    ControlFlow,
    // `new int[] { expr1, expr2 }`
    ArrayInitializer,
    CollectionInitializer,
    ObjectInitializer,
    StackAllocInitializer,
    // the result of a property setter used inline (`Use(this.Property = value);`), built by TransformAssignment: instructions of
    // one call whose last argument is a stloc to a temporary the final instruction loads
    CallInlineAssign,
    // a call with named arguments: each instruction assigns a new local, the final instruction is the call
    CallWithNamedArgs,
    DeconstructionConversions,
    DeconstructionAssignments,
    WithInitializer,
    // string interpolation with DefaultInterpolatedStringHandler
    InterpolatedString,
};

// A block of IL instructions. When execution reaches the end of the list, the final instruction (not part of the list) runs and its
// value is the block's value; for void blocks it is usually 'nop'. Blocks are: 1) targets of branches in block containers, 2) groups
// of statements (the true instruction of an if), 3) inline blocks that evaluate to a value (array initializers).
class Block final : public ILInstruction {
public:
    static constexpr OpCode opcode = OpCode::Block;
    using ILInstruction::perform_match;

    static const SlotInfo instruction_slot;
    static const SlotInfo final_instruction_slot;

    const BlockKind kind;
    InstructionCollection<ILInstruction> instructions;

    explicit Block(BlockKind kind = BlockKind::ControlFlow);
    Block(const Block&) = delete;

    // For blocks in a container: the number of incoming control flow edges (kept current as branches are added and removed and when
    // the block becomes a container's entry point).
    int incoming_edge_count() const { return incoming_edge_count_; }
    void set_incoming_edge_count(int value) { incoming_edge_count_ = value; }
    // Executed after the instructions list; its value is the block's. Part of children(), not of `instructions`.
    ILInstruction* final_instruction() const { return final_instruction_; }
    void set_final_instruction(ILInstruction* value) {
        validate_child(value);
        set_child_instruction(final_instruction_, value, instructions.count());
    }
    // The name of the block: its IL offset (IL_xxxx).
    std::string label() const;

    // Runs the block transforms in order (il/transforms/il_transform.cpp).
    void run_transforms(const std::vector<std::unique_ptr<transforms::IBlockTransform>>& transforms, transforms::BlockTransformContext& context);
    // Deletes this block from its container (not the entry point).
    void remove();
    static ILInstruction* get_predecessor(ILInstruction* inst);
    // The single instruction of a block of one instruction; else the input.
    static ILInstruction* unwrap(ILInstruction* inst);
    static Block* find_closest_block(ILInstruction* inst);
    // The closest ancestor that is a child of a control-flow block.
    static ILInstruction* get_containing_statement(ILInstruction* inst);
    bool match_inline_assign_block(CallInstruction** call, ILInstruction** value);
    bool match_if_at_end_of_block(ILInstruction** condition, ILInstruction** true_inst, ILInstruction** false_inst);

    int get_child_count() const override { return instructions.count() + 1; }
    ILInstruction* get_child(int index) const override;
    void set_child(int index, ILInstruction* value) override;
    const SlotInfo& get_child_slot(int index) const override;
    ILInstruction* clone() const override;
    StackType result_type() const override { return final_instruction_->result_type(); }
    InstructionFlags direct_flags() const override { return InstructionFlags::None; }
    bool perform_match(ILInstruction* other, patterns::Match& match) override;
    void check_invariant(ILPhase phase) override;
    void instruction_collection_update_complete() override;
    bool can_inline_into_slot(int child_index, ILInstruction* expression_being_moved) override;

protected:
    InstructionFlags compute_flags() const override;
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    ILInstruction* final_instruction_ = nullptr;
    int incoming_edge_count_ = 0;
};

// ---- pinned region

// A region where a pinned variable is used (the first form of a fixed statement).
class PinnedRegion final : public ILInstruction, public IStoreInstruction {
public:
    static constexpr OpCode opcode = OpCode::PinnedRegion;
    using ILInstruction::perform_match;

    static const SlotInfo init_slot;
    static const SlotInfo body_slot;

    PinnedRegion(ILVariable* variable, ILInstruction* init, ILInstruction* body);
    PinnedRegion(const PinnedRegion&) = default;

    ILVariable* variable() const override { return variable_; }
    void set_variable(ILVariable* value) override;
    ILInstruction* as_instruction() override { return this; }
    ILInstruction* init() const { return init_; }
    void set_init(ILInstruction* value) {
        validate_child(value);
        set_child_instruction(init_, value, 0);
    }
    ILInstruction* body() const { return body_; }
    void set_body(ILInstruction* value) {
        validate_child(value);
        set_child_instruction(body_, value, 1);
    }

    int get_child_count() const override { return 2; }
    ILInstruction* get_child(int index) const override;
    void set_child(int index, ILInstruction* value) override;
    const SlotInfo& get_child_slot(int index) const override;
    ILInstruction* clone() const override;
    StackType result_type() const override { return StackType::Void; }
    InstructionFlags direct_flags() const override { return InstructionFlags::MayWriteLocals; }
    bool perform_match(ILInstruction* other, patterns::Match& match) override;
    void check_invariant(ILPhase phase) override;

protected:
    InstructionFlags compute_flags() const override { return InstructionFlags::MayWriteLocals | init_->flags() | body_->flags(); }
    void connected() override;
    void disconnected() override;
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    ILVariable* variable_;
    ILInstruction* init_ = nullptr;
    ILInstruction* body_ = nullptr;
};

// ---- branch and leave

// What branches and leaves have in common: the container they go to.
class IBranchOrLeaveInstruction {
public:
    virtual ~IBranchOrLeaveInstruction() = default;
    virtual BlockContainer* target_container() const = 0;
};

// Unconditional branch: `goto target;`. Jumping to the entry point of the current block container is a `continue`.
class Branch final : public SimpleInstruction, public IBranchOrLeaveInstruction {
public:
    static constexpr OpCode opcode = OpCode::Branch;
    using ILInstruction::perform_match;

    explicit Branch(int target_il_offset) : SimpleInstruction(OpCode::Branch), target_il_offset_(target_il_offset) {}
    explicit Branch(Block* target_block);
    Branch(const Branch&) = default;

    int target_il_offset() const;
    // Null only inside the ILReader; non-null once the BlockBuilder has run.
    Block* target_block() const { return target_block_; }
    void set_target_block(Block* value);
    // The container that contains the target block.
    BlockContainer* target_container() const override;
    std::string target_label() const;
    // Whether this branch executes at least one finally block before jumping to the target block.
    bool triggers_finally_block() const;
    static bool get_executes_finally_block(ILInstruction* inst, BlockContainer* container);

    StackType result_type() const override { return StackType::Void; }
    InstructionFlags direct_flags() const override { return InstructionFlags::EndPointUnreachable | InstructionFlags::MayBranch; }
    ILInstruction* clone() const override;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;
    void check_invariant(ILPhase phase) override;

protected:
    InstructionFlags compute_flags() const override { return InstructionFlags::EndPointUnreachable | InstructionFlags::MayBranch; }
    void connected() override;
    void disconnected() override;
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    int target_il_offset_ = 0;
    Block* target_block_ = nullptr;
};

// Leaves a block container (and, with a value, makes it the container's value). Phase-1 execution is a no-op.
class Leave final : public ILInstruction, public IBranchOrLeaveInstruction {
public:
    static constexpr OpCode opcode = OpCode::Leave;
    using ILInstruction::perform_match;

    static const SlotInfo value_slot;

    // (The ILReader makes leaves without a target container for 'endfinally'; BlockBuilder fills it in.) A null value is a nop.
    explicit Leave(BlockContainer* target_container, ILInstruction* value = nullptr);
    Leave(const Leave&) = default;

    ILInstruction* value() const { return value_; }
    void set_value(ILInstruction* v) {
        validate_child(v);
        set_child_instruction(value_, v, 0);
    }
    BlockContainer* target_container() const override { return target_container_; }
    void set_target_container(BlockContainer* value);
    std::string target_label() const;
    // Whether the leave leaves the whole ILFunction (its target is the function's main container): a value-less `return;` in a void
    // function, `yield break;` in an iterator. False for leaves that leave it indirectly.
    bool is_leaving_function() const;
    // Whether this leave executes at least one finally block before jumping to the end of the target container.
    bool triggers_finally_block() const;

    int get_child_count() const override { return 1; }
    ILInstruction* get_child(int index) const override;
    void set_child(int index, ILInstruction* value) override;
    const SlotInfo& get_child_slot(int index) const override;
    ILInstruction* clone() const override;
    StackType result_type() const override { return StackType::Void; }
    InstructionFlags direct_flags() const override { return InstructionFlags::MayBranch | InstructionFlags::EndPointUnreachable; }
    bool perform_match(ILInstruction* other, patterns::Match& match) override;
    void check_invariant(ILPhase phase) override;

protected:
    InstructionFlags compute_flags() const override {
        return value_->flags() | InstructionFlags::MayBranch | InstructionFlags::EndPointUnreachable;
    }
    void connected() override;
    void disconnected() override;
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    ILInstruction* value_ = nullptr;
    BlockContainer* target_container_;
};

// ---- if and null coalescing

// If statement / conditional expression: `if (condition) trueExpr else falseExpr`. The condition must be StackType::I4. Also the
// logical operators: `a || b` is `if (a) (ldc.i4 1) else (b)`, `a && b` is `if (a) (b) else (ldc.i4 0)`.
class IfInstruction final : public ILInstruction {
public:
    static constexpr OpCode opcode = OpCode::IfInstruction;
    using ILInstruction::perform_match;

    static const SlotInfo condition_slot;
    static const SlotInfo true_inst_slot;
    static const SlotInfo false_inst_slot;

    // A null false instruction is a nop.
    IfInstruction(ILInstruction* condition, ILInstruction* true_inst, ILInstruction* false_inst = nullptr);
    IfInstruction(const IfInstruction&) = default;
    static IfInstruction* logic_and(ILInstruction* lhs, ILInstruction* rhs);
    static IfInstruction* logic_or(ILInstruction* lhs, ILInstruction* rhs);
    // Whether the instruction occurs where it is compared with 0.
    static bool is_in_condition_slot(ILInstruction* inst);

    ILInstruction* condition() const { return condition_; }
    void set_condition(ILInstruction* value) {
        validate_child(value);
        set_child_instruction(condition_, value, 0);
    }
    ILInstruction* true_inst() const { return true_inst_; }
    void set_true_inst(ILInstruction* value) {
        validate_child(value);
        set_child_instruction(true_inst_, value, 1);
    }
    ILInstruction* false_inst() const { return false_inst_; }
    void set_false_inst(ILInstruction* value) {
        validate_child(value);
        set_child_instruction(false_inst_, value, 2);
    }

    int get_child_count() const override { return 3; }
    ILInstruction* get_child(int index) const override;
    void set_child(int index, ILInstruction* value) override;
    const SlotInfo& get_child_slot(int index) const override;
    ILInstruction* clone() const override;
    StackType result_type() const override;
    InstructionFlags direct_flags() const override { return InstructionFlags::ControlFlow; }
    bool perform_match(ILInstruction* other, patterns::Match& match) override;
    void check_invariant(ILPhase phase) override;

protected:
    InstructionFlags compute_flags() const override;
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    ILInstruction* condition_ = nullptr;
    ILInstruction* true_inst_ = nullptr;
    ILInstruction* false_inst_ = nullptr;
};

// Kind of null-coalescing operator: `if.notnull(valueInst, fallbackInst)` is C# `value ?? fallback`.
enum class NullCoalescingKind : std::uint8_t {
    // both are of reference type: `valueInst != null ? valueInst : fallbackInst`, valueInst evaluated once
    Ref,
    // both are Nullable<T>: `valueInst.HasValue ? valueInst : fallbackInst`
    Nullable,
    // valueInst is Nullable<T>, fallbackInst a non-nullable value type: `valueInst.HasValue ? valueInst.Value : fallbackInst`
    NullableWithValueFallback,
};

class NullCoalescingInstruction final : public ILInstruction {
public:
    static constexpr OpCode opcode = OpCode::NullCoalescingInstruction;
    using ILInstruction::perform_match;

    static const SlotInfo value_inst_slot;
    static const SlotInfo fallback_inst_slot;

    const NullCoalescingKind kind;
    StackType underlying_result_type = StackType::O;

    NullCoalescingInstruction(NullCoalescingKind kind, ILInstruction* value_inst, ILInstruction* fallback_inst);
    NullCoalescingInstruction(const NullCoalescingInstruction&) = default;

    ILInstruction* value_inst() const { return value_inst_; }
    void set_value_inst(ILInstruction* value) {
        validate_child(value);
        set_child_instruction(value_inst_, value, 0);
    }
    ILInstruction* fallback_inst() const { return fallback_inst_; }
    void set_fallback_inst(ILInstruction* value) {
        validate_child(value);
        set_child_instruction(fallback_inst_, value, 1);
    }

    int get_child_count() const override { return 2; }
    ILInstruction* get_child(int index) const override;
    void set_child(int index, ILInstruction* value) override;
    const SlotInfo& get_child_slot(int index) const override;
    ILInstruction* clone() const override;
    StackType result_type() const override { return fallback_inst_->result_type(); }
    InstructionFlags direct_flags() const override { return InstructionFlags::ControlFlow; }
    bool perform_match(ILInstruction* other, patterns::Match& match) override;
    void check_invariant(ILPhase phase) override;

protected:
    InstructionFlags compute_flags() const override;
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    ILInstruction* value_inst_ = nullptr;
    ILInstruction* fallback_inst_ = nullptr;
};

// ---- switch

class SwitchSection;

// Generalization of IL switch-case: like a VB switch over integers, labels may be integer ranges (as long; an I4 or I value is
// sign-extended to I8).
class SwitchInstruction final : public ILInstruction {
public:
    static constexpr OpCode opcode = OpCode::SwitchInstruction;
    using ILInstruction::perform_match;

    static const SlotInfo value_slot;
    static const SlotInfo section_slot;

    // If lifted, the value is a Nullable<T> of an integral T and the section with has_null_label() runs for null.
    bool is_lifted = false;
    // Type information for the value, set by ILInlining to keep stack information that would be lost.
    const IType* type = nullptr;
    InstructionCollection<SwitchSection> sections;

    explicit SwitchInstruction(ILInstruction* value);
    SwitchInstruction(const SwitchInstruction&) = delete;

    ILInstruction* value() const { return value_; }
    void set_value(ILInstruction* v) {
        validate_child(v);
        set_child_instruction(value_, v, 0);
    }
    void set_result_type(StackType result_type) { result_type_ = result_type; }
    // The section with the most labels.
    SwitchSection* get_default_section();

    int get_child_count() const override { return 1 + sections.count(); }
    ILInstruction* get_child(int index) const override;
    void set_child(int index, ILInstruction* value) override;
    const SlotInfo& get_child_slot(int index) const override;
    ILInstruction* clone() const override;
    StackType result_type() const override { return result_type_; }
    InstructionFlags direct_flags() const override { return InstructionFlags::ControlFlow; }
    bool perform_match(ILInstruction* other, patterns::Match& match) override;
    void check_invariant(ILPhase phase) override;

protected:
    InstructionFlags compute_flags() const override;
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    ILInstruction* value_ = nullptr;
    StackType result_type_ = StackType::Void;
};

// Switch section within a switch statement.
class SwitchSection final : public ILInstruction {
public:
    static constexpr OpCode opcode = OpCode::SwitchSection;
    using ILInstruction::perform_match;

    static const SlotInfo body_slot;

    SwitchSection();
    SwitchSection(const SwitchSection&) = default;

    ILInstruction* body() const { return body_; }
    void set_body(ILInstruction* value) {
        validate_child(value);
        set_child_instruction(body_, value, 0);
    }
    // Serves as 'case null' in a lifted switch.
    bool has_null_label() const { return has_null_label_; }
    void set_has_null_label(bool value) { has_null_label_ = value; }
    // Only holds a compiler-generated throw helper of a switch expression; invisible in the decompiled source.
    bool is_compiler_generated_default_section() const { return is_compiler_generated_default_section_; }
    void set_is_compiler_generated_default_section(bool value) { is_compiler_generated_default_section_ = value; }
    // The labels that jump to this section.
    const util::LongSet& labels() const { return labels_; }
    void set_labels(util::LongSet value) { labels_ = std::move(value); }

    int get_child_count() const override { return 1; }
    ILInstruction* get_child(int index) const override;
    void set_child(int index, ILInstruction* value) override;
    const SlotInfo& get_child_slot(int index) const override;
    ILInstruction* clone() const override;
    StackType result_type() const override { return StackType::Void; }
    InstructionFlags direct_flags() const override { return InstructionFlags::None; }
    bool perform_match(ILInstruction* other, patterns::Match& match) override;

protected:
    InstructionFlags compute_flags() const override { return body_->flags(); }
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    ILInstruction* body_ = nullptr;
    bool has_null_label_ = false;
    bool is_compiler_generated_default_section_ = false;
    util::LongSet labels_;
};

// ---- try

class TryInstruction : public ILInstruction {
public:
    static const SlotInfo try_block_slot;

    ILInstruction* try_block() const { return try_block_; }
    void set_try_block(ILInstruction* value) {
        validate_child(value);
        set_child_instruction(try_block_, value, 0);
    }

protected:
    TryInstruction(OpCode op_code, ILInstruction* try_block) : ILInstruction(op_code) { set_try_block(try_block); }

private:
    ILInstruction* try_block_ = nullptr;
};

class TryCatchHandler;

// Try-catch statement. The value of the try and catch blocks is ignored: it always returns void.
class TryCatch final : public TryInstruction {
public:
    static constexpr OpCode opcode = OpCode::TryCatch;
    using ILInstruction::perform_match;

    static const SlotInfo handler_slot;
    InstructionCollection<TryCatchHandler> handlers;

    explicit TryCatch(ILInstruction* try_block);
    TryCatch(const TryCatch&) = delete;

    int get_child_count() const override { return 1 + handlers.count(); }
    ILInstruction* get_child(int index) const override;
    void set_child(int index, ILInstruction* value) override;
    const SlotInfo& get_child_slot(int index) const override;
    ILInstruction* clone() const override;
    StackType result_type() const override { return StackType::Void; }
    InstructionFlags direct_flags() const override { return InstructionFlags::ControlFlow; }
    bool perform_match(ILInstruction* other, patterns::Match& match) override;

protected:
    InstructionFlags compute_flags() const override;
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;
};

// Catch handler within a try-catch statement. When an exception occurs in the try block of the parent try.catch, the runtime
// searches the nearest enclosing handler with a matching variable type, assigns the exception to the variable and executes the
// filter; if the filter evaluates to 0 the exception is not caught, if 1 the stack is unwound, the exception caught and the body run.
class TryCatchHandler final : public ILInstruction, public IStoreInstruction {
public:
    static constexpr OpCode opcode = OpCode::TryCatchHandler;
    using ILInstruction::perform_match;

    static const SlotInfo filter_slot;
    static const SlotInfo body_slot;

    TryCatchHandler(ILInstruction* filter, ILInstruction* body, ILVariable* variable);
    TryCatchHandler(const TryCatchHandler&) = default;

    ILVariable* variable() const override { return variable_; }
    void set_variable(ILVariable* value) override;
    ILInstruction* as_instruction() override { return this; }
    ILInstruction* filter() const { return filter_; }
    void set_filter(ILInstruction* value) {
        validate_child(value);
        set_child_instruction(filter_, value, 0);
    }
    ILInstruction* body() const { return body_; }
    void set_body(ILInstruction* value) {
        validate_child(value);
        set_child_instruction(body_, value, 1);
    }
    // The IL range of the instructions at the start of the catch block that store the exception in its variable (empty when the
    // filter is not empty).
    Interval exception_specifier_il_range() const { return exception_specifier_il_range_; }
    void add_exception_specifier_il_range(Interval new_range) {
        exception_specifier_il_range_ = combine_il_range(exception_specifier_il_range_, new_range);
    }

    int get_child_count() const override { return 2; }
    ILInstruction* get_child(int index) const override;
    void set_child(int index, ILInstruction* value) override;
    const SlotInfo& get_child_slot(int index) const override;
    ILInstruction* clone() const override;
    StackType result_type() const override { return StackType::Void; }
    // the body is not evaluated if the filter returns 0
    InstructionFlags direct_flags() const override { return InstructionFlags::ControlFlow | InstructionFlags::MayWriteLocals; }
    bool perform_match(ILInstruction* other, patterns::Match& match) override;
    void check_invariant(ILPhase phase) override;

protected:
    InstructionFlags compute_flags() const override {
        return filter_->flags() | body_->flags() | InstructionFlags::ControlFlow | InstructionFlags::MayWriteLocals;
    }
    void connected() override;
    void disconnected() override;
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    ILVariable* variable_;
    ILInstruction* filter_ = nullptr;
    ILInstruction* body_ = nullptr;
    Interval exception_specifier_il_range_;
};

// Try-finally statement.
class TryFinally final : public TryInstruction {
public:
    static constexpr OpCode opcode = OpCode::TryFinally;
    using ILInstruction::perform_match;

    static const SlotInfo finally_block_slot;

    TryFinally(ILInstruction* try_block, ILInstruction* finally_block);
    TryFinally(const TryFinally&) = default;

    ILInstruction* finally_block() const { return finally_block_; }
    void set_finally_block(ILInstruction* value) {
        validate_child(value);
        set_child_instruction(finally_block_, value, 1);
    }

    int get_child_count() const override { return 2; }
    ILInstruction* get_child(int index) const override;
    void set_child(int index, ILInstruction* value) override;
    const SlotInfo& get_child_slot(int index) const override;
    ILInstruction* clone() const override;
    StackType result_type() const override { return try_block()->result_type(); }
    InstructionFlags direct_flags() const override { return InstructionFlags::ControlFlow; }
    bool perform_match(ILInstruction* other, patterns::Match& match) override;

protected:
    // if the end point of either the try or the finally is unreachable, so is the try-finally's
    InstructionFlags compute_flags() const override { return try_block()->flags() | finally_block_->flags() | InstructionFlags::ControlFlow; }
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    ILInstruction* finally_block_ = nullptr;
};

// Try-fault statement.
class TryFault final : public TryInstruction {
public:
    static constexpr OpCode opcode = OpCode::TryFault;
    using ILInstruction::perform_match;

    static const SlotInfo fault_block_slot;

    TryFault(ILInstruction* try_block, ILInstruction* fault_block);
    TryFault(const TryFault&) = default;

    ILInstruction* fault_block() const { return fault_block_; }
    void set_fault_block(ILInstruction* value) {
        validate_child(value);
        set_child_instruction(fault_block_, value, 1);
    }

    int get_child_count() const override { return 2; }
    ILInstruction* get_child(int index) const override;
    void set_child(int index, ILInstruction* value) override;
    const SlotInfo& get_child_slot(int index) const override;
    ILInstruction* clone() const override;
    StackType result_type() const override { return try_block()->result_type(); }
    InstructionFlags direct_flags() const override { return InstructionFlags::ControlFlow; }
    bool perform_match(ILInstruction* other, patterns::Match& match) override;

protected:
    // the end point of the try-fault is unreachable iff the try's is
    InstructionFlags compute_flags() const override {
        return try_block()->flags() | (fault_block_->flags() & ~InstructionFlags::EndPointUnreachable) | InstructionFlags::ControlFlow;
    }
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    ILInstruction* fault_block_ = nullptr;
};

// ---- lock and using

// Lock statement.
class LockInstruction final : public ILInstruction {
public:
    static constexpr OpCode opcode = OpCode::LockInstruction;
    using ILInstruction::perform_match;

    static const SlotInfo on_expression_slot;
    static const SlotInfo body_slot;

    LockInstruction(ILInstruction* on_expression, ILInstruction* body);
    LockInstruction(const LockInstruction&) = default;

    ILInstruction* on_expression() const { return on_expression_; }
    void set_on_expression(ILInstruction* value) {
        validate_child(value);
        set_child_instruction(on_expression_, value, 0);
    }
    ILInstruction* body() const { return body_; }
    void set_body(ILInstruction* value) {
        validate_child(value);
        set_child_instruction(body_, value, 1);
    }

    int get_child_count() const override { return 2; }
    ILInstruction* get_child(int index) const override;
    void set_child(int index, ILInstruction* value) override;
    const SlotInfo& get_child_slot(int index) const override;
    ILInstruction* clone() const override;
    StackType result_type() const override { return StackType::Void; }
    InstructionFlags direct_flags() const override { return InstructionFlags::ControlFlow | InstructionFlags::SideEffect; }
    bool perform_match(ILInstruction* other, patterns::Match& match) override;
    void check_invariant(ILPhase phase) override;

protected:
    InstructionFlags compute_flags() const override {
        return on_expression_->flags() | body_->flags() | InstructionFlags::ControlFlow | InstructionFlags::SideEffect;
    }
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    ILInstruction* on_expression_ = nullptr;
    ILInstruction* body_ = nullptr;
};

// Using statement: `stloc v(resourceExpression); try { body } finally { v?.Dispose(); }`. The value of v is undefined after the body.
class UsingInstruction final : public ILInstruction, public IStoreInstruction {
public:
    static constexpr OpCode opcode = OpCode::UsingInstruction;
    using ILInstruction::perform_match;

    static const SlotInfo resource_expression_slot;
    static const SlotInfo body_slot;

    UsingInstruction(ILVariable* variable, ILInstruction* resource_expression, ILInstruction* body);
    UsingInstruction(const UsingInstruction&) = default;

    ILVariable* variable() const override { return variable_; }
    void set_variable(ILVariable* value) override;
    ILInstruction* as_instruction() override { return this; }
    ILInstruction* resource_expression() const { return resource_expression_; }
    void set_resource_expression(ILInstruction* value) {
        validate_child(value);
        set_child_instruction(resource_expression_, value, 0);
    }
    ILInstruction* body() const { return body_; }
    void set_body(ILInstruction* value) {
        validate_child(value);
        set_child_instruction(body_, value, 1);
    }
    bool is_async() const { return is_async_; }
    void set_is_async(bool value) { is_async_ = value; }
    bool is_ref_struct() const { return is_ref_struct_; }
    void set_is_ref_struct(bool value) { is_ref_struct_ = value; }

    int get_child_count() const override { return 2; }
    ILInstruction* get_child(int index) const override;
    void set_child(int index, ILInstruction* value) override;
    const SlotInfo& get_child_slot(int index) const override;
    ILInstruction* clone() const override;
    StackType result_type() const override { return StackType::Void; }
    InstructionFlags direct_flags() const override {
        return InstructionFlags::MayWriteLocals | InstructionFlags::ControlFlow | InstructionFlags::SideEffect;
    }
    bool perform_match(ILInstruction* other, patterns::Match& match) override;
    void check_invariant(ILPhase phase) override;

protected:
    InstructionFlags compute_flags() const override {
        return InstructionFlags::MayWriteLocals | resource_expression_->flags() | body_->flags() | InstructionFlags::ControlFlow |
               InstructionFlags::SideEffect;
    }
    void connected() override;
    void disconnected() override;
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    ILVariable* variable_;
    ILInstruction* resource_expression_ = nullptr;
    ILInstruction* body_ = nullptr;
    bool is_async_ = false;
    bool is_ref_struct_ = false;
};

// ---- yield return and await

// Yield return (iterators, before the state machine is rebuilt).
class YieldReturn final : public ILInstruction {
public:
    static constexpr OpCode opcode = OpCode::YieldReturn;
    using ILInstruction::perform_match;

    static const SlotInfo value_slot;

    explicit YieldReturn(ILInstruction* value);
    YieldReturn(const YieldReturn&) = default;

    ILInstruction* value() const { return value_; }
    void set_value(ILInstruction* v) {
        validate_child(v);
        set_child_instruction(value_, v, 0);
    }

    int get_child_count() const override { return 1; }
    ILInstruction* get_child(int index) const override;
    void set_child(int index, ILInstruction* value) override;
    const SlotInfo& get_child_slot(int index) const override;
    ILInstruction* clone() const override;
    StackType result_type() const override { return StackType::Void; }
    InstructionFlags direct_flags() const override { return InstructionFlags::MayBranch | InstructionFlags::SideEffect; }
    bool perform_match(ILInstruction* other, patterns::Match& match) override;

protected:
    InstructionFlags compute_flags() const override { return InstructionFlags::MayBranch | InstructionFlags::SideEffect | value_->flags(); }
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    ILInstruction* value_ = nullptr;
};

// C# await operator.
class Await final : public ILInstruction {
public:
    static constexpr OpCode opcode = OpCode::Await;
    using ILInstruction::perform_match;

    static const SlotInfo value_slot;

    const IMethod* get_awaiter_method = nullptr;
    const IMethod* get_result_method = nullptr;

    explicit Await(ILInstruction* value);
    Await(const Await&) = default;

    ILInstruction* value() const { return value_; }
    void set_value(ILInstruction* v) {
        validate_child(v);
        set_child_instruction(value_, v, 0);
    }

    int get_child_count() const override { return 1; }
    ILInstruction* get_child(int index) const override;
    void set_child(int index, ILInstruction* value) override;
    const SlotInfo& get_child_slot(int index) const override;
    ILInstruction* clone() const override;
    StackType result_type() const override;
    InstructionFlags direct_flags() const override { return InstructionFlags::SideEffect; }
    bool perform_match(ILInstruction* other, patterns::Match& match) override;

protected:
    InstructionFlags compute_flags() const override { return InstructionFlags::SideEffect | value_->flags(); }
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    ILInstruction* value_ = nullptr;
};

}  // namespace cb::dotnet::decompiler::il
