// dotnet/decompiler/il/transforms/remove_dead_variable_init.hpp -- clears "the initial value is used" from locals that are definitely
// assigned before every use (the initial value is a dead store); removes stores to variables that are never read (when the settings,
// an iterator or an async method ask for it, or the variable was marked redundant), keeping the stored expression when it has side
// effects; and infers the type of `ref` stack slots from what is stored into them.
//
//     RemoveDeadVariableInit transform;
//     transform.run(function, context);
//     RemoveDeadVariableInit::reset_uses_initial_value_flag(function, context);   // only the first part
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.Transforms: RemoveDeadVariableInit, from Transforms/RemoveDeadVariableInit.cs),
// Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <string>

#include "dotnet/decompiler/il/transforms/il_transform.hpp"

namespace cb::dotnet::decompiler::il::transforms {

class RemoveDeadVariableInit final : public IILTransform {
public:
    void run(ILFunction* function, ILTransformContext& context) override;
    std::string name() const override { return "RemoveDeadVariableInit"; }

    // Sets UsesInitialValue to false on every variable that is not a parameter and is never used uninitialized.
    static void reset_uses_initial_value_flag(ILFunction* function, ILTransformContext& context);
};

}  // namespace cb::dotnet::decompiler::il::transforms
