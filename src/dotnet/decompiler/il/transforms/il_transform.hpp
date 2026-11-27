// dotnet/decompiler/il/transforms/il_transform.hpp -- the frame of the ILAst transforms: a transform runs on a function (IILTransform),
// on a block (IBlockTransform, run by BlockILTransform over each container's dominator tree, pre-order and post-order) or on one
// statement of a block (IStatementTransform, run by StatementTransform from the last statement back, re-running where asked); the
// contexts carry the type system, the settings and the cancellation token.
//
//     ILTransformContext context(function, type_system, settings);
//     function->run_transforms(transforms, context);      // each transform in order, the invariants checked in debug builds
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.Transforms: IILTransform, ILTransformContext, IBlockTransform, BlockTransformContext,
// BlockILTransform, IStatementTransform, StatementTransformContext, StatementTransform; ICSharpCode.Decompiler.DebugInfo's Stepper is
// not ported (it only records steps for ILSpy's debug view), so the Step* calls do nothing), Copyright AlphaSierraPapa for the ILSpy
// team, MIT (see NOTICE in dotnet/decompiler). Not yet ported: the context's C# resolver and DecompileRun (with the C# stages).
#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "dotnet/decompiler/il/instructions.hpp"
#include "dotnet/decompiler/util/cancellation.hpp"

namespace cb::dotnet::decompiler {
class DecompilerSettings;
}
namespace cb::dotnet::decompiler::typesystem {
class DecompilerTypeSystem;
}
namespace cb::dotnet::decompiler::metadata {
class MetadataFile;
}
namespace cb::dotnet::decompiler::flow_analysis {
class ControlFlowNode;
}
namespace cb::dotnet::decompiler::il::control_flow {
class ControlFlowGraph;
}

namespace cb::dotnet::decompiler::il {
class ILReader;
}

namespace cb::dotnet::decompiler::il::transforms {

class ILTransformContext;

class IILTransform {
public:
    virtual ~IILTransform() = default;
    virtual void run(ILFunction* function, ILTransformContext& context) = 0;
    // The class name (upstream's GetType().Name), for the step log.
    virtual std::string name() const = 0;
};

class ILTransformContext {
public:
    ILTransformContext(ILFunction* function, const typesystem::DecompilerTypeSystem& type_system, const DecompilerSettings& settings);
    // A copy of `context` for another function (or the same).
    ILTransformContext(const ILTransformContext& context, ILFunction* function = nullptr);
    virtual ~ILTransformContext() = default;

    ILFunction* function() const { return function_; }
    const typesystem::DecompilerTypeSystem& type_system() const { return *type_system_; }
    const DecompilerSettings& settings() const { return *settings_; }
    const metadata::MetadataFile& pe_file() const;
    util::CancellationToken cancellation_token;

    // A reader for more method bodies (lambdas, state machines) with the settings' options.
    std::unique_ptr<ILReader> create_il_reader() const;

    // The step log (upstream's [Conditional("STEP")] methods; compiled out upstream in release builds, as here).
    void step(const std::string&, ILInstruction*) {}
    void step_start_group(const std::string&, ILInstruction* = nullptr) {}
    void step_end_group(bool = false) {}
    void end_step(ILInstruction*) {}

private:
    ILFunction* function_;
    const typesystem::DecompilerTypeSystem* type_system_;
    const DecompilerSettings* settings_;
};

class BlockTransformContext;

class IBlockTransform {
public:
    virtual ~IBlockTransform() = default;
    virtual void run(Block* block, BlockTransformContext& context) = 0;
    virtual std::string name() const = 0;
};

class BlockTransformContext : public ILTransformContext {
public:
    explicit BlockTransformContext(const ILTransformContext& context) : ILTransformContext(context) {}
    // The block being transformed.
    Block* block = nullptr;
    // Its node in the control flow graph.
    flow_analysis::ControlFlowNode* control_flow_node = nullptr;
    // The graph of the container the block is in.
    control_flow::ControlFlowGraph* control_flow_graph = nullptr;
    // The instructions from this index on were already transformed by the post-order transforms (inlined blocks are put at the end).
    int index_of_first_already_transformed_instruction = 0;
};

// Runs block transforms on every block of every container of the function, in the container's dominator tree: the pre-order ones
// going down, the post-order ones coming up.
class BlockILTransform final : public IILTransform {
public:
    std::vector<std::unique_ptr<IBlockTransform>> pre_order_transforms;
    std::vector<std::unique_ptr<IBlockTransform>> post_order_transforms;

    void run(ILFunction* function, ILTransformContext& context) override;
    std::string name() const override { return "BlockILTransform"; }
    // "BlockILTransform (LoopDetection, ...)"
    std::string to_string() const;

private:
    void visit_block(flow_analysis::ControlFlowNode* entry_node, BlockTransformContext& context);
    bool running_ = false;
};

class StatementTransformContext;

class IStatementTransform {
public:
    virtual ~IStatementTransform() = default;
    // Runs on the statement at `pos` (block->instructions[pos]); may change it and the statements after it.
    virtual void run(Block* block, int pos, StatementTransformContext& context) = 0;
    virtual std::string name() const = 0;
};

class StatementTransformContext : public ILTransformContext {
public:
    explicit StatementTransformContext(BlockTransformContext& block_context) : ILTransformContext(block_context), block_context_(&block_context) {}
    BlockTransformContext& block_context() const { return *block_context_; }
    Block* block() const { return block_context_->block; }

    // Runs the transforms again from `pos` (the latest asked wins when it is the greater).
    void request_rerun(int pos) {
        if (!rerun_position || pos > *rerun_position) rerun_position = pos;
    }
    // Runs the transforms again on the current statement.
    void request_rerun() { rerun_current_position = true; }

    bool rerun_current_position = false;
    std::optional<int> rerun_position;

private:
    BlockTransformContext* block_context_;
};

// Runs statement transforms interleaved, statement by statement, from the last statement not yet transformed back to the first.
class StatementTransform final : public IBlockTransform {
public:
    explicit StatementTransform(std::vector<std::unique_ptr<IStatementTransform>> children) : children_(std::move(children)) {}
    void run(Block* block, BlockTransformContext& context) override;
    std::string name() const override { return "StatementTransform"; }

private:
    std::vector<std::unique_ptr<IStatementTransform>> children_;
};

}  // namespace cb::dotnet::decompiler::il::transforms
