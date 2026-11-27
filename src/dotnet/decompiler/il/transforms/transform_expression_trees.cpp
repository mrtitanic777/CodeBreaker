// dotnet/decompiler/il/transforms/transform_expression_trees.cpp -- see transform_expression_trees.hpp.
#include "dotnet/decompiler/il/transforms/transform_expression_trees.hpp"

#include <cassert>
#include <stdexcept>

#include "core/error.hpp"
#include "dotnet/decompiler/csharp/resolver/csharp_conversions.hpp"
#include "dotnet/decompiler/csharp/resolver/csharp_operators.hpp"
#include "dotnet/decompiler/csharp/resolver/csharp_resolver.hpp"
#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/il/il_instruction_extensions.hpp"
#include "dotnet/decompiler/il/transforms/transform_display_class_usage.hpp"
#include "dotnet/decompiler/typesystem/decompiler_type_system.hpp"
#include "dotnet/decompiler/typesystem/known_type_reference.hpp"
#include "dotnet/decompiler/typesystem/metadata_entities.hpp"
#include "dotnet/decompiler/typesystem/type_arena.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::il::transforms {

using typesystem::KnownTypeCode;
using typesystem::SpecialType;
using typesystem::TypeKind;

namespace {

const IMethod* as_method(const IMember* member) {
    auto* method = dynamic_cast<const IMethod*>(member);
    if (method == nullptr) throw UsageError("Unable to cast object to type IMethod");
    return method;
}

const IField* as_field(const IMember* member) {
    auto* field = dynamic_cast<const IField*>(member);
    if (field == nullptr) throw UsageError("Unable to cast object to type IField");
    return field;
}

CallInstruction* as_call_instruction(ILInstruction* inst) {
    auto* call = dynamic_cast<CallInstruction*>(inst);
    if (call == nullptr) throw UsageError("Unable to cast object to type CallInstruction");
    return call;
}

// type.SkipModifiers() is ByReferenceType
bool is_by_ref(const IType* type) {
    return type != nullptr && dynamic_cast<const typesystem::ByReferenceType*>(typesystem::skip_modifiers(type)) != nullptr;
}

// ParameterizedType pt && pt.FullName == "System.Linq.Expressions.Expression" && pt.TypeArguments.Count == 1
const typesystem::ParameterizedType* as_expression_tree_type(const IType* type) {
    auto* pt = dynamic_cast<const typesystem::ParameterizedType*>(type);
    if (pt != nullptr && pt->full_name() == "System.Linq.Expressions.Expression" && pt->type_arguments().size() == 1) return pt;
    return nullptr;
}

// StackType.ToPrimitiveType()
PrimitiveType stack_type_to_primitive_type(StackType stack_type) { return typesystem::to_primitive_type(stack_type); }

}  // namespace

TransformExpressionTrees::Converted TransformExpressionTrees::fail() { return {nullptr, &SpecialType::unknown_type()}; }
TransformExpressionTrees::ConvertedBind TransformExpressionTrees::fail_bind() { return {nullptr, &SpecialType::unknown_type()}; }

// Returns true if the instruction matches the pattern for Expression.Lambda calls.
bool TransformExpressionTrees::might_be_expression_tree(ILInstruction* inst, ILInstruction* stmt) {
    (void)stmt;
    auto* call = dynamic_cast<CallInstruction*>(inst);
    if (!(call != nullptr && typesystem::full_name_is(call->method, "System.Linq.Expressions.Expression", "Lambda") && call->arguments().count() == 2))
        return false;
    auto* block = as<Block>(call->arguments()[1]);
    if (!(is_empty_parameter_list(call->arguments()[1]) || (block != nullptr && block->kind == BlockKind::ArrayInitializer))) return false;
    return true;
}

bool TransformExpressionTrees::is_empty_parameter_list(ILInstruction* inst) {
    auto* empty_call = dynamic_cast<CallInstruction*>(inst);
    if (empty_call != nullptr && typesystem::full_name_is(empty_call->method, "System.Array", "Empty") && empty_call->arguments().count() == 0) return true;
    const IType* type = nullptr;
    if (inst->match_new_arr(&type) && type->full_name() == "System.Linq.Expressions.ParameterExpression") return true;
    if (inst->match_new_arr(&type) && type->full_name() == "System.Linq.Expressions.Expression") return true;
    return false;
}

bool TransformExpressionTrees::match_parameter_variable_assignment(ILInstruction* expr, ILVariable** parameter_reference_var, const IType** type,
                                                                   std::string* name) {
    // stloc(v, call(Expression::Parameter, call(Type::GetTypeFromHandle, ldtoken(...)), ldstr(...)))
    *type = nullptr;
    name->clear();
    ILInstruction* init = nullptr;
    if (!expr->match_st_loc(parameter_reference_var, &init)) return false;
    if (!(*parameter_reference_var)->is_single_definition()) return false;
    if (!((*parameter_reference_var)->kind() == VariableKind::Local || (*parameter_reference_var)->kind() == VariableKind::StackSlot)) return false;
    if ((*parameter_reference_var)->type() == nullptr ||
        (*parameter_reference_var)->type()->full_name() != "System.Linq.Expressions.ParameterExpression")
        return false;
    auto* init_call = dynamic_cast<CallInstruction*>(init);
    if (!(init_call != nullptr && init_call->arguments().count() == 2)) return false;
    if (!(typesystem::full_name_is(init_call->method, "System.Linq.Expressions.Expression", "Parameter"))) return false;
    auto* type_arg = dynamic_cast<CallInstruction*>(init_call->arguments()[0]);
    if (type_arg == nullptr || type_arg->arguments().count() != 1) return false;
    if (!typesystem::full_name_is(type_arg->method, "System.Type", "GetTypeFromHandle")) return false;
    return type_arg->arguments()[0]->match_ld_type_token(type) && init_call->arguments()[1]->match_ld_str(name);
}

const std::pair<const IType*, std::string>* TransformExpressionTrees::find_parameter(ILVariable* variable) const {
    for (const auto& entry : parameters_)
        if (entry.first == variable) return &entry.second;
    return nullptr;
}

ILVariable* TransformExpressionTrees::find_parameter_mapping(ILVariable* variable) const {
    for (const auto& entry : parameter_mapping_)
        if (entry.first == variable) return entry.second;
    return nullptr;
}

void TransformExpressionTrees::run(Block* block, int pos, StatementTransformContext& context) {
    if (!context.settings().expression_trees) return;
    context_ = &context;
    parameters_.clear();
    parameter_mapping_.clear();
    instructions_to_remove_.clear();
    lambda_stack_.clear();
    for (int i = pos; i < block->instructions.count(); i++) {
        ILVariable* v = nullptr;
        const IType* type = nullptr;
        std::string name;
        if (match_parameter_variable_assignment(block->instructions[i], &v, &type, &name)) {
            if (find_parameter(v) != nullptr) throw std::invalid_argument("An item with the same key has already been added.");
            parameters_.push_back({v, {type, name}});
            continue;
        }
        if (try_convert_expression_tree(block->instructions[i], block->instructions[i])) {
            for (ILInstruction* inst : instructions_to_remove_) block->instructions.remove(inst);
            instructions_to_remove_.clear();
        }
        break;
    }
}

bool TransformExpressionTrees::try_convert_expression_tree(ILInstruction* instruction, ILInstruction* statement) {
    if (might_be_expression_tree(instruction, statement)) {
        Converted converted = convert_lambda(static_cast<CallInstruction*>(instruction));
        if (converted.build) {
            context_->step("Convert Expression Tree", instruction);
            auto* new_lambda = static_cast<ILFunction*>(converted.build());
            set_expression_tree_flag(new_lambda, static_cast<CallInstruction*>(instruction));
            instruction->replace_with(new_lambda);
            context_->end_step(new_lambda);
            return true;
        }
        return false;
    }
    auto* block = as<Block>(instruction);
    if (block != nullptr && block->kind == BlockKind::ControlFlow) return false;  // don't look into nested blocks
    for (ILInstruction* child : instruction->children()) {
        if (try_convert_expression_tree(child, statement)) return true;
    }
    return false;
}

