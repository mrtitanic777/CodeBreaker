// dotnet/decompiler/il/transforms/transform_assignment.cpp -- see transform_assignment.hpp.
#include "dotnet/decompiler/il/transforms/transform_assignment.hpp"

#include <cassert>
#include <cstdint>
#include <stdexcept>
#include <vector>

#include "dotnet/decompiler/csharp/call_builder.hpp"
#include "dotnet/decompiler/csharp/expression_builder.hpp"
#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/il/il_instruction_extensions.hpp"
#include "dotnet/decompiler/il/pointer_arithmetic_offset.hpp"
#include "dotnet/decompiler/il/semantic_helper.hpp"
#include "dotnet/decompiler/il/transforms/il_inlining.hpp"
#include "dotnet/decompiler/typesystem/decompiler_type_system.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/typesystem/type_utils.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::il::transforms {

using typesystem::ByReferenceType;
using typesystem::ICompilation;
using typesystem::IProperty;
using typesystem::KnownTypeCode;
using typesystem::PointerType;
using typesystem::TypeKind;

namespace {

// ToCompound of the dynamic binary operations
ExpressionType to_compound(ExpressionType from) {
    switch (from) {
        case ExpressionType::Add: return ExpressionType::AddAssign;
        case ExpressionType::AddChecked: return ExpressionType::AddAssignChecked;
        case ExpressionType::And: return ExpressionType::AndAssign;
        case ExpressionType::Divide: return ExpressionType::DivideAssign;
        case ExpressionType::ExclusiveOr: return ExpressionType::ExclusiveOrAssign;
        case ExpressionType::LeftShift: return ExpressionType::LeftShiftAssign;
        case ExpressionType::Modulo: return ExpressionType::ModuloAssign;
        case ExpressionType::Multiply: return ExpressionType::MultiplyAssign;
        case ExpressionType::MultiplyChecked: return ExpressionType::MultiplyAssignChecked;
        case ExpressionType::Or: return ExpressionType::OrAssign;
        case ExpressionType::Power: return ExpressionType::PowerAssign;
        case ExpressionType::RightShift: return ExpressionType::RightShiftAssign;
        case ExpressionType::Subtract: return ExpressionType::SubtractAssign;
        case ExpressionType::SubtractChecked: return ExpressionType::SubtractAssignChecked;
        default: return from;
    }
}

// block.Instructions.ElementAtOrDefault(index)
ILInstruction* element_at_or_default(Block* block, int index) { return block->instructions.element_at_or_default(index); }

// variable.StoreInstructions.SingleOrDefault()
IStoreInstruction* single_store_or_default(const ILVariable* variable) {
    const auto& stores = variable->store_instructions();
    return stores.size() == 1 ? stores[0] : nullptr;
}

}  // namespace

void TransformAssignment::run(Block* block, int pos, StatementTransformContext& context) {
    context_ = &context;
    if (context.settings().make_assignment_expressions) {
        if (transform_inline_assignment_st_obj_or_call(block, pos) || transform_inline_assignment_local(block, pos)) {
            // both inline assignments create a top-level stloc which might affect inlining
            context.request_rerun();
            return;
        }
    }
    if (context.settings().introduce_increment_and_decrement) {
        if (transform_post_inc_dec_operator_with_inline_store(block, pos) || transform_post_inc_dec_operator(block, pos) ||
            transform_pre_inc_dec_operator_with_inline_store(block, pos)) {
            // again, new top-level stloc might need inlining:
            context.request_rerun();
            return;
        }
    }
}

