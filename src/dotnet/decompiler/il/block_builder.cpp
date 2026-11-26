// dotnet/decompiler/il/block_builder.cpp -- see block_builder.hpp.
#include "dotnet/decompiler/il/block_builder.hpp"

#include <algorithm>
#include <cassert>
#include <format>
#include <unordered_set>

#include "dotnet/decompiler/disassembler/disassembler_helpers.hpp"
#include "dotnet/decompiler/il/il_instruction_extensions.hpp"
#include "dotnet/decompiler/il/il_reader.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"

namespace cb::dotnet::decompiler::il {

namespace srm = cb::dotnet::metadata;

struct BlockBuilder::OnErrorDispatch {
    explicit OnErrorDispatch(ILVariable* v) : variable(v) {}
    ILVariable* variable;
    // the target offsets in the order added (upstream's HashSet<int> enumerates so)
    std::vector<int> target_il_offsets;
    std::unordered_set<int> target_set;
    std::vector<Branch*> branches;
};

BlockBuilder::BlockBuilder(const srm::MethodBodyBlock& body, std::vector<std::pair<int, ILVariable*>> variable_by_exception_handler,
                           const typesystem::ICompilation& compilation)
    : body_(body), variable_by_exception_handler_(std::move(variable_by_exception_handler)), compilation_(compilation) {}

BlockBuilder::~BlockBuilder() = default;

void BlockBuilder::create_container_structure() {
    std::vector<TryCatch*> try_catch_list;
    const auto& regions = body_.exception_regions();
    for (std::size_t region_index = 0; region_index < regions.size(); ++region_index) {
        const srm::ExceptionRegion& eh = regions[region_index];
        Interval try_range(eh.try_offset, eh.try_offset + eh.try_length);
        auto* handler_block = make<BlockContainer>();
        handler_block->add_il_range(Interval(eh.handler_offset, eh.handler_offset + eh.handler_length));
        handler_containers_.emplace(handler_block->start_il_offset(), handler_block);
        if (eh.kind == srm::ExceptionRegionKind::Fault || eh.kind == srm::ExceptionRegionKind::Finally) {
            auto* try_block = make<BlockContainer>();
            try_block->add_il_range(try_range);
            TryInstruction* inst;
            if (eh.kind == srm::ExceptionRegionKind::Finally)
                inst = make<TryFinally>(try_block, handler_block);
            else
                inst = make<TryFault>(try_block, handler_block);
            inst->set_il_range(try_range);
            try_instruction_list_.push_back(inst);
            continue;
        }
        TryCatch* try_catch = nullptr;
        for (TryCatch* tc : try_catch_list) {
            // upstream's ILRanges.SingleOrDefault(): an instruction has one range
            if (Interval(tc->try_block()->start_il_offset(), tc->try_block()->end_il_offset()) == try_range) {
                try_catch = tc;
                break;
            }
        }
        if (try_catch == nullptr) {
            auto* try_block = make<BlockContainer>();
            try_block->add_il_range(try_range);
            try_catch = make<TryCatch>(try_block);
            try_catch->add_il_range(try_range);
            try_catch_list.push_back(try_catch);
            try_instruction_list_.push_back(try_catch);
        }
        ILInstruction* filter;
        if (eh.kind == srm::ExceptionRegionKind::Filter) {
            auto* filter_block = make<BlockContainer>(ContainerKind::Normal, StackType::I4);
            filter_block->add_il_range(Interval(eh.filter_offset, eh.handler_offset));
            handler_containers_.emplace(filter_block->start_il_offset(), filter_block);
            filter = filter_block;
        } else {
            filter = make<LdcI4>(1);
        }
        ILVariable* variable = nullptr;
        for (const auto& [index, v] : variable_by_exception_handler_)
            if (index == static_cast<int>(region_index)) variable = v;
        auto* handler = make<TryCatchHandler>(filter, handler_block, variable);
        handler->add_il_range(*filter);
        handler->add_il_range(*handler_block);
        try_catch->handlers.add(handler);
        try_catch->add_il_range(*handler);
    }
    if (!try_instruction_list_.empty()) {
        // by the try block's start, then the longest first (OrderBy + ThenByDescending: stable)
        std::stable_sort(try_instruction_list_.begin(), try_instruction_list_.end(), [](const TryInstruction* a, const TryInstruction* b) {
            if (a->try_block()->start_il_offset() != b->try_block()->start_il_offset())
                return a->try_block()->start_il_offset() < b->try_block()->start_il_offset();
            return a->try_block()->end_il_offset() > b->try_block()->end_il_offset();
        });
        next_try_ = try_instruction_list_[0];
    }
}

void BlockBuilder::create_blocks(BlockContainer* main_container, const std::vector<Block*>& basic_blocks, const util::CancellationToken& cancellation_token) {
    create_container_structure();
    main_container->set_il_range(Interval(0, static_cast<int>(body_.il_bytes().size())));
    current_container_ = main_container;
    std::vector<Block*> ordered = basic_blocks;
    std::stable_sort(ordered.begin(), ordered.end(), [](const Block* a, const Block* b) { return a->start_il_offset() < b->start_il_offset(); });
    for (Block* block : ordered) {
        cancellation_token.throw_if_cancellation_requested();
        int start = block->start_il_offset();
        // leave the containers that end before this block
        while (start >= current_container_->end_il_offset() && !container_stack_.empty()) {
            current_container_ = container_stack_.back();
            container_stack_.pop_back();
        }
        // enter a handler
        if (auto it = handler_containers_.find(start); it != handler_containers_.end()) {
            container_stack_.push_back(current_container_);
            current_container_ = it->second;
        }
        // enter try blocks
        while (next_try_ != nullptr && start == next_try_->try_block()->start_il_offset()) {
            auto* block_for_try = make<Block>();
            block_for_try->set_il_range(*next_try_);
            block_for_try->instructions.add(next_try_);
            current_container_->blocks.add(block_for_try);
            container_stack_.push_back(current_container_);
            current_container_ = as<BlockContainer>(next_try_->try_block());
            ++current_try_index_;
            next_try_ = current_try_index_ < static_cast<int>(try_instruction_list_.size()) ? try_instruction_list_[static_cast<std::size_t>(current_try_index_)]
                                                                                            : nullptr;
        }
        current_container_->blocks.add(block);
    }
    assert(current_try_index_ == static_cast<int>(try_instruction_list_.size()) && next_try_ == nullptr);
    connect_branches(main_container, cancellation_token);
    create_on_error_dispatchers();
}

void BlockBuilder::connect_branches(ILInstruction* inst, const util::CancellationToken& cancellation_token) {
    if (auto* branch = as<Branch>(inst)) {
        cancellation_token.throw_if_cancellation_requested();
        assert(branch->target_block() == nullptr);
        Block* target_block = find_branch_target(branch->target_il_offset());
        if (target_block == nullptr) {
            auto* invalid = make<InvalidBranch>("Could not find block for branch target " + disassembler::offset_to_string(branch->target_il_offset()));
            invalid->set_il_range(*branch);
            branch->replace_with(invalid);
        } else {
            branch->set_target_block(target_block);
        }
        return;
    }
    if (auto* leave = as<Leave>(inst)) {
        if (leave->target_container() == nullptr) {
            leave->set_target_container(container_stack_.back());
            leave->set_value(ILReader::cast(leave->value(), leave->target_container()->expected_result_type(), nullptr, leave->start_il_offset()));
        }
        return;
    }
    if (auto* container = as<BlockContainer>(inst)) {
        container_stack_.push_back(container);
        int block_count = container->blocks.count();
        for (int i = 0; i < block_count; ++i) {
            cancellation_token.throw_if_cancellation_requested();
            Block* block = container->blocks[i];
            connect_branches(block, cancellation_token);
            if (block->instructions.count() == 0 || !block->instructions.last()->has_flag(InstructionFlags::EndPointUnreachable))
                block->instructions.add(make<InvalidBranch>("Unexpected end of block"));
        }
        container_stack_.pop_back();
        return;
    }
    for (ILInstruction* child : inst->children()) connect_branches(child, cancellation_token);
}

Block* BlockBuilder::find_branch_target(int target_il_offset) {
    // the containers from the innermost out
    for (auto it = container_stack_.rbegin(); it != container_stack_.rend(); ++it) {
        BlockContainer* container = *it;
        for (Block* block : container->blocks)
            if (block->start_il_offset() == target_il_offset && !block->il_range_is_empty()) return block;
        if (container->slot_info() == &TryCatchHandler::body_slot) {
            auto* try_catch = as<TryCatch>(container->parent()->parent());
            if (try_catch->try_block()->start_il_offset() < target_il_offset && target_il_offset < try_catch->try_block()->end_il_offset())
                return create_branch_target_for_on_error_jump(try_catch, target_il_offset);
        }
    }
    return nullptr;
}

Block* BlockBuilder::create_branch_target_for_on_error_jump(TryCatch* try_catch, int target_il_offset) {
    OnErrorDispatch* dispatch = nullptr;
    for (auto& [tc, d] : on_error_dispatchers_)
        if (tc == try_catch) dispatch = d.get();
    if (dispatch == nullptr) {
        const IType* int32 = compilation_.find_type(typesystem::KnownTypeCode::Int32);
        ILVariable* new_dispatch_var = make<ILVariable>(VariableKind::Local, int32, StackType::I4);
        new_dispatch_var->name = std::format("try{:04x}_dispatch", try_catch->start_il_offset());
        on_error_dispatchers_.emplace_back(try_catch, std::make_unique<OnErrorDispatch>(new_dispatch_var));
        dispatch = on_error_dispatchers_.back().second.get();
    }
    if (dispatch->target_set.insert(target_il_offset).second) dispatch->target_il_offsets.push_back(target_il_offset);
    auto* block = make<Block>();
    block->instructions.add(make<StLoc>(dispatch->variable, make<LdcI4>(target_il_offset)));
    auto* branch = make<Branch>(try_catch->try_block()->start_il_offset());
    block->instructions.add(branch);
    dispatch->branches.push_back(branch);
    container_stack_.back()->blocks.add(block);
    return block;
}

std::vector<ILVariable*> BlockBuilder::on_error_dispatcher_variables() const {
    std::vector<ILVariable*> result;
    for (const auto& [tc, d] : on_error_dispatchers_) result.push_back(d->variable);
    return result;
}

void BlockBuilder::create_on_error_dispatchers() {
    for (auto& [try_catch, dispatch] : on_error_dispatchers_) {
        auto* block = as<Block>(try_catch->parent());
        block->instructions.insert(try_catch->child_index(), make<StLoc>(dispatch->variable, make<LdcI4>(-1)));
        int split_at = try_catch->child_index();
        auto* new_block = make<Block>();
        new_block->add_il_range(*try_catch);
        std::vector<ILInstruction*> moved(block->instructions.begin() + split_at, block->instructions.end());
        new_block->instructions.add_range(moved);
        block->instructions.remove_range(split_at, block->instructions.count() - split_at);
        block->instructions.add(make<Branch>(new_block));
        as<BlockContainer>(block->parent())->blocks.add(new_block);
        for (Branch* b : dispatch->branches) b->set_target_block(new_block);

        auto* try_body = as<BlockContainer>(try_catch->try_block());
        auto* dispatch_block = make<Block>();
        dispatch_block->add_il_range(Interval(try_catch->start_il_offset(), try_catch->start_il_offset() + 1));
        auto* switch_inst = make<SwitchInstruction>(make<LdLoc>(dispatch->variable));
        switch_inst->add_il_range(Interval(try_catch->start_il_offset(), try_catch->start_il_offset() + 1));
        for (int offset : dispatch->target_il_offsets) {
            Block* target_block = nullptr;
            for (Block* b : try_body->blocks)
                if (b->start_il_offset() == offset && !b->il_range_is_empty()) {
                    target_block = b;
                    break;
                }
            ILInstruction* branch_inst;
            if (target_block == nullptr)
                branch_inst = make<InvalidBranch>("Could not find block for branch target " + disassembler::offset_to_string(offset));
            else
                branch_inst = make<Branch>(target_block);
            auto* section = make<SwitchSection>();
            section->set_labels(util::LongSet(static_cast<std::int64_t>(offset)));
            section->set_body(branch_inst);
            switch_inst->sections.add(section);
        }
        std::vector<util::LongInterval> used;
        for (int offset : dispatch->target_il_offsets) used.push_back(util::LongInterval::inclusive(offset, offset));
        util::LongSet used_labels(used);
        auto* default_section = make<SwitchSection>();
        default_section->set_labels(used_labels.invert());
        default_section->set_body(make<Branch>(try_body->entry_point()));
        switch_inst->sections.add(default_section);
        auto* note = make<InvalidExpression>("ILSpy has introduced the following switch to emulate a goto from catch-block to try-block");
        note->severity = "Note";
        dispatch_block->instructions.add(note);
        dispatch_block->instructions.add(switch_inst);
        try_body->blocks.insert(0, dispatch_block);
    }
}

}  // namespace cb::dotnet::decompiler::il
