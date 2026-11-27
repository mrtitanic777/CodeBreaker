// dotnet/decompiler/il/transforms/cached_delegate_initialization.hpp -- removes the lazy cache the C# and Visual Basic compilers put
// around an anonymous method's delegate (a static field or a local that is null-checked and filled on first use), so the delegate
// construction stands where the delegate is used.
//
//     if (CachedAnonMethodDelegate == null)           ... one use of the DelegateConstruction ...
//         CachedAnonMethodDelegate = DelegateConstruction;   =>
//     ... one use of CachedAnonMethodDelegate ...
//
//     CachedDelegateInitialization transform;
//     transform.run(block, context);
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.Transforms: CachedDelegateInitialization, from CachedDelegateInitialization.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <string>

#include "dotnet/decompiler/il/transforms/il_transform.hpp"

namespace cb::dotnet::decompiler::il::transforms {

class CachedDelegateInitialization final : public IBlockTransform {
public:
    void run(Block* block, BlockTransformContext& context) override;
    std::string name() const override { return "CachedDelegateInitialization"; }

private:
    bool cached_delegate_initialization_with_field(IfInstruction* inst);
    bool cached_delegate_initialization_with_local(IfInstruction* inst);
    bool cached_delegate_initialization_roslyn_in_static_with_local(IfInstruction* inst);
    bool cached_delegate_initialization_roslyn_with_local(IfInstruction* inst);
    bool cached_delegate_initialization_vb(IfInstruction* inst);
    bool cached_delegate_initialization_vb_with_return(IfInstruction* inst);
    bool cached_delegate_initialization_vb_with_closure(IfInstruction* inst);

    BlockTransformContext* context_ = nullptr;
};

}  // namespace cb::dotnet::decompiler::il::transforms