TransformExpressionTrees::Converted TransformExpressionTrees::convert_lambda(CallInstruction* instruction) {
    if (instruction->method->name() != "Lambda" || instruction->arguments().count() != 2 ||
        instruction->method->return_type()->full_name() != "System.Linq.Expressions.Expression" ||
        instruction->method->return_type()->type_arguments().size() != 1)
        return fail();
    std::vector<const typesystem::IParameter*> parameter_list;
    std::vector<ILVariable*> parameter_variables_list;
    if (!read_parameters(instruction->arguments()[1], &parameter_list, &parameter_variables_list)) return fail();
    auto* container = make<BlockContainer>();
    container->add_il_range(*instruction);
    const IType* function_type = instruction->method->return_type()->type_arguments()[0];
    const IMethod* invoke_method = typesystem::get_delegate_invoke_method(function_type);
    const IType* return_type = invoke_method != nullptr ? invoke_method->return_type() : &SpecialType::unknown_type();
    auto* function = make<ILFunction>(return_type, parameter_list, context_->function()->generic_context, container, ILFunctionKind::ExpressionTree);
    function->delegate_type = function_type;
    function->set_kind(is_expression_tree(function_type) ? ILFunctionKind::ExpressionTree : ILFunctionKind::Delegate);
    for (ILVariable* variable : parameter_variables_list) function->variables().add(variable);
    function->add_il_range(*instruction);
    lambda_stack_.push_back(function);
    Converted body = convert_instruction(instruction->arguments()[0]);
    lambda_stack_.pop_back();
    if (!body.build) return fail();

    Converter body_instruction = body.build;
    auto build_function = [this, function, container, instruction, body_instruction]() -> ILInstruction* {
        lambda_stack_.push_back(function);
        ILInstruction* converted_body = body_instruction();
        lambda_stack_.pop_back();
        container->set_expected_result_type(converted_body->result_type());
        auto* block = make<Block>();
        block->instructions.add(make<Leave>(container, converted_body));
        container->blocks.add(block);
        // Replace all other usages of the parameter variable
        for (const auto& mapping : parameter_mapping_) {
            std::vector<LdLoc*> loads = mapping.first->load_instructions();
            for (LdLoc* load : loads) {
                if (load->is_descendant_of(instruction)) continue;
                load->replace_with(make<LdLoc>(mapping.second));
            }
        }
        return function;
    };
    return {build_function, function->delegate_type};
}

TransformExpressionTrees::Converted TransformExpressionTrees::convert_quote(CallInstruction* invocation) {
    if (invocation->arguments().count() != 1) return fail();
    ILInstruction* argument = invocation->arguments()[0];
    if (auto* function = as<ILFunction>(argument)) {
        return {[function]() -> ILInstruction* { return function; }, function->delegate_type};
    } else {
        Converted converted = convert_instruction(argument);
        if (!converted.build) return converted;
        Converter inner = converted.build;
        auto build_quote = [this, inner, argument]() -> ILInstruction* {
            ILInstruction* f = inner();
            auto* lambda = as<ILFunction>(f);
            auto* call = dynamic_cast<CallInstruction*>(argument);
            if (lambda != nullptr && call != nullptr) {
                set_expression_tree_flag(lambda, call);
            }

            return f;
        };
        return {build_quote, converted.type};
    }
}

void TransformExpressionTrees::set_expression_tree_flag(ILFunction* lambda, CallInstruction* call) {
    lambda->set_kind(is_expression_tree(call->method->return_type()) ? ILFunctionKind::ExpressionTree : ILFunctionKind::Delegate);
    lambda->delegate_type = call->method->return_type();
}

bool TransformExpressionTrees::read_parameters(ILInstruction* initializer, std::vector<const typesystem::IParameter*>* parameters,
                                               std::vector<ILVariable*>* parameter_variables) {
    if (auto* initializer_block = as<Block>(initializer)) {
        if (initializer_block->kind != BlockKind::ArrayInitializer) return false;
        int i = 0;
        for (ILInstruction* item : initializer_block->instructions) {
            auto* inst = as<StObj>(item);
            if (inst == nullptr) continue;
            if (i >= static_cast<int>(parameters_.size())) return false;
            ILVariable* v = nullptr;
            if (!inst->value()->match_ld_loc(&v)) return false;
            const auto* value = find_parameter(v);
            if (value == nullptr) return false;
            // Add parameter variable only once to mapping.
            if (find_parameter_mapping(v) == nullptr) {
                auto* param = make<ILVariable>(VariableKind::Parameter, value->first, i);
                param->name = value->second;
                parameter_mapping_.push_back({v, param});
                parameter_variables->push_back(param);
                parameters->push_back(context_->type_system().arena().keep(std::make_unique<typesystem::DefaultParameter>(value->first, value->second)));
                instructions_to_remove_.push_back(static_cast<ILInstruction*>(v->store_instructions().at(0)->as_instruction()));
            }
            i++;
        }
        return true;
    }
    return is_empty_parameter_list(initializer);
}

TransformExpressionTrees::Converted TransformExpressionTrees::convert_instruction(ILInstruction* instruction, const IType* type_hint) {
    Converted converted = convert_instruction_core(instruction, type_hint);
    if (!converted.build) return {nullptr, converted.type};

    Converter inst = converted.build;
    const IType* type = converted.type;
    auto do_convert = [inst, type, type_hint]() -> ILInstruction* {
        ILInstruction* result = inst();
        assert(type != nullptr && "IType must be non-null!");
        assert(result->result_type() == typesystem::get_stack_type(*type) && "StackTypes must match!");
        if (type_hint != nullptr) {
            if (result->result_type() != typesystem::get_stack_type(*type_hint)) {
                return make<Conv>(result, stack_type_to_primitive_type(typesystem::get_stack_type(*type_hint)), false, typesystem::get_sign(*type_hint));
            }
        }
        return result;
    };
    return {do_convert, type_hint != nullptr ? type_hint : type};
}

TransformExpressionTrees::Converted TransformExpressionTrees::convert_instruction_core(ILInstruction* instruction, const IType* type_hint) {
    if (auto* invocation = dynamic_cast<CallInstruction*>(instruction)) {
        if (invocation->method->declaring_type()->full_name() != "System.Linq.Expressions.Expression") return fail();

        const std::string name = invocation->method->name();
        if (name == "Add") return convert_binary_numeric_operator(invocation, BinaryNumericOperator::Add, false);
        if (name == "AddChecked") return convert_binary_numeric_operator(invocation, BinaryNumericOperator::Add, true);
        if (name == "And") return convert_binary_numeric_operator(invocation, BinaryNumericOperator::BitAnd);
        if (name == "AndAlso") return convert_logic_operator(invocation, true);
        if (name == "ArrayAccess" || name == "ArrayIndex") return convert_array_index(invocation);
        if (name == "ArrayLength") return convert_array_length(invocation);
        if (name == "Call") return convert_call(invocation);
        if (name == "Coalesce") return convert_coalesce(invocation);
        if (name == "Condition") return convert_condition(invocation);
        if (name == "Constant") return convert_constant(invocation);
        if (name == "Convert") return convert_cast(invocation, false);
        if (name == "ConvertChecked") return convert_cast(invocation, true);
        if (name == "Divide") return convert_binary_numeric_operator(invocation, BinaryNumericOperator::Div);
        if (name == "Equal") return convert_comparison(invocation, ComparisonKind::Equality);
        if (name == "ExclusiveOr") return convert_binary_numeric_operator(invocation, BinaryNumericOperator::BitXor);
        if (name == "Field") return convert_field(invocation, type_hint);
        if (name == "GreaterThan") return convert_comparison(invocation, ComparisonKind::GreaterThan);
        if (name == "GreaterThanOrEqual") return convert_comparison(invocation, ComparisonKind::GreaterThanOrEqual);
        if (name == "Invoke") return convert_invoke(invocation);
        if (name == "Lambda") return convert_lambda(invocation);
        if (name == "LeftShift") return convert_binary_numeric_operator(invocation, BinaryNumericOperator::ShiftLeft);
        if (name == "LessThan") return convert_comparison(invocation, ComparisonKind::LessThan);
        if (name == "LessThanOrEqual") return convert_comparison(invocation, ComparisonKind::LessThanOrEqual);
        if (name == "ListInit") return convert_list_init(invocation);
        if (name == "MemberInit") return convert_member_init(invocation);
        if (name == "Modulo") return convert_binary_numeric_operator(invocation, BinaryNumericOperator::Rem);
        if (name == "Multiply") return convert_binary_numeric_operator(invocation, BinaryNumericOperator::Mul, false);
        if (name == "MultiplyChecked") return convert_binary_numeric_operator(invocation, BinaryNumericOperator::Mul, true);
        if (name == "Negate") return convert_unary_numeric_operator(invocation, BinaryNumericOperator::Sub, false);
        if (name == "NegateChecked") return convert_unary_numeric_operator(invocation, BinaryNumericOperator::Sub, true);
        if (name == "New") return convert_new_object(invocation);
        if (name == "NewArrayBounds") return convert_new_array_bounds(invocation);
        if (name == "NewArrayInit") return convert_new_array_init(invocation);
        if (name == "Not") return convert_not_operator(invocation);
        if (name == "NotEqual") return convert_comparison(invocation, ComparisonKind::Inequality);
        if (name == "OnesComplement") return convert_not_operator(invocation);
        if (name == "Or") return convert_binary_numeric_operator(invocation, BinaryNumericOperator::BitOr);
        if (name == "OrElse") return convert_logic_operator(invocation, false);
        if (name == "Property") return convert_property(invocation);
        if (name == "Quote") return convert_quote(invocation);
        if (name == "RightShift") return convert_binary_numeric_operator(invocation, BinaryNumericOperator::ShiftRight);
        if (name == "Subtract") return convert_binary_numeric_operator(invocation, BinaryNumericOperator::Sub, false);
        if (name == "SubtractChecked") return convert_binary_numeric_operator(invocation, BinaryNumericOperator::Sub, true);
        if (name == "TypeAs") return convert_type_as(invocation);
        if (name == "TypeIs") return convert_type_is(invocation);
        return fail();
    }
    if (auto* function = as<ILFunction>(instruction)) {
        auto apply_changes_to_il_function = [this, function]() -> ILInstruction* {
            if (function->kind() == ILFunctionKind::ExpressionTree) {
                function->delegate_type = unwrap_expression_tree(function->delegate_type);
                function->set_kind(ILFunctionKind::Delegate);
            }
            return function;
        };
        return {apply_changes_to_il_function, function->delegate_type};
    }
    if (auto* ldloc = as<LdLoc>(instruction)) {
        if (is_expression_tree_parameter(ldloc->variable())) {
            // Replace an already mapped parameter with the actual ILVariable, we generated earlier.
            if (ILVariable* v = find_parameter_mapping(ldloc->variable())) {
                if (is_by_ref(type_hint) && !v->type()->is_by_ref_like()) return {[v]() -> ILInstruction* { return make<LdLoca>(v); }, type_hint};
                return {[v]() -> ILInstruction* { return make<LdLoc>(v); }, v->type()};
            }
            // This is a parameter variable from an outer scope. We can't replace these variables just yet, because the transform works
            // backwards. We simply return the same instruction again, but return the actual expected type, so our transform can continue
            // normally. Later, we will replace all references to unmapped variables, with references to mapped parameters.
            if (ldloc->variable()->is_single_definition()) {
                ILInstruction* instr = ldloc->variable()->store_instructions().at(0)->as_instruction();
                ILVariable* unused_variable = nullptr;
                const IType* t = nullptr;
                std::string unused_name;
                if (match_parameter_variable_assignment(instr, &unused_variable, &t, &unused_name)) {
                    return {[t, ldloc]() -> ILInstruction* { return make<ExpressionTreeCast>(t, ldloc, false); }, t};
                }
            }
        }
        return fail();
    }
    return fail();
}