// stloc s(value)
// stloc l(ldloc s)
// stobj(..., ldloc s)
//   where ... is pure and does not use s or l,
//   and where neither the 'stloc s' nor the 'stobj' truncates
// -->
// stloc l(stobj (..., value))
// e.g. used for inline assignment to instance field
//
// -or-
//
// stloc s(value)
// stobj (..., ldloc s)
//   where ... is pure and does not use s, and where the 'stobj' does not truncate
// -->
// stloc s(stobj (..., value))
// e.g. used for inline assignment to static field
//
// -or-
//
// stloc s(value)
// call set_Property(..., ldloc s)
//   where the '...' arguments are pure and not using 's'
// -->
// stloc s(Block InlineAssign { call set_Property(..., stloc i(value)); final: ldloc i })
//   new temporary 'i' has type of the property; transform only valid if 'stloc i' doesn't truncate
bool TransformAssignment::transform_inline_assignment_st_obj_or_call(Block* block, int pos) {
    StatementTransformContext& context = *context_;
    auto* inst = as<StLoc>(block->instructions[pos]);
    // in some cases it can be a compiler-generated local
    if (inst == nullptr || (inst->variable()->kind() != VariableKind::StackSlot && inst->variable()->kind() != VariableKind::Local)) return false;
    if (is_implicit_truncation(inst->value(), inst->variable()->type(), &context.type_system())) {
        // 'stloc s' is implicitly truncating the value
        return false;
    }
    ILVariable* local;
    int next_pos;
    StLoc* local_store = as<StLoc>(block->instructions[pos + 1]);
    if (local_store != nullptr) {  // with extra local
        if (local_store->variable()->kind() != VariableKind::Local || !local_store->value()->match_ld_loc(inst->variable())) return false;
        // if we're using an extra local, we'll delete "s", so check that that doesn't have any additional uses
        if (!(inst->variable()->is_single_definition() && inst->variable()->load_count() == 2)) return false;
        local = local_store->variable();
        next_pos = pos + 2;
    } else {
        local = inst->variable();
        local_store = nullptr;
        next_pos = pos + 1;

        if (local->load_count() == 1 && local->address_count() == 0) {
            // inline assignment would produce a dead store in this case, which is ugly
            // and causes problems with the deconstruction transform.
            return false;
        }
    }
    if (auto* stobj = as<StObj>(block->instructions[next_pos])) {
        // unaligned.stobj cannot be inlined in C#
        if (stobj->unaligned_prefix() > 0) return false;
        if (!stobj->value()->match_ld_loc(inst->variable())) return false;
        if (!SemanticHelper::is_pure(stobj->target()->flags()) || inst->variable()->is_used_within(stobj->target())) return false;
        const IType* pointer_type = infer_type(stobj->target(), &context.type_system());
        const IType* new_type = stobj->type();
        if (typesystem::is_compatible_pointer_type_for_memory_access(*pointer_type, *stobj->type())) {
            if (auto* byref = dynamic_cast<const ByReferenceType*>(pointer_type))
                new_type = byref->element_type();
            else if (auto* pointer = dynamic_cast<const PointerType*>(pointer_type))
                new_type = pointer->element_type();
        }
        ImplicitTruncationResult truncation = check_implicit_truncation(inst->value(), new_type, &context.type_system());
        if (truncation == ImplicitTruncationResult::ValueChanged) {
            // 'stobj' is implicitly truncating the value
            return false;
        }
        if (truncation == ImplicitTruncationResult::ValueChangedDueToSignMismatch) {
            // Change the sign of the type to skip implicit truncation
            new_type = swap_sign(new_type, context.type_system());
        }
        context.step("Inline assignment stobj", stobj);
        stobj->set_type(new_type);
        block->instructions.remove(local_store);
        block->instructions.remove(stobj);
        stobj->set_value(inst->value());
        auto* inline_assignment = make<StLoc>(local, stobj);
        inst->replace_with(inline_assignment);
        context.end_step(inline_assignment);
        // note: our caller will trigger a re-run, which will call HandleStObjCompoundAssign if applicable
        return true;
    } else if (auto* call = as<CallInstruction>(block->instructions[next_pos])) {
        // call must be a setter call:
        if (!(call->op_code == OpCode::Call || call->op_code == OpCode::CallVirt)) return false;
        if (call->result_type() != StackType::Void || call->arguments().count() == 0) return false;
        const IProperty* property = dynamic_cast<const IProperty*>(call->method->accessor_owner());
        if (property == nullptr) return false;
        if (!member_equals(call->method, property->setter())) return false;
        if (!(property->is_indexer() || property->setter()->parameters().size() == 1)) {
            // this is a parameterized property, which cannot be expressed as C# code.
            // setter calls are not valid in expression context, if property syntax cannot be used.
            return false;
        }
        if (!call->arguments().last()->match_ld_loc(inst->variable())) return false;
        for (int i = 0; i < call->arguments().count() - 1; ++i) {
            ILInstruction* arg = call->arguments()[i];
            if (!SemanticHelper::is_pure(arg->flags()) || inst->variable()->is_used_within(arg)) return false;
        }
        if (is_implicit_truncation(inst->value(), call->method->parameters().back()->type(), &context.type_system())) {
            // setter call is implicitly truncating the value
            return false;
        }
        // stloc s(Block InlineAssign { call set_Property(..., stloc i(value)); final: ldloc i })
        context.step("Inline assignment call", call);
        block->instructions.remove(local_store);
        block->instructions.remove(call);
        ILVariable* new_var = context.function()->register_variable(VariableKind::StackSlot, call->method->parameters().back()->type());
        call->arguments().set(call->arguments().count() - 1, make<StLoc>(new_var, inst->value()));
        auto* inline_block = make<Block>(BlockKind::CallInlineAssign);
        inline_block->instructions.add(call);
        inline_block->set_final_instruction(make<LdLoc>(new_var));
        auto* inline_assignment = make<StLoc>(local, inline_block);
        inst->replace_with(inline_assignment);
        context.end_step(inline_assignment);
        // because the ExpressionTransforms don't look into inline blocks, manually trigger HandleCallCompoundAssign
        if (handle_compound_assign(call, context)) {
            // if we did construct a compound assignment, it should have made our inline block redundant:
            assert(!inline_block->is_connected());
        }
        return true;
    } else {
        return false;
    }
}

const IType* TransformAssignment::swap_sign(const IType* type, const ICompilation& compilation) {
    switch (typesystem::to_primitive_type(*type)) {
        case PrimitiveType::I1: return compilation.find_type(KnownTypeCode::Byte);
        case PrimitiveType::I2: return compilation.find_type(KnownTypeCode::UInt16);
        case PrimitiveType::I4: return compilation.find_type(KnownTypeCode::UInt32);
        case PrimitiveType::I8: return compilation.find_type(KnownTypeCode::UInt64);
        case PrimitiveType::U1: return compilation.find_type(KnownTypeCode::SByte);
        case PrimitiveType::U2: return compilation.find_type(KnownTypeCode::Int16);
        case PrimitiveType::U4: return compilation.find_type(KnownTypeCode::Int32);
        case PrimitiveType::U8: return compilation.find_type(KnownTypeCode::Int64);
        case PrimitiveType::I: return compilation.find_type(KnownTypeCode::UIntPtr);
        case PrimitiveType::U: return compilation.find_type(KnownTypeCode::IntPtr);
        default: throw std::invalid_argument("Type must have an opposing sign: " + type->to_string());
    }
}

ILInstruction* TransformAssignment::unwrap_small_integer_conv(ILInstruction* inst, Conv** conv) {
    *conv = as<Conv>(inst);
    if (*conv != nullptr && (*conv)->kind == ConversionKind::Truncate && is_small_integer_type((*conv)->target_type)) {
        // for compound assignments to small integers, the compiler emits a "conv" instruction
        return (*conv)->argument();
    } else {
        return inst;
    }
}

bool TransformAssignment::validate_compound_assign(BinaryNumericInstruction* binary, Conv* conv, const IType* target_type,
                                                   const DecompilerSettings& settings) {
    if (!NumericCompoundAssign::is_binary_compatible_with_type(binary, target_type, &settings)) return false;
    if (conv != nullptr && !(conv->target_type == typesystem::to_primitive_type(*target_type) && conv->check_for_overflow == binary->check_for_overflow))
        return false;  // conv does not match binary operation
    return true;
}

