// dotnet/decompiler/csharp/auto_event_decompiler.cpp -- see auto_event_decompiler.hpp.
#include "dotnet/decompiler/csharp/auto_event_decompiler.hpp"

#include <algorithm>
#include <array>
#include <string>
#include <string_view>
#include <vector>

#include "dotnet/decompiler/csharp/csharp_decompiler.hpp"
#include "dotnet/decompiler/csharp/syntax/attribute_section.hpp"
#include "dotnet/decompiler/csharp/syntax/event_declaration.hpp"
#include "dotnet/decompiler/csharp/syntax/type_system_ast_builder.hpp"
#include "dotnet/decompiler/decompile_run.hpp"
#include "dotnet/decompiler/il/instructions.hpp"
#include "dotnet/decompiler/metadata/method_semantics_lookup.hpp"
#include "dotnet/decompiler/typesystem/decompiler_type_system.hpp"
#include "dotnet/decompiler/typesystem/metadata_module.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::csharp::auto_event_decompiler {

namespace srm = cb::dotnet::metadata;

using il::Block;
using il::BlockContainer;
using il::Call;
using il::CastClass;
using il::Comp;
using il::ComparisonKind;
using il::ContainerKind;
using il::ILInstruction;
using il::ILVariable;
using il::VariableKind;
using typesystem::Accessibility;
using typesystem::DecompilerTypeSystem;
using typesystem::IAttribute;
using typesystem::IEvent;
using typesystem::IField;
using typesystem::IMethod;
using typesystem::MetadataModule;
using typesystem::NormalizeTypeVisitor;

