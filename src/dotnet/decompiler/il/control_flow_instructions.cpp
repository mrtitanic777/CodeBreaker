// dotnet/decompiler/il/control_flow_instructions.cpp -- see control_flow_instructions.hpp.
#include "dotnet/decompiler/il/control_flow_instructions.hpp"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <format>
#include <stdexcept>

#include "core/error.hpp"
#include "dotnet/decompiler/disassembler/disassembler_helpers.hpp"
#include "dotnet/decompiler/il/il_type_extensions.hpp"
#include "dotnet/decompiler/il/instruction_output_il.hpp"
#include "dotnet/decompiler/il/instructions.hpp"
#include "dotnet/decompiler/il/patterns.hpp"
#include "dotnet/decompiler/il/semantic_helper.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/util/bit_set.hpp"

namespace cb::dotnet::decompiler::il {

namespace {

[[noreturn]] void index_out_of_range() { throw std::out_of_range("child index"); }


std::string block_kind_name(BlockKind kind) {
    switch (kind) {
        case BlockKind::ControlFlow: return "ControlFlow";
        case BlockKind::ArrayInitializer: return "ArrayInitializer";
        case BlockKind::CollectionInitializer: return "CollectionInitializer";
        case BlockKind::ObjectInitializer: return "ObjectInitializer";
        case BlockKind::StackAllocInitializer: return "StackAllocInitializer";
        case BlockKind::CallInlineAssign: return "CallInlineAssign";
        case BlockKind::CallWithNamedArgs: return "CallWithNamedArgs";
        case BlockKind::DeconstructionConversions: return "DeconstructionConversions";
        case BlockKind::DeconstructionAssignments: return "DeconstructionAssignments";
        case BlockKind::WithInitializer: return "WithInitializer";
        case BlockKind::InterpolatedString: return "InterpolatedString";
    }
    return std::to_string(static_cast<int>(kind));
}

void write_message(ITextOutput& output, const std::string& message) {
    if (!message.empty()) {
        output.write("(\"");
        output.write(message);
        output.write("\")");
    }
}

}  // namespace

// ---- Nop, InvalidBranch, InvalidExpression, DebugBreak

ILInstruction* Nop::clone() const { return make<Nop>(*this); }
bool Nop::perform_match(ILInstruction* other, patterns::Match&) { return as<Nop>(other) != nullptr; }

void Nop::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    if (kind != NopKind::Normal) output.write("." + std::string(kind == NopKind::Pop ? "pop" : "normal"));
    if (!comment.empty()) output.write(" // " + comment);
}

ILInstruction* InvalidBranch::clone() const { return make<InvalidBranch>(*this); }
bool InvalidBranch::perform_match(ILInstruction* other, patterns::Match&) { return as<InvalidBranch>(other) != nullptr; }
void InvalidBranch::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    write_message(output, message);
}

ILInstruction* InvalidExpression::clone() const { return make<InvalidExpression>(*this); }
bool InvalidExpression::perform_match(ILInstruction* other, patterns::Match&) { return as<InvalidExpression>(other) != nullptr; }
void InvalidExpression::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    write_message(output, message);
}

ILInstruction* DebugBreak::clone() const { return make<DebugBreak>(*this); }
bool DebugBreak::perform_match(ILInstruction* other, patterns::Match&) { return as<DebugBreak>(other) != nullptr; }

// ---- ILFunction

const SlotInfo ILFunction::body_slot{"Body"};
const SlotInfo ILFunction::local_functions_slot{"LocalFunctions"};

ILFunction::ILFunction(const IMethod* method_, int code_size_, GenericContext generic_context_, ILInstruction* body, ILFunctionKind kind)
    : ILInstruction(OpCode::ILFunction),
      method(method_),
      generic_context(std::move(generic_context_)),
      name(method_->name()),
      code_size(code_size_),
      return_type(method_->return_type()),
      parameters(method_->parameters()),
      local_functions(this, 1),
      variables_(this),
      kind_(kind) {
    set_body(body);
}

ILFunction::ILFunction(const IType* return_type_, std::vector<const IParameter*> parameters_, GenericContext generic_context_,
                       ILInstruction* body, ILFunctionKind kind)
    : ILInstruction(OpCode::ILFunction),
      generic_context(std::move(generic_context_)),
      return_type(return_type_),
      parameters(std::move(parameters_)),
      local_functions(this, 1),
      variables_(this),
      kind_(kind) {
    if (return_type_ == nullptr) throw std::invalid_argument("returnType");
    set_body(body);
}

ILInstruction* ILFunction::get_child(int index) const {
    if (index == 0) return body_;
    return local_functions[index - 1];
}

void ILFunction::set_child(int index, ILInstruction* value) {
    if (index == 0)
        set_body(value);
    else
        local_functions.set(index - 1, static_cast<ILFunction*>(value));
}

const SlotInfo& ILFunction::get_child_slot(int index) const {
    if (index == 0) return body_slot;
    return local_functions_slot;
}

void ILFunction::clone_variables() { throw UsageError("ILFunction.CloneVariables is currently not supported!"); }

ILInstruction* ILFunction::clone() const {
    // Upstream clones the body and the local functions, then fails in CloneVariables (not supported): the result is the failure.
    const_cast<ILFunction*>(this)->clone_variables();
}

StackType ILFunction::result_type() const { return delegate_type != nullptr ? get_stack_type(*delegate_type) : StackType::O; }

bool ILFunction::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<ILFunction>(other);
    return o != nullptr && body_->perform_match(o->body_, match) && patterns::ListMatch::do_match(local_functions, o->local_functions, match);
}

void ILFunction::set_kind(ILFunctionKind value) {
    if (kind_ == ILFunctionKind::TopLevelFunction || kind_ == ILFunctionKind::LocalFunction)
        throw UsageError("ILFunction.Kind of a top-level or local function may not be changed.");
    kind_ = value;
}

int ILFunction::chained_constructor_call_il_offset() {
    if (ctor_call_start_ == std::numeric_limits<int>::min()) {
        if (method == nullptr || !method->is_constructor() || method->is_static()) {
            ctor_call_start_ = -1;
        } else {
            ctor_call_start_ = -1;
            for (ILInstruction* d : descendants()) {
                auto* call = as<CallInstruction>(d);
                if (call != nullptr && as<NewObj>(call) == nullptr && call->method->is_constructor() &&
                    call->method->declaring_type()->is_reference_type() == true && as<Block>(call->parent()) != nullptr) {
                    ctor_call_start_ = d->start_il_offset();
                    break;
                }
            }
        }
    }
    return ctor_call_start_;
}

void ILFunction::check_invariant(ILPhase phase) {
#ifndef NDEBUG
    switch (kind_) {
        case ILFunctionKind::TopLevelFunction:
            assert(parent() == nullptr);
            assert(delegate_type == nullptr);
            assert(declaration_scope == nullptr);
            assert(method != nullptr);
            break;
        case ILFunctionKind::Delegate:
            assert(delegate_type != nullptr);
            assert(declaration_scope == nullptr);
            assert(!(delegate_type->full_name() == "System.Linq.Expressions.Expression" && delegate_type->type_parameter_count() == 1));
            break;
        case ILFunctionKind::ExpressionTree:
            assert(delegate_type != nullptr);
            assert(declaration_scope == nullptr);
            assert(delegate_type->full_name() == "System.Linq.Expressions.Expression" && delegate_type->type_parameter_count() == 1);
            break;
        case ILFunctionKind::LocalFunction:
            assert(as<ILFunction>(parent()) != nullptr && slot_info() == &local_functions_slot);
            assert(declaration_scope != nullptr);
            assert(delegate_type == nullptr);
            assert(method != nullptr);
            break;
    }
    for (int i = 0; i < variables_.count(); i++) {
        assert(variables_[i]->function() == this);
        assert(variables_[i]->index_in_function() == i);
        variables_[i]->check_invariant();
    }
#endif
    ILInstruction::check_invariant(phase);
}

void ILFunction::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    if (method != nullptr) {
        output.write(' ');
        il::write_to(*method, output);
    }
    switch (kind_) {
        case ILFunctionKind::ExpressionTree: output.write(".ET"); break;
        case ILFunctionKind::LocalFunction: output.write(".local"); break;
        default: break;
    }
    if (delegate_type != nullptr) {
        output.write("[");
        il::write_to(*delegate_type, output);
        output.write("]");
    }
    output.write_line(" {");
    output.indent();

    if (is_async()) output.write_line(".async");
    if (is_iterator) output.write_line(".iterator");
    if (declaration_scope != nullptr) {
        output.write("declared as " + name + " in ");
        output.write_local_reference(declaration_scope->entry_point()->label(), static_cast<const BlockContainer*>(declaration_scope));
        output.write_line();
    }

    output.mark_fold_start(std::to_string(variables_.count()) + " variable(s)", true);
    for (const ILVariable* variable : variables_) {
        variable->write_definition_to(output);
        output.write_line();
    }
    output.mark_fold_end();
    output.write_line();

    for (const std::string& warning : warnings) output.write_line("//" + warning);

    body_->write_to(output, options);
    output.write_line();

    for (ILFunction* local_function : local_functions) {
        output.write_line();
        local_function->write_to(output, options);
    }

    if (options.show_il_ranges) {
        util::LongSet unused_il_ranges = find_unused_il_ranges();
        if (!unused_il_ranges.is_empty()) {
            output.write("// Unused IL Ranges: ");
            std::string joined;
            bool first = true;
            for (const util::LongInterval& range : unused_il_ranges.intervals()) {
                if (!first) joined += ", ";
                first = false;
                joined += std::format("[{:04x}..{:04x}]", range.start, range.inclusive_end());
            }
            output.write(joined);
            output.write_line();
        }
    }

    output.unindent();
    output.write_line("}");
}

