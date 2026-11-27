// dotnet/decompiler/il/transforms/transform_collection_and_object_initializers.cpp -- see
// transform_collection_and_object_initializers.hpp.
#include "dotnet/decompiler/il/transforms/transform_collection_and_object_initializers.hpp"

#include <algorithm>
#include <climits>
#include <optional>
#include <stdexcept>

#include "core/error.hpp"
#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/csharp/resolver/csharp_resolver.hpp"
#include "dotnet/decompiler/csharp/resolver/member_lookup.hpp"
#include "dotnet/decompiler/csharp/resolver/type_inference.hpp"
#include "dotnet/decompiler/il/semantic_helper.hpp"
#include "dotnet/decompiler/il/transforms/copy_propagation.hpp"
#include "dotnet/decompiler/il/transforms/delegate_construction.hpp"
#include "dotnet/decompiler/il/transforms/il_inlining.hpp"
#include "dotnet/decompiler/il/transforms/transform_display_class_usage.hpp"
#include "dotnet/decompiler/il/transforms/tuple_transform.hpp"
#include "dotnet/decompiler/nr_extensions.hpp"
#include "dotnet/decompiler/srm_extensions.hpp"
#include "dotnet/decompiler/typesystem/known_attributes.hpp"
#include "dotnet/decompiler/typesystem/known_type_reference.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::il::transforms {

using typesystem::KnownTypeCode;
using typesystem::TypeKind;

namespace {

// Whether `inst` or a node below it loads or takes the address of `variable`.
bool loads_variable(ILInstruction* inst, ILVariable* variable) {
    for (ILInstruction* descendant : inst->descendants()) {
        auto* with_variable = dynamic_cast<IInstructionWithVariableOperand*>(descendant);
        if (with_variable != nullptr && with_variable->variable() == variable && (is<LdLoc>(descendant) || is<LdLoca>(descendant))) return true;
    }
    return false;
}

const IType* get_return_type_from_instruction(ILInstruction* instruction) {
    if (auto* call = dynamic_cast<CallInstruction*>(instruction)) {
        if (!(call->op_code == OpCode::CallVirt || call->op_code == OpCode::Call)) return nullptr;
        return call->method->return_type();
    }
    if (auto* ldobj = as<LdObj>(instruction)) {
        if (auto* ldflda = as<LdFlda>(ldobj->target())) return ldflda->field()->return_type();
        return nullptr;
    }
    if (auto* stobj = as<StObj>(instruction)) {
        if (auto* ldflda = as<LdFlda>(stobj->target())) return ldflda->field()->return_type();
        return nullptr;
    }
    return nullptr;
}

bool is_accessor_accessible(const IMethod* setter, const csharp::resolver::CSharpResolver* resolve_context) {
    if (resolve_context == nullptr) return true;
    csharp::resolver::MemberLookup lookup(resolve_context->current_type_definition(), resolve_context->current_module());
    return lookup.is_accessible(setter, setter->declaring_type_definition() == resolve_context->current_type_definition());
}

bool can_be_used_in_initializer(const typesystem::IProperty* property, const csharp::resolver::CSharpResolver* resolve_context, AccessPathKind kind) {
    if (property->can_set() && (property->accessibility() == property->setter()->accessibility() || is_accessor_accessible(property->setter(), resolve_context)))
        return true;
    return kind != AccessPathKind::Setter;
}

bool is_method_applicable(const IMethod* method, const InstructionCollection<ILInstruction>& arguments, const IType* root_type,
                          csharp::resolver::CSharpResolver* resolver, const DecompilerSettings* settings) {
    if (method->is_static() && !method->is_extension_method()) return false;
    if (dynamic_cast<const typesystem::IProperty*>(method->accessor_owner()) != nullptr) return true;
    if (!(method->name() == "Add") || arguments.count() == 0) return false;
    if (method->is_extension_method()) {
        if (settings != nullptr && !settings->extension_methods_in_collection_initializers) return false;
        if (!resolver->can_transform_to_extension_method_call(method, true)) return false;
    }
    const IType* target_type = get_return_type_from_instruction(arguments[0]);
    if (target_type == nullptr) target_type = root_type;
    if (target_type == nullptr) return false;
    bool is_enumerable = false;
    for (const IType* i : typesystem::get_all_base_types(target_type)) {
        if (typesystem::is_known_type(i, KnownTypeCode::IEnumerable) || typesystem::is_known_type(i, KnownTypeCode::IEnumerableOfT)) {
            is_enumerable = true;
            break;
        }
    }
    if (!is_enumerable) return false;
    // CanInferTypeArgumentsFromParameters(method)
    if (method->type_parameters().empty()) return true;
    // always use unspecialized member, otherwise type inference fails
    const auto* definition = dynamic_cast<const IMethod*>(method->member_definition());
    // TODO upstream: this is not entirely correct... we need argument type information to resolve Add methods properly
    std::vector<semantics::ResolveResult*> argument_results;
    std::vector<const IType*> parameter_types;
    for (const typesystem::IParameter* p : definition->parameters()) {
        argument_results.push_back(make<semantics::ResolveResult>(p->type()));
        parameter_types.push_back(p->type());
    }
    bool success = false;
    csharp::resolver::TypeInference(resolver->compilation()).infer_type_arguments(definition->type_parameters(), argument_results, parameter_types, &success);
    return success;
}

}  // namespace