bool TransformExpressionTrees::is_expression_tree(const IType* delegate_type) { return as_expression_tree_type(delegate_type) != nullptr; }

const IType* TransformExpressionTrees::unwrap_expression_tree(const IType* delegate_type) {
    if (auto* pt = as_expression_tree_type(delegate_type)) return pt->type_arguments()[0];
    return delegate_type;
}

TransformExpressionTrees::Converted TransformExpressionTrees::convert_array_index(CallInstruction* invocation) {
    if (invocation->arguments().count() != 2) return fail();
    Converted array = convert_instruction(invocation->arguments()[0]);
    if (!array.build) return fail();
    auto* type = dynamic_cast<const typesystem::ArrayType*>(array.type);
    if (type == nullptr) return fail();
    std::vector<ILInstruction*> arguments;
    if (!match_argument_list(invocation->arguments()[1], &arguments)) arguments = {invocation->arguments()[1]};

    Converter array_builder = array.build;
    auto convert = [this, arguments, array_builder, type]() -> ILInstruction* {
        std::vector<Converter> to_be_converted(arguments.size());
        for (std::size_t i = 0; i < arguments.size(); i++) {
            Converted converted = convert_instruction(arguments[i]);
            if (!converted.build) return nullptr;
            to_be_converted[i] = converted.build;
        }
        ILInstruction* array_inst = array_builder();
        std::vector<ILInstruction*> indices;
        for (const auto& f : to_be_converted) indices.push_back(f());
        auto* ld_elema = make<LdElema>(type->element_type(), array_inst, indices);
        ld_elema->delay_exceptions = true;
        return make<LdObj>(ld_elema, type->element_type());
    };
    return {convert, type->element_type()};
}

TransformExpressionTrees::Converted TransformExpressionTrees::convert_array_length(CallInstruction* invocation) {
    if (invocation->arguments().count() != 1) return fail();
    Converted converted = convert_instruction(invocation->arguments()[0]);
    if (!converted.build) return fail();
    Converter inner = converted.build;
    return {[inner]() -> ILInstruction* { return make<LdLen>(StackType::I4, inner()); }, context_->type_system().find_type(KnownTypeCode::Int32)};
}

TransformExpressionTrees::Converted TransformExpressionTrees::convert_binary_numeric_operator(CallInstruction* invocation, BinaryNumericOperator op,
                                                                                              std::optional<bool> is_checked) {
    if (invocation->arguments().count() < 2) return fail();
    Converted left = convert_instruction(invocation->arguments()[0]);
    if (!left.build) return fail();
    Converted right = convert_instruction(invocation->arguments()[1]);
    if (!right.build) return fail();
    const IType* left_type = left.type;
    const IType* right_type = right.type;
    Converter left_builder = left.build;
    Converter right_builder = right.build;
    const IMember* method = nullptr;
    switch (invocation->arguments().count()) {
        case 2: {
            if (op == BinaryNumericOperator::ShiftLeft || op == BinaryNumericOperator::ShiftRight) {
                if (!typesystem::is_known_type(typesystem::nullable_type::get_underlying_type(right_type), KnownTypeCode::Int32)) return fail();
            } else {
                if (!typesystem::type_equals(right_type, left_type)) return fail();
            }
            bool checked_ = is_checked == true;
            return {[op, left_builder, right_builder, left_type, right_type, checked_]() -> ILInstruction* {
                        ILInstruction* l = left_builder();
                        ILInstruction* r = right_builder();
                        return make<BinaryNumericInstruction>(
                            op, l, r, typesystem::get_stack_type(*typesystem::nullable_type::get_underlying_type(left_type)),
                            typesystem::get_stack_type(*typesystem::nullable_type::get_underlying_type(right_type)), checked_,
                            typesystem::get_sign(*left_type),
                            typesystem::nullable_type::is_nullable(left_type) || typesystem::nullable_type::is_nullable(right_type));
                    },
                    left_type};
        }
        case 3: {
            if (!match_get_method_from_handle(invocation->arguments()[2], &method)) return fail();
            const IMethod* call_method = as_method(method);
            return {[left_builder, right_builder, call_method]() -> ILInstruction* {
                        auto* call = make<Call>(call_method);
                        call->arguments().add(left_builder());
                        call->arguments().add(right_builder());
                        return call;
                    },
                    method->return_type()};
        }
        case 4: {
            std::int32_t is_lifted_to_null = 0;
            if (!invocation->arguments()[2]->match_ldc_i4(&is_lifted_to_null)) return fail();
            if (!match_get_method_from_handle(invocation->arguments()[3], &method)) return fail();
            bool is_lifted = typesystem::nullable_type::is_nullable(left_type);
            if (is_lifted) method = csharp::resolver::CSharpOperators::lift_user_defined_operator(as_method(method));
            const IMethod* call_method = as_method(method);
            const IType* result_type = is_lifted_to_null != 0 ? typesystem::nullable_type::create(method->compilation(), method->return_type()) : method->return_type();
            return {[left_builder, right_builder, call_method]() -> ILInstruction* {
                        auto* call = make<Call>(call_method);
                        call->arguments().add(left_builder());
                        call->arguments().add(right_builder());
                        return call;
                    },
                    result_type};
        }
        default: return fail();
    }
}

TransformExpressionTrees::ConvertedBind TransformExpressionTrees::convert_bind(CallInstruction* invocation) {
    if (invocation->arguments().count() != 2) return fail_bind();
    Converted value = convert_instruction(invocation->arguments()[1]);
    if (!value.build) return fail_bind();
    const IMember* member = nullptr;
    if (match_get_method_from_handle(invocation->arguments()[0], &member)) {
        const IMethod* method = as_method(member);
        // It is possible to use Expression.Bind with a get-accessor, however, it would be an invalid expression tree if the property is
        // readonly. As this is an assignment, the ILAst expects a set-accessor. To avoid any problems constructing property assignments,
        // we explicitly use the set-accessor instead.
        auto* property = dynamic_cast<const typesystem::IProperty*>(method->accessor_owner());
        if (property != nullptr && property->can_set() && method != property->setter()) {
            member = property->setter();
        }
    } else if (match_get_field_from_handle(invocation->arguments()[0], &member)) {
    } else {
        return fail_bind();
    }
    Converter value_builder = value.build;
    if (auto* method = dynamic_cast<const IMethod*>(member)) {
        if (method->is_static())
            return {[method, value_builder](ILVariable* target_variable) -> ILInstruction* {
                        auto* call = make<Call>(method);
                        call->arguments().add(make<LdLoc>(target_variable));
                        call->arguments().add(value_builder());
                        return call;
                    },
                    method->return_type()};
        else
            return {[method, value_builder](ILVariable* target_variable) -> ILInstruction* {
                        auto* call = make<CallVirt>(method);
                        call->arguments().add(make<LdLoc>(target_variable));
                        call->arguments().add(value_builder());
                        return call;
                    },
                    method->return_type()};
    }
    if (auto* field = dynamic_cast<const IField*>(member)) {
        return {[field, value_builder](ILVariable* target_variable) -> ILInstruction* {
                    auto* ld_flda = make<LdFlda>(make<LdLoc>(target_variable), field);
                    ld_flda->delay_exceptions = true;
                    ILInstruction* v = value_builder();
                    return make<StObj>(ld_flda, v, field->return_type());
                },
                field->return_type()};
    }
    return fail_bind();
}

