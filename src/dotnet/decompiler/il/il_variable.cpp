// dotnet/decompiler/il/il_variable.cpp -- see il_variable.hpp.
#include "dotnet/decompiler/il/il_variable.hpp"

#include <cassert>
#include <format>
#include <functional>
#include <stdexcept>

#include "core/error.hpp"
#include "dotnet/decompiler/il/il_type_extensions.hpp"
#include "dotnet/decompiler/il/instruction_output_il.hpp"
#include "dotnet/decompiler/il/instructions.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"

namespace cb::dotnet::decompiler::il {

bool is_local(VariableKind kind) {
    switch (kind) {
        case VariableKind::Local:
        case VariableKind::ExceptionLocal:
        case VariableKind::ForeachLocal:
        case VariableKind::UsingLocal:
        case VariableKind::PatternLocal:
        case VariableKind::PinnedLocal:
        case VariableKind::PinnedRegionLocal:
        case VariableKind::DisplayClassLocal:
            return true;
        default:
            return false;
    }
}

ILVariable::ILVariable(VariableKind kind, const IType* type, std::optional<int> index)
    : ILVariable(kind, type, type != nullptr ? get_stack_type(*type) : StackType::Unknown, index) {}

ILVariable::ILVariable(VariableKind kind, const IType* type, StackType stack_type, std::optional<int> index)
    : stack_type(stack_type), kind_(kind), type_(type), index_(index) {
    if (type == nullptr) throw std::invalid_argument("type");
    if (kind == VariableKind::Parameter) {
        initial_value_is_initialized_ = true;
        uses_initial_value_ = true;
    }
    check_invariant();
}

void ILVariable::set_kind(VariableKind value) {
    if (kind_ == VariableKind::Parameter) throw UsageError("Kind=Parameter cannot be changed!");
    if (index_.has_value() && is_local(value) && !is_local(kind_)) index_.reset();
    kind_ = value;
}

void ILVariable::set_type(const IType* value) {
    if (get_stack_type(*value) != stack_type)
        throw std::invalid_argument(std::format("Expected stack-type: {} may not be changed. Found: {}", il::to_string(stack_type),
                                                il::to_string(get_stack_type(*value))));
    type_ = value;
}

void ILVariable::check_invariant() const {
    switch (kind_) {
        case VariableKind::Local:
        case VariableKind::ForeachLocal:
        case VariableKind::PatternLocal:
        case VariableKind::PinnedLocal:
        case VariableKind::PinnedRegionLocal:
        case VariableKind::UsingLocal:
        case VariableKind::ExceptionLocal:
        case VariableKind::DisplayClassLocal:
            assert(!index_.has_value() || *index_ >= 0);
            break;
        case VariableKind::Parameter:
            assert(index_.has_value() && *index_ >= -1);
            break;
        case VariableKind::ExceptionStackSlot:
            assert(index_.has_value() && *index_ >= 0);
            break;
        default:
            break;
    }
}

namespace {
template <class T>
int add_instruction(std::vector<T*>& list, T* inst) {
    list.push_back(inst);
    return static_cast<int>(list.size()) - 1;
}
template <class T>
void remove_instruction(std::vector<T*>& list, int index, [[maybe_unused]] T* inst) {
    assert(list.at(static_cast<std::size_t>(index)) == inst);
    std::size_t index_to_move = list.size() - 1;
    list[static_cast<std::size_t>(index)] = list[index_to_move];
    list[static_cast<std::size_t>(index)]->set_index_in_variable_instruction_mapping(index);
    list.pop_back();
}
}  // namespace

void ILVariable::add_load_instruction(LdLoc* inst) { inst->index_in_load_instruction_list = add_instruction(load_instructions_, inst); }
void ILVariable::add_store_instruction(IStoreInstruction* inst) { inst->index_in_store_instruction_list = add_instruction(store_instructions_, inst); }
void ILVariable::add_address_instruction(LdLoca* inst) { inst->index_in_address_instruction_list = add_instruction(address_instructions_, inst); }
void ILVariable::remove_load_instruction(LdLoc* inst) { remove_instruction(load_instructions_, inst->index_in_load_instruction_list, inst); }
void ILVariable::remove_store_instruction(IStoreInstruction* inst) {
    remove_instruction(store_instructions_, inst->index_in_store_instruction_list, inst);
}
void ILVariable::remove_address_instruction(LdLoca* inst) {
    remove_instruction(address_instructions_, inst->index_in_address_instruction_list, inst);
}

void ILVariable::set_initial_value_is_initialized(bool value) {
    if (kind_ == VariableKind::Parameter && !value) throw UsageError("Cannot remove InitialValueIsInitialized from parameters");
    initial_value_is_initialized_ = value;
}

void ILVariable::set_uses_initial_value(bool value) {
    if (kind_ == VariableKind::Parameter && !value) throw UsageError("Cannot remove UsesInitialValue from parameters");
    uses_initial_value_ = value;
}

void ILVariable::write_definition_to(ITextOutput& output) const {
    if (is_ref_read_only) output.write("readonly ");
    switch (kind_) {
        case VariableKind::Local: output.write("local "); break;
        case VariableKind::PinnedLocal: output.write("pinned local "); break;
        case VariableKind::PinnedRegionLocal: output.write("PinnedRegion local "); break;
        case VariableKind::Parameter: output.write("param "); break;
        case VariableKind::ExceptionLocal: output.write("exception local "); break;
        case VariableKind::ExceptionStackSlot: output.write("exception stack "); break;
        case VariableKind::StackSlot: output.write("stack "); break;
        case VariableKind::InitializerTarget: output.write("initializer "); break;
        case VariableKind::ForeachLocal: output.write("foreach "); break;
        case VariableKind::UsingLocal: output.write("using "); break;
        case VariableKind::NamedArgument: output.write("named_arg "); break;
        case VariableKind::DisplayClassLocal: output.write("display_class local "); break;
        case VariableKind::PatternLocal: output.write("pattern local "); break;
        case VariableKind::DeconstructionInitTemporary: output.write("deconstruction init temporary "); break;
    }
    output.write_local_reference(name.value_or(""), static_cast<const ILVariable*>(this), true);
    output.write(" : ");
    il::write_to(*type_, output);
    output.write('(');
    if (kind_ == VariableKind::Parameter || kind_ == VariableKind::Local || kind_ == VariableKind::PinnedLocal ||
        kind_ == VariableKind::PinnedRegionLocal)
        // upstream formats an int? (null writes as nothing)
        output.write(std::format("Index={}, ", index_.has_value() ? std::to_string(*index_) : std::string()));
    output.write(std::format("LoadCount={}, AddressCount={}, StoreCount={})", load_count(), address_count(), store_count()));
    if (kind_ != VariableKind::Parameter) {
        output.write(initial_value_is_initialized_ ? " init" : " uninit");
        output.write(uses_initial_value_ ? " used" : " unused");
    }
    if (capture_scope != nullptr) {
        output.write(" captured in ");
        Block* entry = capture_scope->entry_point();
        output.write_local_reference(entry != nullptr ? entry->label() : std::string(), static_cast<const BlockContainer*>(capture_scope));
    }
    if (state_machine_field != nullptr) output.write(" from state-machine");
}

void ILVariable::write_to(ITextOutput& output) const { output.write_local_reference(name.value_or(""), static_cast<const ILVariable*>(this)); }

bool ILVariable::is_used_within(ILInstruction* inst) const {
    if (auto* iwvo = dynamic_cast<IInstructionWithVariableOperand*>(inst); iwvo != nullptr && iwvo->variable() == this) return true;
    for (ILInstruction* child : inst->children())
        if (is_used_within(child)) return true;
    return false;
}

bool ILVariableEqualityComparer::operator()(const ILVariable* x, const ILVariable* y) const {
    if (x == y) return true;
    if (x == nullptr || y == nullptr) return false;
    if (x->kind() == VariableKind::StackSlot || y->kind() == VariableKind::StackSlot) return false;
    if (x->kind() == VariableKind::PatternLocal || y->kind() == VariableKind::PatternLocal) return false;
    if (!(x->function() == y->function() && x->kind() == y->kind())) return false;
    if (x->index().has_value()) return x->index() == y->index();
    if (x->state_machine_field != nullptr)
        return y->state_machine_field != nullptr && x->state_machine_field->equals(y->state_machine_field);
    return false;
}

std::size_t ILVariableHash::operator()(const ILVariable* v) const {
    auto mix = [](std::size_t h, std::size_t x) { return h ^ (x + 0x9e3779b97f4a7c15ull + (h << 6) + (h >> 2)); };
    if (v->kind() == VariableKind::StackSlot || v->kind() == VariableKind::PatternLocal) return std::hash<const void*>{}(v);
    std::size_t h = mix(std::hash<const void*>{}(v->function()), static_cast<std::size_t>(v->kind()));
    if (v->index().has_value()) return mix(h, static_cast<std::size_t>(*v->index()));
    if (v->state_machine_field != nullptr) return mix(h, static_cast<std::size_t>(v->state_machine_field->hash_code()));
    return std::hash<const void*>{}(v);
}

bool ILVariableCollection::add(ILVariable* item) {
    if (item->function_ != nullptr) {
        if (item->function_ == scope_) return false;
        throw std::invalid_argument("Variable already belongs to another scope");
    }
    item->function_ = scope_;
    item->index_in_function_ = count();
    list_.push_back(item);
    return true;
}

void ILVariableCollection::clear() {
    for (ILVariable* v : list_) v->function_ = nullptr;
    list_.clear();
}

bool ILVariableCollection::contains(const ILVariable* item) const {
    assert(item->function_ != scope_ || list_[static_cast<std::size_t>(item->index_in_function_)] == item);
    return item->function_ == scope_;
}

bool ILVariableCollection::remove(ILVariable* item) {
    if (item->function_ != scope_) return false;
    assert(list_[static_cast<std::size_t>(item->index_in_function_)] == item);
    remove_at(item->index_in_function_);
    return true;
}

void ILVariableCollection::remove_at(int index) {
    auto i = static_cast<std::size_t>(index);
    list_[i]->function_ = nullptr;
    list_[i] = list_.back();
    list_[i]->index_in_function_ = index;
    list_.pop_back();
}

void ILVariableCollection::remove_dead() {
    auto should_remove = [](const ILVariable* v) {
        if (!v->is_dead()) return false;
        if (v->kind() == VariableKind::DisplayClassLocal) return false;
        if (v->kind() == VariableKind::Parameter)
            return v->index() == -1 && v->function()->kind() != ILFunctionKind::TopLevelFunction;
        return true;
    };
    for (int i = 0; i < count();) {
        if (should_remove(list_[static_cast<std::size_t>(i)]))
            remove_at(i);
        else
            ++i;
    }
}

}  // namespace cb::dotnet::decompiler::il
