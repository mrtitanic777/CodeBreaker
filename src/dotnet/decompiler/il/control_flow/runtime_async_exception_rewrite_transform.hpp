// dotnet/decompiler/il/control_flow/runtime_async_exception_rewrite_transform.hpp -- reverses Roslyn's runtime-async lowering of
// try-catch and try-finally that contain an `await`, so the rest of the pipeline sees ordinary TryCatch and TryFinally instructions.
//
// try-catch with await is lowered to a flag-flat shape where the catch handler stores a captured object plus a "did the catch fire" int
// flag, and the original catch body runs after the protected region inside an `if (flag == 1)` guard. try-finally with await is lowered
// to a try-catch[object] whose handler stores the exception, followed by the original finally body inlined, followed by an
// `if (obj != null) ExceptionDispatchInfo.Capture((obj as Exception) ?? throw obj).Throw()` rethrow idiom.
//
//     RuntimeAsyncExceptionRewriteTransform::run(function, context);
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.ControlFlow: RuntimeAsyncExceptionRewriteTransform, from
// ControlFlow/RuntimeAsyncExceptionRewriteTransform.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in
// dotnet/decompiler).
#pragma once

#include <functional>
#include <unordered_map>
#include <vector>

#include "dotnet/decompiler/il/instructions.hpp"
#include "dotnet/decompiler/il/transforms/il_transform.hpp"

namespace cb::dotnet::decompiler::il::control_flow {

class RuntimeAsyncExceptionRewriteTransform {
public:
    static void run(ILFunction* function, transforms::ILTransformContext& context);

private:
    using FilterObjByHandler = std::unordered_map<TryCatchHandler*, ILVariable*>;

    // Strip the runtime-async obj-store machinery from the entry of the filter's trueBody. After this runs, the filter has the same shape
    // as a state-machine-async catch-when filter, and DetectCatchWhenConditionBlocks (the next EarlyILTransform) handles isinst
    // recognition, retyping handler.Variable to the user's catch type, and propagating it through trivial stloc copies inside the handler.
    // We only need to peel off the runtime-async-specific captured-object prefix.
    //
    // Shape we strip from trueBody:
    //   [stloc tmp2(ldloc tmpVar);]                  optional copy
    //   stloc obj(ldloc tmp_or_tmp2)
    //   [stloc typedEx(castclass T'(ldloc obj));]    optional, when source has a typed catch var
    // where tmpVar is the isinst-result local set in the filter's entry block. The captured `obj` local is shared across every catch
    // handler in the multi-handler form, so it must NOT be remapped function-wide here; we record it per handler and let the body
    // rewriter scope the remap to each handler's catch body.
    static bool normalize_runtime_async_filter(TryCatchHandler* handler, ILFunction* function, FilterObjByHandler& filter_obj_by_handler,
                                               transforms::ILTransformContext& context);
    static void remap_variable_reads(ILInstruction* root, ILVariable* from, ILVariable* to);

