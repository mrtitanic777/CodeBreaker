// dotnet/decompiler/il/transforms/inline_return_transform.hpp -- duplicates (or moves) a return block into the containers that
// store a local right before branching to it, so `v = x; br RET` ... `RET: return v` becomes `v = x; br RET'` with a return block
// of its own in the container of the store, where later transforms can inline the store into the return.
//
//     InlineReturnTransform transform;
//     transform.run(function, context);
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.Transforms: InlineReturnTransform, from Transforms/InlineReturnTransform.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <string>
#include <tuple>
#include <vector>

#include "dotnet/decompiler/il/transforms/il_transform.hpp"

namespace cb::dotnet::decompiler::il::transforms {

class InlineReturnTransform final : public IILTransform {
public:
    void run(ILFunction* function, ILTransformContext& context) override;
    std::string name() const override { return "InlineReturnTransform"; }

private:
    // Collects the stores to `return_var`: false if any is not a stloc directly in a block as its penultimate instruction, followed
    // by a branch to `leave_block`, with a block container as an ancestor. Otherwise `instructions_to_modify` gets, for each, the
    // target block container, the leave block and the branch that should be modified.
    static bool can_modify_instructions(ILVariable* return_var, Block* leave_block,
                                        std::vector<std::tuple<BlockContainer*, Block*, Branch*>>& instructions_to_modify);
};

}  // namespace cb::dotnet::decompiler::il::transforms
