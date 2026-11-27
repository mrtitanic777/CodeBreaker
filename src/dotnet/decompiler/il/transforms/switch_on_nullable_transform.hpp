// dotnet/decompiler/il/transforms/switch_on_nullable_transform.hpp -- detects the switch-on-nullable patterns of the C# compiler (the
// legacy compiler's GetValueOrDefault form and Roslyn's HasValue form) and turns each into one ILAst switch over the nullable's value,
// the null case being a section that has the null label.
//
//     SwitchOnNullableTransform transform;
//     transform.run(function, context);
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.Transforms: SwitchOnNullableTransform, from SwitchOnNullableTransform.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <string>

#include "dotnet/decompiler/il/transforms/il_transform.hpp"

namespace cb::dotnet::decompiler::il::transforms {

class SwitchOnNullableTransform final : public IILTransform {
public:
    void run(ILFunction* function, ILTransformContext& context) override;
    std::string name() const override { return "SwitchOnNullableTransform"; }

private:
    // Matches legacy C# switch on nullable.
    static bool match_switch_on_nullable(InstructionCollection<ILInstruction>& instructions, int i, SwitchInstruction** new_switch);
    static SwitchInstruction* build_lifted_switch(Block* null_case_block, SwitchInstruction* switch_inst, ILInstruction* switch_value,
                                                  const IType* nullable_type);
    // Matches Roslyn C# switch on nullable.
    static bool match_roslyn_switch_on_nullable(InstructionCollection<ILInstruction>& instructions, int i, SwitchInstruction** new_switch);
};

}  // namespace cb::dotnet::decompiler::il::transforms
