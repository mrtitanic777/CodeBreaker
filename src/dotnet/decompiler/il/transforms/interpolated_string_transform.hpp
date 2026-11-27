// dotnet/decompiler/il/transforms/interpolated_string_transform.hpp -- recognizes the code the C# compiler emits for an interpolated
// string built with DefaultInterpolatedStringHandler (a handler local, calls to AppendLiteral and AppendFormatted on it, then
// ToStringAndClear) and moves the calls into an InterpolatedString block around the ToStringAndClear call.
//
//     stloc v(newobj DefaultInterpolatedStringHandler..ctor(ldc.i4 7, ldc.i4 1))
//     call AppendLiteral(ldloca v, ldstr "Hello ")                =>   ... Block InterpolatedString {
//     call AppendFormatted(ldloca v, ldloc name)                           call AppendLiteral(...) ; call AppendFormatted(...)
//     ... call ToStringAndClear(ldloca v) ...                              final: call ToStringAndClear(ldloca v) } ...
//
//     InterpolatedStringTransform transform;
//     transform.run(block, pos, context);
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.Transforms: InterpolatedStringTransform, from Transforms/InterpolatedStringTransform.cs),
// Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <string>

#include "dotnet/decompiler/il/transforms/il_transform.hpp"

namespace cb::dotnet::decompiler::il::transforms {

class InterpolatedStringTransform final : public IStatementTransform {
public:
    void run(Block* block, int pos, StatementTransformContext& context) override;
    std::string name() const override { return "InterpolatedStringTransform"; }

private:
    bool is_known_call(Block* block, int pos, ILVariable* v) const;
    bool find_to_string_and_clear(Block* block, int pos, int interpolation_start, int interpolation_end, ILVariable* v,
                                  ILInstruction** insertion_point) const;
};

}  // namespace cb::dotnet::decompiler::il::transforms