util::LongSet ILFunction::find_unused_il_ranges() const {
    std::vector<util::LongInterval> used_il_ranges;
    // Whether the instruction has an IL range that counts as used (upstream: SequencePointBuilder.HasUsableILRange).
    auto has_usable_il_range = [](const ILInstruction* inst) {
        if (inst->il_range_is_empty()) return false;
        if (inst->parent() == nullptr) return false;
        return !(is<BlockContainer>(inst) || is<Block>(inst));
    };
    auto mark_used_il_ranges = [&](auto&& self, ILInstruction* inst) -> void {
        if (has_usable_il_range(inst)) used_il_ranges.emplace_back(inst->start_il_offset(), inst->end_il_offset());
        if (!is<ILFunction>(inst))
            for (ILInstruction* child : inst->children()) self(self, child);
    };
    mark_used_il_ranges(mark_used_il_ranges, body_);
    return util::LongSet(util::LongInterval(0, code_size)).except_with(util::LongSet(used_il_ranges));
}

bool ILFunction::can_inline_into_slot(int child_index, ILInstruction* expression_being_moved) {
    // With expression trees, constants are occasionally inlined into an existing expression tree. Only completely pure constants may
    // be: a MayReadLocals effect would already be a problem, as the evaluation is delayed until the function is called.
    assert(child_index == 0);
    (void)child_index;
    return kind_ == ILFunctionKind::ExpressionTree && expression_being_moved->flags() == InstructionFlags::None;
}

ILVariable* ILFunction::register_variable(VariableKind variable_kind, const IType* type, const std::string& variable_name) {
    auto* variable = make<ILVariable>(variable_kind, type);
    std::string new_name = variable_name;
    bool blank = std::all_of(new_name.begin(), new_name.end(), [](unsigned char c) { return std::isspace(c) != 0; });
    if (blank) {
        new_name = "I_" + std::to_string(helper_variable_count_++);
        variable->has_generated_name = true;
    }
    variable->name = new_name;
    variables_.add(variable);
    return variable;
}

void ILFunction::recombine_variables(ILVariable* variable1, ILVariable* variable2) {
    if (variable1 == variable2) return;
    assert(ILVariableEqualityComparer{}(variable1, variable2));
    std::vector<LdLoc*> loads = variable2->load_instructions();
    for (LdLoc* ldloc : loads) ldloc->set_variable(variable1);
    std::vector<IStoreInstruction*> stores = variable2->store_instructions();
    for (IStoreInstruction* store : stores) store->set_variable(variable1);
    std::vector<LdLoca*> addresses = variable2->address_instructions();
    for (LdLoca* ldloca : addresses) ldloca->set_variable(variable1);
    bool ok = variables_.remove(variable2);
    assert(ok);
    (void)ok;
}

// ---- BlockContainer

const SlotInfo BlockContainer::block_slot{"Block", false, true};

BlockContainer::BlockContainer(ContainerKind kind, StackType expected_result_type)
    : ILInstruction(OpCode::BlockContainer), blocks(this, 0), kind_(kind), expected_result_type_(expected_result_type) {}

ILInstruction* BlockContainer::clone() const {
    auto* clone = make<BlockContainer>(kind_, expected_result_type_);
    clone->add_il_range(*this);
    std::vector<Block*> block_clones;
    for (Block* block : blocks) block_clones.push_back(static_cast<Block*>(block->clone()));
    clone->blocks.add_range(block_clones);
    // adjust branch instructions to point to the new container
    for (ILInstruction* inst : clone->descendants()) {
        if (auto* branch = as<Branch>(inst)) {
            if (branch->target_block() != nullptr && branch->target_block()->parent() == this)
                branch->set_target_block(clone->blocks[branch->target_block()->child_index()]);
        }
    }
    for (ILInstruction* inst : clone->descendants()) {
        if (auto* leave = as<Leave>(inst)) {
            if (leave->target_container() == this) leave->set_target_container(clone);
        }
    }
    return clone;
}

void BlockContainer::set_entry_point(Block* value) {
    if (entry_point_ != nullptr && is_connected()) entry_point_->set_incoming_edge_count(entry_point_->incoming_edge_count() - 1);
    entry_point_ = value;
    if (entry_point_ != nullptr && is_connected()) entry_point_->set_incoming_edge_count(entry_point_->incoming_edge_count() + 1);
}

void BlockContainer::instruction_collection_update_complete() {
    ILInstruction::instruction_collection_update_complete();
    set_entry_point(blocks.first_or_default());
}

void BlockContainer::connected() {
    ILInstruction::connected();
    if (entry_point_ != nullptr) entry_point_->set_incoming_edge_count(entry_point_->incoming_edge_count() + 1);
}

void BlockContainer::disconnected() {
    ILInstruction::disconnected();
    if (entry_point_ != nullptr) entry_point_->set_incoming_edge_count(entry_point_->incoming_edge_count() - 1);
}

void BlockContainer::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    output.write_local_reference("BlockContainer", static_cast<const BlockContainer*>(this), true);
    output.write(' ');
    switch (kind_) {
        case ContainerKind::Loop: output.write("(while-true) "); break;
        case ContainerKind::Switch: output.write("(switch) "); break;
        case ContainerKind::While: output.write("(while) "); break;
        case ContainerKind::DoWhile: output.write("(do-while) "); break;
        case ContainerKind::For: output.write("(for) "); break;
        default: break;
    }
    output.mark_fold_start("{...}");
    output.write_line("{");
    output.indent();
    for (Block* inst : blocks) {
        if (inst->parent() == this) {
            inst->write_to(output, options);
        } else {
            output.write("stale reference to ");
            output.write_local_reference(inst->label(), static_cast<const Block*>(inst));
        }
        output.write_line();
        output.write_line();
    }
    output.unindent();
    output.write("}");
    output.mark_fold_end();
}

ILInstruction* BlockContainer::get_child(int index) const { return blocks[index]; }

void BlockContainer::set_child(int index, ILInstruction* value) {
    if (blocks[index] != value) throw UsageError("Cannot replace blocks in BlockContainer");
}

bool BlockContainer::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<BlockContainer>(other);
    return o != nullptr && patterns::ListMatch::do_match(blocks, o->blocks, match);
}

void BlockContainer::check_invariant(ILPhase phase) {
    ILInstruction::check_invariant(phase);
#ifndef NDEBUG
    assert(blocks.count() > 0 && entry_point_ == blocks[0]);
    assert(!is_connected() || entry_point_->incoming_edge_count() >= 1);
    assert(as<ILFunction>(parent()) != nullptr || !il_range_is_empty());
    for (Block* b : blocks) {
        assert(b->has_flag(InstructionFlags::EndPointUnreachable));
        assert(b->kind == BlockKind::ControlFlow);  // this also implies that the blocks don't use FinalInstruction
    }
    assert(static_cast<int>(topological_sort(true).size()) == blocks.count() && "Container should not have any unreachable blocks");
    Block* body_start_block = nullptr;
    switch (kind_) {
        case ContainerKind::Normal: break;
        case ContainerKind::Loop: assert(entry_point_->incoming_edge_count() > 1); break;
        case ContainerKind::Switch:
            assert(entry_point_->instructions.count() == 1);
            assert(as<SwitchInstruction>(entry_point_->instructions[0]) != nullptr);
            assert(entry_point_->incoming_edge_count() == 1);
            break;
        case ContainerKind::While: {
            assert(entry_point_->incoming_edge_count() > 1);
            assert(blocks.count() >= 2);
            ILInstruction* condition = nullptr;
            bool ok = match_condition_block(entry_point_, &condition, &body_start_block);
            assert(ok);
            (void)ok;
            assert(body_start_block == blocks[1]);
            break;
        }
        case ContainerKind::DoWhile: {
            assert(entry_point_->incoming_edge_count() > 1);
            assert(blocks.count() >= 2);
            ILInstruction* condition = nullptr;
            bool ok = match_condition_block(blocks.last(), &condition, &body_start_block);
            assert(ok);
            (void)ok;
            assert(body_start_block == entry_point_);
            break;
        }
        case ContainerKind::For: {
            assert(entry_point_->incoming_edge_count() == 2);
            assert(blocks.count() >= 3);
            ILInstruction* condition = nullptr;
            bool ok = match_condition_block(entry_point_, &condition, &body_start_block);
            assert(ok);
            assert(match_increment_block(blocks.last()));
            (void)ok;
            assert(body_start_block == blocks[1]);
            break;
        }
    }
    (void)body_start_block;
#endif
}