bool TransformAssignment::matching_getter_and_setter_calls(CallInstruction* getter_call, CallInstruction* setter_call, FinalizeMatch* finalize_match) {
    *finalize_match = nullptr;
    if (getter_call == nullptr || setter_call == nullptr || !is_same_member(getter_call->method->accessor_owner(), setter_call->method->accessor_owner()))
        return false;
    if (setter_call->op_code != getter_call->op_code) return false;
    const IProperty* owner = dynamic_cast<const IProperty*>(getter_call->method->accessor_owner());
    if (owner == nullptr || !is_same_member(getter_call->method, owner->getter()) || !is_same_member(setter_call->method, owner->setter())) return false;
    if (setter_call->arguments().count() != getter_call->arguments().count() + 1) return false;
    // Ensure that same arguments are passed to getterCall and setterCall:
    for (int j = 0; j < getter_call->arguments().count(); j++) {
        ILVariable* v;
        if (setter_call->arguments()[j]->match_st_loc(&v) && v->is_single_definition() && v->load_count() == 1) {
            if (getter_call->arguments()[j]->match_ld_loc(v)) {
                // OK, setter call argument is saved in temporary that is re-used for getter call
                if (!*finalize_match) {
                    *finalize_match = [getter_call, setter_call](ILTransformContext&) {
                        assert(setter_call->arguments().count() == getter_call->arguments().count() + 1);
                        for (int k = 0; k < getter_call->arguments().count(); k++) {
                            ILVariable* v2;
                            ILInstruction* value;
                            if (setter_call->arguments()[k]->match_st_loc(&v2, &value)) {
                                assert(v2->is_single_definition() && v2->load_count() == 1);
                                assert(getter_call->arguments()[k]->match_ld_loc(v2));
                                getter_call->arguments().set(k, value);
                            }
                        }
                    };
                }
                continue;
            }
        }
        if (!SemanticHelper::is_pure(getter_call->arguments()[j]->flags())) return false;
        if (!getter_call->arguments()[j]->match(setter_call->arguments()[j]).success()) return false;
    }
    return true;
}

bool TransformAssignment::handle_compound_assign(ILInstruction* compound_store, StatementTransformContext& context) {
    if (!context.settings().make_assignment_expressions || !context.settings().introduce_increment_and_decrement) return false;
    if (is<CallInstruction>(compound_store) && compound_store->slot_info() != &Block::instruction_slot) {
        // replacing 'call set_Property' with a compound assignment instruction
        // changes the return value of the expression, so this is only valid on block-level.
        return false;
    }
    const IType* target_type;
    ILInstruction* setter_value;
    if (!is_compound_store(compound_store, &target_type, &setter_value, context.type_system())) return false;
    // targetType = The type of the property/field/etc. being stored to.
    // setterValue = The value being stored.
    auto* store_in_setter = as<StLoc>(setter_value);
    if (store_in_setter != nullptr) {
        // We'll move the stloc to top-level:
        // callvirt set_Property(ldloc S_1, stloc v(binary.op(callvirt get_Property(ldloc S_1), value)))
        // ==> stloc v(compound.op.new(callvirt get_Property(ldloc S_1), value))
        setter_value = store_in_setter->value();
        if (is_small_integer_type(*store_in_setter->variable()->type())) {
            // 'stloc v' implicitly truncates the value.
            // Ensure that type of 'v' matches the type of the property:
            if (typesystem::get_size(*store_in_setter->variable()->type()) != typesystem::get_size(*target_type)) return false;
            if (typesystem::get_sign(*store_in_setter->variable()->type()) != typesystem::get_sign(*target_type)) return false;
        }
    }
    ILVariable* forbidden_variable = store_in_setter != nullptr ? store_in_setter->variable() : nullptr;
    ILInstruction* new_inst;
    Conv* small_int_conv;
    ILInstruction* unwrapped = unwrap_small_integer_conv(setter_value, &small_int_conv);
    ILInstruction* target;
    CompoundTargetKind target_kind;
    FinalizeMatch finalize_match;
    if (auto* binary = as<BinaryNumericInstruction>(unwrapped)) {
        if (is<StLoc>(compound_store)) {
            // transform local variables only for user-defined operators
            return false;
        }
        if (!is_matching_compound_load(binary->left(), compound_store, &target, &target_kind, &finalize_match, forbidden_variable)) return false;
        if (!validate_compound_assign(binary, small_int_conv, target_type, context.settings())) return false;
        context.step("Compound assignment (binary.numeric)", compound_store);
        if (finalize_match) finalize_match(context);
        new_inst = make<NumericCompoundAssign>(binary, target, target_kind, binary->right(), target_type, CompoundEvalMode::EvaluatesToNewValue);
    } else if (auto* operator_call = as<Call>(setter_value); operator_call != nullptr && operator_call->method->is_operator()) {
        if (operator_call->arguments().count() == 0) return false;
        if (!is_matching_compound_load(operator_call->arguments()[0], compound_store, &target, &target_kind, &finalize_match, forbidden_variable))
            return false;
        ILInstruction* rhs;
        if (operator_call->arguments().count() == 2) {
            if (!csharp::ExpressionBuilder::get_assignment_operator_type_from_metadata_name(operator_call->method->name(), context.settings()).has_value()) return false;
            rhs = operator_call->arguments()[1];
        } else if (operator_call->arguments().count() == 1) {
            if (!UserDefinedCompoundAssign::is_increment_or_decrement(operator_call->method, &context.settings())) return false;
            // use a dummy node so that we don't need a dedicated instruction for user-defined unary operator calls
            rhs = make<LdcI4>(1);
        } else {
            return false;
        }
        if (operator_call->is_lifted()) return false;  // TODO: add tests and think about whether nullables need special considerations
        context.step("Compound assignment (user-defined binary)", compound_store);
        if (finalize_match) finalize_match(context);
        new_inst = make<UserDefinedCompoundAssign>(operator_call->method, CompoundEvalMode::EvaluatesToNewValue, target, target_kind, rhs);
    } else if (auto* dynamic_binary_op = as<DynamicBinaryOperatorInstruction>(setter_value)) {
        if (!is_matching_compound_load(dynamic_binary_op->left(), compound_store, &target, &target_kind, &finalize_match, forbidden_variable))
            return false;
        context.step("Compound assignment (dynamic binary)", compound_store);
        if (finalize_match) finalize_match(context);
        new_inst = make<DynamicCompoundAssign>(to_compound(dynamic_binary_op->operation()), dynamic_binary_op->binder_flags(), target,
                                               dynamic_binary_op->left_argument_info(), dynamic_binary_op->right(),
                                               dynamic_binary_op->right_argument_info(), target_kind);
    } else if (auto* concat_call = as<Call>(setter_value); concat_call != nullptr && UserDefinedCompoundAssign::is_string_concat(concat_call->method)) {
        // setterValue is a string.Concat() invocation
        if (is<StLoc>(compound_store)) {
            // transform local variables only for user-defined operators
            return false;
        }
        if (concat_call->arguments().count() != 2) return false;  // for now we only support binary compound assignments
        if (!typesystem::is_known_type(target_type, KnownTypeCode::String)) return false;
        ILInstruction* arg = concat_call->arguments()[0];
        if (auto* call = as<Call>(arg); call != nullptr && csharp::CallBuilder::is_string_to_read_only_span_char_implicit_conversion(call->method)) {
            arg = call->arguments()[0];
            auto* new_obj = as<NewObj>(concat_call->arguments()[1]);
            if (!(new_obj != nullptr && new_obj->arguments().count() == 1 && is<AddressOf>(new_obj->arguments()[0]) &&
                  ILInlining::is_read_only_span_char_ctor(new_obj->method))) {
                return false;
            }
        }

        if (!is_matching_compound_load(arg, compound_store, &target, &target_kind, &finalize_match, forbidden_variable)) return false;
        context.step("Compound assignment (string concatenation)", compound_store);
        if (finalize_match) finalize_match(context);
        new_inst = make<UserDefinedCompoundAssign>(concat_call->method, CompoundEvalMode::EvaluatesToNewValue, target, target_kind,
                                                   concat_call->arguments()[1]);
    } else {
        return false;
    }
    new_inst->add_il_range(*setter_value);
    if (store_in_setter != nullptr) {
        store_in_setter->set_value(new_inst);
        new_inst = store_in_setter;
        context.request_rerun();  // moving stloc to top-level might trigger inlining
    }
    compound_store->replace_with(new_inst);
    context.end_step(new_inst);
    if (auto* inline_assign_block = as<Block>(new_inst->parent()); inline_assign_block != nullptr && inline_assign_block->kind == BlockKind::CallInlineAssign) {
        // It's possible that we first replaced the instruction in an inline-assign helper block.
        // In such a situation, we know from the block invariant that we're have a storeInSetter.
        assert(store_in_setter != nullptr);
        assert(store_in_setter->variable()->is_single_definition() && store_in_setter->variable()->load_count() == 1);
        assert(inline_assign_block->instructions.count() == 1 && inline_assign_block->instructions[0] == store_in_setter);
        assert(inline_assign_block->final_instruction()->match_ld_loc(store_in_setter->variable()));
        // Block CallInlineAssign { stloc I_0(compound.op(...)); final: ldloc I_0 }
        // --> compound.op(...)
        inline_assign_block->replace_with(store_in_setter->value());
    }
    return true;
}

