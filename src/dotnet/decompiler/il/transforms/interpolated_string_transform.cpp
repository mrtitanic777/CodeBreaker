// dotnet/decompiler/il/transforms/interpolated_string_transform.cpp -- see interpolated_string_transform.hpp.
#include "dotnet/decompiler/il/transforms/interpolated_string_transform.hpp"

#include <cassert>

#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/il/transforms/il_inlining.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"

namespace cb::dotnet::decompiler::il::transforms {

using typesystem::KnownTypeCode;

void InterpolatedStringTransform::run(Block* block, int pos, StatementTransformContext& context) {
    if (!context.settings().string_interpolation) return;
    int interpolation_start = pos;
    int interpolation_end;
    ILInstruction* insertion_point = nullptr;
    // stloc v(newobj DefaultInterpolatedStringHandler..ctor(ldc.i4 literalLength, ldc.i4 formattedCount))
    auto* stloc = as<StLoc>(block->instructions[pos]);
    ILVariable* v = nullptr;
    NewObj* new_obj = nullptr;
    std::int32_t ignored = 0;
    if (stloc != nullptr) {
        v = stloc->variable();
        new_obj = as<NewObj>(stloc->value());
    }
    if (stloc != nullptr && v->kind() == VariableKind::Local && new_obj != nullptr && new_obj->arguments().count() == 2 &&
        typesystem::is_known_type(v->type(), KnownTypeCode::DefaultInterpolatedStringHandler) &&
        typesystem::is_known_type(new_obj->method->declaring_type(), KnownTypeCode::DefaultInterpolatedStringHandler) &&
        new_obj->arguments()[0]->match_ldc_i4(&ignored) && new_obj->arguments()[1]->match_ldc_i4(&ignored)) {
        // { call MethodName(ldloca v, ...) }
        do {
            pos++;
        } while (is_known_call(block, pos, v));
        interpolation_end = pos;
        // ... call ToStringAndClear(ldloca v) ...
        if (!find_to_string_and_clear(block, pos, interpolation_start, interpolation_end, v, &insertion_point)) {
            return;
        }
        if (!(v->store_count() == 1 && v->address_count() == interpolation_end - interpolation_start && v->load_count() == 0)) {
            return;
        }
    } else {
        return;
    }
    context.step("Transform DefaultInterpolatedStringHandler " + v->name.value_or(""), stloc);
    v->set_kind(VariableKind::InitializerTarget);
    auto* replacement = make<Block>(BlockKind::InterpolatedString);
    for (int i = interpolation_start; i < interpolation_end; i++) {
        replacement->instructions.add(block->instructions[i]);
    }
    ILInstruction* call_to_string_and_clear = insertion_point;
    insertion_point->replace_with(replacement);
    replacement->set_final_instruction(call_to_string_and_clear);
    block->instructions.remove_range(interpolation_start, interpolation_end - interpolation_start);
    context.end_step(replacement);
}

bool InterpolatedStringTransform::is_known_call(Block* block, int pos, ILVariable* v) const {
    if (pos >= block->instructions.count() - 1) return false;
    auto* call = as<Call>(block->instructions[pos]);
    if (call == nullptr) return false;
    if (!(call->arguments().count() > 1)) return false;
    if (!call->arguments()[0]->match_ld_loca(v)) return false;
    if (call->method->is_static()) return false;
    if (!typesystem::is_known_type(call->method->declaring_type(), KnownTypeCode::DefaultInterpolatedStringHandler)) return false;
    const std::string method_name = call->method->name();
    const int count = call->arguments().count();
    if (method_name == "AppendLiteral" && count == 2 && is<LdStr>(call->arguments()[1])) return true;
    if (method_name == "AppendFormatted" && count == 2) return true;
    if (method_name == "AppendFormatted" && count == 3 && is<LdStr>(call->arguments()[2])) return true;
    if (method_name == "AppendFormatted" && count == 3 && is<LdcI4>(call->arguments()[2])) return true;
    if (method_name == "AppendFormatted" && count == 4 && is<LdcI4>(call->arguments()[2]) && is<LdStr>(call->arguments()[3])) return true;
    return false;
}

bool InterpolatedStringTransform::find_to_string_and_clear(Block* block, int pos, int interpolation_start, int interpolation_end,
                                                           ILVariable* v, ILInstruction** insertion_point) const {
    *insertion_point = nullptr;
    if (pos >= block->instructions.count()) return false;
    // find
    // ... call ToStringAndClear(ldloca v) ...
    // in block.Instructions[pos]
    for (int i = interpolation_start; i < interpolation_end; i++) {
        ILInlining::FindResult result =
            ILInlining::find_load_in_next(block->instructions[pos], v, block->instructions[i], InliningOptions::None);
        if (result.type != ILInlining::FindResultType::Found) return false;
        if (*insertion_point == nullptr) *insertion_point = result.load_inst->parent();
        assert(*insertion_point == result.load_inst->parent());
    }
    auto* call = as<Call>(*insertion_point);
    return call != nullptr && call->arguments().count() == 1 && call->method->name() == "ToStringAndClear" && !call->method->is_static();
}

}  // namespace cb::dotnet::decompiler::il::transforms
