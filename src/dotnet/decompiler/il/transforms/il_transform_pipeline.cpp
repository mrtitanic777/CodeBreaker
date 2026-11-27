// dotnet/decompiler/il/transforms/il_transform_pipeline.cpp -- see il_transform_pipeline.hpp.
#include "dotnet/decompiler/il/transforms/il_transform_pipeline.hpp"

#include "dotnet/decompiler/il/control_flow/async_await_decompiler.hpp"
#include "dotnet/decompiler/il/control_flow/condition_detection.hpp"
#include "dotnet/decompiler/il/control_flow/control_flow_simplification.hpp"
#include "dotnet/decompiler/il/control_flow/detect_pinned_regions.hpp"
#include "dotnet/decompiler/il/control_flow/exit_points.hpp"
#include "dotnet/decompiler/il/control_flow/loop_detection.hpp"
#include "dotnet/decompiler/il/control_flow/remove_redundant_return.hpp"
#include "dotnet/decompiler/il/control_flow/switch_detection.hpp"
#include "dotnet/decompiler/il/control_flow/yield_return_decompiler.hpp"
#include "dotnet/decompiler/il/transforms/assign_variable_names.hpp"
#include "dotnet/decompiler/il/transforms/cached_delegate_initialization.hpp"
#include "dotnet/decompiler/il/transforms/cached_read_only_span_initialization.hpp"
#include "dotnet/decompiler/il/transforms/copy_propagation.hpp"
#include "dotnet/decompiler/il/transforms/deconstruction_transform.hpp"
#include "dotnet/decompiler/il/transforms/delegate_construction.hpp"
#include "dotnet/decompiler/il/transforms/detect_catch_when_condition_blocks.hpp"
#include "dotnet/decompiler/il/transforms/dynamic_call_site_transform.hpp"
#include "dotnet/decompiler/il/transforms/dynamic_is_event_assignment_transform.hpp"
#include "dotnet/decompiler/il/transforms/early_expression_transforms.hpp"
#include "dotnet/decompiler/il/transforms/expression_transforms.hpp"
#include "dotnet/decompiler/il/transforms/fix_remaining_increments.hpp"
#include "dotnet/decompiler/il/transforms/high_level_loop_transform.hpp"
#include "dotnet/decompiler/il/transforms/il_inlining.hpp"
#include "dotnet/decompiler/il/transforms/index_range_transform.hpp"
#include "dotnet/decompiler/il/transforms/inline_return_transform.hpp"
#include "dotnet/decompiler/il/transforms/interpolated_string_transform.hpp"
#include "dotnet/decompiler/il/transforms/introduce_dynamic_type_on_locals.hpp"
#include "dotnet/decompiler/il/transforms/introduce_native_int_type_on_locals.hpp"
#include "dotnet/decompiler/il/transforms/introduce_ref_read_only_modifier_on_locals.hpp"
#include "dotnet/decompiler/il/transforms/ld_loca_dup_init_obj_transform.hpp"
#include "dotnet/decompiler/il/transforms/local_function_decompiler.hpp"
#include "dotnet/decompiler/il/transforms/lock_transform.hpp"
#include "dotnet/decompiler/il/transforms/named_argument_transform.hpp"
#include "dotnet/decompiler/il/transforms/null_coalescing_transform.hpp"
#include "dotnet/decompiler/il/transforms/null_propagation_transform.hpp"
#include "dotnet/decompiler/il/transforms/nullable_lifting_transform.hpp"
#include "dotnet/decompiler/il/transforms/pattern_matching_transform.hpp"
#include "dotnet/decompiler/il/transforms/proxy_call_replacer.hpp"
#include "dotnet/decompiler/il/transforms/reduce_nesting_transform.hpp"
#include "dotnet/decompiler/il/transforms/remove_dead_variable_init.hpp"
#include "dotnet/decompiler/il/transforms/remove_infeasible_path_transform.hpp"
#include "dotnet/decompiler/il/transforms/remove_unconstrained_generic_reference_type_check.hpp"
#include "dotnet/decompiler/il/transforms/split_variables.hpp"
#include "dotnet/decompiler/il/transforms/switch_on_nullable_transform.hpp"
#include "dotnet/decompiler/il/transforms/switch_on_string_transform.hpp"
#include "dotnet/decompiler/il/transforms/transform_array_initializers.hpp"
#include "dotnet/decompiler/il/transforms/transform_assignment.hpp"
#include "dotnet/decompiler/il/transforms/transform_collection_and_object_initializers.hpp"
#include "dotnet/decompiler/il/transforms/transform_display_class_usage.hpp"
#include "dotnet/decompiler/il/transforms/transform_expression_trees.hpp"
#include "dotnet/decompiler/il/transforms/user_defined_logic_transform.hpp"
#include "dotnet/decompiler/il/transforms/using_transform.hpp"

