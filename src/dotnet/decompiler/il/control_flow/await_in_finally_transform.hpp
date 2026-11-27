// dotnet/decompiler/il/control_flow/await_in_finally_transform.hpp -- puts back the finally blocks that contain an `await`: the compiler
// turns such a try-finally into a try-catch that stores the exception object, the finally body after it, and a rethrow of the stored
// exception. This moves the finally body into a TryFinally again and sends the exits of the try to where the state they set leads.
//
//     AwaitInFinallyTransform::run(function, context);
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.ControlFlow: AwaitInFinallyTransform, from ControlFlow/AwaitInFinallyTransform.cs),
// Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <tuple>

#include "dotnet/decompiler/il/instructions.hpp"
#include "dotnet/decompiler/il/transforms/il_transform.hpp"

namespace cb::dotnet::decompiler::il::control_flow {

class ControlFlowGraph;

class AwaitInFinallyTransform {
public:
    static void run(ILFunction* function, transforms::ILTransformContext& context);

    // Matches a catch handler that stores its exception in an "object"-typed local and then branches out of the catch:
    //     [stloc tmp(ldloc exceptionVariable);]
    //     stloc objectVariable(ldloc tmp_or_exceptionVariable)
    //     br branchTarget
    // Used by the state-machine async lowering AND by the runtime-async try-finally lowering.
    static bool match_object_store_catch_handler(BlockContainer* catch_block_container, ILVariable* exception_variable, ILVariable** object_variable,
                                                 Block** branch_target);

    // Move `new_entry_point` and every block it dominates (excluding any block dominated by `end_block` other than `end_block` itself) from
    // their current container into `target_container`.
    static void move_dominated_blocks_to_container(Block* new_entry_point, Block* end_block, ControlFlowGraph& graph, BlockContainer* target_container,
                                                   transforms::ILTransformContext& context);

private:
    static void simplify_end_of_finally(transforms::ILTransformContext& context, ILVariable* object_variable, Block* before_exception_capture_block,
                                        ILVariable* object_variable_copy, BlockContainer* finally_container);
    static bool validate_state_variable(ILVariable* state_variable, StLoc* initializer, TryCatch* try_catch, Block* entry_point_of_finally);
    static void move_block(Block* block, BlockContainer* target, transforms::ILTransformContext& context);
    // (the block after the finally, the block that captures the exception, the copy of the object variable); the blocks are nullptr when the
    // pattern is not found.
    static std::tuple<Block*, Block*, ILVariable*> find_block_after_finally(transforms::ILTransformContext& context, Block* block,
                                                                              ILVariable* object_variable);
};

}  // namespace cb::dotnet::decompiler::il::control_flow
