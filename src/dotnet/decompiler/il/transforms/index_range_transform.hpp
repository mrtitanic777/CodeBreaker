// dotnet/decompiler/il/transforms/index_range_transform.hpp -- the C# 8 System.Index / System.Range feature: recognizes the offset
// arithmetic the C# compiler emits for `array[^1]`, `list[^2]` and slices such as `span[1..^1]` (a length read into a temporary,
// GetOffset calls and subtractions from it, a Slice or Substring call) and turns it back into System.Index and System.Range values.
//
//     stloc len(call get_Length(ldloc span)) ; stloc start(call GetOffset(index, ldloc len)) ; ... call get_Item(ldloc span, ldloc start)
//         =>  call get_Item(ldloc span, ldobj System.Index(index))        (a synthetic indexer taking System.Index)
//
//     IndexRangeTransform transform;
//     transform.run(block, pos, context);                          // as a statement transform
//     IndexRangeTransform::handle_ld_elema(ldelema, context);      // `array[System.Index]`, from the expression transforms
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.Transforms: IndexRangeTransform, from Transforms/IndexRangeTransform.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler). Not yet ported: SyntheticRangeIndexAccessor (the
// replacement of an indexer or Slice call by a call taking System.Index or System.Range is not made, and a call already replaced is not
// recognized, so only `array[System.Index]` is transformed).
#pragma once

#include <cstdint>
#include <string>

#include "dotnet/decompiler/il/transforms/il_transform.hpp"

namespace cb::dotnet::decompiler::il::transforms {

class IndexRangeTransform final : public IStatementTransform {
public:
    // Called by expression transforms. Handles the `array[System.Index]` cases.
    static bool handle_ld_elema(LdElema* ldelema, ILTransformContext& context);

    void run(Block* block, int pos, StatementTransformContext& context) override;
    std::string name() const override { return "IndexRangeTransform"; }

private:
    class IndexMethods;

    enum class IndexKind : std::uint8_t {
        // indexLoad is an integer, from the start of the container
        FromStart,
        // indexLoad is loading the address of a System.Index struct
        RefSystemIndex,
        // indexLoad is an integer, from the end of the container
        FromEnd,
        // Always equivalent to `0`, used for the start-index when slicing without a startpoint `a[..end]`
        TheStart,
        // Always equivalent to `^0`, used for the end-index when slicing without an endpoint `a[start..]`
        TheEnd,
    };

    static bool match_index_implicit_conv(ILInstruction* inst, ILInstruction** offset_inst);
    static bool is_slicing_method(const IMethod* method);
    // Checks that the number of uses of the containerLengthVar variable matches those expected in the pattern.
    static bool check_container_length_variable_use_count(ILVariable* container_length_var, IndexKind start_index_kind,
                                                          IndexKind end_index_kind = IndexKind::FromStart);
    // Matches 'addressof System.Index(call get_Start/get_End(ldloca rangeVar))'.
    static bool match_index_from_range(IndexKind index_kind, ILInstruction* index_load, ILVariable* range_var, const char* accessor_name);
    static ILInstruction* make_index(IndexKind index_kind, ILInstruction* index_load, const IndexMethods& special_methods);
    // Whether the C# compiler will call `container[int]` when using `container[Index]`.
    static bool csharp_will_generate_indexer(const IType* declaring_type, bool slicing);
    // Matches `stloc containerLengthVar(call get_Length/get_Count(ldloc containerVar))`.
    static bool match_container_length_store(ILInstruction* inst, ILVariable** length_var, ILVariable** container_var);
    // If lengthVar is non-null, matches 'ldloc lengthVar'; otherwise `call get_Length/get_Count(ldloc containerVar)`.
    static bool match_container_length(ILInstruction* init, ILVariable* length_var, ILVariable** container_var);
    static bool match_container_var(ILInstruction* inst, ILVariable** container_var);
    // Matches an instruction computing an offset: `call System.Index.GetOffset(indexLoad, ldloc containerLengthVar)` or
    // `binary.sub.i4(ldloc containerLengthVar, indexLoad)`. Anything else is an `int` expression from the start of the container.
    static IndexKind match_get_offset(ILInstruction* inst, ILInstruction** index_load, ILVariable* container_length_var, ILVariable** container_var);
    // Matches an instruction computing a slice length: `binary.sub.i4(call GetOffset(endIndexLoad, ldloc length), ldloc startOffset))`.
    static bool match_slice_length(ILInstruction* inst, IndexKind* end_index_kind, ILInstruction** end_index_load, ILVariable* container_length_var,
                                   ILVariable** container_var, ILVariable* start_offset_var);
};

}  // namespace cb::dotnet::decompiler::il::transforms