// stloc s(value)
// stloc l(ldloc s)
//   where neither 'stloc s' nor 'stloc l' truncates the value
// -->
// stloc s(stloc l(value))
bool TransformAssignment::transform_inline_assignment_local(Block* block, int pos) {
    StatementTransformContext& context = *context_;
    auto* inst = as<StLoc>(block->instructions[pos]);
    auto* next_inst = as<StLoc>(element_at_or_default(block, pos + 1));
    if (inst == nullptr || next_inst == nullptr) return false;
    if (inst->variable()->kind() != VariableKind::StackSlot) return false;
    if (!(next_inst->variable()->kind() == VariableKind::Local || next_inst->variable()->kind() == VariableKind::Parameter)) return false;
    if (!next_inst->value()->match_ld_loc(inst->variable())) return false;
    if (is_implicit_truncation(inst->value(), inst->variable()->type(), &context.type_system())) {
        // 'stloc s' is implicitly truncating the stack value
        return false;
    }
    if (is_implicit_truncation(inst->value(), next_inst->variable()->type(), &context.type_system())) {
        // 'stloc l' is implicitly truncating the stack value
        return false;
    }
    if (next_inst->variable()->stack_type == StackType::Ref) {
        // ref locals need to be initialized when they are declared, so
        // we can only use inline assignments when we know that the
        // ref local is definitely assigned.
        // We don't have an easy way to check for that in this transform,
        // so avoid inline assignments to ref locals for now.
        return false;
    }
    context.step("Inline assignment to local variable", inst);
    ILInstruction* value = inst->value();
    ILVariable* var = next_inst->variable();
    ILVariable* stack_var = inst->variable();
    block->instructions.remove_at(pos);
    auto* inner_store = make<StLoc>(var, value);
    auto* inline_assignment = make<StLoc>(stack_var, inner_store);
    next_inst->replace_with(inline_assignment);
    context.end_step(inline_assignment);
    return true;
}

bool TransformAssignment::is_implicit_truncation(ILInstruction* value, const IType* type, const ICompilation* compilation, bool allow_nullable_value) {
    return check_implicit_truncation(value, type, compilation, allow_nullable_value) != ImplicitTruncationResult::ValuePreserved;
}

