// dotnet/decompiler/il/transforms/copy_propagation.hpp -- a very simple form of copy propagation, used for assignments from arguments to
// local variables (a single-assignment target of an argument that is never changed is replaced by the argument) and for assignments
// of address-loading instructions to locals; also removes dead stores to stack slots.
//
//     CopyPropagation transform;
//     transform.run(function, context);
//     CopyPropagation::propagate(store, context);        // one single-definition store
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.Transforms: CopyPropagation, from Transforms/CopyPropagation.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <string>
#include <unordered_set>

#include "dotnet/decompiler/il/transforms/il_transform.hpp"

namespace cb::dotnet::decompiler {
class DecompilerSettings;
}

namespace cb::dotnet::decompiler::il::transforms {

class CopyPropagation final : public IILTransform {
public:
    using VariableSet = std::unordered_set<ILVariable*, ILVariableHash, ILVariableEqualityComparer>;

    // Propagates the value of the store (whose variable has a single definition) to all loads of the variable.
    static void propagate(StLoc* store, ILTransformContext& context);

    void run(ILFunction* function, ILTransformContext& context) override;
    std::string name() const override { return "CopyPropagation"; }

private:
    static void run_on_block(Block* block, ILTransformContext& context, const VariableSet* split_variables = nullptr);
    static bool can_perform_copy_propagation(ILVariable* target, ILInstruction* value, const VariableSet* split_variables,
                                             const DecompilerSettings& settings);
    static void do_propagate(ILVariable* v, ILInstruction* copied_expr, Block* block, int& i, ILTransformContext& context);
};

}  // namespace cb::dotnet::decompiler::il::transforms
