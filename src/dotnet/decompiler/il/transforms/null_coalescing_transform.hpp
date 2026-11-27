// dotnet/decompiler/il/transforms/null_coalescing_transform.hpp -- builds the NullCoalescingInstruction (`if.notnull(a, b)`, C#'s `??`)
// for reference types (`stloc s(a); if (s == null) s = b;`), including the throw-expression forms (`a ?? throw ...`), a null-guard the
// compiler hoisted in front of a chained constructor call, and the same with a nullable value type's HasValue check. The `??` of
// nullable value types is made by NullableLiftingTransform.
//
//     NullCoalescingTransform transform;
//     transform.run(block, pos, statement_context);
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.Transforms: NullCoalescingTransform, from Transforms/NullCoalescingTransform.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler)..
#pragma once

#include <string>

#include "dotnet/decompiler/il/transforms/il_transform.hpp"

namespace cb::dotnet::decompiler::il::transforms {

class NullCoalescingTransform final : public IStatementTransform {
public:
    void run(Block* block, int pos, StatementTransformContext& context) override;
    std::string name() const override { return "NullCoalescingTransform"; }

private:
    // Handles NullCoalescingInstruction case 1: reference types.
    bool transform_ref_types(Block* block, int pos, StatementTransformContext& context);
    // When an argument of a chained constructor call contains a throwing null-check (e.g. `arg ?? throw ...`) and `arg` is evaluated
    // more than once, the C# compiler hoists the null-check in front of the chained call:
    //   if (comp.o(ldloc param == ldnull)) throw(...)
    //   call Base..ctor(..., ldlen(ldloc param), ..., ldloc param, ...)
    // The guard then blocks TransformFieldAndConstructorInitializers from lifting the chained call into a `: this(...)` / `: base(...)`
    // clause. Replace the guard with
    //   stloc temp(if.notnull(ldloc param, throw(...)))
    // redirecting the first following use of the parameter (the position the check was hoisted from) to `temp`, and leave moving the
    // coalescing expression into the chained call to ILInlining, which does so only when that preserves the order of evaluation.
    // Only reference-type chains (`base/this..ctor` calls, identified by IL offset via ILInlining::is_in_constructor_initializer) need
    // this: value types chain via `this = new TSelf(...)`, which is an ordinary body statement, so a preceding guard statement is legal
    // C# and can stay.
    bool transform_hoisted_constructor_argument_null_guard(Block* block, int pos, StatementTransformContext& context);
    // Matches a hoisted argument null-guard `if (comp(ldloc param == ldnull)) throw(...)` (no else branch). Only parameters qualify:
    // nothing else is in scope before the constructor initializer.
    static bool is_argument_null_guard(ILInstruction* inst, LdLoc** param_load, Throw** throw_inst);
    // stloc v(value)
    // if (logic.not(call get_HasValue(ldloca v))) throw(...)
    // ... Call(arg1, arg2, call GetValueOrDefault(ldloca v), arg4) ...
    // =>
    // ... Call(arg1, arg2, if.notnull(value, throw(...)), arg4) ...
    bool transform_throw_expression_value_types(Block* block, int pos, StatementTransformContext& context);
};

}  // namespace cb::dotnet::decompiler::il::transforms
