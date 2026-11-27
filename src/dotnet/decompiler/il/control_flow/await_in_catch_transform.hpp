// dotnet/decompiler/il/control_flow/await_in_catch_transform.hpp -- puts back the catch blocks that contain an `await`: the compiler
// moves such a catch body out of the catch (it cannot await there), leaving a stub that stores the exception and an id and jumps to a
// table, and after the try it dispatches on the id to the real body. This moves each real body back into its handler.
//
//     AwaitInCatchTransform::run(function, context);
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.ControlFlow: AwaitInCatchTransform, from ControlFlow/AwaitInCatchTransform.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <unordered_set>
#include <vector>

#include "dotnet/decompiler/il/instructions.hpp"
#include "dotnet/decompiler/il/transforms/il_transform.hpp"

namespace cb::dotnet::decompiler::il::control_flow {

class AwaitInCatchTransform {
public:
    static void run(ILFunction* function, transforms::ILTransformContext& context);

    // Matches the idiom that rethrows a caught exception object keeping its stack trace:
    //   stloc typedExceptionVariable(isinst Exception(ldloc objectVariable))
    //   if (comp.o(ldloc typedExceptionVariable != ldnull)) br captureBlock
    //   br throwBlock
    // with captureBlock { callvirt Throw(call Capture(ldloc typedExceptionVariable)) ... } and throwBlock { throw(ldloc objectVariable) };
    // it is what `throw;` in the catch body was turned into. *object_variable is where the object variable is expected (nullptr: any;
    // it is set to the one found).
    static bool match_exception_capture_block(transforms::ILTransformContext& context, Block* block, ILVariable** object_variable,
                                              StLoc** typed_exception_variable_store, Block** capture_block, Block** throw_block);

private:
    struct CatchBlockInfo {
        int id = 0;
        TryCatchHandler* handler = nullptr;
        Block* real_catch_block_entry_point = nullptr;
        ILInstruction* next_block_or_exit_container = nullptr;
        ILInstruction* jump_table_entry = nullptr;
        ILVariable* object_variable = nullptr;
    };

    static void transform_async_throw_to_throw(transforms::ILTransformContext& context, std::unordered_set<Block*>& removed_blocks, Block* block);
    static void move_block(Block* block, BlockContainer* target);
    // Analyzes all catch handlers and returns every handler that follows the await catch handler pattern.
    static bool analyze_handlers(InstructionCollection<TryCatchHandler>& handlers, ILVariable** catch_handler_identifier,
                                 std::vector<CatchBlockInfo>* transformable_catch_blocks);
    // Matches the await catch handler pattern:
    // [stloc V_3(ldloc E_100)	- copy exception variable to a temporary]
    // stloc V_6(ldloc V_3)	- store exception in 'global' object variable
    // stloc V_5(ldc.i4 2)		- store id of catch block in 'identifierVariable'
    // br IL_0075				- jump out of catch block to the head of the catch-handler jump table
    static bool match_await_catch_handler(TryCatchHandler* handler, int* id, ILVariable** identifier_variable, Block** real_entry_point,
                                          ILInstruction** next_block_or_exit_container, ILInstruction** jump_table_entry, ILVariable** object_variable);
    static bool parse_switch_jump_table(int id, SwitchInstruction* jump_table, ILVariable* identifier_variable, Block** real_entry_point,
                                        ILInstruction** next_block_or_exit_container, ILInstruction** jump_table_entry);
    static bool parse_if_jump_table(int id, Block* jump_table_entry_block, ILVariable* identifier_variable, Block** real_entry_point,
                                    ILInstruction** next_block_or_exit_container, ILInstruction** jump_table_entry);
};

}  // namespace cb::dotnet::decompiler::il::control_flow
