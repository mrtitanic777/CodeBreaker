// dotnet/decompiler/csharp/expression_builder.cpp -- see expression_builder.hpp.
#include "dotnet/decompiler/csharp/expression_builder.hpp"
#include "dotnet/decompiler/csharp/l2_body.hpp"
#include "translate/frontends/cil/cil_names.hpp"

#include <algorithm>
#include <bit>
#include <cassert>
#include <functional>
#include <stdexcept>
#include <variant>
#include <format>
#include <map>
#include <memory>

#include "dotnet/decompiler/csharp/call_builder.hpp"
#include "dotnet/decompiler/csharp/auto_event_decompiler.hpp"
#include "dotnet/decompiler/csharp/resolver/csharp_invocation_resolve_result.hpp"
#include "dotnet/decompiler/csharp/resolver/dynamic_invocation_resolve_result.hpp"
#include "dotnet/decompiler/csharp/resolver/dynamic_member_resolve_result.hpp"
#include "dotnet/decompiler/csharp/resolver/lambda_resolve_result.hpp"
#include "dotnet/decompiler/csharp/resolver/member_lookup.hpp"
#include "dotnet/decompiler/csharp/statement_builder.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax_extensions.hpp"
#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/il/il_type_extensions.hpp"
#include "dotnet/decompiler/il/instruction_output_il.hpp"
#include "dotnet/decompiler/il/instructions.hpp"
#include "dotnet/decompiler/il/pointer_arithmetic_offset.hpp"
#include "dotnet/decompiler/il/semantic_helper.hpp"
#include "dotnet/decompiler/il/transforms/il_extraction.hpp"
#include "dotnet/decompiler/il/transforms/il_inlining.hpp"
#include "dotnet/decompiler/il/transforms/transform_array_initializers.hpp"
#include "dotnet/decompiler/il/transforms/transform_expression_trees.hpp"
#include "dotnet/decompiler/il/transforms/tuple_transform.hpp"
#include "dotnet/decompiler/metadata/method_semantics_lookup.hpp"
#include "dotnet/decompiler/nr_extensions.hpp"
#include "dotnet/decompiler/semantics/array_create_resolve_result.hpp"
#include "dotnet/decompiler/semantics/by_reference_resolve_result.hpp"
#include "dotnet/decompiler/semantics/constant_resolve_result.hpp"
#include "dotnet/decompiler/semantics/conversion_resolve_result.hpp"
#include "dotnet/decompiler/semantics/error_resolve_result.hpp"
#include "dotnet/decompiler/semantics/invocation_resolve_result.hpp"
#include "dotnet/decompiler/semantics/member_resolve_result.hpp"
#include "dotnet/decompiler/semantics/operator_resolve_result.hpp"
#include "dotnet/decompiler/semantics/size_of_resolve_result.hpp"
#include "dotnet/decompiler/semantics/this_resolve_result.hpp"
#include "dotnet/decompiler/semantics/throw_resolve_result.hpp"
#include "dotnet/decompiler/semantics/type_is_resolve_result.hpp"
#include "dotnet/decompiler/semantics/type_of_resolve_result.hpp"
#include "dotnet/decompiler/semantics/type_resolve_result.hpp"
#include "dotnet/decompiler/typesystem/fake_member.hpp"
#include "dotnet/decompiler/typesystem/metadata_entities.hpp"
#include "dotnet/decompiler/typesystem/metadata_module.hpp"
#include "dotnet/decompiler/typesystem/specialized_member.hpp"
#include "dotnet/decompiler/typesystem/tuple_type.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/typesystem/type_utils.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"
#include "core/error.hpp"

#include "dotnet/decompiler/csharp/csharp_decompiler.hpp"
#include "dotnet/decompiler/csharp/transforms/pattern_statement_transform.hpp"
#include "dotnet/decompiler/csharp/transforms/replace_method_calls_with_operators.hpp"
#include "dotnet/decompiler/csharp/transforms/add_checked_blocks.hpp"

#include "dotnet/decompiler/util/char_info.hpp"
#include "dotnet/decompiler/util/csharp_primitive_cast.hpp"

