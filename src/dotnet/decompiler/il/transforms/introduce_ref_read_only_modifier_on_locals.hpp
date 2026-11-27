// dotnet/decompiler/il/transforms/introduce_ref_read_only_modifier_on_locals.hpp -- marks a by-reference local `ref readonly` when
// what is stored into it is known to be a readonly reference (C# then requires the modifier on the local), or it holds a
// readonly.ldelema address, so the declared local keeps the meaning.
//
//     IntroduceRefReadOnlyModifierOnLocals transform;
//     transform.run(function, context);
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL: IntroduceRefReadOnlyModifierOnLocals, from Transforms/IntroduceRefReadOnlyModifierOnLocals.cs),
// Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <string>

#include "dotnet/decompiler/il/transforms/il_transform.hpp"

namespace cb::dotnet::decompiler::il::transforms {

class IntroduceRefReadOnlyModifierOnLocals final : public IILTransform {
public:
    void run(ILFunction* function, ILTransformContext& context) override;
    std::string name() const override { return "IntroduceRefReadOnlyModifierOnLocals"; }

private:
    // Infers `ref readonly` from usage: the variable is a by-ref local and a value stored into it is known to be readonly.
    static bool is_used_as_ref_readonly(ILVariable* variable);
};

}  // namespace cb::dotnet::decompiler::il::transforms
