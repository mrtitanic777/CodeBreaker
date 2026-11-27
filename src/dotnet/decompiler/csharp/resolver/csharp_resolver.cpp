#include "dotnet/decompiler/csharp/resolver/csharp_resolver.hpp"

#include <algorithm>
#include <cassert>
#include <cwctype>
#include <mutex>
#include <stdexcept>
#include <unordered_map>

#include "dotnet/decompiler/csharp/resolver/await_resolve_result.hpp"
#include "dotnet/decompiler/csharp/resolver/csharp_invocation_resolve_result.hpp"
#include "dotnet/decompiler/csharp/resolver/csharp_operators.hpp"
#include "dotnet/decompiler/csharp/resolver/dynamic_invocation_resolve_result.hpp"
#include "dotnet/decompiler/csharp/resolver/dynamic_member_resolve_result.hpp"
#include "dotnet/decompiler/csharp/resolver/log.hpp"
#include "dotnet/decompiler/csharp/resolver/type_inference.hpp"
#include "dotnet/decompiler/semantics/ambiguous_resolve_result.hpp"
#include "dotnet/decompiler/semantics/array_access_resolve_result.hpp"
#include "dotnet/decompiler/semantics/by_reference_resolve_result.hpp"
#include "dotnet/decompiler/semantics/constant_resolve_result.hpp"
#include "dotnet/decompiler/semantics/conversion_resolve_result.hpp"
#include "dotnet/decompiler/semantics/error_resolve_result.hpp"
#include "dotnet/decompiler/csharp/resolver/lambda_resolve_result.hpp"
#include "dotnet/decompiler/semantics/local_resolve_result.hpp"
#include "dotnet/decompiler/semantics/member_resolve_result.hpp"
#include "dotnet/decompiler/semantics/named_argument_resolve_result.hpp"
#include "dotnet/decompiler/semantics/size_of_resolve_result.hpp"
#include "dotnet/decompiler/semantics/this_resolve_result.hpp"
#include "dotnet/decompiler/semantics/type_of_resolve_result.hpp"
#include "dotnet/decompiler/semantics/unknown_member_resolve_result.hpp"
#include "dotnet/decompiler/typesystem/inheritance_helper.hpp"
#include "dotnet/decompiler/typesystem/metadata_entities.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/typesystem/type_utils.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"
#include "dotnet/decompiler/util/char_info.hpp"
#include "dotnet/decompiler/util/csharp_primitive_cast.hpp"

namespace cb::dotnet::decompiler::csharp::resolver {

using namespace typesystem;
using il::ExpressionType;
using semantics::AmbiguousTypeResolveResult;
using semantics::ArrayAccessResolveResult;
using semantics::ByReferenceResolveResult;
using semantics::ConstantResolveResult;
using semantics::Conversion;
using semantics::ConversionResolveResult;
using semantics::ErrorResolveResult;
using semantics::LocalResolveResult;
using semantics::MemberResolveResult;
using semantics::NamedArgumentResolveResult;
using semantics::NamespaceResolveResult;
using semantics::OperatorResolveResult;
using semantics::SizeOfResolveResult;
using semantics::ThisResolveResult;
using semantics::TypeOfResolveResult;
using semantics::TypeResolveResult;
using semantics::UnknownIdentifierResolveResult;
using semantics::UnknownMemberResolveResult;
using semantics::UnknownMethodResolveResult;

namespace {

// The numeric value of a known type code: KnownTypeCode numbers System.TypeCode's members the same, so the comparisons upstream makes
// between TypeCodes (Char <= code <= Decimal) are made on these.
int code_of(KnownTypeCode code) { return static_cast<int>(code); }

bool is_in_range(KnownTypeCode code, KnownTypeCode first, KnownTypeCode last) { return code_of(code) >= code_of(first) && code_of(code) <= code_of(last); }

// The known type code of the CLR type a constant value is an instance of (None for null).
KnownTypeCode known_type_code_of(const ConstantValue& value) {
    switch (value.index()) {
        case 1: return KnownTypeCode::Boolean;
        case 2: return KnownTypeCode::Char;
        case 3: return KnownTypeCode::SByte;
        case 4: return KnownTypeCode::Byte;
        case 5: return KnownTypeCode::Int16;
        case 6: return KnownTypeCode::UInt16;
        case 7: return KnownTypeCode::Int32;
        case 8: return KnownTypeCode::UInt32;
        case 9: return KnownTypeCode::Int64;
        case 10: return KnownTypeCode::UInt64;
        case 11: return KnownTypeCode::Single;
        case 12: return KnownTypeCode::Double;
        case 13: return KnownTypeCode::String;
        case 14: return KnownTypeCode::Decimal;
        default: return KnownTypeCode::None;
    }
}

bool is_null(const ConstantValue& value) { return std::holds_alternative<std::monostate>(value); }
bool is_string(const ConstantValue& value) { return std::holds_alternative<std::u16string>(value); }

// `char.ToLower(c) + rest` of upstream's MakeParameterName, on the UTF-8 text.
std::string lower_first_character(const std::string& text) {
    const std::u16string utf16_text = util::utf16(text);
    if (utf16_text.empty()) return text;
    std::u16string result = utf16_text;
    const char16_t first = result[0];
    if (first < 0x80) {
        if (first >= u'A' && first <= u'Z') result[0] = static_cast<char16_t>(first + (u'a' - u'A'));
    } else {
        result[0] = static_cast<char16_t>(std::towlower(static_cast<wint_t>(first)));
    }
    return util::utf8(result);
}

// util::csharp_primitive_cast with the resolver's own exceptions: an overflow is an ArithmeticError (upstream's OverflowException is an
// ArithmeticException, which the operator evaluation catches).
ConstantValue cast_constant(KnownTypeCode code, const ConstantValue& value, bool check_for_overflow) {
    try {
        return util::csharp_primitive_cast(code, value, check_for_overflow);
    } catch (const util::PrimitiveCastOverflowError&) {
        throw OverflowError();
    } catch (const util::PrimitiveCastInvalidError& e) {
        throw InvalidCastError(e.what());
    }
}

bool contains_name(const ArgumentNames& names, const std::string& name) {
    for (const std::optional<std::string>& n : names) {
        if (n.has_value() && *n == name) return true;
    }
    return false;
}

}  // namespace

// ---- the state the resolvers share ---------------------------------------------------------------------------------------------

// The lookups of simple names inside one type definition, cached for all resolvers of that definition.
struct CSharpResolver::TypeDefinitionCache {
    explicit TypeDefinitionCache(const ITypeDefinition* type_definition) : type_definition(type_definition) {}