// ---- TransformCollectionAndObjectInitializers --------------------------------------------------------------------------------------

void TransformCollectionAndObjectInitializers::run(Block* block, int pos, StatementTransformContext& context) {
    if (!context.settings().object_or_collection_initializers) return;
    ILInstruction* inst = block->instructions[pos];
    // Match stloc(v, newobj)
    ILVariable* v = nullptr;
    ILInstruction* init_inst = nullptr;
    if (!inst->match_st_loc(&v, &init_inst) || (v->kind() != VariableKind::Local && v->kind() != VariableKind::StackSlot)) return;
    const IType* inst_type = nullptr;
    BlockKind block_kind = BlockKind::CollectionInitializer;
    int insertion_pos = init_inst->child_index();
    auto siblings = init_inst->parent()->children();
    const IMethod* current_method = context.function()->method;
    // we allow a castclass instruction to wrap the init instruction: this is needed, for example, for inherited record types used on
    // .NET runtimes (e.g., .NET 4.x), where covariant return types are not supported.
    ILInstruction* arg = nullptr;
    const IType* target_type = nullptr;
    if (init_inst->match_cast_class(&arg, &target_type)) {
        init_inst = arg;
    }
    if (auto* new_obj_inst = as<NewObj>(init_inst)) {
        if (new_obj_inst->il_stack_was_empty && v->kind() == VariableKind::Local &&
            !type_contains_init_only_properties(new_obj_inst->method->declaring_type_definition()) && !current_method->is_constructor() &&
            !decompiler::is_compiler_generated_or_is_in_compiler_generated_class(current_method)) {
            // on statement level (no other expressions on IL stack), prefer to keep local variables (but not stack slots), unless we are
            // in a constructor (where inlining object initializers might be critical for the base ctor call) or a compiler-generated
            // delegate method, which might be used in a query expression.
            return;
        }
        // Do not try to transform delegate construction. DelegateConstruction transform cannot deal with this.
        const IMethod* delegate_method = nullptr;
        ILInstruction* delegate_target = nullptr;
        const IType* delegate_type = nullptr;
        if (DelegateConstruction::match_delegate_construction(new_obj_inst, &delegate_method, &delegate_target, &delegate_type) || TransformDisplayClassUsage::is_potential_closure(context, new_obj_inst)) return;
        // Cannot build a collection/object initializer attached to an AnonymousTypeCreateExpression anon = new { A = 5 } { 3,4,5 } is
        // invalid syntax.
        if (decompiler::contains_anonymous_type(new_obj_inst->method->declaring_type())) return;
        // Tuples cannot have initializers
        std::vector<ILInstruction*> tuple_arguments;
        if (TupleTransform::match_tuple_construction(new_obj_inst, &tuple_arguments)) return;
        inst_type = new_obj_inst->method->declaring_type();
    } else if (auto* default_val = as<DefaultValue>(init_inst)) {
        inst_type = default_val->type();
    } else if (auto* c = as<Call>(init_inst); c != nullptr && typesystem::full_name_is(c->method, "System.Activator", "CreateInstance") &&
                                              c->method->type_arguments().size() == 1) {
        if (!context.settings().use_object_creation_of_generic_type_parameter) {
            return;
        }
        inst_type = c->method->type_arguments()[0];
        block_kind = BlockKind::ObjectInitializer;
    } else if (auto* ci = dynamic_cast<CallInstruction*>(init_inst); ci != nullptr && context.settings().with_expressions && is_record_clone_method_call(ci)) {
        inst_type = ci->method->declaring_type();
        block_kind = BlockKind::WithInitializer;
        if (ci->arguments().count() != 1) throw UsageError("Sequence contains no elements or more than one");
        init_inst = ci->arguments()[0];
    } else {
        const typesystem::ITypeDefinition* type_def = v->type()->get_definition();
        if (context.settings().with_expressions && type_def != nullptr && type_def->is_reference_type() == std::optional<bool>(false) && type_def->is_record()) {
            inst_type = v->type();
            block_kind = BlockKind::WithInitializer;
        } else {
            return;
        }
    }
    if (target_type != nullptr) {
        inst_type = target_type;
    }
    // Copy-propagate stack slot holding an 'ldloca' of the variable
    if (pos < block->instructions.count()) {
        auto* st_loc_stack = as<StLoc>(block->instructions[pos + 1]);
        if (st_loc_stack != nullptr && st_loc_stack->variable()->kind() == VariableKind::StackSlot && st_loc_stack->variable()->is_single_definition()) {
            auto* ld_loca = as<LdLoca>(st_loc_stack->value());
            if (ld_loca != nullptr && ld_loca->variable() == v) CopyPropagation::propagate(st_loc_stack, context);
        }
    }
    int initializer_items_count = 0;
    bool initializer_contains_init_only_items = false;
    possible_index_variables_.clear();
    current_path_.clear();
    is_collection_ = false;
    path_stack_.clear();
    path_stack_.emplace_back();
    // Detect initializer type by scanning the following statements each must be a callvirt with ldloc v as first argument if the
    // method is a setter we're dealing with an object initializer if the method is named Add and has at least 2 arguments we're dealing
    // with a collection/dictionary initializer
    while (pos + initializer_items_count + 1 < block->instructions.count() &&
           is_part_of_initializer(block->instructions, pos + initializer_items_count + 1, v, inst_type, &block_kind,
                                  &initializer_contains_init_only_items, context)) {
        initializer_items_count++;
    }
    // Do not convert the statements into an initializer if there's an incompatible usage of the initializer variable directly after the
    // possible initializer.
    if (!initializer_contains_init_only_items && is_method_call_on_variable(block->instructions[pos + initializer_items_count + 1], v)) return;
    // Calculate the correct number of statements inside the initializer: All index variables that were used in the initializer have Index
    // set to -1. We fetch the first unused variable from the list and remove all instructions after its first usage (i.e. the init store)
    // from the initializer.
    std::optional<int> index;
    for (const IndexVariable& info : possible_index_variables_) {
        if (info.index > -1 && (!index || info.index < *index)) index = info.index;
    }
    if (index) {
        initializer_items_count = *index - pos - 1;
    }
    // The initializer would be empty, there's nothing to do here.
    if (initializer_items_count <= 0) return;
    context.step("CollectionOrObjectInitializer", inst);
    // Create a new block and final slot (initializer target variable)
    auto* initializer_block = make<Block>(block_kind);
    ILVariable* final_slot = context.function()->register_variable(VariableKind::InitializerTarget, inst_type);
    initializer_block->set_final_instruction(make<LdLoc>(final_slot));
    initializer_block->instructions.add(make<StLoc>(final_slot, init_inst));
    // Move all instructions to the initializer block.
    for (int i = 1; i <= initializer_items_count; i++) {
        ILInstruction* item = block->instructions[i + pos];
        if (auto* call = dynamic_cast<CallInstruction*>(item)) {
            if (!(call->op_code == OpCode::CallVirt || call->op_code == OpCode::Call)) continue;
            ILInstruction* new_target = call->arguments()[0];
            for (ILInstruction* descendant : new_target->descendants()) {
                auto* load = dynamic_cast<IInstructionWithVariableOperand*>(descendant);
                if (load != nullptr && (is<LdLoc>(descendant) || is<LdLoca>(descendant)) && load->variable() == v) load->set_variable(final_slot);
            }
            initializer_block->instructions.add(call);
        } else if (auto* st_obj = as<StObj>(item)) {
            for (ILInstruction* descendant : st_obj->target()->descendants()) {
                auto* load = dynamic_cast<IInstructionWithVariableOperand*>(descendant);
                if (load != nullptr && (is<LdLoc>(descendant) || is<LdLoca>(descendant)) && load->variable() == v) load->set_variable(final_slot);
            }
            initializer_block->instructions.add(st_obj);
        } else if (auto* st_loc = as<StLoc>(item)) {
            initializer_block->instructions.add(st_loc);
        }
    }
    block->instructions.remove_range(pos + 1, initializer_items_count);
    siblings.set(insertion_pos, initializer_block);
    ILInlining::inline_if_possible(block, pos, context);
    context.end_step(initializer_block);
}

