// dotnet/decompiler/il/control_flow/runtime_async_manual_await_transform.hpp -- collapses the manual await a runtime-async method is
// compiled to (GetAwaiter, get_IsCompleted, AsyncHelpers.UnsafeAwaitAwaiter and GetResult across three blocks) into one `await`
// instruction, as the state machine pipeline makes it.
//
//     RuntimeAsyncManualAwaitTransform::run(function, context);
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.ControlFlow: RuntimeAsyncManualAwaitTransform, from
// ControlFlow/RuntimeAsyncManualAwaitTransform.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include "dotnet/decompiler/il/instructions.hpp"
#include "dotnet/decompiler/il/transforms/il_transform.hpp"

namespace cb::dotnet::decompiler::il::control_flow {

class RuntimeAsyncManualAwaitTransform {
public:
    static void run(ILFunction* function, transforms::ILTransformContext& context);

private:
    // Block X (head) {
    //     [stloc awaitable(<value>);]                       (optional: when GetAwaiter takes an address)
    //     stloc awaiter(call GetAwaiter(<expr>))
    //     if (call get_IsCompleted(ldloca awaiter)) br completedBlock
    //     br pauseBlock
    // }
    // Block pauseBlock {
    //     call AsyncHelpers.UnsafeAwaitAwaiter[<T>](ldloc awaiter)
    //     br completedBlock
    // }
    // Block completedBlock {
    //     call GetResult(ldloca awaiter)                     (possibly inlined into a containing expression)
    //     ...
    // }
    // =>
    // Block X { ...; br completedBlock }
    // Block completedBlock { <Await(value)>; ... }
    static bool detect_runtime_async_manual_await(Block* block, transforms::ILTransformContext& context);
};

}  // namespace cb::dotnet::decompiler::il::control_flow