    // stloc obj(ldnull)
    // .try { ... ; br continuation }
    // catch ex : object when (ldc.i4 1) {
    //     [stloc tmp(ldloc ex);]
    //     stloc obj(ldloc tmp_or_ex)
    //     br continuation
    // }
    // Block continuation {
    //     <finally body>
    //     if (obj == null) leave outer
    //     <isinst-Exception + conditional throw via ExceptionDispatchInfo>
    // }
    // =>
    // .try { ... ; leave finallyContainer }
    // finally { <finally body> }
    // Block continuation {
    //     leave outer
    // }
    static bool try_rewrite_try_finally(TryCatch* try_catch, TryCatchHandler* handler, Block* handler_block, Block* parent_block, BlockContainer* container,
                                        transforms::ILTransformContext& context);
    // Scan instructions before `tryCatch` for the runtime-async flag-init store. The lowering inserts an `stloc obj(ldnull)` (try-finally)
    // or `stloc num(ldc.i4 0)` (try-catch) before the try region; the catch handler overwrites it, and the continuation reads it to decide
    // whether an exception occurred (and which case). Match by `referenceVar`'s slot/kind/type rather than identity — after SplitVariables,
    // the pre-init's ILVariable may differ from the in-handler one. Returns nullptr when no matching store is found.
    static StLoc* find_flag_init_store(Block* parent_block, TryCatch* try_catch, ILVariable* reference_var,
                                       const std::function<bool(ILInstruction*)>& match_init_value);
    // True when `leave` exits `container` itself or any ancestor container — i.e. control transfers out of `container`.
    static bool leave_exits_container(Leave* leave, BlockContainer* container);
    // Remove `stloc v(ldc.i4 0)` instructions immediately preceding `tryFinally` whose target variable is never read.
    static void remove_dead_flag_stores(Block* parent_block, TryFinally* try_finally);
    // Locate the "if (obj == null) leave outer; br dispatchHead" finally-exit block, plus all dispatch blocks.
    static bool find_finally_dispatch_exit(Block* start, ILVariable* object_variable, BlockContainer* container, Block** finally_exit_block,
                                           std::vector<Block*>* dispatch_blocks, ILInstruction** after_finally_exit);
    // Match a tail of the form
    //     if (comp.o(ldloc obj == ldnull)) <leaveOuter | br anyBlock>
    //     br dispatchHead
    // (trailing two instructions of the block; instructions before are part of the finally body). The if-true branch may go directly to
    // a function leave, to a one-instruction leave block, or to any other block in the outer container (e.g. an early-return flag check
    // from a nested catch).
    static bool match_finally_exit_block(Block* block, ILVariable* object_variable, Block** dispatch_head, ILInstruction** after_finally_exit);
    // Block dispatchHead {
    //     stloc tmp(isinst Exception(ldloc obj))
    //     if (comp.o(ldloc tmp != ldnull)) br captureBlock
    //     br throwBlock
    // }
    // Block captureBlock { callvirt Throw(call Capture(ldloc tmp)); leave outer }
    // Block throwBlock { throw(ldloc obj) }
    static bool collect_dispatch_blocks(Block* dispatch_head, ILVariable* object_variable, std::vector<Block*>* dispatch_blocks);
    static void rewrite_finally_exit(Block* finally_exit_block, BlockContainer* finally_container);
    static std::vector<Block*> collect_reachable(Block* entry, const std::vector<Block*>& exclude);

    // stloc num(0)
    // .try { ... ; br continuation }
    // catch ex : T when (ldc.i4 1) {
    //     [stloc tmp(ldloc ex);]
    //     [stloc obj(ldloc tmp_or_ex);]
    //     stloc num(1)
    //     br continuation
    // }
    // Block continuation {
    //     if (comp.i4(num != 1)) leave outer ; or "if (num == 1) br catchBody; leave outer"
    //     br catchBody
    // }
    // Block catchBody { ... }
    // =>
    // .try { ... ; br continuation }
    // catch ex : T { ...catchBody, with reads of obj as ex... }
    // Block continuation {
    //     leave outer
    // }
    static bool try_rewrite_try_catch(TryCatch* try_catch, TryCatchHandler* handler, Block* handler_block, Block* parent_block, BlockContainer* container,
                                      FilterObjByHandler& filter_obj_by_handler, transforms::ILTransformContext& context);
    // Multi-handler runtime-async try-catch:
    //
    //   stloc num(0)
    //   .try { ... ; br continuation }
    //   catch ex_1 : T_1 when (...) { [stloc tmp1(ex_1);] [stloc obj(...);] stloc num(K_1); br continuation }
    //   catch ex_2 : T_2 when (...) { [stloc tmp2(ex_2);] [stloc obj(...);] stloc num(K_2); br continuation }
    //   ...
    // Block continuation {
    //   switch (ldloc num) { case [K_i..K_i+1): br case_K_i ... ; default: leave outer }
    // }
    // Block case_K_i { <user catch body for handler i> }
    //
    // =>
    //   .try { ... ; br continuation }
    //   catch ex_1 : T_1 when (...) { ...case body 1 inlined, with obj/tmp reads remapped to ex_1... }
    //   catch ex_2 : T_2 when (...) { ...case body 2 inlined, with obj/tmp reads remapped to ex_2... }
    //   ...
    // Block continuation {
    //   leave outer
    // }
    static bool try_rewrite_multi_handler_try_catch(TryCatch* try_catch, Block* parent_block, BlockContainer* container,
                                                    FilterObjByHandler& filter_obj_by_handler, transforms::ILTransformContext& context);
    // For 2-handler multi-catches, Roslyn emits an if-chain rather than a switch:
    //   Block continuation {
    //     if (ldloc num == K_1) br case_K_1
    //     br nextBlock
    //   }
    //   Block nextBlock {
    //     if (ldloc num == K_2) br case_K_2
    //     <leave outer | br defaultExitBlock>
    //   }
    // Where the chain may extend beyond two if-blocks.
    static bool match_if_chain_dispatch(Block* continuation, ILVariable* num_variable, BlockContainer* container, std::unordered_map<int, Block*>* case_blocks,
                                        ILInstruction** default_exit);
    // Block continuation { switch (ldloc num) { case [K..K+1): br case_K ... ; default: <leave outer | br end> } }
    static bool match_switch_dispatch(Block* continuation, ILVariable* num_variable, std::unordered_map<int, Block*>* case_blocks, ILInstruction** default_exit);
    // Block continuation {
    //   Variant A: if (comp.i4(num != 1)) <exit>; br catchBody
    //   Variant B: if (comp.i4(num == 1)) br catchBody; <exit>
    // }
    // <exit> is either a direct Leave that exits `container` (or any ancestor) — which is the top-level shape where "no exception" leaves
    // the function — or a Branch to a leave-block in `container`. The Branch form arises when the try-catch is nested (e.g. inside an
    // outer try-finally), where "no exception" branches to the outer try-block's exit point.
    static bool match_catch_entry_check(Block* continuation, ILVariable* num_variable, BlockContainer* container, Block** catch_body_entry,
                                        ILInstruction** after_catch_exit);
    // True when `inst` transfers control out of `container`. Accepts three shapes that all arise from runtime-async lowering:
    //  - direct `Leave` to `container` or any ancestor;
    //  - cross-container `Branch` whose target lives in a strict ancestor of `container` (the inner-try-catch-inside-outer-try-finally
    //    case, where Roslyn emits a single branch that spans the inner container);
    //  - `Branch` to a one-instruction leave-block in `container` (the indirected canonical leave-via-helper-block form).
    static bool is_container_exit(ILInstruction* inst, BlockContainer* container);
    static void replace_variable_reads_with_handler_variable(ILInstruction* root, ILVariable* from, ILVariable* to);

