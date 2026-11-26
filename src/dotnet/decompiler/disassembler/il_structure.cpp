// dotnet/decompiler/disassembler/il_structure.cpp -- ILStructure.
#include "dotnet/decompiler/disassembler/il_structure.hpp"

#include <algorithm>

#include "dotnet/decompiler/disassembler/il_parser.hpp"
#include "dotnet/decompiler/util/bit_set.hpp"

namespace cb::dotnet::decompiler::disassembler {
namespace {

struct Branch {
    util::Interval source;
    int target;
};

bool is_unconditional_branch(ILOpCode op) {
    switch (op) {
    case ILOpCode::Br: case ILOpCode::Br_s: case ILOpCode::Ret: case ILOpCode::Endfilter: case ILOpCode::Endfinally: case ILOpCode::Throw:
    case ILOpCode::Rethrow: case ILOpCode::Leave: case ILOpCode::Leave_s:
        return true;
    default:
        return false;
    }
}

std::pair<std::vector<Branch>, util::BitSet> find_all_branches(srm::BlobReader body) {
    std::vector<Branch> result;
    util::BitSet after_unconditional(body.length() + 1);
    body.reset();
    while (body.remaining_bytes() > 0) {
        const int offset = body.offset();
        const ILOpCode op = decode_op_code(body);
        switch (metadata::get_operand_type(op)) {
        case OperandType::BrTarget:
        case OperandType::ShortBrTarget: {
            const int target = decode_branch_target(body, op);
            const int end = body.offset();
            result.push_back({util::Interval(offset, end), target});
            after_unconditional.assign(end, is_unconditional_branch(op));
            break;
        }
        case OperandType::Switch:
            for (const int t : decode_switch_targets(body)) result.push_back({util::Interval(offset, body.offset()), t});
            break;
        default:
            skip_operand(body, op);
            after_unconditional.assign(body.offset(), is_unconditional_branch(op));
            break;
        }
    }
    return {std::move(result), std::move(after_unconditional)};
}

}  // namespace

ILStructure::ILStructure(const metadata::MetadataFile& module_, srm::MethodDefinitionHandle handle, const metadata::MetadataGenericContext& context,
                         const srm::MethodBodyBlock& body)
    : ILStructure(module_, handle, context, ILStructureType::Root, 0, static_cast<int>(body.il_bytes().size())) {
    // the tree of exception structures
    const auto& regions = body.exception_regions();
    for (std::size_t i = 0; i < regions.size(); ++i) {
        const srm::ExceptionRegion& eh = regions[i];
        const bool seen = std::any_of(regions.begin(), regions.begin() + static_cast<std::ptrdiff_t>(i),
                                      [&](const srm::ExceptionRegion& old) { return old.try_offset == eh.try_offset && old.try_length == eh.try_length; });
        if (!seen)
            add_nested_structure(std::make_unique<ILStructure>(module, handle, context, ILStructureType::Try, eh.try_offset, eh.try_offset + eh.try_length, eh));
        if (eh.kind == srm::ExceptionRegionKind::Filter)
            add_nested_structure(std::make_unique<ILStructure>(module, handle, context, ILStructureType::Filter, eh.filter_offset, eh.handler_offset, eh));
        add_nested_structure(
            std::make_unique<ILStructure>(module, handle, context, ILStructureType::Handler, eh.handler_offset, eh.handler_offset + eh.handler_length, eh));
    }
    // very simple loop detection: backward branches, the last first so the biggest loop boundary is found first ("continue;" loops)
    const auto [branches, after_unconditional] = find_all_branches(body.il_reader());
    for (std::size_t i = branches.size(); i-- > 0;) {
        const int loop_end = branches[i].source.end;
        const int loop_start = branches[i].target;
        if (loop_start >= loop_end) continue;
        // a possible loop: does it have only one entry point? (the first instruction is one, unless what precedes it is an unconditional branch)
        int entry_point = -1;
        if (loop_start > 0 && !after_unconditional[loop_start]) entry_point = branches[i].target;
        bool multiple = false;
        for (const Branch& b : branches) {
            if (b.source.start < loop_start || b.source.start >= loop_end) {
                if (loop_start <= b.target && b.target < loop_end) {   // a jump into the loop from outside it
                    if (entry_point < 0) entry_point = b.target;
                    else if (b.target != entry_point) multiple = true;
                }
            }
        }
        if (!multiple) add_nested_structure(std::make_unique<ILStructure>(module, handle, context, ILStructureType::Loop, loop_start, loop_end, entry_point));
    }
    sort_children();
}

ILStructure::ILStructure(const metadata::MetadataFile& module_, srm::MethodDefinitionHandle handle, const metadata::MetadataGenericContext& context,
                         ILStructureType type_, int start, int end, const srm::ExceptionRegion& handler)
    : module(module_), method_handle(handle), generic_context(context), type(type_), start_offset(start), end_offset(end), exception_handler(handler) {}

ILStructure::ILStructure(const metadata::MetadataFile& module_, srm::MethodDefinitionHandle handle, const metadata::MetadataGenericContext& context,
                         ILStructureType type_, int start, int end, int loop_entry_point)
    : module(module_), method_handle(handle), generic_context(context), type(type_), start_offset(start), end_offset(end),
      loop_entry_point_offset(loop_entry_point) {}

bool ILStructure::add_nested_structure(std::unique_ptr<ILStructure> s) {
    // the loop-like shape of "continue;" is not a nested loop
    if (type == ILStructureType::Loop && s->type == ILStructureType::Loop && s->start_offset == start_offset) return false;
    if (s->start_offset < 0) return false;
    for (const auto& child : children) {
        if (child->start_offset <= s->start_offset && s->end_offset <= child->end_offset) return child->add_nested_structure(std::move(s));
        if (!(child->end_offset <= s->start_offset || s->end_offset <= child->start_offset)) {
            // they overlap: unless the new one holds the child, the nesting is invalid and the new one is dropped
            if (!(s->start_offset <= child->start_offset && child->end_offset <= s->end_offset)) return false;
        }
    }
    // the existing structures inside the new one move into it
    for (std::size_t i = 0; i < children.size(); ++i) {
        if (s->start_offset <= children[i]->start_offset && children[i]->end_offset <= s->end_offset) {
            s->children.push_back(std::move(children[i]));
            children.erase(children.begin() + static_cast<std::ptrdiff_t>(i));
            --i;
        }
    }
    children.push_back(std::move(s));
    return true;
}

void ILStructure::sort_children() {
    std::sort(children.begin(), children.end(), [](const auto& a, const auto& b) { return a->start_offset < b->start_offset; });
    for (const auto& c : children) c->sort_children();
}

const ILStructure& ILStructure::get_innermost(int offset) const {
    for (const auto& c : children)
        if (c->start_offset <= offset && offset < c->end_offset) return c->get_innermost(offset);
    return *this;
}

}  // namespace cb::dotnet::decompiler::disassembler
