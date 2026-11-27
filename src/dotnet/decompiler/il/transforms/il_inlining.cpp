// dotnet/decompiler/il/transforms/il_inlining.cpp -- see il_inlining.hpp.
#include "dotnet/decompiler/il/transforms/il_inlining.hpp"

#include <cassert>
#include <stdexcept>

#include "core/error.hpp"
#include "dotnet/decompiler/csharp/csharp_decompiler.hpp"
#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/il/semantic_helper.hpp"
#include "dotnet/decompiler/il/transforms/named_argument_transform.hpp"
#include "dotnet/decompiler/il/transforms/nullable_lifting_transform.hpp"
#include "dotnet/decompiler/typesystem/tuple_type.hpp"
#include "dotnet/decompiler/typesystem/synthetic_range_index_accessor.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::il::transforms {

using typesystem::KnownTypeCode;
using typesystem::MethodSemanticsAttributes;
using typesystem::ReferenceKind;
using typesystem::TypeKind;

namespace {

}  // namespace

// ---- the transform ------------------------------------------------------------------------------------------------------------------

void ILInlining::run(ILFunction* function, ILTransformContext& context) {
    for (ILInstruction* inst : function->descendants()) {
        if (auto* block = as<Block>(inst)) inline_all_in_block(function, block, options, context);
    }
    function->variables().remove_dead();
}

void ILInlining::run(Block* block, BlockTransformContext& context) {
    inline_all_in_block(context.function(), block, options, context);
}

void ILInlining::run(Block* block, int pos, StatementTransformContext& context) {
    InliningOptions options_here = options | options_for_block(block, pos, context);
    while (inline_one_if_possible(block, pos, options_here, context)) {
        // repeat inlining until nothing changes.
    }
}

InliningOptions ILInlining::options_for_block(Block* block, int pos, ILTransformContext& context) {
    InliningOptions options = InliningOptions::None;
    if (context.settings().aggressive_inlining || is_catch_when_block(block)) {
        options |= InliningOptions::Aggressive;
    } else {
        ILFunction* function = nullptr;
        for (ILInstruction* ancestor : block->ancestors()) {
            if (auto* f = as<ILFunction>(ancestor)) {
                function = f;
                break;
            }
        }
        ILInstruction* inst = block->instructions[pos];
        if (is_in_constructor_initializer(function, inst) || prefer_expressions_over_statements(function)) {
            options |= InliningOptions::Aggressive;
        }
    }
    if (!context.settings().use_ref_locals_for_accurate_order_of_evaluation) {
        options |= InliningOptions::AllowChangingOrderOfEvaluationForExceptions;
    }
    return options;
}

bool ILInlining::prefer_expressions_over_statements(ILFunction* function) {
    switch (function->kind()) {
        case ILFunctionKind::Delegate:
            for (const IParameter* p : function->parameters) {
                if (csharp::CSharpDecompiler::is_transparent_identifier(p->name())) return true;
            }
            return false;
        case ILFunctionKind::ExpressionTree: return true;
        default: return false;
    }
}

bool ILInlining::inline_all_in_block(ILFunction*, Block* block, InliningOptions options, ILTransformContext& context) {
    bool modified = false;
    auto& instructions = block->instructions;
    for (int i = instructions.count() - 1; i >= 0; i--) {
        if (is<StLoc>(instructions[i])) {
            if (inline_one_if_possible(block, i, options, context)) {
                modified = true;
                continue;
            }
        }
    }
    return modified;
}

bool ILInlining::is_in_constructor_initializer(ILFunction* function, ILInstruction* inst) {
    int ctor_call_start = function->chained_constructor_call_il_offset();
    if (inst->end_il_offset() > ctor_call_start) return false;
    ILInstruction* top_level_inst = nullptr;
    for (ILInstruction* instr : inst->ancestors()) {
        if (is<Block>(instr->parent())) top_level_inst = instr;
    }
    if (top_level_inst == nullptr) return false;
    return top_level_inst->end_il_offset() <= ctor_call_start;
}

bool ILInlining::is_catch_when_block(Block* block) {
    BlockContainer* container = BlockContainer::find_closest_container(block);
    if (container == nullptr) return false;
    auto* handler = as<TryCatchHandler>(container->parent());
    return handler != nullptr && handler->filter() == container;
}

