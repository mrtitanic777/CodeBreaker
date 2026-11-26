// dotnet/decompiler/il/il_reader.cpp -- see il_reader.hpp.
#include "dotnet/decompiler/il/il_reader.hpp"

#include <algorithm>
#include <cassert>
#include <deque>
#include <format>
#include <map>
#include <unordered_map>
#include <unordered_set>

#include "dotnet/decompiler/disassembler/il_parser.hpp"
#include "dotnet/decompiler/disassembler/method_body_disassembler.hpp"
#include "dotnet/decompiler/il/block_builder.hpp"
#include "dotnet/decompiler/il/il_type_extensions.hpp"
#include "dotnet/decompiler/il/il_visitor.hpp"
#include "dotnet/decompiler/il/instruction_output_il.hpp"
#include "dotnet/decompiler/srm_extensions.hpp"
#include "dotnet/decompiler/typesystem/task_type.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"
#include "dotnet/decompiler/util/char_info.hpp"
#include "dotnet/decompiler/util/union_find.hpp"
#include "dotnet/metadata/il_opcode.hpp"
#include "dotnet/metadata/metadata_reader.hpp"

namespace cb::dotnet::decompiler::il {

using srm::ILOpCode;
using typesystem::ICompilation;
using typesystem::KnownTypeCode;
using typesystem::Sign;
using typesystem::TypeKind;

namespace {

// The evaluation stack: an immutable stack of the stack slot variables (upstream's ImmutableStack<ILVariable>). Enumerates from the top.
class VariableStack {
public:
    VariableStack() = default;
    bool is_empty() const { return top_ == nullptr; }
    ILVariable* peek() const { return top_->head; }
    VariableStack push(ILVariable* v) const { return VariableStack(std::make_shared<const Node>(Node{v, top_})); }
    VariableStack pop() const { return VariableStack(top_->tail); }
    VariableStack pop(ILVariable** v) const {
        *v = top_->head;
        return VariableStack(top_->tail);
    }
    int count() const {
        int n = 0;
        for (const Node* node = top_.get(); node != nullptr; node = node->tail.get()) ++n;
        return n;
    }
    // From the top down.
    std::vector<ILVariable*> items() const {
        std::vector<ILVariable*> result;
        for (const Node* node = top_.get(); node != nullptr; node = node->tail.get()) result.push_back(node->head);
        return result;
    }
    // The items pushed in order (the last on top).
    static VariableStack create_range(const std::vector<ILVariable*>& items) {
        VariableStack s;
        for (ILVariable* v : items) s = s.push(v);
        return s;
    }

private:
    struct Node {
        ILVariable* head;
        std::shared_ptr<const Node> tail;
    };
    explicit VariableStack(std::shared_ptr<const Node> top) : top_(std::move(top)) {}
    std::shared_ptr<const Node> top_;
};

const IType* find_type(const ICompilation& compilation, StackType stack_type, Sign sign = Sign::None) {
    switch (stack_type) {
        case StackType::Unknown: return &typesystem::SpecialType::unknown_type();
        case StackType::Ref: return typesystem::make_by_reference_type(&typesystem::SpecialType::unknown_type());
        default: return compilation.find_type(to_known_type_code(stack_type, sign));
    }
}

std::string hex4(int value) { return std::format("{:04x}", value); }

// string.IsNullOrWhiteSpace
bool is_null_or_white_space(const std::string& text) {
    for (char16_t c : util::utf16(text))
        if (!util::is_white_space(c)) return false;
    return true;
}

}  // namespace

class ILReader::Impl {
public:
    Impl(ILReader& owner, const MetadataModule& module)
        : owner_(owner), compilation_(module.compilation()), module_(module), metadata_(module.metadata()) {}

    ILFunction* read_il(srm::MethodDefinitionHandle method, const srm::MethodBodyBlock& body, GenericContext generic_context, ILFunctionKind kind,
                        const util::CancellationToken& cancellation_token);
    void write_typed_il(srm::MethodDefinitionHandle method, const srm::MethodBodyBlock& body, ITextOutput& output, GenericContext generic_context,
                        const util::CancellationToken& cancellation_token);

private:
    // A basic block as read: its Block, the stack on entry, and the edges out with the stack on each.
    struct ImportedBlock {
        ImportedBlock(int offset, VariableStack input_stack) : block(make<Block>(BlockKind::ControlFlow)), input_stack(std::move(input_stack)) {
            block->add_il_range(Interval(offset, offset));
        }
        Block* block;
        VariableStack input_stack;
        bool import_started = false;
        std::vector<std::pair<ImportedBlock*, VariableStack>> outgoing_edges;
        int start_il_offset() const { return block->start_il_offset(); }

        // Where the new edge's stack has a wider type in some slot, the wider types (and false when nothing changed).
        bool merge_stack_types(const VariableStack& new_edge) {
            std::vector<ILVariable*> a = input_stack.items();
            std::vector<ILVariable*> b = new_edge.items();
            bool changed = false;
            std::size_t n = std::min(a.size(), b.size());
            for (std::size_t i = 0; i < n; ++i)
                if (a[i]->stack_type < b[i]->stack_type) changed = true;
            if (!changed || a.size() != b.size()) return false;
            std::vector<ILVariable*> output;
            for (std::size_t i = 0; i < n; ++i) output.push_back(a[i]->stack_type < b[i]->stack_type ? b[i] : a[i]);
            std::reverse(output.begin(), output.end());  // the order of the stack again
            input_stack = VariableStack::create_range(output);
            import_started = false;
            return true;
        }
        void reset_for_reimport() {
            block->instructions.clear();
            outgoing_edges.clear();
        }
    };

    struct DecodedInstruction {
        DecodedInstruction(ILInstruction* inst) : instruction(inst) {}  // NOLINT: implicit, as upstream's conversion
        DecodedInstruction(ILInstruction* inst, bool pushed) : instruction(inst), pushed_on_expression_stack(pushed) {}
        ILInstruction* instruction;
        bool pushed_on_expression_stack = false;
    };

    void init(srm::MethodDefinitionHandle method_definition_handle, const srm::MethodBodyBlock& body, GenericContext generic_context);
    srm::EntityHandle read_and_decode_metadata_token();
    const IType* read_and_decode_type_reference();
    const IMethod* read_and_decode_method_reference();
    const IField* read_and_decode_field_reference();
    std::vector<ILVariable*> init_local_variables();
    void init_parameter_variables();
    ILVariable* create_il_variable(int index, const IType* type);
    ILVariable* create_il_variable(int index, const IType* parameter_type, const std::string& name);
    void warn(const std::string& message);
    util::UnionFind<ILVariable*> check_outgoing_edges();
    void insert_stack_adjustment(Block* block, ILVariable* var_a, ILVariable* var_b);
    ImportedBlock* store_stack_for_offset(int offset, const VariableStack& stack);
    void read_instructions(const util::CancellationToken& cancellation_token);
    void read_block(ImportedBlock* block, const util::CancellationToken& cancellation_token);
    bool current_stack_is_empty() const { return current_stack_.is_empty() && expression_stack_.empty(); }
    void prepare_branch_targets_and_stacks_for_exception_handlers();
    void ensure_exception_handlers_have_blocks();
    DecodedInstruction neg();
    DecodedInstruction decode_instruction();
    StackType peek_stack_type() const;
    DecodedInstruction push(ILInstruction* inst);
    ILInstruction* peek();
    ILInstruction* pop();
    ILInstruction* pop(StackType expected_type);
    ILInstruction* pop_pointer();
    ILInstruction* pop_st_obj_target();
    ILInstruction* pop_field_target(const IField* field);
    ILInstruction* pop_ld_fld_target(const IField* field);
    ILInstruction* return_();
    ILInstruction* decode_ldstr();
    ILInstruction* ldarg(int v);
    ILInstruction* ldarga(int v);
    ILInstruction* starg(int v);
    ILInstruction* ldloc(int v);
    ILInstruction* ldloca(int v);
    ILInstruction* stloc(int v);
    DecodedInstruction ld_elem(const IType* type);
    ILInstruction* st_elem(const IType* type);
    ILInstruction* init_obj(ILInstruction* target, const IType* type);
    DecodedInstruction decode_constrained_call();
    DecodedInstruction decode_tail_call();
    DecodedInstruction decode_unaligned();
    DecodedInstruction decode_volatile();
    DecodedInstruction decode_readonly();
    DecodedInstruction decode_call(OpCode op_code);
    DecodedInstruction decode_call_indirect();
    ILInstruction* comparison(ComparisonKind kind, bool un = false);
    bool is_invalid_branch(int target) const { return target < 0 || target >= reader_.length(); }
    ILInstruction* decode_comparison_branch(ILOpCode op_code, ComparisonKind kind, bool un = false);
    ILInstruction* decode_conditional_branch(ILOpCode op_code, bool negate);
    ILInstruction* decode_unconditional_branch(ILOpCode op_code, bool is_leave = false);
    void mark_branch_target(int target_il_offset, bool is_fall_through = false);
    void flush_expression_stack();
    ILInstruction* decode_switch();
    DecodedInstruction binary_numeric(BinaryNumericOperator op, bool check_for_overflow = false, Sign sign = Sign::None);
    ILInstruction* decode_jmp();
    ILInstruction* ld_token(srm::EntityHandle token);

    ILReader& owner_;
    const ICompilation& compilation_;
    const MetadataModule& module_;
    const srm::MetadataReader& metadata_;