InstructionFlags BlockContainer::compute_flags() const {
    InstructionFlags flags = InstructionFlags::ControlFlow;
    for (Block* block : blocks) flags |= block->flags();
    // the end point of the container is only reachable if there's a leave instruction
    if (leave_count_ == 0)
        flags |= InstructionFlags::EndPointUnreachable;
    else
        flags &= ~InstructionFlags::EndPointUnreachable;
    return flags;
}

bool BlockContainer::can_inline_into_slot(int child_index, ILInstruction*) {
    // Inlining into the entry point is allowed as long as no code moves into a loop (used to inline into the switch expression).
    return child_index == 0 && entry_point_->incoming_edge_count() == 1;
}

std::vector<Block*> BlockContainer::topological_sort(bool delete_unreachable_blocks) {
    // Visit the blocks in post-order (a depth-first search with successors handled in reverse order, then the order reversed).
    util::BitSet visited(blocks.count());
    std::vector<Block*> post_order;
    struct Item {
        Block* node;
        bool is_post_order_continuation;
    };
    std::vector<Item> worklist;
    worklist.push_back({entry_point_, false});
    while (!worklist.empty()) {
        Item item = worklist.back();
        if (item.is_post_order_continuation) {
            post_order.push_back(item.node);
            worklist.pop_back();
            continue;
        }
        // mark as visited
        assert(item.node->parent() == this);
        if (visited[item.node->child_index()]) {
            worklist.pop_back();
            continue;
        }
        visited.set(item.node->child_index());
        worklist.back().is_post_order_continuation = true;
        for (ILInstruction* inst : item.node->descendants()) {
            if (auto* branch = as<Branch>(inst)) {
                if (branch->target_block()->parent() == this) worklist.push_back({branch->target_block(), false});
            }
        }
    }
    std::reverse(post_order.begin(), post_order.end());
    if (!delete_unreachable_blocks) {
        for (int i = 0; i < blocks.count(); i++) {
            if (!visited[i]) post_order.push_back(blocks[i]);
        }
    }
    return post_order;
}

void BlockContainer::sort_blocks(bool delete_unreachable_blocks) {
    if (blocks.count() < 2) return;
    std::vector<Block*> new_order = topological_sort(delete_unreachable_blocks);
    assert(new_order[0] == blocks[0]);
    blocks.replace_list(new_order);
}

BlockContainer* BlockContainer::find_closest_container(ILInstruction* inst) {
    while (inst != nullptr) {
        if (auto* bc = as<BlockContainer>(inst)) return bc;
        inst = inst->parent();
    }
    return nullptr;
}

BlockContainer* BlockContainer::find_closest_switch_container(ILInstruction* inst) {
    while (inst != nullptr) {
        if (auto* bc = as<BlockContainer>(inst))
            if (bc->kind() == ContainerKind::Switch) return bc;
        inst = inst->parent();
    }
    return nullptr;
}

bool BlockContainer::match_condition_block(Block* block, ILInstruction** condition, Block** body_start_block) {
    *condition = nullptr;
    *body_start_block = nullptr;
    if (block->instructions.count() != 1) return false;
    ILInstruction* true_inst = nullptr;
    ILInstruction* false_inst = nullptr;
    if (!block->instructions[0]->match_if_instruction(condition, &true_inst, &false_inst)) return false;
    return false_inst->match_leave(static_cast<const BlockContainer*>(this)) && true_inst->match_branch(body_start_block);
}

bool BlockContainer::match_increment_block(Block* block) {
    if (block->instructions.count() == 0) return false;
    if (!block->instructions.last()->match_branch(static_cast<const Block*>(entry_point_))) return false;
    return true;
}

ILInstruction* BlockContainer::single_instruction() {
    if (blocks.count() != 1) return this;
    if (blocks[0]->instructions.count() != 1) return blocks[0];
    return blocks[0]->instructions[0];
}

// ---- Block

const SlotInfo Block::instruction_slot{"Instruction", false, true};
const SlotInfo Block::final_instruction_slot{"FinalInstruction"};

Block::Block(BlockKind kind_) : ILInstruction(OpCode::Block), kind(kind_), instructions(this, 0) { set_final_instruction(make<Nop>()); }

ILInstruction* Block::clone() const {
    auto* clone = make<Block>(kind);
    clone->add_il_range(*this);
    std::vector<ILInstruction*> instruction_clones;
    for (ILInstruction* inst : instructions) instruction_clones.push_back(inst->clone());
    clone->instructions.add_range(instruction_clones);
    clone->set_final_instruction(final_instruction_->clone());
    return clone;
}

void Block::instruction_collection_update_complete() {
    ILInstruction::instruction_collection_update_complete();
    if (final_instruction_->parent() == this) final_instruction_->set_child_index(instructions.count());
}

std::string Block::label() const { return disassembler::offset_to_string(start_il_offset()); }

ILInstruction* Block::get_child(int index) const {
    if (index == instructions.count()) return final_instruction_;
    return instructions[index];
}

void Block::set_child(int index, ILInstruction* value) {
    if (index == instructions.count())
        set_final_instruction(value);
    else
        instructions.set(index, value);
}

const SlotInfo& Block::get_child_slot(int index) const {
    if (index == instructions.count()) return final_instruction_slot;
    return instruction_slot;
}

InstructionFlags Block::compute_flags() const {
    InstructionFlags flags = InstructionFlags::None;
    for (ILInstruction* inst : instructions) flags |= inst->flags();
    flags |= final_instruction_->flags();
    return flags;
}

bool Block::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<Block>(other);
    return o != nullptr && kind == o->kind && patterns::ListMatch::do_match(instructions, o->instructions, match) &&
           final_instruction_->perform_match(o->final_instruction_, match);
}

bool Block::can_inline_into_slot(int child_index, ILInstruction*) {
    switch (kind) {
        case BlockKind::ControlFlow:
            if (!is<BlockContainer>(parent())) return false;
            return child_index == 0;
        case BlockKind::ArrayInitializer:
        case BlockKind::CollectionInitializer:
        case BlockKind::ObjectInitializer:
        case BlockKind::CallInlineAssign:
            // allow inlining into the first instruction of the block
            return child_index == 0;
        default:
            return false;
    }
}

void Block::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    output.write("Block ");
    output.write_local_reference(label(), static_cast<const Block*>(this), true);
    if (kind != BlockKind::ControlFlow) output.write(" (" + block_kind_name(kind) + ")");
    if (is<BlockContainer>(parent())) output.write(" (incoming: " + std::to_string(incoming_edge_count_) + ")");
    output.write(' ');
    output.mark_fold_start("{...}");
    output.write_line("{");
    output.indent();
    int index = 0;
    for (ILInstruction* inst : instructions) {
        if (options.show_child_index_in_block) {
            output.write("[" + std::to_string(index) + "] ");
            index++;
        }
        inst->write_to(output, options);
        output.write_line();
    }
    if (final_instruction_->op_code != OpCode::Nop) {
        output.write("final: ");
        final_instruction_->write_to(output, options);
        output.write_line();
    }
    output.unindent();
    output.write("}");
    output.mark_fold_end();
}