int ILInlining::inline_into(Block* block, int pos, InliningOptions options, ILTransformContext& context) {
    if (pos >= block->instructions.count()) return 0;
    int count = 0;
    while (--pos >= 0) {
        if (inline_one_if_possible(block, pos, options, context))
            count++;
        else
            break;
    }
    return count;
}

bool ILInlining::inline_if_possible(Block* block, int pos, ILTransformContext& context) {
    return inline_one_if_possible(block, pos, InliningOptions::Aggressive, context);
}

bool ILInlining::inline_one_if_possible(Block* block, int pos, InliningOptions options, ILTransformContext& context) {
    context.cancellation_token.throw_if_cancellation_requested();
    auto* stloc = as<StLoc>(block->instructions[pos]);
    if (stloc == nullptr) return false;
    if (!variable_can_be_used_for_inlining(stloc->variable())) return false;
    // Inlining of small integer types might be semantically incorrect (the stloc might perform implicit truncation), but that cannot
    // be avoided this easily without breaking lots of tests.
    return inline_one(stloc, options, context);
}

bool ILInlining::variable_can_be_used_for_inlining(ILVariable* v) {
    if (v->kind() == VariableKind::PinnedLocal) return false;
    // ensure the variable is accessed only a single time
    if (v->store_count() != 1) return false;
    if (v->load_count() + v->address_count() != 1) return false;
    return true;
}

bool ILInlining::inline_one(StLoc* stloc, InliningOptions options, ILTransformContext& context) {
    ILVariable* v = stloc->variable();
    Block* block = as<Block>(stloc->parent());
    assert(block != nullptr);
    int pos = stloc->child_index();
    if (do_inline(v, stloc->value(), block->instructions.element_at_or_default(pos + 1), options, context)) {
        // Assign the ranges of the stloc instruction:
        stloc->value()->add_il_range(*stloc);
        // Remove the stloc instruction:
        assert(block->instructions[pos] == stloc);
        block->instructions.remove_at(pos);
        return true;
    } else if (v->load_count() == 0 && v->address_count() == 0) {
        // The variable is never loaded
        if (SemanticHelper::is_pure(stloc->value()->flags())) {
            // Remove completely if the instruction has no effects (except for reading locals)
            context.step("Remove dead store without side effects", stloc);
            block->instructions.remove_at(pos);
            return true;
        } else if (v->kind() == VariableKind::StackSlot) {
            context.step("Remove dead store, but keep expression", stloc);
            // Assign the ranges of the stloc instruction:
            stloc->value()->add_il_range(*stloc);
            // Remove the stloc, but keep the inner expression
            ILInstruction* kept_expression = stloc->value();
            stloc->replace_with(kept_expression);
            context.end_step(kept_expression);
            return true;
        }
    }
    return false;
}