    GenericContext generic_context_;
    const IMethod* method_ = nullptr;
    const srm::MethodBodyBlock* body_ = nullptr;
    StackType method_return_stack_type_ = StackType::Void;
    srm::BlobReader reader_;
    VariableStack current_stack_;
    ImportedBlock* current_block_ = nullptr;
    std::vector<ILInstruction*> expression_stack_;
    std::vector<ILVariable*> parameter_variables_;
    std::vector<ILVariable*> local_variables_;
    util::BitSet is_branch_target_{0};
    BlockContainer* main_container_ = nullptr;
    int current_instruction_start_ = 0;
    bool is_runtime_async_ = false;
    // by offset, kept in the order made (upstream's Dictionary, enumerated in insertion order)
    std::vector<std::unique_ptr<ImportedBlock>> blocks_;
    std::unordered_map<int, ImportedBlock*> blocks_by_offset_;
    std::deque<ImportedBlock*> import_queue_;
    // by exception region (its index in the body), in the order made
    std::vector<std::pair<int, ILVariable*>> variable_by_exception_handler_;
    std::vector<ILVariable*> stack_variables_;
    bool stack_variables_set_ = false;
    const IType* constrained_prefix_ = nullptr;
};

void ILReader::Impl::init(srm::MethodDefinitionHandle method_definition_handle, const srm::MethodBodyBlock& body, GenericContext generic_context) {
    if (method_definition_handle.is_nil()) throw std::invalid_argument("methodDefinitionHandle.IsNil");
    method_ = module_.get_definition(method_definition_handle);
    is_runtime_async_ = typesystem::any(module_.type_system_options(), typesystem::TypeSystemOptions::RuntimeAsync) &&
                        (metadata_.get_method_definition(method_definition_handle).impl_attributes() & kMethodImplAsync) != 0;
    if (generic_context.class_type_parameters == nullptr && generic_context.method_type_parameters == nullptr) {
        // no generic context given: the method's own type parameters
        generic_context = GenericContext::of(*method_);
    } else {
        // a generic context given: the method specialized for it
        method_ = method_->specialize_method(generic_context.to_substitution());
    }
    generic_context_ = generic_context;
    body_ = &body;
    reader_ = body.il_reader();
    current_stack_ = VariableStack();
    expression_stack_.clear();
    if (is_runtime_async_)
        method_return_stack_type_ = get_stack_type(*typesystem::task_type::unpack_any_task(compilation_, method_->return_type()));
    else
        method_return_stack_type_ = get_stack_type(*method_->return_type());
    init_parameter_variables();
    local_variables_ = init_local_variables();
    for (ILVariable* v : local_variables_) {
        v->set_initial_value_is_initialized(body.local_variables_initialized());
        v->set_uses_initial_value(true);
    }
    main_container_ = make<BlockContainer>(ContainerKind::Normal, method_return_stack_type_);
    blocks_.clear();
    blocks_by_offset_.clear();
    import_queue_.clear();
    is_branch_target_ = util::BitSet(reader_.length());
    variable_by_exception_handler_.clear();
}

srm::EntityHandle ILReader::Impl::read_and_decode_metadata_token() {
    int token = reader_.read_int32();
    // SRM uses negative tokens as "virtual tokens"; row ids below 1 are always invalid
    if (token <= 0) throw srm::BadImageFormatError(static_cast<std::uint64_t>(reader_.offset()), "Invalid metadata token");
    srm::EntityHandle handle = srm::entity_handle(static_cast<std::uint32_t>(token));
    // the runtime fails on a row id of 0; the code after such an instruction is taken as unreachable
    if (handle.is_nil()) throw srm::BadImageFormatError(static_cast<std::uint64_t>(reader_.offset()), "Invalid metadata token");
    return handle;
}

const IType* ILReader::Impl::read_and_decode_type_reference() {
    srm::EntityHandle type_reference = read_and_decode_metadata_token();
    return module_.resolve_type(type_reference, generic_context_);
}

const IMethod* ILReader::Impl::read_and_decode_method_reference() {
    srm::EntityHandle method_reference = read_and_decode_metadata_token();
    return module_.resolve_method(method_reference, generic_context_);
}

const IField* ILReader::Impl::read_and_decode_field_reference() {
    srm::EntityHandle field_reference = read_and_decode_metadata_token();
    auto* f = dynamic_cast<const IField*>(module_.resolve_entity(field_reference, generic_context_));
    if (f == nullptr) throw srm::BadImageFormatError(static_cast<std::uint64_t>(reader_.offset()), "Invalid field token");
    return f;
}

std::vector<ILVariable*> ILReader::Impl::init_local_variables() {
    if (body_->local_signature().is_nil()) return {};
    std::vector<const IType*> variable_types;
    try {
        variable_types = module_.decode_local_signature(body_->local_signature(), generic_context_);
    } catch (const srm::BadImageFormatError& ex) {
        owner_.warnings.push_back("Error decoding local variables: " + ex.message());
        variable_types.clear();
    }
    std::vector<ILVariable*> local_variables(variable_types.size());
    for (std::size_t index = 0; index < variable_types.size(); ++index)
        local_variables[index] = create_il_variable(static_cast<int>(index), variable_types[index]);
    return local_variables;
}

void ILReader::Impl::init_parameter_variables() {
    const auto& parameters = method_->parameters();
    int pop_count = static_cast<int>(parameters.size());
    if (!method_->is_static()) ++pop_count;
    if (!parameters.empty() && parameters.back()->type() == &typesystem::SpecialType::arg_list()) --pop_count;
    parameter_variables_.assign(static_cast<std::size_t>(pop_count), nullptr);
    int param_index = 0;
    int offset = 0;
    if (!method_->is_static()) {
        offset = 1;
        const IType* declaring_type = method_->declaring_type();
        if (typesystem::is_unbound(declaring_type)) {
            // a method definition (not specialized): its declaring type is a definition too, made usable with its own type parameters
            std::vector<const IType*> type_arguments(declaring_type->type_parameters().begin(), declaring_type->type_parameters().end());
            declaring_type = typesystem::make_parameterized_type(declaring_type, std::move(type_arguments));
        }
        ILVariable* il_var = create_il_variable(-1, declaring_type, "this");
        il_var->is_ref_read_only = method_->this_is_ref_read_only();
        parameter_variables_[static_cast<std::size_t>(param_index++)] = il_var;
    }
    while (param_index < static_cast<int>(parameter_variables_.size())) {
        const typesystem::IParameter* parameter = parameters[static_cast<std::size_t>(param_index - offset)];
        ILVariable* il_var = create_il_variable(param_index - offset, parameter->type(), parameter->name());
        il_var->is_ref_read_only =
            parameter->reference_kind() == typesystem::ReferenceKind::In || parameter->reference_kind() == typesystem::ReferenceKind::RefReadOnly;
        parameter_variables_[static_cast<std::size_t>(param_index)] = il_var;
        ++param_index;
    }
    assert(param_index == static_cast<int>(parameter_variables_.size()));
}

ILVariable* ILReader::Impl::create_il_variable(int index, const IType* type) {
    VariableKind kind;
    if (auto* pinned = dynamic_cast<const typesystem::PinnedType*>(typesystem::skip_modifiers(type))) {
        kind = VariableKind::PinnedLocal;
        type = pinned->element_type();
    } else {
        kind = VariableKind::Local;
    }
    // (debug information's extra type information and names: not yet ported)
    ILVariable* il_var = make<ILVariable>(kind, type, index);
    il_var->name = "V_" + std::to_string(index);
    il_var->has_generated_name = true;
    return il_var;
}

ILVariable* ILReader::Impl::create_il_variable(int index, const IType* parameter_type, const std::string& name) {
    assert(!typesystem::is_unbound(parameter_type));
    const typesystem::ITypeDefinition* def = parameter_type->get_definition();
    if (def != nullptr && index < 0 && def->is_reference_type() == false) parameter_type = typesystem::make_by_reference_type(parameter_type);
    ILVariable* il_var = make<ILVariable>(VariableKind::Parameter, parameter_type, index);
    assert(il_var->store_count() == 1);  // the initial store when the method is called with an argument
    if (index < 0) {
        il_var->name = "this";
    } else if (is_null_or_white_space(name)) {
        il_var->name = "P_" + std::to_string(index);
        il_var->has_generated_name = true;
    } else {
        il_var->name = name;
    }
    return il_var;
}

void ILReader::Impl::warn(const std::string& message) { owner_.warnings.push_back(std::format("IL_{}: {}", hex4(current_instruction_start_), message)); }

util::UnionFind<ILVariable*> ILReader::Impl::check_outgoing_edges() {
    util::UnionFind<ILVariable*> union_find;
    for (const auto& block : blocks_) {
        for (const auto& [outgoing, stack] : block->outgoing_edges) {
            std::vector<ILVariable*> a = stack.items();
            std::vector<ILVariable*> b = outgoing->input_stack.items();
            if (a.size() != b.size()) {
                // mismatched stacks are not merged
                owner_.warnings.push_back(std::format("IL_{}->IL{}: Incompatible stack heights: {} vs {}", hex4(block->block->end_il_offset()),
                                                      hex4(outgoing->start_il_offset()), a.size(), b.size()));
                continue;
            }
            for (std::size_t i = 0; i < a.size(); ++i) {
                ILVariable* var_a = a[i];
                ILVariable* var_b = b[i];
                if (var_a->stack_type == var_b->stack_type) {
                    // the stack types match: one variable
                    union_find.merge(var_a, var_b);
                } else {
                    assert(var_a->stack_type < var_b->stack_type);
                    if (!(var_a->stack_type == StackType::I && var_b->stack_type == StackType::I4) &&
                        !(var_a->stack_type == StackType::I4 && var_b->stack_type == StackType::I) &&
                        !(var_a->stack_type == StackType::F4 && var_b->stack_type == StackType::F8) &&
                        !(var_a->stack_type == StackType::F8 && var_b->stack_type == StackType::F4) && var_a->stack_type != StackType::Unknown &&
                        var_b->stack_type != StackType::Unknown) {
                        owner_.warnings.push_back(std::format("IL_{}->IL{}: Incompatible stack types: {} vs {}", hex4(block->block->end_il_offset()),
                                                              hex4(outgoing->start_il_offset()), to_string(var_a->stack_type),
                                                              to_string(var_b->stack_type)));
                    }
                    insert_stack_adjustment(block->block, var_a, var_b);
                }
            }
        }
    }
    return union_find;
}

void ILReader::Impl::insert_stack_adjustment(Block* block, ILVariable* var_a, ILVariable* var_b) {
    int insertion_position = block->instructions.count();
    while (insertion_position > 0 && block->instructions[insertion_position - 1]->has_flag(InstructionFlags::MayBranch)) {
        // a branch must not be initializing var_a
        assert(!block->instructions[insertion_position - 1]->has_flag(InstructionFlags::MayWriteLocals));
        --insertion_position;
    }
    ILInstruction* value = make<LdLoc>(var_a);
    value = make<Conv>(value, to_primitive_type(var_b->stack_type), false, Sign::Signed);
    auto* store = make<StLoc>(var_b, value);
    store->is_stack_adjustment = true;
    block->instructions.insert(insertion_position, store);
}

ILReader::Impl::ImportedBlock* ILReader::Impl::store_stack_for_offset(int offset, const VariableStack& stack) {
    if (auto it = blocks_by_offset_.find(offset); it != blocks_by_offset_.end()) {
        ImportedBlock* existing = it->second;
        bool was_imported = existing->import_started;
        // the stack changed: the block is imported again
        if (existing->merge_stack_types(stack) && was_imported) import_queue_.push_back(existing);
        return existing;
    }
    auto new_block = std::make_unique<ImportedBlock>(offset, stack);
    ImportedBlock* result = new_block.get();
    blocks_.push_back(std::move(new_block));
    blocks_by_offset_.emplace(offset, result);
    import_queue_.push_back(result);
    return result;
}

namespace {

// Gives the stack slot variables merged by the union-find one variable each (named S_0, S_1 ... in the order first seen).
class CollectStackVariablesVisitor final : public ILVisitorT<ILInstruction*> {
public:
    explicit CollectStackVariablesVisitor(util::UnionFind<ILVariable*>& union_find) : union_find_(union_find) {}
    std::vector<ILVariable*> variables;  // in the order added (upstream's HashSet enumerates so)

