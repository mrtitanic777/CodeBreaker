// dotnet/decompiler/il/control_flow/remove_redundant_return.hpp -- removes the `leave` of the whole function (return / yield break) at
// the end of a void function or iterator where falling off the end does the same, so the C# needs no `return;`. Like DetectExitPoints,
// but it only acts where no goto is needed.
//
//     RemoveRedundantReturn transform;
//     transform.run(function, context);
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.ControlFlow: RemoveRedundantReturn, from ControlFlow/RemoveRedundantReturn.cs),
// Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <string>

#include "dotnet/decompiler/il/transforms/il_transform.hpp"

namespace cb::dotnet::decompiler::il::control_flow {

class RemoveRedundantReturn final : public transforms::IILTransform {
public:
    void run(ILFunction* function, transforms::ILTransformContext& context) override;
    std::string name() const override { return "RemoveRedundantReturn"; }

private:
    // Whether the instruction (last in its block) ended in a return that was removed or turned into a leave of its container.
    static bool convert_return_to_fallthrough(ILInstruction* inst);
};

}  // namespace cb::dotnet::decompiler::il::control_flow