// Inlines 'inlined_expression' into 'next', if possible. Does not check whether 'v' has only one use; the caller is expected to
// validate whether inlining 'v' has any effects on other uses of 'v'.
bool ILInlining::do_inline(ILVariable* v, ILInstruction* inlined_expression, ILInstruction* next, InliningOptions options,
                           ILTransformContext& context) {
    FindResult r = find_load_in_next(next, v, inlined_expression, options);
    if (r.type == FindResultType::Found || r.type == FindResultType::NamedArgument) {
        ILInstruction* load_inst = r.load_inst;
        // A stackalloc whose result is a pointer (rather than a Span<T>) is only valid C# as the initializer of a pointer-typed local.
        // Inlining it into any expression position retypes it as Span<T>, which breaks every pointer use (dereference, a
        // pointer-typed argument, a reinterpreting cast). Moving it into a local store keeps it a local initializer, and the
        // Span<T>/ReadOnlySpan<T> constructor is the one position where reinterpreting it as a span is exactly the point; everything
        // else must not inline.
        auto* inlined_block = as<Block>(inlined_expression);
        if (inlined_expression->result_type() == StackType::I &&
            (is<LocAlloc>(inlined_expression) || (inlined_block != nullptr && inlined_block->kind == BlockKind::StackAllocInitializer)) &&
            !is<StLoc>(load_inst->parent()) && !is_stack_alloc_span_constructor_argument(load_inst)) {
            return false;
        }
        if (load_inst->op_code == OpCode::LdLoca) {
            if (!is_generated_temporary_for_address_of(static_cast<LdLoca*>(load_inst), v, inlined_expression, options)) return false;
        } else {
            assert(load_inst->op_code == OpCode::LdLoc);
            bool aggressive = (options & InliningOptions::Aggressive) != InliningOptions::None;
            if (!aggressive && v->kind() != VariableKind::StackSlot && !non_aggressive_inline_into(next, r, inlined_expression, v)) {
                return false;
            }
        }

        if (r.type == FindResultType::NamedArgument) {
            NamedArgumentTransform::introduce_named_argument(r.call_argument, context);
            // Now that the argument is evaluated early, we can inline as usual
        }

        context.step("Inline variable '" + v->name.value_or("") + "'", inlined_expression);
        // Assign the ranges of the ldloc instruction:
        inlined_expression->add_il_range(*load_inst);

        ILInstruction* inlined_result;
        if (load_inst->op_code == OpCode::LdLoca) {
            // it was an ldloca instruction, so we need to use the pseudo-opcode 'addressof' to preserve the semantics of the
            // compiler-generated temporary
            assert(static_cast<LdLoca*>(load_inst)->variable() == v);
            inlined_result = make<AddressOf>(inlined_expression, v->type());
            load_inst->replace_with(inlined_result);
        } else {
            inlined_result = inlined_expression;
            load_inst->replace_with(inlined_expression);
        }
        context.end_step(inlined_result);
        return true;
    }
    return false;
}

// Whether load_inst is the pointer argument of a `new Span<T>(pointer, length)` / `new ReadOnlySpan<T>(pointer, length)` constructor,
// the one position where a pointer-typed stackalloc may be inlined (TransformSpanTCtorContainingStackAlloc turns it into a span
// stackalloc afterwards).
bool ILInlining::is_stack_alloc_span_constructor_argument(ILInstruction* load_inst) {
    auto* new_obj = as<NewObj>(load_inst->parent());
    return new_obj != nullptr && new_obj->arguments().count() == 2 && new_obj->arguments()[0] == load_inst &&
           (typesystem::is_known_type(new_obj->method->declaring_type(), KnownTypeCode::SpanOfT) ||
            typesystem::is_known_type(new_obj->method->declaring_type(), KnownTypeCode::ReadOnlySpanOfT));
}