namespace {

constexpr std::array<std::string_view, 3> attribute_types_to_remove_from_auto_event_accessors = {
    "System.Runtime.CompilerServices.CompilerGeneratedAttribute",
    "System.Diagnostics.DebuggerBrowsableAttribute",
    "System.Runtime.CompilerServices.MethodImplAttribute",
};

constexpr std::array<std::string_view, 2> attribute_types_to_remove_from_auto_event_fields = {
    "System.Runtime.CompilerServices.CompilerGeneratedAttribute",
    "System.Diagnostics.DebuggerBrowsableAttribute",
};

template <std::size_t N>
std::vector<const IAttribute*> without_attribute_types(const std::vector<const IAttribute*>& attributes,
                                                       const std::array<std::string_view, N>& attribute_types_to_remove) {
    std::vector<const IAttribute*> result;
    for (const IAttribute* a : attributes) {
        std::string full_name = a->attribute_type()->full_name();
        if (std::find(attribute_types_to_remove.begin(), attribute_types_to_remove.end(), full_name) == attribute_types_to_remove.end())
            result.push_back(a);
    }
    return result;
}

bool is_same_field(const IField* a, const IField* b) { return typesystem::member_equals(a->member_definition(), b->member_definition()); }

const IField* find_backing_field(const IEvent* ev) {
    const auto* module = dynamic_cast<const MetadataModule*>(ev->parent_module());
    const typesystem::ITypeDefinition* declaring_type = ev->declaring_type_definition();
    if (module == nullptr || declaring_type == nullptr) return nullptr;
    const auto& lookup = module->metadata_file()->property_and_event_backing_field_lookup();
    for (const IField* field : declaring_type->fields()) {
        if (field->metadata_token().is_nil()) continue;
        srm::EventDefinitionHandle event_handle;
        if (!lookup.is_event_backing_field(srm::FieldDefinitionHandle(field->metadata_token()), event_handle)) continue;
        if (event_handle != srm::EventDefinitionHandle(ev->metadata_token())) continue;
        return field->accessibility() == Accessibility::Private && field->is_static() == ev->is_static() ? field : nullptr;
    }
    return nullptr;
}

bool is_delegate_combine_method(const IMethod* method, bool is_add_accessor) {
    return method->is_static() && method->name() == (is_add_accessor ? "Combine" : "Remove") &&
           method->declaring_type()->full_name() == "System.Delegate";
}

bool is_compare_exchange_method(const IMethod* method) {
    return method->is_static() && method->name() == "CompareExchange" && method->declaring_type()->full_name() == "System.Threading.Interlocked";
}

bool match_ld_this_or_alias(ILInstruction* inst, const ILVariable* this_alias) {
    return inst->match_ld_this() || (this_alias != nullptr && inst->match_ld_loc(this_alias));
}

bool match_load_of_field(ILInstruction* inst, const IMethod* accessor, const IField* field, const ILVariable* this_alias = nullptr) {
    if (accessor->is_static()) {
        const IField* f = nullptr;
        return inst->match_lds_fld(&f) && is_same_field(f, field);
    }
    ILInstruction* target = nullptr;
    const IField* f = nullptr;
    return inst->match_ld_fld(&target, &f) && match_ld_this_or_alias(target, this_alias) && is_same_field(f, field);
}

bool match_address_of_field(ILInstruction* inst, const IMethod* accessor, const IField* field) {
    if (accessor->is_static()) {
        const IField* f = nullptr;
        return inst->match_lds_flda(&f) && is_same_field(f, field);
    }
    ILInstruction* target = nullptr;
    const IField* f = nullptr;
    return inst->match_ld_flda(&target, &f) && target->match_ld_this() && is_same_field(f, field);
}

bool match_ld_value_parameter(ILInstruction* inst) {
    ILVariable* v = nullptr;
    return inst->match_ld_loc(&v) && v->kind() == VariableKind::Parameter && v->index() == 0;
}

// Matches "(T)Delegate.Combine(comparand, value)" (Delegate.Remove for the remove accessor).
bool match_casted_delegate_combine_call(ILInstruction* inst, const IField* field, const ILVariable* comparand, bool is_add_accessor) {
    auto* castclass = il::as<CastClass>(inst);
    if (castclass == nullptr) return false;
    if (!NormalizeTypeVisitor::type_erasure().equivalent_types(castclass->type(), field->return_type())) return false;
    auto* call = il::as<Call>(castclass->argument());
    if (call == nullptr || !is_delegate_combine_method(call->method, is_add_accessor) || call->arguments().count() != 2) return false;
    return call->arguments()[0]->match_ld_loc(comparand) && match_ld_value_parameter(call->arguments()[1]);
}

// Matches "Interlocked.CompareExchange(ref this.field, valueArg, comparandArg)".
bool match_compare_exchange_call(ILInstruction* inst, const IMethod* accessor, const IField* field, const ILVariable* value_arg,
                                 const ILVariable* comparand_arg) {
    auto* call = il::as<Call>(inst);
    if (call == nullptr || !is_compare_exchange_method(call->method) || call->arguments().count() != 3) return false;
    return match_address_of_field(call->arguments()[0], accessor, field) && call->arguments()[1]->match_ld_loc(value_arg) &&
           call->arguments()[2]->match_ld_loc(comparand_arg);
}

// Matches the loop exit at the end of the compare-exchange loop body:
//   if (oldValue == comparand) break;
// i.e. "if (comp.o(ldloc oldValue == ldloc comparand)) leave loop" followed by the back-branch.
bool match_loop_exit(Block* loop_body, BlockContainer* loop, const ILVariable* old_value, const ILVariable* comparand) {
    ILInstruction* condition = nullptr;
    ILInstruction* true_inst = nullptr;
    if (!loop_body->instructions[loop_body->instructions.count() - 2]->match_if_instruction(&condition, &true_inst)) return false;
    auto* comp = il::as<Comp>(condition);
    if (comp == nullptr || comp->kind() != ComparisonKind::Equality) return false;
    if (!comp->left()->match_ld_loc(old_value) || !comp->right()->match_ld_loc(comparand)) return false;
    if (!true_inst->match_leave(static_cast<const BlockContainer*>(loop))) return false;
    return loop_body->instructions[loop_body->instructions.count() - 1]->match_branch(static_cast<const Block*>(loop_body));
}

// Matches the thread-safe compare-exchange loop generated by csc 4 and Roslyn (C# and VB):
//   T oldValue = this.field;
//   while (true) {
//       T comparand = oldValue;
//       T newValue = (T)Delegate.Combine(comparand, value);
//       oldValue = Interlocked.CompareExchange(ref this.field, newValue, comparand);
//       if (oldValue == comparand) break;
//   }
bool match_compare_exchange_loop(Block* body, BlockContainer* function_body, const IMethod* accessor, const IField* field, bool is_add_accessor) {
    if (body->instructions.count() != 3) return false;
    ILVariable* old_value = nullptr;
    ILInstruction* init = nullptr;
    if (!body->instructions[0]->match_st_loc(&old_value, &init)) return false;
    if (!match_load_of_field(init, accessor, field)) return false;
    auto* loop = il::as<BlockContainer>(body->instructions[1]);
    if (loop == nullptr || loop->kind() != ContainerKind::Loop || loop->blocks.count() != 1) return false;
    if (!body->instructions[2]->match_leave(static_cast<const BlockContainer*>(function_body))) return false;
    Block* loop_body = loop->entry_point();
    if (loop_body->instructions.count() != 5) return false;
    ILVariable* comparand = nullptr;
    ILInstruction* comparand_init = nullptr;
    if (!loop_body->instructions[0]->match_st_loc(&comparand, &comparand_init) || comparand == old_value) return false;
    if (!comparand_init->match_ld_loc(old_value)) return false;
    ILVariable* new_value = nullptr;
    ILInstruction* new_value_init = nullptr;
    if (!loop_body->instructions[1]->match_st_loc(&new_value, &new_value_init) || new_value == old_value || new_value == comparand) return false;
    if (!match_casted_delegate_combine_call(new_value_init, field, comparand, is_add_accessor)) return false;
    ILInstruction* compare_exchange_call = nullptr;
    if (!loop_body->instructions[2]->match_st_loc(static_cast<const ILVariable*>(old_value), &compare_exchange_call)) return false;
    if (!match_compare_exchange_call(compare_exchange_call, accessor, field, new_value, comparand)) return false;
    return match_loop_exit(loop_body, loop, old_value, comparand);
}

// Matches the compare-exchange loop generated by mcs, which passes the combined delegate directly to Interlocked.CompareExchange and
// uses the previous exchange result as comparand:
//   T oldValue = this.field;
//   while (true) {
//       T comparand = oldValue;
//       oldValue = Interlocked.CompareExchange(ref this.field, (T)Delegate.Combine(comparand, value), oldValue);
//       if (oldValue == comparand) break;
//   }
bool match_compare_exchange_loop_mcs(Block* body, BlockContainer* function_body, const IMethod* accessor, const IField* field, bool is_add_accessor) {
    if (body->instructions.count() != 3) return false;
    ILVariable* old_value = nullptr;
    ILInstruction* init = nullptr;
    if (!body->instructions[0]->match_st_loc(&old_value, &init)) return false;
    if (!match_load_of_field(init, accessor, field)) return false;
    auto* loop = il::as<BlockContainer>(body->instructions[1]);
    if (loop == nullptr || loop->kind() != ContainerKind::Loop || loop->blocks.count() != 1) return false;
    if (!body->instructions[2]->match_leave(static_cast<const BlockContainer*>(function_body))) return false;
    Block* loop_body = loop->entry_point();
    if (loop_body->instructions.count() != 4) return false;
    ILVariable* comparand = nullptr;
    ILInstruction* comparand_init = nullptr;
    if (!loop_body->instructions[0]->match_st_loc(&comparand, &comparand_init) || comparand == old_value) return false;
    if (!comparand_init->match_ld_loc(old_value)) return false;
    ILInstruction* compare_exchange_call = nullptr;
    if (!loop_body->instructions[1]->match_st_loc(static_cast<const ILVariable*>(old_value), &compare_exchange_call)) return false;
    auto* call = il::as<Call>(compare_exchange_call);
    if (call == nullptr || !is_compare_exchange_method(call->method) || call->arguments().count() != 3) return false;
    if (!match_address_of_field(call->arguments()[0], accessor, field)) return false;
    if (!match_casted_delegate_combine_call(call->arguments()[1], field, comparand, is_add_accessor)) return false;
    if (!call->arguments()[2]->match_ld_loc(static_cast<const ILVariable*>(old_value))) return false;
    return match_loop_exit(loop_body, loop, old_value, comparand);
}

// Matches the non-thread-safe shape generated by csc before 4.0 (the accessors are [MethodImpl(Synchronized)] instead):
//   this.field = (T)Delegate.Combine(this.field, value);
// mcs 2.x compiles the accessor as a compound assignment, evaluating 'this' once into a temporary (IL 'dup'), so the store may be
// preceded by "stloc S(ldloc this)" with both field accesses going through S.
bool match_simple_combine_assignment(Block* body, BlockContainer* function_body, const IMethod* accessor, const IField* field, bool is_add_accessor) {
    ILVariable* this_alias = nullptr;
    int pos = 0;
    if (!accessor->is_static() && body->instructions.count() == 3) {
        ILInstruction* alias_init = nullptr;
        if (!body->instructions[0]->match_st_loc(&this_alias, &alias_init) || !alias_init->match_ld_this()) return false;
        if (this_alias->store_count() != 1 || this_alias->load_count() != 2 || this_alias->address_count() != 0) return false;
        pos = 1;
    } else if (body->instructions.count() != 2) {
        return false;
    }
    ILInstruction* value = nullptr;
    if (accessor->is_static()) {
        const IField* f = nullptr;
        if (!body->instructions[pos]->match_sts_fld(&f, &value) || !is_same_field(f, field)) return false;
    } else {
        ILInstruction* target = nullptr;
        const IField* f = nullptr;
        if (!body->instructions[pos]->match_st_fld(&target, &f, &value) || !match_ld_this_or_alias(target, this_alias) || !is_same_field(f, field))
            return false;
    }
    if (!body->instructions[pos + 1]->match_leave(static_cast<const BlockContainer*>(function_body))) return false;
    auto* castclass = il::as<CastClass>(value);
    if (castclass == nullptr) return false;
    if (!NormalizeTypeVisitor::type_erasure().equivalent_types(castclass->type(), field->return_type())) return false;
    auto* call = il::as<Call>(castclass->argument());
    if (call == nullptr || !is_delegate_combine_method(call->method, is_add_accessor) || call->arguments().count() != 2) return false;
    return match_load_of_field(call->arguments()[0], accessor, field, this_alias) && match_ld_value_parameter(call->arguments()[1]);
}

bool is_automatic_accessor(const DecompilerTypeSystem& type_system, const IMethod* accessor, const IField* field, bool is_add_accessor,
                           const util::CancellationToken& cancellation_token) {
    Block* body = CSharpDecompiler::decompile_body_for_analysis(accessor, type_system, cancellation_token);
    if (body == nullptr) return false;
    auto* function_body = il::as<BlockContainer>(body->parent());
    if (function_body == nullptr || function_body->blocks.count() != 1) return false;
    return match_compare_exchange_loop(body, function_body, accessor, field, is_add_accessor) ||
           match_simple_combine_assignment(body, function_body, accessor, field, is_add_accessor) ||
           match_compare_exchange_loop_mcs(body, function_body, accessor, field, is_add_accessor);
}

bool is_automatic_event_uncached(const DecompilerTypeSystem& type_system, const IEvent* ev, const util::CancellationToken& cancellation_token,
                                 const IField** backing_field) {
    *backing_field = nullptr;
    if (ev->is_explicit_interface_implementation() || ev->declaring_type_definition() == nullptr) return false;
    const IMethod* add_accessor = ev->add_accessor();
    const IMethod* remove_accessor = ev->remove_accessor();
    if (add_accessor == nullptr || !add_accessor->has_body() || remove_accessor == nullptr || !remove_accessor->has_body()) return false;
    *backing_field = find_backing_field(ev);
    if (*backing_field == nullptr) return false;
    // ignore tuple element names, dynamic and nullability
    if (!NormalizeTypeVisitor::type_erasure().equivalent_types(ev->return_type(), (*backing_field)->return_type())) return false;
    return is_automatic_accessor(type_system, add_accessor, *backing_field, true, cancellation_token) &&
           is_automatic_accessor(type_system, remove_accessor, *backing_field, false, cancellation_token);
}

}  // namespace

