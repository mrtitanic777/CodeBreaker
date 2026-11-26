// dotnet/decompiler/disassembler/reflection_disassembler.cpp -- ReflectionDisassembler.
#include "dotnet/decompiler/disassembler/reflection_disassembler.hpp"

#include <bit>
#include <cmath>
#include <format>

#include "dotnet/decompiler/il/instruction_output_extensions.hpp"
#include "dotnet/decompiler/metadata/custom_attribute_decoder.hpp"
#include "dotnet/decompiler/metadata/metadata_extensions.hpp"
#include "dotnet/decompiler/srm_extensions.hpp"
#include "dotnet/decompiler/util/char_info.hpp"
#include "dotnet/metadata/custom_attribute_decoder.hpp"

namespace cb::dotnet::decompiler::disassembler {
namespace {

using HK = srm::HandleKind;

// EnumNameCollection: (value, name) in order; a null name is a value that is known but not written.
using EnumNames = std::vector<std::pair<long long, const char*>>;

const EnumNames kMethodAttributeFlags = {{0x0020, "final"},  {0x0080, "hidebysig"}, {0x0800, "specialname"}, {0x2000, nullptr},
                                         {0x0008, "export"}, {0x1000, "rtspecialname"}, {0x8000, "reqsecobj"}, {0x0100, "newslot"},
                                         {0x0200, "strict"}, {0x0400, "abstract"}, {0x0040, "virtual"}, {0x0010, "static"}, {0x4000, nullptr}};
const EnumNames kMethodVisibility = {{1, "private"}, {2, "famandassem"}, {3, "assembly"}, {4, "family"}, {5, "famorassem"}, {6, "public"}};
const EnumNames kCallingConvention = {{1, "unmanaged cdecl"}, {2, "unmanaged stdcall"}, {3, "unmanaged thiscall"}, {4, "unmanaged fastcall"},
                                      {5, "vararg"}, {0, nullptr}};
const EnumNames kMethodCodeType = {{0, "cil"}, {1, "native"}, {2, "optil"}, {3, "runtime"}};
const EnumNames kMethodImpl = {{0x0020, "synchronized"}, {0x0008, "noinlining"}, {0x0040, "nooptimization"}, {0x0080, "preservesig"},
                               {0x1000, "internalcall"}, {0x0010, "forwardref"}, {0x0100, "aggressiveinlining"}};
const EnumNames kFieldVisibility = {{1, "private"}, {2, "famandassem"}, {3, "assembly"}, {4, "family"}, {5, "famorassem"}, {6, "public"}};
const EnumNames kFieldAttributes = {{0x0010, "static"}, {0x0040, "literal"}, {0x0020, "initonly"}, {0x0200, "specialname"}, {0x0400, "rtspecialname"},
                                    {0x0080, "notserialized"}};
const EnumNames kPropertyAttributes = {{0x0200, "specialname"}, {0x0400, "rtspecialname"}, {0x1000, "hasdefault"}};
const EnumNames kEventAttributes = {{0x0200, "specialname"}, {0x0400, "rtspecialname"}};
const EnumNames kTypeVisibility = {{1, "public"}, {0, "private"}, {2, "nested public"}, {3, "nested private"}, {5, "nested assembly"}, {4, "nested family"},
                                   {6, "nested famandassem"}, {7, "nested famorassem"}};
const EnumNames kTypeLayout = {{0x00, "auto"}, {0x08, "sequential"}, {0x10, "explicit"}};
const EnumNames kTypeStringFormat = {{0x20000, "auto"}, {0x00000, "ansi"}, {0x10000, "unicode"}};
const EnumNames kTypeAttributes = {{0x0080, "abstract"}, {0x0100, "sealed"}, {0x0400, "specialname"}, {0x1000, "import"}, {0x2000, "serializable"},
                                   {0x4000, "windowsruntime"}, {0x100000, "beforefieldinit"}, {0x40000, nullptr}};

void write_flags(long long val, const EnumNames& names, ITextOutput& output) {
    long long tested = 0;
    for (const auto& [key, name] : names) {
        tested |= key;
        if ((val & key) != 0 && name != nullptr) {
            output.write(name);
            output.write(' ');
        }
    }
    if ((val & ~tested) != 0) output.write(std::format("flags({:04x}) ", val & ~tested));
}

void write_enum(long long val, const EnumNames& names, ITextOutput& output) {
    for (const auto& [key, name] : names) {
        if (key == val) {
            if (name != nullptr) {
                output.write(name);
                output.write(' ');
            }
            return;
        }
    }
    if (val != 0) output.write(std::format("flags({:04x}) ", val));
}

std::string primitive_type_code_to_string(srm::PrimitiveTypeCode code) {
    using P = srm::PrimitiveTypeCode;
    switch (code) {
    case P::Boolean: return "bool";
    case P::Byte: return "uint8";
    case P::SByte: return "int8";
    case P::Char: return "char";
    case P::Int16: return "int16";
    case P::UInt16: return "uint16";
    case P::Int32: return "int32";
    case P::UInt32: return "uint32";
    case P::Int64: return "int64";
    case P::UInt64: return "uint64";
    case P::Single: return "float32";
    case P::Double: return "float64";
    case P::String: return "string";
    case P::Object: return "object";
    default: return "unknown";
    }
}

// System.Reflection.PortableExecutable.Subsystem and CorFlags as their ToString writes them.
std::string subsystem_name(std::uint16_t s) {
    switch (s) {
    case 0: return "Unknown";
    case 1: return "Native";
    case 2: return "WindowsGui";
    case 3: return "WindowsCui";
    case 5: return "OS2Cui";
    case 7: return "PosixCui";
    case 8: return "NativeWindows";
    case 9: return "WindowsCEGui";
    case 10: return "EfiApplication";
    case 11: return "EfiBootServiceDriver";
    case 12: return "EfiRuntimeDriver";
    case 13: return "EfiRom";
    case 14: return "Xbox";
    case 16: return "WindowsBootApplication";
    default: return std::to_string(s);
    }
}

std::string cor_flags_name(std::uint32_t flags) {
    static const std::pair<std::uint32_t, const char*> kNames[] = {{0x1, "ILOnly"}, {0x2, "Requires32Bit"}, {0x4, "ILLibrary"}, {0x8, "StrongNameSigned"},
                                                                    {0x10, "NativeEntryPoint"}, {0x10000, "TrackDebugData"}, {0x20000, "Prefers32Bit"}};
    if (flags == 0) return "0";
    std::uint32_t rest = flags;
    std::string out;
    for (const auto& [bit, name] : kNames) {
        if ((flags & bit) == 0) continue;
        if (!out.empty()) out += ", ";
        out += name;
        rest &= ~bit;
    }
    return rest != 0 ? std::to_string(flags) : out;
}

// A string as .NET's Guid(string) reads it (only the "D" and "B" forms here); the empty GUID for anything else.
srm::Guid parse_guid(std::string_view text) {
    std::string hex;
    for (const char c : text)
        if (std::isxdigit(static_cast<unsigned char>(c))) hex += c;
    srm::Guid g;
    if (hex.size() != 32) return g;
    auto byte = [&](std::size_t i) { return static_cast<std::uint8_t>(std::stoul(hex.substr(i * 2, 2), nullptr, 16)); };
    const std::uint8_t b[16] = {byte(3), byte(2), byte(1), byte(0), byte(5), byte(4), byte(7), byte(6),
                                byte(8), byte(9), byte(10), byte(11), byte(12), byte(13), byte(14), byte(15)};
    std::copy(std::begin(b), std::end(b), g.bytes.begin());
    return g;
}

template <class List>
auto to_vector(const List& list) {
    std::vector<std::decay_t<decltype(*list.begin())>> v;
    for (const auto h : list) v.push_back(h);
    return v;
}

}  // namespace

// ---- the security and attribute value provider ------------------------------------------------------------------------------

struct ReflectionDisassembler::SecurityType {
    srm::PrimitiveTypeCode code = static_cast<srm::PrimitiveTypeCode>(0);
    std::optional<std::string> name;
};

class ReflectionDisassembler::SecurityDeclarationDecoder {
public:
    SecurityDeclarationDecoder(ITextOutput& output, metadata::IAssemblyResolver* resolver, const metadata::MetadataFile& module)
        : output_(output), resolver_(resolver), module_(module) {}