void Block::check_invariant(ILPhase phase) {
    ILInstruction::check_invariant(phase);
#ifndef NDEBUG
    for (int i = 0; i < instructions.count() - 1; i++) {
        // only the last instruction may have an unreachable end point
        assert(!instructions[i]->has_flag(InstructionFlags::EndPointUnreachable));
    }
    switch (kind) {
        case BlockKind::ControlFlow: assert(final_instruction_->op_code == OpCode::Nop); break;
        case BlockKind::CallInlineAssign: {
            CallInstruction* call = nullptr;
            ILInstruction* value = nullptr;
            assert(match_inline_assign_block(&call, &value));
            break;
        }
        case BlockKind::CallWithNamedArgs: {
            auto* call = as<CallInstruction>(final_instruction_);
            assert(call != nullptr);
            for (ILInstruction* inst : instructions) {
                auto* stloc = as<StLoc>(inst);
                assert(stloc != nullptr && "Instructions in CallWithNamedArgs must be assignments");
                assert(stloc->variable()->kind() == VariableKind::NamedArgument);
                assert(stloc->variable()->is_single_definition() && stloc->variable()->load_count() == 1);
                assert(stloc->variable()->load_instructions()[0]->parent() == final_instruction_);
            }
            if (call->is_instance_call()) {
                // special case: with instance calls, Instructions[0] must be for the this parameter
                ILVariable* v = static_cast<StLoc*>(instructions[0])->variable();
                assert(call->arguments()[0]->match_ld_loc(static_cast<const ILVariable*>(v)));
                (void)v;
            }
            break;
        }
        case BlockKind::ArrayInitializer: {
            auto* final_ld = as<LdLoc>(final_instruction_);
            assert(final_ld != nullptr && final_ld->variable()->is_single_definition() &&
                   final_ld->variable()->kind() == VariableKind::InitializerTarget);
            const IType* type = nullptr;
            ILInstruction* init = nullptr;
            assert(instructions[0]->match_st_loc(static_cast<const ILVariable*>(final_ld->variable()), &init) && init->match_new_arr(&type));
            for (int i = 1; i < instructions.count(); i++) {
                ILInstruction* target = nullptr;
                ILInstruction* stored = nullptr;
                const IType* t = nullptr;
                assert(instructions[i]->match_st_obj(&target, &stored, &t) && type != nullptr && type->equals(t));
                ILInstruction* array = nullptr;
                assert(target->match_ld_elema(&t, &array) && type->equals(t));
                ILVariable* v = nullptr;
                assert(array->match_ld_loc(&v) && v == final_ld->variable());
            }
            break;
        }
        case BlockKind::CollectionInitializer:
        case BlockKind::ObjectInitializer: {
            auto* final2 = as<LdLoc>(final_instruction_);
            assert(final2 != nullptr);
            ILVariable* init_var2 = final2->variable();
            assert(init_var2->store_count() == 1 && init_var2->kind() == VariableKind::InitializerTarget);
            ILInstruction* init2 = nullptr;
            bool condition = instructions[0]->match_st_loc(static_cast<const ILVariable*>(init_var2), &init2);
            assert(condition);
            (void)condition;
            for (int i = 1; i < instructions.count(); i++) {
                if (auto* st = as<StLoc>(instructions[i])) {
                    ILVariable* v = st->variable();
                    for (LdLoc* inst : v->load_instructions()) assert(inst->is_descendant_of(this));
                    for (LdLoca* inst : v->address_instructions()) assert(inst->is_descendant_of(this));
                    for (IStoreInstruction* inst : v->store_instructions()) assert(inst->as_instruction()->is_descendant_of(this));
                }
                // WAITS: the initializer's type (NewObj, DefaultValue, CallWithNamedArgs, record Clone, Activator.CreateInstance) and the
                // assert on AccessPathElement::get_access_path(instructions[i], type) for the other statements: their code is in a .cxx
                // (work in progress), which the default build lacks.
            }

            break;
        }
        case BlockKind::DeconstructionConversions: assert(slot_info() == &DeconstructInstruction::conversions_slot); break;
        case BlockKind::DeconstructionAssignments: assert(slot_info() == &DeconstructInstruction::assignments_slot); break;
        case BlockKind::InterpolatedString: {
            auto* final_call = as<Call>(final_instruction_);
            assert(final_call != nullptr && final_call->method->name() == "ToStringAndClear" && final_call->arguments().count() == 1);
            auto* interpol_init = as<StLoc>(instructions[0]);
            assert(interpol_init != nullptr && interpol_init->variable()->kind() == VariableKind::InitializerTarget &&
                   interpol_init->variable()->address_count() == instructions.count() && interpol_init->variable()->store_count() == 1);
            for (int i = 1; i < instructions.count(); i++) {
                auto* inst = as<Call>(instructions[i]);
                assert(inst != nullptr);
                assert(inst->arguments().count() >= 1 &&
                       inst->arguments()[0]->match_ld_loca(static_cast<const ILVariable*>(interpol_init->variable())));
            }
            break;
        }
        default: break;
    }
#endif
}

void Block::remove() {
    assert(child_index() > 0);
    auto* container = static_cast<BlockContainer*>(parent());
    assert(container->blocks[child_index()] == this);
    container->blocks.swap_remove_at(child_index());
}

ILInstruction* Block::get_predecessor(ILInstruction* inst) {
    auto* block = as<Block>(inst->parent());
    if (block != nullptr && inst->child_index() > 0) return block->instructions[inst->child_index() - 1];
    return nullptr;
}

ILInstruction* Block::unwrap(ILInstruction* inst) {
    if (auto* block = as<Block>(inst)) {
        if (block->instructions.count() == 1 && block->final_instruction_->match_nop()) return block->instructions[0];
    }
    return inst;
}

Block* Block::find_closest_block(ILInstruction* inst) {
    ILInstruction* curr = inst;
    while (curr != nullptr) {
        if (auto* b = as<Block>(curr)) return b;
        curr = curr->parent();
    }
    return nullptr;
}

ILInstruction* Block::get_containing_statement(ILInstruction* inst) {
    ILInstruction* curr = inst;
    while (curr != nullptr) {
        if (auto* parent_block = as<Block>(curr->parent()))
            if (parent_block->kind == BlockKind::ControlFlow) return curr;
        curr = curr->parent();
    }
    return nullptr;
}

bool Block::match_inline_assign_block(CallInstruction** call, ILInstruction** value) {
    *call = nullptr;
    *value = nullptr;
    if (kind != BlockKind::CallInlineAssign) return false;
    if (instructions.count() != 1) return false;
    *call = as<CallInstruction>(instructions[0]);
    if (*call == nullptr || (*call)->arguments().count() == 0) return false;
    ILVariable* tmp = nullptr;
    if (!(*call)->arguments().last()->match_st_loc(&tmp, value)) return false;
    if (!(tmp->is_single_definition() && tmp->load_count() == 1)) return false;
    return final_instruction_->match_ld_loc(static_cast<const ILVariable*>(tmp));
}

bool Block::match_if_at_end_of_block(ILInstruction** condition, ILInstruction** true_inst, ILInstruction** false_inst) {
    *condition = nullptr;
    *true_inst = nullptr;
    *false_inst = nullptr;
    if (instructions.count() < 2) return false;
    if (instructions[instructions.count() - 2]->match_if_instruction(condition, true_inst)) {
        // swap trueInst and falseInst for every logic.not in the condition
        *false_inst = instructions.last();
        ILInstruction* arg = nullptr;
        while ((*condition)->match_logic_not(&arg)) {
            *condition = arg;
            std::swap(*true_inst, *false_inst);
        }
        return true;
    }
    return false;
}

// ---- PinnedRegion

const SlotInfo PinnedRegion::init_slot{"Init", true};
const SlotInfo PinnedRegion::body_slot{"Body"};

PinnedRegion::PinnedRegion(ILVariable* variable, ILInstruction* init, ILInstruction* body)
    : ILInstruction(OpCode::PinnedRegion), variable_(variable) {
    if (variable == nullptr) throw std::invalid_argument("variable");
    set_init(init);
    set_body(body);
}

void PinnedRegion::set_variable(ILVariable* value) {
    assert(value != nullptr);
    if (is_connected()) variable_->remove_store_instruction(this);
    variable_ = value;
    if (is_connected()) variable_->add_store_instruction(this);
}

void PinnedRegion::connected() {
    ILInstruction::connected();
    variable_->add_store_instruction(this);
}

void PinnedRegion::disconnected() {
    variable_->remove_store_instruction(this);
    ILInstruction::disconnected();
}

ILInstruction* PinnedRegion::get_child(int index) const {
    switch (index) {
        case 0: return init_;
        case 1: return body_;
        default: index_out_of_range();
    }
}
void PinnedRegion::set_child(int index, ILInstruction* value) {
    switch (index) {
        case 0: return set_init(value);
        case 1: return set_body(value);
        default: index_out_of_range();
    }
}
const SlotInfo& PinnedRegion::get_child_slot(int index) const {
    switch (index) {
        case 0: return init_slot;
        case 1: return body_slot;
        default: index_out_of_range();
    }
}

ILInstruction* PinnedRegion::clone() const {
    auto* clone = make<PinnedRegion>(*this);
    clone->set_init(init_->clone());
    clone->set_body(body_->clone());
    return clone;
}

void PinnedRegion::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    output.write(' ');
    variable_->write_to(output);
    output.write('(');
    init_->write_to(output, options);
    output.write(", ");
    body_->write_to(output, options);
    output.write(')');
}

bool PinnedRegion::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<PinnedRegion>(other);
    return o != nullptr && variable_ == o->variable_ && init_->perform_match(o->init_, match) && body_->perform_match(o->body_, match);
}

void PinnedRegion::check_invariant(ILPhase phase) {
    ILInstruction::check_invariant(phase);
#ifndef NDEBUG
    assert(phase <= ILPhase::InILReader || is_descendant_of(variable_->function()));
    assert(phase <= ILPhase::InILReader || variable_->function()->variables()[variable_->index_in_function()] == variable_);
    assert(variable_->kind() == VariableKind::PinnedRegionLocal);
#endif
}

// ---- Branch

Branch::Branch(Block* target_block) : SimpleInstruction(OpCode::Branch) {
    if (target_block == nullptr) throw std::invalid_argument("targetBlock");
    target_block_ = target_block;
    target_il_offset_ = target_block->start_il_offset();
}

int Branch::target_il_offset() const { return target_block_ != nullptr ? target_block_->start_il_offset() : target_il_offset_; }

void Branch::set_target_block(Block* value) {
    if (target_block_ != nullptr && is_connected()) target_block_->set_incoming_edge_count(target_block_->incoming_edge_count() - 1);
    target_block_ = value;
    if (target_block_ != nullptr && is_connected()) target_block_->set_incoming_edge_count(target_block_->incoming_edge_count() + 1);
}

BlockContainer* Branch::target_container() const {
    return target_block_ != nullptr ? static_cast<BlockContainer*>(target_block_->parent()) : nullptr;
}

void Branch::connected() {
    SimpleInstruction::connected();
    if (target_block_ != nullptr) target_block_->set_incoming_edge_count(target_block_->incoming_edge_count() + 1);
}

void Branch::disconnected() {
    SimpleInstruction::disconnected();
    if (target_block_ != nullptr) target_block_->set_incoming_edge_count(target_block_->incoming_edge_count() - 1);
}

std::string Branch::target_label() const {
    return target_block_ != nullptr ? target_block_->label() : std::format("IL_{:04x}", target_il_offset());
}

