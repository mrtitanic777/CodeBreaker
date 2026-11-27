// dotnet/decompiler/il/transforms/ld_loca_dup_init_obj_transform.cpp -- see ld_loca_dup_init_obj_transform.hpp.
#include "dotnet/decompiler/il/transforms/ld_loca_dup_init_obj_transform.hpp"

#include "dotnet/decompiler/il/il_instruction_extensions.hpp"
#include "dotnet/decompiler/typesystem/type_utils.hpp"

namespace cb::dotnet::decompiler::il::transforms {

void LdLocaDupInitObjTransform::run(ILFunction* function, ILTransformContext& context) {
    for (ILInstruction* inst : function->descendants()) {
        auto* block = as<Block>(inst);
        if (block == nullptr) continue;
        for (int i = 0; i < block->instructions.count(); i++) {
            try_transform(block, i, context);
        }
    }
}

bool LdLocaDupInitObjTransform::try_transform(Block* block, int i, ILTransformContext& context) {
    auto* inst1 = as<StLoc>(block->instructions[i]);
    if (inst1 == nullptr) return false;
    ILVariable* s = inst1->variable();
    auto* ld_loca = as<LdLoca>(inst1->value());
    if (ld_loca == nullptr) return false;
    ILVariable* v = ld_loca->variable();
    auto* inst2 = as<StObj>(block->instructions.element_at_or_default(i + 1));
    if (inst2 == nullptr) return false;
    if (!(inst2->target()->match_ld_loc(s) && typesystem::is_compatible_type_for_memory_access(*v->type(), *inst2->type()) &&
          inst2->unaligned_prefix() == 0 && !inst2->is_volatile() && is<DefaultValue>(inst2->value()))) {
        return false;
    }
    context.step("LdLocaDupInitObjTransform", inst1);
    StLoc* new_inst = with_il_range(make<StLoc>(v, inst2->value()), inst2);
    block->instructions.set(i, new_inst);
    block->instructions.set(i + 1, inst1);
    context.end_step(new_inst);
    return true;
}

}  // namespace cb::dotnet::decompiler::il::transforms
