// dotnet/decompiler/csharp/call_builder.cpp -- see call_builder.hpp.
#include "dotnet/decompiler/csharp/call_builder.hpp"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <stdexcept>
#include <unordered_set>

#include "dotnet/decompiler/csharp/expression_builder.hpp"
#include "dotnet/decompiler/csharp/resolver/csharp_conversions.hpp"
#include "dotnet/decompiler/csharp/resolver/lambda_resolve_result.hpp"
#include "dotnet/decompiler/csharp/resolver/log.hpp"
#include "dotnet/decompiler/csharp/resolver/member_lookup.hpp"
#include "dotnet/decompiler/csharp/resolver/name_lookup_mode.hpp"
#include "dotnet/decompiler/csharp/resolver/overload_resolution.hpp"
#include "dotnet/decompiler/csharp/statement_builder.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax_extensions.hpp"
#include "dotnet/decompiler/il/instruction_output_il.hpp"
#include "dotnet/decompiler/il/transforms/assign_variable_names.hpp"
#include "dotnet/decompiler/il/transforms/delegate_construction.hpp"
#include "dotnet/decompiler/il/transforms/il_inlining.hpp"
#include "dotnet/decompiler/il/transforms/tuple_transform.hpp"
#include "dotnet/decompiler/nr_extensions.hpp"
#include "dotnet/decompiler/semantics/array_create_resolve_result.hpp"
#include "dotnet/decompiler/semantics/by_reference_resolve_result.hpp"
#include "dotnet/decompiler/semantics/constant_resolve_result.hpp"
#include "dotnet/decompiler/semantics/conversion_resolve_result.hpp"
#include "dotnet/decompiler/semantics/interpolated_string_resolve_result.hpp"
#include "dotnet/decompiler/semantics/member_resolve_result.hpp"
#include "dotnet/decompiler/semantics/out_var_resolve_result.hpp"
#include "dotnet/decompiler/semantics/tuple_resolve_result.hpp"
#include "dotnet/decompiler/semantics/type_resolve_result.hpp"
#include "dotnet/decompiler/typesystem/inheritance_helper.hpp"
#include "dotnet/decompiler/typesystem/known_attributes.hpp"
#include "dotnet/decompiler/typesystem/metadata_entities.hpp"
#include "dotnet/decompiler/typesystem/synthetic_range_index_accessor.hpp"
#include "dotnet/decompiler/typesystem/tuple_type.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"
#include "dotnet/decompiler/util/char_info.hpp"

