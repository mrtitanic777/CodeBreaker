// dotnet/decompiler/il/transforms/transform_array_initializers.hpp -- recognizes the ways a compiler fills an array in place and turns each
// into one array initializer block: the RuntimeHelpers.InitializeArray call over a data field, one store per element, arrays of arrays,
// stackalloc buffers filled by cpblk or by element stores, and the ReadOnlySpan constructor and RuntimeHelpers.CreateSpan over a data
// field (a UTF-8 literal when the bytes are text).
//
//     stloc a(newarr int(ldc.i4 2))                  stloc a(Block ArrayInitializer {
//     stobj int(ldelema int(ldloc a, 0), 1)     =>       stloc t(newarr int(2)); stobj(ldelema(ldloc t, 0), 1); ...; final: ldloc t })
//     stobj int(ldelema int(ldloc a, 1), 2)
//
//     TransformArrayInitializers transform;
//     transform.run(block, pos, context);
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.Transforms: TransformArrayInitializers, from TransformArrayInitializers.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "dotnet/decompiler/il/transforms/il_transform.hpp"
#include "dotnet/decompiler/metadata/metadata_file.hpp"
#include "dotnet/decompiler/srm_extensions.hpp"

namespace cb::dotnet::decompiler::il::transforms {

class TransformArrayInitializers final : public IStatementTransform {
public:
    // The elements of an array initializer: the indices of each (one LdcI4 per dimension) and its value (nullptr for an element left at
    // its default).
    using ArrayInitializerValues = std::vector<std::pair<std::vector<ILInstruction*>, ILInstruction*>>;

    void run(Block* block, int pos, StatementTransformContext& context) override;
    std::string name() const override { return "TransformArrayInitializers"; }

    // `new ReadOnlySpan<T>(ldsflda data, size)` over a data field -> the initializer (or UTF-8 literal) block in `replacement`.
    static bool transform_span_t_array_initialization(NewObj* inst, StatementTransformContext& context, ILInstruction** replacement);
    // `RuntimeHelpers.CreateSpan<T>(ldmembertoken data)` -> the same.
    static bool transform_runtime_helpers_create_span_initialization(Call* inst, StatementTransformContext& context, ILInstruction** replacement);

    // The simple case where RuntimeHelpers.InitializeArray is not used: one store per element after the array's creation, in index order.
    static bool handle_simple_array_initializer(ILFunction* function, Block* block, int pos, ILVariable* store, const std::vector<int>& array_length,
                                                ArrayInitializerValues* values, int* instructions_to_remove);

    // The zero of an element type (the value of an element the initializer leaves out).
    static ILInstruction* get_null_expression(const IType* element_type);

private:
    bool do_transform(ILFunction* function, Block* body, int pos);
    bool do_transform_multi_dim(ILFunction* function, Block* body, int pos);
    bool do_transform_stack_alloc_initializer(Block* body, int pos);
    bool do_transform_inline_runtime_helpers_initialize_array(Block* body, int pos);

    ILInstruction* read_element(srm::BlobReader& blob, const IType* element_type);
    bool handle_cpblk_initializer(Block* block, int pos, ILVariable* v, long long length, srm::BlobReader* blob, const IType** element_type);
    bool handle_sequential_loc_alloc_initializer(Block* block, int pos, ILVariable* store, ILInstruction* loc_alloc_instruction,
                                                 const IType** element_type, std::vector<StObj*>* values, int* instructions_to_remove);
    void handle_loc_alloc_initializer_prefix(Block* block, int* pos, ILVariable* store, long long byte_count, srm::BlobReader* blob, bool* has_blob,
                                             int* instructions_to_remove);
    ILInstruction* rewrap_store(ILVariable* target, StObj* store_instruction, const IType* type);
    StObj* st_elem_ptr(ILVariable* target, int offset, ILInstruction* value, const IType* type);
    bool handle_jagged_array_initializer(Block* block, int pos, ILVariable* store, const IType* element_type, int length, ILVariable** final_store,
                                         std::vector<ILInstruction*>* values, int* instructions_to_remove);
    bool match_jagged_array_store(Block* block, int pos, ILVariable* store, int index, ILInstruction** initializer, const IType** type);
    bool match_initialize_array_call(ILInstruction* instruction, ILInstruction** array, srm::FieldDefinitionHandle* field);
    bool handle_runtime_helpers_initialize_array(Block* body, int pos, ILVariable* array, const IType* array_type, const std::vector<int>& array_length,
                                                 std::vector<ILInstruction*>* values, int* found_pos);

    StatementTransformContext* context_ = nullptr;
};

}  // namespace cb::dotnet::decompiler::il::transforms
