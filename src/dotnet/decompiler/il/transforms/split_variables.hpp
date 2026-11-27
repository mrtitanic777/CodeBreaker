// dotnet/decompiler/il/transforms/split_variables.hpp -- live range splitting for IL variables: a local that is used for several
// unrelated values (the compiler reuses slots) becomes one variable per set of stores that can reach the same loads, found with the
// reaching definitions analysis. Parameters, pinned locals and most stack slots are left alone; so are locals whose address is used
// in a way the analysis does not understand.
//
//     SplitVariables transform;
//     transform.run(function, context);
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.Transforms: SplitVariables, from Transforms/SplitVariables.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler). The reference to `Span<T>`'s indexer accessor is by the
// declaring type and the accessor's shape, as upstream.
#pragma once

#include <cstdint>
#include <string>

#include "dotnet/decompiler/il/transforms/il_transform.hpp"

namespace cb::dotnet::decompiler::il::transforms {

class SplitVariables final : public IILTransform {
public:
    void run(ILFunction* function, ILTransformContext& context) override;
    std::string name() const override { return "SplitVariables"; }

private:
    class GroupStores;

    static bool is_candidate_variable(ILVariable* v);

    enum class AddressUse : std::uint8_t {
        Unknown,
        // Address is immediately used for reading and/or writing, without the possibility of the variable being directly stored to
        // (via 'stloc') in between the 'ldloca' and the use of the address.
        Immediate,
        // We support some limited form of ref locals referring to a target variable, without giving up splitting of the target
        // variable. Requirements:
        //  * the ref local is single-assignment
        //  * the ref local is initialized directly with 'ldloca target; stloc ref_local', not a derived pointer (e.g.
        //    'ldloca target; ldflda F; stloc ref_local').
        //  * all uses of the ref_local are immediate.
        // There may be stores to the target variable in between the 'stloc ref_local' and its uses, but we handle that case by treating
        // each use of the ref_local as an address access of the target variable (as if the ref_local was eliminated via copy
        // propagation).
        WithSupportedRefLocals,
    };

    static AddressUse determine_address_use(ILInstruction* address_loading_instruction, ILVariable* target_var);
    static AddressUse handle_call(ILInstruction* address_loading_instruction, ILVariable* target_var, CallInstruction* call);
    static bool is_span_of_t_indexer_accessor(const IMethod* method);
    // Given 'ldloc ref_local' and 'ldloca target; stloc ref_local', returns the ldloca. Must return a non-null LdLoca for every use of
    // a supported ref local.
    static LdLoca* get_address_load_for_ref_local_use(LdLoc* ldloc);
};

}  // namespace cb::dotnet::decompiler::il::transforms
