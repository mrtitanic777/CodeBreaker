// dotnet/decompiler/il/transforms/il_transform.cxx -- see il_transform.hpp.
#include "dotnet/decompiler/il/transforms/il_transform.hpp"

#include <cassert>

#include "core/error.hpp"
#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/flow_analysis/control_flow_node.hpp"
#include "dotnet/decompiler/il/control_flow/control_flow_graph.hpp"
#include "dotnet/decompiler/il/il_reader.hpp"
#include "dotnet/decompiler/typesystem/decompiler_type_system.hpp"
#include "dotnet/decompiler/util/tree_traversal.hpp"

namespace cb::dotnet::decompiler::il {

void ILFunction::run_transforms(const std::vector<std::unique_ptr<transforms::IILTransform>>& transforms, transforms::ILTransformContext& context) {
#ifndef NDEBUG
    check_invariant(ILPhase::Normal);
#endif
    for (const auto& transform : transforms) {
        context.cancellation_token.throw_if_cancellation_requested();
        if (auto* block_transform = dynamic_cast<transforms::BlockILTransform*>(transform.get()))
            context.step_start_group(block_transform->to_string());
        else
            context.step_start_group(transform->name());
        transform->run(this, context);
#ifndef NDEBUG
        check_invariant(ILPhase::Normal);
#endif
        context.step_end_group(true);
    }
}

void Block::run_transforms(const std::vector<std::unique_ptr<transforms::IBlockTransform>>& transforms, transforms::BlockTransformContext& context) {
#ifndef NDEBUG
    check_invariant(ILPhase::Normal);
#endif
    for (const auto& transform : transforms) {
        context.cancellation_token.throw_if_cancellation_requested();
        assert(context.index_of_first_already_transformed_instruction <= instructions.count());
        context.step_start_group(transform->name());
        transform->run(this, context);
#ifndef NDEBUG
        check_invariant(ILPhase::Normal);
#endif
        context.step_end_group();
    }
}

}  // namespace cb::dotnet::decompiler::il

namespace cb::dotnet::decompiler::il::transforms {

ILTransformContext::ILTransformContext(ILFunction* function, const typesystem::DecompilerTypeSystem& type_system, const DecompilerSettings& settings)
    : function_(function), type_system_(&type_system), settings_(&settings) {
    if (function == nullptr) throw std::invalid_argument("function");
}

ILTransformContext::ILTransformContext(const ILTransformContext& context, ILFunction* function)
    : cancellation_token(context.cancellation_token),
      function_(function != nullptr ? function : context.function_),
      type_system_(context.type_system_),
      settings_(context.settings_) {}

const metadata::MetadataFile& ILTransformContext::pe_file() const { return *type_system_->main_metadata_module().metadata_file(); }

std::unique_ptr<ILReader> ILTransformContext::create_il_reader() const {
    auto reader = std::make_unique<ILReader>(type_system_->main_metadata_module());
    reader->use_debug_symbols = settings_->use_debug_symbols;
    reader->use_ref_locals_for_accurate_order_of_evaluation = settings_->use_ref_locals_for_accurate_order_of_evaluation;
    return reader;
}

std::string BlockILTransform::to_string() const {
    std::string names;
    for (const auto* list : {&pre_order_transforms, &post_order_transforms})
        for (const auto& t : *list) {
            if (!names.empty()) names += ", ";
            names += t->name();
        }
    return "BlockILTransform (" + names + ")";
}

void BlockILTransform::run(ILFunction* function, ILTransformContext& context) {
    if (running_) throw UsageError("Reentrancy detected. Transforms (and the CSharpDecompiler) are neither thread-safe nor re-entrant.");
    struct Running {
        bool& flag;
        explicit Running(bool& f) : flag(f) { flag = true; }
        ~Running() { flag = false; }
    } guard(running_);
    BlockTransformContext block_context(context);
    assert(block_context.function() == function);
    std::vector<BlockContainer*> containers;
    for (ILInstruction* inst : function->descendants())
        if (auto* container = as<BlockContainer>(inst)) containers.push_back(container);
    for (BlockContainer* container : containers) {
        context.cancellation_token.throw_if_cancellation_requested();
        control_flow::ControlFlowGraph graph(container, context.cancellation_token);
        block_context.control_flow_graph = &graph;
        visit_block(graph.get_node(container->entry_point()), block_context);
        block_context.control_flow_graph = nullptr;
    }
}

void BlockILTransform::visit_block(flow_analysis::ControlFlowNode* entry_node, BlockTransformContext& context) {
    // the pre-order transforms run as the walk reaches a node (before its dominator tree children), the post-order ones after them
    auto pre_order = [&](flow_analysis::ControlFlowNode* cfg_node) {
        Block* block = cfg_node->user_data;
        context.step_start_group(block->label(), block);
        context.control_flow_node = cfg_node;
        context.block = block;
        context.index_of_first_already_transformed_instruction = block->instructions.count();
        block->run_transforms(pre_order_transforms, context);
        return cfg_node->dominator_tree_children;
    };
    util::post_order(entry_node, pre_order, [&](flow_analysis::ControlFlowNode* cfg_node) {
        Block* block = cfg_node->user_data;
        context.control_flow_node = cfg_node;
        context.block = block;
        context.index_of_first_already_transformed_instruction = block->instructions.count();
        block->run_transforms(post_order_transforms, context);
        context.step_end_group();
    });
}

void StatementTransform::run(Block* block, BlockTransformContext& context) {
    StatementTransformContext ctx(context);
    int pos = 0;
    if (context.index_of_first_already_transformed_instruction == 0) return;
    ctx.rerun_position = context.index_of_first_already_transformed_instruction - 1;
    while (pos >= 0) {
        if (ctx.rerun_position) {
            assert(*ctx.rerun_position >= pos);
            pos = *ctx.rerun_position;
            ctx.rerun_position.reset();
        }
        for (const auto& transform : children_) {
            transform->run(block, pos, ctx);
#ifndef NDEBUG
            block->instructions[pos]->check_invariant(ILPhase::Normal);
#endif
            if (ctx.rerun_current_position) {
                ctx.rerun_current_position = false;
                ctx.request_rerun(pos);
            }
            if (ctx.rerun_position) break;
        }
        if (!ctx.rerun_position) --pos;
    }
    assert(block->has_flag(InstructionFlags::EndPointUnreachable));
}

}  // namespace cb::dotnet::decompiler::il::transforms
