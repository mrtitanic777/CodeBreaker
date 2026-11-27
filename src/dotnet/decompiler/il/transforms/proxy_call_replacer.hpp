// dotnet/decompiler/il/transforms/proxy_call_replacer.hpp -- replaces a call to a compiler-generated proxy method (a method of the
// current or an outer class whose whole body is one call that passes its parameters on) with a call to the method it forwards to. The
// async-await decompiler needs this for the `await` helpers the compiler wraps in such proxies.
//
//     ProxyCallReplacer transform;
//     transform.run(function, context);
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.Transforms: ProxyCallReplacer, from Transforms/ProxyCallReplacer.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler). Not yet ported: CSharpDecompiler's EarlyILTransforms,
// which upstream runs over the proxy's body before matching it (see "WAITS" in the .cxx).
#pragma once

#include <string>

#include "dotnet/decompiler/il/transforms/il_transform.hpp"

namespace cb::dotnet::decompiler::typesystem {
class ITypeDefinition;
}

namespace cb::dotnet::decompiler::il::transforms {

class ProxyCallReplacer final : public IILTransform {
public:
    void run(ILFunction* function, ILTransformContext& context) override;
    std::string name() const override { return "ProxyCallReplacer"; }

private:
    static void run_on_call(CallInstruction* inst, ILTransformContext& context);
    static bool is_defined_in_current_or_outer_class(const IMethod* method, const typesystem::ITypeDefinition* declaring_type_definition);
};

}  // namespace cb::dotnet::decompiler::il::transforms
