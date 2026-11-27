// dotnet/decompiler/il/transforms/ld_loca_dup_init_obj_transform.hpp -- rewrites the `ldloca; dup; initobj` sequence of Roslyn 2 and
// later into the `ldloca; ldloca; initobj` form of the old csc, which later inlining understands:
//
//     stloc s(ldloca v); stobj T(ldloc s, default.value T)         (s loaded more than once)
//     =>  stloc v(default.value T); stloc s(ldloca v)
//
//     LdLocaDupInitObjTransform transform;
//     transform.run(function, context);
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.Transforms: LdLocaDupInitObjTransform, from Transforms/LdLocaDupInitObjTransform.cs),
// Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <string>

#include "dotnet/decompiler/il/transforms/il_transform.hpp"

namespace cb::dotnet::decompiler::il::transforms {

class LdLocaDupInitObjTransform final : public IILTransform {
public:
    void run(ILFunction* function, ILTransformContext& context) override;
    std::string name() const override { return "LdLocaDupInitObjTransform"; }

private:
    static bool try_transform(Block* block, int i, ILTransformContext& context);
};

}  // namespace cb::dotnet::decompiler::il::transforms
