// dotnet/decompiler/csharp/translated_expression.cpp -- see translated_expression.hpp.
#include "dotnet/decompiler/csharp/translated_expression.hpp"

#include <cassert>
#include <optional>
#include <stdexcept>
#include <string>
#include <variant>

#include "dotnet/decompiler/csharp/annotations.hpp"
#include "dotnet/decompiler/csharp/expression_builder.hpp"
#include "dotnet/decompiler/csharp/resolver/csharp_conversions.hpp"
#include "dotnet/decompiler/csharp/resolver/csharp_resolver.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax_extensions.hpp"
#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/il/il_type_extensions.hpp"
#include "dotnet/decompiler/il/instructions.hpp"
#include "dotnet/decompiler/il/pointer_arithmetic_offset.hpp"
#include "dotnet/decompiler/semantics/by_reference_resolve_result.hpp"
#include "dotnet/decompiler/semantics/constant_resolve_result.hpp"
#include "dotnet/decompiler/semantics/conversion_resolve_result.hpp"
#include "dotnet/decompiler/semantics/error_resolve_result.hpp"
#include "dotnet/decompiler/semantics/invocation_resolve_result.hpp"
#include "dotnet/decompiler/semantics/operator_resolve_result.hpp"
#include "dotnet/decompiler/semantics/tuple_resolve_result.hpp"
#include "dotnet/decompiler/typesystem/tuple_type.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/typesystem/type_utils.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"
#include "dotnet/decompiler/util/csharp_primitive_cast.hpp"

#include "dotnet/decompiler/csharp/transforms/add_checked_blocks.hpp"