namespace {

// NumericCompoundAssign.IsBinaryCompatibleWithType asks for it (see set_implicit_truncation_check).
const bool implicit_truncation_check_registered = (set_implicit_truncation_check(&TransformAssignment::is_implicit_truncation), true);

}  // namespace

TransformAssignment::ImplicitTruncationResult TransformAssignment::check_implicit_truncation(ILInstruction* value, const IType* type,
                                                                                            const ICompilation* compilation, bool allow_nullable_value) {
    if (!is_small_integer_type(*type)) {
        // Implicit truncation in ILAst only happens for small integer types;
        // other types of implicit truncation in IL cause the ILReader to insert
        // conv instructions.
        return ImplicitTruncationResult::ValuePreserved;
    }
    // With small integer types, test whether the value might be changed by
    // truncation (based on type.GetSize()) followed by sign/zero extension (based on type.GetSign()).
    // (it's OK to have false-positives here if we're unsure)
    std::int32_t val;
    if (value->match_ldc_i4(&val)) {
        const typesystem::ITypeDefinition* definition = typesystem::get_enum_underlying_type(*type)->get_definition();
        bool value_fits = false;
        if (definition != nullptr) {
            switch (definition->known_type_code()) {
                case KnownTypeCode::Boolean: value_fits = val == 0 || val == 1; break;
                case KnownTypeCode::Byte: value_fits = val >= 0 && val <= 255; break;
                case KnownTypeCode::SByte: value_fits = val >= -128 && val <= 127; break;
                case KnownTypeCode::Int16: value_fits = val >= -32768 && val <= 32767; break;
                case KnownTypeCode::UInt16:
                case KnownTypeCode::Char: value_fits = val >= 0 && val <= 65535; break;
                default: value_fits = false; break;
            }
        }
        return value_fits ? ImplicitTruncationResult::ValuePreserved : ImplicitTruncationResult::ValueChanged;
    } else if (auto* conv = as<Conv>(value)) {
        PrimitiveType primitive_type = typesystem::to_primitive_type(*type);
        PrimitiveType conv_target_type = conv->target_type;
        if (conv_target_type == primitive_type) return ImplicitTruncationResult::ValuePreserved;
        if (get_size(primitive_type) == get_size(conv_target_type) && get_sign(primitive_type) != get_sign(conv_target_type) &&
            has_opposite_sign(primitive_type))
            return ImplicitTruncationResult::ValueChangedDueToSignMismatch;
        return ImplicitTruncationResult::ValueChanged;
    } else if (is<Comp>(value)) {
        return ImplicitTruncationResult::ValuePreserved;  // comp returns 0 or 1, which always fits
    } else if (auto* bni = as<BinaryNumericInstruction>(value)) {
        switch (bni->op) {
            case BinaryNumericOperator::BitAnd:
            case BinaryNumericOperator::BitOr:
            case BinaryNumericOperator::BitXor: {
                // If both input values fit into the type without truncation,
                // the result of a binary operator will also fit.
                ImplicitTruncationResult left_truncation = check_implicit_truncation(bni->left(), type, compilation, allow_nullable_value);
                // If the left side is truncating and a sign change is not possible we do not need to evaluate the right side
                if (left_truncation == ImplicitTruncationResult::ValueChanged) return ImplicitTruncationResult::ValueChanged;
                ImplicitTruncationResult right_truncation = check_implicit_truncation(bni->right(), type, compilation, allow_nullable_value);
                return common_implicit_truncation(left_truncation, right_truncation);
            }
            default: break;
        }
    } else if (auto* if_inst = as<IfInstruction>(value)) {
        ImplicitTruncationResult true_truncation = check_implicit_truncation(if_inst->true_inst(), type, compilation, allow_nullable_value);
        // If the true branch is truncating and a sign change is not possible we do not need to evaluate the false branch
        if (true_truncation == ImplicitTruncationResult::ValueChanged) return ImplicitTruncationResult::ValueChanged;
        ImplicitTruncationResult false_truncation = check_implicit_truncation(if_inst->false_inst(), type, compilation, allow_nullable_value);
        return common_implicit_truncation(true_truncation, false_truncation);
    } else {
        const IType* inferred_type = infer_type(value, compilation);
        if (allow_nullable_value) {
            inferred_type = typesystem::nullable_type::get_underlying_type(inferred_type);
        }
        if (inferred_type->kind() != TypeKind::Unknown) {
            PrimitiveType inferred_primitive = typesystem::to_primitive_type(*inferred_type);
            PrimitiveType primitive_type = typesystem::to_primitive_type(*type);

            bool same_sign = get_sign(inferred_primitive) == get_sign(primitive_type);
            if (get_size(inferred_primitive) <= get_size(primitive_type) && same_sign) return ImplicitTruncationResult::ValuePreserved;
            if (get_size(inferred_primitive) == get_size(primitive_type) && !same_sign && has_opposite_sign(primitive_type))
                return ImplicitTruncationResult::ValueChangedDueToSignMismatch;
            return ImplicitTruncationResult::ValueChanged;
        }
    }
    // In unknown cases, assume that the value might be changed by truncation.
    return ImplicitTruncationResult::ValueChanged;
}

TransformAssignment::ImplicitTruncationResult TransformAssignment::common_implicit_truncation(ImplicitTruncationResult left, ImplicitTruncationResult right) {
    if (left == right) return left;
    // Note: in all cases where left!=right, we return ValueChanged:
    // if only one side can be fixed by changing the sign, we don't want to change the sign of the other side.
    return ImplicitTruncationResult::ValueChanged;
}

