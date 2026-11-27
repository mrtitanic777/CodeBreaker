// dotnet/decompiler/il/transforms/introduce_native_int_type_on_locals.hpp -- gives a local of type IntPtr or UIntPtr the type `nint` or
// `nuint` where it is used as a native integer (in native-int arithmetic, passed to a native-int parameter) or assigned one, and does
// the same for every local of the same slot in the function.
//
//     IntroduceNativeIntTypeOnLocals transform;
//     transform.run(function, context);
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL: IntroduceNativeIntTypeOnLocals, from IntroduceNativeIntTypeOnLocals.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <string>

#include "dotnet/decompiler/il/transforms/il_transform.hpp"

namespace cb::dotnet::decompiler::typesystem {
class ICompilation;
}

namespace cb::dotnet::decompiler::il::transforms {

class IntroduceNativeIntTypeOnLocals final : public IILTransform {
public:
    void run(ILFunction* function, ILTransformContext& context) override;
    std::string name() const override { return "IntroduceNativeIntTypeOnLocals"; }

private:
    static bool is_used_as_native_int(LdLoc* load);
    static bool is_native_int_store(IStoreInstruction* store, const typesystem::ICompilation& compilation);
};

}  // namespace cb::dotnet::decompiler::il::transforms
