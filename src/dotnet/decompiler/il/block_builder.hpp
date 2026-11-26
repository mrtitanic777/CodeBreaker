// dotnet/decompiler/il/block_builder.hpp -- puts the basic blocks ILReader made into BlockContainers nested by the method's exception
// regions (try, catch, filter, finally, fault), connects each branch to its target block and each leave to its container, and turns a
// jump from a catch block back into its try block (VB's On Error) into a dispatch switch at the start of the try.
//
//     BlockBuilder builder(body, variable_by_exception_handler, compilation);
//     builder.create_blocks(main_container, basic_blocks, cancellation_token);
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL: BlockBuilder), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in
// dotnet/decompiler).
#pragma once

#include <map>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

#include "dotnet/decompiler/il/instructions.hpp"
#include "dotnet/decompiler/util/cancellation.hpp"
#include "dotnet/metadata/pe_reader.hpp"

namespace cb::dotnet::decompiler::typesystem {
class ICompilation;
}

namespace cb::dotnet::decompiler::il {

class BlockBuilder {
public:
    // variable_by_exception_handler: each catch or filter region (by its index in the body) with its exception variable.
    BlockBuilder(const dotnet::metadata::MethodBodyBlock& body, std::vector<std::pair<int, ILVariable*>> variable_by_exception_handler,
                 const typesystem::ICompilation& compilation);
    ~BlockBuilder();

    bool create_extended_blocks = false;

    void create_blocks(BlockContainer* main_container, const std::vector<Block*>& basic_blocks, const util::CancellationToken& cancellation_token);
    // The dispatch variables made for jumps from catch blocks into their try blocks.
    std::vector<ILVariable*> on_error_dispatcher_variables() const;

private:
    struct OnErrorDispatch;

    void create_container_structure();
    void connect_branches(ILInstruction* inst, const util::CancellationToken& cancellation_token);
    Block* find_branch_target(int target_il_offset);
    Block* create_branch_target_for_on_error_jump(TryCatch* try_catch, int target_il_offset);
    void create_on_error_dispatchers();

    const dotnet::metadata::MethodBodyBlock& body_;
    std::vector<std::pair<int, ILVariable*>> variable_by_exception_handler_;
    const typesystem::ICompilation& compilation_;
    std::vector<TryInstruction*> try_instruction_list_;
    std::unordered_map<int, BlockContainer*> handler_containers_;
    int current_try_index_ = 0;
    TryInstruction* next_try_ = nullptr;
    BlockContainer* current_container_ = nullptr;
    std::vector<BlockContainer*> container_stack_;  // the top is the back
    // in the order made (upstream's Dictionary)
    std::vector<std::pair<TryCatch*, std::unique_ptr<OnErrorDispatch>>> on_error_dispatchers_;
};

}  // namespace cb::dotnet::decompiler::il