// Whether this is a temporary variable generated by the C# compiler for instance method calls on value type values.
bool ILInlining::is_generated_temporary_for_address_of(LdLoca* load_inst, [[maybe_unused]] ILVariable* v, ILInstruction* inlined_expression,
                                                       InliningOptions options) {
    assert(load_inst->variable() == v);
    if (!has_flag(options, InliningOptions::AllowInliningOfLdloca)) {
        return false;  // inlining of ldloca is not allowed in the early inlining stage
    }
    // Inlining a value type variable is allowed only if the resulting code will maintain the semantics that the method is operating on
    // a copy. Thus, we have to ensure we're operating on an r-value. Additionally, we cannot inline in cases where the C# compiler
    // prohibits the direct use of the rvalue (e.g. M(ref (MyStruct)obj); is invalid).
    const IMethod* method = nullptr;
    const IType* constrained_to = nullptr;
    if (is_used_as_this_pointer_in_call(load_inst, &method, &constrained_to)) {
        if (has_flag(options, InliningOptions::Aggressive)) {
            // Inlining might be required in ctor initializers (see #2714). expressionBuilder.VisitAddressOf will handle creating the
            // copy for us.
            return true;
        }

        switch (classify_expression(inlined_expression)) {
            case ExpressionClassification::RValue:
                // For struct method calls on rvalues, the C# compiler always generates temporaries.
                return true;
            case ExpressionClassification::MutableLValue:
                // For struct method calls on mutable lvalues, the C# compiler never generates temporaries.
                return false;
            case ExpressionClassification::ReadonlyLValue:
                // For struct method calls on readonly lvalues, the C# compiler only generates a temporary if it isn't a "readonly
                // struct"
                return method_requires_copy_for_readonly_lvalue(method, constrained_to);
            default: throw UsageError("invalid expression classification");
        }
    } else if (is<LdElemaInlineArray>(load_inst->parent())) {
        return true;
    } else if (is_passed_to_read_only_span_ctor(load_inst)) {
        // Always inlining is possible here, because it's an 'in' or 'ref readonly' parameter and the C# compiler allows calling it
        // with an rvalue, even though that might produce a warning. Note that we don't need to check the expression classification,
        // because expressionBuilder.VisitAddressOf will handle creating the copy for us. This is necessary, because there are
        // compiler-generated uses of this ctor when passing a single-element array to a params ROS<T> parameter and our following
        // transforms assume the value is already inlined.
        return true;
    } else if (is_passed_to_inline_array_as_span(load_inst)) {
        // Inlining is not allowed:
        // <PrivateImplementationDetails>.InlineArrayAsReadOnlySpan(GetInlineArray()) is invalid C# code
        return false;
    } else if (is_passed_to_in_parameter(load_inst)) {
        if (has_flag(options, InliningOptions::Aggressive)) {
            // Inlining might be required in ctor initializers (see #2714). expressionBuilder.VisitAddressOf will handle creating the
            // copy for us.
            return true;
        }

        switch (classify_expression(inlined_expression)) {
            case ExpressionClassification::RValue:
                // For rvalues passed to in parameters, the C# compiler generates a temporary.
                return true;
            case ExpressionClassification::MutableLValue:
            case ExpressionClassification::ReadonlyLValue:
                // For lvalues passed to in parameters, the C# compiler never generates temporaries.
                return false;
            default: throw UsageError("invalid expression classification");
        }
    } else if (is_used_as_this_pointer_in_field_read(load_inst)) {
        // mcs generated temporaries for field reads on rvalues (#1555)
        return classify_expression(inlined_expression) == ExpressionClassification::RValue;
    } else {
        return false;
    }
}

bool ILInlining::is_passed_to_inline_array_as_span(LdLoca* load_inst) {
    auto* call = as<Call>(load_inst->parent());
    if (call == nullptr) return false;
    const IMethod* method = call->method;
    const IType* declaring_type = method->declaring_type();
    if (declaring_type->reflection_name() != "<PrivateImplementationDetails>") return false;
    const std::string name = method->name();
    if (!(name == "InlineArrayAsReadOnlySpan" || name == "InlineArrayAsSpan")) return false;
    const auto& parameters = method->parameters();
    if (parameters.size() != 2) return false;
    const IParameter* arg = parameters[0];
    const IParameter* length = parameters[1];
    return (typesystem::is_known_type(method->return_type(), KnownTypeCode::SpanOfT) ||
            typesystem::is_known_type(method->return_type(), KnownTypeCode::ReadOnlySpanOfT)) &&
           arg->type()->kind() == TypeKind::ByReference && typesystem::is_known_type(length->type(), KnownTypeCode::Int32);
}

bool ILInlining::method_requires_copy_for_readonly_lvalue(const IMethod* method, const IType* constrained_to) {
    if (method == nullptr) return true;
    const IType* type = constrained_to != nullptr ? constrained_to : method->declaring_type();
    if (type->is_reference_type() == true) return false;  // reference types are never implicitly copied
    if (method->this_is_ref_read_only()) return false;     // no copies for calls on readonly structs
    return true;
}

bool ILInlining::is_used_as_this_pointer_in_call(LdLoca* ldloca) {
    const IMethod* method;
    const IType* constrained_type;
    return is_used_as_this_pointer_in_call(ldloca, &method, &constrained_type);
}