bool is_automatic_event(const DecompilerTypeSystem& type_system, const IEvent* ev, DecompileRun& decompile_run,
                        const util::CancellationToken& cancellation_token, const IField** backing_field) {
    auto cached = decompile_run.automatic_events.find(ev);
    if (cached == decompile_run.automatic_events.end()) {
        const IField* field = nullptr;
        *backing_field = is_automatic_event_uncached(type_system, ev, cancellation_token, &field) ? field : nullptr;
        decompile_run.automatic_events.emplace(ev, *backing_field);
    } else {
        *backing_field = cached->second;
    }
    return *backing_field != nullptr;
}

void add_field_like_event_attributes(syntax::EventDeclaration* event_decl, syntax::TypeSystemAstBuilder& ast_builder, const IEvent* ev,
                                     const IField* backing_field) {
    event_decl->attributes().add_range(ast_builder.convert_attributes(
        without_attribute_types(ev->add_accessor()->get_attributes(), attribute_types_to_remove_from_auto_event_accessors), "method"));
    event_decl->attributes().add_range(ast_builder.convert_attributes(
        without_attribute_types(backing_field->get_attributes(), attribute_types_to_remove_from_auto_event_fields), "field"));
}

}  // namespace cb::dotnet::decompiler::csharp::auto_event_decompiler