bool TransformCollectionAndObjectInitializers::type_contains_init_only_properties(const typesystem::ITypeDefinition* type_definition) {
    if (type_definition == nullptr) return false;
    for (const typesystem::IProperty* property : type_definition->properties()) {
        if (property->setter() != nullptr && property->setter()->is_init_only()) return true;
    }
    return false;
}

bool TransformCollectionAndObjectInitializers::is_record_clone_method_call(CallInstruction* ci) {
    const typesystem::ITypeDefinition* declaring = ci->method->declaring_type_definition();
    if (declaring == nullptr || !declaring->is_record()) return false;
    if (ci->method->name() != "<Clone>$") return false;
    if (ci->arguments().count() != 1) return false;

    return true;
}

bool TransformCollectionAndObjectInitializers::is_method_call_on_variable(ILInstruction* inst, ILVariable* variable) {
    if (inst->match_ld_loc_ref(static_cast<const ILVariable*>(variable))) return true;
    if (auto* call = dynamic_cast<CallInstruction*>(inst); call != nullptr && call->arguments().count() > 0 && !call->method->is_static())
        return is_method_call_on_variable(call->arguments()[0], variable);
    ILInstruction* target = nullptr;
    const IField* field = nullptr;
    ILInstruction* value = nullptr;
    if (inst->match_ld_fld(&target, &field) || inst->match_st_fld(&target, &field, &value) || inst->match_ld_flda(&target, &field))
        return is_method_call_on_variable(target, variable);
    return false;
}

