// dotnet/decompiler/il/transforms/early_expression_transforms.hpp -- the expression clean-ups that run before inlining: comparisons
// with null get their canonical kind, `stobj`/`ldobj` of a local's address become `stloc`/`ldloc` (ILInlining only works with those),
// `addressof(ldloc)` under an `ldobj` becomes `ldloca`, runtime-async `AsyncHelpers.Await(x)` calls become `await`, and
// `new decimal(...)` with constant arguments becomes a decimal constant.
//
//     EarlyExpressionTransforms transform;
//     transform.run(function, context);
//     EarlyExpressionTransforms::st_obj_to_st_loc(st_obj, context);     // the helpers other transforms call
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.Transforms: EarlyExpressionTransforms, from Transforms/EarlyExpressionTransforms.cs),
// Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <string>

#include "dotnet/decompiler/il/il_visitor.hpp"
#include "dotnet/decompiler/il/transforms/il_transform.hpp"

namespace cb::dotnet::decompiler::il::transforms {

class EarlyExpressionTransforms final : public ILVisitor, public IILTransform {
public:
    void run(ILFunction* function, ILTransformContext& context) override;
    std::string name() const override { return "EarlyExpressionTransforms"; }

    void default_visit(ILInstruction& inst) override;
    void visit_comp(Comp& inst) override;
    void visit_st_obj(StObj& inst) override;
    void visit_ld_obj(LdObj& inst) override;
    void visit_call(Call& inst) override;
    void visit_new_obj(NewObj& inst) override;

    static void fix_comparison_kind_ld_null(Comp* inst, ILTransformContext& context);
    // This transform is required because ILInlining only works with stloc/ldloc.
    static bool st_obj_to_st_loc(StObj* inst, ILTransformContext& context);
    static bool ld_obj_to_ld_loc(LdObj* inst, ILTransformContext& context);
    // ldobj(...(addressof(ldloc V))) where ... can be zero or more ldflda instructions => ldobj(...(ldloca V))
    static void address_of_ld_loc_to_ld_loca(LdObj* inst, ILTransformContext& context);
    // Runtime-async lowering emits `call AsyncHelpers.Await(value)` in place of the IL Await instruction. Converts it back so the
    // transforms that match on Await see the shape the state-machine async pipeline also produces. Only in a function that is
    // runtime-async (the ILReader sets IsAsync iff the MethodImpl.Async bit is present).
    static bool transform_async_helpers_await_to_await(Call* inst, ILTransformContext& context);
    static bool is_async_helpers_method(const IMethod* method, const std::string& name);

private:
    static bool transform_decimal_ctor_to_constant(NewObj* inst, LdcDecimal** result);

    ILTransformContext* context_ = nullptr;
};

}  // namespace cb::dotnet::decompiler::il::transforms