    SecurityType get_primitive_type(srm::PrimitiveTypeCode code) { return {code, std::nullopt}; }
    SecurityType get_system_type() { return {static_cast<srm::PrimitiveTypeCode>(0), "type"}; }
    SecurityType get_sz_array_type(SecurityType element) {
        return {element.code, element.name.value_or(primitive_type_code_to_string(element.code)) + "[]"};
    }
    SecurityType get_type_from_definition(const srm::MetadataReader& reader, srm::TypeDefinitionHandle handle, std::uint8_t) {
        const std::string full = get_full_type_name(handle, reader).full_name();
        srm::PrimitiveTypeCode code{};
        if (is_enum(reader.get_type_definition(handle), reader, code)) return {code, "enum " + full};
        return {static_cast<srm::PrimitiveTypeCode>(0), full};
    }
    SecurityType get_type_from_reference(const srm::MetadataReader& reader, srm::TypeReferenceHandle handle, std::uint8_t) {
        const std::string full = get_full_type_name(handle, reader).full_name();
        const std::optional<std::string> containing = declaring_module(reader, handle);
        std::string qualified = containing ? full + ", " + *containing : full;
        srm::PrimitiveTypeCode code = static_cast<srm::PrimitiveTypeCode>(0);
        const auto [target, resolved] = resolve_type(qualified);
        if (target != nullptr) {
            if (!is_enum(target->metadata().get_type_definition(resolved), target->metadata(), code)) code = static_cast<srm::PrimitiveTypeCode>(0);
            else qualified = "enum " + qualified;
        }
        return {code, qualified};
    }
    SecurityType get_type_from_serialized_name(const std::string& name) {
        if (resolver_ == nullptr) throw metadata::EnumUnderlyingTypeResolveError();
        const auto [containing, handle] = resolve_type(name);
        if (handle.is_nil()) throw metadata::EnumUnderlyingTypeResolveError();
        srm::PrimitiveTypeCode code{};
        if (is_enum(containing->metadata().get_type_definition(handle), containing->metadata(), code)) return {code, "enum " + name};
        return {static_cast<srm::PrimitiveTypeCode>(0), name};
    }
    srm::PrimitiveTypeCode get_underlying_enum_type(const SecurityType& type) { return type.code; }
    bool is_system_type(const SecurityType& type) { return type.name == "type"; }

private:
    std::optional<std::string> declaring_module(const srm::MetadataReader& reader, srm::TypeReferenceHandle handle) {
        const srm::TypeReference tr = reader.get_type_reference(handle);
        switch (tr.resolution_scope().kind()) {
        case HK::TypeReference: return declaring_module(reader, srm::TypeReferenceHandle(tr.resolution_scope()));
        case HK::AssemblyReference: {
            std::string name;
            return metadata::try_get_full_assembly_name(reader.get_assembly_reference(srm::AssemblyReferenceHandle(tr.resolution_scope())), reader, name)
                       ? std::optional<std::string>(name)
                       : std::nullopt;
        }
        case HK::ModuleReference: return std::string(reader.get_string(reader.get_module_reference(srm::ModuleReferenceHandle(tr.resolution_scope())).name()));
        default: return std::nullopt;
        }
    }

    static srm::TypeDefinitionHandle find_type(const metadata::MetadataFile& module, const std::vector<std::string>& name) {
        const srm::MetadataReader& md = module.metadata();
        srm::NamespaceDefinition current = md.get_namespace_definition_root();
        std::optional<std::vector<srm::TypeDefinitionHandle>> types;
        for (std::size_t i = 0; i < name.size(); ++i) {
            const std::string& identifier = name[i];
            if (types) {
                bool restarted = true;
                while (restarted) {
                    restarted = false;
                    for (const srm::TypeDefinitionHandle t : *types) {
                        const srm::TypeDefinition td = md.get_type_definition(t);
                        if (identifier == md.get_string(td.name())) {
                            if (i + 1 == name.size()) return t;
                            types = td.get_nested_types();
                            restarted = true;   // upstream's "goto restart": the same identifier against the nested types
                            break;
                        }
                    }
                }
            } else {
                srm::NamespaceDefinitionHandle next;
                for (const srm::NamespaceDefinitionHandle ns : current.namespace_definitions())
                    if (md.get_namespace_definition(ns).name() == identifier) {
                        next = ns;
                        break;
                    }
                if (!next.is_nil()) {
                    current = md.get_namespace_definition(next);
                } else {
                    types = current.type_definitions();
                    --i;
                }
            }
        }
        return {};
    }

    std::pair<std::shared_ptr<metadata::MetadataFile>, srm::TypeDefinitionHandle> resolve_type(const std::string& type_name) {
        const std::size_t comma = type_name.find(", ");
        const std::string type_part = type_name.substr(0, comma);
        std::vector<std::string> parts;
        for (std::size_t start = 0;;) {
            const std::size_t dot = type_part.find('.', start);
            parts.push_back(type_part.substr(start, dot == std::string::npos ? std::string::npos : dot - start));
            if (dot == std::string::npos) break;
            start = dot + 1;
        }
        std::shared_ptr<metadata::MetadataFile> containing;
        srm::TypeDefinitionHandle handle;
        if (comma != std::string::npos && resolver_ != nullptr) containing = resolver_->resolve(metadata::AssemblyNameReference::parse(type_name.substr(comma + 2)));
        if (containing) {
            handle = find_type(*containing, parts);
        } else {
            // a name without its assembly: this assembly, then mscorlib
            handle = find_type(module_, parts);
            containing = std::shared_ptr<metadata::MetadataFile>(std::shared_ptr<metadata::MetadataFile>(), const_cast<metadata::MetadataFile*>(&module_));
            if (handle.is_nil()) {
                if (const auto mscorlib = resolve_mscorlib()) {
                    handle = find_type(*mscorlib, parts);
                    containing = mscorlib;
                }
            }
        }
        return {containing, handle};
    }

    std::shared_ptr<metadata::MetadataFile> resolve_mscorlib() {
        if (mscorlib_) return mscorlib_;
        if (resolver_ == nullptr) return nullptr;
        mscorlib_ = resolver_->resolve(metadata::AssemblyNameReference::parse("mscorlib"));
        return mscorlib_;
    }

