// dotnet/decompiler/il/transforms/combine_exits_transform.cxx -- see combine_exits_transform.hpp.
#include "dotnet/decompiler/il/transforms/combine_exits_transform.hpp"

#include "dotnet/decompiler/il/il_instruction_extensions.hpp"
#if __has_include("dotnet/decompiler/il/transforms/expression_transforms.hpp")
#include "dotnet/decompiler/il/transforms/expression_transforms.hpp"
#define CB_HAVE_EXPRESSION_TRANSFORMS 1
#endif

namespace cb::dotnet::decompiler::il::transforms {

void CombineExitsTransform::run(ILFunction* function, ILTransformContext& context) {
    auto* container = as<BlockContainer>(function->body());
    if (!(container != nullptr && container->blocks.count() == 1)) return;
    Leave* combined_exit = combine_exits(container->entry_point());
    if (combined_exit == nullptr) return;
#ifdef CB_HAVE_EXPRESSION_TRANSFORMS
    ExpressionTransforms::run_on_single_statement(combined_exit, context);
#else
    (void)context;
    // WAITS: ExpressionTransforms (run_on_single_statement)
#endif
}

Leave* CombineExitsTransform::combine_exits(Block* block) {
    auto* if_inst = as<IfInstruction>(block->instructions.second_to_last_or_default());
    auto* leave_else = as<Leave>(block->instructions.last_or_default());
    if (!(if_inst != nullptr && leave_else != nullptr)) return nullptr;
    if (!if_inst->false_inst()->match_nop()) return nullptr;
    // try to unwrap true branch to single instruction:
    ILInstruction* true_instruction = Block::unwrap(if_inst->true_inst());
    // if the true branch is a block with multiple instructions:
    // try to apply the combine exits transform to the nested block
    // and then continue on that transformed block.
    // Example:
    // if (cond) {
    //   if (cond2) {
    //     leave (value)
    //   }
    //   leave (value2)
    // }
    // leave (value3)
    // =>
    // leave (if (cond) value else if (cond2) value2 else value3)
    if (auto* nested_block = as<Block>(true_instruction); nested_block != nullptr && nested_block->instructions.count() == 2)
        true_instruction = combine_exits(nested_block);
    auto* leave = as<Leave>(true_instruction);
    if (leave == nullptr) return nullptr;
    if (!(leave->is_leaving_function() && leave_else->is_leaving_function())) return nullptr;
    if (leave->value()->match_nop() || leave_else->value()->match_nop()) return nullptr;
    // if (cond) {
    //   leave (value)
    // }
    // leave (elseValue)
    // =>
    // leave (if (cond) value else elseValue)
    auto* value = make<IfInstruction>(if_inst->condition(), leave->value(), leave_else->value());
    value->add_il_range(*if_inst);
    auto* combined_leave = make<Leave>(leave->target_container(), value);
    combined_leave->add_il_range(*leave_else);
    combined_leave->add_il_range(*leave);
    if_inst->replace_with(combined_leave);
    block->instructions.remove_at(combined_leave->child_index() + 1);
    return combined_leave;
}

}  // namespace cb::dotnet::decompiler::il::transforms