bool TransformAssignment::is_compound_store(ILInstruction* inst, const IType** store_type, ILInstruction** value, const ICompilation& compilation) {
    *value = nullptr;
    *store_type = nullptr;
    if (auto* stobj = as<StObj>(inst)) {
        // stobj.Type may just be 'int' (due to stind.i4) when we're actually operating on a 'ref MyEnum'.
        // Try to determine the real type of the object we're modifying:
        *store_type = infer_type(stobj->target(), &compilation);
        if (auto* ref_type = dynamic_cast<const ByReferenceType*>(*store_type)) {
            if (typesystem::is_compatible_type_for_memory_access(*ref_type->element_type(), *stobj->type())) {
                *store_type = ref_type->element_type();
            } else {
                *store_type = stobj->type();
            }
        } else if (auto* pointer_type = dynamic_cast<const PointerType*>(*store_type)) {
            if (typesystem::is_compatible_type_for_memory_access(*pointer_type->element_type(), *stobj->type())) {
                *store_type = pointer_type->element_type();
            } else {
                *store_type = stobj->type();
            }
        } else {
            *store_type = stobj->type();
        }
        *value = stobj->value();
        return SemanticHelper::is_pure(stobj->target()->flags());
    } else if (auto* call = as<CallInstruction>(inst); call != nullptr && (call->op_code == OpCode::Call || call->op_code == OpCode::CallVirt)) {
        if (call->method->parameters().empty()) {
            return false;
        }
        for (int i = 0; i < call->arguments().count() - 1; ++i) {
            ILInstruction* arg = call->arguments()[i];
            ILVariable* v;
            if (arg->match_st_loc(&v) && v->is_single_definition() && v->load_count() == 1) {
                continue;  // OK, IsMatchingCompoundLoad can perform an adjustment in this special case
            }
            if (!SemanticHelper::is_pure(arg->flags())) {
                return false;
            }
        }
        *store_type = call->method->parameters().back()->type();
        *value = call->arguments().last();
        const IProperty* owner = dynamic_cast<const IProperty*>(call->method->accessor_owner());
        return is_same_member(call->method, owner != nullptr ? owner->setter() : nullptr);
    } else if (auto* stloc = as<StLoc>(inst); stloc != nullptr && (stloc->variable()->kind() == VariableKind::Local || stloc->variable()->kind() == VariableKind::Parameter)) {
        *store_type = stloc->variable()->type();
        *value = stloc->value();
        return true;
    } else {
        return false;
    }
}

bool TransformAssignment::is_matching_compound_load(ILInstruction* load, ILInstruction* store, ILInstruction** target, CompoundTargetKind* target_kind,
                                                    FinalizeMatch* finalize_match, ILVariable* forbidden_variable, ILInstruction* previous_instruction) {
    *target = nullptr;
    *target_kind = static_cast<CompoundTargetKind>(0);
    *finalize_match = nullptr;

    auto is_duplicated_address_computation = [&](ILInstruction* store_target, ILInstruction* load_target) {
        // Sometimes roslyn duplicates the address calculation:
        // stloc S_0(ldloc refParam)
        // stloc V_0(ldobj System.Int32(ldloc refParam))
        // stobj System.Int32(ldloc S_0, binary.add.i4(ldloc V_0, ldc.i4 1))
        while (is<LdFlda>(store_target) && is<LdFlda>(load_target)) {
            auto* store_ld_flda = static_cast<LdFlda*>(store_target);
            auto* load_ld_flda = static_cast<LdFlda*>(load_target);
            if (!member_equals(store_ld_flda->field(), load_ld_flda->field())) return false;
            store_target = store_ld_flda->target();
            load_target = load_ld_flda->target();
        }
        ILVariable* s;
        if (!store_target->match_ld_loc(&s)) return false;
        if (!(s->kind() == VariableKind::StackSlot && s->is_single_definition() && s != forbidden_variable)) return false;
        IStoreInstruction* single_store = single_store_or_default(s);
        if ((single_store != nullptr ? single_store->as_instruction() : nullptr) != previous_instruction) return false;
        auto* address_store = as<StLoc>(previous_instruction);
        return address_store != nullptr && address_store->value()->match(load_target).success();
    };

    auto* ldobj = as<LdObj>(load);
    auto* stobj = as<StObj>(store);
    if (ldobj != nullptr && stobj != nullptr) {
        assert(SemanticHelper::is_pure(stobj->target()->flags()));
        if (!SemanticHelper::is_pure(ldobj->target()->flags())) return false;
        if (forbidden_variable != nullptr && forbidden_variable->is_used_within(ldobj->target())) return false;
        *target = ldobj->target();
        *target_kind = CompoundTargetKind::Address;
        if (ldobj->target()->match(stobj->target()).success()) {
            return true;
        } else if (is_duplicated_address_computation(stobj->target(), ldobj->target())) {
            // Use S_0 as target, so that S_0 can later be eliminated by inlining.
            // (we can't eliminate previousInstruction right now, because it's before the transform's starting instruction)
            *target = stobj->target();
            return true;
        } else {
            return false;
        }
    } else if (matching_getter_and_setter_calls(as<CallInstruction>(load), as<CallInstruction>(store), finalize_match)) {
        if (forbidden_variable != nullptr && forbidden_variable->is_used_within(load)) return false;
        *target = load;
        *target_kind = CompoundTargetKind::Property;
        return true;
    } else if (auto* ldloc = as<LdLoc>(load); ldloc != nullptr && is<StLoc>(store) &&
                                              ILVariableEqualityComparer{}(ldloc->variable(), static_cast<StLoc*>(store)->variable())) {
        auto* stloc = static_cast<StLoc*>(store);
        if (ILVariableEqualityComparer{}(ldloc->variable(), forbidden_variable)) return false;
        *target = with_il_range(make<LdLoca>(ldloc->variable()), ldloc);
        *target_kind = CompoundTargetKind::Address;
        *finalize_match = [ldloc, stloc](ILTransformContext& context) { context.function()->recombine_variables(ldloc->variable(), stloc->variable()); };
        return true;
    } else {
        return false;
    }
}

