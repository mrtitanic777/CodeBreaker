// dotnet/decompiler/metadata/metadata_file.cpp -- MetadataFile, PEFile and MetadataGenericContext.
#include "dotnet/decompiler/metadata/metadata_file.hpp"

#include "dotnet/decompiler/metadata/metadata_extensions.hpp"

namespace cb::dotnet::decompiler::metadata {

const std::string& MetadataFile::name() const {
    if (!name_) {
        const srm::MetadataReader& md = metadata();
        if (md.is_assembly()) name_ = std::string(md.get_string(md.get_assembly_definition().name()));
        else name_ = std::string(md.get_string(md.get_module_definition().name()));
    }
    return *name_;
}

const std::string& MetadataFile::full_name() const {
    if (!full_name_) full_name_ = metadata().is_assembly() ? get_full_assembly_name(metadata()) : name();
    return *full_name_;
}

TargetRuntime MetadataFile::get_runtime() const {
    const std::string& version = metadata().metadata_version();
    if (version.size() <= 1) return TargetRuntime::Unknown;
    switch (version[1]) {
    case '1':
        if (version.size() <= 3) return TargetRuntime::Unknown;
        // (as upstream: the character is compared with the number 1, so this is never 1.0)
        return version[3] == 1 ? TargetRuntime::Net_1_0 : TargetRuntime::Net_1_1;
    case '2': return TargetRuntime::Net_2_0;
    case '4': return TargetRuntime::Net_4_0;
    default: return TargetRuntime::Unknown;
    }
}

srm::MethodBodyBlock MetadataFile::get_method_body(int) const {
    throw srm::BadImageFormatError(0, "This metadata file does not contain method bodies.");
}
ByteView MetadataFile::get_section_data(int) const { throw srm::BadImageFormatError(0, "This metadata file does not support sections."); }
int MetadataFile::get_containing_section_index(int) const { throw srm::BadImageFormatError(0, "This metadata file does not support sections."); }
const std::vector<srm::SectionHeader>& MetadataFile::section_headers() const {
    throw srm::BadImageFormatError(0, "This metadata file does not support sections.");
}

PEFile::PEFile(std::filesystem::path file_name, std::unique_ptr<srm::PEReader> reader)
    : MetadataFile(Kind::PortableExecutable, std::move(file_name), reader->has_metadata() ? &reader->metadata() : nullptr), reader_(std::move(reader)) {
    if (!reader_->has_metadata()) throw MetadataFileNotSupportedError("PE file does not contain any managed metadata.");
}

std::shared_ptr<PEFile> PEFile::open(const std::filesystem::path& file) {
    auto reader = std::make_unique<srm::PEReader>(srm::PEReader::open(file));
    if (!reader->has_metadata()) throw MetadataFileNotSupportedError("PE file does not contain any managed metadata.");
    return std::make_shared<PEFile>(file, std::move(reader));
}

std::shared_ptr<PEFile> PEFile::from_bytes(std::filesystem::path file_name, Bytes image) {
    auto reader = std::make_unique<srm::PEReader>(srm::PEReader::from_bytes(std::move(image), file_name.string()));
    if (!reader->has_metadata()) throw MetadataFileNotSupportedError("PE file does not contain any managed metadata.");
    return std::make_shared<PEFile>(std::move(file_name), std::move(reader));
}

int PEFile::metadata_offset() const {
    const auto rva = reader_->cor_header().metadata_directory.relative_virtual_address;
    const auto off = reader_->pe_image().rva_to_offset(rva);
    return off ? static_cast<int>(*off) : 0;
}

srm::MethodBodyBlock PEFile::get_method_body(int rva) const { return reader_->get_method_body(static_cast<std::uint32_t>(rva)); }
ByteView PEFile::get_section_data(int rva) const { return reader_->get_section_data(static_cast<std::uint32_t>(rva)); }
int PEFile::get_containing_section_index(int rva) const { return reader_->get_containing_section_index(static_cast<std::uint32_t>(rva)); }

MetadataGenericContext::MetadataGenericContext(srm::MethodDefinitionHandle method, const srm::MetadataReader& metadata)
    : metadata_(&metadata), declaring_type_(metadata.get_method_definition(method).get_declaring_type()), method_(method) {}

std::string MetadataGenericContext::get_generic_type_parameter_name(int index) const {
    const srm::GenericParameterHandle p = get_generic_type_parameter_handle_or_null(index);
    if (p.is_nil() || metadata_ == nullptr) return std::to_string(index);
    return std::string(metadata_->get_string(metadata_->get_generic_parameter(p).name()));
}

std::string MetadataGenericContext::get_generic_method_type_parameter_name(int index) const {
    const srm::GenericParameterHandle p = get_generic_method_type_parameter_handle_or_null(index);
    if (p.is_nil() || metadata_ == nullptr) return std::to_string(index);
    return std::string(metadata_->get_string(metadata_->get_generic_parameter(p).name()));
}

srm::GenericParameterHandle MetadataGenericContext::get_generic_type_parameter_handle_or_null(int index) const {
    if (declaring_type_.is_nil() || index < 0 || metadata_ == nullptr) return {};
    const auto params = metadata_->get_type_definition(declaring_type_).get_generic_parameters();
    if (index >= static_cast<int>(params.size())) return {};
    return params[static_cast<std::size_t>(index)];
}

srm::GenericParameterHandle MetadataGenericContext::get_generic_method_type_parameter_handle_or_null(int index) const {
    if (method_.is_nil() || index < 0 || metadata_ == nullptr) return {};
    const auto params = metadata_->get_method_definition(method_).get_generic_parameters();
    if (index >= static_cast<int>(params.size())) return {};
    return params[static_cast<std::size_t>(index)];
}

}  // namespace cb::dotnet::decompiler::metadata
