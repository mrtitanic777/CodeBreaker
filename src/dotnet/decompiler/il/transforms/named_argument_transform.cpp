// dotnet/decompiler/il/transforms/named_argument_transform.cpp -- see named_argument_transform.hpp.
#include "dotnet/decompiler/il/transforms/named_argument_transform.hpp"

#include <cassert>

#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/il/transforms/il_extraction.hpp"
#include "dotnet/decompiler/nr_extensions.hpp"
#include "dotnet/decompiler/typesystem/decompiler_type_system.hpp"
#include "dotnet/decompiler/typesystem/metadata_entities.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::il::transforms {

using typesystem::TypeKind;

namespace {

}  // namespace

NamedArgumentTransform::FindResult NamedArgumentTransform::can_introduce_named_argument(CallInstruction* call, ILInstruction* child,
                                                                                         ILVariable* v,
                                                                                         ILInstruction* expression_being_moved) {
    assert(child->parent() == call);
    if (call->is_instance_call() && child->child_index() == 0)
        return FindResult::stop();  // cannot use named arg to move expressionBeingMoved before this pointer
    if (call->method->is_operator() || call->method->is_accessor())
        return FindResult::stop();  // cannot use named arg for operators or accessors
    if (dynamic_cast<const typesystem::VarArgInstanceMethod*>(call->method) != nullptr)
        return FindResult::stop();  // the call builder does not support named arguments with varargs
    if (call->method->is_constructor()) {
        const IType* type = call->method->declaring_type();
        if (type->kind() == TypeKind::Delegate || decompiler::is_anonymous_type(type)) return FindResult::stop();
    }
    for (const typesystem::IParameter* p : call->method->parameters()) {
        if (p->name().empty()) return FindResult::stop();  // cannot use named arguments
    }
    for (int i = child->child_index(); i < call->arguments().count(); i++) {
        FindResult r = ILInlining::find_load_in_next(call->arguments()[i], v, expression_being_moved, InliningOptions::None);
        if (r.type == FindResultType::Found) {
            return FindResult::named_argument(r.load_inst, call->arguments()[i]);
        }
    }
    return FindResult::stop();
}

NamedArgumentTransform::FindResult NamedArgumentTransform::can_extend_named_argument(Block* block, ILVariable* v,
                                                                                      ILInstruction* expression_being_moved) {
    assert(block->kind == BlockKind::CallWithNamedArgs);
    ILInstruction* first_arg = as<StLoc>(block->instructions[0])->value();
    FindResult r = ILInlining::find_load_in_next(first_arg, v, expression_being_moved, InliningOptions::IntroduceNamedArguments);
    if (r.type == FindResultType::Found || r.type == FindResultType::NamedArgument) {
        return r;  // OK, inline into first instruction of block
    }
    auto* call = as<CallInstruction>(block->final_instruction());
    if (call->is_instance_call()) {
        // For instance calls, block.Instructions[0] is the argument for the 'this' pointer. We can only insert at position 1.
        if (r.type == FindResultType::Stop) {
            // error: can't move expressionBeingMoved after block.Instructions[0]
            return FindResult::stop();
        }
        // Because we always ensure block.Instructions[0] is the 'this' argument, it's possible that the place we actually need to
        // inline into is within block.Instructions[1]:
        if (block->instructions.count() > 1) {
            r = ILInlining::find_load_in_next(block->instructions[1], v, expression_being_moved, InliningOptions::IntroduceNamedArguments);
            if (r.type == FindResultType::Found || r.type == FindResultType::NamedArgument) {
                return r;  // OK, inline into block.Instructions[1]
            }
        }
    }
    for (ILInstruction* arg : call->arguments()) {
        if (arg->match_ld_loc(v)) {
            return FindResult::named_argument(arg, arg);
        }
    }
    return FindResult::stop();
}

void NamedArgumentTransform::introduce_named_argument(ILInstruction* arg, ILTransformContext& context) {
    auto* call = as<CallInstruction>(arg->parent());
#ifndef NDEBUG
    {
        ILFunction* first_function = nullptr;
        for (ILInstruction* ancestor : call->ancestors()) {
            if (auto* f = as<ILFunction>(ancestor)) {
                first_function = f;
                break;
            }
        }
        assert(context.function() == first_function);
    }
#endif
    const IType* type = find_type_of_stack_type(context.type_system(), arg->result_type());
    ILVariable* v = context.function()->register_variable(VariableKind::NamedArgument, type);
    context.step("Introduce named argument '" + v->name.value_or("") + "'", arg);
    Block* named_arg_block = as<Block>(call->parent());
    if (named_arg_block == nullptr || named_arg_block->kind != BlockKind::CallWithNamedArgs) {
        // create namedArgBlock:
        named_arg_block = make<Block>(BlockKind::CallWithNamedArgs);
        call->replace_with(named_arg_block);
        named_arg_block->set_final_instruction(call);
        if (call->is_instance_call()) {
            const IType* this_var_type = call->constrained_to != nullptr ? call->constrained_to : call->method->declaring_type();
            if (CallInstruction::expected_type_for_this_pointer(*call->method->declaring_type(), call->constrained_to) == StackType::Ref) {
                this_var_type = typesystem::make_by_reference_type(this_var_type);
            }
            ILVariable* this_arg_var = context.function()->register_variable(VariableKind::NamedArgument, this_var_type, "this_arg");
            named_arg_block->instructions.add(make<StLoc>(this_arg_var, call->arguments()[0]));
            call->arguments().set(0, make<LdLoc>(this_arg_var));
        }
    }
    int arg_index = arg->child_index();
    assert(call->arguments()[arg_index] == arg);
    auto* new_inst = make<StLoc>(v, arg);
    named_arg_block->instructions.insert(call->is_instance_call() ? 1 : 0, new_inst);
    call->arguments().set(arg_index, make<LdLoc>(v));
    context.end_step(new_inst);
}

void NamedArgumentTransform::run(Block* block, int pos, StatementTransformContext& context) {
    if (!context.settings().named_arguments) return;
    InliningOptions options = ILInlining::options_for_block(block, pos, context);
    options |= InliningOptions::IntroduceNamedArguments;
    ILInlining::inline_one_if_possible(block, pos, options, context);
}

}  // namespace cb::dotnet::decompiler::il::transforms
