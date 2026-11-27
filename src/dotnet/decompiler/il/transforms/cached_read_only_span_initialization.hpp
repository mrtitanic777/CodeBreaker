// dotnet/decompiler/il/transforms/cached_read_only_span_initialization.hpp -- collapses the lazy cache Roslyn emits for a
// ReadOnlySpan<T> made from an array literal on frameworks without RuntimeHelpers.CreateSpan: the load from the compiler-generated cache
// field, the null check and the write-back become one store of the array initializer, so the unwritable <PrivateImplementationDetails>
// field disappears from the output. Runs right after CachedDelegateInitialization, which does the same for anonymous-method delegates.
//
//     stloc V(ldobj T[](ldsflda cache))               stloc V(arrayInitializer)
//     if (V == null) { stloc V(arrayInitializer);  =>  ... one use of V ...
//                      stobj(ldsflda cache, V) }
//
//     CachedReadOnlySpanInitialization transform;
//     transform.run(block, context);
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.Transforms: CachedReadOnlySpanInitialization, from CachedReadOnlySpanInitialization.cs),
// Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <string>

#include "dotnet/decompiler/il/transforms/il_transform.hpp"

namespace cb::dotnet::decompiler::il::transforms {

class CachedReadOnlySpanInitialization final : public IBlockTransform {
public:
    void run(Block* block, BlockTransformContext& context) override;
    std::string name() const override { return "CachedReadOnlySpanInitialization"; }

private:
    // Matches `stloc V(ldobj(ldsflda cacheField))` at block[i - 1] and the if at block[i], and replaces the load from the cache by the
    // initializer value, dropping the if.
    static bool do_transform(Block* block, int i, IfInstruction* inst, BlockTransformContext& context);
};

}  // namespace cb::dotnet::decompiler::il::transforms