bool ILInlining::is_used_as_this_pointer_in_call(LdLoca* ldloca, const IMethod** method, const IType** constrained_type) {
    *method = nullptr;
    *constrained_type = nullptr;
    if (ldloca->variable()->type()->is_reference_type().value_or(false)) return false;
    ILInstruction* inst = ldloca;
    if (is<LdObjIfRef>(inst->parent())) {
        inst = inst->parent();
    }
    while (auto* ldflda = as<LdFlda>(inst->parent())) {
        inst = ldflda;
    }
    if (inst->child_index() != 0) return false;
    switch (inst->parent()->op_code) {
        case OpCode::Call:
        case OpCode::CallVirt: {
            auto* call_inst = static_cast<CallInstruction*>(inst->parent());
            *method = call_inst->method;
            *constrained_type = call_inst->constrained_to;
            if ((*method)->is_accessor()) {
                if ((*method)->accessor_kind() == MethodSemanticsAttributes::Getter) {
                    // C# doesn't allow property compound assignments on temporary structs
                    auto* cai = as<CompoundAssignmentInstruction>(inst->parent()->parent());
                    return !(cai != nullptr && cai->target_kind == CompoundTargetKind::Property && cai->target() == inst->parent());
                } else {
                    // C# doesn't allow calling setters on temporary structs
                    return false;
                }
            }
            return !(*method)->is_static();
        }
        case OpCode::Await: *method = static_cast<Await*>(inst->parent())->get_awaiter_method; return true;
        case OpCode::NullableUnwrap: return static_cast<NullableUnwrap*>(inst->parent())->ref_input;
        case OpCode::MatchInstruction: *method = static_cast<MatchInstruction*>(inst->parent())->method(); return true;
        default: return false;
    }
}

bool ILInlining::is_used_as_this_pointer_in_field_read(LdLoca* ldloca) {
    if (ldloca->variable()->type()->is_reference_type().value_or(false)) return false;
    ILInstruction* inst = ldloca;
    while (auto* ldflda = as<LdFlda>(inst->parent())) {
        inst = ldflda;
    }
    return inst != ldloca && is<LdObj>(inst->parent());
}

bool ILInlining::is_passed_to_in_parameter(LdLoca* ldloca) {
    auto* call = as<CallInstruction>(ldloca->parent());
    if (call == nullptr) return false;
    const IParameter* parameter = call->get_parameter(ldloca->child_index());
    return parameter != nullptr && parameter->reference_kind() == ReferenceKind::In;
}

bool ILInlining::is_passed_to_read_only_span_ctor(LdLoca* ldloca) {
    auto* call = as<NewObj>(ldloca->parent());
    if (call == nullptr) return false;
    const IMethod* method = call->method;
    return method->is_constructor() && method->parameters().size() == 1 &&
           typesystem::is_known_type(method->declaring_type(), KnownTypeCode::ReadOnlySpanOfT);
}

bool ILInlining::is_read_only_span_char_ctor(const IMethod* method) {
    if (!(method->is_constructor() && method->parameters().size() == 1 &&
          typesystem::is_known_type(method->declaring_type(), KnownTypeCode::ReadOnlySpanOfT)))
        return false;
    if (!typesystem::is_known_type(method->declaring_type()->type_arguments()[0], KnownTypeCode::Char)) return false;
    const IType* parameter_type = method->parameters()[0]->type();
    if (parameter_type->kind() != TypeKind::ByReference) return false;
    return typesystem::is_known_type(dynamic_cast<const typesystem::ByReferenceType*>(parameter_type)->element_type(), KnownTypeCode::Char);
}

ExpressionClassification ILInlining::classify_expression(ILInstruction* inst) {
    switch (inst->op_code) {
        case OpCode::LdLoc:
        case OpCode::StLoc: {
            ILVariable* v = as<IInstructionWithVariableOperand>(inst)->variable();
            if (v->is_ref_read_only || v->kind() == VariableKind::ForeachLocal || v->kind() == VariableKind::UsingLocal) {
                return ExpressionClassification::ReadonlyLValue;
            } else {
                return ExpressionClassification::MutableLValue;
            }
        }
        case OpCode::LdObj:
            // ldobj typically refers to a storage location, but readonly fields are an exception.
            if (is_readonly_reference(static_cast<LdObj*>(inst)->target())) {
                return ExpressionClassification::ReadonlyLValue;
            } else {
                return ExpressionClassification::MutableLValue;
            }
        case OpCode::StObj:
            // stobj is the same as ldobj.
            if (is_readonly_reference(static_cast<StObj*>(inst)->target())) {
                return ExpressionClassification::ReadonlyLValue;
            } else {
                return ExpressionClassification::MutableLValue;
            }
        case OpCode::Call: {
            const IMethod* m = static_cast<CallInstruction*>(inst)->method;
            // multi-dimensional array getters are lvalues, everything else is an rvalue.
            if (m->declaring_type()->kind() == TypeKind::Array) {
                return ExpressionClassification::MutableLValue;
            } else {
                return ExpressionClassification::RValue;
            }
        }
        default: return ExpressionClassification::RValue;  // most instructions result in an rvalue
    }
}