TransformExpressionTrees::Converted TransformExpressionTrees::convert_call(CallInstruction* invocation) {
    if (invocation->arguments().count() < 2) return fail();
    std::vector<ILInstruction*> arguments;
    bool have_arguments = false;
    Converter target_converter;
    const IType* target_type = nullptr;
    const IMember* member = nullptr;
    if (match_get_method_from_handle(invocation->arguments()[0], &member)) {
        // static method
        have_arguments = true;
        if (invocation->arguments().count() != 2 || !match_argument_list(invocation->arguments()[1], &arguments)) {
            arguments.clear();
            for (int i = 1; i < invocation->arguments().count(); i++) arguments.push_back(invocation->arguments()[i]);
        }
    } else if (match_get_method_from_handle(invocation->arguments()[1], &member)) {
        have_arguments = true;
        if (invocation->arguments().count() != 3 || !match_argument_list(invocation->arguments()[2], &arguments)) {
            arguments.clear();
            for (int i = 2; i < invocation->arguments().count(); i++) arguments.push_back(invocation->arguments()[i]);
        }
        if (!invocation->arguments()[0]->match_ld_null()) {
            Converted target = convert_instruction(invocation->arguments()[0]);
            target_converter = target.build;
            target_type = target.type;
            if (!target_converter) return fail();
        }
    }
    if (!have_arguments) return fail();
    const IMethod* method = as_method(member);
    auto converted_arguments = convert_call_arguments(arguments, method);
    if (!converted_arguments) return fail();
    if (method->full_name() == "System.Reflection.MethodInfo.CreateDelegate" && method->parameters().size() == 2) {
        const IMember* target_method = nullptr;
        if (!match_get_method_from_handle(unpack_constant(invocation->arguments()[0]), &target_method)) return fail();
        const IType* delegate_type = nullptr;
        if (!match_get_type_from_handle(unpack_constant(arguments.at(0)), &delegate_type)) return fail();
        Converter second = converted_arguments->at(1);
        const IMethod* ftn_method = as_method(target_method);
        return {[delegate_type, second, ftn_method]() -> ILInstruction* {
                    auto constructors = delegate_type->get_constructors();
                    if (constructors.size() != 1) throw UsageError("Sequence contains no matching element or more than one");
                    auto* new_obj = make<NewObj>(constructors[0]);
                    new_obj->arguments().add(second());
                    new_obj->arguments().add(make<LdFtn>(ftn_method));
                    return new_obj;
                },
                delegate_type};
    }

    std::vector<Converter> args = *converted_arguments;
    auto build_call = [this, method, target_converter, target_type, args]() -> ILInstruction* {
        CallInstruction* call;
        if (method->is_static()) {
            call = make<Call>(method);
        } else {
            call = make<CallVirt>(method);
        }
        if (target_converter) {
            call->arguments().add(prepare_call_target(method->declaring_type(), target_converter(), target_type));
        }
        for (const auto& f : args) call->arguments().add(f());
        return call;
    };
    return {build_call, method->return_type()};
}

ILInstruction* TransformExpressionTrees::prepare_call_target(const IType* expected_type, ILInstruction* target, const IType* target_type) {
    ILInstruction* result;
    switch (CallInstruction::expected_type_for_this_pointer(*expected_type, nullptr)) {
        case StackType::Ref:
            if (target->result_type() == StackType::Ref) {
                result = target;
            } else if (auto* ldloc = as<LdLoc>(target)) {
                result = with_il_range(make<LdLoca>(ldloc->variable()), ldloc);
            } else {
                result = make<AddressOf>(target, expected_type);
            }
            break;
        case StackType::O: {
            std::optional<bool> is_reference_type = target_type->is_reference_type();
            if (is_reference_type.has_value() && !*is_reference_type) {
                result = make<Box>(target, target_type);
            } else {
                result = target;
            }
            break;
        }
        default: result = target; break;
    }

    if (expected_type->kind() == TypeKind::Unknown && result->result_type() != StackType::Unknown) {
        result = make<Conv>(target, PrimitiveType::Unknown, false, Sign::None);
    } else if (expected_type->kind() != TypeKind::Unknown && result->result_type() == StackType::Unknown) {
        // if references are missing, we need to coerce the unknown type to the expected type. Otherwise we will get loads of assertions
        // and expression trees are usually explicit about any conversions.
        result = make<Conv>(result, typesystem::to_primitive_type(*expected_type), false, Sign::None);
    }

    return result;
}

ILInstruction* TransformExpressionTrees::unpack_constant(ILInstruction* inst) {
    auto* call = dynamic_cast<CallInstruction*>(inst);
    if (!(call != nullptr && call->method->full_name() == "System.Linq.Expressions.Expression.Constant" && call->arguments().count() == 2)) return inst;
    return call->arguments()[0];
}

std::optional<std::vector<TransformExpressionTrees::Converter>> TransformExpressionTrees::convert_call_arguments(
    const std::vector<ILInstruction*>& arguments, const IMethod* method) {
    std::vector<Converter> converted(arguments.size());
    assert(arguments.size() == method->parameters().size());
    for (std::size_t i = 0; i < arguments.size(); i++) {
        const IType* expected_type = method->parameters()[i]->type();
        Converter argument = convert_instruction(arguments[i], expected_type).build;
        if (!argument) return std::nullopt;
        converted[i] = argument;
    }
    return converted;
}

TransformExpressionTrees::Converted TransformExpressionTrees::convert_cast(CallInstruction* invocation, bool is_checked) {
    if (invocation->arguments().count() < 2) return fail();
    const IType* target_type = nullptr;
    if (!match_get_type_from_handle(invocation->arguments()[1], &target_type)) return fail();
    Converted expr = convert_instruction(invocation->arguments()[0]);
    if (!expr.build) return fail();
    if (typesystem::is_small_integer_type(*expr.type) && typesystem::is_known_type(target_type, KnownTypeCode::Int32)) return {expr.build, target_type};
    Converter inner = expr.build;
    return {[target_type, inner, is_checked]() -> ILInstruction* { return make<ExpressionTreeCast>(target_type, inner(), is_checked); }, target_type};
}

TransformExpressionTrees::Converted TransformExpressionTrees::convert_coalesce(CallInstruction* invocation) {
    if (invocation->arguments().count() != 2) return fail();
    Converted true_inst = convert_instruction(invocation->arguments()[0]);
    if (!true_inst.build) return fail();
    Converted fallback_inst = convert_instruction(invocation->arguments()[1]);
    if (!fallback_inst.build) return fail();
    NullCoalescingKind kind = NullCoalescingKind::Ref;
    const IType* true_inst_type_non_nullable = typesystem::nullable_type::get_underlying_type(true_inst.type);
    const IType* target_type;
    if (typesystem::nullable_type::is_nullable(true_inst.type) && csharp::resolver::CSharpConversions::get(context_->type_system()).implicit_conversion(fallback_inst.type, true_inst_type_non_nullable)->is_valid()) {
        target_type = true_inst_type_non_nullable;
        kind = typesystem::nullable_type::is_nullable(fallback_inst.type) ? NullCoalescingKind::Nullable : NullCoalescingKind::NullableWithValueFallback;
    } else if (csharp::resolver::CSharpConversions::get(context_->type_system()).implicit_conversion(fallback_inst.type, true_inst.type)->is_valid()) {
        target_type = true_inst.type;
    } else {
        target_type = fallback_inst.type;
    }
    Converter true_builder = true_inst.build;
    Converter fallback_builder = fallback_inst.build;
    StackType underlying_result_type = typesystem::get_stack_type(*true_inst_type_non_nullable);
    return {[kind, true_builder, fallback_builder, underlying_result_type]() -> ILInstruction* {
                ILInstruction* value = true_builder();
                ILInstruction* fallback = fallback_builder();
                auto* coalescing = make<NullCoalescingInstruction>(kind, value, fallback);
                coalescing->underlying_result_type = underlying_result_type;
                return coalescing;
            },
            target_type};
}