namespace cb::dotnet::decompiler::csharp {

using semantics::ByReferenceResolveResult;
using semantics::ConstantResolveResult;
using semantics::ConversionResolveResult;
using semantics::ErrorResolveResult;
using semantics::InvocationResolveResult;
using semantics::OperatorResolveResult;
using semantics::TupleResolveResult;
using syntax::BaseReferenceExpression;
using syntax::BinaryOperatorExpression;
using syntax::BinaryOperatorType;
using syntax::CastExpression;
using syntax::ConditionalExpression;
using syntax::DirectionExpression;
using syntax::FieldDirection;
using syntax::NamedArgumentExpression;
using syntax::NullReferenceExpression;
using syntax::ObjectCreateExpression;
using syntax::PrimitiveExpression;
using syntax::ThisReferenceExpression;
using syntax::ThrowExpression;
using syntax::TupleExpression;
using syntax::UnaryOperatorExpression;
using syntax::UnaryOperatorType;
using typesystem::KnownTypeCode;
using typesystem::SpecialType;
using typesystem::TypeKind;
using typesystem::is_any_pointer;
using typesystem::is_known_type;

namespace {

// The annotation AddCheckedBlocks reads from a cast of an integer.
enum class CheckKind { Checked, Unchecked, ExplicitUnchecked };

void add_check_annotation(Expression* cast, CheckKind kind) {
    switch (kind) {
        case CheckKind::Checked: cast->add_annotation(transforms::AddCheckedBlocks::checked_annotation()); break;
        case CheckKind::Unchecked: cast->add_annotation(transforms::AddCheckedBlocks::unchecked_annotation()); break;
        case CheckKind::ExplicitUnchecked: cast->add_annotation(transforms::AddCheckedBlocks::explicit_unchecked_annotation()); break;
    }
}

const typesystem::ByReferenceType* static_cast_by_ref(const IType* type) { return dynamic_cast<const typesystem::ByReferenceType*>(type); }

bool is_int_constant(const ConstantValue& value, int expected) {
    if (auto* i = std::get_if<std::int32_t>(&value)) return *i == expected;
    return false;
}

}  // namespace

// ---- construction --------------------------------------------------------------------------------------------------------------

TranslatedExpression::TranslatedExpression(Expression* expr) : expression(expr) {
    assert(expr != nullptr);
    resolve_result = expr->annotation<ResolveResult>();
    if (resolve_result == nullptr) resolve_result = ErrorResolveResult::unknown_error();
}

TranslatedExpression::TranslatedExpression(Expression* expr, ResolveResult* rr) : expression(expr), resolve_result(rr) {
    assert(expr != nullptr && rr != nullptr);
    assert(expr->annotation<ResolveResult>() == rr);
}

TranslatedExpression TranslatedExpression::unwrap_child(Expression* descendant) const {
    if (descendant == expression) return *this;
    for (syntax::AstNode* parent = descendant->parent(); parent != nullptr; parent = parent->parent()) {
        for (ILInstruction* inst : il_instructions_of(parent)) descendant->add_annotation(inst);
        if (parent == expression) return TranslatedExpression(syntax::detach(descendant));
    }
    throw std::invalid_argument("descendant must be a descendant of the current node");
}

// ---- convert_to -----------------------------------------------------------------------------------------------------------------

TranslatedExpression TranslatedExpression::convert_to(const IType* target_type, ExpressionBuilder& expression_builder, bool check_for_overflow,
                                                      bool allow_implicit_conversion) const {
    // The pieces of the ExpressionBuilder this function reads.
    const ICompilation& compilation = *expression_builder.compilation;
    const DecompilerSettings& settings = *expression_builder.settings;
    resolver::CSharpConversions& conversions = resolver::CSharpConversions::get(compilation);

    const IType* source_type = this->type();
    if (typesystem::NormalizeTypeVisitor::ignore_nullability_and_tuples().equivalent_types(source_type, target_type)) {
        // Make explicit conversion implicit, if possible
        if (allow_implicit_conversion) {
            if (auto* conversion = dynamic_cast<ConversionResolveResult*>(resolve_result)) {
                auto* cast = syntax::as<CastExpression>(expression);
                auto* oce = syntax::as<ObjectCreateExpression>(expression);
                if (cast != nullptr && cast_can_be_made_implicit(compilation, conversion->conversion, conversion->input->type(), source_type, target_type)) {
                    TranslatedExpression result = this->unwrap_child(cast->expression());
                    if (conversion->conversion->is_user_defined()) {
                        result.expression->add_annotation(make<ImplicitConversionAnnotation>(conversion));
                    }
                    return result;
                } else if (oce != nullptr && conversion->conversion->is_method_group_conversion() && oce->arguments().count() == 1 &&
                           settings.use_implicit_method_group_conversion) {
                    return this->unwrap_child(oce->arguments()[0]);
                }
            } else if (auto* invocation = dynamic_cast<InvocationResolveResult*>(resolve_result)) {
                auto* oce = syntax::as<ObjectCreateExpression>(expression);
                if (oce != nullptr && oce->arguments().count() == 1 && is_known_type(invocation->type(), KnownTypeCode::NullableOfT)) {
                    return this->unwrap_child(oce->arguments()[0]);
                }
            }
        }
        return *this;
    }
    if (target_type->kind() == TypeKind::Void || target_type->kind() == TypeKind::None) {
        return *this;  // don't attempt to insert cast to '?' or 'void' as these are not valid.
    } else if (target_type->kind() == TypeKind::Unknown) {
        // don't attempt cast to '?', or casts between an unknown type and a known type with same name
        if (target_type->name() == "?" || target_type->reflection_name() == source_type->reflection_name()) {
            return *this;
        }
        // However we still want explicit casts to types that are merely unresolved
    }
    if (auto* conv_annotation = expression->annotation<ImplicitConversionAnnotation>()) {
        // If an implicit user-defined conversion was stripped from this expression; it needs to be re-introduced before we can apply
        // other casts to this expression. This happens when the CallBuilder discovers that the conversion is necessary in order to
        // choose the correct overload.
        expression->remove_annotations<ImplicitConversionAnnotation>();
        syntax::AstType* cast_type = expression_builder.convert_type(conv_annotation->target_type());
        return without_il_instruction(make<CastExpression>(cast_type, expression))
            .with_rr(conv_annotation->conversion_resolve_result)
            .convert_to(target_type, expression_builder, check_for_overflow, allow_implicit_conversion);
    }
    if (syntax::is<ThrowExpression>(expression) && allow_implicit_conversion) {
        return *this;  // Throw expressions have no type and are implicitly convertible to any type
    }
    if (auto* tuple_expr = syntax::as<TupleExpression>(expression)) {
        auto* target_tuple_type = dynamic_cast<const typesystem::TupleType*>(target_type);
        if (target_tuple_type != nullptr && tuple_expr->elements().count() == static_cast<int>(target_tuple_type->element_types().size())) {
            // Conversion of a tuple literal: convert element-wise
            auto* new_tuple_expr = make<TupleExpression>();
            std::vector<ResolveResult*> new_element_rrs;
            // element names: discard existing names and use target_tuple_type instead
            const auto& new_element_names = target_tuple_type->element_names();
            std::vector<Expression*> elements = tuple_expr->elements().to_vector();
            for (std::size_t index = 0; index < elements.size(); index++) {
                Expression* element_expr = elements[index];
                const IType* element_target_type = target_tuple_type->element_types()[index];
                auto* nae = syntax::as<NamedArgumentExpression>(element_expr);
                Expression* inner = syntax::detach(nae != nullptr ? nae->expression() : element_expr);
                TranslatedExpression new_element_expr =
                    TranslatedExpression(inner).convert_to(element_target_type, expression_builder, check_for_overflow, allow_implicit_conversion);
                if (new_element_names.empty() || index >= new_element_names.size() || !new_element_names[index].has_value() ||
                    new_element_names[index]->empty()) {
                    new_tuple_expr->elements().add(new_element_expr.expression);
                } else {
                    new_tuple_expr->elements().add(make<NamedArgumentExpression>(*new_element_names[index], new_element_expr.expression));
                }
                new_element_rrs.push_back(new_element_expr.resolve_result);
            }
            const typesystem::ITypeDefinition* tuple_definition = target_tuple_type->get_definition();
            return csharp::with_il_instruction(new_tuple_expr, this->il_instructions())
                .with_rr(make<TupleResolveResult>(compilation, new_element_rrs, std::vector<std::optional<std::string>>{},
                                                  tuple_definition != nullptr ? tuple_definition->parent_module() : nullptr));
        }
    }
    if (auto* conv = dynamic_cast<ConversionResolveResult*>(resolve_result)) {
        auto* cast2 = syntax::as<CastExpression>(expression);
        if (cast2 != nullptr && !conv->conversion->is_user_defined() &&
            cast_can_be_made_implicit(compilation, conv->conversion, conv->input->type(), source_type, target_type)) {
            TranslatedExpression unwrapped = this->unwrap_child(cast2->expression());
            if (allow_implicit_conversion) return unwrapped;
            return unwrapped.convert_to(target_type, expression_builder, check_for_overflow, allow_implicit_conversion);
        }
    }
    if (auto* uoe = syntax::as<UnaryOperatorExpression>(expression);
        uoe != nullptr && uoe->operator_() == UnaryOperatorType::NullConditional && target_type->is_reference_type() == true) {
        // "(T)(x?).AccessChain" is invalid, but "((T)x)?.AccessChain" is valid and equivalent
        TranslatedExpression inner = unwrap_child(uoe->expression()).convert_to(target_type, expression_builder, check_for_overflow, allow_implicit_conversion);
        return with_rr(make<UnaryOperatorExpression>(UnaryOperatorType::NullConditional, inner.expression), make<ResolveResult>(target_type))
            .without_il_instruction();
    }
    const IType* utype = typesystem::nullable_type::get_underlying_type(source_type);
    const IType* target_utype = typesystem::nullable_type::get_underlying_type(target_type);
    if (is_known_type(source_type, KnownTypeCode::Boolean) && !is_known_type(target_utype, KnownTypeCode::Boolean) &&
        il::is_integer_type(il::get_stack_type(*target_utype))) {
        // convert from boolean to integer (or enum)
        TranslatedExpression one = ldc_i4(compilation, 1).convert_to(target_type, expression_builder, check_for_overflow);
        TranslatedExpression zero = ldc_i4(compilation, 0).convert_to(target_type, expression_builder, check_for_overflow);
        return without_il_instruction(make<ConditionalExpression>(this->expression, one.expression, zero.expression))
            .with_rr(make<ResolveResult>(target_type));
    }
    if (is_known_type(target_type, KnownTypeCode::Boolean)) {
        // convert to boolean through byte, to simulate the truncation to 8 bits
        return this->convert_to(compilation.find_type(KnownTypeCode::Byte), expression_builder, check_for_overflow)
            .convert_to_boolean(expression_builder);
    }

    // Special-case IntPtr and UIntPtr: they behave extremely weird, see IntPtr.txt for details.
    if (is_known_type(source_type, KnownTypeCode::IntPtr)) {
        // Conversion from IntPtr
        // Direct cast only works correctly for IntPtr -> long.
        // IntPtr -> int works correctly only in checked context.
        // Everything else can be worked around by casting via long.
        if (!(is_known_type(target_type, KnownTypeCode::Int64) || target_type->kind() == TypeKind::NInt ||
              (check_for_overflow && is_known_type(target_type, KnownTypeCode::Int32)) || is_any_pointer(target_type->kind()) ||
              target_type->kind() == TypeKind::ByReference)) {
            const IType* convert_via = settings.native_integers ? &SpecialType::nint() : compilation.find_type(KnownTypeCode::Int64);
            return this->convert_to(convert_via, expression_builder, check_for_overflow)
                .convert_to(target_type, expression_builder, check_for_overflow, allow_implicit_conversion);
        }
    } else if (is_known_type(source_type, KnownTypeCode::UIntPtr)) {
        // Conversion from UIntPtr
        // Direct cast only works correctly for UIntPtr -> ulong.
        // UIntPtr -> uint works correctly only in checked context.
        // Everything else can be worked around by casting via ulong.
        if (!(is_known_type(target_type, KnownTypeCode::UInt64) || target_type->kind() == TypeKind::NUInt ||
              (check_for_overflow && is_known_type(target_type, KnownTypeCode::UInt32)) || is_any_pointer(target_type->kind()) ||
              target_type->kind() == TypeKind::ByReference)) {
            const IType* convert_via = settings.native_integers ? &SpecialType::nuint() : compilation.find_type(KnownTypeCode::UInt64);
            return this->convert_to(convert_via, expression_builder, check_for_overflow)
                .convert_to(target_type, expression_builder, check_for_overflow, allow_implicit_conversion);
        }
    }
    if (is_known_type(target_utype, KnownTypeCode::IntPtr) && il::is_integer_type(il::get_stack_type(*utype))) {
        // Conversion to IntPtr
        if (is_known_type(source_type, KnownTypeCode::Int32) || source_type->kind() == TypeKind::NInt) {
            // normal casts work for int/nint (both in checked and unchecked context)
            // note that pointers only allow normal casts in unchecked contexts
        } else if (settings.native_integers) {
            // if native integer types are available, prefer using those
            return this->convert_to(&SpecialType::nint(), expression_builder, check_for_overflow)
                .convert_to(target_type, expression_builder, check_for_overflow, allow_implicit_conversion);
        } else if (check_for_overflow) {
            // if overflow-checking is enabled, we can simply cast via long:
            // (and long itself works directly in checked context)
            if (!is_known_type(source_type, KnownTypeCode::Int64)) {
                return this->convert_to(compilation.find_type(KnownTypeCode::Int64), expression_builder, check_for_overflow)
                    .convert_to(target_type, expression_builder, check_for_overflow);
            }
        } else if (!is_any_pointer(source_type->kind())) {
            // If overflow-checking is disabled, the only way to truncate to native size without throwing an exception in 32-bit mode
            // is to use a pointer type.
            return this->convert_to(typesystem::make_pointer_type(compilation.find_type(KnownTypeCode::Void)), expression_builder, check_for_overflow)
                .convert_to(target_type, expression_builder, check_for_overflow);
        }
    } else if (is_known_type(target_utype, KnownTypeCode::UIntPtr) && il::is_integer_type(il::get_stack_type(*utype))) {
        // Conversion to UIntPtr
        if (is_known_type(source_type, KnownTypeCode::UInt32) || is_any_pointer(source_type->kind()) || source_type->kind() == TypeKind::NUInt) {
            // normal casts work for uint/nuint and pointers (both in checked and unchecked context)
        } else if (settings.native_integers) {
            // if native integer types are available, prefer using those
            return this->convert_to(&SpecialType::nuint(), expression_builder, check_for_overflow)
                .convert_to(target_type, expression_builder, check_for_overflow, allow_implicit_conversion);
        } else if (check_for_overflow) {
            // if overflow-checking is enabled, we can simply cast via ulong:
            // (and ulong itself works directly in checked context)
            if (!is_known_type(source_type, KnownTypeCode::UInt64)) {
                return this->convert_to(compilation.find_type(KnownTypeCode::UInt64), expression_builder, check_for_overflow)
                    .convert_to(target_type, expression_builder, check_for_overflow);
            }
        } else {
            // If overflow-checking is disabled, the only way to truncate to native size without throwing an exception in 32-bit mode
            // is to use a pointer type.
            return this->convert_to(typesystem::make_pointer_type(compilation.find_type(KnownTypeCode::Void)), expression_builder, check_for_overflow)
                .convert_to(target_type, expression_builder, check_for_overflow);
        }
    }

    if (is_any_pointer(target_type->kind()) && source_type->kind() == TypeKind::Enum) {
        // enum to pointer: C# doesn't allow such casts
        // -> convert via underlying type
        return this->convert_to(il::get_enum_underlying_type(*source_type), expression_builder, check_for_overflow)
            .convert_to(target_type, expression_builder, check_for_overflow);
    } else if (target_utype->kind() == TypeKind::Enum && is_any_pointer(source_type->kind())) {
        // pointer to enum: C# doesn't allow such casts
        // -> convert via underlying type
        return this->convert_to(il::get_enum_underlying_type(*target_utype), expression_builder, check_for_overflow)
            .convert_to(target_type, expression_builder, check_for_overflow);
    }
    if ((is_any_pointer(target_type->kind()) && is_known_type(source_type, KnownTypeCode::Char)) ||
        (is_known_type(target_utype, KnownTypeCode::Char) && is_any_pointer(source_type->kind()))) {
        // char <-> pointer: C# doesn't allow such casts
        // -> convert via ushort
        return this->convert_to(compilation.find_type(KnownTypeCode::UInt16), expression_builder, check_for_overflow)
            .convert_to(target_type, expression_builder, check_for_overflow);
    }
    if (target_type->kind() == TypeKind::Pointer && source_type->kind() == TypeKind::ByReference && syntax::is<DirectionExpression>(expression)) {
        // convert from reference to pointer
        Expression* arg = syntax::detach(static_cast<DirectionExpression*>(expression)->expression());
        const IType* pointer_type =
            typesystem::make_pointer_type(static_cast_by_ref(source_type)->element_type());
        auto* arg_uoe = syntax::as<UnaryOperatorExpression>(arg);
        if (arg_uoe != nullptr && arg_uoe->operator_() == UnaryOperatorType::Dereference) {
            // &*ptr -> ptr
            return TranslatedExpression(arg_uoe).unwrap_child(arg_uoe->expression()).convert_to(target_type, expression_builder);
        }
        TranslatedExpression pointer_expr =
            csharp::with_il_instruction(make<UnaryOperatorExpression>(UnaryOperatorType::AddressOf, arg), this->il_instructions())
                .with_rr(make<ResolveResult>(pointer_type));
        // perform remaining pointer cast, if necessary
        return pointer_expr.convert_to(target_type, expression_builder);
    }
    Expression* expr = nullptr;
    if (target_type->kind() == TypeKind::ByReference) {
        if (typesystem::NormalizeTypeVisitor::type_erasure().equivalent_types(target_type, this->type())) {
            return *this;
        }
        const IType* element_type = static_cast_by_ref(target_type)->element_type();
        auto* this_dir = syntax::as<DirectionExpression>(expression);
        bool has_address_of = false;
        for (ILInstruction* inst : this->il_instructions())
            if (inst->op_code == il::OpCode::AddressOf) has_address_of = true;
        if (this_dir != nullptr && has_address_of) {
            ResolveResult* inner_rr = get_resolve_result(this_dir->expression());
            if (il::get_stack_type(*inner_rr->type()) == il::get_stack_type(*element_type)) {
                // When converting a reference to a temporary to a different type, apply the cast to the temporary instead.
                TranslatedExpression converted_temp =
                    this->unwrap_child(this_dir->expression()).convert_to(element_type, expression_builder, check_for_overflow);
                return csharp::with_il_instruction(make<DirectionExpression>(FieldDirection::Ref, converted_temp.expression), this->il_instructions())
                    .with_rr(make<ByReferenceResolveResult>(converted_temp.resolve_result, typesystem::ReferenceKind::Ref));
            }
        }
        if (this->type()->kind() == TypeKind::ByReference && !is_fixed_variable()) {
            // Convert between managed reference types.
            // We can't do this by going through a pointer type because that would temporarily stop GC tracking.
            // Instead, emit `ref Unsafe.As<T>(ref expr)`
            return expression_builder.call_unsafe_intrinsic(
                "As", {this->expression}, target_type, nullptr,
                std::vector<const IType*>{static_cast_by_ref(this->type())->element_type(), element_type});
        }
        // Convert from integer/pointer to reference.
        // First, convert to the corresponding pointer type:
        TranslatedExpression arg = this->convert_to(typesystem::make_pointer_type(element_type), expression_builder, check_for_overflow);
        ResolveResult* element_rr = nullptr;
        auto* unary = syntax::as<UnaryOperatorExpression>(arg.expression);
        if (unary != nullptr && unary->operator_() == UnaryOperatorType::AddressOf) {
            // If we already have an address -> unwrap
            expr = arg.unwrap_child(unary->expression()).expression;
            element_rr = get_resolve_result(expr);
        } else {
            // Otherwise dereference the pointer:
            expr = make<UnaryOperatorExpression>(UnaryOperatorType::Dereference, arg.expression);
            element_rr = make<ResolveResult>(element_type);
            expr->add_annotation(element_rr);
        }
        // And then take a reference:
        return without_il_instruction(make<DirectionExpression>(FieldDirection::Ref, expr))
            .with_rr(make<ByReferenceResolveResult>(element_rr, typesystem::ReferenceKind::Ref));
    }
    if (this->resolve_result->is_compile_time_constant() && !std::holds_alternative<std::monostate>(this->resolve_result->constant_value()) &&
        typesystem::nullable_type::is_nullable(target_type) && !utype->equals(target_utype) &&
        il::is_integer_type(il::get_stack_type(*target_utype))) {
        // Casts like `(uint?)-1` are only valid in an explicitly unchecked context, but we don't have logic to ensure such a context
        // (usually we emit into an implicitly unchecked context). This only applies with constants as input (int->uint? is fine in
        // implicitly unchecked context). We use an intermediate cast to the nullable's underlying type, which results in a constant
        // conversion, so the final output will be something like `(uint?)uint.MaxValue`
        return convert_to(target_utype, expression_builder, check_for_overflow, false)
            .convert_to(target_type, expression_builder, check_for_overflow, allow_implicit_conversion);
    }
    ResolveResult* rr = expression_builder.resolver->with_check_for_overflow(check_for_overflow).resolve_cast(target_type, resolve_result);
    if (rr->is_compile_time_constant() && !rr->is_error()) {
        TranslatedExpression converted_result =
            expression_builder.convert_constant_value(rr, allow_implicit_conversion).with_il_instruction(this->il_instructions());
        auto* output_literal = syntax::as<PrimitiveExpression>(converted_result.expression);
        auto* input_literal = syntax::as<PrimitiveExpression>(expression);
        if (output_literal != nullptr && input_literal != nullptr) {
            output_literal->set_format(input_literal->format());
        }
        return converted_result;
    } else if (rr->is_error() && target_type->is_reference_type() == true && source_type->is_reference_type() == true) {
        // Conversion between two reference types, but no direct cast allowed? cast via object
        // Just make sure we avoid infinite recursion even if the resolver falsely claims we can't cast directly:
        if (!(is_known_type(target_type, KnownTypeCode::Object) || is_known_type(source_type, KnownTypeCode::Object))) {
            return this->convert_to(compilation.find_type(KnownTypeCode::Object), expression_builder)
                .convert_to(target_type, expression_builder, check_for_overflow, allow_implicit_conversion);
        }
    } else if (source_type->kind() == TypeKind::Dynamic && target_type->is_reference_type() == true &&
               !is_known_type(target_type, KnownTypeCode::Object)) {
        // "static" conversion between dynamic and a reference type requires us to add a cast to object, otherwise recompilation would
        // produce a dynamic cast.
        // (T)dynamicExpression is a "dynamic" cast
        // (T)(object)dynamicExpression is a "static" cast
        // as "dynamic" casts are handled differently by ExpressionBuilder.VisitDynamicConvertInstruction we can always insert the cast
        // to object, if we encounter a conversion from any reference type to dynamic.
        return this->convert_to(compilation.find_type(KnownTypeCode::Object), expression_builder)
            .convert_to(target_type, expression_builder, check_for_overflow, allow_implicit_conversion);
    }
    {
        ConstantValue constant = resolve_result->constant_value();
        bool is_zero = is_int_constant(constant, 0);
        if (auto* u = std::get_if<std::uint32_t>(&constant)) is_zero = is_zero || *u == 0u;
        if (is_any_pointer(target_type->kind()) && is_zero) {
            if (allow_implicit_conversion) {
                return csharp::with_il_instruction(make<NullReferenceExpression>(), this->il_instructions())
                    .with_rr(make<ConstantResolveResult>(&SpecialType::null_type(), ConstantValue{}));
            }
            return csharp::with_il_instruction(make<CastExpression>(expression_builder.convert_type(target_type), make<NullReferenceExpression>()),
                                       this->il_instructions())
                .with_rr(make<ConstantResolveResult>(target_type, ConstantValue{}));
        }
    }
    if (allow_implicit_conversion) {
        if (conversions.implicit_conversion(resolve_result, target_type)->is_valid()) {
            return *this;
        }
    } else {
        if (typesystem::NormalizeTypeVisitor::ignore_nullability_and_tuples().equivalent_types(source_type, target_type)) {
            // avoid an explicit cast when types differ only in nullability of reference types
            return *this;
        }
    }
    // BaseReferenceExpression must not be used with CastExpressions
    expr = syntax::is<BaseReferenceExpression>(expression)
               ? static_cast<Expression*>(csharp::with_il_instruction(make<ThisReferenceExpression>(), this->il_instructions()).expression)
               : expression;
    auto* cast_expr = make<CastExpression>(expression_builder.convert_type(target_type), expr);
    bool needs_check_annotation = il::is_integer_type(il::get_stack_type(*target_utype));
    if (needs_check_annotation) {
        if (check_for_overflow) {
            add_check_annotation(cast_expr, CheckKind::Checked);
        } else if (resolve_result->is_compile_time_constant() && il::is_csharp_native_integer_type(*target_utype)) {
            // unchecked potentially-overflowing cast of constant to n(u)int:
            // Placement in implicitly unchecked context is not good enough when applied to compile-time constant, the constant must be
            // placed into an explicit unchecked block.
            // (note that non-potentially-overflowing casts will be handled by constant folding and won't get here)
            add_check_annotation(cast_expr, CheckKind::ExplicitUnchecked);
        } else {
            add_check_annotation(cast_expr, CheckKind::Unchecked);
        }
    }
    return without_il_instruction(cast_expr).with_rr(rr);
}

bool TranslatedExpression::is_fixed_variable() const {
    if (auto* dir_expr = syntax::as<DirectionExpression>(expression)) {
        ILInstruction* inst = dir_expr->expression()->annotation<ILInstruction>();
        return inst != nullptr && il::PointerArithmeticOffset::is_fixed_variable(inst);
    }
    return false;
}

// Whether an implicit conversion from 'input_type' to 'new_target_type' would have the same semantics as the existing cast from
// 'input_type' to 'old_target_type'. The existing cast is classified in 'conversion'.
bool TranslatedExpression::cast_can_be_made_implicit(const ICompilation& compilation, const semantics::Conversion* conversion, const IType* input_type,
                                                     const IType* old_target_type, const IType* new_target_type) {
    resolver::CSharpConversions& conversions = resolver::CSharpConversions::get(compilation);
    if (!conversion->is_implicit()) {
        // If the cast was required for the old conversion, avoid making it implicit.
        return false;
    }
    if (old_target_type->kind() == TypeKind::NInt || old_target_type->kind() == TypeKind::NUInt || new_target_type->kind() == TypeKind::NInt ||
        new_target_type->kind() == TypeKind::NUInt) {
        // nint has identity conversion with IntPtr, but the two have different implicit conversions
        return false;
    }
    if (conversion->is_boxing_conversion()) {
        return conversions.is_boxing_conversion_or_involving_type_parameter(input_type, new_target_type);
    }
    if (conversion->is_interpolated_string_conversion()) {
        return is_known_type(new_target_type, KnownTypeCode::FormattableString) || is_known_type(new_target_type, KnownTypeCode::IFormattable);
    }
    return conversions.identity_conversion(old_target_type, new_target_type);
}

TranslatedExpression TranslatedExpression::ldc_i4(const ICompilation& compilation, int value) {
    return without_il_instruction(make<PrimitiveExpression>(ConstantValue{static_cast<std::int32_t>(value)}))
        .with_rr(make<ConstantResolveResult>(compilation.find_type(KnownTypeCode::Int32), ConstantValue{static_cast<std::int32_t>(value)}));
}

// In conditional contexts, remove the bool-cast emitted when converting an "implicit operator bool" invocation.
TranslatedExpression TranslatedExpression::unwrap_implicit_bool_conversion(const std::function<bool(const IType*)>& type_filter) const {
    if (!is_known_type(this->type(), KnownTypeCode::Boolean)) return *this;
    auto* rr = dynamic_cast<ConversionResolveResult*>(this->resolve_result);
    if (rr == nullptr) return *this;
    if (!(rr->conversion->is_user_defined() && rr->conversion->is_implicit())) return *this;
    if (type_filter && !type_filter(rr->input->type())) return *this;
    if (auto* cast = syntax::as<CastExpression>(expression)) {
        return this->unwrap_child(cast->expression());
    }
    return *this;
}

// Expects that the input expression is an integer expression; produces an expression that returns true iff the integer value is not 0.
// If negate is true, instead produces an expression that returns true iff the integer value is 0.
TranslatedExpression TranslatedExpression::convert_to_boolean(ExpressionBuilder& expression_builder, bool negate) const {
    if (is_known_type(type(), KnownTypeCode::Boolean) || type()->kind() == TypeKind::Unknown) {
        if (negate) {
            return expression_builder.logic_not(*this).without_il_instruction();
        } else {
            return *this;
        }
    }
    assert(il::is_integer_type(il::get_stack_type(*type())));
    const ICompilation& compilation = *expression_builder.compilation;
    const IType* bool_type = compilation.find_type(KnownTypeCode::Boolean);
    ConstantValue constant = resolve_result->is_compile_time_constant() ? resolve_result->constant_value() : ConstantValue{};
    if (auto* int_value = std::get_if<std::int32_t>(&constant)) {
        bool val = *int_value != 0;
        val ^= negate;
        return csharp::with_il_instruction(make<PrimitiveExpression>(ConstantValue{val}), this->il_instructions())
            .with_rr(make<ConstantResolveResult>(bool_type, ConstantValue{val}));
    } else if (auto* byte_value = std::get_if<std::uint8_t>(&constant)) {
        bool val = *byte_value != 0;
        val ^= negate;
        return csharp::with_il_instruction(make<PrimitiveExpression>(ConstantValue{val}), this->il_instructions())
            .with_rr(make<ConstantResolveResult>(bool_type, ConstantValue{val}));
    } else if (type()->kind() == TypeKind::Pointer) {
        TranslatedExpression null_ref = without_il_instruction(make<NullReferenceExpression>())
                                            .with_rr(make<ConstantResolveResult>(&SpecialType::null_type(), ConstantValue{}));
        BinaryOperatorType op = negate ? BinaryOperatorType::Equality : BinaryOperatorType::InEquality;
        return without_il_instruction(make<BinaryOperatorExpression>(expression, op, null_ref.expression))
            .with_rr(make<OperatorResolveResult>(bool_type, il::ExpressionType::NotEqual,
                                                 std::vector<ResolveResult*>{this->resolve_result, null_ref.resolve_result}));
    }
    // An enum with a member of value 0: compare against that member.
    const typesystem::ITypeDefinition* type_def = type()->kind() == TypeKind::Enum ? type()->get_definition() : nullptr;
    bool has_zero_member = false;
    if (type_def != nullptr) {
        for (const typesystem::IField* field : type_def->fields()) {
            ConstantValue value = field->get_constant_value();
            if (std::holds_alternative<std::monostate>(value)) continue;
            ConstantValue as_ulong = util::csharp_primitive_cast(KnownTypeCode::UInt64, value, false);
            if (auto* u = std::get_if<std::uint64_t>(&as_ulong); u != nullptr && *u == 0) {
                has_zero_member = true;
                break;
            }
        }
    }
    if (has_zero_member) {
        TranslatedExpression zero =
            ExpressionWithResolveResult(expression_builder.convert_constant_value(make<ConstantResolveResult>(type(), ConstantValue{std::int32_t{0}}), true))
                .without_il_instruction();
        BinaryOperatorType op = negate ? BinaryOperatorType::Equality : BinaryOperatorType::InEquality;
        return without_il_instruction(make<BinaryOperatorExpression>(expression, op, zero.expression))
            .with_rr(make<OperatorResolveResult>(bool_type, il::ExpressionType::NotEqual,
                                                 std::vector<ResolveResult*>{this->resolve_result, zero.resolve_result}));
    }
    TranslatedExpression zero = without_il_instruction(make<PrimitiveExpression>(ConstantValue{std::int32_t{0}}))
                                    .with_rr(make<ConstantResolveResult>(compilation.find_type(KnownTypeCode::Int32), ConstantValue{std::int32_t{0}}));
    BinaryOperatorType op = negate ? BinaryOperatorType::Equality : BinaryOperatorType::InEquality;
    return without_il_instruction(make<BinaryOperatorExpression>(expression, op, zero.expression))
        .with_rr(make<OperatorResolveResult>(bool_type, il::ExpressionType::NotEqual,
                                             std::vector<ResolveResult*>{this->resolve_result, zero.resolve_result}));
}

}  // namespace cb::dotnet::decompiler::csharp
