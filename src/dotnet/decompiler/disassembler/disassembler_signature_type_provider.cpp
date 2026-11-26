// dotnet/decompiler/disassembler/disassembler_signature_type_provider.cpp -- DisassemblerSignatureTypeProvider.
#include "dotnet/decompiler/disassembler/disassembler_signature_type_provider.hpp"

#include <format>

#include "dotnet/decompiler/il/instruction_output_extensions.hpp"

namespace cb::dotnet::decompiler::disassembler {
namespace {

ILNameSyntax for_elements(ILNameSyntax syntax) {
    return syntax == ILNameSyntax::SignatureNoNamedTypeParameters ? syntax : ILNameSyntax::Signature;
}

}  // namespace

TypeWriter DisassemblerSignatureTypeProvider::get_array_type(TypeWriter element, const srm::ArrayShape& shape) {
    return [self = *this, element = std::move(element), shape](ILNameSyntax syntax) {
        element(for_elements(syntax));
        self.output_.write('[');
        for (int i = 0; i < shape.rank; ++i) {
            if (i > 0) self.output_.write(", ");
            const auto ui = static_cast<std::size_t>(i);
            if (ui < shape.lower_bounds.size() || ui < shape.sizes.size()) {
                int lower = 0;
                if (ui < shape.lower_bounds.size()) {
                    lower = shape.lower_bounds[ui];
                    self.output_.write(std::to_string(lower));
                }
                self.output_.write("...");
                if (ui < shape.sizes.size()) self.output_.write(std::to_string(lower + shape.sizes[ui] - 1));
            }
        }
        self.output_.write(']');
    };
}

TypeWriter DisassemblerSignatureTypeProvider::get_by_reference_type(TypeWriter element) {
    return [self = *this, element = std::move(element)](ILNameSyntax syntax) {
        element(for_elements(syntax));
        self.output_.write('&');
    };
}

TypeWriter DisassemblerSignatureTypeProvider::get_function_pointer_type(const srm::MethodSignature<TypeWriter>& signature) {
    return [self = *this, signature](ILNameSyntax syntax) {
        self.output_.write("method ");
        il::write_to(signature.header, self.output_);
        signature.return_type(syntax);
        self.output_.write(" *(");
        for (std::size_t i = 0; i < signature.parameter_types.size(); ++i) {
            if (i > 0) self.output_.write(", ");
            signature.parameter_types[i](syntax);
        }
        self.output_.write(')');
    };
}

TypeWriter DisassemblerSignatureTypeProvider::get_generic_instantiation(TypeWriter generic, const std::vector<TypeWriter>& arguments) {
    return [self = *this, generic = std::move(generic), arguments](ILNameSyntax syntax) {
        const ILNameSyntax inner = for_elements(syntax);
        generic(inner);
        self.output_.write('<');
        for (std::size_t i = 0; i < arguments.size(); ++i) {
            if (i > 0) self.output_.write(", ");
            arguments[i](inner);
        }
        self.output_.write('>');
    };
}

TypeWriter DisassemblerSignatureTypeProvider::get_generic_method_parameter(const metadata::MetadataGenericContext& context, int index) {
    return [self = *this, context, index](ILNameSyntax syntax) {
        self.output_.write("!!");
        self.write_type_parameter(context.get_generic_method_type_parameter_handle_or_null(index), index, syntax);
    };
}

TypeWriter DisassemblerSignatureTypeProvider::get_generic_type_parameter(const metadata::MetadataGenericContext& context, int index) {
    return [self = *this, context, index](ILNameSyntax syntax) {
        self.output_.write("!");
        self.write_type_parameter(context.get_generic_type_parameter_handle_or_null(index), index, syntax);
    };
}

void DisassemblerSignatureTypeProvider::write_type_parameter(srm::GenericParameterHandle parameter, int index, ILNameSyntax syntax) const {
    if (parameter.is_nil() || syntax == ILNameSyntax::SignatureNoNamedTypeParameters) {
        output_.write(std::to_string(index));
        return;
    }
    const srm::GenericParameter p = metadata_.get_generic_parameter(parameter);
    if (p.name().is_nil()) output_.write(std::to_string(p.index()));
    else output_.write(escape(metadata_.get_string(p.name())));
}

TypeWriter DisassemblerSignatureTypeProvider::get_modified_type(TypeWriter modifier, TypeWriter unmodified, bool is_required) {
    return [self = *this, modifier = std::move(modifier), unmodified = std::move(unmodified), is_required](ILNameSyntax syntax) {
        unmodified(syntax);
        self.output_.write(is_required ? " modreq" : " modopt");
        self.output_.write('(');
        modifier(ILNameSyntax::TypeName);
        self.output_.write(')');
    };
}

TypeWriter DisassemblerSignatureTypeProvider::get_pinned_type(TypeWriter element) {
    return [self = *this, element = std::move(element)](ILNameSyntax syntax) {
        element(for_elements(syntax));
        self.output_.write(" pinned");
    };
}

TypeWriter DisassemblerSignatureTypeProvider::get_pointer_type(TypeWriter element) {
    return [self = *this, element = std::move(element)](ILNameSyntax syntax) {
        element(for_elements(syntax));
        self.output_.write('*');
    };
}

TypeWriter DisassemblerSignatureTypeProvider::get_primitive_type(srm::PrimitiveTypeCode code) {
    using P = srm::PrimitiveTypeCode;
    const char* name = nullptr;
    switch (code) {
    case P::SByte: name = "int8"; break;
    case P::Int16: name = "int16"; break;
    case P::Int32: name = "int32"; break;
    case P::Int64: name = "int64"; break;
    case P::Byte: name = "uint8"; break;
    case P::UInt16: name = "uint16"; break;
    case P::UInt32: name = "uint32"; break;
    case P::UInt64: name = "uint64"; break;
    case P::Single: name = "float32"; break;
    case P::Double: name = "float64"; break;
    case P::Void: name = "void"; break;
    case P::Boolean: name = "bool"; break;
    case P::String: name = "string"; break;
    case P::Char: name = "char"; break;
    case P::Object: name = "object"; break;
    case P::IntPtr: name = "native int"; break;
    case P::UIntPtr: name = "native uint"; break;
    case P::TypedReference: name = "typedref"; break;
    default: throw std::out_of_range("typeCode");
    }
    return [self = *this, name](ILNameSyntax) { self.output_.write(name); };
}

TypeWriter DisassemblerSignatureTypeProvider::get_sz_array_type(TypeWriter element) {
    return [self = *this, element = std::move(element)](ILNameSyntax syntax) {
        element(for_elements(syntax));
        self.output_.write('[');
        self.output_.write(']');
    };
}

TypeWriter DisassemblerSignatureTypeProvider::write_type_handle(srm::EntityHandle handle, std::uint8_t raw_type_kind) {
    return [self = *this, handle, raw_type_kind](ILNameSyntax) {
        switch (raw_type_kind) {
        case 0x00: break;
        case 0x11: self.output_.write("valuetype "); break;
        case 0x12: self.output_.write("class "); break;
        default: throw srm::BadImageFormatError(0, std::format("Unexpected rawTypeKind: {} (0x{:x})", raw_type_kind, raw_type_kind));
        }
        il::write_to(handle, self.module_, self.output_, metadata::MetadataGenericContext());
    };
}

TypeWriter DisassemblerSignatureTypeProvider::get_type_from_definition(const srm::MetadataReader&, srm::TypeDefinitionHandle handle, std::uint8_t raw_type_kind) {
    return write_type_handle(handle, raw_type_kind);
}

TypeWriter DisassemblerSignatureTypeProvider::get_type_from_reference(const srm::MetadataReader&, srm::TypeReferenceHandle handle, std::uint8_t raw_type_kind) {
    return write_type_handle(handle, raw_type_kind);
}

TypeWriter DisassemblerSignatureTypeProvider::get_type_from_specification(const srm::MetadataReader& reader, const metadata::MetadataGenericContext& context,
                                                                          srm::TypeSpecificationHandle handle, std::uint8_t) {
    return reader.get_type_specification(handle).decode_signature(*this, context);
}

}  // namespace cb::dotnet::decompiler::disassembler