TransformCollectionAndObjectInitializers::IndexVariable* TransformCollectionAndObjectInitializers::find_possible_index_variable(ILVariable* variable) {
    for (IndexVariable& entry : possible_index_variables_)
        if (entry.variable == variable) return &entry;
    return nullptr;
}

bool TransformCollectionAndObjectInitializers::add_to_top_of_path_stack(const AccessPathElement& element) {
    auto& top = path_stack_.back();
    for (const AccessPathElement& existing : top)
        if (existing == element) return false;
    top.push_back(element);
    return true;
}

bool TransformCollectionAndObjectInitializers::is_part_of_initializer(InstructionCollection<ILInstruction>& instructions, int pos, ILVariable* target,
                                                                      const IType* root_type, BlockKind* block_kind,
                                                                      bool* initializer_contains_init_only_items, StatementTransformContext& context) {
    // Include any stores to local variables that are single-assigned and do not reference the initializer-variable in the list of
    // possible index variables. Index variables are used to implement dictionary initializers.
    if (auto* stloc = as<StLoc>(instructions[pos]); stloc != nullptr && stloc->variable()->kind() == VariableKind::Local && stloc->variable()->is_single_definition()) {
        if (!context.settings().dictionary_initializers) return false;
        if (loads_variable(stloc->value(), target)) return false;
        if (find_possible_index_variable(stloc->variable()) != nullptr) throw std::invalid_argument("An item with the same key has already been added.");
        possible_index_variables_.push_back({stloc->variable(), stloc->child_index(), stloc->value()});
        return true;
    }
    AccessPathElement::AccessPath access = AccessPathElement::get_access_path(instructions[pos], root_type, &context.settings(), &context.csharp_resolver());
    if (access.kind == AccessPathKind::Invalid || target != access.target) return false;
    // Treat last element separately: Can either be an Add method call or property setter.
    AccessPathElement last_element = access.path.back();
    std::vector<AccessPathElement> new_path = access.path;
    new_path.pop_back();
    // Compare new path with current path:
    std::size_t min_len = std::min(current_path_.size(), new_path.size());
    std::size_t first_difference_index = 0;
    while (first_difference_index < min_len && new_path[first_difference_index] == current_path_[first_difference_index]) first_difference_index++;
    while (current_path_.size() > first_difference_index) {
        is_collection_ = false;
        current_path_.pop_back();
        path_stack_.pop_back();
    }
    while (current_path_.size() < new_path.size()) {
        AccessPathElement new_element = new_path[current_path_.size()];
        current_path_.push_back(new_element);
        if (is_collection_ || !add_to_top_of_path_stack(new_element)) return false;
        path_stack_.emplace_back();
    }

    auto mark_used_indices = [&]() {
        for (ILVariable* index : access.used_index_variables) {
            if (IndexVariable* item = find_possible_index_variable(index)) item->index = -1;
        }
    };

    switch (access.kind) {
        case AccessPathKind::Adder:
            is_collection_ = true;
            if (!path_stack_.back().empty()) return false;
            mark_used_indices();
            return true;
        case AccessPathKind::Setter:
            if (is_collection_ || !add_to_top_of_path_stack(last_element)) return false;
            if (!access.values || access.values->size() != 1 || !is_valid_object_initializer_target(current_path_)) return false;
            if (*block_kind != BlockKind::ObjectInitializer && *block_kind != BlockKind::WithInitializer) *block_kind = BlockKind::ObjectInitializer;
            if (auto* property = dynamic_cast<const typesystem::IProperty*>(last_element.member)) {
                if (property->setter() != nullptr && property->setter()->is_init_only()) *initializer_contains_init_only_items = true;
            }
            mark_used_indices();
            return true;
        default: return false;
    }
}

