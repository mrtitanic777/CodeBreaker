// dotnet/decompiler/il/transforms/fix_remaining_increments.cxx -- see fix_remaining_increments.hpp.
#include "dotnet/decompiler/il/transforms/fix_remaining_increments.hpp"

#include <cassert>
#include <format>
#include <vector>

#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/il/il_instruction_extensions.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"

namespace cb::dotnet::decompiler::il::transforms {

using typesystem::KnownTypeCode;

void FixRemainingIncrements::run(ILFunction* function, ILTransformContext& context) {
    std::vector<Call*> calls_to_fix;
    for (ILInstruction* descendant : function->descendants()) {
        auto* call = as<Call>(descendant);
        if (call == nullptr) continue;
        if (!UserDefinedCompoundAssign::is_increment_or_decrement(call->method, &context.settings())) continue;
        if (call->arguments().count() != 1) continue;
        if (typesystem::is_known_type(call->method->declaring_type(), KnownTypeCode::Decimal)) {
            // For decimal, legacy csc can optimize "d + 1m" to "op_Increment(d)".
            // We can handle these calls in ReplaceMethodCallsWithOperators.
            continue;
        }
        calls_to_fix.push_back(call);
    }
    for (Call* call : calls_to_fix) {
        // A user-defined increment/decrement that was not handled by TransformAssignment.
        // This can happen because the variable-being-incremented was optimized out by Roslyn,
        // e.g.
        //   public void Issue1552Pre(UserType a, UserType b)
        //   {
        //      UserType num = a + b;
        //      Console.WriteLine(++num);
        //   }
        // can end up being compiled to:
        //      Console.WriteLine(UserType.op_Increment(a + b));
        if (call->slot_info() == &StLoc::value_slot && call->parent()->slot_info() == &Block::instruction_slot) {
            auto* store = static_cast<StLoc*>(call->parent());
            auto* block = static_cast<Block*>(store->parent());
            context.step(std::format("Fix {} call at 0x{:04x} using {}", call->method->name(), call->start_il_offset(), store->variable()->name.value_or("")), call);
            // stloc V(call op_Increment(...))
            // ->
            // stloc V(...)
            // compound.assign op_Increment(V)
            call->replace_with(call->arguments()[0]);
            auto* compound_assign = with_il_range(make<UserDefinedCompoundAssign>(call->method, CompoundEvalMode::EvaluatesToNewValue,
                                                                                  make<LdLoca>(store->variable()), CompoundTargetKind::Address,
                                                                                  make<LdcI4>(1)),
                                                  call);
            block->instructions.insert(store->child_index() + 1, compound_assign);
            context.end_step(compound_assign);
        } else {
            context.step(std::format("Fix {} call at 0x{:04x} using new local", call->method->name(), call->start_il_offset()), call);
            ILVariable* new_variable = call->arguments()[0]->extract(context);
            if (new_variable == nullptr) {
                assert(false && "Failed to extract argument of remaining increment/decrement");
                continue;
            }
            new_variable->set_type(call->get_parameter(0)->type());
            assert(call->arguments()[0]->match_ld_loc(new_variable));
            auto* compound_assign = with_il_range(make<UserDefinedCompoundAssign>(call->method, CompoundEvalMode::EvaluatesToNewValue,
                                                                                  make<LdLoca>(new_variable), CompoundTargetKind::Address,
                                                                                  make<LdcI4>(1)),
                                                  call);
            call->replace_with(compound_assign);
            context.end_step(compound_assign);
        }
    }
}

}  // namespace cb::dotnet::decompiler::il::transforms
