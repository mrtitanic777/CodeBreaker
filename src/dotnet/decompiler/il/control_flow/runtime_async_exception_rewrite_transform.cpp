// dotnet/decompiler/il/control_flow/runtime_async_exception_rewrite_transform.cpp -- see runtime_async_exception_rewrite_transform.hpp.
#include "dotnet/decompiler/il/control_flow/runtime_async_exception_rewrite_transform.hpp"

#include <algorithm>
#include <cassert>
#include <deque>
#include <unordered_set>

#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/il/control_flow/await_in_catch_transform.hpp"
#include "dotnet/decompiler/il/control_flow/await_in_finally_transform.hpp"
#include "dotnet/decompiler/il/control_flow/control_flow_graph.hpp"
#include "dotnet/decompiler/il/il_instruction_extensions.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"

namespace cb::dotnet::decompiler::il::control_flow {

using transforms::ILTransformContext;
using typesystem::KnownTypeCode;

void RuntimeAsyncExceptionRewriteTransform::run(ILFunction* function, ILTransformContext& context) {
    if (!context.settings().await_in_catch_finally) return;

    bool changed = false;

    // Pre-pass: normalize runtime-async catch filters that wrap a type-test + obj-store.
    // `catch (T ex) when (filter)` is lowered to `catch object when ({ isinst T; obj=ex; <filter> })`,
    // even when T is `object` (i.e. the source is `catch when (filter)`). After this pre-pass the
    // catch handler looks structurally identical to a non-filter catch, so the body matcher in
    // TryRewriteTryCatch can run unchanged.
    //
    // The captured obj local is shared across every catch handler that opts into the runtime-async
    // rethrow protocol, so per-handler filter normalization must NOT do a function-wide remap of
    // reads of obj — that would tag every reference (in both this handler's own dispatch idiom and
    // every other handler's dispatch idiom) with the same handler variable. Instead, record the
    // obj variable per handler and let TryRewriteTryCatch / the multi-handler transform remap it
    // scoped to the moved catch body.
    FilterObjByHandler filter_obj_by_handler;
    std::vector<TryCatchHandler*> handlers;
    for (ILInstruction* d : function->descendants())
        if (auto* handler = as<TryCatchHandler>(d)) handlers.push_back(handler);
    for (TryCatchHandler* handler : handlers) {
        if (normalize_runtime_async_filter(handler, function, filter_obj_by_handler, context)) changed = true;
    }

    std::vector<TryCatch*> try_catches;
    for (ILInstruction* d : function->descendants())
        if (auto* try_catch = as<TryCatch>(d)) try_catches.push_back(try_catch);
    for (TryCatch* try_catch : try_catches) {
        auto* parent_block = as<Block>(try_catch->parent());
        if (parent_block == nullptr) continue;
        auto* container = as<BlockContainer>(parent_block->parent());
        if (container == nullptr) continue;

        if (try_catch->handlers.count() == 1) {
            TryCatchHandler* handler = try_catch->handlers[0];
            auto* handler_body = as<BlockContainer>(handler->body());
            if (handler_body == nullptr) continue;
            if (handler_body->blocks.count() != 1) continue;
            Block* handler_block = handler_body->blocks[0];

            if (try_rewrite_try_finally(try_catch, handler, handler_block, parent_block, container, context)) {
                changed = true;
                continue;
            }

            if (try_rewrite_try_catch(try_catch, handler, handler_block, parent_block, container, filter_obj_by_handler, context)) {
                changed = true;
                continue;
            }
        } else if (try_catch->handlers.count() >= 2) {
            if (try_rewrite_multi_handler_try_catch(try_catch, parent_block, container, filter_obj_by_handler, context)) {
                changed = true;
                continue;
            }
        }
    }

    // Recognize the "early return from inside a try via a flag local" pattern that runtime-async
    // emits whenever a return statement crosses an enclosing try-finally with await.
    std::vector<TryFinally*> try_finallys;
    for (ILInstruction* d : function->descendants())
        if (auto* try_finally = as<TryFinally>(d)) try_finallys.push_back(try_finally);
    for (TryFinally* try_finally : try_finallys) {
        if (try_rewrite_flag_based_early_return(try_finally, context)) changed = true;
    }

    if (changed) {
        for (ILInstruction* d : function->body()->descendants()) {
            if (auto* c = as<BlockContainer>(d)) c->sort_blocks(true);
        }
    }
}

bool RuntimeAsyncExceptionRewriteTransform::normalize_runtime_async_filter(TryCatchHandler* handler, ILFunction* function,
                                                                           FilterObjByHandler& filter_obj_by_handler, ILTransformContext& context) {
    if (!typesystem::is_known_type(handler->variable()->type(), KnownTypeCode::Object)) return false;
    auto* filter_container = as<BlockContainer>(handler->filter());
    if (filter_container == nullptr) return false;

    // Find the isinst-result temp via the standard catch-when entry shape:
    //   stloc tmp(isinst T(ldloc handlerVar)); if (tmp != null) br trueBody; br falseBody
    Block* entry = filter_container->entry_point();
    if (entry->instructions.count() != 3) return false;
    ILVariable* tmp_var = nullptr;
    ILInstruction* tmp_value = nullptr;
    if (!entry->instructions[0]->match_st_loc(&tmp_var, &tmp_value)) return false;
    ILInstruction* isinst_arg = nullptr;
    const IType* unused_type = nullptr;
    if (!tmp_value->match_is_inst(&isinst_arg, &unused_type)) return false;
    if (!isinst_arg->match_ld_loc(handler->variable())) return false;
    auto* if_inst = as<IfInstruction>(entry->instructions[1]);
    if (if_inst == nullptr) return false;
    ILInstruction* cond_arg = nullptr;
    if (!if_inst->condition()->match_comp_not_equals_null(&cond_arg) || !cond_arg->match_ld_loc(tmp_var)) return false;
    auto* true_branch = as<Branch>(if_inst->true_inst());
    if (true_branch == nullptr) return false;
    Block* true_body = true_branch->target_block();
    int n = true_body->instructions.count();
    if (n < 1) return false;

    // Recognize the prefix.
    int prefix = 0;
    ILVariable* tmp2_var = nullptr;
    ILVariable* obj_var = nullptr;
    ILVariable* first = nullptr;
    ILInstruction* first_value = nullptr;
    if (!true_body->instructions[0]->match_st_loc(&first, &first_value)) return false;
    if (!first_value->match_ld_loc(tmp_var)) return false;
    ILVariable* second = nullptr;
    ILInstruction* second_value = nullptr;
    if (n > 1 && true_body->instructions[1]->match_st_loc(&second, &second_value) && second_value->match_ld_loc(first)) {
        tmp2_var = first;
        obj_var = second;
        prefix = 2;
    } else {
        obj_var = first;
        prefix = 1;
    }

    ILVariable* typed_ex_var = nullptr;
    if (prefix < n) {
        auto* cast_store = as<StLoc>(true_body->instructions[prefix]);
        auto* cast_class = cast_store != nullptr ? as<CastClass>(cast_store->value()) : nullptr;
        if (cast_class != nullptr && cast_class->argument()->match_ld_loc(obj_var)) {
            typed_ex_var = cast_store->variable();
            prefix++;
        }
    }

    context.step_start_group("Strip runtime-async catch-filter prefix", handler);

    true_body->instructions.remove_range(0, prefix);

    // Remap reads of the per-handler synthesized variables (tmp2 / typedEx) to handler.Variable.
    // Both are unique per handler so a function-wide remap is safe.
    if (tmp2_var != nullptr) remap_variable_reads(function, tmp2_var, handler->variable());
    if (typed_ex_var != nullptr) remap_variable_reads(function, typed_ex_var, handler->variable());

    // Optimized builds inline `castclass T(ldloc obj)` directly into the user filter expression
    // instead of stashing it in a typedEx local. Remap obj reads within the filter only.
    remap_variable_reads(handler->filter(), obj_var, handler->variable());

    // The obj variable is shared between handlers in the multi-handler form; the body rewriter
    // remaps it scoped per handler.
    filter_obj_by_handler[handler] = obj_var;

    context.step_end_group(true);
    return true;
}

void RuntimeAsyncExceptionRewriteTransform::remap_variable_reads(ILInstruction* root, ILVariable* from, ILVariable* to) {
    std::vector<LdLoc*> ldlocs;
    for (ILInstruction* d : root->descendants())
        if (auto* ldloc = as<LdLoc>(d)) ldlocs.push_back(ldloc);
    for (LdLoc* ldloc : ldlocs) {
        if (ldloc->variable() != from) continue;
        // Drop redundant castclass to the new handler type that may now wrap the load.
        if (auto* cc = as<CastClass>(ldloc->parent()); cc != nullptr && typesystem::type_equals(cc->type(), to->type()))
            cc->replace_with(with_il_range(with_il_range(make<LdLoc>(to), cc), ldloc));
        else
            ldloc->replace_with(with_il_range(make<LdLoc>(to), ldloc));
    }
    std::vector<StLoc*> stlocs;
    for (ILInstruction* d : root->descendants())
        if (auto* stloc = as<StLoc>(d)) stlocs.push_back(stloc);
    for (StLoc* stloc : stlocs) {
        auto* parent = as<Block>(stloc->parent());
        if (stloc->variable() == from && parent != nullptr) parent->instructions.remove_at(stloc->child_index());
    }
}

bool RuntimeAsyncExceptionRewriteTransform::try_rewrite_try_finally(TryCatch* try_catch, TryCatchHandler* handler, Block* /*handler_block*/, Block* parent_block,
                                                                    BlockContainer* container, ILTransformContext& context) {
    if (!typesystem::is_known_type(handler->variable()->type(), KnownTypeCode::Object)) return false;

    // match catch body: [stloc tmp(ldloc ex);] stloc obj(ldloc tmp_or_ex); br continuation
    // Reuse the helper from AwaitInFinallyTransform — same shape on both lowerings.
    ILVariable* object_variable = nullptr;
    Block* continuation = nullptr;
    if (!AwaitInFinallyTransform::match_object_store_catch_handler(static_cast<BlockContainer*>(handler->body()), handler->variable(), &object_variable,
                                                                   &continuation)) {
        return false;
    }
    if (!typesystem::is_known_type(object_variable->type(), KnownTypeCode::Object)) return false;
    if (continuation->parent() != container) return false;

    // Pre-try: somewhere among the instructions preceding the TryCatch we expect
    // an `stloc obj(ldnull)`. Other (unrelated) stores may be interleaved.
    StLoc* flag_init_store = find_flag_init_store(parent_block, try_catch, object_variable, [](ILInstruction* v) { return v->match_ld_null(); });
    if (flag_init_store == nullptr) return false;

    // Every outward exit of the try body must branch to `continuation`. The runtime-async
    // lowering rewrites every return / fallthrough to `br continuation` and routes throws
    // through the synthetic catch, so a Leave or a Branch to anything else means we're not
    // looking at a lowered shape. A try body with no outward exit at all is also fine —
    // the user wrote `try { throw ...; } finally { await ... }`, which lowers to a try body
    // whose only exit is the throw (handled by the synthetic catch).
    for (ILInstruction* d : try_catch->try_block()->descendants()) {
        auto* inst = as<IBranchOrLeaveInstruction>(d);
        if (inst == nullptr) continue;
        // Skip intra-tryBody control flow: inst.TargetContainer being tryBody itself or any
        // container nested inside tryBody means control stays within tryBody. Only when the
        // target container is a strict ancestor of tryBody does the instruction exit it.
        if (inst->target_container()->is_descendant_of(try_catch->try_block())) continue;
        if (auto* branch = as<Branch>(d); branch != nullptr && branch->target_block() == continuation) continue;
        return false;
    }

    // Find the dispatch idiom at the end of the finally body.
    // Pattern: a block ending with "if (obj == null) leave outer; br dispatchHead"
    // where dispatchHead is the block containing the isinst Exception + ExceptionDispatchInfo idiom.
    Block* finally_exit_block = nullptr;
    std::vector<Block*> dispatch_blocks;
    ILInstruction* after_finally_exit = nullptr;
    if (!find_finally_dispatch_exit(continuation, object_variable, container, &finally_exit_block, &dispatch_blocks, &after_finally_exit)) return false;

    context.step_start_group("Rewrite runtime-async try-finally", try_catch);

    // Determine whether the if-true branch of the finally exit is a direct Leave-with-value
    // or a Branch to a one-instruction leave block. In the latter case, that block stays
    // in the outer container so it follows the new TryFinally and provides the return value.
    Block* leave_block = nullptr;
    if (auto* br_to_leave = as<Branch>(after_finally_exit)) leave_block = br_to_leave->target_block();

    // Build the CFG once, before any structural changes. The dominator analysis below
    // uses this snapshot to identify which blocks belong to the finally body.
    ControlFlowGraph cfg(container, context.cancellation_token);

    // Build a new BlockContainer for the finally body.
    BlockContainer* finally_container = with_il_range(make<BlockContainer>(), handler->body());

    // Replace the TryCatch with a TryFinally now so that the move below places blocks into
    // the freshly attached finallyContainer (which needs a parent to satisfy invariants).
    auto* try_block_container = static_cast<BlockContainer*>(try_catch->try_block());
    TryFinally* try_finally = with_il_range(make<TryFinally>(try_block_container, finally_container), try_catch);
    try_catch->replace_with(try_finally);

    // Move the finally body — all blocks dominated by `continuation`, stopping at
    // `finallyExitBlock` (so dispatchHead, captureBlock, throwBlock, and the leave block
    // stay in the outer container).
    AwaitInFinallyTransform::move_dominated_blocks_to_container(continuation, finally_exit_block, cfg, finally_container, context);

    // Strip the trailing dispatch-check (last 2 instructions: if + br dispatchHead) from
    // the finally exit block, and end with `leave finallyContainer`.
    rewrite_finally_exit(finally_exit_block, finally_container);

    // Redirect try-block branches that targeted `continuation` to `Leave(tryBlockContainer)`
    // so normal try-block completion runs the finally, then resumes after the TryFinally.
    std::vector<Branch*> branches;
    for (ILInstruction* d : try_block_container->descendants())
        if (auto* br = as<Branch>(d)) branches.push_back(br);
    for (Branch* br : branches) {
        if (br->target_block() == continuation) br->replace_with(with_il_range(make<Leave>(try_block_container), br));
    }

    // Append a successor instruction so the parent block remains EndPointUnreachable.
    // If there was a separate leave-block, branch to it (it stays in the outer container);
    // otherwise reuse the original Leave-with-value — RewriteFinallyExit already detached
    // it from the if-instruction that previously held it. Skip when the try body always
    // throws — the resulting TryFinally's endpoint is unreachable and a successor
    // instruction after it would put a non-final unreachable-endpoint instruction in the
    // block, violating the block invariant.
    if (!try_finally->has_flag(InstructionFlags::EndPointUnreachable)) {
        if (leave_block != nullptr)
            parent_block->instructions.add(with_il_range(make<Branch>(leave_block), after_finally_exit));
        else
            parent_block->instructions.add(after_finally_exit);
    }

    // Remove the pre-init `stloc obj(ldnull)`. Also remove any dead
    // `stloc <int>(ldc.i4 0)` immediately preceding the TryFinally — the runtime-async
    // lowering pre-allocates a flag local even for try-finally where it's never read,
    // and leaving it between the resource store and the TryFinally would block
    // UsingTransform from recognizing the using/await foreach pattern.
    parent_block->instructions.remove_at(flag_init_store->child_index());
    remove_dead_flag_stores(parent_block, try_finally);

    // Dispatch blocks are now unreachable; SortBlocks at the end of Run will drop them.
    context.step_end_group(true);
    return true;
}

StLoc* RuntimeAsyncExceptionRewriteTransform::find_flag_init_store(Block* parent_block, TryCatch* try_catch, ILVariable* reference_var,
                                                                   const std::function<bool(ILInstruction*)>& match_init_value) {
    int try_index = try_catch->child_index();
    for (int i = 0; i < try_index; i++) {
        auto* s = as<StLoc>(parent_block->instructions[i]);
        if (s == nullptr) continue;
        if (s->variable()->index() != reference_var->index() || s->variable()->kind() != reference_var->kind()) continue;
        if (!typesystem::type_equals(s->variable()->type(), reference_var->type())) continue;
        if (!match_init_value(s->value())) continue;
        return s;
    }
    return nullptr;
}

bool RuntimeAsyncExceptionRewriteTransform::leave_exits_container(Leave* leave, BlockContainer* container) {
    if (leave->target_container() == container) return true;
    return container->is_descendant_of(leave->target_container());
}

void RuntimeAsyncExceptionRewriteTransform::remove_dead_flag_stores(Block* parent_block, TryFinally* try_finally) {
    while (try_finally->child_index() > 0) {
        auto* dead_store = as<StLoc>(parent_block->instructions[try_finally->child_index() - 1]);
        if (!(dead_store != nullptr && dead_store->value()->match_ldc_i4(0) && dead_store->variable()->load_count() == 0)) break;
        parent_block->instructions.remove_at(dead_store->child_index());
    }
}

bool RuntimeAsyncExceptionRewriteTransform::find_finally_dispatch_exit(Block* start, ILVariable* object_variable, BlockContainer* container,
                                                                       Block** finally_exit_block, std::vector<Block*>* dispatch_blocks,
                                                                       ILInstruction** after_finally_exit) {
    *finally_exit_block = nullptr;
    dispatch_blocks->clear();
    *after_finally_exit = nullptr;

    // Walk reachable blocks until we find a block whose body matches the finally-exit shape.
    std::unordered_set<Block*> visited;
    std::deque<Block*> queue;
    queue.push_back(start);
    while (!queue.empty()) {
        Block* b = queue.front();
        queue.pop_front();
        if (!visited.insert(b).second) continue;
        Block* dispatch_head = nullptr;
        if (match_finally_exit_block(b, object_variable, &dispatch_head, after_finally_exit)) {
            *finally_exit_block = b;
            if (collect_dispatch_blocks(dispatch_head, object_variable, dispatch_blocks)) return true;
            return false;
        }
        for (ILInstruction* d : b->descendants()) {
            auto* br = as<Branch>(d);
            if (br != nullptr && br->target_block() != nullptr && br->target_block()->parent() == container) queue.push_back(br->target_block());
        }
    }
    return false;
}

bool RuntimeAsyncExceptionRewriteTransform::match_finally_exit_block(Block* block, ILVariable* object_variable, Block** dispatch_head,
                                                                     ILInstruction** after_finally_exit) {
    *dispatch_head = nullptr;
    *after_finally_exit = nullptr;

    int count = block->instructions.count();
    if (count < 2) return false;
    auto* if_inst = as<IfInstruction>(block->instructions[count - 2]);
    if (if_inst == nullptr) return false;
    ILInstruction* arg = nullptr;
    if (!if_inst->condition()->match_comp_equals_null(&arg) || !arg->match_ld_loc(object_variable)) return false;
    *after_finally_exit = if_inst->true_inst();
    if (auto* leave_outer = as<Leave>(*after_finally_exit); leave_outer != nullptr && leave_outer->is_leaving_function()) {
        // direct leave OK
    } else if (auto* br_target = as<Branch>(*after_finally_exit)) {
        // Branch to another block — could be a single-instruction leave block (canonical) or
        // a non-trivial successor (e.g. an early-return check from a nested catch).
        Block* target = br_target->target_block();
        auto* leave_outer2 = target->instructions.count() == 1 ? as<Leave>(target->instructions[0]) : nullptr;
        if (leave_outer2 != nullptr && leave_outer2->is_leaving_function()) {
            *after_finally_exit = leave_outer2;
        }
        // otherwise: keep the Branch as afterFinallyExit so the post-TryFinally successor is wired up
    } else {
        return false;
    }
    if (!block->instructions[count - 1]->match_branch(dispatch_head)) return false;
    return true;
}

bool RuntimeAsyncExceptionRewriteTransform::collect_dispatch_blocks(Block* dispatch_head, ILVariable* object_variable, std::vector<Block*>* dispatch_blocks) {
    dispatch_blocks->clear();
    if (dispatch_head->instructions.count() != 3) return false;
    ILVariable* typed_ex_var = nullptr;
    ILInstruction* typed_ex_value = nullptr;
    if (!dispatch_head->instructions[0]->match_st_loc(&typed_ex_var, &typed_ex_value)) return false;
    ILInstruction* is_inst_arg = nullptr;
    const IType* unused_type = nullptr;
    if (!typed_ex_value->match_is_inst(&is_inst_arg, &unused_type)) return false;
    if (!is_inst_arg->match_ld_loc(object_variable)) return false;
    auto* if_inst = as<IfInstruction>(dispatch_head->instructions[1]);
    if (if_inst == nullptr) return false;
    ILInstruction* not_null_arg = nullptr;
    if (!if_inst->condition()->match_comp_not_equals_null(&not_null_arg) || !not_null_arg->match_ld_loc(typed_ex_var)) return false;
    auto* to_capture = as<Branch>(if_inst->true_inst());
    if (to_capture == nullptr) return false;
    auto* to_throw = as<Branch>(dispatch_head->instructions[2]);
    if (to_throw == nullptr) return false;

    *dispatch_blocks = {dispatch_head, to_capture->target_block(), to_throw->target_block()};
    return true;
}

void RuntimeAsyncExceptionRewriteTransform::rewrite_finally_exit(Block* finally_exit_block, BlockContainer* finally_container) {
    // Strip the trailing 2 instructions (if + br dispatchHead) and append `leave finallyContainer`.
    // The new Leave occupies the same end-of-finally position the removed dispatch check sat at,
    // so inherit the IL range from both to keep source mapping aligned.
    int count = finally_exit_block->instructions.count();
    ILInstruction* if_inst = finally_exit_block->instructions[count - 2];
    ILInstruction* br_inst = finally_exit_block->instructions[count - 1];
    finally_exit_block->instructions.remove_range(count - 2, 2);
    Leave* leave = with_il_range(make<Leave>(finally_container), if_inst);
    leave->add_il_range(*br_inst);
    finally_exit_block->instructions.add(leave);
}

std::vector<Block*> RuntimeAsyncExceptionRewriteTransform::collect_reachable(Block* entry, const std::vector<Block*>& exclude) {
    std::unordered_set<Block*> exclude_set(exclude.begin(), exclude.end());
    std::unordered_set<Block*> visited;
    std::vector<Block*> result;
    std::deque<Block*> queue;
    queue.push_back(entry);
    while (!queue.empty()) {
        Block* b = queue.front();
        queue.pop_front();
        if (!visited.insert(b).second) continue;
        if (exclude_set.contains(b)) continue;
        if (b->parent() != entry->parent()) continue;
        result.push_back(b);
        for (ILInstruction* d : b->descendants()) {
            auto* br = as<Branch>(d);
            if (br != nullptr && br->target_block() != nullptr && br->target_block()->parent() == entry->parent()) queue.push_back(br->target_block());
        }
    }
    return result;
}

bool RuntimeAsyncExceptionRewriteTransform::try_rewrite_try_catch(TryCatch* try_catch, TryCatchHandler* handler, Block* handler_block, Block* parent_block,
                                                                  BlockContainer* container, FilterObjByHandler& filter_obj_by_handler,
                                                                  ILTransformContext& context) {
    // Match catch body: [stloc tmp(ldloc ex);] [stloc obj(ldloc tmp);] stloc num(1); br continuation
    if (handler_block->instructions.count() < 2) return false;
    Block* continuation = nullptr;
    if (!handler_block->instructions.last()->match_branch(&continuation)) return false;
    if (continuation->parent() != container) return false;

    ILVariable* num_variable = nullptr;
    ILInstruction* num_store = handler_block->instructions[handler_block->instructions.count() - 2];
    ILInstruction* num_value = nullptr;
    if (!num_store->match_st_loc(&num_variable, &num_value)) return false;
    if (!num_value->match_ldc_i4(1)) return false;
    if (!typesystem::is_known_type(num_variable->type(), KnownTypeCode::Int32)) return false;

    // Collect optional tmp/obj stores (everything before the num=1 store)
    ILVariable* object_variable = nullptr;
    ILVariable* temporary_variable = nullptr;
    int prefix_count = handler_block->instructions.count() - 2;
    if (prefix_count >= 1) {
        // Last prefix instruction may be: stloc obj(ldloc tmp_or_ex)
        ILVariable* v = nullptr;
        ILInstruction* val = nullptr;
        if (handler_block->instructions[prefix_count - 1]->match_st_loc(&v, &val)) {
            ILVariable* tmp_v = nullptr;
            ILVariable* tmp_v2 = nullptr;
            ILInstruction* tmp_val = nullptr;
            if (val->match_ld_loc(handler->variable())) {
                // Direct stloc obj(ldloc ex) — no tmp variable
                object_variable = v;
                prefix_count--;
            } else if (val->match_ld_loc(&tmp_v) && prefix_count >= 2 && handler_block->instructions[prefix_count - 2]->match_st_loc(&tmp_v2, &tmp_val) &&
                       tmp_v == tmp_v2 && tmp_val->match_ld_loc(handler->variable())) {
                temporary_variable = tmp_v;
                object_variable = v;
                prefix_count -= 2;
            } else {
                return false;
            }
        }
    }
    if (prefix_count != 0) return false;

    // Pre-try: somewhere before the TryCatch we expect `stloc num(ldc.i4 0)`.
    StLoc* flag_init_store = find_flag_init_store(parent_block, try_catch, num_variable, [](ILInstruction* v) { return v->match_ldc_i4(0); });
    if (flag_init_store == nullptr) return false;

    // Continuation must contain a "num == 1" check that branches to the catch body, or
    // alternatively a "num != 1" check that leaves outer.
    Block* catch_body_entry = nullptr;
    ILInstruction* after_catch_exit = nullptr;
    if (!match_catch_entry_check(continuation, num_variable, container, &catch_body_entry, &after_catch_exit)) return false;

    context.step_start_group("Rewrite runtime-async try-catch", try_catch);

    // Move catch body blocks (those dominated by catchBodyEntry within `container`) into the handler.
    std::vector<Block*> body_blocks = collect_reachable(catch_body_entry, {});
    for (Block* b : body_blocks) {
        b->remove();
    }

    // Replace handler's existing block (which was just the prefix + num=1 + branch)
    // with the catch body. Preserve the original branch target redirection: branches that
    // targeted `continuation` from inside the body now target the new continuation
    // (the `leave outer` block which is `continuation` itself, after we strip its catch-entry-check).
    handler_block->instructions.clear();
    handler_block->instructions.add(make<Branch>(catch_body_entry));
    for (Block* b : body_blocks) static_cast<BlockContainer*>(handler->body())->blocks.add(b);

    // Replace reads of `obj` (and `tmp`) inside the moved catch body with reads of handler.Variable.
    // `objectVariable` here is the handler's body-level obj (only present for non-filter catches);
    // `filterObj` is the obj recorded by NormalizeRuntimeAsyncFilter when the catch carries a filter.
    if (object_variable != nullptr) replace_variable_reads_with_handler_variable(handler->body(), object_variable, handler->variable());
    if (temporary_variable != nullptr) replace_variable_reads_with_handler_variable(handler->body(), temporary_variable, handler->variable());
    if (auto it = filter_obj_by_handler.find(handler); it != filter_obj_by_handler.end())
        replace_variable_reads_with_handler_variable(handler->body(), it->second, handler->variable());

    // Inside the moved blocks, locate any leftover dispatch idiom (originating from `throw;`)
    // and replace it with `Rethrow`.
    std::vector<Block*> handler_blocks;
    for (ILInstruction* d : handler->body()->descendants())
        if (auto* b = as<Block>(d)) handler_blocks.push_back(b);
    for (Block* b : handler_blocks) replace_dispatch_idiom_with_rethrow(b, handler->variable(), context);

    // Strip the catch-entry check from `continuation` — replace with a raw `leave outer`.
    // Clear() already detached `afterCatchExit` (it was either continuation.Instructions[1]
    // or a child of the now-detached if-instruction), so we can re-add it directly.
    continuation->instructions.clear();
    continuation->instructions.add(after_catch_exit);

    // Remove the pre-try `stloc num(0)`.
    parent_block->instructions.remove_at(flag_init_store->child_index());

    context.step_end_group(true);
    return true;
}

bool RuntimeAsyncExceptionRewriteTransform::try_rewrite_multi_handler_try_catch(TryCatch* try_catch, Block* parent_block, BlockContainer* container,
                                                                                FilterObjByHandler& filter_obj_by_handler, ILTransformContext& context) {
    struct HandlerInfo {
        TryCatchHandler* handler;
        int k;
        ILVariable* body_obj;
        ILVariable* body_tmp;
    };
    Block* continuation = nullptr;
    ILVariable* num_variable = nullptr;
    std::unordered_set<int> seen_k;
    std::vector<HandlerInfo> handler_infos;

    for (TryCatchHandler* handler : try_catch->handlers) {
        auto* hb = as<BlockContainer>(handler->body());
        if (hb == nullptr || hb->blocks.count() != 1) return false;
        Block* hblock = hb->blocks[0];
        if (hblock->instructions.count() < 2) return false;
        Block* br = nullptr;
        if (!hblock->instructions.last()->match_branch(&br)) return false;
        if (continuation == nullptr)
            continuation = br;
        else if (continuation != br)
            return false;
        if (continuation->parent() != container) return false;

        ILVariable* nv = nullptr;
        ILInstruction* nval = nullptr;
        if (!hblock->instructions[hblock->instructions.count() - 2]->match_st_loc(&nv, &nval)) return false;
        int k = 0;
        if (!nval->match_ldc_i4(&k)) return false;
        if (!typesystem::is_known_type(nv->type(), KnownTypeCode::Int32)) return false;
        if (num_variable == nullptr)
            num_variable = nv;
        else if (num_variable != nv)
            return false;
        if (!seen_k.insert(k).second) return false;

        ILVariable* body_obj = nullptr;
        ILVariable* body_tmp = nullptr;
        int prefix = hblock->instructions.count() - 2;
        ILVariable* v = nullptr;
        ILInstruction* val = nullptr;
        if (prefix >= 1 && hblock->instructions[prefix - 1]->match_st_loc(&v, &val)) {
            ILVariable* tmp_v = nullptr;
            ILVariable* tmp_v2 = nullptr;
            ILInstruction* tmp_val = nullptr;
            if (val->match_ld_loc(handler->variable())) {
                body_obj = v;
                prefix--;
            } else if (val->match_ld_loc(&tmp_v) && prefix >= 2 && hblock->instructions[prefix - 2]->match_st_loc(&tmp_v2, &tmp_val) && tmp_v == tmp_v2 &&
                       tmp_val->match_ld_loc(handler->variable())) {
                body_tmp = tmp_v;
                body_obj = v;
                prefix -= 2;
            } else {
                return false;
            }
        }
        if (prefix != 0) return false;

        handler_infos.push_back(HandlerInfo{handler, k, body_obj, body_tmp});
    }

    // Pre-try: stloc num(ldc.i4 0).
    StLoc* flag_init_store = find_flag_init_store(parent_block, try_catch, num_variable, [](ILInstruction* v) { return v->match_ldc_i4(0); });
    if (flag_init_store == nullptr) return false;
    // Block continuation { switch (ldloc num) { case K: br case_K ... ; default: leave outer } }
    // — or, for a small number of handlers (typically 2) where Roslyn emits an if-chain
    // instead of a switch — a chain of `if (num == K_i) br case_K_i` blocks ending in a leave.
    std::unordered_map<int, Block*> case_blocks;
    ILInstruction* default_exit = nullptr;
    if (!match_switch_dispatch(continuation, num_variable, &case_blocks, &default_exit) &&
        !match_if_chain_dispatch(continuation, num_variable, container, &case_blocks, &default_exit))
        return false;
    // Every K we recorded must have a case in the switch.
    for (const HandlerInfo& info : handler_infos)
        if (!case_blocks.contains(info.k)) return false;

    context.step_start_group("Rewrite runtime-async multi-handler try-catch", try_catch);
    ControlFlowGraph cfg(container, context.cancellation_token);

    for (const HandlerInfo& info : handler_infos) {
        Block* case_body = case_blocks.at(info.k);
        TryCatchHandler* handler = info.handler;
        auto* handler_body = static_cast<BlockContainer*>(handler->body());
        Block* handler_entry = handler_body->blocks[0];

        // Move case-body and dominated blocks into the handler's body container.
        AwaitInFinallyTransform::move_dominated_blocks_to_container(case_body, nullptr, cfg, handler_body, context);

        // Replace the handler-entry block (still holds the prefix + num=K + branch) with a
        // single `br caseBody`, since caseBody is now at index >= 1 in handlerBody.
        handler_entry->instructions.clear();
        handler_entry->instructions.add(make<Branch>(case_body));

        // Remap the per-handler synthesized variables to the handler.Variable inside the moved body.
        if (info.body_obj != nullptr) replace_variable_reads_with_handler_variable(handler->body(), info.body_obj, handler->variable());
        if (info.body_tmp != nullptr) replace_variable_reads_with_handler_variable(handler->body(), info.body_tmp, handler->variable());
        if (auto it = filter_obj_by_handler.find(handler); it != filter_obj_by_handler.end())
            replace_variable_reads_with_handler_variable(handler->body(), it->second, handler->variable());

        std::vector<Block*> handler_blocks;
        for (ILInstruction* d : handler->body()->descendants())
            if (auto* b = as<Block>(d)) handler_blocks.push_back(b);
        for (Block* b : handler_blocks) replace_dispatch_idiom_with_rethrow(b, handler->variable(), context);
    }

    // Replace continuation with the default exit (leave outer). Clone the default-exit so
    // we don't worry about whose tree it currently belongs to (the switch instruction we're
    // tearing down, or a later block in an if-chain dispatch). Clear the clone's ILRange —
    // it now sits at a different location than the original, so reusing the source offset
    // would produce wrong sequence points.
    ILInstruction* default_exit_clone = default_exit->clone();
    default_exit_clone->set_il_range(Interval());
    continuation->instructions.clear();
    continuation->instructions.add(default_exit_clone);

    // Remove the pre-try `stloc num(0)`.
    parent_block->instructions.remove_at(flag_init_store->child_index());

    context.step_end_group(true);
    return true;
}

bool RuntimeAsyncExceptionRewriteTransform::match_if_chain_dispatch(Block* continuation, ILVariable* num_variable, BlockContainer* container,
                                                                    std::unordered_map<int, Block*>* case_blocks, ILInstruction** default_exit) {
    case_blocks->clear();
    *default_exit = nullptr;
    std::unordered_set<Block*> visited;
    Block* current = continuation;
    while (true) {
        if (!visited.insert(current).second) return false;
        if (current->instructions.count() != 2) return false;
        auto* if_inst = as<IfInstruction>(current->instructions[0]);
        if (if_inst == nullptr) return false;
        ILInstruction* lhs = nullptr;
        ILInstruction* rhs = nullptr;
        int k = 0;
        if (!if_inst->condition()->match_comp_equals(&lhs, &rhs) || !lhs->match_ld_loc(num_variable) || !rhs->match_ldc_i4(&k)) return false;
        Block* case_block = nullptr;
        if (!if_inst->true_inst()->match_branch(&case_block)) return false;
        if (case_blocks->contains(k)) return false;
        (*case_blocks)[k] = case_block;
        ILInstruction* fallthrough = current->instructions[1];
        if (auto* direct_leave = as<Leave>(fallthrough); direct_leave != nullptr && is_leave_to_container_or_ancestor(direct_leave, container)) {
            *default_exit = direct_leave;
            return !case_blocks->empty();
        }
        Block* next_block = nullptr;
        if (!fallthrough->match_branch(&next_block) || next_block->parent() != container) return false;
        // One-instruction leave block ends the chain — typical for "switch default" / "no case matched".
        auto* final_leave = next_block->instructions.count() == 1 ? as<Leave>(next_block->instructions[0]) : nullptr;
        if (final_leave != nullptr && is_leave_to_container_or_ancestor(final_leave, container)) {
            *default_exit = final_leave;
            return !case_blocks->empty();
        }
        current = next_block;
    }
}

bool RuntimeAsyncExceptionRewriteTransform::match_switch_dispatch(Block* continuation, ILVariable* num_variable, std::unordered_map<int, Block*>* case_blocks,
                                                                  ILInstruction** default_exit) {
    case_blocks->clear();
    *default_exit = nullptr;
    if (continuation->instructions.count() != 1) return false;
    auto* switch_inst = as<SwitchInstruction>(continuation->instructions[0]);
    if (switch_inst == nullptr) return false;
    if (!switch_inst->value()->match_ld_loc(num_variable)) return false;

    SwitchSection* default_section = switch_inst->get_default_section();
    if (default_section == nullptr) return false;
    if (auto* def_leave = as<Leave>(default_section->body()); def_leave != nullptr && def_leave->is_leaving_function())
        *default_exit = def_leave;
    else if (is<Branch>(default_section->body()))
        *default_exit = default_section->body();
    else
        return false;

    for (SwitchSection* section : switch_inst->sections) {
        if (section == default_section) continue;
        Block* case_block = nullptr;
        if (!section->body()->match_branch(&case_block)) return false;
        // Each non-default section's labels must cover exactly one integer value (K).
        if (section->labels().count() != 1) return false;
        (*case_blocks)[static_cast<int>(section->labels().intervals()[0].start)] = case_block;
    }
    return true;
}

bool RuntimeAsyncExceptionRewriteTransform::match_catch_entry_check(Block* continuation, ILVariable* num_variable, BlockContainer* container,
                                                                    Block** catch_body_entry, ILInstruction** after_catch_exit) {
    *catch_body_entry = nullptr;
    *after_catch_exit = nullptr;

    if (continuation->instructions.count() != 2) return false;
    auto* if_inst = as<IfInstruction>(continuation->instructions[0]);
    if (if_inst == nullptr) return false;

    // Equals form: if (num == 1) br catchBody ; <exit>
    ILInstruction* lhs = nullptr;
    ILInstruction* rhs = nullptr;
    if (if_inst->condition()->match_comp_equals(&lhs, &rhs) && lhs->match_ld_loc(num_variable) && rhs->match_ldc_i4(1)) {
        auto* eq_branch = as<Branch>(if_inst->true_inst());
        if (eq_branch != nullptr && is_container_exit(continuation->instructions[1], container)) {
            *catch_body_entry = eq_branch->target_block();
            *after_catch_exit = continuation->instructions[1];
            return *catch_body_entry != nullptr && (*catch_body_entry)->parent() == container;
        }
    }

    // Not-equals form: if (num != 1) <exit> ; br catchBody
    if (if_inst->condition()->match_comp_not_equals(&lhs, &rhs) && lhs->match_ld_loc(num_variable) && rhs->match_ldc_i4(1) &&
        is_container_exit(if_inst->true_inst(), container)) {
        auto* ne_branch = as<Branch>(continuation->instructions[1]);
        if (ne_branch != nullptr) {
            *catch_body_entry = ne_branch->target_block();
            *after_catch_exit = if_inst->true_inst();
            return *catch_body_entry != nullptr && (*catch_body_entry)->parent() == container;
        }
    }

    return false;
}

bool RuntimeAsyncExceptionRewriteTransform::is_container_exit(ILInstruction* inst, BlockContainer* container) {
    if (auto* leave = as<Leave>(inst)) return leave_exits_container(leave, container);
    if (auto* br = as<Branch>(inst); br != nullptr && br->target_block() != nullptr) {
        auto* target_container = as<BlockContainer>(br->target_block()->parent());
        if (target_container == nullptr) return false;
        if (target_container != container && container->is_descendant_of(target_container)) return true;
        if (target_container == container && br->target_block()->instructions.count() == 1) {
            if (auto* br_leave = as<Leave>(br->target_block()->instructions[0])) return leave_exits_container(br_leave, container);
        }
    }
    return false;
}

void RuntimeAsyncExceptionRewriteTransform::replace_variable_reads_with_handler_variable(ILInstruction* root, ILVariable* from, ILVariable* to) {
    std::vector<LdLoc*> ldlocs;
    for (ILInstruction* d : root->descendants())
        if (auto* ldloc = as<LdLoc>(d)) ldlocs.push_back(ldloc);
    for (LdLoc* ldloc : ldlocs) {
        if (ldloc->variable() != from) continue;
        // If parent is a CastClass to handler.Variable.Type or a base, inline directly.
        if (auto* cc = as<CastClass>(ldloc->parent()); cc != nullptr && typesystem::type_equals(cc->type(), to->type())) {
            cc->replace_with(with_il_range(with_il_range(make<LdLoc>(to), cc), ldloc));
        } else {
            ldloc->replace_with(with_il_range(make<LdLoc>(to), ldloc));
        }
    }
    std::vector<StLoc*> stlocs;
    for (ILInstruction* d : root->descendants())
        if (auto* stloc = as<StLoc>(d)) stlocs.push_back(stloc);
    for (StLoc* stloc : stlocs) {
        if (stloc->variable() == from) {
            // Drop dead stores to the synthesized variable.
            if (auto* parent_block = as<Block>(stloc->parent())) {
                parent_block->instructions.remove_at(stloc->child_index());
            }
        }
    }
}

bool RuntimeAsyncExceptionRewriteTransform::try_rewrite_flag_based_early_return(TryFinally* try_finally, ILTransformContext& context) {
    auto* parent_block = as<Block>(try_finally->parent());
    if (parent_block == nullptr) return false;
    auto* container = as<BlockContainer>(parent_block->parent());
    if (container == nullptr) return false;
    // The TryFinally is followed in parentBlock by either a `br checkBlock` or an inline
    // IfInstruction (when Roslyn merges the flag check into the parent block). Identify the
    // (block, startIndex) pair where the flag-check sequence lives.
    int try_finally_idx = try_finally->child_index();
    if (try_finally_idx == parent_block->instructions.count() - 1) return false;

    Block* check_block = nullptr;
    int check_start_index = 0;
    bool check_inline = false;
    auto* br_to_check = as<Branch>(parent_block->instructions[try_finally_idx + 1]);
    if (br_to_check != nullptr && br_to_check->target_block() != nullptr && br_to_check->target_block()->parent() == container) {
        check_block = br_to_check->target_block();
        check_start_index = 0;
        check_inline = false;
    } else if (is<IfInstruction>(parent_block->instructions[try_finally_idx + 1])) {
        check_block = parent_block;
        check_start_index = try_finally_idx + 1;
        check_inline = true;
    } else {
        return false;
    }
    // The flag-check sequence is two consecutive instructions: an IfInstruction guarding on
    // `flagVar` and a fall-through. Identify the "early" path (taken when flag == K) and the
    // "normal" path (the other) — each can be either a Branch to a block (we then need to
    // follow it via ResolveEarlyReturnValue), or a direct Leave / Throw.
    if (check_start_index + 1 >= check_block->instructions.count()) return false;
    auto* if_inst = as<IfInstruction>(check_block->instructions[check_start_index]);
    if (if_inst == nullptr) return false;

    ILVariable* flag_var = nullptr;
    int target_k = 0;
    ILInstruction* early_action = nullptr;
    ILInstruction* normal_action = nullptr;
    ILInstruction* lhs = nullptr;
    ILInstruction* rhs = nullptr;
    if (if_inst->condition()->match_comp_equals(&lhs, &rhs) && lhs->match_ld_loc(&flag_var) && rhs->match_ldc_i4(&target_k)) {
        early_action = if_inst->true_inst();
        normal_action = check_block->instructions[check_start_index + 1];
    } else if (if_inst->condition()->match_comp_not_equals(&lhs, &rhs) && lhs->match_ld_loc(&flag_var) && rhs->match_ldc_i4(&target_k)) {
        normal_action = if_inst->true_inst();
        early_action = check_block->instructions[check_start_index + 1];
    } else {
        return false;
    }
    if (!typesystem::is_known_type(flag_var->type(), KnownTypeCode::Int32)) return false;

    // Resolve the early-action into a list of instructions to splice into each flag-setter.
    std::vector<ILInstruction*> early_action_template;
    auto* early_leave = as<Leave>(early_action);
    auto* early_branch = as<Branch>(early_action);
    if (early_leave != nullptr && is_leave_to_container_or_ancestor(early_leave, container)) {
        early_action_template = {early_leave};
    } else if (early_branch != nullptr && early_branch->target_block() != nullptr && early_branch->target_block()->parent() == container &&
               try_get_early_action_template(early_branch->target_block(), container, &early_action_template)) {
        // templated
    } else {
        return false;
    }

    // Find flag-setter blocks anywhere inside the try-block (including nested containers).
    // Shape: zero or more capture-forwarding stores, then `stloc flagVar(K); leave tryBlock`.
    // Walking descendants picks up the post-rewrite spliced inner-flag-setters in the nested
    // try-finally case — those live inside the inner try-block container but still leave to
    // the outer try-block (because the splice inlined the outer's flag-set + leave).
    auto* try_block_container = as<BlockContainer>(try_finally->try_block());
    if (try_block_container == nullptr) return false;
    std::vector<Block*> flag_setters;
    for (ILInstruction* d : try_block_container->descendants()) {
        auto* b = as<Block>(d);
        if (b == nullptr) continue;
        int n = b->instructions.count();
        if (n < 2) continue;
        auto* set_store = as<StLoc>(b->instructions[n - 2]);
        if (set_store == nullptr || !flag_variable_matches(set_store->variable(), flag_var) || !set_store->value()->match_ldc_i4(target_k)) {
            continue;
        }
        auto* leave_from_try = as<Leave>(b->instructions[n - 1]);
        if (leave_from_try == nullptr || leave_from_try->target_container() != try_block_container) {
            continue;
        }
        flag_setters.push_back(b);
    }
    if (flag_setters.empty()) return false;

    // Verify flagVar is only set in flag-setters and the pre-try init (`stloc flagVar(0)`).
    for (IStoreInstruction* store_instruction : flag_var->store_instructions()) {
        auto* store = as<StLoc>(store_instruction->as_instruction());
        if (store == nullptr) continue;
        if (std::any_of(flag_setters.begin(), flag_setters.end(), [store](Block* fs) { return fs->instructions.contains(store); })) continue;
        if (store->parent() == parent_block && store->value()->match_ldc_i4(0)) continue;
        return false;
    }

    context.step_start_group("Reduce runtime-async flag-based early return", try_finally);

    // Replace each flag-setter's leave-tryBlock with a leave directly to leaveTargetContainer
    // (and, for value-returning sites, with the captured value). The capture-forwarding stores
    // before the flag setter stay; downstream cleanup drops them once the read in `earlyBlock`
    // disappears. Clear the spliced clones' top-level ILRange so each splice site keeps its
    // own sequence points instead of all claiming the source offset of the early-action.
    for (Block* fs : flag_setters) {
        int n = fs->instructions.count();
        // Drop the stloc flagVar(K) and the trailing leave-tryBlock. Splice in a clone of
        // the early-action template so the flag-setter's capture-forwarding stores flow
        // directly into the early-action's stores + final leave.
        fs->instructions.remove_at(n - 1);
        fs->instructions.remove_at(n - 2);
        for (ILInstruction* inst : early_action_template) {
            ILInstruction* spliced = inst->clone();
            spliced->set_il_range(Interval());
            fs->instructions.add(spliced);
        }
    }

    // Replace the flag-check sequence with the normal-path action. The early-path is now
    // taken by the rewritten flag-setters themselves, so the only remaining successor is
    // the normal path. Clone the normal-action so we don't have to worry about whose tree
    // it currently belongs to (it may be a child of the if-instruction we're tearing down),
    // and clear its ILRange — the clone lives at a different location than the original.
    ILInstruction* normal_action_clone = normal_action->clone();
    normal_action_clone->set_il_range(Interval());
    if (check_inline) {
        parent_block->instructions.remove_at(check_start_index + 1);
        parent_block->instructions.remove_at(check_start_index);
        parent_block->instructions.add(normal_action_clone);
    } else {
        check_block->instructions.clear();
        check_block->instructions.add(normal_action_clone);
    }

    // After the splice each flag-setter exits via a Leave that doesn't return to the
    // surrounding try-block, so any block whose only entry was the now-cleared check-block
    // is now unreachable. Drop those so the HasFlag check below sees the post-rewrite truth.
    try_block_container->sort_blocks(true);

    // After rewrite, the TryFinally may have unreachable endpoint (all flag-setter sites
    // became Leaves to an ancestor container, so the try-body no longer has a Leave-to-tryBlock).
    // The post-TryFinally instructions in parentBlock are then dead — and leaving them in place
    // would put a non-final unreachable-endpoint instruction in the block, violating the block
    // invariant. Drop everything after the TryFinally in that case.
    if (try_finally->has_flag(InstructionFlags::EndPointUnreachable)) {
        while (parent_block->instructions.count() > try_finally->child_index() + 1) parent_block->instructions.remove_at(parent_block->instructions.count() - 1);
    }

    // Drop the pre-try `stloc flagVar(0)`.
    for (int i = 0; i < parent_block->instructions.count(); i++) {
        auto* s = as<StLoc>(parent_block->instructions[i]);
        if (s != nullptr && flag_variable_matches(s->variable(), flag_var) && s->value()->match_ldc_i4(0)) {
            parent_block->instructions.remove_at(i);
            i--;
        }
    }

    context.step_end_group(true);
    return true;
}

bool RuntimeAsyncExceptionRewriteTransform::flag_variable_matches(ILVariable* candidate, ILVariable* flag_var) {
    if (candidate == flag_var) return true;
    return candidate->index() == flag_var->index() && candidate->kind() == flag_var->kind() &&
           typesystem::is_known_type(candidate->type(), KnownTypeCode::Int32);
}

bool RuntimeAsyncExceptionRewriteTransform::try_get_early_action_template(Block* early_block, BlockContainer* container, std::vector<ILInstruction*>* template_) {
    template_->clear();
    std::unordered_set<Block*> visited;
    Block* current = early_block;
    while (true) {
        if (!visited.insert(current).second) {
            template_->clear();
            return false;
        }
        if (current->instructions.count() == 0) {
            template_->clear();
            return false;
        }
        for (int i = 0; i < current->instructions.count() - 1; i++) template_->push_back(current->instructions[i]);
        ILInstruction* last = current->instructions[current->instructions.count() - 1];
        if (auto* leave = as<Leave>(last); leave != nullptr && is_leave_to_container_or_ancestor(leave, container)) {
            template_->push_back(leave);
            return true;
        }
        Block* next = nullptr;
        if (last->match_branch(&next)) {
            current = next;
            continue;
        }
        template_->clear();
        return false;
    }
}

bool RuntimeAsyncExceptionRewriteTransform::resolve_early_return_value(Block* early_block, BlockContainer* container, ILVariable** capture_var,
                                                                       BlockContainer** leave_target_container) {
    *capture_var = nullptr;
    *leave_target_container = nullptr;

    // Direct leave: earlyBlock is just `Leave container (value?)`.
    if (early_block->instructions.count() == 1) {
        auto* direct_leave = as<Leave>(early_block->instructions[0]);
        if (direct_leave != nullptr && is_leave_to_container_or_ancestor(direct_leave, container)) {
            *leave_target_container = direct_leave->target_container();
            if (direct_leave->value()->match_ld_loc(capture_var)) return true;
            if (direct_leave->value()->op_code == OpCode::Nop) {
                *capture_var = nullptr;
                return true;
            }
            return false;
        }
    }

    // Direct branch-to-leave (void return shape): earlyBlock is `[br leaveBlock]` where
    // `leaveBlock = [Leave container (void)]`.
    if (early_block->instructions.count() == 1) {
        Block* br_target = nullptr;
        if (early_block->instructions[0]->match_branch(&br_target) && br_target->instructions.count() == 1) {
            auto* br_leave = as<Leave>(br_target->instructions[0]);
            if (br_leave != nullptr && is_leave_to_container_or_ancestor(br_leave, container) && br_leave->value()->op_code == OpCode::Nop) {
                *leave_target_container = br_leave->target_container();
                *capture_var = nullptr;
                return true;
            }
        }
    }

    // Indirected shape: earlyBlock copies the capture into a returnVar and branches to a
    // one-instruction `Leave container (ldloc returnVar)` block.
    if (early_block->instructions.count() != 2) return false;
    ILVariable* return_var = nullptr;
    ILInstruction* rv_value = nullptr;
    if (!early_block->instructions[0]->match_st_loc(&return_var, &rv_value)) return false;
    if (!rv_value->match_ld_loc(capture_var)) return false;
    Block* leave_block = nullptr;
    if (!early_block->instructions[1]->match_branch(&leave_block)) return false;
    if (leave_block->instructions.count() != 1) return false;
    auto* final_leave = as<Leave>(leave_block->instructions[0]);
    if (final_leave == nullptr) return false;
    if (!is_leave_to_container_or_ancestor(final_leave, container)) return false;
    if (!final_leave->value()->match_ld_loc(return_var)) return false;
    *leave_target_container = final_leave->target_container();
    return true;
}

bool RuntimeAsyncExceptionRewriteTransform::is_leave_to_container_or_ancestor(Leave* leave, BlockContainer* container) {
    if (leave->target_container() == container) return true;
    return container->is_descendant_of(leave->target_container());
}

void RuntimeAsyncExceptionRewriteTransform::replace_dispatch_idiom_with_rethrow(Block* block, ILVariable* handler_variable, ILTransformContext& context) {
    // Reuse AwaitInCatchTransform.MatchExceptionCaptureBlock through the block-tail shape:
    //   stloc typedExVar(isinst Exception(ldloc handlerVariable))
    //   if (comp.o(ldloc typedExVar != ldnull)) br captureBlock
    //   br throwBlock
    // Block captureBlock { callvirt Throw(call Capture(ldloc typedExVar)); leave/br }
    // Block throwBlock { throw(ldloc handlerVariable) }
    ILVariable* v = handler_variable;
    StLoc* typed_exception_variable_store = nullptr;
    Block* capture_block = nullptr;
    Block* throw_block = nullptr;
    if (AwaitInCatchTransform::match_exception_capture_block(context, block, &v, &typed_exception_variable_store, &capture_block, &throw_block)) {
        if (v != handler_variable) return;
        // The Rethrow stands in for the whole dispatch idiom (this block's tail + the
        // capture/throw blocks). Capture IL ranges from each component before the
        // removals detach them, so source mapping stays anchored to the original bytes.
        Rethrow* rethrow = with_il_range(make<Rethrow>(), typed_exception_variable_store);
        rethrow->add_il_range(*block->instructions[typed_exception_variable_store->child_index() + 1]);
        rethrow->add_il_range(*block->instructions[typed_exception_variable_store->child_index() + 2]);
        for (ILInstruction* inst : capture_block->instructions) rethrow->add_il_range(*inst);
        for (ILInstruction* inst : throw_block->instructions) rethrow->add_il_range(*inst);
        block->instructions.remove_range(typed_exception_variable_store->child_index() + 1, 2);
        capture_block->remove();
        throw_block->remove();
        typed_exception_variable_store->replace_with(rethrow);
    }
}

}  // namespace cb::dotnet::decompiler::il::control_flow