namespace cb::dotnet::decompiler::csharp {

using namespace il;
using namespace typesystem;
using namespace semantics;
using namespace syntax;
using namespace resolver;
using namespace type_system;
using il::transforms::AccessPathElement;
using il::transforms::AccessPathKind;
using il::ExpressionType;
using resolver::DecompiledLambdaResolveResult;
using resolver::LambdaConversion;
using typesystem::FullTypeName;
using typesystem::KnownAttribute;
using typesystem::NormalizeTypeVisitor;
using typesystem::TopLevelTypeName;
using typesystem::is_known_type;

namespace {

// upstream's `constantValue != null`
bool has_value(const ConstantValue& value) { return !std::holds_alternative<std::monostate>(value); }

// upstream's `(long)CSharpPrimitiveCast.Cast(TypeCode.Int64, value, checkForOverflow: false)`
std::int64_t to_int64(const ConstantValue& value) {
    ConstantValue result = util::csharp_primitive_cast(KnownTypeCode::Int64, value, false);
    return std::get<std::int64_t>(result);
}

enum class CheckedAnnotationKind { Checked, Unchecked, ExplicitUnchecked };

// Marks an expression the way AddCheckedBlocks reads it: arithmetic in a checked context, in an unchecked one, or in an explicit
// unchecked(...) block.
void annotate_checked(Expression* expr, CheckedAnnotationKind kind) {
    switch (kind) {
        case CheckedAnnotationKind::Checked: expr->add_annotation(transforms::AddCheckedBlocks::checked_annotation()); break;
        case CheckedAnnotationKind::Unchecked: expr->add_annotation(transforms::AddCheckedBlocks::unchecked_annotation()); break;
        case CheckedAnnotationKind::ExplicitUnchecked: expr->add_annotation(transforms::AddCheckedBlocks::explicit_unchecked_annotation()); break;
    }
}

// The things of CSharpDecompiler (DecompileRun, IsFixedField) and of the pattern and operator transforms this builder asks.
const UsingScope* using_scope_of(DecompileRun& run) { return run.using_scope; }
bool is_fixed_field(const IField* field, const IType** element_type, int* size) { return CSharpDecompiler::is_fixed_field(field, element_type, size); }

bool is_backing_field_of_automatic_property(const IField* field, const IProperty** property) {
    return transforms::PatternStatementTransform::is_backing_field_of_automatic_property(field, property);
}

bool has_checked_equivalent(const IMethod* method) { return csharp::transforms::ReplaceMethodCallsWithOperators::has_checked_equivalent(method); }
Expression* remove_redundant_to_string_in_concat(const TranslatedExpression& value, const IMethod* method, bool is_last_argument) {
    return csharp::transforms::ReplaceMethodCallsWithOperators::remove_redundant_to_string_in_concat(value.expression, method, is_last_argument);
}

std::string block_kind_text(BlockKind kind) {
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
    return "Unknown";
}

// upstream's `string` is UTF-16
std::u16string to_u16(const std::string& utf8) { return util::utf16(utf8); }

std::string replace_all(std::string text, const std::string& from, const std::string& to) {
    for (std::size_t pos = 0; (pos = text.find(from, pos)) != std::string::npos; pos += to.size()) text.replace(pos, from.size(), to);
    return text;
}

// upstream's `ExpressionWithILInstruction.WithRR` reached through the free function
TranslatedExpression with_rr(ExpressionWithILInstruction expr, ResolveResult* resolve_result) { return expr.with_rr(resolve_result); }

}  // namespace

// ---- construction and the translation entry points ------------------------------------------------------------------------------

ExpressionBuilder::ExpressionBuilder(StatementBuilder* statement_builder_, const DecompilerTypeSystem& type_system,
                                     const ITypeResolveContext& decompilation_context_, ILFunction* current_function_,
                                     const DecompilerSettings& settings_, DecompileRun& decompile_run,
                                     const util::CancellationToken& cancellation_token, L2Body* l2_body)
    : statement_builder(statement_builder_),
      decompilation_context(&decompilation_context_),
      current_function(current_function_),
      compilation(&decompilation_context_.compilation()),
      resolver(make<CSharpResolver>(CSharpTypeResolveContext(&compilation->main_module(), using_scope_of(decompile_run),
                                                             decompilation_context_.current_type_definition(), decompilation_context_.current_member()))),
      ast_builder(resolver),
      type_inference(*compilation),
      settings(&settings_),
      type_system_(&type_system),
      cancellation_token_(cancellation_token),
      l2(l2_body) {
    ast_builder.always_use_short_type_names = true;
    ast_builder.add_resolve_result_annotations = true;
    ast_builder.show_attributes = true;
    ast_builder.use_nullable_specifier_for_value_types = settings->lift_nullables;
    ast_builder.always_use_global = settings->always_use_global;
    type_inference.set_algorithm(TypeInferenceAlgorithm::Improved);
}

AstType* ExpressionBuilder::convert_type(const IType* type) {
    AstType* ast_type = ast_builder.convert_type(type);
    assert(ast_type->annotation<TypeResolveResult>() != nullptr);
    return ast_type;
}

ExpressionWithResolveResult ExpressionBuilder::convert_constant_value(ResolveResult* rr, bool allow_implicit_conversion) {
    Expression* expr = ast_builder.convert_constant_value(rr);
    if (!allow_implicit_conversion) {
        if (is<NullReferenceExpression>(expr) && rr->type()->kind() != TypeKind::Null) {
            expr = make<CastExpression>(convert_type(rr->type()), expr);
        } else if (is_csharp_small_integer_type(*rr->type())) {
            expr = make<CastExpression>(
                make<syntax::PrimitiveType>(*KnownTypeReference::get_csharp_name_by_type_code(rr->type()->get_definition()->known_type_code())), expr);
            // Note: no unchecked annotation necessary, because the constant was folded to be in-range
        } else if (is_csharp_native_integer_type(*rr->type())) {
            expr = make<CastExpression>(make<syntax::PrimitiveType>(rr->type()->name()), expr);
            // Note: no unchecked annotation necessary, because the rr wouldn't be a constant if the value wasn't in-range on 32bit
        }
    }
    ResolveResult* expr_rr = expr->annotation<ResolveResult>();
    if (expr_rr == nullptr) {
        expr_rr = rr;
        expr->add_annotation(rr);
    }
    return ExpressionWithResolveResult(expr, expr_rr);
}

ExpressionWithResolveResult ExpressionBuilder::convert_constant_value(ResolveResult* rr, bool allow_implicit_conversion, bool display_as_hex) {
    ast_builder.print_integral_values_as_hex = display_as_hex;
    struct Restore {
        syntax::TypeSystemAstBuilder* builder;
        ~Restore() { builder->print_integral_values_as_hex = false; }
    } restore{&ast_builder};
    return convert_constant_value(rr, allow_implicit_conversion);
}

TranslatedExpression ExpressionBuilder::translate(ILInstruction* inst, const IType* type_hint) {
    assert(inst != nullptr);
    cancellation_token_.throw_if_cancellation_requested();
    TranslationContext context;
    context.type_hint = type_hint != nullptr ? type_hint : &SpecialType::unknown_type();
    std::optional<TranslatedExpression> from_l2;
    if (l2 != nullptr) {
        if (const translate::ir::Node* node = l2->node_or_lower(inst)) from_l2 = translate_from_l2(*node, *inst, context);
        l2->count(from_l2.has_value(), *inst);
    }
    TranslatedExpression cexpr = from_l2 ? *std::move(from_l2) : accept_visitor<TranslationContext, TranslatedExpression>(*inst, *this, context);
#ifndef NDEBUG
    if (inst->result_type() != StackType::Void && cexpr.type()->kind() != TypeKind::Unknown && inst->result_type() != StackType::Unknown &&
        cexpr.type()->kind() != TypeKind::None) {
        // Validate the translate post-condition (documented in expression_builder.hpp):
        if (is_integer_type(inst->result_type())) {
            assert(is_integer_type(get_stack_type(*cexpr.type())) && "IL instructions of integer type must convert into C# expressions of integer type");
            assert(get_sign(*cexpr.type()) != Sign::None && "Must have a sign specified for zero/sign-extension");
        } else if (auto* liftable = dynamic_cast<ILiftableInstruction*>(inst); liftable != nullptr && liftable->is_lifted()) {
            if (liftable->underlying_result_type() != StackType::Unknown) {
                assert(nullable_type::is_nullable(cexpr.type()));
                const IType* underlying = nullable_type::get_underlying_type(cexpr.type());
                if (is_integer_type(liftable->underlying_result_type())) {
                    assert(is_integer_type(get_stack_type(*underlying)) && "IL instructions of integer type must convert into C# expressions of integer type");
                    assert(get_sign(*underlying) != Sign::None && "Must have a sign specified for zero/sign-extension");
                } else {
                    assert(get_stack_type(*underlying) == liftable->underlying_result_type());
                }
            }
        } else if (inst->result_type() == StackType::Ref) {
            assert(get_stack_type(*cexpr.type()) == StackType::Ref || is_integer_type(get_stack_type(*cexpr.type())));
        } else {
            assert(get_stack_type(*cexpr.type()) == inst->result_type());
        }
    }
#endif
    return cexpr;
}

TranslatedExpression ExpressionBuilder::translate_condition(ILInstruction* condition, bool negate) {
    assert(condition->result_type() == StackType::I4);
    TranslatedExpression expr = translate(condition, known(KnownTypeCode::Boolean));
    if (get_size(get_stack_type(*expr.type())) > 4) {
        expr = expr.convert_to(find_type(StackType::I4, get_sign(*expr.type())), *this);
    }
    return expr.convert_to_boolean(*this, negate);
}

ExpressionWithResolveResult ExpressionBuilder::convert_variable(ILVariable* variable) {
    Expression* expr;
    if (variable->kind() == VariableKind::Parameter && variable->index().has_value() && *variable->index() < 0)
        expr = make<ThisReferenceExpression>();
    else
        expr = make<IdentifierExpression>(variable->name.value());
    if (variable->type()->kind() == TypeKind::ByReference) {
        // When loading a by-ref parameter, use 'ref paramName'.
        // We'll strip away the 'ref' when dereferencing.

        // Ensure that the IdentifierExpression itself also gets a resolve result, as that might
        // get used after the 'ref' is stripped away:
        const IType* element_type = dynamic_cast<const ByReferenceType*>(variable->type())->element_type();
        auto* element_rr = make<ILVariableResolveResult>(variable, element_type);
        with_rr(expr, element_rr);

        expr = make<DirectionExpression>(FieldDirection::Ref, expr);
        return with_rr(expr, make<ByReferenceResolveResult>(element_rr, ReferenceKind::Ref));
    }
    return with_rr(expr, make<ILVariableResolveResult>(variable, variable->type()));
}

bool ExpressionBuilder::hides_variable_with_name(const std::string& name) { return hides_variable_with_name(current_function, name); }

bool ExpressionBuilder::hides_variable_with_name(ILFunction* current_function, const std::string& name) {
    auto hides_variable_or_nested_function = [&](ILFunction* function) {
        for (ILVariable* v : function->variables()) {
            if (v->name.has_value() && *v->name == name) return true;
        }
        for (ILFunction* f : function->local_functions) {
            if (f->name == name) return true;
        }
        return false;
    };
    for (ILInstruction* ancestor : current_function->ancestors()) {
        if (auto* function = il::as<ILFunction>(ancestor); function != nullptr && hides_variable_or_nested_function(function)) return true;
    }
    return false;
}

ILFunction* ExpressionBuilder::resolve_local_function(const IMethod* method) {
    assert(method->is_local_function());
    method = dynamic_cast<const IMethod*>(dynamic_cast<const IMethod*>(method->member_definition())->reduced_from()->member_definition());
    for (ILInstruction* ancestor : current_function->ancestors()) {
        auto* parent = il::as<ILFunction>(ancestor);
        if (parent == nullptr) continue;
        for (ILFunction* f : parent->local_functions) {
            if (f->method->member_definition()->equals(method)) return f;
        }
    }
    return nullptr;
}

bool ExpressionBuilder::requires_qualifier(const IMember* member, const TranslatedExpression& target) {
    if (settings->always_qualify_member_references || hides_variable_with_name(member->name())) return true;
    if (member->is_static()) return !is_current_or_containing_type(member->declaring_type_definition());
    return !(is<ThisReferenceExpression>(target.expression) || is<BaseReferenceExpression>(target.expression));
}

ExpressionWithResolveResult ExpressionBuilder::convert_field(const IField* field, ILInstruction* target_instruction) {
    const IEvent* ev = nullptr;
    if (settings->automatic_events && is_backing_field_of_automatic_event(field, &ev)) {
        // The field-like event hides its backing field, so the reference is printed as
        // the event; inside the declaring type that denotes the backing field.
        TranslatedExpression event_target = translate_target(target_instruction, true, ev->is_static(), ev->declaring_type());
        bool require_event_target = requires_qualifier(ev, event_target);
        auto* event_resolve_result = make<MemberResolveResult>(event_target.resolve_result, ev);
        Expression* event_reference = require_event_target ? static_cast<Expression*>(make<MemberReferenceExpression>(event_target.expression, ev->name()))
                                                           : static_cast<Expression*>(make<IdentifierExpression>(ev->name()));
        return with_rr(event_reference, event_resolve_result);
    }

    TranslatedExpression target = translate_target(target_instruction, true, field->is_static(), field->declaring_type());
    bool require_target;
    // If this is a reference to the backing field of an automatic property and we're going to transform automatic properties
    // in PatternStatementTransform, then we have to do the "requires qualifier"-check based on the property instead of the field.
    // It is easier to solve this special case here than in PatternStatementTransform, because here we perform all resolver checks.
    // It feels a bit hacky, though.
    const IProperty* property = nullptr;
    if (settings->automatic_properties && is_backing_field_of_automatic_property(field, &property) &&
        decompilation_context->current_member() != property && (property->can_set() || settings->getter_only_automatic_properties)) {
        require_target = requires_qualifier(property, target);
    } else {
        require_target = requires_qualifier(field, target);
    }
    bool target_casted = false;
    ResolveResult* target_resolve_result = require_target ? target.resolve_result : nullptr;

    MemberResolveResult* mrr = nullptr;
    auto is_ambiguous_access = [&]() {
        if (target_resolve_result == nullptr) {
            mrr = dynamic_cast<MemberResolveResult*>(resolver->resolve_simple_name(field->name(), {}, false));
        } else {
            MemberLookup lookup(resolver->current_type_definition(), resolver->current_type_definition()->parent_module());
            mrr = dynamic_cast<MemberResolveResult*>(lookup.lookup(target.resolve_result, field->name(), {}, false));
        }
        return mrr == nullptr || mrr->is_error() || !mrr->member()->equals(field, NormalizeTypeVisitor::type_erasure());
    };

    while (is_ambiguous_access()) {
        if (!require_target) {
            require_target = true;
            target_resolve_result = target.resolve_result;
        } else if (!target_casted) {
            target_casted = true;
            target = target.convert_to(field->declaring_type(), *this);
            target_resolve_result = target.resolve_result;
        } else {
            // the field reference is still ambiguous, however, mrr might refer to a different member,
            // e.g., in the case of auto events, their backing fields have the same name.
            // "this.Event" is ambiguous, but should refer to the field, not the event.
            mrr = nullptr;
            break;
        }
    }

    if (mrr == nullptr) {
        mrr = make<MemberResolveResult>(target.resolve_result, field);
    }

    ExpressionWithResolveResult expr = require_target ? with_rr(make<MemberReferenceExpression>(target.expression, field->name()), mrr)
                                                      : with_rr(make<IdentifierExpression>(field->name()), mrr);

    if (field->type()->kind() == TypeKind::ByReference) {
        expr = with_rr(make<DirectionExpression>(FieldDirection::Ref, expr.expression), make<ByReferenceResolveResult>(mrr, ReferenceKind::Ref));
    }

    return expr;
}

// References to an automatic event's backing field are printed as the event. Gated on
// the AutoEventDecompiler verdict: a custom event is not usable as a value (#3858), and
// the field-identity check keeps same-typed sibling events apart (#3575). Within the
// event's own accessors the field is printed as-is.
bool ExpressionBuilder::is_backing_field_of_automatic_event(const IField* field, const IEvent** ev) {
    *ev = nullptr;
    if (field->metadata_token().is_nil() || dynamic_cast<const MetadataModule*>(field->parent_module()) == nullptr) return false;
    auto* module = static_cast<const MetadataModule*>(field->parent_module());
    srm::EventDefinitionHandle event_handle;
    if (!module->metadata_file()->property_and_event_backing_field_lookup().is_event_backing_field(srm::FieldDefinitionHandle(field->metadata_token()), event_handle))
        return false;
    const IEvent* result = module->get_definition(event_handle);
    if (auto* accessor = dynamic_cast<const IMethod*>(decompilation_context->current_member());
        accessor != nullptr && accessor->accessor_owner() != nullptr && accessor->accessor_owner()->equals(result)) {
        return false;
    }
    const IField* backing_field = nullptr;
    if (!auto_event_decompiler::is_automatic_event(*type_system_, result, statement_builder->decompile_run, cancellation_token_, &backing_field) ||
        !backing_field->equals(field->member_definition())) {
        return false;
    }
    *ev = result;
    return true;
}

TranslatedExpression ExpressionBuilder::is_type(IsInst& inst) {
    TranslatedExpression arg = translate(inst.argument());
    arg = unwrap_boxing_conversion(arg);
    return with_rr(with_il_instruction(make<IsExpression>(arg.expression, convert_type(tuple_underlying_type_or_self(inst.type()))), &inst),
                   make<TypeIsResolveResult>(arg.resolve_result, inst.type(), known(KnownTypeCode::Boolean)));
}

TranslatedExpression ExpressionBuilder::visit_is_inst(IsInst& inst, TranslationContext) { return translate_is_inst(TypedUnaryFacts::of(inst)); }

TranslatedExpression ExpressionBuilder::translate_is_inst(const TypedUnaryFacts& inst) {
    TranslatedExpression arg = translate(inst.argument());
    if (inst.type()->is_reference_type() != std::optional<bool>(true)) {
        // isinst with a value type results in an expression of "boxed value type",
        // which is not supported in C#.
        // It's also not supported for unconstrained generic types.
        // Note that several other instructions special-case isinst arguments:
        //  unbox.any T(isinst T(expr)) ==> "expr as T" for nullable value types and class-constrained generic types
        //  comp(isinst T(expr) != null) ==> "expr is T"
        //  on block level (StatementBuilder.VisitIsInst) => "expr is T"
        auto* box = il::as<Box>(inst.argument());
        if (SemanticHelper::is_pure(inst.argument()->flags()) || (box != nullptr && SemanticHelper::is_pure(box->argument()->flags()))) {
            // We can emulate isinst using
            //   expr is T ? expr : null
            // (doubling the boxing side-effect is harmless because the "expr is T" part won't observe object identity,
            //  and we need to support this because Roslyn pattern matching sometimes generates such code.)
            Expression* is_expr = with_il_instruction(make<IsExpression>(arg.expression, convert_type(inst.type())), inst.self).expression;
            Expression* true_expr = arg.expression->clone();
            return without_il_instruction(make<ConditionalExpression>(is_expr, true_expr, make<NullReferenceExpression>()))
                .with_rr(make<ResolveResult>(arg.type()));
        }
        return error_expression("isinst with value type is only supported in some contexts");
    }
    arg = unwrap_boxing_conversion(arg);
    return with_rr(with_il_instruction(make<AsExpression>(arg.expression, convert_type(inst.type())), inst.self),
                   make<ConversionResolveResult>(inst.type(), arg.resolve_result, Conversion::try_cast()));
}

TranslatedExpression ExpressionBuilder::unwrap_boxing_conversion(TranslatedExpression arg) {
    auto* cast = as<CastExpression>(arg.expression);
    auto* crr = dynamic_cast<ConversionResolveResult*>(arg.resolve_result);
    if (cast != nullptr && is_known_type(arg.type(), KnownTypeCode::Object) && crr != nullptr && crr->conversion->is_boxing_conversion()) {
        // When 'is' or 'as' is used with a value type or type parameter,
        // the C# compiler implicitly boxes the input.
        arg = arg.unwrap_child(cast->expression());
    }
    return arg;
}

TranslatedExpression ExpressionBuilder::visit_new_obj(NewObj& inst, TranslationContext context) { return translate_new_obj(CallFacts::of(inst), context); }

TranslatedExpression ExpressionBuilder::translate_new_obj(const CallFacts& inst, TranslationContext context) {
    const IType* type = inst.method->declaring_type();
    if (is_known_type(type, KnownTypeCode::SpanOfT) || is_known_type(type, KnownTypeCode::ReadOnlySpanOfT)) {
        if (inst.arguments.size() == 2) {
            auto* b = il::as<Block>(inst.arguments[0]);
            if (b != nullptr && b->kind == BlockKind::StackAllocInitializer) {
                return translate_stack_alloc_initializer(b, type->type_arguments()[0]);
            }
        }
    }
    return CallBuilder(*this, *type_system_, *settings).build(inst, context.type_hint);
}

TranslatedExpression ExpressionBuilder::visit_ld_virt_delegate(LdVirtDelegate& inst, TranslationContext) {
    return CallBuilder(*this, *type_system_, *settings).build(LdVirtDelegateFacts::of(inst));
}

TranslatedExpression ExpressionBuilder::visit_new_arr(NewArr& inst, TranslationContext) { return translate_new_arr(NewArrFacts::of(inst)); }

TranslatedExpression ExpressionBuilder::translate_new_arr(const NewArrFacts& inst) {
    int dimensions = static_cast<int>(inst.indices().size());
    std::vector<TranslatedExpression> args;
    for (ILInstruction* arg : inst.indices()) args.push_back(translate_array_index(arg));
    auto* expr = make<ArrayCreateExpression>();
    expr->set_type(convert_type(inst.type()));
    if (auto* ct = as<ComposedType>(expr->type())) {
        // change "new (int[,])[10] to new int[10][,]"
        ct->array_specifiers().move_to(expr->additional_array_specifiers());
    }
    for (const TranslatedExpression& arg : args) expr->arguments().add(arg.expression);
    std::vector<ResolveResult*> size_rrs;
    for (const TranslatedExpression& a : args) size_rrs.push_back(a.resolve_result);
    return with_rr(with_il_instruction(expr, inst.self),
                   make<ArrayCreateResolveResult>(make_array_type(*compilation, inst.type(), dimensions), std::move(size_rrs), std::nullopt));
}

TranslatedExpression ExpressionBuilder::visit_loc_alloc(LocAlloc& inst, TranslationContext context) { return translate_loc_alloc_expression(LocAllocFacts::of(inst), context); }

TranslatedExpression ExpressionBuilder::translate_loc_alloc_expression(const LocAllocFacts& inst, TranslationContext context) {
    const IType* element_type = nullptr;
    StackAllocExpression* expr = translate_loc_alloc(inst, context.type_hint, &element_type);
    return with_rr(with_il_instruction(expr, inst.self), make<ResolveResult>(make_pointer_type(element_type)));
}

TranslatedExpression ExpressionBuilder::visit_loc_alloc_span(LocAllocSpan& inst, TranslationContext context) { return translate_loc_alloc_span_expression(LocAllocFacts::of(inst), context); }

TranslatedExpression ExpressionBuilder::translate_loc_alloc_span_expression(const LocAllocFacts& inst, TranslationContext context) {
    const IType* element_type = nullptr;
    StackAllocExpression* expr = translate_loc_alloc_span(inst, context.type_hint, &element_type);
    return with_rr(with_il_instruction(expr, inst.self), make<ResolveResult>(inst.type()));
}

StackAllocExpression* ExpressionBuilder::translate_loc_alloc_span(const LocAllocFacts& inst, const IType*, const IType** element_type) {
    *element_type = inst.type()->type_arguments()[0];
    TranslatedExpression count_expression = translate(inst.argument()).convert_to(known(KnownTypeCode::Int32), *this);
    auto* result = make<StackAllocExpression>();
    result->set_type(convert_type(*element_type));
    result->set_count_expression(count_expression.expression);
    return result;
}

StackAllocExpression* ExpressionBuilder::translate_loc_alloc(const LocAllocFacts& inst, const IType* type_hint, const IType** element_type) {
    TranslatedExpression count_expression;
    const PointerType* pointer_type = nullptr;
    ILInstruction* left = nullptr;
    ILInstruction* right = nullptr;
    const IType* size_of_element_type = nullptr;
    if (inst.argument()->match_binary_numeric_instruction(BinaryNumericOperator::Mul, &left, &right) &&
        right->unwrap_conv(ConversionKind::SignExtend)->unwrap_conv(ConversionKind::ZeroExtend)->match_size_of(&size_of_element_type)) {
        // Determine the element type from the sizeof
        *element_type = size_of_element_type;
        count_expression = translate(left->unwrap_conv(ConversionKind::ZeroExtend));
        pointer_type = dynamic_cast<const PointerType*>(make_pointer_type(*element_type));
    } else {
        // Determine the element type from the expected pointer type in this context
        pointer_type = dynamic_cast<const PointerType*>(type_hint);
        std::optional<TranslatedExpression> offset;
        if (pointer_type != nullptr) {
            TranslatedExpression translated_argument = translate(inst.argument());
            offset = get_pointer_arithmetic_offset(inst.argument(), translated_argument, pointer_type->element_type(), true, true);
        }
        if (pointer_type != nullptr && offset.has_value()) {
            count_expression = *offset;
            *element_type = pointer_type->element_type();
        } else {
            *element_type = known(KnownTypeCode::Byte);
            pointer_type = dynamic_cast<const PointerType*>(make_pointer_type(*element_type));
            count_expression = translate(inst.argument());
        }
    }
    count_expression = count_expression.convert_to(known(KnownTypeCode::Int32), *this);
    auto* result = make<StackAllocExpression>();
    result->set_type(convert_type(*element_type));
    result->set_count_expression(count_expression.expression);
    return result;
}

TranslatedExpression ExpressionBuilder::visit_ldc_i4(LdcI4& inst, TranslationContext context) {
    ResolveResult* rr;
    if (get_sign(*context.type_hint) == Sign::Unsigned) {
        rr = make<ConstantResolveResult>(known(KnownTypeCode::UInt32), ConstantValue(static_cast<std::uint32_t>(inst.value)));
    } else {
        rr = make<ConstantResolveResult>(known(KnownTypeCode::Int32), ConstantValue(static_cast<std::int32_t>(inst.value)));
    }
    rr = adjust_constant_to_type(rr, context.type_hint);
    return convert_constant_value(rr, true).with_il_instruction(&inst);
}

TranslatedExpression ExpressionBuilder::visit_ldc_i8(LdcI8& inst, TranslationContext context) {
    ResolveResult* rr;
    if (get_sign(*context.type_hint) == Sign::Unsigned) {
        rr = make<ConstantResolveResult>(known(KnownTypeCode::UInt64), ConstantValue(static_cast<std::uint64_t>(inst.value)));
    } else {
        rr = make<ConstantResolveResult>(known(KnownTypeCode::Int64), ConstantValue(static_cast<std::int64_t>(inst.value)));
    }
    rr = adjust_constant_to_type(rr, context.type_hint);
    return convert_constant_value(rr, true).with_il_instruction(&inst);
}

bool ExpressionBuilder::should_display_as_hex(std::int64_t value, const IType* type) {
    if (value >= 0 && value <= 9) return false;
    if (value < 0 && get_sign(*type) == Sign::Signed) return false;
    return true;
}

TranslatedExpression ExpressionBuilder::visit_ldc_f4(LdcF4& inst, TranslationContext) {
    Expression* expr = ast_builder.convert_constant_value(known(KnownTypeCode::Single), ConstantValue(inst.value));
    return TranslatedExpression(with_il_instruction(expr, &inst).expression);
}

TranslatedExpression ExpressionBuilder::visit_ldc_f8(LdcF8& inst, TranslationContext) {
    Expression* expr = ast_builder.convert_constant_value(known(KnownTypeCode::Double), ConstantValue(inst.value));
    return TranslatedExpression(with_il_instruction(expr, &inst).expression);
}

TranslatedExpression ExpressionBuilder::visit_ldc_decimal(LdcDecimal& inst, TranslationContext) {
    Expression* expr = ast_builder.convert_constant_value(known(KnownTypeCode::Decimal), ConstantValue(inst.value));
    return TranslatedExpression(with_il_instruction(expr, &inst).expression);
}

TranslatedExpression ExpressionBuilder::visit_ld_str(LdStr& inst, TranslationContext) {
    return with_rr(with_il_instruction(make<PrimitiveExpression>(ConstantValue(to_u16(inst.value))), &inst),
                   make<ConstantResolveResult>(known(KnownTypeCode::String), ConstantValue(to_u16(inst.value))));
}

TranslatedExpression ExpressionBuilder::visit_ld_str_utf8(LdStrUtf8& inst, TranslationContext) {
    const IType* type = make_parameterized_type(known(KnownTypeCode::ReadOnlySpanOfT), {known(KnownTypeCode::Byte)});
    return with_rr(with_il_instruction(make<PrimitiveExpression>(ConstantValue(to_u16(inst.value)), LiteralFormat::Utf8Literal), &inst),
                   make<ConstantResolveResult>(type, ConstantValue(to_u16(inst.value))));
}

TranslatedExpression ExpressionBuilder::visit_ld_null(LdNull& inst, TranslationContext) {
    return get_default_value_expression(&SpecialType::null_type()).with_il_instruction(&inst);
}

TranslatedExpression ExpressionBuilder::visit_default_value(DefaultValue& inst, TranslationContext) {
    return get_default_value_expression(inst.type()).with_il_instruction(&inst);
}

ExpressionWithResolveResult ExpressionBuilder::get_default_value_expression(const IType* type) {
    if (type->is_reference_type() == std::optional<bool>(true)) {
        return with_rr(make<NullReferenceExpression>(), make<ConstantResolveResult>(&SpecialType::null_type(), ConstantValue()));
    } else if (is_known_type(type, KnownTypeCode::NullableOfT)) {
        auto* crr = make<ConstantResolveResult>(&SpecialType::null_type(), ConstantValue());
        return with_rr(make<CastExpression>(convert_type(type), with_rr(make<NullReferenceExpression>(), crr).expression),
                       make<ConversionResolveResult>(type, crr, Conversion::null_literal_conversion()));
    } else if (is_known_type(type, KnownTypeCode::Decimal)) {
        return with_rr(make<PrimitiveExpression>(ConstantValue(util::Decimal())), make<ConstantResolveResult>(type, ConstantValue(util::Decimal())));
    }
    return with_rr(make<DefaultValueExpression>(convert_type(type)), make<ConstantResolveResult>(type, CSharpResolver::get_default_value(type)));
}

TranslatedExpression ExpressionBuilder::visit_size_of(SizeOf& inst, TranslationContext) {
    if (is_unmanaged_type(inst.type(), settings->introduce_unmanaged_constraint)) {
        return with_rr(with_il_instruction(make<SizeOfExpression>(convert_type(inst.type())), &inst),
                       make<SizeOfResolveResult>(known(KnownTypeCode::Int32), inst.type(), std::nullopt));
    }
    return call_unsafe_intrinsic("SizeOf", {}, known(KnownTypeCode::Int32), &inst, {inst.type()});
}

TranslatedExpression ExpressionBuilder::visit_ld_type_token(LdTypeToken& inst, TranslationContext) { return translate_ld_type_token(TypeTokenFacts::of(inst)); }

TranslatedExpression ExpressionBuilder::translate_ld_type_token(const TypeTokenFacts& inst) {
    Expression* typeof_expr = with_rr(make<TypeOfExpression>(convert_type(inst.type())),
                                      make<TypeOfResolveResult>(known(KnownTypeCode::Type), inst.type())).expression;
    return with_rr(with_il_instruction(make<MemberReferenceExpression>(typeof_expr, "TypeHandle"), inst.self),
                   make<TypeOfResolveResult>(typesystem::find_type(*compilation, TopLevelTypeName("System", "RuntimeTypeHandle")), inst.type()));
}

TranslatedExpression ExpressionBuilder::visit_bit_not(BitNot& inst, TranslationContext context) {
    TranslatedExpression argument = translate(inst.argument());
    const IType* arg_utype = nullable_type::get_underlying_type(argument.type());

    if (get_size(get_stack_type(*arg_utype)) < get_size(inst.underlying_result_type()) ||
        (arg_utype->kind() == TypeKind::Enum && is_small_integer_type(*arg_utype)) ||
        (get_stack_type(*arg_utype) == StackType::I && !is_csharp_native_integer_type(*arg_utype)) ||
        is_known_type(arg_utype, KnownTypeCode::Boolean) || is_known_type(arg_utype, KnownTypeCode::Char)) {
        // Argument is undersized (even after implicit integral promotion to I4)
        // -> we need to perform sign/zero-extension before the BitNot.
        // Same if the argument is an enum based on a small integer type
        // (those don't undergo numeric promotion in C# the way non-enum small integer types do).
        // Same if the type is one that does not support ~ (IntPtr, bool and char).
        Sign sign = get_sign(*context.type_hint);
        if (sign == Sign::None) {
            sign = get_sign(*arg_utype);
        }
        const IType* target_type = find_arithmetic_type(inst.underlying_result_type(), sign);
        if (inst.is_lifted()) {
            target_type = nullable_type::create(*compilation, target_type);
        }
        argument = argument.convert_to(target_type, *this);
    }

    ExpressionWithResolveResult result =
        with_rr(make<UnaryOperatorExpression>(UnaryOperatorType::BitNot, argument.expression),
                resolver->resolve_unary_operator(UnaryOperatorType::BitNot, argument.resolve_result));
    return result.with_il_instruction(&inst);
}

ExpressionWithResolveResult ExpressionBuilder::logic_not(TranslatedExpression expr) {
    // "!expr" implicitly converts to bool so we can remove the cast;
    // but only if doing so wouldn't cause us to call a user-defined "operator !"
    expr = expr.unwrap_implicit_bool_conversion([](const IType* type) {
        return type->get_methods([](const IMethod* m) { return m->is_operator() && m->name() == "op_LogicalNot"; }).empty();
    });
    return with_rr(make<UnaryOperatorExpression>(UnaryOperatorType::Not, expr.expression),
                   make<OperatorResolveResult>(known(KnownTypeCode::Boolean), ExpressionType::Not, std::vector<ResolveResult*>{expr.resolve_result}));
}

TranslatedExpression ExpressionBuilder::visit_ld_loc(LdLoc& inst, TranslationContext) {
    if (inst.variable()->kind() == VariableKind::StackSlot && inst.variable()->is_single_definition()) {
        loaded_variables_set_.insert(inst.variable());
    }
    return convert_variable(inst.variable()).with_il_instruction(&inst);
}

TranslatedExpression ExpressionBuilder::visit_ld_loca(LdLoca& inst, TranslationContext) {
    TranslatedExpression expr = convert_variable(inst.variable()).with_il_instruction(&inst);
    // Note that we put the instruction on the IdentifierExpression instead of the DirectionExpression,
    // because the DirectionExpression might get removed by dereferencing instructions such as LdObj
    return without_il_instruction(make<DirectionExpression>(FieldDirection::Ref, expr.expression))
        .with_rr(make<ByReferenceResolveResult>(expr.resolve_result, ReferenceKind::Ref));
}

TranslatedExpression ExpressionBuilder::visit_st_loc(StLoc& inst, TranslationContext) { return translate_st_loc(StLocFacts::of(inst)); }

TranslatedExpression ExpressionBuilder::translate_st_loc(const StLocFacts& inst) {
    TranslatedExpression translated_value = translate(inst.value(), inst.variable()->type());
    auto is_other_value_type = [](const IType* type) {
        return type->is_reference_type() == std::optional<bool>(false) && get_stack_type(*type) == StackType::O;
    };
    auto all_stores_use_consistent_type = [&](const std::vector<IStoreInstruction*>& store_instructions, const IType* expected_type) {
        expected_type = expected_type->accept_visitor(NormalizeTypeVisitor::type_erasure());
        for (IStoreInstruction* store : store_instructions) {
            auto* stloc = dynamic_cast<StLoc*>(store);
            if (stloc == nullptr) return false;
            const IType* type = infer_type(stloc->value(), compilation)->accept_visitor(NormalizeTypeVisitor::type_erasure());
            if (!type->equals(expected_type)) return false;
        }
        return true;
    };
    auto can_use_type_for_stack_slot = [&](ILVariable* v, const IType* type) {
        return v->is_single_definition() || is_other_value_type(type) || v->stack_type == StackType::Ref ||
               all_stores_use_consistent_type(v->store_instructions(), type);
    };
    if (inst.variable()->kind() == VariableKind::StackSlot && loaded_variables_set_.find(inst.variable()) == loaded_variables_set_.end()) {
        // Stack slots in the ILAst have inaccurate types (e.g. System.Object for StackType.O)
        // so we should replace them with more accurate types where possible:
        const IType* type = nullptr;
        if (can_use_type_for_stack_slot(inst.variable(), translated_value.type()) &&
            inst.variable()->stack_type == get_stack_type(*translated_value.type()) && translated_value.type()->kind() != TypeKind::Null) {
            inst.variable()->set_type(translated_value.type());
            if (l2 != nullptr) l2->variable_changed(inst.variable());
        } else if (inst.value()->match_default_value(&type) && is_other_value_type(type)) {
            inst.variable()->set_type(type);
            if (l2 != nullptr) l2->variable_changed(inst.variable());
        }
    }
    TranslatedExpression lhs = convert_variable(inst.variable()).without_il_instruction();
    auto* dir_expr = as<DirectionExpression>(lhs.expression);
    auto* lhs_ref_rr = dynamic_cast<ByReferenceResolveResult*>(lhs.resolve_result);
    if (dir_expr != nullptr && lhs_ref_rr != nullptr) {
        // ref (re-)assignment, emit "ref (a = ref b)".
        lhs = lhs.unwrap_child(dir_expr->expression());
        translated_value = translated_value.convert_to(lhs_ref_rr->type(), *this, false, true);
        ExpressionWithResolveResult assign =
            with_rr(make<AssignmentExpression>(lhs.expression, translated_value.expression),
                    make<OperatorResolveResult>(lhs.type(), ExpressionType::Assign,
                                                std::vector<ResolveResult*>{lhs_ref_rr, translated_value.resolve_result}));
        return without_il_instruction(make<DirectionExpression>(FieldDirection::Ref, assign.expression)).with_rr(lhs_ref_rr);
    }
    return assignment(lhs, translated_value).with_il_instruction(inst.self);
}

TranslatedExpression ExpressionBuilder::visit_comp(Comp& inst, TranslationContext) { return translate_comp_facts(CompFacts::of(inst)); }

TranslatedExpression ExpressionBuilder::translate_comp_facts(const CompFacts& inst) {
    if (inst.lifting_kind == ComparisonLiftingKind::ThreeValuedLogic) {
        if (inst.kind() == ComparisonKind::Equality && inst.right()->match_ldc_i4(0)) {
            // lifted logic.not
            const IType* target_type = nullable_type::create(*compilation, known(KnownTypeCode::Boolean));
            TranslatedExpression arg = translate(inst.left(), target_type).convert_to(target_type, *this);
            return with_il_instruction(make<UnaryOperatorExpression>(UnaryOperatorType::Not, arg.expression), inst.self)
                .with_rr(make<OperatorResolveResult>(target_type, ExpressionType::Not, std::vector<ResolveResult*>{arg.resolve_result}));
        }
        return error_expression("Nullable comparisons with three-valued-logic not supported in C#");
    }
    if (inst.input_type == StackType::Ref) {
        // Reference comparison using Unsafe intrinsics
        assert(!inst.is_lifted());
        const char* method_name;
        bool negate;
        switch (inst.kind()) {
            case ComparisonKind::Equality: method_name = "AreSame"; negate = false; break;
            case ComparisonKind::Inequality: method_name = "AreSame"; negate = true; break;
            case ComparisonKind::LessThan: method_name = "IsAddressLessThan"; negate = false; break;
            case ComparisonKind::LessThanOrEqual: method_name = "IsAddressGreaterThan"; negate = true; break;
            case ComparisonKind::GreaterThan: method_name = "IsAddressGreaterThan"; negate = false; break;
            case ComparisonKind::GreaterThanOrEqual: method_name = "IsAddressLessThan"; negate = true; break;
            default: throw UsageError("Invalid ComparisonKind");
        }
        TranslatedExpression left = translate(inst.left());
        TranslatedExpression right = translate(inst.right());
        if (left.type()->kind() != TypeKind::ByReference || !NormalizeTypeVisitor::type_erasure().equivalent_types(left.type(), right.type())) {
            const IType* common_ref_type = make_by_reference_type(known(KnownTypeCode::Byte));
            left = left.convert_to(common_ref_type, *this);
            right = right.convert_to(common_ref_type, *this);
        }
        const IType* bool_type = known(KnownTypeCode::Boolean);
        TranslatedExpression expr = call_unsafe_intrinsic(method_name, {left.expression, right.expression}, bool_type, inst.self);
        if (negate) {
            expr = without_il_instruction(make<UnaryOperatorExpression>(UnaryOperatorType::Not, expr.expression)).with_rr(make<ResolveResult>(bool_type));
        }
        return expr;
    }
    if (is_equality_or_inequality(inst.kind())) {
        bool negate_output = false;
        TranslatedExpression result = translate_ceq(inst, &negate_output);
        if (negate_output) return logic_not(result).with_il_instruction(inst.self);
        return result;
    }
    return translate_comp(inst);
}

// Translates the equality comparison between left and right.
TranslatedExpression ExpressionBuilder::translate_ceq(const CompFacts& inst, bool* negate_output) {
    assert(is_equality_or_inequality(inst.kind()));
    // Translate '(e as T) == null' to '!(e is T)'.
    // This is necessary for correctness when T is a value type.
    if (inst.left()->op_code == OpCode::IsInst && inst.right()->op_code == OpCode::LdNull) {
        *negate_output = inst.kind() == ComparisonKind::Equality;
        return is_type(static_cast<IsInst&>(*inst.left()));
    } else if (inst.right()->op_code == OpCode::IsInst && inst.left()->op_code == OpCode::LdNull) {
        *negate_output = inst.kind() == ComparisonKind::Equality;
        return is_type(static_cast<IsInst&>(*inst.right()));
    }

    TranslatedExpression left = translate(inst.left());
    TranslatedExpression right = translate(inst.right());

    // Remove redundant bool comparisons
    if (is_known_type(left.type(), KnownTypeCode::Boolean)) {
        if (inst.right()->match_ldc_i4(0)) {
            // 'b == 0' => '!b'
            // 'b != 0' => 'b'
            *negate_output = inst.kind() == ComparisonKind::Equality;
            return left;
        }
        if (inst.right()->match_ldc_i4(1)) {
            // 'b == 1' => 'b'
            // 'b != 1' => '!b'
            *negate_output = inst.kind() == ComparisonKind::Inequality;
            return left;
        }
    } else if (is_known_type(right.type(), KnownTypeCode::Boolean)) {
        if (inst.left()->match_ldc_i4(0)) {
            // '0 == b' => '!b'
            // '0 != b' => 'b'
            *negate_output = inst.kind() == ComparisonKind::Equality;
            return right;
        }
        if (inst.left()->match_ldc_i4(1)) {
            // '1 == b' => 'b'
            // '1 != b' => '!b'
            *negate_output = inst.kind() == ComparisonKind::Inequality;
            return right;
        }
    }
    // Handle comparisons between unsafe pointers and null:
    if (left.type()->kind() == TypeKind::Pointer && inst.right()->match_ldc_i(std::int64_t{0})) {
        *negate_output = false;
        right = TranslatedExpression(with_il_instruction(with_rr(make<NullReferenceExpression>(), make<ConstantResolveResult>(&SpecialType::null_type(), ConstantValue())).expression, inst.right()).expression);
        return create_builtin_binary_operator(left, to_binary_operator_type(inst.kind()), right).with_il_instruction(inst.self);
    } else if (right.type()->kind() == TypeKind::Pointer && inst.left()->match_ldc_i(std::int64_t{0})) {
        *negate_output = false;
        left = TranslatedExpression(with_il_instruction(with_rr(make<NullReferenceExpression>(), make<ConstantResolveResult>(&SpecialType::null_type(), ConstantValue())).expression, inst.left()).expression);
        return create_builtin_binary_operator(left, to_binary_operator_type(inst.kind()), right).with_il_instruction(inst.self);
    }

    // Special case comparisons with enum and char literals
    left = try_unite_equality_operand_type(left, right);
    right = try_unite_equality_operand_type(right, left);

    if (is_special_cased_reference_comparison_with_null(left, right)) {
        // When comparing a string/delegate with null, the C# compiler generates a reference comparison.
        *negate_output = false;
        return create_builtin_binary_operator(left, to_binary_operator_type(inst.kind()), right).with_il_instruction(inst.self);
    }

    auto* rr = dynamic_cast<OperatorResolveResult*>(resolver->resolve_binary_operator(to_binary_operator_type(inst.kind()), left.resolve_result, right.resolve_result));
    if (rr == nullptr || rr->is_error() || rr->user_defined_operator_method() != nullptr ||
        get_stack_type(*nullable_type::get_underlying_type(rr->operands()[0]->type())) != inst.input_type ||
        !is_known_type(rr->type(), KnownTypeCode::Boolean)) {
        const IType* target_type;
        if (inst.input_type == StackType::O) {
            target_type = known(KnownTypeCode::Object);
        } else {
            const IType* left_utype = nullable_type::get_underlying_type(left.type());
            const IType* right_utype = nullable_type::get_underlying_type(right.type());
            if (get_stack_type(*left_utype) == inst.input_type && !is_small_integer_type(*left_utype)) {
                target_type = left_utype;
            } else if (get_stack_type(*right_utype) == inst.input_type && !is_small_integer_type(*right_utype)) {
                target_type = right_utype;
            } else {
                target_type = find_type(inst.input_type, get_sign(*left_utype));
            }
        }
        if (inst.is_lifted()) {
            target_type = nullable_type::create(*compilation, target_type);
        }
        if (target_type->equals(left.type())) {
            right = right.convert_to(target_type, *this);
        } else {
            left = left.convert_to(target_type, *this);
        }
        rr = dynamic_cast<OperatorResolveResult*>(
            resolver->resolve_binary_operator(to_binary_operator_type(inst.kind()), left.resolve_result, right.resolve_result));
        if (rr == nullptr || rr->is_error() || rr->user_defined_operator_method() != nullptr ||
            get_stack_type(*nullable_type::get_underlying_type(rr->operands()[0]->type())) != inst.input_type ||
            !is_known_type(rr->type(), KnownTypeCode::Boolean)) {
            // If converting one input wasn't sufficient, convert both:
            left = left.convert_to(target_type, *this);
            right = right.convert_to(target_type, *this);
            rr = make<OperatorResolveResult>(known(KnownTypeCode::Boolean), BinaryOperatorExpression::get_linq_node_type(to_binary_operator_type(inst.kind()), false),
                                             std::vector<ResolveResult*>{left.resolve_result, right.resolve_result});
        }
    }
    *negate_output = false;
    return with_il_instruction(make<BinaryOperatorExpression>(left.expression, to_binary_operator_type(inst.kind()), right.expression), inst.self).with_rr(rr);
}

TranslatedExpression ExpressionBuilder::try_unite_equality_operand_type(TranslatedExpression left, TranslatedExpression right) {
    // Special case for enum flag check "(enum & EnumType.SomeValue) == 0"
    // so that the const 0 value is printed as 0 integer and not as enum type, e.g. EnumType.None
    auto* binary_expr = as<BinaryOperatorExpression>(right.expression);
    const ConstantValue* constant = nullptr;
    ConstantValue constant_value;
    if (left.resolve_result->is_compile_time_constant()) {
        constant_value = left.resolve_result->constant_value();
        constant = &constant_value;
    }
    if (left.resolve_result->is_compile_time_constant() && is_csharp_primitive_integer_type(*left.resolve_result->type()) && constant != nullptr &&
        std::holds_alternative<std::int32_t>(*constant) && std::get<std::int32_t>(*constant) == 0 &&
        nullable_type::get_underlying_type(right.type())->kind() == TypeKind::Enum && binary_expr != nullptr &&
        binary_expr->operator_() == BinaryOperatorType::BitwiseAnd) {
        return adjust_constant_expression_to_type(left, known(KnownTypeCode::Int32));
    }
    return adjust_constant_expression_to_type(left, right.type());
}

bool ExpressionBuilder::is_special_cased_reference_comparison_with_null(TranslatedExpression lhs, TranslatedExpression rhs) {
    if (lhs.type()->kind() == TypeKind::Null) std::swap(lhs, rhs);
    return rhs.type()->kind() == TypeKind::Null && (lhs.type()->kind() == TypeKind::Delegate || is_known_type(lhs.type(), KnownTypeCode::String)) &&
           lhs.type()->get_definition() != decompilation_context->current_type_definition();
}

ExpressionWithResolveResult ExpressionBuilder::create_builtin_binary_operator(TranslatedExpression left, BinaryOperatorType type,
                                                                              TranslatedExpression right, bool check_for_overflow) {
    return with_rr(make<BinaryOperatorExpression>(left.expression, type, right.expression),
                   make<OperatorResolveResult>(known(KnownTypeCode::Boolean), BinaryOperatorExpression::get_linq_node_type(type, check_for_overflow),
                                               std::vector<ResolveResult*>{left.resolve_result, right.resolve_result}));
}

// Handle Comp instruction, operators other than equality/inequality.
TranslatedExpression ExpressionBuilder::translate_comp(const CompFacts& inst) {
    BinaryOperatorType op = to_binary_operator_type(inst.kind());
    TranslatedExpression left = translate(inst.left());
    TranslatedExpression right = translate(inst.right());

    if (left.type()->kind() == TypeKind::Pointer && right.type()->kind() == TypeKind::Pointer) {
        return create_builtin_binary_operator(left, op, right).with_il_instruction(inst.self);
    }

    left = prepare_arithmetic_argument(left, inst.input_type, inst.sign, inst.is_lifted());
    right = prepare_arithmetic_argument(right, inst.input_type, inst.sign, inst.is_lifted());

    // Special case comparisons with enum and char literals
    left = adjust_constant_expression_to_type(left, right.type());
    right = adjust_constant_expression_to_type(right, left.type());

    // attempt comparison without any additional casts
    auto* rr = dynamic_cast<OperatorResolveResult*>(resolver->resolve_binary_operator(to_binary_operator_type(inst.kind()), left.resolve_result, right.resolve_result));
    if (rr != nullptr && !rr->is_error()) {
        const IType* comp_utype = nullable_type::get_underlying_type(rr->operands()[0]->type());
        if (get_sign(*comp_utype) == inst.sign && get_stack_type(*comp_utype) == inst.input_type) {
            return with_il_instruction(make<BinaryOperatorExpression>(left.expression, op, right.expression), inst.self).with_rr(rr);
        }
    }

    if (is_integer_type(inst.input_type)) {
        // Ensure the inputs have the correct sign:
        const IType* input_type = find_arithmetic_type(inst.input_type, inst.sign);
        if (inst.is_lifted()) {
            input_type = nullable_type::create(*compilation, input_type);
        }
        left = left.convert_to(input_type, *this);
        right = right.convert_to(input_type, *this);
    } else if (inst.input_type == StackType::O) {
        // Unsafe.As<object, UIntPtr>(ref left) op Unsafe.As<object, UIntPtr>(ref right)
        // TTo Unsafe.As<TFrom, TTo>(ref TFrom source)
        const IType* integer_type = known(inst.sign == Sign::Signed ? KnownTypeCode::IntPtr : KnownTypeCode::UIntPtr);
        auto wrap_in_unsafe_as = [&](TranslatedExpression expr) {
            const IType* type = expr.type();
            expr = wrap_in_ref(expr, make_by_reference_type(type));
            return call_unsafe_intrinsic("As", {expr.expression}, integer_type, nullptr, {type, integer_type});
        };
        left = wrap_in_unsafe_as(left);
        right = wrap_in_unsafe_as(right);
    }
    return with_il_instruction(make<BinaryOperatorExpression>(left.expression, op, right.expression), inst.self)
        .with_rr(make<OperatorResolveResult>(known(KnownTypeCode::Boolean), BinaryOperatorExpression::get_linq_node_type(op, false),
                                             std::vector<ResolveResult*>{left.resolve_result, right.resolve_result}));
}

TranslatedExpression ExpressionBuilder::visit_three_valued_bool_and(ThreeValuedBoolAnd& inst, TranslationContext) {
    return handle_three_valued_logic(LogicOperatorFacts::of(inst), BinaryOperatorType::BitwiseAnd, ExpressionType::And);
}

TranslatedExpression ExpressionBuilder::visit_three_valued_bool_or(ThreeValuedBoolOr& inst, TranslationContext) {
    return handle_three_valued_logic(LogicOperatorFacts::of(inst), BinaryOperatorType::BitwiseOr, ExpressionType::Or);
}

TranslatedExpression ExpressionBuilder::handle_three_valued_logic(const LogicOperatorFacts& inst, BinaryOperatorType op, ExpressionType eop) {
    TranslatedExpression left = translate(inst.left());
    TranslatedExpression right = translate(inst.right());
    const IType* bool_type = known(KnownTypeCode::Boolean);
    const IType* nullable_bool_type = nullable_type::create(*compilation, bool_type);
    if (nullable_type::is_nullable(left.type())) {
        left = left.convert_to(nullable_bool_type, *this);
        if (nullable_type::is_nullable(right.type())) {
            right = right.convert_to(nullable_bool_type, *this);
        } else {
            right = right.convert_to(bool_type, *this);
        }
    } else {
        left = left.convert_to(bool_type, *this);
        right = right.convert_to(nullable_bool_type, *this);
    }
    return with_rr(make<BinaryOperatorExpression>(left.expression, op, right.expression),
                   make<OperatorResolveResult>(nullable_bool_type, eop, nullptr, true, std::vector<ResolveResult*>{left.resolve_result, right.resolve_result}))
        .with_il_instruction(inst.self);
}

TranslatedExpression ExpressionBuilder::visit_throw(Throw& inst, TranslationContext) { return translate_throw(OperandFacts::of(inst)); }

TranslatedExpression ExpressionBuilder::translate_throw(const OperandFacts& inst) {
    return with_rr(with_il_instruction(make<ThrowExpression>(translate(inst.argument()).expression), inst.self), make<ThrowResolveResult>());
}

TranslatedExpression ExpressionBuilder::visit_user_defined_logic_operator(UserDefinedLogicOperator& inst, TranslationContext) { return translate_user_defined_logic_operator(LogicOperatorFacts::of(inst)); }

TranslatedExpression ExpressionBuilder::translate_user_defined_logic_operator(const LogicOperatorFacts& inst) {
    const IType* left_type = inst.method()->parameters()[0]->type();
    const IType* right_type = inst.method()->parameters()[1]->type();
    TranslatedExpression left = translate(inst.left(), left_type).convert_to(left_type, *this);
    TranslatedExpression right = translate(inst.right(), right_type).convert_to(right_type, *this);
    BinaryOperatorType op;
    if (inst.method()->name() == "op_BitwiseAnd") {
        op = BinaryOperatorType::ConditionalAnd;
    } else if (inst.method()->name() == "op_BitwiseOr") {
        op = BinaryOperatorType::ConditionalOr;
    } else {
        throw UsageError("Invalid method name");
    }
    return with_rr(make<BinaryOperatorExpression>(left.expression, op, right.expression),
                   make<InvocationResolveResult>(nullptr, inst.method(), std::vector<ResolveResult*>{left.resolve_result, right.resolve_result}))
        .with_il_instruction(inst.self);
}

ExpressionWithResolveResult ExpressionBuilder::assignment(TranslatedExpression left, TranslatedExpression right) {
    right = right.convert_to(left.type(), *this, false, true);
    return with_rr(make<AssignmentExpression>(left.expression, right.expression),
                   make<OperatorResolveResult>(left.type(), ExpressionType::Assign, std::vector<ResolveResult*>{left.resolve_result, right.resolve_result}));
}

TranslatedExpression ExpressionBuilder::visit_binary_numeric_instruction(BinaryNumericInstruction& inst, TranslationContext context) {
    return translate_binary_numeric(BinaryNumericFacts::of(inst), context);
}

TranslatedExpression ExpressionBuilder::translate_binary_numeric(const BinaryNumericFacts& inst, TranslationContext context) {
    switch (inst.op) {
        case BinaryNumericOperator::Add: return handle_binary_numeric(inst, BinaryOperatorType::Add, context);
        case BinaryNumericOperator::Sub: return handle_binary_numeric(inst, BinaryOperatorType::Subtract, context);
        case BinaryNumericOperator::Mul: return handle_binary_numeric(inst, BinaryOperatorType::Multiply, context);
        case BinaryNumericOperator::Div: {
            std::optional<TranslatedExpression> pointer_subtraction = handle_pointer_subtraction(inst);
            if (pointer_subtraction.has_value()) return *pointer_subtraction;
            return handle_binary_numeric(inst, BinaryOperatorType::Divide, context);
        }
        case BinaryNumericOperator::Rem: return handle_binary_numeric(inst, BinaryOperatorType::Modulus, context);
        case BinaryNumericOperator::BitAnd: return handle_binary_numeric(inst, BinaryOperatorType::BitwiseAnd, context);
        case BinaryNumericOperator::BitOr: return handle_binary_numeric(inst, BinaryOperatorType::BitwiseOr, context);
        case BinaryNumericOperator::BitXor: return handle_binary_numeric(inst, BinaryOperatorType::ExclusiveOr, context);
        case BinaryNumericOperator::ShiftLeft: return handle_shift(inst, BinaryOperatorType::ShiftLeft, context);
        case BinaryNumericOperator::ShiftRight: return handle_shift(inst, BinaryOperatorType::ShiftRight, context);
        default: throw std::out_of_range("BinaryNumericOperator");
    }
}

// Translates pointer arithmetic:
//   ptr + int
//   int + ptr
//   ptr - int
// Returns nullopt if 'inst' is not performing pointer arithmetic.
// 'ptr - ptr' is not handled here, but in handle_pointer_subtraction()!
std::optional<TranslatedExpression> ExpressionBuilder::handle_pointer_arithmetic(const BinaryNumericFacts& inst, TranslatedExpression left,
                                                                                  TranslatedExpression right, TranslationContext context) {
    if (!(inst.op == BinaryNumericOperator::Add || inst.op == BinaryNumericOperator::Sub)) return std::nullopt;
    if (inst.check_for_overflow || inst.is_lifted()) return std::nullopt;
    if (!(inst.left_input_type == StackType::I && inst.right_input_type == StackType::I)) return std::nullopt;
    const PointerType* pointer_type;
    ILInstruction* byte_offset_inst;
    TranslatedExpression byte_offset_expr;
    if (left.type()->kind() == TypeKind::Pointer) {
        byte_offset_inst = inst.right();
        byte_offset_expr = right;
        pointer_type = dynamic_cast<const PointerType*>(left.type());
    } else if (right.type()->kind() == TypeKind::Pointer) {
        if (inst.op != BinaryNumericOperator::Add) return std::nullopt;
        byte_offset_inst = inst.left();
        byte_offset_expr = left;
        pointer_type = dynamic_cast<const PointerType*>(right.type());
    } else {
        return std::nullopt;
    }
    std::optional<TranslatedExpression> offset_expression_from_type_hint;
    if (context.type_hint->kind() == TypeKind::Pointer) {
        // We use the type hint if one of the following is true:
        // * The current element type is a non-primitive struct.
        // * The current element type has a different size than the type hint element type.
        // This prevents the type hint from overriding in undesirable situations (eg changing char* to short*).

        auto* type_hint = dynamic_cast<const PointerType*>(context.type_hint);
        int element_type_size = get_size(*pointer_type->element_type());
        if (element_type_size == 0 || get_size(*type_hint->element_type()) != element_type_size) {
            offset_expression_from_type_hint = get_pointer_arithmetic_offset(byte_offset_inst, byte_offset_expr, type_hint->element_type(), inst.check_for_overflow);
            if (offset_expression_from_type_hint.has_value()) {
                pointer_type = type_hint;
            }
        }
    }
    std::optional<TranslatedExpression> offset_expr = offset_expression_from_type_hint;
    if (!offset_expr.has_value()) {
        offset_expr = get_pointer_arithmetic_offset(byte_offset_inst, byte_offset_expr, pointer_type->element_type(), inst.check_for_overflow);
    }
    if (!offset_expr.has_value()) {
        // FallBackToBytePointer
        pointer_type = dynamic_cast<const PointerType*>(make_pointer_type(known(KnownTypeCode::Byte)));
        offset_expr = ensure_integer_type(byte_offset_expr);
    }

    if (left.type()->kind() == TypeKind::Pointer) {
        assert(inst.op == BinaryNumericOperator::Add || inst.op == BinaryNumericOperator::Sub);
        left = left.convert_to(pointer_type, *this);
        right = *offset_expr;
    } else {
        assert(inst.op == BinaryNumericOperator::Add);
        assert(right.type()->kind() == TypeKind::Pointer);
        left = *offset_expr;
        right = right.convert_to(pointer_type, *this);
    }
    BinaryOperatorType operator_type = inst.op == BinaryNumericOperator::Add ? BinaryOperatorType::Add : BinaryOperatorType::Subtract;
    return with_il_instruction(make<BinaryOperatorExpression>(left.expression, operator_type, right.expression), inst.self)
        .with_rr(make<OperatorResolveResult>(pointer_type, BinaryOperatorExpression::get_linq_node_type(operator_type, inst.check_for_overflow),
                                             std::vector<ResolveResult*>{left.resolve_result, right.resolve_result}));
}

// Translates pointer arithmetic with managed pointers:
//   ref + int
//   int + ref
//   ref - int
//   ref - ref
std::optional<TranslatedExpression> ExpressionBuilder::handle_managed_pointer_arithmetic(const BinaryNumericFacts& inst, TranslatedExpression left,
                                                                                          TranslatedExpression right) {
    if (!(inst.op == BinaryNumericOperator::Add || inst.op == BinaryNumericOperator::Sub)) return std::nullopt;
    if (inst.check_for_overflow || inst.is_lifted()) return std::nullopt;
    auto get_reference_type = [&](const IType* type) -> const ByReferenceType* {
        if (auto* pt = dynamic_cast<const PointerType*>(type)) {
            return dynamic_cast<const ByReferenceType*>(make_by_reference_type(pt->element_type()));
        }
        return dynamic_cast<const ByReferenceType*>(make_by_reference_type(known(KnownTypeCode::Byte)));
    };
    if (inst.op == BinaryNumericOperator::Sub && inst.left_input_type == StackType::Ref && inst.right_input_type == StackType::Ref) {
        // ref - ref => i
        // ByteOffset() expects the parameters the wrong way around, so order using named arguments
        Expression* target_arg = make<NamedArgumentExpression>("target", left.expression);
        Expression* origin_arg = make<NamedArgumentExpression>("origin", right.expression);
        return call_unsafe_intrinsic("ByteOffset", {target_arg, origin_arg}, known(KnownTypeCode::IntPtr), inst.self);
    }
    if (inst.left_input_type == StackType::Ref && is_integer_type(inst.right_input_type)) {
        // ref [+-] int
        auto* brt = dynamic_cast<const ByReferenceType*>(left.type());
        if (brt == nullptr) {
            brt = get_reference_type(left.type());
            left = left.convert_to(brt, *this);
        }
        std::string name = (inst.op == BinaryNumericOperator::Sub ? "Subtract" : "Add");
        ILInstruction* offset_inst = PointerArithmeticOffset::detect(inst.right(), brt->element_type(), inst.check_for_overflow);
        if (offset_inst != nullptr) {
            const IType* element_type = nullptr;
            auto* ld_flda = il::as<LdFlda>(inst.left());
            LdFlda* nested_ld_flda = ld_flda != nullptr ? il::as<LdFlda>(ld_flda->target()) : nullptr;
            if (settings->fixed_buffers && inst.op == BinaryNumericOperator::Add && ld_flda != nullptr && nested_ld_flda != nullptr &&
                is_fixed_field(nested_ld_flda->field(), &element_type, nullptr)) {
                Expression* field_access = convert_field(nested_ld_flda->field(), nested_ld_flda->target()).expression;
                auto* mrr = static_cast<MemberResolveResult*>(get_resolve_result(field_access));
                field_access->remove_annotations<ResolveResult>();
                ExpressionWithResolveResult field_result = with_rr(
                    field_access, make<MemberResolveResult>(mrr->target_result(), mrr->member(), make_pointer_type(element_type)));
                TranslatedExpression result = field_result.with_il_instruction(inst.self);
                right = translate_array_index(offset_inst);
                TranslatedExpression expr = with_il_instruction(make<IndexerExpression>(result.expression, std::vector<Expression*>{right.expression}), inst.self)
                                                .with_rr(make<ResolveResult>(element_type));
                return without_il_instruction(make<DirectionExpression>(FieldDirection::Ref, expr.expression))
                    .with_rr(make<ByReferenceResolveResult>(expr.resolve_result, ReferenceKind::Ref));
            }
            right = translate(offset_inst);
            right = convert_array_index(right, inst.right_input_type, true);
            return call_unsafe_intrinsic(name, {left.expression, right.expression}, brt, inst.self);
        }
        right = convert_array_index(right, inst.right_input_type, true);
        return call_unsafe_intrinsic(name + "ByteOffset", {left.expression, right.expression}, brt, inst.self);
    }

    if (inst.left_input_type == StackType::I && inst.right_input_type == StackType::Ref && inst.op == BinaryNumericOperator::Add) {
        // int + ref
        auto* brt = dynamic_cast<const ByReferenceType*>(right.type());
        if (brt == nullptr) {
            brt = get_reference_type(right.type());
            right = right.convert_to(brt, *this);
        }
        ILInstruction* offset_inst = PointerArithmeticOffset::detect(inst.left(), brt->element_type(), inst.check_for_overflow);
        if (offset_inst != nullptr) {
            left = translate(offset_inst);
            left = convert_array_index(left, inst.left_input_type, true);
            Expression* offset_arg = make<NamedArgumentExpression>("elementOffset", left.expression);
            Expression* source_arg = make<NamedArgumentExpression>("source", right.expression);
            return call_unsafe_intrinsic("Add", {offset_arg, source_arg}, brt, inst.self);
        }
        left = convert_array_index(left, inst.left_input_type, true);
        Expression* offset_arg = make<NamedArgumentExpression>("byteOffset", left.expression);
        Expression* source_arg = make<NamedArgumentExpression>("source", right.expression);
        return call_unsafe_intrinsic("AddByteOffset", {offset_arg, source_arg}, brt, inst.self);
    }
    return std::nullopt;
}

TranslatedExpression ExpressionBuilder::call_unsafe_intrinsic(const std::string& name, const std::vector<Expression*>& arguments, const IType* return_type,
                                                               ILInstruction* inst, const std::vector<const IType*>& type_arguments) {
    auto* target = make<MemberReferenceExpression>();
    target->set_target(make<TypeReferenceExpression>(ast_builder.convert_type(known(KnownTypeCode::Unsafe))));
    target->set_member_name(name);
    for (const IType* type_argument : type_arguments) target->type_arguments().add(ast_builder.convert_type(type_argument));
    auto* invocation_expr = make<InvocationExpression>(target, arguments);
    ExpressionWithILInstruction invocation = inst != nullptr ? with_il_instruction(invocation_expr, inst) : without_il_instruction(invocation_expr);
    if (auto* brt = dynamic_cast<const ByReferenceType*>(return_type)) {
        return wrap_in_ref(invocation.with_rr(make<ResolveResult>(brt->element_type())), brt);
    }
    return invocation.with_rr(make<ResolveResult>(return_type));
}

TranslatedExpression ExpressionBuilder::ensure_integer_type(TranslatedExpression expr) {
    if (!is_csharp_primitive_integer_type(*expr.type()) && !is_csharp_native_integer_type(*expr.type())) {
        // pointer arithmetic accepts all primitive integer types, but no enums etc.
        expr = expr.convert_to(find_arithmetic_type(get_stack_type(*expr.type()), get_sign(*expr.type())), *this);
    }
    return expr;
}

std::optional<TranslatedExpression> ExpressionBuilder::get_pointer_arithmetic_offset(ILInstruction* byte_offset_inst, TranslatedExpression byte_offset_expr,
                                                                                      const IType* pointer_element_type, bool check_for_overflow,
                                                                                      bool unwrap_zero_extension) {
    ILInstruction* count_offset_inst = PointerArithmeticOffset::detect(byte_offset_inst, pointer_element_type, check_for_overflow, unwrap_zero_extension);
    if (count_offset_inst == nullptr) {
        return std::nullopt;
    }
    if (count_offset_inst == byte_offset_inst) {
        return ensure_integer_type(byte_offset_expr);
    }
    TranslatedExpression expr = translate(count_offset_inst);
    // Keep original ILInstruction as annotation
    expr.expression->remove_annotations<ILInstruction>();
    return ensure_integer_type(expr.with_il_instruction(byte_offset_inst));
}

// Called for divisions, detect and handles the code pattern:
//   div(sub(a, b), sizeof(T))
// when a,b are of type T*.
// This is what the C# compiler generates for pointer subtraction.
std::optional<TranslatedExpression> ExpressionBuilder::handle_pointer_subtraction(const BinaryNumericFacts& inst) {
    assert(inst.op == BinaryNumericOperator::Div);
    if (inst.check_for_overflow || inst.left_input_type != StackType::I) return std::nullopt;
    auto* sub = il::as<BinaryNumericInstruction>(inst.left());
    if (sub == nullptr || sub->op != BinaryNumericOperator::Sub) return std::nullopt;
    if (sub->check_for_overflow) return std::nullopt;
    // First, attempt to parse the 'sizeof' on the RHS
    const IType* element_type = nullptr;
    std::int64_t element_size = 0;
    if (inst.right()->match_ldc_i(&element_size)) {
        element_type = nullptr;
        // OK, might be pointer subtraction if the element size matches
    } else if (inst.right()->unwrap_conv(ConversionKind::SignExtend)->match_size_of(&element_type)) {
        // OK, might be pointer subtraction if the element type matches
    } else {
        return std::nullopt;
    }
    TranslatedExpression left = translate(sub->left());
    TranslatedExpression right = translate(sub->right());
    auto is_matching_pointer_type = [&](const IType* type) {
        if (auto* pt = dynamic_cast<const PointerType*>(type)) {
            if (element_type != nullptr) return element_type->equals(pt->element_type());
            if (element_size > 0) {
                std::optional<int> size = PointerArithmeticOffset::compute_size_of(*pt->element_type());
                return size.has_value() && *size == element_size;
            }
        }
        return false;
    };
    const IType* pointer_type;
    if (is_matching_pointer_type(left.type())) {
        pointer_type = left.type();
    } else if (is_matching_pointer_type(right.type())) {
        pointer_type = right.type();
    } else if (element_size == 1 && left.type()->kind() == TypeKind::Pointer && right.type()->kind() == TypeKind::Pointer) {
        // two pointers (neither matching), we're dividing by 1 (debug builds only),
        // -> subtract two byte pointers
        pointer_type = make_pointer_type(known(KnownTypeCode::Byte));
    } else {
        // neither is a matching pointer type
        // -> not a pointer subtraction after all
        return std::nullopt;
    }
    // We got a pointer subtraction.
    left = left.convert_to(pointer_type, *this);
    right = right.convert_to(pointer_type, *this);
    auto* rr = make<OperatorResolveResult>(known(KnownTypeCode::Int64), ExpressionType::Subtract,
                                           std::vector<ResolveResult*>{left.resolve_result, right.resolve_result});
    return with_il_instruction(make<BinaryOperatorExpression>(left.expression, BinaryOperatorType::Subtract, right.expression),
                               std::vector<ILInstruction*>{inst.self, sub})
        .with_rr(rr);
}

TranslatedExpression ExpressionBuilder::handle_binary_numeric(const BinaryNumericFacts& inst, BinaryOperatorType op, TranslationContext context) {
    CSharpResolver resolver_with_overflow_check = resolver->with_check_for_overflow(inst.check_for_overflow);
    bool propagate_type_hint = is_bitwise(op) && inst.left_input_type != inst.right_input_type;
    TranslatedExpression left = translate(inst.left(), propagate_type_hint ? context.type_hint : nullptr);
    TranslatedExpression right = translate(inst.right(), propagate_type_hint ? context.type_hint : nullptr);

    if (inst.underlying_result_type() == StackType::Ref) {
        std::optional<TranslatedExpression> ptr_result = handle_managed_pointer_arithmetic(inst, left, right);
        if (ptr_result.has_value()) return *ptr_result;
    }
    if (left.type()->kind() == TypeKind::Pointer || right.type()->kind() == TypeKind::Pointer) {
        std::optional<TranslatedExpression> ptr_result = handle_pointer_arithmetic(inst, left, right, context);
        if (ptr_result.has_value()) return *ptr_result;
    }

    left = prepare_arithmetic_argument(left, inst.left_input_type, inst.sign, inst.is_lifted());
    right = prepare_arithmetic_argument(right, inst.right_input_type, inst.sign, inst.is_lifted());

    if (op == BinaryOperatorType::Subtract && inst.left()->match_ldc_i(std::int64_t{0})) {
        const IType* right_utype = nullable_type::get_underlying_type(right.type());
        if (is_known_type(right_utype, KnownTypeCode::Int32) || is_known_type(right_utype, KnownTypeCode::Int64) ||
            is_csharp_small_integer_type(*right_utype) || right_utype->kind() == TypeKind::NInt) {
            // unary minus is supported on signed int, nint and long, and on the small integer types (since they promote to int)
            auto* uoe = make<UnaryOperatorExpression>(UnaryOperatorType::Minus, right.expression);
            annotate_checked(uoe, inst.check_for_overflow ? CheckedAnnotationKind::Checked : CheckedAnnotationKind::Unchecked);
            const IType* result_type = find_arithmetic_type(inst.right_input_type, Sign::Signed);
            if (inst.is_lifted()) result_type = nullable_type::create(*compilation, result_type);
            return with_il_instruction(uoe, inst.self)
                .with_rr(make<OperatorResolveResult>(result_type, inst.check_for_overflow ? ExpressionType::NegateChecked : ExpressionType::Negate,
                                                     std::vector<ResolveResult*>{right.resolve_result}));
        }
    }
    if (is_bitwise(op) && (left.type()->kind() == TypeKind::Enum || right.type()->kind() == TypeKind::Enum)) {
        left = adjust_constant_expression_to_type(left, right.type());
        right = adjust_constant_expression_to_type(right, left.type());
    }

    ResolveResult* rr = resolver_with_overflow_check.resolve_binary_operator(op, left.resolve_result, right.resolve_result);
    if (rr->is_error() || get_stack_type(*nullable_type::get_underlying_type(rr->type())) != inst.underlying_result_type() ||
        !is_compatible_with_sign(rr->type(), inst.sign)) {
        // Left and right operands are incompatible, so convert them to a common type
        Sign sign = inst.sign;
        if (sign == Sign::None) {
            // If the sign doesn't matter, try to use the same sign as expected by the context
            sign = get_sign(*context.type_hint);
            if (sign == Sign::None) {
                sign = is_bitwise(op) ? Sign::Unsigned : Sign::Signed;
            }
        }
        const IType* target_type = find_arithmetic_type(inst.underlying_result_type(), sign);
        left = left.convert_to(nullable_type::is_nullable(left.type()) ? nullable_type::create(*compilation, target_type) : target_type, *this);
        right = right.convert_to(nullable_type::is_nullable(right.type()) ? nullable_type::create(*compilation, target_type) : target_type, *this);
        rr = resolver_with_overflow_check.resolve_binary_operator(op, left.resolve_result, right.resolve_result);
    }
    if (is_bitwise(op)) {
        if (has_value(left.resolve_result->constant_value())) {
            std::int64_t value = to_int64(left.resolve_result->constant_value());
            std::vector<ILInstruction*> instructions = left.il_instructions();
            left = convert_constant_value(left.resolve_result, false, should_display_as_hex(value, left.type())).with_il_instruction(instructions);
        }
        if (has_value(right.resolve_result->constant_value())) {
            std::int64_t value = to_int64(right.resolve_result->constant_value());
            std::vector<ILInstruction*> instructions = right.il_instructions();
            right = convert_constant_value(right.resolve_result, false, should_display_as_hex(value, right.type())).with_il_instruction(instructions);
        }
    }
    TranslatedExpression result_expr =
        with_il_instruction(make<BinaryOperatorExpression>(left.expression, op, right.expression), inst.self).with_rr(rr);
    if (binary_operator_might_check_for_overflow(op) && !is_float_type(inst.underlying_result_type())) {
        CheckedAnnotationKind annotation;
        if (inst.check_for_overflow) {
            annotation = CheckedAnnotationKind::Checked;
        } else if (rr->is_compile_time_constant() && constant_binary_operator_overflows(op, left.resolve_result, right.resolve_result)) {
            // A compile-time constant subexpression is always evaluated in a checked context,
            // even within an (implicitly) unchecked context, so emitting it bare would fail to
            // compile with CS0220 ("operation overflows at compile time in checked mode").
            // Force an explicit unchecked(...) wrapper, just like an overflowing constant
            // n(u)int cast. This typically happens after inlining turns a runtime accumulator
            // (e.g. a GetHashCode prime chain) into a constant subexpression.
            annotation = CheckedAnnotationKind::ExplicitUnchecked;
        } else {
            annotation = CheckedAnnotationKind::Unchecked;
        }
        annotate_checked(result_expr.expression, annotation);
    }
    return result_expr;
}

// Gets a type matching the stack type and sign.
const IType* ExpressionBuilder::find_type(StackType stack_type, Sign sign) {
    if (stack_type == StackType::I && settings->native_integers) {
        return sign == Sign::Unsigned ? &SpecialType::nuint() : &SpecialType::nint();
    }
    return il::transforms::find_type_of_stack_type(*compilation, stack_type, sign);
}

// Gets a type used for performing arithmetic with the stack type and sign.
//
// This may result in a larger type than requested when the selected C# version
// doesn't support native integers.
// Should only be used after a call to prepare_arithmetic_argument()
// to ensure that we're not preserving extra bits from an oversized TranslatedExpression.
const IType* ExpressionBuilder::find_arithmetic_type(StackType stack_type, Sign sign) {
    if (stack_type == StackType::I) {
        if (settings->native_integers) {
            return sign == Sign::Unsigned ? &SpecialType::nuint() : &SpecialType::nint();
        }
        // If native integers are not available, use 64-bit arithmetic instead
        stack_type = StackType::I8;
    }
    return il::transforms::find_type_of_stack_type(*compilation, stack_type, sign);
}

// Handle oversized arguments needing truncation; and avoid IntPtr/pointers in arguments.
TranslatedExpression ExpressionBuilder::prepare_arithmetic_argument(TranslatedExpression arg, StackType arg_stack_type, Sign sign, bool is_lifted) {
    if (is_lifted && !nullable_type::is_nullable(arg.type())) {
        is_lifted = false;  // don't cast to nullable if this input wasn't already nullable
    }
    const IType* arg_utype = is_lifted ? nullable_type::get_underlying_type(arg.type()) : arg.type();
    if (is_integer_type(arg_stack_type) && get_size(arg_stack_type) < get_size(*arg_utype)) {
        // If the argument is oversized (needs truncation to match stack size of its ILInstruction),
        // perform the truncation now.
        const IType* target_type = find_type(arg_stack_type, sign);
        arg_utype = target_type;
        if (is_lifted) target_type = nullable_type::create(*compilation, target_type);
        arg = arg.convert_to(target_type, *this);
    }
    if (is_known_type(arg_utype, KnownTypeCode::IntPtr) || is_known_type(arg_utype, KnownTypeCode::UIntPtr)) {
        // None of the operators we might want to apply are supported by IntPtr/UIntPtr.
        // Also, pointer arithmetic has different semantics (works in number of elements, not bytes).
        // So any inputs of size StackType.I must be converted to long/ulong.
        const IType* target_type = find_arithmetic_type(StackType::I, sign);
        if (is_lifted) target_type = nullable_type::create(*compilation, target_type);
        arg = arg.convert_to(target_type, *this);
    }
    return arg;
}

// Gets whether `type` has the specified `sign`. If `sign` is None, always returns true.
bool ExpressionBuilder::is_compatible_with_sign(const IType* type, Sign sign) {
    return sign == Sign::None || get_sign(*nullable_type::get_underlying_type(type)) == sign;
}

bool ExpressionBuilder::binary_operator_might_check_for_overflow(BinaryOperatorType op) {
    switch (op) {
        case BinaryOperatorType::BitwiseAnd:
        case BinaryOperatorType::BitwiseOr:
        case BinaryOperatorType::ExclusiveOr:
        case BinaryOperatorType::ShiftLeft:
        case BinaryOperatorType::ShiftRight:
        case BinaryOperatorType::UnsignedShiftRight: return false;
        default: return true;
    }
}

// Returns true if the (already unchecked-resolved) constant binary operation overflows when
// evaluated in a checked context. Used to decide whether an explicit unchecked(...) wrapper is
// required: a compile-time constant subexpression is always overflow-checked at compile time,
// regardless of the surrounding (implicitly) unchecked context.
bool ExpressionBuilder::constant_binary_operator_overflows(BinaryOperatorType op, ResolveResult* left, ResolveResult* right) {
    if (!has_value(left->constant_value()) || !has_value(right->constant_value())) return false;
    ResolveResult* checked_result = resolver->with_check_for_overflow(true).resolve_binary_operator(op, left, right);
    return checked_result->is_error();
}

TranslatedExpression ExpressionBuilder::handle_shift(const BinaryNumericFacts& inst, BinaryOperatorType op, TranslationContext context) {
    TranslatedExpression left = translate(inst.left());
    TranslatedExpression right = translate(inst.right());

    left = prepare_arithmetic_argument(left, inst.left_input_type, inst.sign, inst.is_lifted());

    Sign sign = inst.sign;
    const IType* left_utype = nullable_type::get_underlying_type(left.type());
    bool could_use_unsigned_right_shift =
        (sign == Sign::Unsigned && op == BinaryOperatorType::ShiftRight && settings->unsigned_right_shift &&
         (is_csharp_primitive_integer_type(*left_utype) || is_csharp_native_integer_type(*left_utype))
         // If we need to cast to unsigned anyway, don't use >>> operator.
         && get_sign(*context.type_hint) != Sign::Unsigned);
    if (is_csharp_small_integer_type(*left_utype) && inst.underlying_result_type() == StackType::I4 &&
        (sign != Sign::Unsigned || could_use_unsigned_right_shift)) {
        // With small integer types, C# will promote to int and perform signed shifts.
        // We thus don't need any casts in this case.
        // The >>> operator also promotes to signed int, but then performs an unsigned shift.
        if (sign == Sign::Unsigned) {
            op = BinaryOperatorType::UnsignedShiftRight;
        }
    } else if (could_use_unsigned_right_shift && get_size(*left_utype) == get_size(inst.underlying_result_type()) && get_sign(*left_utype) == Sign::Signed) {
        // Use C# 11 unsigned right shift operator. We don't need any casts in this case.
        op = BinaryOperatorType::UnsignedShiftRight;
    } else {
        // Insert cast to target type.
        if (sign == Sign::None) {
            // if we don't need a specific sign, prefer keeping that of the input:
            sign = get_sign(*left_utype);
        }
        const IType* target_type = find_arithmetic_type(inst.underlying_result_type(), sign);
        if (nullable_type::is_nullable(left.type())) {
            target_type = nullable_type::create(*compilation, target_type);
        }
        left = left.convert_to(target_type, *this);
    }

    // Shift operators in C# always expect type 'int' on the right-hand-side
    if (nullable_type::is_nullable(right.type())) {
        right = right.convert_to(nullable_type::create(*compilation, known(KnownTypeCode::Int32)), *this);
    } else {
        right = right.convert_to(known(KnownTypeCode::Int32), *this);
    }

    return with_il_instruction(make<BinaryOperatorExpression>(left.expression, op, right.expression), inst.self)
        .with_rr(resolver->resolve_binary_operator(op, left.resolve_result, right.resolve_result));
}

TranslatedExpression ExpressionBuilder::visit_user_defined_compound_assign(UserDefinedCompoundAssign& inst, TranslationContext) {
    return translate_user_defined_compound_assign(UserDefinedCompoundAssignFacts::of(inst));
}

TranslatedExpression ExpressionBuilder::translate_user_defined_compound_assign(const UserDefinedCompoundAssignFacts& inst) {
    const IType* load_type;
    bool is_span_based_string_concat = CallBuilder::is_span_based_string_concat(inst.method);
    if (is_span_based_string_concat) {
        load_type = type_system_->find_type(KnownTypeCode::String);
    } else {
        load_type = inst.method->parameters()[0]->type();
    }

    ExpressionWithResolveResult target;
    if (inst.target_kind == CompoundTargetKind::Address) {
        target = ld_obj(inst.target(), load_type);
    } else {
        target = translate(inst.target(), load_type);
    }
    std::optional<OperatorType> op_type = OperatorDeclaration::get_operator_type(inst.method->name());
    if (op_type.has_value() && OperatorDeclaration::is_checked(*op_type)) {
        annotate_checked(target.expression, CheckedAnnotationKind::Checked);
    } else if (has_checked_equivalent(inst.method)) {
        annotate_checked(target.expression, CheckedAnnotationKind::Unchecked);
    }
    if (UserDefinedCompoundAssign::is_string_concat(inst.method)) {
        assert(inst.method->parameters().size() == 2);
        Expression* value_expr;
        ResolveResult* value_resolve_result;
        auto* new_obj = il::as<NewObj>(inst.value());
        AddressOf* address_of = new_obj != nullptr && new_obj->arguments().count() == 1 ? il::as<AddressOf>(new_obj->arguments()[0]) : nullptr;
        if (is_span_based_string_concat && address_of != nullptr) {
            const IType* char_type = type_system_->find_type(KnownTypeCode::Char);
            TranslatedExpression value = translate(address_of->value(), char_type).convert_to(char_type, *this);
            value_expr = value.expression;
            value_resolve_result = value.resolve_result;
        } else {
            TranslatedExpression value = translate(inst.value()).convert_to(inst.method->parameters()[1]->type(), *this, false, true);
            value_expr = detach(remove_redundant_to_string_in_concat(value, inst.method, true));
            value_resolve_result = value.resolve_result;
        }
        return with_il_instruction(make<AssignmentExpression>(target.expression, AssignmentOperatorType::Add, value_expr), inst.self)
            .with_rr(make<OperatorResolveResult>(inst.method->return_type(), ExpressionType::AddAssign, inst.method, inst.is_lifted(),
                                                 std::vector<ResolveResult*>{target.resolve_result, value_resolve_result}));
    } else if (inst.method->parameters().size() == 2) {
        TranslatedExpression value = translate(inst.value()).convert_to(inst.method->parameters()[1]->type(), *this);
        std::optional<AssignmentOperatorType> op = get_assignment_operator_type_from_metadata_name(inst.method->name(), *settings);
        assert(op.has_value());

        return with_il_instruction(make<AssignmentExpression>(target.expression, *op, value.expression), inst.self)
            .with_rr(make<OperatorResolveResult>(inst.method->return_type(), AssignmentExpression::get_linq_node_type(*op, false), inst.method,
                                                 inst.is_lifted(), std::vector<ResolveResult*>{target.resolve_result, value.resolve_result}));
    }
    std::optional<UnaryOperatorType> op = get_unary_operator_type_from_metadata_name(inst.method->name(), inst.eval_mode == CompoundEvalMode::EvaluatesToOldValue);
    assert(op.has_value());

    return with_il_instruction(make<UnaryOperatorExpression>(*op, target.expression), inst.self)
        .with_rr(make<OperatorResolveResult>(inst.method->return_type(), UnaryOperatorExpression::get_linq_node_type(*op, false), inst.method,
                                             inst.is_lifted(), std::vector<ResolveResult*>{target.resolve_result}));
}

std::optional<AssignmentOperatorType> ExpressionBuilder::get_assignment_operator_type_from_metadata_name(const std::string& name,
                                                                                                         const DecompilerSettings& settings) {
    if (name == "op_Addition") return AssignmentOperatorType::Add;
    if (name == "op_Subtraction") return AssignmentOperatorType::Subtract;
    if (name == "op_Multiply") return AssignmentOperatorType::Multiply;
    if (name == "op_Division") return AssignmentOperatorType::Divide;
    if (name == "op_Modulus") return AssignmentOperatorType::Modulus;
    if (name == "op_BitwiseAnd") return AssignmentOperatorType::BitwiseAnd;
    if (name == "op_BitwiseOr") return AssignmentOperatorType::BitwiseOr;
    if (name == "op_ExclusiveOr") return AssignmentOperatorType::ExclusiveOr;
    if (name == "op_LeftShift") return AssignmentOperatorType::ShiftLeft;
    if (name == "op_RightShift") return AssignmentOperatorType::ShiftRight;
    if (name == "op_UnsignedRightShift" && settings.unsigned_right_shift) return AssignmentOperatorType::UnsignedShiftRight;
    return std::nullopt;
}

std::optional<UnaryOperatorType> ExpressionBuilder::get_unary_operator_type_from_metadata_name(const std::string& name, bool is_postfix) {
    if (name == "op_Increment" || name == "op_CheckedIncrement") return is_postfix ? UnaryOperatorType::PostIncrement : UnaryOperatorType::Increment;
    if (name == "op_Decrement" || name == "op_CheckedDecrement") return is_postfix ? UnaryOperatorType::PostDecrement : UnaryOperatorType::Decrement;
    return std::nullopt;
}

TranslatedExpression ExpressionBuilder::visit_numeric_compound_assign(NumericCompoundAssign& inst, TranslationContext) {
    return translate_numeric_compound_assign(NumericCompoundAssignFacts::of(inst));
}

TranslatedExpression ExpressionBuilder::translate_numeric_compound_assign(const NumericCompoundAssignFacts& inst) {
    switch (inst.op) {
        case BinaryNumericOperator::Add: return handle_compound_assignment(inst, AssignmentOperatorType::Add);
        case BinaryNumericOperator::Sub: return handle_compound_assignment(inst, AssignmentOperatorType::Subtract);
        case BinaryNumericOperator::Mul: return handle_compound_assignment(inst, AssignmentOperatorType::Multiply);
        case BinaryNumericOperator::Div: return handle_compound_assignment(inst, AssignmentOperatorType::Divide);
        case BinaryNumericOperator::Rem: return handle_compound_assignment(inst, AssignmentOperatorType::Modulus);
        case BinaryNumericOperator::BitAnd: return handle_compound_assignment(inst, AssignmentOperatorType::BitwiseAnd);
        case BinaryNumericOperator::BitOr: return handle_compound_assignment(inst, AssignmentOperatorType::BitwiseOr);
        case BinaryNumericOperator::BitXor: return handle_compound_assignment(inst, AssignmentOperatorType::ExclusiveOr);
        case BinaryNumericOperator::ShiftLeft: return handle_compound_shift(inst, AssignmentOperatorType::ShiftLeft);
        case BinaryNumericOperator::ShiftRight:
            if (inst.sign == Sign::Unsigned && get_sign(*inst.type()) == Sign::Signed) {
                assert(settings->unsigned_right_shift);
                return handle_compound_shift(inst, AssignmentOperatorType::UnsignedShiftRight);
            } else if (inst.sign == Sign::Unsigned && is_csharp_small_integer_type(*inst.type()) && settings->unsigned_right_shift) {
                // For small unsigned integer types promoted to signed int, the sign bit will be zero,
                // so there is no difference between signed and unsigned shift.
                // However the IL still indicates which C# operator was used, so preserve that if the setting allows us to.
                return handle_compound_shift(inst, AssignmentOperatorType::UnsignedShiftRight);
            }
            return handle_compound_shift(inst, AssignmentOperatorType::ShiftRight);
        default: throw std::out_of_range("BinaryNumericOperator");
    }
}

TranslatedExpression ExpressionBuilder::handle_compound_assignment(const NumericCompoundAssignFacts& inst, AssignmentOperatorType op) {
    ExpressionWithResolveResult target;
    if (inst.target_kind == CompoundTargetKind::Address) {
        target = ld_obj(inst.target(), inst.type());
    } else {
        target = translate(inst.target(), inst.type());
    }

    TranslatedExpression result_expr;
    if (inst.eval_mode == CompoundEvalMode::EvaluatesToOldValue) {
        assert(op == AssignmentOperatorType::Add || op == AssignmentOperatorType::Subtract);
#ifndef NDEBUG
        if (auto* ptr_type = dynamic_cast<const PointerType*>(target.type())) {
            ILInstruction* inst_value = PointerArithmeticOffset::detect(inst.value(), ptr_type->element_type(), inst.check_for_overflow);
            assert(inst_value != nullptr);
            assert(inst_value->match_ldc_i(std::int64_t{1}));
        } else {
            assert(inst.value()->match_ldc_i(std::int64_t{1}) || inst.value()->match_ldc_f4(1.0f) || inst.value()->match_ldc_f8(1.0));
        }
#endif
        UnaryOperatorType unary;
        ExpressionType expr_type;
        if (op == AssignmentOperatorType::Add) {
            unary = UnaryOperatorType::PostIncrement;
            expr_type = ExpressionType::PostIncrementAssign;
        } else {
            unary = UnaryOperatorType::PostDecrement;
            expr_type = ExpressionType::PostDecrementAssign;
        }
        result_expr = with_il_instruction(make<UnaryOperatorExpression>(unary, target.expression), inst.self)
                          .with_rr(make<OperatorResolveResult>(target.type(), expr_type, std::vector<ResolveResult*>{target.resolve_result}));
    } else {
        TranslatedExpression value = translate(inst.value());
        value = prepare_arithmetic_argument(value, inst.right_input_type, inst.sign, inst.is_lifted());
        auto convert_value = [&](TranslatedExpression v, const IType* target_type) {
            bool allow_implicit_conversion = true;
            if (get_stack_type(*target_type) == StackType::I) {
                // Force explicit cast for (U)IntPtr, keep allowing implicit conversion only for n(u)int
                allow_implicit_conversion = is_csharp_native_integer_type(*target_type);
                target_type = get_sign(*target_type) == Sign::Unsigned ? &SpecialType::nuint() : &SpecialType::nint();
            }
            if (nullable_type::is_nullable(v.type())) {
                target_type = nullable_type::create(*compilation, target_type);
            }
            return v.convert_to(target_type, *this, inst.check_for_overflow, allow_implicit_conversion);
        };
        switch (op) {
            case AssignmentOperatorType::Add:
            case AssignmentOperatorType::Subtract:
                if (target.type()->kind() == TypeKind::Pointer) {
                    std::optional<TranslatedExpression> pao = get_pointer_arithmetic_offset(
                        inst.value(), value, dynamic_cast<const PointerType*>(target.type())->element_type(), inst.check_for_overflow);
                    if (pao.has_value()) {
                        value = *pao;
                    } else {
                        value.expression->add_trailing_trivia(make<Comment>("ILSpy Error: GetPointerArithmeticOffset() failed", CommentType::MultiLine));
                    }
                } else {
                    const IType* target_type = get_enum_underlying_type(*nullable_type::get_underlying_type(target.type()));
                    value = convert_value(value, target_type);
                }
                break;
            case AssignmentOperatorType::Multiply:
            case AssignmentOperatorType::Divide:
            case AssignmentOperatorType::Modulus:
            case AssignmentOperatorType::BitwiseAnd:
            case AssignmentOperatorType::BitwiseOr:
            case AssignmentOperatorType::ExclusiveOr: {
                const IType* target_type = nullable_type::get_underlying_type(target.type());
                value = convert_value(value, target_type);
                break;
            }
            default: break;
        }
        result_expr = with_il_instruction(make<AssignmentExpression>(target.expression, op, value.expression), inst.self)
                          .with_rr(make<OperatorResolveResult>(target.type(), AssignmentExpression::get_linq_node_type(op, inst.check_for_overflow),
                                                               std::vector<ResolveResult*>{target.resolve_result, value.resolve_result}));
    }
    if (assignment_operator_might_check_for_overflow(op) && !is_float_type(inst.underlying_result_type())) {
        annotate_checked(result_expr.expression, inst.check_for_overflow ? CheckedAnnotationKind::Checked : CheckedAnnotationKind::Unchecked);
    }
    return result_expr;
}

TranslatedExpression ExpressionBuilder::handle_compound_shift(const NumericCompoundAssignFacts& inst, AssignmentOperatorType op) {
    assert(inst.eval_mode == CompoundEvalMode::EvaluatesToNewValue);
    ExpressionWithResolveResult target;
    if (inst.target_kind == CompoundTargetKind::Address) {
        target = ld_obj(inst.target(), inst.type());
    } else {
        target = translate(inst.target(), inst.type());
    }
    TranslatedExpression value = translate(inst.value());

    // Shift operators in C# always expect type 'int' on the right-hand-side
    if (nullable_type::is_nullable(value.type())) {
        value = value.convert_to(nullable_type::create(*compilation, known(KnownTypeCode::Int32)), *this);
    } else {
        value = value.convert_to(known(KnownTypeCode::Int32), *this);
    }

    return with_il_instruction(make<AssignmentExpression>(target.expression, op, value.expression), inst.self)
        .with_rr(resolver->resolve_assignment(op, target.resolve_result, value.resolve_result));
}

bool ExpressionBuilder::assignment_operator_might_check_for_overflow(AssignmentOperatorType op) {
    switch (op) {
        case AssignmentOperatorType::BitwiseAnd:
        case AssignmentOperatorType::BitwiseOr:
        case AssignmentOperatorType::ExclusiveOr:
        case AssignmentOperatorType::ShiftLeft:
        case AssignmentOperatorType::ShiftRight: return false;
        default: return true;
    }
}

TranslatedExpression ExpressionBuilder::visit_conv(Conv& inst, TranslationContext context) { return translate_conv(ConvFacts::of(inst), context); }

TranslatedExpression ExpressionBuilder::translate_conv(const ConvFacts& inst, TranslationContext context) {
    Sign hint_sign = inst.input_sign;
    if (hint_sign == Sign::None) {
        hint_sign = get_sign(*context.type_hint);
    }
    TranslatedExpression arg = translate(inst.argument(), find_arithmetic_type(inst.input_type, hint_sign));
    const IType* input_type = nullable_type::get_underlying_type(arg.type());
    StackType input_stack_type = inst.input_type;
    // Note: we're dealing with two conversions here:
    // a) the implicit conversion from `input_type` to `input_stack_type`
    //    (due to the ExpressionBuilder post-condition being flexible with regards to the integer type width)
    //    If this is a widening conversion, I'm calling the argument C# type "oversized".
    //    If this is a narrowing conversion, I'm calling the argument C# type "undersized".
    // b) the actual conversion instruction from `input_stack_type` to `inst.target_type`

    // Also, we need to be very careful with regards to the conversions we emit:
    // In C#, zero vs. sign-extension depends on the input type,
    // but in the ILAst conv instruction it depends on the output type.
    // However, in the conv.ovf instructions, the .NET runtime behavior seems to depend on the input type,
    // in violation of the ECMA-335 spec!

    auto get_type = [&](KnownTypeCode type_code) {
        const IType* type = known(type_code);
        // Prefer n(u)int over (U)IntPtr
        if (type_code == KnownTypeCode::IntPtr && settings->native_integers && !type->equals(context.type_hint)) {
            type = &SpecialType::nint();
        } else if (type_code == KnownTypeCode::UIntPtr && settings->native_integers && !type->equals(context.type_hint)) {
            type = &SpecialType::nuint();
        }
        if (inst.is_lifted()) {
            type = nullable_type::create(*compilation, type);
        }
        return type;
    };

    if (inst.check_for_overflow || inst.kind == ConversionKind::IntToFloat) {
        // We need to first convert the argument to the expected sign.
        // We also need to perform any input narrowing conversion so that it doesn't get mixed up with the overflow check.
        assert(inst.input_sign != Sign::None);
        if (get_size(*input_type) > get_size(input_stack_type) || get_sign(*input_type) != inst.input_sign) {
            arg = arg.convert_to(get_type(to_known_type_code(input_stack_type, inst.input_sign)), *this);
        }
        // Because casts with overflow check match C# semantics (zero/sign-extension depends on source type),
        // we can just directly cast to the target type.
        return arg.convert_to(get_type(to_known_type_code(inst.target_type)), *this, inst.check_for_overflow).with_il_instruction(inst.self);
    }

    // The `default:` branch of the switch below (reached by `goto default` upstream).
    auto convert_to_target_type = [&]() -> TranslatedExpression {
        // We need to convert to inst.target_type, or to an equivalent type.
        const IType* target_type;
        if (inst.target_type == to_primitive_type(*nullable_type::get_underlying_type(context.type_hint)) &&
            nullable_type::is_nullable(context.type_hint) == inst.is_lifted()) {
            target_type = context.type_hint;
        } else if (inst.target_type == il::PrimitiveType::Ref) {
            // converting to unknown ref-type
            target_type = make_by_reference_type(known(KnownTypeCode::Byte));
        } else if (inst.target_type == il::PrimitiveType::None) {
            // convert to some object type
            // (e.g. invalid I4->O conversion)
            target_type = known(KnownTypeCode::Object);
        } else {
            target_type = get_type(to_known_type_code(inst.target_type));
        }
        return arg.convert_to(target_type, *this, inst.check_for_overflow).with_il_instruction(inst.self);
    };

    switch (inst.kind) {
        case ConversionKind::StartGCTracking:
            // A "start gc tracking" conversion is inserted in the ILAst whenever
            // some instruction expects a managed pointer, but we pass an unmanaged pointer.
            // We'll leave the C#-level conversion (from T* to ref T) to the consumer that expects the managed pointer.
            return arg;
        case ConversionKind::StopGCTracking:
            if (input_type->kind() == TypeKind::ByReference) {
                if (PointerArithmeticOffset::is_fixed_variable(inst.argument())) {
                    // cast to corresponding pointer type:
                    const IType* pointer_type = make_pointer_type(dynamic_cast<const ByReferenceType*>(input_type)->element_type());
                    return arg.convert_to(pointer_type, *this).with_il_instruction(inst.self);
                }
                // emit Unsafe.AsPointer() intrinsic:
                return call_unsafe_intrinsic("AsPointer", {arg.expression}, make_pointer_type(known(KnownTypeCode::Void)), inst.self);
            } else if (is_integer_type(get_stack_type(*arg.type()))) {
                // ConversionKind.StopGCTracking should only be used with managed references,
                // but it's possible that we're supposed to stop tracking something we just started to track.
                return arg;
            }
            return convert_to_target_type();
        case ConversionKind::SignExtend:
            // We just need to ensure the input type before the conversion is signed.
            // Also, if the argument was translated into an oversized C# type,
            // we need to perform the truncatation to the input stack type.
            if (get_sign(*input_type) != Sign::Signed || value_might_be_oversized(arg.resolve_result, input_stack_type)) {
                // Note that an undersized C# type is handled just fine:
                // If it is unsigned we'll zero-extend it to the width of the input_stack_type here,
                // and it is signed we just combine the two sign-extensions into a single sign-extending conversion.
                arg = arg.convert_to(get_type(to_known_type_code(input_stack_type, Sign::Signed)), *this);
            }
            // Then, we can just return the argument as-is: the ExpressionBuilder post-condition allows us
            // to force our parent instruction to handle the actual sign-extension conversion.
            // (our caller may have more information to pick a better fitting target type)
            return arg.with_il_instruction(inst.self);
        case ConversionKind::ZeroExtend:
            // If overflow check cannot fail, handle this just like sign extension (except for swapped signs)
            if (get_sign(*input_type) != Sign::Unsigned || get_size(*input_type) > get_size(input_stack_type)) {
                arg = arg.convert_to(get_type(to_known_type_code(input_stack_type, Sign::Unsigned)), *this);
            }
            return arg.with_il_instruction(inst.self);
        case ConversionKind::Nop:
            // no need to generate any C# code for a nop conversion
            return arg.with_il_instruction(inst.self);
        case ConversionKind::Truncate:
            // Note: there are three sizes involved here:
            // A = input_type size
            // B = input_stack_type size
            // C = inst.target_type size.
            // We know that C < B (otherwise this wouldn't be the truncation case).
            // 1) If C < B < A, we just combine the two truncations into one.
            // 2) If C < B = A, there's no input conversion, just the truncation
            // 3) If C <= A < B, all the extended bits get removed again by the truncation.
            // 4) If A < C < B, some extended bits remain even after truncation.
            // In cases 1-3, the overall conversion is a truncation or no-op.
            // In case 4, the overall conversion is a zero/sign extension, but to a smaller
            // size than the original conversion.
            if (is_small_integer_type(inst.target_type)) {
                // If the target type is a small integer type, IL will implicitly sign- or zero-extend
                // the result after the truncation back to StackType.I4.
                // (which means there's actually 3 conversions involved!)
                // Note that we must handle truncation to small integer types ourselves:
                // our caller only sees the StackType.I4 and doesn't know to truncate to the small type.

                if (get_size(*input_type) <= get_size(inst.target_type) && get_sign(*input_type) == get_sign(inst.target_type)) {
                    // There's no actual truncation involved, and the result of the Conv instruction is extended
                    // the same way as the original instruction
                    // -> we can return arg directly
                    return arg.with_il_instruction(inst.self);
                }
                // We need to actually truncate; *or* we need to change the sign for the remaining extension to I4.
                return convert_to_target_type();  // Emit simple cast to inst.target_type
            }
            assert(get_size(inst.target_type) == get_size(inst.underlying_result_type()));
            // For non-small integer types, we can let the whole unchecked truncation
            // get handled by our caller (using the ExpressionBuilder post-condition).

            // Case 4 (left-over extension from implicit conversion) can also be handled by our caller.
            return arg.with_il_instruction(inst.self);
        case ConversionKind::Invalid:
            if (inst.input_type == StackType::Unknown && inst.target_type == il::PrimitiveType::None && arg.type()->kind() == TypeKind::Unknown) {
                // Unknown -> O conversion.
                // Our post-condition allows us to also use expressions with unknown type where O is expected,
                // so avoid introducing an `(object)` cast because we're likely to cast back to the same unknown type,
                // just in a signature context where we know that it's a class type.
                return arg.with_il_instruction(inst.self);
            }
            return convert_to_target_type();
        default: return convert_to_target_type();
    }
}

// Gets whether the ResolveResult computes a value that might be oversized for the specified stack type.
bool ExpressionBuilder::value_might_be_oversized(ResolveResult* rr, StackType stack_type) {
    const IType* input_type = nullable_type::get_underlying_type(rr->type());
    if (get_size(*input_type) <= get_size(stack_type)) {
        // The input type is smaller or equal to the stack type,
        // it can't be an oversized value.
        return false;
    }
    if (auto* orr = dynamic_cast<OperatorResolveResult*>(rr)) {
        if (stack_type == StackType::I && orr->operator_type() == ExpressionType::Subtract && orr->operands().size() == 2 &&
            orr->operands()[0]->type()->kind() == TypeKind::Pointer && orr->operands()[1]->type()->kind() == TypeKind::Pointer) {
            // Even though a pointer subtraction produces a value of type long in C#,
            // the value will always fit in a native int.
            return false;
        }
    }
    // We don't have any information about the value, so it might be oversized.
    return true;
}

TranslatedExpression ExpressionBuilder::visit_call(Call& inst, TranslationContext context) { return translate_call(CallFacts::of(inst), context); }

TranslatedExpression ExpressionBuilder::visit_call_virt(CallVirt& inst, TranslationContext context) { return translate_call(CallFacts::of(inst), context); }

TranslatedExpression ExpressionBuilder::translate_call(const CallFacts& inst, TranslationContext context) {
    if (inst.op_code == OpCode::NewObj) return translate_new_obj(inst, context);
    return wrap_in_ref(CallBuilder(*this, *type_system_, *settings).build(inst), inst.method->return_type());
}

TranslatedExpression ExpressionBuilder::wrap_in_ref(TranslatedExpression expr, const IType* type) {
    if (type->kind() == TypeKind::ByReference) {
        return without_il_instruction(make<DirectionExpression>(FieldDirection::Ref, expr.expression))
            .with_rr(make<ByReferenceResolveResult>(expr.resolve_result, ReferenceKind::Ref));
    }
    return expr;
}

bool ExpressionBuilder::is_current_or_containing_type(const ITypeDefinition* type) {
    const ITypeDefinition* current_type_definition = decompilation_context->current_type_definition();
    while (current_type_definition != nullptr) {
        if (type == current_type_definition) return true;
        current_type_definition = current_type_definition->declaring_type_definition();
    }
    return false;
}

bool ExpressionBuilder::is_base_type_of_current_type(const ITypeDefinition* type) {
    for (const ITypeDefinition* t : get_all_base_type_definitions(decompilation_context->current_type_definition())) {
        if (t == type) return true;
    }
    return false;
}

ExpressionWithResolveResult ExpressionBuilder::translate_function(const IType* delegate_type, ILFunction* function) {
    const IMethod* method = function->method != nullptr ? dynamic_cast<const IMethod*>(function->method->member_definition()) : nullptr;

    // Create AnonymousMethodExpression and prepare parameters
    auto* ame = make<AnonymousMethodExpression>();
    ame->set_is_async(function->is_async());
    for (ParameterDeclaration* pd : make_parameters(function->parameters, function)) ame->parameters().add(pd);
    StatementBuilder builder(*type_system_, *decompilation_context, function, *settings, statement_builder->decompile_run, cancellation_token_, l2);
    BlockStatement* body = builder.convert_as_block(function->body());

    Statement* warning_anchor = body->statements().first_or_null();
    for (const std::string& warning : function->warnings) {
        auto* warning_statement = make<EmptyStatement>();
        warning_statement->add_trailing_trivia(make<Comment>(warning));
        if (warning_anchor != nullptr)
            body->statements().insert_before(warning_anchor, warning_statement);
        else
            body->statements().add(warning_statement);
    }
    std::vector<AttributeSection*> attribute_sections;
    if (method != nullptr) {
        for (const IAttribute* attr : method->get_attributes()) {
            if (is_known_type(attr->attribute_type(), KnownAttribute::CompilerGenerated)) continue;
            if (function->is_async()) {
                if (is_known_type(attr->attribute_type(), KnownAttribute::AsyncStateMachine)) continue;
                if (is_known_type(attr->attribute_type(), KnownAttribute::DebuggerStepThrough)) continue;
            }
            attribute_sections.push_back(make<AttributeSection>(ast_builder.convert_attribute(attr)));
        }
        for (const IAttribute* attr : method->get_return_type_attributes()) {
            auto* section = make<AttributeSection>(ast_builder.convert_attribute(attr));
            section->set_attribute_target("return");
            attribute_sections.push_back(section);
        }
    }

    bool is_lambda = false;
    auto any_parameter = [&](auto predicate) {
        for (ParameterDeclaration* p : ame->parameters()) {
            if (predicate(p)) return true;
        }
        return false;
    };
    if (any_parameter([](ParameterDeclaration* p) { return p->type() == nullptr; })) {
        // if there is an anonymous type involved, we are forced to use a lambda expression.
        is_lambda = true;
    } else if (!attribute_sections.empty() || any_parameter([](ParameterDeclaration* p) { return p->attributes().count() > 0; })) {
        // C# 10 lambdas can have attributes, but anonymous methods cannot
        is_lambda = true;
    } else if (settings->use_lambda_syntax &&
               !any_parameter([](ParameterDeclaration* p) { return !(p->parameter_modifier() == ReferenceKind::None && !p->is_params()); })) {
        // otherwise use lambda only if an expression lambda is possible
        is_lambda = (body->statements().count() == 1 && is<ReturnStatement>(body->statements()[0]));
    }
    // Remove the parameter list from an AnonymousMethodExpression if the parameters are not used in the method body
    bool any_parameter_referencing_identifier = false;
    for (AstNode* node : body->descendants()) {
        auto* ident = as<IdentifierExpression>(node);
        if (ident == nullptr) continue;
        ILVariable* v = get_il_variable(ident);
        if (v != nullptr && v->function() == function && v->kind() == VariableKind::Parameter) {
            any_parameter_referencing_identifier = true;
            break;
        }
    }
    if (!is_lambda && !any_parameter_referencing_identifier) {
        ame->parameters().clear();
    }

    Expression* replacement;
    const IType* inferred_return_type;
    if (is_lambda) {
        auto* lambda = make<LambdaExpression>();
        lambda->attributes().add_range(attribute_sections);
        lambda->set_is_async(ame->is_async());
        copy_annotations_from(lambda, ame);
        ame->parameters().move_to(lambda->parameters());
        ReturnStatement* return_stmt = body->statements().count() == 1 ? as<ReturnStatement>(body->statements()[0]) : nullptr;
        if (return_stmt != nullptr && return_stmt->expression() != nullptr) {
            lambda->set_body(detach(return_stmt->expression()));
            inferred_return_type = get_resolve_result(lambda->body())->type();
        } else {
            lambda->set_body(body);
            inferred_return_type = infer_return_type(body);
        }
        replacement = lambda;
    } else {
        ame->set_body(body);
        inferred_return_type = infer_return_type(body);
        replacement = ame;
    }
    if (ame->is_async()) {
        inferred_return_type = get_task_type(inferred_return_type);
    }

    bool any_implicitly_typed = any_parameter([](ParameterDeclaration* p) { return p->type() == nullptr; });
    auto* rr = make<DecompiledLambdaResolveResult>(function, delegate_type, inferred_return_type, is_lambda || ame->parameters().count() > 0, !is_lambda,
                                                   any_implicitly_typed);

    TranslatedExpression translated_lambda = with_il_instruction(replacement, function).with_rr(rr);
    return with_rr(make<CastExpression>(convert_type(delegate_type), translated_lambda.expression),
                   make<ConversionResolveResult>(delegate_type, rr, LambdaConversion::instance()));
}

TranslatedExpression ExpressionBuilder::visit_il_function(ILFunction& function, TranslationContext) {
    return translate_function(function.delegate_type, &function).with_il_instruction(&function);
}

const IType* ExpressionBuilder::infer_return_type(BlockStatement* body) {
    std::vector<ResolveResult*> return_expressions;
    std::function<void(AstNode*)> collect_return_expressions = [&](AstNode* node) {
        if (auto* ret = as<ReturnStatement>(node)) {
            if (ret->expression() != nullptr) {
                return_expressions.push_back(get_resolve_result(ret->expression()));
            }
        } else if (is<LambdaExpression>(node) || is<AnonymousMethodExpression>(node)) {
            // do not recurse into nested lambdas
            return;
        }
        for (AstNode* child : node->children().to_vector()) {
            collect_return_expressions(child);
        }
    };
    collect_return_expressions(body);
    TypeInference ti(*compilation, resolver->conversions());
    bool success;
    return ti.get_best_common_type(return_expressions, &success);
    // Failure to infer a return type does not make the lambda invalid,
    // so we can ignore the 'success' value
}

const IType* ExpressionBuilder::get_task_type(const IType* result_type) {
    if (result_type->kind() == TypeKind::Unknown) return &SpecialType::unknown_type();
    if (result_type->kind() == TypeKind::Void) return known(KnownTypeCode::Task);

    const ITypeDefinition* def = known(KnownTypeCode::TaskOfT)->get_definition();
    if (def != nullptr) return make_parameterized_type(def, {result_type});
    return &SpecialType::unknown_type();
}

std::vector<ParameterDeclaration*> ExpressionBuilder::make_parameters(const std::vector<const IParameter*>& parameters, ILFunction* function) {
    std::vector<ParameterDeclaration*> result;
    std::map<int, ILVariable*> variables;
    for (ILVariable* v : function->variables()) {
        if (v->kind() == VariableKind::Parameter) variables[*v->index()] = v;
    }
    int i = 0;
    for (const IParameter* parameter : parameters) {
        ParameterDeclaration* pd = ast_builder.convert_parameter(parameter);
        auto it = variables.find(i);
        if (it != variables.end()) {
            ILVariable* v = it->second;
            pd->add_annotation(make<ILVariableResolveResult>(v, parameters[static_cast<std::size_t>(i)]->type()));
            pd->set_name(*v->name);
        }
        if (pd->name().empty() && !is_arg_list(pd->type())) {
            // needs to be consistent with logic in ILReader.CreateILVariable
            pd->set_name("P_" + std::to_string(i));
        }
        if (settings->anonymous_types && contains_anonymous_type(parameter->type())) pd->set_type(nullptr);
        result.push_back(pd);
        i++;
    }
    return result;
}

TranslatedExpression ExpressionBuilder::visit_block_container(BlockContainer& container, TranslationContext context) {
    BlockContainer* old_return_container = statement_builder->current_return_container;
    const IType* old_result_type = statement_builder->current_result_type;
    bool old_is_iterator = statement_builder->current_is_iterator;

    statement_builder->current_return_container = &container;
    statement_builder->current_result_type = context.type_hint;
    statement_builder->current_is_iterator = false;
    struct Restore {
        StatementBuilder* builder;
        BlockContainer* return_container;
        const IType* result_type;
        bool is_iterator;
        ~Restore() {
            builder->current_return_container = return_container;
            builder->current_result_type = result_type;
            builder->current_is_iterator = is_iterator;
        }
    } restore{statement_builder, old_return_container, old_result_type, old_is_iterator};

    BlockStatement* body = statement_builder->convert_as_block(&container);
    auto* comment_statement = make<EmptyStatement>();
    comment_statement->add_trailing_trivia(make<Comment>(" Could not convert BlockContainer to single expression"));
    body->statements().insert_before(body->statements().first_or_null(), comment_statement);
    // set ILVariable.UsesInitialValue for any variables being used inside the container
    for (ILInstruction* descendant : container.descendants()) {
        if (auto* stloc = il::as<StLoc>(descendant)) stloc->variable()->set_uses_initial_value(true);
    }
    auto* ame = make<AnonymousMethodExpression>();
    ame->set_body(body);
    const IType* system_func_type = typesystem::find_type(*compilation, FullTypeName(TopLevelTypeName("System", "Func", 1)));
    const IType* block_return_type = infer_return_type(body);
    const IType* delegate_type = make_parameterized_type(system_func_type, {block_return_type});
    Expression* invocation_target = make<CastExpression>(convert_type(delegate_type), ame);
    ResolveResult* rr;
    // This might happen when trying to decompile an assembly built for a target framework
    // where System.Func<T> does not exist yet.
    if (system_func_type->kind() == TypeKind::Unknown) {
        rr = make<ResolveResult>(block_return_type);
    } else {
        const IMethod* invoke_method = get_delegate_invoke_method(delegate_type);
        rr = make<CSharpInvocationResolveResult>(make<ResolveResult>(delegate_type), invoke_method, std::vector<ResolveResult*>{});
    }
    return with_rr(with_il_instruction(make<InvocationExpression>(make<MemberReferenceExpression>(invocation_target, "Invoke")), &container), rr);
}

TranslatedExpression ExpressionBuilder::translate_target(ILInstruction* target, bool non_virtual_invocation, bool member_static,
                                                          const IType* member_declaring_type, const IType* constrained_to) {
    auto match_ld_this = [&](ILInstruction* inst) {
        // ldloc this
        if (inst->match_ld_this()) return true;
        if (resolver->current_type_definition()->kind() == TypeKind::Struct) {
            // box T(ldobj T(ldloc this))
            ILInstruction* arg = nullptr;
            const IType* type = nullptr;
            if (!inst->match_box(&arg, &type)) return false;
            ILInstruction* arg2 = nullptr;
            const IType* type2 = nullptr;
            if (!arg->match_ld_obj(&arg2, &type2)) return false;
            if (!type->equals(type2) || !type->equals(resolver->current_type_definition())) return false;
            return arg2->match_ld_this();
        }
        return false;
    };
    auto should_use_base_reference = [&]() {
        if (!non_virtual_invocation) return false;
        if (!match_ld_this(target)) return false;
        if ((constrained_to != nullptr ? constrained_to : member_declaring_type)->get_definition() == resolver->current_type_definition()) return false;
        return true;
    };

    // If references are missing member.IsStatic might not be set correctly.
    // Additionally check target for null, in order to avoid a crash.
    if (!member_static && target != nullptr) {
        if (should_use_base_reference()) {
            const IType* base_reference_type = nullptr;
            for (const IType* t : resolver->current_type_definition()->direct_base_types()) {
                if (t->kind() != TypeKind::Interface) {
                    base_reference_type = t;
                    break;
                }
            }
            return with_il_instruction(make<BaseReferenceExpression>(), target)
                .with_rr(make<ThisResolveResult>(base_reference_type != nullptr ? base_reference_type : member_declaring_type, non_virtual_invocation));
        }
        const IType* target_type_hint = constrained_to != nullptr ? constrained_to : member_declaring_type;
        if (CallInstruction::expected_type_for_this_pointer(*member_declaring_type, constrained_to) == StackType::Ref) {
            if (target->result_type() == StackType::Ref) {
                target_type_hint = make_by_reference_type(target_type_hint);
            } else {
                target_type_hint = make_pointer_type(target_type_hint);
            }
        }
        TranslatedExpression translated_target = translate(target, target_type_hint);
        if (CallInstruction::expected_type_for_this_pointer(*member_declaring_type, constrained_to) == StackType::Ref) {
            // When accessing members on value types, ensure we use a reference of the correct type,
            // and not a pointer or a reference to a different type (issue #1333)
            auto* brt = dynamic_cast<const ByReferenceType*>(translated_target.type());
            if (!(brt != nullptr && NormalizeTypeVisitor::type_erasure().equivalent_types(
                                        brt->element_type(), constrained_to != nullptr ? constrained_to : member_declaring_type))) {
                translated_target = translated_target.convert_to(
                    make_by_reference_type(constrained_to != nullptr ? constrained_to : member_declaring_type), *this);
            }
        }
        if (is<DirectionExpression>(translated_target.expression)) {
            // (ref x).member => x.member
            translated_target = translated_target.unwrap_child(static_cast<DirectionExpression*>(translated_target.expression)->expression());
        } else if (auto* uoe = as<UnaryOperatorExpression>(translated_target.expression);
                   uoe != nullptr && uoe->operator_() == UnaryOperatorType::NullConditional && is<DirectionExpression>(uoe->expression())) {
            // (ref x)?.member => x?.member
            translated_target = translated_target.unwrap_child(static_cast<DirectionExpression*>(uoe->expression())->expression());
            // note: we need to create a new ResolveResult for the null-conditional operator,
            // using the underlying type of the input expression without the DirectionExpression
            translated_target = with_rr(make<UnaryOperatorExpression>(UnaryOperatorType::NullConditional, translated_target.expression),
                                        make<ResolveResult>(nullable_type::get_underlying_type(translated_target.type())))
                                    .without_il_instruction();
        }
        translated_target = ensure_target_not_nullable(translated_target, target);
        return translated_target;
    }
    const IType* type = constrained_to != nullptr ? constrained_to : member_declaring_type;
    return with_rr(make<TypeReferenceExpression>(convert_type(type)), make<TypeResolveResult>(type)).without_il_instruction();
}

TranslatedExpression ExpressionBuilder::ensure_target_not_nullable(TranslatedExpression expr, ILInstruction*) {
    // TODO Improve nullability support so that we do not sprinkle ! operators everywhere.
    // `inst` is the instruction that got translated into `expr`; a nullable `expr` would get a
    // SuppressNullableWarning operator here (not after a null-conditional, due to dotnet/roslyn#43659).
    return expr;
}

TranslatedExpression ExpressionBuilder::visit_ld_obj(LdObj& inst, TranslationContext context) { return translate_ld_obj(LdObjFacts::of(inst), context); }

TranslatedExpression ExpressionBuilder::translate_ld_obj(const LdObjFacts& inst, TranslationContext context) {
    const IType* load_type = inst.type();
    bool load_type_used_in_generic = inst.unaligned_prefix() != 0 || inst.target()->result_type() == StackType::Ref;
    if (context.type_hint->kind() != TypeKind::Unknown && is_compatible_type_for_memory_access(*context.type_hint, *load_type) &&
        !(load_type_used_in_generic && is_any_pointer(context.type_hint->kind()))) {
        load_type = context.type_hint;
    }
    if (inst.unaligned_prefix() != 0) {
        // Use one of: Unsafe.ReadUnaligned<T>(void*)
        //         or: Unsafe.ReadUnaligned<T>(ref byte)
        TranslatedExpression pointer = translate(inst.target());
        if (is<DirectionExpression>(pointer.expression)) {
            pointer = pointer.convert_to(make_by_reference_type(known(KnownTypeCode::Byte)), *this);
        } else {
            pointer = pointer.convert_to(make_pointer_type(known(KnownTypeCode::Void)), *this, false, true);
        }
        return call_unsafe_intrinsic("ReadUnaligned", {pointer.expression}, load_type, inst.self, {load_type});
    }
    ExpressionWithResolveResult result = ld_obj(inst.target(), load_type);
    // if (target.Type.IsSmallIntegerType() && load_type.IsSmallIntegerType() && target.Type.GetSign() != load_type.GetSign())
    //     return result.ConvertTo(load_type, this);
    return result.with_il_instruction(inst.self);
}

ExpressionWithResolveResult ExpressionBuilder::ld_obj(ILInstruction* address, const IType* load_type) {
    const IType* address_type_hint = address->result_type() == StackType::Ref ? make_by_reference_type(load_type) : make_pointer_type(load_type);
    TranslatedExpression target = translate(address, address_type_hint);
    if (is_compatible_pointer_type_for_memory_access(*target.type(), *load_type)) {
        ExpressionWithResolveResult result;
        if (auto* dir_expr = as<DirectionExpression>(target.expression)) {
            // we can dereference the managed reference by stripping away the 'ref'
            result = target.unwrap_child(dir_expr->expression());
        } else if (auto* pointer_type = dynamic_cast<const PointerType*>(target.type())) {
            auto* uoe = as<UnaryOperatorExpression>(target.expression);
            if (uoe != nullptr && uoe->operator_() == UnaryOperatorType::AddressOf) {
                // We can dereference the pointer by stripping away the '&'
                result = target.unwrap_child(uoe->expression());
            } else {
                // Dereference the existing pointer
                result = with_rr(make<UnaryOperatorExpression>(UnaryOperatorType::Dereference, target.expression),
                                 make<ResolveResult>(pointer_type->element_type()));
            }
        } else {
            // reference type behind non-DirectionExpression?
            // this case should be impossible, but we can use a pointer cast
            // just to make sure
            target = target.convert_to(make_pointer_type(load_type), *this);
            return with_rr(make<UnaryOperatorExpression>(UnaryOperatorType::Dereference, target.expression), make<ResolveResult>(load_type));
        }
        // we don't convert result to inst.Type, because the LdObj type
        // might be inaccurate (it's often System.Object for all reference types),
        // and our parent node should already insert casts where necessary
        return result;
    }
    // We need to cast the pointer type:
    if (is<DirectionExpression>(target.expression)) {
        target = target.convert_to(make_by_reference_type(load_type), *this);
    } else if (!is_unmanaged_type(load_type, settings->introduce_unmanaged_constraint)) {
        // Use: Unsafe.Read<T>(void*)
        target = target.convert_to(make_pointer_type(known(KnownTypeCode::Void)), *this, false, true);
        return call_unsafe_intrinsic("Read", {target.expression}, load_type, nullptr, {load_type});
    } else {
        target = target.convert_to(make_pointer_type(load_type), *this);
    }
    if (auto* dir_expr = as<DirectionExpression>(target.expression)) {
        return target.unwrap_child(dir_expr->expression());
    }
    return with_rr(make<UnaryOperatorExpression>(UnaryOperatorType::Dereference, target.expression), make<ResolveResult>(load_type));
}

TranslatedExpression ExpressionBuilder::visit_st_obj(StObj& inst, TranslationContext) { return translate_st_obj(StObjFacts::of(inst)); }

TranslatedExpression ExpressionBuilder::translate_st_obj(const StObjFacts& inst) {
    if (inst.unaligned_prefix() != 0 || (inst.target()->result_type() != StackType::Ref && !is_unmanaged_type(inst.type(), settings->introduce_unmanaged_constraint))) {
        return st_obj_via_helper_call(inst);
    }

    const IType* pointer_type_hint = inst.target()->result_type() == StackType::Ref ? make_by_reference_type(inst.type()) : make_pointer_type(inst.type());
    TranslatedExpression pointer = translate(inst.target(), pointer_type_hint);
    TranslatedExpression target;
    TranslatedExpression value;
    const IType* memory_type;
    // Check if we need to cast to pointer type:
    if (is_compatible_pointer_type_for_memory_access(*pointer.type(), *inst.type())) {
        // cast not necessary, we can use the existing type
        memory_type = dynamic_cast<const TypeWithElementType*>(pointer.type())->element_type();
    } else {
        // We need to introduce a pointer cast
        value = translate(inst.value(), inst.type());
        if (is_compatible_type_for_memory_access(*value.type(), *inst.type())) {
            memory_type = value.type();
        } else {
            memory_type = inst.type();
        }
        if (is<DirectionExpression>(pointer.expression)) {
            pointer = pointer.convert_to(make_by_reference_type(memory_type), *this);
        } else {
            pointer = pointer.convert_to(make_pointer_type(memory_type), *this);
        }
    }

    if (is<DirectionExpression>(pointer.expression)) {
        // we can deference the managed reference by stripping away the 'ref'
        target = pointer.unwrap_child(static_cast<DirectionExpression*>(pointer.expression)->expression());
    } else {
        auto* uoe = as<UnaryOperatorExpression>(pointer.expression);
        if (uoe != nullptr && uoe->operator_() == UnaryOperatorType::AddressOf) {
            // *&ptr -> ptr
            target = pointer.unwrap_child(uoe->expression());
        } else {
            target = without_il_instruction(make<UnaryOperatorExpression>(UnaryOperatorType::Dereference, pointer.expression))
                         .with_rr(make<ResolveResult>(memory_type));
        }
    }
    if (value.expression == nullptr) {
        value = translate(inst.value(), target.type());
    }
    auto* dir_expr = as<DirectionExpression>(target.expression);
    auto* lhs_ref_rr = dynamic_cast<ByReferenceResolveResult*>(target.resolve_result);
    if (dir_expr != nullptr && lhs_ref_rr != nullptr) {
        // ref (re-)assignment, emit "ref (a = ref b)".
        target = target.unwrap_child(dir_expr->expression());
        value = value.convert_to(lhs_ref_rr->type(), *this, false, true);
        ExpressionWithResolveResult assign =
            with_rr(make<AssignmentExpression>(target.expression, value.expression),
                    make<OperatorResolveResult>(target.type(), ExpressionType::Assign, std::vector<ResolveResult*>{lhs_ref_rr, value.resolve_result}));
        return without_il_instruction(make<DirectionExpression>(FieldDirection::Ref, assign.expression)).with_rr(lhs_ref_rr);
    }
    return assignment(target, value).with_il_instruction(inst.self);
}

TranslatedExpression ExpressionBuilder::st_obj_via_helper_call(const StObjFacts& inst) {
    // "unaligned.1; stobj" -> decompile to a call of
    //    Unsafe.WriteUnaligned<T>(void*, T)
    // or Unsafe.WriteUnaligned<T>(ref byte, T)
    // "stobj ManagedType" -> decompile to a call of
    //    Unsafe.Write<T>(void*, T)
    TranslatedExpression pointer = translate(inst.target());
    TranslatedExpression value = translate(inst.value(), inst.type());
    if (is<DirectionExpression>(pointer.expression) && inst.unaligned_prefix() != 0) {
        pointer = pointer.convert_to(make_by_reference_type(known(KnownTypeCode::Byte)), *this);
    } else {
        pointer = pointer.convert_to(make_pointer_type(known(KnownTypeCode::Void)), *this, false, true);
    }
    if (!is_compatible_type_for_memory_access(*value.type(), *inst.type())) {
        value = value.convert_to(inst.type(), *this);
    }
    if (inst.unaligned_prefix() != 0) {
        return call_unsafe_intrinsic("WriteUnaligned", {pointer.expression, value.expression}, known(KnownTypeCode::Void), inst.self);
    }
    return call_unsafe_intrinsic("Write", {pointer.expression, value.expression}, inst.type(), inst.self);
}

TranslatedExpression ExpressionBuilder::visit_ld_len(LdLen& inst, TranslationContext) {
    const IType* array_type = known(KnownTypeCode::Array);
    TranslatedExpression array_expr = translate(inst.array(), array_type);
    if (array_expr.type()->kind() != TypeKind::Array) {
        array_expr = array_expr.convert_to(array_type, *this);
    }
    array_expr = ensure_target_not_nullable(array_expr, inst.array());
    std::string member_name;
    KnownTypeCode code;
    if (inst.result_type() == StackType::I4) {
        member_name = "Length";
        code = KnownTypeCode::Int32;
    } else {
        member_name = "LongLength";
        code = KnownTypeCode::Int64;
    }
    std::vector<const IProperty*> properties = array_type->get_properties([&](const IProperty* p) { return p->name() == member_name; });
    const IProperty* member = properties.empty() ? nullptr : properties.front();
    ResolveResult* rr = member == nullptr ? static_cast<ResolveResult*>(make<ResolveResult>(known(code)))
                                          : static_cast<ResolveResult*>(make<MemberResolveResult>(array_expr.resolve_result, member));
    return with_rr(with_il_instruction(make<MemberReferenceExpression>(array_expr.expression, member_name), &inst), rr);
}

TranslatedExpression ExpressionBuilder::visit_ld_flda(LdFlda& inst, TranslationContext) { return translate_ld_flda(LdFldaFacts::of(inst)); }

TranslatedExpression ExpressionBuilder::translate_ld_flda(const LdFldaFacts& inst) {
    auto* nested_ld_flda = il::as<LdFlda>(inst.target());
    const IType* element_type = nullptr;
    if (settings->fixed_buffers && inst.field()->name() == "FixedElementField" && nested_ld_flda != nullptr &&
        is_fixed_field(nested_ld_flda->field(), &element_type, nullptr)) {
        Expression* field_access = convert_field(nested_ld_flda->field(), nested_ld_flda->target()).expression;
        auto* mrr = static_cast<MemberResolveResult*>(get_resolve_result(field_access));
        field_access->remove_annotations<ResolveResult>();
        TranslatedExpression result = with_rr(field_access, make<MemberResolveResult>(mrr->target_result(), mrr->member(), make_pointer_type(element_type)))
                                          .with_il_instruction(inst.self);
        if (inst.result_type() == StackType::Ref) {
            // `target.field` has pointer-type.
            auto* parent_conv = il::as<Conv>(inst.parent());
            if (inst.slot_info() == &PinnedRegion::init_slot || (parent_conv != nullptr && parent_conv->target_type == il::PrimitiveType::U)) {
                // Convert pointer to ref if we're in a context where we're going to convert
                // the ref back to a pointer.
                return result.convert_to(make_by_reference_type(element_type), *this);
            }
            // We can't use `ref *target.field` unless `target` is non-movable,
            // but we can use `ref target.field[0]`.
            ExpressionWithResolveResult array_access =
                with_rr(make<IndexerExpression>(result.expression, std::vector<Expression*>{make<PrimitiveExpression>(ConstantValue(std::int32_t{0}))}),
                        make<ResolveResult>(element_type));
            return without_il_instruction(make<DirectionExpression>(FieldDirection::Ref, array_access.expression))
                .with_rr(make<ByReferenceResolveResult>(array_access.resolve_result, ReferenceKind::Ref));
        }
        return result;
    }
    TranslatedExpression expr;
    const IType* underlying_tuple_type = nullptr;
    ILInstruction* target = nullptr;
    int position = 0;
    if (il::transforms::TupleTransform::match_tuple_field_access(static_cast<LdFlda*>(inst.self), &underlying_tuple_type, &target, &position)) {
        TranslatedExpression translated_target = translate_target(target, true, false, underlying_tuple_type);
        auto* tuple_type = dynamic_cast<const TupleType*>(translated_target.type());
        if (tuple_type != nullptr && NormalizeTypeVisitor::type_erasure().equivalent_types(tuple_type, underlying_tuple_type) &&
            position <= static_cast<int>(tuple_type->element_names().size())) {
            std::optional<std::string> element_name = tuple_type->element_names()[static_cast<std::size_t>(position - 1)];
            std::string name = element_name.has_value() ? *element_name : "Item" + std::to_string(position);
            // tuple_type.ElementTypes are more accurate w.r.t. nullability/dynamic than inst.Field.Type
            auto* rr = make<MemberResolveResult>(translated_target.resolve_result, inst.field(), tuple_type->element_types()[static_cast<std::size_t>(position - 1)]);
            expr = with_rr(make<MemberReferenceExpression>(translated_target.expression, name), rr).with_il_instruction(inst.self);
        } else {
            expr = convert_field(inst.field(), inst.target()).with_il_instruction(inst.self);
        }
    } else {
        expr = convert_field(inst.field(), inst.target()).with_il_instruction(inst.self);
    }
    if (inst.result_type() == StackType::I) {
        // ldflda producing native pointer
        return without_il_instruction(make<UnaryOperatorExpression>(UnaryOperatorType::AddressOf, expr.expression))
            .with_rr(make<ResolveResult>(make_pointer_type(expr.type())));
    }
    // ldflda producing managed pointer
    return without_il_instruction(make<DirectionExpression>(FieldDirection::Ref, expr.expression))
        .with_rr(make<ByReferenceResolveResult>(expr.resolve_result, ReferenceKind::Ref));
}

TranslatedExpression ExpressionBuilder::visit_lds_flda(LdsFlda& inst, TranslationContext) { return translate_lds_flda(LdsFldaFacts::of(inst)); }

TranslatedExpression ExpressionBuilder::translate_lds_flda(const LdsFldaFacts& inst) {
    TranslatedExpression expr = convert_field(inst.field()).with_il_instruction(inst.self);
    return without_il_instruction(make<DirectionExpression>(FieldDirection::Ref, expr.expression))
        .with_rr(make<ByReferenceResolveResult>(expr.resolve_result, ReferenceKind::Ref));
}

TranslatedExpression ExpressionBuilder::visit_ld_elema(LdElema& inst, TranslationContext) { return translate_ld_elema(LdElemaFacts::of(inst)); }

TranslatedExpression ExpressionBuilder::translate_ld_elema(const LdElemaFacts& inst) {
    TranslatedExpression array_expr = translate(inst.array());
    auto* array_type = dynamic_cast<const ArrayType*>(array_expr.type());
    if (array_type == nullptr || !is_compatible_type_for_memory_access(*array_type->element_type(), *inst.type())) {
        array_type = dynamic_cast<const ArrayType*>(make_array_type(*compilation, inst.type(), static_cast<int>(inst.indices().size())));
        array_expr = array_expr.convert_to(array_type, *this);
    }
    std::vector<Expression*> index_expressions;
    if (inst.with_system_index) {
        const IType* system_index = known(KnownTypeCode::Index);
        for (ILInstruction* i : inst.indices()) {
            index_expressions.push_back(translate(i, system_index).convert_to(system_index, *this).expression);
        }
    } else {
        for (ILInstruction* i : inst.indices()) {
            index_expressions.push_back(translate_array_index(i).expression);
        }
    }
    auto* indexer_expr = make<IndexerExpression>(array_expr.expression, index_expressions);
    TranslatedExpression expr = with_il_instruction(indexer_expr, inst.self).with_rr(make<ResolveResult>(array_type->element_type()));
    return without_il_instruction(make<DirectionExpression>(FieldDirection::Ref, expr.expression))
        .with_rr(make<ByReferenceResolveResult>(expr.resolve_result, ReferenceKind::Ref));
}

TranslatedExpression ExpressionBuilder::visit_ld_elema_inline_array(LdElemaInlineArray& inst, TranslationContext) { return translate_ld_elema_inline_array(LdElemaInlineArrayFacts::of(inst)); }

TranslatedExpression ExpressionBuilder::translate_ld_elema_inline_array(const LdElemaInlineArrayFacts& inst) {
    TranslatedExpression array_expr = translate_target(inst.array(), true, false, inst.type());
    const IType* inline_array_element_type = typesystem::get_inline_array_element_type(inst.type());
    std::vector<Expression*> index_expressions;
    for (ILInstruction* i : inst.indices()) index_expressions.push_back(translate_array_index(i).expression);
    auto* indexer_expr = make<IndexerExpression>(array_expr.expression, index_expressions);
    TranslatedExpression expr = with_il_instruction(indexer_expr, inst.self).with_rr(make<ResolveResult>(inline_array_element_type));
    return without_il_instruction(make<DirectionExpression>(FieldDirection::Ref, expr.expression))
        .with_rr(make<ByReferenceResolveResult>(expr.resolve_result, ReferenceKind::Ref));
}

TranslatedExpression ExpressionBuilder::translate_array_index(ILInstruction* i) {
    TranslatedExpression input = translate(i);
    return convert_array_index(input, i->result_type(), false);
}

TranslatedExpression ExpressionBuilder::convert_array_index(TranslatedExpression input, StackType stack_type, bool allow_int_ptr) {
    if (get_size(*input.type()) > get_size(stack_type)) {
        // truncate oversized result
        return input.convert_to(find_type(stack_type, get_sign(*input.type())), *this);
    }
    if (is_csharp_primitive_integer_type(*input.type()) || is_csharp_native_integer_type(*input.type())) {
        // can be used as array index as-is
        return input;
    }
    if (allow_int_ptr && (is_known_type(input.type(), KnownTypeCode::IntPtr) || is_known_type(input.type(), KnownTypeCode::UIntPtr))) {
        return input;
    }
    if (stack_type != StackType::I4 && get_stack_type(*input.type()) == StackType::I4) {
        // prefer casting to int if that's big enough
        stack_type = StackType::I4;
    }
    const IType* target_type = find_arithmetic_type(stack_type, get_sign(*input.type()));
    return input.convert_to(target_type, *this);
}

bool ExpressionBuilder::is_unbox_any_with_is_inst(const TypedUnaryFacts& unbox_any, const IType* is_inst_type) {
    return unbox_any.type()->equals(is_inst_type) &&
           (is_known_type(unbox_any.type(), KnownTypeCode::NullableOfT) || is_inst_type->is_reference_type() == std::optional<bool>(true));
}

TranslatedExpression ExpressionBuilder::visit_unbox_any(UnboxAny& inst, TranslationContext) { return translate_unbox_any(TypedUnaryFacts::of(inst)); }

TranslatedExpression ExpressionBuilder::translate_unbox_any(const TypedUnaryFacts& inst) {
    TranslatedExpression arg;
    auto* is_inst = il::as<IsInst>(inst.argument());
    if (is_inst != nullptr && is_unbox_any_with_is_inst(inst, is_inst->type())) {
        // unbox.any T(isinst T(expr)) ==> expr as T
        // This is used for generic types and nullable value types
        arg = unwrap_boxing_conversion(translate(is_inst->argument()));
        return with_rr(with_il_instruction(make<AsExpression>(arg.expression, convert_type(inst.type())), inst.self),
                       make<ConversionResolveResult>(inst.type(), arg.resolve_result, Conversion::try_cast()));
    }

    arg = translate(inst.argument());
    const IType* target_type = inst.type();
    if (target_type->kind() == TypeKind::TypeParameter) {
        ResolveResult* rr = resolver->resolve_cast(target_type, arg.resolve_result);
        if (rr->is_error()) {
            // C# 6.2.7 Explicit conversions involving type parameters:
            // if we can't directly convert to a type parameter,
            // try via its effective base class.
            arg = arg.convert_to(dynamic_cast<const ITypeParameter*>(target_type)->effective_base_class(), *this);
        }
    } else {
        // Before unboxing arg must be a object
        arg = arg.convert_to(known(KnownTypeCode::Object), *this);
    }

    return with_rr(with_il_instruction(make<CastExpression>(convert_type(target_type), arg.expression), inst.self),
                   make<ConversionResolveResult>(target_type, arg.resolve_result, Conversion::unboxing_conversion()));
}

TranslatedExpression ExpressionBuilder::visit_unbox(Unbox& inst, TranslationContext) { return translate_unbox(TypedUnaryFacts::of(inst)); }

TranslatedExpression ExpressionBuilder::translate_unbox(const TypedUnaryFacts& inst) {
    TranslatedExpression arg = translate(inst.argument());
    ExpressionWithResolveResult cast_expression = with_rr(make<CastExpression>(convert_type(inst.type()), arg.expression),
                                                          make<ConversionResolveResult>(inst.type(), arg.resolve_result, Conversion::unboxing_conversion()));
    return with_il_instruction(make<DirectionExpression>(FieldDirection::Ref, cast_expression.expression), inst.self)
        .with_rr(make<ByReferenceResolveResult>(cast_expression.resolve_result, ReferenceKind::Ref));
}

TranslatedExpression ExpressionBuilder::visit_box(Box& inst, TranslationContext) { return translate_box(TypedUnaryFacts::of(inst)); }

TranslatedExpression ExpressionBuilder::translate_box(const TypedUnaryFacts& inst) {
    const IType* target_type = inst.type();
    TranslatedExpression arg = translate(inst.argument(), target_type);
    if (settings->native_integers && !arg.type()->equals(target_type)) {
        if (is_known_type(target_type, KnownTypeCode::IntPtr)) {
            target_type = &SpecialType::nint();
        } else if (is_known_type(target_type, KnownTypeCode::UIntPtr)) {
            target_type = &SpecialType::nuint();
        }
    }
    arg = arg.convert_to(target_type, *this);
    const IType* obj = known(KnownTypeCode::Object);
    return with_rr(with_il_instruction(make<CastExpression>(convert_type(obj), arg.expression), inst.self),
                   make<ConversionResolveResult>(obj, arg.resolve_result, Conversion::boxing_conversion()));
}

TranslatedExpression ExpressionBuilder::visit_cast_class(CastClass& inst, TranslationContext) { return translate_cast_class(TypedUnaryFacts::of(inst)); }

TranslatedExpression ExpressionBuilder::translate_cast_class(const TypedUnaryFacts& inst) {
    return translate(inst.argument()).convert_to(inst.type(), *this);
}

TranslatedExpression ExpressionBuilder::visit_expression_tree_cast(ExpressionTreeCast& inst, TranslationContext) { return translate_expression_tree_cast(ExpressionTreeCastFacts::of(inst)); }

TranslatedExpression ExpressionBuilder::translate_expression_tree_cast(const ExpressionTreeCastFacts& inst) {
    return translate(inst.argument()).convert_to(inst.type(), *this, inst.is_checked());
}

TranslatedExpression ExpressionBuilder::visit_arglist(Arglist& inst, TranslationContext) {
    auto* expr = make<UndocumentedExpression>();
    expr->set_undocumented_expression_type(UndocumentedExpressionType::ArgListAccess);
    return with_rr(with_il_instruction(expr, &inst), make<TypeResolveResult>(typesystem::find_type(*compilation, TopLevelTypeName("System", "RuntimeArgumentHandle"))));
}

TranslatedExpression ExpressionBuilder::visit_make_ref_any(MakeRefAny& inst, TranslationContext) {
    Expression* arg = translate(inst.argument()).expression;
    if (auto* direction = as<DirectionExpression>(arg)) {
        arg = direction->expression();
    }
    auto* expr = make<UndocumentedExpression>();
    expr->set_undocumented_expression_type(UndocumentedExpressionType::MakeRef);
    expr->arguments().add(detach(arg));
    return with_rr(with_il_instruction(expr, &inst), make<TypeResolveResult>(typesystem::find_type(*compilation, TopLevelTypeName("System", "TypedReference"))));
}

TranslatedExpression ExpressionBuilder::visit_ref_any_type(RefAnyType& inst, TranslationContext) {
    auto* undocumented = make<UndocumentedExpression>();
    undocumented->set_undocumented_expression_type(UndocumentedExpressionType::RefType);
    undocumented->arguments().add(detach(translate(inst.argument()).expression));
    return with_rr(with_il_instruction(make<MemberReferenceExpression>(undocumented, "TypeHandle"), &inst),
                   make<TypeResolveResult>(typesystem::find_type(*compilation, TopLevelTypeName("System", "RuntimeTypeHandle"))));
}

TranslatedExpression ExpressionBuilder::visit_ref_any_value(RefAnyValue& inst, TranslationContext) {
    auto* undocumented = make<UndocumentedExpression>();
    undocumented->set_undocumented_expression_type(UndocumentedExpressionType::RefValue);
    undocumented->arguments().add(translate(inst.argument()).expression);
    undocumented->arguments().add(make<TypeReferenceExpression>(convert_type(inst.type())));
    ExpressionWithResolveResult expr = with_rr(undocumented, make<ResolveResult>(inst.type()));
    return without_il_instruction(make<DirectionExpression>(FieldDirection::Ref, expr.with_il_instruction(&inst).expression))
        .with_rr(make<ByReferenceResolveResult>(expr.resolve_result, ReferenceKind::Ref));
}

TranslatedExpression ExpressionBuilder::visit_block(Block& block, TranslationContext context) { return translate_block(block, block.kind, context); }

// (the initializers' translations read the block's instructions, as the builders' rewrites leave them)
TranslatedExpression ExpressionBuilder::translate_block(Block& block, BlockKind kind, TranslationContext context) {
    switch (kind) {
        case BlockKind::ArrayInitializer: return translate_array_initializer(&block);
        case BlockKind::StackAllocInitializer: return translate_stack_alloc_initializer(&block, context.type_hint);
        case BlockKind::CollectionInitializer:
        case BlockKind::ObjectInitializer: return translate_object_and_collection_initializer(&block);
        case BlockKind::WithInitializer: return translate_with_initializer(&block);
        case BlockKind::CallInlineAssign: return translate_setter_call_assignment(&block);
        case BlockKind::CallWithNamedArgs: return translate_call_with_named_args(&block);
        case BlockKind::InterpolatedString: return translate_interpolated_string(&block);
        default: return error_expression("Unknown block type: " + block_kind_text(block.kind));
    }
}

TranslatedExpression ExpressionBuilder::translate_interpolated_string(Block* block) {
    std::vector<InterpolatedStringContent*> content;

    for (int i = 1; i < block->instructions.count(); i++) {
        auto* call = static_cast<Call*>(block->instructions[i]);

        auto build_interpolation = [&](int alignment = 0, std::optional<std::string> suffix = std::nullopt) {
            return make<Interpolation>(translate(call->arguments()[1]).convert_to(call->get_parameter(1)->type(), *this, false, true).expression, alignment,
                                       std::move(suffix));
        };

        const std::string& name = call->method->name();
        int argument_count = call->arguments().count();
        if (name == "AppendLiteral") {
            std::string text = static_cast<LdStr*>(call->arguments()[1])->value;
            text = replace_all(replace_all(text, "{", "{{"), "}", "}}");
            content.push_back(make<InterpolatedStringText>(text));
        } else if (name == "AppendFormatted" && argument_count == 2) {
            content.push_back(build_interpolation());
        } else if (name == "AppendFormatted" && argument_count == 3 && il::as<LdStr>(call->arguments()[2]) != nullptr) {
            content.push_back(build_interpolation(0, il::as<LdStr>(call->arguments()[2])->value));
        } else if (name == "AppendFormatted" && argument_count == 3 && il::as<LdcI4>(call->arguments()[2]) != nullptr) {
            content.push_back(build_interpolation(il::as<LdcI4>(call->arguments()[2])->value));
        } else if (name == "AppendFormatted" && argument_count == 4 && il::as<LdcI4>(call->arguments()[2]) != nullptr &&
                   il::as<LdStr>(call->arguments()[3]) != nullptr) {
            content.push_back(build_interpolation(il::as<LdcI4>(call->arguments()[2])->value, il::as<LdStr>(call->arguments()[3])->value));
        } else {
            throw std::logic_error("not supported");
        }
    }

    return with_rr(with_il_instruction(make<InterpolatedStringExpression>(content), block), make<ResolveResult>(known(KnownTypeCode::String)));
}

TranslatedExpression ExpressionBuilder::translate_call_with_named_args(Block* block) {
    return wrap_in_ref(CallBuilder(*this, *type_system_, *settings).call_with_named_args(block),
                       static_cast<CallInstruction*>(block->final_instruction())->method->return_type());
}

TranslatedExpression ExpressionBuilder::translate_setter_call_assignment(Block* block) {
    CallInstruction* call = nullptr;
    ILInstruction* value = nullptr;
    if (!block->match_inline_assign_block(&call, &value)) {
        // should never happen unless the ILAst is invalid
        return error_expression("Error: MatchInlineAssignBlock() returned false");
    }
    std::vector<ILInstruction*> arguments(call->arguments().begin(), call->arguments().end());
    arguments[arguments.size() - 1] = value;
    return CallBuilder(*this, *type_system_, *settings).build(call->op_code, call->method, arguments).with_il_instruction(call);
}

TranslatedExpression ExpressionBuilder::translate_object_and_collection_initializer(Block* block) {
    auto* stloc = block->instructions.count() > 0 ? il::as<StLoc>(block->instructions[0]) : nullptr;
    auto* final_inst = il::as<LdLoc>(block->final_instruction());
    // Check basic structure of block
    if (stloc == nullptr || final_inst == nullptr || stloc->variable() != final_inst->variable() ||
        stloc->variable()->kind() != VariableKind::InitializerTarget)
        throw std::invalid_argument("given Block is invalid!");
    InitializedObjectResolveResult* init_obj_rr;
    TranslatedExpression expr;
    // Detect type of initializer
    ILInstruction* value = stloc->value();
    if (auto* new_obj_inst = il::as<NewObj>(value)) {
        init_obj_rr = make<InitializedObjectResolveResult>(new_obj_inst->method->declaring_type());
        expr = CallBuilder(*this, *type_system_, *settings).build(new_obj_inst);
    } else if (auto* default_val = il::as<DefaultValue>(value)) {
        init_obj_rr = make<InitializedObjectResolveResult>(default_val->type());
        expr = with_rr(with_il_instruction(make<ObjectCreateExpression>(convert_type(default_val->type())), default_val),
                       make<TypeResolveResult>(default_val->type()));
    } else if (auto* call_with_named_args = il::as<Block>(value); call_with_named_args != nullptr && call_with_named_args->kind == BlockKind::CallWithNamedArgs) {
        expr = translate_call_with_named_args(call_with_named_args);
        init_obj_rr = make<InitializedObjectResolveResult>(expr.type());
    } else if (auto* c = il::as<Call>(value);
               c != nullptr && full_name_is(c->method, "System.Activator", "CreateInstance") && c->method->type_arguments().size() == 1) {
        const IType* type = c->method->type_arguments()[0];
        init_obj_rr = make<InitializedObjectResolveResult>(type);
        expr = with_rr(with_il_instruction(make<ObjectCreateExpression>(convert_type(type)), c), make<TypeResolveResult>(type));
    } else {
        throw std::invalid_argument("given Block is invalid!");
    }
    auto* oce = static_cast<ObjectCreateExpression*>(expr.expression);
    oce->set_initializer(build_array_initializer_expression(block, init_obj_rr));
    return expr.with_il_instruction(block);
}

ArrayInitializerExpression* ExpressionBuilder::build_array_initializer_expression(Block* block, InitializedObjectResolveResult* init_obj_rr) {
    std::vector<std::vector<TranslatedExpression>> elements_stack;
    elements_stack.emplace_back();  // `elements`: the bottom of the stack; the vector is reserved below so the final read stays valid
    std::optional<std::vector<AccessPathElement>> current_path;
    std::map<ILVariable*, ILInstruction*> index_variables;
    for (int index = 1; index < block->instructions.count(); index++) {
        ILInstruction* inst = block->instructions[index];
        // Collect indexer variables (for C# 6 dictionary initializers)
        if (auto* index_store = il::as<StLoc>(inst)) {
            index_variables.emplace(index_store->variable(), index_store->value());
            continue;
        }
        // Get current path
        AccessPathElement::AccessPath info = AccessPathElement::get_access_path(inst, init_obj_rr->type(), settings);
        // This should not happen, because the IL transform should not create invalid access paths,
        // but we leave it here as sanity check.
        if (info.kind == AccessPathKind::Invalid) continue;
        // Calculate "difference" to previous path
        if (!current_path.has_value()) {
            current_path = info.path;
        } else {
            std::size_t min_len = std::min(current_path->size(), info.path.size());
            std::size_t first_difference_index = 0;
            while (first_difference_index < min_len && info.path[first_difference_index] == (*current_path)[first_difference_index]) first_difference_index++;
            while (elements_stack.size() - 1 > first_difference_index) {
                const AccessPathElement& method_element = (*current_path)[elements_stack.size() - 1];
                const AccessPathElement& path_element = (*current_path)[elements_stack.size() - 2];
                std::vector<TranslatedExpression> values = std::move(elements_stack.back());
                elements_stack.pop_back();
                elements_stack.back().push_back(make_initializer_assignment(init_obj_rr, method_element, path_element, values, index_variables));
            }
            current_path = info.path;
        }
        // Fill the stack with empty expression lists
        while (elements_stack.size() < current_path->size()) elements_stack.emplace_back();
        const AccessPathElement& last_element = current_path->back();
        auto* member_rr = make<MemberResolveResult>(init_obj_rr, last_element.member);
        switch (info.kind) {
            case AccessPathKind::Adder:
                assert(dynamic_cast<const IMethod*>(last_element.member) != nullptr);
                elements_stack.back().push_back(
                    CallBuilder(*this, *type_system_, *settings)
                        .build_collection_initializer_expression(last_element.op_code, dynamic_cast<const IMethod*>(last_element.member), init_obj_rr, *info.values)
                        .with_il_instruction(inst));
                break;
            case AccessPathKind::Setter:
                assert(dynamic_cast<const IProperty*>(last_element.member) != nullptr || dynamic_cast<const IField*>(last_element.member) != nullptr);
                if (last_element.indices.has_value() && !last_element.indices->empty()) {
                    auto* property = dynamic_cast<const IProperty*>(last_element.member);
                    assert(property->parameters().size() == last_element.indices->size());
                    assert(property->setter() != nullptr);
                    elements_stack.back().push_back(
                        CallBuilder(*this, *type_system_, *settings)
                            .build_dictionary_initializer_expression(last_element.op_code, property->setter(), init_obj_rr,
                                                                     get_indices(*last_element.indices, index_variables), info.values->front())
                            .with_il_instruction(inst));
                } else {
                    TranslatedExpression value =
                        translate(info.values->front(), member_rr->type()).convert_to(member_rr->type(), *this, false, true);
                    TranslatedExpression assignment_expr =
                        with_il_instruction(make<NamedExpression>(last_element.member->name(), value.expression), inst).with_rr(member_rr);
                    elements_stack.back().push_back(assignment_expr);
                }
                break;
            default: break;
        }
    }
    while (elements_stack.size() > 1) {
        const AccessPathElement& method_element = (*current_path)[elements_stack.size() - 1];
        const AccessPathElement& path_element = (*current_path)[elements_stack.size() - 2];
        std::vector<TranslatedExpression> values = std::move(elements_stack.back());
        elements_stack.pop_back();
        elements_stack.back().push_back(make_initializer_assignment(init_obj_rr, method_element, path_element, values, index_variables));
    }
    std::vector<Expression*> element_expressions;
    for (const TranslatedExpression& e : elements_stack.front()) element_expressions.push_back(e.expression);
    return make<ArrayInitializerExpression>(element_expressions);
}

std::vector<ILInstruction*> ExpressionBuilder::get_indices(const std::vector<ILInstruction*>& indices, const std::map<ILVariable*, ILInstruction*>& index_variables) {
    std::vector<ILInstruction*> result;
    for (ILInstruction* inst : indices) {
        auto* ld = il::as<LdLoc>(inst);
        if (ld != nullptr) {
            auto it = index_variables.find(ld->variable());
            if (it != index_variables.end()) {
                result.push_back(it->second);
                continue;
            }
        }
        result.push_back(inst);
    }
    return result;
}

TranslatedExpression ExpressionBuilder::make_initializer_assignment(InitializedObjectResolveResult* rr, const AccessPathElement& member_path,
                                                                     const AccessPathElement& value_path, const std::vector<TranslatedExpression>& values,
                                                                     const std::map<ILVariable*, ILInstruction*>& index_variables) {
    auto make_array_initializer = [&]() {
        std::vector<Expression*> expressions;
        for (const TranslatedExpression& v : values) expressions.push_back(v.expression);
        return with_rr(make<ArrayInitializerExpression>(expressions), make<ResolveResult>(&SpecialType::unknown_type())).without_il_instruction();
    };
    TranslatedExpression value;
    auto* method = dynamic_cast<const IMethod*>(member_path.member);
    if (method != nullptr && method->name() == "Add") {
        value = make_array_initializer();
    } else if (values.size() == 1 && !(is<AssignmentExpression>(values[0].expression) || is<NamedExpression>(values[0].expression))) {
        value = values[0];
    } else {
        value = make_array_initializer();
    }
    if (value_path.indices.has_value() && !value_path.indices->empty()) {
        std::vector<Expression*> index_expressions;
        for (ILInstruction* i : get_indices(*value_path.indices, index_variables)) index_expressions.push_back(translate(i).expression);
        Expression* index = make<IndexerExpression>(nullptr, index_expressions);
        return with_rr(make<AssignmentExpression>(index, value.expression), make<MemberResolveResult>(rr, value_path.member)).without_il_instruction();
    }
    return with_rr(make<NamedExpression>(value_path.member->name(), value.expression), make<MemberResolveResult>(rr, value_path.member)).without_il_instruction();
}

TranslatedExpression ExpressionBuilder::translate_array_initializer(Block* block) {
    auto* stloc = block->instructions.count() > 0 ? il::as<StLoc>(block->instructions[0]) : nullptr;
    auto* final_inst = il::as<LdLoc>(block->final_instruction());
    const IType* type = nullptr;
    if (stloc == nullptr || final_inst == nullptr || !stloc->value()->match_new_arr(&type)) throw std::invalid_argument("given Block is invalid!");
    if (stloc->variable() != final_inst->variable() || stloc->variable()->kind() != VariableKind::InitializerTarget)
        throw std::invalid_argument("given Block is invalid!");
    auto* new_arr = static_cast<NewArr*>(stloc->value());

    std::vector<TranslatedExpression> translated_dimensions;
    for (ILInstruction* i : new_arr->indices()) translated_dimensions.push_back(translate(i));

    for (const TranslatedExpression& dim : translated_dimensions) {
        if (!dim.resolve_result->is_compile_time_constant()) throw std::invalid_argument("given Block is invalid!");
    }
    int dimensions = new_arr->indices().count();
    std::vector<int> dimension_sizes;
    for (const TranslatedExpression& dim : translated_dimensions) dimension_sizes.push_back(std::get<std::int32_t>(dim.resolve_result->constant_value()));
    // upstream's ArrayInitializer: the expression and the number of elements added so far
    // (not Expression.Elements.Count: https://github.com/icsharpcode/ILSpy/issues/1202)
    struct ArrayInitializer {
        ArrayInitializerExpression* expression;
        int current_element_count = 0;
    };
    std::vector<ArrayInitializer> container;
    ArrayInitializer root{make<ArrayInitializerExpression>()};
    ArrayInitializerExpression* root_expression = root.expression;
    container.push_back(root);
    std::vector<ResolveResult*> element_resolve_results;

    for (int i = 1; i < block->instructions.count(); i++) {
        ILInstruction* target = nullptr;
        ILInstruction* value = nullptr;
        const IType* t = nullptr;
        if (!block->instructions[i]->match_st_obj(&target, &value, &t) || !type->equals(t)) throw std::invalid_argument("given Block is invalid!");
        ILInstruction* array = nullptr;
        if (!target->match_ld_elema(&t, &array) || !type->equals(t)) throw std::invalid_argument("given Block is invalid!");
        ILVariable* v = nullptr;
        if (!array->match_ld_loc(&v) || v != final_inst->variable()) throw std::invalid_argument("given Block is invalid!");
        while (static_cast<int>(container.size()) < dimensions) {
            auto* aie = make<ArrayInitializerExpression>();
            ArrayInitializer& parent_initializer = container.back();
            parent_initializer.expression->elements().add(aie);
            parent_initializer.current_element_count++;
            container.push_back(ArrayInitializer{aie});
        }
        TranslatedExpression val;
        bool old = ast_builder.use_special_constants;
        struct Restore {
            syntax::TypeSystemAstBuilder* builder;
            bool old;
            ~Restore() { builder->use_special_constants = old; }
        } restore{&ast_builder, old};
        ast_builder.use_special_constants = !is_csharp_primitive_integer_type(*type) && !is_known_type(type, KnownTypeCode::Decimal);
        val = translate(value, type).convert_to(type, *this, false, true);
        ast_builder.use_special_constants = old;
        ArrayInitializer& current_initializer = container.back();
        current_initializer.expression->elements().add(val.expression);
        current_initializer.current_element_count++;
        element_resolve_results.push_back(val.resolve_result);
        while (!container.empty() && container.back().current_element_count == dimension_sizes[container.size() - 1]) {
            container.pop_back();
        }
    }
    std::vector<ArraySpecifier*> additional_specifiers;
    AstType* type_expression;
    if (settings->anonymous_types && contains_anonymous_type(type)) {
        type_expression = nullptr;
        additional_specifiers = {make<ArraySpecifier>()};
    } else {
        type_expression = convert_type(type);
        auto* comp_type = as<ComposedType>(type_expression);
        if (comp_type != nullptr && comp_type->array_specifiers().count() > 0) {
            for (ArraySpecifier* a : comp_type->array_specifiers()) additional_specifiers.push_back(clone_as(a));
            comp_type->array_specifiers().clear();
        }
    }
    auto* expr = make<ArrayCreateExpression>();
    expr->set_type(type_expression);
    expr->set_initializer(root_expression);
    expr->additional_array_specifiers().add_range(additional_specifiers);
    if (!contains_anonymous_type(type)) {
        for (ILInstruction* i : new_arr->indices()) expr->arguments().add(translate(i).expression);
    }
    std::vector<ResolveResult*> size_rrs;
    for (ILInstruction* i : new_arr->indices()) size_rrs.push_back(translate(i).resolve_result);
    return with_rr(with_il_instruction(expr, block),
                   make<ArrayCreateResolveResult>(make_array_type(*compilation, type, dimensions), std::move(size_rrs), std::move(element_resolve_results)));
}

TranslatedExpression ExpressionBuilder::translate_stack_alloc_initializer(Block* block, const IType* type_hint) {
    auto* stloc = block->instructions.count() > 0 ? il::as<StLoc>(block->instructions[0]) : nullptr;
    auto* final_inst = il::as<LdLoc>(block->final_instruction());
    if (stloc == nullptr || final_inst == nullptr || stloc->variable() != final_inst->variable() ||
        stloc->variable()->kind() != VariableKind::InitializerTarget)
        throw std::invalid_argument("given Block is invalid!");
    StackAllocExpression* stack_alloc_expression;
    const IType* element_type = nullptr;
    const IType* t = nullptr;
    ILInstruction* stored_target = nullptr;
    ILInstruction* stored_value = nullptr;
    if (block->instructions.count() < 2 || !block->instructions[1]->match_st_obj(&stored_target, &stored_value, &t)) throw std::invalid_argument("given Block is invalid!");
    // Derive the element type from the type actually being stored when the type hint does
    // not pin it down. The hint is unreliable here: when the allocation size is a folded
    // constant (e.g. 'localloc 16' for 'stackalloc int[4]') the element type can no longer
    // be read off a 'count * sizeof(T)' expression, and the surrounding context may type the
    // buffer as a plain native int rather than a T*. In both cases falling back to 't' keeps
    // the element type consistent with the stores in the block.
    auto* pt = dynamic_cast<const PointerType*>(type_hint);
    if (pt == nullptr || !is_compatible_type_for_memory_access(*t, *pt->element_type())) {
        type_hint = make_pointer_type(t);
    }
    if (auto* loc_alloc = il::as<LocAlloc>(stloc->value())) {
        stack_alloc_expression = translate_loc_alloc(LocAllocFacts::of(*loc_alloc), type_hint, &element_type);
    } else if (auto* loc_alloc_span = il::as<LocAllocSpan>(stloc->value())) {
        stack_alloc_expression = translate_loc_alloc_span(LocAllocFacts::of(*loc_alloc_span), type_hint, &element_type);
    } else {
        throw std::invalid_argument("given Block is invalid!");
    }
    auto* initializer = make<ArrayInitializerExpression>();
    stack_alloc_expression->set_initializer(initializer);
    const PointerType* pointer_type = dynamic_cast<const PointerType*>(make_pointer_type(element_type));
    std::int64_t expected_offset = 0;

    for (int i = 1; i < block->instructions.count(); i++) {
        // stobj type(binary.add.i(ldloc I_0, conv i4->i <sign extend> (ldc.i4 offset)), value)
        ILInstruction* target = nullptr;
        ILInstruction* value = nullptr;
        if (!block->instructions[i]->match_st_obj(&target, &value, &t) || !is_compatible_type_for_memory_access(*element_type, *t))
            throw std::invalid_argument("given Block is invalid!");
        std::int64_t offset = 0;
        target = target->unwrap_conv(ConversionKind::StopGCTracking);

        if (!target->match_ld_loc(stloc->variable())) {
            ILInstruction* left = nullptr;
            ILInstruction* right = nullptr;
            if (!target->match_binary_numeric_instruction(BinaryNumericOperator::Add, &left, &right)) throw std::invalid_argument("given Block is invalid!");
            auto* binary = static_cast<BinaryNumericInstruction*>(target);
            left = left->unwrap_conv(ConversionKind::StopGCTracking);
            ILInstruction* offset_inst = PointerArithmeticOffset::detect(right, pointer_type->element_type(), binary->check_for_overflow);
            if (!left->match_ld_loc(final_inst->variable()) || offset_inst == nullptr) throw std::invalid_argument("given Block is invalid!");
            if (!offset_inst->match_ldc_i(&offset)) throw std::invalid_argument("given Block is invalid!");
        }
        while (expected_offset < offset) {
            initializer->elements().add(translate(il::transforms::TransformArrayInitializers::get_null_expression(element_type), element_type).expression);
            expected_offset++;
        }
        TranslatedExpression val = translate(value, element_type).convert_to(element_type, *this, false, true);
        initializer->elements().add(val.expression);
        expected_offset++;
    }
    return with_rr(with_il_instruction(stack_alloc_expression, block), make<ResolveResult>(stloc->variable()->type()));
}

TranslatedExpression ExpressionBuilder::translate_with_initializer(Block* block) {
    auto* stloc = block->instructions.count() > 0 ? il::as<StLoc>(block->instructions[0]) : nullptr;
    auto* final_inst = il::as<LdLoc>(block->final_instruction());
    if (stloc == nullptr || final_inst == nullptr || stloc->variable() != final_inst->variable() ||
        stloc->variable()->kind() != VariableKind::InitializerTarget)
        throw std::invalid_argument("given Block is invalid!");

    auto* with_initializer_expression = make<WithInitializerExpression>();
    with_initializer_expression->set_expression(translate(stloc->value(), stloc->variable()->type()).expression);
    with_initializer_expression->set_initializer(
        build_array_initializer_expression(block, make<InitializedObjectResolveResult>(stloc->variable()->type())));

    return with_rr(with_il_instruction(with_initializer_expression, block), make<ResolveResult>(stloc->variable()->type()));
}

// If expr is a constant integer expression, and its value fits into type,
// convert the expression into the target type.
// Otherwise, returns the expression unmodified.
TranslatedExpression ExpressionBuilder::adjust_constant_expression_to_type(TranslatedExpression expr, const IType* type_hint) {
    ResolveResult* new_rr = adjust_constant_to_type(expr.resolve_result, type_hint);
    if (new_rr == expr.resolve_result) return expr;
    return convert_constant_value(new_rr, true).with_il_instruction(expr.il_instructions());
}

ResolveResult* ExpressionBuilder::adjust_constant_to_type(ResolveResult* rr, const IType* type_hint) {
    if (!rr->is_compile_time_constant()) return rr;
    type_hint = nullable_type::get_underlying_type(type_hint);
    if (rr->type()->equals(type_hint)) return rr;
    // Convert to type hint, if this is possible without loss of accuracy
    ConstantValue value = rr->constant_value();
    bool is_zero = (std::holds_alternative<std::int32_t>(value) && std::get<std::int32_t>(value) == 0) ||
                   (std::holds_alternative<std::uint32_t>(value) && std::get<std::uint32_t>(value) == 0u);
    bool is_one = (std::holds_alternative<std::int32_t>(value) && std::get<std::int32_t>(value) == 1) ||
                  (std::holds_alternative<std::uint32_t>(value) && std::get<std::uint32_t>(value) == 1u);
    if (is_known_type(type_hint, KnownTypeCode::Boolean)) {
        if (is_zero) {
            rr = make<ConstantResolveResult>(type_hint, ConstantValue(false));
        } else if (is_one) {
            rr = make<ConstantResolveResult>(type_hint, ConstantValue(true));
        }
    } else if (type_hint->kind() == TypeKind::Enum || is_known_type(type_hint, KnownTypeCode::Char) || is_csharp_small_integer_type(*type_hint)) {
        ResolveResult* cast_rr = resolver->with_check_for_overflow(true).resolve_cast(type_hint, rr);
        if (cast_rr->is_compile_time_constant() && !cast_rr->is_error()) {
            rr = cast_rr;
        }
    } else if (is_any_pointer(type_hint->kind()) && is_zero) {
        rr = make<ConstantResolveResult>(type_hint, ConstantValue());
    }
    return rr;
}

TranslatedExpression ExpressionBuilder::visit_null_coalescing_instruction(NullCoalescingInstruction& inst, TranslationContext) { return translate_null_coalescing(NullCoalescingFacts::of(inst)); }

TranslatedExpression ExpressionBuilder::translate_null_coalescing(const NullCoalescingFacts& inst) {
    TranslatedExpression value = translate(inst.value_inst());
    TranslatedExpression fallback = translate(inst.fallback_inst());
    fallback = adjust_constant_expression_to_type(fallback, value.type());
    ResolveResult* rr = resolver->resolve_binary_operator(BinaryOperatorType::NullCoalescing, value.resolve_result, fallback.resolve_result);
    if (rr->is_error()) {
        const IType* target_type;
        if (is<ThrowExpression>(fallback.expression) && fallback.type()->equals(&SpecialType::no_type())) {
            target_type = nullable_type::get_underlying_type(value.type());
        } else if (!value.type()->equals(&SpecialType::null_type()) && !fallback.type()->equals(&SpecialType::null_type()) &&
                   !value.type()->equals(fallback.type())) {
            target_type = known(to_known_type_code(inst.underlying_result_type));
        } else {
            target_type = value.type()->equals(&SpecialType::null_type()) ? fallback.type() : value.type();
        }
        if (inst.kind != NullCoalescingKind::Ref) {
            value = value.convert_to(nullable_type::create(*compilation, target_type), *this);
        } else {
            value = value.convert_to(target_type, *this);
        }
        if (inst.kind == NullCoalescingKind::Nullable) {
            value = value.convert_to(nullable_type::create(*compilation, target_type), *this);
        } else {
            fallback = fallback.convert_to(target_type, *this);
        }
        rr = make<ResolveResult>(target_type);
    }
    return with_il_instruction(make<BinaryOperatorExpression>(value.expression, BinaryOperatorType::NullCoalescing, fallback.expression), inst.self).with_rr(rr);
}

TranslatedExpression ExpressionBuilder::visit_if_instruction(IfInstruction& inst, TranslationContext context) { return translate_if(IfFacts::of(inst), context); }

TranslatedExpression ExpressionBuilder::translate_if(const IfFacts& inst, TranslationContext context) {
    TranslatedExpression condition = translate_condition(inst.condition());
    TranslatedExpression true_branch = translate(inst.true_inst(), context.type_hint);
    TranslatedExpression false_branch = translate(inst.false_inst(), context.type_hint);
    BinaryOperatorType op = BinaryOperatorType::Any;
    TranslatedExpression rhs;

    ILInstruction* lhs_inst = nullptr;
    ILInstruction* rhs_inst = nullptr;
    if ((l2 != nullptr ? l2->match_logic_and(*inst.self, &lhs_inst, &rhs_inst) : inst.match_logic_and(&lhs_inst, &rhs_inst)) &&
        !(l2 != nullptr ? l2->is_ldc_i4(*rhs_inst, 1) : rhs_inst->match_ldc_i4(1))) {
        op = BinaryOperatorType::ConditionalAnd;
        assert(rhs_inst == inst.true_inst());
        rhs = true_branch;
    } else if ((l2 != nullptr ? l2->match_logic_or(*inst.self, &lhs_inst, &rhs_inst) : inst.match_logic_or(&lhs_inst, &rhs_inst)) &&
               !(l2 != nullptr ? l2->is_ldc_i4(*rhs_inst, 0) : rhs_inst->match_ldc_i4(0))) {
        op = BinaryOperatorType::ConditionalOr;
        assert(rhs_inst == inst.false_inst());
        rhs = false_branch;
    }
    // ILAst LogicAnd/LogicOr can return a different value than 0 or 1
    // if the rhs is evaluated.
    // We can only correctly translate it to C# if the rhs is of type boolean:
    if (op != BinaryOperatorType::Any && (is_known_type(rhs.type(), KnownTypeCode::Boolean) || IfInstruction::is_in_condition_slot(inst.self))) {
        if (get_size(get_stack_type(*rhs.type())) > 4) {
            rhs = rhs.convert_to(find_type(StackType::I4, get_sign(*rhs.type())), *this);
        }
        rhs = rhs.convert_to_boolean(*this);
        return with_il_instruction(make<BinaryOperatorExpression>(condition.expression, op, rhs.expression), inst.self)
            .with_rr(make<ResolveResult>(known(KnownTypeCode::Boolean)));
    }

    condition = condition.unwrap_implicit_bool_conversion();
    true_branch = adjust_constant_expression_to_type(true_branch, false_branch.type());
    false_branch = adjust_constant_expression_to_type(false_branch, true_branch.type());

    ResolveResult* rr = resolver->resolve_conditional(condition.resolve_result, true_branch.resolve_result, false_branch.resolve_result);
    if (rr->is_error()) {
        const IType* target_type;
        if (!true_branch.type()->equals(&SpecialType::null_type()) && !false_branch.type()->equals(&SpecialType::null_type()) &&
            !true_branch.type()->equals(false_branch.type())) {
            bool success = false;
            target_type = type_inference.get_best_common_type({true_branch.resolve_result, false_branch.resolve_result}, &success);
            if (!success || get_stack_type(*target_type) != inst.result_type()) {
                // Figure out the target type based on inst.ResultType.
                if (context.type_hint->kind() != TypeKind::Unknown && get_stack_type(*context.type_hint) == inst.result_type()) {
                    target_type = context.type_hint;
                } else if (inst.result_type() == StackType::Ref) {
                    // target_type should be a ref-type
                    if (true_branch.type()->kind() == TypeKind::ByReference) {
                        target_type = true_branch.type();
                    } else if (false_branch.type()->kind() == TypeKind::ByReference) {
                        target_type = false_branch.type();
                    } else {
                        // fall back to 'ref byte' if we can't determine a referenced type otherwise
                        target_type = make_by_reference_type(known(KnownTypeCode::Byte));
                    }
                } else {
                    target_type = find_type(inst.result_type(), get_sign(*context.type_hint));
                }
            }
        } else {
            target_type = true_branch.type()->equals(&SpecialType::null_type()) ? false_branch.type() : true_branch.type();
        }
        true_branch = true_branch.convert_to(target_type, *this);
        false_branch = false_branch.convert_to(target_type, *this);
        rr = make<ResolveResult>(target_type);
    }
    if (rr->type()->kind() == TypeKind::ByReference) {
        // C# conditional ref looks like this:
        // ref (arr != null ? ref trueBranch : ref falseBranch);
        auto* conditional_resolve_result = make<ResolveResult>(dynamic_cast<const ByReferenceType*>(rr->type())->element_type());
        TranslatedExpression conditional =
            with_il_instruction(make<ConditionalExpression>(condition.expression, true_branch.expression, false_branch.expression), inst.self)
                .with_rr(conditional_resolve_result);
        return without_il_instruction(make<DirectionExpression>(FieldDirection::Ref, conditional.expression))
            .with_rr(make<ByReferenceResolveResult>(conditional_resolve_result, ReferenceKind::Ref));
    }
    return with_il_instruction(make<ConditionalExpression>(condition.expression, true_branch.expression, false_branch.expression), inst.self).with_rr(rr);
}

ExpressionBuilder::SwitchValue ExpressionBuilder::translate_switch_value(const SwitchFacts& inst, bool is_expression_context) {
    TranslatedExpression value;
    const IType* governing_type;
    // prepare expression and expected type
    // first try to guess a governing type
    auto* str_to_int = il::as<StringToInt>(inst.value());
    if (str_to_int != nullptr) {
        value = translate(str_to_int->argument());
        governing_type = str_to_int->expected_type() != nullptr ? str_to_int->expected_type() : known(KnownTypeCode::String);
    } else {
        value = translate(inst.value());
        governing_type = inst.type != nullptr ? inst.type : value.type();

        // validate the governing type
        if (inst.value()->result_type() == StackType::I8) {
            if (get_stack_type(*governing_type) != StackType::I8) governing_type = find_type(StackType::I8, get_sign(*governing_type));
        } else if (inst.value()->result_type() == StackType::I4) {
            if (get_stack_type(*governing_type) != StackType::I4) governing_type = find_type(StackType::I4, get_sign(*governing_type));
            if (is_small_integer_type(*governing_type)) {
                const SwitchSectionFacts* default_section = inst.get_default_section();
                int bits = 8 * get_size(*governing_type);
                int min_value = get_sign(*governing_type) == Sign::Unsigned ? 0 : -(1 << (bits - 1));
                int max_value = get_sign(*governing_type) == Sign::Unsigned ? (1 << bits) - 1 : (1 << (bits - 1)) - 1;
                for (const SwitchSectionFacts* section : inst.sections()) {
                    if (section == default_section) continue;
                    util::LongInterval interval = section->labels().containing_interval();
                    if (interval.start < min_value || interval.inclusive_end() > max_value) {
                        // governing type is too small to hold all case values
                        governing_type = find_type(StackType::I4, Sign::Signed);
                        break;
                    }
                }
            }
        } else {
            assert(inst.value()->result_type() == StackType::O);
            assert(inst.is_lifted);
            assert(inst.type == governing_type);
        }
    }

    if (is_expression_context) {
        value = value.convert_to(governing_type, *this, false, false);
    } else {
        value = value.convert_to(governing_type, *this, false, true);

        const IType* csharp_governing_type = get_csharp_switch_governing_type(value.type());
        if (!csharp_governing_type->equals(governing_type)) {
            value = value.convert_to(governing_type, *this, false, false);
        }
    }

    const IType* case_type = str_to_int != nullptr ? known(KnownTypeCode::String) : governing_type;

    return SwitchValue{value, case_type, str_to_int};
}

const IType* ExpressionBuilder::get_csharp_switch_governing_type(const IType* type) {
    auto is_compatible_with_switch = [](const IType* t) {
        t = nullable_type::get_underlying_type(t);
        return is_known_type(t, KnownTypeCode::Boolean) || is_known_type(t, KnownTypeCode::SByte) || is_known_type(t, KnownTypeCode::Byte) ||
               is_known_type(t, KnownTypeCode::Int16) || is_known_type(t, KnownTypeCode::UInt16) || is_known_type(t, KnownTypeCode::Int32) ||
               is_known_type(t, KnownTypeCode::UInt32) || is_known_type(t, KnownTypeCode::Int64) || is_known_type(t, KnownTypeCode::UInt64) ||
               is_known_type(t, KnownTypeCode::Char) || is_known_type(t, KnownTypeCode::String);
    };
    auto is_implicit_conversion_operator = [](const IMethod* operator_method) {
        if (!operator_method->is_operator()) return false;
        if (operator_method->name() != "op_Implicit") return false;
        if (operator_method->parameters().size() != 1) return false;
        return true;
    };
    if (is_compatible_with_switch(type)) return type;

    std::vector<const IMethod*> applicable_implicit_conversion_operators;
    for (const IMethod* m : type->get_methods(is_implicit_conversion_operator)) {
        if (is_compatible_with_switch(m->return_type())) applicable_implicit_conversion_operators.push_back(m);
    }
    if (applicable_implicit_conversion_operators.size() != 1) return type;
    return applicable_implicit_conversion_operators[0]->return_type();
}

TranslatedExpression ExpressionBuilder::visit_switch_instruction(SwitchInstruction& inst, TranslationContext context) {
    return translate_switch_expression(SwitchFacts::of(inst), context);
}

TranslatedExpression ExpressionBuilder::translate_switch_expression(const SwitchFacts& inst, TranslationContext context) {
    // switch-expression does not support implicit conversions
    SwitchValue switch_value = translate_switch_value(inst, true);
    TranslatedExpression value = switch_value.value;
    const IType* type = switch_value.case_type;
    StringToInt* str_to_int = switch_value.string_to_int;

    const SwitchSectionFacts* default_section = inst.get_default_section();
    auto* switch_expr = make<SwitchExpression>();
    switch_expr->set_expression(value.expression);
    const IType* result_type;
    if (context.type_hint->kind() != TypeKind::Unknown && get_stack_type(*context.type_hint) == inst.result_type()) {
        result_type = context.type_hint;
    } else {
        result_type = known(to_known_type_code(inst.result_type()));
    }
    auto translate_section_body = [&](const SwitchSectionFacts* section) -> Expression* {
        TranslatedExpression body = translate(section->body(), result_type);
        return body.convert_to(result_type, *this, false, true).expression;
    };

    for (const SwitchSectionFacts* section : inst.sections()) {
        if (section == default_section) continue;
        auto* ses = make<SwitchExpressionSection>();
        if (section->has_null_label()) {
            assert(section->labels().is_empty());
            ses->set_pattern(make<NullReferenceExpression>());
        } else {
            std::vector<std::int64_t> label_values = section->labels().values();
            assert(label_values.size() == 1);
            std::vector<ConstantResolveResult*> rrs = statement_builder->create_typed_case_label(label_values.front(), type,
                                                                                                 str_to_int != nullptr ? &str_to_int->map() : nullptr);
            assert(rrs.size() == 1);
            ses->set_pattern(ast_builder.convert_constant_value(rrs.front()));
        }
        ses->set_body(translate_section_body(section));
        switch_expr->switch_sections().add(ses);
    }

    if (!default_section->is_compiler_generated_default_section()) {
        auto* default_ses = make<SwitchExpressionSection>();
        default_ses->set_pattern(make<IdentifierExpression>("_"));
        default_ses->set_body(translate_section_body(default_section));
        switch_expr->switch_sections().add(default_ses);
    }

    return with_il_instruction(switch_expr, inst.self).with_rr(make<ResolveResult>(result_type));
}

TranslatedExpression ExpressionBuilder::visit_address_of(AddressOf& inst, TranslationContext) { return translate_address_of(AddressOfFacts::of(inst)); }

TranslatedExpression ExpressionBuilder::translate_address_of(const AddressOfFacts& inst) {
    il::transforms::ExpressionClassification classification = il::transforms::ILInlining::classify_expression(inst.value());
    TranslatedExpression value = translate(inst.value(), inst.type());
    value = value.convert_to(inst.type(), *this);
    auto can_ignore_copy = [&]() {
        ILInstruction* load_address = inst.self;
        while (auto* parent = il::as<LdFlda>(load_address->parent())) {
            load_address = parent;
        }
        if (il::as<LdObj>(load_address->parent()) != nullptr) {
            // Ignore copy, never introduce a cast
            return true;
        }
        return false;
    };
    // ILAst AddressOf copies the value to a temporary, but when invoking a method in C#
    // on a mutable lvalue, we would end up modifying the original lvalue, not just the copy.
    // We solve this by introducing a "redundant" cast. Casts are classified as rvalue
    // and ensure that the C# compiler will also create a copy.
    if (classification == il::transforms::ExpressionClassification::MutableLValue && !can_ignore_copy() && !is<CastExpression>(value.expression)) {
        value = without_il_instruction(make<CastExpression>(convert_type(inst.type()), value.expression))
                    .with_rr(make<ConversionResolveResult>(inst.type(), value.resolve_result, Conversion::identity_conversion()));
    }
    return with_il_instruction(make<DirectionExpression>(FieldDirection::Ref, value.expression), inst.self)
        .with_rr(make<ByReferenceResolveResult>(value.resolve_result, ReferenceKind::Ref));
}

TranslatedExpression ExpressionBuilder::visit_await(Await& inst, TranslationContext) { return translate_await(AwaitFacts::of(inst)); }

TranslatedExpression ExpressionBuilder::translate_await(const AwaitFacts& inst) {
    const IType* expected_type = nullptr;
    if (inst.get_awaiter_method != nullptr) {
        if (inst.get_awaiter_method->is_static()) {
            if (!inst.get_awaiter_method->parameters().empty()) expected_type = inst.get_awaiter_method->parameters().front()->type();
        } else {
            expected_type = inst.get_awaiter_method->declaring_type();
        }
    }

    TranslatedExpression value = translate(inst.value(), expected_type);
    if (is<DirectionExpression>(value.expression)) {
        // we can deference the managed reference by stripping away the 'ref'
        value = value.unwrap_child(static_cast<DirectionExpression*>(value.expression)->expression());
    }
    if (expected_type != nullptr) {
        value = value.convert_to(expected_type, *this, false, true);
    }
    return with_il_instruction(make<UnaryOperatorExpression>(UnaryOperatorType::Await, value.expression), inst.self)
        .with_rr(make<ResolveResult>(inst.get_result_method != nullptr ? inst.get_result_method->return_type() : &SpecialType::unknown_type()));
}

TranslatedExpression ExpressionBuilder::visit_nullable_rewrap(NullableRewrap& inst, TranslationContext) { return translate_nullable_rewrap(OperandFacts::of(inst)); }

TranslatedExpression ExpressionBuilder::translate_nullable_rewrap(const OperandFacts& inst) {
    TranslatedExpression arg = translate(inst.argument());
    const IType* type = arg.type();
    if (nullable_type::is_non_nullable_value_type(type)) {
        type = nullable_type::create(*compilation, type);
    }
    return with_il_instruction(make<UnaryOperatorExpression>(UnaryOperatorType::NullConditionalRewrap, arg.expression), inst.self)
        .with_rr(make<ResolveResult>(type));
}

TranslatedExpression ExpressionBuilder::visit_nullable_unwrap(NullableUnwrap& inst, TranslationContext) { return translate_nullable_unwrap(NullableUnwrapFacts::of(inst)); }

TranslatedExpression ExpressionBuilder::translate_nullable_unwrap(const NullableUnwrapFacts& inst) {
    TranslatedExpression arg = translate(inst.argument());
    if (inst.ref_input && !inst.ref_output() && is<DirectionExpression>(arg.expression)) {
        arg = arg.unwrap_child(static_cast<DirectionExpression*>(arg.expression)->expression());
    }
    return with_il_instruction(make<UnaryOperatorExpression>(UnaryOperatorType::NullConditional, arg.expression), inst.self)
        .with_rr(make<ResolveResult>(nullable_type::get_underlying_type(arg.type())));
}

TranslatedExpression ExpressionBuilder::visit_dynamic_convert_instruction(DynamicConvertInstruction& inst, TranslationContext) { return translate_dynamic_convert(DynamicFacts::of(inst)); }

TranslatedExpression ExpressionBuilder::translate_dynamic_convert(const DynamicFacts& inst) {
    TranslatedExpression operand = translate(inst.argument()).convert_to(&SpecialType::dynamic(), *this);
    TranslatedExpression result =
        with_il_instruction(make<CastExpression>(convert_type(inst.type()), operand.expression), inst.self)
            .with_rr(make<ConversionResolveResult>(inst.type(), operand.resolve_result,
                                                   inst.is_explicit() ? Conversion::explicit_dynamic_conversion() : Conversion::implicit_dynamic_conversion()));
    annotate_checked(result.expression, inst.is_checked() ? CheckedAnnotationKind::Checked : CheckedAnnotationKind::Unchecked);
    return result;
}

namespace {

// upstream's `source.Skip(1)`
template <class Range>
auto skip_first(const Range& range) {
    return std::vector<std::remove_cv_t<std::remove_reference_t<decltype(*std::begin(range))>>>(std::next(std::begin(range)), std::end(range));
}

std::vector<Expression*> expressions_of(const std::vector<TranslatedExpression>& translated) {
    std::vector<Expression*> result;
    for (const TranslatedExpression& a : translated) result.push_back(a.expression);
    return result;
}

std::vector<ResolveResult*> resolve_results_of(const std::vector<TranslatedExpression>& translated) {
    std::vector<ResolveResult*> result;
    for (const TranslatedExpression& a : translated) result.push_back(a.resolve_result);
    return result;
}

}  // namespace

TranslatedExpression ExpressionBuilder::visit_dynamic_get_index_instruction(DynamicGetIndexInstruction& inst, TranslationContext) { return translate_dynamic_get_index(DynamicFacts::of(inst)); }

TranslatedExpression ExpressionBuilder::translate_dynamic_get_index(const DynamicFacts& inst) {
    TranslatedExpression target = translate_dynamic_target(inst.arguments()[0], inst.argument_info()[0]);
    std::vector<TranslatedExpression> arguments = translate_dynamic_arguments(skip_first(inst.arguments()), skip_first(inst.argument_info()));
    return with_il_instruction(make<IndexerExpression>(target.expression, expressions_of(arguments)), inst.self)
        .with_rr(make<DynamicInvocationResolveResult>(
            target.resolve_result, DynamicInvocationType::Indexing, resolve_results_of(arguments), std::vector<ResolveResult*>{},
            create_dynamic_indexer_symbol(dynamic_argument_type(inst.argument_info()[0]), skip_first(inst.argument_info()))));
}

TranslatedExpression ExpressionBuilder::visit_dynamic_get_member_instruction(DynamicGetMemberInstruction& inst, TranslationContext) { return translate_dynamic_get_member(DynamicFacts::of(inst)); }

TranslatedExpression ExpressionBuilder::translate_dynamic_get_member(const DynamicFacts& inst) {
    TranslatedExpression target = translate_dynamic_target(inst.target(), inst.target_argument_info());
    return with_il_instruction(make<MemberReferenceExpression>(target.expression, inst.name()), inst.self)
        .with_rr(make<DynamicMemberResolveResult>(target.resolve_result, inst.name(), create_dynamic_member_symbol(inst.name(), inst.target_argument_info())));
}

TranslatedExpression ExpressionBuilder::visit_dynamic_invoke_constructor_instruction(DynamicInvokeConstructorInstruction& inst, TranslationContext) { return translate_dynamic_invoke_constructor(DynamicFacts::of(inst)); }

TranslatedExpression ExpressionBuilder::translate_dynamic_invoke_constructor(const DynamicFacts& inst) {
    const IType* constructor_type = nullptr;
    if (!(inst.argument_info()[0].has_flag(CSharpArgumentInfoFlags::IsStaticType) &&
          il::transforms::TransformExpressionTrees::match_get_type_from_handle(inst.arguments()[0], &constructor_type)))
        return error_expression("Could not detect static type for DynamicInvokeConstructorInstruction");
    std::vector<TranslatedExpression> arguments = translate_dynamic_arguments(skip_first(inst.arguments()), skip_first(inst.argument_info()));
    const IMethod* constructor = create_dynamic_constructor_symbol(constructor_type, skip_first(inst.argument_info()));
    return with_il_instruction(make<ObjectCreateExpression>(convert_type(constructor_type), expressions_of(arguments)), inst.self)
        .with_rr(make<CSharpInvocationResolveResult>(make<ResolveResult>(constructor_type), constructor, resolve_results_of(arguments)));
}

TranslatedExpression ExpressionBuilder::visit_dynamic_invoke_member_instruction(DynamicInvokeMemberInstruction& inst, TranslationContext) { return translate_dynamic_invoke_member(DynamicFacts::of(inst)); }

TranslatedExpression ExpressionBuilder::translate_dynamic_invoke_member(const DynamicFacts& inst) {
    Expression* target_expr;
    TranslatedExpression target = translate_dynamic_target(inst.arguments()[0], inst.argument_info()[0]);
    std::vector<AstType*> type_arguments;
    for (const IType* t : inst.type_arguments()) type_arguments.push_back(convert_type(t));
    if ((inst.binder_flags() & CSharpBinderFlags::InvokeSimpleName) != CSharpBinderFlags::None && is<ThisReferenceExpression>(target.expression)) {
        auto* identifier = make<IdentifierExpression>(inst.name());
        identifier->type_arguments().add_range(type_arguments);
        target_expr = identifier;
    } else {
        target_expr = make<MemberReferenceExpression>(target.expression, inst.name(), type_arguments);
    }
    std::vector<TranslatedExpression> arguments = translate_dynamic_arguments(skip_first(inst.arguments()), skip_first(inst.argument_info()));
    return with_il_instruction(make<InvocationExpression>(target_expr, expressions_of(arguments)), inst.self)
        .with_rr(make<DynamicInvocationResolveResult>(
            target.resolve_result, DynamicInvocationType::Invocation, resolve_results_of(arguments), std::vector<ResolveResult*>{},
            create_dynamic_invoke_member_symbol(inst.name(), inst.argument_info()[0], skip_first(inst.argument_info()), inst.type_arguments())));
}

TranslatedExpression ExpressionBuilder::visit_dynamic_invoke_instruction(DynamicInvokeInstruction& inst, TranslationContext) { return translate_dynamic_invoke(DynamicFacts::of(inst)); }

TranslatedExpression ExpressionBuilder::translate_dynamic_invoke(const DynamicFacts& inst) {
    TranslatedExpression target = translate_dynamic_target(inst.arguments()[0], inst.argument_info()[0]);
    std::vector<TranslatedExpression> arguments = translate_dynamic_arguments(skip_first(inst.arguments()), skip_first(inst.argument_info()));
    return with_il_instruction(make<InvocationExpression>(target.expression, expressions_of(arguments)), inst.self)
        .with_rr(make<DynamicInvocationResolveResult>(target.resolve_result, DynamicInvocationType::Invocation, resolve_results_of(arguments)));
}

TranslatedExpression ExpressionBuilder::translate_dynamic_target(ILInstruction* inst, const CSharpArgumentInfo& argument_info) {
    assert(!argument_info.has_flag(CSharpArgumentInfoFlags::NamedArgument));
    assert(!argument_info.has_flag(CSharpArgumentInfoFlags::IsOut));

    const IType* call_target_type = nullptr;
    if (argument_info.has_flag(CSharpArgumentInfoFlags::IsStaticType) &&
        il::transforms::TransformExpressionTrees::match_get_type_from_handle(inst, &call_target_type)) {
        return with_rr(make<TypeReferenceExpression>(convert_type(call_target_type)), make<TypeResolveResult>(call_target_type)).without_il_instruction();
    }

    const IType* target_type = &SpecialType::dynamic();
    if (argument_info.has_flag(CSharpArgumentInfoFlags::UseCompileTimeType)) {
        target_type = argument_info.compile_time_type;
    }

    TranslatedExpression translated_target = translate(inst, target_type).convert_to(target_type, *this);

    if (argument_info.has_flag(CSharpArgumentInfoFlags::IsRef) && is<DirectionExpression>(translated_target.expression)) {
        // (ref x).member => x.member
        translated_target = translated_target.unwrap_child(static_cast<DirectionExpression*>(translated_target.expression)->expression());
    }

    return translated_target;
}

// The type to show for a dynamic callsite argument: its recorded compile-time type when the runtime
// binder was told to use it (statically-typed or constant arguments), otherwise `dynamic`.
const IType* ExpressionBuilder::dynamic_argument_type(const CSharpArgumentInfo& info) {
    if ((info.has_flag(CSharpArgumentInfoFlags::UseCompileTimeType) || info.has_flag(CSharpArgumentInfoFlags::Constant)) && info.compile_time_type != nullptr) {
        return info.compile_time_type;
    }
    return &SpecialType::dynamic();
}

// Synthesizes a member on the target type for a dynamic member access (a.Member), so the member
// reference carries a navigable symbol / hover tooltip. Since the real member is unknown, this is a
// `dynamic`-typed field named after the accessed member, declared on the target's compile-time type.
const IMember* ExpressionBuilder::create_dynamic_member_symbol(const std::string& name, const CSharpArgumentInfo& target_info) {
    auto field = std::make_unique<FakeField>(*compilation);
    field->name_value = name;
    field->return_type_value = &SpecialType::dynamic();
    field->declaring_type_value = dynamic_argument_type(target_info);
    return compilation->arena().keep(std::move(field));
}

// Synthesizes a member on the target type for a dynamic member invocation (a.Method(b)): a
// `dynamic`-returning method named after the invoked member, with one parameter per argument
// typed by dynamic_argument_type, so the member reference carries a navigable symbol /
// hover tooltip.
const IMember* ExpressionBuilder::create_dynamic_invoke_member_symbol(const std::string& name, const CSharpArgumentInfo& target_info,
                                                                       const std::vector<CSharpArgumentInfo>& argument_info,
                                                                       const std::vector<const IType*>& type_arguments) {
    auto method_owner = std::make_unique<FakeMethod>(*compilation, SymbolKind::Method);
    FakeMethod* method = compilation->arena().keep(std::move(method_owner));
    method->name_value = name;
    method->return_type_value = &SpecialType::dynamic();
    method->declaring_type_value = dynamic_argument_type(target_info);
    if (!argument_info.empty()) {
        for (const CSharpArgumentInfo& info : argument_info) {
            method->parameters_value.push_back(compilation->arena().keep(std::make_unique<DefaultParameter>(dynamic_argument_type(info), info.name)));
        }
    }
    if (type_arguments.empty()) return method;
    // Explicit type arguments (a.Method<int>()): give the fake a matching set of type parameters,
    // conventionally named T, T2, ..., and specialize it with the actual arguments like a real
    // generic call would.
    for (std::size_t i = 0; i < type_arguments.size(); i++) {
        method->type_parameters_value.push_back(compilation->arena().keep(
            std::make_unique<DefaultTypeParameter>(method, static_cast<int>(i), i == 0 ? std::string("T") : "T" + std::to_string(i + 1))));
    }
    return SpecializedMethod::create(method, TypeParameterSubstitution(std::nullopt, type_arguments));
}

// Synthesizes the constructor for a dynamic object creation (`new T(b)` where an argument is
// dynamic): a constructor on the created type with one parameter per argument typed by
// dynamic_argument_type, so the type name and parentheses carry a hover tooltip.
const IMethod* ExpressionBuilder::create_dynamic_constructor_symbol(const IType* declaring_type, const std::vector<CSharpArgumentInfo>& argument_info) {
    FakeMethod* constructor = compilation->arena().keep(std::make_unique<FakeMethod>(*compilation, SymbolKind::Constructor));
    constructor->name_value = ".ctor";
    constructor->declaring_type_value = declaring_type;
    constructor->return_type_value = known(KnownTypeCode::Void);
    for (const CSharpArgumentInfo& info : argument_info) {
        constructor->parameters_value.push_back(compilation->arena().keep(std::make_unique<DefaultParameter>(dynamic_argument_type(info), info.name)));
    }
    return constructor;
}

// Synthesizes the indexer for a dynamic index access (a[b]): an indexer on the target type whose
// parameters are typed from the callsite delegate, so the brackets carry a hover tooltip.
const IMember* ExpressionBuilder::create_dynamic_indexer_symbol(const IType* declaring_type, const std::vector<CSharpArgumentInfo>& argument_info) {
    FakeProperty* property = compilation->arena().keep(std::make_unique<FakeProperty>(*compilation));
    for (const CSharpArgumentInfo& info : argument_info) {
        property->parameters_value.push_back(compilation->arena().keep(std::make_unique<DefaultParameter>(dynamic_argument_type(info), info.name)));
    }
    property->name_value = "Item";
    property->is_indexer_value = true;
    property->return_type_value = &SpecialType::dynamic();
    property->declaring_type_value = declaring_type;
    return property;
}

std::vector<TranslatedExpression> ExpressionBuilder::translate_dynamic_arguments(const std::vector<ILInstruction*>& arguments,
                                                                                  const std::vector<CSharpArgumentInfo>& argument_info) {
    std::vector<TranslatedExpression> result;
    std::size_t count = std::min(arguments.size(), argument_info.size());
    for (std::size_t i = 0; i < count; i++) {
        result.push_back(translate_dynamic_argument(arguments[i], argument_info[i]));
    }
    return result;
}

TranslatedExpression ExpressionBuilder::translate_dynamic_argument(ILInstruction* argument, const CSharpArgumentInfo& info) {
    assert(!info.has_flag(CSharpArgumentInfoFlags::IsStaticType));

    const IType* type_hint = &SpecialType::dynamic();
    if (info.has_flag(CSharpArgumentInfoFlags::UseCompileTimeType)) {
        type_hint = info.compile_time_type;
    }
    TranslatedExpression translated_expression = translate(argument, type_hint);
    if (!(type_hint->equals(&SpecialType::dynamic()) && translated_expression.type()->equals(&SpecialType::null_type()))) {
        translated_expression = translated_expression.convert_to(type_hint, *this);
    }
    if (info.has_flag(CSharpArgumentInfoFlags::IsOut)) {
        translated_expression = change_direction_expression_to(translated_expression, ReferenceKind::Out, il::as<AddressOf>(argument) != nullptr);
    }
    bool name_is_blank = std::all_of(info.name.begin(), info.name.end(), [](unsigned char c) { return std::isspace(c) != 0; });
    if (info.has_flag(CSharpArgumentInfoFlags::NamedArgument) && !name_is_blank) {
        translated_expression = TranslatedExpression(make<NamedArgumentExpression>(info.name, translated_expression.expression));
    }

    return translated_expression;
}

TranslatedExpression ExpressionBuilder::change_direction_expression_to(TranslatedExpression input, ReferenceKind kind, bool is_address_of) {
    auto* dir_expr = as<DirectionExpression>(input.expression);
    auto* brrr = dynamic_cast<ByReferenceResolveResult*>(input.resolve_result);
    if (dir_expr == nullptr || brrr == nullptr) return input;
    if ((is_address_of || is<ThisReferenceExpression>(dir_expr->expression())) && (kind == ReferenceKind::In || kind == ReferenceKind::RefReadOnly)) {
        return input.unwrap_child(dir_expr->expression());
    }
    switch (kind) {
        case ReferenceKind::Ref: dir_expr->set_field_direction(FieldDirection::Ref); break;
        case ReferenceKind::Out: dir_expr->set_field_direction(FieldDirection::Out); break;
        case ReferenceKind::In: dir_expr->set_field_direction(FieldDirection::In); break;
        case ReferenceKind::RefReadOnly: dir_expr->set_field_direction(FieldDirection::In); break;
        default: throw std::logic_error("Unsupported reference kind");
    }
    dir_expr->remove_annotations<ByReferenceResolveResult>();
    if (brrr->element_result == nullptr)
        brrr = make<ByReferenceResolveResult>(brrr->element_type(), kind);
    else
        brrr = make<ByReferenceResolveResult>(brrr->element_result, kind);
    dir_expr->add_annotation(brrr);
    return TranslatedExpression(dir_expr);
}

TranslatedExpression ExpressionBuilder::visit_dynamic_set_index_instruction(DynamicSetIndexInstruction& inst, TranslationContext) { return translate_dynamic_set_index(DynamicFacts::of(inst)); }

TranslatedExpression ExpressionBuilder::translate_dynamic_set_index(const DynamicFacts& inst) {
    assert(inst.arguments().size() >= 3);
    TranslatedExpression target = translate_dynamic_target(inst.arguments()[0], inst.argument_info()[0]);
    std::vector<TranslatedExpression> arguments = translate_dynamic_arguments(skip_first(inst.arguments()), skip_first(inst.argument_info()));
    TranslatedExpression value = arguments.back();
    std::vector<TranslatedExpression> index_arguments(arguments.begin(), arguments.end() - 1);
    std::vector<CSharpArgumentInfo> index_info(inst.argument_info().begin() + 1, inst.argument_info().begin() + 1 + (inst.argument_info().size() - 2));
    TranslatedExpression indexer =
        without_il_instruction(make<IndexerExpression>(target.expression, expressions_of(index_arguments)))
            .with_rr(make<DynamicInvocationResolveResult>(target.resolve_result, DynamicInvocationType::Indexing, resolve_results_of(index_arguments), std::vector<ResolveResult*>{},
                                                          create_dynamic_indexer_symbol(dynamic_argument_type(inst.argument_info()[0]), index_info)));
    return assignment(indexer, value).with_il_instruction(inst.self);
}

TranslatedExpression ExpressionBuilder::visit_dynamic_set_member_instruction(DynamicSetMemberInstruction& inst, TranslationContext) { return translate_dynamic_set_member(DynamicFacts::of(inst)); }

TranslatedExpression ExpressionBuilder::translate_dynamic_set_member(const DynamicFacts& inst) {
    TranslatedExpression target = translate_dynamic_target(inst.target(), inst.target_argument_info());
    TranslatedExpression value = translate_dynamic_argument(inst.value(), inst.value_argument_info());
    TranslatedExpression member =
        without_il_instruction(make<MemberReferenceExpression>(target.expression, inst.name()))
            .with_rr(make<DynamicMemberResolveResult>(target.resolve_result, inst.name(), create_dynamic_member_symbol(inst.name(), inst.target_argument_info())));
    return assignment(member, value).with_il_instruction(inst.self);
}

TranslatedExpression ExpressionBuilder::visit_dynamic_binary_operator_instruction(DynamicBinaryOperatorInstruction& inst, TranslationContext context) { return translate_dynamic_binary_operator(DynamicFacts::of(inst), context); }

TranslatedExpression ExpressionBuilder::translate_dynamic_binary_operator(const DynamicFacts& inst, TranslationContext context) {
    bool checked_context = (inst.binder_flags() & CSharpBinderFlags::CheckedContext) != CSharpBinderFlags::None;
    auto create_binary_operator = [&](BinaryOperatorType operator_type, std::optional<bool> is_checked = std::nullopt) {
        TranslatedExpression left = translate_dynamic_argument(inst.left(), inst.left_argument_info());
        TranslatedExpression right = translate_dynamic_argument(inst.right(), inst.right_argument_info());
        auto* boe = make<BinaryOperatorExpression>(left.expression, operator_type, right.expression);
        if (is_checked == true)
            annotate_checked(boe, CheckedAnnotationKind::Checked);
        else if (is_checked == false)
            annotate_checked(boe, CheckedAnnotationKind::Unchecked);
        return with_il_instruction(boe, inst.self).with_rr(make<ResolveResult>(&SpecialType::dynamic()));
    };
    switch (inst.operation()) {
        case ExpressionType::Add:
        case ExpressionType::AddAssign: return create_binary_operator(BinaryOperatorType::Add, checked_context);
        case ExpressionType::AddChecked:
        case ExpressionType::AddAssignChecked: return create_binary_operator(BinaryOperatorType::Add, true);
        case ExpressionType::Subtract:
        case ExpressionType::SubtractAssign: return create_binary_operator(BinaryOperatorType::Subtract, checked_context);
        case ExpressionType::SubtractChecked:
        case ExpressionType::SubtractAssignChecked: return create_binary_operator(BinaryOperatorType::Subtract, true);
        case ExpressionType::Multiply:
        case ExpressionType::MultiplyAssign: return create_binary_operator(BinaryOperatorType::Multiply, checked_context);
        case ExpressionType::MultiplyChecked:
        case ExpressionType::MultiplyAssignChecked: return create_binary_operator(BinaryOperatorType::Multiply, true);
        case ExpressionType::Divide:
        case ExpressionType::DivideAssign: return create_binary_operator(BinaryOperatorType::Divide);
        case ExpressionType::Modulo:
        case ExpressionType::ModuloAssign: return create_binary_operator(BinaryOperatorType::Modulus);
        case ExpressionType::Equal: return create_binary_operator(BinaryOperatorType::Equality);
        case ExpressionType::NotEqual: return create_binary_operator(BinaryOperatorType::InEquality);
        case ExpressionType::LessThan: return create_binary_operator(BinaryOperatorType::LessThan);
        case ExpressionType::LessThanOrEqual: return create_binary_operator(BinaryOperatorType::LessThanOrEqual);
        case ExpressionType::GreaterThan: return create_binary_operator(BinaryOperatorType::GreaterThan);
        case ExpressionType::GreaterThanOrEqual: return create_binary_operator(BinaryOperatorType::GreaterThanOrEqual);
        case ExpressionType::And:
        case ExpressionType::AndAssign: return create_binary_operator(BinaryOperatorType::BitwiseAnd);
        case ExpressionType::Or:
        case ExpressionType::OrAssign: return create_binary_operator(BinaryOperatorType::BitwiseOr);
        case ExpressionType::ExclusiveOr:
        case ExpressionType::ExclusiveOrAssign: return create_binary_operator(BinaryOperatorType::ExclusiveOr);
        case ExpressionType::LeftShift:
        case ExpressionType::LeftShiftAssign: return create_binary_operator(BinaryOperatorType::ShiftLeft);
        case ExpressionType::RightShift:
        case ExpressionType::RightShiftAssign: return create_binary_operator(BinaryOperatorType::ShiftRight);
        default: return default_visit(*inst.self, context);
    }
}

TranslatedExpression ExpressionBuilder::visit_dynamic_logic_operator_instruction(DynamicLogicOperatorInstruction& inst, TranslationContext context) { return translate_dynamic_logic_operator(DynamicFacts::of(inst), context); }

TranslatedExpression ExpressionBuilder::translate_dynamic_logic_operator(const DynamicFacts& inst, TranslationContext context) {
    BinaryOperatorType operator_type;
    if (inst.operation() == ExpressionType::AndAlso) {
        operator_type = BinaryOperatorType::ConditionalAnd;
    } else if (inst.operation() == ExpressionType::OrElse) {
        operator_type = BinaryOperatorType::ConditionalOr;
    } else {
        assert(false && "Unknown operation for DynamicLogicOperatorInstruction");
        return default_visit(*inst.self, context);
    }
    TranslatedExpression left = translate_dynamic_argument(inst.left(), inst.left_argument_info());
    TranslatedExpression right = translate_dynamic_argument(inst.right(), inst.right_argument_info());
    auto* boe = make<BinaryOperatorExpression>(left.expression, operator_type, right.expression);
    return with_il_instruction(boe, inst.self).with_rr(make<ResolveResult>(&SpecialType::dynamic()));
}

TranslatedExpression ExpressionBuilder::visit_dynamic_unary_operator_instruction(DynamicUnaryOperatorInstruction& inst, TranslationContext context) { return translate_dynamic_unary_operator(DynamicFacts::of(inst), context); }

TranslatedExpression ExpressionBuilder::translate_dynamic_unary_operator(const DynamicFacts& inst, TranslationContext context) {
    bool checked_context = (inst.binder_flags() & CSharpBinderFlags::CheckedContext) != CSharpBinderFlags::None;
    auto create_unary_operator = [&](UnaryOperatorType operator_type, std::optional<bool> is_checked = std::nullopt) {
        TranslatedExpression operand = translate_dynamic_argument(inst.operand(), inst.operand_argument_info());
        auto* uoe = make<UnaryOperatorExpression>(operator_type, operand.expression);
        if (is_checked == true)
            annotate_checked(uoe, CheckedAnnotationKind::Checked);
        else if (is_checked == false)
            annotate_checked(uoe, CheckedAnnotationKind::Unchecked);
        return with_il_instruction(uoe, inst.self).with_rr(make<ResolveResult>(&SpecialType::dynamic()));
    };
    switch (inst.operation()) {
        case ExpressionType::Not: return create_unary_operator(UnaryOperatorType::Not);
        case ExpressionType::Decrement: return create_unary_operator(UnaryOperatorType::Decrement, checked_context);
        case ExpressionType::Increment: return create_unary_operator(UnaryOperatorType::Increment, checked_context);
        case ExpressionType::Negate: return create_unary_operator(UnaryOperatorType::Minus, checked_context);
        case ExpressionType::NegateChecked: return create_unary_operator(UnaryOperatorType::Minus, true);
        case ExpressionType::UnaryPlus: return create_unary_operator(UnaryOperatorType::Plus, checked_context);
        case ExpressionType::OnesComplement: return create_unary_operator(UnaryOperatorType::BitNot);
        case ExpressionType::IsTrue: {
            TranslatedExpression operand = translate_dynamic_argument(inst.operand(), inst.operand_argument_info());
            Expression* expr;
            if (inst.slot_info() == &IfInstruction::condition_slot) {
                // We rely on the context implicitly invoking "operator true".
                expr = make<UnaryOperatorExpression>(UnaryOperatorType::IsTrue, operand.expression);
            } else {
                // Create a dummy conditional to ensure "operator true" will be invoked.
                expr = make<ConditionalExpression>(operand.expression, make<PrimitiveExpression>(ConstantValue(true)),
                                                   make<PrimitiveExpression>(ConstantValue(false)));
            }
            return with_il_instruction(expr, inst.self).with_rr(make<ResolveResult>(known(KnownTypeCode::Boolean)));
        }
        case ExpressionType::IsFalse: {
            TranslatedExpression operand = translate_dynamic_argument(inst.operand(), inst.operand_argument_info());
            // Create a dummy conditional to ensure "operator false" will be invoked.
            Expression* expr = make<ConditionalExpression>(operand.expression, make<PrimitiveExpression>(ConstantValue(false)),
                                                           make<PrimitiveExpression>(ConstantValue(true)));
            return with_il_instruction(expr, inst.self).with_rr(make<ResolveResult>(known(KnownTypeCode::Boolean)));
        }
        default: return default_visit(*inst.self, context);
    }
}

TranslatedExpression ExpressionBuilder::visit_dynamic_compound_assign(DynamicCompoundAssign& inst, TranslationContext) { return translate_dynamic_compound_assign(DynamicFacts::of(inst)); }

TranslatedExpression ExpressionBuilder::translate_dynamic_compound_assign(const DynamicFacts& inst) {
    ExpressionWithResolveResult target;
    if (inst.target_kind == CompoundTargetKind::Address) {
        target = ld_obj(inst.target(), &SpecialType::dynamic());
    } else {
        target = translate_dynamic_argument(inst.target(), inst.target_argument_info());
    }
    TranslatedExpression value = translate_dynamic_argument(inst.value(), inst.value_argument_info());

    auto* ae = make<AssignmentExpression>(target.expression, *AssignmentExpression::get_assignment_operator_type_from_expression_type(inst.operation()),
                                          value.expression);
    if ((inst.binder_flags() & CSharpBinderFlags::CheckedContext) != CSharpBinderFlags::None)
        annotate_checked(ae, CheckedAnnotationKind::Checked);
    else
        annotate_checked(ae, CheckedAnnotationKind::Unchecked);
    return with_il_instruction(ae, inst.self)
        .with_rr(make<OperatorResolveResult>(&SpecialType::dynamic(), inst.operation(), std::vector<ResolveResult*>{target.resolve_result, value.resolve_result}));
}

TranslatedExpression ExpressionBuilder::visit_ld_ftn(LdFtn& inst, TranslationContext) { return translate_ld_ftn(MethodPointerFacts::of(inst)); }

TranslatedExpression ExpressionBuilder::translate_ld_ftn(const MethodPointerFacts& inst) {
    ExpressionWithResolveResult delegate_ref = CallBuilder(*this, *type_system_, *settings).build_method_reference(inst.method(), false);
    if (!inst.method()->is_static()) {
        // C# 9 function pointers don't support instance methods
        return with_rr(make<InvocationExpression>(make<IdentifierExpression>("__ldftn"), std::vector<Expression*>{delegate_ref.expression}),
                       make<ResolveResult>(make_pointer_type(known(KnownTypeCode::Void))))
            .with_il_instruction(inst.self);
    }
    // C# 9 function pointer
    srm::SignatureCallingConvention calling_convention = srm::SignatureCallingConvention::Default;
    std::vector<const IType*> custom_calling_conventions;
    const IAttribute* unmanaged_callers_only_attribute = nullptr;
    for (const IAttribute* attribute : inst.method()->get_attributes()) {
        if (is_known_type(attribute->attribute_type(), KnownAttribute::UnmanagedCallersOnly)) {
            unmanaged_callers_only_attribute = attribute;
            break;
        }
    }
    if (unmanaged_callers_only_attribute != nullptr) {
        calling_convention = srm::SignatureCallingConvention::Unmanaged;

        const srm::CustomAttributeNamedArgument<const IType*>* calling_conventions_argument = nullptr;
        for (const auto& named_argument : unmanaged_callers_only_attribute->named_arguments()) {
            if (named_argument.name.has_value() && *named_argument.name == "CallConvs") {
                calling_conventions_argument = &named_argument;
                break;
            }
        }
        const auto* array = calling_conventions_argument != nullptr
                                ? std::get_if<std::vector<srm::CustomAttributeTypedArgument<const IType*>>>(&calling_conventions_argument->value)
                                : nullptr;
        if (array != nullptr) {
            for (const auto& element : *array) {
                const IType* const* type_pointer = std::get_if<const IType*>(&element.value);
                if (type_pointer == nullptr || *type_pointer == nullptr) continue;
                const IType* type = *type_pointer;
                std::optional<srm::SignatureCallingConvention> found_calling_convention;
                if (type->namespace_() == "System.Runtime.CompilerServices" && calling_convention == srm::SignatureCallingConvention::Unmanaged) {
                    const std::string name = type->name();
                    if (name == "CallConvCdecl") found_calling_convention = srm::SignatureCallingConvention::CDecl;
                    else if (name == "CallConvFastcall") found_calling_convention = srm::SignatureCallingConvention::FastCall;
                    else if (name == "CallConvStdcall") found_calling_convention = srm::SignatureCallingConvention::StdCall;
                    else if (name == "CallConvThiscall") found_calling_convention = srm::SignatureCallingConvention::ThisCall;
                }
                if (found_calling_convention.has_value()) {
                    calling_convention = *found_calling_convention;
                } else {
                    custom_calling_conventions.push_back(type);
                }
            }
        }
    }
    std::vector<const IType*> parameter_types;
    std::vector<ReferenceKind> parameter_reference_kinds;
    for (const IParameter* p : inst.method()->parameters()) {
        parameter_types.push_back(p->type());
        parameter_reference_kinds.push_back(p->reference_kind());
    }
    const FunctionPointerType* ftp = compilation->arena().keep(std::make_unique<FunctionPointerType>(
        *compilation, true, calling_convention, std::move(custom_calling_conventions), inst.method()->return_type(),
        inst.method()->return_type_is_ref_read_only(), std::move(parameter_types), std::move(parameter_reference_kinds)));
    ExpressionWithResolveResult address_of = with_rr(make<UnaryOperatorExpression>(UnaryOperatorType::AddressOf, delegate_ref.expression),
                                                     make<ResolveResult>(&SpecialType::no_type()));
    address_of.expression->add_annotation(inst.self);
    const Conversion* conversion = Conversion::method_group_conversion(inst.method(), false, false);
    return with_rr(make<CastExpression>(convert_type(ftp), address_of.expression), make<ConversionResolveResult>(ftp, address_of.resolve_result, conversion))
        .without_il_instruction();
}

TranslatedExpression ExpressionBuilder::visit_ld_virt_ftn(LdVirtFtn& inst, TranslationContext) { return translate_ld_virt_ftn(MethodPointerFacts::of(inst)); }

TranslatedExpression ExpressionBuilder::translate_ld_virt_ftn(const MethodPointerFacts& inst) {
    // C# 9 function pointers don't support instance methods
    ExpressionWithResolveResult delegate_ref = CallBuilder(*this, *type_system_, *settings).build_method_reference(inst.method(), true);
    return with_rr(make<InvocationExpression>(make<IdentifierExpression>("__ldvirtftn"), std::vector<Expression*>{delegate_ref.expression}),
                   make<ResolveResult>(make_pointer_type(known(KnownTypeCode::Void))))
        .with_il_instruction(inst.self);
}

TranslatedExpression ExpressionBuilder::visit_call_indirect(CallIndirect& inst, TranslationContext) { return translate_call_indirect(CallIndirectFacts::of(inst)); }

TranslatedExpression ExpressionBuilder::translate_call_indirect(const CallIndirectFacts& inst) {
    if (inst.is_instance()) {
        return error_expression("calli with instance method signature not supportd");
    }

    TranslatedExpression function_pointer = translate(inst.function_pointer(), inst.function_pointer_type());
    if (!NormalizeTypeVisitor::type_erasure().equivalent_types(function_pointer.type(), inst.function_pointer_type())) {
        function_pointer = function_pointer.convert_to(inst.function_pointer_type(), *this);
    }
    auto* fpt = dynamic_cast<const FunctionPointerType*>(skip_modifiers(function_pointer.type()));
    auto* invocation = make<InvocationExpression>();
    invocation->set_target(function_pointer.expression);
    std::size_t count = std::min(static_cast<std::size_t>(inst.arguments.size()), std::min(fpt->parameter_types.size(), fpt->parameter_reference_kinds.size()));
    for (std::size_t i = 0; i < count; i++) {
        ILInstruction* arg_inst = inst.arguments[i];
        const IType* param_type = fpt->parameter_types[i];
        ReferenceKind param_ref_kind = fpt->parameter_reference_kinds[i];
        TranslatedExpression arg = translate(arg_inst, param_type).convert_to(param_type, *this, false, true);
        if (param_ref_kind != ReferenceKind::None) {
            arg = change_direction_expression_to(arg, param_ref_kind, il::as<AddressOf>(arg_inst) != nullptr);
        }
        invocation->arguments().add(arg.expression);
    }
    if (auto* brt = dynamic_cast<const ByReferenceType*>(skip_modifiers(fpt->return_type))) {
        auto* rr = make<ResolveResult>(brt->element_type());
        ExpressionWithResolveResult inner = with_rr(invocation, rr);
        return with_rr(make<DirectionExpression>(FieldDirection::Ref, inner.with_il_instruction(inst.self).expression),
                       make<ByReferenceResolveResult>(rr, ReferenceKind::Ref))
            .without_il_instruction();
    }
    return with_rr(invocation, make<ResolveResult>(fpt->return_type)).with_il_instruction(inst.self);
}

TranslatedExpression ExpressionBuilder::visit_deconstruct_instruction(DeconstructInstruction& inst, TranslationContext) { return translate_deconstruct(DeconstructFacts::of(inst)); }

TranslatedExpression ExpressionBuilder::translate_deconstruct(const DeconstructFacts& inst) {
    const IType* rhs_type = inst.type;
    TranslatedExpression rhs = translate(inst.tested_operand, rhs_type);
    if (auto* dir_expr = as<DirectionExpression>(rhs.expression)) {
        // Deconstructing a value type takes the address of the deconstructed value:
        // (ref x) => x
        rhs = rhs.unwrap_child(dir_expr->expression());
    }
    rhs = rhs.convert_to(rhs_type, *this);  // TODO allowImplicitConversion
    auto& assignments = inst.self->assignments()->instructions;
    int assignment_pos = 0;
    auto& inits = inst.self->init;
    int init_pos = 0;

    std::map<ILVariable*, ILVariable*> conversion_mapping;

    for (ILInstruction* conv : inst.self->conversions()->instructions) {
        ILVariable* output_variable = nullptr;
        ILVariable* input_variable = nullptr;
        if (!DeconstructInstruction::is_conversion_stloc(conv, &output_variable, &input_variable)) continue;
        conversion_mapping.emplace(input_variable, output_variable);
    }

    auto replace_assignment_target = [&](ILInstruction* target) {
        ILVariable* v = nullptr;
        if (target->match_ld_loc(&v) && v->kind() == VariableKind::DeconstructionInitTemporary) {
            assert(inits[init_pos]->variable() == v);
            if (l2 != nullptr) l2->moved(target, inits[init_pos]->value());
            target->replace_with(inits[init_pos]->value());
            init_pos++;
        }
    };
    auto construct_assignment_target = [&](ILInstruction* assignment, ILVariable* value) {
        if (auto* stloc = il::as<StLoc>(assignment)) {
            assert(stloc->value()->match_ld_loc(value));
            (void)stloc;
            (void)value;
        } else if (auto* call = dynamic_cast<CallInstruction*>(assignment)) {
            for (int i = 0; i < call->arguments().count() - 1; i++) {
                replace_assignment_target(call->arguments()[i]);
            }
            assert(call->arguments()[call->arguments().count() - 1]->match_ld_loc(value));
        } else if (auto* stobj = il::as<StObj>(assignment)) {
            ILInstruction* target = stobj->target();
            ILInstruction* nested_target = nullptr;
            const IField* field = nullptr;
            while (target->match_ld_flda(&nested_target, &field)) target = nested_target;
            replace_assignment_target(target);
            assert(stobj->value()->match_ld_loc(value));
        } else {
            throw std::logic_error("not supported");
        }
        TranslatedExpression expr = translate(assignment);
        return expr.unwrap_child(static_cast<AssignmentExpression*>(expr.expression)->left());
    };
    std::function<TupleExpression*(MatchInstruction*)> construct_tuple = [&](MatchInstruction* match_instruction) -> TupleExpression* {
        auto* expr = make<TupleExpression>();
        for (ILInstruction* sub_pattern_inst : match_instruction->sub_patterns()) {
            auto* sub_pattern = static_cast<MatchInstruction*>(sub_pattern_inst);
            if (sub_pattern->is_var()) {
                if (sub_pattern->has_designator()) {
                    ILVariable* value = sub_pattern->variable();
                    auto it = conversion_mapping.find(sub_pattern->variable());
                    if (it != conversion_mapping.end()) value = it->second;
                    expr->elements().add(construct_assignment_target(assignments[assignment_pos], value).expression);
                    assignment_pos++;
                } else {
                    expr->elements().add(make<IdentifierExpression>("_"));
                }
            } else {
                expr->elements().add(construct_tuple(sub_pattern));
            }
        }
        expr->add_annotation(match_instruction);
        return expr;
    };

    TupleExpression* lhs = construct_tuple(inst.self->pattern());
    return with_rr(with_il_instruction(make<AssignmentExpression>(lhs, rhs.expression), inst.self), make<ResolveResult>(known(KnownTypeCode::Void)));
}

TranslatedExpression ExpressionBuilder::visit_match_instruction(MatchInstruction& inst, TranslationContext) { return translate_match(MatchFacts::of(inst)); }

TranslatedExpression ExpressionBuilder::translate_match(const MatchFacts& inst) {
    TranslatedExpression left = translate(inst.tested_operand());
    // remove boxing conversion if possible, however, we still need a cast in
    // test case PatternMatching.GenericValueTypePatternStringRequiresCastToObject
    auto* crr = dynamic_cast<ConversionResolveResult*>(left.resolve_result);
    auto* cast_expr = as<CastExpression>(left.expression);
    if (crr != nullptr && crr->conversion->is_boxing_conversion() && cast_expr != nullptr &&
        (crr->input->type()->is_reference_type() != std::optional<bool>(false) || inst.variable()->type()->is_reference_type() == std::optional<bool>(false))) {
        left = left.unwrap_child(cast_expr->expression());
    }
    ExpressionWithILInstruction right = translate_pattern(inst.self, left.type());

    return with_rr(make<BinaryOperatorExpression>(left.expression, BinaryOperatorType::IsPattern, right.expression),
                   make<ResolveResult>(known(KnownTypeCode::Boolean)))
        .with_il_instruction(inst.self);
}

ExpressionWithILInstruction ExpressionBuilder::translate_pattern(ILInstruction* pattern, const IType* left_hand_type) {
    if (auto* match_instruction = il::as<MatchInstruction>(pattern)) {
        if (match_instruction->is_deconstruct_call) throw std::logic_error("not implemented");
        if (match_instruction->is_deconstruct_tuple) throw std::logic_error("not implemented");
        if (match_instruction->sub_patterns().count() > 0 || (match_instruction->check_not_null && !match_instruction->check_type)) {
            auto* recursive_pattern_expression = make<RecursivePatternExpression>();
            if (match_instruction->check_type) {
                recursive_pattern_expression->set_type(convert_type(match_instruction->variable()->type()));
            } else {
                assert(match_instruction->check_not_null || match_instruction->variable()->type()->is_reference_type() == std::optional<bool>(false));
            }
            for (ILInstruction* sub_pattern : match_instruction->sub_patterns()) {
                ILInstruction* tested_operand = nullptr;
                if (!MatchInstruction::is_pattern_match(sub_pattern, &tested_operand, settings)) {
                    assert(false && "Invalid sub pattern");
                    continue;
                }
                const IMember* member;
                ILInstruction* field_target = nullptr;
                if (auto* call = dynamic_cast<CallInstruction*>(tested_operand)) {
                    member = call->method->accessor_owner();
                } else if (const IField* f = nullptr; tested_operand->match_ld_fld(&field_target, &f)) {
                    member = f;
                } else {
                    assert(false && "Invalid sub pattern");
                    continue;
                }
                auto* named = make<NamedArgumentExpression>();
                named->set_name(member->name());
                named->set_expression(translate_pattern(sub_pattern, member->return_type()).expression);
                recursive_pattern_expression->sub_patterns().add(with_rr(named, make<MemberResolveResult>(nullptr, member)).expression);
            }
            if (match_instruction->has_designator()) {
                auto* designator = make<SingleVariableDesignation>();
                designator->set_identifier(*match_instruction->variable()->name);
                designator->add_annotation(make<ILVariableResolveResult>(match_instruction->variable()));
                recursive_pattern_expression->set_designation(designator);
            }
            return with_il_instruction(recursive_pattern_expression, match_instruction);
        } else if (match_instruction->has_designator() || !match_instruction->check_type) {
            auto* designator = make<SingleVariableDesignation>();
            designator->set_identifier(*match_instruction->variable()->name);
            designator->add_annotation(make<ILVariableResolveResult>(match_instruction->variable()));
            AstType* type;
            if (match_instruction->check_type) {
                type = convert_type(match_instruction->variable()->type());
            } else {
                assert(match_instruction->is_var());
                type = make<SimpleType>("var");
            }
            auto* declaration = make<DeclarationExpression>();
            declaration->set_type(type);
            declaration->set_designation(designator);
            return with_il_instruction(declaration, match_instruction);
        }
        assert(match_instruction->check_type);
        return with_il_instruction(make<TypeReferenceExpression>(convert_type(match_instruction->variable()->type())), match_instruction);
    }
    if (auto* comp = il::as<Comp>(pattern)) {
        TranslatedExpression constant_value = translate(comp->right(), left_hand_type);
        switch (comp->kind()) {
            case ComparisonKind::Equality: return ExpressionWithILInstruction(constant_value.with_il_instruction(comp).expression);
            case ComparisonKind::Inequality:
                return with_il_instruction(make<UnaryOperatorExpression>(UnaryOperatorType::PatternNot, constant_value.expression), comp);
            case ComparisonKind::LessThan:
                return with_il_instruction(make<UnaryOperatorExpression>(UnaryOperatorType::PatternRelationalLessThan, constant_value.expression), comp);
            case ComparisonKind::LessThanOrEqual:
                return with_il_instruction(make<UnaryOperatorExpression>(UnaryOperatorType::PatternRelationalLessThanOrEqual, constant_value.expression), comp);
            case ComparisonKind::GreaterThan:
                return with_il_instruction(make<UnaryOperatorExpression>(UnaryOperatorType::PatternRelationalGreaterThan, constant_value.expression), comp);
            case ComparisonKind::GreaterThanOrEqual:
                return with_il_instruction(make<UnaryOperatorExpression>(UnaryOperatorType::PatternRelationalGreaterThanOrEqual, constant_value.expression),
                                           comp);
            default: throw UsageError("Unexpected comparison kind");
        }
    }
    if (auto* call = il::as<Call>(pattern)) {
        if (MatchInstruction::is_call_to_op_equality(call, KnownTypeCode::String)) {
            return ExpressionWithILInstruction(translate(call->arguments()[1]).expression);
        }
        if (MatchInstruction::is_call_to_op_equality(call, KnownTypeCode::Decimal)) {
            return ExpressionWithILInstruction(translate(call->arguments()[1]).expression);
        }
    }
    throw std::logic_error("not implemented");
}

TranslatedExpression ExpressionBuilder::visit_invalid_branch(InvalidBranch& inst, TranslationContext) {
    std::string message = "Error";
    if (inst.start_il_offset() != 0) {
        message += std::format(" near IL_{:04x}", inst.start_il_offset());
    }
    if (!inst.message.empty()) {
        message += ": " + inst.message;
    }
    return error_expression(message);
}

TranslatedExpression ExpressionBuilder::visit_invalid_expression(InvalidExpression& inst, TranslationContext) {
    std::string message = inst.severity;
    if (inst.start_il_offset() != 0) {
        message += std::format(" near IL_{:04x}", inst.start_il_offset());
    }
    if (!inst.message.empty()) {
        message += ": " + inst.message;
    }
    return error_expression(message);
}

TranslatedExpression ExpressionBuilder::default_visit(ILInstruction& inst, TranslationContext) {
    return error_expression("OpCode not supported: " + std::string(il::opcode_name(inst.op_code)));
}

TranslatedExpression ExpressionBuilder::error_expression(const std::string& message) {
    auto* e = make<ErrorExpression>();
    e->add_trailing_trivia(make<Comment>(message, CommentType::MultiLine));
    return without_il_instruction(e).with_rr(ErrorResolveResult::unknown_error());
}

// ---- stage 14: the kinds read from L2 ----------------------------------------------------------------------------------------------
//
// Each is the visitor above it reads the ILAst with, reading the same facts from the instruction's L2 node (ILAST_TO_L2.md section 1):
//   Lit      ldc.i4 ldc.i8 (an integer of stack I4, I8), ldc.r4 ldc.r8 (a double of stack F4, F8; a NaN's and a negative zero's bits in
//            the text), ldc.decimal (the text of a System.Decimal), ldstr (a string; Utf8Literal: ldstr.utf8), ldnull (no value)
//   Default  default.value (the node's type)
//   SizeOf   sizeof (`of`)
//   Length   ldlen (the array; the node's stack kind: I4 Length, else LongLength)
//   Name     ldloc (its symbol: a stack slot is role Temp; its single definition from the tree's uses, ir::TreeIndex)
//   Unary    ldloca (`&` of a Name that is no instruction of its own; not RValueAddr), bit.not (`~`: the operand's stack kind in left_stk,
//            lifted unless lift is None)
// The instruction is what the ILAst annotation names (the C# transforms read it); the node is annotated beside it.
std::optional<TranslatedExpression> ExpressionBuilder::translate_from_l2(const translate::ir::Node& n, ILInstruction& inst, TranslationContext context) {
    namespace ir = translate::ir;
    namespace cil = translate::frontends::cil;
    const auto from = [&](TranslatedExpression e, Expression* annotated = nullptr) {
        (annotated != nullptr ? annotated : e.expression)->add_annotation(make<L2NodeAnnotation>(&n));
        return e;
    };
    // An operand: the instruction its node stands for, while it is still this instruction's operand (a rewrite of the ILAst the
    // builders make without telling the L2Body, as the deconstruction's replace_with, leaves the node naming an instruction that
    // moved: the ILAst then translates this one).
    const auto operand = [&](const ir::Node* child) -> ILInstruction* {
        ILInstruction* i = l2->instruction(child);
        return i != nullptr && i->parent() == &inst && inst.get_child(i->child_index()) == i ? i : nullptr;
    };
    const auto variable_of = [&](const ir::Node* name_node) -> ILVariable* {
        const auto* name = ir::get<ir::Name>(name_node);
        return name != nullptr && name->sym ? l2->variable(name->sym.get()) : nullptr;
    };

    if (const auto* lit = ir::get<ir::Lit>(&n)) {
        const auto bits = [&]() -> std::optional<std::uint64_t> {
            if (!lit->text || lit->text->size() < 3) return std::nullopt;
            return std::stoull(lit->text->substr(2), nullptr, 16);
        };
        switch (n.stack) {
            case ir::Stack::I4: {
                const auto value = static_cast<std::int32_t>(std::get<std::int64_t>(lit->value));
                ResolveResult* rr = get_sign(*context.type_hint) == Sign::Unsigned
                                        ? make<ConstantResolveResult>(known(KnownTypeCode::UInt32), ConstantValue(static_cast<std::uint32_t>(value)))
                                        : make<ConstantResolveResult>(known(KnownTypeCode::Int32), ConstantValue(value));
                rr = adjust_constant_to_type(rr, context.type_hint);
                return from(convert_constant_value(rr, true).with_il_instruction(&inst));
            }
            case ir::Stack::I8: {
                const std::int64_t value = std::get<std::int64_t>(lit->value);
                ResolveResult* rr = get_sign(*context.type_hint) == Sign::Unsigned
                                        ? make<ConstantResolveResult>(known(KnownTypeCode::UInt64), ConstantValue(static_cast<std::uint64_t>(value)))
                                        : make<ConstantResolveResult>(known(KnownTypeCode::Int64), ConstantValue(value));
                rr = adjust_constant_to_type(rr, context.type_hint);
                return from(convert_constant_value(rr, true).with_il_instruction(&inst));
            }
            case ir::Stack::F4: {
                const auto b = bits();
                const float value = b ? std::bit_cast<float>(static_cast<std::uint32_t>(*b)) : static_cast<float>(std::get<double>(lit->value));
                Expression* expr = ast_builder.convert_constant_value(known(KnownTypeCode::Single), ConstantValue(value));
                return from(TranslatedExpression(with_il_instruction(expr, &inst).expression));
            }
            case ir::Stack::F8: {
                const auto b = bits();
                const double value = b ? std::bit_cast<double>(*b) : std::get<double>(lit->value);
                Expression* expr = ast_builder.convert_constant_value(known(KnownTypeCode::Double), ConstantValue(value));
                return from(TranslatedExpression(with_il_instruction(expr, &inst).expression));
            }
            case ir::Stack::O: {
                if (std::holds_alternative<std::monostate>(lit->value))
                    return from(get_default_value_expression(&SpecialType::null_type()).with_il_instruction(&inst));
                const auto* text = std::get_if<std::string>(&lit->value);
                if (text == nullptr) return std::nullopt;
                if (n.type && n.type == translate::ir::decimal_type()) {
                    const std::optional<util::Decimal> d = cil::decimal_from(*text);
                    if (!d) return std::nullopt;
                    Expression* expr = ast_builder.convert_constant_value(known(KnownTypeCode::Decimal), ConstantValue(*d));
                    return from(TranslatedExpression(with_il_instruction(expr, &inst).expression));
                }
                if (ir::has_flag(n, ir::NodeFlag::Utf8Literal)) {
                    const IType* type = make_parameterized_type(known(KnownTypeCode::ReadOnlySpanOfT), {known(KnownTypeCode::Byte)});
                    return from(with_rr(with_il_instruction(make<PrimitiveExpression>(ConstantValue(to_u16(*text)), LiteralFormat::Utf8Literal), &inst),
                                        make<ConstantResolveResult>(type, ConstantValue(to_u16(*text)))));
                }
                return from(with_rr(with_il_instruction(make<PrimitiveExpression>(ConstantValue(to_u16(*text))), &inst),
                                    make<ConstantResolveResult>(known(KnownTypeCode::String), ConstantValue(to_u16(*text)))));
            }
            default: return std::nullopt;
        }
    }
    if (ir::is<ir::Default>(&n)) {
        const IType* type = cil::clr_type(n.type);
        if (type == nullptr) return std::nullopt;
        return from(get_default_value_expression(type).with_il_instruction(&inst));
    }
    if (const auto* size_of = ir::get<ir::SizeOf>(&n)) {
        const IType* type = cil::clr_type(size_of->of);
        if (type == nullptr) return std::nullopt;
        if (is_unmanaged_type(type, settings->introduce_unmanaged_constraint)) {
            return from(with_rr(with_il_instruction(make<SizeOfExpression>(convert_type(type)), &inst),
                                make<SizeOfResolveResult>(known(KnownTypeCode::Int32), type, std::nullopt)));
        }
        return from(call_unsafe_intrinsic("SizeOf", {}, known(KnownTypeCode::Int32), &inst, {type}));
    }
    if (const auto* length = ir::get<ir::Length>(&n)) {
        ILInstruction* array = operand(length->arr);
        if (array == nullptr) return std::nullopt;
        const IType* array_type = known(KnownTypeCode::Array);
        TranslatedExpression array_expr = translate(array, array_type);
        if (array_expr.type()->kind() != TypeKind::Array) array_expr = array_expr.convert_to(array_type, *this);
        array_expr = ensure_target_not_nullable(array_expr, array);
        const bool int32 = n.stack == ir::Stack::I4;
        const std::string member_name = int32 ? "Length" : "LongLength";
        const KnownTypeCode code = int32 ? KnownTypeCode::Int32 : KnownTypeCode::Int64;
        std::vector<const IProperty*> properties = array_type->get_properties([&](const IProperty* p) { return p->name() == member_name; });
        const IProperty* member = properties.empty() ? nullptr : properties.front();
        ResolveResult* rr = member == nullptr ? static_cast<ResolveResult*>(make<ResolveResult>(known(code)))
                                              : static_cast<ResolveResult*>(make<MemberResolveResult>(array_expr.resolve_result, member));
        return from(with_rr(with_il_instruction(make<MemberReferenceExpression>(array_expr.expression, member_name), &inst), rr));
    }
    if (const auto* name = ir::get<ir::Name>(&n)) {
        ILVariable* v = variable_of(&n);
        if (v == nullptr) return std::nullopt;
        if (name->sym->role == ir::VarRole::Temp && l2->index().is_single_definition(name->sym.get())) loaded_variables_set_.insert(v);
        return from(convert_variable(v).with_il_instruction(&inst));
    }
    if (const auto* unary = ir::get<ir::Unary>(&n)) {
        if (unary->op == "&" && !ir::has_flag(n, ir::NodeFlag::RValueAddr) && ir::is<ir::Name>(unary->x)) {
            ILVariable* v = variable_of(unary->x);
            if (v == nullptr) return std::nullopt;
            TranslatedExpression expr = convert_variable(v).with_il_instruction(&inst);
            // (the instruction, and the node beside it, on the IdentifierExpression: see visit_ld_loca)
            expr.expression->add_annotation(make<L2NodeAnnotation>(&n));
            return without_il_instruction(make<DirectionExpression>(FieldDirection::Ref, expr.expression))
                .with_rr(make<ByReferenceResolveResult>(expr.resolve_result, ReferenceKind::Ref));
        }
        if (unary->op == "~") {
            ILInstruction* argument_inst = operand(unary->x);
            if (argument_inst == nullptr) return std::nullopt;
            const auto underlying_result_type = static_cast<StackType>(unary->left_stk);
            const bool is_lifted = unary->lift != ir::Lift::None;
            TranslatedExpression argument = translate(argument_inst);
            const IType* arg_utype = nullable_type::get_underlying_type(argument.type());
            if (get_size(get_stack_type(*arg_utype)) < get_size(underlying_result_type) ||
                (arg_utype->kind() == TypeKind::Enum && is_small_integer_type(*arg_utype)) ||
                (get_stack_type(*arg_utype) == StackType::I && !is_csharp_native_integer_type(*arg_utype)) ||
                is_known_type(arg_utype, KnownTypeCode::Boolean) || is_known_type(arg_utype, KnownTypeCode::Char)) {
                // see visit_bit_not: an undersized argument, a small enum, IntPtr, bool and char are extended first
                Sign sign = get_sign(*context.type_hint);
                if (sign == Sign::None) sign = get_sign(*arg_utype);
                const IType* target_type = find_arithmetic_type(underlying_result_type, sign);
                if (is_lifted) target_type = nullable_type::create(*compilation, target_type);
                argument = argument.convert_to(target_type, *this);
            }
            ExpressionWithResolveResult result = with_rr(make<UnaryOperatorExpression>(UnaryOperatorType::BitNot, argument.expression),
                                                         resolver->resolve_unary_operator(UnaryOperatorType::BitNot, argument.resolve_result));
            return from(result.with_il_instruction(&inst));
        }
        if (unary->op == "*" && !ir::has_flag(n, ir::NodeFlag::IfRef)) {
            // ldobj (lift.cpp: a dereference; its type the type loaded)
            ILInstruction* target = operand(unary->x);
            const IType* type = cil::clr_type(n.type);
            if (target == nullptr || type == nullptr) return std::nullopt;
            return from(translate_ld_obj(LdObjFacts{&inst, target, type, cil::unaligned_of(n)}, context));
        }
        if (unary->op == "&" && ir::has_flag(n, ir::NodeFlag::RValueAddr)) {
            // addressof: the address of a value
            ILInstruction* value = operand(unary->x);
            const IType* type = cil::clr_type(n.type);
            if (value == nullptr || type == nullptr) return std::nullopt;
            return from(translate_address_of(AddressOfFacts{&inst, value, type}));
        }
        if (unary->op == "&") {
            if (const auto* select = ir::get<ir::Select>(unary->x)) {
                const IField* field = cil::clr_field(select->sym);
                if (field == nullptr) return std::nullopt;
                if (ir::is<ir::TypeName>(select->obj)) return from(translate_lds_flda(LdsFldaFacts{&inst, field}));   // ldsflda
                ILInstruction* target = operand(select->obj);
                if (target == nullptr) return std::nullopt;
                return from(translate_ld_flda(LdFldaFacts{&inst, target, field, static_cast<StackType>(n.stack)}));   // ldflda
            }
            if (const auto* index = ir::get<ir::Index>(unary->x); index != nullptr && ir::has_flag(n, ir::NodeFlag::InlineArray)) {
                // ldelema.inlinearray
                ILInstruction* array = operand(index->arr);
                const IType* type = cil::clr_type(index->elem);
                if (array == nullptr || type == nullptr) return std::nullopt;
                std::vector<ILInstruction*> indices;
                for (const ir::Node* i : ir::statements(index->index)) {
                    ILInstruction* x = operand(i);
                    if (x == nullptr) return std::nullopt;
                    indices.push_back(x);
                }
                return from(translate_ld_elema_inline_array(LdElemaInlineArrayFacts{&inst, array, std::move(indices), type}));
            }
            if (const auto* index = ir::get<ir::Index>(unary->x); index != nullptr && !ir::has_flag(n, ir::NodeFlag::InlineArray)) {
                // ldelema: the element's address
                ILInstruction* array = operand(index->arr);
                const IType* type = cil::clr_type(index->elem);
                if (array == nullptr || type == nullptr) return std::nullopt;
                std::vector<ILInstruction*> indices;
                for (const ir::Node* i : ir::statements(index->index)) {
                    ILInstruction* x = operand(i);
                    if (x == nullptr) return std::nullopt;
                    indices.push_back(x);
                }
                return from(translate_ld_elema(LdElemaFacts{&inst, array, std::move(indices), type, ir::has_flag(n, ir::NodeFlag::FromEnd)}));
            }
        }
    }
    if (const auto* address = ir::get<ir::AddressOf>(&n)) {
        // ldftn
        const IMethod* method = cil::clr_method(address->sym);
        if (method == nullptr) return std::nullopt;
        return from(translate_ld_ftn(MethodPointerFacts{&inst, method}));
    }
    if (const auto* ref = ir::get<ir::MethodRef>(&n); ref != nullptr && ref->mode == "virtual_ptr") {
        // ldvirtftn (its object is not translated)
        const IMethod* method = cil::clr_method(ref->sym);
        if (method == nullptr) return std::nullopt;
        return from(translate_ld_virt_ftn(MethodPointerFacts{&inst, method}));
    }
    if (const auto* awaited = ir::get<ir::Await>(&n)) {
        ILInstruction* value = operand(awaited->x);
        if (value == nullptr) return std::nullopt;
        return from(translate_await(AwaitFacts{&inst, value, cil::clr_method(awaited->awaiter), cil::clr_method(awaited->result)}));
    }
    if (const auto* unwrap = ir::get<ir::OptionalUnwrap>(&n)) {
        // nullable.unwrap (lift.cpp: its result type the node's stack slot)
        ILInstruction* argument = operand(unwrap->x);
        if (argument == nullptr) return std::nullopt;
        return from(translate_nullable_unwrap(NullableUnwrapFacts{&inst, argument, unwrap->ref_in, n.stack == ir::Stack::Ref}));
    }
    if (const auto* rewrap = ir::get<ir::OptionalChain>(&n)) {
        ILInstruction* argument = operand(rewrap->x);
        if (argument == nullptr) return std::nullopt;
        return from(translate_nullable_rewrap(OperandFacts{&inst, argument}));
    }
    if (const auto* thrown = ir::get<ir::ThrowExpr>(&n); thrown != nullptr && thrown->x != nullptr) {
        ILInstruction* argument = operand(thrown->x);
        if (argument == nullptr) return std::nullopt;
        return from(translate_throw(OperandFacts{&inst, argument}));
    }
    if (const auto* token = ir::get<ir::Intrinsic>(&n); token != nullptr && token->name == "cil.ldtoken_type" && !token->targs.empty()) {
        const IType* type = cil::clr_type(token->targs.front());
        if (type == nullptr) return std::nullopt;
        return from(translate_ld_type_token(TypeTokenFacts{&inst, type}));
    }
    if (const auto* cond = ir::get<ir::Cond>(&n); cond != nullptr && cond->orelse != nullptr) {
        // if as an expression
        auto* self = il::as<IfInstruction>(&inst);
        ILInstruction* condition = operand(cond->cond);
        ILInstruction* then = operand(cond->then);
        ILInstruction* orelse = operand(cond->orelse);
        if (self == nullptr || condition == nullptr || then == nullptr || orelse == nullptr) return std::nullopt;
        return from(translate_if(IfFacts{self, condition, then, orelse, static_cast<StackType>(n.stack)}, context));
    }
    if (ir::is<ir::SwitchExpr>(&n)) {
        // switch as an expression
        auto* self = il::as<il::SwitchInstruction>(&inst);
        std::optional<SwitchFacts> facts = self != nullptr ? l2->switch_facts(n, *self) : std::nullopt;
        if (!facts) return std::nullopt;
        return from(translate_switch_expression(*facts, context));
    }
    if (ir::is<ir::Empty>(&n)) {
        // a nop where a value is wanted (an argument slot the call builder fills with one): what the visitor's default gives
        return from(error_expression("OpCode not supported: " + std::string(il::opcode_name(OpCode::Nop))));
    }
    if (const auto* intrinsic = ir::get<ir::Intrinsic>(&n); intrinsic != nullptr && intrinsic->name.starts_with("cs.dynamic.")) {
        // the dynamic operations (lift.cpp: the binder flags imms[0], the operation imms[1], the context the last type argument where
        // flagged, the name strs[0], the operands and their argument infos in order)
        if (intrinsic->imms.empty()) return std::nullopt;
        DynamicFacts facts{&inst, static_cast<il::CSharpBinderFlags>(intrinsic->imms[0])};
        const bool has_context = ir::has_flag(n, ir::NodeFlag::HasContext);
        if (has_context && intrinsic->targs.empty()) return std::nullopt;
        facts.context = has_context ? cil::clr_type(intrinsic->targs.back()) : nullptr;
        const std::size_t own_targs = intrinsic->targs.size() - (has_context ? 1 : 0);
        for (const ir::Node* a : intrinsic->args) {
            ILInstruction* x = operand(a);
            if (x == nullptr) return std::nullopt;
            facts.arguments_.push_back(x);
        }
        for (const ir::ArgMeta& m : intrinsic->argmeta) {
            il::CSharpArgumentInfo info;
            info.name = m.name.value_or("");
            info.flags = static_cast<il::CSharpArgumentInfoFlags>(m.flags);
            info.compile_time_type = cil::clr_type(m.type);
            facts.argument_info_.push_back(std::move(info));
        }
        if (intrinsic->imms.size() > 1) facts.operation_ = static_cast<il::ExpressionType>(intrinsic->imms[1]);
        if (!intrinsic->strs.empty()) facts.name_ = intrinsic->strs.front();
        const std::string_view kind = std::string_view(intrinsic->name).substr(std::string_view("cs.dynamic.").size());
        const auto args = [&](std::size_t operands, std::size_t infos) { return facts.arguments_.size() == operands && facts.argument_info_.size() == infos; };
        const auto is = [&](OpCode op) { return inst.op_code == op; };
        if (kind == "compound_assign" && is(OpCode::DynamicCompoundAssign) && args(2, 2) && intrinsic->imms.size() > 2) {
            facts.context = nullptr;
            facts.target_kind = static_cast<il::CompoundTargetKind>(intrinsic->imms[2]);
            return from(translate_dynamic_compound_assign(facts));
        }
        if (kind == "binary" && is(OpCode::DynamicBinaryOperatorInstruction) && args(2, 2)) return from(translate_dynamic_binary_operator(facts, context));
        if (kind == "logic" && is(OpCode::DynamicLogicOperatorInstruction) && args(2, 2)) return from(translate_dynamic_logic_operator(facts, context));
        if (kind == "unary" && is(OpCode::DynamicUnaryOperatorInstruction) && args(1, 1)) return from(translate_dynamic_unary_operator(facts, context));
        if (kind == "convert" && is(OpCode::DynamicConvertInstruction) && args(1, 0) && own_targs >= 1) {
            facts.type_ = cil::clr_type(intrinsic->targs[0]);
            return from(translate_dynamic_convert(facts));
        }
        if (kind == "get_member" && is(OpCode::DynamicGetMemberInstruction) && args(1, 1)) return from(translate_dynamic_get_member(facts));
        if (kind == "set_member" && is(OpCode::DynamicSetMemberInstruction) && args(2, 2)) return from(translate_dynamic_set_member(facts));
        if (facts.arguments_.size() != facts.argument_info_.size()) return std::nullopt;
        if (kind == "get_index" && is(OpCode::DynamicGetIndexInstruction)) return from(translate_dynamic_get_index(facts));
        if (kind == "set_index" && is(OpCode::DynamicSetIndexInstruction)) return from(translate_dynamic_set_index(facts));
        if (kind == "invoke" && is(OpCode::DynamicInvokeInstruction)) return from(translate_dynamic_invoke(facts));
        if (kind == "invoke_member" && is(OpCode::DynamicInvokeMemberInstruction)) {
            for (std::size_t i = 0; i < own_targs; ++i) facts.type_arguments_.push_back(cil::clr_type(intrinsic->targs[i]));
            return from(translate_dynamic_invoke_member(facts));
        }
        if (kind == "invoke_constructor" && is(OpCode::DynamicInvokeConstructorInstruction)) {
            facts.type_ = cil::clr_type(n.type);
            return from(translate_dynamic_invoke_constructor(facts));
        }
        return std::nullopt;
    }
    if (const auto* alloc = ir::get<ir::StackAlloc>(&n)) {
        // localloc, localloc.span (lift.cpp: a span's type the element slot)
        ILInstruction* count = operand(alloc->count);
        if (count == nullptr) return std::nullopt;
        if (!alloc->span) return from(translate_loc_alloc_expression(LocAllocFacts{&inst, count, nullptr}, context));
        const IType* type = cil::clr_type(alloc->elem);
        if (type == nullptr) return std::nullopt;
        return from(translate_loc_alloc_span_expression(LocAllocFacts{&inst, count, type}, context));
    }
    if (const auto* ref = ir::get<ir::MethodRef>(&n); ref != nullptr && ref->mode == "virtual_delegate") {
        auto* self = il::as<il::LdVirtDelegate>(&inst);
        ILInstruction* target = operand(ref->target);
        const IMethod* method = cil::clr_method(ref->sym);
        if (self == nullptr || target == nullptr || method == nullptr) return std::nullopt;
        return from(CallBuilder(*this, *type_system_, *settings).build(LdVirtDelegateFacts{self, target, cil::clr_type(ref->of), method}));
    }
    if (const auto* destructure = ir::get<ir::Destructure>(&n)) {
        // deconstruct: the value and its type the pattern's (its tested operand and variable)
        auto* self = il::as<il::DeconstructInstruction>(&inst);
        const auto* match = ir::get<ir::PatternMatch>(destructure->pattern);
        il::ILInstruction* pattern = self != nullptr ? l2->instruction(destructure->pattern) : nullptr;
        ILInstruction* tested = match != nullptr && pattern != nullptr ? l2->operand(match->tested, *pattern) : nullptr;
        const IType* type = match != nullptr && match->var ? cil::clr_type(match->var->type) : nullptr;
        if (self == nullptr || pattern == nullptr || pattern->parent() != self || tested == nullptr || type == nullptr) return std::nullopt;
        return from(translate_deconstruct(DeconstructFacts{self, tested, type}));
    }
    if (const auto* match = ir::get<ir::PatternMatch>(&n)) {
        // match (lift.cpp: the variable the pattern's; the value tested)
        auto* self = il::as<il::MatchInstruction>(&inst);
        ILInstruction* tested = operand(match->tested);
        ILVariable* v = match->var ? l2->variable(match->var.get()) : nullptr;
        if (self == nullptr || tested == nullptr || v == nullptr) return std::nullopt;
        return from(translate_match(MatchFacts{self, tested, v}));
    }
    if (const auto* block = ir::get<ir::BlockExpr>(&n); block != nullptr && block->role) {
        // a block of another kind than control flow: an initializer, a named-argument call, an interpolated string (its role)
        const auto kind = cil::block_kind_from(block->role);
        auto* self = il::as<Block>(&inst);
        if (!kind || self == nullptr) return std::nullopt;
        return from(translate_block(*self, *kind, context));
    }
    if (ir::is<ir::Lambda>(&n)) {
        // a lambda, an anonymous method or an expression tree (lift.cpp: the delegate type the node's type)
        auto* function = il::as<ILFunction>(&inst);
        if (function == nullptr) return std::nullopt;
        return from(translate_function(cil::clr_type(n.type), function).with_il_instruction(&inst));
    }
    const auto call_facts = [&](il::OpCode op_code, const ir::SymP& sym, const ir::Node* target, const std::vector<ir::Node*>& args) -> std::optional<CallFacts> {
        // call, callvirt, newobj (lift.cpp: the method the symbol's; the `this` argument the target; the prefixes flags and cil.constrained)
        const IMethod* method = cil::clr_method(sym);
        if (method == nullptr || inst.op_code != op_code) return std::nullopt;
        CallFacts facts{static_cast<il::CallInstruction*>(&inst), op_code, method, {}, nullptr, ir::has_flag(n, ir::NodeFlag::Tail)};
        facts.arguments.reserve(args.size() + 1);
        if (target != nullptr) {
            ILInstruction* t = operand(target);
            if (t == nullptr) return std::nullopt;
            facts.arguments.push_back(t);
        }
        for (const ir::Node* a : args) {
            ILInstruction* x = operand(a);
            if (x == nullptr) return std::nullopt;
            facts.arguments.push_back(x);
        }
        if (const auto* t = n.info.get_as<ir::TypeP>("cil.constrained")) facts.constrained_to = cil::clr_type(*t);
        return facts;
    };
    if (const auto* call = ir::get<ir::Call>(&n); call != nullptr && call->form == ir::CallForm::Indirect) {
        // calli (lift.cpp: the target the function pointer; the signature's type cil.calli_sig, whether it takes `this` its FnType's)
        const auto* sig = n.info.get_as<ir::TypeP>("cil.calli_sig");
        const auto* ptr = sig != nullptr && *sig ? std::get_if<ir::FnPtrType>(&(*sig)->body) : nullptr;
        const auto* fn = ptr != nullptr && ptr->fn ? std::get_if<ir::FnType>(&ptr->fn->body) : nullptr;
        ILInstruction* pointer = operand(call->target);
        if (fn == nullptr || pointer == nullptr) return std::nullopt;
        CallIndirectFacts facts{&inst, pointer, dynamic_cast<const FunctionPointerType*>(cil::clr_type(*sig)), fn->has_this, {}};
        facts.arguments.reserve(call->args.size());
        for (const ir::Node* a : call->args) {
            ILInstruction* x = operand(a);
            if (x == nullptr) return std::nullopt;
            facts.arguments.push_back(x);
        }
        return from(translate_call_indirect(facts));
    }
    if (const auto* call = ir::get<ir::Call>(&n); call != nullptr && call->form != ir::CallForm::Indirect && !ir::has_flag(n, ir::NodeFlag::PinnableRef)) {
        const auto facts = call_facts(call->form == ir::CallForm::Virtual ? OpCode::CallVirt : OpCode::Call, call->sym, call->target, call->args);
        if (!facts) return std::nullopt;
        return from(translate_call(*facts, context));
    }
    if (const auto* new_object = ir::get<ir::New>(&n)) {
        const auto facts = call_facts(OpCode::NewObj, new_object->sym, nullptr, new_object->args);
        if (!facts) return std::nullopt;
        return from(translate_new_obj(*facts, context));
    }
    if (const auto* new_array = ir::get<ir::NewArray>(&n); new_array != nullptr && new_array->init.empty()) {
        // newarr: the element type, the lengths
        const IType* type = cil::clr_type(new_array->elem);
        if (type == nullptr) return std::nullopt;
        std::vector<ILInstruction*> dims;
        for (const ir::Node* d : new_array->dims) {
            ILInstruction* x = operand(d);
            if (x == nullptr) return std::nullopt;
            dims.push_back(x);
        }
        return from(translate_new_arr(NewArrFacts{&inst, type, std::move(dims)}));
    }
    if (const auto* assign = ir::get<ir::Assign>(&n)) {
        if (!assign->op && !assign->eval && !assign->sym && ir::is<ir::Name>(assign->target)) {
            // stloc (lift.cpp: an Assign to a Name)
            ILVariable* v = variable_of(assign->target);
            ILInstruction* value = operand(assign->value);
            if (v == nullptr || value == nullptr) return std::nullopt;
            return from(translate_st_loc(StLocFacts{&inst, v, value}));
        }
        if (assign->eval) {
            // a compound assignment (lift.cpp: one with an eval mode; a user-defined one names its operator)
            ILInstruction* target = operand(assign->target);
            ILInstruction* value = operand(assign->value);
            if (target == nullptr || value == nullptr) return std::nullopt;
            const auto eval = static_cast<il::CompoundEvalMode>(*assign->eval);
            const auto via = static_cast<il::CompoundTargetKind>(assign->via.value_or(ir::Via::Address));
            if (assign->sym) {
                const IMethod* method = cil::clr_method(assign->sym);
                if (method == nullptr) return std::nullopt;
                return from(translate_user_defined_compound_assign(UserDefinedCompoundAssignFacts{&inst, method, eval, via, target, value}));
            }
            const auto op = assign->op ? cil::binary_op_from(*assign->op) : std::nullopt;
            const IType* type = cil::clr_type(n.type);
            if (!op || type == nullptr) return std::nullopt;
            const auto left_type = static_cast<StackType>(assign->left_stk);
            const auto right_type = static_cast<StackType>(assign->right_stk);
            return from(translate_numeric_compound_assign(NumericCompoundAssignFacts{
                &inst, *op, cil::sign_from(assign->is_signed), assign->overflow == ir::Overflow::Trap, left_type, right_type, assign->lift == ir::Lift::CSharp,
                il::BinaryNumericInstruction::compute_result_type(*op, left_type, right_type), type, eval, via, target, value}));
        }
        const auto* deref = ir::get<ir::Unary>(assign->target);
        if (!assign->op && !assign->eval && !assign->sym && deref != nullptr && deref->op == "*") {
            // stobj (lift.cpp: an Assign to a dereference; the type and the prefixes the dereference's)
            ILInstruction* target = operand(deref->x);
            ILInstruction* value = operand(assign->value);
            const IType* type = cil::clr_type(assign->target->type);
            if (target == nullptr || value == nullptr || type == nullptr) return std::nullopt;
            return from(translate_st_obj(StObjFacts{&inst, target, value, type, cil::unaligned_of(*assign->target)}));
        }
        return std::nullopt;
    }
    if (const auto* cast = ir::get<ir::Cast>(&n)) {
        const ir::ConvKind conv = cast->conv.value_or(ir::ConvKind::Invalid);
        const auto typed = [&](TranslatedExpression (ExpressionBuilder::*translate_kind)(const TypedUnaryFacts&)) -> std::optional<TranslatedExpression> {
            // box, unbox, unbox.any, castclass, isinst (lift.cpp: the conversion kind; the type the node's)
            ILInstruction* argument = operand(cast->x);
            const IType* type = cil::clr_type(n.type);
            if (argument == nullptr || type == nullptr) return std::nullopt;
            return from((this->*translate_kind)(TypedUnaryFacts{&inst, argument, type}));
        };
        switch (conv) {
            case ir::ConvKind::Box: return typed(&ExpressionBuilder::translate_box);
            case ir::ConvKind::Unbox: return typed(&ExpressionBuilder::translate_unbox);
            case ir::ConvKind::UnboxAny: return typed(&ExpressionBuilder::translate_unbox_any);
            case ir::ConvKind::RefCast: return typed(&ExpressionBuilder::translate_cast_class);
            case ir::ConvKind::TryCast: return typed(&ExpressionBuilder::translate_is_inst);
            case ir::ConvKind::ExprTree: {
                ILInstruction* argument = operand(cast->x);
                const IType* type = cil::clr_type(n.type);
                if (argument == nullptr || type == nullptr) return std::nullopt;
                return from(translate_expression_tree_cast(ExpressionTreeCastFacts{&inst, argument, type, cast->overflow == ir::Overflow::Trap}));
            }
            default: break;
        }
        // conv (lift.cpp: the target from the node's type, the rest from the Cast's slots)
        ILInstruction* argument = operand(cast->x);
        const auto target = n.type ? cil::conv_target_from(*n.type, n.info.get_as<bool>("cil.target_r") != nullptr) : std::nullopt;
        if (argument == nullptr || !target) return std::nullopt;
        return from(translate_conv(ConvFacts{&inst, argument, static_cast<StackType>(cast->from_stk), cil::sign_from(cast->from_signed), *target,
                                             static_cast<ConversionKind>(conv), cast->overflow == ir::Overflow::Trap, cast->lift == ir::Lift::CSharp},
                                   context));
    }
    if (const auto* binary = ir::get<ir::Binary>(&n)) {
        ILInstruction* left = operand(binary->left);
        ILInstruction* right = operand(binary->right);
        if (left == nullptr || right == nullptr) return std::nullopt;
        if (binary->sym) {
            // a user-defined && or || (lift.cpp: the operator method the symbol)
            const IMethod* method = cil::clr_method(binary->sym);
            if (method == nullptr) return std::nullopt;
            return from(translate_user_defined_logic_operator(LogicOperatorFacts{&inst, method, left, right}));
        }
        if (binary->lift == ir::Lift::ThreeValued && (binary->op == "&&" || binary->op == "||")) {
            const bool both = binary->op == "&&";
            return from(handle_three_valued_logic(LogicOperatorFacts{&inst, nullptr, left, right}, both ? BinaryOperatorType::BitwiseAnd : BinaryOperatorType::BitwiseOr,
                                                  both ? ExpressionType::And : ExpressionType::Or));
        }
        if (binary->op == "??") {
            // a ?? b (lift.cpp: the kind of coalescing; the underlying result type the left operand's stack slot)
            return from(translate_null_coalescing(NullCoalescingFacts{&inst, left, right,
                                                                      static_cast<il::NullCoalescingKind>(binary->coalesce.value_or(ir::Coalesce::Ref)),
                                                                      static_cast<StackType>(binary->left_stk)}));
        }
        const auto left_type = static_cast<StackType>(binary->left_stk);
        const auto right_type = static_cast<StackType>(binary->right_stk);
        if (const auto kind = cil::comparison_from(binary->op)) {
            // comp (lift.cpp: the comparison operator; its input type the left operand's)
            return from(translate_comp_facts(CompFacts{&inst, *kind, static_cast<il::ComparisonLiftingKind>(binary->lift), left_type,
                                                       cil::sign_from(binary->is_signed), left, right}));
        }
        if (const auto op = cil::binary_op_from(binary->op)) {
            // binary.<op>
            return from(translate_binary_numeric(BinaryNumericFacts{&inst, *op, left, right, left_type, right_type, binary->overflow == ir::Overflow::Trap,
                                                                    cil::sign_from(binary->is_signed), binary->lift == ir::Lift::CSharp,
                                                                    il::BinaryNumericInstruction::compute_result_type(*op, left_type, right_type)},
                                                 context));
        }
        return std::nullopt;
    }
    return std::nullopt;
}

}  // namespace cb::dotnet::decompiler::csharp
