#include "dotnet/decompiler/metadata/resource.hpp"

namespace cb::dotnet::decompiler::metadata {

std::string MetadataResource::name() const { return std::string(module_->metadata().get_string(module_->metadata().get_manifest_resource(handle_).name())); }

std::uint32_t MetadataResource::attributes() const { return module_->metadata().get_manifest_resource(handle_).attributes(); }

ResourceType MetadataResource::resource_type() const {
    const auto resource = module_->metadata().get_manifest_resource(handle_);
    const cb::dotnet::metadata::EntityHandle implementation = resource.implementation();
    if (implementation.is_nil()) return ResourceType::Embedded;
    if (implementation.kind() == cb::dotnet::metadata::HandleKind::AssemblyReference) return ResourceType::AssemblyLinked;
    return ResourceType::Linked;
}

bool MetadataResource::try_read_resource(ByteView& data) const {
    // embedded resources cannot be read from this binary.
    if (resource_type() != ResourceType::Embedded) return false;
    const cb::dotnet::metadata::CorHeader* cor_header = module_->cor_header();
    if (cor_header == nullptr) return false;
    const cb::dotnet::metadata::DirectoryEntry resources = cor_header->resources_directory;
    // validate resources directory (GetSectionData throws on negative RVAs)
    if (static_cast<std::int32_t>(resources.relative_virtual_address) <= 0) return false;
    const ByteView section_data = module_->get_section_data(static_cast<int>(resources.relative_virtual_address));
    // validate section length: we need at least 4 bytes to extract the actual length of the resource blob.
    if (section_data.size() < 4) return false;
    const std::int64_t offset = module_->metadata().get_manifest_resource(handle_).offset();
    // validate resource offset
    if (offset < 0 || offset > static_cast<std::int64_t>(section_data.size()) - 4) return false;
    const std::uint8_t* ptr = section_data.data() + offset;
    // get actual length of resource blob (an int: a high bit makes it negative).
    const std::int32_t length = static_cast<std::int32_t>(static_cast<std::uint32_t>(ptr[0]) | (static_cast<std::uint32_t>(ptr[1]) << 8) |
                                                          (static_cast<std::uint32_t>(ptr[2]) << 16) | (static_cast<std::uint32_t>(ptr[3]) << 24));
    if (length < 0 || static_cast<std::int64_t>(length) > static_cast<std::int64_t>(section_data.size())) return false;
    // upstream would read `length` bytes after the length field even when they run past the section; here that is no resource
    if (static_cast<std::int64_t>(length) > static_cast<std::int64_t>(section_data.size()) - offset - 4) return false;
    data = ByteView(ptr + 4, static_cast<std::size_t>(length));
    return true;
}

std::optional<ByteView> MetadataResource::try_open_stream() const {
    ByteView data;
    if (!try_read_resource(data)) return std::nullopt;
    return data;
}

std::optional<std::int64_t> MetadataResource::try_get_length() const {
    ByteView data;
    if (!try_read_resource(data)) return std::nullopt;
    return static_cast<std::int64_t>(data.size());
}

std::vector<std::unique_ptr<Resource>> get_resources(const MetadataFile& module) {
    std::vector<std::unique_ptr<Resource>> resources;
    for (const cb::dotnet::metadata::ManifestResourceHandle handle : module.metadata().manifest_resources())
        resources.push_back(std::make_unique<MetadataResource>(module, handle));
    return resources;
}

}  // namespace cb::dotnet::decompiler::metadata
