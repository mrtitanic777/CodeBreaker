// dotnet/decompiler/il/transforms/inline_array_transform.hpp -- turns the calls the C# compiler emits to reach an element of an
// inline array (a struct marked [InlineArray]) into ldelema.inlinearray: the Span indexer applied to the span of the array, and the
// InlineArrayElementRef and InlineArrayFirstElementRef helpers of <PrivateImplementationDetails>.
//
//     call get_Item(addressof Span(call InlineArrayAsSpan(addr, 4)), index)   =>   ldelema.inlinearray T(addr, index)
//     call InlineArrayElementRef(addr, ldc.i4 2)                              =>   ldelema.inlinearray T(addr, ldc.i4 2)
//     call InlineArrayFirstElementRef(addr)                                   =>   ldelema.inlinearray T(addr, ldc.i4 0)
//
//     if (InlineArrayTransform::run_on_expression(call, context)) ...         // called by the expression transforms
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.Transforms: InlineArrayTransform, from Transforms/InlineArrayTransform.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include "dotnet/decompiler/il/transforms/il_transform.hpp"

namespace cb::dotnet::decompiler::il::transforms {

class InlineArrayTransform {
public:
    // Replaces the call by an ldelema.inlinearray when it is one of the patterns; whether it did.
    static bool run_on_expression(Call* inst, StatementTransformContext& context);

private:
    static bool match_span_indexer_with_inline_array_as_span(Call* inst, const IType** type, ILInstruction** addr, ILInstruction** index,
                                                             bool* is_read_only);
    static bool match_inline_array_element_ref(Call* inst, const IType** type, ILInstruction** addr, ILInstruction** index,
                                               bool* is_read_only);
    static bool match_inline_array_first_element_ref(Call* inst, const IType** type, ILInstruction** addr, bool* is_read_only);
    static bool match_span_get_item(const IMethod* method, const char* type_name);
    static bool match_inline_array_helper(const IMethod* method, const char* method_name, const IType** inline_array_type);
};

}  // namespace cb::dotnet::decompiler::il::transforms