bool ILInlining::is_readonly_reference(ILInstruction* addr) {
    if (auto* ldflda = as<LdFlda>(addr)) {
        return ldflda->field()->is_read_only() ||
               (ldflda->field()->declaring_type()->kind() == TypeKind::Struct && is_readonly_reference(ldflda->target()));
    }
    if (auto* ldsflda = as<LdsFlda>(addr)) return ldsflda->field()->is_read_only();
    if (auto* ldloc = as<LdLoc>(addr)) return ldloc->variable()->is_ref_read_only;
    if (auto* call = as<Call>(addr)) return call->method->return_type_is_ref_read_only();
    if (auto* call_virt = as<CallVirt>(addr)) return call_virt->method->return_type_is_ref_read_only();
    if (auto* calli = as<CallIndirect>(addr)) return calli->function_pointer_type()->return_is_ref_read_only;
    if (is<AddressOf>(addr)) {
        // C# doesn't allow mutation of value-type temporaries
        return true;
    }
    ILInstruction* ld_fld_target;
    const IField* field;
    if (addr->match_ld_fld(&ld_fld_target, &field)) return field->return_type_is_ref_read_only();
    return false;
}

// Determines whether a variable should be inlined in non-aggressive mode, even though it is not a generated variable. `next` is the
// next top-level expression, `v` the variable being eliminated by inlining and `inlined_expression` the expression being inlined.
bool ILInlining::non_aggressive_inline_into(ILInstruction* next, const FindResult& find_result, ILInstruction* inlined_expression,
                                            ILVariable* v) {
    if (find_result.type == FindResultType::NamedArgument) {
        auto* original_store = as<StLoc>(inlined_expression->parent());
        return !original_store->il_stack_was_empty;
    }
    assert(find_result.type == FindResultType::Found);

    ILInstruction* load_inst = find_result.load_inst;
    assert(load_inst->is_descendant_of(next));

    // decide based on the source expression being inlined
    switch (inlined_expression->op_code) {
        case OpCode::DefaultValue:
        case OpCode::StObj:
        case OpCode::NumericCompoundAssign:
        case OpCode::UserDefinedCompoundAssign:
        case OpCode::Await:
        case OpCode::SwitchInstruction: return true;
        case OpCode::LdLoc:
            if (v->state_machine_field == nullptr && static_cast<LdLoc*>(inlined_expression)->variable()->state_machine_field != nullptr) {
                // Roslyn likes to put the result of fetching a state machine field into a temporary variable, so inline more
                // aggressively in such cases.
                return true;
            }
            break;
        default: break;
    }
    if (inlined_expression->result_type() == StackType::Ref) {
        // VB likes to use ref locals for compound assignment (the C# compiler uses ref stack slots instead). We want to avoid
        // unnecessary ref locals, so we'll always inline them if possible.
        return true;
    }

    ILInstruction* parent = load_inst->parent();
    const IType* ignored_type = nullptr;
    ILInstruction* ignored_arg = nullptr;
    if (NullableLiftingTransform::match_nullable_ctor(parent, &ignored_type, &ignored_arg)) {
        // inline into nullable ctor call in lifted operator
        parent = parent->parent();
    }
    if (auto* liftable = as<ILiftableInstruction>(parent)) {
        if (liftable->is_lifted()) return true;  // inline into lifted operators
    }
    // decide based on the new parent into which we are inlining:
    switch (parent->op_code) {
        case OpCode::NullCoalescingInstruction:
            if (typesystem::nullable_type::is_nullable(v->type())) return true;  // inline nullables into ?? operator
            break;
        case OpCode::NullableUnwrap: return true;  // inline into ?. operator
        case OpCode::UserDefinedLogicOperator:
        case OpCode::DynamicLogicOperatorInstruction: return true;  // inline into (left slot of) user-defined && or || operator
        case OpCode::DynamicGetMemberInstruction:
        case OpCode::DynamicGetIndexInstruction:
            if (parent->parent()->op_code == OpCode::DynamicCompoundAssign) return true;  // inline into dynamic compound assignments
            break;
        case OpCode::DynamicCompoundAssign: return true;
        case OpCode::GetPinnableReference:
        case OpCode::LocAllocSpan: return true;  // inline size-expressions into localloc.span
        case OpCode::Call:
        case OpCode::CallVirt:
            // Aggressive inline into property/indexer getter calls for compound assignment calls (The compiler generates locals for
            // these because it doesn't want to evaluate the args twice for getter+setter)
            if (parent->slot_info() == &CompoundAssignmentInstruction::target_slot) {
                return true;
            }
            if (dynamic_cast<const typesystem::SyntheticRangeIndexAccessor*>(static_cast<CallInstruction*>(parent)->method) != nullptr) {
                return true;
            }
            break;
        case OpCode::CallIndirect:
            if (load_inst->slot_info() == &CallIndirect::function_pointer_slot) return true;
            break;
        case OpCode::LdElema:
            if (static_cast<LdElema*>(parent)->with_system_index) {
                return true;
            }
            break;
        case OpCode::Leave:
        case OpCode::YieldReturn: return true;
        case OpCode::SwitchInstruction:
            // Preserve type info on switch instruction, if we're inlining a local variable into the switch-value slot.
            if (v->kind() != VariableKind::StackSlot && load_inst->slot_info() == &SwitchInstruction::value_slot) {
                auto* switch_inst = static_cast<SwitchInstruction*>(parent);
                if (switch_inst->type == nullptr) switch_inst->type = v->type();
            }
            return true;
        case OpCode::StringToInt:
            if (parent->slot_info() == &SwitchInstruction::value_slot) return true;
            break;
        case OpCode::MatchInstruction: {
            auto* match = static_cast<MatchInstruction*>(parent);
            if (match->is_deconstruct_tuple || (match->check_type && match->variable()->type()->is_reference_type() != true)) {
                return true;
            }
            break;
        }
        default: break;
    }
    // decide based on the top-level target instruction into which we are inlining:
    switch (next->op_code) {
        case OpCode::IfInstruction: {
            ILInstruction* logic_not_arg;
            while (parent->match_logic_not(&logic_not_arg)) {
                parent = parent->parent();
            }
            return parent == next;
        }
        default: return false;
    }
}