    // Recognize the runtime-async lowering of an early return that crosses a try-finally. Roslyn rewrites `return value;` inside a
    // try-block as:
    //     stloc capture(value)
    //     stloc flag(K)
    //     leave-try   (i.e. let the finally run, then exit the try-finally)
    // followed by post-try logic of the form:
    //     if (flag == K) leave outer (capture)
    //
    // Detect that pattern around a TryFinally we just produced and rewrite each capture-set-flag-and-leave site into a direct
    // `leave outer (value)`, then drop the flag/post-flag-check machinery. The leave still passes through the TryFinally so the user's
    // finally body runs before the function returns, which is the intended source-level semantics of `return` from inside a try-finally.
    static bool try_rewrite_flag_based_early_return(TryFinally* try_finally, transforms::ILTransformContext& context);
    // True when `candidate` is the same slot/kind/type as `flagVar` — handles SplitVariables splitting the pre-init off from the in-try set.
    static bool flag_variable_matches(ILVariable* candidate, ILVariable* flag_var);
    // Build a template of instructions to splice into each flag-setter when the early-action is a Branch to a helper block. Follows a chain
    // of "stores + br to next block" until reaching a block whose last instruction is a Leave to `container` or an ancestor. The final
    // template is the concatenation of all the stores encountered, followed by the terminating Leave.
    static bool try_get_early_action_template(Block* early_block, BlockContainer* container, std::vector<ILInstruction*>* template_);
    // Resolve `earlyBlock` to (captureVar?, leaveTargetContainer). Used by the simpler shapes; callers that need to handle the
    // multi-instruction nested-forwarding form use `TryGetEarlyActionTemplate` instead.
    static bool resolve_early_return_value(Block* early_block, BlockContainer* container, ILVariable** capture_var, BlockContainer** leave_target_container);
    // True when `leave` exits the immediate `container` or any of its ancestor BlockContainers. Self-leave (TargetContainer == container)
    // is required for the top-level return case where the early-return leaves the function body container itself.
    static bool is_leave_to_container_or_ancestor(Leave* leave, BlockContainer* container);
    static void replace_dispatch_idiom_with_rethrow(Block* block, ILVariable* handler_variable, transforms::ILTransformContext& context);
};

}  // namespace cb::dotnet::decompiler::il::control_flow
