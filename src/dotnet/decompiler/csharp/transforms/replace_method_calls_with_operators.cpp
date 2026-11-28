// dotnet/decompiler/csharp/transforms/replace_method_calls_with_operators.cpp -- see replace_method_calls_with_operators.hpp.
#include "dotnet/decompiler/csharp/l2_body.hpp"
#include "dotnet/decompiler/csharp/transforms/replace_method_calls_with_operators.hpp"

#include "dotnet/decompiler/csharp/annotations.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"
#include "dotnet/decompiler/csharp/transforms/custom_patterns.hpp"
#include "dotnet/decompiler/csharp/transforms/pattern_arena.hpp"
#include "dotnet/decompiler/csharp/transforms/transform_context.hpp"
#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/il/instructions.hpp"
#include "dotnet/decompiler/il/transforms/il_inlining.hpp"
#include "dotnet/decompiler/semantics/invocation_resolve_result.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"
#include "dotnet/decompiler/util/decimal.hpp"

#include "dotnet/decompiler/csharp/transforms/add_checked_blocks.hpp"

namespace cb::dotnet::decompiler::csharp::transforms {

using namespace syntax;
using namespace syntax::pattern_matching;
using typesystem::IMethod;
using typesystem::IType;
using typesystem::KnownTypeCode;
using typesystem::TypeKind;

namespace {

// AddCheckedBlocks.CheckedAnnotation / UncheckedAnnotation.
void add_checked_annotation(AstNode* node) {
    node->add_annotation(AddCheckedBlocks::checked_annotation());
}

void add_unchecked_annotation(AstNode* node) {
    node->add_annotation(AddCheckedBlocks::unchecked_annotation());
}

// typeof(T).TypeHandle, or __reftype(x).TypeHandle
MemberReferenceExpression* type_handle_on_type_of_pattern() {
    static MemberReferenceExpression* const pattern = build_pattern([] {
        Choice* choice = make<Choice>();
        choice->add(make<TypeOfExpression>(AstType::from_pattern(make<AnyNode>())));
        choice->add(make<UndocumentedExpression>(UndocumentedExpressionType::RefType, std::vector<Expression*>{Expression::from_pattern(make<AnyNode>())}));
        return make<MemberReferenceExpression>(Expression::from_pattern(choice), "TypeHandle");
    });
    return pattern;
}

// target.ToString() and target?.ToString()
Pattern* to_string_call_pattern() {
    static Pattern* const pattern = build_pattern([]() -> Pattern* {
        Choice* choice = make<Choice>();
        // target.ToString()
        choice->add(with_name(make<InvocationExpression>(make<MemberReferenceExpression>(Expression::from_pattern(make<AnyNode>("target")), "ToString")), "call"));
        // target?.ToString()
        choice->add(with_name(
            make<UnaryOperatorExpression>(
                UnaryOperatorType::NullConditionalRewrap,
                with_name(make<InvocationExpression>(make<MemberReferenceExpression>(
                              make<UnaryOperatorExpression>(UnaryOperatorType::NullConditional, Expression::from_pattern(make<AnyNode>("target"))), "ToString")),
                          "call")),
            "nullConditional"));
        return choice;
    });
    return pattern;
}

// (MethodInfo)MethodBase.GetMethodFromHandle(ldtoken(method).MethodHandle, typeof(declaringType).TypeHandle)
Expression* get_method_or_constructor_from_handle_pattern() {
    static Expression* const pattern = build_pattern([] {
        Choice* type_choice = make<Choice>();
        type_choice->add(make<TypePattern>("System.Reflection", "MethodInfo"));
        type_choice->add(make<TypePattern>("System.Reflection", "ConstructorInfo"));
        Expression* method_base = make<TypeReferenceExpression>(to_type(make<TypePattern>("System.Reflection", "MethodBase")));
        Expression* ldtoken_node = Expression::from_pattern(make<NamedNode>(
            "ldtokenNode", make<MemberReferenceExpression>(to_expression(make<LdTokenPattern>("method")), "MethodHandle")));
        Expression* declaring_type_handle = Expression::from_pattern(
            make<OptionalNode>(make<MemberReferenceExpression>(make<TypeOfExpression>(AstType::from_pattern(make<AnyNode>("declaringType"))), "TypeHandle")));
        return static_cast<Expression*>(make<CastExpression>(
            AstType::from_pattern(type_choice),
            make<InvocationExpression>(make<MemberReferenceExpression>(method_base, "GetMethodFromHandle"),
                                       std::vector<Expression*>{ldtoken_node, declaring_type_handle})));
    });
    return pattern;
}

}  // namespace

void ReplaceMethodCallsWithOperators::visit_invocation_expression(InvocationExpression& invocation_expression) {
    DepthFirstAstVisitor::visit_invocation_expression(invocation_expression);
    process_invocation_expression(&invocation_expression);
}

void ReplaceMethodCallsWithOperators::process_invocation_expression(InvocationExpression* invocation_expression) {
    const auto* method = dynamic_cast<const IMethod*>(get_symbol(invocation_expression));
    if (method == nullptr) return;
    std::vector<Expression*> arguments = invocation_expression->arguments().to_vector();
    // Reduce "String.Concat(a, b)" to "a + b"
    if (is_string_concat(method) && context_->settings.string_concat) {
        if (arguments.size() == 1) {
            auto* ace = as<ArrayCreateExpression>(arguments[0]);
            if (ace != nullptr && ace->initializer() != nullptr && method->parameters().size() == 1 &&
                method->parameters()[0]->type()->kind() == TypeKind::Array) {
                arguments = ace->initializer()->elements().to_vector();
            }
        }
        if (!check_arguments_for_string_concat(arguments)) return;
        bool is_in_expression_tree = false;
        for (AstNode* ancestor : invocation_expression->ancestors()) {
            auto* lambda = as<LambdaExpression>(ancestor);
            if (lambda == nullptr) continue;
            auto* function = lambda->annotation<il::ILFunction>();
            if (function != nullptr && function_kind_of(context_->l2, *function) == il::ILFunctionKind::ExpressionTree) {
                is_in_expression_tree = true;
                break;
            }
        }
        context_->step("Replace String.Concat with +", invocation_expression);
        Expression* arg0 = detach(arguments[0]);
        Expression* arg1 = detach(arguments[1]);
        if (!is_in_expression_tree) {
            arg1 = detach(remove_redundant_to_string_in_concat(arg1, method, /*is_last_argument*/ arguments.size() == 2));
            if (typesystem::is_known_type(get_resolve_result(arg1)->type(), KnownTypeCode::String)) {
                arg0 = detach(remove_redundant_to_string_in_concat(arg0, method, /*is_last_argument*/ false));
            }
        }
        BinaryOperatorExpression* expr = make<BinaryOperatorExpression>(arg0, BinaryOperatorType::Add, arg1);
        for (std::size_t i = 2; i < arguments.size(); i++) {
            Expression* arg = detach(arguments[i]);
            if (!is_in_expression_tree) {
                arg = detach(remove_redundant_to_string_in_concat(arg, method, /*is_last_argument*/ i == arguments.size() - 1));
            }
            expr = make<BinaryOperatorExpression>(expr, BinaryOperatorType::Add, arg);
        }
        copy_annotations_from(expr, invocation_expression);
        invocation_expression->replace_with(expr);
        context_->end_step(expr);
        return;
    }
    std::string full_name = method->full_name();
    if (full_name == "System.Type.GetTypeFromHandle") {
        if (arguments.size() == 1) {
            if (is_match(type_handle_on_type_of_pattern(), arguments[0])) {
                context_->step("Replace GetTypeFromHandle with typeof", invocation_expression);
                Expression* target = static_cast<MemberReferenceExpression*>(arguments[0])->target();
                copy_instructions_from(target, invocation_expression);
                invocation_expression->replace_with(target);
                context_->end_step(target);
                return;
            }
        }
    } else if (full_name == "System.Activator.CreateInstance") {
        if (context_->settings.use_object_creation_of_generic_type_parameter && arguments.empty() && method->type_arguments().size() == 1 &&
            is_instantiable_type_parameter(method->type_arguments()[0])) {
            context_->step("Replace Activator.CreateInstance with new", invocation_expression);
            auto* object_create = make<ObjectCreateExpression>(context_->type_system_ast_builder.convert_type(method->type_arguments()[0]));
            invocation_expression->replace_with(object_create);
            context_->end_step(object_create);
        }
    } else if (full_name == "System.Runtime.CompilerServices.RuntimeHelpers.GetSubArray") {
        if (arguments.size() == 2 && context_->settings.ranges) {
            context_->step("Replace RuntimeHelpers.GetSubArray with range indexer", invocation_expression);
            Expression* array = detach(arguments[0]);
            Expression* range = detach(arguments[1]);
            auto* slicing = make<IndexerExpression>(array, std::vector<Expression*>{range});
            copy_annotations_from(slicing, invocation_expression);
            invocation_expression->replace_with(slicing);
            context_->end_step(slicing);
        }
    }
    bool is_checked = false;
    std::optional<BinaryOperatorType> bop = get_binary_operator_type_from_metadata_name(method->name(), is_checked, context_->settings);
    if (bop && arguments.size() == 2) {
        context_->step("Replace operator method with binary operator", invocation_expression);
        invocation_expression->arguments().clear();  // detach arguments from invocationExpression
        if (is_checked) {
            add_checked_annotation(invocation_expression);
        } else if (has_checked_equivalent(method)) {
            add_unchecked_annotation(invocation_expression);
        }
        Expression* left = unwrap_in_direction_expression(arguments[0]);
        Expression* right = unwrap_in_direction_expression(arguments[1]);
        auto* binary_operator = copy_annotations_from(make<BinaryOperatorExpression>(left, *bop, right), invocation_expression);
        invocation_expression->replace_with(binary_operator);
        context_->end_step(binary_operator);
        return;
    }
    std::optional<UnaryOperatorType> uop = get_unary_operator_type_from_metadata_name(method->name(), is_checked, context_->settings);
    if (uop && arguments.size() == 1) {
        if (is_checked) {
            add_checked_annotation(invocation_expression);
        } else if (has_checked_equivalent(method)) {
            add_unchecked_annotation(invocation_expression);
        }
        if (*uop == UnaryOperatorType::Increment || *uop == UnaryOperatorType::Decrement) {
            // `op_Increment(a)` is not equivalent to `++a`, because it doesn't assign the incremented value to a.
            if (typesystem::is_known_type(method->declaring_type(), KnownTypeCode::Decimal)) {
                context_->step("Replace decimal increment method with arithmetic", invocation_expression);
                // Legacy csc optimizes "d + 1m" to "op_Increment(d)", so reverse that optimization here:
                Expression* operand = detach(unwrap_in_direction_expression(arguments[0]));
                auto* one = make<PrimitiveExpression>(typesystem::ConstantValue(util::Decimal(1, 0, 0, false, 0)));
                auto* arithmetic = copy_annotations_from(
                    make<BinaryOperatorExpression>(operand, (*uop == UnaryOperatorType::Increment ? BinaryOperatorType::Add : BinaryOperatorType::Subtract), one),
                    invocation_expression);
                invocation_expression->replace_with(arithmetic);
                context_->end_step(arithmetic);
            }
        } else {
            context_->step("Replace operator method with unary operator", invocation_expression);
            arguments[0]->remove();  // detach argument
            auto* unary_operator = copy_annotations_from(make<UnaryOperatorExpression>(*uop, unwrap_in_direction_expression(arguments[0])), invocation_expression);
            invocation_expression->replace_with(unary_operator);
            context_->end_step(unary_operator);
        }
        return;
    }
    if ((method->name() == "op_Explicit" || method->name() == "op_CheckedExplicit") && arguments.size() == 1) {
        context_->step("Replace conversion operator method with cast", invocation_expression);
        arguments[0]->remove();  // detach argument
        if (method->name() == "op_CheckedExplicit") {
            add_checked_annotation(invocation_expression);
        } else if (has_checked_equivalent(method)) {
            add_unchecked_annotation(invocation_expression);
        }
        AstType* cast_type = context_->type_system_ast_builder.convert_type(method->return_type());
        auto* cast = copy_annotations_from(make<CastExpression>(cast_type, unwrap_in_direction_expression(arguments[0])), invocation_expression);
        invocation_expression->replace_with(cast);
        context_->end_step(cast);
        return;
    }
    if (method->name() == "op_True" && arguments.size() == 1 && invocation_expression->slot() != nullptr &&
        invocation_expression->slot()->kind() == &Slots::condition) {
        context_->step("Remove op_True from condition", invocation_expression);
        Expression* condition = unwrap_in_direction_expression(arguments[0]);
        invocation_expression->replace_with(condition);
        context_->end_step(condition);
        return;
    }
}

bool ReplaceMethodCallsWithOperators::has_checked_equivalent(const IMethod* method) {
    std::string name = method->name();
    if (name.starts_with("op_")) name = "op_Checked" + name.substr(3);
    return !method->declaring_type()->get_methods([&](const IMethod* m) { return m->is_operator() && m->name() == name; }).empty();
}

bool ReplaceMethodCallsWithOperators::is_instantiable_type_parameter(const IType* type) {
    const auto* tp = dynamic_cast<const typesystem::ITypeParameter*>(type);
    return tp != nullptr && tp->has_default_constructor_constraint();
}

bool ReplaceMethodCallsWithOperators::check_arguments_for_string_concat(const std::vector<Expression*>& arguments) {
    if (arguments.size() < 2) return false;
    for (Expression* arg : arguments)
        if (is<NamedArgumentExpression>(arg)) return false;
    // The evaluation order when the object.ToString() calls happen is a mess:
    // The C# spec says the evaluation for order for each individual string + should be:
    //   * evaluate left argument
    //   * evaluate right argument
    //   * call ToString() on object argument
    // What actually happens pre-VS2019.3:
    //   * evaluate all arguments in chain of + operators from left to right
    //   * call ToString() on all object arguments from left to right
    // What happens in VS2019.3:
    //   * for each argument in chain of + operators fom left to right:
    //       * evaluate argument
    //       * call ToString() on object argument
    // See https://github.com/dotnet/roslyn/issues/38641 for details.
    // To ensure the decompiled code's behavior matches the original IL behavior, no matter which compiler is used to recompile it, we
    // require that all implicit ToString() calls except for the last are free of side effects.
    for (std::size_t i = 0; i + 1 < arguments.size(); i++) {
        if (!to_string_is_known_effect_free(get_resolve_result(arguments[i])->type())) return false;
    }
    for (Expression* arg : arguments) {
        semantics::ResolveResult* rr = get_resolve_result(arg);
        if (auto* irr = dynamic_cast<semantics::InvocationResolveResult*>(rr); irr != nullptr && is_string_concat(irr->member())) {
            // Roslyn + mcs also flatten nested string.Concat() invocations within a operator+ use, which causes it to use the incorrect
            // evaluation order despite the code using an explicit string.Concat() call.
            // This problem is avoided if the outer call remains string.Concat() as well.
            return false;
        }
        if (rr->type()->is_by_ref_like()) {
            // ref structs cannot be converted to object for use with +
            return false;
        }
    }
    // One of the first two arguments must be string, otherwise the + operator won't resolve to a string concatenation.
    return typesystem::is_known_type(get_resolve_result(arguments[0])->type(), KnownTypeCode::String) ||
           typesystem::is_known_type(get_resolve_result(arguments[1])->type(), KnownTypeCode::String);
}

bool ReplaceMethodCallsWithOperators::is_string_concat(const typesystem::IParameterizedMember* member) {
    const auto* method = dynamic_cast<const IMethod*>(member);
    return method != nullptr && method->name() == "Concat" && typesystem::is_known_type(method->declaring_type(), KnownTypeCode::String);
}

Expression* ReplaceMethodCallsWithOperators::remove_redundant_to_string_in_concat(Expression* expr, const IMethod* concat_method, bool is_last_argument) {
    Match m = match(to_string_call_pattern(), expr);
    if (!m.success()) return expr;
    auto is_string_parameter = [](const typesystem::IParameter* p) {
        const IType* ty = p->type();
        if (p->is_params() && ty->kind() == TypeKind::Array) ty = dynamic_cast<const typesystem::TypeWithElementType*>(ty)->element_type();
        return typesystem::is_known_type(ty, KnownTypeCode::String);
    };
    for (const typesystem::IParameter* p : concat_method->parameters()) {
        if (!is_string_parameter(p)) {
            // If we're using a string.Concat() overload involving object parameters, string.Concat() itself already calls ToString() so the C#
            // compiler shouldn't generate additional ToString() calls in this case.
            return expr;
        }
    }
    const auto* to_string_method = dynamic_cast<const IMethod*>(get_symbol(single<Expression>(m, "call")));
    Expression* target = single<Expression>(m, "target");
    const IType* type = get_resolve_result(target)->type();
    if (type->is_by_ref_like()) {
        // ref structs cannot be converted to object for use with +
        return expr;
    }
    if (!(is_last_argument || to_string_is_known_effect_free(type))) {
        // ToString() order of evaluation matters, see check_arguments_for_string_concat().
        return expr;
    }
    if (type->is_reference_type() != false && !m.has("nullConditional")) {
        // ToString() might throw NullReferenceException, but the builtin operator+ doesn't.
        return expr;
    }
    if (!to_string_is_known_effect_free(type) && to_string_method != nullptr &&
        il::transforms::ILInlining::method_requires_copy_for_readonly_lvalue(to_string_method)) {
        // ToString() on a struct may mutate the struct.
        // For operator+ the C# compiler creates a temporary copy before implicitly calling ToString(), whereas an explicit ToString() call
        // would mutate the original lvalue. So we can't remove the compiler-generated ToString() call in cases where this might make a
        // difference.
        return expr;
    }
    // All checks succeeded, we can eliminate the ToString() call. The C# compiler will generate an equivalent call if the code is recompiled.
    return target;
}

bool ReplaceMethodCallsWithOperators::to_string_is_known_effect_free(const IType* type) {
    type = typesystem::nullable_type::get_underlying_type(type);
    const typesystem::ITypeDefinition* definition = type->get_definition();
    if (definition == nullptr) return false;
    switch (definition->known_type_code()) {
        case KnownTypeCode::Boolean:
        case KnownTypeCode::Char:
        case KnownTypeCode::SByte:
        case KnownTypeCode::Byte:
        case KnownTypeCode::Int16:
        case KnownTypeCode::UInt16:
        case KnownTypeCode::Int32:
        case KnownTypeCode::UInt32:
        case KnownTypeCode::Int64:
        case KnownTypeCode::UInt64:
        case KnownTypeCode::Single:
        case KnownTypeCode::Double:
        case KnownTypeCode::Decimal:
        case KnownTypeCode::IntPtr:
        case KnownTypeCode::UIntPtr:
        case KnownTypeCode::String:
            return true;
        default:
            return false;
    }
}

std::optional<BinaryOperatorType> ReplaceMethodCallsWithOperators::get_binary_operator_type_from_metadata_name(const std::string& name, bool& is_checked,
                                                                                                                const DecompilerSettings& settings) {
    is_checked = false;
    if (name == "op_Addition") return BinaryOperatorType::Add;
    if (name == "op_Subtraction") return BinaryOperatorType::Subtract;
    if (name == "op_Multiply") return BinaryOperatorType::Multiply;
    if (name == "op_Division") return BinaryOperatorType::Divide;
    if (name == "op_CheckedAddition" && settings.checked_operators) {
        is_checked = true;
        return BinaryOperatorType::Add;
    }
    if (name == "op_CheckedSubtraction" && settings.checked_operators) {
        is_checked = true;
        return BinaryOperatorType::Subtract;
    }
    if (name == "op_CheckedMultiply" && settings.checked_operators) {
        is_checked = true;
        return BinaryOperatorType::Multiply;
    }
    if (name == "op_CheckedDivision" && settings.checked_operators) {
        is_checked = true;
        return BinaryOperatorType::Divide;
    }
    if (name == "op_Modulus") return BinaryOperatorType::Modulus;
    if (name == "op_BitwiseAnd") return BinaryOperatorType::BitwiseAnd;
    if (name == "op_BitwiseOr") return BinaryOperatorType::BitwiseOr;
    if (name == "op_ExclusiveOr") return BinaryOperatorType::ExclusiveOr;
    if (name == "op_LeftShift") return BinaryOperatorType::ShiftLeft;
    if (name == "op_RightShift") return BinaryOperatorType::ShiftRight;
    if (name == "op_UnsignedRightShift" && settings.unsigned_right_shift) return BinaryOperatorType::UnsignedShiftRight;
    if (name == "op_Equality") return BinaryOperatorType::Equality;
    if (name == "op_Inequality") return BinaryOperatorType::InEquality;
    if (name == "op_LessThan") return BinaryOperatorType::LessThan;
    if (name == "op_LessThanOrEqual") return BinaryOperatorType::LessThanOrEqual;
    if (name == "op_GreaterThan") return BinaryOperatorType::GreaterThan;
    if (name == "op_GreaterThanOrEqual") return BinaryOperatorType::GreaterThanOrEqual;
    return std::nullopt;
}

std::optional<UnaryOperatorType> ReplaceMethodCallsWithOperators::get_unary_operator_type_from_metadata_name(const std::string& name, bool& is_checked,
                                                                                                              const DecompilerSettings& settings) {
    is_checked = false;
    if (name == "op_LogicalNot") return UnaryOperatorType::Not;
    if (name == "op_OnesComplement") return UnaryOperatorType::BitNot;
    if (name == "op_UnaryNegation") return UnaryOperatorType::Minus;
    if (name == "op_CheckedUnaryNegation" && settings.checked_operators) {
        is_checked = true;
        return UnaryOperatorType::Minus;
    }
    if (name == "op_UnaryPlus") return UnaryOperatorType::Plus;
    if (name == "op_Increment") return UnaryOperatorType::Increment;
    if (name == "op_Decrement") return UnaryOperatorType::Decrement;
    if (name == "op_CheckedIncrement" && settings.checked_operators) {
        is_checked = true;
        return UnaryOperatorType::Increment;
    }
    if (name == "op_CheckedDecrement" && settings.checked_operators) {
        is_checked = true;
        return UnaryOperatorType::Decrement;
    }
    return std::nullopt;
}

void ReplaceMethodCallsWithOperators::visit_cast_expression(CastExpression& cast_expression) {
    DepthFirstAstVisitor::visit_cast_expression(cast_expression);
    // Handle methodof
    Match m = match(get_method_or_constructor_from_handle_pattern(), &cast_expression);
    if (m.success()) {
        AstNode* method_node = single<AstNode>(m, "method");
        const auto* method = dynamic_cast<const IMethod*>(get_symbol(method_node));
        if (m.has("declaringType") && method != nullptr) {
            AstType* declaring_type = detach(single<AstType>(m, "declaringType"));
            Expression* new_node = make<MemberReferenceExpression>(make<TypeReferenceExpression>(declaring_type), method->name());
            std::vector<Expression*> parameter_types;
            for (const typesystem::IParameter* p : method->parameters())
                parameter_types.push_back(make<TypeReferenceExpression>(context_->type_system_ast_builder.convert_type(p->type())));
            new_node = make<InvocationExpression>(new_node, parameter_types);
            method_node->replace_with(new_node);
        }
        Expression* ldtoken_node = single<Expression>(m, "ldtokenNode");
        cast_expression.replace_with(copy_annotations_from(ldtoken_node, &cast_expression));
    }
}

void ReplaceMethodCallsWithOperators::run(AstNode* root_node, TransformContext& context) {
    context_ = &context;
    try {
        root_node->accept_visitor(static_cast<IAstVisitor&>(*this));
    } catch (...) {
        context_ = nullptr;
        throw;
    }
    context_ = nullptr;
}

}  // namespace cb::dotnet::decompiler::csharp::transforms