TransformExpressionTrees::Converted TransformExpressionTrees::convert_comparison(CallInstruction* invocation, ComparisonKind kind) {
    if (invocation->arguments().count() < 2) return fail();
    Converted left = convert_instruction(invocation->arguments()[0]);
    if (!left.build) return fail();
    Converted right = convert_instruction(invocation->arguments()[1]);
    if (!right.build) return fail();
    Converter left_builder = left.build;
    Converter right_builder = right.build;
    const IType* left_type = left.type;
    const IType* right_type = right.type;
    std::int32_t is_lifted_to_null = 0;
    const IMember* method = nullptr;
    if (invocation->arguments().count() == 4 && invocation->arguments()[2]->match_ldc_i4(&is_lifted_to_null) &&
        match_get_method_from_handle(invocation->arguments()[3], &method)) {
        bool is_lifted = typesystem::nullable_type::is_nullable(left_type);
        if (is_lifted) method = csharp::resolver::CSharpOperators::lift_user_defined_operator(as_method(method));
        const IMethod* call_method = as_method(method);
        return {[left_builder, right_builder, call_method]() -> ILInstruction* {
                    auto* call = make<Call>(call_method);
                    call->arguments().add(left_builder());
                    call->arguments().add(right_builder());
                    return call;
                },
                is_lifted_to_null != 0 ? typesystem::nullable_type::create(method->compilation(), method->return_type()) : method->return_type()};
    }
    auto* rr = dynamic_cast<semantics::OperatorResolveResult*>(context_->csharp_resolver().resolve_binary_operator(
        to_binary_operator_type(kind), make<semantics::ResolveResult>(left_type), make<semantics::ResolveResult>(right_type)));
    if (rr != nullptr && !rr->is_error() && rr->user_defined_operator_method() != nullptr) {
        const IMethod* user_defined_operator = rr->user_defined_operator_method();
        return {[left_builder, right_builder, user_defined_operator]() -> ILInstruction* {
                    auto* call = make<Call>(user_defined_operator);
                    call->arguments().add(left_builder());
                    call->arguments().add(right_builder());
                    return call;
                },
                user_defined_operator->return_type()};
    }
    if (typesystem::is_known_type(left_type, KnownTypeCode::String) && typesystem::is_known_type(right_type, KnownTypeCode::String)) {
        const IMethod* operator_method = nullptr;
        const char* operator_name;
        switch (kind) {
            case ComparisonKind::Equality: operator_name = "op_Equality"; break;
            case ComparisonKind::Inequality: operator_name = "op_Inequality"; break;
            default: return fail();
        }
        auto candidates = left_type->get_methods([operator_name](const IMethod* m) {
            return m->is_operator() && m->name() == operator_name && m->parameters().size() == 2;
        });
        for (const IMethod* m : candidates) {
            if (typesystem::is_known_type(m->parameters()[0]->type(), KnownTypeCode::String) &&
                typesystem::is_known_type(m->parameters()[1]->type(), KnownTypeCode::String)) {
                operator_method = m;
                break;
            }
        }
        if (operator_method == nullptr) return fail();
        return {[left_builder, right_builder, operator_method]() -> ILInstruction* {
                    auto* call = make<Call>(operator_method);
                    call->arguments().add(left_builder());
                    call->arguments().add(right_builder());
                    return call;
                },
                operator_method->return_type()};
    }
    const IType* result_type = context_->type_system().find_type(KnownTypeCode::Boolean);
    ComparisonLiftingKind lifting = typesystem::nullable_type::is_nullable(left_type) ? ComparisonLiftingKind::CSharp : ComparisonLiftingKind::None;
    const IType* utype = typesystem::nullable_type::get_underlying_type(left_type);
    return {[kind, lifting, utype, left_builder, right_builder]() -> ILInstruction* {
                ILInstruction* l = left_builder();
                ILInstruction* r = right_builder();
                return make<Comp>(kind, lifting, typesystem::get_stack_type(*utype), typesystem::get_sign(*utype), l, r);
            },
            result_type};
}

TransformExpressionTrees::Converted TransformExpressionTrees::convert_condition(CallInstruction* invocation) {
    if (invocation->arguments().count() != 3) return fail();
    Converted condition = convert_instruction(invocation->arguments()[0]);
    if (!condition.build || !typesystem::is_known_type(condition.type, KnownTypeCode::Boolean)) return fail();
    Converted true_inst = convert_instruction(invocation->arguments()[1]);
    if (!true_inst.build) return fail();
    Converted false_inst = convert_instruction(invocation->arguments()[2]);
    if (!false_inst.build) return fail();
    if (!typesystem::NormalizeTypeVisitor::type_erasure().equivalent_types(true_inst.type, false_inst.type)) return fail();
    Converter c = condition.build;
    Converter t = true_inst.build;
    Converter f = false_inst.build;
    return {[c, t, f]() -> ILInstruction* {
                ILInstruction* cond = c();
                ILInstruction* true_value = t();
                ILInstruction* false_value = f();
                return make<IfInstruction>(cond, true_value, false_value);
            },
            true_inst.type};
}

TransformExpressionTrees::Converted TransformExpressionTrees::convert_constant(CallInstruction* invocation) {
    ILInstruction* value = nullptr;
    const IType* type = nullptr;
    if (!match_constant_call(invocation, &value, &type)) return fail();
    ILInstruction* arg = nullptr;
    const IType* box_type = nullptr;
    if (value->match_box(&arg, &box_type)) {
        if (box_type->kind() == TypeKind::Enum || typesystem::is_known_type(box_type, KnownTypeCode::Boolean))
            return {[this, box_type, arg, invocation]() -> ILInstruction* { return make<ExpressionTreeCast>(box_type, convert_value(arg, invocation), false); },
                    box_type};
        return {[this, arg, invocation]() -> ILInstruction* { return convert_value(arg, invocation); }, type};
    }
    return {[this, value, invocation]() -> ILInstruction* { return convert_value(value, invocation); }, type};
}

TransformExpressionTrees::Converted TransformExpressionTrees::convert_element_init(CallInstruction* invocation) {
    if (invocation->arguments().count() != 2) return fail();
    const IMember* member = nullptr;
    if (!match_get_method_from_handle(invocation->arguments()[0], &member)) return fail();
    std::vector<ILInstruction*> arguments;
    if (!match_argument_list(invocation->arguments()[1], &arguments)) return fail();
    std::vector<Converter> args(arguments.size());
    for (std::size_t i = 0; i < arguments.size(); i++) {
        Converter arg = convert_instruction(arguments[i]).build;
        if (!arg) return fail();
        args[i] = arg;
    }

    const IMethod* method = as_method(member);
    auto build_call = [method, args]() -> ILInstruction* {
        CallInstruction* call = method->is_static() ? static_cast<CallInstruction*>(make<Call>(method)) : make<CallVirt>(method);
        for (const auto& f : args) call->arguments().add(f());
        return call;
    };
    return {build_call, member->return_type()};
}

TransformExpressionTrees::Converted TransformExpressionTrees::convert_field(CallInstruction* invocation, const IType* type_hint) {
    if (invocation->arguments().count() != 2) return fail();
    Converter target_converter;
    if (!invocation->arguments()[0]->match_ld_null()) {
        target_converter = convert_instruction(invocation->arguments()[0]).build;
        if (!target_converter) return fail();
    }
    const IMember* member = nullptr;
    if (!match_get_field_from_handle(invocation->arguments()[1], &member)) return fail();
    const IType* type = member->return_type();
    if (is_by_ref(type_hint) && !member->return_type()->is_by_ref_like()) {
        type = type_hint;
    }
    auto build_field = [target_converter, member, type_hint]() -> ILInstruction* {
        ILInstruction* inst;
        if (!target_converter) {
            inst = make<LdsFlda>(as_field(member));
        } else {
            ILInstruction* target = target_converter();
            if (member->declaring_type()->is_reference_type() == std::optional<bool>(true)) {
                auto* ld_flda = make<LdFlda>(target, as_field(member));
                ld_flda->delay_exceptions = true;
                inst = ld_flda;
            } else {
                auto* address_of = make<AddressOf>(target, member->declaring_type());
                auto* ld_flda = make<LdFlda>(address_of, as_field(member));
                ld_flda->delay_exceptions = true;
                inst = ld_flda;
            }
        }
        if (!(is_by_ref(type_hint) && !member->return_type()->is_by_ref_like())) {
            inst = make<LdObj>(inst, member->return_type());
        }
        return inst;
    };
    return {build_field, type};
}