bool Branch::triggers_finally_block() const { return get_executes_finally_block(const_cast<Branch*>(this), target_container()); }

bool Branch::get_executes_finally_block(ILInstruction* inst, BlockContainer* container) {
    for (; inst != container && inst != nullptr; inst = inst->parent()) {
        if (is<TryFinally>(inst->parent()) && inst->slot_info() == &TryInstruction::try_block_slot) return true;
    }
    return false;
}

void Branch::check_invariant(ILPhase phase) {
    SimpleInstruction::check_invariant(phase);
#ifndef NDEBUG
    if (phase > ILPhase::InILReader) {
        assert(target_block_ != nullptr && is<BlockContainer>(target_block_->parent()));
        assert(is_descendant_of(target_block_->parent()));
        assert(target_block_->parent()->get_child(target_block_->child_index()) == target_block_);
    }
#endif
}

void Branch::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    output.write(' ');
    if (target_block_ != nullptr)
        output.write_local_reference(target_label(), static_cast<const Block*>(target_block_));
    else
        output.write_local_reference(target_label(), target_il_offset());
}

ILInstruction* Branch::clone() const { return make<Branch>(*this); }

bool Branch::perform_match(ILInstruction* other, patterns::Match&) {
    auto* o = as<Branch>(other);
    return o != nullptr && target_block_ == o->target_block_;
}

// ---- Leave

const SlotInfo Leave::value_slot{"Value", true};

Leave::Leave(BlockContainer* target_container, ILInstruction* value) : ILInstruction(OpCode::Leave), target_container_(target_container) {
    set_value(value != nullptr ? value : make<Nop>());
}

void Leave::set_target_container(BlockContainer* value) {
    if (target_container_ != nullptr && is_connected()) target_container_->set_leave_count(target_container_->leave_count() - 1);
    target_container_ = value;
    if (target_container_ != nullptr && is_connected()) target_container_->set_leave_count(target_container_->leave_count() + 1);
}

void Leave::connected() {
    ILInstruction::connected();
    if (target_container_ != nullptr) target_container_->set_leave_count(target_container_->leave_count() + 1);
}

void Leave::disconnected() {
    ILInstruction::disconnected();
    if (target_container_ != nullptr) target_container_->set_leave_count(target_container_->leave_count() - 1);
}

std::string Leave::target_label() const {
    return target_container_ != nullptr && target_container_->entry_point() != nullptr ? target_container_->entry_point()->label() : std::string();
}

bool Leave::is_leaving_function() const { return target_container_ != nullptr && is<ILFunction>(target_container_->parent()); }

bool Leave::triggers_finally_block() const { return Branch::get_executes_finally_block(const_cast<Leave*>(this), target_container_); }

void Leave::check_invariant(ILPhase phase) {
    ILInstruction::check_invariant(phase);
#ifndef NDEBUG
    assert(phase <= ILPhase::InILReader || is_descendant_of(target_container_));
    assert(phase <= ILPhase::InILReader || phase == ILPhase::InAsyncAwait || value_->result_type() == target_container_->result_type());
#endif
}

void Leave::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    if (target_container_ != nullptr) {
        output.write(' ');
        output.write_local_reference(target_label(), static_cast<const BlockContainer*>(target_container_));
        output.write(" (");
        value_->write_to(output, options);
        output.write(')');
    }
}

ILInstruction* Leave::get_child(int index) const {
    if (index == 0) return value_;
    index_out_of_range();
}
void Leave::set_child(int index, ILInstruction* value) {
    if (index == 0) return set_value(value);
    index_out_of_range();
}
const SlotInfo& Leave::get_child_slot(int index) const {
    if (index == 0) return value_slot;
    index_out_of_range();
}

ILInstruction* Leave::clone() const {
    auto* clone = make<Leave>(*this);
    clone->set_value(value_->clone());
    return clone;
}

bool Leave::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<Leave>(other);
    return o != nullptr && value_->perform_match(o->value_, match) && target_container_ == o->target_container_;
}

// ---- IfInstruction

const SlotInfo IfInstruction::condition_slot{"Condition", true};
const SlotInfo IfInstruction::true_inst_slot{"TrueInst"};
const SlotInfo IfInstruction::false_inst_slot{"FalseInst"};

IfInstruction::IfInstruction(ILInstruction* condition, ILInstruction* true_inst, ILInstruction* false_inst) : ILInstruction(OpCode::IfInstruction) {
    set_condition(condition);
    set_true_inst(true_inst);
    set_false_inst(false_inst != nullptr ? false_inst : make<Nop>());
}

IfInstruction* IfInstruction::logic_and(ILInstruction* lhs, ILInstruction* rhs) { return make<IfInstruction>(lhs, rhs, make<LdcI4>(0)); }

IfInstruction* IfInstruction::logic_or(ILInstruction* lhs, ILInstruction* rhs) { return make<IfInstruction>(lhs, make<LdcI4>(1), rhs); }

void IfInstruction::check_invariant(ILPhase phase) {
    ILInstruction::check_invariant(phase);
    assert(condition_->result_type() == StackType::I4);
    assert(true_inst_->result_type() == false_inst_->result_type() || true_inst_->has_direct_flag(InstructionFlags::EndPointUnreachable) ||
           false_inst_->has_direct_flag(InstructionFlags::EndPointUnreachable));
}

StackType IfInstruction::result_type() const {
    if (true_inst_->has_direct_flag(InstructionFlags::EndPointUnreachable)) return false_inst_->result_type();
    return true_inst_->result_type();
}

InstructionFlags IfInstruction::compute_flags() const {
    return InstructionFlags::ControlFlow | condition_->flags() | SemanticHelper::combine_branches(true_inst_->flags(), false_inst_->flags());
}

void IfInstruction::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    if (options.use_logic_operation_sugar) {
        ILInstruction* lhs = nullptr;
        ILInstruction* rhs = nullptr;
        if (match_logic_and(&lhs, &rhs)) {
            output.write("logic.and(");
            lhs->write_to(output, options);
            output.write(", ");
            rhs->write_to(output, options);
            output.write(')');
            return;
        }
        if (match_logic_or(&lhs, &rhs)) {
            output.write("logic.or(");
            lhs->write_to(output, options);
            output.write(", ");
            rhs->write_to(output, options);
            output.write(')');
            return;
        }
    }
    write(output, op_code);
    output.write(" (");
    condition_->write_to(output, options);
    output.write(") ");
    true_inst_->write_to(output, options);
    if (false_inst_->op_code != OpCode::Nop) {
        output.write(" else ");
        false_inst_->write_to(output, options);
    }
}

bool IfInstruction::is_in_condition_slot(ILInstruction* inst) {
    const SlotInfo* slot = inst->slot_info();
    if (slot == &condition_slot) return true;
    if (slot == &true_inst_slot || slot == &false_inst_slot || slot == &NullCoalescingInstruction::fallback_inst_slot)
        return is_in_condition_slot(inst->parent());
    if (auto* comp = as<Comp>(inst->parent())) {
        if (comp->left() == inst && comp->right()->match_ldc_i4(0)) return true;
        if (comp->right() == inst && comp->left()->match_ldc_i4(0)) return true;
    }
    return false;
}

ILInstruction* IfInstruction::get_child(int index) const {
    switch (index) {
        case 0: return condition_;
        case 1: return true_inst_;
        case 2: return false_inst_;
        default: index_out_of_range();
    }
}
void IfInstruction::set_child(int index, ILInstruction* value) {
    switch (index) {
        case 0: return set_condition(value);
        case 1: return set_true_inst(value);
        case 2: return set_false_inst(value);
        default: index_out_of_range();
    }
}
const SlotInfo& IfInstruction::get_child_slot(int index) const {
    switch (index) {
        case 0: return condition_slot;
        case 1: return true_inst_slot;
        case 2: return false_inst_slot;
        default: index_out_of_range();
    }
}

ILInstruction* IfInstruction::clone() const {
    auto* clone = make<IfInstruction>(*this);
    clone->set_condition(condition_->clone());
    clone->set_true_inst(true_inst_->clone());
    clone->set_false_inst(false_inst_->clone());
    return clone;
}

bool IfInstruction::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<IfInstruction>(other);
    return o != nullptr && condition_->perform_match(o->condition_, match) && true_inst_->perform_match(o->true_inst_, match) &&
           false_inst_->perform_match(o->false_inst_, match);
}

// ---- NullCoalescingInstruction

const SlotInfo NullCoalescingInstruction::value_inst_slot{"ValueInst", true};
const SlotInfo NullCoalescingInstruction::fallback_inst_slot{"FallbackInst"};

NullCoalescingInstruction::NullCoalescingInstruction(NullCoalescingKind kind_, ILInstruction* value_inst, ILInstruction* fallback_inst)
    : ILInstruction(OpCode::NullCoalescingInstruction), kind(kind_) {
    set_value_inst(value_inst);
    set_fallback_inst(fallback_inst);
}

void NullCoalescingInstruction::check_invariant(ILPhase phase) {
    ILInstruction::check_invariant(phase);
    assert(value_inst_->result_type() == StackType::O);  // lhs is reference type or nullable type
    assert(fallback_inst_->result_type() == StackType::O || kind == NullCoalescingKind::NullableWithValueFallback);
    assert(result_type() == underlying_result_type || kind == NullCoalescingKind::Nullable);
}