    ILInstruction* default_visit(ILInstruction& inst) override {
        for (ILInstruction* child : inst.children()) {
            ILInstruction* new_child = accept_visitor(*child, *this);
            if (new_child != child) child->replace_with(new_child);
        }
        return &inst;
    }
    ILInstruction* visit_ld_loc(LdLoc& inst) override {
        default_visit(inst);
        if (inst.variable()->kind() == VariableKind::StackSlot) {
            ILVariable* variable = union_find_.find(inst.variable());
            if (seen_.insert(variable).second) {
                variables.push_back(variable);
                variable->name = "S_" + std::to_string(variables.size() - 1);
            }
            auto* result = make<LdLoc>(variable);
            result->set_il_range(inst);
            return result;
        }
        return &inst;
    }
    ILInstruction* visit_st_loc(StLoc& inst) override {
        default_visit(inst);
        if (inst.variable()->kind() == VariableKind::StackSlot) {
            ILVariable* variable = union_find_.find(inst.variable());
            if (seen_.insert(variable).second) {
                variables.push_back(variable);
                variable->name = "S_" + std::to_string(variables.size() - 1);
            }
            auto* result = make<StLoc>(variable, inst.value());
            result->set_il_range(inst);
            return result;
        }
        return &inst;
    }

private:
    util::UnionFind<ILVariable*>& union_find_;
    std::unordered_set<ILVariable*> seen_;
};

}  // namespace

void ILReader::Impl::read_instructions(const util::CancellationToken& cancellation_token) {
    reader_.reset();
    store_stack_for_offset(0, VariableStack());
    if (reader_.length() == 0) {
        blocks_by_offset_.at(0)->block->instructions.add(make<InvalidBranch>("Empty body found. Decompiled assembly might be a reference assembly."));
        stack_variables_.clear();
        stack_variables_set_ = true;
        return;
    }
    disassembler::set_branch_targets(reader_, is_branch_target_);
    prepare_branch_targets_and_stacks_for_exception_handlers();
    // the IL, block by block
    while (!import_queue_.empty()) {
        cancellation_token.throw_if_cancellation_requested();
        ImportedBlock* block = import_queue_.front();
        import_queue_.pop_front();
        read_block(block, cancellation_token);
    }
    ensure_exception_handlers_have_blocks();
    // one variable for each stack slot where the edges meet
    util::UnionFind<ILVariable*> union_find = check_outgoing_edges();
    CollectStackVariablesVisitor visitor(union_find);
    for (const auto& block : blocks_) accept_visitor(*block->block, visitor);
    stack_variables_ = visitor.variables;
    stack_variables_set_ = true;
}

void ILReader::Impl::read_block(ImportedBlock* block, const util::CancellationToken& cancellation_token) {
    assert(!block->import_started);
    block->reset_for_reimport();
    block->import_started = true;
    reader_.set_offset(block->start_il_offset());
    current_block_ = block;
    current_stack_ = block->input_stack;
    // up to the end of the block
    while (reader_.remaining_bytes() > 0) {
        cancellation_token.throw_if_cancellation_requested();
        int start = reader_.offset();
        current_instruction_start_ = start;
        bool started_with_empty_stack = current_stack_is_empty();
        DecodedInstruction decoded = nullptr;
        try {
            decoded = decode_instruction();
        } catch (const srm::BadImageFormatError& ex) {
            decoded = make<InvalidBranch>(ex.message());
        }
        ILInstruction* inst = decoded.instruction;
        if (inst->result_type() == StackType::Unknown && inst->op_code != OpCode::InvalidBranch && inst->op_code != OpCode::InvalidExpression)
            warn("Unknown result type (might be due to invalid IL or missing references)");
#ifndef NDEBUG
        inst->check_invariant(ILPhase::InILReader);
#endif
        int end = reader_.offset();
        inst->add_il_range(Interval(start, end));
        if (!decoded.pushed_on_expression_stack) {
            // flush, so side effects keep their order
            flush_expression_stack();
            block->block->instructions.add(inst);
        }
        if ((!decoded.pushed_on_expression_stack &&
             (inst->op_code == OpCode::Nop || inst->op_code == OpCode::Call || inst->op_code == OpCode::CallIndirect || inst->op_code == OpCode::CallVirt)) ||
            started_with_empty_stack)
            owner_.sequence_point_candidates.push_back(inst->start_il_offset());
        if (inst->has_direct_flag(InstructionFlags::EndPointUnreachable)) break;  // the end of the block; what follows is unreachable
        if (is_branch_target_[end] || inst->has_flag(InstructionFlags::MayBranch)) break;  // the end of the block (a fall through follows)
    }
    // finish the block
    flush_expression_stack();
    block->block->add_il_range(Interval(block->start_il_offset(), reader_.offset()));
    if (!block->block->has_flag(InstructionFlags::EndPointUnreachable)) {
        // the fall through branch
        ILInstruction* branch;
        if (reader_.remaining_bytes() > 0) {
            mark_branch_target(reader_.offset(), true);
            branch = make<Branch>(reader_.offset());
        } else {
            branch = make<InvalidBranch>("End of method reached without returning.");
        }
        auto* switch_inst = as<SwitchInstruction>(block->block->instructions.last_or_default());
        if (switch_inst != nullptr && switch_inst->sections.last()->body()->match_nop()) {
            // the default branch in the switch, not after it
            switch_inst->sections.last()->set_body(branch);
            assert(switch_inst->has_flag(InstructionFlags::EndPointUnreachable));
        } else {
            block->block->instructions.add(branch);
        }
    }
}

void ILReader::Impl::prepare_branch_targets_and_stacks_for_exception_handlers() {
    // the branch targets and stacks the exception handlers give
    const auto& regions = body_->exception_regions();
    for (std::size_t i = 0; i < regions.size(); ++i) {
        const srm::ExceptionRegion& eh = regions[i];
        // the start of a try block is a block boundary
        is_branch_target_.set(eh.try_offset);
        VariableStack eh_stack;
        if (eh.kind == srm::ExceptionRegionKind::Catch) {
            const IType* catch_type = module_.resolve_type(eh.catch_type, generic_context_);
            ILVariable* v = make<ILVariable>(VariableKind::ExceptionStackSlot, catch_type, eh.handler_offset);
            v->name = "E_" + std::to_string(eh.handler_offset);
            v->has_generated_name = true;
            variable_by_exception_handler_.emplace_back(static_cast<int>(i), v);
            eh_stack = VariableStack().push(v);
        } else if (eh.kind == srm::ExceptionRegionKind::Filter) {
            ILVariable* v = make<ILVariable>(VariableKind::ExceptionStackSlot, compilation_.find_type(KnownTypeCode::Object), eh.handler_offset);
            v->name = "E_" + std::to_string(eh.handler_offset);
            v->has_generated_name = true;
            variable_by_exception_handler_.emplace_back(static_cast<int>(i), v);
            eh_stack = VariableStack().push(v);
        }
        if (eh.filter_offset != -1) {
            is_branch_target_.set(eh.filter_offset);
            store_stack_for_offset(eh.filter_offset, eh_stack);
        }
        if (eh.handler_offset != -1) {
            is_branch_target_.set(eh.handler_offset);
            store_stack_for_offset(eh.handler_offset, eh_stack);
        }
    }
}

void ILReader::Impl::ensure_exception_handlers_have_blocks() {
    // The handler and filter offsets have blocks; an unreachable try offset may not, and BlockBuilder needs one (it may delete the whole
    // unreachable try later).
    for (const srm::ExceptionRegion& eh : body_->exception_regions()) {
        if (blocks_by_offset_.contains(eh.try_offset)) continue;
        auto block = std::make_unique<ImportedBlock>(eh.try_offset, VariableStack());
        block->block->instructions.add(make<InvalidBranch>("Unreachable try block"));
        blocks_by_offset_.emplace(eh.try_offset, block.get());
        blocks_.push_back(std::move(block));
    }
}

void ILReader::Impl::write_typed_il(srm::MethodDefinitionHandle method, const srm::MethodBodyBlock& body, ITextOutput& output, GenericContext generic_context,
                                    const util::CancellationToken& cancellation_token) {
    init(method, body, generic_context);
    read_instructions(cancellation_token);
    std::vector<ImportedBlock*> ordered;
    for (const auto& b : blocks_) ordered.push_back(b.get());
    std::stable_sort(ordered.begin(), ordered.end(), [](const ImportedBlock* a, const ImportedBlock* b) { return a->start_il_offset() < b->start_il_offset(); });
    for (ImportedBlock* import_block : ordered) {
        output.write("   [");
        bool is_first_element = true;
        for (ILVariable* element : import_block->input_stack.items()) {
            if (is_first_element)
                is_first_element = false;
            else
                output.write(", ");
            output.write_local_reference(element->name.value_or(""), static_cast<const ILVariable*>(element));
            output.write(":");
            write(output, element->stack_type);
        }
        output.write(']');
        output.write_line();
        import_block->block->write_to(output, ILAstWritingOptions{});
        output.write_line();
    }
    disassembler::MethodBodyDisassembler disassembler(output, cancellation_token);
    disassembler.detect_control_structure = false;
    disassembler.write_exception_handlers(*module_.metadata_file(), method, body);
}

ILFunction* ILReader::Impl::read_il(srm::MethodDefinitionHandle method, const srm::MethodBodyBlock& body, GenericContext generic_context, ILFunctionKind kind,
                                    const util::CancellationToken& cancellation_token) {
    cancellation_token.throw_if_cancellation_requested();
    init(method, body, generic_context);
    read_instructions(cancellation_token);
    std::vector<std::pair<int, ILVariable*>> handler_variables = variable_by_exception_handler_;
    BlockBuilder block_builder(body, handler_variables, compilation_);
    std::vector<Block*> basic_blocks;
    for (const auto& ib : blocks_) basic_blocks.push_back(ib->block);
    block_builder.create_blocks(main_container_, basic_blocks, cancellation_token);
    auto* function = make<ILFunction>(method_, static_cast<int>(body.il_bytes().size()), generic_context_, main_container_, kind);
    if (is_runtime_async_) function->async_return_type = typesystem::task_type::unpack_any_task(compilation_, method_->return_type());
    for (ILVariable* v : parameter_variables_) function->variables().add(v);
    for (ILVariable* v : local_variables_) function->variables().add(v);
    function->local_variable_signature_length = static_cast<int>(local_variables_.size());
    assert(stack_variables_set_);
    for (ILVariable* v : stack_variables_) function->variables().add(v);
    for (const auto& [region, v] : variable_by_exception_handler_) function->variables().add(v);
    for (ILVariable* v : block_builder.on_error_dispatcher_variables()) function->variables().add(v);
    function->add_ref();  // the root is connected
    std::vector<Block*> removed_blocks;
    std::vector<BlockContainer*> containers;
    for (ILInstruction* inst : function->descendants())
        if (auto* c = as<BlockContainer>(inst)) containers.push_back(c);
    for (BlockContainer* c : containers) {
        std::vector<Block*> new_order = c->topological_sort(true);
        if (new_order.size() < static_cast<std::size_t>(c->blocks.count())) {
            std::unordered_set<Block*> kept(new_order.begin(), new_order.end());
            for (Block* b : c->blocks)
                if (!kept.contains(b) && std::find(removed_blocks.begin(), removed_blocks.end(), b) == removed_blocks.end()) removed_blocks.push_back(b);
        }
        c->blocks.replace_list(new_order);
    }
    if (!removed_blocks.empty()) {
        std::stable_sort(removed_blocks.begin(), removed_blocks.end(), [](const Block* a, const Block* b) { return a->start_il_offset() < b->start_il_offset(); });
        std::string text = "Discarded unreachable code: ";
        for (std::size_t i = 0; i < removed_blocks.size(); ++i) {
            if (i > 0) text += ", ";
            text += "IL_" + hex4(removed_blocks[i]->start_il_offset());
        }
        function->warnings.push_back(text);
    }
    std::sort(owner_.sequence_point_candidates.begin(), owner_.sequence_point_candidates.end());
    function->sequence_point_candidates = owner_.sequence_point_candidates;
    function->warnings.insert(function->warnings.end(), owner_.warnings.begin(), owner_.warnings.end());
    return function;
}

ILReader::Impl::DecodedInstruction ILReader::Impl::neg() {
    switch (peek_stack_type()) {
        case StackType::I4: {
            ILInstruction* zero = make<LdcI4>(0);
            return push(make<BinaryNumericInstruction>(BinaryNumericOperator::Sub, zero, pop(), false, Sign::None));
        }
        case StackType::I: {
            ILInstruction* zero = make<Conv>(make<LdcI4>(0), PrimitiveType::I, false, Sign::None);
            return push(make<BinaryNumericInstruction>(BinaryNumericOperator::Sub, zero, pop(), false, Sign::None));
        }
        case StackType::I8: {
            ILInstruction* zero = make<LdcI8>(0);
            return push(make<BinaryNumericInstruction>(BinaryNumericOperator::Sub, zero, pop(), false, Sign::None));
        }
        case StackType::F4: {
            ILInstruction* zero = make<LdcF4>(0.0f);
            return push(make<BinaryNumericInstruction>(BinaryNumericOperator::Sub, zero, pop(), false, Sign::None));
        }
        case StackType::F8: {
            ILInstruction* zero = make<LdcF8>(0.0);
            return push(make<BinaryNumericInstruction>(BinaryNumericOperator::Sub, zero, pop(), false, Sign::None));
        }
        default: {
            warn("Unsupported input type for neg.");
            ILInstruction* zero = make<LdcI4>(0);
            return push(make<BinaryNumericInstruction>(BinaryNumericOperator::Sub, zero, pop(), false, Sign::None));
        }
    }
}

ILReader::Impl::DecodedInstruction ILReader::Impl::decode_instruction() {
    if (reader_.remaining_bytes() == 0) return make<InvalidBranch>("Unexpected end of body");
    ILOpCode op_code = disassembler::decode_op_code(reader_);
    auto conv = [&](PrimitiveType target, bool overflow, Sign sign) { return push(make<Conv>(pop(), target, overflow, sign)); };
    auto ld_obj = [&](KnownTypeCode code) { return push(make<LdObj>(pop_pointer(), compilation_.find_type(code))); };
    // stind: value is popped first, then the target (the target runs before the value, so the order is kept)
    auto st_ind = [&](StackType value_type, KnownTypeCode code) -> DecodedInstruction {
        ILInstruction* value = pop(value_type);
        ILInstruction* target = pop_st_obj_target();
        return make<StObj>(target, value, compilation_.find_type(code));
    };
    switch (op_code) {
        case ILOpCode::Constrained: return decode_constrained_call();
        case ILOpCode::Readonly: return decode_readonly();
        case ILOpCode::Tail: return decode_tail_call();
        case ILOpCode::Unaligned: return decode_unaligned();
        case ILOpCode::Volatile: return decode_volatile();
        case ILOpCode::Add: return binary_numeric(BinaryNumericOperator::Add);
        case ILOpCode::Add_ovf: return binary_numeric(BinaryNumericOperator::Add, true, Sign::Signed);
        case ILOpCode::Add_ovf_un: return binary_numeric(BinaryNumericOperator::Add, true, Sign::Unsigned);
        case ILOpCode::And: return binary_numeric(BinaryNumericOperator::BitAnd);
        case ILOpCode::Arglist: return push(make<Arglist>());
        case ILOpCode::Beq:
        case ILOpCode::Beq_s: return decode_comparison_branch(op_code, ComparisonKind::Equality);
        case ILOpCode::Bge:
        case ILOpCode::Bge_s: return decode_comparison_branch(op_code, ComparisonKind::GreaterThanOrEqual);
        case ILOpCode::Bge_un:
        case ILOpCode::Bge_un_s: return decode_comparison_branch(op_code, ComparisonKind::GreaterThanOrEqual, true);
        case ILOpCode::Bgt:
        case ILOpCode::Bgt_s: return decode_comparison_branch(op_code, ComparisonKind::GreaterThan);
        case ILOpCode::Bgt_un:
        case ILOpCode::Bgt_un_s: return decode_comparison_branch(op_code, ComparisonKind::GreaterThan, true);
        case ILOpCode::Ble:
        case ILOpCode::Ble_s: return decode_comparison_branch(op_code, ComparisonKind::LessThanOrEqual);
        case ILOpCode::Ble_un:
        case ILOpCode::Ble_un_s: return decode_comparison_branch(op_code, ComparisonKind::LessThanOrEqual, true);
        case ILOpCode::Blt:
        case ILOpCode::Blt_s: return decode_comparison_branch(op_code, ComparisonKind::LessThan);
        case ILOpCode::Blt_un:
        case ILOpCode::Blt_un_s: return decode_comparison_branch(op_code, ComparisonKind::LessThan, true);
        case ILOpCode::Bne_un:
        case ILOpCode::Bne_un_s: return decode_comparison_branch(op_code, ComparisonKind::Inequality, true);
        case ILOpCode::Br:
        case ILOpCode::Br_s: return decode_unconditional_branch(op_code);
        case ILOpCode::Break: return make<DebugBreak>();
        case ILOpCode::Brfalse:
        case ILOpCode::Brfalse_s: return decode_conditional_branch(op_code, true);
        case ILOpCode::Brtrue:
        case ILOpCode::Brtrue_s: return decode_conditional_branch(op_code, false);
        case ILOpCode::Call: return decode_call(OpCode::Call);
        case ILOpCode::Callvirt: return decode_call(OpCode::CallVirt);
        case ILOpCode::Calli: return decode_call_indirect();
        case ILOpCode::Ceq: return push(comparison(ComparisonKind::Equality));
        case ILOpCode::Cgt: return push(comparison(ComparisonKind::GreaterThan));
        case ILOpCode::Cgt_un: return push(comparison(ComparisonKind::GreaterThan, true));
        case ILOpCode::Clt: return push(comparison(ComparisonKind::LessThan));
        case ILOpCode::Clt_un: return push(comparison(ComparisonKind::LessThan, true));
        case ILOpCode::Ckfinite: return make<Ckfinite>(peek());
        case ILOpCode::Conv_i1: return conv(PrimitiveType::I1, false, Sign::None);
        case ILOpCode::Conv_i2: return conv(PrimitiveType::I2, false, Sign::None);
        case ILOpCode::Conv_i4: return conv(PrimitiveType::I4, false, Sign::None);
        case ILOpCode::Conv_i8: return conv(PrimitiveType::I8, false, Sign::None);
        case ILOpCode::Conv_r4: return conv(PrimitiveType::R4, false, Sign::Signed);
        case ILOpCode::Conv_r8: return conv(PrimitiveType::R8, false, Sign::Signed);
        case ILOpCode::Conv_u1: return conv(PrimitiveType::U1, false, Sign::None);
        case ILOpCode::Conv_u2: return conv(PrimitiveType::U2, false, Sign::None);
        case ILOpCode::Conv_u4: return conv(PrimitiveType::U4, false, Sign::None);
        case ILOpCode::Conv_u8: return conv(PrimitiveType::U8, false, Sign::None);
        case ILOpCode::Conv_i: return conv(PrimitiveType::I, false, Sign::None);
        case ILOpCode::Conv_u: return conv(PrimitiveType::U, false, Sign::None);
        case ILOpCode::Conv_r_un: return conv(PrimitiveType::R, false, Sign::Unsigned);
        case ILOpCode::Conv_ovf_i1: return conv(PrimitiveType::I1, true, Sign::Signed);
        case ILOpCode::Conv_ovf_i2: return conv(PrimitiveType::I2, true, Sign::Signed);
        case ILOpCode::Conv_ovf_i4: return conv(PrimitiveType::I4, true, Sign::Signed);
        case ILOpCode::Conv_ovf_i8: return conv(PrimitiveType::I8, true, Sign::Signed);
        case ILOpCode::Conv_ovf_u1: return conv(PrimitiveType::U1, true, Sign::Signed);
        case ILOpCode::Conv_ovf_u2: return conv(PrimitiveType::U2, true, Sign::Signed);
        case ILOpCode::Conv_ovf_u4: return conv(PrimitiveType::U4, true, Sign::Signed);
        case ILOpCode::Conv_ovf_u8: return conv(PrimitiveType::U8, true, Sign::Signed);
        case ILOpCode::Conv_ovf_i: return conv(PrimitiveType::I, true, Sign::Signed);
        case ILOpCode::Conv_ovf_u: return conv(PrimitiveType::U, true, Sign::Signed);
        case ILOpCode::Conv_ovf_i1_un: return conv(PrimitiveType::I1, true, Sign::Unsigned);
        case ILOpCode::Conv_ovf_i2_un: return conv(PrimitiveType::I2, true, Sign::Unsigned);
        case ILOpCode::Conv_ovf_i4_un: return conv(PrimitiveType::I4, true, Sign::Unsigned);
        case ILOpCode::Conv_ovf_i8_un: return conv(PrimitiveType::I8, true, Sign::Unsigned);
        case ILOpCode::Conv_ovf_u1_un: return conv(PrimitiveType::U1, true, Sign::Unsigned);
        case ILOpCode::Conv_ovf_u2_un: return conv(PrimitiveType::U2, true, Sign::Unsigned);
        case ILOpCode::Conv_ovf_u4_un: return conv(PrimitiveType::U4, true, Sign::Unsigned);
        case ILOpCode::Conv_ovf_u8_un: return conv(PrimitiveType::U8, true, Sign::Unsigned);
        case ILOpCode::Conv_ovf_i_un: return conv(PrimitiveType::I, true, Sign::Unsigned);
        case ILOpCode::Conv_ovf_u_un: return conv(PrimitiveType::U, true, Sign::Unsigned);
        case ILOpCode::Cpblk: {
            // the ILAst runs destAddress, sourceAddress, size: the IL's order
            ILInstruction* size = pop(StackType::I4);
            ILInstruction* source_address = pop_pointer();
            ILInstruction* dest_address = pop_pointer();
            return make<Cpblk>(dest_address, source_address, size);
        }
        case ILOpCode::Div: return binary_numeric(BinaryNumericOperator::Div, false, Sign::Signed);
        case ILOpCode::Div_un: return binary_numeric(BinaryNumericOperator::Div, false, Sign::Unsigned);
        case ILOpCode::Dup: return push(peek());
        case ILOpCode::Endfilter: return make<Leave>(nullptr, pop());
        case ILOpCode::Endfinally: return make<Leave>(nullptr);
        case ILOpCode::Initblk: {
            // the ILAst runs address, value, size: the IL's order
            ILInstruction* size = pop(StackType::I4);
            ILInstruction* value = pop(StackType::I4);
            ILInstruction* address = pop_pointer();
            return make<Initblk>(address, value, size);
        }
        case ILOpCode::Jmp: return decode_jmp();
        case ILOpCode::Ldarg:
        case ILOpCode::Ldarg_s: return push(ldarg(disassembler::decode_index(reader_, op_code)));
        case ILOpCode::Ldarg_0: return push(ldarg(0));
        case ILOpCode::Ldarg_1: return push(ldarg(1));
        case ILOpCode::Ldarg_2: return push(ldarg(2));
        case ILOpCode::Ldarg_3: return push(ldarg(3));
        case ILOpCode::Ldarga:
        case ILOpCode::Ldarga_s: return push(ldarga(disassembler::decode_index(reader_, op_code)));
        case ILOpCode::Ldc_i4: return push(make<LdcI4>(reader_.read_int32()));
        case ILOpCode::Ldc_i8: return push(make<LdcI8>(reader_.read_int64()));
        case ILOpCode::Ldc_r4: return push(make<LdcF4>(reader_.read_single()));
        case ILOpCode::Ldc_r8: return push(make<LdcF8>(reader_.read_double()));
        case ILOpCode::Ldc_i4_m1: return push(make<LdcI4>(-1));
        case ILOpCode::Ldc_i4_0: return push(make<LdcI4>(0));
        case ILOpCode::Ldc_i4_1: return push(make<LdcI4>(1));
        case ILOpCode::Ldc_i4_2: return push(make<LdcI4>(2));
        case ILOpCode::Ldc_i4_3: return push(make<LdcI4>(3));
        case ILOpCode::Ldc_i4_4: return push(make<LdcI4>(4));
        case ILOpCode::Ldc_i4_5: return push(make<LdcI4>(5));
        case ILOpCode::Ldc_i4_6: return push(make<LdcI4>(6));
        case ILOpCode::Ldc_i4_7: return push(make<LdcI4>(7));
        case ILOpCode::Ldc_i4_8: return push(make<LdcI4>(8));
        case ILOpCode::Ldc_i4_s: return push(make<LdcI4>(reader_.read_sbyte()));
        case ILOpCode::Ldnull: return push(make<LdNull>());
        case ILOpCode::Ldstr: return push(decode_ldstr());
        case ILOpCode::Ldftn: return push(make<LdFtn>(read_and_decode_method_reference()));
        case ILOpCode::Ldind_i1: return ld_obj(KnownTypeCode::SByte);
        case ILOpCode::Ldind_i2: return ld_obj(KnownTypeCode::Int16);
        case ILOpCode::Ldind_i4: return ld_obj(KnownTypeCode::Int32);
        case ILOpCode::Ldind_i8: return ld_obj(KnownTypeCode::Int64);
        case ILOpCode::Ldind_u1: return ld_obj(KnownTypeCode::Byte);
        case ILOpCode::Ldind_u2: return ld_obj(KnownTypeCode::UInt16);
        case ILOpCode::Ldind_u4: return ld_obj(KnownTypeCode::UInt32);
        case ILOpCode::Ldind_r4: return ld_obj(KnownTypeCode::Single);
        case ILOpCode::Ldind_r8: return ld_obj(KnownTypeCode::Double);
        case ILOpCode::Ldind_i: return ld_obj(KnownTypeCode::IntPtr);
        case ILOpCode::Ldind_ref: return ld_obj(KnownTypeCode::Object);
        case ILOpCode::Ldloc:
        case ILOpCode::Ldloc_s: return push(ldloc(disassembler::decode_index(reader_, op_code)));
        case ILOpCode::Ldloc_0: return push(ldloc(0));
        case ILOpCode::Ldloc_1: return push(ldloc(1));
        case ILOpCode::Ldloc_2: return push(ldloc(2));
        case ILOpCode::Ldloc_3: return push(ldloc(3));
        case ILOpCode::Ldloca:
        case ILOpCode::Ldloca_s: return push(ldloca(disassembler::decode_index(reader_, op_code)));
        case ILOpCode::Leave:
        case ILOpCode::Leave_s: return decode_unconditional_branch(op_code, true);
        case ILOpCode::Localloc: return push(make<LocAlloc>(pop()));
        case ILOpCode::Mul: return binary_numeric(BinaryNumericOperator::Mul, false, Sign::None);
        case ILOpCode::Mul_ovf: return binary_numeric(BinaryNumericOperator::Mul, true, Sign::Signed);
        case ILOpCode::Mul_ovf_un: return binary_numeric(BinaryNumericOperator::Mul, true, Sign::Unsigned);
        case ILOpCode::Neg: return neg();
        case ILOpCode::Newobj: return decode_call(OpCode::NewObj);
        case ILOpCode::Nop: return make<Nop>();
        case ILOpCode::Not: return push(make<BitNot>(pop()));
        case ILOpCode::Or: return binary_numeric(BinaryNumericOperator::BitOr);
        case ILOpCode::Pop: {
            flush_expression_stack();  // drop only the value, not its side effects
            pop();
            auto* nop = make<Nop>();
            nop->kind = NopKind::Pop;
            return nop;
        }
        case ILOpCode::Rem: return binary_numeric(BinaryNumericOperator::Rem, false, Sign::Signed);
        case ILOpCode::Rem_un: return binary_numeric(BinaryNumericOperator::Rem, false, Sign::Unsigned);
        case ILOpCode::Ret: return return_();
        case ILOpCode::Shl: return binary_numeric(BinaryNumericOperator::ShiftLeft, false, Sign::None);
        case ILOpCode::Shr: return binary_numeric(BinaryNumericOperator::ShiftRight, false, Sign::Signed);
        case ILOpCode::Shr_un: return binary_numeric(BinaryNumericOperator::ShiftRight, false, Sign::Unsigned);
        case ILOpCode::Starg:
        case ILOpCode::Starg_s: return starg(disassembler::decode_index(reader_, op_code));
        case ILOpCode::Stind_i1: return st_ind(StackType::I4, KnownTypeCode::SByte);
        case ILOpCode::Stind_i2: return st_ind(StackType::I4, KnownTypeCode::Int16);
        case ILOpCode::Stind_i4: return st_ind(StackType::I4, KnownTypeCode::Int32);
        case ILOpCode::Stind_i8: return st_ind(StackType::I8, KnownTypeCode::Int64);
        case ILOpCode::Stind_r4: return st_ind(StackType::F4, KnownTypeCode::Single);
        case ILOpCode::Stind_r8: return st_ind(StackType::F8, KnownTypeCode::Double);
        case ILOpCode::Stind_i: return st_ind(StackType::I, KnownTypeCode::IntPtr);
        case ILOpCode::Stind_ref: return st_ind(StackType::O, KnownTypeCode::Object);
        case ILOpCode::Stloc:
        case ILOpCode::Stloc_s: return stloc(disassembler::decode_index(reader_, op_code));
        case ILOpCode::Stloc_0: return stloc(0);
        case ILOpCode::Stloc_1: return stloc(1);
        case ILOpCode::Stloc_2: return stloc(2);
        case ILOpCode::Stloc_3: return stloc(3);
        case ILOpCode::Sub: return binary_numeric(BinaryNumericOperator::Sub, false, Sign::None);
        case ILOpCode::Sub_ovf: return binary_numeric(BinaryNumericOperator::Sub, true, Sign::Signed);
        case ILOpCode::Sub_ovf_un: return binary_numeric(BinaryNumericOperator::Sub, true, Sign::Unsigned);
        case ILOpCode::Switch: return decode_switch();
        case ILOpCode::Xor: return binary_numeric(BinaryNumericOperator::BitXor);
        case ILOpCode::Box: {
            const IType* type = read_and_decode_type_reference();
            return push(make<Box>(pop(get_stack_type(*type)), type));
        }
        case ILOpCode::Castclass: {
            ILInstruction* argument = pop(StackType::O);
            return push(make<CastClass>(argument, read_and_decode_type_reference()));
        }
        case ILOpCode::Cpobj: {
            const IType* type = read_and_decode_type_reference();
            // the target runs before the value's ldobj
            auto* ld = make<LdObj>(pop_pointer(), type);
            return make<StObj>(pop_st_obj_target(), ld, type);
        }
        case ILOpCode::Initobj: {
            ILInstruction* target = pop_st_obj_target();
            return init_obj(target, read_and_decode_type_reference());
        }
        case ILOpCode::Isinst: {
            const IType* type = read_and_decode_type_reference();
            if (type->is_reference_type() != true) flush_expression_stack();  // a value-type isinst limits inlining
            return push(make<IsInst>(pop(StackType::O), type));
        }
        case ILOpCode::Ldelem: return ld_elem(read_and_decode_type_reference());
        case ILOpCode::Ldelem_i1: return ld_elem(compilation_.find_type(KnownTypeCode::SByte));
        case ILOpCode::Ldelem_i2: return ld_elem(compilation_.find_type(KnownTypeCode::Int16));
        case ILOpCode::Ldelem_i4: return ld_elem(compilation_.find_type(KnownTypeCode::Int32));
        case ILOpCode::Ldelem_i8: return ld_elem(compilation_.find_type(KnownTypeCode::Int64));
        case ILOpCode::Ldelem_u1: return ld_elem(compilation_.find_type(KnownTypeCode::Byte));
        case ILOpCode::Ldelem_u2: return ld_elem(compilation_.find_type(KnownTypeCode::UInt16));
        case ILOpCode::Ldelem_u4: return ld_elem(compilation_.find_type(KnownTypeCode::UInt32));
        case ILOpCode::Ldelem_r4: return ld_elem(compilation_.find_type(KnownTypeCode::Single));
        case ILOpCode::Ldelem_r8: return ld_elem(compilation_.find_type(KnownTypeCode::Double));
        case ILOpCode::Ldelem_i: return ld_elem(compilation_.find_type(KnownTypeCode::IntPtr));
        case ILOpCode::Ldelem_ref: return ld_elem(compilation_.find_type(KnownTypeCode::Object));
        case ILOpCode::Ldelema: {
            // LdElema runs the array before the indices: the IL's order
            ILInstruction* indices = pop();
            ILInstruction* array = pop();
            return push(make<LdElema>(read_and_decode_type_reference(), array, std::vector<ILInstruction*>{indices}));
        }
        case ILOpCode::Ldfld: {
            const IField* field = read_and_decode_field_reference();
            auto* address = make<LdFlda>(pop_ld_fld_target(field), field);
            address->delay_exceptions = true;
            return push(make<LdObj>(address, field->type()));
        }
        case ILOpCode::Ldflda: {
            const IField* field = read_and_decode_field_reference();
            return push(make<LdFlda>(pop_field_target(field), field));
        }
        case ILOpCode::Stfld: {
            const IField* field = read_and_decode_field_reference();
            ILInstruction* value = pop(get_stack_type(*field->type()));
            auto* target = make<LdFlda>(pop_field_target(field), field);
            target->delay_exceptions = true;
            return make<StObj>(target, value, field->type());
        }
        case ILOpCode::Ldlen: return push(make<LdLen>(StackType::I, pop(StackType::O)));
        case ILOpCode::Ldobj: {
            ILInstruction* target = pop_pointer();
            return push(make<LdObj>(target, read_and_decode_type_reference()));
        }
        case ILOpCode::Ldsfld: {
            const IField* field = read_and_decode_field_reference();
            return push(make<LdObj>(make<LdsFlda>(field), field->type()));
        }
        case ILOpCode::Ldsflda: return push(make<LdsFlda>(read_and_decode_field_reference()));
        case ILOpCode::Stsfld: {
            const IField* field = read_and_decode_field_reference();
            ILInstruction* value = pop(get_stack_type(*field->type()));
            return make<StObj>(make<LdsFlda>(field), value, field->type());
        }
        case ILOpCode::Ldtoken: return push(ld_token(read_and_decode_metadata_token()));
        case ILOpCode::Ldvirtftn: {
            ILInstruction* argument = pop();
            return push(make<LdVirtFtn>(argument, read_and_decode_method_reference()));
        }
        case ILOpCode::Mkrefany: {
            ILInstruction* argument = pop_pointer();
            return push(make<MakeRefAny>(argument, read_and_decode_type_reference()));
        }
        case ILOpCode::Newarr: {
            const IType* type = read_and_decode_type_reference();
            return push(make<NewArr>(type, std::vector<ILInstruction*>{pop()}));
        }
        case ILOpCode::Refanytype: return push(make<RefAnyType>(pop()));
        case ILOpCode::Refanyval: {
            ILInstruction* argument = pop();
            return push(make<RefAnyValue>(argument, read_and_decode_type_reference()));
        }
        case ILOpCode::Rethrow: return make<Rethrow>();
        case ILOpCode::Sizeof: return push(make<SizeOf>(read_and_decode_type_reference()));
        case ILOpCode::Stelem: return st_elem(read_and_decode_type_reference());
        case ILOpCode::Stelem_i1: return st_elem(compilation_.find_type(KnownTypeCode::SByte));
        case ILOpCode::Stelem_i2: return st_elem(compilation_.find_type(KnownTypeCode::Int16));
        case ILOpCode::Stelem_i4: return st_elem(compilation_.find_type(KnownTypeCode::Int32));
        case ILOpCode::Stelem_i8: return st_elem(compilation_.find_type(KnownTypeCode::Int64));
        case ILOpCode::Stelem_r4: return st_elem(compilation_.find_type(KnownTypeCode::Single));
        case ILOpCode::Stelem_r8: return st_elem(compilation_.find_type(KnownTypeCode::Double));
        case ILOpCode::Stelem_i: return st_elem(compilation_.find_type(KnownTypeCode::IntPtr));
        case ILOpCode::Stelem_ref: return st_elem(compilation_.find_type(KnownTypeCode::Object));
        case ILOpCode::Stobj: {
            const IType* type = read_and_decode_type_reference();
            // the target runs before the value
            ILInstruction* value = pop(get_stack_type(*type));
            ILInstruction* target = pop_st_obj_target();
            return make<StObj>(target, value, type);
        }
        case ILOpCode::Throw: return make<Throw>(pop());
        case ILOpCode::Unbox: {
            ILInstruction* argument = pop();
            return push(make<Unbox>(argument, read_and_decode_type_reference()));
        }
        case ILOpCode::Unbox_any: {
            ILInstruction* argument = pop();
            return push(make<UnboxAny>(argument, read_and_decode_type_reference()));
        }
        default: return make<InvalidBranch>(std::format("Unknown opcode: 0x{:02X}", static_cast<int>(op_code)));
    }
}

StackType ILReader::Impl::peek_stack_type() const {
    if (!expression_stack_.empty()) return expression_stack_.back()->result_type();
    if (current_stack_.is_empty()) return StackType::Unknown;
    return current_stack_.peek()->stack_type;
}

ILReader::Impl::DecodedInstruction ILReader::Impl::push(ILInstruction* inst) {
    expression_stack_.push_back(inst);
    return DecodedInstruction(inst, true);
}

ILInstruction* ILReader::Impl::peek() {
    flush_expression_stack();
    if (current_stack_.is_empty()) {
        auto* invalid = make<InvalidExpression>("Stack underflow");
        invalid->set_il_range(Interval(reader_.offset(), reader_.offset()));
        return invalid;
    }
    return make<LdLoc>(current_stack_.peek());
}

ILInstruction* ILReader::Impl::pop() {
    if (!expression_stack_.empty()) {
        ILInstruction* inst = expression_stack_.back();
        expression_stack_.pop_back();
        return inst;
    }
    if (current_stack_.is_empty()) {
        auto* invalid = make<InvalidExpression>("Stack underflow");
        invalid->set_il_range(Interval(reader_.offset(), reader_.offset()));
        return invalid;
    }
    ILVariable* v = nullptr;
    current_stack_ = current_stack_.pop(&v);
    return make<LdLoc>(v);
}

ILInstruction* ILReader::Impl::pop(StackType expected_type) {
    ILInstruction* inst = pop();
    return cast(inst, expected_type, &owner_.warnings, reader_.offset());
}

ILInstruction* ILReader::cast(ILInstruction* inst, StackType expected_type, std::vector<std::string>* warnings, int il_offset) {
    auto warn = [&](const std::string& message) {
        if (warnings != nullptr) warnings->push_back(std::format("IL_{}: {}", hex4(il_offset), message));
    };
    if (expected_type != inst->result_type()) {
        if (auto* invalid = as<InvalidExpression>(inst)) {
            invalid->expected_result_type = expected_type;
        } else if (expected_type == StackType::I && inst->result_type() == StackType::I4) {
            // IL converts I4 to I implicitly
            inst = make<Conv>(inst, PrimitiveType::I, false, Sign::None);
        } else if (expected_type == StackType::I4 && inst->result_type() == StackType::I) {
            // C++/CLI sometimes converts the other way implicitly too
            inst = make<Conv>(inst, PrimitiveType::I4, false, Sign::None);
        } else if (expected_type == StackType::Unknown) {
            inst = make<Conv>(inst, PrimitiveType::Unknown, false, Sign::None);
        } else if (inst->result_type() == StackType::Ref) {
            // GC tracking stops implicitly (ldloca or ldsflda passed to a method expecting a native pointer)
            inst = make<Conv>(inst, PrimitiveType::I, false, Sign::None);
            switch (expected_type) {
                case StackType::I4: inst = make<Conv>(inst, PrimitiveType::I4, false, Sign::None); break;
                case StackType::I: break;
                case StackType::I8: inst = make<Conv>(inst, PrimitiveType::I8, false, Sign::None); break;
                default:
                    warn(std::format("Expected {}, but got {}", to_string(expected_type), to_string(StackType::Ref)));
                    inst = make<Conv>(inst, to_primitive_type(expected_type), false, Sign::None);
                    break;
            }
        } else if (expected_type == StackType::Ref) {
            // GC tracking starts implicitly (an object to an interior reference); the invalid cases are handled here too, as the last
            // branch's to_primitive_type cannot take Ref
            if (!is_integer_type(inst->result_type()) && inst->result_type() != StackType::O)
                warn(std::format("Expected {}, but got {}", to_string(expected_type), to_string(inst->result_type())));
            inst = make<Conv>(inst, PrimitiveType::Ref, false, Sign::None);
        } else if (expected_type == StackType::F8 && inst->result_type() == StackType::F4) {
            // IL converts F4 to F8 implicitly (they are the same in IL)
            inst = make<Conv>(inst, PrimitiveType::R8, false, Sign::Signed);
        } else if (expected_type == StackType::F4 && inst->result_type() == StackType::F8) {
            inst = make<Conv>(inst, PrimitiveType::R4, false, Sign::Signed);
        } else {
            warn(std::format("Expected {}, but got {}", to_string(expected_type), to_string(inst->result_type())));
            inst = make<Conv>(inst, to_primitive_type(expected_type), false, Sign::Signed);
        }
    }
    return inst;
}

ILInstruction* ILReader::Impl::pop_pointer() {
    ILInstruction* inst = pop();
    switch (inst->result_type()) {
        case StackType::I4:
        case StackType::I8:
        case StackType::Unknown: return make<Conv>(inst, PrimitiveType::I, false, Sign::None);
        case StackType::I:
        case StackType::Ref: return inst;
        default:
            warn("Expected native int or pointer, but got " + std::string(to_string(inst->result_type())));
            return make<Conv>(inst, PrimitiveType::I, false, Sign::None);
    }
}

ILInstruction* ILReader::Impl::pop_st_obj_target() {
    // stobj's target slot may not take an inlined ldelema or ldflda (StObj::check_target_slot)
    if (!expression_stack_.empty() && (is<LdElema>(expression_stack_.back()) || is<LdFlda>(expression_stack_.back()))) flush_expression_stack();
    return pop_pointer();
}

ILInstruction* ILReader::Impl::pop_field_target(const IField* field) {
    std::optional<bool> is_reference_type = field->declaring_type()->is_reference_type();
    if (is_reference_type == true) return pop(StackType::O);
    if (is_reference_type == false) return pop_pointer();
    // a field of a type that is not resolved
    StackType stack_type = peek_stack_type();
    if (stack_type == StackType::O || stack_type == StackType::Unknown) return pop();
    return pop_pointer();
}

ILInstruction* ILReader::Impl::pop_ld_fld_target(const IField* field) {
    std::optional<bool> is_reference_type = field->declaring_type()->is_reference_type();
    if (is_reference_type == true) return pop(StackType::O);
    if (is_reference_type == false) {
        // a field of a value type: ldfld takes temporaries
        if (peek_stack_type() == StackType::O || peek_stack_type() == StackType::Unknown) return make<AddressOf>(pop(), field->declaring_type());
        return pop_pointer();
    }
    // a field of a type that is not resolved
    if (peek_stack_type() == StackType::O || peek_stack_type() == StackType::Unknown) return pop();
    return pop_pointer();
}

ILInstruction* ILReader::Impl::return_() {
    if (method_return_stack_type_ == StackType::Void) return make<Leave>(main_container_);
    if (current_instruction_start_ == 0) {
        assert(expression_stack_.empty() && current_stack_.is_empty());
        return make<InvalidBranch>(
            "Method body consists only of 'ret', but nothing is being returned. Decompiled assembly might be a reference assembly.");
    }
    return make<Leave>(main_container_, pop(method_return_stack_type_));
}

ILInstruction* ILReader::Impl::decode_ldstr() { return make<LdStr>(util::utf8(disassembler::decode_user_string(reader_, metadata_))); }

ILInstruction* ILReader::Impl::ldarg(int v) {
    if (v >= 0 && v < static_cast<int>(parameter_variables_.size())) return make<LdLoc>(parameter_variables_[static_cast<std::size_t>(v)]);
    return make<InvalidExpression>(std::format("ldarg {} (out-of-bounds)", v));
}

ILInstruction* ILReader::Impl::ldarga(int v) {
    if (v >= 0 && v < static_cast<int>(parameter_variables_.size())) return make<LdLoca>(parameter_variables_[static_cast<std::size_t>(v)]);
    return make<InvalidExpression>(std::format("ldarga {} (out-of-bounds)", v));
}

ILInstruction* ILReader::Impl::starg(int v) {
    if (v >= 0 && v < static_cast<int>(parameter_variables_.size())) {
        ILVariable* variable = parameter_variables_[static_cast<std::size_t>(v)];
        return make<StLoc>(variable, pop(variable->stack_type));
    }
    flush_expression_stack();
    pop();
    return make<InvalidExpression>(std::format("starg {} (out-of-bounds)", v));
}

ILInstruction* ILReader::Impl::ldloc(int v) {
    if (v >= 0 && v < static_cast<int>(local_variables_.size())) return make<LdLoc>(local_variables_[static_cast<std::size_t>(v)]);
    return make<InvalidExpression>(std::format("ldloc {} (out-of-bounds)", v));
}

ILInstruction* ILReader::Impl::ldloca(int v) {
    if (v >= 0 && v < static_cast<int>(local_variables_.size())) return make<LdLoca>(local_variables_[static_cast<std::size_t>(v)]);
    return make<InvalidExpression>(std::format("ldloca {} (out-of-bounds)", v));
}

ILInstruction* ILReader::Impl::stloc(int v) {
    if (v >= 0 && v < static_cast<int>(local_variables_.size())) {
        ILVariable* variable = local_variables_[static_cast<std::size_t>(v)];
        auto* store = make<StLoc>(variable, pop(variable->stack_type));
        store->il_stack_was_empty = current_stack_is_empty();
        return store;
    }
    flush_expression_stack();
    pop();
    return make<InvalidExpression>(std::format("stloc {} (out-of-bounds)", v));
}

ILReader::Impl::DecodedInstruction ILReader::Impl::ld_elem(const IType* type) {
    ILInstruction* indices = pop();
    ILInstruction* array = pop();
    auto* address = make<LdElema>(type, array, std::vector<ILInstruction*>{indices});
    address->delay_exceptions = true;
    return push(make<LdObj>(address, type));
}

ILInstruction* ILReader::Impl::st_elem(const IType* type) {
    ILInstruction* value = pop(get_stack_type(*type));
    ILInstruction* index = pop();
    ILInstruction* array = pop();
    // the order is array, index, value
    auto* address = make<LdElema>(type, array, std::vector<ILInstruction*>{index});
    address->delay_exceptions = true;
    return make<StObj>(address, value, type);
}

ILInstruction* ILReader::Impl::init_obj(ILInstruction* target, const IType* type) {
    auto* value = make<DefaultValue>(type);
    value->il_stack_was_empty = current_stack_is_empty();
    return make<StObj>(target, value, type);
}

ILReader::Impl::DecodedInstruction ILReader::Impl::decode_constrained_call() {
    constrained_prefix_ = read_and_decode_type_reference();
    DecodedInstruction inst = decode_instruction();
    auto* call = as<CallInstruction>(inst.instruction);
    if (call != nullptr)
        assert(call->constrained_to == constrained_prefix_);
    else
        warn("Ignored invalid 'constrained' prefix");
    constrained_prefix_ = nullptr;
    return inst;
}

ILReader::Impl::DecodedInstruction ILReader::Impl::decode_tail_call() {
    DecodedInstruction inst = decode_instruction();
    if (auto* call = as<CallInstruction>(inst.instruction))
        call->is_tail = true;
    else
        warn("Ignored invalid 'tail' prefix");
    return inst;
}

ILReader::Impl::DecodedInstruction ILReader::Impl::decode_unaligned() {
    std::uint8_t alignment = reader_.read_byte();
    DecodedInstruction inst = decode_instruction();
    if (auto* sup = dynamic_cast<ISupportsUnalignedPrefix*>(inst.instruction))
        sup->set_unaligned_prefix(alignment);
    else
        warn("Ignored invalid 'unaligned' prefix");
    return inst;
}

ILReader::Impl::DecodedInstruction ILReader::Impl::decode_volatile() {
    DecodedInstruction inst = decode_instruction();
    if (auto* svp = dynamic_cast<ISupportsVolatilePrefix*>(inst.instruction))
        svp->set_is_volatile(true);
    else
        warn("Ignored invalid 'volatile' prefix");
    return inst;
}

ILReader::Impl::DecodedInstruction ILReader::Impl::decode_readonly() {
    DecodedInstruction inst = decode_instruction();
    if (auto* ldelema = as<LdElema>(inst.instruction))
        ldelema->is_read_only = true;
    else
        warn("Ignored invalid 'readonly' prefix");
    return inst;
}

ILReader::Impl::DecodedInstruction ILReader::Impl::decode_call(OpCode op_code) {
    const IMethod* method = read_and_decode_method_reference();
    // The arguments popped from the last; the array ends up in evaluation order.
    auto prepare_arguments = [&](bool first_argument_is_st_obj_target) {
        int first_argument = (op_code != OpCode::NewObj && !method->is_static()) ? 1 : 0;
        const auto& parameters = method->parameters();
        std::vector<ILInstruction*> arguments(static_cast<std::size_t>(first_argument) + parameters.size());
        const IType* type_of_this = constrained_prefix_ != nullptr ? constrained_prefix_ : method->declaring_type();
        StackType expected_stack_type = CallInstruction::expected_type_for_this_pointer(*method->declaring_type(), constrained_prefix_);
        bool requires_ld_obj_if_ref = first_argument == 1 && !first_argument_is_st_obj_target && owner_.use_ref_locals_for_accurate_order_of_evaluation &&
                                      expected_stack_type == StackType::Ref && type_of_this->is_reference_type() != false;
        for (int i = static_cast<int>(parameters.size()) - 1; i >= 0; --i) {
            if (requires_ld_obj_if_ref) flush_expression_stack();
            arguments[static_cast<std::size_t>(first_argument + i)] = pop(get_stack_type(*parameters[static_cast<std::size_t>(i)]->type()));
        }
        if (first_argument == 1) {
            ILInstruction* first_argument_instruction;
            if (first_argument_is_st_obj_target) {
                first_argument_instruction = pop_st_obj_target();
            } else {
                first_argument_instruction = pop(expected_stack_type);
                if (requires_ld_obj_if_ref) first_argument_instruction = make<LdObjIfRef>(first_argument_instruction, type_of_this);
            }
            arguments[0] = first_argument_instruction;
        }
        return arguments;
    };
    const IType* declaring_type = method->declaring_type();
    if (declaring_type->kind() == TypeKind::Array) {
        std::vector<ILInstruction*> arguments = prepare_arguments(false);
        const IType* element_type = dynamic_cast<const typesystem::ArrayType*>(declaring_type)->element_type();
        if (op_code == OpCode::NewObj) return push(make<NewArr>(element_type, arguments));
        const std::string name = method->name();
        if (name == "Set") {
            ILInstruction* target = arguments[0];
            std::vector<ILInstruction*> indices(arguments.begin() + 1, arguments.end() - 1);
            ILInstruction* value = arguments.back();
            // the order is target, indices, value
            auto* address = make<LdElema>(element_type, target, indices);
            address->delay_exceptions = true;
            return make<StObj>(address, value, element_type);
        }
        if (name == "Get") {
            std::vector<ILInstruction*> indices(arguments.begin() + 1, arguments.end());
            auto* address = make<LdElema>(element_type, arguments[0], indices);
            address->delay_exceptions = true;
            return push(make<LdObj>(address, element_type));
        }
        if (name == "Address") {
            std::vector<ILInstruction*> indices(arguments.begin() + 1, arguments.end());
            return push(make<LdElema>(element_type, arguments[0], indices));
        }
        warn("Unknown method called on array type: " + name);
        auto* call = CallInstruction::create(op_code, method);
        call->il_stack_was_empty = current_stack_is_empty();
        call->constrained_to = constrained_prefix_;
        call->arguments().add_range(arguments);
        if (call->result_type() != StackType::Void) return push(call);
        return call;
    }
    if (declaring_type->kind() == TypeKind::Struct && method->is_constructor() && !method->is_static() && op_code == OpCode::Call &&
        method->return_type()->kind() == TypeKind::Void) {
        // "call Struct.ctor(target, ...)" has no C# form; the nearest is `*target = new Struct(...);`, so the call is
        // "stobj Struct(target, newobj Struct.ctor(...))". This happens here, not in a transform, because stobj's target slot
        // limits inlining (no ldflda without delayed exceptions).
        std::vector<ILInstruction*> arguments = prepare_arguments(true);
        auto* newobj = make<NewObj>(method);
        newobj->il_stack_was_empty = current_stack_is_empty();
        newobj->constrained_to = constrained_prefix_;
        newobj->arguments().add_range(std::vector<ILInstruction*>(arguments.begin() + 1, arguments.end()));
        return make<StObj>(arguments[0], newobj, declaring_type);
    }
    std::vector<ILInstruction*> arguments = prepare_arguments(false);
    auto* call = CallInstruction::create(op_code, method);
    call->il_stack_was_empty = current_stack_is_empty();
    call->constrained_to = constrained_prefix_;
    call->arguments().add_range(arguments);
    if (call->result_type() != StackType::Void) return push(call);
    return call;
}

ILReader::Impl::DecodedInstruction ILReader::Impl::decode_call_indirect() {
    srm::EntityHandle token = read_and_decode_metadata_token();
    if (token.kind() != srm::HandleKind::StandaloneSignature) throw srm::BadImageFormatError(static_cast<std::uint64_t>(reader_.offset()), "Invalid calli metadata token");
    auto signature_handle = srm::StandaloneSignatureHandle(token);
    auto [header, fpt] = module_.decode_method_signature(signature_handle, generic_context_);
    ILInstruction* function_pointer = pop(StackType::I);
    int first_argument = header.is_instance() ? 1 : 0;
    const auto& parameter_types = fpt->parameter_types;
    std::vector<ILInstruction*> arguments(static_cast<std::size_t>(first_argument) + parameter_types.size());
    for (int i = static_cast<int>(parameter_types.size()) - 1; i >= 0; --i)
        arguments[static_cast<std::size_t>(first_argument + i)] = pop(get_stack_type(*parameter_types[static_cast<std::size_t>(i)]));
    if (first_argument == 1) arguments[0] = pop();
    // the arguments are in evaluation order (the reverse of the pops)
    auto* call = make<CallIndirect>(header.is_instance(), header.has_explicit_this(), fpt, function_pointer, arguments);
    if (call->result_type() != StackType::Void) return push(call);
    return call;
}

ILInstruction* ILReader::Impl::comparison(ComparisonKind kind, bool un) {
    if (!is_equality_or_inequality(kind) && peek_stack_type() == StackType::O) flush_expression_stack();
    ILInstruction* right = pop();
    ILInstruction* left = pop();
    // left runs before right: the IL's order
    if ((left->result_type() == StackType::O || left->result_type() == StackType::Ref) && is_integer_type(right->result_type())) {
        // C++/CLI sometimes compares object references with integers (also Ref == I in Unsafe.IsNullRef())
        if (right->result_type() == StackType::I4) right = make<Conv>(right, PrimitiveType::I, false, Sign::None);  // at least native int
        left = make<Conv>(left, to_primitive_type(right->result_type()), false, Sign::None);
    } else if ((right->result_type() == StackType::O || right->result_type() == StackType::Ref) && is_integer_type(left->result_type())) {
        if (left->result_type() == StackType::I4) left = make<Conv>(left, PrimitiveType::I, false, Sign::None);
        right = make<Conv>(right, to_primitive_type(left->result_type()), false, Sign::None);
    }
    auto make_explicit_conversion = [&](StackType source_type, StackType target_type, PrimitiveType conversion_type) {
        if (left->result_type() == source_type && right->result_type() == target_type)
            left = make<Conv>(left, conversion_type, false, Sign::None);
        else if (left->result_type() == target_type && right->result_type() == source_type)
            right = make<Conv>(right, conversion_type, false, Sign::None);
    };
    // the implicit integer conversions made explicit
    make_explicit_conversion(StackType::I4, StackType::I, PrimitiveType::I);
    make_explicit_conversion(StackType::I4, StackType::I8, PrimitiveType::I8);
    make_explicit_conversion(StackType::I, StackType::I8, PrimitiveType::I8);
    // ECMA-335 table 4: binary comparison or branch operations
    if (is_float_type(left->result_type()) && is_float_type(right->result_type())) {
        // the implicit F4 to F8 conversion made explicit
        if (left->result_type() != right->result_type()) make_explicit_conversion(StackType::F4, StackType::F8, PrimitiveType::R8);
        // for floats, 'un' means unordered
        if (un) return Comp::logic_not(make<Comp>(negate(kind), Sign::None, left, right));
        return make<Comp>(kind, Sign::None, left, right);
    }
    if (is_integer_type(left->result_type()) && is_integer_type(right->result_type()) && !is_equality_or_inequality(kind)) {
        // an integer comparison where the sign matters
        return make<Comp>(kind, un ? Sign::Unsigned : Sign::Signed, left, right);
    }
    if (left->result_type() == right->result_type()) {
        // integer equality, object reference or managed reference comparison
        return make<Comp>(kind, Sign::None, left, right);
    }
    warn(std::format("Invalid comparison between {} and {}", to_string(left->result_type()), to_string(right->result_type())));
    if (left->result_type() < right->result_type())
        left = make<Conv>(left, to_primitive_type(right->result_type()), false, Sign::Signed);
    else
        right = make<Conv>(right, to_primitive_type(left->result_type()), false, Sign::Signed);
    return make<Comp>(kind, Sign::None, left, right);
}

ILInstruction* ILReader::Impl::decode_comparison_branch(ILOpCode op_code, ComparisonKind kind, bool un) {
    int start = reader_.offset() - 1;  // these opcodes are one byte
    int target = disassembler::decode_branch_target(reader_, op_code);
    ILInstruction* condition = comparison(kind, un);
    condition->add_il_range(Interval(start, reader_.offset()));
    if (!is_invalid_branch(target)) {
        mark_branch_target(target);
        return make<IfInstruction>(condition, make<Branch>(target));
    }
    return make<IfInstruction>(condition, make<InvalidBranch>("Invalid branch target"));
}

ILInstruction* ILReader::Impl::decode_conditional_branch(ILOpCode op_code, bool negate_) {
    int target = disassembler::decode_branch_target(reader_, op_code);
    ILInstruction* condition = pop();
    ComparisonKind kind = negate_ ? ComparisonKind::Equality : ComparisonKind::Inequality;
    switch (condition->result_type()) {
        case StackType::O:
            // an explicit comparison with null
            condition = make<Comp>(kind, Sign::None, condition, make<LdNull>());
            break;
        case StackType::I:
            // an explicit comparison with 0
            condition = make<Comp>(kind, Sign::None, condition, make<Conv>(make<LdcI4>(0), PrimitiveType::I, false, Sign::None));
            break;
        case StackType::I8: condition = make<Comp>(kind, Sign::None, condition, make<LdcI8>(0)); break;
        case StackType::Ref: {
            // an explicit comparison with a null reference
            ILInstruction* left = make<Conv>(condition, PrimitiveType::I, false, Sign::None);
            condition = make<Comp>(kind, Sign::None, left, make<Conv>(make<LdcI4>(0), PrimitiveType::I, false, Sign::None));
            break;
        }
        case StackType::I4:
            if (negate_) condition = Comp::logic_not(condition);
            break;
        default:
            condition = make<Conv>(condition, PrimitiveType::I4, false, Sign::None);
            if (negate_) condition = Comp::logic_not(condition);
            break;
    }
    if (!is_invalid_branch(target)) {
        mark_branch_target(target);
        return make<IfInstruction>(condition, make<Branch>(target));
    }
    return make<IfInstruction>(condition, make<InvalidBranch>("Invalid branch target"));
}

ILInstruction* ILReader::Impl::decode_unconditional_branch(ILOpCode op_code, bool is_leave) {
    int target = disassembler::decode_branch_target(reader_, op_code);
    if (is_leave) {
        flush_expression_stack();
        current_stack_ = VariableStack();
    }
    if (!is_invalid_branch(target)) {
        mark_branch_target(target);
        return make<Branch>(target);
    }
    return make<InvalidBranch>("Invalid branch target");
}

void ILReader::Impl::mark_branch_target(int target_il_offset, bool is_fall_through) {
    flush_expression_stack();
    assert(is_fall_through || is_branch_target_[target_il_offset]);
    (void)is_fall_through;
    ImportedBlock* target_block = store_stack_for_offset(target_il_offset, current_stack_);
    assert(current_block_ != nullptr);
    current_block_->outgoing_edges.emplace_back(target_block, current_stack_);
}

void ILReader::Impl::flush_expression_stack() {
    assert(current_block_ != nullptr);
    for (ILInstruction* inst : expression_stack_) {
        assert(inst->result_type() != StackType::Void);
        const IType* type = find_type(compilation_, inst->result_type());
        ILVariable* v = make<ILVariable>(VariableKind::StackSlot, type, inst->result_type());
        v->has_generated_name = true;
        current_stack_ = current_stack_.push(v);
        auto* store = make<StLoc>(v, inst);
        store->set_il_range(*inst);
        current_block_->block->instructions.add(store);
    }
    expression_stack_.clear();
}

ILInstruction* ILReader::Impl::decode_switch() {
    std::vector<int> targets = disassembler::decode_switch_targets(reader_);
    auto* instr = make<SwitchInstruction>(pop(StackType::I4));
    for (std::size_t i = 0; i < targets.size(); ++i) {
        auto* section = make<SwitchSection>();
        section->set_labels(util::LongSet(static_cast<std::int64_t>(i)));
        int target = targets[i];
        if (!is_invalid_branch(target)) {
            mark_branch_target(target);
            section->set_body(make<Branch>(target));
        } else {
            section->set_body(make<InvalidBranch>("Invalid branch target"));
        }
        instr->sections.add(section);
    }
    auto* default_section = make<SwitchSection>();
    default_section->set_labels(util::LongSet(util::LongInterval(0, static_cast<std::int64_t>(targets.size()))).invert());
    default_section->set_body(make<Nop>());
    instr->sections.add(default_section);
    return instr;
}

ILReader::Impl::DecodedInstruction ILReader::Impl::binary_numeric(BinaryNumericOperator op, bool check_for_overflow, Sign sign) {
    ILInstruction* right = pop();
    ILInstruction* left = pop();
    // left runs before right: the IL's order
    if (op != BinaryNumericOperator::Add && op != BinaryNumericOperator::Sub) {
        // every Ref taken as I, the conversion made explicit
        if (left->result_type() == StackType::Ref) left = make<Conv>(left, PrimitiveType::I, false, Sign::None);
        if (right->result_type() == StackType::Ref) right = make<Conv>(right, PrimitiveType::I, false, Sign::None);
    }
    auto make_explicit_conversion = [&](StackType source_type, StackType target_type, PrimitiveType conversion_type) {
        if (left->result_type() == source_type && right->result_type() == target_type)
            left = make<Conv>(left, conversion_type, false, Sign::None);
        else if (left->result_type() == target_type && right->result_type() == source_type)
            right = make<Conv>(right, conversion_type, false, Sign::None);
    };
    if (op != BinaryNumericOperator::ShiftLeft && op != BinaryNumericOperator::ShiftRight) {
        make_explicit_conversion(StackType::I4, StackType::I, PrimitiveType::I);
        make_explicit_conversion(StackType::I4, StackType::I8, PrimitiveType::I8);
        make_explicit_conversion(StackType::I, StackType::I8, PrimitiveType::I8);
        make_explicit_conversion(StackType::F4, StackType::F8, PrimitiveType::R8);
    }
    return push(make<BinaryNumericInstruction>(op, left, right, check_for_overflow, sign));
}

ILInstruction* ILReader::Impl::decode_jmp() {
    const IMethod* method = read_and_decode_method_reference();
    // jmp becomes a tail call
    auto* call = make<Call>(method);
    call->is_tail = true;
    call->il_stack_was_empty = true;
    if (!method->is_static()) call->arguments().add(ldarg(0));
    for (std::size_t i = 0; i < method->parameters().size(); ++i) call->arguments().add(ldarg(call->arguments().count()));
    return make<Leave>(main_container_, call);
}

ILInstruction* ILReader::Impl::ld_token(srm::EntityHandle token) {
    if (is_type_kind(token.kind())) return make<LdTypeToken>(module_.resolve_type(token, generic_context_));
    if (is_member_kind(token.kind())) {
        if (auto* member = dynamic_cast<const IMember*>(module_.resolve_entity(token, generic_context_))) return make<LdMemberToken>(member);
    }
    throw srm::BadImageFormatError(static_cast<std::uint64_t>(reader_.offset()), "Invalid metadata token for ldtoken instruction.");
}

// ---- ILReader

ILReader::ILReader(const MetadataModule& module) : impl_(std::make_unique<Impl>(*this, module)) {}
ILReader::~ILReader() = default;

ILFunction* ILReader::read_il(srm::MethodDefinitionHandle method, const srm::MethodBodyBlock& body, GenericContext generic_context, ILFunctionKind kind,
                              const util::CancellationToken& cancellation_token) {
    return impl_->read_il(method, body, generic_context, kind, cancellation_token);
}

void ILReader::write_typed_il(srm::MethodDefinitionHandle method, const srm::MethodBodyBlock& body, ITextOutput& output, GenericContext generic_context,
                              const util::CancellationToken& cancellation_token) {
    impl_->write_typed_il(method, body, output, generic_context, cancellation_token);
}

}  // namespace cb::dotnet::decompiler::il
