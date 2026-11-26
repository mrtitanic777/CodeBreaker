// dotnet/decompiler/disassembler/method_body_disassembler.cpp -- MethodBodyDisassembler.
#include "dotnet/decompiler/disassembler/method_body_disassembler.hpp"

#include <format>

#include "dotnet/decompiler/disassembler/il_parser.hpp"
#include "dotnet/decompiler/disassembler/opcode_info.hpp"
#include "dotnet/decompiler/disassembler/reflection_disassembler.hpp"
#include "dotnet/decompiler/il/instruction_output_extensions.hpp"
#include "dotnet/decompiler/metadata/metadata_token_helpers.hpp"

namespace cb::dotnet::decompiler::disassembler {

void MethodBodyDisassembler::disassemble(const metadata::MetadataFile& module, srm::MethodDefinitionHandle handle) {
    module_ = &module;
    metadata_ = &module.metadata();
    generic_context_ = metadata::MetadataGenericContext(handle, module);
    signature_decoder_.emplace(module, output_);
    const srm::MethodDefinition method = metadata_->get_method_definition(handle);
    const int rva = method.relative_virtual_address();

    // the IL
    output_.write_line(std::format("// Method begins at RVA 0x{:04x}", static_cast<std::uint32_t>(rva)));
    if (rva == 0) {
        output_.write_line("// Header size: 0");
        output_.write_line("// Code size: 0 (0x0)");
        output_.write_line(".maxstack 0");
        output_.write_line();
        return;
    }
    std::optional<srm::MethodBodyBlock> body;
    srm::BlobReader body_block;
    try {
        body = module.get_method_body(rva);
        body_block = srm::BlobReader(module.get_section_data(rva));
    } catch (const srm::BadImageFormatError& e) {
        output_.write_line("// " + e.message());
        return;
    }
    srm::BlobReader blob = body->il_reader();
    const int header_size = get_header_size(body_block);
    output_.write_line(std::format("// Header size: {}", header_size));
    output_.write_line(std::format("// Code size: {0} (0x{0:x})", blob.length()));
    output_.write_line(std::format(".maxstack {}", body->max_stack()));
    const srm::CorHeader* cor = module.cor_header();
    const srm::MethodDefinitionHandle entry_point(cor != nullptr ? cor->entry_point_token_or_relative_virtual_address & 0xFFFFFF : 0);
    if (handle == entry_point) output_.write_line(".entrypoint");
    disassemble_locals_block(handle, *body);
    output_.write_line();

    if (detect_control_structure && blob.length() > 0) {
        blob.reset();
        util::BitSet branch_targets(blob.length());
        set_branch_targets(blob, branch_targets);
        blob.reset();
        const ILStructure root(module, handle, generic_context_, *body);
        write_structure_body(root, branch_targets, blob, rva + header_size);
    } else {
        while (blob.remaining_bytes() > 0) {
            cancellation_token_.throw_if_cancellation_requested();
            write_instruction(output_, module, handle, blob, rva);
        }
        write_exception_handlers(module, handle, *body);
    }
}

void MethodBodyDisassembler::disassemble_locals_block(srm::MethodDefinitionHandle, const srm::MethodBodyBlock& body) {
    if (body.local_signature().is_nil()) return;
    output_.write(".locals");
    write_metadata_token(body.local_signature(), true);
    if (body.local_variables_initialized()) output_.write(" init");
    const srm::StandaloneSignature sig = metadata_->get_standalone_signature(body.local_signature());
    std::vector<TypeWriter> signature;
    try {
        if (sig.get_kind() == srm::StandaloneSignatureKind::LocalVariables) signature = sig.decode_local_signature(*signature_decoder_, generic_context_);
        else output_.write(" /* wrong signature kind */");
    } catch (const srm::BadImageFormatError& e) {
        output_.write(" /* " + e.message() + " */");
    }
    output_.write(' ');
    output_.write_line("(");
    output_.indent();
    for (std::size_t index = 0; index < signature.size(); ++index) {
        output_.write_local_reference("[" + std::to_string(index) + "]", "loc_" + std::to_string(index), true);
        output_.write(' ');
        signature[index](ILNameSyntax::TypeName);
        if (index + 1 < signature.size()) output_.write(',');
        output_.write_line();
    }
    output_.unindent();
    output_.write_line(")");
}

void MethodBodyDisassembler::write_exception_handlers(const metadata::MetadataFile& module, srm::MethodDefinitionHandle handle,
                                                      const srm::MethodBodyBlock& body) {
    module_ = &module;
    metadata_ = &module.metadata();
    generic_context_ = metadata::MetadataGenericContext(handle, module);
    signature_decoder_.emplace(module, output_);
    if (body.exception_regions().empty()) return;
    output_.write_line();
    for (const srm::ExceptionRegion& eh : body.exception_regions()) {
        write_to(eh, module, generic_context_, output_);
        output_.write_line();
    }
}

void MethodBodyDisassembler::write_structure_header(const ILStructure& s) {
    switch (s.type) {
    case ILStructureType::Loop:
        output_.write("// loop start");
        if (s.loop_entry_point_offset >= 0) {
            output_.write(" (head: ");
            write_offset_reference(output_, s.loop_entry_point_offset);
            output_.write(')');
        }
        output_.write_line();
        break;
    case ILStructureType::Try:
        output_.write_line(".try");
        output_.write_line("{");
        break;
    case ILStructureType::Handler:
        switch (s.exception_handler.kind) {
        case srm::ExceptionRegionKind::Filter: break;   // a filter's handler block has no header
        case srm::ExceptionRegionKind::Catch:
            output_.write("catch");
            if (!s.exception_handler.catch_type.is_nil()) {
                output_.write(' ');
                il::write_to(s.exception_handler.catch_type, s.module, output_, s.generic_context, ILNameSyntax::TypeName);
            }
            output_.write_line();
            break;
        case srm::ExceptionRegionKind::Finally: output_.write_line("finally"); break;
        case srm::ExceptionRegionKind::Fault: output_.write_line("fault"); break;
        default: throw std::out_of_range("ExceptionHandler.Kind");
        }
        output_.write_line("{");
        break;
    case ILStructureType::Filter:
        output_.write_line("filter");
        output_.write_line("{");
        break;
    default: throw std::out_of_range("Type");
    }
    output_.indent();
}

void MethodBodyDisassembler::write_structure_body(const ILStructure& s, const util::BitSet& branch_targets, srm::BlobReader& body, int method_rva) {
    bool first = true;
    bool prev_was_branch = false;
    std::size_t child_index = 0;
    while (body.remaining_bytes() > 0 && body.offset() < s.end_offset) {
        cancellation_token_.throw_if_cancellation_requested();
        const int offset = body.offset();
        if (child_index < s.children.size() && s.children[child_index]->start_offset <= offset && offset < s.children[child_index]->end_offset) {
            const ILStructure& child = *s.children[child_index++];
            write_structure_header(child);
            write_structure_body(child, branch_targets, body, method_rva);
            write_structure_footer(child);
        } else {
            if (!first && (prev_was_branch || branch_targets[offset])) output_.write_line();   // a blank line after branches and before targets
            const ILOpCode op = decode_op_code(body);
            body.set_offset(offset);
            write_instruction(output_, *module_, s.method_handle, body, method_rva);
            prev_was_branch = srm::is_branch(op) || is_return(op) || op == ILOpCode::Throw || op == ILOpCode::Rethrow || op == ILOpCode::Switch;
        }
        first = false;
    }
}

void MethodBodyDisassembler::write_structure_footer(const ILStructure& s) {
    output_.unindent();
    switch (s.type) {
    case ILStructureType::Loop: output_.write_line("// end loop"); break;
    case ILStructureType::Try: output_.write_line("} // end .try"); break;
    case ILStructureType::Handler: output_.write_line("} // end handler"); break;
    case ILStructureType::Filter: output_.write_line("} // end filter"); break;
    default: throw std::out_of_range("Type");
    }
}

void MethodBodyDisassembler::write_instruction(ITextOutput& output, const metadata::MetadataFile& file, srm::MethodDefinitionHandle method_handle,
                                               srm::BlobReader& blob, int method_rva) {
    const srm::MetadataReader& metadata = file.metadata();
    const int offset = blob.offset();
    // (sequence points come with the debug information)
    const ILOpCode op = decode_op_code(blob);
    const OperandType type = metadata::get_operand_type(op);
    if (metadata::is_defined(op) && operand_size(type) <= blob.remaining_bytes()) {
        write_rva(blob, offset + method_rva, op);
        output.write_local_reference(offset_to_string(offset), offset, true);
        output.write(": ");
        write_op_code(op);
        switch (type) {
        case OperandType::BrTarget:
        case OperandType::ShortBrTarget: {
            output.write(' ');
            const int target = decode_branch_target(blob, op);
            output.write_local_reference(offset_to_string(target), target);
            break;
        }
        case OperandType::Field:
        case OperandType::Method:
        case OperandType::Sig:
        case OperandType::Type: {
            output.write(' ');
            const int token = blob.read_int32();
            std::optional<srm::EntityHandle> handle = metadata::try_as_entity_handle(token);
            try {
                if (handle) il::write_to(*handle, *module_, output, generic_context_);
            } catch (const srm::BadImageFormatError&) {
                handle.reset();
            }
            write_metadata_token(handle ? std::optional<srm::Handle>(*handle) : std::nullopt, token, true);
            break;
        }
        case OperandType::Tok: {
            output.write(' ');
            const int token = blob.read_int32();
            std::optional<srm::EntityHandle> handle = metadata::try_as_entity_handle(token);
            if (handle) {
                switch (handle->kind()) {
                case srm::HandleKind::MemberReference:
                    switch (metadata.get_member_reference(srm::MemberReferenceHandle(*handle)).get_kind()) {
                    case srm::MemberReferenceKind::Method: output.write("method "); break;
                    case srm::MemberReferenceKind::Field: output.write("field "); break;
                    }
                    break;
                case srm::HandleKind::FieldDefinition: output.write("field "); break;
                case srm::HandleKind::MethodDefinition: output.write("method "); break;
                default: break;
                }
            }
            try {
                if (handle) il::write_to(*handle, *module_, output, generic_context_);
            } catch (const srm::BadImageFormatError&) {
                handle.reset();
            }
            write_metadata_token(handle ? std::optional<srm::Handle>(*handle) : std::nullopt, token, true);
            break;
        }
        case OperandType::ShortI:
            output.write(' ');
            write_operand(output, static_cast<long long>(blob.read_sbyte()));
            break;
        case OperandType::I:
            output.write(' ');
            write_operand(output, static_cast<long long>(blob.read_int32()));
            break;
        case OperandType::I8:
            output.write(' ');
            write_operand(output, static_cast<long long>(blob.read_int64()));
            break;
        case OperandType::ShortR:
            output.write(' ');
            write_operand(output, blob.read_single());
            break;
        case OperandType::R:
            output.write(' ');
            write_operand(output, blob.read_double());
            break;
        case OperandType::String: {
            const int token = blob.read_int32();
            output.write(' ');
            std::optional<srm::UserStringHandle> user_string;
            std::u16string text;
            try {
                // (as upstream: MetadataTokens.UserStringHandle keeps the token's low 24 bits, whatever its table byte says)
                user_string = srm::UserStringHandle(static_cast<std::uint32_t>(token) & 0xFFFFFF);
                text = metadata.get_user_string(*user_string);
            } catch (const srm::BadImageFormatError&) {
                user_string.reset();
            }
            if (user_string) write_operand(output, std::u16string_view(text));
            write_metadata_token(user_string ? std::optional<srm::Handle>(*user_string) : std::nullopt, token, true);
            break;
        }
        case OperandType::Switch: {
            srm::BlobReader tmp = blob;
            const std::vector<int> targets = decode_switch_targets(blob);
            if (show_raw_rva_offset_and_bytes) output.write_line(" (");
            else output.write(" (");
            tmp.read_int32();
            for (std::size_t i = 0; i < targets.size(); ++i) {
                if (i > 0) {
                    if (show_raw_rva_offset_and_bytes) output.write_line(",");
                    else output.write(", ");
                }
                if (show_raw_rva_offset_and_bytes) {
                    output.write("/*              ");
                    const std::uint8_t b0 = tmp.read_byte(), b1 = tmp.read_byte(), b2 = tmp.read_byte(), b3 = tmp.read_byte();
                    output.write(std::format("{:02X}{:02X}{:02X}{:02X}", b0, b1, b2, b3));
                    output.write("         */ ");
                    output.write("                 ");
                }
                output.write_local_reference(offset_to_string(targets[i]), targets[i]);
            }
            output.write(")");
            break;
        }
        case OperandType::Variable: {
            output.write(' ');
            const int index = blob.read_uint16();
            if (op == ILOpCode::Ldloc || op == ILOpCode::Ldloca || op == ILOpCode::Stloc) write_variable_reference(output, metadata, method_handle, index);
            else write_parameter_reference(output, metadata, method_handle, index);
            break;
        }
        case OperandType::ShortVariable: {
            output.write(' ');
            const int index = blob.read_byte();
            if (op == ILOpCode::Ldloc_s || op == ILOpCode::Ldloca_s || op == ILOpCode::Stloc_s) write_variable_reference(output, metadata, method_handle, index);
            else write_parameter_reference(output, metadata, method_handle, index);
            break;
        }
        default: break;
        }
    } else {
        const auto value = static_cast<std::uint16_t>(op);
        if (value > 0xFF) {
            if (show_raw_rva_offset_and_bytes) {
                output.write("/* ");
                output.write(std::format("0x{:08X} {:02X}", static_cast<std::uint32_t>(offset + method_rva), value >> 8));
                output.write("                 */ ");
            }
            output.write_local_reference(offset_to_string(offset), offset, true);
            output.write(": ");
            // a 16-bit value as two emitbyte directives
            output.write_line(std::format(".emitbyte 0x{:x}", value >> 8));
            if (show_raw_rva_offset_and_bytes) {
                output.write("/* ");
                output.write(std::format("0x{:08X} {:02X}", static_cast<std::uint32_t>(offset + method_rva + 1), value & 0xFF));
                output.write("                 */ ");
            }
            output.write_local_reference(offset_to_string(offset + 1), offset + 1, true);
            output.write(": ");
            output.write(std::format(".emitbyte 0x{:x}", value & 0xFF));
        } else {
            if (show_raw_rva_offset_and_bytes) {
                output.write("/* ");
                output.write(std::format("0x{:08X} {:02X}", static_cast<std::uint32_t>(offset + method_rva), value & 0xFF));
                output.write("                 */ ");
            }
            output.write_local_reference(offset_to_string(offset), offset, true);
            output.write(": ");
            output.write(std::format(".emitbyte 0x{:x}", value & 0xFF));
        }
    }
    output.write_line();
}

void MethodBodyDisassembler::write_rva(srm::BlobReader blob, int offset, ILOpCode op) {
    if (!show_raw_rva_offset_and_bytes) return;
    output_.write("/* ");
    srm::BlobReader tmp = blob;
    if (op == ILOpCode::Switch) tmp.read_int32();
    else skip_operand(tmp, op);
    output_.write(std::format("0x{:08X} {:02X}", static_cast<std::uint32_t>(offset), static_cast<std::uint16_t>(op)));
    int spaces = static_cast<std::uint16_t>(op) > 0xFF ? 14 : 16;
    while (blob.offset() < tmp.offset()) {
        output_.write(std::format("{:02X}", blob.read_byte()));
        spaces -= 2;
    }
    if (spaces > 0) output_.write(std::string(static_cast<std::size_t>(spaces), ' '));
    output_.write(" */ ");
}

void MethodBodyDisassembler::write_op_code(ILOpCode op) {
    const OpCodeInfo info(op, std::string(metadata::get_display_name(op)));
    switch (op) {
    case ILOpCode::Ldarg_0: case ILOpCode::Ldarg_1: case ILOpCode::Ldarg_2: case ILOpCode::Ldarg_3: {
        output_.write_reference(info, true);
        const std::string index = info.name.substr(6);
        output_.write_local_reference(index, "param_" + index);
        break;
    }
    case ILOpCode::Ldloc_0: case ILOpCode::Ldloc_1: case ILOpCode::Ldloc_2: case ILOpCode::Ldloc_3: case ILOpCode::Stloc_0: case ILOpCode::Stloc_1:
    case ILOpCode::Stloc_2: case ILOpCode::Stloc_3: {
        output_.write_reference(info, true);
        const std::string index = info.name.substr(6);
        output_.write_local_reference(index, "loc_" + index);
        break;
    }
    default: output_.write_reference(info); break;
    }
}

void MethodBodyDisassembler::write_metadata_token(srm::EntityHandle handle, bool space_before) {
    write_metadata_token(std::optional<srm::Handle>(handle), static_cast<int>(srm::get_token(handle)), space_before);
}

void MethodBodyDisassembler::write_metadata_token(std::optional<srm::Handle> handle, int metadata_token, bool space_before) {
    disassembler::write_metadata_token(output_, *module_, handle, metadata_token, false, space_before, show_metadata_tokens, show_metadata_tokens_in_base10);
}

}  // namespace cb::dotnet::decompiler::disassembler