InstructionFlags NullCoalescingInstruction::compute_flags() const {
    // valueInst is always executed; fallbackInst only sometimes
    return InstructionFlags::ControlFlow | value_inst_->flags() | SemanticHelper::combine_branches(InstructionFlags::None, fallback_inst_->flags());
}

void NullCoalescingInstruction::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    output.write("(");
    value_inst_->write_to(output, options);
    output.write(", ");
    fallback_inst_->write_to(output, options);
    output.write(")");
}

ILInstruction* NullCoalescingInstruction::get_child(int index) const {
    switch (index) {
        case 0: return value_inst_;
        case 1: return fallback_inst_;
        default: index_out_of_range();
    }
}
void NullCoalescingInstruction::set_child(int index, ILInstruction* value) {
    switch (index) {
        case 0: return set_value_inst(value);
        case 1: return set_fallback_inst(value);
        default: index_out_of_range();
    }
}
const SlotInfo& NullCoalescingInstruction::get_child_slot(int index) const {
    switch (index) {
        case 0: return value_inst_slot;
        case 1: return fallback_inst_slot;
        default: index_out_of_range();
    }
}

ILInstruction* NullCoalescingInstruction::clone() const {
    auto* clone = make<NullCoalescingInstruction>(*this);
    clone->set_value_inst(value_inst_->clone());
    clone->set_fallback_inst(fallback_inst_->clone());
    return clone;
}

bool NullCoalescingInstruction::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<NullCoalescingInstruction>(other);
    return o != nullptr && value_inst_->perform_match(o->value_inst_, match) && fallback_inst_->perform_match(o->fallback_inst_, match);
}

// ---- SwitchInstruction

const SlotInfo SwitchInstruction::value_slot{"Value", true};
const SlotInfo SwitchInstruction::section_slot{"Section", false, true};

SwitchInstruction::SwitchInstruction(ILInstruction* value) : ILInstruction(OpCode::SwitchInstruction), sections(this, 1) { set_value(value); }

InstructionFlags SwitchInstruction::compute_flags() const {
    InstructionFlags section_flags = InstructionFlags::EndPointUnreachable;  // neutral element for combine_branches
    for (SwitchSection* section : sections) section_flags = SemanticHelper::combine_branches(section_flags, section->flags());
    return value_->flags() | InstructionFlags::ControlFlow | section_flags;
}

void SwitchInstruction::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    output.write("switch");
    if (is_lifted) output.write(".lifted");
    output.write(' ');
    if (type != nullptr) il::write_to(*type, output);
    output.write('(');
    value_->write_to(output, options);
    output.write(") ");
    output.mark_fold_start("{...}");
    output.write_line("{");
    output.indent();
    for (SwitchSection* section : sections) {
        section->write_to(output, options);
        output.write_line();
    }
    output.unindent();
    output.write('}');
    output.mark_fold_end();
}

ILInstruction* SwitchInstruction::get_child(int index) const {
    if (index == 0) return value_;
    return sections[index - 1];
}
void SwitchInstruction::set_child(int index, ILInstruction* value) {
    if (index == 0)
        set_value(value);
    else
        sections.set(index - 1, static_cast<SwitchSection*>(value));
}
const SlotInfo& SwitchInstruction::get_child_slot(int index) const {
    if (index == 0) return value_slot;
    return section_slot;
}

// Upstream's clone copies only the value, the IL range and the sections (not is_lifted, type or the result type).
ILInstruction* SwitchInstruction::clone() const {
    auto* clone = make<SwitchInstruction>(value_->clone());
    clone->add_il_range(*this);
    clone->set_value(value_->clone());
    std::vector<SwitchSection*> section_clones;
    for (SwitchSection* h : sections) section_clones.push_back(static_cast<SwitchSection*>(h->clone()));
    clone->sections.add_range(section_clones);
    return clone;
}

bool SwitchInstruction::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<SwitchInstruction>(other);
    return o != nullptr && is_lifted == o->is_lifted && value_->perform_match(o->value_, match) &&
           patterns::ListMatch::do_match(sections, o->sections, match);
}

void SwitchInstruction::check_invariant(ILPhase phase) {
    ILInstruction::check_invariant(phase);
#ifndef NDEBUG
    bool expect_null_section = is_lifted;
    util::LongSet sets;
    for (SwitchSection* section : sections) {
        if (section->has_null_label()) {
            assert(expect_null_section && "Duplicate 'case null' or 'case null' in non-lifted switch.");
            expect_null_section = false;
        }
        assert(!section->labels().is_empty() || section->has_null_label());
        assert(!section->labels().overlaps(sets));
        assert(section->body()->result_type() == result_type());
        sets = sets.union_with(section->labels());
    }
    assert(sets.set_equals(util::LongSet::universe()) && "switch does not handle all possible cases");
    assert(!expect_null_section && "Lifted switch is missing 'case null'");
    assert(is_lifted ? (value_->result_type() == StackType::O) : (value_->result_type() == StackType::I4 || value_->result_type() == StackType::I8));
#endif
}

SwitchSection* SwitchInstruction::get_default_section() {
    // pick the section with the most labels as the default section
    SwitchSection* default_section = sections.first();
    for (SwitchSection* section : sections) {
        if (section->labels().count() > default_section->labels().count()) default_section = section;
    }
    return default_section;
}

// ---- SwitchSection

const SlotInfo SwitchSection::body_slot{"Body"};

SwitchSection::SwitchSection() : ILInstruction(OpCode::SwitchSection) {}

void SwitchSection::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    if (is_compiler_generated_default_section_) output.write("generated.");
    output.write_local_reference("case", static_cast<const SwitchSection*>(this), true);
    output.write(' ');
    if (has_null_label_) {
        output.write("null");
        if (!labels_.is_empty()) {
            output.write(", ");
            output.write(labels_.to_string());
        }
    } else {
        output.write(labels_.to_string());
    }
    output.write(": ");
    body_->write_to(output, options);
}

ILInstruction* SwitchSection::get_child(int index) const {
    if (index == 0) return body_;
    index_out_of_range();
}
void SwitchSection::set_child(int index, ILInstruction* value) {
    if (index == 0) return set_body(value);
    index_out_of_range();
}
const SlotInfo& SwitchSection::get_child_slot(int index) const {
    if (index == 0) return body_slot;
    index_out_of_range();
}

ILInstruction* SwitchSection::clone() const {
    auto* clone = make<SwitchSection>(*this);
    clone->set_body(body_->clone());
    return clone;
}

bool SwitchSection::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<SwitchSection>(other);
    return o != nullptr && body_->perform_match(o->body_, match) && labels_.set_equals(o->labels_) && has_null_label_ == o->has_null_label_;
}

// ---- try

const SlotInfo TryInstruction::try_block_slot{"TryBlock"};
const SlotInfo TryCatch::handler_slot{"Handler", false, true};
const SlotInfo TryCatchHandler::filter_slot{"Filter"};
const SlotInfo TryCatchHandler::body_slot{"Body"};
const SlotInfo TryFinally::finally_block_slot{"FinallyBlock"};
const SlotInfo TryFault::fault_block_slot{"FaultBlock"};

TryCatch::TryCatch(ILInstruction* try_block) : TryInstruction(OpCode::TryCatch, try_block), handlers(this, 1) {}

ILInstruction* TryCatch::clone() const {
    auto* clone = make<TryCatch>(try_block()->clone());
    clone->add_il_range(*this);
    std::vector<TryCatchHandler*> handler_clones;
    for (TryCatchHandler* h : handlers) handler_clones.push_back(static_cast<TryCatchHandler*>(h->clone()));
    clone->handlers.add_range(handler_clones);
    return clone;
}

void TryCatch::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    output.write(".try ");
    try_block()->write_to(output, options);
    for (TryCatchHandler* handler : handlers) {
        output.write(' ');
        handler->write_to(output, options);
    }
}

InstructionFlags TryCatch::compute_flags() const {
    InstructionFlags flags = try_block()->flags();
    for (TryCatchHandler* handler : handlers) flags = SemanticHelper::combine_branches(flags, handler->flags());
    return flags | InstructionFlags::ControlFlow;
}

ILInstruction* TryCatch::get_child(int index) const {
    if (index == 0) return try_block();
    return handlers[index - 1];
}
void TryCatch::set_child(int index, ILInstruction* value) {
    if (index == 0)
        set_try_block(value);
    else
        handlers.set(index - 1, static_cast<TryCatchHandler*>(value));
}
const SlotInfo& TryCatch::get_child_slot(int index) const {
    if (index == 0) return try_block_slot;
    return handler_slot;
}

bool TryCatch::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<TryCatch>(other);
    return o != nullptr && try_block()->perform_match(o->try_block(), match) && patterns::ListMatch::do_match(handlers, o->handlers, match);
}

TryCatchHandler::TryCatchHandler(ILInstruction* filter, ILInstruction* body, ILVariable* variable)
    : ILInstruction(OpCode::TryCatchHandler), variable_(variable) {
    set_filter(filter);
    set_body(body);
    if (variable == nullptr) throw std::invalid_argument("variable");
}

