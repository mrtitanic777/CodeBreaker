// dotnet/decompiler/il/control_flow/exit_points.hpp -- detects suitable exit points for block containers.
//
// An "exit point" is an instruction that makes control flow leave the container (a branch or leave). If an exit point sits right after
// a block container, every equivalent exit point inside the container becomes a `leave container`. If the container is not yet
// followed by an exit point but has room for one (its end point is unreachable), one of the non-return exit points inside is moved
// behind the container and the equivalent ones inside become leaves. That makes it easier for the following transforms to build
// control flow that falls out of blocks instead of using goto and break.
//
//     DetectExitPoints transform;
//     transform.run(function, context);
//     bool same = DetectExitPoints::compatible_exit_instruction(a, b);   // both branch to one block, or leave one container
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.ControlFlow: DetectExitPoints, from ControlFlow/ExitPoints.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "dotnet/decompiler/il/il_visitor.hpp"
#include "dotnet/decompiler/il/transforms/il_transform.hpp"
#include "dotnet/decompiler/util/cancellation.hpp"

namespace cb::dotnet::decompiler::il::control_flow {

class DetectExitPoints final : public ILVisitor, public transforms::IILTransform {
public:
    // The instruction executed next after `inst`: the next instruction of its block, or that of the enclosing try / handler / pinned /
    // using / lock; the "not yet determined" marker (a nop) at the end of a block, the "no exit" marker (a nop) where it cannot be
    // identified.
    static ILInstruction* get_exit(ILInstruction* inst);
    // Whether the two are both exit instructions (branch or leave) of the same exit.
    static bool compatible_exit_instruction(ILInstruction* exit1, ILInstruction* exit2);

    void run(ILFunction* function, transforms::ILTransformContext& context) override;
    std::string name() const override { return "DetectExitPoints"; }

    void default_visit(ILInstruction& inst) override;
    void visit_block_container(BlockContainer& container) override;
    void visit_block(Block& block) override;
    void visit_branch(Branch& inst) override;
    void visit_leave(Leave& inst) override;

private:
    struct ContainerContext {
        BlockContainer* container;
        // The instruction executed next after leaving the container. The "not yet determined" marker means the container is last in
        // its parent block, so it has no leave instructions yet and any exit instruction may be moved out and replaced with a leave.
        ILInstruction* current_exit;
        // With current_exit "not yet determined", the exits found; one of them is chosen once the container was visited completely.
        std::optional<std::vector<ILInstruction*>> potential_exits;

        ContainerContext(BlockContainer* container, ILInstruction* current_exit);
        void handle_exit(ILInstruction* inst);
    };

    static bool is_infinite_loop(Block* block);
    static ILInstruction* choose_exit(const std::vector<ILInstruction*>& potential_exits);

    util::CancellationToken cancellation_token_;
    std::vector<Block*> blocks_potentially_made_unreachable_;
    std::vector<std::unique_ptr<ContainerContext>> container_stack_;
};

}  // namespace cb::dotnet::decompiler::il::control_flow
