// dotnet/decompiler/il/control_flow/detect_pinned_regions.hpp -- turns IL's pinned locals into scoped pinned regions (C# `fixed`).
//
// IL pins an object with a `pinned` local: a store to it starts the pin, a store of null or zero ends it. C# can only pin in a scoped
// `fixed` block, so the region the pin affects must be found, and all of its blocks collected. That is done early, before the other
// control flow constructs are built (it must run before LoopDetection and after variable inlining). It also recognizes the
// null-safe array-to-pointer pattern and the custom GetPinnableReference pattern, replaces the by-reference pin variable by a native
// pointer, and handles pinned strings.
//
//     DetectPinnedRegions transform;
//     transform.run(function, context);
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.ControlFlow: DetectPinnedRegions, from ControlFlow/DetectPinnedRegions.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <string>

#include "dotnet/decompiler/il/transforms/il_transform.hpp"

namespace cb::dotnet::decompiler::il::control_flow {

class DetectPinnedRegions final : public transforms::IILTransform {
public:
    void run(ILFunction* function, transforms::ILTransformContext& context) override;
    std::string name() const override { return "DetectPinnedRegions"; }

private:
    // Ensures that every write to a pinned local is followed by a branch instruction, so the pinning region has no half blocks.
    void split_blocks_at_writes_to_pinned_locals(BlockContainer* container);

    // null-safe array to pointer, and the custom GetPinnableReference pattern
    void detect_null_safe_array_to_pointer_or_custom_ref_pin(BlockContainer* container);
    bool is_custom_ref_pin_pattern(Block* block, ILInstruction** ldloc_mem, CallInstruction** call_gpr, ILVariable** v, StLoc** ptr_assign,
                                   Block** target_block, Block** null_block, Block** not_null_block);
    bool is_null_safe_array_to_pointer_pattern(Block* block, ILVariable** v, ILVariable** p, Block** target_block);
    bool is_null_safe_array_to_pointer_not_null_block(Block* block, ILVariable* v, ILVariable* p, Block* null_or_empty_block,
                                                      Block* target_block);
    bool is_null_safe_array_to_pointer_not_null_and_not_empty_block(Block* block, ILVariable* v, ILVariable* p, Block* target_block);
    bool is_null_safe_array_to_pointer_null_or_empty_block(Block* block, ILVariable** p, Block** target_block);

    // CreatePinnedRegion
    bool detect_pinned_region(Block* block);
    bool create_pinned_region(Block* block, StLoc* st_loc);
    static void handle_branch_leaving_pinned_region(ILInstruction* potential_branch, const std::vector<int>& reached_edges_per_block,
                                                    BlockContainer* source_container, ILVariable* pinned_region_var);
    static bool is_null_or_zero(ILInstruction* inst);

    // ProcessPinnedRegion: after a pinned region was detected, processes its body, replacing the pin variable with a native pointer
    // as far as possible.
    void process_pinned_region(PinnedRegion* pinned_region);
    void move_array_to_pointer_to_pinned_region_init(PinnedRegion* pinned_region);
    void replace_pinned_var(ILVariable* old_var, ILVariable* new_var, ILInstruction* inst);
    static bool is_slot_accepting_both_managed_and_unmanaged_pointers(const SlotInfo* slot_info);
    static bool is_branch_on_null(ILInstruction* cond_branch, ILVariable* native_var, Block** target_block);
    void handle_string_to_pointer(PinnedRegion* pinned_region);
    static bool is_offset_to_string_data_block(Block* block, ILVariable* native_var, Block* target_block);
    static bool is_offset_to_string_data_call(ILInstruction* inst);
    // Eliminates an extra local variable that Roslyn tends to generate.
    static void use_existing_variable_for_pinned_region(PinnedRegion* pinned_region);

    transforms::ILTransformContext* context_ = nullptr;
};

}  // namespace cb::dotnet::decompiler::il::control_flow
