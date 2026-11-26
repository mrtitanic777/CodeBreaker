// dotnet/decompiler/disassembler/il_parser.hpp -- reading IL a piece at a time: an opcode, past its operand, a branch's or switch's
// targets, a method header's size; and the branch targets of a whole body.
//
// Ported from ILSpy (ICSharpCode.Decompiler.Disassembler: ILParser), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in
// dotnet/decompiler).
#pragma once

#include <climits>
#include <string>
#include <vector>

#include "dotnet/decompiler/metadata/il_opcodes.hpp"
#include "dotnet/decompiler/util/bit_set.hpp"
#include "dotnet/metadata/metadata_reader.hpp"

namespace cb::dotnet::decompiler::disassembler {

namespace srm = cb::dotnet::metadata;
using metadata::OperandType;
using srm::ILOpCode;

ILOpCode decode_op_code(srm::BlobReader& blob);
int operand_size(OperandType type);
// Past the operand; to the end of the body when it is cut short.
void skip_operand(srm::BlobReader& blob, ILOpCode op);
// The target offset; INT_MIN when the operand is cut short.
int decode_branch_target(srm::BlobReader& blob, ILOpCode op);
std::vector<int> decode_switch_targets(srm::BlobReader& blob);
std::u16string decode_user_string(srm::BlobReader& blob, const srm::MetadataReader& metadata);
int decode_index(srm::BlobReader& blob, ILOpCode op);
bool is_return(ILOpCode op);   // ret, endfilter, endfinally
// 1 for a tiny header, the fat header's size field times 4 for a fat one.
int get_header_size(srm::BlobReader body_block);
void set_branch_targets(srm::BlobReader& blob, util::BitSet& branch_targets);

}  // namespace cb::dotnet::decompiler::disassembler