bool ILInlining::can_inline_into(ILInstruction* expr, ILVariable* v, ILInstruction* expression_being_moved) {
    return find_load_in_next(expr, v, expression_being_moved, InliningOptions::None).type == FindResultType::Found;
}

ILInlining::FindResult ILInlining::FindResult::found(ILInstruction* load_inst) {
    assert(load_inst->op_code == OpCode::LdLoc || load_inst->op_code == OpCode::LdLoca);
    return {FindResultType::Found, load_inst, nullptr};
}

ILInlining::FindResult ILInlining::FindResult::named_argument(ILInstruction* load_inst, ILInstruction* call_arg) {
    assert(load_inst->op_code == OpCode::LdLoc || load_inst->op_code == OpCode::LdLoca);
    assert(is<CallInstruction>(call_arg->parent()));
    return {FindResultType::NamedArgument, load_inst, call_arg};
}

ILInlining::FindResult ILInlining::FindResult::deconstruction(DeconstructInstruction* deconstruction) {
    return {FindResultType::Deconstruction, deconstruction, nullptr};
}

ILInlining::FindResult ILInlining::find_load_in_next(ILInstruction* expr, ILVariable* v, ILInstruction* expression_being_moved,
                                                      InliningOptions options) {
    if (expr == nullptr) return FindResult::stop();
    if (expr->match_ld_loc(v) || expr->match_ld_loca(v)) {
        // Match found, we can inline unless there are slot restrictions.
        if (expr->parent()->satisfies_slot_restriction_for_inlining(expr->child_index(), expression_being_moved)) {
            return FindResult::found(expr);
        }
        // We cannot inline because the targets slot restrictions are not satisfied
        if ((options & InliningOptions::AllowChangingOrderOfEvaluationForExceptions) != InliningOptions::None &&
            expr->slot_info() == &StObj::target_slot) {
            // Special case: inlining will change code semantics, but we accept that so that we can avoid a ref local.
            if (auto* ldflda = as<LdFlda>(expression_being_moved))
                ldflda->delay_exceptions = true;
            else if (auto* ldelema = as<LdElema>(expression_being_moved))
                ldelema->delay_exceptions = true;
            return FindResult::found(expr);
        }
        return FindResult::stop();
    } else if (auto* block = as<Block>(expr); block != nullptr && block->kind == BlockKind::CallWithNamedArgs) {
        return NamedArgumentTransform::can_extend_named_argument(block, v, expression_being_moved);
    } else if (has_flag(options, InliningOptions::FindDeconstruction) && is<DeconstructInstruction>(expr)) {
        return FindResult::deconstruction(static_cast<DeconstructInstruction*>(expr));
    }
    for (ILInstruction* child : expr->children()) {
        if (!expr->can_inline_into_slot(child->child_index(), expression_being_moved)) return FindResult::stop();

        // Recursively try to find the load instruction
        FindResult r = find_load_in_next(child, v, expression_being_moved, options);
        if (r.type != FindResultType::Continue) {
            if (r.type == FindResultType::Stop && (options & InliningOptions::IntroduceNamedArguments) != InliningOptions::None) {
                if (auto* call = as<CallInstruction>(expr))
                    return NamedArgumentTransform::can_introduce_named_argument(call, child, v, expression_being_moved);
            }
            return r;
        }
    }
    if (is_safe_for_inline_over(expr, expression_being_moved))
        return FindResult::continue_();  // continue searching
    else
        return FindResult::stop();  // abort, inlining not possible
}