bool TransformCollectionAndObjectInitializers::is_valid_object_initializer_target(const std::vector<AccessPathElement>& path) {
    if (path.empty()) return true;
    const AccessPathElement& element = path.back();
    auto* p = dynamic_cast<const typesystem::IProperty*>(element.member);
    if (p == nullptr) return true;
    if (!p->is_indexer()) return true;
    if (path.size() >= 2) {
        const AccessPathElement& previous = path[path.size() - 2];
        return typesystem::NormalizeTypeVisitor::ignore_nullability_and_tuples().equivalent_types(previous.member->return_type(), element.member->declaring_type());
    }
    return false;
}

// ---- AccessPathElement -------------------------------------------------------------------------------------------------------------

std::string AccessPathElement::to_string() const {
    return "[" + (member != nullptr ? member->to_string() : std::string()) + ", " + (indices ? "ICSharpCode.Decompiler.IL.ILInstruction[]" : "") + "]";
}

namespace {

// ILInstructionMatchComparer.Equals
bool il_instructions_match(ILInstruction* x, ILInstruction* y) {
    if (x == y) return true;
    if (x == nullptr || y == nullptr) return false;
    return SemanticHelper::is_pure(x->flags()) && SemanticHelper::is_pure(y->flags()) && x->match(y).success();
}

}  // namespace

bool AccessPathElement::equals(const AccessPathElement& other) const {
    if (!(other.member == member || (member != nullptr && member_equals(member, other.member)))) return false;
    if (!indices && !other.indices) return true;
    if (!indices || !other.indices) return false;
    if (indices->size() != other.indices->size()) return false;
    for (std::size_t i = 0; i < indices->size(); i++)
        if (!il_instructions_match((*indices)[i], (*other.indices)[i])) return false;
    return true;
}

