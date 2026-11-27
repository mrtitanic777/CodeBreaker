// dotnet/decompiler/il/transforms/lock_transform.hpp -- recognizes the try-finally the C# compilers emit for a `lock` statement
// (Roslyn's Monitor.Enter(obj, ref flag) form, the .NET 2 and 4 forms, the yield-return state machine form and the Mono compiler's) and
// turns it into one LockInstruction over the locked expression.
//
//     stloc obj(lockObj); stloc flag(0)                  .lock (lockObj) BlockContainer {
//     .try { call Enter(obj, ldloca flag); ... }   =>      ...
//     finally { if (flag) call Exit(obj) }               }
//
//     LockTransform transform;
//     transform.run(block, context);
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.Transforms: LockTransform, from LockTransform.cs), Copyright AlphaSierraPapa for the ILSpy
// team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <string>
#include <vector>

#include "dotnet/decompiler/il/transforms/il_transform.hpp"

namespace cb::dotnet::decompiler::il::transforms {

class LockTransform final : public IBlockTransform {
public:
    void run(Block* block, BlockTransformContext& context) override;
    std::string name() const override { return "LockTransform"; }

private:
    bool do_transform(Block* block, int i);
    bool transform_lock_mcs(Block* block, int i);
    bool transform_lock_v2(Block* block, int i);
    bool transform_lock_v4(Block* block, int i);
    bool transform_lock_v4_yield_return(Block* block, int i);
    bool transform_lock_roslyn(Block* block, int i);

    bool match_exit_block(Block* entry_point, ILVariable* flag, ILVariable* obj);
    bool match_exit_block(Block* exit_block, ILVariable* obj);
    bool match_lock_entry_point(Block* entry_point, ILVariable* flag, ILVariable* obj);
    bool match_lock_entry_point(Block* entry_point, ILVariable* flag, StLoc** obj);
    bool match_lock_entry_point(Block* entry_point, ILVariable* flag, ILVariable** exit_variable, StLoc** obj);
    bool match_call(Call* call, const std::string& method_name, ILVariable* flag, StLoc** obj);
    bool match_call(Call* call, const std::string& method_name, const std::vector<ILVariable*>& variables);

    BlockTransformContext* context_ = nullptr;
};

}  // namespace cb::dotnet::decompiler::il::transforms