void TryCatchHandler::set_variable(ILVariable* value) {
    assert(value != nullptr);
    if (is_connected()) variable_->remove_store_instruction(this);
    variable_ = value;
    if (is_connected()) variable_->add_store_instruction(this);
}

void TryCatchHandler::connected() {
    ILInstruction::connected();
    variable_->add_store_instruction(this);
}

void TryCatchHandler::disconnected() {
    variable_->remove_store_instruction(this);
    ILInstruction::disconnected();
}

ILInstruction* TryCatchHandler::get_child(int index) const {
    switch (index) {
        case 0: return filter_;
        case 1: return body_;
        default: index_out_of_range();
    }
}
void TryCatchHandler::set_child(int index, ILInstruction* value) {
    switch (index) {
        case 0: return set_filter(value);
        case 1: return set_body(value);
        default: index_out_of_range();
    }
}
const SlotInfo& TryCatchHandler::get_child_slot(int index) const {
    switch (index) {
        case 0: return filter_slot;
        case 1: return body_slot;
        default: index_out_of_range();
    }
}

ILInstruction* TryCatchHandler::clone() const {
    auto* clone = make<TryCatchHandler>(*this);
    clone->set_filter(filter_->clone());
    clone->set_body(body_->clone());
    return clone;
}

void TryCatchHandler::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    output.write("catch ");
    if (variable_ != nullptr) {
        output.write_local_reference(variable_->name.value_or(""), static_cast<const ILVariable*>(variable_), true);
        output.write(" : ");
        output.write(variable_->type()->to_string());
    }
    output.write(" when (");
    filter_->write_to(output, options);
    output.write(')');
    output.write(' ');
    body_->write_to(output, options);
}

bool TryCatchHandler::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<TryCatchHandler>(other);
    return o != nullptr && filter_->perform_match(o->filter_, match) && body_->perform_match(o->body_, match) && variable_ == o->variable_;
}

void TryCatchHandler::check_invariant(ILPhase phase) {
    ILInstruction::check_invariant(phase);
#ifndef NDEBUG
    assert(is<TryCatch>(parent()));
    assert(filter_->result_type() == StackType::I4);
    assert(is_descendant_of(variable_->function()));
#endif
}

TryFinally::TryFinally(ILInstruction* try_block, ILInstruction* finally_block) : TryInstruction(OpCode::TryFinally, try_block) {
    set_finally_block(finally_block);
}

ILInstruction* TryFinally::clone() const {
    ILInstruction* try_clone = try_block()->clone();
    ILInstruction* finally_clone = finally_block_->clone();
    auto* clone = make<TryFinally>(try_clone, finally_clone);
    clone->add_il_range(*this);
    return clone;
}

void TryFinally::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    output.write(".try ");
    try_block()->write_to(output, options);
    output.write(" finally ");
    finally_block_->write_to(output, options);
}

ILInstruction* TryFinally::get_child(int index) const {
    switch (index) {
        case 0: return try_block();
        case 1: return finally_block_;
        default: index_out_of_range();
    }
}
void TryFinally::set_child(int index, ILInstruction* value) {
    switch (index) {
        case 0: return set_try_block(value);
        case 1: return set_finally_block(value);
        default: index_out_of_range();
    }
}
const SlotInfo& TryFinally::get_child_slot(int index) const {
    switch (index) {
        case 0: return try_block_slot;
        case 1: return finally_block_slot;
        default: index_out_of_range();
    }
}

bool TryFinally::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<TryFinally>(other);
    return o != nullptr && try_block()->perform_match(o->try_block(), match) && finally_block_->perform_match(o->finally_block_, match);
}

TryFault::TryFault(ILInstruction* try_block, ILInstruction* fault_block) : TryInstruction(OpCode::TryFault, try_block) {
    set_fault_block(fault_block);
}

ILInstruction* TryFault::clone() const {
    ILInstruction* try_clone = try_block()->clone();
    ILInstruction* fault_clone = fault_block_->clone();
    auto* clone = make<TryFault>(try_clone, fault_clone);
    clone->add_il_range(*this);
    return clone;
}

void TryFault::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    output.write(".try ");
    try_block()->write_to(output, options);
    output.write(" fault ");
    fault_block_->write_to(output, options);
}

ILInstruction* TryFault::get_child(int index) const {
    switch (index) {
        case 0: return try_block();
        case 1: return fault_block_;
        default: index_out_of_range();
    }
}
void TryFault::set_child(int index, ILInstruction* value) {
    switch (index) {
        case 0: return set_try_block(value);
        case 1: return set_fault_block(value);
        default: index_out_of_range();
    }
}
const SlotInfo& TryFault::get_child_slot(int index) const {
    switch (index) {
        case 0: return try_block_slot;
        case 1: return fault_block_slot;
        default: index_out_of_range();
    }
}

bool TryFault::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<TryFault>(other);
    return o != nullptr && try_block()->perform_match(o->try_block(), match) && fault_block_->perform_match(o->fault_block_, match);
}

// ---- LockInstruction

const SlotInfo LockInstruction::on_expression_slot{"OnExpression", true};
const SlotInfo LockInstruction::body_slot{"Body"};

LockInstruction::LockInstruction(ILInstruction* on_expression, ILInstruction* body) : ILInstruction(OpCode::LockInstruction) {
    set_on_expression(on_expression);
    set_body(body);
}

void LockInstruction::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    output.write("lock (");
    on_expression_->write_to(output, options);
    output.write_line(") {");
    output.indent();
    body_->write_to(output, options);
    output.unindent();
    output.write_line();
    output.write("}");
}

ILInstruction* LockInstruction::get_child(int index) const {
    switch (index) {
        case 0: return on_expression_;
        case 1: return body_;
        default: index_out_of_range();
    }
}
void LockInstruction::set_child(int index, ILInstruction* value) {
    switch (index) {
        case 0: return set_on_expression(value);
        case 1: return set_body(value);
        default: index_out_of_range();
    }
}
const SlotInfo& LockInstruction::get_child_slot(int index) const {
    switch (index) {
        case 0: return on_expression_slot;
        case 1: return body_slot;
        default: index_out_of_range();
    }
}

ILInstruction* LockInstruction::clone() const {
    auto* clone = make<LockInstruction>(*this);
    clone->set_on_expression(on_expression_->clone());
    clone->set_body(body_->clone());
    return clone;
}

bool LockInstruction::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<LockInstruction>(other);
    return o != nullptr && on_expression_->perform_match(o->on_expression_, match) && body_->perform_match(o->body_, match);
}

void LockInstruction::check_invariant(ILPhase phase) {
    ILInstruction::check_invariant(phase);
    assert(on_expression_->result_type() == StackType::O);
}

// ---- UsingInstruction

const SlotInfo UsingInstruction::resource_expression_slot{"ResourceExpression", true};
const SlotInfo UsingInstruction::body_slot{"Body"};

UsingInstruction::UsingInstruction(ILVariable* variable, ILInstruction* resource_expression, ILInstruction* body)
    : ILInstruction(OpCode::UsingInstruction), variable_(variable) {
    if (variable == nullptr) throw std::invalid_argument("variable");
    set_resource_expression(resource_expression);
    set_body(body);
}

void UsingInstruction::set_variable(ILVariable* value) {
    assert(value != nullptr);
    if (is_connected()) variable_->remove_store_instruction(this);
    variable_ = value;
    if (is_connected()) variable_->add_store_instruction(this);
}

void UsingInstruction::connected() {
    ILInstruction::connected();
    variable_->add_store_instruction(this);
}

void UsingInstruction::disconnected() {
    variable_->remove_store_instruction(this);
    ILInstruction::disconnected();
}

void UsingInstruction::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    output.write("using");
    if (is_async_) output.write(".async");
    if (is_ref_struct_) output.write(".ref");
    output.write(" (");
    variable_->write_to(output);
    output.write(" = ");
    resource_expression_->write_to(output, options);
    output.write_line(") {");
    output.indent();
    body_->write_to(output, options);
    output.unindent();
    output.write_line();
    output.write("}");
}

ILInstruction* UsingInstruction::get_child(int index) const {
    switch (index) {
        case 0: return resource_expression_;
        case 1: return body_;
        default: index_out_of_range();
    }
}
void UsingInstruction::set_child(int index, ILInstruction* value) {
    switch (index) {
        case 0: return set_resource_expression(value);
        case 1: return set_body(value);
        default: index_out_of_range();
    }
}
const SlotInfo& UsingInstruction::get_child_slot(int index) const {
    switch (index) {
        case 0: return resource_expression_slot;
        case 1: return body_slot;
        default: index_out_of_range();
    }
}

ILInstruction* UsingInstruction::clone() const {
    auto* clone = make<UsingInstruction>(*this);
    clone->set_resource_expression(resource_expression_->clone());
    clone->set_body(body_->clone());
    return clone;
}

bool UsingInstruction::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<UsingInstruction>(other);
    return o != nullptr && variable_ == o->variable_ && resource_expression_->perform_match(o->resource_expression_, match) &&
           body_->perform_match(o->body_, match);
}

void UsingInstruction::check_invariant(ILPhase phase) {
    ILInstruction::check_invariant(phase);
#ifndef NDEBUG
    assert(phase <= ILPhase::InILReader || is_descendant_of(variable_->function()));
    assert(phase <= ILPhase::InILReader || variable_->function()->variables()[variable_->index_in_function()] == variable_);
    assert(resource_expression_->result_type() == StackType::O);
#endif
}

