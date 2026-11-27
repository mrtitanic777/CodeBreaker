// dotnet/decompiler/il/transforms/named_argument_transform.hpp -- introduces named arguments where inlining a variable into a call would
// otherwise change the order the arguments are evaluated in: the argument that loads the variable is stored first into a named argument
// variable, inside a CallWithNamedArgs block around the call, and the call reads it back.
//
//     stloc x(f())                           Block CallWithNamedArgs {
//     call M(g(), ldloc x)           =>        stloc x(f())
//                                              call M(g(), ldloc x)      // written M(g(), b: f())
//                                          }
//
//     NamedArgumentTransform transform;
//     transform.run(block, pos, context);                     // as a statement transform
//     FindResult r = NamedArgumentTransform::can_introduce_named_argument(call, child, v, expression_being_moved);   // for ILInlining
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.Transforms: NamedArgumentTransform, from Transforms/NamedArgumentTransform.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler)..
#pragma once

#include <string>

#include "dotnet/decompiler/il/transforms/il_inlining.hpp"
#include "dotnet/decompiler/il/transforms/il_transform.hpp"

namespace cb::dotnet::decompiler::il::transforms {

class NamedArgumentTransform final : public IStatementTransform {
public:
    using FindResult = ILInlining::FindResult;
    using FindResultType = ILInlining::FindResultType;

    static FindResult can_introduce_named_argument(CallInstruction* call, ILInstruction* child, ILVariable* v,
                                                   ILInstruction* expression_being_moved);
    static FindResult can_extend_named_argument(Block* block, ILVariable* v, ILInstruction* expression_being_moved);

    // Introduces a named argument for 'arg' and evaluates it before the other arguments (except for the "this" pointer).
    static void introduce_named_argument(ILInstruction* arg, ILTransformContext& context);

    void run(Block* block, int pos, StatementTransformContext& context) override;
    std::string name() const override { return "NamedArgumentTransform"; }
};

}  // namespace cb::dotnet::decompiler::il::transforms