// Whether it is safe to move 'expression_being_moved' past 'expr'.
bool ILInlining::is_safe_for_inline_over(ILInstruction* expr, ILInstruction* expression_being_moved) {
    return SemanticHelper::may_reorder(expression_being_moved, expr);
}

CallInstruction* ILInlining::find_first_inlined_call(ILInstruction* inst) {
    for (ILInstruction* child : inst->children()) {
        if (!child->slot_info()->can_inline_into) break;
        CallInstruction* call = find_first_inlined_call(child);
        if (call != nullptr) {
            return call;
        }
    }
    return as<CallInstruction>(inst);
}

bool ILInlining::can_move_into(ILInstruction* expression_being_moved, ILInstruction* stmt, ILInstruction* target_load) {
    assert(target_load->is_descendant_of(stmt));
    for (ILInstruction* inst = target_load; inst != stmt; inst = inst->parent()) {
        if (!inst->parent()->can_inline_into_slot(inst->child_index(), expression_being_moved)) return false;
        // Check whether re-ordering with predecessors is valid:
        int child_index = inst->child_index();
        for (int i = 0; i < child_index; ++i) {
            ILInstruction* predecessor = inst->parent()->get_child(i);
            if (!is_safe_for_inline_over(predecessor, expression_being_moved)) return false;
        }
    }
    return true;
}

bool ILInlining::can_move_into_call_virt(ILInstruction* expression_being_moved, ILInstruction* stmt, CallVirt* nested_call_virt,
                                         ILInstruction* target_load) {
    assert(target_load->is_descendant_of(stmt) && nested_call_virt->is_descendant_of(stmt));
    ILInstruction* this_arg = nested_call_virt->arguments()[0];
    assert(is<LdObjIfRef>(this_arg));
    for (ILInstruction* inst = target_load; inst != stmt; inst = inst->parent()) {
        if (!inst->parent()->can_inline_into_slot(inst->child_index(), expression_being_moved)) return false;
        // Check whether re-ordering with predecessors is valid:
        int child_index = inst->child_index();
        for (int i = 0; i < child_index; ++i) {
            ILInstruction* predecessor = inst->parent()->get_child(i);
            if (predecessor != this_arg && !is_safe_for_inline_over(predecessor, expression_being_moved)) return false;
        }
    }
    return true;
}

bool ILInlining::can_uninline(ILInstruction* arg, ILInstruction* stmt) {
    // moving into and moving out-of are equivalent
    return can_move_into(arg, stmt, arg);
}

}  // namespace cb::dotnet::decompiler::il::transforms