TransformExpressionTrees::Converted TransformExpressionTrees::convert_invoke(CallInstruction* invocation) {
    if (invocation->arguments().count() != 2) return fail();
    Converted target = convert_instruction(invocation->arguments()[0]);
    if (!target.build) return fail();
    const IMethod* invoke_method = typesystem::get_delegate_invoke_method(target.type);
    if (invoke_method == nullptr) return fail();
    std::vector<ILInstruction*> arguments;
    if (!match_argument_list(invocation->arguments()[1], &arguments)) return fail();
    auto converted_arguments = convert_call_arguments(arguments, invoke_method);
    if (!converted_arguments) return fail();

    Converter target_converter = target.build;
    std::vector<Converter> args = *converted_arguments;
    auto build_call = [invoke_method, target_converter, args]() -> ILInstruction* {
        auto* call = make<CallVirt>(invoke_method);
        call->arguments().add(target_converter());
        for (const auto& f : args) call->arguments().add(f());
        return call;
    };
    return {build_call, invoke_method->return_type()};
}

TransformExpressionTrees::Converted TransformExpressionTrees::convert_list_init(CallInstruction* invocation) {
    if (invocation->arguments().count() < 2) return fail();
    Converter new_obj = convert_instruction(invocation->arguments()[0]).build;
    if (!new_obj) return fail();
    const IMethod* ctor = nullptr;
    if (!match_new(as_call_instruction(invocation->arguments()[0]), &ctor)) return fail();
    std::vector<ILInstruction*> arguments;
    const IMember* member = nullptr;
    if (!match_get_method_from_handle(invocation->arguments()[1], &member)) {
        if (!match_argument_list(invocation->arguments()[1], &arguments)) return fail();
    } else {
        if (invocation->arguments().count() != 3 || !match_argument_list(invocation->arguments()[2], &arguments)) return fail();
    }
    if (arguments.empty()) return fail();
    std::vector<BindConverter> converted_arguments(arguments.size());
    for (std::size_t i = 0; i < arguments.size(); i++) {
        auto* element_init = dynamic_cast<CallInstruction*>(arguments[i]);
        if (element_init != nullptr && element_init->method->full_name() == "System.Linq.Expressions.Expression.ElementInit") {
            Converter arg = convert_element_init(element_init).build;
            if (!arg) return fail();

            converted_arguments[i] = [arg](ILVariable* v) -> ILInstruction* {
                ILInstruction* a = arg();
                auto* call = as_call_instruction(a);
                call->arguments().insert(0, make<LdLoc>(v));
                return a;
            };
        } else {
            Converter arg = convert_instruction(arguments[i]).build;
            if (!arg) return fail();
            converted_arguments[i] = [arg](ILVariable*) -> ILInstruction* { return arg(); };
        }
    }

    auto build_block = [this, new_obj, ctor, converted_arguments]() -> ILInstruction* {
        auto* initializer_block = make<Block>(BlockKind::CollectionInitializer);

        ILFunction* function = lambda_stack_.back();
        ILVariable* initializer = function->register_variable(VariableKind::InitializerTarget, ctor->declaring_type());
        initializer_block->set_final_instruction(make<LdLoc>(initializer));
        initializer_block->instructions.add(make<StLoc>(initializer, new_obj()));
        for (const auto& f : converted_arguments) initializer_block->instructions.add(f(initializer));
        return initializer_block;
    };
    return {build_block, ctor->declaring_type()};
}

TransformExpressionTrees::Converted TransformExpressionTrees::convert_logic_operator(CallInstruction* invocation, bool and_) {
    if (invocation->arguments().count() < 2) return fail();
    Converted left = convert_instruction(invocation->arguments()[0]);
    if (!left.build) return fail();
    Converted right = convert_instruction(invocation->arguments()[1]);
    if (!right.build) return fail();
    Converter left_builder = left.build;
    Converter right_builder = right.build;
    const IMember* method = nullptr;
    switch (invocation->arguments().count()) {
        case 2: {
            const IType* result_type = context_->type_system().find_type(KnownTypeCode::Boolean);
            return {[and_, left_builder, right_builder]() -> ILInstruction* {
                        ILInstruction* l = left_builder();
                        ILInstruction* r = right_builder();
                        return and_ ? IfInstruction::logic_and(l, r) : IfInstruction::logic_or(l, r);
                    },
                    result_type};
        }
        case 3: {
            if (!match_get_method_from_handle(invocation->arguments()[2], &method)) return fail();
            const IMethod* call_method = as_method(method);
            return {[left_builder, right_builder, call_method]() -> ILInstruction* {
                        auto* call = make<Call>(call_method);
                        call->arguments().add(left_builder());
                        call->arguments().add(right_builder());
                        return call;
                    },
                    method->return_type()};
        }
        case 4: {
            std::int32_t is_lifted_to_null = 0;
            if (!invocation->arguments()[2]->match_ldc_i4(&is_lifted_to_null)) return fail();
            if (!match_get_method_from_handle(invocation->arguments()[3], &method)) return fail();
            bool is_lifted = typesystem::nullable_type::is_nullable(left.type);
            if (is_lifted) method = csharp::resolver::CSharpOperators::lift_user_defined_operator(as_method(method));
            const IMethod* call_method = as_method(method);
            return {[left_builder, right_builder, call_method]() -> ILInstruction* {
                        auto* call = make<Call>(call_method);
                        call->arguments().add(left_builder());
                        call->arguments().add(right_builder());
                        return call;
                    },
                    is_lifted_to_null != 0 ? typesystem::nullable_type::create(method->compilation(), method->return_type()) : method->return_type()};
        }
        default: return fail();
    }
}

TransformExpressionTrees::Converted TransformExpressionTrees::convert_member_init(CallInstruction* invocation) {
    if (invocation->arguments().count() != 2) return fail();
    Converter new_obj = convert_instruction(invocation->arguments()[0]).build;
    if (!new_obj) return fail();
    const IMethod* ctor = nullptr;
    if (!match_new(as_call_instruction(invocation->arguments()[0]), &ctor)) return fail();
    std::vector<ILInstruction*> arguments;
    if (!match_argument_list(invocation->arguments()[1], &arguments)) return fail();
    if (arguments.empty()) return fail();

    std::vector<BindConverter> converted_arguments(arguments.size());
    for (std::size_t i = 0; i < arguments.size(); i++) {
        BindConverter arg;
        auto* bind = dynamic_cast<CallInstruction*>(arguments[i]);
        if (bind != nullptr && bind->method->full_name() == "System.Linq.Expressions.Expression.Bind") {
            arg = convert_bind(bind).build;
            if (!arg) return fail();
        } else {
            return fail();
        }
        converted_arguments[i] = arg;
    }

    auto build_block = [this, new_obj, ctor, converted_arguments]() -> ILInstruction* {
        ILFunction* function = lambda_stack_.back();
        ILVariable* initializer = function->register_variable(VariableKind::InitializerTarget, ctor->declaring_type());

        auto* initializer_block = make<Block>(BlockKind::CollectionInitializer);
        initializer_block->set_final_instruction(make<LdLoc>(initializer));
        initializer_block->instructions.add(make<StLoc>(initializer, new_obj()));
        for (const auto& f : converted_arguments) initializer_block->instructions.add(f(initializer));

        return initializer_block;
    };

    return {build_block, ctor->declaring_type()};
}

TransformExpressionTrees::Converted TransformExpressionTrees::convert_new_array_bounds(CallInstruction* invocation) {
    if (invocation->arguments().count() != 2) return fail();
    const IType* type = nullptr;
    if (!match_get_type_from_handle(invocation->arguments()[0], &type)) return fail();
    std::vector<ILInstruction*> arguments;
    if (!match_argument_list(invocation->arguments()[1], &arguments)) return fail();
    if (arguments.empty()) return fail();
    std::vector<Converter> indices(arguments.size());
    for (std::size_t i = 0; i < arguments.size(); i++) {
        Converter index = convert_instruction(arguments[i]).build;
        if (!index) return fail();
        indices[i] = index;
    }
    return {[type, indices]() -> ILInstruction* {
                std::vector<ILInstruction*> built;
                for (const auto& f : indices) built.push_back(f());
                return make<NewArr>(type, built);
            },
            typesystem::make_array_type(context_->type_system(), type, static_cast<int>(arguments.size()))};
}

