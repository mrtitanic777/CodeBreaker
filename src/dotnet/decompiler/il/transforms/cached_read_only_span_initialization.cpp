// dotnet/decompiler/il/transforms/cached_read_only_span_initialization.cpp -- see cached_read_only_span_initialization.hpp.
#include "dotnet/decompiler/il/transforms/cached_read_only_span_initialization.hpp"

#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/nr_extensions.hpp"
#include "dotnet/decompiler/typesystem/known_attributes.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"

namespace cb::dotnet::decompiler::il::transforms {

void CachedReadOnlySpanInitialization::run(Block* block, BlockTransformContext& context) {
    if (!context.settings().array_initializers) return;

    // The store that loads the cache field precedes the if, at block.instructions[i - 1], so there is nothing to match when i == 0.
    for (int i = context.index_of_first_already_transformed_instruction - 1; i >= 1; i--) {
        if (auto* inst = as<IfInstruction>(block->instructions[i]); inst != nullptr && do_transform(block, i, inst, context)) {
            context.index_of_first_already_transformed_instruction = block->instructions.count();
        }
    }
}

bool CachedReadOnlySpanInitialization::do_transform(Block* block, int i, IfInstruction* inst, BlockTransformContext& context) {
    // storeBeforeIf: stloc V(ldobj(ldsflda cacheField)), cacheField a compiler-generated static field.
    auto* store_before_if = as<StLoc>(block->instructions[i - 1]);
    if (store_before_if == nullptr) return false;
    auto* ld_obj = as<LdObj>(store_before_if->value());
    if (ld_obj == nullptr) return false;
    auto* lds_flda = as<LdsFlda>(ld_obj->target());
    if (lds_flda == nullptr) return false;
    const IField* cache_field = lds_flda->field();

    if (!is_compiler_generated_or_is_in_compiler_generated_class(cache_field)) return false;

    // V is assigned exactly twice (before-if load + in-if init) and read exactly three times
    // (null-check condition + cache write-back + one real downstream usage), with no address-of.
    ILVariable* v = store_before_if->variable();
    if (v->store_count() != 2 || v->load_count() != 3 || v->address_count() != 0) return false;

    // The if must be a simple `if (...) { ... }` (no else) with a two-instruction body.
    auto* true_block = as<Block>(inst->true_inst());
    if (!inst->false_inst()->match_nop() || true_block == nullptr || true_block->instructions.count() != 2) return false;

    // condition: V == null (match_comp_equals_null also accepts null == V and the negated forms).
    ILInstruction* null_check_arg = nullptr;
    if (!inst->condition()->match_comp_equals_null(&null_check_arg) || !null_check_arg->match_ld_loc(v)) return false;

    // trueBlock[0]: stloc V(value)
    auto* store_value = as<StLoc>(true_block->instructions[0]);
    if (store_value == nullptr || store_value->variable() != v) return false;

    // trueBlock[1]: stobj(ldsflda cacheField, ldloc V) -> the write-back to the same cache field.
    auto* stobj = as<StObj>(true_block->instructions[1]);
    const IField* cache_field2 = nullptr;
    if (stobj == nullptr || !stobj->target()->match_lds_flda(&cache_field2) || !cache_field->equals(cache_field2) ||
        !stobj->value()->match_ld_loc(v)) {
        return false;
    }

    context.step("CachedReadOnlySpanInitialization", inst);
    store_before_if->set_value(store_value->value());
    block->instructions.remove_at(i);
    return true;
}

}  // namespace cb::dotnet::decompiler::il::transforms
