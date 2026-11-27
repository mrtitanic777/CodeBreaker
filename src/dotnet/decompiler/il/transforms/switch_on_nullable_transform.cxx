// dotnet/decompiler/il/transforms/switch_on_nullable_transform.cxx -- see switch_on_nullable_transform.hpp.
#include "dotnet/decompiler/il/transforms/switch_on_nullable_transform.hpp"

#include <unordered_set>
#include <vector>

#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/il/control_flow/switch_detection.hpp"
#include "dotnet/decompiler/il/il_instruction_extensions.hpp"
#include "dotnet/decompiler/il/semantic_helper.hpp"
#include "dotnet/decompiler/il/transforms/nullable_lifting_transform.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"

namespace cb::dotnet::decompiler::il::transforms {

void SwitchOnNullableTransform::run(ILFunction* function, ILTransformContext& context) {
    if (!context.settings().lift_nullables) return;

    // the containers in the order they were changed (upstream's HashSet is only walked for the sort)
    std::vector<BlockContainer*> changed_containers;
    std::unordered_set<BlockContainer*> changed_set;

    for (ILInstruction* descendant : function->descendants()) {
        auto* block = as<Block>(descendant);
        if (block == nullptr) continue;
        bool changed = false;
        for (int i = block->instructions.count() - 1; i >= 0; i--) {
            SwitchInstruction* new_switch = nullptr;
            if (match_switch_on_nullable(block->instructions, i, &new_switch)) {
                new_switch->add_il_range(*block->instructions[i - 2]);
                block->instructions[i + 1]->replace_with(new_switch);
                block->instructions.remove_range(i - 2, 3);
                i -= 2;
                changed = true;
                continue;
            }
            if (match_roslyn_switch_on_nullable(block->instructions, i, &new_switch)) {
                new_switch->add_il_range(*block->instructions[i]);
                new_switch->add_il_range(*block->instructions[i + 1]);
                block->instructions[i]->replace_with(new_switch);
                block->instructions.remove_at(i + 1);
                changed = true;
                continue;
            }
        }
        if (!changed) continue;
        control_flow::SwitchDetection::simplify_switch_instruction(block, context);
        if (auto* container = as<BlockContainer>(block->parent())) {
            if (changed_set.insert(container).second) changed_containers.push_back(container);
        }
    }

    for (BlockContainer* container : changed_containers) container->sort_blocks(true);
}

bool SwitchOnNullableTransform::match_switch_on_nullable(InstructionCollection<ILInstruction>& instructions, int i,
                                                         SwitchInstruction** new_switch) {
    *new_switch = nullptr;
    // match first block:
    // stloc tmp(ldloca switchValueVar)
    // stloc switchVariable(call GetValueOrDefault(ldloc tmp))
    // if (logic.not(call get_HasValue(ldloc tmp))) br nullCaseBlock
    // br switchBlock
    if (i < 2) return false;
    ILVariable* tmp = nullptr;
    ILInstruction* ldloca = nullptr;
    ILVariable* switch_variable = nullptr;
    ILInstruction* get_value_or_default = nullptr;
    ILInstruction* condition = nullptr;
    ILInstruction* true_inst = nullptr;
    if (!instructions[i - 2]->match_st_loc(&tmp, &ldloca) || !instructions[i - 1]->match_st_loc(&switch_variable, &get_value_or_default) ||
        !instructions[i]->match_if_instruction(&condition, &true_inst))
        return false;
    if (!tmp->is_single_definition() || tmp->load_count() != 2) return false;
    if (!switch_variable->is_single_definition() || switch_variable->load_count() != 1) return false;
    Block* switch_block = nullptr;
    Block* null_case_block = nullptr;
    if (!instructions[i + 1]->match_branch(&switch_block) || !true_inst->match_branch(&null_case_block)) return false;
    ILVariable* switch_value_var = nullptr;
    if (!ldloca->match_ld_loca(&switch_value_var)) return false;
    ILInstruction* get_has_value = nullptr;
    if (!condition->match_logic_not(&get_has_value)) return false;
    ILInstruction* get_value_or_default_arg = nullptr;
    if (!NullableLiftingTransform::match_get_value_or_default(get_value_or_default, &get_value_or_default_arg)) return false;
    ILInstruction* get_has_value_arg = nullptr;
    if (!NullableLiftingTransform::match_has_value_call(get_has_value, &get_has_value_arg)) return false;
    if (!(get_has_value_arg->match_ld_loc(tmp) && get_value_or_default_arg->match_ld_loc(tmp))) return false;
    // match second block: switchBlock
    // switch (ldloc switchVariable) {
    // 	case [0..1): br caseBlock1
    //  ... more cases ...
    // 	case [long.MinValue..0),[1..5),[6..10),[11..long.MaxValue]: br defaultBlock
    // }
    if (switch_block->instructions.count() != 1 || switch_block->incoming_edge_count() != 1) return false;
    auto* switch_inst = as<SwitchInstruction>(switch_block->instructions[0]);
    if (switch_inst == nullptr) return false;
    const IType* nullable_type = as<Call>(get_has_value)->method->declaring_type();
    *new_switch = build_lifted_switch(null_case_block, switch_inst, make<LdLoc>(switch_value_var), nullable_type);
    return true;
}

SwitchInstruction* SwitchOnNullableTransform::build_lifted_switch(Block* null_case_block, SwitchInstruction* switch_inst,
                                                                  ILInstruction* switch_value, const IType* nullable_type) {
    auto* new_switch = make<SwitchInstruction>(switch_value);
    new_switch->is_lifted = true;
    new_switch->type = nullable_type;
    new_switch->sections.add_range(switch_inst->sections.items());
    auto* section = make<SwitchSection>();
    section->set_body(make<Branch>(null_case_block));
    section->set_has_null_label(true);
    new_switch->sections.add(section);
    return new_switch;
}

bool SwitchOnNullableTransform::match_roslyn_switch_on_nullable(InstructionCollection<ILInstruction>& instructions, int i,
                                                                SwitchInstruction** new_switch) {
    *new_switch = nullptr;
    // match first block:
    // if (logic.not(call get_HasValue(target))) br nullCaseBlock
    // br switchBlock
    ILInstruction* condition = nullptr;
    ILInstruction* true_inst = nullptr;
    if (!instructions[i]->match_if_instruction(&condition, &true_inst)) return false;
    Block* switch_block = nullptr;
    Block* null_case_block = nullptr;
    if (!instructions[i + 1]->match_branch(&switch_block) || !true_inst->match_branch(&null_case_block)) return false;
    ILInstruction* get_has_value = nullptr;
    ILInstruction* target = nullptr;
    if (!condition->match_logic_not(&get_has_value) || !NullableLiftingTransform::match_has_value_call(get_has_value, &target) ||
        !SemanticHelper::is_pure(target->flags()))
        return false;
    // match second block: switchBlock
    // note: I have seen cases where switchVar is inlined into the switch.
    // stloc switchVar(call GetValueOrDefault(ldloca tmp))
    // switch (ldloc switchVar) {
    // 	case [0..1): br caseBlock1
    // ... more cases ...
    // 	case [long.MinValue..0),[1..5),[6..10),[11..long.MaxValue]: br defaultBlock
    // }
    if (switch_block->incoming_edge_count() != 1) return false;
    SwitchInstruction* switch_inst = nullptr;
    switch (switch_block->instructions.count()) {
    case 2: {
        // this is the normal case described by the pattern above
        ILVariable* switch_var = nullptr;
        ILInstruction* get_value_or_default = nullptr;
        if (!switch_block->instructions[0]->match_st_loc(&switch_var, &get_value_or_default)) return false;
        if (!switch_var->is_single_definition() || switch_var->load_count() != 1) return false;
        ILInstruction* target2 = nullptr;
        if (!(NullableLiftingTransform::match_get_value_or_default(get_value_or_default, &target2) && target2->match(target).success()))
            return false;
        auto* si = as<SwitchInstruction>(switch_block->instructions[1]);
        if (si == nullptr) return false;
        switch_inst = si;
        break;
    }
    case 1: {
        // this is the special case where `call GetValueOrDefault(ldloca tmp)` is inlined into the switch.
        auto* si = as<SwitchInstruction>(switch_block->instructions[0]);
        if (si == nullptr) return false;
        ILInstruction* target2 = nullptr;
        if (!(NullableLiftingTransform::match_get_value_or_default(si->value(), &target2) && target2->match(target).success())) return false;
        switch_inst = si;
        break;
    }
    default:
        return false;
    }
    ILInstruction* switch_value = nullptr;
    ILVariable* v = nullptr;
    if (target->match_ld_loca(&v))
        switch_value = with_il_range(make<LdLoc>(v), target);
    else
        switch_value = make<LdObj>(target, as<CallInstruction>(get_has_value)->method->declaring_type());
    const IType* nullable_type = as<Call>(get_has_value)->method->declaring_type();
    *new_switch = build_lifted_switch(null_case_block, switch_inst, switch_value, nullable_type);
    return true;
}

}  // namespace cb::dotnet::decompiler::il::transforms