TransformExpressionTrees::Converted TransformExpressionTrees::convert_new_array_init(CallInstruction* invocation) {
    if (invocation->arguments().count() != 2) return fail();
    const IType* type = nullptr;
    if (!match_get_type_from_handle(invocation->arguments()[0], &type)) return fail();
    std::vector<ILInstruction*> arguments;
    if (!match_argument_list(invocation->arguments()[1], &arguments)) return fail();
    const IType* array_type = typesystem::make_array_type(context_->type_system(), type);
    if (arguments.empty()) return {[type]() -> ILInstruction* { return make<NewArr>(type, std::vector<ILInstruction*>{make<LdcI4>(0)}); }, array_type};
    std::vector<Converter> converted_arguments(arguments.size());
    for (std::size_t i = 0; i < arguments.size(); i++) {
        ILInstruction* item = arguments[i];
        Converter value = convert_instruction(item).build;
        if (!value) return fail();
        converted_arguments[i] = value;
    }

    auto build_initializer = [this, type, array_type, converted_arguments]() -> ILInstruction* {
        ILFunction* function = lambda_stack_.back();
        ILVariable* variable = function->register_variable(VariableKind::InitializerTarget, array_type);
        auto* initializer = make<Block>(BlockKind::ArrayInitializer);
        initializer->instructions.add(
            make<StLoc>(variable, make<NewArr>(type, std::vector<ILInstruction*>{make<LdcI4>(static_cast<int>(converted_arguments.size()))})));
        for (std::size_t i = 0; i < converted_arguments.size(); i++) {
            auto* ld_elema = make<LdElema>(type, make<LdLoc>(variable), std::vector<ILInstruction*>{make<LdcI4>(static_cast<int>(i))});
            ld_elema->delay_exceptions = true;
            ILInstruction* value = converted_arguments[i]();
            initializer->instructions.add(make<StObj>(ld_elema, value, type));
        }
        initializer->set_final_instruction(make<LdLoc>(variable));
        return initializer;
    };

    return {build_initializer, array_type};
}

bool TransformExpressionTrees::match_new(CallInstruction* invocation, const IMethod** ctor) {
    *ctor = nullptr;
    if (invocation->method->name() != "New") return false;
    const IType* type = nullptr;
    const IMember* member = nullptr;
    switch (invocation->arguments().count()) {
        case 1: {
            if (match_get_type_from_handle(invocation->arguments()[0], &type)) {
                auto constructors = type->get_constructors([](const IMethod* c) { return c->parameters().empty(); });
                *ctor = constructors.empty() ? nullptr : constructors[0];
                return *ctor != nullptr;
            }
            if (match_get_constructor_from_handle(invocation->arguments()[0], &member)) {
                *ctor = as_method(member);
                return true;
            }
            return false;
        }
        case 2:
        case 3:
            if (!match_get_constructor_from_handle(invocation->arguments()[0], &member)) return false;
            *ctor = as_method(member);
            return true;
        default: return false;
    }
}

TransformExpressionTrees::Converted TransformExpressionTrees::convert_new_object(CallInstruction* invocation) {
    const IType* type = nullptr;
    const IMember* member = nullptr;
    std::vector<ILInstruction*> arguments;
    auto build_new_obj = [](const IMethod* method, const std::vector<Converter>& args) -> ILInstruction* {
        auto* new_obj = make<NewObj>(method);
        for (const auto& f : args) new_obj->arguments().add(f());
        return new_obj;
    };
    switch (invocation->arguments().count()) {
        case 1: {
            if (match_get_type_from_handle(invocation->arguments()[0], &type)) {
                auto constructors = type->get_constructors([](const IMethod* c) { return c->parameters().empty(); });
                const IMethod* ctor = constructors.empty() ? nullptr : constructors[0];
                if (ctor == nullptr) return fail();
                return {[ctor]() -> ILInstruction* { return make<NewObj>(ctor); }, type};
            }
            if (match_get_constructor_from_handle(invocation->arguments()[0], &member)) {
                const IMethod* ctor = as_method(member);
                return {[ctor]() -> ILInstruction* { return make<NewObj>(ctor); }, member->declaring_type()};
            }
            return fail();
        }
        case 2:
        case 3: {
            if (!match_get_constructor_from_handle(invocation->arguments()[0], &member)) return fail();
            if (!match_argument_list(invocation->arguments()[1], &arguments)) return fail();
            const IMethod* method = as_method(member);
            auto converted_arguments = convert_call_arguments(arguments, method);
            if (!converted_arguments) return fail();
            std::vector<Converter> args = *converted_arguments;
            return {[build_new_obj, method, args]() -> ILInstruction* { return build_new_obj(method, args); }, member->declaring_type()};
        }
        default: return fail();
    }
}

TransformExpressionTrees::Converted TransformExpressionTrees::convert_not_operator(CallInstruction* invocation) {
    if (invocation->arguments().count() < 1) return fail();
    Converted argument = convert_instruction(invocation->arguments()[0]);
    if (!argument.build) return fail();
    const IType* underlying_type = typesystem::nullable_type::get_underlying_type(argument.type);
    Converter arg_builder = argument.build;
    switch (invocation->arguments().count()) {
        case 1: {
            bool is_lifted = typesystem::nullable_type::is_nullable(argument.type);
            return {[underlying_type, arg_builder, is_lifted]() -> ILInstruction* {
                        if (typesystem::is_known_type(underlying_type, KnownTypeCode::Boolean)) return Comp::logic_not(arg_builder(), is_lifted);
                        return make<BitNot>(arg_builder(), is_lifted, typesystem::get_stack_type(*underlying_type));
                    },
                    argument.type};
        }
        case 2: {
            const IMember* method = nullptr;
            if (!match_get_method_from_handle(invocation->arguments()[1], &method)) return fail();
            const IMethod* call_method = as_method(method);
            return {[arg_builder, call_method]() -> ILInstruction* {
                        auto* call = make<Call>(call_method);
                        call->arguments().add(arg_builder());
                        return call;
                    },
                    method->return_type()};
        }
        default: return fail();
    }
}

TransformExpressionTrees::Converted TransformExpressionTrees::convert_property(CallInstruction* invocation) {
    if (invocation->arguments().count() < 2) return fail();
    Converter target_converter;
    const IType* target_type = nullptr;
    if (!invocation->arguments()[0]->match_ld_null()) {
        Converted target = convert_instruction(invocation->arguments()[0]);
        target_converter = target.build;
        target_type = target.type;
        if (!target_converter) return fail();
    }
    const IMember* member = nullptr;
    if (!match_get_method_from_handle(invocation->arguments()[1], &member)) return fail();
    std::vector<ILInstruction*> arguments;
    if (invocation->arguments().count() != 3 || !match_argument_list(invocation->arguments()[2], &arguments)) {
        arguments.clear();
    }
    auto converted_arguments = convert_call_arguments(arguments, as_method(member));
    if (!converted_arguments) return fail();
    std::vector<Converter> args = *converted_arguments;
    auto build_property = [this, member, target_converter, target_type, args]() -> ILInstruction* {
        CallInstruction* call;
        if (member->is_static()) {
            call = make<Call>(as_method(member));
        } else {
            call = make<CallVirt>(as_method(member));
        }
        if (target_converter) {
            call->arguments().add(prepare_call_target(member->declaring_type(), target_converter(), target_type));
        }
        for (const auto& f : args) call->arguments().add(f());
        return call;
    };
    return {build_property, member->return_type()};
}

TransformExpressionTrees::Converted TransformExpressionTrees::convert_type_as(CallInstruction* invocation) {
    if (invocation->arguments().count() != 2) return fail();
    Converter converted = convert_instruction(invocation->arguments()[0]).build;
    const IType* type = nullptr;
    if (!match_get_type_from_handle(invocation->arguments()[1], &type)) return fail();
    if (!converted) return fail();
    auto build_type_as = [converted, type]() -> ILInstruction* {
        ILInstruction* inst = make<IsInst>(converted(), type);
        // We must follow ECMA-335, III.4.6: If typeTok is a nullable type, Nullable<T>, it is interpreted as "boxed" T.
        if (typesystem::is_known_type(type, KnownTypeCode::NullableOfT)) inst = make<UnboxAny>(inst, type);
        return inst;
    };
    return {build_type_as, type};
}

TransformExpressionTrees::Converted TransformExpressionTrees::convert_type_is(CallInstruction* invocation) {
    if (invocation->arguments().count() != 2) return fail();
    Converter converted = convert_instruction(invocation->arguments()[0]).build;
    const IType* type = nullptr;
    if (!match_get_type_from_handle(invocation->arguments()[1], &type)) return fail();
    const IType* result_type = context_->type_system().find_type(KnownTypeCode::Boolean);
    if (converted)
        return {[converted, type]() -> ILInstruction* {
                    auto* is_inst = make<IsInst>(converted(), type);
                    return make<Comp>(ComparisonKind::Inequality, Sign::None, is_inst, make<LdNull>());
                },
                result_type};
    return fail();
}