namespace cb::dotnet::decompiler::il::transforms {

namespace {
template <class... T>
std::vector<std::unique_ptr<IILTransform>> list(std::unique_ptr<T>... items) {
    std::vector<std::unique_ptr<IILTransform>> result;
    (result.push_back(std::move(items)), ...);
    return result;
}
template <class... T>
std::vector<std::unique_ptr<IStatementTransform>> statements(std::unique_ptr<T>... items) {
    std::vector<std::unique_ptr<IStatementTransform>> result;
    (result.push_back(std::move(items)), ...);
    return result;
}
template <class T>
std::unique_ptr<T> mk() {
    return std::make_unique<T>();
}
}  // namespace

std::vector<std::unique_ptr<IILTransform>> early_il_transforms(bool aggressively_duplicate_return_blocks) {
    auto simplification = std::make_unique<control_flow::ControlFlowSimplification>();
    simplification->aggressively_duplicate_return_blocks = aggressively_duplicate_return_blocks;
    return list(std::move(simplification), mk<SplitVariables>(), mk<ILInlining>());
}

std::vector<std::unique_ptr<IILTransform>> get_il_transforms() {
    using namespace control_flow;
    auto loops = std::make_unique<BlockILTransform>();
    loops->post_order_transforms.push_back(mk<LoopDetection>());

    auto blocks = std::make_unique<BlockILTransform>();
    blocks->post_order_transforms.push_back(mk<ConditionDetection>());
    blocks->post_order_transforms.push_back(mk<LockTransform>());
    blocks->post_order_transforms.push_back(mk<UsingTransform>());
    // CachedDelegateInitialization runs after ConditionDetection, before NullCoalescingTransform
    blocks->post_order_transforms.push_back(mk<CachedDelegateInitialization>());
    blocks->post_order_transforms.push_back(mk<CachedReadOnlySpanInitialization>());
    // statement by statement: the transforms that open up inlining (inlining first; the others ask for re-runs)
    auto inlining = mk<ILInlining>();
    inlining->options = InliningOptions::AllowInliningOfLdloca;
    blocks->post_order_transforms.push_back(std::make_unique<StatementTransform>(statements(
        std::move(inlining), mk<ExpressionTransforms>(), mk<DynamicIsEventAssignmentTransform>(), mk<TransformAssignment>(),
        mk<NullCoalescingTransform>(), mk<NullableLiftingStatementTransform>(), mk<NullPropagationStatementTransform>(),
        mk<TransformArrayInitializers>(), mk<TransformCollectionAndObjectInitializers>(), mk<TransformExpressionTrees>(),
        mk<IndexRangeTransform>(), mk<DeconstructionTransform>(), mk<NamedArgumentTransform>(),
        mk<RemoveUnconstrainedGenericReferenceTypeCheck>(), mk<UserDefinedLogicTransform>(), mk<InterpolatedStringTransform>())));

    return list(mk<ControlFlowSimplification>(),
                // SplitVariables only after ControlFlowSimplification duplicates return blocks, so the return variable is split
                mk<SplitVariables>(), mk<ILInlining>(),
                mk<InlineReturnTransform>(),  // before DetectPinnedRegions
                mk<RemoveInfeasiblePathTransform>(),
                mk<DetectPinnedRegions>(),  // after inlining, before the non-critical control flow transforms
                mk<YieldReturnDecompiler>(), mk<AsyncAwaitDecompiler>(), mk<DetectCatchWhenConditionBlocks>(),  // before loop detection
                mk<DetectExitPoints>(), mk<LdLocaDupInitObjTransform>(), mk<EarlyExpressionTransforms>(),
                mk<SplitVariables>(),  // again: stobj(ldloca V, ...) may open new replacements
                mk<RemoveDeadVariableInit>(),  // after EarlyExpressionTransforms
                mk<ControlFlowSimplification>(),  // split variables may allow more branch to leave inlining
                mk<DynamicCallSiteTransform>(), mk<SwitchDetection>(), mk<SwitchOnStringTransform>(), mk<SwitchOnNullableTransform>(),
                mk<SplitVariables>(),  // again: SwitchOnNullableTransform removes ldloca
                mk<IntroduceRefReadOnlyModifierOnLocals>(), std::move(loops),
                mk<DetectExitPoints>(),  // again after loop detection
                mk<PatternMatchingTransform>(),  // after LoopDetection, before ConditionDetection
                std::move(blocks), mk<ProxyCallReplacer>(), mk<FixRemainingIncrements>(), mk<CopyPropagation>(), mk<DelegateConstruction>(),
                mk<LocalFunctionDecompiler>(), mk<TransformDisplayClassUsage>(), mk<HighLevelLoopTransform>(), mk<ReduceNestingTransform>(),
                mk<RemoveRedundantReturn>(), mk<IntroduceDynamicTypeOnLocals>(), mk<IntroduceNativeIntTypeOnLocals>(), mk<AssignVariableNames>());
}

}  // namespace cb::dotnet::decompiler::il::transforms