// ---- YieldReturn and Await

const SlotInfo YieldReturn::value_slot{"Value", true};
const SlotInfo Await::value_slot{"Value", true};

YieldReturn::YieldReturn(ILInstruction* value) : ILInstruction(OpCode::YieldReturn) { set_value(value); }

ILInstruction* YieldReturn::get_child(int index) const {
    if (index == 0) return value_;
    index_out_of_range();
}
void YieldReturn::set_child(int index, ILInstruction* value) {
    if (index == 0) return set_value(value);
    index_out_of_range();
}
const SlotInfo& YieldReturn::get_child_slot(int index) const {
    if (index == 0) return value_slot;
    index_out_of_range();
}
ILInstruction* YieldReturn::clone() const {
    auto* clone = make<YieldReturn>(*this);
    clone->set_value(value_->clone());
    return clone;
}
void YieldReturn::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    output.write('(');
    value_->write_to(output, options);
    output.write(')');
}
bool YieldReturn::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<YieldReturn>(other);
    return o != nullptr && value_->perform_match(o->value_, match);
}

Await::Await(ILInstruction* value) : ILInstruction(OpCode::Await) { set_value(value); }

StackType Await::result_type() const {
    return get_result_method != nullptr ? get_stack_type(*get_result_method->return_type()) : StackType::Unknown;
}
ILInstruction* Await::get_child(int index) const {
    if (index == 0) return value_;
    index_out_of_range();
}
void Await::set_child(int index, ILInstruction* value) {
    if (index == 0) return set_value(value);
    index_out_of_range();
}
const SlotInfo& Await::get_child_slot(int index) const {
    if (index == 0) return value_slot;
    index_out_of_range();
}
ILInstruction* Await::clone() const {
    auto* clone = make<Await>(*this);
    clone->set_value(value_->clone());
    return clone;
}
void Await::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    output.write('(');
    value_->write_to(output, options);
    output.write(')');
}
bool Await::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<Await>(other);
    return o != nullptr && value_->perform_match(o->value_, match);
}

// ---- the Match* methods of ILInstruction (Instructions.cs's generated ones and PatternMatching.cs's) for these classes

bool ILInstruction::match_invalid_branch() const { return as<InvalidBranch>(this) != nullptr; }
bool ILInstruction::match_invalid_expression() const { return as<InvalidExpression>(this) != nullptr; }
bool ILInstruction::match_nop() const { return as<Nop>(this) != nullptr; }
bool ILInstruction::match_debug_break() const { return as<DebugBreak>(this) != nullptr; }

bool ILInstruction::match_pinned_region(ILVariable** variable, ILInstruction** init, ILInstruction** body) const {
    if (const auto* inst = as<PinnedRegion>(this)) {
        *variable = inst->variable();
        *init = inst->init();
        *body = inst->body();
        return true;
    }
    *variable = nullptr;
    *init = nullptr;
    *body = nullptr;
    return false;
}

bool ILInstruction::match_try_catch_handler(ILInstruction** filter, ILInstruction** body, ILVariable** variable) const {
    if (const auto* inst = as<TryCatchHandler>(this)) {
        *filter = inst->filter();
        *body = inst->body();
        *variable = inst->variable();
        return true;
    }
    *filter = nullptr;
    *body = nullptr;
    *variable = nullptr;
    return false;
}

bool ILInstruction::match_try_catch_handler(ILVariable** variable) const {
    if (const auto* inst = as<TryCatchHandler>(this)) {
        *variable = inst->variable();
        return true;
    }
    *variable = nullptr;
    return false;
}

bool ILInstruction::match_lock_instruction(ILInstruction** on_expression, ILInstruction** body) const {
    if (const auto* inst = as<LockInstruction>(this)) {
        *on_expression = inst->on_expression();
        *body = inst->body();
        return true;
    }
    *on_expression = nullptr;
    *body = nullptr;
    return false;
}

bool ILInstruction::match_using_instruction(ILVariable** variable, ILInstruction** resource_expression, ILInstruction** body) const {
    if (const auto* inst = as<UsingInstruction>(this)) {
        *variable = inst->variable();
        *resource_expression = inst->resource_expression();
        *body = inst->body();
        return true;
    }
    *variable = nullptr;
    *resource_expression = nullptr;
    *body = nullptr;
    return false;
}

bool ILInstruction::match_yield_return(ILInstruction** value) const {
    if (const auto* inst = as<YieldReturn>(this)) {
        *value = inst->value();
        return true;
    }
    *value = nullptr;
    return false;
}

bool ILInstruction::match_await(ILInstruction** value) const {
    if (const auto* inst = as<Await>(this)) {
        *value = inst->value();
        return true;
    }
    *value = nullptr;
    return false;
}

bool ILInstruction::match_return(ILInstruction** value) const {
    const auto* inst = as<Leave>(this);
    if (inst != nullptr && inst->is_leaving_function()) {
        *value = inst->value();
        return true;
    }
    *value = nullptr;
    return false;
}

bool ILInstruction::match_branch(Block** target_block) const {
    const auto* inst = as<Branch>(this);
    if (inst != nullptr) {
        *target_block = inst->target_block();
        return true;
    }
    *target_block = nullptr;
    return false;
}

bool ILInstruction::match_branch(const Block* target_block) const {
    const auto* inst = as<Branch>(this);
    return inst != nullptr && inst->target_block() == target_block;
}

bool ILInstruction::match_leave(BlockContainer** target_container, ILInstruction** value) const {
    const auto* inst = as<Leave>(this);
    if (inst != nullptr) {
        *target_container = inst->target_container();
        *value = inst->value();
        return true;
    }
    *target_container = nullptr;
    *value = nullptr;
    return false;
}

bool ILInstruction::match_leave(const BlockContainer* target_container, ILInstruction** value) const {
    const auto* inst = as<Leave>(this);
    if (inst != nullptr && target_container == inst->target_container()) {
        *value = inst->value();
        return true;
    }
    *value = nullptr;
    return false;
}

bool ILInstruction::match_leave(BlockContainer** target_container) const {
    const auto* inst = as<Leave>(this);
    if (inst != nullptr && inst->value()->match_nop()) {
        *target_container = inst->target_container();
        return true;
    }
    *target_container = nullptr;
    return false;
}

bool ILInstruction::match_leave(const BlockContainer* target_container) const {
    const auto* inst = as<Leave>(this);
    return inst != nullptr && inst->target_container() == target_container && inst->value()->match_nop();
}

bool ILInstruction::match_if_instruction(ILInstruction** condition, ILInstruction** true_inst, ILInstruction** false_inst) const {
    if (const auto* inst = as<IfInstruction>(this)) {
        *condition = inst->condition();
        *true_inst = inst->true_inst();
        *false_inst = inst->false_inst();
        return true;
    }
    *condition = nullptr;
    *true_inst = nullptr;
    *false_inst = nullptr;
    return false;
}

bool ILInstruction::match_if_instruction_positive_condition(ILInstruction** condition, ILInstruction** true_inst,
                                                            ILInstruction** false_inst) const {
    if (match_if_instruction(condition, true_inst, false_inst)) {
        // swap trueInst and falseInst for every logic.not in the condition
        ILInstruction* arg = nullptr;
        while ((*condition)->match_logic_not(&arg)) {
            *condition = arg;
            ILInstruction* tmp = *true_inst;
            *true_inst = *false_inst;
            *false_inst = tmp;
        }
        return true;
    }
    return false;
}

bool ILInstruction::match_if_instruction(ILInstruction** condition, ILInstruction** true_inst) const {
    const auto* inst = as<IfInstruction>(this);
    if (inst != nullptr && inst->false_inst()->match_nop()) {
        *condition = inst->condition();
        *true_inst = inst->true_inst();
        return true;
    }
    *condition = nullptr;
    *true_inst = nullptr;
    return false;
}

bool ILInstruction::match_logic_and(ILInstruction** lhs, ILInstruction** rhs) const {
    const auto* inst = as<IfInstruction>(this);
    if (inst != nullptr && inst->false_inst()->match_ldc_i4(0)) {
        *lhs = inst->condition();
        *rhs = inst->true_inst();
        return true;
    }
    *lhs = nullptr;
    *rhs = nullptr;
    return false;
}

bool ILInstruction::match_logic_or(ILInstruction** lhs, ILInstruction** rhs) const {
    const auto* inst = as<IfInstruction>(this);
    if (inst != nullptr && inst->true_inst()->match_ldc_i4(1)) {
        *lhs = inst->condition();
        *rhs = inst->false_inst();
        return true;
    }
    *lhs = nullptr;
    *rhs = nullptr;
    return false;
}

bool ILInstruction::match_logic_not(ILInstruction** arg) const {
    const auto* comp = as<Comp>(this);
    if (comp != nullptr && comp->kind() == ComparisonKind::Equality && comp->lifting_kind == ComparisonLiftingKind::None &&
        comp->right()->match_ldc_i4(0)) {
        *arg = comp->left();
        return true;
    }
    *arg = nullptr;
    return false;
}

}  // namespace cb::dotnet::decompiler::il