TransformExpressionTrees::Converted TransformExpressionTrees::convert_unary_numeric_operator(CallInstruction* invocation, BinaryNumericOperator op,
                                                                                              std::optional<bool> is_checked) {
    if (invocation->arguments().count() < 1) return fail();
    Converted argument = convert_instruction(invocation->arguments()[0]);
    if (!argument.build) return fail();
    Converter arg_builder = argument.build;
    const IType* argument_type = argument.type;
    switch (invocation->arguments().count()) {
        case 1: {
            ILInstruction* left;
            const IType* underlying_type = typesystem::nullable_type::get_underlying_type(argument_type);

            switch (typesystem::get_stack_type(*underlying_type)) {
                case StackType::I4: left = make<LdcI4>(0); break;
                case StackType::I8: left = make<LdcI8>(0); break;
                case StackType::I: left = make<Conv>(make<LdcI4>(0), PrimitiveType::I, false, Sign::None); break;
                case StackType::F4: left = make<LdcF4>(0.0f); break;
                case StackType::F8: left = make<LdcF8>(0.0); break;
                case StackType::O:
                    if (typesystem::is_known_type(underlying_type, KnownTypeCode::Decimal)) {
                        left = make<LdcDecimal>(util::Decimal(0, 0, 0, false, 0));
                        break;
                    }
                    return fail();
                default: return fail();
            }
            bool checked_ = is_checked == true;
            return {[op, left, arg_builder, underlying_type, checked_, argument_type]() -> ILInstruction* {
                        ILInstruction* right = arg_builder();
                        return make<BinaryNumericInstruction>(op, left, right, typesystem::get_stack_type(*underlying_type),
                                                              typesystem::get_stack_type(*underlying_type), checked_,
                                                              typesystem::get_sign(*argument_type),
                                                              typesystem::nullable_type::is_nullable(argument_type));
                    },
                    argument_type};
        }
        case 2: {
            const IMember* method = nullptr;
            if (!match_get_method_from_handle(invocation->arguments()[1], &method)) return fail();
            const IMethod* call_method = as_method(method);
            return {[arg_builder, call_method]() -> ILInstruction* {
                        auto* call = make<Call>(call_method);
                        call->arguments().add(arg_builder());
                        return call;
                    },
                    method->return_type()};
        }
        default: break;
    }
    return fail();
}

ILInstruction* TransformExpressionTrees::convert_value(ILInstruction* value, ILInstruction* context) {
    if (auto* ldloc = as<LdLoc>(value)) {
        if (is_expression_tree_parameter(ldloc->variable())) {
            ILVariable* v = find_parameter_mapping(ldloc->variable());
            if (v == nullptr) return ldloc->clone();
            auto* parent_call = dynamic_cast<CallInstruction*>(context);
            if (parent_call != nullptr && parent_call->method->full_name() == "System.Linq.Expressions.Expression.Call" &&
                typesystem::is_integer_type(v->stack_type))
                return with_il_range(make<LdLoca>(v), ldloc);
            return nullptr;
        } else if (is_closure_reference(ldloc->variable())) {
            if (ldloc->variable()->kind() == VariableKind::Local) {
                ldloc->variable()->set_kind(VariableKind::DisplayClassLocal);
            }
            if (ldloc->variable()->capture_scope == nullptr) {
                ldloc->variable()->capture_scope = BlockContainer::find_closest_container(context);
                ILFunction* f = nullptr;
                for (ILInstruction* ancestor : ldloc->variable()->capture_scope->ancestors()) {
                    if ((f = as<ILFunction>(ancestor)) != nullptr) break;
                }
                if (f != nullptr) {
                    f->captured_variables.insert(ldloc->variable());
                }
            }
            return ldloc;
        } else {
            return ldloc;
        }
    }
    return value->clone();
}

bool TransformExpressionTrees::is_closure_reference(ILVariable* variable) {
    if (!variable->is_single_definition() || variable->store_instructions().size() != 1) return false;
    auto* store = as<StLoc>(variable->store_instructions()[0]->as_instruction());
    if (store == nullptr) return false;
    auto* new_obj = as<NewObj>(store->value());
    if (new_obj == nullptr) return false;
    return TransformDisplayClassUsage::is_potential_closure(*context_, new_obj);
}

bool TransformExpressionTrees::is_expression_tree_parameter(ILVariable* variable) {
    return variable->type()->full_name() == "System.Linq.Expressions.ParameterExpression";
}

bool TransformExpressionTrees::match_constant_call(ILInstruction* inst, ILInstruction** value, const IType** type) {
    *value = nullptr;
    *type = nullptr;
    auto* call = dynamic_cast<CallInstruction*>(inst);
    if (call != nullptr && call->method->full_name() == "System.Linq.Expressions.Expression.Constant") {
        *value = call->arguments()[0];
        if (call->arguments().count() == 2) return match_get_type_from_handle(call->arguments()[1], type);
        switch ((*value)->op_code) {
            case OpCode::LdNull: *type = &SpecialType::null_type(); break;
            case OpCode::LdStr: *type = context_->type_system().find_type(KnownTypeCode::String); break;
            case OpCode::LdcF4: *type = context_->type_system().find_type(KnownTypeCode::Single); break;
            case OpCode::LdcF8: *type = context_->type_system().find_type(KnownTypeCode::Double); break;
            case OpCode::LdcI4: *type = context_->type_system().find_type(KnownTypeCode::Int32); break;
            case OpCode::LdcI8: *type = context_->type_system().find_type(KnownTypeCode::Int64); break;
            default: *type = infer_type(*value, &context_->type_system()); break;
        }
        return true;
    }
    return false;
}

bool TransformExpressionTrees::match_get_type_from_handle(ILInstruction* inst, const IType** type) {
    *type = nullptr;
    auto* get_type_call = dynamic_cast<CallInstruction*>(inst);
    return get_type_call != nullptr && get_type_call->method->full_name() == "System.Type.GetTypeFromHandle" && get_type_call->arguments().count() == 1 &&
           get_type_call->arguments()[0]->match_ld_type_token(type);
}

bool TransformExpressionTrees::match_get_method_from_handle(ILInstruction* inst, const IMember** member) {
    *member = nullptr;
    // castclass System.Reflection.MethodInfo(call GetMethodFromHandle(ldmembertoken op_Addition))
    ILInstruction* arg = nullptr;
    const IType* type = nullptr;
    if (!inst->match_cast_class(&arg, &type)) return false;
    if (type->full_name() != "System.Reflection.MethodInfo") return false;
    auto* call = dynamic_cast<CallInstruction*>(arg);
    if (!(call != nullptr && call->method->full_name() == "System.Reflection.MethodBase.GetMethodFromHandle")) return false;
    return match_from_handle_parameter_list(call, member);
}

bool TransformExpressionTrees::match_get_constructor_from_handle(ILInstruction* inst, const IMember** member) {
    *member = nullptr;
    // castclass System.Reflection.ConstructorInfo(call GetMethodFromHandle(ldmembertoken op_Addition))
    ILInstruction* arg = nullptr;
    const IType* type = nullptr;
    if (!inst->match_cast_class(&arg, &type)) return false;
    if (type->full_name() != "System.Reflection.ConstructorInfo") return false;
    auto* call = dynamic_cast<CallInstruction*>(arg);
    if (!(call != nullptr && call->method->full_name() == "System.Reflection.MethodBase.GetMethodFromHandle")) return false;
    return match_from_handle_parameter_list(call, member);
}

bool TransformExpressionTrees::match_get_field_from_handle(ILInstruction* inst, const IMember** member) {
    *member = nullptr;
    auto* call = dynamic_cast<CallInstruction*>(inst);
    if (!(call != nullptr && call->method->full_name() == "System.Reflection.FieldInfo.GetFieldFromHandle")) return false;
    return match_from_handle_parameter_list(call, member);
}

bool TransformExpressionTrees::match_from_handle_parameter_list(CallInstruction* call, const IMember** member) {
    *member = nullptr;
    switch (call->arguments().count()) {
        case 1:
            if (!call->arguments()[0]->match_ld_member_token(member)) return false;
            break;
        case 2: {
            if (!call->arguments()[0]->match_ld_member_token(member)) return false;
            const IType* unused = nullptr;
            if (!call->arguments()[1]->match_ld_type_token(&unused)) return false;
            break;
        }
        default: return false;
    }
    return true;
}

bool TransformExpressionTrees::match_argument_list(ILInstruction* inst, std::vector<ILInstruction*>* arguments) {
    arguments->clear();
    auto* block = as<Block>(inst);
    if (!(block != nullptr && block->kind == BlockKind::ArrayInitializer)) {
        if (is_empty_parameter_list(inst)) {
            return true;
        }
        return false;
    }
    int i = 0;
    for (ILInstruction* instruction : block->instructions) {
        auto* item = as<StObj>(instruction);
        if (item == nullptr) continue;
        auto* ldelem = as<LdElema>(item->target());
        if (ldelem != nullptr && ldelem->indices().count() != 1) throw UsageError("Sequence contains no elements or more than one");
        if (!(ldelem != nullptr && ldelem->indices()[0]->match_ldc_i4(i))) return false;
        arguments->push_back(item->value());
        i++;
    }
    return true;
}

}  // namespace cb::dotnet::decompiler::il::transforms
