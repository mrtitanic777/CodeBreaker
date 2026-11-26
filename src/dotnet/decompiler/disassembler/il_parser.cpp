// dotnet/decompiler/disassembler/il_parser.cpp -- ILParser.
#include "dotnet/decompiler/disassembler/il_parser.hpp"

#include <format>

namespace cb::dotnet::decompiler::disassembler {

ILOpCode decode_op_code(srm::BlobReader& blob) {
    const std::uint8_t b = blob.read_byte();
    if (b == 0xFE && blob.remaining_bytes() >= 1) return static_cast<ILOpCode>(0xFE00 + blob.read_byte());
    return static_cast<ILOpCode>(b);
}

int operand_size(OperandType type) {
    switch (type) {
    case OperandType::I8: case OperandType::R: return 8;
    case OperandType::BrTarget: case OperandType::Field: case OperandType::Method: case OperandType::I: case OperandType::Sig:
    case OperandType::String: case OperandType::Tok: case OperandType::Type: case OperandType::ShortR:
        return 4;
    case OperandType::Switch: return 4;   // at least 4, usually more
    case OperandType::Variable: return 2;
    case OperandType::ShortVariable: case OperandType::ShortBrTarget: case OperandType::ShortI: return 1;
    default: return 0;
    }
}

void skip_operand(srm::BlobReader& blob, ILOpCode op) {
    const OperandType type = metadata::get_operand_type(op);
    int size;
    if (type == OperandType::Switch) {
        const std::uint32_t n = blob.remaining_bytes() >= 4 ? blob.read_uint32() : UINT32_MAX;
        size = n < INT_MAX / 4 ? static_cast<int>(n * 4) : INT_MAX;
    } else {
        size = operand_size(type);
    }
    if (size <= blob.remaining_bytes()) blob.set_offset(blob.offset() + size);
    else blob.set_offset(blob.length());   // a missing or partial operand at the end of the body
}

int decode_branch_target(srm::BlobReader& blob, ILOpCode op) {
    const int size = srm::get_branch_operand_size(op);
    if (size > blob.remaining_bytes()) return INT_MIN;
    const int rel = size == 4 ? blob.read_int32() : blob.read_sbyte();
    return static_cast<int>(static_cast<std::uint32_t>(rel) + static_cast<std::uint32_t>(blob.offset()));
}

std::vector<int> decode_switch_targets(srm::BlobReader& blob) {
    if (blob.remaining_bytes() < 4) {
        blob.set_offset(blob.length());
        return {};
    }
    std::uint32_t n = blob.read_uint32();
    bool overflow = false;
    if (n > static_cast<std::uint32_t>(blob.remaining_bytes() / 4)) {
        n = static_cast<std::uint32_t>(blob.remaining_bytes() / 4);
        overflow = true;
    }
    std::vector<int> targets(n);
    const std::uint32_t base = static_cast<std::uint32_t>(blob.offset()) + 4 * n;
    for (std::uint32_t i = 0; i < n; ++i) targets[i] = static_cast<int>(static_cast<std::uint32_t>(blob.read_int32()) + base);
    if (overflow) blob.set_offset(blob.length());
    return targets;
}

std::u16string decode_user_string(srm::BlobReader& blob, const srm::MetadataReader& metadata) {
    return metadata.get_user_string(srm::UserStringHandle(blob.read_uint32() & 0xFFFFFF));
}

int decode_index(srm::BlobReader& blob, ILOpCode op) {
    switch (metadata::get_operand_type(op)) {
    case OperandType::ShortVariable: return blob.read_byte();
    case OperandType::Variable: return blob.read_uint16();
    default: throw UsageError(std::format("{} not supported!", metadata::get_display_name(op)));
    }
}

bool is_return(ILOpCode op) { return op == ILOpCode::Ret || op == ILOpCode::Endfilter || op == ILOpCode::Endfinally; }

int get_header_size(srm::BlobReader body_block) {
    const std::uint8_t header = body_block.read_byte();
    if ((header & 3) == 3) {   // fat
        const std::uint16_t large = static_cast<std::uint16_t>(body_block.read_byte() << 8 | header);
        return static_cast<std::uint8_t>(large >> 12) * 4;
    }
    return 1;   // tiny
}

void set_branch_targets(srm::BlobReader& blob, util::BitSet& branch_targets) {
    while (blob.remaining_bytes() > 0) {
        const ILOpCode op = decode_op_code(blob);
        if (op == ILOpCode::Switch) {
            for (const int target : decode_switch_targets(blob))
                if (target >= 0 && target < blob.length()) branch_targets.set(target);
        } else if (srm::is_branch(op)) {
            const int target = decode_branch_target(blob, op);
            if (target >= 0 && target < blob.length()) branch_targets.set(target);
        } else {
            skip_operand(blob, op);
        }
    }
}

}  // namespace cb::dotnet::decompiler::disassembler