AccessPathElement::AccessPath AccessPathElement::get_access_path(ILInstruction* instruction, const IType* root_type, const DecompilerSettings* settings,
                                                                 csharp::resolver::CSharpResolver* resolver) {
    AccessPath result;
    std::vector<AccessPathElement>& path = result.path;
    ILVariable* target = nullptr;
    AccessPathKind kind = AccessPathKind::Invalid;
    std::optional<std::vector<ILInstruction*>>& values = result.values;
    ILInstruction* inst = instruction;
    std::vector<ILVariable*>& used_index_variables = result.used_index_variables;
    while (inst != nullptr) {
        // upstream's `goto default`
        bool invalid = false;
        if (auto* call = dynamic_cast<CallInstruction*>(inst)) {
            if (!(call->op_code == OpCode::CallVirt || call->op_code == OpCode::Call)) {
                invalid = true;
            } else {
                const IMethod* method = call->method;
                if (resolver != nullptr && !is_method_applicable(method, call->arguments(), root_type, resolver, settings)) {
                    invalid = true;
                } else {
                    inst = call->arguments()[0];
                    if (auto* ld_obj_if_ref = as<LdObjIfRef>(inst)) {
                        inst = ld_obj_if_ref->target();
                    }
                    if (method->is_accessor()) {
                        auto* property = dynamic_cast<const typesystem::IProperty*>(method->accessor_owner());
                        if (property != nullptr && !can_be_used_in_initializer(property, resolver, kind)) {
                            invalid = true;
                        } else {
                            bool is_getter = method->accessor_kind() == typesystem::MethodSemanticsAttributes::Getter;
                            std::vector<ILInstruction*> indices;
                            int take = call->arguments().count() - (is_getter ? 1 : 2);
                            for (int i = 1; i < call->arguments().count() && static_cast<int>(indices.size()) < take; i++) indices.push_back(call->arguments()[i]);
                            if (!indices.empty() && settings != nullptr && !settings->dictionary_initializers) {
                                invalid = true;
                            } else {
                                // Mark all index variables as used
                                for (ILInstruction* index : indices) {
                                    if (auto* with_variable = dynamic_cast<IInstructionWithVariableOperand*>(index)) used_index_variables.push_back(with_variable->variable());
                                }
                                path.insert(path.begin(), AccessPathElement(call->op_code, method->accessor_owner(), indices));
                            }
                        }
                    } else {
                        path.insert(path.begin(), AccessPathElement(call->op_code, method));
                    }
                    if (!invalid && !values) {
                        if (method->is_accessor()) {
                            kind = AccessPathKind::Setter;
                            values = std::vector<ILInstruction*>{call->arguments()[call->arguments().count() - 1]};
                        } else {
                            kind = AccessPathKind::Adder;
                            values = std::vector<ILInstruction*>();
                            for (int i = 1; i < call->arguments().count(); i++) values->push_back(call->arguments()[i]);
                            if (values->empty()) invalid = true;
                        }
                    }
                }
            }
        } else if (auto* ldobj = as<LdObj>(inst)) {
            auto* ldflda = as<LdFlda>(ldobj->target());
            if (ldflda != nullptr && (kind != AccessPathKind::Setter || !ldflda->field()->is_read_only())) {
                path.insert(path.begin(), AccessPathElement(ldobj->op_code, ldflda->field()));
                inst = ldflda->target();
            } else {
                invalid = true;
            }
        } else if (auto* ldobj_if_ref = as<LdObjIfRef>(inst)) {
            auto* ldflda = as<LdFlda>(ldobj_if_ref->target());
            if (ldflda != nullptr && (kind != AccessPathKind::Setter || !ldflda->field()->is_read_only())) {
                path.insert(path.begin(), AccessPathElement(ldobj_if_ref->op_code, ldflda->field()));
                inst = ldflda->target();
            } else if (auto* ldloca = as<LdLoca>(ldobj_if_ref->target())) {
                target = ldloca->variable();
                inst = nullptr;
            } else {
                invalid = true;
            }
        } else if (auto* stobj = as<StObj>(inst)) {
            if (auto* ldflda = as<LdFlda>(stobj->target())) {
                path.insert(path.begin(), AccessPathElement(stobj->op_code, ldflda->field()));
                inst = ldflda->target();
                if (!values) {
                    values = std::vector<ILInstruction*>{stobj->value()};
                    kind = AccessPathKind::Setter;
                }
            } else {
                invalid = true;
            }
        } else if (auto* ldloc = as<LdLoc>(inst)) {
            target = ldloc->variable();
            inst = nullptr;
        } else if (auto* ldloca2 = as<LdLoca>(inst)) {
            target = ldloca2->variable();
            inst = nullptr;
        } else if (auto* ldflda2 = as<LdFlda>(inst)) {
            path.insert(path.begin(), AccessPathElement(ldflda2->op_code, ldflda2->field()));
            inst = ldflda2->target();
        } else {
            invalid = true;
        }
        if (invalid) {
            kind = AccessPathKind::Invalid;
            inst = nullptr;
        }
    }
    if (kind != AccessPathKind::Invalid && values) {
        for (ILInstruction* value : *values) {
            if (loads_variable(value, target)) {
                kind = AccessPathKind::Invalid;
                break;
            }
        }
    }
    result.kind = kind;
    result.target = target;
    return result;
}

}  // namespace cb::dotnet::decompiler::il::transforms