// stloc l(stloc target(binary.add(ldloc target, ldc.i4 1)))
bool TransformAssignment::transform_pre_inc_dec_operator_with_inline_store(Block* block, int pos) {
    StatementTransformContext& context = *context_;
    ILInstruction* store = block->instructions[pos];
    const IType* target_type1;
    ILInstruction* value1;
    if (!is_compound_store(store, &target_type1, &value1, context.type_system())) {
        return false;
    }
    const IType* target_type2;
    ILInstruction* value2;
    if (!is_compound_store(value1, &target_type2, &value2, context.type_system())) {
        return false;
    }
    if (target_type1 != target_type2) return false;
    const IType* target_type = target_type1;
    auto* stloc_outer = as<StLoc>(store);
    auto* stloc_inner = as<StLoc>(value1);
    LdLoc* ldloc;
    Conv* conv;
    auto* binary = as<BinaryNumericInstruction>(unwrap_small_integer_conv(value2, &conv));
    Call* operator_call = nullptr;
    if (binary != nullptr && (binary->right()->match_ldc_i(1) || binary->right()->match_ldc_f4(1) || binary->right()->match_ldc_f8(1))) {
        if (!(binary->op == BinaryNumericOperator::Add || binary->op == BinaryNumericOperator::Sub)) return false;

        if (conv != nullptr) {
            PrimitiveType primitive_type = typesystem::to_primitive_type(*target_type);
            if (get_size(primitive_type) == get_size(conv->target_type) && get_sign(primitive_type) != get_sign(conv->target_type))
                target_type = swap_sign(target_type, context.type_system());
        }

        if (!validate_compound_assign(binary, conv, target_type, context.settings())) return false;
        ldloc = as<LdLoc>(binary->left());
    } else if (auto* call = as<Call>(value2); call != nullptr && call->method->is_operator() && call->arguments().count() == 1) {
        operator_call = call;
        if (!(operator_call->method->name() == "op_Increment" || operator_call->method->name() == "op_Decrement")) return false;
        if (operator_call->is_lifted()) return false;  // TODO: add tests and think about whether nullables need special considerations
        ldloc = as<LdLoc>(operator_call->arguments()[0]);
    } else {
        return false;
    }
    if (stloc_outer == nullptr) return false;
    if (stloc_inner == nullptr) return false;
    if (ldloc == nullptr) return false;
    if (!(stloc_outer->variable()->kind() == VariableKind::Local || stloc_outer->variable()->kind() == VariableKind::StackSlot)) return false;
    ILInstruction* target;
    CompoundTargetKind target_kind;
    FinalizeMatch finalize_match;
    if (!is_matching_compound_load(ldloc, stloc_inner, &target, &target_kind, &finalize_match)) return false;
    if (is_implicit_truncation(stloc_outer->value(), stloc_outer->variable()->type(), &context.type_system())) return false;
    context.step("TransformPreIncDecOperatorWithInlineStore", store);
    if (finalize_match) finalize_match(context);
    if (binary != nullptr) {
        auto* compound = make<NumericCompoundAssign>(binary, target, target_kind, binary->right(), target_type, CompoundEvalMode::EvaluatesToNewValue);
        block->instructions.set(pos, make<StLoc>(stloc_outer->variable(), compound));
    } else {
        auto* compound = make<UserDefinedCompoundAssign>(operator_call->method, CompoundEvalMode::EvaluatesToNewValue, target, target_kind, make<LdcI4>(1));
        block->instructions.set(pos, make<StLoc>(stloc_outer->variable(), compound));
    }
    context.end_step(block->instructions[pos]);
    return true;
}

// stobj(target, binary.add(stloc l(ldobj(target)), ldc.i4 1))
//   where target is pure and does not use 'l', and the 'stloc l' does not truncate
// -->
// stloc l(compound.op.old(ldobj(target), ldc.i4 1))
//
//  -or-
//
// call set_Prop(args..., binary.add(stloc l(call get_Prop(args...)), ldc.i4 1))
//   where args.. are pure and do not use 'l', and the 'stloc l' does not truncate
// -->
// stloc l(compound.op.old(call get_Prop(target), ldc.i4 1))
//
// This pattern is used for post-increment by legacy csc.
//
// Even though this transform operates only on a single expression, it's not an expression transform
// as the result value of the expression changes (this is OK only for statements in a block).
bool TransformAssignment::transform_post_inc_dec_operator_with_inline_store(Block* block, int pos) {
    StatementTransformContext& context = *context_;
    ILInstruction* store = block->instructions[pos];
    const IType* target_type;
    ILInstruction* value;
    if (!is_compound_store(store, &target_type, &value, context.type_system())) {
        return false;
    }
    StLoc* stloc;
    Conv* conv;
    auto* binary = as<BinaryNumericInstruction>(unwrap_small_integer_conv(value, &conv));
    Call* operator_call = nullptr;
    if (binary != nullptr && (binary->right()->match_ldc_i(1) || binary->right()->match_ldc_f4(1) || binary->right()->match_ldc_f8(1))) {
        if (!(binary->op == BinaryNumericOperator::Add || binary->op == BinaryNumericOperator::Sub)) return false;

        if (conv != nullptr) {
            PrimitiveType primitive_type = typesystem::to_primitive_type(*target_type);
            if (get_size(primitive_type) == get_size(conv->target_type) && get_sign(primitive_type) != get_sign(conv->target_type))
                target_type = swap_sign(target_type, context.type_system());
        }

        if (!validate_compound_assign(binary, conv, target_type, context.settings())) return false;
        stloc = as<StLoc>(binary->left());
    } else if (auto* call = as<Call>(value); call != nullptr && call->method->is_operator() && call->arguments().count() == 1) {
        operator_call = call;
        if (!(operator_call->method->name() == "op_Increment" || operator_call->method->name() == "op_Decrement")) return false;
        if (operator_call->is_lifted()) return false;  // TODO: add tests and think about whether nullables need special considerations
        stloc = as<StLoc>(operator_call->arguments()[0]);
    } else {
        return false;
    }
    if (stloc == nullptr) return false;
    if (!(stloc->variable()->kind() == VariableKind::Local || stloc->variable()->kind() == VariableKind::StackSlot)) return false;
    ILInstruction* target;
    CompoundTargetKind target_kind;
    FinalizeMatch finalize_match;
    if (!is_matching_compound_load(stloc->value(), store, &target, &target_kind, &finalize_match, stloc->variable())) return false;
    if (is_implicit_truncation(stloc->value(), stloc->variable()->type(), &context.type_system())) return false;
    context.step("TransformPostIncDecOperatorWithInlineStore", store);
    if (finalize_match) finalize_match(context);
    if (binary != nullptr) {
        auto* compound = make<NumericCompoundAssign>(binary, target, target_kind, binary->right(), target_type, CompoundEvalMode::EvaluatesToOldValue);
        block->instructions.set(pos, make<StLoc>(stloc->variable(), compound));
    } else {
        auto* compound = make<UserDefinedCompoundAssign>(operator_call->method, CompoundEvalMode::EvaluatesToOldValue, target, target_kind, make<LdcI4>(1));
        block->instructions.set(pos, make<StLoc>(stloc->variable(), compound));
    }
    context.end_step(block->instructions[pos]);
    return true;
}