namespace cb::dotnet::decompiler::csharp {

using il::Block;
using il::Call;
using il::CallInstruction;
using il::NewObj;
using il::OpCode;
using resolver::CSharpConversions;
using resolver::CSharpInvocationResolveResult;
using resolver::MethodGroupResolveResult;
using resolver::MethodListWithDeclaringType;
using resolver::OverloadResolution;
using resolver::OverloadResolutionErrors;
using semantics::ArrayCreateResolveResult;
using semantics::ByReferenceResolveResult;
using semantics::Conversion;
using semantics::ConstantResolveResult;
using semantics::ConversionResolveResult;
using semantics::InitializedObjectResolveResult;
using semantics::InterpolatedStringResolveResult;
using semantics::MemberResolveResult;
using semantics::OutVarResolveResult;
using semantics::TupleResolveResult;
using semantics::TypeResolveResult;
using syntax::ArrayCreateExpression;
using syntax::AssignmentExpression;
using syntax::AssignmentOperatorType;
using syntax::AstType;
using syntax::BaseReferenceExpression;
using syntax::BinaryOperatorExpression;
using syntax::BinaryOperatorType;
using syntax::CastExpression;
using syntax::Comment;
using syntax::CommentType;
using syntax::ConditionalExpression;
using syntax::DirectionExpression;
using syntax::FieldDirection;
using syntax::IndexerExpression;
using syntax::InvocationExpression;
using syntax::MemberReferenceExpression;
using syntax::NamedArgumentExpression;
using syntax::NullReferenceExpression;
using syntax::ObjectCreateExpression;
using syntax::PrimitiveExpression;
using syntax::ThisReferenceExpression;
using syntax::TupleExpression;
using syntax::UnaryOperatorExpression;
using syntax::UnaryOperatorType;
using typesystem::IMember;
using typesystem::IMethod;
using typesystem::IParameter;
using typesystem::IParameterizedMember;
using typesystem::KnownTypeCode;
using typesystem::ReferenceKind;
using typesystem::SpecialType;
using typesystem::SymbolKind;
using typesystem::TypeKind;
using typesystem::is_known_type;

namespace {

// ---- small helpers -----------------------------------------------------------------------------------------------------------

const typesystem::ByReferenceType* by_reference_type_of(const IType* type) { return dynamic_cast<const typesystem::ByReferenceType*>(type); }

// int.TryParse: optional white space, an optional sign, digits, optional white space.
bool try_parse_int(const std::string& text, int* value) {
    std::size_t begin = 0;
    std::size_t end = text.size();
    while (begin < end && std::isspace(static_cast<unsigned char>(text[begin]))) begin++;
    while (end > begin && std::isspace(static_cast<unsigned char>(text[end - 1]))) end--;
    if (begin == end) return false;
    bool negative = false;
    if (text[begin] == '+' || text[begin] == '-') {
        negative = text[begin] == '-';
        begin++;
    }
    if (begin == end) return false;
    long long result = 0;
    for (std::size_t i = begin; i < end; i++) {
        if (text[i] < '0' || text[i] > '9') return false;
        result = result * 10 + (text[i] - '0');
        if (result > 2147483648LL) return false;
    }
    if (negative) result = -result;
    if (result > 2147483647LL || result < -2147483648LL) return false;
    *value = static_cast<int>(result);
    return true;
}

// string.Split(separators, count): at most `count` parts, split at the separators from the left.
std::vector<std::string> split_at_most(const std::string& text, const std::string& separators, int count) {
    std::vector<std::string> parts;
    std::size_t start = 0;
    while (static_cast<int>(parts.size()) < count - 1) {
        std::size_t pos = text.find_first_of(separators, start);
        if (pos == std::string::npos) break;
        parts.push_back(text.substr(start, pos - start));
        start = pos + 1;
    }
    parts.push_back(text.substr(start));
    return parts;
}

}  // namespace

// ---- ArgumentList ----------------------------------------------------------------------------------------------------------------

std::optional<CallBuilder::ArgumentNames> CallBuilder::ArgumentList::get_argument_names(int skip_count) {
    // (upstream fills the array it holds in place when there is one)
    bool any_parameter_name_empty = std::any_of(parameter_names.begin(), parameter_names.end(), [](const std::string& n) { return n.empty(); });
    if (add_names_to_primitive_values && is_primitive_value.any() && !is_expanded_form && !any_parameter_name_empty) {
        assert(skip_count == 0);
        (void)skip_count;
        if (!argument_names.has_value()) {
            // a new array that is only for this call's result
            ArgumentNames local(arguments.size());
            for (int i = 0; i < length(); i++) {
                if (is_primitive_value[i] && !local[i].has_value()) local[i] = parameter_names[i];
            }
            return local;
        }
        for (int i = 0; i < length(); i++) {
            if (is_primitive_value[i] && !(*argument_names)[i].has_value()) (*argument_names)[i] = parameter_names[i];
        }
    }
    return argument_names;
}

std::vector<ResolveResult*> CallBuilder::ArgumentList::get_argument_resolve_results(int skip_count) const {
    std::vector<ResolveResult*> all;
    for (int index = 0; index < length(); index++) {
        const TranslatedExpression& expression = arguments[static_cast<std::size_t>(index)];
        const IParameter* param = expected_parameters[static_cast<std::size_t>(index)];
        if (use_implicitly_typed_out && param->reference_kind() == ReferenceKind::Out) {
            if (auto* brt = dynamic_cast<const typesystem::ByReferenceType*>(expression.type())) {
                all.push_back(make<OutVarResolveResult>(brt->element_type()));
                continue;
            }
        }
        all.push_back(expression.resolve_result);
    }
    std::vector<ResolveResult*> result;
    int actual = get_actual_argument_count();
    for (int i = skip_count; i < static_cast<int>(all.size()) && static_cast<int>(result.size()) < actual; i++) result.push_back(all[static_cast<std::size_t>(i)]);
    return result;
}

std::vector<ResolveResult*> CallBuilder::ArgumentList::get_argument_resolve_results_direct(int skip_count) const {
    std::vector<ResolveResult*> result;
    int actual = get_actual_argument_count();
    for (int i = skip_count; i < length() && static_cast<int>(result.size()) < actual; i++) result.push_back(arguments[static_cast<std::size_t>(i)].resolve_result);
    return result;
}

std::vector<Expression*> CallBuilder::ArgumentList::get_argument_expressions(int skip_count) {
    std::optional<ArgumentNames> names = get_argument_names(skip_count);
    int argument_count = get_actual_argument_count();
    bool implicitly_typed_out = use_implicitly_typed_out;
    auto add_annotations = [implicitly_typed_out](Expression* expression) {
        if (!implicitly_typed_out) return expression;
        auto* brrr = dynamic_cast<ByReferenceResolveResult*>(get_resolve_result(expression));
        if (brrr != nullptr && brrr->reference_kind() == ReferenceKind::Out) {
            expression->add_annotation(UseImplicitlyTypedOutAnnotation::instance());
        }
        return expression;
    };
    std::vector<Expression*> result;
    if (!names.has_value()) {
        for (int i = skip_count; i < length() && static_cast<int>(result.size()) < argument_count; i++)
            result.push_back(add_annotations(arguments[static_cast<std::size_t>(i)].expression));
    } else {
        assert(skip_count == 0);
        for (int i = 0; i < length() && i < argument_count && i < static_cast<int>(names->size()); i++) {
            const std::optional<std::string>& name = (*names)[static_cast<std::size_t>(i)];
            Expression* expression = arguments[static_cast<std::size_t>(i)].expression;
            if (!name.has_value())
                result.push_back(add_annotations(expression));
            else
                result.push_back(make<NamedArgumentExpression>(*name, add_annotations(expression)));
        }
    }
    return result;
}

bool CallBuilder::ArgumentList::can_infer_anonymous_type_property_names_from_arguments() const {
    for (int i = 0; i < length(); i++) {
        std::optional<std::string> inferred_name;
        Expression* expression = arguments[static_cast<std::size_t>(i)].expression;
        if (auto* identifier = syntax::as<syntax::IdentifierExpression>(expression))
            inferred_name = identifier->identifier();
        else if (auto* member = syntax::as<MemberReferenceExpression>(expression))
            inferred_name = member->member_name();
        if (!inferred_name.has_value() || *inferred_name != expected_parameters[static_cast<std::size_t>(i)]->name()) return false;
    }
    return true;
}

void CallBuilder::ArgumentList::check_no_named_or_optional_arguments() const {
    assert(!argument_to_parameter_map.has_value() && !argument_names.has_value() && first_optional_argument_index < 0);
}

const typesystem::IParameter* CallBuilder::make_default_parameter(const IType* type) {
    owned_parameters_.push_back(std::make_unique<typesystem::DefaultParameter>(type, std::string()));
    return owned_parameters_.back().get();
}

// ---- CallBuilder -------------------------------------------------------------------------------------------------------------------

CallBuilder::CallBuilder(ExpressionBuilder& expression_builder, const typesystem::ICompilation& type_system, const DecompilerSettings& settings)
    : settings_(settings), expression_builder_(expression_builder), resolver_(expression_builder.resolver), type_system_(type_system) {}

TranslatedExpression CallBuilder::build(CallInstruction* inst, const IType* type_hint) {
    if (auto* newobj = il::as<NewObj>(inst)) {
        const IMethod* delegate_method = nullptr;
        il::ILInstruction* delegate_target = nullptr;
        const IType* delegate_type = nullptr;
        if (il::transforms::DelegateConstruction::match_delegate_construction(newobj, &delegate_method, &delegate_target, &delegate_type)) {
            return handle_delegate_construction(newobj);
        }
    }
    std::vector<il::ILInstruction*> tuple_elements;
    if (settings_.tuple_types && il::as<NewObj>(inst) != nullptr &&
        il::transforms::TupleTransform::match_tuple_construction(il::as<NewObj>(inst), &tuple_elements) && tuple_elements.size() >= 2) {
        std::optional<std::vector<const IType*>> element_types = typesystem::TupleType::get_tuple_element_types(inst->method->declaring_type());
        std::vector<std::optional<std::string>> element_names;
        if (auto* tt = dynamic_cast<const typesystem::TupleType*>(type_hint)) element_names = tt->element_names();
        assert(element_types.has_value() && "MatchTupleConstruction should not succeed unless we got a valid tuple type.");
        assert(element_types->size() == tuple_elements.size());
        auto* tuple = make<TupleExpression>();
        std::vector<ResolveResult*> element_rrs;
        for (std::size_t index = 0; index < tuple_elements.size(); index++) {
            il::ILInstruction* element = tuple_elements[index];
            const IType* element_type = (*element_types)[index];
            TranslatedExpression translated_element =
                expression_builder_.translate(element, element_type).convert_to(element_type, expression_builder_, false, true);
            if (element_names.empty() || index >= element_names.size() || !element_names[index].has_value() || element_names[index]->empty()) {
                tuple->elements().add(translated_element.expression);
            } else {
                tuple->elements().add(make<NamedArgumentExpression>(*element_names[index], translated_element.expression));
            }
            element_rrs.push_back(translated_element.resolve_result);
        }
        const typesystem::ITypeDefinition* tuple_definition = inst->method->declaring_type()->get_definition();
        return with_rr(tuple, make<TupleResolveResult>(*expression_builder_.compilation, element_rrs, element_names,
                                                       tuple_definition != nullptr ? tuple_definition->parent_module() : nullptr))
            .with_il_instruction(inst);
    }
    std::vector<std::pair<il::ILInstruction*, KnownTypeCode>> operands;
    if (settings_.string_concat && is_span_based_string_concat(inst, &operands)) {
        return build_string_concat(inst->method, operands).with_il_instruction(inst);
    }
    TranslatedExpression result =
        build(inst->op_code, inst->method, inst->arguments().items(), std::nullopt, inst->constrained_to).with_il_instruction(inst);
    if (inst->is_tail) {
        // Surface the IL 'tail.' prefix as an inline marker, e.g. '/*tail.*/Callee(x)'.
        // F# emits tail calls pervasively, and the prefix is otherwise dropped entirely.
        result.expression->add_leading_trivia(make<Comment>("tail.", CommentType::MultiLine));
    }
    return result;
}

ExpressionWithResolveResult CallBuilder::build_string_concat(const IMethod* method,
                                                             const std::vector<std::pair<il::ILInstruction*, KnownTypeCode>>& operands) {
    const IType* type = type_system_.find_type(operands[0].second);
    ExpressionWithResolveResult result = expression_builder_.translate(operands[0].first, type).convert_to(type, expression_builder_);
    auto* rr = make<MemberResolveResult>(nullptr, method);

    for (std::size_t i = 1; i < operands.size(); i++) {
        type = type_system_.find_type(operands[i].second);
        TranslatedExpression expr = expression_builder_.translate(operands[i].first, type).convert_to(type, expression_builder_);
        result = with_rr(make<BinaryOperatorExpression>(result.expression, BinaryOperatorType::Add, expr.expression), rr);
    }

    return result;
}

bool CallBuilder::is_span_based_string_concat(CallInstruction* call, std::vector<std::pair<il::ILInstruction*, KnownTypeCode>>* operands) {
    operands->clear();

    if (!is_span_based_string_concat(call->method)) {
        return false;
    }

    std::optional<int> first_string_argument_index;

    for (il::ILInstruction* arg : call->arguments().items()) {
        if (auto* op_implicit = il::as<Call>(arg); op_implicit != nullptr && is_string_to_read_only_span_char_implicit_conversion(op_implicit->method)) {
            if (!first_string_argument_index.has_value()) first_string_argument_index = arg->child_index();
            assert(op_implicit->arguments().count() == 1);
            operands->push_back({op_implicit->arguments()[0], KnownTypeCode::String});
        } else if (auto* new_obj = il::as<NewObj>(arg);
                   new_obj != nullptr && new_obj->arguments().count() == 1 && il::as<il::AddressOf>(new_obj->arguments()[0]) != nullptr &&
                   il::transforms::ILInlining::is_read_only_span_char_ctor(new_obj->method)) {
            operands->push_back({il::as<il::AddressOf>(new_obj->arguments()[0])->value(), KnownTypeCode::Char});
        } else {
            return false;
        }
    }

    return call->arguments().count() >= 2 && first_string_argument_index.has_value() && *first_string_argument_index <= 1;
}

bool CallBuilder::is_span_based_string_concat(const IMethod* method) {
    if (!(method->name() == "Concat" && method->is_static())) {
        return false;
    }
    if (!is_known_type(method->declaring_type(), KnownTypeCode::String)) {
        return false;
    }

    for (const IParameter* p : method->parameters()) {
        if (!is_known_type(p->type(), KnownTypeCode::ReadOnlySpanOfT)) return false;
        if (!is_known_type(p->type()->type_arguments()[0], KnownTypeCode::Char)) return false;
    }

    return true;
}

bool CallBuilder::is_string_to_read_only_span_char_implicit_conversion(const IMethod* method) {
    return method->is_operator() && method->name() == "op_Implicit" && method->parameters().size() == 1 &&
           is_known_type(method->return_type(), KnownTypeCode::ReadOnlySpanOfT) &&
           is_known_type(method->return_type()->type_arguments()[0], KnownTypeCode::Char) &&
           is_known_type(method->parameters()[0]->type(), KnownTypeCode::String);
}

ExpressionWithResolveResult CallBuilder::build(OpCode call_op_code, const IMethod* method, const std::vector<il::ILInstruction*>& call_arguments,
                                               const std::optional<std::vector<int>>& argument_to_parameter_map, const IType* constrained_to) {
    if (method->is_explicit_interface_implementation() && call_op_code == OpCode::Call) {
        // Direct non-virtual call to explicit interface implementation.
        // This can't really be represented in C#, but at least in the case where
        // the class is sealed, we can equivalently call the interface member instead:
        std::vector<const IMember*> interface_members = method->explicitly_implemented_interface_members();
        const typesystem::ITypeDefinition* declaring_definition = method->declaring_type_definition();
        if (declaring_definition != nullptr && declaring_definition->kind() == TypeKind::Class && declaring_definition->is_sealed() &&
            interface_members.size() == 1) {
            method = dynamic_cast<const IMethod*>(interface_members[0]);
            call_op_code = OpCode::CallVirt;
        }
    }
    // Used for Call, CallVirt and NewObj
    ExpectedTargetDetails expected_target_details;
    expected_target_details.call_op_code = call_op_code;
    il::ILFunction* local_function = nullptr;
    if (method->is_local_function()) {
        local_function = expression_builder_.resolve_local_function(method);
        assert(local_function != nullptr);
    }
    TranslatedExpression target;
    if (call_op_code == OpCode::NewObj) {
        target = TranslatedExpression();  // no target
    } else if (local_function != nullptr) {
        auto* ide = make<syntax::IdentifierExpression>(local_function->name);
        if (!method->type_arguments().empty()) {
            for (const IType* type_argument : method->type_arguments()) ide->type_arguments().add(expression_builder_.convert_type(type_argument));
        }
        ide->add_annotation(local_function);
        target = without_il_instruction(ide).with_rr(to_method_group(method, local_function));
    } else {
        il::ILInstruction* this_arg = call_arguments.empty() ? nullptr : call_arguments[0];
        if (auto* ld_obj_if_ref = il::as<il::LdObjIfRef>(this_arg)) {
            assert(constrained_to != nullptr);
            this_arg = ld_obj_if_ref->target();
        }
        target = expression_builder_.translate_target(this_arg, call_op_code == OpCode::Call || method->is_constructor(), method->is_static(),
                                                      method->declaring_type(), constrained_to);
        auto* cast = syntax::as<CastExpression>(target.expression);
        auto* conversion = dynamic_cast<ConversionResolveResult*>(target.resolve_result);
        if (constrained_to == nullptr && cast != nullptr && conversion != nullptr && is_known_type(target.type(), KnownTypeCode::Object) &&
            conversion->conversion->is_boxing_conversion()) {
            // boxing conversion on call target?
            // let's see if we can make that implicit:
            target = target.unwrap_child(cast->expression());
            // we'll need to make sure the boxing effect is preserved
            expected_target_details.needs_boxing_conversion = true;
        }
    }

    int first_param_index = (method->is_static() || call_op_code == OpCode::NewObj) ? 0 : 1;
    assert(first_param_index == 0 || !argument_to_parameter_map.has_value() || (*argument_to_parameter_map)[0] == -1);

    ArgumentList argument_list =
        build_argument_list(expected_target_details, target.resolve_result, method, first_param_index, call_arguments, argument_to_parameter_map);

    if (local_function != nullptr) {
        return with_rr(make<InvocationExpression>(target.expression, argument_list.get_argument_expressions()),
                       make<CSharpInvocationResolveResult>(target.resolve_result, method, argument_list.get_argument_resolve_results(),
                                                           OverloadResolutionErrors::None, false, argument_list.is_expanded_form));
    }

    if (auto* var_arg_method = dynamic_cast<const typesystem::VarArgInstanceMethod*>(method)) {
        argument_list.first_optional_argument_index = -1;
        argument_list.add_names_to_primitive_values = false;
        argument_list.use_implicitly_typed_out = false;
        int regular_parameter_count = var_arg_method->regular_parameter_count();
        auto* arg_list_arg = make<syntax::UndocumentedExpression>(syntax::UndocumentedExpressionType::ArgList);
        int param_index = regular_parameter_count;
        assert(!argument_to_parameter_map.has_value() && !argument_list.argument_names.has_value());
        for (std::size_t i = static_cast<std::size_t>(regular_parameter_count); i < argument_list.arguments.size(); i++) {
            arg_list_arg->arguments().add(argument_list.arguments[i]
                                              .convert_to(argument_list.expected_parameters[static_cast<std::size_t>(param_index++)]->type(), expression_builder_)
                                              .expression);
        }
        auto* arg_list_rr = make<ResolveResult>(&SpecialType::arg_list());
        argument_list.arguments.resize(static_cast<std::size_t>(regular_parameter_count));
        argument_list.arguments.push_back(without_il_instruction(arg_list_arg).with_rr(arg_list_rr));
        method = var_arg_method->base_method();
        argument_list.expected_parameters = method->parameters();
    }

    if (settings_.ranges) {
        ExpressionWithResolveResult range_result;
        if (handle_range_construction(&range_result, call_op_code, method, target, argument_list)) {
            return range_result;
        }
    }

    if (call_op_code == OpCode::NewObj) {
        return handle_constructor_call(expected_target_details, target.resolve_result, method, argument_list);
    }

    if (method->name() == "Invoke" && method->declaring_type()->kind() == TypeKind::Delegate && !is_null_conditional(target.expression)) {
        return with_rr(make<InvocationExpression>(target.expression, argument_list.get_argument_expressions()),
                       make<CSharpInvocationResolveResult>(target.resolve_result, method, argument_list.get_argument_resolve_results(),
                                                           OverloadResolutionErrors::None, false, argument_list.is_expanded_form, true));
    }

    if (settings_.string_interpolation && is_interpolated_string_creation(method, argument_list)) {
        ExpressionWithResolveResult result = handle_string_interpolation(method, argument_list);
        if (result.expression != nullptr) return result;
    }

    std::size_t allowed_param_count = is_known_type(method->return_type(), KnownTypeCode::Void) ? 1 : 0;
    if (method->is_accessor() &&
        (method->accessor_owner()->symbol_kind() == SymbolKind::Indexer || argument_list.expected_parameters.size() == allowed_param_count)) {
        argument_list.check_no_named_or_optional_arguments();
        return handle_accessor_call(expected_target_details, method, target, argument_list.arguments, argument_list.argument_names);
    }

    if (is_delegate_equality_comparison(method, argument_list.arguments)) {
        argument_list.check_no_named_or_optional_arguments();
        return with_rr(handle_delegate_equality_comparison(method, argument_list.arguments),
                       make<CSharpInvocationResolveResult>(target.resolve_result, method, argument_list.get_argument_resolve_results(),
                                                           OverloadResolutionErrors::None, false, argument_list.is_expanded_form));
    }

    if (method->is_operator() && method->name() == "op_Implicit" && argument_list.length() == 1) {
        argument_list.check_no_named_or_optional_arguments();
        return handle_implicit_conversion(method, argument_list.arguments[0]);
    }

    if (settings_.inline_arrays && method->declaring_type()->full_name() == "<PrivateImplementationDetails>" &&
        (method->name() == "InlineArrayAsSpan" || method->name() == "InlineArrayAsReadOnlySpan") && argument_list.length() == 2) {
        argument_list.check_no_named_or_optional_arguments();
        const IType* array_type = method->type_arguments()[0];
        std::optional<int> array_length = typesystem::get_inline_array_length(array_type);
        const IType* array_element_type = typesystem::get_inline_array_element_type(array_type);
        TranslatedExpression argument = argument_list.arguments[0];
        TranslatedExpression span_length_expr = argument_list.arguments[1];
        const IType* target_type = method->return_type();
        const IType* span_type = type_system_.find_type(KnownTypeCode::SpanOfT);
        auto* arg_direction = syntax::as<DirectionExpression>(argument.expression);
        if (arg_direction != nullptr && (arg_direction->field_direction() == FieldDirection::In || arg_direction->field_direction() == FieldDirection::Ref)) {
            // `(TargetType)(in arg)` is invalid syntax.
            // Also, `f(in arg)` is invalid when there's an implicit conversion involved.
            argument = argument.unwrap_child(arg_direction->expression());
        }
        ConstantValue span_length_constant = span_length_expr.resolve_result->constant_value();
        auto* span_length_value = std::get_if<std::int32_t>(&span_length_constant);
        if (span_length_value != nullptr && array_length.has_value() && *span_length_value <= *array_length) {
            if (*span_length_value < *array_length) {
                argument = without_il_instruction(
                               make<IndexerExpression>(argument.expression, std::vector<Expression*>{make<BinaryOperatorExpression>(
                                                                                nullptr, BinaryOperatorType::Range, span_length_expr.expression)}))
                               .with_rr(make<ResolveResult>(typesystem::make_parameterized_type(span_type, {array_element_type})));
                if (is_known_type(target_type, KnownTypeCode::SpanOfT)) {
                    return argument;
                }
            }
            return with_rr(make<CastExpression>(expression_builder_.convert_type(target_type), argument.expression),
                           make<ConversionResolveResult>(target_type, argument.resolve_result, Conversion::inline_array_conversion()));
        }
    }

    if (settings_.lift_nullables && method->name() == "GetValueOrDefault" && is_known_type(method->declaring_type(), KnownTypeCode::NullableOfT) &&
        is_known_type(method->declaring_type()->type_arguments()[0], KnownTypeCode::Boolean) && argument_list.length() == 0) {
        argument_list.check_no_named_or_optional_arguments();
        return with_rr(make<BinaryOperatorExpression>(target.expression, BinaryOperatorType::Equality, make<PrimitiveExpression>(ConstantValue{true})),
                       make<CSharpInvocationResolveResult>(target.resolve_result, method, argument_list.get_argument_resolve_results(),
                                                           OverloadResolutionErrors::None, false, argument_list.is_expanded_form));
    }

    const IParameterizedMember* found_method = nullptr;
    CallTransformation transform =
        get_required_transformations_for_call(expected_target_details, method, target, argument_list, CallTransformation::All, &found_method);
    // GetRequiredTransformationsForCall always assigns foundMethod (the resolved overload or 'method').
    assert(found_method != nullptr);

    // Note: after this, 'method' and 'foundMethod' may differ,
    // but as far as allowed by IsAppropriateCallTarget().

    // Need to update list of parameter names, because foundMethod is different and thus might use different names.
    if (!member_equals(method, found_method) && argument_list.parameter_names.size() >= found_method->parameters().size()) {
        for (std::size_t i = 0; i < found_method->parameters().size(); i++) {
            argument_list.parameter_names[i] = found_method->parameters()[i]->name();
        }
    }

    Expression* target_expr = nullptr;
    std::string method_name = method->name();
    syntax::AstNodeCollection<AstType>* type_argument_list = nullptr;
    if ((transform & CallTransformation::NoNamedArgsForPrettiness) != CallTransformation::None) {
        argument_list.add_names_to_primitive_values = false;
    }
    if ((transform & CallTransformation::NoOptionalArgumentAllowed) != CallTransformation::None) {
        argument_list.first_optional_argument_index = -1;
    }
    if ((transform & CallTransformation::RequireTarget) != CallTransformation::None) {
        auto* member_reference = make<MemberReferenceExpression>(target.expression, method_name);
        target_expr = member_reference;
        type_argument_list = &member_reference->type_arguments();

        // HACK : convert this.Dispose() to ((IDisposable)this).Dispose(), if Dispose is an explicitly implemented interface method.
        // settings.AlwaysCastTargetsOfExplicitInterfaceImplementationCalls == true is used in Windows Forms' InitializeComponent methods.
        if (method->is_explicit_interface_implementation() &&
            (syntax::is<ThisReferenceExpression>(target.expression) || settings_.always_cast_targets_of_explicit_interface_implementation_calls)) {
            const IMember* interface_member = method->explicitly_implemented_interface_members()[0];
            auto* cast_expression =
                make<CastExpression>(expression_builder_.convert_type(interface_member->declaring_type()), syntax::detach(target.expression));
            method_name = interface_member->name();
            member_reference = make<MemberReferenceExpression>(cast_expression, method_name);
            target_expr = member_reference;
            type_argument_list = &member_reference->type_arguments();
        }
        if (constrained_to != nullptr) {
            if (auto* cast_target = syntax::as<CastExpression>(member_reference->target())) {
                cast_target->add_trailing_trivia(make<Comment>("cast due to constrained. prefix", CommentType::MultiLine));
            }
        }
    } else {
        auto* identifier = make<syntax::IdentifierExpression>(method_name);
        target_expr = identifier;
        type_argument_list = &identifier->type_arguments();
    }

    if ((transform & CallTransformation::RequireTypeArguments) != CallTransformation::None &&
        (!settings_.anonymous_types || !std::any_of(method->type_arguments().begin(), method->type_arguments().end(),
                                                    [](const IType* a) { return contains_anonymous_type(a); }))) {
        for (const IType* type_argument : method->type_arguments()) type_argument_list->add(expression_builder_.convert_type(type_argument));
    }
    return with_rr(make<InvocationExpression>(target_expr, argument_list.get_argument_expressions()),
                   make<CSharpInvocationResolveResult>(target.resolve_result, found_method, argument_list.get_argument_resolve_results_direct(),
                                                       OverloadResolutionErrors::None, false, argument_list.is_expanded_form));
}

ExpressionWithResolveResult CallBuilder::handle_string_interpolation(const IMethod* method, ArgumentList& argument_list) {
    std::string format;
    std::vector<StringInterpolationToken> tokens;
    if (!try_get_string_interpolation_tokens(argument_list, &format, &tokens)) return ExpressionWithResolveResult();

    std::vector<TranslatedExpression>& arguments = argument_list.arguments;
    std::vector<syntax::InterpolatedStringContent*> content;

    ArrayCreateExpression* ace = argument_list.length() == 2 ? syntax::as<ArrayCreateExpression>(arguments[1].expression) : nullptr;
    bool unpack_single_element_array = !argument_list.is_expanded_form && argument_list.length() == 2 && ace != nullptr &&
                                       ace->initializer() != nullptr && ace->initializer()->elements().count() == 1;

    auto unpack_single_element_array_of = [&](TranslatedExpression& argument) {
        if (!unpack_single_element_array) return;
        auto* array_creation = static_cast<ArrayCreateExpression*>(argument_list.arguments[1].expression);
        auto* array_creation_rr = static_cast<ArrayCreateResolveResult*>(argument_list.arguments[1].resolve_result);
        Expression* element = syntax::detach(array_creation->initializer()->elements()[0]);
        argument = TranslatedExpression(element, array_creation_rr->initializer_elements->front());
    };

    if (tokens.empty()) {
        return ExpressionWithResolveResult();
    }

    for (const StringInterpolationToken& token : tokens) {
        TranslatedExpression argument;
        switch (token.kind) {
            case TokenKind::String: content.push_back(make<syntax::InterpolatedStringText>(*token.format)); break;
            case TokenKind::Argument:
                argument = arguments[static_cast<std::size_t>(token.index + 1)];
                unpack_single_element_array_of(argument);
                content.push_back(make<syntax::Interpolation>(argument.expression));
                break;
            case TokenKind::ArgumentWithFormat:
                argument = arguments[static_cast<std::size_t>(token.index + 1)];
                unpack_single_element_array_of(argument);
                content.push_back(make<syntax::Interpolation>(argument.expression, 0, token.format));
                break;
            case TokenKind::ArgumentWithAlignment:
                argument = arguments[static_cast<std::size_t>(token.index + 1)];
                unpack_single_element_array_of(argument);
                content.push_back(make<syntax::Interpolation>(argument.expression, token.alignment));
                break;
            case TokenKind::ArgumentWithAlignmentAndFormat:
                argument = arguments[static_cast<std::size_t>(token.index + 1)];
                unpack_single_element_array_of(argument);
                content.push_back(make<syntax::Interpolation>(argument.expression, token.alignment, token.format));
                break;
            case TokenKind::Error: break;
        }
    }
    const IType* formattable_string_type = expression_builder_.compilation->find_type(KnownTypeCode::FormattableString);
    std::vector<ResolveResult*> interpolation_arguments = argument_list.get_argument_resolve_results(1);
    auto* isrr = make<InterpolatedStringResolveResult>(expression_builder_.compilation->find_type(KnownTypeCode::String), format, interpolation_arguments);
    auto* expr = make<syntax::InterpolatedStringExpression>();
    expr->content().add_range(content);
    if (method->name() == "Format") return with_rr(expr, isrr);
    ExpressionWithResolveResult inner = with_rr(expr, isrr);
    return with_rr(make<CastExpression>(expression_builder_.convert_type(formattable_string_type), inner.expression),
                   make<ConversionResolveResult>(formattable_string_type, isrr, Conversion::implicit_interpolated_string_conversion()));
}

// Converts a call to an Add method to a collection initializer expression.
ExpressionWithResolveResult CallBuilder::build_collection_initializer_expression(OpCode call_op_code, const IMethod* method,
                                                                                  InitializedObjectResolveResult* target,
                                                                                  const std::vector<il::ILInstruction*>& call_arguments) {
    // (see ECMA-334, section 12.7.11.4):
    // The collection object to which a collection initializer is applied shall be of a type that implements
    // System.Collections.IEnumerable or a compile-time error occurs. For each specified element in order,
    // the collection initializer invokes an Add method on the target object with the expression list of the
    // element initializer as argument list, applying normal overload resolution for each invocation. Thus, the
    // collection object shall contain an applicable Add method for each element initializer.

    // The list of applicable methods includes all methods (as of C# 6.0 extension methods, too) named 'Add'
    // that can be invoked on the target object, with the following exceptions:
    // - Methods with ref or out parameters may not be used,
    // - methods that have type parameters, that cannot be inferred from the parameter list may not be used,
    // - vararg methods may not be used.
    // - named arguments are not supported.
    // However, note that params methods may be used.

    // At this point, we assume that 'method' fulfills all the conditions mentioned above. We just need to make
    // sure that the correct method is called by resolving any ambiguities by inserting casts, if necessary.

    ExpectedTargetDetails expected_target_details;
    expected_target_details.call_op_code = call_op_code;
    TranslatedExpression unused = with_rr(make<syntax::IdentifierExpression>("initializedObject"), target).without_il_instruction();
    std::vector<il::ILInstruction*> args = call_arguments;
    if (method->is_extension_method()) args.insert(args.begin(), il::make<il::Nop>());

    ArgumentList argument_list = build_argument_list(expected_target_details, target, method, 0, args, std::nullopt);
    argument_list.argument_names.reset();
    argument_list.add_names_to_primitive_values = false;
    argument_list.use_implicitly_typed_out = false;
    CallTransformation transform = get_required_transformations_for_call(expected_target_details, method, unused, argument_list, CallTransformation::None, nullptr);
    assert((transform & ~(CallTransformation::NoOptionalArgumentAllowed | CallTransformation::NoNamedArgsForPrettiness)) == CallTransformation::None);

    // Calls with only one argument do not need an array initializer expression to wrap them.
    // Any special cases are handled by the caller (i.e., ExpressionBuilder.TranslateObjectAndCollectionInitializer)
    // Note: we intentionally ignore the firstOptionalArgumentIndex in this case.
    int skip_count;
    if (method->is_extension_method()) {
        if (argument_list.arguments.size() == 2) return argument_list.arguments[1];
        skip_count = 1;
    } else {
        if (argument_list.arguments.size() == 1) return argument_list.arguments[0];
        skip_count = 0;
    }

    if ((transform & CallTransformation::NoOptionalArgumentAllowed) != CallTransformation::None) argument_list.first_optional_argument_index = -1;

    std::vector<Expression*> element_expressions = argument_list.get_argument_expressions(skip_count);
    std::vector<ResolveResult*> element_results = argument_list.get_argument_resolve_results(skip_count);
    return with_rr(make<syntax::ArrayInitializerExpression>(element_expressions),
                   make<CSharpInvocationResolveResult>(target, method, element_results, OverloadResolutionErrors::None, method->is_extension_method(),
                                                       argument_list.is_expanded_form));
}

ExpressionWithResolveResult CallBuilder::build_dictionary_initializer_expression(OpCode call_op_code, const IMethod* method,
                                                                                  InitializedObjectResolveResult* target,
                                                                                  const std::vector<il::ILInstruction*>& indices, il::ILInstruction* value) {
    if (method == nullptr) throw std::invalid_argument("method");
    ExpectedTargetDetails expected_target_details;
    expected_target_details.call_op_code = call_op_code;

    std::vector<il::ILInstruction*> call_arguments;
    call_arguments.push_back(il::make<il::LdNull>());
    call_arguments.insert(call_arguments.end(), indices.begin(), indices.end());
    call_arguments.push_back(value != nullptr ? value : il::make<il::Nop>());

    ArgumentList argument_list = build_argument_list(expected_target_details, target, method, 1, call_arguments, std::nullopt);
    TranslatedExpression unused = with_rr(make<syntax::IdentifierExpression>("initializedObject"), target).without_il_instruction();

    ExpressionWithResolveResult assignment =
        handle_accessor_call(expected_target_details, method, unused, argument_list.arguments, argument_list.argument_names);

    auto* assignment_expression = static_cast<AssignmentExpression*>(assignment.expression);
    auto* indexer = syntax::as<IndexerExpression>(assignment_expression->left());
    if (indexer != nullptr && indexer->target() != nullptr) indexer->target()->remove();

    if (value != nullptr) return assignment;

    return ExpressionWithResolveResult(syntax::detach(assignment_expression->left()));
}

bool CallBuilder::is_interpolated_string_creation(const IMethod* method, const ArgumentList& argument_list) {
    return method->is_static() &&
           ((is_known_type(method->declaring_type(), KnownTypeCode::String) && method->name() == "Format") ||
            (method->name() == "Create" && method->declaring_type()->name() == "FormattableStringFactory" &&
             method->declaring_type()->namespace_() == "System.Runtime.CompilerServices")) &&
           !argument_list.argument_names.has_value()  // Argument names are not allowed
           && (argument_list.is_expanded_form  // Must be expanded form
               || !method->parameters().back()->is_params()  // -or- not a params overload
               || (argument_list.length() == 2 && syntax::is<ArrayCreateExpression>(argument_list.arguments[1].expression)));  // -or- an array literal
}

bool CallBuilder::try_get_string_interpolation_tokens(const ArgumentList& argument_list, std::string* format, std::vector<StringInterpolationToken>* tokens) {
    tokens->clear();
    const std::vector<TranslatedExpression>& arguments = argument_list.arguments;
    if (arguments.empty() || argument_list.argument_names.has_value() || argument_list.argument_to_parameter_map.has_value()) return false;
    auto* crr = dynamic_cast<ConstantResolveResult*>(arguments[0].resolve_result);
    if (!(crr != nullptr && is_known_type(crr->type(), KnownTypeCode::String))) return false;
    for (std::size_t a = 1; a < arguments.size(); a++) {
        for (syntax::AstNode* node : arguments[a].expression->descendants_and_self()) {
            auto* p = syntax::as<PrimitiveExpression>(node);
            if (p != nullptr && std::holds_alternative<std::u16string>(p->value())) return false;
        }
    }
    int i = 0;
    *format = util::utf8(std::get<std::u16string>(crr->constant_value()));
    for (const auto& [kind, data] : tokenize_format_string(*format)) {
        int index = 0;
        std::vector<std::string> arg;
        switch (kind) {
            case TokenKind::Error: return false;
            case TokenKind::String: tokens->push_back({kind, -1, 0, data}); break;
            case TokenKind::Argument:
                if (!try_parse_int(*data, &index) || index != i) return false;
                i++;
                tokens->push_back({kind, index, 0, std::nullopt});
                break;
            case TokenKind::ArgumentWithFormat:
                arg = split_at_most(*data, ":", 2);
                if (arg.size() != 2 || arg[1].empty()) return false;
                if (!try_parse_int(arg[0], &index) || index != i) return false;
                i++;
                tokens->push_back({kind, index, 0, arg[1]});
                break;
            case TokenKind::ArgumentWithAlignment: {
                arg = split_at_most(*data, ",", 2);
                if (arg.size() != 2 || arg[1].empty()) return false;
                if (!try_parse_int(arg[0], &index) || index != i) return false;
                int alignment = 0;
                if (!try_parse_int(arg[1], &alignment)) return false;
                i++;
                tokens->push_back({kind, index, alignment, std::nullopt});
                break;
            }
            case TokenKind::ArgumentWithAlignmentAndFormat: {
                arg = split_at_most(*data, ",:", 3);
                if (arg.size() != 3 || arg[1].empty() || arg[2].empty()) return false;
                if (!try_parse_int(arg[0], &index) || index != i) return false;
                int alignment = 0;
                if (!try_parse_int(arg[1], &alignment)) return false;
                i++;
                tokens->push_back({kind, index, alignment, arg[2]});
                break;
            }
        }
    }
    return i == static_cast<int>(arguments.size()) - 1;
}

// Splits a format string into its text and argument parts (the text is UTF-8; every character the tokenizer looks at is ASCII).
std::vector<std::pair<CallBuilder::TokenKind, std::optional<std::string>>> CallBuilder::tokenize_format_string(const std::string& value) {
    std::vector<std::pair<TokenKind, std::optional<std::string>>> result;
    std::size_t pos = static_cast<std::size_t>(-1);  // (upstream starts at -1)

    auto peek = [&](int steps = 1) -> int {
        std::size_t index = pos + static_cast<std::size_t>(steps);
        if (index < value.size()) return static_cast<unsigned char>(value[index]);
        return -1;
    };

    auto next = [&]() -> int {
        int val = peek();
        pos++;
        return val;
    };

    int next_char;
    TokenKind kind = TokenKind::String;
    std::string sb;

    while ((next_char = next()) > -1) {
        switch (static_cast<char>(next_char)) {
            case '{':
                if (peek() == '{') {
                    kind = TokenKind::String;
                    sb += "{{";
                    next();
                } else {
                    if (!sb.empty()) {
                        result.push_back({kind, sb});
                    }
                    kind = TokenKind::Argument;
                    sb.clear();
                }
                break;
            case '}':
                if (kind != TokenKind::String) {
                    result.push_back({kind, sb});
                    sb.clear();
                    kind = TokenKind::String;
                } else if (peek() == '}') {
                    sb += "}}";
                    next();
                } else {
                    result.push_back({TokenKind::Error, std::nullopt});
                }
                break;
            case ':':
                if (kind == TokenKind::Argument) {
                    kind = TokenKind::ArgumentWithFormat;
                } else if (kind == TokenKind::ArgumentWithAlignment) {
                    kind = TokenKind::ArgumentWithAlignmentAndFormat;
                }
                sb += ':';
                break;
            case ',':
                if (kind == TokenKind::Argument) {
                    kind = TokenKind::ArgumentWithAlignment;
                }
                sb += ',';
                break;
            default: sb += static_cast<char>(next_char); break;
        }
    }
    if (!sb.empty()) {
        if (kind == TokenKind::String)
            result.push_back({kind, sb});
        else
            result.push_back({TokenKind::Error, std::nullopt});
    }
    return result;
}

CallBuilder::ArgumentList CallBuilder::build_argument_list(const ExpectedTargetDetails& expected_target_details, ResolveResult* target, const IMethod* method,
                                                           int first_param_index, const std::vector<il::ILInstruction*>& call_arguments,
                                                           const std::optional<std::vector<int>>& argument_to_parameter_map) {
    ArgumentList list;

    // Translate arguments to the expected parameter types
    std::vector<TranslatedExpression> arguments;
    std::optional<ArgumentNames> argument_names;
    assert(static_cast<int>(call_arguments.size()) == first_param_index + static_cast<int>(method->parameters().size()));
    std::vector<const IParameter*> expected_parameters;  // parameters, but in argument order
    bool is_expanded_form = false;
    util::BitSet is_primitive_value(static_cast<int>(method->parameters().size()));

    // Optional arguments:
    // This value has the following values:
    // -2 - there are no optional arguments
    // -1 - optional arguments are forbidden
    // >= 0 - the index of the first argument that can be removed, because it is optional
    // and is the default value of the parameter.
    int first_optional_argument_index = expression_builder_.settings->optional_arguments ? -2 : -1;
    for (int i = first_param_index; i < static_cast<int>(call_arguments.size()); i++) {
        const IParameter* parameter;
        if (argument_to_parameter_map.has_value()) {
            if (!argument_names.has_value() && (*argument_to_parameter_map)[static_cast<std::size_t>(i)] != i - first_param_index) {
                // Starting at the first argument that is out-of-place,
                // assign names to that argument and all following arguments:
                argument_names = ArgumentNames(method->parameters().size());
            }
            parameter = method->parameters()[static_cast<std::size_t>((*argument_to_parameter_map)[static_cast<std::size_t>(i)])];
            if (argument_names.has_value() && il::transforms::AssignVariableNames::is_valid_name(parameter->name())) {
                (*argument_names)[arguments.size()] = parameter->name();
            }
        } else {
            parameter = method->parameters()[static_cast<std::size_t>(i - first_param_index)];
        }
        TranslatedExpression arg = expression_builder_.translate(call_arguments[static_cast<std::size_t>(i)], parameter->type());
        if (is_primitive_value_that_should_be_named_argument(arg, method, parameter)) {
            is_primitive_value.set(static_cast<int>(arguments.size()));
        }
        if (is_optional_argument(parameter, arg)) {
            if (first_optional_argument_index == -2) first_optional_argument_index = i - first_param_index;
        } else {
            if (first_optional_argument_index != -1) first_optional_argument_index = -2;
        }
        if (expression_builder_.settings->expand_params_arguments && parameter->is_params() && i + 1 == static_cast<int>(call_arguments.size()) &&
            !argument_to_parameter_map.has_value()) {
            // Parameter is marked params
            // If the argument is an array creation, inline all elements into the call and add missing default values.
            // Otherwise handle it normally.
            if (transform_params_argument(expected_target_details, target, method, parameter, arg, expected_parameters, arguments)) {
                assert(!argument_names.has_value());
                first_optional_argument_index = -1;
                is_expanded_form = true;
                continue;
            }
        }

        const IType* parameter_type;
        if (parameter->type()->kind() == TypeKind::Dynamic) {
            parameter_type = expression_builder_.compilation->find_type(KnownTypeCode::Object);
        } else {
            parameter_type = parameter->type();
        }

        arg = arg.convert_to(parameter_type, expression_builder_, false, arg.type()->kind() != TypeKind::Dynamic);

        if (parameter->reference_kind() != ReferenceKind::None) {
            arg = ExpressionBuilder::change_direction_expression_to(arg, parameter->reference_kind(),
                                                                    il::as<il::AddressOf>(call_arguments[static_cast<std::size_t>(i)]) != nullptr);
        }

        arguments.push_back(arg);
        expected_parameters.push_back(parameter);
    }

    list.expected_parameters = expected_parameters;
    list.arguments = arguments;
    for (const IParameter* p : expected_parameters) list.parameter_names.push_back(p->name());
    list.argument_names = argument_names;
    list.argument_to_parameter_map = argument_to_parameter_map;
    list.is_expanded_form = is_expanded_form;
    list.is_primitive_value = is_primitive_value;
    list.first_optional_argument_index = first_optional_argument_index;
    list.use_implicitly_typed_out = true;
    list.add_names_to_primitive_values = expression_builder_.settings->named_arguments && expression_builder_.settings->non_trailing_named_arguments;
    return list;
}

bool CallBuilder::is_primitive_value_that_should_be_named_argument(const TranslatedExpression& arg, const IMethod* method, const IParameter* p) {
    if (!arg.resolve_result->is_compile_time_constant() || is_known_type(method->declaring_type(), KnownTypeCode::NullableOfT)) return false;
    return is_known_type(p->type(), KnownTypeCode::Boolean);
}

bool CallBuilder::transform_params_argument(const ExpectedTargetDetails& expected_target_details, ResolveResult* target_resolve_result, const IMethod* method,
                                            const IParameter* parameter, const TranslatedExpression& params_argument,
                                            std::vector<const IParameter*>& expected_parameters, std::vector<TranslatedExpression>& arguments) {
    (void)parameter;
    const IType* element_type = nullptr;
    std::vector<const IParameter*> expanded_parameters;
    std::vector<TranslatedExpression> expanded_arguments;

    // Extracts the elements of the params argument when it is an array creation or one of the empty-array / span patterns.
    auto extract_arguments = [&]() -> bool {
        if (auto* invocation = dynamic_cast<CSharpInvocationResolveResult*>(params_argument.resolve_result)) {
            const IMethod* invoked = dynamic_cast<const IMethod*>(invocation->member());
            if (invoked == nullptr) return false;
            // match System.Array.Empty<T>()
            if (invocation->arguments.empty() && invoked->is_static() && invoked->full_name() == "System.Array.Empty" && invoked->type_arguments().size() == 1) {
                element_type = invoked->type_arguments()[0];
                return true;
            }
            // match System.ReadOnlySpan<T>..ctor(ref readonly T)
            auto* oce = syntax::as<ObjectCreateExpression>(params_argument.expression);
            if (oce != nullptr && invoked->is_constructor() && invoked->parameters().size() == 1 &&
                invoked->parameters()[0]->reference_kind() == ReferenceKind::RefReadOnly &&
                dynamic_cast<const typesystem::ByReferenceType*>(invoked->parameters()[0]->type()) != nullptr &&
                invoked->declaring_type()->type_arguments().size() == 1 && is_known_type(invoked->declaring_type(), KnownTypeCode::ReadOnlySpanOfT)) {
                const IType* param_type = by_reference_type_of(invoked->parameters()[0]->type())->element_type();
                const IType* type2 = invoked->declaring_type()->type_arguments()[0];
                if (param_type->equals(type2)) {
                    element_type = type2;
                    expanded_arguments.push_back(TranslatedExpression(oce->arguments()[0]));
                    expanded_parameters.push_back(make_default_parameter(type2));
                    return true;
                }
            }
            return false;
        }
        if (auto* array_create = dynamic_cast<ArrayCreateResolveResult*>(params_argument.resolve_result)) {
            auto* array_type = dynamic_cast<const typesystem::ArrayType*>(array_create->type());
            if (array_type == nullptr || array_create->size_arguments.size() != 1) return false;
            ConstantValue array_length_constant = array_create->size_arguments[0]->constant_value();
            auto* array_length = std::get_if<std::int32_t>(&array_length_constant);
            if (array_length == nullptr) return false;
            const IType* type3 = array_type->element_type();
            element_type = type3;
            auto* ace = static_cast<ArrayCreateExpression*>(params_argument.expression);
            if (ace->initializer() != nullptr)
                for (Expression* e : ace->initializer()->elements()) expanded_arguments.push_back(TranslatedExpression(e));
            for (int i = 0; i < *array_length; i++) {
                expanded_parameters.push_back(make_default_parameter(type3));
                if (static_cast<int>(expanded_arguments.size()) <= i)
                    expanded_arguments.push_back(TranslatedExpression(without_il_instruction(expression_builder_.get_default_value_expression(type3)).expression));
            }
            return true;
        }
        return false;
    };

    if (extract_arguments()) {
        expanded_parameters.insert(expanded_parameters.begin(), expected_parameters.begin(), expected_parameters.end());
        expanded_arguments.insert(expanded_arguments.begin(), arguments.begin(), arguments.end());
        std::vector<ResolveResult*> expanded_results;
        for (const TranslatedExpression& a : expanded_arguments) expanded_results.push_back(a.resolve_result);
        const IParameterizedMember* unused_member = nullptr;
        bool best_candidate_is_expanded_form = false;
        if (is_unambiguous_call(expected_target_details, method, target_resolve_result, {}, expanded_results, std::nullopt, -1, &unused_member,
                                &best_candidate_is_expanded_form) == OverloadResolutionErrors::None &&
            best_candidate_is_expanded_form) {
            expected_parameters = expanded_parameters;
            arguments.clear();
            for (const TranslatedExpression& a : expanded_arguments) arguments.push_back(TranslatedExpression(syntax::detach(a.expression)));
            return true;
        }
    }
    return false;
}

bool CallBuilder::is_optional_argument(const IParameter* parameter, const TranslatedExpression& arg) {
    if (!parameter->is_optional()) return false;

    auto* conversion_rr = dynamic_cast<ConversionResolveResult*>(arg.resolve_result);
    if (!arg.resolve_result->is_compile_time_constant() && !(conversion_rr != nullptr && conversion_rr->conversion->is_null_literal_conversion()))
        return false;
    for (const typesystem::IAttribute* a : parameter->get_attributes()) {
        if (is_known_type(a->attribute_type(), typesystem::KnownAttribute::CallerMemberName) ||
            is_known_type(a->attribute_type(), typesystem::KnownAttribute::CallerFilePath) ||
            is_known_type(a->attribute_type(), typesystem::KnownAttribute::CallerLineNumber))
            return false;
    }
    return parameter->get_constant_value() == arg.resolve_result->constant_value();
}

CallBuilder::CallTransformation CallBuilder::get_required_transformations_for_call(const ExpectedTargetDetails& expected_target_details, const IMethod* method,
                                                                                   TranslatedExpression& target, ArgumentList& argument_list,
                                                                                   CallTransformation allowed_transforms,
                                                                                   const IParameterizedMember** found_method_out) {
    CallTransformation transform = CallTransformation::None;
    const IParameterizedMember* found_method = nullptr;

    // initialize requireTarget flag
    bool require_target;
    ResolveResult* target_resolve_result;
    if ((allowed_transforms & CallTransformation::RequireTarget) != CallTransformation::None) {
        if (settings_.always_qualify_member_references || expression_builder_.hides_variable_with_name(method->name())) {
            require_target = true;
        } else {
            if (method->is_local_function())
                require_target = false;
            else if (method->is_static())
                require_target = !expression_builder_.is_current_or_containing_type(method->declaring_type_definition()) || method->name() == ".cctor";
            else if (method->name() == ".ctor")
                require_target = true;  // always use target for base/this-ctor-call, the constructor initializer pattern depends on this
            else if (syntax::is<BaseReferenceExpression>(target.expression))
                require_target = (expected_target_details.call_op_code != OpCode::CallVirt && method->is_virtual());
            else
                require_target = !syntax::is<ThisReferenceExpression>(target.expression);
        }
        target_resolve_result = require_target ? target.resolve_result : nullptr;
    } else {
        // HACK: this is a special case for collection initializer calls, they do not allow a target to be
        // emitted, but we still need it for overload resolution.
        require_target = true;
        target_resolve_result = target.resolve_result;
    }

    // initialize requireTypeArguments flag
    bool require_type_arguments;
    std::vector<const IType*> type_arguments;
    bool applied_require_type_arguments_shortcut = false;
    if (!method->type_parameters().empty() && (allowed_transforms & CallTransformation::RequireTypeArguments) != CallTransformation::None &&
        !is_possible_extension_method_call_on_null(method, argument_list.arguments)) {
        // The ambiguity resolution below only adds type arguments as last resort measure, however there are
        // methods, such as Enumerable.OfType<TResult>(IEnumerable input) that always require type arguments,
        // as those cannot be inferred from the parameters, which leads to bloated expressions full of extra casts
        // that are no longer required once we add the type arguments.
        // We lend overload resolution a hand by detecting such cases beforehand and requiring type arguments,
        // if necessary.
        if (!can_infer_type_arguments_from_arguments(method, argument_list, expression_builder_.type_inference)) {
            if (settings_.anonymous_types &&
                std::any_of(method->type_arguments().begin(), method->type_arguments().end(), [](const IType* a) { return contains_anonymous_type(a); }) &&
                pin_types_of_null_arguments(argument_list) &&
                can_infer_type_arguments_from_arguments(method, argument_list, expression_builder_.type_inference)) {
                // Anonymous types cannot be written as explicit type arguments; instead the
                // null arguments were rewritten so that all type arguments are inferable.
                require_type_arguments = false;
                type_arguments.clear();
            } else {
                require_type_arguments = true;
                type_arguments = method->type_arguments();
                applied_require_type_arguments_shortcut = true;
            }
        } else {
            require_type_arguments = false;
            type_arguments.clear();
        }
    } else {
        require_type_arguments = false;
        type_arguments.clear();
    }

    bool target_casted = false;
    bool arguments_casted = false;
    bool original_require_target = require_target;
    bool skip_target_cast = method->accessibility() <= typesystem::Accessibility::Protected &&
                            expression_builder_.is_base_type_of_current_type(method->declaring_type_definition());
    OverloadResolutionErrors errors;
    bool best_candidate_is_expanded_form = false;
    while (true) {
        errors = is_unambiguous_call(expected_target_details, method, target_resolve_result, type_arguments, argument_list.get_argument_resolve_results(),
                                     argument_list.get_argument_names(), argument_list.first_optional_argument_index, &found_method,
                                     &best_candidate_is_expanded_form);
        if (!(errors != OverloadResolutionErrors::None || best_candidate_is_expanded_form != argument_list.is_expanded_form)) break;

        bool use_default = false;
        switch (errors) {
            case OverloadResolutionErrors::OutVarTypeMismatch:
                assert(argument_list.use_implicitly_typed_out);
                argument_list.use_implicitly_typed_out = false;
                continue;
            case OverloadResolutionErrors::TypeInferenceFailed:
                if ((allowed_transforms & CallTransformation::RequireTypeArguments) != CallTransformation::None) {
                    // goto case WrongNumberOfTypeArguments
                    if (require_type_arguments) {
                        use_default = true;
                        break;
                    }
                    require_type_arguments = true;
                    type_arguments = method->type_arguments();
                    continue;
                }
                use_default = true;
                break;
            case OverloadResolutionErrors::WrongNumberOfTypeArguments:
                assert((allowed_transforms & CallTransformation::RequireTypeArguments) != CallTransformation::None);
                if (require_type_arguments) {
                    use_default = true;
                    break;
                }
                require_type_arguments = true;
                type_arguments = method->type_arguments();
                continue;
            case OverloadResolutionErrors::MissingArgumentForRequiredParameter:
                if (argument_list.first_optional_argument_index == -1) {
                    use_default = true;
                    break;
                }
                argument_list.first_optional_argument_index = -1;
                continue;
            default: use_default = true; break;
        }
        if (use_default) {
            // TODO : implement some more intelligent algorithm that decides which of these fixes (cast args, add target, cast target, add type args)
            // is best in this case. Additionally we should not cast all arguments at once, but step-by-step try to add only a minimal number of casts.
            if (argument_list.add_names_to_primitive_values) {
                argument_list.add_names_to_primitive_values = false;
            } else if (argument_list.first_optional_argument_index >= 0) {
                argument_list.first_optional_argument_index = -1;
            } else if (!arguments_casted) {
                // If we added type arguments beforehand, but that didn't make the code any better,
                // undo that decision and add casts first.
                if (applied_require_type_arguments_shortcut) {
                    require_type_arguments = false;
                    type_arguments.clear();
                    applied_require_type_arguments_shortcut = false;
                }
                arguments_casted = true;
                argument_list.use_implicitly_typed_out = false;
                cast_arguments(argument_list.arguments, argument_list.expected_parameters);
            } else if ((allowed_transforms & CallTransformation::RequireTarget) != CallTransformation::None && !require_target) {
                require_target = true;
                target_resolve_result = target.resolve_result;
            } else if ((allowed_transforms & CallTransformation::RequireTarget) != CallTransformation::None && !target_casted) {
                if (skip_target_cast && require_target != original_require_target) {
                    require_target = original_require_target;
                    if (!original_require_target) target_resolve_result = nullptr;
                    allowed_transforms &= ~CallTransformation::RequireTarget;
                } else {
                    target_casted = true;
                    target = target.convert_to(method->declaring_type(), expression_builder_);
                    target_resolve_result = target.resolve_result;
                }
            } else if ((allowed_transforms & CallTransformation::RequireTypeArguments) != CallTransformation::None && !require_type_arguments) {
                require_type_arguments = true;
                type_arguments = method->type_arguments();
            } else if ((allowed_transforms & CallTransformation::EnforceExplicitIn) != CallTransformation::None) {
                enforce_explicit_in(argument_list.arguments, argument_list.expected_parameters);
                allowed_transforms &= ~CallTransformation::EnforceExplicitIn;
            } else {
                // We've given up.
                found_method = method;
                break;
            }
            continue;
        }
    }
    if ((allowed_transforms & CallTransformation::RequireTarget) != CallTransformation::None && require_target) transform |= CallTransformation::RequireTarget;
    if ((allowed_transforms & CallTransformation::RequireTypeArguments) != CallTransformation::None && require_type_arguments)
        transform |= CallTransformation::RequireTypeArguments;
    if (argument_list.first_optional_argument_index < 0) transform |= CallTransformation::NoOptionalArgumentAllowed;
    if (!argument_list.add_names_to_primitive_values) transform |= CallTransformation::NoNamedArgsForPrettiness;
    if (found_method_out != nullptr) *found_method_out = found_method;
    return transform;
}

void CallBuilder::enforce_explicit_in(std::vector<TranslatedExpression>& arguments, const std::vector<const IParameter*>& expected_parameters) {
    for (std::size_t i = 0; i < arguments.size(); i++) {
        if (expected_parameters[i]->reference_kind() != ReferenceKind::In) continue;
        if (syntax::is<DirectionExpression>(arguments[i].expression)) continue;

        arguments[i] = wrap_in_as_ref_read_only(arguments[i]);
        expression_builder_.statement_builder->emit_as_ref_read_only = true;
    }
}

TranslatedExpression CallBuilder::wrap_in_as_ref_read_only(const TranslatedExpression& arg) {
    auto* invocation = make<InvocationExpression>(make<syntax::IdentifierExpression>("ILSpyHelper_AsRefReadOnly"));
    invocation->arguments().add(arg.expression);
    return without_il_instruction(make<DirectionExpression>(FieldDirection::In, invocation)).with_rr(make<ByReferenceResolveResult>(arg.type(), ReferenceKind::In)) ;
}

bool CallBuilder::is_possible_extension_method_call_on_null(const IMethod* method, const std::vector<TranslatedExpression>& arguments) {
    return method->is_extension_method() && !arguments.empty() && syntax::is<NullReferenceExpression>(arguments[0].expression);
}

bool CallBuilder::can_infer_type_arguments_from_arguments(const IMethod* method, const ArgumentList& argument_list, resolver::TypeInference& type_inference) {
    if (method->type_parameters().empty()) return true;
    // always use unspecialized member, otherwise type inference fails
    method = dynamic_cast<const IMethod*>(method->member_definition());
    std::vector<const IType*> param_types_in_argument_order;
    if (!argument_list.argument_to_parameter_map.has_value()) {
        for (const IParameter* p : method->parameters()) param_types_in_argument_order.push_back(p->type());
    } else {
        for (int index : *argument_list.argument_to_parameter_map)
            param_types_in_argument_order.push_back(index >= 0 ? method->parameters()[static_cast<std::size_t>(index)]->type() : &SpecialType::unknown_type());
    }
    std::vector<ResolveResult*> argument_results;
    for (const TranslatedExpression& a : argument_list.arguments) argument_results.push_back(a.resolve_result);
    bool success = false;
    type_inference.infer_type_arguments(method->type_parameters(), argument_results, param_types_in_argument_order, &success);
    return success;
}

// C# has no syntax to spell out an anonymous type, so a null literal cannot be given such a type with a cast. The minimal expression that
// produces a null value of an anonymous type is a conditional expression whose never-taken branch creates an instance of the type:
// `true ? null : new { A = default(int) }`. Replaces null-literal arguments of an anonymous type with such an expression, so that type
// arguments involving anonymous types (which cannot be written explicitly either) become inferable from the arguments.
// Returns true, if at least one argument was replaced.
bool CallBuilder::pin_types_of_null_arguments(ArgumentList& argument_list) {
    bool any_argument_replaced = false;
    for (int i = 0; i < argument_list.length(); i++) {
        const IType* expected_type = argument_list.expected_parameters[static_cast<std::size_t>(i)]->type();
        if (!syntax::is<NullReferenceExpression>(argument_list.arguments[static_cast<std::size_t>(i)].expression)) continue;
        if (!is_anonymous_type(expected_type)) continue;
        NewObj* new_obj = new_anonymous_type_instance(expected_type);
        if (new_obj == nullptr) continue;
        TranslatedExpression null_literal = argument_list.arguments[static_cast<std::size_t>(i)];
        Expression* null_expression = syntax::detach(null_literal.expression);
        TranslatedExpression instance = expression_builder_.translate(new_obj, expected_type);
        argument_list.arguments[static_cast<std::size_t>(i)] =
            with_il_instruction(make<ConditionalExpression>(make<PrimitiveExpression>(ConstantValue{true}), null_expression, instance.expression),
                                null_literal.il_instructions())
                .with_rr(make<ResolveResult>(expected_type));
        any_argument_replaced = true;
    }
    return any_argument_replaced;
}

// Builds a 'newobj' instruction creating an instance of the anonymous type with default property values; translating it yields
// object-initializer syntax, the only way to name the type in source code. Returns null if a property type involves an anonymous type
// other than by direct nesting (e.g. an array of anonymous type), because its default value expression would have to name it.
NewObj* CallBuilder::new_anonymous_type_instance(const IType* type) {
    auto* new_obj = il::make<NewObj>(type->get_constructors()[0]);
    for (const IParameter* parameter : new_obj->method->parameters()) {
        il::ILInstruction* argument;
        if (is_anonymous_type(parameter->type()))
            argument = new_anonymous_type_instance(parameter->type());
        else
            argument = contains_anonymous_type(parameter->type()) ? nullptr : il::make<il::DefaultValue>(parameter->type());
        if (argument == nullptr) return nullptr;
        new_obj->arguments().add(argument);
    }
    return new_obj;
}

void CallBuilder::cast_arguments(std::vector<TranslatedExpression>& arguments, const std::vector<const IParameter*>& expected_parameters) {
    for (std::size_t i = 0; i < arguments.size(); i++) {
        if (settings_.anonymous_types && contains_anonymous_type(expected_parameters[i]->type())) {
            if (auto* lambda = syntax::as<syntax::LambdaExpression>(arguments[i].expression)) {
                modify_return_type_of_lambda(lambda);
            }
        } else {
            const IParameter* parameter = expected_parameters[i];
            const IType* parameter_type;
            if (parameter->type()->kind() == TypeKind::Dynamic) {
                parameter_type = expression_builder_.compilation->find_type(KnownTypeCode::Object);
            } else {
                parameter_type = parameter->type();
            }

            auto* brt = dynamic_cast<const typesystem::ByReferenceType*>(parameter_type);
            if (parameter->reference_kind() == ReferenceKind::In && brt != nullptr &&
                dynamic_cast<const typesystem::ByReferenceType*>(arguments[i].type()) == nullptr) {
                parameter_type = brt->element_type();
            }

            arguments[i] = arguments[i].convert_to(parameter_type, expression_builder_, false, false);
        }
    }
}

bool CallBuilder::is_null_conditional(Expression* expr) {
    auto* uoe = syntax::as<UnaryOperatorExpression>(expr);
    return uoe != nullptr && uoe->operator_() == UnaryOperatorType::NullConditional;
}

void CallBuilder::modify_return_type_of_lambda(syntax::LambdaExpression* lambda) {
    auto* resolve_result = static_cast<resolver::DecompiledLambdaResolveResult*>(get_resolve_result(lambda));
    if (auto* expr_body = syntax::as<Expression>(lambda->body())) {
        lambda->set_body(TranslatedExpression(syntax::detach(expr_body)).convert_to(resolve_result->return_type(), expression_builder_).expression);
    } else {
        modify_return_statement_inside_lambda(resolve_result->return_type(), lambda);
    }
    resolve_result->inferred_return_type = resolve_result->return_type();
}

void CallBuilder::modify_return_statement_inside_lambda(const IType* return_type, syntax::AstNode* parent) {
    for (syntax::AstNode* child : parent->children().to_vector()) {
        if (syntax::is<syntax::LambdaExpression>(child) || syntax::is<syntax::AnonymousMethodExpression>(child)) continue;
        if (auto* ret = syntax::as<syntax::ReturnStatement>(child)) {
            if (ret->expression() != nullptr)
                ret->set_expression(TranslatedExpression(syntax::detach(ret->expression())).convert_to(return_type, expression_builder_).expression);
            continue;
        }
        modify_return_statement_inside_lambda(return_type, child);
    }
}

bool CallBuilder::is_delegate_equality_comparison(const IMethod* method, const std::vector<TranslatedExpression>& arguments) {
    // Comparison on a delegate type is a C# builtin operator
    // that compiles down to a Delegate.op_Equality call.
    // We handle this as a special case to avoid inserting a cast to System.Delegate.
    return method->is_operator() && is_known_type(method->declaring_type(), KnownTypeCode::Delegate) &&
           (method->name() == "op_Equality" || method->name() == "op_Inequality") && arguments.size() == 2 &&
           arguments[0].type()->kind() == TypeKind::Delegate && arguments[1].type()->equals(arguments[0].type());
}

Expression* CallBuilder::handle_delegate_equality_comparison(const IMethod* method, const std::vector<TranslatedExpression>& arguments) {
    return make<BinaryOperatorExpression>(arguments[0].expression,
                                          method->name() == "op_Equality" ? BinaryOperatorType::Equality : BinaryOperatorType::InEquality, arguments[1].expression);
}

ExpressionWithResolveResult CallBuilder::handle_implicit_conversion(const IMethod* method, TranslatedExpression argument) {
    CSharpConversions& conversions = CSharpConversions::get(*expression_builder_.compilation);
    const IType* target_type = method->return_type();
    const Conversion* conv = conversions.implicit_conversion(argument.type(), target_type);
    if (!(conv->is_user_defined() && conv->is_valid() &&
          conv->method()->equals(method, typesystem::NormalizeTypeVisitor::type_erasure()))) {
        // implicit conversion to targetType isn't directly possible, so first insert a cast to the argument type
        argument = argument.convert_to(method->parameters()[0]->type(), expression_builder_);
        conv = conversions.implicit_conversion(argument.type(), target_type);
    }
    auto* argument_direction = syntax::as<DirectionExpression>(argument.expression);
    if (argument_direction != nullptr && argument_direction->field_direction() == FieldDirection::In) {
        // `(TargetType)(in arg)` is invalid syntax.
        // Also, `f(in arg)` is invalid when there's an implicit conversion involved.
        argument = argument.unwrap_child(argument_direction->expression());
    }
    return with_rr(make<CastExpression>(expression_builder_.convert_type(target_type), argument.expression),
                   make<ConversionResolveResult>(target_type, argument.resolve_result, conv));
}

OverloadResolutionErrors CallBuilder::is_unambiguous_call(const ExpectedTargetDetails& expected_target_details, const IMethod* method, ResolveResult* target,
                                                          const std::vector<const IType*>& type_arguments, const std::vector<ResolveResult*>& arguments,
                                                          std::optional<ArgumentNames> argument_names, int first_optional_argument_index,
                                                          const IParameterizedMember** found_member, bool* best_candidate_is_expanded_form) {
    *found_member = nullptr;
    *best_candidate_is_expanded_form = false;
    const typesystem::ITypeDefinition* current_type_definition = resolver_->current_type_definition();
    resolver::MemberLookup lookup(current_type_definition, current_type_definition->parent_module());

    if (!(first_optional_argument_index < 0 || !argument_names.has_value())) {
        argument_names->resize(std::min(argument_names->size(), static_cast<std::size_t>(first_optional_argument_index)));
    }

    OverloadResolution overload_resolution(resolver_->compilation(), arguments, argument_names.value_or(ArgumentNames{}), type_arguments,
                                           &expression_builder_.resolver->conversions());
    if (expected_target_details.call_op_code == OpCode::NewObj) {
        for (const IMethod* ctor : method->declaring_type()->get_constructors()) {
            bool allow_protected_access = resolver_->current_type_definition() == method->declaring_type_definition();
            if (lookup.is_accessible(ctor, allow_protected_access)) {
                resolver::log::indent();
                OverloadResolutionErrors errors = overload_resolution.add_candidate(ctor);
                resolver::log::unindent();
                (void)errors;
            }
        }
    } else if (method->is_operator()) {
        std::vector<const IParameterizedMember*> operator_candidates;
        auto add_unique = [&operator_candidates](const std::vector<const IParameterizedMember*>& more) {
            for (const IParameterizedMember* m : more) {
                bool present = std::any_of(operator_candidates.begin(), operator_candidates.end(),
                                           [m](const IParameterizedMember* existing) { return member_equals(existing, m); });
                if (!present) operator_candidates.push_back(m);
            }
        };
        if (arguments.size() == 1) {
            const IType* arg_type = typesystem::nullable_type::get_underlying_type(arguments[0]->type());
            operator_candidates = resolver_->get_user_defined_operator_candidates(arg_type, method->name());
            if (method->name() == "op_Explicit") {
                // For casts, also consider candidates from the target type we are casting to.
                std::vector<const IParameterizedMember*> hash_set;
                std::swap(hash_set, operator_candidates);
                add_unique(hash_set);
                const IType* target_type = typesystem::nullable_type::get_underlying_type(method->return_type());
                add_unique(resolver_->get_user_defined_operator_candidates(target_type, method->name()));
            }
        } else if (arguments.size() == 2) {
            const IType* lhs_type = typesystem::nullable_type::get_underlying_type(arguments[0]->type());
            const IType* rhs_type = typesystem::nullable_type::get_underlying_type(arguments[1]->type());
            add_unique(resolver_->get_user_defined_operator_candidates(lhs_type, method->name()));
            add_unique(resolver_->get_user_defined_operator_candidates(rhs_type, method->name()));
        }
        for (const IParameterizedMember* m : operator_candidates) {
            overload_resolution.add_candidate(m);
        }
    } else if (target == nullptr) {
        auto* result = dynamic_cast<MethodGroupResolveResult*>(resolver_->resolve_simple_name(method->name(), type_arguments, true));
        if (result == nullptr) return OverloadResolutionErrors::AmbiguousMatch;
        overload_resolution.add_method_lists(result->methods_grouped_by_declaring_type());
    } else {
        auto* result = dynamic_cast<MethodGroupResolveResult*>(lookup.lookup(target, method->name(), type_arguments, true));
        if (result == nullptr) return OverloadResolutionErrors::AmbiguousMatch;
        overload_resolution.add_method_lists(result->methods_grouped_by_declaring_type());
    }
    *best_candidate_is_expanded_form = overload_resolution.best_candidate_is_expanded_form();
    if (overload_resolution.best_candidate_errors() != OverloadResolutionErrors::None) return overload_resolution.best_candidate_errors();
    if (overload_resolution.is_ambiguous()) return OverloadResolutionErrors::AmbiguousMatch;
    *found_member = overload_resolution.get_best_candidate_with_substituted_type_arguments();
    if (!is_appropriate_call_target(expected_target_details, method, *found_member)) return OverloadResolutionErrors::AmbiguousMatch;
    std::optional<std::vector<int>> map = overload_resolution.get_argument_to_parameter_map();
    for (std::size_t i = 0; i < arguments.size(); i++) {
        ResolveResult* arg = arguments[i];
        int parameter_index = (*map)[i];
        auto* rr = dynamic_cast<OutVarResolveResult*>(arg);
        if (rr != nullptr && parameter_index >= 0) {
            const IParameter* param = (*found_member)->parameters()[static_cast<std::size_t>(parameter_index)];
            const IType* param_type = typesystem::unwrap_by_ref(param->type());
            if (!param_type->equals(rr->original_variable_type)) return OverloadResolutionErrors::OutVarTypeMismatch;
        }
    }

    return OverloadResolutionErrors::None;
}

bool CallBuilder::is_unambiguous_access(const ExpectedTargetDetails& expected_target_details, ResolveResult* target, const IMethod* method,
                                        const std::vector<TranslatedExpression>& arguments, const std::optional<ArgumentNames>& argument_names,
                                        const IMember** found_member) {
    *found_member = nullptr;
    if (target == nullptr) {
        auto* result = dynamic_cast<MemberResolveResult*>(resolver_->resolve_simple_name(method->accessor_owner()->name(), {}, false));
        if (result == nullptr || result->is_error()) return false;
        *found_member = result->member();
    } else {
        resolver::MemberLookup lookup(resolver_->current_type_definition(), resolver_->current_type_definition()->parent_module());
        if (method->accessor_owner()->symbol_kind() == SymbolKind::Indexer) {
            std::vector<ResolveResult*> argument_results;
            for (const TranslatedExpression& a : arguments) argument_results.push_back(a.resolve_result);
            OverloadResolution overload_resolution(resolver_->compilation(), argument_results, argument_names.value_or(ArgumentNames{}), {},
                                                   &expression_builder_.resolver->conversions());
            overload_resolution.add_method_lists(lookup.lookup_indexers(target));
            if (overload_resolution.best_candidate_errors() != OverloadResolutionErrors::None) return false;
            if (overload_resolution.is_ambiguous()) return false;
            *found_member = overload_resolution.get_best_candidate_with_substituted_type_arguments();
        } else {
            auto* result = dynamic_cast<MemberResolveResult*>(lookup.lookup(target, method->accessor_owner()->name(), {}, false));
            if (result == nullptr || result->is_error()) return false;
            *found_member = result->member();
        }
    }
    return *found_member != nullptr && is_appropriate_call_target(expected_target_details, method->accessor_owner(), *found_member);
}

ExpressionWithResolveResult CallBuilder::handle_accessor_call(const ExpectedTargetDetails& expected_target_details, const IMethod* method,
                                                              TranslatedExpression target, std::vector<TranslatedExpression> arguments,
                                                              const std::optional<ArgumentNames>& argument_names) {
    bool require_target;
    if (settings_.always_qualify_member_references || method->accessor_owner()->symbol_kind() == SymbolKind::Indexer ||
        expression_builder_.hides_variable_with_name(method->accessor_owner()->name()))
        require_target = true;
    else if (method->is_static())
        require_target = !expression_builder_.is_current_or_containing_type(method->declaring_type_definition());
    else
        require_target = !syntax::is<ThisReferenceExpression>(target.expression);
    bool target_casted = false;
    bool is_setter = is_known_type(method->return_type(), KnownTypeCode::Void);
    bool arguments_casted = (is_setter && method->parameters().size() == 1) || (!is_setter && method->parameters().empty());
    ResolveResult* target_resolve_result = require_target ? target.resolve_result : nullptr;

    TranslatedExpression value;
    if (is_setter) {
        value = arguments.back();
        arguments.pop_back();
    }

    const IMember* found_member = nullptr;
    while (!is_unambiguous_access(expected_target_details, target_resolve_result, method, arguments, argument_names, &found_member)) {
        if (!arguments_casted) {
            arguments_casted = true;
            cast_arguments(arguments, method->parameters());
        } else if (!require_target) {
            require_target = true;
            target_resolve_result = target.resolve_result;
        } else if (!target_casted) {
            target_casted = true;
            target = target.convert_to(method->accessor_owner()->declaring_type(), expression_builder_);
            target_resolve_result = target.resolve_result;
        } else {
            found_member = method->accessor_owner();
            break;
        }
    }

    auto* rr = make<MemberResolveResult>(target.resolve_result, found_member);

    auto argument_expressions = [&arguments]() {
        std::vector<Expression*> result;
        for (const TranslatedExpression& a : arguments) result.push_back(a.expression);
        return result;
    };

    if (is_setter) {
        TranslatedExpression expr;

        if (!arguments.empty()) {
            expr = without_il_instruction(make<IndexerExpression>(dynamic_cast<InitializedObjectResolveResult*>(target.resolve_result) != nullptr ? nullptr : target.expression,
                                                                  argument_expressions()))
                       .with_rr(rr);
        } else if (require_target) {
            expr = without_il_instruction(make<MemberReferenceExpression>(target.expression, method->accessor_owner()->name())).with_rr(rr);
        } else {
            expr = without_il_instruction(make<syntax::IdentifierExpression>(method->accessor_owner()->name())).with_rr(rr);
        }

        AssignmentOperatorType op = AssignmentOperatorType::Assign;
        if (auto* parent_event = dynamic_cast<const typesystem::IEvent*>(method->accessor_owner())) {
            if (member_equals(method, parent_event->add_accessor())) {
                op = AssignmentOperatorType::Add;
            }
            if (member_equals(method, parent_event->remove_accessor())) {
                op = AssignmentOperatorType::Subtract;
            }
        }
        return with_rr(make<AssignmentExpression>(expr.expression, op, value.expression), make<TypeResolveResult>(method->accessor_owner()->return_type()));
    } else {
        if (!arguments.empty()) {
            return without_il_instruction(make<IndexerExpression>(target.expression, argument_expressions())).with_rr(rr);
        } else if (require_target) {
            return without_il_instruction(make<MemberReferenceExpression>(target.expression, method->accessor_owner()->name())).with_rr(rr);
        } else {
            return without_il_instruction(make<syntax::IdentifierExpression>(method->accessor_owner()->name())).with_rr(rr);
        }
    }
}

bool CallBuilder::is_appropriate_call_target(const ExpectedTargetDetails& expected_target_details, const IMember* expected_target, const IMember* actual_target) {
    if (expected_target->equals(actual_target, typesystem::NormalizeTypeVisitor::type_erasure())) return true;

    if (expected_target_details.call_op_code == OpCode::CallVirt && actual_target->is_override()) {
        if (expected_target_details.needs_boxing_conversion && actual_target->declaring_type()->is_reference_type() != true) return false;
        for (const IMember* possible_target : typesystem::inheritance_helper::get_base_members(actual_target, false)) {
            if (expected_target->equals(possible_target, typesystem::NormalizeTypeVisitor::type_erasure())) return true;
            if (!possible_target->is_override()) break;
        }
    }
    return false;
}

ExpressionWithResolveResult CallBuilder::handle_constructor_call(const ExpectedTargetDetails& expected_target_details, ResolveResult* target, const IMethod* method,
                                                                 ArgumentList& argument_list) {
    if (settings_.anonymous_types && is_anonymous_type(method->declaring_type())) {
        assert(!argument_list.argument_to_parameter_map.has_value() && !argument_list.argument_names.has_value() && argument_list.first_optional_argument_index < 0);
        auto* atce = make<syntax::AnonymousTypeCreateExpression>();
        if (argument_list.can_infer_anonymous_type_property_names_from_arguments()) {
            for (Expression* e : argument_list.get_argument_expressions()) atce->initializers().add(e);
        } else {
            for (int i = 0; i < argument_list.length(); i++) {
                atce->initializers().add(make<syntax::NamedExpression>(
                    argument_list.expected_parameters[static_cast<std::size_t>(i)]->name(),
                    argument_list.arguments[static_cast<std::size_t>(i)]
                        .convert_to(argument_list.expected_parameters[static_cast<std::size_t>(i)]->type(), expression_builder_)
                        .expression));
            }
        }
        return with_rr(atce, make<CSharpInvocationResolveResult>(target, method, argument_list.get_argument_resolve_results(),
                                                                 OverloadResolutionErrors::None, false, argument_list.is_expanded_form, false,
                                                                 argument_list.argument_to_parameter_map));
    } else {
        while (true) {
            const IParameterizedMember* unused_member = nullptr;
            bool best_candidate_is_expanded_form = false;
            OverloadResolutionErrors errors =
                is_unambiguous_call(expected_target_details, method, nullptr, {}, argument_list.get_argument_resolve_results(),
                                    argument_list.get_argument_names(), argument_list.first_optional_argument_index, &unused_member,
                                    &best_candidate_is_expanded_form);
            if (!(errors != OverloadResolutionErrors::None || best_candidate_is_expanded_form != argument_list.is_expanded_form)) break;
            if (argument_list.add_names_to_primitive_values) {
                argument_list.add_names_to_primitive_values = false;
                continue;
            }
            if (argument_list.first_optional_argument_index >= 0) {
                argument_list.first_optional_argument_index = -1;
                continue;
            }
            cast_arguments(argument_list.arguments, argument_list.expected_parameters);
            break;  // make sure that we don't not end up in an infinite loop
        }
        const IType* return_type_override = nullptr;
        if ((type_system_.type_system_options() & typesystem::TypeSystemOptions::NativeIntegersWithoutAttribute) != typesystem::TypeSystemOptions{}) {
            // For DeclaringType, we don't use nint/nuint (so that DeclaringType.GetConstructors etc. works),
            // but in NativeIntegersWithoutAttribute mode we must use nint/nuint for expression types,
            // so that the appropriate set of conversions is used for further overload resolution.
            if (is_known_type(method->declaring_type(), KnownTypeCode::IntPtr))
                return_type_override = &SpecialType::nint();
            else if (is_known_type(method->declaring_type(), KnownTypeCode::UIntPtr))
                return_type_override = &SpecialType::nuint();
        }
        AstType* created_type = expression_builder_.convert_type(method->declaring_type());
        std::vector<Expression*> created_arguments = argument_list.get_argument_expressions();
        return with_rr(make<ObjectCreateExpression>(created_type, created_arguments),
                       make<CSharpInvocationResolveResult>(target, method, argument_list.get_argument_resolve_results(), OverloadResolutionErrors::None,
                                                           false, argument_list.is_expanded_form, false, argument_list.argument_to_parameter_map,
                                                           std::vector<ResolveResult*>{}, return_type_override));
    }
}

TranslatedExpression CallBuilder::handle_delegate_construction(CallInstruction* inst) {
    il::ILInstruction* this_arg = inst->arguments()[0];
    il::ILInstruction* func = inst->arguments()[1];
    const IMethod* method;
    ExpectedTargetDetails expected_target_details;
    switch (func->op_code) {
        case OpCode::LdFtn:
            method = static_cast<il::LdFtn*>(func)->method();
            expected_target_details.call_op_code = OpCode::Call;
            break;
        case OpCode::LdVirtFtn:
            method = static_cast<il::LdVirtFtn*>(func)->method();
            expected_target_details.call_op_code = OpCode::CallVirt;
            break;
        default: throw std::invalid_argument("Unknown instruction type: " + std::string(il::opcode_name(func->op_code)));
    }
    if (can_use_delegate_construction(method, this_arg, typesystem::get_delegate_invoke_method(inst->method->declaring_type()))) {
        return handle_delegate_construction(inst->method->declaring_type(), method, expected_target_details, this_arg, inst);
    } else {
        ArgumentList argument_list =
            build_argument_list(expected_target_details, nullptr, inst->method, 0, inst->arguments().items(), std::nullopt);
        ExpectedTargetDetails new_obj_details;
        new_obj_details.call_op_code = OpCode::NewObj;
        return handle_constructor_call(new_obj_details, nullptr, inst->method, argument_list).with_il_instruction(inst);
    }
}

bool CallBuilder::can_use_delegate_construction(const IMethod* target_method, il::ILInstruction* this_arg, const IMethod* invoke_method) {
    // Accessors cannot be directly referenced as method group in C#
    // see https://github.com/icsharpcode/ILSpy/issues/1741#issuecomment-540179101
    if (target_method->is_accessor()) return false;
    if (target_method->is_static()) {
        // If the invoke method is known, we can compare the parameter counts to figure out whether the
        // delegate is static or binds the first argument
        if (invoke_method != nullptr) {
            if (invoke_method->parameters().size() == target_method->parameters().size()) {
                return this_arg->match_ld_null();
            } else if (target_method->is_extension_method() && invoke_method->parameters().size() == target_method->parameters().size() - 1) {
                return true;
            } else {
                return false;
            }
        } else {
            // delegate type unknown:
            return this_arg->match_ld_null() || target_method->is_extension_method();
        }
    } else {
        // targetMethod is instance method
        if (invoke_method != nullptr && invoke_method->parameters().size() != target_method->parameters().size()) return false;
        return true;
    }
}

TranslatedExpression CallBuilder::build(il::LdVirtDelegate* inst) {
    ExpectedTargetDetails expected_target_details;
    expected_target_details.call_op_code = OpCode::CallVirt;
    return handle_delegate_construction(inst->type(), inst->method(), expected_target_details, inst->argument(), inst);
}

ExpressionWithResolveResult CallBuilder::build_method_reference(const IMethod* method, bool is_virtual) {
    ExpectedTargetDetails expected_target_details;
    expected_target_details.call_op_code = is_virtual ? OpCode::CallVirt : OpCode::Call;
    ExpressionWithResolveResult expr = build_delegate_reference(method, nullptr, expected_target_details, nullptr);
    expr.expression->remove_annotations<ResolveResult>();
    return with_rr(expr.expression, make<MemberResolveResult>(nullptr, method));
}

ExpressionWithResolveResult CallBuilder::build_delegate_reference(const IMethod* method, const IMethod* invoke_method,
                                                                  const ExpectedTargetDetails& expected_target_details, il::ILInstruction* this_arg) {
    ExpressionWithResolveResult target_expression;
    DelegateReference reference = disambiguate_delegate_reference(method, invoke_method, expected_target_details, this_arg);
    if (reference.target.expression != nullptr) {
        auto* mre = make<MemberReferenceExpression>(reference.target.expression, reference.method_name);
        if (reference.add_type_arguments) {
            for (const IType* type_argument : method->type_arguments()) mre->type_arguments().add(expression_builder_.convert_type(type_argument));
        }
        target_expression = with_rr(mre, reference.result);
    } else {
        auto* ide = make<syntax::IdentifierExpression>(reference.method_name);
        if (reference.add_type_arguments) {
            for (const IType* type_argument : method->type_arguments()) ide->type_arguments().add(expression_builder_.convert_type(type_argument));
        }
        target_expression = with_rr(ide, reference.result);
    }
    return target_expression;
}

CallBuilder::DelegateReference CallBuilder::disambiguate_delegate_reference(const IMethod* method, const IMethod* invoke_method,
                                                                            const ExpectedTargetDetails& expected_target_details,
                                                                            il::ILInstruction* this_arg) {
    if (method->is_local_function()) {
        il::ILFunction* local_function = expression_builder_.resolve_local_function(method);
        assert(local_function != nullptr);
        return {TranslatedExpression(), true, local_function->name, to_method_group(method, local_function)};
    }
    if (method->is_extension_method() && invoke_method != nullptr && method->parameters().size() - 1 == invoke_method->parameters().size()) {
        const IType* target_type = method->parameters()[0]->type();
        auto* this_arg_box = il::as<il::Box>(this_arg);
        if (target_type->kind() == TypeKind::ByReference && this_arg_box != nullptr) {
            target_type = by_reference_type_of(target_type)->element_type();
            this_arg = this_arg_box->argument();
        }
        TranslatedExpression target = expression_builder_.translate(this_arg, target_type);
        TranslatedExpression current_target = target;
        bool target_casted = false;
        bool add_type_arguments = false;
        // Initial inputs for IsUnambiguousMethodReference:
        ResolveResult* target_resolve_result = target.resolve_result;
        std::vector<const IType*> type_arguments;
        if (this_arg->match_ld_null()) {
            target_casted = true;
            current_target = current_target.convert_to(target_type, expression_builder_);
            target_resolve_result = current_target.resolve_result;
        }
        // Find somewhat minimal solution:
        ResolveResult* result = nullptr;
        while (!is_unambiguous_method_reference(expected_target_details, method, target_resolve_result, type_arguments, true, &result)) {
            if (!target_casted) {
                // try casting target
                target_casted = true;
                current_target = current_target.convert_to(target_type, expression_builder_);
                target_resolve_result = current_target.resolve_result;
                continue;
            }
            if (!add_type_arguments) {
                // try adding type arguments
                add_type_arguments = true;
                type_arguments = method->type_arguments();
                continue;
            }
            break;
        }
        return {current_target, add_type_arguments, method->name(), result};
    } else {
        // Prepare call target
        const IType* target_type = method->declaring_type();
        auto* this_arg_box = il::as<il::Box>(this_arg);
        if (target_type->is_reference_type() == false && this_arg_box != nullptr) {
            // Normal struct instance method calls (which TranslateTarget is meant for) expect a 'ref T',
            // but delegate construction uses a 'box T'.
            if (auto* ldobj = il::as<il::LdObj>(this_arg_box->argument())) {
                this_arg = ldobj->target();
            } else {
                this_arg = il::make<il::AddressOf>(this_arg_box->argument(), this_arg_box->type());
            }
        }
        TranslatedExpression target = expression_builder_.translate_target(
            this_arg, expected_target_details.call_op_code == OpCode::Call, method->is_static(), method->declaring_type(), nullptr);
        // check if target is required
        bool require_target = expression_builder_.hides_variable_with_name(method->name()) ||
                              (method->is_static() ? !expression_builder_.is_current_or_containing_type(method->declaring_type_definition())
                                                   : !syntax::is<ThisReferenceExpression>(target.expression));
        // Try to find minimal expression
        // If target is required, include it from the start
        bool target_added = require_target;
        TranslatedExpression current_target = target_added ? target : TranslatedExpression();
        // Remember other decisions:
        bool target_casted = false;
        bool add_type_arguments = false;
        // Initial inputs for IsUnambiguousMethodReference:
        ResolveResult* target_resolve_result = target_added ? target.resolve_result : nullptr;
        std::vector<const IType*> type_arguments;
        // Find somewhat minimal solution:
        ResolveResult* result = nullptr;
        while (!is_unambiguous_method_reference(expected_target_details, method, target_resolve_result, type_arguments, false, &result)) {
            if (!add_type_arguments) {
                // try adding type arguments
                add_type_arguments = true;
                type_arguments = method->type_arguments();
                continue;
            }
            if (!target_added) {
                // try adding target
                target_added = true;
                current_target = target;
                target_resolve_result = target.resolve_result;
                continue;
            }
            if (!target_casted) {
                // try casting target
                target_casted = true;
                current_target = current_target.convert_to(target_type, expression_builder_);
                target_resolve_result = current_target.resolve_result;
                continue;
            }
            break;
        }
        if (auto* mgrr = dynamic_cast<MethodGroupResolveResult*>(result)) {
            result = mgrr->with_chosen_method(method);
        }
        return {current_target, add_type_arguments, method->name(), result};
    }
}

TranslatedExpression CallBuilder::handle_delegate_construction(const IType* delegate_type, const IMethod* method,
                                                               const ExpectedTargetDetails& expected_target_details, il::ILInstruction* this_arg,
                                                               il::ILInstruction* inst) {
    const IMethod* invoke_method = typesystem::get_delegate_invoke_method(delegate_type);
    ExpressionWithResolveResult target_expression = build_delegate_reference(method, invoke_method, expected_target_details, this_arg);
    AstType* created_type = expression_builder_.convert_type(delegate_type);
    TranslatedExpression oce =
        with_il_instruction(make<ObjectCreateExpression>(created_type, std::vector<Expression*>{target_expression.expression}), inst)
            .with_rr(make<ConversionResolveResult>(
                delegate_type, target_expression.resolve_result,
                Conversion::method_group_conversion(method, expected_target_details.call_op_code == OpCode::CallVirt, false)));
    return oce;
}

bool CallBuilder::is_unambiguous_method_reference(const ExpectedTargetDetails& expected_target_details, const IMethod* method, ResolveResult* target,
                                                  const std::vector<const IType*>& type_arguments, bool is_extension_method_reference,
                                                  ResolveResult** result) {
    resolver::MemberLookup lookup(resolver_->current_type_definition(), resolver_->current_type_definition()->parent_module());
    std::optional<OverloadResolution> overload_resolution;

    if (is_extension_method_reference) {
        *result = dynamic_cast<MethodGroupResolveResult*>(
            resolver_->resolve_member_access(target, method->name(), type_arguments, resolver::NameLookupMode::InvocationTarget));
        if (*result == nullptr) return false;
        std::vector<ResolveResult*> parameter_results;
        for (const IParameter* p : method->parameters()) parameter_results.push_back(make<TypeResolveResult>(p->type()));
        overload_resolution = resolver::perform_overload_resolution(*static_cast<MethodGroupResolveResult*>(*result), resolver_->current_type_resolve_context().compilation(),
                                                                    parameter_results, {}, true);
        if (overload_resolution->is_ambiguous()) return false;
    } else {
        std::vector<ResolveResult*> parameter_results;  // there are no arguments, use parameter types
        for (const IParameter* p : method->parameters()) parameter_results.push_back(make<TypeResolveResult>(p->type()));
        overload_resolution.emplace(resolver_->compilation(), parameter_results, ArgumentNames{},  // argument names are not possible
                                    type_arguments, &expression_builder_.resolver->conversions());
        if (target == nullptr) {
            *result = resolver_->resolve_simple_name(method->name(), type_arguments, false);
            auto* mgrr = dynamic_cast<MethodGroupResolveResult*>(*result);
            if (mgrr == nullptr) return false;
            overload_resolution->add_method_lists(mgrr->methods_grouped_by_declaring_type());
        } else {
            *result = lookup.lookup(target, method->name(), type_arguments, false);
            auto* mgrr = dynamic_cast<MethodGroupResolveResult*>(*result);
            if (mgrr == nullptr) return false;
            overload_resolution->add_method_lists(mgrr->methods_grouped_by_declaring_type());
        }
    }

    const IParameterizedMember* found_method = overload_resolution->get_best_candidate_with_substituted_type_arguments();
    if (!is_appropriate_call_target(expected_target_details, method, found_method)) return false;
    return dynamic_cast<MethodGroupResolveResult*>(*result) != nullptr;
}

MethodGroupResolveResult* CallBuilder::to_method_group(const IMethod* method, il::ILFunction* local_function) {
    return make<MethodGroupResolveResult>(
        nullptr, local_function->name,
        std::vector<MethodListWithDeclaringType>{MethodListWithDeclaringType(method->declaring_type(), std::vector<const IParameterizedMember*>{method})},
        method->type_arguments());
}

TranslatedExpression CallBuilder::call_with_named_args(Block* block) {
    assert(block->kind == il::BlockKind::CallWithNamedArgs);
    auto* call = static_cast<CallInstruction*>(block->final_instruction());
    std::vector<il::ILInstruction*> arguments(static_cast<std::size_t>(call->arguments().count()), nullptr);
    std::vector<int> argument_to_parameter_map(arguments.size(), 0);
    int first_param_index = call->is_instance_call() ? 1 : 0;
    // Arguments from temporary variables (VariableKind.NamedArgument):
    std::size_t pos = 0;
    for (il::ILInstruction* instruction : block->instructions.items()) {
        auto* stloc = static_cast<il::StLoc*>(instruction);
        assert(stloc->variable()->load_instructions().size() == 1 && stloc->variable()->load_instructions()[0]->parent() == call);
        arguments[pos] = stloc->value();
        argument_to_parameter_map[pos] = stloc->variable()->load_instructions()[0]->child_index() - first_param_index;
        pos++;
    }
    // Remaining argument:
    for (il::ILInstruction* arg : call->arguments().items()) {
        il::ILVariable* v = nullptr;
        if (arg->match_ld_loc(&v) && v->kind() == il::VariableKind::NamedArgument) {
            continue;  // already handled in loop above
        }
        arguments[pos] = arg;
        argument_to_parameter_map[pos] = arg->child_index() - first_param_index;
        pos++;
    }
    assert(pos == arguments.size());
    return build(call->op_code, call->method, arguments, argument_to_parameter_map, call->constrained_to).with_il_instruction(call).with_il_instruction(block);
}

bool CallBuilder::handle_range_construction(ExpressionWithResolveResult* result, OpCode call_op_code, const IMethod* method, const TranslatedExpression& target,
                                            const ArgumentList& argument_list) {
    *result = ExpressionWithResolveResult();
    if (argument_list.argument_names.has_value()) {
        return false;  // range syntax doesn't support named arguments
    }
    if (is_known_type(method->declaring_type(), KnownTypeCode::Range)) {
        if (call_op_code == OpCode::NewObj && argument_list.length() == 2) {
            *result = with_rr(make<BinaryOperatorExpression>(argument_list.arguments[0].expression, BinaryOperatorType::Range, argument_list.arguments[1].expression),
                              make<MemberResolveResult>(nullptr, method));
            return true;
        } else if (call_op_code == OpCode::Call && method->name() == "get_All" && argument_list.length() == 0) {
            const IMember* owner = method->accessor_owner() != nullptr ? method->accessor_owner() : method;
            *result = with_rr(make<BinaryOperatorExpression>(nullptr, BinaryOperatorType::Range, nullptr), make<MemberResolveResult>(nullptr, owner));
            return true;
        } else if (call_op_code == OpCode::Call && method->name() == "StartAt" && argument_list.length() == 1) {
            *result = with_rr(make<BinaryOperatorExpression>(argument_list.arguments[0].expression, BinaryOperatorType::Range, nullptr),
                              make<MemberResolveResult>(nullptr, method));
            return true;
        } else if (call_op_code == OpCode::Call && method->name() == "EndAt" && argument_list.length() == 1) {
            *result = with_rr(make<BinaryOperatorExpression>(nullptr, BinaryOperatorType::Range, argument_list.arguments[0].expression),
                              make<MemberResolveResult>(nullptr, method));
            return true;
        }
    } else if (call_op_code == OpCode::NewObj && is_known_type(method->declaring_type(), KnownTypeCode::Index)) {
        if (argument_list.length() != 2) return false;
        auto* pe = syntax::as<PrimitiveExpression>(argument_list.arguments[1].expression);
        if (!(pe != nullptr && std::holds_alternative<bool>(pe->value()) && std::get<bool>(pe->value()))) return false;
        *result = with_rr(make<UnaryOperatorExpression>(UnaryOperatorType::IndexFromEnd, argument_list.arguments[0].expression),
                          make<MemberResolveResult>(nullptr, method));
        return true;
    } else if (auto* range_index_accessor = dynamic_cast<const typesystem::SyntheticRangeIndexAccessor*>(method);
               range_index_accessor != nullptr && range_index_accessor->is_slicing()) {
        // For slicing the method is called Slice()/Substring(), but we still need to output indexer notation.
        // So special-case range-based slicing here.
        std::vector<Expression*> argument_expressions;
        for (const TranslatedExpression& a : argument_list.arguments) argument_expressions.push_back(a.expression);
        *result = with_rr(make<IndexerExpression>(target.expression, argument_expressions), make<MemberResolveResult>(target.resolve_result, method));
        return true;
    }
    return false;
}

}  // namespace cb::dotnet::decompiler::csharp