    ITextOutput& output_;
    metadata::IAssemblyResolver* resolver_;
    const metadata::MetadataFile& module_;
    std::shared_ptr<metadata::MetadataFile> mscorlib_;
};

// ---- the disassembler --------------------------------------------------------------------------------------------------------

void write_metadata_token(ITextOutput& output, const metadata::MetadataFile& module, std::optional<srm::Handle> handle, int metadata_token, bool space_after,
                          bool space_before, bool show_metadata_tokens, bool base10) {
    // without a handle (it could not be read) the token is always written
    if (show_metadata_tokens || !handle) {
        if (space_before) output.write(' ');
        output.write("/* ");
        const std::string text = base10 ? std::to_string(metadata_token) : std::format("{:08X}", static_cast<std::uint32_t>(metadata_token));
        if (!handle || !is_entity_handle(*handle)) output.write(text);
        else output.write_reference(module, *handle, text, "metadata");
        output.write(" */");
        if (space_after) output.write(' ');
    } else if (space_before && space_after) {
        output.write(' ');
    }
}

ReflectionDisassembler::ReflectionDisassembler(ITextOutput& output, util::CancellationToken cancellation_token)
    : ReflectionDisassembler(output, std::make_unique<MethodBodyDisassembler>(output, cancellation_token), cancellation_token) {}

ReflectionDisassembler::ReflectionDisassembler(ITextOutput& output, std::unique_ptr<MethodBodyDisassembler> method_body_disassembler,
                                               util::CancellationToken cancellation_token)
    : output_(output), cancellation_token_(cancellation_token), method_body_disassembler_(std::move(method_body_disassembler)) {}

template <class H>
std::vector<H> ReflectionDisassembler::process(const metadata::MetadataFile& module, std::vector<H> items) {
    return entity_processor != nullptr ? entity_processor->process(module, std::move(items)) : items;
}

void ReflectionDisassembler::disassemble_method(const metadata::MetadataFile& module, srm::MethodDefinitionHandle handle) {
    const metadata::MetadataGenericContext context(handle, module);
    output_.write_reference(module, handle, ".method", "decompile", true);
    output_.write(" ");
    disassemble_method_header_internal(module, handle, context);
    disassemble_method_block(module, handle, context);
}

void ReflectionDisassembler::disassemble_method_header(const metadata::MetadataFile& module, srm::MethodDefinitionHandle handle) {
    const metadata::MetadataGenericContext context(handle, module);
    output_.write_reference(module, handle, ".method", "decompile", true);
    output_.write(" ");
    disassemble_method_header_internal(module, handle, context);
}

void ReflectionDisassembler::disassemble_method_header_internal(const metadata::MetadataFile& module, srm::MethodDefinitionHandle handle,
                                                                const metadata::MetadataGenericContext& context) {
    const srm::MetadataReader& metadata = module.metadata();
    const MethodBodyDisassembler& body = *method_body_disassembler_;
    write_metadata_token(output_, module, srm::Handle(handle), static_cast<int>(srm::get_token(handle)), true, false, body.show_metadata_tokens,
                         body.show_metadata_tokens_in_base10);
    const srm::MethodDefinition method = metadata.get_method_definition(handle);
    //    .method public hidebysig  specialname
    //               instance default class [mscorlib]System.IO.TextWriter get_BaseWriter ()  cil managed
    write_enum(method.attributes() & 0x0007, kMethodVisibility, output_);
    write_flags(method.attributes() & ~0x0007, kMethodAttributeFlags, output_);
    const bool compiler_controlled = (method.attributes() & 0x0007) == 0;
    if (compiler_controlled) output_.write("privatescope ");
    if ((method.attributes() & 0x2000) == 0x2000) {   // PinvokeImpl
        output_.write("pinvokeimpl");
        const srm::MethodImport info = method.get_import();
        if (!info.module.is_nil()) {
            const srm::ModuleReference module_ref = metadata.get_module_reference(info.module);
            output_.write("(\"" + escape_string(metadata.get_string(module_ref.name())) + "\"");
            if (!info.name.is_nil() && metadata.get_string(info.name) != metadata.get_string(method.name()))
                output_.write(" as \"" + escape_string(metadata.get_string(info.name)) + "\"");
            if ((info.attributes & 0x0001) == 0x0001) output_.write(" nomangle");
            switch (info.attributes & 0x0006) {
            case 0x0002: output_.write(" ansi"); break;
            case 0x0006: output_.write(" autochar"); break;
            case 0x0004: output_.write(" unicode"); break;
            default: break;
            }
            if ((info.attributes & 0x0040) == 0x0040) output_.write(" lasterr");
            switch (info.attributes & 0x0700) {
            case 0x0200: output_.write(" cdecl"); break;
            case 0x0500: output_.write(" fastcall"); break;
            case 0x0300: output_.write(" stdcall"); break;
            case 0x0400: output_.write(" thiscall"); break;
            case 0x0100: output_.write(" winapi"); break;
            default: break;
            }
            output_.write(')');
        }
        output_.write(' ');
    }
    output_.write_line();
    output_.indent();
    std::optional<srm::MethodSignature<TypeWriter>> signature;
    try {
        DisassemblerSignatureTypeProvider provider(module, output_);
        signature = method.decode_signature(provider, context);
        if (signature->header.has_explicit_this()) output_.write("instance explicit ");
        else if (signature->header.is_instance()) output_.write("instance ");
        write_enum(static_cast<long long>(signature->header.calling_convention()), kCallingConvention, output_);
        signature->return_type(ILNameSyntax::Signature);
    } catch (const srm::BadImageFormatError&) {
        signature.reset();
        output_.write("<bad signature>");
    }
    output_.write(' ');
    const auto parameters = method.get_parameters();
    if (!parameters.empty()) {
        const srm::Parameter first = metadata.get_parameter(parameters[0]);
        if (first.sequence_number() == 0) {
            const srm::BlobHandle marshalling = first.get_marshalling_descriptor();
            if (!marshalling.is_nil()) write_marshal_info(metadata.get_blob_reader(marshalling));
        }
    }
    if (compiler_controlled)
        output_.write(escape(std::format("{}$PST{:08X}", metadata.get_string(method.name()), srm::get_token(handle))));
    else
        output_.write(escape(metadata.get_string(method.name())));
    write_type_parameters(output_, module, context, method.get_generic_parameters());
    // ( params )
    output_.write(" (");
    if (signature && !signature->parameter_types.empty()) {
        output_.write_line();
        output_.indent();
        write_parameters(metadata, parameters, *signature);
        output_.unindent();
    }
    output_.write(") ");
    // cil managed
    write_enum(method.impl_attributes() & 0x0003, kMethodCodeType, output_);
    if ((method.impl_attributes() & 0x0004) == 0) output_.write("managed ");
    else output_.write("unmanaged ");
    write_flags(method.impl_attributes() & ~(0x0003 | 0x0004), kMethodImpl, output_);
    output_.unindent();
}

void ReflectionDisassembler::disassemble_method_block(const metadata::MetadataFile& module, srm::MethodDefinitionHandle handle,
                                                      const metadata::MetadataGenericContext& context) {
    const srm::MetadataReader& metadata = module.metadata();
    const srm::MethodDefinition method = metadata.get_method_definition(handle);
    open_block(is_in_type_);
    write_attributes(module, method.get_custom_attributes());
    for (const srm::MethodImplementationHandle h : get_method_implementations(handle, metadata)) {
        const srm::MethodImplementation impl = metadata.get_method_implementation(h);
        output_.write(".override method ");
        il::write_to(impl.method_declaration(), module, output_, context);
        output_.write_line();
    }
    for (const srm::GenericParameterHandle p : method.get_generic_parameters()) write_generic_parameters_and_attributes(module, context, p);
    for (const srm::ParameterHandle p : method.get_parameters()) write_parameter_attributes(module, p);
    write_security_declarations(module, method.get_declarative_security_attributes());
    if (has_body(method)) method_body_disassembler_->disassemble(module, handle);
    const srm::TypeDefinition declaring = metadata.get_type_definition(method.get_declaring_type());
    close_block("end of method " + escape(metadata.get_string(declaring.name())) + "::" + escape(metadata.get_string(method.name())));
}

void ReflectionDisassembler::write_security_declarations(const metadata::MetadataFile& module, const std::vector<srm::DeclarativeSecurityAttributeHandle>& declarations) {
    static constexpr const char* kActions[] = {nullptr, "request", "demand", "assert", "deny", "permitonly", "linkcheck", "inheritcheck", "reqmin",
                                               "reqopt", "reqrefuse", "prejitgrant", "prejitdeny", "noncasdemand", "noncaslinkdemand", "noncasinheritance"};
    for (const srm::DeclarativeSecurityAttributeHandle h : declarations) {
        output_.write(".permissionset ");
        const srm::DeclarativeSecurityAttribute decl = module.metadata().get_declarative_security_attribute(h);
        const std::uint16_t action = decl.action();
        if (action >= 1 && action <= 15) output_.write(kActions[action]);
        else output_.write(std::to_string(action));
        srm::BlobReader blob = module.metadata().get_blob_reader(decl.permission_set());
        if (assembly_resolver == nullptr) {
            output_.write(" = ");
            write_blob(blob);
            output_.write_line();
        } else if (static_cast<char>(blob.read_byte()) != '.') {
            blob.reset();
            output_.write_line();
            output_.indent();
            output_.write("bytearray");
            write_blob(blob);
            output_.write_line();
            output_.unindent();
        } else {
            TextOutputWithRollback rollback(output_);
            try {
                try_decode_security_declaration(rollback, blob, module);
                rollback.commit();
            } catch (const srm::BadImageFormatError&) {
                blob.reset();
                output_.write(" = ");
                write_blob(blob);
                output_.write_line();
            } catch (const metadata::EnumUnderlyingTypeResolveError&) {
                blob.reset();
                output_.write(" = ");
                write_blob(blob);
                output_.write_line();
            }
        }
    }
}

void ReflectionDisassembler::try_decode_security_declaration(TextOutputWithRollback& output, srm::BlobReader blob, const metadata::MetadataFile& module) {
    output.write_line(" = {");
    output.indent();
    std::optional<std::string> current_assembly_name;
    if (module.metadata().is_assembly()) {
        try {
            current_assembly_name = std::string(module.metadata().get_string(module.metadata().get_assembly_definition().name()));
        } catch (const srm::BadImageFormatError&) {
            current_assembly_name = "<ERR: invalid assembly name>";
        }
    }
    const int count = blob.read_compressed_integer();
    for (int i = 0; i < count; ++i) {
        const std::string full_type_name = blob.read_serialized_string().value_or(std::string());
        std::vector<std::string> name_parts;
        for (std::size_t start = 0;;) {
            const std::size_t at = full_type_name.find(", ", start);
            name_parts.push_back(full_type_name.substr(start, at == std::string::npos ? std::string::npos : at - start));
            if (at == std::string::npos) break;
            start = at + 2;
        }
        if (name_parts.size() < 2 || name_parts[1] == current_assembly_name) {
            output.write("class ");
            output.write(escape(full_type_name));
        } else {
            output.write('[');
            output.write(name_parts[1]);
            output.write(']');
            output.write(name_parts[0]);
        }
        output.write(" = {");
        blob.read_compressed_integer();   // ? (the standard seems to be wrong here; as Cecil)
        const int arg_count = blob.read_compressed_integer();
        SecurityDeclarationDecoder provider(output, assembly_resolver.get(), module);
        metadata::CustomAttributeDecoder<SecurityDeclarationDecoder> decoder(provider, module.metadata(), true);
        const auto arguments = decoder.decode_named_arguments(blob, arg_count);
        if (arg_count > 0) {
            output.write_line();
            output.indent();
        }
        for (const auto& argument : arguments) {
            output.write(argument.kind == srm::CustomAttributeNamedArgumentKind::Field ? "field " : "property ");
            output.write(argument.type.name.value_or(primitive_type_code_to_string(argument.type.code)));
            output.write(" " + escape(argument.name.value_or(std::string())) + " = ");
            write_value(output, argument.type, argument.value);
            output.write_line();
        }
        if (arg_count > 0) output.unindent();
        output.write('}');
        if (i + 1 < count) output.write(',');
        output.write_line();
    }
    output.unindent();
    output.write_line("}");
}

namespace {

// A simple attribute value (upstream's object) as a metadata constant, for write_operand.
template <class V>
srm::ConstantValue as_constant(const V& value) {
    return std::visit(
        [](const auto& x) -> srm::ConstantValue {
            using X = std::decay_t<decltype(x)>;
            if constexpr (std::is_same_v<X, std::string>) return util::utf16(x);
            else if constexpr (std::is_arithmetic_v<X> || std::is_same_v<X, char16_t>) return x;
            else return std::monostate{};
        },
        value);
}

}  // namespace

template <class Value>
void ReflectionDisassembler::write_value(ITextOutput& output, const SecurityType& type, const Value& value) {
    using Arg = srm::CustomAttributeTypedArgument<SecurityType>;
    auto simple = [&](const auto& v, const std::string& type_name) {
        if (type_name == "string") {
            std::string s = std::holds_alternative<std::string>(v) ? std::get<std::string>(v) : std::string();
            std::string e = escape_string(s), out;
            for (const char c : e) out += c == '\'' ? std::string("\\'") : std::string(1, c);
            output.write("'" + out + "'");
        } else if (type_name == "type") {
            const SecurityType& info = std::get<SecurityType>(v);
            const std::string n = info.name.value_or("");
            output.write(n.starts_with("enum ") ? n.substr(5) : n);
        } else {
            write_operand(output, as_constant(v));
        }
    };
    if (const auto* boxed = std::get_if<std::shared_ptr<const Arg>>(&value)) {
        output.write("object(");
        write_value(output, (*boxed)->type, (*boxed)->value);
        output.write(")");
    } else if (const auto* array = std::get_if<std::vector<Arg>>(&value)) {
        const std::string element_type = type.name && !type.name->starts_with("enum ") ? type.name->substr(0, type.name->size() - 2)
                                                                                         : primitive_type_code_to_string(type.code);
        output.write(element_type);
        output.write("[");
        output.write(std::to_string(array->size()));
        output.write("](");
        bool first = true;
        for (const Arg& item : *array) {
            if (!first) output.write(" ");
            if (const auto* boxed_item = std::get_if<std::shared_ptr<const Arg>>(&item.value)) write_value(output, (*boxed_item)->type, (*boxed_item)->value);
            else simple(item.value, element_type);
            first = false;
        }
        output.write(")");
    } else {
        const std::string type_name = type.name && !type.name->starts_with("enum ") ? *type.name : primitive_type_code_to_string(type.code);
        output.write(type_name);
        output.write("(");
        simple(value, type_name);
        output.write(")");
    }
}

void ReflectionDisassembler::write_marshal_info(srm::BlobReader marshal_info) {
    output_.write("marshal(");
    write_native_type(marshal_info);
    output_.write(") ");
}

void ReflectionDisassembler::write_native_type(srm::BlobReader& blob) {
    const std::uint8_t type = blob.read_byte();
    switch (type) {
    case 0x66: case 0x50: break;   // None, Max
    case 0x02: output_.write("bool"); break;
    case 0x03: output_.write("int8"); break;
    case 0x04: output_.write("unsigned int8"); break;
    case 0x05: output_.write("int16"); break;
    case 0x06: output_.write("unsigned int16"); break;
    case 0x07: output_.write("int32"); break;
    case 0x08: output_.write("unsigned int32"); break;
    case 0x09: output_.write("int64"); break;
    case 0x0a: output_.write("unsigned int64"); break;
    case 0x0b: output_.write("float32"); break;
    case 0x0c: output_.write("float64"); break;
    case 0x14: output_.write("lpstr"); break;
    case 0x1f: output_.write("int"); break;
    case 0x20: output_.write("unsigned int"); break;
    case 0x26: output_.write("Func"); break;
    case 0x2a: {   // ARRAY
        if (blob.remaining_bytes() > 0) write_native_type(blob);
        output_.write('[');
        const int size_parameter_index = blob.try_read_compressed_integer().value_or(-1);
        const int size = blob.try_read_compressed_integer().value_or(-1);
        const int size_parameter_multiplier = blob.try_read_compressed_integer().value_or(-1);
        if (size >= 0) output_.write(std::to_string(size));
        if (size_parameter_index >= 0 && size_parameter_multiplier != 0) {
            output_.write(" + ");
            output_.write(std::to_string(size_parameter_index));
        }
        output_.write(']');
        break;
    }
    case 0x0f: output_.write("currency"); break;
    case 0x13: output_.write("bstr"); break;
    case 0x15: output_.write("lpwstr"); break;
    case 0x16: output_.write("lptstr"); break;
    case 0x17: output_.write(std::format("fixed sysstring[{}]", blob.read_compressed_integer())); break;
    case 0x19: output_.write("iunknown"); break;
    case 0x1a: output_.write("idispatch"); break;
    case 0x1b: output_.write("struct"); break;
    case 0x1c: output_.write("interface"); break;
    case 0x1d: {   // SafeArray
        output_.write("safearray ");
        if (blob.remaining_bytes() > 0) {
            const std::uint8_t element = blob.read_byte();
            switch (element) {
            case 0: break;
            case 2: output_.write("int16"); break;
            case 3: output_.write("int32"); break;
            case 4: output_.write("float32"); break;
            case 5: output_.write("float64"); break;
            case 6: output_.write("currency"); break;
            case 7: output_.write("date"); break;
            case 8: output_.write("bstr"); break;
            case 9: output_.write("idispatch"); break;
            case 10: output_.write("error"); break;
            case 11: output_.write("bool"); break;
            case 12: output_.write("variant"); break;
            case 13: output_.write("iunknown"); break;
            case 14: output_.write("decimal"); break;
            case 16: output_.write("int8"); break;
            case 17: output_.write("unsigned int8"); break;
            case 18: output_.write("unsigned int16"); break;
            case 19: output_.write("unsigned int32"); break;
            case 22: output_.write("int"); break;
            case 23: output_.write("unsigned int"); break;
            default: output_.write(std::to_string(element)); break;
            }
        }
        break;
    }
    case 0x1e:   // FixedArray
        output_.write("fixed array");
        output_.write(std::format("[{}]", blob.try_read_compressed_integer().value_or(0)));
        if (blob.remaining_bytes() > 0) {
            output_.write(' ');
            write_native_type(blob);
        }
        break;
    case 0x22: output_.write("byvalstr"); break;
    case 0x23: output_.write("ansi bstr"); break;
    case 0x24: output_.write("tbstr"); break;
    case 0x25: output_.write("variant bool"); break;
    case 0x28: output_.write("as any"); break;
    case 0x2b: output_.write("lpstruct"); break;
    case 0x2c: {   // CustomMarshaler
        const std::string guid_value = blob.read_serialized_string().value_or("");
        const std::string unmanaged_type = blob.read_serialized_string().value_or("");
        const std::string managed_type = blob.read_serialized_string().value_or("");
        const std::string cookie = blob.read_serialized_string().value_or("");
        const srm::Guid guid = guid_value.empty() ? srm::Guid{} : parse_guid(guid_value);
        output_.write(std::format("custom(\"{}\", \"{}\"", escape_string(managed_type), escape_string(cookie)));
        if (guid != srm::Guid{} || !unmanaged_type.empty()) output_.write(std::format(", \"{}\", \"{}\"", guid.to_string(), escape_string(unmanaged_type)));
        output_.write(')');
        break;
    }
    case 0x2d: output_.write("error"); break;
    default: output_.write(std::to_string(type)); break;
    }
}

template <class Params>
void ReflectionDisassembler::write_parameters(const srm::MetadataReader& metadata, const Params& parameters, const srm::MethodSignature<TypeWriter>& signature) {
    int i = 0;
    const int offset = signature.header.is_instance() ? 1 : 0;
    auto param_ref = [&](int index) { return "param_" + std::to_string(index + offset); };
    for (const srm::ParameterHandle h : parameters) {
        const srm::Parameter p = metadata.get_parameter(h);
        if (p.sequence_number() == 0) continue;   // the return type's
        // the gaps in the parameter list
        while (i < p.sequence_number() - 1) {
            if (i > 0) {
                output_.write(',');
                output_.write_line();
            }
            signature.parameter_types.at(static_cast<std::size_t>(i))(ILNameSyntax::Signature);
            output_.write(' ');
            output_.write_local_reference("''", param_ref(i), true);
            ++i;
        }
        if (i > 0) {
            output_.write(',');
            output_.write_line();
        }
        if ((p.attributes() & 0x0001) == 0x0001) output_.write("[in] ");
        if ((p.attributes() & 0x0002) == 0x0002) output_.write("[out] ");
        if ((p.attributes() & 0x0010) == 0x0010) output_.write("[opt] ");
        signature.parameter_types.at(static_cast<std::size_t>(i))(ILNameSyntax::Signature);
        output_.write(' ');
        const srm::BlobHandle md = p.get_marshalling_descriptor();
        if (!md.is_nil()) write_marshal_info(metadata.get_blob_reader(md));
        output_.write_local_reference(escape(metadata.get_string(p.name())), param_ref(i), true);
        ++i;
    }
    // the remaining parameter types, unnamed
    while (i < signature.required_parameter_count) {
        if (i > 0) {
            output_.write(',');
            output_.write_line();
        }
        signature.parameter_types.at(static_cast<std::size_t>(i))(ILNameSyntax::Signature);
        output_.write(' ');
        output_.write_local_reference("''", param_ref(i), true);
        ++i;
    }
    output_.write_line();
}

void ReflectionDisassembler::write_generic_parameters_and_attributes(const metadata::MetadataFile& module, const metadata::MetadataGenericContext& context,
                                                                     srm::GenericParameterHandle handle) {
    const srm::MetadataReader& metadata = module.metadata();
    const srm::GenericParameter p = metadata.get_generic_parameter(handle);
    if (!p.get_custom_attributes().empty()) {
        output_.write(std::format(".param type {}", metadata.get_string(p.name())));
        output_.write_line();
        output_.indent();
        write_attributes(module, p.get_custom_attributes());
        output_.unindent();
    }
    for (const srm::GenericParameterConstraintHandle ch : p.get_constraints()) {
        const srm::GenericParameterConstraint constraint = metadata.get_generic_parameter_constraint(ch);
        if (!constraint.get_custom_attributes().empty()) {
            output_.write(std::format(".param constraint {}, ", metadata.get_string(p.name())));
            il::write_to(constraint.type(), module, output_, context, ILNameSyntax::TypeName);
            output_.write_line();
            output_.indent();
            write_attributes(module, constraint.get_custom_attributes());
            output_.unindent();
        }
    }
}

void ReflectionDisassembler::write_parameter_attributes(const metadata::MetadataFile& module, srm::ParameterHandle handle) {
    const srm::MetadataReader& metadata = module.metadata();
    const srm::Parameter p = metadata.get_parameter(handle);
    if (p.get_default_value().is_nil() && p.get_custom_attributes().empty()) return;
    output_.write(std::format(".param [{}]", p.sequence_number()));
    if (!p.get_default_value().is_nil()) {
        output_.write(" = ");
        write_constant(metadata, metadata.get_constant(p.get_default_value()));
    }
    output_.write_line();
    output_.indent();
    write_attributes(module, p.get_custom_attributes());
    output_.unindent();
}

void ReflectionDisassembler::write_constant(const srm::MetadataReader& metadata, const srm::Constant& constant) {
    if (constant.type_code() == srm::ConstantTypeCode::NullReference) {
        output_.write("nullref");
        return;
    }
    srm::BlobReader blob = metadata.get_blob_reader(constant.value());
    srm::ConstantValue value;
    try {
        value = blob.read_constant(constant.type_code());
    } catch (const std::out_of_range&) {
        output_.write(std::format("/* Constant with invalid typecode: {} */", static_cast<int>(constant.type_code())));
        return;
    }
    if (const auto* s = std::get_if<std::u16string>(&value)) {
        write_operand(output_, std::u16string_view(*s));
        return;
    }
    // the value's .NET type, as IL names it
    static constexpr const char* kTypeNames[] = {"System.Boolean", "System.Char", "System.SByte", "System.Byte", "System.Int16", "System.UInt16",
                                                 "System.Int32", "System.UInt32", "System.Int64", "System.UInt64", "System.Single", "System.Double"};
    const std::size_t index = value.index();   // 1 (bool) .. 12 (double), as ConstantValue lists them
    const std::optional<std::string> type_name = index >= 1 && index <= 12 ? primitive_type_name(kTypeNames[index - 1]) : std::nullopt;
    output_.write(type_name.value_or(""));
    output_.write('(');
    if (const auto* f = std::get_if<float>(&value); f && (std::isnan(*f) || std::isinf(*f)))
        output_.write(std::format("0x{:08x}", std::bit_cast<std::uint32_t>(*f)));
    else if (const auto* d = std::get_if<double>(&value); d && (std::isnan(*d) || std::isinf(*d)))
        output_.write(std::format("0x{:016x}", std::bit_cast<std::uint64_t>(*d)));
    else
        write_operand(output_, value);
    output_.write(')');
}

void ReflectionDisassembler::disassemble_field(const metadata::MetadataFile& module, srm::FieldDefinitionHandle handle) {
    const srm::MetadataReader& metadata = module.metadata();
    const srm::FieldDefinition field = metadata.get_field_definition(handle);
    const char section_prefix = disassemble_field_header_internal(module, handle, metadata, field);
    output_.write_line();
    const auto attributes = field.get_custom_attributes();
    if (!attributes.empty()) {
        output_.mark_fold_start();
        write_attributes(module, attributes);
        output_.mark_fold_end();
    }
    if ((field.attributes() & 0x0100) == 0) return;   // HasFieldRVA
    // the field's data, II.16.3.1 of ECMA-335 6th edition
    const int rva = field.get_relative_virtual_address();
    const int section_index = module.get_containing_section_index(rva);
    if (section_index < 0) {
        output_.write_line(std::format("// RVA {:08X} invalid (not in any section)", static_cast<std::uint32_t>(rva)));
        return;
    }
    ByteView init;
    try {
        init = get_initial_value(field, module);
    } catch (const srm::BadImageFormatError& e) {
        init = {};
        output_.write_line(std::format("// .data {}_{:08X} = {}", section_prefix, static_cast<std::uint32_t>(field.get_relative_virtual_address()), e.message()));
    }
    if (init.empty()) return;
    const srm::SectionHeader& section = module.section_headers()[static_cast<std::size_t>(section_index)];
    output_.write(".data ");
    if (section.name == ".text") output_.write("cil ");
    else if (section.name == ".tls") output_.write("tls ");
    else if (section.name != ".data") output_.write("/* " + section.name + " */ ");
    output_.write(std::format("{}_{:08X} = bytearray ", section_prefix, static_cast<std::uint32_t>(rva)));
    write_blob(srm::BlobReader(init));
    output_.write_line();
}

void ReflectionDisassembler::disassemble_field_header(const metadata::MetadataFile& module, srm::FieldDefinitionHandle handle) {
    const srm::MetadataReader& metadata = module.metadata();
    disassemble_field_header_internal(module, handle, metadata, metadata.get_field_definition(handle));
}

char ReflectionDisassembler::disassemble_field_header_internal(const metadata::MetadataFile& module, srm::FieldDefinitionHandle handle,
                                                               const srm::MetadataReader& metadata, const srm::FieldDefinition& field) {
    const MethodBodyDisassembler& body = *method_body_disassembler_;
    output_.write_reference(module, handle, ".field", "decompile", true);
    write_metadata_token(output_, module, srm::Handle(handle), static_cast<int>(srm::get_token(handle)), true, true, body.show_metadata_tokens,
                         body.show_metadata_tokens_in_base10);
    const int offset = field.get_offset();
    if (offset > -1) output_.write("[" + std::to_string(offset) + "] ");
    write_enum(field.attributes() & 0x0007, kFieldVisibility, output_);
    constexpr int has_x = 0x8000 | 0x1000 | 0x0100;   // HasDefault | HasFieldMarshal | HasFieldRVA
    write_flags(field.attributes() & ~(0x0007 | has_x), kFieldAttributes, output_);
    DisassemblerSignatureTypeProvider provider(module, output_);
    const TypeWriter signature = field.decode_signature(provider, metadata::MetadataGenericContext(field.get_declaring_type(), module));
    const srm::BlobHandle marshalling = field.get_marshalling_descriptor();
    if (!marshalling.is_nil()) write_marshal_info(metadata.get_blob_reader(marshalling));
    signature(ILNameSyntax::Signature);
    output_.write(' ');
    output_.write(escape(metadata.get_string(field.name())));
    char section_prefix = 'D';
    if ((field.attributes() & 0x0100) != 0) {
        const int rva = field.get_relative_virtual_address();
        section_prefix = get_rva_section_prefix(module, rva);
        output_.write(std::format(" at {}_{:08X}", section_prefix, static_cast<std::uint32_t>(rva)));
    }
    const srm::ConstantHandle default_value = field.get_default_value();
    if (!default_value.is_nil()) {
        output_.write(" = ");
        write_constant(metadata, metadata.get_constant(default_value));
    }
    return section_prefix;
}

char ReflectionDisassembler::get_rva_section_prefix(const metadata::MetadataFile& module, int rva) {
    const auto* pe = dynamic_cast<const metadata::PEFile*>(&module);
    if (pe == nullptr) throw UsageError("Cannot get RVA section prefix from module");
    const int index = pe->reader().get_containing_section_index(static_cast<std::uint32_t>(rva));
    if (index < 0) return 'D';
    const std::string& name = pe->reader().section_headers()[static_cast<std::size_t>(index)].name;
    if (name == ".tls") return 'T';
    if (name == ".text") return 'I';
    return 'D';
}

void ReflectionDisassembler::disassemble_property(const metadata::MetadataFile& module, srm::PropertyDefinitionHandle property) {
    const srm::MetadataReader& metadata = module.metadata();
    const srm::PropertyDefinition definition = metadata.get_property_definition(property);
    const srm::PropertyAccessors accessors = disassemble_property_header_internal(module, property, metadata, definition);
    open_block(false);
    write_attributes(module, definition.get_custom_attributes());
    write_nested_method(".get", module, accessors.getter);
    write_nested_method(".set", module, accessors.setter);
    for (const srm::MethodDefinitionHandle m : accessors.others) write_nested_method(".other", module, m);
    close_block();
}

void ReflectionDisassembler::disassemble_property_header(const metadata::MetadataFile& module, srm::PropertyDefinitionHandle property) {
    const srm::MetadataReader& metadata = module.metadata();
    disassemble_property_header_internal(module, property, metadata, metadata.get_property_definition(property));
}

srm::PropertyAccessors ReflectionDisassembler::disassemble_property_header_internal(const metadata::MetadataFile& module, srm::PropertyDefinitionHandle handle,
                                                                                    const srm::MetadataReader& metadata, const srm::PropertyDefinition& property) {
    const MethodBodyDisassembler& body = *method_body_disassembler_;
    output_.write_reference(module, handle, ".property", "decompile", true);
    write_metadata_token(output_, module, srm::Handle(handle), static_cast<int>(srm::get_token(handle)), true, true, body.show_metadata_tokens,
                         body.show_metadata_tokens_in_base10);
    write_flags(property.attributes(), kPropertyAttributes, output_);
    srm::PropertyAccessors accessors = property.get_accessors();
    const srm::TypeDefinitionHandle declaring = metadata.get_method_definition(get_any(accessors)).get_declaring_type();
    DisassemblerSignatureTypeProvider provider(module, output_);
    const auto signature = property.decode_signature(provider, metadata::MetadataGenericContext(declaring, module));
    if (signature.header.is_instance()) output_.write("instance ");
    signature.return_type(ILNameSyntax::Signature);
    output_.write(' ');
    output_.write(escape(metadata.get_string(property.name())));
    output_.write('(');
    if (!signature.parameter_types.empty()) {
        const auto parameters = metadata.get_method_definition(get_any(accessors)).get_parameters();
        const std::uint32_t count = accessors.getter.is_nil() ? parameters.size() - 1 : parameters.size();
        std::vector<srm::ParameterHandle> taken;
        for (std::uint32_t i = 0; i < count && i < parameters.size(); ++i) taken.push_back(parameters[i]);
        output_.write_line();
        output_.indent();
        write_parameters(metadata, taken, signature);
        output_.unindent();
    }
    output_.write(')');
    return accessors;
}

void ReflectionDisassembler::write_nested_method(std::string_view keyword, const metadata::MetadataFile& module, srm::MethodDefinitionHandle method) {
    if (method.is_nil()) return;
    output_.write(keyword);
    output_.write(' ');
    il::write_to(method, module, output_, metadata::MetadataGenericContext());
    output_.write_line();
}

void ReflectionDisassembler::disassemble_event(const metadata::MetadataFile& module, srm::EventDefinitionHandle handle) {
    const srm::EventDefinition event = module.metadata().get_event_definition(handle);
    const srm::EventAccessors accessors = event.get_accessors();
    disassemble_event_header_internal(module, handle, event, accessors);
    open_block(false);
    write_attributes(module, event.get_custom_attributes());
    write_nested_method(".addon", module, accessors.adder);
    write_nested_method(".removeon", module, accessors.remover);
    write_nested_method(".fire", module, accessors.raiser);
    for (const srm::MethodDefinitionHandle m : accessors.others) write_nested_method(".other", module, m);
    close_block();
}

void ReflectionDisassembler::disassemble_event_header(const metadata::MetadataFile& module, srm::EventDefinitionHandle handle) {
    const srm::EventDefinition event = module.metadata().get_event_definition(handle);
    disassemble_event_header_internal(module, handle, event, event.get_accessors());
}

void ReflectionDisassembler::disassemble_event_header_internal(const metadata::MetadataFile& module, srm::EventDefinitionHandle handle,
                                                               const srm::EventDefinition& event, const srm::EventAccessors& accessors) {
    const srm::MetadataReader& md = module.metadata();
    srm::TypeDefinitionHandle declaring;
    if (!accessors.adder.is_nil()) declaring = md.get_method_definition(accessors.adder).get_declaring_type();
    else if (!accessors.remover.is_nil()) declaring = md.get_method_definition(accessors.remover).get_declaring_type();
    else declaring = md.get_method_definition(accessors.raiser).get_declaring_type();
    const MethodBodyDisassembler& body = *method_body_disassembler_;
    output_.write_reference(module, handle, ".event", "decompile", true);
    write_metadata_token(output_, module, srm::Handle(handle), static_cast<int>(srm::get_token(handle)), true, true, body.show_metadata_tokens,
                         body.show_metadata_tokens_in_base10);
    write_flags(event.attributes(), kEventAttributes, output_);
    DisassemblerSignatureTypeProvider provider(module, output_);
    TypeWriter signature;
    switch (event.type().kind()) {
    case HK::TypeDefinition: signature = provider.get_type_from_definition(md, srm::TypeDefinitionHandle(event.type()), 0); break;
    case HK::TypeReference: signature = provider.get_type_from_reference(md, srm::TypeReferenceHandle(event.type()), 0); break;
    case HK::TypeSpecification:
        signature = provider.get_type_from_specification(md, metadata::MetadataGenericContext(declaring, module), srm::TypeSpecificationHandle(event.type()), 0);
        break;
    default: throw srm::BadImageFormatError(0, "Expected a TypeDef, TypeRef or TypeSpec handle!");
    }
    signature(ILNameSyntax::TypeName);
    output_.write(' ');
    output_.write(escape(md.get_string(event.name())));
}

void ReflectionDisassembler::disassemble_type(const metadata::MetadataFile& module, srm::TypeDefinitionHandle type) {
    const srm::MetadataReader& md = module.metadata();
    const srm::TypeDefinition definition = md.get_type_definition(type);
    const metadata::MetadataGenericContext context(type, module);
    disassemble_type_header_internal(module, type, definition, context);
    const std::vector<srm::InterfaceImplementationHandle> interfaces = process(module, definition.get_interface_implementations());
    if (!interfaces.empty()) {
        output_.indent();
        bool first = true;
        for (const srm::InterfaceImplementationHandle i : interfaces) {
            if (!first) output_.write_line(",");
            output_.write(first ? "implements " : "           ");
            first = false;
            il::write_to(md.get_interface_implementation(i).interface_(), module, output_, context, ILNameSyntax::TypeName);
        }
        output_.write_line();
        output_.unindent();
    }
    output_.write_line("{");
    output_.indent();
    const bool old_is_in_type = is_in_type_;
    is_in_type_ = true;
    write_attributes(module, definition.get_custom_attributes());
    write_security_declarations(module, definition.get_declarative_security_attributes());
    for (const srm::GenericParameterHandle tp : definition.get_generic_parameters()) write_generic_parameters_and_attributes(module, context, tp);
    const srm::TypeLayout layout = definition.get_layout();
    if (!layout.is_default()) {
        output_.write_line(std::format(".pack {}", layout.packing_size));
        output_.write_line(std::format(".size {}", layout.size));
        output_.write_line();
    }
    for (const srm::InterfaceImplementationHandle h : interfaces) {
        const srm::InterfaceImplementation iface = md.get_interface_implementation(h);
        const auto attributes = iface.get_custom_attributes();
        if (!attributes.empty()) {
            output_.write(".interfaceimpl type ");
            il::write_to(iface.interface_(), module, output_, context, ILNameSyntax::TypeName);
            output_.write_line();
            output_.indent();
            write_attributes(module, attributes);
            output_.unindent();
            output_.write_line();
        }
    }
    const std::vector<srm::TypeDefinitionHandle> nested = process(module, definition.get_nested_types());
    if (!nested.empty()) {
        output_.write_line("// Nested Types");
        for (const srm::TypeDefinitionHandle n : nested) {
            cancellation_token_.throw_if_cancellation_requested();
            disassemble_type(module, n);
            output_.write_line();
        }
        output_.write_line();
    }
    const std::vector<srm::FieldDefinitionHandle> fields = process(module, to_vector(definition.get_fields()));
    if (!fields.empty()) {
        output_.write_line("// Fields");
        for (const srm::FieldDefinitionHandle f : fields) {
            cancellation_token_.throw_if_cancellation_requested();
            disassemble_field(module, f);
        }
        output_.write_line();
    }
    const std::vector<srm::MethodDefinitionHandle> methods = process(module, to_vector(definition.get_methods()));
    if (!methods.empty()) {
        output_.write_line("// Methods");
        for (const srm::MethodDefinitionHandle m : methods) {
            cancellation_token_.throw_if_cancellation_requested();
            disassemble_method(module, m);
            output_.write_line();
        }
    }
    const std::vector<srm::EventDefinitionHandle> events = process(module, to_vector(definition.get_events()));
    if (!events.empty()) {
        output_.write_line("// Events");
        for (const srm::EventDefinitionHandle e : events) {
            cancellation_token_.throw_if_cancellation_requested();
            disassemble_event(module, e);
            output_.write_line();
        }
        output_.write_line();
    }
    const std::vector<srm::PropertyDefinitionHandle> properties = process(module, to_vector(definition.get_properties()));
    if (!properties.empty()) {
        output_.write_line("// Properties");
        for (const srm::PropertyDefinitionHandle p : properties) {
            cancellation_token_.throw_if_cancellation_requested();
            disassemble_property(module, p);
        }
        output_.write_line();
    }
    close_block("end of class " + (!definition.get_declaring_type().is_nil() ? std::string(md.get_string(definition.name()))
                                                                               : get_full_type_name(definition, md).to_string()));
    is_in_type_ = old_is_in_type;
}

void ReflectionDisassembler::disassemble_type_header(const metadata::MetadataFile& module, srm::TypeDefinitionHandle type) {
    const srm::TypeDefinition definition = module.metadata().get_type_definition(type);
    disassemble_type_header_internal(module, type, definition, metadata::MetadataGenericContext(type, module));
}

void ReflectionDisassembler::disassemble_type_header_internal(const metadata::MetadataFile& module, srm::TypeDefinitionHandle handle,
                                                              const srm::TypeDefinition& type, const metadata::MetadataGenericContext& context) {
    const MethodBodyDisassembler& body = *method_body_disassembler_;
    output_.write_reference(module, handle, ".class", "decompile", true);
    write_metadata_token(output_, module, srm::Handle(handle), static_cast<int>(srm::get_token(handle)), true, true, body.show_metadata_tokens,
                         body.show_metadata_tokens_in_base10);
    const std::uint32_t attrs = type.attributes();
    if ((attrs & 0x20) == 0x20) output_.write("interface ");
    write_enum(attrs & 0x07, kTypeVisibility, output_);
    write_enum(attrs & 0x18, kTypeLayout, output_);
    write_enum(attrs & 0x30000, kTypeStringFormat, output_);
    constexpr std::uint32_t masks = 0x20 | 0x07 | 0x18 | 0x30000;   // ClassSemantics | Visibility | Layout | StringFormat
    write_flags(attrs & ~masks, kTypeAttributes, output_);
    output_.write(type.get_declaring_type().is_nil() ? metadata::to_il_name_string(get_full_type_name(type, module.metadata()))
                                                     : escape(module.metadata().get_string(type.name())));
    write_type_parameters(output_, module, context, type.get_generic_parameters());
    output_.mark_fold_start("...", !expand_member_definitions && is_in_type_, is_in_type_);
    output_.write_line();
    const srm::EntityHandle base = get_base_type_or_nil(type);
    if (!base.is_nil()) {
        output_.indent();
        output_.write("extends ");
        il::write_to(base, module, output_, context, ILNameSyntax::TypeName);
        output_.write_line();
        output_.unindent();
    }
}

void ReflectionDisassembler::write_type_parameters(ITextOutput& output, const metadata::MetadataFile& module, const metadata::MetadataGenericContext& context,
                                                   const std::vector<srm::GenericParameterHandle>& parameters) {
    if (parameters.empty()) return;
    output.write('<');
    const srm::MetadataReader& md = module.metadata();
    for (std::size_t i = 0; i < parameters.size(); ++i) {
        if (i > 0) output.write(", ");
        const srm::GenericParameter gp = md.get_generic_parameter(parameters[i]);
        const std::uint16_t a = gp.attributes();
        if ((a & 0x0004) == 0x0004) output.write("class ");
        else if ((a & 0x0008) == 0x0008) output.write("valuetype ");
        if ((a & kAllowByRefLike) == kAllowByRefLike) output.write("byreflike ");
        if ((a & 0x0010) == 0x0010) output.write(".ctor ");
        const auto constraints = gp.get_constraints();
        if (!constraints.empty()) {
            output.write('(');
            for (std::size_t j = 0; j < constraints.size(); ++j) {
                if (j > 0) output.write(", ");
                il::write_to(md.get_generic_parameter_constraint(constraints[j]).type(), module, output, context, ILNameSyntax::TypeName);
            }
            output.write(") ");
        }
        if ((a & 0x0002) == 0x0002) output.write('-');
        else if ((a & 0x0001) == 0x0001) output.write('+');
        output.write(escape(md.get_string(gp.name())));
    }
    output.write('>');
}

void ReflectionDisassembler::write_attributes(const metadata::MetadataFile& module, const std::vector<srm::CustomAttributeHandle>& attributes) {
    const srm::MetadataReader& md = module.metadata();
    const MethodBodyDisassembler& body = *method_body_disassembler_;
    for (const srm::CustomAttributeHandle a : process(module, attributes)) {
        output_.write(".custom ");
        write_metadata_token(output_, module, srm::Handle(a), static_cast<int>(srm::get_token(a)), true, false, body.show_metadata_tokens,
                             body.show_metadata_tokens_in_base10);
        const srm::CustomAttribute attr = md.get_custom_attribute(a);
        il::write_to(attr.constructor(), module, output_, metadata::MetadataGenericContext());
        if (!attr.value().is_nil()) {
            output_.write(" = ");
            if (decode_custom_attribute_blobs) write_decoded_custom_attribute_blob(attr, module);
            else write_blob(attr.value(), md);
        }
        output_.write_line();
    }
}

void ReflectionDisassembler::write_decoded_custom_attribute_blob(const srm::CustomAttribute& attr, const metadata::MetadataFile& module) {
    std::optional<srm::CustomAttributeValue<SecurityType>> value;
    try {
        SecurityDeclarationDecoder provider(output_, assembly_resolver.get(), module);
        value = attr.decode_value(provider);
    } catch (const srm::BadImageFormatError&) {
        output_.write("/* Could not decode attribute value */ ");
        write_blob(attr.value(), module.metadata());
        return;
    }
    output_.write("{");
    output_.indent();
    for (const auto& arg : value->fixed_arguments) {
        output_.write_line();
        write_value(output_, arg.type, arg.value);
    }
    for (const auto& arg : value->named_arguments) {
        output_.write_line();
        output_.write(arg.kind == srm::CustomAttributeNamedArgumentKind::Field ? "field " : "property ");
        output_.write(arg.type.name.value_or(primitive_type_code_to_string(arg.type.code)));
        output_.write(" " + escape(arg.name.value_or(std::string())) + " = ");
        write_value(output_, arg.type, arg.value);
    }
    output_.write_line();
    output_.unindent();
    output_.write("}");
}

void ReflectionDisassembler::write_blob(srm::BlobHandle blob, const srm::MetadataReader& metadata) { write_blob(metadata.get_blob_reader(blob)); }

void ReflectionDisassembler::write_blob(srm::BlobReader reader) {
    output_.write("(");
    if (reader.length() > 0) {
        output_.indent();
        for (int i = 0; i < reader.length(); ++i) {
            if (i % 16 == 0 && i < reader.length() - 1) output_.write_line();
            else output_.write(' ');
            output_.write(std::format("{:02x}", reader.read_byte()));
        }
        output_.write_line();
        output_.unindent();
    }
    output_.write(")");
}

void ReflectionDisassembler::open_block(bool default_collapsed) {
    output_.mark_fold_start("...", !expand_member_definitions && default_collapsed, true);
    output_.write_line();
    output_.write_line("{");
    output_.indent();
}

void ReflectionDisassembler::close_block(std::optional<std::string_view> comment) {
    output_.unindent();
    output_.write("}");
    if (comment) output_.write(" // " + std::string(*comment));
    output_.mark_fold_end();
    output_.write_line();
}

void ReflectionDisassembler::disassemble_namespace(std::string_view name_space, const metadata::MetadataFile& module,
                                                   const std::vector<srm::TypeDefinitionHandle>& types) {
    if (!name_space.empty()) {
        output_.write(".namespace " + escape(name_space));
        open_block(false);
    }
    const bool old_is_in_type = is_in_type_;
    is_in_type_ = true;
    for (const srm::TypeDefinitionHandle td : types) {
        cancellation_token_.throw_if_cancellation_requested();
        disassemble_type(module, td);
        output_.write_line();
    }
    if (!name_space.empty()) {
        close_block();
        is_in_type_ = old_is_in_type;
    }
}

void ReflectionDisassembler::write_assembly_header(const metadata::MetadataFile& module) {
    const srm::MetadataReader& md = module.metadata();
    if (!md.is_assembly()) return;
    output_.write(".assembly ");
    const srm::AssemblyDefinition asm_ = md.get_assembly_definition();
    if ((asm_.flags() & 0x0200) == 0x0200) output_.write("windowsruntime ");
    output_.write(escape(md.get_string(asm_.name())));
    open_block(false);
    write_attributes(module, asm_.get_custom_attributes());
    write_security_declarations(module, asm_.get_declarative_security_attributes());
    if (!asm_.public_key().is_nil()) {
        output_.write(".publickey = ");
        write_blob(asm_.public_key(), md);
        output_.write_line();
    }
    if (asm_.hash_algorithm() != 0) {
        output_.write(std::format(".hash algorithm 0x{:08x}", asm_.hash_algorithm()));
        if (asm_.hash_algorithm() == 0x8004) output_.write(" // SHA1");
        output_.write_line();
    }
    const srm::Version v = asm_.version();
    output_.write_line(std::format(".ver {}:{}:{}:{}", v.major, v.minor, v.build, v.revision));
    close_block();
}

void ReflectionDisassembler::write_assembly_references(const srm::MetadataReader& md) {
    for (const srm::ModuleReferenceHandle m : metadata::get_module_references(md))
        output_.write_line(std::format(".module extern {}", escape(md.get_string(md.get_module_reference(m).name()))));
    for (const srm::AssemblyReferenceHandle a : md.assembly_references()) {
        const srm::AssemblyReference aref = md.get_assembly_reference(a);
        output_.write(".assembly extern ");
        if ((aref.flags() & 0x0200) == 0x0200) output_.write("windowsruntime ");
        output_.write(escape(md.get_string(aref.name())));
        open_block(false);
        if (!aref.public_key_or_token().is_nil()) {
            output_.write(".publickeytoken = ");
            write_blob(aref.public_key_or_token(), md);
            output_.write_line();
        }
        const srm::Version v = aref.version();
        output_.write_line(std::format(".ver {}:{}:{}:{}", v.major, v.minor, v.build, v.revision));
        close_block();
    }
}

void ReflectionDisassembler::write_module_header(const metadata::MetadataFile& module, bool skip_mvid) {
    const srm::MetadataReader& md = module.metadata();
    auto write_exported_type = [&](const srm::ExportedType& t) {
        if (!t.namespace_().is_nil()) {
            output_.write(escape(md.get_string(t.namespace_())));
            output_.write('.');
        }
        output_.write(escape(md.get_string(t.name())));
    };
    for (const srm::ExportedTypeHandle et : md.exported_types()) {
        const srm::ExportedType exported = md.get_exported_type(et);
        output_.write(".class extern ");
        if (exported.is_forwarder()) output_.write("forwarder ");
        write_exported_type(exported);
        open_block(false);
        switch (exported.implementation().kind()) {
        case HK::AssemblyFile: {
            const srm::AssemblyFile file = md.get_assembly_file(srm::AssemblyFileHandle(exported.implementation()));
            output_.write_line(std::format(".file {}", md.get_string(file.name())));
            const int type_def_id = exported.get_type_definition_id();
            if (type_def_id != 0) output_.write_line(std::format(".class 0x{:08x}", static_cast<std::uint32_t>(type_def_id)));
            break;
        }
        case HK::ExportedType: {
            output_.write(".class extern ");
            srm::ExportedType declaring = md.get_exported_type(srm::ExportedTypeHandle(exported.implementation()));
            while (true) {
                write_exported_type(declaring);
                if (declaring.implementation().kind() != HK::ExportedType) break;
                declaring = md.get_exported_type(srm::ExportedTypeHandle(declaring.implementation()));
            }
            output_.write_line();
            break;
        }
        case HK::AssemblyReference:
            output_.write(".assembly extern ");
            output_.write(escape(md.get_string(md.get_assembly_reference(srm::AssemblyReferenceHandle(exported.implementation())).name())));
            output_.write_line();
            break;
        default: throw srm::BadImageFormatError(0, "Implementation must either be an index into the File, ExportedType or AssemblyRef table.");
        }
        close_block();
    }
    const srm::ModuleDefinition module_definition = md.get_module_definition();
    output_.write_line(std::format(".module {}", md.get_string(module_definition.name())));
    if (!skip_mvid) {
        std::string guid = "{" + md.get_guid(module_definition.mvid()).to_string() + "}";
        for (char& c : guid) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
        output_.write_line(std::format("// MVID: {}", guid));
    }
    if (const auto* pe = dynamic_cast<const metadata::PEFile*>(&module)) {
        const srm::PEHeader& h = pe->reader().pe_header();
        output_.write_line(std::format(".imagebase 0x{:08x}", h.image_base));
        output_.write_line(std::format(".file alignment 0x{:08x}", h.file_alignment));
        output_.write_line(std::format(".stackreserve 0x{:08x}", h.size_of_stack_reserve));
        output_.write_line(std::format(".subsystem 0x{:x} // {}", h.subsystem, subsystem_name(h.subsystem)));
        const std::uint32_t flags = pe->reader().cor_header().flags;
        output_.write_line(std::format(".corflags 0x{:x} // {}", flags, cor_flags_name(flags)));
    }
    write_attributes(module, md.get_custom_attributes(srm::ModuleDefinitionHandle(1)));
}

std::string show_il(const metadata::MetadataFile& module) {
    PlainTextOutput output;
    output.write_line("// IL code: " + module.name());
    ReflectionDisassembler disassembler(output, util::CancellationToken());
    disassembler.write_module_contents(module);
    return output.text();
}

void ReflectionDisassembler::write_module_contents(const metadata::MetadataFile& module) {
    for (const srm::TypeDefinitionHandle h : process(module, metadata::get_top_level_type_definitions(module.metadata()))) {
        disassemble_type(module, h);
        output_.write_line();
    }
}

}  // namespace cb::dotnet::decompiler::disassembler