    const ITypeDefinition* type_definition;
    std::mutex mutex;
    std::unordered_map<std::string, ResolveResult*> simple_name_lookup_cache_expression;
    std::unordered_map<std::string, ResolveResult*> simple_name_lookup_cache_invocation_target;
    std::unordered_map<std::string, ResolveResult*> simple_type_lookup_cache;
};

// An entry of the immutable stack of local variable dictionaries; the beginning of a block is one entry.
struct CSharpResolver::VariableScopeNode {
    std::shared_ptr<VariableDictionary> variables;
    std::shared_ptr<const VariableScopeNode> next;
};

struct CSharpResolver::ObjectInitializerContext {
    ResolveResult* initialized_object;
    std::shared_ptr<const ObjectInitializerContext> prev;
};

// ---- constructors ------------------------------------------------------------------------------------------------------------------

CSharpResolver::CSharpResolver(const ICompilation& compilation)
    : compilation_(&compilation), conversions_(&CSharpConversions::get(compilation)), context_(&compilation.main_module()) {}

CSharpResolver::CSharpResolver(const CSharpTypeResolveContext& context)
    : compilation_(&context.compilation()), conversions_(&CSharpConversions::get(context.compilation())), context_(context) {
    if (context.current_type_definition() != nullptr)
        current_type_definition_cache_ = std::make_shared<TypeDefinitionCache>(context.current_type_definition());
}

ResolveResult* CSharpResolver::error_result() const { return ErrorResolveResult::unknown_error(); }

// ---- properties --------------------------------------------------------------------------------------------------------------------

CSharpResolver CSharpResolver::with_context(const CSharpTypeResolveContext& new_context) const {
    CSharpResolver result(*this);
    result.context_ = new_context;
    return result;
}

CSharpResolver CSharpResolver::with_check_for_overflow(bool check_for_overflow) const {
    if (check_for_overflow == check_for_overflow_) return *this;
    CSharpResolver result(*this);
    result.check_for_overflow_ = check_for_overflow;
    return result;
}

CSharpResolver CSharpResolver::with_is_within_lambda_expression(bool is_within_lambda_expression) const {
    CSharpResolver result(*this);
    result.is_within_lambda_expression_ = is_within_lambda_expression;
    return result;
}

CSharpResolver CSharpResolver::with_current_member(const IMember* member) const { return with_context(context_.with_current_member(member)); }

CSharpResolver CSharpResolver::with_current_using_scope(const UsingScope* using_scope) const { return with_context(context_.with_using_scope(using_scope)); }

CSharpResolver CSharpResolver::with_current_type_definition(const ITypeDefinition* type_definition) const {
    if (current_type_definition() == type_definition) return *this;

    CSharpResolver result(*this);
    result.context_ = context_.with_current_type_definition(type_definition);
    if (type_definition != nullptr)
        result.current_type_definition_cache_ = std::make_shared<TypeDefinitionCache>(type_definition);
    else
        result.current_type_definition_cache_ = nullptr;
    return result;
}

// ---- local variable management -----------------------------------------------------------------------------------------------------

CSharpResolver CSharpResolver::with_local_variable_stack(std::shared_ptr<const VariableScopeNode> stack) const {
    CSharpResolver result(*this);
    result.local_variable_stack_ = std::move(stack);
    return result;
}

CSharpResolver CSharpResolver::add_variables(std::shared_ptr<VariableDictionary> variables) const {
    if (variables == nullptr) throw std::invalid_argument("variables");
    auto node = std::make_shared<VariableScopeNode>();
    node->variables = std::move(variables);
    node->next = local_variable_stack_;
    return with_local_variable_stack(std::move(node));
}

std::vector<const IVariable*> CSharpResolver::local_variables() const {
    std::vector<const IVariable*> result;
    for (const VariableScopeNode* node = local_variable_stack_.get(); node != nullptr; node = node->next.get()) {
        for (const auto& item : node->variables->items()) result.push_back(item.second);
    }
    return result;
}

// ---- object initializer context ----------------------------------------------------------------------------------------------------

CSharpResolver CSharpResolver::with_object_initializer_stack(std::shared_ptr<const ObjectInitializerContext> stack) const {
    CSharpResolver result(*this);
    result.object_initializer_stack_ = std::move(stack);
    return result;
}

CSharpResolver CSharpResolver::push_object_initializer(ResolveResult* initialized_object) const {
    if (initialized_object == nullptr) throw std::invalid_argument("initializedObject");
    auto context = std::make_shared<ObjectInitializerContext>();
    context->initialized_object = initialized_object;
    context->prev = object_initializer_stack_;
    return with_object_initializer_stack(std::move(context));
}

CSharpResolver CSharpResolver::pop_object_initializer() const {
    if (object_initializer_stack_ == nullptr) throw UsageError("there is no object initializer to pop");
    return with_object_initializer_stack(object_initializer_stack_->prev);
}

ResolveResult* CSharpResolver::current_object_initializer() const {
    return object_initializer_stack_ != nullptr ? object_initializer_stack_->initialized_object : error_result();
}

// ---- ResolveUnaryOperator ----------------------------------------------------------------------------------------------------------

ResolveResult* CSharpResolver::resolve_unary_operator(UnaryOperatorType op, ResolveResult* expression) const {
    if (expression->type()->kind() == TypeKind::Dynamic) {
        if (op == UnaryOperatorType::Await) {
            auto* get_awaiter = make<DynamicMemberResolveResult>(expression, "GetAwaiter");
            auto* invocation = make<DynamicInvocationResolveResult>(get_awaiter, DynamicInvocationType::Invocation, std::vector<ResolveResult*>{});
            return make<AwaitResolveResult>(&SpecialType::dynamic(), invocation, &SpecialType::dynamic(), nullptr, nullptr, nullptr);
        }
        return unary_operator_resolve_result(&SpecialType::dynamic(), op, expression);
    }

    // C# spec (draft-v11): 12.4.4 Unary operator overload resolution
    const std::string overloadable_operator_name = get_overloadable_operator_name(op);
    if (overloadable_operator_name.empty()) {
        switch (op) {
            case UnaryOperatorType::Dereference: {
                const auto* p = dynamic_cast<const PointerType*>(expression->type());
                if (p != nullptr) return unary_operator_resolve_result(p->element_type(), op, expression);
                return error_result();
            }
            case UnaryOperatorType::AddressOf:
                return unary_operator_resolve_result(make_pointer_type(expression->type()), op, expression);
            case UnaryOperatorType::Await: {
                ResolveResult* get_awaiter_method_group = resolve_member_access(expression, "GetAwaiter", {}, NameLookupMode::InvocationTarget);
                std::vector<ResolveResult*> no_arguments;
                ResolveResult* get_awaiter_invocation = resolve_invocation(get_awaiter_method_group, no_arguments, {}, false);

                MemberLookup lookup = create_member_lookup();
                const IMethod* get_result_method;
                const IType* await_result_type;
                auto* get_result_method_group = dynamic_cast<MethodGroupResolveResult*>(lookup.lookup(get_awaiter_invocation, "GetResult", {}, true));
                if (get_result_method_group != nullptr) {
                    OverloadResolution get_result_or = perform_overload_resolution(*get_result_method_group, *compilation_, {}, {}, false, true, true, true,
                                                                                   false, conversions_);
                    get_result_method = get_result_or.found_applicable_candidate()
                                            ? dynamic_cast<const IMethod*>(get_result_or.get_best_candidate_with_substituted_type_arguments())
                                            : nullptr;
                    await_result_type = get_result_method != nullptr ? get_result_method->return_type() : &SpecialType::unknown_type();
                } else {
                    get_result_method = nullptr;
                    await_result_type = &SpecialType::unknown_type();
                }

                ResolveResult* is_completed_rr = lookup.lookup(get_awaiter_invocation, "IsCompleted", {}, false);
                const IProperty* is_completed_property = nullptr;
                if (const auto* member_rr = dynamic_cast<MemberResolveResult*>(is_completed_rr); member_rr != nullptr)
                    is_completed_property = dynamic_cast<const IProperty*>(member_rr->member());
                if (is_completed_property != nullptr && (!is_known_type(is_completed_property->return_type(), KnownTypeCode::Boolean) || !is_completed_property->can_get()))
                    is_completed_property = nullptr;
                (void)get_result_method;
                (void)await_result_type;
                // Not adjusted to TS changes for interface impls. But I believe this is dead code for ILSpy anyways... (upstream: its own
                // code for the awaiter's OnCompleted method is commented out, and it throws).
                throw std::logic_error("NotImplemented: await of a non-dynamic operand");
            }
            default:
                return ErrorResolveResult::unknown_error();
        }
    }
    // If the type is nullable, get the underlying type:
    const IType* type = nullable_type::get_underlying_type(expression->type());
    const bool is_nullable = nullable_type::is_nullable(expression->type());

    // the operator is overloadable:
    OverloadResolution user_defined_operator_or = create_overload_resolution({expression});
    for (const IParameterizedMember* candidate : get_user_defined_operator_candidates(type, overloadable_operator_name)) {
        user_defined_operator_or.add_candidate(candidate);
    }
    if (user_defined_operator_or.found_applicable_candidate()) {
        return create_resolve_result_for_user_defined_operator(user_defined_operator_or, syntax::get_linq_node_type(op, check_for_overflow()));
    }

    expression = unary_numeric_promotion(op, type, is_nullable, expression);
    const std::vector<CSharpOperators::OperatorMethod*>* method_group;
    CSharpOperators& operators = CSharpOperators::get(*compilation_);
    switch (op) {
        case UnaryOperatorType::Increment:
        case UnaryOperatorType::Decrement:
        case UnaryOperatorType::PostIncrement:
        case UnaryOperatorType::PostDecrement: {
            // C# spec (draft-v11): 12.8.16 Postfix increment and decrement operators; 12.9.7 Prefix increment and decrement operators
            const KnownTypeCode code = get_type_code(type);
            if (is_in_range(code, KnownTypeCode::Char, KnownTypeCode::Decimal) || type->kind() == TypeKind::Enum || type->kind() == TypeKind::Pointer ||
                is_csharp_native_integer_type(*type))
                return unary_operator_resolve_result(expression->type(), op, expression, is_nullable);
            return make<ErrorResolveResult>(expression->type());
        }
        case UnaryOperatorType::Plus:
            if (is_csharp_native_integer_type(*type)) return unary_operator_resolve_result(expression->type(), op, expression, is_nullable);
            method_group = &operators.unary_plus_operators();
            break;
        case UnaryOperatorType::Minus:
            if (is_csharp_native_integer_type(*type)) return unary_operator_resolve_result(expression->type(), op, expression, is_nullable);
            method_group = check_for_overflow() ? &operators.checked_unary_minus_operators() : &operators.unchecked_unary_minus_operators();
            break;
        case UnaryOperatorType::Not:
            method_group = &operators.logical_negation_operators();
            break;
        case UnaryOperatorType::BitNot:
            if (type->kind() == TypeKind::Enum) {
                if (expression->is_compile_time_constant() && !is_nullable && !is_null(expression->constant_value())) {
                    // evaluate as (E)(~(U)x);
                    const IType* u = compilation_->find_type(known_type_code_of(expression->constant_value()));
                    auto* unpacked_enum = make<ConstantResolveResult>(u, expression->constant_value());
                    ResolveResult* rr = resolve_unary_operator(op, unpacked_enum);
                    rr = with_check_for_overflow(false).resolve_cast(type, rr);
                    if (rr->is_compile_time_constant()) return rr;
                }
                return unary_operator_resolve_result(expression->type(), op, expression, is_nullable);
            } else if (is_csharp_native_integer_type(*type)) {
                return unary_operator_resolve_result(expression->type(), op, expression, is_nullable);
            } else {
                method_group = &operators.bitwise_complement_operators();
                break;
            }
        default:
            throw UsageError("invalid unary operator");
    }
    OverloadResolution builtin_operator_or = create_overload_resolution({expression});
    for (CSharpOperators::OperatorMethod* candidate : *method_group) {
        builtin_operator_or.add_candidate(candidate);
    }
    const auto* m = dynamic_cast<const CSharpOperators::UnaryOperatorMethod*>(builtin_operator_or.best_candidate());
    const IType* result_type = m->return_type();
    if (builtin_operator_or.best_candidate_errors() != OverloadResolutionErrors::None) {
        if (user_defined_operator_or.best_candidate() != nullptr) {
            // If there are any user-defined operators, prefer those over the built-in operators. It'll be a more informative error.
            return create_resolve_result_for_user_defined_operator(user_defined_operator_or, syntax::get_linq_node_type(op, check_for_overflow()));
        } else if (builtin_operator_or.best_candidate_ambiguous_with() != nullptr) {
            // If the best candidate is ambiguous, just use the input type instead of picking one of the ambiguous overloads.
            return make<ErrorResolveResult>(expression->type());
        } else {
            return make<ErrorResolveResult>(result_type);
        }
    } else if (expression->is_compile_time_constant() && m->can_evaluate_at_compile_time()) {
        ConstantValue val;
        try {
            val = m->invoke(*this, expression->constant_value());
        } catch (const ArithmeticError&) {
            return make<ErrorResolveResult>(result_type);
        }
        return make<ConstantResolveResult>(result_type, std::move(val));
    } else {
        expression = convert(expression, m->parameters()[0]->type(), builtin_operator_or.argument_conversions()[0]);
        return unary_operator_resolve_result(result_type, op, expression, dynamic_cast<const ILiftedOperator*>(builtin_operator_or.best_candidate()) != nullptr);
    }
}

OperatorResolveResult* CSharpResolver::unary_operator_resolve_result(const IType* result_type, UnaryOperatorType op, ResolveResult* expression,
                                                                     bool is_lifted) const {
    return make<OperatorResolveResult>(result_type, syntax::get_linq_node_type(op, check_for_overflow()), nullptr, is_lifted,
                                       std::vector<ResolveResult*>{expression});
}

// ---- UnaryNumericPromotion ---------------------------------------------------------------------------------------------------------

ResolveResult* CSharpResolver::unary_numeric_promotion(UnaryOperatorType op, const IType*& type, bool is_nullable, ResolveResult* expression) const {
    // C# spec (draft-v11): 12.4.7.2 Unary numeric promotions
    KnownTypeCode code = get_type_code(type);
    if (is_nullable && type->kind() == TypeKind::Null) code = KnownTypeCode::SByte;  // cause promotion of null to int32
    switch (op) {
        case UnaryOperatorType::Minus:
            if (code == KnownTypeCode::UInt32) {
                type = compilation_->find_type(KnownTypeCode::Int64);
                return convert(expression, make_nullable(type, is_nullable),
                               is_nullable ? Conversion::implicit_nullable_conversion() : Conversion::implicit_numeric_conversion());
            }
            [[fallthrough]];
        case UnaryOperatorType::Plus:
        case UnaryOperatorType::BitNot:
            if (is_in_range(code, KnownTypeCode::Char, KnownTypeCode::UInt16)) {
                type = compilation_->find_type(KnownTypeCode::Int32);
                return convert(expression, make_nullable(type, is_nullable),
                               is_nullable ? Conversion::implicit_nullable_conversion() : Conversion::implicit_numeric_conversion());
            }
            break;
        default:
            break;
    }
    return expression;
}

// ---- GetOverloadableOperatorName ---------------------------------------------------------------------------------------------------

std::string CSharpResolver::get_overloadable_operator_name(UnaryOperatorType op) {
    switch (op) {
        case UnaryOperatorType::Not: return "op_LogicalNot";
        case UnaryOperatorType::BitNot: return "op_OnesComplement";
        case UnaryOperatorType::Minus: return "op_UnaryNegation";
        case UnaryOperatorType::Plus: return "op_UnaryPlus";
        case UnaryOperatorType::Increment:
        case UnaryOperatorType::PostIncrement: return "op_Increment";
        case UnaryOperatorType::Decrement:
        case UnaryOperatorType::PostDecrement: return "op_Decrement";
        default: return {};
    }
}

std::string CSharpResolver::get_overloadable_operator_name(BinaryOperatorType op) {
    switch (op) {
        case BinaryOperatorType::Add: return "op_Addition";
        case BinaryOperatorType::Subtract: return "op_Subtraction";
        case BinaryOperatorType::Multiply: return "op_Multiply";
        case BinaryOperatorType::Divide: return "op_Division";
        case BinaryOperatorType::Modulus: return "op_Modulus";
        case BinaryOperatorType::BitwiseAnd: return "op_BitwiseAnd";
        case BinaryOperatorType::BitwiseOr: return "op_BitwiseOr";
        case BinaryOperatorType::ExclusiveOr: return "op_ExclusiveOr";
        case BinaryOperatorType::ShiftLeft: return "op_LeftShift";
        case BinaryOperatorType::ShiftRight: return "op_RightShift";
        case BinaryOperatorType::UnsignedShiftRight: return "op_UnsignedRightShift";
        case BinaryOperatorType::Equality: return "op_Equality";
        case BinaryOperatorType::InEquality: return "op_Inequality";
        case BinaryOperatorType::GreaterThan: return "op_GreaterThan";
        case BinaryOperatorType::LessThan: return "op_LessThan";
        case BinaryOperatorType::GreaterThanOrEqual: return "op_GreaterThanOrEqual";
        case BinaryOperatorType::LessThanOrEqual: return "op_LessThanOrEqual";
        default: return {};
    }
}

// ---- ResolveBinaryOperator ---------------------------------------------------------------------------------------------------------

ResolveResult* CSharpResolver::resolve_binary_operator(BinaryOperatorType op, ResolveResult* lhs, ResolveResult* rhs) const {
    if (lhs->type()->kind() == TypeKind::Dynamic || rhs->type()->kind() == TypeKind::Dynamic) {
        lhs = convert(lhs, &SpecialType::dynamic());
        rhs = convert(rhs, &SpecialType::dynamic());
        return binary_operator_resolve_result(&SpecialType::dynamic(), lhs, op, rhs);
    }

    // C# 4.0 spec: 7.3.4 Binary operator overload resolution
    std::string overloadable_operator_name = get_overloadable_operator_name(op);
    if (overloadable_operator_name.empty()) {
        // Handle logical and/or exactly as bitwise and/or:
        // - If the user overloads a bitwise operator, that implicitly creates the corresponding logical operator.
        // - If both inputs are compile-time constants, it doesn't matter that we don't short-circuit.
        // - If inputs aren't compile-time constants, we don't evaluate anything, so again it doesn't matter that we don't short-circuit
        if (op == BinaryOperatorType::ConditionalAnd) {
            overloadable_operator_name = get_overloadable_operator_name(BinaryOperatorType::BitwiseAnd);
        } else if (op == BinaryOperatorType::ConditionalOr) {
            overloadable_operator_name = get_overloadable_operator_name(BinaryOperatorType::BitwiseOr);
        } else if (op == BinaryOperatorType::NullCoalescing) {
            // null coalescing operator is not overloadable and needs to be handled separately
            return resolve_null_coalescing_operator(lhs, rhs);
        } else {
            return ErrorResolveResult::unknown_error();
        }
    }

    // If the type is nullable, get the underlying type:
    bool is_nullable = nullable_type::is_nullable(lhs->type()) || nullable_type::is_nullable(rhs->type());
    const IType* lhs_type = nullable_type::get_underlying_type(lhs->type());
    const IType* rhs_type = nullable_type::get_underlying_type(rhs->type());

    // the operator is overloadable:
    OverloadResolution user_defined_operator_or = create_overload_resolution({lhs, rhs});
    // a HashSet of the candidates: each once, in the order they were found
    std::vector<const IParameterizedMember*> user_operator_candidates;
    for (const IType* operand_type : {lhs_type, rhs_type}) {
        for (const IParameterizedMember* candidate : get_user_defined_operator_candidates(operand_type, overloadable_operator_name)) {
            const bool already = std::any_of(user_operator_candidates.begin(), user_operator_candidates.end(),
                                             [&](const IParameterizedMember* c) { return member_equals(c, candidate); });
            if (!already) user_operator_candidates.push_back(candidate);
        }
    }
    for (const IParameterizedMember* candidate : user_operator_candidates) {
        user_defined_operator_or.add_candidate(candidate);
    }
    if (user_defined_operator_or.found_applicable_candidate()) {
        return create_resolve_result_for_user_defined_operator(user_defined_operator_or, syntax::get_linq_node_type(op, check_for_overflow()));
    }

    if ((lhs_type->kind() == TypeKind::Null && rhs_type->is_reference_type() == false) ||
        (lhs_type->is_reference_type() == false && rhs_type->kind() == TypeKind::Null)) {
        is_nullable = true;
    }
    if (op == BinaryOperatorType::ShiftLeft || op == BinaryOperatorType::ShiftRight || op == BinaryOperatorType::UnsignedShiftRight) {
        // special case: the shift operators allow "var x = null << null", producing int?.
        if (lhs_type->kind() == TypeKind::Null && rhs_type->kind() == TypeKind::Null) is_nullable = true;
        // for shift operators, do unary promotion independently on both arguments
        lhs = unary_numeric_promotion(UnaryOperatorType::Plus, lhs_type, is_nullable, lhs);
        rhs = unary_numeric_promotion(UnaryOperatorType::Plus, rhs_type, is_nullable, rhs);
    } else {
        const bool allow_nullable_constants = op == BinaryOperatorType::Equality || op == BinaryOperatorType::InEquality;
        if (!binary_numeric_promotion(is_nullable, lhs, rhs, allow_nullable_constants)) return make<ErrorResolveResult>(lhs->type());
    }
    // re-read underlying types after numeric promotion
    lhs_type = nullable_type::get_underlying_type(lhs->type());
    rhs_type = nullable_type::get_underlying_type(rhs->type());

    const std::vector<CSharpOperators::OperatorMethod*>* method_group = nullptr;
    CSharpOperators& operators = CSharpOperators::get(*compilation_);
    switch (op) {
        case BinaryOperatorType::Multiply:
            method_group = &operators.multiplication_operators();
            break;
        case BinaryOperatorType::Divide:
            method_group = &operators.division_operators();
            break;
        case BinaryOperatorType::Modulus:
            method_group = &operators.remainder_operators();
            break;
        case BinaryOperatorType::Add:
            method_group = &operators.addition_operators();
            {
                if (lhs_type->kind() == TypeKind::Enum) {
                    // E operator +(E x, U y);
                    const IType* underlying_type = make_nullable(get_enum_underlying_type(lhs_type), is_nullable);
                    if (try_convert_enum(rhs, underlying_type, is_nullable, lhs)) {
                        return handle_enum_operator(is_nullable, lhs_type, op, lhs, rhs);
                    }
                }
                if (rhs_type->kind() == TypeKind::Enum) {
                    // E operator +(U x, E y);
                    const IType* underlying_type = make_nullable(get_enum_underlying_type(rhs_type), is_nullable);
                    if (try_convert_enum(lhs, underlying_type, is_nullable, rhs)) {
                        return handle_enum_operator(is_nullable, rhs_type, op, lhs, rhs);
                    }
                }

                if (lhs_type->kind() == TypeKind::Delegate && try_convert(rhs, lhs_type)) {
                    return binary_operator_resolve_result(lhs_type, lhs, op, rhs);
                } else if (rhs_type->kind() == TypeKind::Delegate && try_convert(lhs, rhs_type)) {
                    return binary_operator_resolve_result(rhs_type, lhs, op, rhs);
                }

                if (lhs_type->kind() == TypeKind::Null && rhs_type->kind() == TypeKind::Null) return make<ErrorResolveResult>(&SpecialType::null_type());
            }
            break;
        case BinaryOperatorType::Subtract:
            method_group = &operators.subtraction_operators();
            {
                if (lhs_type->kind() == TypeKind::Enum) {
                    // U operator -(E x, E y);
                    if (try_convert_enum(rhs, lhs->type(), is_nullable, lhs, false)) {
                        return handle_enum_subtraction(is_nullable, lhs_type, lhs, rhs);
                    }

                    // E operator -(E x, U y);
                    const IType* underlying_type = make_nullable(get_enum_underlying_type(lhs_type), is_nullable);
                    if (try_convert_enum(rhs, underlying_type, is_nullable, lhs)) {
                        return handle_enum_operator(is_nullable, lhs_type, op, lhs, rhs);
                    }
                }
                if (rhs_type->kind() == TypeKind::Enum) {
                    // U operator -(E x, E y);
                    if (try_convert_enum(lhs, rhs->type(), is_nullable, rhs)) {
                        return handle_enum_subtraction(is_nullable, rhs_type, lhs, rhs);
                    }

                    // E operator -(U x, E y);
                    const IType* underlying_type = make_nullable(get_enum_underlying_type(rhs_type), is_nullable);
                    if (try_convert_enum(lhs, underlying_type, is_nullable, rhs)) {
                        return handle_enum_operator(is_nullable, rhs_type, op, lhs, rhs);
                    }
                }

                if (lhs_type->kind() == TypeKind::Delegate && try_convert(rhs, lhs_type)) {
                    return binary_operator_resolve_result(lhs_type, lhs, op, rhs);
                } else if (rhs_type->kind() == TypeKind::Delegate && try_convert(lhs, rhs_type)) {
                    return binary_operator_resolve_result(rhs_type, lhs, op, rhs);
                }

                if (lhs_type->kind() == TypeKind::Null && rhs_type->kind() == TypeKind::Null) return make<ErrorResolveResult>(&SpecialType::null_type());
            }
            break;
        case BinaryOperatorType::ShiftLeft:
            method_group = &operators.shift_left_operators();
            break;
        case BinaryOperatorType::ShiftRight:
            method_group = &operators.shift_right_operators();
            break;
        case BinaryOperatorType::UnsignedShiftRight:
            method_group = &operators.unsigned_shift_right_operators();
            break;
        case BinaryOperatorType::Equality:
        case BinaryOperatorType::InEquality:
        case BinaryOperatorType::LessThan:
        case BinaryOperatorType::GreaterThan:
        case BinaryOperatorType::LessThanOrEqual:
        case BinaryOperatorType::GreaterThanOrEqual: {
            if (lhs_type->kind() == TypeKind::Enum && try_convert(rhs, lhs->type())) {
                // bool operator op(E x, E y);
                return handle_enum_comparison(op, lhs_type, is_nullable, lhs, rhs);
            } else if (rhs_type->kind() == TypeKind::Enum && try_convert(lhs, rhs->type())) {
                // bool operator op(E x, E y);
                return handle_enum_comparison(op, rhs_type, is_nullable, lhs, rhs);
            } else if (dynamic_cast<const PointerType*>(lhs_type) != nullptr && dynamic_cast<const PointerType*>(rhs_type) != nullptr) {
                return binary_operator_resolve_result(compilation_->find_type(KnownTypeCode::Boolean), lhs, op, rhs);
            } else if (is_csharp_native_integer_type(*lhs_type) || is_csharp_native_integer_type(*rhs_type)) {
                if (type_equals(lhs_type, rhs_type))
                    return binary_operator_resolve_result(compilation_->find_type(KnownTypeCode::Boolean), lhs, op, rhs, is_nullable);
                return make<ErrorResolveResult>(compilation_->find_type(KnownTypeCode::Boolean));
            }
            if (op == BinaryOperatorType::Equality || op == BinaryOperatorType::InEquality) {
                if (lhs_type->is_reference_type() == true && rhs_type->is_reference_type() == true && lhs_type->kind() != TypeKind::Null &&
                    rhs_type->kind() != TypeKind::Null &&
                    (conversions_->identity_conversion(lhs_type, rhs_type) || conversions_->explicit_conversion(lhs_type, rhs_type)->is_reference_conversion() ||
                     conversions_->explicit_conversion(rhs_type, lhs_type)->is_reference_conversion())) {
                    // If it's a reference comparison
                    if (op == BinaryOperatorType::Equality)
                        method_group = &operators.reference_equality_operators();
                    else
                        method_group = &operators.reference_inequality_operators();
                    break;
                } else if ((lhs_type->kind() == TypeKind::Null && is_nullable_type_or_non_value_type(rhs->type())) ||
                           (is_nullable_type_or_non_value_type(lhs->type()) && rhs_type->kind() == TypeKind::Null)) {
                    // compare type parameter or nullable type with the null literal
                    return binary_operator_resolve_result(compilation_->find_type(KnownTypeCode::Boolean), lhs, op, rhs);
                }
            }
            switch (op) {
                case BinaryOperatorType::Equality:
                    method_group = &operators.value_equality_operators();
                    break;
                case BinaryOperatorType::InEquality:
                    method_group = &operators.value_inequality_operators();
                    break;
                case BinaryOperatorType::LessThan:
                    method_group = &operators.less_than_operators();
                    break;
                case BinaryOperatorType::GreaterThan:
                    method_group = &operators.greater_than_operators();
                    break;
                case BinaryOperatorType::LessThanOrEqual:
                    method_group = &operators.less_than_or_equal_operators();
                    break;
                case BinaryOperatorType::GreaterThanOrEqual:
                    method_group = &operators.greater_than_or_equal_operators();
                    break;
                default:
                    throw UsageError("invalid binary operator");
            }
        } break;
        case BinaryOperatorType::BitwiseAnd:
        case BinaryOperatorType::BitwiseOr:
        case BinaryOperatorType::ExclusiveOr: {
            if (lhs_type->kind() == TypeKind::Enum) {
                // bool operator op(E x, E y);
                if (try_convert_enum(rhs, lhs->type(), is_nullable, lhs)) {
                    return handle_enum_operator(is_nullable, lhs_type, op, lhs, rhs);
                }
            }

            if (rhs_type->kind() == TypeKind::Enum) {
                // bool operator op(E x, E y);
                if (try_convert_enum(lhs, rhs->type(), is_nullable, rhs)) {
                    return handle_enum_operator(is_nullable, rhs_type, op, lhs, rhs);
                }
            }

            switch (op) {
                case BinaryOperatorType::BitwiseAnd:
                    method_group = &operators.bitwise_and_operators();
                    break;
                case BinaryOperatorType::BitwiseOr:
                    method_group = &operators.bitwise_or_operators();
                    break;
                case BinaryOperatorType::ExclusiveOr:
                    method_group = &operators.bitwise_xor_operators();
                    break;
                default:
                    throw UsageError("invalid binary operator");
            }
        } break;
        case BinaryOperatorType::ConditionalAnd:
            method_group = &operators.logical_and_operators();
            break;
        case BinaryOperatorType::ConditionalOr:
            method_group = &operators.logical_or_operators();
            break;
        default:
            throw UsageError("invalid binary operator");
    }
    if (is_csharp_native_integer_type(*lhs_type) || is_csharp_native_integer_type(*rhs_type)) {
        if (type_equals(lhs_type, rhs_type)) {
            return binary_operator_resolve_result(is_nullable ? nullable_type::create(*compilation_, lhs_type) : lhs_type, lhs, op, rhs, is_nullable);
        }
        // mixing nint/nuint is not allowed
        return make<ErrorResolveResult>(lhs_type);
    }
    OverloadResolution builtin_operator_or = create_overload_resolution({lhs, rhs});
    for (CSharpOperators::OperatorMethod* candidate : *method_group) {
        builtin_operator_or.add_candidate(candidate);
    }
    const auto* m = dynamic_cast<const CSharpOperators::BinaryOperatorMethod*>(builtin_operator_or.best_candidate());
    const IType* result_type = m->return_type();
    if (builtin_operator_or.best_candidate_errors() != OverloadResolutionErrors::None) {
        // If there are any user-defined operators, prefer those over the built-in operators. It'll be a more informative error.
        if (user_defined_operator_or.best_candidate() != nullptr)
            return create_resolve_result_for_user_defined_operator(user_defined_operator_or, syntax::get_linq_node_type(op, check_for_overflow()));
        return make<ErrorResolveResult>(result_type);
    } else if (lhs->is_compile_time_constant() && rhs->is_compile_time_constant() && m->can_evaluate_at_compile_time()) {
        ConstantValue val;
        try {
            val = m->invoke(*this, lhs->constant_value(), rhs->constant_value());
        } catch (const ArithmeticError&) {
            return make<ErrorResolveResult>(result_type);
        }
        return make<ConstantResolveResult>(result_type, std::move(val));
    } else {
        lhs = convert(lhs, m->parameters()[0]->type(), builtin_operator_or.argument_conversions()[0]);
        rhs = convert(rhs, m->parameters()[1]->type(), builtin_operator_or.argument_conversions()[1]);
        return binary_operator_resolve_result(result_type, lhs, op, rhs, dynamic_cast<const ILiftedOperator*>(builtin_operator_or.best_candidate()) != nullptr);
    }
}

bool CSharpResolver::is_nullable_type_or_non_value_type(const IType* type) const {
    return nullable_type::is_nullable(type) || type->is_reference_type() != false;
}

ResolveResult* CSharpResolver::binary_operator_resolve_result(const IType* result_type, ResolveResult* lhs, BinaryOperatorType op, ResolveResult* rhs,
                                                              bool is_lifted) const {
    return make<OperatorResolveResult>(result_type, syntax::get_linq_node_type(op, check_for_overflow()), nullptr, is_lifted,
                                       std::vector<ResolveResult*>{lhs, rhs});
}

// ---- Pointer arithmetic ------------------------------------------------------------------------------------------------------------

CSharpOperators::BinaryOperatorMethod* CSharpResolver::pointer_arithmetic_operator(const IType* result_type, const IType* input_type1, KnownTypeCode input_type2) const {
    return pointer_arithmetic_operator(result_type, input_type1, compilation_->find_type(input_type2));
}

CSharpOperators::BinaryOperatorMethod* CSharpResolver::pointer_arithmetic_operator(const IType* result_type, KnownTypeCode input_type1, const IType* input_type2) const {
    return pointer_arithmetic_operator(result_type, compilation_->find_type(input_type1), input_type2);
}

CSharpOperators::BinaryOperatorMethod* CSharpResolver::pointer_arithmetic_operator(const IType* result_type, const IType* input_type1, const IType* input_type2) const {
    auto method = std::make_unique<CSharpOperators::BinaryOperatorMethod>(*compilation_);
    method->return_type_value = result_type;
    method->parameters_value.push_back(compilation_->arena().keep(std::make_unique<DefaultParameter>(input_type1, std::string())));
    method->parameters_value.push_back(compilation_->arena().keep(std::make_unique<DefaultParameter>(input_type2, std::string())));
    // the operators object keeps the method for the life of the compilation
    return static_cast<CSharpOperators::BinaryOperatorMethod*>(CSharpOperators::get(*compilation_).own(std::move(method)));
}

// ---- Enum helper methods -----------------------------------------------------------------------------------------------------------

const IType* CSharpResolver::get_enum_underlying_type(const IType* enum_type) const {
    const ITypeDefinition* def = enum_type->get_definition();
    return def != nullptr ? def->enum_underlying_type() : &SpecialType::unknown_type();
}

// Handles the case where an enum value is compared with another enum value: bool operator op(E x, E y);
ResolveResult* CSharpResolver::handle_enum_comparison(BinaryOperatorType op, const IType* enum_type, bool is_nullable, ResolveResult* lhs,
                                                      ResolveResult* rhs) const {
    // evaluate as ((U)x op (U)y)
    const IType* element_type = get_enum_underlying_type(enum_type);
    if (lhs->is_compile_time_constant() && rhs->is_compile_time_constant() && !is_nullable && element_type->kind() != TypeKind::Enum) {
        ResolveResult* cast_lhs = resolve_cast(element_type, lhs);
        ResolveResult* cast_rhs = resolve_cast(element_type, rhs);
        ResolveResult* rr = resolve_binary_operator(op, cast_lhs, cast_rhs);
        if (rr->is_compile_time_constant()) return rr;
    }
    const IType* result_type = compilation_->find_type(KnownTypeCode::Boolean);
    return binary_operator_resolve_result(result_type, lhs, op, rhs, is_nullable);
}

// Handles the case where an enum value is subtracted from another enum value: U operator -(E x, E y);
ResolveResult* CSharpResolver::handle_enum_subtraction(bool is_nullable, const IType* enum_type, ResolveResult* lhs, ResolveResult* rhs) const {
    // evaluate as (U)((U)x - (U)y)
    const IType* element_type = get_enum_underlying_type(enum_type);
    if (lhs->is_compile_time_constant() && rhs->is_compile_time_constant() && !is_nullable && element_type->kind() != TypeKind::Enum) {
        ResolveResult* cast_lhs = resolve_cast(element_type, lhs);
        ResolveResult* cast_rhs = resolve_cast(element_type, rhs);
        ResolveResult* rr = resolve_binary_operator(BinaryOperatorType::Subtract, cast_lhs, cast_rhs);
        rr = with_check_for_overflow(false).resolve_cast(element_type, rr);
        if (rr->is_compile_time_constant()) return rr;
    }
    const IType* result_type = make_nullable(element_type, is_nullable);
    return binary_operator_resolve_result(result_type, lhs, BinaryOperatorType::Subtract, rhs, is_nullable);
}

// Handles the following enum operators:
//   E operator +(E x, U y);  E operator +(U x, E y);  E operator -(E x, U y);  E operator &(E x, E y);  E operator |(E x, E y);  E operator ^(E x, E y);
ResolveResult* CSharpResolver::handle_enum_operator(bool is_nullable, const IType* enum_type, BinaryOperatorType op, ResolveResult* lhs,
                                                    ResolveResult* rhs) const {
    // evaluate as (E)((U)x op (U)y)
    if (lhs->is_compile_time_constant() && rhs->is_compile_time_constant() && !is_nullable) {
        const IType* element_type = get_enum_underlying_type(enum_type);
        if (element_type->kind() != TypeKind::Enum) {
            ResolveResult* cast_lhs = resolve_cast(element_type, lhs);
            ResolveResult* cast_rhs = resolve_cast(element_type, rhs);
            ResolveResult* rr = resolve_binary_operator(op, cast_lhs, cast_rhs);
            rr = with_check_for_overflow(false).resolve_cast(enum_type, rr);
            if (rr->is_compile_time_constant()) return rr;  // only report result if it's a constant; use the regular OperatorResolveResult codepath otherwise
        }
    }
    const IType* result_type = make_nullable(enum_type, is_nullable);
    return binary_operator_resolve_result(result_type, lhs, op, rhs, is_nullable);
}

const IType* CSharpResolver::make_nullable(const IType* type, bool is_nullable) const {
    if (is_nullable) return nullable_type::create(*compilation_, type);
    return type;
}

// ---- BinaryNumericPromotion --------------------------------------------------------------------------------------------------------

bool CSharpResolver::binary_numeric_promotion(bool is_nullable, ResolveResult*& lhs, ResolveResult*& rhs, bool allow_nullable_constants) const {
    // C# spec (draft-v11): 12.4.7.3 Binary numeric promotions
    const IType* lhs_u_type = nullable_type::get_underlying_type(lhs->type());
    const IType* rhs_u_type = nullable_type::get_underlying_type(rhs->type());
    KnownTypeCode lhs_code = get_type_code(lhs_u_type);
    KnownTypeCode rhs_code = get_type_code(rhs_u_type);
    // Treat C# 9 native integers as falling between int and long. However they don't have a TypeCode, so we hack around that here:
    if (lhs_u_type->kind() == TypeKind::NInt) {
        lhs_code = KnownTypeCode::Int32;
    } else if (lhs_u_type->kind() == TypeKind::NUInt) {
        lhs_code = KnownTypeCode::UInt32;
    }
    if (rhs_u_type->kind() == TypeKind::NInt) {
        rhs_code = KnownTypeCode::Int32;
    } else if (rhs_u_type->kind() == TypeKind::NUInt) {
        rhs_code = KnownTypeCode::UInt32;
    }
    // if one of the inputs is the null literal, promote that to the type of the other operand
    if (is_nullable && lhs->type()->kind() == TypeKind::Null && is_in_range(rhs_code, KnownTypeCode::Boolean, KnownTypeCode::Decimal)) {
        lhs = cast_to(rhs_code, is_nullable, lhs, allow_nullable_constants);
        lhs_code = rhs_code;
    } else if (is_nullable && rhs->type()->kind() == TypeKind::Null && is_in_range(lhs_code, KnownTypeCode::Boolean, KnownTypeCode::Decimal)) {
        rhs = cast_to(lhs_code, is_nullable, rhs, allow_nullable_constants);
        rhs_code = lhs_code;
    }
    bool binding_error = false;
    if (is_in_range(lhs_code, KnownTypeCode::Char, KnownTypeCode::Decimal) && is_in_range(rhs_code, KnownTypeCode::Char, KnownTypeCode::Decimal)) {
        KnownTypeCode target_type;
        if (lhs_code == KnownTypeCode::Decimal || rhs_code == KnownTypeCode::Decimal) {
            target_type = KnownTypeCode::Decimal;
            binding_error = (lhs_code == KnownTypeCode::Single || lhs_code == KnownTypeCode::Double || rhs_code == KnownTypeCode::Single ||
                             rhs_code == KnownTypeCode::Double);
        } else if (lhs_code == KnownTypeCode::Double || rhs_code == KnownTypeCode::Double) {
            target_type = KnownTypeCode::Double;
        } else if (lhs_code == KnownTypeCode::Single || rhs_code == KnownTypeCode::Single) {
            target_type = KnownTypeCode::Single;
        } else if (lhs_code == KnownTypeCode::UInt64 || rhs_code == KnownTypeCode::UInt64) {
            target_type = KnownTypeCode::UInt64;
            binding_error = is_signed(lhs_code, lhs) || is_signed(rhs_code, rhs);
        } else if (lhs_u_type->kind() == TypeKind::NUInt || rhs_u_type->kind() == TypeKind::NUInt) {
            binding_error = is_signed(lhs_code, lhs) || is_signed(rhs_code, rhs);
            lhs = cast_to(&SpecialType::nuint(), is_nullable, lhs, allow_nullable_constants);
            rhs = cast_to(&SpecialType::nuint(), is_nullable, rhs, allow_nullable_constants);
            return !binding_error;
        } else if (lhs_code == KnownTypeCode::UInt32 || rhs_code == KnownTypeCode::UInt32) {
            target_type = (is_signed(lhs_code, lhs) || is_signed(rhs_code, rhs)) ? KnownTypeCode::Int64 : KnownTypeCode::UInt32;
        } else if (lhs_code == KnownTypeCode::Int64 || rhs_code == KnownTypeCode::Int64) {
            target_type = KnownTypeCode::Int64;
        } else if (lhs_u_type->kind() == TypeKind::NInt || rhs_u_type->kind() == TypeKind::NInt) {
            lhs = cast_to(&SpecialType::nint(), is_nullable, lhs, allow_nullable_constants);
            rhs = cast_to(&SpecialType::nint(), is_nullable, rhs, allow_nullable_constants);
            return !binding_error;
        } else {
            target_type = KnownTypeCode::Int32;
        }
        lhs = cast_to(target_type, is_nullable, lhs, allow_nullable_constants);
        rhs = cast_to(target_type, is_nullable, rhs, allow_nullable_constants);
    }
    return !binding_error;
}

bool CSharpResolver::is_signed(KnownTypeCode code, ResolveResult* rr) const {
    // Determine whether the rr with code == get_type_code(underlying type of rr's type) is a signed primitive type.
    switch (code) {
        case KnownTypeCode::SByte:
        case KnownTypeCode::Int16:
            return true;
        case KnownTypeCode::Int32:
            // for int, consider implicit constant expression conversion
            if (rr->is_compile_time_constant() && !is_null(rr->constant_value()) && std::get<std::int32_t>(rr->constant_value()) >= 0) return false;
            return true;
        case KnownTypeCode::Int64:
            // for long, consider implicit constant expression conversion
            if (rr->is_compile_time_constant() && !is_null(rr->constant_value()) && std::get<std::int64_t>(rr->constant_value()) >= 0) return false;
            return true;
        default:
            return false;
    }
}

ResolveResult* CSharpResolver::cast_to(KnownTypeCode target_type, bool is_nullable, ResolveResult* expression, bool allow_nullable_constants) const {
    return cast_to(compilation_->find_type(target_type), is_nullable, expression, allow_nullable_constants);
}

ResolveResult* CSharpResolver::cast_to(const IType* target_type, bool is_nullable, ResolveResult* expression, bool allow_nullable_constants) const {
    const IType* nullable_type_ = make_nullable(target_type, is_nullable);
    if (type_equals(nullable_type_, expression->type())) return expression;
    if (allow_nullable_constants && expression->is_compile_time_constant()) {
        if (is_null(expression->constant_value())) return make<ConstantResolveResult>(nullable_type_, ConstantValue{});
        ResolveResult* rr = resolve_cast(target_type, expression);
        if (rr->is_error()) return rr;
        if (rr->is_compile_time_constant()) return make<ConstantResolveResult>(nullable_type_, rr->constant_value());
    }
    return convert(expression, nullable_type_, is_nullable ? Conversion::implicit_nullable_conversion() : Conversion::implicit_numeric_conversion());
}

// ---- Null coalescing operator ------------------------------------------------------------------------------------------------------

ResolveResult* CSharpResolver::resolve_null_coalescing_operator(ResolveResult* lhs, ResolveResult* rhs) const {
    if (nullable_type::is_nullable(lhs->type())) {
        const IType* a0 = nullable_type::get_underlying_type(lhs->type());
        if (try_convert(rhs, a0)) {
            return binary_operator_resolve_result(a0, lhs, BinaryOperatorType::NullCoalescing, rhs);
        }
    }
    if (try_convert(rhs, lhs->type())) {
        return binary_operator_resolve_result(lhs->type(), lhs, BinaryOperatorType::NullCoalescing, rhs);
    }
    if (try_convert(lhs, rhs->type())) {
        return binary_operator_resolve_result(rhs->type(), lhs, BinaryOperatorType::NullCoalescing, rhs);
    }
    return make<ErrorResolveResult>(lhs->type());
}

// ---- Get user-defined operator candidates ------------------------------------------------------------------------------------------

std::vector<const IParameterizedMember*> CSharpResolver::get_user_defined_operator_candidates(const IType* type, const std::string& operator_name) const {
    if (operator_name.empty()) return {};
    const KnownTypeCode c = get_type_code(type);
    if (is_in_range(c, KnownTypeCode::Boolean, KnownTypeCode::Decimal)) {
        // The .NET framework contains some of C#'s built-in operators as user-defined operators. However, we must not use those as
        // user-defined operators (we would skip numeric promotion).
        return {};
    }
    // C# spec (draft-v11): 12.4.6 Candidate user-defined operators
    std::vector<const IMethod*> operators = type->get_methods([&](const IMethod* m) { return m->is_operator() && m->name() == operator_name; });
    lift_user_defined_operators(operators);
    return std::vector<const IParameterizedMember*>(operators.begin(), operators.end());
}

void CSharpResolver::lift_user_defined_operators(std::vector<const IMethod*>& operators) const {
    const std::size_t non_lifted_method_count = operators.size();
    // Construct lifted operators
    for (std::size_t i = 0; i < non_lifted_method_count; i++) {
        const IMethod* lifted_method = CSharpOperators::lift_user_defined_operator(operators[i]);
        if (lifted_method != nullptr) operators.push_back(lifted_method);
    }
}

ResolveResult* CSharpResolver::create_resolve_result_for_user_defined_operator(OverloadResolution& r, ExpressionType operator_type) const {
    if (r.best_candidate_errors() != OverloadResolutionErrors::None) return r.create_resolve_result(nullptr);
    const IMethod* method = dynamic_cast<const IMethod*>(r.best_candidate());
    const bool is_lifted = dynamic_cast<const ILiftedOperator*>(method) != nullptr;
    return make<OperatorResolveResult>(method->return_type(), operator_type, method, is_lifted, r.get_arguments_with_conversions());
}

// ---- ResolveCast -------------------------------------------------------------------------------------------------------------------

bool CSharpResolver::try_convert(ResolveResult*& rr, const IType* target_type) const {
    const Conversion* c = conversions_->implicit_conversion(rr, target_type);
    if (c->is_valid()) {
        rr = convert(rr, target_type, c);
        return true;
    }
    return false;
}

// Converts `rr` (when a conversion exists) to `target_type`. is_nullable: whether we are dealing with a lifted operator; enum_rr: the
// enum-typed result, to which a nullable conversion is applied if necessary; allow_conversion_from_constant_zero: whether the conversion from
// the constant zero is allowed. Returns whether the conversion is successful; if it is not, the reference parameters are not modified.
bool CSharpResolver::try_convert_enum(ResolveResult*& rr, const IType* target_type, bool& is_nullable, ResolveResult*& enum_rr,
                                      bool allow_conversion_from_constant_zero) const {
    const Conversion* c;
    if (!is_nullable) {
        // Try non-nullable
        c = conversions_->implicit_conversion(rr, target_type);
        if (c->is_valid() && (allow_conversion_from_constant_zero || !c->is_enumeration_conversion())) {
            rr = convert(rr, target_type, c);
            return true;
        }
    }
    // make target_type nullable if it isn't already:
    if (!is_known_type(target_type, KnownTypeCode::NullableOfT)) target_type = nullable_type::create(*compilation_, target_type);

    c = conversions_->implicit_conversion(rr, target_type);
    if (c->is_valid() && (allow_conversion_from_constant_zero || !c->is_enumeration_conversion())) {
        rr = convert(rr, target_type, c);
        is_nullable = true;
        // Also convert the enum-typed result to nullable, if it isn't already
        if (!is_known_type(enum_rr->type(), KnownTypeCode::NullableOfT)) {
            const IType* nullable_type_ = nullable_type::create(*compilation_, enum_rr->type());
            enum_rr = make<ConversionResolveResult>(nullable_type_, enum_rr, Conversion::implicit_nullable_conversion());
        }
        return true;
    }
    return false;
}

ResolveResult* CSharpResolver::convert(ResolveResult* rr, const IType* target_type) const {
    return convert(rr, target_type, conversions_->implicit_conversion(rr, target_type));
}

ResolveResult* CSharpResolver::convert(ResolveResult* rr, const IType* target_type, const Conversion* c) const {
    if (c == Conversion::identity_conversion()) return rr;
    if (rr->is_compile_time_constant() && c != Conversion::none() && !c->is_user_defined()) return resolve_cast(target_type, rr);
    return make<ConversionResolveResult>(target_type, rr, c, check_for_overflow_);
}

ResolveResult* CSharpResolver::resolve_cast(const IType* target_type, ResolveResult* expression) const {
    // C# spec (draft-v11): 12.9.8 Cast expressions
    const Conversion* c = conversions_->explicit_conversion(expression, target_type);
    if (expression->is_compile_time_constant() && !c->is_user_defined()) {
        const IType* underlying_type = typesystem::get_enum_underlying_type(*target_type);
        KnownTypeCode code = get_type_code(underlying_type);
        const ConstantValue constant_value = expression->constant_value();
        if (is_in_range(code, KnownTypeCode::Boolean, KnownTypeCode::Decimal) && !is_null(constant_value)) {
            if (is_string(constant_value)) return make<ErrorResolveResult>(target_type);
            try {
                return make<ConstantResolveResult>(target_type, csharp_primitive_cast(code, constant_value));
            } catch (const OverflowError&) {
                return make<ErrorResolveResult>(target_type);
            } catch (const InvalidCastError&) {
                return make<ErrorResolveResult>(target_type);
            }
        } else if (code == KnownTypeCode::String) {
            if (is_null(constant_value) || is_string(constant_value)) return make<ConstantResolveResult>(target_type, constant_value);
            return make<ErrorResolveResult>(target_type);
        } else if ((underlying_type->kind() == TypeKind::NInt || underlying_type->kind() == TypeKind::NUInt) && !is_null(constant_value)) {
            if (is_string(constant_value)) return make<ErrorResolveResult>(target_type);
            code = (underlying_type->kind() == TypeKind::NInt ? KnownTypeCode::Int32 : KnownTypeCode::UInt32);
            try {
                return make<ConstantResolveResult>(target_type, cast_constant(code, constant_value, true));
            } catch (const OverflowError&) {
                // If constant value doesn't fit into 32-bits, the conversion is not a compile-time constant
                return make<ConversionResolveResult>(target_type, expression, c, check_for_overflow_);
            } catch (const InvalidCastError&) {
                return make<ErrorResolveResult>(target_type);
            }
        }
    }
    return make<ConversionResolveResult>(target_type, expression, c, check_for_overflow_);
}

ConstantValue CSharpResolver::csharp_primitive_cast(KnownTypeCode target_type, const ConstantValue& input) const {
    return cast_constant(target_type, input, check_for_overflow_);
}

// ---- ResolveSimpleName -------------------------------------------------------------------------------------------------------------

ResolveResult* CSharpResolver::resolve_simple_name(const std::string& identifier, const std::vector<const IType*>& type_arguments, bool is_invocation_target) const {
    // C# 4.0 spec: 7.6.2 Simple Names
    return lookup_simple_name_or_type_name(identifier, type_arguments, is_invocation_target ? NameLookupMode::InvocationTarget : NameLookupMode::Expression);
}

ResolveResult* CSharpResolver::lookup_simple_name_or_type_name(const std::string& identifier, const std::vector<const IType*>& type_arguments,
                                                               NameLookupMode lookup_mode) const {
    // C# 4.0 spec: 3.8 Namespace and type names; 7.6.2 Simple Names
    const int k = static_cast<int>(type_arguments.size());

    if (k == 0) {
        if (lookup_mode == NameLookupMode::Expression || lookup_mode == NameLookupMode::InvocationTarget) {
            // Look in local variables
            for (const VariableScopeNode* node = local_variable_stack_.get(); node != nullptr; node = node->next.get()) {
                if (const IVariable* const* v = node->variables->find(identifier); v != nullptr) return make<LocalResolveResult>(*v);
            }
            // Look in parameters of current method
            const auto* parameterized_member = dynamic_cast<const IParameterizedMember*>(current_member());
            if (parameterized_member != nullptr) {
                for (const IParameter* p : parameterized_member->parameters()) {
                    if (p->name() == identifier) return make<LocalResolveResult>(p);
                }
            }
        }

        // look in type parameters of current method
        const auto* m = dynamic_cast<const IMethod*>(current_member());
        if (m != nullptr) {
            for (const ITypeParameter* tp : m->type_parameters()) {
                if (tp->name() == identifier) return make<TypeResolveResult>(tp);
            }
        }
    }

    const bool parameterize_result_type =
        !(!type_arguments.empty() && std::all_of(type_arguments.begin(), type_arguments.end(), [](const IType* t) { return t->kind() == TypeKind::UnboundTypeArgument; }));

    ResolveResult* r = nullptr;
    if (current_type_definition_cache_ != nullptr) {
        std::unordered_map<std::string, ResolveResult*>* cache = nullptr;
        bool found_in_cache = false;
        if (k == 0) {
            switch (lookup_mode) {
                case NameLookupMode::Expression:
                    cache = &current_type_definition_cache_->simple_name_lookup_cache_expression;
                    break;
                case NameLookupMode::InvocationTarget:
                    cache = &current_type_definition_cache_->simple_name_lookup_cache_invocation_target;
                    break;
                case NameLookupMode::Type:
                    cache = &current_type_definition_cache_->simple_type_lookup_cache;
                    break;
                default:
                    break;
            }
            if (cache != nullptr) {
                const std::lock_guard lock(current_type_definition_cache_->mutex);
                const auto it = cache->find(identifier);
                if (it != cache->end()) {
                    found_in_cache = true;
                    r = it->second;
                } else {
                    r = nullptr;
                }
            }
        }
        if (found_in_cache) {
            r = (r != nullptr ? r->shallow_clone() : nullptr);
        } else {
            r = look_in_current_type(identifier, type_arguments, lookup_mode, parameterize_result_type);
            if (cache != nullptr) {
                // also cache missing members (r == null)
                const std::lock_guard lock(current_type_definition_cache_->mutex);
                (*cache)[identifier] = r;
            }
        }
        if (r != nullptr) return r;
    }

    if (context_.current_using_scope() == nullptr) {
        // If no using scope was specified, we still need to look in the global namespace:
        r = look_in_using_scope_namespace(nullptr, &compilation_->root_namespace(), identifier, type_arguments, parameterize_result_type);
    } else {
        if (k == 0 && lookup_mode != NameLookupMode::TypeInUsingDeclaration) {
            if (context_.current_using_scope()->try_get_resolve_cache(identifier, r)) {
                r = (r != nullptr ? r->shallow_clone() : nullptr);
            } else {
                r = look_in_current_using_scope(identifier, type_arguments, false, false);
                context_.current_using_scope()->try_add_resolve_cache(identifier, r);
            }
        } else {
            r = look_in_current_using_scope(identifier, type_arguments, lookup_mode == NameLookupMode::TypeInUsingDeclaration, parameterize_result_type);
        }
    }
    if (r != nullptr) return r;

    if (type_arguments.empty() && identifier == "dynamic") return make<TypeResolveResult>(&SpecialType::dynamic());
    return make<UnknownIdentifierResolveResult>(identifier, static_cast<int>(type_arguments.size()));
}

bool CSharpResolver::is_variable_reference_with_same_type(ResolveResult* rr, const std::string& identifier, semantics::TypeResolveResult** trr) const {
    if (dynamic_cast<MemberResolveResult*>(rr) == nullptr && dynamic_cast<LocalResolveResult*>(rr) == nullptr) {
        *trr = nullptr;
        return false;
    }
    *trr = dynamic_cast<TypeResolveResult*>(lookup_simple_name_or_type_name(identifier, {}, NameLookupMode::Type));
    return *trr != nullptr && type_equals((*trr)->type(), rr->type());
}

ResolveResult* CSharpResolver::look_in_current_type(const std::string& identifier, const std::vector<const IType*>& type_arguments, NameLookupMode lookup_mode,
                                                    bool parameterize_result_type) const {
    const int k = static_cast<int>(type_arguments.size());
    MemberLookup lookup = create_member_lookup(lookup_mode);
    // look in current type definitions
    for (const ITypeDefinition* t = current_type_definition(); t != nullptr; t = t->declaring_type_definition()) {
        if (k == 0) {
            // Look for type parameter with that name
            const std::vector<const ITypeParameter*>& type_parameters = t->type_parameters();
            // Look at all type parameters, including those copied from outer classes, so that we can fetch the version with the correct owner.
            for (const ITypeParameter* type_parameter : type_parameters) {
                if (type_parameter->name() == identifier) return make<TypeResolveResult>(type_parameter);
            }
        }

        if (lookup_mode == NameLookupMode::BaseTypeReference && t == current_type_definition()) {
            // don't look in current type when resolving a base type reference
            continue;
        }

        ResolveResult* r;
        if (lookup_mode == NameLookupMode::Expression || lookup_mode == NameLookupMode::InvocationTarget) {
            ResolveResult* target_resolve_result = (t == current_type_definition() ? resolve_this_reference() : make<TypeResolveResult>(t));
            r = lookup.lookup(target_resolve_result, identifier, type_arguments, lookup_mode == NameLookupMode::InvocationTarget);
        } else {
            r = lookup.lookup_type(t, identifier, type_arguments, parameterize_result_type);
        }
        if (dynamic_cast<UnknownMemberResolveResult*>(r) == nullptr)  // but do return AmbiguousMemberResolveResult
            return r;
    }
    return nullptr;
}

ResolveResult* CSharpResolver::look_in_current_using_scope(const std::string& identifier, const std::vector<const IType*>& type_arguments,
                                                           bool is_in_using_declaration, bool parameterize_result_type) const {
    // look in current namespace definitions
    const UsingScope* current_using_scope_ = this->current_using_scope();
    for (const UsingScope* u = current_using_scope_; u != nullptr; u = u->parent()) {
        ResolveResult* result_in_namespace = look_in_using_scope_namespace(u, u->namespace_(), identifier, type_arguments, parameterize_result_type);
        if (result_in_namespace != nullptr) return result_in_namespace;
        // then look for aliases:
        if (type_arguments.empty()) {
            const std::vector<std::string>& extern_aliases = u->extern_aliases();
            if (std::find(extern_aliases.begin(), extern_aliases.end(), identifier) != extern_aliases.end()) {
                return resolve_extern_alias(identifier);
            }
            if (!(is_in_using_declaration && u == current_using_scope_)) {
                for (const auto& pair : u->using_aliases()) {
                    if (pair.first == identifier) return pair.second->shallow_clone();
                }
            }
        }
        // finally, look in the imported namespaces:
        if (!(is_in_using_declaration && u == current_using_scope_)) {
            const IType* first_result = nullptr;
            for (const INamespace* imported_namespace : u->usings()) {
                const ITypeDefinition* def = imported_namespace->get_type_definition(identifier, static_cast<int>(type_arguments.size()));
                if (def != nullptr) {
                    const IType* result_type;
                    if (parameterize_result_type && !type_arguments.empty())
                        result_type = make_parameterized_type(def, type_arguments);
                    else
                        result_type = def;

                    if (first_result == nullptr || !top_level_type_definition_is_accessible(first_result->get_definition())) {
                        if (top_level_type_definition_is_accessible(result_type->get_definition())) first_result = result_type;
                    } else if (top_level_type_definition_is_accessible(def)) {
                        return make<AmbiguousTypeResolveResult>(first_result);
                    }
                }
            }
            if (first_result != nullptr) return make<TypeResolveResult>(first_result);
        }
        // if we didn't find anything: repeat lookup with parent namespace
    }
    return nullptr;
}

ResolveResult* CSharpResolver::look_in_using_scope_namespace(const UsingScope* using_scope, const INamespace* n, const std::string& identifier,
                                                             const std::vector<const IType*>& type_arguments, bool parameterize_result_type) const {
    if (n == nullptr) return nullptr;
    // first look for a namespace
    const int k = static_cast<int>(type_arguments.size());
    if (k == 0) {
        const INamespace* child_namespace = n->get_child_namespace(identifier);
        if (child_namespace != nullptr) {
            if (using_scope != nullptr && using_scope->has_alias(identifier))
                return make<AmbiguousTypeResolveResult>(make_unknown_type(compilation_->arena(), std::nullopt, identifier));
            return make<NamespaceResolveResult>(child_namespace);
        }
    }
    // then look for a type
    const ITypeDefinition* def = n->get_type_definition(identifier, k);
    if (def != nullptr && top_level_type_definition_is_accessible(def)) {
        const IType* result = def;
        if (parameterize_result_type && k > 0) result = make_parameterized_type(def, type_arguments);
        if (using_scope != nullptr && using_scope->has_alias(identifier)) return make<AmbiguousTypeResolveResult>(result);
        return make<TypeResolveResult>(result);
    }
    return nullptr;
}

bool CSharpResolver::top_level_type_definition_is_accessible(const ITypeDefinition* type_def) const {
    if (type_def->accessibility() == Accessibility::Internal) return type_def->parent_module()->internals_visible_to(compilation_->main_module());
    return true;
}

ResolveResult* CSharpResolver::resolve_alias(const std::string& identifier) const {
    if (identifier == "global") return make<NamespaceResolveResult>(&compilation_->root_namespace());

    for (const UsingScope* n = current_using_scope(); n != nullptr; n = n->parent()) {
        const std::vector<std::string>& extern_aliases = n->extern_aliases();
        if (std::find(extern_aliases.begin(), extern_aliases.end(), identifier) != extern_aliases.end()) return resolve_extern_alias(identifier);
        for (const auto& pair : n->using_aliases()) {
            if (pair.first == identifier) {
                auto* namespace_result = dynamic_cast<NamespaceResolveResult*>(pair.second);
                return namespace_result != nullptr ? static_cast<ResolveResult*>(namespace_result) : error_result();
            }
        }
    }
    return error_result();
}

ResolveResult* CSharpResolver::resolve_extern_alias(const std::string& alias) const {
    const INamespace* ns = compilation_->get_namespace_for_extern_alias(alias);
    if (ns != nullptr) return make<NamespaceResolveResult>(ns);
    return error_result();
}

// ---- ResolveMemberAccess -----------------------------------------------------------------------------------------------------------

ResolveResult* CSharpResolver::resolve_member_access(ResolveResult* target, const std::string& identifier, const std::vector<const IType*>& type_arguments,
                                                     NameLookupMode lookup_mode) const {
    // C# 4.0 spec: 7.6.4

    const bool parameterize_result_type =
        !(!type_arguments.empty() && std::all_of(type_arguments.begin(), type_arguments.end(), [](const IType* t) { return t->kind() == TypeKind::UnboundTypeArgument; }));
    if (auto* nrr = dynamic_cast<NamespaceResolveResult*>(target); nrr != nullptr) {
        return resolve_member_access_on_namespace(nrr, identifier, type_arguments, parameterize_result_type);
    }

    if (target->type()->kind() == TypeKind::Dynamic) return make<DynamicMemberResolveResult>(target, identifier);

    MemberLookup lookup = create_member_lookup(lookup_mode);
    ResolveResult* result;
    switch (lookup_mode) {
        case NameLookupMode::Expression:
            result = lookup.lookup(target, identifier, type_arguments, false);
            break;
        case NameLookupMode::InvocationTarget:
            result = lookup.lookup(target, identifier, type_arguments, true);
            break;
        case NameLookupMode::Type:
        case NameLookupMode::TypeInUsingDeclaration:
        case NameLookupMode::BaseTypeReference:
            // Don't do the UnknownMemberResolveResult/MethodGroupResolveResult processing, it's only relevant for expressions.
            return lookup.lookup_type(target->type(), identifier, type_arguments, parameterize_result_type);
        default:
            throw std::invalid_argument("Invalid value for NameLookupMode");
    }
    if (dynamic_cast<UnknownMemberResolveResult*>(result) != nullptr) {
        // We intentionally use all extension methods here, not just the eligible ones. Proper eligibility checking is only possible for
        // the full invocation (after we know the remaining arguments). The eligibility check in get_extension_methods is only intended
        // for code completion.
        std::vector<std::vector<const IMethod*>> extension_methods = get_extension_methods(identifier, type_arguments);
        if (!extension_methods.empty()) {
            auto* group = make<MethodGroupResolveResult>(target, identifier, std::vector<MethodListWithDeclaringType>{}, type_arguments);
            group->extension_methods = std::move(extension_methods);
            return group;
        }
    } else {
        auto* mgrr = dynamic_cast<MethodGroupResolveResult*>(result);
        if (mgrr != nullptr) {
            assert(mgrr->extension_methods.empty());
            // set the values that are necessary to make the group's extension methods lookup work (a copy of this resolver that lives
            // as long as the arena)
            mgrr->resolver = make<CSharpResolver>(*this);
        }
    }
    return result;
}

ResolveResult* CSharpResolver::resolve_member_access_on_namespace(NamespaceResolveResult* nrr, const std::string& identifier,
                                                                  const std::vector<const IType*>& type_arguments, bool parameterize_result_type) const {
    if (type_arguments.empty()) {
        const INamespace* child_namespace = nrr->namespace_()->get_child_namespace(identifier);
        if (child_namespace != nullptr) return make<NamespaceResolveResult>(child_namespace);
    }
    const ITypeDefinition* def = nrr->namespace_()->get_type_definition(identifier, static_cast<int>(type_arguments.size()));
    if (def != nullptr) {
        if (parameterize_result_type && !type_arguments.empty()) return make<TypeResolveResult>(make_parameterized_type(def, type_arguments));
        return make<TypeResolveResult>(def);
    }
    return error_result();
}

MemberLookup CSharpResolver::create_member_lookup() const {
    const ITypeDefinition* current_type_definition_ = this->current_type_definition();
    const bool is_in_enum_member_initializer = current_member() != nullptr && current_member()->symbol_kind() == SymbolKind::Field &&
                                               current_type_definition_ != nullptr && current_type_definition_->kind() == TypeKind::Enum;
    return MemberLookup(current_type_definition_, &compilation_->main_module(), is_in_enum_member_initializer);
}

MemberLookup CSharpResolver::create_member_lookup(NameLookupMode lookup_mode) const {
    if (lookup_mode == NameLookupMode::BaseTypeReference && current_type_definition() != nullptr) {
        // When looking up a base type reference, treat us as being outside the current type definition for accessibility purposes. This
        // avoids a stack overflow when referencing a protected class nested inside the base class of a parent class.
        return MemberLookup(current_type_definition()->declaring_type_definition(), &compilation_->main_module(), false);
    }
    return create_member_lookup();
}

// ---- ResolveIdentifierInObjectInitializer ------------------------------------------------------------------------------------------

ResolveResult* CSharpResolver::resolve_identifier_in_object_initializer(const std::string& identifier) const {
    MemberLookup member_lookup = create_member_lookup();
    return member_lookup.lookup(current_object_initializer(), identifier, {}, false);
}

// ---- ResolveForeach ----------------------------------------------------------------------------------------------------------------

ForEachResolveResult* CSharpResolver::resolve_foreach(ResolveResult* expression) const {
    MemberLookup member_lookup = create_member_lookup();

    const IType* collection_type;
    const IType* enumerator_type;
    const IType* element_type;
    ResolveResult* get_enumerator_invocation;
    ResolveResult* current_rr = nullptr;
    std::vector<ResolveResult*> no_arguments;
    // C# 4.0 spec: 8.8.4 The foreach statement
    if (expression->type()->kind() == TypeKind::Array || expression->type()->kind() == TypeKind::Dynamic) {
        collection_type = compilation_->find_type(KnownTypeCode::IEnumerable);
        enumerator_type = compilation_->find_type(KnownTypeCode::IEnumerator);
        if (expression->type()->kind() == TypeKind::Array) {
            element_type = dynamic_cast<const ArrayType*>(expression->type())->element_type();
        } else {
            element_type = &SpecialType::dynamic();
        }
        get_enumerator_invocation = resolve_cast(collection_type, expression);
        get_enumerator_invocation = resolve_member_access(get_enumerator_invocation, "GetEnumerator", {}, NameLookupMode::InvocationTarget);
        get_enumerator_invocation = resolve_invocation(get_enumerator_invocation, no_arguments);
    } else {
        auto* get_enumerator_method_group = dynamic_cast<MethodGroupResolveResult*>(member_lookup.lookup(expression, "GetEnumerator", {}, true));
        if (get_enumerator_method_group != nullptr) {
            OverloadResolution overload_resolution = perform_overload_resolution(*get_enumerator_method_group, *compilation_, {}, {}, false, false, false);
            if (overload_resolution.found_applicable_candidate() && !overload_resolution.is_ambiguous() && !overload_resolution.best_candidate()->is_static() &&
                overload_resolution.best_candidate()->accessibility() == Accessibility::Public) {
                collection_type = expression->type();
                get_enumerator_invocation = overload_resolution.create_resolve_result(expression);
                enumerator_type = get_enumerator_invocation->type();
                current_rr = member_lookup.lookup(make<ResolveResult>(enumerator_type), "Current", {}, false);
                element_type = current_rr->type();
            } else {
                check_for_enumerable_interface(expression, collection_type, enumerator_type, element_type, get_enumerator_invocation);
            }
        } else {
            check_for_enumerable_interface(expression, collection_type, enumerator_type, element_type, get_enumerator_invocation);
        }
    }
    const IMethod* move_next_method = nullptr;
    auto* move_next_method_group = dynamic_cast<MethodGroupResolveResult*>(member_lookup.lookup(make<ResolveResult>(enumerator_type), "MoveNext", {}, false));
    if (move_next_method_group != nullptr) {
        OverloadResolution overload_resolution = perform_overload_resolution(*move_next_method_group, *compilation_, {}, {}, false, false, false);
        move_next_method = dynamic_cast<const IMethod*>(overload_resolution.get_best_candidate_with_substituted_type_arguments());
    }

    if (current_rr == nullptr) current_rr = member_lookup.lookup(make<ResolveResult>(enumerator_type), "Current", {}, false);
    const IProperty* current_property = nullptr;
    if (auto* current_member_rr = dynamic_cast<MemberResolveResult*>(current_rr); current_member_rr != nullptr)
        current_property = dynamic_cast<const IProperty*>(current_member_rr->member());

    const IType* void_type = compilation_->find_type(KnownTypeCode::Void);
    return make<ForEachResolveResult>(get_enumerator_invocation, collection_type, enumerator_type, element_type, current_property, move_next_method, void_type);
}

void CSharpResolver::check_for_enumerable_interface(ResolveResult* expression, const IType*& collection_type, const IType*& enumerator_type,
                                                    const IType*& element_type, ResolveResult*& get_enumerator_invocation) const {
    std::optional<bool> is_generic;
    element_type = get_element_type_from_ienumerable(expression->type(), *compilation_, false, is_generic);
    if (is_generic == true) {
        const ITypeDefinition* enumerable_of_t = compilation_->find_type(KnownTypeCode::IEnumerableOfT)->get_definition();
        if (enumerable_of_t != nullptr)
            collection_type = make_parameterized_type(enumerable_of_t, {element_type});
        else
            collection_type = &SpecialType::unknown_type();

        const ITypeDefinition* enumerator_of_t = compilation_->find_type(KnownTypeCode::IEnumeratorOfT)->get_definition();
        if (enumerator_of_t != nullptr)
            enumerator_type = make_parameterized_type(enumerator_of_t, {element_type});
        else
            enumerator_type = &SpecialType::unknown_type();
    } else if (is_generic == false) {
        collection_type = compilation_->find_type(KnownTypeCode::IEnumerable);
        enumerator_type = compilation_->find_type(KnownTypeCode::IEnumerator);
    } else {
        collection_type = &SpecialType::unknown_type();
        enumerator_type = &SpecialType::unknown_type();
    }
    get_enumerator_invocation = resolve_cast(collection_type, expression);
    get_enumerator_invocation = resolve_member_access(get_enumerator_invocation, "GetEnumerator", {}, NameLookupMode::InvocationTarget);
    std::vector<ResolveResult*> no_arguments;
    get_enumerator_invocation = resolve_invocation(get_enumerator_invocation, no_arguments);
}

// ---- GetExtensionMethods -----------------------------------------------------------------------------------------------------------

std::vector<std::vector<const IMethod*>> CSharpResolver::get_extension_methods(const std::string& name, const std::vector<const IType*>& type_arguments) const {
    return get_extension_methods(static_cast<const IType*>(nullptr), name, type_arguments, false);
}

std::vector<std::vector<const IMethod*>> CSharpResolver::get_extension_methods(const IType* target_type, const std::string& name,
                                                                              const std::vector<const IType*>& type_arguments,
                                                                              bool substitute_inferred_types) const {
    MemberLookup lookup = create_member_lookup();
    std::vector<std::vector<const IMethod*>> extension_method_groups;
    for (const std::vector<const IMethod*>& input_group : get_all_extension_methods(lookup)) {
        std::vector<const IMethod*> output_group;
        for (const IMethod* method : input_group) {
            if (!name.empty() && method->name() != name) continue;
            if (!lookup.is_accessible(method, false)) continue;
            std::vector<const IType*> inferred_types;
            if (!type_arguments.empty()) {
                if (method->type_parameters().size() != type_arguments.size()) continue;
                const IMethod* sm = method->specialize_method(TypeParameterSubstitution(std::nullopt, type_arguments));
                if (is_eligible_extension_method(*compilation_, *conversions_, target_type, sm, false, &inferred_types)) output_group.push_back(sm);
            } else {
                if (is_eligible_extension_method(*compilation_, *conversions_, target_type, method, true, &inferred_types)) {
                    if (substitute_inferred_types && !inferred_types.empty()) {
                        output_group.push_back(method->specialize_method(TypeParameterSubstitution(std::nullopt, inferred_types)));
                    } else {
                        output_group.push_back(method);
                    }
                }
            }
        }
        if (!output_group.empty()) extension_method_groups.push_back(std::move(output_group));
    }
    return extension_method_groups;
}

bool CSharpResolver::is_eligible_extension_method(const IType* target_type, const IMethod* method, bool use_type_inference,
                                                  std::vector<const IType*>* out_inferred_types) {
    if (target_type == nullptr) throw std::invalid_argument("targetType");
    if (method == nullptr) throw std::invalid_argument("method");
    const ICompilation& compilation = method->compilation();
    return is_eligible_extension_method(compilation, CSharpConversions::get(compilation), target_type, method, use_type_inference, out_inferred_types);
}

bool CSharpResolver::is_eligible_extension_method(const ICompilation& compilation, CSharpConversions& conversions, const IType* target_type, const IMethod* method,
                                                  bool use_type_inference, std::vector<const IType*>* out_inferred_types) {
    if (out_inferred_types != nullptr) out_inferred_types->clear();
    if (target_type == nullptr) return true;
    if (method->parameters().empty()) return false;
    const IType* this_parameter_type = method->parameters()[0]->type();
    if (this_parameter_type->kind() == TypeKind::ByReference) {
        // extension method with `this in` or `this ref`
        this_parameter_type = dynamic_cast<const ByReferenceType*>(this_parameter_type)->element_type();
    }
    if (use_type_inference && !method->type_parameters().empty()) {
        // We need to infer type arguments from target_type:
        TypeInference ti(compilation, conversions);
        std::vector<ResolveResult*> arguments{make<ResolveResult>(target_type)};
        std::vector<const IType*> parameter_types{this_parameter_type};
        bool success = false;
        std::vector<const IType*> inferred_types = ti.infer_type_arguments(method->type_parameters(), arguments, parameter_types, &success);
        // (upstream's substitution shares the array, so it sees the slots replaced below: it is made again after each replacement)
        TypeParameterSubstitution substitution(std::nullopt, inferred_types);
        // Validate that the types that could be inferred (aren't unknown) satisfy the constraints:
        bool has_inferred_types = false;
        for (std::size_t i = 0; i < inferred_types.size(); i++) {
            if (inferred_types[i]->kind() != TypeKind::Unknown && inferred_types[i]->kind() != TypeKind::UnboundTypeArgument) {
                has_inferred_types = true;
                if (!OverloadResolution::validate_constraints(method->type_parameters()[i], inferred_types[i], &substitution, conversions)) return false;
            } else {
                inferred_types[i] = method->type_parameters()[i];  // do not substitute types that could not be inferred
                substitution = TypeParameterSubstitution(std::nullopt, inferred_types);
            }
        }
        if (has_inferred_types && out_inferred_types != nullptr) *out_inferred_types = inferred_types;
        this_parameter_type = this_parameter_type->accept_visitor(substitution);
    }
    const Conversion* c = conversions.implicit_conversion(target_type, this_parameter_type);
    return c->is_valid() && (c->is_identity_conversion() || c->is_reference_conversion() || c->is_boxing_conversion() || c->is_implicit_span_conversion());
}

// All extension methods available in the current using scope. This list includes inaccessible methods.
std::vector<std::vector<const IMethod*>> CSharpResolver::get_all_extension_methods(const MemberLookup& lookup) const {
    const UsingScope* current_using_scope_ = context_.current_using_scope();
    if (current_using_scope_ == nullptr) return {};
    if (const UsingScope::ExtensionMethodGroups* known = current_using_scope_->all_extension_methods(); known != nullptr) return *known;
    UsingScope::ExtensionMethodGroups extension_method_groups;
    for (const UsingScope* scope = current_using_scope_; scope != nullptr; scope = scope->parent()) {
        const INamespace* ns = scope->namespace_();
        if (ns != nullptr) {
            std::vector<const IMethod*> m = get_extension_methods(lookup, ns);
            if (!m.empty()) extension_method_groups.push_back(std::move(m));
        }

        // scope.Usings.Distinct().SelectMany(importedNamespace => GetExtensionMethods(lookup, importedNamespace))
        std::vector<const INamespace*> distinct_usings;
        for (const INamespace* imported_namespace : scope->usings()) {
            if (std::find(distinct_usings.begin(), distinct_usings.end(), imported_namespace) == distinct_usings.end()) distinct_usings.push_back(imported_namespace);
        }
        std::vector<const IMethod*> m;
        for (const INamespace* imported_namespace : distinct_usings) {
            for (const IMethod* method : get_extension_methods(lookup, imported_namespace)) m.push_back(method);
        }
        if (!m.empty()) extension_method_groups.push_back(std::move(m));
    }
    return current_using_scope_->get_or_set_all_extension_methods(std::move(extension_method_groups));
}

std::vector<const IMethod*> CSharpResolver::get_extension_methods(const MemberLookup& lookup, const INamespace* ns) const {
    // TODO (upstream): maybe make this a property on INamespace?
    std::vector<const IMethod*> result;
    for (const ITypeDefinition* c : ns->types()) {
        if (c->is_static() && c->has_extensions() && c->type_parameters().empty() && lookup.is_accessible(c, false)) {
            for (const IMethod* m : c->methods()) {
                if (m->is_extension_method()) result.push_back(m);
            }
        }
    }
    return result;
}

// ---- ResolveInvocation -------------------------------------------------------------------------------------------------------------

std::vector<ResolveResult*> CSharpResolver::add_argument_names_if_necessary(const std::vector<ResolveResult*>& arguments, const ArgumentNames& argument_names) {
    if (argument_names.empty()) return arguments;
    std::vector<ResolveResult*> result(arguments.size());
    for (std::size_t i = 0; i < arguments.size(); i++) {
        result[i] = (argument_names[i].has_value() ? static_cast<ResolveResult*>(make<NamedArgumentResolveResult>(*argument_names[i], arguments[i])) : arguments[i]);
    }
    return result;
}

ResolveResult* CSharpResolver::resolve_invocation(ResolveResult* target, std::vector<ResolveResult*>& arguments, const ArgumentNames& argument_names,
                                                  bool allow_optional_parameters) const {
    // C# 4.0 spec: 7.6.5

    if (target->type()->kind() == TypeKind::Dynamic) {
        return make<DynamicInvocationResolveResult>(target, DynamicInvocationType::Invocation, add_argument_names_if_necessary(arguments, argument_names));
    }

    const bool is_dynamic = std::any_of(arguments.begin(), arguments.end(), [](ResolveResult* a) { return a->type()->kind() == TypeKind::Dynamic; });
    if (auto* mgrr = dynamic_cast<MethodGroupResolveResult*>(target); mgrr != nullptr) {
        if (is_dynamic) {
            // If we have dynamic arguments, we need to represent the invocation as a dynamic invocation if there is more than one applicable method.
            OverloadResolution or2 = create_overload_resolution(arguments, argument_names, mgrr->type_arguments());
            struct ApplicableMethod {
                const IType* declaring_type;
                const IParameterizedMember* method;
            };
            std::vector<ApplicableMethod> applicable_methods;
            for (const MethodListWithDeclaringType& list : mgrr->methods_grouped_by_declaring_type()) {
                for (const IParameterizedMember* m : list) {
                    if (OverloadResolution::is_applicable(or2.add_candidate(m))) applicable_methods.push_back({list.declaring_type(), m});
                }
            }

            if (applicable_methods.size() > 1) {
                ResolveResult* actual_target;
                if (std::all_of(applicable_methods.begin(), applicable_methods.end(), [](const ApplicableMethod& x) { return x.method->is_static(); }) &&
                    dynamic_cast<TypeResolveResult*>(mgrr->target_result()) == nullptr)
                    actual_target = make<TypeResolveResult>(mgrr->target_type());
                else
                    actual_target = mgrr->target_result();

                std::vector<MethodListWithDeclaringType> l;
                for (const ApplicableMethod& m : applicable_methods) {
                    if (l.empty() || l.back().declaring_type() != m.declaring_type) l.emplace_back(m.declaring_type);
                    l.back().push_back(m.method);
                }
                return make<DynamicInvocationResolveResult>(make<MethodGroupResolveResult>(actual_target, mgrr->method_name(), std::move(l), mgrr->type_arguments()),
                                                            DynamicInvocationType::Invocation, add_argument_names_if_necessary(arguments, argument_names));
            }
        }

        OverloadResolution overload_resolution = perform_overload_resolution(*mgrr, *compilation_, arguments, argument_names, true, true, allow_optional_parameters,
                                                                             true, check_for_overflow_, conversions_);
        if (overload_resolution.best_candidate() != nullptr) {
            if (overload_resolution.best_candidate()->is_static() && !overload_resolution.is_extension_method_invocation() &&
                dynamic_cast<TypeResolveResult*>(mgrr->target_result()) == nullptr)
                return overload_resolution.create_resolve_result(make<TypeResolveResult>(mgrr->target_type()), {}, is_dynamic ? &SpecialType::dynamic() : nullptr);
            return overload_resolution.create_resolve_result(mgrr->target_result(), {}, is_dynamic ? &SpecialType::dynamic() : nullptr);
        }
        // No candidate found at all (not even an inapplicable one). This can happen with empty method groups (as sometimes used with
        // extension methods)
        return make<UnknownMethodResolveResult>(mgrr->target_type(), mgrr->method_name(), mgrr->type_arguments(), create_parameters(arguments, argument_names));
    }
    if (auto* umrr = dynamic_cast<UnknownMemberResolveResult*>(target); umrr != nullptr) {
        return make<UnknownMethodResolveResult>(umrr->target_type(), umrr->member_name(), umrr->type_arguments(), create_parameters(arguments, argument_names));
    }
    if (auto* uirr = dynamic_cast<UnknownIdentifierResolveResult*>(target); uirr != nullptr && current_type_definition() != nullptr) {
        return make<UnknownMethodResolveResult>(current_type_definition(), uirr->identifier(), std::vector<const IType*>{},
                                                create_parameters(arguments, argument_names));
    }
    const IMethod* invoke_method = get_delegate_invoke_method(target->type());
    if (invoke_method != nullptr) {
        OverloadResolution overload_resolution = create_overload_resolution(arguments, argument_names);
        overload_resolution.add_candidate(invoke_method);
        std::vector<ResolveResult*> arguments_with_conversions = overload_resolution.get_arguments_with_conversions_and_names();
        const OverloadResolutionErrors errors = overload_resolution.best_candidate_errors();
        const bool is_expanded_form = overload_resolution.best_candidate_is_expanded_form();
        std::optional<std::vector<int>> argument_to_parameter_map = overload_resolution.get_argument_to_parameter_map();
        return make<CSharpInvocationResolveResult>(target, invoke_method, std::move(arguments_with_conversions), errors, false, is_expanded_form, true,
                                                   std::move(argument_to_parameter_map), std::vector<ResolveResult*>{},
                                                   is_dynamic ? &SpecialType::dynamic() : nullptr);
    }
    return error_result();
}

ResolveResult* CSharpResolver::resolve_invocation(ResolveResult* target, std::vector<ResolveResult*>& arguments, const ArgumentNames& argument_names) const {
    return resolve_invocation(target, arguments, argument_names, true);
}

std::vector<const IParameter*> CSharpResolver::create_parameters(const std::vector<ResolveResult*>& arguments, const ArgumentNames& argument_names_in) const {
    std::vector<const IParameter*> list;
    ArgumentNames argument_names;
    if (argument_names_in.empty()) {
        argument_names.resize(arguments.size());
    } else {
        if (argument_names_in.size() != arguments.size()) throw std::invalid_argument("argumentNames");
        argument_names = argument_names_in;
    }
    for (std::size_t i = 0; i < arguments.size(); i++) {
        // invent argument names where necessary:
        if (!argument_names[i].has_value()) {
            std::string new_argument_name = guess_parameter_name(arguments[i]);
            if (contains_name(argument_names, new_argument_name)) {
                // disambiguate argument name (e.g. add a number)
                int num = 1;
                std::string new_name;
                do {
                    new_name = new_argument_name + std::to_string(num);
                    num++;
                } while (contains_name(argument_names, new_name));
                new_argument_name = new_name;
            }
            argument_names[i] = new_argument_name;
        }

        // create the parameter:
        auto* brrr = dynamic_cast<ByReferenceResolveResult*>(arguments[i]);
        if (brrr != nullptr) {
            list.push_back(compilation_->arena().keep(std::make_unique<DefaultParameter>(arguments[i]->type(), *argument_names[i], nullptr,
                                                                                       std::vector<const IAttribute*>{}, brrr->reference_kind())));
        } else {
            // argument might be a lambda or delegate type, so we have to try to guess the delegate type
            const IType* type = arguments[i]->type();
            if (type->kind() == TypeKind::Null || type->kind() == TypeKind::None) {
                list.push_back(compilation_->arena().keep(std::make_unique<DefaultParameter>(compilation_->find_type(KnownTypeCode::Object), *argument_names[i])));
            } else {
                list.push_back(compilation_->arena().keep(std::make_unique<DefaultParameter>(type, *argument_names[i])));
            }
        }
    }
    return list;
}

std::string CSharpResolver::guess_parameter_name(ResolveResult* rr) {
    if (auto* mrr = dynamic_cast<MemberResolveResult*>(rr); mrr != nullptr) return mrr->member()->name();

    if (auto* umrr = dynamic_cast<UnknownMemberResolveResult*>(rr); umrr != nullptr) return umrr->member_name();

    if (auto* mgrr = dynamic_cast<MethodGroupResolveResult*>(rr); mgrr != nullptr) return mgrr->method_name();

    if (auto* vrr = dynamic_cast<LocalResolveResult*>(rr); vrr != nullptr) return make_parameter_name(vrr->variable()->name());

    if (rr->type()->kind() != TypeKind::Unknown && !rr->type()->name().empty()) return make_parameter_name(rr->type()->name());
    return "parameter";
}

std::string CSharpResolver::make_parameter_name(const std::string& variable_name_in) {
    if (variable_name_in.empty()) return "parameter";
    std::string variable_name = variable_name_in;
    if (util::utf16(variable_name).size() > 1 && variable_name[0] == '_') variable_name = variable_name.substr(1);
    return lower_first_character(variable_name);
}

OverloadResolution CSharpResolver::create_overload_resolution(const std::vector<ResolveResult*>& arguments, const ArgumentNames& argument_names,
                                                              const std::vector<const IType*>& type_arguments) const {
    OverloadResolution overload_resolution(*compilation_, arguments, argument_names, type_arguments, conversions_);
    overload_resolution.set_check_for_overflow(check_for_overflow_);
    return overload_resolution;
}

// ---- ResolveIndexer ----------------------------------------------------------------------------------------------------------------

ResolveResult* CSharpResolver::resolve_indexer(ResolveResult* target, std::vector<ResolveResult*>& arguments, const ArgumentNames& argument_names) const {
    switch (target->type()->kind()) {
        case TypeKind::Dynamic:
            return make<DynamicInvocationResolveResult>(target, DynamicInvocationType::Indexing, add_argument_names_if_necessary(arguments, argument_names));

        case TypeKind::Array:
        case TypeKind::Pointer:
            // 7.6.6.1 Array access / 18.5.3 Pointer element access
            adjust_array_access_arguments(arguments);
            return make<ArrayAccessResolveResult>(dynamic_cast<const TypeWithElementType*>(target->type())->element_type(), target, arguments);
        default:
            break;
    }

    // 7.6.6.2 Indexer access

    MemberLookup lookup = create_member_lookup();
    std::vector<MethodListWithDeclaringType> indexers = lookup.lookup_indexers(target);

    if (std::any_of(arguments.begin(), arguments.end(), [](ResolveResult* a) { return a->type()->kind() == TypeKind::Dynamic; })) {
        // If we have dynamic arguments, we need to represent the invocation as a dynamic invocation if there is more than one applicable indexer.
        OverloadResolution or2 = create_overload_resolution(arguments, argument_names, {});
        std::size_t applicable_indexers = 0;
        for (const MethodListWithDeclaringType& list : indexers) {
            for (const IParameterizedMember* m : list) {
                if (OverloadResolution::is_applicable(or2.add_candidate(m))) applicable_indexers++;
            }
        }

        if (applicable_indexers > 1) {
            return make<DynamicInvocationResolveResult>(target, DynamicInvocationType::Indexing, add_argument_names_if_necessary(arguments, argument_names));
        }
    }

    OverloadResolution overload_resolution = create_overload_resolution(arguments, argument_names);
    overload_resolution.add_method_lists(indexers);
    if (overload_resolution.best_candidate() != nullptr) return overload_resolution.create_resolve_result(target);
    return error_result();
}

// Converts all arguments to int, uint, long or ulong.
void CSharpResolver::adjust_array_access_arguments(std::vector<ResolveResult*>& arguments) const {
    for (ResolveResult*& argument : arguments) {
        if (!(try_convert(argument, compilation_->find_type(KnownTypeCode::Int32)) || try_convert(argument, compilation_->find_type(KnownTypeCode::UInt32)) ||
              try_convert(argument, compilation_->find_type(KnownTypeCode::Int64)) || try_convert(argument, compilation_->find_type(KnownTypeCode::UInt64)))) {
            // conversion failed
            argument = convert(argument, compilation_->find_type(KnownTypeCode::Int32), Conversion::none());
        }
    }
}

// ---- ResolveObjectCreation ---------------------------------------------------------------------------------------------------------

ResolveResult* CSharpResolver::resolve_object_creation(const IType* type, std::vector<ResolveResult*>& arguments, const ArgumentNames& argument_names,
                                                       bool allow_protected_access, const std::vector<ResolveResult*>& initializer_statements) const {
    if (type->kind() == TypeKind::Delegate && arguments.size() == 1) {
        ResolveResult* input = arguments[0];
        const IMethod* invoke = get_delegate_invoke_method(input->type());
        if (invoke != nullptr) {
            MethodListWithDeclaringType method_list(input->type());
            method_list.push_back(invoke);
            input = make<MethodGroupResolveResult>(input, invoke->name(), std::vector<MethodListWithDeclaringType>{std::move(method_list)},
                                                   std::vector<const IType*>{});
        }
        return convert(input, type);
    }
    OverloadResolution overload_resolution = create_overload_resolution(arguments, argument_names);
    MemberLookup lookup = create_member_lookup();
    const bool track_applicable = std::any_of(arguments.begin(), arguments.end(), [](ResolveResult* a) { return a->type()->kind() == TypeKind::Dynamic; });
    std::vector<const IMethod*> all_applicable;
    for (const IMethod* ctor : type->get_constructors()) {
        if (lookup.is_accessible(ctor, allow_protected_access)) {
            const OverloadResolutionErrors or_errors = overload_resolution.add_candidate(ctor);
            if (track_applicable && OverloadResolution::is_applicable(or_errors)) all_applicable.push_back(ctor);
        } else {
            overload_resolution.add_candidate(ctor, OverloadResolutionErrors::Inaccessible);
        }
    }

    if (track_applicable && all_applicable.size() > 1) {
        // If we have dynamic arguments, we need to represent the invocation as a dynamic invocation if there is more than one applicable constructor.
        MethodListWithDeclaringType method_list(type, std::vector<const IParameterizedMember*>(all_applicable.begin(), all_applicable.end()));
        auto* group = make<MethodGroupResolveResult>(nullptr, all_applicable[0]->name(), std::vector<MethodListWithDeclaringType>{std::move(method_list)},
                                                     std::vector<const IType*>{});
        return make<DynamicInvocationResolveResult>(group, DynamicInvocationType::ObjectCreation, add_argument_names_if_necessary(arguments, argument_names),
                                                    initializer_statements);
    }

    if (overload_resolution.best_candidate() != nullptr) return overload_resolution.create_resolve_result(nullptr, initializer_statements);
    return make<ErrorResolveResult>(type);
}

// ---- ResolveSizeOf -----------------------------------------------------------------------------------------------------------------

ResolveResult* CSharpResolver::resolve_size_of(const IType* type) const {
    const IType* int32 = compilation_->find_type(KnownTypeCode::Int32);
    std::optional<int> size;
    const IType* type_for_constant = (type->kind() == TypeKind::Enum) ? type->get_definition()->enum_underlying_type() : type;

    switch (get_type_code(type_for_constant)) {
        case KnownTypeCode::Boolean:
        case KnownTypeCode::SByte:
        case KnownTypeCode::Byte:
            size = 1;
            break;
        case KnownTypeCode::Char:
        case KnownTypeCode::Int16:
        case KnownTypeCode::UInt16:
            size = 2;
            break;
        case KnownTypeCode::Int32:
        case KnownTypeCode::UInt32:
        case KnownTypeCode::Single:
            size = 4;
            break;
        case KnownTypeCode::Int64:
        case KnownTypeCode::UInt64:
        case KnownTypeCode::Double:
            size = 8;
            break;
        default:
            break;
    }
    return make<SizeOfResolveResult>(int32, type, size);
}

// ---- Resolve This/Base Reference ---------------------------------------------------------------------------------------------------

ResolveResult* CSharpResolver::resolve_this_reference() const {
    const ITypeDefinition* t = current_type_definition();
    if (t != nullptr) {
        if (t->type_parameter_count() != 0) {
            // Self-parameterize the type
            const std::vector<const ITypeParameter*>& type_parameters = t->type_parameters();
            return make<ThisResolveResult>(make_parameterized_type(t, std::vector<const IType*>(type_parameters.begin(), type_parameters.end())));
        }
        return make<ThisResolveResult>(t);
    }
    return error_result();
}

ResolveResult* CSharpResolver::resolve_base_reference() const {
    const ITypeDefinition* t = current_type_definition();
    if (t != nullptr) {
        for (const IType* base_type : t->direct_base_types()) {
            if (base_type->kind() != TypeKind::Unknown && base_type->kind() != TypeKind::Interface) {
                return make<ThisResolveResult>(base_type, true);
            }
        }
    }
    return error_result();
}

// ---- ResolveConditional ------------------------------------------------------------------------------------------------------------

ResolveResult* CSharpResolver::resolve_condition(ResolveResult* input) const {
    if (input == nullptr) throw std::invalid_argument("input");
    const IType* boolean = compilation_->find_type(KnownTypeCode::Boolean);
    const Conversion* c = conversions_->implicit_conversion(input, boolean);
    if (!c->is_valid()) {
        std::vector<const IMethod*> op_true = input->type()->get_methods([](const IMethod* m) { return m->is_operator() && m->name() == "op_True"; });
        if (!op_true.empty()) {
            c = Conversion::user_defined_conversion(op_true.front(), true, Conversion::none(), Conversion::none());
        }
    }
    return convert(input, boolean, c);
}

ResolveResult* CSharpResolver::resolve_condition_false(ResolveResult* input) const {
    if (input == nullptr) throw std::invalid_argument("input");
    const IType* boolean = compilation_->find_type(KnownTypeCode::Boolean);
    const Conversion* c = conversions_->implicit_conversion(input, boolean);
    if (!c->is_valid()) {
        std::vector<const IMethod*> op_false = input->type()->get_methods([](const IMethod* m) { return m->is_operator() && m->name() == "op_False"; });
        if (!op_false.empty()) {
            c = Conversion::user_defined_conversion(op_false.front(), true, Conversion::none(), Conversion::none());
            return convert(input, boolean, c);
        }
    }
    return resolve_unary_operator(UnaryOperatorType::Not, convert(input, boolean, c));
}

ResolveResult* CSharpResolver::resolve_conditional(ResolveResult* condition, ResolveResult* true_expression, ResolveResult* false_expression) const {
    // C# 4.0 spec 7.14: Conditional operator

    bool is_valid;
    const IType* result_type;
    if (true_expression->type()->kind() == TypeKind::Dynamic || false_expression->type()->kind() == TypeKind::Dynamic) {
        result_type = &SpecialType::dynamic();
        // (upstream's non-short-circuit `&`: both conversions are tried)
        const bool true_converted = try_convert(true_expression, result_type);
        const bool false_converted = try_convert(false_expression, result_type);
        is_valid = true_converted && false_converted;
    } else if (has_type(true_expression) && has_type(false_expression)) {
        const Conversion* t2f = conversions_->implicit_conversion(true_expression, false_expression->type());
        const Conversion* f2t = conversions_->implicit_conversion(false_expression, true_expression->type());
        // The operator is valid:
        // a) if there's a conversion in one direction but not the other
        // b) if there are conversions in both directions, and the types are equivalent
        if (is_better_conditional_conversion(t2f, f2t)) {
            result_type = false_expression->type();
            is_valid = true;
            true_expression = convert(true_expression, result_type, t2f);
        } else if (is_better_conditional_conversion(f2t, t2f)) {
            result_type = true_expression->type();
            is_valid = true;
            false_expression = convert(false_expression, result_type, f2t);
        } else {
            result_type = true_expression->type();
            is_valid = type_equals(true_expression->type(), false_expression->type());
        }
    } else if (has_type(true_expression)) {
        result_type = true_expression->type();
        is_valid = try_convert(false_expression, result_type);
    } else if (has_type(false_expression)) {
        result_type = false_expression->type();
        is_valid = try_convert(true_expression, result_type);
    } else {
        return error_result();
    }
    condition = resolve_condition(condition);
    if (is_valid) {
        if (condition->is_compile_time_constant() && true_expression->is_compile_time_constant() && false_expression->is_compile_time_constant()) {
            const ConstantValue condition_value = condition->constant_value();
            if (const bool* val = std::get_if<bool>(&condition_value); val != nullptr) {
                if (*val) return true_expression;
                return false_expression;
            }
        }
        return make<OperatorResolveResult>(result_type, ExpressionType::Conditional, std::vector<ResolveResult*>{condition, true_expression, false_expression});
    }
    return make<ErrorResolveResult>(result_type);
}

bool CSharpResolver::is_better_conditional_conversion(const Conversion* c1, const Conversion* c2) const {
    // Valid is better than ImplicitConstantExpressionConversion is better than invalid
    if (!c1->is_valid()) return false;
    if (c1 != Conversion::implicit_constant_expression_conversion() && c2 == Conversion::implicit_constant_expression_conversion()) return true;
    return !c2->is_valid();
}

bool CSharpResolver::has_type(ResolveResult* r) { return r->type()->kind() != TypeKind::None && r->type()->kind() != TypeKind::Null; }

// ---- ResolvePrimitive --------------------------------------------------------------------------------------------------------------

ResolveResult* CSharpResolver::resolve_primitive(const ConstantValue& value) const {
    if (is_null(value)) return make<ResolveResult>(&SpecialType::null_type());
    const IType* type = compilation_->find_type(known_type_code_of(value));
    return make<ConstantResolveResult>(type, value);
}

// ---- ResolveDefaultValue -----------------------------------------------------------------------------------------------------------

ResolveResult* CSharpResolver::resolve_default_value(const IType* type) const { return make<ConstantResolveResult>(type, get_default_value(type)); }

ConstantValue CSharpResolver::get_default_value(const IType* type) {
    const ITypeDefinition* type_def = type->get_definition();
    if (type_def == nullptr) return {};
    if (type_def->kind() == TypeKind::Enum) {
        type_def = type_def->enum_underlying_type()->get_definition();
        if (type_def == nullptr) return {};
    }
    switch (type_def->known_type_code()) {
        case KnownTypeCode::Boolean: return false;
        case KnownTypeCode::Char: return char16_t{0};
        case KnownTypeCode::SByte: return std::int8_t{0};
        case KnownTypeCode::Byte: return std::uint8_t{0};
        case KnownTypeCode::Int16: return std::int16_t{0};
        case KnownTypeCode::UInt16: return std::uint16_t{0};
        case KnownTypeCode::Int32: return std::int32_t{0};
        case KnownTypeCode::UInt32: return std::uint32_t{0};
        case KnownTypeCode::Int64: return std::int64_t{0};
        case KnownTypeCode::UInt64: return std::uint64_t{0};
        case KnownTypeCode::Single: return 0.0f;
        case KnownTypeCode::Double: return 0.0;
        case KnownTypeCode::Decimal: return util::Decimal{};
        default: return {};
    }
}

// ---- ResolveArrayCreation ----------------------------------------------------------------------------------------------------------

ArrayCreateResolveResult* CSharpResolver::resolve_array_creation(const IType* element_type, const std::vector<int>& size_arguments,
                                                                 std::vector<ResolveResult*>* initializer_elements) const {
    std::vector<ResolveResult*> size_arg_results(size_arguments.size());
    for (std::size_t i = 0; i < size_arguments.size(); i++) {
        if (size_arguments[i] < 0)
            size_arg_results[i] = ErrorResolveResult::unknown_error();
        else
            size_arg_results[i] = make<ConstantResolveResult>(compilation_->find_type(KnownTypeCode::Int32), ConstantValue{std::int32_t{size_arguments[i]}});
    }
    return resolve_array_creation(element_type, size_arg_results, initializer_elements);
}

ArrayCreateResolveResult* CSharpResolver::resolve_array_creation(const IType* element_type, std::vector<ResolveResult*>& size_arguments,
                                                                 std::vector<ResolveResult*>* initializer_elements) const {
    const int dimensions = static_cast<int>(size_arguments.size());
    if (dimensions == 0) throw std::invalid_argument("sizeArguments.Length must not be 0");
    if (element_type == nullptr) {
        TypeInference type_inference(*compilation_, *conversions_);
        bool success = false;
        element_type = type_inference.get_best_common_type(initializer_elements != nullptr ? *initializer_elements : std::vector<ResolveResult*>{}, &success);
    }
    const IType* array_type = make_array_type(*compilation_, element_type, dimensions);

    adjust_array_access_arguments(size_arguments);

    if (initializer_elements != nullptr) {
        for (ResolveResult*& element : *initializer_elements) element = convert(element, element_type);
    }
    return make<ArrayCreateResolveResult>(array_type, size_arguments,
                                          initializer_elements != nullptr ? std::optional<std::vector<ResolveResult*>>(*initializer_elements) : std::nullopt);
}

ResolveResult* CSharpResolver::resolve_type_of(const IType* referenced_type) const {
    return make<TypeOfResolveResult>(compilation_->find_type(KnownTypeCode::Type), referenced_type);
}

// ---- ResolveAssignment -------------------------------------------------------------------------------------------------------------

ResolveResult* CSharpResolver::resolve_assignment(AssignmentOperatorType op, ResolveResult* lhs, ResolveResult* rhs) const {
    const ExpressionType linq_op = syntax::get_linq_node_type(op, check_for_overflow());
    const std::optional<BinaryOperatorType> bop = syntax::get_corresponding_binary_operator(op);
    if (!bop.has_value()) {
        ResolveResult* converted = convert(rhs, lhs->type());
        return make<OperatorResolveResult>(lhs->type(), linq_op, std::vector<ResolveResult*>{lhs, converted});
    }
    ResolveResult* bop_result = resolve_binary_operator(*bop, lhs, rhs);
    auto* op_result = dynamic_cast<OperatorResolveResult*>(bop_result);
    if (op_result == nullptr || op_result->operands().size() != 2) return bop_result;
    return make<OperatorResolveResult>(lhs->type(), linq_op, op_result->user_defined_operator_method(), op_result->is_lifted_operator(),
                                       std::vector<ResolveResult*>{lhs, op_result->operands()[1]});
}

// ---- CanTransformToExtensionMethodCall ---------------------------------------------------------------------------------------------

bool CSharpResolver::can_transform_to_extension_method_call(const IMethod* method, const std::vector<const IType*>& type_arguments, ResolveResult* target,
                                                            std::vector<ResolveResult*>& arguments, const ArgumentNames& argument_names) const {
    if (dynamic_cast<LambdaResolveResult*>(target) != nullptr) return false;
    auto* rr = dynamic_cast<MethodGroupResolveResult*>(resolve_member_access(target, method->name(), type_arguments, NameLookupMode::InvocationTarget));
    if (rr == nullptr) return false;
    OverloadResolution overload_resolution = perform_overload_resolution(*rr, current_type_resolve_context().compilation(), arguments, argument_names, true);
    if (overload_resolution.is_ambiguous()) return false;
    return member_equals(method, overload_resolution.get_best_candidate_with_substituted_type_arguments()) &&
           CSharpResolver::is_eligible_extension_method(target->type(), method, false, nullptr);
}

bool CSharpResolver::can_transform_to_extension_method_call(const IMethod* method, bool ignore_type_arguments, bool ignore_argument_names) const {
    if (method->parameters().empty()) return false;
    ResolveResult* target_type = make<ResolveResult>(method->parameters()[0]->type());
    std::vector<ResolveResult*> param_types;
    for (std::size_t i = 1; i < method->parameters().size(); i++) param_types.push_back(make<ResolveResult>(method->parameters()[i]->type()));
    ArgumentNames param_names;
    if (!ignore_argument_names) {
        for (const IParameter* p : method->parameters()) param_names.push_back(p->name());
    }
    std::vector<const IType*> type_args;
    if (!ignore_type_arguments) type_args = method->type_arguments();
    return can_transform_to_extension_method_call(method, type_args, target_type, param_types, param_names);
}

}  // namespace cb::dotnet::decompiler::csharp::resolver
