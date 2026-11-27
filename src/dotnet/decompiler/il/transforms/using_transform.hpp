// dotnet/decompiler/il/transforms/using_transform.hpp -- recognizes a try-finally whose finally block disposes a variable as a `using`
// statement (the C# and Visual Basic forms, and the await using form that calls DisposeAsync): the store of the resource, the
// try-finally and the dispose check become one UsingInstruction.
//
//     stloc obj(resourceExpression)               using (resourceExpression) {
//     .try { ... } finally { if (obj != null)  =>   ...
//                               obj.Dispose(); }   }
//
//     UsingTransform transform;
//     transform.run(block, context);
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.Transforms: UsingTransform, from UsingTransform.cs), Copyright AlphaSierraPapa for the
// ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <cstdint>
#include <string>

#include "dotnet/decompiler/il/transforms/il_transform.hpp"

namespace cb::dotnet::decompiler::typesystem {
enum class KnownTypeCode : std::uint8_t;
}

namespace cb::dotnet::decompiler::il::transforms {

class UsingTransform final : public IBlockTransform {
public:
    void run(Block* block, BlockTransformContext& context) override;
    std::string name() const override { return "UsingTransform"; }

private:
    bool transform_using(Block* block, int i);
    bool transform_using_vb(Block* block, int i);
    bool check_resource_type(const IType* type);
    static bool implements_foreach_pattern(const IType* type);
    bool match_dispose_block(BlockContainer* container, ILVariable* obj_var, bool using_null, const std::string& dispose_method_full_name,
                             typesystem::KnownTypeCode dispose_type_code);
    bool match_dispose_check(ILVariable* obj_var, ILInstruction* check_inst, bool is_reference, bool using_null, int* num_obj_var_loads_in_check,
                             std::string dispose_method_full_name, typesystem::KnownTypeCode dispose_type_code);
    static bool match_null_check_or_type_check(ILInstruction* condition, ILVariable** obj_var, typesystem::KnownTypeCode dispose_type,
                                               bool* is_inlined_is_inst);
    bool transform_async_using(Block* block, int i);
    static bool check_async_resource_type(const IType* type, std::string* dispose_method_full_name);
    static bool unwrap_await(ILInstruction** await_instruction);

    BlockTransformContext* context_ = nullptr;
};

}  // namespace cb::dotnet::decompiler::il::transforms