// stloc tmp(ldobj(target))
// stobj(target, binary.op(ldloc tmp, ldc.i4 1))
//   target is pure and does not use 'tmp', 'stloc does not truncate'
// -->
// stloc tmp(compound.op.old(ldobj(target), ldc.i4 1))
//
// This is usually followed by inlining or eliminating 'tmp'.
//
// Local variables use a similar pattern, also detected by this function:
//
// stloc tmp(ldloc target)
// stloc target(binary.op(ldloc tmp, ldc.i4 1))
// -->
// stloc tmp(compound.op.old(ldloca target, ldc.i4 1))
//
// This pattern occurs with legacy csc for static fields, and with Roslyn for most post-increments.
bool TransformAssignment::transform_post_inc_dec_operator(Block* block, int i) {
    StatementTransformContext& context = *context_;
    auto* inst = as<StLoc>(block->instructions[i]);
    ILInstruction* store = element_at_or_default(block, i + 1);
    if (inst == nullptr || store == nullptr) return false;
    ILVariable* tmp_var = inst->variable();
    const IType* target_type;
    ILInstruction* value;
    if (!is_compound_store(store, &target_type, &value, context.type_system())) return false;
    ImplicitTruncationResult truncation = check_implicit_truncation(inst->value(), target_type, &context.type_system());
    if (truncation == ImplicitTruncationResult::ValueChanged) {
        // 'stloc tmp' is implicitly truncating the value
        return false;
    }
    if (truncation == ImplicitTruncationResult::ValueChangedDueToSignMismatch) {
        auto* st_obj = as<StObj>(store);
        if (!(st_obj != nullptr && typesystem::type_equals(st_obj->type(), target_type))) {
            // We cannot apply the sign change, so we can't fix the truncation
            return false;
        }
    }
    ILInstruction* target;
    CompoundTargetKind target_kind;
    FinalizeMatch finalize_match;
    if (!is_matching_compound_load(inst->value(), store, &target, &target_kind, &finalize_match, inst->variable(), element_at_or_default(block, i - 1))) {
        return false;
    }
    Conv* conv;
    if (auto* binary = as<BinaryNumericInstruction>(unwrap_small_integer_conv(value, &conv))) {
        if (!(binary->op == BinaryNumericOperator::Add || binary->op == BinaryNumericOperator::Sub)) return false;
        if (!binary->left()->match_ld_loc(tmp_var)) return false;
        if (auto* ptr_type = dynamic_cast<const PointerType*>(target_type)) {
            ILInstruction* right = PointerArithmeticOffset::detect(binary->right(), ptr_type->element_type(), binary->check_for_overflow);
            if (right == nullptr || !right->match_ldc_i(1)) return false;
        } else if (!(binary->right()->match_ldc_i(1) || binary->right()->match_ldc_f4(1) || binary->right()->match_ldc_f8(1)))
            return false;
        if (truncation == ImplicitTruncationResult::ValueChangedDueToSignMismatch && is<StObj>(store)) {
            // Change the sign of the type to skip implicit truncation
            target_type = swap_sign(target_type, context.type_system());
            static_cast<StObj*>(store)->set_type(target_type);
        }
        if (!validate_compound_assign(binary, conv, target_type, context.settings())) return false;
        context.step("TransformPostIncDecOperator (builtin)", inst);
        if (finalize_match) finalize_match(context);
        inst->set_value(make<NumericCompoundAssign>(binary, target, target_kind, binary->right(), target_type, CompoundEvalMode::EvaluatesToOldValue));
    } else if (auto* operator_call = as<Call>(value); operator_call != nullptr && operator_call->method->is_operator() && operator_call->arguments().count() == 1) {
        if (!operator_call->arguments()[0]->match_ld_loc(tmp_var)) return false;
        if (!UserDefinedCompoundAssign::is_increment_or_decrement(operator_call->method, &context.settings())) return false;
        if (operator_call->is_lifted()) return false;  // TODO: add tests and think about whether nullables need special considerations
        context.step("TransformPostIncDecOperator (user-defined)", inst);
        assert(truncation == ImplicitTruncationResult::ValuePreserved);
        if (finalize_match) finalize_match(context);
        inst->set_value(make<UserDefinedCompoundAssign>(operator_call->method, CompoundEvalMode::EvaluatesToOldValue, target, target_kind, make<LdcI4>(1)));
    } else {
        return false;
    }
    block->instructions.remove_at(i + 1);
    if (inst->variable()->is_single_definition() && inst->variable()->load_count() == 0) {
        // dead store -> it was a statement-level post-increment
        inst->replace_with(inst->value());
    }
    return true;
}

bool TransformAssignment::is_same_member(const IMember* a, const IMember* b) {
    if (a == nullptr || b == nullptr) return false;
    a = a->member_definition();
    b = b->member_definition();
    return a->equals(b);
}

}  // namespace cb::dotnet::decompiler::il::transforms
